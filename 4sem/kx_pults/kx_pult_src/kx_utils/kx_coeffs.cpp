#include "kx_coeffs.h"
#include "kx_protocol_x.h"

double X[KX_X_COUNT];
PIVector<double> K;


KX_Coefficients::KX_Coefficients(const PIString & config, const PIString & name, bool _pult) {
	setName("kx_coeffs");
	PIConfig conf(config, PIIODevice::ReadOnly);
	fname = conf.getValue(name).getValue("file", "k.dat");
	readCoeffs();
	int ns = piMaxi(conf.getValue(name).getValue("count", 0), K.size_s());
	if (ns > K.size_s()) {
		K.resize(ns, 0.f);
		formulas.resize(ns);
		writeCoeffs();
	}
	config_ = config;
	name_ = name;
	pult_ = _pult;
	k_protocol = 0;
	renew();
	//readCoeffs();
}


KX_Coefficients::~KX_Coefficients() {
	stop();
	k_protocol->stop();
	delete k_protocol;
}


void KX_Coefficients::readCoeffs() {
	{
		PIConfig conf(fname, PIIODevice::ReadOnly);
		int cnt = conf.rootEntry().childCount();
		K.resize(cnt);
		formulas.resize(cnt);
		PIString kn, cf;
		for (int i = 0; i < cnt; i++) {
			kn = "k" + PIString::fromNumber(i);
			K[i] = (double)conf.getValue(kn, 0.f);
			cf = conf.getValue(kn + ".formula", "");
			if (cf.isEmpty()) cf = PIString::fromNumber(K[i]);
			formulas[i] = cf;
		}
	}
	k_content = PIFile(fname, PIIODevice::ReadOnly).readAll();
	//piCout << k_content.size();
	piCoutObj << "Readed" << K.size_s() << "coefficients in" << k_content.size_s() << "bytes";
	coeffs_cnt = K.size_s();
}


void KX_Coefficients::writeCoeffs() {
	PIFile::remove(fname);
	PIFile conf(fname, PIIODevice::ReadWrite);
	//conf.clear();
	for (int i = 0; i < K.size_s(); i++) {
		conf << "k" + PIString::fromNumber(i) << " = " << PIString::fromNumber(K[i]).replaceAll(",", ".") << "\n";
		conf << "k" + PIString::fromNumber(i) << ".formula = " << formulas[i] << "\n";
	}
	//conf.close();
	//conf.writeAll();
	k_content = conf.readAll();
	//piCout << k_content.size();
	piCoutObj << "Written" << K.size_s() << "coefficients in" << k_content.size_s() << "bytes";
	coeffs_cnt = K.size_s();
}


void KX_Coefficients::setKFile(const PIString & f) {
	fname = f;
	readCoeffs();
}


void KX_Coefficients::sendCoeffs() {
	stop();
	int curcnt, pcnt = (k_content.size_s() - 1) / __BYTES_IN_PACKET + 1;
	k_protocol->to_k.full_count = k_content.size_s();
	for (int i = 0; i < pcnt; i++) {
		k_protocol->to_k.first_index = i * __BYTES_IN_PACKET;
		if (i == pcnt - 1) curcnt = k_content.size_s() % __BYTES_IN_PACKET;
		else curcnt = __BYTES_IN_PACKET;
		//cout << "send " << curcnt << " coeffs\n";
		k_protocol->to_k.count = curcnt;
		memcpy(k_protocol->to_k.coeffs, k_content.data(k_protocol->to_k.first_index), curcnt);
		//for (int j = 0; j < curcnt; j++) k_protocol->to_k.coeffs[j] = K.at(k_protocol->to_k.first_index + j);
		k_protocol->send();
		piMSleep(5);
	}
	//cout << "waiting for commit ...\n";
	waitingCommit = true;
	tm.reset();
	start(10);
}


void KX_Coefficients::receiveCoeffs() {
	stop();
	k_protocol->to_k.full_count = k_protocol->to_k.first_index = k_protocol->to_k.count = -6;
	k_protocol->send();
	//cout << "request send\n";
	//cout << "waiting for receive ...\n";
	waitingCommit = false;
	tm.reset();
	start(10);
}


void KX_Coefficients::received(bool correct) {
	//cout << "kpi received " << correct << endl;
	if (!correct) return;
	k_syns = k_protocol->from_k;
	if (waitingCommit) {
		if (k_syns.first_index != -1 ||
			k_syns.full_count != -1 ||
			k_syns.count != -k_content.size_s()) return;
		//cout << "commit received\n";
		waitingCommit = false;
		stop();
		//cout << "send succeed\n";
		//emit sendSucceed();
		sendSucceed();
		return;
	} else {
		if (k_syns.first_index == -6 && k_syns.full_count == -6 && k_syns.count == -6) {
			//cout << "request received\n";
			sendCoeffs();
			return;
		};
	}
	if (k_syns.first_index < 0 || k_syns.count < 0) return;
	if (k_syns.first_index == 0) {
		//cout << "first receive\n";
		k_tmp.resize(k_syns.full_count);
		received_cnt = 0;
	}
	//for (int i = 0; i < k_syns.count; i++) K.at(k_syns.first_index + i) = k_syns.coeffs[i];
	//piCout << k_tmp.size() << k_syns.first_index << k_syns.count;
	memcpy(k_tmp.data(k_syns.first_index), k_syns.coeffs, k_syns.count);
	received_cnt += k_syns.count;
	tm.reset();
	//cout << "received " << k_syns.count << " bytes with " << k_syns.first_index << " first\n";
	if (received_cnt < k_syns.full_count) return;
	//cout << "finally received " << received_cnt << " bytes\n";
	stop();
	PIFile::remove(fname);
	PIFile conf(fname, PIIODevice::ReadWrite);
	conf.write(k_tmp.data(), k_tmp.size_s());
	conf.close();
	readCoeffs();
	//writeCoeffs();
	k_protocol->to_k.full_count = k_protocol->to_k.first_index = -1;
	k_protocol->to_k.count = -received_cnt;
	k_protocol->send();
	waitingCommit = false;
	//emit receiveSucceed();
	receiveSucceed();
	//cout << "receive succeed\n";
}


void KX_Coefficients::renew() {
	if (k_protocol != 0) {
		k_protocol->stop();
		delete k_protocol;
	}
	k_protocol = new __KPi(config_, name_, pult_);
	CONNECT1(void, bool, k_protocol, received, this, received)
	waitingCommit = false;
}


void KX_Coefficients::tick(void * , int ) {
	//piCout << "timer" << tm.elapsed_s();
	if (tm.elapsed_s() < 10.) return;
	if (waitingCommit) {
		//cout << "commit timeout\n";
		//cout << "send failed\n";
		//emit sendFailed();
		sendFailed();
	} else {
		//cout << "receive timeout\n";
		//cout << "receive failed\n";
		//emit receiveFailed();
		receiveFailed();
	}
	waitingCommit = false;
	stop();
}

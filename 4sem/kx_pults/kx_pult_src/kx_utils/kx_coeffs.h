#ifndef KX_COEFFS_H
#define KX_COEFFS_H

#include "piprotocol.h"

#define __BYTES_IN_PACKET 4000

extern PIVector<double> K;

class __KPi: public PIProtocol
{
	PIOBJECT_SUBCLASS(__KPi, PIProtocol)
public:
#pragma pack (push, 1)
	struct KX_K_Sync {
		uchar type;
		uchar addr_to;
		int count;
		int full_count;
		int first_index;
		uchar coeffs[__BYTES_IN_PACKET];
		uint checksum;
	};
#pragma pack (pop)
	__KPi(const PIString & config, const PIString & name, bool _pult = false): PIProtocol(config, name, &from_k, 2, &(from_k.count), sizeof(from_k) - 2, &to_k, sizeof(to_k)) {
		PIConfig conf(config, PIIODevice::ReadOnly);
        PIConfig::Entry ce = conf.getValue(name);
		to_k.type = from_k.type = ce.getValue("type", 0xA);
		to_k.addr_to = ce.getValue("addr_k", 0x15);
		from_k.addr_to = ce.getValue("addr_pult", 0x75);
		if (_pult) piSwap<uchar>(to_k.addr_to, from_k.addr_to);
		packetExtractor()->setHeader(PIByteArray(&from_k, 2));
		start();
	}
	KX_K_Sync to_k, from_k;
private:
	bool validate() {
		return (from_k.checksum == checksum_i(&from_k, sizeof(from_k) - 4));
	}
	bool aboutSend() {
		to_k.checksum = checksum_i(&to_k, sizeof(to_k) - 4);
		return true;
	}
};


class KX_Coefficients: public PITimer
{
	PIOBJECT_SUBCLASS(KX_Coefficients, PITimer)
	friend class KX_Pult;
public:
	KX_Coefficients(const PIString & config, const PIString & name = "k", bool _pult = false);
	~KX_Coefficients();
	
	void readCoeffs();
	void writeCoeffs();
	
	void sendCoeffs();
	void receiveCoeffs();

	bool isReady() const {return !waitingCommit;}
	const PIString & fileName() const {return fname;}
	const PIString * fileName_ptr() const {return &fname;}
	int coeffsCount() {coeffs_cnt = K.size_s(); return coeffs_cnt;}
	const int * coeffsCount_ptr() const {return &coeffs_cnt;}
	
	const PIString & formula(int index) const {return formulas[index];}
	void setFormula(int index, const PIString & f) {formulas[index] = f;}
	
	void setKFile(const PIString & f);
	
	EVENT(sendSucceed)
	EVENT(sendFailed)
	EVENT(receiveSucceed)
	EVENT(receiveFailed)

	
	__KPi * k_protocol;

private:
	EVENT_HANDLER1(void, received, bool, correct);
	void renew();
	void tick(void *, int);

	__KPi::KX_K_Sync k_syns;
	bool pult_, waitingCommit;
	int received_cnt, coeffs_cnt;
	PIString fname, config_, name_;
	PIStringList formulas;
	PIByteArray k_content, k_tmp;
	PITimeMeasurer tm;

};

#endif // KX_COEFFS_H

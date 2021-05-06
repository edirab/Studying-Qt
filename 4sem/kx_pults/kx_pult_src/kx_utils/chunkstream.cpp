#include "chunkstream.h"


void ChunkStream::setSource(const QByteArray & data) {
	stream_.setVersion(QDataStream::Qt_4_8);
	data_ = const_cast<QByteArray*>(&data);
	_init();
}


void ChunkStream::setSource(QByteArray * data) {
	stream_.setVersion(QDataStream::Qt_4_8);
	data_ = (data ? data : &tmp_data);
	_init();
}


void ChunkStream::setSource(QDataStream & str) {
	stream_.setVersion(QDataStream::Qt_4_8);
	str >> tmp_data;
	data_ = &tmp_data;
	_init();
}


ChunkStream::~ChunkStream() {

}


void ChunkStream::_init() {
	last_id = -1;
	last_data.clear();
	buffer.close();
	buffer.setBuffer(data_);
	buffer.open(QIODevice::ReadWrite);
	stream_.setDevice(&buffer);
}

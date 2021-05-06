#ifndef CHUNKSTREAM_H
#define CHUNKSTREAM_H

#include <QDataStream>
#include <QBuffer>
#include <QByteArray>


class ChunkStream
{
public:
	ChunkStream(const QByteArray & data) {setSource(data);}
	ChunkStream(QDataStream & str) {setSource(str);}
	ChunkStream(QByteArray * data = 0) {setSource(data);}
	~ChunkStream();
	
	template <typename T>
	struct Chunk {
		Chunk(int i, const T & d): id(i), data(d) {}
		int id;
		T data;
	};
	template <typename T> static Chunk<T> chunk(int id, const T & data) {return Chunk<T>(id, data);}
	
	void setSource(const QByteArray & data);
	void setSource(QDataStream & str);
	void setSource(QByteArray * data);
	QDataStream & dataStream() {return stream_;}
	QByteArray data() const {return tmp_data;}
	bool atEnd() const {return stream_.atEnd();}
	
	int read() {stream_ >> last_id >> last_data; return last_id;}
	int getID() {return last_id;}
	template <typename T>
	T getData() const {T ret; QDataStream s(last_data); s.setVersion(QDataStream::Qt_4_8); s >> ret; return ret;}
	template <typename T>
	void get(T & v) const {v = getData<T>();}
private:
	void _init();
	
	int last_id;
	QByteArray * data_, last_data, tmp_data;
	QBuffer buffer;
	QDataStream stream_;
	
};

template <typename T>
ChunkStream & operator <<(ChunkStream & s, const ChunkStream::Chunk<T> & c) {
	QByteArray ba;
	QDataStream bas(&ba, QIODevice::WriteOnly);
	bas.setVersion(QDataStream::Qt_4_8);
	bas << c.data;
	s.dataStream() << c.id << ba;
	return s;
}

#endif // CHUNKSTREAM_H

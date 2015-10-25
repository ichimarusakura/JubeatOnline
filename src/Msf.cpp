#include <iostream>
#include <fstream>
#include <winsock.h>
#include "Msf.h"

using namespace std;

jubeat_online::Msf::Msf(std::string filename) {
	this->sequences_ = new vector<Sequence>();
	if (filename.length() > 0) {
		this->LoadFromFile(filename);
	}
}

void jubeat_online::Msf::AddSequence(Sequence sequence) {
	this->sequences_->push_back(sequence);
}

void jubeat_online::Msf::AddSequence(SequenceType type, short int panel_number, int time) {
	Sequence seq = {type, panel_number, time};
	this->AddSequence(seq);
}

void jubeat_online::Msf::Save(string filename) {
	ofstream msf_stream(filename, ios::binary);
	if (msf_stream.fail()) {
		throw exception("Stream error");
	}

	for (Sequence seq: (*this->sequences_))
	{
		// 泥臭いバイトオーダ変換です
		int32_t buf;
		buf = htonl(seq.type) >> 16;
		msf_stream.write((char *)&buf, sizeof(seq.type));

		buf = htonl(seq.panel_number) >> 16;
		msf_stream.write((char *)&buf, sizeof(seq.panel_number));

		buf = htonl(seq.time);
		msf_stream.write((char *)&buf, sizeof(seq.time));
	}
	msf_stream.close();
}

void jubeat_online::Msf::LoadFromFile(string filename) {
	// ファイルを読み込みながら、AddSequenceを使ってMsfオブジェクトに変換する
	ifstream msf_stream(filename, ios::binary);
	if (msf_stream.fail()) {
		throw exception("Stream error");
	}

	// パラメータ読み込み用バッファ
	int16_t type;
	int16_t panel_number;
	int32_t time;
	// バイトオーダー変換用バッファ
	char buf;

	while (msf_stream.eof()) {
		Sequence sequence;
		msf_stream.read(&buf, 2);
		type = ntohs(buf);
		switch (type) {
		case 0x0000:
			sequence.type = Offset;
			break;
		case 0x0001:
			sequence.type = Bpm;
		case 0x0002:
			sequence.type = Note;
		case 0x0003:
			sequence.type = Hold;
		case 0x0004:
			sequence.type = Release;
		default:
			throw exception("invalid sequence type");
		}

		msf_stream.read(&buf, 2);
		panel_number = ntohs(buf);
		if (panel_number <= 0x0000 || panel_number >= 0x000F) {
			throw exception("invalid panel number");
		}
		sequence.panel_number = (panel_number);

		msf_stream.read(&buf, 4);
		time = ntohl(buf);
		sequence.time = time;
		this->AddSequence(sequence);
	}
	msf_stream.close();
}

jubeat_online::Msf::~Msf() {
	delete this->sequences_;
}
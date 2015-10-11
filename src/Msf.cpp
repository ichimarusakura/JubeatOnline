#include<iostream>
#include <fstream>
#include "Msf.h"

using namespace std;

enum SequenceType {
	Offset,
	Bpm,
	Note,
	Hold,
	Release,
};

typedef struct {
	SequenceType type;
	int panel_number;
	int time;
}Sequence;

Msf::Msf() {
	this->sequences_ = new vector<Sequence>();
}
void Msf::AddSequence(Sequence sequence) {
	this->sequences_->push_back(sequence);
}

void Msf::AddSequence(SequenceType type, int panel_number, int time) {
	Sequence seq = {type, panel_number, time};
	this->AddSequence(seq);
}

Msf * Msf::FromFile(string filename) {
	Msf * msf = new Msf();

	// ファイルを読み込みながら、AddSequenceを使ってMsfオブジェクトに変換する
	ifstream msf_stream(filename, ios::binary);
	if (msf_stream.fail()) {
		throw exception("Can't open file");
	}

	// パラメータ読み込み用バッファ
	char * type;
	char * panel_number;
	char * time;
	while (msf_stream.eof()) {
		msf_stream.read(type, 2);
		msf_stream.read(panel_number, 2);
		msf_stream.read(time, 4);

		// 読み込んだパラメータからシーケンスを生成
		Sequence sequence;
		switch ((int)type) {
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

		if ((int)panel_number <= 0x0000 || (int)panel_number >= 0x000F) {
			throw exception("invalid panel number");
		}

		sequence.panel_number = (*panel_number);
		sequence.time = (int)time;
		msf->AddSequence(sequence);
	}

	return msf;
}

Msf::~Msf() {
	delete this->sequences_;
}
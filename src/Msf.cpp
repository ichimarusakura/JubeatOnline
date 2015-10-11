#include<iostream>
#include <fstream>
#include "Msf.h"

using namespace std;

// C++11����typed enum���g�����I�I������ˁI�I
enum SequenceType : char {
	Offset = 0x0000,
	Bpm = 0x0001,
	Note = 0x0002,
	Hold = 0x0003,
	Release = 0x0004,
};

typedef struct {
	SequenceType type;
	char panel_number;
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

void Msf::Save(string filename) {
	ofstream msf_stream(filename, ios::binary);
	if (msf_stream.fail()) {
		throw exception("Stream error");
	}

	for (Sequence seq: (*this->sequences_))
	{
		// typed enum������switch����Ȃ��ˁI�I������˂��������I
		msf_stream << seq.type;
		msf_stream << seq.panel_number;
		msf_stream << seq.time;
	}
	msf_stream.close();
}

Msf * Msf::FromFile(string filename) {
	Msf * msf = new Msf();

	// �t�@�C����ǂݍ��݂Ȃ���AAddSequence���g����Msf�I�u�W�F�N�g�ɕϊ�����
	ifstream msf_stream(filename, ios::binary);
	if (msf_stream.fail()) {
		throw exception("Stream error");
	}

	// �p�����[�^�ǂݍ��ݗp�o�b�t�@
	char * type;
	char * panel_number;
	char * time;
	while (msf_stream.eof()) {
		msf_stream.read(type, 2);
		msf_stream.read(panel_number, 2);
		msf_stream.read(time, 4);

		// �ǂݍ��񂾃p�����[�^����V�[�P���X�𐶐�
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
	msf_stream.close();

	return msf;
}

Msf::~Msf() {
	delete this->sequences_;
}
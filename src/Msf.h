#pragma once

#include<string>
#include<vector>

namespace jubeat_online {

	enum SequenceType : int16_t {
		Offset = 0x0000,
		Bpm = 0x0001,
		Note = 0x0002,
		Hold = 0x0003,
		Release = 0x0004,
	};

	typedef struct {
		SequenceType type;
		short int panel_number;
		int time;
	}Sequence;

	class Msf
	{
	private:
		std::vector<Sequence> * sequences_;
	public:
		Msf(std::string filename = "");
		void AddSequence(Sequence sequence);
		void AddSequence(SequenceType type, short int panel_number, int time);
		void Save(std::string filename);
		void LoadFromFile(std::string filename);
		~Msf();
	};

}
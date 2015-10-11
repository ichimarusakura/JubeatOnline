#pragma once

#include<string>
#include<vector>

class Msf
{
private:
	std::vector<Sequence> * sequences_;
public:
	Msf();
	void AddSequence(Sequence sequence);
	void AddSequence(SequenceType type, int panel_number, int time);
	void Save(std::string filename);
	static Msf * FromFile(std::string filename);
	~Msf();
};
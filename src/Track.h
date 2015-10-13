#pragma once
#include<string>
#include"PlayData.h"

class Track
{
private:
	int min_bpm_;
	int max_bpm_;
	std::string title_;
	std::string artist_;
public:
	Track();
	//PlayData * GetEmptyPlayData();
	//PlayData * GetBestPlayData();
	~Track();
};
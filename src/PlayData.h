#pragma once
#include "Track.h"

class PlayData
{
private:
	int score_;
	bool is_finished_;
	int current_bpm;
	int shutter_angle_;
	Track track_;
public:
	PlayData();
	~PlayData();
};
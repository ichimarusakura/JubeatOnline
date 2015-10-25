#include "Player.h"

jubeat_online::Player::Player(std::string name, std::string description, std::string place)
{
	this->name_        = name;
	this->description_ = description;
	this->place_       = place;
}

std::string jubeat_online::Player::GetName() {
	return this->name_;
}

std::string jubeat_online::Player::GetDescription() {
	return this->description_;
}

std::string jubeat_online::Player::GetPlace() {
	return this->place_;
}

jubeat_online::Player::~Player()
{
}

#pragma once
#include <string>

namespace jubeat_online {

	class Player
	{
	private:
		std::string name_;
		std::string description_;
		std::string place_;
	public:
		Player(std::string name, std::string description, std::string place);
		std::string GetName();
		std::string GetDescription();
		std::string GetPlace();
		~Player();
	};

}
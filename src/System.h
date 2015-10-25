#pragma once
#include "SceneBase.h"
#include "Player.h"

namespace jubeat_online {
	class System
	{
	private:
		static SceneBase * current_scene_;
		static sf::RenderWindow * window_;
		static Player * myself_;
	public:
		System();
		static void Boot();
		static void LoadScene(SceneBase * next_scene);
		static sf::RenderWindow * GetWindow();
		static SceneBase * GetCurrentScene();
		static Player * GetMyself();
		static void ShutDown();
		~System();
	};
}
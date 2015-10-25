#pragma once
#include "SceneBase.h"

namespace jubeat_online {
	class System
	{
	private:
		static SceneBase * current_scene_;
		static sf::RenderWindow * window_;
	public:
		System();
		static void Boot();
		static void LoadScene(SceneBase * next_scene);
		static sf::RenderWindow * GetWindow();
		static SceneBase * GetCurrentScene();
		static void ShutDown();
		~System();
	};
}
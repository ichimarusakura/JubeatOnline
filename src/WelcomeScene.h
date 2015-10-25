#pragma once
#include"SceneBase.h"
#include "image_sequence.hpp"

namespace jubeat_online {

	class WelcomeScene : public SceneBase
	{
	private:
		sf::Texture * bg_texture_;
		jubeat_online::ImageSequence * marker_;
		bool marker_loaded_;
	public:
		WelcomeScene();
		void Init();
		void Update();
		~WelcomeScene();
	};
};
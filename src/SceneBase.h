#pragma once
#include <SFML/Graphics.hpp>

namespace jubeat_online {

	class SceneBase : public sf::RenderTexture
	{
	public:
		SceneBase();
		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual ~SceneBase();
	};
};
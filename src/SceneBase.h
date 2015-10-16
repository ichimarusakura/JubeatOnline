#pragma once
#include <SFML/Graphics.hpp>

namespace jubeat_online {

	class SceneBase
	{
	public:
		SceneBase();
		virtual void Init() = 0;
		virtual void Update(sf::RenderTexture * texture) = 0;
		virtual ~SceneBase();
	};
};
#pragma once
#ifndef JUBEAT_ONLINE_DIVIDED_IMAGE_HEADER
#define JUBEAT_ONLINE_DIVIDED_IMAGE_HEADER

#include <SFML/Graphics.hpp>

namespace jubeat_online {

	class DividedImage : public sf::Drawable, public sf::Transformable
	{
	public:

		bool load(const std::string& tileset, sf::Vector2u tileSize, unsigned int xnum, unsigned int ynum, unsigned int all_num);

		void setFrame(const int frame);

	private:

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const ;

		sf::VertexArray m_vertices_;
		sf::Texture m_tileset_;
		
		unsigned int xnum_;
		unsigned int ynum_;
		unsigned int all_num_;
		unsigned int frame_;

		sf::Vector2u tile_size_;

	};

}

#endif
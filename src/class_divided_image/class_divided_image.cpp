#include "divided_image.h"

bool jubeat_online::DividedImage::load(const std::string & tileset, sf::Vector2u tileSize, unsigned int xnum, unsigned int ynum, unsigned int all_num)
{

	if (all_num > xnum * ynum) return false;	//パラメータエラー

	// タイルセット画像を読み込んでテクスチャを作成
	
	// （m_tileset は sf::Texture です）

	if (!m_tileset_.loadFromFile(tileset)) {
		return false;
	}

	// 頂点リストの頂点数を調節。
	// マップチップの数に頂点数４を掛ける。チップは四角形だから。
	m_vertices_.setPrimitiveType(sf::Quads);
	m_vertices_.resize(xnum * ynum * 4);

	xnum_ = xnum;
	ynum_ = ynum;

	all_num_ = all_num;

	tile_size_ = tileSize;


	return true;
}


void jubeat_online::DividedImage::setFrame(const int frame)
{
	if (frame > all_num_) frame_ = 0;
	else frame_ = frame;


	int i = frame_ % xnum_;
	int j = frame_ / xnum_;

	// タイル番号の現在位置を取得
	int tileNumber = i + j * xnum_;

	// タイルセットの中での位置を求める。（テクスチャ座標）
	int tu = tileNumber % (m_tileset_.getSize().x / tile_size_.x);
	int tv = tileNumber / (m_tileset_.getSize().x / tile_size_.x);

	// 対応する４つの頂点へのポインタを取得
	sf::Vertex* quad = &m_vertices_[(i + j * xnum_) * 4];

	// 頂点４つを設定
	quad[0].position = sf::Vector2f(i    * tile_size_.x, j    * tile_size_.y);
	quad[1].position = sf::Vector2f((i + 1) * tile_size_.x, j    * tile_size_.y);
	quad[2].position = sf::Vector2f((i + 1) * tile_size_.x, (j + 1) * tile_size_.y);
	quad[3].position = sf::Vector2f(i    * tile_size_.x, (j + 1) * tile_size_.y);

	// 頂点４つのテクスチャ座標を設定
	quad[0].texCoords = sf::Vector2f(tu    * tile_size_.x, tv    * tile_size_.y);
	quad[1].texCoords = sf::Vector2f((tu + 1) * tile_size_.x, tv    * tile_size_.y);
	quad[2].texCoords = sf::Vector2f((tu + 1) * tile_size_.x, (tv + 1) * tile_size_.y);
	quad[3].texCoords = sf::Vector2f(tu    * tile_size_.x, (tv + 1) * tile_size_.y);

	setOrigin(static_cast<float>(tile_size_.x) * i, static_cast<float>(tile_size_.x) * j);
}

void jubeat_online::DividedImage::draw(sf::RenderTarget & target, sf::RenderStates states)const
{
	


	// トランスフォームを適用
	states.transform *= getTransform();

	// テクスチャ（タイルセット）をセット
	states.texture = &m_tileset_;

	// 頂点リストを描画
	target.draw(m_vertices_, states);
}
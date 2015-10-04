#include "divided_image.h"

bool jubeat_online::DividedImage::load(const std::string & tileset, sf::Vector2u tileSize, unsigned int xnum, unsigned int ynum, unsigned int all_num)
{

	if (all_num > xnum * ynum) return false;	//�p�����[�^�G���[

	// �^�C���Z�b�g�摜��ǂݍ���Ńe�N�X�`�����쐬
	
	// �im_tileset �� sf::Texture �ł��j

	if (!m_tileset_.loadFromFile(tileset)) {
		return false;
	}

	// ���_���X�g�̒��_���𒲐߁B
	// �}�b�v�`�b�v�̐��ɒ��_���S���|����B�`�b�v�͎l�p�`������B
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

	// �^�C���ԍ��̌��݈ʒu���擾
	int tileNumber = i + j * xnum_;

	// �^�C���Z�b�g�̒��ł̈ʒu�����߂�B�i�e�N�X�`�����W�j
	int tu = tileNumber % (m_tileset_.getSize().x / tile_size_.x);
	int tv = tileNumber / (m_tileset_.getSize().x / tile_size_.x);

	// �Ή�����S�̒��_�ւ̃|�C���^���擾
	sf::Vertex* quad = &m_vertices_[(i + j * xnum_) * 4];

	// ���_�S��ݒ�
	quad[0].position = sf::Vector2f(i    * tile_size_.x, j    * tile_size_.y);
	quad[1].position = sf::Vector2f((i + 1) * tile_size_.x, j    * tile_size_.y);
	quad[2].position = sf::Vector2f((i + 1) * tile_size_.x, (j + 1) * tile_size_.y);
	quad[3].position = sf::Vector2f(i    * tile_size_.x, (j + 1) * tile_size_.y);

	// ���_�S�̃e�N�X�`�����W��ݒ�
	quad[0].texCoords = sf::Vector2f(tu    * tile_size_.x, tv    * tile_size_.y);
	quad[1].texCoords = sf::Vector2f((tu + 1) * tile_size_.x, tv    * tile_size_.y);
	quad[2].texCoords = sf::Vector2f((tu + 1) * tile_size_.x, (tv + 1) * tile_size_.y);
	quad[3].texCoords = sf::Vector2f(tu    * tile_size_.x, (tv + 1) * tile_size_.y);

	setOrigin(static_cast<float>(tile_size_.x) * i, static_cast<float>(tile_size_.x) * j);
}

void jubeat_online::DividedImage::draw(sf::RenderTarget & target, sf::RenderStates states)const
{
	


	// �g�����X�t�H�[����K�p
	states.transform *= getTransform();

	// �e�N�X�`���i�^�C���Z�b�g�j���Z�b�g
	states.texture = &m_tileset_;

	// ���_���X�g��`��
	target.draw(m_vertices_, states);
}
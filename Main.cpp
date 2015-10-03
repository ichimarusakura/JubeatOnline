
#include "src\class_image_sequence\image_sequence.h"

#include <thread>
#include <iostream>

#include <SFML/Graphics.hpp>

int main(void){

	std::cout << "jubeat ONLINE version 0.1\n";
	
	sf::Vector2i win_pos(1920, -840);
	sf::RenderWindow window(sf::VideoMode(1080 , 1920), "jubeat ONLINE ver0.1",sf::Style::None);
	window.setPosition(win_pos);
	window.setVerticalSyncEnabled(true);
	sf::RenderTexture ScreenBuf;
	ScreenBuf.create(1080,1920);  //�o�b�t�@�����
	ScreenBuf.setSmooth(true);  //�X���[�X�ݒ�ON
	
	sf::Texture Gr;



	Gr.loadFromFile("media\\image.png");

	jubeat_online::ImageSequence is;
	is.LoadSequence("media\\clear.isf");

	//�ǂݍ���

	//�E�C���h�E���J���Ă���i�Q�[�����[�v�j
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			//�u�N���[�Y���v�����ꂽ�v�C�x���g�F�E�C���h�E�����
			if (event.type == sf::Event::Closed)
				window.close();
		}

		ScreenBuf.clear(sf::Color(0, 0, 0, 255));  //�o�b�t�@��ʂ����ŃN���A
												   //�Ȃɂ��`��

		sf::Sprite graph(Gr);
		graph.setOrigin(graph.getLocalBounds().width / 2.0f, graph.getLocalBounds().height / 2.0f);
		graph.setPosition(540, 960);
		ScreenBuf.draw(graph);

												   //�`��I���

		ScreenBuf.display();    //�o�b�t�@��ʂ��A�b�v�f�[�g
		sf::Sprite sprite(ScreenBuf.getTexture());  //�o�b�t�@��ʗp�̃X�v���C�g�����
		window.clear();     //��ʂ��N���A
		window.draw(sprite);    //�o�b�t�@��ʃe�N�X�`���̓������X�v���C�g����ʂɕ`��
								//���Ȃ݂�sf::Sprite��Position�̏����l��(0,0)�ł��B
		window.display();   //�`��A�b�v�f�[�g
	}

	return 0;
}
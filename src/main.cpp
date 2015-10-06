
#include "image_sequence.hpp"

#include <thread>
#include <iostream>

#include <math.h>

#include <SFML/Graphics.hpp>

int main(void){

	std::cout << "jubeat ONLINE version 0.1\n";
	
	sf::Vector2i win_pos(1920, -604);
	sf::RenderWindow window(sf::VideoMode(1080,1920), "jubeat ONLINE ver0.1",sf::Style::None);
	window.setPosition(win_pos);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(30);
	



	sf::RenderTexture ScreenBuf;
	ScreenBuf.create(768,1360);  //�o�b�t�@�����

	ScreenBuf.setSmooth(true);  //�X���[�X�ݒ�ON
	
	sf::Texture Gr;
	Gr.loadFromFile("media/media.png");

	jubeat_online::ImageSequence marker;
	marker.LoadDivGraph(25, 5, 5, 230, 230, "media\\shutter.png");


	//jubeat_online::ImageSequence is;
	//is.LoadSequence("media\\wtjp.isf");
	//is.set_is_repeat(true);
	
	int t = 0;

	//�E�C���h�E���J���Ă���i�Q�[�����[�v�j
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			//�u�N���[�Y���v�����ꂽ�v�C�x���g�F�E�C���h�E�����
			if (event.type == sf::Event::Closed)
				window.close();
		}

		ScreenBuf.clear(sf::Color(0, 0, 0, 255));  //�o�b�t�@��ʂ����ŃN���A
			
		sf::Sprite graph(Gr);
		graph.setOrigin(graph.getLocalBounds().width / 2.0f, graph.getLocalBounds().height / 2.0f);
		graph.setPosition(384, 660);
		graph.setRotation(-90);
		ScreenBuf.draw(graph);
		//�Ȃɂ��`��
		
		if (marker.WaitLoadComplete() == 0 && t == 0) {
			t = 1;
			marker.PlaySequence();
		}
		
		if (t == 1) {
			marker.DrawSequence(500, 500, &ScreenBuf);
		}
		



		ScreenBuf.display();    //�o�b�t�@��ʂ��A�b�v�f�[�g
		sf::Sprite sprite(ScreenBuf.getTexture());  //�o�b�t�@��ʗp�̃X�v���C�g�����
		sprite.setScale(1080.0f / 768.0f, 1920.0f / 1360.0f);

		window.clear();     //��ʂ��N���A
		
		window.draw(sprite);    //�o�b�t�@��ʃe�N�X�`���̓������X�v���C�g����ʂɕ`��
								//���Ȃ݂�sf::Sprite��Position�̏����l��(0,0)�ł��B
		window.display();   //�`��A�b�v�f�[�g

		//if (is.WaitLoadComplete() == 0) //std::cout << "end\n";
	}

	return 0;
}
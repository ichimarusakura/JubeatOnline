#include <thread>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include "image_sequence.hpp"
#include "Msf.h"
#include "SceneBase.h"
#include "WelcomeScene.h"

using namespace jubeat_online;

int main(void){

	std::cout << "jubeat ONLINE version 0.1\n";
	
	// Open window
	sf::Vector2i win_pos(50, 50);
	sf::RenderWindow window(sf::VideoMode(600,600), "jubeat ONLINE ver0.1");
	window.setPosition(win_pos);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(30);


	sf::RenderTexture * scene_texture = new sf::RenderTexture();
	scene_texture->create(768,1360);  //�o�b�t�@�����
	scene_texture->setSmooth(true);  //�X���[�X�ݒ�ON

	// initalize scene
	SceneBase * current_scene = new WelcomeScene();
	current_scene->Init();

	//�E�C���h�E���J���Ă���i�Q�[�����[�v�j
	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				delete current_scene;
				delete scene_texture;
				window.close();
			}
		}

		// ���݂̃V�[�����X�V
		current_scene->Update(scene_texture);
		
		// �V�[���p�o�b�t�@��`��
		scene_texture->display();
		//�o�b�t�@��ʗp�̃X�v���C�g�����
		sf::Sprite sprite(scene_texture->getTexture());
		sprite.setScale(1080.0f / 768.0f, 1920.0f / 1360.0f);
		
		//��ʂ��N���A
		window.clear();
		
		//�o�b�t�@��ʃe�N�X�`���̓������X�v���C�g����ʂɕ`��
		//���Ȃ݂�sf::Sprite��Position�̏����l��(0,0)�ł��B
		window.draw(sprite);

		window.display();
	}

	return 0;
}
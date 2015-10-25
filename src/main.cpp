#include <thread>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include "image_sequence.hpp"
#include "Msf.h"
#include "SceneBase.h"
#include "WelcomeScene.h"
#include "System.h"

using namespace jubeat_online;
using namespace std;

void DeleteResources(void);

int main(void){

	std::cout << "jubeat ONLINE version 0.1\n";
	
	// Open window
	System::Boot();

	// Shutdown�������ǂ����̃t���O�B�Ȃ񂩌���
	bool exit = false;

	//�E�C���h�E���J���Ă���i�Q�[�����[�v�j
	sf::RenderWindow * window = System::GetWindow();
	while (window->isOpen()) {
		sf::Event event;

		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				System::ShutDown();
				exit = true;
				break;
			}
		}
		if (exit) {
			break;
		}

		// �V�[�����ǂݍ��܂�Ă��Ȃ����WelcomeScene���B
		if (System::GetCurrentScene() == NULL) {
			System::LoadScene(new WelcomeScene());
		}
		SceneBase * current_scene = System::GetCurrentScene();

		// �V�[�����A�b�v�f�[�g
		current_scene->Update();
		current_scene->display();

		// �V�[���p�X�v���C�g�𐶐�
		sf::Sprite sprite(System::GetCurrentScene()->getTexture());
		//sprite.setScale(1080.0f / 768.0f, 1920.0f / 1360.0f);
		
		//��ʂ��N���A
		window->clear();
		
		//�o�b�t�@��ʃe�N�X�`���̓������X�v���C�g����ʂɕ`��
		//���Ȃ݂�sf::Sprite��Position�̏����l��(0,0)�ł��B
		window->draw(sprite);
		window->display();
	}

	return 0;
}
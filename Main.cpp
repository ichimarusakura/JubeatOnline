#include <SFML/Graphics.hpp>

#include <Windows.h>	//�ꎞ�I

#include <thread>

int n[500];

void DoThread(sf::Texture* a) {
	OutputDebugString("�X���b�h�J�n\n");
	for (int i = 0; i < 5; i++) {
		a[i].loadFromFile("media\\image2.png");

		a[i].setSmooth(true); //��������Ɗg��k���������Ƀo�C���j�A�␳��������
	}
	OutputDebugString("�ǂݍ��݂ł���\n");
}



int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::Texture Gr;
	sf::Texture P[500];



	Gr.loadFromFile("media\\image.png");    //test�Ƃ����e�N�X�`�������[�h�B
	Gr.setSmooth(true); //��������Ɗg��k���������Ƀo�C���j�A�␳��������

	printf("Aaa");

	int t = 0;

	while (window.isOpen()) {
		t++;
		if (t == 500) {
			std::thread t1(DoThread, P);
			t1.detach();

		}
		if (t == 10000) break;
		sf::Event event;
		while (window.pollEvent(event)) {
			//�u�N���[�Y���v�����ꂽ�v�C�x���g�F�E�C���h�E�����
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(); //��ʂ��N���A����i�����l�͍��j

						//��������摜�Ƃ���\��
		sf::Sprite sprite(Gr);  //��ʏ�ɕ\������X�v���C�g�����A�e�N�X�`����o�^
								//sprite.setColor(sf::Color(255, 0, 64, 128));   //RGBA=(255,0,64,128)�ŕ\��
								//sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);   //�摜�^�񒆂𒆐S�Ƃ���
								//sprite.setPosition(320, 240);    //��ʏ��(320,240)��
								//sprite.setScale(-2.0f, 0.5f);    //X������2�{�AY������1/2�{�Ɉ����L�΂���������X�����ɔ��]����
								//sprite.setRotation(90);     //�X�v���C�g��90�x��]����(�x���\�L�ł��邱�Ƃɒ���)

		window.draw(sprite, sf::RenderStates(sf::BlendAdd)); //���Z�����ŕ\��

															 //�����܂ŕ\������
		window.display();   //��ʏ�ɓ]��
	}
	
	OutputDebugString("�ʃX���b�h�œǂݍ��񂾉摜\n");

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			//�u�N���[�Y���v�����ꂽ�v�C�x���g�F�E�C���h�E�����
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(); //��ʂ��N���A����i�����l�͍��j

						//��������摜�Ƃ���\��
		sf::Sprite sprite(P[4]);  //��ʏ�ɕ\������X�v���C�g�����A�e�N�X�`����o�^
								   //sprite.setColor(sf::Color(255, 0, 64, 128));   //RGBA=(255,0,64,128)�ŕ\��
								   //sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);   //�摜�^�񒆂𒆐S�Ƃ���
								   //sprite.setPosition(320, 240);    //��ʏ��(320,240)��
								   //sprite.setScale(-2.0f, 0.5f);    //X������2�{�AY������1/2�{�Ɉ����L�΂���������X�����ɔ��]����
								   //sprite.setRotation(90);     //�X�v���C�g��90�x��]����(�x���\�L�ł��邱�Ƃɒ���)

		window.draw(sprite, sf::RenderStates(sf::BlendAdd)); //���Z�����ŕ\��

															 //�����܂ŕ\������
		window.display();   //��ʏ�ɓ]��
	}

	return 0;
}
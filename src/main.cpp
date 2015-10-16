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
using namespace std;

SceneBase * current_scene;
sf::RenderTexture * scene_texture;


void DeleteResources(void);

int main(void){

	std::cout << "jubeat ONLINE version 0.1\n";
	
	// Open window
	sf::Vector2i win_pos(50, 50);
	sf::RenderWindow window(sf::VideoMode(600,600), "jubeat ONLINE ver0.1");
	window.setPosition(win_pos);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(30);


	scene_texture = new sf::RenderTexture();
	scene_texture->create(768,1360);  //バッファを作る
	scene_texture->setSmooth(true);  //スムース設定ON

	// initalize scene
	current_scene = new WelcomeScene();
	current_scene->Init();
	bool exit = false;
	//ウインドウが開いている（ゲームループ）
	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				DeleteResources();
				exit = true;
			}
		}
		if (exit) {
			break;
		}

		// 現在のシーンを更新
		current_scene->Update(scene_texture);
		
		// シーン用バッファを描画
		scene_texture->display();
		//バッファ画面用のスプライトを作る
		sf::Sprite sprite(scene_texture->getTexture());
		sprite.setScale(1080.0f / 768.0f, 1920.0f / 1360.0f);
		
		//画面をクリア
		window.clear();
		
		//バッファ画面テクスチャの入ったスプライトを画面に描画
		//ちなみにsf::SpriteのPositionの初期値は(0,0)です。
		window.draw(sprite);

		window.display();
	}

	return 0;
}

void DeleteResources() {
	delete current_scene;
	delete scene_texture;
}
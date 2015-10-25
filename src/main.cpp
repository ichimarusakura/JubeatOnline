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

	// Shutdownしたかどうかのフラグ。なんか嫌だ
	bool exit = false;

	//ウインドウが開いている（ゲームループ）
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

		// シーンが読み込まれていなければWelcomeSceneを。
		if (System::GetCurrentScene() == NULL) {
			System::LoadScene(new WelcomeScene());
		}
		SceneBase * current_scene = System::GetCurrentScene();

		// シーンをアップデート
		current_scene->Update();
		current_scene->display();

		// シーン用スプライトを生成
		sf::Sprite sprite(System::GetCurrentScene()->getTexture());
		//sprite.setScale(1080.0f / 768.0f, 1920.0f / 1360.0f);
		
		//画面をクリア
		window->clear();
		
		//バッファ画面テクスチャの入ったスプライトを画面に描画
		//ちなみにsf::SpriteのPositionの初期値は(0,0)です。
		window->draw(sprite);
		window->display();
	}

	return 0;
}
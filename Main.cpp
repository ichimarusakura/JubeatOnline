
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
	ScreenBuf.create(1080,1920);  //バッファを作る
	ScreenBuf.setSmooth(true);  //スムース設定ON
	
	sf::Texture Gr;



	Gr.loadFromFile("media\\image.png");

	jubeat_online::ImageSequence is;
	is.LoadSequence("media\\clear.isf");

	//読み込み

	//ウインドウが開いている（ゲームループ）
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			//「クローズが要求された」イベント：ウインドウを閉じる
			if (event.type == sf::Event::Closed)
				window.close();
		}

		ScreenBuf.clear(sf::Color(0, 0, 0, 255));  //バッファ画面を黒でクリア
												   //なにか描画

		sf::Sprite graph(Gr);
		graph.setOrigin(graph.getLocalBounds().width / 2.0f, graph.getLocalBounds().height / 2.0f);
		graph.setPosition(540, 960);
		ScreenBuf.draw(graph);

												   //描画終わり

		ScreenBuf.display();    //バッファ画面をアップデート
		sf::Sprite sprite(ScreenBuf.getTexture());  //バッファ画面用のスプライトを作る
		window.clear();     //画面をクリア
		window.draw(sprite);    //バッファ画面テクスチャの入ったスプライトを画面に描画
								//ちなみにsf::SpriteのPositionの初期値は(0,0)です。
		window.display();   //描画アップデート
	}

	return 0;
}
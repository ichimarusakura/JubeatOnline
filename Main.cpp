#include <thread>
#include <iostream>

#include <math.h>

#include <SFML/Graphics.hpp>

#include "src/include/image_sequence.hpp"
#include "src/include/output_logtext.hpp"

int main(void){

	std::cout << "jubeat ONLINE version 0.1\n";
	
	sf::Vector2i win_pos(1920, -840);
	sf::RenderWindow window(sf::VideoMode(1080,1920), "jubeat ONLINE ver0.1",sf::Style::None);
	window.setPosition(win_pos);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(30);
	


	sf::RenderTexture ScreenBuf;
	ScreenBuf.create(768,1360);  //バッファを作る

	ScreenBuf.setSmooth(true);  //スムース設定ON
	
	sf::Texture Gr;
	Gr.loadFromFile("media/media.png");

	jubeat_online::ImageSequence marker;
	marker.set_is_repeat(true);
	marker.set_fps(24);
	marker.LoadDivGraph(25, 5, 5, 230, 230, "media\\shutter.png");


	jubeat_online::ImageSequence is;
	is.LoadSequence("media\\wtjp.isf");
	is.set_is_repeat(true);

	jubeat_online::ImageSequence is2;
	is2.LoadSequence("media\\wtjp.isf");
	is2.set_is_repeat(true);
	
	int t = 0;

	//ウインドウが開いている（ゲームループ）
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			//「クローズが要求された」イベント：ウインドウを閉じる
			if (event.type == sf::Event::Closed)
				window.close();
		}

		ScreenBuf.clear(sf::Color(0, 0, 0, 255));  //バッファ画面を黒でクリア
			
		sf::Sprite graph(Gr);
		graph.setOrigin(graph.getLocalBounds().width / 2.0f, graph.getLocalBounds().height / 2.0f);
		graph.setPosition(384, 660);
		graph.setRotation(-90);
		ScreenBuf.draw(graph);
		//なにか描画
		
		if (marker.WaitLoadComplete() == 0 && t == 0) {
			t = 1;
			marker.PlaySequence();
		}
		
		if (t == 1) {
			marker.DrawSequence(500, 500, &ScreenBuf);
			marker.DrawFrame(100, 100, 1.0, 15, &ScreenBuf);
		}
		



		ScreenBuf.display();    //バッファ画面をアップデート
		sf::Sprite sprite(ScreenBuf.getTexture());  //バッファ画面用のスプライトを作る
		sprite.setScale(1080.0f / 768.0f, 1920.0f / 1360.0f);

		window.clear();     //画面をクリア
		
		window.draw(sprite);    //バッファ画面テクスチャの入ったスプライトを画面に描画
								//ちなみにsf::SpriteのPositionの初期値は(0,0)です。
		window.display();   //描画アップデート

		//if (is.WaitLoadComplete() == 0) //std::cout << "end\n";
	}

	jubeat_online::OutputLogtext::Output("プログラム終了-----------------------\n");

	return 0;
}
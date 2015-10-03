#include <SFML/Graphics.hpp>

#include <Windows.h>	//一時的

#include <thread>

int n[500];

void DoThread(sf::Texture* a) {
	OutputDebugString("スレッド開始\n");
	for (int i = 0; i < 5; i++) {
		a[i].loadFromFile("media\\image2.png");

		a[i].setSmooth(true); //こうすると拡大縮小した時にバイリニア補正がかかる
	}
	OutputDebugString("読み込みできた\n");
}



int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::Texture Gr;
	sf::Texture P[500];



	Gr.loadFromFile("media\\image.png");    //testというテクスチャをロード。
	Gr.setSmooth(true); //こうすると拡大縮小した時にバイリニア補正がかかる

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
			//「クローズが要求された」イベント：ウインドウを閉じる
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(); //画面をクリアする（初期値は黒）

						//ここから画像とかを表示
		sf::Sprite sprite(Gr);  //画面上に表示するスプライトを作り、テクスチャを登録
								//sprite.setColor(sf::Color(255, 0, 64, 128));   //RGBA=(255,0,64,128)で表示
								//sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);   //画像真ん中を中心として
								//sprite.setPosition(320, 240);    //画面上の(320,240)に
								//sprite.setScale(-2.0f, 0.5f);    //X方向に2倍、Y方向に1/2倍に引き伸ばしたうえでX方向に反転させ
								//sprite.setRotation(90);     //スプライトを90度回転させ(度数表記であることに注意)

		window.draw(sprite, sf::RenderStates(sf::BlendAdd)); //加算合成で表示

															 //ここまで表示部分
		window.display();   //画面上に転送
	}
	
	OutputDebugString("別スレッドで読み込んだ画像\n");

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			//「クローズが要求された」イベント：ウインドウを閉じる
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(); //画面をクリアする（初期値は黒）

						//ここから画像とかを表示
		sf::Sprite sprite(P[4]);  //画面上に表示するスプライトを作り、テクスチャを登録
								   //sprite.setColor(sf::Color(255, 0, 64, 128));   //RGBA=(255,0,64,128)で表示
								   //sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);   //画像真ん中を中心として
								   //sprite.setPosition(320, 240);    //画面上の(320,240)に
								   //sprite.setScale(-2.0f, 0.5f);    //X方向に2倍、Y方向に1/2倍に引き伸ばしたうえでX方向に反転させ
								   //sprite.setRotation(90);     //スプライトを90度回転させ(度数表記であることに注意)

		window.draw(sprite, sf::RenderStates(sf::BlendAdd)); //加算合成で表示

															 //ここまで表示部分
		window.display();   //画面上に転送
	}

	return 0;
}
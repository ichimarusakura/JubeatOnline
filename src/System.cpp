#include "System.h"
#include "SceneBase.h"

sf::RenderWindow * jubeat_online::System::window_;
jubeat_online::SceneBase * jubeat_online::System::current_scene_;

jubeat_online::System::System() {}

void jubeat_online::System::Boot() {
	sf::Vector2i win_pos(50, 50);
	jubeat_online::System::window_ = new sf::RenderWindow(sf::VideoMode(600, 600), "jubeat ONLINE ver0.1");
	jubeat_online::System::window_->setPosition(win_pos);
	jubeat_online::System::window_->setVerticalSyncEnabled(true);
	jubeat_online::System::window_->setFramerateLimit(30);
}

void jubeat_online::System::LoadScene(jubeat_online::SceneBase * next_scene) {
	if (jubeat_online::System::current_scene_ != NULL) {
		delete jubeat_online::System::current_scene_;
	}
	jubeat_online::System::current_scene_ = next_scene;
	jubeat_online::System::current_scene_->Init();
}

sf::RenderWindow * jubeat_online::System::GetWindow() {
	return jubeat_online::System::window_;
}

jubeat_online::SceneBase * jubeat_online::System::GetCurrentScene() {
	return jubeat_online::System::current_scene_;
}

void jubeat_online::System::ShutDown() {
	delete jubeat_online::System::current_scene_;

	// RenderWindowクラスのデストラクタには、close()の処理が含まれています
	delete jubeat_online::System::window_;
}

jubeat_online::System::~System() {}
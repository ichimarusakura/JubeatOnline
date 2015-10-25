#include "System.h"
#include "SceneBase.h"
#include "ini_filereader.hpp"
#include <iostream>

using namespace std;

sf::RenderWindow * jubeat_online::System::window_;
jubeat_online::SceneBase * jubeat_online::System::current_scene_;
jubeat_online::Player * jubeat_online::System::myself_;

jubeat_online::System::System() {}

void jubeat_online::System::Boot() {
	INIFileReader config_reader;
	config_reader.LoadFile("config.ini");
	
	// Load Layout
	string layout_mode = config_reader.getValue("layout", "mode", "vertical");
	int panel_size = stoi(config_reader.getValue("layout", "panel_size", "100"));
	int panel_margin = stoi(config_reader.getValue("layout", "panel_margin", "15"));
	int top_display_height = stoi(config_reader.getValue("layout", "top_display_height", "280"));
	int top_display_margin = stoi(config_reader.getValue("layout", "top_display_margin", "100"));

	// Calc window size
	int window_width = panel_size * 4 + panel_margin * 5;
	int window_height = window_width + top_display_height + top_display_margin;
	if (layout_mode == "horizontal") {
		int tmp = window_width;
		window_width = window_height;
		window_height = tmp;
	}

	// Open window
	sf::Vector2i win_pos(50, 50);
	jubeat_online::System::window_ = new sf::RenderWindow(sf::VideoMode(window_width, window_height), "jubeat ONLINE ver0.1", sf::Style::Titlebar|sf::Style::Close);
	jubeat_online::System::window_->setPosition(win_pos);
	jubeat_online::System::window_->setVerticalSyncEnabled(true);
	jubeat_online::System::window_->setFramerateLimit(30);

	// Load player info
	string player_place       = config_reader.getValue("player", "place", "未設定");
	string player_name        = config_reader.getValue("player", "name", "GUEST");
	string player_description = config_reader.getValue("player", "description", "よろしくお願いします");
	jubeat_online::System::myself_ = new Player(player_name, player_description, player_description);
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

jubeat_online::Player * jubeat_online::System::GetMyself() {
	return jubeat_online::System::myself_;
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
#include "WelcomeScene.h"
#include<iostream>
using namespace std;
jubeat_online::WelcomeScene::WelcomeScene()
{
}
void jubeat_online::WelcomeScene::Init() {
	this->create(768, 1360);
	this->setSmooth(true);
	this->bg_texture_ = new sf::Texture();
	this->bg_texture_->loadFromFile("media/media.png");

	this->marker_ = new ImageSequence();
	this->marker_->LoadDivGraph(25, 5, 5, 230, 230, "media\\shutter.png");
	this->marker_loaded_ = false;
}

void jubeat_online::WelcomeScene::Update() {

	//バッファ画面を黒でクリア
	this->clear(sf::Color(0, 0, 0, 255));

	sf::Sprite graph(*this->bg_texture_);
	graph.setOrigin(graph.getLocalBounds().width / 2.0f, graph.getLocalBounds().height / 2.0f);
	graph.setPosition(384, 660);
	graph.setRotation(-90);
	this->draw(graph);

	if (this->marker_->WaitLoadComplete() == 0 && !this->marker_loaded_) {
		this->marker_loaded_ = true;
		this->marker_->PlaySequence();
	}

	if (this->marker_loaded_) {
		this->marker_->DrawSequence(200, 200, this);
	}
}

jubeat_online::WelcomeScene::~WelcomeScene() {
	delete this->bg_texture_;
	delete this->marker_;
}
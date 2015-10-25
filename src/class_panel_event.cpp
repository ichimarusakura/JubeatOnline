
//author white
//2015/10/23 made

//class_panel_event.cpp
//panel_event.hppの内部

#include "panel_event.hpp"
#include <thread>

jubeat_online::PanelEventQueue * jubeat_online::PanelEvent::que_head = NULL;
jubeat_online::PanelEventQueue * jubeat_online::PanelEvent::que_tail = NULL;
unsigned int jubeat_online::PanelEvent::que_num_ = 0;
jubeat_online::PanelEventConsole jubeat_online::PanelEvent::console_;
std::mutex jubeat_online::PanelEvent::mtx_;

int jubeat_online::PanelEvent::Init(const std::string ini_fpath) {
	
	//初期化


	//iniファイル読み込み


	//別スレッド立ち上げ
	std::thread check_th;

	//テスト

	return 0;
}


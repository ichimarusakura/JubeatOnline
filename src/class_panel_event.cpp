
//author white
//2015/10/23 made

//class_panel_event.cpp
//panel_event.hpp�̓���

#include "panel_event.hpp"
#include <thread>

jubeat_online::PanelEventQueue * jubeat_online::PanelEvent::que_head = NULL;
jubeat_online::PanelEventQueue * jubeat_online::PanelEvent::que_tail = NULL;
unsigned int jubeat_online::PanelEvent::que_num_ = 0;
jubeat_online::PanelEventConsole jubeat_online::PanelEvent::console_;
std::mutex jubeat_online::PanelEvent::mtx_;

int jubeat_online::PanelEvent::Init(const std::string ini_fpath) {
	
	//������


	//ini�t�@�C���ǂݍ���


	//�ʃX���b�h�����グ
	std::thread check_th;

	//�e�X�g

	return 0;
}


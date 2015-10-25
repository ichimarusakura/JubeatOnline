haina#pragma once
#ifndef JUBEAT_ONLINE_PANEL_EVENT_HEADER
#define JUBEAT_ONLINE_PANEL_EVENT_HEADER


//***************TEMPORARY***************//
//#include "msf.h"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <mutex>

namespace jubeat_online {
	enum SequenceType : int16_t {
		Offset = 0x0000,
		Bpm = 0x0001,
		Note = 0x0002,
		Hold = 0x0003,
		Release = 0x0004
	};

	typedef struct {
		SequenceType type;
		short int panel_number;
		int time;
	}Sequence;



	class Scene : public sf::RenderTexture{
	public:
		Scene() {}
		virtual void Init() {}
		virtual int Update() { return 0; }
		~Scene() {}
	protected:
	private:
	};



	class Position {
	public:
		int x, y;
	};

	class Panel {
	public:
		Position pos;
	};

	class Me {
	public:
		Panel a[16];
		Panel getPos(int n) { return a[n]; }
	};
}



namespace jubeat_online {

	/// <summary>1�����̃p�l���̉����������i�[����Ă���N���X</summary>
	class PanelEventQueue {
	public:
		/// <summary>Sequence�^�̃f�[�^���e�B</summary>
		Sequence data_;

	private:
		//�����N���X�g���̗v�f�̃A�h���X
		PanelEventQueue * next_element_;
	};

	/// <summary>�p�l�����͂̐ݒ�A�`�F�b�N������Scene�N���X�p��</summary>
	class PanelEventConsole : public Scene {
	public:
		/// <summary>�p�l�����͂̐ݒ�y�у`�F�b�N�����邽�߂̃R���\�[����ʂ�W�J���܂��B</summary>
		/// <param name='window'>��ʃo�b�t�@</param>
		void Update(sf::RenderTexture window);
		
	};

	/// <summary>�p�l���̓��͂��擾���邽�߂̃N���X</summary>
	class PanelEvent {
	public:

		/// <summary>���������s���֐��B�R���X�g���N�^�Ƃ͂܂��ʁB���̎��ʃX���b�h���쐬����B</summary>
		/// <param name='ini_fpath'>���������邽�߂�ini�t�@�C���p�X�B</param>
		/// <returns>0:���� -1:�ُ픭��</returns>
		static int Init(const std::string ini_fpath);

		/// <summary>�Z�b�e�B���O�R���\�[���Ƃ��đ��݂���Scene�N���X�p���ȃN���X���擾�B</summary>
		/// <returns>�R���\�[����ʂ�W�J����Scene�N���X���p������PanelEventConsole���擾</returns>
		static PanelEventConsole console(void);

		/// <summary>1���̃C�x���g���A���L���[����B</summary>
		/// <returns>�p�l���C�x���g�����̗v�f��Ԃ��B</returns>
		static PanelEventQueue getEvent(void);

		/// <summary>�L���[�v�f����Ԃ�</summary>
		/// <returns>�L���[�v�f��</returns>
		static unsigned int get_queue_num(void);

		/// <summary>�L���[�������L�^���邩�ǂ����̃t���O��ݒ肷��Bfalse�ɂ����ꍇ�A���łɂ������L���[�͑S�Ĕj�������B</summary>
		/// <param name='flag'>true�Ȃ�L�^����Afalse�Ȃ痚�������������L�^���Ȃ�</param>
		static void is_queue(const bool flag);

		/// <summary>�������̃p�l���g��\������B�֐����Ă΂ꂽ�^�C�~���O�ŉ�����Ă�����̘̂g�Ɍ���B</summary>
		/// <param name='window'>��ʃo�b�t�@</param>
		/// <returns>0:���� -1:�ُ픭��</returns>
		static int DrawFrame(sf::RenderTexture window);

		/// <summary>�I�[�o�[�t���[���Ă��邩�m�F����B���̊֐����Ă񂾏ꍇ�A�t���O�̓��Z�b�g�����B</summary>
		/// <returns>�I�[�o�[�t���[�t���O</returns>
		static bool is_overflow(void);

	private:
		static PanelEventQueue * que_head;
		static PanelEventQueue * que_tail;

		static unsigned int que_num_;

		static PanelEventConsole console_;

		static std::mutex mtx_;
	};

}


#endif
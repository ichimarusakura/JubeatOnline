//***************************************************
//*   ImageSequence Class Library					*
//*													*
//*		Programmed by �ق킢��						*
//***************************************************


#pragma once
#ifndef JUBEAT_ONLINE_IMAGESEQUENCE_H_
#define JUBEAT_ONLINE_IMAGESEQUENCE_H_

#include <mutex>

#include <SFML/Graphics.hpp>


namespace jubeat_online {

	//c_ImageSequence�֐��ɂ���
	//	�V�[�P���X�摜�����ƂɃA�j���[�V������W�J���܂��B
	//	���̊֐��ŁA�V�[�P���X�摜�̃��[�h����`��܂ł��ׂčs���܂�
	//	�������A�V�[�P���X�摜�𓮉�Ƃ��čĐ�����v���O�����ł��̂�
	//	���W��\����ԂȂǂ͊O������w�肷��K�v������܂��B

	/// <summary>ImageSequence�N���X�́A���ʂ�߂�l�Ƃ���֐��́A�߂�l�̌^</summary>
	enum ImageSequenceResult {
		OK = 0,
		LOAD_ERROR = -1,
		MALFORMED_FILE = -2,
		NULL_FILEPATH = -3,
		OUT_OF_MEMORY = -4,
	};

	/// <summary>�V�[�P���X�摜�����ƂɃA�j���[�V������W�J���܂��B
	/// ���̃N���X�ŁA�V�[�P���X�̉摜�̃��[�h����`��܂ł��ׂčs���܂��B</summary>
	class ImageSequence {
	private:
		char* filename_;				//�t�@�C�����i�[

		sf::Texture* images_;			//�V�[�P���X�摜��ێ�
		unsigned int all_image_frame_;	//�S���̃V�[�P���X�摜����
		unsigned int now_frame_;		//�Đ����A�������͈ꎞ��~���̌��݂̃t���[���ԍ�
		unsigned int fps_;				//����fps


		bool is_repeat_;				//�J��Ԃ���
		unsigned int in_frame_;			//���s�[�g�p�A�擪�t���[��
		unsigned int out_frame_;		//���s�[�g�p�A�ŏI�t���[��

		float x_, y_;						//�\��������W
		sf::Clock started_time_;		//�X�^�[�g����(ms)
		unsigned int started_frame_;
		bool is_play_;

		float exrate_;					//�g��k���{��
		
		bool is_loaded_;				//�ǂݍ��݊���������
		bool is_allocated_;				//�������̊m�ۂȂ�
		ImageSequenceResult load_result_;

		std::mutex mtx_;

		void LoadThread();
		void LoadDivThread(const int x_div, const int y_div, const int width, const int height);

	public:
		ImageSequence();
		~ImageSequence();
		
		// *** �ǂݍ��݁A�Đ����� ***

		/// <summary>�V�[�P���X�摜�̃t�@�C���p�X��o�^���܂��B
		/// ���̊֐����Ăяo�������_�ł̓��[�h�����͔������܂���</summary>
		/// <param name='filename'>�V�[�P���X�摜��Ǝ��́@�`���ł܂Ƃ߂��t�@�C���̃p�X�t�����O</param>
		jubeat_online::ImageSequenceResult SetSequenceFilename(const char* filename);

		/// <summary>�V�[�P���X�摜�̃��[�h�������擾���܂��B</summary>
		/// <returns>�ǂݍ��ݑ҂��t�@�C�����B0�Ŋ����A���̒l�Ń��[�h�Ɏ��s</returns>
		int WaitLoadComplete(void);

		/// <summary>�V�[�P���X�摜��ǂݍ��݂܂��B</summary>
		/// <param name='filename'>�V�[�P���X�摜��Ǝ��̌`���ł܂Ƃ߂��t�@�C���̃p�X�t�����O�BSetSequenceFilename���Ăяo���Ă���ꍇ��NULL�w�肵�܂��B
		/// �����ł��t�@�C������^�����ꍇ�́A������̏���D�悵�܂�</param>
		/// <returns>0:���� -1:���[�h���s -2:�t�@�C���`���s�� -3:�t�@�C�����w��Ȃ�</returns>
		/// <remarks>�w�肳�ꂽ�`���̏ڍׂ�ImageSequence.txt���Q�Ƃ�������</remarks>
		ImageSequenceResult LoadSequence(const char* filename = NULL);

		/// <summary>�V�[�P���X�摜�𐮗�z�u�摜����ǂݍ��݂܂��B</summary>
		/// <param name='all_framecount'>��荞�މ摜�̑�����</param>
		/// <param name='x_div'>X�������̕�����</param>
		/// <param name='y_div'>Y�������̕�����</param>
		/// <param name='width'>1�R�}�̕�</param>
		/// <param name='height'>1�R�}�̍���</param>
		/// <param name='filename'>�V�[�P���X�摜��Ǝ��̌`���ł܂Ƃ߂��t�@�C���̃p�X�t�����O�BSetSequenceFilename���Ăяo���Ă���ꍇ��NULL�w�肵�܂��B
		/// �����ł��t�@�C������^�����ꍇ�́A������̏���D�悵�܂�</param>
		/// <returns>ImageSequenceResult�^</returns>
		/// <remarks>�w�肳�ꂽ�`���̏ڍׂ�ImageSequence.txt���Q�Ƃ�������</remarks>
		ImageSequenceResult LoadDivGraph(const int all_framecount, const int x_div, const int y_div, const int width, const int height, const char *filename = NULL);


		/// <summary>�V�[�P���X�摜�𓮉�Ƃ��čĐ����܂��B���̊֐����ĂԑO��x,y���W���w�肵�Ă����K�v������܂�</summary>
		/// <param name='frame'>�Đ����J�n����t���[���ԍ��ł��B�f�t�H���g��0�t���[������ł�</param>
		/// <returns>int�^�B���������ꍇ��0�A�Đ��Ɏ��s�����ꍇ��-1��Ԃ��܂�</returns>
		ImageSequenceResult PlaySequence(const unsigned int frame = 0);

		/// <summary>�V�[�P���X�摜��`�ʂ��܂��B�Đ����ɕ`�ʂł��܂��B</summary>
		/// <param name='x'>�\������x���W</param>
		/// <param name='y'>�\������y���W</param>
		/// <param name='ex'>�g�嗦</param>
		/// <param name='screen_buffer'>�X�N���[���`�ʗp�o�b�t�@</param>
		/// <returns>int�^�B���������ꍇ�̓t���[���ԍ��A�Đ��Ɏ��s�����ꍇ��-1��Ԃ��܂�</returns>
		int DrawSequence(const float x, const float y, const float ex, sf::RenderTexture* screen_buffer);

		/// <summary>�V�[�P���X�摜��`�ʂ��܂��B�Đ����ɕ`�ʂł��܂��B</summary>
		/// <param name='ex'>�g�嗦</param>
		/// <param name='screen_buffer'>�X�N���[���`�ʗp�o�b�t�@</param>
		/// <returns>int�^�B���������ꍇ�̓t���[���ԍ��A�Đ��Ɏ��s�����ꍇ��-1��Ԃ��܂�</returns>
		int DrawSequence(const float x, const float y, sf::RenderTexture* screen_buffer);

		/// <summary>�V�[�P���X�摜��`�ʂ��܂��B�Đ����ɕ`�ʂł��܂��B</summary>
		/// <returns>int�^�B���������ꍇ�̓t���[���ԍ��A�Đ��Ɏ��s�����ꍇ��-1��Ԃ��܂�</returns>
		int DrawSequence(sf::RenderTexture* screen_buffer);
		
		/// <summary>�V�[�P���X�摜���t���[���Î~��Ƃ��ĕ`�ʂ��܂��B</summary>
		/// <param name='frame'>�\���������t���[��</param>
		/// <param name='screen_buffer'>�X�N���[���`�ʃo�b�t�@</param>
		/// <returns>int�^�B���������ꍇ��0�A���s�����ꍇ��-1��Ԃ��܂�</returns>
		int DrawFrame(const float x, const float y, const float ex, const unsigned int frame, sf::RenderTexture* screen_buffer);


		/// <summary>�V�[�P���X�̍폜���s���܂��B</summary>
		void DeleteSequence(void);
		

		// *** �����o�ϐ�����֐� ***

		/// <summary>X���W�̐ݒ���s���܂�</summary>
		/// <param name='x'>�V�����ݒ肷��X���W�ł�</param>
		void set_x(const float x);

		/// <summary>X���W�̎擾���s���܂�</summary>
		/// <returns>int�^X���W��Ԃ��܂�</returns>
		float x(void) const;


		/// <summary>Y���W�̐ݒ���s���܂�</summary>
		/// <param name='x'>�V�����ݒ肷��Y���W�ł�</param>
		void set_y(const float y);

		/// <summary>Y���W�̎擾���s���܂�</summary>
		/// <returns>int�^Y���W��Ԃ��܂�</returns>
		float y(void) const;


		/// <summary>���[�v���̃C���|�C���g�̐ݒ���s���܂�
		/// ���s�[�g�t���O�������Ă���A�A�E�g�|�C���g�ɒB�����ꍇ�A���̃t���[�����烊�s�[�g����܂�</summary>
		/// <param name='frame'>�C���|�C���g�Ƃ��Đݒ肷��t���[���ԍ��ł�</param>
		void set_in_frame(const unsigned int frame);

		/// <summary>���[�v���̃C���|�C���g�̎擾���s���܂�</summary>
		/// <returns>unsigned int�^�ŃC���|�C���g�t���[���ԍ���Ԃ��܂�</returns>
		unsigned int in_frame(void) const;


		/// <summary>���[�v���̃A�E�g�|�C���g�̐ݒ���s���܂�
		/// ���s�[�g�t���O�������Ă���A�A�E�g�|�C���g�ɒB�����ꍇ�A�C���|�C���g�t���[���փ��s�[�g����܂�</summary>
		/// <param name='frame'>�A�E�g�|�C���g�Ƃ��Đݒ肷��t���[���ԍ��ł�</param>
		void set_out_frame(const unsigned int frame);

		/// <summary>���[�v���̃A�E�g�|�C���g�̎擾���s���܂�</summary>
		/// <returns>unsigned int�^�ŃA�E�g�|�C���g�t���[���ԍ���Ԃ��܂�</returns>
		unsigned int out_frame(void) const;
		
		/// <summary>���[�v���̃C���|�C���g�̏��������s���܂��B��������s����ƃC���|�C���g�͐擪�̃t���[���ɐݒ肳��܂�</summary>
		void InitInPoint(void);

		/// <summary>���[�v���̃A�E�g�|�C���g�̏��������s���܂��B��������s����ƃA�E�g�|�C���g�͍Ō���̃t���[���ɐݒ肳��܂�</summary>
		void InitOutPoint(void);
		

		/// <summary>���s�[�g���邩�̃t���O��ݒ肵�܂�</summary>
		/// <param name='flag'>���s�[�g����ꍇ��true�A���Ȃ��ꍇ��false���w��</param>
		void set_is_repeat(const bool flag);

		/// <summary>���s�[�g���邩�̃t���O���擾���܂�</summary>
		/// <returns>���s�[�g�ݒ�̏ꍇtrue�A���s�[�g���Ȃ��ꍇfalse���Ԃ���܂�</returns>
		bool is_repeat(void) const;

		/// <summary>fps���Z�b�g���܂�</summary>
		/// <param name=""></param>
		void set_fps(const unsigned int fps);


	};



}


#endif
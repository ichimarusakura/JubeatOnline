//***************************************************
//*   ImageSequence Class Library					*
//*													*
//*		Programmed by �ق킢��						*
//***************************************************


#pragma once
#ifndef JUBEAT_ONLINE_IMAGESEQUENCE_H_
#define JUBEAT_ONLINE_IMAGESEQUENCE_H_

#ifndef NULL
#define NULL 0
#endif

namespace jubeatOnline {

	//c_ImageSequence�֐��ɂ���
	//	�V�[�P���X�摜�����ƂɃA�j���[�V������W�J���܂��B
	//	���̊֐��ŁA�V�[�P���X�摜�̃��[�h����`��܂ł��ׂčs���܂�
	//	�������A�V�[�P���X�摜�𓮉�Ƃ��čĐ�����v���O�����ł��̂�
	//	���W��\����ԂȂǂ͊O������w�肷��K�v������܂��B

	/// <summary>�V�[�P���X�摜�����ƂɃA�j���[�V������W�J���܂��B
	/// ���̃N���X�ŁA�V�[�P���X�̉摜�̃��[�h����`��܂ł��ׂčs���܂��B</summary>
	class c_ImageSequence {
	private:
		char* filename_;				//�t�@�C�����i�[

		int* images_;					//�V�[�P���X�摜��ێ�
		unsigned int all_image_frame_;	//�S���̃V�[�P���X�摜����
		unsigned int now_frame_;		//�Đ����A�������͈ꎞ��~���̌��݂̃t���[���ԍ�
		unsigned int fps_;				//����fps

		bool is_loaded_;				//�ǂݍ��݊���������

		bool is_repeat_;				//�J��Ԃ���
		unsigned int in_frame_;			//���s�[�g�p�A�擪�t���[��
		unsigned int out_frame_;		//���s�[�g�p�A�ŏI�t���[��

		int x_, y_;						//�\��������W
		unsigned int started_time_;		//�X�^�[�g����(ms)

		bool is_expand;					//�g��k���\�����邩
		double exrate;					//�g��k���{��
		
	public:
		
		// *** �ǂݍ��݁A�Đ����� ***

		/// <summary>�V�[�P���X�摜�̃t�@�C���p�X��o�^���܂��B
		/// ���̊֐����Ăяo�������_�ł̓��[�h�����͔������܂���</summary>
		/// <param name='filename'>�V�[�P���X�摜��Ǝ��́@�`���ł܂Ƃ߂��t�@�C���̃p�X�t�����O</param>
		void SetSequenceFilename(const char* filename);

		/// <summary>�V�[�P���X�摜��ǂݍ��݂܂��B</summary>
		/// <param name='filename'>�V�[�P���X�摜��Ǝ��̌`���ł܂Ƃ߂��t�@�C���̃p�X�t�����O�BSetSequenceFilename���Ăяo���Ă���ꍇ��NULL�w�肵�܂��B
		/// �����ł��t�@�C������^�����ꍇ�́A������̏���D�悵�܂�</param>
		/// <returns>0:���� -1:���[�h���s -2:�t�@�C���`���s�� -3:�t�@�C�����w��Ȃ�</returns>
		/// <remarks>�w�肳�ꂽ�`���̏ڍׂ�ImageSequence.txt���Q�Ƃ�������</remarks>
		int LoadSequence(const char* filename = NULL);

		/// <summary>�V�[�P���X�摜�𐮗�z�u�摜����ǂݍ��݂܂��B</summary>
		/// <param name='all_framecount'>��荞�މ摜�̑�����</param>
		/// <param name='x_div'>X�������̕�����</param>
		/// <param name='y_div'>Y�������̕�����</param>
		/// <param name='width'>1�R�}�̕�</param>
		/// <param name='height'>1�R�}�̍���</param>
		/// <param name='filename'>�V�[�P���X�摜��Ǝ��̌`���ł܂Ƃ߂��t�@�C���̃p�X�t�����O�BSetSequenceFilename���Ăяo���Ă���ꍇ��NULL�w�肵�܂��B
		/// �����ł��t�@�C������^�����ꍇ�́A������̏���D�悵�܂�</param>
		/// <returns>����:0 ���[�h�Ɏ��s:-1 �t�@�C���`���s��:-2 �t�@�C�����w��Ȃ�:-3</returns>
		/// <remarks>�w�肳�ꂽ�`���̏ڍׂ�ImageSequence.txt���Q�Ƃ�������</remarks>
		int LoadDivGraph(const int all_framecount, const int x_div, const int y_div, const int width, const int height, const char *filename = NULL);

		/// <summary>�V�[�P���X�摜�𓮉�Ƃ��čĐ����܂�</summary>
		/// <param name='x'>�`�ʂ���x���W�������Ŏw�肵�܂�</param>
		/// <param name='y'>�`�ʂ���y���W�������Ŏw�肵�܂�</param>
		/// <param name='frame'>�Đ����J�n����t���[���ԍ��ł��B�f�t�H���g��0�t���[������ł�</param>
		/// <returns>int�^�B���������ꍇ��0�A�Đ��Ɏ��s�����ꍇ��-1��Ԃ��܂�</returns>
		int PlaySequence(const int x, const int y, const unsigned int frame = 0);

		/// <summary>�V�[�P���X�摜�𓮉�Ƃ��čĐ����܂��B���̊֐����ĂԑO��x,y���W���w�肵�Ă����K�v������܂�</summary>
		/// <param name='frame'>�Đ����J�n����t���[���ԍ��ł��B�f�t�H���g��0�t���[������ł�</param>
		/// <returns>int�^�B���������ꍇ��0�A�Đ��Ɏ��s�����ꍇ��-1��Ԃ��܂�</returns>
		int PlaySequence(const unsigned int frame = 0);
		

		// *** �����o�ϐ�����֐� ***

		/// <summary>X���W�̐ݒ���s���܂�</summary>
		/// <param name='x'>�V�����ݒ肷��X���W�ł�</param>
		void X(const int x);

		/// <summary>X���W�̎擾���s���܂�</summary>
		/// <returns>int�^X���W��Ԃ��܂�</returns>
		int X(void) const;


		/// <summary>Y���W�̐ݒ���s���܂�</summary>
		/// <param name='x'>�V�����ݒ肷��Y���W�ł�</param>
		void Y(const int y);

		/// <summary>Y���W�̎擾���s���܂�</summary>
		/// <returns>int�^Y���W��Ԃ��܂�</returns>
		int Y(void);


		/// <summary>���[�v���̃C���|�C���g�̐ݒ���s���܂�
		/// ���s�[�g�t���O�������Ă���A�A�E�g�|�C���g�ɒB�����ꍇ�A���̃t���[�����烊�s�[�g����܂�</summary>
		/// <param name='frame'>�C���|�C���g�Ƃ��Đݒ肷��t���[���ԍ��ł�</param>
		void InPoint(const unsigned int frame);

		/// <summary>���[�v���̃C���|�C���g�̎擾���s���܂�</summary>
		/// <returns>unsigned int�^�ŃC���|�C���g�t���[���ԍ���Ԃ��܂�</returns>
		unsigned int InPoint(void) const;


		/// <summary>���[�v���̃A�E�g�|�C���g�̐ݒ���s���܂�
		/// ���s�[�g�t���O�������Ă���A�A�E�g�|�C���g�ɒB�����ꍇ�A�C���|�C���g�t���[���փ��s�[�g����܂�</summary>
		/// <param name='frame'>�A�E�g�|�C���g�Ƃ��Đݒ肷��t���[���ԍ��ł�</param>
		void OutPoint(const unsigned int frame);

		/// <summary>���[�v���̃A�E�g�|�C���g�̎擾���s���܂�</summary>
		/// <returns>unsigned int�^�ŃA�E�g�|�C���g�t���[���ԍ���Ԃ��܂�</returns>
		unsigned int OutPoint(void) const;
		
		/// <summary>���[�v���̃C���|�C���g�̏��������s���܂��B��������s����ƃC���|�C���g�͐擪�̃t���[���ɐݒ肳��܂�</summary>
		void InitInPoint(void);

		/// <summary>���[�v���̃A�E�g�|�C���g�̏��������s���܂��B��������s����ƃA�E�g�|�C���g�͍Ō���̃t���[���ɐݒ肳��܂�</summary>
		void InitOutPoint(void);
		

		/// <summary>���s�[�g���邩�̃t���O��ݒ肵�܂�</summary>
		/// <param name='flag'>���s�[�g����ꍇ��true�A���Ȃ��ꍇ��false���w��</param>
		void RepeatFlag(const bool flag);

		/// <summary>���s�[�g���邩�̃t���O���擾���܂�</summary>
		/// <returns>���s�[�g�ݒ�̏ꍇtrue�A���s�[�g���Ȃ��ꍇfalse���Ԃ���܂�</returns>
		bool RepeatFlag(void) const;

	};


	typedef c_ImageSequence		ImageSequence;
	typedef c_ImageSequence*	lpImageSequence;

}


#endif
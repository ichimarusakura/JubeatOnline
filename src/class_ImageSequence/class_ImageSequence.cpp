//***********************************************
//*		ImageSequence Class Source File			*
//*			Programmed by white					*
//***********************************************

#include "ImageSequenceClass.h"

#include <stdio.h>
#include <stdlib.h>


int jubeatOnline::c_ImageSequence::LoadSequence(const char * filename){

	//�V�[�P���X�摜�t�@�C���̃t�@�C�����̏���
	//if (filename != NULL) SetSequenceFilename(filename);
	//else if (filename_ == NULL) return -3;		//�t�@�C�������w�肵��
	
	//�V�[�P���X�摜��ǂݍ���

	//�t�@�C�����J��
	FILE* fp;
	if (fopen_s(&fp, filename, "rb") != 0) {
		//�t�@�C���̊J���Ɏ��s
		return -1;
	}

	char tmp[4096];
	unsigned char type = 0x00, pass = 0x00, length = 0x00, fps = 0x00;
	

	//�܂����ʎq�̎擾
	if ((type = fgetc(fp)) == EOF) return -2;	//�t�@�C�������Ȃ�����

	//�ȈՃp�X���[�h
	if ((pass = fgetc(fp)) == EOF) return -2;	//�t�@�C�������Ȃ�����

	//long�o�C�g����
	if ((length = fgetc(fp)) == EOF) return -2;	//�t�@�C�������Ȃ�����

	//�t���[�����[�g
	if ((fps = fgetc(fp)) == EOF) return -2;	//�t�@�C�������Ȃ�����

	for (all_image_frame_ = 0; feof(fp) == 0; all_image_frame_++) {
		//�t�@�C�������擾
		fgets((char*)tmp, length + 1, fp);

		long size = 0;
		for (int i = length-1; i >= 0; i--) {
			size <<= 8;
			size |= (0x000000ff & tmp[i]);
		}
	
		unsigned char* data = new unsigned char[size + 1];

		//fgets(
	}

	
	

	



	return 0;
}
/*
void jubeatOnline::c_ImageSequence::RepeatFlag(const bool flag)
{

}*/

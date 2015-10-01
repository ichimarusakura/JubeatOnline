//***********************************************
//*		ImageSequence Class Source File			*
//*			Programmed by white					*
//***********************************************

#include "ImageSequence.h"

#include "DxLib.h"

#include <stdio.h>
#include <stdlib.h>


int jubeatOnline::c_ImageSequence::LoadSequence(const char * filename){

	int ret = 0;

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

	unsigned char type = 0x00, pass = 0x00, length = 0x00, fps = 0x00;
	
	do {

		//�܂����ʎq�̎擾
		if (fread_s(&type, sizeof(char), 1, 1, fp) < 1) { ret = -2; break; }	//�t�@�C�������Ȃ�����

		//�ȈՃp�X���[�h
		if (fread_s(&pass, sizeof(char), 1, 1, fp) < 1) { ret = -2; break; }	//�t�@�C�������Ȃ�����

		//long�o�C�g����
		if (fread_s(&length, sizeof(char), 1, 1, fp) < 1) { ret = -2; break; }	//�t�@�C�������Ȃ�����

		//�t���[�����[�g
		if (fread_s(&fps, sizeof(char), 1, 1, fp) < 1) { ret = -2; break; }	//�t�@�C�������Ȃ�����
		fps_ = static_cast<unsigned int>(fps);

		//���t���[����
		char tm[2];
		all_image_frame_ = 0;
		if (fread_s(tm, sizeof(char)*2, 1, 2, fp) < 2) { ret = -2; break; }	//�t�@�C�������Ȃ�����
		for (int i = 0; i < 2; i++) {
			int t = (0x000000ff & tm[i]);
			t <<= 8 * i;
			all_image_frame_ |= t;
		}

		//�摜�t���[���i�[�̈�
		images_ = new int[all_image_frame_];
		if (images_ == NULL) {
			ret = -4;
			break;
		}else do {

			unsigned int loaded = 0;	//�ǂݍ��݂����������t���[����
			for (loaded = 0; loaded < all_image_frame_; loaded++) {

				//���[�v�ɓ������㎸�s�����ꍇ�A�摜�̃�������������Ȃ���΂Ȃ�Ȃ�
				//�ǂݍ��܂ꂽ�摜��loaded���ƂȂ�
				
				//�t�@�C�������擾
				long size = 0;

				//�t�@�C���T�C�Y�i�[�p�z��m�ہi���I�j
				char* size_str = new char[length];
				if (size_str == NULL) {
					ret = -4;
					break;
				}else do {

					//�ǂݍ��݁i�T�C�Y�j
					if (fread_s(size_str, length, 1, length, fp) < length) {
						ret = -2;
						break;
					}

					for (int i = 0; i < length; i++) {
						long t = (0x000000ff & size_str[i]);
						t <<= 8 * i;
						size |= t;
					}

					//�f�[�^�{�̂������Ă���̈�m��
					unsigned char* data = new unsigned char[size];
					if (data == NULL) {
						ret = -4;
						break;
					}
					else do {

						//�t�@�C����ǂݎ��
						if (fread_s(data, size, 1, size, fp) < size) {
							ret = -2;
							break;
						}

						for (int i = 0; i < size; i++) {
							//�𓀍��
							data[i] ^= pass;
						}

						if ((images_[loaded] = CreateGraphFromMem(data, size)) == -1) {
							//�摜���ǂݍ��߂Ȃ������ꍇ
							ret = -1;
							break;
						}
					} while (0);

					delete [] data;

				} while (0);

				delete [] size_str;

			}//end of for
			if (ret != 0) {
				//�G���[�����������ꍇ�̓ǂݍ��񂾉摜�ɑ΂���J������
				for (unsigned int i = 0; i < loaded; i++) DeleteGraph(images_[i]);

			}

			if (loaded != all_image_frame_) return -2;	//�������Ȃ�


		} while (0);

		if (ret != 0) {
			//�G���[�����������ꍇ�����Aimage_���J������
			delete[] images_;
		}

	} while (0);


	if(ret == 0) is_loaded_ = true;

	return ret;	//����
}
/*
void jubeatOnline::c_ImageSequence::RepeatFlag(const bool flag)
{

}*/
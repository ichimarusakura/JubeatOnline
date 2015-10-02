//***********************************************
//*		ImageSequence Class Source File			*
//*			Programmed by white					*
//***********************************************

#include "image_sequence.h"

#include "DxLib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <thread>
#include <exception>




void jubeat_online::ImageSequence::LoadData(int* dst, const int length,FILE* fp,const unsigned char pass) {


	unsigned int loaded = 0;	//�ǂݍ��݂����������t���[����
	for (loaded = 0; loaded < all_image_frame_; loaded++) {


		//���[�v�ɓ������㎸�s�����ꍇ�A�摜�̃�������������Ȃ���΂Ȃ�Ȃ�
		//�ǂݍ��܂ꂽ�摜��loaded���ƂȂ�

		//�t�@�C�������擾
		long size = 0;

		//�t�@�C���T�C�Y�i�[�p�z��m�ہi���I�j
		//char* size_str = new char[length];
		if (size_str == NULL) {
			load_result_ = -4;
			break;
		}
		else do {

			//�ǂݍ��݁i�T�C�Y�j
			if (fread_s(size_str, length, 1, length, fp) < length) {
				load_result_ = -2;
				break;
			}

			for (int i = 0; i < length; i++) {
				long t = (0x000000ff & size_str[i]);
				t <<= 8 * i;
				size |= t;
			}

			//�f�[�^�{�̂������Ă���̈�m��
			unsigned char* ndata = (unsigned char*)malloc(sizeof(unsigned char) * size);
			files_[loaded] = ndata;
			if (ndata == NULL) {
				load_result_ = -4;
				break;
			}
			else do {

				//�t�@�C����ǂݎ��
				if (fread_s(ndata, sizeof(char) * size, 1, size, fp) < (size_t)size) {
					load_result_ = -2;
					break;
				}

				for (int i = 0; i < size; i++) {
					//�𓀍��
					ndata[i] ^= pass;
				}




				try {
					dst[loaded] = CreateGraphFromMem(ndata, (int)size);
				}
				catch (std::exception &ex) {
					std::cerr << ex.what() << std::endl;
					load_result_ = -5;
					break;
				}
				

				ProcessMessage();

			} while (0);
			//free(ndata);


		} while (0);


	}//end of for
	if (load_result_ != 0) {
		//�G���[�����������ꍇ�̓ǂݍ��񂾉摜�ɑ΂���J������
		for (unsigned int i = 0; i < loaded; i++) DeleteGraph(images_[i]);

	}

	fclose(fp);


	is_loaded_ = true;


}


int jubeat_online::ImageSequence::LoadSequence(jubeat_online::ImageSequence* me, const char * filename){


	//
	//�V�[�P���X�摜�t�@�C���̃t�@�C�����̏���
	//if (filename != NULL) SetSequenceFilename(filename);
	//else if (filename_ == NULL) return -3;		//�t�@�C�������w�肵��
	
	//TO DO����SetSequence��ǉ����邱��
	//temporary
	is_loaded_ = false;
	size_t leng = strlen(filename);
	//if(filename_ == NULL)
	filename_ = new char[leng + 1];
	if (filename_ == NULL) return -4;

	strcpy_s(filename_,leng + 1, filename);

	//�V�[�P���X�摜��ǂݍ���

	int ret = 0;
	//�t�@�C�����J��
	FILE* fp;
	if (fopen_s(&fp, filename_, "rb") != 0) {
		//�t�@�C���̊J���Ɏ��s
		ret = -1;
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
		if (fread_s(tm, sizeof(char) * 2, 1, 2, fp) < 2) { ret = -2; break; }	//�t�@�C�������Ȃ�����
		for (int i = 0; i < 2; i++) {
			int t = (0x000000ff & tm[i]);
			t <<= 8 * i;
			all_image_frame_ |= t;
		}
		size_str = new unsigned char[length];

		//�摜�t���[���i�[�̈�
		images_ = (int*)malloc(sizeof(int) * all_image_frame_);
		files_ = (unsigned char**)malloc(sizeof(unsigned char*) * all_image_frame_);
		if (images_ == NULL || files_ == NULL){
			if (images_ != NULL) free(images_);
			if (files_ != NULL) free(files_);
			ret = -4;
			break;
		}
		else do {

			LoadData(images_, length, fp, pass);

		} while (0);


		delete[] size_str;

	} while (0);


	if (ret == 0) {
		is_loaded_ = true;
		//���ׂĂ̓ǂݍ��݂��I���܂ő҂�
	}

	return ret;	//����

}

int jubeat_online::ImageSequence::WaitLoadComplete(void) {
	if (is_loaded_ == false) return all_image_frame_;

	int as = 0;
		loaded_num_ = 0;
		success_num_ = 0;
		// �ǂݍ��݂��I����Ă�����摜��`�悷��
		for (unsigned int i = 0; i < all_image_frame_; i++)
		{
			//ProcessMessage();
			as = CheckHandleASyncLoad(images_[i]);
			if(as == FALSE)
			{
				if (files_[i] != NULL) {
					free(files_[i]);
					files_[i] = NULL;
				}
				//DrawGraph(i * 32, 0, images_[i], TRUE);
			}
			else if (as == -1) {
				return -1;
			}
		}
	if(as > 0) return as;
	//���[�h������
	return (success_num_ - loaded_num_);
}

/*
void jubeatOnline::c_ImageSequence::RepeatFlag(const bool flag)
{

}*/

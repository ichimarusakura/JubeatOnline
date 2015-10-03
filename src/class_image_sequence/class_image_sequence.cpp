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


jubeat_online::ImageSequence::ImageSequence() {
	filename_ = NULL;
	files_ = NULL;
	images_ = NULL;

	all_image_frame_ = 0;
	now_frame_ = 0;
	fps_ = 0;

	is_repeat_ = 0;
	in_frame_ = 0;
	out_frame_ = 0;

	x_ = 0;
	y_ = 0;
	started_time_ = 0;
	is_expand = 0;
	exrate = 1.0f;

	is_allocated_ = false;
	failed_num_ = 0;

}

jubeat_online::ImageSequence::~ImageSequence() {
	DeleteSequence();
}



jubeat_online::ImageSequenceResult	jubeat_online::ImageSequence::LoadSequence(const char * filename){

	//***********************************************************************
	//�֐���:LoadSeqeunce
	//���@��:�Ǝ��`���̘A���ŉ摜�̓ǂݍ��ݖ��߂𔭐������܂�
	//		 ���̊֐����Ă΂��܂łɁADxLib::SetUseASyncLoadFlag(TRUE)��
	//		 �Ă΂�Ă���K�v������܂��B�Ă΂Ȃ��ꍇ�͂��̊֐�����
	//		 ���S�Ƀ��[�h���������܂��B
	//�߂�l:ImageSequenceResult�^
	//***********************************************************************

	ImageSequenceResult ret = ImageSequenceResult::OK;

	//�V�[�P���X�摜�t�@�C���̃t�@�C�����̏���
	if (filename != NULL) {
		ImageSequenceResult fret = SetSequenceFilename(filename);
		if(fret != ImageSequenceResult::OK) return fret;
	}

	else if (filename_ == NULL) return ImageSequenceResult::NULL_FILEPATH;		//�t�@�C�������w�肵��
	
	//TO DO����SetSequence��ǉ����邱��
	//temporary
	
	if (is_allocated_) DeleteSequence();
	

	//�V�[�P���X�摜��ǂݍ���
	//�t�@�C�����J��
	FILE* fp;
	if (fopen_s(&fp, filename_, "rb") != 0) {
		//�t�@�C���̊J���Ɏ��s
		return ImageSequenceResult::LOAD_ERROR;
	}
	
	unsigned char type = 0x00, pass = 0x00, length = 0x00, fps = 0x00;

	do {

		//�܂����ʎq�̎擾
		if (fread_s(&type, sizeof(char), 1, 1, fp) < 1) { ret = ImageSequenceResult::MALFORMED_FILE; break; }

		//�ȈՃp�X���[�h
		if (fread_s(&pass, sizeof(char), 1, 1, fp) < 1) { ret = ret = ImageSequenceResult::MALFORMED_FILE; break; }

		//long�o�C�g����
		if (fread_s(&length, sizeof(char), 1, 1, fp) < 1) { ret = ret = ImageSequenceResult::MALFORMED_FILE; break; }

		//�t���[�����[�g
		if (fread_s(&fps, sizeof(char), 1, 1, fp) < 1) { ret = ret = ImageSequenceResult::MALFORMED_FILE; break; }
		fps_ = static_cast<unsigned int>(fps);

		//���t���[����
		char tm[2];
		all_image_frame_ = 0;
		if (fread_s(tm, sizeof(char) * 2, 1, 2, fp) < 2) { ret = ret = ImageSequenceResult::MALFORMED_FILE; break; }
		for (int i = 0; i < 2; i++) {
			unsigned int t = static_cast<unsigned int>(0x000000ff & tm[i]);
			t <<= 8 * i;
			all_image_frame_ |= t;
		}
		
		unsigned char* size_str = new unsigned char[length];

		//�摜�t���[���i�[�̈�
		images_ = static_cast<int*>(malloc(sizeof(int) * all_image_frame_));
		files_ = static_cast<unsigned char**>(malloc(sizeof(unsigned char*) * all_image_frame_));
		if (images_ == NULL || files_ == NULL){
			if (images_ != NULL) free(images_);
			if (files_ != NULL) free(files_);
			ret = ImageSequenceResult::OUT_OF_MEMORY;
			break;
		}
		else do {

			unsigned int loaded = 0;	//�ǂݍ��݂����������t���[����
			for (loaded = 0; loaded < all_image_frame_ && ret == ImageSequenceResult::OK; loaded++) {


				//���[�v�ɓ������㎸�s�����ꍇ�A�摜�̃�������������Ȃ���΂Ȃ�Ȃ�
				//�ǂݍ��܂ꂽ�摜��loaded���ƂȂ�

				//�t�@�C�������擾
				long size = 0;

				//�t�@�C���T�C�Y�i�[�p�z��m�ہi���I�j
				//char* size_str = new char[length];
				if (size_str == NULL) {
					ret= ImageSequenceResult::OUT_OF_MEMORY;
					break;
				}
				else do {

					//�ǂݍ��݁i�T�C�Y�j
					if (fread_s(size_str, length, 1, length, fp) < length) {
						ret = ImageSequenceResult::MALFORMED_FILE;
						break;
					}

					for (int i = 0; i < length; i++) {
						long t = static_cast<long>(0x000000ff & size_str[i]);
						t <<= 8 * i;
						size |= t;
					}

					//�f�[�^�{�̂������Ă���̈�m��
					unsigned char* ndata = static_cast<unsigned char*>(malloc(sizeof(unsigned char) * size));
					
					files_[loaded] = ndata;
					if (ndata == NULL) {
						ret = ImageSequenceResult::OUT_OF_MEMORY;
						break;
					}
					else do {

						//�t�@�C����ǂݎ��
						if (fread_s(ndata, sizeof(char) * size, 1, size, fp) < (size_t)size) {
							ret = ImageSequenceResult::MALFORMED_FILE;
							break;
						}

						for (int i = 0; i < size; i++) {
							//�𓀍��
							ndata[i] ^= pass;
						}
						
						try {
							images_[loaded] = CreateGraphFromMem(ndata, (int)size);
						}
						catch (std::exception &ex) {
							ret = ImageSequenceResult::LOAD_ERROR;
							OutputDebugString("[��O]");
							OutputDebugString(ex.what());
							OutputDebugString("\r\n");
							break;
						}
						
					} while (0);
					//free(ndata);


				} while (0);

				if(ProcessMessage() != 0) break;	//�E�B���h�E�̏I��

			}//end of for


			if (ret != ImageSequenceResult::OK) {
				//�G���[�����������ꍇ�̓ǂݍ��񂾉摜�ɑ΂���J������
				for (unsigned int i = 0; i < loaded; i++) DeleteGraph(images_[i]);
			}

			fclose(fp);
			fp = NULL;

		} while (0);


		delete[] size_str;
		size_str = NULL;

	} while (0);


	if (ret == ImageSequenceResult::OK) {
		is_allocated_ = true;
	}

	return ret;
}

jubeat_online::ImageSequenceResult	jubeat_online::ImageSequence::SetSequenceFilename(const char * filename){

	//***********************************************************************
	//�֐���:SetSequenceFilename
	//���@��:�ǂݍ��ރt�@�C������ݒ肵�܂��B
	//		 ���̊֐��Ń��[�h�͔������܂���B
	//�߂�l:ImageSequenceResult�^
	//***********************************************************************

	if (filename_ == NULL) {
		//����̓o�^
		size_t fsize = strlen(filename) + 1;
		filename_ = static_cast<char*>(malloc(sizeof(char) * fsize));
		if (filename_ == NULL) return ImageSequenceResult::OUT_OF_MEMORY;
		strcpy_s(filename_, fsize, filename);
	}
	else {
		free(filename_);
		filename_ = NULL;
		return ImageSequence::SetSequenceFilename(filename);
	}

	return ImageSequenceResult::OK;
}

int									jubeat_online::ImageSequence::WaitLoadComplete(void) {

	//***********************************************************************
	//�֐���:WaitLoadComplete
	//���@��:���[�h���I��莟��A���͌Ă΂Ȃ��Ă͂Ȃ�܂���B
	//		 LoadSequence�ɂ��摜���[�h�̃L���[��������������ǂ����A
	//		 ���[�v�Ōp���I�ɖ₢���킹�Ă��������B�܂��A����Ɠ�����
	//		 ProcessMessage()�����[�v�Ŗ₢���킹�Ă��������B
	//�߂�l:int�^�ŁA0�Ȃ琬���A���̒l�Ȃ�A���̃t���[�������܂��ǂݍ��߂�
	//		 ���Ȃ��Ƃ������ƁA���̒l�Ȃ�ǂݍ��݂Ɏ��s�������������}�C�i�X��
	//		 �\�����Ă��܂��B
	//***********************************************************************

	//LoadSequence���s������
	if (!is_allocated_) return -1;
	
	if (is_loaded_) return failed_num_;

	int as = 0;
	unsigned int loaded_num = 0;
	failed_num_ = 0;
	
	// �ǂݍ��݂��I����Ă�����摜��`�悷��
	for (unsigned int i = 0; i < all_image_frame_; i++) {
		as = CheckHandleASyncLoad(images_[i]);
		if (as == FALSE){
			//�ǂݍ��ݐ�p�̃t�@�C���i�[������
			if (files_[i] != NULL) {
				free(files_[i]);
				files_[i] = NULL;
			}
			loaded_num++;
		}
		else if (as == -1) {
			failed_num_--;
		}
	}

	if (all_image_frame_ > loaded_num) return all_image_frame_ - loaded_num;
	
	//���[�h�����������ꍇ
	if (files_ != NULL) {
		free(files_);
		files_ = NULL;
	}

	is_loaded_ = true;
	return failed_num_;
}

void								jubeat_online::ImageSequence::DeleteSequence(void){

	//***********************************************************************
	//�֐���:DeleteSequence
	//���@��:�V�[�P���X�̍폜�����܂��B�������Ɠ��l�ł��B
	//***********************************************************************

	//�V�[�P���X�̏��������s���܂��B

	if (is_allocated_ == false) return;
	
	if (images_ != NULL) {
		for (unsigned int i = 0; i < all_image_frame_; i++) {
			DeleteGraph(images_[i]);
		}
		free(images_);
		images_ = NULL;
	}
	
	if (files_ != NULL) {
		for (unsigned int i = 0; i < all_image_frame_; i++) {
			if(files_[i] != NULL) free(files_[i]);
		}
		free(files_);
	}

	if (filename_ != NULL) {
		free(filename_);
		filename_ = NULL;
	}

	all_image_frame_ = 0;
	now_frame_ = 0;
	fps_ = 0;

	is_repeat_ = 0;
	in_frame_ = 0;
	out_frame_ = 0;

	x_ = 0;
	y_ = 0;
	started_time_ = 0;
	is_expand = 0;
	exrate = 1.0f;

	is_allocated_ = false;
	failed_num_ = 0;

	is_allocated_ = false;
	is_loaded_ = false;
	
}

jubeat_online::ImageSequenceResult	jubeat_online::ImageSequence::PlaySequence(const int x, const int y, const unsigned int frame){

	//***********************************************************************
	//�֐���:PlaySequence
	//���@��:�V�[�P���X�摜���Đ����܂��B
	//		 LoadSequence�AWaitLoadComplete���Ăяo���A���[�h��������
	//		 �Ă���K�v������܂��B���������s���Ă���摜�������
	//		 �����̃T�C�Y��IMAGE_SEQUENCE_DEFAULT_IMAGE_COLOR�Ɏw�肳�ꂽ
	//		 �F�ԍ��̎l�p�Ŗ��߂��܂��B
	//���@��:[x]�`�ʂ��钆�S��x���W
	//		 [y]�`�ʂ��钆�S��y���W
	//		 [frame]�`����J�n����t���[��
	//�߂�l:int�^�ŁA0�Ȃ琬���A���̒l�Ȃ�A���̃t���[�������܂��ǂݍ��߂�
	//		 ���Ȃ��Ƃ������ƁA���̒l�Ȃ�ǂݍ��݂Ɏ��s�������������}�C�i�X��
	//		 �\�����Ă��܂��B
	//***********************************************************************

	if (is_loaded_ == false) return ImageSequenceResult::LOAD_ERROR;

	x_ = x;
	y_ = y;

	now_frame_ = frame;

	started_time_ = GetNowCount();

	return ImageSequenceResult::OK;
}



/*
void jubeatOnline::c_ImageSequence::RepeatFlag(const bool flag)
{

}*/

//***********************************************
//*		ImageSequence Class Source File			*
//*			Programmed by white					*
//***********************************************

#include "image_sequence.h"

#include <string.h>

#include <iostream>
#include <thread>
#include <exception>


jubeat_online::ImageSequence::ImageSequence() {
	filename_ = NULL;
	images_ = NULL;

	all_image_frame_ = 0;
	now_frame_ = 0;
	fps_ = 0;

	is_repeat_ = 0;
	in_frame_ = 0;
	out_frame_ = 0;

	x_ = 0;
	y_ = 0;
	exrate_ = 1.0f;
	is_play_ = false;

	is_allocated_ = false;
	is_loaded_ = false;

	load_result_ = ImageSequenceResult::LOAD_ERROR;

}

jubeat_online::ImageSequence::~ImageSequence() {
	DeleteSequence();
}


void								jubeat_online::ImageSequence::LoadThread(void) {

	//***********************************************************************
	//�֐���:(private)LoadThread
	//���@��:�ʃX���b�h�ŘA�ԉ摜�̓ǂݍ��݂����܂�
	//�߂�l:void�^�B���ʂ�load_result_�Ɋi�[����܂��B
	//***********************************************************************
	
	std::lock_guard<std::mutex> lock(mtx_);

	//�߂�l�i�[�p
	ImageSequenceResult ret = ImageSequenceResult::OK;

	//�V�[�P���X�摜��ǂݍ���
	//�t�@�C�����J��
	FILE* fp;
	//mtx.lock();
	if (fopen_s(&fp, filename_, "rb") != 0) {
		//�t�@�C���̊J���Ɏ��s
		ret = ImageSequenceResult::LOAD_ERROR;
		return;
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
		images_ = new sf::Texture[all_image_frame_];
		if (images_ == NULL) {
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

				if (size_str == NULL) {
					ret = ImageSequenceResult::OUT_OF_MEMORY;
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
					unsigned char* ndata = new unsigned char[size];

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
							if (!images_[loaded].loadFromMemory(ndata, size)) {
								throw;
							}
						}
						catch (std::exception &ex) {
							std::cout << "[exception]Image cannot load:" << ex.what();
						}

					} while (0);
					delete[] ndata;


				} while (0);


			}//end of for


			if (ret != ImageSequenceResult::OK) {
				//�G���[�����������ꍇ�̓ǂݍ��񂾉摜�ɑ΂���J������
				delete[] images_;
			}

			fclose(fp);
			fp = NULL;

		} while (0);


		delete[] size_str;
		size_str = NULL;

	} while (0);


	if (ret == ImageSequenceResult::OK) {
		is_loaded_ = true;
		out_frame_ = all_image_frame_;
	}

	load_result_ = ret;
	std::cout << "ImageSequence�X���b�h���I�����܂�" << filename_ << "\n";


	//mtx.unlock();
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
	
	std::cout << "ImageSequence�X���b�h���J�n���܂�" << filename << "\n";

	std::thread th(&jubeat_online::ImageSequence::LoadThread, this);
	th.detach();

	is_allocated_ = true;

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
	//���@��:���[�h���I��������ǂ����擾�ł��܂��B
	//�߂�l:int�^�ŁA0�Ȃ琬���A���̒l�Ȃ�A�܂��ǂݍ��߂Ă��Ȃ��Ƃ������ƁA
	//		 ���̒l�Ȃ�ǂݍ��݂Ɏ��s�������������}�C�i�X�ŕ\�����Ă��܂��B
	//***********************************************************************

	//LoadSequence���s������
	if (!is_allocated_) return -1;
	
	if (is_loaded_) {
		if(load_result_ == ImageSequenceResult::OK) return 0;
		else return -1;
	}
	return 1;
}


int jubeat_online::ImageSequence::DrawSequence(const float x, const float y, const float ex, sf::RenderTexture * screen_buffer)
{
	exrate_ = ex;
	return DrawSequence(x,y,screen_buffer);
}

int jubeat_online::ImageSequence::DrawSequence(const float x, const float y, sf::RenderTexture * screen_buffer)
{
	x_ = x;
	y_ = y;
	return DrawSequence(screen_buffer);
}

int jubeat_online::ImageSequence::DrawSequence(sf::RenderTexture* screen_buffer)
{
	//***********************************************************************
	//�֐���:DrawSequence
	//���@��:��ʂɎ��ۂɕ`�ʂ��܂�
	//�߂�l:int�^�B-1�͕`�ʂ����A����ȊO�͕`�ʂ����t���[���ԍ�
	//***********************************************************************
	//���[�h�͊������Ă��邩
	if (is_loaded_ == false) return -1;

	//�Đ�����
	if (is_play_) {
		sf::Time t(started_time_.getElapsedTime());
		now_frame_ = static_cast<int>(t.asSeconds() * fps_);
		if (now_frame_ >= all_image_frame_ || now_frame_ >= out_frame_) {
			if (is_repeat_ == true) {
				//���s�[�g����
				now_frame_ %= (out_frame_ - in_frame_);
			}
			else {
				is_play_ = false;
				now_frame_ = 0;
			}
		}

		sf::Sprite graph(images_[now_frame_]);
		graph.setOrigin(graph.getLocalBounds().width / 2.0f, graph.getLocalBounds().height / 2.0f);
		graph.setPosition(x_, y_);
		graph.setScale(exrate_, exrate_);
		screen_buffer->draw(graph);

		return now_frame_;
	}
	

	return -1;
}

void								jubeat_online::ImageSequence::DeleteSequence(void){

	//***********************************************************************
	//�֐���:DeleteSequence
	//���@��:�V�[�P���X�̍폜�����܂��B�������Ɠ��l�ł��B
	//***********************************************************************

	//�V�[�P���X�̏��������s���܂��B

	if (is_allocated_ == false) return;
	
	if (images_ != NULL) {
		delete[] images_;
		images_ = NULL;
	}
	

	if (filename_ != NULL) {
		delete[] filename_;
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
	exrate_ = 1.0f;

	is_allocated_ = false;

	is_allocated_ = false;
	is_loaded_ = false;
	
}

void jubeat_online::ImageSequence::set_x(const int x)
{
	x_ = x;
}

int jubeat_online::ImageSequence::x(void) const
{
	return x_;
}

void jubeat_online::ImageSequence::set_y(const int y)
{
	y_ = y;
}

int jubeat_online::ImageSequence::y(void) const
{
	return y_;
}

void jubeat_online::ImageSequence::set_in_frame(const unsigned int frame)
{
	in_frame_ = frame;
}

unsigned int jubeat_online::ImageSequence::in_frame(void) const
{
	return in_frame_;
}

void jubeat_online::ImageSequence::set_out_frame(const unsigned int frame)
{
	out_frame_ = frame;
}

unsigned int jubeat_online::ImageSequence::out_frame(void) const
{
	return out_frame_;
}

void jubeat_online::ImageSequence::InitInPoint(void)
{
	set_in_frame(0);
}

void jubeat_online::ImageSequence::InitOutPoint(void)
{
	set_out_frame(all_image_frame_);
}

void jubeat_online::ImageSequence::LoadDivThread(const int x_div, const int y_div, const int width, const int height)
{
	std::lock_guard<std::mutex> lock(mtx_);

	load_result_ = ImageSequenceResult::OK;

	sf::Image gr;
	if (gr.loadFromFile(filename_) == false) {
		std::cout << "[���s]" << filename_ << "��������܂���ł���\n";
		load_result_ = ImageSequenceResult::LOAD_ERROR;
		return;
	}

	images_ = new sf::Texture[all_image_frame_];
	if (images_ == NULL) {
		load_result_ = ImageSequenceResult::OUT_OF_MEMORY;
		return;
	}

	for (int x = 0; x < x_div; x++) {
		for (int y = 0; y < y_div; y++) {
			images_[x + y * x_div].loadFromImage(gr, sf::IntRect(x * width, y * height, width, height));
		}
	}

	is_loaded_ = true;
	std::cout << "ImageSequence DividedImage�̃X���b�h���I�����܂���\n";
}

jubeat_online::ImageSequenceResult jubeat_online::ImageSequence::LoadDivGraph(const int all_framecount, const int x_div, const int y_div, const int width, const int height, const char * filename)
{

	ImageSequenceResult ret = ImageSequenceResult::OK;

	//�V�[�P���X�摜�t�@�C���̃t�@�C�����̏���
	if (filename != NULL) {
		ImageSequenceResult fret = SetSequenceFilename(filename);
		if (fret != ImageSequenceResult::OK) return fret;
	}

	all_image_frame_ = all_framecount;
	

	std::cout << "ImageSequence DividedImage�̃X���b�h���J�n���܂���\n";
	std::thread th(&jubeat_online::ImageSequence::LoadDivThread, this,x_div,y_div,width,height);
	th.detach();


	return ret;
}

jubeat_online::ImageSequenceResult	jubeat_online::ImageSequence::PlaySequence(const unsigned int frame){

	//***********************************************************************
	//�֐���:PlaySequence
	//���@��:�V�[�P���X�摜���Đ����܂��B
	//		 LoadSequence�AWaitLoadComplete���Ăяo���A���[�h��������
	//		 �Ă���K�v������܂��B���������s���Ă���摜�������
	//		 �����̃T�C�Y��IMAGE_SEQUENCE_DEFAULT_IMAGE_COLOR�Ɏw�肳�ꂽ
	//		 �F�ԍ��̎l�p�Ŗ��߂��܂��B
	//���@��:[frame]�`����J�n����t���[��
	//�߂�l:int�^�ŁA0�Ȃ琬���A���̒l�Ȃ�A���̃t���[�������܂��ǂݍ��߂�
	//		 ���Ȃ��Ƃ������ƁA���̒l�Ȃ�ǂݍ��݂Ɏ��s�������������}�C�i�X��
	//		 �\�����Ă��܂��B
	//***********************************************************************

	if (is_loaded_ == false) return ImageSequenceResult::LOAD_ERROR;


	now_frame_ = frame;

	is_play_ = true;
	started_frame_ = frame;
	started_time_.restart();

	
	return ImageSequenceResult::OK;
}

void jubeat_online::ImageSequence::set_is_repeat(const bool flag)
{
	is_repeat_ = flag;
}

bool jubeat_online::ImageSequence::is_repeat(void) const
{
	return is_repeat_;
}
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



jubeat_online::ImageSequenceResult jubeat_online::ImageSequence::LoadSequence(const char * filename){

	//***********************************************************************
	//関数名:LoadSeqeunce
	//説　明:独自形式の連続版画像の読み込み命令を発生させます
	//		 この関数が呼ばれるまでに、DxLib::SetUseASyncLoadFlag(TRUE)が
	//		 呼ばれている必要があります。呼ばない場合はこの関数内で
	//		 完全にロードが完了します。
	//戻り値:ImageSequenceResult型
	//***********************************************************************

	ImageSequenceResult ret = ImageSequenceResult::OK;

	//シーケンス画像ファイルのファイル名の処理
	if (filename != NULL) {
		ImageSequenceResult fret = SetSequenceFilename(filename);
		if(fret != ImageSequenceResult::OK) return fret;
	}

	else if (filename_ == NULL) return ImageSequenceResult::NULL_FILEPATH;		//ファイル名を指定しろ
	
	//TO DOこのSetSequenceを追加すること
	//temporary
	
	if (is_allocated_) DeleteSequence();
	

	//シーケンス画像を読み込む
	//ファイルを開封
	FILE* fp;
	if (fopen_s(&fp, filename_, "rb") != 0) {
		//ファイルの開封に失敗
		return ImageSequenceResult::LOAD_ERROR;
	}
	
	unsigned char type = 0x00, pass = 0x00, length = 0x00, fps = 0x00;

	do {

		//まず識別子の取得
		if (fread_s(&type, sizeof(char), 1, 1, fp) < 1) { ret = ImageSequenceResult::MALFORMED_FILE; break; }

		//簡易パスワード
		if (fread_s(&pass, sizeof(char), 1, 1, fp) < 1) { ret = ret = ImageSequenceResult::MALFORMED_FILE; break; }

		//longバイト長さ
		if (fread_s(&length, sizeof(char), 1, 1, fp) < 1) { ret = ret = ImageSequenceResult::MALFORMED_FILE; break; }

		//フレームレート
		if (fread_s(&fps, sizeof(char), 1, 1, fp) < 1) { ret = ret = ImageSequenceResult::MALFORMED_FILE; break; }
		fps_ = static_cast<unsigned int>(fps);

		//総フレーム数
		char tm[2];
		all_image_frame_ = 0;
		if (fread_s(tm, sizeof(char) * 2, 1, 2, fp) < 2) { ret = ret = ImageSequenceResult::MALFORMED_FILE; break; }
		for (int i = 0; i < 2; i++) {
			unsigned int t = static_cast<unsigned int>(0x000000ff & tm[i]);
			t <<= 8 * i;
			all_image_frame_ |= t;
		}
		
		unsigned char* size_str = new unsigned char[length];

		//画像フレーム格納領域
		images_ = static_cast<int*>(malloc(sizeof(int) * all_image_frame_));
		files_ = static_cast<unsigned char**>(malloc(sizeof(unsigned char*) * all_image_frame_));
		if (images_ == NULL || files_ == NULL){
			if (images_ != NULL) free(images_);
			if (files_ != NULL) free(files_);
			ret = ImageSequenceResult::OUT_OF_MEMORY;
			break;
		}
		else do {

			unsigned int loaded = 0;	//読み込みが完了したフレーム数
			for (loaded = 0; loaded < all_image_frame_ && ret == ImageSequenceResult::OK; loaded++) {


				//ループに入った後失敗した場合、画像のメモリ解放もしなければならない
				//読み込まれた画像はloaded枚となる

				//ファイル長を取得
				long size = 0;

				//ファイルサイズ格納用配列確保（動的）
				//char* size_str = new char[length];
				if (size_str == NULL) {
					ret= ImageSequenceResult::OUT_OF_MEMORY;
					break;
				}
				else do {

					//読み込み（サイズ）
					if (fread_s(size_str, length, 1, length, fp) < length) {
						ret = ImageSequenceResult::MALFORMED_FILE;
						break;
					}

					for (int i = 0; i < length; i++) {
						long t = static_cast<long>(0x000000ff & size_str[i]);
						t <<= 8 * i;
						size |= t;
					}

					//データ本体を持ってくる領域確保
					unsigned char* ndata = static_cast<unsigned char*>(malloc(sizeof(unsigned char) * size));
					
					files_[loaded] = ndata;
					if (ndata == NULL) {
						ret = ImageSequenceResult::OUT_OF_MEMORY;
						break;
					}
					else do {

						//ファイルを読み取る
						if (fread_s(ndata, sizeof(char) * size, 1, size, fp) < (size_t)size) {
							ret = ImageSequenceResult::MALFORMED_FILE;
							break;
						}

						for (int i = 0; i < size; i++) {
							//解凍作業
							ndata[i] ^= pass;
						}
						
						try {
							images_[loaded] = CreateGraphFromMem(ndata, (int)size);
						}
						catch (std::exception &ex) {
							ret = ImageSequenceResult::LOAD_ERROR;
							OutputDebugString("[例外]");
							OutputDebugString(ex.what());
							OutputDebugString("\r\n");
							break;
						}
						
					} while (0);
					//free(ndata);


				} while (0);

				if(ProcessMessage() != 0) break;	//ウィンドウの終了

			}//end of for


			if (ret != ImageSequenceResult::OK) {
				//エラーが発生した場合の読み込んだ画像に対する開放処理
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

jubeat_online::ImageSequenceResult jubeat_online::ImageSequence::SetSequenceFilename(const char * filename){
	if (filename_ == NULL) {
		//初回の登録
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

int jubeat_online::ImageSequence::WaitLoadComplete(void) {

	//LoadSequence実行したか
	if (!is_allocated_) return -1;
	
	if (is_loaded_) return failed_num_;

	int as = 0;
	unsigned int loaded_num = 0;
	failed_num_ = 0;
	
	// 読み込みが終わっていたら画像を描画する
	for (unsigned int i = 0; i < all_image_frame_; i++) {
		as = CheckHandleASyncLoad(images_[i]);
		if (as == FALSE){
			//読み込み専用のファイル格納メモリ
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
	
	//ロードが完了した場合
	if (files_ != NULL) {
		free(files_);
		files_ = NULL;
	}

	is_loaded_ = true;
	return failed_num_;
}

jubeat_online::ImageSequenceResult jubeat_online::ImageSequence::DeleteSequence(void){
	
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

	is_allocated_ = false;
	is_loaded_ = false;


	return ImageSequenceResult::OK;
}

/*
void jubeatOnline::c_ImageSequence::RepeatFlag(const bool flag)
{

}*/

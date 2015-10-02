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


	unsigned int loaded = 0;	//読み込みが完了したフレーム数
	for (loaded = 0; loaded < all_image_frame_; loaded++) {


		//ループに入った後失敗した場合、画像のメモリ解放もしなければならない
		//読み込まれた画像はloaded枚となる

		//ファイル長を取得
		long size = 0;

		//ファイルサイズ格納用配列確保（動的）
		//char* size_str = new char[length];
		if (size_str == NULL) {
			load_result_ = -4;
			break;
		}
		else do {

			//読み込み（サイズ）
			if (fread_s(size_str, length, 1, length, fp) < length) {
				load_result_ = -2;
				break;
			}

			for (int i = 0; i < length; i++) {
				long t = (0x000000ff & size_str[i]);
				t <<= 8 * i;
				size |= t;
			}

			//データ本体を持ってくる領域確保
			unsigned char* ndata = (unsigned char*)malloc(sizeof(unsigned char) * size);
			files_[loaded] = ndata;
			if (ndata == NULL) {
				load_result_ = -4;
				break;
			}
			else do {

				//ファイルを読み取る
				if (fread_s(ndata, sizeof(char) * size, 1, size, fp) < (size_t)size) {
					load_result_ = -2;
					break;
				}

				for (int i = 0; i < size; i++) {
					//解凍作業
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
		//エラーが発生した場合の読み込んだ画像に対する開放処理
		for (unsigned int i = 0; i < loaded; i++) DeleteGraph(images_[i]);

	}

	fclose(fp);


	is_loaded_ = true;


}


int jubeat_online::ImageSequence::LoadSequence(jubeat_online::ImageSequence* me, const char * filename){


	//
	//シーケンス画像ファイルのファイル名の処理
	//if (filename != NULL) SetSequenceFilename(filename);
	//else if (filename_ == NULL) return -3;		//ファイル名を指定しろ
	
	//TO DOこのSetSequenceを追加すること
	//temporary
	is_loaded_ = false;
	size_t leng = strlen(filename);
	//if(filename_ == NULL)
	filename_ = new char[leng + 1];
	if (filename_ == NULL) return -4;

	strcpy_s(filename_,leng + 1, filename);

	//シーケンス画像を読み込む

	int ret = 0;
	//ファイルを開封
	FILE* fp;
	if (fopen_s(&fp, filename_, "rb") != 0) {
		//ファイルの開封に失敗
		ret = -1;
	}



	unsigned char type = 0x00, pass = 0x00, length = 0x00, fps = 0x00;

	do {

		//まず識別子の取得
		if (fread_s(&type, sizeof(char), 1, 1, fp) < 1) { ret = -2; break; }	//ファイルが少なすぎる

																				//簡易パスワード
		if (fread_s(&pass, sizeof(char), 1, 1, fp) < 1) { ret = -2; break; }	//ファイルが少なすぎる

																				//longバイト長さ
		if (fread_s(&length, sizeof(char), 1, 1, fp) < 1) { ret = -2; break; }	//ファイルが少なすぎる

																				//フレームレート
		if (fread_s(&fps, sizeof(char), 1, 1, fp) < 1) { ret = -2; break; }	//ファイルが少なすぎる
		fps_ = static_cast<unsigned int>(fps);

		//総フレーム数
		char tm[2];
		all_image_frame_ = 0;
		if (fread_s(tm, sizeof(char) * 2, 1, 2, fp) < 2) { ret = -2; break; }	//ファイルが少なすぎる
		for (int i = 0; i < 2; i++) {
			int t = (0x000000ff & tm[i]);
			t <<= 8 * i;
			all_image_frame_ |= t;
		}
		size_str = new unsigned char[length];

		//画像フレーム格納領域
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
		//すべての読み込みが終わるまで待つ
	}

	return ret;	//成功

}

int jubeat_online::ImageSequence::WaitLoadComplete(void) {
	if (is_loaded_ == false) return all_image_frame_;

	int as = 0;
		loaded_num_ = 0;
		success_num_ = 0;
		// 読み込みが終わっていたら画像を描画する
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
	//ロードが完了
	return (success_num_ - loaded_num_);
}

/*
void jubeatOnline::c_ImageSequence::RepeatFlag(const bool flag)
{

}*/

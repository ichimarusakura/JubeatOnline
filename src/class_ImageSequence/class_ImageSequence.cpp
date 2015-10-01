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

	//シーケンス画像ファイルのファイル名の処理
	//if (filename != NULL) SetSequenceFilename(filename);
	//else if (filename_ == NULL) return -3;		//ファイル名を指定しろ
	
	//シーケンス画像を読み込む

	//ファイルを開封
	FILE* fp;
	if (fopen_s(&fp, filename, "rb") != 0) {
		//ファイルの開封に失敗
		return -1;
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
		if (fread_s(tm, sizeof(char)*2, 1, 2, fp) < 2) { ret = -2; break; }	//ファイルが少なすぎる
		for (int i = 0; i < 2; i++) {
			int t = (0x000000ff & tm[i]);
			t <<= 8 * i;
			all_image_frame_ |= t;
		}

		//画像フレーム格納領域
		images_ = new int[all_image_frame_];
		if (images_ == NULL) {
			ret = -4;
			break;
		}else do {

			unsigned int loaded = 0;	//読み込みが完了したフレーム数
			for (loaded = 0; loaded < all_image_frame_; loaded++) {

				//ループに入った後失敗した場合、画像のメモリ解放もしなければならない
				//読み込まれた画像はloaded枚となる
				
				//ファイル長を取得
				long size = 0;

				//ファイルサイズ格納用配列確保（動的）
				char* size_str = new char[length];
				if (size_str == NULL) {
					ret = -4;
					break;
				}else do {

					//読み込み（サイズ）
					if (fread_s(size_str, length, 1, length, fp) < length) {
						ret = -2;
						break;
					}

					for (int i = 0; i < length; i++) {
						long t = (0x000000ff & size_str[i]);
						t <<= 8 * i;
						size |= t;
					}

					//データ本体を持ってくる領域確保
					unsigned char* data = new unsigned char[size];
					if (data == NULL) {
						ret = -4;
						break;
					}
					else do {

						//ファイルを読み取る
						if (fread_s(data, size, 1, size, fp) < size) {
							ret = -2;
							break;
						}

						for (int i = 0; i < size; i++) {
							//解凍作業
							data[i] ^= pass;
						}

						if ((images_[loaded] = CreateGraphFromMem(data, size)) == -1) {
							//画像が読み込めなかった場合
							ret = -1;
							break;
						}
					} while (0);

					delete [] data;

				} while (0);

				delete [] size_str;

			}//end of for
			if (ret != 0) {
				//エラーが発生した場合の読み込んだ画像に対する開放処理
				for (unsigned int i = 0; i < loaded; i++) DeleteGraph(images_[i]);

			}

			if (loaded != all_image_frame_) return -2;	//整合性なし


		} while (0);

		if (ret != 0) {
			//エラーが発生した場合だけ、image_を開放する
			delete[] images_;
		}

	} while (0);


	if(ret == 0) is_loaded_ = true;

	return ret;	//成功
}
/*
void jubeatOnline::c_ImageSequence::RepeatFlag(const bool flag)
{

}*/

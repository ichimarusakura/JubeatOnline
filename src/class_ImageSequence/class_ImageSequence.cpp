//***********************************************
//*		ImageSequence Class Source File			*
//*			Programmed by white					*
//***********************************************

#include "ImageSequenceClass.h"

#include <stdio.h>
#include <stdlib.h>


int jubeatOnline::c_ImageSequence::LoadSequence(const char * filename){

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

	char tmp[4096];
	unsigned char type = 0x00, pass = 0x00, length = 0x00, fps = 0x00;
	

	//まず識別子の取得
	if ((type = fgetc(fp)) == EOF) return -2;	//ファイルが少なすぎる

	//簡易パスワード
	if ((pass = fgetc(fp)) == EOF) return -2;	//ファイルが少なすぎる

	//longバイト長さ
	if ((length = fgetc(fp)) == EOF) return -2;	//ファイルが少なすぎる

	//フレームレート
	if ((fps = fgetc(fp)) == EOF) return -2;	//ファイルが少なすぎる

	for (all_image_frame_ = 0; feof(fp) == 0; all_image_frame_++) {
		//ファイル長を取得
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

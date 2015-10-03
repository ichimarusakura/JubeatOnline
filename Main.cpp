#include <DxLib.h>

#include <math.h>

#include <process.h>

#include <iostream>
#include <exception>

//temporary
#include "src/class_image_sequence/image_sequence.h"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	DxLib::SetWindowText("jubeat ONLINE");
	DxLib::ChangeWindowMode(true);
	DxLib::SetGraphMode(1080, 1920, 32);
	DxLib::SetMultiThreadFlag(TRUE);
	DxLib::WaitVSync(TRUE);
	DxLib::SetWindowPosition(1910, -640);
	// 描画先を裏画面にする
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	
	
	if (DxLib::DxLib_Init() == -1) return -1;

	jubeat_online::ImageSequence n;
	n.SetSequenceFilename("media\\clear.isf");

	DxLib::SetUseASyncLoadFlag(TRUE);

	n.LoadSequence();

	DxLib::SetUseASyncLoadFlag(FALSE);


	// メインループ(何かキーが押されたらループを抜ける)
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_RETURN) == 0)
	{
		// 画面のクリア
		ClearDrawScreen();



		n.WaitLoadComplete();



		// 非同期読み込みの数を描画
		DrawFormatString(0, 0, GetColor(255, 255, 255), "非同期読み込みの数 %d", GetASyncLoadNum());

		// 裏画面の内容を表画面に反映
		ScreenFlip();
	}

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面のクリア
		ClearDrawScreen();

		n.DeleteSequence();




		// 非同期読み込みの数を描画
		DrawString(100, 100, "Delete", 65535);

		// 裏画面の内容を表画面に反映
		ScreenFlip();
	}

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}

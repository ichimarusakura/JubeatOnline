#include <DxLib.h>

#include <math.h>

#include <process.h>

#include <iostream>
#include <exception>

//temporary
#include "src/class_image_sequence/image_sequence.h"


int load = 0;
void Draw() {
	int ddd = 0;
	OutputDebugString("実行できました\n");
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		if ((ddd % 60) == 0) OutputDebugString("ループしてます\n");
		ClsDrawScreen();
		DrawGraph(0,0,load,false);
		//DxLib::DrawBox(0, 0, 5, 30, GetColor(255, 255, 255), true);

		/*		clsDx();
		//printfDx("\n\n\n%d/%d", n.success_num_, n.loaded_num_);
		DxLib::DrawBox(0, 0, 30, 5, GetColor(255, 255, 255), true);
		DxLib::DrawBox(0, 0, 5, 30, GetColor(255, 255, 255), true);

		DxLib::DrawBox(1080, 0, 1050, 5, GetColor(255, 255, 255), true);
		DxLib::DrawBox(1080, 0, 1075, 30, GetColor(255, 255, 255), true);

		DxLib::DrawBox(0, 1920, 30, 1915, GetColor(255, 255, 255), true);
		DxLib::DrawBox(0, 1920, 5, 1890, GetColor(255, 255, 255), true);

		DxLib::DrawBox(1080, 1920, 1050, 1915, GetColor(255, 255, 255), true);
		DxLib::DrawBox(1080, 1920, 1075, 1890, GetColor(255, 255, 255), true);

		DrawCircle((int)(sin(GetNowCount() / 2000.0f) * 200 + 500), (int)(cos(GetNowCount() / 500.0f) * 300 + 500), (int)(sin(GetNowCount() / 1000.0f) * 200 + 220), GetColor(255, 255, 0), true);
		*/
		ScreenCopy();
		//Sleep(16);
		ddd++;

	}

}


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

	//int GrHandle[200];
	//int i;

	/*
	// 非同期読み込み設定に変更

	// Test1.bmp を 20回読み込む
	for (i = 0; i < 200; i++)
	{
		GrHandle[i] = LoadGraph("media\\image.png");
	}
	*/
	// 同期読み込み設定に変更
	DxLib::SetUseASyncLoadFlag(TRUE);


	DxLib::SetUseASyncLoadFlag(FALSE);


	// メインループ(何かキーが押されたらループを抜ける)
	while (ProcessMessage() == 0 && CheckHitKeyAll() == 0)
	{
		// 画面のクリア
		ClearDrawScreen();

		// 読み込みが終わっていたら画像を描画する
		/*for (i = 0; i < 200; i++)
		{
			if (CheckHandleASyncLoad(GrHandle[i]) == FALSE)
			{
				DrawGraph(i * 32, 0, GrHandle[i], TRUE);
			}
		}*/


		// 非同期読み込みの数を描画
		DrawFormatString(0, 0, GetColor(255, 255, 255), "非同期読み込みの数 %d", GetASyncLoadNum());

		// 裏画面の内容を表画面に反映
		ScreenFlip();
	}

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}

/*

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DxLib::SetWindowText("jubeat ONLINE");
	DxLib::ChangeWindowMode(true);
	DxLib::SetGraphMode(1080, 1920, 32);
	DxLib::SetMultiThreadFlag(TRUE);
	DxLib::WaitVSync(TRUE);
	DxLib::SetWindowPosition(1910, -640);
	if (DxLib::DxLib_Init() == -1)
	{
		return -1;
	}

	DxLib::SetDrawScreen(DX_SCREEN_BACK);


	load = LoadGraph("media\\image.png");



	jubeat_online::ImageSequence n;


	int res = n.LoadSequence(&n, "media\\button.isf");



	int ddd = 0;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		ClsDrawScreen();
		DrawGraph(0, (int)(sin(ddd / 100.0f)*100.0f + 100), load, false);

		clsDx();
		DxLib::DrawBox(0, 0, 30, 5, GetColor(255, 255, 255), true);
		DxLib::DrawBox(0, 0, 5, 30, GetColor(255, 255, 255), true);

		DxLib::DrawBox(1080, 0, 1050, 5, GetColor(255, 255, 255), true);
		DxLib::DrawBox(1080, 0, 1075, 30, GetColor(255, 255, 255), true);

		DxLib::DrawBox(0, 1920, 30, 1915, GetColor(255, 255, 255), true);
		DxLib::DrawBox(0, 1920, 5, 1890, GetColor(255, 255, 255), true);

		DxLib::DrawBox(1080, 1920, 1050, 1915, GetColor(255, 255, 255), true);
		DxLib::DrawBox(1080, 1920, 1075, 1890, GetColor(255, 255, 255), true);

		DrawCircle((int)(sin(GetNowCount() / 2000.0f) * 200 + 500), (int)(cos(GetNowCount() / 500.0f) * 300 + 500), (int)(sin(GetNowCount() / 1000.0f) * 200 + 220), GetColor(255, 255, 0), true);
		
		printfDx("\n\n%d",n.WaitLoadComplete());
		
		ScreenFlip();
		ddd++;

	}
	

	DxLib::DxLib_End();

	return 0;
}*/
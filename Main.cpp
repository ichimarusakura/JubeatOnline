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
//	SetChangeScreenModeGraphicsSystemResetFlag(
	DxLib::SetWindowPosition(1910, -640);
	if (DxLib::DxLib_Init() == -1)
	{
		return -1;
	}

	DxLib::SetDrawScreen(DX_SCREEN_BACK);

	//int t = 

	int t = 0;

	/*try {
		std::thread t1(Draw);
		t1.detach();
	}
	catch (std::exception &ex) {
		std::cerr << ex.what() << std::endl;
	}
	*/
	load = LoadGraph("media\\image.png");
	//DWORD a;
	//HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Draw, NULL, 0, &a);



	jubeat_online::ImageSequence n;
	//int mmm = GetNowCount();
	//t = GetNowCount();
	int res = n.LoadSequence(&n, "media\\clear.isf");
	//t = GetNowCount() - t;
	/*
	Sleep(10);
	int aaa[100];
	for (int i = 0; i < 100; i++) {
		aaa[i] = LoadGraph("media\\image.png");
		OutputDebugString("オラ");
		Sleep(10);
	}*/

	//HANDLE hHandle = (HANDLE)_beginthread(n.DoThread, 0, &n);


	//別スレッドで実行
	//HANDLE hHandle = (HANDLE)_beginthread(n.DoThread, 0, &n);
	//CloseHandle(hThread);
	//WaitForMultipleObjects(1, &hThread, TRUE, INFINITE);
	//int r = LoadData();
	//WaitForSingleObject(hHandle, INFINITE);
	int ddd = 0;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		ClsDrawScreen();
		DrawGraph(0, (int)(sin(ddd / 100.0f)*100.0f + 100), load, false);
		//DxLib::DrawBox(0, 0, 5, 30, GetColor(255, 255, 255), true);

				clsDx();
		printfDx("\n\n\n%d/%d", n.success_num_, n.loaded_num_);
		DxLib::DrawBox(0, 0, 30, 5, GetColor(255, 255, 255), true);
		DxLib::DrawBox(0, 0, 5, 30, GetColor(255, 255, 255), true);

		DxLib::DrawBox(1080, 0, 1050, 5, GetColor(255, 255, 255), true);
		DxLib::DrawBox(1080, 0, 1075, 30, GetColor(255, 255, 255), true);

		DxLib::DrawBox(0, 1920, 30, 1915, GetColor(255, 255, 255), true);
		DxLib::DrawBox(0, 1920, 5, 1890, GetColor(255, 255, 255), true);

		DxLib::DrawBox(1080, 1920, 1050, 1915, GetColor(255, 255, 255), true);
		DxLib::DrawBox(1080, 1920, 1075, 1890, GetColor(255, 255, 255), true);

		DrawCircle((int)(sin(GetNowCount() / 2000.0f) * 200 + 500), (int)(cos(GetNowCount() / 500.0f) * 300 + 500), (int)(sin(GetNowCount() / 1000.0f) * 200 + 220), GetColor(255, 255, 0), true);
		
		ScreenFlip();
		//Sleep(16);
		ddd++;

	}
	

	DxLib::DxLib_End();

	return 0;
}
#include <DxLib.h>

//temporary
#include "src/class_image_sequence/image_sequence.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DxLib::SetWindowText("jubeat ONLINE");
	DxLib::ChangeWindowMode(true);
	DxLib::SetGraphMode(1080, 1920, 32);
	DxLib::WaitVSync(FALSE);
	DxLib::SetWindowPosition(1910, -640);
	if (DxLib::DxLib_Init() == -1)
	{
		return -1;
	}

	DxLib::SetDrawScreen(DX_SCREEN_BACK);


	jubeat_online::ImageSequence n;
	int res = n.LoadSequence("media\\clear.isf");


	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		ClsDrawScreen();
		clsDx();
		printfDx("%d\n", res);

		DxLib::DrawBox(0, 0, 30, 5, GetColor(255, 255, 255), true);
		DxLib::DrawBox(0, 0, 5, 30, GetColor(255, 255, 255), true);

		DxLib::DrawBox(1080, 0, 1050, 5, GetColor(255, 255, 255), true);
		DxLib::DrawBox(1080, 0, 1075, 30, GetColor(255, 255, 255), true);

		DxLib::DrawBox(0, 1920, 30, 1915, GetColor(255, 255, 255), true);
		DxLib::DrawBox(0, 1920, 5, 1890, GetColor(255, 255, 255), true);

		DxLib::DrawBox(1080, 1920, 1050, 1915, GetColor(255, 255, 255), true);
		DxLib::DrawBox(1080, 1920, 1075, 1890, GetColor(255, 255, 255), true);



		ScreenFlip();

	}

	DxLib::DxLib_End();

	return 0;
}
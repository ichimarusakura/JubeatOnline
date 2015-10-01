#include <DxLib.h>

//temporary
#include "src/class_ImageSequence/ImageSequence.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	DxLib::ChangeWindowMode(true);

	if (DxLib::DxLib_Init() == -1)
	{
		return -1;
	}

	DxLib::SetDrawScreen(DX_SCREEN_BACK);

	DxLib::DrawPixel(320, 240, GetColor(255, 255, 255));

	jubeatOnline::ImageSequence n;
	printfDx("%d",n.LoadSequence("media/clear.isf"));
	ScreenFlip();



	DxLib::WaitKey();
	DxLib::DxLib_End();
	return 0;
}
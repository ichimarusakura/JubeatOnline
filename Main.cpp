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
	// �`���𗠉�ʂɂ���
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	
	
	if (DxLib::DxLib_Init() == -1) return -1;

	jubeat_online::ImageSequence n;
	n.SetSequenceFilename("media\\clear.isf");

	DxLib::SetUseASyncLoadFlag(TRUE);

	n.LoadSequence();

	DxLib::SetUseASyncLoadFlag(FALSE);


	// ���C�����[�v(�����L�[�������ꂽ�烋�[�v�𔲂���)
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_RETURN) == 0)
	{
		// ��ʂ̃N���A
		ClearDrawScreen();



		n.WaitLoadComplete();



		// �񓯊��ǂݍ��݂̐���`��
		DrawFormatString(0, 0, GetColor(255, 255, 255), "�񓯊��ǂݍ��݂̐� %d", GetASyncLoadNum());

		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();
	}

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ��ʂ̃N���A
		ClearDrawScreen();

		n.DeleteSequence();




		// �񓯊��ǂݍ��݂̐���`��
		DrawString(100, 100, "Delete", 65535);

		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();
	}

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}

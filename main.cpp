#include "DxLib.h"
#include"Player.h"
#include"Finish.h"
#include<stdio.h>

// �E�B���h�E�̃^�C�g���ɕ\�����镶����
const char TITLE[] = "LE2C_06_�G���P���V��: �^�C�g��";

// �E�B���h�E����
const int WIN_WIDTH = 600;

// �E�B���h�E�c��
const int WIN_HEIGHT = 400;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(TITLE);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���


	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	Player* player = new Player;
	Finish* finish = new Finish();
	int	score1;
	int	score2;
	int	score3;
	int	score4;
	int	score5;

	// �ŐV�̃L�[�{�[�h���p
	char keys[256] = { 0 };

	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	char oldkeys[256] = { 0 };

	// �Q�[�����[�v
	while (true) {
		// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		for (int i = 0; i < 256; i++)
		{
			oldkeys[i] = keys[i];
		}
		// �ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		// ��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		// �X�V����
		player->Move(keys);
		finish->Move(keys);

		FILE* fp;

		char fname[] = "Ranking.csv";

		errno_t err;

		if (finish->Finishflag == 1)
		{
			err = fopen_s(&fp, fname, "w");

			if (fp == nullptr)
			{
				printf("err");
			}
			else {
				if (player->KeyPush > score1)
				{
					score5 = score4;
					score4 = score3;
					score3 = score2;
					score2 = score1;
					score1 = player->KeyPush;
				}
				else if (player->KeyPush >= score2 && player->KeyPush < score1)
				{
					score5 = score4;
					score4 = score3;
					score3 = score2;
					score2 = player->KeyPush;
				}
				else if (player->KeyPush >= score3 && player->KeyPush < score2)
				{
					score5 = score4;
					score4 = score3;
					score3 = player->KeyPush;
				}
				else if (player->KeyPush >= score4 && player->KeyPush < score3)
				{
					score5 = score4;
					score4 = player->KeyPush;
				}
				else if (player->KeyPush >= score5 && player->KeyPush < score4)
				{
					score5 = player->KeyPush;
				}
				fprintf(fp, "%d,%d,%d,%d,%d", &score1, &score2, &score3, &score4, &score5);
				fclose(fp);

				player->KeyPush = 0;
			}
		}

		err = fopen_s(&fp, fname, "r");

		if (fp == nullptr)
		{
			printf("err");
		}

		else {
			fscanf_s(fp, "%d,%d,%d,%d,%d", &score1, &score2, &score3, &score4, &score5);
			fclose(fp);
		}

		// �`�揈��
		DrawFormatString(250, 0, GetColor(255, 255, 255), "push=%d", score1);
		DrawFormatString(250, 50, GetColor(255, 255, 255), "push=%d", score2);
		DrawFormatString(250, 100, GetColor(255, 255, 255), "push=%d", score3);
		DrawFormatString(250, 150, GetColor(255, 255, 255), "push=%d", score4);
		DrawFormatString(250, 200, GetColor(255, 255, 255), "push=%d", score5);
		DrawFormatString(250, 250, GetColor(255, 255, 255), "push=%d", player->KeyPush);

		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1) {
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}

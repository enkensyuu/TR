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

void show(int x[], int num2)
{
	for (int i = 0; i < num2; i++)
	{
		printf("%d", x[i]);
	}
}

void insert(int y[], int num3)
{
	int j, tmp;
	for (int i = 0; i < num3; i++)
	{
		j = i;
		tmp = y[j];
		while (j > 0 && y[j - 1] > tmp)
		{
			y[j] = y[j - 1];
			j--;
		}
		y[j] = tmp;
	}
}

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
	int	score[5];

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
		const int num = 5;
		int score[num];
		int i;

		FILE* fp;
		
		char fname[] = "Ranking.csv";
		
		errno_t err;

		err = fopen_s(&fp, fname, "r");
		if (fp == nullptr)
		{
			printf("err");
		}
		
		else {
			fscanf_s(fp, "%d,%d,%d,%d,%d", &score[0], &score[1], &score[2], &score[3], &score[4]);
			fclose(fp);
		}

		if (finish->Finishflag == 1)
		{
			show(score, num);
			insert(score, num);

			if (score[0] <= player->KeyPush)
			{
				score[0] = player->KeyPush;
			}

			err = fopen_s(&fp, fname, "w");

			if (fp == nullptr)
			{
				printf("err");
			}
			else {
				fprintf(fp, "%d,%d.%d,%d,%d", score[0], score[1], score[2], score[3], score[4]);
				fclose(fp);
			
			}

			player->KeyPush = 0;
		}

		// �`�揈��
		DrawFormatString(250, 50, GetColor(255, 255, 255), "score1=%d", score[4]);
		DrawFormatString(250, 100, GetColor(255, 255, 255), "score2=%d", score[3]);
		DrawFormatString(250, 150, GetColor(255, 255, 255), "score3=%d", score[2]);
		DrawFormatString(250, 200, GetColor(255, 255, 255), "score4=%d", score[1]);
		DrawFormatString(250, 250, GetColor(255, 255, 255), "score5=%d", score[0]);
		DrawFormatString(250, 300, GetColor(255, 255, 255), "push=%d", player->KeyPush);

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

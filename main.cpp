#include "DxLib.h"
#include"Player.h"
#include"Finish.h"
#include<stdio.h>

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "LE2C_06_エンケンシュ: タイトル";

// ウィンドウ横幅
const int WIN_WIDTH = 600;

// ウィンドウ縦幅
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
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み


	// ゲームループで使う変数の宣言
	Player* player = new Player;
	Finish* finish = new Finish();
	int	score[5];

	// 最新のキーボード情報用
	char keys[256] = { 0 };

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };

	// ゲームループ
	while (true) {
		// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
		for (int i = 0; i < 256; i++)
		{
			oldkeys[i] = keys[i];
		}
		// 最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		// 更新処理
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

		// 描画処理
		DrawFormatString(250, 50, GetColor(255, 255, 255), "score1=%d", score[4]);
		DrawFormatString(250, 100, GetColor(255, 255, 255), "score2=%d", score[3]);
		DrawFormatString(250, 150, GetColor(255, 255, 255), "score3=%d", score[2]);
		DrawFormatString(250, 200, GetColor(255, 255, 255), "score4=%d", score[1]);
		DrawFormatString(250, 250, GetColor(255, 255, 255), "score5=%d", score[0]);
		DrawFormatString(250, 300, GetColor(255, 255, 255), "push=%d", player->KeyPush);

		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1) {
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}

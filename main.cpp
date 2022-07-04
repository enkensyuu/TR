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
	int	score1;
	int	score2;
	int	score3;
	int	score4;
	int	score5;

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

		// 描画処理
		DrawFormatString(250, 0, GetColor(255, 255, 255), "push=%d", score1);
		DrawFormatString(250, 50, GetColor(255, 255, 255), "push=%d", score2);
		DrawFormatString(250, 100, GetColor(255, 255, 255), "push=%d", score3);
		DrawFormatString(250, 150, GetColor(255, 255, 255), "push=%d", score4);
		DrawFormatString(250, 200, GetColor(255, 255, 255), "push=%d", score5);
		DrawFormatString(250, 250, GetColor(255, 255, 255), "push=%d", player->KeyPush);

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

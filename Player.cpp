#include"Player.h"
#include"DxLib.h"
#include<stdio.h>

Player::Player()
{
	KeyPush = 0;
}

void Player::Move(char* keys)
{
	if (keys[KEY_INPUT_A] == 1)
	{
		KeyPush += 1;
	}
}



#include "Finish.h"
#include"Dxlib.h"

void Finish::Move(char* keys)
{
	if (keys[KEY_INPUT_SPACE] == 1)
	{
		Finishflag = 1;
	}
	if (Finishflag == 1)
	{
		FinishTimer--;
	}
	if (FinishTimer <= 0)
	{
		Finishflag = 0;
		FinishTimer = 50;
	}
}

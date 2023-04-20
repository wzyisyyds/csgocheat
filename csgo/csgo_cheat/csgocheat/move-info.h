#pragma once
#include <winternl.h>



// -----------------------------------   SendInput“∆∂Ø  -----------------------------------
struct Send_info {
	INPUT input;
};


bool Send_Move(int x, int y);

bool Init_SendInput();
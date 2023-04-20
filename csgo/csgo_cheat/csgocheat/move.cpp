#include <math.h>
#include <windows.h>


#include"move-info.h"		
#include <iostream>


static struct Send_info Sen_info;			//Sendinput接口



// -----------------------------------   SendInput移动  -----------------------------------
static inline bool Create_Input() {
	//创建输入结构
	Sen_info.input.type = INPUT_MOUSE;
	Sen_info.input.mi.dx = 0;
	Sen_info.input.mi.dy = 0;
	Sen_info.input.mi.mouseData = 0;
	Sen_info.input.mi.dwFlags = MOUSEEVENTF_MOVE;   //MOUSEEVENTF_ABSOLUTE 代表决对位置  MOUSEEVENTF_MOVE代表移动事件
	Sen_info.input.mi.time = 0;
	Sen_info.input.mi.dwExtraInfo = 0;

	return true;
}

bool Send_Move(int x, int y) {
	//设置移动
	Sen_info.input.mi.dx = x;
	Sen_info.input.mi.dy = y;
	//移动
	UINT hr = SendInput(1, &Sen_info.input, sizeof(INPUT));
	if (!hr) {
		std::cout << "Send_Move: SendInput发送失败，报错码: " << GetLastError() << std::endl;
		return false;
	}
	return true;
}

bool Init_SendInput() {

	if (!Create_Input()) {
		return false;
	}
	return true;
}

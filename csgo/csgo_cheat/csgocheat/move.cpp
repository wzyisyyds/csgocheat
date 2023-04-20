#include <math.h>
#include <windows.h>


#include"move-info.h"		
#include <iostream>


static struct Send_info Sen_info;			//Sendinput�ӿ�



// -----------------------------------   SendInput�ƶ�  -----------------------------------
static inline bool Create_Input() {
	//��������ṹ
	Sen_info.input.type = INPUT_MOUSE;
	Sen_info.input.mi.dx = 0;
	Sen_info.input.mi.dy = 0;
	Sen_info.input.mi.mouseData = 0;
	Sen_info.input.mi.dwFlags = MOUSEEVENTF_MOVE;   //MOUSEEVENTF_ABSOLUTE �������λ��  MOUSEEVENTF_MOVE�����ƶ��¼�
	Sen_info.input.mi.time = 0;
	Sen_info.input.mi.dwExtraInfo = 0;

	return true;
}

bool Send_Move(int x, int y) {
	//�����ƶ�
	Sen_info.input.mi.dx = x;
	Sen_info.input.mi.dy = y;
	//�ƶ�
	UINT hr = SendInput(1, &Sen_info.input, sizeof(INPUT));
	if (!hr) {
		std::cout << "Send_Move: SendInput����ʧ�ܣ�������: " << GetLastError() << std::endl;
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

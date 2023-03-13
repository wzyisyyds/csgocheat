#pragma once
#include <windows.h>
#include<iostream>
#include <stdio.h>
#include <conio.h>

extern bool aimbotflag;
extern bool exitpossprocess;
extern int aimbot_vk_code;
extern int exitpossprocess_vk_code;
extern HHOOK keyboardHook;		// ¹³×Ó¾ä±ú

DWORD WINAPI hookmain(_In_ LPVOID lpParameter);
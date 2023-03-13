#pragma once
#include <windows.h>
#include<iostream>
#include <stdio.h>
#include <conio.h>

extern bool aimbotflag;
extern bool exitpossprocess;
extern HHOOK keyboardHook;		// ¹³×Ó¾ä±ú

DWORD WINAPI hookmain(_In_ LPVOID lpParameter);
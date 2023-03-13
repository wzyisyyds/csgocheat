#pragma once

#include <windows.h>
#include<winioctl.h>
#include"Mystructs.h"

#define IOCTL_GetModuleBaseAddr CTL_CODE(FILE_DEVICE_UNKNOWN,0x901,METHOD_BUFFERED,FILE_ANY_ACCESS)

#define IOCTL_ReadMemory CTL_CODE(FILE_DEVICE_UNKNOWN,0x902,METHOD_BUFFERED,FILE_ANY_ACCESS)



EXTERN_C_START

__declspec(dllexport) ULONG_PTR WINAPI GetModuleBaseAddr(HANDLE hProcessID, char* name, SIZE_T NameOfSize);

__declspec(dllexport) BOOL WINAPI ReadMemory(HANDLE hProcessID, PVOID pBuffer, PVOID pBaseAddress, SIZE_T szBufferSize);

EXTERN_C_END
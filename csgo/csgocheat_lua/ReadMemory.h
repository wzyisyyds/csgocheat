#pragma once
#include"header.h"
#include <TlHelp32.h> 

typedef ULONG_PTR(WINAPI* GetModuleBaseAddr)(HANDLE hProcessID, char* name, SIZE_T NameOfSize);

typedef BOOL(WINAPI* ReadMemory)(HANDLE hProcessID, PVOID pBuffer, PVOID pBaseAddress, SIZE_T szBufferSize);


bool dll_init(PVOID addr);


bool read_EntityList(PVOID out_buffer);

bool read_playerinfo(playerinfo& player_info);


bool read_ViewMatrix(PVOID buffer);
// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"



ULONG_PTR WINAPI GetModuleBaseAddr(HANDLE hProcessID, char* name, SIZE_T NameOfSize)
{

	if (NameOfSize>200)
	{
		return NULL;
	}
	GetModuleBaseAddrStruct data;
	GetModuleBaseAddrStruct retdata;
	data.hProcessID = hProcessID;
	for (size_t i = 0; i < NameOfSize; i++)
	{
		data.name[i] = name[i];
	}
	data.name[NameOfSize] = '\0';

	HANDLE hdevice = NULL;
	hdevice = CreateFile(L"\\\\.\\RWMDevice", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hdevice == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	ULONG bread = 0;



	bool is = DeviceIoControl(hdevice, IOCTL_GetModuleBaseAddr, &data, sizeof(GetModuleBaseAddrStruct), &retdata, sizeof(GetModuleBaseAddrStruct), &bread, NULL);
	if (!is)
	{
		CloseHandle(hdevice);
		return NULL;
	}

	CloseHandle(hdevice);

	return retdata.addr;
}



BOOL WINAPI ReadMemory(HANDLE hProcessID, PVOID pBuffer, PVOID pBaseAddress, SIZE_T szBufferSize)
{
	if (szBufferSize>1024)
	{
		return false;
	}

	HANDLE hdevice = NULL;
	hdevice = CreateFile(L"\\\\.\\RWMDevice", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hdevice == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	ULONG bread = 0;

	ReadMemoryStruct data;
	ReadMemoryStruct retdata;
	data.choice = CR3;

	data.hProcessID = hProcessID;
	data.pBaseAddress = pBaseAddress;
	data.szBufferSize = szBufferSize;



	bool is = DeviceIoControl(hdevice, IOCTL_ReadMemory, &data, sizeof(ReadMemoryStruct), &retdata, sizeof(ReadMemoryStruct), &bread, NULL);

	if (!is)
	{
		CloseHandle(hdevice);
		return false;
	}

	PUCHAR tem_pBuffer = (PUCHAR)pBuffer;


	for (size_t i = 0; i < szBufferSize; i++)
	{
		tem_pBuffer[i] = retdata.Buffer[i];
	}


	CloseHandle(hdevice);

	return true;

}






BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


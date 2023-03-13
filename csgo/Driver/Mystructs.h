#pragma once
#pragma pack(4)
typedef 
struct GetModuleBaseAddrStruct
{
	HANDLE hProcessID;
	char name[256];
	ULONG_PTR addr;
} ModuleBaseStruct, *PtrModuleBaseStruct;

typedef
enum ReadMemoryChoice
{
	CR3,
	API//�����Ѿ�����������CR3�ȶ�
} RMChoice;


typedef
struct ReadMemoryStruct
{
	RMChoice choice; //0�л�cr3 ,1δ������api
	HANDLE hProcessID;
	PVOID pBaseAddress;
	UCHAR Buffer[1024];
	SIZE_T szBufferSize;
	
} ReadMemoryS, *PtrReadMemoryS;
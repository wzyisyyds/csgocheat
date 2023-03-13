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
	API//可能已经废弃，不如CR3稳定
} RMChoice;


typedef
struct ReadMemoryStruct
{
	RMChoice choice; //0切换cr3 ,1未描述的api
	HANDLE hProcessID;
	PVOID pBaseAddress;
	UCHAR Buffer[1024];
	SIZE_T szBufferSize;
	
} ReadMemoryS, *PtrReadMemoryS;
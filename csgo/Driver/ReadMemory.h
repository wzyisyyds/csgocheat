#pragma once
#include<ntifs.h>
#include<intrin.h>


EXTERN_C_START
//ReadProcessMemory
NTSTATUS MmCopyVirtualMemory(
	IN  PEPROCESS FromProcess,
	IN  CONST VOID* FromAddress,
	IN  PEPROCESS ToProcess,
	OUT PVOID ToAddress,
	IN  SIZE_T BufferSize,
	IN  KPROCESSOR_MODE PreviousMode,
	OUT PSIZE_T NumberOfBytesCopied
);

NTSTATUS Cr3ReadMemory(HANDLE hProcessID, PVOID pBaseAddress, PVOID pBuffer, SIZE_T szBufferSize);

NTSTATUS ReadVirtualMemory(HANDLE hProcessID, PVOID pBaseAddress, PVOID pBuffer, SIZE_T szBufferSize);

NTSTATUS MdlReadMemory(HANDLE hProcessID, PVOID pBaseAddress, PVOID pBuffer, SIZE_T szBufferSize);

PVOID MapMemory(PMDL* pMdl, PVOID pAddress, SIZE_T sSize, MODE Mode);

VOID UnMapMemory(PMDL* pMdl, PVOID pAddress);

EXTERN_C_END
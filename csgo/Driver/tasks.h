#pragma once
#include"ReadMemory.h"
#include"GetModuleBase.h"
#include"Mystructs.h"



EXTERN_C_START
NTSTATUS GetModuleBaseAddrTask(PDEVICE_OBJECT pdevice, PIRP pirp);
NTSTATUS ReadMemoryTask(PDEVICE_OBJECT pdevice, PIRP pirp);

EXTERN_C_END
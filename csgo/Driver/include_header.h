#pragma once

#define DEVICE_NAME L"\\Device\\RWMDevice"
#define SYM_NAME L"\\??\\RWMDevice"

#define IOCTL_GetModuleBaseAddr CTL_CODE(FILE_DEVICE_UNKNOWN,0x901,METHOD_BUFFERED,FILE_ANY_ACCESS)

#define IOCTL_ReadMemory CTL_CODE(FILE_DEVICE_UNKNOWN,0x902,METHOD_BUFFERED,FILE_ANY_ACCESS)

#include"tasks.h"
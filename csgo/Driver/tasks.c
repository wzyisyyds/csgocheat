#include"tasks.h"



NTSTATUS GetModuleBaseAddrTask(PDEVICE_OBJECT pdevice, PIRP pirp)
{
	NTSTATUS ntstatus = STATUS_SUCCESS;
	PIO_STACK_LOCATION Pstack = IoGetCurrentIrpStackLocation(pirp);
	ULONG ioret = 0;

	PtrModuleBaseStruct buffer = (PtrModuleBaseStruct)pirp->AssociatedIrp.SystemBuffer;
	ULONG_PTR tem = GetModuleBaseAddr(buffer->hProcessID, buffer->name);
	buffer->addr = tem;
	ioret = sizeof(ModuleBaseStruct);

	pirp->IoStatus.Status = ntstatus;
	pirp->IoStatus.Information = ioret;
	IoCompleteRequest(pirp, IO_NO_INCREMENT);
	return ntstatus;
}


NTSTATUS ReadMemoryTask(PDEVICE_OBJECT pdevice, PIRP pirp)
{
	NTSTATUS ntstatus = STATUS_SUCCESS;
	PIO_STACK_LOCATION Pstack = IoGetCurrentIrpStackLocation(pirp);
	ULONG ioret = 0;

	PtrReadMemoryS buffer = (PtrReadMemoryS)pirp->AssociatedIrp.SystemBuffer;

	DbgPrint("%lld %llx %lld", (ULONG_PTR)buffer->hProcessID,(ULONG_PTR)buffer->pBaseAddress, (ULONG_PTR)buffer->szBufferSize);

	switch (buffer->choice)
	{
	case CR3:
	{
		ntstatus = Cr3ReadMemory(buffer->hProcessID, buffer->pBaseAddress, buffer->Buffer, buffer->szBufferSize);
	}
		break;
	case API:
	{
		ntstatus = ReadVirtualMemory(buffer->hProcessID, buffer->pBaseAddress, buffer->Buffer, buffer->szBufferSize);
	}
		break;
	default:
		break;
	}
	

	ioret = sizeof(ReadMemoryS);

	pirp->IoStatus.Status = ntstatus;
	pirp->IoStatus.Information = ioret;
	IoCompleteRequest(pirp, IO_NO_INCREMENT);
	return ntstatus;
}
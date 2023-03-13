#include"include_header.h"




VOID MyDriverUnload(PDRIVER_OBJECT pdriver)
{
	
	DbgPrint("bye world\n"); 

	if (pdriver->DeviceObject)
	{
		IoDeleteDevice(pdriver->DeviceObject);
		UNICODE_STRING sym_name = { 0 };
		RtlInitUnicodeString(&sym_name, SYM_NAME);
		IoDeleteSymbolicLink(&sym_name);
	}
}



NTSTATUS MJ_Cleanup(PDEVICE_OBJECT pdevice, PIRP pirp)
{
	NTSTATUS ntstatus = STATUS_SUCCESS;
	pirp->IoStatus.Status = ntstatus;
	pirp->IoStatus.Information = 0;
	IoCompleteRequest(pirp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}


NTSTATUS MJ_NoThings(PDEVICE_OBJECT pdevice, PIRP pirp)
{
	NTSTATUS ntstatus = STATUS_SUCCESS;
	pirp->IoStatus.Status = ntstatus;
	pirp->IoStatus.Information = 0;
	IoCompleteRequest(pirp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS MJ_Control(PDEVICE_OBJECT pdevice, PIRP pirp)
{
	NTSTATUS ntstatus = STATUS_SUCCESS;
	

	PIO_STACK_LOCATION Pstack = IoGetCurrentIrpStackLocation(pirp);
	ULONG iocode = Pstack->Parameters.DeviceIoControl.IoControlCode;
	ULONG ioret = 0;
	pirp->IoStatus.Status = ntstatus;
	pirp->IoStatus.Information = ioret;
	switch (iocode)
	{
	case IOCTL_GetModuleBaseAddr:
	{
		return GetModuleBaseAddrTask(pdevice, pirp);
	}
		break;
	case IOCTL_ReadMemory:
	{
		return ReadMemoryTask(pdevice, pirp);
	}
	break;

	default:
		break;
	}
	return STATUS_SUCCESS;
}



	NTSTATUS DriverEntry(PDRIVER_OBJECT pdriver, PUNICODE_STRING regpath)
	{
		NTSTATUS status = STATUS_SUCCESS;
		DbgPrint("hello world\n");
		pdriver->DriverUnload = MyDriverUnload;

		//Create Device
		UNICODE_STRING device_name = { 0 };
		PDEVICE_OBJECT pdevce = NULL;
		RtlInitUnicodeString(&device_name, DEVICE_NAME);
		status = IoCreateDevice(pdriver, 512, &device_name, FILE_DEVICE_UNKNOWN, 0, FALSE, &pdevce);

		if (!NT_SUCCESS(status))
		{
			DbgPrint("Create Device Failed:%x\n", status);
			return status;
		}

		pdevce->Flags = DO_BUFFERED_IO;;


		//Create SymbolicLink
		UNICODE_STRING sym_name = { 0 };
		RtlInitUnicodeString(&sym_name, SYM_NAME);
		status = IoCreateSymbolicLink(&sym_name, &device_name);


		if (!NT_SUCCESS(status))
		{
			DbgPrint("Create SymbolicLink Failed:%x\n", status);
			IoDeleteDevice(pdevce);
			return status;
		}

		for (size_t i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
		{
			pdriver->MajorFunction[i] = MJ_NoThings;
		}

		pdriver->MajorFunction[IRP_MJ_CLEANUP] = MJ_Cleanup;

		pdriver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = MJ_Control;

		return STATUS_SUCCESS;
	}


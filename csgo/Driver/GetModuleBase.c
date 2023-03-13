
#include"GetModuleBase.h"


//32λģ���ȡ
ULONG_PTR x86GetModuleBaseAddr(PEPROCESS pEprocess,PPEB32 pPeb32,char *szModuleName) {
	ULONG_PTR ulpModuleAddr = 0;
	ULONG ulRet = 0;
	MmCopyVirtualMemory(pEprocess, pPeb32, pEprocess, pPeb32, 0x1, UserMode, &ulRet);
	PPEB_LDR_DATA32 pldLdr = ULongToPtr(pPeb32->Ldr);
	PLIST_ENTRY32 pModuleList = (PLIST_ENTRY32)&(pldLdr->InLoadOrderModuleList);
	PLDR_DATA_TABLE_ENTRY32 pLdrData = (PLDR_DATA_TABLE_ENTRY32)ULongToPtr(pModuleList->Flink);
	while (pLdrData != pModuleList)
	{
		PWCH pwchDllName=pLdrData->BaseDllName.Buffer;
		UNICODE_STRING usDllName;	
		RtlInitUnicodeString(&usDllName, pwchDllName);
		STRING strModuleName = { 0 };
		RtlInitAnsiString(&strModuleName, szModuleName);
		UNICODE_STRING ulModuleName = { 0 };
		RtlAnsiStringToUnicodeString(&ulModuleName, &strModuleName, TRUE);
		if (RtlCompareUnicodeString(&usDllName,&ulModuleName,TRUE)==0)
		{
			ulpModuleAddr = pLdrData->DllBase;
			break;
		}
		else
		{
			pLdrData = (PLDR_DATA_TABLE_ENTRY32)ULongToPtr(pLdrData->InLoadOrderLinks.Flink);
		}
		
	}
	return ulpModuleAddr;
}
//64λģ���ȡ
ULONG_PTR x64GetModuleBaseAddr(PEPROCESS pEprocess, PPEB pPeb64,char* szModuleName) {
	ULONG_PTR  ulpModuleAddr = 0;
	ULONG ulRet = 0;
	MmCopyVirtualMemory(pEprocess, pPeb64, pEprocess, pPeb64, 0x1, UserMode, &ulRet);
	PPEB_LDR_DATA pldLdr = pPeb64->Ldr;
	PLIST_ENTRY pModuleList = (PLIST_ENTRY)&(pldLdr->InLoadOrderModuleList);
	PLDR_DATA_TABLE_ENTRY pLdrData = (PLDR_DATA_TABLE_ENTRY)(pModuleList->Flink);
	while (pLdrData != pModuleList)
	{
		STRING strModuleName = { 0 };
		RtlInitAnsiString(&strModuleName, szModuleName);
		UNICODE_STRING ulModuleName = { 0 };
		RtlAnsiStringToUnicodeString(&ulModuleName, &strModuleName, TRUE);
		if (RtlCompareUnicodeString(&ulModuleName, &pLdrData->BaseDllName, TRUE) == 0)
		{
			ulpModuleAddr = pLdrData->DllBase;
			break;
		}
		else {
			pLdrData = (PLDR_DATA_TABLE_ENTRY)pLdrData->InLoadOrderLinks.Flink;
		}
	}
		return ulpModuleAddr;
	

}
//������1����ID 2ģ������
ULONG_PTR GetModuleBaseAddr(HANDLE hProcessId,char* szModuleName) {
	if (!szModuleName) {//ģ�������Ƿ��õ�
		return 0;
	}
	//���̽ṹ
	PEPROCESS pEprocess = NULL;
	//APC�ṹ
	KAPC_STATE kApcState = { 0 };
	//ģ���ַ
	ULONG_PTR ulpModuleAddr = 0;
	//״̬
	NTSTATUS ntStatus = STATUS_SUCCESS;

	//��ȡ���̽ṹ
	ntStatus = PsLookupProcessByProcessId(hProcessId, &pEprocess);
	if (!NT_SUCCESS(ntStatus))
	{
		return 0;
	}
	//�ҿ�����
	KeStackAttachProcess(pEprocess, &kApcState);
	//��ȡPEB 32λ��64λ
	PPEB pPeb64 = PsGetProcessPeb(pEprocess);
	PPEB32 pPeb32 = PsGetProcessWow64Process(pEprocess);
	if (pPeb32)
	{
		//32PEB
		ulpModuleAddr=x86GetModuleBaseAddr(pEprocess, pPeb32, szModuleName);

	}
	else {
		//64PEB
		ulpModuleAddr=x64GetModuleBaseAddr(pEprocess, pPeb64, szModuleName);

	}
	//����ҿ�
	KeUnstackDetachProcess(&kApcState);
	return ulpModuleAddr;


}
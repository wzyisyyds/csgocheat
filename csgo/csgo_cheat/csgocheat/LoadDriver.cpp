#include"LoadDriver.h"


SC_HANDLE hSCM = NULL;
SC_HANDLE hSer = NULL;


void InstallDriver(LPCSTR lpServiceName, LPCSTR lpDisplayName)
{
	CHAR szDriverFullPath[MAX_PATH] = { 0 };
	GetFullPathNameA(lpDisplayName, MAX_PATH, szDriverFullPath, NULL);
	hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSer == NULL)
	hSer = CreateServiceA(hSCM, lpServiceName, lpDisplayName, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_AUTO_START, SERVICE_ERROR_IGNORE, szDriverFullPath, NULL, NULL, NULL, NULL, NULL);
	if (hSer==NULL)
	{
		std::cout << "������װʧ��!" << std::endl;
		return;
	}
	else {
		std::cout << "������װ�ɹ�" << std::endl;
	}
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hSer);
	system("shutdown -r -t 15");
}

void UnInstallDriver(LPCSTR lpServiceName)
{
	hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	hSer = OpenServiceA(hSCM, lpServiceName, SERVICE_ALL_ACCESS);
	BOOL bSucc = DeleteService(hSer);
	if (bSucc == TRUE)
	{
		std::cout << "����ж�سɹ�" << std::endl;
	}
	else {
		std::cout << "����ж��ʧ��" << std::endl;
		return;
	}
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hSer);
	system("shutdown -r -t 15");
}

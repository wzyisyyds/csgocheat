#pragma once
#include<string>
#include<iostream>
#include<Windows.h>
#include<time.h>

//��װ
void InstallDriver(LPCSTR lpServiceName, LPCSTR lpDisplayName);

//ж��
void UnInstallDriver(LPCSTR lpServiceName);
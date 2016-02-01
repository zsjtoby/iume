
// iUMeCloud.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include <atlfile.h>

#define IUME_INSTANCE_OBJECT_NAME                L"{1I2U3M4E-5678-IUME-CLOD-5678CLOUDXXX}"

// CiUMeCloudApp:
// 有关此类的实现，请参阅 iUMeCloud.cpp
//

class CiUMeCloudApp : public CWinApp
{
public:
	CiUMeCloudApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现
private:
	struct iUMe_Instance_Data
	{
		HWND hActiveWnd;
	};

	CAtlFileMapping<iUMe_Instance_Data> m_mapping;
	BOOL CheckInstance();

public:
	void SetActiveWindow(HWND hWndActive);

	DECLARE_MESSAGE_MAP()
};

extern CiUMeCloudApp theApp;
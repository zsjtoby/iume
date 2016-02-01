#include "stdafx.h"
#include "PubUnit.h"

CDuiString m_strApp;


CDuiString GetCurrentTimeString()
{
	SYSTEMTIME time = {0};
	TCHAR szTime[MAX_PATH] = {0};
	::GetLocalTime( &time );

	_stprintf_s(szTime,MAX_PATH,_T("%04d年%02d月%02d日 %02d:%02d:%02d"), 
		time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);

	return szTime;
}	

CDuiString GetApp()
{
	CDuiString str;
	TCHAR   szCurrentDir[MAX_PATH] = {0};

	::GetCurrentDirectory(MAX_PATH, szCurrentDir);								// 获得APP文件路径名
	str.Format(_T("%s"), szCurrentDir);

	return str;
}

BOOL IsExist(LPCTSTR strPath)
{
	CFile file;																						
	CFileStatus status;

	return file.GetStatus(strPath, status);
}

BOOL IsWindowRunBackGround(HWND hWnd)
{
#if 0 
	DWORD dwExStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
	return !(dwExStyle & WS_EX_APPWINDOW) && !::IsWindowVisible(hWnd);
#endif 

	return !::IsWindowVisible(hWnd);
}

void SetWindowRunBackGround(HWND hWnd, BOOL bBackground)
{
#if 0
	DWORD dwExStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
	if(bBackground)
	{
		//dwExStyle |= WS_EX_TOOLWINDOW;
		dwExStyle &= ~WS_EX_APPWINDOW;
	}
	else
	{
		//dwExStyle &= (~ WS_EX_TOOLWINDOW);
		dwExStyle |= WS_EX_APPWINDOW;
	}
	::SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle);
#endif
	::ShowWindow( hWnd, bBackground ? SW_HIDE:SW_SHOW );
}

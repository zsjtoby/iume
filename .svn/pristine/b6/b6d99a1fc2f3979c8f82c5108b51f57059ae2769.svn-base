#include "stdafx.h"
#include "PubUnit.h"

#include "..\MsgBox.h"
CDuiString m_strApp;

ST_BASE_CFG g_stBaseCfg;
ST_READ_CFG g_stReadCfg;
ST_ADVANCE_CFG g_stAdvanceCfg;

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

int ShowMessageBox(HWND* hWnd, LPCTSTR lpHint, LPCTSTR lpValue, int nShowId, int nImageId)
{
	CMsgBox *dlg = new CMsgBox;
	if (dlg == NULL)	return -1;
	
	dlg->Create(*hWnd, lpHint, UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
	//dlg->Create(NULL, lpHint, UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);	
	dlg->SetShowOption(lpHint, lpValue, nShowId, nImageId);
	dlg->CenterWindow();
	dlg->ShowModal();

	return CMsgBox::GetReturnValue();
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
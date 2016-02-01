// Implementation of the CNotifyIconData class and the CTrayIconImpl template.
#pragma once
 
#include <shellapi.h>
// Wrapper class for the Win32 NOTIFYICONDATA structure
class CNotifyIconData : public NOTIFYICONDATA
{
public:	
	CNotifyIconData()
	{
		memset(this, 0, sizeof(NOTIFYICONDATA));
		cbSize = sizeof(NOTIFYICONDATA);
	}
};

// Template used to support adding an icon to the taskbar.
// This class will maintain a taskbar icon and associated context menu.
template <class T>
class CTrayIconImpl
{
protected:
	UINT WM_TRAYICON;
	CNotifyIconData m_nid;
	bool m_bInstalled;
	UINT m_nDefault;
public:	
	CTrayIconImpl() : m_bInstalled(false), m_nDefault(0)
	{
		WM_TRAYICON = ::RegisterWindowMessage(_T("WM_TRAYICON"));
	}
	
	~CTrayIconImpl()
	{
		RemoveIcon();
	}

	bool InstallIcon(LPCTSTR lpszToolTip, HICON hIcon, UINT nID)
	{
		T* pT = static_cast<T*>(this);
		
		m_nid.hWnd = pT->GetHWND();
		m_nid.uID = nID;
		m_nid.hIcon = hIcon;
		m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		m_nid.uCallbackMessage = WM_TRAYICON;
		_tcscpy_s(m_nid.szTip, 100, lpszToolTip);

		m_bInstalled = Shell_NotifyIcon(NIM_ADD, &m_nid) ? true : false;

		return m_bInstalled;
	}

	bool RemoveIcon()
	{
		if (!m_bInstalled)
			return false;

		m_nid.uFlags = 0;
		return Shell_NotifyIcon(NIM_DELETE, &m_nid) ? true : false;
	}

	bool SetTooltipText(LPCTSTR pszTooltipText)
	{
		if (pszTooltipText == NULL)
			return FALSE;

		m_nid.uFlags = NIF_TIP;
		_tcscpy_s(m_nid.szTip, MAX_PATH, pszTooltipText);

		return Shell_NotifyIcon(NIM_MODIFY, &m_nid) ? true : false;
	}

	void SetDefaultItem(UINT nID) { m_nDefault = nID; }
 
	LRESULT OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (wParam != m_nid.uID)
			return 0;

		T* pT = static_cast<T*>(this);

		if (LOWORD(lParam) == WM_RBUTTONUP)
		{
			POINT pt;
			::GetCursorPos(&pt);
			pT->ShowTrayMenu(pt);
		}
		else if (LOWORD(lParam) == WM_LBUTTONDBLCLK)
		{ 
			HWND hWnd = pT->GetHWND();
			::ShowWindow(hWnd, IsIconic(hWnd)?SW_RESTORE:SW_SHOW);

			BringWindowToTop(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd); 
		}
		return 0;
	}
};

#include "stdafx.h"

#include "Skin.h"
#include "Client.h"

CSkin::CSkin(CClient* pClient, RECT rc)
	: m_client(pClient)
	, m_rect(rc)
{
	Create(NULL, _T("skin"), WS_POPUP, WS_EX_TOOLWINDOW, 0, 0);
	ShowWindow(true);
}

LPCTSTR CSkin::GetWindowClassName() const 
{ 
	return _T("CSkin");
}

void CSkin::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CSkin::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if (_tcsstr(msg.pSender->GetName(), _T("skin")) != 0)
		{
			CDuiString str = msg.pSender->GetName();
			m_client->SetImage(_wtoi(str.Right(2)));
		}
	}
}

void CSkin::InitWindow()
{
	SIZE size = m_PaintManager.GetInitSize();
	MoveWindow(m_hWnd, m_rect.right - size.cx, m_rect.top, size.cx, size.cy, FALSE);
}

CDuiString CSkin::GetSkinFile()
{
	return _T("skin.xml");
}

CDuiString CSkin::GetSkinFolder()
{
	return _T("");
}

LRESULT CSkin::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Close();
	return 0;
}

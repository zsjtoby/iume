#include "stdafx.h"
#include "About.h"

bool CAbout::m_bPopup = false;

CAbout::CAbout()
{
	m_bPopup = true;
}

CAbout::~CAbout()
{
	m_bPopup = false;
}

LPCTSTR CAbout::GetWindowClassName() const   
{   
	return _T("CAbout"); 
}

CDuiString CAbout::GetSkinFile()
{   
	return _T("About.xml");  
}

CDuiString CAbout::GetSkinFolder()              
{   
	return _T("");  
}

void CAbout::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CAbout::Notify(TNotifyUI& msg)
{	
	if (_tcsicmp(msg.sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), MY_DUILIB_BTN_CLOSE) == 0)
		{
			Close();
		}
		else if (_tcsicmp(msg.pSender->GetName(), MY_DUILIB_BTN_SURE) == 0)
		{
			Close();
		}
	}
}

bool CAbout::IsPopup()
{
	return m_bPopup;
}
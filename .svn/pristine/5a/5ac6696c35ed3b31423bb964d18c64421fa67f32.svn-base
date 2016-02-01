#include "stdafx.h"
#include "Card.h"

bool CCard::m_bPopup = false;

CCard::CCard(POINT &pt)
: m_ptOwner(pt)
{

}

CCard::~CCard()
{

}

LPCTSTR CCard::GetWindowClassName() const   
{   
	return _T("CCard"); 
}

CDuiString CCard::GetSkinFile()
{   
	return _T("Card.xml");  
}

CDuiString CCard::GetSkinFolder()              
{   
	return _T("");  
}

void CCard::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CCard::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		SetWindowPos(m_hWnd, HWND_TOPMOST, m_ptOwner.x, m_ptOwner.y, 300, 100, SWP_SHOWWINDOW);
	}		
}

bool CCard::IsPopup()
{
	return m_bPopup;
}
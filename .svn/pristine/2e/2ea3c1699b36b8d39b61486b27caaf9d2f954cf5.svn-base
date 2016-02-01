#include "stdafx.h"
#include "iCloud.h"

CCloud::CCloud()
: m_nIndex(0)
, m_bAcSwitch(false)
{
}

CCloud::~CCloud()
{
	PostQuitMessage(0);
}

LPCTSTR CCloud::GetWindowClassName() const   
{   
	return _T("CCloud"); 
}

CDuiString CCloud::GetSkinFile()
{   
	return _T("cloud.xml");  
}

CDuiString CCloud::GetSkinFolder()              
{   
	return _T("");  
}

void CCloud::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CCloud::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		
	}		
	else if (_tcsicmp(msg.sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), MY_DUILIB_BTN_CLOSE) == 0)
		{
			Close();
		}
		else if (_tcsicmp(msg.pSender->GetName(), MY_DUILIB_BTN_MIN) == 0)
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnleft")) == 0)
		{
			if (m_nIndex > 0)
			{
				m_nIndex--;
				SwitchDialog(m_nIndex);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnright")) == 0)
		{
			if (m_nIndex < 2)
			{
				m_nIndex++;
				SwitchDialog(m_nIndex);
			}
		}
	}
}

void CCloud::SwitchDialog(int nIndex)
{
	CTabLayoutUI* pTable = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabPage")));
	ASSERT(pTable != NULL);

	if (nIndex != pTable->GetCurSel())
	{
		pTable->SelectItem(m_nIndex);
		m_nIndex = nIndex;
	}
}
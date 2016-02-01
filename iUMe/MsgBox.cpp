#include "stdafx.h"
#include "MsgBox.h"

int CMsgBox::m_nReturnID = 0;

CMsgBox::CMsgBox()
{
}

CMsgBox::~CMsgBox()
{
}

LPCTSTR CMsgBox::GetWindowClassName() const   
{   
	return _T("CMsgBox"); 
}

CDuiString CMsgBox::GetSkinFile()
{   
	return _T("MsgBox.xml");  
}

CDuiString CMsgBox::GetSkinFolder()              
{   
	return _T("");  
}

void CMsgBox::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CMsgBox::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), MY_DUILIB_BTN_CLOSE) == 0)
		{
			Close();
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnID")) != 0)
		{
			Close();
		}
	}
}

void CMsgBox::SetShowOption(LPCTSTR lpHint, LPCTSTR lpValue, int nShowId, int nImageId)
{
	CDuiString strValue;
	CControlUI *pCtrl = NULL;

	pCtrl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("title")));
	ASSERT(pCtrl != NULL);
	pCtrl->SetText(lpHint);

	pCtrl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("value")));
	ASSERT(pCtrl != NULL);
	pCtrl->SetText(lpValue);

	CControlUI *pBtnGroup[3] = { NULL };
	CControlUI *pHGroup[3] = { NULL };		// xml中已默认为不可见

	for (int i = 0; i < 3; ++i)
	{
		strValue.Format(_T("btnID_%02d"), i+1);
		pBtnGroup[i] = static_cast<CButtonUI*>(m_PaintManager.FindControl(strValue));
		ASSERT(pBtnGroup[i] != NULL);

		strValue.Format(_T("btnbar_%02d"), i+1);
		pHGroup[i] = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(strValue));
		ASSERT(pHGroup[i] != NULL);
	}

	//pCtrl = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("title")));
	//ASSERT(pCtrl != NULL);
	static CDuiString strButton[7] = { _T("确定"), _T("取消"), 
					_T("是"), _T("否"), _T("重试"), _T("终止"), _T("忽略") };
	switch (nShowId)
	{
	case MB_OK:
		pHGroup[2]->SetVisible(true);
		pBtnGroup[2]->SetText(strButton[0]);
		break;
	case MB_OKCANCEL:
		pHGroup[1]->SetVisible(true);
		pHGroup[2]->SetVisible(true);
		pBtnGroup[1]->SetText(strButton[0]);
		pBtnGroup[2]->SetText(strButton[1]);
		break;
	case MB_ABORTRETRYIGNORE:
		pHGroup[0]->SetVisible(true);
		pHGroup[1]->SetVisible(true);
		pHGroup[2]->SetVisible(true);
		pBtnGroup[0]->SetText(strButton[4]);
		pBtnGroup[1]->SetText(strButton[5]);
		pBtnGroup[2]->SetText(strButton[6]);
		break;
	case MB_YESNOCANCEL:
		pHGroup[0]->SetVisible(true);
		pHGroup[1]->SetVisible(true);
		pHGroup[2]->SetVisible(true);
		pBtnGroup[0]->SetText(strButton[2]);
		pBtnGroup[1]->SetText(strButton[3]);
		pBtnGroup[2]->SetText(strButton[1]);
		break;
	case MB_YESNO:
		pHGroup[1]->SetVisible(true);
		pHGroup[2]->SetVisible(true);
		pBtnGroup[1]->SetText(strButton[2]);
		pBtnGroup[2]->SetText(strButton[3]);
		break;
	case MB_RETRYCANCEL:
		pHGroup[1]->SetVisible(true);
		pHGroup[2]->SetVisible(true);
		pBtnGroup[1]->SetText(strButton[4]);
		pBtnGroup[2]->SetText(strButton[1]);
		break;
	default:
		break;
	}

	static CDuiString strHint[3] = { _T("msgbox\\hint.png"), _T("msgbox\\warning.png"), _T("msgbox\\error.png") };
	pCtrl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("imgHint")));
	ASSERT(pCtrl != NULL);
	pCtrl->SetBkImage(strHint[nImageId%3]);

}

int CMsgBox::GetReturnValue()
{
	return m_nReturnID;
}

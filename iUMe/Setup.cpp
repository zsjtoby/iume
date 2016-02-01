#include "stdafx.h"
#include "Setup.h"

#include "..\common\file\IniFile.h"

bool CSetup::m_bPopup = false;

CSetup::CSetup(ST_SKIN stSkin)
{
	m_stSkin.strImage = stSkin.strImage;	// 本例中只用到图片作背景
	m_bPopup = true;
}

CSetup::~CSetup()
{
	m_bPopup = false;
}

LPCTSTR CSetup::GetWindowClassName() const   
{   
	return _T("CSetup"); 
}

CDuiString CSetup::GetSkinFile()
{   
	return _T("setup.xml");  
}

CDuiString CSetup::GetSkinFolder()              
{   
	return _T("");  
}

void CSetup::OnFinalMessage(HWND hWnd)
{
	RemoveObserver();

	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CSetup::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		InitBaseCfg();
		InitReadCfg();
		InitAdvanceCfg();
		ChangeSkin(m_stSkin);

		CheckExecuteBtn(0);
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
		// 点击确定时检查所有变动项并执行操作
		else if (_tcsicmp(msg.pSender->GetName(), MY_DUILIB_BTN_SURE) == 0)
		{
			if (memcmp(&g_stBaseCfg, &m_stBase, 1) != 0)
			{
				ExecuteBase();
			}
			if (memcmp(&g_stReadCfg, &m_stRead, 4) != 0)
			{
				ExecuteRead();
			}
			if (memcmp(&g_stAdvanceCfg, &m_stAdvance, 1) != 0)
			{
				ExecuteAdvance();
			}
			
			Close();
		}
		else if (_tcsicmp(msg.pSender->GetName(), MY_DUILIB_BTN_CANCEL) == 0)
		{
			Close();
		}
		// 点击应用时只检查当前页面改动项并执行
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnexecute")) == 0)
		{
			CTabLayoutUI *pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tab_setup")));
			ASSERT(pTab != NULL);

			int nIndex = pTab->GetCurSel();
			switch (nIndex)
			{
			case 0:
				ExecuteBase();
				break;
			case 1:
				ExecuteRead();
				break;
			case 2:
				ExecuteAdvance();
				break;
			default:
				break;
			}

			CheckExecuteBtn(nIndex);
		}
		else if (_tcsstr(msg.pSender->GetName(), _T("base_")) != 0)
		{
			if (_tcsicmp(msg.pSender->GetName(), _T("base_AutoRun")) == 0)
			{
				m_stBase.uAuto = !m_stBase.uAuto;
			}
			else if (_tcsicmp(msg.pSender->GetName(), _T("base_shortcut")) == 0)
			{
				m_stBase.uShortcut = !m_stBase.uShortcut;
			}
			else if (_tcsicmp(msg.pSender->GetName(), _T("base_settop")) == 0)
			{
				m_stBase.uTop = !m_stBase.uTop;
			}
			else if (_tcsicmp(msg.pSender->GetName(), _T("base_userproject")) == 0)
			{
				m_stBase.uUserProj = !m_stBase.uUserProj;
			}
			else if (_tcsicmp(msg.pSender->GetName(), _T("base_optNoExit")) == 0)
			{
				m_stBase.uExit &= 0x0;
			}
			else if (_tcsicmp(msg.pSender->GetName(), _T("base_optExit")) == 0)
			{
				m_stBase.uExit |= 0x1;
			}

			CheckExecuteBtn(0);
		}
		else if (_tcsstr(msg.pSender->GetName(), _T("read_")) != 0)
		{
			if (_tcsicmp(msg.pSender->GetName(), _T("read_optSound")) == 0)
			{
				m_stRead.uSound = !m_stRead.uSound;
			}
			//read_edtTimer
			//read_volume
			//read_cbbFont
			else if (_tcsicmp(msg.pSender->GetName(), _T("read_optAuto")) == 0)
			{
				m_stRead.uAuto = !m_stRead.uAuto;
			}
			else if (_tcsicmp(msg.pSender->GetName(), _T("read_optBold")) == 0)
			{
				m_stRead.uBold = !m_stRead.uBold;
			}
			else if (_tcsicmp(msg.pSender->GetName(), _T("read_optItalic")) == 0)
			{
				m_stRead.uItalic = !m_stRead.uItalic;
			}
			else if (_tcsstr(msg.pSender->GetName(), _T("read_cr")) != 0)
			{
				m_stRead.uImage = _wtoi(msg.pSender->GetName().Right(1));
			}

			CheckExecuteBtn(1);
		}
		else if (_tcsstr(msg.pSender->GetName(), _T("advance_")) != 0)
		{
			if (_tcsicmp(msg.pSender->GetName(), _T("advance_optAuto")) == 0)
			{
				m_stAdvance.uUpgrade = 0;
			}
			else if (_tcsicmp(msg.pSender->GetName(), _T("advance_optHint")) == 0)
			{
				m_stAdvance.uUpgrade = 1;
			}
			else if (_tcsicmp(msg.pSender->GetName(), _T("advance_optNor")) == 0)
			{
				m_stAdvance.uUpgrade = 2;
			}

			CheckExecuteBtn(2);
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("opt_basic")) == 0)
		{
			CTabLayoutUI *pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tab_setup")));
			ASSERT(pTab != NULL);

			if (pTab->GetCurSel() != 0)
			{
				pTab->SelectItem(0);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("opt_read")) == 0)
		{
			CTabLayoutUI *pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tab_setup")));
			ASSERT(pTab != NULL);

			if (pTab->GetCurSel() != 1)
			{
				pTab->SelectItem(1);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("opt_advance")) == 0)
		{
			CTabLayoutUI *pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tab_setup")));
			ASSERT(pTab != NULL);

			if (pTab->GetCurSel() != 2)
			{
				pTab->SelectItem(2);
			}
		}
	}
	else if (_tcsicmp(msg.sType, DUI_MSGTYPE_TABSELECT) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), _T("tab_setup")) == 0)
		{
			CTabLayoutUI *pTab = (CTabLayoutUI*)msg.pSender;
			CheckExecuteBtn(pTab->GetCurSel());
		}	
	}
	else if (_tcsicmp(msg.sType, DUI_MSGTYPE_ITEMSELECT) == 0)
	{
		if (_tcsstr(msg.pSender->GetName(), _T("read_")) != 0)
		{
			if (_tcsicmp(msg.pSender->GetName(), _T("read_cbbVoice")) == 0)
			{
				CComboUI* pCbb = (CComboUI*)(msg.pSender);
				m_stRead.uVoice = pCbb->GetCurSel();
			}
			else if (_tcsicmp(msg.pSender->GetName(), _T("read_cbbSpeed")) == 0)
			{
				CComboUI* pCbb = (CComboUI*)(msg.pSender);
				m_stRead.uSpeed = pCbb->GetCurSel();
			}

			CheckExecuteBtn(1);
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("advance_cbbEngine")) == 0)
		{
			CComboUI* pCbb = (CComboUI*)(msg.pSender);
			m_stAdvance.uEngine = pCbb->GetCurSel();

			CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("advance_labelEngine")));
			ASSERT(pLabel != NULL);
			pLabel->SetText(msg.pSender->GetText());

			CheckExecuteBtn(2);
		}	
	}
}

BOOL CSetup::Receive(ST_SKIN param)
{
	ChangeSkin(param);

	return TRUE;
}

void CSetup::ChangeSkin(ST_SKIN stSkin)
{
	m_stSkin.strImage = stSkin.strImage;		// 暂仅使用图片作背景

	CControlUI* background = m_PaintManager.FindControl(_T("bg"));
	if (background != NULL && !stSkin.strImage.IsEmpty())
	{
		background->SetBkImage(m_stSkin.strImage);
	}
}

void CSetup::ExecuteBase()
{
	UINT8 uValue = 0;
	memcpy(&uValue, &m_stBase, 1);
	memcpy(&g_stBaseCfg, &uValue, 1);

	CIniFile tmpIni;
	tmpIni.SetPathName(m_strApp + _T("\\bin\\setup.ini"));
	tmpIni.WriteUInt(_T("SETUP"), _T("base"), uValue);
}

void CSetup::ExecuteRead()
{
	UINT32 uValue = 0;
	memcpy(&uValue, &m_stRead, 4);
	memcpy(&g_stReadCfg, &uValue, 4);

	CIniFile tmpIni;
	tmpIni.SetPathName(m_strApp + _T("\\bin\\setup.ini"));
	tmpIni.WriteUInt(_T("SETUP"), _T("read"), uValue);
}

void CSetup::ExecuteAdvance()
{
	UINT8 uValue = 0;
	memcpy(&uValue, &m_stAdvance, 1);
	memcpy(&g_stAdvanceCfg, &uValue, 1);

	CIniFile tmpIni;
	tmpIni.SetPathName(m_strApp + _T("\\bin\\setup.ini"));
	tmpIni.WriteUInt(_T("SETUP"), _T("advance"), uValue);
}

void CSetup::CheckExecuteBtn(int nIndex)
{
	CButtonUI* btn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnexecute")));
	ASSERT(btn != NULL);

	switch (nIndex)
	{
	case 0:
		btn->SetEnabled(memcmp(&m_stBase, &g_stBaseCfg, 1));
		break;
	case 1:
		btn->SetEnabled(memcmp(&m_stRead, &g_stReadCfg, 4));
		break;
	case 2:
		btn->SetEnabled(memcmp(&m_stAdvance, &g_stAdvanceCfg, 1));
		break;
	default:
		break;
	}
}

bool CSetup::IsPopup()
{
	return m_bPopup;
}

void CSetup::InitBaseCfg()
{
	memcpy(&m_stBase, &g_stBaseCfg, 1);

	const int iCOUNT = 6;
	COptionUI *pOption[iCOUNT] = { NULL };
	CDuiString strName[iCOUNT] = {_T("base_AutoRun"), _T("base_shortcut"), _T("base_userproject"),
		_T("base_settop"), _T("base_optNoExit"), _T("base_optExit")};

	for (int i = 0; i < iCOUNT; ++i)
	{
		pOption[i] = static_cast<COptionUI*>(m_PaintManager.FindControl(strName[i]));
		ASSERT(pOption[i] != NULL);
	}

	pOption[0]->Selected(m_stBase.uAuto);
	pOption[1]->Selected(m_stBase.uShortcut);
	pOption[2]->Selected(m_stBase.uUserProj);
	pOption[3]->Selected(m_stBase.uTop);
	pOption[4]->Selected(!m_stBase.uExit);
	pOption[5]->Selected(m_stBase.uExit);
}

void CSetup::InitReadCfg()
{	
	memcpy(&m_stRead, &g_stReadCfg, 4);

	// read_optSound read_edtTimer read_volume read_cbbFont read_optAuto
	// read_optBold read_optItalic read_cr read_cbbVoice read_cbbSpeed
}

void CSetup::InitAdvanceCfg()
{
	memcpy(&m_stAdvance, &g_stAdvanceCfg, 1);

	COptionUI* pOption = NULL;
	switch (m_stAdvance.uUpgrade)
	{
	case 0:
		pOption = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("advance_optAuto")));		
		break;
	case 1:
		pOption = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("advance_optHint")));
		break;
	case 2:
		pOption = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("advance_optNor")));
		break;
	default:
		break;
	}
	ASSERT(pOption != NULL);
	pOption->Selected(true);

	CComboUI* pCbb = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("advance_cbbEngine")));
	CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("advance_labelEngine")));
	ASSERT(pCbb != NULL && pLabel != NULL);

	// 直接使用CComboUI的SelectItem及动态创建CListLabelElementUI并设置属性均不能设置默认项，
	// 故此处使用一个CLabelUI显示文本
	pLabel->SetText(pCbb->GetItemAt(m_stAdvance.uEngine)->GetText());	
}
#include "stdafx.h"
#include "Guide.h"

#include "..\common\file\IniFile.h"

bool CGuide::m_bPopup = false;
CGuide::CGuide()
: m_nCount(0)
, m_nCurrent(0)
, m_btnPrev(NULL)
, m_btnNext(NULL)
, m_labelHtml(NULL)
, m_pFrame(NULL)
{
	m_bPopup = true;
}

CGuide::~CGuide()
{
	m_bPopup = false;
}

LPCTSTR CGuide::GetWindowClassName() const   
{   
	return _T("CGuide"); 
}

CDuiString CGuide::GetSkinFile()
{   
	return _T("Guide.xml");  
}

CDuiString CGuide::GetSkinFolder()              
{   
	return _T("");  
}

void CGuide::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CGuide::Notify(TNotifyUI& msg)
{	
	if (_tcsicmp(msg.sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		m_btnNext = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnright")));
		m_btnPrev = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnleft")));
		m_labelHtml = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("current")));
		m_pFrame = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("bg")));
		ASSERT(m_btnPrev != NULL && m_btnNext != NULL && m_labelHtml != NULL && m_pFrame != NULL);

		LoadIni();
		SetGuideIndex(0);
	}
	else if (_tcsicmp(msg.sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), MY_DUILIB_BTN_CLOSE) == 0)
		{
			Close();
		}
		else if (_tcsicmp(msg.pSender->GetName(), MY_DUILIB_BTN_SURE) == 0)
		{
			Close();
		}
		else if (msg.pSender == m_btnNext)
		{
			m_nCurrent++;
			m_nCurrent %= m_nCount;

			SetGuideIndex(m_nCurrent);
		}
		else if (msg.pSender == m_btnPrev)
		{
			m_nCurrent += m_nCount;		// 用于保证当前索引为非负数
			m_nCurrent--;		
			m_nCurrent %= m_nCount;

			SetGuideIndex(m_nCurrent);
		}
	}
}

bool CGuide::IsPopup()
{
	return m_bPopup;
}

void CGuide::LoadIni()
{
	// 从单独文件中读取需要展示的图片数，方便新版特性时CGuide类不需要改变，图片命名格式固定
	// 可以根据需要自行更改
	CIniFile tmpIni;
	tmpIni.SetPathName(m_strApp + _T("\\bin\\guide.ini"));
	m_nCount = tmpIni.GetInt(_T("GUIDE"), _T("count"), 0);
}

void CGuide::SetGuideIndex(int nIndex)
{
	TCHAR szValue[MAX_PATH] = {0};
	_stprintf_s(szValue, MAX_PATH - 1, _T("file='guide\\%02d.jpg'"), nIndex);

	m_pFrame->SetBkImage(szValue);
	m_pFrame->NeedUpdate();

	CDuiString strHtml = _T("{x 10}");
	for (int i = 0; i < m_nCount; ++i)
	{	
		if (i == m_nCurrent)
		{
			strHtml += _T("{i cur.png}{x 10}");
		}
		else
		{
			strHtml += _T("{i normal.png}{x 10}");
		}	
	}

	m_labelHtml->SetText(strHtml);
}
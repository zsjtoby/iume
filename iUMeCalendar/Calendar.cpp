#include "stdafx.h"
#include "Calendar.h"

#include "..\common\tinyxml\tinyxml.h"
#include "..\common\convert\kscconv.h"

static CDuiString g_strWeek[7];

CCalendar::CCalendar()
: m_pCalendar(NULL)
{

}

CCalendar::~CCalendar()
{
	PostQuitMessage(0);
}

LPCTSTR CCalendar::GetWindowClassName() const   
{   
	return _T("CCalendar"); 
}

CDuiString CCalendar::GetSkinFile()
{   
	return _T("calendar.xml");  
}

CDuiString CCalendar::GetSkinFolder()              
{   
	return _T("");  
}

CControlUI* CCalendar::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("CalendarList")) == 0)
	{
		return new CCalendarList(m_PaintManager);
	}

	return NULL;
}

void CCalendar::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CCalendar::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		GetWeekOfXML(g_strWeek);
		InitCalendar();
		LoadMonth(2015, 1);	
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
	}
}

void CCalendar::InitCalendar()
{
	SYSTEMTIME time = {0};
	TCHAR szTime[MAX_PATH] = {0};
	::GetLocalTime( &time );

	CDuiString strValue;
	UINT8 uYear[4] = {0};
	UINT8 uMonth[2] = {0};
	
	GetYearArray(uYear, time.wYear);
	GetMonthOrDayArray(uMonth, time.wMonth);
	
	CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("YearMonth")));
	ASSERT(pLabel != NULL);
	strValue.Format(_T("{i number\\num.png 10 %d}{i number\\num.png 10 %d}{i \
number\\num.png 10 %d}{i number\\num.png 10 %d}年{i number\\num.png 10 %d}\
{i number\\num.png 10 %d}月"), uYear[3], uYear[2], uYear[1], uYear[0], uMonth[1], uMonth[0]);
	pLabel->SetText(strValue);

	UINT8 uDay[2] = {0};
	GetMonthOrDayArray(uDay, time.wDay);

	CLabelUI* pDate = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Date")));
	ASSERT(pDate != NULL);
	strValue.Format(_T("{i number\\%d.png}{i number\\%d.png}"), uDay[1], uDay[0]);
	pDate->SetText(strValue);

	CLabelUI* pWeek = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Week")));
	ASSERT(pWeek != NULL);
	strValue.Format(_T("{b}%s{/b}"), g_strWeek[time.wDayOfWeek]);
	pWeek->SetText(strValue);

	m_pCalendar = static_cast<CCalendarList*>(m_PaintManager.FindControl(_T("listCalendar")));
	ASSERT(m_pCalendar != NULL);
	m_pCalendar->InitMonthCalendar();
}

void CCalendar::GetYearArray(UINT8 uYear[4], int nYear)
{
	uYear[3] = nYear/1000;
	uYear[2] = (nYear%1000)/100;
	uYear[1] = (nYear%100)/10;
	uYear[0] = nYear%10;
}

void CCalendar::GetMonthOrDayArray(UINT8 uValue[2], int nValue)
{
	uValue[1] = nValue/10;
	uValue[0] = nValue%10;
}

void CCalendar::GetWeekOfXML(CDuiString strWeek[7])
{
	// 开始解析
	TiXmlDocument xmlDoc;
	const TiXmlElement *pRoot = NULL;
	const TiXmlElement *pItem = NULL;

	USES_CONVERSION;
	char buffer[MAX_PATH] = {0};
	sprintf(buffer, "%s\\bin\\week.xml", W2A(m_strApp));

	if (xmlDoc.LoadFile(buffer, TIXML_ENCODING_UTF8))
	{
		pRoot = xmlDoc.FirstChildElement("weekinfo");
		if (pRoot == NULL)	return ;

		bool bFirst = false;
		std::string strText;

		for (int i = 0; i < 7; i++)
		{	
			if (!bFirst)
			{
				sprintf(buffer, "week_%d", i);
				bFirst = true;
				pItem = pRoot->FirstChildElement(buffer);	
			}

			if (pItem == NULL)	continue;
			strText = pItem->Attribute("text");

			if (!strText.empty())
			{
				std::wstring strTemp1 = Utf8ToUnicode(strText);
				strWeek[i] = strTemp1.c_str();
			}
	
			sprintf(buffer, "week_%d", i+1);
			pItem = pItem->NextSiblingElement(buffer);
		}
	}
}

void CCalendar::LoadMonth(int nYear, int nMonth)
{
	m_pCalendar->LoadMonth(nYear, nMonth);
}
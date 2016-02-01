#include "StdAfx.h"
#include "CalendarList.h"

#include ".\public\kalendar.h"

namespace DuiLib
{

CCalendarList::CCalendarList(CPaintManagerUI& paint_manager)
: m_pm(paint_manager)
{
#if 0	// 此处使用固定列而不使用固定单元格大小
	SetItemSize(CSize(70, 70));
#else
	SetColumns(7);		
#endif
}

CCalendarList::~CCalendarList()
{

}

void CCalendarList::InitMonthCalendar()
{
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			CContainerUI* pDesk = NULL;
			if( !m_dlgBuilder.GetMarkup()->IsValid() )
			{
				pDesk = static_cast<CContainerUI*>(m_dlgBuilder.Create(_T("item.xml"), (UINT)0, NULL, &m_pm));
			}
			else
			{
				pDesk = static_cast<CContainerUI*>(m_dlgBuilder.Create((UINT)0, &m_pm));
			}

			if( pDesk != NULL )	
			{
				this->Add(pDesk);
			}		
		}
	}
}

void CCalendarList::LoadMonth(int nYear, int nMonth)
{
	CMyKalendar myKalendar;
	
	int nDays = myKalendar.GetMonthTotalDays(nYear, nMonth);
	int nWeek = myKalendar.Kimlarsson(nYear, nMonth, 1);	// 获取该月 1 号为星期几

	COptionUI* pOption = NULL;
	CContainerUI* pControl = NULL;

	CDuiString strValue;
	int i = 0;
	int temp = 0;

	if (nWeek > 0)		// 需要统计上月天数
	{
		nMonth--;

		if (nMonth == 0)
		{
			nYear--;
			nMonth += 12;
		}

		temp = myKalendar.GetMonthTotalDays(nYear, nMonth);
	}

	for (i = 0; i < nWeek; ++i)
	{
		pControl = static_cast<CContainerUI*>(GetItemAt(i));
		pOption = static_cast<COptionUI*>(pControl->FindSubControl(_T("textweek")));
		strValue.Format(_T("text=\"{c #00007F}{b}%d{/b}{/c}{x 5}{c #FF0000}初一{/c}\""), temp - nWeek + i + 1);
		pOption->ApplyAttributeList(strValue);

		pOption->SetEnabled(false);
	}

	for (i = nWeek; i < nWeek + nDays; ++i)
	{
		pControl = static_cast<CContainerUI*>(GetItemAt(i));
		pOption = static_cast<COptionUI*>(pControl->FindSubControl(_T("textweek")));
		strValue.Format(_T("text=\"{c #00007F}{b}%d{/b}{/c}{x 5}{c #FF0000}初一{/c}\""), i - nWeek + 1);
		pOption->ApplyAttributeList(strValue);
		pOption->SetEnabled(true);
	}

	for (i = nWeek + nDays; i < 6 * 7; ++i)
	{
		pControl = static_cast<CContainerUI*>(GetItemAt(i));
		pOption = static_cast<COptionUI*>(pControl->FindSubControl(_T("textweek")));
		strValue.Format(_T("text=\"{c #00007F}{b}%d{/b}{/c}{x 5}{c #FF0000}初一{/c}\""), i - nWeek - nDays + 1);
		pOption->ApplyAttributeList(strValue);
		pOption->SetEnabled(false);
	}
}

}	// namespace DuiLib
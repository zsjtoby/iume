#ifndef __CALENDAR_H__
#define __CALENDAR_H__

#include "CalendarList.h"

class CCalendar : public WindowImplBase
{
public:
	CCalendar();
	~CCalendar();

public:
	virtual LPCTSTR GetWindowClassName() const;
	virtual void OnFinalMessage(HWND hWnd);
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder(); 
	void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

public:
	void LoadMonth(int nYear, int nMonth);

private:
	void InitCalendar();
	void GetYearArray(UINT8 uYear[4], int nYear);
	void GetMonthOrDayArray(UINT8 uValue[2], int nValue);
	void GetWeekOfXML(CDuiString strWeek[7]);

private:
	CCalendarList *m_pCalendar;
};

#endif __CALENDAR_H__
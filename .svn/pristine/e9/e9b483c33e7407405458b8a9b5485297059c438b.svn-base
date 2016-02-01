#pragma once

#ifndef __CALENDAR_LIST_H__
#define __CALENDAR_LIST_H__

namespace DuiLib
{

class CCalendarList : public CTileLayoutUI
{
public:
	CCalendarList(CPaintManagerUI& paint_manager);
	virtual ~CCalendarList(void);

public:
	void LoadMonth(int nYear, int nMonth);
	void InitMonthCalendar();

private:
	CPaintManagerUI& m_pm;
	CDialogBuilder m_dlgBuilder;
};

}	// namespace DuiLib

#endif	// #ifndef __CALENDAR_LIST_H__
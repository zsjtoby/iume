#pragma once

#ifndef _BOOK_CLIENT_H__
#define _BOOK_CLIENT_H__

namespace DuiLib
{

class CBookClient : public CTileLayoutUI
{
public:
	CBookClient(CPaintManagerUI& paint_manager);
	virtual ~CBookClient(void);

public:
	bool AddNode(BOOK_INFO *item);
	void InEdit();
	bool RemoveAt(int nIndex);
	bool GetEdit() const;

private:
	CPaintManagerUI& m_pm;
	CDialogBuilder m_dlgBuilder;
	bool m_bEdit;
};

}	// namespace DuiLib

#endif
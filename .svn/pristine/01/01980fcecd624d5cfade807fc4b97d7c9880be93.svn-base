#ifndef __CARD_H__
#define __CARD_H__

class CCard : public WindowImplBase
{
public:
	CCard(POINT &pt);
	~CCard();

public:
	virtual LPCTSTR GetWindowClassName() const;
	virtual void OnFinalMessage(HWND hWnd);
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder(); 
	void Notify(TNotifyUI& msg);

public:
	static bool IsPopup();

private:
	static bool m_bPopup;
	POINT m_ptOwner;
};

#endif __CARD_H__
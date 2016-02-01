#ifndef __ABOUT_H__
#define __ABOUT_H__

class CAbout : public WindowImplBase
{
public:
	CAbout();
	~CAbout();

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
};

#endif // ifndef __ABOUT_H__
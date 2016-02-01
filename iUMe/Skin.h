#ifndef __SKIN_H__
#define __SKIN_H__

class CClient;
using namespace DuiLib;

class CSkin : public WindowImplBase
{
public:
	CSkin(CClient* pClient, RECT rc);

	LPCTSTR GetWindowClassName() const;
	virtual void OnFinalMessage(HWND hWnd);
	void Notify(TNotifyUI& msg);
	virtual void InitWindow();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	RECT m_rect;
	CClient* m_client;
};

#endif // #ifndef __SKIN_H__
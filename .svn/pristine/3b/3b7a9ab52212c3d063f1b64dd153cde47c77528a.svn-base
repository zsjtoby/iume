#ifndef __MSG_BOX_H__
#define __MSG_BOX_H__

class CMsgBox : public WindowImplBase
{
public:
	CMsgBox();
	~CMsgBox();

public:
	static int GetReturnValue();
	void SetShowOption(LPCTSTR lpHint, LPCTSTR lpValue, int nShowId, int nImageId = 0);

public:
	virtual LPCTSTR GetWindowClassName() const;
	virtual void OnFinalMessage(HWND hWnd);
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder(); 
	void Notify(TNotifyUI& msg);

private:
	static int m_nReturnID;
};

#endif __MSG_BOX_H__
#ifndef __GUIDE_H__
#define __GUIDE_H__

class CGuide : public WindowImplBase
{
public:
	CGuide();
	~CGuide();

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
	int m_nCount;
	int m_nCurrent;

	CButtonUI* m_btnPrev;
	CButtonUI* m_btnNext;
	CLabelUI*	m_labelHtml;
	CVerticalLayoutUI* m_pFrame;

	void LoadIni();
	void SetGuideIndex(int nIndex);
};

#endif // ifndef __GUIDE_H__
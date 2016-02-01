#ifndef __SETUP_H__
#define __SETUP_H__

class CSetup : public WindowImplBase, public SkinChangedReceiver
{
public:
	CSetup(ST_SKIN stSkin);
	~CSetup();

public:
	virtual LPCTSTR GetWindowClassName() const;
	virtual void OnFinalMessage(HWND hWnd);
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder(); 
	void Notify(TNotifyUI& msg);

public:
	static bool IsPopup();
	virtual BOOL Receive(ST_SKIN param);

private:
	ST_BASE_CFG m_stBase;	
	ST_READ_CFG m_stRead;
	ST_ADVANCE_CFG m_stAdvance;
	ST_SKIN m_stSkin;

private:
	static bool m_bPopup;
	
private:
	void InitBaseCfg();
	void InitReadCfg();
	void InitAdvanceCfg();
	void ChangeSkin(ST_SKIN stSkin);
	void CheckExecuteBtn(int nIndex);

	void ExecuteBase();
	void ExecuteRead();
	void ExecuteAdvance();
};

#endif __SETUP_H__
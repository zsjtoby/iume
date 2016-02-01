#ifndef __I_CLOUD_H__
#define __I_CLOUD_H__

typedef struct tag_WeatherInfo
{
	CDuiString strProvince;		// 省份
	CDuiString strCity;			// 城市
	CDuiString strCounty;		// 县城
	CDuiString strAreaId;		// 城市 id
	int nTemp;		// 温度
	CDuiString strWD;	// 风向
}WEATHER_INFO;

class CCloud : public WindowImplBase
{
public:
	CCloud();
	~CCloud();

public:
	virtual LPCTSTR GetWindowClassName() const;
	virtual void OnFinalMessage(HWND hWnd);
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder(); 
	void Notify(TNotifyUI& msg);

public:


private:
	int m_nIndex;
	bool m_bAcSwitch;
	void SwitchDialog(int nIndex);

};

#endif // ifndef __I_CLOUD_H__
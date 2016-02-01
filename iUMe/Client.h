#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "BookClient.h"
#include "AppClient.h"
#include "trayiconimpl.h"

#include <list>
using namespace std;

class CClient : public WindowImplBase, public CTrayIconImpl<CClient>
{
public:
	CClient();
	~CClient();

public:
	virtual LPCTSTR    GetWindowClassName() const;  
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder(); 
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	void SetImage(int nIndex);		// 根据索引设置背景图片
	void ShowTrayMenu(POINT pt);  //显示托盘菜单

private:
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void OnFinalMessage(HWND hWnd);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	void InitLocalUI();				// 根据本地数据初始化界面
	void SaveLocalUI();				// 报存界面相关信息
	void InitWebBrowser();			// 初始化浏览器首页
	void SwitchTabctrl(int nIndex);	// 切换首页/书城/应用
	void LoadLocalApplication();	// 加载本地应用信息
	void LoadApplication();			// 加载应用信息
	void LoadHistoryBook();			// 加载历史书籍信息(在数据库中存储)
	void LoadCurrentEbook(BOOK_INFO *stBook);	// 加载当前书籍信息
	void LoadCurrentApp(APP_INFO *stApp);
	void AddEbookPage(BOOK_INFO *node);			// 添加书籍阅读页
	void RemoveEbookPage(CButtonUI *pCtrl);		// 移除书籍阅读页
	void SwitchEbookPage();						// 切换书籍阅读页
	void MovePrevPage();		// 向前移动书籍阅读页
	void MoveNextPage();		// 向后移动书籍阅读页
	int IsOpenEbook(BOOK_INFO *stBook);		// 判断书籍信息是否已经打开
	bool IsSaveEbook(CDuiString strEbook);	// 判断书籍信息是否已经保存
	bool IsSaveApp(CDuiString strApp);
	void SaveEbook(CDuiString strEbook, BOOK_INFO *stBook);	// 保存书籍信息
	void SaveApp(CDuiString strApp, APP_INFO *stApp);
	void ShowNoEbook();						// 是否显示没有书籍提示
	int DeleteEbookDB(BOOK_INFO *stBook);	// 在数据库中删除书籍信息
	int DeleteAppDB(APP_INFO *stApp);
	void AnalysisApp(APP_INFO *stApp);		// 分析exe名识别程序名称并使用重绘图标
	void LoadIni();

protected:
	void QuitIUME();
	void Notify(TNotifyUI& msg);
	afx_msg LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	enum { emMaxBook = 5 };			// emMaxBook 表示最大能打开的书籍数量
	SkinChangedObserver	m_skinServer;
	int m_nImage;
	int m_nMaxBook;
	int m_nCurrPage;

	ST_SKIN m_stSkin;

	struct ST_CTRL_PT
	{
		CButtonUI* pButton;			// 关闭按钮
		COptionUI* pOption;			// 选项按钮
		CHorizontalLayoutUI* pHorizontal;	// 整个区域位于水平条
		BOOK_INFO* pBook;
	};
	list<ST_CTRL_PT> m_list;
	BOOL m_bAppEdit;			// 编辑应用模式
	BOOL m_bNoBook;				// 是否没有书籍,TRUE表示有书籍，FALSE表示无书籍
};

#endif // #ifndef __CLIENT_H__

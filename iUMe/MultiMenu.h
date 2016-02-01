
#ifndef __MULTI_MENU_H__
#define __MULTI_MENU_H__

const TCHAR* const MenuBkImage = _T("file='menu\\menu_bk.png' corner='30,8,8,8'");
const TCHAR* const ExplandIcon = _T("menu\\right.png");
const TCHAR* const MenuDefaultList = _T("inset=\"2,2,2,2\" bkcolor=\"0xFFE9FFFF\" bordersize=\"0\" header=\"hidden\"");
const TCHAR* const MenuHightLight = _T("file='menu\\menu_hot_bk.png' corner='2,2,2,2'");

struct ContextMenuParam
{
	HWND hWnd;
	WPARAM wParam; // 取值为1时，表示销毁整个菜单， 取值为2时，表示只将hwnd及其所有父窗口显示
};

class MenuElementUI;
class CMultiMenuUI : public CWindowWnd
{
public:
	enum menuLayout: unsigned long
	{
		TEXT_ALIGN_LEFT_DEFAULT = 0,  //文本默认左对齐
		TEXT_ALIGN_CENTER,     //文本居中显示
		TEXT_ALIGN_RIGHT,     //文本右对齐

		DEFAULT_LINE_HEIGHT = 6,   //默认分隔线所占高度
		ITEM_DEFAULT_HEIGHT = 20,   //每一个item的默认高度（只在竖状排列时自定义）
		ITEM_DEFAULT_ICON_WIDTH = 28,  //默认图标所占宽度
		HEADER_DEFAULT_HEIGHT = 40,   //默认的表头高度
		ITEM_DEFAULT_WIDTH = 150,   //窗口的默认宽度
		DEFAULT_TEXT_COLOR = 0xFF000000, //默认字体颜色
		DEFAULT_LINE_COLOR = 0xFF85E4FF,    //默认分隔线颜色
	};

public:
	CMultiMenuUI(menuLayout textalign = TEXT_ALIGN_LEFT_DEFAULT);

	virtual ~CMultiMenuUI(void);

	LPCTSTR GetWindowClassName() const { return _T("MenuUI"); };

	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };

	LPVOID GetInterface(LPCTSTR pstrName);

	void OnFinalMessage(HWND /*hWnd*/) ;

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	/*****
	* 功    能：功能函数 添加菜单选项
	* 参    数：content选项的文本信息，itemID选项对应ID，iconImageName选项左边图标，textcol文本字体颜色，height选项所占高度，extend是否支持扩展
	* 返 回 值：支持扩展时返回扩展窗口指针，否则返回NULL
	******/
	CMultiMenuUI* Add(LPCTSTR content, UINT itemID = 0, LPCTSTR iconImageName = L"", DWORD textcol = 0xFF000000, int height = 0, bool extend = false);//支持html文本

	CMultiMenuUI* Add(MenuElementUI *pctrl, int height = 0, bool extend = false);//添加自定义控件项

	/*****
	* 功    能：绘制分隔线
	* 参    数：color 分隔线显示的颜色
	******/
	void AddLine(DWORD color = DEFAULT_LINE_COLOR);

	void SetItemHeight(int height = ITEM_DEFAULT_HEIGHT);//设置选项的统一高度

	void SetItemWidth(int width = ITEM_DEFAULT_WIDTH);//设置选项的宽度（即菜单窗口宽度）

	/*****
	* 功    能：处理窗口信号
	* 参    数：param 信号数据

	******/
	void ReceiveCloseMsg(ContextMenuParam &param);

	/*****
	* 功    能：窗口初始化，完成基本属性设置
	* 参    数：pt窗口显示的基点，hParent父窗口句柄，
	*   pOwner所属选项，nLeftPos左边预留图标宽度
	******/
	void InitWnd(POINT &pt, HWND hParent = NULL, MenuElementUI* pOwner = NULL, int nLeftPos = ITEM_DEFAULT_ICON_WIDTH);

	void UpdateWnd();//窗口更新

	POINT GetPos();

	void SetPos();
	void AddMenuHeader(int height = HEADER_DEFAULT_HEIGHT, LPCTSTR text = L"", LPCTSTR bkimagename=L"TrayMenu.png");

public:
	CPaintManagerUI m_pm;
	HWND m_hParent;   //父窗口
	CListUI *m_list;  //默认属性"inset=\"2,2,2,2\" bkcolor=\"0xFFE9FFFF\" bordersize=\"0\" itemtextpadding=\"30,0,0,0\" header=\"hidden\" itemshowhtml=\"true\""

private:
	MenuElementUI* m_pOwner;//窗口所属的选项
	POINT m_basePoint;     //窗口左上角顶点位置
	int m_itemHeight;       //每一项的高度
	int m_itemWidth;  //窗口的宽度
	int m_wndHeight;  //窗口的高度
	int m_leftWidth;  //左边图标所占宽度
	menuLayout m_textAlign; //文本对齐方式 默认左对齐
};

class MenuElementUI : public CListContainerElementUI
{

public:
	MenuElementUI(UINT ID = 0);

	~MenuElementUI(){}

	LPCTSTR GetClass() const {return _T("MenuElementUI");}

	LPVOID  GetInterface(LPCTSTR pstrName);

	void DoEvent(TEventUI& event);

	void DoPaint(HDC hDC, const RECT& rcPaint);


	void SetText(LPCTSTR pstrText, DWORD textColor, CMultiMenuUI::menuLayout textalign =  CMultiMenuUI::TEXT_ALIGN_LEFT_DEFAULT);

	void SetIcon(int width, LPCTSTR iconName = L"");//16*16BMP图片

	void SetExplandIcon(LPCTSTR iconName = ExplandIcon);//16*16BMP图片

	void SetLine(bool isline, DWORD col, int width);

	bool AddCtrl(CControlUI* pControl);//添加自定义控件

	CMultiMenuUI* CreateWnd(POINT &pt, HWND hParent, int nLeftPos );

	void DrawItemBk(HDC hDC, const RECT& rcItem);

public:
	UINT   uID;    //菜单ID
	enum
	{
		ICON_LEFT_WIDTH = 4, //图标的左边距
		EXPLAND_ICON_WIDTH = 20,
	};

private:
	CMultiMenuUI   *m_pWnd;   //弹出窗体
	CHorizontalLayoutUI *m_Layout; 
	CLabelUI *m_Text;   //显示文本
	CLabelUI *m_Icon;   //图标
	bool m_bDrawLine;   //是否画线
	bool m_bextend;
	DWORD m_lineColor;          //分隔线颜色
	int m_iconWidth;
};

class WndsVector
{
private:
	typedef std::vector<CMultiMenuUI*> ReceiversVector;
	static ReceiversVector receiverWnd;
public:
	static HWND rootHwnd;

	/*****
	* 功    能：将窗口指针压入窗口队列

	******/
	static inline void AddWnd(CMultiMenuUI* ptr)
	{
		receiverWnd.push_back(ptr);
	}

	/*****
	* 功    能：将窗口信号发送给队列中的每一个窗口

	******/
	static void BroadCast(ContextMenuParam &param)
	{
		if (receiverWnd.empty())
			return;
		ReceiversVector::reverse_iterator it = receiverWnd.rbegin();
		for (; it != receiverWnd.rend(); it ++)
		{
			if ( *it != NULL )
				(*it)->ReceiveCloseMsg(param);
		}
	}

	/*****
	* 功    能：查找窗口队列中包含该句柄的窗口类指针

	******/
	static CMultiMenuUI* FindWndClass(HWND hwnd)
	{
		if (receiverWnd.empty())
			return NULL;
		ReceiversVector::iterator it = receiverWnd.begin();
		for (; it != receiverWnd.end(); it ++)
		{
			if (*it != NULL && (*it)->GetHWND() == hwnd)
				return *it;
		}
		return NULL;
	}
	static inline void RemoveAll()
	{
		receiverWnd.clear();
	}
}; 

#endif // ifndef __MULTI_MENU_H__
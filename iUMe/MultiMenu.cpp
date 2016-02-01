#include "stdafx.h"
#include "MultiMenu.h"

//////////////////////////////////////.cpp

MenuElementUI::MenuElementUI(UINT ID)
	:uID(ID),
	m_pWnd(NULL),
	m_Layout(NULL),
	m_Text(NULL),
	m_Icon(NULL),
	m_bDrawLine(false),
	m_bextend(false),
	m_lineColor(0xFFFFFFFF),
	m_iconWidth(0)
{
	m_Layout = new CHorizontalLayoutUI;
	RECT rc = {0,3,0,0};
	m_Layout->SetInset(rc);
	Add(m_Layout);
}

void MenuElementUI::SetText(LPCTSTR pstrText, DWORD textColor, CMultiMenuUI::menuLayout textalign)
{
	if(m_Text == NULL) m_Text = new CLabelUI;
	m_Text->SetShowHtml(true);
	if(textalign == CMultiMenuUI::TEXT_ALIGN_CENTER)
		m_Text->SetAttribute(L"align",L"center");
	else if(textalign == CMultiMenuUI::TEXT_ALIGN_RIGHT)
		m_Text->SetAttribute(L"align", L"right");
	m_Text->SetTextColor(textColor);
	m_Text->SetText(pstrText);
	m_Layout->Add(m_Text);
}

void MenuElementUI::SetExplandIcon(LPCTSTR iconName)
{
	if(EXPLAND_ICON_WIDTH <= 0)
		return;
	CString str;
	if (iconName != L"")
	{
		str.Format(L"{i %s }",iconName);
	}
	CLabelUI *icon = new CLabelUI;
	icon->SetShowHtml(true);
	icon->SetText(str);
	icon->SetFixedWidth(EXPLAND_ICON_WIDTH);
	m_Layout->Add(icon);
}

void MenuElementUI::SetIcon(int width, LPCTSTR iconName)
{
	CString str;
	if (iconName != L"")
	{
		int i = ICON_LEFT_WIDTH;
		str.Format(L"{x %d}{i %s }",i, iconName);
	}
	if(m_Icon == NULL) m_Icon = new CLabelUI;

	m_Icon->SetFixedWidth(width);
	m_Icon->SetShowHtml(true);
	m_Icon->SetText(str);
	m_Layout->AddAt(m_Icon, 0);
}

bool MenuElementUI::AddCtrl(CControlUI* pControl)
{
	return m_Layout->Add(pControl);
}

void MenuElementUI::SetLine(bool isline, DWORD col,  int width)
{
	m_bDrawLine = isline;
	m_lineColor = col;
	m_iconWidth = width;
}
void MenuElementUI::DrawItemBk(HDC hDC, const RECT& rcItem)
{
	if(m_bDrawLine)
	{
		RECT rcLine = { m_rcItem.left +  m_iconWidth, m_rcItem.bottom - 3, m_rcItem.right-4, m_rcItem.bottom - 3 };
		CRenderEngine::DrawLine(hDC, rcLine, 1, m_lineColor);
	}
	else
		return CListContainerElementUI::DrawItemBk(hDC,rcItem);
}

LPVOID MenuElementUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, L"MenuElement") == 0 ) return static_cast<MenuElementUI*>(this);    
	return CListContainerElementUI::GetInterface(pstrName);
}

void MenuElementUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;
	DrawItemBk(hDC, m_rcItem);
	CContainerUI::DoPaint(hDC, rcPaint);
}

CMultiMenuUI* MenuElementUI::CreateWnd(POINT &pt, HWND hParent, int nLeftPos )
{
	m_pWnd = new CMultiMenuUI;
	ASSERT(m_pWnd != NULL);

	SetExplandIcon();
	m_pWnd->InitWnd(pt, hParent, this, nLeftPos);

	return m_pWnd;
}
void MenuElementUI::DoEvent(TEventUI& event)
{
	if( event.Type == UIEVENT_MOUSEENTER )
	{
		if ( GetIndex() == m_pOwner->GetCurSel())
			return;
		m_pOwner->SelectItem(GetIndex(), true);

		ContextMenuParam param;
		param.hWnd = m_pManager->GetPaintWindow();
		param.wParam = 2;
		WndsVector::BroadCast(param);

		if (m_pWnd != NULL)
		{
			if (m_bextend)
			{
				m_pWnd->ShowWindow(true);  
			}
			else
			{
				m_bextend = !m_bextend;
				m_pWnd->SetPos();
				m_pWnd->UpdateWnd();
			}
		}
		return;
	}
	else if( event.Type == UIEVENT_BUTTONDOWN )
	{
		if (m_pWnd == NULL)
		{
			PostMessage(WndsVector::rootHwnd, uID, event.wParam, event.lParam);

			ContextMenuParam param;
			param.hWnd = NULL;
			param.wParam = 1;
			WndsVector::BroadCast(param);
			return;
		}
	}
	return CListContainerElementUI::DoEvent(event);
}

////////////////////////////////////////////////////////
CMultiMenuUI::CMultiMenuUI(menuLayout textalign):
m_itemHeight(ITEM_DEFAULT_HEIGHT),
	m_itemWidth(ITEM_DEFAULT_WIDTH),
	m_hParent(NULL),
	m_pOwner(NULL),
	m_wndHeight(0),
	m_leftWidth(0),
	m_textAlign(textalign)
{
}

CMultiMenuUI::~CMultiMenuUI(void)
{
}

LPVOID CMultiMenuUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, L"Menu") == 0 ) return static_cast<CMultiMenuUI*>(this);    
	return NULL;
}

LRESULT CMultiMenuUI::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_pm.Init(m_hWnd);

	m_list = new CListUI;
	m_list->SetManager(&m_pm,NULL);
	m_list->ApplyAttributeList(MenuDefaultList);
	m_list->SetBkImage(MenuBkImage);
	m_list->SetSelectedItemImage(MenuHightLight);
		
	m_pm.AttachDialog(m_list);
	m_wndHeight = 4;
	if (m_pOwner == NULL)//首次创建
	{
		WndsVector::RemoveAll();
		WndsVector::rootHwnd = m_hParent;
	}
	WndsVector::AddWnd(this);
	return 0;
}
LRESULT CMultiMenuUI::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND hFocusWnd = (HWND)wParam;

	if( hFocusWnd == m_hWnd ) return 0;

	if (hFocusWnd != NULL && WndsVector::FindWndClass(hFocusWnd) != NULL)
		return 0;

	ContextMenuParam param;
	param.hWnd = GetHWND();
	param.wParam = 1;
	WndsVector::BroadCast(param);

	return 0;
}
LRESULT CMultiMenuUI::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (m_hParent == WndsVector::rootHwnd)
		WndsVector::RemoveAll();
	return 0;
}

void CMultiMenuUI::ReceiveCloseMsg(ContextMenuParam &param)
{
	switch (param.wParam)
	{
	case 1:
		if (m_hParent == WndsVector::rootHwnd )
		{
			Close();
		}
		break;
	case 2:
		{
			HWND hParent = GetParent(m_hWnd);
			while (hParent != NULL)
			{
				if (hParent == param.hWnd)
				{
					ShowWindow(false);
					break;
				}
				hParent = GetParent(hParent);
			}
		}
		break;
	default:
		break;
	}
}
CMultiMenuUI* CMultiMenuUI::Add(LPCTSTR content,UINT itemID , LPCTSTR iconImageName, DWORD textcol, int height, bool extend)
{
	if (height <= 0)
		height = m_itemHeight;

	MenuElementUI *new_node = new MenuElementUI(itemID);
	new_node->SetText(content, textcol, m_textAlign);
	new_node->SetFixedHeight(height);
	m_list->Add(new_node);
	m_wndHeight += height;

	//文本居中时不支持扩展和左边图标
	if (m_textAlign == TEXT_ALIGN_CENTER)
		return NULL;

	if (m_leftWidth > 4)
		new_node->SetIcon(m_leftWidth, iconImageName);

	//文本右对齐时不支持扩展
	if(m_textAlign == TEXT_ALIGN_RIGHT)
		return NULL;

	if (extend)
	{
		POINT pt = {0,0};
		CMultiMenuUI *wndPtr = new_node->CreateWnd(pt, m_hWnd, m_leftWidth);
		wndPtr->SetItemWidth(m_itemWidth);
		wndPtr->SetItemHeight(m_itemHeight);
		return wndPtr;
	}
	else
		return NULL;
}

CMultiMenuUI* CMultiMenuUI::Add(MenuElementUI *pctrl, int height, bool extend)
{
	if (height <= 0)
		height = m_itemHeight;
	pctrl->SetFixedHeight(height);
	m_list->Add(pctrl);
	m_wndHeight += height;

	if (extend)
	{
		POINT pt = {0,0};
		CMultiMenuUI *wndPtr = pctrl->CreateWnd(pt, m_hWnd, m_leftWidth);
		wndPtr->SetItemWidth(m_itemWidth);
		wndPtr->SetItemHeight(m_itemHeight);
		return wndPtr;
	}
	else
		return NULL;
}

void CMultiMenuUI::AddLine(DWORD color)
{
	MenuElementUI *new_node = new MenuElementUI();
	new_node->SetLine(true, color, m_leftWidth); 
	new_node->SetFixedHeight(DEFAULT_LINE_HEIGHT);
	m_list->Add(new_node);
	m_wndHeight += DEFAULT_LINE_HEIGHT;
}

void CMultiMenuUI::SetItemHeight(int height /* = ITEM_DEFAULT_HEIGHT */)
{
	m_itemHeight = height;
}

void CMultiMenuUI::SetItemWidth(int width /* = ITEM_DEFAULT_WIDTH */)
{
	m_itemWidth = width;
}

void CMultiMenuUI::InitWnd(POINT &pt, HWND hParent/* = NULL */, MenuElementUI* pOwner /* = NULL */, int nLeftPos /* = ITEM_DEFAULT_ICON_WIDTH */)
{
	m_basePoint = pt;
	m_hParent = hParent;
	m_pOwner = pOwner;
	if (m_textAlign == TEXT_ALIGN_CENTER)
		m_leftWidth = 4;
	else
		m_leftWidth = nLeftPos;

	Create((m_pOwner == NULL) ? m_hParent : m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST, CDuiRect());
}

POINT CMultiMenuUI::GetPos()
{
	return m_basePoint;
}

void CMultiMenuUI::SetPos()
{
	if (m_pOwner != NULL)
	{
		CMultiMenuUI *parent = WndsVector::FindWndClass(m_hParent);
		ASSERT(parent != NULL);
		POINT pt = parent->GetPos();
		CDuiRect rc = m_pOwner->GetPos();
		m_basePoint.x = pt.x + m_itemWidth;
		m_basePoint.y = pt.y + rc.top;
	}
}
void CMultiMenuUI::AddMenuHeader(int height /* = HEADER_DEFAULT_HEIGHT */, LPCTSTR text /* = "" */, LPCTSTR bkimagename/* ="" */)
{
	m_list->SetAttribute(L"header", L"true");
	CListHeaderItemUI *tt = new CListHeaderItemUI;
	tt->SetFixedHeight(height);
	tt->SetText(text);
	tt->SetBkImage(bkimagename);
	m_list->Add(tt);
	m_wndHeight += height;
}
void CMultiMenuUI::UpdateWnd()
{
	ASSERT(m_leftWidth < m_itemWidth && "the m_leftWidth is larger than m_itemWidth");
	int num = m_list->GetCount();
	if( num == 0 )
	{
		Close();
		return ;
	}
	//检测窗体是否超出桌面
	MONITORINFO oMonitor = {}; 
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	m_pOwner == NULL ? num = 1 : num = 2;
	if( m_basePoint.x + m_itemWidth > rcWork.right )//看横向是否越界
		m_basePoint.x -= m_itemWidth * num;
	if ( m_basePoint.y + m_wndHeight > rcWork.bottom )//看纵向是否越界
		m_basePoint.y - m_wndHeight > rcWork.top ? m_basePoint.y -= m_wndHeight : m_basePoint.y = rcWork.top + 4;

	SetWindowPos(m_hWnd, HWND_TOPMOST, m_basePoint.x, m_basePoint.y, m_itemWidth, m_wndHeight, SWP_SHOWWINDOW);
	SetForegroundWindow(m_hWnd);
}
void CMultiMenuUI::OnFinalMessage(HWND /*hWnd*/) 
{
	delete this;
}
LRESULT CMultiMenuUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg )
	{
	case WM_CREATE:       
		lRes = OnCreate(uMsg, wParam, lParam, bHandled); 
		break;
	case WM_KILLFOCUS:       
		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); 
		break;
	case WM_DESTROY:
		lRes = OnDestroy(uMsg, wParam, lParam, bHandled);
	default:
		bHandled = FALSE;
		break;
	}
	if( bHandled ) 
		return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) 
		return lRes;
	//应用层和 DUILIB 都不处理的消息交由系统默认处理 
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}
std::vector<CMultiMenuUI*> WndsVector::receiverWnd;
HWND WndsVector::rootHwnd = NULL;
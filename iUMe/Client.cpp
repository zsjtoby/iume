#include "stdafx.h"
#include "Client.h"

#include "Skin.h"
#include "MultiMenu.h"
#include "ReadClient.h"
#include "..\common\file\IniFile.h"
#include "..\common\ado\AdoDB.h"
#include "Setup.h"
#include "About.h"
#include "Guide.h"
#include "..\common\winhttp\WinHttpApi.h"
#include "..\common\tinyxml\tinyxml.h"
#include "..\common\convert\kscconv.h"

#include "iUMe.h"

CClient::CClient()	
: m_nImage(-1)		
, m_nMaxBook(0)
, m_nCurrPage(0)
, m_bAppEdit(FALSE)
, m_bNoBook(FALSE)
{
}

CClient::~CClient()
{
	PostQuitMessage(0);
}

LPCTSTR CClient::GetWindowClassName() const   
{   
	return _T("CClient"); 
}

CDuiString CClient::GetSkinFile()
{   
	return _T("Client.xml");  
}

CDuiString CClient::GetSkinFolder()              
{   
	return _T("");  
}

void CClient::QuitIUME()
{
	SaveLocalUI();
	Close();
}

void CClient::OnFinalMessage(HWND hWnd)
{
	RemoveIcon();
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CClient::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

CControlUI* CClient::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("AppClient")) == 0)
	{
		return new CAppClient(m_PaintManager);
	}
	else if (_tcsicmp(pstrClass, _T("BookClient")) == 0)
	{
		return new CBookClient(m_PaintManager);
	}

	return NULL;
}

void CClient::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{	
		theApp.SetActiveWindow(m_hWnd);

		// 增加系统拖盘功能
		HICON hIconSmall = (HICON)::LoadImage(CPaintManagerUI::GetInstance(),
			MAKEINTRESOURCE(IDR_Client), 
			IMAGE_ICON,
			::GetSystemMetrics(SM_CXSMICON),
			::GetSystemMetrics(SM_CYSMICON),
			LR_DEFAULTCOLOR);

		InstallIcon(_T("iUMe"), hIconSmall, IDR_Client);
		SetTooltipText(_T("iUMe"));

		LoadIni();
		InitLocalUI();

		InitWebBrowser();		
		LoadHistoryBook();	
		LoadLocalApplication();
		LoadApplication();
	}
	else if (_tcsicmp(msg.sType, DUI_MSGTYPE_CLICK) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), MY_DUILIB_BTN_CLOSE) == 0)
		{
			if (g_stBaseCfg.uExit)
			{
				QuitIUME();
			}
			else
			{
				ShowWindow(SW_HIDE);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), MY_DUILIB_BTN_MAX) == 0)
		{
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);

			CButtonUI *btn = static_cast<CButtonUI*>(m_PaintManager.FindControl(MY_DUILIB_BTN_MAX));
			btn->SetVisible(false);

			btn = static_cast<CButtonUI*>(m_PaintManager.FindControl(MY_DUILIB_BTN_RESTORE));
			btn->SetVisible(true);	
		}
		else if (_tcsicmp(msg.pSender->GetName(), MY_DUILIB_BTN_MIN) == 0)
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if (_tcsicmp(msg.pSender->GetName(), MY_DUILIB_BTN_RESTORE) == 0)
		{
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
			CButtonUI *btn = static_cast<CButtonUI*>(m_PaintManager.FindControl(MY_DUILIB_BTN_MAX));
			btn->SetVisible(true);

			btn = static_cast<CButtonUI*>(m_PaintManager.FindControl(MY_DUILIB_BTN_RESTORE));
			btn->SetVisible(false);		
		}
		else if (_tcsicmp(msg.pSender->GetName(), MY_DUILIB_BTN_SKIN) == 0)
		{
			CDuiRect rcWindow;
			GetWindowRect(m_hWnd, &rcWindow);
			rcWindow.top = rcWindow.top + msg.pSender->GetPos().bottom;
			new CSkin(this, rcWindow);
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnmenu")) == 0)
		{
			CMultiMenuUI *dlg = new CMultiMenuUI;
			if (dlg == NULL)	return ;

#if 1		// 始终显示在控件下方并左对齐
			RECT rc = msg.pSender->GetPos();
			POINT pt = {rc.left, rc.bottom};
			::ClientToScreen(*this, &pt);
#else		// 始终显示在点击位置
			POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
			::ClientToScreen(*this, &pt);
#endif
			//POINT pt = 
			dlg->InitWnd(pt, m_hWnd);

			dlg->SetItemHeight(25);
			dlg->SetItemWidth(140);
			dlg->Add(L"窗口置顶", MY_MENU_ID_TOP, g_stBaseCfg.uTop?_T("menu\\check.bmp"):_T(""));
			dlg->AddLine();
			dlg->Add(L"系统设置", MY_MENU_ID_SETUP);
			dlg->Add(L"显示新特性", MY_MENU_ID_GUIDE);
			dlg->AddLine();
			CMultiMenuUI *dlg2 = dlg->Add(L"帮助", MY_MENU_ID_HELP, _T(""), 0xFF000000, 0, true);
			dlg2->Add(L"访问官方网站", MY_MENU_ID_HOME);
			dlg2->Add(L"意见反馈", MY_MENU_ID_SUGGEST);
			dlg2->Add(L"关于 iUMe", MY_MENU_ID_ABOUT);

			dlg->UpdateWnd();
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnhome")) == 0)
		{
			SwitchTabctrl(0);
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnbook")) == 0)
		{
			SwitchTabctrl(1);
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnapp")) == 0)
		{
			SwitchTabctrl(2);
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnclosetab")) == 0)
		{
			RemoveEbookPage(static_cast<CButtonUI*>(msg.pSender));
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnBookEx")) == 0)
		{
			CControlUI* pParent = msg.pSender->GetParent();
			BOOK_INFO *node = (BOOK_INFO*)pParent->GetTag();
			if (node != NULL)
			{
				int nIndex = IsOpenEbook(node);
				if (nIndex == -1)
				{
					AddEbookPage(node);
				}
				else
				{
					CTabLayoutUI *pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabbook")));
					pTab->SelectItem(nIndex);
					m_nCurrPage = nIndex;
					
					list<ST_CTRL_PT>::const_iterator it = m_list.begin();	
					for (int i = 0; i < nIndex - 1; ++i)	
						it++;
					it->pOption->Selected(true);
				}
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("tbOptHome")) == 0 || 
					_tcsicmp(msg.pSender->GetName(), _T("tbOption")) == 0)
		{
			CHorizontalLayoutUI *pContainer = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("IContainer")));			
			CHorizontalLayoutUI* pCtrl = static_cast<CHorizontalLayoutUI*>(msg.pSender->GetParent());
			CTabLayoutUI *pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabbook")));

			int nIndex = pContainer->GetItemIndex(pCtrl);

			pTab->SelectItem(nIndex);
			m_nCurrPage = nIndex;
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("lefttab")) == 0)
		{
			MovePrevPage();
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("righttab")) == 0)
		{
			MoveNextPage();
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnAddBook")) == 0)
		{
			CDuiString strPath;
			CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
				_T("文本文件(*.txt)|*.txt||"), AfxGetMainWnd());

			if (dlg.DoModal() == IDOK)
			{
				strPath = dlg.GetPathName();

				if (!IsSaveEbook(strPath))
				{
					// 保存并添加
					BOOK_INFO stBook;
					SaveEbook(strPath, &stBook);
					LoadCurrentEbook(&stBook);
				}
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnAddApp")) == 0)
		{
			CDuiString strPath;
			CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
				_T("应用程序(*.exe)|*.exe||"), AfxGetMainWnd());

			if (dlg.DoModal() == IDOK)
			{
				strPath = dlg.GetPathName();

				if (!IsSaveApp(strPath))
				{
					// 保存并添加
					APP_INFO stApp;
					SaveApp(strPath, &stApp);
					LoadCurrentApp(&stApp);
				}
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnDelBook")) == 0)
		{
			CBookClient* ctrl = static_cast<CBookClient*>(m_PaintManager.FindControl(_T("bookpage")));
			
			if (ctrl != NULL)
			{
				ctrl->InEdit();
				msg.pSender->SetText(ctrl->GetEdit()?_T("退出编辑"):_T("编辑书籍"));
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnDelApp")) == 0)
		{
			CAppClient* ctrl = static_cast<CAppClient*>(m_PaintManager.FindControl(_T("application")));

			if (ctrl != NULL)
			{
				ctrl->InEdit();
				msg.pSender->SetText(ctrl->GetEdit()?_T("退出编辑"):_T("编辑应用"));
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnbookdel")) == 0)
		{
			CBookClient* ctrl = static_cast<CBookClient*>(m_PaintManager.FindControl(_T("bookpage")));
			ASSERT(ctrl);
			CControlUI* pParent = msg.pSender->GetParent();
			ASSERT(pParent != NULL);
			BOOK_INFO *node = (BOOK_INFO*)pParent->GetTag();
			ASSERT(node != NULL);
			int nIndex = ctrl->GetItemIndex(pParent);

			DeleteEbookDB(node);
			ctrl->RemoveAt(nIndex);			// 移除书籍列表
			if (ctrl->GetCount() == 0)		// 无书籍列表时显示no book提示
			{
				m_bNoBook = FALSE;
				ShowNoEbook();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnappdel")) == 0)
		{
			CAppClient* ctrl = static_cast<CAppClient*>(m_PaintManager.FindControl(_T("application")));
			ASSERT(ctrl);
			CControlUI* pParent = msg.pSender->GetParent();	
			ASSERT(pParent != NULL);
			APP_INFO *node = (APP_INFO*)pParent->GetTag();
			ASSERT(node != NULL);
			int nIndex = ctrl->GetItemIndex(pParent);

			DeleteAppDB(node);
			ctrl->RemoveAt(nIndex);			
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnPrev")) == 0)
		{
			CReadClient *pClnt = (CReadClient *)msg.pSender->GetTag();
			ASSERT(pClnt != NULL);
			pClnt->MovePrev();
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnNext")) == 0)
		{
			CReadClient *pClnt = (CReadClient *)msg.pSender->GetTag();
			ASSERT(pClnt != NULL);
			pClnt->MoveNext();
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnsearch")) == 0)
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edtsearch")));
			ASSERT(pEdit != NULL);

			CDuiString strValue;
			strValue = pEdit->GetText();
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("btnAppEx")) == 0)
		{
			CControlUI* pParent = msg.pSender->GetParent();
			APP_INFO *node = (APP_INFO*)(pParent->GetTag());
			ASSERT(node != NULL);

			if (node->bIsLocal)
			{
				ShellExecute(NULL, NULL, node->strExe, NULL, m_strApp, SW_SHOW);
			}
			else
			{
				ShellExecute(NULL, NULL, node->strExe, NULL, NULL, SW_SHOW);
			}
			
		}
	}
	else if (_tcsicmp(msg.sType, DUI_MSGTYPE_VALUECHANGED) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), _T("percent")) == 0)
		{

		}
	}
}

//显示托盘菜单
void CClient::ShowTrayMenu(POINT pt)
{  
	CMultiMenuUI *dlg = new CMultiMenuUI;

	dlg->InitWnd(pt, m_hWnd);

	dlg->SetItemHeight(25);
	dlg->SetItemWidth(140);
	dlg->Add(L"打开主面板", 0);
	dlg->AddLine();
	dlg->Add(L"系统设置", MY_MENU_ID_SETUP);
	dlg->Add(L"在线升级");
	dlg->Add(L"关于 iUMe", MY_MENU_ID_ABOUT);		
	dlg->AddLine();
	dlg->Add(L"退出", MY_MENU_ID_QUIT);						
	dlg->UpdateWnd();		 
}

LRESULT CClient::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{ 
	if(uMsg == WM_TRAYICON)
		OnTrayIcon(uMsg,wParam,lParam,bHandled);

	return 0;
}

LRESULT CClient::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) 
	{
	case MY_MENU_ID_SETUP:
		{
			if (!CSetup::IsPopup())
			{
				CSetup *dlg = new CSetup(m_stSkin);
				if (dlg != NULL)
				{
					dlg->Create(NULL, _T("系统设置"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
					dlg->CenterWindow();
					dlg->ShowWindow(true);
					m_skinServer.AddReceiver(dlg);
				}
			}
		}
		break;
	case MY_MENU_ID_TOP:   
		g_stBaseCfg.uTop = !g_stBaseCfg.uTop;
		break;
	case MY_MENU_ID_GUIDE:
		{
			if (!CGuide::IsPopup())
			{
				CGuide *dlg = new CGuide();
				if (dlg != NULL)
				{
					dlg->Create(NULL, _T("新特性"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
					dlg->CenterWindow();
					dlg->ShowWindow(true);
				}
			}
		}
		break;
	case MY_MENU_ID_ABOUT:   
		{
			if (!CAbout::IsPopup())
			{
				CAbout *dlg = new CAbout();
				if (dlg != NULL)
				{
					dlg->Create(NULL, _T("关于 iUMe"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
					dlg->CenterWindow();
					dlg->ShowWindow(true);
				}
			}
		}
		break;
	case MY_MENU_ID_HOME:   
		ShellExecute(0, NULL, _T("www.baidu.com"), NULL, NULL, SW_SHOW);
		break;
	case MY_MENU_ID_SUGGEST:
		break;
	case MY_MENU_ID_QUIT:
		QuitIUME();
		break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

int CClient::DeleteEbookDB(BOOK_INFO *stBook)
{
	CAdoDB	ado;

	CString strSQL;
	strSQL.Format(_T("%s\\bin\\iUMe.mdb"), m_strApp);

	if (ado.AdoConnect(NULL, DEFAULT_DATEBASE_PASSWORD, NULL, (_bstr_t)strSQL))
	{
		_RecordsetPtr recset;
		strSQL.Format(_T("SELECT * FROM BookInfo WHERE path = '%s' "), stBook->strPath);
		try
		{
			if (ado.AdoOpen((_bstr_t)strSQL, recset))
			{
				recset->Delete(adAffectCurrent);
				recset->Update();
				recset->Close();
			}

			ado.AdoClose();
		}
		catch (...)
		{
			recset->Close();
			ado.AdoClose();
		}
	}

	return 0;
}

int CClient::DeleteAppDB(APP_INFO *stApp)
{
	CAdoDB	ado;

	CString strSQL;
	strSQL.Format(_T("%s\\bin\\iUMe.mdb"), m_strApp);

	if (ado.AdoConnect(NULL, DEFAULT_DATEBASE_PASSWORD, NULL, (_bstr_t)strSQL))
	{
		_RecordsetPtr recset;
		strSQL.Format(_T("SELECT * FROM AppInfo WHERE exe = '%s' "), stApp->strExe);
		try
		{
			if (ado.AdoOpen((_bstr_t)strSQL, recset))
			{
				recset->Delete(adAffectCurrent);
				recset->Update();
				recset->Close();
			}

			ado.AdoClose();
		}
		catch (...)
		{
			recset->Close();
			ado.AdoClose();
		}
	}

	return 0;
}

int CClient::IsOpenEbook(BOOK_INFO *stBook)
{
	int nIndex = -1;
	list<ST_CTRL_PT>::const_iterator it = m_list.begin();

	int i = 0;
	while (it != m_list.end())
	{
		i++;
		if (it->pBook == stBook)
		{
			nIndex = i;
			break;
		}

		it++;
	}

	return nIndex;
}

bool CClient::IsSaveEbook(CDuiString strEbook)
{
	bool bFlag = false;
	CAdoDB	ado;

	CString strSQL;
	strSQL.Format(_T("%s\\bin\\iUMe.mdb"), m_strApp);

	if (ado.AdoConnect(NULL, DEFAULT_DATEBASE_PASSWORD, NULL, (_bstr_t)strSQL))
	{
		_RecordsetPtr recset;
		strSQL.Format(_T("SELECT * FROM BookInfo WHERE path = '%s' "), strEbook);
		try
		{
			if (ado.AdoExecute((_bstr_t)strSQL, recset))
			{
				if (!recset->adoEOF)
				{
					bFlag = TRUE;
				}

				recset->Close();
			}

			ado.AdoClose();
		}
		catch (...)
		{
			recset->Close();
			ado.AdoClose();
		}
	}

	return bFlag;
}

bool CClient::IsSaveApp(CDuiString strApp)
{
	bool bFlag = false;
	CAdoDB	ado;

	CString strSQL;
	strSQL.Format(_T("%s\\bin\\iUMe.mdb"), m_strApp);

	if (ado.AdoConnect(NULL, DEFAULT_DATEBASE_PASSWORD, NULL, (_bstr_t)strSQL))
	{
		_RecordsetPtr recset;
		strSQL.Format(_T("SELECT * FROM AppInfo WHERE exe = '%s' "), strApp);
		try
		{
			if (ado.AdoExecute((_bstr_t)strSQL, recset))
			{
				if (!recset->adoEOF)
				{
					bFlag = TRUE;
				}

				recset->Close();
			}

			ado.AdoClose();
		}
		catch (...)
		{
			recset->Close();
			ado.AdoClose();
		}
	}

	return bFlag;
}

void CClient::SaveEbook(CDuiString strEbook, BOOK_INFO *stBook)
{
	int tmp1 = strEbook.ReverseFind('\\');
	int tmp2 = strEbook.ReverseFind('.');
	stBook->strName = strEbook.Mid(tmp1+1, tmp2-tmp1-1);
	stBook->strPath = strEbook;
	srand(time(0));					
	stBook->nImage = rand()%12;		// 共12张封面图片，随机生成封面图片索引
	stBook->nBookmark = 0;

	CAdoDB	ado;

	CString strSQL;
	strSQL.Format(_T("%s\\bin\\iUMe.mdb"), m_strApp);

	if (ado.AdoConnect(NULL, DEFAULT_DATEBASE_PASSWORD, NULL, (_bstr_t)strSQL))
	{
		_RecordsetPtr recset;
		strSQL = _T("select * from BookInfo");

		try
		{
			if (ado.AdoOpen((_bstr_t)strSQL, recset))
			{
				recset->AddNew();												// 添加新行

				recset->PutCollect((_bstr_t)_T("caption"), (_bstr_t)stBook->strName);
				recset->PutCollect((_bstr_t)_T("path"), (_bstr_t)stBook->strPath);
				recset->PutCollect((_bstr_t)_T("image"), stBook->nImage);
				recset->PutCollect((_bstr_t)_T("bookmark"), stBook->nBookmark);
		
				recset->Update();					
				recset->Close();
			}

			ado.AdoClose();
		}
		catch (...)
		{
			recset->Close();
			ado.AdoClose();
		}
	}
}

void CClient::SaveApp(CDuiString strApp, APP_INFO *stApp)
{
	stApp->strExe = strApp;				
	AnalysisApp(stApp);
	stApp->bIsHot = false;
	stApp->bIsLocal = false;
	stApp->bIsNew = false;

	CAdoDB	ado;
	CString strSQL;
	strSQL.Format(_T("%s\\bin\\iUMe.mdb"), m_strApp);

	if (ado.AdoConnect(NULL, DEFAULT_DATEBASE_PASSWORD, NULL, (_bstr_t)strSQL))
	{
		_RecordsetPtr recset;
		strSQL = _T("select * from AppInfo");

		try
		{
			if (ado.AdoOpen((_bstr_t)strSQL, recset))
			{
				recset->AddNew();												// 添加新行

				recset->PutCollect((_bstr_t)_T("caption"), (_bstr_t)stApp->strName);
				recset->PutCollect((_bstr_t)_T("exe"), (_bstr_t)stApp->strExe);
				recset->PutCollect((_bstr_t)_T("image"), stApp->nImage);

				recset->Update();					
				recset->Close();
			}

			ado.AdoClose();
		}
		catch (...)
		{
			recset->Close();
			ado.AdoClose();
		}
	}
}

void CClient::AnalysisApp(APP_INFO *stApp)
{
	int tmp1 = stApp->strExe.ReverseFind('\\');
	int tmp2 = stApp->strExe.ReverseFind('.');
	
	stApp->strName = stApp->strExe.Mid(tmp1+1, tmp2-tmp1-1);
	stApp->nImage = 255;

	USES_CONVERSION;
	char buffer[MAX_PATH] = {0};
	sprintf(buffer, "%s\\bin\\distinct.xml", W2A(m_strApp));

	// 开始解析
	TiXmlDocument xmlDoc;
	const TiXmlElement *pRoot = NULL;
	const TiXmlElement *pItem = NULL;

	if (xmlDoc.LoadFile(buffer, TIXML_ENCODING_UTF8))
	{
		pRoot = xmlDoc.FirstChildElement("appinfo");
		if (pRoot == NULL)	return ;

		pItem = pRoot->FirstChildElement(W2A(stApp->strName));
		if (pItem != NULL)
		{
			std::string strId, strText;

			strId = pItem->Attribute("id");
			strText = pItem->Attribute("text");
			std::wstring strTemp;

			if (!strId.empty())
			{
				strTemp = Utf8ToUnicode(strText);
			}

			stApp->nImage = atoi(strId.c_str());
			stApp->strName = strTemp.c_str();
		}
	}
}

void CClient::SaveLocalUI()
{
	CIniFile cfg;
	CString strFile;
	strFile.Format(_T("%s\\bin\\setup.ini"), m_strApp.GetData());
	cfg.SetPathName(strFile);

	cfg.WriteInt(_T("skin"), _T("skin"), m_nImage);
}

void CClient::InitLocalUI()
{
	CIniFile cfg;
	CString strFile;
	strFile.Format(_T("%s\\bin\\setup.ini"), m_strApp.GetData());
	cfg.SetPathName(strFile);

	int nImage = cfg.GetInt(_T("skin"), _T("skin"), 0);
	SetImage(nImage);
}

void CClient::SwitchTabctrl(int nIndex)
{
	CTabLayoutUI* pCtrl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabclient")));
	ASSERT(pCtrl != NULL);

	if (pCtrl->GetCurSel() != nIndex)
	{
		pCtrl->SelectItem(nIndex);
	}
}

void CClient::MovePrevPage()
{
	CTabLayoutUI *pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabbook")));
	if (pTab != NULL && m_nCurrPage > 0)
	{
		if (m_nCurrPage == 1)
		{
			COptionUI *pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("tbOptHome")));	
			if (pOpt != NULL)
			{
				pOpt->Selected(true);
			}
		}
		else
		{
			// 在链表中同步查找
			int pos = 0;
			list<ST_CTRL_PT>::iterator it = m_list.begin();
			while (it != m_list.end())
			{
				pos++;

				if (pos == m_nCurrPage - 1)
				{
					it->pOption->Selected(true);
					break;
				}

				++it;
			}
		}

		m_nCurrPage--;
		pTab->SelectItem(m_nCurrPage);
	}
}

void CClient::MoveNextPage()
{
	CTabLayoutUI *pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabbook")));
	if (pTab != NULL && m_nCurrPage < m_nMaxBook)
	{
		// 在链表中同步查找
		int pos = 0;
		list<ST_CTRL_PT>::iterator it = m_list.begin();
		while (it != m_list.end())
		{
			pos++;

			if (pos == m_nCurrPage + 1)
			{
				it->pOption->Selected(true);
				break;
			}

			++it;
		}

		m_nCurrPage++;
		pTab->SelectItem(m_nCurrPage);	
	}
}

void CClient::RemoveEbookPage(CButtonUI *pCtrl)
{
	CHorizontalLayoutUI *pContainer = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("IContainer")));
	CControlUI* pDelCtl = static_cast<CControlUI*>(pCtrl->GetParent());

	int nIndex = pContainer->GetItemIndex(pDelCtl);
	pContainer->Remove(pDelCtl);
	
	CTabLayoutUI *pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabbook")));
	pTab->RemoveAt(nIndex);
		
	m_nMaxBook--;

	bool bNeedUpdate = false;		
	if (m_nCurrPage >= nIndex)
	{
		bNeedUpdate = true;
		m_nCurrPage--;
	}

	pTab->SelectItem(m_nCurrPage);
	if (m_nCurrPage == 0)
	{
		COptionUI *pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("tbOptHome")));
		pOpt->Selected(true);
		bNeedUpdate = false;
	}

	// 在链表中同步删除
	list<ST_CTRL_PT>::iterator it = m_list.begin();
	nIndex = 0;
	bool bComplete = false;
	while (it != m_list.end())
	{	
		if (it->pButton == pCtrl)
		{
			it = m_list.erase(it);
			bComplete = true;

			if (!bNeedUpdate)		// 不需要更新或更新完成时退出
				break;
			else
				continue;
		}

		nIndex++;
		if (bNeedUpdate && (nIndex == m_nCurrPage))
		{
			bNeedUpdate = false;
			it->pOption->Selected(true);

			if (bComplete)			// 删除操作已完成时退出
				break;
		}	

		++it;
	}	
}

void CClient::AddEbookPage(BOOK_INFO *node)
{
	if (!IsExist(node->strPath))	
	{
		ShowMessageBox(&this->m_hWnd, _T("系统提示"), _T("找不到指定的书籍"), MB_OK);
		return ;
	}

	if (m_nMaxBook < emMaxBook)
	{
		CDuiString strHint;
		CHorizontalLayoutUI *pContainer = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("IContainer")));

		int nIndex = 0;
		// 创建标签
		if (pContainer == NULL)	return ;
		
		COptionUI* pNewOption = new COptionUI;
		strHint.Format(_T("name=\"tbOption\" text=\"%s\" float=\"false\" group=\"group1\" height=\"30\" maxwidth=\"174\" \
normalimage=\"file=\'tab\\tabbar_normal.png\' corner=\'5,5,5,5\'\" hotimage=\"file=\'tab\\tabbar_hover.png\' corner=\'5,5,5,5\'\" \
pushedimage=\"file=\'tab\\tabbar_pushed.png\' corner=\'5,5,5,5\'\" selectedimage=\"file=\'tab\\tabbar_pushed.png\' corner=\'5,5,5,5\'\" textalign=\"center\""), node->strName);

		pNewOption->ApplyAttributeList(strHint);

		CButtonUI* pCloseBtn = new CButtonUI;
		pCloseBtn->ApplyAttributeList(_T("name=\"btnclosetab\" float=\"true\" normalimage=\"UserTipClose_normal.png\" hotimage=\"UserTipClose_mouseover.png\" \
bordersize=\"0\" pos=\"-15,7,-8,13\" bordercolor=\"#FF000000\" width=\"7\" height=\"6\""));

		// 标签HorLayout = OptionUI + 关闭按钮
		CHorizontalLayoutUI* pNewHorLayout = new CHorizontalLayoutUI;
		pNewHorLayout->ApplyAttributeList(_T("maxwidth=\"174\" "));

		CHorizontalLayoutUI* pSpace = new CHorizontalLayoutUI;
		pSpace->ApplyAttributeList(_T("width=\"5\""));
		// 在标签HorLayout上增加 OptionUI 和 关闭按钮
		pNewHorLayout->Add(pSpace);
		pNewHorLayout->Add(pNewOption);
		pNewHorLayout->Add(pCloseBtn);

		nIndex = pContainer->GetCount();
		pContainer->AddAt(pNewHorLayout, nIndex);
		
		// tab页添加控件
		CTabLayoutUI *pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabbook")));
		if (pTab == NULL)	return ;

		CReadClient *pCont = new CReadClient(m_PaintManager, node);
		pTab->AddAt(pCont, nIndex);
		pTab->SelectItem(nIndex);
		pCont->JumpBookmark(node->nBookmark);	// 跳到书签首页
		pNewOption->Selected(true);
	
		m_nCurrPage = nIndex;	
		m_nMaxBook++;

		// 同步增加到链表
		ST_CTRL_PT stPtCtrl;
		stPtCtrl.pButton = pCloseBtn;
		stPtCtrl.pOption = pNewOption;
		stPtCtrl.pHorizontal = pNewHorLayout;
		stPtCtrl.pBook = node;

		m_list.push_back(stPtCtrl);
	}
	else
	{
		CDuiString strHint;
		strHint.Format(_T("最多支持同时打开 %d 本书籍"), emMaxBook);
		ShowMessageBox(&this->m_hWnd, _T("系统提示"), strHint, MB_OK);
	}
}

void CClient::LoadIni()
{
	CIniFile tmpIni;
	tmpIni.SetPathName(m_strApp + _T("\\bin\\setup.ini"));

	UINT8 uValue1 = tmpIni.GetUInt(_T("SETUP"), _T("base"), 0);
	memcpy(&g_stBaseCfg, &uValue1, 1);

	UINT32 uValue2 = tmpIni.GetUInt(_T("SETUP"), _T("read"), 0);
	memcpy(&g_stReadCfg, &uValue2, 4);	

	UINT8 uValue3 = tmpIni.GetUInt(_T("SETUP"), _T("advance"), 0);
	memcpy(&g_stAdvanceCfg, &uValue3, 1);	
}

void CClient::InitWebBrowser()
{
	CWebBrowserUI *pWeb = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("homepage")));
	if (pWeb != NULL)
	{
		pWeb->Navigate2(_T("http://www.baidu.com/"));
		pWeb->Release();
	}
#if 0
	static const int QUERY_TIMEOUT = 5*1000;
	static const LPCWSTR QUERY_SERVER_NAME = L"www.weather.com.cn";
	CString QUERY_OBJECT_NAME = L"/data/sk/101190101.html";

	WinHttpApi httpApi;
	if (!httpApi.Initialize())	return ;

	HINTERNET hSess = NULL;
	HINTERNET hConn = NULL;
	HINTERNET hHttp = NULL;

	do 
	{
		hSess = httpApi.OpenSession(L"User Agent");
		if(hSess == NULL)	break;

		httpApi.SetTimeouts(hSess, QUERY_TIMEOUT, QUERY_TIMEOUT, QUERY_TIMEOUT, QUERY_TIMEOUT);

		hConn = httpApi.Connect(hSess, QUERY_SERVER_NAME, INTERNET_DEFAULT_HTTP_PORT);
		if(hConn == NULL) break;

		hHttp = httpApi.OpenRequest(hConn, L"GET", QUERY_OBJECT_NAME, INTERNET_SCHEME_HTTP);
		if(hHttp == NULL) break;

		BOOL result = httpApi.SendRequest(hHttp, NULL, 0);
		if(!result) break;

		result = httpApi.EndRequest(hHttp);
		if(!result) break;

		// 查询状态
		DWORD cbReturn = 0;
		DWORD cbSize = sizeof(cbReturn);
		result = httpApi.QueryInfo(hHttp, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, reinterpret_cast<char*>(&cbReturn), &cbSize);
		if(!result || cbReturn == HTTP_STATUS_NOT_FOUND) 
			break;

		cbReturn = 0;
		cbSize = sizeof(cbReturn);

		char buffer[1024];
		result = httpApi.ReadData(hHttp, &buffer[0], 1024, &cbReturn);
		if(!result || cbReturn <= 0) break;

		// 在结尾加\r\n\0空字符
		buffer[cbReturn] = '\0';
		std::wstring strTemp = Utf8ToUnicode(buffer);

	}
	while (FALSE);

	if(hHttp != NULL)
		httpApi.CloseInternetHandle(hHttp);

	if(hConn != NULL)
		httpApi.CloseInternetHandle(hConn);

	if(hSess != NULL)
		httpApi.CloseInternetHandle(hSess);

	httpApi.Uninitialize();
#endif
}

void CClient::LoadLocalApplication()
{
	CAppClient* ctrl = static_cast<CAppClient*>(m_PaintManager.FindControl(_T("application")));
	ASSERT(ctrl != NULL);

	CIniFile tmpIni(m_strApp + _T("\\bin\\soft.ini"));
	vector<CString> vecSection;
	
	tmpIni.GetSectionNames(&vecSection);

	APP_INFO tmpApp;
	for (int i = 0; i < vecSection.size(); ++i)
	{
		tmpApp.strExe	= vecSection[i] + _T(".exe");
		tmpApp.strName	= tmpIni.GetString(vecSection[i], _T("name"));
		tmpApp.bIsHot	= tmpIni.GetUInt(vecSection[i], _T("hot"), 0);
		tmpApp.bIsNew	= tmpIni.GetUInt(vecSection[i], _T("new"), 0);
		tmpApp.nImage	= tmpIni.GetUInt(vecSection[i], _T("image"), 0);
		tmpApp.bIsLocal = true;

		ctrl->AddNode(&tmpApp);
	}
}

void CClient::LoadApplication()
{
	CAppClient* ctrl = static_cast<CAppClient*>(m_PaintManager.FindControl(_T("application")));

	if (ctrl != NULL)
	{
		CAdoDB	ado;

		CDuiString strSQL;
		strSQL.Format(_T("%s\\bin\\iUMe.mdb"), m_strApp);

		if (ado.AdoConnect(NULL, DEFAULT_DATEBASE_PASSWORD, NULL, (_bstr_t)strSQL))
		{
			_RecordsetPtr recset;
			strSQL = _T("select * from AppInfo");
			try
			{
				if (ado.AdoExecute((_bstr_t)strSQL, recset))
				{
					APP_INFO tmp;
					tmp.bIsNew	= false;
					tmp.bIsHot	= false;
					tmp.bIsLocal= false;

					while (!recset->adoEOF)
					{
						tmp.strName = (LPCTSTR)(_bstr_t)recset->GetCollect((_bstr_t)_T("caption"));
						tmp.strExe = (LPCTSTR)(_bstr_t)recset->GetCollect((_bstr_t)_T("exe"));
						tmp.nImage = recset->GetCollect((_bstr_t)_T("image"));

						ctrl->AddNode(&tmp);
						recset->MoveNext();
					}

					recset->Close();
				}

				ado.AdoClose();
			}
			catch (...)
			{
				recset->Close();
				ado.AdoClose();
			}
		}
	}
}

void CClient::LoadCurrentEbook(BOOK_INFO *stBook)
{
	CBookClient* ctrl = static_cast<CBookClient*>(m_PaintManager.FindControl(_T("bookpage")));
	ctrl->AddNode(stBook);

	if (!m_bNoBook)
	{
		m_bNoBook = TRUE;
		ShowNoEbook();	
	}
}

void CClient::LoadCurrentApp(APP_INFO *stApp)
{
	CAppClient* ctrl = static_cast<CAppClient*>(m_PaintManager.FindControl(_T("application")));
	ctrl->AddNode(stApp);
}

void CClient::ShowNoEbook()
{
	CHorizontalLayoutUI* hintCtrl = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("nobook")));
	CBookClient* ctrl = static_cast<CBookClient*>(m_PaintManager.FindControl(_T("bookpage")));
	ASSERT(hintCtrl != NULL && ctrl != NULL);		
	hintCtrl->SetVisible(!m_bNoBook);
	ctrl->SetVisible(m_bNoBook);
}

void CClient::LoadHistoryBook()
{
	CBookClient* ctrl = static_cast<CBookClient*>(m_PaintManager.FindControl(_T("bookpage")));
	bool bflag = false;

	if (ctrl != NULL)
	{
		CAdoDB	ado;

		CDuiString strSQL;
		strSQL.Format(_T("%s\\bin\\iUMe.mdb"), m_strApp);

		if (ado.AdoConnect(NULL, DEFAULT_DATEBASE_PASSWORD, NULL, (_bstr_t)strSQL))
		{
			_RecordsetPtr recset;
			strSQL = _T("select * from BookInfo");
			try
			{
				if (ado.AdoExecute((_bstr_t)strSQL, recset))
				{
					BOOK_INFO tmp;
					while (!recset->adoEOF)
					{
						m_bNoBook = TRUE;
						bflag = true;
						tmp.strName = (LPCTSTR)(_bstr_t)recset->GetCollect((_bstr_t)_T("caption"));
						tmp.strPath = (LPCTSTR)(_bstr_t)recset->GetCollect((_bstr_t)_T("path"));
						tmp.nImage = recset->GetCollect((_bstr_t)_T("image"));
						tmp.nBookmark = recset->GetCollect((_bstr_t)_T("bookmark"));

						ctrl->AddNode(&tmp);
						recset->MoveNext();
					}

					recset->Close();
				}

				ado.AdoClose();
			}
			catch (...)
			{
				recset->Close();
				ado.AdoClose();
			}
		}
	}

	ShowNoEbook();
}

LRESULT CClient::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SC_CLOSE)
	{
		bHandled = true;
		Close();
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(m_hWnd);

	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	
	if (wParam != SC_MINIMIZE && (::IsZoomed(m_hWnd) != bZoomed))
	//if ((wParam == SC_MAXIMIZE || wParam == SC_RESTORE) && (::IsZoomed(m_hWnd) != bZoomed))
	{
		CButtonUI *btnmax = static_cast<CButtonUI*>(m_PaintManager.FindControl(MY_DUILIB_BTN_MAX));
		CButtonUI *btnrestore = static_cast<CButtonUI*>(m_PaintManager.FindControl(MY_DUILIB_BTN_RESTORE));

		btnmax->SetVisible(bZoomed);
		btnrestore->SetVisible(!bZoomed);
	}

	return 0;
}

void CClient::SetImage(int nIndex)
{
	if (m_nImage == nIndex)	return ;
	
	m_nImage = nIndex;
	
	CControlUI* bg = m_PaintManager.FindControl(_T("bg"));
	if (bg != NULL)
	{
		TCHAR szBuf[MAX_PATH] = {0};
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='frame\\%02d.jpg' corner='600,200,1,1'"), nIndex);

		bg->SetBkImage(szBuf);
		bg->NeedUpdate();

		m_stSkin.strImage = szBuf;
		m_skinServer.Broadcast(m_stSkin);
	}
}

LRESULT CClient::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}
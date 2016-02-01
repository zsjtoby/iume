#include "StdAfx.h"
#include "ReadClient.h"

#include "..\common\ado\AdoDB.h"

#include <iostream> 
#include <string> 
#include <fstream> 
using namespace std; 

namespace DuiLib
{

CReadClient::CReadClient(CPaintManagerUI& paint_manager, BOOK_INFO *item)
: m_pRichedit(NULL)
, m_pSlider(NULL)
, m_nCurrent(0)
, m_nCount(0)
, m_bRun(false)
, m_bExit(false)
, m_nBookmark(0)
, m_stBook(item)
{
	CDialogBuilder dlgBuilder;
	// 必须传入paint_manager，否则该xml不能使用默认滚动条等
	CContainerUI* pReadClnt = static_cast<CContainerUI*>(dlgBuilder.Create(_T("iReader.xml"), (UINT)0, NULL, &paint_manager));

	m_pRichedit = static_cast<CRichEditUI*>(pReadClnt->FindSubControl(_T("editBook")));
	m_pSlider = static_cast<CSliderUI*>(pReadClnt->FindSubControl(_T("percent")));
	ASSERT(m_pSlider != NULL && m_pRichedit != NULL);

	if (pReadClnt != NULL) 
	{
		this->Add(pReadClnt);

		CButtonUI* btnPrev = static_cast<CButtonUI*>(pReadClnt->FindSubControl(_T("btnPrev")));
		CButtonUI* btnNext = static_cast<CButtonUI*>(pReadClnt->FindSubControl(_T("btnNext")));

		btnPrev->SetTag((UINT_PTR)this);
		btnNext->SetTag((UINT_PTR)this);

		HANDLE hHandle = CreateThread(NULL,0,LoadThread,this,0,NULL);
		CloseHandle(hHandle);
		
		Sleep(100);						// 加延时以便切换至线程运行，并确保文本读取一定预存量
		m_nCount = m_vecText.size();	// 临时赋值
	}
	else 
	{
		this->RemoveAll();
	}
}

CReadClient::~CReadClient(void)
{
	if (m_bRun)
	{
		m_bExit = true;
		Sleep(100);
	}
	SaveBookmark();
}

int CReadClient::LoadEbook(CDuiString strPath)
{
	if (strPath.IsEmpty())	return 0;

	// 暂仅直接阅读一种模式
	ifstream in_file(strPath.GetData());

	if (in_file.is_open())
	{
		string strValue;
		vector<string> vecValue;
		int i = 0;
		while (getline(in_file, strValue))  
		{
			if (m_bExit) break;
			i++;
			if (i == EBOOK_MAX_LINE)
			{
				i = 0;
				m_vecText.push_back(vecValue);
				m_nCount = m_vecText.size();		// 预存一页后即时更新
				vecValue.clear();
			}
			else
			{
				vecValue.push_back(strValue);
			}
		}

		if (i != 0)
		{
			m_vecText.push_back(vecValue);
			m_nCount = m_vecText.size();
			i++;
		}

		in_file.close();
	}

	return m_vecText.size();
}

int CReadClient::MoveNext()
{
	if (m_nCurrent < m_nCount - 1)
	{
		m_nCurrent++;
		MoveJump(m_nCurrent);

		return 0;
	}

	return -1;
}

int CReadClient::MovePrev()
{
	if (m_nCurrent > 0 || m_nCount != 0)
	{
		m_nCurrent--;
		MoveJump(m_nCurrent);

		return 0;
	}

	return -1;
}

DWORD WINAPI CReadClient::LoadThread(LPVOID lParam)
{	
	CReadClient* pClnt = (CReadClient*)lParam;
	pClnt->m_bRun = true;
	pClnt->LoadEbook(pClnt->m_stBook->strPath);
	pClnt->m_bRun = false;

	return 0;
}

int CReadClient::CalcPage(float fPercent)
{
	return 0;
}

float CReadClient::CalcPercent(int nPage)
{
	return 0.0;
}

int CReadClient::JumpBookmark(int nPage)		
{
	m_nBookmark = nPage;
	if (nPage < m_nCount - 1)
	{
		MoveJump(nPage);
	}
	else
	{
		while (nPage >= m_nCount && m_bRun)	// 尚未预存至书签页
		{
			Sleep(100);
		}

		if (nPage > m_nCount - 1)			// 预存结束，表示书签页已超出范围，此时直接跳转至第1页
		{
			MoveJump(0);	
		}
		else
		{
			MoveJump(nPage);
		}
	}

	return 0;
}

int CReadClient::MoveJump(int nPage)
{
	if (nPage > m_nCount - 1 || nPage < 0 || m_nCount == 0)	return -1;
	
	m_nCurrent = nPage;

	CDuiString strText;
	m_pRichedit->SetText(_T(""));

	USES_CONVERSION;
	for (int i = 0; i < m_vecText[m_nCurrent].size(); ++i)
	{
		strText = A2T(m_vecText[m_nCurrent][i].c_str());
		m_pRichedit->AppendText(strText);
		m_pRichedit->AppendText(_T("\n"));
	}

	return 0;
}

void CReadClient::SaveBookmark()
{
	if (m_nBookmark == m_nCurrent)	return ;

	CAdoDB	ado;
	CString strSQL;
	strSQL.Format(_T("%s\\bin\\iUMe.mdb"), m_strApp);

	if (ado.AdoConnect(NULL, DEFAULT_DATEBASE_PASSWORD, NULL, (_bstr_t)strSQL))
	{
		_RecordsetPtr recset;
		strSQL.Format(_T("select bookmark from BookInfo where path = '%s'"), m_stBook->strPath);

		try
		{
			if (ado.AdoOpen((_bstr_t)strSQL, recset))
			{
				recset->PutCollect((_bstr_t)_T("bookmark"), m_nCurrent);

				recset->Update();					
				recset->Close();
			}

			ado.AdoClose();
			m_stBook->nBookmark = m_nCurrent;
		}
		catch (...)
		{
			recset->Close();
			ado.AdoClose();
		}
	}
}

} //namespace DuiLib

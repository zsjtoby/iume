#include "StdAfx.h"
#include "AppClient.h"

namespace DuiLib
{

CAppClient::CAppClient(CPaintManagerUI& paint_manager)
: m_pm(paint_manager)
, m_bEdit(false)
{
	SetItemSize(CSize(130, 100));
}

CAppClient::~CAppClient()
{

}

bool CAppClient::AddNode(APP_INFO *item)
{
	if (item == NULL)	return false;

	APP_INFO *node	= new APP_INFO;
	node->strName	= item->strName;
	node->nImage	= item->nImage;
	node->bIsNew	= item->bIsNew;
	node->bIsHot	= item->bIsHot;
	node->strExe	= item->strExe;
	node->bIsLocal	= item->bIsLocal;

	CContainerUI* pDesk = NULL;
	if( !m_dlgBuilder.GetMarkup()->IsValid() )
	{
		pDesk = static_cast<CContainerUI*>(m_dlgBuilder.Create(_T("iAppEx.xml"), (UINT)0, NULL, &m_pm));
	}
	else
	{
		pDesk = static_cast<CContainerUI*>(m_dlgBuilder.Create((UINT)0, &m_pm));
	}

	if( pDesk != NULL )	
	{
		this->Add(pDesk);

		CButtonUI* btn = static_cast<CButtonUI*>(m_pm.FindSubControlByName(pDesk, _T("btnAppEx")));
		CLabelUI *text = static_cast<CLabelUI*>(m_pm.FindSubControlByName(pDesk, _T("labelAppEx")));
		CButtonUI *btnEdit = static_cast<CButtonUI*>(m_pm.FindSubControlByName(pDesk, _T("btnappdel")));
		ASSERT(btn != NULL && text != NULL && btnEdit != NULL);

		CDuiString strImage;
		strImage.Format(_T("app\\%02d.png"), node->nImage);

		btn->SetBkImage(strImage);
		if (node->bIsNew)				// 优先显示新应用
		{
			btn->SetText(_T("{i new.png}"));
		}
		else if (node->bIsHot)
		{
			btn->SetText(_T("{i hot.png}"));
		}
		
		btnEdit->SetVisible(node->bIsLocal?FALSE:m_bEdit);	
		
		text->SetText(node->strName);
		pDesk->SetTag((UINT_PTR)node);
	}
	else
	{
		delete node;
		node = NULL;

		return false;
	}

	return true;
}

void CAppClient::InEdit()
{
	m_bEdit = !m_bEdit;
	int num = this->GetCount();
	CContainerUI* pCtrl = NULL;

	APP_INFO *stApp = NULL;
	for (int i = 0; i < num; ++i)
	{
		stApp = NULL;
		pCtrl = static_cast<CContainerUI*>(this->GetItemAt(i));
		stApp = (APP_INFO *)(pCtrl->GetTag());
		pCtrl = static_cast<CContainerUI*>(pCtrl->FindSubControl(_T("btnappdel")));
		ASSERT(pCtrl != NULL && stApp != NULL);	
		if (!stApp->bIsLocal)
			pCtrl->SetVisible(m_bEdit);	// 本地应用不可编辑
	}
}

bool CAppClient::GetEdit() const
{
	return m_bEdit;
}

bool CAppClient::RemoveAt(int nIndex)
{
	CControlUI* pCtrl = this->GetItemAt(nIndex);
	APP_INFO *node = (APP_INFO*)pCtrl->GetTag();
	ASSERT(node != NULL);

	delete node;

	return CTileLayoutUI::RemoveAt(nIndex);
}

}	// namespace DuiLib
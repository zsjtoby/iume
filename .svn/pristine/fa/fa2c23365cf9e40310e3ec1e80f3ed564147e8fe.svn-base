#include "StdAfx.h"
#include "BookClient.h"

namespace DuiLib
{

CBookClient::CBookClient(CPaintManagerUI& paint_manager)
: m_pm(paint_manager)
, m_bEdit(false)
{
	SetItemSize(CSize(130, 100));
}

CBookClient::~CBookClient(void)
{

}

bool CBookClient::AddNode(BOOK_INFO *item)
{
	if (item == NULL)	return false;

	BOOK_INFO *node	= new BOOK_INFO;
	node->strName	= item->strName;
	node->nImage	= item->nImage;
	node->strPath	= item->strPath;
	node->nBookmark	= item->nBookmark;

	CContainerUI* pDesk = NULL;
	if( !m_dlgBuilder.GetMarkup()->IsValid() )
	{
		pDesk = static_cast<CContainerUI*>(m_dlgBuilder.Create(_T("iBook.xml"), (UINT)0, NULL, &m_pm));
	}
	else
	{
		pDesk = static_cast<CContainerUI*>(m_dlgBuilder.Create((UINT)0, &m_pm));
	}

	if( pDesk != NULL )	
	{
		this->Add(pDesk);

		CButtonUI* btn = static_cast<CButtonUI*>(m_pm.FindSubControlByName(pDesk, _T("btnBookEx")));
		CLabelUI *text = static_cast<CLabelUI*>(m_pm.FindSubControlByName(pDesk, _T("labelBookEx")));
		CButtonUI *btnEdit = static_cast<CButtonUI*>(m_pm.FindSubControlByName(pDesk, _T("btnbookdel")));
		if (btn == NULL || text == NULL || btnEdit == NULL)	return false;

		CDuiString strImage;
		strImage.Format(_T("book\\%02d.png"), node->nImage);
		btn->SetBkImage(strImage);
		text->SetText(node->strName);
		btnEdit->SetVisible(m_bEdit);

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

void CBookClient::InEdit()
{
	m_bEdit = !m_bEdit;
	int num = this->GetCount();
	CContainerUI* pCtrl = NULL;

	for (int i = 0; i < num; ++i)
	{
		pCtrl = static_cast<CContainerUI*>(this->GetItemAt(i));
		pCtrl = static_cast<CContainerUI*>(pCtrl->FindSubControl(_T("btnbookdel")));
		ASSERT(pCtrl != NULL);
		pCtrl->SetVisible(m_bEdit);
	}
}

bool CBookClient::GetEdit() const
{
	return m_bEdit;
}

bool CBookClient::RemoveAt(int nIndex)
{
	CControlUI* pCtrl = this->GetItemAt(nIndex);
	BOOK_INFO *node = (BOOK_INFO*)pCtrl->GetTag();
	ASSERT(node != NULL);

	delete node;

	return CTileLayoutUI::RemoveAt(nIndex);
}

} //namespace DuiLib

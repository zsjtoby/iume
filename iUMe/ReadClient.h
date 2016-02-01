#pragma once

#ifndef __READ_CLIENT_H__
#define __READ_CLIENT_H__

#define EBOOK_MAX_LINE	50				// 每页行数

namespace DuiLib
{

class CReadClient : public CVerticalLayoutUI
{
public:
	CReadClient(CPaintManagerUI& paint_manager, BOOK_INFO *item);
	virtual ~CReadClient(void);

public:
	int MoveNext();							// 往下翻页。返回-1表示已经是最后一页
	int MovePrev();							// 上前翻页。返回-1表示已经是第一页
	int JumpBookmark(int nPage);			// 跳转至书签	

private:
	int CalcPage(float fPercent);			// 根据百分比计算页数
	float CalcPercent(int nPage);			// 根据页数计算百分比
	static DWORD WINAPI LoadThread(LPVOID lParam);	// 加载文本内容线程
	int MoveJump(int nPage);				// 跳转至某页

private:
	int m_nBookmark;
	BOOK_INFO *m_stBook;

protected:
	int LoadEbook(CDuiString strPath);		// 返回总页数
	void SaveBookmark();					// 保存书签
	vector<vector<string> > m_vecText;		// 书籍内容
	int m_nCount;							// 总页数
	int m_nCurrent;							// 当前页数
	CRichEditUI *m_pRichedit;
	CSliderUI	*m_pSlider;	

	bool m_bRun;				// 线程运行标志
	bool m_bExit;				// 线程退出标志
};

}	// namespace DuiLib

#endif	// #ifndef __READ_CLIENT_H__
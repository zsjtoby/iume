#pragma once

#ifndef __APP_CLIENT_H__
#define __APP_CLIENT_H__

namespace DuiLib
{

typedef struct tag_AppInfo
{
	CDuiString strName;			// 应用名称
	CDuiString strExe;			// Exe名称(含.exe)
	int nImage;					// 应用图标索引
	bool bIsLocal;				// 是否本地应用（自带应用视为本地应用，用户添加应用视为第三方应用）
	bool bIsNew;				// 是否新应用（本地应用属性）
	bool bIsHot;				// 是否热门应用（本地应用属性）
}APP_INFO;

class CAppClient : public CTileLayoutUI
{
public:
	CAppClient(CPaintManagerUI& paint_manager);
	virtual ~CAppClient(void);

public:
	bool AddNode(APP_INFO *item);
	void InEdit();
	bool RemoveAt(int nIndex);
	bool GetEdit() const;

private:
	CPaintManagerUI& m_pm;
	CDialogBuilder m_dlgBuilder;
	bool m_bEdit;
};

}	// namespace DuiLib

#endif
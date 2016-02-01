
/********************************************************************
** Copyright (c) 2012,深圳国人通信射频研发事业部 All rights reserved. 
** 摘    要：主要封装对数据库操作的函数
** 作    者：fansujun
** 创建日期：20121001
** 备    注：首先需要初始化
*********************************************************************/

#pragma once

#ifndef __ADO_DB_H__
#define __ADO_DB_H__

// 添加ADO的链接库
#import "c:\Program Files\Common Files\system\ado\msadox.dll"				// 用于创建Access数据库
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")rename("BOF","adoBOF")

typedef struct _stDatabaseTableStruct
{
	LPCTSTR lpTbName;
	int nType;
	int nLen;
}DB_TB_STRUCT;

class CAdoDB  
{
public:
	CAdoDB();
	virtual ~CAdoDB(void);

public:	
	bool AdoConnect(const char *user, const char *pwd, const char *srvip, const char *dbname);
	bool AdoOpen(const char *sql, _RecordsetPtr &recPtr);
	bool AdoExecute(const char *sql, _RecordsetPtr &recPtr, long lOptions = adCmdText);
	void AdoClose();
	bool IsConnect() const;
	bool AddTable(const char *tablename, DB_TB_STRUCT stTbStruct[], int nCount);	
	bool AddDatabase(const char *dbname);	// 创建Access数据库时，dbname必须为数据库文件绝对路径
	bool AddAccessPwd(const char *password, const char *mdb);	// 给无密码Access数据库文件添加访问密码

protected:
	_ConnectionPtr	m_pConnect;															// 数据库连接对象
	_RecordsetPtr	m_pRecordSet;														// 数据库记录集对象
	_CommandPtr		m_pCommand;															// 命令对象

public:

private:
	bool m_bConnect;
	char m_user[100];
	char m_password[100];
	char m_databasesrc[MAX_PATH];
};


#endif	// __ADO_DB_H__
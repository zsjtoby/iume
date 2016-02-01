#include "stdafx.h"
#include "AdoDB.h"

CAdoDB::CAdoDB()
:m_bConnect(false)
{
	::CoInitialize(NULL);	// 初始化COM环境

	m_pConnect.CreateInstance(__uuidof(Connection));			
	m_pRecordSet.CreateInstance(__uuidof(Recordset));
	m_pCommand.CreateInstance(__uuidof(Command));

	ZeroMemory(m_user, sizeof(m_user));
	ZeroMemory(m_password, sizeof(m_password));
	ZeroMemory(m_databasesrc, sizeof(m_databasesrc));
}


CAdoDB::~CAdoDB()
{
	if (m_pConnect != NULL)
	{
		m_pConnect = NULL;
	}
	if (m_pCommand != NULL)
	{
		m_pCommand = NULL;
	}
	::CoUninitialize();		//	释放COM库
	m_bConnect = false;
}

bool CAdoDB::AdoConnect(const char *user, 
							const char *pwd, 
							const char *srvip,
							const char *dbname)
{
	if (m_bConnect || dbname == NULL)	return false;

	char cSQL[255] = {0};

	if (pwd == NULL)	// 访问未加密Access数据库
	{		
		sprintf(cSQL, "Provider=Microsoft.JET.OLEDB.4.0;Data source = %s", dbname);
	}
	else				// 访问加密Access数据库
	{		
		sprintf(cSQL, "DRIVER={Microsoft Access Driver (*.mdb)};uid=Admin;pwd=%s;DBQ=%s;",pwd, dbname);
	}	

	try
	{
		m_pConnect->Open(cSQL, "", "", NULL);
		m_bConnect = true;
		strcpy(m_databasesrc, dbname);
	}
	catch (_com_error &e)
	{
		return false;
	}

	return true;
}

bool CAdoDB::IsConnect() const
{
	return m_bConnect;
}

bool CAdoDB::AdoOpen(const char *sql, _RecordsetPtr &recPtr)
{
	if (sql == NULL)	return false;

	try
	{
		m_pRecordSet->Open(sql, m_pConnect.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		recPtr = m_pRecordSet;
	}
	catch(_com_error &e)										
	{
		return false;
	}

	return true;
}

bool CAdoDB::AdoExecute(const char *sql, _RecordsetPtr &recPtr, long lOptions/* = adCmdText*/)
{
	if (sql == NULL)	return false;

	try
	{
		// 用Connection和Command对象执行Execute方法得到的记录集为只读
		recPtr = m_pConnect->Execute(sql, NULL, lOptions);
	}
	catch (_com_error e)
	{
		//printf("Execute发生异常.Error: %s; 文件: %s; 行: %d/n", e.ErrorMessage(), __FILE__, __LINE__);
		return false;
	}

	return true;
}

void CAdoDB::AdoClose()
{
	try
	{
		m_pRecordSet->Close();
		m_pConnect->Close();
		m_bConnect = false;
	}
	catch (_com_error &e)
	{
		// 
	}	
}

bool CAdoDB::AddTable(const char *tablename, DB_TB_STRUCT stTbStruct[], int nCount)
{
	if (!m_bConnect || tablename == NULL)	return false;

	HRESULT   hr = S_OK; 

	ADOX::_CatalogPtr	pCatalog = NULL; 
	ADOX::_TablePtr		pTable = NULL; 

	try 
	{ 
		hr = pCatalog.CreateInstance(__uuidof(ADOX::Catalog));
		if (FAILED(hr))	return false;

		char sSQL[255] = {0};
		if (m_password == NULL)
		{
			sprintf(sSQL, "Provider=Microsoft.Jet.OLEDB.4.0;data source=%s;", m_databasesrc);
		}
		else
		{
			sprintf(sSQL, "Provider=Microsoft.Jet.OLEDB.4.0;data source=%s;Persist Security Info=True;Jet OLEDB:Database Password=%s", 
				m_databasesrc, m_password);
		}

		pCatalog->PutActiveConnection(sSQL); 
		hr = pTable.CreateInstance(__uuidof(ADOX::Table));

		pTable->PutName(tablename); 
		for (int i = 0; i < nCount; i++)
		{
			// 数据类型为adInteger, adVarWChar等等
			pTable->Columns->Append((_bstr_t)stTbStruct[i].lpTbName, 
				ADOX::DataTypeEnum(stTbStruct[i].nType), stTbStruct[i].nLen); 
		}

		pCatalog->Tables->Append(_variant_t((IDispatch*)pTable)); 
		pCatalog->Tables->Refresh();
	}
	catch (_com_error &e)
	{
		return false;
	}

	return true;
}

bool CAdoDB::AddDatabase(const char *dbname)
{
	if (dbname == NULL)	return false;

	HRESULT hr = S_OK;
	char sSQL[100] = {0};
	sprintf(sSQL, "Provider=Microsoft.JET.OLEDB.4.0;Data source = %s", dbname);

	ADOX::_CatalogPtr pCatalog = NULL;
	hr = pCatalog.CreateInstance(__uuidof(ADOX::Catalog));

	try
	{
		FAILED(hr)?_com_issue_error(hr):pCatalog->Create(_bstr_t(sSQL));
	}  
	catch(_com_error &e)
	{
		return false;
	}

	return true;
}

bool CAdoDB::AddAccessPwd(const char *password, const char *mdb)
{
	if (password == NULL)	return false;

	try
	{
		HRESULT hr = S_OK;

		char sSQL[255] = {0};
		sprintf(sSQL, "Provider=Microsoft.Jet.OLEDB.4.0;User ID=admin;Data Source=%s;Mode=Share Deny Read|Share Deny Write", mdb);
		hr = m_pConnect->Open(sSQL, "","",0);

		ZeroMemory(sSQL, sizeof(sSQL));
		sprintf(sSQL, "ALTER DATABASE PASSWORD [%s] NULL", password);
		m_pConnect->Execute(sSQL, NULL, 0/*adCmdText*/); 
		m_pConnect->Close();
	}
	catch (_com_error &e)
	{
		return false;
	}

	return true;
}
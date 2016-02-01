#ifndef __MY_DEFINE_H__
#define __MY_DEFINE_H__

// 数据库访问部分
#define DEFAULT_DATEBASE_PASSWORD	"1314"	

// 公共资源名称部分
#define MY_DUILIB_BTN_CLOSE		_T("btnclose")
#define MY_DUILIB_BTN_MAX		_T("btnmax")
#define MY_DUILIB_BTN_RESTORE	_T("btnrestore")
#define MY_DUILIB_BTN_MIN		_T("btnmin")
#define MY_DUILIB_BTN_SKIN		_T("btnskin")
#define MY_DUILIB_BTN_SURE		_T("btnsure")
#define MY_DUILIB_BTN_CANCEL	_T("btncancel")

#define DUILIB_WARNING_MSG	0x00000001		// 告警
#define DUILIB_HINT_MSG		0x00000002		// 提示
#define DUILIB_ERROR_MSG	0x00000003		// 出错

#define MY_DUILIB_MENU_ID	0x10010		// 菜单id基准
#define MY_MENU_ID_TOP		MY_DUILIB_MENU_ID + 0		
#define MY_MENU_ID_SETUP	MY_DUILIB_MENU_ID + 1
#define MY_MENU_ID_GUIDE	MY_DUILIB_MENU_ID + 3
#define MY_MENU_ID_HELP		MY_DUILIB_MENU_ID + 4
#define MY_MENU_ID_HOME		MY_DUILIB_MENU_ID + 5
#define MY_MENU_ID_SUGGEST	MY_DUILIB_MENU_ID + 6
#define MY_MENU_ID_ABOUT	MY_DUILIB_MENU_ID + 7	

#define MY_MENU_ID_QUIT		MY_DUILIB_MENU_ID + 100


#endif __MY_DEFINE_H__
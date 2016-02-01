#ifndef __PUB_UNIT_H__
#define __PUB_UNIT_H__

#include "..\\.\DuiLib\observer_impl_base.h"

typedef struct tag_BookInfo
{
	CDuiString strName;			// 书籍名称
	int nImage;					// 封面索引
	CDuiString strPath;			// 书籍路径
	int nBookmark;				// 书签，用页码标识
}BOOK_INFO;

typedef struct stSkinInfo
{
	DWORD bkColor;
	CDuiString strImage;
}ST_SKIN;

typedef class ObserverImpl<BOOL, ST_SKIN> SkinChangedObserver;
typedef class ReceiverImpl<BOOL, ST_SKIN> SkinChangedReceiver;

typedef struct st_BaseCfg
{
	UINT8 uAuto		: 1;	// 自启动
	UINT8 uShortcut	: 1;	// 桌面快捷方式
	UINT8 uUserProj	: 1;	// 用户体验计划
	UINT8 uTop		: 1;	// 窗口置顶
	UINT8 uExit		: 1;	// 关闭主面板时关闭程序
	UINT8 uReserve	: 3;	// 保留值
}ST_BASE_CFG;

typedef struct st_ReadCfg
{
	UINT32 uSound		: 1;	// 语音播报开关
	UINT32 uSpeed		: 2;	// 语速	
	UINT32 uVoice		: 2;	// 语音类型
	UINT32 uVolume		: 3;	// 音量

	UINT32 uAuto		: 1;	// 自动翻页
	UINT32 uTimer		: 4;	// 自动翻页时长
	UINT32 uReserve1	: 3;	// 保留值1

	UINT32 uFont		: 3;	// 字体类型
	UINT32 uBold		: 1;	// 粗体
	UINT32 uItalic		: 1;	// 斜体
	UINT32 uSize		: 4;	// 大小
	UINT32 uImage		: 4;	// 背景图片索引
	UINT32 uReserve2	: 3;	// 保留值2
}ST_READ_CFG;

typedef struct st_AdvanceCfg
{
	UINT8 uUpgrade	: 2;	// 自动更新类型 0 自动，1 提醒更新，2 关闭更新
	UINT8 uEngine	: 3;	// 搜索引擎类型 0 谷歌，1 百度，2 必应 3，待增加
	UINT8 uReserve	: 3;	// 保留值
}ST_ADVANCE_CFG;

extern ST_BASE_CFG g_stBaseCfg;
extern ST_READ_CFG g_stReadCfg;
extern ST_ADVANCE_CFG g_stAdvanceCfg;
extern CDuiString m_strApp;

CDuiString GetCurrentTimeString();
CDuiString GetApp();
BOOL IsExist(LPCTSTR strPath);
int ShowMessageBox(HWND* hWnd, LPCTSTR lpHint, LPCTSTR lpValue, int nShowId, int nImageId = 0);
BOOL IsWindowRunBackGround(HWND hWnd);
void SetWindowRunBackGround(HWND hWnd, BOOL bBackground);

#endif // ifndef __PUB_UNIT_H__
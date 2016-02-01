#ifndef __KALENDAR_H__
#define __KALENDAR_H__

enum emConstKalendar { emCK_MONTHS = 12 };

class CMyKalendar
{
public:
	CMyKalendar();
	~CMyKalendar();

public:
	bool IsLeapYear(int nYear);
	int GetYearTotalDays(int nYear);				// 某年总共天数
	int GetMonthTotalDays(int nYear, int nMonth);	// 某年某月总共天数	
	// 蔡勒(Zeller)公式，返回0~6，分别表示周日~周六；适用范围 1582年10月15日 之后
	int Zeller(int nYear, int nMonth, int nDay);	
	// 基姆拉尔森(Kim larsson)公式 功能同上；
	int Kimlarsson(int nYear, int nMonth, int nDay);
	// 计算过去的天数（含当天）
	int CalcYearPassedDays(int nYear, int nMonth, int nDay);
	// 计算一年中还剩下的天数（不含当天）
	int CalcYearRestDays(int nYear, int nMonth, int nDay);

public:


private:

};


extern const WORD START_YEAR;
extern const WORD END_YEAR;

class CKalendar
{
private:
	WORD    m_iYear, m_iMonth, m_iDay;

public:
	CKalendar(WORD iYear, WORD iMonth, WORD iDay);
	CKalendar();

public:
	//判断iYear是不是闰年
    static BOOL IsLeapYear(WORD iYear)
	{return !(iYear%4)&&(iYear%100) || !(iYear%400);}

	//计算iYear,iMonth,iDay对应是星期几 1年1月1日 --- 65535年12月31日
	static WORD WeekDay(WORD iYear, WORD iMonth, WORD iDay);

	//返回iYear年iMonth月的天数 1年1月 --- 65535年12月
	static WORD MonthDays(WORD iYear, WORD iMonth);

	//返回阴历iLunarYer年阴历iLunarMonth月的天数，如果iLunarMonth为闰月，
	//高字为第二个iLunarMonth月的天数，否则高字为0 
	// 1901年1月---2050年12月
	static LONG LunarMonthDays(WORD iLunarYear, WORD iLunarMonth);

	//返回阴历iLunarYear年的总天数
	// 1901年1月---2050年12月
	static WORD LunarYearDays(WORD iLunarYear);

	//返回阴历iLunarYear年的闰月月份，如没有返回0
	// 1901年1月---2050年12月
	static WORD GetLeapMonth(WORD iLunarYear);

	//把iYear年格式化成天干记年法表示的字符串
	static void FormatLunarYear(WORD  iYear, char *pBuffer);

	//把iMonth格式化成中文字符串
	static void FormatMonth(WORD iMonth, char *pBuffer, BOOL bLunar = TRUE);

    //把iDay格式化成中文字符串
	static void FormatLunarDay(WORD  iDay, char *pBuffer);

	//计算公历两个日期间相差的天数  1年1月1日 --- 65535年12月31日
	static LONG CalcDateDiff(WORD iEndYear, WORD iEndMonth, WORD iEndDay,
		                     WORD iStartYear = START_YEAR, 
							 WORD iStartMonth =1, WORD iStartDay =1);

	//计算公历iYear年iMonth月iDay日对应的阴历日期,返回对应的阴历节气 0-24
	//1901年1月1日---2050年12月31日
	static WORD GetLunarDate(WORD iYear, WORD iMonth, WORD iDay,
		                     WORD &iLunarYear, WORD &iLunarMonth, WORD &iLunarDay);

public:
	virtual ~CKalendar();

public:
	//计算从1901年1月1日过iSpanDays天后的阴历日期
	static void   l_CalcLunarDate(WORD &iYear, WORD &iMonth ,WORD &iDay, LONG iSpanDays);
    //计算公历iYear年iMonth月iDay日对应的节气 0-24，0表不是节气
	static WORD   l_GetLunarHolDay(WORD iYear, WORD iMonth, WORD iDay);

};


#endif // ifndef __KALENDAR_H__
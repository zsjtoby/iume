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
	int GetYearTotalDays(int nYear);				// ĳ���ܹ�����
	int GetMonthTotalDays(int nYear, int nMonth);	// ĳ��ĳ���ܹ�����	
	// ����(Zeller)��ʽ������0~6���ֱ��ʾ����~���������÷�Χ 1582��10��15�� ֮��
	int Zeller(int nYear, int nMonth, int nDay);	
	// ��ķ����ɭ(Kim larsson)��ʽ ����ͬ�ϣ�
	int Kimlarsson(int nYear, int nMonth, int nDay);
	// �����ȥ�������������죩
	int CalcYearPassedDays(int nYear, int nMonth, int nDay);
	// ����һ���л�ʣ�µ��������������죩
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
	//�ж�iYear�ǲ�������
    static BOOL IsLeapYear(WORD iYear)
	{return !(iYear%4)&&(iYear%100) || !(iYear%400);}

	//����iYear,iMonth,iDay��Ӧ�����ڼ� 1��1��1�� --- 65535��12��31��
	static WORD WeekDay(WORD iYear, WORD iMonth, WORD iDay);

	//����iYear��iMonth�µ����� 1��1�� --- 65535��12��
	static WORD MonthDays(WORD iYear, WORD iMonth);

	//��������iLunarYer������iLunarMonth�µ����������iLunarMonthΪ���£�
	//����Ϊ�ڶ���iLunarMonth�µ��������������Ϊ0 
	// 1901��1��---2050��12��
	static LONG LunarMonthDays(WORD iLunarYear, WORD iLunarMonth);

	//��������iLunarYear���������
	// 1901��1��---2050��12��
	static WORD LunarYearDays(WORD iLunarYear);

	//��������iLunarYear��������·ݣ���û�з���0
	// 1901��1��---2050��12��
	static WORD GetLeapMonth(WORD iLunarYear);

	//��iYear���ʽ������ɼ��귨��ʾ���ַ���
	static void FormatLunarYear(WORD  iYear, char *pBuffer);

	//��iMonth��ʽ���������ַ���
	static void FormatMonth(WORD iMonth, char *pBuffer, BOOL bLunar = TRUE);

    //��iDay��ʽ���������ַ���
	static void FormatLunarDay(WORD  iDay, char *pBuffer);

	//���㹫���������ڼ���������  1��1��1�� --- 65535��12��31��
	static LONG CalcDateDiff(WORD iEndYear, WORD iEndMonth, WORD iEndDay,
		                     WORD iStartYear = START_YEAR, 
							 WORD iStartMonth =1, WORD iStartDay =1);

	//���㹫��iYear��iMonth��iDay�ն�Ӧ����������,���ض�Ӧ���������� 0-24
	//1901��1��1��---2050��12��31��
	static WORD GetLunarDate(WORD iYear, WORD iMonth, WORD iDay,
		                     WORD &iLunarYear, WORD &iLunarMonth, WORD &iLunarDay);

public:
	virtual ~CKalendar();

public:
	//�����1901��1��1�չ�iSpanDays������������
	static void   l_CalcLunarDate(WORD &iYear, WORD &iMonth ,WORD &iDay, LONG iSpanDays);
    //���㹫��iYear��iMonth��iDay�ն�Ӧ�Ľ��� 0-24��0�����ǽ���
	static WORD   l_GetLunarHolDay(WORD iYear, WORD iMonth, WORD iDay);

};


#endif // ifndef __KALENDAR_H__
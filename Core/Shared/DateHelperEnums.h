// DateHelper.h: interface for the CDateHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATEHELPERENUMS_H__2A4E63F6_A106_4295_BCBA_06D03CD67AE7__INCLUDED_)
#define AFX_DATEHELPERENUMS_H__2A4E63F6_A106_4295_BCBA_06D03CD67AE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

enum DH_DATE
{
	DHD_BEGINTHISWEEK, // DHD_ENDTHISWEEK - 6
	DHD_TODAY,
	DHD_TOMORROW,
	DHD_ENDTHISWEEK,   
	DHD_ENDNEXTWEEK,   // DHD_ENDTHISWEEK + 7
	DHD_ENDTHISMONTH,  // beginning of next month - 1
	DHD_ENDNEXTMONTH,  // gets trickier :)
	DHD_ENDTHISYEAR,
	DHD_ENDNEXTYEAR,
	DHD_YESTERDAY,
	DHD_NOW,
	DHD_BEGINTHISMONTH, 
	DHD_BEGINTHISYEAR, 
};

enum DH_UNITS
{
	DHU_NULL		= 0,	// error code
	DHU_WEEKDAYS	= 'd',
	DHU_DAYS		= 'D',
	DHU_WEEKS		= 'W',
	DHU_MONTHS		= 'M',
	DHU_YEARS		= 'Y',
};

enum
{
	DHFD_ISO		= 0x0001,
	DHFD_DOW		= 0x0002,
	DHFD_TIME		= 0x0004,
	DHFD_NOSEC		= 0x0008,
	DHFD_NOYEAR		= 0x0010,
	DHFD_NODAY		= 0x0020,
	DHFD_NOCENTURY	= 0x0040,
	DHFD_HASTIME	= 0x0080, // Applies to DHFD_TIME
};

enum DH_DAYOFWEEK // Days of week bit flags
{
	DHW_NONE		= 0X00,

	DHW_SUNDAY		= 0X01,
	DHW_MONDAY		= 0X02,
	DHW_TUESDAY		= 0X04,
	DHW_WEDNESDAY	= 0X08,
	DHW_THURSDAY	= 0X10,
	DHW_FRIDAY		= 0X20,
	DHW_SATURDAY	= 0X40,

	DHW_EVERYDAY	= 0x7F,
	DHW_ALL			= 0x7F
};

enum DH_MONTH // Months of year bit flags
			  // Start at 16 to be distinguishable 
			  // from plain month indices (1-12)
{
	DHM_NONE		= 0X0000,

	DHM_JANUARY		= 0X0010, 
	DHM_FEBRUARY	= 0X0020,
	DHM_MARCH		= 0X0040,
	DHM_APRIL		= 0X0080,
	DHM_MAY			= 0X0100,
	DHM_JUNE		= 0X0200,
	DHM_JULY		= 0X0400,
	DHM_AUGUST		= 0X0800,
	DHM_SEPTEMBER	= 0X1000,
	DHM_OCTOBER		= 0X2000,
	DHM_NOVEMBER	= 0X4000,
	DHM_DECEMBER	= 0X8000,

	DHM_ALL			= 0xFFF0
};

static DH_DAYOFWEEK DH_DAYSOFWEEK[7] =
{
	DHW_SUNDAY,
	DHW_MONDAY,
	DHW_TUESDAY,
	DHW_WEDNESDAY,
	DHW_THURSDAY,
	DHW_FRIDAY,
	DHW_SATURDAY,
};

enum OLE_DAYOFWEEK // OLE Days of week
{
	DHO_UNDEF	= -1,

	DHO_SUNDAY	= 1,
	DHO_MONDAY,
	DHO_TUESDAY,
	DHO_WEDNESDAY,
	DHO_THURSDAY,
	DHO_FRIDAY,
	DHO_SATURDAY,
};

static OLE_DAYOFWEEK OLE_DAYSOFWEEK[7] =
{
	DHO_SUNDAY,
	DHO_MONDAY,
	DHO_TUESDAY,
	DHO_WEDNESDAY,
	DHO_THURSDAY,
	DHO_FRIDAY,
	DHO_SATURDAY,
};

enum // Compare Flags
{
	DHC_COMPARETIME			= 0x01,
	DHC_COMPARESECONDS		= 0x02, // ignored without DHC_COMPARETIME
	DHC_NOTIMEISENDOFDAY	= 0x04, // ignored without DHC_COMPARETIME
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_DATEHELPERENUMS_H__2A4E63F6_A106_4295_BCBA_06D03CD67AE7__INCLUDED_)

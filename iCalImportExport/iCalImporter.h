// iCalImporter.h: interface for the CiCalImporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICALIMPORTER_H__36D6AF21_80EA_4361_85E9_B9BCDB38F913__INCLUDED_)
#define AFX_ICALIMPORTER_H__36D6AF21_80EA_4361_85E9_B9BCDB38F913__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\IImportExport.h"
#include "..\Interfaces\ITasklist.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

class ITransText;

//////////////////////////////////////////////////////////////////////

struct ICALITEM;

class CiCalEventArray : public CArray<ICALITEM*, ICALITEM*&>
{
public:
	~CiCalEventArray();
};

//////////////////////////////////////////////////////////////////////

enum ICI_LINETYPE
{
	ICILT_NONE = -1,
		
	ICILT_BEGIN_VCAL,
		
	ICILT_PRODID,
	ICILT_VERSION,
		
	ICILT_BEGIN_VTIMEZONE,
	ICILT_BEGIN_STANDARD,
	ICILT_BEGIN_DAYLIGHT,
	ICILT_BEGIN_VEVENT,
	ICILT_BEGIN_VTODO,
	ICILT_BEGIN_VALARM,
		
	ICILT_DTSTART,
	ICILT_DTDUE,
	ICILT_DTEND,
	ICILT_SUMMARY,
	ICILT_STATUS,
	ICILT_CATEGORIES,
	ICILT_URL,
	ICILT_ORGANIZER,
	ICILT_ORGANIZERCN,
	ICILT_ATTENDEE,
	ICILT_UID,
	ICILT_DESCRIPTION,
	ICILT_RELATEDTOPARENT,
	ICILT_PRIORITY,
	ICILT_PERCENT,
	ICILT_RECURRENCE,
		
	ICILT_END_VTIMEZONE,
	ICILT_END_STANDARD,
	ICILT_END_DAYLIGHT,
	ICILT_END_VEVENT,
	ICILT_END_VTODO,
	ICILT_END_VALARM,
		
	ICILT_END_VCAL,
};

//////////////////////////////////////////////////////////////////////

class CiCalImporter : public IImportTasklist  
{
public:
	CiCalImporter();
	virtual ~CiCalImporter();

	// interface implementation
	void Release() { delete this; }
	void SetLocalizer(ITransText* pTT);

	// caller must copy only
	LPCTSTR GetMenuText() const { return _T("iCalendar"); }
	LPCTSTR GetFileFilter() const { return _T("iCalendar Files (*.ics)|*.ics||"); }
	LPCTSTR GetFileExtension() const { return _T("ics"); }
	HICON GetIcon() const { return m_hIcon; }

	IIMPORT_RESULT Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	HICON m_hIcon;
	CStringArray m_aUnhandledTaskAttrib;

protected:
	int ImportTasks(const CStringArray& aLines, CiCalEventArray& aTasks);
	bool ImportTasks(const CiCalEventArray& aTasks, ITaskList12* pTasks);

protected:
	static int PreProcessFileLines(CStringArray& aLines);
	static BOOL ExtractDate(const CString& sValue, COleDateTime& date, BOOL bEndDate);
	static BOOL DecodeRecurrence(const CString& sRecur, int& nRegularity, DWORD& dwSpecific1, DWORD& dwSpecific2);
	static DWORD DecodeDayOfMonth(const CString& sDOM);

	enum ICI_STATE
	{
		ICIS_ERROR = -1,
		ICIS_BEGIN,
		ICIS_END,
		ICIS_VCAL,
		ICIS_VTIMEZONE,
		ICIS_STANDARD,
		ICIS_DAYLIGHT,
		ICIS_VEVENT,
		ICIS_VTODO,
		ICIS_VALARM,
	};
	
	static ICI_LINETYPE ExtractInfo(const CString& sLine, CString& sValue, ICI_STATE nState);
	static ICI_STATE UpdateState(ICI_LINETYPE nType, ICI_STATE nState);
	static int SortProc(const void* v1, const void* v2);

	static LPCTSTR GetMetaID() { return _T("ICAL_86563213-20EB-497b-AA7B-7EF25E295576"); }

};

#endif // !defined(AFX_ICALIMPORTER_H__36D6AF21_80EA_4361_85E9_B9BCDB38F913__INCLUDED_)

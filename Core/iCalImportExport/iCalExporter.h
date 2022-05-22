// iCalExporter.h: interface for the CiCalExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICALEXPORTER_H__5CDCA505_6C5C_4342_8F80_67368C7EE6A5__INCLUDED_)
#define AFX_ICALEXPORTER_H__5CDCA505_6C5C_4342_8F80_67368C7EE6A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\Icon.h"

#include "..\Interfaces\IImportExport.h"
#include "..\Interfaces\ITasklist.h"

class CStdioFileEx;

enum ICALEXPORTAS
{
	ICEA_EVENT,
	ICEA_TODO,
	ICEA_APPT,
};

class CiCalExporter : public IExportTasklist  
{
public:
	CiCalExporter();
	virtual ~CiCalExporter();

	// interface implementation
	void Release() { delete this; }
	void SetLocalizer(ITransText* pTT);
	bool SupportsHtmlComments() const { return false; }
	HICON GetIcon() const { return m_icon; }

	// caller must copy only
	LPCTSTR GetMenuText() const { return _T("iCalendar"); }
	LPCTSTR GetFileFilter() const { return _T("iCalendar Files (*.ics)|*.ics||"); }
	LPCTSTR GetFileExtension() const { return _T("ics"); }
	LPCWSTR GetTypeID() const { return _T("5CDCA505_6C5C_4342_8F80_67368C7EE6A5"); }

	IIMPORTEXPORT_RESULT Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);
	IIMPORTEXPORT_RESULT Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CIcon m_icon;
	BOOL NODUEDATEISTODAYORSTART;
	ICALEXPORTAS EXPORTFORMAT;

protected:
	int ExportTask(const ITASKLISTBASE* pTasks, HTASKITEM hTask, const CString& sParentUID, 
					CStdioFile& fileOut, BOOL bAndSiblings);
	bool InitConsts(DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);

	BOOL GetTaskDates(const ITASKLISTBASE* pTasks, HTASKITEM hTask, COleDateTime& dtStart, COleDateTime& dtEnd, COleDateTime& dtDue) const;

	static void __cdecl WriteString(CStdioFile& fileOut, LPCTSTR lpszFormat, ...);
	static CString FormatDateTime(LPCTSTR szType, const COleDateTime& date, BOOL bStartOfDay);
	static CString FormatRecurrence(int nRegularity, DWORD dwSpecific1, DWORD dwSpecific2);
	static CString FormatDayOfMonth(DWORD dwDOM);
	static void WriteHeader(CStdioFileEx& fileOut);
	static CString FormatUID(LPCTSTR szFileName, DWORD dwTaskID);
};

#endif // !defined(AFX_ICALEXPORTER_H__5CDCA505_6C5C_4342_8F80_67368C7EE6A5__INCLUDED_)

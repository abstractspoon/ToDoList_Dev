// TDCAnonymizeTasklist.h: interface for the CTDCAnonymizeTasklist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCANONYMIZETASKTIMELOG_H__36BFC958_99EA_4B0B_8E44_04AB78F47CE4__INCLUDED_)
#define AFX_TDCANONYMIZETASKTIMELOG_H__36BFC958_99EA_4B0B_8E44_04AB78F47CE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "TDCTaskTimeLog.h"

#include "..\Shared\mapex.h"

//////////////////////////////////////////////////////////////////////

class CTDCAnonymizeTaskTimeLog : protected CTDCTaskTimeLog
{
public:
	static BOOL Anonymize(LPCTSTR szLogfile);
	static BOOL Anonymize(LPCTSTR szLogfile, CString& sAnonFilePath);

protected:
	CString m_sContent;
	CMapStringToStringMap m_mapSharedData;

protected:
	CTDCAnonymizeTaskTimeLog();

	BOOL AnonymizeLog(LPCTSTR szLogfile, CString& sAnonFilePath);
	BOOL AnonymizeLogItems(CTaskTimeLogItemArray& aLogItems);

	CString AnonymizeText(const CString& sItem) const;
	void AnonymizeItem(CString& sItem, CMapStringToString& mapTLD) const;

	BOOL BuildContent(const CTaskTimeLogItemArray& aLogItems);
	CString GetContent(const CTaskTimeLogItemArray& aLogItems) const;

	static BOOL IsWhiteSpaceOrNumberOrPunctuation(TCHAR c);

};

#endif // !defined(AFX_TDCANONYMIZETASKTIMELOG_H__36BFC958_99EA_4B0B_8E44_04AB78F47CE4__INCLUDED_)

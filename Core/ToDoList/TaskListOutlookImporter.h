// OutlookImporter.h: interface for the COutlookImporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTLOOKIMPORTER_H__C35841C7_32A6_4705_A061_24A85C3A0223__INCLUDED_)
#define AFX_OUTLOOKIMPORTER_H__C35841C7_32A6_4705_A061_24A85C3A0223__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\icon.h"

#include "..\Interfaces\IImportExport.h"

//////////////////////////////////////////////////////////////////////

class ITransText;

//////////////////////////////////////////////////////////////////////

static const LPCTSTR OUTLOOKIMPORT_TYPEID = _T("C35841C7_32A6_4705_A061_24A85C3A0223");

//////////////////////////////////////////////////////////////////////

class CTaskListOutlookImporter : public IImportTasklist  
{
public:
	CTaskListOutlookImporter();
	virtual ~CTaskListOutlookImporter();

	void Release() { delete this; }
	void SetLocalizer(ITransText* pTT);

	LPCTSTR GetMenuText() const { return _T("Microsoft Outlook"); }
	LPCTSTR GetFileFilter() const { return NULL; }
	LPCTSTR GetFileExtension() const { return NULL; }
	LPCTSTR GetTypeID() const { return OUTLOOKIMPORT_TYPEID; }
	HICON GetIcon() const { return m_icon; }
	
	IIMPORTEXPORT_RESULT Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);
	
protected:
	CIcon m_icon;

};

#endif // !defined(AFX_OUTLOOKIMPORTER_H__C35841C7_32A6_4705_A061_24A85C3A0223__INCLUDED_)

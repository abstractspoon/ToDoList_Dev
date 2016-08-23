// OutlookExporter.h: interface for the COutlookExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTLOOKEXPORTER_H__CA0FFD07_69C8_4A9F_A453_41674A63BF4C__INCLUDED_)
#define AFX_OUTLOOKEXPORTER_H__CA0FFD07_69C8_4A9F_A453_41674A63BF4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\IImportExport.h"

class ITransText;

class COutlookExporter : public IExportTasklist    
{
public:
	COutlookExporter();
	virtual ~COutlookExporter();

	void Release() { delete this; }
	void SetLocalizer(ITransText* pTT);

	LPCTSTR GetMenuText() const { return _T("Microsoft Outlook"); }
	LPCTSTR GetFileFilter() const { return NULL; }
	LPCTSTR GetFileExtension() const { return NULL; }

	virtual bool Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, BOOL bSilent, IPreferences* pPrefs, LPCTSTR szKey);

};

#endif // !defined(AFX_OUTLOOKEXPORTER_H__CA0FFD07_69C8_4A9F_A453_41674A63BF4C__INCLUDED_)

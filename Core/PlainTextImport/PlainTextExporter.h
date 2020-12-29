// PlainTextExporter.h: interface for the CPlainTextExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAINTEXTEXPORTER_H__69016DB3_5424_49DF_A877_962E83BC6E6B__INCLUDED_)
#define AFX_PLAINTEXTEXPORTER_H__69016DB3_5424_49DF_A877_962E83BC6E6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\IImportExport.h"
#include "..\Interfaces\ITasklist.h" 

class ITransText;

class CPlainTextExporter : public IExportTasklist  
{
public:
	CPlainTextExporter();
	virtual ~CPlainTextExporter();

	void Release() { delete this; }
	void SetLocalizer(ITransText* pTT);
	bool SupportsHtmlComments() const { return false; }
	HICON GetIcon() const { return NULL; }

	LPCTSTR GetMenuText() const { return _T("Outline"); }
	LPCTSTR GetFileFilter() const { return _T("Text Files (*.txt)|*.txt||"); }
	LPCTSTR GetFileExtension() const { return _T("txt"); }
	LPCWSTR GetTypeID() const { return _T("69016DB3_5424_49DF_A877_962E83BC6E6B"); }

	IIMPORTEXPORT_RESULT Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);
	IIMPORTEXPORT_RESULT Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CString INDENT;
	BOOL WANTPROJECT;

protected:
	bool InitConsts(DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);
	void ExportTask(const ITaskList* pSrcTaskFile, HTASKITEM hTask, 
					CStdioFile& fileOut, int nDepth, BOOL bAndSiblings);
};

#endif // !defined(AFX_PLAINTEXTEXPORTER_H__69016DB3_5424_49DF_A877_962E83BC6E6B__INCLUDED_)

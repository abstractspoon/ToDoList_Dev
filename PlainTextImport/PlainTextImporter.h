// PlainTextImporter.h: interface for the CPlainTextImporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAINTEXTIMPORTER_H__E1C1DB38_D45E_481E_8D91_7D8455C5155E__INCLUDED_)
#define AFX_PLAINTEXTIMPORTER_H__E1C1DB38_D45E_481E_8D91_7D8455C5155E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\IImportExport.h"

class CPlainTextImporter : public IImportTasklist  
{
public:
	CPlainTextImporter();
	virtual ~CPlainTextImporter();
	
	void Release() { delete this; }
	void SetLocalizer(ITransText* pTT);

	LPCTSTR GetMenuText() const { return _T("Outline"); }
	LPCTSTR GetFileFilter() const { return _T("Text Files (*.txt)|*.txt||"); }
	LPCTSTR GetFileExtension() const { return _T("txt"); }
	HICON GetIcon() const { return NULL; }
	
	IIMPORT_RESULT Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, BOOL bSilent, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CString INDENT;
	int ROOTDEPTH;
	BOOL WANTPROJECT;

protected:
	bool InitConsts(BOOL bSilent, IPreferences* pPrefs, LPCTSTR szKey);
	int GetDepth(const CString& sLine); // returns the indent
	BOOL GetTitleComments(const CString& sLine, CString& sTitle, CString& sComments);
};

#endif // !defined(AFX_PLAINTEXTIMPORTER_H__E1C1DB38_D45E_481E_8D91_7D8455C5155E__INCLUDED_)

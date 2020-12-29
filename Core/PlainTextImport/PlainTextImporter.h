// PlainTextImporter.h: interface for the CPlainTextImporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAINTEXTIMPORTER_H__36A40C31_D9DD_4235_9720_5F16FA66C677__INCLUDED_)
#define AFX_PLAINTEXTIMPORTER_H__36A40C31_D9DD_4235_9720_5F16FA66C677__INCLUDED_

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
	LPCWSTR GetTypeID() const { return _T("36A40C31-D9DD-4235-9720-5F16FA66C677"); }
	HICON GetIcon() const { return NULL; }
	
	IIMPORTEXPORT_RESULT Import(LPCTSTR szSrcFilePath, ITaskList* pDestTaskFile, DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);

protected:
	CString INDENT;
	int ROOTDEPTH;
	BOOL WANTPROJECT;

protected:
	bool InitConsts(DWORD dwFlags, IPreferences* pPrefs, LPCTSTR szKey);
	int GetDepth(const CString& sLine); // returns the indent
	BOOL GetTitleComments(const CString& sLine, CString& sTitle, CString& sComments);
};

#endif // !defined(AFX_PLAINTEXTIMPORTER_H__36A40C31_D9DD_4235_9720_5F16FA66C677__INCLUDED_)

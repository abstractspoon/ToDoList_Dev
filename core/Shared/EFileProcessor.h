// FileProcessor.h: interface for the EFileProcessor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEPROCESSOR_H__7C0FEF4E_9484_11D4_95AD_EA54ED4D1646__INCLUDED_)
#define AFX_FILEPROCESSOR_H__7C0FEF4E_9484_11D4_95AD_EA54ED4D1646__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxmt.h>

class EFileProcessor
{
public:
	EFileProcessor();
	virtual ~EFileProcessor();

protected:
	// returns the number of files processed
	int EProcessFiles(LPCTSTR szSrcFolder, LPCTSTR szSrcExt = NULL, BOOL bRecursive = TRUE, 
					LPCTSTR szDestFolder = NULL, LPCTSTR szDestExt = NULL);
	
	// virtual callbacks
protected:
	// return false if the contents as an output are to be ignored
	// if the base class is called it will load the file and fill sContents
	virtual BOOL EProcessFile(LPCTSTR szFilePath, CTime tLastModified, DWORD dwSize, CString& sContents) = 0;

	// return false if the folder should not be parsed
	virtual BOOL EProcessFolder(LPCTSTR szFolderPath, CTime tLastModified) { return TRUE; }

	// helpers
	BOOL EReadFile(LPCTSTR szFilePath, CString& sContents, BOOL bPreserveCR = TRUE);
	BOOL EWriteFile(LPCTSTR szFilePath, const CString& sContents);

	void ETerminate(); // call from within EProcessFile to stop the filefind
	BOOL EContinue();

protected:
	int m_nFileCount;
	int m_nTimeTaken; // in millisecs
	BOOL m_bContinue;
	CCriticalSection m_CritSec;			// handles thread contention.

private:
	void EDoProcessFiles(LPCTSTR szSrcFolder, const CString& sSrcExt, BOOL bRecursive, 
					LPCTSTR szDestFolder, LPCTSTR szDestExt);
};

#endif // !defined(AFX_FILEPROCESSOR_H__7C0FEF4E_9484_11D4_95AD_EA54ED4D1646__INCLUDED_)

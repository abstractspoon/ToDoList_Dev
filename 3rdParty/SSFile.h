/////////////////////////////////////////////////////////////////////////////
// Structured Storage Helper Class
//
// This code is copyright (c) Andrew Peace, 2001. 
//
// It may not be redistributed in either source code or library (either 
// static or DLL) form without the author's consent.  You may freely use
// it in any programs you create with no obligation, though it's correctness
// is not guarenteed, and no responsiblity can be taken for the consequences
// of its use.
/////////////////////////////////////////////////////////////////////////////
// SSFile.h : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __SSFILE_H__
#define __SSFILE_H__

#include <objidl.h>
#include <afxole.h>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CSSFile class

class CSSFile
{
// Construction
public:
	CSSFile();
	virtual ~CSSFile();

// Implementation
public:
	// file-system level access:
	bool CreateSSFile(const CString & filename, DWORD mode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE);
	bool OpenSSFile(const CString & filename, DWORD mode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE);
	IStorage * Attach(IStorage* pNews);
	IStorage * Detach();

	// storage-level access:
	bool CreateStorage(const CString & name, bool enter = false, DWORD mode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE);
	bool EnterStorage(const CString & name, DWORD mode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE);
	bool ExitStorage();

	bool CreateStream(const CString & name, COleStreamFile &sf, DWORD mode = CFile::modeReadWrite | CFile::shareExclusive);
	bool OpenStream(const CString & name, COleStreamFile &sf, DWORD mode = CFile::modeReadWrite | CFile::shareExclusive);

	bool DestroyElement(const CString & szName);

  static HRESULT StreamToFile(IStream * stream, CString strFile_name);

	// status info:
	IStorage * GetRootStorage() const;
	IStorage * GetCurrentStorage() const;
	CString GetPath(const CString & SepChar = _T("\\")) const;
	CString GetFilename() const;
	bool IsOpen() const;
	void Close();

	struct StorageData
	{
		IStorage *Stg;
		StorageData *ParentStg;

		StorageData()
		{
			Stg = NULL;
			ParentStg = NULL;
		}
	};

protected:
	bool m_bOpen;
	CString m_strFilename;
	IStorage *m_pRootStg;
	StorageData *m_pCurrentStg;
};

/////////////////////////////////////////////////////////////////////////////

#endif // __SSFILE_H__


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
// SSFile.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SSFile.h"

//#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <share.h>
#include <errno.h>
#include <set>
//#include "shlobj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSSFile

CSSFile::CSSFile()
{
	m_bOpen = false;
	m_strFilename = _T("");
	m_pCurrentStg = NULL;
	m_pRootStg = NULL;
}

CSSFile::~CSSFile()
{
	Close();
}

/////////////////////////////////////////////////////////////////////////////
// Opening, creating and closing storage files

bool CSSFile::OpenSSFile(const CString & filename, DWORD mode)
{
	if (m_bOpen)
		Close();

	WCHAR wcFName[MAX_PATH];
#ifndef UNICODE
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCTSTR)filename, -1, wcFName, MAX_PATH);
#else
	wcscpy(wcFName, (LPCTSTR)filename);
#endif

	if (StgOpenStorage(wcFName, NULL, mode, NULL, 0, &m_pRootStg) != S_OK)
		return false;

	m_bOpen = true;
	m_strFilename = filename;

  m_pCurrentStg = new StorageData;
  m_pCurrentStg->ParentStg = NULL;
  m_pCurrentStg->Stg = m_pRootStg;

	return true;
}

bool CSSFile::CreateSSFile(const CString & filename, DWORD mode)
{
	if (m_bOpen)
		Close();

	WCHAR wcFName[MAX_PATH];
#ifndef UNICODE
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCTSTR)filename, -1, wcFName, MAX_PATH);
#else
	wcscpy(wcFName, (LPCTSTR)filename);
#endif

	if (StgCreateDocfile(wcFName, mode, 0, &m_pRootStg) != S_OK)
		return false;

	m_bOpen = true;
	m_strFilename = filename;

  m_pCurrentStg = new StorageData;
  m_pCurrentStg->ParentStg = NULL;
  m_pCurrentStg->Stg = m_pRootStg;

	return true;
}

IStorage* CSSFile::Attach(IStorage* pNews)
{
	ASSERT(pNews != NULL);

	// store the old root storage:
	IStorage* old = m_pRootStg;
	while (ExitStorage())
		;	
	
	// set up the new one:
	m_pRootStg = pNews;
	m_pCurrentStg = new StorageData;
	m_pCurrentStg->ParentStg = NULL;
	m_pCurrentStg->Stg = pNews;

  m_bOpen = true;

	// return the old one:
	return old;
}

void CSSFile::Close()
{
	if (m_bOpen)
	{
		// Release all pointers in the open tree:
		while (ExitStorage())
			;
  }

  if (m_pCurrentStg)
		  m_pCurrentStg->Stg->Release();

	delete m_pCurrentStg;
  m_pCurrentStg = NULL;

	m_pRootStg = NULL;
	m_bOpen = false;
	m_strFilename = _T("");
}

IStorage * CSSFile::Detach()
{
	while (ExitStorage())
		;

	IStorage* rtn = m_pRootStg;

  if (m_pCurrentStg)
    m_pCurrentStg->Stg->Release();

  delete m_pCurrentStg;
  m_pCurrentStg = NULL;

	m_bOpen = false;
	m_pRootStg = NULL;
	m_strFilename = _T("");

	return rtn;
}

/////////////////////////////////////////////////////////////////////////////
// General storage manipulation functions:

bool CSSFile::DestroyElement(const CString & szName)
{
	ASSERT(m_pCurrentStg && m_bOpen);

	WCHAR wcFName[MAX_PATH];
#ifndef UNICODE
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCTSTR)szName, -1, wcFName, MAX_PATH);
#else
	wcscpy(wcFName, (LPCTSTR)szName);
#endif

	return (m_pCurrentStg->Stg->DestroyElement(wcFName) == S_OK);
}

/////////////////////////////////////////////////////////////////////////////
// Inter-storage functions:

bool CSSFile::CreateStorage(const CString & name, bool enter, DWORD mode)
{
	ASSERT(m_pCurrentStg && m_bOpen);

	WCHAR wcFName[MAX_PATH];
#ifndef UNICODE
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCTSTR)name, -1, wcFName, MAX_PATH);
#else
	wcscpy(wcFName, (LPCTSTR)name);
#endif

	IStorage* pNewStorage;
	if (m_pCurrentStg->Stg->CreateStorage(wcFName, mode, 0, 0, &pNewStorage) != S_OK)
		return false;

	if (!enter)
		pNewStorage->Release();
	else
	{
		StorageData* pSD = new StorageData;
		pSD->ParentStg = m_pCurrentStg;
		pSD->Stg = pNewStorage;
		m_pCurrentStg = pSD;
	}
	
	return true;
}

bool CSSFile::EnterStorage(const CString & name, DWORD mode)
{
	ASSERT(m_pCurrentStg && m_bOpen);

	WCHAR wcFName[MAX_PATH];
#ifndef UNICODE
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCTSTR)name, -1, wcFName, MAX_PATH);
#else
	wcscpy(wcFName, (LPCTSTR)name);
#endif

	IStorage* pStg;
	if (m_pCurrentStg->Stg->OpenStorage(wcFName, NULL, mode, NULL, 0, &pStg) == S_OK)
	{
		StorageData* pSD = new StorageData;
		pSD->ParentStg = m_pCurrentStg;
		pSD->Stg = pStg;

		m_pCurrentStg = pSD;
		
		return true;
	}
	else
		return false;
}

bool CSSFile::ExitStorage()
{
	ASSERT(m_pCurrentStg || !m_bOpen);

  if (!m_bOpen && !m_pCurrentStg)
    return false;

	if (m_pCurrentStg->ParentStg)
	{
		m_pCurrentStg->Stg->Release();
		StorageData* pSD = m_pCurrentStg->ParentStg;
		delete m_pCurrentStg;
		m_pCurrentStg = pSD;

		return true;
	}
	else
		return false; // no storage to exit out of without closing the file
}

/////////////////////////////////////////////////////////////////////////////
// Stream functions:

bool CSSFile::CreateStream(const CString & name, COleStreamFile & sf, DWORD mode)
{
	// note the BOOL return type being converted to bool;
	return (sf.CreateStream(m_pCurrentStg->Stg, (LPCTSTR)name, mode) == TRUE);
}

bool CSSFile::OpenStream(const CString & name, COleStreamFile & sf, DWORD mode)
{
	ASSERT(m_pCurrentStg && m_bOpen);

	return (sf.OpenStream(m_pCurrentStg->Stg, (LPCTSTR)name, mode) == TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Status functions

bool CSSFile::IsOpen() const
{
	return m_bOpen;
}

CString CSSFile::GetFilename() const
{
	return m_strFilename;
}

CString CSSFile::GetPath(const CString & SepChar) const
{
	ASSERT(m_pCurrentStg && m_bOpen);

	StorageData* pSD = m_pCurrentStg;

	// loop through each storage in the tree and concatenate names along with the
	// separator character:
	CString strPath;
	while (pSD->ParentStg)
	{
		STATSTG sg;
		pSD->Stg->Stat(&sg, STATFLAG_DEFAULT);

		CString strTemp = sg.pwcsName;
		CoTaskMemFree((void *)sg.pwcsName);

		strPath = strTemp + SepChar + strPath;

		pSD = pSD->ParentStg; // up a level for next interation
	}

	strPath = m_strFilename + SepChar + strPath;
	return strPath;
}

IStorage * CSSFile::GetCurrentStorage() const
{
	ASSERT(m_pCurrentStg && m_bOpen);

	return m_pCurrentStg->Stg;
}

IStorage * CSSFile::GetRootStorage() const
{
	ASSERT(m_pCurrentStg && m_bOpen);

	return m_pRootStg;
}

HRESULT CSSFile::StreamToFile(IStream *stream, CString strFile_name)
{
  #define BLOCK_SIZE 1024

  HRESULT       hr = S_OK;
  unsigned char buffer[BLOCK_SIZE];
  unsigned long bytes_read = 0;
  int           bytes_written = 0;
  int		    handle = _tsopen(strFile_name, O_RDWR | O_BINARY | O_CREAT, SH_DENYNO, S_IREAD | S_IWRITE);

  if (handle != -1) 
  {
    do 
	{
      hr = stream->Read(buffer, BLOCK_SIZE, &bytes_read);
    
	  if (bytes_read)
        bytes_written = _write(handle, buffer, bytes_read);
	  
    } 
	while (S_OK == hr && bytes_read == BLOCK_SIZE);

    _close(handle);

    if (bytes_written == 0)
		::DeleteFile(strFile_name);
  }
  else 
  {
	  unsigned long  error;

	  if ((error = GetLastError()) == 0L)
		  error = _doserrno;

	  hr = HRESULT_FROM_WIN32(errno);
  }

  return hr;
}

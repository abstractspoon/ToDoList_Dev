// TasklistStorageMgr.cpp: implementation of the CTasklistStorageMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TasklistStorageMgr.h"
#include "ITaskList.h"
#include "ITasklistStorage.h"

#include "..\shared\filemisc.h"
#include "..\shared\misc.h"
#include "..\shared\localizer.h"

#include "..\3rdParty\base64coder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const LPCTSTR PREF_KEY = _T("OtherStorage");

//////////////////////////////////////////////////////////////////////

TSM_TASKLISTINFO::TSM_TASKLISTINFO()
{
	Reset();
}

TSM_TASKLISTINFO::TSM_TASKLISTINFO(const TSM_TASKLISTINFO& info)
{
	(*this = info);
}

const TSM_TASKLISTINFO& TSM_TASKLISTINFO::operator=(const TSM_TASKLISTINFO& info)
{
	sStorageID = info.sStorageID;

	lstrcpyn(szTasklistID, info.szTasklistID, ITS_TASKLISTID_LEN);
	lstrcpyn(szTasklistName, info.szTasklistName, _MAX_PATH);
	lstrcpyn(szLocalFileName, info.szLocalFileName, _MAX_PATH);
	lstrcpyn(szDisplayPath, info.szDisplayPath, _MAX_PATH);
	lstrcpyn(szPassword, info.szPassword, ITS_PASSWORD_LEN);
	
	return *this;
}

void TSM_TASKLISTINFO::Reset()
{
	sStorageID.Empty();
	szTasklistID[0] = 0;
	szTasklistName[0] = 0;
	szLocalFileName[0] = 0;
	szDisplayPath[0] = 0;
	szPassword[0] = 0;
}

BOOL TSM_TASKLISTINFO::HasInfo() const
{
	return (!sStorageID.IsEmpty() &&
			szTasklistID[0] &&
			szDisplayPath[0]);
}

BOOL TSM_TASKLISTINFO::HasTasklistName() const
{
	return !Misc::IsEmpty(szTasklistName);
}

void TSM_TASKLISTINFO::SetTasklistName(LPCTSTR szName)
{
	lstrcpyn(szTasklistName, szName, _MAX_PATH);
}

BOOL TSM_TASKLISTINFO::HasLocalFilePath() const
{
	return !Misc::IsEmpty(szLocalFileName);
}

void TSM_TASKLISTINFO::SetLocalFilePath(LPCTSTR szFilePath)
{
	lstrcpyn(szLocalFileName, szFilePath, _MAX_PATH);
}

void TSM_TASKLISTINFO::ClearLocalFilePath()
{
	szLocalFileName[0] = 0;
}

CString TSM_TASKLISTINFO::EncodeInfo(BOOL bIncPassword) const
{
	// format all the info into a single string with each element
	// encoded for safety
	CString sInfo, sPassword = (bIncPassword ? szPassword : _T(""));

	sInfo.Format(_T("%s;%s;%s;%s;%s;%s"), 
				Encode(sStorageID),
				Encode(szTasklistID),
				Encode(szLocalFileName),
				Encode(szDisplayPath),
				Encode(sPassword),
				Encode(szTasklistName));

	// then encode that too
	return Encode(sInfo);
}

CString TSM_TASKLISTINFO::Decode(const CString& sData)
{
	return Base64Coder().Decode(sData);
}

CString TSM_TASKLISTINFO::Encode(const CString& sData)
{
	return Base64Coder::Encode(sData);
}

BOOL TSM_TASKLISTINFO::IsStorage(const CString& sInfo, BOOL bIncPassword)
{
	return TSM_TASKLISTINFO().DecodeInfo(sInfo, bIncPassword);
}

BOOL TSM_TASKLISTINFO::DecodeInfo(const CString& sInfo, BOOL bIncPassword)
{
	Reset();

	// decode the string
	CString sDecoded = Decode(sInfo);

	// split the string into it's components
	CStringArray aParts;

	// there must be 5 bits regardless
	if (Misc::Split(sDecoded, aParts, ';', TRUE) >= 5)
	{
		// decode and assign the bits
		sStorageID = Decode(aParts[0]);

		// check the lengths in case someone is sending us a dud string
		while (true) // easy exit
		{
			if (!DecodeInfo(aParts[1], szTasklistID, ITS_TASKLISTID_LEN))
				break;

			if (!DecodeInfo(aParts[2], szLocalFileName, _MAX_PATH))
				break;

			if (!DecodeInfo(aParts[3], szDisplayPath, _MAX_PATH))
				break;

			if (bIncPassword && !DecodeInfo(aParts[4], szPassword, ITS_PASSWORD_LEN))
				break;

			if ((aParts.GetSize() == 6) && !DecodeInfo(aParts[5], szTasklistName, _MAX_PATH))
				break;

			// else
			return TRUE;
		}
	}

	// else
	Reset();
	return FALSE;
}

BOOL TSM_TASKLISTINFO::DecodeInfo(const CString& sPart, LPTSTR szAttrib, int nMaxLen)
{
	CString sTemp = Decode(sPart);

	if (sTemp.GetLength() > nMaxLen)
		return FALSE;

	lstrcpy(szAttrib, sTemp);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTasklistStorageMgr::CTasklistStorageMgr() : m_bInitialized(FALSE)
{

}

CTasklistStorageMgr::~CTasklistStorageMgr()
{
	Release();
}

void CTasklistStorageMgr::Release()
{
	if (!m_bInitialized)
		return;

	// cleanup
	int nInterface = m_aStorage.GetSize();

	while (nInterface--)
		m_aStorage[nInterface]->Release();

	m_aStorage.RemoveAll();

	m_bInitialized = FALSE;
}

void CTasklistStorageMgr::Initialize() const
{
	if (m_bInitialized)
		return;

	// we need a non-const pointer to update the array
	CTasklistStorageMgr* pMgr = const_cast<CTasklistStorageMgr*>(this);

	// look at every dll from wherever we are now
	CFileFind ff;
    CString sSearchPath = FileMisc::GetAppFilePath(), sFolder, sDrive;

	FileMisc::SplitPath(sSearchPath, &sDrive, &sFolder);
	FileMisc::MakePath(sSearchPath, sDrive, sFolder, _T("*"), _T(".dll"));

	BOOL bContinue = ff.FindFile(sSearchPath);
	
	while (bContinue)
	{
		bContinue = ff.FindNextFile();
		
		if (!ff.IsDots() && !ff.IsDirectory())
		{
			CString sDllPath = ff.GetFilePath();

			if (IsTasklistStorageDll(sDllPath))
			{
				ITasklistStorage* pStorage = CreateTasklistStorageInterface(sDllPath);

				if (pStorage)
				{
					pStorage->SetLocalizer(CLocalizer::GetLocalizer());
					pMgr->m_aStorage.Add(pStorage);
				}
			}
		}
	}

	pMgr->m_bInitialized = TRUE;
}

void CTasklistStorageMgr::UpdateLocalizer()
{
	if (!m_bInitialized)
		return;

	int nInterface = m_aStorage.GetSize();

	while (nInterface--)
		m_aStorage[nInterface]->SetLocalizer(CLocalizer::GetLocalizer());
}

int CTasklistStorageMgr::GetNumStorage() const
{
	Initialize(); // initialize on demand

	return m_aStorage.GetSize();
}

CString CTasklistStorageMgr::GetStorageMenuText(int nStorage) const
{
	Initialize(); // initialize on demand

	CString sText;

	if (nStorage >= 0 && nStorage < m_aStorage.GetSize())
	{
		ASSERT (m_aStorage[nStorage] != NULL);
		sText = m_aStorage[nStorage]->GetMenuText();
	}

	return Misc::Trim(sText);
}

CString CTasklistStorageMgr::GetStorageTypeID(int nStorage) const
{
	Initialize(); // initialize on demand

	CString sText;

	if (nStorage >= 0 && nStorage < m_aStorage.GetSize())
	{
		ASSERT (m_aStorage[nStorage] != NULL);
		sText = m_aStorage[nStorage]->GetTypeID();
	}

	return Misc::Trim(sText);
}

HICON CTasklistStorageMgr::GetStorageIcon(int nStorage) const
{
	Initialize(); // initialize on demand

	if (nStorage >= 0 && nStorage < m_aStorage.GetSize())
	{
		ASSERT (m_aStorage[nStorage] != NULL);
		return m_aStorage[nStorage]->GetIcon();
	}

	return NULL;
}

BOOL CTasklistStorageMgr::RetrieveTasklist(TSM_TASKLISTINFO* pFInfo, ITaskList* pDestTaskFile, 
										   int nByStorage, IPreferences* pPrefs, BOOL bPrompt)
{
	Initialize(); // initialize on demand

	// verify storage component
	ASSERT(!pFInfo->sStorageID.IsEmpty() || (nByStorage != -1));

	if (!pFInfo->sStorageID.IsEmpty())
		nByStorage = FindStorage(pFInfo->sStorageID);

	ASSERT(nByStorage != -1);
	
	if (nByStorage >= 0 && nByStorage < m_aStorage.GetSize())
	{
		CWaitCursor cursor;
		
		ITasklistStorage* pStorage = m_aStorage[nByStorage];
		ASSERT (pStorage != NULL);

		CString sKey;
		sKey.Format(_T("%s\\%s"), PREF_KEY, pStorage->GetTypeID());
		
		if (pStorage->RetrieveTasklist(pFInfo, pDestTaskFile, pPrefs, sKey, (bPrompt != FALSE)))
		{
			// add storageID
			pFInfo->sStorageID = pStorage->GetTypeID();
			return TRUE;
		}
	}

	// else
	return FALSE;
}

BOOL CTasklistStorageMgr::StoreTasklist(TSM_TASKLISTINFO* pFInfo, const ITaskList* pSrcTaskFile, 
										int nByStorage, IPreferences* pPrefs, BOOL bPrompt)
{
	Initialize(); // initialize on demand

	// sanity checks
	ASSERT (pSrcTaskFile && FileMisc::FileExists(pFInfo->szLocalFileName));

	// verify storage component
	ASSERT(!pFInfo->sStorageID.IsEmpty() || (nByStorage != -1));

	if (!pFInfo->sStorageID.IsEmpty())
		nByStorage = FindStorage(pFInfo->sStorageID);

	ASSERT(nByStorage != -1);

	if (nByStorage >= 0 && nByStorage < m_aStorage.GetSize())
	{
		CWaitCursor cursor;
		
		ITasklistStorage* pStorage = m_aStorage[nByStorage];
		ASSERT (pStorage != NULL);
		
		CString sKey;
		sKey.Format(_T("%s\\%s"), PREF_KEY, pStorage->GetTypeID());
		
		if (pStorage->StoreTasklist(pFInfo, pSrcTaskFile, pPrefs, sKey, (bPrompt != FALSE)))
		{
			pFInfo->sStorageID = pStorage->GetTypeID();
			return TRUE;
		}
	}

	// else
	return FALSE;
}

int CTasklistStorageMgr::FindStorage(LPCTSTR szTypeID) const 
{
	Initialize(); // initialize on demand

	int nStorage = m_aStorage.GetSize();

	while (nStorage--)
	{
		if (GetStorageTypeID(nStorage).CompareNoCase(szTypeID) == 0) // match
			return nStorage;
	}
	
	// else
	return -1;
}

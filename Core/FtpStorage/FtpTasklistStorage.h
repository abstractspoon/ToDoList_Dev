// FtpTasklistStorage.h: interface for the CFtpTasklistStorage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPTASKLISTSTORAGE_H__14908CE5_AA9F_4AFC_B72E_3F2BDD0993F0__INCLUDED_)
#define AFX_FTPTASKLISTSTORAGE_H__14908CE5_AA9F_4AFC_B72E_3F2BDD0993F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\Icon.h"

#include "..\Interfaces\ITaskListStorage.h"

/////////////////////////////////////////////////////////////////////////////

class CRemoteFile;

/////////////////////////////////////////////////////////////////////////////

class CFtpTasklistStorageApp : public ITasklistStorage, public CWinApp  
{
public:
	CFtpTasklistStorageApp();
	virtual ~CFtpTasklistStorageApp();

	// interface implementation
    void Release();
	void SetLocalizer(ITransText* pTT);

	// caller must copy result only
	LPCTSTR GetMenuText() const { return _T("Ftp Server"); }
	HICON GetIcon() const { return m_icon; }
	LPCTSTR GetTypeID() const { return _T("14908CE5_AA9F_4AFC_B72E_3F2BDD0993F0"); }

	bool RetrieveTasklist(ITS_TASKLISTINFO* pFInfo, ITaskList* pDestTaskFile, IPreferences* pPrefs, LPCTSTR szKey, bool bPrompt);
	bool StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* pSrcTaskFile, IPreferences* pPrefs, LPCTSTR szKey, bool bPrompt);

protected:
	CIcon m_icon;
	CString m_sCachedPassword;

protected:
	BOOL InitInstance();
	static void CopyInfo(const CString& sLocalPath, const CString& sRemotePath, const CRemoteFile& rmFrom, ITS_TASKLISTINFO* pToFInfo);
};

#endif // !defined(AFX_FTPTASKLISTSTORAGE_H__14908CE5_AA9F_4AFC_B72E_3F2BDD0993F0__INCLUDED_)

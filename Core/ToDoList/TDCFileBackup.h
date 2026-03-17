// CToDoCtrlMgr.h : header file
//

#if !defined(AFX_TDCFILEBACKUP_H__13051D32_D372_4205_BA71_05FAC2159F1C__INCLUDED_)
#define AFX_TDCFILEBACKUP_H__13051D32_D372_4205_BA71_05FAC2159F1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
 
class CTDCFileBackup
{
public:
// 	CTDCFileBackup();
// 	~CTDCFileBackup();

	static BOOL CreateBackup(const CString& sFilePath, const CString& sBackupFolder, int nNumKeepBackups);
	static void BackupLogFiles(const CString& sTDLPath, const CString& sBackupFolder, int nNumKeepBackups, BOOL bLogSeparately);
};

#endif // !defined(AFX_TDCFILEBACKUP_H__13051D32_D372_4205_BA71_05FAC2159F1C__INCLUDED_)

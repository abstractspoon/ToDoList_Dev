// CTDCFileBackup.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDCFileBackup.h"
#include "TDCTasktimelog.h"

#include "..\shared\mapex.h"
#include "..\shared\filemisc.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

void CTDCFileBackup::BackupLogFiles(const CString& sTDLPath, const CString& sBackupFolder, 
									int nNumKeepBackups, BOOL bLogSeparately)
{
	CTDCTaskTimeLog log(sTDLPath);

	if (bLogSeparately)
	{
#define MAGIC_TASKID	    999999999
#define MAGIC_TASKIDSTR _T("999999999")
		
		CString sLogPattern = log.GetLogPath(MAGIC_TASKID, TRUE);
		sLogPattern.Replace(MAGIC_TASKIDSTR, _T("*"));

		CStringArray aLogFiles;
		int nFile = FileMisc::FindFiles(FileMisc::GetFolderFromFilePath(sLogPattern), 
										aLogFiles, 
										FALSE, 
										FileMisc::GetFileNameFromPath(sLogPattern));
		while (nFile--)
			CreateBackup(aLogFiles[nFile], sBackupFolder, nNumKeepBackups);
	}
	else // single log file
	{
		CString sLogPath = log.GetLogPath();
		
		CreateBackup(sLogPath, sBackupFolder, nNumKeepBackups);
	}
}

BOOL CTDCFileBackup::CreateBackup(const CString& sFilePath, const CString& sBackupFolder, 
								  int nNumKeepBackups)
{
	// No need to create a backup of non-existent file
	if (!FileMisc::FileExists(sFilePath))
		return FALSE;
	
	// NOTE: We encode the app version in the backup name to ensure that
	// in the event that an update contains a breaking bug which corrupts 
	// files we haven't already deleted 'good' backups of the previous 
	// version before the bug is discovered.
	if (!CFileBackup().MakeBackup(sFilePath, FBS_APPVERSION | FBS_DATETIMESTAMP, sBackupFolder, _T("")))
	{
		ASSERT(0);
		return FALSE;
	}

	if (nNumKeepBackups > 0)
	{
		CString sBackupPattern = FileMisc::GetFileNameFromPath(sFilePath);
		FileMisc::AddToFileName(sBackupPattern, _T(".*"));

		// Build file lists for each distinct version including the current
		CStringArray aFilenames;
		int nNumBackups = FileMisc::FindFiles(sBackupFolder, aFilenames, FALSE, sBackupPattern);

		CMapStringToStringArray mapBackupFiles;

		for (int nFile = 0; nFile < nNumBackups; nFile++)
		{
			// Extract version from file path
			CStringArray aNameParts;

			if (Misc::Split(aFilenames[nFile], aNameParts, '.') == 4) // <filename>,<app ver>.<datetime>.<ext>
			{
				CString sFileVer = aNameParts[1];
				mapBackupFiles.GetAddMapping(sFileVer)->Add(aFilenames[nFile]);
			}
		}

		// Delete all but the last backup for each past version,
		// and all but the last 'nNumKeepBackups' for the current version
		CString sCurVer = FileMisc::GetAppVersion('_');
		POSITION pos = mapBackupFiles.GetStartPosition();

		while (pos)
		{
			CString sAppVer;
			CStringArray* pFiles;
			
			mapBackupFiles.GetNextAssoc(pos, sAppVer, pFiles);
			Misc::SortArray(aFilenames); // earliest -> latest

			int nNumToKeep = ((sAppVer == sCurVer) ? nNumKeepBackups : 1);

			while (pFiles->GetSize() > nNumToKeep)
			{
				FileMisc::DeleteFile(pFiles->GetAt(0));
				pFiles->RemoveAt(0);
			}
		}
	}

	return TRUE;
}

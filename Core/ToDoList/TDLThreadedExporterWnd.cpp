// TDLThreadedExportWnd.cpp : implementation file
//

#include "stdafx.h"
#include "TDLThreadedExporterWnd.h"
#include "TDCImportExportMgr.h"
#include "tdcmsg.h"
#include "tdcstruct.h"

#include "..\shared\filemisc.h"
#include "..\shared\misc.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

struct TEWEXPORTWRAP
{
	TEWEXPORTWRAP(TDCEXPORTTASKLIST* pOrgExport, HWND hwndTemp)
	{
		pExport = pOrgExport;
		hwndTempNotify = hwndTemp;
	}
	
	HWND hwndTempNotify;
	TDCEXPORTTASKLIST* pExport;
};

/////////////////////////////////////////////////////////////////////////////

class CTEWPreferencesWrap : public IPreferences
{
public:
	CTEWPreferencesWrap(IPreferences* pPrefs) : m_pPrefs(pPrefs) {}
	virtual ~CTEWPreferencesWrap() {}
	
	UINT GetProfileInt(LPCWSTR lpszSection, LPCWSTR lpszEntry, int nDefault) const
	{
		// Hack to prevent exporters adding space for notes
		if ((CString(_T("Preferences")).CompareNoCase(lpszSection) == 0) &&
			(CString(_T("ExportSpaceForNotes")).CompareNoCase(lpszEntry) == 0))
		{
			return FALSE;
		}
		else
		{
			return m_pPrefs->GetProfileInt(lpszSection, lpszEntry, nDefault);
		}
	}

	bool WriteProfileInt(LPCWSTR lpszSection, LPCWSTR lpszEntry, int nValue)
	{
		return m_pPrefs->WriteProfileInt(lpszSection, lpszEntry, nValue);
	}
	
	LPCWSTR GetProfileString(LPCWSTR lpszSection, LPCWSTR lpszEntry, LPCWSTR lpszDefault) const
	{
		return m_pPrefs->GetProfileString(lpszSection, lpszEntry, lpszDefault);
	}
	
	bool WriteProfileString(LPCWSTR lpszSection, LPCWSTR lpszEntry, LPCWSTR lpszValue)
	{
		return m_pPrefs->WriteProfileString(lpszSection, lpszEntry, lpszValue);
	}
	
	double GetProfileDouble(LPCWSTR lpszSection, LPCWSTR lpszEntry, double dDefault) const
	{
		return m_pPrefs->GetProfileDouble(lpszSection, lpszEntry, dDefault);
	}
	
	bool WriteProfileDouble(LPCWSTR lpszSection, LPCWSTR lpszEntry, double dValue)
	{
		return m_pPrefs->WriteProfileDouble(lpszSection, lpszEntry, dValue);
	}
	
	bool DeleteProfileEntry(LPCWSTR lpszSection, LPCWSTR lpszEntry)
	{
		return m_pPrefs->DeleteProfileEntry(lpszSection, lpszEntry);
	}

	bool DeleteProfileSection(LPCWSTR lpszSection, bool bIncSubSections)
	{
		return m_pPrefs->DeleteProfileSection(lpszSection, bIncSubSections);
	}

	bool HasProfileSection(LPCWSTR lpszSection) const
	{
		return m_pPrefs->HasProfileSection(lpszSection);
	}
	
protected:
	IPreferences* m_pPrefs;
};

/////////////////////////////////////////////////////////////////////////////
// CTDLThreadedExportWnd

CTDLThreadedExporterWnd::CTDLThreadedExporterWnd() 
	: 
	m_nNumThreads(0)
{
}

CTDLThreadedExporterWnd::~CTDLThreadedExporterWnd()
{
}


BEGIN_MESSAGE_MAP(CTDLThreadedExporterWnd, CWnd)
	//{{AFX_MSG_MAP(CTDLThreadedExportWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_TDLTE_EXPORTTHREADFINISHED, OnExportThreadFinished)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTDLThreadedExportWnd message handlers

BOOL CTDLThreadedExporterWnd::ExportTasks(TDCEXPORTTASKLIST* pExport)
{
	if (!(pExport && pExport->IsValid()))
	{
		ASSERT(0);
		return FALSE;
	}

	// Create once only
	if (!GetSafeHwnd())
	{
		if (!Create(NULL, NULL, (WS_CHILD | WS_DISABLED), CRect(0, 0, 0, 0), 
					CWnd::FromHandle(pExport->hWndNotify), (UINT)IDC_STATIC))
		{
			ASSERT(0);
			return FALSE;
		}
	}

	TEWEXPORTWRAP* pWrapExport = new TEWEXPORTWRAP(pExport, GetSafeHwnd());

	// start thread paused
	CWinThread* pThread = AfxBeginThread(ExportThreadProc, 
										 (LPVOID)pWrapExport, 
										 THREAD_PRIORITY_NORMAL, 
										 0, 
										 CREATE_SUSPENDED);
	ASSERT(pThread);

	if (pThread)
	{
		if (pThread->ResumeThread() == 1)
		{
			m_nNumThreads++;
			return TRUE;
		}
		
		// else
		pThread->Delete();
		// Fall thru
	}

	// else call thread proc directly
	return ExportThreadProc((LPVOID)pWrapExport);
}

UINT CTDLThreadedExporterWnd::ExportThreadProc(LPVOID pParam)
{
	FileMisc::LogTextRaw(_T("\nCTDLThreadedExporterWnd::ExportThreadProc(begin)"));

	TEWEXPORTWRAP* pExportWrap = (TEWEXPORTWRAP*)pParam;

	TDCEXPORTTASKLIST* pExport = pExportWrap->pExport;
	BOOL bSuccess = TRUE;

	// Validation
	if (!pExport || !pExport->IsValid())
	{
		bSuccess = FALSE;
		FileMisc::LogTextRaw(_T("CTDLThreadedExporterWnd::ExportThreadProc(Bad inputs) failed\n"));
	}
	else if (!pExport->sSaveIntermediatePath.IsEmpty())
	{
		if (!pExport->tasks.Save(pExport->sSaveIntermediatePath, SFEF_UTF16))
		{
			bSuccess = FALSE;
			FileMisc::LogTextRaw(_T("CTDLThreadedExporterWnd::ExportThreadProc(Intermediate save) failed\n"));
		}

		// Restore original tasklist path
		pExport->tasks.SetFilePath(pExport->sTDCPath);
	}

	// The actual export
	if (bSuccess)
	{
		if (FileMisc::FileExists(pExport->sStylesheet)) // bTransform
		{
			bSuccess = pExport->tasks.TransformToFile(pExport->sStylesheet, pExport->sExportPath);

			if (!bSuccess)
				FileMisc::LogTextRaw(_T("CTDLThreadedExporterWnd::ExportThreadProc(Transform) failed\n"));
		}
		else if (pExport->nPurpose == TDCTEP_DUETASKNOTIFY)
		{
			// For due task notifications we don't want space added for notes
			// so we use a proxy which handles this
			CPreferences prefs;
			CTEWPreferencesWrap prefsWrap(prefs);

			bSuccess = (IIER_SUCCESS == pExport->pImpExpMgr->ExportTaskList(pExport->tasks, 
																			pExport->sExportPath, 
																			pExport->nExporter, 
																			IIEF_SILENT, 
																			&prefsWrap));
			if (!bSuccess)
				FileMisc::LogTextRaw(_T("CTDLThreadedExporterWnd::ExportThreadProc(Due tasks) failed"));
		}
		else
		{
			bSuccess = (IIER_SUCCESS == pExport->pImpExpMgr->ExportTaskList(pExport->tasks, 
																			pExport->sExportPath, 
																			pExport->nExporter, 
																			IIEF_SILENT));
			if (!bSuccess)
				FileMisc::LogTextRaw(_T("CTDLThreadedExporterWnd::ExportThreadProc(Export) failed"));
		}
	}

	// notify ourselves
	FileMisc::LogTextRaw(_T("CTDLThreadedExporterWnd::ExportThreadProc(Posting self-notification)"));

	::PostMessage(pExportWrap->hwndTempNotify, WM_TDLTE_EXPORTTHREADFINISHED, bSuccess, (LPARAM)pExportWrap);

	FileMisc::LogTextRaw(_T("CTDLThreadedExporterWnd::ExportThreadProc(end)"));
	
	return bSuccess;
}

LRESULT CTDLThreadedExporterWnd::OnExportThreadFinished(WPARAM wp, LPARAM lp)
{
	FileMisc::LogTextRaw(_T("CTDLThreadedExporterWnd::ExportThreadProc(Received self-notification)"));

	m_nNumThreads--;
	ASSERT(m_nNumThreads >= 0);

	TEWEXPORTWRAP* pExportWrap = (TEWEXPORTWRAP*)lp;
	
	TDCEXPORTTASKLIST* pExport = pExportWrap->pExport;
	ASSERT(pExport && pExport->IsValid());
	
	// cleanup
	delete pExportWrap;

	FileMisc::LogTextRaw(_T("CTDLThreadedExporterWnd::ExportThreadProc(Deleted wrapper memory)"));

	// Notify parent
	FileMisc::LogTextRaw(_T("CTDLThreadedExporterWnd::ExportThreadProc(Posting parent-notification)"));

	::PostMessage(pExport->hWndNotify, WM_TDLTE_EXPORTTHREADFINISHED, wp, (LPARAM)pExport);
	
	return 0L;
}

void CTDLThreadedExporterWnd::OnDestroy()
{
	ASSERT(IsFinished());

	CWnd::OnDestroy();
}
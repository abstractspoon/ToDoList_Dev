// TDCSourceControlHelper.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TDCSourceControl.h"
#include "ToDoCtrl.h"

#include "..\shared\FileMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// CTDCSourceControlHelper

CTDCSourceControl::CTDCSourceControl(CToDoCtrl* pTDC) 
	: 
	m_pTDC(pTDC),
	m_bSourceControlled(FALSE),
	m_bCheckedOut(FALSE)
{
}

CTDCSourceControl::~CTDCSourceControl()
{
}

void CTDCSourceControl::Initialise(CTaskFile& tasks)
{
	m_bSourceControlled = tasks.IsSourceControlled();

	if (m_bSourceControlled)
	{
		m_bCheckedOut = MatchesOurSourceControlID(tasks.GetCheckOutTo());

		if (!m_bCheckedOut && m_pTDC->HasStyle(TDCS_CHECKOUTONLOAD))
			CheckOut(tasks, CString());
	}
	else
	{
		m_bCheckedOut = FALSE;
	}
}

void CTDCSourceControl::PrepareTasksForSave(CTaskFile& tasks) const
{
	if (m_bSourceControlled)
	{
		if (m_bCheckedOut)
			tasks.SetCheckedOutTo(GetSourceControlID());
		else
			tasks.SetCheckedOutTo(_T(""));
	}
	else
	{
		tasks.RemoveFromSourceControl();
	}
}

BOOL CTDCSourceControl::GetTasklistPath(CString& sTasklistPath) const
{
	sTasklistPath = m_pTDC->GetFilePath();
	
	return !sTasklistPath.IsEmpty();
}

CString CTDCSourceControl::GetSourceControlID() const
{
	if (m_pTDC->HasStyle(TDCS_INCLUDEUSERINCHECKOUT))
		return Misc::FormatComputerNameAndUser();

	// else
	return Misc::GetComputerName();
}

BOOL CTDCSourceControl::MatchesOurSourceControlID(const CString& sID) const
{
	if (sID.IsEmpty())
		return FALSE;

	if (SourceControlIDsMatch(sID, GetSourceControlID()))
		return TRUE;

	// Check if the naming option has been switched since our last check
	CString sReverseID;

	if (m_pTDC->HasStyle(TDCS_INCLUDEUSERINCHECKOUT))
		sReverseID = Misc::GetComputerName();
	else 
		sReverseID = Misc::FormatComputerNameAndUser();

	return SourceControlIDsMatch(sID, sReverseID);
}

BOOL CTDCSourceControl::SourceControlIDsMatch(const CString& sID1, const CString& sID2)
{
	return (sID1.CompareNoCase(sID2) == 0);
}

BOOL CTDCSourceControl::CanCheckOut() const
{
	return (m_bSourceControlled && !m_bCheckedOut);
}

TDC_FILE CTDCSourceControl::CheckOut(CTaskFile& tasks, CString& sCheckedOutTo, BOOL bForce, LPCTSTR szTasklistPath)
{
	if (!m_bSourceControlled)
	{
		// caller must think we're source controlled
		ASSERT(0);
		return TDCF_SSC_NOTSRCCONTROLLED;
	}

	if (m_pTDC->IsDelayLoaded())
	{
		return TDCF_SSC_DELAYLOADED;
	}

	if (m_bCheckedOut)
	{
		// caller must think we're checked in
		ASSERT(0);
		return TDCF_SUCCESS;
	}
	
	CString sTaskfilePath;

	if (!GetTasklistPath(sTaskfilePath))
	{
		if (FileMisc::FileExists(szTasklistPath) && !tasks.IsEmpty())
		{
			sTaskfilePath = szTasklistPath;
		}
		else
		{
			// Never been saved so it's ours!
			m_bCheckedOut = TRUE;
			return TDCF_SUCCESS;
		}
	}

	// Always backup before overwriting
	CTempFileBackup backup(sTaskfilePath);

	// snap shot mod time so we can restore it
	FILETIME ftMod = { 0 };
	VERIFY(FileMisc::GetFileLastModified(sTaskfilePath, ftMod));

	TDC_FILE nResult = TDCF_UNSET;
	COleDateTime dtLastMod = tasks.GetLastModifiedOle();

	// Open the tasklist WITHOUT decrypting
	// And hold it open until we're done to prevent any other access
	if (tasks.Open(sTaskfilePath, XF_READWRITE, FALSE))
	{
		// If the tasks are empty or they have a password but are not encrypted
		// we (re)load the tasks
		if (tasks.IsEmpty() || (!tasks.GetPassword().IsEmpty() && !tasks.IsEncrypted()))
			tasks.LoadEx();

		// Check if someone else already has it checked out
		sCheckedOutTo = tasks.GetCheckOutTo();

		if (!bForce && !sCheckedOutTo.IsEmpty())
		{
			ASSERT(!MatchesOurSourceControlID(sCheckedOutTo));
			nResult = TDCF_SSC_CHECKEDOUTBYOTHER;
		}
		else
		{
			// Set checkout info
			tasks.SetCheckedOutTo(GetSourceControlID());

			if (tasks.SaveEx())
			{
				m_bCheckedOut = TRUE;
				nResult = TDCF_SUCCESS;
			}
		}

		tasks.Close();
	}
	else
	{
		// Assume the file is locked by another user
		nResult = TDCF_SSC_CHECKEDOUTBYOTHER;
	}

	// Error handling
	if (nResult == TDCF_SUCCESS)
	{
		FileMisc::SetFileLastModified(sTaskfilePath, ftMod);

		// load tasks if they have changed
		if (tasks.GetLastModifiedOle() != dtLastMod)
		{
			tasks.Decrypt(m_pTDC->GetPassword());
			VERIFY(m_pTDC->LoadTasks(tasks));
		}

		m_pTDC->SetSourceControlStatus(TRUE, GetSourceControlID());
	}
	else
	{
		if (nResult == TDCF_UNSET)
			nResult = TDC::MapTaskfileError(tasks.GetLastFileError());

		VERIFY(backup.RestoreBackup());
	}

	return nResult;
}

TDC_FILE CTDCSourceControl::CheckIn()
{
	if (!m_bSourceControlled)
	{
		ASSERT(0);
		return TDCF_SSC_NOTSRCCONTROLLED;
	}
	else if (!m_bCheckedOut)
	{
		ASSERT(0);
		return TDCF_SSC_NOTCHECKEDOUT;
	}

	CString sTaskfilePath;

	if (!GetTasklistPath(sTaskfilePath))
	{
		m_bCheckedOut = FALSE;
		return TDCF_SUCCESS;
	}

	CTaskFile tasks(m_pTDC->m_sPassword);

	m_pTDC->Flush();
	m_pTDC->EndTimeTracking(TRUE, FALSE);
	m_pTDC->BuildTasksForSave(tasks);

	return CheckIn(tasks);
}

TDC_FILE CTDCSourceControl::CheckIn(CTaskFile& tasks)
{
	if (!m_bSourceControlled)
	{
		ASSERT(0);
		return TDCF_SSC_NOTSRCCONTROLLED;
	}
	else if (!m_bCheckedOut)
	{
		ASSERT(0);
		return TDCF_SSC_NOTCHECKEDOUT;
	}

	CString sTaskfilePath;

	if (!GetTasklistPath(sTaskfilePath) || tasks.IsEmpty())
	{
		ASSERT(0);
		return TDCF_OTHER;
	}

	// Must be already encrypted
	ASSERT(tasks.GetPassword().IsEmpty() || tasks.IsEncrypted());

	tasks.SetCheckedOutTo(_T(""));

	// snap shot mod time so we can restore it
	FILETIME ftMod = { 0 };
	VERIFY(FileMisc::GetFileLastModified(sTaskfilePath, ftMod));

	TDC_FILE nResult = CToDoCtrl::SaveTaskfile(tasks, sTaskfilePath);

	if (nResult == TDCF_SUCCESS)
	{
		m_bCheckedOut = FALSE;
		FileMisc::SetFileLastModified(sTaskfilePath, ftMod);

		m_pTDC->SetSourceControlStatus(TRUE, _T(""));
	}

	return nResult;
}

BOOL CTDCSourceControl::IsCheckedOut() const
{
	return (m_bSourceControlled && m_bCheckedOut);
}

BOOL CTDCSourceControl::IsSourceControlled() const 
{ 
	return m_bSourceControlled; 
}

BOOL CTDCSourceControl::CanAddToSourceControl(BOOL bAdd) const
{
	if (m_pTDC->m_bArchive)
		return FALSE;

	if ((bAdd && m_bSourceControlled) || (!bAdd && !m_bSourceControlled))
		return FALSE;

	if (!bAdd && !m_bCheckedOut)
		return FALSE;

	return TRUE;
}

TDC_FILE CTDCSourceControl::AddToSourceControl(BOOL bAdd)
{
	// Sanity checks
	if (m_pTDC->m_bArchive)
		return TDCF_SSC_ARCHIVE;

	if ((bAdd && m_bSourceControlled) || (!bAdd && !m_bSourceControlled))
	{
		ASSERT(0);
		return TDCF_SUCCESS;
	}

	if (!bAdd && !m_bCheckedOut)
		return TDCF_SSC_NOTCHECKEDOUT;

	CString sTaskfilePath;
	BOOL bHasFilePath = GetTasklistPath(sTaskfilePath);

	if (bAdd)
	{
		CString sCheckedOutTo = GetSourceControlID();

		if (bHasFilePath) // Already saved
		{
			CTaskFile tasks;
			m_pTDC->BuildTasksForSave(tasks);

			// auto-checkout
			tasks.SetCheckedOutTo(sCheckedOutTo); 

			TDC_FILE nResult = CToDoCtrl::SaveTaskfile(tasks, sTaskfilePath);

			if (nResult != TDCF_SUCCESS)			
			{
				ASSERT(0);
				return nResult;
			}
		}

		m_bSourceControlled = TRUE;
		m_bCheckedOut = TRUE;

		m_pTDC->SetSourceControlStatus(m_bSourceControlled, sCheckedOutTo);
	}
	else
	{
		if (bHasFilePath) // Already saved
		{
			CTaskFile tasks;
			m_pTDC->BuildTasksForSave(tasks);

			tasks.RemoveFromSourceControl();

			TDC_FILE nResult = CToDoCtrl::SaveTaskfile(tasks, sTaskfilePath);

			if (nResult != TDCF_SUCCESS)			
			{
				ASSERT(0);
				return nResult;
			}
		}

		m_bSourceControlled = FALSE;
		m_bCheckedOut = FALSE;

		m_pTDC->SetSourceControlStatus(FALSE, _T(""));
	}

	return TDCF_SUCCESS;
}


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

CTDCSourceControl::CTDCSourceControl(const CToDoCtrl& tdc) 
	: 
	m_tdc(tdc),
	m_bSourceControlled(FALSE),
	m_bCheckedOut(FALSE)
{
}

CTDCSourceControl::~CTDCSourceControl()
{
}

void CTDCSourceControl::InitialiseState(const CTaskFile& tasks)
{
	m_bSourceControlled = tasks.IsSourceControlled();

	if (m_bSourceControlled)
		m_bCheckedOut = MatchesOurSourceControlID(tasks.GetCheckOutTo());
	else
		m_bCheckedOut = FALSE;
}

void CTDCSourceControl::PrepareTasksForSave(CTaskFile& tasks) const
{
	CString sCheckedOutTo;
	
	if (m_bCheckedOut)
		sCheckedOutTo = GetSourceControlID();

	PrepareTasksForSave(tasks, m_bSourceControlled, sCheckedOutTo);
}

void CTDCSourceControl::PrepareTasksForSave(CTaskFile& tasks, BOOL bSourceControlled, LPCTSTR szCheckedOutTo)
{
	if (bSourceControlled)
		tasks.SetCheckedOutTo(szCheckedOutTo);
	else
		tasks.RemoveFromSourceControl();
}

BOOL CTDCSourceControl::GetTasklistPath(CString& sTasklistPath) const
{
	sTasklistPath = m_tdc.GetFilePath();
	
	return !sTasklistPath.IsEmpty();
}

CString CTDCSourceControl::GetSourceControlID() const
{
	if (m_tdc.HasStyle(TDCS_INCLUDEUSERINCHECKOUT))
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

	if (m_tdc.HasStyle(TDCS_INCLUDEUSERINCHECKOUT))
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

TDC_FILE CTDCSourceControl::PreCheckOut(CTaskFile& tasks, CString& sCheckedOutTo, BOOL bForce) const
{
	if (!m_bSourceControlled)
	{
		// caller must think we're source controlled
		ASSERT(0);
		return TDCF_SSC_NOTSRCCONTROLLED;
	}
	
	if (m_tdc.IsDelayLoaded())
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

	if (GetTasklistPath(sTaskfilePath))
	{
		if (tasks.IsEmpty() && !tasks.Load(sTaskfilePath, NULL, FALSE)) // don't decrypt
		{
			return m_tdc.MapTaskfileError(tasks.GetLastFileError());
		}

		sCheckedOutTo = tasks.GetCheckOutTo();

		if (!bForce && !sCheckedOutTo.IsEmpty())
		{
			ASSERT(!MatchesOurSourceControlID(sCheckedOutTo));
			return TDCF_OTHER;
		}
	}

	PrepareTasksForSave(tasks, TRUE, GetSourceControlID());

	return TDCF_SUCCESS;
}

TDC_FILE CTDCSourceControl::CheckOut(const CTaskFile& tasks)
{
	if (!m_bSourceControlled || m_tdc.IsDelayLoaded())
	{
		ASSERT(0);
		return TDCF_SSC_NOTSRCCONTROLLED;
	}

	CString sTaskfilePath;

	if (!GetTasklistPath(sTaskfilePath))
	{
		m_bCheckedOut = TRUE;
		return TDCF_SUCCESS;
	}
	
	// Taskfile must have been prepared correctly via PreCheckout
	if (!tasks.IsSourceControlled())
	{
		ASSERT(0);
		return TDCF_SSC_NOTSRCCONTROLLED;
	}

	CString sCheckedOutTo = tasks.GetCheckOutTo();

	if (!MatchesOurSourceControlID(sCheckedOutTo))
	{
		return TDCF_SSC_CHECKEDOUTBYOTHER;
	}

	// Must set this before calling CToDoCtrl::BuildTasksForSave
	m_bCheckedOut = TRUE;

	CTaskFile file;
	m_tdc.BuildTasksForSave(file, FALSE);

	TDC_FILE nResult = m_tdc.SaveTaskfile(file, sTaskfilePath);

	if (nResult != TDCF_SUCCESS)
		m_bCheckedOut = FALSE;

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

	// Set this before call to CToDoCtrl::BuildTasksForSave
	m_bCheckedOut = FALSE;

	CString sTaskfilePath;

	if (!GetTasklistPath(sTaskfilePath))
		return TDCF_SUCCESS;

	// snap shot mod time so we can restore it
	FILETIME ftMod = { 0 };
	VERIFY(FileMisc::GetFileLastModified(sTaskfilePath, ftMod));

	CTaskFile file(m_tdc.m_sPassword);
	m_tdc.BuildTasksForSave(file, FALSE);

	TDC_FILE nResult = m_tdc.SaveTaskfile(file, sTaskfilePath);

	if (nResult == TDCF_SUCCESS)
		FileMisc::SetFileLastModified(sTaskfilePath, ftMod);
	else
		m_bCheckedOut = TRUE; // failed

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
	if (m_tdc.m_bArchive)
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
	if (m_tdc.m_bArchive)
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
		if (bHasFilePath)
		{
			CTaskFile tasks;
			m_tdc.BuildTasksForSave(tasks, FALSE);

			tasks.SetCheckedOutTo(GetSourceControlID()); // auto-checkout

			TDC_FILE nResult = m_tdc.SaveTaskfile(tasks, sTaskfilePath);

			if (nResult != TDCF_SUCCESS)			
			{
				ASSERT(0);
				return nResult;
			}
		}

		m_bSourceControlled = TRUE;
		m_bCheckedOut = TRUE;
	}
	else
	{
		if (bHasFilePath)
		{
			CTaskFile tasks;
			m_tdc.BuildTasksForSave(tasks, FALSE);

			tasks.RemoveFromSourceControl();

			TDC_FILE nResult = m_tdc.SaveTaskfile(tasks, sTaskfilePath);

			if (nResult != TDCF_SUCCESS)			
			{
				ASSERT(0);
				return nResult;
			}
		}

		m_bSourceControlled = FALSE;
		m_bCheckedOut = FALSE;
	}

	return TDCF_SUCCESS;
}


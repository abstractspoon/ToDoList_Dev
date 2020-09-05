// RecurrenceEdit.cpp: implementation of the CRecurringTaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTaskDependencyEdit.h"
#include "resource.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\autoflag.h"
#include "..\Shared\enstring.h"
#include "..\Shared\localizer.h"
#include "..\Shared\winclasses.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDLTaskDependencyEdit::CTDLTaskDependencyEdit()
{
}

CTDLTaskDependencyEdit::~CTDLTaskDependencyEdit()
{

}

BEGIN_MESSAGE_MAP(CTDLTaskDependencyEdit, CEnEdit)
	//{{AFX_MSG_MAP(CTDLTaskDependencyEdit)
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChange)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////

void CTDLTaskDependencyEdit::PreSubclassWindow()
{
	CEnEdit::PreSubclassWindow();

	SetWindowText(m_aDepends.Format()); // for display purposes
}

int CTDLTaskDependencyEdit::Parse(CTDCDependencyArray& aDepends) const
{
	CString sDepends;
	GetWindowText(sDepends);

	Misc::Trim(sDepends);

	CStringArray aItems;
	Misc::Split(sDepends, aItems);

	aDepends.Set(aItems);

	return aDepends.GetSize();
}

BOOL CTDLTaskDependencyEdit::OnChange()
{
	CTDCDependencyArray aDepends;
	Parse(aDepends);

	if (m_aDepends.MatchAll(aDepends))
	{
		// No change
		return TRUE;
	}

	// Remove deleted dependencies
	int nDepend = m_aDepends.GetSize();

	while (nDepend--)
	{
		if (!aDepends.HasDependency(m_aDepends[nDepend]))
			m_aDepends.RemoveAt(nDepend);
	}

	// Add new dependencies
	nDepend = aDepends.GetSize();

	while (nDepend--)
	{
		if (!m_aDepends.HasDependency(aDepends[nDepend]))
			m_aDepends.Add(aDepends[nDepend]);
	}
	
	// Notify parent
	return FALSE; 
}

void CTDLTaskDependencyEdit::GetDependencies(CTDCDependencyArray& aDepends) const
{
	aDepends.Copy(m_aDepends);
}

void CTDLTaskDependencyEdit::SetDependencies(const CTDCDependencyArray& aDepends)
{
	m_aDepends.Copy(aDepends);

	if (GetSafeHwnd())
		SetWindowText(m_aDepends.Format()); // for display purposes
}

void CTDLTaskDependencyEdit::DDX(CDataExchange* pDX, CTDCDependencyArray& aValues)
{
	if (pDX->m_bSaveAndValidate)
		GetDependencies(aValues);
	else
		SetDependencies(aValues);
}


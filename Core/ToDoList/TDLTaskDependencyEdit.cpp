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

const int REBTN_EDIT = 1;

enum 
{
	DEPEND_COL,
	LEADIN_COL,
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDLTaskDependencyEdit::CTDLTaskDependencyEdit() : m_bNotifyingParent(FALSE)
{
	AddButton(REBTN_EDIT, _T("..."), CEnString(IDS_OPTIONS));
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
	if (!m_bNotifyingParent)
	{
		CTDCDependencyArray aDepends;
		Parse(aDepends);

		if (m_aDepends.MatchAll(aDepends, FALSE))
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
	}
	
	// Notify parent
	return FALSE; 
}

void CTDLTaskDependencyEdit::OnBtnClick(UINT nID)
{
	switch (nID)
	{
	case REBTN_EDIT:
		DoEdit();
		break;
	}
}

BOOL CTDLTaskDependencyEdit::DoEdit()
{
	if (IsWindowEnabled())
	{
		CTDLTaskDependencyEditDlg dialog(m_aDepends);
		
		if (dialog.DoModal() == IDOK)
		{
			CTDCDependencyArray aDepends;
			dialog.GetDependencies(aDepends);
			
			if (!aDepends.MatchAll(m_aDepends))
			{
				m_aDepends.Copy(aDepends);
				SetWindowText(m_aDepends.Format()); // for display purposes
				
				// notify parent
				CAutoFlag af(m_bNotifyingParent, TRUE);

				GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), EN_CHANGE), (LPARAM)GetSafeHwnd());
			}

			return TRUE;
		}
	}

	// all else
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

/////////////////////////////////////////////////////////////////////////////
// CTDLTaskDependencyOptionDlg dialog

CTDLTaskDependencyListCtrl::CTDLTaskDependencyListCtrl()
{
	SetMinItemHeight(18);
}

void CTDLTaskDependencyListCtrl::SetDependencies(const CTDCDependencyArray& aDepends)
{
	ASSERT(GetSafeHwnd());

	CRect rList;
	GetClientRect(rList);

	AddCol(CEnString(IDS_TDLBC_DEPENDS), ((rList.Width() * 2) / 3));
	AddCol(CEnString(IDS_DEPENDSLEADIN_COL), (rList.Width() / 3));
	SetAutoRowPrompt(CEnString(IDS_NEWDEPENDENCY_PROMPT));
	ShowGrid(TRUE, TRUE);
	AutoAdd(TRUE, FALSE);

	for (int nDepend = 0; nDepend < aDepends.GetSize(); nDepend++)
	{
		const TDCDEPENDENCY& depend = aDepends[nDepend];

		int nRow = AddRow(depend.Format(), nDepend);

		if (depend.IsLocal())
			SetItemText(nRow, LEADIN_COL, Misc::Format(depend.nDaysLeadIn));
	}
}

int CTDLTaskDependencyListCtrl::GetDependencies(CTDCDependencyArray& aDepends) const
{
	aDepends.RemoveAll();

	int nNumRows = (GetItemCount() - 1);
	TDCDEPENDENCY depend;

	for (int nRow = 0; nRow < nNumRows; nRow++)
	{
		if (depend.Parse(GetItemText(nRow, DEPEND_COL)))
		{
			if (depend.IsLocal())
				depend.nDaysLeadIn = _ttoi(GetItemText(nRow, LEADIN_COL));
			else
				depend.nDaysLeadIn = 0;

			aDepends.Add(depend);
		}
	}

	return aDepends.GetSize();
}

BOOL CTDLTaskDependencyListCtrl::CanEditCell(int nRow, int nCol) const
{
	switch (nCol)
	{
	case LEADIN_COL:
		return TDCDEPENDENCY(GetItemText(nRow, DEPEND_COL)).IsLocal();
	}

	// else
	return CInputListCtrl::CanEditCell(nRow, nCol);
}

COLORREF CTDLTaskDependencyListCtrl::GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	switch (nCol)
	{
	case LEADIN_COL:
		if (!IsPrompt(nItem) && !CanEditCell(nItem, nCol))
			return GetSysColor(COLOR_3DFACE);
		break;
	}

	// else
	return CInputListCtrl::GetItemBackColor(nItem, nCol, bSelected, bDropHighlighted, bWndFocus);
}

// -----------------------------------------------------

CTDLTaskDependencyEditDlg::CTDLTaskDependencyEditDlg(const CTDCDependencyArray& aDepends, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_TASKDEPENDENCY_DIALOG, _T("Dependency"), pParent)
{
	//{{AFX_DATA_INIT(CRecurringTaskOptionDlg)
	//}}AFX_DATA_INIT
	m_aDepends.Copy(aDepends);
}

void CTDLTaskDependencyEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CRecurringTaskOptionDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_DEPENDENCIES, m_lcDependencies);
}

BEGIN_MESSAGE_MAP(CTDLTaskDependencyEditDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CRecurringTaskOptionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecurringTaskOptionDlg message handlers
/////////////////////////////////////////////////////////////////////////////

BOOL CTDLTaskDependencyEditDlg::OnInitDialog()  
{
	CTDLDialog::OnInitDialog();

	m_lcDependencies.SetDependencies(m_aDepends);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLTaskDependencyEditDlg::OnOK()  
{
	CTDLDialog::OnOK();

	m_lcDependencies.GetDependencies(m_aDepends);
}

int CTDLTaskDependencyEditDlg::GetDependencies(CTDCDependencyArray& aDepends) const
{
	aDepends.Copy(m_aDepends);

	return aDepends.GetSize();
}


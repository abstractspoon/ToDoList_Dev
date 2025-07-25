// RecurrenceEdit.cpp: implementation of the CRecurringTaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTaskDependencyEdit.h"
#include "TaskFile.h"
#include "TDCImageList.h"
#include "resource.h"

#include "..\shared\autoflag.h"
#include "..\Shared\enstring.h"
#include "..\Shared\graphicsmisc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDLTaskDependencyEdit::CTDLTaskDependencyEdit() 
	: 
	CEnEdit(_T("0123456789, "), ME_LOCALIZESEPARATOR),
	m_bNotifyingParent(FALSE)
{
}

CTDLTaskDependencyEdit::~CTDLTaskDependencyEdit()
{

}

BEGIN_MESSAGE_MAP(CTDLTaskDependencyEdit, CEnEdit)
	//{{AFX_MSG_MAP(CTDLTaskDependencyEdit)
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChange)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_KILLFOCUS()

END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////

void CTDLTaskDependencyEdit::PreSubclassWindow()
{
	CEnEdit::PreSubclassWindow();

	SetWindowText(FormatDependencies());
}

BOOL CTDLTaskDependencyEdit::UpdateDepends()
{
	CTDCDependencyArray aDepends;
	Parse(aDepends);

	return UpdateDepends(aDepends);
}

BOOL CTDLTaskDependencyEdit::UpdateDepends(const CTDCDependencyArray& aDepends)
{
	if (m_aDepends.MatchAll(aDepends, FALSE))
		return FALSE;

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

	CAutoFlag af(m_bNotifyingParent, TRUE);
	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), EN_CHANGE), (LPARAM)GetSafeHwnd());

	return TRUE;
}

void CTDLTaskDependencyEdit::OnKillFocus(CWnd* pNewWnd)
{
	CMaskEdit::OnKillFocus(pNewWnd);

	UpdateDepends();
}

BOOL CTDLTaskDependencyEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->hwnd == *this)
	{
		switch (pMsg->message)
		{
		case WM_KEYDOWN:
			switch (pMsg->wParam)
			{
			case VK_RETURN:
				return UpdateDepends();
			}
		}
	}

	// else
	return CMaskEdit::PreTranslateMessage(pMsg);
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
	// If we're in the middle of notifying the parent we return FALSE
	// to allow the message to continue on
	if (m_bNotifyingParent)
		return FALSE;

	// In general we don't notify the parent as changes happen
	// except if all the dependencies have been deleted
	// Note: we only want to update m_aDepends if we allowing this
	// change through to the parent
	if (m_aDepends.GetSize())
	{
		CTDCDependencyArray aDepends;

		if (!Parse(aDepends))
		{
			m_aDepends.RemoveAll();
			return FALSE;
		}
	}

	// All else -> eat
	return TRUE;
}

BOOL CTDLTaskDependencyEdit::DoEdit(const CDWordArray& aDependentTaskIDs, const CTaskFile& tasks,
									const CTDCImageList& ilTasks, BOOL bShowParentsAsFolders, BOOL bShowLeadInTimes)
{
	if (IsWindowEnabled())
	{
		CTDLTaskDependencyEditDlg dialog(aDependentTaskIDs, tasks, ilTasks, m_aDepends, bShowParentsAsFolders, bShowLeadInTimes);
		
		if (dialog.DoModal() == IDOK)
		{
			CTDCDependencyArray aDepends;
			dialog.GetDependencies(aDepends);
			
			if (UpdateDepends(aDepends))
			{
				SetWindowText(FormatDependencies());
				return TRUE;
			}
		}
	}

	// all else
	return FALSE;
}

HBRUSH CTDLTaskDependencyEdit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	if (m_brCircular.GetSafeHandle())
	{
		pDC->SetTextColor(m_crCircularText);
		pDC->SetBkMode(TRANSPARENT);
	}

	return (HBRUSH)m_brCircular.GetSafeHandle();
}

int CTDLTaskDependencyEdit::GetDependencies(CTDCDependencyArray& aDepends) const
{
	aDepends.Copy(m_aDepends);
	return aDepends.GetSize();
}

void CTDLTaskDependencyEdit::SetDependencies(const CTDCDependencyArray& aDepends)
{
	m_aDepends.Copy(aDepends);

	if (GetSafeHwnd())
		SetWindowText(FormatDependencies());
}

CString CTDLTaskDependencyEdit::FormatDependencies(TCHAR cSep)
{
	return m_aDepends.Format(cSep);
}

void CTDLTaskDependencyEdit::SetDependenciesAreCircular(BOOL bCircular, COLORREF crCircular)
{
	if (bCircular && !m_brCircular.GetSafeHandle())
	{
		m_brCircular.CreateSolidBrush(crCircular);
		m_crCircularText = GraphicsMisc::GetBestTextColor(crCircular);
	}
	else if (!bCircular && m_brCircular.GetSafeHandle())
	{
		m_brCircular.DeleteObject();
		m_crCircularText = CLR_NONE;
	}
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

enum
{
	DEPEND_COL,
	LEADIN_COL,
};

const UINT COMBO_ID = 1001;

/////////////////////////////////////////////////////////////////////////////

CTDLTaskDependencyListCtrl::CTDLTaskDependencyListCtrl(const CDWordArray& aDependentTaskIDs, const CTaskFile& tasks, 
													   const CTDCImageList& ilTasks, BOOL bShowParentsAsFolders, BOOL bShowLeadInTimes)
	: 
	m_tasks(tasks),
	m_ilTasks(ilTasks),
	m_bShowParentTasksAsFolders(bShowParentsAsFolders),
	m_bShowLeadInTimes(bShowLeadInTimes)
{
	m_aDependentTaskIDs.Copy(aDependentTaskIDs);
	m_cbTasks.SetShowParentTasksAsFolders(bShowParentsAsFolders);
}

BEGIN_MESSAGE_MAP(CTDLTaskDependencyListCtrl, CInputListCtrl)
	//{{AFX_MSG_MAP(CTDLTaskDependencyListCtrl)
	//}}AFX_MSG_MAP
	ON_CBN_SELENDCANCEL(COMBO_ID, OnTaskComboCancel)
	ON_CBN_SELENDOK(COMBO_ID, OnTaskComboOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CTDLTaskDependencyListCtrl::SetDependencies(const CTDCDependencyArray& aDepends)
{
	ASSERT(GetSafeHwnd());

	CRect rList;
	GetClientRect(rList);
	
	if (m_bShowLeadInTimes)
	{
		AddCol(CEnString(IDS_TDLBC_DEPENDS), ((rList.Width() * 2) / 3), ILCT_COMBO);
		AddCol(CEnString(IDS_DEPENDSLEADIN_COL), (rList.Width() / 3));
	}
	else
	{
		AddCol(CEnString(IDS_TDLBC_DEPENDS), rList.Width(), ILCT_COMBO);
	}

	SetAutoRowPrompt(CEnString(IDS_NEWDEPENDENCY_PROMPT));
	ShowGrid(TRUE, TRUE);
	AutoAdd(TRUE, FALSE);

	// This gets around the constness
	ListView_SetImageList(*this, m_ilTasks, LVSIL_SMALL);

	CreateControl(m_cbTasks, COMBO_ID, CBS_DROPDOWN); // edit field and no sort

	for (int nDepend = 0; nDepend < aDepends.GetSize(); nDepend++)
	{
		const TDCDEPENDENCY& depend = aDepends[nDepend];

		if (depend.IsLocal())
		{
			HTASKITEM hTask = m_tasks.FindTask(depend.dwTaskID);
			CString sName;

			if (hTask)
				sName.Format(_T("%s (%ld)"), m_tasks.GetTaskTitle(hTask), depend.dwTaskID);
			else
				sName = Misc::Format(depend.dwTaskID);

			int nImage = m_ilTasks.GetImageIndex(m_tasks.GetTaskIcon(hTask));

			if ((nImage == -1) && m_bShowParentTasksAsFolders && m_tasks.IsTaskParent(hTask))
				nImage = 0;

			int nRow = AddRow(sName, nImage);

			SetItemData(nRow, depend.dwTaskID);
			SetItemText(nRow, LEADIN_COL, Misc::Format(depend.nDaysLeadIn));
		}
		else
		{
			int nRow = AddRow(depend.Format());
			SetItemText(nRow, LEADIN_COL, _T("0"));
		}
	}
}

int CTDLTaskDependencyListCtrl::GetDependencies(CTDCDependencyArray& aDepends) const
{
	aDepends.RemoveAll();

	int nNumRows = (GetItemCount() - 1);
	TDCDEPENDENCY depend;

	for (int nRow = 0; nRow < nNumRows; nRow++)
	{
		depend.dwTaskID = GetItemData(nRow);

		if (depend.dwTaskID) // local
		{
			depend.nDaysLeadIn = _ttoi(GetItemText(nRow, LEADIN_COL));
		}
		else if (depend.Parse(GetItemText(nRow, DEPEND_COL)))
		{
			depend.nDaysLeadIn = 0;
		}
		else
		{
			continue;
		}

		aDepends.Add(depend);
	}

	return aDepends.GetSize();
}

BOOL CTDLTaskDependencyListCtrl::CanEditCell(int nRow, int nCol) const
{
	switch (nCol)
	{
	case LEADIN_COL:
		return (GetItemData(nRow) != 0);
	}

	// else
	return CInputListCtrl::CanEditCell(nRow, nCol);
}

void CTDLTaskDependencyListCtrl::EditCell(int nItem, int nCol, BOOL bBtnClick)
{
	if (!CanEditCell(nItem, nCol))
		return;

	switch (nCol)
	{
	case DEPEND_COL:
		ShowControl(m_cbTasks, nItem, nCol, bBtnClick);
		break;

	default:
		CInputListCtrl::EditCell(nItem, nCol, bBtnClick);
		break;
	}
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

void CTDLTaskDependencyListCtrl::PrepareControl(CWnd& ctrl, int nRow, int nCol)
{
	UNREFERENCED_PARAMETER(ctrl);

	switch (nCol)
	{
	case DEPEND_COL:
		{
			ASSERT(&ctrl == &m_cbTasks);

			PrepareTaskCombo(nRow);
		}
		break;

	case LEADIN_COL:
		{
			ASSERT(&ctrl == &m_editBox);

			m_editBox.SetMask(_T("-0123456789"));
		}
		break;
	}
}

void CTDLTaskDependencyListCtrl::OnTaskComboCancel()
{
	m_cbTasks.ShowWindow(SW_HIDE);
	SetFocus();
}

void CTDLTaskDependencyListCtrl::OnTaskComboOK()
{
	m_cbTasks.ShowWindow(SW_HIDE);

	int nRow = GetCurSel();
	CString sTask = m_cbTasks.GetSelectedTaskName();

	if (IsPrompt(nRow))
	{
		nRow = AddRow(sTask);
		SetItemText(nRow, LEADIN_COL, _T("0"));
	}
	else
	{
		SetItemText(nRow, DEPEND_COL, sTask);
	}

	SetItemData(nRow, m_cbTasks.GetSelectedTaskID(TRUE));
	SetItemImage(nRow, m_cbTasks.GetSelectedTaskImage());
	SetFocus();
}

void CTDLTaskDependencyListCtrl::PrepareTaskCombo(int nRow)
{
	// Populate once only
	if (m_cbTasks.GetCount() == 0)
	{
		m_cbTasks.Populate(m_tasks, m_ilTasks);
		CDialogHelper::RefreshMaxDropWidth(m_cbTasks);
	}

	// Prevent dependencies on self and disable all 'other' 
	// dependencies to prevent duplicate selection
	int nLVItem = (GetItemCount() - 1);

	if ((nLVItem > 1) || m_aDependentTaskIDs.GetSize())
	{
		CMap<DWORD, DWORD, int, int&> mapCBItems;
		CDialogHelper::BuildItemDataMapT(m_cbTasks, mapCBItems);

		while (nLVItem--)
		{
			int nCBTask = -1;
			DWORD dwDependID = GetItemData(nLVItem);

			if (dwDependID && mapCBItems.Lookup(dwDependID, nCBTask))
				m_cbTasks.SetDisabledItem(nCBTask, (nRow != nLVItem));
		}

		int nID = m_aDependentTaskIDs.GetSize();

		while (nID--)
		{
			int nCBTask = -1;

			if (mapCBItems.Lookup(m_aDependentTaskIDs[nID], nCBTask))
				m_cbTasks.SetDisabledItem(nCBTask, TRUE);
		}
	}

	m_cbTasks.SetSelectedTaskID(GetItemData(nRow));
}

/////////////////////////////////////////////////////////////////////////////

CTDLTaskDependencyEditDlg::CTDLTaskDependencyEditDlg(const CDWordArray& aDependentTaskIDs, const CTaskFile& tasks, const CTDCImageList& ilTasks,
													 const CTDCDependencyArray& aDepends, BOOL bShowParentsAsFolders, 
													 BOOL bShowLeadInTimes, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_TASKDEPENDENCY_DIALOG, _T("Dependency"), pParent),
	m_lcDependencies(aDependentTaskIDs, tasks, ilTasks, bShowParentsAsFolders, bShowLeadInTimes)
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


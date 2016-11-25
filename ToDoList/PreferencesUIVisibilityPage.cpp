// PreferencesUIVisibilityPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesUIVisibilityPage.h"

#include "..\Shared\DialogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencesUIVisibilityPage property page

IMPLEMENT_DYNCREATE(CPreferencesUIVisibilityPage, CPreferencesPageBase)

CPreferencesUIVisibilityPage::CPreferencesUIVisibilityPage() 
	: 
	CPreferencesPageBase(IDD_PREFUIVISIBILITY_PAGE)
{
	//{{AFX_DATA_INIT(CPreferencesUIVisibilityPage)
	//}}AFX_DATA_INIT
	m_nAttribShow = TDLSA_ASCOLUMN;
}

CPreferencesUIVisibilityPage::~CPreferencesUIVisibilityPage()
{
}

void CPreferencesUIVisibilityPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesUIVisibilityPage)
	DDX_Control(pDX, IDC_VISIBILITY, m_lcVisibility);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_SHOWALLATTRIB, (int&)m_nAttribShow);
}


BEGIN_MESSAGE_MAP(CPreferencesUIVisibilityPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesUIVisibilityPage)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_SHOWALLATTRIB, OnChangeAttribShow)
	ON_BN_CLICKED(IDC_SHOWANYATTRIB, OnChangeAttribShow)
	ON_BN_CLICKED(IDC_SHOWATTRIBASCOLUMN, OnChangeAttribShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesUIVisibilityPage message handlers

BOOL CPreferencesUIVisibilityPage::OnInitDialog()
{
	CPreferencesPageBase::OnInitDialog();

	m_lcVisibility.SetCurSel(0, 0);

	return TRUE;
}

void CPreferencesUIVisibilityPage::GetColumnAttributeVisibility(TDCCOLEDITFILTERVISIBILITY& vis) const
{
	m_lcVisibility.GetVisibility(vis);
}

void CPreferencesUIVisibilityPage::SetColumnAttributeVisibility(const TDCCOLEDITFILTERVISIBILITY& vis)
{
	m_lcVisibility.SetVisibility(vis);
}

void CPreferencesUIVisibilityPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	TDCCOLEDITFILTERVISIBILITY vis;

	if (!vis.Load(pPrefs, szKey))
	{
		// first time OR backwards compatibility
		CTDCColumnIDMap mapColumns;

		mapColumns.AddColumn(TDCC_ID);
		mapColumns.AddColumn(TDCC_PRIORITY);
		mapColumns.AddColumn(TDCC_PERCENT);
		mapColumns.AddColumn(TDCC_TIMEEST);
		mapColumns.AddColumn(TDCC_TIMESPENT);
		mapColumns.AddColumn(TDCC_TRACKTIME);
		mapColumns.AddColumn(TDCC_DUEDATE);
		mapColumns.AddColumn(TDCC_ALLOCTO);
		mapColumns.AddColumn(TDCC_STATUS);
		mapColumns.AddColumn(TDCC_CATEGORY);
		mapColumns.AddColumn(TDCC_FILEREF);
		mapColumns.AddColumn(TDCC_RECURRENCE);
		mapColumns.AddColumn(TDCC_REMINDER);
		mapColumns.AddColumn(TDCC_TAGS);
	
		vis.SetVisibleColumns(mapColumns);

		if (pPrefs->GetProfileInt(szKey, _T("ShowSubtaskCompletion"), FALSE))
			vis.SetColumnVisible(TDCC_SUBTASKDONE);

		BOOL bShowCtrlsAsColumns = pPrefs->GetProfileInt(szKey, _T("ShowCtrlsAsColumns"), FALSE);

		vis.SetShowEditsAndFilters(bShowCtrlsAsColumns ? TDLSA_ASCOLUMN : TDLSA_ALL);

		// if any time field is hidden we must enable 'any' attribute
		// and remove those fields
		BOOL bHideDueTimeField = pPrefs->GetProfileInt(szKey, _T("HideDueTimeField"), FALSE);
		BOOL bHideStartTimeField = pPrefs->GetProfileInt(szKey, _T("HideStartTimeField"), FALSE);
		BOOL bHideDoneTimeField = pPrefs->GetProfileInt(szKey, _T("HideDoneTimeField"), FALSE);

		if (bHideDoneTimeField || bHideDueTimeField || bHideStartTimeField)
		{
			vis.SetShowEditsAndFilters(TDLSA_ANY);

			if (bHideStartTimeField)
				vis.SetEditFieldVisible(TDCA_STARTTIME, FALSE);

			if (bHideDueTimeField)
				vis.SetEditFieldVisible(TDCA_DUETIME, FALSE);

			if (bHideDoneTimeField)
				vis.SetEditFieldVisible(TDCA_DONETIME, FALSE);
		}
	}

	m_lcVisibility.SetVisibility(vis);
	m_nAttribShow = vis.GetShowEditsAndFilters();
}

void CPreferencesUIVisibilityPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	TDCCOLEDITFILTERVISIBILITY vis;
	m_lcVisibility.GetVisibility(vis);

	vis.Save(pPrefs, szKey);
}

void CPreferencesUIVisibilityPage::OnSize(UINT nType, int cx, int cy) 
{
	CPreferencesPageBase::OnSize(nType, cx, cy);
	
	if (m_lcVisibility.GetSafeHwnd())
	{
		// calculate border
		CPoint ptBorders = CDialogHelper::GetCtrlRect(this, IDC_LISTLABEL).TopLeft();

		// calc offsets
		CRect rVis = CDialogHelper::GetCtrlRect(this, IDC_ATTRIBGROUP);

		int nXOffset = (cx - rVis.right - ptBorders.x);
		int nYOffset = (cy - rVis.bottom - ptBorders.y);

		CDialogHelper::OffsetCtrl(this, IDC_ATTRIBGROUP, 0, nYOffset);
		CDialogHelper::OffsetCtrl(this, IDC_SHOWALLATTRIB, 0, nYOffset);
		CDialogHelper::OffsetCtrl(this, IDC_SHOWATTRIBASCOLUMN, 0, nYOffset);
		CDialogHelper::OffsetCtrl(this, IDC_SHOWANYATTRIB, 0, nYOffset);

		CDialogHelper::ResizeCtrl(this, IDC_ATTRIBGROUP, nXOffset, 0);
		CDialogHelper::ResizeChild(&m_lcVisibility, nXOffset, nYOffset);
	}
}

void CPreferencesUIVisibilityPage::OnChangeAttribShow() 
{
	UpdateData();

	m_lcVisibility.SetShowEditsAndFilters(m_nAttribShow);
}

// GanttPreferencesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "GanttPreferencesDlg.h"
#include "GanttMsg.h"
#include "Ganttstatic.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\enstring.h"
#include "..\shared\misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const COLORREF DEF_PARENTCOLOR			= RGB(0, 0, 0);
const COLORREF DEF_DEFAULTCOLOR			= RGB(70, 135, 245);

/////////////////////////////////////////////////////////////////////////////
// CGanttPreferencesPage dialog

CGanttPreferencesPage::CGanttPreferencesPage(CWnd* /*pParent*/ /*=NULL*/)
	: CPreferencesPageBase(IDD_PREFERENCES_PAGE)
{
	//{{AFX_DATA_INIT(CGanttPreferencesPage)
	//}}AFX_DATA_INIT
	m_crParent = DEF_PARENTCOLOR;
	m_crDefault = DEF_DEFAULTCOLOR;

	m_aColumnVis.SetSize(NUM_TREECOLUMNS + 1);
}

void CGanttPreferencesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CGanttPreferencesPage)
	DDX_Check(pDX, IDC_DISPLAYALLOCTO, m_bDisplayTrailingAllocTo);
	DDX_Check(pDX, IDC_DISPLAYTASKTITLE, m_bDisplayTrailingTaskTitle);
	DDX_Check(pDX, IDC_AUTOSCROLLSELECTION, m_bAutoScrollSelection);
	DDX_Check(pDX, IDC_DEFAULTCOLOR, m_bSpecifyDefaultColor);
	DDX_Check(pDX, IDC_CALCULATEPARENTDATES, m_bAutoCalcParentDates);
	DDX_Check(pDX, IDC_CALCULATEMISSINGSTARTDATE, m_bCalculateMissingStartDates);
	DDX_Check(pDX, IDC_CALCULATEMISSINGDUEDATE, m_bCalculateMissingDueDates);
	DDX_Radio(pDX, IDC_DEFAULTPARENTCOLORS, m_nParentColoring);
	DDX_Check(pDX, IDC_USETAGFORMILESTONE, m_bUseTagForMilestone);
	DDX_Text(pDX, IDC_MILESTONETAG, m_sMilestoneTag);
	DDX_Check(pDX, IDC_DISPLAYPROGRESS, m_bDisplayProgressInBar);
	DDX_Check(pDX, IDC_DECADESAREONEBASED, m_bDecadesAreOneBased);
	DDX_Check(pDX, IDC_DISPLAYPARENTSASROLLUPS, m_bDisplayParentsAsRollups);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SETPARENTCOLOR, m_btParentColor);
	DDX_Control(pDX, IDC_SETDEFAULTCOLOR, m_btDefaultColor);
	DDX_Control(pDX, IDC_COLUMNVISIBILITY, m_lbColumnVisibility);
}


BEGIN_MESSAGE_MAP(CGanttPreferencesPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CGanttPreferencesPage)
	ON_BN_CLICKED(IDC_DEFAULTCOLOR, OnDefaultcolor)
	ON_BN_CLICKED(IDC_SETDEFAULTCOLOR, OnSetDefaultcolor)
	ON_BN_CLICKED(IDC_DEFAULTPARENTCOLORS, OnChangeParentColoring)
	ON_BN_CLICKED(IDC_USETAGFORMILESTONE, OnUseTagForMilestone)
	ON_BN_CLICKED(IDC_NOPARENTCOLOR, OnChangeParentColoring)
	ON_BN_CLICKED(IDC_SETPARENTCOLOR, OnSetParentColor)
	ON_BN_CLICKED(IDC_SPECIFYPARENTCOLOR, OnChangeParentColoring)
	ON_BN_CLICKED(IDC_CALCULATEPARENTDATES, OnCalculateParentDates)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGanttPreferencesPage message handlers

COLORREF CGanttPreferencesPage::GetDefaultColor() const 
{ 
	return m_bSpecifyDefaultColor ? m_crDefault : CLR_NONE; 
}

void CGanttPreferencesPage::OnSetDefaultcolor() 
{
	m_crDefault = m_btDefaultColor.GetColor();
	Invalidate();
}

void CGanttPreferencesPage::OnDefaultcolor() 
{
	UpdateData();
	m_btDefaultColor.EnableWindow(m_bSpecifyDefaultColor);
}

BOOL CGanttPreferencesPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();
	
	m_mgrGroupLines.AddGroupLine(IDC_COLORSGROUP, *this);
	m_mgrGroupLines.AddGroupLine(IDC_DATESGROUP, *this);

	m_btDefaultColor.EnableWindow(m_bSpecifyDefaultColor);
	m_btParentColor.EnableWindow(m_nParentColoring == 2);

	m_btDefaultColor.SetColor(m_crDefault);
	m_btParentColor.SetColor(m_crParent);

	for (int nCol = 0; nCol < NUM_TREECOLUMNS; nCol++)
	{
		const GANTTCOLUMN& colVis = GANTTTREECOLUMNS[nCol];
		
		int nItem = CDialogHelper::AddString(m_lbColumnVisibility, colVis.nIDAttribName, colVis.nColID);
		m_lbColumnVisibility.SetCheck(nItem, m_aColumnVis[colVis.nColID]);
	}

	GetDlgItem(IDC_MILESTONETAG)->EnableWindow(m_bUseTagForMilestone);
	GetDlgItem(IDC_DISPLAYPARENTSASROLLUPS)->EnableWindow(m_bAutoCalcParentDates);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGanttPreferencesPage::OnChangeParentColoring() 
{
	UpdateData();
	m_btParentColor.EnableWindow(m_nParentColoring == 2);
}

void CGanttPreferencesPage::OnCalculateParentDates() 
{
	UpdateData();

	GetDlgItem(IDC_DISPLAYPARENTSASROLLUPS)->EnableWindow(m_bAutoCalcParentDates);
}

void CGanttPreferencesPage::OnSetParentColor() 
{
	m_crParent = m_btParentColor.GetColor();
}

int CGanttPreferencesPage::GetParentColoring(COLORREF& crParent) const
{
	crParent = m_crParent;
	return m_nParentColoring;
}

void CGanttPreferencesPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	pPrefs->WriteProfileString(szKey, _T("MilestoneTag"), m_sMilestoneTag);

	pPrefs->WriteProfileInt(szKey, _T("UseTagForMilestone"), m_bUseTagForMilestone);
	pPrefs->WriteProfileInt(szKey, _T("DisplayAllocTo"), m_bDisplayTrailingAllocTo);
	pPrefs->WriteProfileInt(szKey, _T("DisplayTaskTitle"), m_bDisplayTrailingTaskTitle);
	pPrefs->WriteProfileInt(szKey, _T("AutoScrollSelection"), m_bAutoScrollSelection);
	pPrefs->WriteProfileInt(szKey, _T("AutoCalcParentDates"), m_bAutoCalcParentDates);
	pPrefs->WriteProfileInt(szKey, _T("CalculateMissingStartDates"), m_bCalculateMissingStartDates);
	pPrefs->WriteProfileInt(szKey, _T("CalculateMissingDueDates"), m_bCalculateMissingDueDates);
	pPrefs->WriteProfileInt(szKey, _T("SpecifyDefaultColor"), m_bSpecifyDefaultColor);
	pPrefs->WriteProfileInt(szKey, _T("DefaultColor"), (int)m_crDefault);
	pPrefs->WriteProfileInt(szKey, _T("ParentColoring"), m_nParentColoring);
	pPrefs->WriteProfileInt(szKey, _T("ParentColor"), (int)m_crParent);
	pPrefs->WriteProfileInt(szKey, _T("DisplayProgressInBar"), m_bDisplayProgressInBar);
	pPrefs->WriteProfileInt(szKey, _T("DecadesAreOneBased"), m_bDecadesAreOneBased);
	pPrefs->WriteProfileInt(szKey, _T("DisplayParentsAsRollups"), m_bDisplayParentsAsRollups);

	m_btParentColor.SavePreferences(pPrefs);
	m_btDefaultColor.SavePreferences(pPrefs);

	CString sColVis(szKey);
	sColVis += _T("\\ColumnVisibility");

	for (int nCol = 0; nCol < NUM_TREECOLUMNS; nCol++)
	{
		GTLC_COLUMN nColID = GANTTTREECOLUMNS[nCol].nColID;
		CString sCol(Misc::MakeKey(_T("Column%d"), nColID));

		pPrefs->WriteProfileInt(sColVis, sCol, m_aColumnVis[nColID]);
	}
}

void CGanttPreferencesPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey) 
{
	m_sMilestoneTag = pPrefs->GetProfileString(szKey, _T("MilestoneTag"), CEnString(_T("Milestone")));
	m_bUseTagForMilestone = pPrefs->GetProfileInt(szKey, _T("UseTagForMilestone"), TRUE);
	m_bDisplayTrailingAllocTo = pPrefs->GetProfileInt(szKey, _T("DisplayAllocTo"), TRUE);
	m_bDisplayTrailingTaskTitle = pPrefs->GetProfileInt(szKey, _T("DisplayTaskTitle"), FALSE);
	m_bAutoScrollSelection = pPrefs->GetProfileInt(szKey, _T("AutoScrollSelection"), TRUE);
	m_bAutoCalcParentDates = pPrefs->GetProfileInt(szKey, _T("AutoCalcParentDates"), TRUE);
	m_bCalculateMissingStartDates = pPrefs->GetProfileInt(szKey, _T("CalculateMissingStartDates"), TRUE);
	m_bCalculateMissingDueDates = pPrefs->GetProfileInt(szKey, _T("CalculateMissingDueDates"), TRUE);
	m_bSpecifyDefaultColor = pPrefs->GetProfileInt(szKey, _T("SpecifyDefaultColor"), FALSE);
	m_crDefault = (COLORREF)pPrefs->GetProfileInt(szKey, _T("DefaultColor"), DEF_DEFAULTCOLOR);
	m_crParent = (COLORREF)pPrefs->GetProfileInt(szKey, _T("ParentColor"), DEF_PARENTCOLOR);
	m_nParentColoring = pPrefs->GetProfileInt(szKey, _T("ParentColoring"), 0);
	m_bDisplayProgressInBar = pPrefs->GetProfileInt(szKey, _T("DisplayProgressInBar"), FALSE);
	m_bDecadesAreOneBased = pPrefs->GetProfileInt(szKey, _T("DecadesAreOneBased"), FALSE);
	m_bDisplayParentsAsRollups = pPrefs->GetProfileInt(szKey, _T("DisplayParentsAsRollups"), FALSE);

	CColourButton::LoadPreferences(pPrefs);

	CString sColVis(szKey);
	sColVis += _T("\\ColumnVisibility");

	for (int nCol = 0; nCol < NUM_TREECOLUMNS; nCol++)
	{
		GTLC_COLUMN nColID = GANTTTREECOLUMNS[nCol].nColID;
		CString sCol(Misc::MakeKey(_T("Column%d"), nColID));

		m_aColumnVis[nColID] = pPrefs->GetProfileInt(sColVis, sCol, GANTTTREECOLUMNS[nCol].bDefaultVis);
	}
}

void CGanttPreferencesPage::OnOK()
{
	for (int nCol = 0; nCol < NUM_TREECOLUMNS; nCol++)
	{
		GTLC_COLUMN nColID = GANTTTREECOLUMNS[nCol].nColID;
		m_aColumnVis[nColID] = m_lbColumnVisibility.GetCheckByData(nColID);
	}

	CPreferencesPageBase::OnOK();
}

void CGanttPreferencesPage::GetColumnVisibility(CDWordArray& aColumnVis) const
{
	aColumnVis.Copy(m_aColumnVis);
	aColumnVis[GTLCC_TITLE] = TRUE; // for completeness
}

void CGanttPreferencesPage::OnUseTagForMilestone() 
{
	UpdateData();
	GetDlgItem(IDC_MILESTONETAG)->EnableWindow(m_bUseTagForMilestone);
}

CString CGanttPreferencesPage::GetMilestoneTag() const
{
	return m_bUseTagForMilestone ? m_sMilestoneTag : _T("");
}

/////////////////////////////////////////////////////////////////////////////
// CGanttPreferencesDlg dialog

const UINT IDC_HELPBUTTON = 1001;

CGanttPreferencesDlg::CGanttPreferencesDlg(CWnd* pParent /*=NULL*/)
	: 
	CPreferencesDlgBase(IDD_PREFERENCES_DIALOG, IDC_PPHOST, IDR_GANTTCHART, IDI_HELP_BUTTON, pParent)
{
	//{{AFX_DATA_INIT(CGanttPreferencesDlg)
	//}}AFX_DATA_INIT

	m_ppHost.AddPage(&m_page);
}

BEGIN_MESSAGE_MAP(CGanttPreferencesDlg, CPreferencesDlgBase)
	//{{AFX_MSG_MAP(CGanttPreferencesPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CGanttPreferencesDlg::OnInitDialog() 
{
	CPreferencesDlgBase::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGanttPreferencesDlg::DoHelp()
{
	ASSERT(m_pParentWnd);
	
	if (m_pParentWnd)
		m_pParentWnd->SendMessage(WM_GTLC_PREFSHELP);
}


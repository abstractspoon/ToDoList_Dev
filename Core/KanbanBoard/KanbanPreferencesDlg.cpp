// KanbanPreferencesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanPreferencesDlg.h"
#include "Kanbanenum.h"
#include "KanbanMsg.h"

#include "..\shared\misc.h"
#include "..\shared\localizer.h"
#include "..\shared\Messagebox.h"

#include "..\3rdparty\XNamedColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKanbanPreferencesPage dialog

IMPLEMENT_DYNCREATE(CKanbanPreferencesPage, CPreferencesPageBase)

CKanbanPreferencesPage::CKanbanPreferencesPage(CWnd* /*pParent*/ /*=NULL*/)
	: 
	CPreferencesPageBase(IDD_PREFERENCES_PAGE), 
	m_nFixedAttrib(TDCA_STATUS),
	m_bSortSubtaskBelowParent(FALSE),
	m_bColorBarByPriority(FALSE),
	m_bIndentSubtasks(FALSE),
	m_bShowTaskColorAsBar(FALSE),
	m_bHideEmptyAttributeValues(TRUE),
	m_bSpecifyFullColor(TRUE),
	m_bAltKeyOverridesMaxCount(TRUE),
	m_crFullColumn(255),
	m_cbAttributes(TRUE) // include <none>
{
	//{{AFX_DATA_INIT(CKanbanPreferencesPage)
	//}}AFX_DATA_INIT
}

void CKanbanPreferencesPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CKanbanPreferencesPage)
	DDX_Check(pDX, IDC_SHOWTASKCOLORASBAR, m_bShowTaskColorAsBar);
	DDX_Control(pDX, IDC_DISPLAYATTRIBUTES, m_lbDisplayAttrib);
	DDX_Control(pDX, IDC_ATTRIBUTES, m_cbAttributes);
	DDX_Check(pDX, IDC_SORTSUBTASKSBELOWPARENT, m_bSortSubtaskBelowParent);
	DDX_Check(pDX, IDC_ALWAYSSHOWBACKLOG, m_bAlwaysShowBacklog);
	DDX_Control(pDX, IDC_COLUMNDEFS, m_lcFixedColumnDefs);
	DDX_Check(pDX, IDC_HIDEEMPTYATTRIBS, m_bHideEmptyAttributeValues);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_COLORBARBYPRIORITY, m_bColorBarByPriority);
	DDX_Check(pDX, IDC_INDENTSUBTASKS, m_bIndentSubtasks);
	DDX_Check(pDX, IDC_SPECFIFYFULLCOLOUR, m_bSpecifyFullColor);
	DDX_Check(pDX, IDC_ALTOVERRIDESMAXCOUNT, m_bAltKeyOverridesMaxCount);
	DDX_Control(pDX, IDC_SETFULLCOLOR, m_btFullColor);

	m_cbAttributes.DDX(pDX, m_nFixedAttrib, m_sFixedCustomAttribID);
	m_btFullColor.DDX(pDX, m_crFullColumn);
}

BEGIN_MESSAGE_MAP(CKanbanPreferencesPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CKanbanPreferencesPage)
	ON_CBN_SELENDOK(IDC_ATTRIBUTES, OnSelchangeAttribute)
	ON_BN_CLICKED(IDC_MOVECOL_DOWN, OnMoveFixedColDown)
	ON_BN_CLICKED(IDC_MOVECOL_UP, OnMoveFixedColUp)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_COLUMNDEFS, OnItemchangedColumndefs)
	ON_BN_CLICKED(IDC_FIXEDCOLUMNS, OnChangeColumnType)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SPECFIFYFULLCOLOUR, OnSpecifyFullColor)
	ON_BN_CLICKED(IDC_SETFULLCOLOR, OnSetFullColor)
	ON_BN_CLICKED(IDC_FIXEDCOLUMNS, OnSortSubtasksBelowParents)
	ON_BN_CLICKED(IDC_SHOWTASKCOLORASBAR, OnShowColorAsBar)
	ON_BN_CLICKED(IDC_POPULATECOLUMNS, OnPopulateFixedColumns)
	ON_COMMAND(IDC_SORTSUBTASKSBELOWPARENT, OnSortSubtasksBelowParents)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanbanPreferencesPage message handlers

BOOL CKanbanPreferencesPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();
	
	m_mgrGroupLines.AddGroupLine(IDC_COLUMNGROUP, *this);
	m_btFullColor.EnableWindow(m_bSpecifyFullColor);
	
	m_cbAttributes.SetCustomAttributeDefs(m_aCustAttribDefs);
	m_cbAttributes.SetSelectedAttribute(m_nFixedAttrib, m_sFixedCustomAttribID);

	EnableDisableControls();

	m_lcFixedColumnDefs.SetColumnDefinitions(m_aFixedColumnDefs);
	m_lcFixedColumnDefs.SetCurSel(0);

	UpdateFixedAttributeValueCombo();
	BuildDisplayAttributeListBox();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKanbanPreferencesPage::OnSpecifyFullColor()
{
	UpdateData();

	m_btFullColor.EnableWindow(m_bSpecifyFullColor);
}

void CKanbanPreferencesPage::OnSetFullColor()
{
	UpdateData();
}

TDC_ATTRIBUTE CKanbanPreferencesPage::GetFixedAttributeToTrack(CString& sCustomAttribID) const
{
	sCustomAttribID.Empty();

	if (!HasFixedColumns())
		return TDCA_NONE;

	if (IsCustomFixedAttribute())
	{
		sCustomAttribID = m_sFixedCustomAttribID;

		if (m_aCustAttribDefs.GetSize())
			return m_aCustAttribDefs.GetDefinitionID(m_sFixedCustomAttribID);

		// else
		return TDCA_CUSTOMATTRIB;
	}
	
	// else
	return m_nFixedAttrib;
}

BOOL CKanbanPreferencesPage::IsCustomFixedAttribute() const 
{ 
	if (!KBUtils::IsCustomAttribute(m_nFixedAttrib))
		return FALSE;

	ASSERT(!m_sFixedCustomAttribID.IsEmpty());
	return TRUE;
}

void CKanbanPreferencesPage::UpdateFixedAttributeValueCombo()
{
	if (m_nFixedAttrib != TDCA_NONE)
	{
		CString sAttribID = (IsCustomFixedAttribute() ? m_sFixedCustomAttribID : KBUtils::GetAttributeID(m_nFixedAttrib));
		ASSERT(!sAttribID.IsEmpty());

		CStringArray aValues;
		const CKanbanValueMap* pValues = m_mapAttribValues.GetMapping(sAttribID);

		if (pValues)
		{
			POSITION pos = pValues->GetStartPosition();
			CString sValueID, sValue;

			while (pos)
			{
				pValues->GetNextAssoc(pos, sValueID, sValue);

				// Omit backlog
				if (!sValue.IsEmpty())
					aValues.Add(sValue);
			}

			Misc::SortArray(aValues);
		}

		m_lcFixedColumnDefs.SetAttributeValues(aValues);
	}
}

void CKanbanPreferencesPage::OnOK()
{
	CPreferencesPageBase::OnOK();

	m_lcFixedColumnDefs.GetColumnDefinitions(m_aFixedColumnDefs);

	m_aDisplayAttrib.RemoveAll();
	
	int nNumItems = m_lbDisplayAttrib.GetCount();
	
	for (int nItem = 0; nItem < nNumItems; nItem++)
	{
		if (m_lbDisplayAttrib.GetCheck(nItem) != 0)
		{
			TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)m_lbDisplayAttrib.GetItemData(nItem);
			m_aDisplayAttrib.Add(nAttribID);
		}
	}
}

BOOL CKanbanPreferencesPage::HasFixedColumns() const 
{ 
	if (m_nFixedAttrib == TDCA_NONE)
		return FALSE;
	
	return (m_aFixedColumnDefs.GetSize() > 0); 
}

int CKanbanPreferencesPage::GetFixedColumnDefinitions(CKanbanColumnArray& aColumnDefs) const
{
	aColumnDefs.RemoveAll();

	if (HasFixedColumns())
	{
		aColumnDefs.Copy(m_aFixedColumnDefs);

		// Fill in the attribute ID which is not known by the listctrl
		CString sAttribID = GetFixedAttributeID();
		int nCol = aColumnDefs.GetSize();

		while (nCol--)
			aColumnDefs[nCol].sAttribID = sAttribID;
	}

	return aColumnDefs.GetSize();
}

void CKanbanPreferencesPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	pPrefs->WriteProfileInt(szKey, _T("FixedAttribute"), m_nFixedAttrib);

	if (IsCustomFixedAttribute())
		pPrefs->WriteProfileString(szKey, _T("FixedCustomAttributeID"), m_sFixedCustomAttribID);
	else
		pPrefs->DeleteProfileEntry(szKey, _T("FixedCustomAttributeID"));

	pPrefs->WriteProfileInt(szKey, _T("AlwaysShowBacklog"), m_bAlwaysShowBacklog);
	pPrefs->WriteProfileInt(szKey, _T("SortSubtaskBelowParent"), m_bSortSubtaskBelowParent);
	pPrefs->WriteProfileInt(szKey, _T("ShowTaskColorAsBar"), m_bShowTaskColorAsBar);
	pPrefs->WriteProfileInt(szKey, _T("ColorBarByPriority"), m_bColorBarByPriority);
	pPrefs->WriteProfileInt(szKey, _T("IndentSubtasks"), m_bIndentSubtasks);
	pPrefs->WriteProfileInt(szKey, _T("HideEmptyAttributeValues"), m_bHideEmptyAttributeValues);
	pPrefs->WriteProfileInt(szKey, _T("SpecifyFullColumnColor"), m_bSpecifyFullColor);
	pPrefs->WriteProfileInt(szKey, _T("FullColumnColor"), m_crFullColumn);
	pPrefs->WriteProfileInt(szKey, _T("AltKeyOverridesMaxCount"), m_bAltKeyOverridesMaxCount);

	// column defs
	int nNumDefs = m_aFixedColumnDefs.GetSize();
	pPrefs->WriteProfileInt(szKey, _T("FixedColumnCount"), nNumDefs);

	for (int nDef = 0; nDef < nNumDefs; nDef++)
	{
		const KANBANCOLUMN& colDef = m_aFixedColumnDefs.GetData()[nDef];
		CString sColKey = Misc::MakeKey(_T("ColumnDef%d"), nDef, szKey);

		pPrefs->WriteProfileString(sColKey, _T("Title"), colDef.sTitle);
		pPrefs->WriteProfileString(sColKey, _T("Value"), Misc::FormatArray(colDef.aAttribValues));
 		pPrefs->WriteProfileInt(sColKey, _T("MaxItems"), colDef.nMaxTaskCount);
		pPrefs->WriteProfileInt(sColKey, _T("BkgndColor"), colDef.crBackground);
	}

	// Display Attributes
	int nNumAttrib = m_aDisplayAttrib.GetSize();
	for (int nAtt = 0; nAtt < nNumAttrib; nAtt++)
	{
		CString sEntry = Misc::MakeKey(_T("DisplayAttrib%d"), nAtt);
		pPrefs->WriteProfileInt(szKey, sEntry, m_aDisplayAttrib[nAtt]);
	}

	pPrefs->WriteProfileInt(szKey, _T("DisplayAttribCount"), nNumAttrib);
	pPrefs->WriteProfileInt(szKey, _T("AutoAddFlagAndParent"), FALSE);
}

void CKanbanPreferencesPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey) 
{
	m_nFixedAttrib = (TDC_ATTRIBUTE)pPrefs->GetProfileInt(szKey, _T("FixedAttribute"), TDCA_NONE);
	m_sFixedCustomAttribID = pPrefs->GetProfileString(szKey, _T("FixedCustomAttributeID"));

	m_bAlwaysShowBacklog = pPrefs->GetProfileInt(szKey, _T("AlwaysShowBacklog"), TRUE);
	m_bSortSubtaskBelowParent = pPrefs->GetProfileInt(szKey, _T("SortSubtaskBelowParent"), TRUE);
	m_bShowTaskColorAsBar = pPrefs->GetProfileInt(szKey, _T("ShowTaskColorAsBar"), FALSE);
	m_bColorBarByPriority = pPrefs->GetProfileInt(szKey, _T("ColorBarByPriority"), FALSE);
	m_bIndentSubtasks = pPrefs->GetProfileInt(szKey, _T("IndentSubtasks"), FALSE);
	m_bHideEmptyAttributeValues = pPrefs->GetProfileInt(szKey, _T("HideEmptyAttributeValues"), TRUE);
	m_bSpecifyFullColor = pPrefs->GetProfileInt(szKey, _T("SpecifyFullColumnColor"), TRUE);
	m_crFullColumn = pPrefs->GetProfileInt(szKey, _T("FullColumnColor"), 255);
	m_bAltKeyOverridesMaxCount = pPrefs->GetProfileInt(szKey, _T("AltKeyOverridesMaxCount"), TRUE);

	// column definitions
	m_aFixedColumnDefs.RemoveAll();
	int nNumDefs = pPrefs->GetProfileInt(szKey, _T("FixedColumnCount"));

	for (int nDef = 0; nDef < nNumDefs; nDef++)
	{
		KANBANCOLUMN colDef;
		CString sColKey = Misc::MakeKey(_T("ColumnDef%d"), nDef, szKey);
			
		colDef.sTitle = pPrefs->GetProfileString(sColKey, _T("Title"));
 		colDef.nMaxTaskCount = pPrefs->GetProfileInt(sColKey, _T("MaxItems"));
		colDef.crBackground = (COLORREF)pPrefs->GetProfileInt(sColKey, _T("BkgndColor"), CLR_NONE);

		// Validate color
		if (colDef.crBackground == GetSysColor(COLOR_WINDOW))
			colDef.crBackground = CLR_NONE;

		CString sValues = pPrefs->GetProfileString(sColKey, _T("Value"));

		if (sValues.IsEmpty())
			colDef.aAttribValues.Add(sValues);
		else
			Misc::Split(sValues, colDef.aAttribValues);

		m_aFixedColumnDefs.Add(colDef);
	}

	// Display Attributes
	m_aDisplayAttrib.RemoveAll();
	int nNumAttrib = pPrefs->GetProfileInt(szKey, _T("DisplayAttribCount"));
	
	for (int nAtt = 0; nAtt < nNumAttrib; nAtt++)
	{
		CString sEntry = Misc::MakeKey(_T("DisplayAttrib%d"), nAtt);
		TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)pPrefs->GetProfileInt(szKey, sEntry, TDCA_NONE);

		if (nAttribID != TDCA_NONE)
			m_aDisplayAttrib.Add(nAttribID);
	}	

	// Backwards compatibility
	if (pPrefs->GetProfileInt(szKey, _T("AutoAddFlagAndParent"), TRUE))
	{
		Misc::AddUniqueItemT(TDCA_FLAG, m_aDisplayAttrib);

		if (!Misc::IsHighContrastActive())
			Misc::AddUniqueItemT(TDCA_PARENT, m_aDisplayAttrib);
	}
}

void CKanbanPreferencesPage::OnChangeColumnType() 
{
	UpdateData();
	
	EnableDisableControls();
}

void CKanbanPreferencesPage::OnShowColorAsBar()
{
	UpdateData();
	
	EnableDisableControls();
}

void CKanbanPreferencesPage::EnableDisableControls()
{
	GetDlgItem(IDC_INDENTSUBTASKS)->EnableWindow(m_bSortSubtaskBelowParent);
	GetDlgItem(IDC_COLORBARBYPRIORITY)->EnableWindow(m_bShowTaskColorAsBar);

	m_lcFixedColumnDefs.EnableWindow(m_cbAttributes.GetSelectedAttribute() != TDCA_NONE);

	GetDlgItem(IDC_MOVECOL_DOWN)->EnableWindow(m_lcFixedColumnDefs.CanMoveSelectedColumnRow(FALSE));
	GetDlgItem(IDC_MOVECOL_UP)->EnableWindow(m_lcFixedColumnDefs.CanMoveSelectedColumnRow(TRUE));
	GetDlgItem(IDC_POPULATECOLUMNS)->EnableWindow(m_lcFixedColumnDefs.IsWindowEnabled());
}

void CKanbanPreferencesPage::OnSelchangeAttribute() 
{
	UpdateData();

	if ((m_lcFixedColumnDefs.GetItemCount() > 1) && 
		(CMessageBox::AfxShow(IDS_DELETEALLROWS, (MB_YESNO | MB_ICONQUESTION)) == IDYES))
	{
		m_lcFixedColumnDefs.DeleteAllItems(FALSE);
	}

	EnableDisableControls();
	UpdateFixedAttributeValueCombo();
}

void CKanbanPreferencesPage::OnSortSubtasksBelowParents()
{
	UpdateData();

	EnableDisableControls();
}

void CKanbanPreferencesPage::OnMoveFixedColDown() 
{
	VERIFY(m_lcFixedColumnDefs.MoveSelectedColumnRow(FALSE));
}

void CKanbanPreferencesPage::OnMoveFixedColUp() 
{
	VERIFY(m_lcFixedColumnDefs.MoveSelectedColumnRow(TRUE));
}

CString CKanbanPreferencesPage::GetFixedAttributeID() const
{
	if (IsCustomFixedAttribute())
	{
		ASSERT(!m_sFixedCustomAttribID.IsEmpty());
		return m_sFixedCustomAttribID;
	}
	
	// else
	return KBUtils::GetAttributeID(m_nFixedAttrib);
}

void CKanbanPreferencesPage::OnPopulateFixedColumns()
{
	m_aFixedColumnDefs.RemoveAll();

	CString sAttribID(GetFixedAttributeID());
	ASSERT(!sAttribID.IsEmpty());
	
	const CKanbanValueMap* pValues = m_mapAttribValues.GetMapping(sAttribID);
	
	if (pValues)
	{
		CStringArray aValues;

		if (pValues->GetValues(aValues))
		{
			Misc::SortArray(aValues);

			for (int nVal = 0; nVal < aValues.GetSize(); nVal++)
			{
				KANBANCOLUMN colDef;

				colDef.sAttribID = sAttribID;
				colDef.sTitle = aValues[nVal];
				colDef.aAttribValues.Add(colDef.sTitle);
				colDef.nMaxTaskCount = 0;

				if (colDef.sTitle.IsEmpty())
					colDef.sTitle = CEnString(IDS_BACKLOG);

				m_aFixedColumnDefs.Add(colDef);
			}
		}
	}
	
	m_lcFixedColumnDefs.SetColumnDefinitions(m_aFixedColumnDefs);
	m_lcFixedColumnDefs.SetCurSel(0);
}

void CKanbanPreferencesPage::OnItemchangedColumndefs(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	EnableDisableControls();
	
	*pResult = 0;
}

void CKanbanPreferencesPage::SetCustomAttributes(const CKanbanCustomAttributeDefinitionArray& aCustAttribDefs)
{
	m_aCustAttribDefs.Copy(aCustAttribDefs);
	m_cbAttributes.SetCustomAttributeDefs(aCustAttribDefs);

	// Check that the fixed column custom attribute is still available
	if (!m_sFixedCustomAttribID.IsEmpty())
	{
		m_nFixedAttrib = aCustAttribDefs.GetDefinitionID(m_sFixedCustomAttribID);

		if (m_nFixedAttrib == TDCA_NONE)
			m_sFixedCustomAttribID.Empty();

		if (m_cbAttributes.GetSafeHwnd())
		{
			m_cbAttributes.SetSelectedAttribute(m_nFixedAttrib, m_sFixedCustomAttribID);
			EnableDisableControls();
		}
	}
}

void CKanbanPreferencesPage::SetAttributeValues(const CKanbanAttributeValueMap& mapValues)
{
	CString sAttribID;
	CKanbanValueMap* pValues = NULL;
	POSITION pos = mapValues.GetStartPosition();

	while (pos)
	{
		mapValues.GetNextAssoc(pos, sAttribID, pValues);
		ASSERT(pValues);

		CKanbanValueMap* pCopyValues = m_mapAttribValues.GetAddMapping(sAttribID);
		ASSERT(pCopyValues);

		Misc::Copy(*pValues, *pCopyValues);
	}
}

void CKanbanPreferencesPage::BuildDisplayAttributeListBox()
{
	ASSERT(m_lbDisplayAttrib.GetSafeHwnd());

	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_ALLOCBY,		TDCA_ALLOCBY); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_ALLOCTO,		TDCA_ALLOCTO); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_CATEGORY,		TDCA_CATEGORY); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_COST,			TDCA_COST); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_CREATEDATE,	TDCA_CREATIONDATE);
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_CREATEDBY,		TDCA_CREATEDBY);
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_DONEDATE,		TDCA_DONEDATE); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_DUEDATE,		TDCA_DUEDATE); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_EXTERNALID,	TDCA_EXTERNALID); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_FLAG,			TDCA_FLAG); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_FILELINK,		TDCA_FILELINK); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_LASTMOD,		TDCA_LASTMODDATE); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_LOCK,			TDCA_LOCK); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_PERCENT,		TDCA_PERCENT); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_PRIORITY,		TDCA_PRIORITY); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_RECURRENCE,	TDCA_RECURRENCE); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_RISK,			TDCA_RISK); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_STARTDATE,		TDCA_STARTDATE);
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_STATUS,		TDCA_STATUS); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_TAGS,			TDCA_TAGS); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_TASKID,		TDCA_ID); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_TIMEEST,		TDCA_TIMEESTIMATE); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_TIMEREMAINING,	TDCA_TIMEREMAINING); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_TIMESPENT,		TDCA_TIMESPENT); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_VERSION,		TDCA_VERSION); 
	CDialogHelper::AddStringT(m_lbDisplayAttrib, IDS_DISPLAY_PARENT,		TDCA_PARENT); 

	int nItem = m_aDisplayAttrib.GetSize();
	
	while (nItem--)
	{
		int nIndex = CDialogHelper::FindItemByDataT(m_lbDisplayAttrib, m_aDisplayAttrib[nItem]);
		ASSERT(nIndex != LB_ERR);
		
		m_lbDisplayAttrib.SetCheck(nIndex, 1);
	}
}

int CKanbanPreferencesPage::GetDisplayAttributes(CKanbanAttributeArray& aAttrib) const
{
	aAttrib.Copy(m_aDisplayAttrib);
	
	return aAttrib.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// CKanbanPreferencesDlg dialog

const UINT IDC_HELPBUTTON = 1001;

CKanbanPreferencesDlg::CKanbanPreferencesDlg(CWnd* pParent /*=NULL*/)
	: 
	CPreferencesDlgBase(IDD_PREFERENCES_DIALOG, IDC_PPHOST, IDR_KANBAN, IDI_HELP_BUTTON, pParent)
{
	//{{AFX_DATA_INIT(CKanbanPreferencesDlg)
	//}}AFX_DATA_INIT

	m_ppHost.AddPage(&m_page);
}

BEGIN_MESSAGE_MAP(CKanbanPreferencesDlg, CPreferencesDlgBase)
	//{{AFX_MSG_MAP(CKanbanPreferencesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CKanbanPreferencesDlg::SetCustomAttributeDefs(const CKanbanCustomAttributeDefinitionArray& aCustomAttribDefs)
{
	m_page.SetCustomAttributes(aCustomAttribDefs);
}

void CKanbanPreferencesDlg::SetAttributeValues(const CKanbanAttributeValueMap& mapValues)
{
	m_page.SetAttributeValues(mapValues);
}

BOOL CKanbanPreferencesDlg::OnInitDialog() 
{
	CPreferencesDlgBase::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKanbanPreferencesDlg::DoHelp()
{
	ASSERT(m_pParentWnd);

	if (m_pParentWnd)
		m_pParentWnd->SendMessage(WM_KBC_PREFSHELP);
}

int CKanbanPreferencesDlg::GetDisplayAttributes(CKanbanAttributeArray& aAttrib, TDC_ATTRIBUTE nExclude) const
{
	if (m_page.GetDisplayAttributes(aAttrib) && (nExclude != TDCA_NONE))
		Misc::RemoveItemT(nExclude, aAttrib);

	return aAttrib.GetSize();
}

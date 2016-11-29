// PreferencesTaskDefPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesTaskDefPage.h"
#include "tdcenum.h"
#include "tdcmapping.h"
#include "todoitem.h"
#include "tdltaskicondlg.h"

#include "..\shared\enstring.h"
#include "..\shared\misc.h"
#include "..\shared\preferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const LPCTSTR ENDL = _T("\r\n");

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskDefPage property page

IMPLEMENT_DYNCREATE(CPreferencesTaskDefPage, CPreferencesPageBase)

CPreferencesTaskDefPage::CPreferencesTaskDefPage() 
	: 
	CPreferencesPageBase(CPreferencesTaskDefPage::IDD),
	m_cbDefReminder(TDLRPC_SHOWNONE | TDLRPC_SHOWZERO),
	m_nDefReminderLeadin(TDLRPC_NOREMINDER)
{
	//{{AFX_DATA_INIT(CPreferencesTaskDefPage)
	//}}AFX_DATA_INIT
	m_nSelAttribUse = -1;
	m_bReminderBeforeDue = TRUE;

	// attrib use
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_PRIORITY, TDCA_PRIORITY, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_RISK, TDCA_RISK, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_TIMEEST, TDCA_TIMEEST, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_ALLOCTO, TDCA_ALLOCTO, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_ALLOCBY, TDCA_ALLOCBY, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_STATUS, TDCA_STATUS, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_CATEGORY, TDCA_CATEGORY, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_PTDP_COLOR, TDCA_COLOR, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_PTDP_DUEDATE, TDCA_DUEDATE, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_PTDP_VERSION, TDCA_VERSION, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_STARTDATE, TDCA_STARTDATE, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_FLAG, TDCA_FLAG, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_EXTERNALID, TDCA_EXTERNALID, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_TAGS, TDCA_TAGS, -1)); 

	m_eCost.SetMask(_T(".0123456789"), ME_LOCALIZEDECIMAL);
}

CPreferencesTaskDefPage::~CPreferencesTaskDefPage()
{
}

void CPreferencesTaskDefPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesTaskDefPage)
	DDX_Control(pDX, IDC_DEFREMINDER, m_cbDefReminder);
	DDX_Control(pDX, IDC_DEFAULTRISK, m_cbDefRisk);
	DDX_Control(pDX, IDC_DEFAULTPRIORITY, m_cbDefPriority);
	DDX_Text(pDX, IDC_DEFAULTCREATEDBY, m_sDefCreatedBy);
	DDX_Text(pDX, IDC_DEFAULTCOST, m_dDefCost);
	DDX_Check(pDX, IDC_UPDATEINHERITATTRIB, m_bUpdateInheritAttributes);
	DDX_Text(pDX, IDC_CATEGORYLIST, m_sDefCategoryList);
	DDX_Text(pDX, IDC_STATUSLIST, m_sDefStatusList);
	DDX_Text(pDX, IDC_ALLOCTOLIST, m_sDefAllocToList);
	DDX_Text(pDX, IDC_ALLOCBYLIST, m_sDefAllocByList);
 	DDX_Text(pDX, IDC_VERSIONLIST, m_sDefVersionList);
 	DDX_Text(pDX, IDC_TAGLIST, m_sDefTagList);
	DDX_Check(pDX, IDC_CATLIST_READONLY, m_bCatListReadonly);
	DDX_Check(pDX, IDC_STATUSLIST_READONLY, m_bStatusListReadonly);
	DDX_Check(pDX, IDC_ALLOCTOLIST_READONLY, m_bAllocToListReadonly);
	DDX_Check(pDX, IDC_ALLOCBYLIST_READONLY, m_bAllocByListReadonly);
 	DDX_Check(pDX, IDC_VERSIONLIST_READONLY, m_bVersionListReadonly);
 	DDX_Check(pDX, IDC_TAGLIST_READONLY, m_bTagListReadonly);
	DDX_CBIndex(pDX, IDC_DEFREMINDERDATE, m_bReminderBeforeDue);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DEFAULTTIMESPENT, m_eTimeSpent);
	DDX_Control(pDX, IDC_DEFAULTTIMEEST, m_eTimeEst);
	DDX_Control(pDX, IDC_DEFAULTCOST, m_eCost);
	DDX_Control(pDX, IDC_SETDEFAULTCOLOR, m_btDefColor);
	DDX_Control(pDX, IDC_SETDEFAULTICON, m_btDefIcon);
	DDX_Control(pDX, IDC_INHERITATTRIBUTES, m_lbAttribUse);
	DDX_CBPriority(pDX, IDC_DEFAULTPRIORITY, m_nDefPriority);
	DDX_CBRisk(pDX, IDC_DEFAULTRISK, m_nDefRisk);
	DDX_Text(pDX, IDC_DEFAULTTIMEEST, m_dDefTimeEst);
	DDX_Text(pDX, IDC_DEFAULTTIMESPENT, m_dDefTimeSpent);
	DDX_Text(pDX, IDC_DEFAULTALLOCTO, m_sDefAllocTo);
	DDX_LBIndex(pDX, IDC_INHERITATTRIBUTES, m_nSelAttribUse);
	DDX_Text(pDX, IDC_DEFAULTALLOCBY, m_sDefAllocBy);
	DDX_Text(pDX, IDC_DEFAULTSTATUS, m_sDefStatus);
	DDX_Text(pDX, IDC_DEFAULTTAGS, m_sDefTags);
	DDX_Text(pDX, IDC_DEFAULTCATEGORY, m_sDefCategory);
	DDX_Check(pDX, IDC_USEPARENTATTRIB, m_bInheritParentAttributes);
	DDX_Check(pDX, IDC_USECREATIONFORDEFSTARTDATE, m_bUseCreationForDefStartDate);
	DDX_Check(pDX, IDC_USECREATIONFORDEFDUEDATE, m_bUseCreationForDefDueDate);

	if (pDX->m_bSaveAndValidate)
	{
		m_nDefReminderLeadin = m_cbDefReminder.GetSelectedPeriod();
	}
	else
	{
		m_cbDefReminder.SetSelectedPeriod(m_nDefReminderLeadin);
	}
}


BEGIN_MESSAGE_MAP(CPreferencesTaskDefPage, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesTaskDefPage)
	ON_CBN_SELCHANGE(IDC_DEFREMINDER, OnSelchangeReminder)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SETDEFAULTICON, OnSetdefaulticon)
	ON_BN_CLICKED(IDC_SETDEFAULTCOLOR, OnSetdefaultcolor)
	ON_BN_CLICKED(IDC_USEPARENTATTRIB, OnUseparentattrib)
	ON_CLBN_CHKCHANGE(IDC_INHERITATTRIBUTES, OnAttribUseChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskDefPage message handlers

BOOL CPreferencesTaskDefPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();
	
	m_mgrGroupLines.AddGroupLine(IDC_DEFGROUP, *this);
	m_mgrGroupLines.AddGroupLine(IDC_INHERITGROUP, *this);
	m_mgrGroupLines.AddGroupLine(IDC_DROPLISTGROUP, *this);

	GetDlgItem(IDC_INHERITATTRIBUTES)->EnableWindow(m_bInheritParentAttributes);
	GetDlgItem(IDC_UPDATEINHERITATTRIB)->EnableWindow(m_bInheritParentAttributes);
	GetDlgItem(IDC_DEFREMINDERDATE)->EnableWindow(m_nDefReminderLeadin != TDLRPC_NOREMINDER);

	m_btDefColor.SetColor(m_crDef);
	m_ilTaskIcons.LoadDefaultImages();

	int nIcon = m_ilTaskIcons.GetImageIndex(m_sDefIcon);
	m_btDefIcon.SetIcon((nIcon == -1) ? NULL : m_ilTaskIcons.ExtractIcon(nIcon));

	int nAttrib = m_aAttribPrefs.GetSize();
	
	while (nAttrib--)
	{
		int nIndex = m_lbAttribUse.InsertString(0, m_aAttribPrefs[nAttrib].sName);

		m_lbAttribUse.SetItemData(nIndex, m_aAttribPrefs[nAttrib].nAttrib);
		m_lbAttribUse.SetCheck(nIndex, m_aAttribPrefs[nAttrib].bUse ? 1 : 0);
	}

	// init edit prompts()
	m_mgrPrompts.SetEditPrompt(IDC_DEFAULTALLOCTO, *this, CEnString(IDS_PTDP_NAMEPROMPT));
	m_mgrPrompts.SetEditPrompt(IDC_DEFAULTALLOCBY, *this, CEnString(IDS_PTDP_NAMEPROMPT));
	m_mgrPrompts.SetEditPrompt(IDC_DEFAULTSTATUS, *this, CEnString(IDS_PTDP_STATUSPROMPT));
	m_mgrPrompts.SetEditPrompt(IDC_DEFAULTTAGS, *this, CEnString(IDS_PTDP_TAGSPROMPT));
	m_mgrPrompts.SetEditPrompt(IDC_DEFAULTCATEGORY, *this, CEnString(IDS_PTDP_CATEGORYPROMPT));
	m_mgrPrompts.SetEditPrompt(IDC_DEFAULTCREATEDBY, *this, CEnString(IDS_PTDP_NAMEPROMPT));
	m_mgrPrompts.SetComboEditPrompt(IDC_ALLOCTOLIST, *this, CEnString(IDS_PTDP_NAMEPROMPT));
	m_mgrPrompts.SetComboEditPrompt(IDC_ALLOCBYLIST, *this, CEnString(IDS_PTDP_NAMEPROMPT));
	m_mgrPrompts.SetComboEditPrompt(IDC_STATUSLIST, *this, CEnString(IDS_PTDP_STATUSPROMPT));
	m_mgrPrompts.SetComboEditPrompt(IDC_CATEGORYLIST, *this, CEnString(IDS_PTDP_CATEGORYPROMPT));
 	m_mgrPrompts.SetComboEditPrompt(IDC_VERSIONLIST, *this, CEnString(IDS_PTDP_VERSIONPROMPT));
 	m_mgrPrompts.SetComboEditPrompt(IDC_TAGLIST, *this, CEnString(IDS_PTDP_TAGSPROMPT));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPreferencesTaskDefPage::GetReminder(UINT& nMinutes, BOOL& bBeforeDue) const
{
	if (m_nDefReminderLeadin == TDLRPC_NOREMINDER)
		return FALSE;

	// else
	nMinutes = m_nDefReminderLeadin;
	bBeforeDue = m_bReminderBeforeDue;

	return TRUE;
}

void CPreferencesTaskDefPage::SetPriorityColors(const CDWordArray& aColors)
{
	m_cbDefPriority.SetColors(aColors);
}

void CPreferencesTaskDefPage::OnOK() 
{
	CPreferencesPageBase::OnOK();
}

void CPreferencesTaskDefPage::OnSetdefaultcolor() 
{
	m_crDef = m_btDefColor.GetColor();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesTaskDefPage::OnUseparentattrib() 
{
	UpdateData();

	GetDlgItem(IDC_INHERITATTRIBUTES)->EnableWindow(m_bInheritParentAttributes);
	GetDlgItem(IDC_UPDATEINHERITATTRIB)->EnableWindow(m_bInheritParentAttributes);

	CPreferencesPageBase::OnControlChange();
}

int CPreferencesTaskDefPage::GetListItems(TDC_ATTRIBUTE nList, CStringArray& aItems) const
{
	aItems.RemoveAll();

	// include default task attributes
	CString sDef;

	switch (nList)
	{
	case TDCA_ALLOCBY:	sDef = (m_sDefAllocByList + ENDL + m_sDefAllocBy); break;
	case TDCA_ALLOCTO:	sDef = (m_sDefAllocToList + ENDL + m_sDefAllocTo); break;
	case TDCA_STATUS:	sDef = (m_sDefStatusList + ENDL + m_sDefStatus); break; 
	case TDCA_CATEGORY:	sDef = (m_sDefCategoryList + ENDL + m_sDefCategory); break;
	case TDCA_VERSION:	sDef = (m_sDefVersionList/* + ENDL + m_sDefVersion*/); break; 
	case TDCA_TAGS:		sDef = (m_sDefTagList + ENDL + m_sDefTags); break;

	default: ASSERT(0);	return 0;
	}

	CStringArray aDef;
	Misc::Split(sDef, aDef, ENDL);
	Misc::AddUniqueItems(aDef, aItems);

	return aItems.GetSize();
}

BOOL CPreferencesTaskDefPage::GetListIsReadonly(TDC_ATTRIBUTE nList) const
{
	switch (nList)
	{
	case TDCA_ALLOCBY:	return m_bAllocByListReadonly;
	case TDCA_ALLOCTO:	return m_bAllocToListReadonly;
	case TDCA_STATUS:	return m_bStatusListReadonly; 
	case TDCA_CATEGORY:	return m_bCatListReadonly;
	case TDCA_VERSION:	return m_bVersionListReadonly;
	case TDCA_TAGS:		return m_bTagListReadonly;
	}

	// else
	ASSERT(0);	
	return TRUE;
}

CString* CPreferencesTaskDefPage::GetListText(TDC_ATTRIBUTE nList)
{
	CString* pList = NULL;

	switch (nList)
	{
	case TDCA_ALLOCBY:	pList = &m_sDefAllocByList; break;
	case TDCA_ALLOCTO:	pList = &m_sDefAllocToList; break;
	case TDCA_STATUS:	pList = &m_sDefStatusList; break;
	case TDCA_CATEGORY:	pList = &m_sDefCategoryList; break;
	case TDCA_VERSION:	pList = &m_sDefVersionList; break;
	case TDCA_TAGS:		pList = &m_sDefTagList; break;

	default: ASSERT(0);
	}

	return pList;
}

BOOL CPreferencesTaskDefPage::AddListItem(TDC_ATTRIBUTE nList, LPCTSTR szItem)
{
	UpdateData();

	CString* pList = GetListText(nList);

	if (pList)
	{
		// parse string into array
		CStringArray aItems;

		if (Misc::Split(*pList, aItems, ENDL))
		{
			// add to array
			if (Misc::AddUniqueItem(szItem, aItems))
			{
				// update edit control
				*pList = Misc::FormatArray(aItems, ENDL);
				UpdateData(FALSE);

				return TRUE;
			}
		}
	}

	return FALSE; // not unique
}

BOOL CPreferencesTaskDefPage::DeleteListItem(TDC_ATTRIBUTE nList, LPCTSTR szItem)
{
	UpdateData();

	CString* pList = GetListText(nList);

	if (pList)
	{
		// parse string into array
		CStringArray aItems;

		if (Misc::Split(*pList, aItems, ENDL))
		{
			// delete from array
			if (Misc::RemoveItem(szItem, aItems))
			{
				// update edit control
				*pList = Misc::FormatArray(aItems, ENDL);
				UpdateData(FALSE);

				return TRUE;
			}
		}
	}

	return FALSE; // not found
}

void CPreferencesTaskDefPage::OnAttribUseChange()
{
	UpdateData();

	ASSERT (m_nSelAttribUse >= 0);
	
	if (m_nSelAttribUse >= 0)
	{
		TDC_ATTRIBUTE nSelAttrib = (TDC_ATTRIBUTE)m_lbAttribUse.GetItemData(m_nSelAttribUse);

		// search for this item
		int nAttrib = m_aAttribPrefs.GetSize();
		
		while (nAttrib--)
		{
			if (m_aAttribPrefs[nAttrib].nAttrib == nSelAttrib)
			{
				m_aAttribPrefs[nAttrib].bUse = m_lbAttribUse.GetCheck(m_nSelAttribUse);
				break;
			}
		}
	}

	GetDlgItem(IDC_UPDATEINHERITATTRIB)->EnableWindow(m_bInheritParentAttributes);

	CPreferencesPageBase::OnControlChange();
}

BOOL CPreferencesTaskDefPage::HasCheckedAttributes() const
{
	int nAttrib = m_aAttribPrefs.GetSize();

	while (nAttrib--)
	{
		if (m_aAttribPrefs[nAttrib].bUse)
			return TRUE;
	}

	// else
	return FALSE;
}

int CPreferencesTaskDefPage::GetParentAttribsUsed(CTDCAttributeMap& mapAttribs, BOOL& bUpdateAttrib) const
{
	mapAttribs.RemoveAll();

	if (m_bInheritParentAttributes)
	{
		bUpdateAttrib = m_bUpdateInheritAttributes;
		int nIndex = (int)m_aAttribPrefs.GetSize();
		
		while (nIndex--)
		{
			if (m_aAttribPrefs[nIndex].bUse)
				mapAttribs.AddAttribute(m_aAttribPrefs[nIndex].nAttrib);
		}
	}
	else
		bUpdateAttrib = FALSE;

	return mapAttribs.GetCount();
}


void CPreferencesTaskDefPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	// load settings
	m_nDefPriority = pPrefs->GetProfileInt(szKey, _T("DefaultPriority"), 5); 
	m_nDefRisk = pPrefs->GetProfileInt(szKey, _T("DefaultRisk"), 0); 
	m_sDefAllocTo = pPrefs->GetProfileString(szKey, _T("DefaultAllocTo"));
	m_sDefAllocBy = pPrefs->GetProfileString(szKey, _T("DefaultAllocBy"));
	m_sDefStatus = pPrefs->GetProfileString(szKey, _T("DefaultStatus"));
	m_sDefTags = pPrefs->GetProfileString(szKey, _T("DefaultTags"));
	m_sDefCategory = pPrefs->GetProfileString(szKey, _T("DefaultCategory"));
	m_sDefCreatedBy = pPrefs->GetProfileString(szKey, _T("DefaultCreatedBy"), Misc::GetUserName());
	m_crDef = pPrefs->GetProfileInt(szKey, _T("DefaultColor"), 0);
	m_bInheritParentAttributes = pPrefs->GetProfileInt(szKey, _T("InheritParentAttributes"), pPrefs->GetProfileInt(szKey, _T("UseParentAttributes")));
	m_bUpdateInheritAttributes = pPrefs->GetProfileInt(szKey, _T("UpdateInheritAttributes"), FALSE);
	m_bUseCreationForDefStartDate = pPrefs->GetProfileInt(szKey, _T("UseCreationForDefStartDate"), TRUE);
	m_bUseCreationForDefDueDate = pPrefs->GetProfileInt(szKey, _T("UseCreationForDefDueDate"), FALSE);
	m_dDefCost = Misc::Atof(pPrefs->GetProfileString(szKey, _T("DefaultCost"), _T("0")));
	m_dDefTimeEst = pPrefs->GetProfileDouble(szKey, _T("DefaultTimeEstimate"), 0);
	m_eTimeEst.SetUnits((TH_UNITS)pPrefs->GetProfileInt(szKey, _T("DefaultTimeEstUnits"), THU_HOURS));
	m_dDefTimeSpent = pPrefs->GetProfileDouble(szKey, _T("DefaultTimeSpent"), 0);
	m_eTimeSpent.SetUnits((TH_UNITS)pPrefs->GetProfileInt(szKey, _T("DefaultTimeSpentUnits"), THU_HOURS));
	m_bCatListReadonly = pPrefs->GetProfileInt(szKey, _T("CatListReadonly"), FALSE);
	m_bStatusListReadonly = pPrefs->GetProfileInt(szKey, _T("StatusListReadonly"), FALSE);
	m_bAllocToListReadonly = pPrefs->GetProfileInt(szKey, _T("AllocToListReadonly"), FALSE);
	m_bAllocByListReadonly = pPrefs->GetProfileInt(szKey, _T("AllocByListReadonly"), FALSE);
	m_bVersionListReadonly = pPrefs->GetProfileInt(szKey, _T("VersionListReadonly"), FALSE);
	m_bTagListReadonly = pPrefs->GetProfileInt(szKey, _T("TagListReadonly"), FALSE);
	m_sDefIcon = pPrefs->GetProfileString(szKey, _T("DefaultIcon"));
	m_nDefReminderLeadin = pPrefs->GetProfileInt(szKey, _T("DefaultReminderLeadin"), TDLRPC_NOREMINDER);
	m_bReminderBeforeDue = pPrefs->GetProfileInt(szKey, _T("ReminderBeforeDue"), TRUE);

   // attribute use
	int nIndex = m_aAttribPrefs.GetSize();
	
	while (nIndex--)
	{
		CString sKey = Misc::MakeKey(_T("Attrib%d"), m_aAttribPrefs[nIndex].nAttrib);
		m_aAttribPrefs[nIndex].bUse = pPrefs->GetProfileInt(_T("Preferences\\AttribUse"), sKey, FALSE);
	}

	// combo lists
	m_sDefCategoryList = LoadDefaultListItems(pPrefs, _T("Preferences\\CategoryList"));
	m_sDefStatusList = LoadDefaultListItems(pPrefs, _T("Preferences\\StatusList"));
	m_sDefAllocToList = LoadDefaultListItems(pPrefs, _T("Preferences\\AllocToList"));
	m_sDefAllocByList = LoadDefaultListItems(pPrefs, _T("Preferences\\AllocByList"));
	m_sDefVersionList = LoadDefaultListItems(pPrefs, _T("Preferences\\VersionList"));
	m_sDefTagList = LoadDefaultListItems(pPrefs, _T("Preferences\\TagList"));
}

CString CPreferencesTaskDefPage::LoadDefaultListItems(const IPreferences* pPrefs, LPCTSTR szKey)
{
	CStringArray aItems;

	CPreferences::GetProfileArray(pPrefs, szKey, aItems);
	Misc::SortArray(aItems);

	return Misc::FormatArray(aItems, ENDL);
}

void CPreferencesTaskDefPage::SaveDefaultListItems(LPCTSTR szValueList, IPreferences* pPrefs, LPCTSTR szKey)
{
	CStringArray aItems;

	Misc::Split(szValueList, aItems, ENDL);
	CPreferences::WriteProfileArray(pPrefs, szKey, aItems);
}

void CPreferencesTaskDefPage::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	// save settings
	pPrefs->WriteProfileInt(szKey, _T("DefaultPriority"), m_nDefPriority);
	pPrefs->WriteProfileInt(szKey, _T("DefaultRisk"), m_nDefRisk);
	pPrefs->WriteProfileString(szKey, _T("DefaultAllocTo"), m_sDefAllocTo);
	pPrefs->WriteProfileString(szKey, _T("DefaultAllocBy"), m_sDefAllocBy);
	pPrefs->WriteProfileString(szKey, _T("DefaultStatus"), m_sDefStatus);
	pPrefs->WriteProfileString(szKey, _T("DefaultTags"), m_sDefTags);
	pPrefs->WriteProfileString(szKey, _T("DefaultCategory"), m_sDefCategory);
	pPrefs->WriteProfileString(szKey, _T("DefaultCreatedBy"), m_sDefCreatedBy);
	pPrefs->WriteProfileString(szKey, _T("DefaultIcon"), m_sDefIcon);
	pPrefs->WriteProfileInt(szKey, _T("DefaultColor"), m_crDef);
	pPrefs->WriteProfileInt(szKey, _T("InheritParentAttributes"), m_bInheritParentAttributes);
	pPrefs->WriteProfileInt(szKey, _T("UpdateInheritAttributes"), m_bUpdateInheritAttributes);
	pPrefs->WriteProfileInt(szKey, _T("UseCreationForDefStartDate"), m_bUseCreationForDefStartDate);
	pPrefs->WriteProfileInt(szKey, _T("UseCreationForDefDueDate"), m_bUseCreationForDefDueDate);
	pPrefs->WriteProfileDouble(szKey, _T("DefaultCost"), m_dDefCost);
	pPrefs->WriteProfileDouble(szKey, _T("DefaultTimeEstimate"), m_dDefTimeEst);
	pPrefs->WriteProfileInt(szKey, _T("DefaultTimeEstUnits"), m_eTimeEst.GetUnits());
	pPrefs->WriteProfileDouble(szKey, _T("DefaultTimeSpent"), m_dDefTimeSpent);
	pPrefs->WriteProfileInt(szKey, _T("DefaultTimeSpentUnits"), m_eTimeSpent.GetUnits());
	pPrefs->WriteProfileInt(szKey, _T("CatListReadonly"), m_bCatListReadonly);
	pPrefs->WriteProfileInt(szKey, _T("StatusListReadonly"), m_bStatusListReadonly);
	pPrefs->WriteProfileInt(szKey, _T("AllocToListReadonly"), m_bAllocToListReadonly);
	pPrefs->WriteProfileInt(szKey, _T("AllocByListReadonly"), m_bAllocByListReadonly);
	pPrefs->WriteProfileInt(szKey, _T("VersionListReadonly"), m_bVersionListReadonly);
	pPrefs->WriteProfileInt(szKey, _T("TagListReadonly"), m_bTagListReadonly);
	pPrefs->WriteProfileInt(szKey, _T("DefaultReminderLeadin"), m_nDefReminderLeadin);
	pPrefs->WriteProfileInt(szKey, _T("ReminderBeforeDue"), m_bReminderBeforeDue);
	
	// attribute usage
	int nIndex = m_aAttribPrefs.GetSize();

	while (nIndex--)
	{
		CString sKey = Misc::MakeKey(_T("Attrib%d"), m_aAttribPrefs[nIndex].nAttrib);
		pPrefs->WriteProfileInt(_T("Preferences\\AttribUse"), sKey, m_aAttribPrefs[nIndex].bUse);
	}

	// combo lists
	SaveDefaultListItems(m_sDefCategoryList, pPrefs, _T("Preferences\\CategoryList"));
	SaveDefaultListItems(m_sDefStatusList, pPrefs, _T("Preferences\\StatusList"));
	SaveDefaultListItems(m_sDefAllocToList, pPrefs, _T("Preferences\\AllocToList"));
	SaveDefaultListItems(m_sDefAllocByList, pPrefs, _T("Preferences\\AllocByList"));
	SaveDefaultListItems(m_sDefVersionList, pPrefs, _T("Preferences\\VersionList"));
	SaveDefaultListItems(m_sDefTagList, pPrefs, _T("Preferences\\TagList"));
}

void CPreferencesTaskDefPage::GetTaskAttributes(TODOITEM& tdiDefault) const
{
	tdiDefault.sTitle = CEnString(IDS_TASK);
	tdiDefault.color = m_crDef;
	tdiDefault.sAllocBy = m_sDefAllocBy;
	tdiDefault.sStatus = m_sDefStatus;
	tdiDefault.dTimeEstimate = m_dDefTimeEst;
	tdiDefault.dTimeSpent = m_dDefTimeSpent;
	tdiDefault.nTimeEstUnits = TDC::MapTHUnitsToUnits(m_eTimeEst.GetUnits());
	tdiDefault.nTimeSpentUnits = TDC::MapTHUnitsToUnits(m_eTimeSpent.GetUnits());
	tdiDefault.dCost = m_dDefCost;
	tdiDefault.nPriority = m_nDefPriority;
	tdiDefault.nRisk = m_nDefRisk;
	tdiDefault.sCreatedBy = m_sDefCreatedBy;
	tdiDefault.sIcon = m_sDefIcon;
	
	tdiDefault.dateStart.m_dt = (m_bUseCreationForDefStartDate ? -1 : 0);
	tdiDefault.dateStart.m_status = COleDateTime::null;
	tdiDefault.dateDue.m_dt = (m_bUseCreationForDefDueDate ? -1 : 0);
	tdiDefault.dateDue.m_status = COleDateTime::null;
	
	Misc::Split(m_sDefCategory, tdiDefault.aCategories);
	Misc::Split(m_sDefAllocTo, tdiDefault.aAllocTo);
	Misc::Split(m_sDefTags, tdiDefault.aTags);
}


void CPreferencesTaskDefPage::OnSetdefaulticon() 
{
	CTDLTaskIconDlg dialog(m_ilTaskIcons, m_sDefIcon);

	if (dialog.DoModal() == IDOK)
	{
		m_sDefIcon = dialog.GetIconName();
		
		int nIcon = m_ilTaskIcons.GetImageIndex(m_sDefIcon);
		m_btDefIcon.SetIcon((nIcon == -1) ? NULL : m_ilTaskIcons.ExtractIcon(nIcon));
	}
}

void CPreferencesTaskDefPage::OnSelchangeReminder() 
{
	UpdateData();

	GetDlgItem(IDC_DEFREMINDERDATE)->EnableWindow(m_nDefReminderLeadin != TDLRPC_NOREMINDER);
}

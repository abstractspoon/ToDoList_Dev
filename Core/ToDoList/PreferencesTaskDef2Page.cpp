// PreferencesTaskDefPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesTaskDef2Page.h"
#include "tdcenum.h"
#include "tdcmapping.h"

#include "..\shared\enstring.h"
#include "..\shared\misc.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR ENDL = _T("\r\n");

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskDef2Page property page

IMPLEMENT_DYNCREATE(CPreferencesTaskDef2Page, CPreferencesPageBase)

CPreferencesTaskDef2Page::CPreferencesTaskDef2Page() 
	: 
	CPreferencesPageBase(CPreferencesTaskDef2Page::IDD)
{
	//{{AFX_DATA_INIT(CPreferencesTaskDef2Page)
	//}}AFX_DATA_INIT
	m_nSelAttribUse = -1;

	// attrib use
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_PRIORITY, TDCA_PRIORITY, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_RISK, TDCA_RISK, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_TIMEEST, TDCA_TIMEESTIMATE, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_ALLOCTO, TDCA_ALLOCTO, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_ALLOCBY, TDCA_ALLOCBY, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_STATUS, TDCA_STATUS, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_CATEGORY, TDCA_CATEGORY, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_PTDP_COLOR, TDCA_COLOR, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_PTDP_DUEDATE, TDCA_DUEDATE, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_PTDP_VERSION, TDCA_VERSION, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_STARTDATE, TDCA_STARTDATE, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_FLAG, TDCA_FLAG, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_LOCK, TDCA_LOCK, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_EXTERNALID, TDCA_EXTERNALID, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_TAGS, TDCA_TAGS, -1)); 
	m_aAttribPrefs.Add(ATTRIBPREF(IDS_TDLBC_CUSTOMATTRIBS, TDCA_CUSTOMATTRIB, -1)); 
}

CPreferencesTaskDef2Page::~CPreferencesTaskDef2Page()
{
}

void CPreferencesTaskDef2Page::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesTaskDef2Page)
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
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_INHERITATTRIBUTES, m_lbInheritAttrib);
	DDX_LBIndex(pDX, IDC_INHERITATTRIBUTES, m_nSelAttribUse);
	DDX_Check(pDX, IDC_USEPARENTATTRIB, m_bInheritParentAttributes);
}


BEGIN_MESSAGE_MAP(CPreferencesTaskDef2Page, CPreferencesPageBase)
	//{{AFX_MSG_MAP(CPreferencesTaskDef2Page)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_USEPARENTATTRIB, OnUseparentattrib)
	ON_CLBN_CHKCHANGE(IDC_INHERITATTRIBUTES, OnAttribUseChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesTaskDef2Page message handlers

BOOL CPreferencesTaskDef2Page::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesTaskDef2Page::OnFirstShow()
{
	CPreferencesPageBase::OnFirstShow();
	
	AddGroupLine(IDC_DEFGROUP);
	AddGroupLine(IDC_INHERITGROUP);
	AddGroupLine(IDC_DROPLISTGROUP);

	GetDlgItem(IDC_INHERITATTRIBUTES)->EnableWindow(m_bInheritParentAttributes);
	GetDlgItem(IDC_UPDATEINHERITATTRIB)->EnableWindow(m_bInheritParentAttributes);

	int nItem = m_aAttribPrefs.GetSize();
	
	while (nItem--)
	{
		int nIndex = m_lbInheritAttrib.AddString(m_aAttribPrefs[nItem].sName);

		m_lbInheritAttrib.SetItemData(nIndex, m_aAttribPrefs[nItem].nAttrib);
		m_lbInheritAttrib.SetCheck(nIndex, m_aAttribPrefs[nItem].bUse ? 1 : 0);
	}
	UpdateCustomAttributeInfoVisibility();

	m_mgrPrompts.SetEditPrompt(IDC_ALLOCTOLIST, *this, CEnString(IDS_TDC_NOBODY));
	m_mgrPrompts.SetEditPrompt(IDC_ALLOCBYLIST, *this, CEnString(IDS_TDC_NOBODY));
	m_mgrPrompts.SetEditPrompt(IDC_STATUSLIST, *this, CEnString(IDS_TDC_NONE));
	m_mgrPrompts.SetEditPrompt(IDC_CATEGORYLIST, *this, CEnString(IDS_TDC_NONE));
 	m_mgrPrompts.SetEditPrompt(IDC_VERSIONLIST, *this, CEnString(IDS_TDC_NONE));
 	m_mgrPrompts.SetEditPrompt(IDC_TAGLIST, *this, CEnString(IDS_TDC_NONE));
}

void CPreferencesTaskDef2Page::OnOK() 
{
	CPreferencesPageBase::OnOK();
}

void CPreferencesTaskDef2Page::OnUseparentattrib() 
{
	UpdateData();
	UpdateCustomAttributeInfoVisibility();

	GetDlgItem(IDC_INHERITATTRIBUTES)->EnableWindow(m_bInheritParentAttributes);
	GetDlgItem(IDC_UPDATEINHERITATTRIB)->EnableWindow(m_bInheritParentAttributes);

	CPreferencesPageBase::OnControlChange();
}

BOOL CPreferencesTaskDef2Page::EnableCustomAttributeInheritance()
{
	// Find the custom attribute item
	int nCustItem = m_aAttribPrefs.GetSize();

	while (nCustItem--)
	{
		if (m_aAttribPrefs[nCustItem].nAttrib == TDCA_CUSTOMATTRIB)
			break;
	}
	ASSERT(nCustItem != -1);

	if (m_bInheritParentAttributes && (m_aAttribPrefs[nCustItem].bUse > 0))
		return FALSE;
	
	if (!m_bInheritParentAttributes)
	{
		// If inheritance was not previously set, then clear any currently selected
		// attributes to avoid unexpected consequences of suddenly turning it on
		int nItem = m_aAttribPrefs.GetSize();

		while (nItem--)
			m_aAttribPrefs[nItem].bUse = FALSE;

		m_bInheritParentAttributes = TRUE;
		m_bUpdateInheritAttributes = TRUE;
	}

	// Turn on the custom attribute item
	m_aAttribPrefs[nCustItem].bUse = TRUE;

	return TRUE;
}

int CPreferencesTaskDef2Page::GetListItems(TDC_ATTRIBUTE nList, CStringArray& aItems) const
{
	aItems.RemoveAll();

	// include default task attributes
	CString sDef;

	switch (nList)
	{
	case TDCA_ALLOCBY:	sDef = m_sDefAllocByList;	break;
	case TDCA_ALLOCTO:	sDef = m_sDefAllocToList;	break;
	case TDCA_STATUS:	sDef = m_sDefStatusList;	break; 
	case TDCA_CATEGORY:	sDef = m_sDefCategoryList;	break;
	case TDCA_VERSION:	sDef = m_sDefVersionList;	break; 
	case TDCA_TAGS:		sDef = m_sDefTagList;		break;

	default: ASSERT(0);	return 0;
	}

	CStringArray aDef;
	Misc::Split(sDef, aDef, ENDL);
	Misc::AddUniqueItems(aDef, aItems);

	return aItems.GetSize();
}

BOOL CPreferencesTaskDef2Page::GetListIsReadonly(TDC_ATTRIBUTE nList) const
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

CString* CPreferencesTaskDef2Page::GetListText(TDC_ATTRIBUTE nList)
{
	CString* pList = NULL;

	switch (nList)
	{
	case TDCA_ALLOCBY:	pList = &m_sDefAllocByList;		break;
	case TDCA_ALLOCTO:	pList = &m_sDefAllocToList;		break;
	case TDCA_STATUS:	pList = &m_sDefStatusList;		break;
	case TDCA_CATEGORY:	pList = &m_sDefCategoryList;	break;
	case TDCA_VERSION:	pList = &m_sDefVersionList;		break;
	case TDCA_TAGS:		pList = &m_sDefTagList;			break;

	default: ASSERT(0);
	}

	return pList;
}

BOOL CPreferencesTaskDef2Page::AddListItem(TDC_ATTRIBUTE nList, LPCTSTR szItem)
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

BOOL CPreferencesTaskDef2Page::DeleteListItem(TDC_ATTRIBUTE nList, LPCTSTR szItem)
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

void CPreferencesTaskDef2Page::OnAttribUseChange()
{
	UpdateData();

	ASSERT (m_nSelAttribUse >= 0);
	
	if (m_nSelAttribUse >= 0)
	{
		TDC_ATTRIBUTE nSelAttrib = (TDC_ATTRIBUTE)m_lbInheritAttrib.GetItemData(m_nSelAttribUse);

		// search for this item
		int nAttrib = m_aAttribPrefs.GetSize();
		
		while (nAttrib--)
		{
			if (m_aAttribPrefs[nAttrib].nAttrib == nSelAttrib)
			{
				m_aAttribPrefs[nAttrib].bUse = m_lbInheritAttrib.GetCheck(m_nSelAttribUse);
				break;
			}
		}
	}
	UpdateCustomAttributeInfoVisibility();

	CPreferencesPageBase::OnControlChange();
}

void CPreferencesTaskDef2Page::UpdateCustomAttributeInfoVisibility()
{
	BOOL bShowInfo = m_bInheritParentAttributes;

	if (bShowInfo)
	{
		int nCustAttrib = FindItemByData(m_lbInheritAttrib, TDCA_CUSTOMATTRIB);
		bShowInfo = ((nCustAttrib != -1) && m_lbInheritAttrib.GetCheck(nCustAttrib));
	}

	GetDlgItem(IDC_CUSTOMATTRIB_INFO)->ShowWindow(bShowInfo ? SW_SHOW : SW_HIDE);
}

BOOL CPreferencesTaskDef2Page::HasCheckedAttributes() const
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

int CPreferencesTaskDef2Page::GetInheritParentAttributes(CTDCAttributeMap& mapAttribs, BOOL& bUpdateAttrib) const
{
	mapAttribs.RemoveAll();

	if (m_bInheritParentAttributes)
	{
		bUpdateAttrib = m_bUpdateInheritAttributes;
		int nIndex = (int)m_aAttribPrefs.GetSize();
		
		while (nIndex--)
		{
			if (m_aAttribPrefs[nIndex].bUse)
				mapAttribs.Add(m_aAttribPrefs[nIndex].nAttrib);
		}
	}
	else
		bUpdateAttrib = FALSE;

	return mapAttribs.GetCount();
}


void CPreferencesTaskDef2Page::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	// load settings
	m_bInheritParentAttributes = pPrefs->GetProfileInt(szKey, _T("InheritParentAttributes"), pPrefs->GetProfileInt(szKey, _T("UseParentAttributes")));
	m_bUpdateInheritAttributes = pPrefs->GetProfileInt(szKey, _T("UpdateInheritAttributes"), FALSE);
	m_bCatListReadonly = pPrefs->GetProfileInt(szKey, _T("CatListReadonly"), FALSE);
	m_bStatusListReadonly = pPrefs->GetProfileInt(szKey, _T("StatusListReadonly"), FALSE);
	m_bAllocToListReadonly = pPrefs->GetProfileInt(szKey, _T("AllocToListReadonly"), FALSE);
	m_bAllocByListReadonly = pPrefs->GetProfileInt(szKey, _T("AllocByListReadonly"), FALSE);
	m_bVersionListReadonly = pPrefs->GetProfileInt(szKey, _T("VersionListReadonly"), FALSE);
	m_bTagListReadonly = pPrefs->GetProfileInt(szKey, _T("TagListReadonly"), FALSE);

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

CString CPreferencesTaskDef2Page::LoadDefaultListItems(const IPreferences* pPrefs, LPCTSTR szKey)
{
	CStringArray aItems;

	CPreferences::GetProfileArray(pPrefs, szKey, aItems);
	Misc::SortArray(aItems);

	return Misc::FormatArray(aItems, ENDL);
}

void CPreferencesTaskDef2Page::SaveDefaultListItems(LPCTSTR szValueList, IPreferences* pPrefs, LPCTSTR szKey)
{
	CStringArray aItems;

	Misc::Split(szValueList, aItems, ENDL);
	CPreferences::WriteProfileArray(pPrefs, szKey, aItems);
}

void CPreferencesTaskDef2Page::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	// save settings
	pPrefs->WriteProfileInt(szKey, _T("InheritParentAttributes"), m_bInheritParentAttributes);
	pPrefs->WriteProfileInt(szKey, _T("UpdateInheritAttributes"), m_bUpdateInheritAttributes);
	pPrefs->WriteProfileInt(szKey, _T("CatListReadonly"), m_bCatListReadonly);
	pPrefs->WriteProfileInt(szKey, _T("StatusListReadonly"), m_bStatusListReadonly);
	pPrefs->WriteProfileInt(szKey, _T("AllocToListReadonly"), m_bAllocToListReadonly);
	pPrefs->WriteProfileInt(szKey, _T("AllocByListReadonly"), m_bAllocByListReadonly);
	pPrefs->WriteProfileInt(szKey, _T("VersionListReadonly"), m_bVersionListReadonly);
	pPrefs->WriteProfileInt(szKey, _T("TagListReadonly"), m_bTagListReadonly);
	
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


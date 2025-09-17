// PreferencesToolPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PreferencesUICustomToolbarPage.h"

#include "..\Shared\misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencesToolbarPage property page

CPreferencesUICustomToolbarPage::CPreferencesUICustomToolbarPage(const CTDCImageList& ilIcons)
	: 
	CPreferencesPageBase(IDD_PREFTOOLBAR_PAGE),
	m_ilcButtons(ilIcons)
{
	//{{AFX_DATA_INIT(CPreferencesUICustomToolbarPage)
	//}}AFX_DATA_INIT
}

CPreferencesUICustomToolbarPage::~CPreferencesUICustomToolbarPage()
{
}

void CPreferencesUICustomToolbarPage::DoDataExchange(CDataExchange* pDX)
{
	CPreferencesPageBase::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CPreferencesUICustomToolbarPage)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTONLIST, m_ilcButtons);
}

BEGIN_MESSAGE_MAP(CPreferencesUICustomToolbarPage, CPreferencesPageBase) 
	//{{AFX_MSG_MAP(CPreferencesUICustomToolbarPage)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveButtonUp)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMoveButtonDown)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_DELETE, OnDeleteButton)
	ON_BN_CLICKED(IDC_DUPLICATE, OnDuplicateButton)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_BUTTONLIST, OnListSelChange)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesToolbarPage message handlers

BOOL CPreferencesUICustomToolbarPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesUICustomToolbarPage::OnFirstShow()
{
	CPreferencesPageBase::OnFirstShow();
	
	EnableDisableButtons();
}

void CPreferencesUICustomToolbarPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR /*szKey*/)
{
	// load tools
	CTDCToolbarButtonArray aButtons;
	CString sButtons = pPrefs->GetProfileString(_T("CustomToolbar"), _T("Buttons"));

	if (!sButtons.IsEmpty())
	{
		CStringArray aBtnPairs;
		int nPair = Misc::Split(sButtons, aBtnPairs, '|');
		TDCCUSTOMTOOLBARBUTTON tb;

		while (nPair--)
		{
			CString sMenuID = aBtnPairs[nPair];
			
			if (Misc::Split(sMenuID, tb.sImageID, ':'))
			{
				ASSERT(!tb.sImageID.IsEmpty() || (sMenuID == _T("0")));

				tb.nMenuID = (UINT)_ttoi(sMenuID);
				aButtons.InsertAt(0, tb);
			}
		}
	}
	else // backward compatibility
	{
		int nBtn = pPrefs->GetProfileInt(_T("CustomToolbar"), _T("ButtonCount"), 0);
		TDCCUSTOMTOOLBARBUTTON tb;

		while (nBtn--)
		{
			CString sKey = Misc::MakeKey(_T("CustomToolbar\\Button%d"), nBtn + 1);

			tb.nMenuID = pPrefs->GetProfileInt(sKey, _T("MenuID"), 0);
			tb.sImageID = pPrefs->GetProfileString(sKey, _T("ImageID"), _T(""));

			aButtons.InsertAt(0, tb);
		}
	}

	// If no tools and 'first time' then create a toolbar to showcase the feature
	if (!aButtons.GetSize() && pPrefs->GetProfileInt(_T("CustomToolbar"), _T("FirstTime"), TRUE))
	{
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON(ID_NEW,						_T("28")));
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON(ID_PRINTPREVIEW,				_T("82")));
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON(ID_SENDTASKS,				_T("39")));
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON());							// separator
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON(ID_EDIT_FLAGTASK,			_T("49")));
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON(ID_EDIT_LOCKTASK,			_T("100")));
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON(ID_EDIT_PASTEATTRIBUTES,		_T("258")));
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON());							// separator
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON(ID_VIEW_CLEARFILTER,			_T("62")));
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON(ID_VIEW_REFRESHFILTER,		_T("99")));
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON());							// separator
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON(ID_TOOLS_ANALYSELOGGEDTIME,	_T("93")));
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON());							// separator
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON(ID_HELP_DONATE,				_T("16")));
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON());							// separator
		aButtons.Add(TDCCUSTOMTOOLBARBUTTON(ID_TOOLS_CHECKFORUPDATES,	_T("83")));
	}
	
	m_ilcButtons.SetButtons(aButtons);
}

void CPreferencesUICustomToolbarPage::SavePreferences(IPreferences* pPrefs, LPCTSTR /*szKey*/) const
{
	CTDCToolbarButtonArray aButtons;
	int nBtnCount = GetToolbarButtons(aButtons);

	CString sButtons;

	for (int nBtn = 0; nBtn < nBtnCount; nBtn++)
	{
		const TDCCUSTOMTOOLBARBUTTON& tb = aButtons[nBtn];
 		sButtons += Misc::Format(_T("%d:%s|"), tb.nMenuID, tb.sImageID);
	}
	sButtons.TrimRight('|');

	// Check and delete old preferences once only
	if (pPrefs->HasProfileSection(_T("CustomToolbar\\Button1")))
		pPrefs->DeleteProfileSection(_T("CustomToolbar"), true);

	pPrefs->WriteProfileString(_T("CustomToolbar"), _T("Buttons"), sButtons);
	pPrefs->WriteProfileInt(_T("CustomToolbar"), _T("FirstTime"), FALSE);
}

void CPreferencesUICustomToolbarPage::OnSize(UINT nType, int cx, int cy) 
{
	CPreferencesPageBase::OnSize(nType, cx, cy);
	
	if (m_ilcButtons.GetSafeHwnd())
	{
		CRect rBtnList = GetChildRect(&m_ilcButtons);

		int nXOffset = (cx - rBtnList.left - GetCtrlRect(this, IDC_MOVEDOWN).right);
		int nYOffset = (cy - rBtnList.top - rBtnList.bottom);

		OffsetCtrl(this, IDC_MOVEDOWN, nXOffset, 0);
		OffsetCtrl(this, IDC_MOVEUP, nXOffset, 0);
		OffsetCtrl(this, IDC_DELETE, nXOffset, 0);
		OffsetCtrl(this, IDC_DUPLICATE, nXOffset, 0);

		ResizeChild(&m_ilcButtons, nXOffset, nYOffset);
	}
}

BOOL CPreferencesUICustomToolbarPage::HasToolbarButtons() const
{
	return m_ilcButtons.HasButtons();
}

int CPreferencesUICustomToolbarPage::GetToolbarButtons(CTDCToolbarButtonArray& aButtons) const
{
	return m_ilcButtons.GetButtons(aButtons);
}

UINT CPreferencesUICustomToolbarPage::GetLastCustomToolbarButtonID() const
{
	return m_ilcButtons.GetLastButtonID();
}

void CPreferencesUICustomToolbarPage::OnOK()
{
	CPreferencesPageBase::OnOK();
}

void CPreferencesUICustomToolbarPage::OnMoveButtonUp() 
{
	if (m_ilcButtons.MoveSelectedButtonUp())
		EnableDisableButtons();
}

void CPreferencesUICustomToolbarPage::OnMoveButtonDown() 
{
	if (m_ilcButtons.MoveSelectedButtonDown())
		EnableDisableButtons();
}

void CPreferencesUICustomToolbarPage::OnDeleteButton() 
{
	if (m_ilcButtons.DeleteSelectedButton())
		EnableDisableButtons();
}

void CPreferencesUICustomToolbarPage::OnDuplicateButton() 
{
	if (m_ilcButtons.DuplicateSelectedButton())
		EnableDisableButtons();
}

void CPreferencesUICustomToolbarPage::OnListSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (CEnListCtrl::IsSelectionChange((NMLISTVIEW*)pNMHDR))
	{
		EnableDisableButtons();
		*pResult = 0;
	}
}

void CPreferencesUICustomToolbarPage::EnableDisableButtons()
{
	GetDlgItem(IDC_MOVEDOWN)->EnableWindow(m_ilcButtons.CanMoveSelectedButtonDown());
	GetDlgItem(IDC_MOVEUP)->EnableWindow(m_ilcButtons.CanMoveSelectedButtonUp());
	GetDlgItem(IDC_DELETE)->EnableWindow(m_ilcButtons.CanDeleteSelectedButton());
	GetDlgItem(IDC_DUPLICATE)->EnableWindow(m_ilcButtons.CanDuplicateSelectedButton());
}

BOOL CPreferencesUICustomToolbarPage::RemapMenuItemIDs(const CMap<UINT, UINT, UINT, UINT&>& mapCmdIDs)
{
	CTDCToolbarButtonArray aButtons;

	if (!m_ilcButtons.GetButtons(aButtons))
		return FALSE;
	
	if (!CTDLCustomToolbar::RemapMenuItemIDs(mapCmdIDs, aButtons))
		return FALSE;

	m_ilcButtons.SetButtons(aButtons);
	return TRUE;
}

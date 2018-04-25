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

IMPLEMENT_DYNCREATE(CPreferencesUICustomToolbarPage, CPreferencesPageBase)

CPreferencesUICustomToolbarPage::CPreferencesUICustomToolbarPage() 
	: 
	CPreferencesPageBase(CPreferencesUICustomToolbarPage::IDD)
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
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesToolbarPage message handlers

BOOL CPreferencesUICustomToolbarPage::OnInitDialog() 
{
	CPreferencesPageBase::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesUICustomToolbarPage::LoadPreferences(const IPreferences* pPrefs, LPCTSTR /*szKey*/)
{
	// load tools
	CToolbarButtonArray aButtons;
	int nBtnCount = pPrefs->GetProfileInt(_T("Toolbar"), _T("ButtonCount"), 0);

	for (int nBtn = 1; nBtn <= nBtnCount; nBtn++)
	{
		CString sKey = Misc::MakeKey(_T("Toolbar\\Button%d"), nBtn);

		TOOLBARBUTTON tb;
		tb.nMenuID = pPrefs->GetProfileInt(sKey, _T("MenuID"), 0);
		tb.sImageID = pPrefs->GetProfileString(sKey, _T("ImageID"), _T(""));
		
		aButtons.Add(tb);
	}
	
	m_ilcButtons.SetButtons(aButtons);
}

void CPreferencesUICustomToolbarPage::SavePreferences(IPreferences* pPrefs, LPCTSTR /*szKey*/) const
{
	CToolbarButtonArray aButtons;
	int nBtnCount = GetToolbarButtons(aButtons);

	for (int nBtn = 0; nBtn < nBtnCount; nBtn++)
	{
		const TOOLBARBUTTON& tb = aButtons[nBtn];

        CString sKey = Misc::MakeKey(_T("Toolbar\\Button%d"), nBtn + 1);
		
		pPrefs->WriteProfileInt(sKey, _T("MenuID"), tb.nMenuID);
		pPrefs->WriteProfileString(sKey, _T("ImageID"), tb.sImageID);
	}

	pPrefs->WriteProfileInt(_T("Toolbar"), _T("ButtonCount"), nBtnCount);
}

void CPreferencesUICustomToolbarPage::OnSize(UINT nType, int cx, int cy) 
{
	CPreferencesPageBase::OnSize(nType, cx, cy);
	
	if (m_ilcButtons.GetSafeHwnd())
	{
		CPoint ptBorders = GetChildRect(&m_ilcButtons).TopLeft();
	//	ResizeChild(&m_ilcButtons, cx - 2 * ptBorders.x, cy - 2 * ptBorders.y);
	}
}

BOOL CPreferencesUICustomToolbarPage::HasToolbarButtons() const
{
	return m_ilcButtons.HasButtons();
}

int CPreferencesUICustomToolbarPage::GetToolbarButtons(CToolbarButtonArray& aButtons) const
{
	return m_ilcButtons.GetButtons(aButtons);
}

void CPreferencesUICustomToolbarPage::OnOK()
{
	CPreferencesPageBase::OnOK();
}

void CPreferencesUICustomToolbarPage::OnMoveButtonUp() 
{
	m_ilcButtons.MoveSelectedButtonUp();
}

void CPreferencesUICustomToolbarPage::OnMoveButtonDown() 
{
	m_ilcButtons.MoveSelectedButtonDown();
}

void CPreferencesUICustomToolbarPage::OnDeleteButton() 
{
	m_ilcButtons.DeleteSelectedButton();
}

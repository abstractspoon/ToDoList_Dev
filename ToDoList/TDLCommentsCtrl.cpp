left
bottom
bottom
bottom
bottom
top
// TDLCommentsCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TDLCommentsCtrl.h"

#include "..\shared\contentMgr.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\dlgunits.h"
#include "..\shared\misc.h"

#include "..\interfaces\icontentcontrol.h"

/////////////////////////////////////////////////////////////////////////////

enum
{
	IDC_COMMENTSFORMATCOMBO = 1001,
	IDC_COMMENTSCTRL,
};

/////////////////////////////////////////////////////////////////////////////

const UINT WM_TCC_INITCOMMENTS = (WM_APP + 1);

/////////////////////////////////////////////////////////////////////////////
// CTDLCommentsCtrl dialog

IMPLEMENT_DYNAMIC(CTDLCommentsCtrl, CRuntimeDlg)

CTDLCommentsCtrl::CTDLCommentsCtrl(LPCTSTR szLabel, const CContentMgr* pMgrContent)
	:
	m_pMgrContent(pMgrContent), 
	m_cbCommentsFmt(pMgrContent)
{
	int nDLUComboOffset = 0;

	if (!Misc::IsEmpty(szLabel))
	{
		AddRCControl(_T("CONTROL"), WC_STATIC, szLabel, 
					SS_CENTERIMAGE, 0, 0, 0, 40, 12, (UINT)IDC_STATIC);

		nDLUComboOffset = 43;
	}

	AddRCControl(_T("CONTROL"), WC_COMBOBOX, _T(""), 
					CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS | WS_VSCROLL | WS_TABSTOP,
					0, nDLUComboOffset, 0, 85, 200, IDC_COMMENTSFORMATCOMBO);
}

CTDLCommentsCtrl::~CTDLCommentsCtrl()
{
}

void CTDLCommentsCtrl::DoDataExchange(CDataExchange* pDX)
{
	CRuntimeDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMMENTSFORMATCOMBO, m_cbCommentsFmt);
}

BEGIN_MESSAGE_MAP(CTDLCommentsCtrl, CRuntimeDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()

	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCommentsformat)
	ON_REGISTERED_MESSAGE(WM_ICC_CONTENTCHANGE, OnCommentsChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLCommentsCtrl message handlers

BOOL CTDLCommentsCtrl::Create(CWnd* pParent, const CRect& rPos, UINT nID)
{
	return CRuntimeDlg::Create(NULL, (WS_CHILD | WS_VISIBLE | WS_TABSTOP), 
								WS_EX_CONTROLPARENT, rPos, pParent, nID);
}

BOOL CTDLCommentsCtrl::OnInitDialog()
{
	CRuntimeDlg::OnInitDialog();
	
	// Delay initialisation of comments until after any font changes
	PostMessage(WM_TCC_INITCOMMENTS);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLCommentsCtrl::OnSize(UINT nType, int cx, int cy)
{
	CRuntimeDlg::OnSize(nType, cx, cy);

	CRect rComments;
	CalcCommentsCtrlRect(rComments, cx, cy);

	::MoveWindow(m_ctrlComments, rComments.left, rComments.right, rComments.Width(), rComments.Height(), TRUE);
}

HBRUSH CTDLCommentsCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CRuntimeDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
	{
		if (m_brBack.GetSafeHandle())
			hbr = m_brBack;
	}

	return hbr;
}

BOOL CTDLCommentsCtrl::OnEraseBkgnd(CDC* pDC)
{
	ExcludeCtrl(this, IDC_COMMENTSFORMATCOMBO, pDC);
	ExcludeCtrl(this, IDC_COMMENTSCTRL, pDC);

	return CRuntimeDlg::OnEraseBkgnd(pDC);
}

void CTDLCommentsCtrl::OnSelchangeCommentsformat() 
{
	ASSERT(m_pMgrContent);

	m_cbCommentsFmt.GetSelectedFormat(m_cfDefault);

	if (m_ctrlComments.GetContentFormat() != m_cfDefault)
	{
		CRect rComments;
		CalcCommentsCtrlRect(rComments);

		DWORD dwStyle = (WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_CLIPSIBLINGS); 

		if (m_pMgrContent->CreateContentControl(m_cfDefault, m_ctrlComments, 
												IDC_COMMENTSCTRL, dwStyle, WS_EX_CLIENTEDGE, 
												rComments, *this))
		{
			CUIThemeFile theme;
			theme.crToolbarDark = theme.crToolbarLight = RGB(255, 255, 255);

			m_ctrlComments.SetUITheme(theme);
			m_ctrlComments.SendMessage(WM_SETFONT, (WPARAM)CDialogHelper::GetFont(*this));
		}

		if (!m_ctrlComments.SetContent(m_defCustomComments, TRUE))
			m_ctrlComments.SetTextContent(m_sDefTextComments, TRUE);
	}
}

void CTDLCommentsCtrl::CalcCommentsCtrlRect(CRect& rCtrl, int cx, int cy) const
{
	ASSERT(GetSafeHwnd());

	if ((cx == 0) || (cy == 0))
		GetClientRect(rCtrl);
	else
		rCtrl.SetRect(0, 0, cx, cy);

	CRect rCombo = GetCtrlRect(IDC_COMMENTSFORMATCOMBO);
	rCtrl.top = (rCombo.bottom + CDlgUnits(this).ToPixelsY(4));
}

LRESULT CTDLCommentsCtrl::OnCommentsChange(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (!m_ctrlComments.IsSettingContent())
	{
		m_defCustomComments.Empty();
		m_sDefTextComments.Empty();

		m_ctrlComments.GetContent(m_defCustomComments);
		m_ctrlComments.GetTextContent(m_sDefTextComments);
	}

	return 0L;
}

LRESULT CTDLCommentsCtrl::OnCommentsDoHelp(WPARAM wParam, LPARAM lParam)
{
	// Forward to parent
	return GetParent()->SendMessage(WM_ICC_DOHELP, wParam, lParam);
}

LRESULT CTDLCommentsCtrl::OnCommentsKillFocus(WPARAM wParam, LPARAM lParam)
{
	// Forward to parent
	return GetParent()->SendMessage(WM_ICC_KILLFOCUS, wParam, lParam);
}

LRESULT CTDLCommentsCtrl::OnCommentsWantSpellCheck(WPARAM wParam, LPARAM lParam)
{
	// Forward to parent
	return GetParent()->SendMessage(WM_ICC_WANTSPELLCHECK, wParam, lParam);
}

void CTDLCommentsCtrl::SetUITheme(const CUIThemeFile& theme)
{
	ASSERT(GetSafeHwnd());

	m_theme = theme;

	GraphicsMisc::VerifyDeleteObject(m_brBack);

	if (m_theme.crAppBackLight != CLR_NONE)
		m_brBack.CreateSolidBrush(m_theme.crAppBackLight);

	m_ctrlComments.SetUITheme(m_theme);

	Invalidate();
}

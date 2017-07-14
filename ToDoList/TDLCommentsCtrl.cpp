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
	IDC_LABEL = 1001,
	IDC_COMBO,
	IDC_CTRL,
};

/////////////////////////////////////////////////////////////////////////////
// CTDLCommentsCtrl dialog

IMPLEMENT_DYNAMIC(CTDLCommentsCtrl, CRuntimeDlg)

CTDLCommentsCtrl::CTDLCommentsCtrl(LPCTSTR szLabel, int nComboLenDLU, const CContentMgr* pMgrContent)
	:
	m_pMgrContent(pMgrContent), 
	m_cbCommentsFmt(pMgrContent),
	m_hFont(NULL)
{
	int nComboOffsetDLU = 0;

	if (!Misc::IsEmpty(szLabel))
	{
		AddRCControl(_T("CONTROL"), WC_STATIC, szLabel, 
					SS_CENTERIMAGE, 0, 0, 0, 40, 12, IDC_LABEL);

		nComboOffsetDLU = 43;
	}

	AddRCControl(_T("CONTROL"), WC_COMBOBOX, _T(""), 
					CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS | WS_VSCROLL | WS_TABSTOP,
					0, nComboOffsetDLU, 0, nComboLenDLU, 200, IDC_COMBO);
}

CTDLCommentsCtrl::~CTDLCommentsCtrl()
{
}

void CTDLCommentsCtrl::DoDataExchange(CDataExchange* pDX)
{
	CRuntimeDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO, m_cbCommentsFmt);
}

BEGIN_MESSAGE_MAP(CTDLCommentsCtrl, CRuntimeDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_SETFONT, OnSetFont)

	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCommentsformat)
	ON_REGISTERED_MESSAGE(WM_ICC_CONTENTCHANGE, OnCommentsChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLCommentsCtrl message handlers

BOOL CTDLCommentsCtrl::Create(CWnd* pParent, const CRect& rPos, UINT nID)
{
	SetBordersDLU(0);

	return CRuntimeDlg::Create(NULL, (WS_CHILD | WS_VISIBLE | WS_TABSTOP), 
								WS_EX_CONTROLPARENT, rPos, pParent, nID);
}

BOOL CTDLCommentsCtrl::OnInitDialog()
{
	CRuntimeDlg::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CTDLCommentsCtrl::OnSetFont(WPARAM wParam, LPARAM /*lParam*/)
{
	// Once only
	if (!m_ctrlComments.GetSafeHwnd() && !m_hFont)
		SetDefaultCommentsFont((HFONT)wParam);

	return Default();
}

void CTDLCommentsCtrl::SetDefaultCommentsFont(HFONT hFont)
{
	m_hFont = hFont;

	if (m_ctrlComments.GetSafeHwnd())
		m_ctrlComments.SendMessage(WM_SETFONT, (WPARAM)m_hFont);
}

void CTDLCommentsCtrl::OnSize(UINT nType, int cx, int cy)
{
	CRuntimeDlg::OnSize(nType, cx, cy);

	if (m_ctrlComments.GetSafeHwnd())
	{
		CRect rComments;
		CalcCommentsCtrlRect(rComments, cx, cy);

		::MoveWindow(m_ctrlComments, rComments.left, rComments.right, rComments.Width(), rComments.Height(), TRUE);
	}
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
	if (m_theme.crAppBackLight != CLR_NONE)
	{
		ExcludeCtrl(this, IDC_LABEL, pDC);
		ExcludeCtrl(this, IDC_COMBO, pDC);
		ExcludeCtrl(this, IDC_CTRL, pDC);

		CRect rClient;
		GetClientRect(rClient);

		pDC->FillSolidRect(rClient, m_theme.crAppBackLight);
		return TRUE;
	}

	return CRuntimeDlg::OnEraseBkgnd(pDC);
}

void CTDLCommentsCtrl::OnSelchangeCommentsformat() 
{
	if (UpdateControlFormat())
	{
		// Notify Parent
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM)GetSafeHwnd());
	}
}

BOOL CTDLCommentsCtrl::UpdateControlFormat()
{
	ASSERT(m_pMgrContent);
	ASSERT(GetSafeHwnd());

	CONTENTFORMAT cf;
	m_cbCommentsFmt.GetSelectedFormat(cf);

	if (m_ctrlComments.GetContentFormat() == cf)
		return FALSE;

	// Cache existing content
	CString sTextContent;
	CBinaryData customContent;
	GetContent(sTextContent, customContent);

	CRect rComments;
	CalcCommentsCtrlRect(rComments);

	DWORD dwStyle = (WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_CLIPSIBLINGS); 

	if (!m_pMgrContent->CreateContentControl(cf, m_ctrlComments, 
		IDC_CTRL, dwStyle, WS_EX_CLIENTEDGE, rComments, *this))
	{
		return FALSE;
	}

	m_ctrlComments.SetUITheme(m_theme);
	
	if (m_hFont)
		m_ctrlComments.SendMessage(WM_SETFONT, (WPARAM)m_hFont);

	// Restore content
	SetContent(sTextContent, customContent);

	return TRUE;
}

void CTDLCommentsCtrl::CalcCommentsCtrlRect(CRect& rCtrl, int cx, int cy) const
{
	ASSERT(GetSafeHwnd());

	if ((cx == 0) || (cy == 0))
		GetClientRect(rCtrl);
	else
		rCtrl.SetRect(0, 0, cx, cy);

	CRect rCombo = GetCtrlRect(IDC_COMBO);
	rCtrl.top = (rCombo.bottom + CDlgUnits(this).ToPixelsY(4));
}

LRESULT CTDLCommentsCtrl::OnCommentsChange(WPARAM wParam, LPARAM lParam)
{
	if (!m_ctrlComments.IsSettingContent())
	{
		// Forward to parent
		return GetParent()->SendMessage(WM_ICC_CONTENTCHANGE, wParam, lParam);
	}

	// else
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

BOOL CTDLCommentsCtrl::GetContent(CString& sTextContent, CBinaryData& customContent) const
{
	BOOL bRes = (m_ctrlComments.GetTextContent(sTextContent) > 0);
	bRes |= (m_ctrlComments.GetContent(customContent) > 0);

	return bRes;
}

BOOL CTDLCommentsCtrl::SetContent(const CString& sTextContent, const CBinaryData& customContent)
{
	if (!customContent.IsEmpty() && m_ctrlComments.SetContent(customContent, TRUE))
		return TRUE;

	// else
	return (!sTextContent.IsEmpty() && m_ctrlComments.SetTextContent(sTextContent, TRUE));
}

BOOL CTDLCommentsCtrl::GetSelectedFormat(CONTENTFORMAT& cf) const
{
	return (m_cbCommentsFmt.GetSelectedFormat(cf) != CB_ERR);
}

BOOL CTDLCommentsCtrl::SetSelectedFormat(const CONTENTFORMAT& cf)
{
	CONTENTFORMAT cfSel;
	m_cbCommentsFmt.GetSelectedFormat(cfSel);

	if (m_ctrlComments.GetSafeHwnd() && (cf == cfSel))
		return TRUE;
	
	if (m_cbCommentsFmt.SetSelectedFormat(cf) == CB_ERR)
		return FALSE;

	// else
	UpdateControlFormat();
	return TRUE;
}

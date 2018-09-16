// TDLCommentsCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLCommentsCtrl.h"

#include "..\shared\contentMgr.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\dlgunits.h"
#include "..\shared\misc.h"
#include "..\shared\themed.h"
#include "..\shared\preferences.h"
#include "..\shared\enstring.h"
#include "..\shared\winclasses.h"
#include "..\shared\wclassdefines.h"

#include "..\interfaces\icontentcontrol.h"

/////////////////////////////////////////////////////////////////////////////

enum
{
	IDC_COMBOLABEL = 1001,
	IDC_COMBO,
	IDC_CTRL,
};

/////////////////////////////////////////////////////////////////////////////
// CTDLCommentsCtrl dialog

IMPLEMENT_DYNAMIC(CTDLCommentsCtrl, CRuntimeDlg)

CTDLCommentsCtrl::CTDLCommentsCtrl(BOOL bLabel, int nComboLenDLU, const CContentMgr* pMgrContent)
	:
	m_pMgrContent(pMgrContent), 
	m_cbCommentsFmt(pMgrContent),
	m_bFirstLoadCommentsPrefs(TRUE),
	m_hDefaultFont(NULL),
	m_bReadOnly(FALSE)
{
	int nComboOffsetDLU = 0;

	if (bLabel)
	{
		CString sLabel;
		sLabel.LoadString(IDS_TDC_FIELD_COMMENTS);
		int nLabelLen = 40;

		AddRCControl(_T("CONTROL"), WC_STATIC, sLabel, SS_CENTERIMAGE, 0, 0, 1, nLabelLen, 12, IDC_COMBOLABEL);

		nComboOffsetDLU = (nLabelLen + 3);
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
	ON_WM_SETFOCUS()
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_CBN_SELENDOK(IDC_COMBO, OnSelchangeCommentsformat)
	ON_REGISTERED_MESSAGE(WM_ICC_CONTENTCHANGE, OnCommentsChange)
	ON_REGISTERED_MESSAGE(WM_ICC_KILLFOCUS, OnCommentsKillFocus)
	ON_REGISTERED_MESSAGE(WM_ICC_WANTSPELLCHECK, OnCommentsWantSpellCheck)
	ON_REGISTERED_MESSAGE(WM_ICC_DOHELP, OnCommentsDoHelp)
	ON_REGISTERED_MESSAGE(WM_ICC_GETCLIPBOARD, OnCommentsGetClipboard)
	ON_REGISTERED_MESSAGE(WM_ICC_HASCLIPBOARD, OnCommentsHasClipboard)
	ON_REGISTERED_MESSAGE(WM_ICC_TASKLINK, OnCommentsTaskLink)
	ON_REGISTERED_MESSAGE(WM_ICC_FAILEDLINK, OnCommentsFailedLink)
	ON_REGISTERED_MESSAGE(WM_ICC_GETLINKTOOLTIP, OnCommentsGetTooltip)
	ON_WM_DESTROY()
	ON_WM_ENABLE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLCommentsCtrl message handlers

BOOL CTDLCommentsCtrl::Create(CWnd* pParent, UINT nID, const CRect& rPos)
{
	SetBordersDLU(0);

	return CRuntimeDlg::Create(NULL, (WS_CHILD | WS_VISIBLE | WS_TABSTOP), 
								WS_EX_CONTROLPARENT, rPos, pParent, nID);
}

BOOL CTDLCommentsCtrl::OnInitDialog()
{
	CRuntimeDlg::OnInitDialog();

	m_cfLastCustom.Empty();
	m_LastCustomComments.Empty();

	if (!m_sComboPrompt.IsEmpty())
		m_mgrPrompts.SetComboPrompt(m_cbCommentsFmt, m_sComboPrompt);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLCommentsCtrl::SetWindowPrompts(LPCTSTR szComboPrompt, LPCTSTR szCommentsPrompt)
{
	ASSERT(GetSafeHwnd());

	m_sComboPrompt = szComboPrompt;
	m_sCommentsPrompt = szCommentsPrompt;

	if (m_cbCommentsFmt.GetSafeHwnd())
		m_mgrPrompts.SetComboPrompt(m_cbCommentsFmt, szComboPrompt);

	if (m_ctrlComments.GetSafeHwnd() && CWinClasses::IsEditControl(m_ctrlComments))
		m_mgrPrompts.SetEditPrompt(m_ctrlComments, szCommentsPrompt);
}

LRESULT CTDLCommentsCtrl::OnSetFont(WPARAM wParam, LPARAM /*lParam*/)
{
	// If the default font has not been explicitly set
	// then use this font once only
	if (!m_ctrlComments.GetSafeHwnd() && !m_hDefaultFont)
		m_hDefaultFont = (HFONT)wParam;

	return Default();
}

void CTDLCommentsCtrl::SetDefaultCommentsFont(HFONT hFont)
{
	m_hDefaultFont = hFont;

	if (m_ctrlComments.GetSafeHwnd())
		m_ctrlComments.SendMessage(WM_SETFONT, (WPARAM)m_hDefaultFont);
}

void CTDLCommentsCtrl::SetCtrlStates(RT_CTRLSTATE nComboState, RT_CTRLSTATE nCommentsState)
{
	switch (nComboState)
	{
		case RTCS_ENABLED:
			m_cbCommentsFmt.EnableWindow(TRUE);
			break;

		case RTCS_DISABLED:
		case RTCS_READONLY:
			m_cbCommentsFmt.EnableWindow(FALSE);
			break;
	}

	switch (nCommentsState)
	{
		case RTCS_ENABLED:
			::EnableWindow(m_ctrlComments, TRUE);
			m_ctrlComments.SetReadOnly(FALSE);
			break;

		case RTCS_DISABLED:
			::EnableWindow(m_ctrlComments, FALSE);
			break;

		case RTCS_READONLY:
			::EnableWindow(m_ctrlComments, TRUE);
			m_ctrlComments.SetReadOnly(TRUE);
			break;
	}
}

void CTDLCommentsCtrl::OnSize(UINT nType, int cx, int cy)
{
	CRuntimeDlg::OnSize(nType, cx, cy);

	if (m_ctrlComments.GetSafeHwnd())
	{
		CRect rComments;
		CalcCommentsCtrlRect(rComments, cx, cy);

		::MoveWindow(m_ctrlComments, rComments.left, rComments.top, rComments.Width(), rComments.Height(), TRUE);
	}
}

HBRUSH CTDLCommentsCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CRuntimeDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (m_brBack.GetSafeHandle())
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = m_brBack;
		}
	}

	return hbr;
}

BOOL CTDLCommentsCtrl::OnEraseBkgnd(CDC* pDC)
{
	if (m_theme.crAppBackLight != CLR_NONE)
	{
		ExcludeCtrl(this, IDC_COMBOLABEL, pDC);
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
	// Cache current content
	CString sOldTextContent;
	CBinaryData oldCustomContent;
	CONTENTFORMAT cfOld = m_ctrlComments.GetContentFormat();
	BOOL bHadContent = GetContent(sOldTextContent, oldCustomContent);

	// Restore content if there was any
	if (UpdateControlFormat())
	{
		if (bHadContent)
		{
			CONTENTFORMAT cfNew = m_ctrlComments.GetContentFormat();

			if (!cfOld.FormatIsText() && cfNew.FormatIsText())
			{
				// If switching to a text format from a binary format
				// we save off the binary in case they decide to switch 
				// back without having made an edit
				m_cfLastCustom = cfOld;
				m_LastCustomComments = oldCustomContent;

				m_ctrlComments.SetTextContent(sOldTextContent, TRUE);
			}
			else
			{
				if (m_cfLastCustom == cfNew)
				{
					// if the new format is the same as the 'last' one
					// then restore the 'last' custom comments
					m_ctrlComments.SetContent(m_LastCustomComments, TRUE);
				}
				else
				{ 
					// just set the text format content
					m_ctrlComments.SetTextContent(sOldTextContent, TRUE);
				}

				m_cfLastCustom.Empty();
				m_LastCustomComments.Empty();
			}
		}
	}

	// Notify Parent
	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetSafeHwnd());
}

BOOL CTDLCommentsCtrl::UpdateControlFormat()
{
	ASSERT(m_pMgrContent);
	ASSERT(GetSafeHwnd());

	// save outgoing content prefs provided they've already been loaded
	if (!m_bFirstLoadCommentsPrefs)
		SavePreferences();

	CONTENTFORMAT cf;
	m_cbCommentsFmt.GetSelectedFormat(cf);

	if (m_ctrlComments.GetContentFormat() == cf)
	{
		LoadPreferences();
		m_bFirstLoadCommentsPrefs = m_sPrefsFilePath.IsEmpty();

		return FALSE;
	}

	CRect rComments;
	CalcCommentsCtrlRect(rComments);

	DWORD dwStyle = (WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_CLIPSIBLINGS); 

	if (!m_pMgrContent->CreateContentControl(cf, m_ctrlComments, 
		IDC_CTRL, dwStyle, WS_EX_CLIENTEDGE, rComments, *this))
	{
		return FALSE;
	}

	if (CThemed::IsAppThemed())
		m_ctrlComments.SetUITheme(m_theme);
	
	if (m_hDefaultFont)
		m_ctrlComments.SendMessage(WM_SETFONT, (WPARAM)m_hDefaultFont);

	if (CWinClasses::IsEditControl(m_ctrlComments))
		m_mgrPrompts.SetEditPrompt(m_ctrlComments, m_sCommentsPrompt);
	
	LoadPreferences();

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
	rCtrl.top = (rCombo.bottom + CDlgUnits(this).ToPixelsY(2));
}

LRESULT CTDLCommentsCtrl::OnCommentsChange(WPARAM wParam, LPARAM lParam)
{
	if (!m_ctrlComments.IsSettingContent())
	{
		// An edit means there is no going back
		m_cfLastCustom.Empty();
		m_LastCustomComments.Empty();

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

LRESULT CTDLCommentsCtrl::OnCommentsGetClipboard(WPARAM wParam, LPARAM lParam)
{
	// Forward to parent
	return GetParent()->SendMessage(WM_ICC_GETCLIPBOARD, wParam, lParam);
}

LRESULT CTDLCommentsCtrl::OnCommentsHasClipboard(WPARAM wParam, LPARAM lParam)
{
	// Forward to parent
	return GetParent()->SendMessage(WM_ICC_HASCLIPBOARD, wParam, lParam);
}

LRESULT CTDLCommentsCtrl::OnCommentsTaskLink(WPARAM wParam, LPARAM lParam)
{
	// Forward to parent
	return GetParent()->SendMessage(WM_ICC_TASKLINK, wParam, lParam);
}

LRESULT CTDLCommentsCtrl::OnCommentsFailedLink(WPARAM wParam, LPARAM lParam)
{
	// Forward to parent
	return GetParent()->SendMessage(WM_ICC_FAILEDLINK, wParam, lParam);
}

LRESULT CTDLCommentsCtrl::OnCommentsGetTooltip(WPARAM wParam, LPARAM lParam)
{
	// Forward to parent
	return GetParent()->SendMessage(WM_ICC_GETLINKTOOLTIP, wParam, lParam);
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

BOOL CTDLCommentsCtrl::SetContent(const CString& sTextContent, const CBinaryData& customContent, BOOL bResetSelection)
{
	BOOL bSet = (!customContent.IsEmpty() && m_ctrlComments.SetContent(customContent, bResetSelection));

	if (!bSet)
		bSet = m_ctrlComments.SetTextContent(sTextContent, bResetSelection);

	if (bSet)
	{
		// An edit means there is no going back
		m_cfLastCustom.Empty();
		m_LastCustomComments.Empty();
	}

	return bSet;
}

void CTDLCommentsCtrl::ClearContent()
{
	m_ctrlComments.SetTextContent(_T(""), FALSE);
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
	{
		// Setting to an empty format is not an error 
		// it's just a way to indicate multiple formats
		return cf.IsEmpty();
	}

	// else
	UpdateControlFormat();
	return TRUE;
}

void CTDLCommentsCtrl::SavePreferences() const 
{ 
	CPreferences prefs;
	CString sKey = GetPreferencesKey();

	m_ctrlComments.SavePreferences(prefs, sKey); 
}

void CTDLCommentsCtrl::LoadPreferences() 
{ 
	CPreferences prefs;
	CString sKey = GetPreferencesKey();

	m_ctrlComments.LoadPreferences(prefs, sKey); 
}

CString CTDLCommentsCtrl::GetPreferencesKey() const
{
	CString sKey;

	if (!m_sPrefsFilePath.IsEmpty())
		sKey.Format(_T("FileStates\\%s"), CPreferences::KeyFromFile(m_sPrefsFilePath));
	else
		sKey = _T("FileStates\\Default");

	return sKey;
}

void CTDLCommentsCtrl::OnDestroy()
{
	CRuntimeDlg::OnDestroy();

	SavePreferences();
}

void CTDLCommentsCtrl::OnEnable(BOOL bEnable)
{
	CRuntimeDlg::OnEnable(bEnable);

	m_cbCommentsFmt.EnableWindow(bEnable);
	::EnableWindow(m_ctrlComments, bEnable);

	if (bEnable)
		m_ctrlComments.SetReadOnly(m_bReadOnly);
}

void CTDLCommentsCtrl::SetPreferencesFilePath(LPCTSTR szFilePath) 
{ 
	m_sPrefsFilePath = szFilePath; 

	if (!m_sPrefsFilePath.IsEmpty())
		LoadPreferences();
}

void CTDLCommentsCtrl::OnSetFocus(CWnd* /*pOldWnd*/)
{
	m_ctrlComments.SetFocus();
}

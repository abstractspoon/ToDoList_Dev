// PreferencesPageBase.cpp : implementation file
//

#include "stdafx.h"
#include "PreferencesBase.h"
#include "graphicsmisc.h"
#include "deferwndmove.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencesPageBase property page

CPreferencesPageBase::CPreferencesPageBase(UINT nDlgTemplateID) 
	: 
	CPropertyPage(nDlgTemplateID), 
	m_brush(NULL), 
	m_crback(CLR_NONE), 
	m_bFirstShow(TRUE), 
	m_nHelpID(nDlgTemplateID)
{
}

CPreferencesPageBase::CPreferencesPageBase(UINT nDlgTemplateID, UINT nHelpID)
	: 
	CPropertyPage(nDlgTemplateID), 
	m_brush(NULL), 
	m_crback(CLR_NONE), 
	m_bFirstShow(TRUE), 
	m_nHelpID(nHelpID)
{
}

CPreferencesPageBase::~CPreferencesPageBase()
{
	GraphicsMisc::VerifyDeleteObject(m_brush);
}

IMPLEMENT_DYNAMIC(CPreferencesPageBase, CPropertyPage);

BEGIN_MESSAGE_MAP(CPreferencesPageBase, CPropertyPage)
	ON_CONTROL_RANGE(BN_CLICKED, 0, 0xffff, OnControlChange)
	ON_CONTROL_RANGE(EN_CHANGE, 0, 0xffff, OnControlChange)
	ON_CONTROL_RANGE(CBN_EDITCHANGE, 0, 0xffff, OnControlChange)
	ON_CONTROL_RANGE(CBN_SELCHANGE, 0, 0xffff, OnControlChange)
	ON_CONTROL_RANGE(CLBN_CHKCHANGE, 0, 0xffff, OnControlChange)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

CWnd* CPreferencesPageBase::GetDlgItem(UINT nID) const
{
	static CWnd wnd;
	CWnd* pWnd = CDialog::GetDlgItem(nID);

	if (pWnd)
		return pWnd;
	
	// else
	wnd.m_hWnd = NULL;
	return &wnd;
}

BOOL CPreferencesPageBase::OnEraseBkgnd(CDC* pDC)
{
	if (m_brush == NULL)
		return CPropertyPage::OnEraseBkgnd(pDC);

	// else
	CRect rClient;
	pDC->GetClipBox(rClient);	
	pDC->FillSolidRect(rClient, m_crback);

	return TRUE;
}

void CPreferencesPageBase::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);

	// resize controls to fit text
	if (m_bFirstShow)
	{
		m_bFirstShow = FALSE;
		ResizeButtonStaticTextFieldsToFit(this);
	}
}

HBRUSH CPreferencesPageBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC && m_brush != NULL)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brush;
	}
	
	return hbr;
}

void CPreferencesPageBase::OnControlChange(UINT /*nID*/)
{
	if (IsWindowVisible())
		GetParent()->SendMessage(WM_PPB_CTRLCHANGE);
}

void CPreferencesPageBase::SetBackgroundColor(COLORREF color)
{
	if (color == m_crback)
		return;

	m_crback = color;

	GraphicsMisc::VerifyDeleteObject(m_brush);

	if (color != CLR_NONE)
		m_brush = ::CreateSolidBrush(color);

	if (GetSafeHwnd())
		Invalidate(TRUE);
}


/////////////////////////////////////////////////////////////////////////////
// CPreferencesDlgBase dialog


CPreferencesDlgBase::CPreferencesDlgBase(UINT nDlgTemplateID, 
	UINT nPPHostFrameCtrlID, UINT nHelpBtnIconID, CWnd* pParent) 
	: 
	CDialog(nDlgTemplateID, pParent), 
	m_nInitPage(-1),
	m_pDoModalPrefs(NULL),
	m_nPPHostFrameCtrlID(nPPHostFrameCtrlID),
	m_nHelpBtnIconID(nHelpBtnIconID),
	m_nDlgTemplateID(nDlgTemplateID),
	m_btnHelp(nDlgTemplateID, FALSE)
{
}

BEGIN_MESSAGE_MAP(CPreferencesDlgBase, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDHELP, OnHelpButton)
	ON_WM_HELPINFO()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

void CPreferencesDlgBase::OnOK()
{
	CDialog::OnOK();
	
	m_ppHost.OnOK();

	if (m_pDoModalPrefs)
		SavePreferences(m_pDoModalPrefs);
}

void CPreferencesDlgBase::OnApply()
{
	m_ppHost.OnApply();

	if (m_pDoModalPrefs)
		SavePreferences(m_pDoModalPrefs);
}

int CPreferencesDlgBase::DoModal(IPreferences* pPrefs, int nInitPage)
{
	if (nInitPage != -1)
		m_nInitPage = nInitPage;

	// Temporary only
	m_pDoModalPrefs = pPrefs;
	
	int nRet = CDialog::DoModal();

	m_pDoModalPrefs = NULL;

	return nRet;
}

BOOL CPreferencesDlgBase::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rWindow, rClient;
	GetWindowRect(rWindow);
	GetClientRect(rClient);

	m_sizeOrgWindow = rWindow.Size();
	m_sizeCurClient = rClient.Size();

	CRect rHost = GetCtrlRect(this, m_nPPHostFrameCtrlID);
	rHost.DeflateRect(1, 1);

	VERIFY(CreatePPHost(rHost));

	if (m_btnHelp.Create(IDHELP, this) && (m_nHelpBtnIconID != 0))
	{
		HICON hIcon = GraphicsMisc::LoadIcon(m_nHelpBtnIconID);
		m_btnHelp.SetIcon(hIcon);
	}
	
	// restore previous size, ensuring it does not exceed
	// the screen's work area
	CRect rWorkArea;

	if (GraphicsMisc::GetAvailableScreenSpace(GetSafeHwnd(), rWorkArea))
	{
		int nHeight = m_pDoModalPrefs->GetProfileInt(_T("Preferences\\DialogState"), _T("Height"), rWindow.Height());
		int nWidth = m_pDoModalPrefs->GetProfileInt(_T("Preferences\\DialogState"), _T("Width"), rWindow.Width());

		rWindow.bottom = rWindow.top + min(nHeight, rWorkArea.Height());
		rWindow.right = rWindow.left + min(nWidth, rWorkArea.Width());

		MoveWindow(rWindow);
	}

	LoadPreferences(m_pDoModalPrefs);
	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPreferencesDlgBase::CreatePPHost(UINT nHostID)
{
	if (GetDlgItem(nHostID))
	{
		CRect rPPHost;
		GetDlgItem(nHostID)->GetWindowRect(rPPHost);
		ScreenToClient(rPPHost);
		
		return CreatePPHost(rPPHost);
	}
	
	// else
	return FALSE;
}

BOOL CPreferencesDlgBase::CreatePPHost(LPRECT pRect)
{
	if (m_ppHost.Create(pRect, this))
	{
		if (m_nInitPage > 0 && m_nInitPage < m_ppHost.GetPageCount())
			return SetActivePage(m_nInitPage);
		
		// else
		return TRUE;
	}
	
	// else
	return FALSE;
}

BOOL CPreferencesDlgBase::SetActivePage(int nPage)
{
	CPropertyPage* pPage = m_ppHost.GetPage(nPage);
	ASSERT (pPage);
	
	return m_ppHost.SetActivePage(pPage);
}

BOOL CPreferencesDlgBase::SetActivePage(CPreferencesPageBase* pPage)
{
	if (pPage->IsKindOf(RUNTIME_CLASS(CPreferencesPageBase)))
		return m_ppHost.SetActivePage(pPage);
	
	// else
	return FALSE;
}

BOOL CPreferencesDlgBase::AddPage(CPreferencesPageBase* pPage)
{
	if (pPage->IsKindOf(RUNTIME_CLASS(CPreferencesPageBase)))
		return m_ppHost.AddPage(pPage);
	
	// else
	return FALSE;
}

void CPreferencesDlgBase::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CPreferencesDlgBase::LoadPreferences(const IPreferences* pPrefs)
{
	ASSERT(pPrefs);

	// cycle the page loading the preferences for each one
	int nPage = m_ppHost.GetPageCount();
	
	while (nPage--)
	{
		CPreferencesPageBase* pPage = (CPreferencesPageBase*)m_ppHost.GetPage(nPage);
		ASSERT(pPage->IsKindOf(RUNTIME_CLASS(CPreferencesPageBase)));
		
		pPage->LoadPreferences(pPrefs);
	}
	
	// initial page
	if (m_nInitPage < 0 || m_nInitPage >= m_ppHost.GetPageCount())
		m_nInitPage = pPrefs->GetProfileInt(_T("Preferences\\DialogState"), _T("StartPage"), 0);
}

void CPreferencesDlgBase::SavePreferences(IPreferences* pPrefs)
{
	ASSERT(pPrefs);

	// cycle the page saving the preferences for each one
	int nPage = m_ppHost.GetPageCount();
	
	while (nPage--)
	{
		CPreferencesPageBase* pPage = (CPreferencesPageBase*)m_ppHost.GetPage(nPage);
		ASSERT(pPage->IsKindOf(RUNTIME_CLASS(CPreferencesPageBase)));
		
		pPage->SavePreferences(pPrefs);
	}
	
	if (GetSafeHwnd())
	{
		CRect rWindow;
		GetWindowRect(rWindow);

		pPrefs->WriteProfileInt(_T("Preferences\\DialogState"), _T("Height"), rWindow.Height());
		pPrefs->WriteProfileInt(_T("Preferences\\DialogState"), _T("Width"), rWindow.Width());
		pPrefs->WriteProfileInt(_T("Preferences\\DialogState"), _T("StartPage"), m_ppHost.GetActiveIndex());
	}
}

void CPreferencesDlgBase::SetPageBackgroundColor(COLORREF color)
{
	int nPage = m_ppHost.GetPageCount();
	
	while (nPage--)
	{
		CPreferencesPageBase* pPage = (CPreferencesPageBase*)m_ppHost.GetPage(nPage);
		ASSERT(pPage->IsKindOf(RUNTIME_CLASS(CPreferencesPageBase)));
		
		pPage->SetBackgroundColor(color);
	}
}

void CPreferencesDlgBase::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_ppHost.GetSafeHwnd())
		Resize(cx, cy);
}

void CPreferencesDlgBase::Resize(int cx, int cy)
{
	if (cx == 0 || cy == 0)
	{
		CRect rClient;
		GetClientRect(rClient);

		cx = rClient.Width();
		cy = rClient.Height();
	}

	// calculate deltas
	int nDX = (cx - m_sizeCurClient.cx);
	int nDY = (cy - m_sizeCurClient.cy);

	if ((nDX == 0) || (nDY == 0))
		return;

	m_sizeCurClient.cx = cx;
	m_sizeCurClient.cy = cy;

	CDeferWndMove dwm(50);
	ReposContents(dwm, nDX, nDY);
		
	GetDlgItem(m_nPPHostFrameCtrlID)->Invalidate(TRUE);
	GetDlgItem(m_nPPHostFrameCtrlID)->UpdateWindow();

	m_btnHelp.UpdatePosition();
}

void CPreferencesDlgBase::ReposContents(CDeferWndMove& dwm, int nDX, int nDY)
{
	// offset buttons
	dwm.OffsetCtrl(this, IDHELP, 0, nDY);
	dwm.OffsetCtrl(this, IDOK, nDX, nDY);
	dwm.OffsetCtrl(this, IDCANCEL, nDX, nDY);

	// PPHost and border
	dwm.ResizeCtrl(this, m_ppHost.GetDlgCtrlID(), nDX, nDY);
	dwm.ResizeCtrl(this, m_nPPHostFrameCtrlID, nDX, nDY);
}


void CPreferencesDlgBase::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CDialog::OnGetMinMaxInfo(lpMMI);

	lpMMI->ptMinTrackSize.x = m_sizeOrgWindow.cx;
	lpMMI->ptMinTrackSize.y = m_sizeOrgWindow.cy;
}

BOOL CPreferencesDlgBase::OnHelpInfo(HELPINFO* /*pHelpInfo*/)
{
	OnHelpButton();
	return TRUE;
}

void CPreferencesDlgBase::OnHelpButton() 
{
	const CPreferencesPageBase* pPage = GetActivePage();
	ASSERT(pPage);

	if (pPage && pPage->IsKindOf(RUNTIME_CLASS(CPreferencesPageBase)))
		AfxGetApp()->WinHelp(pPage->GetHelpID());
	else
		AfxGetApp()->WinHelp(m_nDlgTemplateID);
}

/////////////////////////////////////////////////////////////////////////////

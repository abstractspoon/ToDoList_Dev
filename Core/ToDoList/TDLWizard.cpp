// WizardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLWizard.h"

#include "..\shared\GraphicsMisc.h"
#include "..\shared\DialogHelper.h"
#include "..\shared\Misc.h"
#include "..\shared\Icon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardDlg dialog

const int PSH_WIZARD97_EX = 0x01000000;
const int IDC_TOPDIVIDERID = 0x3027;
const int PROGRESS_INCREMENT = 10;
const int PROGRESS_HEIGHT = GraphicsMisc::ScaleByDPIFactor(3);
const int TIMERID_ANIMATEBACK = 1;

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTDLWizard, CPropertySheetEx)

CTDLWizard::CTDLWizard()
{
	InitSheet(NULL, 0); 
}

CTDLWizard::CTDLWizard(UINT nIDCaption, UINT nIconID) : CPropertySheetEx()
{
	InitSheet(CEnString(nIDCaption), nIconID);
}

CTDLWizard::CTDLWizard(LPCTSTR pszCaption, UINT nIconID) : CPropertySheetEx()
{
	InitSheet(pszCaption, nIconID);
}

BEGIN_MESSAGE_MAP(CTDLWizard, CPropertySheetEx)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_GETFONT, OnGetFont)
END_MESSAGE_MAP()

// --------------------------------------------------------------------------

void CTDLWizard::InitSheet(LPCTSTR szTitle, UINT nIconID)
{
	m_bProgressEnabled = TRUE;
	m_bAutoAdvance = TRUE;
	m_nNumSteps = -1;
	m_nCurStep = 0;
	m_strCaption = szTitle;

	VERIFY(GraphicsMisc::CreateFont(m_font, _T("Tahoma")));
	
	VERIFY(m_bmHeader.Attach(GraphicsMisc::MakeWizardImage(CIcon(IDR_MAINFRAME, 48, FALSE))));
	m_psh.hbmHeader = m_bmHeader;

	m_psh.dwFlags |= PSH_WIZARD97_EX | PSH_HEADER | PSH_USEHBMHEADER;
	m_psh.dwFlags &= ~(PSH_HASHELP);

	if (nIconID)
	{
		m_psh.pszIcon = MAKEINTRESOURCE(nIconID);
		m_psh.dwFlags |= PSH_USEICONID;
	}

	m_psh.hInstance = AfxGetInstanceHandle();
}

BOOL CTDLWizard::OnInitDialog() 
{
	CPropertySheetEx::OnInitDialog();

	SetFont(&m_font);

	// Create progress bar
	if (m_bProgressEnabled)
	{
		CRect rProgress = CDialogHelper::GetCtrlRect(this, IDC_TOPDIVIDERID);

		rProgress.top = rProgress.bottom;
		rProgress.bottom += PROGRESS_HEIGHT;

		VERIFY(m_wndProgress.Create(WS_CHILD | WS_VISIBLE, rProgress, this, IDC_STATIC));
		m_nCurStep = -1;

		if (m_bAutoAdvance)
			m_nNumSteps = GetPageCount();

		m_wndProgress.SetRange(0, (PROGRESS_INCREMENT * m_nNumSteps));
		
		if (m_bAutoAdvance)
			IncrementProgress();
	}

	if (!m_strCaption.IsEmpty())
		SetWindowText(m_strCaption);

	return TRUE;
}

void CTDLWizard::EnableProgressBar(BOOL bEnable)
{
	ASSERT(!GetSafeHwnd());

	m_bProgressEnabled = bEnable;
	m_bAutoAdvance = bEnable;
	m_nNumSteps = -1; // Initialised in OnInitDialog
}

void CTDLWizard::EnableProgressBar(BOOL bEnable, int nNumSteps)
{
	ASSERT(!GetSafeHwnd());
	ASSERT(!bEnable || (nNumSteps >= 1));

	m_bProgressEnabled = TRUE;
	m_bAutoAdvance = FALSE;
	m_nNumSteps = nNumSteps;
}

BOOL CTDLWizard::OnCommand(WPARAM wParam, LPARAM lParam)
{
	BOOL bRes = CPropertySheetEx::OnCommand(wParam, lParam);

	if (HIWORD(wParam) == BN_CLICKED)
	{
		CDialogHelper::SetFont(GetActivePage(), m_font, FALSE);

		switch (LOWORD(wParam))
		{
		case ID_WIZBACK:
			if (m_bAutoAdvance)
				DecrementProgress();
			break;

		case ID_WIZNEXT:
			if (m_bAutoAdvance)
				IncrementProgress();
			break;

		case ID_WIZFINISH:
			for (int nPage = 0; nPage < GetPageCount(); nPage++)
				GetPage(nPage)->UpdateData();
			break;
		}
	}

	return bRes;
}

void CTDLWizard::IncrementProgress()
{
	if (m_nCurStep > (m_nNumSteps - 1))
		return;

	m_nCurStep++;

	m_wndProgress.OffsetPos(PROGRESS_INCREMENT);
}

void CTDLWizard::DecrementProgress()
{
	if (m_nCurStep == 0)
		return;

	m_nCurStep--;

	// The progress control doesn't animate for backwards
	// moves so we must do it ourselves
	SetTimer(TIMERID_ANIMATEBACK, (400 / PROGRESS_INCREMENT), NULL);
}

void CTDLWizard::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMERID_ANIMATEBACK)
	{
		int nPos = m_wndProgress.GetPos() - 1;

		m_wndProgress.SetPos(nPos);
		m_wndProgress.UpdateWindow();

		if (nPos == ((m_nCurStep + 1) * PROGRESS_INCREMENT))
			KillTimer(nIDEvent);
	}
	else
	{
		CPropertySheetEx::OnTimer(nIDEvent);
	}
}

BOOL CTDLWizard::OnEraseBkgnd(CDC* pDC)
{
	// Reduce flicker when navigating
	CDialogHelper::ExcludeCtrls(this, pDC);

	return CPropertySheetEx::OnEraseBkgnd(pDC);
}

/////////////////////////////////////////////////////////////////////////////

CTDLWizardPage::CTDLWizardPage(UINT nIDTemplate) : CPropertyPageEx(nIDTemplate)
{
}

BOOL CTDLWizardPage::OnSetActive()
{
	CDialogHelper::SetFont(this, GraphicsMisc::GetFont(::GetParent(*this)));

	return CPropertyPageEx::OnSetActive();
}

void CTDLWizardPage::SetWizardButtons(DWORD dwBtns)
{
	GetParent()->PostMessage(PSM_SETWIZBUTTONS, 0, dwBtns);
}

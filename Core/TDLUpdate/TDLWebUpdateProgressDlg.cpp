// WebUpdateProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tdlupdate.h"
#include "TDLWebUpdateProgressDlg.h"

#include "..\Shared\EnString.h"
#include "..\Shared\misc.h"
#include "..\Shared\DialogHelper.h"
#include "..\Shared\GraphicsMisc.h"
#include "..\Shared\filemisc.h"
#include "..\Shared\windowicons.h"
#include "..\Shared\icon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef LVS_EX_DOUBLEBUFFER
#	define LVS_EX_DOUBLEBUFFER  0x00010000
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLWebUpdateProgressDlg dialog

CTDLWebUpdateProgressDlg::CTDLWebUpdateProgressDlg(const CPoint& ptPos)
	: 
	CTDLWizard(IDS_WEBUPDATE_TITLE, IDR_MAINFRAME),
	m_ptInitialPos(ptPos)
{
	AddPage(&m_page);

	EnableProgressBar(TRUE, TDLWP_NUMSTATES);
}

CTDLWebUpdateProgressDlg::~CTDLWebUpdateProgressDlg()
{
}

BEGIN_MESSAGE_MAP(CTDLWebUpdateProgressDlg, CTDLWizard)
	ON_WM_CLOSE()
	ON_COMMAND(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------

void CTDLWebUpdateProgressDlg::OnCancel() 
{ 
	if (m_bModeless)
		m_bCancelled = TRUE; 
	else
		EndDialog(IDCANCEL);
}

void CTDLWebUpdateProgressDlg::SetProgressStatus(TDL_WEBUPDATE_PROGRESS nStatus, int nPercent)
{
	TDL_WEBUPDATE_PROGRESS nCurStatus = m_page.GetProgressStatus();
	m_page.SetProgressStatus(nStatus, nPercent);

	if (nStatus != nCurStatus)
	{
		IncrementProgress();
		Misc::ProcessMsgLoop(1000); // allow animation to finish

		// Hide 'Cancel' button from 'Copy' stage onwards
		GetDlgItem(IDCANCEL)->EnableWindow(nStatus < TDLWP_COPY);

		// Prevent the 'Next' button from repeatedly reappearing
		GetDlgItem(ID_WIZNEXT)->ShowWindow(SW_HIDE);
	}
}

TDL_WEBUPDATE_PROGRESS CTDLWebUpdateProgressDlg::GetProgressStatus() const
{
	return m_page.GetProgressStatus();
}

BOOL CTDLWebUpdateProgressDlg::OnInitDialog() 
{
	// set taskbar icon
	m_icons.Initialise(*this, IDR_MAINFRAME);

	// set dialog icon
	SetIcon(m_icons.GetSmallIcon(), FALSE);
	
	CTDLWizard::OnInitDialog();

	SetWizardButtons(0);
	
	// hide back/next buttons
	GetDlgItem(ID_WIZBACK)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_WIZNEXT)->ShowWindow(SW_HIDE);

	// focus cancel button
	GetDlgItem(IDCANCEL)->SetFocus();

	// reposition the dialog before showing
	if ((GetSystemMetrics(SM_CMONITORS) > 1) && (m_ptInitialPos != CPoint(0, 0)))
	{
		CRect rWindow;
		GetWindowRect(rWindow);

		rWindow.OffsetRect(m_ptInitialPos - rWindow.CenterPoint());
		MoveWindow(rWindow);
	}

	m_bCancelled = FALSE;

	return FALSE;
}

void CTDLWebUpdateProgressDlg::OnClose()
{
	ShowWindow(SW_HIDE);

	OnCancel();
}

BOOL CTDLWebUpdateProgressDlg::IsCancelled() const 
{ 
	Misc::ProcessMsgLoop();

	return m_bCancelled; 
}

/////////////////////////////////////////////////////////////////////////////
// CTDLWebUpdateProgressPage dialog

const int PADDING = 10;

enum // list columns
{
	ITEM_COL,
	DESCRIPTION_COL,
	STATUS_COL
};

// --------------------------------------------------------------

IMPLEMENT_DYNCREATE(CTDLWebUpdateProgressPage, CPropertyPageEx)

CTDLWebUpdateProgressPage::CTDLWebUpdateProgressPage()
	: 
	CPropertyPageEx(IDD_WEBUPDATE_PROGRESS_PAGE, 0),
	m_nStatus(TDLWP_NONE)
{
	m_psp.dwFlags &= ~(PSP_HASHELP);
	
	m_strHeaderTitle.Format(CEnString(IDS_WEBUPDATE_PROGRESSHEADER), Misc::GetUserName());
	m_strHeaderSubTitle = "\n" + CEnString(IDS_WEBUPDATE_PROGRESS);

	m_sDone = CEnString(IDS_WEBUPDATEDONE);

	m_aProgressDescriptions.Add(CEnString(IDS_WEBUPDATEPROGRESS_DOWNLOAD));
	m_aProgressDescriptions.Add(CEnString(IDS_WEBUPDATEPROGRESS_UNZIP));
	m_aProgressDescriptions.Add(CEnString(IDS_WEBUPDATEPROGRESS_BACKUP));
	m_aProgressDescriptions.Add(CEnString(IDS_WEBUPDATEPROGRESS_COPY));
	m_aProgressDescriptions.Add(CEnString(IDS_WEBUPDATEPROGRESS_CLEANUP));
	m_aProgressDescriptions.Add(_T("")); // completion doesn't display a string

	ASSERT(m_aProgressDescriptions.GetSize() == TDLWP_NUMSTATES);
}


void CTDLWebUpdateProgressPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PROGRESS, m_lcProgress);
}


BEGIN_MESSAGE_MAP(CTDLWebUpdateProgressPage, CPropertyPageEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS, OnProgressCustomDraw)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------

BOOL CTDLWebUpdateProgressPage::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();

	m_nStatus = TDLWP_NONE;

	// Calculate longest column strings
	CClientDC dc(&m_lcProgress);
	HFONT hOldFont = GraphicsMisc::PrepareDCFont(&dc, m_lcProgress);

	CString sMaxItem = _T("10."), sMaxDesc;

	// progress descriptions
	int nItem = m_aProgressDescriptions.GetSize(), nMaxDescLen = 0;

	while (nItem--)
	{
		int nItemLen = dc.GetTextExtent(m_aProgressDescriptions[nItem]).cx;

		if (nItemLen > nMaxDescLen)
		{
			nMaxDescLen = nItemLen;
			sMaxDesc = m_aProgressDescriptions[nItem];
		}
	}

	// Progress status
	CString sMaxStatus = _T("100%");

	if (dc.GetTextExtent(sMaxStatus).cx < dc.GetTextExtent(m_sDone).cx)
		sMaxStatus = m_sDone;
	
	dc.SelectObject(hOldFont);

	// Create progress columns
	m_lcProgress.InsertColumn(ITEM_COL, sMaxItem);
	m_lcProgress.InsertColumn(DESCRIPTION_COL, sMaxDesc);
	m_lcProgress.InsertColumn(STATUS_COL, sMaxStatus);

	// Use auto-sizing must come after creation of all columns
	m_lcProgress.SetColumnWidth(ITEM_COL, LVSCW_AUTOSIZE_USEHEADER);
	m_lcProgress.SetColumnWidth(DESCRIPTION_COL, LVSCW_AUTOSIZE_USEHEADER);
	m_lcProgress.SetColumnWidth(STATUS_COL, LVSCW_AUTOSIZE_USEHEADER);

	// Reduce flicker during % updates
	ListView_SetExtendedListViewStyleEx(m_lcProgress, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);

	return TRUE;
}

BOOL CTDLWebUpdateProgressPage::OnSetActive() 
{
	CPropertyPageEx::OnSetActive();
	
	// set tabstops
	UINT nTabStop = 16;
	GetDlgItem(IDC_PROGRESS)->SendMessage(EM_SETTABSTOPS, 1, (LPARAM)&nTabStop);
	
	return TRUE;
}

void CTDLWebUpdateProgressPage::SetProgressStatus(TDL_WEBUPDATE_PROGRESS nStatus, int nPercent)
{
	ASSERT(nStatus < m_aProgressDescriptions.GetSize());

	int nCurItem = (m_lcProgress.GetItemCount() - 1);

	if (nStatus != m_nStatus)
	{
		ASSERT(nPercent == 0);

		if (m_nStatus != TDLWP_NONE)
		{
			// Set previous item done
			ASSERT(nCurItem == (nStatus - 1));

			m_lcProgress.SetItemText(nCurItem, STATUS_COL, m_sDone);
		}

		// Add new item
		m_nStatus = nStatus;

		if (nStatus != TDLWP_COMPLETE)
		{
			nCurItem = m_lcProgress.InsertItem((m_lcProgress.GetItemCount()), _T(""));

			m_lcProgress.SetItemData(nCurItem, nStatus);
			m_lcProgress.SetItemText(nCurItem, ITEM_COL, Misc::Format(_T("%d."), nCurItem + 1)); // one-based
			m_lcProgress.SetItemText(nCurItem, DESCRIPTION_COL, m_aProgressDescriptions[nCurItem]);
		}
	}

	if ((nStatus == TDLWP_DOWNLOAD) || (nStatus == TDLWP_UNZIP))
		m_lcProgress.SetItemText(nCurItem, STATUS_COL, Misc::Format(_T("%d%%"), nPercent));

	m_lcProgress.UpdateWindow();
}

void CTDLWebUpdateProgressPage::OnProgressCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = CDRF_DODEFAULT;

	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		{
			CRect rClient;
			m_lcProgress.GetClientRect(rClient);

			::FillRect(pLVCD->nmcd.hdc, rClient, ::GetSysColorBrush(COLOR_WINDOW));
		}
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		pLVCD->clrTextBk = GetSysColor(COLOR_WINDOW);
		*pResult = CDRF_NEWFONT;
		break;
	}
}

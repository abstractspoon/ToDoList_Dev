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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#define PSH_WIZARD97_EX 0x01000000

#ifndef LVS_EX_DOUBLEBUFFER
#	define LVS_EX_DOUBLEBUFFER  0x00010000
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef BINDSTATUS_ACCEPTRANGES
#	define BINDSTATUS_ACCEPTRANGES (BINDSTATUS_LOADINGMIMEHANDLER + 8)
#endif

enum
{
	UF_BINDSTATUS_FIRST = BINDSTATUS_FINDINGRESOURCE,
	UF_BINDSTATUS_LAST = BINDSTATUS_ACCEPTRANGES
};

/////////////////////////////////////////////////////////////////////////////
// CTDLWebUpdateProgressDlg dialog

CTDLWebUpdateProgressDlg::CTDLWebUpdateProgressDlg(const CPoint& ptPos)
	: 
	CPropertySheetEx(_T(""), NULL, 0),
	m_ptInitialPos(ptPos)
{
	m_hFont = GraphicsMisc::CreateFont(_T("Tahoma"));
	
	m_page.AttachFont(m_hFont);
	AddPage(&m_page);
	
	m_psh.dwFlags |= PSH_WIZARD97_EX | PSH_HEADER | PSH_USEICONID/* | PSH_WATERMARK*/;		
	m_psh.dwFlags &= ~(PSH_HASHELP);
	
	m_psh.hInstance = AfxGetInstanceHandle(); 
	m_psh.pszIcon = MAKEINTRESOURCE(IDR_MAINFRAME);
	m_psh.pszbmHeader = MAKEINTRESOURCE(IDB_WIZ_HEADER);
	
	SetWizardMode();
}

BEGIN_MESSAGE_MAP(CTDLWebUpdateProgressDlg, CPropertySheetEx)
	//{{AFX_MSG_MAP(CTDLWebUpdateProgressDlg)
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_COMMAND(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLWebUpdateProgressDlg message handlers

void CTDLWebUpdateProgressDlg::OnCancel() 
{ 
	if (m_bModeless)
		m_bCancelled = TRUE; 
	else
		EndDialog(IDCANCEL);
}

void CTDLWebUpdateProgressDlg::SetProgressStatus(TDL_WEBUPDATE_PROGRESS nStatus)
{
	m_page.SetProgressStatus(nStatus);

	// extra handling
	switch (nStatus)
	{
	case TDLWP_COPY:
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		break;

	case TDLWP_COMPLETE:
		// Pause for a moment
		Sleep(1000);
		break;
	}

	// for some reason the 'Next' button keeps reappearing
	GetDlgItem(ID_WIZNEXT)->ShowWindow(SW_HIDE);
}

TDL_WEBUPDATE_PROGRESS CTDLWebUpdateProgressDlg::GetProgressStatus() const
{
	return m_page.GetProgressStatus();
}

BOOL CTDLWebUpdateProgressDlg::OnInitDialog() 
{
	CDialogHelper::SetFont(this, m_hFont);

	// set taskbar icon
	m_icons.Initialise(*this, IDR_MAINFRAME);

	// set dialog icon
	SetIcon(m_icons.GetSmallIcon(), FALSE);
	
	CPropertySheetEx::OnInitDialog();

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
// IUnknown
STDMETHODIMP CTDLWebUpdateProgressDlg::QueryInterface(REFIID riid, void **ppvObject)
{
	TRACE(_T("IUnknown::QueryInterface\n"));

	*ppvObject = NULL;
	
	// IUnknown
	if (::IsEqualIID(riid, __uuidof(IUnknown)))
	{
		TRACE(_T("IUnknown::QueryInterface(IUnknown)\n"));

		*ppvObject = this;
	}
	// IBindStatusCallback
	else if (::IsEqualIID(riid, __uuidof(IBindStatusCallback)))
	{
		TRACE(_T("IUnknown::QueryInterface(IBindStatusCallback)\n"));

		*ppvObject = static_cast<IBindStatusCallback *>(this);
	}

	if (*ppvObject)
	{
		(*reinterpret_cast<LPUNKNOWN *>(ppvObject))->AddRef();

		return S_OK;
	}
	
	return E_NOINTERFACE;
}                                             

STDMETHODIMP_(ULONG) CTDLWebUpdateProgressDlg::AddRef()
{
	TRACE(_T("IUnknown::AddRef\n"));

	return ++m_ulObjRefCount;
}

STDMETHODIMP_(ULONG) CTDLWebUpdateProgressDlg::Release()
{
	TRACE(_T("IUnknown::Release\n"));

	return --m_ulObjRefCount;
}

/////////////////////////////////////////////////////////////////////////////
// IBindStatusCallback
STDMETHODIMP CTDLWebUpdateProgressDlg::OnStartBinding(DWORD, IBinding *)
{
	TRACE(_T("IBindStatusCallback::OnStartBinding\n"));

	return S_OK;
}

STDMETHODIMP CTDLWebUpdateProgressDlg::GetPriority(LONG *)
{
	TRACE(_T("IBindStatusCallback::GetPriority\n"));

	return E_NOTIMPL;
}

STDMETHODIMP CTDLWebUpdateProgressDlg::OnLowResource(DWORD)
{
	TRACE(_T("IBindStatusCallback::OnLowResource\n"));

	return S_OK;
}

STDMETHODIMP CTDLWebUpdateProgressDlg::OnProgress(ULONG ulProgress,
										 ULONG ulProgressMax,
										 ULONG ulStatusCode,
										 LPCWSTR /*szStatusText*/)
{
#ifdef _DEBUG
	static const LPCTSTR plpszStatus[] = 
	{
		_T("BINDSTATUS_FINDINGRESOURCE"),  // 1
		_T("BINDSTATUS_CONNECTING"),
		_T("BINDSTATUS_REDIRECTING"),
		_T("BINDSTATUS_BEGINDOWNLOADDATA"),
		_T("BINDSTATUS_DOWNLOADINGDATA"),
		_T("BINDSTATUS_ENDDOWNLOADDATA"),
		_T("BINDSTATUS_BEGINDOWNLOADCOMPONENTS"),
		_T("BINDSTATUS_INSTALLINGCOMPONENTS"),
		_T("BINDSTATUS_ENDDOWNLOADCOMPONENTS"),
		_T("BINDSTATUS_USINGCACHEDCOPY"),
		_T("BINDSTATUS_SENDINGREQUEST"),
		_T("BINDSTATUS_CLASSIDAVAILABLE"),
		_T("BINDSTATUS_MIMETYPEAVAILABLE"),
		_T("BINDSTATUS_CACHEFILENAMEAVAILABLE"),
		_T("BINDSTATUS_BEGINSYNCOPERATION"),
		_T("BINDSTATUS_ENDSYNCOPERATION"),
		_T("BINDSTATUS_BEGINUPLOADDATA"),
		_T("BINDSTATUS_UPLOADINGDATA"),
		_T("BINDSTATUS_ENDUPLOADINGDATA"),
		_T("BINDSTATUS_PROTOCOLCLASSID"),
		_T("BINDSTATUS_ENCODING"),
		_T("BINDSTATUS_VERFIEDMIMETYPEAVAILABLE"),
		_T("BINDSTATUS_CLASSINSTALLLOCATION"),
		_T("BINDSTATUS_DECODING"),
		_T("BINDSTATUS_LOADINGMIMEHANDLER"),
		_T("BINDSTATUS_CONTENTDISPOSITIONATTACH"),
		_T("BINDSTATUS_FILTERREPORTMIMETYPE"),
		_T("BINDSTATUS_CLSIDCANINSTANTIATE"),
		_T("BINDSTATUS_IUNKNOWNAVAILABLE"),
		_T("BINDSTATUS_DIRECTBIND"),
		_T("BINDSTATUS_RAWMIMETYPE"),
		_T("BINDSTATUS_PROXYDETECTING"),
		_T("BINDSTATUS_ACCEPTRANGES"),
		_T("???")  // unknown
	};

// 	TRACE(_T("IBindStatusCallback::OnProgress\n"));
// 	TRACE(_T("ulProgress: %lu, ulProgressMax: %lu\n"), ulProgress, ulProgressMax);
// 	TRACE(_T("ulStatusCode: %lu "), ulStatusCode);
#endif

	if (ulStatusCode < UF_BINDSTATUS_FIRST ||
		ulStatusCode > UF_BINDSTATUS_LAST)
	{
		ulStatusCode = UF_BINDSTATUS_LAST + 1;
	}
	
//	TRACE(_T("(%s), szStatusText: %ls\n"), plpszStatus[ulStatusCode - UF_BINDSTATUS_FIRST], szStatusText);

	if (m_hWnd && m_page.IsDownloading())
	{
		switch (ulStatusCode)
		{
		case BINDSTATUS_BEGINDOWNLOADDATA:
		case BINDSTATUS_DOWNLOADINGDATA:
		case BINDSTATUS_ENDDOWNLOADDATA:
			m_page.SetDownloadPercent((int)MulDiv(ulProgress, 100, ulProgressMax));
			break;
		}
		
		Misc::ProcessMsgLoop();
	}

	return S_OK;
}

STDMETHODIMP CTDLWebUpdateProgressDlg::OnStopBinding(HRESULT, LPCWSTR)
{
	TRACE(_T("IBindStatusCallback::OnStopBinding\n"));

	return S_OK;
}

STDMETHODIMP CTDLWebUpdateProgressDlg::GetBindInfo(DWORD *, BINDINFO *)
{
	TRACE(_T("IBindStatusCallback::GetBindInfo\n"));

	return S_OK;
}

STDMETHODIMP CTDLWebUpdateProgressDlg::OnDataAvailable(DWORD, DWORD,
											  FORMATETC *, STGMEDIUM *)
{
	TRACE(_T("IBindStatusCallback::OnDataAvailable\n"));

	return S_OK;
}

STDMETHODIMP CTDLWebUpdateProgressDlg::OnObjectAvailable(REFIID, IUnknown *)
{
	TRACE(_T("IBindStatusCallback::OnObjectAvailable\n"));

	return S_OK;
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

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CTDLWebUpdateProgressPage, CPropertyPageEx)

CTDLWebUpdateProgressPage::CTDLWebUpdateProgressPage()
: CPropertyPageEx(IDD_WEBUPDATE_PROGRESS_PAGE, 0)
{
	//{{AFX_DATA_INIT(CTDLWebUpdateProgressPage)
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~(PSP_HASHELP);
	
	m_strHeaderTitle.Format(CEnString(IDS_WEBUPDATE_PROGRESSHEADER), Misc::GetUserName());
	m_strHeaderSubTitle = CEnString(IDS_WEBUPDATE_PROGRESS);

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
	//{{AFX_DATA_MAP(CTDLWebUpdateProgressPage)
	DDX_Control(pDX, IDC_PROGRESS, m_lcProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLWebUpdateProgressPage, CPropertyPageEx)
//{{AFX_MSG_MAP(CTDLWebUpdateProgressPage)
//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS, OnProgressCustomDraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLWebUpdateProgressPage message handlers

BOOL CTDLWebUpdateProgressPage::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();

	CDialogHelper::SetFont(this, m_hFont);

	// Calculate longest column strings
	CClientDC dc(&m_lcProgress);
	HGDIOBJ hOldFont = dc.SelectObject(m_hFont);

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

void CTDLWebUpdateProgressPage::SetProgressStatus(TDL_WEBUPDATE_PROGRESS nStatus)
{
	ASSERT(nStatus < m_aProgressDescriptions.GetSize());

	m_nStatus = nStatus;

	// Finish off previous item
	int nCurItem = (m_lcProgress.GetItemCount() - 1);

	if (nCurItem >= 0)
		m_lcProgress.SetItemText(nCurItem, STATUS_COL, m_sDone);
	
	// Add new item
	int nNewItem = m_lcProgress.InsertItem((nCurItem + 1), _T(""));

	m_lcProgress.SetItemText(nNewItem, ITEM_COL, Misc::Format(_T("%d."), nNewItem + 1)); // one-based
	m_lcProgress.SetItemText(nNewItem, DESCRIPTION_COL, m_aProgressDescriptions[nNewItem]);

	if (nNewItem == 0)
		m_lcProgress.SetItemText(nNewItem, STATUS_COL, _T("0%"));

	m_lcProgress.UpdateWindow();
}

BOOL CTDLWebUpdateProgressPage::SetDownloadPercent(int nPercent)
{
	if (!IsDownloading() || (nPercent < 0) || (nPercent > 100))
	{
		ASSERT(0);
		return FALSE;
	}

	m_lcProgress.SetItemText(0, STATUS_COL, Misc::Format(_T("%d%%"), nPercent));
	return TRUE;
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

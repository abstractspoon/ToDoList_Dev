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
	m_nStatus = nStatus;

	// extra handling
	switch (m_nStatus)
	{
	case TDLWP_UNZIP:
		AddProgress(IDS_WEBUPDATEPROGRESS_UNZIP);
		break;

	case TDLWP_BACKUP:
		AddProgress(IDS_WEBUPDATEPROGRESS_BACKUP);
		break;
		
	case TDLWP_COPY:
		// can't cancel from here on
		{
			AddProgress(IDS_WEBUPDATEPROGRESS_COPY);

			GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		}
		break;

	case TDLWP_CLEANUP:
		AddProgress(IDS_WEBUPDATEPROGRESS_CLEANUP);
		break;
		
	case TDLWP_COMPLETE:
		AddProgress(_T(""));
		break;
	}
}

void CTDLWebUpdateProgressDlg::AddProgress(UINT nIDProgress)
{
	AddProgress(CEnString(nIDProgress));
}

void CTDLWebUpdateProgressDlg::AddProgress(const CString& sProgress)
{
	// add 'Done' to last element
	if (m_aProgress.GetSize())
	{
		int nLast = (m_aProgress.GetSize() - 1);
		
		m_aProgress[nLast] += ' ';
		m_aProgress[nLast] += CEnString(IDS_WEBUPDATEDONE);
	}
	
	if (!sProgress.IsEmpty())
		m_aProgress.Add(sProgress);
	
	m_page.SetProgress(m_aProgress);
	
	// for some reason the 'Next' button keeps reappearing
	GetDlgItem(ID_WIZNEXT)->ShowWindow(SW_HIDE);
}

BOOL CTDLWebUpdateProgressDlg::OnInitDialog() 
{
	CDialogHelper::SetFont(this, m_hFont);

	// set dialog icon
	HICON hIcon = GraphicsMisc::LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, FALSE);
	
	// set taskbar icon
	m_icons.Initialise(*this, IDR_MAINFRAME);

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

	// initialise progress
	AddProgress(CEnString(IDS_WEBUPDATEPROGRESS_DOWNLOAD, 0));

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

	if (m_hWnd && (m_nStatus == TDLWP_DOWNLOAD))
	{
		switch (ulStatusCode)
		{
		case BINDSTATUS_BEGINDOWNLOADDATA:
		case BINDSTATUS_DOWNLOADINGDATA:
		case BINDSTATUS_ENDDOWNLOADDATA:
			{
				CString sProgress;
				sProgress.Format(CEnString(IDS_WEBUPDATEPROGRESS_DOWNLOAD), MulDiv(ulProgress, 100, ulProgressMax));

				if ((m_aProgress.GetSize() == 0) || 
					(m_aProgress[0] != sProgress))
				{
					m_aProgress.RemoveAll();
					m_aProgress.Add(sProgress);

					m_page.SetProgress(m_aProgress);
				}
			}
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

IMPLEMENT_DYNCREATE(CTDLWebUpdateProgressPage, CPropertyPageEx)

CTDLWebUpdateProgressPage::CTDLWebUpdateProgressPage()
: CPropertyPageEx(IDD_WEBUPDATE_PROGRESS_PAGE, 0)
{
	//{{AFX_DATA_INIT(CTDLWebUpdateProgressPage)
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~(PSP_HASHELP);
	
	m_strHeaderTitle.Format(CEnString(IDS_WEBUPDATE_PROGRESSHEADER), Misc::GetUserName());
	m_strHeaderSubTitle = CEnString(IDS_WEBUPDATE_PROGRESS);
}


void CTDLWebUpdateProgressPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLWebUpdateProgressPage)
	DDX_Text(pDX, IDC_PROGRESS, m_sProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLWebUpdateProgressPage, CPropertyPageEx)
//{{AFX_MSG_MAP(CTDLWebUpdateProgressPage)
ON_WM_CTLCOLOR()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLWebUpdateProgressPage message handlers

BOOL CTDLWebUpdateProgressPage::OnInitDialog() 
{
	CDialogHelper::SetFont(this, m_hFont);
	
	return CPropertyPageEx::OnInitDialog();
}

HBRUSH CTDLWebUpdateProgressPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hBrush = CPropertyPageEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_PROGRESS)
	{
 		pDC->SetBkMode(TRANSPARENT);
 		hBrush = GetSysColorBrush(COLOR_WINDOW);
 	}
	
	return hBrush;
}

BOOL CTDLWebUpdateProgressPage::OnSetActive() 
{
	CPropertyPageEx::OnSetActive();
	
	// set tabstops
	UINT nTabStop = 16;
	GetDlgItem(IDC_PROGRESS)->SendMessage(EM_SETTABSTOPS, 1, (LPARAM)&nTabStop);
	
	return TRUE;
}

void CTDLWebUpdateProgressPage::SetProgress(const CStringArray& aProgress)
{
	m_sProgress = Misc::FormatArrayAsNumberedList(aProgress, _T(".\t"));
	
	// replace '\n' with '\r\n'
	m_sProgress.Replace(_T("\n"), _T("\r\n"));
	UpdateData(FALSE);

	// set cursor to end of text
	int nSel = m_sProgress.GetLength();

	GetDlgItem(IDC_PROGRESS)->SendMessage(EM_SETSEL, nSel, nSel);
	GetDlgItem(IDC_PROGRESS)->Invalidate();
	GetDlgItem(IDC_PROGRESS)->UpdateWindow();
}

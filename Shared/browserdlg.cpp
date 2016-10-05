// browserdlg.cpp : implementation file
//

#include "stdafx.h"
#include "browserdlg.h"
#include "misc.h"

#include "..\3rdParty\stdiofileex.h"

#include <afxdisp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrowserDlg dialog


CBrowserDlg::CBrowserDlg(BOOL bBrowser, LPCTSTR szSettingsKey) 
: CRuntimeDlg(NULL, szSettingsKey), m_bBrowser(bBrowser), m_bHideOnClose(FALSE)
{
	//{{AFX_DATA_INIT(CBrowserDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AddControl(_T("static"), _T(""), WS_CHILD, 0, 0, 0, 0, 0, (UINT)-1);
}


void CBrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrowserDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBrowserDlg, CDialog)
	//{{AFX_MSG_MAP(CBrowserDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_EN_SETFOCUS(1, OnEditSetFocus)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrowserDlg message handlers

int CBrowserDlg::DoModal(LPCTSTR szCaption, LPCTSTR szUrlPath, CWnd* pParentWnd)
{
	if (Misc::IsEmpty(szUrlPath))
		return IDCANCEL;

	m_sUrl = szUrlPath;

	return CRuntimeDlg::DoModal(szCaption, (DWORD)RTD_DEFSTYLE | WS_THICKFRAME, (DWORD)RTD_DEFEXSTYLE,
								rectAuto, pParentWnd);
}

BOOL CBrowserDlg::Create(LPCTSTR szCaption, LPCTSTR szUrlPath, CWnd* pParentWnd)
{
	if (Misc::IsEmpty(szUrlPath))
		return IDCANCEL;

	m_sUrl = szUrlPath;

	return CRuntimeDlg::Create(szCaption, (DWORD)RTD_DEFSTYLE | WS_THICKFRAME, (DWORD)RTD_DEFEXSTYLE,
								rectAuto, pParentWnd);
}

BOOL CBrowserDlg::Navigate(LPCTSTR szUrlPath, LPCTSTR szCaption)
{
	if (!Misc::IsEmpty(szCaption))
		SetWindowText(szCaption);

	if (m_bBrowser)
	{
		ASSERT(m_browser.GetSafeHwnd());

		COleVariant vFlags(0L), vFrame(_T("")), vData(_T("")), vHeaders(_T(""));
		m_browser.Navigate(szUrlPath, vFlags, vFrame, vData, vHeaders);
	}
	else
	{
		ASSERT(m_edit.GetSafeHwnd());

		CStdioFileEx file;
		
		if (file.Open(szUrlPath, CFile::modeRead | CFile::typeText))
		{
			CString sLine, sFile;
			
			while (file.ReadString(sLine))
			{
				sFile += sLine;
				sFile += '\n';
			}
			
			// replace carriage returns by linefeed/cr because that's
			// what edit control prefer
			sFile.Replace(_T("\n"), _T("\r\n"));
			
			m_edit.SetWindowText(sFile);
		}
	}

	return TRUE;
}

BOOL CBrowserDlg::OnInitDialog() 
{
	CRuntimeDlg::OnInitDialog();

	VERIFY (SetDisplayInBrowser(m_bBrowser));
	VERIFY (Navigate(m_sUrl));

	if (m_sSettingsKey.IsEmpty())
	{
		AutoFit();
		CenterWindow();
	}
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBrowserDlg::SetDisplayInBrowser(BOOL bBrowser) 
{ 
	if (!GetSafeHwnd())
	{
		m_bBrowser = bBrowser;
		return TRUE;
	}

	// if nothing has changed just return
	// or destroy any existing window
	if (bBrowser)
	{
		if (m_browser.GetSafeHwnd())
		{
			ASSERT(m_bBrowser);
			return TRUE;
		}
		else if (m_edit.GetSafeHwnd())
		{
			m_edit.DestroyWindow();
		}
	}
	else
	{
		if (m_edit.GetSafeHwnd())
		{
			ASSERT(!m_bBrowser);
			return TRUE;
		}
		else if (m_browser.GetSafeHwnd())
		{
			m_browser.DestroyWindow();
		}
	}

	m_bBrowser = bBrowser; 

	CRect rClient;
	GetClientRect(rClient);

	if (rClient.IsRectEmpty())
		rClient.SetRect(0, 0, 600, 400);

	if (bBrowser)
	{
		AfxEnableControlContainer();
		
		return m_browser.Create(_T(""), WS_CHILD | WS_TABSTOP | WS_VISIBLE, rClient, this, 1);
	}
	else if (m_edit.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_READONLY | ES_MULTILINE | WS_VSCROLL | ES_NOHIDESEL, rClient, this, 1))
	{
		m_edit.SetFont(CFont::FromHandle(CDialogHelper::GetFont(this)));
		m_edit.SetMargins(5, 5);
		
		return TRUE;
	}

	// else
	return FALSE;
}

void CBrowserDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (m_browser.GetSafeHwnd())
		m_browser.MoveWindow(0, 0, cx, cy);

	else if (m_edit.GetSafeHwnd())
		m_edit.MoveWindow(0, 0, cx, cy);
}

void CBrowserDlg::OnEditSetFocus()
{
	if (!m_bBrowser)
		m_edit.SetSel(-1, -1);
}

void CBrowserDlg::OnClose()
{
	if (m_bHideOnClose)
	{
		ShowWindow(SW_HIDE);
		return;
	}

	// else
	CRuntimeDlg::OnClose();
}

void CBrowserDlg::OnOK()
{
	if (m_bHideOnClose)
	{
		ShowWindow(SW_HIDE);
		return;
	}

	// else
	CRuntimeDlg::OnOK();
}

void CBrowserDlg::OnCancel()
{
	if (m_bHideOnClose)
	{
		ShowWindow(SW_HIDE);
		return;
	}

	// else
	CRuntimeDlg::OnCancel();
}

// passworddialog.cpp : implementation file
//

#include "stdafx.h"
#include "passworddialog.h"
#include "stringres.h"
#include "enstring.h"
#include "misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasswordDialog dialog

CMap<UINT, UINT, CString, CString&> CPasswordDialog::s_mapText;

#define WM_POSTINITDIALOG (WM_APP+1)

CPasswordDialog::CPasswordDialog(BOOL bConfirm, LPCTSTR szExplanation, CWnd* /*pParent*/)
	: CRuntimeDlg(), m_bConfirm(bConfirm)
{
	//{{AFX_DATA_INIT(CPasswordDialog)
	m_sPassword = _T("");
	m_sConfirmation = _T("");
	//}}AFX_DATA_INIT

	// figure out how long the prompts might be
	CString sPassword = GetItemText(IDC_PD_PASSWORDLABEL, _T("&Enter Password"));
	CString sConfirm = GetItemText(IDC_PD_CONFIRMLABEL, _T("&Confirm Password"));

	const int PROMPTLEN = max(m_bConfirm ? sConfirm.GetLength() : 0, sPassword.GetLength()) * 4;
	const int EDITSTART = PROMPTLEN + 8;
	const int EDITEND = EDITSTART + 110;

	int nLines = CalcLinesRequired(szExplanation, EDITEND - 7);
	const int YOFFSET = m_bConfirm ? 19 : 0;
	const int YEXPOFFSET = (NULL == szExplanation) ? 0 : (nLines * 8 + 8);

	if (szExplanation)
	    AddRCControl(_T("LTEXT"), _T(""), szExplanation, 0, 0,7,7, EDITEND - 7, (nLines * 8), (UINT)IDC_STATIC);

    AddRCControl(_T("LTEXT"), _T(""), sPassword, 0, 0,7,9 + YEXPOFFSET,PROMPTLEN,8, IDC_PD_PASSWORDLABEL);
    AddRCControl(_T("EDITTEXT"), _T(""), _T(""), WS_TABSTOP | ES_PASSWORD | ES_AUTOHSCROLL, 0, EDITSTART,7 + YEXPOFFSET,110,14, IDC_PD_PASSWORD);

	if (m_bConfirm)
	{
		AddRCControl(_T("LTEXT"), _T(""), sConfirm, 0, 0,7, 28 + YEXPOFFSET,PROMPTLEN,8, IDC_PD_CONFIRMLABEL);
		AddRCControl(_T("EDITTEXT"), _T(""), _T(""), WS_TABSTOP | ES_PASSWORD | ES_AUTOHSCROLL, 0, EDITSTART,25 + YEXPOFFSET,110,14, IDC_PD_CONFIRM);
	}

    AddRCControl(_T("CONTROL"), _T("Static"), _T(""), SS_ETCHEDHORZ, 0, 7,30 + YOFFSET + YEXPOFFSET, EDITEND - 7,1, (UINT)IDC_STATIC);
    AddRCControl(_T("DEFPUSHBUTTON"), _T(""), _T("OK"), WS_DISABLED | WS_TABSTOP, 0, EDITEND - 50 - 4 - 50,40 + YOFFSET + YEXPOFFSET,50,14,IDOK);
    AddRCControl(_T("PUSHBUTTON"), _T(""), _T("Cancel"), WS_TABSTOP, 0, EDITEND - 50,40 + YOFFSET + YEXPOFFSET,50,14,IDCANCEL);
}


void CPasswordDialog::DoDataExchange(CDataExchange* pDX)
{
	CRuntimeDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswordDialog)
	DDX_Text(pDX, IDC_PD_PASSWORD, m_sPassword);

	if (m_bConfirm)
		DDX_Text(pDX, IDC_PD_CONFIRM, m_sConfirmation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasswordDialog, CRuntimeDlg)
	//{{AFX_MSG_MAP(CPasswordDialog)
	ON_EN_CHANGE(IDC_PD_PASSWORD, OnChangePassword)
	ON_EN_CHANGE(IDC_PD_CONFIRM, OnChangeConfirmation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswordDialog message handlers

int CPasswordDialog::DoModal() 
{	
	CString sText = GetItemText(PD_TITLE, _T("Password Required"));
	CString sTitle;

	sTitle.Format(_T("%s - %s"), sText, AfxGetAppName());

	return CRuntimeDlg::DoModal(sTitle, RTD_DEFSTYLE, RTD_DEFEXSTYLE | WS_EX_TOPMOST); 
}

BOOL CPasswordDialog::OnInitDialog()
{
	CRuntimeDlg::OnInitDialog();

	GetDlgItem(IDC_PD_PASSWORD)->SetFocus();

	return FALSE;
}

// static method
BOOL CPasswordDialog::RetrievePassword(BOOL bConfirm, CString& sPassword, LPCTSTR szExplanation)
{
	BOOL bContinue = TRUE;

	while (bContinue)
	{
		CPasswordDialog dialog(bConfirm, szExplanation);

		if (IDOK == dialog.DoModal())
			bContinue = dialog.GetPassword().IsEmpty();
		else
			return FALSE;

		// check confirmation too
		if (!bContinue)
		{
			if (bConfirm)
			{
				if (dialog.GetConfirmation() != dialog.GetPassword())
				{
					szExplanation = GetItemText(DLG_PD_CONFIRMFAILED, _T("Passwords don't match"));
					bContinue = TRUE;
				}
				else
					sPassword = dialog.GetPassword();
			}
			else
				sPassword = dialog.GetPassword();
		}
	}

	return TRUE;
}

// static method
BOOL CPasswordDialog::VerifyPassword(LPCTSTR szPassword, LPCTSTR szExplanation, LPCTSTR szErrorMsg)
{
	while (TRUE)
	{
		CPasswordDialog dialog(FALSE, szExplanation);

		if (IDOK == dialog.DoModal())
		{
			if (dialog.GetPassword() == szPassword)
				return TRUE;

			// else
			if (!Misc::IsEmpty(szErrorMsg))
				szExplanation = szErrorMsg;
		}
		else
			break; // user cancelled
	}

	return FALSE;
}

void CPasswordDialog::OnChangePassword() 
{
	UpdateData();

	GetDlgItem(IDOK)->EnableWindow(!m_sPassword.IsEmpty() && (!m_bConfirm || !m_sConfirmation.IsEmpty()));
}

void CPasswordDialog::OnChangeConfirmation() 
{
	UpdateData();

	if (m_bConfirm)
		GetDlgItem(IDOK)->EnableWindow(!m_sPassword.IsEmpty() && (!m_bConfirm || !m_sConfirmation.IsEmpty()));
}

CString CPasswordDialog::GetItemText(UINT nIDItem, LPCTSTR szDefault)
{
	CString sText;
	s_mapText.Lookup(nIDItem, sText);
	
	if (sText.IsEmpty() && szDefault)
		return szDefault;

	return sText;
}

void CPasswordDialog::SetItemText(UINT nIDItem, UINT nIDText)
{
	s_mapText[nIDItem] = CEnString(nIDText);
}


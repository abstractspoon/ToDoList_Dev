// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AboutDlg.h"
#include "stringres.h"
#include "enstring.h"
#include "misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef LVS_EX_LABELTIP
#define LVS_EX_LABELTIP     0x00004000
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

enum // ctrl IDs
{
	IDC_APPICON = 100,
	IDC_APPNAME, // 101
	IDC_DIVIDER, // 102
	IDC_APPDESCRIPTION, // 103
	IDC_COPYRIGHT, // 104
	IDC_LICENSE // 105
};

const int DLUPERLINE = 9;

CAboutDlg::CAboutDlg(UINT nAppIconID, AB_STYLE nStyle, 
					 LPCTSTR szAppName, LPCTSTR szAppDescription, LPCTSTR szCopyright, LPCTSTR szLicense, 
					 int nAppLines, int nDescLines, int nCopyrightLines, int nLicenseLines,
					 int nWidth) 
: CRuntimeDlg(), 
	m_sAppName(szAppName), 
	m_sAppDescription(szAppDescription), 
	m_sCopyright(szCopyright),
	m_sLicense(szLicense),
	m_nStyle(nStyle)
{
	SetBordersDLU(7);

    // icon
	AddRCControl(_T("ICON"), _T(""), _T(""), SS_ICON, 0, 5,5,20,20, IDC_APPICON);

    // variable height items
    const UINT ITEMIDS[] = { IDC_APPNAME, IDC_APPDESCRIPTION, IDC_COPYRIGHT, IDC_LICENSE };
    const UINT NUMLINES[] =  { max(nAppLines, 0), max(nDescLines, 0), max(nCopyrightLines, 0), max(nLicenseLines, 0) };
    const UINT NUMITEMS = sizeof(ITEMIDS) / sizeof(UINT);

    int nTop = 7;

	if (nWidth == -1)
		nWidth = 180;

    for (int nItem = 0; nItem < NUMITEMS; nItem++)
    {
        int nHeight = (NUMLINES[nItem] * DLUPERLINE);

		// special case: copyright
		if (ITEMIDS[nItem] == IDC_COPYRIGHT)
		{
			// add a 'contributers' label
			nTop -= 3;
			AddRCControl(_T("LTEXT"), _T(""), _T("&Contributors:"), 0, 0, 36, nTop, nWidth, DLUPERLINE, (UINT)IDC_STATIC);
			nTop += DLUPERLINE;

			switch (m_nStyle)
			{
			case ABS_EDITCOPYRIGHT:
				AddRCControl(_T("CONTROL"), _T("RICHEDIT"), _T(""), ES_READONLY | ES_MULTILINE | WS_VSCROLL, 0, 36, nTop, nWidth, nHeight, ITEMIDS[nItem]);
				break;

			case ABS_HTMLCOPYRIGHT:
				AddRCControl(_T("LTEXT"), _T(""), _T(""), SS_NOPREFIX, 0, 36, nTop, nWidth, nHeight, ITEMIDS[nItem]);
				break;

			case ABS_LISTCOPYRIGHT:
				AddRCControl(_T("CONTROL"), WC_LISTVIEW, _T(""), LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_SORTASCENDING, 0, 36, nTop, nWidth, nHeight, ITEMIDS[nItem]);
				break;
			}
		}
		else
			AddRCControl(_T("LTEXT"), _T(""), _T(""), SS_NOPREFIX, 0, 36, nTop, nWidth, nHeight, ITEMIDS[nItem]);
		
        nTop += nHeight;

        if (nHeight && nItem < (NUMITEMS - 1))
            nTop += 6;
    }

    // divider and ok button
    AddRCControl(_T("CONTROL"), _T("static"), _T(""), SS_ETCHEDHORZ, 0, 7, nTop + 4, nWidth + 30, 1, IDC_DIVIDER);
	AddRCControl(_T("DEFPUSHBUTTON"), _T(""), BTN_OK, WS_GROUP | BS_DEFPUSHBUTTON, 0, (nWidth + 36 - 50) / 2, nTop + 10, 50, 14, IDOK);

	m_hIcon = AfxGetApp()->LoadIcon(nAppIconID);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CRuntimeDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_APPICON, m_stIcon);
	DDX_Text(pDX, IDC_APPNAME, m_sAppName);
	DDX_Text(pDX, IDC_APPDESCRIPTION, m_sAppDescription);
	DDX_Text(pDX, IDC_COPYRIGHT, m_sCopyright);
	DDX_Text(pDX, IDC_LICENSE, m_sLicense);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_APPNAME, m_stAppName);
	DDX_Control(pDX, IDC_APPDESCRIPTION, m_stAppDescription);
	DDX_Control(pDX, IDC_LICENSE, m_stLicense);

	switch (m_nStyle)
	{
	case ABS_EDITCOPYRIGHT:
		DDX_Control(pDX, IDC_COPYRIGHT, m_reCopyright);
		break;
		
	case ABS_HTMLCOPYRIGHT:
		DDX_Control(pDX, IDC_COPYRIGHT, m_stCopyright);
		break;
		
	case ABS_LISTCOPYRIGHT:
		DDX_Control(pDX, IDC_COPYRIGHT, m_lcCopyright);
		break;
	}
}

BEGIN_MESSAGE_MAP(CAboutDlg, CRuntimeDlg)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

int CAboutDlg::DoModal()
{
	return CRuntimeDlg::DoModal(ABOUT_TITLE, WS_VISIBLE | WS_POPUP | WS_BORDER | WS_CAPTION | DS_CENTER);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CRuntimeDlg::OnInitDialog();
	
	if (m_hIcon)
		((CStatic*)GetDlgItem(IDC_APPICON))->SetIcon(m_hIcon);

	if (m_reCopyright.GetSafeHwnd())
	{
		m_reBorder.Initialize(m_reCopyright);
		m_reCopyright.SetMargins(5);
	}
	else if (m_lcCopyright.GetSafeHwnd())
	{
		CRect rList;
		m_lcCopyright.GetClientRect(rList);
		rList.right -= GetSystemMetrics(SM_CXVSCROLL);

		m_lcCopyright.InsertColumn(0, _T("Contributor"), LVCFMT_LEFT, (rList.Width() * 2) / 5);
		m_lcCopyright.InsertColumn(1, _T("Contribution"), LVCFMT_LEFT, (rList.Width() * 3) / 5);

		CStringArray aRows;
		int nRows = Misc::Split(m_sCopyright, aRows, '\n', TRUE);

		for (int nRow = 0; nRow < nRows; nRow++)
		{
			CStringArray aCols;
			int nCols = Misc::Split(aRows[nRow], aCols, '\t', TRUE);
			
			if (!nCols)
				continue;

			int nIndex = m_lcCopyright.InsertItem(nRow, aCols[0]);

			if (nCols >= 2)
				m_lcCopyright.SetItemText(nIndex, 1, aCols[1]);
		}

		// dummy imagelist to increase row height
		if (m_il.Create(1, 16, ILC_COLOR, 1, 1))
			m_lcCopyright.SetImageList(&m_il, LVSIL_SMALL);

	//	ListView_SetExtendedListViewStyleEx(m_lcOptions, LVS_EX_GRIDLINES, LVS_EX_GRIDLINES);
		ListView_SetExtendedListViewStyleEx(m_lcCopyright, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
		ListView_SetExtendedListViewStyleEx(m_lcCopyright, LVS_EX_LABELTIP, LVS_EX_LABELTIP);
	}

	m_stAppName.SetBkColor(GetSysColor(COLOR_3DFACE));
	m_stAppDescription.SetBkColor(GetSysColor(COLOR_3DFACE));
	m_stCopyright.SetBkColor(GetSysColor(COLOR_3DFACE));
	m_stLicense.SetBkColor(GetSysColor(COLOR_3DFACE));

	GetDlgItem(IDOK)->SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

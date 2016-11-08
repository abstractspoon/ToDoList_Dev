// TransTextEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TransTextEditor.h"
#include "TransTextEditorDlg.h"

#include "..\shared\enfiledialog.h"
#include "..\shared\holdredraw.h"
#include "..\shared\filemisc.h"
#include "..\shared\dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransTextEditorDlg dialog

CTransTextEditorDlg::CTransTextEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransTextEditorDlg::IDD, pParent), m_bEdited(FALSE)
{
	//{{AFX_DATA_INIT(CTransTextEditorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTransTextEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransTextEditorDlg)
	DDX_Control(pDX, IDC_DICTIONARY, m_lcDictItems);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTransTextEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CTransTextEditorDlg)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_FILE_LOADDICTIONARY, OnFileLoadDictionary)
	ON_COMMAND(ID_FILE_SAVEDICTIONARY, OnFileSaveDictionary)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_DICTIONARY, OnEndlabeleditDictionary)
	ON_COMMAND(ID_FILE_CLOSE, OnFileExit)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransTextEditorDlg message handlers

BOOL CTransTextEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetWindowText(m_sBaseTitle);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	CMenu menu;

	if (menu.LoadMenu(IDR_MAINFRAME))
		SetMenu(&menu);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_lcDictItems.AddCol(_T("English Text"), 300);
	m_lcDictItems.AddCol(_T("Translated Text"), 300);
	m_lcDictItems.AddCol(_T("UI Element"), 100);

	m_lcDictItems.ShowGrid(TRUE, TRUE);
	m_lcDictItems.SetMinItemHeight(18);
	m_lcDictItems.DisableColumnEditing(0, TRUE);
	m_lcDictItems.DisableColumnEditing(2, TRUE);
	m_lcDictItems.SetColumnTextColor(0, RGB(100, 100, 100));
	m_lcDictItems.SetColumnTextColor(2, RGB(100, 100, 100));

	m_lcDictItems.EnableSorting(TRUE);
	m_lcDictItems.SetSortColumn(0);
	m_lcDictItems.SetSortAscending(TRUE);

	DICTITEM::SetTranslationOption(ITTTO_ADD2DICTIONARY);
	
	ResizeList();
	LoadState();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTransTextEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CTransTextEditorDlg::OnFileLoadDictionary() 
{
	CFileOpenDialog dialog(_T("Select Dictionary"), _T(".csv"), NULL, EOFN_DEFAULTOPEN, _T("Dictionaries (*.csv)|*.csv||"));

	if (dialog.DoModal() == IDOK)
		LoadDictionary(dialog.GetPathName());
}

BOOL CTransTextEditorDlg::LoadDictionary(LPCTSTR szDictPath)
{
	if (PromptAndSave())
	{
		CWaitCursor cursor;
		CHoldRedraw hr(m_lcDictItems);
		
		m_lcDictItems.DeleteAllItems();
		
		if (m_dictionary.LoadDictionary(szDictPath))
		{
			const CDictionaryItems& items = m_dictionary.GetItems();
			POSITION pos = items.GetStartPosition();
			
			while (pos)
			{
				CString sEnglish;
				DICTITEM* pDI = NULL;
				
				items.GetNextAssoc(pos, sEnglish, pDI);
				ASSERT(!sEnglish.IsEmpty() && pDI);
				
				if (!sEnglish.IsEmpty() && pDI)
				{
					int nItem = m_lcDictItems.InsertItem(m_lcDictItems.GetItemCount(), sEnglish);
					ASSERT(nItem != -1);
					
					m_lcDictItems.SetItemText(nItem, 1, pDI->GetTextOut());
					m_lcDictItems.SetItemText(nItem, 2, pDI->GetClassID());
					m_lcDictItems.SetItemData(nItem, nItem);
					
					// Alternatives
					const CMapStringToString& alts = pDI->GetAlternatives();
					POSITION posAlt = alts.GetStartPosition();
					
					while (posAlt)
					{
						CString sTranslation, sClassID;
						alts.GetNextAssoc(posAlt, sClassID, sTranslation);
						
						int nItem = m_lcDictItems.InsertItem(m_lcDictItems.GetItemCount(), sEnglish);
						ASSERT(nItem != -1);
						
						m_lcDictItems.SetItemText(nItem, 1, sTranslation);
						m_lcDictItems.SetItemText(nItem, 2, sClassID);
						m_lcDictItems.SetItemData(nItem, nItem);
					}
				}
			}
			
			m_lcDictItems.Sort();
			
			m_bEdited = FALSE;
			UpdateCaption();

			return TRUE;
		}
	}

	// else
	return FALSE;
}

void CTransTextEditorDlg::OnFileSaveDictionary() 
{
	m_dictionary.SaveDictionary();

	m_bEdited = FALSE;
	UpdateCaption();
}

void CTransTextEditorDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	ResizeList(cx, cy);
}

void CTransTextEditorDlg::ResizeList(int cx, int cy)
{
	if (m_lcDictItems.GetSafeHwnd())
	{
		if ((cx == 0) && (cy == 0))
		{
			CRect rClient;
			GetClientRect(rClient);

			cx = rClient.Width();
			cy = rClient.Height();
		}

		m_lcDictItems.MoveWindow(0, 0, cx, cy);

		// Resize columns 0 and 1
		int nColWidth = ((cx - GetSystemMetrics(SM_CXVSCROLL) - 100 - 6) / 2);

		m_lcDictItems.SetColumnWidth(0, nColWidth);
		m_lcDictItems.SetColumnWidth(1, nColWidth);
	}
}

void CTransTextEditorDlg::OnEndlabeleditDictionary(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	ASSERT(pDispInfo->item.iSubItem == 1);

	// Modify the dictionary item that this refers to
	CString sEnglish = m_lcDictItems.GetItemText(pDispInfo->item.iItem, 0);
	CString sClassID = m_lcDictItems.GetItemText(pDispInfo->item.iItem, 2);

	VERIFY(m_dictionary.ModifyItem(sEnglish, sClassID, pDispInfo->item.pszText));

	m_bEdited = TRUE;
	UpdateCaption();

	*pResult = 0;
}

void CTransTextEditorDlg::UpdateCaption()
{
	CString sCaption(m_sBaseTitle);
	CString sDictPath(m_dictionary.GetDictionaryPath());

	if (!sDictPath.IsEmpty())
	{
		sCaption += _T(" - ");
		sCaption += FileMisc::GetFileNameFromPath(sDictPath);

		if (m_bEdited)
			sCaption += '*';
	}

	SetWindowText(sCaption);
}

void CTransTextEditorDlg::OnOK()
{
	// do nothing
}

void CTransTextEditorDlg::OnCancel()
{
	// do nothing
}

BOOL CTransTextEditorDlg::PromptAndSave()
{
	if (m_bEdited)
	{
		CEnString sText(IDS_SAVECHANGES, FileMisc::GetFileNameFromPath(m_dictionary.GetDictionaryPath()));

		int nRet = CDialogHelper::ShowMessageBox(*this, m_sBaseTitle, _T("Save Changes?"), sText, MB_YESNOCANCEL);

		switch (nRet)
		{
		case IDYES:
			m_dictionary.SaveDictionary();
			return TRUE;

		case IDNO:
			return TRUE;
		}
		
		// all else cancel
		return FALSE;
	}

	return TRUE;
}

void CTransTextEditorDlg::OnFileExit() 
{
	EndDialog(IDCANCEL);
}

void CTransTextEditorDlg::OnClose() 
{
	if (PromptAndSave())
	{
		SaveState();
		EndDialog(IDCANCEL);
	}
}

void CTransTextEditorDlg::LoadState()
{
	CString sDictPath = AfxGetApp()->GetProfileString(_T("State"), _T("Dictionary"));
	
	if (!sDictPath.IsEmpty())
		LoadDictionary(sDictPath);
}

void CTransTextEditorDlg::SaveState()
{
	AfxGetApp()->WriteProfileString(_T("State"), _T("Dictionary"), m_dictionary.GetDictionaryPath());
}

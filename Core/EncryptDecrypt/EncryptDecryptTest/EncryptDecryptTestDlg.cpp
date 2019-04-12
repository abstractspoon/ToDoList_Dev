// EncryptDecryptTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EncryptDecryptTest.h"
#include "EncryptDecryptTestDlg.h"

#include "..\..\shared\iencryption.h"
#include "..\..\shared\passworddialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEncryptDecryptTestDlg dialog

CEncryptDecryptTestDlg::CEncryptDecryptTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEncryptDecryptTestDlg::IDD, pParent), m_pEncryptor(NULL)
{
	//{{AFX_DATA_INIT(CEncryptDecryptTestDlg)
	m_sFilePath = _T("");
	m_sEncryptedPath = _T("");
	m_sDecryptedPath = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pEncryptor = CreateEncryptionInterface("encryptdecrypt.dll");
}

CEncryptDecryptTestDlg::~CEncryptDecryptTestDlg()
{
	ReleaseEncryptionInterface(m_pEncryptor);
}

void CEncryptDecryptTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEncryptDecryptTestDlg)
	DDX_Text(pDX, IDC_FILEPATH, m_sFilePath);
	DDX_Text(pDX, IDC_ENCRYPTEDPATH, m_sEncryptedPath);
	DDX_Text(pDX, IDC_DECRYPTEDPATH, m_sDecryptedPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEncryptDecryptTestDlg, CDialog)
	//{{AFX_MSG_MAP(CEncryptDecryptTestDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_ENCRYPT, OnEncrypt)
	ON_BN_CLICKED(IDC_DECRYPT, OnDecrypt)
	ON_BN_CLICKED(IDC_OPENDECRYPTED, OnOpendecrypted)
	ON_BN_CLICKED(IDC_OPENENCRYPTED, OnOpenencrypted)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEncryptDecryptTestDlg message handlers

BOOL CEncryptDecryptTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEncryptDecryptTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEncryptDecryptTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CEncryptDecryptTestDlg::OnBrowse() 
{
	CFileDialog dialog(TRUE, NULL, m_sFilePath, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "All Files (*.*)|*.*||");
	dialog.m_ofn.lpstrTitle = "Select File to Encrypt";

	if (dialog.DoModal() == IDOK)
	{
		m_sFilePath = dialog.GetPathName();
		m_sEncryptedPath.Empty();
		m_sDecryptedPath.Empty();

		UpdateData(FALSE);
	}
}

void CEncryptDecryptTestDlg::OnEncrypt() 
{
	if (!m_pEncryptor)
	{
		AfxMessageBox("Could not initialize the encryption engine");
		return;
	}

	if (m_sFilePath.IsEmpty() || GetFileAttributes(m_sFilePath) == 0xffffffff)
	{
		AfxMessageBox("Please select a valid file first");
		return;
	}

	CString sPassword;

	if (CPasswordDialog::RetrievePassword(FALSE, sPassword))
	{
		unsigned char* pInput = NULL;
		int nLenInput = 0;
		unsigned char* pOutput = NULL;
		int nLenOutput = 0;

		if (ReadFile(m_sFilePath, pInput, nLenInput))
		{
			if (m_pEncryptor->Encrypt(pInput, nLenInput, sPassword, pOutput, nLenOutput))
			{
				// build the output file name based on m_sFilePath
				char szExt[_MAX_EXT];
				_splitpath(m_sFilePath, NULL, NULL, NULL, szExt);

				m_sEncryptedPath = m_sFilePath;
				m_sEncryptedPath.Replace(szExt, ".enc");
				m_sEncryptedPath += szExt;

				if (!WriteFile(m_sEncryptedPath, pOutput, nLenOutput))
					m_sEncryptedPath.Empty();

				UpdateData(FALSE);
			}
		}

		// cleanup
		delete [] pInput;
		m_pEncryptor->FreeBuffer(pOutput);
	}
}

void CEncryptDecryptTestDlg::OnDecrypt() 
{
	if (!m_pEncryptor)
	{
		AfxMessageBox("Could not initialize the encryption engine");
		return;
	}

	if (m_sEncryptedPath.IsEmpty() || GetFileAttributes(m_sEncryptedPath) == 0xffffffff)
	{
		AfxMessageBox("Please encrypt the file first");
		return;
	}
	
	CString sPassword;

	if (CPasswordDialog::RetrievePassword(FALSE, sPassword))
	{
		unsigned char* pInput = NULL;
		int nLenInput = 0;
		unsigned char* pOutput = NULL;
		int nLenOutput = 0;

		if (ReadFile(m_sEncryptedPath, pInput, nLenInput))
		{
			if (m_pEncryptor->Decrypt(pInput, nLenInput, sPassword, pOutput, nLenOutput))
			{
				// build the output file name based on m_sEncryptedPath
				m_sDecryptedPath = m_sEncryptedPath;
				m_sDecryptedPath.Replace(".enc", ".dec");

				if (!WriteFile(m_sDecryptedPath, pOutput, nLenOutput))
					m_sEncryptedPath.Empty();

				UpdateData(FALSE);
			}
		}

		// cleanup
		delete [] pInput;
		m_pEncryptor->FreeBuffer(pOutput);
	}
}

void CEncryptDecryptTestDlg::OnOpendecrypted() 
{
	if (m_sDecryptedPath.IsEmpty() || GetFileAttributes(m_sDecryptedPath) == 0xffffffff)
	{
		AfxMessageBox("Please decrypt the file first");
		return;
	}

	ShellExecute(*this, NULL, m_sDecryptedPath, NULL, NULL, SW_SHOWNORMAL);
}

void CEncryptDecryptTestDlg::OnOpenencrypted() 
{
	if (m_sEncryptedPath.IsEmpty() || GetFileAttributes(m_sEncryptedPath) == 0xffffffff)
	{
		AfxMessageBox("Please encrypt the file first");
		return;
	}

	ShellExecute(*this, NULL, m_sEncryptedPath, NULL, NULL, SW_SHOWNORMAL);
}

BOOL CEncryptDecryptTestDlg::ReadFile(LPCTSTR szFilePath, unsigned char*& pBuffer, int& nLenBuffer)
{
	CFile file;

	if (!file.Open(szFilePath, CFile::modeRead))
		return FALSE;

	nLenBuffer = file.GetLength();

	if (nLenBuffer == 0)
		return FALSE;

	pBuffer = new unsigned char[nLenBuffer];

	if (file.Read(pBuffer, nLenBuffer) != (UINT)nLenBuffer)
	{
		delete [] pBuffer;
		nLenBuffer = 0;
		return FALSE;
	}

	// else
	return TRUE;
}

BOOL CEncryptDecryptTestDlg::WriteFile(LPCTSTR szFilePath, unsigned char* pBuffer, int nLenBuffer)
{
	CFile file;

	if (!file.Open(szFilePath, CFile::modeCreate | CFile::modeWrite))
		return FALSE;

	file.Write(pBuffer, nLenBuffer);
	return TRUE;
}

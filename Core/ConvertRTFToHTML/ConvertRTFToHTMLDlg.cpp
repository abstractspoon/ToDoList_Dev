
// ConvertRTFToHTMLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ConvertRTFToHTML.h"
#include "ConvertRTFToHTMLDlg.h"
#include "afxdialogex.h"

#include "..\ToDoList\TaskFile.h"
#include "..\ToDoList\ToDoCtrlData.h"

#include "..\Shared\FileMisc.h"
#include "..\shared\Rtf2HtmlConverter.h"
#include "..\Shared\WebMisc.h"
#include "..\Shared\XmlCharMap.h"

#include "..\3rdparty\compression.h"

#pragma comment(lib, "..\\3rdparty\\zlib\\zlibstat.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CString FILTER_TASKLISTS(_T("Tasklists (*.tdl)|*.tdl||"));

static const LPCWSTR HTML_GUID = L"FE0B6B6E-2B61-4AEB-AA0D-98DBE5942F02";
static const LPCTSTR RTF_TYPEID = L"849CF988-79FE-418A-A40D-01FE3AFCAB2C";

// CConvertRTFToHTMLDlg dialog



CConvertRTFToHTMLDlg::CConvertRTFToHTMLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONVERTRTFTOHTML_DIALOG, pParent)
	, m_eInputTasklist(FES_COMBOSTYLEBTN, FILTER_TASKLISTS)
	, m_eOutputTasklist(FES_COMBOSTYLEBTN | FES_SAVEAS, FILTER_TASKLISTS)
	, m_sCurrentTask(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_sInputTasklist = AfxGetApp()->GetProfileString(L"Preferences", L"InputTasklist");
	m_sOutputTasklist = AfxGetApp()->GetProfileString(L"Preferences", L"OutputTasklist");
}

void CConvertRTFToHTMLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUTTASKLIST, m_eInputTasklist);
	DDX_Control(pDX, IDC_OUTPUTTASKLIST, m_eOutputTasklist);
	DDX_Text(pDX, IDC_INPUTTASKLIST, m_sInputTasklist);
	DDX_Text(pDX, IDC_OUTPUTTASKLIST, m_sOutputTasklist);
	DDX_Text(pDX, IDC_CURRENTTASK, m_sCurrentTask);
}

BEGIN_MESSAGE_MAP(CConvertRTFToHTMLDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CConvertRTFToHTMLDlg message handlers

BOOL CConvertRTFToHTMLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CConvertRTFToHTMLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CConvertRTFToHTMLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CConvertRTFToHTMLDlg::OnOK()
{
	UpdateData();

	AfxGetApp()->WriteProfileString(L"Preferences", L"InputTasklist", m_sInputTasklist);
	AfxGetApp()->WriteProfileString(L"Preferences", L"OutputTasklist", m_sOutputTasklist);

	if (FileMisc::IsSamePath(m_sInputTasklist, m_sOutputTasklist))
	{
		AfxMessageBox(L"Output file path cannot be same as input");
		return;
	}

	// Do the conversion
	CTaskFile tasks;

	if (!tasks.Load(m_sInputTasklist))
	{
		AfxMessageBox(L"Failed to open input tasklist");
		return;
	}

	CTDCStyleMap styles;
	CTDCCustomAttribDefinitionArray aCustomAttribDefs;
	tasks.GetCustomAttributeDefs(aCustomAttribDefs);

	CToDoCtrlData data(styles, aCustomAttribDefs);

	if (!data.BuildDataModel(tasks))
	{
		AfxMessageBox(L"Failed to load tasks from tasklist");
		return;
	}

	POSITION pos = data.GetFirstTaskPosition();
	CRtfHtmlConverter rtfHtml;
	CWaitCursor cursor;

	// Create an image folder using the output path
	CString sImageFolder(m_sOutputTasklist);
	FileMisc::RemoveExtension(sImageFolder);

	while (pos)
	{
		const TODOITEM* pTDI = NULL;
		DWORD dwTaskID = data.GetNextTask(pos, pTDI);

		if (pTDI->cfComments.CompareNoCase(RTF_TYPEID) == 0)
		{
			m_sCurrentTask.Format(L"\"%s\"", pTDI->sTitle);
			UpdateData(FALSE);

			const unsigned char* pContent = pTDI->customComments.Get();
			int nLength = pTDI->customComments.GetLength();

			// we may have to decompress it first
			unsigned char* pDecompressed = NULL;

			if (nLength && !rtfHtml.IsRTF((const char*)pTDI->customComments.Get()))
			{
				int nLenDecompressed = 0;

				if (Compression::Decompress(pContent, nLength, pDecompressed, nLenDecompressed))
				{
					pContent = pDecompressed;
					nLength = nLenDecompressed;

					// check again that it is RTF
					if (!rtfHtml.IsRTF((const char*)pContent))
					{
						nLength = 0;
					}
				}
				else
				{
					nLength = 0;
				}
			}

			if (nLength)
			{
				rtfHtml.SetAllowUseOfMSWord(TRUE);

				CString sHtml;

				if (rtfHtml.ConvertRtfToHtml((const char*)pContent, tasks.GetHtmlCharSet(), sHtml, sImageFolder))
				{
					HTASKITEM hTask = tasks.FindTask(dwTaskID);
					ASSERT(hTask);

					PostProcessHtml(sHtml);

					tasks.SetTaskCustomComments(hTask, CBinaryData(sHtml), HTML_GUID);
				}
			}

			// cleanup
			delete [] pDecompressed;
		}
	}

	if (tasks.Save(m_sOutputTasklist, SFEF_UTF16))
	{
		AfxMessageBox(L"Tasklist was successfully converted");
	}
	else
	{
		AfxMessageBox(L"Failed to save converted tasklist");
	}

#ifdef _DEBUG
	CString sTDLPath = FileMisc::GetModuleFolder();
	sTDLPath += L"\\..\\..\\ToDoList\\Unicode_Debug\\ToDoList.exe";

	ShellExecute(*this, NULL, sTDLPath, m_sOutputTasklist, NULL, SW_SHOWNORMAL);
#endif

	CDialogEx::OnOK();
}

BOOL CConvertRTFToHTMLDlg::PostProcessHtml(CString& sHtml) const
{
	BOOL bChanged = FALSE;

	// 1. Create real links for URIs
/*
	int nHtmlLen = sHtml.GetLength();
	int nStart = sHtml.Find('>');

	while (nStart != -1)
	{
		int nEnd = sHtml.Find('<', (nStart + 1));

		if (nEnd == -1)
			break;

		int nLen = (nEnd - nStart);

		if (nLen > 1)
		{
			CString sContent(sHtml.Mid(nStart + 1, (nLen - 1)));
			CXmlCharMap::ConvertFromRep(sContent);

			TRACE(L"CConvertRTFToHTMLDlg::PostProcessHtml(%d->%d, %s)\n", nStart, nEnd, sContent);

			CUrlArray aUrls;
			int nUrl = m_parser.ParseText(sContent, aUrls);

			// Work in reverse to avoid messing up url positions
			while (nUrl--)
			{
				const URLITEM& url = aUrls[nUrl];

				CString sLink;
				sLink.Format(L"<A HREF=%s>%s</A>", url.sUrl, url.sUrl);

				sContent = sContent.Left(url.cr.cpMin) + sLink + sContent.Mid(url.cr.cpMax);
			}

			if (aUrls.GetSize())
			{
				sHtml = sHtml.Left(nStart) + sContent + sHtml.Mid(nEnd);
				nEnd = nStart + sContent.GetLength();

				bChanged = TRUE;
			}
		}

		nStart = sHtml.Find('>', (nEnd + 1));
	}
*/
	
	return bChanged;
}
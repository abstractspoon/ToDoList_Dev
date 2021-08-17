
// ConvertRTFToHTMLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ConvertRTFToHTML.h"
#include "ConvertRTFToHTMLDlg.h"

#include "..\ToDoList\TaskFile.h"
#include "..\ToDoList\ToDoCtrlData.h"

#include "..\Shared\FileMisc.h"
#include "..\shared\Rtf2HtmlConverter.h"
#include "..\Shared\WebMisc.h"
#include "..\Shared\XmlCharMap.h"
#include "..\Shared\MSWordHelper.h"

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
	: CDialog(IDD_CONVERTRTFTOHTML_DIALOG, pParent)
	, m_eInputTasklist(FES_COMBOSTYLEBTN, FILTER_TASKLISTS)
	, m_eOutputTasklist(FES_COMBOSTYLEBTN | FES_SAVEAS, FILTER_TASKLISTS)
	, m_sCurrentTask(_T(""))
	, m_bUseMSWordForConversion(FALSE)
	, m_bOpenConvertedTasklist(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_sInputTasklist = AfxGetApp()->GetProfileString(L"Preferences", L"InputTasklist");
	m_sOutputTasklist = AfxGetApp()->GetProfileString(L"Preferences", L"OutputTasklist");

	if (CMSWordHelper::IsWordInstalled())
		m_bUseMSWordForConversion = AfxGetApp()->GetProfileInt(L"Preferences", L"UseMSWord", CMSWordHelper::IsWordInstalled());
	else
		m_bUseMSWordForConversion = FALSE;

	m_bOpenConvertedTasklist = AfxGetApp()->GetProfileInt(L"Preferences", L"OpenConverted", FALSE);
}

void CConvertRTFToHTMLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUTTASKLIST, m_eInputTasklist);
	DDX_Control(pDX, IDC_OUTPUTTASKLIST, m_eOutputTasklist);
	DDX_Text(pDX, IDC_INPUTTASKLIST, m_sInputTasklist);
	DDX_Text(pDX, IDC_OUTPUTTASKLIST, m_sOutputTasklist);
	DDX_Text(pDX, IDC_CURRENTTASK, m_sCurrentTask);
	DDX_Check(pDX, IDC_USEMSWORD, m_bUseMSWordForConversion);
	DDX_Check(pDX, IDC_OPENAFTER, m_bOpenConvertedTasklist);
}

BEGIN_MESSAGE_MAP(CConvertRTFToHTMLDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CConvertRTFToHTMLDlg message handlers

BOOL CConvertRTFToHTMLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_parser.AddProtocol(L"tdl://", FALSE);

	GetDlgItem(IDC_USEMSWORD)->EnableWindow(CMSWordHelper::IsWordInstalled());

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
		CDialog::OnPaint();
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
	AfxGetApp()->WriteProfileInt(L"Preferences", L"UseMSWord", m_bUseMSWordForConversion);
	AfxGetApp()->WriteProfileInt(L"Preferences", L"OpenConverted", m_bOpenConvertedTasklist);

	if (FileMisc::IsSamePath(m_sInputTasklist, m_sOutputTasklist))
	{
		AfxMessageBox(L"Output file path cannot be same as input");
		return;
	}

	// Do the conversion
	CTaskFile tasksIn;

	if (!tasksIn.Load(m_sInputTasklist))
	{
		AfxMessageBox(L"Failed to open input tasklist");
		return;
	}

	CTDCStyleMap styles;
	CTDCCustomAttribDefinitionArray aCustomAttribDefs;
	tasksIn.GetCustomAttributeDefs(aCustomAttribDefs);

	CToDoCtrlData data(styles, aCustomAttribDefs);

	if (!data.BuildDataModel(tasksIn))
	{
		AfxMessageBox(L"Failed to load tasks from tasklist");
		return;
	}

	POSITION pos = data.GetStart();
	CRtfHtmlConverter rtfHtml;
	CWaitCursor cursor;

	// Create an image folder using the output path
	CString sImageFolder(m_sOutputTasklist);
	FileMisc::RemoveExtension(sImageFolder);

	CTaskFile tasksOut(tasksIn);

	while (pos)
	{
		DWORD dwTaskID = data.GetNextTaskID(pos);
		const TODOITEM* pTDI = data.GetTask(dwTaskID);

		if (pTDI->IsReference())
			continue;

		m_sCurrentTask.Format(L"\"%s\"", pTDI->sTitle);
		UpdateData(FALSE);

		if (pTDI->cfComments.CompareNoCase(RTF_TYPEID) == 0)
		{
			HTASKITEM hTask = tasksOut.FindTask(dwTaskID);
			ASSERT(hTask);

			CBinaryData content(pTDI->customComments);
			const unsigned char* pContent = content.Get();
			int nLength = content.GetLength();

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

			CString sHtml;

			if (nLength)
			{
				rtfHtml.SetAllowUseOfMSWord(m_bUseMSWordForConversion);

				if (rtfHtml.ConvertRtfToHtml((const char*)pContent, tasksOut.GetHtmlCharSet(), sHtml, sImageFolder))
				{
					PostProcessHtml(pTDI->sComments, sHtml);
				}
				else
				{
					sHtml.Empty();
				}
			}

			tasksOut.SetTaskCustomComments(hTask, CBinaryData(sHtml), HTML_GUID);

			// cleanup
			delete [] pDecompressed;
		}
	}

	if (tasksOut.Save(m_sOutputTasklist, SFEF_UTF16))
	{
		AfxMessageBox(L"Tasklist was successfully converted");
	}
	else
	{
		AfxMessageBox(L"Failed to save converted tasklist");
	}

	if (m_bOpenConvertedTasklist)
	{
#ifdef _DEBUG
		CString sTDLPath = FileMisc::GetModuleFolder();
		sTDLPath += L"\\..\\..\\ToDoList\\Unicode_Debug\\ToDoList.exe";

		ShellExecute(*this, NULL, sTDLPath, m_sOutputTasklist, NULL, SW_SHOWNORMAL);
#else
		ShellExecute(*this, NULL, m_sOutputTasklist, NULL, NULL, SW_SHOWNORMAL);
#endif
	}

	CDialog::OnOK();
}

BOOL CConvertRTFToHTMLDlg::PostProcessHtml(const CString& sTextComments, CString& sHtml) const
{
	FileMisc::SaveFile(FileMisc::GetTempFilePath(L"before", L"html"), sHtml, SFEF_UTF16);

	BOOL bChanged = FALSE;

	bChanged |= FixupLinks(sTextComments, sHtml);
	bChanged |= FixupColors(sHtml);
	bChanged |= FixupLists(sHtml);

	FileMisc::SaveFile(FileMisc::GetTempFilePath(L"after", L"html"), sHtml, SFEF_UTF16);

	return bChanged;
}

BOOL CConvertRTFToHTMLDlg::FixupLinks(const CString& sTextComments, CString& sHtml) const
{
	BOOL bChanged = FALSE, bHtmlLinksFound = FALSE;

	if (m_bUseMSWordForConversion)
	{
		// 1. MSWord converts href links correctly but then wraps
		// the link text in a span which breaks the link in the UI
		// so we remove any span that encompasses a valid URI
		CString sLowerHtml = Misc::ToLower(sHtml);

		const CString SPANSTART = L"<span";
		const CString SPANEND = L"</span>";

		int nStartStart = sLowerHtml.Find(SPANSTART);

		while (nStartStart != -1)
		{
			int nEndStart = sLowerHtml.Find('>', nStartStart + SPANSTART.GetLength());

			if (nEndStart == -1)
				break;

			int nStartEnd = sLowerHtml.Find(SPANEND, nEndStart);

			if (nStartEnd == -1)
				break;

			int nEndEnd = (nStartEnd + SPANEND.GetLength());
			CString sContent = sHtml.Mid(nEndStart + 1, (nStartEnd - (nEndStart + 1)));

			if (m_parser.MatchesProtocol(sContent))
			{
				// Remove the span around the link text
				sHtml = sHtml.Left(nStartStart) + sContent + sHtml.Mid(nEndEnd);
				sLowerHtml = sLowerHtml.Left(nStartStart) + sContent + sLowerHtml.Mid(nEndEnd);

				// Adjust end to compensate
				nEndEnd = (nStartStart + sContent.GetLength());

				bChanged = TRUE;
			}

			nStartStart = sLowerHtml.Find(SPANSTART, (nEndStart + 1));
		}

		// 2. MSWord converts links without a protocol (eg. www.abstractspoon.com) 
		// into file-links pointing to the user's temp folder. However, we know
		// that links should be the same as the links text, so if the link text
		// has no protocol we give it one
		CStringArray aLinks, aLinksText;
		int nLink = WebMisc::ExtractHtmlLinks(sHtml, aLinks, aLinksText);

		if (nLink)
		{
			bHtmlLinksFound = TRUE;

			while (nLink--)
			{
				const CString& sLinkText = aLinksText[nLink];

				if (WebMisc::IsURL(sLinkText) && (sLinkText.Find(':') == -1))
				{
					const CString& sLink = aLinks[nLink];
					CString sFilePath;

					if (WebMisc::DecodeFileURI(sLink, sFilePath) && FileMisc::IsTempFilePath(sFilePath))
					{
						VERIFY(sHtml.Replace(sLink, (L"https://" + sLinkText)) == 1);

						bChanged = TRUE;
					}
				}
			}
		}
	}

	if (!bHtmlLinksFound)
	{
		CStringArray aLinks;
		int nLink = m_parser.ParseText(sTextComments, aLinks);

		if (nLink)
		{
			// remove duplicates because CString::Replace will
			// replace duplicates
			CStringSet mapUrls;
			mapUrls.CopyFrom(aLinks);

			POSITION pos = mapUrls.GetStartPosition();

			while (pos)
			{
				CString sHref, sUrl = mapUrls.GetNext(pos);
				sHref.Format(L"<A HREF='%s'>%s</A>", sUrl, sUrl);

				TXT2XML(sUrl); // to match Word

				VERIFY(sHtml.Replace(sUrl, sHref) > 0);

				bChanged = TRUE;
			}
		}
	}
	
	return bChanged;
}

BOOL CConvertRTFToHTMLDlg::FixupColors(CString& sHtml) const
{
	BOOL bChanged = FALSE;

	// TODO

	return bChanged;
}

BOOL CConvertRTFToHTMLDlg::FixupLists(CString& sHtml) const
{
	BOOL bChanged = FALSE;

	// TODO

	return bChanged;
}

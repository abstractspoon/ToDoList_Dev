// TDLCmdlineOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLCmdlineOptionsDlg.h"

#include "..\shared\Misc.h"
#include "..\shared\enstring.h"
#include "..\shared\clipboard.h"
#include "..\shared\themed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef LVS_EX_LABELTIP
#define LVS_EX_LABELTIP     0x00004000
#endif

/////////////////////////////////////////////////////////////////////////////

static const UINT CMDLINE_OPTIONS[] = 
{
	IDS_COMMANDLINEOPTIONS,
	IDS_CMDLINE_INIFILE,             
	IDS_CMDLINE_TASKFILE,               
	IDS_CMDLINE_FORCEVISIBLE,           
	IDS_CMDLINE_NOPWORDPROMPT,          
	IDS_CMDLINE_LOGGING,             
	IDS_CMDLINE_IMPORT,                 
	IDS_CMDLINE_COMMANDID,              
	IDS_CMDLINE_STARTEMPTY,             
	IDS_CMDLINE_TASKLINK,               
	IDS_CMDLINE_RESTART,             
	IDS_CMDLINE_SAVEINTERMEDIATE,       
	IDS_CMDLINE_RANDOMISE,              
	IDS_CMDLINE_SAVEUIVISINTASKLIST, 
	IDS_CMDLINE_ALLOWFORCEDCHECKOUT, 
	IDS_CMDLINE_NEWTASK,             
	IDS_CMDLINE_SELECTTASKID,           
	IDS_CMDLINE_PARENTID,               
	IDS_CMDLINE_SIBLINGID,              
	IDS_CMDLINE_TASKTITLE,           
	IDS_CMDLINE_TASKCOMMENTS,
	IDS_CMDLINE_TASKEXTID,              
	IDS_CMDLINE_TASKCATEGORY,           
	IDS_CMDLINE_TASKSTATUS,             
	IDS_CMDLINE_TASKPRIORITY,           
	IDS_CMDLINE_TASKRISK,               
	IDS_CMDLINE_TASKTAGS,               
	IDS_CMDLINE_TASKCOST,               
	IDS_CMDLINE_TASKDEPENDENCY,         
	IDS_CMDLINE_TASKTIMEEST,         
	IDS_CMDLINE_TASKTIMESPENT,          
	IDS_CMDLINE_TASKFILELINK,         
	IDS_CMDLINE_TASKALLOCBY,         
	IDS_CMDLINE_TASKALLOCTO,         
	IDS_CMDLINE_TASKSTARTDATE,          
	IDS_CMDLINE_TASKSTARTTIME,          
	IDS_CMDLINE_TASKDUEDATE,         
	IDS_CMDLINE_TASKDUETIME,         
	IDS_CMDLINE_TASKDONEDATE,           
	IDS_CMDLINE_TASKDONETIME,           
	IDS_CMDLINE_TASKCREATEDATE,         
	IDS_CMDLINE_TASKCREATETIME,         
	IDS_CMDLINE_TASKPERCENT,         
	IDS_CMDLINE_TASKVERSION,         
	IDS_CMDLINE_TASKCUSTOMATTRIB,       
	IDS_CMDLINE_TASKCOPYATTRIB,         
	IDS_CMDLINE_HELP,                   
};
const int NUM_OPTIONS = sizeof(CMDLINE_OPTIONS) / sizeof(UINT);

/////////////////////////////////////////////////////////////////////////////////////

// CTDLCmdlineOptionsDlg dialog

CTDLCmdlineOptionsDlg::CTDLCmdlineOptionsDlg(CWnd* pParent /*=NULL*/)
	: CTDLDialog(CTDLCmdlineOptionsDlg::IDD, _T("CmdlineOptions"), pParent)
{
	//{{AFX_DATA_INIT(CTDLCmdlineOptionsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTDLCmdlineOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLCmdlineOptionsDlg)
	DDX_Control(pDX, IDC_CMDLINE_OPTIONS, m_lcOptions);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLCmdlineOptionsDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLCmdlineOptionsDlg)
	ON_BN_CLICKED(IDC_COPYSHORTCUTS, OnCopyshortcuts)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLCmdlineOptionsDlg message handlers

BOOL CTDLCmdlineOptionsDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
	// columns
	m_lcOptions.InsertColumn(0, _T("Option"));
	m_lcOptions.InsertColumn(1, _T("Description"));

	// options
	CStringArray aOptions;
	int nNumOpt = GetOptions(aOptions);

	for (int nOpt = 0; nOpt < nNumOpt; nOpt++)
	{
		CString sOption(aOptions[nOpt]), sText;

		if (Misc::Split(sOption, sText, '\t', TRUE) && !sText.IsEmpty())
		{
			int nIndex = m_lcOptions.InsertItem(nOpt, sOption);
			m_lcOptions.SetItemText(nIndex, 1, sText);
		}
	}
	m_lcOptions.SetColumnWidth(0, LVSCW_AUTOSIZE);
	m_lcOptions.SetColumnWidth(1, LVSCW_AUTOSIZE);
	
	// dummy imagelist to increase row height
	if (m_il.Create(1, 16, ILC_COLOR, 1, 1))
		m_lcOptions.SetImageList(&m_il, LVSIL_SMALL);

	ListView_SetExtendedListViewStyleEx(m_lcOptions, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	ListView_SetExtendedListViewStyleEx(m_lcOptions, LVS_EX_LABELTIP, LVS_EX_LABELTIP);

	CThemed::SetWindowTheme(&m_lcOptions, _T("Explorer"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLCmdlineOptionsDlg::OnCopyshortcuts() 
{
	CStringArray aOptions;
	GetOptions(aOptions);

	Misc::SortArray(aOptions);

	CClipboard(*this).SetText(Misc::FormatArray(aOptions, '\n'));
}

int CTDLCmdlineOptionsDlg::GetOptions(CStringArray& aOptions)
{
	for (int nOpt = 0; nOpt < NUM_OPTIONS; nOpt++)
	{
		CEnString sOption(CMDLINE_OPTIONS[nOpt]);

		if (!sOption.IsEmpty())
			aOptions.Add(sOption);
	}

	return aOptions.GetSize();
}

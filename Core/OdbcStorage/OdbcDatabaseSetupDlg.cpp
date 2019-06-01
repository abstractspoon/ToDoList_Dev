// OdbcStorageSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "OdbcDatabaseSetupDlg.h"
#include "Odbchelper.h"

#include "..\Shared\databaseEx.h"
#include "..\Shared\dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COdbcDatabaseSetupDlg dialog


COdbcDatabaseSetupDlg::COdbcDatabaseSetupDlg(CDatabaseEx& db, const ODBCSETUP& setup, CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_DATABASESETUP_DIALOG, pParent), 
	m_db(db), 
	m_setup(setup)
{
	//{{AFX_DATA_INIT(COdbcDatabaseSetupDlg)
	//}}AFX_DATA_INIT
	m_sDlgTitle = CEnString(IDS_DBSETUP_DLGTITLE, m_db.FormatName());
//	m_sTasksTable = m_setup.tables.tableTasks.sTableName;
}


void COdbcDatabaseSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COdbcDatabaseSetupDlg)
	DDX_Control(pDX, IDC_TASKATTRIBUTESETUP, m_lcAttribSetup);
	DDX_Control(pDX, IDC_TASKS_TASKLISTKEYFIELD, m_cbTasksTasklistKeyField);
	DDX_Control(pDX, IDC_TASKS_PARENTKEYFIELD, m_cbTasksParentKeyField);
	DDX_Control(pDX, IDC_TASKS_TABLENAME, m_cbTasksTableName);
	DDX_Control(pDX, IDC_TASKS_KEYFIELD, m_cbTasksKeyField);
	DDX_Control(pDX, IDC_TASKLISTS_TABLENAME, m_cbTasklistsTableName);
	DDX_Control(pDX, IDC_TASKLISTS_NAMEFIELD, m_cbTasklistsNameField);
	DDX_Control(pDX, IDC_TASKLISTS_KEYFIELD, m_cbTasklistsKeyField);
	DDX_Control(pDX, IDC_ALLOCTO_TABLENAME, m_cbAllocToTableName);
	DDX_Control(pDX, IDC_ALLOCTO_NAMEFIELD, m_cbAllocToNameField);
	DDX_Control(pDX, IDC_ALLOCTO_KEYFIELD, m_cbAllocToKeyField);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_TASKS_PARENTKEYFIELD, m_setup.tables.tableTasks.sParentKeyField);
	DDX_Text(pDX, IDC_TASKS_TASKLISTKEYFIELD, m_setup.tables.tableTasks.sTasklistKeyField);
	DDX_Text(pDX, IDC_TASKS_TABLENAME, m_setup.tables.tableTasks.sTableName);
	DDX_Text(pDX, IDC_TASKS_KEYFIELD, m_setup.tables.tableTasks.sTaskKeyField);
	DDX_Text(pDX, IDC_TASKLISTS_TABLENAME, m_setup.tables.tableTasklists.sTableName);
	DDX_Text(pDX, IDC_TASKLISTS_NAMEFIELD, m_setup.tables.tableTasklists.sContentField);
	DDX_Text(pDX, IDC_TASKLISTS_KEYFIELD, m_setup.tables.tableTasklists.sKeyField);
	DDX_Text(pDX, IDC_ALLOCTO_TABLENAME, m_setup.tables.tableAllocTo.sTableName);
	DDX_Text(pDX, IDC_ALLOCTO_NAMEFIELD, m_setup.tables.tableAllocTo.sContentField);
	DDX_Text(pDX, IDC_ALLOCTO_KEYFIELD, m_setup.tables.tableAllocTo.sKeyField);
}


BEGIN_MESSAGE_MAP(COdbcDatabaseSetupDlg, CDialog)
	//{{AFX_MSG_MAP(COdbcDatabaseSetupDlg)
	ON_BN_CLICKED(IDC_VALIDATE, OnValidate)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_TASKATTRIBUTESETUP, OnEditTaskAttributeList)
	ON_CBN_SELCHANGE(IDC_ALLOCTO_TABLENAME, OnSelchangeAlloctoTablename)
	ON_CBN_SELCHANGE(IDC_TASKLISTS_TABLENAME, OnSelchangeTasklistsTablename)
	ON_CBN_SELCHANGE(IDC_TASKS_TABLENAME, OnSelchangeTasksTablename)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_ALLOCTO_KEYFIELD, OnSelchangeTableField)
	ON_CBN_SELCHANGE(IDC_ALLOCTO_NAMEFIELD, OnSelchangeTableField)
	ON_CBN_SELCHANGE(IDC_TASKLISTS_KEYFIELD, OnSelchangeTableField)
	ON_CBN_SELCHANGE(IDC_TASKLISTS_NAMEFIELD, OnSelchangeTableField)
	ON_CBN_SELCHANGE(IDC_TASKS_KEYFIELD, OnSelchangeTableField)
	ON_CBN_SELCHANGE(IDC_TASKS_TASKLISTKEYFIELD, OnSelchangeTableField)
	ON_CBN_SELCHANGE(IDC_TASKS_PARENTKEYFIELD, OnSelchangeTableField)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COdbcDatabaseSetupDlg message handlers

void COdbcDatabaseSetupDlg::OnValidate() 
{
	ODBCSETUP setup;
	GetStorageSetup(setup);

	CString sErrors;

	if (!COdbcHelper::VerifySetup(setup, sErrors))
	{
		sErrors = CEnString(IDS_ERR_VALIDATION) + _T("\n\n") + sErrors;
		AfxMessageBox(sErrors);
	}
}

BOOL COdbcDatabaseSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// title
	SetWindowText(m_sDlgTitle);

	// tasks table
	const ODBCTASKSTABLESETUP& table = m_setup.tables.tableTasks;

	COdbcHelper::InitCombo(m_cbTasksTableName, m_db, table.sTableName, TRUE);
	COdbcHelper::InitCombo(m_cbTasksKeyField, m_db, table.sTableName, table.sTaskKeyField);
	COdbcHelper::InitCombo(m_cbTasksParentKeyField, m_db, table.sTableName, table.sParentKeyField);
	COdbcHelper::InitCombo(m_cbTasksTasklistKeyField, m_db, table.sTableName, table.sTasklistKeyField, !HasTasklistTable());

	// tasklist table
	COdbcHelper::InitCombo(m_cbTasklistsTableName, m_db, m_setup.tables.tableTasklists, TRUE);
	COdbcHelper::InitCombo(m_cbTasklistsKeyField, m_db, m_setup.tables.tableTasklists, TRUE);
	COdbcHelper::InitCombo(m_cbTasklistsNameField, m_db, m_setup.tables.tableTasklists, FALSE);

	// alloc to table
	COdbcHelper::InitCombo(m_cbAllocToTableName, m_db, m_setup.tables.tableAllocTo, TRUE);
	COdbcHelper::InitCombo(m_cbAllocToKeyField, m_db, m_setup.tables.tableAllocTo, TRUE);
	COdbcHelper::InitCombo(m_cbAllocToNameField, m_db, m_setup.tables.tableAllocTo, FALSE);

	EnableCombos();

//	if (!m_sTasksTable.IsEmpty())
	if (!m_setup.tables.tableTasks.sTableName.IsEmpty())
	{
		m_lcAttribSetup.Initialise(m_db, m_setup.tables.tableTasks.sTableName, m_setup.aAttrib);
	}
	else // setup panel is disabled until Tasks table is set
	{
		m_lcAttribSetup.EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COdbcDatabaseSetupDlg::EnableCombos()
{
	m_cbAllocToKeyField.EnableWindow(!m_setup.tables.tableAllocTo.sTableName.IsEmpty());
	m_cbAllocToNameField.EnableWindow(!m_setup.tables.tableAllocTo.sTableName.IsEmpty());

	m_cbTasklistsKeyField.EnableWindow(!m_setup.tables.tableTasklists.sTableName.IsEmpty());
	m_cbTasklistsNameField.EnableWindow(!m_setup.tables.tableTasklists.sTableName.IsEmpty());

	m_cbTasksKeyField.EnableWindow(!m_setup.tables.tableTasks.sTableName.IsEmpty());
	m_cbTasksParentKeyField.EnableWindow(!m_setup.tables.tableTasks.sTableName.IsEmpty());
	m_cbTasksTasklistKeyField.EnableWindow(!m_setup.tables.tableTasks.sTableName.IsEmpty());

	CDialogHelper::RefreshMaxDropWidth(m_cbAllocToKeyField);
	CDialogHelper::RefreshMaxDropWidth(m_cbAllocToNameField);
	CDialogHelper::RefreshMaxDropWidth(m_cbTasklistsKeyField);
	CDialogHelper::RefreshMaxDropWidth(m_cbTasklistsNameField);
	CDialogHelper::RefreshMaxDropWidth(m_cbTasksKeyField);
	CDialogHelper::RefreshMaxDropWidth(m_cbTasksParentKeyField);
	CDialogHelper::RefreshMaxDropWidth(m_cbTasksTasklistKeyField);
}

void COdbcDatabaseSetupDlg::GetStorageSetup(ODBCSETUP& setup)
{
	setup = m_setup;
}

void COdbcDatabaseSetupDlg::OnEditTaskAttributeList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	m_lcAttribSetup.GetAttributeSetup(m_setup.aAttrib);
	
	*pResult = 0;
}

void COdbcDatabaseSetupDlg::OnSelchangeAlloctoTablename() 
{
	UpdateData();

	// initialize
	COdbcHelper::InitCombo(m_cbAllocToKeyField, m_db, m_setup.tables.tableAllocTo, TRUE);
	COdbcHelper::InitCombo(m_cbAllocToNameField, m_db, m_setup.tables.tableAllocTo, FALSE);

	// and enable 
	EnableCombos();
}

void COdbcDatabaseSetupDlg::OnSelchangeTasklistsTablename() 
{
	UpdateData();
	
	COdbcHelper::InitCombo(m_cbTasklistsKeyField, m_db, m_setup.tables.tableTasklists, TRUE);
	COdbcHelper::InitCombo(m_cbTasklistsNameField, m_db, m_setup.tables.tableTasklists, FALSE);

	// re-init task 'tasklist' combo to add/remove empty item
	const ODBCTASKSTABLESETUP& table = m_setup.tables.tableTasks;

	COdbcHelper::InitCombo(m_cbTasksTasklistKeyField, m_db, table.sTableName, table.sTasklistKeyField, !HasTasklistTable());

	// and enable 
	EnableCombos();
}

void COdbcDatabaseSetupDlg::OnSelchangeTasksTablename() 
{
	UpdateData();
	
	const ODBCTASKSTABLESETUP& table = m_setup.tables.tableTasks;

	COdbcHelper::InitCombo(m_cbTasksKeyField, m_db, table.sTableName, _T(""), TRUE);
	COdbcHelper::InitCombo(m_cbTasksParentKeyField, m_db, table.sTableName, table.sParentKeyField);
	COdbcHelper::InitCombo(m_cbTasksTasklistKeyField, m_db, table.sTableName, table.sTasklistKeyField, !HasTasklistTable());

	// and enable 
	EnableCombos();
	
	COdbcAttribSetupArray aSetup;
	
	m_lcAttribSetup.Initialise(m_db, m_setup.tables.tableTasks.sTableName, aSetup);
	m_lcAttribSetup.EnableWindow(TRUE);
}

void COdbcDatabaseSetupDlg::OnSelchangeTableField() 
{
	// update m_setup
	UpdateData();
}

BOOL COdbcDatabaseSetupDlg::HasTasklistTable() const
{
	return !m_setup.tables.tableTasklists.sTableName.IsEmpty();
}

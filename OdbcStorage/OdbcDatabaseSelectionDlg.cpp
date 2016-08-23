// OdbcDatabaseSelectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "Odbchelper.h"
#include "OdbcDatabaseSelectionDlg.h"

#include "..\shared\misc.h"
#include "..\shared\databaseEx.h"
#include "..\shared\dialoghelper.h"

#include "..\Interfaces\IPreferences.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

const LPCTSTR DEFAULT_CONNECT = _T("ODBC;");

/////////////////////////////////////////////////////////////////////////////
// COdbcDatabaseSelectionDlg dialog

COdbcDatabaseSelectionDlg::COdbcDatabaseSelectionDlg(const COdbcDatabaseSetupArray& aDbSetup, CWnd* pParent /*=NULL*/)
	: 
	CDialog(IDD_DATABASESELECTION_DIALOG, pParent), 
	m_lcDatabases(aDbSetup), 
	m_nSelDb(-1)
{
	//{{AFX_DATA_INIT(COdbcDatabaseSelectionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COdbcDatabaseSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COdbcDatabaseSelectionDlg)
	DDX_Control(pDX, IDC_DATABASE_LIST, m_lcDatabases);
	DDX_Control(pDX, IDC_ALLOCTOLIST, m_cbAllocTo);
	DDX_Control(pDX, IDC_TASKLISTS, m_cbTasklists);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		m_dbSelectedSetup.sAllocToID = m_cbAllocTo.GetSelectedValueKey();
		m_dbSelectedSetup.sTasklistID = m_cbTasklists.GetSelectedValueKey();

		m_dbSelectedSetup.sAllocTo = CDialogHelper::GetSelectedItem(m_cbAllocTo);
		m_dbSelectedSetup.sTasklist = CDialogHelper::GetSelectedItem(m_cbTasklists);
	}
	else
	{
		m_cbAllocTo.SelectValueByKey(m_dbSelectedSetup.sAllocToID);
		m_cbTasklists.SelectValueByKey(m_dbSelectedSetup.sTasklistID);
	}
}


BEGIN_MESSAGE_MAP(COdbcDatabaseSelectionDlg, CDialog)
	//{{AFX_MSG_MAP(COdbcDatabaseSelectionDlg)
	ON_BN_CLICKED(IDC_NEW_DATABASE, OnNewDatabase)
	ON_NOTIFY(LVN_USERSELCHANGE, IDC_DATABASE_LIST, OnSelchangedDatabaseList)
	ON_BN_CLICKED(IDC_DELETE_DATABASE, OnDeleteDatabase)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_DATABASE_LIST, OnEditSelectedDatabase)
	ON_CBN_SELCHANGE(IDC_ALLOCTOLIST, OnSelchangeModifiers)
	ON_CBN_SELCHANGE(IDC_TASKLISTS, OnSelchangeModifiers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COdbcDatabaseSelectionDlg message handlers

void COdbcDatabaseSelectionDlg::OnNewDatabase() 
{
	CDatabaseEx db;

	if (db.Connect(DEFAULT_CONNECT))
	{
		ODBCDATABASESETUP dbSetup;

		dbSetup.sDbName = db.GetDatabaseName();
		dbSetup.sConnect = db.GetConnect();

		if (!m_lcDatabases.AddDatabaseSetup(dbSetup))
			AfxMessageBox(CEnString(IDS_ERR_DATABASE_EXISTS));
	}

	//m_lcDatabases.Invalidate(TRUE); // always
}

BOOL COdbcDatabaseSelectionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (m_lcDatabases.Initialize() && m_lcDatabases.GetItemCount())
	{
		m_lcDatabases.SetCurSel(0, 0);
		m_lcDatabases.SetFocus();

		UpdateSelectedDatabase();
		UpdateData(FALSE);
	}
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int COdbcDatabaseSelectionDlg::GetSelectedDatabaseSetup(ODBCDATABASESETUP& dbSetup) const
{
	dbSetup = m_dbSelectedSetup;

	return m_nSelDb;
}

int COdbcDatabaseSelectionDlg::GetDatabaseSetups(COdbcDatabaseSetupArray& aDbSetup) const
{
	int nCount = m_lcDatabases.GetDatabaseSetups(aDbSetup);

	return nCount;
}

void COdbcDatabaseSelectionDlg::OnSelchangedDatabaseList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nSel = m_lcDatabases.GetCurSel();

	if ((nSel != m_nSelDb))
	{
		m_nSelDb = nSel;
		m_lcDatabases.GetSelectedDatabaseSetup(m_dbSelectedSetup);

		CDatabaseEx db;

		if (db.Connect(m_dbSelectedSetup.sConnect))
		{
			InitialiseCombos(db);
			UpdateData(FALSE);
		}
		else
		{
			m_cbAllocTo.ResetContent();
			m_cbTasklists.ResetContent();
	
			m_cbAllocTo.EnableWindow(FALSE);
			m_cbTasklists.EnableWindow(FALSE);
		}
	}
	
	*pResult = 0;
}

void COdbcDatabaseSelectionDlg::InitialiseCombos(CDatabaseEx& db)
{
	COdbcHelper::InitCombo(m_cbTasklists, db, m_dbSelectedSetup.setup.tables.tableTasklists);
	COdbcHelper::InitCombo(m_cbAllocTo, db, m_dbSelectedSetup.setup.tables.tableAllocTo, TRUE);
}

void COdbcDatabaseSelectionDlg::UpdateSelectedDatabase()
{
	// fake database selection change
	NM_LISTVIEW nmlv = { 0 };
	LRESULT lr = 0;

	m_nSelDb = -1;
	OnSelchangedDatabaseList(&(nmlv.hdr), &lr);
}

void COdbcDatabaseSelectionDlg::OnDeleteDatabase() 
{
	if (AfxMessageBox(IDS_CONFIRM_DELETE_DB, MB_ICONWARNING | MB_YESNO) == IDYES)
	{
		m_lcDatabases.DeleteSelectedDatabaseSetup();
	}
}

void COdbcDatabaseSelectionDlg::OnEditSelectedDatabase(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	m_lcDatabases.GetSelectedDatabaseSetup(m_dbSelectedSetup);
	UpdateSelectedDatabase();

	*pResult = 0;
}

void COdbcDatabaseSelectionDlg::OnSelchangeModifiers() 
{
	UpdateData();

	m_lcDatabases.SetSelectedDatabaseModifiers(m_dbSelectedSetup.sTasklistID, m_dbSelectedSetup.sAllocToID);
}

void COdbcDatabaseSelectionDlg::OnOK()
{
	UpdateData();

	// don't let the user selected a 'bad' database setup
	CString sErrors;

	if (COdbcHelper::VerifySetup(m_dbSelectedSetup.setup, sErrors))
	{
		CDialog::OnOK();
		return;
	}

	// else notify the user
	sErrors = CEnString(IDS_ERR_SELECTEDDATABASE) + _T("\n\n") + sErrors;
	AfxMessageBox(sErrors);
}

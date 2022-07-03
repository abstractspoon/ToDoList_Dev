// TDLCleanupPreferencesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLCleanupIniPreferencesDlg.h"

#include "..\Shared\misc.h"
#include "..\Shared\filemisc.h"
#include "..\Shared\EnString.h"

#include "..\Interfaces\Preferences.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

LPCTSTR FILESTATEKEY = _T("FileStates");
LPCTSTR REMINDERKEY	 = _T("Reminders");
LPCTSTR DEFAULTKEY	 = _T("Default");
LPCTSTR SETTINGSKEY	 = _T("Settings");

/////////////////////////////////////////////////////////////////////////////
// CTDLCleanupPreferencesDlg dialog


CTDLCleanupIniPreferencesDlg::CTDLCleanupIniPreferencesDlg(const CStringArray& aExclusions, CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_CLEANUPINIPREFERENCES_DIALOG, _T("CleanupPrefs"), pParent),
	m_mru(0, _T("MRU"), _T("TaskList%d"), 16)
{
	//{{AFX_DATA_INIT(CTDLCleanupIniPreferencesDlg)
	//}}AFX_DATA_INIT
	m_sPrefsFile = CPreferences::GetPath();
	
	m_aExclusions.Copy(aExclusions);
	m_aExclusions.Add(DEFAULTKEY);
}


void CTDLCleanupIniPreferencesDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLCleanupIniPreferencesDlg)
	DDX_Control(pDX, IDC_TASKLISTS, m_lbTasklists);
	DDX_Text(pDX, IDC_PREFSLOCATION, m_sPrefsFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTDLCleanupIniPreferencesDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLCleanupIniPreferencesDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SELECTALL, OnSelectAll)
	ON_BN_CLICKED(IDC_CLEARALL, OnClearAll)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLCleanupPreferencesDlg message handlers

BOOL CTDLCleanupIniPreferencesDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();
	
	// Build the list of tasklists
	CPreferences prefs;
	CStringArray aSections;

	if (!prefs.GetProfileSectionNames(aSections))
	{
		EndDialog(IDCANCEL);
	}
	else
	{
		// Build a list of all tasklists having state info in the ini file
		CStringArray aTasklists;
		int nSection = aSections.GetSize();
		
		while (nSection--)
		{
			const CString& sSection = aSections[nSection];
			
			// does it start with "FileStates\\" 
			if ((sSection.Find(FILESTATEKEY) != 0) && (sSection.Find(REMINDERKEY) != 0))
				continue;

			// split the section name into its parts
			CStringArray aSubSections;
			
			// We are only interested in the section with two parts
			// the file name is always the second item
			if (Misc::Split(sSection, aSubSections, '\\') != 2)
				continue;

			aTasklists.Add(aSubSections[1]);
		}

		// Also check the MRU
		m_mru.ReadList(prefs);
		m_mru.GetFileNames(m_aMRU);

		Misc::AddUniqueItems(m_aMRU, aTasklists);
				
		// 'Last Active' tasklist
		m_sLastActiveFile = prefs.GetProfileString(SETTINGSKEY, _T("LastActiveFile"));

		if (!m_sLastActiveFile.IsEmpty())
			Misc::AddUniqueItem(FileMisc::GetFileNameFromPath(m_sLastActiveFile), aTasklists);

		// and 'Last Open' tasklists
		int nTDCCount = prefs.GetProfileInt(SETTINGSKEY, _T("NumLastFiles"), 0);
		m_aLastOpen.RemoveAll();
		
		for (int nTDC = 0; nTDC < nTDCCount; nTDC++)
		{
			CString sKey = Misc::MakeKey(_T("LastFile%d"), nTDC);
			CString sLastFile = prefs.GetProfileString(SETTINGSKEY, sKey);
			
			if (!sLastFile.IsEmpty())
			{
				Misc::AddUniqueItem(sLastFile, m_aLastOpen);
				Misc::AddUniqueItem(FileMisc::GetFileNameFromPath(sLastFile), aTasklists);
			}
		}
		
		// Remove 'Exclusions'
		Misc::RemoveItems(m_aExclusions, aTasklists);

		// Add to list box
		int nItem = aTasklists.GetSize();

		if (nItem)
		{
			while (nItem--)
				m_lbTasklists.AddString(aTasklists[nItem]);
		}
		else
		{
			AfxMessageBox(CEnString(IDS_CLEANUPPREFS_NOTASLISTS));
			EndDialog(IDCANCEL);
		}
	}
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLCleanupIniPreferencesDlg::OnOK()
{
	CTDLDialog::OnOK();

	// Get the checked items from the list
	CStringArray aChecked;
	int nTasklist = m_lbTasklists.GetCheckedItems(aChecked);

	if (nTasklist)
	{
		// Confirm with user
		CEnString sConfirm(IDS_CONFIRM_CLEANUPPREFS);
		
		sConfirm += _T("\n\n");
		sConfirm += Misc::FormatArray(aChecked, _T("\n\n"));

		if (AfxMessageBox(sConfirm, MB_YESNO) == IDNO)
			return;

		CWaitCursor cursor;
		CPreferences prefs;

		// Traverse the selected tasklists deleting info as we go
		BOOL bMRUChange = FALSE, bLastOpenChange = FALSE;
		CString sSection;
		
		while (nTasklist--)
		{
			CString sTasklist = aChecked[nTasklist];

			// File state information
			sSection.Format(_T("%s\\%s"), FILESTATEKEY, sTasklist);
			
			if (prefs.HasProfileSection(sSection))
				VERIFY(prefs.DeleteProfileSection(sSection, TRUE));
			
			// Reminders
			sSection.Format(_T("%s\\%s"), REMINDERKEY, sTasklist);
			
			if (prefs.HasProfileSection(sSection))
				VERIFY(prefs.DeleteProfileSection(sSection, TRUE));

			// MRU
			int nFind = Misc::Find(sTasklist, m_aMRU);

			while (nFind != -1)
			{
				m_mru.Remove(nFind);
				m_aMRU.RemoveAt(nFind);

				bMRUChange = TRUE;
				nFind = Misc::Find(sTasklist, m_aMRU);
			}

			// Last Open tasklists
			nFind = Misc::Find(sTasklist, m_aLastOpen);
			
			while (nFind != -1)
			{
				m_aLastOpen.RemoveAt(nFind);
				
				bLastOpenChange = TRUE;
				nFind = Misc::Find(sTasklist, m_aLastOpen);
			}

			if (Misc::Find(sTasklist, m_sLastActiveFile) != -1)
			{
				m_sLastActiveFile.Empty();
				bLastOpenChange = TRUE;
			}
		}

		// Update the MRU/Last Open lists
		if (bMRUChange)
			m_mru.WriteList(prefs, TRUE);

		if (bLastOpenChange)
		{
			prefs.WriteProfileString(SETTINGSKEY, _T("LastActiveFile"), m_sLastActiveFile);
			int nTDCCount = m_aLastOpen.GetSize();
			
			for (int nTDC = 0; nTDC < nTDCCount; nTDC++)
			{
				CString sKey = Misc::MakeKey(_T("LastFile%d"), nTDC);
				prefs.WriteProfileString(SETTINGSKEY, sKey, m_aLastOpen[nTDC]);
			}
		}

		// Backup the preferences
		CFileBackup(prefs.GetPath(), FBS_OVERWRITE, _T(""), _T(".pre_cleanup"));
		
		prefs.Save();
	}
}

void CTDLCleanupIniPreferencesDlg::OnSelectAll()
{
	m_lbTasklists.SetAllChecked(TRUE);
}

void CTDLCleanupIniPreferencesDlg::OnClearAll()
{
	m_lbTasklists.SetAllChecked(FALSE);
}

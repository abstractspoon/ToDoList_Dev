// TDLCsvImportExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLCsvImportExportDlg.h"
#include "tdcstatic.h"

#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\fileregister.h"
#include "..\shared\dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

static const CString COMMA(_T(","));

/////////////////////////////////////////////////////////////////////////////
// CTDLCsvImportExportDlg dialog

CTDLCsvImportExportDlg::CTDLCsvImportExportDlg(const CString& sFilePath,
											   const CMapStringToString& mapImportCustAttrib,
											   IPreferences* pPrefs, 
											   LPCTSTR szKey, 
											   CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_CSVIMPORTEXPORT_DIALOG, _T(""), pParent), 
	m_lcColumnSetup(TRUE), 
	m_eFilePath(FES_NOBROWSE)
{
	Misc::Copy(mapImportCustAttrib, m_mapImportCustAttrib);

	VERIFY(DoInit(TRUE, sFilePath, pPrefs, szKey));
}

CTDLCsvImportExportDlg::CTDLCsvImportExportDlg(const CString& sFilePath, 
											   const CTDCAttributeArray& aExportAttributes, 
											   IPreferences* pPrefs, 
											   LPCTSTR szKey, 
											   CWnd* pParent /*=NULL*/)
	: 
	CTDLDialog(IDD_CSVIMPORTEXPORT_DIALOG, _T(""), pParent), 
	m_lcColumnSetup(FALSE), 
	m_eFilePath(FES_NOBROWSE)
{
	m_aExportAttributes.Copy(aExportAttributes);
	
	VERIFY(DoInit(FALSE, sFilePath, pPrefs, szKey));
}

BOOL CTDLCsvImportExportDlg::DoInit(BOOL bImport, const CString& sFilePath, IPreferences* pPrefs, LPCTSTR szKey)
{
	m_bImporting = bImport;
	m_sFilePath = sFilePath; 
	m_pPrefs = pPrefs;
	m_sPrefsKey.Format(_T("%s\\CsvColumnMapping"), szKey);
	m_sDelim = Misc::GetListSeparator();
	m_bAlwaysExportTaskIDs = TRUE;

	InitialiseDelimiter();
	LoadMasterColumnMapping();

	// user mapping
	CTDCAttributeMapping aMapping;

	if (m_bImporting)
		BuildImportColumnMapping(aMapping);
	else
		BuildExportColumnMapping(aMapping);

	m_lcColumnSetup.SetColumnMapping(aMapping);

	return TRUE;
}

void CTDLCsvImportExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CTDLDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTDLCsvImportExportDlg)
	DDX_Text(pDX, IDC_CSVDELIMITER, m_sDelim);
	DDX_Text(pDX, IDC_CSVFILEPATH, m_sFilePath); 
	DDX_Control(pDX, IDC_CSVFILEPATH, m_eFilePath);
	DDX_Control(pDX, IDC_COLUMNSETUP, m_lcColumnSetup);
	DDX_Check(pDX, IDC_EXPORTTASKIDS, m_bAlwaysExportTaskIDs);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTDLCsvImportExportDlg, CTDLDialog)
	//{{AFX_MSG_MAP(CTDLCsvImportExportDlg)
	ON_EN_CHANGE(IDC_CSVDELIMITER, OnChangeCsvdelimiter)
	ON_BN_CLICKED(IDC_EXPORTTASKIDS, OnExportTaskIds)
	ON_CONTROL(TDCN_IMPORTMAPPINGCHANGE, IDC_COLUMNSETUP, OnImportMappingChange)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLCsvImportExportDlg message handlers

BOOL CTDLCsvImportExportDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

	m_lcColumnSetup.SetFirstColumnStretchy(TRUE);

	if (m_bImporting)
	{
		SetWindowText(CEnString(IDS_CSV_IMPORTDLG));

		GetDlgItem(IDC_EXPORTTASKIDS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EXPORTTASKIDS)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DIVIDER)->EnableWindow(FALSE);
		GetDlgItem(IDC_DIVIDER)->ShowWindow(SW_HIDE);

		// Resize the list to take up the space
		CRect rList = CDialogHelper::GetChildRect(&m_lcColumnSetup);
		CRect rDivider = CDialogHelper::GetCtrlRect(this, IDC_DIVIDER);

		rList.bottom = rDivider.bottom;

		m_lcColumnSetup.MoveWindow(rList);

		EnableDisableOK();
	}
	else
	{
		SetWindowText(CEnString(IDS_CSV_EXPORTDLG));
		OnExportTaskIds();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLCsvImportExportDlg::InitialiseDelimiter()
{
	ASSERT(GetSafeHwnd() == NULL);

	// load last used delimiter
	CString sUIDelim = m_pPrefs->GetProfileString(m_sPrefsKey, _T("Delimiter"));

	if (m_bImporting)
	{
		ASSERT(!m_sFilePath.IsEmpty());

		// read first few lines from file
		CStringArray aLines;

		if (FileMisc::LoadFile(m_sFilePath, aLines, 5))
		{
			CStringArray aDelims;

			if (!sUIDelim.IsEmpty())
				aDelims.Add(GetFileDelimiter(sUIDelim));

			if (IsUsingExcel())
			{
				aDelims.Add(_T("\t"));
				aDelims.Add(Misc::GetListSeparator());
			}
			else
			{
				aDelims.Add(Misc::GetListSeparator());
				aDelims.Add(_T("\t"));
			}

			if (Misc::GetListSeparator() != COMMA)
				aDelims.Add(COMMA);

			// Search the lines for the highest number of delimiter matches
			int nMaxDelim = -1, nMaxSplits = 0;
			CStringArray aUnused;

			for (int nDelim = 0; nDelim < aDelims.GetSize(); nDelim++)
			{
				for (int nLine = 0; nLine < aLines.GetSize(); nLine++)
				{
					int nSplits = Misc::Split(aLines[nLine], aUnused, aDelims[nDelim], TRUE);

					if ((nMaxDelim == -1) || (nSplits > nMaxSplits))
					{
						nMaxDelim = nDelim;
						nMaxSplits = nSplits;
					}
				}
			}

			if (nMaxDelim != -1)
				sUIDelim = GetUIDelimiter(aDelims[nMaxDelim]);
		}
	}
	else
	{
		// If Excel is the default app for csv we always use tab
		if (IsExportingForExcel())
			sUIDelim = _T("\\t");
	}

	if (!sUIDelim.IsEmpty())
		m_sDelim = sUIDelim;
}

int CTDLCsvImportExportDlg::GetColumnMapping(CTDCAttributeMapping& aMapping) const 
{ 
	int nNumMappings = m_lcColumnSetup.GetColumnMapping(aMapping);

	// Fixup existing custom attribute column names
	if (m_bImporting)
	{
		for (int nMapping = 0; nMapping < nNumMappings; nMapping++)
		{
			TDCATTRIBUTEMAPPING& mapping = aMapping[nMapping];

			if (mapping.nTDCAttrib == TDCA_EXISTING_CUSTOMATTRIBUTE)
			{
				CString sCustID = FindCustomAttributeID(mapping.sColumnName), sCustLabel;
				ASSERT(!sCustID.IsEmpty());

				m_mapImportCustAttrib.Lookup(sCustID, sCustLabel);
				ASSERT(!sCustLabel.IsEmpty());

				mapping.sColumnName.Format(_T("%s (%s)"), sCustLabel, sCustID);
			}
		}
	}

	return aMapping.GetSize();
}

CString CTDLCsvImportExportDlg::GetDelimiter() const 
{ 
	return GetFileDelimiter(m_sDelim); 
}

CString CTDLCsvImportExportDlg::GetFileDelimiter(const CString& sUIDelim)
{
	// some special cases
	if (sUIDelim == _T("\\t"))
	{
		return _T("\t");
	}
	else if (sUIDelim == _T("\\n"))
	{
		return _T("\n");
	}
	else if (sUIDelim == _T("\\r\\n"))
	{
		return _T("\r\n");
	}

	// else
	return sUIDelim; 
}

CString CTDLCsvImportExportDlg::GetUIDelimiter(const CString& sFileDelim)
{
	// some special cases
	if (sFileDelim == _T("\t"))
	{
		return _T("\\t");
	}
	else if (sFileDelim == _T("\n"))
	{
		return _T("\\n");
	}
	else if (sFileDelim == _T("\r\n"))
	{
		return _T("\\r\\n");
	}

	// else
	return sFileDelim; 
}

void CTDLCsvImportExportDlg::OnChangeCsvdelimiter() 
{
	CString sOldDelim = m_sDelim;
	UpdateData();

	if (!m_sDelim.IsEmpty() && m_bImporting && (m_sDelim != sOldDelim))
	{
		CTDCAttributeMapping aMapping;
		
		if (BuildImportColumnMapping(aMapping))
			m_lcColumnSetup.SetColumnMapping(aMapping);
	}
}

void CTDLCsvImportExportDlg::BuildDefaultMasterColumnMapping()
{
	m_aMasterColumnMapping.RemoveAll();

	for (int nCol = 0; nCol < ATTRIB_COUNT; nCol++)
	{
		TDC_ATTRIBUTE attrib = ATTRIBUTES[nCol].nAttribID;
		CEnString sName(ATTRIBUTES[nCol].nAttribResID);

		m_aMasterColumnMapping.Add(TDCATTRIBUTEMAPPING(sName, attrib));
	}
}

int CTDLCsvImportExportDlg::BuildImportColumnMapping(CTDCAttributeMapping& aImportMapping) const
{
	ASSERT (m_bImporting);
	ASSERT(!m_sFilePath.IsEmpty());

	// read first few lines from file
	CStringArray aLines;
	
	if (!FileMisc::LoadFile(m_sFilePath, aLines, 5))
		return FALSE;
	
	// look for the first line containing something
	// we can use as column heading names
	CStringArray aColumnHeaders;

	for (int nLine = 0; nLine < aLines.GetSize(); nLine++)
	{
		if (Misc::Split(aLines[nLine], aColumnHeaders, GetDelimiter(), TRUE))
			break;
	}

	if (aColumnHeaders.GetSize() == 0)
		return FALSE;
		
	// build column mapping from file attributes
	for (int nCol = 0; nCol < aColumnHeaders.GetSize(); nCol++)
	{
		CString sName = Misc::GetItem(aColumnHeaders, nCol);

		if (sName.IsEmpty())
			sName = CEnString(IDS_UNNAMEDFILTER);

		// try to map text column names to column IDs
		// Note: Must include _all_ columns else the column
		// indices will be out of sync with the data rows
		aImportMapping.Add(TDCATTRIBUTEMAPPING(sName, GetMasterColumnAttribute(sName)));
	}

	return aImportMapping.GetSize();
}

BOOL CTDLCsvImportExportDlg::IsExportingForExcel() const
{
	return (!m_bImporting && IsUsingExcel() && 
			!m_pPrefs->GetProfileInt(_T("Preferences"), _T("ExportCsvToUTF8"), FALSE));
}

BOOL CTDLCsvImportExportDlg::IsUsingExcel()
{
	return CFileRegister::IsRegisteredApp(_T("csv"), _T("EXCEL.EXE"), TRUE);
}

int CTDLCsvImportExportDlg::BuildExportColumnMapping(CTDCAttributeMapping& aExportMapping) const
{
	ASSERT (!m_bImporting);

	// build column mapping from passed in attributes
	for (int nAttrib = 0; nAttrib < m_aExportAttributes.GetSize(); nAttrib++)
	{
		TDC_ATTRIBUTE attrib = m_aExportAttributes[nAttrib];
		ASSERT(attrib != TDCA_NONE);

		// try to map text column names to column IDs
		aExportMapping.Add(TDCATTRIBUTEMAPPING(GetMasterColumnName(attrib), attrib));
	}

	return aExportMapping.GetSize();
}

void CTDLCsvImportExportDlg::OnOK()
{
	CTDLDialog::OnOK();
	
	// save attribute mapping
	UpdateMasterColumnMappingFromList();
	SaveMasterColumnMapping();
}

int CTDLCsvImportExportDlg::LoadMasterColumnMapping()
{
	BuildDefaultMasterColumnMapping();

	m_bAlwaysExportTaskIDs = m_pPrefs->GetProfileInt(m_sPrefsKey, _T("AlwaysExportTaskIDs"), TRUE);
	int nColumns = m_pPrefs->GetProfileInt(m_sPrefsKey, _T("ColumnCount"), 0);

	// overwrite with translations unless they are empty names
	for (int nCol = 0; nCol < nColumns; nCol++)
	{
		CString sKey = Misc::MakeKey(_T("ColumnAttrib%d"), nCol);
		TDC_ATTRIBUTE attrib = (TDC_ATTRIBUTE)m_pPrefs->GetProfileInt(m_sPrefsKey, sKey, TDCA_NONE);
		
		sKey = Misc::MakeKey(_T("ColumnName%d"), nCol);
		CString sName = m_pPrefs->GetProfileString(m_sPrefsKey, sKey);
		
		if (!sName.IsEmpty())
			SetMasterColumnName(attrib, sName);
	}

	return m_aMasterColumnMapping.GetSize();
}

void CTDLCsvImportExportDlg::UpdateMasterColumnMappingFromList()
{
	// get mapping from list ctrl and update names in master mapping
	CTDCAttributeMapping aListMapping;
	int nListRows = m_lcColumnSetup.GetColumnMapping(aListMapping);

	for (int nRow = 0; nRow < nListRows; nRow++)
	{
		const TDCATTRIBUTEMAPPING& mapping = aListMapping[nRow];

		SetMasterColumnName(mapping.nTDCAttrib, mapping.sColumnName);
	}
}

void CTDLCsvImportExportDlg::SaveMasterColumnMapping() const
{
	m_pPrefs->WriteProfileInt(m_sPrefsKey, _T("AlwaysExportTaskIDs"), m_bAlwaysExportTaskIDs);

	int nColumns = m_aMasterColumnMapping.GetSize();
	m_pPrefs->WriteProfileInt(m_sPrefsKey, _T("ColumnCount"), nColumns);

	for (int nCol = 0; nCol < nColumns; nCol++)
	{
		const TDCATTRIBUTEMAPPING& col = m_aMasterColumnMapping[nCol];

		CString sKey = Misc::MakeKey(_T("ColumnName%d"), nCol);
		m_pPrefs->WriteProfileString(m_sPrefsKey, sKey, col.sColumnName);
		
		sKey = Misc::MakeKey(_T("ColumnAttrib%d"), nCol);
		m_pPrefs->WriteProfileInt(m_sPrefsKey, sKey, col.nTDCAttrib);
	}

	// save delimiter if different to default
	if (m_sDelim == Misc::GetListSeparator())
		m_pPrefs->WriteProfileString(m_sPrefsKey, _T("Delimiter"), _T(""));
	else
		m_pPrefs->WriteProfileString(m_sPrefsKey, _T("Delimiter"), m_sDelim);
}

CString CTDLCsvImportExportDlg::GetMasterColumnName(TDC_ATTRIBUTE attrib) const
{
	ASSERT(!m_bImporting);

	int nCol = FindMasterColumn(attrib);
	return (nCol == -1) ? _T("") : m_aMasterColumnMapping[nCol].sColumnName;
}

TDC_ATTRIBUTE CTDLCsvImportExportDlg::GetMasterColumnAttribute(LPCTSTR szColumn) const
{
	ASSERT(m_bImporting);

	int nCol = FindMasterColumn(szColumn);

	if (nCol != -1)
		return m_aMasterColumnMapping[nCol].nTDCAttrib;

	// Try for a custom attribute
	if (!FindCustomAttributeID(szColumn).IsEmpty())
		return TDCA_EXISTING_CUSTOMATTRIBUTE;

	return TDCA_NONE;
}

CString CTDLCsvImportExportDlg::FindCustomAttributeID(LPCTSTR szColumn) const
{
	ASSERT(m_bImporting);

	POSITION pos = m_mapImportCustAttrib.GetStartPosition();

	while (pos)
	{
		CString sCustID, sCustLabel;
		m_mapImportCustAttrib.GetNextAssoc(pos, sCustID, sCustLabel);

		if (sCustID.CompareNoCase(szColumn) == 0)
			return sCustID;

		if (sCustLabel.CompareNoCase(szColumn) == 0)
			return sCustID;

		// Try combined name
		CString sCombinedName;
		sCombinedName.Format(_T("%s (%s)"), sCustLabel, sCustID);

		if (sCombinedName.CompareNoCase(szColumn) == 0)
			return sCustID;
	}

	// not found
	return _T("");
}

void CTDLCsvImportExportDlg::SetMasterColumnAttribute(LPCTSTR szColumn, TDC_ATTRIBUTE attrib)
{
	// check if attribute is already in use
	int nAttribCol = FindMasterColumn(attrib);
	int nNameCol = FindMasterColumn(szColumn);

	// and clear if it is
	if (nAttribCol != -1 && nAttribCol != nNameCol)
		m_aMasterColumnMapping[nNameCol].nTDCAttrib = TDCA_NONE;

	if (nNameCol != -1)
		m_aMasterColumnMapping[nNameCol].nTDCAttrib = attrib;
}

void CTDLCsvImportExportDlg::SetMasterColumnName(TDC_ATTRIBUTE attrib, LPCTSTR szColumn)
{
	// prevent setting the master mapping to an empty name
	if ((attrib != TDCA_NONE) && !Misc::IsEmpty(szColumn))
	{
		// check if the column name is already in use
		int nNameCol = FindMasterColumn(szColumn);
		int nAttribCol = FindMasterColumn(attrib);

		// and clear if it is
		if ((nNameCol != -1) && (nNameCol != nAttribCol))
			m_aMasterColumnMapping[nNameCol].sColumnName.Empty();

		// and set new name
		if (nAttribCol != -1)
			m_aMasterColumnMapping[nAttribCol].sColumnName = szColumn;
	}
}

int CTDLCsvImportExportDlg::FindMasterColumn(TDC_ATTRIBUTE attrib) const
{
	int nColumns = m_aMasterColumnMapping.GetSize();

	for (int nCol = 0; nCol < nColumns; nCol++)
	{
		if (m_aMasterColumnMapping[nCol].nTDCAttrib == attrib)
			return nCol;
	}

	// else
	return -1;
}

int CTDLCsvImportExportDlg::FindMasterColumn(LPCTSTR szColumn) const
{
	int nColumns = m_aMasterColumnMapping.GetSize();

	for (int nCol = 0; nCol < nColumns; nCol++)
	{
		if (m_aMasterColumnMapping[nCol].sColumnName.CompareNoCase(szColumn) == 0)
			return nCol;
	}

	// else
	return -1;
}

HBRUSH CTDLCsvImportExportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTDLDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ((pWnd->GetDlgCtrlID() == IDC_IMPORT_MUSTMAPTITLE) && !Misc::IsHighContrastActive())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(255);
	}

	return hbr;
}

void CTDLCsvImportExportDlg::OnExportTaskIds() 
{
	ASSERT(!m_bImporting);

	UpdateData();

	CTDCAttributeMapping aMapping;
	m_lcColumnSetup.GetColumnMapping(aMapping);

	if (!m_bAlwaysExportTaskIDs)
	{
		// Find if these attributes were originally present
		BOOL bWantTaskID = FALSE, bWantParentID = FALSE;

		for (int nAttrib = 0; nAttrib < m_aExportAttributes.GetSize(); nAttrib++)
		{
			if (!bWantTaskID)
				bWantTaskID = (m_aExportAttributes[nAttrib] == TDCA_ID);

			if (!bWantParentID)
				bWantParentID = (m_aExportAttributes[nAttrib] == TDCA_PARENTID);
		}

		// if attribute was not present in original attributes then remove
		int nCol = aMapping.GetSize();

		while (nCol--)
		{
			TDC_ATTRIBUTE attrib = aMapping[nCol].nTDCAttrib;

			if (attrib == TDCA_ID && !bWantTaskID)
				aMapping.RemoveAt(nCol);

			else if (attrib == TDCA_PARENTID && !bWantParentID)
				aMapping.RemoveAt(nCol);
		}
	}
	else // always include
	{
		// find out what's already present
		int nTaskID = -1, nParentTaskID = -1;

		for (int nCol = 0; nCol < aMapping.GetSize() && (nTaskID == -1 || nParentTaskID == -1); nCol++)
		{
			TDC_ATTRIBUTE attrib = aMapping[nCol].nTDCAttrib;

			if (attrib == TDCA_ID)
				nTaskID = nCol;

			else if (attrib == TDCA_PARENTID)
				nParentTaskID = nCol;
		}

		// Add TaskID and/or ParentTaskID if not present
		if (nTaskID == -1)
			aMapping.Add(TDCATTRIBUTEMAPPING(GetMasterColumnName(TDCA_ID), TDCA_ID)); 

		if (nParentTaskID == -1)
			aMapping.Add(TDCATTRIBUTEMAPPING(GetMasterColumnName(TDCA_PARENTID), TDCA_PARENTID));
	}
	
	m_lcColumnSetup.SetColumnMapping(aMapping);
}

void CTDLCsvImportExportDlg::OnImportMappingChange()
{
	EnableDisableOK();
}

void CTDLCsvImportExportDlg::EnableDisableOK()
{
	BOOL bEnable = (!m_bImporting || m_lcColumnSetup.IsAttributeMapped(TDCA_TASKNAME));

	GetDlgItem(IDOK)->EnableWindow(bEnable);
	GetDlgItem(IDC_IMPORT_MUSTMAPTITLE)->ShowWindow(bEnable ? SW_HIDE : SW_SHOW);
}

// TDLCsvImportExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLCsvImportExportDlg.h"
#include "tdcstatic.h"

#include "..\shared\misc.h"
#include "..\shared\filemisc.h"
#include "..\shared\fileregister.h"

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
											   IPreferences* pPrefs, LPCTSTR szKey, CWnd* pParent /*=NULL*/)
	: CTDLDialog(IDD_CSVIMPORTEXPORT_DIALOG, pParent), 
	m_lcColumnSetup(TRUE), 
	m_eFilePath(FES_NOBROWSE)
{
	VERIFY(DoInit(sFilePath, pPrefs, szKey, NULL));
}

CTDLCsvImportExportDlg::CTDLCsvImportExportDlg(const CString& sFilePath, 
											   const CTDCAttributeArray& aExportAttributes, 
											   IPreferences* pPrefs, LPCTSTR szKey, CWnd* pParent /*=NULL*/)
	: CTDLDialog(IDD_CSVIMPORTEXPORT_DIALOG, pParent), 
	m_lcColumnSetup(FALSE), 
	m_eFilePath(FES_NOBROWSE)
{
	VERIFY(DoInit(sFilePath, pPrefs, szKey, &aExportAttributes));
}

BOOL CTDLCsvImportExportDlg::DoInit(const CString& sFilePath, 
									IPreferences* pPrefs, 
									LPCTSTR szKey, 
									const CTDCAttributeArray* pExportAttributes)
{
	m_sFilePath = sFilePath; 
	m_pPrefs = pPrefs;
	m_sPrefKey.Format(_T("%s\\CsvColumnMapping"), szKey);
	m_sDelim = Misc::GetListSeparator();
	m_bAlwaysExportTaskIDs = TRUE;
	m_bImporting = (pExportAttributes ? FALSE : TRUE);

	InitialiseDelimiter();
	LoadMasterColumnMapping();

	// user mapping
	CTDCAttributeMapping aMapping;

	if (m_bImporting)
	{
		BuildImportColumnMapping(aMapping);
	}
	else
	{
		m_aExportAttributes.Copy(*pExportAttributes);
		BuildExportColumnMapping(aMapping);
	}

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
	ON_BN_CLICKED(IDC_EXPORTTASKIDS, OnExportTaskids)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLCsvImportExportDlg message handlers

BOOL CTDLCsvImportExportDlg::OnInitDialog() 
{
	CTDLDialog::OnInitDialog();

	SetWindowText(CEnString(m_bImporting ? IDS_CSV_IMPORTDLG : IDS_CSV_EXPORTDLG));

	GetDlgItem(IDC_EXPORTTASKIDS)->EnableWindow(!m_bImporting);
	GetDlgItem(IDC_EXPORTTASKIDS)->ShowWindow(m_bImporting ? SW_HIDE : SW_SHOW);

	m_lcColumnSetup.SetFirstColumnStretchy(TRUE);

	if (!m_bImporting)
		OnExportTaskids();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTDLCsvImportExportDlg::InitialiseDelimiter()
{
	ASSERT(GetSafeHwnd() == NULL);

	// load last used delimiter
	CString sUIDelim = m_pPrefs->GetProfileString(m_sPrefKey, _T("Delimiter"));

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
				for (int nLine = 0; nLine < 5; nLine++)
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
	return m_lcColumnSetup.GetColumnMapping(aMapping);
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
		TDC_ATTRIBUTE attrib = ATTRIBUTES[nCol].attrib;
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

	for (int nLine = 0; nLine < 5; nLine++)
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

	m_bAlwaysExportTaskIDs = m_pPrefs->GetProfileInt(m_sPrefKey, _T("AlwaysExportTaskIDs"), TRUE);
	int nColumns = m_pPrefs->GetProfileInt(m_sPrefKey, _T("ColumnCount"), 0);

	// overwrite with translations unless they are empty names
	for (int nCol = 0; nCol < nColumns; nCol++)
	{
		CString sKey = Misc::MakeKey(_T("ColumnAttrib%d"), nCol);
		TDC_ATTRIBUTE attrib = (TDC_ATTRIBUTE)m_pPrefs->GetProfileInt(m_sPrefKey, sKey, TDCA_NONE);
		
		sKey = Misc::MakeKey(_T("ColumnName%d"), nCol);
		CString sName = m_pPrefs->GetProfileString(m_sPrefKey, sKey);
		
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
		SetMasterColumnName(aListMapping[nRow].nTDCAttrib, aListMapping[nRow].sColumnName);
	}
}

void CTDLCsvImportExportDlg::SaveMasterColumnMapping() const
{
	m_pPrefs->WriteProfileInt(m_sPrefKey, _T("AlwaysExportTaskIDs"), m_bAlwaysExportTaskIDs);

	int nColumns = m_aMasterColumnMapping.GetSize();
	m_pPrefs->WriteProfileInt(m_sPrefKey, _T("ColumnCount"), nColumns);

	for (int nCol = 0; nCol < nColumns; nCol++)
	{
		const TDCATTRIBUTEMAPPING& col = m_aMasterColumnMapping[nCol];

		CString sKey = Misc::MakeKey(_T("ColumnName%d"), nCol);
		m_pPrefs->WriteProfileString(m_sPrefKey, sKey, col.sColumnName);
		
		sKey = Misc::MakeKey(_T("ColumnAttrib%d"), nCol);
		m_pPrefs->WriteProfileInt(m_sPrefKey, sKey, col.nTDCAttrib);
	}

	// save delimiter if different to default
	if (m_sDelim == Misc::GetListSeparator())
		m_pPrefs->WriteProfileString(m_sPrefKey, _T("Delimiter"), _T(""));
	else
		m_pPrefs->WriteProfileString(m_sPrefKey, _T("Delimiter"), m_sDelim);
}

CString CTDLCsvImportExportDlg::GetMasterColumnName(TDC_ATTRIBUTE attrib) const
{
	int nCol = FindMasterColumn(attrib);
	return (nCol == -1) ? _T("") : m_aMasterColumnMapping[nCol].sColumnName;
}

TDC_ATTRIBUTE CTDLCsvImportExportDlg::GetMasterColumnAttribute(LPCTSTR szColumn) const
{
	int nCol = FindMasterColumn(szColumn);
	return (nCol == -1) ? TDCA_NONE : m_aMasterColumnMapping[nCol].nTDCAttrib;
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
	if (!Misc::IsEmpty(szColumn))
	{
		// check if the column name is already in use
		int nNameCol = FindMasterColumn(szColumn);
		int nAttribCol = FindMasterColumn(attrib);

		// and clear if it is
		if (nNameCol != -1 && nNameCol != nAttribCol)
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

void CTDLCsvImportExportDlg::OnExportTaskids() 
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

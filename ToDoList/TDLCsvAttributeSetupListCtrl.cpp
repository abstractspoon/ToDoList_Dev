// TDLCsvAttributeSetupListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLCsvAttributeSetupListCtrl.h"
#include "tdcstatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum { IMPORT_COLUMNNAME, IMPORT_COLUMNID };
enum { EXPORT_COLUMNID, EXPORT_COLUMNNAME };

enum { ATTRIB_ID = 5000 };
const int COL_WIDTH = 200;

/////////////////////////////////////////////////////////////////////////////

CSVCOLUMNMAPPING::CSVCOLUMNMAPPING() : nTDCAttrib(TDCA_NONE) 
{

}
	
CSVCOLUMNMAPPING::CSVCOLUMNMAPPING(const CString& sName, TDC_ATTRIBUTE tdcAttrib, DWORD dwData) 
{ 
	sColumnName = sName; 
	nTDCAttrib = tdcAttrib;
	dwItemData = dwData;
}

CSVCOLUMNMAPPING::CSVCOLUMNMAPPING(UINT nNameID, TDC_ATTRIBUTE tdcAttrib, DWORD dwData) 
{ 
	sColumnName.LoadString(nNameID); 
	nTDCAttrib = tdcAttrib;
	dwItemData = dwData;
}

/////////////////////////////////////////////////////////////////////////////

int CTDCCsvColumnMapping::Find(const CString& sCol) const
{
	int nMap = GetSize();
	
	while (nMap--)
	{
		if (GetData()[nMap].sColumnName.CompareNoCase(sCol) == 0)
			return nMap;
	}
	
	// not found
	return -1;
}

int CTDCCsvColumnMapping::Find(TDC_ATTRIBUTE nAttrib) const
{
	int nMap = GetSize();
	
	while (nMap--)
	{
		if (GetData()[nMap].nTDCAttrib == nAttrib)
			return nMap;
	}
	
	// not found
	return -1;
}

int CTDCCsvColumnMapping::FindMappedAttribute(TDC_ATTRIBUTE nAttrib) const
{
	int nMap = GetSize();
	
	while (nMap--)
	{
		const CSVCOLUMNMAPPING& col = GetData()[nMap];

		if ((col.nTDCAttrib == nAttrib) && !col.sColumnName.IsEmpty())
			return nMap;
	}
	
	// not found or mapped
	return -1;
}

BOOL CTDCCsvColumnMapping::IsAttributeMapped(TDC_ATTRIBUTE nAttrib) const
{
	return (FindMappedAttribute(nAttrib) != -1);
}

/////////////////////////////////////////////////////////////////////////////
// CTDLCsvAttributeSetupListCtrl

CTDLCsvAttributeSetupListCtrl::CTDLCsvAttributeSetupListCtrl(BOOL bImporting, BOOL bOneToOneMapping)
 : m_bImporting(bImporting), m_bOneToOneMapping(bOneToOneMapping)
{
}

CTDLCsvAttributeSetupListCtrl::~CTDLCsvAttributeSetupListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTDLCsvAttributeSetupListCtrl, CInputListCtrl)
	//{{AFX_MSG_MAP(CTDLCsvAttributeSetupListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_CBN_CLOSEUP(ATTRIB_ID, OnAttribEditCancel)
	ON_CBN_SELENDCANCEL(ATTRIB_ID, OnAttribEditCancel)
	ON_CBN_SELENDOK(ATTRIB_ID, OnAttribEditOK)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnNameEditOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLCsvAttributeSetupListCtrl message handlers

void CTDLCsvAttributeSetupListCtrl::SetColumnMapping(const CTDCCsvColumnMapping& aMapping)
{
	m_aMapping.Copy(aMapping);

	if (GetSafeHwnd())
		BuildListCtrl();
}

int CTDLCsvAttributeSetupListCtrl::GetColumnMapping(CTDCCsvColumnMapping& aMapping) const
{
	aMapping.Copy(m_aMapping);
	return aMapping.GetSize();
}

void CTDLCsvAttributeSetupListCtrl::PreSubclassWindow() 
{
	// create child controls 
	// we need combo to be created first
	CreateControl(m_cbAttributes, ATTRIB_ID);

	CInputListCtrl::PreSubclassWindow();

	// build column combo because that is static
	for (int nAttrib = 0; nAttrib < ATTRIB_COUNT; nAttrib++)
	{
		const TDCATTRIBUTE& att = ATTRIBUTES[nAttrib];

		// ignore special attributes
		if (att.attrib == TDCA_NONE || att.attrib == TDCA_COLOR || att.attrib == TDCA_PROJNAME ||
			att.attrib == TDCA_RECURRENCE || att.attrib == TDCA_POSITION||
			att.attrib == TDCA_TASKNAMEORCOMMENTS || att.attrib == TDCA_ANYTEXTATTRIBUTE)
			continue;

		int nItem = m_cbAttributes.AddString(CEnString(att.nAttribResID)); 
		m_cbAttributes.SetItemData(nItem, (DWORD)att.attrib); 
	}

	// add custom attribute placeholder if importing
	if (m_bImporting)
	{
		int nItem = m_cbAttributes.AddString(CEnString(IDS_CSV_CUSTOMATTRIB)); 
		m_cbAttributes.SetItemData(nItem, (DWORD)TDCA_CUSTOMATTRIB_FIRST); 

		nItem = m_cbAttributes.AddString(CEnString(IDS_CSV_CUSTOMLISTATTRIB)); 
		m_cbAttributes.SetItemData(nItem, (DWORD)TDCA_CUSTOMATTRIB_LAST); 
	}

	m_header.EnableTracking(FALSE);

	if (m_bImporting)
	{
		InsertColumn(IMPORT_COLUMNNAME, CEnString(IDS_CSV_COLUMNNAME), LVCFMT_LEFT, COL_WIDTH);
		InsertColumn(IMPORT_COLUMNID, CEnString(IDS_CSV_MAPSTOATTRIBUTE), LVCFMT_LEFT, COL_WIDTH);

		SetColumnType(IMPORT_COLUMNNAME, ILCT_TEXT);
		DisableColumnEditing(IMPORT_COLUMNNAME, TRUE);
		SetColumnType(IMPORT_COLUMNID, ILCT_DROPLIST);
	}
	else // export
	{
		InsertColumn(EXPORT_COLUMNID, CEnString(IDS_CSV_ATTRIBUTE), LVCFMT_LEFT, COL_WIDTH);
		InsertColumn(EXPORT_COLUMNNAME, CEnString(IDS_CSV_MAPSTOCOLUMNNAME), LVCFMT_LEFT, COL_WIDTH);

		SetColumnType(EXPORT_COLUMNID, ILCT_TEXT);
		DisableColumnEditing(EXPORT_COLUMNID, TRUE);
		SetColumnType(EXPORT_COLUMNNAME, ILCT_TEXT);
	}
	SetView(LVS_REPORT);

	BuildListCtrl();
}

void CTDLCsvAttributeSetupListCtrl::InitState()
{
	CInputListCtrl::InitState();
	
	AutoAdd(FALSE, FALSE);
	ShowGrid(TRUE, TRUE);
}

void CTDLCsvAttributeSetupListCtrl::BuildListCtrl()
{
	DeleteAllItems();

	if (m_bImporting)
	{
		for (int nRow = 0; nRow < m_aMapping.GetSize(); nRow++)
		{
			CSVCOLUMNMAPPING& col = m_aMapping[nRow];

			if (!col.sColumnName.IsEmpty())
			{
				int nItem = InsertItem(GetItemCount(), col.sColumnName);
				SetItemText(nItem, IMPORT_COLUMNID, GetAttributeName(col.nTDCAttrib));
				SetItemData(nItem, col.nTDCAttrib);
			}
		}
	}
	else // export
	{
		for (int nRow = 0; nRow < m_aMapping.GetSize(); nRow++)
		{
			CSVCOLUMNMAPPING& col = m_aMapping[nRow];
			
			if (!col.sColumnName.IsEmpty())
			{
				int nItem = InsertItem(GetItemCount(), GetAttributeName(col.nTDCAttrib));
				SetItemText(nItem, EXPORT_COLUMNNAME, col.sColumnName);
				SetItemData(nItem, col.nTDCAttrib);
			}
		}
	}

	CRect rCombo;
	m_cbAttributes.GetWindowRect(rCombo);
	
	SetMinItemHeight(rCombo.Height());

	if (GetItemCount())
		SetCurSel(0);
}

// static helper
CString CTDLCsvAttributeSetupListCtrl::GetAttributeName(TDC_ATTRIBUTE nAtt)
{
	for (int nAttrib = 0; nAttrib < ATTRIB_COUNT; nAttrib++)
	{
		const TDCATTRIBUTE& att = ATTRIBUTES[nAttrib];

		if (nAtt == att.attrib)
			return CEnString(att.nAttribResID);
	}

	return _T("");
}

int CTDLCsvAttributeSetupListCtrl::FindRow(TDC_ATTRIBUTE nAtt, int nIgnoreRow) const
{
	for (int nRow = 0; nRow < GetItemCount(); nRow++)
	{
		if (nRow == nIgnoreRow)
			continue;

		if (nAtt == (TDC_ATTRIBUTE)GetItemData(nRow))
			return nRow;
	}

	return -1;
}

int CTDLCsvAttributeSetupListCtrl::FindRow(const CString& sName, int nIgnoreRow) const
{
	for (int nRow = 0; nRow < GetItemCount(); nRow++)
	{
		if (nRow == nIgnoreRow)
			continue;

		if (m_bImporting)
		{
			if (GetItemText(nRow, IMPORT_COLUMNNAME).CompareNoCase(sName) == 0)
				return nRow;
		}
		else // exporting
		{
			if (GetItemText(nRow, EXPORT_COLUMNNAME).CompareNoCase(sName) == 0)
				return nRow;
		}
	}

	return -1;
}

void CTDLCsvAttributeSetupListCtrl::EditCell(int nItem, int nCol)
{
	if ((m_bImporting && nCol == IMPORT_COLUMNID) ||
		(!m_bImporting && nCol == EXPORT_COLUMNID))
	{
		PrepareEdit(nItem, nCol);
		ShowControl(m_cbAttributes, nItem, nCol); 
	}
	else
		CInputListCtrl::EditCell(nItem, nCol);
}

BOOL CTDLCsvAttributeSetupListCtrl::IsEditing() const 
{
	return CInputListCtrl::IsEditing() ||
			m_cbAttributes.IsWindowVisible();
}

BOOL CTDLCsvAttributeSetupListCtrl::CanEditSelectedCell() const 
{
	return CInputListCtrl::CanEditSelectedCell();
}

void CTDLCsvAttributeSetupListCtrl::PrepareEdit(int nRow, int nCol)
{
	if (m_bImporting && nCol == IMPORT_COLUMNID)
	{
		TDC_ATTRIBUTE nAttrib = (TDC_ATTRIBUTE)GetItemData(nRow);
		CString sAttrib = GetAttributeName(nAttrib);

		int nFind = m_cbAttributes.FindStringExact(0, sAttrib);

		if (nFind == -1)
			nFind = 0;

		m_cbAttributes.SetCurSel(nFind);
	}		
}

BOOL CTDLCsvAttributeSetupListCtrl::CanDeleteSelectedCell() const
{
	int nRow, nCol;
	GetCurSel(nRow, nCol);

	return ((m_bImporting && nCol == IMPORT_COLUMNID) ||
			(!m_bImporting && nCol == EXPORT_COLUMNNAME));
}

BOOL CTDLCsvAttributeSetupListCtrl::DeleteSelectedCell()
{
	int nRow, nCol;
	GetCurSel(nRow, nCol);

	if ((m_bImporting && nCol == IMPORT_COLUMNID) ||
		(!m_bImporting && nCol == EXPORT_COLUMNNAME))
	{
		if (m_bImporting)
		{
			m_aMapping[nRow].nTDCAttrib = TDCA_NONE;
			SetItemData(nRow, (DWORD)TDCA_NONE);
		}
		else // exporting
			m_aMapping[nRow].sColumnName.Empty();

		return CInputListCtrl::DeleteSelectedCell();
	}
	
	// else
	return FALSE;
}

void CTDLCsvAttributeSetupListCtrl::OnAttribEditCancel()
{
	HideControl(m_cbAttributes);
}

void CTDLCsvAttributeSetupListCtrl::OnAttribEditOK()
{
	ASSERT (m_bImporting);

	HideControl(m_cbAttributes);

	// update attribute type
	int nSel = m_cbAttributes.GetCurSel();

	if (nSel != CB_ERR)
	{
		CString sSel;

		TDC_ATTRIBUTE nAttrib = (TDC_ATTRIBUTE)m_cbAttributes.GetItemData(nSel);
		m_cbAttributes.GetLBText(nSel, sSel);

		int nRow = GetCurSel();
		ASSERT(nRow != -1);

		// check that this column ID is not already in use
		if (m_bOneToOneMapping && 
			(nAttrib != TDCA_CUSTOMATTRIB_FIRST) && 
			(nAttrib != TDCA_CUSTOMATTRIB_LAST))
		{
			int nExist = FindRow(nAttrib, nRow);

			if (nExist != -1)
			{
				// update mapping
				TDC_ATTRIBUTE nExistAttrib = (TDC_ATTRIBUTE)GetItemData(nExist);

				int nExistMap = m_aMapping.Find(nExistAttrib);
				ASSERT(nExistMap != -1);

				m_aMapping[nExistMap].nTDCAttrib = TDCA_NONE;

				// clear field
				SetItemText(nExist, IMPORT_COLUMNID, _T(""));
				SetItemData(nExist, (DWORD)TDCA_NONE);

			}
		}

		// update list
		SetItemText(nRow, IMPORT_COLUMNID, sSel);
		SetItemData(nRow, nAttrib);

		// update mapping
		CString sMap = GetItemText(nRow, IMPORT_COLUMNNAME);
	
		int nMap = m_aMapping.Find(sMap);
		ASSERT(nMap != -1);

		m_aMapping[nMap].nTDCAttrib = nAttrib;
	}
}

void CTDLCsvAttributeSetupListCtrl::TraceMapping() const
{
	for (int nAtt = 0; nAtt < m_aMapping.GetSize(); nAtt++)
	{
		const CSVCOLUMNMAPPING& col = m_aMapping[nAtt];

		TRACE (_T("'%s' maps to '%d'\n"), col.sColumnName, col.nTDCAttrib);
	}
}

void CTDLCsvAttributeSetupListCtrl::OnNameEditOK(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	ASSERT (!m_bImporting);

	// update column name
	int nRow = GetCurSel(); // list selection
	CString sSel = GetItemText(nRow, EXPORT_COLUMNNAME);

	// check that this column name is not already in use
	if (m_bOneToOneMapping)
	{
		int nExist = FindRow(sSel, nRow);
		
		if (nExist != -1)
		{
			// clear it
			SetItemText(nExist, EXPORT_COLUMNNAME, _T(""));

			// update mapping
			TDC_ATTRIBUTE nExistAttrib = (TDC_ATTRIBUTE)GetItemData(nExist);

			int nExistMap = m_aMapping.Find(nExistAttrib);
			ASSERT(nExistMap != -1);

			m_aMapping[nExistMap].sColumnName.Empty();
		}
	}	

	// update list
	SetItemText(nRow, EXPORT_COLUMNNAME, sSel);

	// update mapping
	TDC_ATTRIBUTE nAttrib = (TDC_ATTRIBUTE)GetItemData(nRow);

	int nMap = m_aMapping.Find(nAttrib);
	ASSERT(nMap != -1);
	
	m_aMapping[nMap].sColumnName = sSel;

	*pResult = 0;
}


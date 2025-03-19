// TDLCsvAttributeSetupListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TDLImpExpAttributeMappingListCtrl.h"
#include "tdcstatic.h"

#include "..\shared\Localizer.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\holdredraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum { IMPORT_COLUMNNAME, IMPORT_COLUMNID };
enum { EXPORT_COLUMNID, EXPORT_COLUMNNAME };

enum { ATTRIB_ID = 5000 };

/////////////////////////////////////////////////////////////////////////////
// CTDLCsvAttributeSetupListCtrl

CTDLImportExportAttributeMappingListCtrl::CTDLImportExportAttributeMappingListCtrl(BOOL bImporting, BOOL bOneToOneMapping)
 : m_bImporting(bImporting), m_bOneToOneMapping(bOneToOneMapping)
{
	SetFirstColumnStretchy(TRUE);
}

CTDLImportExportAttributeMappingListCtrl::~CTDLImportExportAttributeMappingListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTDLImportExportAttributeMappingListCtrl, CInputListCtrl)
	//{{AFX_MSG_MAP(CTDLCsvAttributeSetupListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_CBN_CLOSEUP(ATTRIB_ID, OnAttribEditCancel)
	ON_CBN_SELENDCANCEL(ATTRIB_ID, OnAttribEditCancel)
	ON_CBN_SELENDOK(ATTRIB_ID, OnAttribEditOK)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnNameEditOK)
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLCsvAttributeSetupListCtrl message handlers

void CTDLImportExportAttributeMappingListCtrl::SetColumnMapping(const CTDCAttributeMapping& aMapping)
{
	m_aMapping.Copy(aMapping);

	if (GetSafeHwnd())
		BuildListCtrl();
}

int CTDLImportExportAttributeMappingListCtrl::GetColumnMapping(CTDCAttributeMapping& aMapping) const
{
	aMapping.Copy(m_aMapping);
	return aMapping.GetSize();
}

void CTDLImportExportAttributeMappingListCtrl::PreSubclassWindow() 
{
	CInputListCtrl::PreSubclassWindow();

	// build column combo because that is static
	CLocalizer::EnableTranslation(m_cbAttributes, FALSE);

	for (int nAtt = 0; nAtt < ATTRIB_COUNT; nAtt++)
	{
		const TDCATTRIBUTE& att = TASKATTRIBUTES[nAtt];

		// ignore certain attributes
		switch (att.nAttributeID)
		{
		case TDCA_COLOR:
		case TDCA_RECURRENCE:
		case TDCA_POSITION:
		case TDCA_TASKNAMEORCOMMENTS:
		case TDCA_ANYTEXTATTRIBUTE:
		case TDCA_COMMENTSSIZE:
		case TDCA_COMMENTSFORMAT:
			continue;

		case TDCA_NONE:
			// Allow mapping to 'none' when importing
			if (m_bImporting)
				CDialogHelper::AddStringT(m_cbAttributes, _T(""), att.nAttributeID);
			break;

		default:
			CDialogHelper::AddStringT(m_cbAttributes, CEnString(att.nLabelResID), att.nAttributeID);
			break;
		}
	}

	// add custom attribute placeholder if importing
	if (m_bImporting)
	{
		CDialogHelper::AddStringT(m_cbAttributes, CEnString(IDS_CSV_CUSTOMATTRIB), TDCA_NEW_CUSTOMATTRIBUTE);
		CDialogHelper::AddStringT(m_cbAttributes, CEnString(IDS_CSV_CUSTOMLISTATTRIB), TDCA_NEW_CUSTOMATTRIBUTE_LIST);
	}

	m_header.EnableTracking(FALSE);

	if (m_bImporting)
	{
		InsertColumn(IMPORT_COLUMNNAME, CEnString(IDS_CSV_COLUMNNAME));
		InsertColumn(IMPORT_COLUMNID, CEnString(IDS_CSV_MAPSTOATTRIBUTE));

		SetColumnType(IMPORT_COLUMNNAME, ILCT_TEXT);
		EnableColumnEditing(IMPORT_COLUMNNAME, FALSE);
		SetColumnType(IMPORT_COLUMNID, ILCT_COMBO);
	}
	else // export
	{
		InsertColumn(EXPORT_COLUMNID, CEnString(IDS_CSV_ATTRIBUTE));
		InsertColumn(EXPORT_COLUMNNAME, CEnString(IDS_CSV_MAPSTOCOLUMNNAME));

		SetColumnType(EXPORT_COLUMNID, ILCT_TEXT);
		EnableColumnEditing(EXPORT_COLUMNID, FALSE);
		SetColumnType(EXPORT_COLUMNNAME, ILCT_TEXT);
	}

	BuildListCtrl();
	ResizeColumns();
}

void CTDLImportExportAttributeMappingListCtrl::InitState()
{
	CInputListCtrl::InitState();
	
	// create child controls 
	// we need combo to be created first
	CreateControl(m_cbAttributes, ATTRIB_ID);

	AutoAdd(FALSE, FALSE);
	ShowGrid(TRUE, TRUE);
}

void CTDLImportExportAttributeMappingListCtrl::BuildListCtrl()
{
	CLockUpdates hr(*this);
	CWaitCursor cursor;

	DeleteAllItems();

	for (int nRow = 0; nRow < m_aMapping.GetSize(); nRow++)
	{
		const TDCATTRIBUTEMAPPING& col = m_aMapping.GetData()[nRow];

		if (!col.sColumnName.IsEmpty())
		{
			CString sAttribName(GetAttributeName(col.nAttributeID));
			CString sFrom(m_bImporting ? col.sColumnName : sAttribName);
			CString sTo(m_bImporting ? sAttribName : col.sColumnName);

			int nItem = AddRow(sFrom);

			SetItemText(nItem, IMPORT_COLUMNID, sTo);
			SetItemData(nItem, col.nAttributeID);
		}
	}

	if (GetItemCount())
		SetCurSel(0);
}

// static helper
CString CTDLImportExportAttributeMappingListCtrl::GetAttributeName(TDC_ATTRIBUTE nAtt)
{
	// Special case:
	if (nAtt == TDCA_EXISTING_CUSTOMATTRIBUTE)
		return CEnString(IDS_CSV_EXISTCUSTOMATTRIB);
	
	return TDC::GetAttributeLabel(nAtt);
}

int CTDLImportExportAttributeMappingListCtrl::FindRow(TDC_ATTRIBUTE nAtt, int nIgnoreRow) const
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

int CTDLImportExportAttributeMappingListCtrl::FindRow(const CString& sName, int nIgnoreRow) const
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

void CTDLImportExportAttributeMappingListCtrl::EditCell(int nItem, int nCol, BOOL bBtnClick)
{
	if ((m_bImporting && nCol == IMPORT_COLUMNID) ||
		(!m_bImporting && nCol == EXPORT_COLUMNID))
	{
		PrepareEdit(nItem, nCol);
		ShowControl(m_cbAttributes, nItem, nCol, bBtnClick);
	}
	else
	{
		CInputListCtrl::EditCell(nItem, nCol, bBtnClick);
	}
}

BOOL CTDLImportExportAttributeMappingListCtrl::IsEditing() const 
{
	return CInputListCtrl::IsEditing() ||
			m_cbAttributes.IsWindowVisible();
}

void CTDLImportExportAttributeMappingListCtrl::PrepareEdit(int nRow, int nCol)
{
	if (m_bImporting && nCol == IMPORT_COLUMNID)
	{
		TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)GetItemData(nRow);
		CString sAttrib = GetAttributeName(nAttribID);

		int nFind = m_cbAttributes.FindStringExact(0, sAttrib);

		if (nFind == -1)
			nFind = 0;

		m_cbAttributes.SetCurSel(nFind);
	}		
}

BOOL CTDLImportExportAttributeMappingListCtrl::CanDeleteSelectedCell() const
{
	int nRow, nCol;
	GetCurSel(nRow, nCol);

	return ((m_bImporting && nCol == IMPORT_COLUMNID) ||
			(!m_bImporting && nCol == EXPORT_COLUMNNAME));
}

BOOL CTDLImportExportAttributeMappingListCtrl::DeleteSelectedCell()
{
	int nRow, nCol;
	GetCurSel(nRow, nCol);

	if ((m_bImporting && nCol == IMPORT_COLUMNID) ||
		(!m_bImporting && nCol == EXPORT_COLUMNNAME))
	{
		TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)GetItemData(nRow);

		int nMapping = m_aMapping.Find(nAttribID);
		ASSERT(nMapping != -1);

		TDCATTRIBUTEMAPPING& attrib = m_aMapping[nMapping];

		if (m_bImporting)
		{
			attrib.nAttributeID = TDCA_NONE;
			SetItemData(nRow, (DWORD)TDCA_NONE);
		}
		else // exporting
		{
			attrib.sColumnName.Empty();
		}

		return CInputListCtrl::DeleteSelectedCell();
	}
	
	// else
	return FALSE;
}

void CTDLImportExportAttributeMappingListCtrl::OnAttribEditCancel()
{
	HideControl(m_cbAttributes);
}

void CTDLImportExportAttributeMappingListCtrl::OnAttribEditOK()
{
	ASSERT (m_bImporting);

	HideControl(m_cbAttributes);

	// update attribute type
	int nSel = m_cbAttributes.GetCurSel();

	if (nSel != CB_ERR)
	{
		CString sSel;

		TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)m_cbAttributes.GetItemData(nSel);
		m_cbAttributes.GetLBText(nSel, sSel);

		int nRow = GetCurSel();
		ASSERT(nRow != -1);

		// check that this column ID is not already in use
		if (m_bOneToOneMapping && 
			(nAttribID != TDCA_CUSTOMATTRIB_FIRST) && 
			(nAttribID != TDCA_CUSTOMATTRIB_LAST) &&
			(nAttribID != TDCA_NONE))
		{
			int nExist = FindRow(nAttribID, nRow);

			if (nExist != -1)
			{
				// update mapping
				TDC_ATTRIBUTE nExistAttribID = (TDC_ATTRIBUTE)GetItemData(nExist);

				int nExistMap = m_aMapping.Find(nExistAttribID);
				ASSERT(nExistMap != -1);

				m_aMapping[nExistMap].nAttributeID = TDCA_NONE;

				// clear field
				SetItemText(nExist, IMPORT_COLUMNID, _T(""));
				SetItemData(nExist, (DWORD)TDCA_NONE);
			}
		}

		// update list
		SetItemText(nRow, IMPORT_COLUMNID, sSel);
		SetItemData(nRow, nAttribID);

		// update mapping
		CString sMap = GetItemText(nRow, IMPORT_COLUMNNAME);
	
		int nMap = m_aMapping.Find(sMap);
		ASSERT(nMap != -1);

		m_aMapping[nMap].nAttributeID = nAttribID;

		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), TDCN_IMPORTMAPPINGCHANGE), (LPARAM)GetSafeHwnd());
	}
}

void CTDLImportExportAttributeMappingListCtrl::TraceMapping() const
{
	for (int nAtt = 0; nAtt < m_aMapping.GetSize(); nAtt++)
	{
		const TDCATTRIBUTEMAPPING& col = m_aMapping[nAtt];

		TRACE (_T("'%s' maps to '%d'\n"), col.sColumnName, col.nAttributeID);
	}
}

void CTDLImportExportAttributeMappingListCtrl::OnNameEditOK(NMHDR* /*pNMHDR*/, LRESULT* pResult)
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
	TDC_ATTRIBUTE nAttribID = (TDC_ATTRIBUTE)GetItemData(nRow);

	int nMap = m_aMapping.Find(nAttribID);
	ASSERT(nMap != -1);
	
	m_aMapping[nMap].sColumnName = sSel;

	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), TDCN_EXPORTMAPPINGCHANGE), (LPARAM)GetSafeHwnd());

	*pResult = 0;
}

void CTDLImportExportAttributeMappingListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CInputListCtrl::OnSize(nType, cx, cy);

	ResizeColumns();
}

void CTDLImportExportAttributeMappingListCtrl::ResizeColumns()
{
	CRect rClient;
	GetClientRect(rClient);

	// Size the columns equally
	SetColumnWidth(0, rClient.Width() / 2);
	SetColumnWidth(1, rClient.Width() / 2);
}

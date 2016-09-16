// TDCCustomAttributeHelper.cpp: implementation of the CTDCCustomAttributeHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCCustomAttributeHelper.h"
#include "TDCstruct.h"
#include "tdliconcombobox.h"

#include "..\shared\DialogHelper.h"
#include "..\shared\checkcombobox.h"
#include "..\shared\autocombobox.h"
#include "..\shared\wclassdefines.h"
#include "..\shared\maskedit.h"
#include "..\shared\orderedtreectrl.h"
#include "..\shared\datetimectrlex.h"
#include "..\shared\misc.h"
#include "..\shared\localizer.h"
#include "..\shared\timecombobox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWnd* CTDCCustomAttributeHelper::CreateCustomAttribute(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, 
													   const CTDCImageList& ilImages,
													   CWnd* pParent, UINT nCtrlID, 
													   BOOL bVisible, BOOL bBuddy)
{
	// Sanity check
	if (bBuddy && !AttributeWantsBuddy(attribDef))
	{
		ASSERT(0);
		return NULL;
	}
	
	DWORD dwDataType = attribDef.GetDataType();
	DWORD dwListType = attribDef.GetListType();

	CWnd* pControl = NULL;
	DWORD dwStyle = (WS_CHILD | WS_TABSTOP | (bVisible ? WS_VISIBLE : 0));
	DWORD dwExStyle = WS_EX_CLIENTEDGE;
	LPCTSTR szClass = NULL;

	switch (dwListType)
	{
	case TDCCA_NOTALIST:
		{
			switch (dwDataType)
			{
			case TDCCA_STRING:
				pControl = new CEdit;
				szClass = WC_EDIT;
				dwStyle |= ES_LEFT | ES_AUTOHSCROLL;
				break;
				
			case TDCCA_DATE:
				if (bBuddy)
				{
					pControl = new CTimeComboBox(TCB_HALFHOURS | TCB_NOTIME);
					szClass = WC_COMBOBOX;
					dwStyle |= CBS_DROPDOWN | WS_VSCROLL | CBS_AUTOHSCROLL;
				}
				else
				{
					pControl = new CDateTimeCtrlEx;
					szClass = WC_DATETIMEPICK;
					dwStyle |= DTS_SHORTDATEFORMAT | DTS_RIGHTALIGN | DTS_SHOWNONE;
				}
				break;
				
			case TDCCA_INTEGER:
				pControl = new CMaskEdit(_T("0123456789"));
				szClass = WC_EDIT;
				dwStyle |= ES_LEFT | ES_AUTOHSCROLL;
				break;
				
			case TDCCA_DOUBLE:
				pControl = new CMaskEdit(_T(".0123456789"), ME_LOCALIZEDECIMAL);
				szClass = WC_EDIT;
				dwStyle |= ES_LEFT | ES_AUTOHSCROLL;
				break;
		
			// these don't have controls
			case TDCCA_BOOL:
			case TDCCA_ICON:
				break;
			}
		}
		break;
		
	case TDCCA_AUTOLIST:
		pControl = new CAutoComboBox(ACBS_ALLOWDELETE);
		szClass = WC_COMBOBOX;
		dwStyle |= CBS_DROPDOWN | CBS_SORT | WS_VSCROLL | CBS_AUTOHSCROLL;

		// add number mask as required
		switch (dwDataType)
		{
		case TDCCA_INTEGER:
			((CAutoComboBox*)pControl)->SetEditMask(_T("0123456789"));
			break;
			
		case TDCCA_DOUBLE:
			((CAutoComboBox*)pControl)->SetEditMask(_T(".0123456789"), ME_LOCALIZEDECIMAL);
			break;
		}
		break;
		
	case TDCCA_FIXEDLIST:
		szClass = WC_COMBOBOX;
		dwStyle |= CBS_DROPDOWNLIST | WS_VSCROLL | CBS_AUTOHSCROLL;

		switch (dwDataType)
		{
		case TDCCA_ICON:
			pControl = new CTDLIconComboBox(ilImages, FALSE);
			break;
			
		default:
			pControl = new COwnerdrawComboBoxBase; // so they render the same
			break;
		}
		break;
		
	case TDCCA_AUTOMULTILIST:
		pControl = new CCheckComboBox(ACBS_ALLOWDELETE);
		szClass = WC_COMBOBOX;
		dwStyle |= CBS_DROPDOWN | CBS_SORT | WS_VSCROLL | CBS_AUTOHSCROLL;

		// add number mask as required
		switch (dwDataType)
		{
		case TDCCA_INTEGER:
			((CAutoComboBox*)pControl)->SetEditMask(_T("0123456789"));
			break;
			
		case TDCCA_DOUBLE:
			((CAutoComboBox*)pControl)->SetEditMask(_T(".0123456789"), ME_LOCALIZEDECIMAL);
			break;
		}
		break;
		
	case TDCCA_FIXEDMULTILIST:
		szClass = WC_COMBOBOX;
		dwStyle |= CBS_DROPDOWNLIST | WS_VSCROLL | CBS_AUTOHSCROLL;

		switch (dwDataType)
		{
		case TDCCA_ICON:
			pControl = new CTDLIconComboBox(ilImages, TRUE);
			break;
			
		default:
			pControl = new CCheckComboBox;
			break;
		}
		break;
	}

	ASSERT (pControl);

	if (pControl)
	{
		if (!pControl->CreateEx(dwExStyle, 
								szClass, 
								NULL, 
								dwStyle, 
								CRect(0, 0, 0, 0), 
								pParent, 
								nCtrlID))
		{
			delete pControl;
			pControl = NULL;
		}
		else 
		{
			// content is already in user's language
			CLocalizer::EnableTranslation(*pControl, FALSE);

			// font
			pControl->SendMessage(WM_SETFONT, (WPARAM)CDialogHelper::GetFont(pParent)); 

			// add default data to lists
			if (attribDef.IsList()) 
			{
				ASSERT(pControl->IsKindOf(RUNTIME_CLASS(CComboBox)));
				CComboBox* pCB = (CComboBox*)pControl;

				// build a combined list of auto-data and default-data
				CStringArray aListData;

				if (attribDef.GetUniqueListData(aListData))
					CDialogHelper::SetComboBoxItems(*pCB, aListData);
				
				// prepend empty item to single item lists
				switch (attribDef.GetListType())
				{
				case TDCCA_FIXEDLIST:
				case TDCCA_AUTOLIST:
					pCB->InsertString(0, _T(""));
					break;
				}
			}
		}
	}

	return pControl;
}

BOOL CTDCCustomAttributeHelper::AttributeWantsBuddy(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef)
{
	switch (attribDef.GetDataType())
	{
	case TDCCA_STRING:
	case TDCCA_INTEGER:
	case TDCCA_DOUBLE:
	case TDCCA_BOOL:
	case TDCCA_ICON:
		return FALSE;
		
	case TDCCA_DATE:
		return attribDef.HasFeature(TDCCAF_SHOWTIME);
	}

	ASSERT(0);
	return FALSE;
}

CString CTDCCustomAttributeHelper::GetControlLabel(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bBuddy)
{
	if (!bBuddy)
	{
		return (attribDef.sLabel.IsEmpty() ? attribDef.sColumnTitle : attribDef.sLabel);
	}
	else if (AttributeWantsBuddy(attribDef))
	{
		switch (attribDef.GetDataType())
		{
		case TDCCA_STRING:
		case TDCCA_INTEGER:
		case TDCCA_DOUBLE:
		case TDCCA_BOOL:
		case TDCCA_ICON:
			return _T("");
			
		case TDCCA_DATE:
			return CEnString(_T("Time"));
		}
	}

	ASSERT(0);
	return _T("");
}

CWnd* CTDCCustomAttributeHelper::CreateCustomAttributeLabel(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, 
															CWnd* pParent, UINT nCtrlID, 
															BOOL bVisible, BOOL bBuddy)
{
	CStatic* pLabel = new CStatic;
	ASSERT (pLabel);

	if (pLabel)
	{
		DWORD dwStyle = (WS_CHILD | (bVisible ? WS_VISIBLE : 0) | SS_CENTERIMAGE);

		if (!pLabel->Create(NULL, dwStyle, CRect(0, 0, 0, 0), pParent, nCtrlID))
		{
			delete pLabel;
			pLabel = NULL;
		}
		else // font
		{
			// label is already in user's language
			CLocalizer::EnableTranslation(*pLabel, FALSE);
			
			// THEN set label
			pLabel->SetWindowText(GetControlLabel(attribDef, bBuddy));

			// then font			
			pLabel->SendMessage(WM_SETFONT, (WPARAM)CDialogHelper::GetFont(pParent), 0); 
		}
	}

	return pLabel;
}

BOOL CTDCCustomAttributeHelper::GetControl(UINT nCtrlID, const CTDCCustomControlArray& aControls, 
										   CUSTOMATTRIBCTRLITEM& ctrl)
{
	// sanity check
	if (nCtrlID == 0)
	{
		ASSERT(0);
		return FALSE;
	}

	int nCtrl = aControls.GetSize();

	while (nCtrl--)
	{
		const CUSTOMATTRIBCTRLITEM& ctrlCustom = aControls.GetData()[nCtrl];

		if ((ctrlCustom.nCtrlID == nCtrlID) ||
			(ctrlCustom.nBuddyCtrlID == nCtrlID))
		{
			ctrl = ctrlCustom;
			return TRUE;
		}
	}

	// not found 
	return FALSE;
}

BOOL CTDCCustomAttributeHelper::GetControl(const CString& sUniqueID, const CTDCCustomControlArray& aControls, 
										   CUSTOMATTRIBCTRLITEM& ctrl)
{
	int nCtrl = aControls.GetSize();

	while (nCtrl--)
	{
		const CUSTOMATTRIBCTRLITEM& ctrlCustom = aControls.GetData()[nCtrl];

		if (ctrlCustom.sAttribID == sUniqueID)
		{
			ctrl = ctrlCustom;
			return TRUE;
		}
	}

	// not found 
	return FALSE;
}

void CTDCCustomAttributeHelper::CleanupCustomAttributeUI(CTDCCustomControlArray& aControls, CWnd* pParent)
{
	for (int nCtrl = 0; nCtrl < aControls.GetSize(); nCtrl++)
	{
		const CUSTOMATTRIBCTRLITEM& ctrl = aControls.GetData()[nCtrl];
		
		CWnd* pCtrl = pParent->GetDlgItem(ctrl.nCtrlID);
		
		if (pCtrl)
		{
			pCtrl->DestroyWindow();
			delete pCtrl;
		}
		
		CWnd* pLabel = pParent->GetDlgItem(ctrl.nLabelID);
		
		if (pLabel)
		{
			pLabel->DestroyWindow();
			delete pLabel;
		}

		// Buddy controls
		if (ctrl.nBuddyCtrlID)
		{
			CWnd* pCtrl = pParent->GetDlgItem(ctrl.nBuddyCtrlID);
			
			if (pCtrl)
			{
				pCtrl->DestroyWindow();
				delete pCtrl;
			}
			
			CWnd* pLabel = pParent->GetDlgItem(ctrl.nBuddyLabelID);
			
			if (pLabel)
			{
				pLabel->DestroyWindow();
				delete pLabel;
			}
		}
	}
	
	aControls.RemoveAll();
}

BOOL CTDCCustomAttributeHelper::RebuildCustomAttributeUI(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
														 CTDCCustomControlArray& aControls, 
														 const CTDCImageList& ilImages,
														 CWnd* pParent, UINT nCtrlIDPos)
{
	ASSERT_VALID(pParent);

	// remove all existing custom attribute fields
	CleanupCustomAttributeUI(aControls, pParent);

	// recreate controls and columns
	UINT nID = IDC_FIRST_CUSTOMDATAFIELD;

	const CWnd* pInsertAfter = pParent->GetDlgItem(nCtrlIDPos);
	ASSERT(pInsertAfter);

	for (int nAttrib = 0; nAttrib < aAttribDefs.GetSize(); nAttrib++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs.GetData()[nAttrib];

		// don't add disabled controls
		if (!attribDef.bEnabled)
			continue;

		// NOTE: flag and image types don't need controls because they are 
		// handled by clicking the tasklist directly
		switch (attribDef.GetDataType())
		{
		case TDCCA_BOOL:
			break;

		case TDCCA_ICON:
			if (!attribDef.IsList())
				break;
			// else fall thru

		default:
			{
				CUSTOMATTRIBCTRLITEM ctrl;
				
				ctrl.nAttrib = attribDef.GetAttributeID();
				ctrl.sAttribID = attribDef.sUniqueID;

				CWnd* pCtrl = NULL;
				CWnd* pLabel = NULL;
				CWnd* pBuddyCtrl = NULL;
				CWnd* pBuddyLabel = NULL;

				// Main control
				ctrl.nCtrlID = nID++;
				ctrl.nLabelID = nID++;
				
				pLabel = CreateCustomAttributeLabel(attribDef, pParent, ctrl.nLabelID, FALSE, FALSE);
				ASSERT_VALID(pLabel);
				
				pCtrl = CreateCustomAttribute(attribDef, ilImages, pParent, ctrl.nCtrlID, FALSE, FALSE);
				ASSERT_VALID(pCtrl);

				// Buddy control
				BOOL bWantsBuddy = AttributeWantsBuddy(attribDef);

				if (bWantsBuddy)
				{
					ctrl.nBuddyCtrlID = nID++;
					ctrl.nBuddyLabelID = nID++;
					
					pBuddyLabel = CreateCustomAttributeLabel(attribDef, pParent, ctrl.nBuddyLabelID, FALSE, TRUE);
					ASSERT_VALID(pLabel);
					
					pBuddyCtrl = CreateCustomAttribute(attribDef, ilImages, pParent, ctrl.nBuddyCtrlID, FALSE, TRUE);
					ASSERT_VALID(pCtrl);
				}					
				
				if (!pCtrl || !pLabel || (bWantsBuddy && (!pBuddyCtrl || !pBuddyLabel)))
				{
					ASSERT(0);
					delete pCtrl;
					delete pLabel;
					delete pBuddyCtrl;
					delete pBuddyLabel;

					return FALSE;
				}

				// insert after nCtrlIDPos
				pLabel->SetWindowPos(pInsertAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);
				pCtrl->SetWindowPos(pLabel, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);


				if (bWantsBuddy)
				{
					// insert after nCtrlIDPos
					pBuddyLabel->SetWindowPos(pCtrl, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);
					pBuddyCtrl->SetWindowPos(pBuddyLabel, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);
					
					pInsertAfter = pBuddyCtrl; // next insertion point
				}
				else
				{
					pInsertAfter = pCtrl; // next insertion point
				}

				aControls.Add(ctrl);
			}
		}
	}

	return TRUE;
}

CString CTDCCustomAttributeHelper::GetAttributeTypeID(TDC_COLUMN nColID, const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	TDCCUSTOMATTRIBUTEDEFINITION attribDef;

	if (GetAttributeDef(nColID, aAttribDefs, attribDef))
		return attribDef.sUniqueID;

	// all else
	return _T("");
}

TDC_ATTRIBUTE CTDCCustomAttributeHelper::GetAttributeID(TDC_COLUMN nColID, 
											const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	if (!IsCustomColumn(nColID))
		return TDCA_NONE;

	int nDef = aAttribDefs.GetSize();
	
	while (nDef--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& def = aAttribDefs.GetData()[nDef];

		if (def.GetColumnID() == nColID)
			return def.GetAttributeID();
	}

	// all else
	return TDCA_NONE;
}

TDC_ATTRIBUTE CTDCCustomAttributeHelper::GetAttributeID(const CString& sUniqueID, 
											const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	int nDef = aAttribDefs.GetSize();
	
	while (nDef--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& def = aAttribDefs.GetData()[nDef];

		if (sUniqueID.CompareNoCase(def.sUniqueID) == 0)
			return def.GetAttributeID();
	}

	// all else
	return TDCA_NONE;
}

CString CTDCCustomAttributeHelper::GetAttributeTypeID(TDC_ATTRIBUTE nAttribID, const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	TDCCUSTOMATTRIBUTEDEFINITION attribDef;

	if (GetAttributeDef(nAttribID, aAttribDefs, attribDef))
		return attribDef.sUniqueID;

	// all else
	return _T("");
}

BOOL CTDCCustomAttributeHelper::GetAttributeDef(TDC_ATTRIBUTE nAttribID, 
												const CTDCCustomAttribDefinitionArray& aAttribDefs,
												TDCCUSTOMATTRIBUTEDEFINITION& attribDef)
{
	if (!IsCustomAttribute(nAttribID))
		return FALSE;

	int nDef = aAttribDefs.GetSize();

	while (nDef--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& def = aAttribDefs.GetData()[nDef];

		if (def.GetAttributeID() == nAttribID)
		{
			attribDef = def;
			return TRUE;
		}
	}

	// all else
	return FALSE;
}

BOOL CTDCCustomAttributeHelper::GetAttributeDef(const CString& sUniqueID, 
												const CTDCCustomAttribDefinitionArray& aAttribDefs,
												TDCCUSTOMATTRIBUTEDEFINITION& attribDef)
{
	int nAttrib = FindAttribute(sUniqueID, aAttribDefs);

	if (nAttrib != -1)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& def = aAttribDefs.GetData()[nAttrib];
		attribDef = def;
		return TRUE;
	}

	// all else
	return FALSE;
}

BOOL CTDCCustomAttributeHelper::GetAttributeDef(const CUSTOMATTRIBCTRLITEM& ctrl,
												const CTDCCustomAttribDefinitionArray& aAttribDefs,
												TDCCUSTOMATTRIBUTEDEFINITION& attribDef)
{
	return GetAttributeDef(ctrl.sAttribID, aAttribDefs, attribDef);
}

BOOL CTDCCustomAttributeHelper::GetAttributeDef(TDC_COLUMN nColID, 
												const CTDCCustomAttribDefinitionArray& aAttribDefs,
												TDCCUSTOMATTRIBUTEDEFINITION& attribDef)
{
	if (!IsCustomColumn(nColID))
		return FALSE;
	
	int nDef = aAttribDefs.GetSize();
	
	while (nDef--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& def = aAttribDefs.GetData()[nDef];
		
		if (def.GetColumnID() == nColID)
		{
			attribDef = def;
			return TRUE;
		}
	}
	
	// all else
	return FALSE;
}

DWORD CTDCCustomAttributeHelper::GetAttributeDataType(const CString& sUniqueID, 
													const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	int nAttrib = FindAttribute(sUniqueID, aAttribDefs);

	if (nAttrib != -1)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& def = aAttribDefs.GetData()[nAttrib];
		return def.GetDataType();
	}

	// all else
	return TDCCA_STRING;
}

BOOL CTDCCustomAttributeHelper::IsColumnSortable(TDC_COLUMN nColID, 
												const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	ASSERT(IsCustomColumn(nColID));

	TDCCUSTOMATTRIBUTEDEFINITION attribDef;

	if (GetAttributeDef(nColID, aAttribDefs, attribDef))
		return attribDef.HasFeature(TDCCAF_SORT);

	// else
	return FALSE;
}

BOOL CTDCCustomAttributeHelper::IsCustomAttribute(TDC_ATTRIBUTE nAttribID)
{
	return (nAttribID >= TDCA_CUSTOMATTRIB_FIRST && nAttribID <= TDCA_CUSTOMATTRIB_LAST);
}

BOOL CTDCCustomAttributeHelper::IsCustomAttributeEnabled(TDC_ATTRIBUTE nAttribID, 
														const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	TDCCUSTOMATTRIBUTEDEFINITION attribDef;

	return (GetAttributeDef(nAttribID, aAttribDefs, attribDef) && attribDef.bEnabled);
}

BOOL CTDCCustomAttributeHelper::IsCustomColumn(TDC_COLUMN nColID)
{
	return (nColID >= TDCC_CUSTOMCOLUMN_FIRST && nColID <= TDCC_CUSTOMCOLUMN_LAST);
}

BOOL CTDCCustomAttributeHelper::IsCustomColumnEnabled(TDC_COLUMN nColID, 
													  const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	TDCCUSTOMATTRIBUTEDEFINITION attribDef;

	return (GetAttributeDef(nColID, aAttribDefs, attribDef) && attribDef.bEnabled);
}

BOOL CTDCCustomAttributeHelper::IsCustomControl(UINT nCtrlID)
{
	return (nCtrlID >= IDC_FIRST_CUSTOMDATAFIELD && nCtrlID <= IDC_LAST_CUSTOMDATAFIELD);
}

int CTDCCustomAttributeHelper::FindAttribute(const CString& sAttribID, 
											 const CTDCCustomAttribDefinitionArray& aAttribDefs, int nIgnore)
{
	// validate attribute type id
	ASSERT(!sAttribID.IsEmpty());

	if (sAttribID.IsEmpty())
		return -1;

	// search attribute defs for unique ID
	int nAttribDef = aAttribDefs.GetSize();

	while (nAttribDef--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs.GetData()[nAttribDef];

		if ((nAttribDef != nIgnore) && (attribDef.sUniqueID.CompareNoCase(sAttribID) == 0))
			return nAttribDef;
	}

	// not found
	return -1;
}

void CTDCCustomAttributeHelper::SaveAutoListDataToDefs(const CWnd* pParent, 
														const CTDCCustomControlArray& aControls, 
														const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	int nDef = aAttribDefs.GetSize();
	
	while (nDef--)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& def = aAttribDefs.GetData()[nDef];

		if (def.bEnabled && def.IsAutoList())
		{
			const CComboBox* pCombo = (const CComboBox*)GetControlFromAttributeDef(pParent, def, aControls);
			ASSERT(pCombo && pCombo->IsKindOf(RUNTIME_CLASS(CComboBox)));

			if (pCombo && pCombo->IsKindOf(RUNTIME_CLASS(CComboBox)))
			{
				CDialogHelper::GetComboBoxItems(*pCombo, def.aAutoListData);

				// remove any default data
				Misc::RemoveItems(def.aDefaultListData, def.aAutoListData);
			}
		}
	}
}

CWnd* CTDCCustomAttributeHelper::GetControlFromAttributeDef(const CWnd* pParent, 
															const TDCCUSTOMATTRIBUTEDEFINITION& attribDef,
															const CTDCCustomControlArray& aControls)
{
	ASSERT_VALID(pParent);

	if (!pParent || !pParent->GetSafeHwnd())
		return FALSE;

	// search for attribute type ID in aControls
	int nCtrl = aControls.GetSize();

	while (nCtrl--)
	{
		const CUSTOMATTRIBCTRLITEM& ctrl = aControls.GetData()[nCtrl];

		if (ctrl.sAttribID.CompareNoCase(attribDef.sUniqueID) == 0)
			return pParent->GetDlgItem(ctrl.nCtrlID);
	}

	// all else
	return NULL;
}

CString CTDCCustomAttributeHelper::GetAttributeTypeID(UINT nCtrlID, const CTDCCustomControlArray& aControls)
{
	CUSTOMATTRIBCTRLITEM ctrl;

	if (GetControl(nCtrlID, aControls, ctrl))
		return ctrl.sAttribID;

	// not found
	return _T("");
}

void CTDCCustomAttributeHelper::UpdateCustomAttributeControls(const CWnd* pParent, CTDCCustomControlArray& aControls,
																const CTDCCustomAttribDefinitionArray& aAttribDefs,
																const CMapStringToString& mapData)
{
	int nCtrl = aControls.GetSize();
	CString sData;

	while (nCtrl--)
	{
		const CUSTOMATTRIBCTRLITEM& ctrl = aControls.GetData()[nCtrl];
		
		if (mapData.Lookup(ctrl.sAttribID, sData))
			UpdateCustomAttributeControl(pParent, ctrl, aAttribDefs, sData);
	}
}

void CTDCCustomAttributeHelper::ClearCustomAttributeControls(const CWnd* pParent, CTDCCustomControlArray& aControls,
															const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	int nCtrl = aControls.GetSize();

	while (nCtrl--)
	{
		const CUSTOMATTRIBCTRLITEM& ctrl = aControls.GetData()[nCtrl];
		UpdateCustomAttributeControl(pParent, ctrl, aAttribDefs, _T(""));
	}
}

BOOL CTDCCustomAttributeHelper::GetControlData(const CWnd* pParent, const CTDCCustomControlArray& aControls,
												const CTDCCustomAttribDefinitionArray& aAttribDefs,
												CMapStringToString& mapData)
{
	mapData.RemoveAll();

	int nCtrl = aControls.GetSize();

	while (nCtrl--)
	{
		const CUSTOMATTRIBCTRLITEM& ctrl = aControls.GetData()[nCtrl];
		mapData[ctrl.sAttribID] = GetControlData(pParent, ctrl, aAttribDefs);
	}

	return mapData.GetCount();
}

CString CTDCCustomAttributeHelper::GetControlData(const CWnd* pParent, const CUSTOMATTRIBCTRLITEM& ctrl,
													const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	ASSERT_VALID(pParent);

	DWORD dwDataType = 0, dwListType = 0;
	VERIFY(GetControlAttributeTypes(ctrl, aAttribDefs, dwDataType, dwListType));

	CWnd* pCtrl = pParent->GetDlgItem(ctrl.nCtrlID);
	ASSERT_VALID(pCtrl);

	if (pCtrl == NULL)
		return _T("");

	TDCCADATA data;
	CString sText;
	CStringArray aItems;
	COleDateTime date;

	switch (dwListType)
	{
	case TDCCA_NOTALIST:
		switch (dwDataType)
		{
		case TDCCA_STRING:
		case TDCCA_INTEGER:
		case TDCCA_DOUBLE:
			pCtrl->GetWindowText(sText);
			data.Set(sText);
			break;
			
		case TDCCA_DATE:
			((CDateTimeCtrlEx*)pCtrl)->GetTime(date);
			date = CDateHelper::GetDateOnly(date);

			if (CDateHelper::IsDateSet(date) && ctrl.HasBuddy())
			{
				CTimeComboBox* pBuddy = (CTimeComboBox*)pParent->GetDlgItem(ctrl.nBuddyCtrlID);
				ASSERT_VALID(pBuddy);
				
				if (pBuddy == NULL)
					return _T("");

				double dTime = pBuddy->GetOleTime();

				if (dTime > 0)
					date.m_dt += dTime;
			}
			data.Set(date);
			break;
			
			// these don't have controls
		case TDCCA_ICON:
		case TDCCA_BOOL:
			ASSERT(0);
			break;
		}
		break;
		
	case TDCCA_AUTOLIST:
		pCtrl->GetWindowText(sText);
		data.Set(sText);
		break;
		
	case TDCCA_FIXEDLIST:
		// decode icons
		if (dwDataType == TDCCA_ICON)
		{
			sText = ((CTDLIconComboBox*)pCtrl)->GetSelectedImage();
		}
		else
		{
			pCtrl->GetWindowText(sText);
		}
		data.Set(sText);
		break;
		
	case TDCCA_AUTOMULTILIST:
	case TDCCA_FIXEDMULTILIST:
		((CCheckComboBox*)pCtrl)->GetChecked(aItems);
		data.Set(aItems);
		break;
	}

	return data.AsString();
}

CString CTDCCustomAttributeHelper::FormatData(const CString& sData, const CString& sUniqueID, 
												const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	if (sData.IsEmpty())
		return sData;

	TDCCUSTOMATTRIBUTEDEFINITION def;
	VERIFY(GetAttributeDef(sUniqueID, aAttribDefs, def));

	if (def.IsList())
	{
		CStringArray aData;
		VERIFY(TDCCADATA(sData).AsArray(aData));

		return Misc::FormatArray(aData, '+');
	}
	else if (def.GetDataType() == TDCCA_DATE)
	{
		return TDCCADATA(sData).AsDate().Format(VAR_DATEVALUEONLY);
	}

	// all else
	return sData;
}

void CTDCCustomAttributeHelper::UpdateCustomAttributeControl(const CWnd* pParent, const CUSTOMATTRIBCTRLITEM& ctrl,
															  const CTDCCustomAttribDefinitionArray& aAttribDefs,
															  const CString& sData)
{
	ASSERT_VALID(pParent);

	DWORD dwDataType = 0, dwListType = 0;
	VERIFY(GetControlAttributeTypes(ctrl, aAttribDefs, dwDataType, dwListType));

	CWnd* pCtrl = pParent->GetDlgItem(ctrl.nCtrlID);
	ASSERT_VALID(pCtrl);

	if (pCtrl == NULL)
		return;

	TDCCADATA data(sData);
	CStringArray aItems;

	switch (dwListType)
	{
	case TDCCA_NOTALIST:
		{
			switch (dwDataType)
			{
			case TDCCA_STRING:
			case TDCCA_INTEGER:
			case TDCCA_DOUBLE:
				pCtrl->SetWindowText(data.AsString());
				break;
				
			case TDCCA_DATE:
				{
					CDateTimeCtrlEx* pDTC = (CDateTimeCtrlEx*)pCtrl;
					COleDateTime date = data.AsDate();

					if (!CDateHelper::IsDateSet(date))
					{
						pDTC->SetTime(COleDateTime::GetCurrentTime());
						pDTC->SendMessage(DTM_SETSYSTEMTIME, GDT_NONE, 0);
					}
					else
					{
						pDTC->SetTime(date);
					}

					if (ctrl.HasBuddy() && CDateHelper::DateHasTime(date))
					{
						CTimeComboBox* pBuddy = (CTimeComboBox*)pParent->GetDlgItem(ctrl.nBuddyCtrlID);
						ASSERT_VALID(pBuddy);
						
						if (pBuddy == NULL)
							return;
						
						pBuddy->SetOleTime(date);
					}
				}
				break;
				
			// these don't have controls
			case TDCCA_ICON:
			case TDCCA_BOOL:
				break;
			}
		}
		break;
		
	case TDCCA_AUTOLIST:
		{
			CAutoComboBox* pACB = (CAutoComboBox*)pCtrl;

			if (sData.IsEmpty())
				pACB->SetCurSel(-1);
			else
			{
				pACB->AddString(sData);
				pACB->SelectString(-1, sData);
			}
		}
		break;
		
	case TDCCA_FIXEDLIST:
		{
			CComboBox* pCB = (CComboBox*)pCtrl;

			if (sData.IsEmpty())
			{
				pCB->SetCurSel(-1);
			}
			else if (dwDataType == TDCCA_ICON)
			{
				((CTDLIconComboBox*)pCtrl)->SelectImage(sData);
			}
			else
				pCB->SelectString(-1, sData);
		}
		break;
		
	case TDCCA_AUTOMULTILIST:
	case TDCCA_FIXEDMULTILIST:
		{
			CCheckComboBox* pCCB = (CCheckComboBox*)pCtrl;

			data.AsArray(aItems);
			pCCB->SetChecked(aItems);
		}
		break;
	}
}

BOOL CTDCCustomAttributeHelper::GetControlAttributeTypes(const CUSTOMATTRIBCTRLITEM& ctrl,
														 const CTDCCustomAttribDefinitionArray& aAttribDefs,
														 DWORD& dwDataType, DWORD& dwListType)
{
	ASSERT(IsCustomControl(ctrl.nCtrlID));

	if (!IsCustomControl(ctrl.nCtrlID))
		return FALSE;

	// search attribute defs for unique ID
	int nAttribDef = FindAttribute(ctrl.sAttribID, aAttribDefs);

	if (nAttribDef != -1)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs.GetData()[nAttribDef];
	
		dwDataType = attribDef.GetDataType();
		dwListType = attribDef.GetListType();
		return TRUE;
	}

	// not found
	return FALSE;
}

FIND_ATTRIBTYPE CTDCCustomAttributeHelper::GetAttributeFindType(const CString& sUniqueID, BOOL bRelativeDate,
																const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	TDC_ATTRIBUTE nAttribID = GetAttributeID(sUniqueID, aAttribDefs);
	//ASSERT(nAttribID != TDCA_NONE);

	if (nAttribID == TDCA_NONE)
		return FT_NONE;

	ASSERT(IsCustomAttribute(nAttribID));

	if (!IsCustomAttribute(nAttribID))
		return FT_NONE;

	return GetAttributeFindType(nAttribID, bRelativeDate, aAttribDefs);
}

FIND_ATTRIBTYPE CTDCCustomAttributeHelper::GetAttributeFindType(TDC_ATTRIBUTE nAttribID, BOOL bRelativeDate, 
																const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	if (!IsCustomAttribute(nAttribID))
		return SEARCHPARAM::GetAttribType(nAttribID, bRelativeDate);

	TDCCUSTOMATTRIBUTEDEFINITION attribDef;
	VERIFY (GetAttributeDef(nAttribID, aAttribDefs, attribDef));

	// treat lists as strings, except for icon lists
	if (attribDef.IsList() && (attribDef.GetDataType() != TDCCA_ICON))
		return FT_STRING;

	// else
	DWORD dwDataType = attribDef.GetDataType();

	switch (dwDataType)
	{
	case TDCCA_STRING:		return FT_STRING;
	case TDCCA_INTEGER:		return FT_INTEGER;
	case TDCCA_DOUBLE:		return FT_DOUBLE;
	case TDCCA_DATE:		return (bRelativeDate ? FT_DATE_REL : FT_DATE);
	case TDCCA_BOOL:		return FT_BOOL;
	case TDCCA_ICON:		return FT_ICON;
	}

	return FT_NONE;
}

int CTDCCustomAttributeHelper::AppendUniqueAttributes(const CTDCCustomAttribDefinitionArray& aAttribDefs,
														CTDCCustomAttribDefinitionArray& aMasterDefs)
{
	if (aMasterDefs.GetSize() == 0)
	{
		aMasterDefs.Copy(aAttribDefs);
	}
	else
	{
		for (int nAttrib = 0; nAttrib < aAttribDefs.GetSize(); nAttrib++)
		{
			TDCCUSTOMATTRIBUTEDEFINITION attribDef = aAttribDefs[nAttrib];

			// look for duplicate attrib ID in master list
			if (FindAttribute(attribDef.sUniqueID, aMasterDefs) == -1)
				aMasterDefs.Add(attribDef);
			
			// else skip
		}
	}

	return aMasterDefs.GetSize();
}

int CTDCCustomAttributeHelper::CalcLongestListItem(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, CDC* pDC)
{
	ASSERT (attribDef.IsList());

	if (!attribDef.IsList())
		return 0;

	int nItem = attribDef.aDefaultListData.GetSize(), nLongest = 0;

	while (nItem--)
	{
		const CString& sItem = Misc::GetItem(attribDef.aDefaultListData, nItem);
		int nItemLen = 0;

		switch (attribDef.GetDataType())
		{
		case TDCCA_STRING:
		case TDCCA_INTEGER:	
		case TDCCA_DOUBLE:	
		case TDCCA_DATE:	
		case TDCCA_BOOL:
			nItemLen = pDC->GetTextExtent(sItem).cx;
			break;

		case TDCCA_ICON:
			if (attribDef.IsList())
			{
				nItemLen = 20; // for the icon
				
				// check for trailing text
				CString sDummy, sName;
				
				if (attribDef.DecodeImageTag(sItem, sDummy, sName) && !sName.IsEmpty())
					nItemLen += pDC->GetTextExtent(sName).cx;
			}
			break;
		}

		nLongest = max(nLongest, nItemLen);
	}

	return nLongest;
}

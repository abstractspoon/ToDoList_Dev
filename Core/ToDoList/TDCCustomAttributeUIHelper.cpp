// TDCCustomAttributeHelper.cpp: implementation of the CTDCCustomAttributeUIHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Resource.h"
#include "TDCCustomAttributeUIHelper.h"
#include "TDCstruct.h"
#include "tdliconcombobox.h"
#include "todoitem.h"
#include "TDLFilterDateComboBox.h"
#include "TDCImageList.h"

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
#include "..\shared\timeedit.h"
#include "..\shared\fileedit.h"
#include "..\shared\FileComboBox.h"
#include "..\shared\HoldRedraw.h"
#include "..\shared\WndPrompt.h"
#include "..\shared\Icon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWnd* CTDCCustomAttributeUIHelper::CreateAttributeCtrl(CWnd* pParent, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef,
													   const TDCCADATA& data,
													   const CTDCImageList& ilImages,
													   UINT nCtrlID, BOOL bBuddy, BOOL bMultiSelectionFilter)
{
	// Sanity check
	BOOL bFilter = IsCustomFilterControl(nCtrlID);

	if (bBuddy && !AttributeWantsBuddy(attribDef, bFilter))
	{
		ASSERT(0);
		return NULL;
	}
	
	DWORD dwDataType = attribDef.GetDataType();
	DWORD dwListType = attribDef.GetListType();

	CWnd* pControl = NULL;
	DWORD dwStyle = (WS_CHILD | WS_TABSTOP); // Invisible
	DWORD dwExStyle = WS_EX_CLIENTEDGE;
	LPCTSTR szClass = NULL;

	if (dwListType == TDCCA_NOTALIST)
	{
		switch (dwDataType)
		{
		case TDCCA_STRING:
			pControl = new CEdit;
			szClass = WC_EDIT;
			dwStyle |= (ES_LEFT | ES_AUTOHSCROLL);
			break;

		case TDCCA_DATE:
			if (bFilter)
			{
				if (bBuddy)
				{
					switch (data.AsInteger())
					{
					case FD_NEXTNDAYS:
						{
							CEnEdit* pEdit = new CEnEdit(TRUE, _T("0123456789"));
							CIcon iconUpdate(IDI_UPDATE_FILTER, 16);

							pEdit->AddButton(1, iconUpdate, CEnString(IDS_TDC_UPDATEFILTER_TIP));

							pControl = pEdit;
							szClass = WC_EDIT;
							dwStyle |= (ES_LEFT | ES_AUTOHSCROLL);
						}
						break;

					case FD_USER:
					default:
						pControl = new CDateTimeCtrlEx(0);
						szClass = WC_DATETIMEPICK;
						dwStyle |= (DTS_SHORTDATEFORMAT | DTS_RIGHTALIGN);
						break;
					}
				}
				else
				{
					pControl = new CTDLFilterDateComboBox();
					szClass = WC_COMBOBOX;
					dwStyle |= (CBS_DROPDOWNLIST | WS_VSCROLL | CBS_AUTOHSCROLL);
				}
			}
			else
			{
				if (bBuddy)
				{
					// For time of day
					pControl = new CTimeComboBox(TCB_HALFHOURS | TCB_NOTIME | TCB_HOURSINDAY);
					szClass = WC_COMBOBOX;
					dwStyle |= (CBS_DROPDOWN | WS_VSCROLL | CBS_AUTOHSCROLL);
				}
				else
				{
					pControl = new CDateTimeCtrlEx(0);
					szClass = WC_DATETIMEPICK;
					dwStyle |= (DTS_SHORTDATEFORMAT | DTS_RIGHTALIGN | DTS_SHOWNONE);
				}
			}
			break;

		case TDCCA_INTEGER:
			pControl = new CMaskEdit(_T("-0123456789"));
			szClass = WC_EDIT;
			dwStyle |= (ES_LEFT | ES_AUTOHSCROLL);
			break;

		case TDCCA_DOUBLE:
			pControl = new CMaskEdit(_T("-.0123456789"), ME_LOCALIZEDECIMAL);
			szClass = WC_EDIT;
			dwStyle |= (ES_LEFT | ES_AUTOHSCROLL);
			break;

		case TDCCA_FRACTION:
			pControl = new CMaskEdit(_T("0123456789/"));
			szClass = WC_EDIT;
			dwStyle |= (ES_LEFT | ES_AUTOHSCROLL);
			break;

		case TDCCA_FILELINK:
			pControl = new CFileEdit(FES_GOBUTTON);
			szClass = WC_EDIT;
			dwStyle |= (ES_LEFT | ES_AUTOHSCROLL);
			break;

		case TDCCA_TIMEPERIOD:
			pControl = new CTimeEdit;
			szClass = WC_EDIT;
			dwStyle |= (ES_LEFT | ES_AUTOHSCROLL);
			break;

		case TDCCA_BOOL:
			if (attribDef.HasFeature(TDCCAF_SHOWEDITFIELD))
			{
				pControl = new CButton;
				szClass = WC_BUTTON;
				dwStyle |= BS_AUTOCHECKBOX;
				dwExStyle = 0;
			}
			break;

		case TDCCA_ICON:
		case TDCCA_CALCULATION:
			// these don't have controls
			break;

		default:
			// Unknown/unhandled data type
			ASSERT(0);
			return NULL;
		}
	}
	else if (bFilter)
	{
		szClass = WC_COMBOBOX;
		dwStyle |= (CBS_DROPDOWNLIST | WS_VSCROLL | CBS_AUTOHSCROLL);

		switch (dwDataType)
		{
		case TDCCA_ICON:
			pControl = new CTDLIconComboBox(ilImages, bMultiSelectionFilter, TRUE);
			break;

		default:
			pControl = new CEnCheckComboBox(bMultiSelectionFilter, IDS_TDC_NONE, IDS_TDC_ANY);
			break;
		}
	}
	else // it's a list
	{
		switch (dwListType)
		{
		case TDCCA_AUTOLIST:
			if (dwDataType == TDCCA_FILELINK)
			{
				pControl = new CFileComboBox(FES_GOBUTTON);
				szClass = WC_COMBOBOX;
				dwStyle |= (CBS_DROPDOWN | WS_VSCROLL | CBS_AUTOHSCROLL);
			}
			else
			{
				pControl = new CAutoComboBox(ACBS_ALLOWDELETE | ACBS_AUTOCOMPLETE);
				szClass = WC_COMBOBOX;
				dwStyle |= (CBS_DROPDOWN | CBS_SORT | WS_VSCROLL | CBS_AUTOHSCROLL);

				// add number mask as required
				switch (dwDataType)
				{
				case TDCCA_INTEGER:
					((CAutoComboBox*)pControl)->SetEditMask(_T("-0123456789"));
					break;
				
				case TDCCA_DOUBLE:
					((CAutoComboBox*)pControl)->SetEditMask(_T("-.0123456789"), ME_LOCALIZEDECIMAL);
					break;
				}
			}
			break;
		
		case TDCCA_FIXEDLIST:
			{
				szClass = WC_COMBOBOX;
				dwStyle |= (CBS_DROPDOWNLIST | WS_VSCROLL | CBS_AUTOHSCROLL);

				switch (dwDataType)
				{
				case TDCCA_ICON:
					pControl = new CTDLIconComboBox(ilImages, FALSE, FALSE);
					break;
				
				default:
					pControl = new COwnerdrawComboBoxBase; // so they render the same
					break;
				}
			}
			break;
		
		case TDCCA_AUTOMULTILIST:
			{
				pControl = new CCheckComboBox(ACBS_ALLOWDELETE | ACBS_AUTOCOMPLETE);
				szClass = WC_COMBOBOX;
				dwStyle |= (CBS_DROPDOWN | CBS_SORT | WS_VSCROLL | CBS_AUTOHSCROLL);

				// Add number mask as required
				// Note: These need to include a space because CCheckComboBox 
				// includes a space when it formats multiple items
				switch (dwDataType)
				{
				case TDCCA_INTEGER:
					((CAutoComboBox*)pControl)->SetEditMask(_T("-0123456789, "), ME_LOCALIZESEPARATOR);
					break;
				
				case TDCCA_DOUBLE:
					((CAutoComboBox*)pControl)->SetEditMask(_T("-.0123456789,"), ME_LOCALIZEDECIMAL | ME_LOCALIZESEPARATOR);
					break;
				}
			}
			break;
		
		case TDCCA_FIXEDMULTILIST:
			{
				szClass = WC_COMBOBOX;
				dwStyle |= (CBS_DROPDOWNLIST | WS_VSCROLL | CBS_AUTOHSCROLL);
			
				switch (dwDataType)
				{
				case TDCCA_ICON:
					pControl = new CTDLIconComboBox(ilImages, TRUE, FALSE);
					break;
				
				default:
					pControl = new CCheckComboBox;
					break;
				}
			}
			break;

		default:
			ASSERT(0); // Sanity check
			break;
		}
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

			// add default data to non-filelink lists
			if (attribDef.IsList() && !attribDef.IsDataType(TDCCA_FILELINK)) 
			{
				ASSERT(pControl->IsKindOf(RUNTIME_CLASS(CComboBox)));
				CComboBox* pCB = (CComboBox*)pControl;

				CStringArray aListData;
				attribDef.GetUniqueListData(aListData);
				
				if (pControl->IsKindOf(RUNTIME_CLASS(CEnCheckComboBox)))
				{
					ASSERT(bFilter || (dwDataType == TDCCA_ICON));

					((CEnCheckComboBox*)pCB)->SetStrings(aListData);
				}
				else
				{
					CDialogHelper::SetComboBoxItems(*pCB, aListData);
				}

				if (!bFilter)
				{
					// prepend empty items to single selection lists
					switch (attribDef.GetListType())
					{
					case TDCCA_FIXEDLIST:
					case TDCCA_AUTOLIST:
						if (!attribDef.HasFeature(TDCCAF_EXCLUDEBLANKITEM))
							pCB->InsertString(0, _T(""));
						break;
					}
				}
			}
			else if (bFilter && pControl->IsKindOf(RUNTIME_CLASS(CDateTimeCtrlEx)))
			{
				((CDateTimeCtrlEx*)pControl)->SetMonthCalStyle(MCS_WEEKNUMBERS);
			}
		}
	}

	return pControl;
}

BOOL CTDCCustomAttributeUIHelper::AttributeWantsBuddy(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bFilter)
{
	switch (attribDef.GetDataType())
	{
	case TDCCA_STRING:
	case TDCCA_INTEGER:
	case TDCCA_DOUBLE:
	case TDCCA_FRACTION:
	case TDCCA_BOOL:
	case TDCCA_ICON:
	case TDCCA_FILELINK:
	case TDCCA_TIMEPERIOD:
		return FALSE;
		
	case TDCCA_DATE:
		return (bFilter || attribDef.HasFeature(TDCCAF_SHOWTIME));
	}

	ASSERT(0);
	return FALSE;
}

CString CTDCCustomAttributeUIHelper::GetControlLabel(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, 
													 BOOL bBuddy, BOOL bFilter)
{
	if (!bBuddy)
	{
		return (attribDef.sLabel.IsEmpty() ? attribDef.sColumnTitle : attribDef.sLabel);
	}
	else if (AttributeWantsBuddy(attribDef, bFilter))
	{
		switch (attribDef.GetDataType())
		{
		case TDCCA_STRING:
		case TDCCA_INTEGER:
		case TDCCA_DOUBLE:
		case TDCCA_FRACTION:
		case TDCCA_BOOL:
		case TDCCA_ICON:
		case TDCCA_FILELINK:
		case TDCCA_TIMEPERIOD:
			break;
			
		case TDCCA_DATE:
			if (!bFilter)
				return CEnString(IDS_CUSTOM_TIMEATTRIB, GetControlLabel(attribDef, FALSE, bFilter));
			break;

		default:
			ASSERT(0);
			break;
		}
	}

	return _T("");
}

CWnd* CTDCCustomAttributeUIHelper::CreateAttributeLabelCtrl(CWnd* pParent, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, 
															const TDCCADATA& /*data*/, UINT nCtrlID, BOOL bBuddy)
{
	CStatic* pLabel = new CStatic;
	ASSERT (pLabel);

	if (pLabel)
	{
		DWORD dwStyle = (WS_CHILD | SS_CENTERIMAGE); // Invisible

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
			BOOL bFilter = IsCustomFilterControl(nCtrlID);

			pLabel->SetWindowText(GetControlLabel(attribDef, bBuddy, bFilter));

			// then font			
			pLabel->SendMessage(WM_SETFONT, (WPARAM)CDialogHelper::GetFont(pParent), 0); 
		}
	}

	return pLabel;
}

BOOL CTDCCustomAttributeUIHelper::GetControl(UINT nCtrlID, const CTDCCustomControlArray& aControls, 
										   CUSTOMATTRIBCTRLITEM& ctrl)
{
	// sanity check
	if (nCtrlID == 0)
	{
		ASSERT(0);
		return FALSE;
	}

	int nCtrl = aControls.Find(nCtrlID, FALSE);

	if (nCtrl != -1)
	{
		ctrl = aControls[nCtrl];
		return TRUE;
	}

	// not found 
	return FALSE;
}

BOOL CTDCCustomAttributeUIHelper::GetControl(const CString& sUniqueID, const CTDCCustomControlArray& aControls, 
										   CUSTOMATTRIBCTRLITEM& ctrl)
{
	int nCtrl = aControls.Find(sUniqueID);

	if (nCtrl != -1)
	{
		ctrl = aControls[nCtrl];
		return TRUE;
	}

	// not found 
	return FALSE;
}

void CTDCCustomAttributeUIHelper::CleanupControls(CTDCCustomControlArray& aControls, CWnd* pParent)
{
	aControls.DeleteCtrls(pParent);
	aControls.RemoveAll();
}

void CTDCCustomAttributeUIHelper::AddWindowPrompts(const CTDCCustomControlArray& aControls, CWnd* pParent, CWndPromptManager& mgrPrompts)
{
	int nCtrl = aControls.GetSize();

	while (nCtrl--)
	{
		const CWnd* pCtrl = aControls[nCtrl].GetCtrl(pParent);
		ASSERT_VALID(pCtrl);

		if (pCtrl)
		{
			if (pCtrl->IsKindOf(RUNTIME_CLASS(CEdit)))
			{
				mgrPrompts.SetEditPrompt(*pCtrl, IDS_TDC_NONE);
			}
			else if (pCtrl->IsKindOf(RUNTIME_CLASS(CComboBox)))
			{
				mgrPrompts.SetComboPrompt(*pCtrl, IDS_TDC_NONE);
			}
		}
	}
}

BOOL CTDCCustomAttributeUIHelper::NeedRebuildEditControls(const CTDCCustomAttribDefinitionArray& aOldAttribDefs, 
														const CTDCCustomAttribDefinitionArray& aNewAttribDefs, 
														const CTDCCustomControlArray& aOldControls)
{
	return NeedRebuildControls(aOldAttribDefs, aNewAttribDefs, aOldControls, IDC_FIRST_CUSTOMEDITFIELD);
}

BOOL CTDCCustomAttributeUIHelper::NeedRebuildFilterControls(const CTDCCustomAttribDefinitionArray& aOldAttribDefs, 
														  const CTDCCustomAttribDefinitionArray& aNewAttribDefs, 
															const CTDCCustomControlArray& aOldControls)
{
	return NeedRebuildControls(aOldAttribDefs, aNewAttribDefs, aOldControls, IDC_FIRST_CUSTOMFILTERFIELD);
}

BOOL CTDCCustomAttributeUIHelper::NeedRebuildControls(const CTDCCustomAttribDefinitionArray& aOldAttribDefs, 
													  const CTDCCustomAttribDefinitionArray& aNewAttribDefs, 
													  const CTDCCustomControlArray& aOldControls, UINT nCtrlIDStart)
{
	CTDCCustomControlArray aNewControls;
	int nNumNewCtrls = GetCustomAttributeCtrls(aNewAttribDefs, nCtrlIDStart, aNewControls);

	if (nNumNewCtrls != aOldControls.GetSize())
		return TRUE;

	// Compare each new item with the old, also checking list data
	// Order is important
	BOOL bFilter = IsCustomFilterControl(nCtrlIDStart);

	for (int nCtrl = 0; nCtrl < nNumNewCtrls; nCtrl++)
	{
		const CUSTOMATTRIBCTRLITEM& ctrlNew = aNewControls[nCtrl];
		const CUSTOMATTRIBCTRLITEM& ctrlOld = aOldControls[nCtrl];

		if (ctrlNew != ctrlOld)
			return TRUE;

		// Definition indices can be different
		int nNewDef = aNewAttribDefs.Find(ctrlNew.sAttribID);
		int nOldDef = aOldAttribDefs.Find(ctrlNew.sAttribID);
		
		ASSERT((nNewDef != -1) && (nOldDef != -1));

		const TDCCUSTOMATTRIBUTEDEFINITION& attribNew = aNewAttribDefs[nNewDef];
		const TDCCUSTOMATTRIBUTEDEFINITION& attribOld = aOldAttribDefs[nOldDef];

		// Only compare auto-list data when filtering
		if (!attribNew.Matches(attribOld, bFilter))
			return TRUE;
	}

	return FALSE;
}

int CTDCCustomAttributeUIHelper::GetCustomAttributeCtrls(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
														 UINT nCtrlIDStart, CTDCCustomControlArray& aControls)
{
	aControls.RemoveAll();

	UINT nID = nCtrlIDStart;
	BOOL bFilter = IsCustomFilterControl(nCtrlIDStart);

	for (int nAttrib = 0; nAttrib < aAttribDefs.GetSize(); nAttrib++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs.GetData()[nAttrib];

		// don't add unwanted controls
		if (!WantControl(attribDef, bFilter))
			continue;

		switch (attribDef.GetDataType())
		{
		case TDCCA_ICON:
			if (!attribDef.IsList())
				break;
			// else fall thru

		default:
			{
				CUSTOMATTRIBCTRLITEM ctrl;

				ctrl.nAttrib = attribDef.GetAttributeID();
				ctrl.sAttribID = attribDef.sUniqueID;

				// Main control
				ctrl.nCtrlID = nID++;
				ctrl.nLabelID = nID++;

				// Buddy control
				if (AttributeWantsBuddy(attribDef, bFilter))
				{
					ctrl.nBuddyCtrlID = nID++;
					ctrl.nBuddyLabelID = nID++;
				}					

				aControls.Add(ctrl);
			}
		}
	}

	return aControls.GetSize();
}

BOOL CTDCCustomAttributeUIHelper::RebuildEditControls(CWnd* pParent, const CTDCCustomAttribDefinitionArray& aAttribDefs,
													  const CTDCImageList& ilImages,
													  UINT nCtrlIDPos,
													  CTDCCustomControlArray& aControls)
{
	CTDCCustomAttributeDataMap mapUnused;

	return RebuildControls(pParent, aAttribDefs, mapUnused, ilImages, nCtrlIDPos, IDC_FIRST_CUSTOMEDITFIELD, FALSE, FALSE, aControls);
}

BOOL CTDCCustomAttributeUIHelper::RebuildFilterControls(CWnd* pParent, 
														const CTDCCustomAttribDefinitionArray& aAttribDefs,
														const CTDCCustomAttributeDataMap& mapCtrlData,
														const CTDCImageList& ilImages,
														UINT nCtrlIDPos,
														BOOL bMultiSelection,
														CTDCCustomControlArray& aControls)
{
	return RebuildControls(pParent, aAttribDefs, mapCtrlData, ilImages, nCtrlIDPos, IDC_FIRST_CUSTOMFILTERFIELD, TRUE, bMultiSelection, aControls);
}

BOOL CTDCCustomAttributeUIHelper::RebuildControls(CWnd* pParent, 
												  const CTDCCustomAttribDefinitionArray& aAttribDefs,
												  const CTDCCustomAttributeDataMap& mapCtrlData,
												  const CTDCImageList& ilImages,
												  UINT nCtrlIDPos, UINT nCtrlIDStart,
												  BOOL bFilter, BOOL bMultiSelectionFilter,
												  CTDCCustomControlArray& aControls)
{
	ASSERT_VALID(pParent);

	CHoldRedraw hr(*pParent);

	// remove all existing custom attribute fields
	CleanupControls(aControls, pParent);

	// recreate controls and columns
	UINT nID = nCtrlIDStart;

	const CWnd* pInsertAfter = pParent->GetDlgItem(nCtrlIDPos);
	ASSERT(pInsertAfter);

	for (int nAttrib = 0; nAttrib < aAttribDefs.GetSize(); nAttrib++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs.GetData()[nAttrib];

		// don't add unwanted controls
		if (!WantControl(attribDef, bFilter))
			continue;
		
		switch (attribDef.GetDataType())
		{
		case TDCCA_CALCULATION:
			// Don't have edit fields
			continue;

		case TDCCA_BOOL:
			if (!attribDef.HasFeature(TDCCAF_SHOWEDITFIELD))
				continue;
			// else fall thru
			break;

		case TDCCA_ICON:
			if (!attribDef.IsList())
				continue;
			// else fall thru
			break;
		}

		CUSTOMATTRIBCTRLITEM ctrl;

		ctrl.nAttrib = attribDef.GetAttributeID();
		ctrl.sAttribID = attribDef.sUniqueID;

		TDCCADATA data;
		mapCtrlData.Lookup(attribDef.sUniqueID, data);

		CWnd* pCtrl = NULL;
		CWnd* pLabel = NULL;
		CWnd* pBuddyCtrl = NULL;
		CWnd* pBuddyLabel = NULL;

		// Main control
		ctrl.nCtrlID = nID++;
		ctrl.nLabelID = nID++;

		pCtrl = CreateAttributeCtrl(pParent, attribDef, data, ilImages, ctrl.nCtrlID, FALSE, bMultiSelectionFilter);

		if (pCtrl)
			pLabel = CreateAttributeLabelCtrl(pParent, attribDef, data, ctrl.nLabelID, FALSE);

		// Buddy control
		BOOL bWantsBuddy = (pCtrl && pLabel && AttributeWantsBuddy(attribDef, bFilter));

		if (bWantsBuddy)
		{
			ctrl.nBuddyCtrlID = nID++;
			ctrl.nBuddyLabelID = nID++;

			pBuddyCtrl = CreateAttributeCtrl(pParent, attribDef, data, ilImages, ctrl.nBuddyCtrlID, TRUE, bMultiSelectionFilter);

			if (pBuddyCtrl)
				pBuddyLabel = CreateAttributeLabelCtrl(pParent, attribDef, data, ctrl.nBuddyLabelID, TRUE);
		}

		if (!pCtrl || !pLabel || (bWantsBuddy && (!pBuddyCtrl || !pBuddyLabel)))
		{
			ASSERT(0);

			// Make resilient to opening newer tasklists 
			// with data types unknown to us
			ctrl.DeleteCtrls(pParent);
			continue;
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

		SetBuddyVisibility(pParent, ctrl, attribDef, data);

		aControls.Add(ctrl);
	}

	return TRUE;
}

BOOL CTDCCustomAttributeUIHelper::WantControl(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bFilter)
{
	if (attribDef.bEnabled)
		return (!bFilter || attribDef.HasFeature(TDCCAF_FILTER));

	return FALSE;
}

BOOL CTDCCustomAttributeUIHelper::IsCustomEditControl(UINT nCtrlID)
{
	return (nCtrlID >= IDC_FIRST_CUSTOMEDITFIELD && nCtrlID <= IDC_LAST_CUSTOMEDITFIELD);
}

BOOL CTDCCustomAttributeUIHelper::IsCustomFilterControl(UINT nCtrlID)
{
	return (nCtrlID >= IDC_FIRST_CUSTOMFILTERFIELD && nCtrlID <= IDC_LAST_CUSTOMFILTERFIELD);
}

CString CTDCCustomAttributeUIHelper::GetFilterControlTooltip(const CWnd* pParent, UINT nCtrlID)
{
	ASSERT(IsCustomFilterControl(nCtrlID));

	return GetControlTooltip(pParent, nCtrlID);
}

CString CTDCCustomAttributeUIHelper::GetEditControlTooltip(const CWnd* pParent, UINT nCtrlID)
{
	ASSERT(IsCustomEditControl(nCtrlID));
	
	return GetControlTooltip(pParent, nCtrlID);
}

CString CTDCCustomAttributeUIHelper::GetControlTooltip(const CWnd* pParent, UINT nCtrlID)
{
	CWnd* pCtrl = pParent->GetDlgItem(nCtrlID);

	if (pCtrl && pCtrl->IsKindOf(RUNTIME_CLASS(CCheckComboBox)))
		return ((CCheckComboBox*)pCtrl)->GetTooltip();

	// else
	return _T("");
}

BOOL CTDCCustomAttributeUIHelper::FlushEditControl(CWnd* pCtrl, const CWnd* pParent, const CTDCCustomControlArray& aControls)
{
	if (!pCtrl || !pParent)
	{
		ASSERT(0);
		return FALSE;
	}

	if (aControls.GetSize() == 0)
		return FALSE;

	int nCtrl = aControls.GetSize();

	while (nCtrl--)
	{
		const CUSTOMATTRIBCTRLITEM& ctrl = aControls.GetData()[nCtrl];

		CWnd* pCustomCtrl = ctrl.GetCtrl(pParent);
		ASSERT(pCustomCtrl);
		
		if (CDialogHelper::IsChildOrSame(pCustomCtrl, pCtrl))
		{
			if (pCustomCtrl->IsKindOf(RUNTIME_CLASS(CAutoComboBox)))
			{
				((CAutoComboBox*)pCustomCtrl)->Flush();
				return TRUE;
			}

			break;
		}
	}

	return FALSE;
}

void CTDCCustomAttributeUIHelper::SaveAutoListDataToDefs(const CWnd* pParent, 
														const CTDCCustomControlArray& aControls, 
														const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	int nCtrl = aControls.GetSize();
	
	while (nCtrl--)
	{
		SaveAutoListDataToDef(pParent, aControls.GetData()[nCtrl], aAttribDefs);
	}
}

void CTDCCustomAttributeUIHelper::SaveAutoListDataToDef(const CWnd* pParent, 
														const CUSTOMATTRIBCTRLITEM& ctrl,
														const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	int nDef = aAttribDefs.Find(ctrl.nAttrib);
	
	if (nDef == -1)
	{
		ASSERT(0);
		return;
	}

	const TDCCUSTOMATTRIBUTEDEFINITION& def = aAttribDefs.GetData()[nDef];

	if (def.bEnabled && def.IsAutoList() && !def.IsDataType(TDCCA_FILELINK))
	{
		const CComboBox* pCombo = (const CComboBox*)ctrl.GetCtrl(pParent);
		ASSERT(pCombo && pCombo->IsKindOf(RUNTIME_CLASS(CComboBox)));

		if (pCombo && pCombo->IsKindOf(RUNTIME_CLASS(CComboBox)))
		{
			CDialogHelper::GetComboBoxItems(*pCombo, def.aAutoListData);

			// remove any default data
			Misc::RemoveItems(def.aDefaultListData, def.aAutoListData);
		}
	}
}

CWnd* CTDCCustomAttributeUIHelper::GetControlFromAttributeDef(const CWnd* pParent, 
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
			return ctrl.GetCtrl(pParent);
	}

	// all else
	return NULL;
}

void CTDCCustomAttributeUIHelper::UpdateControls(const CWnd* pParent, CTDCCustomControlArray& aControls,
												const CTDCCustomAttribDefinitionArray& aAttribDefs,
												const CTDCCustomAttributeDataMap& mapData)
{
	int nCtrl = aControls.GetSize();

	while (nCtrl--)
		UpdateControl(pParent, aControls.GetData()[nCtrl], aAttribDefs, mapData);
}

void CTDCCustomAttributeUIHelper::UpdateControl(const CWnd* pParent, CUSTOMATTRIBCTRLITEM& ctrl,
												const CTDCCustomAttribDefinitionArray& aAttribDefs,
												const CTDCCustomAttributeDataMap& mapData)
{
	ASSERT(TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(ctrl.nAttrib));

	TDCCADATA data;
		
	if (mapData.Lookup(ctrl.sAttribID, data))
		UpdateControl(pParent, ctrl, aAttribDefs, data);
	else
		ClearControl(pParent, ctrl, aAttribDefs);
}

void CTDCCustomAttributeUIHelper::ClearControls(const CWnd* pParent, CTDCCustomControlArray& aControls,
												const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	int nCtrl = aControls.GetSize();

	while (nCtrl--)
	{
		CUSTOMATTRIBCTRLITEM& ctrl = aControls[nCtrl];
		ClearControl(pParent, ctrl, aAttribDefs);
	}
}

TDCCAUI_UPDATERESULT CTDCCustomAttributeUIHelper::GetControlsData(const CWnd* pParent, CTDCCustomControlArray& aControls,
												const CTDCCustomAttribDefinitionArray& aAttribDefs,
												const CTDCCustomAttributeDataMap& mapDataPrev,
												CTDCCustomAttributeDataMap& mapData)
{
	mapData.RemoveAll();

	int nCtrl = aControls.GetSize();
	TDCCADATA data, dataPrev;
	
	TDCCAUI_UPDATERESULT nRes = TDCCAUIRES_FAIL;

	while (nCtrl--)
	{
		CUSTOMATTRIBCTRLITEM& ctrl = aControls[nCtrl];
		mapDataPrev.Lookup(ctrl.sAttribID, dataPrev);

		TDCCAUI_UPDATERESULT nCtrlRes = GetControlData(pParent, ctrl, aAttribDefs, dataPrev, data);

		switch (nCtrlRes)
		{
		case TDCCAUIRES_FAIL:
			mapData.RemoveKey(ctrl.sAttribID);
			break;

		case TDCCAUIRES_SUCCESS:
			mapData[ctrl.sAttribID] = data;

			if (nRes != TDCCAUIRES_REPOSCTRLS)
				nRes = nCtrlRes;
			break;

		case TDCCAUIRES_REPOSCTRLS:
			mapData[ctrl.sAttribID] = data;
			nRes = nCtrlRes;
			break;
		}
	}

	return nRes;
}

TDCCAUI_UPDATERESULT CTDCCustomAttributeUIHelper::GetControlData(const CWnd* pParent, CUSTOMATTRIBCTRLITEM& ctrl,
																 const CTDCCustomAttribDefinitionArray& aAttribDefs,
																 const TDCCADATA& dataPrev, TDCCADATA& data)
{
	ASSERT_VALID(pParent);
	ASSERT(TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(ctrl.nAttrib));

	DWORD dwDataType = 0, dwListType = 0;
	VERIFY(GetControlAttributeTypes(ctrl, aAttribDefs, dwDataType, dwListType));

	BOOL bFilter = IsCustomFilterControl(ctrl.nCtrlID);
	CWnd* pCtrl = ctrl.GetCtrl(pParent);

	if (pCtrl == NULL)
	{
		ASSERT(0);
		return TDCCAUIRES_FAIL;
	}

	CString sText;
	CStringArray aItems;
	COleDateTime date;

	TDCCAUI_UPDATERESULT nRes = TDCCAUIRES_SUCCESS;

	if (dwListType == TDCCA_NOTALIST)
	{
		switch (dwDataType)
		{
		case TDCCA_STRING:
		case TDCCA_INTEGER:
		case TDCCA_DOUBLE:
		case TDCCA_FILELINK:
		case TDCCA_FRACTION:
			pCtrl->GetWindowText(sText);
			data.Set(sText);
			break;

		case TDCCA_DATE:
			if (bFilter)
			{
				FILTER_DATE nFilter = ((CTDLFilterDateComboBox*)pCtrl)->GetSelectedFilter();
				FILTER_DATE nPrevFilter = (FILTER_DATE)dataPrev.AsInteger();

				BOOL bShowBuddy = FALSE;
				BOOL bWasShowingBuddy = ((nPrevFilter == FD_USER) || (nPrevFilter == FD_NEXTNDAYS));

				switch (nFilter)
				{
				case FD_ANY:
					data.Clear();
					break;

				case FD_USER:
					{
						// Check existing buddy is of the right type
						BOOL bCreated = FALSE;
						CWnd* pBuddy = CheckRecreateDateFilterBuddy(pParent, ctrl, nFilter, aAttribDefs, bCreated);

						if (pBuddy)
						{
							COleDateTime date;
							VERIFY(((CDateTimeCtrlEx*)pBuddy)->GetTime(date));

							SetDateFilter(nFilter, date.m_dt, data);
							
							if (bCreated)
							{
								// Reset combo text
								((CTDLFilterDateComboBox*)pCtrl)->SetNextNDays(7);

								nRes = TDCCAUIRES_REPOSCTRLS;
							}

							bShowBuddy = TRUE;
						}
					}
					break;

				case FD_NEXTNDAYS:
					{
						// Check existing buddy is of the right type
						BOOL bCreated = FALSE;
						CWnd* pBuddy = CheckRecreateDateFilterBuddy(pParent, ctrl, nFilter, aAttribDefs, bCreated);

						if (pBuddy)
						{
							CString sDays;
							pBuddy->GetWindowText(sDays);

							double dDays = ValidateDateFilter(FD_NEXTNDAYS, _ttoi(sDays));
							SetDateFilter(nFilter, dDays, data);

							if (sDays.IsEmpty())
								pBuddy->SetWindowText(Misc::Format((int)dDays));

							// Update combo text
							((CTDLFilterDateComboBox*)pCtrl)->SetNextNDays((int)dDays);

							bShowBuddy = TRUE;

							if (bCreated)
								nRes = TDCCAUIRES_REPOSCTRLS;
						}
						else
						{
							ASSERT(0);
						}
					}
					break;

				default:
					data.Set(nFilter);
					break;
				}

				ctrl.ShowBuddy(pParent, bShowBuddy);

				if (Misc::StateChanged(bWasShowingBuddy, bShowBuddy))
					nRes = TDCCAUIRES_REPOSCTRLS;
			}
			else
			{
				VERIFY(((CDateTimeCtrlEx*)pCtrl)->GetTime(date));
				date = CDateHelper::GetDateOnly(date);

				if (CDateHelper::IsDateSet(date) && ctrl.HasBuddy())
				{
					CTimeComboBox* pBuddy = (CTimeComboBox*)ctrl.GetBuddy(pParent);
					ASSERT_VALID(pBuddy);
				
					if (pBuddy == NULL)
						return TDCCAUIRES_FAIL;

					double dTime = pBuddy->GetOleTime();

					if (dTime > 0)
						date.m_dt += dTime;
				}
				data.Set(date);
			}
			break;
			
		case TDCCA_TIMEPERIOD:
			{
				TH_UNITS nUnits = ((CTimeEdit*)pCtrl)->GetUnits();
				double dTime = ((CTimeEdit*)pCtrl)->Convert();

				data.Set(TDCTIMEPERIOD(dTime, nUnits));
			}
			break;

		case TDCCA_BOOL:
			data.Set(((CButton*)pCtrl)->GetCheck() != 0);
			break;

		case TDCCA_ICON:
			// these don't have controls
			ASSERT(0);
			return TDCCAUIRES_FAIL;
		}
	}
	else if (IsCustomFilterControl(ctrl.nCtrlID))
	{
		ASSERT(pCtrl->IsKindOf(RUNTIME_CLASS(CEnCheckComboBox)));

		((CEnCheckComboBox*)pCtrl)->GetChecked(aItems);
		data.Set(aItems, bFilter);
	}
	else // it's a list
	{
		switch (dwListType)
		{
		case TDCCA_AUTOLIST:
			if (dwDataType == TDCCA_FILELINK)
			{
				((CFileComboBox*)pCtrl)->GetFileList(aItems);
				data.Set(aItems);
			}
			else
			{
				pCtrl->GetWindowText(sText);
				data.Set(sText);
			}
			break;
		
		case TDCCA_FIXEDLIST:
			if (dwDataType == TDCCA_ICON)
				sText = ((CTDLIconComboBox*)pCtrl)->GetSelectedImage();
			else
				pCtrl->GetWindowText(sText);

			data.Set(sText);
			break;
		
		case TDCCA_AUTOMULTILIST:
		case TDCCA_FIXEDMULTILIST:
			{
				const CCheckComboBox* pCombo = (CCheckComboBox*)pCtrl;
				CStringArray aExtra;

				pCombo->GetChecked(aItems);
				pCombo->GetChecked(aExtra, CCBC_MIXED);

				data.Set(aItems, aExtra);
			}
			break;
		}
	}

	return nRes;
}

CWnd* CTDCCustomAttributeUIHelper::CheckRecreateDateFilterBuddy(const CWnd* pParent, const CUSTOMATTRIBCTRLITEM& ctrl, FILTER_DATE nFilter,
																const CTDCCustomAttribDefinitionArray& aAttribDefs, BOOL& bCreated)
{
	CWnd* pBuddy = ctrl.GetBuddy(pParent);
	bCreated = FALSE;

	if (pBuddy)
	{
		BOOL bDeleteBuddy = FALSE;

		switch (nFilter)
		{
		case FD_USER:
			bDeleteBuddy = !pBuddy->IsKindOf(RUNTIME_CLASS(CDateTimeCtrlEx));
			break;

		case FD_NEXTNDAYS:
			bDeleteBuddy = !pBuddy->IsKindOf(RUNTIME_CLASS(CEnEdit));
			break;
		}

		if (bDeleteBuddy)
		{
			pBuddy->DestroyWindow();
			delete pBuddy;
			pBuddy = NULL;
		}
	}

	if (!pBuddy)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
		GET_DEF_RET(aAttribDefs, ctrl.sAttribID, pDef, NULL);

		pBuddy = CreateAttributeCtrl(const_cast<CWnd*>(pParent), 
									 *pDef,
									 TDCCADATA(nFilter),	// new type
									 CTDCImageList(),		// not required
									 ctrl.nBuddyCtrlID, 
									 TRUE,					// buddy
									 FALSE);				// multi-selection droplist

		bCreated = TRUE;
	}

	ASSERT_VALID(pBuddy);
	
	return pBuddy;
}

FILTER_DATE CTDCCustomAttributeUIHelper::GetDateFilter(const TDCCADATA& data, double& dUserVal)
{
	if (data.IsEmpty())
		return FD_ANY;

	CString sFilter(data.AsString()), sValue;

	if (Misc::Split(sFilter, sValue, '|') && !sValue.IsEmpty())
		dUserVal = _ttof(sValue);
	else
		dUserVal = 0.0;

	FILTER_DATE nFilter = (FILTER_DATE)_ttoi(sFilter);

	ValidateDateFilter(nFilter, dUserVal);

	return nFilter;
}

double CTDCCustomAttributeUIHelper::ValidateDateFilter(FILTER_DATE nFilter, double dUserVal)
{
	switch (nFilter)
	{
	case FD_NEXTNDAYS:
		if (dUserVal <= 0.0)
		{
			//ASSERT(0);
			return 7.0;
		}
		break;

	case FD_USER:
		if (dUserVal == 0.0)
		{
			//ASSERT(0);
			return COleDateTime::GetCurrentTime().m_dt;
		}
		break;

	default:
		ASSERT(dUserVal == 0.0);
		return 0.0;
	}

	return dUserVal;
}

void CTDCCustomAttributeUIHelper::SetDateFilter(FILTER_DATE nFilter, double dUserVal, TDCCADATA& data)
{
	dUserVal = ValidateDateFilter(nFilter, dUserVal);

	if (dUserVal != 0.0)
		data.Set(Misc::Format(_T("%d|%f"), nFilter, dUserVal));
	else
		data.Set(nFilter);
}

void CTDCCustomAttributeUIHelper::SetBuddyVisibility(const CWnd* pParent, CUSTOMATTRIBCTRLITEM& ctrl, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, const TDCCADATA& data)
{
	if (!attribDef.IsList())
	{
		switch (attribDef.GetDataType())
		{
		case TDCCA_DATE:
			if (IsCustomFilterControl(ctrl.nCtrlID))
			{
				switch (data.AsInteger())
				{
				case FD_NEXTNDAYS:
				case FD_USER:
					ctrl.ShowBuddy(pParent, TRUE);
					break;

				default:
					ctrl.ShowBuddy(pParent, FALSE);
					break;
				}
			}
			else
			{
				if (attribDef.HasFeature(TDCCAF_SHOWTIME))
					ctrl.ShowBuddy(pParent, TRUE);
			}
			break;

		default:
			ASSERT(!ctrl.HasBuddy());
			break;
		}
	}
}

void CTDCCustomAttributeUIHelper::DDX(CDataExchange* pDX, CTDCCustomControlArray& aControls,
									  const CTDCCustomAttribDefinitionArray& aAttribDefs,
									  CTDCCustomAttributeDataMap& mapData)
{
	if (pDX->m_bSaveAndValidate)
	{
		GetControlsData(pDX->m_pDlgWnd, aControls, aAttribDefs, mapData, mapData);
	}
	else
	{
		if (mapData.GetCount() == 0)
			CTDCCustomAttributeUIHelper::ClearControls(pDX->m_pDlgWnd, aControls, aAttribDefs);
		else
			CTDCCustomAttributeUIHelper::UpdateControls(pDX->m_pDlgWnd, aControls, aAttribDefs, mapData);
	}
}

void CTDCCustomAttributeUIHelper::ClearControl(const CWnd* pParent, CUSTOMATTRIBCTRLITEM& ctrl,
											const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	UpdateControl(pParent, ctrl, aAttribDefs, TDCCADATA());
}

void CTDCCustomAttributeUIHelper::UpdateControl(const CWnd* pParent, CUSTOMATTRIBCTRLITEM& ctrl,
											const CTDCCustomAttribDefinitionArray& aAttribDefs,
											const TDCCADATA& data)
{
	ASSERT_VALID(pParent);
	ASSERT(TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(ctrl.nAttrib));

	DWORD dwDataType = 0, dwListType = 0;
	VERIFY(GetControlAttributeTypes(ctrl, aAttribDefs, dwDataType, dwListType));

	BOOL bFilter = IsCustomFilterControl(ctrl.nCtrlID);
	CWnd* pCtrl = ctrl.GetCtrl(pParent);

	if (pCtrl == NULL)
	{
		ASSERT(0);
		return;
	}

	CStringArray aItems;

	if (dwListType == TDCCA_NOTALIST)
	{
		switch (dwDataType)
		{
		case TDCCA_STRING:
		case TDCCA_FILELINK:
			pCtrl->SetWindowText(data.AsString());
			break;

		case TDCCA_INTEGER:
			if (data.IsEmpty())
				pCtrl->SetWindowText(data.AsString());
			else
				pCtrl->SetWindowText(Misc::Format(data.AsInteger()));
			break;

		case TDCCA_FRACTION:
			if (data.IsEmpty())
			{
				pCtrl->SetWindowText(data.AsString());
			}
			else if (data.IsFraction())
			{
				CString sValue(data.AsString());
				Misc::TrimTrailingDecimalZeros(sValue);

				pCtrl->SetWindowText(sValue);
			}
			else
			{
				pCtrl->SetWindowText(Misc::Format(data.AsDouble()));
			}
			break;

		case TDCCA_DOUBLE:
			if (data.IsEmpty())
				pCtrl->SetWindowText(data.AsString());
			else
				pCtrl->SetWindowText(Misc::Format(data.AsDouble()));
			break;

		case TDCCA_DATE:
			if (bFilter)
			{
				double dUserVal;
				FILTER_DATE nFilter = GetDateFilter(data, dUserVal);

				((CTDLFilterDateComboBox*)pCtrl)->SelectFilter(nFilter);

				if (nFilter == FD_USER)
				{
					ASSERT(ctrl.HasBuddy());

					CDateTimeCtrlEx* pBuddy = (CDateTimeCtrlEx*)ctrl.GetBuddy(pParent);
					ASSERT_VALID(pBuddy);

					if (pBuddy)
						pBuddy->SetTime(dUserVal);
				}
				else if (nFilter == FD_NEXTNDAYS)
				{
					ASSERT(ctrl.HasBuddy());

					CWnd* pBuddy = ctrl.GetBuddy(pParent);
					ASSERT_VALID(pBuddy);

					if (pBuddy)
						pBuddy->SetWindowText(Misc::Format((int)dUserVal));

					((CTDLFilterDateComboBox*)pCtrl)->SetNextNDays((int)dUserVal);
				}
				else
				{
					ctrl.ShowBuddy(pParent, FALSE);
				}
			}
			else
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
					CTimeComboBox* pBuddy = (CTimeComboBox*)ctrl.GetBuddy(pParent);
					ASSERT_VALID(pBuddy);

					if (pBuddy)
						pBuddy->SetOleTime(date);
					else
						ASSERT(0);
				}
			}
			break;

		case TDCCA_TIMEPERIOD:
			{
				TDCTIMEPERIOD time;

				if (!data.AsTimePeriod(time))
					time.SetTime(0.0, TDCU_HOURS);

				((CTimeEdit*)pCtrl)->SetTime(time.dAmount, time.GetTHUnits());
			}
			break;

		case TDCCA_BOOL:
			((CButton*)pCtrl)->SetCheck(data.AsBool() ? 1 : 0);
			break;

		case TDCCA_ICON:
			// these don't have controls
			ASSERT(0);
			break;
		}
	}
	else if (IsCustomFilterControl(ctrl.nCtrlID))
	{
		ASSERT(pCtrl->IsKindOf(RUNTIME_CLASS(CEnCheckComboBox)));

		data.AsArray(aItems);
		((CEnCheckComboBox*)pCtrl)->SetChecked(aItems);
	}
	else
	{
		switch (dwListType)
		{
		case TDCCA_AUTOLIST:
			if (dwDataType == TDCCA_FILELINK)
			{
				data.AsArray(aItems);
				((CFileComboBox*)pCtrl)->SetFileList(aItems);
			}
			else
			{
				CAutoComboBox* pACB = (CAutoComboBox*)pCtrl;

				if (data.IsEmpty())
				{
					pACB->SetCurSel(-1);
				}
				else
				{
					int nItem = pACB->AddString(data.AsString());
					pACB->SetCurSel(nItem);
				}
			}
			break;

		case TDCCA_FIXEDLIST:
			{
				COwnerdrawComboBoxBase* pCB = (COwnerdrawComboBoxBase*)pCtrl;

				if (data.IsEmpty())
				{
					pCB->SetCurSel(-1);
				}
				else if (dwDataType == TDCCA_ICON)
				{
					((CTDLIconComboBox*)pCtrl)->SelectImage(data.AsString());
				}
				else
				{
					int nItem = pCB->FindStringExact(-1, data.AsString(), FALSE);
					pCB->SetCurSel(nItem);
				}
			}
			break;

		case TDCCA_AUTOMULTILIST:
		case TDCCA_FIXEDMULTILIST:
			{
				CStringArray aExtra;
				data.AsArrays(aItems, aExtra);

				((CCheckComboBox*)pCtrl)->SetChecked(aItems, aExtra);
			}
			break;
		}
	}
}

void CTDCCustomAttributeUIHelper::UpdateControlAutoListData(const CWnd* pParent, 
															const CUSTOMATTRIBCTRLITEM& ctrl,
															const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	int nAttribDef = aAttribDefs.Find(ctrl.sAttribID);

	if (nAttribDef == -1)
	{
		ASSERT(0);
		return;
	}

	const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs.GetData()[nAttribDef];

	if (!attribDef.IsList() || attribDef.IsDataType(TDCCA_FILELINK))
	{
		return;
	}
	
	CWnd* pCtrl = ctrl.GetCtrl(pParent);
	ASSERT_VALID(pCtrl);

	if (pCtrl && pCtrl->IsKindOf(RUNTIME_CLASS(CAutoComboBox)))
	{
		CStringArray aItems;
		attribDef.GetUniqueListData(aItems);

		((CAutoComboBox*)pCtrl)->SetStrings(aItems);
	}
}

BOOL CTDCCustomAttributeUIHelper::GetControlAttributeTypes(const CUSTOMATTRIBCTRLITEM& ctrl,
														 const CTDCCustomAttribDefinitionArray& aAttribDefs,
														 DWORD& dwDataType, DWORD& dwListType)
{
	ASSERT(IsCustomEditControl(ctrl.nCtrlID) || IsCustomFilterControl(ctrl.nCtrlID));

	if (!IsCustomEditControl(ctrl.nCtrlID) && !IsCustomFilterControl(ctrl.nCtrlID))
		return FALSE;

	// search attribute defs for unique ID
	const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
	GET_DEF_RET(aAttribDefs, ctrl.sAttribID, pDef, FALSE);
	
	dwDataType = pDef->GetDataType();
	dwListType = pDef->GetListType();

	return TRUE;
}

int CTDCCustomAttributeUIHelper::EnableMultiSelectionFilter(const CTDCCustomControlArray& aControls, 
															CWnd* pParent, BOOL bEnable)
{
	int nCtrl = aControls.GetSize(), nNumFound = 0;
	
	while (nCtrl--)
	{
		const CUSTOMATTRIBCTRLITEM& ctrl = aControls[nCtrl];

		CWnd* pCtrl = ctrl.GetCtrl(pParent);
		ASSERT_VALID(pCtrl);

		if (pCtrl && pCtrl->IsKindOf(RUNTIME_CLASS(CEnCheckComboBox)))
		{
			CEnCheckComboBox* pCombo = (CEnCheckComboBox*)pCtrl;

			pCombo->EnableMultiSelection(bEnable);
			nNumFound++;
		}
	}

	return nNumFound;
}

void CTDCCustomAttributeUIHelper::ClearFilterCheckboxHistory(const CTDCCustomControlArray& aControls, CWnd* pParent)
{
	int nCtrl = aControls.GetSize();
	
	while (nCtrl--)
		ClearFilterCheckboxHistory(aControls[nCtrl], pParent);
}

void CTDCCustomAttributeUIHelper::ClearFilterCheckboxHistory(const CUSTOMATTRIBCTRLITEM& ctrl, CWnd* pParent)
{
	CWnd* pCtrl = ctrl.GetCtrl(pParent);
	ASSERT_VALID(pCtrl);

	if (pCtrl && pCtrl->IsKindOf(RUNTIME_CLASS(CEnCheckComboBox)))
	{
		((CEnCheckComboBox*)pCtrl)->ClearMultiSelectionHistory();
	}
}

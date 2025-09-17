// TDCCustomAttributeHelper.cpp: implementation of the CTDCCustomAttributeUIHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Resource.h"
#include "TDCCustomFilterAttributeUIHelper.h"
#include "TDCstruct.h"
#include "tdliconcombobox.h"
#include "todoitem.h"
#include "TDLFilterDateComboBox.h"
#include "TDCImageList.h"

#include "..\shared\DialogHelper.h"
#include "..\shared\checkcombobox.h"
#include "..\shared\autocombobox.h"
#include "..\shared\wclassdefines.h"
#include "..\shared\enedit.h"
#include "..\shared\datetimectrlex.h"
#include "..\shared\misc.h"
#include "..\shared\localizer.h"
#include "..\shared\HoldRedraw.h"
#include "..\shared\Icon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWnd* CTDCCustomFilterAttributeUIHelper::CreateAttributeCtrl(CWnd* pParent, 
															 const TDCCUSTOMATTRIBUTEDEFINITION& attribDef,
															 const TDCCADATA& data,
															 const CTDCImageList& ilImages,
															 UINT nCtrlID,
															 BOOL bBuddy,
															 BOOL bMultiSelectionFilter)
{
	if (bBuddy && !AttributeWantsBuddy(attribDef))
	{
		ASSERT(0);
		return NULL;
	}
	
	CWnd* pControl = NULL;
	DWORD dwStyle = (WS_CHILD | WS_TABSTOP); // Invisible initially
	DWORD dwExStyle = WS_EX_CLIENTEDGE;
	LPCTSTR szClass = NULL;

	if (attribDef.IsList())
	{
		szClass = WC_COMBOBOX;
		dwStyle |= (CBS_DROPDOWNLIST | WS_VSCROLL | CBS_AUTOHSCROLL);

		if (!attribDef.IsFixedList())
			dwStyle |= CBS_SORT;

		if (attribDef.IsDataType(TDCCA_ICON))
			pControl = new CTDLIconComboBox(ilImages, bMultiSelectionFilter, TRUE);
		else
			pControl = new CEnCheckComboBox(bMultiSelectionFilter, IDS_TDC_NONE, IDS_TDC_ANY);
	}
	else if (attribDef.IsDataType(TDCCA_DATE))
	{
		if (bBuddy)
		{
			switch (data.AsInteger())
			{
			case FD_NEXTNDAYS:
				{
					CEnEdit* pEdit = new CEnEdit(_T("0123456789"));
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

	if (pControl == NULL)
	{
		// Unknown/unhandled data type
		ASSERT(0);
		return NULL;
	}

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

			CStringArray aListData;
			attribDef.GetUniqueListData(aListData);

			if (pControl->IsKindOf(RUNTIME_CLASS(CEnCheckComboBox)))
				((CEnCheckComboBox*)pCB)->SetStrings(aListData);
			else
				CDialogHelper::SetComboBoxItems(*pCB, aListData);
		}
		else if (pControl->IsKindOf(RUNTIME_CLASS(CDateTimeCtrlEx)))
		{
			((CDateTimeCtrlEx*)pControl)->SetMonthCalStyle(MCS_WEEKNUMBERS);
		}
	}

	return pControl;
}

BOOL CTDCCustomFilterAttributeUIHelper::AttributeWantsBuddy(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef)
{
	return (attribDef.GetDataType() == TDCCA_DATE);
}

CString CTDCCustomFilterAttributeUIHelper::GetControlLabel(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bBuddy)
{
	if (bBuddy)
		return _T("");

	//else
	return (attribDef.sLabel.IsEmpty() ? attribDef.sColumnTitle : attribDef.sLabel);
}

CWnd* CTDCCustomFilterAttributeUIHelper::CreateAttributeLabelCtrl(CWnd* pParent, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, 
																	const TDCCADATA& /*data*/, UINT nCtrlID, BOOL bBuddy)
{
	CStatic* pLabel = new CStatic;
	ASSERT (pLabel);

	if (pLabel)
	{
		DWORD dwStyle = (WS_CHILD | SS_CENTERIMAGE); // Invisible initially

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

BOOL CTDCCustomFilterAttributeUIHelper::GetControl(UINT nCtrlID, const CTDCCustomControlArray& aControls, 
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

void CTDCCustomFilterAttributeUIHelper::CleanupControls(CTDCCustomControlArray& aControls, CWnd* pParent)
{
	aControls.DeleteCtrls(pParent);
	aControls.RemoveAll();
}

BOOL CTDCCustomFilterAttributeUIHelper::NeedRebuildControls(const CTDCCustomAttribDefinitionArray& aOldAttribDefs,
															const CTDCCustomAttribDefinitionArray& aNewAttribDefs,
															const CTDCCustomControlArray& aOldControls)
{
	CTDCCustomControlArray aNewControls;
	int nNumNewCtrls = GetCustomAttributeCtrls(aNewAttribDefs, aNewControls);

	if (nNumNewCtrls != aOldControls.GetSize())
		return TRUE;

	// Compare each new item with the old, also checking list data
	// Order is important
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
		if (!attribNew.Matches(attribOld, TRUE))
			return TRUE;
	}

	return FALSE;
}

int CTDCCustomFilterAttributeUIHelper::GetCustomAttributeCtrls(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
																CTDCCustomControlArray& aControls)
{
	aControls.RemoveAll();

	UINT nID = IDC_FIRST_CUSTOMFILTERFIELD;

	for (int nAtt = 0; nAtt < aAttribDefs.GetSize(); nAtt++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs[nAtt];

		// don't add unwanted controls
		if (!WantControl(attribDef))
			continue;

		CUSTOMATTRIBCTRLITEM ctrl;

		ctrl.nAttributeID = attribDef.GetAttributeID();
		ctrl.sAttribID = attribDef.sUniqueID;

		// Main control
		ctrl.nCtrlID = nID++;
		ctrl.nLabelID = nID++;

		// Buddy control
		if (AttributeWantsBuddy(attribDef))
		{
			ctrl.nBuddyCtrlID = nID++;
			ctrl.nBuddyLabelID = nID++;
		}

		aControls.Add(ctrl);
	}

	return aControls.GetSize();
}

BOOL CTDCCustomFilterAttributeUIHelper::RebuildControls(CWnd* pParent, 
														const CTDCCustomAttribDefinitionArray& aAttribDefs,
														const CTDCCustomAttributeDataMap& mapCtrlData,
														const CTDCImageList& ilImages,
														UINT nCtrlIDPos,
														BOOL bMultiSelection,
														CTDCCustomControlArray& aControls)
{
	ASSERT_VALID(pParent);

	CHoldRedraw hr(*pParent);

	// remove all existing custom attribute fields
	CleanupControls(aControls, pParent);

	// recreate controls and columns
	UINT nID = IDC_FIRST_CUSTOMFILTERFIELD;

	const CWnd* pInsertAfter = pParent->GetDlgItem(nCtrlIDPos);
	ASSERT(pInsertAfter);

	for (int nAtt = 0; nAtt < aAttribDefs.GetSize(); nAtt++)
	{
		const TDCCUSTOMATTRIBUTEDEFINITION& attribDef = aAttribDefs[nAtt];

		// don't add unwanted controls
		if (!WantControl(attribDef))
			continue;
		
		CUSTOMATTRIBCTRLITEM ctrl;

		ctrl.nAttributeID = attribDef.GetAttributeID();
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

		pCtrl = CreateAttributeCtrl(pParent, attribDef, data, ilImages, ctrl.nCtrlID, FALSE, bMultiSelection);

		if (pCtrl)
			pLabel = CreateAttributeLabelCtrl(pParent, attribDef, data, ctrl.nLabelID, FALSE);

		// Buddy control
		BOOL bWantsBuddy = (pCtrl && pLabel && AttributeWantsBuddy(attribDef));

		if (bWantsBuddy)
		{
			ctrl.nBuddyCtrlID = nID++;
			ctrl.nBuddyLabelID = nID++;

			pBuddyCtrl = CreateAttributeCtrl(pParent, attribDef, data, ilImages, ctrl.nBuddyCtrlID, TRUE, bMultiSelection);

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

BOOL CTDCCustomFilterAttributeUIHelper::WantControl(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef/*, BOOL bFilter*/)
{
	return (attribDef.bEnabled && attribDef.HasFeature(TDCCAF_FILTER));
}

BOOL CTDCCustomFilterAttributeUIHelper::IsCustomControl(UINT nCtrlID)
{
	return ((nCtrlID >= IDC_FIRST_CUSTOMFILTERFIELD) && 
			(nCtrlID <= IDC_LAST_CUSTOMFILTERFIELD));
}

CString CTDCCustomFilterAttributeUIHelper::GetControlTooltip(const CWnd* pParent, UINT nCtrlID)
{
	ASSERT(IsCustomControl(nCtrlID));

	CWnd* pCtrl = pParent->GetDlgItem(nCtrlID);

	if (pCtrl && pCtrl->IsKindOf(RUNTIME_CLASS(CCheckComboBox)))
		return ((CCheckComboBox*)pCtrl)->GetTooltip();

	// else
	return _T("");
}

void CTDCCustomFilterAttributeUIHelper::UpdateControls(const CWnd* pParent, 
													   CTDCCustomControlArray& aControls,
													   const CTDCCustomAttribDefinitionArray& aAttribDefs,
													   const CTDCCustomAttributeDataMap& mapData)
{
	int nCtrl = aControls.GetSize();

	while (nCtrl--)
		UpdateControl(pParent, aControls.GetData()[nCtrl], aAttribDefs, mapData);
}

void CTDCCustomFilterAttributeUIHelper::UpdateControl(const CWnd* pParent, 
													  CUSTOMATTRIBCTRLITEM& ctrl,
													  const CTDCCustomAttribDefinitionArray& aAttribDefs,
													  const CTDCCustomAttributeDataMap& mapData)
{
	ASSERT(TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(ctrl.nAttributeID));

	TDCCADATA data;
		
	if (mapData.Lookup(ctrl.sAttribID, data))
		UpdateControl(pParent, ctrl, aAttribDefs, data);
	else
		ClearControl(pParent, ctrl, aAttribDefs);
}

void CTDCCustomFilterAttributeUIHelper::ClearControls(const CWnd* pParent, 
													  CTDCCustomControlArray& aControls,
													  const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	int nCtrl = aControls.GetSize();

	while (nCtrl--)
	{
		CUSTOMATTRIBCTRLITEM& ctrl = aControls[nCtrl];
		ClearControl(pParent, ctrl, aAttribDefs);
	}
}

TDCCAUI_UPDATERESULT CTDCCustomFilterAttributeUIHelper::GetControlData(const CWnd* pParent, CUSTOMATTRIBCTRLITEM& ctrl,
																	   const CTDCCustomAttribDefinitionArray& aAttribDefs,
																	   const TDCCADATA& dataPrev, TDCCADATA& data)
{
	ASSERT_VALID(pParent);
	ASSERT(TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(ctrl.nAttributeID));

	DWORD dwDataType = 0, dwListType = 0;
	VERIFY(GetControlAttributeTypes(ctrl, aAttribDefs, dwDataType, dwListType));

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
		if (dwDataType == TDCCA_DATE)
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
					CWnd* pBuddy = CheckRecreateDateBuddy(pParent, ctrl, nFilter, aAttribDefs, bCreated);

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
					CWnd* pBuddy = CheckRecreateDateBuddy(pParent, ctrl, nFilter, aAttribDefs, bCreated);

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

			if (Misc::StatesDiffer(bWasShowingBuddy, bShowBuddy))
				nRes = TDCCAUIRES_REPOSCTRLS;
		}
	}
	else
	{
		ASSERT(pCtrl->IsKindOf(RUNTIME_CLASS(CEnCheckComboBox)));

		((CEnCheckComboBox*)pCtrl)->GetChecked(aItems);
		data.Set(aItems, TRUE);
	}

	return nRes;
}

CWnd* CTDCCustomFilterAttributeUIHelper::CheckRecreateDateBuddy(const CWnd* pParent, 
																const CUSTOMATTRIBCTRLITEM& ctrl, 
																FILTER_DATE nFilter,
																const CTDCCustomAttribDefinitionArray& aAttribDefs, 
																BOOL& bCreated)
{
	ASSERT(IsCustomControl(ctrl.nCtrlID));

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
		GET_CUSTDEF_RET(aAttribDefs, ctrl.sAttribID, pDef, NULL);

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

FILTER_DATE CTDCCustomFilterAttributeUIHelper::GetDateFilter(const TDCCADATA& data, double& dUserVal)
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

double CTDCCustomFilterAttributeUIHelper::ValidateDateFilter(FILTER_DATE nFilter, double dUserVal)
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

void CTDCCustomFilterAttributeUIHelper::SetDateFilter(FILTER_DATE nFilter, double dUserVal, TDCCADATA& data)
{
	dUserVal = ValidateDateFilter(nFilter, dUserVal);

	if (dUserVal != 0.0)
		data.Set(Misc::Format(_T("%d|%f"), nFilter, dUserVal));
	else
		data.Set(nFilter);
}

void CTDCCustomFilterAttributeUIHelper::SetBuddyVisibility(const CWnd* pParent, CUSTOMATTRIBCTRLITEM& ctrl, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, const TDCCADATA& data)
{
	if (!attribDef.IsList())
	{
		switch (attribDef.GetDataType())
		{
		case TDCCA_DATE:
			if (IsCustomControl(ctrl.nCtrlID))
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

void CTDCCustomFilterAttributeUIHelper::ClearControl(const CWnd* pParent, CUSTOMATTRIBCTRLITEM& ctrl,
											const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	UpdateControl(pParent, ctrl, aAttribDefs, TDCCADATA());
}

void CTDCCustomFilterAttributeUIHelper::UpdateControl(const CWnd* pParent, 
													  CUSTOMATTRIBCTRLITEM& ctrl,
													  const CTDCCustomAttribDefinitionArray& aAttribDefs,
													  const TDCCADATA& data)
{
	ASSERT_VALID(pParent);
	ASSERT(TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(ctrl.nAttributeID));

	DWORD dwDataType = 0, dwListType = 0;
	VERIFY(GetControlAttributeTypes(ctrl, aAttribDefs, dwDataType, dwListType));

	CWnd* pCtrl = ctrl.GetCtrl(pParent);

	if (pCtrl == NULL)
	{
		ASSERT(0);
		return;
	}

	CStringArray aItems;

	if (dwListType == TDCCA_NOTALIST)
	{
		if (dwDataType == TDCCA_DATE)
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
	}
	else
	{
		ASSERT(pCtrl->IsKindOf(RUNTIME_CLASS(CEnCheckComboBox)));

		data.AsArray(aItems);
		((CEnCheckComboBox*)pCtrl)->SetChecked(aItems);
	}
}

void CTDCCustomFilterAttributeUIHelper::UpdateControlAutoListData(const CWnd* pParent, 
															const CUSTOMATTRIBCTRLITEM& ctrl,
															const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
	GET_CUSTDEF_ALT(aAttribDefs, ctrl.nAttributeID, pDef, return);

	if (pDef->IsList())
	{
		CWnd* pCtrl = ctrl.GetCtrl(pParent);
		ASSERT_VALID(pCtrl);

		if (pCtrl && pCtrl->IsKindOf(RUNTIME_CLASS(CAutoComboBox)))
		{
			CStringArray aItems;
			pDef->GetUniqueListData(aItems);

			((CAutoComboBox*)pCtrl)->SetStrings(aItems);
		}
	}
}

BOOL CTDCCustomFilterAttributeUIHelper::GetControlAttributeTypes(const CUSTOMATTRIBCTRLITEM& ctrl,
														 const CTDCCustomAttribDefinitionArray& aAttribDefs,
														 DWORD& dwDataType, DWORD& dwListType)
{
	const TDCCUSTOMATTRIBUTEDEFINITION* pDef = NULL;
	GET_CUSTDEF_RET(aAttribDefs, ctrl.sAttribID, pDef, FALSE);
	
	dwDataType = pDef->GetDataType();
	dwListType = pDef->GetListType();

	return TRUE;
}

int CTDCCustomFilterAttributeUIHelper::EnableMultiSelection(const CTDCCustomControlArray& aControls, 
															CWnd* pParent, 
															BOOL bEnable)
{
	int nCtrl = aControls.GetSize(), nNumFound = 0;
	
	while (nCtrl--)
	{
		CWnd* pCtrl = aControls[nCtrl].GetCtrl(pParent);
		ASSERT_VALID(pCtrl);

		if (pCtrl && pCtrl->IsKindOf(RUNTIME_CLASS(CEnCheckComboBox)))
		{
			((CEnCheckComboBox*)pCtrl)->EnableMultiSelection(bEnable);
			nNumFound++;
		}
	}

	return nNumFound;
}

void CTDCCustomFilterAttributeUIHelper::ClearCheckboxHistory(const CTDCCustomControlArray& aControls, CWnd* pParent)
{
	int nCtrl = aControls.GetSize();
	
	while (nCtrl--)
		ClearCheckboxHistory(aControls[nCtrl], pParent);
}

void CTDCCustomFilterAttributeUIHelper::ClearCheckboxHistory(const CUSTOMATTRIBCTRLITEM& ctrl, CWnd* pParent)
{
	CWnd* pCtrl = ctrl.GetCtrl(pParent);
	ASSERT_VALID(pCtrl);

	if (pCtrl && pCtrl->IsKindOf(RUNTIME_CLASS(CEnCheckComboBox)))
	{
		((CEnCheckComboBox*)pCtrl)->ClearMultiSelectionHistory();
	}
}

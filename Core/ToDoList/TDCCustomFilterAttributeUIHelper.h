// TDCCustomAttributeHelper.h: interface for the CTDCCustomAttributeUIHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCCUSTOMATTRIBUTEHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)
#define AFX_TDCCUSTOMATTRIBUTEHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdcstruct.h"

//////////////////////////////////////////////////////////////////////

class CTDCCustomAttribDefinitionArray;
class CTDCCustomControlArray;
class CTDCImageList;

struct TDCCUSTOMATTRIBUTEDEFINITION;

//////////////////////////////////////////////////////////////////////

enum TDCCAUI_UPDATERESULT
{
	TDCCAUIRES_FAIL = 0,
	TDCCAUIRES_SUCCESS,
	TDCCAUIRES_REPOSCTRLS,
};

//////////////////////////////////////////////////////////////////////

class CTDCCustomFilterAttributeUIHelper
{
public:
	static BOOL IsCustomControl(UINT nCtrlID);

	static BOOL RebuildControls(CWnd* pParent,
								const CTDCCustomAttribDefinitionArray& aAttribDefs,
								const CTDCCustomAttributeDataMap& mapCtrlData,
								const CTDCImageList& ilImages,
								UINT nCtrlIDPos,
								BOOL bMultiSelection,
								CTDCCustomControlArray& aControls);

	static BOOL NeedRebuildControls(const CTDCCustomAttribDefinitionArray& aOldAttribDefs,
									const CTDCCustomAttribDefinitionArray& aNewAttribDefs,
									const CTDCCustomControlArray& aOldControls);

	static int EnableMultiSelection(const CTDCCustomControlArray& aControls, CWnd* pParent, BOOL bEnable = TRUE);
	static void CleanupControls(CTDCCustomControlArray& aControls, CWnd* pParent);
	static void ClearCheckboxHistory(const CTDCCustomControlArray& aControls, CWnd* pParent);
	static void ClearCheckboxHistory(const CUSTOMATTRIBCTRLITEM& ctrl, CWnd* pParent);
	static CString GetControlTooltip(const CWnd* pParent, UINT nCtrlID);

	static BOOL GetControl(UINT nCtrlID,
						   const CTDCCustomControlArray& aControls,
						   CUSTOMATTRIBCTRLITEM& ctrl);

	static void UpdateControl(const CWnd* pParent, 
							  CUSTOMATTRIBCTRLITEM& ctrl,
							  const CTDCCustomAttribDefinitionArray& aAttribDefs,
							  const TDCCADATA& data);

	static void UpdateControlAutoListData(const CWnd* pParent, 
										  const CUSTOMATTRIBCTRLITEM& ctrl,
										  const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static void ClearControl(const CWnd* pParent,
							 CUSTOMATTRIBCTRLITEM& ctrl,
							 const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static void UpdateControls(const CWnd* pParent,
							   CTDCCustomControlArray& aControls,
							   const CTDCCustomAttribDefinitionArray& aAttribDefs,
							   const CTDCCustomAttributeDataMap& mapData);

	static void UpdateControl(const CWnd* pParent,
							  CUSTOMATTRIBCTRLITEM& ctrl,
							  const CTDCCustomAttribDefinitionArray& aAttribDefs,
							  const CTDCCustomAttributeDataMap& mapData);

	static void ClearControls(const CWnd* pParent, 
							  CTDCCustomControlArray& aControls,
							  const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static BOOL GetControlAttributeTypes(const CUSTOMATTRIBCTRLITEM& ctrl,
										 const CTDCCustomAttribDefinitionArray& aAttribDefs,
										 DWORD& dwDataType, 
										 DWORD& dwListType);

	static TDCCAUI_UPDATERESULT GetControlData(const CWnd* pParent, CUSTOMATTRIBCTRLITEM& ctrl,
											   const CTDCCustomAttribDefinitionArray& aAttribDefs,
											   const TDCCADATA& dataPrev, TDCCADATA& data);
protected:
	static CWnd* CreateAttributeCtrl(CWnd* pParent, 
									 const TDCCUSTOMATTRIBUTEDEFINITION& attribDef,
									 const TDCCADATA& data,
									 const CTDCImageList& ilImages,
									 UINT nCtrlID, 
									 BOOL bBuddy, 
									 BOOL bMultiSelectionFilter);

	static CWnd* CreateAttributeLabelCtrl(CWnd* pParent, 
										  const TDCCUSTOMATTRIBUTEDEFINITION& attribDef,
										  const TDCCADATA& data, 
										  UINT nCtrlID, 
										  BOOL bBuddy);

	static int GetCustomAttributeCtrls(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
									   CTDCCustomControlArray& aControls);

	static BOOL AttributeWantsBuddy(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef);
	static CString GetControlLabel(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bBuddy);
	static BOOL WantControl(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef);
	static FILTER_DATE GetDateFilter(const TDCCADATA& data, double& dUserVal);
	static void SetDateFilter(FILTER_DATE nFilter, double dUserVal, TDCCADATA& data);
	static double ValidateDateFilter(FILTER_DATE nFilter, double dUserVal);
	static void SetBuddyVisibility(const CWnd* pParent, CUSTOMATTRIBCTRLITEM& ctrl, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, const TDCCADATA& data);

	static CWnd* CheckRecreateDateBuddy(const CWnd* pParent, 
										const CUSTOMATTRIBCTRLITEM& ctrl, 
										FILTER_DATE nFilter, 
										const CTDCCustomAttribDefinitionArray& aAttribDefs, BOOL& bCreated);
};

#endif // !defined(AFX_TDCCUSTOMATTRIBUTEHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)

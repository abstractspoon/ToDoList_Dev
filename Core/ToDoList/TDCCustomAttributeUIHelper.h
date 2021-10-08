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
class CWndPromptManager;

struct TDCCUSTOMATTRIBUTEDEFINITION;

//////////////////////////////////////////////////////////////////////

enum TDCCAUI_UPDATERESULT
{
	TDCCAUIRES_FAIL = 0,
	TDCCAUIRES_SUCCESS,
	TDCCAUIRES_REPOSCTRLS,
};

//////////////////////////////////////////////////////////////////////

class CTDCCustomAttributeUIHelper
{
public:
	static BOOL RebuildEditControls(CWnd* pParent, 
									const CTDCCustomAttribDefinitionArray& aAttribDefs,
									const CTDCImageList& ilImages,
									UINT nCtrlIDPos,
									CTDCCustomControlArray& aControls);

	static BOOL RebuildFilterControls(CWnd* pParent, 
									  const CTDCCustomAttribDefinitionArray& aAttribDefs,
									  const CTDCCustomAttributeDataMap& mapCtrlData,
									  const CTDCImageList& ilImages,
									  UINT nCtrlIDPos,
									  BOOL bMultiSelection,
									  CTDCCustomControlArray& aControls);

	static BOOL NeedRebuildEditControls(const CTDCCustomAttribDefinitionArray& aOldAttribDefs,
										const CTDCCustomAttribDefinitionArray& aNewAttribDefs,
										const CTDCCustomControlArray& aOldControls);

	static BOOL NeedRebuildFilterControls(const CTDCCustomAttribDefinitionArray& aOldAttribDefs,
										  const CTDCCustomAttribDefinitionArray& aNewAttribDefs,
										  const CTDCCustomControlArray& aOldControls);

	static void CleanupControls(CTDCCustomControlArray& aControls, CWnd* pParent);
	static void AddWindowPrompts(const CTDCCustomControlArray& aControls, CWnd* pParent, CWndPromptManager& mgrPrompts);

	static int EnableMultiSelectionFilter(const CTDCCustomControlArray& aControls, CWnd* pParent, BOOL bEnable = TRUE);
	static void ClearFilterCheckboxHistory(const CTDCCustomControlArray& aControls, CWnd* pParent);
	static void ClearFilterCheckboxHistory(const CUSTOMATTRIBCTRLITEM& ctrl, CWnd* pParent);

	static BOOL FlushEditControl(CWnd* pCtrl, const CWnd* pParent, const CTDCCustomControlArray& aControls);

	static BOOL IsCustomEditControl(UINT nCtrlID);
	static BOOL IsCustomFilterControl(UINT nCtrlID);

	static CString GetFilterControlTooltip(const CWnd* pParent, UINT nCtrlID);
	static CString GetEditControlTooltip(const CWnd* pParent, UINT nCtrlID);

	static void SaveAutoListDataToDefs(const CWnd* pParent, const CTDCCustomControlArray& aControls,
									   const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static void SaveAutoListDataToDef(const CWnd* pParent, const CUSTOMATTRIBCTRLITEM& ctrl,
									   const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static BOOL GetControl(UINT nCtrlID, const CTDCCustomControlArray& aControls, CUSTOMATTRIBCTRLITEM& ctrl);
	static BOOL GetControl(const CString& sUniqueID, const CTDCCustomControlArray& aControls, CUSTOMATTRIBCTRLITEM& ctrl);

	static void UpdateControl(const CWnd* pParent, CUSTOMATTRIBCTRLITEM& ctrl,
							  const CTDCCustomAttribDefinitionArray& aAttribDefs,
							  const TDCCADATA& data);

	static void UpdateControlAutoListData(const CWnd* pParent, const CUSTOMATTRIBCTRLITEM& ctrl,
										  const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static void ClearControl(const CWnd* pParent, CUSTOMATTRIBCTRLITEM& ctrl,
							 const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static void UpdateControls(const CWnd* pParent,
							   CTDCCustomControlArray& aControls,
							   const CTDCCustomAttribDefinitionArray& aAttribDefs,
							   const CTDCCustomAttributeDataMap& mapData);

	static void UpdateControl(const CWnd* pParent, 
								CUSTOMATTRIBCTRLITEM& ctrl,
								const CTDCCustomAttribDefinitionArray& aAttribDefs,
								const CTDCCustomAttributeDataMap& mapData);

	static void ClearControls(const CWnd* pParent, CTDCCustomControlArray& aControls,
							  const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static BOOL GetControlAttributeTypes(const CUSTOMATTRIBCTRLITEM& ctrl,
										 const CTDCCustomAttribDefinitionArray& aAttribDefs,
										 DWORD& dwDataType, DWORD& dwListType);

	static TDCCAUI_UPDATERESULT GetControlsData(const CWnd* pParent, CTDCCustomControlArray& aControls,
												const CTDCCustomAttribDefinitionArray& aAttribDefs,
												const CTDCCustomAttributeDataMap& mapDataPrev,
												CTDCCustomAttributeDataMap& mapData);

	static TDCCAUI_UPDATERESULT GetControlData(const CWnd* pParent, CUSTOMATTRIBCTRLITEM& ctrl,
											   const CTDCCustomAttribDefinitionArray& aAttribDefs,
											   const TDCCADATA& dataPrev, TDCCADATA& data);

	static void DDX(CDataExchange* pDX, CTDCCustomControlArray& aControls,
					const CTDCCustomAttribDefinitionArray& aAttribDefs, 
					CTDCCustomAttributeDataMap& value);

protected:
	static CWnd* CreateAttributeCtrl(CWnd* pParent, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef,
									 const TDCCADATA& data,
									 const CTDCImageList& ilImages,
									 UINT nCtrlID, BOOL bBuddy, BOOL bMultiSelectionFilter);

	static CWnd* CreateAttributeLabelCtrl(CWnd* pParent, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef,
										  const TDCCADATA& data, UINT nCtrlID, BOOL bBuddy);

	static CWnd* GetControlFromAttributeDef(const CWnd* pParent, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef,
											const CTDCCustomControlArray& aControls);

	static BOOL AttributeWantsBuddy(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bFilter);
	static CString GetControlLabel(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bBuddy, BOOL bFilter);

	static BOOL RebuildControls(CWnd* pParent, 
								const CTDCCustomAttribDefinitionArray& aAttribDefs,
								const CTDCCustomAttributeDataMap& mapCtrlData,
								const CTDCImageList& ilImages,
								UINT nCtrlIDPos, UINT nCtrlIDStart,
								BOOL bFilter, BOOL bMultiSelectionFilter,
								CTDCCustomControlArray& aControls);

	static BOOL NeedRebuildControls(const CTDCCustomAttribDefinitionArray& aOldAttribDefs,
									const CTDCCustomAttribDefinitionArray& aNewAttribDefs,
									const CTDCCustomControlArray& aOldControls, UINT nCtrlIDStart);

	static int GetCustomAttributeCtrls(const CTDCCustomAttribDefinitionArray& aAttribDefs, UINT nCtrlIDStart, 
									   CTDCCustomControlArray& aControls);

	static BOOL WantControl(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bFilter);

	static CString GetControlTooltip(const CWnd* pParent, UINT nCtrlID);
	static FILTER_DATE GetDateFilter(const TDCCADATA& data, double& dUserVal);
	static void SetDateFilter(FILTER_DATE nFilter, double dUserVal, TDCCADATA& data);
	static double ValidateDateFilter(FILTER_DATE nFilter, double dUserVal);
	static CWnd* CheckRecreateDateFilterBuddy(const CWnd* pParent, const CUSTOMATTRIBCTRLITEM& ctrl, FILTER_DATE nFilter, 
											  const CTDCCustomAttribDefinitionArray& aAttribDefs, BOOL& bCreated);
	static void SetBuddyVisibility(const CWnd* pParent, CUSTOMATTRIBCTRLITEM& ctrl, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, const TDCCADATA& data);
};

#endif // !defined(AFX_TDCCUSTOMATTRIBUTEHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)

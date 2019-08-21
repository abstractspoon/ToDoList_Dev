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

class CTDCCustomAttributeUIHelper
{
public:
	static BOOL RebuildEditControls(const CTDCCustomAttribDefinitionArray& aAttribDefs,
									const CTDCImageList& ilImages,
									CWnd* pParent, 
									UINT nCtrlIDPos,
									CTDCCustomControlArray& aControls);

	static BOOL RebuildFilterControls(const CTDCCustomAttribDefinitionArray& aAttribDefs,
									  const CTDCImageList& ilImages,
									  CWnd* pParent, 
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

	static BOOL FlushEditControl(CWnd* pCtrl, CWnd* pParent,
								 const CTDCCustomControlArray& aControls);

	static BOOL IsCustomEditControl(UINT nCtrlID);
	static BOOL IsCustomFilterControl(UINT nCtrlID);

	static CString GetFilterControlTooltip(UINT nCtrlID, CWnd* pParent);
	static CString GetEditControlTooltip(UINT nCtrlID, CWnd* pParent);

	static void SaveAutoListDataToDefs(const CWnd* pParent, const CTDCCustomControlArray& aControls,
									   const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static BOOL GetControl(UINT nCtrlID, const CTDCCustomControlArray& aControls, CUSTOMATTRIBCTRLITEM& ctrl);
	static BOOL GetControl(const CString& sUniqueID, const CTDCCustomControlArray& aControls, CUSTOMATTRIBCTRLITEM& ctrl);

	static void UpdateControl(const CWnd* pParent, const CUSTOMATTRIBCTRLITEM& ctrl,
							  const CTDCCustomAttribDefinitionArray& aAttribDefs,
							  const TDCCADATA& data);

	static void ClearControl(const CWnd* pParent, const CUSTOMATTRIBCTRLITEM& ctrl,
							 const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static void UpdateControls(const CWnd* pParent,
							   const CTDCCustomControlArray& aControls,
							   const CTDCCustomAttribDefinitionArray& aAttribDefs,
							   const CTDCCustomAttributeDataMap& mapData);

	static void ClearControls(const CWnd* pParent, const CTDCCustomControlArray& aControls,
							  const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static BOOL GetControlAttributeTypes(const CUSTOMATTRIBCTRLITEM& ctrl,
										 const CTDCCustomAttribDefinitionArray& aAttribDefs,
										 DWORD& dwDataType, DWORD& dwListType);

	static BOOL GetControlData(const CWnd* pParent, const CUSTOMATTRIBCTRLITEM& ctrl,
							   const CTDCCustomAttribDefinitionArray& aAttribDefs,
							   TDCCADATA& data);

	static BOOL GetControlData(const CWnd* pParent, const CTDCCustomControlArray& aControls,
							   const CTDCCustomAttribDefinitionArray& aAttribDefs,
							   CTDCCustomAttributeDataMap& mapData);

	static void DDX(CDataExchange* pDX, const CTDCCustomControlArray& aControls,
					const CTDCCustomAttribDefinitionArray& aAttribDefs, CTDCCustomAttributeDataMap& value);

	// not currently used
	static CWnd* GetControlFromAttributeDef(const CWnd* pParent, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef,
											const CTDCCustomControlArray& aControls);

protected:
	static CWnd* CreateAttributeCtrl(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef,
								 const CTDCImageList& ilImages, CWnd* pParent,
								 UINT nCtrlID, BOOL bBuddy, BOOL bFilter, BOOL bMultiSelectionFilter);

	static CWnd* CreateAttributeLabelCtrl(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef,
									  CWnd* pParent, UINT nCtrlID, BOOL bBuddy);

	static BOOL AttributeWantsBuddy(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef);
	static CString GetControlLabel(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bBuddy);

	static BOOL RebuildControls(const CTDCCustomAttribDefinitionArray& aAttribDefs,
								const CTDCImageList& ilImages, CWnd* pParent,
								UINT nCtrlIDPos, UINT nCtrlIDStart,
								BOOL bFilter, BOOL bMultiSelectionFilter,
								CTDCCustomControlArray& aControls);
	static BOOL NeedRebuildControls(const CTDCCustomAttribDefinitionArray& aOldAttribDefs,
									const CTDCCustomAttribDefinitionArray& aNewAttribDefs,
									const CTDCCustomControlArray& aOldControls, UINT nCtrlIDStart, BOOL bFilter);
	static int GetCustomAttributeCtrls(const CTDCCustomAttribDefinitionArray& aAttribDefs,
									   CTDCCustomControlArray& aControls, UINT nCtrlIDStart, BOOL bFilter);
	static BOOL WantControl(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bFilter);
	static CString GetControlTooltip(UINT nCtrlID, CWnd* pParent);
};

#endif // !defined(AFX_TDCCUSTOMATTRIBUTEHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)

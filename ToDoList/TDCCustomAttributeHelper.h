// TDCCustomAttributeHelper.h: interface for the CTDCCustomAttributeHelper class.
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

//////////////////////////////////////////////////////////////////////

class CTDCCustomAttributeHelper  
{
public:
	static BOOL RebuildEditControls(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
									CTDCCustomControlArray& aControls, 
									const CTDCImageList& ilImages, 
									CWnd* pParent, UINT nCtrlIDPos);
	static BOOL RebuildFilterControls(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
									CTDCCustomControlArray& aControls, 
									const CTDCImageList& ilImages, 
									CWnd* pParent, UINT nCtrlIDPos,
									BOOL bMultiSelection);

	static BOOL NeedRebuildEditControls(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
										const CTDCCustomControlArray& aControls);
	static BOOL NeedRebuildFilterControls(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
											const CTDCCustomControlArray& aControls);

	static void CleanupControls(CTDCCustomControlArray& aControls, CWnd* pParent);

	// -------------------------------------------------------------------------

	static int EnableMultiSelectionFilter(const CTDCCustomControlArray& aControls, CWnd* pParent, BOOL bEnable = TRUE);
	
	static CString GetAttributeTypeID(UINT nCtrlID, const CTDCCustomControlArray& aControls);
	static CString GetAttributeTypeID(TDC_COLUMN nColID, const CTDCCustomAttribDefinitionArray& aAttribDefs);
	static CString GetAttributeTypeID(TDC_ATTRIBUTE nAttribID, const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static BOOL IsCustomAttribute(TDC_ATTRIBUTE nAttribID);
	static BOOL IsCustomAttributeEnabled(TDC_ATTRIBUTE nAttribID, const CTDCCustomAttribDefinitionArray& aAttribDefs);
	static BOOL IsCustomColumn(TDC_COLUMN nColID);
	static BOOL IsCustomColumnEnabled(TDC_COLUMN nColID, const CTDCCustomAttribDefinitionArray& aAttribDefs);
	static BOOL IsCustomEditControl(UINT nCtrlID);
	static BOOL IsCustomFilterControl(UINT nCtrlID);

	static BOOL GetAttributeDef(TDC_COLUMN nColID, 
								const CTDCCustomAttribDefinitionArray& aAttribDefs,
								TDCCUSTOMATTRIBUTEDEFINITION& attribDef);

	static BOOL GetAttributeDef(TDC_ATTRIBUTE nAttribID, 
								const CTDCCustomAttribDefinitionArray& aAttribDefs,
								TDCCUSTOMATTRIBUTEDEFINITION& attribDef);

	static BOOL GetAttributeDef(const CUSTOMATTRIBCTRLITEM& ctrl, 
								const CTDCCustomAttribDefinitionArray& aAttribDefs,
								TDCCUSTOMATTRIBUTEDEFINITION& attribDef);

	static BOOL GetAttributeDef(const CString& sUniqueID, 
								const CTDCCustomAttribDefinitionArray& aAttribDefs,
								TDCCUSTOMATTRIBUTEDEFINITION& attribDef);

	static DWORD GetAttributeDataType(const CString& sUniqueID, 
										const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static CString FormatData(const TDCCADATA& data, const CString& sUniqueID, 
								const CTDCCustomAttribDefinitionArray& aAttribDefs);
	static CString FormatData(const TDCCADATA& data, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef);

	static TDC_ATTRIBUTE GetAttributeID(TDC_COLUMN nColID, 
										const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static TDC_ATTRIBUTE GetAttributeID(const CString& sUniqueID, 
										const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static BOOL IsColumnSortable(TDC_COLUMN nColID, 
								const CTDCCustomAttribDefinitionArray& aAttribDefs);
	
	// not currently used
	static CWnd* GetControlFromAttributeDef(const CWnd* pParent, const TDCCUSTOMATTRIBUTEDEFINITION& attribDef,
											const CTDCCustomControlArray& aControls);

	static void SaveAutoListDataToDefs(const CWnd* pParent, const CTDCCustomControlArray& aControls, 
										const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static BOOL GetControl(UINT nCtrlID, const CTDCCustomControlArray& aControls, CUSTOMATTRIBCTRLITEM& ctrl);
	static BOOL GetControl(const CString& sUniqueID, const CTDCCustomControlArray& aControls, CUSTOMATTRIBCTRLITEM& ctrl);

	static void UpdateControl(const CWnd* pParent, const CUSTOMATTRIBCTRLITEM& ctrl,
								const CTDCCustomAttribDefinitionArray& aAttribDefs,
								const TDCCADATA& data);

	static void ClearControl(const CWnd* pParent, const CUSTOMATTRIBCTRLITEM& ctrl,
								const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static void UpdateControls(const CWnd* pParent, CTDCCustomControlArray& aControls,
								const CTDCCustomAttribDefinitionArray& aAttribDefs,
								const CTDCCustomAttributeDataMap& mapData);

	static void ClearControls(const CWnd* pParent, CTDCCustomControlArray& aControls,
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

	static FIND_ATTRIBTYPE GetAttributeFindType(const CString& sUniqueID, BOOL bRelativeDate,
												const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static FIND_ATTRIBTYPE GetAttributeFindType(TDC_ATTRIBUTE nAttribID, BOOL bRelativeDate,
												const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static void ValidateAttributeFindOperator(const SEARCHPARAM& sp, 
											const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static BOOL AppendFilterRules(const CTDCCustomAttributeDataMap& mapData, 
								const CTDCCustomAttribDefinitionArray& aAttribDefs, CSearchParamArray& aRules);

protected:
	static CWnd* CreateAttribute(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, 
								const CTDCImageList& ilImages, CWnd* pParent, 
								UINT nCtrlID, BOOL bBuddy, BOOL bFilter, BOOL bMultiSelectionFilter);
	
	static CWnd* CreateAttributeLabel(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, 
										CWnd* pParent, UINT nCtrlID, BOOL bBuddy);
	
	static BOOL AttributeWantsBuddy(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef);
	static CString GetControlLabel(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bBuddy);

	static BOOL RebuildControls(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
								CTDCCustomControlArray& aControls, 
								const CTDCImageList& ilImages, CWnd* pParent, 
								UINT nCtrlIDPos, UINT nCtrlIDStart, 
								BOOL bFilter, BOOL bMultiSelectionFilter);
	static BOOL NeedRebuildControls(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
									const CTDCCustomControlArray& aControls, UINT nCtrlIDStart, BOOL bFilter);
	static int GetCustomAttributeCtrls(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
									CTDCCustomControlArray& aControls, UINT nCtrlIDStart, BOOL bFilter);
	static BOOL WantControl(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bFilter);
};

#endif // !defined(AFX_TDCCUSTOMATTRIBUTEHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)

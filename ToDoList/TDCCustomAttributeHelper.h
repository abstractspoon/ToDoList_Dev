// TDCCustomAttributeHelper.h: interface for the CTDCCustomAttributeHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCCUSTOMATTRIBUTEHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)
#define AFX_TDCCUSTOMATTRIBUTEHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdcstruct.h"

class CTDCCustomAttribDefinitionArray;
class CTDCCustomControlArray;
class CTDCImageList;

class CTDCCustomAttributeHelper  
{
public:
	static BOOL RebuildCustomAttributeUI(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
										CTDCCustomControlArray& aControls, 
										const CTDCImageList& ilImages, CWnd* pParent, UINT nCtrlIDPos);

	static void CleanupCustomAttributeUI(CTDCCustomControlArray& aControls, CWnd* pParent);
	
	static CString GetAttributeTypeID(UINT nCtrlID, const CTDCCustomControlArray& aControls);
	static CString GetAttributeTypeID(TDC_COLUMN nColID, const CTDCCustomAttribDefinitionArray& aAttribDefs);
	static CString GetAttributeTypeID(TDC_ATTRIBUTE nAttribID, const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static BOOL IsCustomAttribute(TDC_ATTRIBUTE nAttribID);
	static BOOL IsCustomAttributeEnabled(TDC_ATTRIBUTE nAttribID, const CTDCCustomAttribDefinitionArray& aAttribDefs);
	static BOOL IsCustomColumn(TDC_COLUMN nColID);
	static BOOL IsCustomColumnEnabled(TDC_COLUMN nColID, const CTDCCustomAttribDefinitionArray& aAttribDefs);
	static BOOL IsCustomControl(UINT nCtrlID);

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

	static CString FormatData(const CString& sData, const CString& sUniqueID, 
								const CTDCCustomAttribDefinitionArray& aAttribDefs);

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

	static void UpdateCustomAttributeControl(const CWnd* pParent, const CUSTOMATTRIBCTRLITEM& ctrl,
											const CTDCCustomAttribDefinitionArray& aAttribDefs,
											const CString& sData);

	static void UpdateCustomAttributeControls(const CWnd* pParent, CTDCCustomControlArray& aControls,
											const CTDCCustomAttribDefinitionArray& aAttribDefs,
											const CMapStringToString& mapData);

	static void ClearCustomAttributeControls(const CWnd* pParent, CTDCCustomControlArray& aControls,
											const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static BOOL GetControlAttributeTypes(const CUSTOMATTRIBCTRLITEM& ctrl,
										const CTDCCustomAttribDefinitionArray& aAttribDefs,
										DWORD& dwDataType, DWORD& dwListType);

	static CString GetControlData(const CWnd* pParent, const CUSTOMATTRIBCTRLITEM& ctrl,
									const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static BOOL GetControlData(const CWnd* pParent, const CTDCCustomControlArray& aControls,
								const CTDCCustomAttribDefinitionArray& aAttribDefs,
								CMapStringToString& mapData);

	static FIND_ATTRIBTYPE GetAttributeFindType(const CString& sUniqueID, BOOL bRelativeDate,
											const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static FIND_ATTRIBTYPE GetAttributeFindType(TDC_ATTRIBUTE nAttribID, BOOL bRelativeDate,
											const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static void ValidateAttributeFindOperator(const SEARCHPARAM& sp, 
											const CTDCCustomAttribDefinitionArray& aAttribDefs);

protected:
	static CWnd* CreateCustomAttribute(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, 
										const CTDCImageList& ilImages,
										CWnd* pParent, UINT nCtrlID, 
										BOOL bVisible, BOOL bBuddy);
	
	static CWnd* CreateCustomAttributeLabel(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, 
											CWnd* pParent, UINT nCtrlID, 
											BOOL bVisible, BOOL bBuddy);
	
	static BOOL AttributeWantsBuddy(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef);
	static CString GetControlLabel(const TDCCUSTOMATTRIBUTEDEFINITION& attribDef, BOOL bBuddy);
};

#endif // !defined(AFX_TDCCUSTOMATTRIBUTEHELPER_H__4044B3B7_1EA0_4279_9620_F2035DAE87DF__INCLUDED_)

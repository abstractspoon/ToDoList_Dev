// TDCOutlookImportHelper.h: interface for the CTDCOutlookImportHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCOUTLOOKIMPORTHELPER_H__5B04111B_AD60_4EE0_A8B3_7E7D93772BC4__INCLUDED_)
#define AFX_TDCOUTLOOKIMPORTHELPER_H__5B04111B_AD60_4EE0_A8B3_7E7D93772BC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Interfaces\ITaskList.h"

//////////////////////////////////////////////////////////////////////

class CTDCAttributeMapping;
class TODOITEM;

namespace OutlookAPI
{
	class _Item;
}

struct TLDT_DATA;

//////////////////////////////////////////////////////////////////////

class CTDCOutlookImportHelper  
{
public:
	static BOOL ImportTask(const CTDCAttributeMapping& aMapping, OutlookAPI::_Item* pItem, BOOL bWantConfidential, ITaskList* pTasks, BOOL bWantAttrib = TRUE);
	static int ImportTasks(const TLDT_DATA* pData, UINT nIDMappingError, ITaskList* pTasks);

protected:
	static BOOL ImportTask(const CTDCAttributeMapping& aMapping, OutlookAPI::_Item* pItem, BOOL bWantConfidential, TODOITEM& tdi);
	static BOOL SetTaskAttributes(ITaskList* pTasks, HTASKITEM hTask, const TODOITEM& tdi);
	static double ImportDuration(const CString& sData, TDC_UNITS& nUnits);
};

#endif // !defined(AFX_TDCOUTLOOKIMPORTHELPER_H__5B04111B_AD60_4EE0_A8B3_7E7D93772BC4__INCLUDED_)

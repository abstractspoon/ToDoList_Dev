// TDCSearchParamHelper.h: interface for the CTDCSearchParamHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCSEARCHPARAMHELPER_H__9355E272_1290_4E1A_ABDD_0A576445BFFA__INCLUDED_)
#define AFX_TDCSEARCHPARAMHELPER_H__9355E272_1290_4E1A_ABDD_0A576445BFFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdcenum.h"

// predecs
class CPreferences;
class CTDCCustomAttribDefinitionArray;
class CTDCCustomAttributeDataMap;
class CSearchParamArray;

struct SEARCHPARAM;


class CTDCSearchParamHelper  
{
public:
	static BOOL LoadRule(const CPreferences& prefs, const CString& sRule,
						const CTDCCustomAttribDefinitionArray& aCustAttribDefs, SEARCHPARAM& rule);

	static BOOL SaveRule(CPreferences& prefs, const CString& sRule, const SEARCHPARAM& rule);

	static FIND_ATTRIBTYPE GetAttributeFindType(const CString& sUniqueID, BOOL bRelativeDate,
												const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static FIND_ATTRIBTYPE GetAttributeFindType(TDC_ATTRIBUTE nAttribID, BOOL bRelativeDate,
												const CTDCCustomAttribDefinitionArray& aAttribDefs);

	static BOOL AppendFilterRules(const CTDCCustomAttributeDataMap& mapData,
								  const CTDCCustomAttribDefinitionArray& aAttribDefs, CSearchParamArray& aRules);

protected:
	static BOOL DecodeAttribute(const CString& sAttrib, DWORD dwFlags,
								const CTDCCustomAttribDefinitionArray& aCustAttribDefs, 
								TDC_ATTRIBUTE& nAttrib, CString& sUniqueID, FIND_ATTRIBTYPE& nFindType);
};

#endif // !defined(AFX_TDCSEARCHPARAMHELPER_H__9355E272_1290_4E1A_ABDD_0A576445BFFA__INCLUDED_)

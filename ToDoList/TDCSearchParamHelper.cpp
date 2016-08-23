// TDCSearchParamHelper.cpp: implementation of the CTDCSearchParamHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCSearchParamHelper.h"
#include "TDCCustomattributehelper.h"

#include "..\shared\Preferences.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

static LPCTSTR REALQUOTE = _T("\"");
static LPCTSTR SAFEQUOTE = _T("{QUOTES}");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CTDCSearchParamHelper::LoadRule(const CPreferences& prefs, const CString& sRule,
									const CTDCCustomAttribDefinitionArray& aCustomAttribDefs, 
									SEARCHPARAM& rule)
{
	CString sAttribID = prefs.GetProfileString(sRule, _T("Attribute"));
	FIND_OPERATOR op = (FIND_OPERATOR)prefs.GetProfileInt(sRule, _T("Operator"), FOP_EQUALS);
	BOOL bAnd = prefs.GetProfileInt(sRule, _T("And"), TRUE);
	DWORD dwFlags = prefs.GetProfileInt(sRule, _T("Flags"), 0);
	CString sValue = prefs.GetProfileString(sRule, _T("Value"));

	TDC_ATTRIBUTE attrib = TDCA_NONE;
	FIND_ATTRIBTYPE nAttribType = FT_NONE;
	CString sCustAttribID;
	
	if (DecodeAttribute(sAttribID, dwFlags, aCustomAttribDefs, attrib, sCustAttribID, nAttribType))
	{
		switch (nAttribType)
		{
		case FT_STRING:
			sValue.Replace(SAFEQUOTE, REALQUOTE);
			break;
			
		case FT_DATE_REL:
			// Backwards compatibility
			sValue.Replace('w', 'W'); // weeks
			sValue.Replace('m', 'M'); // months
			sValue.Replace('y', 'Y'); // years
			break;
		}
		
		if (!sCustAttribID.IsEmpty())
			VERIFY(rule.Set(attrib, sCustAttribID, nAttribType, op, sValue, bAnd));
		else
			VERIFY(rule.Set(attrib, op, sValue, bAnd, nAttribType));

		return TRUE;
	}

	// else
	return FALSE;
}

BOOL CTDCSearchParamHelper::SaveRule(CPreferences& prefs, const CString& sRule, const SEARCHPARAM& rule)
{
	// check for invalid attribute
	if (rule.GetAttribute() == TDCA_NONE)
		return FALSE;

	// handle custom attributes
	if (rule.IsCustomAttribute())
	{
		CString sAttrib;
		sAttrib.Format(_T("%d.%s"), rule.GetAttribute(), rule.GetCustomAttributeID());

		prefs.WriteProfileString(sRule, _T("Attribute"), sAttrib);
	}
	else
	{
		// always write as string
		prefs.WriteProfileString(sRule, _T("Attribute"), Misc::Format(rule.GetAttribute()));
	}

	prefs.WriteProfileInt(sRule, _T("Operator"), rule.GetOperator());
	prefs.WriteProfileInt(sRule, _T("And"), rule.GetAnd());
	prefs.WriteProfileInt(sRule, _T("Flags"), rule.GetFlags());

	// Handle user-quoted strings
	CString sValue = rule.ValueAsString();

	if (rule.TypeIs(FT_STRING) && !sValue.IsEmpty() && Misc::IsQuoted(sValue))
		sValue.Replace(REALQUOTE, SAFEQUOTE);

	prefs.WriteProfileString(sRule, _T("Value"), sValue);

	return TRUE;
}

BOOL CTDCSearchParamHelper::DecodeAttribute(const CString& sAttrib, DWORD dwFlags,
											const CTDCCustomAttribDefinitionArray& aCustAttribDefs, 
											TDC_ATTRIBUTE& nAttrib, CString& sUniqueID,
											FIND_ATTRIBTYPE& nFindType)
{
	nAttrib = TDCA_NONE;
	nFindType = FT_NONE;
	sUniqueID.Empty();

	CStringArray aParts;
	int nNumParts = Misc::Split(sAttrib, aParts, '.', TRUE);

	switch (nNumParts)
	{
	case 1:
		if (Misc::IsNumber(sAttrib))
		{
			nAttrib = (TDC_ATTRIBUTE)_ttoi(sAttrib);
			ASSERT (nAttrib != TDCA_NONE);

			if (SEARCHPARAM::IsCustomAttribute(nAttrib))
			{
				sUniqueID = CTDCCustomAttributeHelper::GetAttributeTypeID(nAttrib, aCustAttribDefs);
				ASSERT(!sUniqueID.IsEmpty());
	
				nFindType = CTDCCustomAttributeHelper::GetAttributeFindType(sUniqueID, dwFlags, aCustAttribDefs);
			}
			else
			{
				nFindType = SEARCHPARAM::GetAttribType(nAttrib, dwFlags);
			}

			ASSERT(nFindType != FT_NONE);
		}
		else
		{
			sUniqueID = sAttrib;
			ASSERT(!sUniqueID.IsEmpty());

			nAttrib = CTDCCustomAttributeHelper::GetAttributeID(sUniqueID, aCustAttribDefs);

			// fallback
			if (nAttrib == TDCA_NONE)
			{
				nAttrib = TDCA_CUSTOMATTRIB;
			}

			ASSERT(SEARCHPARAM::IsCustomAttribute(nAttrib));
			
			nFindType = CTDCCustomAttributeHelper::GetAttributeFindType(sUniqueID, dwFlags, aCustAttribDefs);
			ASSERT(nFindType != FT_NONE);
		}
		break;
		
	case 2:
		nAttrib = (TDC_ATTRIBUTE)_ttoi(aParts[0]);
		ASSERT(SEARCHPARAM::IsCustomAttribute(nAttrib));
		
		sUniqueID = aParts[1];
		ASSERT(!sUniqueID.IsEmpty());

		nFindType = CTDCCustomAttributeHelper::GetAttributeFindType(sUniqueID, dwFlags, aCustAttribDefs);
		ASSERT(nFindType != FT_NONE);
		break;

	case 0:
	default:
		break;
	}

	return ((nAttrib != TDCA_NONE) && (nFindType != FT_NONE));
}

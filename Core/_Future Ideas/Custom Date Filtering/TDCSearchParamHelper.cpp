// TDCSearchParamHelper.cpp: implementation of the CTDCSearchParamHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCSearchParamHelper.h"
#include "TDCStruct.h"

#include "..\Interfaces\Preferences.h"

//////////////////////////////////////////////////////////////////////

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
	FIND_OPERATOR op = prefs.GetProfileEnum(sRule, _T("Operator"), FOP_EQUALS);
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
			
		case FT_DATERELATIVE:
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

		rule.SetFlags(dwFlags);

		return TRUE;
	}
	else if (!sCustAttribID.IsEmpty())
	{
		// let caller know it was a custom attribute
		rule.Set(TDCA_CUSTOMATTRIB, sCustAttribID, FT_STRING, FOP_EQUALS, _T(""));
	}

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
		prefs.WriteProfileString(sRule, _T("Attribute"), rule.GetCustomAttributeID());
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

			if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttrib))
			{
				sUniqueID = aCustAttribDefs.GetAttributeTypeID(nAttrib);
				ASSERT(!sUniqueID.IsEmpty());
	
				nFindType = GetAttributeFindType(sUniqueID, dwFlags, aCustAttribDefs);
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

			nAttrib = aCustAttribDefs.GetAttributeID(sUniqueID);

			// fallback
			if (nAttrib == TDCA_NONE)
			{
				nAttrib = TDCA_CUSTOMATTRIB;
				nFindType = FT_STRING;
			}
			else
			{
				ASSERT(TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttrib));
			
				nFindType = GetAttributeFindType(sUniqueID, dwFlags, aCustAttribDefs);
				ASSERT(nFindType != FT_NONE);
			}
		}
		break;
		
	case 2: // Backwards compatibility
		sUniqueID = aParts[1];
		ASSERT(!sUniqueID.IsEmpty());

		nAttrib = aCustAttribDefs.GetAttributeID(sUniqueID);
 		ASSERT(TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttrib));

		nFindType = GetAttributeFindType(sUniqueID, dwFlags, aCustAttribDefs);
		ASSERT(nFindType != FT_NONE);
		break;

	case 0:
	default:
		break;
	}

	return ((nAttrib != TDCA_NONE) && (nFindType != FT_NONE));
}

FIND_ATTRIBTYPE CTDCSearchParamHelper::GetAttributeFindType(const CString& sUniqueID, BOOL bRelativeDate,
																const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	TDC_ATTRIBUTE nAttribID = aAttribDefs.GetAttributeID(sUniqueID);

	if (nAttribID == TDCA_NONE)
		return FT_NONE;

	if (!TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
	{
		ASSERT(0);
		return FT_NONE;
	}

	return GetAttributeFindType(nAttribID, bRelativeDate, aAttribDefs);
}

FIND_ATTRIBTYPE CTDCSearchParamHelper::GetAttributeFindType(TDC_ATTRIBUTE nAttribID, BOOL bRelativeDate,
																const CTDCCustomAttribDefinitionArray& aAttribDefs)
{
	if (!TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		return SEARCHPARAM::GetAttribType(nAttribID, bRelativeDate);

	TDCCUSTOMATTRIBUTEDEFINITION attribDef;
	VERIFY(aAttribDefs.GetAttributeDef(nAttribID, attribDef));

	// treat lists as strings, except for icon lists
	if (attribDef.IsList() && (attribDef.GetDataType() != TDCCA_ICON))
		return FT_STRING;

	// else
	DWORD dwDataType = attribDef.GetDataType();

	switch (dwDataType)
	{
	case TDCCA_STRING:		return FT_STRING;
	case TDCCA_INTEGER:		return FT_INTEGER;
	case TDCCA_DOUBLE:		return FT_DOUBLE;
	case TDCCA_FRACTION:	return FT_DOUBLE; // TODO
	case TDCCA_DATE:		return (bRelativeDate ? FT_DATERELATIVE : FT_DATE);
	case TDCCA_BOOL:		return FT_BOOL;
	case TDCCA_ICON:		return FT_ICON;
	case TDCCA_TIMEPERIOD:	return FT_TIMEPERIOD;
	}

	return FT_NONE;
}

BOOL CTDCSearchParamHelper::AppendCustomAttributeFilterRules(const CTDCCustomAttributeDataMap& mapData,
															const CTDCCustomAttribDefinitionArray& aAttribDefs,
															CSearchParamArray& aRules)
{
	BOOL bRulesAdded = FALSE;
	POSITION pos = mapData.GetStartPosition();

	while (pos)
	{
		CString sAttribID;
		TDCCADATA data;
		mapData.GetNextAssoc(pos, sAttribID, data);

		TDCCUSTOMATTRIBUTEDEFINITION attribDef;
		VERIFY(aAttribDefs.GetAttributeDef(sAttribID, attribDef));

		if (attribDef.GetListType() == TDCCA_NOTALIST)
		{
			switch (attribDef.GetDataType())
			{
			case TDCCA_STRING:
			case TDCCA_INTEGER:
			case TDCCA_DOUBLE:
			case TDCCA_FRACTION:
			case TDCCA_FILELINK:
			case TDCCA_TIMEPERIOD:
			case TDCCA_ICON:
			case TDCCA_BOOL:
				// Not yet supported
				ASSERT(0);
				break;

			case TDCCA_DATE:
				if (AppendCustomAttributeDateFilter(data, attribDef, aRules))
					bRulesAdded = TRUE;
				break;
			}
		}
		else // list types
		{
			CStringArray aValues;

			if (data.AsArray(aValues))
			{
				SEARCHPARAM rule;

				rule.SetCustomAttribute(attribDef.GetAttributeID(), sAttribID, FT_STRING);
				rule.SetMatchWholeWord(TRUE); // because lists are read-only

				CString sMatchBy = Misc::FormatArray(aValues);

				// special case: 1 empty value
				if ((aValues.GetSize() == 1) && sMatchBy.IsEmpty())
				{
					rule.SetOperator(FOP_NOT_SET);
				}
				else
				{
					rule.SetOperator(FOP_INCLUDES);
					rule.SetValue(sMatchBy);
				}

				aRules.Add(rule);
				bRulesAdded = TRUE;
			}
		}
	}

	return bRulesAdded;
}

BOOL CTDCSearchParamHelper::AppendDateFilter(FILTER_DATE nFilter, const COleDateTime& dateUser, int nNextNDays, 
											 TDC_ATTRIBUTE nDateAttrib, TDC_ATTRIBUTE nTimeAttrib, CSearchParamArray& aRules)
{
	COleDateTime date;

	if (InitFilterDate(nFilter, dateUser, nNextNDays, date))
	{
		// special case: FD_NOW
		if (nFilter == FD_NOW)
			aRules.Add(SEARCHPARAM(nTimeAttrib, FOP_ON_OR_BEFORE, date));
		else
			aRules.Add(SEARCHPARAM(nDateAttrib, FOP_ON_OR_BEFORE, date));
	}
	else if (nFilter == FD_NONE)
	{
		aRules.Add(SEARCHPARAM(nDateAttrib, FOP_NOT_SET));
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CTDCSearchParamHelper::AppendCustomAttributeDateFilter(const TDCCADATA& data, TDCCUSTOMATTRIBUTEDEFINITION& attribDef, CSearchParamArray& aRules)
{
	COleDateTime date, dtUser(CDateHelper::NullDate());
	FILTER_DATE nFilter = (FILTER_DATE)data.AsInteger();

	SEARCHPARAM rule;
	rule.SetCustomAttribute(attribDef.GetAttributeID(), attribDef.sUniqueID, FT_DATE);

	switch (nFilter)
	{
	case FD_ANY:
		return false;

	case FD_NONE:
		rule.SetOperator(FOP_NOT_SET);
		break;

	case FD_USER:
		{
			CString sFilter(data.AsString()), sDate;

			if (Misc::Split(sFilter, sDate, '|') && !sDate.IsEmpty())
				dtUser = _ttof(sDate);
		}
		// fall thru

	case FD_TODAY:
	case FD_TOMORROW:
	case FD_ENDTHISWEEK:
	case FD_ENDNEXTWEEK:
	case FD_ENDTHISMONTH:
	case FD_ENDNEXTMONTH:
	case FD_ENDTHISYEAR:
	case FD_ENDNEXTYEAR:
	case FD_NEXTNDAYS:
	case FD_NOW:
	case FD_YESTERDAY:
		if (InitFilterDate(nFilter, dtUser, 7, date))
		{
			rule.SetOperator(FOP_ON_OR_BEFORE);
			rule.SetValue(date);
		}
		break;

	default:
		ASSERT(0);
		return FALSE;
	}

	aRules.Add(rule);
	return TRUE;
}

BOOL CTDCSearchParamHelper::InitFilterDate(FILTER_DATE nDate, const COleDateTime& dateUser, int nNextNDays, COleDateTime& date)
{
	switch (nDate)
	{
	case FD_TODAY:
		date = CDateHelper::GetDate(DHD_TODAY);
		break;

	case FD_YESTERDAY:
		date = CDateHelper::GetDate(DHD_YESTERDAY);
		break;

	case FD_TOMORROW:
		date = CDateHelper::GetDate(DHD_TOMORROW);
		break;

	case FD_ENDTHISWEEK:
		date = CDateHelper::GetDate(DHD_ENDTHISWEEK);
		break;

	case FD_ENDNEXTWEEK:
		date = CDateHelper::GetDate(DHD_ENDNEXTWEEK);
		break;

	case FD_ENDTHISMONTH:
		date = CDateHelper::GetDate(DHD_ENDTHISMONTH);
		break;

	case FD_ENDNEXTMONTH:
		date = CDateHelper::GetDate(DHD_ENDNEXTMONTH);
		break;

	case FD_ENDTHISYEAR:
		date = CDateHelper::GetDate(DHD_ENDTHISYEAR);
		break;

	case FD_ENDNEXTYEAR:
		date = CDateHelper::GetDate(DHD_ENDNEXTYEAR);
		break;

	case FD_NEXTNDAYS:
		date = (CDateHelper::GetDate(DHD_TODAY) + nNextNDays - 1); // -1 because filter is FOP_ON_OR_BEFORE
		break;

	case FD_NOW:
		date = COleDateTime::GetCurrentTime();
		break;

	case FD_USER:
		ASSERT(CDateHelper::IsDateSet(dateUser));

		date = CDateHelper::GetDateOnly(dateUser);
		break;

	case FD_ANY:
	case FD_NONE:
		break;

	default:
		ASSERT(0);
		break;
	}

	return CDateHelper::IsDateSet(date);
}

void CTDCSearchParamHelper::AppendArrayRule(const CStringArray& aValues, TDC_ATTRIBUTE nAttrib, CSearchParamArray& aRules,
											DWORD dwFlags, DWORD dwIncludeMask)
{
	if (aValues.GetSize())
	{
		CString sMatchBy = Misc::FormatArray(aValues);
		int nRule = -1;

		if ((aValues.GetSize() == 1) && sMatchBy.IsEmpty())
		{
			nRule = aRules.Add(SEARCHPARAM(nAttrib, FOP_NOT_SET));
		}
		else if (dwFlags && dwIncludeMask)
		{
			if (dwFlags & dwIncludeMask)
				nRule = aRules.Add(SEARCHPARAM(nAttrib, FOP_INCLUDES, sMatchBy));
			else
				nRule = aRules.Add(SEARCHPARAM(nAttrib, FOP_EQUALS, sMatchBy));
		}
		else // includes
		{
			nRule = aRules.Add(SEARCHPARAM(nAttrib, FOP_INCLUDES, sMatchBy));
		}

		// Always apply 'match whole word' because filter combos are read-only
		if (nRule != -1)
			aRules[nRule].SetMatchWholeWord(TRUE);
	}
}

void CTDCSearchParamHelper::AppendPriorityRiskRule(int nValue, TDC_ATTRIBUTE nAttrib, CSearchParamArray& aRules,
													int nAnyValue, int nNoValue)
{
	ASSERT((nAttrib == TDCA_PRIORITY) || (nAttrib == TDCA_RISK));

	if (nValue != nAnyValue)
	{
		if (nValue == nNoValue)
		{
			aRules.Add(SEARCHPARAM(nAttrib, FOP_NOT_SET));
		}
		else if (nValue != nAnyValue)
		{
			aRules.Add(SEARCHPARAM(nAttrib, FOP_GREATER_OR_EQUAL, nValue));
		}
	}
}


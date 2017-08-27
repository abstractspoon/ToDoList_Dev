
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCEnumContainers.h"
#include "TDCCustomAttributeHelper.h"

#include "..\shared\Misc.h"
#include "..\shared\preferences.h"

#include "..\Interfaces\IPreferences.h"

//////////////////////////////////////////////////////////////////////

CTDCAttributeMap::CTDCAttributeMap() 
	: CTDCBaseEnumSet<TDC_ATTRIBUTE>()
{
}

CTDCAttributeMap::CTDCAttributeMap(const CTDCAttributeMap& mapOther) 
	: CTDCBaseEnumSet<TDC_ATTRIBUTE>(mapOther)
{
}

CTDCAttributeMap::~CTDCAttributeMap()
{
}

BOOL CTDCAttributeMap::Add(TDC_ATTRIBUTE nAttrib)
{
	if (!CanAdd(nAttrib))
		return FALSE;

	CTDCBaseEnumSet<TDC_ATTRIBUTE>::Add(nAttrib);
	return TRUE;
}


int CTDCAttributeMap::Append(const CTDCAttributeMap& other)
{
	POSITION pos = other.GetStartPosition();
	int nAdded = 0;

	while (pos)
	{
		if (Add(other.GetNext(pos)))
			nAdded++;
	}

	return nAdded;
}

BOOL CTDCAttributeMap::CanAdd(TDC_ATTRIBUTE nAttrib)
{
	switch (nAttrib)
	{
	case TDCA_ALLOCBY:			
	case TDCA_ALLOCTO:			
	case TDCA_CATEGORY:			
	case TDCA_COLOR:			
	case TDCA_COMMENTS:			
	case TDCA_COST:				
	case TDCA_CREATEDBY:		
	case TDCA_CREATIONDATE:		
	case TDCA_DEPENDENCY:		
	case TDCA_DONEDATE:			
	case TDCA_DONETIME:			
	case TDCA_DUEDATE:			
	case TDCA_DUETIME:			
	case TDCA_EXTERNALID:		
	case TDCA_FILEREF:			
	case TDCA_FLAG:				
	case TDCA_ICON:				
	case TDCA_LOCK:				
	case TDCA_ID:				
	case TDCA_LASTMODDATE:	
	case TDCA_LASTMODBY:	
	case TDCA_PARENTID:
	case TDCA_PERCENT:			
	case TDCA_POSITION:	
	case TDCA_PROJECTNAME:
	case TDCA_PRIORITY:			
	case TDCA_RECURRENCE:		
	case TDCA_RISK:				
	case TDCA_STARTDATE:		
	case TDCA_STARTTIME:		
	case TDCA_STATUS:			
	case TDCA_SUBTASKDONE:			
	case TDCA_TAGS:				
	case TDCA_TASKNAME:			
	case TDCA_TIMEEST:			
	case TDCA_TIMESPENT:		
	case TDCA_VERSION:	
		return TRUE;

	default:
		if (CTDCCustomAttributeHelper::IsCustomAttribute(nAttrib))
			return TRUE;
		break;
	}

	ASSERT(0);
	return FALSE;
}

void CTDCAttributeMap::Load(const IPreferences* pPrefs, LPCTSTR szKey, LPCTSTR szValueKeyFmt)
{
	CTDCBaseEnumSet<TDC_ATTRIBUTE>::Load(pPrefs, szKey, szValueKeyFmt, TDCA_NONE);
}

//////////////////////////////////////////////////////////////////////

CTDCAttributeArray::CTDCAttributeArray()
{
}

CTDCAttributeArray::CTDCAttributeArray(const CTDCAttributeArray& aOther)
{
	Copy(aOther);
}

CTDCAttributeArray::~CTDCAttributeArray()
{
}

BOOL CTDCAttributeArray::Has(TDC_ATTRIBUTE nAttrib) const
{
	return Misc::HasT(*this, nAttrib);
}

int CTDCAttributeArray::Add(TDC_ATTRIBUTE nAttrib)
{
	int nFind = Misc::FindT(*this, nAttrib);

	if (nFind != -1)
		return nFind;

	// else
	return CArray<TDC_ATTRIBUTE, TDC_ATTRIBUTE>::Add(nAttrib);
}

void CTDCAttributeArray::Remove(TDC_ATTRIBUTE nAttrib)
{
	int nFind = Misc::FindT(*this, nAttrib);

	if (nFind != -1)
		RemoveAt(nFind);
}

BOOL CTDCAttributeArray::MatchAll(const CTDCAttributeArray& aOther) const
{
	return Misc::MatchAllT(*this, aOther, FALSE);
}

//////////////////////////////////////////////////////////////////////

CTDCColumnIDMap::CTDCColumnIDMap()
	: CTDCBaseEnumSet<TDC_COLUMN>()
{
}

CTDCColumnIDMap::CTDCColumnIDMap(const CTDCColumnIDMap& mapOther)
	: CTDCBaseEnumSet<TDC_COLUMN>(mapOther)
{
}

CTDCColumnIDMap::~CTDCColumnIDMap()
{
}


void CTDCColumnIDMap::Load(const IPreferences* pPrefs, LPCTSTR szKey, LPCTSTR szValueKeyFmt)
{
	CTDCBaseEnumSet<TDC_COLUMN>::Load(pPrefs, szKey, szValueKeyFmt, TDCC_NONE);
}

//////////////////////////////////////////////////////////////////////

CTDCColumnIDArray::CTDCColumnIDArray()
{
}

CTDCColumnIDArray::CTDCColumnIDArray(const CTDCColumnIDArray& aOther)
{
	Copy(aOther);
}

CTDCColumnIDArray::~CTDCColumnIDArray()
{
}

BOOL CTDCColumnIDArray::Has(TDC_COLUMN nColID) const
{
	return Misc::HasT(*this, nColID);
}

int CTDCColumnIDArray::Add(TDC_COLUMN nColID)
{
	int nFind = Misc::FindT(*this, nColID);

	if (nFind != -1)
		return nFind;

	// else
	return CArray<TDC_COLUMN, TDC_COLUMN>::Add(nColID);
}

void CTDCColumnIDArray::Remove(TDC_COLUMN nColID)
{
	int nFind = Misc::FindT(*this, nColID);

	if (nFind != -1)
		RemoveAt(nFind);
}

BOOL CTDCColumnIDArray::MatchAll(const CTDCColumnIDArray& aOther) const
{
	return Misc::MatchAllT(*this, aOther, FALSE);
}

//////////////////////////////////////////////////////////////////////

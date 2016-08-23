
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDCEnumContainers.h"

#include "..\shared\Misc.h"
#include "..\shared\preferences.h"

//////////////////////////////////////////////////////////////////////

CTDCAttributeMap::CTDCAttributeMap() 
	: CTDCBaseEnumMap<TDC_ATTRIBUTE>()
{
}

CTDCAttributeMap::CTDCAttributeMap(const CTDCAttributeMap& mapOther) 
	: CTDCBaseEnumMap<TDC_ATTRIBUTE>(mapOther)
{
}

CTDCAttributeMap::~CTDCAttributeMap()
{
}

BOOL CTDCAttributeMap::HasAttribute(TDC_ATTRIBUTE nAttrib) const
{
	return Has(nAttrib);
}

BOOL CTDCAttributeMap::AddAttribute(TDC_ATTRIBUTE nAttrib)
{
	if ((nAttrib < TDCA_FIRSTATTRIBUTE) || (nAttrib >= TDCA_ATTRIBUTECOUNT))
	{
		if ((nAttrib < TDCA_CUSTOMATTRIB) || (nAttrib > TDCA_CUSTOMATTRIB_LAST))
		{
			ASSERT(0);
			return FALSE;
		}
	}

	// else
	return Add(nAttrib);
}

void CTDCAttributeMap::RemoveAttribute(TDC_ATTRIBUTE nAttrib)
{
	Remove(nAttrib);
}

TDC_ATTRIBUTE CTDCAttributeMap::GetNextAttribute(POSITION& pos) const
{
	return GetNext(pos);
}

BOOL CTDCAttributeMap::MatchAllAttributes(const CTDCAttributeMap& mapOther) const
{
	return CTDCBaseEnumMap<TDC_ATTRIBUTE>::MatchAll(mapOther);
}

void CTDCAttributeMap::CopyAttributes(const CTDCAttributeMap& mapOther)
{
	CTDCBaseEnumMap<TDC_ATTRIBUTE>::Copy(mapOther);
}

void CTDCAttributeMap::AppendAttributes(const CTDCAttributeMap& mapOther)
{
	CTDCBaseEnumMap<TDC_ATTRIBUTE>::Append(mapOther);
}

void CTDCAttributeMap::SaveAttributes(CPreferences& prefs, LPCTSTR szKey, LPCTSTR szValueKeyFmt) const
{
	Save(prefs, szKey, szValueKeyFmt);
}

void CTDCAttributeMap::LoadAttributes(const CPreferences& prefs, LPCTSTR szKey, LPCTSTR szValueKeyFmt)
{
	Load(prefs, szKey, szValueKeyFmt, TDCA_NONE);
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

BOOL CTDCAttributeArray::HasAttribute(TDC_ATTRIBUTE nAttrib) const
{
	return Misc::HasT(*this, nAttrib);
}

int CTDCAttributeArray::AddAttribute(TDC_ATTRIBUTE nAttrib)
{
	int nFind = Misc::FindT(*this, nAttrib);

	if (nFind != -1)
		return nFind;

	// else
	return Add(nAttrib);
}

void CTDCAttributeArray::RemoveAttribute(TDC_ATTRIBUTE nAttrib)
{
	int nFind = Misc::FindT(*this, nAttrib);

	if (nFind != -1)
		RemoveAt(nFind);
}

BOOL CTDCAttributeArray::MatchAll(const CTDCAttributeArray& aOther) const
{
	return Misc::MatchAllT(*this, aOther);
}

//////////////////////////////////////////////////////////////////////

CTDCColumnIDMap::CTDCColumnIDMap()
	: CTDCBaseEnumMap<TDC_COLUMN>()
{
}

CTDCColumnIDMap::CTDCColumnIDMap(const CTDCColumnIDMap& mapOther)
	: CTDCBaseEnumMap<TDC_COLUMN>(mapOther)
{
}

CTDCColumnIDMap::~CTDCColumnIDMap()
{
}

BOOL CTDCColumnIDMap::HasColumn(TDC_COLUMN nColID) const
{
	return Has(nColID);
}

void CTDCColumnIDMap::AddColumn(TDC_COLUMN nColID)
{
	Add(nColID);
}

void CTDCColumnIDMap::RemoveColumn(TDC_COLUMN nColID)
{
	Remove(nColID);
}

TDC_COLUMN CTDCColumnIDMap::GetNextColumn(POSITION& pos) const
{
	return GetNext(pos);
}

BOOL CTDCColumnIDMap::MatchAllColumns(const CTDCColumnIDMap& mapOther) const
{
	return MatchAll(mapOther);
}

void CTDCColumnIDMap::CopyColumns(const CTDCColumnIDMap& mapOther)
{
	Copy(mapOther);
}

void CTDCColumnIDMap::AppendColumns(const CTDCColumnIDMap& mapOther)
{
	Append(mapOther);
}

void CTDCColumnIDMap::SaveColumns(CPreferences& prefs, LPCTSTR szKey, LPCTSTR szValueKeyFmt) const
{
	Save(prefs, szKey, szValueKeyFmt);
}

void CTDCColumnIDMap::LoadColumns(const CPreferences& prefs, LPCTSTR szKey, LPCTSTR szValueKeyFmt)
{
	Load(prefs, szKey, szValueKeyFmt, TDCC_NONE);
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

BOOL CTDCColumnIDArray::HasColumn(TDC_COLUMN nColID) const
{
	return Misc::HasT(*this, nColID);
}

int CTDCColumnIDArray::AddColumn(TDC_COLUMN nColID)
{
	int nFind = Misc::FindT(*this, nColID);

	if (nFind != -1)
		return nFind;

	// else
	return Add(nColID);
}

void CTDCColumnIDArray::RemoveColumn(TDC_COLUMN nColID)
{
	int nFind = Misc::FindT(*this, nColID);

	if (nFind != -1)
		RemoveAt(nFind);
}

BOOL CTDCColumnIDArray::MatchAll(const CTDCColumnIDArray& aOther) const
{
	return Misc::MatchAllT(*this, aOther);
}

//////////////////////////////////////////////////////////////////////

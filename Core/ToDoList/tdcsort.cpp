/////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tdcsort.h"
#include "tdcmapping.h"
#include "tdcstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////

TDSORTCOLUMN::TDSORTCOLUMN(TDC_COLUMN nSortBy, BOOL bSortAscending)
	: 
	nBy(nSortBy), bAscending(bSortAscending)
{
	Validate();
}

BOOL TDSORTCOLUMN::IsSorting() const
{
	return (nBy != TDCC_NONE);
}

BOOL TDSORTCOLUMN::IsSortingBy(TDC_COLUMN nSortBy) const
{
	return (nBy == nSortBy);
}

BOOL TDSORTCOLUMN::IsSortingByCustom() const
{
	return ((nBy >= TDCC_CUSTOMCOLUMN_FIRST) && (nBy <= TDCC_CUSTOMCOLUMN_LAST));
}

BOOL TDSORTCOLUMN::SetSortBy(TDC_COLUMN nSortBy, BOOL bSortAscending)
{
	nBy = nSortBy;

	if (bSortAscending != -1)
		bAscending = bSortAscending;

	return Validate();
}

BOOL TDSORTCOLUMN::Verify() const
{
	return Verify(nBy);
}

void TDSORTCOLUMN::Clear()
{
	nBy = TDCC_NONE;
	bAscending = -1;
}

BOOL TDSORTCOLUMN::Validate()
{
	if (!Verify())
	{
		Clear();
		return FALSE;
	}

	return TRUE;
}

BOOL TDSORTCOLUMN::Matches(const CTDCAttributeMap& mapAttribIDs,
						   const CTDCStyleMap& styles,
						   const CTDCCustomAttribDefinitionArray& aCustAttribs) const
{
	POSITION pos = mapAttribIDs.GetStartPosition();

	while (pos)
	{
		if (Matches(mapAttribIDs.GetNext(pos), nBy, styles, aCustAttribs))
			return TRUE;
	}

	return FALSE;
}

BOOL TDSORTCOLUMN::Verify(TDC_COLUMN nSortBy)
{
	return (((nSortBy >= TDCC_FIRST) && (nSortBy < TDCC_COUNT)) ||
			(nSortBy == TDCC_CLIENT) || 
			(nSortBy == TDCC_NONE));
}

BOOL TDSORTCOLUMN::Matches(TDC_ATTRIBUTE nAttribID, 
						   TDC_COLUMN nSortBy,
						   const CTDCStyleMap& styles,
						   const CTDCCustomAttribDefinitionArray& aCustAttribs)
{
	if (nSortBy == TDCC_NONE)
		return FALSE;

	TDC_COLUMN nAttribCol = TDC::MapAttributeToColumn(nAttribID);

	switch (nAttribID)
	{
	case TDCA_TASKNAME:
	case TDCA_STARTDATE:
	case TDCA_PRIORITY:
	case TDCA_ALLOCTO:
	case TDCA_ALLOCBY:
	case TDCA_STATUS:
	case TDCA_CATEGORY:
	case TDCA_TAGS:
	case TDCA_TIMEESTIMATE:
	case TDCA_RISK:
	case TDCA_EXTERNALID:
	case TDCA_VERSION:
	case TDCA_RECURRENCE:
	case TDCA_ICON:
	case TDCA_COLOR:
	case TDCA_FLAG:
	case TDCA_DUEDATE:
	case TDCA_PERCENT:
	case TDCA_TIMESPENT:
	case TDCA_DEPENDENCY:
	case TDCA_COST:
	case TDCA_FILELINK:
	case TDCA_POSITION:
	case TDCA_POSITION_SAMEPARENT:
	case TDCA_POSITION_DIFFERENTPARENT:
	case TDCA_LOCK:
	case TDCA_SUBTASKDONE:
		{
			ASSERT(nAttribCol != TDCC_NONE);

			if (nAttribCol == nSortBy)
				return TRUE;
		}
		break;

	case TDCA_OFFSETTASK:
		// RECURSIVE calls
		return (Matches(TDCA_STARTDATE, nSortBy, styles, aCustAttribs) ||
				Matches(TDCA_DUEDATE, nSortBy, styles, aCustAttribs)); 

	case TDCA_DONEDATE:
		{
			ASSERT(nAttribCol != TDCC_NONE);

			if (nAttribCol == nSortBy)
				return TRUE;

			if (styles.HasStyle(TDCS_SORTDONETASKSATBOTTOM))
			{
				// some sort columns are unaffected by completed tasks
				switch (nSortBy)
				{
				case TDCC_ID:
					return FALSE;
				}

				// all else
				return TRUE;
			}
		}
		break;

	case TDCA_UNDO:
	case TDCA_PASTE:
	case TDCA_MERGE:
	case TDCA_ALL:
		ASSERT(nAttribCol == TDCC_NONE);
		return TRUE;

	case TDCA_NEWTASK:
	case TDCA_DELETE:
	case TDCA_ARCHIVE:
	case TDCA_ENCRYPT:
	case TDCA_NONE:
	case TDCA_PROJECTNAME:
	case TDCA_COMMENTS:
		ASSERT(nAttribCol == TDCC_NONE);
		return FALSE;

	case TDCA_METADATA:
		return FALSE;

	case TDCA_CUSTOMATTRIBDEFS:	// Resort all custom columns
		ASSERT(nAttribCol == TDCC_NONE);
		return TDCCUSTOMATTRIBUTEDEFINITION::IsCustomColumn(nSortBy);

	default:
		if (TDCCUSTOMATTRIBUTEDEFINITION::IsCustomAttribute(nAttribID))
		{
			ASSERT(aCustAttribs.IsColumnSortable(nAttribCol));
			return (nAttribCol == nSortBy);
		}
		// else unhandled attribute
		ASSERT(0);
		return FALSE;
	}

	ASSERT(nAttribCol != TDCC_NONE);

	// Attribute interdependencies
	switch (nSortBy)
	{
	case TDCC_DONE:
	case TDCC_STARTDATE:
	case TDCC_DUEDATE:
	case TDCC_PERCENT:
		return (nAttribID == TDCA_DONEDATE);

	case TDCC_RISK:
		return ((nAttribID == TDCA_DONEDATE) && styles.HasStyle(TDCS_DONEHAVELOWESTRISK));

	case TDCC_PRIORITY:
		return (((nAttribID == TDCA_DONEDATE) && styles.HasStyle(TDCS_DONEHAVELOWESTPRIORITY)) ||
			((nAttribID == TDCA_DUEDATE) && styles.HasStyle(TDCS_DUEHAVEHIGHESTPRIORITY)));

	case TDCC_TIMEREMAINING:
		return (((nAttribID == TDCA_DUEDATE) && styles.HasStyle(TDCS_CALCREMAININGTIMEBYDUEDATE)) ||
				((nAttribID == TDCA_TIMESPENT) && styles.HasStyle(TDCS_CALCREMAININGTIMEBYSPENT)) ||
				((nAttribID == TDCA_PERCENT) && styles.HasStyle(TDCS_CALCREMAININGTIMEBYPERCENT)));
	}

	// all else
	return FALSE;
}

///////////////////////////////////////////////////////////////////

TDSORTCOLUMNS::TDSORTCOLUMNS()
{
}

TDSORTCOLUMNS::TDSORTCOLUMNS(TDC_COLUMN nBy, BOOL bAscending)
{
	SetSortBy(0, nBy, bAscending);

	Validate();
}

BOOL TDSORTCOLUMNS::IsSorting() const
{
	return IsSorting(0);
}

BOOL TDSORTCOLUMNS::IsSorting(int nCol) const
{
	if ((nCol < 0) || (nCol >= 3))
		return FALSE;

	return cols[nCol].IsSorting();
}

BOOL TDSORTCOLUMNS::IsSortingBy(TDC_COLUMN nSortBy) const
{
	if (nSortBy == TDCC_NONE)
		return !IsSorting();

	for (int nCol = 0; nCol < 3; nCol++)
	{
		if (!cols[nCol].IsSorting())
			break;

		if (cols[nCol].IsSortingBy(nSortBy))
			return TRUE;
	}

	// else
	return FALSE;
}

TDC_COLUMN TDSORTCOLUMNS::GetSortBy(int nCol) const
{
	if ((nCol < 0) || (nCol >= 3))
		return TDCC_NONE;

	return cols[nCol].nBy;
}

BOOL TDSORTCOLUMNS::SetSortBy(const TDSORTCOLUMNS& sortCols)
{
	*this = sortCols;

	return Validate();
}

BOOL TDSORTCOLUMNS::SetSortBy(int nCol, const TDSORTCOLUMN& col)
{
	return SetSortBy(nCol, col.nBy, col.bAscending);
}

BOOL TDSORTCOLUMNS::SetSortBy(int nCol, TDC_COLUMN nBy, BOOL bAscending)
{
	if ((nCol < 0) || (nCol >= 3))
		return FALSE;

	cols[nCol].SetSortBy(nBy, bAscending);

	return Validate();
}

const TDSORTCOLUMN* TDSORTCOLUMNS::Cols() const
{
	return &cols[0];
}

BOOL TDSORTCOLUMNS::Verify() const
{
	for (int nCol = 0; nCol < 3; nCol++)
	{
		if (!cols[nCol].Verify())
			return FALSE;
	}

	// else
	return TRUE;
}

void TDSORTCOLUMNS::Clear()
{
	for (int nCol = 0; nCol < 3; nCol++)
		cols[nCol].Clear();
}

BOOL TDSORTCOLUMNS::Validate()
{
	BOOL bPrevSort = TRUE;

	for (int nCol = 0; nCol < 3; nCol++)
	{
		// Clear 'this' column if previous column is cleared
		if (!bPrevSort)
			cols[nCol].Clear();
		else
			bPrevSort = cols[nCol].Validate();
	}

	return IsSorting();
}

BOOL TDSORTCOLUMNS::Matches(const CTDCAttributeMap& mapAttribIDs,
							const CTDCStyleMap& styles,
							const CTDCCustomAttribDefinitionArray& aCustAttribs) const
{
	if (IsSorting())
	{
		for (int nCol = 0; nCol < 3; nCol++)
		{
			if (cols[nCol].Matches(mapAttribIDs, styles, aCustAttribs))
				return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////

TDSORT::TDSORT(TDC_COLUMN nBy, BOOL bAscending)
	:
	single(nBy, bAscending),
	multi(nBy, bAscending),
	bModSinceLastSort(FALSE),
	bMulti(FALSE)
{
}

BOOL TDSORT::IsSorting() const
{
	if (bMulti)
		return multi.IsSorting();

	// else
	return single.IsSorting();
}

BOOL TDSORT::IsSortingBy(TDC_COLUMN nBy, BOOL bCheckMulti) const
{
	if (!bMulti)
		return single.IsSortingBy(nBy);

	// else multi-sorting
	if (bCheckMulti)
		return multi.IsSortingBy(nBy);

	// else
	return FALSE;
}

BOOL TDSORT::SetSortBy(TDC_COLUMN nBy, BOOL bAscending)
{
	single.SetSortBy(nBy, bAscending);
	bMulti = FALSE;

	return IsSorting();
}

BOOL TDSORT::SetSortBy(const TDSORTCOLUMN& col)
{
	return SetSortBy(col.nBy, col.bAscending);
}

BOOL TDSORT::SetSortBy(const TDSORTCOLUMNS& cols)
{
	multi.SetSortBy(cols);
	bMulti = TRUE;

	return IsSorting();
}

BOOL TDSORT::Verify() const
{
	return (bMulti ? multi.Verify() : single.Verify());
}

BOOL TDSORT::Validate()
{
	single.Validate();
	multi.Validate();

	return IsSorting();
}

BOOL TDSORT::Matches(const CTDCAttributeMap& mapAttribIDs,
					 const CTDCStyleMap& styles,
					 const CTDCCustomAttribDefinitionArray& aCustAttribs) const
{
	if (bMulti)
		return multi.Matches(mapAttribIDs, styles, aCustAttribs);

	// else
	return single.Matches(mapAttribIDs, styles, aCustAttribs);
}

void TDSORT::SaveState(IPreferences* pPrefs, const CString& sKey) const
{
	CString sSortKey = (sKey + _T("\\SortColState"));

	pPrefs->WriteProfileInt(sSortKey, _T("Multi"), bMulti);

	pPrefs->WriteProfileInt(sSortKey, _T("Column"), single.nBy);
	pPrefs->WriteProfileInt(sSortKey, _T("Ascending"), single.bAscending);

	for (int nCol = 0; nCol < 3; nCol++)
	{
		CString sColKey;
		sColKey.Format(_T("Column%d"), (nCol + 1));

		CString sAscendKey;
		sAscendKey.Format(_T("Ascending%d"), (nCol + 1));

		pPrefs->WriteProfileInt(sSortKey, sColKey, multi.Cols()[nCol].nBy);
		pPrefs->WriteProfileInt(sSortKey, sAscendKey, multi.Cols()[nCol].bAscending);
	}
}

BOOL TDSORT::LoadState(const IPreferences* pPrefs, const CString& sKey)
{
	CString sSortKey = (sKey + _T("\\SortColState"));

	single.nBy = (TDC_COLUMN)pPrefs->GetProfileInt(sSortKey, _T("Column"), TDCC_NONE);
	single.bAscending = pPrefs->GetProfileInt(sSortKey, _T("Ascending"), TRUE);

	bMulti = pPrefs->GetProfileInt(sSortKey, _T("Multi"), FALSE);

	for (int nCol = 0; nCol < 3; nCol++)
	{
		CString sColKey;
		sColKey.Format(_T("Column%d"), (nCol + 1));

		CString sAscendKey;
		sAscendKey.Format(_T("Ascending%d"), (nCol + 1));

		multi.SetSortBy(nCol,
			(TDC_COLUMN)pPrefs->GetProfileInt(sSortKey, sColKey, TDCC_NONE),
						pPrefs->GetProfileInt(sSortKey, sAscendKey, TRUE));
	}

	return Validate();
}

/////////////////////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_TDCSORT_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_)
#define AFX_TDCSORT_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// tdlutil.h : header file
//

#include "..\shared\misc.h"
#include "..\shared\mapex.h"
#include "..\shared\enstring.h"
#include "..\shared\preferences.h"

/////////////////////////////////////////////////////////////////////////////////////////////

struct TDSORTCOLUMN
{
	TDSORTCOLUMN(TDC_COLUMN nSortBy = TDCC_NONE, BOOL bSortAscending = -1) 
		: nBy(nSortBy), bAscending(bSortAscending)
	{
		Validate();
	}

	BOOL IsSorting() const
	{
		return (nBy != TDCC_NONE);
	}

	BOOL IsSortingBy(TDC_COLUMN nSortBy) const
	{
		return (nBy == nSortBy);
	}
	
	BOOL IsSortingByCustom() const
	{
		return ((nBy >= TDCC_CUSTOMCOLUMN_FIRST) && (nBy <= TDCC_CUSTOMCOLUMN_LAST));
	}

	BOOL SetSortBy(TDC_COLUMN nSortBy, BOOL bSortAscending = -1) 
	{
		nBy = nSortBy;

		if (bSortAscending != -1)
			bAscending = bSortAscending;

		return Validate();
	}

	BOOL Verify() const
	{
		return Verify(nBy);
	}

	void Clear()
	{
		nBy = TDCC_NONE;
		bAscending = -1;
	}

	BOOL Validate()
	{
		if (!Verify())
		{
			Clear();
			return FALSE;
		}

		return TRUE;
	}

	static BOOL Verify(TDC_COLUMN nSortBy) 
	{
		return (((nSortBy >= TDCC_FIRST) && (nSortBy < TDCC_COUNT)) ||
				(nSortBy == TDCC_CLIENT) || (nSortBy == TDCC_NONE));
	}

	TDC_COLUMN nBy;
	BOOL bAscending;
};

struct TDSORTCOLUMNS
{
	TDSORTCOLUMNS()
	{
	}
	
	TDSORTCOLUMNS(TDC_COLUMN nBy, BOOL bAscending = -1)
	{
		SetSortBy(0, nBy, bAscending);

		Validate();
	}

	BOOL IsSorting() const
	{
		return IsSorting(0);
	}

	BOOL IsSorting(int nCol) const
	{
		if ((nCol < 0) || (nCol >= 3))
			return FALSE;

		return cols[nCol].IsSorting();
	}
	
	BOOL IsSortingBy(TDC_COLUMN nSortBy) const
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

	TDC_COLUMN GetSortBy(int nCol) const
	{
		if ((nCol < 0) || (nCol >= 3))
			return TDCC_NONE;

		return cols[nCol].nBy;
	}

	BOOL SetSortBy(const TDSORTCOLUMNS& cols)
	{
		*this = cols;

		return Validate();
	}

	BOOL SetSortBy(int nCol, const TDSORTCOLUMN& col)
	{
		return SetSortBy(nCol, col.nBy, col.bAscending);
	}

	BOOL SetSortBy(int nCol, TDC_COLUMN nBy, BOOL bAscending = -1) 
	{
		if ((nCol < 0) || (nCol >= 3))
			return FALSE;

		cols[nCol].SetSortBy(nBy, bAscending);

		return Validate();
	}

	const TDSORTCOLUMN* Cols() const
	{
		return &cols[0];
	}

	BOOL Verify() const
	{
		for (int nCol = 0; nCol < 3; nCol++)
		{
			if (!cols[nCol].Verify())
				return FALSE;
		}

		// else
		return TRUE;
	}

	void Clear()
	{
		for (int nCol = 0; nCol < 3; nCol++)
			cols[nCol].Clear();
	}

	BOOL Validate()
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

protected:
	TDSORTCOLUMN cols[3];
};

struct TDSORT
{
	TDSORT(TDC_COLUMN nBy = TDCC_NONE, BOOL bAscending = -1) 
		: 
		single(nBy, bAscending),
		multi(nBy, bAscending),
		bModSinceLastSort(FALSE), 
		bMulti(FALSE)
	{
	}

	BOOL IsSorting() const
	{
		if (bMulti)
			return multi.IsSorting();

		// else
		return single.IsSorting();
	}

	BOOL IsSortingBy(TDC_COLUMN nBy, BOOL bCheckMulti) const
	{
		if (!bMulti)
			return single.IsSortingBy(nBy);

		// else multi-sorting
		if (bCheckMulti)
			return multi.IsSortingBy(nBy);

		// else
		return FALSE;
	}

	BOOL SetSortBy(TDC_COLUMN nBy, BOOL bAscending)
	{
		single.SetSortBy(nBy, bAscending);
		bMulti = FALSE;

		return IsSorting();
	}

	BOOL SetSortBy(const TDSORTCOLUMN& col)
	{
		return SetSortBy(col.nBy, col.bAscending);
	}

	BOOL SetSortBy(const TDSORTCOLUMNS& cols)
	{
		multi.SetSortBy(cols);
		bMulti = TRUE;
		
		return IsSorting();
	}

	BOOL Verify() const
	{
		return (bMulti ? multi.Verify() : single.Verify());
	}

	BOOL Validate()
	{
		single.Validate();
		multi.Validate();

		return IsSorting();
	}

	void SaveState(IPreferences* pPrefs, const CString& sKey) const
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

	BOOL LoadState(const IPreferences* pPrefs, const CString& sKey)
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

	TDSORTCOLUMN single;
	TDSORTCOLUMNS multi;

	BOOL bMulti;
	BOOL bModSinceLastSort;
};

/////////////////////////////////////////////////////////////////////////////////////////////

#endif // AFX_TDCSORT_H__5951FDE6_508A_4A9D_A55D_D16EB026AEF7__INCLUDED_

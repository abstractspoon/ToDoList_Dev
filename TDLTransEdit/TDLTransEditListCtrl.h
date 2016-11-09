#pragma once

#include "..\shared\InputListCtrl.h"

// CTDLTransEditListCtrl

class CTransDictionary;

class CTDLTransEditListCtrl : public CInputListCtrl
{
	DECLARE_DYNAMIC(CTDLTransEditListCtrl)

public:
	CTDLTransEditListCtrl();
	virtual ~CTDLTransEditListCtrl();

	void Initialise();
	BOOL RebuildList(const CTransDictionary& dict);

protected:
	DECLARE_MESSAGE_MAP()

protected:
	virtual int CompareItems(DWORD dwItemData1, DWORD dwItemData2, int nSortColumn);
	virtual COLORREF GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;
	virtual COLORREF GetItemTextColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const;

};



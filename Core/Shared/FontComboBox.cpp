#include "stdafx.h"
#include "fontcombobox.h"

// reserve lobyte for charset
#define PRINTER_FONT 0x0100
#define TT_FONT	0x0200
#define DEVICE_FONT 0x0400

#define BMW 16
#define BMH 15

static int nFontSizes[] = {8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48, 72};
int CFontComboBox::m_nFontHeight = 0;

class CFontDesc
{
public:
	CFontDesc(LPCTSTR lpszName, LPCTSTR lpszScript, BYTE nCharSet, 
		BYTE nPitchAndFamily, DWORD dwFlags);
	CString m_strName;
	CString m_strScript;
	BYTE m_nCharSet;
	BYTE m_nPitchAndFamily;
	DWORD m_dwFlags;
};

CFontDesc::CFontDesc(LPCTSTR lpszName, LPCTSTR lpszScript, BYTE nCharSet, 
	BYTE nPitchAndFamily, DWORD dwFlags)
{
	m_strName = lpszName;
	m_strScript = lpszScript;
	m_nCharSet = nCharSet;
	m_nPitchAndFamily = nPitchAndFamily;
	m_dwFlags = dwFlags;
}

/////////////////////////////////////////////////////////////////////////////
// CFontComboBox 

BEGIN_MESSAGE_MAP(CFontComboBox, CComboBox)
	//{{AFX_MSG_MAP(CFontComboBox)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Global help commands
END_MESSAGE_MAP()

CFontComboBox::CFontComboBox()
{
//	VERIFY(m_bmFontType.LoadBitmap(IDB_FONTTYPE));
}

void CFontComboBox::OnDestroy() 
{
	// destroy all the CFontDesc's
	EmptyContents();
	CComboBox::OnDestroy();
}

void CFontComboBox::EmptyContents()
{
	// destroy all the CFontDesc's
	int nCount = GetCount();
	for (int i=0;i<nCount;i++)
		delete (CFontDesc*)GetItemData(i);

	ResetContent();
}

void CFontComboBox::EnumFontFamiliesEx(CDC* pDC, /*BOOL bIgnoreCharsets,*/ BYTE nCharSet)
{
	CMapStringToPtr map;
	CString str = GetSelectedFont(), sFontName;

	EmptyContents();
	ResetContent();

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfCharSet = nCharSet;

	if (pDC && pDC->m_hDC != NULL)
	{
		::EnumFontFamiliesEx(pDC->m_hDC, &lf,
			(FONTENUMPROC) EnumFamPrinterCallBackEx, (LPARAM) this, NULL);
	}
	else
	{
		HDC hDC = ::GetDC(::GetDesktopWindow());
		ASSERT(hDC != NULL);

		::EnumFontFamiliesEx(hDC, &lf,
				(FONTENUMPROC) EnumFamScreenCallBackEx, (LPARAM) this, NULL);

		::ReleaseDC(::GetDesktopWindow(), hDC);
	}

	// now walk through the fonts and remove (charset) from fonts with only one

	int nCount = m_arrayFontDesc.GetSize();
	// walk through fonts adding names to string map
	// first time add value 0, after that add value 1
	//fabio_2005
	int i = 0;
	for (i = 0; i<nCount;i++)
	{
		CFontDesc* pDesc = (CFontDesc*)m_arrayFontDesc[i];
		void* pv = NULL;
		if (map.Lookup(pDesc->m_strName, pv)) // found it
		{
			if (pv == NULL) // only one entry so far
			{
				map.RemoveKey(pDesc->m_strName);
				map.SetAt(pDesc->m_strName, (void*)1);
			}
		}
		else // not found
			map.SetAt(pDesc->m_strName, (void*)0);
	}

	// only add one entry per fontname if so requested
	for (i = 0; i<nCount;i++)
	{
		CFontDesc* pDesc = (CFontDesc*)m_arrayFontDesc[i];
		CString str = pDesc->m_strName;
		void* pv = NULL;
		VERIFY(map.Lookup(str, pv));

		// if thye font names not in the combobox, add it
		if (FindString(-1, str) == CB_ERR)
		{
			int nIndex = AddString(str);

			ASSERT(nIndex >=0);

			if (nIndex >=0) //no error
				SetItemData(nIndex, (DWORD)pDesc);
		}
		else // delete the font description
			delete pDesc;
	}

	SetSelectedFont(str);
	m_arrayFontDesc.RemoveAll();
}

void CFontComboBox::AddFont(ENUMLOGFONT* pelf, DWORD dwType, LPCTSTR lpszScript)
{
	LOGFONT& lf = pelf->elfLogFont;
	if (lf.lfCharSet == MAC_CHARSET) // don't put in MAC fonts, commdlg doesn't either
		return;

	// Don't display vertical font for FE platform
	if ((GetSystemMetrics(SM_DBCSENABLED)) && (lf.lfFaceName[0] == '@'))
		return;

	// don't put in non-printer raster fonts
	CFontDesc* pDesc = new CFontDesc(lf.lfFaceName, lpszScript, 
		lf.lfCharSet, lf.lfPitchAndFamily, dwType);

	m_arrayFontDesc.Add(pDesc);
}

BOOL CALLBACK AFX_EXPORT CFontComboBox::EnumFamScreenCallBack(ENUMLOGFONT* pelf, 
	NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis)
{
	// don't put in non-printer raster fonts
	if (FontType & RASTER_FONTTYPE)
		return 1;

	DWORD dwData = (FontType & TRUETYPE_FONTTYPE) ? TT_FONT : 0;
	((CFontComboBox *)pThis)->AddFont(pelf, dwData);

	return 1;
}

BOOL CALLBACK AFX_EXPORT CFontComboBox::EnumFamPrinterCallBack(ENUMLOGFONT* pelf, 
	NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis)
{
	DWORD dwData = PRINTER_FONT;

	if (FontType & TRUETYPE_FONTTYPE)
		dwData |= TT_FONT;

	else if (FontType & DEVICE_FONTTYPE)
		dwData |= DEVICE_FONT;

	((CFontComboBox *)pThis)->AddFont(pelf, dwData);

	return 1;
}

BOOL CALLBACK AFX_EXPORT CFontComboBox::EnumFamScreenCallBackEx(ENUMLOGFONTEX* pelf, 
	NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis)
{
	// don't put in non-printer raster fonts
	if (FontType & RASTER_FONTTYPE)
		return 1;

	DWORD dwData = (FontType & TRUETYPE_FONTTYPE) ? TT_FONT : 0;

	((CFontComboBox *)pThis)->AddFont((ENUMLOGFONT*)pelf, dwData, CString(pelf->elfScript));

	return 1;
}

BOOL CALLBACK AFX_EXPORT CFontComboBox::EnumFamPrinterCallBackEx(ENUMLOGFONTEX* pelf, 
	NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis)
{
	DWORD dwData = PRINTER_FONT;

	if (FontType & TRUETYPE_FONTTYPE)
		dwData |= TT_FONT;

	else if (FontType & DEVICE_FONTTYPE)
		dwData |= DEVICE_FONT;

	((CFontComboBox *)pThis)->AddFont((ENUMLOGFONT*)pelf, dwData, CString(pelf->elfScript));

	return 1;
}

void CFontComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	ASSERT(lpDIS->CtlType == ODT_COMBOBOX);
	int id = (int)(WORD)lpDIS->itemID;

	CDC *pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rc(lpDIS->rcItem);

	if (lpDIS->itemState & ODS_FOCUS)
		pDC->DrawFocusRect(rc);

	int nIndexDC = pDC->SaveDC();

	CBrush brushFill;

	if (lpDIS->itemState & ODS_SELECTED)
	{
		brushFill.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else
		brushFill.CreateSolidBrush(pDC->GetBkColor());

	pDC->SetBkMode(TRANSPARENT);
	pDC->FillRect(rc, &brushFill);

	CFontDesc* pDesc= (CFontDesc*)lpDIS->itemData;
	ASSERT(pDesc != NULL);
/*	DWORD dwData = pDesc->m_dwFlags;

	if (dwData & (TT_FONT|DEVICE_FONT)) // truetype or device flag set by SetItemData
	{
		CDC dc;
		dc.CreateCompatibleDC(pDC);
		CBitmap* pBitmap = dc.SelectObject(&m_bmFontType);

		if (dwData & TT_FONT)
			pDC->BitBlt(rc.left, rc.top, BMW, BMH, &dc, BMW, 0, SRCAND);
		else // DEVICE_FONT
			pDC->BitBlt(rc.left, rc.top, BMW, BMH, &dc, 0, 0, SRCAND);

		// cleanup
		dc.SelectObject(pBitmap);
	}
*/	
	rc.left += BMW + 6;
	CString strText;
	GetLBText(id, strText);
	pDC->TextOut(rc.left,rc.top,strText,strText.GetLength());

	pDC->RestoreDC(nIndexDC);

	brushFill.DeleteObject();
}

void CFontComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	ASSERT(lpMIS->CtlType == ODT_COMBOBOX);
	ASSERT(m_nFontHeight > 0);
	CRect rc;
	
	GetWindowRect(&rc);
	lpMIS->itemWidth = rc.Width();
	lpMIS->itemHeight = max(BMH, m_nFontHeight);
}

int CFontComboBox::CompareItem(LPCOMPAREITEMSTRUCT lpCIS)
{
	ASSERT(lpCIS->CtlType == ODT_COMBOBOX);
	int id1 = (int)(WORD)lpCIS->itemID1;
	int id2 = (int)(WORD)lpCIS->itemID2;
	CString str1,str2;
	if (id1 == -1)
		return -1;
	if (id2 == -1)
		return 1;
	GetLBText(id1, str1);
	GetLBText(id2, str2);
	return str1.Collate(str2);
}

// find a font with the face name and charset
void CFontComboBox::MatchFont(LPCTSTR lpszName, BYTE nCharSet)
{
	int nFirstIndex = FindString(-1, lpszName);
	if (nFirstIndex != CB_ERR)
	{
		int nIndex = nFirstIndex;
		do
		{ 
			CFontDesc* pDesc = (CFontDesc*)GetItemData(nIndex);
			ASSERT(pDesc != NULL);
			// check the actual font name to avoid matching Courier western
			// to Courier New western
			if ((nCharSet == DEFAULT_CHARSET || pDesc->m_nCharSet == nCharSet) && 
				lstrcmp(lpszName, pDesc->m_strName)==0)
			{
				//got a match
				if (GetCurSel() != nIndex)
					SetCurSel(nIndex);
				return;
			}
			nIndex = FindString(nIndex, lpszName);
		} while (nIndex != nFirstIndex);
		// loop until found or back to first item again
	}
	//enter font name
	SetSelectedFont(lpszName, TRUE);
}

CString CFontComboBox::GetSelectedFont()
{
	CString sFont;

	int nIndex = GetCurSel();

	if (nIndex == CB_ERR)
		GetWindowText(sFont);
	else
		GetLBText(nIndex, sFont);

	return sFont;
}

void CFontComboBox::SetSelectedFont(LPCTSTR lpszText,BOOL bMatchExact)
{
	int idx = (bMatchExact) ? FindStringExact(-1,lpszText) : FindString(-1, lpszText);

	SetCurSel((idx==CB_ERR) ? -1 : idx);

	if (idx == CB_ERR)
		SetWindowText(lpszText);
}

void CFontComboBox::PreSubclassWindow()
{
	CComboBox::PreSubclassWindow();

	EnumFontFamiliesEx();
}



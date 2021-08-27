// EnBitmap.h: interface for the CEnBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENBITMAP_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_)
#define AFX_ENBITMAP_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\3rdparty\colordef.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////////////////////////

enum EB_IMAGETYPE
{
	FT_UNKNOWN = -1,
		
	FT_BMP,
	FT_ICO,
	FT_JPG,
	FT_GIF,
	FT_PNG,
};

//////////////////////////////////////////////////////////////////////////////////////////

class CEnBitmap : public CBitmap  
{
public:
	CEnBitmap(COLORREF crBkgnd = RGB(255, 255, 255));
	virtual ~CEnBitmap();

	BOOL LoadImage(LPCTSTR szImagePath, COLORREF crBack = CLR_NONE, int cx = 0, int cy = 0);
	BOOL LoadImage(UINT uIDRes, LPCTSTR szResourceType, HMODULE hInst = NULL, COLORREF crBack = CLR_NONE, int cx = 0, int cy = 0); 
	BOOL CopyImage(HBITMAP hBitmap);
	BOOL CopyImage(CBitmap* pBitmap);
	BOOL CopyImage(HICON hIcon, COLORREF crBack = CLR_NONE, int cx = 0, int cy = 0);
	BOOL CopyImages(HIMAGELIST hImageList, COLORREF crBack = CLR_NONE);
	BOOL CopyImage(HIMAGELIST hImageList, int iImage, COLORREF crBack = CLR_NONE);

	static BOOL CopyImageFileToClipboard(HWND hWnd, LPCTSTR szImagePath, COLORREF crBack = CLR_NONE, WORD nBpp = 0);
	static BOOL CopyToClipboard(HWND hWnd, const CBitmap& bm, WORD nBpp = 0);
	BOOL CopyToClipboard(HWND hWnd, WORD nBpp = 0) const;

	CSize GetSize() const;
	HICON ExtractIcon(COLORREF crMask = CLR_NONE, int cx = 0, int cy = 0);

	// helpers
	static HBITMAP LoadImageFile(LPCTSTR szImagePath, COLORREF crBack = CLR_NONE, int cx = 0, int cy = 0);
	static HBITMAP LoadImageResource(UINT uIDRes, LPCTSTR szResourceType, HMODULE hInst = NULL, COLORREF crBack = CLR_NONE, int cx = 0, int cy = 0); 
	static BOOL GetResource(LPCTSTR lpName, LPCTSTR lpType, HMODULE hInst, void* pResource, int& nBufSize);
	static IPicture* LoadFromBuffer(BYTE* pBuff, int nSize);
	static EB_IMAGETYPE GetFileType(LPCTSTR szImagePath);
	static BOOL IsSupportedImageFile(LPCTSTR szImagePath);
	static HICON LoadImageFileAsIcon(LPCTSTR szImagePath, COLORREF crBack = CLR_NONE, int cx = 0, int cy = 0);
	static CSize GetImageSize(HBITMAP hbm);
	static HBITMAP ResizeImage(HBITMAP hbm, int cx, int cy, COLORREF crBack = CLR_NONE);
	static HBITMAP ResizeImage(HICON hIcon, int cx, int cy, COLORREF crBack = CLR_NONE);
	static HICON ExtractIcon(HBITMAP hbm, COLORREF crMask = CLR_NONE, int cx = 0, int cy = 0);

protected:
	COLORREF m_crBkgnd;

protected:
	RGBX* GetDIBits32();
	BOOL PrepareBitmapInfo32(BITMAPINFO& bi, HBITMAP hBitmap = NULL);
	BOOL FillBackground(CDC* pDC, int cx, int cy, COLORREF crBkgnd = CLR_NONE);
	BOOL HasBackgroundColor(COLORREF crAltBack) const;

	static BOOL Fill(RGBX* pPixels, CSize size, COLORREF color);
	static HBITMAP ExtractBitmap(IPicture* pPicture, COLORREF crBack, int cx, int cy);
};

#endif // !defined(AFX_ENBITMAP_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_)

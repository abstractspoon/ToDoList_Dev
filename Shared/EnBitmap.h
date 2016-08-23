// EnBitmap.h: interface for the CEnBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENBITMAP_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_)
#define AFX_ENBITMAP_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "colordef.h"
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////////////
// base class for image processing

class C32BitImageProcessor
{
public:
	C32BitImageProcessor(BOOL bEnableWeighting = FALSE);
	virtual ~C32BitImageProcessor();

	virtual CSize CalcDestSize(CSize sizeSrc);
	virtual BOOL ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest, 
								COLORREF crMask = CLR_NONE);

	void CalcWeightedColor(RGBX* pPixels, CSize size, double dX, double dY, RGBX& rgbResult);

protected:
	BOOL m_bWeightingEnabled;
};

typedef CArray<C32BitImageProcessor*, C32BitImageProcessor*> C32BIPArray;

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
	BOOL Copy(HIMAGELIST hImageList);

	static BOOL CopyImageFileToClipboard(HWND hWnd, LPCTSTR szImagePath, COLORREF crBack = CLR_NONE, WORD nBpp = 0);
	static BOOL CopyToClipboard(HWND hWnd, const CBitmap& bm, WORD nBpp = 0);
	BOOL CopyToClipboard(HWND hWnd, WORD nBpp = 0) const;

	BOOL ProcessImage(C32BitImageProcessor* pProcessor, COLORREF crMask = CLR_NONE);
	BOOL ProcessImage(C32BIPArray& aProcessors, COLORREF crMask = CLR_NONE); // ordered list of processors

	CSize GetSize() const;
	BOOL Resize(int cx, int cy);
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
	static HBITMAP ResizeImage(HBITMAP hbm, int cx, int cy);
	static HICON ExtractIcon(HBITMAP hbm, COLORREF crMask = CLR_NONE, int cx = 0, int cy = 0);

protected:
	COLORREF m_crBkgnd;

protected:
	RGBX* GetDIBits32();
	BOOL PrepareBitmapInfo32(BITMAPINFO& bi, HBITMAP hBitmap = NULL);

	static BOOL Fill(RGBX* pPixels, CSize size, COLORREF color);
	static HBITMAP ExtractBitmap(IPicture* pPicture, COLORREF crBack, int cx, int cy);
	static HBITMAP ExtractBitmap(const CImageList& il, COLORREF crBack, int cx, int cy);
};

#endif // !defined(AFX_ENBITMAP_H__1FDE0A4E_8AB4_11D6_95AD_EFA89432A428__INCLUDED_)

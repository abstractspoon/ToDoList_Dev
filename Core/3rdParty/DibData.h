// CDibData.h
//
// Author : John R. Shaw (shawj2@earthlink.net)
// Date   : Oct 14 2003
//
// Copyright (C) 2003 John R. Shaw
// All rights reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in this file is used in any commercial application 
// then a simple email would be nice.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL JOHN R. SHAW BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
//
// Please email bug reports, bug fixes, enhancements, requests and
// comments to: shawj2@earthlink.net
//

#ifndef __CDIBDATA_H__
#define __CDIBDATA_H__

/////////////////////////////////////////////////////////////////////////////
//! class CDibData

class CDibData;

class CDibData : public CObject
{
	DECLARE_DYNAMIC(CDibData)

	friend CDibData;

	HANDLE m_hDib;				//!< Handle to DIB object
	LPBYTE m_pDib;				//!< Pointer to DIB
	LPBYTE m_pBits;				//!< Pointer to image bits in DIB
	LPBYTE* m_pLine;			//!< DIB line pointer array
	LPRGBQUAD m_pColorTable;	//!< Pointer to color table in DIB
	WORD  m_wBitsPerPixel;		//!< Number of bits-per-pixel
	DWORD m_dwColorTableSize;	//!< Number of color table entries
	DWORD m_dwBitfields[3];		//!< DIB bitfields (16 & 24 bit images)
	WORD  m_wRShift[3];			//!< Right shift values (16 & 24 bit images)
	WORD  m_wLShift[3];			//!< Left  shift values (16 & 24 bit images)

protected:
	void SetDefaultBitfields(WORD wBitsColor);
	HANDLE CreateBitmapInfo(CBitmap* pbmpSrc, BOOL* pbFinished);
	void FixImageData();
	CDibData* CreateEmptyDIB(WORD wBitsPerPixel);
	UINT GetOptimalColors(LPRGBQUAD pColors, UINT cColors, UINT nColorBits);
	HBITMAP CreateDIBitmap();

public:
	// Constructors/Destructors
	CDibData(const CDibData&);
	CDibData();
	~CDibData();

	// Attach/Detach
	BOOL Attach(HANDLE hDib);
	HANDLE Detach();

	// DIB deletion
	BOOL DeleteObject();

	// Bitmap update
	BOOL SetDIBits(CBitmap* pbmpDest);
	BOOL SetDIBits(HBITMAP hBitmap);

	// Pixel set/get
	DWORD GetPixelValue(int x, int y) const;
	BOOL SetPixelValue(int x, int y, DWORD dwPixel);

	COLORREF GetPixel(int x, int y) const;
	BOOL SetPixel(int x, int y, COLORREF clPixel);

	BOOL CopyPixelValue(int xDest, int yDest,
		CDibData& dibSrc, int xSrc, int ySrc);
	BOOL CopyPixel(int xDest, int yDest,
		CDibData& dibSrc, int xSrc, int ySrc);

	// Palette/Color table
	BOOL GetPaletteEntries(
		UINT uStartIndex, UINT cEntries, LPRGBQUAD pColors) const;
	BOOL SetPaletteEntries(
		UINT uStartIndex, UINT cEntries, const LPRGBQUAD pColors);
	UINT GetNearestPaletteIndex(const LPRGBQUAD pColors) const;
	UINT GetNearestPaletteIndex(COLORREF clrSrc) const;

	// DIB creation/initialization
	BOOL CreateDIB(CBitmap* pbmpSrc, BOOL bCompress = FALSE);
	BOOL CreateDIB(HBITMAP hBitmap, BOOL bCompress = FALSE);
	HBITMAP LoadDIB(LPCTSTR lpszPathName);

	// DIB storing
	BOOL SaveDIB(LPCTSTR lpszPathName,
		CBitmap* pbmpSrc = NULL, BOOL bCompress = FALSE);

	// Create converted copy of DIB
	CDibData* GetConvertedDIB(WORD wBitsPerPixel, BOOL bOptimize = FALSE);

	/////////////////////////////////////////////////////////////////////////
	// Direct DIB access
	LPBYTE GetDibPtr() const;			//!< Pointer to DIB
	DWORD GetDibSize() const;			//!< Total size of DIB
	LPBITMAPINFOHEADER GetHeaderPtr() const;//!< Pointer to DIB header
	DWORD GetHeaderSize() const;		//!< Size of header
	LONG  GetWidth() const;				//!< Width of image
	LONG  GetHeight() const;			//!< Height of image (may be negative)
	WORD  GetPlanes() const;			//!< Number of planes (=1)
	WORD  GetBitCount() const;			//!< Bits per plane
	DWORD GetCompression() const;		//!< BI_RGB, BI_RLE8, BI_RLE4, BI_BITFIELDS
	DWORD GetSizeImage() const;			//!< Image size (may be 0)
	LONG  GetXPelsPerMeter() const;
	LONG  GetYPelsPerMeter() const;
	DWORD GetClrUsed() const;			//!< Colors used, 0 to 256
	DWORD GetClrImportant() const;		//!< Important colors, 0 to 256

	WORD  GetBitsPerPixel() const;		//!< 1, 2, 4, 8, 16, 24, 32
	DWORD GetByteWidth() const;			//!< Bytes per scan line
	DWORD GetMaxImageSize() const;		//!< Maximum possible image size

	WORD  GetSizeOfBitFields() const;	//!< Bit field size or 0
	LPDWORD GetBitFeildsPtr();			//!< Pointer to bit fields or NULL

	WORD  GetMaxColors() const;			//!< Maximum colors, 0 to 256
	DWORD GetUsedColors() const;		//!< Used or maximum colors, 0 to 256

	DWORD GetSizeOfColorTable() const;	//!< Size of color table in bytes
	DWORD GetColorTableOffset() const;	//!< Offset to color table or 0
	LPRGBQUAD GetColorTablePtr() const;	//!< Pointer to color table or NULL

	DWORD GetImageOffset() const;		//!< Offset to image data
	LPBYTE GetImagePtr() const;			//!< Pointer to image data

	/////////////////////////////////////////////////////////////////////////////
	// Debug: Helpers
#ifdef _DEBUG
	friend void DumpDibFileHeader(const LPBITMAPINFOHEADER pbmfh);
	friend void DumpDibInfoHeader(const LPBYTE pDib);
	friend void DumpDibBitfields(const LPBYTE pDib);
	friend void DumpDibColorTable(const LPBYTE pDib);
	friend void DumpDib(const LPBYTE pDib);
	friend void DumpDibFile(LPCTSTR lpszPathName,BOOL bPixels);
#endif
};

/////////////////////////////////////////////////////////////////////////////
//! class CDibData inline functions

inline BOOL CDibData::
CopyPixelValue(int xDest, int yDest,
	CDibData& dibSrc, int xSrc, int ySrc)
{ return SetPixelValue(xDest, yDest, dibSrc.GetPixelValue(xSrc, ySrc)); }
inline BOOL CDibData::
CopyPixel(int xDest, int yDest,
	CDibData& dibSrc, int xSrc, int ySrc)
{ return SetPixel(xDest, yDest, dibSrc.GetPixel(xSrc, ySrc)); }

inline LPBYTE CDibData::GetDibPtr() const
{ return m_pDib; }
inline LPBITMAPINFOHEADER CDibData::GetHeaderPtr() const
{ ASSERT(m_pDib != NULL); return((LPBITMAPINFOHEADER)m_pDib); }
inline DWORD CDibData::GetHeaderSize() const
{ return(GetHeaderPtr()->biSize); }
inline LONG  CDibData::GetWidth() const
{ return(GetHeaderPtr()->biWidth); }
inline LONG  CDibData::GetHeight() const
{ return(GetHeaderPtr()->biHeight); }
inline WORD  CDibData::GetPlanes() const
{ return(GetHeaderPtr()->biPlanes); }
inline WORD  CDibData::GetBitCount() const
{ return(GetHeaderPtr()->biBitCount); }
inline DWORD CDibData::GetCompression() const
{ return(GetHeaderPtr()->biCompression); }
inline DWORD CDibData::GetSizeImage() const
{ return(GetHeaderPtr()->biSizeImage); }
inline LONG  CDibData::GetXPelsPerMeter() const
{ return(GetHeaderPtr()->biXPelsPerMeter); }
inline LONG  CDibData::GetYPelsPerMeter() const
{ return(GetHeaderPtr()->biYPelsPerMeter); }
inline DWORD CDibData::GetClrUsed() const
{ return(GetHeaderPtr()->biClrUsed); }
inline DWORD CDibData::GetClrImportant() const
{ return(GetHeaderPtr()->biClrImportant); }

inline WORD  CDibData::GetBitsPerPixel() const
{ return WORD(GetPlanes() * GetBitCount()); }

inline DWORD CDibData::GetByteWidth() const
{ return(((GetWidth() * GetBitsPerPixel() + 31) & ~31) >> 3); }

inline DWORD CDibData::GetMaxImageSize() const
{ return(GetByteWidth() * abs(GetHeight())); }

inline WORD  CDibData::GetSizeOfBitFields() const
{ return WORD((GetCompression() == BI_BITFIELDS) ? sizeof(m_dwBitfields) : 0); }

inline LPDWORD CDibData::GetBitFeildsPtr()
{ return(LPDWORD)(GetSizeOfBitFields() ? (m_pDib + GetHeaderSize()) : NULL); }

inline WORD CDibData::GetMaxColors() const
{ return WORD((GetBitsPerPixel() <= 8) ? (1 << GetBitsPerPixel()) : 0); }

inline DWORD CDibData::GetUsedColors() const
{ return(GetClrUsed() ? GetClrUsed() : GetMaxColors()); }

inline DWORD CDibData::GetSizeOfColorTable() const
{ return(GetUsedColors() * sizeof(RGBQUAD)); }

inline DWORD CDibData::GetColorTableOffset() const
{ return(GetHeaderSize() + GetSizeOfBitFields()); }

inline LPRGBQUAD CDibData::GetColorTablePtr() const
{ return(LPRGBQUAD)(GetUsedColors() ? (m_pDib + GetColorTableOffset()) : NULL); }

inline DWORD CDibData::GetImageOffset() const
{ return(GetColorTableOffset() + GetSizeOfColorTable()); }

inline LPBYTE CDibData::GetImagePtr() const
{ return(m_pDib + GetImageOffset()); }

inline DWORD CDibData::GetDibSize() const
{
	if( GetSizeImage() )
		return GetImageOffset() + GetSizeImage();
	return GetImageOffset() + GetMaxImageSize();
}

/////////////////////////////////////////////////////////////////////////////
//! Debug: Helpers

#ifdef _DEBUG

// Dib dump helpers
void DumpDibFileHeader(const LPBITMAPINFOHEADER pbmfh);
void DumpDibInfoHeader(const LPBYTE pDib);
void DumpDibBitfields(const LPBYTE pDib);
void DumpDibColorTable(const LPBYTE pDib);
void DumpDib(const LPBYTE pDib);
void DumpDibFile(LPCTSTR lpszPathName,BOOL bPixels);

#define DEBUG_TRACE_FHEADER(pbmfh)   DumpDibFileHeader(pbmfh)
#define DEBUG_TRACE_IHEADER(pDib)    DumpDibInfoHeader(pDib)
#define DEBUG_TRACE_BITFIELDS(pDib)  DumpDibBitfields(pDib)
#define DEBUG_TRACE_COLORTABLE(pDib) DumpDibColorTable(pDib)
#define DEBUG_TRACE_DIB(pDib)        DumpDib(pDib)
#define DEBUG_TRACE_DIBFILE(lpszPathName,bPixels) \
	DumpDibFile(lpszPathName,bPixels)

// Allocation helpers
HANDLE DibAlloc(DWORD dwLen);
LPBYTE DibLock(HANDLE hDib);
BOOL   DibUnlock(HANDLE hDib);
HANDLE DibFree(HANDLE hDib);

#else

// Dib dump helpers
#define DEBUG_TRACE_FHEADER(pbmfh)   ((void)0)
#define DEBUG_TRACE_IHEADER(pDib)    ((void)0)
#define DEBUG_TRACE_BITFIELDS(pDib)  ((void)0)
#define DEBUG_TRACE_COLORTABLE(pDib) ((void)0)
#define DEBUG_TRACE_DIB(pDib)        ((void)0)
#define DEBUG_TRACE_DIBFILE(lpszPathName, bPixels) ((void)0)

// Allocation helpers
#define DibAlloc(dwLen) ::GlobalAlloc(GHND, dwLen)
#define DibLock(hDib)   (LPBYTE)::GlobalLock(hDib)
#define DibUnlock(hDib) ::GlobalUnlock(hDib)
#define DibFree(hDib)   ::GlobalFree(hDib)

#endif // _DEBUG

#endif // __CDIBDATA_H__

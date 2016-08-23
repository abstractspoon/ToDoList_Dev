// CDibData.cpp
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

//
// Purpose:
//   The CDibData class was created to allow direct access to
//   uncompressed image bits and for saving bitmap files,
//   given any valid bitmap handle.
//
// Expanded purpose:
//   Allow opening and color depth conversion of bitmaps.
//
// Possible problems:
//   1) Loss of some of the original bitmap header information.
//   2) Keeping DIB memory block in sync with original bitmap.
//

// Notes:
//
// 16-bit DIBs:
// Question:
//   What happen when you open a 16-bit DIB that does not have
//   biCompression = BI_BITFIELDS then call
//   GetDIBits() to save?
// Answer:
//   GetDIBits() generates a DIB with biCompression = BI_BITFIELDS
//   and bit-fields set to default values.
// Conclusion:
//   Always save with biCompression = BI_BITFIELDS and with the
//   bit-fields set to the appropriate values.
//   ( Default: PAL_BGR { FF0000, 00FF00, 0000FF }
//           or PAL_RGB { 0000FF, 00FF00, FF0000 } )
//
// GetDIBits() always returns a bottom-up bitmap even
// if the original was top-down.
//
// BUG: LoadImage() can not load top-down bitmap.
// (pBitmapInfo->bmiHeader.biHeight < 0)
// If LoadImage() loads bitmap from a file and invalid argument
// error occurs.
// If LoadImage() loads bitmap w/DIBSECTION from a file then
// an image size to large error occurs (not enough memory).
// Conclusion: LoadImage() ass-u-mes biHeight > 0 and therefore
// all calculations give invalid values.
//
// GetObject(hdc, sizeof(DIBSECTION), (LPVOID)&dibSection);
// dibSection.dsBm contains correct BITMAP data for non-DIBSECTION
// if bitmap is not a DIBSECTION, all other data is invalid.
//
// WinXP (file open dialog) .BMP properties reports BI_RLE8 compressed as
// having a depth of 32 instead of 8.
//
// biImageSize not always set ?
// Just purchased an HP computer and discovered that some of the bitmaps
// included with Windows XP Home Edition have been altered.
// (biImageSize = 0, biXPelsPerMeter set, biYPelsPerMeter set)
//

// References:
//
// "Programming Windows Fifth Edition" by Charles Petzold
//
// MSDN
//
// The Palette Manager: How and Why
//
// More Fun with MFC:
// DIBs, Palettes, Subclassing, and a Gamut of Reusable Goodies
// By Paul DiLascia 
//
// BUG: TrueType Glyphs Wrong Color Rasterized to 8bpp DIBSection
// ID: Q180008 
// Key words: DIBSECTION
//
// BUG: GetObject Returns Partial Information for DIBSections
// ID: Q151072 (applies to file-mapped DIBSection)
// Key words: DIBSECTION
//
// BUG: Incorrect Painting Using Pattern Brushes on DIBSections
// ID: Q149956 
// RESOLUTION: Use CreateDIBPatternBrushPt() on a DIBSection having a
// different bit-depth than the screen.
// Key words: DIBSECTION
//
// "PALLETIZED.CPP" By MarkFi, April 1998
// Key words: DIBSECTION
//
// "Microsoft Windows CE Display Drivers and Hardware"
// Excellent explanation of bits-per-pixel format.
// Key words: PALLETIZED.CPP
//
// See CreateFont() and LOGFONT: ANTIALIASED_QUALITY
// Key words: DIBSECTION
//
// "How to Reference Colors in a DIB Section by Index"
// ID: Q138256
// Key words: DIBSECTION
//
// "Working with Colors" (Printing, DIBSECTION, CE, etc...)
// "An offscreen DIB section should have the same color table as the screen;
// otherwise, GDI will have to perform a time-consuming color translating
// blit when the DIB section is transferred to the screen. For grayscale
// devices, the color table should be 0x000000, 0x808080, 0xc0c0c0,
// and 0xFFFFFF. For color devices, the application should first query the
// standard palette to determine its color display capabilities, and then
// build a matching color table."
// Key words: DIBSECTION
//
// DIBs and Their Uses By Ron Gery
// ID: Q81498
// Key words: "HBITMAP AND DIB"
//
// HOWTO: Distinguish Between DIB Sections & Device-Dependent Bit
// IsDIBSection()
// ID: Q187539 
// Key words: "HBITMAP AND DIB"
//
// SetBitmapDimensionEx(): "The bitmap cannot be a DIB-section bitmap."
//
// PRB: Dithered Brushes Are Not Supported in DIB Sections
// ID: Q137370 
//
// CreateDIBSection()
// Windows NT/ 2000: You need to guarantee that the GDI subsystem has
// completed any drawing to a bitmap created by CreateDIBSection before
// you draw to the bitmap yourself. Access to the bitmap must be
// synchronized. Do this by calling the GdiFlush function. This applies
// to any use of the pointer to the bitmap's bit values, including passing
// the pointer in calls to functions such as SetDIBits.
//
// "Bitmap Compression"
// Key words: "RLE Compression"
//
// INFO: Using GetDIBits()
// ID: Q74601
// Key words: GetDIBits
//
// INFO: General limitations under Win32s
// ID: Q131896 
// "This means that an application cannot know the format of the bits
// returned by GetBitmapBits() and should not attempt to directly manipulate
// them."
// Key words: GetDIBits
//
// "Animation in Windows"
// MORE WARNINGS!!!
// Key words: GetDIBits
//
// HOWTO: Get the Color Table of a DIBSection in Windows CE
// ID: Q196650 
// Key words: DIBSECTION
//
// HOWTO: How To Use LoadImage() to Read a BMP File
// ID: Q158898 
// Key words: DIBSECTION
//
// INFO: Resource Bitmaps Are Read-Only in Windows CE
// ID: Q168258 
// Key words: DIBSECTION
//
// Wicked Code By Jeff Prosise (1996/97)
// Key words: DIBSECTION, "Color Quantization", "Octree Color"
//
// Wicked Code By Jeff Prosise (1997)
// "The strategy here is to use ::GetDIBits to convert the
// image into a 24-bit DIB one scan line at a time. A pleasant
// side effect of using ::GetDIBits in this manner is that RLE-
// encoded 4-bit and 8-bit DIBs will be uncompressed."
// Key words: DIBSECTION, "CQuantizer Class"
//
// Problems Converting Some Bitmap Files from Windows 98
// ID: Q195986
// Key words: "RLE Compression"
//
// Using Color and Grayscale Palettes
// "Some Windows CE based devices support only a 2-bpp palette,
// with four gray-scale colors: black, white, light gray, and dark gray."
//

// Of interest:
// "Double Buffering with Class!" By Michele Leroux
//
// "How to Exploit Multiple Monitor Support in Memphis and Windows NT 5.0"
// By David Campbell
// "GetOptimalDIBFormat()"
// Key words: "HBITMAP AND DIB"
//
// HOWTO: Retrieving an Optimal DIB Format for a Device
// ID: Q230492 
// Key words: "HBITMAP AND DIB"
//
// INFO: Retrieving Palette Information from a Bitmap Resource
// ID: Q124947
// Key words: "HBITMAP AND DIB"
//
// HOWTO: Get the Color Table of a DIBSection in Windows CE
// ID: Q196650 
// Key words: "HBITMAP AND DIB"
//
// Bit2Mono.exe Changes Color Bitmaps to Monochrome
// ID: Q77282 
// Key words: "HBITMAP AND DIB"
//
// INFO: Rotating a Bitmap by 90 Degrees
// ID: Q77127 
// Key words: "HBITMAP AND DIB"
//
// "Storing an Image"
// Key words: "HBITMAP AND DIB"
//
// C++ Q & A By Paul DiLascia (1997)
// Partial quote By Eddie Diener:
// "OWL uses multiple inheritance and you can easily derive from more
// than one window type to create another type of window."
//
// "sRGB: A Standard Color Space"
//
// "Creating Bitmaps"
// (2 bits per pixel is supported on Windows CE)
// Windows CE supports arbitrary bit pixel formats, which enable you
// to use blit functions among bitmaps with different pixel depths.
// Key words: DIBSECTION
//
// FADEIN App Uses Palette Animation & Identity Palettes
// ID: Q149855
// Key words: DIBSECTION
//
// HOWTO: Capture and Print an Entire Window
// ID: Q186736 
// Key words: DIBSECTION
// 
// Support for BITMAPV4HEADER & BITMAPV5HEADER.
// (See "Using Structures in ICM 2.0")
//

#include "stdafx.h"

//#include "MyTrace.h"
#include "DibData.h"

IMPLEMENT_DYNAMIC(CDibData,CObject)

/////////////////////////////////////////////////////////////////////////////
//! class CDibDataVars
//!
//! This class is used to centralize some of the calculations needed by
//! several functions in the CDibData class.
//!
class CDibDataVars
{
	CDibDataVars::CDibDataVars() {}	// No default constructor
public:
	CDibDataVars(const LPBITMAPINFOHEADER pInfo);
	~CDibDataVars() {}

	void SetDataVars(const LPBITMAPINFOHEADER pInfo);

	DWORD m_dwCompression;		//!< Compression type
	WORD  m_wBpp;				//!< Number of bits-per-pixel
	DWORD m_dwByteWidth;		//!< Number of bytes-per-line
	DWORD m_dwImageSize;		//!< Size of image
	DWORD m_dwDibSize;			//!< Size of DIB
	DWORD m_dwFieldSize;		//!< Size of bit-fields section
	DWORD m_dwNumColors;		//!< Number of colors used
	DWORD m_dwSizeColorTable;	//!< Size of color table
	DWORD m_dwImageOffset;		//!< Offset to image data
	DWORD m_dwFieldOffset;		//!< Offset to bit-fields
	DWORD m_dwColorOffset;		//!< Offset to color-table
	DWORD m_dwMaxDibSize;		//!< Maximum uncompressed DIB size
	DWORD m_dwMaxColor;			//!< Maximum number of possible colors
};


// CDibDataVars::CDibDataVars(const LPBITMAPINFOHEADER pInfo)
/**
\par Parameters:
  pInfo
		[in] Pointer to bitmap information header.

\par Remarks:
  Constructs a CDibDataVars object.
*/
CDibDataVars::CDibDataVars(const LPBITMAPINFOHEADER pInfo)
{ SetDataVars(pInfo); }

// CDibDataVars::SetDataVars(const LPBITMAPINFOHEADER pInfo)
/**
\par Return Value:
  void.

\par Parameters:
  pInfo
		[in] Pointer to bitmap information header.

\par Remarks:
  This function calculates values needed by several functions in the
  CDibData class.

\par
  There is no error checking for this utility class/function.
*/
void CDibDataVars::SetDataVars(const LPBITMAPINFOHEADER pInfo)
{
	m_dwCompression = pInfo->biCompression;
	m_wBpp          = WORD(pInfo->biPlanes * pInfo->biBitCount);
	m_dwByteWidth   = (((pInfo->biWidth * m_wBpp + 31) & ~31) >> 3);
	m_dwImageSize   = m_dwByteWidth * abs(pInfo->biHeight);
	m_dwMaxDibSize  = pInfo->biSize + m_dwImageSize;
	m_dwDibSize     = pInfo->biSize;
	m_dwFieldSize   = 0L;
	m_dwMaxColor    = 0L;
	m_dwNumColors   = 0L;
	m_dwImageOffset = 0L;
	m_dwFieldOffset = 0L;
	m_dwColorOffset = 0L;

	if( m_wBpp <= 1 )
		m_wBpp = 1;
	else if( m_wBpp <= 2 )
		m_wBpp = 2;
	else if( m_wBpp <= 4 )
		m_wBpp = 4;
	else if( m_wBpp <= 8 )
		m_wBpp = 8;
	else if( m_wBpp <= 16 )
		m_wBpp = 16;
	else if( m_wBpp <= 24 )
		m_wBpp = 24;
	else
		m_wBpp = 32;

	if( pInfo->biSizeImage )
		m_dwImageSize = pInfo->biSizeImage;

	if( BI_BITFIELDS == m_dwCompression )
	{
		m_dwFieldOffset = m_dwDibSize;
		m_dwFieldSize   = 3 * sizeof(DWORD);
		m_dwDibSize    += m_dwFieldSize;

		m_dwMaxDibSize += m_dwFieldSize;
	}

	if( m_wBpp <= 8 )
	{
		m_dwMaxColor = 1 << m_wBpp;
		if( pInfo->biClrUsed )
			m_dwNumColors = pInfo->biClrUsed;
		else
			m_dwNumColors = m_dwMaxColor;

		m_dwColorOffset    = m_dwDibSize;
		m_dwSizeColorTable = m_dwNumColors * sizeof(RGBQUAD);
		m_dwDibSize       += m_dwSizeColorTable;

		m_dwMaxDibSize    += m_dwMaxColor * sizeof(RGBQUAD);
	}

	m_dwImageOffset = m_dwDibSize;
	m_dwDibSize    += m_dwImageSize;
}

/////////////////////////////////////////////////////////////////////////////
// class CDibData

// CDibData::CDibData(const CDibData& dibSrc)
/**
\par Parameters:
  dibSrc
		[in] Source DIB used to initialize this object.

\par Remarks:
  Copy constructor for CDibData object.
*/
CDibData::CDibData(const CDibData& dibSrc)
{
	ASSERT(dibSrc.m_hDib != NULL);
//	MYTRACE(1, _T("CDibData::CDibData(const CDibData&)\n"));

	// Initialize normally
	m_hDib             = NULL;
	m_pDib             = NULL;
	m_pBits            = NULL;
	m_pLine            = NULL;
	m_pColorTable      = NULL;
	m_wBitsPerPixel    = 0;
	m_dwColorTableSize = 0;

	::ZeroMemory(m_dwBitfields, sizeof(m_dwBitfields));
	::ZeroMemory(m_wRShift    , sizeof(m_wRShift));
	::ZeroMemory(m_wLShift    , sizeof(m_wLShift));

	// Copy srource DIB
	DWORD dwDibSize = ::GlobalSize(dibSrc.m_hDib);
	if( dibSrc.m_hDib && dwDibSize )
	{
		HANDLE hDib = DibAlloc(dwDibSize);
		if( hDib )
		{
			LPBYTE pDib = DibLock(hDib);
			if( !pDib )
			{
				DibFree(hDib);
				//MYTRACE(0, _T("CDibData copy ctor failed to LOCK!\n"));
			}
			else
			{
				::CopyMemory(pDib, dibSrc.m_pDib, dwDibSize);
				DibUnlock(hDib);

				// Attach copy to this
				if( !Attach(hDib) )
				{
					DibFree(hDib);
					//MYTRACE(0, _T("CDibData copy ctor failed to ATTACH!\n"));
				}
			}
		}
	}

//	MYTRACE(-1, _T("CDibData::CDibData(const CDibData&)\n"));
}

// CDibData::CDibData()
/**
\par Remarks: 
  Constructs a CDibData object. The resulting object must be
  initialized with one of the initialization member functions.
*/
CDibData::CDibData() :
	m_hDib(NULL), m_pDib(NULL), m_pBits(NULL),
	m_pColorTable(NULL), m_pLine(NULL),
	m_wBitsPerPixel(0), m_dwColorTableSize(0)
{
	//MYTRACE(1, _T("CDibData::CDibData()\n"));
	::ZeroMemory(m_dwBitfields, sizeof(m_dwBitfields));
	::ZeroMemory(m_wRShift    , sizeof(m_wRShift));
	::ZeroMemory(m_wLShift    , sizeof(m_wLShift));
	//MYTRACE(-1, _T("CDibData::CDibData()\n"));
}

// CDibData::~CDibData()
/**
\par Remarks:
  Deconstructs a CDibData object.
*/
CDibData::~CDibData()
{
	//MYTRACE( 1, _T("CDibData::~CDibData()\n"));
	DeleteObject();
	//MYTRACE(-1, _T("CDibData::~CDibData()\n"));
}

// BOOL CDibData::DeleteObject();
/**
\par Return Value:
  Always nonzero.

\par Remarks:
  Frees all system storage associated with this object.
*/
BOOL CDibData::DeleteObject()
{
	ASSERT_VALID(this);
	//MYTRACE(1,_T("CDibData::DeleteObject()\n"));

	if( m_hDib && !DibUnlock(m_hDib) )
		DibFree(m_hDib);
	delete [] m_pLine;

	m_hDib             = NULL;
	m_pDib             = NULL;
	m_pBits            = NULL;
	m_pLine            = NULL;
	m_pColorTable      = NULL;
	m_wBitsPerPixel    = 0;
	m_dwColorTableSize = 0;

	::ZeroMemory(m_dwBitfields, sizeof(m_dwBitfields));
	::ZeroMemory(m_wRShift    , sizeof(m_wRShift));
	::ZeroMemory(m_wLShift    , sizeof(m_wLShift));

	//MYTRACE(-1,_T("CDibData::DeleteObject() = TRUE\n"));
	return TRUE;
}

// BOOL CDibData::Attach(HANDLE hDib)
/**
\par Return Value:
  Nonzero on success; otherwise 0.

\par Parameters:
  hDib
		[in] A handle to a DIB object.

\par Remarks:
  Attaches DIB handle to this object.
*/
BOOL CDibData::Attach(HANDLE hDib)
{
	ASSERT_VALID(this);
	ASSERT(hDib != NULL);
	ASSERT(m_hDib == NULL);
	//MYTRACE(1,_T("CDibData::Attach(hDib = 0x%p)\n"), hDib);

	// Can a new handle be attached ?
	if( !hDib || m_hDib )
	{
		//MYTRACE(-1, _T("CDibData::Attach() = FALSE\n"));
		if( !hDib )
			::SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}

	// Check size (also checks if handle is valid)
	DWORD dwGblSize = ::GlobalSize(hDib);
	if( dwGblSize < sizeof(BITMAPINFOHEADER) )
	{
		//MYTRACE(-1, _T("CDibData::Attach() = FALSE\n"));
		if( ::GetLastError() == ERROR_SUCCESS )
			::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE; // Bad handle or not a DIB handle
	}

	// Can we lock it ?
	LPBYTE pDib = DibLock(hDib);
	if( !pDib )
	{
		//MYTRACE(-1, _T("CDibData::Attach() = FALSE\n"));
		return FALSE;
	}
	LPBITMAPINFOHEADER pInfo = (LPBITMAPINFOHEADER)pDib;

	// Is the header size correct?
	if( sizeof(BITMAPINFOHEADER) != pInfo->biSize /*&&
		sizeof(BITMAPV4HEADER)   != pInfo->biSize &&
		sizeof(BITMAPV5HEADER)   != pInfo->biSize*/ )
	{
		DibUnlock(hDib);
		//MYTRACE(-1, _T("CDibData::Attach() = FALSE\n"));
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	// Init. local variables
	CDibDataVars ddv(pInfo);

	// Is it large enough to hold a DIB?
	if( ddv.m_dwDibSize > dwGblSize )
	{
		DibUnlock(hDib);
		//MYTRACE(-1, _T("CDibData::Attach() = FALSE\n"));
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	// We appear to have a DIB handle
	// Now we need to initialize internal variables

	// 1st Try to allocate line pointer array
	m_pLine = new LPBYTE[abs(pInfo->biHeight)];
	if( !m_pLine )
	{
		DibUnlock(hDib);
		//MYTRACE( 0, _T("Memory allocation failer\n"));
		//MYTRACE(-1, _T("CDibData::Attach() = FALSE\n"));
		::SetLastError(ERROR_OUTOFMEMORY);
		return FALSE;
	}

	// 2nd initialize bit-fields
	if( !ddv.m_dwFieldOffset )
	{
		SetDefaultBitfields(ddv.m_wBpp);
	}
	else
	{
		// Make copy of bit-fields
		::CopyMemory(m_dwBitfields,
			pDib + ddv.m_dwFieldOffset, sizeof(m_dwBitfields));

		// If: bit-fields are empty
		if( m_dwBitfields[0] == 0L &&
			m_dwBitfields[0] == 0L &&
			m_dwBitfields[1] == 0L )
		{
			SetDefaultBitfields(ddv.m_wBpp);

			// Fix DIB bit-fields
			::CopyMemory(pDib + ddv.m_dwFieldOffset,
				m_dwBitfields, sizeof(m_dwBitfields));
		}
		// Else: Set shift values
		else
		{
			DWORD dwMask;
			WORD  dwShift;
			for( int i = 0; i < 3; ++i )
			{
				// Shift mask
				dwMask = m_dwBitfields[i];
				if( !dwMask )
					continue;

				// Right shift
				for( dwShift = 0; !(dwMask & 1); ++dwShift )
					dwMask >>= 1;
				m_wRShift[i] = dwShift;

				// Left shift
				for( dwShift = 0; dwMask & 1; ++dwShift )
					dwMask >>= 1;
				m_wLShift[i] = WORD(8 - dwShift);
			}
		}
	}

	// 3rd Initialize all remaining internal variables
	m_hDib             = hDib;
	m_pDib             = pDib;
	m_pBits            = pDib + ddv.m_dwImageOffset;
	m_wBitsPerPixel    = ddv.m_wBpp;
	m_dwColorTableSize = ddv.m_dwNumColors;

	if( ddv.m_dwColorOffset )
		m_pColorTable = LPRGBQUAD(pDib + ddv.m_dwColorOffset);

	if( pInfo->biHeight > 0 )
	{
		LONG y = pInfo->biHeight - 1;
		for( LONG i = 0; y >= 0; --y, ++i )
		{
			m_pLine[i] = m_pBits + y * ddv.m_dwByteWidth;
		}
	}
	else
	{
		LONG lHeight = abs(pInfo->biHeight);
		for( LONG i = 0; i < lHeight; ++i )
		{
			m_pLine[i] = m_pBits + i * ddv.m_dwByteWidth;
		}
	}

	//MYTRACE(-1, _T("CDibData::Attach() = TRUE\n"));
	return TRUE;
}

// HANDLE CDibData::Detach()
/**
\par Return Value:
  A handle to a global DIB memory block.

\par Remarks:
  Call this member to detach m_hDib from the CDibData object.
*/
HANDLE CDibData::Detach()
{
	//MYTRACE(1, _T("CDibData::Detach()\n"));
	HGLOBAL hDib = m_hDib;
	if( m_hDib )
	{
		DibUnlock(m_hDib);
		m_hDib = NULL;
		DeleteObject();
	}
	//MYTRACE(-1, _T("CDibData::Detach() = 0x%p\n"), hDib);
	return hDib;
}

// void CDibData::SetDefaultBitfields(WORD wBitsColor)
/**
\par Return Value:
  void.

\par Parameters:
  wBitsColor
		[in] Number of bits per color.

\par Remarks:
  Sets default DIB bitfields (and shift values) used internally
  for working with 16, 24, and 32 bit DIBs.
*/
void CDibData::SetDefaultBitfields(WORD wBitsColor)
{
	if( wBitsColor == 16 )
	{
		m_dwBitfields[0] = 0x00007C00;
		m_dwBitfields[1] = 0x000003E0;
		m_dwBitfields[2] = 0x0000001F;

		m_wRShift[0] = 10;
		m_wRShift[1] =  5;
		m_wRShift[2] =  0;

		m_wLShift[0] = m_wLShift[1] = m_wLShift[2] = 3;
	}
	else if(wBitsColor == 24 || wBitsColor == 32 )
	{
		m_dwBitfields[0] = 0x00FF0000;
		m_dwBitfields[1] = 0x0000FF00;
		m_dwBitfields[2] = 0x000000FF;

		m_wRShift[0] = 16;
		m_wRShift[1] =  8;
		m_wRShift[2] =  0;

		m_wLShift[0] = m_wLShift[1] = m_wLShift[2] = 0;
	}
}

// HANDLE CDibData::CreateBitmapInfo(CBitmap* pbmpSrc, BOOL* pbFinished)
/**
\par \par Return Value:
  Handle to DIB object; otherwise NULL.

\par 
  To get extended error information, call GetLastError. 

\par Parameters:
  pbmpSrc
		[in] Pointer to the source bitmap used to create the
		DIB object. Must not be NULL.

\par
  pbFinished
		[out] Pointer to finished flag.
		Must not be NULL.

\par Remarks:
  This member is called internally to create a DIB object with
  only the DIB information header [and color-table] initialized.

\par
  If this function returns a non-NULL value and the finished flag
  is set, then the entire DIB object has been initialized.

\par 
  For 16 and 24 bit image the compression field is always set
  to BI_BITFIELDS.

\par 
  For 1, 2, 4, and 8 bit DIBs, the bitmap identified by the pbmpSrc
  parameter must not be selected into a device context when the
  application calls this member. 
*/
HANDLE CDibData::CreateBitmapInfo(CBitmap* pbmpSrc, BOOL* pbFinished)
{
	ASSERT_VALID(this);
	ASSERT(pbmpSrc != NULL);
	ASSERT(m_pDib == NULL);
	ASSERT(pbFinished != NULL);

	if( pbFinished )
		*pbFinished = FALSE;

	//MYTRACE(1,_T("CDibData::CreateBitmapInfo(pbmpSrc = 0x%p)\n"), pbmpSrc);
	if( m_pDib || !pbmpSrc || !pbmpSrc->GetSafeHandle() )
	{
		//MYTRACE(-1,_T("CDibData::CreateBitmapInfo() = NULL\n"));
		if( !pbmpSrc )
			::SetLastError(ERROR_INVALID_PARAMETER);
		else if( !pbmpSrc->GetSafeHandle() )
			::SetLastError(ERROR_INVALID_HANDLE);
		else
			::SetLastError(ERROR_ALREADY_INITIALIZED);
		return NULL;
	}
		
	// Get bitmap info.
	DIBSECTION ds;
	::ZeroMemory(&ds, sizeof(ds));
	if( !pbmpSrc->GetObject(sizeof(ds), (LPVOID)&ds) )
	{
		//MYTRACE(-1,_T("CDibData::CreateBitmapInfo() = NULL\n"));
		if( ::GetLastError() == ERROR_SUCCESS )
			::SetLastError(ERROR_INVALID_PARAMETER);
		return NULL;
	}

	// Determine if it is a dib-section
	BOOL bDibSection = (ds.dsBm.bmBits != NULL);

	// Init. local variables
	BITMAPINFOHEADER bmih;
	::ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));
	if( bDibSection )
	{
		::CopyMemory(&bmih, &ds.dsBmih, sizeof(BITMAPINFOHEADER));
	}
	else
	{
		bmih.biSize        = sizeof(BITMAPINFOHEADER); 
		bmih.biWidth       = ds.dsBm.bmWidth;
		bmih.biHeight      = ds.dsBm.bmHeight;
		bmih.biPlanes      = ds.dsBm.bmPlanes;
		bmih.biBitCount    = ds.dsBm.bmBitsPixel;
		bmih.biClrUsed     = 0L;
		bmih.biCompression = BI_RGB;

		DWORD bpp = bmih.biPlanes * bmih.biBitCount;
		if( bpp <= 8 )
			bmih.biClrUsed = 1 << bpp;

		// Always use bit-field compression for 16 & 32 bit DIBs
		CDibDataVars ddv(&bmih);

		if( 16 == ddv.m_wBpp || ddv.m_wBpp == 32 )
			bmih.biCompression = BI_BITFIELDS;
	}

	CDibDataVars ddv(&bmih);
	m_dwColorTableSize = ddv.m_dwSizeColorTable;

	// Get bitfield masks if needed
	if( bDibSection && bmih.biCompression == BI_BITFIELDS )
	{
		DWORD dwMask;
		WORD  dwShift;
		::CopyMemory(m_dwBitfields, ds.dsBitfields, sizeof(m_dwBitfields));
		for( int i = 0; i < 3; ++i )
		{
			// Shift mask
			dwMask = m_dwBitfields[i];
			if( !dwMask )
				continue;

			// Right shift
			for( dwShift = 0; !(dwMask & 1); ++dwShift )
				dwMask >>= 1;
			m_wRShift[i] = dwShift;

			// Left shift
			for( dwShift = 0; dwMask & 1; ++dwShift )
				dwMask >>= 1;
			m_wLShift[i] = WORD(8 - dwShift);
		}
	}
	else
	{
		SetDefaultBitfields(ddv.m_wBpp);
	}

	// Allocate storage
	HANDLE hDib = DibAlloc(ddv.m_dwMaxDibSize);
	LPBYTE pDib = DibLock(hDib);
	if( !hDib || !pDib )
	{
		if( hDib )
			DibFree(hDib);
		//MYTRACE(-1,_T("CDibData::CreateBitmapInfo() = NULL\n"));
		return NULL;
	}
	
	// Fill in Information Header
	::CopyMemory(pDib, &bmih, sizeof(BITMAPINFOHEADER) );

	if( !bDibSection )
		((LPBITMAPINFOHEADER)pDib)->biCompression = ddv.m_dwCompression;

	if( ddv.m_dwFieldOffset )
	{
		::CopyMemory(LPDWORD(pDib + ddv.m_dwFieldOffset),
			m_dwBitfields, sizeof(m_dwBitfields));
	}

	if( ddv.m_dwColorOffset )
	{
		if( bDibSection )
		{
			HDC hdc = ::CreateCompatibleDC(NULL);
			if( hdc )
			{
				HGDIOBJ hObj = 
					::SelectObject(hdc, pbmpSrc->GetSafeHandle());
				(DWORD)::GetDIBColorTable(hdc, 0, ddv.m_dwNumColors,
					LPRGBQUAD(pDib + ddv.m_dwColorOffset));
				::SelectObject(hdc, hObj);
				::DeleteDC(hdc);
			}
#ifdef _DEBUG
			else
			{
				//MYTRACE(0,_T("Can not create screen compatible DC\n"));
			}
#endif
		}
		// else Create a color table ???
	}

	if( pbFinished && bDibSection )
	{
		if( bmih.biCompression == BI_RLE4 || bmih.biCompression == BI_RLE8 )
		{
			::CopyMemory(pDib + ddv.m_dwImageOffset,
				ds.dsBm.bmBits, ddv.m_dwImageSize);
			*pbFinished = TRUE;
		}
	}

	DibUnlock(hDib);
	//MYTRACE(-1,_T("CDibData::CreateBitmapInfo() = 0x%p\n"), hDib);
	return hDib;
}

// BOOL CDibData::CreateDIB(CBitmap* pbmpSrc, BOOL bCompress = FALSE)
/**
\par Return Value:
  Nonzero on success; otherwise 0.

\par 
  To get extended error information, call GetLastError. 

\par Parameters:
  pbmpSrc
		[in] Points to the source bitmap used to create DIB.
		Must not be NULL.
\par
  bCompress
		[in] A flag indicating whether the resulting DIB should
		be compressed or not.

\par Remarks:
  This member is called to create a DIB: containing the
  information header, [bitfields, color table,] and image bits.

\par
  If bCompress is set to TRUE, RLE is used to compress 4 and 8
  bit DIBs. If the compressed size would result in an image size
  greater than the uncompressed size, compression is not used.

\par
  For 16 and 24 bit image the compression field is always set
  to BI_BITFIELDS.

\par
  For 1, 2, 4, and 8 bit DIBs, the bitmap identified by the pbmpSrc
  parameter must not be selected into a device context when the
  application calls this member. 
*/
BOOL CDibData::CreateDIB(CBitmap* pbmpSrc, BOOL bCompress/* = FALSE*/)
{
	ASSERT_VALID(this);
	ASSERT(pbmpSrc != NULL);
	ASSERT(m_pDib == NULL);
	//MYTRACE(1,_T("CDibData::CreateDIB(pbmpSrc = 0x%p, %s)\n"),
	//	pbmpSrc, MYGETBOOLSTR(bCompress));

	// Create partially initialized DIB
	BOOL bFinished;
	HANDLE hDib = CreateBitmapInfo(pbmpSrc, &bFinished);

	if( !hDib )
	{
		//MYTRACE(-1,_T("CDibData::CreateDIB() = NULL\n"));
		return FALSE;
	}

	if( bFinished )
	{
		if( !Attach(hDib) )
		{
			DibFree(hDib);
			//MYTRACE(-1,_T("CDibData::CreateDIB() = NULL\n"));
			return FALSE;
		}
		else
		{
			//MYTRACE(-1,_T("CDibData::CreateDIB() = TRUE\n"));
			return TRUE; 
		}
	}

	LPBYTE pDib = DibLock(hDib);
	if( !pDib )
	{
		DibFree(hDib);
		//MYTRACE(-1,_T("CDibData::CreateDIB() = NULL\n"));
		return FALSE;
	}

	LPBITMAPINFO pBmpInfo             = (LPBITMAPINFO)pDib;
	LPBITMAPINFOHEADER pBmpInfoHeader = (LPBITMAPINFOHEADER)pDib;
	CDibDataVars ddv(pBmpInfoHeader);

	// Set compression type
	if( bCompress )
	{
		if( ddv.m_wBpp == 4 )
			pBmpInfoHeader->biCompression = BI_RLE4;
		else if( ddv.m_wBpp == 8 )
			pBmpInfoHeader->biCompression = BI_RLE8;
	}

	// Get correct BITMAPINFO data for compressed/uncompressed DIB
	HDC hdc = ::CreateCompatibleDC(NULL);
	if( !hdc )
	{
		//MYTRACE(0,_T("Can not create screen compatible DC\n"));
		DibUnlock(hDib);
		DibFree(hDib);
		//MYTRACE(-1,_T("CDibData::CreateDIB() = FALSE\n"));
		return FALSE;
	}

GET_UNCOMPRESSED_IMAGE:

	BOOL bError = !::GetDIBits(hdc, (HBITMAP)pbmpSrc->GetSafeHandle(),
		0, abs(pBmpInfoHeader->biHeight), NULL, pBmpInfo, DIB_RGB_COLORS);

	// If driver can not compress images
	if( bError || pBmpInfoHeader->biSizeImage == 0 )
	{
		if( bCompress )
		{
			//MYTRACE(0,_T("Driver could not compress image!\n"));
			//MYTRACE(0,_T("Trying to get uncompressed image\n"));
			// Try uncompressed image
			bCompress = FALSE;
			pBmpInfoHeader->biSizeImage   = ddv.m_dwImageSize;
			pBmpInfoHeader->biCompression = BI_RGB;
			pBmpInfoHeader->biClrUsed     = ddv.m_dwNumColors;
			goto GET_UNCOMPRESSED_IMAGE;
		}

		// Driver can not get uncompressed image either
		//MYTRACE(0,_T("Driver can not get uncompressed image!\n"));
		::DeleteDC(hdc);
		DibUnlock(hDib);
		DibFree(hDib);
		//MYTRACE(-1,_T("CDibData::CreateDIB() = FALSE\n"));
		if( ::GetLastError() == ERROR_SUCCESS )
			::SetLastError(ERROR_BAD_DRIVER_LEVEL);
		return FALSE;
	}

	// Must try uncompressed image if smaller
	// or there will be a buffer overrun.
	if( pBmpInfoHeader->biCompression == BI_RLE4 ||
		pBmpInfoHeader->biCompression == BI_RLE8 )
	{
		if( ddv.m_dwImageSize < pBmpInfoHeader->biSizeImage )
		{
			//MYTRACE(0,_T("COMPRESSED[%u] > UNCOMPRESSED[%u]!\n"),
			//	pBmpInfoHeader->biSizeImage, ddv.m_dwImageSize);
			//MYTRACE(0,_T("Getting uncompressed image\n"));

			pBmpInfoHeader->biSizeImage   = ddv.m_dwImageSize;
			pBmpInfoHeader->biCompression = BI_RGB;
			pBmpInfoHeader->biClrUsed     = ddv.m_dwNumColors;
			goto GET_UNCOMPRESSED_IMAGE;
		}
		/*
		// TESTING COMPRESSED FILES
		if( ddv.m_dwImageSize < pBmpInfoHeader->biSizeImage )
		{
			CDibDataVars ddvTemp(pBmpInfoHeader);
			DibUnlock(hDib);
			HANDLE hTempDib = ::GlobalReAlloc(hDib, ddvTemp.m_dwDibSize, GHND);
			if( hTempDib )
			{
				hDib = hTempDib;
				pDib = DibLock(hDib);
				pBmpInfo = (LPBITMAPINFO)pDib;
				pBmpInfoHeader = (LPBITMAPINFOHEADER)pDib;
				::CopyMemory(&ddv, &ddvTemp, sizeof(CDibDataVars));
			}
		}
		*/
	}

	// Get ready to copy data to DIB
	ddv.SetDataVars(pBmpInfoHeader);	// Insure m_dwImageOffset is correct
	LPBYTE pBits = pDib + ddv.m_dwImageOffset;

	// Copy image to DIB
	if( ddv.m_dwImageSize < pBmpInfoHeader->biSizeImage )
	{
		//MYTRACE(0,_T("THIS SHOULD NOT HAPPEN!\n"));
		//MYTRACE(0,_T("GDI[%u] > CALC[%u]\n"),
		//	pBmpInfoHeader->biSizeImage, ddv.m_dwImageSize);
		bError = TRUE;
	}
	else
	{
		bError = !::GetDIBits(hdc, (HBITMAP)pbmpSrc->GetSafeHandle(), 0,
			abs(pBmpInfoHeader->biHeight), pBits, pBmpInfo, DIB_RGB_COLORS);
	}
	if( bError )
	{
		::DeleteDC(hdc);
		DibUnlock(hDib);
		DibFree(hDib);
		//MYTRACE(-1,_T("CDibData::CreateDIB() = FALSE\n"));
		return FALSE;
	}

	::DeleteDC(hdc);

	DibUnlock(hDib);

	BOOL bResult = Attach(hDib);
	if( !bResult )
		DibFree(hDib);

	//MYTRACE(-1,_T("CDibData::CreateDIB() = %s\n"),MYGETBOOLSTR(bResult));
	return bResult; 
}

// BOOL CDibData::CreateDIB(HBITMAP hBitmap, BOOL bCompress = FALSE)
/**
\par Return Value:
  Nonzero on success; otherwise 0.

\par 
  To get extended error information, call GetLastError. 

\par Parameters:
  hBitmap
		[in] Handle to the source bitmap used to create DIB.

\par 
  bCompress
		[in] A flag indicating whether the resulting DIB should
		be compressed or not.

\par Remarks:
  This member is called to create a DIB: containing the
  information header, [bitfields, color table,] and image bits.

\par
  If bCompress is set to TRUE, RLE is used to compress 4 and 8
  bit DIBs. If the compressed size would result in an image size
  greater than the uncompressed size, compression is not used.

\par
  For 16 and 24 bit image the compression field is always set
  to BI_BITFIELDS.

\par
  For 1, 2, 4, and 8 bit DIBs, the bitmap identified by the hBitmap
  parameter must not be selected into a device context when the
  application calls this member. 
*/
BOOL CDibData::CreateDIB(HBITMAP hBitmap, BOOL bCompress/* = FALSE*/)
{
	ASSERT_VALID(this);
	CBitmap bmp;
	bmp.Attach(hBitmap);
	BOOL bResult = CreateDIB(&bmp, bCompress);
	bmp.Detach();
	return bResult;
}

// BOOL CDibData::SetDIBits(CBitmap* pbmpDest)
/**
\par Return Value:
  Nonzero on success; otherwise 0.

\par 
  To get extended error information, call GetLastError. 

\par Parameters:
  pbmpDest
		[in] Points to the destination bitmap where the bits
		are to be copied. Must not be NULL.

\par Remarks:
  The bitmap identified by the pbmpDest parameter must not be selected
  into a device context when the application calls this function.

\par
  The destination bitmap must be compatible with the bitmap used to
  create the DIB block contained in this object, it also must be
  the same size (height & width) as the original.

\par
  Normally the parameter passed to this member is the same as the
  parameter passed to CreateDIB(), when this object was created.
*/
BOOL CDibData::SetDIBits(CBitmap* pbmpDest)
{
	ASSERT_VALID(this);
	ASSERT(pbmpDest != NULL);
	ASSERT(pbmpDest->GetSafeHandle() != NULL);
	ASSERT(m_pDib != NULL);

	//MYTRACE(1,_T("CDibData::SetDIBits(pbmpDest = 0x%p)\n"), pbmpDest);

	BITMAP bmDest;
	if( !m_pDib || !pbmpDest || !pbmpDest->GetBitmap(&bmDest) )
	{
		if( !pbmpDest )
			::SetLastError(ERROR_INVALID_PARAMETER);
		else if( !pbmpDest->GetSafeHandle() )
			::SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}

	ASSERT(bmDest.bmHeight    == abs(GetHeight()));
	ASSERT(bmDest.bmWidth     == GetWidth()      );
	ASSERT(bmDest.bmBitsPixel == GetBitCount()   );
	ASSERT(bmDest.bmPlanes    == GetPlanes()     );

	// The device context identified by the hdc parameter is
	// used only if the DIB_PAL_COLORS constant is set for the
	// fuColorUse parameter; otherwise it is ignored.
	int nResult = ::SetDIBits(
		NULL, (HBITMAP)pbmpDest->GetSafeHandle(),
		0,  bmDest.bmHeight, GetImagePtr(),
		(LPBITMAPINFO)m_pDib, DIB_RGB_COLORS);

	//MYTRACE(-1,_T("CDibData::SetDIBits() = %s\n"),MYGETBOOLSTR(nResult != 0));
	return( nResult != 0 );
}

// BOOL CDibData::SetDIBits(HBITMAP hBitmap)
/**
\par Return Value:
  Nonzero on success; otherwise 0.

\par 
  To get extended error information, call GetLastError. 

\par Parameters:
  hBitmap
		[in] Handle to the destination bitmap where the bits
		are to be copied. Must not be NULL.

\par Remarks:
  The bitmap identified by the hBitmap parameter must not be selected
  into a device context when the application calls this function.

\par
  The destination bitmap must be compatible with the bitmap used to
  create the DIB block contained in this object, it also must be
  the same size (height & width) as the original.

\par
  Normally the parameter passed to this member is the same as the
  parameter passed to CreateDIB(), when this object was created.
*/
BOOL CDibData::SetDIBits(HBITMAP hBitmap)
{
	ASSERT_VALID(this);
	ASSERT(hBitmap != NULL);
	ASSERT(m_pDib != NULL);

	//MYTRACE(1,_T("CDibData::SetDIBits(hBitmap = 0x%p)\n"), hBitmap);

	CBitmap bmpTemp;
	bmpTemp.Attach(hBitmap);
	int nResult = SetDIBits(&bmpTemp);
	bmpTemp.Detach();

	//MYTRACE(-1,_T("CDibData::SetDIBits() = %s\n"),MYGETBOOLSTR(nResult != 0));
	return( nResult != 0 );
}

// BOOL CDibData::SaveDIB(LPCTSTR lpszPathName,
//	CBitmap* pbmpSrc = NULL, BOOL bCompress = FALSE)
/**
\par Return Value:
  Nonzero on success; otherwise 0.

\par 
  To get extended error information, call GetLastError. 

\par Parameters:
  lpszPathName
		[in] Pointer to a null-terminated string that specifies
		the name of the file to create or open. 
\par
  pbmpSrc
		[in] Points to the source bitmap to save.

\par
  bCompress
		[in] A flag indicating whether the resulting DIB should
		be compressed or not.

\par Remarks:
  If pbmpSrc is not NULL then it is saved to file, otherwise
  the internally stored DIB is saved to file.

\par
  If bCompress is TRUE than the DIB is stored in compressed
  format, provided that the compressed format is smaller than the
  uncompressed format.

\par
  Note: bCompress only applies to 4 and 8 bits-per-pixel DIBs.
*/
BOOL CDibData::SaveDIB(LPCTSTR lpszPathName,
	CBitmap* pbmpSrc/* = NULL*/, BOOL bCompress/* = FALSE*/)
{
	ASSERT_VALID(this);
	ASSERT(lpszPathName != NULL);
	ASSERT(m_pDib != NULL);

	//MYTRACE(1,_T("CDibData::SaveDIB(%s, pbmpSrc = 0x%p, bCompress = %s)\n"),
	//	lpszPathName, pbmpSrc, MYGETBOOLSTR(bCompress));

	// Validate arguments
	if( !lpszPathName )
	{
		//MYTRACE(-1,_T("CDibData::SaveDIB() = FALSE\n"));
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	// Get/Create source DIB

	CDibData* pSrcDib = this;
	CDibData TempDib;

	// Are we saving externally supplied bitmap
	if( pbmpSrc )
	{
		// Create temporary DIB
		if( !TempDib.CreateDIB(pbmpSrc, bCompress) )
		{
			//MYTRACE(-1,_T("CDibData::SaveDIB() = FALSE\n"));
			return FALSE;
		}
		pSrcDib = &TempDib;
	}
	// Else: Do we have internally stored DIB ?
	else if( !m_pDib )
	{
		//MYTRACE(-1,_T("CDibData::SaveDIB() = FALSE\n"));
		return FALSE;
	}
	// Else: Does the user want it stored as compressed DIB ?
	else if( bCompress && (m_wBitsPerPixel == 4 || m_wBitsPerPixel == 8) )
	{
		// If not already compressed
		if( GetCompression() != BI_RLE4 && GetCompression() != BI_RLE8 )
		{
			HBITMAP hBitmap = CreateDIBitmap();
			if( hBitmap )
			{
				if( TempDib.CreateDIB(hBitmap, bCompress) )
					pSrcDib = &TempDib;
				::DeleteObject(HGDIOBJ(hBitmap));
			}
		}
	}

	// Save source DIB to file

	DWORD dwDibSize = pSrcDib->GetDibSize();
	
	// Initialize bitmap file header
    BITMAPFILEHEADER bmpFileHeader;
	::ZeroMemory(&bmpFileHeader, sizeof(BITMAPFILEHEADER));
    bmpFileHeader.bfType    = 0x4D42;
	bmpFileHeader.bfSize    = sizeof(BITMAPFILEHEADER) + dwDibSize;
    bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + pSrcDib->GetImageOffset();
	
	// Create bitmap file
	BOOL bResult = FALSE;
	try
	{
		CFile cf(lpszPathName, CFile::modeCreate | CFile::modeWrite);
		cf.Write((LPVOID)&bmpFileHeader, sizeof(BITMAPFILEHEADER));
		cf.Write((LPVOID)pSrcDib->GetDibPtr(), dwDibSize);
		cf.Close();
		bResult = TRUE;
	}
	catch( CFileException* /*e*/)
	{
		//MYTRACE(0,_T("File Exception\n"));
	}

	//MYTRACE(-1,_T("CDibData::SaveDIB() = %s\n"),MYGETBOOLSTR(bResult));
	return bResult;
}

// HBITMAP CDibData::CreateDIBitmap()
/**
\par Return Value:
  Handle to DIB section; otherwise NULL.

\par 
  To get extended error information, call GetLastError. 

\par Remarks:
  This member is used to create a DIB section from internally stored DIB.

\par
  If the DIB file contains 2 bits-per-pixel image then
  this member converts it to 4 bits-per-pixel, for displaying
  on non-WinCE displays. The original bits-per-pixel is not
  saved.
*/
HBITMAP CDibData::CreateDIBitmap()
{
	ASSERT_VALID(this);
	ASSERT(m_pDib != NULL);
	//MYTRACE(1,_T("CDibData::CreateDIBitmap()\n"));

	// 
	if( !m_pDib )
	{
		//MYTRACE(-1,_T("CDibData::CreateDIBitmap() = FALSE\n"));
		return FALSE;
	}

	HBITMAP hBitmap = NULL;
	LPVOID pBits    = NULL;

	// If 2 bits-per-pixel format we need to convert it
	// to 4 bits-per-pixel for viewing / editing
	if( m_wBitsPerPixel == 2 )
	{
		// Create converted DIB object
		// Convert to 4 bits-per-pixel
		//MYTRACE(0,_T("Converting to 4 bits-per-pixel\n"));
		CDibData* pDestDib = GetConvertedDIB(4);
		if( pDestDib )
		{
			// Create DIB section from converted DIB
			// The device context identified by the hdc parameter is
			// used only if the DIB_PAL_COLORS constant is set for the
			// fuColorUse parameter; otherwise it is ignored.
			//MYTRACE(0,_T("Create DIB section\n"));
			hBitmap = ::CreateDIBSection(NULL, LPBITMAPINFO(pDestDib->GetDibPtr()),
				DIB_RGB_COLORS, &pBits, NULL, 0);
			if( hBitmap )
			{
				DeleteObject();
				Attach(pDestDib->Detach());
			}

			delete pDestDib;
		}
	}
	else
	{
		// Create DIB section
		// The device context identified by the hdc parameter is
		// used only if the DIB_PAL_COLORS constant is set for the
		// fuColorUse parameter; otherwise it is ignored.
		hBitmap = ::CreateDIBSection(NULL, LPBITMAPINFO(m_pDib),
			DIB_RGB_COLORS, &pBits, NULL, 0);
		//MYTRACE(0,_T("0x%p = CreateDIBitmap();\n"), hBitmap);
	}

	// If every thing is OK, then copy bits to DIB section
	if( hBitmap )
		SetDIBits(hBitmap);

	//MYTRACE(-1,_T("CDibData::CreateDIBitmap() = 0x%p\n"),hBitmap);
	return hBitmap;
}

// HBITMAP CDibData::LoadDIB(LPCTSTR lpszPathName)
/**
\par Return Value:
  Handle to DIB section; otherwise NULL.

\par 
  To get extended error information, call GetLastError.

\par Parameters:
  lpszPathName
		[in] Pointer to a null-terminated string that specifies
		the name of the file to load.

\par Remarks:
  This member is used to create a DIB section from the given
  file name.

\par
  If the DIB file contains 2 bits-per-pixel image then
  this member converts it to 4 bits-per-pixel, for displaying
  on non-WinCE displays. The original bits-per-pixel is not
  saved.

\par
  As an alternative you can use LoadImage() to load a bitmap
  image file.
\par

  Note: LoadImage() can not load top-down bitmaps or 2 bits-per-pixel
  bitmaps [,this may change in the future].
*/
HBITMAP CDibData::LoadDIB(LPCTSTR lpszPathName)
{
	ASSERT_VALID(this);
	ASSERT(lpszPathName != NULL);
	ASSERT(m_pDib == NULL);
	//MYTRACE(1,_T("CDibData::LoadDIB(%s)\n"),lpszPathName);
	/****/DEBUG_TRACE_DIBFILE(lpszPathName, FALSE);

	// Validate arguments
	if( !lpszPathName || m_pDib )
	{
		//MYTRACE(-1,_T("CDibData::LoadDIB() = FALSE\n"));
		if( !lpszPathName )
			::SetLastError(ERROR_INVALID_PARAMETER);
		else
			::SetLastError(ERROR_ALREADY_INITIALIZED);
		return FALSE;
	}

	HANDLE hDib = NULL;
	LPBYTE pDib = NULL;

	try
	{
		// Copy DIB from file
		//MYTRACE(0,_T("Reading file\n"));
		CFile cf(lpszPathName, CFile::modeRead);

		// Read bitmap file header
		BITMAPFILEHEADER bmfh;
		cf.Read((void*)&bmfh, sizeof(BITMAPFILEHEADER));

		// Verify file type
		//MYTRACE(0,_T("Checking if valid DIB file\n"));
		if( bmfh.bfType != 0x4D42 )
		{
			cf.Close();
			//MYTRACE(-1,_T("CDibData::LoadDIB() = NULL\n"));
			return NULL;
		}

		// Read DIB from file
		DWORD dwLen = cf.GetLength() - sizeof(BITMAPFILEHEADER);
		hDib = DibAlloc(dwLen);
		pDib = DibLock(hDib);
		if( pDib )
			cf.Read((void*)pDib, (UINT)dwLen);
		cf.Close();

		if( !pDib )
		{
			if( hDib )
				DibFree(hDib);
			//MYTRACE(-1,_T("CDibData::LoadDIB() = NULL\n"));
			return NULL;
		}

		DibUnlock(hDib);
	}
	catch( CFileException* /*e*/)
	{
		//MYTRACE(0,_T("File Exception\n"));
		if( pDib )
			DibUnlock(hDib);
		if( hDib )
			DibFree(hDib);
		//MYTRACE(-1,_T("CDibData::LoadDIB() = NULL\n"));
		return NULL;
	}

	// Attach DIB to this object
	if( !Attach(hDib) )
	{
		DibFree(hDib);
		//MYTRACE(-1,_T("CDibData::LoadDIB() = NULL\n"));
		return NULL;
	}

	// Note: LoadImage() decompresses compressed DIBs automatically.
	HBITMAP hBitmap = (HBITMAP)::LoadImage(
		NULL, lpszPathName, IMAGE_BITMAP,
		0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	//MYTRACE(0,_T("0x%p = LoadImage(...)\n"));\

	// Create DIB section from internally stored DIB
	if( !hBitmap )
	{
		hBitmap = CreateDIBitmap();
	}

	// Is every thing is OK ?
	if( !hBitmap )
	{
		DeleteObject();
	}
	// Else is DIB compressed ?
	else if( GetCompression() == BI_RLE4 || GetCompression() == BI_RLE8 )
	{
		DIBSECTION ds;
		::ZeroMemory(&ds, sizeof(ds));
		if( ::GetObject(hBitmap, sizeof(DIBSECTION), &ds) )
		{
			CDibDataVars ddv(&ds.dsBmih);
			Detach();
			HANDLE hTemp = ::GlobalReAlloc(hDib, ddv.m_dwDibSize, GHND);
			if( hTemp )
			{
				hDib = hTemp;
				Attach(hDib);
				::CopyMemory(m_pDib + ddv.m_dwImageOffset,
					ds.dsBm.bmBits, ddv.m_dwImageSize);
			}
			else
			{
				DibFree(hDib);
				::DeleteObject(HGDIOBJ(hBitmap));
				hDib    = NULL;
				hBitmap = NULL;
			}
		}
	}

	//MYTRACE(-1,_T("CDibData::LoadDIB() = 0x%p\n"),hBitmap);
	return hBitmap;
}

// DWORD CDibData::GetPixelValue(int x, int y) const
/**
\par Return Value:
  The value stored at the given coordinates; otherwise CLR_INVALID.
\par 
  To get extended error information, call GetLastError.

\par Parameters:
  x 
		[in] Specifies the logical x-coordinate of the pixel to be examined.

\par
  y 
		[in] Specifies the logical y-coordinate of the pixel to be examined.

\par Remarks:
  The DIB must be valid and uncompressed.
*/
DWORD CDibData::GetPixelValue(int x, int y) const
{
	ASSERT_VALID(this);
	ASSERT(m_pDib != NULL);

	if( !m_pLine )
		return CLR_INVALID;

	if( x < 0 || GetWidth()  <= x ||
		y < 0 || abs(GetHeight()) <= y )
	{
		return CLR_INVALID;
	}

	LPBYTE pPixel = m_pLine[y] + ((x * m_wBitsPerPixel) >> 3);

	switch( m_wBitsPerPixel )
	{
		case  1: 
			return 0x01 & (*pPixel >> (7 - (x & 7)));
		case  2: // WinCE ?
			return 0x03 & (*pPixel >> ((3 - (x & 3)) << 1));
		case  4:
			return 0x0F & (*pPixel >> (x & 1 ? 0 : 4));
		case  8: 
			return *pPixel;
		case 16: 
			return *(LPWORD)pPixel;
		case 24: 
			return 0x00FFFFFF & *(LPDWORD)pPixel;
		case 32: 
			return *(LPDWORD)pPixel;
	}

	return CLR_INVALID;
}

// BOOL CDibData::SetPixelValue(int x, int y, DWORD dwPixel)
/**
\par Return Value:
  Nonzero on success; otherwise 0.

\par 
  To get extended error information, call GetLastError.

\par Parameters:
  x 
		[in] Specifies the x-coordinate, 
		in logical units, of the point to be set.

\par
  y 
		[in] Specifies the y-coordinate, 
		in logical units, of the point to be set.

\par
  dwPixel
		[in] Specifies the value to set.

\par Remarks:
  The DIB must be valid and uncompressed.
*/
BOOL CDibData::SetPixelValue(int x, int y, DWORD dwPixel)
{
	ASSERT_VALID(this);
	ASSERT(m_pDib != NULL);

	if( !m_pLine )
		return FALSE;

	if( x < 0 || GetWidth()  <= x ||
		y < 0 || abs(GetHeight()) <= y )
	{
		return FALSE;
	}

	LPBYTE pPixel = m_pLine[y] + ((x * m_wBitsPerPixel) >> 3);

	switch( m_wBitsPerPixel )
	{
		case  1:
			{
				int iLShift = (7 - (x & 7));
				*pPixel &= ~(1     << iLShift );
				*pPixel |= dwPixel << iLShift;
			}
			break;

		case  2: // WinCE
			{
				int iLShift = ((3 - (x & 3)) << 1);
				*pPixel &= ~(3     << iLShift );
				*pPixel |= dwPixel << iLShift;
			}
			break;

		case  4:
			*pPixel &= 0x0F    << (x & 1 ? 4 : 0);
			*pPixel |= dwPixel << (x & 1 ? 0 : 4);
			break;

		case  8:
			*pPixel = (BYTE)dwPixel;
			break;

		case 16:
			*(LPWORD)pPixel = (WORD)dwPixel;
			break;

		case 24:
			*(RGBTRIPLE*)pPixel = *(RGBTRIPLE*)&dwPixel;
			break;

		case 32:
			*(LPDWORD)pPixel = dwPixel;
			break;

		default:
			return FALSE;
	}
	return TRUE;
}

// COLORREF CDibData::GetPixel(int x, int y) const
/**
\par Return Value:
  The RGB color value stored at the given coordinates; otherwise CLR_INVALID.

\par 
  To get extended error information, call GetLastError.

\par Parameters:
  x 
		[in] Specifies the logical x-coordinate of the pixel to be examined.

\par
  y 
		[in] Specifies the logical y-coordinate of the pixel to be examined.

\par Remarks:
  The DIB must be valid and uncompressed.
*/
COLORREF CDibData::GetPixel(int x, int y) const
{
	ASSERT_VALID(this);
	ASSERT(m_pDib != NULL);

	DWORD dwPixel = GetPixelValue(x, y);
	if( dwPixel == CLR_INVALID )
		return CLR_INVALID;

	BYTE Red, Green, Blue;
	if( m_wBitsPerPixel <= 8 )
	{
		RGBQUAD rgbq;
		if( !GetPaletteEntries((UINT)dwPixel, 1, &rgbq) )
			return CLR_INVALID;
		Red = rgbq.rgbRed, Green = rgbq.rgbGreen, Blue = rgbq.rgbBlue;
	}
	else if( m_wBitsPerPixel == 24 )
	{
		RGBTRIPLE rgbt = *(RGBTRIPLE*)&dwPixel;
		Red = rgbt.rgbtRed, Green = rgbt.rgbtGreen, Blue = rgbt.rgbtBlue;
	}
	else if( m_wBitsPerPixel == 32 &&
		GetCompression() == BI_RGB )
	{
		RGBQUAD rgbq = *(LPRGBQUAD)&dwPixel;
		Red = rgbq.rgbRed, Green = rgbq.rgbGreen, Blue = rgbq.rgbBlue;
	}
	else
	{
		Red   = (BYTE)(((m_dwBitfields[0] & dwPixel) 
			>> m_wRShift[0]) << m_wLShift[0]);

		Green = (BYTE)(((m_dwBitfields[1] & dwPixel)
			>> m_wRShift[1]) << m_wLShift[1]);

		Blue  = (BYTE)(((m_dwBitfields[2] & dwPixel)
			>> m_wRShift[2]) << m_wLShift[2]);
	}

	return RGB(Red, Green, Blue);
}

// BOOL CDibData::SetPixel(int x, int y, COLORREF clPixel)
/**
\par Return Value:
  Nonzero on success; otherwise 0.

\par 
  To get extended error information, call GetLastError.

\par Parameters:
  x 
		[in] Specifies the x-coordinate, 
		in logical units, of the point to be set.

\par
  y 
		[in] Specifies the y-coordinate, 
		in logical units, of the point to be set.

\par
  dwPixel
		[in] Specifies the RGB color value to set.

\par Remarks:
  The DIB must be valid and uncompressed.
*/
BOOL CDibData::SetPixel(int x, int y, COLORREF clPixel)
{
	ASSERT_VALID(this);
	ASSERT(m_pDib != NULL);

	if( clPixel == CLR_INVALID)
		return FALSE;

	RGBQUAD rgbq;
	rgbq.rgbRed   = GetRValue(clPixel);
	rgbq.rgbGreen = GetGValue(clPixel);
	rgbq.rgbBlue  = GetBValue(clPixel);
	rgbq.rgbReserved = 0;

	DWORD dwPixel;
	if( m_wBitsPerPixel <= 8 )
	{
		dwPixel = GetNearestPaletteIndex(&rgbq);
		if( dwPixel == CLR_INVALID )
			return FALSE;
	}
	else if( m_wBitsPerPixel == 24 )
	{
		*(RGBTRIPLE*)&dwPixel = *(RGBTRIPLE*)&rgbq;
		dwPixel &= 0x00FFFFFF;
	}
	else if( m_wBitsPerPixel == 32 &&
		GetCompression() == BI_RGB )
	{
		*(LPRGBQUAD)&dwPixel = rgbq;
	}
	else
	{
		dwPixel  = (((DWORD)rgbq.rgbRed   >> m_wLShift[0]) << m_wRShift[0]);
		dwPixel |= (((DWORD)rgbq.rgbGreen >> m_wLShift[1]) << m_wRShift[1]);
		dwPixel |= (((DWORD)rgbq.rgbBlue  >> m_wLShift[2]) << m_wRShift[2]);
	}

	return SetPixelValue(x, y, dwPixel);
}

// CDibData* CDibData::CreateEmptyDIB(WORD wBitsPerPixel)
/**
\par Return Value:
  Pointer to new CDibData; otherwise NULL.

\par 
  To get extended error information, call GetLastError.

\par Parameters:
  wBitsPerPixel
		[in] The number of bits-per-pixel used to create the
		new DIB.

\par Remarks:
  This member creates an empty DIB of the same size as the
  original, but with the given color depth.

\par
  The new DIB preservers most of the original header information,
  accept where color depth and compression type is concerned.

\par
  The returned object is sufficiently large enough to hold an
  uncompressed DIB.
*/
CDibData* CDibData::CreateEmptyDIB(WORD wBitsPerPixel)
{
	ASSERT_VALID(this);
	//MYTRACE(0,_T("CDibData::CreateEmptyDIB(BPP = %d)\n"), wBitsPerPixel);

	// Insure bits-per-color is correct
	if( wBitsPerPixel <= 1 )
		wBitsPerPixel = 1;
	else if( wBitsPerPixel <= 2 )
		wBitsPerPixel = 2;
	else if( wBitsPerPixel <= 4 )
		wBitsPerPixel = 4;
	else if( wBitsPerPixel <= 8 )
		wBitsPerPixel = 8;
	else if( wBitsPerPixel <= 16 )
		wBitsPerPixel = 16;
	else if( wBitsPerPixel <= 24 )
		wBitsPerPixel = 24;
	else
		wBitsPerPixel = 32;

	// Prepare for creation
	BITMAPINFOHEADER bmih;
	::CopyMemory(&bmih, m_pDib, sizeof(BITMAPINFOHEADER));

	// Note: CDibDataVars uses biSize in calculations & only the fields
	// that can be found in a BITMAPINFOHEADER are used.
	bmih.biSize        = GetHeaderSize();
	bmih.biPlanes      = 1;
	bmih.biBitCount    = wBitsPerPixel;
	bmih.biClrUsed     = 0L;
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage   = 0L;

	if( wBitsPerPixel == 16 || wBitsPerPixel == 32 )
		bmih.biCompression = BI_BITFIELDS;

	CDibDataVars ddv(&bmih);
	bmih.biSizeImage = ddv.m_dwImageSize;
	bmih.biClrUsed   = ddv.m_dwNumColors;

	// Create new CDibData
	CDibData* pDestDib = NULL;
	HANDLE hDib = DibAlloc(ddv.m_dwMaxDibSize);
	LPBYTE pDib = DibLock(hDib);
	if( !pDib || !hDib )
	{
		if( hDib )
			DibFree(hDib);
	}
	else
	{
		// Copy original header to preserve unknown fields if any
		if( GetHeaderSize() > sizeof(BITMAPINFOHEADER) )
			::CopyMemory(pDib, m_pDib, GetHeaderSize());

		// Overwrite with known base header information
		::CopyMemory(pDib, &bmih, sizeof(BITMAPINFOHEADER));

		// Create new CDibData object & attach DIB to it
		DibUnlock(hDib);
		pDestDib = new CDibData();
		if( !pDestDib )
		{
			DibFree(hDib);
			::SetLastError(ERROR_OUTOFMEMORY);
		}
		else if( !pDestDib->Attach(hDib) )
		{
			DibFree(hDib);
			delete pDestDib;
			pDestDib = NULL;
		}
	}

	//MYTRACE(-1,_T("CDibData::CreateEmptyDIB() = 0x%p\n"), pDestDib);
	return pDestDib;
}

// void CDibData::FixImageData()
/**
\par Return Value:
  void

\par Parameters:
  void

\par Remarks:
  Used to recalculate image data pointers.

\par
  This member function is used by GetConvertedDIB() to recalculate
  image data pointers if the number of colors actually used is
  less than the maximum number of colors.
*/
void CDibData::FixImageData()
{
	ASSERT_VALID(this);
	ASSERT(m_pDib != NULL);

	LPBITMAPINFOHEADER pInfo = (LPBITMAPINFOHEADER)m_pDib;
	CDibDataVars ddv(pInfo);

	m_pBits = m_pDib + ddv.m_dwImageOffset;

	if( pInfo->biHeight > 0 )
	{
		LONG y = pInfo->biHeight - 1;
		for( LONG i = 0; y >= 0; --y, ++i )
		{
			m_pLine[i] = m_pBits + y * ddv.m_dwByteWidth;
		}
	}
	else
	{
		LONG lHeight = abs(pInfo->biHeight);
		for( LONG i = 0; i < lHeight; ++i )
		{
			m_pLine[i] = m_pBits + i * ddv.m_dwByteWidth;
		}
	}
}

// CDibData* CDibData::
// GetConvertedDIB(WORD wBitsPerPixel, BOOL bOptimize = FALSE)
/**
\par Return Value:
  Pointer to new CDibData; otherwise NULL.

\par 
  To get extended error information, call GetLastError.

\par Parameters:
  wBitsPerPixel
		[in] The number of bits-per-pixel used to create the
		new DIB.

\par
  bOptimize
		[in] Used to specify use of optimized color table instead
		of default color table.

\par Remarks:
  The DIB being converted must not be compressed.

\par
  The bOptimize parameter is used to optimize the 16-color table
  for conversion from 16, 24, or 32 bits to 4 bit images.

\par
  For conversion from 16, 24, or 32 bits to 8 bit images, an
  optimized color table is used automatically.

\par
  If the conversion would only generate a copy of the original
  then no conversion occurs and the return value is NULL.
*/
CDibData* CDibData::
GetConvertedDIB(WORD wBitsPerPixel, BOOL bOptimize/* = FALSE*/)
{
	ASSERT_VALID(this);
	ASSERT(m_pDib != NULL);
	//MYTRACE(1,_T("CDibData::GetConvertedDIB(%u, %s)\n"),
	//	wBitsPerPixel, MYGETBOOLSTR(bOptimize));

	// Check for DIB
	if( !m_pDib )
	{
		//MYTRACE(-1,_T("CDibData::GetConvertedDIB() = NULL\n"));
		return NULL;
	}

	// Check if compressed
	if( GetCompression() == BI_RLE4 || GetCompression() == BI_RLE8 )
	{
		//MYTRACE(-1,_T("CDibData::GetConvertedDIB() = NULL\n"));
		return NULL;
	}

	// Create Empty DIB
	CDibData* pDestDib = CreateEmptyDIB(wBitsPerPixel);
	if( !pDestDib )
	{
		//MYTRACE(-1,_T("CDibData::GetConvertedDIB() = NULL\n"));
		return NULL;
	}

	// Declare/Initialize local variables
	WORD wSrcBitsPerPixel = m_wBitsPerPixel;
	WORD wDstBitsPerPixel = pDestDib->m_wBitsPerPixel;

	LONG cx = pDestDib->GetWidth();
	LONG cy = abs(pDestDib->GetHeight());

	// If no conversion required
	if( wSrcBitsPerPixel == wDstBitsPerPixel && BI_RGB == GetCompression() )
	{
		delete pDestDib;
		//MYTRACE(-1,_T("CDibData::GetConvertedDIB() = NULL\n"));
		return NULL;
	}
	
	// Convert
	if( wSrcBitsPerPixel < wDstBitsPerPixel && wDstBitsPerPixel <= 8 )
	{
		pDestDib->SetPaletteEntries(0, m_dwColorTableSize, m_pColorTable);

		if( GetClrUsed() || GetClrImportant() )
		{
			pDestDib->GetHeaderPtr()->biClrUsed      = GetClrUsed();
			pDestDib->GetHeaderPtr()->biClrImportant = GetClrImportant();
		}
		else
			pDestDib->GetHeaderPtr()->biClrUsed = m_dwColorTableSize;

		pDestDib->FixImageData();

		for( LONG y = 0; y < cy; ++y )
			for( LONG x = 0; x < cx; ++x )
				pDestDib->SetPixelValue(x, y, GetPixelValue(x, y));
	}
	else if( wDstBitsPerPixel >= 16 )
	{
		for( LONG y = 0; y < cy; ++y )
			for( LONG x = 0; x < cx; ++x )
				pDestDib->SetPixel(x, y, GetPixel(x, y));
	}
	else
	{
		if( wSrcBitsPerPixel >= 16 && wDstBitsPerPixel == 8 )
		{
			RGBQUAD rgbqOp[256];
			UINT uColors = GetOptimalColors(rgbqOp, 256, wDstBitsPerPixel);

			if( uColors < 256 )
			{
				//
				// This slows down conversion but generates
				// a proper gray scale color table.
				//
				// There are other types of color tables that can be
				// determined by examining the optimal color table.
				// Examples:
				//   1) [blue scale] + [green scale] + [red scale]
				//   2) [VGA]
				//

				// Check if gray scale
				UINT i = 0;
				for( i = 0; i < uColors; ++i )
				{
					if( rgbqOp[i].rgbRed != rgbqOp[i].rgbGreen ||
						rgbqOp[i].rgbRed != rgbqOp[i].rgbBlue  )
					{
						break;
					}

				}

				// If gray scale
				if( i == uColors )
				{
					uColors = 256;
					for( UINT i = 0; i < uColors; ++i )
					{
						rgbqOp[i].rgbRed   =
						rgbqOp[i].rgbGreen =
						rgbqOp[i].rgbBlue  = (BYTE)i;
					}
				}
			}

			pDestDib->SetPaletteEntries(0, uColors, rgbqOp);
			if( uColors < 256 )
			{
				pDestDib->GetHeaderPtr()->biClrUsed = uColors;
				pDestDib->FixImageData();
			}
		}
		else if( wDstBitsPerPixel == 1 )
		{
			RGBQUAD rgbqMono[2] = {
				{0x00, 0x00, 0x00, 0x00},
				{0xFF, 0xFF, 0xFF, 0x00}};
			pDestDib->SetPaletteEntries(0, 2, rgbqMono);
		}
		else if( wDstBitsPerPixel == 2 )
		{
			RGBQUAD rgbqCE[4] = {
				{0x00, 0x00, 0x00, 0x00},
				{0x80, 0x80, 0x80, 0x00},
				{0xC0, 0xC0, 0xC0, 0x00},
				{0xFF, 0xFF, 0xFF, 0x00}};
			pDestDib->SetPaletteEntries(0, 4, rgbqCE);
		}
		else if( wSrcBitsPerPixel >= 8 && wDstBitsPerPixel == 4 )
		{
			RGBQUAD rgbVGA[16] = { 
				{0x00, 0x00, 0x00, 0x00},
				{0x00, 0x00, 0x80, 0x00},
				{0x00, 0x80, 0x00, 0x00},
				{0x00, 0x80, 0x80, 0x00},
				{0x80, 0x00, 0x00, 0x00},
				{0x80, 0x00, 0x80, 0x00},
				{0x80, 0x80, 0x00, 0x00},
				{0x80, 0x80, 0x80, 0x00},
				{0xC0, 0xC0, 0xC0, 0x00},
				{0x00, 0x00, 0xFF, 0x00},
				{0x00, 0xFF, 0x00, 0x00},
				{0x00, 0xFF, 0xFF, 0x00},
				{0xFF, 0x00, 0x00, 0x00},
				{0xFF, 0x00, 0xFF, 0x00},
				{0xFF, 0xFF, 0x00, 0x00},
				{0xFF, 0xFF, 0xFF, 0x00}};

			if( bOptimize )
			{
				UINT uColors = GetOptimalColors(rgbVGA, 16, wDstBitsPerPixel);
				pDestDib->SetPaletteEntries(0, uColors, rgbVGA);
				if( uColors < 16 )
				{
					pDestDib->GetHeaderPtr()->biClrUsed = uColors;
					pDestDib->FixImageData();
				}
			}

			pDestDib->SetPaletteEntries(0, 16, rgbVGA);
		}
		else
		{
			delete pDestDib;
			//MYTRACE( 0,_T("Invalid conversion type requested!\n"));
			//MYTRACE( 0,_T("%u Bit to %u Bit image\n"),
			//	wSrcBitsPerPixel, wDstBitsPerPixel);
			//MYTRACE(-1,_T("CDibData::GetConvertedDIB() = NULL\n"));
			return NULL;
		}

		for( LONG y = 0; y < cy; ++y )
			for( LONG x = 0; x < cx; ++x )
			{
				pDestDib->SetPixelValue(x, y,
					pDestDib->GetNearestPaletteIndex(GetPixel(x, y)));
			}
	}

	//MYTRACE(-1,_T("CDibData::GetConvertedDIB() = 0x%p\n"), pDestDib);
	return pDestDib;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// CQuantizer Class : MSJ : Wicked Code 1997 : By Jeff Prosise
#include "Quantize.h"

/////////////////////////////////////////////////////////////////////////////
// CDibQuantizer Class derived From CQuantizer.
class CDibQuantizer : public CQuantizer
{
public:
    CDibQuantizer(UINT nMaxColors, UINT nColorBits);
    //virtual ~CDibQuantizer();
	BOOL ProcessImage(CDibData* pDibDataClass);
};

CDibQuantizer::CDibQuantizer(UINT nMaxColors, UINT nColorBits)
: CQuantizer(nMaxColors, nColorBits)
{}

BOOL CDibQuantizer::ProcessImage(CDibData* pDibDataClass)
{
	ASSERT(this);
	ASSERT(pDibDataClass != NULL);

	if( !pDibDataClass || !pDibDataClass->m_hDib )
	{
		if( !pDibDataClass )
			::SetLastError(ERROR_INVALID_PARAMETER);
		else
			::SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}

	LONG cx = pDibDataClass->GetWidth();
	LONG cy = abs(pDibDataClass->GetHeight());
	COLORREF clrSrc;

	for( LONG y = 0; y < cy; ++y )
	{
		for( LONG x = 0; x < cx; ++x )
		{
			clrSrc = pDibDataClass->GetPixel(x, y);

            AddColor(&m_pTree,
				GetRValue(clrSrc), GetGValue(clrSrc), GetBValue(clrSrc),
				m_nColorBits, 0, &m_nLeafCount, m_pReducibleNodes);

            while( m_nLeafCount > m_nMaxColors )
                ReduceTree (m_nColorBits, &m_nLeafCount, m_pReducibleNodes);
		}
	}

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// UINT CDibData::
// GetOptimalColors(LPRGBQUAD pColors, UINT cColors, UINT nColorBits)
/**
\par Return Value:
  Number of colors copied to pColors; otherwise 0.

\par 
  To get extended error information, call GetLastError.

\par Parameters:
  pColors
		[out] Pointer to a buffer that receives an array of RGBQUAD data
		structures containing color information derived from this objects
		DIB.

\par
  cColors
		[in] Specifies the maximum number of color table entries to retrieve.

\par
  nColorBits
		[in] The number of bits-per-color used to determine the optimal
		color table.

\par Remarks:
  This member should be called to retrieve the optimal color table for DIB
  section bitmaps that use 4, or 8 bpp.
*/
UINT CDibData::
GetOptimalColors(LPRGBQUAD pColors, UINT cColors, UINT nColorBits)
{
	ASSERT_VALID(this);
	ASSERT(m_pDib != NULL);
	ASSERT(pColors != NULL);
	ASSERT(cColors != 0);
	ASSERT(nColorBits != 0);
	//MYTRACE(1,_T("CDibData::GetOptimalColors(0x%p, %u, %u)\n"),
	//	pColors, cColors, nColorBits);

	UINT uColorCount = 0U;
	if( !m_hDib )
		::SetLastError(ERROR_INVALID_HANDLE);
	else if( !pColors || !cColors || !nColorBits )
		::SetLastError(ERROR_INVALID_PARAMETER);
	else
	{
		CDibQuantizer Q(cColors, nColorBits);
		Q.ProcessImage(this);
		uColorCount = Q.GetColorCount();
		if( uColorCount <= cColors )
			Q.GetColorTable(pColors);
		else
			uColorCount = 0;
	}

	//MYTRACE(-1,_T("CDibData::GetOptimalColors() = %u\n"), uColorCount);
	return uColorCount;
}

// BOOL CDibData::
// GetPaletteEntries(UINT uStartIndex, UINT cEntries, LPRGBQUAD pColors) const
/**
\par Return Value:
  Nonzero if successful; otherwise 0.

\par 
  To get extended error information, call GetLastError.

\par Parameters:
  uStartIndex
		[in] A zero-based color table index that specifies the first
		color table entry to retrieve.

\par
  cEntries
		[in] Specifies the number of color table entries to retrieve.

\par
  pColors
		[out] Pointer to a buffer that receives an array of RGBQUAD data
		structures containing color information from the DIB's color
		table. The buffer must be large enough to contain as many
		RGBQUAD data structures as the value of cEntries.

\par Remarks:
  This member should be called to retrieve the color table for DIB
  section bitmaps that use 1, 2, 4, or 8 bpp.
*/
BOOL CDibData::
GetPaletteEntries(UINT uStartIndex, UINT cEntries, LPRGBQUAD pColors) const
{
	ASSERT_VALID(this);
	ASSERT(m_pDib != NULL);
	ASSERT(pColors != NULL);
	ASSERT(cEntries != 0);

	if( !m_hDib )
		::SetLastError(ERROR_INVALID_HANDLE);
	else if( !cEntries || !pColors )
		::SetLastError(ERROR_INVALID_PARAMETER);
	else if( m_pColorTable && uStartIndex + cEntries <= m_dwColorTableSize )
	{
		::CopyMemory(pColors, m_pColorTable + uStartIndex,
			cEntries * sizeof(RGBQUAD));
		return TRUE;
	}

	return FALSE;
}

// BOOL CDibData::
// SetPaletteEntries(UINT uStartIndex, UINT cEntries, LPRGBQUAD pColors)
/**
\par Return Value:
  Nonzero if successful; otherwise 0.

\par 
  To get extended error information, call GetLastError.

\par Parameters:
  uStartIndex
		[in] A zero-based color table index that specifies the first
		color table entry to retrieve.

\par
  cEntries
		[in] Specifies the number of color table entries to retrieve.

\par
  pColors
		[in] Pointer to an array of RGBQUAD structures containing new
		color information for the DIB's color table. 

\par Remarks:
  This member should be called to set the color table for DIBs that
  use 1, 2, 4, or 8 bpp.
*/
BOOL CDibData::
SetPaletteEntries(UINT uStartIndex, UINT cEntries, LPRGBQUAD pColors)
{
	ASSERT_VALID(this);
	ASSERT(m_pDib != NULL);
	ASSERT(pColors != NULL);
	ASSERT(cEntries != 0);

	if( !m_hDib )
		::SetLastError(ERROR_INVALID_HANDLE);
	else if( !cEntries || !pColors )
		::SetLastError(ERROR_INVALID_PARAMETER);
	else if( m_pColorTable && uStartIndex + cEntries <= m_dwColorTableSize )
	{
		::CopyMemory(m_pColorTable + uStartIndex, pColors,
			cEntries * sizeof(RGBQUAD));
			return TRUE;
	}

	return FALSE;
}

// UINT CDibData::GetNearestPaletteIndex(LPRGBQUAD pColors) const
/**
\par Return Value:
  Color table index; otherwise CLR_INVALID.

\par Parameters:
  pColors
		[in] Specifies a color to be matched. 

\par Remarks:
  This member should be called to retrieve the nearest color table index,
  to the given color value. (1, 2, 4, or 8 bpp)
*/
UINT CDibData::GetNearestPaletteIndex(const LPRGBQUAD pColor) const
{
	ASSERT_VALID(this);
	ASSERT(m_pDib != NULL);
	ASSERT(pColor != NULL);

	UINT uNearestIndex = CLR_INVALID;
	if( m_pColorTable )
	{
		DWORD dwNumColors = GetClrUsed();
		if( !dwNumColors )
			dwNumColors = m_dwColorTableSize;

		DWORD dwDiff, dwMinDiff = 0xFFFFFFFF; // any # > 3 * 255
		for( UINT i = 0; i < dwNumColors && dwMinDiff; ++i )
		{
			dwDiff =
				abs(m_pColorTable[i].rgbRed   - pColor->rgbRed  ) +
				abs(m_pColorTable[i].rgbGreen - pColor->rgbGreen) +
				abs(m_pColorTable[i].rgbBlue  - pColor->rgbBlue );
			if( dwDiff < dwMinDiff )
			{
				dwMinDiff = dwDiff;
				uNearestIndex = i;
			}
		}
	}
	return uNearestIndex;
}

// UINT CDibData::GetNearestPaletteIndex(COLORREF clrSrc) const
/**
\par Return Value:
  Color table index; otherwise CLR_INVALID.

\par Parameters:
  clrSrc
		[in] Specifies a color to be matched. 

\par Remarks:
  This member should be called to retrieve the nearest color table index,
  to the given color value. (1, 2, 4, or 8 bpp)
*/
UINT CDibData::GetNearestPaletteIndex(COLORREF clrSrc) const
{
	RGBQUAD rgbq;
	rgbq.rgbRed   = GetRValue(clrSrc);
	rgbq.rgbGreen = GetGValue(clrSrc);
	rgbq.rgbBlue  = GetBValue(clrSrc);
	rgbq.rgbReserved = 0;
	return GetNearestPaletteIndex(&rgbq);
}

/////////////////////////////////////////////////////////////////////////////
//
// DIB Debugging helpers
//
/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG

#include <afxtempl.h>		// Template support

/////////////////////////////////////////////////////////////////////////////
// Dib dump helpers

typedef struct tag_DBTrack
{
	HANDLE hDib;
	int iLockCount;
} DB_TRACK_;

CArray<DB_TRACK_, DB_TRACK_> g_DebugHandleTrack;

int DebugFindHandle(HANDLE hDib)
{
	DB_TRACK_ db_Track;
	int iSize = g_DebugHandleTrack.GetSize(), i;
	for( i = 0; i < iSize; ++i )
	{
		db_Track = g_DebugHandleTrack.GetAt(i);
		if( hDib == db_Track.hDib )
			break;
	}
	if( i >= iSize )
		i = -1;
	return i;
}

int DebugAddHandle(HANDLE hDib)
{
	int iIndex = DebugFindHandle(hDib);
	if( iIndex < 0 )
	{
		DB_TRACK_ db_Track;
		db_Track.hDib = hDib;
		db_Track.iLockCount = 0;
		iIndex = g_DebugHandleTrack.Add(db_Track);
	}
	return iIndex;
}

HANDLE DibAlloc(DWORD dwLen)
{
	HANDLE hDib = ::GlobalAlloc(GHND, dwLen);
	if( hDib )
	{
		//MYTRACE(0, _T("hDib = 0x%p = GlobalAlloc(GHND, %u)\n"), hDib, dwLen);
		DebugAddHandle(hDib);
	}
	//else
		//MYTRACE(0, _T("GlobalAlloc(GHND, %u) failed\n"), dwLen);
	return hDib;
}

LPBYTE DibLock(HANDLE hDib)
{
	LPBYTE pDib = (LPBYTE)::GlobalLock(hDib);
	if( pDib )
	{
		int iIndex = DebugFindHandle(hDib);
		if( iIndex >= 0 )
		{
			DB_TRACK_ db_Track = g_DebugHandleTrack.GetAt(iIndex);
			++db_Track.iLockCount;
			g_DebugHandleTrack.SetAt(iIndex, db_Track);
			//MYTRACE(0, _T("GlobalLock(hDib = 0x%p) : Lock Count = %d\n"),
			//	hDib, db_Track.iLockCount);
		}
	}
	//else
		//MYTRACE(0, _T("GlobalLock(hDib = 0x%p) failed\n"), hDib);
	return pDib;
}

BOOL DibUnlock(HANDLE hDib)
{
	BOOL bResult = ::GlobalUnlock(hDib);
	int iIndex = DebugFindHandle(hDib);
	if( iIndex >= 0 )
	{
		DB_TRACK_ db_Track = g_DebugHandleTrack.GetAt(iIndex);
		if( db_Track.iLockCount > 0 )
		{
			--db_Track.iLockCount;
			g_DebugHandleTrack.SetAt(iIndex, db_Track);
			//MYTRACE(0, _T("GlobalUnlock(hDib = 0x%p) : Lock Count = %d\n"),
			//	hDib, db_Track.iLockCount);
		}
	}
	return bResult;
}

HANDLE DibFree(HANDLE hDib)
{
	HANDLE hOld = ::GlobalFree(hDib);
	if( !hOld )
	{
		int iIndex = DebugFindHandle(hDib);
		if( iIndex >= 0 )
		{
			//MYTRACE(0, _T("GlobalFree(hDib = 0x%p)\n"), hDib);
			g_DebugHandleTrack.RemoveAt(iIndex);
			//MYTRACE(0, _T("Number of DIB handles remaining: %d\n"),
			//	g_DebugHandleTrack.GetSize());
		}
	}
	//else
		//MYTRACE(0, _T("GlobalFree(hDib = 0x%p) failed\n"), hDib);
	return hOld;
}

/////////////////////////////////////////////////////////////////////////////
// Dib dump helpers

void DumpDibFileHeader(const LPBITMAPFILEHEADER pbmfh)
{
	if( !pbmfh )
		return;

	TRACE(_T("\nBITMAPFILEHEADER:\n{\n"));
	TRACE(_T(" bfType      = %X\n"), pbmfh->bfType);
	TRACE(_T(" bfSize      = %u\n"), pbmfh->bfSize);
	TRACE(_T(" bfReserved1 = %u\n"), pbmfh->bfReserved1);
	TRACE(_T(" bfReserved2 = %u\n"), pbmfh->bfReserved2);
	TRACE(_T(" bfOffBits   = %u\n"), pbmfh->bfOffBits);
	TRACE(_T("};\n"));
}

void DumpDibInfoHeader(const LPBYTE pDib)
{
	if( !pDib )
		return;

	DWORD dwSize = *(LPDWORD)pDib;
	if( dwSize == sizeof(BITMAPCOREHEADER) )
	{
		LPBITMAPCOREHEADER pch = (LPBITMAPCOREHEADER)pDib;
		TRACE(_T("\nBITMAPCOREHEADER:\n{\n"));
		TRACE(_T(" bcSize     = %u\n"), pch->bcSize);
		TRACE(_T(" bcWidth    = %u\n"), pch->bcWidth);
		TRACE(_T(" bcHeight   = %u\n"), pch->bcHeight);
		TRACE(_T(" bcPlanes   = %u\n"), pch->bcPlanes);
		TRACE(_T(" bcBitCount = %u\n"), pch->bcBitCount);
		TRACE(_T("};\n"));
		return;
	}

	LPCTSTR szCompression[] =
	{
		_T("BI_RGB"),
		_T("BI_RLE8"),
		_T("BI_RLE4"),
		_T("BI_BITFIELDS"),
		_T("BI_JPEG"),
		_T("BI_PNG")
	};

	LPBITMAPV5HEADER lpHeader = (LPBITMAPV5HEADER)pDib;
	if( dwSize == sizeof(BITMAPINFOHEADER) )
		TRACE(_T("\nBITMAPINFOHEADER:\n{\n"));
	else if( dwSize == sizeof(BITMAPV4HEADER) )
		TRACE(_T("\nBITMAPV4HEADER:\n{\n"));
	else if( dwSize == sizeof(BITMAPV5HEADER) )
		TRACE(_T("\nBITMAPV5HEADER:\n{\n"));

	if( dwSize >= sizeof(BITMAPINFOHEADER) )
	{
		TRACE(_T(" Size          = %d\n"), lpHeader->bV5Size);
		TRACE(_T(" Width         = %d\n"), lpHeader->bV5Width);
		TRACE(_T(" Height        = %d\n"), lpHeader->bV5Height);
		TRACE(_T(" Planes        = %u\n"), lpHeader->bV5Planes);
		TRACE(_T(" BitCount      = %u\n"), lpHeader->bV5BitCount);
		TRACE(_T(" Compression   = %s\n"),
			szCompression[lpHeader->bV5Compression%5]);
		TRACE(_T(" SizeImage     = %u\n"), lpHeader->bV5SizeImage);
		TRACE(_T(" XPelsPerMeter = %d\n"), lpHeader->bV5YPelsPerMeter);
		TRACE(_T(" YPelsPerMeter = %d\n"), lpHeader->bV5XPelsPerMeter);
		TRACE(_T(" ClrUsed       = %u\n"), lpHeader->bV5ClrUsed);
		TRACE(_T(" ClrImportant  = %u\n"), lpHeader->bV5ClrImportant);
	}

	if( dwSize >= sizeof(BITMAPV4HEADER) )
	{
		#define CIE_DEC(V) ((((DWORD)(V)) >> 30) & 0x3)
		#define CIE_MAN(V) (((DWORD)(V)) & 0x3FFFFFFF)

		TRACE(_T(" RedMask       = %u\n"), lpHeader->bV5RedMask);
		TRACE(_T(" GreenMask     = %u\n"), lpHeader->bV5GreenMask);
		TRACE(_T(" BlueMask      = %u\n"), lpHeader->bV5BlueMask);
		TRACE(_T(" AlphaMask     = %u\n"), lpHeader->bV5AlphaMask);
		TRACE(_T(" CSType        = %u\n"), lpHeader->bV5CSType);
		TRACE(_T(" bV5Endpoints\n"));
		TRACE(_T(" {\n"));
		TRACE(_T("  ciexyzRed\n"));
		TRACE(_T("  {\n"));
		TRACE(_T("   ciexyzX     = %d.%d\n"),
			CIE_DEC(lpHeader->bV5Endpoints.ciexyzRed.ciexyzX),
			CIE_MAN(lpHeader->bV5Endpoints.ciexyzRed.ciexyzX));
		TRACE(_T("   ciexyzY     = %d.%d\n"),
			CIE_DEC(lpHeader->bV5Endpoints.ciexyzRed.ciexyzY),
			CIE_MAN(lpHeader->bV5Endpoints.ciexyzRed.ciexyzY));
		TRACE(_T("   ciexyzZ     = %d.%d\n"),
			CIE_DEC(lpHeader->bV5Endpoints.ciexyzRed.ciexyzZ),
			CIE_MAN(lpHeader->bV5Endpoints.ciexyzRed.ciexyzZ));
		TRACE(_T("  };\n"));
		TRACE(_T("  ciexyzGreen\n"));
		TRACE(_T("  {\n"));
		TRACE(_T("   ciexyzX     = %d.%d\n"),
			CIE_DEC(lpHeader->bV5Endpoints.ciexyzGreen.ciexyzX),
			CIE_MAN(lpHeader->bV5Endpoints.ciexyzGreen.ciexyzX));
		TRACE(_T("   ciexyzY     = %d.%d\n"),
			CIE_DEC(lpHeader->bV5Endpoints.ciexyzGreen.ciexyzY),
			CIE_MAN(lpHeader->bV5Endpoints.ciexyzGreen.ciexyzY));
		TRACE(_T("   ciexyzZ     = %d.%d\n"),
			CIE_DEC(lpHeader->bV5Endpoints.ciexyzGreen.ciexyzZ),
			CIE_MAN(lpHeader->bV5Endpoints.ciexyzGreen.ciexyzZ));
		TRACE(_T("  };\n"));
		TRACE(_T("  ciexyzBlue\n"));
		TRACE(_T("  {\n"));
		TRACE(_T("   ciexyzX     = %d.%d\n"),
			CIE_DEC(lpHeader->bV5Endpoints.ciexyzBlue.ciexyzX),
			CIE_MAN(lpHeader->bV5Endpoints.ciexyzBlue.ciexyzX));
		TRACE(_T("   ciexyzY     = %d.%d\n"),
			CIE_DEC(lpHeader->bV5Endpoints.ciexyzBlue.ciexyzY),
			CIE_MAN(lpHeader->bV5Endpoints.ciexyzBlue.ciexyzY));
		TRACE(_T("   ciexyzZ     = %d.%d\n"),
			CIE_DEC(lpHeader->bV5Endpoints.ciexyzBlue.ciexyzZ),
			CIE_MAN(lpHeader->bV5Endpoints.ciexyzBlue.ciexyzZ));
		TRACE(_T("  };\n"));
		TRACE(_T(" };\n"));
		TRACE(_T(" GammaRed      = %u\n"), lpHeader->bV5GammaRed);
		TRACE(_T(" GammaGreen    = %u\n"), lpHeader->bV5GammaGreen);
		TRACE(_T(" GammaBlue     = %u\n"), lpHeader->bV5GammaBlue);
	}

	if( dwSize >= sizeof(BITMAPV5HEADER) )
	{
		TRACE(_T(" Intent        = %u\n"), lpHeader->bV5Intent);
		TRACE(_T(" ProfileData   = %u\n"), lpHeader->bV5ProfileData);
		TRACE(_T(" ProfileSize   = %u\n"), lpHeader->bV5ProfileSize);
		TRACE(_T(" Reserved      = %u\n"), lpHeader->bV5Reserved);
	}

	TRACE(_T("};\n"));
}

void DumpDibBitfields(const LPBYTE pDib)
{
	if( !pDib )
		return;

	CDibDataVars ddv((LPBITMAPINFOHEADER)pDib);
	if( ddv.m_dwFieldOffset )
	{
		LPDWORD pBitfields = LPDWORD(pDib + ddv.m_dwFieldOffset);
// 		TRACE(_T("\nBIT FIELDS\n{\n"
// 				 "   RED: %8.8X\n"
// 				 " GREEN: %8.8X\n"
// 				 "  BLUE: %8.8X\n};\n"),
//			pBitfields[0], pBitfields[1], pBitfields[2]);
	}
}

void DumpDibColorTable(const LPBYTE pDib)
{
	if( !pDib )
		return;

	CDibDataVars ddv((LPBITMAPINFOHEADER)pDib);
	if( ddv.m_dwColorOffset )
	{
		LPRGBQUAD pColorTable = LPRGBQUAD(pDib + ddv.m_dwColorOffset);
		TRACE(_T("\nCOLOR TABLE\n{\n"));
		for( DWORD i = 0; i < ddv.m_dwNumColors; ++i )
		{
			TRACE(_T(" %3.3u) %2.2X, %2.2X, %2.2X\n"),
				i, pColorTable[i].rgbRed, 
				pColorTable[i].rgbGreen, pColorTable[i].rgbBlue);
		}
		TRACE(_T("};\n"));
	}
}

void DumpDib(const LPBYTE pDib)
{
	if( !pDib )
		return;

	DumpDibInfoHeader(pDib);
	DumpDibBitfields(pDib);
	DumpDibColorTable(pDib);

	CDibDataVars ddv((LPBITMAPINFOHEADER)pDib);

	DWORD i, n;
	TCHAR szRaw[512];
	TCHAR szByte[32];
	TRACE(_T("\nPIXEL BITS\n{\n"));
	for( i = ddv.m_dwImageOffset; i < ddv.m_dwDibSize; )
	{
		szRaw[0] = _T('\0');
		for( n = 0; n < 32 && i < ddv.m_dwDibSize; ++n, ++i )
		{
			_stprintf(szByte, _T(" %2.2X"), pDib[i]);
			_tcscat(szRaw, szByte);
		}
		_tcscat(szRaw, _T("\n"));
		TRACE(szRaw);
	}
	TRACE(_T("};\n"));
}

void DumpDibFile(LPCTSTR lpszPathName,BOOL bPixels)
{
	LPBYTE pBuffer = NULL;
	try
	{
		CFile cf(lpszPathName, CFile::modeRead);
		DWORD dwLen = cf.GetLength();
		pBuffer = new BYTE[dwLen];
		if( pBuffer )
			cf.Read((void*)pBuffer, (UINT)dwLen);
		cf.Close();

		if( !pBuffer )
			::SetLastError(ERROR_OUTOFMEMORY);
		else
		{
			LPBITMAPFILEHEADER pbmfh = (LPBITMAPFILEHEADER)pBuffer;
			if( pbmfh->bfType == 0x4D42 )
			{
				if( bPixels )
					TRACE(_T("\nSTART DIB FILE DUMP\n"));
				else
					TRACE(_T("\nSTART PARTIAL DIB FILE DUMP\n"));
				DumpDibFileHeader(pbmfh);
				LPBYTE pDib = pBuffer + sizeof(BITMAPFILEHEADER);
				if( bPixels )
					DumpDib(pDib);
				else
				{
					DumpDibInfoHeader(pDib);
					DumpDibBitfields(pDib);
					DumpDibColorTable(pDib);
				}
				if( bPixels )
					TRACE(_T("\nEND DIB FILE DUMP\n\n"));
				else
					TRACE(_T("\nEND PARTIAL DIB FILE DUMP\n\n"));
			}
		}
	}
	catch( CFileException* /*e*/ )
	{
		TRACE(_T("\nDumpDibFile() : CFileException\n\n"));
	}

	delete [] pBuffer;
}

#endif

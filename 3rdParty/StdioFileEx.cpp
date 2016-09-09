// StdioFileEx.cpp: implementation of the CStdioFileEx class.
//
// Version 1.1 23 August 2003.	Incorporated fixes from Dennis Jeryd.
// Version 1.3 19 February 2005. Incorporated fixes from Howard J Oh and some of my own.
// Version 1.4 26 February 2005. Fixed stupid screw-up in code from 1.3.
// Version 1.5 18 November 2005. - Incorporated fixes from Andy Goodwin.
//	                              - Allows code page to be specified for reading/writing
//	                              - Properly calculates multibyte buffer size instead of
//	                                 assuming lstrlen(s).
//	                              - Should handle UTF8 properly.
// Version 1.6 19 July 2007.     - ReadString incorrectly removed \r or \n characters 
//	                                immediately preceding line breaks.
//                                 Fixed tab problem in these comments! (Perry).
//                                 Made GetMultiByteStringFromUnicodeString input string const
//                                  (Perry).
//                                 Avoided double conversion if code page not set.
//                                  (Konrad Windszus).
//                                 Fixed ASSERT in GetUnicodeStringFromMultiByteString
//                                  (Konrad Windszus).
//                                 Maximum line length restriction removed. Lines of any length
//                                   can now be read thanks to C.B. Falconer's fggets (fgoodgets),
//                                   ably assisted by Ana Sayfa and Dave Kondrad.
//                                 Substantial code reorganisation and tidying.
//                                 Use of strlen/lstrlen eliminated. Conversion functions always used
//                                   to calculate required buffers.
//                                 Serious, systematic tests are now included with the code.
//                                 Options included to switch off BOM writing and alter the Unicode
//                                   filler char.
//											  BOM is only stripped off if actually there.
//                                 UTF-8 BOM is now read and written. UTF-8 conversion works.
//
// Copyright David Pritchard 2003-2007. davidpritchard@ctv.es
//
// You can use this class freely, but please keep my ego happy 
// by leaving this comment in place.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StdioFileEx.h"
#include "ggets.h"
#include <WCHAR.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif 

//////////////////////////////////////////////////////////////////////

const unsigned char UTF8_BOM[] = { unsigned char(0xEF), unsigned char(0xBB), unsigned char(0xBF) };
const UINT UTF8_BOM_SIZE = sizeof(UTF8_BOM);

const wchar_t UNICODE_BOM[] = { 0xFEFF };
const UINT UNICODE_BOM_SIZE = sizeof(UNICODE_BOM);

//////////////////////////////////////////////////////////////////////

#define MAXBOMLEN max(UTF8_BOM_SIZE, UNICODE_BOM_SIZE)

//////////////////////////////////////////////////////////////////////

// Macro to do post-exception cleanup. Rethrows exception afterwards
#define	FINALLY(x)		catch(...)\
                        {\
                           ASSERT(false);\
                           x;\
                           throw;\
                        }\
                        x;

//////////////////////////////////////////////////////////////////////

// An adaptation of the supremely handy DELETE_SAFE macro by mandhjo
// (http://www.experts-exchange.com/Programming/Programming_Languages/MFC/Q_10292771.html)
#define	DELETE_SAFE_ARRAY(parr)		if (parr)\
                                 {\
                                   delete [] parr;\
                                   parr = NULL;\
                                 }

//////////////////////////////////////////////////////////////////////

// Add this flag to write in Unicode. For the moment, out of range of all the Visual Studio 2005 flags
const UINT CStdioFileEx::modeWriteUnicode = 0x200000; 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStdioFileEx::CStdioFileEx()
	: 
	CStdioFile(),
	m_bCheckFilePos(true), 
	m_cUnicodeFillerChar(SFE_DEFAULT_UNICODE_FILLER_CHAR), 
	m_nFileCodePage(-1), 
	m_nFormat(SFEF_UNKNOWN)
{
}

CStdioFileEx::CStdioFileEx(LPCWSTR lpszFileName,UINT nOpenFlags)
	:
	CStdioFile(lpszFileName, nOpenFlags),
	m_bCheckFilePos(true), 
	m_cUnicodeFillerChar(SFE_DEFAULT_UNICODE_FILLER_CHAR), 
	m_nFileCodePage(-1), 
	m_nFormat(SFEF_UNKNOWN)
{
}

BOOL CStdioFileEx::Open(LPCWSTR lpszFileName, UINT nOpenFlags, SFE_FORMAT nFormat, UINT nAnsiCodePage, CFileException* pError /*=NULL*/)
{
	// Process any Unicode stuff. This no longer checks for the Unicode BOM. We do this on
	// opening for efficiency.
	ProcessOpenFlags(nOpenFlags, nFormat);

	if (!CStdioFile::Open(lpszFileName, nOpenFlags, pError))
		return FALSE;

	m_bCheckFilePos = true;
	m_nBytesWritten = 0;

	switch (nFormat)
	{
	case SFEF_AUTODETECT:
		m_nFormat = ReadFormat(nAnsiCodePage);
		m_nFileCodePage = GetFileCodePage(m_nFormat, nAnsiCodePage);
		break;

	case SFEF_UTF16:
	case SFEF_UTF8WITHOUTBOM:
	case SFEF_UTF8:
	case SFEF_ANSI:
		m_nFormat = nFormat;
		m_nFileCodePage = GetFileCodePage(nFormat, nAnsiCodePage);
		break;

	default:
		ASSERT(0);
		return FALSE;
	}

	// Reset to start of file
	SeekToBegin();

	return TRUE;
}

int CStdioFileEx::GetFileCodePage(SFE_FORMAT nFormat, int nAnsiCodePage)
{
	switch (nFormat)
	{
	case SFEF_UTF16: 
		return -1;
		
	case SFEF_UTF8WITHOUTBOM:
	case SFEF_UTF8:
		return CP_UTF8;
		
	case SFEF_ANSI:
		return nAnsiCodePage;
	}
	
	// all else
	ASSERT(0);
	return -1;
}

SFE_FORMAT CStdioFileEx::ReadFormat(int nAnsiCodePage)
{
	SFE_FORMAT nFormat = SFEF_UNKNOWN;

	SeekToBegin();

	if (GetLength() >= MAXBOMLEN)
	{
		BYTE cBOMTest[MAXBOMLEN];
		Read(&cBOMTest, MAXBOMLEN);

		// check for unicode BOM
		if (memcmp(&cBOMTest, UNICODE_BOM, UNICODE_BOM_SIZE) == 0)
		{
			nFormat = SFEF_UTF16;
		}
		else if (memcmp(&cBOMTest, UTF8_BOM, UTF8_BOM_SIZE) == 0)
		{
			nFormat = SFEF_UTF8;
		}
		else if (nAnsiCodePage == CP_ACP)
		{
			nFormat = SFEF_UTF8WITHOUTBOM;
		}
		else
		{
			nFormat = SFEF_ANSI;
		}
	}

	return nFormat;
}

int CStdioFileEx::CheckReadBOM()
{
	// If at position 0, discard byte-order mark before reading. To optimise reading, we only
	// check this when the m_bCheckFilePos is set (this avoids a call to ftell every time we
	// read a line)
	if (m_bCheckFilePos && (GetPosition() == 0))
	{
		BYTE cBOM[MAXBOMLEN] = { 0 };
		
		// Look for Unicode BOM
		if (m_nFormat == SFEF_UTF16)
		{
			if (Read(cBOM, UNICODE_BOM_SIZE) == UNICODE_BOM_SIZE)
			{
				if (memcmp(cBOM, UNICODE_BOM, UNICODE_BOM_SIZE) == 0)
					return UNICODE_BOM_SIZE;
				
				// else restore pos
				SeekToBegin();
			}
		}
		// Look for UTF8 BOM
		else if (m_nFormat == SFEF_UTF8)
		{
			if (Read(cBOM, UTF8_BOM_SIZE) == UTF8_BOM_SIZE)
			{
				if (memcmp(cBOM, UTF8_BOM, UTF8_BOM_SIZE) == 0)
					return UTF8_BOM_SIZE;
				
				// else restore pos
				SeekToBegin();
			}
		}
	}
	
	// all else
	return 0;
}

BOOL CStdioFileEx::ReadString(CString& rString)
{
	ASSERT(m_pStream != NULL);

	BOOL		bReadData = FALSE;
	LPWSTR		lpsz;
	int			nLen = 0;

	CheckReadBOM();

	// Read Unicode line or multibyte line (implementations 
	// differ depending on the compilation)
	if (m_nFormat == SFEF_UTF16)
	{
		bReadData = ReadUnicodeLine(rString);
	}
	else
	{
		bReadData = ReadMultiByteLine(rString);
	}

	// Then remove end-of-line character as necessary.  
	// fggets keeps the end-of-line confusion level at maximum by stripping the \n
	// from the end of lines, but leaving the \r. Grrrr.
	// Remember that you could quite legitimately have a \r or \n at the end of
	// your line before the actual \r\n line break. 
	if (bReadData)
	{
		// Copied from FileTxt.cpp but adapted to use of fgets
		nLen = rString.GetLength();
		
		lpsz = rString.GetBuffer(0);

		// Strip \r from the end
		if (nLen != 0 && (lpsz[nLen-1] == _T('\r') ))
		{
			rString.GetBufferSetLength(nLen-1);
		}

		rString.ReleaseBuffer();

		// Now we've moved on in the file, don't bother to check any more unless the
		// file pointer is moved
		m_bCheckFilePos = false;
	}

	return bReadData;
}

LPWSTR CStdioFileEx::ReadString(LPWSTR lpsz,UINT nMax)
{
	// Can't really provide compatible behaviour at the moment, so no Unicode handling.
	ASSERT(false);

	return CStdioFile::ReadString(lpsz, nMax);
}

BOOL CStdioFileEx::ReadFile(CString& sFile)
{
	SeekToBegin();
	int nByteOffset = CheckReadBOM();

	// read entire file in as a byte stream
	DWORD dwBytesLeft = ((DWORD)GetLength() - nByteOffset), dwTotalBytesRead = 0;

	if (dwBytesLeft == 0) // zero length file
		return TRUE;

	LPSTR pFileContents = new CHAR[dwBytesLeft + 2]; // +2 for unicode zero terminator
	ZeroMemory(pFileContents, dwBytesLeft + 2);
	LPSTR pFilePtr = pFileContents;
		
	while (dwBytesLeft > 0)
	{
		DWORD nRead = Read(pFilePtr, dwBytesLeft);

		pFilePtr += nRead;
		dwTotalBytesRead += nRead;
		dwBytesLeft -= nRead;
	}

	if (m_nFormat == SFEF_UTF16) // byte stream is already unicode format
	{
		sFile = (LPCWSTR)pFileContents;
	}
	else // convert _from_ multibyte
	{
		int nCodePage = ((m_nFileCodePage == -1) ? GetCurrentLocaleCodePage() : m_nFileCodePage);
		int nLen = MultiByteToWideChar(nCodePage, 0, (LPCSTR)pFileContents, dwTotalBytesRead, NULL, 0);

		// if wlen == 0, some unknown codepage was probably used.
		ASSERT(nLen);

		if (nLen > 0) 
		{
			LPWSTR pTo = sFile.GetBufferSetLength(nLen); 
			nLen = MultiByteToWideChar(nCodePage, 0, (LPCSTR)pFileContents, dwTotalBytesRead, pTo, nLen);
		}
	}

	delete [] pFileContents;
	return !sFile.IsEmpty();
}


// --------------------------------------------------------------------------------------------
//
//	CStdioFileEx::WriteString()
//
// --------------------------------------------------------------------------------------------
// Returns:    void
// Parameters: LPCWSTR lpsz
//
// Purpose:		Writes string to file either in Unicode or multibyte, depending on whether the caller specified the
//					CStdioFileEx::modeWriteUnicode flag. Override of base class function.
// Notes:		If writing in Unicode we need to:
//						a) Write the Byte-order-mark at the beginning of the file
//						b) Write all strings in byte-mode
//					-	If we were compiled in Unicode, we need to convert Unicode to multibyte if 
//						we want to write in multibyte
//					-	If we were compiled in multi-byte, we need to convert multibyte to Unicode if 
//						we want to write in Unicode.
// Exceptions:	None.
//
void CStdioFileEx::WriteString(LPCWSTR lpsz)
{
	if (m_nFormat == SFEF_UNKNOWN)
	{
		ASSERT(0);
		return;
	}

	// If at position 0, write byte-order mark before writing anything else
	if (!m_pStream || (GetPosition() == 0))
	{
		switch (m_nFormat)
		{
		case SFEF_UTF16:
			{
				CFile::Write(&UNICODE_BOM, UNICODE_BOM_SIZE);
				m_nBytesWritten = UNICODE_BOM_SIZE;
			}
			break;

		case SFEF_UTF8:
			{
				CFile::Write(UTF8_BOM, UTF8_BOM_SIZE);
				m_nBytesWritten = UTF8_BOM_SIZE;
			}
			break;


		default:
			m_nBytesWritten = 0;
			break;
		}

	}

	// write the string
	switch (m_nFormat)
	{
	case SFEF_UTF16:
		m_nBytesWritten += WriteUnicodeLine(lpsz);
		break;
		
	default:
		m_nBytesWritten += WriteMultiByteLine(lpsz);
		break;
	}
}

BOOL CStdioFileEx::IsSaving(UINT nOpenFlags)
{
	return (nOpenFlags & (CFile::modeWrite | CFile::modeReadWrite));
}

void CStdioFileEx::ProcessOpenFlags(UINT& nOpenFlags, SFE_FORMAT nFormat)
{
	// add unicode as required if saving
	if (IsSaving(nOpenFlags))
	{
		switch (nFormat)
		{
		case SFEF_UTF16:
			nOpenFlags |= CStdioFileEx::modeWriteUnicode;
			break;
			
		default:
			nOpenFlags &= ~CStdioFileEx::modeWriteUnicode;
			break;
		}

		// must have read/write access
		nOpenFlags &= ~CFile::modeWrite;
		nOpenFlags |= CFile::modeReadWrite;
	}

	// Always use binary mode, for any type of writing
	nOpenFlags &= ~CFile::typeText;
	nOpenFlags |= CFile::typeBinary;
}

STDIOEXLONG CStdioFileEx::Seek(STDIOEXLONG lOff,UINT nFrom)
{
	// Note that we should check the file position on the next read
	m_bCheckFilePos = true;

	return CStdioFile::Seek((LONG)lOff, nFrom);
}

// Get the current user´s code page
UINT CStdioFileEx::GetCurrentLocaleCodePage()
{
	_TCHAR	szLocalCodePage[10];
	UINT		nLocaleCodePage = 0;
	int		nLocaleChars = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, szLocalCodePage, 10);

	// If OK
	if (nLocaleChars > 0)
	{
		nLocaleCodePage = (UINT)_ttoi(szLocalCodePage);
		ASSERT(nLocaleCodePage > 0);
	}
	else
	{
		ASSERT(false);
	}

	// O means either: no ANSI code page (Unicode-only locale?) or failed to get locale
	// In the case of Unicode-only locales, what do multibyte apps do? Answers on a postcard.
	return nLocaleCodePage;
}

// --------------------------------------------------------------------------------------------
//
//	CStdioFileEx::GetUnicodeStringFromMultiByteString()
//
// --------------------------------------------------------------------------------------------
// Returns:    int - number of chars written
// Parameters: LPCSTR		szMultiByteString		(IN)	Multi-byte input string
//					wchar_t*		szUnicodeString		(OUT)	Unicode outputstring
//					size_t		nUnicodeBufferSize	(IN)	Size of Unicode output buffer in chars
//					UINT			nCodePage				(IN)	Code page used to perform conversion
//																		Default = CP_ACP (Get local code page).
//
// Purpose:		Gets a Unicode string from a MultiByte string.
// Notes:		None.
// Exceptions:	None.
//
int CStdioFileEx::GetUnicodeStringFromMultiByteString(IN LPCSTR szMultiByteString, OUT wchar_t* szUnicodeString, IN const size_t nUnicodeBufferSize, IN UINT nCodePage)
{
	int		nCharsWritten = 0;
		
	if (szUnicodeString && szMultiByteString)
	{
		// If no code page specified, take default for system
		if (nCodePage == (UINT)-1)
		{
			nCodePage = GetACP();
		}

		try 
		{
			// Zero out buffer first
			memset((void*)szUnicodeString, '\0', sizeof(wchar_t) * nUnicodeBufferSize);

			// When converting to UTF8, don't set any flags (see Q175392).
			nCharsWritten = MultiByteToWideChar((UINT)nCodePage,(nCodePage==CP_UTF8 ?
				0:MB_PRECOMPOSED),szMultiByteString,-1,szUnicodeString, (int)nUnicodeBufferSize);
		}
		catch(...)
		{
			// Level 4 compile says this is unreachable code in VS2005. I guess MultiByteToWideChar doesn't throw exceptions.
//			TRACE(_T("Controlled exception in MultiByteToWideChar!\n"));
		}
	}
	else
	{
		ASSERT(false);
	}

	// Konrad Windszus 29/3/2006: ASSERT in wrong place!
	// Should have at least the terminator (right?)
	ASSERT(nCharsWritten > 0);

	// Now fix nCharsWritten to exclude \0 terminator 
	if (nCharsWritten > 0)
	{
		nCharsWritten--;
	}

	return nCharsWritten;
}

// --------------------------------------------------------------------------------------------
//
//	CStdioFileEx::GetRequiredUnicodeLengthFromMultiByteString()
//
// --------------------------------------------------------------------------------------------
// Returns:    int - number of chars needed
// Parameters: LPCSTR		szMultiByteString		(IN)	Multi-byte input string
//					UINT			nCodePage				(IN)	Code page of input string
//																		Default = CP_ACP (local code page).
//
// Purpose:		Gets the length required, in wchar_t values (chars) to convert a MultiByte string to a Unicode string.
// Notes:		None.
// Exceptions:	None.
//
int CStdioFileEx::GetRequiredUnicodeLengthFromMultiByteString(IN LPCSTR szMultiByteString,IN UINT nCodePage /*=CP_ACP OPTIONAL*/)
{
	int	nCharsNeeded = 0;
		
	if (szMultiByteString)
	{
		// If no code page specified, take default for system
		if (nCodePage == (UINT)-1)
		{
			nCodePage = GetACP();
		}

		try 
		{
			// When converting to UTF8, don't set any flags (see Q175392).
			nCharsNeeded = MultiByteToWideChar((UINT)nCodePage,(nCodePage==CP_UTF8 ?
				0:MB_PRECOMPOSED),
				szMultiByteString,
				-1,	// Get the function to work out the length
				NULL, // No output buffer needed
				0		// Ask to be told how many chars we need (includes space for terminator)
				);
		}
		catch(...)
		{
			// Level 4 compile says this is unreachable code in VS2005. I guess MultiByteToWideChar doesn't throw exceptions.
//			TRACE(_T("Controlled exception in MultiByteToWideChar!\n"));
		}
	}
	else
	{
		ASSERT(false);
	}

	// Konrad Windszus 29/3/2006: ASSERT in wrong place!
	// Should have at least the terminator (right?). The value returned by MultiByteToWideChar includes the terminator.
	ASSERT(nCharsNeeded > 0);

	return nCharsNeeded;
}

// --------------------------------------------------------------------------------------------
//
//	CStdioFileEx::GetNewUnicodeStringFromMultiByteString()
//
// --------------------------------------------------------------------------------------------
// Returns:    int - number of chars written (0 means error)
// Parameters: LPCSTR		szMultiByteString		(IN)		Multi-byte input string
//					CTemplateSmartPtrArray<wchar_t>& 
//									spUnicodeString		(IN/OUT)	Smart pointer containing default buffer (or NULL) 
//																				on input, and pointing to buffer used for conversion
//																				on output. A newly allocated buffer will be automatically
//																				deleted when the smart ptr object is destroyed.
//																				This allows a default buffer to be declared and used for
//																				most strings. Dynamic allocation is only performed when
//																				the default buffer would not be large enough.
//					int			nDefaultBufferSize	(IN)		Size of default buffer in smart ptr (may be 0).
//					UINT			nCodePage				(IN)		Code page used to perform conversion
//																			Default = CP_ACP (Get local code page).
//
// Purpose:		Gets a Unicode string from a MultiByte string. Calculates the buffer for you and
//					allocates it with "new". 
// Notes:		It's better to ask this function to allocate the buffer for you, because it will
//					calculate the correct size. If we just take the number of bytes from the multibyte
//					string as the size, we won't be in danger of allocating too little memory, but we
//					may well allocate too much.
//					
//					The use of a smart ptr array combines this flexibility with efficiency. A default buffer can be passed in
//					and used wherever is it sufficient to contain the output string. This avoids lots of unnecessary "new"s and
//					"delete"s when reading or writing large files.
// Exceptions:	None.
//
int CStdioFileEx::GetNewUnicodeStringFromMultiByteString(IN LPCSTR szMultiByteString,IN OUT CTemplateSmartPtrArray<wchar_t>& spUnicodeString, IN const int nDefaultBufferSize/*=0*/,IN UINT nCodePage /*=CP_ACP OPTIONAL*/)
{
	int nActualBufferSize = nDefaultBufferSize;

	// Calculate the required buffer size and allocate
	int nUnicodeBufferSizeChars = GetRequiredUnicodeLengthFromMultiByteString(szMultiByteString, nCodePage);

	// If we have enough in the default buffer, don't bother to allocate
	if (nUnicodeBufferSizeChars > nDefaultBufferSize)
	{
		// The new buffer will be marked as "owned" by the smart ptr by default, and therefore automatically deleted by the smart ptr
		spUnicodeString.Assign(new wchar_t[nUnicodeBufferSizeChars]);

		nActualBufferSize = nUnicodeBufferSizeChars;
	}

	// Call standard
	return GetUnicodeStringFromMultiByteString(szMultiByteString, spUnicodeString.GetBuffer(), nActualBufferSize, nCodePage);
}

// --------------------------------------------------------------------------------------------
//
//	CStdioFileEx::GetMultiByteStringFromUnicodeString()
//
// --------------------------------------------------------------------------------------------
// Returns:    int	- number of chars written. 0 if error.
// Parameters: wchar_t *	szUnicodeString			(IN)	Unicode input string
//					char*			szMultiByteString			(OUT)	Multibyte output string
//					int			nMultiByteBufferSize		(IN)	Multibyte buffer size
//					UINT			nCodePage					(IN)	Code page used to perform conversion
//																			Default = CP_ACP (Get local code page).
//					char			cFillerChar					(IN)  Unicode-to-multibyte filler char 
//																			Default = #
//
// Purpose:		Gets a MultiByte string from a Unicode string.
// Notes:		It's better to ask this function to allocate the buffer for you, because it will
//					calculate the correct size. Multibyte code pages will require larger buffers than
//					the normal Western code pages, so we can't just say new char[numchars]!
// Exceptions:	None.
//
int CStdioFileEx::GetMultiByteStringFromUnicodeString(IN const wchar_t * szUnicodeString, OUT char* szMultiByteString, 
																			IN const int nMultiByteBufferSize, IN UINT nCodePage/*=CP_ACP OPTIONAL*/,
																			IN char cFillerChar/*=sDEFAULT_UNICODE_FILLER_CHAR OPTIONAL*/)
{
	BOOL			bUsedDefChar	= FALSE;
	int			nBytesWritten = 0;

	// Fix by Andy Goodwin: don't do anything if buffer is 0
	if ( nMultiByteBufferSize > 0 )
	{
		if (szUnicodeString && szMultiByteString) 
		{
			// Zero out buffer first
			memset((void*)szMultiByteString, '\0', sizeof(char) * nMultiByteBufferSize);

			// If no code page specified, take default for system
			if (nCodePage == (UINT)-1)
			{
				nCodePage = (UINT)GetACP();
			}

			try 
			{
				// If writing to UTF8, flags, default char and boolean flag must be NULL
				nBytesWritten = WideCharToMultiByte((UINT)nCodePage, 
					(nCodePage==CP_UTF8 ? 0 : WC_COMPOSITECHECK | WC_SEPCHARS), // Flags
					szUnicodeString,-1, 
					szMultiByteString, 
					nMultiByteBufferSize, 
					(nCodePage==CP_UTF8 ? NULL: &cFillerChar),		// Filler char
					(nCodePage==CP_UTF8 ? NULL: &bUsedDefChar));		// Did we use filler char?

				// If no chars were written and the buffer is not 0, error!
				if (nBytesWritten == 0 && nMultiByteBufferSize > 0)
				{
					TRACE1("Error in WideCharToMultiByte: %d\n", ::GetLastError());
				}
			}
			catch(...) 
			{
				TRACE0("Controlled exception in WideCharToMultiByte!\n");
			}
		} 
	}

	// Now fix nCharsWritten to exclude \0 terminator 
	if (nBytesWritten > 0)
	{
		nBytesWritten = nBytesWritten - sizeof('\0');
	}

	return nBytesWritten;
}



//---------------------------------------------------------------------------------------------------
//
//	CStdioFileEx::GetRequiredMultiByteLengthForUnicodeString()
//
//---------------------------------------------------------------------------------------------------
// Returns:    int - no of bytes required
// Parameters: wchar_t *	szUnicodeString			(IN)	String to convert
//					UINT			nCodePage=CP_ACP			(IN)	Code page to which to convert
//
// Purpose:		Obtains the multi-byte buffer size (in bytes) needed to accommodate a converted Unicode string.
//	Notes:		We can't assume that the buffer length is simply equal to the number of characters
//					because that wouldn't accommodate multibyte characters!
//
int CStdioFileEx::GetRequiredMultiByteLengthForUnicodeString(IN const wchar_t * szUnicodeString,IN UINT nCodePage /*=CP_ACP OPTIONAL*/)
{
	int nBytesNeeded = 0;

	try 
	{
		// If no code page specified, take default for system
		if (nCodePage == -1)
		{
			nCodePage = GetACP();
		}

		// If writing to UTF8, flags, default char and boolean flag must be NULL
		nBytesNeeded = WideCharToMultiByte((UINT)nCodePage, 
			(nCodePage==CP_UTF8 ? 0 : WC_COMPOSITECHECK | WC_SEPCHARS), // Flags
			szUnicodeString,-1, 
			NULL, 
			0,	// Calculate required buffer, please! (Includes space for terminator)
			NULL,	// Filler char doesn't matter here
			NULL);
	}
	catch(...) 
	{
		// TRACE(_T("Controlled exception in WideCharToMultiByte!\n"));
		// Gives us "unreachable code" error compiling on level 4
	}

	return nBytesNeeded;
}


// --------------------------------------------------------------------------------------------
//
//	CStdioFileEx::GetNewMultiByteStringFromUnicodeString()
//
// --------------------------------------------------------------------------------------------
// Returns:    int	- number of chars written. 0 if error.
// Parameters: wchar_t *	szUnicodeString			(IN)		Unicode input string
//					CTemplateSmartPtrArray<char>& 
//									spMultiByteString			(IN/OUT)	Smart pointer containing default buffer (or NULL) 
//																					on input, and pointing to buffer used for conversion
//																					on output. A newly allocated buffer will be automatically
//																					deleted when the smart ptr object is destroyed.
//																					This allows a default buffer to be declared and used for
//																					most strings. Dynamic allocation is only performed when
//																					the default buffer would not be large enough.
//					int			nDefaultBufferSize		(IN)		Size of default buffer in smart ptr (may be 0).
//					UINT			nCodePage					(IN)		Code page used to perform conversion
//																					Default = CP_ACP (Get local code page).
//					char			cFillerChar					(IN)		Unicode-to-multibyte filler char 
//																				Default = #
//
// Purpose:		Gets a MultiByte string from a Unicode string. Calculates the buffer for you and
//					allocates it with new. 
// Notes:		It's better to ask this function to allocate the buffer for you, because it will
//					calculate the correct size. Multibyte code pages will require larger buffers than
//					the normal Western code pages, so we can't just say new char[numchars]!
//					
//					The use of a smart ptr array combines this flexibility with efficiency. A default buffer can be passed in
//					and used wherever is it sufficient to contain the output string. This avoids lots of unnecessary "new"s and
//					"delete"s when reading or writing large files.
// Exceptions:	None.
//
int CStdioFileEx::GetNewMultiByteStringFromUnicodeString(IN const wchar_t * szUnicodeString,IN OUT CTemplateSmartPtrArray<char>& spMultiByteString, IN const int nDefaultBufferSize/*=0*/,
																			IN UINT nCodePage /*=CP_ACP OPTIONAL*/, IN char cFillerChar/*=sDEFAULT_UNICODE_FILLER_CHAR OPTIONAL*/)
{
	int nActualBufferSize = nDefaultBufferSize;

	// Calculate the required buffer size and allocate
	int nMultibyteBufferSizeBytes = GetRequiredMultiByteLengthForUnicodeString(szUnicodeString, nCodePage);

	// If we have enough in the default buffer, don't bother to allocate
	if (nMultibyteBufferSizeBytes > nDefaultBufferSize)
	{
		// The new buffer will be marked as "owned" by the smart ptr by default, and therefore automatically deleted by the smart ptr
		spMultiByteString.Assign(new char[nMultibyteBufferSizeBytes]);

		nActualBufferSize = nMultibyteBufferSizeBytes;
	}

	// Call standard
	return GetMultiByteStringFromUnicodeString(szUnicodeString, spMultiByteString.GetBuffer(), nActualBufferSize, nCodePage, cFillerChar);
}

/**********************************************************************************/
/*                                  Reading                                       */
/**********************************************************************************/

BOOL CStdioFileEx::ReadUnicodeLine(OUT CString& sOutputLine)
{
	BOOL			bReadData = FALSE;
	wchar_t*		pszUnicodeString = (wchar_t*)&m_arrUnicodeDefaultBuffer;
	bool			bNeedToDelete = false;

	try
	{
		bReadData = (FGGETS_OK == fggets<wchar_t>(&pszUnicodeString, m_pStream, bNeedToDelete, SFE_DEFAULT_BUFFER_SIZE));

		if (bReadData)
		{
			sOutputLine = (CString)pszUnicodeString;
		}
		else
		{
			sOutputLine.Empty();
		}
	}
	// Ensure we always clean up, no matter what
	FINALLY( if (bNeedToDelete) DELETE_SAFE_ARRAY(pszUnicodeString); )

	return bReadData;
}

// Read Multibyte in Unicode compilation
BOOL CStdioFileEx::ReadMultiByteLine(OUT CString& sOutputLine)
{
	BOOL			bReadData = FALSE;
	char	*		pszMultiByteString = (char*)&m_arrMultibyteDefaultBuffer;;
	int			nChars = 0;
	bool			bNeedToDelete = false;

	try
	{
		// Read the string -- the function dynamically allocates the necessary memory according
		// to the line length
		bReadData = (FGGETS_OK == fggets<char>(&pszMultiByteString, m_pStream, bNeedToDelete, SFE_DEFAULT_BUFFER_SIZE));

		if (bReadData)
		{
			// Assign default buffer to take care of 99% of cases. Not owned by smart ptr array, so won't be deleted
			CTemplateSmartPtrArray<wchar_t>	spUnicodeString((wchar_t*)&m_arrUnicodeDefaultBuffer, bTEMPLATESMARTPTR_NOTOWNED);

			// Use all-in-one allocation and conversion function. Avoid _mbslen and such like since they're unreliable
			// with UTF8
			nChars = GetNewUnicodeStringFromMultiByteString(pszMultiByteString, spUnicodeString, SFE_DEFAULT_BUFFER_SIZE, m_nFileCodePage);

			if (nChars > 0)
			{
				sOutputLine = (CString)spUnicodeString.GetBuffer();
			}
		}
		
		// Empty the string if we failed to read anything
		if (nChars == 0)
		{
			sOutputLine.Empty();
		}
	}
	// Ensure we always clean up, no matter what
	FINALLY( if (bNeedToDelete) DELETE_SAFE_ARRAY(pszMultiByteString); )

	return bReadData;
}

/**********************************************************************************/
/*                                  Writing                                       */
/**********************************************************************************/

// Unicode in Unicode -- no conversion needed
UINT CStdioFileEx::WriteUnicodeLine(IN LPCWSTR sInputLine)
{
	// Write in byte mode
	UINT nLenBytes = (UINT)(wcslen(sInputLine) * sizeof(wchar_t));
	CFile::Write(sInputLine, nLenBytes);

	return nLenBytes;
}

// Multibyte/ANSI in Unicode -- have to convert to Multibyte, taking into account the desired code page
UINT CStdioFileEx::WriteMultiByteLine(IN LPCWSTR sInputLine)
{
	int			nCharsWritten = 0;

	// Assign default buffer to take care of 99% of cases. Not owned by smart ptr array, so won't be deleted
	CTemplateSmartPtrArray<char>	spMultiByteString((char*)&m_arrMultibyteDefaultBuffer, bTEMPLATESMARTPTR_NOTOWNED);

	// Call all-in-one function to calculate required buffer size and allocate
	nCharsWritten = GetNewMultiByteStringFromUnicodeString(sInputLine, spMultiByteString, SFE_DEFAULT_BUFFER_SIZE, m_nFileCodePage, m_cUnicodeFillerChar);

	if (nCharsWritten > 0)
	{
		// Do byte-mode write using actual chars written (fix by Howard J Oh)
		CFile::Write((const void*)spMultiByteString.GetBuffer(), nCharsWritten*sizeof(char));
	}

	return nCharsWritten*sizeof(char);
}


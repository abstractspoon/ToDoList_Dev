// StdioFileEx.h: interface for the CStdioFileEx class.
//
// Version 1.1 23 August 2003.	Incorporated fixes from Dennis Jeryd.
// Version 1.3 19 February 2005. Incorporated fixes from Howard J Oh and some of my own.
// Version 1.4 26 February 2005. Fixed stupid screw-up in code from 1.3.
// Version 1.5 18 November 2005. - Incorporated fixes from Andy Goodwin.
//	                              - Allows code page to be specified for reading/writing
//	                              - Properly calculates multibyte buffer size instead of
//	                              	assuming lstrlen(s).
//	                              - Should handle UTF8 properly.
// Version 1.6 ??? 2006.			- ReadString incorrectly removed \r or \n characters 
//	                                immediately preceding line breaks
//                                 Fixed tab problem in these comments! (Perry)
//                                 Made GetMultiByteStringFromUnicodeString input string const
//                                  (Perry)
//                                 Avoided double conversion if code page not set.
//                                  (Konrad Windszus)
//                                 Fixed ASSERT in GetUnicodeStringFromMultiByteString
//                                  (Konrad Windszus)
//                                 Maximum line length restriction removed. Lines of any length
//                                   can now be read thanks to C.B. Falconer's fggets (fgoodgets),
//                                   ably assisted by Ana Sayfa and Dave Kondrad.
//                                 Substantial code reorganisation and tidying.
//                                 _mbstrlen now used everywhere instead of strlen/lstrlen 
//                                   to get correct multibyte string length.
//                                 Serious, systematic tests are now included with the code.
//											  BOM is only stripped off if actually there.
//                                 UTF-8 BOM is now read and written. UTF-8 conversion works.
//
// Copyright David Pritchard 2003-2007. davidpritchard@ctv.es
//
// You can use this class freely, but please keep my ego happy 
// by leaving this comment in place.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDIOFILEEX_H__41AFE3CA_25E0_482F_8B00_C40775BCDB81__INCLUDED_)
#define AFX_STDIOFILEEX_H__41AFE3CA_25E0_482F_8B00_C40775BCDB81__INCLUDED_

#include "TemplateSmartPtr.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#define	MS_COMPILER_VS2005	1400		// v8.0

// Make length type ULONGLONG for VS 2005
#if _MSC_VER < MS_COMPILER_VS2005  // If prior to VS 2005
	typedef unsigned long STDIOEXLONG;
#else
	typedef ULONGLONG STDIOEXLONG;
#endif 

//////////////////////////////////////////////////////////////////////

#define	SFE_DEFAULT_UNICODE_FILLER_CHAR		'#'			// Filler char used when no conversion from Unicode to local code page is possible
#define	SFE_DEFAULT_BUFFER_SIZE				4096		// Size of default buffer for strings

//////////////////////////////////////////////////////////////////////

enum SFE_FORMAT
{
	SFEF_UNKNOWN = -1,
	SFEF_AUTODETECT = SFEF_UNKNOWN,
	SFEF_UTF16,
	SFEF_UTF8WITHOUTBOM,
	SFEF_UTF8,
	SFEF_ANSI,
};

//////////////////////////////////////////////////////////////////////

class CStdioFileEx: public CStdioFile
{
public:
	CStdioFileEx();
	CStdioFileEx(LPCWSTR lpszFileName, UINT nOpenFlags);

	virtual BOOL			Open(LPCWSTR lpszFileName, UINT nOpenFlags, SFE_FORMAT nFormat = SFEF_AUTODETECT, UINT nCodePage = CP_ACP, CFileException* pError = NULL);

	virtual BOOL			ReadString(CString& rString);
	virtual LPWSTR			ReadString(LPWSTR lpsz, UINT nMax);
	virtual void			WriteString(LPCWSTR lpsz);
	virtual STDIOEXLONG		Seek(STDIOEXLONG lOff, UINT nFrom);

	BOOL					ReadFile(CString& sFile);
	UINT					GetBytesWritten() const { return m_nBytesWritten; }

	SFE_FORMAT				GetFormat() const { return m_nFormat; }

	static BOOL				LoadFile(LPCWSTR szPathname, CString& sText, BOOL bDenyWrite = FALSE);
	static int				LoadFile(LPCWSTR szPathname, CStringArray& aLines, int nNumLines = -1);
	static BOOL				SaveFile(LPCWSTR szPathname, const CString& sText, SFE_FORMAT nFormat, UINT nCodePage = CP_ACP);

	static UINT				GetCurrentLocaleCodePage();
	static BOOL				IsSaving(UINT nOpenFlags);

protected:

	int     CheckReadBOM();
	BOOL	ReadUnicodeLine(OUT CString& sOutputLine);
	BOOL	ReadMultiByteLine(OUT CString& sOutputLine);
	UINT	WriteUnicodeLine(IN LPCWSTR sInputLine);
	UINT	WriteMultiByteLine(IN LPCWSTR sInputLine);
	SFE_FORMAT ReadFormat(int nCodePage);
	
	static int GetFileCodePage(SFE_FORMAT nFormat, int nCodePage);
	static void ProcessOpenFlags(UINT& nOpenFlags, SFE_FORMAT nFormat);

	SFE_FORMAT	m_nFormat;
	int			m_nFileCodePage;
	char		m_cUnicodeFillerChar;
	wchar_t		m_arrUnicodeDefaultBuffer[SFE_DEFAULT_BUFFER_SIZE];	// default buffer for strings; used to avoid dynamic allocation where possible
	char		m_arrMultibyteDefaultBuffer[SFE_DEFAULT_BUFFER_SIZE]; // default buffer for strings; used to avoid dynamic allocation where possible
	bool		m_bCheckFilePos;
	UINT		m_nBytesWritten;

private:
	// Additional flag to allow Unicode text writing
	static const UINT modeWriteUnicode;

	// static utility functions

	// --------------------------------------------------------------------------------------------
	//
	//	CStdioFileEx::GetUnicodeStringFromMultiByteString()
	//
	// --------------------------------------------------------------------------------------------
	// Returns:    int - number of chars written (0 means error)
	// Parameters: LPCSTR		szMultiByteString		(IN)	Multi-byte input string
	//					wchar_t*		szUnicodeString		(OUT)	Unicode output string
	//					size_t		nUnicodeBufferSize	(IN)	Size of Unicode output buffer
	//					UINT			nCodePage				(IN)	Code page used to perform conversion
	//																		Default = CP_ACP (Get local code page).
	//
	// Purpose:		Gets a Unicode string from a MultiByte string.
	// Notes:		None.
	// Exceptions:	None.
	//
	static int		GetUnicodeStringFromMultiByteString(IN LPCSTR szMultiByteString, 
														OUT wchar_t* szUnicodeString,
														IN const size_t nUnicodeBufferSize, 
														IN UINT nCodePage);

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
	static int		GetRequiredUnicodeLengthFromMultiByteString(IN LPCSTR szMultiByteString, 
																IN UINT nCodePage);

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
	static int		GetNewUnicodeStringFromMultiByteString(IN LPCSTR szMultiByteString, 
															IN OUT CTemplateSmartPtrArray<wchar_t>& spUnicodeString, 
															IN const int nDefaultBufferSize,
															IN UINT nCodePage);

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
	// Notes:		.
	// Exceptions:	None.
	//
	static int			GetMultiByteStringFromUnicodeString(IN const wchar_t * szUnicodeString, 
															OUT char* szMultiByteString,
															IN const int nMultiByteBufferSize,
															IN UINT nCodePage,
															IN char cFillerChar);


	//---------------------------------------------------------------------------------------------------
	//
	//	CStdioFileEx::GetRequiredMultiByteLengthForUnicodeString()
	//
	//---------------------------------------------------------------------------------------------------
	// Returns:    int - no of bytes required
	// Parameters: IN const wchar_t * szUnicodeString,int nCodePage=-1, char cFillerChar='#'
	//
	// Purpose:		Obtains the multi-byte buffer size needed to accommodate a converted Unicode string.
	//	Notes:		We can't assume that the buffer length is simply equal to the number of characters
	//					because that wouldn't accommodate multibyte characters!
	//
	static int			GetRequiredMultiByteLengthForUnicodeString(IN const wchar_t * szUnicodeString,
																	IN UINT nCodePage);

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
	static int			GetNewMultiByteStringFromUnicodeString(IN const wchar_t * szUnicodeString,
																IN OUT CTemplateSmartPtrArray<char>& spMultiByteString, 
																IN const int nDefaultBufferSize,
																IN UINT nCodePage,
																IN char cFillerChar);
};

#endif // !defined(AFX_STDIOFILEEX_H__41AFE3CA_25E0_482F_8B00_C40775BCDB81__INCLUDED_)

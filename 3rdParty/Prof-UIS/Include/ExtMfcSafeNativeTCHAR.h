// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2004 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// http://www.fossware.com
// mailto:foss@fossware.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#if (!defined __ExtMfcSafeNativeTCHAR_H)
#define __ExtMfcSafeNativeTCHAR_H

//
//
//
// Prof-UIS native unicode character type support for VC++ .NET 2002 or later
//
//
//

// enable template-based chars for unicode builds under Visual C++ .NET
// (if commented then automatic conversion between "unsigned short" and
// native "__wchar_t" is off)
//
// #define __EXT_MFC_ENABLE_TEMPLATED_CHARS
//

#if _MFC_VER >= 0x700
	#if (defined _UNICODE) && (defined _WCHAR_T_DEFINED)
		#if (defined __EXT_MFC_ENABLE_TEMPLATED_CHARS)
			// this should be configured by user
			// (determines whether Prof-UIS/UNICODE uses
			// native "__wchar_t" or not)
			// #define __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T

			// signal we are using extra-safe and compatible chars
			#define __EXT_MFC_TEMPLATED_CHARS_IMPLEMENTED

			#if (defined _NATIVE_WCHAR_T_DEFINED)
				#pragma message("   Prof-UIS native unicode character type support:")
				#pragma message("      adding automatic conversion from unsigned short to __wchar_t")
				// if we compiling the Prof-UIS library or client project
				// withing native unicode character type,
				// then we need to include additonal exports for the clients
				// without _NATIVE_WCHAR_T_DEFINED
				#if (defined __PROF_UIS_IMPL__)
					#if (!defined __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T)
						#error Prof-UIS native unicode character type support: the __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T symbol should be definded to provide client programs about valid Prof-UIS types
					#endif // (!defined __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T)
					#pragma message("   Prof-UIS real wchar_t type is:")
					#pragma message("      native __wchar_t")
				#endif // (defined __PROF_UIS_IMPL__)
			#else  // if - (defined _NATIVE_WCHAR_T_DEFINED)
				#pragma message("   Prof-UIS native unicode character type support:")
				#pragma message("      adding automatic conversion from __wchar_t to unsigned short")
				// if we compiling the Prof-UIS library  or client project
				// without native unicode character type, then we need
				// to include additonal exports for the clients
				// which need _NATIVE_WCHAR_T_DEFINED
				#if (defined __PROF_UIS_IMPL__)
					#if (defined __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T)
						#error Prof-UIS native unicode character type support: the __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T symbol should NOT be definded to provide client programs about valid Prof-UIS types
					#endif // (defined __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T)
					#pragma message("   Prof-UIS real wchar_t type is:")
					#pragma message("      unsigned short")
				#endif // (defined __PROF_UIS_IMPL__)
			#endif // if not - (defined _NATIVE_WCHAR_T_DEFINED)

			#if (!defined __PROF_UIS_IMPL__)
				#if (defined __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T)
					#pragma message("   Prof-UIS real wchar_t type is:")
					#pragma message("      native __wchar_t")
				#else // if - (defined __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T)
					#pragma message("   Prof-UIS real wchar_t type is:")
					#pragma message("      unsigned short")
				#endif // if not - (defined __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T)
				#if (defined _NATIVE_WCHAR_T_DEFINED)
					#pragma message("   Client project real wchar_t type is:")
					#pragma message("      native __wchar_t")
					#if (defined __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T)
						#pragma message("      (Prof-UIS native wchar_t conversion system is not needed)")
					#endif // (defined __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T)
				#else  // if - (defined _NATIVE_WCHAR_T_DEFINED)
					#pragma message("   Client project real wchar_t type is:")
					#pragma message("      unsigned short")
					#if (!defined __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T)
						#pragma message("      (Prof-UIS native wchar_t conversion system is not needed)")
					#endif // (!defined __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T)
				#endif // if not - (defined _NATIVE_WCHAR_T_DEFINED)
			#endif // (!defined __PROF_UIS_IMPL__)

			#if (defined __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T)
				typedef __wchar_t __prof_uis_used_wchar_t;
				typedef unsigned short __prof_uis_converted_wchar_t;
			#else // if - (defined __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T)
				typedef unsigned short __prof_uis_used_wchar_t;
				typedef __wchar_t __prof_uis_converted_wchar_t;
			#endif // if - not (defined __EXT_MFC_COMPILED_WITH_NATIVE_WCHAR_T)

			template < typename BaseType = char >
			class CExtSafeChTraitsBase
			{
			public:
				typedef char XCHAR;
				typedef char * PXSTR;
				typedef const char * PCXSTR;
				typedef __wchar_t YCHAR;
				typedef __wchar_t * PYSTR;
				typedef const __wchar_t * PCYSTR;
			};

			template<>
			class CExtSafeChTraitsBase< __wchar_t >
			{
			public:
				typedef __wchar_t XCHAR;
				typedef __wchar_t * PXSTR;
				typedef const __wchar_t * PCXSTR;
				typedef char YCHAR;
				typedef char * PYSTR;
				typedef const char * PCYSTR;
			};
			template<>
			class CExtSafeChTraitsBase< unsigned short >
			{
			public:
				typedef unsigned short XCHAR;
				typedef unsigned short * PXSTR;
				typedef const unsigned short * PCXSTR;
				typedef char YCHAR;
				typedef char * PYSTR;
				typedef const char * PCYSTR;
			};

			template < typename _CharType = char >
			class CExtSafeChTraitsCRT : public CExtSafeChTraitsBase < _CharType >
			{
			public:
				static char * CharNext( const char * p ) throw()
				{
					return
						reinterpret_cast
							< char * >
								(
									_mbsinc(
										reinterpret_cast
											< const unsigned char * >
												( p )
										)
								);
				}
				static int IsDigit( char ch ) throw()
				{
					return _ismbcdigit( ch );
				}
				static int IsSpace( char ch ) throw()
				{
					return _ismbcspace( ch );
				}
				static int StringCompare( LPCSTR pszA, LPCSTR pszB ) throw()
				{
					return
						_mbscmp(
							reinterpret_cast < const unsigned char * > ( pszA ),
							reinterpret_cast < const unsigned char * > ( pszB )
							);
				}
				static int StringCompareIgnore( LPCSTR pszA, LPCSTR pszB ) throw()
				{
					return
						_mbsicmp(
							reinterpret_cast < const unsigned char * > ( pszA ),
							reinterpret_cast < const unsigned char * > ( pszB )
							);
				}
				static int StringCollate( LPCSTR pszA, LPCSTR pszB ) throw()
				{
					return
						_mbscoll(
							reinterpret_cast < const unsigned char * > ( pszA ),
							reinterpret_cast < const unsigned char * > ( pszB )
							);
				}
				static int StringCollateIgnore( LPCSTR pszA, LPCSTR pszB ) throw()
				{
					return
						_mbsicoll(
							reinterpret_cast < const unsigned char * > ( pszA ),
							reinterpret_cast < const unsigned char * > ( pszB )
							);
				}
				static LPCSTR StringFindString( LPCSTR pszBlock, LPCSTR pszMatch ) throw()
				{
					return
						reinterpret_cast < LPCSTR >
							(
								_mbsstr(
									reinterpret_cast < const unsigned char * > ( pszBlock ),
									reinterpret_cast < const unsigned char * > ( pszMatch )
									)
							);
				}
				static LPSTR StringFindString( LPSTR pszBlock, LPCSTR pszMatch ) throw()
				{
					return
						(
							const_cast < LPSTR >
								(
									StringFindString(
										const_cast < LPCSTR > ( pszBlock ),
										pszMatch
										)
								)
						);
				}
				static LPCSTR StringFindChar( LPCSTR pszBlock, char chMatch ) throw()
				{
					return
						reinterpret_cast < LPCSTR >
							(
								_mbschr(
									reinterpret_cast < const unsigned char * > ( pszBlock ),
									chMatch
									)
							);
				}
				static LPCSTR StringFindCharRev( LPCSTR psz, char ch ) throw()
				{
					return
						reinterpret_cast < LPSTR >
							(
								_mbsrchr(
									reinterpret_cast < const unsigned char * > ( psz ),
									ch
									)
							);
				}
				static LPCSTR StringScanSet( LPCSTR pszBlock, LPCSTR pszMatch ) throw()
				{
					return
						reinterpret_cast < LPCSTR >
							(
								_mbspbrk(
									reinterpret_cast < const unsigned char * > ( pszBlock ),
									reinterpret_cast < const unsigned char * > ( pszMatch )
									)
							);
				}
				static int StringSpanIncluding( LPCSTR pszBlock, LPCSTR pszSet ) throw()
				{
					return
						(int) _mbsspn(
							reinterpret_cast < const unsigned char * > ( pszBlock ),
							reinterpret_cast < const unsigned char * >( pszSet )
							);
				}
				static int StringSpanExcluding( LPCSTR pszBlock, LPCSTR pszSet ) throw()
				{
					return
						(int) _mbscspn(
							reinterpret_cast < const unsigned char * > ( pszBlock ),
							reinterpret_cast < const unsigned char * > ( pszSet )
							);
				}
				static LPSTR StringUppercase( LPSTR psz ) throw()
				{
					return
						reinterpret_cast < LPSTR >
							(
								_mbsupr(
									reinterpret_cast < unsigned char * > ( psz )
									)
							);
				}
				static LPSTR StringLowercase( LPSTR psz ) throw()
				{
					return
						reinterpret_cast < LPSTR >
							(
								_mbslwr(
									reinterpret_cast < unsigned char * > ( psz )
									)
							);
				}
				static LPSTR StringReverse( LPSTR psz ) throw()
				{
					return
						reinterpret_cast < LPSTR >
							(
								_mbsrev(
									reinterpret_cast < unsigned char * > ( psz )
									)
							);
				}
				static int GetFormattedLength( LPCSTR pszFormat, va_list args ) throw()
				{
					return _vscprintf( pszFormat, args );
				}
				static int Format( LPSTR pszBuffer, LPCSTR pszFormat, va_list args ) throw()
				{
					return vsprintf( pszBuffer, pszFormat, args );
				}
				static int GetBaseTypeLength( LPCSTR pszSrc ) throw()
				{
					return int( strlen( pszSrc ) );
				}
				static int GetBaseTypeLength( LPCSTR pszSrc, int nLength ) throw()
				{
					(void)pszSrc;
					return nLength;
				}
				static int GetBaseTypeLength( LPCWSTR pszSource ) throw()
				{
					return
						::WideCharToMultiByte(
							_AtlGetConversionACP(),
							0,
							pszSource,
							-1,
							NULL,
							0,
							NULL,
							NULL
							) - 1;
				}
				static int GetBaseTypeLength( LPCWSTR pszSource, int nLength ) throw()
				{
					return
						::WideCharToMultiByte(
							_AtlGetConversionACP(),
							0,
							pszSource,
							nLength,
							NULL,
							0,
							NULL,
							NULL
							);
				}
				static void ConvertToBaseType(
					LPSTR pszDest,
					int nDestLength,
					LPCSTR pszSrc,
					int nSrcLength = -1
					) throw()
				{
					(void)nSrcLength;
					memcpy( pszDest, pszSrc, nDestLength * sizeof(char) );
				}
				static void ConvertToBaseType(
					LPSTR pszDest,
					int nDestLength,
					LPCWSTR pszSrc,
					int nSrcLength = -1
					) throw()
				{
					::WideCharToMultiByte(
						_AtlGetConversionACP(),
						0,
						pszSrc,
						nSrcLength,
						pszDest,
						nDestLength,
						NULL,
						NULL
						);
				}
				static void ConvertToOem( LPSTR psz ) throw()
				{
					::AnsiToOem( psz, psz );
				}
				static void ConvertToAnsi( LPSTR psz ) throw()
				{
					::OemToAnsi( psz, psz );
				}
				static void FloodCharacters( char ch, int nLength, char * pch ) throw()
				{
					memset( pch, ch, nLength );
				}
				static BSTR AllocSysString( const char * pchData, int nDataLength ) throw()
				{
					int nLen =
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pchData,
							nDataLength,
							NULL,
							NULL
							);
					BSTR bstr = ::SysAllocStringLen( NULL, nLen );
					if( bstr != NULL )
					{
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pchData,
							nDataLength,
							bstr,
							nLen
							);
					}
					return bstr;
				}
				static BOOL ReAllocSysString( const char * pchData, BSTR* pbstr, int nDataLength ) throw()
				{
					int nLen =
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pchData,
							nDataLength,
							NULL,
							NULL
							);
					BOOL bSuccess = ::SysReAllocStringLen( pbstr, NULL, nLen );
					if( bSuccess )
					{
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pchData,
							nDataLength,
							*pbstr,
							nLen
							);
					}
					return bSuccess;
				}
				static DWORD FormatMessage(
					DWORD dwFlags,
					LPCVOID pSource,
					DWORD dwMessageID,
					DWORD dwLanguageID,
					LPSTR pszBuffer,
					DWORD nSize,
					va_list * pArguments
					) throw()
				{
					return
						::FormatMessageA(
							dwFlags,
							pSource,
							dwMessageID,
							dwLanguageID,
							pszBuffer,
							nSize,
							pArguments
							);
				}
				static int SafeStringLen( LPCSTR psz ) throw()
				{
					return (psz != NULL) ? int( strlen( psz ) ) : 0;
				}
				static int SafeStringLen( LPCWSTR psz ) throw()
				{
					return (psz != NULL) ? int( wcslen( psz ) ) : 0;
				}
				static int GetCharLen( const __wchar_t * pch ) throw()
				{
					(void)pch;
					return 1;
				}
				static int GetCharLen( const char* pch ) throw()
				{
					return int(
						_mbclen(
							reinterpret_cast < const unsigned char * > ( pch )
							)
						);
				}
				static DWORD GetEnvironmentVariable(
					LPCSTR pszVar,
					LPSTR pszBuffer,
					DWORD dwSize
					) throw()
				{
					return ::GetEnvironmentVariableA( pszVar, pszBuffer, dwSize );
				}
			};

			template<>
			class CExtSafeChTraitsCRT < __wchar_t > : public ChTraitsBase < __wchar_t >
			{
#if defined(_UNICODE) && !defined(_CSTRING_ALWAYS_THUNK)
				static DWORD _GetEnvironmentVariableW(
					const __wchar_t * pszName,
					__wchar_t * pszBuffer,
					DWORD nSize
					) throw()
				{
					return
						::GetEnvironmentVariableW(
							(LPCWSTR)pszName,
							(LPWSTR)pszBuffer,
							nSize
							);
				}
#else
				static DWORD WINAPI _GetEnvironmentVariableW(
					const __wchar_t * pszName, 
					__wchar_t * pszBuffer,
					DWORD nSize
					) throw()
				{
					return
						_strthunks.pfnGetEnvironmentVariableW(
							(LPCWSTR)pszName,
							(LPWSTR)pszBuffer,
							nSize
							);
				}
#endif
			public:
				static __wchar_t * CharNext( const __wchar_t * psz ) throw()
				{
					return const_cast< __wchar_t * >( psz+1 );
				}
				static int IsDigit( __wchar_t ch ) throw()
				{
					return iswdigit( ch );
				}
				static int IsSpace( __wchar_t ch ) throw()
				{
					return iswspace( ch );
				}
				static int StringCompare( const __wchar_t * pszA, const __wchar_t * pszB ) throw()
				{
					return wcscmp( (wchar_t *) pszA, (wchar_t *) pszB );
				}
				static int StringCompareIgnore( const __wchar_t * pszA, const __wchar_t * pszB ) throw()
				{
					return _wcsicmp( (wchar_t *)pszA, (wchar_t *)pszB );
				}
				static int StringCollate( const __wchar_t * pszA, const __wchar_t * pszB ) throw()
				{
					return wcscoll( (wchar_t *)pszA, (wchar_t *)pszB );
				}
				static int StringCollateIgnore( const __wchar_t * pszA, const __wchar_t * pszB ) throw()
				{
					return _wcsicoll( (wchar_t *)pszA, (wchar_t *)pszB );
				}
				static const __wchar_t * StringFindString( const __wchar_t * pszBlock, const __wchar_t * pszMatch ) throw()
				{
					return (__wchar_t *)wcsstr( (wchar_t *)pszBlock, (wchar_t *)pszMatch );
				}
				static __wchar_t * StringFindString( __wchar_t * pszBlock, const __wchar_t * pszMatch ) throw()
				{
					return ( const_cast< __wchar_t * >( StringFindString( const_cast< const __wchar_t * >( pszBlock ), pszMatch ) ) );
				}
				static const __wchar_t * StringFindChar( const __wchar_t * pszBlock, __wchar_t chMatch ) throw()
				{
					return (__wchar_t *)wcschr( (wchar_t *)pszBlock, (wchar_t)chMatch );
				}
				static const __wchar_t * StringFindCharRev( const __wchar_t * psz, __wchar_t ch ) throw()
				{
					return (__wchar_t *)wcsrchr( (wchar_t *)psz, (wchar_t)ch );
				}
				static const __wchar_t * StringScanSet( const __wchar_t * pszBlock, const __wchar_t * pszMatch ) throw()
				{
					return (__wchar_t *)wcspbrk( (wchar_t *)pszBlock, (wchar_t *)pszMatch );
				}
				static int StringSpanIncluding( const __wchar_t * pszBlock, const __wchar_t * pszSet ) throw()
				{
					return (int)wcsspn( (wchar_t *)pszBlock, (wchar_t *)pszSet );
				}
				static int StringSpanExcluding( const __wchar_t * pszBlock, const __wchar_t * pszSet ) throw()
				{
					return (int)wcscspn( (wchar_t *)pszBlock, (wchar_t *)pszSet );
				}
				static __wchar_t * StringUppercase( __wchar_t * psz ) throw()
				{
					return (__wchar_t *)_wcsupr( (wchar_t *)psz );
				}
				static __wchar_t * StringLowercase( __wchar_t * psz ) throw()
				{
					return (__wchar_t *)_wcslwr( (wchar_t *)psz );
				}
				static __wchar_t * StringReverse( __wchar_t * psz ) throw()
				{
					return (__wchar_t *)_wcsrev( (wchar_t *)psz );
				}
				static int GetFormattedLength( const __wchar_t * pszFormat, va_list args) throw()
				{
					return _vscwprintf( (wchar_t *)pszFormat, args );
				}
				static int Format( __wchar_t * pszBuffer, const __wchar_t * pszFormat, va_list args) throw()
				{
					return vswprintf( (wchar_t *)pszBuffer, (wchar_t *)pszFormat, args );
				}
				static int GetBaseTypeLength( LPCSTR pszSrc ) throw()
				{
					return
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pszSrc,
							-1,
							NULL,
							0
							) - 1;
				}
				static int GetBaseTypeLength( LPCSTR pszSrc, int nLength ) throw()
				{
					return
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pszSrc,
							nLength,
							NULL,
							0
							);
				}
				static int GetBaseTypeLength( const __wchar_t * pszSrc ) throw()
				{
					return (int)wcslen( (wchar_t *)pszSrc );
				}
				static int GetBaseTypeLength( const __wchar_t * pszSrc, int nLength ) throw()
				{
					(void)pszSrc;
					return nLength;
				}
				static void ConvertToBaseType(
					__wchar_t * pszDest,
					int nDestLength,
					LPCSTR pszSrc,
					int nSrcLength = -1
					) throw()
				{
					::MultiByteToWideChar(
						_AtlGetConversionACP(),
						0,
						pszSrc,
						nSrcLength,
						(LPWSTR)pszDest,
						nDestLength
						);
				}
				static void ConvertToBaseType(
					__wchar_t * pszDest,
					int nDestLength,
					const __wchar_t * pszSrc,
					int nSrcLength = -1
					) throw()
				{
					(void)nSrcLength;
					memcpy( pszDest, pszSrc, nDestLength * sizeof(__wchar_t) );
				}
				static void FloodCharacters( __wchar_t ch, int nLength, __wchar_t * psz ) throw()
				{
					for( int i = 0; i < nLength; i++ )
					{
						psz[i] = ch;
					}
				}
				static BSTR AllocSysString( const __wchar_t * pchData, int nDataLength ) throw()
				{
					return ::SysAllocStringLen( (const OLECHAR *)pchData, nDataLength );
				}

				static BOOL ReAllocSysString( const __wchar_t * pchData, BSTR* pbstr, int nDataLength ) throw()
				{
					return ::SysReAllocStringLen( pbstr, (const OLECHAR *)pchData, nDataLength );
				}
#ifdef _UNICODE
				static DWORD FormatMessage(
					DWORD dwFlags,
					LPCVOID pSource,
					DWORD dwMessageID,
					DWORD dwLanguageID,
					__wchar_t * pszBuffer,
					DWORD nSize,
					va_list * pArguments
					) throw()
				{
					return
						::FormatMessageW(
							dwFlags,
							pSource,
							dwMessageID,
							dwLanguageID,
							(LPWSTR)pszBuffer,
							nSize,
							pArguments
							);
				}
#endif
				static int SafeStringLen( LPCSTR psz ) throw()
				{
					return (psz != NULL) ? (int)strlen( psz ) : 0;
				}
				static int SafeStringLen( const __wchar_t * psz ) throw()
				{
					return (psz != NULL) ? (int)wcslen( (wchar_t *)psz ) : 0;
				}
				static int GetCharLen( const __wchar_t * pch ) throw()
				{
					(void)pch;
					return 1;
				}
				static int GetCharLen( const char * pch ) throw()
				{
					return (int)(
						_mbclen(
							reinterpret_cast < const unsigned char * > ( pch )
							)
						);
				}
				static DWORD GetEnvironmentVariable( const __wchar_t * pszVar, __wchar_t * pszBuffer, DWORD dwSize ) throw()
				{
					return _GetEnvironmentVariableW( pszVar, pszBuffer, dwSize );
				}
			};

			template<>
			class CExtSafeChTraitsCRT < unsigned short > : public ChTraitsBase < unsigned short >
			{
#if defined(_UNICODE) && !defined(_CSTRING_ALWAYS_THUNK)
				static DWORD _GetEnvironmentVariableW(
					const unsigned short * pszName,
					unsigned short * pszBuffer,
					DWORD nSize
					) throw()
				{
					return
						::GetEnvironmentVariableW(
							(LPCWSTR)pszName,
							(LPWSTR)pszBuffer,
							nSize
							);
				}
#else
				static DWORD WINAPI _GetEnvironmentVariableW(
					const unsigned short * pszName, 
					unsigned short * pszBuffer,
					DWORD nSize
					) throw()
				{
					return
						_strthunks.pfnGetEnvironmentVariableW(
							(LPCWSTR)pszName,
							(LPWSTR)pszBuffer,
							nSize
							);
				}
#endif
			public:
				static unsigned short * CharNext( const unsigned short * psz ) throw()
				{
					return const_cast< unsigned short * >( psz+1 );
				}
				static int IsDigit( unsigned short ch ) throw()
				{
					return iswdigit( ch );
				}
				static int IsSpace( unsigned short ch ) throw()
				{
					return iswspace( ch );
				}
				static int StringCompare( const unsigned short * pszA, const unsigned short * pszB ) throw()
				{
					return wcscmp( (wchar_t *)pszA, (wchar_t *)pszB );
				}
				static int StringCompareIgnore( const unsigned short * pszA, const unsigned short * pszB ) throw()
				{
					return _wcsicmp( (wchar_t *)pszA, (wchar_t *)pszB );
				}
				static int StringCollate( const unsigned short * pszA, const unsigned short * pszB ) throw()
				{
					return wcscoll( (wchar_t *)pszA, (wchar_t *)pszB );
				}
				static int StringCollateIgnore( const unsigned short * pszA, const unsigned short * pszB ) throw()
				{
					return _wcsicoll( (wchar_t *)pszA, (wchar_t *)pszB );
				}
				static const unsigned short * StringFindString( const unsigned short * pszBlock, const unsigned short * pszMatch ) throw()
				{
					return (unsigned short *)wcsstr( (wchar_t *)pszBlock, (wchar_t *)pszMatch );
				}
				static unsigned short * StringFindString( unsigned short * pszBlock, const unsigned short * pszMatch ) throw()
				{
					return ( const_cast< unsigned short * >( StringFindString( const_cast< const unsigned short * >( pszBlock ), pszMatch ) ) );
				}
				static const unsigned short * StringFindChar( const unsigned short * pszBlock, unsigned short chMatch ) throw()
				{
					return (unsigned short *)wcschr( (wchar_t *)pszBlock, (wchar_t)chMatch );
				}
				static const unsigned short * StringFindCharRev( const unsigned short * psz, unsigned short ch ) throw()
				{
					return (unsigned short *)wcsrchr( (wchar_t *)psz, (wchar_t)ch );
				}
				static const unsigned short * StringScanSet( const unsigned short * pszBlock, const unsigned short * pszMatch ) throw()
				{
					return (unsigned short *)wcspbrk( (wchar_t *)pszBlock, (wchar_t *)pszMatch );
				}
				static int StringSpanIncluding( const unsigned short * pszBlock, const unsigned short * pszSet ) throw()
				{
					return (int)wcsspn( (wchar_t *)pszBlock, (wchar_t *)pszSet );
				}
				static int StringSpanExcluding( const unsigned short * pszBlock, const unsigned short * pszSet ) throw()
				{
					return (int)wcscspn( (wchar_t *)pszBlock, (wchar_t *)pszSet );
				}
				static unsigned short * StringUppercase( unsigned short * psz ) throw()
				{
					return (unsigned short *)_wcsupr( (wchar_t *)psz );
				}
				static unsigned short * StringLowercase( unsigned short * psz ) throw()
				{
					return (unsigned short *)_wcslwr( (wchar_t *)psz );
				}
				static unsigned short * StringReverse( unsigned short * psz ) throw()
				{
					return (unsigned short *)_wcsrev( (wchar_t *)psz );
				}
				static int GetFormattedLength( const unsigned short * pszFormat, va_list args) throw()
				{
					return _vscwprintf( (wchar_t *)pszFormat, args );
				}
				static int Format( unsigned short * pszBuffer, const unsigned short * pszFormat, va_list args) throw()
				{
					return vswprintf( (wchar_t *)pszBuffer, (wchar_t *)pszFormat, args );
				}
				static int GetBaseTypeLength( LPCSTR pszSrc ) throw()
				{
					return
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pszSrc,
							-1,
							NULL,
							0
							) - 1;
				}
				static int GetBaseTypeLength( LPCSTR pszSrc, int nLength ) throw()
				{
					return
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pszSrc,
							nLength,
							NULL,
							0
							);
				}
				static int GetBaseTypeLength( const unsigned short * pszSrc ) throw()
				{
					return (int)wcslen( (wchar_t *)pszSrc );
				}
				static int GetBaseTypeLength( const unsigned short * pszSrc, int nLength ) throw()
				{
					(void)pszSrc;
					return nLength;
				}
				static void ConvertToBaseType(
					unsigned short * pszDest,
					int nDestLength,
					LPCSTR pszSrc,
					int nSrcLength = -1
					) throw()
				{
					::MultiByteToWideChar(
						_AtlGetConversionACP(),
						0,
						pszSrc,
						nSrcLength,
						(LPWSTR)pszDest,
						nDestLength
						);
				}
				static void ConvertToBaseType(
					unsigned short * pszDest,
					int nDestLength,
					const unsigned short * pszSrc,
					int nSrcLength = -1
					) throw()
				{
					(void)nSrcLength;
					memcpy( pszDest, pszSrc, nDestLength * sizeof(unsigned short) );
				}
				static void FloodCharacters( unsigned short ch, int nLength, unsigned short * psz ) throw()
				{
					for( int i = 0; i < nLength; i++ )
					{
						psz[i] = ch;
					}
				}
				static BSTR AllocSysString( const unsigned short* pchData, int nDataLength ) throw()
				{
					return ::SysAllocStringLen( (const OLECHAR *)pchData, nDataLength );
				}
				static BOOL ReAllocSysString( const unsigned short* pchData, BSTR* pbstr, int nDataLength ) throw()
				{
					return ::SysReAllocStringLen( pbstr, (const OLECHAR *)pchData, nDataLength );
				}
#ifdef _UNICODE
				static DWORD FormatMessage(
					DWORD dwFlags,
					LPCVOID pSource,
					DWORD dwMessageID,
					DWORD dwLanguageID,
					unsigned short * pszBuffer,
					DWORD nSize,
					va_list * pArguments
					) throw()
				{
					return
						::FormatMessageW(
							dwFlags,
							pSource,
							dwMessageID,
							dwLanguageID,
							(LPWSTR)pszBuffer,
							nSize,
							pArguments
							);
				}
#endif
				static int SafeStringLen( LPCSTR psz ) throw()
				{
					return (psz != NULL) ? (int)strlen( psz ) : 0;
				}
				static int SafeStringLen( const unsigned short * psz ) throw()
				{
					return (psz != NULL) ? (int)wcslen( (wchar_t *)psz ) : 0;
				}
				static int GetCharLen( const unsigned short * pch ) throw()
				{
					(void)pch;
					return 1;
				}
				static int GetCharLen( const char * pch ) throw()
				{
					return (int)(
						_mbclen(
							reinterpret_cast < const unsigned char * > ( pch )
							)
						);
				}
				static DWORD GetEnvironmentVariable(
					const unsigned short * pszVar,
					unsigned short * pszBuffer,
					DWORD dwSize
					) throw()
				{
					return _GetEnvironmentVariableW( pszVar, pszBuffer, dwSize );
				}
			};

			template <
				typename _CharType = char,
				class StringIterator = CExtSafeChTraitsCRT < char >
				>
			class CExtSafeStrTraitMFC : public StringIterator
			{
			public:
				static HINSTANCE FindStringResourceInstance( UINT nID ) throw()
				{
					return ( AfxFindStringResourceHandle( nID ) );
				}
				static ATL::IAtlStringMgr * GetDefaultManager() throw()
				{
					return ( AfxGetStringManager() );
				}
				static char* CharNext( const char * p ) throw()
				{
					return reinterpret_cast< char* >( _mbsinc( reinterpret_cast< const unsigned char* >( p ) ) );
				}
				static int IsDigit( char ch ) throw()
				{
					return _ismbcdigit( ch );
				}
				static int IsSpace( char ch ) throw()
				{
					return _ismbcspace( ch );
				}
				static int StringCompare( LPCSTR pszA, LPCSTR pszB ) throw()
				{
					return
						_mbscmp(
							reinterpret_cast < const unsigned char * > ( pszA ),
							reinterpret_cast < const unsigned char * > ( pszB )
							);
				}
				static int StringCompareIgnore( LPCSTR pszA, LPCSTR pszB ) throw()
				{
					return
						_mbsicmp(
							reinterpret_cast < const unsigned char * > ( pszA ),
							reinterpret_cast < const unsigned char * > ( pszB )
							);
				}
				static int StringCollate( LPCSTR pszA, LPCSTR pszB ) throw()
				{
					return
						_mbscoll(
							reinterpret_cast < const unsigned char * > ( pszA ),
							reinterpret_cast < const unsigned char * > ( pszB )
							);
				}
				static int StringCollateIgnore( LPCSTR pszA, LPCSTR pszB ) throw()
				{
					return
						_mbsicoll(
							reinterpret_cast < const unsigned char * > ( pszA ),
							reinterpret_cast < const unsigned char * > ( pszB )
							);
				}
				static LPCSTR StringFindString( LPCSTR pszBlock, LPCSTR pszMatch ) throw()
				{
					return reinterpret_cast < LPCSTR > (
						_mbsstr(
							reinterpret_cast< const unsigned char * > ( pszBlock ),
							reinterpret_cast< const unsigned char * > ( pszMatch )
							)
						);
				}
				static LPSTR StringFindString( LPSTR pszBlock, LPCSTR pszMatch ) throw()
				{
					return ( const_cast< LPSTR >
							(
								StringFindString(
									const_cast< LPCSTR >( pszBlock ),
									pszMatch
									)
							)
						);
				}
				static LPCSTR StringFindChar( LPCSTR pszBlock, char chMatch ) throw()
				{
					return reinterpret_cast < LPCSTR > (
						_mbschr(
							reinterpret_cast< const unsigned char* >( pszBlock ),
							chMatch
							)
						);
				}
				static LPCSTR StringFindCharRev( LPCSTR psz, char ch ) throw()
				{
					return reinterpret_cast < LPSTR > (
						_mbsrchr(
							reinterpret_cast< const unsigned char* >( psz ),
							ch
							)
						);
				}
				static LPCSTR StringScanSet( LPCSTR pszBlock, LPCSTR pszMatch ) throw()
				{
					return reinterpret_cast < LPCSTR > (
						_mbspbrk(
							reinterpret_cast < const unsigned char * > ( pszBlock ),
							reinterpret_cast < const unsigned char * > ( pszMatch )
							)
						);
				}
				static int StringSpanIncluding( LPCSTR pszBlock, LPCSTR pszSet ) throw()
				{
					return
						(int) _mbsspn(
							reinterpret_cast < const unsigned char * > ( pszBlock ),
							reinterpret_cast < const unsigned char * > ( pszSet )
							);
				}
				static int StringSpanExcluding( LPCSTR pszBlock, LPCSTR pszSet ) throw()
				{
					return
						(int) _mbscspn(
							reinterpret_cast < const unsigned char * > ( pszBlock ),
							reinterpret_cast < const unsigned char * > ( pszSet )
							);
				}
				static LPSTR StringUppercase( LPSTR psz ) throw()
				{
					return reinterpret_cast < LPSTR > (
						_mbsupr(
							reinterpret_cast < unsigned char * > ( psz )
							)
						);
				}
				static LPSTR StringLowercase( LPSTR psz ) throw()
				{
					return reinterpret_cast < LPSTR > (
						_mbslwr(
							reinterpret_cast < unsigned char * > ( psz )
							)
						);
				}
				static LPSTR StringReverse( LPSTR psz ) throw()
				{
					return reinterpret_cast < LPSTR > (
						_mbsrev(
							reinterpret_cast < unsigned char * > ( psz )
							)
						);
				}
				static int GetFormattedLength( LPCSTR pszFormat, va_list args ) throw()
				{
					return _vscprintf( pszFormat, args );
				}
				static int Format( LPSTR pszBuffer, LPCSTR pszFormat, va_list args ) throw()
				{
					return vsprintf( pszBuffer, pszFormat, args );
				}
				static int GetBaseTypeLength( LPCSTR pszSrc ) throw()
				{
					return int( strlen( pszSrc ) );
				}
				static int GetBaseTypeLength( LPCSTR pszSrc, int nLength ) throw()
				{
					(void)pszSrc;
					return nLength;
				}
				static int GetBaseTypeLength( LPCWSTR pszSource ) throw()
				{
					return ::WideCharToMultiByte( _AtlGetConversionACP(), 0, pszSource, -1, NULL, 0, NULL, NULL )-1;
				}
				static int GetBaseTypeLength( LPCWSTR pszSource, int nLength ) throw()
				{
					return
						::WideCharToMultiByte(
							_AtlGetConversionACP(),
							0,
							pszSource,
							nLength,
							NULL,
							0,
							NULL,
							NULL
							);
				}
				static void
					ConvertToBaseType(
						LPSTR pszDest,
						int nDestLength,
						LPCSTR pszSrc,
						int nSrcLength = -1
						) throw()
				{
					(void)nSrcLength;
					memcpy( pszDest, pszSrc, nDestLength * sizeof(char) );
				}
				static void ConvertToBaseType(
					LPSTR pszDest,
					int nDestLength,
					LPCWSTR pszSrc,
					int nSrcLength = -1
					) throw()
				{
					::WideCharToMultiByte(
						_AtlGetConversionACP(),
						0,
						pszSrc,
						nSrcLength,
						pszDest,
						nDestLength,
						NULL,
						NULL
						);
				}
				static void ConvertToOem( LPSTR psz ) throw()
				{
					::AnsiToOem( psz, psz );
				}
				static void ConvertToAnsi( LPSTR psz ) throw()
				{
					::OemToAnsi( psz, psz );
				}
				static void FloodCharacters( char ch, int nLength, char* pch ) throw()
				{
					memset( pch, ch, nLength );
				}
				static BSTR AllocSysString( const char* pchData, int nDataLength ) throw()
				{
					int nLen =
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pchData,
							nDataLength,
							NULL,
							NULL
							);
					BSTR bstr = ::SysAllocStringLen( NULL, nLen );
					if( bstr != NULL )
					{
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pchData,
							nDataLength,
							bstr,
							nLen
							);
					}

					return bstr;
				}
				static BOOL ReAllocSysString( const char* pchData, BSTR* pbstr, int nDataLength ) throw()
				{
					int nLen =
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pchData,
							nDataLength,
							NULL,
							NULL
							);
					BOOL bSuccess = ::SysReAllocStringLen( pbstr, NULL, nLen );
					if( bSuccess )
					{
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pchData,
							nDataLength,
							*pbstr,
							nLen
							);
					}
					return bSuccess;
				}
				static DWORD FormatMessage(
					DWORD dwFlags,
					LPCVOID pSource,
					DWORD dwMessageID,
					DWORD dwLanguageID,
					LPSTR pszBuffer,
					DWORD nSize,
					va_list * pArguments
					) throw()
				{
					return
						::FormatMessageA(
							dwFlags,
							pSource,
							dwMessageID,
							dwLanguageID,
							pszBuffer,
							nSize,
							pArguments
							);
				}
				static int SafeStringLen( LPCSTR psz ) throw()
				{
					return (psz != NULL) ? int( strlen( psz ) ) : 0;
				}
				static int SafeStringLen( LPCWSTR psz ) throw()
				{
					return (psz != NULL) ? int( wcslen( psz ) ) : 0;
				}
				static int GetCharLen( const __wchar_t* pch ) throw()
				{
					(void)pch;
					return 1;
				}
				static int GetCharLen( const char* pch ) throw()
				{
					return int(
						_mbclen(
							reinterpret_cast < const unsigned char * > ( pch )
							)
						);
				}
				static DWORD GetEnvironmentVariable(
					LPCSTR pszVar,
					LPSTR pszBuffer,
					DWORD dwSize
					) throw()
				{
					return ::GetEnvironmentVariableA( pszVar, pszBuffer, dwSize );
				}
			};

			template<>
			class CExtSafeStrTraitMFC<
					__wchar_t,
					CExtSafeChTraitsCRT < __wchar_t >
				>
			{
#if defined(_UNICODE) && !defined(_CSTRING_ALWAYS_THUNK)
				static DWORD _GetEnvironmentVariableW(
					const __wchar_t * pszName,
					__wchar_t * pszBuffer,
					DWORD nSize
					) throw()
				{
					return
						::GetEnvironmentVariableW(
							(LPCWSTR)pszName,
							(LPWSTR)pszBuffer,
							nSize
							);
				}
#else
				static DWORD WINAPI _GetEnvironmentVariableW(
					const __wchar_t * pszName, 
					__wchar_t * pszBuffer,
					DWORD nSize
					) throw()
				{
					return
						_strthunks.pfnGetEnvironmentVariableW(
							(LPCWSTR)pszName,
							(LPWSTR)pszBuffer,
							nSize
							);
				}
#endif
			public:
				static HINSTANCE FindStringResourceInstance( UINT nID ) throw()
				{
					return ( AfxFindStringResourceHandle( nID ) );
				}
				static ATL::IAtlStringMgr * GetDefaultManager() throw()
				{
					return ( AfxGetStringManager() );
				}
				static __wchar_t * CharNext( const __wchar_t * psz ) throw()
				{
					return const_cast< __wchar_t * >( psz+1 );
				}
				static int IsDigit( __wchar_t ch ) throw()
				{
					return iswdigit( ch );
				}
				static int IsSpace( __wchar_t ch ) throw()
				{
					return iswspace( ch );
				}
				static int StringCompare( const __wchar_t * pszA, const __wchar_t * pszB ) throw()
				{
					return wcscmp( (wchar_t *)pszA, (wchar_t *)pszB );
				}
				static int StringCompareIgnore( const __wchar_t * pszA, const __wchar_t * pszB ) throw()
				{
					return _wcsicmp( (wchar_t *)pszA, (wchar_t *)pszB );
				}
				static int StringCollate( const __wchar_t * pszA, const __wchar_t * pszB ) throw()
				{
					return wcscoll( (wchar_t *)pszA, (wchar_t *)pszB );
				}
				static int StringCollateIgnore( const __wchar_t * pszA, const __wchar_t * pszB ) throw()
				{
					return _wcsicoll( (wchar_t *)pszA, (wchar_t *)pszB );
				}
				static const __wchar_t * StringFindString( const __wchar_t * pszBlock, const __wchar_t * pszMatch ) throw()
				{
					return (__wchar_t *)wcsstr( (wchar_t *)pszBlock, (wchar_t *)pszMatch );
				}
				static __wchar_t * StringFindString( __wchar_t * pszBlock, const __wchar_t * pszMatch ) throw()
				{
					return ( const_cast< __wchar_t * >( StringFindString( const_cast< const __wchar_t * >( pszBlock ), pszMatch ) ) );
				}
				static const __wchar_t * StringFindChar( const __wchar_t * pszBlock, __wchar_t chMatch ) throw()
				{
					return (__wchar_t *)wcschr( (wchar_t *)pszBlock, (wchar_t)chMatch );
				}
				static const __wchar_t * StringFindCharRev( const __wchar_t * psz, __wchar_t ch ) throw()
				{
					return (__wchar_t *)wcsrchr( (wchar_t *)psz, (wchar_t)ch );
				}
				static const __wchar_t * StringScanSet( const __wchar_t * pszBlock, const __wchar_t * pszMatch ) throw()
				{
					return (__wchar_t *)wcspbrk( (wchar_t *)pszBlock, (wchar_t *)pszMatch );
				}
				static int StringSpanIncluding( const __wchar_t * pszBlock, const __wchar_t * pszSet ) throw()
				{
					return (int)wcsspn( (wchar_t *)pszBlock, (wchar_t *)pszSet );
				}
				static int StringSpanExcluding( const __wchar_t * pszBlock, const __wchar_t * pszSet ) throw()
				{
					return (int)wcscspn( (wchar_t *)pszBlock, (wchar_t *)pszSet );
				}
				static __wchar_t * StringUppercase( __wchar_t * psz ) throw()
				{
					return (__wchar_t *)_wcsupr( (wchar_t *)psz );
				}
				static __wchar_t * StringLowercase( __wchar_t * psz ) throw()
				{
					return (__wchar_t *)_wcslwr( (wchar_t *)psz );
				}
				static __wchar_t * StringReverse( __wchar_t * psz ) throw()
				{
					return (__wchar_t *)_wcsrev( (wchar_t *)psz );
				}
				static int GetFormattedLength( const __wchar_t * pszFormat, va_list args) throw()
				{
					return _vscwprintf( (wchar_t *)pszFormat, args );
				}
				static int Format( __wchar_t * pszBuffer, const __wchar_t * pszFormat, va_list args) throw()
				{
					return vswprintf( (wchar_t *)pszBuffer, (wchar_t *)pszFormat, args );
				}
				static int GetBaseTypeLength( LPCSTR pszSrc ) throw()
				{
					return
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pszSrc,
							-1,
							NULL,
							0
							) - 1;
				}
				static int GetBaseTypeLength( LPCSTR pszSrc, int nLength ) throw()
				{
					return
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pszSrc,
							nLength,
							NULL,
							0
							);
				}
				static int GetBaseTypeLength( const __wchar_t * pszSrc ) throw()
				{
					return (int)wcslen( (wchar_t *)pszSrc );
				}
				static int GetBaseTypeLength( const __wchar_t * pszSrc, int nLength ) throw()
				{
					(void)pszSrc;
					return nLength;
				}

				static void ConvertToBaseType(
					__wchar_t * pszDest,
					int nDestLength,
					LPCSTR pszSrc,
					int nSrcLength = -1
					) throw()
				{
					::MultiByteToWideChar(
						_AtlGetConversionACP(),
						0,
						pszSrc,
						nSrcLength,
						(LPWSTR)pszDest,
						nDestLength
						);
				}
				static void ConvertToBaseType(
					__wchar_t * pszDest,
					int nDestLength,
					const __wchar_t * pszSrc,
					int nSrcLength = -1
					) throw()
				{
					(void)nSrcLength;
					memcpy( pszDest, pszSrc, nDestLength * sizeof(__wchar_t) );
				}
				static void FloodCharacters( __wchar_t ch, int nLength, __wchar_t * psz ) throw()
				{
					for( int i = 0; i < nLength; i++ )
					{
						psz[i] = ch;
					}
				}
				static BSTR AllocSysString( const __wchar_t * pchData, int nDataLength ) throw()
				{
					return ::SysAllocStringLen( (const OLECHAR *)pchData, nDataLength );
				}
				static BOOL ReAllocSysString( const __wchar_t * pchData, BSTR* pbstr, int nDataLength ) throw()
				{
					return ::SysReAllocStringLen( pbstr, (const OLECHAR *)pchData, nDataLength );
				}
#ifdef _UNICODE
				static DWORD FormatMessage(
					DWORD dwFlags,
					LPCVOID pSource,
					DWORD dwMessageID,
					DWORD dwLanguageID,
					__wchar_t * pszBuffer,
					DWORD nSize,
					va_list * pArguments
					) throw()
				{
					return
						::FormatMessageW(
							dwFlags,
							pSource,
							dwMessageID,
							dwLanguageID,
							(LPWSTR)pszBuffer,
							nSize,
							pArguments
							);
				}
#endif
				static int SafeStringLen( LPCSTR psz ) throw()
				{
					return (psz != NULL) ? (int)strlen( psz ) : 0;
				}
				static int SafeStringLen( const __wchar_t * psz ) throw()
				{
					return (psz != NULL) ? (int)wcslen((wchar_t *) psz ) : 0;
				}
				static int GetCharLen( const __wchar_t* pch ) throw()
				{
					(void)pch;
					return 1;
				}
				static int GetCharLen( const char* pch ) throw()
				{
					return (int)(
						_mbclen(
							reinterpret_cast < const unsigned char * > ( pch )
							)
						);
				}
				static DWORD GetEnvironmentVariable(
					const __wchar_t * pszVar,
					__wchar_t * pszBuffer,
					DWORD dwSize
					) throw()
				{
					return _GetEnvironmentVariableW( pszVar, pszBuffer, dwSize );
				}
			};

			template<>
			class CExtSafeStrTraitMFC<
					unsigned short,
					CExtSafeChTraitsCRT < unsigned short >
				>
			{
#if defined(_UNICODE) && !defined(_CSTRING_ALWAYS_THUNK)
				static DWORD _GetEnvironmentVariableW(
					const unsigned short * pszName,
					unsigned short * pszBuffer,
					DWORD nSize
					) throw()
				{
					return
						::GetEnvironmentVariableW(
							(LPCWSTR)pszName,
							(LPWSTR)pszBuffer,
							nSize
							);
				}
#else
				static DWORD WINAPI _GetEnvironmentVariableW(
					const unsigned short * pszName, 
					unsigned short * pszBuffer,
					DWORD nSize
					) throw()
				{
					return
						_strthunks.pfnGetEnvironmentVariableW(
							(LPCWSTR)pszName,
							(LPWSTR)pszBuffer,
							nSize
							);
				}
#endif
			public:
				static HINSTANCE FindStringResourceInstance( UINT nID ) throw()
				{
					return ( AfxFindStringResourceHandle( nID ) );
				}

				static ATL::IAtlStringMgr * GetDefaultManager() throw()
				{
					return ( AfxGetStringManager() );
				}
				static unsigned short * CharNext( const unsigned short * psz ) throw()
				{
					return const_cast< unsigned short * >( psz+1 );
				}
				static int IsDigit( unsigned short ch ) throw()
				{
					return iswdigit( ch );
				}
				static int IsSpace( unsigned short ch ) throw()
				{
					return iswspace( ch );
				}
				static int StringCompare( const unsigned short * pszA, const unsigned short * pszB ) throw()
				{
					return wcscmp( (wchar_t *)pszA, (wchar_t *)pszB );
				}
				static int StringCompareIgnore( const unsigned short * pszA, const unsigned short * pszB ) throw()
				{
					return _wcsicmp( (wchar_t *)pszA, (wchar_t *)pszB );
				}
				static int StringCollate( const unsigned short * pszA, const unsigned short * pszB ) throw()
				{
					return wcscoll( (wchar_t *)pszA, (wchar_t *)pszB );
				}
				static int StringCollateIgnore( const unsigned short * pszA, const unsigned short * pszB ) throw()
				{
					return _wcsicoll( (wchar_t *)pszA, (wchar_t *)pszB );
				}
				static const unsigned short * StringFindString( const unsigned short * pszBlock, const unsigned short * pszMatch ) throw()
				{
					return (unsigned short *)wcsstr( (wchar_t *)pszBlock, (wchar_t *)pszMatch );
				}
				static unsigned short * StringFindString( unsigned short * pszBlock, const unsigned short * pszMatch ) throw()
				{
					return ( const_cast< unsigned short * >( StringFindString( const_cast< const unsigned short * >( pszBlock ), pszMatch ) ) );
				}
				static const unsigned short * StringFindChar( const unsigned short * pszBlock, unsigned short chMatch ) throw()
				{
					return (unsigned short *)wcschr( (wchar_t *)pszBlock, (wchar_t)chMatch );
				}
				static const unsigned short * StringFindCharRev( const unsigned short * psz, unsigned short ch ) throw()
				{
					return (unsigned short *)wcsrchr( (wchar_t *)psz, (wchar_t)ch );
				}
				static const unsigned short * StringScanSet( const unsigned short * pszBlock, const unsigned short * pszMatch ) throw()
				{
					return (unsigned short *)wcspbrk( (wchar_t *)pszBlock, (wchar_t *)pszMatch );
				}
				static int StringSpanIncluding( const unsigned short * pszBlock, const unsigned short * pszSet ) throw()
				{
					return (int)wcsspn( (wchar_t *)pszBlock, (wchar_t *)pszSet );
				}
				static int StringSpanExcluding( const unsigned short * pszBlock, const unsigned short * pszSet ) throw()
				{
					return (int)wcscspn( (wchar_t *)pszBlock, (wchar_t *)pszSet );
				}
				static unsigned short * StringUppercase( unsigned short * psz ) throw()
				{
					return (unsigned short *)_wcsupr( (wchar_t *)psz );
				}
				static unsigned short * StringLowercase( unsigned short * psz ) throw()
				{
					return (unsigned short *)_wcslwr( (wchar_t *)psz );
				}
				static unsigned short * StringReverse( unsigned short * psz ) throw()
				{
					return (unsigned short *)_wcsrev( (wchar_t *)psz );
				}
				static int GetFormattedLength( const unsigned short * pszFormat, va_list args) throw()
				{
					return _vscwprintf( (wchar_t *)pszFormat, args );
				}
				static int Format( unsigned short * pszBuffer, const unsigned short * pszFormat, va_list args) throw()
				{
					return vswprintf( (wchar_t *)pszBuffer, (wchar_t *)pszFormat, args );
				}
				static int GetBaseTypeLength( LPCSTR pszSrc ) throw()
				{
					return
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pszSrc,
							-1,
							NULL,
							0
							) - 1;
				}
				static int GetBaseTypeLength( LPCSTR pszSrc, int nLength ) throw()
				{
					return
						::MultiByteToWideChar(
							_AtlGetConversionACP(),
							0,
							pszSrc,
							nLength,
							NULL,
							0
							);
				}
				static int GetBaseTypeLength( const unsigned short * pszSrc ) throw()
				{
					return (int)wcslen( (wchar_t *)pszSrc );
				}
				static int GetBaseTypeLength( const unsigned short * pszSrc, int nLength ) throw()
				{
					(void)pszSrc;
					return nLength;
				}
				static void ConvertToBaseType(
					unsigned short * pszDest,
					int nDestLength,
					LPCSTR pszSrc,
					int nSrcLength = -1
					) throw()
				{
					::MultiByteToWideChar(
						_AtlGetConversionACP(),
						0,
						pszSrc,
						nSrcLength,
						(LPWSTR)pszDest,
						nDestLength
						);
				}
				static void ConvertToBaseType(
					unsigned short * pszDest,
					int nDestLength,
					const unsigned short * pszSrc,
					int nSrcLength = -1
					) throw()
				{
					(void)nSrcLength;
					memcpy( pszDest, pszSrc, nDestLength * sizeof(unsigned short) );
				}
				static void FloodCharacters( unsigned short ch, int nLength, unsigned short * psz ) throw()
				{
					for( int i = 0; i < nLength; i++ )
					{
						psz[i] = ch;
					}
				}
				static BSTR AllocSysString( const unsigned short * pchData, int nDataLength ) throw()
				{
					return ::SysAllocStringLen( (const OLECHAR *)pchData, nDataLength );
				}
				static BOOL ReAllocSysString( const unsigned short * pchData, BSTR * pbstr, int nDataLength ) throw()
				{
					return ::SysReAllocStringLen( pbstr, (const OLECHAR *)pchData, nDataLength );
				}
#ifdef _UNICODE
				static DWORD FormatMessage(
					DWORD dwFlags,
					LPCVOID pSource,
					DWORD dwMessageID,
					DWORD dwLanguageID,
					unsigned short * pszBuffer,
					DWORD nSize,
					va_list * pArguments
					) throw()
				{
					return
						::FormatMessageW(
							dwFlags,
							pSource,
							dwMessageID,
							dwLanguageID,
							(LPWSTR)pszBuffer,
							nSize,
							pArguments
							);
				}
#endif
				static int SafeStringLen( LPCSTR psz ) throw()
				{
					return (psz != NULL) ? (int)strlen( psz ) : 0;
				}
				static int SafeStringLen( const unsigned short * psz ) throw()
				{
					return (psz != NULL) ? (int)wcslen( (wchar_t *)psz ) : 0;
				}
				static int GetCharLen( const unsigned short* pch ) throw()
				{
					(void)pch;
					return 1;
				}
				static int GetCharLen( const char* pch ) throw()
				{
					return (int)(
						_mbclen(
							reinterpret_cast < const unsigned char * > ( pch )
							)
						);
				}
				static DWORD GetEnvironmentVariable(
					const unsigned short * pszVar,
					unsigned short * pszBuffer,
					DWORD dwSize
					) throw()
				{
					return _GetEnvironmentVariableW( pszVar, pszBuffer, dwSize );
				}
			};

			template < typename BaseType >
			class CExtSafeSimpleStringT
			{
			public:
				typedef typename CExtSafeChTraitsBase< BaseType >::XCHAR XCHAR;
				typedef typename CExtSafeChTraitsBase< BaseType >::PXSTR PXSTR;
				typedef typename CExtSafeChTraitsBase< BaseType >::PCXSTR PCXSTR;
				typedef typename CExtSafeChTraitsBase< BaseType >::YCHAR YCHAR;
				typedef typename CExtSafeChTraitsBase< BaseType >::PYSTR PYSTR;
				typedef typename CExtSafeChTraitsBase< BaseType >::PCYSTR PCYSTR;

			public:
				explicit CExtSafeSimpleStringT( IAtlStringMgr * pStringMgr ) throw()
				{
					ATLASSERT( pStringMgr != NULL );
					CStringData * pData = pStringMgr->GetNilString();
					Attach( pData );
				}
				CExtSafeSimpleStringT( const CExtSafeSimpleStringT & strSrc )
				{
					CStringData * pSrcData = strSrc.GetData();
					CStringData * pNewData = CloneData( pSrcData );
					Attach( pNewData );
				}
				CExtSafeSimpleStringT( PCXSTR pszSrc, IAtlStringMgr * pStringMgr )
				{
					ATLASSERT( pStringMgr != NULL );
					int nLength = StringLength( pszSrc );
					CStringData * pData = pStringMgr->Allocate( nLength, sizeof(XCHAR) );
					if( pData == NULL )
					{
						ThrowMemoryException();
					}
					Attach( pData );
					SetLength( nLength );
					CopyChars( m_pszData, pszSrc, nLength );
				}
				CExtSafeSimpleStringT( const XCHAR * pchSrc, int nLength, IAtlStringMgr * pStringMgr )
				{
					ATLASSERT( pStringMgr != NULL );
					CStringData * pData = pStringMgr->Allocate( nLength, sizeof(XCHAR) );
					if( pData == NULL )
					{
						ThrowMemoryException();
					}
					Attach( pData );
					SetLength( nLength );
					CopyChars( m_pszData, pchSrc, nLength );
				}
				~CExtSafeSimpleStringT() throw()
				{
					CStringData * pData = GetData();
					pData->Release();
				}
				CExtSafeSimpleStringT & operator=( const CExtSafeSimpleStringT & strSrc )
				{
					CStringData * pSrcData = strSrc.GetData();
					CStringData * pOldData = GetData();
					if( pSrcData != pOldData )
					{
						if( pOldData->IsLocked() )
						{
							SetString( strSrc.GetString(), strSrc.GetLength() );
						}
						else
						{
							CStringData * pNewData = CloneData( pSrcData );
							pOldData->Release();
							Attach( pNewData );
						}
					}
					return ( *this );
				}
				CExtSafeSimpleStringT & operator=( PCXSTR pszSrc )
				{
					SetString( pszSrc );
					return ( *this );
				}
				CExtSafeSimpleStringT & operator+=( const CExtSafeSimpleStringT & strSrc )
				{
					Append( strSrc );
					return ( *this );
				}
				CExtSafeSimpleStringT & operator+=( PCXSTR pszSrc )
				{
					Append( pszSrc );
					return ( *this );
				}
				template < int t_nSize >
				CExtSafeSimpleStringT & operator+=( const CStaticString < XCHAR, t_nSize > & strSrc )
				{
					Append( strSrc.m_psz, strSrc.GetLength() );
					return ( *this );
				}
				CExtSafeSimpleStringT & operator+=( char ch )
				{
					AppendChar( XCHAR( ch ) );
					return ( *this );
				}
				CExtSafeSimpleStringT & operator+=( unsigned char ch )
				{
					AppendChar( XCHAR( ch ) );
					return ( *this );
				}
				CExtSafeSimpleStringT & operator+=( wchar_t ch )
				{
					AppendChar( XCHAR( ch ) );
					return ( *this );
				}
				XCHAR operator[]( int iChar ) const throw()
				{
					ATLASSERT( (iChar >= 0) && (iChar <= GetLength()) );
					return ( m_pszData[iChar] );
				}
				operator PCXSTR() const throw()
				{
					return ( m_pszData );
				}
				void Append( PCXSTR pszSrc )
				{
					Append( pszSrc, StringLength( pszSrc ) );
				}
				void Append( PCXSTR pszSrc, int nLength )
				{
					UINT_PTR nOffset = pszSrc-GetString();
					UINT nOldLength = GetLength();
					int nNewLength = nOldLength+nLength;
					PXSTR pszBuffer = GetBuffer( nNewLength );
					if( nOffset <= nOldLength )
					{
						pszSrc = pszBuffer+nOffset;
					}
					CopyChars( pszBuffer+nOldLength, pszSrc, nLength );
					ReleaseBufferSetLength( nNewLength );
				}
				void AppendChar( XCHAR ch )
				{
					UINT nOldLength = GetLength();
					int nNewLength = nOldLength+1;
					PXSTR pszBuffer = GetBuffer( nNewLength );
					pszBuffer[nOldLength] = ch;
					ReleaseBufferSetLength( nNewLength );
				}
				void Append( const CExtSafeSimpleStringT & strSrc )
				{
					Append( strSrc.GetString(), strSrc.GetLength() );
				}
				void Empty() throw()
				{
					CStringData * pOldData = GetData();
					IAtlStringMgr * pStringMgr = pOldData->pStringMgr;
					if( pOldData->nDataLength == 0 )
					{
						return;
					}
					if( pOldData->IsLocked() )
					{
						SetLength( 0 );
					}
					else
					{
						pOldData->Release();
						CStringData * pNewData = pStringMgr->GetNilString();
						Attach( pNewData );
					}
				}
				void FreeExtra() throw()
				{
					CStringData * pOldData = GetData();
					int nLength = pOldData->nDataLength;
					IAtlStringMgr * pStringMgr = pOldData->pStringMgr;
					if( pOldData->nAllocLength == nLength )
					{
						return;
					}
					if( !pOldData->IsLocked() )
					{
						CStringData * pNewData = pStringMgr->Allocate( nLength, sizeof(XCHAR) );
						if( pNewData == NULL )
						{
							SetLength( nLength );
							return;
						}
						CopyChars( PXSTR( pNewData->data() ), PCXSTR( pOldData->data() ), nLength );
						pOldData->Release();
						Attach( pNewData );
						SetLength( nLength );
					}
				}
				int GetAllocLength() const throw()
				{
					return ( GetData()->nAllocLength );
				}
				XCHAR GetAt( int iChar ) const throw()
				{
					ATLASSERT( (iChar >= 0) && (iChar <= GetLength()) );
					return ( m_pszData[iChar] );
				}
				PXSTR GetBuffer()
				{
					CStringData * pData = GetData();
					if( pData->IsShared() )
					{
						Fork( pData->nDataLength );
					}
					return ( m_pszData );
				}
				PXSTR GetBuffer( int nMinBufferLength )
				{
					return ( PrepareWrite( nMinBufferLength ) );
				}
				PXSTR GetBufferSetLength( int nLength )
				{
					PXSTR pszBuffer = GetBuffer( nLength );
					SetLength( nLength );
					return ( pszBuffer );
				}
				int GetLength() const throw()
				{
					return ( GetData()->nDataLength );
				}
				IAtlStringMgr * GetManager() const throw()
				{
					return ( GetData()->pStringMgr->Clone() );
				}
				PCXSTR GetString() const throw()
				{
					return ( m_pszData );
				}
				bool IsEmpty() const throw()
				{
					return ( GetLength() == 0 );
				}
				PXSTR LockBuffer()
				{
					CStringData * pData = GetData();
					if( pData->IsShared() )
					{
						Fork( pData->nDataLength );
						pData = GetData();
					}
					pData->Lock();
					return ( m_pszData );
				}
				void UnlockBuffer() throw()
				{
					CStringData * pData = GetData();
					pData->Unlock();
				}
				void Preallocate( int nLength )
				{
					PrepareWrite( nLength );
				}
				void ReleaseBuffer( int nNewLength = -1 ) throw()
				{
					if( nNewLength == -1 )
					{
						nNewLength = StringLength( m_pszData );
					}
					SetLength( nNewLength );
				}
				void ReleaseBufferSetLength( int nNewLength ) throw()
				{
					ATLASSERT( nNewLength >= 0 );
					SetLength( nNewLength );
				}
				void Truncate( int nNewLength )
				{
					ATLASSERT( nNewLength <= GetLength() );
					GetBuffer( nNewLength );
					ReleaseBufferSetLength( nNewLength );
				}
				void SetAt( int iChar, XCHAR ch )
				{
					ATLASSERT( (iChar >= 0) && (iChar < GetLength()) );
					int nLength = GetLength();
					PXSTR pszBuffer = GetBuffer();
					pszBuffer[iChar] = ch;
					ReleaseBufferSetLength( nLength );
				}
				void SetManager( IAtlStringMgr * pStringMgr )
				{
					ATLASSERT( IsEmpty() );
					CStringData * pData = GetData();
					pData->Release();
					pData = pStringMgr->GetNilString();
					Attach( pData );
				}
				void SetString( PCXSTR pszSrc )
				{
					SetString( pszSrc, StringLength( pszSrc ) );
				}
				void SetString( PCXSTR pszSrc, int nLength )
				{
					if( nLength == 0 )
					{
						Empty();
					}
					else
					{
						UINT nOldLength = GetLength();
						UINT_PTR nOffset = pszSrc-GetString();
						PXSTR pszBuffer = GetBuffer( nLength );
						if( nOffset <= nOldLength )
						{
							CopyCharsOverlapped( pszBuffer, pszBuffer+nOffset, nLength );
						}
						else
						{
							CopyChars( pszBuffer, pszSrc, nLength );
						}
						ReleaseBufferSetLength( nLength );
					}
				}

			public:
				friend CExtSafeSimpleStringT operator+(
					const CExtSafeSimpleStringT & str1,
					const CExtSafeSimpleStringT & str2
					)
				{
					CExtSafeSimpleStringT s( str1.GetManager() );
					Concatenate( s, str1, str1.GetLength(), str2, str2.GetLength() );
					return ( s );
				}
				friend CExtSafeSimpleStringT operator+(
					const CExtSafeSimpleStringT & str1,
					PCXSTR psz2
					)
				{
					CExtSafeSimpleStringT s( str1.GetManager() );
					Concatenate( s, str1, str1.GetLength(), psz2, StringLength( psz2 ) );
					return ( s );
				}
				friend CExtSafeSimpleStringT operator+(
					PCXSTR psz1,
					const CExtSafeSimpleStringT & str2
					)
				{
					CExtSafeSimpleStringT s( str2.GetManager() );
					Concatenate( s, psz1, StringLength( psz1 ), str2, str2.GetLength() );
					return ( s );
				}
				static void CopyChars( XCHAR * pchDest, const XCHAR * pchSrc, int nChars ) throw()
				{
					memcpy( pchDest, pchSrc, nChars * sizeof(XCHAR) );
				}
				static void CopyCharsOverlapped( XCHAR * pchDest, const XCHAR * pchSrc, int nChars ) throw()
				{
					memmove( pchDest, pchSrc, nChars * sizeof(XCHAR) );
				}
#ifdef _ATL_MIN_CRT
				ATL_NOINLINE static int StringLength( PCXSTR psz ) throw()
				{
					int nLength = 0;
					if( psz != NULL )
					{
						const XCHAR * pch = psz;
						while( *pch != 0 )
						{
							nLength++;
							pch++;
						}
					}
					return ( nLength );
				}
#else
				static int StringLength( const char* psz ) throw()
				{
					if( psz == NULL )
					{
						return ( 0 );
					}
					return ( int( strlen( psz ) ) );
				}
				static int StringLength( const __wchar_t * psz ) throw()
				{
					if( psz == NULL )
					{
						return ( 0 );
					}
					return ( int( wcslen( (wchar_t *)psz ) ) );
				}
				static int StringLength( const unsigned short * psz ) throw()
				{
					if( psz == NULL )
					{
						return ( 0 );
					}
					return ( int( wcslen( (wchar_t *)psz ) ) );
				}
#endif
			protected:
				static void Concatenate( CExtSafeSimpleStringT & strResult, PCXSTR psz1, int nLength1, PCXSTR psz2, int nLength2 )
				{
					int nNewLength = nLength1+nLength2;
					PXSTR pszBuffer = strResult.GetBuffer( nNewLength );
					CopyChars( pszBuffer, psz1, nLength1 );
					CopyChars( pszBuffer+nLength1, psz2, nLength2 );
					strResult.ReleaseBufferSetLength( nNewLength );
				}
				ATL_NOINLINE __declspec( noreturn ) static void ThrowMemoryException()
				{
					AtlThrow( E_OUTOFMEMORY );
				}
			private:
				void Attach( CStringData * pData ) throw()
				{
					m_pszData = static_cast < PXSTR >( pData->data() );
				}
				ATL_NOINLINE void Fork( int nLength )
				{
					CStringData * pOldData = GetData();
					int nOldLength = pOldData->nDataLength;
					CStringData * pNewData = pOldData->pStringMgr->Clone()->Allocate( nLength, sizeof(XCHAR) );
					if( pNewData == NULL )
					{
						ThrowMemoryException();
					}
					int nCharsToCopy = ((nOldLength < nLength) ? nOldLength : nLength)+1;
					CopyChars( PXSTR( pNewData->data() ), PCXSTR( pOldData->data() ), nCharsToCopy );
					pNewData->nDataLength = nOldLength;
					pOldData->Release();
					Attach( pNewData );
				}
				CStringData * GetData() const throw()
				{
					return ( reinterpret_cast < CStringData * > ( m_pszData ) - 1 );
				}
				PXSTR PrepareWrite( int nLength )
				{
					CStringData * pOldData = GetData();
					int nShared = 1-pOldData->nRefs;
					int nTooShort = pOldData->nAllocLength-nLength;
					if( (nShared|nTooShort) < 0 )
					{
						PrepareWrite2( nLength );
					}
					return ( m_pszData );
				}
				ATL_NOINLINE void PrepareWrite2( int nLength )
				{
					CStringData * pOldData = GetData();
					if( pOldData->nDataLength > nLength )
					{
						nLength = pOldData->nDataLength;
					}
					if( pOldData->IsShared() )
					{
						Fork( nLength );
					}
					else if( pOldData->nAllocLength < nLength )
					{
						int nNewLength = pOldData->nAllocLength;
						if( nNewLength > 1024 )
						{
							nNewLength += 1024;
						}
						else
						{
							nNewLength *= 2;
						}
						if( nNewLength < nLength )
						{
							nNewLength = nLength;
						}
						Reallocate( nNewLength );
					}
				}
				ATL_NOINLINE void Reallocate( int nLength )
				{
					CStringData * pOldData = GetData();
					ATLASSERT( pOldData->nAllocLength < nLength );
					IAtlStringMgr * pStringMgr = pOldData->pStringMgr;
					CStringData * pNewData = pStringMgr->Reallocate( pOldData, nLength, sizeof(XCHAR) );
					if( pNewData == NULL )
					{
						ThrowMemoryException();
					}
					Attach( pNewData );
				}
				void SetLength( int nLength ) throw()
				{
					ATLASSERT( nLength >= 0 );
					ATLASSERT( nLength <= GetData()->nAllocLength );
					GetData()->nDataLength = nLength;
					m_pszData[nLength] = 0;
				}
				static CStringData * CloneData( CStringData * pData )
				{
					CStringData * pNewData = NULL;
					IAtlStringMgr * pNewStringMgr = pData->pStringMgr->Clone();
					if( !pData->IsLocked() && (pNewStringMgr == pData->pStringMgr) )
					{
						pNewData = pData;
						pNewData->AddRef();
					}
					else
					{
						pNewData = pNewStringMgr->Allocate( pData->nDataLength, sizeof(XCHAR) );
						if( pNewData == NULL )
						{
							ThrowMemoryException();
						}
						pNewData->nDataLength = pData->nDataLength;
						CopyChars( PXSTR( pNewData->data() ), PCXSTR( pData->data() ), pData->nDataLength+1 );
					}
					return ( pNewData );
				}
			private:
				PXSTR m_pszData;
			};

			template < typename BaseType, class StringTraits >
			class CExtSafeStringT : public CExtSafeSimpleStringT < BaseType >
			{
			public:
				typedef CExtSafeSimpleStringT< BaseType > CThisSimpleString;
				typedef StringTraits StrTraits;
				typedef BaseType WCHAR;
				typedef BaseType * LPWSTR;
				typedef const BaseType * LCPWSTR;

			public:
				CExtSafeStringT() throw()
					: CThisSimpleString( StringTraits::GetDefaultManager() )
				{
				}
				explicit CExtSafeStringT( IAtlStringMgr * pStringMgr ) throw()
					: CThisSimpleString( pStringMgr )
				{ 
				}
//				CExtSafeStringT( const VARIANT & varSrc );
//				CExtSafeStringT( const VARIANT & varSrc, IAtlStringMgr * pStringMgr );
				static void Construct( CExtSafeStringT * pString )
				{
					new( pString ) CExtSafeStringT;
				}
				CExtSafeStringT( const CExtSafeStringT & strSrc )
					: CThisSimpleString( strSrc )
				{
				}
				CExtSafeStringT( const CThisSimpleString & strSrc )
					: CThisSimpleString( strSrc )
				{
				}
				CExtSafeStringT( const XCHAR * pszSrc )
					: CThisSimpleString( StringTraits::GetDefaultManager() )
				{
					if( !CheckImplicitLoad( pszSrc ) )
					{
						*this = pszSrc;
					}
				}
				CExtSafeStringT( LPCSTR pszSrc, IAtlStringMgr * pStringMgr )
					: CThisSimpleString( pStringMgr )
				{
					if( !CheckImplicitLoad( pszSrc ) )
					{
						*this = pszSrc;
					}
				}
				CSTRING_EXPLICIT CExtSafeStringT( const YCHAR * pszSrc )
					: CThisSimpleString( StringTraits::GetDefaultManager() )
				{
					if( !CheckImplicitLoad( pszSrc ) )
					{
						*this = pszSrc;
					}
				}
				CExtSafeStringT( LPCWSTR pszSrc, IAtlStringMgr * pStringMgr )
					: CThisSimpleString( pStringMgr )
				{
					if( !CheckImplicitLoad( pszSrc ) )
					{
						*this = (PCXSTR)pszSrc;
					}
				}
//#ifdef _MANAGED
//				CExtSafeStringT( System::String* pString )
//					: CThisSimpleString( StringTraits::GetDefaultManager() )
//				{
//					const wchar_t __pin* psz = PtrToStringChars( pString );
//					*this = psz;
//				}
//#endif
				CSTRING_EXPLICIT CExtSafeStringT( const unsigned char * pszSrc )
					: CThisSimpleString( StringTraits::GetDefaultManager() )
				{
					*this = reinterpret_cast< const char* >( pszSrc );
				}
				CExtSafeStringT(
					const unsigned char * pszSrc,
					IAtlStringMgr * pStringMgr
					)
					: CThisSimpleString( pStringMgr )
				{
					*this = reinterpret_cast < const char * > ( pszSrc );
				}
				CSTRING_EXPLICIT CExtSafeStringT( char ch, int nLength = 1 )
					: CThisSimpleString( StringTraits::GetDefaultManager() )
				{
					ATLASSERT( nLength >= 0 );
					if( nLength > 0 )
					{
						PXSTR pszBuffer = GetBuffer( nLength );
						StringTraits::FloodCharacters( XCHAR( ch ), nLength, pszBuffer );
						ReleaseBufferSetLength( nLength );
					}
				}
				CSTRING_EXPLICIT CExtSafeStringT( wchar_t ch, int nLength = 1 )
					: CThisSimpleString( StringTraits::GetDefaultManager() )
				{
					ATLASSERT( nLength >= 0 );
					if( nLength > 0 )
					{
						PXSTR pszBuffer = GetBuffer( nLength );
						StringTraits::FloodCharacters( XCHAR( ch ), nLength, pszBuffer );
						ReleaseBufferSetLength( nLength );
					}
				}
				CExtSafeStringT( const XCHAR * pch, int nLength )
					: CThisSimpleString( pch, nLength, StringTraits::GetDefaultManager() )
				{
				}
				CExtSafeStringT( const XCHAR * pch, int nLength, IAtlStringMgr * pStringMgr )
					: CThisSimpleString( pch, nLength, pStringMgr )
				{
				}
				CExtSafeStringT( const YCHAR * pch, int nLength )
					: CThisSimpleString( StringTraits::GetDefaultManager() )
				{
					ATLASSERT( nLength >= 0 );
					if( nLength > 0 )
					{
						ATLASSERT( AtlIsValidAddress( pch, nLength * sizeof(YCHAR), FALSE ) );
						int nDestLength = StringTraits::GetBaseTypeLength( pch, nLength );
						PXSTR pszBuffer = GetBuffer( nDestLength );
						StringTraits::ConvertToBaseType( pszBuffer, nDestLength, pch, nLength );
						ReleaseBufferSetLength( nDestLength );
					}
				}
				CExtSafeStringT( const YCHAR * pch, int nLength, IAtlStringMgr * pStringMgr )
					: CThisSimpleString( pStringMgr )
				{
					ATLASSERT( nLength >= 0 );
					if( nLength > 0 )
					{
						ATLASSERT( AtlIsValidAddress( pch, nLength * sizeof(YCHAR), FALSE ) );
						int nDestLength = StringTraits::GetBaseTypeLength( pch, nLength );
						PXSTR pszBuffer = GetBuffer( nDestLength );
						StringTraits::ConvertToBaseType( pszBuffer, nDestLength, pch, nLength );
						ReleaseBufferSetLength( nDestLength );
					}
				}
				~CExtSafeStringT() throw()
				{
				}
				CExtSafeStringT & operator=( const CExtSafeStringT & strSrc )
				{
					CThisSimpleString::operator=( strSrc );
					return ( *this );
				}
				CExtSafeStringT & operator=( const CThisSimpleString& strSrc )
				{
					CThisSimpleString::operator=( strSrc );
					return ( *this );
				}
				CExtSafeStringT & operator=( PCXSTR pszSrc )
				{
					CThisSimpleString::operator=( pszSrc );
					return ( *this );
				}
				CExtSafeStringT & operator=( PCYSTR pszSrc )
				{
					int nDestLength = (pszSrc != NULL) ? StringTraits::GetBaseTypeLength( pszSrc ) : 0;
					if( nDestLength > 0 )
					{
						PXSTR pszBuffer = GetBuffer( nDestLength );
						StringTraits::ConvertToBaseType( pszBuffer, nDestLength, pszSrc );
						ReleaseBufferSetLength( nDestLength );
					}
					else
					{
						Empty();
					}
					return ( *this );
				}
				CExtSafeStringT & operator=( const unsigned char* pszSrc )
				{
					return ( operator=( reinterpret_cast< const char* >( pszSrc ) ) );
				}
				CExtSafeStringT & operator=( char ch )
				{
					char ach[2] = { ch, 0 };
					return ( operator=( ach ) );
				}
				CExtSafeStringT & operator=( wchar_t ch )
				{
					wchar_t ach[2] = { ch, 0 };
					return ( operator=( ach ) );
				}
//				CExtSafeStringT & operator=( const VARIANT& var );
				CExtSafeStringT & operator+=( const CThisSimpleString& str )
				{
					CThisSimpleString::operator+=( str );
					return ( *this );
				}
				CExtSafeStringT & operator+=( PCXSTR pszSrc )
				{
					CThisSimpleString::operator+=( pszSrc );
					return ( *this );
				}
				template < int t_nSize >
				CExtSafeStringT & operator+=( const CStaticString < XCHAR, t_nSize > & strSrc )
				{
					CThisSimpleString::operator+=( strSrc );
					return ( *this );
				}
				CExtSafeStringT & operator+=( PCYSTR pszSrc )
				{
					CExtSafeStringT str( pszSrc, GetManager() );
					return ( operator+=( str ) );
				}
				CExtSafeStringT & operator+=( char ch )
				{
					CThisSimpleString::operator+=( ch );
					return ( *this );
				}
				CExtSafeStringT & operator+=( unsigned char ch )
				{
					CThisSimpleString::operator+=( ch );
					return ( *this );
				}
				CExtSafeStringT & operator+=( wchar_t ch )
				{
					CThisSimpleString::operator+=( ch );
					return ( *this );
				}
//				CExtSafeStringT & operator+=( const VARIANT& var );
				int Compare( PCXSTR psz ) const throw()
				{
					ATLASSERT( AtlIsValidString( ((LPCWSTR)psz) ) );
					return ( StringTraits::StringCompare( GetString(), psz ) );
				}
				int CompareNoCase( PCXSTR psz ) const throw()
				{
					ATLASSERT( AtlIsValidString( ((LPCWSTR)psz) ) );
					return ( StringTraits::StringCompareIgnore( GetString(), psz ) );
				}
				int Collate( PCXSTR psz ) const throw()
				{
					ATLASSERT( AtlIsValidString( ((LPCWSTR)psz) ) );
					return ( StringTraits::StringCollate( GetString(), psz ) );
				}
				int CollateNoCase( PCXSTR psz ) const throw()
				{
					ATLASSERT( AtlIsValidString( ((LPCWSTR)psz) ) );
					return ( StringTraits::StringCollateIgnore( GetString(), psz ) );
				}
				int Delete( int iIndex, int nCount = 1 )
				{
					ATLASSERT( iIndex >= 0 );
					ATLASSERT( nCount >= 0 );
					int nLength = GetLength();
					if( (nCount+iIndex) > nLength )
					{
						nCount = nLength-iIndex;
					}
					if( nCount > 0 )
					{
						int nNewLength = nLength-nCount;
						int nXCHARsToCopy = nLength-(iIndex+nCount)+1;
						PXSTR pszBuffer = GetBuffer();
						memmove( pszBuffer+iIndex, pszBuffer+iIndex+nCount, nXCHARsToCopy * sizeof(XCHAR) );
						ReleaseBufferSetLength( nNewLength );
					}
					return ( GetLength() );
				}
				int Insert( int iIndex, XCHAR ch )
				{
					ATLASSERT( iIndex >= 0 );
					if( iIndex > GetLength() )
					{
						iIndex = GetLength();
					}
					int nNewLength = GetLength()+1;
					PXSTR pszBuffer = GetBuffer( nNewLength );
					memmove( pszBuffer+iIndex+1, pszBuffer+iIndex, (nNewLength-iIndex) * sizeof(XCHAR) );
					pszBuffer[iIndex] = ch;
					ReleaseBufferSetLength( nNewLength );
					return ( nNewLength );
				}
				int Insert( int iIndex, PCXSTR psz )
				{
					ATLASSERT( iIndex >= 0 );
					if( iIndex > GetLength() )
					{
						iIndex = GetLength();
					}
					int nInsertLength = StringTraits::SafeStringLen( psz );
					int nNewLength = GetLength();
					if( nInsertLength > 0 )
					{
						nNewLength += nInsertLength;
						PXSTR pszBuffer = GetBuffer( nNewLength );
						memmove( pszBuffer+iIndex+nInsertLength,
							pszBuffer+iIndex, (nNewLength-iIndex-nInsertLength+1) * sizeof(XCHAR) );
						memcpy( pszBuffer+iIndex, psz, nInsertLength * sizeof(XCHAR) );
						ReleaseBufferSetLength( nNewLength );
					}
					return ( nNewLength );
				}
				int Replace( XCHAR chOld, XCHAR chNew )
				{
					int nCount = 0;
					if( chOld != chNew )
					{
						bool bCopied = false;
						PXSTR pszBuffer = const_cast< PXSTR >( GetString() );
						int nLength = GetLength();
						int iChar = 0;
						while( iChar < nLength )
						{
							if( pszBuffer[iChar] == chOld )
							{
								if( !bCopied )
								{
									bCopied = true;
									pszBuffer = GetBuffer( nLength );
								}
								pszBuffer[iChar] = chNew;
								nCount++;
							}
							iChar = int( StringTraits::CharNext( pszBuffer+iChar )-pszBuffer );
						}
						if( bCopied )
						{
							ReleaseBufferSetLength( nLength );
						}
					}
					return ( nCount );
				}
				int Replace( PCXSTR pszOld, PCXSTR pszNew )
				{
					int nSourceLen = StringTraits::SafeStringLen( pszOld );
					if( nSourceLen == 0 )
						return ( 0 );
					int nReplacementLen = StringTraits::SafeStringLen( pszNew );
					int nCount = 0;
					{
						PCXSTR pszStart = GetString();
						PCXSTR pszEnd = pszStart+GetLength();
						while( pszStart < pszEnd )
						{
							PCXSTR pszTarget;
							while( (pszTarget = StringTraits::StringFindString( pszStart, pszOld ) ) != NULL)
							{
								nCount++;
								pszStart = pszTarget+nSourceLen;
							}
							pszStart += StringTraits::SafeStringLen( pszStart )+1;
						}
					}
					if( nCount > 0 )
					{
						int nOldLength = GetLength();
						int nNewLength = nOldLength+(nReplacementLen-nSourceLen)*nCount;

						PXSTR pszBuffer = GetBuffer( max( nNewLength, nOldLength ) );

						PXSTR pszStart = pszBuffer;
						PXSTR pszEnd = pszStart+nOldLength;
						while( pszStart < pszEnd )
						{
							PXSTR pszTarget;
							while( (pszTarget = StringTraits::StringFindString( pszStart, pszOld ) ) != NULL )
							{
								int nBalance = nOldLength-int(pszTarget-pszBuffer+nSourceLen);
								memmove( pszTarget+nReplacementLen, pszTarget+nSourceLen, nBalance * sizeof(XCHAR) );

								memcpy( pszTarget, pszNew, nReplacementLen * sizeof(XCHAR) );
								pszStart = pszTarget+nReplacementLen;
								pszTarget[nReplacementLen+nBalance] = 0;
								nOldLength += (nReplacementLen-nSourceLen);
							}
							pszStart += StringTraits::SafeStringLen( pszStart )+1;
						}
						ATLASSERT( pszBuffer[nNewLength] == 0 );
						ReleaseBufferSetLength( nNewLength );
					}
					return ( nCount );
				}
				int Remove( XCHAR chRemove )
				{
					int nLength = GetLength();
					PXSTR pszBuffer = GetBuffer( nLength );
					PXSTR pszSource = pszBuffer;
					PXSTR pszDest = pszBuffer;
					PXSTR pszEnd = pszBuffer+nLength;
					while( pszSource < pszEnd )
					{
						PXSTR pszNewSource = StringTraits::CharNext( pszSource );
						if( *pszSource != chRemove )
						{
							PXSTR pszNewDest = pszDest+(pszNewSource-pszSource);
							while( pszDest != pszNewDest )
							{
								*pszDest = *pszSource;
								pszSource++;
								pszDest++;
							}
						}
						pszSource = pszNewSource;
					}
					*pszDest = 0;
					int nCount = int( pszSource-pszDest );
					ReleaseBufferSetLength( nLength-nCount );
					return ( nCount );
				}
				CExtSafeStringT Tokenize( PCXSTR pszTokens, int& iStart ) const
				{
					ATLASSERT( iStart >= 0 );
					if( pszTokens == NULL )
					{
						return ( *this );
					}
					PCXSTR pszPlace = GetString()+iStart;
					PCXSTR pszEnd = GetString()+GetLength();
					if( pszPlace < pszEnd )
					{
						int nIncluding =
							StringTraits::StringSpanIncluding( pszPlace, pszTokens );
						if( (pszPlace+nIncluding) < pszEnd )
						{
							pszPlace += nIncluding;
							int nExcluding = StringTraits::StringSpanExcluding( pszPlace, pszTokens );
							int iFrom = iStart+nIncluding;
							int nUntil = nExcluding;
							iStart = iFrom+nUntil+1;
							return ( Mid( iFrom, nUntil ) );
						}
					}
					iStart = -1;
					return ( CExtSafeStringT( GetManager() ) );
				}
				int Find( XCHAR ch, int iStart = 0 ) const throw()
				{
					ATLASSERT( iStart >= 0 );
					int nLength = GetLength();
					if( iStart >= nLength)
					{
						return ( -1 );
					}
					PCXSTR psz = StringTraits::StringFindChar( GetString()+iStart, ch );
					return ( (psz == NULL) ? -1 : int( psz-GetString() ) );
				}
				int Find( PCXSTR pszSub, int iStart = 0 ) const throw()
				{
					ATLASSERT( iStart >= 0 );
					ATLASSERT( AtlIsValidString( ((LPCWSTR)pszSub) ) );
					int nLength = GetLength();
					if( iStart > nLength )
					{
						return ( -1 );
					}
					PCXSTR psz = StringTraits::StringFindString( GetString()+iStart, pszSub );
					return ( (psz == NULL) ? -1 : int( psz-GetString() ) );
				}
				int FindOneOf( PCXSTR pszCharSet ) const throw()
				{
					ATLASSERT( AtlIsValidString( ((LPCWSTR)pszCharSet) ) );
					PCXSTR psz = StringTraits::StringScanSet( GetString(), pszCharSet );
					return ( (psz == NULL) ? -1 : int( psz-GetString() ) );
				}
				int ReverseFind( XCHAR ch ) const throw()
				{
					PCXSTR psz = StringTraits::StringFindCharRev( GetString(), ch );
					return ( (psz == NULL) ? -1 : int( psz-GetString() ) );
				}
				CExtSafeStringT & MakeUpper()
				{
					int nLength = GetLength();
					PXSTR pszBuffer = GetBuffer( nLength );
					StringTraits::StringUppercase( pszBuffer );
					ReleaseBufferSetLength( nLength );
					return ( *this );
				}
				CExtSafeStringT & MakeLower()
				{
					int nLength = GetLength();
					PXSTR pszBuffer = GetBuffer( nLength );
					StringTraits::StringLowercase( pszBuffer );
					ReleaseBufferSetLength( nLength );
					return ( *this );
				}
				CExtSafeStringT & MakeReverse()
				{
					int nLength = GetLength();
					PXSTR pszBuffer = GetBuffer( nLength );
					StringTraits::StringReverse( pszBuffer );
					ReleaseBufferSetLength( nLength );
					return ( *this );
				}
				CExtSafeStringT & TrimRight()
				{
					PCXSTR psz = GetString();
					PCXSTR pszLast = NULL;
					while( *psz != 0 )
					{
						if( StringTraits::IsSpace( *psz ) )
						{
							if( pszLast == NULL )
								pszLast = psz;
						}
						else
						{
							pszLast = NULL;
						}
						psz = StringTraits::CharNext( psz );
					}
					if( pszLast != NULL )
					{
						int iLast = int( pszLast-GetString() );

						Truncate( iLast );
					}
					return ( *this );
				}
				CExtSafeStringT & TrimLeft()
				{
					PCXSTR psz = GetString();
					while( StringTraits::IsSpace( *psz ) )
					{
						psz = StringTraits::CharNext( psz );
					}
					if( psz != GetString() )
					{
						int iFirst = int( psz-GetString() );
						PXSTR pszBuffer = GetBuffer( GetLength() );
						psz = pszBuffer+iFirst;
						int nDataLength = GetLength()-iFirst;
						memmove( pszBuffer, psz, (nDataLength+1) * sizeof(XCHAR) );
						ReleaseBufferSetLength( nDataLength );
					}
					return ( *this );
				}
				CExtSafeStringT & Trim()
				{
					return ( TrimRight().TrimLeft() );
				}
				CExtSafeStringT & Trim( XCHAR chTarget )
				{
					return ( TrimRight( chTarget ).TrimLeft( chTarget ) );
				}
				CExtSafeStringT & Trim( PCXSTR pszTargets )
				{
					return ( TrimRight( pszTargets ).TrimLeft( pszTargets ) );
				}
				CExtSafeStringT & TrimRight( XCHAR chTarget )
				{
					PCXSTR psz = GetString();
					PCXSTR pszLast = NULL;
					while( *psz != 0 )
					{
						if( *psz == chTarget )
						{
							if( pszLast == NULL )
							{
								pszLast = psz;
							}
						}
						else
						{
							pszLast = NULL;
						}
						psz = StringTraits::CharNext( psz );
					}
					if( pszLast != NULL )
					{
						int iLast = int( pszLast-GetString() );
						Truncate( iLast );
					}
					return ( *this );
				}
				CExtSafeStringT & TrimRight( PCXSTR pszTargets )
				{
					if( (pszTargets == NULL) || (*pszTargets == 0) )
					{
						return ( *this );
					}
					PCXSTR psz = GetString();
					PCXSTR pszLast = NULL;
					while( *psz != 0 )
					{
						if( StringTraits::StringFindChar( pszTargets, *psz ) != NULL )
						{
							if( pszLast == NULL )
							{
								pszLast = psz;
							}
						}
						else
						{
							pszLast = NULL;
						}
						psz = StringTraits::CharNext( psz );
					}
					if( pszLast != NULL )
					{
						int iLast = int( pszLast-GetString() );
						Truncate( iLast );
					}
					return ( *this );
				}
				CExtSafeStringT & TrimLeft( XCHAR chTarget )
				{
					PCXSTR psz = GetString();
					while( chTarget == *psz )
					{
						psz = StringTraits::CharNext( psz );
					}
					if( psz != GetString() )
					{
						int iFirst = int( psz-GetString() );
						PXSTR pszBuffer = GetBuffer( GetLength() );
						psz = pszBuffer+iFirst;
						int nDataLength = GetLength()-iFirst;
						memmove( pszBuffer, psz, (nDataLength+1) * sizeof(XCHAR) );
						ReleaseBufferSetLength( nDataLength );
					}
					return ( *this );
				}
				CExtSafeStringT & TrimLeft( PCXSTR pszTargets )
				{
					if( (pszTargets == NULL) || (*pszTargets == 0) )
					{
						return ( *this );
					}
					PCXSTR psz = GetString();
					while( (*psz != 0) && (StringTraits::StringFindChar( pszTargets, *psz ) != NULL) )
					{
						psz = StringTraits::CharNext( psz );
					}
					if( psz != GetString() )
					{
						int iFirst = int( psz-GetString() );
						PXSTR pszBuffer = GetBuffer( GetLength() );
						psz = pszBuffer+iFirst;
						int nDataLength = GetLength()-iFirst;
						memmove( pszBuffer, psz, (nDataLength+1) * sizeof(XCHAR) );
						ReleaseBufferSetLength( nDataLength );
					}
					return ( *this );
				}
				__if_exists( StringTraits::ConvertToOem )
				{
							void AnsiToOem()
							{
								int nLength = GetLength();
								PXSTR pszBuffer = GetBuffer( nLength );
								StringTraits::ConvertToOem( pszBuffer );
								ReleaseBufferSetLength( nLength );
							}
				}
				__if_exists( StringTraits::ConvertToAnsi )
				{
							void OemToAnsi()
							{
								int nLength = GetLength();
								PXSTR pszBuffer = GetBuffer( nLength );
								StringTraits::ConvertToAnsi( pszBuffer );
								ReleaseBufferSetLength( nLength );
							}
				}
				CExtSafeStringT Mid( int iFirst ) const
				{
					return ( Mid( iFirst, GetLength()-iFirst ) );
				}
				CExtSafeStringT Mid( int iFirst, int nCount ) const
				{
					ATLASSERT( iFirst >= 0 );
					ATLASSERT( nCount >= 0 );
					if( (iFirst+nCount) > GetLength() )
					{
						nCount = GetLength()-iFirst;
					}
					if( iFirst > GetLength() )
					{
						nCount = 0;
					}
					ATLASSERT( (nCount == 0) || ((iFirst+nCount) <= GetLength()) );
					if( (iFirst == 0) && ((iFirst+nCount) == GetLength()) )
					{
						return ( *this );
					}
					return ( CExtSafeStringT( GetString()+iFirst, nCount, GetManager() ) );
				}
				CExtSafeStringT Right( int nCount ) const
				{
					ATLASSERT( nCount >= 0 );
					int nLength = GetLength();
					if( nCount >= nLength )
					{
						return ( *this );
					}
					return ( CExtSafeStringT( GetString()+nLength-nCount, nCount, GetManager() ) );
				}
				CExtSafeStringT Left( int nCount ) const
				{
					ATLASSERT( nCount >= 0 );
					int nLength = GetLength();
					if( nCount >= nLength )
					{
						return ( *this );
					}
					return ( CExtSafeStringT( GetString(), nCount, GetManager() ) );
				}
				CExtSafeStringT SpanIncluding( PCXSTR pszCharSet ) const
				{
					ATLASSERT( AtlIsValidString( ((LPCWSTR)pszCharSet) ) );
					return ( Left( StringTraits::StringSpanIncluding( GetString(), pszCharSet ) ) );
				}
				CExtSafeStringT SpanExcluding( PCXSTR pszCharSet ) const
				{
					ATLASSERT( AtlIsValidString( ((LPCWSTR)pszCharSet) ) );
					return ( Left( StringTraits::StringSpanExcluding( GetString(), pszCharSet ) ) );
 				}
				void __cdecl Format( PCXSTR pszFormat, ... )
				{
					ATLASSERT( AtlIsValidString( ((LPCWSTR)pszFormat) ) );
					va_list argList;
					va_start( argList, pszFormat );
					FormatV( pszFormat, argList );
					va_end( argList );
				}
				void __cdecl Format( UINT nFormatID, ... )
				{
					CExtSafeStringT strFormat( GetManager() );
					ATLVERIFY( strFormat.LoadString( nFormatID ) );
					va_list argList;
					va_start( argList, nFormatID );
					FormatV( strFormat, argList );
					va_end( argList );
				}
				void __cdecl AppendFormat( UINT nFormatID, ... )
				{
					va_list argList;
					va_start( argList, nFormatID );
					CExtSafeStringT strFormat( GetManager() );
					ATLVERIFY( strFormat.LoadString( nFormatID ) ); 
					AppendFormatV( strFormat, argList );
					va_end( argList );
				}
				void __cdecl AppendFormat( PCXSTR pszFormat, ... )
				{
					ATLASSERT( AtlIsValidString( ((LPCWSTR)pszFormat) ) );
					va_list argList;
					va_start( argList, pszFormat );
					AppendFormatV( pszFormat, argList );
					va_end( argList );
				}
				void AppendFormatV( PCXSTR pszFormat, va_list args )
				{
					ATLASSERT( AtlIsValidString( ((LPCWSTR)pszFormat) ) );
					int nCurrentLength = GetLength();
					int nAppendLength = StringTraits::GetFormattedLength( pszFormat, args );
					PXSTR pszBuffer = GetBuffer( nCurrentLength+nAppendLength );
					StringTraits::Format( pszBuffer+nCurrentLength, pszFormat, args );
					ReleaseBufferSetLength( nCurrentLength+nAppendLength );
				}
				void FormatV( PCXSTR pszFormat, va_list args )
				{
					ATLASSERT( AtlIsValidString( ((LPCWSTR)pszFormat) ) );
					int nLength = StringTraits::GetFormattedLength( pszFormat, args );
					PXSTR pszBuffer = GetBuffer( nLength );
					StringTraits::Format( pszBuffer, pszFormat, args );
					ReleaseBufferSetLength( nLength );
				}
				__if_exists(StringTraits::FormatMessage)
				{
							void __cdecl FormatMessage( PCXSTR pszFormat, ... )
							{
								va_list argList;
								va_start( argList, pszFormat );

								FormatMessageV( pszFormat, &argList );

								va_end( argList );
							}
							void __cdecl FormatMessage( UINT nFormatID, ... )
							{
								CExtSafeStringT strFormat( GetManager() );
								ATLVERIFY( strFormat.LoadString( nFormatID ) );
								va_list argList;
								va_start( argList, nFormatID );
								FormatMessageV( strFormat, &argList );
								va_end( argList );
							}
				}
				BSTR AllocSysString() const
				{
					BSTR bstrResult = StringTraits::AllocSysString( GetString(), 
						GetLength() );
					if( bstrResult == NULL )
					{
						ThrowMemoryException();
					}
					return ( bstrResult );
				}
				BSTR SetSysString( BSTR* pbstr ) const
				{
					ATLASSERT( AtlIsValidAddress( pbstr, sizeof(BSTR) ) );
					if( !StringTraits::ReAllocSysString( GetString(), pbstr,
						GetLength() ) )
					{
						ThrowMemoryException();
					}
					ATLASSERT( *pbstr != NULL );
					return ( *pbstr );
				}
				BOOL GetEnvironmentVariable( PCXSTR pszVar )
				{
					ULONG nLength = StringTraits::GetEnvironmentVariable( pszVar, NULL, 0 );
					BOOL bRetVal = FALSE;
					if( nLength == 0 )
					{
						Empty();
					}
					else
					{
						PXSTR pszBuffer = GetBuffer( nLength );
						StringTraits::GetEnvironmentVariable( pszVar, pszBuffer, nLength );
						ReleaseBuffer();
						bRetVal = TRUE;
					}
					return ( bRetVal );
				}
				BOOL LoadString( UINT nID )
				{
					HINSTANCE hInst = StringTraits::FindStringResourceInstance( nID );
					if( hInst == NULL )
					{
						return ( FALSE );
					}
					return ( LoadString( hInst, nID ) );		
				}
				BOOL LoadString( HINSTANCE hInstance, UINT nID )
				{
					const ATLSTRINGRESOURCEIMAGE* pImage = AtlGetStringResourceImage( hInstance, nID );
					if( pImage == NULL )
					{
						return ( FALSE );
					}
					int nLength = StringTraits::GetBaseTypeLength(
						(PCXSTR)pImage->achString,
						pImage->nLength
						);
					PXSTR pszBuffer = GetBuffer( nLength );
					StringTraits::ConvertToBaseType(
						pszBuffer,
						nLength,
						(PCXSTR)pImage->achString,
						pImage->nLength
						);
					ReleaseBufferSetLength( nLength );
					return ( TRUE );
				}
				BOOL LoadString( HINSTANCE hInstance, UINT nID, WORD wLanguageID )
				{
					const ATLSTRINGRESOURCEIMAGE* pImage = AtlGetStringResourceImage( hInstance, nID, wLanguageID );
					if( pImage == NULL )
					{
						return ( FALSE );
					}
					int nLength = StringTraits::GetBaseTypeLength( pImage->achString, pImage->nLength );
					PXSTR pszBuffer = GetBuffer( nLength );
					StringTraits::ConvertToBaseType(
						pszBuffer,
						nLength,
						(StringTraits::PCXSTR)pImage->achString,
						pImage->nLength
						);
					ReleaseBufferSetLength( nLength );
					return ( TRUE );
				}
				friend CExtSafeStringT operator+( const CExtSafeStringT & str1, const CExtSafeStringT & str2 )
				{
					CExtSafeStringT strResult( str1.GetManager() );
					Concatenate( strResult, str1, str1.GetLength(), str2, str2.GetLength() );
					return ( strResult );
				}
				friend CExtSafeStringT operator+( const CExtSafeStringT & str1, PCXSTR psz2 )
				{
					CExtSafeStringT strResult( str1.GetManager() );
					Concatenate( strResult, str1, str1.GetLength(), psz2, StringLength( psz2 ) );
					return ( strResult );
				}
				friend CExtSafeStringT operator+( PCXSTR psz1, const CExtSafeStringT & str2 )
				{
					CExtSafeStringT strResult( str2.GetManager() );
					Concatenate( strResult, psz1, StringLength( psz1 ), str2, str2.GetLength() );
					return ( strResult );
				}
				friend CExtSafeStringT operator+( const CExtSafeStringT & str1, wchar_t ch2 )
				{
					CExtSafeStringT strResult( str1.GetManager() );
					XCHAR chTemp = XCHAR( ch2 );
					Concatenate( strResult, str1, str1.GetLength(), &chTemp, 1 );
					return ( strResult );
				}
				friend CExtSafeStringT operator+( const CExtSafeStringT & str1, char ch2 )
				{
					CExtSafeStringT strResult( str1.GetManager() );
					XCHAR chTemp = XCHAR( ch2 );
					Concatenate( strResult, str1, str1.GetLength(), &chTemp, 1 );
					return ( strResult );
				}
				friend CExtSafeStringT operator+( wchar_t ch1, const CExtSafeStringT & str2 )
				{
					CExtSafeStringT strResult( str2.GetManager() );
					XCHAR chTemp = XCHAR( ch1 );
					Concatenate( strResult, &chTemp, 1, str2, str2.GetLength() );
					return ( strResult );
				}
				friend CExtSafeStringT operator+( char ch1, const CExtSafeStringT & str2 )
				{
					CExtSafeStringT strResult( str2.GetManager() );
					XCHAR chTemp = XCHAR( ch1 );
					Concatenate( strResult, &chTemp, 1, str2, str2.GetLength() );
					return ( strResult );
				}
				friend bool operator==( const CExtSafeStringT & str1, const CExtSafeStringT & str2 ) throw()
				{
					return ( str1.Compare( str2 ) == 0 );
				}
				friend bool operator==(
					const CExtSafeStringT & str1, PCXSTR psz2 ) throw()
				{
					return ( str1.Compare( psz2 ) == 0 );
				}
				friend bool operator==(
					PCXSTR psz1, const CExtSafeStringT & str2 ) throw()
				{
					return ( str2.Compare( psz1 ) == 0 );
				}
				friend bool operator==(
					const CExtSafeStringT & str1, PCYSTR psz2 ) throw( ... )
				{
					CExtSafeStringT str2( psz2, str1.GetManager() );
					return ( str1 == str2 );
				}
				friend bool operator==(
					PCYSTR psz1, const CExtSafeStringT & str2 ) throw( ... )
				{
					CExtSafeStringT str1( psz1, str2.GetManager() );
					return ( str1 == str2 );
				}
				friend bool operator!=(
					const CExtSafeStringT & str1, const CExtSafeStringT & str2 ) throw()
				{
					return ( str1.Compare( str2 ) != 0 );
				}
				friend bool operator!=(
					const CExtSafeStringT & str1, PCXSTR psz2 ) throw()
				{
					return ( str1.Compare( psz2 ) != 0 );
				}
				friend bool operator!=(
					PCXSTR psz1, const CExtSafeStringT & str2 ) throw()
				{
					return ( str2.Compare( psz1 ) != 0 );
				}
				friend bool operator!=(
					const CExtSafeStringT & str1, PCYSTR psz2 ) throw( ... )
				{
					CExtSafeStringT str2( psz2, str1.GetManager() );
					return ( str1 != str2 );
				}
				friend bool operator!=(
					PCYSTR psz1, const CExtSafeStringT & str2 ) throw( ... )
				{
					CExtSafeStringT str1( psz1, str2.GetManager() );
					return ( str1 != str2 );
				}
				friend bool operator<( const CExtSafeStringT & str1, const CExtSafeStringT & str2 ) throw()
				{
					return ( str1.Compare( str2 ) < 0 );
				}
				friend bool operator<( const CExtSafeStringT & str1, PCXSTR psz2 ) throw()
				{
					return ( str1.Compare( psz2 ) < 0 );
				}
				friend bool operator<( PCXSTR psz1, const CExtSafeStringT & str2 ) throw()
				{
					return ( str2.Compare( psz1 ) >= 0 );
				}
				friend bool operator>( const CExtSafeStringT & str1, const CExtSafeStringT & str2 ) throw()
				{
					return ( str1.Compare( str2 ) > 0 );
				}
				friend bool operator>( const CExtSafeStringT & str1, PCXSTR psz2 ) throw()
				{
					return ( str1.Compare( psz2 ) > 0 );
				}
				friend bool operator>( PCXSTR psz1, const CExtSafeStringT & str2 ) throw()
				{
					return ( str2.Compare( psz1 ) <= 0 );
				}
				friend bool operator<=( const CExtSafeStringT & str1, const CExtSafeStringT & str2 ) throw()
				{
					return ( str1.Compare( str2 ) <= 0 );
				}
				friend bool operator<=( const CExtSafeStringT & str1, PCXSTR psz2 ) throw()
				{
					return ( str1.Compare( psz2 ) <= 0 );
				}
				friend bool operator<=( PCXSTR psz1, const CExtSafeStringT & str2 ) throw()
				{
					return ( str2.Compare( psz1 ) > 0 );
				}
				friend bool operator>=( const CExtSafeStringT & str1, const CExtSafeStringT & str2 ) throw()
				{
					return ( str1.Compare( str2 ) >= 0 );
				}
				friend bool operator>=( const CExtSafeStringT & str1, PCXSTR psz2 ) throw()
				{
					return ( str1.Compare( psz2 ) >= 0 );
				}
				friend bool operator>=( PCXSTR psz1, const CExtSafeStringT & str2 ) throw()
				{
					return ( str2.Compare( psz1 ) < 0 );
				}
				friend bool operator==( XCHAR ch1, const CExtSafeStringT & str2 ) throw()
				{
					return ( (str2.GetLength() == 1) && (str2[0] == ch1) );
				}
				friend bool operator==( const CExtSafeStringT & str1, XCHAR ch2 ) throw()
				{
					return ( (str1.GetLength() == 1) && (str1[0] == ch2) );
				}
				friend bool operator!=( XCHAR ch1, const CExtSafeStringT & str2 ) throw()
				{
					return ( (str2.GetLength() != 1) || (str2[0] != ch1) );
				}
				friend bool operator!=( const CExtSafeStringT & str1, XCHAR ch2 ) throw()
				{
					return ( (str1.GetLength() != 1) || (str1[0] != ch2) );
				}
			private:
				bool CheckImplicitLoad( const void* pv )
				{
					bool bRet = false;
					if( (pv != NULL) && IS_INTRESOURCE( pv ) )
					{
						UINT nID = LOWORD( reinterpret_cast< DWORD_PTR >( pv ) );
						if( !LoadString( nID ) )
						{
							ATLTRACE( atlTraceString, 2, _T( "Warning: implicit LoadString(%u) failed\n" ), nID );
						}
						bRet = true;
					}
					return ( bRet );
				}
				__if_exists( StringTraits::FormatMessage )
				{
							void FormatMessageV( PCXSTR pszFormat, va_list* pArgList )
							{
								CHeapPtr< XCHAR, CLocalAllocator > pszTemp;
								DWORD dwResult = StringTraits::FormatMessage( FORMAT_MESSAGE_FROM_STRING|
									FORMAT_MESSAGE_ALLOCATE_BUFFER, pszFormat, 0, 0, reinterpret_cast< PXSTR >( &pszTemp ),
									0, pArgList );
								if( dwResult == 0 )
								{
									ThrowMemoryException();
								}

								*this = pszTemp;
							}
				}
			};

			template < typename BaseType, class StringTraits >
			inline
			CArchive & operator << (
				CArchive& ar,
				const CExtSafeStringT < BaseType, StringTraits > & str
				)
			{
				AfxWriteStringLength(
					ar,
					str.GetLength(),
					sizeof(BaseType) == sizeof(wchar_t)
					);
				ar.Write(
					str,
					str.GetLength() * sizeof(BaseType)
					);
				return ar;
			}

			template < typename BaseType, class StringTraits >
			inline
			CArchive & operator >> (
				CArchive& ar,
				CExtSafeStringT < BaseType, StringTraits > & str
				)
			{
				int nCharSize;
				UINT nLength = UINT( AfxReadStringLength( ar, nCharSize ) );
				if( nCharSize == sizeof(char) )
				{
					ATL::CTempBuffer < char > pszBufferA( nLength + 1 );
					pszBufferA[ nLength ] = '\0';
					UINT nBytesRead =
						ar.Read(
							pszBufferA,
							nLength * sizeof(char)
							);
					if( nBytesRead != ( nLength * sizeof(char) ) )
						AfxThrowArchiveException( CArchiveException::endOfFile );
					str = pszBufferA;
				}
				else
				{
					ASSERT( nCharSize == sizeof(wchar_t) );
					ATL::CTempBuffer< wchar_t > pszBufferW( nLength+1 );
					pszBufferW[ nLength ] = L'\0';
					UINT nBytesRead =
						ar.Read(
							pszBufferW,
							nLength * sizeof(wchar_t)
							);
					if( nBytesRead != ( nLength * sizeof(wchar_t) ) )
						AfxThrowArchiveException( CArchiveException::endOfFile );
					str = (BaseType *)(wchar_t *)pszBufferW;
				}
				return ar;
			}

			typedef
				CExtSafeStrTraitMFC <
					__prof_uis_used_wchar_t,
					CExtSafeChTraitsCRT< __prof_uis_used_wchar_t >
					>
				__prof_uis_used_StrTraitMFC;
			typedef
				CExtSafeStringT <
					__prof_uis_used_wchar_t,
					__prof_uis_used_StrTraitMFC
					>
				__prof_uis_used_CStringT;

			typedef
				CExtSafeStrTraitMFC <
					__prof_uis_converted_wchar_t,
					CExtSafeChTraitsCRT< __prof_uis_used_wchar_t >
					>
				__prof_uis_converted_StrTraitMFC;
			typedef
				CExtSafeStringT <
					__prof_uis_converted_wchar_t,
					__prof_uis_converted_StrTraitMFC
					>
				__prof_uis_converted_CStringT;

			#if (defined _MSC_EXTENSIONS)
				#define __EXT_MFC_TEMPLATED_WCHAR_T_ADAPTOR_NO_VTABLE	__declspec(novtable)
			#else
				#define __EXT_MFC_TEMPLATED_WCHAR_T_ADAPTOR_NO_VTABLE
			#endif

			template < class _c_, class _n_ >
			class __EXT_MFC_TEMPLATED_WCHAR_T_ADAPTOR_NO_VTABLE CExtNWCT
			{
			protected:
				_c_ m_data;
			public:
				typedef CExtNWCT < _c_, _n_ > _Myt;
				CExtNWCT( _c_ data )
					: m_data( data )
				{
				}
				CExtNWCT( _n_ data )
					: m_data( * ( ( _c_ * ) ( &data ) ) )
				{
				}
				CExtNWCT( const int _the_null_val = NULL )
					: m_data( reinterpret_cast < _c_ > ( _the_null_val ) )
				{
				}
				CExtNWCT( const _Myt & other )
					: m_data( other.m_data )
				{
				}
				operator _c_ ()
				{
					return m_data;
				}
				operator _n_ ()
				{
					return * ( ( _n_ * ) ( &m_data ) );
				}
				operator const _c_ () const
				{
					return m_data;
				}
				operator const _n_ () const
				{
					return * ( ( _n_ * ) ( &m_data ) );
				}
				_Myt & operator = ( const _Myt & other )
				{
					m_data = other.m_data;
					return (*this);
				}
				_Myt & operator = ( _c_ data )
				{
					m_data = data;
					return (*this);
				}
				_Myt & operator = ( _n_ data )
				{
					m_data = * ( ( _c_ * ) ( &data ) );
					return (*this);
				}
				_Myt & operator = ( const int _the_null_val )
				{
					m_data = ( reinterpret_cast < _c_ > ( _the_null_val ) );
					return (*this);
				}
				bool operator == ( const _Myt & other ) const
				{
					return (m_data == other.m_data) ? true : false;
				}
				bool operator == ( const _c_ & data ) const
				{
					return (m_data == data) ? true : false;
				}
				bool operator == ( const _n_ & data ) const
				{
					return (m_data == ( * ( ( _c_ * ) ( &data ) ) )) ? true : false;
				}
				bool operator == ( const int _the_null_val ) const
				{
					return (m_data == ( reinterpret_cast < _c_ > ( _the_null_val ) ) ) ? true : false;
				}
				bool operator != ( const _Myt & other ) const
				{
					return (m_data != other.m_data) ? true : false;
				}
				bool operator != ( const _c_ & data ) const
				{
					return (m_data != data) ? true : false;
				}
				bool operator != ( const _n_ & data ) const
				{
					return (m_data != ( * ( ( _c_ * ) ( &data ) ) )) ? true : false;
				}
				bool operator != ( const int _the_null_val ) const
				{
					return (m_data != ( reinterpret_cast < _c_ > ( _the_null_val ) ) ) ? true : false;
				}
				bool operator <= ( const _Myt & other ) const
				{
					return (m_data <= other.m_data) ? true : false;
				}
				bool operator <= ( const _c_ & data ) const
				{
					return (m_data <= data) ? true : false;
				}
				bool operator <= ( const _n_ & data ) const
				{
					return (m_data <= ( * ( ( _c_ * ) ( &data ) ) )) ? true : false;
				}
				bool operator >= ( const _Myt & other ) const
				{
					return (m_data >= other.m_data) ? true : false;
				}
				bool operator >= ( const _c_ & data ) const
				{
					return (m_data >= data) ? true : false;
				}
				bool operator >= ( const _n_ & data ) const
				{
					return (m_data >= ( * ( ( _c_ * ) ( &data ) ) )) ? true : false;
				}
				bool operator < ( const _Myt & other ) const
				{
					return (m_data < other.m_data) ? true : false;
				}
				bool operator < ( const _c_ & data ) const
				{
					return (m_data < data) ? true : false;
				}
				bool operator < ( const _n_ & data ) const
				{
					return (m_data < ( * ( ( _c_ * ) ( &data ) ) )) ? true : false;
				}
				bool operator > ( const _Myt & other ) const
				{
					return (m_data > other.m_data) ? true : false;
				}
				bool operator > ( const _c_ & data ) const
				{
					return (m_data > data) ? true : false;
				}
				bool operator > ( const _n_ & data ) const
				{
					return (m_data > ( * ( ( _c_ * ) ( &data ) ) )) ? true : false;
				}

				operator bool () const
				{
					return (m_data == reinterpret_cast < _c_ > ( NULL )) ? false : true;
				}
//				operator int () const
//				{
//					return (int)m_data;
//				}
//				operator long () const
//				{
//					return (long)m_data;
//				}
//				operator unsigned int () const
//				{
//					return (unsigned int)m_data;
//				}
//				operator unsigned long () const
//				{
//					return (unsigned long)m_data;
//				}
			}; // class CExtNWCT

			template < class _c_, class _n_, class _a_ >
			class __EXT_MFC_TEMPLATED_WCHAR_T_ADAPTOR_NO_VTABLE CExtNWCTA
				: public CExtNWCT < _c_, _n_ >
			{
			public:
				typedef CExtNWCTA < _c_, _n_, _a_ > _Myt;
				typedef CExtNWCT < _c_, _n_ > _Mybt;
				CExtNWCTA( _c_ data )
					: _Mybt( data )
				{
				}
				CExtNWCTA( _n_ data )
					: _Mybt( data )
				{
				}
				CExtNWCTA( const int _the_null_val = NULL )
					: _Mybt( _the_null_val )
				{
				}
				CExtNWCTA( const _Myt & other )
					: _Mybt( other )
				{
				}

				CExtNWCTA( const __prof_uis_used_CStringT & other )
					: _Mybt( ( _i_ )( other.GetString() ) )
				{
				}
				CExtNWCTA( const __prof_uis_converted_CStringT & other )
					: _Mybt( ( _i_ )( other.GetString() ) )
				{
				}
				
//				_Myt & operator = ( const __prof_uis_used_CStringT & other )
//				{
//					return _Mybt::operator=( ( _i_ )( other.GetString() ) );
//				}
//				_Myt & operator = ( const __prof_uis_converted_CStringT & other )
//				{
//					return _Mybt::operator=( ( _i_ )( other.GetString() ) );
//				}

				operator __prof_uis_used_CStringT () const
				{
					return __prof_uis_used_CStringT( (__prof_uis_used_CStringT::XCHAR *)m_data );
				}
				operator __prof_uis_converted_CStringT () const
				{
					return __prof_uis_converted_CStringT( (__prof_uis_converted_CStringT::XCHAR *)m_data );
				}

				_a_ operator[] ( int nIndex ) const
				{
					return ( ( _a_ ) ( m_data[ nIndex ] ) );
				}
			}; // class CExtNWCTA

			template < class _c_, class _i_, class _n_ >
			class __EXT_MFC_TEMPLATED_WCHAR_T_ADAPTOR_NO_VTABLE CExtNWCTC
			{
			protected:
				_i_ m_data;
			public:
				typedef CExtNWCTC < _c_, _i_, _n_ > _Myt;
				CExtNWCTC( _c_ data )
					: m_data( const_cast < _i_ > ( data ) )
				{
				}
				CExtNWCTC( _n_ data )
					: m_data( * ( ( _i_ * ) ( &data ) ) )
				{
				}
				CExtNWCTC( const int _the_null_val = NULL )
					: m_data( reinterpret_cast < _i_ > ( _the_null_val ) )
				{
				}
				CExtNWCTC( const _Myt & other )
					: m_data( other.m_data )
				{
				}
				operator _c_ () const
				{
					return m_data;
				}
				operator _n_ () const
				{
					return * ( ( _n_ * ) ( &m_data ) );
				}
				_Myt & operator = ( const _Myt & other )
				{
					m_data = other.m_data;
					return (*this);
				}
				_Myt & operator = ( _c_ data  )
				{
					m_data = const_cast < _i_ > ( data );
					return (*this);
				}
				_Myt & operator = ( _n_ data )
				{
					m_data = * ( ( _i_ * ) ( &data ) );
					return (*this);
				}
				_Myt & operator = ( const int _the_null_val )
				{
					m_data = ( reinterpret_cast < _i_ > ( _the_null_val ) );
					return (*this);
				}
				bool operator == ( const _Myt & other ) const
				{
					return (m_data == other.m_data) ? true : false;
				}
				bool operator == ( _c_ & data  ) const
				{
					return (m_data == ( const_cast < _i_ > ( data ) ) ) ? true : false;
				}
				bool operator == ( _n_ & data ) const
				{
					return (m_data == ( * ( ( _i_ * ) ( &data ) )) ) ? true : false;
				}
				bool operator == ( const int _the_null_val ) const
				{
					return (m_data == ( reinterpret_cast < _i_ > ( _the_null_val ) ) ) ? true : false;
				}
				bool operator != ( const _Myt & other ) const
				{
					return (m_data != other.m_data) ? true : false;
				}
				bool operator != ( _c_ & data  ) const
				{
					return (m_data != ( const_cast < _i_ > ( data ) ) ) ? true : false;
				}
				bool operator != ( _n_ & data ) const
				{
					return (m_data != ( * ( ( _i_ * ) ( &data ) )) ) ? true : false;
				}
				bool operator != ( const int _the_null_val ) const
				{
					return (m_data != ( reinterpret_cast < _i_ > ( _the_null_val ) ) ) ? true : false;
				}
				bool operator <= ( const _Myt & other ) const
				{
					return (m_data <= other.m_data) ? true : false;
				}
				bool operator <= ( _c_ & data  ) const
				{
					return (m_data <= ( const_cast < _i_ > ( data ) ) ) ? true : false;
				}
				bool operator <= ( _n_ & data ) const
				{
					return (m_data <= ( * ( ( _i_ * ) ( &data ) )) ) ? true : false;
				}
				bool operator >= ( const _Myt & other ) const
				{
					return (m_data >= other.m_data) ? true : false;
				}
				bool operator >= ( _c_ & data  ) const
				{
					return (m_data >= ( const_cast < _i_ > ( data ) ) ) ? true : false;
				}
				bool operator >= ( _n_ & data ) const
				{
					return (m_data >= ( * ( ( _i_ * ) ( &data ) )) ) ? true : false;
				}
				bool operator < ( const _Myt & other ) const
				{
					return (m_data < other.m_data) ? true : false;
				}
				bool operator < ( _c_ & data  ) const
				{
					return (m_data < ( const_cast < _i_ > ( data ) ) ) ? true : false;
				}
				bool operator < ( _n_ & data ) const
				{
					return (m_data < ( * ( ( _i_ * ) ( &data ) )) ) ? true : false;
				}
				bool operator > ( const _Myt & other ) const
				{
					return (m_data > other.m_data) ? true : false;
				}
				bool operator > ( _c_ & data  ) const
				{
					return (m_data > ( const_cast < _i_ > ( data ) ) ) ? true : false;
				}
				bool operator > ( _n_ & data ) const
				{
					return (m_data > ( * ( ( _i_ * ) ( &data ) )) ) ? true : false;
				}
			
				operator bool () const
				{
					return (m_data == reinterpret_cast < _i_ > ( NULL )) ? false : true;
				}
//				operator int () const
//				{
//					return (int)m_data;
//				}
//				operator long () const
//				{
//					return (long)m_data;
//				}
//				operator unsigned int () const
//				{
//					return (unsigned int)m_data;
//				}
//				operator unsigned long () const
//				{
//					return (unsigned long)m_data;
//				}
			}; // class CExtNWCTC

			template < class _c_, class _i_, class _n_, class _a_ >
			class __EXT_MFC_TEMPLATED_WCHAR_T_ADAPTOR_NO_VTABLE CExtNWCTCA
				: public CExtNWCTC < _c_, _i_, _n_ >
			{
			public:
				typedef CExtNWCTCA < _c_, _i_, _n_, _a_ > _Myt;
				typedef CExtNWCTC < _c_, _i_, _n_ > _Mybt;
				CExtNWCTCA( _c_ data )
					: _Mybt( data )
				{
				}
				CExtNWCTCA( _n_ data )
					: _Mybt( data )
				{
				}
				CExtNWCTCA( const int _the_null_val = NULL )
					: _Mybt( _the_null_val )
				{
				}
				CExtNWCTCA( const _Myt & other )
					: _Mybt( other )
				{
				}

				CExtNWCTCA( const __prof_uis_used_CStringT & other )
					: _Mybt( ( _i_ )( other.GetString() ) )
				{
				}
				CExtNWCTCA( const __prof_uis_converted_CStringT & other )
					: _Mybt( ( _i_ )( other.GetString() ) )
				{
				}
				
//				_Myt & operator = ( const __prof_uis_used_CStringT & other )
//				{
//					return _Mybt::operator=( ( _i_ )( other.GetString() ) );
//				}
//				_Myt & operator = ( const __prof_uis_converted_CStringT & other )
//				{
//					return _Mybt::operator=( ( _i_ )( other.GetString() ) );
//				}

				operator __prof_uis_used_CStringT () const
				{
					return __prof_uis_used_CStringT( (__prof_uis_used_CStringT::PCXSTR)m_data );
				}
				operator __prof_uis_converted_CStringT () const
				{
					return __prof_uis_converted_CStringT( (__prof_uis_converted_CStringT::PCXSTR)m_data );
				}

				_a_ operator[] ( int nIndex ) const
				{
					return ( ( _a_ ) ( m_data[ nIndex ] ) );
				}
			}; // CExtNWCTCA

			typedef CExtNWCT   <       unsigned short     ,                            __wchar_t                               > __EXT_MFC_SAFE_TCHAR;
			typedef CExtNWCTC  < const unsigned short     , unsigned short     , const __wchar_t                               > __EXT_MFC_SAFE_CONST_TCHAR;
			typedef CExtNWCTA  <       unsigned short *   ,                            __wchar_t *   , __prof_uis_used_wchar_t > __EXT_MFC_SAFE_LPTSTR;
			typedef CExtNWCTCA < const unsigned short *   , unsigned short *   , const __wchar_t *   , __prof_uis_used_wchar_t > __EXT_MFC_SAFE_LPCTSTR;
			typedef CExtNWCT   <       unsigned short   & ,                            __wchar_t   &                           > __EXT_MFC_SAFE_TCHAR_REF;
			typedef CExtNWCTC  < const unsigned short   & , unsigned short   & , const __wchar_t   &                           > __EXT_MFC_SAFE_CONST_TCHAR_REF;
			typedef CExtNWCTA  <       unsigned short * & ,                            __wchar_t * & , __prof_uis_used_wchar_t > __EXT_MFC_SAFE_LPTSTR_REF;
			typedef CExtNWCTCA < const unsigned short * & , unsigned short * & , const __wchar_t * & , __prof_uis_used_wchar_t > __EXT_MFC_SAFE_LPCTSTR_REF;

			typedef CStringA CExtSafeStringA; // allways same as MFC's

			class CExtSafeStringW : public __prof_uis_used_CStringT
			{
			public:
				CExtSafeStringW()
				{
				}
//				CExtSafeStringW( __EXT_MFC_SAFE_LPTSTR pszSrc )
//				{
//					const __prof_uis_used_wchar_t * ptr = pszSrc;
//					(*this) = ptr;
//				}
				CExtSafeStringW( __EXT_MFC_SAFE_LPCTSTR pszSrc )
				{
					const __prof_uis_used_wchar_t * ptr = pszSrc;
					(*this) = ptr;
				}
				explicit CExtSafeStringW( IAtlStringMgr * pStringMgr ) throw()
					: __prof_uis_used_CStringT( pStringMgr )
				{
				}
//				CExtSafeStringW( const VARIANT & varSrc )
//					: __prof_uis_used_CStringT(  varSrc )
//				{
//				}
//				CExtSafeStringW( const VARIANT & varSrc, IAtlStringMgr * pStringMgr )
//					: __prof_uis_used_CStringT( varSrc, pStringMgr )
//				{
//				}
				CExtSafeStringW( const CExtSafeStringW & strSrc )
					: __prof_uis_used_CStringT( strSrc )
				{
				}
				CExtSafeStringW( const __prof_uis_used_CStringT & strSrc )
					: __prof_uis_used_CStringT( strSrc )
				{
				}
				CExtSafeStringW( const XCHAR * pszSrc )
					: __prof_uis_used_CStringT( pszSrc )
				{
				}
				CExtSafeStringW( LPCSTR pszSrc, IAtlStringMgr * pStringMgr )
					: __prof_uis_used_CStringT( pszSrc, pStringMgr )
				{
				}
				CSTRING_EXPLICIT CExtSafeStringW( const YCHAR * pszSrc )
					: __prof_uis_used_CStringT( pszSrc )
				{
				}
				CExtSafeStringW( LPCWSTR pszSrc, IAtlStringMgr * pStringMgr )
					: __prof_uis_used_CStringT( pszSrc, pStringMgr )
				{
				}
				CSTRING_EXPLICIT CExtSafeStringW( const unsigned char * pszSrc )
					: __prof_uis_used_CStringT( pszSrc )
				{
				}
				CExtSafeStringW( const unsigned char * pszSrc, IAtlStringMgr * pStringMgr )
					: __prof_uis_used_CStringT( pszSrc, pStringMgr )
				{
				}
				CSTRING_EXPLICIT CExtSafeStringW( char ch, int nLength = 1 )
					: __prof_uis_used_CStringT( ch, nLength )
				{
				}
				CSTRING_EXPLICIT CExtSafeStringW( __prof_uis_used_wchar_t ch, int nLength = 1 )
					: __prof_uis_used_CStringT( ((XCHAR *)(&ch)), nLength )
				{
				}
				CSTRING_EXPLICIT CExtSafeStringW( __prof_uis_converted_wchar_t ch, int nLength = 1 )
					: __prof_uis_used_CStringT( ((XCHAR *)(&ch)), nLength )
				{
				}
				CExtSafeStringW( const XCHAR * pch, int nLength )
					: __prof_uis_used_CStringT( pch, nLength )
				{
				}
				CExtSafeStringW( const XCHAR * pch, int nLength, IAtlStringMgr * pStringMgr )
					: __prof_uis_used_CStringT( pch, nLength, pStringMgr )
				{
				}
				CExtSafeStringW( const YCHAR * pch, int nLength )
					: __prof_uis_used_CStringT( pch, nLength )
				{
				}
				CExtSafeStringW( const YCHAR * pch, int nLength, IAtlStringMgr * pStringMgr )
					: __prof_uis_used_CStringT( pch, nLength, pStringMgr )
				{
				}

				operator __prof_uis_converted_wchar_t * ()
				{
					LPVOID pData = (LPVOID)GetString();
					return ( (__prof_uis_converted_wchar_t *) pData );
				}
				operator const __prof_uis_converted_wchar_t * () const
				{
					LPVOID pData = (LPVOID)GetString();
					return ( (__prof_uis_converted_wchar_t *) pData );
				}
				
				operator __prof_uis_used_wchar_t * ()
				{
					LPVOID pData = (LPVOID)GetString();
					return ( (__prof_uis_used_wchar_t *) pData );
				}
				operator const __prof_uis_used_wchar_t * () const
				{
					LPVOID pData = (LPVOID)GetString();
					return ( (__prof_uis_used_wchar_t *) pData );
				}
				
				operator __EXT_MFC_SAFE_LPTSTR ()
				{
					LPVOID pData = (LPVOID)GetString();
					return __EXT_MFC_SAFE_LPTSTR( (__prof_uis_used_wchar_t *) pData );
				}
				operator __EXT_MFC_SAFE_LPCTSTR () const
				{
					LPVOID pData = (LPVOID)GetString();
					return __EXT_MFC_SAFE_LPCTSTR( (__prof_uis_used_wchar_t *) pData );
				}

				operator __prof_uis_converted_CStringT & ()
				{
					return * ( ( __prof_uis_converted_CStringT * ) ( this ) );
				}
				operator const __prof_uis_converted_CStringT & () const
				{
					return * ( ( __prof_uis_converted_CStringT * ) ( this ) );
				}

				CExtSafeStringW & operator = (
					const __prof_uis_used_wchar_t * other
					)
				{
					__prof_uis_used_CStringT::operator=( other );
					return (*this);
				}
				CExtSafeStringW & operator = (
					const __prof_uis_converted_wchar_t * other
					)
				{
					return ( operator=( (__prof_uis_used_wchar_t *)other ) );
				}
				CExtSafeStringW & operator = ( __prof_uis_converted_wchar_t ch )
				{
					__prof_uis_used_wchar_t ach[2] = { ((__prof_uis_used_wchar_t)ch), 0 };
					return ( operator=( reinterpret_cast < PCXSTR > ( ach ) ) );
				}
				CExtSafeStringW & operator = ( __prof_uis_used_wchar_t ch )
				{
					__prof_uis_used_wchar_t ach[2] = { ((__prof_uis_used_wchar_t)ch), 0 };
					return ( operator=( reinterpret_cast < PCXSTR > ( ach ) ) );
				}

				CExtSafeStringW & operator = ( const __prof_uis_used_CStringT & other )
				{
					return ( operator=( reinterpret_cast < PCXSTR > ( other.GetString() ) ) );
				}
				CExtSafeStringW & operator = ( const __prof_uis_converted_CStringT & other )
				{
					return ( operator=( reinterpret_cast < PCXSTR > ( other.GetString() ) ) );
				}

				CExtSafeStringW & operator = (
					__EXT_MFC_SAFE_LPTSTR s
					)
				{
					return ( operator=( ( const __prof_uis_used_wchar_t * ) ( s ) ) );
				}
				CExtSafeStringW & operator = (
					__EXT_MFC_SAFE_LPCTSTR s
					)
				{
					return ( operator=( ( const __prof_uis_used_wchar_t * ) ( s ) ) );
				}

				friend CExtSafeStringW operator + (
					const __prof_uis_used_CStringT & str1,
					__prof_uis_converted_wchar_t * psz2
					)
				{
					return
// disable warning 4675
#pragma warning( push )
#pragma warning ( disable : 4675 )
						operator+(
							str1,
							reinterpret_cast < PCXSTR > ( psz2 )
							);
// rollback warning 4675
#pragma warning( pop )
				}
				friend CExtSafeStringW operator + (
					__prof_uis_converted_wchar_t * psz1,
					const __prof_uis_used_CStringT & str2
					)
				{
// disable warning 4675
#pragma warning( push )
#pragma warning ( disable : 4675 )
					return
						operator+(
							reinterpret_cast < PCXSTR > ( psz1 ),
							str2
							);
// rollback warning 4675
#pragma warning( pop )
				}
				CExtSafeStringW & operator += ( const char * pszSrc )
				{
					__prof_uis_used_CStringT::operator+=( pszSrc );
					return (*this);
				}
				CExtSafeStringW & operator += ( const __prof_uis_converted_wchar_t * pszSrc )
				{
					__prof_uis_used_CStringT::operator+=( reinterpret_cast < PCXSTR > ( pszSrc ) );
					return (*this);
				}
				CExtSafeStringW & operator += ( const CExtSafeStringW & s )
				{
					__prof_uis_used_CStringT::operator+=( s );
					return (*this);
				}
				CExtSafeStringW & operator += ( char ch )
				{
					__prof_uis_used_CStringT::operator+=( ch );
					return (*this);
				}
				CExtSafeStringW & operator += ( __prof_uis_used_wchar_t ch )
				{
					__prof_uis_used_CStringT::operator+=( (unsigned char)ch );
					return (*this);
				}
				CExtSafeStringW & operator += ( __prof_uis_converted_wchar_t ch )
				{
					__prof_uis_used_CStringT::operator+=( (unsigned char)ch );
					return (*this);
				}

				friend CExtSafeStringW operator + (
					const __prof_uis_used_CStringT & str1,
					const char * psz2
					)
				{
// disable warning 4675
#pragma warning( push )
#pragma warning ( disable : 4675 )
					return
						operator+(
							str1,
							CExtSafeStringW( psz2 )
							);
// rollback warning 4675
#pragma warning( pop )
				}
				friend CExtSafeStringW operator + (
					const __prof_uis_used_CStringT & str1,
					const __prof_uis_converted_wchar_t * psz2
					)
				{
// disable warning 4675
#pragma warning( push )
#pragma warning ( disable : 4675 )
					return
						operator+(
							str1,
							reinterpret_cast < PCXSTR > ( psz2 )
							);
// rollback warning 4675
#pragma warning( pop )
				}
				friend CExtSafeStringW operator + (
					const char * psz1,
					const __prof_uis_used_CStringT & str2
					)
				{
					return
						operator+(
							CExtSafeStringW( psz1 ),
							str2
							);
				}
				friend CExtSafeStringW operator + (
					const __prof_uis_converted_wchar_t * psz1,
					const __prof_uis_used_CStringT & str2
					)
				{
					return
						operator+(
							reinterpret_cast < PCXSTR > ( psz1 ),
							str2
							);
				}
//				friend CExtSafeStringW operator + (
//					const __prof_uis_used_CStringT & str1,
//					__prof_uis_converted_wchar_t ch2
//					)
//				{
//					return
//						operator+(
//							str1,
//							* ( reinterpret_cast < XCHAR * > ( &ch2 ) )
//							);
//				}
//				friend CExtSafeStringW operator + (
//					__prof_uis_converted_wchar_t ch1,
//					const __prof_uis_used_CStringT & str2
//					)
//				{
//					return
//						operator+(
//							* ( reinterpret_cast < XCHAR * > ( &ch1 ) ),
//							str2
//							);
//				}
				friend bool operator == (
					const __prof_uis_used_CStringT & str1,
					const __prof_uis_converted_wchar_t * psz2
					) throw()
				{
					return ( str1.Compare( reinterpret_cast < PCXSTR > ( psz2 ) ) == 0 );
				}
				friend bool operator == (
					const __prof_uis_converted_wchar_t * psz1,
					const __prof_uis_used_CStringT & str2
					) throw()
				{
					return ( str2.Compare( reinterpret_cast < PCXSTR > ( psz1 ) ) == 0 );
				}
				friend bool operator != (
					const __prof_uis_used_CStringT & str1,
					const __prof_uis_converted_wchar_t * psz2
					) throw()
				{
					return ( str1.Compare( reinterpret_cast < PCXSTR > ( psz2 ) ) != 0 );
				}
				friend bool operator != (
					const __prof_uis_converted_wchar_t * psz1,
					const __prof_uis_used_CStringT & str2
					) throw()
				{
					return ( str2.Compare( reinterpret_cast < PCXSTR > ( psz1 ) ) != 0 );
				}
				friend bool operator < (
					const __prof_uis_used_CStringT & str1,
					const __prof_uis_converted_wchar_t * psz2
					) throw()
				{
					return ( str1.Compare( reinterpret_cast < PCXSTR > ( psz2 ) ) < 0 );
				}
				friend bool operator < (
					const __prof_uis_converted_wchar_t * psz1,
					const __prof_uis_used_CStringT & str2
					) throw()
				{
					return ( str2.Compare( reinterpret_cast < PCXSTR > ( psz1 ) ) >= 0 );
				}
				friend bool operator > (
					const __prof_uis_used_CStringT & str1,
					const __prof_uis_converted_wchar_t * psz2
					) throw()
				{
					return ( str1.Compare( reinterpret_cast < PCXSTR > ( psz2 ) ) > 0 );
				}
				friend bool operator > (
					const __prof_uis_converted_wchar_t * psz1,
					const __prof_uis_used_CStringT & str2
					) throw()
				{
					return ( str2.Compare( reinterpret_cast < PCXSTR > ( psz1 ) ) <= 0 );
				}
				friend bool operator <= (
					const __prof_uis_used_CStringT & str1,
					const __prof_uis_converted_wchar_t * psz2
					) throw()
				{
					return ( str1.Compare( reinterpret_cast < PCXSTR > ( psz2 ) ) <= 0 );
				}
				friend bool operator <= (
					const __prof_uis_converted_wchar_t * psz1, 
					const __prof_uis_used_CStringT & str2
					) throw()
				{
					return ( str2.Compare( reinterpret_cast < PCXSTR > ( psz1 ) ) > 0 );
				}
				friend bool operator >= (
					const __prof_uis_used_CStringT & str1,
					const __prof_uis_converted_wchar_t * psz2
					) throw()
				{
					return ( str1.Compare( reinterpret_cast < PCXSTR > ( psz2 ) ) >= 0 );
				}
				friend bool operator >= (
					const __prof_uis_converted_wchar_t * psz1,
					const __prof_uis_used_CStringT & str2
					) throw()
				{
					return ( str2.Compare( reinterpret_cast < PCXSTR > ( psz1 ) ) < 0 );
				}
				friend bool operator == (
					__prof_uis_converted_wchar_t ch1,
					const __prof_uis_used_CStringT & str2
					) throw()
				{
					return ( (str2.GetLength() == 1) && (str2[0] == ( * ( reinterpret_cast < XCHAR * > ( &ch1 ) ) )) );
				}
				friend bool operator == (
					const __prof_uis_used_CStringT & str1,
					__prof_uis_converted_wchar_t ch2
					) throw()
				{
					return ( (str1.GetLength() == 1) && (str1[0] == ( * ( reinterpret_cast < XCHAR * > ( &ch2 ) ) )) );
				}
				friend bool operator != (
					__prof_uis_converted_wchar_t ch1,
					const __prof_uis_used_CStringT & str2
					) throw()
				{
					return ( (str2.GetLength() != 1) || (str2[0] != ( * ( reinterpret_cast < XCHAR * > ( &ch1 ) ) )) );
				}
				friend bool operator != (
					const __prof_uis_used_CStringT & str1,
					__prof_uis_converted_wchar_t ch2
					) throw()
				{
					return ( (str1.GetLength() != 1) || (str1[0] != ( * ( reinterpret_cast < XCHAR * > ( &ch2 ) ) )) );
				}
				void __cdecl Format( const __prof_uis_converted_wchar_t * pszFormat, ... )
				{
					ATLASSERT( AtlIsValidString( ((LPCWSTR)pszFormat) ) );
					va_list argList;
					va_start( argList, pszFormat );
					FormatV( ((PCXSTR)pszFormat), argList );
					va_end( argList );
				}
				void __cdecl Format( const __prof_uis_used_wchar_t * pszFormat, ... )
				{
					ATLASSERT( AtlIsValidString( ((LPCWSTR)pszFormat) ) );
					va_list argList;
					va_start( argList, pszFormat );
					FormatV( ((PCXSTR)pszFormat), argList );
					va_end( argList );
				}
			}; // class CExtSafeStringW

			typedef CExtSafeStringW CExtSafeString;

			class CExtSafeStringArray : public CStringArray
			{
			public:
				const CExtSafeString & GetAt( INT_PTR nIndex ) const
				{
					const CString & s = CStringArray::GetAt( nIndex );
					return * ( ( CExtSafeString * ) ( & s ) ); // Xtra-dirty-cast
				}
				void SetAt(
					INT_PTR nIndex,
					__EXT_MFC_SAFE_LPCTSTR newElement
					)
				{
					CStringArray::SetAt(
						nIndex,
						(LPCTSTR)newElement
						);
				}
				void SetAt(
					INT_PTR nIndex,
					const CExtSafeString & newElement
					)
				{
					const CString & s = * ( ( CString * ) ( & newElement ) ); // Xtra-dirty-cast
					CStringArray::SetAt(
						nIndex,
						s
						);
				}
				CExtSafeString & ElementAt( INT_PTR nIndex )
				{
					CString & s = CStringArray::ElementAt( nIndex );
					return * ( ( CExtSafeString * ) ( & s ) ); // Xtra-dirty-cast
				}
				const CExtSafeString * GetData() const
				{
					const CString * s = CStringArray::GetData();
					return ( ( CExtSafeString * ) (  s ) ); // Xtra-dirty-cast
				}
				CExtSafeString * GetData()
				{
					CString * s = CStringArray::GetData();
					return ( ( CExtSafeString * ) (  s ) ); // Xtra-dirty-cast
				}
				void SetAtGrow(
					INT_PTR nIndex,
					__EXT_MFC_SAFE_LPCTSTR newElement
					)
				{
					CStringArray::SetAtGrow(
						nIndex,
						(LPCTSTR)newElement
						);
				}
				void SetAtGrow(
					INT_PTR nIndex,
					const CExtSafeString & newElement
					)
				{
					CStringArray::SetAtGrow(
						nIndex,
						* ( ( CString * ) ( & newElement ) ) // Xtra-dirty-cast
						);
				}
				INT_PTR Add( __EXT_MFC_SAFE_LPCTSTR newElement )
				{
					return
						CStringArray::Add(
							(LPCTSTR)newElement
							);
				}
				INT_PTR Add( const CExtSafeString & newElement )
				{
					return
						CStringArray::Add(
							* ( ( CString * ) ( & newElement ) ) // Xtra-dirty-cast
							);
				}
				const CExtSafeString & operator [] ( INT_PTR nIndex ) const
				{
					const CString & s = CStringArray::operator[]( nIndex );
					return * ( ( CExtSafeString * ) ( & s ) ); // Xtra-dirty-cast
				}
				CExtSafeString & operator [] ( INT_PTR nIndex )
				{
					CString & s = CStringArray::operator[]( nIndex );
					return * ( ( CExtSafeString * ) ( & s ) ); // Xtra-dirty-cast
				}
				void InsertAt(
					INT_PTR nIndex,
					__EXT_MFC_SAFE_LPCTSTR newElement,
					INT_PTR nCount = 1
					)
				{
					CStringArray::InsertAt(
						nIndex,
						(LPCTSTR)newElement,
						nCount
						);
				}
				void InsertAt(
					INT_PTR nIndex,
					const CString & newElement,
					INT_PTR nCount = 1
					)
				{
					CStringArray::InsertAt(
						nIndex,
						* ( ( CString * ) ( & newElement ) ), // Xtra-dirty-cast
						nCount
						);
				}
			}; // class CExtSafeStringArray

			class CExtSafeStringList : public CStringList
			{
			public:
				CExtSafeStringList( INT_PTR nBlockSize = 10 )
					: CStringList( nBlockSize )
				{
				}
				CExtSafeString & GetHead()
				{
					CString & s = CStringList::GetHead();
					return * ( ( CExtSafeString * ) ( & s ) ); // Xtra-dirty-cast
				}
				const CExtSafeString & GetHead() const
				{
					const CString & s = CStringList::GetHead();
					return * ( ( CExtSafeString * ) ( & s ) ); // Xtra-dirty-cast
				}
				CExtSafeString & GetTail()
				{
					CString & s = CStringList::GetTail();
					return * ( ( CExtSafeString * ) ( & s ) ); // Xtra-dirty-cast
				}
				const CExtSafeString & GetTail() const
				{
					const CString & s = CStringList::GetTail();
					return * ( ( CExtSafeString * ) ( & s ) ); // Xtra-dirty-cast
				}
				CExtSafeString RemoveHead()
				{
					return CExtSafeString( (LPCTSTR)CStringList::RemoveHead() );
				}
				CExtSafeString RemoveTail()
				{
					return CExtSafeString( (LPCTSTR)CStringList::RemoveTail() );
				}
				POSITION AddHead( __EXT_MFC_SAFE_LPCTSTR newElement )
				{
					return CStringList::AddHead( (LPCTSTR)newElement );
				}
				POSITION AddTail( __EXT_MFC_SAFE_LPCTSTR newElement )
				{
					return CStringList::AddTail( (LPCTSTR)newElement );
				}
				POSITION AddHead( const CExtSafeString & newElement )
				{
					return CStringList::AddHead( (LPCTSTR)newElement );
				}
				POSITION AddTail( const CExtSafeString & newElement )
				{
					return CStringList::AddTail( (LPCTSTR)newElement );
				}
				CExtSafeString & GetNext( POSITION & rPosition )
				{
					CString & s = CStringList::GetNext( rPosition );
					return * ( ( CExtSafeString * ) ( & s ) ); // Xtra-dirty-cast
				}
				const CExtSafeString & GetNext( POSITION & rPosition ) const
				{
					const CString & s = CStringList::GetNext( rPosition );
					return * ( ( CExtSafeString * ) ( & s ) ); // Xtra-dirty-cast
				}
				CExtSafeString & GetPrev( POSITION & rPosition )
				{
					CString & s = CStringList::GetPrev( rPosition );
					return * ( ( CExtSafeString * ) ( & s ) ); // Xtra-dirty-cast
				}
				const CExtSafeString & GetPrev( POSITION & rPosition ) const
				{
					const CString & s = CStringList::GetPrev( rPosition );
					return * ( ( CExtSafeString * ) ( & s ) ); // Xtra-dirty-cast
				}
				CExtSafeString & GetAt( POSITION position )
				{
					CString & s = CStringList::GetAt( position );
					return * ( ( CExtSafeString * ) ( & s ) ); // Xtra-dirty-cast
				}
				const CExtSafeString & GetAt( POSITION position ) const
				{
					const CString & s = CStringList::GetAt( position );
					return * ( ( CExtSafeString * ) ( & s ) ); // Xtra-dirty-cast
				}
				void SetAt( POSITION pos, __EXT_MFC_SAFE_LPCTSTR newElement )
				{
					CStringList::SetAt( pos, (LPCTSTR)newElement );
				}
				void SetAt( POSITION pos, const CExtSafeString & newElement )
				{
					CStringList::SetAt( pos, (LPCTSTR)newElement );
				}
				POSITION InsertBefore( POSITION position, __EXT_MFC_SAFE_LPCTSTR newElement )
				{
					return CStringList::InsertBefore( position, (LPCTSTR)newElement );
				}
				POSITION InsertAfter( POSITION position, __EXT_MFC_SAFE_LPCTSTR newElement )
				{
					return CStringList::InsertAfter( position, (LPCTSTR)newElement );
				}
				POSITION InsertBefore( POSITION position, const CExtSafeString & newElement )
				{
					return CStringList::InsertBefore( position, (LPCTSTR)newElement );
				}
				POSITION InsertAfter( POSITION position, const CExtSafeString & newElement )
				{
					return CStringList::InsertAfter( position, (LPCTSTR)newElement );
				}
				POSITION Find( __EXT_MFC_SAFE_LPCTSTR searchValue, POSITION startAfter = NULL ) const
				{
					return CStringList::Find( searchValue, startAfter );
				}
			}; // class CExtSafeStringList

			class CExtSafeMapStringToPtr : public CMapStringToPtr
			{
			public:
				CExtSafeMapStringToPtr(
					INT_PTR nBlockSize = 10
					)
					: CMapStringToPtr( nBlockSize )
				{
				}
				BOOL Lookup(
					__EXT_MFC_SAFE_LPCTSTR key,
					void * & rValue
					) const
				{
					return
						CMapStringToPtr::Lookup(
							key,
							rValue
							);
				}
				BOOL LookupKey(
					__EXT_MFC_SAFE_LPCTSTR key,
					__EXT_MFC_SAFE_LPCTSTR & rKey
					) const
				{
					return
						CMapStringToPtr::LookupKey(
							key,
							(LPCTSTR &) rKey
							);
				}
				void * & operator[] (
					__EXT_MFC_SAFE_LPCTSTR key
					)
				{
					return CMapStringToPtr::operator[]( key );
				}
				void SetAt(
					__EXT_MFC_SAFE_LPCTSTR key,
					void * newValue
					)
				{
					CMapStringToPtr::SetAt(
						key,
						newValue
						);
				}
				BOOL RemoveKey(
					__EXT_MFC_SAFE_LPCTSTR key
					)
				{
					return CMapStringToPtr::RemoveKey( key );
				}
				void GetNextAssoc(
					POSITION & rNextPosition,
					CExtSafeString & rKey,
					void * & rValue
					) const
				{
					CMapStringToPtr::GetNextAssoc(
						rNextPosition,
						(CString&) rKey, // Xtra-dirty-cast
						rValue
						);
				}
				UINT HashKey(
					__EXT_MFC_SAFE_LPCTSTR key
					) const
				{
					return CMapStringToPtr::HashKey( key );
				}
			}; // class CExtSafeMapStringToPtr

			class CExtSafeMapStringToOb : public CMapStringToOb
			{
			public:
				CExtSafeMapStringToOb( INT_PTR nBlockSize = 10 )
					: CMapStringToOb( nBlockSize )
				{
				}
				BOOL Lookup(
					__EXT_MFC_SAFE_LPCTSTR key,
					CObject * & rValue
					) const
				{
					return
						CMapStringToOb::Lookup(
							key,
							rValue
							);
				}
				BOOL LookupKey(
					__EXT_MFC_SAFE_LPCTSTR key,
					__EXT_MFC_SAFE_LPCTSTR & rKey
					) const
				{
					return
						CMapStringToOb::LookupKey(
							key,
							(LPCTSTR &) rKey
							);
				}
				CObject * & operator [] ( __EXT_MFC_SAFE_LPCTSTR key )
				{
					return CMapStringToOb::operator[]( key );
				}
				void SetAt(
					__EXT_MFC_SAFE_LPCTSTR key,
					CObject * newValue
					)
				{
					CMapStringToOb::SetAt(
						key,
						newValue
						);
				}
				BOOL RemoveKey( __EXT_MFC_SAFE_LPCTSTR key )
				{
					return CMapStringToOb::RemoveKey( key );
				}
				void GetNextAssoc(
					POSITION & rNextPosition,
					CExtSafeString & rKey,
					CObject * & rValue
					) const
				{
					CMapStringToOb::GetNextAssoc(
						rNextPosition,
						(CString&) rKey, // Xtra-dirty-cast
						rValue
						);
				}
				UINT HashKey( __EXT_MFC_SAFE_LPCTSTR key ) const
				{
					return CMapStringToOb::HashKey( key );
				}
			}; // class CExtSafeMapStringToOb

			class CExtSafeMapStringToString : public CMapStringToString
			{
			public:
				CExtSafeMapStringToString( INT_PTR nBlockSize = 10 )
					: CMapStringToString( nBlockSize )
				{
				}
				BOOL Lookup(
					__EXT_MFC_SAFE_LPCTSTR key,
					CExtSafeString & rValue
					) const
				{
					return
						CMapStringToString::Lookup(
							key,
							(CString &) rValue // Xtra-dirty-cast
							);
				}
				const CPair * PLookup( __EXT_MFC_SAFE_LPCTSTR key ) const
				{
					return CMapStringToString::PLookup( key );
				}
				CPair * PLookup( __EXT_MFC_SAFE_LPCTSTR key )
				{
					return CMapStringToString::PLookup( key );
				}
				BOOL LookupKey(
					__EXT_MFC_SAFE_LPCTSTR key,
					__EXT_MFC_SAFE_LPCTSTR & rKey
					) const
				{
					return
						CMapStringToString::LookupKey(
							key,
							(LPCTSTR &) rKey
							);
				}
				CExtSafeString & operator [] ( __EXT_MFC_SAFE_LPCTSTR key )
				{
					CString & s = CMapStringToString::operator[]( key );
					return * ( ( CExtSafeString * ) ( & s ) ); // Xtra-dirty-cast
				}
				void SetAt(
					__EXT_MFC_SAFE_LPCTSTR key,
					__EXT_MFC_SAFE_LPCTSTR newValue
					)
				{
					CMapStringToString::SetAt(
						key,
						newValue
						);
				}
				BOOL RemoveKey(
					__EXT_MFC_SAFE_LPCTSTR key
					)
				{
					return
						CMapStringToString::RemoveKey(
							key
							);
				}
				void GetNextAssoc(
					POSITION & rNextPosition,
					CExtSafeString & rKey,
					CExtSafeString & rValue
					) const
				{
					CMapStringToString::GetNextAssoc(
						rNextPosition,
						(CString &) rKey, // Xtra-dirty-cast
						(CString &) rValue // Xtra-dirty-cast
						);
				}
				UINT HashKey(__EXT_MFC_SAFE_LPCTSTR key) const
				{
					return CMapStringToString::HashKey( key );
				}
			}; // class CExtSafeMapStringToString

			#define __EXT_MFC_SAFE_ON_WM_DEVMODECHANGE() \
				{ WM_DEVMODECHANGE, 0, 0, 0, AfxSig_vs, \
					(AFX_PMSG)(AFX_PMSGW) \
					(static_cast< void (AFX_MSG_CALL CWnd::*)(__EXT_MFC_SAFE_LPTSTR) > (OnDevModeChange)) },
			#define __EXT_MFC_SAFE_ON_WM_ASKCBFORMATNAME() \
				{ WM_ASKCBFORMATNAME, 0, 0, 0, AfxSig_vwl, \
					(AFX_PMSG)(AFX_PMSGW) \
					(static_cast< void (AFX_MSG_CALL CWnd::*)(UINT, __EXT_MFC_SAFE_LPTSTR) > (OnAskCbFormatName)) },
			#define __EXT_MFC_SAFE_ON_WM_SETTINGCHANGE() \
				{ WM_SETTINGCHANGE, 0, 0, 0, AfxSig_vws, \
					(AFX_PMSG)(AFX_PMSGW) \
					(static_cast< void (AFX_MSG_CALL CWnd::*)(UINT, __EXT_MFC_SAFE_LPCTSTR) > (OnSettingChange)) },
			#define __EXT_MFC_SAFE_ON_WM_WININICHANGE() \
				{ WM_WININICHANGE, 0, 0, 0, AfxSig_vs, \
					(AFX_PMSG)(AFX_PMSGW) \
					(static_cast< void (AFX_MSG_CALL CWnd::*)(__EXT_MFC_SAFE_LPCTSTR) > (OnWinIniChange)) },

		#else // if - (defined __EXT_MFC_ENABLE_TEMPLATED_CHARS)
				#pragma message("   Prof-UIS native unicode character type support:")
				#pragma message("      disabled (symbol __EXT_MFC_ENABLE_TEMPLATED_CHARS is not defined)")
		#endif // if not - (defined __EXT_MFC_ENABLE_TEMPLATED_CHARS)

	#endif // (defined _UNICODE) && (defined _WCHAR_T_DEFINED) && (defined __EXT_MFC_ENABLE_TEMPLATED_CHARS)
#endif // #if _MFC_VER >= 0x700


#if (!defined __EXT_MFC_TEMPLATED_CHARS_IMPLEMENTED)

			typedef         TCHAR   __EXT_MFC_SAFE_TCHAR;
			typedef const   TCHAR   __EXT_MFC_SAFE_CONST_TCHAR;
			typedef LPTSTR          __EXT_MFC_SAFE_LPTSTR;
			typedef LPCTSTR         __EXT_MFC_SAFE_LPCTSTR;
			typedef         TCHAR & __EXT_MFC_SAFE_TCHAR_REF;
			typedef const   TCHAR & __EXT_MFC_SAFE_CONST_TCHAR_REF;
			typedef LPTSTR        & __EXT_MFC_SAFE_LPTSTR_REF;
			typedef LPCTSTR       & __EXT_MFC_SAFE_LPCTSTR_REF;

			typedef TCHAR __prof_uis_used_wchar_t;
			typedef TCHAR __prof_uis_converted_wchar_t;

			typedef CString CExtSafeString;

			#if _MFC_VER >= 0x700
				typedef CStringA CExtSafeStringA; // allways same as MFC's
				typedef CStringW CExtSafeStringW;
			#endif // #if _MFC_VER >= 0x700

			typedef CStringArray          CExtSafeStringArray;
			typedef CStringList           CExtSafeStringList;

			typedef CMapStringToPtr       CExtSafeMapStringToPtr;
			typedef CMapStringToOb        CExtSafeMapStringToOb;
			typedef CMapStringToString    CExtSafeMapStringToString;

			#define  __EXT_MFC_SAFE_ON_WM_DEVMODECHANGE() \
									ON_WM_DEVMODECHANGE()
			#define  __EXT_MFC_SAFE_ON_WM_ASKCBFORMATNAME() \
									ON_WM_ASKCBFORMATNAME()
			#define  __EXT_MFC_SAFE_ON_WM_SETTINGCHANGE() \
									ON_WM_SETTINGCHANGE()
			#define  __EXT_MFC_SAFE_ON_WM_WININICHANGE() \
									ON_WM_WININICHANGE()

#endif (!defined __EXT_MFC_TEMPLATED_CHARS_IMPLEMENTED)

#endif // __ExtMfcSafeNativeTCHAR_H

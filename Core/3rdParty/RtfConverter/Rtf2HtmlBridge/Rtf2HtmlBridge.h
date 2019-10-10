// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RTF2HTMLBRIDGE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RTF2HTMLBRIDGE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef RTF2HTMLBRIDGE_EXPORTS
#define RTF2HTMLBRIDGE_API __declspec(dllexport)
#else
#define RTF2HTMLBRIDGE_API __declspec(dllimport)
#endif

extern "C" RTF2HTMLBRIDGE_API int fnRtf2Html(LPCWSTR rtfFile,
										  LPCWSTR htmlFile,
										  LPCWSTR a1,
										  LPCWSTR a2,
										  LPCWSTR a3,
										  LPCWSTR a4,
										  LPCWSTR a5,
										  LPCWSTR a6,
										  LPCWSTR a7,
										  LPCWSTR a8,
										  LPCWSTR a9);

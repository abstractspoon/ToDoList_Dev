// Rtf2HtmlBridge.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Rtf2HtmlBridge.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

extern "C" RTF2HTMLBRIDGE_API int fnRtf2Html(LPCTSTR rtfFile,
                                          LPCTSTR htmlFile,
										  LPCTSTR a1,
										  LPCTSTR a2,
										  LPCTSTR a3,
										  LPCTSTR a4,
										  LPCTSTR a5,
										  LPCTSTR a6,
										  LPCTSTR a7,
										  LPCTSTR a8,
										  LPCTSTR a9)
{
	System::String^ _rtfFile = gcnew System::String(rtfFile);
	System::String^ _htmlFile = gcnew System::String(htmlFile);
	System::String^ _a1 = gcnew System::String(a1);
	System::String^ _a2 = gcnew System::String(a2);
	System::String^ _a3 = gcnew System::String(a3);
	System::String^ _a4 = gcnew System::String(a4);
	System::String^ _a5 = gcnew System::String(a5);
	System::String^ _a6 = gcnew System::String(a6);
	System::String^ _a7 = gcnew System::String(a7);
	System::String^ _a8 = gcnew System::String(a8);
	System::String^ _a9 = gcnew System::String(a9);

	Itenso::Solutions::Community::Rtf2Html::Program prog(_rtfFile, _htmlFile, 
														 _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);

	return (prog.Execute() ? 1 : 0);
}


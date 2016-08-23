#pragma once

////////////////////////////////////////////////////////////////////////

#include <Windows.h>

////////////////////////////////////////////////////////////////////////

#pragma warning(disable:4189)
#pragma warning(disable:4127)
#pragma warning(disable:4505)

////////////////////////////////////////////////////////////////////////

// function to be exported from dll to create instance of interface
#ifdef _EXPORTING // declare this in project settings for dll _only_
#	define DLL_DECLSPEC __declspec(dllexport)
#else
#	define DLL_DECLSPEC __declspec(dllimport)
#endif 

////////////////////////////////////////////////////////////////////////

#define ICHRONWRAP_VERSION 0x0000

////////////////////////////////////////////////////////////////////////

class IChronicleWrap;

typedef IChronicleWrap* (*PFNCREATE)(); // function prototype
extern "C" DLL_DECLSPEC IChronicleWrap* CreateChronicleWrapInterface();

typedef int (*PFNGETVERSION)(); // function prototype

////////////////////////////////////////////////////////////////////////

// helper methods
static IChronicleWrap* CreateChronicleWrapInterface(LPCTSTR szDllPath)
{
   IChronicleWrap* pInterface = NULL;
   HMODULE hDll = LoadLibrary(szDllPath);

   if (hDll)
   {
      PFNCREATE pCreate = (PFNCREATE)GetProcAddress(hDll, "CreateChronicleWrapInterface");

      if (pCreate)
      {
         // check version
         PFNGETVERSION pVersion = (PFNGETVERSION)GetProcAddress(hDll, "GetInterfaceVersion");

         if (!ICHRONWRAP_VERSION || (pVersion && pVersion() >= ICHRONWRAP_VERSION))
            pInterface = pCreate();
      }
   }

   return pInterface;
}

static BOOL IsChronicleWrapDll(LPCTSTR szDllPath)
{
   HMODULE hDll = LoadLibrary(szDllPath);

   if (hDll)
   {
      PFNCREATE pCreate = (PFNCREATE)GetProcAddress(hDll, "CreateChronicleWrapInterface");
      FreeLibrary(hDll);

      return (NULL != pCreate);
   }

   return FALSE;
}

////////////////////////////////////////////////////////////////////////

enum ICWC_CAPTURESTATE
{
   ICWC_CAPTURESTARTED,
   ICWC_CAPTUREPAUSED,
   ICWC_CAPTURERESUMED,
   ICWC_CAPTURESTOPPED,
};

// wParam = ICWC_CAPTURESTATE, lParam = 0L
UINT WM_ICWC_CAPTURECHANGED = ::RegisterWindowMessage(L"WM_ICWC_CAPTURECHANGE");

////////////////////////////////////////////////////////////////////////

class IChronicleWrap
{
public:
   virtual BOOL Initialise(HWND hwndCallback) = 0;
   virtual void Release() = 0; 

   virtual BOOL StartRecording() = 0;
   virtual BOOL StopRecording() = 0;
   virtual BOOL PauseRecording() = 0;
   virtual BOOL ResumeRecording() = 0;
   virtual BOOL RestartRecording() = 0;

   virtual BOOL CanStartRecording() const = 0;
   virtual BOOL CanStopRecording() const = 0;
   virtual BOOL CanPauseRecording() const = 0;
   virtual BOOL CanResumeRecording() const = 0;
   virtual BOOL CanRestartRecording() const = 0;
  
   virtual BOOL IsRecording() const = 0;
   virtual BOOL IsPaused() const = 0;
};

////////////////////////////////////////////////////////////////////////

// helper method
static void ReleaseChronicleWrapInterface(IChronicleWrap*& pInterface)
{
   if (pInterface)
   {
      pInterface->Release();
      pInterface = NULL;
   }
}

////////////////////////////////////////////////////////////////////////


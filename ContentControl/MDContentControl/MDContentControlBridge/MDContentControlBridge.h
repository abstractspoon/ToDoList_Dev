// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EXPORTERBRIDGE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EXPORTERBRIDGE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include "..\..\..\..\ToDoList_Dev\Interfaces\IContentControl.h"

#include <vcclr.h>
using namespace MDContentControl;

// This class is exported from ExporterBridge.dll
class CMDContentBridge : public IContent
{
public:
   CMDContentBridge();

   LPCWSTR GetTypeID() const;
   LPCWSTR GetTypeDescription() const;
   HICON GetTypeIcon() const;

   void SetLocalizer(ITransText* pTT);
   void Release();

   IContentControl* CreateCtrl(unsigned short nCtrlID, unsigned long nStyle, 
      long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);


   // returns the length of the html or zero if not supported
   int ConvertToHtml(const unsigned char* pContent, int nLength,
      LPCWSTR szCharSet, LPWSTR& szHtml, LPCWSTR szImageDir);
   void FreeHtmlBuffer(LPWSTR& szHtml);

   void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
   void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly);

protected:
   HICON m_hIcon;

protected:
   ~CMDContentBridge();
};

class CMDContentBridgeControl : public IContentControl
{
public:
	CMDContentBridgeControl();

   BOOL Create(UINT nCtrlID, DWORD nStyle, 
      long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);

   int GetContent(unsigned char* pContent) const;
   bool SetContent(const unsigned char* pContent, int nLength, bool bResetSelection);
   LPCWSTR GetTypeID() const;

   // text content if supported. return false if not supported
   int GetTextContent(LPWSTR szContent, int nLength = -1) const;
   bool SetTextContent(LPCWSTR szContent, bool bResetSelection);

   void SetReadOnly(bool bReadOnly);
   HWND GetHwnd() const;

   void Release();
   bool ProcessMessage(MSG* pMsg);

   ISpellCheck* GetSpellCheckInterface();

   bool Undo();
   bool Redo();

   void SetUITheme(const UITHEME* pTheme);

   void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
   void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey);

protected:
   gcroot<MDContentControlCore^> m_wnd;
   //gcroot<System::Windows::Interop::HwndSource^> m_source;
};

DLL_DECLSPEC int GetInterfaceVersion()
{
   return ICONTENTCTRL_VERSION;
}

DLL_DECLSPEC IContent* CreateContentInterface()
{
   return new CMDContentBridge();
}


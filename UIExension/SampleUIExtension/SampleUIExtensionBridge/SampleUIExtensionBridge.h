// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EXPORTERBRIDGE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EXPORTERBRIDGE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include "..\..\..\..\ToDoList_Dev\Interfaces\IUIExtension.h"

#include <vcclr.h>
using namespace SampleUIExtension;

// This class is exported from SampleUIExtensionBridge.dll
class CSampleUIExtensionBridge : public IUIExtension
{
public:
	CSampleUIExtensionBridge();

   void Release(); // releases the interface

   LPCTSTR GetMenuText() const;
   HICON GetIcon() const;
   LPCWSTR GetTypeID() const; // caller must copy result only

   IUIExtensionWindow* CreateExtWindow(UINT nCtrlID, DWORD nStyle, 
      long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);
   void SetLocalizer(ITransText* pTT);

   void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
   void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey);

};

class CSampleUIExtensionBridgeWindow : public IUIExtensionWindow
{
public:
	CSampleUIExtensionBridgeWindow();

   void Release(); // releases the interface
   BOOL Create(UINT nCtrlID, DWORD nStyle, 
      long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);

   HICON GetIcon() const;
   LPCWSTR GetMenuText() const; // caller must copy result only
   LPCWSTR GetTypeID() const; // caller must copy result only

   bool SelectTask(DWORD dwTaskID);
   bool SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount);

   void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes);
   bool WantEditUpdate(IUI_ATTRIBUTE nAttribute) const;
   bool WantSortUpdate(IUI_ATTRIBUTE nAttribute) const;
   bool PrepareNewTask(ITaskList* pTask) const;

   bool ProcessMessage(MSG* pMsg);
   bool DoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra);
   bool CanDoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra) const;

   bool GetLabelEditRect(LPRECT pEdit); // screen coordinates
   IUI_HITTEST HitTest(const POINT& ptScreen) const;

   void SetUITheme(const UITHEME* pTheme);
   void SetReadOnly(bool bReadOnly);
   HWND GetHwnd() const;

   void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
   void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly = FALSE);
   
protected:
   gcroot<SampleUIExtensionCore^> m_wnd;
   gcroot<System::Windows::Interop::HwndSource^> m_source;
};

DLL_DECLSPEC int GetInterfaceVersion()
{
   return IUIEXTENSION_VERSION;
}

DLL_DECLSPEC IUIExtension* CreateUIExtensionInterface()
{
   return new CSampleUIExtensionBridge();
}


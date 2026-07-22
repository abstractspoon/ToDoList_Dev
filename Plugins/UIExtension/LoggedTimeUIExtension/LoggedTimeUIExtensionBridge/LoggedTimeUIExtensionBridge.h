// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EXPORTERBRIDGE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EXPORTERBRIDGE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include <Interfaces\IUIExtension.h>

#include <vcclr.h>
using namespace LoggedTimeUIExtension;

///////////////////////////////////////////////////////////////////////////////
// This class is exported from LoggedTimeUIExtensionBridge.dll

class CLoggedTimeUIExtensionBridge : public IUIExtension
{
public:
	CLoggedTimeUIExtensionBridge();

   void Release(); // releases the interface

   LPCWSTR GetMenuText() const;
   HICON GetIcon() const;
   LPCWSTR GetTypeID() const; // caller must copy result only

   IUIExtensionWindow* CreateExtWindow(UINT nCtrlID, DWORD nStyle, 
      long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);
   void SetLocalizer(ITransText* pTT);

   void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
   void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey);

   bool SupportsTaskSelection() const { return false; }

protected:
	HICON m_hIcon;
	ITransText* m_pTT;
};

///////////////////////////////////////////////////////////////////////////////
// This class is exported from LoggedTimeUIExtensionBridge.dll

class CLoggedTimeUIExtensionBridgeWindow : public IUIExtensionWindow
{
public:
	CLoggedTimeUIExtensionBridgeWindow(ITransText* pTT);

   BOOL Create(UINT nCtrlID, DWORD nStyle, long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);

   HICON GetIcon() const;
   LPCWSTR GetMenuText() const; // caller must copy result only
   LPCWSTR GetTypeID() const; // caller must copy result only
   
   void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate);
   bool WantTaskUpdate(TDC_ATTRIBUTE nAttribID) const;

   bool ProcessMessage(MSG* pMsg);
   bool DoIdleProcessing();
   bool ShowContextMenu(POINT ptScreen);

   bool DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData);
   bool CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const;

   void SetUITheme(const UITHEME* pTheme);
   void SetReadOnly(bool bReadOnly);
   HWND GetHwnd() const;
   void SetTaskFont(HFONT hFont);

   void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
   void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly);

   // Not required
   bool SelectTask(DWORD /*dwTaskID*/, bool /*bTaskLink*/) { return false; }
   bool SelectTasks(const DWORD* /*pdwTaskIDs*/, int /*nTaskCount*/) { return false; }
   bool PrepareNewTask(ITaskList* /*pTask*/) const { return false; }
   bool GetLabelEditRect(LPRECT /*pEdit*/) { return false; }
   bool HitTest(POINT /*ptScreen*/, IUIHITTEST& /*hitTest*/) const { return false; }
   void FilterToolTipMessage(MSG* /*pMsg*/) { }

protected:
   gcroot<LoggedTimeUIExtensionCore^> m_wnd;
   ITransText* m_pTT;
};

DLL_DECLSPEC int GetInterfaceVersion()
{
   return IUIEXTENSION_VERSION;
}

DLL_DECLSPEC IUIExtension* CreateUIExtensionInterface()
{
   return new CLoggedTimeUIExtensionBridge();
}


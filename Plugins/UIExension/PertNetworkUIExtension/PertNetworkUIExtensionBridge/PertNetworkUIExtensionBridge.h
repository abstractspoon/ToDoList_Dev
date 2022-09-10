// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EXPORTERBRIDGE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EXPORTERBRIDGE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include <Interfaces\IUIExtension.h>

#include <vcclr.h>
using namespace PertNetworkUIExtension;

/////////////////////////////////////////////////////////////////////////////////
// This class is exported from PertNetworkUIExtensionBridge.dll

class CPertNetworkUIExtensionBridge : public IUIExtension
{
public:
	CPertNetworkUIExtensionBridge();

   void Release(); // releases the interface

   LPCWSTR GetMenuText() const;
   HICON GetIcon() const;
   LPCWSTR GetTypeID() const; // caller must copy result only

   IUIExtensionWindow* CreateExtWindow(UINT nCtrlID, DWORD nStyle, 
      long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);
   void SetLocalizer(ITransText* pTT);

   void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
   void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey);

protected:
   HICON m_hIcon;
   ITransText* m_pTT;
};

/////////////////////////////////////////////////////////////////////////////////
// This class is exported from PertNetworkUIExtensionBridge.dll

class CPertNetworkUIExtensionBridgeWindow : public IUIExtensionWindow
{
public:
	CPertNetworkUIExtensionBridgeWindow(ITransText* pTT);

   void Release(); // releases the interface
   BOOL Create(UINT nCtrlID, DWORD nStyle, 
      long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);

   HICON GetIcon() const;
   LPCWSTR GetMenuText() const; // caller must copy result only
   LPCWSTR GetTypeID() const; // caller must copy result only

   bool SelectTask(DWORD dwTaskID);
   bool SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount);
   bool SupportsTaskSelection() const { return false; }

   void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate);
   bool WantTaskUpdate(TDC_ATTRIBUTE nAttribute) const;
   bool PrepareNewTask(ITaskList* pTask) const;

   bool ProcessMessage(MSG* pMsg);
   void FilterToolTipMessage(MSG* pMsg) {/*.Net tooltips don't need this*/}

   bool DoAppCommand(IUI_APPCOMMAND nCmd, IUIAPPCOMMANDDATA* pData);
   bool CanDoAppCommand(IUI_APPCOMMAND nCmd, const IUIAPPCOMMANDDATA* pData) const;

   bool GetLabelEditRect(LPRECT pEdit); // screen coordinates
   IUI_HITTEST HitTest(POINT ptScreen) const;
   DWORD HitTestTask(POINT ptScreen, bool bTitleColumnOnly) const;

   void SetUITheme(const UITHEME* pTheme);
   void SetReadOnly(bool bReadOnly);
   HWND GetHwnd() const;
   void SetTaskFont(HFONT hFont);

   void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
   void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly = FALSE);
   
protected:
   gcroot<PertNetworkUIExtensionCore^> m_wnd;
   ITransText* m_pTT;

protected:
	DWORD GetNextTask(IUI_APPCOMMAND nCmd, DWORD dwTaskID) const;
	bool DoAppSelectCommand(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select);
	
};

DLL_DECLSPEC int GetInterfaceVersion()
{
   return IUIEXTENSION_VERSION;
}

DLL_DECLSPEC IUIExtension* CreateUIExtensionInterface()
{
   return new CPertNetworkUIExtensionBridge();
}


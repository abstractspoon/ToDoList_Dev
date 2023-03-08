/////////////////////////////////////////////////////////////////////

#include <Interfaces\IUIExtension.h>

#include <vcclr.h>

/////////////////////////////////////////////////////////////////////
// This class is exported from WordCloudUIExtensionBridge.dll

class CWordCloudUIExtensionBridge : public IUIExtension
{
public:
	CWordCloudUIExtensionBridge();

	void Release(); // releases the interface

	LPCWSTR GetMenuText() const;
	HICON GetIcon() const;
	LPCWSTR GetTypeID() const; // caller must copy result only

	IUIExtensionWindow* CreateExtWindow(UINT nCtrlID, DWORD nStyle, 
		long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);
	void SetLocalizer(ITransText* pTT);

	void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey);

	bool SupportsTaskSelection() const { return true; }

protected:
	HICON m_hIcon;
	ITransText* m_pTT;
};

/////////////////////////////////////////////////////////////////////
// This class is exported from WordCloudUIExtensionBridge.dll

class CWordCloudUIExtensionBridgeWindow : public IUIExtensionWindow
{
public:
	CWordCloudUIExtensionBridgeWindow(ITransText* pTT);

	BOOL Create(UINT nCtrlID, DWORD nStyle, long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);

	HICON GetIcon() const;
	LPCWSTR GetMenuText() const; // caller must copy result only
	LPCWSTR GetTypeID() const; // caller must copy result only

	bool SelectTask(DWORD dwTaskID, bool bTaskLink);
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
	void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly);

protected:
	gcroot<WordCloudUIExtension::WordCloudUIExtensionCore^> m_wnd;
	ITransText* m_pTT;

protected:
	bool DoAppSelectCommand(IUI_APPCOMMAND nCmd, const IUISELECTTASK& select);
	DWORD GetNextTask(IUI_APPCOMMAND nCmd, DWORD dwFromTaskID) const;
};

DLL_DECLSPEC int GetInterfaceVersion()
{
	return IUIEXTENSION_VERSION;
}

DLL_DECLSPEC IUIExtension* CreateUIExtensionInterface()
{
	return new CWordCloudUIExtensionBridge();
}



#include <Interfaces\IContentControl.h>
#include <Interfaces\ISpellCheck.h>

#include <vcclr.h>

////////////////////////////////////////////////////////////////////////////////

using namespace MDContentControl;

////////////////////////////////////////////////////////////////////////////////

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
   ITransText* m_pTT;

protected:
   ~CMDContentBridge();
};

////////////////////////////////////////////////////////////////////////////////

class CRichEditSpellCheck;

////////////////////////////////////////////////////////////////////////////////

class CMDContentControlBridge : public IContentControl
{
public:
	CMDContentControlBridge(ITransText* pTT);
	virtual ~CMDContentControlBridge();

   BOOL Create(UINT nCtrlID, DWORD nStyle, long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);

   // IContentControl interface ------------------------------------
   int GetContent(unsigned char* pContent) const;
   bool SetContent(const unsigned char* pContent, int nLength, bool bResetSelection);
   LPCWSTR GetTypeID() const;

   int GetTextContent(LPWSTR szContent, int nLength = -1) const;
   bool SetTextContent(LPCWSTR szContent, bool bResetSelection);
   bool InsertTextContent(LPCWSTR szContent, bool bAtEnd);

   bool FindReplaceAll(LPCWSTR szFind, LPCWSTR szReplace, bool bCaseSensitive, bool bWholeWord);

   void Enable(bool bEnable);
   void SetReadOnly(bool bReadOnly);
   HWND GetHwnd() const;

   void Release();
   bool ProcessMessage(MSG* pMsg);
   void FilterToolTipMessage(MSG* pMsg) {} //.Net tooltips don't need this
   bool DoIdleProcessing();

   ISpellCheck* GetSpellCheckInterface();

   bool Undo();
   bool Redo();

   void SetUITheme(const UITHEME* pTheme);
   void SetContentFont(HFONT hFont);

   void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
   void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly);

protected:
   gcroot<MDContentControlCore^> m_wnd;
   ITransText* m_pTT;
   CRichEditSpellCheck* m_pSpellCheck;
};

////////////////////////////////////////////////////////////////////////////////

DLL_DECLSPEC int GetInterfaceVersion()
{
   return ICONTENTCTRL_VERSION;
}

DLL_DECLSPEC IContent* CreateContentInterface()
{
   return new CMDContentBridge();
}


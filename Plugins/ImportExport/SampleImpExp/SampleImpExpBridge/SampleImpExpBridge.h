// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EXPORTERBRIDGE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EXPORTERBRIDGE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include <Interfaces\IImportExport.h>

// This class is exported from ExporterBridge.dll
class CSampleImpExpBridge : public IExportTasklist //, IImportTaskList
{
public:
	CSampleImpExpBridge();

   void Release(); // releases the interface

   void SetLocalizer(ITransText* pTT);
   HICON GetIcon() const;
   bool SupportsHtmlComments() const { return false; }

   LPCWSTR GetMenuText() const;
   LPCWSTR GetFileFilter() const;
   LPCWSTR GetFileExtension() const;
   LPCWSTR GetTypeID() const;

   IIMPORTEXPORT_RESULT Export(const ITaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey);
   IIMPORTEXPORT_RESULT Export(const IMultiTaskList* pSrcTaskFile, LPCWSTR szDestFilePath, DWORD dwFlags, IPreferences* pPrefs, LPCWSTR szKey);

protected:
	HICON m_hIcon;
	ITransText* m_pTT;
};

DLL_DECLSPEC int GetInterfaceVersion()
{
   return IIMPORTEXPORT_VERSION;
}

DLL_DECLSPEC IExportTasklist* CreateExportInterface()
{
   return new CSampleImpExpBridge();
}

DLL_DECLSPEC IImportTasklist* CreateImportInterface()
{
   return NULL; // new CSampleImpExpBridge()
}

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EXPORTERBRIDGE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EXPORTERBRIDGE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include <Interfaces\ITasklistStorage.h>

class CMySqlStorageBridge : public ITasklistStorage
{
public:
	CMySqlStorageBridge();

   void Release(); // releases the interface

   virtual LPCWSTR GetMenuText() const;
   virtual HICON GetIcon() const;
   virtual LPCWSTR GetTypeID() const;

   virtual void SetLocalizer(ITransText* pTT);

   virtual bool RetrieveTasklist(ITS_TASKLISTINFO* pFInfo, ITaskList* pDestTaskFile, IPreferences* pPrefs, LPCWSTR szKey, bool bPrompt);
   virtual bool StoreTasklist(ITS_TASKLISTINFO* pFInfo, const ITaskList* pSrcTaskFile, IPreferences* pPrefs, LPCWSTR szKey, bool bPrompt);

protected:
	HICON m_hIcon;
	ITransText* m_pTT;
	WCHAR szCachedPassword[ITS_PASSWORD_LEN + 1]; // for the session only

protected:
	void CopyInfo(MySqlStorage::TasklistConnectionInfo^ fromDef, ITS_TASKLISTINFO* toInfo);
};

DLL_DECLSPEC int GetInterfaceVersion()
{
   return ITASKLISTSTORAGE_VERSION;
}

DLL_DECLSPEC ITasklistStorage* CreateTasklistStorageInterface()
{
   return new CMySqlStorageBridge();
}


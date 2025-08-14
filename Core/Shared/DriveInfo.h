// CDriveInfo : returns info about drives attached

#if !defined ( DRIVEINFO_H )
#define DRIVEINFO_H 

enum { DRIVE_HOST = 7 };

class CDriveInfo
{
public:
	static float GetFreeBytes(int nDrive); // in MB
	static float GetTotalBytes(int nDrive); // in MB

	static CString GetVolume(int nDrive);
	static CString GetRoot(int nDrive);
	static CString GetFullName(int nDrive);
	static char GetLetter(int nDrive);
	static int GetType(int nDrive);
    static int GetPathType(LPCTSTR szPathName);
	static BOOL IsDriveAvailable(int nDrive);
	static void FormatName(CString& sFileName);
	static int GetDrive(LPCTSTR szPathName);
	static BOOL IsMappedPath(LPCTSTR szPathName);
	static BOOL IsRemotePath(LPCTSTR szPathName, BOOL bAllowFileCheck = TRUE);
	static BOOL IsRemovablePath(LPCTSTR szPathName);
	static BOOL IsFixedPath(LPCTSTR szPathName);
	static int IsReadonlyPath(LPCTSTR szPathName); // -1 = no such path, else TRUE/FALSE
	static DWORD GetSerialNumber(int nDrive);
   static BOOL IsUNCPath(LPCTSTR szPathName);
};

#endif

// Zipper.h: interface for the CZipper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZIPPER_H__4249275D_B50B_4AAE_8715_B706D1CA0F2F__INCLUDED_)
#define AFX_ZIPPER_H__4249275D_B50B_4AAE_8715_B706D1CA0F2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct Z_FileInfo
{
	int nFileCount;
	int nFolderCount;
	DWORD dwUncompressedSize;
};

class CZipper  
{
public:
	CZipper(LPCTSTR szFilePath = NULL, LPCTSTR szRootFolder = NULL, bool bAppend = FALSE);
	virtual ~CZipper();

	// simple interface
	static bool ZipFile(LPCTSTR szFilePath); // saves as same name with .zip
	static bool ZipFolder(LPCTSTR szFilePath, bool bIgnoreFilePath); // saves as same name with .zip
	
	// works with prior opened zip
	bool AddFileToZip(LPCTSTR szFilePath, bool bIgnoreFilePath = FALSE);
	bool AddFileToZip(LPCTSTR szFilePath, LPCTSTR szRelFolderPath); // replaces path info from szFilePath with szFolder
	bool AddFileToZip(const BYTE* pFileContents, int nSize, LPCTSTR szRelFilePath);
	bool AddFolderToZip(LPCTSTR szFolderPath, bool bIgnoreFilePath = FALSE);

	// extended interface
	bool OpenZip(LPCTSTR szFilePath, LPCTSTR szRootFolder = NULL, bool bAppend = FALSE);
	bool CloseZip(); // for multiple reuse
	void GetFileInfo(Z_FileInfo& info);
	
protected:
	void* m_uzFile;
	TCHAR m_szRootFolder[MAX_PATH + 1];
	Z_FileInfo m_info;

protected:
	void PrepareSourcePath(LPTSTR szPath);
	bool GetLastModified(LPCTSTR szPath, SYSTEMTIME& sysTime);

	void UnterminatePath(LPTSTR szPath);
	void PrepareZipPath(LPTSTR szPath);

};

#endif // !defined(AFX_ZIPPER_H__4249275D_B50B_4AAE_8715_B706D1CA0F2F__INCLUDED_)

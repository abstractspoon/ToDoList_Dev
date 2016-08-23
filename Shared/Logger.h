// Logger.h: interface for the CLogger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGGER_H__4D834259_C2FD_444C_B3FB_F95CBDED6768__INCLUDED_)
#define AFX_LOGGER_H__4D834259_C2FD_444C_B3FB_F95CBDED6768__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLogger  
{
public:
	virtual ~CLogger();

	static BOOL Initialize(LPCTSTR szAppVer = NULL, int nLevel = 0, int nNumBackups = 7); // will cull any logs exceeding this from oldest forward
	static BOOL __cdecl Log(int nLevel, const char *fmt, ...);
	static BOOL EnableLogging(int nLevel = 1);

protected:
	static BOOL s_bInitialized;
	static HANDLE s_hAccessible; // multi-threaded support

	CString m_sLogFolder;
	int m_nMaxLevel;

protected:
	CLogger(LPCTSTR szAppVer, int nLevel, int nNumBackups = 7);

	static CLogger& GetInstance(LPCTSTR szAppVer = NULL, int nLevel = 0, int nNumBackups = 7);

	static CString GetDate();
	static CString GetTime();

	void AppendToLog(LPCTSTR szText);
	void StartLogging(LPCTSTR szAppVer);
	void EndLogging();
	void CullLogs(int nNumBackups);
	void SetLoggingLevel(int nLevel) { m_nMaxLevel = nLevel; }
	int GetLoggingLevel() { return m_nMaxLevel; }

};

#endif // !defined(AFX_LOGGER_H__4D834259_C2FD_444C_B3FB_F95CBDED6768__INCLUDED_)

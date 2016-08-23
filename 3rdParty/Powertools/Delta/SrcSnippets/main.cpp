
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32ONLY

#include <conio.h>
#include <stdio.h>
#include <tchar.h>
#include <wtypes.h>
#include <winbase.h>

#ifndef ASSERT
	#include <assert.h>
	#define ASSERT assert
#endif
#ifndef VERIFY
	#ifdef _DEBUG
		#define VERIFY ASSERT
	#else
		#define VERIFY
	#endif
#endif

#define TRACE

class COS : public OSVERSIONINFO
{
public:
	COS()
	{
		dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(this);
	}

	const BOOL IsWin95() const { return dwPlatformId==VER_PLATFORM_WIN32_WINDOWS; }
	const BOOL IsWinNT() const { return dwPlatformId==VER_PLATFORM_WIN32_NT; }
};

const COS			g_os;

const UINT PATHmin = min(_MAX_PATH,MAX_PATH);
const UINT PATHmax = max(_MAX_PATH,MAX_PATH);
#undef MAX_PATH
#undef _MAX_PATH

#define CR	_T("\n")
#define TAB	_T("\t")

#ifdef _UNICODE
	#include <wchar.h>
	#define _tmakepath	_wmakepath
	#define _tsplitpath	_wsplitpath
#else
	#define _tmakepath	_makepath
	#define _tsplitpath	_splitpath
#endif

TCHAR g_rgtchProg[PATHmax];

void UpdateConsoleTitle(LPCTSTR tszLastChange)
{
	TCHAR fname[_MAX_FNAME];
	_tsplitpath(g_rgtchProg,NULL,NULL,fname,NULL);

	TCHAR rgtchTitle[2*PATHmax + sizeof(" - ") + _MAX_FNAME];
	_stprintf(rgtchTitle,_T("%s - %s"),tszLastChange,fname);
	VERIFY(SetConsoleTitle(rgtchTitle));
}

BOOL fExists(LPCTSTR tszFilename)
{
	return
		tszFilename &&
		tszFilename[0] &&
		(::GetFileAttributes(tszFilename) != -1);
}

struct WATCHTHREADINFO
{
	TCHAR	rgtchPath[PATHmax];	
};

// target of ptwi owned by thread
// for convenience
#define FNC(x) FILE_NOTIFY_CHANGE_##x

// never self-terminates
DWORD _WatchThreadFuncNT(WATCHTHREADINFO * pwti)
{
	ASSERT(pwti);

	// Snag these values & nuke the object quick in case we're nuked in the future.
	// (I.e. let's avoid the memory leak.)
	TCHAR rgtchPathToWatch[PATHmax] = _T("");
	_tcscpy(rgtchPathToWatch,pwti->rgtchPath);
	delete pwti;
	pwti=NULL;

	typedef WINBASEAPI BOOL WINAPI ReadDirectoryChangesW_PROC(
		HANDLE hDirectory,									// handle to the directory to be watched
		LPVOID lpBuffer,									// pointer to the buffer to receive the read results
		DWORD nBufferLength,								// length of lpBuffer
		BOOL bWatchSubtree,									// flag for monitoring directory or directory tree
		DWORD dwNotifyFilter,								// filter conditions to watch for
		LPDWORD lpBytesReturned,							// number of bytes returned
		LPOVERLAPPED lpOverlapped,							// pointer to structure needed for overlapped I/O
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine	// pointer to completion routine
	);
 
	static ReadDirectoryChangesW_PROC * pReadDirectoryChangesW =
		(ReadDirectoryChangesW_PROC*)GetProcAddress(GetModuleHandle(_T("Kernel32")),"ReadDirectoryChangesW");
	if (!pReadDirectoryChangesW)
	{
		TRACE(_T("WARNING:  _WatchThreadFuncNT - GetProcAddress failed.") CR);
		return 0;
	}

	const DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
	const DWORD dwFlags = FILE_FLAG_BACKUP_SEMANTICS;
	HANDLE hDir = CreateFile(rgtchPathToWatch,FILE_LIST_DIRECTORY,dwShareMode,NULL,OPEN_EXISTING,dwFlags,NULL);

	if (hDir==INVALID_HANDLE_VALUE)
	{
		TRACE(_T("WARNING:  _WatchThreadFuncNT - CreateFile failed.") CR);
		return 0;
	}

	const int cbRGB = 4096;	// @review - plenty?
	BYTE rgb[cbRGB];

	const BOOL fPathToWatchHasTrailingBackslash =
		rgtchPathToWatch[_tcslen(rgtchPathToWatch)-1] == _T('\\');

	const DWORD dwNotifyFilter = FNC(LAST_WRITE)|FNC(FILE_NAME)|FNC(DIR_NAME)|FNC(SECURITY);
	DWORD cbReturned;
	while ((*pReadDirectoryChangesW)(
		hDir,
		(LPVOID)rgb,
		cbRGB,
		TRUE,
		dwNotifyFilter,
		&cbReturned,
		NULL,
		NULL
		))
	{
		const FILE_NOTIFY_INFORMATION * pfni=(const FILE_NOTIFY_INFORMATION *)rgb;
		do
		{
			TCHAR rgtchFileRel[PATHmax] = _T("");
			memset(rgtchFileRel,0,sizeof(rgtchFileRel));
#ifdef _UNICODE
			_tcsncpy(rgtchFileRel,pfni->FileName,PATHmax);
#else
			VERIFY(WideCharToMultiByte(CP_ACP,0,pfni->FileName,pfni->FileNameLength/sizeof(WCHAR),rgtchFileRel,PATHmax,NULL,NULL));
#endif

			TCHAR rgtchFileFull[PATHmax];
			_stprintf(rgtchFileFull, _T("%s%s%s"),
				rgtchPathToWatch,
				fPathToWatchHasTrailingBackslash ? _T("") : _T("\\"),
				rgtchFileRel);

			TCHAR rgtchLastWrite[] = _T("                   ");
			__int64 cbFile = 0;
			{
				WIN32_FIND_DATA fd;
				HANDLE h = FindFirstFile(rgtchFileFull,&fd);
				if (h && (h != INVALID_HANDLE_VALUE))
				{
					FILETIME ftLocal;
					VERIFY(FileTimeToLocalFileTime(&fd.ftLastWriteTime,&ftLocal));
					//
					SYSTEMTIME st;
					VERIFY(FileTimeToSystemTime(&ftLocal,&st));
					//
					_stprintf(rgtchLastWrite,_T("%4d,%2d %2d %2d:%02d:%02d"),
						st.wYear, st.wMonth, st.wDay,
						st.wHour, st.wMinute, st.wSecond
						);

					cbFile = (fd.nFileSizeHigh * MAXDWORD) + fd.nFileSizeLow;
					VERIFY(FindClose(h));
				}
			}

			TCHAR rgtchTitle[sizeof(sizeof(rgtchLastWrite)+_T("(+) "))+PATHmax+19];
			switch (pfni->Action)
			{
			case FILE_ACTION_ADDED:
				_stprintf(rgtchTitle, _T("(+) %s"), rgtchFileFull);
				_tprintf(_T("%s (+) % 19I64i %s\n"), rgtchLastWrite, cbFile, rgtchFileFull);
				break;
			case FILE_ACTION_REMOVED:
				_stprintf(rgtchTitle, _T("(-) %s"), rgtchFileFull);
				_tprintf(_T("%s (-) % 19I64i %s\n"), rgtchLastWrite, cbFile, rgtchFileFull);
				break;
			case FILE_ACTION_RENAMED_OLD_NAME:
				_stprintf(rgtchTitle, _T("(/) %s"), rgtchFileFull);
				_tprintf(_T("%s (/) % 19I64i %s\n"), rgtchLastWrite, cbFile, rgtchFileFull);
				break;
			case FILE_ACTION_RENAMED_NEW_NAME:
				_stprintf(rgtchTitle, _T("(\\) %s"), rgtchFileFull);
				_tprintf(_T("%s (\\) % 19I64i %s\n"), rgtchLastWrite, cbFile, rgtchFileFull);
				break;
			case FILE_ACTION_MODIFIED:
				_stprintf(rgtchTitle, _T("(*) %s"), rgtchFileFull);
				_tprintf(_T("%s (*) % 19I64i %s\n"), rgtchLastWrite, cbFile, rgtchFileFull);
				break;
			default:
				ASSERT(FALSE);
			}

			UpdateConsoleTitle(rgtchTitle);

			if (pfni->NextEntryOffset)
				pfni = (const FILE_NOTIFY_INFORMATION *)((const LPBYTE)pfni+pfni->NextEntryOffset);
			else
				pfni = NULL;
		} while (pfni);
	}

	VERIFY(CloseHandle(hDir));
	hDir = INVALID_HANDLE_VALUE;

	Sleep(INFINITE);

	return 0;
}

void ReportSyntax()
{
	_tprintf(
		CR
		_T("Proper syntax:") CR
		TAB _T("%s [/?] path [path2 ..]") CR
		CR
		_T("Notes:") CR
		TAB _T("Paths must be to a specific file or directory, not a filespec.") CR
		TAB _T("Press 'Q' to terminate.") CR
		CR
		,g_rgtchProg
		);
}

#ifdef _UNICODE
extern "C" int wmain(int argc, const wchar_t * argv[])
#else
int main(int argc, const char * argv[])
#endif
{
	_tcscpy(g_rgtchProg,argv[0]);

	//...

	if (!g_os.IsWinNT() || (((const OSVERSIONINFO &)g_os).dwMajorVersion < 4))
	{
		_ftprintf(stderr, CR _T("ERROR:  Unsupported operating system.") CR);
		_ftprintf(stderr, CR _T("Windows NT 4.0 or higher is required.") CR);
		return __LINE__;
	}

	UpdateConsoleTitle(_T("(no changes detected)"));

	LPCTSTR * rgtszWatchPaths = new LPCTSTR[argc];
	ASSERT(rgtszWatchPaths);
	if (!rgtszWatchPaths)
	{
		_ftprintf(stderr, CR _T("ERROR:  Internal - Out of memory.") CR);
		return __LINE__;
	}

	int cWatchPaths = 0;

	for (int i=1; i<argc; i++)
	{
		LPCTSTR tszArg = argv[i];
		if (!_tcscmp(tszArg,_T("/?")) || !_tcscmp(tszArg,_T("-?")))
		{
			ReportSyntax();
			return 0;
		}

		const BOOL fValidPath = fExists(tszArg);
		if (fValidPath)
			rgtszWatchPaths[cWatchPaths++] = tszArg;
		else
			_ftprintf(stderr, _T("WARNING:  Invalid path '%s' ignored.") CR, tszArg);
	}

	if (!cWatchPaths)
	{
		_ftprintf(stderr, CR _T("ERROR:  Invalid syntax.") CR);
		ReportSyntax();
		return __LINE__;
	}

	HANDLE * rghThreads = new HANDLE[cWatchPaths];
	ASSERT(rghThreads);
	if (!rghThreads)
	{
		_ftprintf(stderr, CR _T("ERROR:  Internal - Out of memory.") CR);
		return __LINE__;
	}
	for (i=0; i<cWatchPaths; i++)
	{
		WATCHTHREADINFO * pwti = new WATCHTHREADINFO;
		ASSERT(pwti);
		_tcscpy(pwti->rgtchPath,rgtszWatchPaths[i]);

		DWORD idThread;
		HANDLE hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)_WatchThreadFuncNT,pwti,0,&idThread);
		rghThreads[i] = hThread;
	}

	delete rgtszWatchPaths;
	rgtszWatchPaths = NULL;

	while (tolower(getch()) != 'q')
		;

	for (i=0; i<cWatchPaths; i++)
		CloseHandle(rghThreads[i]);
	delete rghThreads;
	rghThreads=NULL;

	return 0;
}
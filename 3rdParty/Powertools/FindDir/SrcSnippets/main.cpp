#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <io.h>
#include <stdio.h>
#include <string.h>

#include <wtypes.h>
#include <winver.h>
#include <winnetwk.h>
#pragma comment (lib,"mpr")

const UINT PATHmin = min(_MAX_PATH,MAX_PATH);
const UINT PATHmax = max(_MAX_PATH,MAX_PATH);
#undef MAX_PATH
#undef _MAX_PATH

#define CR	"\n"
#define TAB	"\t"

// MFCBEGONE start

#ifndef ASSERT
	#include <assert.h>
	#define ASSERT assert
#endif

#ifndef TRACE
	#ifdef _DEBUG
		#define TRACE OutputDebugString
	#else
		#define TRACE
	#endif
#endif

#ifndef VERIFY
	#ifdef _DEBUG
		#define VERIFY ASSERT
	#else
		#define VERIFY
	#endif
#endif

// MFCBEGONE end

#define TRACEN(x)	(TRACE(x), TRACE(CR))

void UpdateConsoleTitle(LPCTSTR szFound, LPCTSTR szWanted)
{
	const TCHAR rgtchBase[] = _T(" - FindDir");
	TCHAR rgtchTitle[PATHmax+PATHmax+sizeof(rgtchBase)+2/*()*/+1];
	if (szFound && szWanted)
		sprintf(rgtchTitle,_T("%s%s(%s)"),szFound,rgtchBase,szWanted);
	else
		sprintf(rgtchTitle,_T("done%s"),rgtchBase);
	VERIFY(SetConsoleTitle(rgtchTitle));
}

char g_rgchProg[_MAX_FNAME+_MAX_EXT];

void ShowSyntax(BOOL fComplain=FALSE)
{
	if (fComplain)
		fprintf(stderr, CR "ERROR:  Invalid syntax." CR);

	char fname[_MAX_FNAME];
	_splitpath(g_rgchProg,NULL,NULL,fname,NULL);

	fprintf(stderr,
		CR
		"Proper Syntax:" CR
		"  %s dirwanted server1 [server2 ..]" CR
		CR

		,fname
		);
}

// . & ..
const BOOL fIsSpecialDirectory(const char * szDir)
{
	return !strcmp(szDir,".") || !strcmp(szDir,"..");
}

void FindDirectoryFromShare(LPCTSTR szBase, LPCTSTR szWanted)
{
	if (!szWanted || !szWanted[0])
	{
		ASSERT(FALSE);
		return;
	}

	char rgchSpec[PATHmax];
	strcpy(rgchSpec,szBase);
	strcat(rgchSpec,"\\*.*");

	WIN32_FIND_DATA fd;
	HANDLE hff=FindFirstFile(rgchSpec,&fd);

	BOOL fFound = (hff==INVALID_HANDLE_VALUE) ? FALSE : TRUE;

	while (fFound)
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath(rgchSpec,drive,dir,NULL,NULL);
		_splitpath(fd.cFileName,NULL,NULL,fname,ext);
		//
		char rgchFound[PATHmax];
		_makepath(rgchFound,drive,dir,fname,ext);

		const BOOL fIsSubdirectory = fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

		if (fIsSubdirectory && !fIsSpecialDirectory(fd.cFileName))
		{
			UpdateConsoleTitle(rgchFound,szWanted);

			char rgchDirName[_MAX_FNAME+_MAX_EXT];
			strcpy(rgchDirName,fname);
			strcat(rgchDirName,ext);
			if (!stricmp(rgchDirName,szWanted))
				printf("%s\n",rgchFound);
			FindDirectoryFromShare(rgchFound,szWanted);
		}

		fFound = FindNextFile(hff,&fd);
	}
	if (hff != INVALID_HANDLE_VALUE)
		VERIFY(FindClose(hff));
}

int main(int cArg, char * rgszArg[])
{
	// store app path
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(rgszArg[0],NULL,NULL,fname,ext);
	//
	strcpy(g_rgchProg,fname);
	strcat(g_rgchProg,ext);

	// check arguments
	if (cArg < 3)
	{
		ShowSyntax(TRUE);
		return __LINE__;
	}

	const char * szWanted = rgszArg[1];

	for (int iArg=2; iArg<cArg; iArg++)
	{
		NETRESOURCE nr;
		nr.dwScope = RESOURCE_GLOBALNET;
		nr.dwType = RESOURCETYPE_DISK;
		nr.dwDisplayType = RESOURCEDISPLAYTYPE_GENERIC;
		nr.dwUsage = RESOURCEUSAGE_CONNECTABLE;
		nr.lpLocalName = NULL;
		nr.lpRemoteName = rgszArg[iArg];
		nr.lpComment = NULL;
		nr.lpProvider = NULL;
		HANDLE henum;
		DWORD dwResult = WNetOpenEnum(
			RESOURCE_GLOBALNET,RESOURCETYPE_DISK,RESOURCEUSAGE_CONNECTABLE,&nr,&henum
			);
		if (dwResult == NO_ERROR)
		{
			LPBYTE rgbNetResource[16 * 1024];
			DWORD cbBuffer = sizeof(rgbNetResource);
			DWORD cEntries = 0xFFFFFFFF;
			dwResult = WNetEnumResource(henum,&cEntries,&(rgbNetResource[0]),&cbBuffer);
			if (dwResult == NO_ERROR)
			{
				for (DWORD i=0; i<cEntries; i++)
				{
					NETRESOURCE * pnr = (NETRESOURCE*)&(rgbNetResource[0]);
					FindDirectoryFromShare(pnr[i].lpRemoteName,szWanted);
				}
			}
			else if (dwResult == ERROR_NO_MORE_ITEMS)
				TRACEN("WARN:  main - WNetEnumResource failed.");
			else
				ASSERT(FALSE);
			VERIFY(WNetCloseEnum(henum)==NO_ERROR);
		}
		else
		{
			TRACEN("WARN:  main - WNetOpenEnum failed.");
		}
	}

	UpdateConsoleTitle(NULL,NULL);
	return 0;
}


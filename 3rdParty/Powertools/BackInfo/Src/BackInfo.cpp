#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <lm.h>
#include <time.h>

#include "speed.h"
#include "bitmap.h"

#define NUM_STRINGS     20  // Max number of lines in display
typedef enum 
{
    TEXT_MACHINE_NAME = 0,  // Machine name
    TEXT_USER_NAME,         // User name
    TEXT_SYS_VERSION,       // Operating system version    
    TEXT_SYS_INFO,          // Hardware information
    TEXT_NET_INFO,          // Network information
    TEXT_FREE,              // Free text
    TEXT_TIME,              // Update time
    TEXT_UNUSED             // Unused line
} LineType;

#define MAX_DATA          5 // Number of items in the system data array

typedef struct
{
    LineType    Type;                       // Type of line
    //
    // Font characteristics
    //
    CHAR        szFaceName[LF_FACESIZE];    // Name of font
    DWORD       dwHeight;                   // Size of font
    COLORREF    clrText;                    // Text of font
    DWORD       dwWeight;                   // See FW_BOLD etc.
    BOOL        bItalic;                    // Is font italic?
    DWORD       dwAlign;                    // Font alignment - see TA_CENTER etc.
    DWORD       dwPixelHeight;              // The height of the text line when drawn (in pixels)
    //
    // Shadow characteristics
    //
    DWORD       dwShadowXOffset;            // Shadow horizontal offset from text
    DWORD       dwShadowYOffset;            // Shadow vertical offset from text
    COLORREF    clrShadow;                  // Shadow color
    //
    // File version and free text strings
    //
    CHAR        szText[1024];               // Text to use when displaying free text / file version / registry value    
} LineInfo;

/*******************************************************************************
*                                                                              *
*                    G L O B A L   V A R I A B L E S                           *
*                                                                              *
*******************************************************************************/

//
// Global list of strings representing the collected system information
//
CHAR g_szSystemData[MAX_DATA][1024] = {0};

//
// Full path to the INI file
//
CHAR g_szFullINIPath [MAX_PATH] = {0};

//
// Output bitmap file name
//
CHAR g_szFileName[MAX_PATH] = "";

//
// Input background bitmap file name
//
CHAR g_szBackgroundBitmapFileName[MAX_PATH] = "";

//
// Force the desktop to display the bitmap cenetered
//
BOOL g_bForceDesktopCenter = TRUE;

//
// Offset of text block from image center
//
POINT g_ptOffest = {0};

//
// Lines to display
//
LineInfo Lines[NUM_STRINGS] =
{
    { TEXT_MACHINE_NAME, "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_USER_NAME,    "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_SYS_VERSION,  "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_SYS_INFO,     "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_NET_INFO,     "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_UNUSED,       "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_UNUSED,       "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_UNUSED,       "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_UNUSED,       "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_UNUSED,       "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_UNUSED,       "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_UNUSED,       "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_UNUSED,       "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_UNUSED,       "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_UNUSED,       "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_UNUSED,       "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_UNUSED,       "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_UNUSED,       "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_UNUSED,       "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" },
    { TEXT_TIME,		 "Arial", 22, RGB(255,255,255), FW_NORMAL, FALSE, TA_LEFT , 0, 0, 0, 0, "" }
};

//
// Bitmap background color
//
COLORREF g_clrBackground = RGB(0,0,0);
//
// Use background color from the desktop?
//
BOOL     g_bAutoBackground = FALSE;
//
// Update desktp to use bitmap as wallpaper?
//
BOOL     g_bUpdateDesktop = TRUE;
//
// Suppress errors?
//
BOOL     g_bSuppressErrors = FALSE;
//
// Fonts to use for each line
//
HFONT g_hFonts[NUM_STRINGS] = {0};
//
// Line spacing
//
DWORD g_dwLineSpacing = 3;

/*******************************************************************************
*                                                                              *
*                    F U N C T I O N   P R O T O T Y P E S                     *
*                                                                              *
*******************************************************************************/

void 
Error (
    CHAR *szString, 
    DWORD dwLastErrCode = GetLastError ()
);

BOOL
IsDesktopSKU (LPOSVERSIONINFOEX);

BOOL
ParseCommandLine ();

BOOL
RenderBitmap ();

BOOL
CollectData ();

BOOL
CreateFonts ();

DWORD
GetNumCPUs ();

BOOL 
DestroyFonts ();

BOOL SaveBitmap (
    HDC hDC,
    HBITMAP hBitMap
);

BOOL GetGeneralUserName (
    BOOL  bWinNT,
    LPSTR lpstrUserName
);

BOOL
GetFileVersion (
    LPSTR       lpstrFilePath,
    LPSTR       lpstrFileDisplayName,
    LPSTR       lpstrVersion
);

PBITMAPINFO 
CreateBitmapInfoStruct(
    HBITMAP hBmp
);

BOOL 
GetINIDWord (
    LPCSTR lpcstrSection,
    LPCSTR lpcstrKey,
    LPDWORD lpdwValue
);

BOOL
ReadAndExpandString (
    LPCSTR lpcstrSection,
    LPCSTR lpcstrKey,
    LPDWORD lpdwValue,
    LPCSTR lpcstrDefault = ""
);

BOOL
GetIPAddress (
    LPSTR lpstrIP
);

BOOL
ReadRegDWORD (
    HKEY    hKeyRoot,
    LPCSTR  lpcstrPath,
    LPCSTR  lpcstrValue,
    LPDWORD lpdwReturnVal,
    BOOL    bQuiet = FALSE
);

BOOL
ReadRegString (
    HKEY    hKeyRoot,
    LPCSTR  lpcstrPath,
    LPCSTR  lpcstrValue,
    LPSTR   lpstrBuffer,
    DWORD   dwBufSize
);

BOOL
GetFirstCPUSpeed (
    LPDWORD lpdwCPUSpeed
);

DWORDLONG
GetSystemRAM ();

LPSTR 
GetFormattedSystemRAM ();

LPSTR 
GetFormattedFirstCPUSpeed ();

LPSTR
GetUpdateTime ();


/*******************************************************************************
*                                                                              *
*                        I M P L E M E N T A T I O N                           *
*                                                                              *
*******************************************************************************/

BOOL
ReadRegDWORD (
    HKEY    hKeyRoot,
    LPCSTR  lpcstrPath,
    LPCSTR  lpcstrValue,
    LPDWORD lpdwReturnVal,
    BOOL    bQuiet /* = FALSE */
)
{
    HKEY hKey; 
    DWORD dummy = sizeof (DWORD);
    DWORD dwRes;
    DWORD dw;
    dwRes = RegOpenKeyEx( hKeyRoot,
                          lpcstrPath,
                          0, 
                          KEY_QUERY_VALUE, 
                          &hKey);
    if (ERROR_SUCCESS != dwRes)
    {
        if (!bQuiet)
        {
            Error ("Cannot open reg key", dwRes);
        }
        return FALSE;
    }
    dwRes = RegQueryValueEx( hKey, 
                             lpcstrValue, 
                             NULL, 
                             NULL,
                             (LPBYTE) &dw, 
                             &dummy);
    if (ERROR_SUCCESS != dwRes)
    {
        if (!bQuiet)
        {
            Error ("Cannot read reg value", dwRes);
        }
        RegCloseKey (hKey);
        return FALSE;
    }
    RegCloseKey (hKey);
    *lpdwReturnVal = dw;
    return TRUE;
}   // ReadRegDWORD

BOOL
ReadRegString (
    HKEY    hKeyRoot,
    LPCSTR  lpcstrPath,
    LPCSTR  lpcstrValue,
    LPSTR   lpstrBuffer,
    DWORD   dwBufSize
)
{
    HKEY hKey; 
    DWORD dummy = dwBufSize;
    DWORD dwRes;
    dwRes = RegOpenKeyEx( hKeyRoot,
                          lpcstrPath,
                          0, 
                          KEY_QUERY_VALUE, 
                          &hKey);
    if (ERROR_SUCCESS != dwRes)
    {
        Error ("Cannot open reg key", dwRes);
        return FALSE;
    }
    dwRes = RegQueryValueEx( hKey, 
                             lpcstrValue, 
                             NULL, 
                             NULL,
                             (LPBYTE) lpstrBuffer, 
                             &dummy);
    if (ERROR_SUCCESS != dwRes)
    {
        Error ("Cannot read reg value", dwRes);
        RegCloseKey (hKey);
        return FALSE;
    }
    RegCloseKey (hKey);
    return TRUE;
}   // ReadRegString


void 
Error (
    CHAR *szString, 
    DWORD dwLastErrCode
)
{
    if (g_bSuppressErrors)
    {
        //
        // Errors are suppressed
        //
        return;
    }
    CHAR szTxt[MAX_PATH];

    sprintf (szTxt, "%s (%ld)", szString, dwLastErrCode);
    MessageBox (NULL, szTxt, NULL, MB_ICONERROR | MB_OK);
}

int compare( const void *arg1, const void *arg2 )
{
    DWORD dwArg1 = *(LPDWORD(arg1));
    DWORD dwArg2 = *(LPDWORD(arg2));
    return dwArg1 > dwArg2 ? 1 : (dwArg1 < dwArg2 ? -1 : 0);
}

#define IP_PART1(ip)            (((ip) & 0xff000000) >> 24)
#define IP_PART2(ip)            (((ip) & 0x00ff0000) >> 16)
#define IP_PART3(ip)            (((ip) & 0x0000ff00) >>  8)
#define IP_PART4(ip)            ((ip) & 0x000000ff)
#define FLIP_IP(ip)             ((IP_PART4(ip) << 24) | (IP_PART3(ip) << 16) | (IP_PART2(ip) << 8) | IP_PART1(ip))
#define IP2STRING(ip,string)    sprintf(string,"%d.%d.%d.%d",IP_PART1(ip), IP_PART2(ip), IP_PART3(ip), IP_PART4(ip)); 
#define SUCCESSIVE_IP(ip1,ip2)  (((ip1) + 1 == (ip2)) && (((ip1) & 0xffffff00) == ((ip2) & 0xffffff00)))


DWORD
GetSortedIPAddresses (
    LPDWORD lpdwIPs
)
{
    HMODULE hMod = LoadLibrary ("Ws2_32.dll");

    if (!hMod)
    {
        return 0;
    }

    DWORD dwAdapter = 0;

    typedef int (FAR PASCAL *PWSASTARTUP)(WORD, LPWSADATA);
    typedef int (FAR PASCAL *PGETHOSTNAME)(char far *, int);
    typedef struct hostent FAR * (FAR PASCAL *PGETHOSTBYNAME)(const char FAR *);
    typedef int (FAR PASCAL *PWSACLEANUP)(void);

    PWSASTARTUP pWSAStartup = (PWSASTARTUP)GetProcAddress (hMod, "WSAStartup");
    PGETHOSTNAME pgethostname = (PGETHOSTNAME)GetProcAddress (hMod, "gethostname");
    PGETHOSTBYNAME pgethostbyname = (PGETHOSTBYNAME)GetProcAddress (hMod, "gethostbyname");
    PWSACLEANUP pWSACleanup = (PWSACLEANUP)GetProcAddress (hMod, "WSACleanup");

    if (!pWSAStartup || !pgethostname || !pgethostbyname || !pWSACleanup)
    {
        FreeLibrary (hMod);
        return 0;
    }

    WORD wVersionRequested;
    WSADATA wsaData;
    PHOSTENT pHostInfo;
    wVersionRequested = MAKEWORD( 2, 0 );
    CHAR szName[255];
    BOOL bShutdown = FALSE;

    if (ERROR_SUCCESS != pWSAStartup(wVersionRequested, &wsaData))
    {
        goto exit;
    }
    bShutdown = TRUE;
    if (ERROR_SUCCESS != pgethostname (szName, sizeof(szName)))
    {
        goto exit;
    }
    if (NULL == (pHostInfo = pgethostbyname(szName)))
    {
        goto exit;
    }
    while (pHostInfo->h_addr_list[dwAdapter])
    {
        //
        // Iterate all IP addresses
        //
        struct sockaddr_in sAddr;
        memcpy (&sAddr.sin_addr.s_addr, 
                pHostInfo->h_addr_list[dwAdapter],
                pHostInfo->h_length);
        //
        // Store IP address as DWORD
        //
        lpdwIPs[dwAdapter++] = FLIP_IP(sAddr.sin_addr.S_un.S_addr);
    }
exit:
    if (bShutdown)
    {
        pWSACleanup( );
    }
    FreeLibrary (hMod);
    if (dwAdapter > 1)
    {
        //
        // Sort array of IP addresses
        //
        qsort (LPVOID(lpdwIPs), dwAdapter, sizeof (DWORD), compare);
    }
    return dwAdapter;
}   // GetSortedIPAddresses


DWORD GetIPRange (
    LPDWORD lpdwIPs,
    DWORD   dwNumIPs,
    char   *string
)
{
    DWORD dwRangeWidth = 1;
    DWORD dwLastIP = lpdwIPs[0];
    for (DWORD dw = 1; dw < dwNumIPs; dw++)
    {
        DWORD dwCurIP = lpdwIPs[dw];
        if (SUCCESSIVE_IP(dwLastIP, dwCurIP))
        {
            //
            // Range is kept
            //
            dwRangeWidth++;
        }
        else
        {
            break;
        }
        dwLastIP = dwCurIP;
    }
    if (1 == dwRangeWidth)
    {
        //
        // Only one IP address in this range
        //
        IP2STRING(lpdwIPs[0], string)
        dwRangeWidth = 1;
    }
    else
    {
        //
        // Two or more successive IP addresses
        //
        char szIP[200];

        IP2STRING(lpdwIPs[0], string)
        strcat (string, " ... ");
        sprintf (szIP,"%d", IP_PART4(dwLastIP));
        strcat (string, szIP);
    }        
    return dwRangeWidth;
}   // GetIPRange

BOOL
GetIPAddress (
    LPSTR lpstrIP
)
{
    DWORD dwIPAddresses[2048];
    DWORD dwIPs = GetSortedIPAddresses (LPDWORD(&dwIPAddresses));

    if (!dwIPs)
    {
        return FALSE;
    }
    lpstrIP[0] = '\0';
    for (DWORD dw = 0; dw < dwIPs; )
    {
        char szIPRange[200];

        if (strlen (lpstrIP))
        {
            strcat (lpstrIP, ", ");
        }
        dw += GetIPRange (&dwIPAddresses[dw], dwIPs-dw, szIPRange);
        strcat (lpstrIP, szIPRange);
    }
    return TRUE;
}   // GetIPAddress

BOOL
GetMachineDomain (
    BOOL bIsNT,
    LPSTR lpstrDomain
)
{
    if (!bIsNT)
    {
        //
        // Use the registry (see Q188602)
        //
        DWORD dw;
        //
        // See if the "Log on to Windows NT domain" check-box is on
        //
        if (!ReadRegDWORD (HKEY_LOCAL_MACHINE,
                           "Network\\Logon",
                           "LMLogon",
                           &dw))
        {
            Error ("Cannot read network info");
            return FALSE;
        }
        if (dw)
        {
            //
            // Part of an NT domain - get domain name
            //
            if (!ReadRegString (HKEY_LOCAL_MACHINE,
                                "System\\CurrentControlSet\\Services\\MSNP32\\NetworkProvider",
                                "AuthenticatingAgent",
                                lpstrDomain, 
                                MAX_PATH))
            {
                Error ("Cannot read network info");
                return FALSE;
            }
            return TRUE;
        }
        else
        {
            //
            // Get workgroup info
            //
            if (!ReadRegString (HKEY_LOCAL_MACHINE,
                                "System\\CurrentControlSet\\Services\\VxD\\VNETSUP",
                                "Workgroup",
                                lpstrDomain, 
                                MAX_PATH))
            {
                Error ("Cannot read network info");
                return FALSE;
            }
            return TRUE;
        }
    }
    else
    {
        //
        // Get domain name in Win NT (NetWkstaGetInfo) 
        //
        typedef NET_API_STATUS (NET_API_FUNCTION *PNETWKSTAGETINFO)(LPWSTR, DWORD, LPBYTE *);
        typedef NET_API_STATUS (NET_API_FUNCTION *PNETAPIBUFFERFREE) (LPVOID);

        HMODULE hMod = LoadLibrary ("NetApi32.dll");
        if (!hMod)
        {
            Error ("Cannot load NetApi32.dll");
            return FALSE;
        }
        PNETWKSTAGETINFO pNetWkstaGetInfo = (PNETWKSTAGETINFO)GetProcAddress (hMod, "NetWkstaGetInfo");
        PNETAPIBUFFERFREE pNetApiBufferFree = (PNETAPIBUFFERFREE)GetProcAddress (hMod, "NetApiBufferFree");
        if (!pNetWkstaGetInfo || !pNetApiBufferFree)
        {
            Error ("Cannot link to NetWkstaGetInfo or NetApiBufferFree");
            FreeLibrary (hMod);
            return FALSE;
        }
        PWKSTA_INFO_101 pWskInfo101;
        NET_API_STATUS Status;
        Status = pNetWkstaGetInfo (NULL, 101, (LPBYTE*)(&pWskInfo101));
        if (NERR_Success != Status)
        {
            Error ("Error in NetWkstaGetInfo", Status);
            FreeLibrary (hMod);
            return FALSE;
        }
        WideCharToMultiByte (CP_ACP,
                             0,
                             (LPWSTR) pWskInfo101->wki101_langroup,
                             -1,
                             lpstrDomain,
                             MAX_PATH,
                             NULL,
                             NULL);
        pNetApiBufferFree (pWskInfo101);
        FreeLibrary (hMod);
        return TRUE;
    }
}

BOOL 
GetGeneralUserName (
    BOOL  bWinNT,
    LPSTR lpstrUserName
)
{
    DWORD dwSize = MAX_PATH;
    if (!bWinNT)
    {
        //
        // Just get the user name.
        // We can get the domain of the user but this requires 16-bit code. Not gonna go there !!!!
        //
        if (!GetUserName (lpstrUserName, &dwSize))
        {
            Error ("Can't get user name");
            return FALSE;
        }
        return TRUE;
    }

typedef BOOL (WINAPI *POPENPROCESSTOKEN)(HANDLE, DWORD, PHANDLE);
typedef BOOL (WINAPI *PGETTOKENINFORMATION)(HANDLE, TOKEN_INFORMATION_CLASS, LPVOID, DWORD, PDWORD);
typedef BOOL (WINAPI *PLOOKUPACCOUNTSID)(LPCSTR, PSID, LPSTR, LPDWORD, LPSTR, LPDWORD, PSID_NAME_USE);

    BOOL bRes = FALSE;
    POPENPROCESSTOKEN pOpenProcessToken = NULL;
    PGETTOKENINFORMATION pGetTokenInformation = NULL;
    PLOOKUPACCOUNTSID pLookupAccountSid = NULL;
    HANDLE hTok = NULL;
    BYTE abTokenUser[128];
    DWORD dwReqSize;
    PSID pUserSid;
    CHAR szUserName[MAX_PATH / 2];
    DWORD dwUserNameSize = sizeof (szUserName);
    CHAR szDomainName[MAX_PATH / 2];
    DWORD dwDomainNameSize = sizeof (szDomainName);
    SID_NAME_USE SidNameUse;

    HMODULE hModAdvApi32 = LoadLibrary ("Advapi32.dll");
    if (!hModAdvApi32)
    {
        //
        // Try to get just the user name
        //
        goto exit;
    }

    pOpenProcessToken = (POPENPROCESSTOKEN)GetProcAddress (hModAdvApi32, "OpenProcessToken");
    pGetTokenInformation = (PGETTOKENINFORMATION)GetProcAddress (hModAdvApi32, "GetTokenInformation"); 
    pLookupAccountSid = (PLOOKUPACCOUNTSID)GetProcAddress(hModAdvApi32, "LookupAccountSidA");
    if (!pOpenProcessToken || !pGetTokenInformation || !pLookupAccountSid)
    {
        goto exit;
    }
    if (!pOpenProcessToken (GetCurrentProcess(),
                            TOKEN_QUERY,
                            &hTok))
    {
        goto exit;
    }
    if (!pGetTokenInformation(hTok,
                              TokenUser,
                              (LPVOID)abTokenUser,
                              sizeof(abTokenUser),
                              &dwReqSize))
    {
        goto exit;
    }
    //
    // Get the user name and domain.
    //
    pUserSid = ((TOKEN_USER *)abTokenUser)->User.Sid;

    if (!pLookupAccountSid(NULL,
                           pUserSid,
                           szUserName,
                           &dwUserNameSize,
                           szDomainName,
                           &dwDomainNameSize,
                           &SidNameUse))
    {
        goto exit;
    }
    sprintf (lpstrUserName, "%s\\%s", szDomainName, szUserName);
    bRes = TRUE;

exit:
    if (hTok)
    {
        CloseHandle(hTok);
    }
    if (hModAdvApi32)
    {
        FreeLibrary (hModAdvApi32);
    }
    if (!bRes)
    {
        //
        // Failed - do it the Win9x way
        //
        return GetGeneralUserName (FALSE, lpstrUserName);
    }
    return bRes;
}


BOOL 
GetINIDWord (
    LPCSTR lpcstrSection,
    LPCSTR lpcstrKey,
    LPDWORD lpdwValue
)
{
    CHAR szValue[20];

    GetPrivateProfileString (lpcstrSection,
                             lpcstrKey,
                             "Bullshit",
                             szValue,
                             sizeof (szValue),
                             g_szFullINIPath);
    if (!strcmp (szValue, "Bullshit"))
    {
        //
        // No value there
        //
        return FALSE;
    }
    CHAR *p;
    *lpdwValue = strtoul (szValue, &p, 10);
    return TRUE;
}

BOOL
ReadAndExpandString (
    LPCSTR lpcstrSection,
    LPCSTR lpcstrKey,
    LPSTR  lpstrValue,
    DWORD  dwValueLen,
    LPCSTR lpcstrDefault = ""
)
{
    CHAR szBuffer[1024];
    GetPrivateProfileString (lpcstrSection,
                             lpcstrKey,
                             lpcstrDefault,
                             szBuffer,
                             sizeof (szBuffer),
                             g_szFullINIPath);

    if (!ExpandEnvironmentStrings (szBuffer, lpstrValue, dwValueLen))
    {
        Error ("Can't expand environment variables");
        strncpy (lpstrValue, szBuffer, dwValueLen);
        return TRUE;
    }
    return TRUE;
}


BOOL
ParseCommandLine ()
{
    //
    // The command line should point to an INI file with the following format
    //
    // [General]
    // BackgroundColor = <COLORREF value>       ; The background color to use (default = 0 (black))
    // AutoBackground = [0 | 1]                 ; Use background color of current desktop (default = 0)
    // BackgroundBitmap = <path to BMP file>    ; Overrides AutoBackground and BackgroundColor values. Loads background bitmap from BMP file
    // XOffset = <horizontal offset in pixels>  ; Horizontal offest of the entire text block from the bitmap's center. Can be negative. Default = 0
    // YOffset = <vertical offset in pixels>    ; Vertical offest of the entire text block from the bitmap's center. Can be negative. Default = 0
    // Output = <file name>                     ; Name of output bitmap file (default = no file)
    // UpdateDesktop = [0 | 1]                  ; Update background desktop bitmap (default = 0)
    // ForceDesktopCenter = [0 | 1]             ; Force the desktop to display the bitmap as cenetered (instead of tiled / streched). Default = 1
    // LineSpacing = <value>                    ; Line spacing (default = 3)
    // SuppressErrors = [0 | 1]                 ; If 1, errors are NOT displayed (default = 0)
    //
    // [LineN]                                  ; Text settings for line N, where N between [1..20]
    // Font = <Face name>                       ; Font name (default = "Arial")
    // Size = <Font size>                       ; Font size (default = 22)
    // Color = <COLORREF value>                 ; Font color (default = WHITE)
    // Bold = [0 | 1]                           ; Font boldness (default = 0)
    // Italic = [0 | 1]                         ; Font italicness (default = 0)
    // Alignment = [Left | Right | Center]      ; Font alignment (default = Left)
    // ShadowX = <X offset value>               ; Shadow X offset (positive only, 0 = No X shadow. Default = 0)
    // ShadowY = <Y offset value>               ; Shadow Y offset (positive only, 0 = No Y shadow. Default = 0)
    // ShadowColor = <COLORREF value>           ; Shadow Color (default = 0 (black))
    // Type = [CompName | UserName | SysVer  |  ; Type of information to display on the line
    //         SysInfo  | NetInfo  | FileVer | 
    //         RegValue | FreeText | 
    //         UpdateTime | Unused]    
    //                                          ; CompName - Computer name
    //                                          ; UserName - User name
    //                                          ; SysVer   - Operating system version
    //                                          ; SysInfo  - Hardware information
    //                                          ; NetInfo  - Network information
    //                                          ; FileVer  - Version of a file specified in 'FileName' option
    //                                          ; RegValue - Registry string value. 
    //                                          ; Reg root from 'RegRoot' (e.g. HKLM)
    //                                          ; Reg path from 'RegPath' (e.g. SOFTWARE\Microsoft\Windows NT\CurrentVersion)
    //                                          ; Reg value from 'RegValue' (e.g. CurrentType)
    //                                          ; Reg title from 'RegTitle' (e.g. "The value of X is")
    //                                          ; FreeText - Text specified in 'Text' will be displayed as is
    //                                          ; UpdateTime - The date and time the bitmap was created
    //                                          ; Unused   - Line will not be displayed
    //
    //
    // RegRoot = [HKLM | HKCU]                  ; Registry root to use for 'Type' = 'RegValue'
    // RegPath = <Registry path to read from>   ; Registry path to use for 'Type' = 'RegValue'
    // RegValue = <Registry value to read from> ; Registry value to use for 'Type' = 'RegValue'. Must be of type REG_SZ
    // RegTitle = <Display title of read value> ; Registry value to use for 'Type' = 'RegValue'
    //
    // Text = <free text to display>            ; Free text to display. Used if 'Type' = 'FreeText'
    //
    // FilePath = <full path to file>           ; Path to file to display version for. Used if 'Type' = 'FileVer'
    // FileName = <display name of file>        ; Display name of file specified in 'FilePath'. Used if 'Type' = 'FileVer'
    //

    char *p;
    if (!(__argc > 1 && strlen (__argv[1])))
    {
        //
        // No command line paramter specified.
        // Try to locate backinfo.ini near backinfo.exe
        //
        GetFullPathName (__argv[0], sizeof (g_szFullINIPath), g_szFullINIPath, &p);
        if (strlen (g_szFullINIPath) > 3)
        {
            strcpy (&g_szFullINIPath[strlen (g_szFullINIPath) - 3], "ini");
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        GetFullPathName (__argv[1], sizeof (g_szFullINIPath), g_szFullINIPath, &p);
    }
        
    if (-1 == GetFileAttributes (g_szFullINIPath))
    {
        Error ("Cannot access configuration file", 0);
        return FALSE;
    }
    //
    // Read BackgroundColor
    //
    GetINIDWord ("General", "BackgroundColor", &g_clrBackground);
    //
    // Read AutoBackground
    //
    GetINIDWord ("General", "AutoBackground", (LPDWORD)&g_bAutoBackground);
    //
    // Read BackgroundBitmap
    //
    ReadAndExpandString ("General",
                         "BackgroundBitmap",
                         g_szBackgroundBitmapFileName,
                         sizeof (g_szBackgroundBitmapFileName));

    //
    // Read XOffset
    //
    g_ptOffest.x = (int) GetPrivateProfileInt ("General",
                                               "XOffset",
                                               0,
                                               g_szFullINIPath);

    //
    // Read YOffset
    //
    g_ptOffest.y = (int) GetPrivateProfileInt ("General",
                                               "YOffset",
                                               0,
                                               g_szFullINIPath);
    //
    // Read SuppressErrors
    //
    GetINIDWord ("General", "SuppressErrors", (LPDWORD)&g_bSuppressErrors);
    //
    // Read output file
    //
    ReadAndExpandString ("General",
                         "Output",
                         g_szFileName,
                         sizeof (g_szFileName));
    //
    // Read UpdateDesktop value
    //
    GetINIDWord ("General", "UpdateDesktop", (LPDWORD)&g_bUpdateDesktop);

    //
    // Read ForceDesktopCenter value
    //
    if (!GetINIDWord ("General", "ForceDesktopCenter", (LPDWORD)&g_bForceDesktopCenter))
    {
        //
        // Default is TRUE
        //
        g_bForceDesktopCenter = TRUE;
    }

    
    //
    // Read LineSpacing
    //
    GetINIDWord ("General", "LineSpacing", &g_dwLineSpacing);

    CHAR szSection[] = "Line00";
    for (DWORD dw=0; dw < NUM_STRINGS; dw++)
    {
        DWORD dwVal;
        sprintf (szSection, "Line%d", dw+1);
        CHAR szText[20];
        //
        // Read font name
        //
        ReadAndExpandString (szSection,
                             "Font",
                             Lines[dw].szFaceName,
                             sizeof (Lines[dw].szFaceName),
                             "Arial");
        //
        // Read font size
        //
        GetINIDWord (szSection, "Size", &Lines[dw].dwHeight);
        //
        // Read font color
        //
        GetINIDWord (szSection, "Color", &Lines[dw].clrText);
        //
        // Read font boldness
        //
        GetINIDWord (szSection, "Bold", &dwVal);
        if (dwVal)
        {
            Lines[dw].dwWeight = FW_BOLD;
            
        }
        //
        // Read font italicness
        //
        GetINIDWord (szSection, "Italic", &dwVal);
        if (dwVal)
        {
            Lines[dw].bItalic = TRUE;
            
        }
        //
        // Read font alignment
        //
        ReadAndExpandString (szSection,
                             "Alignment",
                             szText,
                             sizeof (szText),
                             "Left");
        if (!stricmp (szText, "Left"))
        {
            Lines[dw].dwAlign = TA_LEFT;
        }                
        else if (!stricmp (szText, "Right"))
        {
            Lines[dw].dwAlign = TA_RIGHT;
        }
        else if (!stricmp (szText, "Center"))
        {
            Lines[dw].dwAlign = TA_CENTER;
        }
        else
        {
            Error ("Bad font alignment value", dw);
            return FALSE;
        }
        //
        // Read ShadowX
        //
        GetINIDWord (szSection, "ShadowX", &Lines[dw].dwShadowXOffset);
        //
        // Read ShadowY
        //
        GetINIDWord (szSection, "ShadowY", &Lines[dw].dwShadowYOffset);
        //
        // Read ShadowColor
        //
        GetINIDWord (szSection, "ShadowColor", &Lines[dw].clrShadow);
        //
        // Read Type
        //
        ReadAndExpandString (szSection,
                             "Type",
                             szText,
                             sizeof (szText),
                             0 == dw ? "CompName" :
                                1 == dw ? "UserName" :
                                    2 == dw ? "SysVer" : 
                                        3 == dw ? "SysInfo" : "Unused");
        if (!stricmp (szText, "CompName"))
        {
            Lines[dw].Type = TEXT_MACHINE_NAME;
        }
        else if (!stricmp (szText, "UserName"))
        {
            Lines[dw].Type = TEXT_USER_NAME;
        }
        else if (!stricmp (szText, "SysVer"))
        {
            Lines[dw].Type = TEXT_SYS_VERSION;
        }
        else if (!stricmp (szText, "SysInfo"))
        {
            Lines[dw].Type = TEXT_SYS_INFO;
        }
        else if (!stricmp (szText, "NetInfo"))
        {
            Lines[dw].Type = TEXT_NET_INFO;
        }
        else if (!stricmp (szText, "UpdateTime"))
        {
            Lines[dw].Type = TEXT_TIME;
        }
        else if (!stricmp (szText, "RegValue"))
        {
            // 
            // We read the value and display it as if it's a free text
            //
            Lines[dw].Type = TEXT_FREE; 
            HKEY hRoot = NULL;
            CHAR szPath[255];
            CHAR szValue[255];
            CHAR szTitle[255];
            CHAR szReadValue[255];
            ReadAndExpandString (szSection,
                                 "RegRoot",
                                 szPath,
                                 sizeof (szPath));
            if (!stricmp ("HKLM", szPath))
            {
                hRoot = HKEY_LOCAL_MACHINE;
            }
            else if (!stricmp ("HKCU", szPath))
            {
                hRoot = HKEY_CURRENT_USER;
            }
            else
            {
                Error ("Bad registry root", 0);
                Lines[dw].Type = TEXT_UNUSED;  // Unused
                continue;
            }
            ReadAndExpandString (szSection,
                                 "RegPath",
                                 szPath,
                                 sizeof (szPath));
            ReadAndExpandString (szSection,
                                 "RegValue",
                                 szValue,
                                 sizeof (szValue));
            ReadAndExpandString (szSection,
                                 "RegTitle",
                                 szTitle,
                                 sizeof (szTitle));
            if (!ReadRegString (hRoot,
                                szPath,
                                szValue,
                                szReadValue,
                                MAX_PATH))
            {
                Error ("Can't read registry path");
                Lines[dw].Type = TEXT_UNUSED;  // Unused
                continue;
            }
            sprintf (Lines[dw].szText, "%s %s", szTitle, szReadValue);
        }
        else if (!stricmp (szText, "FreeText"))
        {
            Lines[dw].Type = TEXT_FREE;
            //
            // Read free text into szText buffer
            //
            ReadAndExpandString (szSection,
                                 "Text",
                                 Lines[dw].szText,
                                 sizeof (Lines[dw].szText));
            if (!strlen(Lines[dw].szText))
            {
                //
                // If we have nothing to display, disregard this line
                //
                Lines[dw].Type = TEXT_UNUSED;  // Unused
            }
        }
        else if (!stricmp (szText, "FileVer"))
        {
            // 
            // We read the value and display it as if it's a free text
            //
            Lines[dw].Type = TEXT_FREE; 
            CHAR szPath[MAX_PATH];
            CHAR szTitle[MAX_PATH];
            //
            // Read file path
            //
            ReadAndExpandString (szSection,
                                 "FilePath",
                                 szPath,
                                 sizeof (szPath));
            //
            // Read file title
            //
            ReadAndExpandString (szSection,
                                 "FileName",
                                 szTitle,
                                 sizeof (szTitle));
            if (!strlen (szTitle) || !strlen (szPath))
            {
                //
                // Partial info - line will not be displayed
                //
                Lines[dw].Type = TEXT_UNUSED;
                continue;
            }
            if (!GetFileVersion (szPath, szTitle, Lines[dw].szText))
            {
                //
                // Can't read file version - don't display line
                //
                Lines[dw].Type = TEXT_UNUSED;
                continue;
            }
        }
        else if (!stricmp (szText, "Unused"))
        {
            Lines[dw].Type = TEXT_UNUSED;
        }
        else
        {
            Error ("Bad line type", dw);
            return FALSE;
        }
    }
    return TRUE;
}


BOOL
CreateFonts ()
{
    for (DWORD dw=0; dw < NUM_STRINGS; dw++)
    {
        LOGFONT lf = {0};
        lf.lfHeight = Lines[dw].dwHeight;
        lf.lfWidth = 0; // Proportional to height
        strcpy (lf.lfFaceName, Lines[dw].szFaceName);
        lf.lfWeight = Lines[dw].dwWeight;
        lf.lfItalic = Lines[dw].bItalic;
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
        lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
        lf.lfQuality = PROOF_QUALITY;

        g_hFonts[dw] = CreateFontIndirect (&lf);
        if (NULL == g_hFonts[dw])
        {
            Error ("Cannot create font");
            DestroyFonts ();
            return FALSE;
        }
    }
    return TRUE;
}

BOOL
DestroyFonts ()
{   
    for (DWORD dw=0; dw < NUM_STRINGS; dw++)
    {
        if (NULL == g_hFonts[dw])
        {
            continue;
        }
        DeleteObject (g_hFonts[dw]);
    }
    return TRUE;
}

PBITMAPINFO 
CreateBitmapInfoStruct(
    HBITMAP hBmp
)
{ 
    BITMAP bmp; 
    PBITMAPINFO pbmi; 
    WORD        wClrBits; 

    //
    // Retrieve the bitmap's color format, width, and height. 
    //
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) 
    {
        Error ("Can't get bitmap info");
        return NULL;
    }
    //
    // Convert the color format to a count of bits. 
    //
    wClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
    if (wClrBits == 1) 
    {
        wClrBits = 1; 
    }
    else if (wClrBits <= 4) 
    {
        wClrBits = 4; 
    }
    else if (wClrBits <= 8) 
    {
        wClrBits = 8; 
    }
    else if (wClrBits <= 16) 
    {
        wClrBits = 16; 
    }
    else if (wClrBits <= 24) 
    {
        wClrBits = 24; 
    }
    else
    { 
        wClrBits = 32; 
    }
    //
    // Allocate memory for the BITMAPINFO structure. (This structure 
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
    // data structures.) 
    //
    if (wClrBits != 24) 
    {
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                                        sizeof(BITMAPINFOHEADER) + 
                                        sizeof(RGBQUAD) * (1<< wClrBits)); 
    }
    else
    {
        //
        // There is no RGBQUAD array for the 24-bit-per-pixel format. 
        //
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)); 
    }
    if (!pbmi)
    {
        Error ("Can't allocate saved data");
        return NULL;
    }
    //
    // Initialize the fields in the BITMAPINFO structure. 
    //
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (wClrBits < 24) 
    {
        pbmi->bmiHeader.biClrUsed = (1<<wClrBits); 
    }
    //
    // If the bitmap is not compressed, set the BI_RGB flag. 
    //
    pbmi->bmiHeader.biCompression = BI_RGB; 
    //
    // Compute the number of bytes in the array of color 
    // indices and store the result in biSizeImage. 
    // For Windows NT/2000/XP, the width must be DWORD aligned unless 
    // the bitmap is RLE compressed. This example shows this. 
    // For Windows 95/98, the width must be WORD aligned unless the 
    // bitmap is RLE compressed.
    //
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * wClrBits +31) & ~31) /8
                                  * pbmi->bmiHeader.biHeight; 
    //
    // Set biClrImportant to 0, indicating that all of the 
    // device colors are important. 
    //
    pbmi->bmiHeader.biClrImportant = 0; 
    return pbmi; 
} 


BOOL 
SaveBitmap (
    HDC hDC,
    HBITMAP hBitMap
)
{
    HANDLE hf = INVALID_HANDLE_VALUE;   // file handle 
    BITMAPFILEHEADER hdr;               // bitmap file-header 
    PBITMAPINFOHEADER pbih;             // bitmap info-header 
    LPBYTE lpBits = NULL;               // memory pointer 
    DWORD dwTotal;                      // total count of bytes 
    DWORD cb;                           // incremental count of bytes 
    BYTE *hp;                           // byte pointer 
    DWORD dwTmp; 
    BOOL bRes = FALSE;

    PBITMAPINFO pbi = CreateBitmapInfoStruct (hBitMap);
    if (!pbi)
    {
        return FALSE;
    }
    pbih = (PBITMAPINFOHEADER) pbi; 
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (!lpBits) 
    {
        Error ("Can't allocate memory");
        goto exit;
    }        
    //
    // Retrieve the color table (RGBQUAD array) and the bits 
    // (array of palette indices) from the DIB. 
    //
    if (!GetDIBits(hDC, 
                   hBitMap, 
                   0, 
                   (WORD) pbih->biHeight, 
                   lpBits, 
                   pbi, 
                   DIB_RGB_COLORS)) 
    {
        Error ("Can't get pixels");
        goto exit;
    }
    //
    // Create the .BMP file. 
    //
    hf = CreateFile(g_szFileName, 
                    GENERIC_READ | GENERIC_WRITE, 
                    0, 
                    NULL, 
                    CREATE_ALWAYS, 
                    FILE_ATTRIBUTE_NORMAL, 
                    NULL); 
    if (INVALID_HANDLE_VALUE == hf) 
    {
        Error ("Can't create file");
        goto exit;
    }
    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
    //
    // Compute the size of the entire file. 
    //
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
                          pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + 
                          pbih->biSizeImage); 
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 
    //
    // Compute the offset to the array of color indices. 
    //
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
                            pbih->biSize + pbih->biClrUsed * sizeof (RGBQUAD); 
    //
    // Copy the BITMAPFILEHEADER into the .BMP file. 
    //
    if (!WriteFile(hf, 
                   (LPVOID) &hdr, 
                   sizeof(BITMAPFILEHEADER), 
                   (LPDWORD) &dwTmp,  
                   NULL)) 
    {
        Error ("Can't write file");
        goto exit;
    }
    //
    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
    //
    if (!WriteFile(hf, 
                   (LPVOID) pbih, 
                   sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof (RGBQUAD), 
                   (LPDWORD) &dwTmp, 
                   NULL)) 
    {
        Error ("Can't write file");
        goto exit;
    }
    //
    // Copy the array of color indices into the .BMP file. 
    //
    dwTotal = cb = pbih->biSizeImage; 
    hp = lpBits; 
    if (!WriteFile(hf, 
                   (LPSTR) hp, 
                   (int) cb, 
                   (LPDWORD) &dwTmp,
                   NULL)) 
    {
        Error ("Can't write file");
        goto exit;
    }

    bRes = TRUE;

exit:

    if (INVALID_HANDLE_VALUE != hf)
    {
        CloseHandle (hf);
    }
    if (lpBits)
    {
        GlobalFree((HGLOBAL)lpBits);
    }
    if (pbi)
    {
        GlobalFree((HGLOBAL)pbi);
    }
    if (bRes && g_bUpdateDesktop)
    {
        if (g_bForceDesktopCenter)
        {
            //
            // Make sure the desktop bitmap is not tiled or stretched
            //
            HKEY hKey;
            DWORD dwRes;
            dwRes = RegOpenKeyEx( HKEY_CURRENT_USER,
                                  "Control Panel\\Desktop",
                                  0, 
                                  KEY_WRITE, 
                                  &hKey);
            if (ERROR_SUCCESS == dwRes)
            {
                RegSetValueEx (hKey,
                               "TileWallpaper",
                               0,
                               REG_SZ,
                               (LPBYTE)"0",
                               2);
                RegSetValueEx (hKey,
                               "WallpaperStyle",
                               0,
                               REG_SZ,
                               (LPBYTE)"0",
                               2);
                RegCloseKey (hKey);
            }
        }
        //
        // Tell the desktop to draw the bitmap we just created
        //
        if (!SystemParametersInfo (SPI_SETDESKWALLPAPER,
                                   0,
                                   (LPVOID)g_szFileName,
                                   SPIF_UPDATEINIFILE | SPIF_SENDCHANGE))
        {
            Error ("Cannot set wallpaper");
            bRes = FALSE;
        }
    }
    return bRes;
}

BOOL 
RenderBitmap ()
{
    HDC hDC= NULL;
    BOOL bRes = FALSE;
    BITMAPINFO BmpInfo = {0};
    LPVOID lpvBits;
    HDC hScreenDC = GetDC (NULL);
    SIZE szText = {0};
    DWORD dw;
    DWORD y = 0;
    HBRUSH hBrush = NULL;
    HBRUSH hbrshPrev = NULL;
    HBITMAP hbmpPrev = NULL;
    HBITMAP hBitMap;
    RECT rcFill = {0};
    POINT ptStart = {0};

    hDC = CreateCompatibleDC (hScreenDC);
    if (!hDC)
    {
        Error ("Cannot create DC");
        goto exit;
    }
    //
    // Measure the text rectangle
    //
    for (dw = 0; dw < NUM_STRINGS; dw++)
    {
        if (TEXT_UNUSED == Lines[dw].Type)
        {
            //
            // Line is unused
            //
            continue;
        }
        LPCSTR lpcstrText;
        if (TEXT_FREE == Lines[dw].Type)
        {
            //
            // Line is a free text line
            // Use string stored in line under FilePath
            //
            lpcstrText = Lines[dw].szText;
        }
        else
        {
            //
            // Line is one of the predefined strings.
            // Use Type as an index into g_szSystemData
            //
            lpcstrText = g_szSystemData[Lines[dw].Type];
        }
        DWORD  dwTextLen = (DWORD)strlen (lpcstrText);
        SIZE   szCurText;
        HFONT  hPrevFont;

        hPrevFont = (HFONT)SelectObject(hDC, g_hFonts[dw]);
        if (!GetTextExtentPoint (hDC, lpcstrText, dwTextLen, &szCurText))
        {
            Error ("Cannot get text size");
            goto exit;
        }
        SelectObject (hDC, hPrevFont);
        szCurText.cx += Lines[dw].dwShadowXOffset;
        szCurText.cy += Lines[dw].dwShadowYOffset;
        Lines[dw].dwPixelHeight = szCurText.cy;
        szText.cy += szCurText.cy + g_dwLineSpacing;
        szText.cx = max (szText.cx, szCurText.cx);
    }
    szText.cy -= g_dwLineSpacing;
    if (g_bAutoBackground)
    {
        //
        // Read desktop background color from the system
        //
        g_clrBackground = GetSysColor (COLOR_DESKTOP);
    }
    SetBkColor (hDC, GetNearestColor(hDC, g_clrBackground));
    if (!strlen (g_szBackgroundBitmapFileName))
    {
        //
        // Don't use exisiting bitmap as background image
        // Create the bitmap
        //
        BmpInfo.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
        BmpInfo.bmiHeader.biWidth = 2 * abs(g_ptOffest.x) + szText.cx;
        BmpInfo.bmiHeader.biHeight = 2 * abs(g_ptOffest.y) + szText.cy;
        BmpInfo.bmiHeader.biPlanes = 1;
        BmpInfo.bmiHeader.biBitCount = 32;          // TRUE-COLOR only
        BmpInfo.bmiHeader.biCompression = BI_RGB;   // No compression

        hBitMap = CreateDIBSection (hScreenDC, 
                                    &BmpInfo,
                                    DIB_RGB_COLORS,
                                    &lpvBits,
                                    NULL,
                                    NULL
                                    );
        if (!hBitMap)
        {
            Error ("Cannot create bitmap in memory");
            goto exit;
        }
        //
        // Select bitmap into DC
        //
        hbmpPrev = (HBITMAP) SelectObject (hDC, hBitMap);
        //
        // Clear all to background color
        //
        hBrush = CreateSolidBrush (GetNearestColor(hDC, g_clrBackground));
        if (!hBrush)
        {
            Error ("Cannot create solid brush");
            goto exit;
        }
        hbrshPrev = (HBRUSH) SelectObject (hDC, hBrush);
        rcFill.bottom = 2 * abs(g_ptOffest.y) + szText.cy;
        rcFill.right = 2 * abs(g_ptOffest.x) + szText.cx;
        if (!FillRect (hDC,
                       &rcFill,
                       hBrush))
        {
            Error ("Cannot draw background");
            goto exit;
        }
        //
        // Calc text box start position
        //
        ptStart.x = abs(g_ptOffest.x);
        ptStart.y = abs(g_ptOffest.y);
    }
    else
    {
        //
        // Background image specified - load it
        //
        BITMAPINFO bmi;
        bmi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
        bmi.bmiHeader.biBitCount = 0;
        hBitMap = (HBITMAP) LoadImage (NULL,
                                       g_szBackgroundBitmapFileName,
                                       IMAGE_BITMAP,
                                       0,
                                       0,
                                       LR_CREATEDIBSECTION | LR_LOADFROMFILE);
        if (NULL == hBitMap)
        {
            Error ("Cannot load bitmap file");
            goto exit;
        }
        if (!g_bForceDesktopCenter && g_bUpdateDesktop)
        {
            //
            // We will not force the desktop to center our bitmap.
            // Check and see if the desktop will resize it.
            //

            HKEY hKey;
            DWORD dwRes;
            CHAR szVal[10];
            DWORD dwSize = sizeof (szVal);
            dwRes = RegOpenKeyEx( HKEY_CURRENT_USER,
                                  "Control Panel\\Desktop",
                                  0, 
                                  KEY_READ, 
                                  &hKey);
            if (ERROR_SUCCESS == dwRes)
            {
                RegQueryValueEx (hKey,
                                 "WallpaperStyle",
                                 0,
                                 NULL,
                                 (LPBYTE)szVal,
                                 &dwSize);
                RegCloseKey (hKey);
                if (!strcmp ("2", szVal))
                {
                    //
                    // The desktop will stretch our bitmap.
                    // Let's see if we can stretch it ourselves before drawing text on it
                    //
                    int iScreenWidth = GetSystemMetrics (SM_CXSCREEN);
                    int iScreenHeight = GetSystemMetrics (SM_CYSCREEN);
                    if (!GetDIBits (hDC, 
                                    hBitMap,
                                    0,
                                    0,
                                    NULL,
                                    &bmi,
                                    DIB_RGB_COLORS))
                    {
                        Error ("Cannot get bitmap info");
                        goto exit;
                    }
                    if (((iScreenWidth < bmi.bmiHeader.biWidth) && (iScreenHeight < bmi.bmiHeader.biHeight)) ||
                        ((iScreenWidth > bmi.bmiHeader.biWidth) && (iScreenHeight > bmi.bmiHeader.biHeight)))
                    {
                        //
                        // We can try and use our scaling code now
                        //
                        HBITMAP hScaledBmp = ScaleBitmapInt (hBitMap, iScreenWidth, iScreenHeight);
                        if (NULL == hScaledBmp)
                        {
                            Error ("Cannot scale bitmap");
                            goto exit;
                        }
                        DeleteObject (hBitMap);
                        hBitMap = hScaledBmp;
                    }
                }
            }
        }
        //
        // Select bitmap into DC
        //
        hbmpPrev = (HBITMAP) SelectObject (hDC, hBitMap);
        //
        // Calc start point
        //
        bmi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
        bmi.bmiHeader.biBitCount = 0;
        if (!GetDIBits (hDC, 
                        hBitMap,
                        0,
                        0,
                        NULL,
                        &bmi,
                        DIB_RGB_COLORS))
        {
            Error ("Cannot get bitmap info");
            goto exit;
        }

        ptStart.x = bmi.bmiHeader.biWidth  / 2 - szText.cx / 2;
        ptStart.y = bmi.bmiHeader.biHeight / 2 - szText.cy / 2;
    }
    //
    // Apply offset to start point
    //
    ptStart.x += g_ptOffest.x;
    ptStart.y += g_ptOffest.y;
    //
    // Draw all strings
    //
    SetBkMode (hDC, TRANSPARENT);
    for (dw = 0; dw < NUM_STRINGS; dw++)
    {
        if (TEXT_UNUSED == Lines[dw].Type)
        {
            //
            // Line is unused
            //
            continue;
        }
        LPCSTR lpcstrText;
        if (TEXT_FREE == Lines[dw].Type)
        {
            //
            // Line is a free text line
            // Use string stored in line under FilePath
            //
            lpcstrText = Lines[dw].szText;
        }
        else
        {
            //
            // Line is one of the predefined strings.
            // Use Type as an index into g_szSystemData
            //
            lpcstrText = g_szSystemData[Lines[dw].Type];
        }
        DWORD  dwTextLen = (DWORD)strlen (lpcstrText);
        HFONT  hPrevFont;
        DWORD  x = 0;

        SetTextAlign (hDC, Lines[dw].dwAlign);
        hPrevFont = (HFONT)SelectObject(hDC, g_hFonts[dw]);

        if (TA_CENTER == Lines[dw].dwAlign)
        {
            x = szText.cx / 2;
        }
        else if (TA_RIGHT == Lines[dw].dwAlign)
        {
            x = szText.cx;
        }
        if (Lines[dw].dwShadowXOffset || Lines[dw].dwShadowXOffset)
        {
            //
            // Draw shadow first
            //
            SetTextColor (hDC, GetNearestColor(hDC, Lines[dw].clrShadow));

            if (!TextOut (hDC, 
                          ptStart.x + x + Lines[dw].dwShadowXOffset, 
                          ptStart.y + y + Lines[dw].dwShadowYOffset, 
                          lpcstrText, 
                          dwTextLen))
            {
                Error ("can't write text");
                goto exit;
            }
        }
        SetTextColor (hDC, GetNearestColor(hDC, Lines[dw].clrText));

        if (!TextOut (hDC, ptStart.x + x, ptStart.y + y, lpcstrText, dwTextLen))
        {
            Error ("can't write text");
            goto exit;
        }
        SelectObject (hDC, hPrevFont);
        y += (Lines[dw].dwPixelHeight + g_dwLineSpacing);
    }

    bRes = SaveBitmap (hDC, hBitMap);

exit:
    
    if (hbrshPrev)
    {
        SelectObject (hDC, hbrshPrev);
    }
    if (hbmpPrev)
    {
        SelectObject (hDC, hbmpPrev);
    }
    if (hBrush)
    {
        DeleteObject (hBrush);
    }
    if (hBitMap)
    {
        DeleteObject (hBitMap);
    }
    if (hDC)
    {
        DeleteDC (hDC);
    }
    if (hScreenDC)
    {
        ReleaseDC (NULL, hScreenDC);
    }
    return bRes;
}

void 
FixSPString (
    CHAR *szSP
)
{
    CHAR szTemp[130];

    if (strlen (szSP))
    {
        //
        // Service pack is NOT empty string
        //

        //
        // Trim left
        //
        CHAR *p = szSP;
        while (*p)
        {
            if (isspace(*p) || iscntrl(*p))
            {
                //
                // Still nothing to display
                //
                p++;
                continue;
            }
            //
            // First real displayable character
            //
            strcpy (szTemp, p);
            break;
        }
        if (!(*p))
        {
            //
            // All spaces - clear string
            //
            szSP[0]='\0';
            return;
        }
        //
        // Trim right
        //
        p = szTemp + strlen (szTemp) - 1;
        while (p > szTemp)
        {
            if (isspace(*p) || iscntrl(*p))
            {
                //
                // Still nothing to display
                //
                *p = '\0';
                p--;
                continue;
            }
            break;
        }
        //
        // Update output buffer
        //
        if (strlen (szTemp))
        {
            sprintf (szSP, " %s", szTemp);
        }
        else
        {
            szSP[0]='\0';
        }
    }
}
   
LPSTR 
GetSuiteName (OSVERSIONINFOEX *pOSVI)
{
    if (pOSVI->wSuiteMask & VER_SUITE_BLADE)
    {
        return "Web Server ";
    }
    if (pOSVI->wSuiteMask & (VER_SUITE_SMALLBUSINESS | VER_SUITE_SMALLBUSINESS_RESTRICTED))
    {
        return "Small Business Server ";
    }
    if (pOSVI->wSuiteMask & VER_SUITE_DATACENTER)
    {
        return "Datacenter Server ";
    }
    if (pOSVI->wSuiteMask & VER_SUITE_ENTERPRISE)
    {
        return "Enterprise Server ";
    }
    if (pOSVI->wSuiteMask & VER_SUITE_PERSONAL)
    {
        return "Home Edition ";
    }
    if (VER_NT_WORKSTATION == pOSVI->wProductType)
    {
        //
        // Either 'Professional' or 'Workstation', depending on Win2K/WinXP or WinNT4.
        //
        if (pOSVI->dwMajorVersion <= 4)
        {
            return "Workstation";
        }
        else
        {
            return "Professional ";
        }
    }
    if (VER_NT_SERVER == pOSVI->wProductType)
    {
        return "Standard Server ";
    }
    if (VER_NT_DOMAIN_CONTROLLER == pOSVI->wProductType)
    {
        return "Server (Domain controller) ";
    }
    return "Unknown suite ";
}   // GetSuiteName

BOOL 
CollectData ()
{
    BOOL bRes = FALSE;
    BOOL bWinNT = FALSE;
    BOOL bWhistler = FALSE;
    //
    // Get computer name
    //
    DWORD dwCompNameSize = sizeof (g_szSystemData[TEXT_MACHINE_NAME]);
    if (!GetComputerName(g_szSystemData[TEXT_MACHINE_NAME], &dwCompNameSize))
    {
        Error ("Can't get computer name");
        return FALSE;
    }
    //
    // Get system version
    //
    OSVERSIONINFOEX osvi;
    BOOL bOsVersionInfoEx;
    //
    // Try calling GetVersionEx using the OSVERSIONINFOEX structure,
    // which is supported on Windows 2000/XP.
    //
    // If that fails, try using the OSVERSIONINFO structure.
    //
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
    {
        //
        // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
        //
        osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
        if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
        {
            Error ("Can't get system version");
            return FALSE;
        }
    }

    FixSPString (osvi.szCSDVersion);

    switch (osvi.dwPlatformId)
    {
        case VER_PLATFORM_WIN32_NT: 
            //
            // Test for the product.
            //
            bWinNT = TRUE;
            
            if (osvi.dwMajorVersion <= 4)
            {
                strcpy (g_szSystemData[TEXT_SYS_VERSION], "Microsoft Windows NT ");
            }
            else if (5 == osvi.dwMajorVersion)
            {
                if (0 == osvi.dwMinorVersion)
                {
                    strcpy (g_szSystemData[TEXT_SYS_VERSION], "Microsoft Windows 2000 ");
                }
                else if (1 == osvi.dwMinorVersion)
                {
                    //
                    // Windows XP /.NET Server
                    //
                    if (IsDesktopSKU(&osvi))
                    {
                        strcpy (g_szSystemData[TEXT_SYS_VERSION], "Microsoft Windows XP ");
                    }
                    else
                    {
                        strcpy (g_szSystemData[TEXT_SYS_VERSION], "Microsoft Windows .NET ");
                    }        
                    bWhistler = TRUE;
                }
                else
                {
                    sprintf (g_szSystemData[TEXT_SYS_VERSION], 
                             "Microsoft Windows NT version %d.%d ",
                             osvi.dwMajorVersion,
                             osvi.dwMinorVersion);
                }
            }
            else 
            {
                sprintf (g_szSystemData[TEXT_SYS_VERSION], 
                         "Microsoft Windows NT version %d.%d ",
                         osvi.dwMajorVersion,
                         osvi.dwMinorVersion);
            }
            if( bOsVersionInfoEx )
            {
                strcat (g_szSystemData[TEXT_SYS_VERSION], GetSuiteName(&osvi));
                //
                // Test for suite modifiers
                //
                if ((osvi.wSuiteMask & VER_SUITE_TERMINAL) && !bWhistler)
                {
                    //
                    // Windows XP / .NET has TS flag always on
                    //
                    strcat (g_szSystemData[TEXT_SYS_VERSION], "(Terminal server) ");
                }
                if (osvi.wSuiteMask & VER_SUITE_EMBEDDEDNT)
                {
                    strcat (g_szSystemData[TEXT_SYS_VERSION], "(Embedded) ");
                }
            }
            else
            {
                CHAR szProductType[80];

                if (ReadRegString (HKEY_LOCAL_MACHINE,
                                   "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
                                   "ProductType", 
                                   szProductType, 
                                   sizeof (szProductType)))
                {
                    if ( strcmpi( "WINNT", szProductType) == 0 )
                    {
                        strcat (g_szSystemData[TEXT_SYS_VERSION], "Workstation ");
                    }
                    if ( strcmpi( "SERVERNT", szProductType) == 0 )
                    {
                        strcat (g_szSystemData[TEXT_SYS_VERSION], "Server ");
                    }
                }
            }
            //
            // Add service pack (if any) and build number.
            //
            sprintf (g_szSystemData[TEXT_SYS_VERSION], 
                     "%s%s (build %d)",
                     g_szSystemData[TEXT_SYS_VERSION],
                     osvi.szCSDVersion,
                     osvi.dwBuildNumber & 0xFFFF);
            break;
        case VER_PLATFORM_WIN32_WINDOWS:
            if (4 == osvi.dwMajorVersion &&
                osvi.dwMinorVersion > 0)
            {
                //
                // Either Win98 or WinME
                //
                if (osvi.dwMinorVersion < 80)
                {
                    //
                    // Win98
                    //
                    strcpy (g_szSystemData[TEXT_SYS_VERSION], "Microsoft Windows 98 ");
                }
                else
                {
                    //
                    // WinME
                    //
                    strcpy (g_szSystemData[TEXT_SYS_VERSION], "Microsoft Windows Millennium Edition ");
                }
            }
            else
            {
                //
                // Win95
                //
                strcpy (g_szSystemData[TEXT_SYS_VERSION], "Microsoft Windows 95 ");
            }
            //
            // Add service pack (if any) and build number.
            //
            sprintf (g_szSystemData[TEXT_SYS_VERSION], 
                     "%s%s (build %d)",
                     g_szSystemData[TEXT_SYS_VERSION],
                     osvi.szCSDVersion,
                     osvi.dwBuildNumber & 0xFFFF);
            break;

        case VER_PLATFORM_WIN32s:
            sprintf (g_szSystemData[TEXT_SYS_VERSION],
                     "Microsoft Win32s%s (build %d)",
                     osvi.szCSDVersion,
                     osvi.dwBuildNumber & 0xFFFF);
            break;

        default:
            sprintf (g_szSystemData[TEXT_SYS_VERSION],
                     "Unknown system type %d.%d%s (build %d)",
                     osvi.dwMajorVersion,
                     osvi.dwMinorVersion,
                     osvi.szCSDVersion,
                     osvi.dwBuildNumber & 0xFFFF);
            break;
    }
    //
    // Get user name
    //
    CHAR szUser[MAX_PATH];
    if (!GetGeneralUserName (bWinNT, szUser))
    {
        return FALSE;
    }
    _strlwr(szUser);
    sprintf (g_szSystemData[TEXT_USER_NAME], "Logged on user: %s", szUser);

    sprintf (g_szSystemData[TEXT_SYS_INFO],
             "CPU: %d at %s    RAM: %s",
             GetNumCPUs(),
             GetFormattedFirstCPUSpeed(),
             GetFormattedSystemRAM());

	//
	//	Update time
	//
    sprintf (g_szSystemData[TEXT_TIME],
             "Updated at: %s",
              GetUpdateTime());
    //
    // Read network info
    //
    CHAR szIP[1024];
    if (GetIPAddress (szIP))
    {
        //
        // We successfuly got the network IP address
        //
        sprintf (g_szSystemData[TEXT_NET_INFO],
                 "IP address: %s",
                 szIP);
    }
    CHAR szDomain[MAX_PATH];
    if (GetMachineDomain (bWinNT, szDomain))
    {
        //
        // We successfully got the machine's domain name - append to IP info
        //
        _strlwr(szDomain);
        if (strlen (g_szSystemData[TEXT_NET_INFO]))
        {
            //
            // Space IP and domain name
            //
            strcat (g_szSystemData[TEXT_NET_INFO], "     ");
        }
        sprintf (g_szSystemData[TEXT_NET_INFO],
                 "%sMachine domain: %s",
                 g_szSystemData[TEXT_NET_INFO],
                 szDomain);
    }
    return TRUE;
}   // CollectData


DWORD
GetNumCPUs ()
{
    SYSTEM_INFO SysInfo;
    GetSystemInfo (&SysInfo);
    return SysInfo.dwNumberOfProcessors;
}   // GetNumCPUs


BOOL
GetFirstCPUSpeed (
    LPDWORD lpdwCPUSpeed
)
{
    //
    // Try to read the CPU speed from HKLM\HARDWARE\Description\System\CentralProcessor\0\~MHz
    // This is the speed as determined at boot time.
    //
    if (ReadRegDWORD (HKEY_LOCAL_MACHINE,
                      "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                      "~MHz",
                      lpdwCPUSpeed,
                      TRUE))
    {
        return TRUE;
    }
    //
    // Try using Intel's CPUInfo library to measure the CPU speed
    //
    FREQ_INFO CPUFreq = cpuspeed(0);
    //
    // Intel yields freq rates a bit higher than actual CPU speed. Decrease by one.
    // The GetFormattedFirstCPUSpeed() function will take care of the roundup.
    //
    *lpdwCPUSpeed = CPUFreq.norm_freq - 1;
    return TRUE;
}   // GetFirstCPUSpeed

DWORDLONG
GetSystemRAM ()
{
    DWORDLONG dwlRes;

    MEMORYSTATUS MemStat;
    GlobalMemoryStatus (&MemStat);
    dwlRes = MemStat.dwTotalPhys;

    HMODULE hMod = LoadLibrary ("Kernel32.dll");
    if (!hMod)
    {
        return dwlRes;
    }

    typedef BOOL (WINAPI *PGLOBALMEMORYSTATUSEX) (LPMEMORYSTATUSEX);
    PGLOBALMEMORYSTATUSEX pGlobalMemoryStatusEx = (PGLOBALMEMORYSTATUSEX)GetProcAddress (hMod, "GlobalMemoryStatusEx");
    if (!pGlobalMemoryStatusEx)
    {
        FreeLibrary (hMod);
        return dwlRes;
    }
    MEMORYSTATUSEX MemStatEx = {0};
    MemStatEx.dwLength = sizeof (MEMORYSTATUSEX);
    if (!pGlobalMemoryStatusEx (&MemStatEx))
    {
        FreeLibrary (hMod);
        return dwlRes;
    }
    FreeLibrary (hMod);
    dwlRes = MemStatEx.ullTotalPhys;
    return dwlRes;
}   // GetSystemRAM

LPSTR 
GetFormattedSystemRAM ()
{
    static char szFormattedRAM[100];
    
    #define KB_SIZE 1024
    #define MB_SIZE (KB_SIZE*KB_SIZE)
    #define GB_SIZE (MB_SIZE*KB_SIZE)
    #define FOUR_MB_SIZE (4*MB_SIZE)

    DWORDLONG dwlRam = GetSystemRAM();

    if (0 != dwlRam % FOUR_MB_SIZE)
    {
        //
        // Roundup RAM to the next 4MB boundary
        //
        dwlRam = (dwlRam / FOUR_MB_SIZE + 1) * FOUR_MB_SIZE;
    }
    if (dwlRam > GB_SIZE)
    {
        //
        // We have at least 1GB ram
        //
        float f = float(DWORD(dwlRam / MB_SIZE)) / float(KB_SIZE);
        if (int(f * float(10)) - (int(f) * 10) > 0)
        {
            //
            // We have x.yzzzzzz GB of ram and y>0.
            // Print first digit after period
            //
            sprintf (szFormattedRAM, "%-4.1fGB", f);
        }
        else
        {
            //
            // We have x.yzzzzzz GB of ram and y=0.
            // Print as integer value.
            //
            sprintf (szFormattedRAM, "%-4.0fGB", f);
        }
    }
    else
    {
        //
        // We have less that 1GB ram. Print integer size only since we did a roundup already.
        //
        sprintf (szFormattedRAM, "%dMB", DWORD(dwlRam / MB_SIZE));
    }
    return szFormattedRAM;
}   // GetFormattedSystemRAM


LPSTR 
GetFormattedFirstCPUSpeed ()
{
    static char szFormattedCPUSpeed[100];

    DWORD dwCPU;
    DWORD dwAdd = 0;

    if (!GetFirstCPUSpeed (&dwCPU))
    {
        strcpy (szFormattedCPUSpeed, "unknown speed");
        return szFormattedCPUSpeed;
    }
    if (0 == dwCPU % 100)
    {
        //
        // We're on a precise 100Mhz boundary - no need to fixup
        //
    }
    else
    {
        //
        // Roundup CPU speed to the next 33MHz
        //
        dwAdd = dwCPU % 100;
        if (0 != dwAdd % 33)
        {
            dwAdd = ((dwAdd / 33 + 1) * 33) - dwAdd;
        }
        else
        {
            dwAdd = 0;
        }
    }
    dwCPU += dwAdd;
    if (99 == dwCPU % 100)
    {
        //
        // Round up to 100Mhz
        //
        dwCPU += 1;
    }

    sprintf (szFormattedCPUSpeed, "%d MHz", dwCPU);
    return szFormattedCPUSpeed;
}   // GetFormattedFirstCPUSpeed


BOOL
GetFileVersion (
    LPSTR       lpstrFilePath,
    LPSTR       lpstrFileDisplayName,
    LPSTR       lpstrVersion
)
{
    DWORD dwRes = ERROR_SUCCESS;
    DWORD dwVerInfoSize;
    DWORD dwVerHnd=0;           // An ignored parameter, always 0
    LPBYTE lpbVffInfo = NULL;
    VS_FIXEDFILEINFO *pFixedFileInfo;
    UINT uVersionDataLength;
    BOOL bRes = FALSE;
    //
    // Find size needed for version information
    //
    dwVerInfoSize = GetFileVersionInfoSize (lpstrFilePath, &dwVerHnd);
    if (0 == dwVerInfoSize)
    {
        Error ("Can't get version size");
        return FALSE;
    }
    //
    // Allocate memory for file version info
    //
    lpbVffInfo = (LPBYTE)GlobalAlloc (GMEM_FIXED, dwVerInfoSize);
    if (NULL == lpbVffInfo)
    {
        Error ("Can't allocate version buffer");
        return FALSE;
    }
    //
    // Try to get the version info
    //
    if (!GetFileVersionInfo(
        lpstrFilePath,
        dwVerHnd,
        dwVerInfoSize,
        (LPVOID)lpbVffInfo))
    {
        Error ("Can't read version buffer");
        goto exit;
    }
    //
    // Query the required version structure
    //
    if (!VerQueryValue (
        (LPVOID)lpbVffInfo,
        TEXT ("\\"),    // Retrieve the VS_FIXEDFILEINFO struct
        (LPVOID *)&pFixedFileInfo,
        &uVersionDataLength))
    {
        Error ("Can't read version info");
        goto exit;
    }
    sprintf (lpstrVersion, 
             "%s version %d.%d%s (Build %d.%d)",
             lpstrFileDisplayName,
             WORD((pFixedFileInfo->dwProductVersionMS) >> 16),              // Major ver
             WORD((pFixedFileInfo->dwProductVersionMS) & 0x0000ffff),       // Minor ver
             (pFixedFileInfo->dwFileFlags & VS_FF_DEBUG) ? " checked" : "", // Debug flag
             WORD((pFixedFileInfo->dwProductVersionLS) >> 16),              // Major build
             WORD((pFixedFileInfo->dwProductVersionLS) & 0x0000ffff));      // Minor build

    bRes = TRUE;

exit:
    if (lpbVffInfo)
    {
        GlobalFree((HGLOBAL)lpbVffInfo);
    }
    return bRes;
}   // GetFileVersion


LPSTR
GetUpdateTime ()
{
	static char szUpdateTime[200];
	time_t ltime;
	time( &ltime );
	strcpy(szUpdateTime, ctime( &ltime ));
	szUpdateTime[strlen(szUpdateTime)-1]=NULL; // Remove \n character
    return szUpdateTime;
}   // GetUpdateTime

BOOL
IsDesktopSKU (
    LPOSVERSIONINFOEX lposvi
)
{
    if (lposvi->wSuiteMask & VER_SUITE_PERSONAL)
    {
        return TRUE;
    }
    if (lposvi->wSuiteMask & VER_SUITE_ENTERPRISE)
    {
        return FALSE;
    }
    if (lposvi->wSuiteMask & VER_SUITE_DATACENTER)
    {
        return FALSE;
    }
    if (lposvi->wProductType == VER_NT_WORKSTATION)
    {
        return TRUE;
    }
    return FALSE;
}   // IsDesktopSKU

int 
WINAPI 
WinMain (
    HINSTANCE hInstance,      // handle to current instance
    HINSTANCE hPrevInstance,  // handle to previous instance
    LPSTR lpCmdLine,          // command line
    int nCmdShow              // show state
)
{
    if (2 == __argc || 1 == __argc)
    {
        if (!ParseCommandLine())
        {
            return 0;
        }
    }
    if (!CollectData())
    {
        return 0;
    }
    if (!strlen (g_szFileName))
    {
        //
        // No output bitmap - use message box
        //
        CHAR szMsg[MAX_PATH*6];
        sprintf (szMsg,
                 "%s\n%s\n%s\n%s\n%s",
                 g_szSystemData[TEXT_MACHINE_NAME],
                 g_szSystemData[TEXT_USER_NAME],
                 g_szSystemData[TEXT_SYS_VERSION],
                 g_szSystemData[TEXT_SYS_INFO],
                 g_szSystemData[TEXT_NET_INFO]);
        MessageBox (NULL, szMsg, "Sytem information", MB_OK);
    }
    else
    {
        //
        // Output bitmap to file
        //
        if (CreateFonts())
        {
            RenderBitmap();
            DestroyFonts ();
        }
    }
  
    return 0;
}


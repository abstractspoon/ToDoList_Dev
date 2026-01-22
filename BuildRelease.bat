title ToDoList_Dev Release Build

ECHO OFF
CLS

ECHO ToDoList_Dev Release Build
ECHO ==========================
ECHO:

PUSHD %~dp0
SET REPO=%CD%
ECHO REPO = %REPO%
ECHO:

IF NOT EXIST %REPO%\Core EXIT
IF NOT EXIST %REPO%\Plugins EXIT

ECHO [106m[30m Remember to update ToDoList version number[0m
ECHO [106m[30m Remember to pull latest translations[0m
ECHO:
PAUSE
ECHO:

REM - Detours
REM "C:\Program Files (x86)\Microsoft Visual Studio\Common\MSDev98\Bin\msdev.exe" .\3rdParty\Detours\Detours.dsw /MAKE "ALL - Win32 Unicode Release"

REM - Build core app in VC6
ECHO Building ToDoList Core 
ECHO ======================
ECHO:

SET MSDEV="C:\Program Files (x86)\Microsoft Visual Studio\Common\MSDev98\Bin\msdev.exe"
SET OUTPUT_FILE=%REPO%\Core\ToDoList\Unicode_Release\Core_Build_Output.txt

REM - WE USE FULL PATHS EVERYWHERE ELSE EXCEPT HERE BECAUSE 
REM - MSDEV ASSUMES THAT THE SOLUTION IS PROVIDED BY NAME ONLY
SET SOLUTION=ToDoList_Core.dsw

ECHO MSDEV    = %MSDEV%
ECHO SOLUTION = %REPO%\Core\%SOLUTION%

IF NOT EXIST %MSDEV% (
ECHO [41m Unable to locate MSDev.exe!![0m
ECHO:
PAUSE
EXIT
)

MKDIR %REPO%\Core\ToDoList\Unicode_Release 2> NUL
DEL %OUTPUT_FILE% 2> NUL

IF EXIST %OUTPUT_FILE% (
ECHO [41m FAILED to delete Core_Build_Output.txt[0m
ECHO:
PAUSE
EXIT
)

cd %REPO%\Core
%MSDEV% ToDoList_Core.dsw /MAKE "ALL - Win32 Unicode Release" /OUT %OUTPUT_FILE% 
ECHO:

REM - Check for compile errors
FINDSTR /C:") : error" %OUTPUT_FILE%
IF %errorlevel%==1 (

REM - Check for include errors
FINDSTR /C:") : fatal error" %OUTPUT_FILE%
IF %errorlevel%==1 (

REM - All other errors
FINDSTR /C:"Error " %OUTPUT_FILE%
)
)

IF %errorlevel%==0 (
ECHO [41m Build FAILED[0m
ECHO:
REM Open the build log
%OUTPUT_FILE%
PAUSE
EXIT
)

REM SUCCESS!
ECHO [42m Build SUCCEEDED[0m
ECHO:

REM Run units tests
ECHO Running Unit Tests
ECHO ==================
ECHO:

SET TDLTEST=%REPO%\Core\TDLTest\Unicode_Release\TDLTest.exe
SET OUTPUT_FILE=%REPO%\Core\TDLTest\Unicode_Release\Test_Output.txt

DEL %OUTPUT_FILE% 2> NUL

IF EXIST %OUTPUT_FILE% (
ECHO [41m FAILED to delete Test_Output.txt[0m
ECHO:
PAUSE
EXIT
)

%TDLTEST% > %OUTPUT_FILE%

REM - Check for test errors
FINDSTR /C:"tests FAILED" %OUTPUT_FILE%

IF %errorlevel%==0 (
ECHO [41m Tests FAILED[0m
ECHO:
REM Open the test log
%OUTPUT_FILE%
PAUSE
EXIT
)

REM - Check for test success
FINDSTR /C:"tests SUCCEEDED" %OUTPUT_FILE% > nul

IF %errorlevel%==1 (
ECHO [41m Test Results EMPTY[0m
ECHO:
PAUSE
EXIT
)

REM SUCCESS!
ECHO [42m Tests SUCCEEDED[0m
ECHO:

REM - Build plugins using MSBuild
ECHO Building ToDoList Plugins
ECHO =========================
ECHO:

SET MSBUILD="C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe"
SET SOLUTION=%REPO%\Plugins\ToDoList_Plugins.sln
SET OUTPUT_FILE=%REPO%\Plugins\Release\Plugins_Build_Output.txt

ECHO MSBUILD  = %MSBUILD%
ECHO SOLUTION = %SOLUTION%
ECHO:

IF NOT EXIST %MSBUILD% (
ECHO [41m Unable to locate MSBuild.exe!![0m
ECHO:
PAUSE
EXIT
)

MKDIR %REPO%\Plugins\Release 2> NUL
DEL %OUTPUT_FILE% 2> NUL

IF EXIST %OUTPUT_FILE% (
ECHO [41m FAILED to delete Plugins_Build_Output.txt[0m
ECHO:
PAUSE
EXIT
)

%MSBUILD% %SOLUTION% /t:Build /p:Configuration=Release /m /v:normal > %OUTPUT_FILE%

REM - Check for compile errors
FINDSTR /C:"): error" %OUTPUT_FILE%
IF %errorlevel%==1 (

REM - Check for link errors
FINDSTR /C:": fatal error" %OUTPUT_FILE%
IF %errorlevel%==1 (

REM - All other errors
FINDSTR /C:"Build FAILED." %OUTPUT_FILE%
)
)

IF %errorlevel%==0 (
ECHO [41m Build FAILED[0m
ECHO:
REM Open the build log
%OUTPUT_FILE%
PAUSE
EXIT
)

REM SUCCESS!
ECHO [42m Build SUCCEEDED[0m
ECHO:

ECHO [43m[30m Verify compiled version number before zipping[0m
ECHO [43m[30m Close any instances of ToDoList before zipping[0m
ECHO:
ECHO [106m[30m Allow caller to cancel building Zip[0m
PAUSE

CALL %REPO%\BuildReleaseZip.bat
CALL %REPO%\BuildCodeZip.bat

REM - Open Downloads folder
Explorer.exe "%REPO%\..\ToDoList_Downloads\Latest"

PAUSE
POPD

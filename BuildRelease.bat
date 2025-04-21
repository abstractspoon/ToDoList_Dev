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

REM - Build core app in VC6
CD %REPO%\Core

REM - Detours
REM "C:\Program Files (x86)\Microsoft Visual Studio\Common\MSDev98\Bin\msdev.exe" .\3rdParty\Detours\Detours.dsw /MAKE "ALL - Win32 Unicode Release"

REM - Core app
ECHO:
ECHO Building ToDoList Core 
ECHO ======================
ECHO:

MKDIR %REPO%\Core\ToDoList\Unicode_Release 2> NUL
SET OUTPUT_FILE=%REPO%\Core\ToDoList\Unicode_Release\Core_Build_Output.txt
DEL  %OUTPUT_FILE% > NUL

SET MSDEV="C:\Program Files (x86)\Microsoft Visual Studio\Common\MSDev98\Bin\msdev.exe"

IF NOT EXIST %MSDEV% (
ECHO Unable to locate MSDev.exe!!
PAUSE
EXIT
)

ECHO MSDEV = %MSDEV%

%MSDEV% .\ToDoList_Core.dsw /MAKE "ALL - Win32 Unicode Release" /OUT %OUTPUT_FILE% 
ECHO:

REM - Check for compile errors
FINDSTR /C:") : error" %OUTPUT_FILE%
IF %errorlevel%==1 (
REM - Check for link errors
FINDSTR /C:"Error executing link.exe" %OUTPUT_FILE%
)

IF %errorlevel%==0 (
ECHO [41m Build FAILED[0m
PAUSE
EXIT
)

REM SUCCESS!
ECHO [42m Build SUCCEEDED[0m

REM Run units tests
ECHO:
ECHO Running Unit Tests
ECHO ==================
ECHO:

CD TDLTest\Unicode_Release

SET OUTPUT_FILE=%REPO%\Core\TDLTest\Unicode_Release\Test_Output.txt
DEL  %OUTPUT_FILE% > NUL

TDLTest > %OUTPUT_FILE%

REM - Check for test errors
FINDSTR /C:"tests FAILED" %OUTPUT_FILE%

IF %errorlevel%==0 (
ECHO [41m Tests FAILED[0m
PAUSE
EXIT
)

REM - Check for test success
FINDSTR /C:"tests SUCCEEDED" %OUTPUT_FILE% > nul

IF %errorlevel%==1 (
ECHO [41m Test Results EMPTY[0m
PAUSE
EXIT
)

REM SUCCESS!
ECHO [42m Tests SUCCEEDED[0m

REM - Build plugins using MSBuild for reliability
ECHO:
ECHO Building ToDoList Plugins
ECHO =========================
ECHO:

SET MSBUILD="C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe"

IF NOT EXIST %MSBUILD% (
ECHO Unable to locate MSBuild.exe!!
PAUSE
EXIT
)

ECHO MSBUILD = %MSBUILD%
ECHO:

CD %REPO%\Plugins
MKDIR .\Release 2> NUL

SET OUTPUT_FILE=%REPO%\Plugins\Release\Build_Output.txt
DEL  %OUTPUT_FILE% > NUL

%MSBUILD% .\ToDoList_Plugins.sln /t:Build /p:Configuration=Release /m /v:normal > %OUTPUT_FILE%

REM - Check for build errors
FINDSTR /C:"Build FAILED." %OUTPUT_FILE%

IF %errorlevel%==0 (
ECHO [41m Build FAILED[0m
PAUSE
EXIT
)

REM SUCCESS!
ECHO [42m Build SUCCEEDED[0m

ECHO:
ECHO [106m[30m Allow caller to cancel building Zip[0m
ECHO:
PAUSE

CALL %REPO%\BuildReleaseZip.bat
CALL %REPO%\BuildCodeZip.bat

REM - Open Downloads folder
Explorer.exe "%REPO%\..\ToDoList_Downloads\Latest"

PAUSE
POPD

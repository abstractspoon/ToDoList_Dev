title ToDoList_Dev Debug Build

ECHO OFF
CLS

ECHO ToDoList_Dev Debug Build
ECHO ========================
ECHO:

PUSHD %~dp0
SET REPO=%CD%
ECHO REPO    = %REPO%

REM - WE USE FULL PATHS EVERYWHERE

IF NOT EXIST %REPO%\Core EXIT
IF NOT EXIST %REPO%\Plugins EXIT

REM - Use MSBuild for both core and plugins
SET MSBUILD="C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe"
ECHO MSBUILD = %MSBUILD%
ECHO:

IF NOT EXIST %MSBUILD% (
ECHO [41m Unable to locate MSBuild.exe!![0m
ECHO:
PAUSE
EXIT
)

REM - Core app
ECHO Building ToDoList Core 
ECHO ======================
ECHO:

SET SOLUTION=%REPO%\Core\ToDoList_Core.sln
SET OUTPUT_FILE=%REPO%\Core\ToDoList\Unicode_Debug\Core_Build_Output.txt

ECHO SOLUTION = %SOLUTION%

MKDIR %REPO%\Core\ToDoList\Unicode_Debug 2> NUL
DEL %OUTPUT_FILE% 2> NUL

%MSBUILD% %SOLUTION% /t:Build /p:Configuration="Unicode Debug" /m /v:normal > %OUTPUT_FILE%
ECHO:

REM - Check for compile errors
FINDSTR /C:"): error" %OUTPUT_FILE%
IF %errorlevel%==1 (

REM - Check for link errors
FINDSTR /C:": fatal error" %OUTPUT_FILE%
IF %errorlevel%==1 (

REM - Check for general errors
FINDSTR /C:"Build FAILED." %OUTPUT_FILE%
)
)

IF %errorlevel%==0 (
ECHO [41m Build FAILED[0m
ECHO:
REM Open the log
%OUTPUT_FILE%
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

SET TDLTEST=%REPO%\Core\TDLTest\Unicode_Debug\TDLTest.exe
SET OUTPUT_FILE=%REPO%\Core\TDLTest\Unicode_Debug\Test_Output.txt

DEL %OUTPUT_FILE% 2> NUL

%TDLTEST% > %OUTPUT_FILE%

REM - Check for test errors
FINDSTR /C:"tests FAILED" %OUTPUT_FILE%

IF %errorlevel%==0 (
ECHO [41m Tests FAILED[0m
ECHO:
REM Open the log
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

SET SOLUTION=%REPO%\Plugins\ToDoList_Plugins.sln
SET OUTPUT_FILE=%REPO%\Plugins\Debug\Build_Output.txt

ECHO SOLUTION = %SOLUTION%
ECHO:

MKDIR %REPO%\Plugins\Debug 2> NUL
DEL %OUTPUT_FILE% 2> NUL

%MSBUILD% %SOLUTION% /t:Build /p:Configuration=Debug /m /v:normal > %OUTPUT_FILE%

REM - Check for compile errors
FINDSTR /C:"): error" %OUTPUT_FILE%
IF %errorlevel%==1 (

REM - Check for link errors
FINDSTR /C:": fatal error" %OUTPUT_FILE%
IF %errorlevel%==1 (

REM - Check for general errors
FINDSTR /C:"Build FAILED." %OUTPUT_FILE%
)
)

IF %errorlevel%==0 (
ECHO [41m Build FAILED[0m
ECHO:
REM Open the log
%OUTPUT_FILE%
PAUSE
EXIT
)

REM SUCCESS!
ECHO [42m Build SUCCEEDED[0m
ECHO:

PAUSE
POPD

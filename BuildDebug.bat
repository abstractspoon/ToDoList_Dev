title ToDoList_Dev Debug Build

pushd %~dp0
set REPO=%CD%
ECHO REPO=%REPO%

if NOT EXIST %REPO%\Core exit
if NOT EXIST %REPO%\Plugins exit

SET MSBUILD="C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe"

REM - Build core app
cd %REPO%\Core

set OUTPUT_FILE=%REPO%\Core\ToDoList\Unicode_Debug\Build_Output.txt
del %OUTPUT_FILE%

%MSBUILD% .\ToDoList_Core.sln /t:Build /p:Configuration="Unicode Debug" /m /v:normal > %OUTPUT_FILE%

REM - Check for build errors
ECHO OFF
findstr /C:"Build FAILED." %OUTPUT_FILE%

if %errorlevel%==1 (
echo [92m Build SUCCEEDED[0m
)
if %errorlevel%==0 (
echo [91m Build FAILED[0m
pause
exit
)

REM Run units tests
ECHO ON

cd TDLTest\Unicode_Debug

set OUTPUT_FILE=Test_Output.txt
del %OUTPUT_FILE%

TDLTest > %OUTPUT_FILE%

REM - Check for test errors
ECHO OFF
findstr /C:"tests FAILED" Test_Output.txt

if %errorlevel%==1 (
echo [92m Tests SUCCEEDED[0m
)
if %errorlevel%==0 (
echo [91m Tests FAILED[0m
pause
exit
)

REM - Build plugins
ECHO ON

cd %REPO%\Plugins

set OUTPUT_FILE=%REPO%\Plugins\Debug\Build_Output.txt
del %OUTPUT_FILE%

%MSBUILD% .\ToDoList_Plugins.sln /t:Build /p:Configuration=Debug /m /v:normal > %OUTPUT_FILE%

REM - Check for build errors
ECHO OFF
findstr /C:"Build FAILED." %OUTPUT_FILE%

if %errorlevel%==1 (
echo [92m Build SUCCEEDED[0m
)
if %errorlevel%==0 (
echo [91m Build FAILED[0m
pause
exit
)

pause

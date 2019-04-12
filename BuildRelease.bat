ECHO OFF
CLS

set REPOROOT=C:\Users\Daniel.Godson\Documents\GitHub

if NOT EXIST %REPOROOT% set REPOROOT=D:\_code
if NOT EXIST %REPOROOT% exit

ECHO REPOROOT=%REPOROOT%

if NOT "%~1" == "" (
   if NOT "%~1" == "\.." (
      set LATESTREPO=%~1
   )
)

ECHO LATESTREPO=%LATESTREPO%

if "%LATESTREPO%" == "" exit

set RESREPO=%REPOROOT%\ToDoList_Resources
set PLUGINSREPO=%REPOROOT%\ToDoList_Plugins

ECHO RESREPO=%RESREPO%
ECHO PLUGINSREPO=%PLUGINSREPO%

ECHO ON

REM - Build Core App
cd %LATESTREPO%
"C:\Program Files (x86)\Microsoft Visual Studio\Common\MSDev98\Bin\msdev.exe" .\Core\ToDoList_All.dsw /MAKE "ALL - Win32 Unicode Release" 

REM - Build Plugins
cd %PLUGINSREPO%
"C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv.com" .\Plugins\ToDoList_Plugins.sln /Build "Release"

REM Allow caller to cancel
pause

CALL %LATESTREPO%\BuildReleaseZip.bat %LATESTREPO%
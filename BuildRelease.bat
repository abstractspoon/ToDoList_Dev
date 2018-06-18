CLS

set REPOROOT=C:\Users\Daniel.Godson\Documents\GitHub

if NOT EXIST %REPOROOT% set CODE_DIR=D:\_code
if NOT EXIST %REPOROOT% exit

set DEVREPO=%REPOROOT%\ToDoList_Dev
set RESREPO=%REPOROOT%\ToDoList_Resources
set LATESTREPO=%REPOROOT%\ToDoList_7.2
set PLUGINSREPO=%REPOROOT%\ToDoList_Plugins

REM - Build Core App
cd "C:\Program Files (x86)\Microsoft Visual Studio\Common\MSDev98\Bin"

msdev %LATESTREPO%\ToDoList\ToDoList_All.dsw /MAKE "ALL - Win32 Unicode Release" 

REM - Build Plugins
cd "C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE"

devenv %PLUGINSREPO%\ToDoList_Plugins.sln /build "Release"

cd %DEVREPO%
CALL BuildReleaseZip.bat %LATESTREPO%
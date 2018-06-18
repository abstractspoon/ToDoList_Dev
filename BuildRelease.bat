CLS

set CODE_DIR=C:\Users\Daniel.Godson\Documents\GitHub
REM set CODE_DIR=D:\_code

set DEV_DIR=%CODE_DIR%\ToDoList_Dev
set LATEST_DIR=%CODE_DIR%\ToDoList_7.2
set RES_DIR=%CODE_DIR%\ToDoList_Resources
set PLUGINS_DIR=%CODE_DIR%\ToDoList_Plugins

REM - Build Core App
cd "C:\Program Files (x86)\Microsoft Visual Studio\Common\MSDev98\Bin"

msdev %LATEST_DIR%\ToDoList\ToDoList_All.dsw /MAKE "ALL - Win32 Unicode Release" /REBUILD

REM - Build Plugins
cd "C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE"

devenv %PLUGINS_DIR%\ToDoList_Plugins.sln /Rebuild "Release"

cd %DEV_DIR%
CALL BuildReleaseZip.bat %LATEST_DIR%\ToDoList
ECHO OFF

pushd %~dp0
set REPO=%CD%
ECHO REPO=%REPO%

REM ProjectZip Location
set PATHPZIP="D:\Tools\ProjectZip.exe"

if NOT EXIST %PATHPZIP% exit

ECHO PATH7ZIP=%PATHPZIP%

REM Options
REM ZP - Zip Path
REM ML - Make Lowercase
REM NM - No Message

ECHO Zipping %REPO%\Core\ToDoList_Core.dsw
%PATHPZIP% %REPO%\Core\ToDoList_Core.dsw /ZP %REPO%\..\ToDoList_Prev\8.2\ToDoList_src_Core.8.2._.zip /ML /NM

ECHO Zipping %REPO%\Plugins\ToDoList_Plugins.sln
%PATHPZIP% %REPO%\Plugins\ToDoList_Plugins.sln /ZP %REPO%\..\ToDoList_Prev\8.2\ToDoList_src_Plugins.8.2._.zip /ML /NM

popd

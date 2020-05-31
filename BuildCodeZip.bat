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
REM OZ - Open Zip
REM NM - No Message

ECHO Zipping %REPO%\Core\ToDoList_Core.dsw
%PATHPZIP% %REPO%\Core\ToDoList_Core.dsw /ZP %REPO%\ToDoList_Core_src_.zip /ML /OZ /NM

ECHO Zipping %REPO%\Plugins\ToDoList_Plugins.sln
%PATHPZIP% %REPO%\Plugins\ToDoList_Plugins.sln /ZP %REPO%\ToDoList_Plugins_src_.zip /ML /OZ /NM

popd
pause

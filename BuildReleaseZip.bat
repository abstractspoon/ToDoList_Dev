if "%~1" == "" exit

set OUTZIP=%1\Unicode_Release\todolist_exe_.zip
set PATH7ZIP="C:\Program Files (x86)\7-Zip\7z.exe"
set RESPATH="D:\_code\ToDoList_Resources"

del %OUTZIP%

REM Zip C++ Binaries
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\ToDoList.exe
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\TDLUpdate.exe
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\TDLUninstall.exe
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\TDLTransEdit.exe

REM Manifest for XP only (Updater will delete for other OSes)
%PATH7ZIP% a %OUTZIP% %1\res\ToDoList.exe.xp.manifest

REM Handle dlls explicitly to maintain control over .NET plugins
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\PlainTextImport.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\Rtf2HtmlBridge.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\RTFContentCtrl.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\BurndownExt.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\TransText.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\CalendarExt.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\EncryptDecrypt.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\FMindImportExport.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\FtpStorage.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\GanttChartExt.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\GPExport.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\iCalImportExport.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\Itenso.*.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\KanbanBoard.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\MLOImport.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\MySpellCheck.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\OdbcStorage.dll

REM .Net Plugins
REM echo.%1 | findstr /C:"_Dev" 1>nul

REM IF ERRORLEVEL 1 goto CONTINUE
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\PluginHelpers.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\DayViewUIExtensionCore.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\DayViewUIExtensionBridge.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\MindMapUIExtensionCore.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\MindMapUIExtensionBridge.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\Calendar.DayView.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\WordCloudUIExtensionCore.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\WordCloudUIExtensionBridge.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\TagCloud.dll
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\Gma.CodeCloud.Controls.dll
REM :CONTINUE
 
REM Copy latest Resources
del %1\Unicode_Release\Resources\ /Q /S
del %1\Unicode_Release\Resources\Translations\backup\ /Q
xcopy %RESPATH%\*.* %1\Unicode_Release\Resources\ /Y /D /E /EXCLUDE:D:\_code\ToDoList_Dev\BuildReleaseZip_Exclude.txt

REM Zip Resources
%PATH7ZIP% a %OUTZIP% %1\Unicode_Release\Resources\ -x!.git*

%OUTZIP%

pause

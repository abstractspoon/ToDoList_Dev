ECHO OFF

pushd %~dp0
SET REPO=%CD%
ECHO REPO=%REPO%

SET RESREPO=%REPO%\..\ToDoList_Resources
ECHO RESREPO=%RESREPO%

REM 7-Zip Location
SET PATH7ZIP="C:\Program Files (x86)\7-Zip\7z.exe"

IF NOT EXIST %PATH7ZIP% SET PATH7ZIP="C:\Program Files\7-Zip\7z.exe"
ECHO PATH7ZIP=%PATH7ZIP%

IF NOT EXIST %PATH7ZIP% exit

SET OUTDIR=%REPO%\Core\ToDoList\Unicode_Release

REM - Extract version from core app
%REPO%\Core\ToDoList\Unicode_Release\ToDoList.exe -ver
SET /P TDLVER=< .\ver.txt
ECHO TDLVER=%TDLVER%
DEL .\ver.txt

REM - Update all TDL components with version from core app
REM - Excludes all dlls we DON'T own the copyright to
SET PATHVEREDIT="D:\tools\rcedit-x64.exe"
ECHO PATHVEREDIT=%PATHVEREDIT%

REM - ToDoList.exe is added so we can also use this list for zipping
SET TDLFILELIST=ToDoList.exe
SET TDLFILELIST=%TDLFILELIST%;TDLUpdate.exe
SET TDLFILELIST=%TDLFILELIST%;TDLUninstall.exe
SET TDLFILELIST=%TDLFILELIST%;TDLTransEdit.exe
SET TDLFILELIST=%TDLFILELIST%;ConvertRTFToHTML.exe

SET TDLFILELIST=%TDLFILELIST%;BurndownExt.dll
SET TDLFILELIST=%TDLFILELIST%;CalendarExt.dll
SET TDLFILELIST=%TDLFILELIST%;EncryptDecrypt.dll
SET TDLFILELIST=%TDLFILELIST%;FMindImportExport.dll
SET TDLFILELIST=%TDLFILELIST%;FtpStorage.dll
SET TDLFILELIST=%TDLFILELIST%;GPExport.dll
SET TDLFILELIST=%TDLFILELIST%;GanttChartExt.dll
SET TDLFILELIST=%TDLFILELIST%;iCalImportExport.dll
SET TDLFILELIST=%TDLFILELIST%;KanbanBoard.dll
SET TDLFILELIST=%TDLFILELIST%;MLOImport.dll
SET TDLFILELIST=%TDLFILELIST%;MySpellCheck.dll
SET TDLFILELIST=%TDLFILELIST%;PlainTextImport.dll
SET TDLFILELIST=%TDLFILELIST%;RTFContentCtrl.dll
SET TDLFILELIST=%TDLFILELIST%;TransText.dll
SET TDLFILELIST=%TDLFILELIST%;WorkloadExt.dll

SET TDLFILELIST=%TDLFILELIST%;CommandHandling.dll
SET TDLFILELIST=%TDLFILELIST%;RichEditExtensions.dll
SET TDLFILELIST=%TDLFILELIST%;WebBrowserEx.dll

SET TDLFILELIST=%TDLFILELIST%;DayViewUIExtensionBridge.dll
SET TDLFILELIST=%TDLFILELIST%;DayViewUIExtensionCore.dll
SET TDLFILELIST=%TDLFILELIST%;HTMLContentControlBridge.dll
SET TDLFILELIST=%TDLFILELIST%;HTMLContentControlCore.dll
SET TDLFILELIST=%TDLFILELIST%;HTMLReportExporterBridge.dll
SET TDLFILELIST=%TDLFILELIST%;HTMLReportExporterCore.dll
SET TDLFILELIST=%TDLFILELIST%;MDContentControlBridge.dll
SET TDLFILELIST=%TDLFILELIST%;MDContentControlCore.dll
SET TDLFILELIST=%TDLFILELIST%;MindMapUIExtensionBridge.dll
SET TDLFILELIST=%TDLFILELIST%;MindMapUIExtensionCore.dll
SET TDLFILELIST=%TDLFILELIST%;PDFExporterBridge.dll
SET TDLFILELIST=%TDLFILELIST%;PDFExporterCore.dll
SET TDLFILELIST=%TDLFILELIST%;PluginHelpers.dll
SET TDLFILELIST=%TDLFILELIST%;SpreadsheetContentControlBridge.dll
SET TDLFILELIST=%TDLFILELIST%;SpreadsheetContentControlCore.dll
SET TDLFILELIST=%TDLFILELIST%;WordCloudUIExtensionBridge.dll
SET TDLFILELIST=%TDLFILELIST%;WordCloudUIExtensionCore.dll

FOR %%f IN (%TDLFILELIST%) DO ( 
   ECHO Updating version in %OUTDIR%\%%f
   %PATHVEREDIT% %OUTDIR%\%%f --set-file-version "%TDLVER%"
)

REM Save symbols to their own zip file
MKDIR %REPO%\..\ToDoList_Symbols
SET OUTZIP=%REPO%\..\ToDoList_Symbols\%TDLVER%.zip

%PATH7ZIP% a %OUTZIP% %OUTDIR%\*.pdb

REM - Zip up app and resources
SET OUTZIP=%OUTDIR%\todolist_exe_.zip

ECHO OUTDIR=%OUTDIR%
ECHO OUTZIP=%OUTZIP%

DEL %OUTZIP%

REM - Core app and dlls we own the copyright to
FOR %%f IN (%TDLFILELIST%) DO ( 
%PATH7ZIP% a %OUTZIP% %OUTDIR%\%%f
)

REM - All other components which we don't own
%PATH7ZIP% a %OUTZIP% %OUTDIR%\Calendar.DayView.dll
%PATH7ZIP% a %OUTZIP% %OUTDIR%\CustomComboBox.dll
%PATH7ZIP% a %OUTZIP% %OUTDIR%\Gma.CodeCloud.Controls.dll
%PATH7ZIP% a %OUTZIP% %OUTDIR%\HtmlAgilityPack.dll
%PATH7ZIP% a %OUTZIP% %OUTDIR%\Itenso.*.dll
%PATH7ZIP% a %OUTZIP% %OUTDIR%\iTextSharp.dll
%PATH7ZIP% a %OUTZIP% %OUTDIR%\Markdig.dll
%PATH7ZIP% a %OUTZIP% %OUTDIR%\Microsoft.VisualStudio.OLE.Interop.dll
%PATH7ZIP% a %OUTZIP% %OUTDIR%\MSDN.HtmlEditorControl.dll
%PATH7ZIP% a %OUTZIP% %OUTDIR%\Rtf2HtmlBridge.dll
%PATH7ZIP% a %OUTZIP% %OUTDIR%\ToolStripToolTip.dll
%PATH7ZIP% a %OUTZIP% %OUTDIR%\UIComponents.dll
%PATH7ZIP% a %OUTZIP% %OUTDIR%\unvell.ReoGrid.dll
%PATH7ZIP% a %OUTZIP% %OUTDIR%\unvell.ReoGridEditorControl.dll
%PATH7ZIP% a %OUTZIP% %OUTDIR%\LinkLabelEx.dll

REM - Manifest for XP only (Updater will delete for other OSes)
%PATH7ZIP% a %OUTZIP% %REPO%\Core\ToDoList\res\ToDoList.exe.XP.manifest

REM - Copy latest Resources
del %OUTDIR%\Resources\ /Q /S
del %OUTDIR%\Resources\Translations\backup\ /Q
xcopy %RESREPO%\*.* %OUTDIR%\Resources\ /Y /D /E /EXCLUDE:%REPO%\BuildReleaseZip_Exclude.txt

REM - Zip install instructions to root
%PATH7ZIP% a %OUTZIP% %OUTDIR%\Resources\Install.Windows.txt
%PATH7ZIP% a %OUTZIP% %OUTDIR%\Resources\Install.Linux.txt

REM - And remove from resources to avoid duplication
del %OUTDIR%\Resources\Install.Windows.txt
del %OUTDIR%\Resources\Install.Linux.txt

REM - Zip Resources
%PATH7ZIP% a %OUTZIP% %OUTDIR%\Resources\ -x!.git*

REM - Copy the zip file to the download folder
copy %OUTZIP% %REPO%\..\ToDoList_Downloads\Latest\

REM - And then move it to ToDoList_Prev\8.2
move %OUTZIP% %REPO%\..\ToDoList_Prev\8.2\ToDoList_exe.8.2._.zip

popd

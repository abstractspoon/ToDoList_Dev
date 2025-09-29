ECHO OFF
ECHO:

REM - WE USE FULL PATHS THROUGHOUT - NO 'CD' WITH RELATIVE PATHS

PUSHD %~dp0

ECHO:
ECHO Versioning Binaries
ECHO ===================
ECHO:

SET REPO=%CD%
SET RESREPO=%REPO%\..\ToDoList_Resources
SET OUTDIR=%REPO%\Core\ToDoList\Unicode_Release
SET PATHVEREDIT=D:\tools\rcedit-x64.exe
SET VERFILE=%OUTDIR%\ver.txt

REM - Extract version from core app
DEL %VERFILE% 2> NUL
%OUTDIR%\ToDoList.exe -ver %VERFILE%

IF NOT EXIST %VERFILE% (
ECHO [41m Unable to locate ver.txt!![0m
ECHO:
PAUSE
EXIT
)

SET /P TDLVER=< %VERFILE%

ECHO REPO        = %REPO%
ECHO OUTDIR      = %OUTDIR%
ECHO RESREPO     = %RESREPO%
ECHO PATHVEREDIT = %PATHVEREDIT%
ECHO TDLVER      = %TDLVER%
ECHO:

IF NOT EXIST %PATHVEREDIT% (
ECHO [41m Unable to locate rcedit-x64.exe!!
ECHO:
PAUSE
EXIT
)

DEL %OUTDIR%\ver.txt 2> NUL

REM - Update all TDL components with version from core app
REM - Excludes all dlls we DON'T own the copyright to
REM - ToDoList.exe is added so we can also use this list for zipping
SET FILELIST=ToDoList.exe
SET FILELIST=%FILELIST%;TDLUpdate.exe
SET FILELIST=%FILELIST%;TDLUninstall.exe
SET FILELIST=%FILELIST%;TDLTransEdit.exe
SET FILELIST=%FILELIST%;ConvertRTFToHTML.exe

SET FILELIST=%FILELIST%;BurndownExt.dll
SET FILELIST=%FILELIST%;CalendarExt.dll
SET FILELIST=%FILELIST%;EncryptDecrypt.dll
SET FILELIST=%FILELIST%;FMindImportExport.dll
SET FILELIST=%FILELIST%;FtpStorage.dll
SET FILELIST=%FILELIST%;GPExport.dll
SET FILELIST=%FILELIST%;GanttChartExt.dll
SET FILELIST=%FILELIST%;iCalImportExport.dll
SET FILELIST=%FILELIST%;KanbanBoard.dll
SET FILELIST=%FILELIST%;MLOImport.dll
SET FILELIST=%FILELIST%;MySpellCheck.dll
SET FILELIST=%FILELIST%;PlainTextImport.dll
SET FILELIST=%FILELIST%;RTFContentCtrl.dll
SET FILELIST=%FILELIST%;TransText.dll
SET FILELIST=%FILELIST%;WorkloadExt.dll

SET FILELIST=%FILELIST%;CommandHandling.dll
SET FILELIST=%FILELIST%;RichEditExtensions.dll
SET FILELIST=%FILELIST%;ImageHelper.dll
SET FILELIST=%FILELIST%;ScrollHelper.dll
SET FILELIST=%FILELIST%;TreeViewHelper.dll
SET FILELIST=%FILELIST%;WebBrowserEx.dll

SET FILELIST=%FILELIST%;DayViewUIExtensionBridge.dll
SET FILELIST=%FILELIST%;DayViewUIExtensionCore.dll
SET FILELIST=%FILELIST%;EvidenceBoardUIExtensionBridge.dll
SET FILELIST=%FILELIST%;EvidenceBoardUIExtensionCore.dll
SET FILELIST=%FILELIST%;HTMLContentControlBridge.dll
SET FILELIST=%FILELIST%;HTMLContentControlCore.dll
SET FILELIST=%FILELIST%;HTMLReportExporterBridge.dll
SET FILELIST=%FILELIST%;HTMLReportExporterCore.dll
SET FILELIST=%FILELIST%;JSONExporterBridge.dll
SET FILELIST=%FILELIST%;JSONExporterCore.dll
SET FILELIST=%FILELIST%;LoggedTimeUIExtensionBridge.dll
SET FILELIST=%FILELIST%;LoggedTimeUIExtensionCore.dll
SET FILELIST=%FILELIST%;MDContentControlBridge.dll
SET FILELIST=%FILELIST%;MDContentControlCore.dll
SET FILELIST=%FILELIST%;MindMapUIExtensionBridge.dll
SET FILELIST=%FILELIST%;MindMapUIExtensionCore.dll
SET FILELIST=%FILELIST%;MySQLStorageBridge.dll
SET FILELIST=%FILELIST%;MySQLStorageCore.dll
SET FILELIST=%FILELIST%;PDFExporterBridge.dll
SET FILELIST=%FILELIST%;PDFExporterCore.dll
SET FILELIST=%FILELIST%;PluginHelpers.dll
SET FILELIST=%FILELIST%;SpreadsheetContentControlBridge.dll
SET FILELIST=%FILELIST%;SpreadsheetContentControlCore.dll
SET FILELIST=%FILELIST%;WordCloudUIExtensionBridge.dll
SET FILELIST=%FILELIST%;WordCloudUIExtensionCore.dll

SET OUTPUT_FILE=%OUTDIR%\Versioning_Output.txt
DEL %OUTPUT_FILE% 2> NUL

FOR %%f IN (%FILELIST%) DO ( 
%PATHVEREDIT% %OUTDIR%\%%f --set-file-version "%TDLVER%" 2>> %OUTPUT_FILE%
)

REM Check for errors
FINDSTR /C:"Unable to load file:" %OUTPUT_FILE%
   
IF %errorlevel%==1 (
FINDSTR /C:"Fatal error:" %OUTPUT_FILE%
)

IF %errorlevel%==0 (
ECHO [41m Versioning FAILED[0m
ECHO:
REM Open the log
%OUTPUT_FILE%
PAUSE
EXIT
)
 
REM SUCCESS! 
ECHO [42m Versioning SUCCEEDED[0m

REM Save symbols to their own zip file
ECHO:
ECHO Zipping symbols
ECHO ===============
ECHO:

SET PATH7ZIP="C:\Program Files (x86)\7-Zip\7z.exe"
IF NOT EXIST %PATH7ZIP% SET PATH7ZIP="C:\Program Files\7-Zip\7z.exe"

ECHO PATH7ZIP = %PATH7ZIP%
ECHO:

IF NOT EXIST %PATH7ZIP% (
ECHO [41m Unable to locate 7z.exe!![0m
ECHO:
PAUSE
EXIT
)

SET OUTPUT_FILE=%OUTDIR%\Symbols_Output.txt

MKDIR %REPO%\..\ToDoList_Symbols 2> NUL
SET OUTZIP=%REPO%\..\ToDoList_Symbols\%TDLVER%.zip

%PATH7ZIP% a %OUTZIP% %OUTDIR%\*.pdb > %OUTPUT_FILE%

REM Check for errors
FINDSTR /C:"The system cannot find the file specified." %OUTPUT_FILE%
   
IF %errorlevel%==0 (
ECHO [41m Symbols FAILED[0m
ECHO:
REM Open the log
%OUTPUT_FILE%
PAUSE
EXIT
)
 
REM SUCCESS! 
ECHO [42m Symbols SUCCEEDED[0m

REM - Zip up app and resources
ECHO:
ECHO Zipping Binaries
ECHO ================
ECHO:

SET OUTZIP=%OUTDIR%\todolist_exe_.zip
SET OUTPUT_FILE=%OUTDIR%\Zip_Output.txt

ECHO OUTZIP = %OUTZIP%
ECHO OUTDIR = %OUTDIR%
ECHO:

DEL %OUTPUT_FILE% 2> NUL
DEL %OUTZIP% 2> NUL

REM - Add all other components which we don't own
SET FILELIST=%FILELIST%;BouncyCastle.Crypto.dll
SET FILELIST=%FILELIST%;Calendar.DayView.dll
SET FILELIST=%FILELIST%;CustomComboBox.dll
SET FILELIST=%FILELIST%;Gma.CodeCloud.Controls.dll
SET FILELIST=%FILELIST%;Google.Protobuf.dll
SET FILELIST=%FILELIST%;HtmlAgilityPack.dll
SET FILELIST=%FILELIST%;Itenso.Rtf.Converter.Html.dll
SET FILELIST=%FILELIST%;Itenso.Rtf.Interpreter.dll
SET FILELIST=%FILELIST%;Itenso.Rtf.Parser.dll
SET FILELIST=%FILELIST%;Itenso.Solutions.Community.Rtf2Html.dll
SET FILELIST=%FILELIST%;Itenso.Sys.dll
SET FILELIST=%FILELIST%;iTextSharp.dll
SET FILELIST=%FILELIST%;LinkLabelEx.dll
SET FILELIST=%FILELIST%;K4os.Compression.LZ4.dll
SET FILELIST=%FILELIST%;K4os.Compression.LZ4.Streams.dll
SET FILELIST=%FILELIST%;K4os.Hash.xxHash.dll
SET FILELIST=%FILELIST%;Markdig.dll
SET FILELIST=%FILELIST%;Microsoft.VisualStudio.OLE.Interop.dll
SET FILELIST=%FILELIST%;MSDN.HtmlEditorControl.dll
SET FILELIST=%FILELIST%;MySql.Data.dll
SET FILELIST=%FILELIST%;Newtonsoft.Json.dll
SET FILELIST=%FILELIST%;RadialTree.dll
SET FILELIST=%FILELIST%;ReverseMarkdown.dll
SET FILELIST=%FILELIST%;Rtf2HtmlBridge.dll
SET FILELIST=%FILELIST%;ToolStripToolTip.dll
SET FILELIST=%FILELIST%;UIComponents.dll
SET FILELIST=%FILELIST%;Ubiety.Dns.Core.dll
SET FILELIST=%FILELIST%;unvell.ReoGrid.dll
SET FILELIST=%FILELIST%;unvell.ReoGridEditorControl.dll
SET FILELIST=%FILELIST%;XmlDiffPatch.dll
SET FILELIST=%FILELIST%;XmlDiffPatch.View.dll
SET FILELIST=%FILELIST%;XmlDiffView.exe
SET FILELIST=%FILELIST%;ZstdNet.dll

FOR %%f IN (%FILELIST%) DO ( 
ECHO Adding '%%f' to zip >> %OUTPUT_FILE%
%PATH7ZIP% a %OUTZIP% %OUTDIR%\%%f >> %OUTPUT_FILE%

REM - Check for locking errors
IF %errorlevel% NEQ 0 (
ECHO [41m Zipping Binaries FAILED[0m
ECHO:
REM Open the log
%OUTPUT_FILE%
PAUSE
EXIT
)
)

REM REquired MS components for C# plugins
%PATH7ZIP% a %OUTZIP% C:\Windows\SysWOW64\MFC140U.dll >> %OUTPUT_FILE%
%PATH7ZIP% a %OUTZIP% C:\Windows\SysWOW64\VCRUNTIME140.dll >> %OUTPUT_FILE%

REM - Manifest for XP only (Updater will delete for other OSes)
%PATH7ZIP% a %OUTZIP% %REPO%\Core\ToDoList\res\ToDoList.exe.XP.manifest >> %OUTPUT_FILE%

REM - Delete and recreate Resources folder
RMDIR %OUTDIR%\Resources /Q /S > NUL
MKDIR %OUTDIR%\Resources 2> NUL

REM - Copy latest Resources
XCOPY %RESREPO%\*.* %OUTDIR%\Resources\ /Y /D /E /EXCLUDE:%REPO%\BuildReleaseZip_Exclude.txt > NUL

REM - Zip install instructions to root
%PATH7ZIP% a %OUTZIP% %OUTDIR%\Resources\Install.Windows.txt >> %OUTPUT_FILE%
%PATH7ZIP% a %OUTZIP% %OUTDIR%\Resources\Install.Linux.txt >> %OUTPUT_FILE%

REM - And remove from resources to avoid duplication
DEL %OUTDIR%\Resources\Install.Windows.txt > NUL
DEL %OUTDIR%\Resources\Install.Linux.txt > NUL

REM - Zip Resources
%PATH7ZIP% a %OUTZIP% %OUTDIR%\Resources\ -x!.git* >> %OUTPUT_FILE%

REM Check for errors
FINDSTR /C:"The system cannot find the file specified." %OUTPUT_FILE% > NUL
   
IF %errorlevel%==0 (
ECHO [41m Zipping Binaries FAILED[0m
ECHO:
REM Open the log
%OUTPUT_FILE%
PAUSE
EXIT
)
 
REM SUCCESS! 
ECHO [42m Zipping Binaries SUCCEEDED[0m
ECHO:

REM - Copy the zip file to the download folder
ECHO Copy todolist_exe_.zip to %REPO%\..\ToDoList_Downloads\Latest\
COPY %OUTZIP% %REPO%\..\ToDoList_Downloads\Latest\
ECHO:
   
REM - And then move it to ToDoList_Prev\9.2
MKDIR %REPO%\..\ToDoList_Prev\9.2 2> NUL
ECHO Rename todolist_exe_.zip to %REPO%\..\ToDoList_Prev\9.2\ToDoList_exe.9.2._.zip
MOVE %OUTZIP% %REPO%\..\ToDoList_Prev\9.2\ToDoList_exe.9.2._.zip
ECHO:
   
POPD

	TITLE	D:\_CODE\Shared\FILEMISC.CPP
	.386P
include listing.inc
if @Version gt 510
.model FLAT
else
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT DWORD USE32 PUBLIC 'BSS'
_BSS	ENDS
$$SYMBOLS	SEGMENT BYTE USE32 'DEBSYM'
$$SYMBOLS	ENDS
$$TYPES	SEGMENT BYTE USE32 'DEBTYP'
$$TYPES	ENDS
_TLS	SEGMENT DWORD USE32 PUBLIC 'TLS'
_TLS	ENDS
;	COMDAT ??_C@_1CC@LAMD@?$AAM?$AAa?$AAg?$AAe?$AAl?$AAl?$AAa?$AAn?$AA?5?$AAM?$AAS?$AAW?$AAH?$AAE?$AAE?$AAL?$AA?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_1O@JDNM@?$AAM?$AAo?$AAu?$AAs?$AAe?$AAZ?$AA?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_1CA@JHCA@?$AAM?$AAS?$AAW?$AAH?$AAE?$AAE?$AAL?$AA_?$AAR?$AAO?$AAL?$AAL?$AAM?$AAS?$AAG?$AA?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_1CK@MMPP@?$AAM?$AAS?$AAH?$AA_?$AAW?$AAH?$AAE?$AAE?$AAL?$AAS?$AAU?$AAP?$AAP?$AAO?$AAR?$AAT?$AA_?$AAM?$AAS?$AAG@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_1CK@GMOB@?$AAM?$AAS?$AAH?$AA_?$AAS?$AAC?$AAR?$AAO?$AAL?$AAL?$AA_?$AAL?$AAI?$AAN?$AAE?$AAS?$AA_?$AAM?$AAS?$AAG@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0EK@DBHF@C?3?2Program?5Files?5?$CIx86?$CJ?2Microsoft@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0EK@MDCF@C?3?2Program?5Files?5?$CIx86?$CJ?2Microsoft@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
CRT$XCA	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCA	ENDS
CRT$XCU	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCU	ENDS
CRT$XCL	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCL	ENDS
CRT$XCC	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCC	ENDS
CRT$XCZ	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCZ	ENDS
;	COMDAT ?HasFlag@Misc@@YAHKK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E268
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E269
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E270
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E271
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E273
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E274
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E275
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E276
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CTempFileBackup@@QAE@ABVCString@@0K0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CTempFileBackup@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CFileBackup@@QAE@ABVCString@@0K0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CFileBackup@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MakeBackup@CFileBackup@@QAEHABVCString@@0K0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RestoreBackup@CFileBackup@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?BuildBackupPath@CFileBackup@@SA?AVCString@@ABV2@0K0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?TerminatePath@FileMisc@@YAAAVCString@@AAV2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?TerminatePath@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?UnterminatePath@FileMisc@@YAAAVCString@@AAV2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?UnterminatePath@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsPathTerminated@FileMisc@@YAHABVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReplaceExtension@FileMisc@@YAAAVCString@@AAV2@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveExtension@FileMisc@@YAAAVCString@@AAV2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ValidateFilepath@FileMisc@@YAAAVCString@@AAV2@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ValidateFilename@FileMisc@@YAAAVCString@@AAV2@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileNameFromPath@FileMisc@@YA?AVCString@@PBGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileLastModified@FileMisc@@YAJPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileLastModified@FileMisc@@YAHPBGAAU_FILETIME@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetFileLastModified@FileMisc@@YAHPBGABU_FILETIME@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileLastModified@FileMisc@@YAHPBGAAU_SYSTEMTIME@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?TouchFile@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FindFiles@FileMisc@@YAHABVCString@@AAVCStringArray@@HPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetDropFilePaths@FileMisc@@YAHPAUHDROP__@@AAVCStringArray@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DeleteFolderContents@FileMisc@@YAHPBGK0PAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DeleteFolder@FileMisc@@YAHPBGKPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFolderSize@FileMisc@@YANPBGH0PAXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsPath@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PathExists@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FolderExists@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FileExists@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FolderFromFilePathExists@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetCwd@FileMisc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetCwd@FileMisc@@YAXABVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFolderFromFilePath@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateFolderFromFilePath@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PathHasWildcard@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsFolderWritable@FileMisc@@YAHPBGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateFolder@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FolderContainsFiles@FileMisc@@YAHPBGH0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MoveFolder@FileMisc@@YAHPBG00KPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CopyFolder@FileMisc@@YAHPBG00KPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MoveFolder@FileMisc@@YAHPBG0KPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CopyFolder@FileMisc@@YAHPBG0KPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DeleteFileW@FileMisc@@YAHPBGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CopyFileW@FileMisc@@YAHPBG0HHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MoveFileW@FileMisc@@YAHPBG0HH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ModifyAttributes@FileMisc@@YAHPBGKKK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ModifyFolderAndContentAttributes@FileMisc@@YAHPBGKKH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileSize@FileMisc@@YANPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsLoggingEnabledW@FileMisc@@YAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetLogFileName@FileMisc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EnableLogging@FileMisc@@YAHHPBGK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LogTimeElapsed@FileMisc@@YAXAAKPBGZZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LogTextRaw@FileMisc@@YAXPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LogText@FileMisc@@YAXPBGZZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AppendLineToFile@FileMisc@@YAHPBGABVCString@@W4SFE_SAVEAS@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CStdioFileEx@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadFileLines@FileMisc@@YAHPBGAAVCStringArray@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CanReadFile@FileMisc@@YAHPBGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SaveFile@FileMisc@@YAHPBGABVCString@@W4SFE_SAVEAS@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SaveFile@FileMisc@@YAHPBGPBD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadFile@FileMisc@@YAHPBGAAVCString@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsTempFile@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTempFolder@FileMisc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTempFileNameW@FileMisc@@YA?AVCString@@PBGI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTempFileNameW@FileMisc@@YA?AVCString@@PBG0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SelectFileInExplorer@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Run@FileMisc@@YAKPAUHWND__@@PBG1H11@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ExtractResource@FileMisc@@YAHIPBGABVCString@@PAUHINSTANCE__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CFileStatus@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetModuleFileNameW@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetModuleFolder@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetModuleDriveType@FileMisc@@YAHPAUHINSTANCE__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetModuleVersion@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetModuleVersion@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetModuleVersion@FileMisc@@YAHPAUHINSTANCE__@@AAVCUIntArray@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAppFileName@FileMisc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAppIniFileName@FileMisc@@YA?AVCString@@ABV2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAppFolder@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAppResourceFolder@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAppModuleFileNames@FileMisc@@YAHAAVCStringArray@@W4FB_MODULE_SORT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAppModuleHandles@FileMisc@@YAHAAVCDWordArray@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LogAppModuleState@FileMisc@@YAXW4FB_MODULE_SORT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAppVersion@FileMisc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAppVersion@FileMisc@@YAHAAVCUIntArray@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SplitVersionNumber@FileMisc@@YAHPBGAAVCUIntArray@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CompareVersions@FileMisc@@YAHPBG0H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CompareVersions@FileMisc@@YAHAAVCUIntArray@@0H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetWindowsFolder@FileMisc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetWindowsSystemFolder@FileMisc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ExtractResource@FileMisc@@YAHPBGI0ABVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HasExtension@FileMisc@@YAHPBG0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetExtension@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SplitPath@FileMisc@@YAXPBGPAVCString@@111@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFullPath@FileMisc@@YA?AVCString@@ABV2@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MakeFullPath@FileMisc@@YAAAVCString@@AAV2@ABV2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetRelativePath@FileMisc@@YA?AVCString@@ABV2@0H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MakeRelativePath@FileMisc@@YAAAVCString@@AAV2@ABV2@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsSamePath@FileMisc@@YAHABVCString@@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSpecialFolder@FileMisc@@YAHHAAVCString@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSpecialFilePath@FileMisc@@YAHHABVCString@@AAV2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsPathInSpecialFolder@FileMisc@@YAHABVCString@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsAppInProgramFiles@FileMisc@@YAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ResolveShortcut@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetLongPathNameW@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetDropFilePaths@FileMisc@@YAHPAVCOleDataObject@@AAVCStringArray@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@QAE@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@QAE@PAUIUnknown@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@QAE@PAUIUnknown@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@QAE@ABU_GUID@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@QAE@PAGK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@QAEAAV0@PAUIUnknown@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@QAEAAV0@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@QAEAAV0@ABU_GUID@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@QAEAAV0@PAG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetInterfacePtr@?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@QBEPAUIUnknown@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateObject@?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@QAEJABU_GUID@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateObject@?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@QAEJPAGK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_AddRef@?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@AAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetIID@?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@SAABU_GUID@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Release@?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@AAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$_CIP@UIUnknown@@$1?IID_IUnknown@@3U_GUID@@B@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCNoTrackObject@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CNoTrackObject@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7CNoTrackObject@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT __imp__FreeResource@4
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
FLAT	GROUP _DATA, CONST, _BSS, CRT$XCA, CRT$XCU, CRT$XCL, CRT$XCC, CRT$XCZ
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
_BSS	SEGMENT
_s_bLogging DD	01H DUP (?)
_s_dwLogFlags DD 01H DUP (?)
_BSS	ENDS
CRT$XCU	SEGMENT
_$S272	DD	FLAT:_$E271
CRT$XCU	ENDS
_DATA	SEGMENT
_ENDL	DD	FLAT:$SG75049
_ALLFILESMASK DD FLAT:$SG75051
_DATA	ENDS
CRT$XCU	SEGMENT
_$S277	DD	FLAT:_$E276
CRT$XCU	ENDS
_DATA	SEGMENT
$SG75049 DB	0dH, 00H, 0aH, 00H, 00H, 00H
	ORG $+2
$SG75051 DB	'*', 00H, '.', 00H, '*', 00H, 00H, 00H
_DATA	ENDS
EXTRN	__chkesp:NEAR
;	COMDAT _$E271
_TEXT	SEGMENT
_$E271	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E268
	call	_$E270
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E271	ENDP
_TEXT	ENDS
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
_BSS	SEGMENT
_s_sLogAppDataKey DD 01H DUP (?)
_BSS	ENDS
;	COMDAT _$E268
_TEXT	SEGMENT
_$E268	PROC NEAR					; COMDAT

; 25   : static CString s_sLogAppDataKey;

	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_s_sLogAppDataKey
	call	??0CString@@QAE@XZ			; CString::CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E268	ENDP
_TEXT	ENDS
EXTRN	_atexit:NEAR
;	COMDAT _$E270
_TEXT	SEGMENT
_$E270	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	push	OFFSET FLAT:_$E269
	call	_atexit
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E270	ENDP
_TEXT	ENDS
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT _$E269
_TEXT	SEGMENT
_$E269	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_s_sLogAppDataKey
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E269	ENDP
_TEXT	ENDS
;	COMDAT _$E276
_TEXT	SEGMENT
_$E276	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E273
	call	_$E275
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E276	ENDP
_TEXT	ENDS
_BSS	SEGMENT
_EMPTYSTRING DD	01H DUP (?)
_BSS	ENDS
;	COMDAT _$E273
_TEXT	SEGMENT
_$E273	PROC NEAR					; COMDAT

; 31   : static CString EMPTYSTRING;

	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_EMPTYSTRING
	call	??0CString@@QAE@XZ			; CString::CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E273	ENDP
_TEXT	ENDS
;	COMDAT _$E275
_TEXT	SEGMENT
_$E275	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	push	OFFSET FLAT:_$E274
	call	_atexit
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E275	ENDP
_TEXT	ENDS
;	COMDAT _$E274
_TEXT	SEGMENT
_$E274	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_EMPTYSTRING
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E274	ENDP
_TEXT	ENDS
PUBLIC	??0CFileBackup@@QAE@ABVCString@@0K0@Z		; CFileBackup::CFileBackup
PUBLIC	??0CTempFileBackup@@QAE@ABVCString@@0K0@Z	; CTempFileBackup::CTempFileBackup
;	COMDAT ??0CTempFileBackup@@QAE@ABVCString@@0K0@Z
_TEXT	SEGMENT
_sFile$ = 8
_sFolder$ = 12
_dwFlags$ = 16
_sExt$ = 20
_this$ = -4
??0CTempFileBackup@@QAE@ABVCString@@0K0@Z PROC NEAR	; CTempFileBackup::CTempFileBackup, COMDAT

; 37   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _sExt$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwFlags$[ebp]
	push	ecx
	mov	edx, DWORD PTR _sFolder$[ebp]
	push	edx
	mov	eax, DWORD PTR _sFile$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CFileBackup@@QAE@ABVCString@@0K0@Z	; CFileBackup::CFileBackup

; 38   : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
??0CTempFileBackup@@QAE@ABVCString@@0K0@Z ENDP		; CTempFileBackup::CTempFileBackup
_TEXT	ENDS
PUBLIC	??1CFileBackup@@QAE@XZ				; CFileBackup::~CFileBackup
PUBLIC	??1CTempFileBackup@@QAE@XZ			; CTempFileBackup::~CTempFileBackup
PUBLIC	?FileExists@FileMisc@@YAHPBG@Z			; FileMisc::FileExists
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	__imp__DeleteFileW@4:NEAR
EXTRN	__imp__MoveFileExW@12:NEAR
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T76701	DD	019930520H
	DD	01H
	DD	FLAT:$T76704
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76704	DD	0ffffffffH
	DD	FLAT:$L76697
xdata$x	ENDS
;	COMDAT ??1CTempFileBackup@@QAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1CTempFileBackup@@QAE@XZ PROC NEAR			; CTempFileBackup::~CTempFileBackup, COMDAT

; 41   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76702
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 42   : 	if (FileMisc::FileExists(m_sBackup))

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?FileExists@FileMisc@@YAHPBG@Z		; FileMisc::FileExists
	add	esp, 4
	test	eax, eax
	je	SHORT $L75073

; 44   : 		if (!::DeleteFile(m_sBackup))

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__DeleteFileW@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75073

; 45   : 			MoveFileEx(m_sBackup, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);

	mov	esi, esp
	push	4
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	DWORD PTR __imp__MoveFileExW@12
	cmp	esi, esp
	call	__chkesp
$L75073:

; 47   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CFileBackup@@QAE@XZ			; CFileBackup::~CFileBackup
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76697:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CFileBackup@@QAE@XZ			; CFileBackup::~CFileBackup
	ret	0
$L76702:
	mov	eax, OFFSET FLAT:$T76701
	jmp	___CxxFrameHandler
text$x	ENDS
??1CTempFileBackup@@QAE@XZ ENDP				; CTempFileBackup::~CTempFileBackup
PUBLIC	?MakeBackup@CFileBackup@@QAEHABVCString@@0K0@Z	; CFileBackup::MakeBackup
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T76713	DD	019930520H
	DD	02H
	DD	FLAT:$T76715
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76715	DD	0ffffffffH
	DD	FLAT:$L76710
	DD	00H
	DD	FLAT:$L76711
xdata$x	ENDS
;	COMDAT ??0CFileBackup@@QAE@ABVCString@@0K0@Z
_TEXT	SEGMENT
_sFile$ = 8
_sFolder$ = 12
_dwFlags$ = 16
_sExt$ = 20
_this$ = -16
__$EHRec$ = -12
??0CFileBackup@@QAE@ABVCString@@0K0@Z PROC NEAR		; CFileBackup::CFileBackup, COMDAT

; 52   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76714
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 53   : 	MakeBackup(sFile, sFolder, dwFlags, sExt);

	mov	eax, DWORD PTR _sExt$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwFlags$[ebp]
	push	ecx
	mov	edx, DWORD PTR _sFolder$[ebp]
	push	edx
	mov	eax, DWORD PTR _sFile$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?MakeBackup@CFileBackup@@QAEHABVCString@@0K0@Z ; CFileBackup::MakeBackup

; 54   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76710:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76711:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76714:
	mov	eax, OFFSET FLAT:$T76713
	jmp	___CxxFrameHandler
text$x	ENDS
??0CFileBackup@@QAE@ABVCString@@0K0@Z ENDP		; CFileBackup::CFileBackup
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T76723	DD	019930520H
	DD	01H
	DD	FLAT:$T76725
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76725	DD	0ffffffffH
	DD	FLAT:$L76720
xdata$x	ENDS
;	COMDAT ??1CFileBackup@@QAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1CFileBackup@@QAE@XZ PROC NEAR			; CFileBackup::~CFileBackup, COMDAT

; 57   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76724
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 58   : }

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76720:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76724:
	mov	eax, OFFSET FLAT:$T76723
	jmp	___CxxFrameHandler
text$x	ENDS
??1CFileBackup@@QAE@XZ ENDP				; CFileBackup::~CFileBackup
PUBLIC	?BuildBackupPath@CFileBackup@@SA?AVCString@@ABV2@0K0@Z ; CFileBackup::BuildBackupPath
PUBLIC	?CreateFolderFromFilePath@FileMisc@@YAHPBG@Z	; FileMisc::CreateFolderFromFilePath
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	?AfxTrace@@YAXPBGZZ:NEAR			; AfxTrace
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
EXTRN	??H@YG?AVCString@@ABV0@D@Z:NEAR			; operator+
EXTRN	__imp__CopyFileW@12:NEAR
EXTRN	?FormatGetLastError@Misc@@YA?AVCString@@K@Z:NEAR ; Misc::FormatGetLastError
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75096 DB	'D:\_CODE\Shared\FILEMISC.CPP', 00H
	ORG $+3
$SG75110 DB	'D:\_CODE\Shared\FILEMISC.CPP', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76737	DD	019930520H
	DD	03H
	DD	FLAT:$T76739
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76739	DD	0ffffffffH
	DD	FLAT:$L76733
	DD	0ffffffffH
	DD	FLAT:$L76734
	DD	01H
	DD	FLAT:$L76735
xdata$x	ENDS
;	COMDAT ?MakeBackup@CFileBackup@@QAEHABVCString@@0K0@Z
_TEXT	SEGMENT
_sFile$ = 8
_sFolder$ = 12
_dwFlags$ = 16
_sExt$ = 20
_this$ = -32
_bRes$ = -16
$T76730 = -20
$T76731 = -24
$T76732 = -28
__$EHRec$ = -12
?MakeBackup@CFileBackup@@QAEHABVCString@@0K0@Z PROC NEAR ; CFileBackup::MakeBackup, COMDAT

; 61   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76738
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 44					; 0000002cH
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-56]
	mov	ecx, 11					; 0000000bH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L75091:

; 62   : 	ASSERT (m_sFile.IsEmpty() && m_sBackup.IsEmpty());

	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75095
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75094
$L75095:
	push	62					; 0000003eH
	push	OFFSET FLAT:$SG75096
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75094
	int	3
$L75094:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75091

; 63   : 
; 64   : 	if (!m_sFile.IsEmpty() || !m_sBackup.IsEmpty())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75098
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75097
$L75098:

; 65   : 		return FALSE;

	xor	eax, eax
	jmp	$L75090
$L75097:

; 66   : 
; 67   : 	if (!FileMisc::FileExists(sFile))

	mov	ecx, DWORD PTR _sFile$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?FileExists@FileMisc@@YAHPBG@Z		; FileMisc::FileExists
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75099

; 68   : 		return FALSE;

	xor	eax, eax
	jmp	$L75090
$L75099:

; 69   : 
; 70   : 	m_sFile = sFile;

	mov	ecx, DWORD PTR _sFile$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=

; 71   : 	m_sBackup = BuildBackupPath(sFile, sFolder, dwFlags, sExt);

	mov	edx, DWORD PTR _sExt$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwFlags$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sFolder$[ebp]
	push	ecx
	mov	edx, DWORD PTR _sFile$[ebp]
	push	edx
	lea	eax, DWORD PTR $T76730[ebp]
	push	eax
	call	?BuildBackupPath@CFileBackup@@SA?AVCString@@ABV2@0K0@Z ; CFileBackup::BuildBackupPath
	add	esp, 20					; 00000014H
	mov	DWORD PTR -36+[ebp], eax
	mov	ecx, DWORD PTR -36+[ebp]
	mov	DWORD PTR -40+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	edx, DWORD PTR -40+[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76730[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 72   : 	FileMisc::CreateFolderFromFilePath(m_sBackup);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?CreateFolderFromFilePath@FileMisc@@YAHPBG@Z ; FileMisc::CreateFolderFromFilePath
	add	esp, 4

; 73   : 
; 74   : 	BOOL bRes = ::CopyFile(m_sFile, m_sBackup, FALSE);

	mov	esi, esp
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	DWORD PTR __imp__CopyFileW@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bRes$[ebp], eax

; 75   : 
; 76   : 	if (!bRes)

	cmp	DWORD PTR _bRes$[ebp], 0
	jne	SHORT $L75102

; 78   : 		TRACE(Misc::FormatGetLastError() + '\n');

	push	10					; 0000000aH
	push	-1
	lea	eax, DWORD PTR $T76731[ebp]
	push	eax
	call	?FormatGetLastError@Misc@@YA?AVCString@@K@Z ; Misc::FormatGetLastError
	add	esp, 8
	mov	DWORD PTR -44+[ebp], eax
	mov	ecx, DWORD PTR -44+[ebp]
	mov	DWORD PTR -48+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	mov	edx, DWORD PTR -48+[ebp]
	push	edx
	lea	eax, DWORD PTR $T76732[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@D@Z		; operator+
	mov	DWORD PTR -52+[ebp], eax
	mov	ecx, DWORD PTR -52+[ebp]
	mov	DWORD PTR -56+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR -56+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?AfxTrace@@YAXPBGZZ			; AfxTrace
	add	esp, 4
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T76732[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76731[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 80   : 	else

	jmp	SHORT $L75108
$L75102:

; 81   : 		ASSERT (FileMisc::FileExists(m_sBackup));

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?FileExists@FileMisc@@YAHPBG@Z		; FileMisc::FileExists
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75109
	push	81					; 00000051H
	push	OFFSET FLAT:$SG75110
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75109
	int	3
$L75109:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75102
$L75108:

; 82   : 
; 83   : 	return bRes;

	mov	eax, DWORD PTR _bRes$[ebp]
$L75090:

; 84   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 56					; 00000038H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76733:
	lea	ecx, DWORD PTR $T76730[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76734:
	lea	ecx, DWORD PTR $T76731[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76735:
	lea	ecx, DWORD PTR $T76732[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76738:
	mov	eax, OFFSET FLAT:$T76737
	jmp	___CxxFrameHandler
text$x	ENDS
?MakeBackup@CFileBackup@@QAEHABVCString@@0K0@Z ENDP	; CFileBackup::MakeBackup
PUBLIC	?RestoreBackup@CFileBackup@@QAEHXZ		; CFileBackup::RestoreBackup
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75119 DB	'D:\_CODE\Shared\FILEMISC.CPP', 00H
data	ENDS
;	COMDAT ?RestoreBackup@CFileBackup@@QAEHXZ
_TEXT	SEGMENT
_this$ = -4
?RestoreBackup@CFileBackup@@QAEHXZ PROC NEAR		; CFileBackup::RestoreBackup, COMDAT

; 87   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L75114:

; 88   : 	ASSERT (!m_sFile.IsEmpty() && !m_sBackup.IsEmpty());

	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75118
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75117
$L75118:
	push	88					; 00000058H
	push	OFFSET FLAT:$SG75119
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75117
	int	3
$L75117:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75114

; 89   : 
; 90   : 	if (m_sFile.IsEmpty() || m_sBackup.IsEmpty())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75121
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75120
$L75121:

; 91   : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75113
$L75120:

; 92   : 
; 93   : 	return ::CopyFile(m_sBackup, m_sFile, FALSE);

	mov	esi, esp
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	DWORD PTR __imp__CopyFileW@12
	cmp	esi, esp
	call	__chkesp
$L75113:

; 94   : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?RestoreBackup@CFileBackup@@QAEHXZ ENDP			; CFileBackup::RestoreBackup
_TEXT	ENDS
PUBLIC	?ReplaceExtension@FileMisc@@YAAAVCString@@AAV2@PBG@Z ; FileMisc::ReplaceExtension
PUBLIC	?GetAppVersion@FileMisc@@YA?AVCString@@XZ	; FileMisc::GetAppVersion
PUBLIC	?SplitPath@FileMisc@@YAXPBGPAVCString@@111@Z	; FileMisc::SplitPath
PUBLIC	?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z	; FileMisc::MakePath
EXTRN	?GetTickCount@COleDateTime@@SG?AV1@XZ:NEAR	; COleDateTime::GetTickCount
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
EXTRN	??0CString@@QAE@PBD@Z:NEAR			; CString::CString
EXTRN	?Format@COleDateTime@@QBE?AVCString@@PBG@Z:NEAR	; COleDateTime::Format
EXTRN	??YCString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator+=
EXTRN	??H@YG?AVCString@@ABV0@0@Z:NEAR			; operator+
EXTRN	??H@YG?AVCString@@DABV0@@Z:NEAR			; operator+
EXTRN	??H@YG?AVCString@@PBGABV0@@Z:NEAR		; operator+
EXTRN	__imp__PathIsRelativeW@4:NEAR
EXTRN	?TrimRight@CString@@QAEXXZ:NEAR			; CString::TrimRight
EXTRN	?Replace@CString@@QAEHPBG0@Z:NEAR		; CString::Replace
EXTRN	?Find@CString@@QBEHG@Z:NEAR			; CString::Find
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75142 DB	'%', 00H, 'Y', 00H, '-', 00H, '%', 00H, 'm', 00H, '-', 00H
	DB	'%', 00H, 'd', 00H, '_', 00H, '%', 00H, 'H', 00H, '-', 00H, '%'
	DB	00H, 'M', 00H, '-', 00H, '%', 00H, 'S', 00H, 00H, 00H
$SG75146 DB	'.', 00H
	ORG $+2
$SG75150 DB	'_', 00H, 00H, 00H
$SG75151 DB	'.', 00H, 00H, 00H
$SG75155 DB	'.', 00H
	ORG $+2
$SG75159 DB	'.', 00H, 'b', 00H, 'a', 00H, 'k', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76781	DD	019930520H
	DD	013H
	DD	FLAT:$T76783
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76783	DD	0ffffffffH
	DD	FLAT:$L76777
	DD	00H
	DD	FLAT:$L76759
	DD	01H
	DD	FLAT:$L76760
	DD	02H
	DD	FLAT:$L76761
	DD	03H
	DD	FLAT:$L76762
	DD	04H
	DD	FLAT:$L76763
	DD	01H
	DD	FLAT:$L76764
	DD	06H
	DD	FLAT:$L76765
	DD	07H
	DD	FLAT:$L76766
	DD	08H
	DD	FLAT:$L76767
	DD	09H
	DD	FLAT:$L76768
	DD	06H
	DD	FLAT:$L76769
	DD	0bH
	DD	FLAT:$L76770
	DD	0cH
	DD	FLAT:$L76771
	DD	0dH
	DD	FLAT:$L76772
	DD	06H
	DD	FLAT:$L76773
	DD	06H
	DD	FLAT:$L76774
	DD	010H
	DD	FLAT:$L76775
	DD	06H
	DD	FLAT:$L76776
xdata$x	ENDS
;	COMDAT ?BuildBackupPath@CFileBackup@@SA?AVCString@@ABV2@0K0@Z
_TEXT	SEGMENT
_sFile$ = 12
_sFolder$ = 16
_dwFlags$ = 20
_sExt$ = 24
___$ReturnUdt$ = 8
_sBackup$ = -20
_sDrive$75131 = -24
_sPath$75132 = -36
_sFName$75133 = -32
_sExt$75134 = -28
_sFExt$ = -16
_sDate$75139 = -40
_sVersion$75148 = -44
$T76748 = -56
$T76749 = -60
$T76750 = -64
$T76751 = -68
$T76752 = -72
$T76753 = -76
$T76754 = -80
$T76755 = -84
$T76756 = -88
$T76757 = -92
$T76758 = -96
$T76778 = -100
__$EHRec$ = -12
?BuildBackupPath@CFileBackup@@SA?AVCString@@ABV2@0K0@Z PROC NEAR ; CFileBackup::BuildBackupPath, COMDAT

; 97   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76782
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 152				; 00000098H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-164]
	mov	ecx, 38					; 00000026H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T76778[ebp], 0

; 98   : 	CString sBackup(sFile);

	mov	eax, DWORD PTR _sFile$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sBackup$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 99   : 	sBackup.TrimRight();

	lea	ecx, DWORD PTR _sBackup$[ebp]
	call	?TrimRight@CString@@QAEXXZ		; CString::TrimRight

; 100  : 
; 101  : 	// handle folder
; 102  : 	if (!sFolder.IsEmpty())

	mov	ecx, DWORD PTR _sFolder$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	$L75130

; 104  : 		CString sDrive, sPath, sFName, sExt;

	lea	ecx, DWORD PTR _sDrive$75131[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sPath$75132[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR _sFName$75133[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR _sExt$75134[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 5

; 105  : 		FileMisc::SplitPath(sFile, &sDrive, &sPath, &sFName, &sExt);

	lea	ecx, DWORD PTR _sExt$75134[ebp]
	push	ecx
	lea	edx, DWORD PTR _sFName$75133[ebp]
	push	edx
	lea	eax, DWORD PTR _sPath$75132[ebp]
	push	eax
	lea	ecx, DWORD PTR _sDrive$75131[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sFile$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?SplitPath@FileMisc@@YAXPBGPAVCString@@111@Z ; FileMisc::SplitPath
	add	esp, 20					; 00000014H

; 106  : 
; 107  : 		if (::PathIsRelative(sFolder))

	mov	ecx, DWORD PTR _sFolder$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__PathIsRelativeW@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L75135

; 109  : 			sPath += sFolder;

	mov	edx, DWORD PTR _sFolder$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sPath$75132[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=

; 110  : 			FileMisc::MakePath(sBackup, sDrive, sPath, sFName, sExt);

	lea	ecx, DWORD PTR _sExt$75134[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sFName$75133[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sPath$75132[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sDrive$75131[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	eax, DWORD PTR _sBackup$[ebp]
	push	eax
	call	?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z ; FileMisc::MakePath
	add	esp, 20					; 00000014H

; 112  : 		else

	jmp	SHORT $L75136
$L75135:

; 113  : 			FileMisc::MakePath(sBackup, NULL, sFolder, sFName, sExt);

	lea	ecx, DWORD PTR _sExt$75134[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sFName$75133[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _sFolder$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	0
	lea	ecx, DWORD PTR _sBackup$[ebp]
	push	ecx
	call	?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z ; FileMisc::MakePath
	add	esp, 20					; 00000014H
$L75136:

; 114  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR _sExt$75134[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR _sFName$75133[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sPath$75132[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sDrive$75131[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75130:

; 115  : 
; 116  : 	// add timestamp before existing file extension
; 117  : 	CString sFExt;

	lea	ecx, DWORD PTR _sFExt$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 6

; 118  : 	FileMisc::SplitPath(sBackup, NULL, NULL, NULL, &sFExt);

	lea	edx, DWORD PTR _sFExt$[ebp]
	push	edx
	push	0
	push	0
	push	0
	lea	ecx, DWORD PTR _sBackup$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?SplitPath@FileMisc@@YAXPBGPAVCString@@111@Z ; FileMisc::SplitPath
	add	esp, 20					; 00000014H

; 119  : 
; 120  : 	if (dwFlags & FBS_TIMESTAMP)

	mov	eax, DWORD PTR _dwFlags$[ebp]
	and	eax, 1
	test	eax, eax
	je	$L75138

; 122  : 		// use ISO date and 24 hour time so that backups can be sorted 
; 123  : 		// by name in date order
; 124  : 		CString sDate = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d_%H-%M-%S"));

	push	OFFSET FLAT:$SG75142
	lea	ecx, DWORD PTR _sDate$75139[ebp]
	push	ecx
	lea	edx, DWORD PTR $T76748[ebp]
	push	edx
	call	?GetTickCount@COleDateTime@@SG?AV1@XZ	; COleDateTime::GetTickCount
	mov	ecx, eax
	call	?Format@COleDateTime@@QBE?AVCString@@PBG@Z ; COleDateTime::Format
	mov	BYTE PTR __$EHRec$[ebp+8], 7

; 125  : 		sFExt = "." + sDate + sFExt;

	push	OFFSET FLAT:$SG75146
	lea	ecx, DWORD PTR $T76749[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 8
	lea	eax, DWORD PTR _sFExt$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sDate$75139[ebp]
	push	ecx
	lea	edx, DWORD PTR $T76749[ebp]
	push	edx
	lea	eax, DWORD PTR $T76750[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -104+[ebp], eax
	mov	ecx, DWORD PTR -104+[ebp]
	mov	DWORD PTR -108+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 9
	mov	edx, DWORD PTR -108+[ebp]
	push	edx
	lea	eax, DWORD PTR $T76751[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -112+[ebp], eax
	mov	ecx, DWORD PTR -112+[ebp]
	mov	DWORD PTR -116+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 10		; 0000000aH
	mov	edx, DWORD PTR -116+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sFExt$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 9
	lea	ecx, DWORD PTR $T76751[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 8
	lea	ecx, DWORD PTR $T76750[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	lea	ecx, DWORD PTR $T76749[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 126  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR _sDate$75139[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75138:

; 127  : 
; 128  : 	// and app version
; 129  : 	if (dwFlags & FBS_APPVERSION)

	mov	eax, DWORD PTR _dwFlags$[ebp]
	and	eax, 2
	test	eax, eax
	je	$L75147

; 131  : 		CString sVersion = FileMisc::GetAppVersion();

	lea	ecx, DWORD PTR _sVersion$75148[ebp]
	push	ecx
	call	?GetAppVersion@FileMisc@@YA?AVCString@@XZ ; FileMisc::GetAppVersion
	add	esp, 4
	mov	BYTE PTR __$EHRec$[ebp+8], 11		; 0000000bH

; 132  : 		sVersion.Replace(_T("."), _T("_"));

	push	OFFSET FLAT:$SG75150
	push	OFFSET FLAT:$SG75151
	lea	ecx, DWORD PTR _sVersion$75148[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 133  : 
; 134  : 		sFExt = "." + sVersion + sFExt;

	push	OFFSET FLAT:$SG75155
	lea	ecx, DWORD PTR $T76752[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 12		; 0000000cH
	lea	edx, DWORD PTR _sFExt$[ebp]
	push	edx
	lea	eax, DWORD PTR _sVersion$75148[ebp]
	push	eax
	lea	ecx, DWORD PTR $T76752[ebp]
	push	ecx
	lea	edx, DWORD PTR $T76753[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -120+[ebp], eax
	mov	eax, DWORD PTR -120+[ebp]
	mov	DWORD PTR -124+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 13		; 0000000dH
	mov	ecx, DWORD PTR -124+[ebp]
	push	ecx
	lea	edx, DWORD PTR $T76754[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -128+[ebp], eax
	mov	eax, DWORD PTR -128+[ebp]
	mov	DWORD PTR -132+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 14		; 0000000eH
	mov	ecx, DWORD PTR -132+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sFExt$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 13		; 0000000dH
	lea	ecx, DWORD PTR $T76754[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 12		; 0000000cH
	lea	ecx, DWORD PTR $T76753[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 11		; 0000000bH
	lea	ecx, DWORD PTR $T76752[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 135  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR _sVersion$75148[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75147:

; 136  : 
; 137  : 	// add extension before existing file extension
; 138  : 	if (sExt.IsEmpty())

	mov	ecx, DWORD PTR _sExt$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75156

; 140  : 		// only add a default extension if not copying to another folder or not adding timestamp
; 141  : 		// or versioninfo else we'd overwrite the existing file which wouldn't achieve much
; 142  : 		if (sFolder.IsEmpty() && (dwFlags == 0))

	mov	ecx, DWORD PTR _sFolder$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75157
	cmp	DWORD PTR _dwFlags$[ebp], 0
	jne	SHORT $L75157

; 143  : 			sFExt = _T(".bak") + sFExt;

	lea	edx, DWORD PTR _sFExt$[ebp]
	push	edx
	push	OFFSET FLAT:$SG75159
	lea	eax, DWORD PTR $T76755[ebp]
	push	eax
	call	??H@YG?AVCString@@PBGABV0@@Z		; operator+
	mov	DWORD PTR -136+[ebp], eax
	mov	ecx, DWORD PTR -136+[ebp]
	mov	DWORD PTR -140+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 15		; 0000000fH
	mov	edx, DWORD PTR -140+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sFExt$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR $T76755[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75157:

; 145  : 	else

	jmp	$L75164
$L75156:

; 147  : 		if (sExt.Find('.') == -1)

	push	46					; 0000002eH
	mov	ecx, DWORD PTR _sExt$[ebp]
	call	?Find@CString@@QBEHG@Z			; CString::Find
	cmp	eax, -1
	jne	SHORT $L75161

; 148  : 			sFExt = '.' + sExt + sFExt;

	lea	eax, DWORD PTR _sFExt$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sExt$[ebp]
	push	ecx
	push	46					; 0000002eH
	lea	edx, DWORD PTR $T76756[ebp]
	push	edx
	call	??H@YG?AVCString@@DABV0@@Z		; operator+
	mov	DWORD PTR -144+[ebp], eax
	mov	eax, DWORD PTR -144+[ebp]
	mov	DWORD PTR -148+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 16		; 00000010H
	mov	ecx, DWORD PTR -148+[ebp]
	push	ecx
	lea	edx, DWORD PTR $T76757[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -152+[ebp], eax
	mov	eax, DWORD PTR -152+[ebp]
	mov	DWORD PTR -156+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 17		; 00000011H
	mov	ecx, DWORD PTR -156+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sFExt$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 16		; 00000010H
	lea	ecx, DWORD PTR $T76757[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR $T76756[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 149  : 		else

	jmp	SHORT $L75164
$L75161:

; 150  : 			sFExt = sExt + sFExt;

	lea	edx, DWORD PTR _sFExt$[ebp]
	push	edx
	mov	eax, DWORD PTR _sExt$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T76758[ebp]
	push	ecx
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -160+[ebp], eax
	mov	edx, DWORD PTR -160+[ebp]
	mov	DWORD PTR -164+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 18		; 00000012H
	mov	eax, DWORD PTR -164+[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFExt$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR $T76758[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75164:

; 152  : 
; 153  : 	FileMisc::ReplaceExtension(sBackup, sFExt);

	lea	ecx, DWORD PTR _sFExt$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sBackup$[ebp]
	push	ecx
	call	?ReplaceExtension@FileMisc@@YAAAVCString@@AAV2@PBG@Z ; FileMisc::ReplaceExtension
	add	esp, 8

; 154  : 
; 155  : 	return sBackup;

	lea	edx, DWORD PTR _sBackup$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T76778[ebp]
	or	al, 1
	mov	DWORD PTR $T76778[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sFExt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sBackup$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 156  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 164				; 000000a4H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76759:
	lea	ecx, DWORD PTR _sBackup$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76760:
	lea	ecx, DWORD PTR _sDrive$75131[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76761:
	lea	ecx, DWORD PTR _sPath$75132[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76762:
	lea	ecx, DWORD PTR _sFName$75133[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76763:
	lea	ecx, DWORD PTR _sExt$75134[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76764:
	lea	ecx, DWORD PTR _sFExt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76765:
	lea	ecx, DWORD PTR _sDate$75139[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76766:
	lea	ecx, DWORD PTR $T76749[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76767:
	lea	ecx, DWORD PTR $T76750[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76768:
	lea	ecx, DWORD PTR $T76751[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76769:
	lea	ecx, DWORD PTR _sVersion$75148[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76770:
	lea	ecx, DWORD PTR $T76752[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76771:
	lea	ecx, DWORD PTR $T76753[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76772:
	lea	ecx, DWORD PTR $T76754[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76773:
	lea	ecx, DWORD PTR $T76755[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76774:
	lea	ecx, DWORD PTR $T76756[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76775:
	lea	ecx, DWORD PTR $T76757[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76776:
	lea	ecx, DWORD PTR $T76758[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76777:
	mov	eax, DWORD PTR $T76778[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76779
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76779:
	ret	0
$L76782:
	mov	eax, OFFSET FLAT:$T76781
	jmp	___CxxFrameHandler
text$x	ENDS
?BuildBackupPath@CFileBackup@@SA?AVCString@@ABV2@0K0@Z ENDP ; CFileBackup::BuildBackupPath
PUBLIC	?TerminatePath@FileMisc@@YAAAVCString@@AAV2@@Z	; FileMisc::TerminatePath
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	??YCString@@QAEABV0@D@Z:NEAR			; CString::operator+=
EXTRN	?ReverseFind@CString@@QBEHG@Z:NEAR		; CString::ReverseFind
;	COMDAT ?TerminatePath@FileMisc@@YAAAVCString@@AAV2@@Z
_TEXT	SEGMENT
_sPath$ = 8
?TerminatePath@FileMisc@@YAAAVCString@@AAV2@@Z PROC NEAR ; FileMisc::TerminatePath, COMDAT

; 161  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 162  : 	sPath.TrimRight();

	mov	ecx, DWORD PTR _sPath$[ebp]
	call	?TrimRight@CString@@QAEXXZ		; CString::TrimRight

; 163  : 
; 164  : 	if (sPath.ReverseFind('\\') != (sPath.GetLength() - 1))

	push	92					; 0000005cH
	mov	ecx, DWORD PTR _sPath$[ebp]
	call	?ReverseFind@CString@@QBEHG@Z		; CString::ReverseFind
	mov	esi, eax
	mov	ecx, DWORD PTR _sPath$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	sub	eax, 1
	cmp	esi, eax
	je	SHORT $L75169

; 165  : 		sPath += '\\';

	push	92					; 0000005cH
	mov	ecx, DWORD PTR _sPath$[ebp]
	call	??YCString@@QAEABV0@D@Z			; CString::operator+=
$L75169:

; 166  : 
; 167  : 	return sPath;

	mov	eax, DWORD PTR _sPath$[ebp]

; 168  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?TerminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ENDP	; FileMisc::TerminatePath
_TEXT	ENDS
PUBLIC	?TerminatePath@FileMisc@@YA?AVCString@@PBG@Z	; FileMisc::TerminatePath
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T76796	DD	019930520H
	DD	02H
	DD	FLAT:$T76798
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76798	DD	0ffffffffH
	DD	FLAT:$L76792
	DD	00H
	DD	FLAT:$L76791
xdata$x	ENDS
;	COMDAT ?TerminatePath@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT
_szPath$ = 12
___$ReturnUdt$ = 8
_sPath$ = -16
$T76793 = -20
__$EHRec$ = -12
?TerminatePath@FileMisc@@YA?AVCString@@PBG@Z PROC NEAR	; FileMisc::TerminatePath, COMDAT

; 171  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76797
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T76793[ebp], 0

; 172  : 	CString sPath(szPath);

	mov	eax, DWORD PTR _szPath$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 173  : 	return TerminatePath(sPath);

	lea	ecx, DWORD PTR _sPath$[ebp]
	push	ecx
	call	?TerminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ; FileMisc::TerminatePath
	add	esp, 4
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76793[ebp]
	or	edx, 1
	mov	DWORD PTR $T76793[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 174  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76791:
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76792:
	mov	eax, DWORD PTR $T76793[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76794
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76794:
	ret	0
$L76797:
	mov	eax, OFFSET FLAT:$T76796
	jmp	___CxxFrameHandler
text$x	ENDS
?TerminatePath@FileMisc@@YA?AVCString@@PBG@Z ENDP	; FileMisc::TerminatePath
PUBLIC	?UnterminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ; FileMisc::UnterminatePath
EXTRN	?Left@CString@@QBE?AV1@H@Z:NEAR			; CString::Left
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T76806	DD	019930520H
	DD	01H
	DD	FLAT:$T76808
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76808	DD	0ffffffffH
	DD	FLAT:$L76804
xdata$x	ENDS
;	COMDAT ?UnterminatePath@FileMisc@@YAAAVCString@@AAV2@@Z
_TEXT	SEGMENT
$T76803 = -20
__$EHRec$ = -12
_sPath$ = 8
_len$ = -16
?UnterminatePath@FileMisc@@YAAAVCString@@AAV2@@Z PROC NEAR ; FileMisc::UnterminatePath, COMDAT

; 177  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76807
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 178  : 	sPath.TrimRight();

	mov	ecx, DWORD PTR _sPath$[ebp]
	call	?TrimRight@CString@@QAEXXZ		; CString::TrimRight

; 179  : 
; 180  : 	int len = sPath.GetLength();

	mov	ecx, DWORD PTR _sPath$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR _len$[ebp], eax

; 181  : 
; 182  : 	if (sPath.ReverseFind('\\') == (len - 1))

	push	92					; 0000005cH
	mov	ecx, DWORD PTR _sPath$[ebp]
	call	?ReverseFind@CString@@QBEHG@Z		; CString::ReverseFind
	mov	ecx, DWORD PTR _len$[ebp]
	sub	ecx, 1
	cmp	eax, ecx
	jne	SHORT $L75179

; 183  : 		sPath = sPath.Left(len - 1);

	mov	edx, DWORD PTR _len$[ebp]
	sub	edx, 1
	push	edx
	lea	eax, DWORD PTR $T76803[ebp]
	push	eax
	mov	ecx, DWORD PTR _sPath$[ebp]
	call	?Left@CString@@QBE?AV1@H@Z		; CString::Left
	mov	DWORD PTR -24+[ebp], eax
	mov	ecx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	edx, DWORD PTR -28+[ebp]
	push	edx
	mov	ecx, DWORD PTR _sPath$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76803[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75179:

; 184  : 
; 185  : 	return sPath;

	mov	eax, DWORD PTR _sPath$[ebp]

; 186  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76804:
	lea	ecx, DWORD PTR $T76803[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76807:
	mov	eax, OFFSET FLAT:$T76806
	jmp	___CxxFrameHandler
text$x	ENDS
?UnterminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ENDP	; FileMisc::UnterminatePath
PUBLIC	?UnterminatePath@FileMisc@@YA?AVCString@@PBG@Z	; FileMisc::UnterminatePath
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T76818	DD	019930520H
	DD	02H
	DD	FLAT:$T76820
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76820	DD	0ffffffffH
	DD	FLAT:$L76814
	DD	00H
	DD	FLAT:$L76813
xdata$x	ENDS
;	COMDAT ?UnterminatePath@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT
$T76815 = -20
__$EHRec$ = -12
_szPath$ = 12
___$ReturnUdt$ = 8
_sPath$ = -16
?UnterminatePath@FileMisc@@YA?AVCString@@PBG@Z PROC NEAR ; FileMisc::UnterminatePath, COMDAT

; 189  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76819
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T76815[ebp], 0

; 190  : 	CString sPath(szPath);

	mov	eax, DWORD PTR _szPath$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 191  : 	return UnterminatePath(sPath);

	lea	ecx, DWORD PTR _sPath$[ebp]
	push	ecx
	call	?UnterminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ; FileMisc::UnterminatePath
	add	esp, 4
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76815[ebp]
	or	edx, 1
	mov	DWORD PTR $T76815[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 192  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76813:
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76814:
	mov	eax, DWORD PTR $T76815[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76816
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76816:
	ret	0
$L76819:
	mov	eax, OFFSET FLAT:$T76818
	jmp	___CxxFrameHandler
text$x	ENDS
?UnterminatePath@FileMisc@@YA?AVCString@@PBG@Z ENDP	; FileMisc::UnterminatePath
PUBLIC	?IsPathTerminated@FileMisc@@YAHABVCString@@@Z	; FileMisc::IsPathTerminated
;	COMDAT ?IsPathTerminated@FileMisc@@YAHABVCString@@@Z
_TEXT	SEGMENT
_sPath$ = 8
?IsPathTerminated@FileMisc@@YAHABVCString@@@Z PROC NEAR	; FileMisc::IsPathTerminated, COMDAT

; 195  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 196  : 	return (sPath.ReverseFind('\\') == (sPath.GetLength() - 1));

	push	92					; 0000005cH
	mov	ecx, DWORD PTR _sPath$[ebp]
	call	?ReverseFind@CString@@QBEHG@Z		; CString::ReverseFind
	mov	esi, eax
	mov	ecx, DWORD PTR _sPath$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	sub	eax, 1
	xor	ecx, ecx
	cmp	esi, eax
	sete	cl
	mov	eax, ecx

; 197  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?IsPathTerminated@FileMisc@@YAHABVCString@@@Z ENDP	; FileMisc::IsPathTerminated
_TEXT	ENDS
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T76832	DD	019930520H
	DD	03H
	DD	FLAT:$T76834
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76834	DD	0ffffffffH
	DD	FLAT:$L76828
	DD	00H
	DD	FLAT:$L76829
	DD	01H
	DD	FLAT:$L76830
xdata$x	ENDS
;	COMDAT ?ReplaceExtension@FileMisc@@YAAAVCString@@AAV2@PBG@Z
_TEXT	SEGMENT
$T76827 = -28
__$EHRec$ = -12
_sFilePath$ = 8
_szExt$ = 12
_sDrive$ = -16
_sDir$ = -24
_sFile$ = -20
?ReplaceExtension@FileMisc@@YAAAVCString@@AAV2@PBG@Z PROC NEAR ; FileMisc::ReplaceExtension, COMDAT

; 200  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76833
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 201  : 	CString sDrive, sDir, sFile;

	lea	ecx, DWORD PTR _sDrive$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sDir$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 202  : 
; 203  : 	SplitPath(sFilePath, &sDrive, &sDir, &sFile);

	push	0
	lea	eax, DWORD PTR _sFile$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sDir$[ebp]
	push	ecx
	lea	edx, DWORD PTR _sDrive$[ebp]
	push	edx
	mov	ecx, DWORD PTR _sFilePath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?SplitPath@FileMisc@@YAXPBGPAVCString@@111@Z ; FileMisc::SplitPath
	add	esp, 20					; 00000014H

; 204  : 	MakePath(sFilePath, sDrive, sDir, sFile, szExt);

	mov	eax, DWORD PTR _szExt$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sDir$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sDrive$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _sFilePath$[ebp]
	push	ecx
	call	?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z ; FileMisc::MakePath
	add	esp, 20					; 00000014H

; 205  : 
; 206  : 	return sFilePath;

	mov	edx, DWORD PTR _sFilePath$[ebp]
	mov	DWORD PTR $T76827[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sDir$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sDrive$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76827[ebp]

; 207  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76828:
	lea	ecx, DWORD PTR _sDrive$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76829:
	lea	ecx, DWORD PTR _sDir$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76830:
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76833:
	mov	eax, OFFSET FLAT:$T76832
	jmp	___CxxFrameHandler
text$x	ENDS
?ReplaceExtension@FileMisc@@YAAAVCString@@AAV2@PBG@Z ENDP ; FileMisc::ReplaceExtension
PUBLIC	?RemoveExtension@FileMisc@@YAAAVCString@@AAV2@@Z ; FileMisc::RemoveExtension
;	COMDAT ?RemoveExtension@FileMisc@@YAAAVCString@@AAV2@@Z
_TEXT	SEGMENT
_sFilePath$ = 8
?RemoveExtension@FileMisc@@YAAAVCString@@AAV2@@Z PROC NEAR ; FileMisc::RemoveExtension, COMDAT

; 210  : {

	push	ebp
	mov	ebp, esp

; 211  : 	return ReplaceExtension(sFilePath, NULL);

	push	0
	mov	eax, DWORD PTR _sFilePath$[ebp]
	push	eax
	call	?ReplaceExtension@FileMisc@@YAAAVCString@@AAV2@PBG@Z ; FileMisc::ReplaceExtension
	add	esp, 8

; 212  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?RemoveExtension@FileMisc@@YAAAVCString@@AAV2@@Z ENDP	; FileMisc::RemoveExtension
_TEXT	ENDS
PUBLIC	?ValidateFilepath@FileMisc@@YAAAVCString@@AAV2@PBG@Z ; FileMisc::ValidateFilepath
EXTRN	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z:NEAR	; Misc::Trim
EXTRN	??H@YG?AVCString@@ABV0@PBG@Z:NEAR		; operator+
EXTRN	?Mid@CString@@QBE?AV1@H@Z:NEAR			; CString::Mid
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75204 DB	'/', 00H, 00H, 00H
$SG75205 DB	'*', 00H, 00H, 00H
$SG75206 DB	'?', 00H, 00H, 00H
$SG75207 DB	'"', 00H, 00H, 00H
$SG75208 DB	'<', 00H, 00H, 00H
$SG75209 DB	'>', 00H, 00H, 00H
$SG75210 DB	'|', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76850	DD	019930520H
	DD	04H
	DD	FLAT:$T76852
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76852	DD	0ffffffffH
	DD	FLAT:$L76845
	DD	00H
	DD	FLAT:$L76846
	DD	01H
	DD	FLAT:$L76847
	DD	02H
	DD	FLAT:$L76848
xdata$x	ENDS
;	COMDAT ?ValidateFilepath@FileMisc@@YAAAVCString@@AAV2@PBG@Z
_TEXT	SEGMENT
$T76841 = -20
$T76842 = -24
$T76843 = -28
$T76844 = -32
__$EHRec$ = -12
_sFilepath$ = 8
_szReplace$ = 12
_nColon$ = -16
?ValidateFilepath@FileMisc@@YAAAVCString@@AAV2@PBG@Z PROC NEAR ; FileMisc::ValidateFilepath, COMDAT

; 215  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76851
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 52					; 00000034H
	push	edi
	lea	edi, DWORD PTR [ebp-64]
	mov	ecx, 13					; 0000000dH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 216  : 	Misc::Trim(sFilepath);

	push	0
	mov	eax, DWORD PTR _sFilepath$[ebp]
	push	eax
	call	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z	; Misc::Trim
	add	esp, 8

; 217  : 
; 218  : 	sFilepath.Replace(_T("/"), szReplace);

	mov	ecx, DWORD PTR _szReplace$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG75204
	mov	ecx, DWORD PTR _sFilepath$[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 219  : 	sFilepath.Replace(_T("*"), szReplace);

	mov	edx, DWORD PTR _szReplace$[ebp]
	push	edx
	push	OFFSET FLAT:$SG75205
	mov	ecx, DWORD PTR _sFilepath$[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 220  : 	sFilepath.Replace(_T("?"), szReplace);

	mov	eax, DWORD PTR _szReplace$[ebp]
	push	eax
	push	OFFSET FLAT:$SG75206
	mov	ecx, DWORD PTR _sFilepath$[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 221  : 	sFilepath.Replace(_T("\""), szReplace);

	mov	ecx, DWORD PTR _szReplace$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG75207
	mov	ecx, DWORD PTR _sFilepath$[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 222  : 	sFilepath.Replace(_T("<"), szReplace);

	mov	edx, DWORD PTR _szReplace$[ebp]
	push	edx
	push	OFFSET FLAT:$SG75208
	mov	ecx, DWORD PTR _sFilepath$[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 223  : 	sFilepath.Replace(_T(">"), szReplace);

	mov	eax, DWORD PTR _szReplace$[ebp]
	push	eax
	push	OFFSET FLAT:$SG75209
	mov	ecx, DWORD PTR _sFilepath$[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 224  : 	sFilepath.Replace(_T("|"), szReplace);

	mov	ecx, DWORD PTR _szReplace$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG75210
	mov	ecx, DWORD PTR _sFilepath$[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 225  : 
; 226  : 	// make sure if a colon exists it is the 2nd pos
; 227  : 	int nColon = sFilepath.ReverseFind(':');

	push	58					; 0000003aH
	mov	ecx, DWORD PTR _sFilepath$[ebp]
	call	?ReverseFind@CString@@QBEHG@Z		; CString::ReverseFind
	mov	DWORD PTR _nColon$[ebp], eax
$L75213:

; 228  : 
; 229  : 	while (nColon != -1 && nColon != 1)

	cmp	DWORD PTR _nColon$[ebp], -1
	je	$L75214
	cmp	DWORD PTR _nColon$[ebp], 1
	je	$L75214

; 231  : 		// replace the colon
; 232  : 		sFilepath = sFilepath.Left(nColon) + szReplace + sFilepath.Mid(nColon + 1);

	mov	edx, DWORD PTR _nColon$[ebp]
	add	edx, 1
	push	edx
	lea	eax, DWORD PTR $T76841[ebp]
	push	eax
	mov	ecx, DWORD PTR _sFilepath$[ebp]
	call	?Mid@CString@@QBE?AV1@H@Z		; CString::Mid
	mov	DWORD PTR -36+[ebp], eax
	mov	ecx, DWORD PTR -36+[ebp]
	mov	DWORD PTR -40+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	edx, DWORD PTR -40+[ebp]
	push	edx
	mov	eax, DWORD PTR _szReplace$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nColon$[ebp]
	push	ecx
	lea	edx, DWORD PTR $T76842[ebp]
	push	edx
	mov	ecx, DWORD PTR _sFilepath$[ebp]
	call	?Left@CString@@QBE?AV1@H@Z		; CString::Left
	mov	DWORD PTR -44+[ebp], eax
	mov	eax, DWORD PTR -44+[ebp]
	mov	DWORD PTR -48+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -48+[ebp]
	push	ecx
	lea	edx, DWORD PTR $T76843[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -52+[ebp], eax
	mov	eax, DWORD PTR -52+[ebp]
	mov	DWORD PTR -56+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR -56+[ebp]
	push	ecx
	lea	edx, DWORD PTR $T76844[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -60+[ebp], eax
	mov	eax, DWORD PTR -60+[ebp]
	mov	DWORD PTR -64+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR -64+[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sFilepath$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T76844[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T76843[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T76842[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76841[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 233  : 		nColon = sFilepath.ReverseFind(':');

	push	58					; 0000003aH
	mov	ecx, DWORD PTR _sFilepath$[ebp]
	call	?ReverseFind@CString@@QBEHG@Z		; CString::ReverseFind
	mov	DWORD PTR _nColon$[ebp], eax

; 234  : 	}

	jmp	$L75213
$L75214:

; 235  : 
; 236  : 	return sFilepath;

	mov	eax, DWORD PTR _sFilepath$[ebp]

; 237  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 64					; 00000040H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76845:
	lea	ecx, DWORD PTR $T76841[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76846:
	lea	ecx, DWORD PTR $T76842[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76847:
	lea	ecx, DWORD PTR $T76843[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76848:
	lea	ecx, DWORD PTR $T76844[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76851:
	mov	eax, OFFSET FLAT:$T76850
	jmp	___CxxFrameHandler
text$x	ENDS
?ValidateFilepath@FileMisc@@YAAAVCString@@AAV2@PBG@Z ENDP ; FileMisc::ValidateFilepath
PUBLIC	?ValidateFilename@FileMisc@@YAAAVCString@@AAV2@PBG@Z ; FileMisc::ValidateFilename
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75223 DB	'\', 00H, 00H, 00H
$SG75224 DB	':', 00H, 00H, 00H
data	ENDS
;	COMDAT ?ValidateFilename@FileMisc@@YAAAVCString@@AAV2@PBG@Z
_TEXT	SEGMENT
_sFilename$ = 8
_szReplace$ = 12
?ValidateFilename@FileMisc@@YAAAVCString@@AAV2@PBG@Z PROC NEAR ; FileMisc::ValidateFilename, COMDAT

; 240  : {

	push	ebp
	mov	ebp, esp

; 241  : 	sFilename.Replace(_T("\\"), szReplace);

	mov	eax, DWORD PTR _szReplace$[ebp]
	push	eax
	push	OFFSET FLAT:$SG75223
	mov	ecx, DWORD PTR _sFilename$[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 242  : 	sFilename.Replace(_T(":"), szReplace);

	mov	ecx, DWORD PTR _szReplace$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG75224
	mov	ecx, DWORD PTR _sFilename$[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 243  : 	
; 244  : 	return ValidateFilepath(sFilename, szReplace);

	mov	edx, DWORD PTR _szReplace$[ebp]
	push	edx
	mov	eax, DWORD PTR _sFilename$[ebp]
	push	eax
	call	?ValidateFilepath@FileMisc@@YAAAVCString@@AAV2@PBG@Z ; FileMisc::ValidateFilepath
	add	esp, 8

; 245  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?ValidateFilename@FileMisc@@YAAAVCString@@AAV2@PBG@Z ENDP ; FileMisc::ValidateFilename
_TEXT	ENDS
PUBLIC	?GetFileNameFromPath@FileMisc@@YA?AVCString@@PBGH@Z ; FileMisc::GetFileNameFromPath
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T76867	DD	019930520H
	DD	03H
	DD	FLAT:$T76869
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76869	DD	0ffffffffH
	DD	FLAT:$L76863
	DD	00H
	DD	FLAT:$L76861
	DD	01H
	DD	FLAT:$L76862
xdata$x	ENDS
;	COMDAT ?GetFileNameFromPath@FileMisc@@YA?AVCString@@PBGH@Z
_TEXT	SEGMENT
$T76864 = -24
__$EHRec$ = -12
_szFilepath$ = 12
_bIncExtension$ = 16
___$ReturnUdt$ = 8
_sFName$ = -20
_sExt$ = -16
?GetFileNameFromPath@FileMisc@@YA?AVCString@@PBGH@Z PROC NEAR ; FileMisc::GetFileNameFromPath, COMDAT

; 248  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76868
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T76864[ebp], 0

; 249  : 	CString sFName, sExt;

	lea	ecx, DWORD PTR _sFName$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 250  : 	SplitPath(szFilepath, NULL, NULL, &sFName, &sExt);

	lea	eax, DWORD PTR _sExt$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFName$[ebp]
	push	ecx
	push	0
	push	0
	mov	edx, DWORD PTR _szFilepath$[ebp]
	push	edx
	call	?SplitPath@FileMisc@@YAXPBGPAVCString@@111@Z ; FileMisc::SplitPath
	add	esp, 20					; 00000014H

; 251  : 
; 252  : 	if (bIncExtension)

	cmp	DWORD PTR _bIncExtension$[ebp], 0
	je	SHORT $L75232

; 253  : 		sFName += sExt;

	lea	eax, DWORD PTR _sExt$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFName$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
$L75232:

; 254  : 	
; 255  : 	return sFName;

	lea	ecx, DWORD PTR _sFName$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76864[ebp]
	or	edx, 1
	mov	DWORD PTR $T76864[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sFName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 256  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76861:
	lea	ecx, DWORD PTR _sFName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76862:
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76863:
	mov	eax, DWORD PTR $T76864[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76865
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76865:
	ret	0
$L76868:
	mov	eax, OFFSET FLAT:$T76867
	jmp	___CxxFrameHandler
text$x	ENDS
?GetFileNameFromPath@FileMisc@@YA?AVCString@@PBGH@Z ENDP ; FileMisc::GetFileNameFromPath
PUBLIC	?GetFileLastModified@FileMisc@@YAJPBG@Z		; FileMisc::GetFileLastModified
PUBLIC	?GetFileLastModified@FileMisc@@YAHPBGAAU_FILETIME@@@Z ; FileMisc::GetFileLastModified
EXTRN	__aulldiv:NEAR
EXTRN	__imp___wstat:NEAR
;	COMDAT ?GetFileLastModified@FileMisc@@YAJPBG@Z
_TEXT	SEGMENT
_szPath$ = 8
_tTime$ = -12
_fileTime$ = -8
_ull$75239 = -56
_st$75241 = -48
?GetFileLastModified@FileMisc@@YAJPBG@Z PROC NEAR	; FileMisc::GetFileLastModified, COMDAT

; 259  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 56					; 00000038H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-56]
	mov	ecx, 14					; 0000000eH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 260  : 	time_t tTime = 0;

	mov	DWORD PTR _tTime$[ebp], 0

; 261  : 	FILETIME fileTime = { 0 };

	mov	DWORD PTR _fileTime$[ebp], 0
	xor	eax, eax
	mov	DWORD PTR _fileTime$[ebp+4], eax

; 262  : 	
; 263  : 	if (GetFileLastModified(szPath, fileTime))

	lea	ecx, DWORD PTR _fileTime$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szPath$[ebp]
	push	edx
	call	?GetFileLastModified@FileMisc@@YAHPBGAAU_FILETIME@@@Z ; FileMisc::GetFileLastModified
	add	esp, 8
	test	eax, eax
	je	SHORT $L75238

; 265  : 		ULARGE_INTEGER ull = { fileTime.dwLowDateTime, fileTime.dwHighDateTime };

	mov	eax, DWORD PTR _fileTime$[ebp]
	mov	DWORD PTR _ull$75239[ebp], eax
	mov	ecx, DWORD PTR _fileTime$[ebp+4]
	mov	DWORD PTR _ull$75239[ebp+4], ecx

; 266  : 		tTime = (time_t)((ull.QuadPart / 10000000UI64) - 11644473600UI64);

	push	0
	push	10000000				; 00989680H
	mov	edx, DWORD PTR _ull$75239[ebp+4]
	push	edx
	mov	eax, DWORD PTR _ull$75239[ebp]
	push	eax
	call	__aulldiv
	sub	eax, -1240428288			; b6109100H
	sbb	edx, 2
	mov	DWORD PTR _tTime$[ebp], eax

; 267  : 		
; 268  : #ifdef _DEBUG
; 269  : 		struct _stat st;
; 270  : 		_tstat(szPath, &st);

	mov	esi, esp
	lea	ecx, DWORD PTR _st$75241[ebp]
	push	ecx
	mov	edx, DWORD PTR _szPath$[ebp]
	push	edx
	call	DWORD PTR __imp___wstat
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
$L75238:

; 273  : 	
; 274  : 	return tTime;

	mov	eax, DWORD PTR _tTime$[ebp]

; 275  : }

	pop	edi
	pop	esi
	add	esp, 56					; 00000038H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetFileLastModified@FileMisc@@YAJPBG@Z ENDP		; FileMisc::GetFileLastModified
_TEXT	ENDS
EXTRN	??0CFileFind@@QAE@XZ:NEAR			; CFileFind::CFileFind
EXTRN	??1CFileFind@@UAE@XZ:NEAR			; CFileFind::~CFileFind
EXTRN	?GetLastWriteTime@CFileFind@@UBEHPAU_FILETIME@@@Z:NEAR ; CFileFind::GetLastWriteTime
EXTRN	?FindFile@CFileFind@@UAEHPBGK@Z:NEAR		; CFileFind::FindFile
EXTRN	?FindNextFileW@CFileFind@@UAEHXZ:NEAR		; CFileFind::FindNextFileW
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T76881	DD	019930520H
	DD	01H
	DD	FLAT:$T76883
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76883	DD	0ffffffffH
	DD	FLAT:$L76879
xdata$x	ENDS
;	COMDAT ?GetFileLastModified@FileMisc@@YAHPBGAAU_FILETIME@@@Z
_TEXT	SEGMENT
$T76877 = -44
$T76878 = -48
__$EHRec$ = -12
_szPath$ = 8
_fileTime$ = 12
_ff$ = -40
?GetFileLastModified@FileMisc@@YAHPBGAAU_FILETIME@@@Z PROC NEAR ; FileMisc::GetFileLastModified, COMDAT

; 278  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76882
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 36					; 00000024H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-48], eax
	mov	DWORD PTR [ebp-44], eax
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 279  : 	// files only
; 280  : 	if (!FileExists(szPath))

	mov	eax, DWORD PTR _szPath$[ebp]
	push	eax
	call	?FileExists@FileMisc@@YAHPBG@Z		; FileMisc::FileExists
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75246

; 281  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75245
$L75246:

; 282  : 	
; 283  : 	CFileFind ff;

	lea	ecx, DWORD PTR _ff$[ebp]
	call	??0CFileFind@@QAE@XZ			; CFileFind::CFileFind
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 284  : 
; 285  : 	if (ff.FindFile(szPath))

	push	0
	mov	ecx, DWORD PTR _szPath$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _ff$[ebp]
	call	?FindFile@CFileFind@@UAEHPBGK@Z		; CFileFind::FindFile
	test	eax, eax
	je	SHORT $L75248

; 287  : 		ff.FindNextFile();

	lea	ecx, DWORD PTR _ff$[ebp]
	call	?FindNextFileW@CFileFind@@UAEHXZ	; CFileFind::FindNextFileW

; 288  : 		ff.GetLastWriteTime(&fileTime);

	mov	edx, DWORD PTR _fileTime$[ebp]
	push	edx
	lea	ecx, DWORD PTR _ff$[ebp]
	call	?GetLastWriteTime@CFileFind@@UBEHPAU_FILETIME@@@Z ; CFileFind::GetLastWriteTime

; 289  : 
; 290  : 		return TRUE;

	mov	DWORD PTR $T76877[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _ff$[ebp]
	call	??1CFileFind@@UAE@XZ			; CFileFind::~CFileFind
	mov	eax, DWORD PTR $T76877[ebp]
	jmp	SHORT $L75245
$L75248:

; 292  : 
; 293  : 	// else
; 294  : 	return FALSE;

	mov	DWORD PTR $T76878[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _ff$[ebp]
	call	??1CFileFind@@UAE@XZ			; CFileFind::~CFileFind
	mov	eax, DWORD PTR $T76878[ebp]
$L75245:

; 295  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 48					; 00000030H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76879:
	lea	ecx, DWORD PTR _ff$[ebp]
	call	??1CFileFind@@UAE@XZ			; CFileFind::~CFileFind
	ret	0
$L76882:
	mov	eax, OFFSET FLAT:$T76881
	jmp	___CxxFrameHandler
text$x	ENDS
?GetFileLastModified@FileMisc@@YAHPBGAAU_FILETIME@@@Z ENDP ; FileMisc::GetFileLastModified
PUBLIC	?SetFileLastModified@FileMisc@@YAHPBGABU_FILETIME@@@Z ; FileMisc::SetFileLastModified
EXTRN	__imp__SetFileTime@16:NEAR
EXTRN	__imp__CloseHandle@4:NEAR
EXTRN	__imp__CreateFileW@28:NEAR
;	COMDAT ?SetFileLastModified@FileMisc@@YAHPBGABU_FILETIME@@@Z
_TEXT	SEGMENT
_szPath$ = 8
_fileTime$ = 12
_hFile$ = -8
_bSuccess$ = -4
?SetFileLastModified@FileMisc@@YAHPBGABU_FILETIME@@@Z PROC NEAR ; FileMisc::SetFileLastModified, COMDAT

; 298  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 299  : 	HANDLE hFile; 
; 300  : 	
; 301  : 	hFile = CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	mov	esi, esp
	push	0
	push	128					; 00000080H
	push	3
	push	0
	push	1
	push	-2147483648				; 80000000H
	mov	eax, DWORD PTR _szPath$[ebp]
	push	eax
	call	DWORD PTR __imp__CreateFileW@28
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hFile$[ebp], eax

; 302  : 	
; 303  : 	if (hFile == INVALID_HANDLE_VALUE) 

	cmp	DWORD PTR _hFile$[ebp], -1
	jne	SHORT $L75257

; 304  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75254
$L75257:

; 305  : 
; 306  : 	BOOL bSuccess = ::SetFileTime(hFile, NULL, NULL, &fileTime);

	mov	esi, esp
	mov	ecx, DWORD PTR _fileTime$[ebp]
	push	ecx
	push	0
	push	0
	mov	edx, DWORD PTR _hFile$[ebp]
	push	edx
	call	DWORD PTR __imp__SetFileTime@16
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bSuccess$[ebp], eax

; 307  : 	::CloseHandle(hFile);

	mov	esi, esp
	mov	eax, DWORD PTR _hFile$[ebp]
	push	eax
	call	DWORD PTR __imp__CloseHandle@4
	cmp	esi, esp
	call	__chkesp

; 308  : 
; 309  : 	return bSuccess;

	mov	eax, DWORD PTR _bSuccess$[ebp]
$L75254:

; 310  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?SetFileLastModified@FileMisc@@YAHPBGABU_FILETIME@@@Z ENDP ; FileMisc::SetFileLastModified
_TEXT	ENDS
PUBLIC	?GetFileLastModified@FileMisc@@YAHPBGAAU_SYSTEMTIME@@H@Z ; FileMisc::GetFileLastModified
EXTRN	__imp__FileTimeToLocalFileTime@8:NEAR
EXTRN	__imp__FileTimeToSystemTime@8:NEAR
;	COMDAT ?GetFileLastModified@FileMisc@@YAHPBGAAU_SYSTEMTIME@@H@Z
_TEXT	SEGMENT
_szPath$ = 8
_sysTime$ = 12
_bLocalTime$ = 16
_fileTime$ = -8
_ftLocal$75267 = -16
?GetFileLastModified@FileMisc@@YAHPBGAAU_SYSTEMTIME@@H@Z PROC NEAR ; FileMisc::GetFileLastModified, COMDAT

; 313  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 314  : 	FILETIME fileTime = { 0 };

	mov	DWORD PTR _fileTime$[ebp], 0
	xor	eax, eax
	mov	DWORD PTR _fileTime$[ebp+4], eax

; 315  : 	
; 316  : 	if (!GetFileLastModified(szPath, fileTime))

	lea	ecx, DWORD PTR _fileTime$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szPath$[ebp]
	push	edx
	call	?GetFileLastModified@FileMisc@@YAHPBGAAU_FILETIME@@@Z ; FileMisc::GetFileLastModified
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75265

; 317  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75263
$L75265:

; 318  : 	
; 319  : 	if (bLocalTime)

	cmp	DWORD PTR _bLocalTime$[ebp], 0
	je	SHORT $L75266

; 321  : 		FILETIME ftLocal = { 0 };

	mov	DWORD PTR _ftLocal$75267[ebp], 0
	xor	eax, eax
	mov	DWORD PTR _ftLocal$75267[ebp+4], eax

; 322  : 		
; 323  : 		if (!FileTimeToLocalFileTime(&fileTime, &ftLocal))

	mov	esi, esp
	lea	ecx, DWORD PTR _ftLocal$75267[ebp]
	push	ecx
	lea	edx, DWORD PTR _fileTime$[ebp]
	push	edx
	call	DWORD PTR __imp__FileTimeToLocalFileTime@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75268

; 324  : 			return FALSE;

	xor	eax, eax
	jmp	SHORT $L75263
$L75268:

; 325  : 		
; 326  : 		fileTime = ftLocal;

	mov	eax, DWORD PTR _ftLocal$75267[ebp]
	mov	DWORD PTR _fileTime$[ebp], eax
	mov	ecx, DWORD PTR _ftLocal$75267[ebp+4]
	mov	DWORD PTR _fileTime$[ebp+4], ecx
$L75266:

; 328  : 	
; 329  : 	return FileTimeToSystemTime(&fileTime, &sysTime);

	mov	esi, esp
	mov	edx, DWORD PTR _sysTime$[ebp]
	push	edx
	lea	eax, DWORD PTR _fileTime$[ebp]
	push	eax
	call	DWORD PTR __imp__FileTimeToSystemTime@8
	cmp	esi, esp
	call	__chkesp
$L75263:

; 330  : }

	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetFileLastModified@FileMisc@@YAHPBGAAU_SYSTEMTIME@@H@Z ENDP ; FileMisc::GetFileLastModified
_TEXT	ENDS
PUBLIC	?TouchFile@FileMisc@@YAHPBG@Z			; FileMisc::TouchFile
EXTRN	__imp___wutime:NEAR
EXTRN	__imp__SetFileAttributesW@8:NEAR
;	COMDAT ?TouchFile@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT
_szPath$ = 8
?TouchFile@FileMisc@@YAHPBG@Z PROC NEAR			; FileMisc::TouchFile, COMDAT

; 333  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 334  : 	::SetFileAttributes(szPath, FILE_ATTRIBUTE_NORMAL);

	mov	esi, esp
	push	128					; 00000080H
	mov	eax, DWORD PTR _szPath$[ebp]
	push	eax
	call	DWORD PTR __imp__SetFileAttributesW@8
	cmp	esi, esp
	call	__chkesp

; 335  : 
; 336  : 	return (_tutime(szPath, NULL) == 0);

	mov	esi, esp
	push	0
	mov	ecx, DWORD PTR _szPath$[ebp]
	push	ecx
	call	DWORD PTR __imp___wutime
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax

; 337  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?TouchFile@FileMisc@@YAHPBG@Z ENDP			; FileMisc::TouchFile
_TEXT	ENDS
PUBLIC	?FindFiles@FileMisc@@YAHABVCString@@AAVCStringArray@@HPBG@Z ; FileMisc::FindFiles
EXTRN	?GetSize@CStringArray@@QBEHXZ:NEAR		; CStringArray::GetSize
EXTRN	?GetFilePath@CFileFind@@UBE?AVCString@@XZ:NEAR	; CFileFind::GetFilePath
EXTRN	?Add@CStringArray@@QAEHABVCString@@@Z:NEAR	; CStringArray::Add
EXTRN	?IsDots@CFileFind@@UBEHXZ:NEAR			; CFileFind::IsDots
EXTRN	?IsDirectory@CFileFind@@QBEHXZ:NEAR		; CFileFind::IsDirectory
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T76902	DD	019930520H
	DD	04H
	DD	FLAT:$T76904
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76904	DD	0ffffffffH
	DD	FLAT:$L76897
	DD	00H
	DD	FLAT:$L76898
	DD	01H
	DD	FLAT:$L76899
	DD	01H
	DD	FLAT:$L76900
xdata$x	ENDS
;	COMDAT ?FindFiles@FileMisc@@YAHABVCString@@AAVCStringArray@@HPBG@Z
_TEXT	SEGMENT
$T76894 = -52
$T76895 = -56
$T76896 = -60
__$EHRec$ = -12
_sFolder$ = 8
_aFiles$ = 12
_bCheckSubFolders$ = 16
_szPattern$ = 20
_ff$ = -44
_sSearchSpec$ = -48
_bContinue$ = -16
?FindFiles@FileMisc@@YAHABVCString@@AAVCStringArray@@HPBG@Z PROC NEAR ; FileMisc::FindFiles, COMDAT

; 340  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76903
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 64					; 00000040H
	push	edi
	lea	edi, DWORD PTR [ebp-76]
	mov	ecx, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 341  : 	CFileFind ff;

	lea	ecx, DWORD PTR _ff$[ebp]
	call	??0CFileFind@@QAE@XZ			; CFileFind::CFileFind
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 342  : 	CString sSearchSpec;

	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 343  : 
; 344  : 	MakePath(sSearchSpec, NULL, sFolder, szPattern, NULL);

	push	0
	mov	eax, DWORD PTR _szPattern$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sFolder$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	0
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	push	ecx
	call	?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z ; FileMisc::MakePath
	add	esp, 20					; 00000014H

; 345  : 
; 346  : 	BOOL bContinue = ff.FindFile(sSearchSpec);

	push	0
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _ff$[ebp]
	call	?FindFile@CFileFind@@UAEHPBGK@Z		; CFileFind::FindFile
	mov	DWORD PTR _bContinue$[ebp], eax
$L75282:

; 347  : 	
; 348  : 	while (bContinue)

	cmp	DWORD PTR _bContinue$[ebp], 0
	je	$L75283

; 350  : 		bContinue = ff.FindNextFile();

	lea	ecx, DWORD PTR _ff$[ebp]
	call	?FindNextFileW@CFileFind@@UAEHXZ	; CFileFind::FindNextFileW
	mov	DWORD PTR _bContinue$[ebp], eax

; 351  : 		
; 352  : 		if (!ff.IsDots())

	lea	ecx, DWORD PTR _ff$[ebp]
	call	?IsDots@CFileFind@@UBEHXZ		; CFileFind::IsDots
	test	eax, eax
	jne	$L75288

; 354  : 			if (ff.IsDirectory())

	lea	ecx, DWORD PTR _ff$[ebp]
	call	?IsDirectory@CFileFind@@QBEHXZ		; CFileFind::IsDirectory
	test	eax, eax
	je	SHORT $L75285

; 356  : 				if (bCheckSubFolders)

	cmp	DWORD PTR _bCheckSubFolders$[ebp], 0
	je	SHORT $L75286

; 357  : 					FindFiles(ff.GetFilePath(), aFiles, TRUE, szPattern);

	mov	edx, DWORD PTR _szPattern$[ebp]
	push	edx
	push	1
	mov	eax, DWORD PTR _aFiles$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T76894[ebp]
	push	ecx
	lea	ecx, DWORD PTR _ff$[ebp]
	call	?GetFilePath@CFileFind@@UBE?AVCString@@XZ ; CFileFind::GetFilePath
	mov	DWORD PTR -64+[ebp], eax
	mov	edx, DWORD PTR -64+[ebp]
	mov	DWORD PTR -68+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	eax, DWORD PTR -68+[ebp]
	push	eax
	call	?FindFiles@FileMisc@@YAHABVCString@@AAVCStringArray@@HPBG@Z ; FileMisc::FindFiles
	add	esp, 16					; 00000010H
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T76894[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75286:

; 359  : 			else

	jmp	SHORT $L75288
$L75285:

; 360  : 				aFiles.Add(ff.GetFilePath());

	lea	ecx, DWORD PTR $T76895[ebp]
	push	ecx
	lea	ecx, DWORD PTR _ff$[ebp]
	call	?GetFilePath@CFileFind@@UBE?AVCString@@XZ ; CFileFind::GetFilePath
	mov	DWORD PTR -72+[ebp], eax
	mov	edx, DWORD PTR -72+[ebp]
	mov	DWORD PTR -76+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	eax, DWORD PTR -76+[ebp]
	push	eax
	mov	ecx, DWORD PTR _aFiles$[ebp]
	call	?Add@CStringArray@@QAEHABVCString@@@Z	; CStringArray::Add
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T76895[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75288:

; 362  : 	}

	jmp	$L75282
$L75283:

; 363  : 
; 364  : 	return aFiles.GetSize();

	mov	ecx, DWORD PTR _aFiles$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR $T76896[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _ff$[ebp]
	call	??1CFileFind@@UAE@XZ			; CFileFind::~CFileFind
	mov	eax, DWORD PTR $T76896[ebp]

; 365  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 76					; 0000004cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76897:
	lea	ecx, DWORD PTR _ff$[ebp]
	call	??1CFileFind@@UAE@XZ			; CFileFind::~CFileFind
	ret	0
$L76898:
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76899:
	lea	ecx, DWORD PTR $T76894[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76900:
	lea	ecx, DWORD PTR $T76895[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76903:
	mov	eax, OFFSET FLAT:$T76902
	jmp	___CxxFrameHandler
text$x	ENDS
?FindFiles@FileMisc@@YAHABVCString@@AAVCStringArray@@HPBG@Z ENDP ; FileMisc::FindFiles
PUBLIC	?GetDropFilePaths@FileMisc@@YAHPAUHDROP__@@AAVCStringArray@@@Z ; FileMisc::GetDropFilePaths
EXTRN	__imp__DragQueryFileW@16:NEAR
EXTRN	__imp__GetLongPathNameW@12:NEAR
EXTRN	?RemoveAll@CStringArray@@QAEXXZ:NEAR		; CStringArray::RemoveAll
EXTRN	?Add@CStringArray@@QAEHPBG@Z:NEAR		; CStringArray::Add
;	COMDAT ?GetDropFilePaths@FileMisc@@YAHPAUHDROP__@@AAVCStringArray@@@Z
_TEXT	SEGMENT
_hDrop$ = 8
_aFiles$ = 12
_nRet$ = -532
_szFileName$ = -524
_nFileCount$ = -528
_i$75299 = -536
?GetDropFilePaths@FileMisc@@YAHPAUHDROP__@@AAVCStringArray@@@Z PROC NEAR ; FileMisc::GetDropFilePaths, COMDAT

; 368  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 536				; 00000218H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-536]
	mov	ecx, 134				; 00000086H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 369  : 	int nRet = 0;

	mov	DWORD PTR _nRet$[ebp], 0

; 370  : 
; 371  : 	aFiles.RemoveAll();

	mov	ecx, DWORD PTR _aFiles$[ebp]
	call	?RemoveAll@CStringArray@@QAEXXZ		; CStringArray::RemoveAll

; 372  : 
; 373  : 	TCHAR szFileName[_MAX_PATH+1] = { 0 };

	mov	WORD PTR _szFileName$[ebp], 0
	mov	ecx, 130				; 00000082H
	xor	eax, eax
	lea	edi, DWORD PTR _szFileName$[ebp+2]
	rep stosd

; 374  : 	UINT nFileCount = ::DragQueryFile(hDrop, 0xffffffff, NULL, 0);

	mov	esi, esp
	push	0
	push	0
	push	-1
	mov	eax, DWORD PTR _hDrop$[ebp]
	push	eax
	call	DWORD PTR __imp__DragQueryFileW@16
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nFileCount$[ebp], eax

; 375  : 
; 376  : 	if (nFileCount != 0xffffffff)

	cmp	DWORD PTR _nFileCount$[ebp], -1
	je	$L75298

; 378  : 		for (UINT i=0; i < nFileCount; i++)

	mov	DWORD PTR _i$75299[ebp], 0
	jmp	SHORT $L75300
$L75301:
	mov	ecx, DWORD PTR _i$75299[ebp]
	add	ecx, 1
	mov	DWORD PTR _i$75299[ebp], ecx
$L75300:
	mov	edx, DWORD PTR _i$75299[ebp]
	cmp	edx, DWORD PTR _nFileCount$[ebp]
	jae	SHORT $L75302

; 380  : 			::DragQueryFile(hDrop, i, szFileName, _MAX_PATH);

	mov	esi, esp
	push	260					; 00000104H
	lea	eax, DWORD PTR _szFileName$[ebp]
	push	eax
	mov	ecx, DWORD PTR _i$75299[ebp]
	push	ecx
	mov	edx, DWORD PTR _hDrop$[ebp]
	push	edx
	call	DWORD PTR __imp__DragQueryFileW@16
	cmp	esi, esp
	call	__chkesp

; 381  : 			::GetLongPathName(szFileName, szFileName, _MAX_PATH);

	mov	esi, esp
	push	260					; 00000104H
	lea	eax, DWORD PTR _szFileName$[ebp]
	push	eax
	lea	ecx, DWORD PTR _szFileName$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetLongPathNameW@12
	cmp	esi, esp
	call	__chkesp

; 382  : 			
; 383  : 			aFiles.Add(szFileName);

	lea	edx, DWORD PTR _szFileName$[ebp]
	push	edx
	mov	ecx, DWORD PTR _aFiles$[ebp]
	call	?Add@CStringArray@@QAEHPBG@Z		; CStringArray::Add

; 384  : 		}

	jmp	SHORT $L75301
$L75302:

; 385  : 
; 386  : 		nRet = aFiles.GetSize();

	mov	ecx, DWORD PTR _aFiles$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nRet$[ebp], eax

; 388  : 	else

	jmp	SHORT $L75303
$L75298:

; 390  : 		// error
; 391  : 		nRet = -1;

	mov	DWORD PTR _nRet$[ebp], -1
$L75303:

; 393  : 
; 394  : 	return nRet;

	mov	eax, DWORD PTR _nRet$[ebp]

; 395  : }

	pop	edi
	pop	esi
	add	esp, 536				; 00000218H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetDropFilePaths@FileMisc@@YAHPAUHDROP__@@AAVCStringArray@@@Z ENDP ; FileMisc::GetDropFilePaths
_TEXT	ENDS
PUBLIC	?HasFlag@Misc@@YAHKK@Z				; Misc::HasFlag
PUBLIC	?FolderExists@FileMisc@@YAHPBG@Z		; FileMisc::FolderExists
PUBLIC	?DeleteFolder@FileMisc@@YAHPBGKPAX@Z		; FileMisc::DeleteFolder
PUBLIC	?DeleteFolderContents@FileMisc@@YAHPBGK0PAX@Z	; FileMisc::DeleteFolderContents
PUBLIC	?DeleteFileW@FileMisc@@YAHPBGH@Z		; FileMisc::DeleteFileW
PUBLIC	?ModifyAttributes@FileMisc@@YAHPBGKKK@Z		; FileMisc::ModifyAttributes
PUBLIC	?LogText@FileMisc@@YAXPBGZZ			; FileMisc::LogText
EXTRN	?Split@Misc@@YAHABVCString@@AAVCStringArray@@GH@Z:NEAR ; Misc::Split
EXTRN	?ProcessMsgLoop@Misc@@YAXXZ:NEAR		; Misc::ProcessMsgLoop
EXTRN	__imp__WaitForSingleObject@8:NEAR
EXTRN	__imp__FindClose@4:NEAR
EXTRN	??0CStringArray@@QAE@XZ:NEAR			; CStringArray::CStringArray
EXTRN	??ACStringArray@@QAEAAVCString@@H@Z:NEAR	; CStringArray::operator[]
EXTRN	??1CStringArray@@UAE@XZ:NEAR			; CStringArray::~CStringArray
EXTRN	??4CString@@QAEABV0@PBG@Z:NEAR			; CString::operator=
EXTRN	__imp__FindFirstFileW@8:NEAR
EXTRN	__imp__FindNextFileW@8:NEAR
EXTRN	__imp__wcsstr:NEAR
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75354 DB	'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'M', 00H, 'i', 00H
	DB	's', 00H, 'c', 00H, ':', 00H, ':', 00H, 'D', 00H, 'e', 00H, 'l'
	DB	00H, 'e', 00H, 't', 00H, 'e', 00H, 'F', 00H, 'o', 00H, 'l', 00H
	DB	'd', 00H, 'e', 00H, 'r', 00H, 'C', 00H, 'o', 00H, 'n', 00H, 't'
	DB	00H, 'e', 00H, 'n', 00H, 't', 00H, 's', 00H, '(', 00H, '%', 00H
	DB	's', 00H, ')', 00H, ' ', 00H, 'd', 00H, 'e', 00H, 'l', 00H, 'a'
	DB	00H, 'y', 00H, 'e', 00H, 'd', 00H, '!', 00H, 00H, 00H
$SG75356 DB	'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'M', 00H, 'i', 00H
	DB	's', 00H, 'c', 00H, ':', 00H, ':', 00H, 'D', 00H, 'e', 00H, 'l'
	DB	00H, 'e', 00H, 't', 00H, 'e', 00H, 'F', 00H, 'o', 00H, 'l', 00H
	DB	'd', 00H, 'e', 00H, 'r', 00H, 'C', 00H, 'o', 00H, 'n', 00H, 't'
	DB	00H, 'e', 00H, 'n', 00H, 't', 00H, 's', 00H, '(', 00H, '%', 00H
	DB	's', 00H, ')', 00H, ' ', 00H, 'f', 00H, 'a', 00H, 'i', 00H, 'l'
	DB	00H, 'e', 00H, 'd', 00H, '!', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76928	DD	019930520H
	DD	07H
	DD	FLAT:$T76930
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76930	DD	0ffffffffH
	DD	FLAT:$L76921
	DD	00H
	DD	FLAT:$L76922
	DD	00H
	DD	FLAT:$L76923
	DD	02H
	DD	FLAT:$L76924
	DD	03H
	DD	FLAT:$L76925
	DD	04H
	DD	FLAT:$L76926
	DD	03H
	DD	FLAT:$L76926
xdata$x	ENDS
;	COMDAT ?DeleteFolderContents@FileMisc@@YAHPBGK0PAX@Z
_TEXT	SEGMENT
$T76913 = -684
$T76918 = -688
$T76919 = -692
$T76920 = -696
__$EHRec$ = -12
_szFolder$ = 8
_dwFlags$ = 12
_szFileMask$ = 16
_hTerminate$ = 20
_bWantSubFolders$ = -52
_bWantMsgLoop$ = -60
_bHiddenReadOnly$ = -64
_bDeleteAll$ = -20
_aMasks$ = -44
_nNumMasks$ = -24
_nNumPass$ = -16
_nResult$ = -68
_bStopped$ = -48
_nPass$ = -56
_sSearchSpec$75326 = -672
_sMask$75327 = -72
_finfo$75331 = -664
_hSearch$75332 = -668
_sItem$75340 = -676
_bFolder$75344 = -680
?DeleteFolderContents@FileMisc@@YAHPBGK0PAX@Z PROC NEAR	; FileMisc::DeleteFolderContents, COMDAT

; 398  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76929
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 704				; 000002c0H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-716]
	mov	ecx, 176				; 000000b0H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 399  : 	// if the dir does not exists just return
; 400  : 	if (!FolderExists(szFolder))

	mov	eax, DWORD PTR _szFolder$[ebp]
	push	eax
	call	?FolderExists@FileMisc@@YAHPBG@Z	; FileMisc::FolderExists
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75310

; 401  : 		return TRUE;

	mov	eax, 1
	jmp	$L75309
$L75310:

; 402  : 
; 403  : 	BOOL bWantSubFolders = Misc::HasFlag(dwFlags, FMDF_SUBFOLDERS);

	push	1
	mov	ecx, DWORD PTR _dwFlags$[ebp]
	push	ecx
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	mov	DWORD PTR _bWantSubFolders$[ebp], eax

; 404  : 	BOOL bWantMsgLoop	 = Misc::HasFlag(dwFlags, FMDF_PROCESSMSGLOOP);

	push	2
	mov	edx, DWORD PTR _dwFlags$[ebp]
	push	edx
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	mov	DWORD PTR _bWantMsgLoop$[ebp], eax

; 405  : 	BOOL bHiddenReadOnly = Misc::HasFlag(dwFlags, FMDF_HIDDENREADONLY);

	push	8
	mov	eax, DWORD PTR _dwFlags$[ebp]
	push	eax
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	mov	DWORD PTR _bHiddenReadOnly$[ebp], eax

; 406  : 	BOOL bDeleteAll		 = (!szFileMask || !(*szFileMask) || _tcsstr(szFileMask, ALLFILESMASK));

	cmp	DWORD PTR _szFileMask$[ebp], 0
	je	SHORT $L76911
	mov	ecx, DWORD PTR _szFileMask$[ebp]
	xor	edx, edx
	mov	dx, WORD PTR [ecx]
	test	edx, edx
	je	SHORT $L76911
	mov	esi, esp
	mov	eax, DWORD PTR _ALLFILESMASK
	push	eax
	mov	ecx, DWORD PTR _szFileMask$[ebp]
	push	ecx
	call	DWORD PTR __imp__wcsstr
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76911
	mov	DWORD PTR -700+[ebp], 0
	jmp	SHORT $L76912
$L76911:
	mov	DWORD PTR -700+[ebp], 1
$L76912:
	mov	edx, DWORD PTR -700+[ebp]
	mov	DWORD PTR _bDeleteAll$[ebp], edx

; 407  : 
; 408  : 	// if a file mask has been specified with subfolders 
; 409  : 	// we need to do multiple passes on each folder, 
; 410  : 	// one (or more) for the files and one for the sub folders
; 411  : 	CStringArray aMasks;

	lea	ecx, DWORD PTR _aMasks$[ebp]
	call	??0CStringArray@@QAE@XZ			; CStringArray::CStringArray
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 412  : 	int nNumMasks = Misc::Split(szFileMask, aMasks, ';');

	mov	eax, DWORD PTR _szFileMask$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T76913[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	push	0
	push	59					; 0000003bH
	lea	ecx, DWORD PTR _aMasks$[ebp]
	push	ecx
	lea	edx, DWORD PTR $T76913[ebp]
	push	edx
	call	?Split@Misc@@YAHABVCString@@AAVCStringArray@@GH@Z ; Misc::Split
	add	esp, 16					; 00000010H
	mov	DWORD PTR _nNumMasks$[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T76913[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 413  : 
; 414  : 	int nNumPass = (bWantSubFolders ? (nNumMasks + 1) : max(nNumMasks, 1));

	cmp	DWORD PTR _bWantSubFolders$[ebp], 0
	je	SHORT $L76916
	mov	eax, DWORD PTR _nNumMasks$[ebp]
	add	eax, 1
	mov	DWORD PTR -704+[ebp], eax
	jmp	SHORT $L76917
$L76916:
	cmp	DWORD PTR _nNumMasks$[ebp], 1
	jle	SHORT $L76914
	mov	ecx, DWORD PTR _nNumMasks$[ebp]
	mov	DWORD PTR -708+[ebp], ecx
	jmp	SHORT $L76915
$L76914:
	mov	DWORD PTR -708+[ebp], 1
$L76915:
	mov	edx, DWORD PTR -708+[ebp]
	mov	DWORD PTR -704+[ebp], edx
$L76917:
	mov	eax, DWORD PTR -704+[ebp]
	mov	DWORD PTR _nNumPass$[ebp], eax

; 415  : 		
; 416  : 	int nResult = 1; // success

	mov	DWORD PTR _nResult$[ebp], 1

; 417  : 	BOOL bStopped = (WaitForSingleObject(hTerminate, 0) == WAIT_OBJECT_0);

	mov	esi, esp
	push	0
	mov	ecx, DWORD PTR _hTerminate$[ebp]
	push	ecx
	call	DWORD PTR __imp__WaitForSingleObject@8
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR _bStopped$[ebp], eax

; 418  : 
; 419  : 	for (int nPass = 0; !bStopped && nPass < nNumPass; nPass++)

	mov	DWORD PTR _nPass$[ebp], 0
	jmp	SHORT $L75323
$L75324:
	mov	edx, DWORD PTR _nPass$[ebp]
	add	edx, 1
	mov	DWORD PTR _nPass$[ebp], edx
$L75323:
	cmp	DWORD PTR _bStopped$[ebp], 0
	jne	$L75325
	mov	eax, DWORD PTR _nPass$[ebp]
	cmp	eax, DWORD PTR _nNumPass$[ebp]
	jge	$L75325

; 421  : 		CString sSearchSpec(szFolder), sMask(szFileMask);

	mov	ecx, DWORD PTR _szFolder$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sSearchSpec$75326[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	edx, DWORD PTR _szFileMask$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sMask$75327[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3

; 422  : 
; 423  : 		if (sMask.IsEmpty() || (nPass == aMasks.GetSize())) // (nPass == last) == 2nd pass (for folders)

	lea	ecx, DWORD PTR _sMask$75327[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75329
	lea	ecx, DWORD PTR _aMasks$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	cmp	DWORD PTR _nPass$[ebp], eax
	jne	SHORT $L75328
$L75329:

; 424  : 			sMask = ALLFILESMASK;

	mov	eax, DWORD PTR _ALLFILESMASK
	push	eax
	lea	ecx, DWORD PTR _sMask$75327[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 425  : 		else

	jmp	SHORT $L75330
$L75328:

; 426  : 			sMask = aMasks[nPass];

	mov	ecx, DWORD PTR _nPass$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _aMasks$[ebp]
	call	??ACStringArray@@QAEAAVCString@@H@Z	; CStringArray::operator[]
	push	eax
	lea	ecx, DWORD PTR _sMask$75327[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
$L75330:

; 427  : 
; 428  : 		TerminatePath(sSearchSpec);

	lea	edx, DWORD PTR _sSearchSpec$75326[ebp]
	push	edx
	call	?TerminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ; FileMisc::TerminatePath
	add	esp, 4

; 429  : 		sSearchSpec += sMask;

	lea	eax, DWORD PTR _sMask$75327[ebp]
	push	eax
	lea	ecx, DWORD PTR _sSearchSpec$75326[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=

; 430  : 
; 431  : 		WIN32_FIND_DATA finfo;
; 432  : 		HANDLE hSearch = NULL;

	mov	DWORD PTR _hSearch$75332[ebp], 0

; 433  : 
; 434  : 		if ((hSearch = FindFirstFile(sSearchSpec, &finfo)) != INVALID_HANDLE_VALUE) 

	mov	esi, esp
	lea	ecx, DWORD PTR _finfo$75331[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sSearchSpec$75326[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	DWORD PTR __imp__FindFirstFileW@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hSearch$75332[ebp], eax
	cmp	DWORD PTR _hSearch$75332[ebp], -1
	je	$L75334
$L75335:

; 438  : 				if (bWantMsgLoop)

	cmp	DWORD PTR _bWantMsgLoop$[ebp], 0
	je	SHORT $L75338

; 439  : 					Misc::ProcessMsgLoop();

	call	?ProcessMsgLoop@Misc@@YAXXZ		; Misc::ProcessMsgLoop
$L75338:

; 440  : 
; 441  : 				if (finfo.cFileName[0] != '.') 

	mov	edx, DWORD PTR _finfo$75331[ebp+44]
	and	edx, 65535				; 0000ffffH
	cmp	edx, 46					; 0000002eH
	je	$L75339

; 443  : 					CString sItem = (TerminatePath(szFolder) + finfo.cFileName);

	lea	eax, DWORD PTR _finfo$75331[ebp+44]
	push	eax
	mov	ecx, DWORD PTR _szFolder$[ebp]
	push	ecx
	lea	edx, DWORD PTR $T76918[ebp]
	push	edx
	call	?TerminatePath@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::TerminatePath
	add	esp, 8
	mov	DWORD PTR -712+[ebp], eax
	mov	eax, DWORD PTR -712+[ebp]
	mov	DWORD PTR -716+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	ecx, DWORD PTR -716+[ebp]
	push	ecx
	lea	edx, DWORD PTR _sItem$75340[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR $T76918[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 444  : 
; 445  : 					// handle hidden/readonly files
; 446  : 					if (bHiddenReadOnly)

	cmp	DWORD PTR _bHiddenReadOnly$[ebp], 0
	je	SHORT $L75343

; 447  : 						ModifyAttributes(sItem, FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN, finfo.dwFileAttributes);

	push	-1
	mov	eax, DWORD PTR _finfo$75331[ebp]
	push	eax
	push	3
	lea	ecx, DWORD PTR _sItem$75340[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?ModifyAttributes@FileMisc@@YAHPBGKKK@Z	; FileMisc::ModifyAttributes
	add	esp, 16					; 00000010H
$L75343:

; 448  : 
; 449  : 					// handle directories
; 450  : 					BOOL bFolder = Misc::HasFlag(finfo.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY);

	push	16					; 00000010H
	mov	ecx, DWORD PTR _finfo$75331[ebp]
	push	ecx
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	mov	DWORD PTR _bFolder$75344[ebp], eax

; 451  : 
; 452  : 					if (bFolder && bWantSubFolders)

	cmp	DWORD PTR _bFolder$75344[ebp], 0
	je	SHORT $L75345
	cmp	DWORD PTR _bWantSubFolders$[ebp], 0
	je	SHORT $L75345

; 454  : 						// delete folder too?
; 455  : 						if (bDeleteAll)

	cmp	DWORD PTR _bDeleteAll$[ebp], 0
	je	SHORT $L75346

; 457  : 							nResult = DeleteFolder(sItem, dwFlags, hTerminate);

	mov	edx, DWORD PTR _hTerminate$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwFlags$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sItem$75340[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?DeleteFolder@FileMisc@@YAHPBGKPAX@Z	; FileMisc::DeleteFolder
	add	esp, 12					; 0000000cH
	mov	DWORD PTR _nResult$[ebp], eax

; 459  : 						else

	jmp	SHORT $L75347
$L75346:

; 461  : 							nResult = DeleteFolderContents(sItem, dwFlags, sMask, hTerminate);

	mov	ecx, DWORD PTR _hTerminate$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sMask$75327[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	edx, DWORD PTR _dwFlags$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sItem$75340[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?DeleteFolderContents@FileMisc@@YAHPBGK0PAX@Z ; FileMisc::DeleteFolderContents
	add	esp, 16					; 00000010H
	mov	DWORD PTR _nResult$[ebp], eax
$L75347:

; 464  : 					else if (!bFolder) // file

	jmp	$L75357
$L75345:
	cmp	DWORD PTR _bFolder$75344[ebp], 0
	jne	$L75357

; 466  : 						if (FileMisc::DeleteFile(sItem))

	push	0
	lea	ecx, DWORD PTR _sItem$75340[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?DeleteFileW@FileMisc@@YAHPBGH@Z	; FileMisc::DeleteFileW
	add	esp, 8
	test	eax, eax
	je	SHORT $L75350

; 468  : 							nResult = 1; // succeeded

	mov	DWORD PTR _nResult$[ebp], 1

; 470  : 						else if (dwFlags & FMDF_ALLOWDELETEONREBOOT)

	jmp	SHORT $L75357
$L75350:
	mov	eax, DWORD PTR _dwFlags$[ebp]
	and	eax, 4
	test	eax, eax
	je	SHORT $L75352

; 472  : 							if (::MoveFileEx(sItem, NULL, MOVEFILE_DELAY_UNTIL_REBOOT))

	mov	esi, esp
	push	4
	push	0
	lea	ecx, DWORD PTR _sItem$75340[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	DWORD PTR __imp__MoveFileExW@12
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L75353

; 474  : 								LogText(_T("FileMisc::DeleteFolderContents(%s) delayed!"), sItem);

	mov	ecx, DWORD PTR _sItem$75340[ebp]
	push	ecx
	push	OFFSET FLAT:$SG75354
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 8

; 475  : 								nResult = -1; // nDelayed

	mov	DWORD PTR _nResult$[ebp], -1

; 477  : 							else

	jmp	SHORT $L75355
$L75353:

; 479  : 								LogText(_T("FileMisc::DeleteFolderContents(%s) failed!"), sItem);

	mov	edx, DWORD PTR _sItem$75340[ebp]
	push	edx
	push	OFFSET FLAT:$SG75356
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 8

; 480  : 								nResult = 0; // failed

	mov	DWORD PTR _nResult$[ebp], 0
$L75355:

; 483  : 						else

	jmp	SHORT $L75357
$L75352:

; 484  : 							nResult = 0; // failed

	mov	DWORD PTR _nResult$[ebp], 0
$L75357:

; 486  : 
; 487  : 					// handle failure
; 488  : 					if (nResult == 0)

	cmp	DWORD PTR _nResult$[ebp], 0
	jne	SHORT $L75358

; 489  : 						break;

	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR _sItem$75340[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	SHORT $L75360
$L75358:

; 490  : 				}

	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR _sItem$75340[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75339:

; 491  : 
; 492  : 				bStopped = (WaitForSingleObject(hTerminate, 0) == WAIT_OBJECT_0);

	mov	esi, esp
	push	0
	mov	eax, DWORD PTR _hTerminate$[ebp]
	push	eax
	call	DWORD PTR __imp__WaitForSingleObject@8
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR _bStopped$[ebp], eax

; 494  : 			while (!bStopped && FindNextFile(hSearch, &finfo));

	cmp	DWORD PTR _bStopped$[ebp], 0
	jne	SHORT $L75360
	mov	esi, esp
	lea	ecx, DWORD PTR _finfo$75331[ebp]
	push	ecx
	mov	edx, DWORD PTR _hSearch$75332[ebp]
	push	edx
	call	DWORD PTR __imp__FindNextFileW@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	$L75335
$L75360:

; 495  : 			
; 496  : 			FindClose(hSearch);

	mov	esi, esp
	mov	eax, DWORD PTR _hSearch$75332[ebp]
	push	eax
	call	DWORD PTR __imp__FindClose@4
	cmp	esi, esp
	call	__chkesp
$L75334:

; 498  : 
; 499  : 		// handle failure
; 500  : 		if (nResult == 0)

	cmp	DWORD PTR _nResult$[ebp], 0
	jne	SHORT $L75361

; 501  : 			break;

	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sMask$75327[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sSearchSpec$75326[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	SHORT $L75325
$L75361:

; 502  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sMask$75327[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sSearchSpec$75326[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	$L75324
$L75325:

; 503  : 
; 504  : 	if (bStopped || (nResult == 0))

	cmp	DWORD PTR _bStopped$[ebp], 0
	jne	SHORT $L75363
	cmp	DWORD PTR _nResult$[ebp], 0
	jne	SHORT $L75362
$L75363:

; 505  : 		return 0;

	mov	DWORD PTR $T76919[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _aMasks$[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	mov	eax, DWORD PTR $T76919[ebp]
	jmp	SHORT $L75309
$L75362:

; 506  : 
; 507  : 	return nResult;

	mov	ecx, DWORD PTR _nResult$[ebp]
	mov	DWORD PTR $T76920[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _aMasks$[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	mov	eax, DWORD PTR $T76920[ebp]
$L75309:

; 508  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 716				; 000002ccH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76921:
	lea	ecx, DWORD PTR _aMasks$[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	ret	0
$L76922:
	lea	ecx, DWORD PTR $T76913[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76923:
	lea	ecx, DWORD PTR _sSearchSpec$75326[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76924:
	lea	ecx, DWORD PTR _sMask$75327[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76925:
	lea	ecx, DWORD PTR $T76918[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76926:
	lea	ecx, DWORD PTR _sItem$75340[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76929:
	mov	eax, OFFSET FLAT:$T76928
	jmp	___CxxFrameHandler
text$x	ENDS
?DeleteFolderContents@FileMisc@@YAHPBGK0PAX@Z ENDP	; FileMisc::DeleteFolderContents
;	COMDAT ?HasFlag@Misc@@YAHKK@Z
_TEXT	SEGMENT
_dwFlags$ = 8
_dwFlag$ = 12
?HasFlag@Misc@@YAHKK@Z PROC NEAR			; Misc::HasFlag, COMDAT

; 215  : 	inline BOOL HasFlag(DWORD dwFlags, DWORD dwFlag) { return (((dwFlags & dwFlag) == dwFlag) ? TRUE : FALSE); }

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR _dwFlags$[ebp]
	and	eax, DWORD PTR _dwFlag$[ebp]
	xor	ecx, ecx
	cmp	eax, DWORD PTR _dwFlag$[ebp]
	sete	cl
	mov	eax, ecx
	pop	ebp
	ret	0
?HasFlag@Misc@@YAHKK@Z ENDP				; Misc::HasFlag
_TEXT	ENDS
EXTRN	__imp__GetLastError@0:NEAR
EXTRN	__imp__RemoveDirectoryW@4:NEAR
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75375 DB	'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'M', 00H, 'i', 00H
	DB	's', 00H, 'c', 00H, ':', 00H, ':', 00H, 'D', 00H, 'e', 00H, 'l'
	DB	00H, 'e', 00H, 't', 00H, 'e', 00H, 'F', 00H, 'o', 00H, 'l', 00H
	DB	'd', 00H, 'e', 00H, 'r', 00H, '(', 00H, '%', 00H, 's', 00H, ')'
	DB	00H, ' ', 00H, 'd', 00H, 'e', 00H, 'l', 00H, 'a', 00H, 'y', 00H
	DB	'e', 00H, 'd', 00H, ' ', 00H, 'd', 00H, 'u', 00H, 'e', 00H, ' '
	DB	00H, 't', 00H, 'o', 00H, ' ', 00H, 'f', 00H, 'i', 00H, 'l', 00H
	DB	'e', 00H, 's', 00H, '!', 00H, 00H, 00H
	ORG $+2
$SG75377 DB	'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'M', 00H, 'i', 00H
	DB	's', 00H, 'c', 00H, ':', 00H, ':', 00H, 'D', 00H, 'e', 00H, 'l'
	DB	00H, 'e', 00H, 't', 00H, 'e', 00H, 'O', 00H, 'n', 00H, 'R', 00H
	DB	'e', 00H, 'b', 00H, 'o', 00H, 'o', 00H, 't', 00H, '(', 00H, '%'
	DB	00H, 's', 00H, ')', 00H, ' ', 00H, 'f', 00H, 'a', 00H, 'i', 00H
	DB	'l', 00H, 'e', 00H, 'd', 00H, '!', 00H, 00H, 00H
	ORG $+2
$SG75383 DB	'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'M', 00H, 'i', 00H
	DB	's', 00H, 'c', 00H, ':', 00H, ':', 00H, 'D', 00H, 'e', 00H, 'l'
	DB	00H, 'e', 00H, 't', 00H, 'e', 00H, 'F', 00H, 'o', 00H, 'l', 00H
	DB	'd', 00H, 'e', 00H, 'r', 00H, '(', 00H, '%', 00H, 's', 00H, ')'
	DB	00H, ' ', 00H, 'f', 00H, 'a', 00H, 'i', 00H, 'l', 00H, 'e', 00H
	DB	'd', 00H, '!', 00H, ' ', 00H, '(', 00H, 'd', 00H, 'w', 00H, ' '
	DB	00H, '=', 00H, ' ', 00H, '%', 00H, '0', 00H, '8', 00H, 'x', 00H
	DB	',', 00H, ' ', 00H, 'h', 00H, 'r', 00H, ' ', 00H, '=', 00H, ' '
	DB	00H, '%', 00H, '0', 00H, '8', 00H, 'x', 00H, 00H, 00H
	ORG $+2
$SG75386 DB	'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'M', 00H, 'i', 00H
	DB	's', 00H, 'c', 00H, ':', 00H, ':', 00H, 'D', 00H, 'e', 00H, 'l'
	DB	00H, 'e', 00H, 't', 00H, 'e', 00H, 'F', 00H, 'o', 00H, 'l', 00H
	DB	'd', 00H, 'e', 00H, 'r', 00H, '(', 00H, '%', 00H, 's', 00H, ')'
	DB	00H, ' ', 00H, 'd', 00H, 'e', 00H, 'l', 00H, 'a', 00H, 'y', 00H
	DB	'e', 00H, 'd', 00H, '!', 00H, 00H, 00H
$SG75388 DB	'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'M', 00H, 'i', 00H
	DB	's', 00H, 'c', 00H, ':', 00H, ':', 00H, 'D', 00H, 'e', 00H, 'l'
	DB	00H, 'e', 00H, 't', 00H, 'e', 00H, 'O', 00H, 'n', 00H, 'R', 00H
	DB	'e', 00H, 'b', 00H, 'o', 00H, 'o', 00H, 't', 00H, '(', 00H, '%'
	DB	00H, 's', 00H, ')', 00H, ' ', 00H, 'f', 00H, 'a', 00H, 'i', 00H
	DB	'l', 00H, 'e', 00H, 'd', 00H, '!', 00H, 00H, 00H
data	ENDS
;	COMDAT ?DeleteFolder@FileMisc@@YAHPBGKPAX@Z
_TEXT	SEGMENT
_szFolder$ = 8
_dwFlags$ = 12
_hTerminate$ = 16
_nResult$ = -4
_dwErr$75380 = -8
_hrErr$75381 = -12
?DeleteFolder@FileMisc@@YAHPBGKPAX@Z PROC NEAR		; FileMisc::DeleteFolder, COMDAT

; 511  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 512  : 	// if the dir does not exists just return
; 513  : 	if (!FolderExists(szFolder))

	mov	eax, DWORD PTR _szFolder$[ebp]
	push	eax
	call	?FolderExists@FileMisc@@YAHPBG@Z	; FileMisc::FolderExists
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75371

; 514  : 		return 1;

	mov	eax, 1
	jmp	$L75370
$L75371:

; 515  : 
; 516  : 	int nResult = DeleteFolderContents(szFolder, dwFlags, NULL, hTerminate);

	mov	ecx, DWORD PTR _hTerminate$[ebp]
	push	ecx
	push	0
	mov	edx, DWORD PTR _dwFlags$[ebp]
	push	edx
	mov	eax, DWORD PTR _szFolder$[ebp]
	push	eax
	call	?DeleteFolderContents@FileMisc@@YAHPBGK0PAX@Z ; FileMisc::DeleteFolderContents
	add	esp, 16					; 00000010H
	mov	DWORD PTR _nResult$[ebp], eax

; 517  : 
; 518  : 	// handle positive result
; 519  : 	if (nResult != 0)

	cmp	DWORD PTR _nResult$[ebp], 0
	je	$L75387

; 521  : 		// handle delayed deletion
; 522  : 		::SetFileAttributes(szFolder, FILE_ATTRIBUTE_NORMAL);

	mov	esi, esp
	push	128					; 00000080H
	mov	ecx, DWORD PTR _szFolder$[ebp]
	push	ecx
	call	DWORD PTR __imp__SetFileAttributesW@8
	cmp	esi, esp
	call	__chkesp

; 523  : 		
; 524  : 		if (nResult == -1) // some files can only be deleted on reboot

	cmp	DWORD PTR _nResult$[ebp], -1
	jne	SHORT $L75374

; 526  : 			LogText(_T("FileMisc::DeleteFolder(%s) delayed due to files!"), szFolder);

	mov	edx, DWORD PTR _szFolder$[ebp]
	push	edx
	push	OFFSET FLAT:$SG75375
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 8

; 527  : 
; 528  : 			if (!::MoveFileEx(szFolder, NULL, MOVEFILE_DELAY_UNTIL_REBOOT))

	mov	esi, esp
	push	4
	push	0
	mov	eax, DWORD PTR _szFolder$[ebp]
	push	eax
	call	DWORD PTR __imp__MoveFileExW@12
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75376

; 530  : 				LogText(_T("FileMisc::DeleteOnReboot(%s) failed!"), szFolder);

	mov	ecx, DWORD PTR _szFolder$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG75377
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 8

; 531  : 				nResult = 0; // failed

	mov	DWORD PTR _nResult$[ebp], 0
$L75376:

; 534  : 		// all contained files were deleted successfully
; 535  : 		// so we can now try to delete the folder itself
; 536  : 		else

	jmp	$L75387
$L75374:

; 538  : 			if (!RemoveDirectory(szFolder))

	mov	esi, esp
	mov	edx, DWORD PTR _szFolder$[ebp]
	push	edx
	call	DWORD PTR __imp__RemoveDirectoryW@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	$L75387

; 540  : 				DWORD dwErr = GetLastError();

	mov	esi, esp
	call	DWORD PTR __imp__GetLastError@0
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _dwErr$75380[ebp], eax

; 541  : 				HRESULT hrErr = HRESULT_FROM_WIN32(dwErr);

	cmp	DWORD PTR _dwErr$75380[ebp], 0
	je	SHORT $L76938
	mov	eax, DWORD PTR _dwErr$75380[ebp]
	and	eax, 65535				; 0000ffffH
	or	eax, 458752				; 00070000H
	or	eax, -2147483648			; 80000000H
	mov	DWORD PTR -16+[ebp], eax
	jmp	SHORT $L76939
$L76938:
	mov	DWORD PTR -16+[ebp], 0
$L76939:
	mov	ecx, DWORD PTR -16+[ebp]
	mov	DWORD PTR _hrErr$75381[ebp], ecx

; 542  : 				LogText(_T("FileMisc::DeleteFolder(%s) failed! (dw = %08x, hr = %08x"), szFolder, dwErr, hrErr);

	mov	edx, DWORD PTR _hrErr$75381[ebp]
	push	edx
	mov	eax, DWORD PTR _dwErr$75380[ebp]
	push	eax
	mov	ecx, DWORD PTR _szFolder$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG75383
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 16					; 00000010H

; 543  : 
; 544  : 				if (dwFlags & FMDF_ALLOWDELETEONREBOOT)

	mov	edx, DWORD PTR _dwFlags$[ebp]
	and	edx, 4
	test	edx, edx
	je	SHORT $L75387

; 546  : 					if (::MoveFileEx(szFolder, NULL, MOVEFILE_DELAY_UNTIL_REBOOT))

	mov	esi, esp
	push	4
	push	0
	mov	eax, DWORD PTR _szFolder$[ebp]
	push	eax
	call	DWORD PTR __imp__MoveFileExW@12
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L75385

; 548  : 						LogText(_T("FileMisc::DeleteFolder(%s) delayed!"), szFolder);

	mov	ecx, DWORD PTR _szFolder$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG75386
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 8

; 549  : 						nResult = -1; // nDelayed

	mov	DWORD PTR _nResult$[ebp], -1

; 551  : 					else

	jmp	SHORT $L75387
$L75385:

; 553  : 						LogText(_T("FileMisc::DeleteOnReboot(%s) failed!"), szFolder);

	mov	edx, DWORD PTR _szFolder$[ebp]
	push	edx
	push	OFFSET FLAT:$SG75388
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 8

; 554  : 						nResult = 0; // failed

	mov	DWORD PTR _nResult$[ebp], 0
$L75387:

; 560  : 
; 561  : 	return nResult;

	mov	eax, DWORD PTR _nResult$[ebp]
$L75370:

; 562  : }

	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?DeleteFolder@FileMisc@@YAHPBGKPAX@Z ENDP		; FileMisc::DeleteFolder
_TEXT	ENDS
PUBLIC	__real@8@00000000000000000000
PUBLIC	__real@8@401f8000000000000000
PUBLIC	?GetFolderSize@FileMisc@@YANPBGH0PAXH@Z		; FileMisc::GetFolderSize
EXTRN	__fltused:NEAR
;	COMDAT __real@8@00000000000000000000
; File D:\_CODE\Shared\FILEMISC.CPP
CONST	SEGMENT
__real@8@00000000000000000000 DQ 00000000000000000r ; 0
CONST	ENDS
;	COMDAT __real@8@401f8000000000000000
CONST	SEGMENT
__real@8@401f8000000000000000 DQ 041f0000000000000r ; 4.29497e+009
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76952	DD	019930520H
	DD	05H
	DD	FLAT:$T76959
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76959	DD	0ffffffffH
	DD	FLAT:$L76947
	DD	00H
	DD	FLAT:$L76948
	DD	01H
	DD	FLAT:$L76949
	DD	02H
	DD	FLAT:$L76950
	DD	01H
	DD	FLAT:$L76950
xdata$x	ENDS
;	COMDAT ?GetFolderSize@FileMisc@@YANPBGH0PAXH@Z
_TEXT	SEGMENT
$T76943 = -640
$T76944 = -648
__$EHRec$ = -12
_szFolder$ = 8
_bIncludeSubFolders$ = 12
_szFileMask$ = 16
_hTerminate$ = 20
_bProcessMsgLoop$ = 24
_dSize$ = -628
_finfo$ = -612
_sSearchSpec$ = -616
_sFileMask$ = -620
_bStopped$ = -16
_h$ = -20
_bFolder$75412 = -632
_sSubFolder$75414 = -636
?GetFolderSize@FileMisc@@YANPBGH0PAXH@Z PROC NEAR	; FileMisc::GetFolderSize, COMDAT

; 565  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76957
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 668				; 0000029cH
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-680]
	mov	ecx, 167				; 000000a7H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 566  : 	// if the dir does not exists just return
; 567  : 	if (!FolderExists(szFolder))

	mov	eax, DWORD PTR _szFolder$[ebp]
	push	eax
	call	?FolderExists@FileMisc@@YAHPBG@Z	; FileMisc::FolderExists
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75396

; 568  : 		return 0;

	fld	QWORD PTR __real@8@00000000000000000000
	jmp	$L75395
$L75396:

; 569  : 	
; 570  : 	double dSize = 0;

	mov	DWORD PTR _dSize$[ebp], 0
	mov	DWORD PTR _dSize$[ebp+4], 0

; 571  : 
; 572  : 	WIN32_FIND_DATA finfo;
; 573  : 	CString sSearchSpec(szFolder), sFileMask(szFileMask);

	mov	ecx, DWORD PTR _szFolder$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	edx, DWORD PTR _szFileMask$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sFileMask$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 574  : 
; 575  : 	if (sFileMask.IsEmpty())

	lea	ecx, DWORD PTR _sFileMask$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75401

; 576  : 		sFileMask = ALLFILESMASK;

	mov	eax, DWORD PTR _ALLFILESMASK
	push	eax
	lea	ecx, DWORD PTR _sFileMask$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L75401:

; 577  : 
; 578  : 	TerminatePath(sSearchSpec);

	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	push	ecx
	call	?TerminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ; FileMisc::TerminatePath
	add	esp, 4

; 579  : 	sSearchSpec += sFileMask;

	lea	edx, DWORD PTR _sFileMask$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=

; 580  : 
; 581  : 	BOOL bStopped = (WaitForSingleObject(hTerminate, 0) == WAIT_OBJECT_0);

	mov	esi, esp
	push	0
	mov	eax, DWORD PTR _hTerminate$[ebp]
	push	eax
	call	DWORD PTR __imp__WaitForSingleObject@8
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR _bStopped$[ebp], eax

; 582  : 	HANDLE h = NULL;

	mov	DWORD PTR _h$[ebp], 0

; 583  : 		
; 584  : 	if (!bStopped && (h = FindFirstFile(sSearchSpec, &finfo)) != INVALID_HANDLE_VALUE) 

	cmp	DWORD PTR _bStopped$[ebp], 0
	jne	$L75406
	mov	esi, esp
	lea	ecx, DWORD PTR _finfo$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	DWORD PTR __imp__FindFirstFileW@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _h$[ebp], eax
	cmp	DWORD PTR _h$[ebp], -1
	je	$L75406
$L75407:

; 588  : 			if (bProcessMsgLoop)

	cmp	DWORD PTR _bProcessMsgLoop$[ebp], 0
	je	SHORT $L75410

; 589  : 				Misc::ProcessMsgLoop();

	call	?ProcessMsgLoop@Misc@@YAXXZ		; Misc::ProcessMsgLoop
$L75410:

; 590  : 
; 591  : 			if (finfo.cFileName[0] != '.') 

	mov	edx, DWORD PTR _finfo$[ebp+44]
	and	edx, 65535				; 0000ffffH
	cmp	edx, 46					; 0000002eH
	je	$L75418

; 593  : 				BOOL bFolder = Misc::HasFlag(finfo.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY);

	push	16					; 00000010H
	mov	eax, DWORD PTR _finfo$[ebp]
	push	eax
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	mov	DWORD PTR _bFolder$75412[ebp], eax

; 594  : 
; 595  : 				if (bFolder && bIncludeSubFolders)

	cmp	DWORD PTR _bFolder$75412[ebp], 0
	je	$L75413
	cmp	DWORD PTR _bIncludeSubFolders$[ebp], 0
	je	$L75413

; 597  : 					CString sSubFolder = (TerminatePath(szFolder) + finfo.cFileName);

	lea	ecx, DWORD PTR _finfo$[ebp+44]
	push	ecx
	mov	edx, DWORD PTR _szFolder$[ebp]
	push	edx
	lea	eax, DWORD PTR $T76943[ebp]
	push	eax
	call	?TerminatePath@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::TerminatePath
	add	esp, 8
	mov	DWORD PTR -652+[ebp], eax
	mov	ecx, DWORD PTR -652+[ebp]
	mov	DWORD PTR -656+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	edx, DWORD PTR -656+[ebp]
	push	edx
	lea	eax, DWORD PTR _sSubFolder$75414[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR $T76943[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 598  : 					
; 599  : 					dSize += GetFolderSize(sSubFolder, TRUE, sFileMask, hTerminate, bProcessMsgLoop);

	mov	ecx, DWORD PTR _bProcessMsgLoop$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hTerminate$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sFileMask$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	1
	lea	ecx, DWORD PTR _sSubFolder$75414[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?GetFolderSize@FileMisc@@YANPBGH0PAXH@Z	; FileMisc::GetFolderSize
	add	esp, 20					; 00000014H
	fadd	QWORD PTR _dSize$[ebp]
	fstp	QWORD PTR _dSize$[ebp]

; 600  : 				}

	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sSubFolder$75414[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 601  : 				else if (!bFolder)

	jmp	SHORT $L75418
$L75413:
	cmp	DWORD PTR _bFolder$75412[ebp], 0
	jne	SHORT $L75418

; 603  : 					dSize += (finfo.nFileSizeHigh * ((double)MAXDWORD + 1)) + finfo.nFileSizeLow;

	mov	eax, DWORD PTR _finfo$[ebp+28]
	mov	DWORD PTR -664+[ebp], eax
	mov	DWORD PTR -664+[ebp+4], 0
	fild	QWORD PTR -664+[ebp]
	fmul	QWORD PTR __real@8@401f8000000000000000
	mov	ecx, DWORD PTR _finfo$[ebp+32]
	mov	DWORD PTR -672+[ebp], ecx
	mov	DWORD PTR -672+[ebp+4], 0
	fild	QWORD PTR -672+[ebp]
	faddp	ST(1), ST(0)
	fadd	QWORD PTR _dSize$[ebp]
	fstp	QWORD PTR _dSize$[ebp]
$L75418:

; 606  : 
; 607  : 			bStopped = (WaitForSingleObject(hTerminate, 0) == WAIT_OBJECT_0);

	mov	esi, esp
	push	0
	mov	edx, DWORD PTR _hTerminate$[ebp]
	push	edx
	call	DWORD PTR __imp__WaitForSingleObject@8
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR _bStopped$[ebp], eax

; 609  : 		while (!bStopped && FindNextFile(h, &finfo));

	cmp	DWORD PTR _bStopped$[ebp], 0
	jne	SHORT $L75421
	mov	esi, esp
	lea	eax, DWORD PTR _finfo$[ebp]
	push	eax
	mov	ecx, DWORD PTR _h$[ebp]
	push	ecx
	call	DWORD PTR __imp__FindNextFileW@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	$L75407
$L75421:

; 610  : 		
; 611  : 		FindClose(h);

	mov	esi, esp
	mov	edx, DWORD PTR _h$[ebp]
	push	edx
	call	DWORD PTR __imp__FindClose@4
	cmp	esi, esp
	call	__chkesp
$L75406:

; 613  : 
; 614  : 	return bStopped ? -1 : dSize;

	cmp	DWORD PTR _bStopped$[ebp], 0
	je	SHORT $L76945
	mov	DWORD PTR -680+[ebp], 0
	mov	DWORD PTR -680+[ebp+4], -1074790400	; bff00000H
	jmp	SHORT $L76946
$L76945:
	mov	eax, DWORD PTR _dSize$[ebp]
	mov	DWORD PTR -680+[ebp], eax
	mov	ecx, DWORD PTR _dSize$[ebp+4]
	mov	DWORD PTR -680+[ebp+4], ecx
$L76946:
	mov	edx, DWORD PTR -680+[ebp]
	mov	DWORD PTR $T76944[ebp], edx
	mov	eax, DWORD PTR -680+[ebp+4]
	mov	DWORD PTR $T76944[ebp+4], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sFileMask$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	fld	QWORD PTR $T76944[ebp]
$L75395:

; 615  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 680				; 000002a8H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76947:
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76948:
	lea	ecx, DWORD PTR _sFileMask$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76949:
	lea	ecx, DWORD PTR $T76943[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76950:
	lea	ecx, DWORD PTR _sSubFolder$75414[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76957:
	mov	eax, OFFSET FLAT:$T76952
	jmp	___CxxFrameHandler
text$x	ENDS
?GetFolderSize@FileMisc@@YANPBGH0PAXH@Z ENDP		; FileMisc::GetFolderSize
PUBLIC	?IsPath@FileMisc@@YAHPBG@Z			; FileMisc::IsPath
EXTRN	?TrimLeft@CString@@QAEXXZ:NEAR			; CString::TrimLeft
EXTRN	?Find@CString@@QBEHPBG@Z:NEAR			; CString::Find
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75428 DB	':', 00H, '\', 00H, 00H, 00H
	ORG $+2
$SG75429 DB	'\', 00H, '\', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76970	DD	019930520H
	DD	01H
	DD	FLAT:$T76972
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76972	DD	0ffffffffH
	DD	FLAT:$L76968
xdata$x	ENDS
;	COMDAT ?IsPath@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT
$T76964 = -20
__$EHRec$ = -12
_szPath$ = 8
_sPath$ = -16
?IsPath@FileMisc@@YAHPBG@Z PROC NEAR			; FileMisc::IsPath, COMDAT

; 618  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76971
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH

; 619  : 	CString sPath(szPath);

	mov	eax, DWORD PTR _szPath$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 620  : 	sPath.TrimLeft();

	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?TrimLeft@CString@@QAEXXZ		; CString::TrimLeft

; 621  : 	sPath.TrimRight();

	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?TrimRight@CString@@QAEXXZ		; CString::TrimRight

; 622  : 	
; 623  : 	return (!sPath.IsEmpty() && 
; 624  : 			(sPath.Find(_T(":\\")) == 1) || (sPath.Find(_T("\\\\")) == 0));

	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L76965
	push	OFFSET FLAT:$SG75428
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	cmp	eax, 1
	je	SHORT $L76966
$L76965:
	push	OFFSET FLAT:$SG75429
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	test	eax, eax
	je	SHORT $L76966
	mov	DWORD PTR -24+[ebp], 0
	jmp	SHORT $L76967
$L76966:
	mov	DWORD PTR -24+[ebp], 1
$L76967:
	mov	ecx, DWORD PTR -24+[ebp]
	mov	DWORD PTR $T76964[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76964[ebp]

; 625  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76968:
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76971:
	mov	eax, OFFSET FLAT:$T76970
	jmp	___CxxFrameHandler
text$x	ENDS
?IsPath@FileMisc@@YAHPBG@Z ENDP				; FileMisc::IsPath
PUBLIC	?PathExists@FileMisc@@YAHPBG@Z			; FileMisc::PathExists
EXTRN	__imp__GetFileAttributesW@4:NEAR
;	COMDAT ?PathExists@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT
_szPath$ = 8
?PathExists@FileMisc@@YAHPBG@Z PROC NEAR		; FileMisc::PathExists, COMDAT

; 628  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 629  : 	// special case
; 630  : 	if (!szPath || !*szPath) // cwd

	cmp	DWORD PTR _szPath$[ebp], 0
	je	SHORT $L75434
	mov	eax, DWORD PTR _szPath$[ebp]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax]
	test	ecx, ecx
	jne	SHORT $L75433
$L75434:

; 631  : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L75432
$L75433:

; 632  : 
; 633  : 	return (::GetFileAttributes(szPath) != 0xffffffff);

	mov	esi, esp
	mov	edx, DWORD PTR _szPath$[ebp]
	push	edx
	call	DWORD PTR __imp__GetFileAttributesW@4
	cmp	esi, esp
	call	__chkesp
	xor	ecx, ecx
	cmp	eax, -1
	setne	cl
	mov	eax, ecx
$L75432:

; 634  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?PathExists@FileMisc@@YAHPBG@Z ENDP			; FileMisc::PathExists
_TEXT	ENDS
EXTRN	__imp__lstrlenW@4:NEAR
;	COMDAT ?FolderExists@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT
_szFolder$ = 8
_dwAttrib$ = -4
?FolderExists@FileMisc@@YAHPBG@Z PROC NEAR		; FileMisc::FolderExists, COMDAT

; 637  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 638  : 	// special case
; 639  : 	if (!szFolder || !*szFolder) // cwd

	cmp	DWORD PTR _szFolder$[ebp], 0
	je	SHORT $L75439
	mov	eax, DWORD PTR _szFolder$[ebp]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax]
	test	ecx, ecx
	jne	SHORT $L75438
$L75439:

; 640  : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L75437
$L75438:

; 641  : 
; 642  : 	if (lstrlen(szFolder) > _MAX_PATH)

	mov	esi, esp
	mov	edx, DWORD PTR _szFolder$[ebp]
	push	edx
	call	DWORD PTR __imp__lstrlenW@4
	cmp	esi, esp
	call	__chkesp
	cmp	eax, 260				; 00000104H
	jle	SHORT $L75440

; 643  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75437
$L75440:

; 644  : 
; 645  : 	// else
; 646  : 	DWORD dwAttrib = GetFileAttributes(szFolder);

	mov	esi, esp
	mov	eax, DWORD PTR _szFolder$[ebp]
	push	eax
	call	DWORD PTR __imp__GetFileAttributesW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _dwAttrib$[ebp], eax

; 647  : 
; 648  : 	return (dwAttrib != 0xffffffff && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));

	cmp	DWORD PTR _dwAttrib$[ebp], -1
	je	SHORT $L76980
	mov	ecx, DWORD PTR _dwAttrib$[ebp]
	and	ecx, 16					; 00000010H
	test	ecx, ecx
	je	SHORT $L76980
	mov	DWORD PTR -8+[ebp], 1
	jmp	SHORT $L76981
$L76980:
	mov	DWORD PTR -8+[ebp], 0
$L76981:
	mov	eax, DWORD PTR -8+[ebp]
$L75437:

; 649  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?FolderExists@FileMisc@@YAHPBG@Z ENDP			; FileMisc::FolderExists
_TEXT	ENDS
;	COMDAT ?FileExists@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT
_szFile$ = 8
_dwAttrib$ = -4
?FileExists@FileMisc@@YAHPBG@Z PROC NEAR		; FileMisc::FileExists, COMDAT

; 652  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 653  : 	if (lstrlen(szFile) > _MAX_PATH)

	mov	esi, esp
	mov	eax, DWORD PTR _szFile$[ebp]
	push	eax
	call	DWORD PTR __imp__lstrlenW@4
	cmp	esi, esp
	call	__chkesp
	cmp	eax, 260				; 00000104H
	jle	SHORT $L75445

; 654  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75444
$L75445:

; 655  : 
; 656  : 	// else
; 657  : 	DWORD dwAttrib = GetFileAttributes(szFile);

	mov	esi, esp
	mov	ecx, DWORD PTR _szFile$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetFileAttributesW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _dwAttrib$[ebp], eax

; 658  : 
; 659  : 	return (dwAttrib != 0xffffffff && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) == 0);

	cmp	DWORD PTR _dwAttrib$[ebp], -1
	je	SHORT $L76984
	mov	edx, DWORD PTR _dwAttrib$[ebp]
	and	edx, 16					; 00000010H
	test	edx, edx
	jne	SHORT $L76984
	mov	DWORD PTR -8+[ebp], 1
	jmp	SHORT $L76985
$L76984:
	mov	DWORD PTR -8+[ebp], 0
$L76985:
	mov	eax, DWORD PTR -8+[ebp]
$L75444:

; 660  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?FileExists@FileMisc@@YAHPBG@Z ENDP			; FileMisc::FileExists
_TEXT	ENDS
PUBLIC	?FolderFromFilePathExists@FileMisc@@YAHPBG@Z	; FileMisc::FolderFromFilePathExists
PUBLIC	?GetFolderFromFilePath@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::GetFolderFromFilePath
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T76992	DD	019930520H
	DD	01H
	DD	FLAT:$T76994
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76994	DD	0ffffffffH
	DD	FLAT:$L76990
xdata$x	ENDS
;	COMDAT ?FolderFromFilePathExists@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT
$T76988 = -16
$T76989 = -20
__$EHRec$ = -12
_szFilePath$ = 8
?FolderFromFilePathExists@FileMisc@@YAHPBG@Z PROC NEAR	; FileMisc::FolderFromFilePathExists, COMDAT

; 663  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76993
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 664  : 	return FolderExists(GetFolderFromFilePath(szFilePath));

	mov	eax, DWORD PTR _szFilePath$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T76989[ebp]
	push	ecx
	call	?GetFolderFromFilePath@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::GetFolderFromFilePath
	add	esp, 8
	mov	DWORD PTR -24+[ebp], eax
	mov	edx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -28+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?FolderExists@FileMisc@@YAHPBG@Z	; FileMisc::FolderExists
	add	esp, 4
	mov	DWORD PTR $T76988[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76989[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76988[ebp]

; 665  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76990:
	lea	ecx, DWORD PTR $T76989[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76993:
	mov	eax, OFFSET FLAT:$T76992
	jmp	___CxxFrameHandler
text$x	ENDS
?FolderFromFilePathExists@FileMisc@@YAHPBG@Z ENDP	; FileMisc::FolderFromFilePathExists
PUBLIC	?GetCwd@FileMisc@@YA?AVCString@@XZ		; FileMisc::GetCwd
EXTRN	__imp__GetCurrentDirectoryW@8:NEAR
EXTRN	?GetBuffer@CString@@QAEPAGH@Z:NEAR		; CString::GetBuffer
EXTRN	?ReleaseBuffer@CString@@QAEXH@Z:NEAR		; CString::ReleaseBuffer
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77004	DD	019930520H
	DD	02H
	DD	FLAT:$T77006
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77006	DD	0ffffffffH
	DD	FLAT:$L77000
	DD	00H
	DD	FLAT:$L76999
xdata$x	ENDS
;	COMDAT ?GetCwd@FileMisc@@YA?AVCString@@XZ
_TEXT	SEGMENT
$T77001 = -20
__$EHRec$ = -12
___$ReturnUdt$ = 8
_sFolder$ = -16
?GetCwd@FileMisc@@YA?AVCString@@XZ PROC NEAR		; FileMisc::GetCwd, COMDAT

; 668  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77005
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T77001[ebp], 0

; 669  : 	CString sFolder;

	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 670  : 
; 671  : 	::GetCurrentDirectory(MAX_PATH, sFolder.GetBuffer(MAX_PATH+1));

	push	261					; 00000105H
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	mov	esi, esp
	push	eax
	push	260					; 00000104H
	call	DWORD PTR __imp__GetCurrentDirectoryW@8
	cmp	esi, esp
	call	__chkesp

; 672  : 	sFolder.ReleaseBuffer();

	push	-1
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 673  : 
; 674  : 	return sFolder;

	lea	eax, DWORD PTR _sFolder$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T77001[ebp]
	or	ecx, 1
	mov	DWORD PTR $T77001[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 675  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76999:
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77000:
	mov	eax, DWORD PTR $T77001[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77002
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77002:
	ret	0
$L77005:
	mov	eax, OFFSET FLAT:$T77004
	jmp	___CxxFrameHandler
text$x	ENDS
?GetCwd@FileMisc@@YA?AVCString@@XZ ENDP			; FileMisc::GetCwd
PUBLIC	?SetCwd@FileMisc@@YAXABVCString@@@Z		; FileMisc::SetCwd
EXTRN	__imp__SetCurrentDirectoryW@4:NEAR
;	COMDAT ?SetCwd@FileMisc@@YAXABVCString@@@Z
_TEXT	SEGMENT
_sCwd$ = 8
?SetCwd@FileMisc@@YAXABVCString@@@Z PROC NEAR		; FileMisc::SetCwd, COMDAT

; 678  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 679  : 	SetCurrentDirectory(sCwd);

	mov	ecx, DWORD PTR _sCwd$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__SetCurrentDirectoryW@4
	cmp	esi, esp
	call	__chkesp

; 680  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?SetCwd@FileMisc@@YAXABVCString@@@Z ENDP		; FileMisc::SetCwd
_TEXT	ENDS
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77020	DD	019930520H
	DD	04H
	DD	FLAT:$T77022
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77022	DD	0ffffffffH
	DD	FLAT:$L77016
	DD	00H
	DD	FLAT:$L77013
	DD	01H
	DD	FLAT:$L77014
	DD	02H
	DD	FLAT:$L77015
xdata$x	ENDS
;	COMDAT ?GetFolderFromFilePath@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT
$T77017 = -28
__$EHRec$ = -12
_szFilePath$ = 12
___$ReturnUdt$ = 8
_sFolder$ = -16
_sDrive$75466 = -20
_sDir$75467 = -24
?GetFolderFromFilePath@FileMisc@@YA?AVCString@@PBG@Z PROC NEAR ; FileMisc::GetFolderFromFilePath, COMDAT

; 683  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77021
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR $T77017[ebp], 0

; 684  : 	CString sFolder;

	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 685  : 
; 686  : 	// check if its a folder already
; 687  : 	if (FolderExists(szFilePath))

	mov	eax, DWORD PTR _szFilePath$[ebp]
	push	eax
	call	?FolderExists@FileMisc@@YAHPBG@Z	; FileMisc::FolderExists
	add	esp, 4
	test	eax, eax
	je	SHORT $L75464

; 689  : 		sFolder = szFilePath;

	mov	ecx, DWORD PTR _szFilePath$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 691  : 	else

	jmp	SHORT $L75465
$L75464:

; 693  : 		// remove file ending
; 694  : 		CString sDrive, sDir;

	lea	ecx, DWORD PTR _sDrive$75466[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sDir$75467[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3

; 695  : 
; 696  : 		SplitPath(szFilePath, &sDrive, &sDir);

	push	0
	push	0
	lea	edx, DWORD PTR _sDir$75467[ebp]
	push	edx
	lea	eax, DWORD PTR _sDrive$75466[ebp]
	push	eax
	mov	ecx, DWORD PTR _szFilePath$[ebp]
	push	ecx
	call	?SplitPath@FileMisc@@YAXPBGPAVCString@@111@Z ; FileMisc::SplitPath
	add	esp, 20					; 00000014H

; 697  : 		MakePath(sFolder, sDrive, sDir);

	push	0
	push	0
	lea	ecx, DWORD PTR _sDir$75467[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sDrive$75466[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	edx, DWORD PTR _sFolder$[ebp]
	push	edx
	call	?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z ; FileMisc::MakePath
	add	esp, 20					; 00000014H

; 698  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sDir$75467[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sDrive$75466[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75465:

; 699  : 
; 700  : 	return UnterminatePath(sFolder);

	lea	eax, DWORD PTR _sFolder$[ebp]
	push	eax
	call	?UnterminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ; FileMisc::UnterminatePath
	add	esp, 4
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T77017[ebp]
	or	ecx, 1
	mov	DWORD PTR $T77017[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 701  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77013:
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77014:
	lea	ecx, DWORD PTR _sDrive$75466[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77015:
	lea	ecx, DWORD PTR _sDir$75467[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77016:
	mov	eax, DWORD PTR $T77017[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77018
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77018:
	ret	0
$L77021:
	mov	eax, OFFSET FLAT:$T77020
	jmp	___CxxFrameHandler
text$x	ENDS
?GetFolderFromFilePath@FileMisc@@YA?AVCString@@PBG@Z ENDP ; FileMisc::GetFolderFromFilePath
PUBLIC	?CreateFolder@FileMisc@@YAHPBG@Z		; FileMisc::CreateFolder
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77031	DD	019930520H
	DD	01H
	DD	FLAT:$T77033
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77033	DD	0ffffffffH
	DD	FLAT:$L77029
xdata$x	ENDS
;	COMDAT ?CreateFolderFromFilePath@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT
$T77027 = -16
$T77028 = -20
__$EHRec$ = -12
_szFilePath$ = 8
?CreateFolderFromFilePath@FileMisc@@YAHPBG@Z PROC NEAR	; FileMisc::CreateFolderFromFilePath, COMDAT

; 704  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77032
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 705  : 	return CreateFolder(GetFolderFromFilePath(szFilePath));

	mov	eax, DWORD PTR _szFilePath$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77028[ebp]
	push	ecx
	call	?GetFolderFromFilePath@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::GetFolderFromFilePath
	add	esp, 8
	mov	DWORD PTR -24+[ebp], eax
	mov	edx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -28+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?CreateFolder@FileMisc@@YAHPBG@Z	; FileMisc::CreateFolder
	add	esp, 4
	mov	DWORD PTR $T77027[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T77028[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77027[ebp]

; 706  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77029:
	lea	ecx, DWORD PTR $T77028[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77032:
	mov	eax, OFFSET FLAT:$T77031
	jmp	___CxxFrameHandler
text$x	ENDS
?CreateFolderFromFilePath@FileMisc@@YAHPBG@Z ENDP	; FileMisc::CreateFolderFromFilePath
PUBLIC	?PathHasWildcard@FileMisc@@YAHPBG@Z		; FileMisc::PathHasWildcard
EXTRN	__imp__wcschr:NEAR
;	COMDAT ?PathHasWildcard@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT
_szFilePath$ = 8
?PathHasWildcard@FileMisc@@YAHPBG@Z PROC NEAR		; FileMisc::PathHasWildcard, COMDAT

; 709  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 710  : 	return (_tcschr(szFilePath, '?') || _tcschr(szFilePath, '*'));

	mov	esi, esp
	push	63					; 0000003fH
	mov	eax, DWORD PTR _szFilePath$[ebp]
	push	eax
	call	DWORD PTR __imp__wcschr
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L77038
	mov	esi, esp
	push	42					; 0000002aH
	mov	ecx, DWORD PTR _szFilePath$[ebp]
	push	ecx
	call	DWORD PTR __imp__wcschr
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L77038
	mov	DWORD PTR -4+[ebp], 0
	jmp	SHORT $L77039
$L77038:
	mov	DWORD PTR -4+[ebp], 1
$L77039:
	mov	eax, DWORD PTR -4+[ebp]

; 711  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?PathHasWildcard@FileMisc@@YAHPBG@Z ENDP		; FileMisc::PathHasWildcard
_TEXT	ENDS
PUBLIC	?IsFolderWritable@FileMisc@@YAHPBGH@Z		; FileMisc::IsFolderWritable
EXTRN	??0CFile@@QAE@XZ:NEAR				; CFile::CFile
EXTRN	??1CFile@@UAE@XZ:NEAR				; CFile::~CFile
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75483 DB	'9', 00H, '0', 00H, 'F', 00H, '9', 00H, '7', 00H, 'F', 00H
	DB	'1', 00H, '8', 00H, '3', 00H, '2', 00H, 'E', 00H, 'C', 00H, '4'
	DB	00H, 'B', 00H, '5', 00H, 'A', 00H, '8', 00H, '2', 00H, 'F', 00H
	DB	'F', 00H, '7', 00H, 'F', 00H, '7', 00H, '6', 00H, '9', 00H, '5'
	DB	00H, '3', 00H, 'E', 00H, 'D', 00H, 'E', 00H, '3', 00H, '7', 00H
	DB	00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77050	DD	019930520H
	DD	02H
	DD	FLAT:$T77052
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77052	DD	0ffffffffH
	DD	FLAT:$L77047
	DD	00H
	DD	FLAT:$L77048
xdata$x	ENDS
;	COMDAT ?IsFolderWritable@FileMisc@@YAHPBGH@Z
_TEXT	SEGMENT
$T77042 = -24
$T77043 = -40
$T77044 = -44
$T77045 = -48
$T77046 = -52
__$EHRec$ = -12
_szFolder$ = 8
_bAutoCreate$ = 12
_szUniqueName$ = -20
_sUniquePath$ = -16
?IsFolderWritable@FileMisc@@YAHPBGH@Z PROC NEAR		; FileMisc::IsFolderWritable, COMDAT

; 714  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77051
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 52					; 00000034H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-64]
	mov	ecx, 13					; 0000000dH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 715  : 	if (bAutoCreate)

	cmp	DWORD PTR _bAutoCreate$[ebp], 0
	je	SHORT $L75480

; 716  : 		CreateFolder(szFolder);

	mov	eax, DWORD PTR _szFolder$[ebp]
	push	eax
	call	?CreateFolder@FileMisc@@YAHPBG@Z	; FileMisc::CreateFolder
	add	esp, 4
$L75480:

; 717  : 
; 718  : 	if (!FolderExists(szFolder))

	mov	ecx, DWORD PTR _szFolder$[ebp]
	push	ecx
	call	?FolderExists@FileMisc@@YAHPBG@Z	; FileMisc::FolderExists
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75481

; 719  : 		return FALSE;

	xor	eax, eax
	jmp	$L75479
$L75481:

; 720  : 		
; 721  : 	// based on generated GUID = {90F97F18-32EC-4B5A-82FF-7F76953EDE37}
; 722  : 	LPCTSTR szUniqueName = _T("90F97F1832EC4B5A82FF7F76953EDE37");

	mov	DWORD PTR _szUniqueName$[ebp], OFFSET FLAT:$SG75483

; 723  : 
; 724  : 	// try to create a unique file in the folder
; 725  : 	CString sUniquePath;

	lea	ecx, DWORD PTR _sUniquePath$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 726  : 	MakePath(sUniquePath, NULL, szFolder, szUniqueName);

	push	0
	mov	edx, DWORD PTR _szUniqueName$[ebp]
	push	edx
	mov	eax, DWORD PTR _szFolder$[ebp]
	push	eax
	push	0
	lea	ecx, DWORD PTR _sUniquePath$[ebp]
	push	ecx
	call	?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z ; FileMisc::MakePath
	add	esp, 20					; 00000014H

; 727  : 
; 728  : 	if (!CFile().Open(sUniquePath, CFile::modeCreate))

	mov	esi, esp
	push	0
	push	4096					; 00001000H
	lea	ecx, DWORD PTR _sUniquePath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR $T77043[ebp]
	call	??0CFile@@QAE@XZ			; CFile::CFile
	mov	DWORD PTR -56+[ebp], eax
	mov	edx, DWORD PTR -56+[ebp]
	mov	DWORD PTR -60+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	eax, DWORD PTR -60+[ebp]
	mov	DWORD PTR -64+[ebp], eax
	mov	ecx, DWORD PTR -64+[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR -64+[ebp]
	call	DWORD PTR [edx+40]
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	BYTE PTR $T77042[ebp], al
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T77043[ebp]
	call	??1CFile@@UAE@XZ			; CFile::~CFile
	mov	eax, DWORD PTR $T77042[ebp]
	and	eax, 255				; 000000ffH
	test	eax, eax
	je	SHORT $L75486

; 729  : 		return FALSE;

	mov	DWORD PTR $T77044[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sUniquePath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77044[ebp]
	jmp	SHORT $L75479
$L75486:

; 730  : 
; 731  : 	if (FileExists(sUniquePath))

	lea	ecx, DWORD PTR _sUniquePath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?FileExists@FileMisc@@YAHPBG@Z		; FileMisc::FileExists
	add	esp, 4
	test	eax, eax
	je	SHORT $L75489

; 733  : 		DeleteFile(sUniquePath, TRUE);

	push	1
	lea	ecx, DWORD PTR _sUniquePath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?DeleteFileW@FileMisc@@YAHPBGH@Z	; FileMisc::DeleteFileW
	add	esp, 8

; 734  : 		return TRUE;

	mov	DWORD PTR $T77045[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sUniquePath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77045[ebp]
	jmp	SHORT $L75479
$L75489:

; 736  : 
; 737  : 	// else 
; 738  : 	return FALSE;

	mov	DWORD PTR $T77046[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sUniquePath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77046[ebp]
$L75479:

; 739  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 64					; 00000040H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77047:
	lea	ecx, DWORD PTR _sUniquePath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77048:
	lea	ecx, DWORD PTR $T77043[ebp]
	call	??1CFile@@UAE@XZ			; CFile::~CFile
	ret	0
$L77051:
	mov	eax, OFFSET FLAT:$T77050
	jmp	___CxxFrameHandler
text$x	ENDS
?IsFolderWritable@FileMisc@@YAHPBGH@Z ENDP		; FileMisc::IsFolderWritable
EXTRN	__imp__CreateDirectoryW@8:NEAR
EXTRN	?Find@CString@@QBEHGH@Z:NEAR			; CString::Find
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75501 DB	':', 00H, '\', 00H, 00H, 00H
	ORG $+2
$SG75506 DB	'\', 00H, '\', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77074	DD	019930520H
	DD	08H
	DD	FLAT:$T77076
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77076	DD	0ffffffffH
	DD	FLAT:$L77065
	DD	00H
	DD	FLAT:$L77066
	DD	01H
	DD	FLAT:$L77067
	DD	01H
	DD	FLAT:$L77068
	DD	01H
	DD	FLAT:$L77069
	DD	01H
	DD	FLAT:$L77070
	DD	01H
	DD	FLAT:$L77071
	DD	01H
	DD	FLAT:$L77072
xdata$x	ENDS
;	COMDAT ?CreateFolder@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT
$T77058 = -36
$T77059 = -40
$T77060 = -44
$T77061 = -48
$T77062 = -52
$T77063 = -56
$T77064 = -60
__$EHRec$ = -12
_szFolder$ = 8
_sFolder$ = -20
_sRemaining$ = -32
_bDone$ = -16
_bResult$ = -28
_nFind$ = -24
?CreateFolder@FileMisc@@YAHPBG@Z PROC NEAR		; FileMisc::CreateFolder, COMDAT

; 742  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77075
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 96					; 00000060H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-108]
	mov	ecx, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 743  : 	if (FolderExists(szFolder))

	mov	eax, DWORD PTR _szFolder$[ebp]
	push	eax
	call	?FolderExists@FileMisc@@YAHPBG@Z	; FileMisc::FolderExists
	add	esp, 4
	test	eax, eax
	je	SHORT $L75495

; 744  : 		return TRUE;

	mov	eax, 1
	jmp	$L75494
$L75495:

; 745  : 
; 746  : 	// start from the highest level folder working to the lowest
; 747  : 	CString sFolder, sRemaining(szFolder);

	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _szFolder$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sRemaining$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 748  : 	UnterminatePath(sRemaining);

	lea	edx, DWORD PTR _sRemaining$[ebp]
	push	edx
	call	?UnterminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ; FileMisc::UnterminatePath
	add	esp, 4

; 749  : 
; 750  : 	BOOL bDone = FALSE;

	mov	DWORD PTR _bDone$[ebp], 0

; 751  : 	BOOL bResult = TRUE;

	mov	DWORD PTR _bResult$[ebp], 1

; 752  : 
; 753  : 	// pull off the :\ or \\ start
; 754  : 	int nFind = sRemaining.Find(_T(":\\"));

	push	OFFSET FLAT:$SG75501
	lea	ecx, DWORD PTR _sRemaining$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	mov	DWORD PTR _nFind$[ebp], eax

; 755  : 
; 756  : 	if (nFind != -1)

	cmp	DWORD PTR _nFind$[ebp], -1
	je	SHORT $L75502

; 758  : 		sFolder += sRemaining.Left(nFind + 2);

	mov	eax, DWORD PTR _nFind$[ebp]
	add	eax, 2
	push	eax
	lea	ecx, DWORD PTR $T77058[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sRemaining$[ebp]
	call	?Left@CString@@QBE?AV1@H@Z		; CString::Left
	mov	DWORD PTR -64+[ebp], eax
	mov	edx, DWORD PTR -64+[ebp]
	mov	DWORD PTR -68+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	eax, DWORD PTR -68+[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T77058[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 759  : 		sRemaining = sRemaining.Mid(nFind + 2);

	mov	ecx, DWORD PTR _nFind$[ebp]
	add	ecx, 2
	push	ecx
	lea	edx, DWORD PTR $T77059[ebp]
	push	edx
	lea	ecx, DWORD PTR _sRemaining$[ebp]
	call	?Mid@CString@@QBE?AV1@H@Z		; CString::Mid
	mov	DWORD PTR -72+[ebp], eax
	mov	eax, DWORD PTR -72+[ebp]
	mov	DWORD PTR -76+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR -76+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sRemaining$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T77059[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 761  : 	else

	jmp	$L75507
$L75502:

; 763  : 		nFind = sRemaining.Find(_T("\\\\"));

	push	OFFSET FLAT:$SG75506
	lea	ecx, DWORD PTR _sRemaining$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	mov	DWORD PTR _nFind$[ebp], eax

; 764  : 		
; 765  : 		if (nFind != -1)

	cmp	DWORD PTR _nFind$[ebp], -1
	je	SHORT $L75507

; 767  : 			sFolder += sRemaining.Left(nFind + 2);

	mov	edx, DWORD PTR _nFind$[ebp]
	add	edx, 2
	push	edx
	lea	eax, DWORD PTR $T77060[ebp]
	push	eax
	lea	ecx, DWORD PTR _sRemaining$[ebp]
	call	?Left@CString@@QBE?AV1@H@Z		; CString::Left
	mov	DWORD PTR -80+[ebp], eax
	mov	ecx, DWORD PTR -80+[ebp]
	mov	DWORD PTR -84+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	edx, DWORD PTR -84+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T77060[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 768  : 			sRemaining = sRemaining.Mid(nFind + 2);

	mov	eax, DWORD PTR _nFind$[ebp]
	add	eax, 2
	push	eax
	lea	ecx, DWORD PTR $T77061[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sRemaining$[ebp]
	call	?Mid@CString@@QBE?AV1@H@Z		; CString::Mid
	mov	DWORD PTR -88+[ebp], eax
	mov	edx, DWORD PTR -88+[ebp]
	mov	DWORD PTR -92+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	mov	eax, DWORD PTR -92+[ebp]
	push	eax
	lea	ecx, DWORD PTR _sRemaining$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T77061[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75507:

; 771  : 
; 772  : 	while (!bDone && bResult)

	cmp	DWORD PTR _bDone$[ebp], 0
	jne	$L75512
	cmp	DWORD PTR _bResult$[ebp], 0
	je	$L75512

; 774  : 		nFind = sRemaining.Find('\\', 1);

	push	1
	push	92					; 0000005cH
	lea	ecx, DWORD PTR _sRemaining$[ebp]
	call	?Find@CString@@QBEHGH@Z			; CString::Find
	mov	DWORD PTR _nFind$[ebp], eax

; 775  : 
; 776  : 		if (nFind == -1)

	cmp	DWORD PTR _nFind$[ebp], -1
	jne	SHORT $L75513

; 778  : 			bDone = TRUE;

	mov	DWORD PTR _bDone$[ebp], 1

; 779  : 			sFolder += sRemaining;

	lea	ecx, DWORD PTR _sRemaining$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=

; 781  : 		else

	jmp	SHORT $L75514
$L75513:

; 783  : 			sFolder += sRemaining.Left(nFind);

	mov	edx, DWORD PTR _nFind$[ebp]
	push	edx
	lea	eax, DWORD PTR $T77062[ebp]
	push	eax
	lea	ecx, DWORD PTR _sRemaining$[ebp]
	call	?Left@CString@@QBE?AV1@H@Z		; CString::Left
	mov	DWORD PTR -96+[ebp], eax
	mov	ecx, DWORD PTR -96+[ebp]
	mov	DWORD PTR -100+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	mov	edx, DWORD PTR -100+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T77062[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 784  : 			sRemaining = sRemaining.Mid(nFind);

	mov	eax, DWORD PTR _nFind$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77063[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sRemaining$[ebp]
	call	?Mid@CString@@QBE?AV1@H@Z		; CString::Mid
	mov	DWORD PTR -104+[ebp], eax
	mov	edx, DWORD PTR -104+[ebp]
	mov	DWORD PTR -108+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	mov	eax, DWORD PTR -108+[ebp]
	push	eax
	lea	ecx, DWORD PTR _sRemaining$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T77063[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75514:

; 786  : 
; 787  : 		// if the folder doesn't exist we try to create it
; 788  : 		if (!FolderExists(sFolder) && (::CreateDirectory(sFolder, NULL) == FALSE))

	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?FolderExists@FileMisc@@YAHPBG@Z	; FileMisc::FolderExists
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75517
	mov	esi, esp
	push	0
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	DWORD PTR __imp__CreateDirectoryW@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75517

; 789  : 			bResult = FALSE;

	mov	DWORD PTR _bResult$[ebp], 0
$L75517:

; 790  : 	}

	jmp	$L75507
$L75512:

; 791  : 
; 792  : 	return bResult;

	mov	ecx, DWORD PTR _bResult$[ebp]
	mov	DWORD PTR $T77064[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sRemaining$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77064[ebp]
$L75494:

; 793  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 108				; 0000006cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77065:
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77066:
	lea	ecx, DWORD PTR _sRemaining$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77067:
	lea	ecx, DWORD PTR $T77058[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77068:
	lea	ecx, DWORD PTR $T77059[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77069:
	lea	ecx, DWORD PTR $T77060[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77070:
	lea	ecx, DWORD PTR $T77061[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77071:
	lea	ecx, DWORD PTR $T77062[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77072:
	lea	ecx, DWORD PTR $T77063[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77075:
	mov	eax, OFFSET FLAT:$T77074
	jmp	___CxxFrameHandler
text$x	ENDS
?CreateFolder@FileMisc@@YAHPBG@Z ENDP			; FileMisc::CreateFolder
PUBLIC	?FolderContainsFiles@FileMisc@@YAHPBGH0@Z	; FileMisc::FolderContainsFiles
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77091	DD	019930520H
	DD	03H
	DD	FLAT:$T77093
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77093	DD	0ffffffffH
	DD	FLAT:$L77087
	DD	00H
	DD	FLAT:$L77088
	DD	01H
	DD	FLAT:$L77089
xdata$x	ENDS
;	COMDAT ?FolderContainsFiles@FileMisc@@YAHPBGH0@Z
_TEXT	SEGMENT
$T77082 = -52
$T77083 = -56
$T77084 = -60
$T77085 = -64
$T77086 = -68
__$EHRec$ = -12
_szFolder$ = 8
_bCheckSubFolders$ = 12
_szFilter$ = 16
_ff$ = -44
_sSearchSpec$ = -48
_bContinue$ = -16
?FolderContainsFiles@FileMisc@@YAHPBGH0@Z PROC NEAR	; FileMisc::FolderContainsFiles, COMDAT

; 796  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77092
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 64					; 00000040H
	push	edi
	lea	edi, DWORD PTR [ebp-76]
	mov	ecx, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 797  : 	CFileFind ff;

	lea	ecx, DWORD PTR _ff$[ebp]
	call	??0CFileFind@@QAE@XZ			; CFileFind::CFileFind
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 798  : 	CString sSearchSpec;

	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 799  : 	
; 800  : 	MakePath(sSearchSpec, NULL, szFolder, szFilter, NULL);

	push	0
	mov	eax, DWORD PTR _szFilter$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szFolder$[ebp]
	push	ecx
	push	0
	lea	edx, DWORD PTR _sSearchSpec$[ebp]
	push	edx
	call	?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z ; FileMisc::MakePath
	add	esp, 20					; 00000014H

; 801  : 	
; 802  : 	BOOL bContinue = ff.FindFile(sSearchSpec);

	push	0
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _ff$[ebp]
	call	?FindFile@CFileFind@@UAEHPBGK@Z		; CFileFind::FindFile
	mov	DWORD PTR _bContinue$[ebp], eax
$L75528:

; 803  : 	
; 804  : 	while (bContinue)

	cmp	DWORD PTR _bContinue$[ebp], 0
	je	$L75529

; 806  : 		bContinue = ff.FindNextFile();

	lea	ecx, DWORD PTR _ff$[ebp]
	call	?FindNextFileW@CFileFind@@UAEHXZ	; CFileFind::FindNextFileW
	mov	DWORD PTR _bContinue$[ebp], eax

; 807  : 		
; 808  : 		if (!ff.IsDots())

	lea	ecx, DWORD PTR _ff$[ebp]
	call	?IsDots@CFileFind@@UBEHXZ		; CFileFind::IsDots
	test	eax, eax
	jne	$L75537

; 810  : 			if (ff.IsDirectory())

	lea	ecx, DWORD PTR _ff$[ebp]
	call	?IsDirectory@CFileFind@@QBEHXZ		; CFileFind::IsDirectory
	test	eax, eax
	je	SHORT $L75531

; 812  : 				if (bCheckSubFolders)

	cmp	DWORD PTR _bCheckSubFolders$[ebp], 0
	je	SHORT $L75534

; 814  : 					if (FolderContainsFiles(ff.GetFilePath(), TRUE, szFilter))

	mov	eax, DWORD PTR _szFilter$[ebp]
	push	eax
	push	1
	lea	ecx, DWORD PTR $T77083[ebp]
	push	ecx
	lea	ecx, DWORD PTR _ff$[ebp]
	call	?GetFilePath@CFileFind@@UBE?AVCString@@XZ ; CFileFind::GetFilePath
	mov	DWORD PTR -72+[ebp], eax
	mov	edx, DWORD PTR -72+[ebp]
	mov	DWORD PTR -76+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR -76+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?FolderContainsFiles@FileMisc@@YAHPBGH0@Z ; FileMisc::FolderContainsFiles
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77082[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T77083[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	DWORD PTR $T77082[ebp], 0
	je	SHORT $L75534

; 815  : 						return TRUE;

	mov	DWORD PTR $T77084[ebp], 1
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _ff$[ebp]
	call	??1CFileFind@@UAE@XZ			; CFileFind::~CFileFind
	mov	eax, DWORD PTR $T77084[ebp]
	jmp	SHORT $L75523
$L75534:

; 818  : 			else // is file

	jmp	SHORT $L75537
$L75531:

; 820  : 				return TRUE;

	mov	DWORD PTR $T77085[ebp], 1
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _ff$[ebp]
	call	??1CFileFind@@UAE@XZ			; CFileFind::~CFileFind
	mov	eax, DWORD PTR $T77085[ebp]
	jmp	SHORT $L75523
$L75537:

; 823  : 	}

	jmp	$L75528
$L75529:

; 824  : 	
; 825  : 	return FALSE;

	mov	DWORD PTR $T77086[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _ff$[ebp]
	call	??1CFileFind@@UAE@XZ			; CFileFind::~CFileFind
	mov	eax, DWORD PTR $T77086[ebp]
$L75523:

; 826  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 76					; 0000004cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77087:
	lea	ecx, DWORD PTR _ff$[ebp]
	call	??1CFileFind@@UAE@XZ			; CFileFind::~CFileFind
	ret	0
$L77088:
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77089:
	lea	ecx, DWORD PTR $T77083[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77092:
	mov	eax, OFFSET FLAT:$T77091
	jmp	___CxxFrameHandler
text$x	ENDS
?FolderContainsFiles@FileMisc@@YAHPBGH0@Z ENDP		; FileMisc::FolderContainsFiles
PUBLIC	?MoveFolder@FileMisc@@YAHPBG00KPAX@Z		; FileMisc::MoveFolder
PUBLIC	?CopyFolder@FileMisc@@YAHPBG00KPAX@Z		; FileMisc::CopyFolder
;	COMDAT ?MoveFolder@FileMisc@@YAHPBG00KPAX@Z
_TEXT	SEGMENT
_szSrcFolder$ = 8
_szDestFolder$ = 12
_szFileMask$ = 16
_dwFlags$ = 20
_hTerminate$ = 24
?MoveFolder@FileMisc@@YAHPBG00KPAX@Z PROC NEAR		; FileMisc::MoveFolder, COMDAT

; 830  : {

	push	ebp
	mov	ebp, esp

; 831  : 	if (CopyFolder(szSrcFolder, szDestFolder, szFileMask, dwFlags, hTerminate))

	mov	eax, DWORD PTR _hTerminate$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwFlags$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szFileMask$[ebp]
	push	edx
	mov	eax, DWORD PTR _szDestFolder$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szSrcFolder$[ebp]
	push	ecx
	call	?CopyFolder@FileMisc@@YAHPBG00KPAX@Z	; FileMisc::CopyFolder
	add	esp, 20					; 00000014H
	test	eax, eax
	je	SHORT $L75547

; 833  : 		// don't pass on hTerminate to ensure the operation completes
; 834  : 		return DeleteFolderContents(szSrcFolder, dwFlags, szFileMask, NULL);

	push	0
	mov	edx, DWORD PTR _szFileMask$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwFlags$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szSrcFolder$[ebp]
	push	ecx
	call	?DeleteFolderContents@FileMisc@@YAHPBGK0PAX@Z ; FileMisc::DeleteFolderContents
	add	esp, 16					; 00000010H
	jmp	SHORT $L75546
$L75547:

; 836  : 
; 837  : 	return FALSE;

	xor	eax, eax
$L75546:

; 838  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?MoveFolder@FileMisc@@YAHPBG00KPAX@Z ENDP		; FileMisc::MoveFolder
_TEXT	ENDS
PUBLIC	?CopyFileW@FileMisc@@YAHPBG0HHH@Z		; FileMisc::CopyFileW
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77119	DD	019930520H
	DD	0aH
	DD	FLAT:$T77121
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77121	DD	0ffffffffH
	DD	FLAT:$L77110
	DD	00H
	DD	FLAT:$L77111
	DD	00H
	DD	FLAT:$L77112
	DD	02H
	DD	FLAT:$L77113
	DD	03H
	DD	FLAT:$L77114
	DD	04H
	DD	FLAT:$L77115
	DD	03H
	DD	FLAT:$L77115
	DD	06H
	DD	FLAT:$L77116
	DD	07H
	DD	FLAT:$L77117
	DD	06H
	DD	FLAT:$L77117
xdata$x	ENDS
;	COMDAT ?CopyFolder@FileMisc@@YAHPBG00KPAX@Z
_TEXT	SEGMENT
$T77100 = -680
$T77105 = -684
$T77106 = -688
$T77107 = -692
__$EHRec$ = -12
_szSrcFolder$ = 8
_szDestFolder$ = 12
_szFileMask$ = 16
_dwFlags$ = 20
_hTerminate$ = 24
_bSubFolders$ = -60
_bMsgLoop$ = -48
_aMasks$ = -40
_nNumMasks$ = -20
_nNumPass$ = -16
_bResult$ = -56
_bStopped$ = -44
_nPass$ = -52
_sSearchSpec$75570 = -664
_sMask$75571 = -64
_finfo$75575 = -656
_hSearch$75576 = -660
_sSource$75584 = -672
_sDest$75587 = -668
_bFolder$75590 = -676
?CopyFolder@FileMisc@@YAHPBG00KPAX@Z PROC NEAR		; FileMisc::CopyFolder, COMDAT

; 842  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77120
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 708				; 000002c4H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-720]
	mov	ecx, 177				; 000000b1H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 843  : 	if (!CreateFolder(szDestFolder))

	mov	eax, DWORD PTR _szDestFolder$[ebp]
	push	eax
	call	?CreateFolder@FileMisc@@YAHPBG@Z	; FileMisc::CreateFolder
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75555

; 844  : 		return FALSE;

	xor	eax, eax
	jmp	$L75554
$L75555:

; 845  : 
; 846  : 	if (!FolderExists(szSrcFolder))

	mov	ecx, DWORD PTR _szSrcFolder$[ebp]
	push	ecx
	call	?FolderExists@FileMisc@@YAHPBG@Z	; FileMisc::FolderExists
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75556

; 847  : 		return FALSE;

	xor	eax, eax
	jmp	$L75554
$L75556:

; 848  : 
; 849  : 	BOOL bSubFolders = Misc::HasFlag(dwFlags, FMDF_SUBFOLDERS);

	push	1
	mov	edx, DWORD PTR _dwFlags$[ebp]
	push	edx
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	mov	DWORD PTR _bSubFolders$[ebp], eax

; 850  : 	BOOL bMsgLoop = Misc::HasFlag(dwFlags, FMDF_PROCESSMSGLOOP);

	push	2
	mov	eax, DWORD PTR _dwFlags$[ebp]
	push	eax
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	mov	DWORD PTR _bMsgLoop$[ebp], eax

; 851  : 
; 852  : 	// if a file mask has been specified with subfolders 
; 853  : 	// we need to do multiple passes on each folder, 
; 854  : 	// one (or more) for the files and one for the sub folders
; 855  : 	CStringArray aMasks;

	lea	ecx, DWORD PTR _aMasks$[ebp]
	call	??0CStringArray@@QAE@XZ			; CStringArray::CStringArray
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 856  : 	int nNumMasks = Misc::Split(szFileMask, aMasks, ';');

	mov	ecx, DWORD PTR _szFileMask$[ebp]
	push	ecx
	lea	ecx, DWORD PTR $T77100[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	push	0
	push	59					; 0000003bH
	lea	edx, DWORD PTR _aMasks$[ebp]
	push	edx
	lea	eax, DWORD PTR $T77100[ebp]
	push	eax
	call	?Split@Misc@@YAHABVCString@@AAVCStringArray@@GH@Z ; Misc::Split
	add	esp, 16					; 00000010H
	mov	DWORD PTR _nNumMasks$[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T77100[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 857  : 	
; 858  : 	int nNumPass = (bSubFolders ? (nNumMasks + 1) : max(nNumMasks, 1));

	cmp	DWORD PTR _bSubFolders$[ebp], 0
	je	SHORT $L77103
	mov	ecx, DWORD PTR _nNumMasks$[ebp]
	add	ecx, 1
	mov	DWORD PTR -696+[ebp], ecx
	jmp	SHORT $L77104
$L77103:
	cmp	DWORD PTR _nNumMasks$[ebp], 1
	jle	SHORT $L77101
	mov	edx, DWORD PTR _nNumMasks$[ebp]
	mov	DWORD PTR -700+[ebp], edx
	jmp	SHORT $L77102
$L77101:
	mov	DWORD PTR -700+[ebp], 1
$L77102:
	mov	eax, DWORD PTR -700+[ebp]
	mov	DWORD PTR -696+[ebp], eax
$L77104:
	mov	ecx, DWORD PTR -696+[ebp]
	mov	DWORD PTR _nNumPass$[ebp], ecx

; 859  : 
; 860  : 	BOOL bResult = TRUE;

	mov	DWORD PTR _bResult$[ebp], 1

; 861  : 	BOOL bStopped = (WaitForSingleObject(hTerminate, 0) == WAIT_OBJECT_0);

	mov	esi, esp
	push	0
	mov	edx, DWORD PTR _hTerminate$[ebp]
	push	edx
	call	DWORD PTR __imp__WaitForSingleObject@8
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR _bStopped$[ebp], eax

; 862  : 
; 863  : 	for (int nPass = 0; !bStopped && nPass < nNumPass; nPass++)

	mov	DWORD PTR _nPass$[ebp], 0
	jmp	SHORT $L75567
$L75568:
	mov	eax, DWORD PTR _nPass$[ebp]
	add	eax, 1
	mov	DWORD PTR _nPass$[ebp], eax
$L75567:
	cmp	DWORD PTR _bStopped$[ebp], 0
	jne	$L75569
	mov	ecx, DWORD PTR _nPass$[ebp]
	cmp	ecx, DWORD PTR _nNumPass$[ebp]
	jge	$L75569

; 865  : 		CString sSearchSpec(szSrcFolder), sMask(szFileMask);

	mov	edx, DWORD PTR _szSrcFolder$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sSearchSpec$75570[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	eax, DWORD PTR _szFileMask$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sMask$75571[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3

; 866  : 
; 867  : 		if (sMask.IsEmpty() || (nPass == aMasks.GetSize())) // (nPass == last) == 2nd pass (for folders)

	lea	ecx, DWORD PTR _sMask$75571[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75573
	lea	ecx, DWORD PTR _aMasks$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	cmp	DWORD PTR _nPass$[ebp], eax
	jne	SHORT $L75572
$L75573:

; 868  : 			sMask = ALLFILESMASK;

	mov	ecx, DWORD PTR _ALLFILESMASK
	push	ecx
	lea	ecx, DWORD PTR _sMask$75571[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 869  : 		else

	jmp	SHORT $L75574
$L75572:

; 870  : 			sMask = aMasks[nPass];

	mov	edx, DWORD PTR _nPass$[ebp]
	push	edx
	lea	ecx, DWORD PTR _aMasks$[ebp]
	call	??ACStringArray@@QAEAAVCString@@H@Z	; CStringArray::operator[]
	push	eax
	lea	ecx, DWORD PTR _sMask$75571[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
$L75574:

; 871  : 
; 872  : 		TerminatePath(sSearchSpec);

	lea	eax, DWORD PTR _sSearchSpec$75570[ebp]
	push	eax
	call	?TerminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ; FileMisc::TerminatePath
	add	esp, 4

; 873  : 		sSearchSpec += sMask;

	lea	ecx, DWORD PTR _sMask$75571[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sSearchSpec$75570[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=

; 874  : 
; 875  : 		WIN32_FIND_DATA finfo;
; 876  : 		HANDLE hSearch = NULL;

	mov	DWORD PTR _hSearch$75576[ebp], 0

; 877  : 
; 878  : 		if ((hSearch = FindFirstFile(sSearchSpec, &finfo)) != INVALID_HANDLE_VALUE) 

	mov	esi, esp
	lea	edx, DWORD PTR _finfo$75575[ebp]
	push	edx
	lea	ecx, DWORD PTR _sSearchSpec$75570[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	DWORD PTR __imp__FindFirstFileW@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hSearch$75576[ebp], eax
	cmp	DWORD PTR _hSearch$75576[ebp], -1
	je	$L75578
$L75579:

; 882  : 				if (bMsgLoop)

	cmp	DWORD PTR _bMsgLoop$[ebp], 0
	je	SHORT $L75582

; 883  : 					Misc::ProcessMsgLoop();

	call	?ProcessMsgLoop@Misc@@YAXXZ		; Misc::ProcessMsgLoop
$L75582:

; 884  : 
; 885  : 				if (finfo.cFileName[0] != '.') 

	mov	eax, DWORD PTR _finfo$75575[ebp+44]
	and	eax, 65535				; 0000ffffH
	cmp	eax, 46					; 0000002eH
	je	$L75583

; 887  : 					CString sSource = (TerminatePath(szSrcFolder) + finfo.cFileName);

	lea	ecx, DWORD PTR _finfo$75575[ebp+44]
	push	ecx
	mov	edx, DWORD PTR _szSrcFolder$[ebp]
	push	edx
	lea	eax, DWORD PTR $T77105[ebp]
	push	eax
	call	?TerminatePath@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::TerminatePath
	add	esp, 8
	mov	DWORD PTR -704+[ebp], eax
	mov	ecx, DWORD PTR -704+[ebp]
	mov	DWORD PTR -708+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	edx, DWORD PTR -708+[ebp]
	push	edx
	lea	eax, DWORD PTR _sSource$75584[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR $T77105[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 888  : 					CString sDest = (TerminatePath(szDestFolder) + finfo.cFileName);

	lea	ecx, DWORD PTR _finfo$75575[ebp+44]
	push	ecx
	mov	edx, DWORD PTR _szDestFolder$[ebp]
	push	edx
	lea	eax, DWORD PTR $T77106[ebp]
	push	eax
	call	?TerminatePath@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::TerminatePath
	add	esp, 8
	mov	DWORD PTR -712+[ebp], eax
	mov	ecx, DWORD PTR -712+[ebp]
	mov	DWORD PTR -716+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	mov	edx, DWORD PTR -716+[ebp]
	push	edx
	lea	eax, DWORD PTR _sDest$75587[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	BYTE PTR __$EHRec$[ebp+8], 9
	lea	ecx, DWORD PTR $T77106[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 889  : 					
; 890  : 					// process directories
; 891  : 					BOOL bFolder = Misc::HasFlag(finfo.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY);

	push	16					; 00000010H
	mov	ecx, DWORD PTR _finfo$75575[ebp]
	push	ecx
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	mov	DWORD PTR _bFolder$75590[ebp], eax

; 892  : 
; 893  : 					if (bSubFolders && bFolder)

	cmp	DWORD PTR _bSubFolders$[ebp], 0
	je	SHORT $L75591
	cmp	DWORD PTR _bFolder$75590[ebp], 0
	je	SHORT $L75591

; 895  : 						bResult = CopyFolder(sSource, sDest, szFileMask, dwFlags, hTerminate);

	mov	edx, DWORD PTR _hTerminate$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwFlags$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szFileMask$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sDest$75587[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sSource$75584[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?CopyFolder@FileMisc@@YAHPBG00KPAX@Z	; FileMisc::CopyFolder
	add	esp, 20					; 00000014H
	mov	DWORD PTR _bResult$[ebp], eax

; 897  : 					else if (!bFolder) // files 

	jmp	SHORT $L75593
$L75591:
	cmp	DWORD PTR _bFolder$75590[ebp], 0
	jne	SHORT $L75593

; 899  : 						bResult = CopyFile(sSource, sDest, 
; 900  : 											Misc::HasFlag(dwFlags, FMDF_HIDDENREADONLY), 
; 901  : 											Misc::HasFlag(dwFlags, FMDF_OVERWRITE), 
; 902  : 											Misc::HasFlag(dwFlags, FMDF_NEWER));

	push	48					; 00000030H
	mov	edx, DWORD PTR _dwFlags$[ebp]
	push	edx
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	push	eax
	push	16					; 00000010H
	mov	eax, DWORD PTR _dwFlags$[ebp]
	push	eax
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	push	eax
	push	8
	mov	ecx, DWORD PTR _dwFlags$[ebp]
	push	ecx
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	push	eax
	lea	ecx, DWORD PTR _sDest$75587[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sSource$75584[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?CopyFileW@FileMisc@@YAHPBG0HHH@Z	; FileMisc::CopyFileW
	add	esp, 20					; 00000014H
	mov	DWORD PTR _bResult$[ebp], eax
$L75593:

; 904  : 				}

	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR _sDest$75587[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR _sSource$75584[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75583:

; 905  : 
; 906  : 				bStopped = (WaitForSingleObject(hTerminate, 0) == WAIT_OBJECT_0);

	mov	esi, esp
	push	0
	mov	edx, DWORD PTR _hTerminate$[ebp]
	push	edx
	call	DWORD PTR __imp__WaitForSingleObject@8
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR _bStopped$[ebp], eax

; 908  : 			while (!bStopped && bResult && FindNextFile(hSearch, &finfo));

	cmp	DWORD PTR _bStopped$[ebp], 0
	jne	SHORT $L75595
	cmp	DWORD PTR _bResult$[ebp], 0
	je	SHORT $L75595
	mov	esi, esp
	lea	eax, DWORD PTR _finfo$75575[ebp]
	push	eax
	mov	ecx, DWORD PTR _hSearch$75576[ebp]
	push	ecx
	call	DWORD PTR __imp__FindNextFileW@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	$L75579
$L75595:

; 909  : 			
; 910  : 			FindClose(hSearch);

	mov	esi, esp
	mov	edx, DWORD PTR _hSearch$75576[ebp]
	push	edx
	call	DWORD PTR __imp__FindClose@4
	cmp	esi, esp
	call	__chkesp
$L75578:

; 912  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sMask$75571[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sSearchSpec$75570[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	$L75568
$L75569:

; 913  : 
; 914  : 	return (!bStopped && bResult);

	cmp	DWORD PTR _bStopped$[ebp], 0
	jne	SHORT $L77108
	cmp	DWORD PTR _bResult$[ebp], 0
	je	SHORT $L77108
	mov	DWORD PTR -720+[ebp], 1
	jmp	SHORT $L77109
$L77108:
	mov	DWORD PTR -720+[ebp], 0
$L77109:
	mov	eax, DWORD PTR -720+[ebp]
	mov	DWORD PTR $T77107[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _aMasks$[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	mov	eax, DWORD PTR $T77107[ebp]
$L75554:

; 915  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 720				; 000002d0H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77110:
	lea	ecx, DWORD PTR _aMasks$[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	ret	0
$L77111:
	lea	ecx, DWORD PTR $T77100[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77112:
	lea	ecx, DWORD PTR _sSearchSpec$75570[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77113:
	lea	ecx, DWORD PTR _sMask$75571[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77114:
	lea	ecx, DWORD PTR $T77105[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77115:
	lea	ecx, DWORD PTR _sSource$75584[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77116:
	lea	ecx, DWORD PTR $T77106[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77117:
	lea	ecx, DWORD PTR _sDest$75587[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77120:
	mov	eax, OFFSET FLAT:$T77119
	jmp	___CxxFrameHandler
text$x	ENDS
?CopyFolder@FileMisc@@YAHPBG00KPAX@Z ENDP		; FileMisc::CopyFolder
PUBLIC	?MoveFolder@FileMisc@@YAHPBG0KPAX@Z		; FileMisc::MoveFolder
;	COMDAT ?MoveFolder@FileMisc@@YAHPBG0KPAX@Z
_TEXT	SEGMENT
_szSrcFolder$ = 8
_szDestFolder$ = 12
_dwFlags$ = 16
_hTerminate$ = 20
?MoveFolder@FileMisc@@YAHPBG0KPAX@Z PROC NEAR		; FileMisc::MoveFolder, COMDAT

; 918  : {

	push	ebp
	mov	ebp, esp

; 919  : 	return MoveFolder(szSrcFolder, szDestFolder, NULL, (dwFlags | FMDF_SUBFOLDERS), hTerminate);

	mov	eax, DWORD PTR _hTerminate$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwFlags$[ebp]
	or	ecx, 1
	push	ecx
	push	0
	mov	edx, DWORD PTR _szDestFolder$[ebp]
	push	edx
	mov	eax, DWORD PTR _szSrcFolder$[ebp]
	push	eax
	call	?MoveFolder@FileMisc@@YAHPBG00KPAX@Z	; FileMisc::MoveFolder
	add	esp, 20					; 00000014H

; 920  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?MoveFolder@FileMisc@@YAHPBG0KPAX@Z ENDP		; FileMisc::MoveFolder
_TEXT	ENDS
PUBLIC	?CopyFolder@FileMisc@@YAHPBG0KPAX@Z		; FileMisc::CopyFolder
;	COMDAT ?CopyFolder@FileMisc@@YAHPBG0KPAX@Z
_TEXT	SEGMENT
_szSrcFolder$ = 8
_szDestFolder$ = 12
_dwFlags$ = 16
_hTerminate$ = 20
?CopyFolder@FileMisc@@YAHPBG0KPAX@Z PROC NEAR		; FileMisc::CopyFolder, COMDAT

; 923  : {

	push	ebp
	mov	ebp, esp

; 924  : 	return CopyFolder(szSrcFolder, szDestFolder, NULL, (dwFlags | FMDF_SUBFOLDERS), hTerminate);

	mov	eax, DWORD PTR _hTerminate$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwFlags$[ebp]
	or	ecx, 1
	push	ecx
	push	0
	mov	edx, DWORD PTR _szDestFolder$[ebp]
	push	edx
	mov	eax, DWORD PTR _szSrcFolder$[ebp]
	push	eax
	call	?CopyFolder@FileMisc@@YAHPBG00KPAX@Z	; FileMisc::CopyFolder
	add	esp, 20					; 00000014H

; 925  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?CopyFolder@FileMisc@@YAHPBG0KPAX@Z ENDP		; FileMisc::CopyFolder
_TEXT	ENDS
;	COMDAT ?DeleteFileW@FileMisc@@YAHPBGH@Z
_TEXT	SEGMENT
_szPath$ = 8
_bHiddenReadOnly$ = 12
?DeleteFileW@FileMisc@@YAHPBGH@Z PROC NEAR		; FileMisc::DeleteFileW, COMDAT

; 928  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 929  : 	if (bHiddenReadOnly)

	cmp	DWORD PTR _bHiddenReadOnly$[ebp], 0
	je	SHORT $L75613

; 930  : 		ModifyAttributes(szPath, FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN, 0);

	push	-1
	push	0
	push	3
	mov	eax, DWORD PTR _szPath$[ebp]
	push	eax
	call	?ModifyAttributes@FileMisc@@YAHPBGKKK@Z	; FileMisc::ModifyAttributes
	add	esp, 16					; 00000010H
$L75613:

; 931  : 
; 932  : 	return ::DeleteFile(szPath);

	mov	esi, esp
	mov	ecx, DWORD PTR _szPath$[ebp]
	push	ecx
	call	DWORD PTR __imp__DeleteFileW@4
	cmp	esi, esp
	call	__chkesp

; 933  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?DeleteFileW@FileMisc@@YAHPBGH@Z ENDP			; FileMisc::DeleteFileW
_TEXT	ENDS
_BSS	SEGMENT
$SG75627 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75628 DB	'f', 00H, 'a', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'd', 00H
	DB	'!', 00H, 00H, 00H
$SG75629 DB	'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'M', 00H, 'i', 00H
	DB	's', 00H, 'c', 00H, ':', 00H, ':', 00H, 'C', 00H, 'o', 00H, 'p'
	DB	00H, 'y', 00H, 'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, '(', 00H
	DB	'%', 00H, 's', 00H, ' ', 00H, '-', 00H, '>', 00H, ' ', 00H, '%'
	DB	00H, 's', 00H, ')', 00H, ' ', 00H, '%', 00H, 's', 00H, 00H, 00H
data	ENDS
;	COMDAT ?CopyFileW@FileMisc@@YAHPBG0HHH@Z
_TEXT	SEGMENT
_szSrcPath$ = 8
_szDestPath$ = 12
_bOverwrite$ = 16
_bHiddenReadOnly$ = 20
_bNewer$ = 24
_bCopied$ = -4
?CopyFileW@FileMisc@@YAHPBG0HHH@Z PROC NEAR		; FileMisc::CopyFileW, COMDAT

; 936  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 937  : 	if (bNewer && (GetFileLastModified(szSrcPath) <= GetFileLastModified(szDestPath)))

	cmp	DWORD PTR _bNewer$[ebp], 0
	je	SHORT $L75621
	mov	eax, DWORD PTR _szSrcPath$[ebp]
	push	eax
	call	?GetFileLastModified@FileMisc@@YAJPBG@Z	; FileMisc::GetFileLastModified
	add	esp, 4
	mov	esi, eax
	mov	ecx, DWORD PTR _szDestPath$[ebp]
	push	ecx
	call	?GetFileLastModified@FileMisc@@YAJPBG@Z	; FileMisc::GetFileLastModified
	add	esp, 4
	cmp	esi, eax
	jg	SHORT $L75621

; 938  : 		return TRUE;

	mov	eax, 1
	jmp	$L75620
$L75621:

; 939  : 
; 940  : 	BOOL bCopied = ::CopyFile(szSrcPath, szDestPath, TRUE);

	mov	esi, esp
	push	1
	mov	edx, DWORD PTR _szDestPath$[ebp]
	push	edx
	mov	eax, DWORD PTR _szSrcPath$[ebp]
	push	eax
	call	DWORD PTR __imp__CopyFileW@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bCopied$[ebp], eax

; 941  : 
; 942  : 	if (!bCopied && FileExists(szDestPath))

	cmp	DWORD PTR _bCopied$[ebp], 0
	jne	SHORT $L75626
	mov	ecx, DWORD PTR _szDestPath$[ebp]
	push	ecx
	call	?FileExists@FileMisc@@YAHPBG@Z		; FileMisc::FileExists
	add	esp, 4
	test	eax, eax
	je	SHORT $L75626

; 944  : 		// if allowed to overwrite try deleting dest file before re-trying
; 945  : 		if (bOverwrite)

	cmp	DWORD PTR _bOverwrite$[ebp], 0
	je	SHORT $L75624

; 947  : 			if (DeleteFile(szDestPath, bHiddenReadOnly))

	mov	edx, DWORD PTR _bHiddenReadOnly$[ebp]
	push	edx
	mov	eax, DWORD PTR _szDestPath$[ebp]
	push	eax
	call	?DeleteFileW@FileMisc@@YAHPBGH@Z	; FileMisc::DeleteFileW
	add	esp, 8
	test	eax, eax
	je	SHORT $L75625

; 949  : 				bCopied = ::CopyFile(szSrcPath, szDestPath, TRUE);

	mov	esi, esp
	push	1
	mov	ecx, DWORD PTR _szDestPath$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szSrcPath$[ebp]
	push	edx
	call	DWORD PTR __imp__CopyFileW@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bCopied$[ebp], eax
$L75625:

; 952  : 		else // not an error

	jmp	SHORT $L75626
$L75624:

; 954  : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L75620
$L75626:

; 957  : 
; 958  : 	LogText(_T("FileMisc::CopyFile(%s -> %s) %s"), szSrcPath, szDestPath, bCopied ? _T("") : _T("failed!"));

	cmp	DWORD PTR _bCopied$[ebp], 0
	je	SHORT $L77132
	mov	DWORD PTR -8+[ebp], OFFSET FLAT:$SG75627
	jmp	SHORT $L77133
$L77132:
	mov	DWORD PTR -8+[ebp], OFFSET FLAT:$SG75628
$L77133:
	mov	eax, DWORD PTR -8+[ebp]
	push	eax
	mov	ecx, DWORD PTR _szDestPath$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szSrcPath$[ebp]
	push	edx
	push	OFFSET FLAT:$SG75629
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 16					; 00000010H

; 959  : 
; 960  : 	return bCopied;

	mov	eax, DWORD PTR _bCopied$[ebp]
$L75620:

; 961  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?CopyFileW@FileMisc@@YAHPBG0HHH@Z ENDP			; FileMisc::CopyFileW
_TEXT	ENDS
PUBLIC	?MoveFileW@FileMisc@@YAHPBG0HH@Z		; FileMisc::MoveFileW
EXTRN	__imp__MoveFileW@8:NEAR
_BSS	SEGMENT
	ALIGN	4

$SG75639 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75640 DB	'f', 00H, 'a', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'd', 00H
	DB	'!', 00H, 00H, 00H
$SG75641 DB	'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'M', 00H, 'i', 00H
	DB	's', 00H, 'c', 00H, ':', 00H, ':', 00H, 'M', 00H, 'o', 00H, 'v'
	DB	00H, 'e', 00H, 'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, '(', 00H
	DB	'%', 00H, 's', 00H, ' ', 00H, '-', 00H, '>', 00H, ' ', 00H, '%'
	DB	00H, 's', 00H, ')', 00H, ' ', 00H, '%', 00H, 's', 00H, 00H, 00H
data	ENDS
;	COMDAT ?MoveFileW@FileMisc@@YAHPBG0HH@Z
_TEXT	SEGMENT
_szSrcPath$ = 8
_szDestPath$ = 12
_bOverwrite$ = 16
_bHiddenReadOnly$ = 20
_bMoved$ = -4
?MoveFileW@FileMisc@@YAHPBG0HH@Z PROC NEAR		; FileMisc::MoveFileW, COMDAT

; 964  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 965  : 	BOOL bMoved = ::MoveFile(szSrcPath, szDestPath);

	mov	esi, esp
	mov	eax, DWORD PTR _szDestPath$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szSrcPath$[ebp]
	push	ecx
	call	DWORD PTR __imp__MoveFileW@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bMoved$[ebp], eax

; 966  : 
; 967  : 	if (!bMoved && bOverwrite && FileExists(szDestPath))

	cmp	DWORD PTR _bMoved$[ebp], 0
	jne	SHORT $L75638
	cmp	DWORD PTR _bOverwrite$[ebp], 0
	je	SHORT $L75638
	mov	edx, DWORD PTR _szDestPath$[ebp]
	push	edx
	call	?FileExists@FileMisc@@YAHPBG@Z		; FileMisc::FileExists
	add	esp, 4
	test	eax, eax
	je	SHORT $L75638

; 969  : 		// try deleting dest file before re-trying
; 970  : 		if (DeleteFile(szDestPath, bHiddenReadOnly))

	mov	eax, DWORD PTR _bHiddenReadOnly$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szDestPath$[ebp]
	push	ecx
	call	?DeleteFileW@FileMisc@@YAHPBGH@Z	; FileMisc::DeleteFileW
	add	esp, 8
	test	eax, eax
	je	SHORT $L75638

; 972  : 			bMoved = ::MoveFile(szSrcPath, szDestPath);

	mov	esi, esp
	mov	edx, DWORD PTR _szDestPath$[ebp]
	push	edx
	mov	eax, DWORD PTR _szSrcPath$[ebp]
	push	eax
	call	DWORD PTR __imp__MoveFileW@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bMoved$[ebp], eax
$L75638:

; 975  : 	
; 976  : 	LogText(_T("FileMisc::MoveFile(%s -> %s) %s"), szSrcPath, szDestPath, bMoved ? _T("") : _T("failed!"));

	cmp	DWORD PTR _bMoved$[ebp], 0
	je	SHORT $L77137
	mov	DWORD PTR -8+[ebp], OFFSET FLAT:$SG75639
	jmp	SHORT $L77138
$L77137:
	mov	DWORD PTR -8+[ebp], OFFSET FLAT:$SG75640
$L77138:
	mov	ecx, DWORD PTR -8+[ebp]
	push	ecx
	mov	edx, DWORD PTR _szDestPath$[ebp]
	push	edx
	mov	eax, DWORD PTR _szSrcPath$[ebp]
	push	eax
	push	OFFSET FLAT:$SG75641
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 16					; 00000010H

; 977  : 
; 978  : 	return bMoved;

	mov	eax, DWORD PTR _bMoved$[ebp]

; 979  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?MoveFileW@FileMisc@@YAHPBG0HH@Z ENDP			; FileMisc::MoveFileW
_TEXT	ENDS
;	COMDAT ?ModifyAttributes@FileMisc@@YAHPBGKKK@Z
_TEXT	SEGMENT
_szPath$ = 8
_dwRemove$ = 12
_dwAdd$ = 16
_dwExistAttrib$ = 20
_bChange$ = -8
_dwNewAttrib$ = -4
?ModifyAttributes@FileMisc@@YAHPBGKKK@Z PROC NEAR	; FileMisc::ModifyAttributes, COMDAT

; 982  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 983  : 	if (dwExistAttrib == 0xFFFFFFFF)

	cmp	DWORD PTR _dwExistAttrib$[ebp], -1
	jne	SHORT $L75648

; 984  : 		dwExistAttrib = ::GetFileAttributes(szPath);

	mov	esi, esp
	mov	eax, DWORD PTR _szPath$[ebp]
	push	eax
	call	DWORD PTR __imp__GetFileAttributesW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _dwExistAttrib$[ebp], eax
$L75648:

; 985  : 
; 986  : 	// verify there is anything to change
; 987  : 	BOOL bChange = FALSE;

	mov	DWORD PTR _bChange$[ebp], 0

; 988  : 
; 989  : 	if (dwRemove && ((dwExistAttrib & dwRemove) != 0))

	cmp	DWORD PTR _dwRemove$[ebp], 0
	je	SHORT $L75650
	mov	ecx, DWORD PTR _dwExistAttrib$[ebp]
	and	ecx, DWORD PTR _dwRemove$[ebp]
	test	ecx, ecx
	je	SHORT $L75650

; 991  : 		bChange = TRUE;

	mov	DWORD PTR _bChange$[ebp], 1

; 993  : 	else if (dwAdd && ((dwExistAttrib & dwAdd) == 0))

	jmp	SHORT $L75652
$L75650:
	cmp	DWORD PTR _dwAdd$[ebp], 0
	je	SHORT $L75652
	mov	edx, DWORD PTR _dwExistAttrib$[ebp]
	and	edx, DWORD PTR _dwAdd$[ebp]
	test	edx, edx
	jne	SHORT $L75652

; 995  : 		bChange = TRUE;

	mov	DWORD PTR _bChange$[ebp], 1
$L75652:

; 997  : 
; 998  : 	if (!bChange)

	cmp	DWORD PTR _bChange$[ebp], 0
	jne	SHORT $L75653

; 999  : 		return -1;

	or	eax, -1
	jmp	SHORT $L75647
$L75653:

; 1000 : 
; 1001 : 	// else
; 1002 : 	DWORD dwNewAttrib = dwExistAttrib;

	mov	eax, DWORD PTR _dwExistAttrib$[ebp]
	mov	DWORD PTR _dwNewAttrib$[ebp], eax

; 1003 : 
; 1004 : 	dwNewAttrib |= dwAdd;

	mov	ecx, DWORD PTR _dwNewAttrib$[ebp]
	or	ecx, DWORD PTR _dwAdd$[ebp]
	mov	DWORD PTR _dwNewAttrib$[ebp], ecx

; 1005 : 	dwNewAttrib &= ~dwRemove;

	mov	edx, DWORD PTR _dwRemove$[ebp]
	not	edx
	mov	eax, DWORD PTR _dwNewAttrib$[ebp]
	and	eax, edx
	mov	DWORD PTR _dwNewAttrib$[ebp], eax

; 1006 : 
; 1007 : 	return ::SetFileAttributes(szPath, dwNewAttrib);

	mov	esi, esp
	mov	ecx, DWORD PTR _dwNewAttrib$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szPath$[ebp]
	push	edx
	call	DWORD PTR __imp__SetFileAttributesW@8
	cmp	esi, esp
	call	__chkesp
$L75647:

; 1008 : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?ModifyAttributes@FileMisc@@YAHPBGKKK@Z ENDP		; FileMisc::ModifyAttributes
_TEXT	ENDS
PUBLIC	?ModifyFolderAndContentAttributes@FileMisc@@YAHPBGKKH@Z ; FileMisc::ModifyFolderAndContentAttributes
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77152	DD	019930520H
	DD	06H
	DD	FLAT:$T77154
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77154	DD	0ffffffffH
	DD	FLAT:$L77147
	DD	00H
	DD	FLAT:$L77148
	DD	0ffffffffH
	DD	FLAT:$L77148
	DD	02H
	DD	FLAT:$L77149
	DD	03H
	DD	FLAT:$L77150
	DD	02H
	DD	FLAT:$L77150
xdata$x	ENDS
;	COMDAT ?ModifyFolderAndContentAttributes@FileMisc@@YAHPBGKKH@Z
_TEXT	SEGMENT
$T77144 = -628
$T77145 = -632
$T77146 = -636
__$EHRec$ = -12
_szFolder$ = 8
_dwRemove$ = 12
_dwAdd$ = 16
_bSubFolders$ = 20
_bResult$ = -616
_sSearchSpec$ = -612
_finfo$ = -604
_hSearch$ = -608
_sPathName$75674 = -620
_bFile$75677 = -624
?ModifyFolderAndContentAttributes@FileMisc@@YAHPBGKKH@Z PROC NEAR ; FileMisc::ModifyFolderAndContentAttributes, COMDAT

; 1011 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77153
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 640				; 00000280H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-652]
	mov	ecx, 160				; 000000a0H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1012 : 	if (!FolderExists(szFolder))

	mov	eax, DWORD PTR _szFolder$[ebp]
	push	eax
	call	?FolderExists@FileMisc@@YAHPBG@Z	; FileMisc::FolderExists
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75661

; 1013 : 		return TRUE; // not an error

	mov	eax, 1
	jmp	$L75660
$L75661:

; 1014 : 
; 1015 : 	// start with folder
; 1016 : 	BOOL bResult = ModifyAttributes(szFolder, dwRemove, dwAdd);

	push	-1
	mov	ecx, DWORD PTR _dwAdd$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwRemove$[ebp]
	push	edx
	mov	eax, DWORD PTR _szFolder$[ebp]
	push	eax
	call	?ModifyAttributes@FileMisc@@YAHPBGKKK@Z	; FileMisc::ModifyAttributes
	add	esp, 16					; 00000010H
	mov	DWORD PTR _bResult$[ebp], eax

; 1017 : 
; 1018 : 	CString sSearchSpec = (TerminatePath(szFolder) + ALLFILESMASK);

	mov	ecx, DWORD PTR _ALLFILESMASK
	push	ecx
	mov	edx, DWORD PTR _szFolder$[ebp]
	push	edx
	lea	eax, DWORD PTR $T77144[ebp]
	push	eax
	call	?TerminatePath@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::TerminatePath
	add	esp, 8
	mov	DWORD PTR -640+[ebp], eax
	mov	ecx, DWORD PTR -640+[ebp]
	mov	DWORD PTR -644+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	edx, DWORD PTR -644+[ebp]
	push	edx
	lea	eax, DWORD PTR _sSearchSpec$[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T77144[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1019 : 	WIN32_FIND_DATA finfo;
; 1020 : 	HANDLE hSearch = NULL;

	mov	DWORD PTR _hSearch$[ebp], 0

; 1021 : 	
; 1022 : 	if ((hSearch = FindFirstFile(sSearchSpec, &finfo)) != INVALID_HANDLE_VALUE) 

	mov	esi, esp
	lea	ecx, DWORD PTR _finfo$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	DWORD PTR __imp__FindFirstFileW@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hSearch$[ebp], eax
	cmp	DWORD PTR _hSearch$[ebp], -1
	je	$L75669
$L75670:

; 1026 : 			if (finfo.cFileName[0] != '.') 

	mov	edx, DWORD PTR _finfo$[ebp+44]
	and	edx, 65535				; 0000ffffH
	cmp	edx, 46					; 0000002eH
	je	$L75673

; 1028 : 				CString sPathName = (TerminatePath(szFolder) + finfo.cFileName);

	lea	eax, DWORD PTR _finfo$[ebp+44]
	push	eax
	mov	ecx, DWORD PTR _szFolder$[ebp]
	push	ecx
	lea	edx, DWORD PTR $T77145[ebp]
	push	edx
	call	?TerminatePath@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::TerminatePath
	add	esp, 8
	mov	DWORD PTR -648+[ebp], eax
	mov	eax, DWORD PTR -648+[ebp]
	mov	DWORD PTR -652+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR -652+[ebp]
	push	ecx
	lea	edx, DWORD PTR _sPathName$75674[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	lea	ecx, DWORD PTR $T77145[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1029 : 				BOOL bFile = !Misc::HasFlag(finfo.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY);

	push	16					; 00000010H
	mov	eax, DWORD PTR _finfo$[ebp]
	push	eax
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR _bFile$75677[ebp], eax

; 1030 : 
; 1031 : 				if (bFile)

	cmp	DWORD PTR _bFile$75677[ebp], 0
	je	SHORT $L75678

; 1033 : 					bResult &= ModifyAttributes(sPathName, dwRemove, dwAdd, finfo.dwFileAttributes);

	mov	ecx, DWORD PTR _finfo$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwAdd$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwRemove$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sPathName$75674[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?ModifyAttributes@FileMisc@@YAHPBGKKK@Z	; FileMisc::ModifyAttributes
	add	esp, 16					; 00000010H
	mov	ecx, DWORD PTR _bResult$[ebp]
	and	ecx, eax
	mov	DWORD PTR _bResult$[ebp], ecx

; 1035 : 				else if (bSubFolders)

	jmp	SHORT $L75680
$L75678:
	cmp	DWORD PTR _bSubFolders$[ebp], 0
	je	SHORT $L75680

; 1037 : 					// folder and contents
; 1038 : 					bResult &= ModifyFolderAndContentAttributes(sPathName, dwRemove, dwAdd, TRUE);

	push	1
	mov	edx, DWORD PTR _dwAdd$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwRemove$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sPathName$75674[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?ModifyFolderAndContentAttributes@FileMisc@@YAHPBGKKH@Z ; FileMisc::ModifyFolderAndContentAttributes
	add	esp, 16					; 00000010H
	mov	ecx, DWORD PTR _bResult$[ebp]
	and	ecx, eax
	mov	DWORD PTR _bResult$[ebp], ecx
$L75680:

; 1040 : 
; 1041 : 			}

	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sPathName$75674[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75673:

; 1043 : 		while (FindNextFile(hSearch, &finfo));

	mov	esi, esp
	lea	edx, DWORD PTR _finfo$[ebp]
	push	edx
	mov	eax, DWORD PTR _hSearch$[ebp]
	push	eax
	call	DWORD PTR __imp__FindNextFileW@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	$L75670

; 1044 : 		
; 1045 : 		FindClose(hSearch);

	mov	esi, esp
	mov	ecx, DWORD PTR _hSearch$[ebp]
	push	ecx
	call	DWORD PTR __imp__FindClose@4
	cmp	esi, esp
	call	__chkesp
$L75669:

; 1047 : 	
; 1048 : 	return bResult;

	mov	edx, DWORD PTR _bResult$[ebp]
	mov	DWORD PTR $T77146[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77146[ebp]
$L75660:

; 1049 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 652				; 0000028cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77147:
	lea	ecx, DWORD PTR $T77144[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77148:
	lea	ecx, DWORD PTR _sSearchSpec$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77149:
	lea	ecx, DWORD PTR $T77145[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77150:
	lea	ecx, DWORD PTR _sPathName$75674[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77153:
	mov	eax, OFFSET FLAT:$T77152
	jmp	___CxxFrameHandler
text$x	ENDS
?ModifyFolderAndContentAttributes@FileMisc@@YAHPBGKKH@Z ENDP ; FileMisc::ModifyFolderAndContentAttributes
PUBLIC	?GetFileSize@FileMisc@@YANPBG@Z			; FileMisc::GetFileSize
EXTRN	?GetLength64@CFileFind@@QBE_JXZ:NEAR		; CFileFind::GetLength64
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77163	DD	019930520H
	DD	01H
	DD	FLAT:$T77165
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77165	DD	0ffffffffH
	DD	FLAT:$L77161
xdata$x	ENDS
;	COMDAT ?GetFileSize@FileMisc@@YANPBG@Z
_TEXT	SEGMENT
$T77159 = -48
$T77160 = -56
__$EHRec$ = -12
_szPath$ = 8
_ff$ = -40
?GetFileSize@FileMisc@@YANPBG@Z PROC NEAR		; FileMisc::GetFileSize, COMDAT

; 1052 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77164
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 52					; 00000034H
	push	edi
	lea	edi, DWORD PTR [ebp-64]
	mov	ecx, 13					; 0000000dH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1053 : 	CFileFind ff;

	lea	ecx, DWORD PTR _ff$[ebp]
	call	??0CFileFind@@QAE@XZ			; CFileFind::CFileFind
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1054 : 	
; 1055 : 	if (ff.FindFile(szPath))

	push	0
	mov	eax, DWORD PTR _szPath$[ebp]
	push	eax
	lea	ecx, DWORD PTR _ff$[ebp]
	call	?FindFile@CFileFind@@UAEHPBGK@Z		; CFileFind::FindFile
	test	eax, eax
	je	SHORT $L75686

; 1057 : 		ff.FindNextFile();

	lea	ecx, DWORD PTR _ff$[ebp]
	call	?FindNextFileW@CFileFind@@UAEHXZ	; CFileFind::FindNextFileW

; 1058 : 
; 1059 : #if _MSC_VER < 1400
; 1060 : 		return (double)ff.GetLength64();

	lea	ecx, DWORD PTR _ff$[ebp]
	call	?GetLength64@CFileFind@@QBE_JXZ		; CFileFind::GetLength64
	mov	DWORD PTR -64+[ebp], eax
	mov	DWORD PTR -64+[ebp+4], edx
	fild	QWORD PTR -64+[ebp]
	fstp	QWORD PTR $T77159[ebp]
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _ff$[ebp]
	call	??1CFileFind@@UAE@XZ			; CFileFind::~CFileFind
	fld	QWORD PTR $T77159[ebp]
	jmp	SHORT $L75684
$L75686:

; 1065 : 
; 1066 : 	// else
; 1067 : 	return 0;

	mov	DWORD PTR $T77160[ebp], 0
	mov	DWORD PTR $T77160[ebp+4], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _ff$[ebp]
	call	??1CFileFind@@UAE@XZ			; CFileFind::~CFileFind
	fld	QWORD PTR $T77160[ebp]
$L75684:

; 1068 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 64					; 00000040H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77161:
	lea	ecx, DWORD PTR _ff$[ebp]
	call	??1CFileFind@@UAE@XZ			; CFileFind::~CFileFind
	ret	0
$L77164:
	mov	eax, OFFSET FLAT:$T77163
	jmp	___CxxFrameHandler
text$x	ENDS
?GetFileSize@FileMisc@@YANPBG@Z ENDP			; FileMisc::GetFileSize
PUBLIC	?IsLoggingEnabledW@FileMisc@@YAHXZ		; FileMisc::IsLoggingEnabledW
;	COMDAT ?IsLoggingEnabledW@FileMisc@@YAHXZ
_TEXT	SEGMENT
?IsLoggingEnabledW@FileMisc@@YAHXZ PROC NEAR		; FileMisc::IsLoggingEnabledW, COMDAT

; 1071 : {

	push	ebp
	mov	ebp, esp

; 1072 : 	return s_bLogging;

	mov	eax, DWORD PTR _s_bLogging

; 1073 : }

	pop	ebp
	ret	0
?IsLoggingEnabledW@FileMisc@@YAHXZ ENDP			; FileMisc::IsLoggingEnabledW
_TEXT	ENDS
PUBLIC	?GetTempFolder@FileMisc@@YA?AVCString@@XZ	; FileMisc::GetTempFolder
PUBLIC	?GetAppFileName@FileMisc@@YA?AVCString@@XZ	; FileMisc::GetAppFileName
PUBLIC	?GetAppFolder@FileMisc@@YA?AVCString@@PBG@Z	; FileMisc::GetAppFolder
PUBLIC	?GetSpecialFilePath@FileMisc@@YAHHABVCString@@AAV2@@Z ; FileMisc::GetSpecialFilePath
PUBLIC	?GetLogFileName@FileMisc@@YA?AVCString@@XZ	; FileMisc::GetLogFileName
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75712 DB	'D:\_CODE\Shared\FILEMISC.CPP', 00H
	ORG $+3
$SG75718 DB	'D:\_CODE\Shared\FILEMISC.CPP', 00H
	ORG $+3
$SG75719 DB	'l', 00H, 'o', 00H, 'g', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77184	DD	019930520H
	DD	08H
	DD	FLAT:$T77186
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77186	DD	0ffffffffH
	DD	FLAT:$L77180
	DD	00H
	DD	FLAT:$L77174
	DD	01H
	DD	FLAT:$L77175
	DD	02H
	DD	FLAT:$L77176
	DD	03H
	DD	FLAT:$L77177
	DD	02H
	DD	FLAT:$L77177
	DD	05H
	DD	FLAT:$L77178
	DD	05H
	DD	FLAT:$L77179
xdata$x	ENDS
;	COMDAT ?GetLogFileName@FileMisc@@YA?AVCString@@XZ
_TEXT	SEGMENT
$T77172 = -32
$T77173 = -36
$T77181 = -40
__$EHRec$ = -12
___$ReturnUdt$ = 8
_sLogFolder$ = -20
_sLogFile$ = -16
_sAppName$ = -24
_sAppFolder$75704 = -28
?GetLogFileName@FileMisc@@YA?AVCString@@XZ PROC NEAR	; FileMisc::GetLogFileName, COMDAT

; 1076 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77185
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 44					; 0000002cH
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-56]
	mov	ecx, 11					; 0000000bH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T77181[ebp], 0

; 1077 : 	CString sLogFolder, sLogFile;

	lea	ecx, DWORD PTR _sLogFolder$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sLogFile$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 1078 : 	CString sAppName(GetFileNameFromPath(GetAppFileName(), FALSE));

	push	0
	lea	eax, DWORD PTR $T77172[ebp]
	push	eax
	call	?GetAppFileName@FileMisc@@YA?AVCString@@XZ ; FileMisc::GetAppFileName
	add	esp, 4
	mov	DWORD PTR -44+[ebp], eax
	mov	ecx, DWORD PTR -44+[ebp]
	mov	DWORD PTR -48+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR -48+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	edx, DWORD PTR _sAppName$[ebp]
	push	edx
	call	?GetFileNameFromPath@FileMisc@@YA?AVCString@@PBGH@Z ; FileMisc::GetFileNameFromPath
	add	esp, 12					; 0000000cH
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	lea	ecx, DWORD PTR $T77172[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1079 : 
; 1080 : 	if (Misc::HasFlag(s_dwLogFlags, FMLF_TEMPDIR))

	push	4
	mov	eax, DWORD PTR _s_dwLogFlags
	push	eax
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	test	eax, eax
	je	SHORT $L75700

; 1082 : 		sLogFolder = GetTempFolder();

	lea	ecx, DWORD PTR $T77173[ebp]
	push	ecx
	call	?GetTempFolder@FileMisc@@YA?AVCString@@XZ ; FileMisc::GetTempFolder
	add	esp, 4
	mov	DWORD PTR -52+[ebp], eax
	mov	edx, DWORD PTR -52+[ebp]
	mov	DWORD PTR -56+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	mov	eax, DWORD PTR -56+[ebp]
	push	eax
	lea	ecx, DWORD PTR _sLogFolder$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	lea	ecx, DWORD PTR $T77173[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1084 : 	else

	jmp	$L75713
$L75700:

; 1086 : 		if (!Misc::HasFlag(s_dwLogFlags, FMLF_NOAPPDIR))

	push	8
	mov	ecx, DWORD PTR _s_dwLogFlags
	push	ecx
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75703

; 1088 : 			CString sAppFolder = GetAppFolder();

	push	0
	lea	edx, DWORD PTR _sAppFolder$75704[ebp]
	push	edx
	call	?GetAppFolder@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::GetAppFolder
	add	esp, 8
	mov	BYTE PTR __$EHRec$[ebp+8], 7

; 1089 : 			
; 1090 : 			if (IsFolderWritable(sAppFolder))

	push	0
	lea	ecx, DWORD PTR _sAppFolder$75704[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsFolderWritable@FileMisc@@YAHPBGH@Z	; FileMisc::IsFolderWritable
	add	esp, 8
	test	eax, eax
	je	SHORT $L75706

; 1091 : 				sLogFolder = sAppFolder;

	lea	eax, DWORD PTR _sAppFolder$75704[ebp]
	push	eax
	lea	ecx, DWORD PTR _sLogFolder$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
$L75706:

; 1092 : 		}

	mov	BYTE PTR __$EHRec$[ebp+8], 5
	lea	ecx, DWORD PTR _sAppFolder$75704[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75703:

; 1093 : 
; 1094 : 		// else use APPDATA folder
; 1095 : 		if (sLogFolder.IsEmpty())

	lea	ecx, DWORD PTR _sLogFolder$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75713
$L75708:

; 1097 : 			VERIFY (GetSpecialFilePath(CSIDL_APPDATA, s_sLogAppDataKey, sLogFolder));

	lea	ecx, DWORD PTR _sLogFolder$[ebp]
	push	ecx
	push	OFFSET FLAT:_s_sLogAppDataKey
	push	26					; 0000001aH
	call	?GetSpecialFilePath@FileMisc@@YAHHABVCString@@AAV2@@Z ; FileMisc::GetSpecialFilePath
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	SHORT $L75711
	push	1097					; 00000449H
	push	OFFSET FLAT:$SG75712
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75711
	int	3
$L75711:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75708

; 1098 : 			
; 1099 : 			// add app name as sub folder if not in key
; 1100 : 			if (s_sLogAppDataKey.Find(sAppName) == -1)

	lea	ecx, DWORD PTR _sAppName$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:_s_sLogAppDataKey
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	cmp	eax, -1
	jne	SHORT $L75713

; 1101 : 				TerminatePath(sLogFolder) += sAppName;

	lea	eax, DWORD PTR _sAppName$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sLogFolder$[ebp]
	push	ecx
	call	?TerminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ; FileMisc::TerminatePath
	add	esp, 4
	mov	ecx, eax
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
$L75713:

; 1104 : 
; 1105 : 	ASSERT(!sLogFolder.IsEmpty());

	lea	ecx, DWORD PTR _sLogFolder$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75717
	push	1105					; 00000451H
	push	OFFSET FLAT:$SG75718
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75717
	int	3
$L75717:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75713

; 1106 : 
; 1107 : 	return MakePath(sLogFile, NULL, sLogFolder, sAppName, _T("log"));

	push	OFFSET FLAT:$SG75719
	lea	ecx, DWORD PTR _sAppName$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sLogFolder$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	0
	lea	eax, DWORD PTR _sLogFile$[ebp]
	push	eax
	call	?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z ; FileMisc::MakePath
	add	esp, 20					; 00000014H
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T77181[ebp]
	or	ecx, 1
	mov	DWORD PTR $T77181[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sAppName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sLogFile$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sLogFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1108 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 56					; 00000038H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77174:
	lea	ecx, DWORD PTR _sLogFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77175:
	lea	ecx, DWORD PTR _sLogFile$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77176:
	lea	ecx, DWORD PTR $T77172[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77177:
	lea	ecx, DWORD PTR _sAppName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77178:
	lea	ecx, DWORD PTR $T77173[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77179:
	lea	ecx, DWORD PTR _sAppFolder$75704[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77180:
	mov	eax, DWORD PTR $T77181[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77182
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77182:
	ret	0
$L77185:
	mov	eax, OFFSET FLAT:$T77184
	jmp	___CxxFrameHandler
text$x	ENDS
?GetLogFileName@FileMisc@@YA?AVCString@@XZ ENDP		; FileMisc::GetLogFileName
PUBLIC	?GetModuleVersion@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z ; FileMisc::GetModuleVersion
PUBLIC	?EnableLogging@FileMisc@@YAHHPBGK@Z		; FileMisc::EnableLogging
PUBLIC	?LogTextRaw@FileMisc@@YAXPBG@Z			; FileMisc::LogTextRaw
EXTRN	??0COSVersion@@QAE@XZ:NEAR			; COSVersion::COSVersion
EXTRN	??1COSVersion@@UAE@XZ:NEAR			; COSVersion::~COSVersion
EXTRN	?FormatOSVersion@COSVersion@@QBE?AVCString@@XZ:NEAR ; COSVersion::FormatOSVersion
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75730 DB	'D:\_CODE\Shared\FILEMISC.CPP', 00H
	ORG $+3
$SG75736 DB	'L', 00H, 'o', 00H, 'g', 00H, 'g', 00H, 'i', 00H, 'n', 00H
	DB	'g', 00H, ' ', 00H, 't', 00H, 'o', 00H, ':', 00H, ' ', 00H, '%'
	DB	00H, 's', 00H, 00H, 00H
	ORG $+2
$SG75737 DB	'-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H
	DB	'-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-'
	DB	00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H
	DB	'-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-'
	DB	00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H
	DB	'-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-'
	DB	00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H
	DB	'-', 00H, '-', 00H, '-', 00H, '-', 00H, 00H, 00H
$SG75740 DB	'W', 00H, 'i', 00H, 'n', 00H, 'd', 00H, 'o', 00H, 'w', 00H
	DB	's', 00H, ' ', 00H, 'V', 00H, 'e', 00H, 'r', 00H, 's', 00H, 'i'
	DB	00H, 'o', 00H, 'n', 00H, ':', 00H, ' ', 00H, '%', 00H, 's', 00H
	DB	00H, 00H
$SG75742 DB	'A', 00H, 'p', 00H, 'p', 00H, ' ', 00H, 'V', 00H, 'e', 00H
	DB	'r', 00H, 's', 00H, 'i', 00H, 'o', 00H, 'n', 00H, ':', 00H, ' '
	DB	00H, '%', 00H, 's', 00H, 00H, 00H
$SG75743 DB	'-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H
	DB	'-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-'
	DB	00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H
	DB	'-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-'
	DB	00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H
	DB	'-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-'
	DB	00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H
	DB	'-', 00H, '-', 00H, '-', 00H, '-', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77203	DD	019930520H
	DD	05H
	DD	FLAT:$T77205
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77205	DD	0ffffffffH
	DD	FLAT:$L77197
	DD	0ffffffffH
	DD	FLAT:$L77198
	DD	0ffffffffH
	DD	FLAT:$L77199
	DD	02H
	DD	FLAT:$L77200
	DD	0ffffffffH
	DD	FLAT:$L77201
xdata$x	ENDS
;	COMDAT ?EnableLogging@FileMisc@@YAHHPBGK@Z
_TEXT	SEGMENT
$T77192 = -16
$T77193 = -20
$T77194 = -24
$T77195 = -28
$T77196 = -32
__$EHRec$ = -12
_bEnable$ = 8
_szAppDataKey$ = 12
_dwFlags$ = 16
?EnableLogging@FileMisc@@YAHHPBGK@Z PROC NEAR		; FileMisc::EnableLogging, COMDAT

; 1111 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77204
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 60					; 0000003cH
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-72]
	mov	ecx, 15					; 0000000fH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1112 : 	if (IsLoggingEnabled())

	call	?IsLoggingEnabledW@FileMisc@@YAHXZ	; FileMisc::IsLoggingEnabledW
	test	eax, eax
	je	SHORT $L75725

; 1113 : 		return TRUE;

	mov	eax, 1
	jmp	$L75724
$L75725:

; 1114 : 
; 1115 : 	// these flags are mutually exclusive
; 1116 : 	// better to fail because user (me) must be confused!
; 1117 : 	ASSERT (!(Misc::HasFlag(dwFlags, FMLF_NOAPPDIR) && Misc::HasFlag(dwFlags, FMLF_TEMPDIR)));

	push	8
	mov	eax, DWORD PTR _dwFlags$[ebp]
	push	eax
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	test	eax, eax
	je	SHORT $L75729
	push	4
	mov	ecx, DWORD PTR _dwFlags$[ebp]
	push	ecx
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	test	eax, eax
	je	SHORT $L75729
	push	1117					; 0000045dH
	push	OFFSET FLAT:$SG75730
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75729
	int	3
$L75729:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75725

; 1118 : 
; 1119 : 	if (Misc::HasFlag(dwFlags, FMLF_NOAPPDIR) && Misc::HasFlag(dwFlags, FMLF_TEMPDIR))

	push	8
	mov	eax, DWORD PTR _dwFlags$[ebp]
	push	eax
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	test	eax, eax
	je	SHORT $L75731
	push	4
	mov	ecx, DWORD PTR _dwFlags$[ebp]
	push	ecx
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	test	eax, eax
	je	SHORT $L75731

; 1120 : 		return FALSE;

	xor	eax, eax
	jmp	$L75724
$L75731:

; 1121 : 
; 1122 : 	s_sLogAppDataKey = szAppDataKey;

	mov	edx, DWORD PTR _szAppDataKey$[ebp]
	push	edx
	mov	ecx, OFFSET FLAT:_s_sLogAppDataKey
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 1123 : 	s_bLogging = bEnable;

	mov	eax, DWORD PTR _bEnable$[ebp]
	mov	DWORD PTR _s_bLogging, eax

; 1124 : 	s_dwLogFlags = dwFlags;

	mov	ecx, DWORD PTR _dwFlags$[ebp]
	mov	DWORD PTR _s_dwLogFlags, ecx

; 1125 : 
; 1126 : 	if (bEnable)

	cmp	DWORD PTR _bEnable$[ebp], 0
	je	$L75732

; 1128 : 		if (!Misc::HasFlag(s_dwLogFlags, FMLF_NORESET))

	push	1
	mov	edx, DWORD PTR _s_dwLogFlags
	push	edx
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75733

; 1129 : 			DeleteFile(GetLogFileName(), TRUE);

	push	1
	lea	eax, DWORD PTR $T77192[ebp]
	push	eax
	call	?GetLogFileName@FileMisc@@YA?AVCString@@XZ ; FileMisc::GetLogFileName
	add	esp, 4
	mov	DWORD PTR -36+[ebp], eax
	mov	ecx, DWORD PTR -36+[ebp]
	mov	DWORD PTR -40+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -40+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?DeleteFileW@FileMisc@@YAHPBGH@Z	; FileMisc::DeleteFileW
	add	esp, 8
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T77192[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75733:

; 1130 : 
; 1131 : 		LogText(_T("Logging to: %s"), GetLogFileName());

	lea	edx, DWORD PTR $T77193[ebp]
	push	edx
	call	?GetLogFileName@FileMisc@@YA?AVCString@@XZ ; FileMisc::GetLogFileName
	add	esp, 4
	mov	DWORD PTR -44+[ebp], eax
	mov	eax, DWORD PTR -44+[ebp]
	mov	DWORD PTR -48+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -48+[ebp]
	mov	edx, DWORD PTR [ecx]
	push	edx
	push	OFFSET FLAT:$SG75736
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 8
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T77193[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1132 : 		LogTextRaw(_T("-------------------------------------------------"));

	push	OFFSET FLAT:$SG75737
	call	?LogTextRaw@FileMisc@@YAXPBG@Z		; FileMisc::LogTextRaw
	add	esp, 4

; 1133 : 		LogText(_T("Windows Version: %s"), COSVersion().FormatOSVersion());

	lea	eax, DWORD PTR $T77195[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77194[ebp]
	call	??0COSVersion@@QAE@XZ			; COSVersion::COSVersion
	mov	DWORD PTR -52+[ebp], eax
	mov	ecx, DWORD PTR -52+[ebp]
	mov	DWORD PTR -56+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR -56+[ebp]
	call	?FormatOSVersion@COSVersion@@QBE?AVCString@@XZ ; COSVersion::FormatOSVersion
	mov	DWORD PTR -60+[ebp], eax
	mov	edx, DWORD PTR -60+[ebp]
	mov	DWORD PTR -64+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	eax, DWORD PTR -64+[ebp]
	mov	ecx, DWORD PTR [eax]
	push	ecx
	push	OFFSET FLAT:$SG75740
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 8
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T77195[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T77194[ebp]
	call	??1COSVersion@@UAE@XZ			; COSVersion::~COSVersion

; 1134 : 		LogText(_T("App Version: %s"), GetModuleVersion());

	push	0
	lea	edx, DWORD PTR $T77196[ebp]
	push	edx
	call	?GetModuleVersion@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z ; FileMisc::GetModuleVersion
	add	esp, 8
	mov	DWORD PTR -68+[ebp], eax
	mov	eax, DWORD PTR -68+[ebp]
	mov	DWORD PTR -72+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 4
	mov	ecx, DWORD PTR -72+[ebp]
	mov	edx, DWORD PTR [ecx]
	push	edx
	push	OFFSET FLAT:$SG75742
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 8
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T77196[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1135 : 		LogTextRaw(_T("-------------------------------------------------"));

	push	OFFSET FLAT:$SG75743
	call	?LogTextRaw@FileMisc@@YAXPBG@Z		; FileMisc::LogTextRaw
	add	esp, 4
$L75732:

; 1137 : 
; 1138 : 	return TRUE;

	mov	eax, 1
$L75724:

; 1139 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 72					; 00000048H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77197:
	lea	ecx, DWORD PTR $T77192[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77198:
	lea	ecx, DWORD PTR $T77193[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77199:
	lea	ecx, DWORD PTR $T77194[ebp]
	call	??1COSVersion@@UAE@XZ			; COSVersion::~COSVersion
	ret	0
$L77200:
	lea	ecx, DWORD PTR $T77195[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77201:
	lea	ecx, DWORD PTR $T77196[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77204:
	mov	eax, OFFSET FLAT:$T77203
	jmp	___CxxFrameHandler
text$x	ENDS
?EnableLogging@FileMisc@@YAHHPBGK@Z ENDP		; FileMisc::EnableLogging
PUBLIC	?LogTimeElapsed@FileMisc@@YAXAAKPBGZZ		; FileMisc::LogTimeElapsed
EXTRN	__imp__GetTickCount@0:NEAR
EXTRN	?AfxIsValidString@@YGHPBGH@Z:NEAR		; AfxIsValidString
EXTRN	?Format@CString@@QAAXPBGZZ:NEAR			; CString::Format
EXTRN	?FormatV@CString@@QAEXPBGPAD@Z:NEAR		; CString::FormatV
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75751 DB	'A', 00H, 'c', 00H, 't', 00H, 'i', 00H, 'o', 00H, 'n', 00H
	DB	00H, 00H
	ORG $+2
$SG75757 DB	'D:\_CODE\Shared\FILEMISC.CPP', 00H
	ORG $+3
$SG75764 DB	'%', 00H, 's', 00H, ' ', 00H, 't', 00H, 'o', 00H, 'o', 00H
	DB	'k', 00H, ' ', 00H, '%', 00H, 'l', 00H, 'u', 00H, ' ', 00H, 'm'
	DB	00H, 's', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77214	DD	019930520H
	DD	02H
	DD	FLAT:$T77216
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77216	DD	0ffffffffH
	DD	FLAT:$L77211
	DD	00H
	DD	FLAT:$L77212
xdata$x	ENDS
;	COMDAT ?LogTimeElapsed@FileMisc@@YAXAAKPBGZZ
_TEXT	SEGMENT
__$EHRec$ = -12
_dwTickStart$ = 8
_szFormat$ = 12
_sMessage$ = -20
_sReason$ = -16
_argList$75758 = -28
_dwTickEnd$ = -24
?LogTimeElapsed@FileMisc@@YAXAAKPBGZZ PROC NEAR		; FileMisc::LogTimeElapsed, COMDAT

; 1142 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77215
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 1143 : 	if (!IsLoggingEnabled())

	call	?IsLoggingEnabledW@FileMisc@@YAHXZ	; FileMisc::IsLoggingEnabledW
	test	eax, eax
	jne	SHORT $L75748

; 1144 : 		return;

	jmp	$L75747
$L75748:

; 1145 : 
; 1146 : 	CString sMessage, sReason(_T("Action"));

	lea	ecx, DWORD PTR _sMessage$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	push	OFFSET FLAT:$SG75751
	lea	ecx, DWORD PTR _sReason$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 1147 : 
; 1148 : 	if (szFormat && *szFormat)

	cmp	DWORD PTR _szFormat$[ebp], 0
	je	SHORT $L75752
	mov	eax, DWORD PTR _szFormat$[ebp]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax]
	test	ecx, ecx
	je	SHORT $L75752
$L75753:

; 1150 : 		// from CString::Format
; 1151 : 		ASSERT(AfxIsValidString(szFormat));

	push	-1
	mov	edx, DWORD PTR _szFormat$[ebp]
	push	edx
	call	?AfxIsValidString@@YGHPBGH@Z		; AfxIsValidString
	test	eax, eax
	jne	SHORT $L75756
	push	1151					; 0000047fH
	push	OFFSET FLAT:$SG75757
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75756
	int	3
$L75756:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75753

; 1152 : 
; 1153 : 		va_list argList;
; 1154 : 		va_start(argList, szFormat);

	lea	ecx, DWORD PTR _szFormat$[ebp+4]
	mov	DWORD PTR _argList$75758[ebp], ecx

; 1155 : 		sReason.FormatV(szFormat, argList);

	mov	edx, DWORD PTR _argList$75758[ebp]
	push	edx
	mov	eax, DWORD PTR _szFormat$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sReason$[ebp]
	call	?FormatV@CString@@QAEXPBGPAD@Z		; CString::FormatV

; 1156 : 		va_end(argList);

	mov	DWORD PTR _argList$75758[ebp], 0
$L75752:

; 1158 : 
; 1159 : 	DWORD dwTickEnd = GetTickCount();

	mov	esi, esp
	call	DWORD PTR __imp__GetTickCount@0
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _dwTickEnd$[ebp], eax

; 1160 : 	sMessage.Format(_T("%s took %lu ms"), sReason, dwTickEnd - dwTickStart);

	mov	ecx, DWORD PTR _dwTickStart$[ebp]
	mov	edx, DWORD PTR _dwTickEnd$[ebp]
	sub	edx, DWORD PTR [ecx]
	push	edx
	mov	eax, DWORD PTR _sReason$[ebp]
	push	eax
	push	OFFSET FLAT:$SG75764
	lea	ecx, DWORD PTR _sMessage$[ebp]
	push	ecx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 16					; 00000010H

; 1161 : 
; 1162 : 	FileMisc::LogText(sMessage);

	lea	ecx, DWORD PTR _sMessage$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 4

; 1163 : 	dwTickStart = dwTickEnd;

	mov	edx, DWORD PTR _dwTickStart$[ebp]
	mov	eax, DWORD PTR _dwTickEnd$[ebp]
	mov	DWORD PTR [edx], eax

; 1164 : }

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sReason$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sMessage$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75747:
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77211:
	lea	ecx, DWORD PTR _sMessage$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77212:
	lea	ecx, DWORD PTR _sReason$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77215:
	mov	eax, OFFSET FLAT:$T77214
	jmp	___CxxFrameHandler
text$x	ENDS
?LogTimeElapsed@FileMisc@@YAXAAKPBGZZ ENDP		; FileMisc::LogTimeElapsed
PUBLIC	?AppendLineToFile@FileMisc@@YAHPBGABVCString@@W4SFE_SAVEAS@@@Z ; FileMisc::AppendLineToFile
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75777 DB	'D:\_CODE\Shared\FILEMISC.CPP', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77231	DD	019930520H
	DD	02H
	DD	FLAT:$T77233
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77233	DD	0ffffffffH
	DD	FLAT:$L77228
	DD	00H
	DD	FLAT:$L77229
xdata$x	ENDS
;	COMDAT ?LogTextRaw@FileMisc@@YAXPBG@Z
_TEXT	SEGMENT
$T77222 = -16
$T77223 = -20
$T77224 = -24
$T77225 = -28
__$EHRec$ = -12
_szText$ = 8
?LogTextRaw@FileMisc@@YAXPBG@Z PROC NEAR		; FileMisc::LogTextRaw, COMDAT

; 1167 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77232
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 28					; 0000001cH
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 1168 : 	if (IsLoggingEnabled())

	call	?IsLoggingEnabledW@FileMisc@@YAHXZ	; FileMisc::IsLoggingEnabledW
	test	eax, eax
	je	$L75771
$L75769:

; 1169 : 		VERIFY(AppendLineToFile(GetLogFileName(), szText));

	mov	eax, DWORD PTR _szText$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77223[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	push	2
	lea	ecx, DWORD PTR $T77223[ebp]
	push	ecx
	lea	edx, DWORD PTR $T77224[ebp]
	push	edx
	call	?GetLogFileName@FileMisc@@YA?AVCString@@XZ ; FileMisc::GetLogFileName
	add	esp, 4
	mov	DWORD PTR -32+[ebp], eax
	mov	eax, DWORD PTR -32+[ebp]
	mov	DWORD PTR -36+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -36+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?AppendLineToFile@FileMisc@@YAHPBGABVCString@@W4SFE_SAVEAS@@@Z ; FileMisc::AppendLineToFile
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	SHORT $L77226
	push	1169					; 00000491H
	push	OFFSET FLAT:$SG75777
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	mov	DWORD PTR $T77225[ebp], eax
	cmp	DWORD PTR $T77225[ebp], 0
	je	SHORT $L77226
	mov	DWORD PTR -40+[ebp], 1
	jmp	SHORT $L77227
$L77226:
	mov	DWORD PTR -40+[ebp], 0
$L77227:
	mov	cl, BYTE PTR -40+[ebp]
	mov	BYTE PTR $T77222[ebp], cl
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T77224[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T77223[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	edx, DWORD PTR $T77222[ebp]
	and	edx, 255				; 000000ffH
	test	edx, edx
	je	SHORT $L75775
	int	3
$L75775:
	xor	eax, eax
	test	eax, eax
	jne	$L75769
$L75771:

; 1170 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77228:
	lea	ecx, DWORD PTR $T77223[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77229:
	lea	ecx, DWORD PTR $T77224[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77232:
	mov	eax, OFFSET FLAT:$T77231
	jmp	___CxxFrameHandler
text$x	ENDS
?LogTextRaw@FileMisc@@YAXPBG@Z ENDP			; FileMisc::LogTextRaw
EXTRN	?Format@COleDateTime@@QBE?AVCString@@KK@Z:NEAR	; COleDateTime::Format
EXTRN	??YCString@@QAEABV0@PBG@Z:NEAR			; CString::operator+=
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75788 DB	'D:\_CODE\Shared\FILEMISC.CPP', 00H
	ORG $+3
$SG75795 DB	' ', 00H, '(', 00H, 00H, 00H
	ORG $+2
$SG75798 DB	')', 00H, 00H, 00H
$SG75806 DB	'D:\_CODE\Shared\FILEMISC.CPP', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77250	DD	019930520H
	DD	03H
	DD	FLAT:$T77252
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77252	DD	0ffffffffH
	DD	FLAT:$L77246
	DD	00H
	DD	FLAT:$L77247
	DD	00H
	DD	FLAT:$L77248
xdata$x	ENDS
;	COMDAT ?LogText@FileMisc@@YAXPBGZZ
_TEXT	SEGMENT
_szText$ = 8
_sLogLine$ = -16
_argList$75789 = -20
$T77239 = -32
$T77240 = -36
$T77241 = -40
$T77242 = -44
$T77243 = -48
__$EHRec$ = -12
?LogText@FileMisc@@YAXPBGZZ PROC NEAR			; FileMisc::LogText, COMDAT

; 1173 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77251
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 56					; 00000038H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-68]
	mov	ecx, 14					; 0000000eH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1174 : 	if (!IsLoggingEnabled())

	call	?IsLoggingEnabledW@FileMisc@@YAHXZ	; FileMisc::IsLoggingEnabledW
	test	eax, eax
	jne	SHORT $L75781

; 1175 : 		return;

	jmp	$L75780
$L75781:

; 1176 : 
; 1177 : 	CString sLogLine;

	lea	ecx, DWORD PTR _sLogLine$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1178 : 
; 1179 : 	if (szText && *szText)

	cmp	DWORD PTR _szText$[ebp], 0
	je	SHORT $L75783
	mov	eax, DWORD PTR _szText$[ebp]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax]
	test	ecx, ecx
	je	SHORT $L75783
$L75784:

; 1181 : 		// from CString::Format
; 1182 : 		ASSERT(AfxIsValidString(szText));

	push	-1
	mov	edx, DWORD PTR _szText$[ebp]
	push	edx
	call	?AfxIsValidString@@YGHPBGH@Z		; AfxIsValidString
	test	eax, eax
	jne	SHORT $L75787
	push	1182					; 0000049eH
	push	OFFSET FLAT:$SG75788
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75787
	int	3
$L75787:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75784

; 1183 : 
; 1184 : 		va_list argList;
; 1185 : 		va_start(argList, szText);

	lea	ecx, DWORD PTR _szText$[ebp+4]
	mov	DWORD PTR _argList$75789[ebp], ecx

; 1186 : 		sLogLine.FormatV(szText, argList);

	mov	edx, DWORD PTR _argList$75789[ebp]
	push	edx
	mov	eax, DWORD PTR _szText$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sLogLine$[ebp]
	call	?FormatV@CString@@QAEXPBGPAD@Z		; CString::FormatV

; 1187 : 		va_end(argList);

	mov	DWORD PTR _argList$75789[ebp], 0
$L75783:

; 1189 : 
; 1190 : 	if (Misc::HasFlag(s_dwLogFlags, FMLF_TIMESTAMP))

	push	2
	mov	ecx, DWORD PTR _s_dwLogFlags
	push	ecx
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	test	eax, eax
	je	SHORT $L75794

; 1192 : 		sLogLine += _T(" (");

	push	OFFSET FLAT:$SG75795
	lea	ecx, DWORD PTR _sLogLine$[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=

; 1193 : 		sLogLine += COleDateTime::GetCurrentTime().Format();

	push	1024					; 00000400H
	push	0
	lea	edx, DWORD PTR $T77240[ebp]
	push	edx
	lea	eax, DWORD PTR $T77239[ebp]
	push	eax
	call	?GetTickCount@COleDateTime@@SG?AV1@XZ	; COleDateTime::GetTickCount
	mov	ecx, eax
	call	?Format@COleDateTime@@QBE?AVCString@@KK@Z ; COleDateTime::Format
	mov	DWORD PTR -52+[ebp], eax
	mov	ecx, DWORD PTR -52+[ebp]
	mov	DWORD PTR -56+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	edx, DWORD PTR -56+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sLogLine$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T77240[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1194 : 		sLogLine += _T(")");

	push	OFFSET FLAT:$SG75798
	lea	ecx, DWORD PTR _sLogLine$[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=
$L75794:

; 1196 : 
; 1197 : 	VERIFY(AppendLineToFile(GetLogFileName(), sLogLine));

	push	2
	lea	eax, DWORD PTR _sLogLine$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77242[ebp]
	push	ecx
	call	?GetLogFileName@FileMisc@@YA?AVCString@@XZ ; FileMisc::GetLogFileName
	add	esp, 4
	mov	DWORD PTR -60+[ebp], eax
	mov	edx, DWORD PTR -60+[ebp]
	mov	DWORD PTR -64+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR -64+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?AppendLineToFile@FileMisc@@YAHPBGABVCString@@W4SFE_SAVEAS@@@Z ; FileMisc::AppendLineToFile
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	SHORT $L77244
	push	1197					; 000004adH
	push	OFFSET FLAT:$SG75806
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	mov	DWORD PTR $T77243[ebp], eax
	cmp	DWORD PTR $T77243[ebp], 0
	je	SHORT $L77244
	mov	DWORD PTR -68+[ebp], 1
	jmp	SHORT $L77245
$L77244:
	mov	DWORD PTR -68+[ebp], 0
$L77245:
	mov	al, BYTE PTR -68+[ebp]
	mov	BYTE PTR $T77241[ebp], al
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T77242[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	ecx, DWORD PTR $T77241[ebp]
	and	ecx, 255				; 000000ffH
	test	ecx, ecx
	je	SHORT $L75804
	int	3
$L75804:
	xor	edx, edx
	test	edx, edx
	jne	$L75794

; 1198 : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sLogLine$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75780:
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 68					; 00000044H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77246:
	lea	ecx, DWORD PTR _sLogLine$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77247:
	lea	ecx, DWORD PTR $T77240[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77248:
	lea	ecx, DWORD PTR $T77242[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77251:
	mov	eax, OFFSET FLAT:$T77250
	jmp	___CxxFrameHandler
text$x	ENDS
?LogText@FileMisc@@YAXPBGZZ ENDP			; FileMisc::LogText
PUBLIC	??1CStdioFileEx@@UAE@XZ				; CStdioFileEx::~CStdioFileEx
EXTRN	??0CStdioFileEx@@QAE@XZ:NEAR			; CStdioFileEx::CStdioFileEx
EXTRN	?Open@CStdioFileEx@@UAEHPBGIW4SFE_SAVEAS@@PAVCFileException@@@Z:NEAR ; CStdioFileEx::Open
EXTRN	?WriteString@CStdioFileEx@@UAEXPBG@Z:NEAR	; CStdioFileEx::WriteString
EXTRN	?SeekToEnd@CFile@@QAEKXZ:NEAR			; CFile::SeekToEnd
EXTRN	__chkstk:NEAR
EXTRN	?Flush@CStdioFile@@UAEXXZ:NEAR			; CStdioFile::Flush
EXTRN	?Close@CStdioFile@@UAEXXZ:NEAR			; CStdioFile::Close
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77262	DD	019930520H
	DD	01H
	DD	FLAT:$T77265
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77265	DD	0ffffffffH
	DD	FLAT:$L77260
xdata$x	ENDS
;	COMDAT ?AppendLineToFile@FileMisc@@YAHPBGABVCString@@W4SFE_SAVEAS@@@Z
_TEXT	SEGMENT
_szPathname$ = 8
_sLine$ = 12
_nSaveAs$ = 16
_file$ = -12344
$T77258 = -12348
$T77259 = -12352
__$EHRec$ = -12
?AppendLineToFile@FileMisc@@YAHPBGABVCString@@W4SFE_SAVEAS@@@Z PROC NEAR ; FileMisc::AppendLineToFile, COMDAT

; 1201 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77263
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	mov	eax, 12340				; 00003034H
	call	__chkstk
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-12352]
	mov	ecx, 3085				; 00000c0dH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1202 : 	if (sLine.IsEmpty())

	mov	ecx, DWORD PTR _sLine$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75812

; 1203 : 		return FALSE;

	xor	eax, eax
	jmp	$L75811
$L75812:

; 1204 : 
; 1205 : 	// make sure parent folder exists
; 1206 : 	if (!CreateFolderFromFilePath(szPathname))

	mov	eax, DWORD PTR _szPathname$[ebp]
	push	eax
	call	?CreateFolderFromFilePath@FileMisc@@YAHPBG@Z ; FileMisc::CreateFolderFromFilePath
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75813

; 1207 : 		return FALSE;

	xor	eax, eax
	jmp	$L75811
$L75813:

; 1208 : 
; 1209 : 	CStdioFileEx file;

	lea	ecx, DWORD PTR _file$[ebp]
	call	??0CStdioFileEx@@QAE@XZ			; CStdioFileEx::CStdioFileEx
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1210 : 
; 1211 : 	if (file.Open(szPathname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite, nSaveAs))

	push	0
	mov	ecx, DWORD PTR _nSaveAs$[ebp]
	push	ecx
	push	12290					; 00003002H
	mov	edx, DWORD PTR _szPathname$[ebp]
	push	edx
	lea	ecx, DWORD PTR _file$[ebp]
	call	?Open@CStdioFileEx@@UAEHPBGIW4SFE_SAVEAS@@PAVCFileException@@@Z ; CStdioFileEx::Open
	test	eax, eax
	je	$L75815

; 1213 : 		file.SeekToEnd();

	lea	ecx, DWORD PTR _file$[ebp]
	call	?SeekToEnd@CFile@@QAEKXZ		; CFile::SeekToEnd

; 1214 : 		file.WriteString(sLine);

	mov	ecx, DWORD PTR _sLine$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _file$[ebp]
	call	?WriteString@CStdioFileEx@@UAEXPBG@Z	; CStdioFileEx::WriteString

; 1215 : 
; 1216 : 		// add trailing new line as necessary
; 1217 : 		if (sLine.Find(ENDL) != (sLine.GetLength() - 2))

	mov	eax, DWORD PTR _ENDL
	push	eax
	mov	ecx, DWORD PTR _sLine$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	mov	esi, eax
	mov	ecx, DWORD PTR _sLine$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	sub	eax, 2
	cmp	esi, eax
	je	SHORT $L75816

; 1218 : 			file.WriteString(ENDL);

	mov	ecx, DWORD PTR _ENDL
	push	ecx
	lea	ecx, DWORD PTR _file$[ebp]
	call	?WriteString@CStdioFileEx@@UAEXPBG@Z	; CStdioFileEx::WriteString
$L75816:

; 1219 : 
; 1220 : 		file.Flush();

	lea	ecx, DWORD PTR _file$[ebp]
	call	?Flush@CStdioFile@@UAEXXZ		; CStdioFile::Flush

; 1221 : 		file.Close();

	lea	ecx, DWORD PTR _file$[ebp]
	call	?Close@CStdioFile@@UAEXXZ		; CStdioFile::Close

; 1222 : 
; 1223 : 		return TRUE;

	mov	DWORD PTR $T77258[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	mov	eax, DWORD PTR $T77258[ebp]
	jmp	SHORT $L75811
$L75815:

; 1225 : 
; 1226 : 	return FALSE;

	mov	DWORD PTR $T77259[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	mov	eax, DWORD PTR $T77259[ebp]
$L75811:

; 1227 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 12352				; 00003040H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77260:
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	ret	0
$L77263:
	mov	eax, OFFSET FLAT:$T77262
	jmp	___CxxFrameHandler
text$x	ENDS
?AppendLineToFile@FileMisc@@YAHPBGABVCString@@W4SFE_SAVEAS@@@Z ENDP ; FileMisc::AppendLineToFile
EXTRN	??1CStdioFile@@UAE@XZ:NEAR			; CStdioFile::~CStdioFile
;	COMDAT ??1CStdioFileEx@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CStdioFileEx@@UAE@XZ PROC NEAR			; CStdioFileEx::~CStdioFileEx, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CStdioFile@@UAE@XZ			; CStdioFile::~CStdioFile
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CStdioFileEx@@UAE@XZ ENDP				; CStdioFileEx::~CStdioFileEx
_TEXT	ENDS
PUBLIC	?LoadFileLines@FileMisc@@YAHPBGAAVCStringArray@@H@Z ; FileMisc::LoadFileLines
EXTRN	?ReadString@CStdioFileEx@@UAEHAAVCString@@@Z:NEAR ; CStdioFileEx::ReadString
EXTRN	?GetLength@CFile@@UBEKXZ:NEAR			; CFile::GetLength
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77277	DD	019930520H
	DD	02H
	DD	FLAT:$T77279
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77279	DD	0ffffffffH
	DD	FLAT:$L77274
	DD	00H
	DD	FLAT:$L77275
xdata$x	ENDS
;	COMDAT ?LoadFileLines@FileMisc@@YAHPBGAAVCStringArray@@H@Z
_TEXT	SEGMENT
_szPathname$ = 8
_aLines$ = 12
_nLineCount$ = 16
_nLinesRead$ = -12348
_file$ = -12344
_sLine$75831 = -12352
$T77273 = -12356
__$EHRec$ = -12
?LoadFileLines@FileMisc@@YAHPBGAAVCStringArray@@H@Z PROC NEAR ; FileMisc::LoadFileLines, COMDAT

; 1230 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77278
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	mov	eax, 12344				; 00003038H
	call	__chkstk
	push	edi
	lea	edi, DWORD PTR [ebp-12356]
	mov	ecx, 3086				; 00000c0eH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1231 : 	int nLinesRead = 0;

	mov	DWORD PTR _nLinesRead$[ebp], 0

; 1232 : 	aLines.RemoveAll();

	mov	ecx, DWORD PTR _aLines$[ebp]
	call	?RemoveAll@CStringArray@@QAEXXZ		; CStringArray::RemoveAll

; 1233 : 
; 1234 : 	CStdioFileEx file;

	lea	ecx, DWORD PTR _file$[ebp]
	call	??0CStdioFileEx@@QAE@XZ			; CStdioFileEx::CStdioFileEx
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1235 : 
; 1236 : 	if (file.Open(szPathname, CFile::modeRead | CFile::shareDenyWrite))

	push	0
	push	2
	push	32					; 00000020H
	mov	eax, DWORD PTR _szPathname$[ebp]
	push	eax
	lea	ecx, DWORD PTR _file$[ebp]
	call	?Open@CStdioFileEx@@UAEHPBGIW4SFE_SAVEAS@@PAVCFileException@@@Z ; CStdioFileEx::Open
	test	eax, eax
	je	SHORT $L75829

; 1238 : 		if (file.GetLength())

	lea	ecx, DWORD PTR _file$[ebp]
	call	?GetLength@CFile@@UBEKXZ		; CFile::GetLength
	test	eax, eax
	je	SHORT $L75829

; 1240 : 			if (nLineCount == -1)

	cmp	DWORD PTR _nLineCount$[ebp], -1
	jne	SHORT $L75830

; 1241 : 				nLineCount = INT_MAX;

	mov	DWORD PTR _nLineCount$[ebp], 2147483647	; 7fffffffH
$L75830:

; 1242 : 
; 1243 : 			// read lines
; 1244 : 			CString sLine;

	lea	ecx, DWORD PTR _sLine$75831[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
$L75833:

; 1245 : 			while (file.ReadString(sLine) && nLinesRead < nLineCount)

	lea	ecx, DWORD PTR _sLine$75831[ebp]
	push	ecx
	lea	ecx, DWORD PTR _file$[ebp]
	call	?ReadString@CStdioFileEx@@UAEHAAVCString@@@Z ; CStdioFileEx::ReadString
	test	eax, eax
	je	SHORT $L75834
	mov	edx, DWORD PTR _nLinesRead$[ebp]
	cmp	edx, DWORD PTR _nLineCount$[ebp]
	jge	SHORT $L75834

; 1247 : 				aLines.Add(sLine);

	lea	eax, DWORD PTR _sLine$75831[ebp]
	push	eax
	mov	ecx, DWORD PTR _aLines$[ebp]
	call	?Add@CStringArray@@QAEHABVCString@@@Z	; CStringArray::Add

; 1248 : 				nLinesRead++;

	mov	ecx, DWORD PTR _nLinesRead$[ebp]
	add	ecx, 1
	mov	DWORD PTR _nLinesRead$[ebp], ecx

; 1249 : 			}

	jmp	SHORT $L75833
$L75834:

; 1250 : 		}

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sLine$75831[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75829:

; 1252 : 
; 1253 : 	return nLinesRead;

	mov	edx, DWORD PTR _nLinesRead$[ebp]
	mov	DWORD PTR $T77273[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	mov	eax, DWORD PTR $T77273[ebp]

; 1254 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 12356				; 00003044H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77274:
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	ret	0
$L77275:
	lea	ecx, DWORD PTR _sLine$75831[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77278:
	mov	eax, OFFSET FLAT:$T77277
	jmp	___CxxFrameHandler
text$x	ENDS
?LoadFileLines@FileMisc@@YAHPBGAAVCStringArray@@H@Z ENDP ; FileMisc::LoadFileLines
PUBLIC	?CanReadFile@FileMisc@@YAHPBGH@Z		; FileMisc::CanReadFile
EXTRN	?Open@CFile@@UAEHPBGIPAVCFileException@@@Z:NEAR	; CFile::Open
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77287	DD	019930520H
	DD	01H
	DD	FLAT:$T77289
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77289	DD	0ffffffffH
	DD	FLAT:$L77285
xdata$x	ENDS
;	COMDAT ?CanReadFile@FileMisc@@YAHPBGH@Z
_TEXT	SEGMENT
_szPathname$ = 8
_bDenyWrite$ = 12
_file$ = -32
_nFlags$ = -16
$T77284 = -36
__$EHRec$ = -12
?CanReadFile@FileMisc@@YAHPBGH@Z PROC NEAR		; FileMisc::CanReadFile, COMDAT

; 1257 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77288
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 1258 : 	CFile file;

	lea	ecx, DWORD PTR _file$[ebp]
	call	??0CFile@@QAE@XZ			; CFile::CFile
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1259 : 	UINT nFlags = CFile::modeRead | (bDenyWrite ? CFile::shareDenyWrite : CFile::shareDenyNone);

	mov	eax, DWORD PTR _bDenyWrite$[ebp]
	neg	eax
	sbb	eax, eax
	and	al, -32					; ffffffe0H
	add	eax, 64					; 00000040H
	mov	DWORD PTR _nFlags$[ebp], eax

; 1260 : 
; 1261 : 	return (file.Open(szPathname, nFlags) != FALSE);

	push	0
	mov	ecx, DWORD PTR _nFlags$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szPathname$[ebp]
	push	edx
	lea	ecx, DWORD PTR _file$[ebp]
	call	?Open@CFile@@UAEHPBGIPAVCFileException@@@Z ; CFile::Open
	neg	eax
	sbb	eax, eax
	neg	eax
	mov	DWORD PTR $T77284[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CFile@@UAE@XZ			; CFile::~CFile
	mov	eax, DWORD PTR $T77284[ebp]

; 1262 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77285:
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CFile@@UAE@XZ			; CFile::~CFile
	ret	0
$L77288:
	mov	eax, OFFSET FLAT:$T77287
	jmp	___CxxFrameHandler
text$x	ENDS
?CanReadFile@FileMisc@@YAHPBGH@Z ENDP			; FileMisc::CanReadFile
PUBLIC	?SaveFile@FileMisc@@YAHPBGABVCString@@W4SFE_SAVEAS@@@Z ; FileMisc::SaveFile
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77298	DD	019930520H
	DD	01H
	DD	FLAT:$T77300
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77300	DD	0ffffffffH
	DD	FLAT:$L77296
xdata$x	ENDS
;	COMDAT ?SaveFile@FileMisc@@YAHPBGABVCString@@W4SFE_SAVEAS@@@Z
_TEXT	SEGMENT
_szPathname$ = 8
_sText$ = 12
_nSaveAs$ = 16
_file$ = -12344
$T77294 = -12348
$T77295 = -12352
__$EHRec$ = -12
?SaveFile@FileMisc@@YAHPBGABVCString@@W4SFE_SAVEAS@@@Z PROC NEAR ; FileMisc::SaveFile, COMDAT

; 1265 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77299
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	mov	eax, 12340				; 00003034H
	call	__chkstk
	push	edi
	lea	edi, DWORD PTR [ebp-12352]
	mov	ecx, 3085				; 00000c0dH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1266 : 	CStdioFileEx file;

	lea	ecx, DWORD PTR _file$[ebp]
	call	??0CStdioFileEx@@QAE@XZ			; CStdioFileEx::CStdioFileEx
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1267 : 
; 1268 : 	if (file.Open(szPathname, CFile::modeCreate | CFile::modeWrite, nSaveAs))

	push	0
	mov	eax, DWORD PTR _nSaveAs$[ebp]
	push	eax
	push	4097					; 00001001H
	mov	ecx, DWORD PTR _szPathname$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _file$[ebp]
	call	?Open@CStdioFileEx@@UAEHPBGIW4SFE_SAVEAS@@PAVCFileException@@@Z ; CStdioFileEx::Open
	test	eax, eax
	je	SHORT $L75849

; 1270 : 		file.WriteString(sText);

	mov	ecx, DWORD PTR _sText$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _file$[ebp]
	call	?WriteString@CStdioFileEx@@UAEXPBG@Z	; CStdioFileEx::WriteString

; 1271 : 		return TRUE;

	mov	DWORD PTR $T77294[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	mov	eax, DWORD PTR $T77294[ebp]
	jmp	SHORT $L75847
$L75849:

; 1273 : 
; 1274 : 	return FALSE;

	mov	DWORD PTR $T77295[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	mov	eax, DWORD PTR $T77295[ebp]
$L75847:

; 1275 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 12352				; 00003040H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77296:
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	ret	0
$L77299:
	mov	eax, OFFSET FLAT:$T77298
	jmp	___CxxFrameHandler
text$x	ENDS
?SaveFile@FileMisc@@YAHPBGABVCString@@W4SFE_SAVEAS@@@Z ENDP ; FileMisc::SaveFile
PUBLIC	?SaveFile@FileMisc@@YAHPBGPBD@Z			; FileMisc::SaveFile
EXTRN	??0CStdioFile@@QAE@XZ:NEAR			; CStdioFile::CStdioFile
EXTRN	?Open@CStdioFile@@UAEHPBGIPAVCFileException@@@Z:NEAR ; CStdioFile::Open
EXTRN	?Write@CStdioFile@@UAEXPBXI@Z:NEAR		; CStdioFile::Write
EXTRN	_strlen:NEAR
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77309	DD	019930520H
	DD	01H
	DD	FLAT:$T77311
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77311	DD	0ffffffffH
	DD	FLAT:$L77307
xdata$x	ENDS
;	COMDAT ?SaveFile@FileMisc@@YAHPBGPBD@Z
_TEXT	SEGMENT
_szPathname$ = 8
_szText$ = 12
_file$ = -32
$T77305 = -36
$T77306 = -40
__$EHRec$ = -12
?SaveFile@FileMisc@@YAHPBGPBD@Z PROC NEAR		; FileMisc::SaveFile, COMDAT

; 1278 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77310
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 28					; 0000001cH
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 1279 : 	CStdioFile file;

	lea	ecx, DWORD PTR _file$[ebp]
	call	??0CStdioFile@@QAE@XZ			; CStdioFile::CStdioFile
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1280 : 
; 1281 : 	if (file.Open(szPathname, CFile::modeCreate | CFile::modeWrite))

	push	0
	push	4097					; 00001001H
	mov	eax, DWORD PTR _szPathname$[ebp]
	push	eax
	lea	ecx, DWORD PTR _file$[ebp]
	call	?Open@CStdioFile@@UAEHPBGIPAVCFileException@@@Z ; CStdioFile::Open
	test	eax, eax
	je	SHORT $L75857

; 1283 : 		file.Write(szText, strlen(szText));

	mov	ecx, DWORD PTR _szText$[ebp]
	push	ecx
	call	_strlen
	add	esp, 4
	push	eax
	mov	edx, DWORD PTR _szText$[ebp]
	push	edx
	lea	ecx, DWORD PTR _file$[ebp]
	call	?Write@CStdioFile@@UAEXPBXI@Z		; CStdioFile::Write

; 1284 : 		return TRUE;

	mov	DWORD PTR $T77305[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFile@@UAE@XZ			; CStdioFile::~CStdioFile
	mov	eax, DWORD PTR $T77305[ebp]
	jmp	SHORT $L75855
$L75857:

; 1286 : 
; 1287 : 	return FALSE;

	mov	DWORD PTR $T77306[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFile@@UAE@XZ			; CStdioFile::~CStdioFile
	mov	eax, DWORD PTR $T77306[ebp]
$L75855:

; 1288 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77307:
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFile@@UAE@XZ			; CStdioFile::~CStdioFile
	ret	0
$L77310:
	mov	eax, OFFSET FLAT:$T77309
	jmp	___CxxFrameHandler
text$x	ENDS
?SaveFile@FileMisc@@YAHPBGPBD@Z ENDP			; FileMisc::SaveFile
PUBLIC	?LoadFile@FileMisc@@YAHPBGAAVCString@@H@Z	; FileMisc::LoadFile
EXTRN	?ReadFile@CStdioFileEx@@QAEHAAVCString@@@Z:NEAR	; CStdioFileEx::ReadFile
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77320	DD	019930520H
	DD	01H
	DD	FLAT:$T77322
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77322	DD	0ffffffffH
	DD	FLAT:$L77318
xdata$x	ENDS
;	COMDAT ?LoadFile@FileMisc@@YAHPBGAAVCString@@H@Z
_TEXT	SEGMENT
_szPathname$ = 8
_sText$ = 12
_bDenyWrite$ = 16
_file$ = -12348
_nFlags$ = -16
$T77316 = -12352
$T77317 = -12356
__$EHRec$ = -12
?LoadFile@FileMisc@@YAHPBGAAVCString@@H@Z PROC NEAR	; FileMisc::LoadFile, COMDAT

; 1291 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77321
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	mov	eax, 12344				; 00003038H
	call	__chkstk
	push	edi
	lea	edi, DWORD PTR [ebp-12356]
	mov	ecx, 3086				; 00000c0eH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1292 : 	CStdioFileEx file;

	lea	ecx, DWORD PTR _file$[ebp]
	call	??0CStdioFileEx@@QAE@XZ			; CStdioFileEx::CStdioFileEx
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1293 : 	UINT nFlags = CFile::modeRead | (bDenyWrite ? CFile::shareDenyWrite : CFile::shareDenyNone);

	mov	eax, DWORD PTR _bDenyWrite$[ebp]
	neg	eax
	sbb	eax, eax
	and	al, -32					; ffffffe0H
	add	eax, 64					; 00000040H
	mov	DWORD PTR _nFlags$[ebp], eax

; 1294 : 
; 1295 : 	if (file.Open(szPathname, nFlags))

	push	0
	push	2
	mov	ecx, DWORD PTR _nFlags$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szPathname$[ebp]
	push	edx
	lea	ecx, DWORD PTR _file$[ebp]
	call	?Open@CStdioFileEx@@UAEHPBGIW4SFE_SAVEAS@@PAVCFileException@@@Z ; CStdioFileEx::Open
	test	eax, eax
	je	SHORT $L75867

; 1297 : 		return (file.ReadFile(sText) != FALSE);

	mov	eax, DWORD PTR _sText$[ebp]
	push	eax
	lea	ecx, DWORD PTR _file$[ebp]
	call	?ReadFile@CStdioFileEx@@QAEHAAVCString@@@Z ; CStdioFileEx::ReadFile
	neg	eax
	sbb	eax, eax
	neg	eax
	mov	DWORD PTR $T77316[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	mov	eax, DWORD PTR $T77316[ebp]
	jmp	SHORT $L75864
$L75867:

; 1299 : 
; 1300 : 	return FALSE;

	mov	DWORD PTR $T77317[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	mov	eax, DWORD PTR $T77317[ebp]
$L75864:

; 1301 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 12356				; 00003044H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77318:
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	ret	0
$L77321:
	mov	eax, OFFSET FLAT:$T77320
	jmp	___CxxFrameHandler
text$x	ENDS
?LoadFile@FileMisc@@YAHPBGAAVCString@@H@Z ENDP		; FileMisc::LoadFile
PUBLIC	?IsTempFile@FileMisc@@YAHPBG@Z			; FileMisc::IsTempFile
EXTRN	?MakeLower@CString@@QAEXXZ:NEAR			; CString::MakeLower
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77331	DD	019930520H
	DD	02H
	DD	FLAT:$T77333
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77333	DD	0ffffffffH
	DD	FLAT:$L77328
	DD	00H
	DD	FLAT:$L77329
xdata$x	ENDS
;	COMDAT ?IsTempFile@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT
_szFilename$ = 8
_sFilename$ = -20
_sTempFolder$ = -16
$T77327 = -24
__$EHRec$ = -12
?IsTempFile@FileMisc@@YAHPBG@Z PROC NEAR		; FileMisc::IsTempFile, COMDAT

; 1304 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77332
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH

; 1305 : 	CString sFilename(szFilename);

	mov	eax, DWORD PTR _szFilename$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFilename$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1306 : 	sFilename.MakeLower();

	lea	ecx, DWORD PTR _sFilename$[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower

; 1307 : 
; 1308 : 	CString sTempFolder = GetTempFolder();

	lea	ecx, DWORD PTR _sTempFolder$[ebp]
	push	ecx
	call	?GetTempFolder@FileMisc@@YA?AVCString@@XZ ; FileMisc::GetTempFolder
	add	esp, 4
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 1309 : 	sTempFolder.MakeLower();

	lea	ecx, DWORD PTR _sTempFolder$[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower

; 1310 : 
; 1311 : 	return (sFilename.Find(sTempFolder) == 0);

	lea	ecx, DWORD PTR _sTempFolder$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sFilename$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR $T77327[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sTempFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sFilename$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77327[ebp]

; 1312 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77328:
	lea	ecx, DWORD PTR _sFilename$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77329:
	lea	ecx, DWORD PTR _sTempFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77332:
	mov	eax, OFFSET FLAT:$T77331
	jmp	___CxxFrameHandler
text$x	ENDS
?IsTempFile@FileMisc@@YAHPBG@Z ENDP			; FileMisc::IsTempFile
EXTRN	__imp__GetTempPathW@8:NEAR
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75883 DB	'C', 00H, ':', 00H, '\', 00H, 'T', 00H, 'e', 00H, 'm', 00H
	DB	'p', 00H, 00H, 00H
data	ENDS
;	COMDAT ?GetTempFolder@FileMisc@@YA?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_szTempPath$ = -524
$T77339 = -528
?GetTempFolder@FileMisc@@YA?AVCString@@XZ PROC NEAR	; FileMisc::GetTempFolder, COMDAT

; 1315 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 528				; 00000210H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-528]
	mov	ecx, 132				; 00000084H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T77339[ebp], 0

; 1316 : 	TCHAR szTempPath[MAX_PATH+1] = { 0 };

	mov	WORD PTR _szTempPath$[ebp], 0
	mov	ecx, 130				; 00000082H
	xor	eax, eax
	lea	edi, DWORD PTR _szTempPath$[ebp+2]
	rep stosd

; 1317 : 	
; 1318 : 	if (::GetTempPath(MAX_PATH, szTempPath))

	mov	esi, esp
	lea	eax, DWORD PTR _szTempPath$[ebp]
	push	eax
	push	260					; 00000104H
	call	DWORD PTR __imp__GetTempPathW@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L75881

; 1319 : 		return CString(szTempPath);

	lea	ecx, DWORD PTR _szTempPath$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	edx, DWORD PTR $T77339[ebp]
	or	edx, 1
	mov	DWORD PTR $T77339[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L75879
$L75881:

; 1320 : 
; 1321 : 	// else
; 1322 : 	return _T("C:\\Temp");

	push	OFFSET FLAT:$SG75883
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	eax, DWORD PTR $T77339[ebp]
	or	al, 1
	mov	DWORD PTR $T77339[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L75879:

; 1323 : }

	pop	edi
	pop	esi
	add	esp, 528				; 00000210H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetTempFolder@FileMisc@@YA?AVCString@@XZ ENDP		; FileMisc::GetTempFolder
_TEXT	ENDS
PUBLIC	?GetTempFileNameW@FileMisc@@YA?AVCString@@PBGI@Z ; FileMisc::GetTempFileNameW
EXTRN	__imp__GetTempFileNameW@16:NEAR
_BSS	SEGMENT
	ALIGN	4

$SG75893 DB	01H DUP (?)
_BSS	ENDS
;	COMDAT ?GetTempFileNameW@FileMisc@@YA?AVCString@@PBGI@Z
_TEXT	SEGMENT
_szPrefix$ = 12
_uUnique$ = 16
___$ReturnUdt$ = 8
_szTempFile$ = -1048
_szTempPath$ = -524
$T77345 = -1052
?GetTempFileNameW@FileMisc@@YA?AVCString@@PBGI@Z PROC NEAR ; FileMisc::GetTempFileNameW, COMDAT

; 1326 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 1052				; 0000041cH
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-1052]
	mov	ecx, 263				; 00000107H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T77345[ebp], 0

; 1327 : 	TCHAR szTempFile[MAX_PATH+1] = { 0 }, szTempPath[MAX_PATH+1] = { 0 };

	mov	WORD PTR _szTempFile$[ebp], 0
	mov	ecx, 130				; 00000082H
	xor	eax, eax
	lea	edi, DWORD PTR _szTempFile$[ebp+2]
	rep stosd
	mov	WORD PTR _szTempPath$[ebp], 0
	mov	ecx, 130				; 00000082H
	xor	eax, eax
	lea	edi, DWORD PTR _szTempPath$[ebp+2]
	rep stosd

; 1328 : 	
; 1329 : 	if (::GetTempPath(MAX_PATH, szTempPath))

	mov	esi, esp
	lea	eax, DWORD PTR _szTempPath$[ebp]
	push	eax
	push	260					; 00000104H
	call	DWORD PTR __imp__GetTempPathW@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L75892

; 1331 : 		if (::GetTempFileName(szTempPath, szPrefix, uUnique, szTempFile))

	mov	esi, esp
	lea	ecx, DWORD PTR _szTempFile$[ebp]
	push	ecx
	mov	edx, DWORD PTR _uUnique$[ebp]
	push	edx
	mov	eax, DWORD PTR _szPrefix$[ebp]
	push	eax
	lea	ecx, DWORD PTR _szTempPath$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetTempFileNameW@16
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L75892

; 1332 : 			return szTempFile;

	lea	edx, DWORD PTR _szTempFile$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	eax, DWORD PTR $T77345[ebp]
	or	al, 1
	mov	DWORD PTR $T77345[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L75888
$L75892:

; 1334 : 
; 1335 : 	return "";

	push	OFFSET FLAT:$SG75893
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	ecx, DWORD PTR $T77345[ebp]
	or	ecx, 1
	mov	DWORD PTR $T77345[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L75888:

; 1336 : }

	pop	edi
	pop	esi
	add	esp, 1052				; 0000041cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetTempFileNameW@FileMisc@@YA?AVCString@@PBGI@Z ENDP	; FileMisc::GetTempFileNameW
_TEXT	ENDS
PUBLIC	?GetTempFileNameW@FileMisc@@YA?AVCString@@PBG0@Z ; FileMisc::GetTempFileNameW
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77354	DD	019930520H
	DD	02H
	DD	FLAT:$T77356
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77356	DD	0ffffffffH
	DD	FLAT:$L77350
	DD	00H
	DD	FLAT:$L77349
xdata$x	ENDS
;	COMDAT ?GetTempFileNameW@FileMisc@@YA?AVCString@@PBG0@Z
_TEXT	SEGMENT
_szFilename$ = 12
_szExt$ = 16
___$ReturnUdt$ = 8
_sTempFile$ = -16
_szTempPath$ = -540
$T77351 = -544
__$EHRec$ = -12
?GetTempFileNameW@FileMisc@@YA?AVCString@@PBG0@Z PROC NEAR ; FileMisc::GetTempFileNameW, COMDAT

; 1339 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77355
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 532				; 00000214H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-544]
	mov	ecx, 133				; 00000085H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T77351[ebp], 0

; 1340 : 	CString sTempFile;

	lea	ecx, DWORD PTR _sTempFile$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1341 : 	TCHAR szTempPath[MAX_PATH+1] = { 0 };

	mov	WORD PTR _szTempPath$[ebp], 0
	mov	ecx, 130				; 00000082H
	xor	eax, eax
	lea	edi, DWORD PTR _szTempPath$[ebp+2]
	rep stosd

; 1342 : 	
; 1343 : 	if (::GetTempPath(MAX_PATH, szTempPath))

	mov	esi, esp
	lea	eax, DWORD PTR _szTempPath$[ebp]
	push	eax
	push	260					; 00000104H
	call	DWORD PTR __imp__GetTempPathW@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L75901

; 1344 : 		MakePath(sTempFile, NULL, szTempPath, szFilename, szExt);

	mov	ecx, DWORD PTR _szExt$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szFilename$[ebp]
	push	edx
	lea	eax, DWORD PTR _szTempPath$[ebp]
	push	eax
	push	0
	lea	ecx, DWORD PTR _sTempFile$[ebp]
	push	ecx
	call	?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z ; FileMisc::MakePath
	add	esp, 20					; 00000014H
$L75901:

; 1345 : 
; 1346 : 	return sTempFile;

	lea	edx, DWORD PTR _sTempFile$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T77351[ebp]
	or	al, 1
	mov	DWORD PTR $T77351[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sTempFile$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1347 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 544				; 00000220H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77349:
	lea	ecx, DWORD PTR _sTempFile$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77350:
	mov	eax, DWORD PTR $T77351[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77352
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77352:
	ret	0
$L77355:
	mov	eax, OFFSET FLAT:$T77354
	jmp	___CxxFrameHandler
text$x	ENDS
?GetTempFileNameW@FileMisc@@YA?AVCString@@PBG0@Z ENDP	; FileMisc::GetTempFileNameW
PUBLIC	?GetWindowsFolder@FileMisc@@YA?AVCString@@XZ	; FileMisc::GetWindowsFolder
PUBLIC	?Run@FileMisc@@YAKPAUHWND__@@PBG1H11@Z		; FileMisc::Run
PUBLIC	?SelectFileInExplorer@FileMisc@@YAHPBG@Z	; FileMisc::SelectFileInExplorer
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75906 DB	'/', 00H, 'n', 00H, ',', 00H, '/', 00H, 's', 00H, 'e', 00H
	DB	'l', 00H, 'e', 00H, 'c', 00H, 't', 00H, ',', 00H, '"', 00H, '%'
	DB	00H, 's', 00H, '"', 00H, 00H, 00H
$SG75909 DB	'%', 00H, 's', 00H, '\', 00H, 'E', 00H, 'x', 00H, 'p', 00H
	DB	'l', 00H, 'o', 00H, 'r', 00H, 'e', 00H, 'r', 00H, '.', 00H, 'e'
	DB	00H, 'x', 00H, 'e', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77367	DD	019930520H
	DD	03H
	DD	FLAT:$T77369
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77369	DD	0ffffffffH
	DD	FLAT:$L77363
	DD	00H
	DD	FLAT:$L77364
	DD	01H
	DD	FLAT:$L77365
xdata$x	ENDS
;	COMDAT ?SelectFileInExplorer@FileMisc@@YAHPBG@Z
_TEXT	SEGMENT
_szFilePath$ = 8
_sFile$ = -20
_sExplorer$ = -16
$T77361 = -24
$T77362 = -28
__$EHRec$ = -12
?SelectFileInExplorer@FileMisc@@YAHPBG@Z PROC NEAR	; FileMisc::SelectFileInExplorer, COMDAT

; 1350 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77368
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 1351 : 	CString sFile;

	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1352 : 	sFile.Format(_T("/n,/select,\"%s\""), szFilePath);

	mov	eax, DWORD PTR _szFilePath$[ebp]
	push	eax
	push	OFFSET FLAT:$SG75906
	lea	ecx, DWORD PTR _sFile$[ebp]
	push	ecx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 12					; 0000000cH

; 1353 : 	
; 1354 : 	CString sExplorer;

	lea	ecx, DWORD PTR _sExplorer$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 1355 : 	sExplorer.Format(_T("%s\\Explorer.exe"), GetWindowsFolder());

	lea	edx, DWORD PTR $T77361[ebp]
	push	edx
	call	?GetWindowsFolder@FileMisc@@YA?AVCString@@XZ ; FileMisc::GetWindowsFolder
	add	esp, 4
	mov	DWORD PTR -32+[ebp], eax
	mov	eax, DWORD PTR -32+[ebp]
	mov	DWORD PTR -36+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR -36+[ebp]
	mov	edx, DWORD PTR [ecx]
	push	edx
	push	OFFSET FLAT:$SG75909
	lea	eax, DWORD PTR _sExplorer$[ebp]
	push	eax
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 12					; 0000000cH
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T77361[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1356 : 	
; 1357 : 	return (Run(NULL, sExplorer, sFile) > 32);

	push	0
	push	0
	push	1
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sExplorer$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	0
	call	?Run@FileMisc@@YAKPAUHWND__@@PBG1H11@Z	; FileMisc::Run
	add	esp, 24					; 00000018H
	mov	ecx, 32					; 00000020H
	cmp	ecx, eax
	sbb	edx, edx
	neg	edx
	mov	DWORD PTR $T77362[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sExplorer$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77362[ebp]

; 1358 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77363:
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77364:
	lea	ecx, DWORD PTR _sExplorer$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77365:
	lea	ecx, DWORD PTR $T77361[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77368:
	mov	eax, OFFSET FLAT:$T77367
	jmp	___CxxFrameHandler
text$x	ENDS
?SelectFileInExplorer@FileMisc@@YAHPBG@Z ENDP		; FileMisc::SelectFileInExplorer
PUBLIC	?HasExtension@FileMisc@@YAHPBG0@Z		; FileMisc::HasExtension
EXTRN	__imp__ShellExecuteW@24:NEAR
EXTRN	__imp__CreateProcessW@40:NEAR
EXTRN	_memset:NEAR
EXTRN	?Empty@CString@@QAEXXZ:NEAR			; CString::Empty
EXTRN	?GetRegisteredAppPath@CFileRegister@@SA?AVCString@@PBGH@Z:NEAR ; CFileRegister::GetRegisteredAppPath
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75933 DB	'.', 00H, 'e', 00H, 'x', 00H, 'e', 00H, 00H, 00H
	ORG $+2
$SG75937 DB	'%', 00H, 's', 00H, ' ', 00H, '"', 00H, '%', 00H, 's', 00H
	DB	'"', 00H, 00H, 00H
$SG75942 DB	'S', 00H, 'h', 00H, 'e', 00H, 'l', 00H, 'l', 00H, 'E', 00H
	DB	'x', 00H, 'e', 00H, 'c', 00H, 'u', 00H, 't', 00H, 'e', 00H, '('
	DB	00H, '%', 00H, 's', 00H, ':', 00H, ' ', 00H, '%', 00H, 's', 00H
	DB	')', 00H, ' ', 00H, 'f', 00H, 'a', 00H, 'i', 00H, 'l', 00H, 'e'
	DB	00H, 'd', 00H, '.', 00H, ' ', 00H, 'R', 00H, 'e', 00H, 't', 00H
	DB	'C', 00H, 'o', 00H, 'd', 00H, 'e', 00H, ' ', 00H, '=', 00H, ' '
	DB	00H, '%', 00H, 'l', 00H, 'd', 00H, 00H, 00H
	ORG $+2
$SG75944 DB	'S', 00H, 'h', 00H, 'e', 00H, 'l', 00H, 'l', 00H, 'E', 00H
	DB	'x', 00H, 'e', 00H, 'c', 00H, 'u', 00H, 't', 00H, 'e', 00H, '('
	DB	00H, '%', 00H, 's', 00H, ')', 00H, ' ', 00H, 'f', 00H, 'a', 00H
	DB	'i', 00H, 'l', 00H, 'e', 00H, 'd', 00H, '.', 00H, ' ', 00H, 'R'
	DB	00H, 'e', 00H, 't', 00H, 'C', 00H, 'o', 00H, 'd', 00H, 'e', 00H
	DB	' ', 00H, '=', 00H, ' ', 00H, '%', 00H, 'l', 00H, 'd', 00H, 00H
	DB	00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77385	DD	019930520H
	DD	06H
	DD	FLAT:$T77387
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77387	DD	0ffffffffH
	DD	FLAT:$L77378
	DD	00H
	DD	FLAT:$L77379
	DD	01H
	DD	FLAT:$L77380
	DD	01H
	DD	FLAT:$L77381
	DD	01H
	DD	FLAT:$L77382
	DD	01H
	DD	FLAT:$L77383
xdata$x	ENDS
;	COMDAT ?Run@FileMisc@@YAKPAUHWND__@@PBG1H11@Z
_TEXT	SEGMENT
_hwnd$ = 8
_lpFile$ = 12
_lpParams$ = 16
_nShowCmd$ = 20
_lpDirectory$ = 24
_lpVerb$ = 28
_sFile$ = -24
_sParams$ = -16
_nHash$75922 = -28
_sApp$75926 = -32
_sApp$75934 = -36
_dwRes$ = -20
_si$75945 = -120
_pi$75946 = -52
$T77375 = -124
$T77376 = -128
$T77377 = -132
__$EHRec$ = -12
?Run@FileMisc@@YAKPAUHWND__@@PBG1H11@Z PROC NEAR	; FileMisc::Run, COMDAT

; 1361 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77386
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 136				; 00000088H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-148]
	mov	ecx, 34					; 00000022H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1362 : 	CString sFile(lpFile), sParams(lpParams);

	mov	eax, DWORD PTR _lpFile$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _lpParams$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sParams$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 1363 : 
; 1364 : 	if (lpParams == NULL)

	cmp	DWORD PTR _lpParams$[ebp], 0
	jne	$L75921

; 1366 : 		int nHash = sFile.Find('#');

	push	35					; 00000023H
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	?Find@CString@@QBEHG@Z			; CString::Find
	mov	DWORD PTR _nHash$75922[ebp], eax

; 1367 : 		
; 1368 : 		if (nHash != -1)

	cmp	DWORD PTR _nHash$75922[ebp], -1
	je	$L75923

; 1370 : 			sParams = sFile.Mid(nHash);

	mov	edx, DWORD PTR _nHash$75922[ebp]
	push	edx
	lea	eax, DWORD PTR $T77375[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	?Mid@CString@@QBE?AV1@H@Z		; CString::Mid
	mov	DWORD PTR -136+[ebp], eax
	mov	ecx, DWORD PTR -136+[ebp]
	mov	DWORD PTR -140+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	edx, DWORD PTR -140+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sParams$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T77375[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1371 : 			sFile = sFile.Left(nHash);

	mov	eax, DWORD PTR _nHash$75922[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77376[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	?Left@CString@@QBE?AV1@H@Z		; CString::Left
	mov	DWORD PTR -144+[ebp], eax
	mov	edx, DWORD PTR -144+[ebp]
	mov	DWORD PTR -148+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	eax, DWORD PTR -148+[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T77376[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1372 : 			
; 1373 : 			CString sApp = CFileRegister::GetRegisteredAppPath(sFile);

	push	0
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sApp$75926[ebp]
	push	ecx
	call	?GetRegisteredAppPath@CFileRegister@@SA?AVCString@@PBGH@Z ; CFileRegister::GetRegisteredAppPath
	add	esp, 12					; 0000000cH
	mov	BYTE PTR __$EHRec$[ebp+8], 4

; 1374 : 			
; 1375 : 			if (!sApp.IsEmpty())

	lea	ecx, DWORD PTR _sApp$75926[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75928

; 1377 : 				sFile = sApp;

	lea	edx, DWORD PTR _sApp$75926[ebp]
	push	edx
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=

; 1378 : 				sParams = lpFile;

	mov	eax, DWORD PTR _lpFile$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sParams$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 1380 : 			else

	jmp	SHORT $L75929
$L75928:

; 1382 : 				sFile = lpFile;

	mov	ecx, DWORD PTR _lpFile$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 1383 : 				sParams.Empty();

	lea	ecx, DWORD PTR _sParams$[ebp]
	call	?Empty@CString@@QAEXXZ			; CString::Empty
$L75929:

; 1385 : 		}

	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sApp$75926[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75923:

; 1387 : 	else if (lpParams[0])

	jmp	SHORT $L75932
$L75921:
	mov	edx, DWORD PTR _lpParams$[ebp]
	xor	eax, eax
	mov	ax, WORD PTR [edx]
	test	eax, eax
	je	SHORT $L75932

; 1389 : 		// if params have been specified and the file is 
; 1390 : 		// not an exe, get the registered app path and
; 1391 : 		// pass it the params and filename
; 1392 : 		if (!HasExtension(lpFile, _T(".exe")))

	push	OFFSET FLAT:$SG75933
	mov	ecx, DWORD PTR _lpFile$[ebp]
	push	ecx
	call	?HasExtension@FileMisc@@YAHPBG0@Z	; FileMisc::HasExtension
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75932

; 1394 : 			CString sApp = CFileRegister::GetRegisteredAppPath(lpFile);

	push	0
	mov	edx, DWORD PTR _lpFile$[ebp]
	push	edx
	lea	eax, DWORD PTR _sApp$75934[ebp]
	push	eax
	call	?GetRegisteredAppPath@CFileRegister@@SA?AVCString@@PBGH@Z ; CFileRegister::GetRegisteredAppPath
	add	esp, 12					; 0000000cH
	mov	BYTE PTR __$EHRec$[ebp+8], 5

; 1395 : 			
; 1396 : 			if (FileExists(sApp))

	lea	ecx, DWORD PTR _sApp$75934[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?FileExists@FileMisc@@YAHPBG@Z		; FileMisc::FileExists
	add	esp, 4
	test	eax, eax
	je	SHORT $L75936

; 1398 : 				sParams.Format(_T("%s \"%s\""), lpParams, lpFile); 

	mov	ecx, DWORD PTR _lpFile$[ebp]
	push	ecx
	mov	edx, DWORD PTR _lpParams$[ebp]
	push	edx
	push	OFFSET FLAT:$SG75937
	lea	eax, DWORD PTR _sParams$[ebp]
	push	eax
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 16					; 00000010H

; 1399 : 				sFile = sApp;

	lea	ecx, DWORD PTR _sApp$75934[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
$L75936:

; 1401 : 		}

	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sApp$75934[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75932:

; 1403 : 	
; 1404 : 	DWORD dwRes = (DWORD)ShellExecute(hwnd, lpVerb, sFile, sParams, lpDirectory, nShowCmd);

	mov	esi, esp
	mov	edx, DWORD PTR _nShowCmd$[ebp]
	push	edx
	mov	eax, DWORD PTR _lpDirectory$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sParams$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _lpVerb$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hwnd$[ebp]
	push	edx
	call	DWORD PTR __imp__ShellExecuteW@24
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _dwRes$[ebp], eax

; 1405 : 	
; 1406 : 	if (dwRes <= 32) // failure

	cmp	DWORD PTR _dwRes$[ebp], 32		; 00000020H
	ja	$L75940

; 1408 : 		if (lpVerb)

	cmp	DWORD PTR _lpVerb$[ebp], 0
	je	SHORT $L75941

; 1409 : 			LogText(_T("ShellExecute(%s: %s) failed. RetCode = %ld"), lpFile, lpVerb, dwRes);

	mov	eax, DWORD PTR _dwRes$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpVerb$[ebp]
	push	ecx
	mov	edx, DWORD PTR _lpFile$[ebp]
	push	edx
	push	OFFSET FLAT:$SG75942
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 16					; 00000010H

; 1410 : 		else

	jmp	SHORT $L75943
$L75941:

; 1411 : 			LogText(_T("ShellExecute(%s) failed. RetCode = %ld"), lpFile, dwRes);

	mov	eax, DWORD PTR _dwRes$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpFile$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG75944
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 12					; 0000000cH
$L75943:

; 1412 : 
; 1413 : 		// try CreateProcess
; 1414 : 		STARTUPINFO si;
; 1415 : 		PROCESS_INFORMATION pi;
; 1416 : 
; 1417 : 		ZeroMemory( &si, sizeof(si) );

	push	68					; 00000044H
	push	0
	lea	edx, DWORD PTR _si$75945[ebp]
	push	edx
	call	_memset
	add	esp, 12					; 0000000cH

; 1418 : 		si.cb = sizeof(si);

	mov	DWORD PTR _si$75945[ebp], 68		; 00000044H

; 1419 : 		si.wShowWindow = (WORD)nShowCmd;

	mov	ax, WORD PTR _nShowCmd$[ebp]
	mov	WORD PTR _si$75945[ebp+48], ax

; 1420 : 		si.dwFlags = STARTF_USESHOWWINDOW;

	mov	DWORD PTR _si$75945[ebp+44], 1

; 1421 : 
; 1422 : 		ZeroMemory( &pi, sizeof(pi) );

	push	16					; 00000010H
	push	0
	lea	ecx, DWORD PTR _pi$75946[ebp]
	push	ecx
	call	_memset
	add	esp, 12					; 0000000cH

; 1426 : 							(LPTSTR)lpFile,	// Command line.
; 1427 : 							NULL,			// Process handle not inheritable.
; 1428 : 							NULL,			// Thread handle not inheritable.
; 1429 : 							FALSE,			// Set handle inheritance to FALSE.
; 1430 : 							0,				// No creation flags.
; 1431 : 							NULL,			// Use parent's environment block.
; 1432 : 							lpDirectory,	// starting directory.
; 1433 : 							&si,			// Pointer to STARTUPINFO structure.
; 1434 : 							&pi ))			// Pointer to PROCESS_INFORMATION structure.

	mov	esi, esp
	lea	edx, DWORD PTR _pi$75946[ebp]
	push	edx
	lea	eax, DWORD PTR _si$75945[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpDirectory$[ebp]
	push	ecx
	push	0
	push	0
	push	0
	push	0
	push	0
	mov	edx, DWORD PTR _lpFile$[ebp]
	push	edx
	push	0
	call	DWORD PTR __imp__CreateProcessW@40
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L75949

; 1436 : 			dwRes = 33; // success

	mov	DWORD PTR _dwRes$[ebp], 33		; 00000021H
$L75949:

; 1438 : 
; 1439 : 		// Close process and thread handles.
; 1440 : 		CloseHandle( pi.hProcess );

	mov	esi, esp
	mov	eax, DWORD PTR _pi$75946[ebp]
	push	eax
	call	DWORD PTR __imp__CloseHandle@4
	cmp	esi, esp
	call	__chkesp

; 1441 : 		CloseHandle( pi.hThread );

	mov	esi, esp
	mov	ecx, DWORD PTR _pi$75946[ebp+4]
	push	ecx
	call	DWORD PTR __imp__CloseHandle@4
	cmp	esi, esp
	call	__chkesp
$L75940:

; 1443 : 	return dwRes;

	mov	edx, DWORD PTR _dwRes$[ebp]
	mov	DWORD PTR $T77377[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sParams$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77377[ebp]

; 1444 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 148				; 00000094H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77378:
	lea	ecx, DWORD PTR _sFile$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77379:
	lea	ecx, DWORD PTR _sParams$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77380:
	lea	ecx, DWORD PTR $T77375[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77381:
	lea	ecx, DWORD PTR $T77376[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77382:
	lea	ecx, DWORD PTR _sApp$75926[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77383:
	lea	ecx, DWORD PTR _sApp$75934[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77386:
	mov	eax, OFFSET FLAT:$T77385
	jmp	___CxxFrameHandler
text$x	ENDS
?Run@FileMisc@@YAKPAUHWND__@@PBG1H11@Z ENDP		; FileMisc::Run
PUBLIC	?ExtractResource@FileMisc@@YAHIPBGABVCString@@PAUHINSTANCE__@@@Z ; FileMisc::ExtractResource
PUBLIC	?GetModuleFileNameW@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z ; FileMisc::GetModuleFileNameW
PUBLIC	??0CFileStatus@@QAE@XZ				; CFileStatus::CFileStatus
EXTRN	__imp__FreeResource@4:NEAR
EXTRN	?GetStatus@CFile@@SGHPBGAAUCFileStatus@@@Z:NEAR	; CFile::GetStatus
EXTRN	__imp__FindResourceW@12:NEAR
EXTRN	?Write@CFile@@UAEXPBXI@Z:NEAR			; CFile::Write
EXTRN	?Close@CFile@@UAEXXZ:NEAR			; CFile::Close
EXTRN	__imp__LoadResource@8:NEAR
EXTRN	__imp__SizeofResource@8:NEAR
EXTRN	??0CTime@@QAE@ABV0@@Z:NEAR			; CTime::CTime
EXTRN	??MCTime@@QBEHV0@@Z:NEAR			; CTime::operator<
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG75970 DB	'C', 00H, 'o', 00H, 'u', 00H, 'l', 00H, 'd', 00H, 'n', 00H
	DB	'''', 00H, 't', 00H, ' ', 00H, 'f', 00H, 'i', 00H, 'n', 00H, 'd'
	DB	00H, ' ', 00H, '%', 00H, 's', 00H, ' ', 00H, 'r', 00H, 'e', 00H
	DB	's', 00H, 'o', 00H, 'u', 00H, 'r', 00H, 'c', 00H, 'e', 00H, ' '
	DB	00H, '%', 00H, 'd', 00H, '!', 00H, 0aH, 00H, 00H, 00H
	ORG $+2
$SG75979 DB	'D:\_CODE\Shared\FILEMISC.CPP', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77402	DD	019930520H
	DD	03H
	DD	FLAT:$T77404
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77404	DD	0ffffffffH
	DD	FLAT:$L77398
	DD	00H
	DD	FLAT:$L77399
	DD	01H
	DD	FLAT:$L77400
xdata$x	ENDS
;	COMDAT ?ExtractResource@FileMisc@@YAHIPBGABVCString@@PAUHINSTANCE__@@@Z
_TEXT	SEGMENT
_nID$ = 8
_szType$ = 12
_sTempFilePath$ = 16
_hInst$ = 20
_sTempPath$ = -20
_fsRes$ = -1100
_fsModule$ = -560
_sModulePath$ = -16
_hRes$75967 = -1108
_len$75972 = -1112
_lpRes$75973 = -1104
_file$75980 = -1128
$T77393 = -1132
$T77394 = -1136
$T77395 = -1140
$T77396 = -1144
$T77397 = -1148
__$EHRec$ = -12
?ExtractResource@FileMisc@@YAHIPBGABVCString@@PAUHINSTANCE__@@@Z PROC NEAR ; FileMisc::ExtractResource, COMDAT

; 1447 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77403
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 1136				; 00000470H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-1148]
	mov	ecx, 284				; 0000011cH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1448 : 	// compare time with that of module from which it was loaded
; 1449 : 	CString sTempPath;

	lea	ecx, DWORD PTR _sTempPath$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1450 : 	CFileStatus fsRes, fsModule;

	lea	ecx, DWORD PTR _fsRes$[ebp]
	call	??0CFileStatus@@QAE@XZ			; CFileStatus::CFileStatus
	lea	ecx, DWORD PTR _fsModule$[ebp]
	call	??0CFileStatus@@QAE@XZ			; CFileStatus::CFileStatus

; 1451 : 	CString sModulePath = GetModuleFileName(hInst);

	mov	eax, DWORD PTR _hInst$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sModulePath$[ebp]
	push	ecx
	call	?GetModuleFileNameW@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z ; FileMisc::GetModuleFileNameW
	add	esp, 8
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 1452 : 
; 1453 : 	if (!CFile::GetStatus(sModulePath, fsModule))

	lea	edx, DWORD PTR _fsModule$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sModulePath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?GetStatus@CFile@@SGHPBGAAUCFileStatus@@@Z ; CFile::GetStatus
	test	eax, eax
	jne	SHORT $L75962

; 1454 : 		return FALSE;

	mov	DWORD PTR $T77393[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sModulePath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sTempPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77393[ebp]
	jmp	$L75956
$L75962:

; 1455 : 	
; 1456 : 	// see if the file has been created before
; 1457 : 	if (!CFile::GetStatus(sTempFilePath, fsRes) || fsRes.m_mtime < fsModule.m_mtime)

	lea	eax, DWORD PTR _fsRes$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sTempFilePath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?GetStatus@CFile@@SGHPBGAAUCFileStatus@@@Z ; CFile::GetStatus
	test	eax, eax
	je	SHORT $L75966
	push	ecx
	mov	ecx, esp
	mov	DWORD PTR $T77394[ebp], esp
	lea	edx, DWORD PTR _fsModule$[ebp+4]
	push	edx
	call	??0CTime@@QAE@ABV0@@Z			; CTime::CTime
	lea	ecx, DWORD PTR _fsRes$[ebp+4]
	call	??MCTime@@QBEHV0@@Z			; CTime::operator<
	test	eax, eax
	je	$L75965
$L75966:

; 1459 : 		// Load the resource into memory
; 1460 : 		HRSRC hRes = FindResource(hInst, (LPCTSTR)nID, szType);

	mov	esi, esp
	mov	eax, DWORD PTR _szType$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nID$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hInst$[ebp]
	push	edx
	call	DWORD PTR __imp__FindResourceW@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hRes$75967[ebp], eax

; 1461 : 		
; 1462 : 		if (!hRes) 

	cmp	DWORD PTR _hRes$75967[ebp], 0
	jne	SHORT $L75969

; 1464 : 			TRACE(_T("Couldn't find %s resource %d!\n"), szType, nID);

	mov	eax, DWORD PTR _nID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szType$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG75970
	call	?AfxTrace@@YAXPBGZZ			; AfxTrace
	add	esp, 12					; 0000000cH

; 1465 : 			return FALSE;

	mov	DWORD PTR $T77395[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sModulePath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sTempPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77395[ebp]
	jmp	$L75956
$L75969:

; 1467 : 		
; 1468 : 		DWORD len = SizeofResource(hInst, hRes);

	mov	esi, esp
	mov	edx, DWORD PTR _hRes$75967[ebp]
	push	edx
	mov	eax, DWORD PTR _hInst$[ebp]
	push	eax
	call	DWORD PTR __imp__SizeofResource@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _len$75972[ebp], eax

; 1469 : 		
; 1470 : 		BYTE* lpRes = (BYTE*)LoadResource(hInst, hRes);

	mov	esi, esp
	mov	ecx, DWORD PTR _hRes$75967[ebp]
	push	ecx
	mov	edx, DWORD PTR _hInst$[ebp]
	push	edx
	call	DWORD PTR __imp__LoadResource@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lpRes$75973[ebp], eax
$L75975:

; 1471 : 		ASSERT(lpRes);

	cmp	DWORD PTR _lpRes$75973[ebp], 0
	jne	SHORT $L75978
	push	1471					; 000005bfH
	push	OFFSET FLAT:$SG75979
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75978
	int	3
$L75978:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75975

; 1472 : 
; 1473 : 		CFile file;

	lea	ecx, DWORD PTR _file$75980[ebp]
	call	??0CFile@@QAE@XZ			; CFile::CFile
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 1474 : 		
; 1475 : 		if (file.Open(sTempFilePath, CFile::modeCreate | CFile::modeWrite))

	push	0
	push	4097					; 00001001H
	mov	ecx, DWORD PTR _sTempFilePath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _file$75980[ebp]
	call	?Open@CFile@@UAEHPBGIPAVCFileException@@@Z ; CFile::Open
	test	eax, eax
	je	SHORT $L75981

; 1477 : 			file.Write(lpRes, len);

	mov	ecx, DWORD PTR _len$75972[ebp]
	push	ecx
	mov	edx, DWORD PTR _lpRes$75973[ebp]
	push	edx
	lea	ecx, DWORD PTR _file$75980[ebp]
	call	?Write@CFile@@UAEXPBXI@Z		; CFile::Write

; 1478 : 			file.Close();

	lea	ecx, DWORD PTR _file$75980[ebp]
	call	?Close@CFile@@UAEXXZ			; CFile::Close

; 1479 : 			FreeResource((HANDLE)lpRes);

	mov	esi, esp
	mov	eax, DWORD PTR _lpRes$75973[ebp]
	push	eax
	call	DWORD PTR __imp__FreeResource@4
	cmp	esi, esp
	call	__chkesp

; 1481 : 		else

	jmp	SHORT $L75983
$L75981:

; 1483 : 			FreeResource((HANDLE)lpRes);

	mov	esi, esp
	mov	ecx, DWORD PTR _lpRes$75973[ebp]
	push	ecx
	call	DWORD PTR __imp__FreeResource@4
	cmp	esi, esp
	call	__chkesp

; 1484 : 			return FALSE;

	mov	DWORD PTR $T77396[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _file$75980[ebp]
	call	??1CFile@@UAE@XZ			; CFile::~CFile
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sModulePath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sTempPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77396[ebp]
	jmp	SHORT $L75956
$L75983:

; 1486 : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _file$75980[ebp]
	call	??1CFile@@UAE@XZ			; CFile::~CFile
$L75965:

; 1487 : 	
; 1488 : 	return TRUE;

	mov	DWORD PTR $T77397[ebp], 1
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sModulePath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sTempPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77397[ebp]
$L75956:

; 1489 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 1148				; 0000047cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77398:
	lea	ecx, DWORD PTR _sTempPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77399:
	lea	ecx, DWORD PTR _sModulePath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77400:
	lea	ecx, DWORD PTR _file$75980[ebp]
	call	??1CFile@@UAE@XZ			; CFile::~CFile
	ret	0
$L77403:
	mov	eax, OFFSET FLAT:$T77402
	jmp	___CxxFrameHandler
text$x	ENDS
?ExtractResource@FileMisc@@YAHIPBGABVCString@@PAUHINSTANCE__@@@Z ENDP ; FileMisc::ExtractResource
EXTRN	??0CTime@@QAE@XZ:NEAR				; CTime::CTime
;	COMDAT ??0CFileStatus@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0CFileStatus@@QAE@XZ PROC NEAR			; CFileStatus::CFileStatus, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CTime@@QAE@XZ			; CTime::CTime
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??0CTime@@QAE@XZ			; CTime::CTime
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??0CTime@@QAE@XZ			; CTime::CTime
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CFileStatus@@QAE@XZ ENDP				; CFileStatus::CFileStatus
_TEXT	ENDS
EXTRN	__imp__GetModuleFileNameW@12:NEAR
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77417	DD	019930520H
	DD	02H
	DD	FLAT:$T77419
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77419	DD	0ffffffffH
	DD	FLAT:$L77413
	DD	00H
	DD	FLAT:$L77412
xdata$x	ENDS
;	COMDAT ?GetModuleFileNameW@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z
_TEXT	SEGMENT
_hMod$ = 12
___$ReturnUdt$ = 8
_sModulePath$ = -16
$T77414 = -20
__$EHRec$ = -12
?GetModuleFileNameW@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z PROC NEAR ; FileMisc::GetModuleFileNameW, COMDAT

; 1492 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77418
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T77414[ebp], 0

; 1493 : 	CString sModulePath;

	lea	ecx, DWORD PTR _sModulePath$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1494 : 
; 1495 : 	::GetModuleFileName(hMod, sModulePath.GetBuffer(MAX_PATH+1), MAX_PATH);

	mov	esi, esp
	push	260					; 00000104H
	push	261					; 00000105H
	lea	ecx, DWORD PTR _sModulePath$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	mov	eax, DWORD PTR _hMod$[ebp]
	push	eax
	call	DWORD PTR __imp__GetModuleFileNameW@12
	cmp	esi, esp
	call	__chkesp

; 1496 : 	sModulePath.ReleaseBuffer();

	push	-1
	lea	ecx, DWORD PTR _sModulePath$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 1497 : 
; 1498 : 	return sModulePath;

	lea	ecx, DWORD PTR _sModulePath$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T77414[ebp]
	or	edx, 1
	mov	DWORD PTR $T77414[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sModulePath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1499 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77412:
	lea	ecx, DWORD PTR _sModulePath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77413:
	mov	eax, DWORD PTR $T77414[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77415
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77415:
	ret	0
$L77418:
	mov	eax, OFFSET FLAT:$T77417
	jmp	___CxxFrameHandler
text$x	ENDS
?GetModuleFileNameW@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z ENDP ; FileMisc::GetModuleFileNameW
PUBLIC	?GetModuleFolder@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z ; FileMisc::GetModuleFolder
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77430	DD	019930520H
	DD	02H
	DD	FLAT:$T77432
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77432	DD	0ffffffffH
	DD	FLAT:$L77426
	DD	00H
	DD	FLAT:$L77425
xdata$x	ENDS
;	COMDAT ?GetModuleFolder@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z
_TEXT	SEGMENT
_hMod$ = 12
___$ReturnUdt$ = 8
$T77424 = -16
$T77427 = -20
__$EHRec$ = -12
?GetModuleFolder@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z PROC NEAR ; FileMisc::GetModuleFolder, COMDAT

; 1502 : { 

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77431
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR $T77427[ebp], 0

; 1503 : 	return GetFolderFromFilePath(GetModuleFileName(hMod)); 

	mov	eax, DWORD PTR _hMod$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77424[ebp]
	push	ecx
	call	?GetModuleFileNameW@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z ; FileMisc::GetModuleFileNameW
	add	esp, 8
	mov	DWORD PTR -24+[ebp], eax
	mov	edx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -28+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	call	?GetFolderFromFilePath@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::GetFolderFromFilePath
	add	esp, 8
	mov	ecx, DWORD PTR $T77427[ebp]
	or	ecx, 1
	mov	DWORD PTR $T77427[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T77424[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1504 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77425:
	lea	ecx, DWORD PTR $T77424[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77426:
	mov	eax, DWORD PTR $T77427[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77428
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77428:
	ret	0
$L77431:
	mov	eax, OFFSET FLAT:$T77430
	jmp	___CxxFrameHandler
text$x	ENDS
?GetModuleFolder@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z ENDP ; FileMisc::GetModuleFolder
PUBLIC	?GetModuleDriveType@FileMisc@@YAHPAUHINSTANCE__@@@Z ; FileMisc::GetModuleDriveType
EXTRN	?GetPathType@CDriveInfo@@SAHPBG@Z:NEAR		; CDriveInfo::GetPathType
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77441	DD	019930520H
	DD	01H
	DD	FLAT:$T77443
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77443	DD	0ffffffffH
	DD	FLAT:$L77439
xdata$x	ENDS
;	COMDAT ?GetModuleDriveType@FileMisc@@YAHPAUHINSTANCE__@@@Z
_TEXT	SEGMENT
_hMod$ = 8
$T77437 = -16
$T77438 = -20
__$EHRec$ = -12
?GetModuleDriveType@FileMisc@@YAHPAUHINSTANCE__@@@Z PROC NEAR ; FileMisc::GetModuleDriveType, COMDAT

; 1507 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77442
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 1508 : 	return CDriveInfo::GetPathType(GetModuleFileName(hMod));

	mov	eax, DWORD PTR _hMod$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77438[ebp]
	push	ecx
	call	?GetModuleFileNameW@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z ; FileMisc::GetModuleFileNameW
	add	esp, 8
	mov	DWORD PTR -24+[ebp], eax
	mov	edx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -28+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?GetPathType@CDriveInfo@@SAHPBG@Z	; CDriveInfo::GetPathType
	add	esp, 4
	mov	DWORD PTR $T77437[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T77438[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77437[ebp]

; 1509 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77439:
	lea	ecx, DWORD PTR $T77438[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77442:
	mov	eax, OFFSET FLAT:$T77441
	jmp	___CxxFrameHandler
text$x	ENDS
?GetModuleDriveType@FileMisc@@YAHPAUHINSTANCE__@@@Z ENDP ; FileMisc::GetModuleDriveType
PUBLIC	?GetModuleVersion@FileMisc@@YA?AVCString@@PBG@Z	; FileMisc::GetModuleVersion
EXTRN	??0CFileVersionInfo@@QAE@XZ:NEAR		; CFileVersionInfo::CFileVersionInfo
EXTRN	??1CFileVersionInfo@@UAE@XZ:NEAR		; CFileVersionInfo::~CFileVersionInfo
EXTRN	?Create@CFileVersionInfo@@QAEHPBG@Z:NEAR	; CFileVersionInfo::Create
EXTRN	?GetFileVersion@CFileVersionInfo@@QBE?AVCString@@XZ:NEAR ; CFileVersionInfo::GetFileVersion
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77453	DD	019930520H
	DD	02H
	DD	FLAT:$T77455
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77455	DD	0ffffffffH
	DD	FLAT:$L77449
	DD	00H
	DD	FLAT:$L77448
xdata$x	ENDS
;	COMDAT ?GetModuleVersion@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT
_szModulePath$ = 12
___$ReturnUdt$ = 8
_fvi$ = -116
$T77450 = -120
__$EHRec$ = -12
?GetModuleVersion@FileMisc@@YA?AVCString@@PBG@Z PROC NEAR ; FileMisc::GetModuleVersion, COMDAT

; 1512 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77454
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 108				; 0000006cH
	push	edi
	lea	edi, DWORD PTR [ebp-120]
	mov	ecx, 27					; 0000001bH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T77450[ebp], 0

; 1513 : 	CFileVersionInfo fvi;

	lea	ecx, DWORD PTR _fvi$[ebp]
	call	??0CFileVersionInfo@@QAE@XZ		; CFileVersionInfo::CFileVersionInfo
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1514 : 
; 1515 : 	if (fvi.Create(szModulePath))

	mov	eax, DWORD PTR _szModulePath$[ebp]
	push	eax
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	?Create@CFileVersionInfo@@QAEHPBG@Z	; CFileVersionInfo::Create
	test	eax, eax
	je	SHORT $L76010

; 1516 : 		return fvi.GetFileVersion();

	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	?GetFileVersion@CFileVersionInfo@@QBE?AVCString@@XZ ; CFileVersionInfo::GetFileVersion
	mov	edx, DWORD PTR $T77450[ebp]
	or	edx, 1
	mov	DWORD PTR $T77450[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	??1CFileVersionInfo@@UAE@XZ		; CFileVersionInfo::~CFileVersionInfo
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L76008
$L76010:

; 1517 : 
; 1518 : 	// else
; 1519 : 	return EMPTYSTRING;

	push	OFFSET FLAT:_EMPTYSTRING
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T77450[ebp]
	or	al, 1
	mov	DWORD PTR $T77450[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	??1CFileVersionInfo@@UAE@XZ		; CFileVersionInfo::~CFileVersionInfo
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L76008:

; 1520 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 120				; 00000078H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77448:
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	??1CFileVersionInfo@@UAE@XZ		; CFileVersionInfo::~CFileVersionInfo
	ret	0
$L77449:
	mov	eax, DWORD PTR $T77450[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77451
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77451:
	ret	0
$L77454:
	mov	eax, OFFSET FLAT:$T77453
	jmp	___CxxFrameHandler
text$x	ENDS
?GetModuleVersion@FileMisc@@YA?AVCString@@PBG@Z ENDP	; FileMisc::GetModuleVersion
EXTRN	?Create@CFileVersionInfo@@QAEHPAUHINSTANCE__@@@Z:NEAR ; CFileVersionInfo::Create
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77465	DD	019930520H
	DD	02H
	DD	FLAT:$T77467
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77467	DD	0ffffffffH
	DD	FLAT:$L77461
	DD	00H
	DD	FLAT:$L77460
xdata$x	ENDS
;	COMDAT ?GetModuleVersion@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z
_TEXT	SEGMENT
_hMod$ = 12
___$ReturnUdt$ = 8
_fvi$ = -116
$T77462 = -120
__$EHRec$ = -12
?GetModuleVersion@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z PROC NEAR ; FileMisc::GetModuleVersion, COMDAT

; 1523 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77466
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 108				; 0000006cH
	push	edi
	lea	edi, DWORD PTR [ebp-120]
	mov	ecx, 27					; 0000001bH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T77462[ebp], 0

; 1524 : 	CFileVersionInfo fvi;

	lea	ecx, DWORD PTR _fvi$[ebp]
	call	??0CFileVersionInfo@@QAE@XZ		; CFileVersionInfo::CFileVersionInfo
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1525 : 
; 1526 : 	if (fvi.Create(hMod))

	mov	eax, DWORD PTR _hMod$[ebp]
	push	eax
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	?Create@CFileVersionInfo@@QAEHPAUHINSTANCE__@@@Z ; CFileVersionInfo::Create
	test	eax, eax
	je	SHORT $L76017

; 1527 : 		return fvi.GetFileVersion();

	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	?GetFileVersion@CFileVersionInfo@@QBE?AVCString@@XZ ; CFileVersionInfo::GetFileVersion
	mov	edx, DWORD PTR $T77462[ebp]
	or	edx, 1
	mov	DWORD PTR $T77462[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	??1CFileVersionInfo@@UAE@XZ		; CFileVersionInfo::~CFileVersionInfo
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L76015
$L76017:

; 1528 : 
; 1529 : 	// else
; 1530 : 	return EMPTYSTRING;

	push	OFFSET FLAT:_EMPTYSTRING
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T77462[ebp]
	or	al, 1
	mov	DWORD PTR $T77462[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	??1CFileVersionInfo@@UAE@XZ		; CFileVersionInfo::~CFileVersionInfo
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L76015:

; 1531 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 120				; 00000078H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77460:
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	??1CFileVersionInfo@@UAE@XZ		; CFileVersionInfo::~CFileVersionInfo
	ret	0
$L77461:
	mov	eax, DWORD PTR $T77462[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77463
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77463:
	ret	0
$L77466:
	mov	eax, OFFSET FLAT:$T77465
	jmp	___CxxFrameHandler
text$x	ENDS
?GetModuleVersion@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z ENDP ; FileMisc::GetModuleVersion
PUBLIC	?GetModuleVersion@FileMisc@@YAHPAUHINSTANCE__@@AAVCUIntArray@@@Z ; FileMisc::GetModuleVersion
EXTRN	?GetFileVersion@CFileVersionInfo@@QBEGH@Z:NEAR	; CFileVersionInfo::GetFileVersion
EXTRN	?RemoveAll@CUIntArray@@QAEXXZ:NEAR		; CUIntArray::RemoveAll
EXTRN	?Add@CUIntArray@@QAEHI@Z:NEAR			; CUIntArray::Add
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77476	DD	019930520H
	DD	01H
	DD	FLAT:$T77478
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77478	DD	0ffffffffH
	DD	FLAT:$L77474
xdata$x	ENDS
;	COMDAT ?GetModuleVersion@FileMisc@@YAHPAUHINSTANCE__@@AAVCUIntArray@@@Z
_TEXT	SEGMENT
_hMod$ = 8
_aVersionParts$ = 12
_fvi$ = -116
$T77472 = -120
$T77473 = -124
__$EHRec$ = -12
?GetModuleVersion@FileMisc@@YAHPAUHINSTANCE__@@AAVCUIntArray@@@Z PROC NEAR ; FileMisc::GetModuleVersion, COMDAT

; 1534 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77477
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 112				; 00000070H
	push	edi
	lea	edi, DWORD PTR [ebp-124]
	mov	ecx, 28					; 0000001cH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1535 : 	CFileVersionInfo fvi;

	lea	ecx, DWORD PTR _fvi$[ebp]
	call	??0CFileVersionInfo@@QAE@XZ		; CFileVersionInfo::CFileVersionInfo
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1536 : 
; 1537 : 	if (fvi.Create(hMod))

	mov	eax, DWORD PTR _hMod$[ebp]
	push	eax
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	?Create@CFileVersionInfo@@QAEHPAUHINSTANCE__@@@Z ; CFileVersionInfo::Create
	test	eax, eax
	je	$L76024

; 1539 : 		aVersionParts.RemoveAll();

	mov	ecx, DWORD PTR _aVersionParts$[ebp]
	call	?RemoveAll@CUIntArray@@QAEXXZ		; CUIntArray::RemoveAll

; 1540 : 
; 1541 : 		aVersionParts.Add(fvi.GetFileVersion(3));

	push	3
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	?GetFileVersion@CFileVersionInfo@@QBEGH@Z ; CFileVersionInfo::GetFileVersion
	and	eax, 65535				; 0000ffffH
	push	eax
	mov	ecx, DWORD PTR _aVersionParts$[ebp]
	call	?Add@CUIntArray@@QAEHI@Z		; CUIntArray::Add

; 1542 : 		aVersionParts.Add(fvi.GetFileVersion(2));

	push	2
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	?GetFileVersion@CFileVersionInfo@@QBEGH@Z ; CFileVersionInfo::GetFileVersion
	and	eax, 65535				; 0000ffffH
	push	eax
	mov	ecx, DWORD PTR _aVersionParts$[ebp]
	call	?Add@CUIntArray@@QAEHI@Z		; CUIntArray::Add

; 1543 : 		aVersionParts.Add(fvi.GetFileVersion(1));

	push	1
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	?GetFileVersion@CFileVersionInfo@@QBEGH@Z ; CFileVersionInfo::GetFileVersion
	and	eax, 65535				; 0000ffffH
	push	eax
	mov	ecx, DWORD PTR _aVersionParts$[ebp]
	call	?Add@CUIntArray@@QAEHI@Z		; CUIntArray::Add

; 1544 : 		aVersionParts.Add(fvi.GetFileVersion(0));

	push	0
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	?GetFileVersion@CFileVersionInfo@@QBEGH@Z ; CFileVersionInfo::GetFileVersion
	and	eax, 65535				; 0000ffffH
	push	eax
	mov	ecx, DWORD PTR _aVersionParts$[ebp]
	call	?Add@CUIntArray@@QAEHI@Z		; CUIntArray::Add

; 1545 : 
; 1546 : 		return TRUE;

	mov	DWORD PTR $T77472[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	??1CFileVersionInfo@@UAE@XZ		; CFileVersionInfo::~CFileVersionInfo
	mov	eax, DWORD PTR $T77472[ebp]
	jmp	SHORT $L76022
$L76024:

; 1548 : 
; 1549 : 	// else
; 1550 : 	return FALSE;

	mov	DWORD PTR $T77473[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	??1CFileVersionInfo@@UAE@XZ		; CFileVersionInfo::~CFileVersionInfo
	mov	eax, DWORD PTR $T77473[ebp]
$L76022:

; 1551 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 124				; 0000007cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77474:
	lea	ecx, DWORD PTR _fvi$[ebp]
	call	??1CFileVersionInfo@@UAE@XZ		; CFileVersionInfo::~CFileVersionInfo
	ret	0
$L77477:
	mov	eax, OFFSET FLAT:$T77476
	jmp	___CxxFrameHandler
text$x	ENDS
?GetModuleVersion@FileMisc@@YAHPAUHINSTANCE__@@AAVCUIntArray@@@Z ENDP ; FileMisc::GetModuleVersion
;	COMDAT ?GetAppFileName@FileMisc@@YA?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
$T77484 = -4
?GetAppFileName@FileMisc@@YA?AVCString@@XZ PROC NEAR	; FileMisc::GetAppFileName, COMDAT

; 1554 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T77484[ebp], 0

; 1555 : 	return GetModuleFileName();

	push	0
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	call	?GetModuleFileNameW@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z ; FileMisc::GetModuleFileNameW
	add	esp, 8
	mov	ecx, DWORD PTR $T77484[ebp]
	or	ecx, 1
	mov	DWORD PTR $T77484[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1556 : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetAppFileName@FileMisc@@YA?AVCString@@XZ ENDP		; FileMisc::GetAppFileName
_TEXT	ENDS
PUBLIC	?GetAppIniFileName@FileMisc@@YA?AVCString@@ABV2@@Z ; FileMisc::GetAppIniFileName
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG76046 DB	'D:\_CODE\Shared\FILEMISC.CPP', 00H
	ORG $+3
$SG76052 DB	'D:\_CODE\Shared\FILEMISC.CPP', 00H
	ORG $+3
$SG76053 DB	'i', 00H, 'n', 00H, 'i', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77497	DD	019930520H
	DD	06H
	DD	FLAT:$T77499
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77499	DD	0ffffffffH
	DD	FLAT:$L77493
	DD	00H
	DD	FLAT:$L77489
	DD	01H
	DD	FLAT:$L77490
	DD	00H
	DD	FLAT:$L77490
	DD	03H
	DD	FLAT:$L77491
	DD	04H
	DD	FLAT:$L77492
xdata$x	ENDS
;	COMDAT ?GetAppIniFileName@FileMisc@@YA?AVCString@@ABV2@@Z
_TEXT	SEGMENT
_sAppDataKey$ = 12
___$ReturnUdt$ = 8
_sAppName$ = -24
_sIniFolder$ = -16
_sIniPath$ = -20
$T77488 = -28
$T77494 = -32
__$EHRec$ = -12
?GetAppIniFileName@FileMisc@@YA?AVCString@@ABV2@@Z PROC NEAR ; FileMisc::GetAppIniFileName, COMDAT

; 1559 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77498
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 28					; 0000001cH
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR $T77494[ebp], 0

; 1560 : 	CString sAppName = GetFileNameFromPath(GetAppFileName(), FALSE);

	push	0
	lea	eax, DWORD PTR $T77488[ebp]
	push	eax
	call	?GetAppFileName@FileMisc@@YA?AVCString@@XZ ; FileMisc::GetAppFileName
	add	esp, 4
	mov	DWORD PTR -36+[ebp], eax
	mov	ecx, DWORD PTR -36+[ebp]
	mov	DWORD PTR -40+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -40+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	edx, DWORD PTR _sAppName$[ebp]
	push	edx
	call	?GetFileNameFromPath@FileMisc@@YA?AVCString@@PBGH@Z ; FileMisc::GetFileNameFromPath
	add	esp, 12					; 0000000cH
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR $T77488[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1561 : 
; 1562 : 	// preferred location is app folder for portability
; 1563 : 	CString sIniFolder = GetAppFolder(), sIniPath;

	push	0
	lea	eax, DWORD PTR _sIniFolder$[ebp]
	push	eax
	call	?GetAppFolder@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::GetAppFolder
	add	esp, 8
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR _sIniPath$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 5

; 1564 : 
; 1565 : 	// but it may not be writable
; 1566 : 	if (!IsFolderWritable(sIniFolder))

	push	0
	lea	ecx, DWORD PTR _sIniFolder$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsFolderWritable@FileMisc@@YAHPBGH@Z	; FileMisc::IsFolderWritable
	add	esp, 8
	test	eax, eax
	jne	SHORT $L76047
$L76042:

; 1568 : 		// try app data
; 1569 : 		VERIFY (GetSpecialFilePath(CSIDL_APPDATA, sAppDataKey, sIniFolder));

	lea	ecx, DWORD PTR _sIniFolder$[ebp]
	push	ecx
	mov	edx, DWORD PTR _sAppDataKey$[ebp]
	push	edx
	push	26					; 0000001aH
	call	?GetSpecialFilePath@FileMisc@@YAHHABVCString@@AAV2@@Z ; FileMisc::GetSpecialFilePath
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	SHORT $L76045
	push	1569					; 00000621H
	push	OFFSET FLAT:$SG76046
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76045
	int	3
$L76045:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76042

; 1570 : 
; 1571 : 		// add app name as sub folder if not in key
; 1572 : 		if (sAppDataKey.Find(sAppName) == -1)

	lea	ecx, DWORD PTR _sAppName$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _sAppDataKey$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	cmp	eax, -1
	jne	SHORT $L76047

; 1574 : 			TerminatePath(sIniFolder) += sAppName;

	lea	ecx, DWORD PTR _sAppName$[ebp]
	push	ecx
	lea	edx, DWORD PTR _sIniFolder$[ebp]
	push	edx
	call	?TerminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ; FileMisc::TerminatePath
	add	esp, 4
	mov	ecx, eax
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
$L76047:

; 1577 : 
; 1578 : 	ASSERT(!sIniFolder.IsEmpty());

	lea	ecx, DWORD PTR _sIniFolder$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L76051
	push	1578					; 0000062aH
	push	OFFSET FLAT:$SG76052
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76051
	int	3
$L76051:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76047

; 1579 : 
; 1580 : 	return MakePath(sIniPath, NULL, sIniFolder, sAppName, _T("ini"));

	push	OFFSET FLAT:$SG76053
	lea	ecx, DWORD PTR _sAppName$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sIniFolder$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	0
	lea	ecx, DWORD PTR _sIniPath$[ebp]
	push	ecx
	call	?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z ; FileMisc::MakePath
	add	esp, 20					; 00000014H
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T77494[ebp]
	or	edx, 1
	mov	DWORD PTR $T77494[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR _sIniPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR _sIniFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sAppName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1581 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77489:
	lea	ecx, DWORD PTR $T77488[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77490:
	lea	ecx, DWORD PTR _sAppName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77491:
	lea	ecx, DWORD PTR _sIniFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77492:
	lea	ecx, DWORD PTR _sIniPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77493:
	mov	eax, DWORD PTR $T77494[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77495
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77495:
	ret	0
$L77498:
	mov	eax, OFFSET FLAT:$T77497
	jmp	___CxxFrameHandler
text$x	ENDS
?GetAppIniFileName@FileMisc@@YA?AVCString@@ABV2@@Z ENDP	; FileMisc::GetAppIniFileName
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77510	DD	019930520H
	DD	02H
	DD	FLAT:$T77512
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77512	DD	0ffffffffH
	DD	FLAT:$L77506
	DD	00H
	DD	FLAT:$L77505
xdata$x	ENDS
;	COMDAT ?GetAppFolder@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT
_szSubFolder$ = 12
___$ReturnUdt$ = 8
_sFolder$ = -16
$T77507 = -20
__$EHRec$ = -12
?GetAppFolder@FileMisc@@YA?AVCString@@PBG@Z PROC NEAR	; FileMisc::GetAppFolder, COMDAT

; 1584 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77511
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T77507[ebp], 0

; 1585 : 	CString sFolder = GetModuleFolder();

	push	0
	lea	eax, DWORD PTR _sFolder$[ebp]
	push	eax
	call	?GetModuleFolder@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z ; FileMisc::GetModuleFolder
	add	esp, 8
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1586 : 
; 1587 : 	if (szSubFolder && *szSubFolder)

	cmp	DWORD PTR _szSubFolder$[ebp], 0
	je	SHORT $L76060
	mov	ecx, DWORD PTR _szSubFolder$[ebp]
	xor	edx, edx
	mov	dx, WORD PTR [ecx]
	test	edx, edx
	je	SHORT $L76060

; 1588 : 		MakePath(sFolder, NULL, sFolder, szSubFolder, NULL);

	push	0
	mov	eax, DWORD PTR _szSubFolder$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	0
	lea	ecx, DWORD PTR _sFolder$[ebp]
	push	ecx
	call	?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z ; FileMisc::MakePath
	add	esp, 20					; 00000014H
$L76060:

; 1589 : 
; 1590 : 	return UnterminatePath(sFolder);

	lea	edx, DWORD PTR _sFolder$[ebp]
	push	edx
	call	?UnterminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ; FileMisc::UnterminatePath
	add	esp, 4
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T77507[ebp]
	or	al, 1
	mov	DWORD PTR $T77507[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1591 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77505:
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77506:
	mov	eax, DWORD PTR $T77507[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77508
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77508:
	ret	0
$L77511:
	mov	eax, OFFSET FLAT:$T77510
	jmp	___CxxFrameHandler
text$x	ENDS
?GetAppFolder@FileMisc@@YA?AVCString@@PBG@Z ENDP	; FileMisc::GetAppFolder
PUBLIC	?GetAppResourceFolder@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::GetAppResourceFolder
;	COMDAT ?GetAppResourceFolder@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT
_szResFolder$ = 12
___$ReturnUdt$ = 8
$T77518 = -4
?GetAppResourceFolder@FileMisc@@YA?AVCString@@PBG@Z PROC NEAR ; FileMisc::GetAppResourceFolder, COMDAT

; 1594 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T77518[ebp], 0

; 1595 : 	return GetAppFolder(szResFolder);

	mov	eax, DWORD PTR _szResFolder$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	call	?GetAppFolder@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::GetAppFolder
	add	esp, 8
	mov	edx, DWORD PTR $T77518[ebp]
	or	edx, 1
	mov	DWORD PTR $T77518[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1596 : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetAppResourceFolder@FileMisc@@YA?AVCString@@PBG@Z ENDP ; FileMisc::GetAppResourceFolder
_TEXT	ENDS
PUBLIC	?GetAppModuleFileNames@FileMisc@@YAHAAVCStringArray@@W4FB_MODULE_SORT@@@Z ; FileMisc::GetAppModuleFileNames
PUBLIC	?GetAppModuleHandles@FileMisc@@YAHAAVCDWordArray@@H@Z ; FileMisc::GetAppModuleHandles
EXTRN	?SortArray@Misc@@YAXAAVCStringArray@@P6AHPBX1@Z@Z:NEAR ; Misc::SortArray
EXTRN	??0CDWordArray@@QAE@XZ:NEAR			; CDWordArray::CDWordArray
EXTRN	??ACDWordArray@@QAEAAKH@Z:NEAR			; CDWordArray::operator[]
EXTRN	??1CDWordArray@@UAE@XZ:NEAR			; CDWordArray::~CDWordArray
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77527	DD	019930520H
	DD	02H
	DD	FLAT:$T77529
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77529	DD	0ffffffffH
	DD	FLAT:$L77524
	DD	00H
	DD	FLAT:$L77525
xdata$x	ENDS
;	COMDAT ?GetAppModuleFileNames@FileMisc@@YAHAAVCStringArray@@W4FB_MODULE_SORT@@@Z
_TEXT	SEGMENT
_aFileNames$ = 8
_nSort$ = 12
_aHandles$ = -36
_nNumMod$ = -16
_i$ = -40
$T77522 = -44
$T77523 = -48
__$EHRec$ = -12
?GetAppModuleFileNames@FileMisc@@YAHAAVCStringArray@@W4FB_MODULE_SORT@@@Z PROC NEAR ; FileMisc::GetAppModuleFileNames, COMDAT

; 1599 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77528
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 44					; 0000002cH
	push	edi
	lea	edi, DWORD PTR [ebp-56]
	mov	ecx, 11					; 0000000bH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1600 : 	aFileNames.RemoveAll();

	mov	ecx, DWORD PTR _aFileNames$[ebp]
	call	?RemoveAll@CStringArray@@QAEXXZ		; CStringArray::RemoveAll

; 1601 : 
; 1602 : 	CDWordArray aHandles;

	lea	ecx, DWORD PTR _aHandles$[ebp]
	call	??0CDWordArray@@QAE@XZ			; CDWordArray::CDWordArray
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1603 : 
; 1604 : 	int nNumMod = GetAppModuleHandles(aHandles, (nSort == FBM_SORTBY_HMODULE));

	xor	eax, eax
	cmp	DWORD PTR _nSort$[ebp], 1
	sete	al
	push	eax
	lea	ecx, DWORD PTR _aHandles$[ebp]
	push	ecx
	call	?GetAppModuleHandles@FileMisc@@YAHAAVCDWordArray@@H@Z ; FileMisc::GetAppModuleHandles
	add	esp, 8
	mov	DWORD PTR _nNumMod$[ebp], eax

; 1605 : 
; 1606 :     for (int i = 0; i < nNumMod; i++)

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L76073
$L76074:
	mov	edx, DWORD PTR _i$[ebp]
	add	edx, 1
	mov	DWORD PTR _i$[ebp], edx
$L76073:
	mov	eax, DWORD PTR _i$[ebp]
	cmp	eax, DWORD PTR _nNumMod$[ebp]
	jge	SHORT $L76075

; 1608 : 		aFileNames.Add(GetModuleFileName((HMODULE)aHandles[i]));

	mov	ecx, DWORD PTR _i$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _aHandles$[ebp]
	call	??ACDWordArray@@QAEAAKH@Z		; CDWordArray::operator[]
	mov	edx, DWORD PTR [eax]
	push	edx
	lea	eax, DWORD PTR $T77522[ebp]
	push	eax
	call	?GetModuleFileNameW@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z ; FileMisc::GetModuleFileNameW
	add	esp, 8
	mov	DWORD PTR -52+[ebp], eax
	mov	ecx, DWORD PTR -52+[ebp]
	mov	DWORD PTR -56+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	edx, DWORD PTR -56+[ebp]
	push	edx
	mov	ecx, DWORD PTR _aFileNames$[ebp]
	call	?Add@CStringArray@@QAEHABVCString@@@Z	; CStringArray::Add
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T77522[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1609 :     }

	jmp	SHORT $L76074
$L76075:

; 1610 : 	
; 1611 : 	// sort 
; 1612 : 	if (nSort == FBM_SORTBY_FILENAME)

	cmp	DWORD PTR _nSort$[ebp], 2
	jne	SHORT $L76078

; 1613 : 		Misc::SortArray(aFileNames);

	push	0
	mov	eax, DWORD PTR _aFileNames$[ebp]
	push	eax
	call	?SortArray@Misc@@YAXAAVCStringArray@@P6AHPBX1@Z@Z ; Misc::SortArray
	add	esp, 8
$L76078:

; 1614 : 
; 1615 : 	return aFileNames.GetSize();

	mov	ecx, DWORD PTR _aFileNames$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR $T77523[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _aHandles$[ebp]
	call	??1CDWordArray@@UAE@XZ			; CDWordArray::~CDWordArray
	mov	eax, DWORD PTR $T77523[ebp]

; 1616 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 56					; 00000038H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77524:
	lea	ecx, DWORD PTR _aHandles$[ebp]
	call	??1CDWordArray@@UAE@XZ			; CDWordArray::~CDWordArray
	ret	0
$L77525:
	lea	ecx, DWORD PTR $T77522[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77528:
	mov	eax, OFFSET FLAT:$T77527
	jmp	___CxxFrameHandler
text$x	ENDS
?GetAppModuleFileNames@FileMisc@@YAHAAVCStringArray@@W4FB_MODULE_SORT@@@Z ENDP ; FileMisc::GetAppModuleFileNames
EXTRN	__imp__GetProcAddress@8:NEAR
EXTRN	?SortArray@Misc@@YAXAAVCDWordArray@@P6AHPBX1@Z@Z:NEAR ; Misc::SortArray
EXTRN	__imp__GetCurrentProcess@0:NEAR
EXTRN	?GetSize@CDWordArray@@QBEHXZ:NEAR		; CDWordArray::GetSize
EXTRN	?RemoveAll@CDWordArray@@QAEXXZ:NEAR		; CDWordArray::RemoveAll
EXTRN	__imp__LoadLibraryW@4:NEAR
EXTRN	?Add@CDWordArray@@QAEHK@Z:NEAR			; CDWordArray::Add
_BSS	SEGMENT
	ALIGN	4

_?hPsapi@?1??GetAppModuleHandles@FileMisc@@YAHAAVCDWordArray@@H@Z@4PAUHINSTANCE__@@A DD 01H DUP (?)
_?$S278@?1??GetAppModuleHandles@FileMisc@@YAHAAVCDWordArray@@H@Z@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG76091 DB	'p', 00H, 's', 00H, 'a', 00H, 'p', 00H, 'i', 00H, '.', 00H
	DB	'd', 00H, 'l', 00H, 'l', 00H, 00H, 00H
$SG76100 DB	'EnumProcessModules', 00H
data	ENDS
;	COMDAT ?GetAppModuleHandles@FileMisc@@YAHAAVCDWordArray@@H@Z
_TEXT	SEGMENT
_aHandles$ = 8
_bSorted$ = 12
_hMods$ = -4100
_cbNeeded$ = -4
_pFN$76098 = -4104
_nNumMod$76102 = -4108
_i$76105 = -4112
?GetAppModuleHandles@FileMisc@@YAHAAVCDWordArray@@H@Z PROC NEAR ; FileMisc::GetAppModuleHandles, COMDAT

; 1619 : {

	push	ebp
	mov	ebp, esp
	mov	eax, 4112				; 00001010H
	call	__chkstk
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-4112]
	mov	ecx, 1028				; 00000404H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1620 : 	aHandles.RemoveAll();

	mov	ecx, DWORD PTR _aHandles$[ebp]
	call	?RemoveAll@CDWordArray@@QAEXXZ		; CDWordArray::RemoveAll

; 1621 : 
; 1622 : 	HMODULE hMods[1024] = { 0 };

	mov	DWORD PTR _hMods$[ebp], 0
	mov	ecx, 1023				; 000003ffH
	xor	eax, eax
	lea	edi, DWORD PTR _hMods$[ebp+4]
	rep stosd

; 1623 :     DWORD cbNeeded = 0;

	mov	DWORD PTR _cbNeeded$[ebp], 0

; 1624 : 
; 1625 : 	static HMODULE hPsapi = LoadLibrary(_T("psapi.dll"));

	xor	eax, eax
	mov	al, BYTE PTR _?$S278@?1??GetAppModuleHandles@FileMisc@@YAHAAVCDWordArray@@H@Z@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L76090
	mov	cl, BYTE PTR _?$S278@?1??GetAppModuleHandles@FileMisc@@YAHAAVCDWordArray@@H@Z@4EA
	or	cl, 1
	mov	BYTE PTR _?$S278@?1??GetAppModuleHandles@FileMisc@@YAHAAVCDWordArray@@H@Z@4EA, cl
	mov	esi, esp
	push	OFFSET FLAT:$SG76091
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?hPsapi@?1??GetAppModuleHandles@FileMisc@@YAHAAVCDWordArray@@H@Z@4PAUHINSTANCE__@@A, eax
$L76090:

; 1626 : 
; 1627 : 	if (hPsapi)

	cmp	DWORD PTR _?hPsapi@?1??GetAppModuleHandles@FileMisc@@YAHAAVCDWordArray@@H@Z@4PAUHINSTANCE__@@A, 0
	je	$L76110

; 1629 : 		typedef BOOL (WINAPI *FNENUMPROCESSMODULES)(HANDLE,	HMODULE*, DWORD, LPDWORD);
; 1630 : 
; 1631 : 		FNENUMPROCESSMODULES pFN = (FNENUMPROCESSMODULES)GetProcAddress(hPsapi, "EnumProcessModules");

	mov	esi, esp
	push	OFFSET FLAT:$SG76100
	mov	edx, DWORD PTR _?hPsapi@?1??GetAppModuleHandles@FileMisc@@YAHAAVCDWordArray@@H@Z@4PAUHINSTANCE__@@A
	push	edx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pFN$76098[ebp], eax

; 1632 : 
; 1633 : 		if (pFN && pFN(::GetCurrentProcess(), hMods, sizeof(hMods), &cbNeeded))

	cmp	DWORD PTR _pFN$76098[ebp], 0
	je	$L76110
	mov	esi, esp
	lea	eax, DWORD PTR _cbNeeded$[ebp]
	push	eax
	push	4096					; 00001000H
	lea	ecx, DWORD PTR _hMods$[ebp]
	push	ecx
	mov	edi, esp
	call	DWORD PTR __imp__GetCurrentProcess@0
	cmp	edi, esp
	call	__chkesp
	push	eax
	call	DWORD PTR _pFN$76098[ebp]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L76110

; 1635 : 			int nNumMod = (int)(cbNeeded / sizeof(HMODULE));

	mov	edx, DWORD PTR _cbNeeded$[ebp]
	shr	edx, 2
	mov	DWORD PTR _nNumMod$76102[ebp], edx

; 1636 : 
; 1637 : 			for (int i = 0; i < nNumMod; i++)

	mov	DWORD PTR _i$76105[ebp], 0
	jmp	SHORT $L76106
$L76107:
	mov	eax, DWORD PTR _i$76105[ebp]
	add	eax, 1
	mov	DWORD PTR _i$76105[ebp], eax
$L76106:
	mov	ecx, DWORD PTR _i$76105[ebp]
	cmp	ecx, DWORD PTR _nNumMod$76102[ebp]
	jge	SHORT $L76108

; 1639 : 				aHandles.Add((DWORD)hMods[i]);

	mov	edx, DWORD PTR _i$76105[ebp]
	mov	eax, DWORD PTR _hMods$[ebp+edx*4]
	push	eax
	mov	ecx, DWORD PTR _aHandles$[ebp]
	call	?Add@CDWordArray@@QAEHK@Z		; CDWordArray::Add

; 1640 : 			}

	jmp	SHORT $L76107
$L76108:

; 1641 : 		
; 1642 : 			// sort 
; 1643 : 			if (bSorted)

	cmp	DWORD PTR _bSorted$[ebp], 0
	je	SHORT $L76110

; 1644 : 				Misc::SortArray(aHandles);

	push	0
	mov	ecx, DWORD PTR _aHandles$[ebp]
	push	ecx
	call	?SortArray@Misc@@YAXAAVCDWordArray@@P6AHPBX1@Z@Z ; Misc::SortArray
	add	esp, 8
$L76110:

; 1647 : 
; 1648 : 	return aHandles.GetSize();

	mov	ecx, DWORD PTR _aHandles$[ebp]
	call	?GetSize@CDWordArray@@QBEHXZ		; CDWordArray::GetSize

; 1649 : }

	pop	edi
	pop	esi
	add	esp, 4112				; 00001010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetAppModuleHandles@FileMisc@@YAHAAVCDWordArray@@H@Z ENDP ; FileMisc::GetAppModuleHandles
_TEXT	ENDS
PUBLIC	?LogAppModuleState@FileMisc@@YAXW4FB_MODULE_SORT@@@Z ; FileMisc::LogAppModuleState
EXTRN	__imp__GetModuleHandleW@4:NEAR
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG76121 DB	'D:\_CODE\Shared\FILEMISC.CPP', 00H
	ORG $+3
$SG76122 DB	'B', 00H, 'e', 00H, 'g', 00H, 'i', 00H, 'n', 00H, ' ', 00H
	DB	'A', 00H, 'p', 00H, 'p', 00H, 'l', 00H, 'i', 00H, 'c', 00H, 'a'
	DB	00H, 't', 00H, 'i', 00H, 'o', 00H, 'n', 00H, ' ', 00H, 'M', 00H
	DB	'o', 00H, 'd', 00H, 'u', 00H, 'l', 00H, 'e', 00H, ' ', 00H, 'S'
	DB	00H, 't', 00H, 'a', 00H, 't', 00H, 'e', 00H, '-', 00H, '-', 00H
	DB	'-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-'
	DB	00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H
	DB	'-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-'
	DB	00H, '-', 00H, 00H, 00H
$SG76129 DB	'H', 00H, 'a', 00H, 'n', 00H, 'd', 00H, 'l', 00H, 'e', 00H
	DB	' ', 00H, '=', 00H, ' ', 00H, '%', 00H, '0', 00H, '8', 00H, 'X'
	DB	00H, ',', 00H, ' ', 00H, 'P', 00H, 'a', 00H, 't', 00H, 'h', 00H
	DB	' ', 00H, '=', 00H, ' ', 00H, '%', 00H, 's', 00H, ',', 00H, ' '
	DB	00H, 'V', 00H, 'e', 00H, 'r', 00H, ' ', 00H, '=', 00H, ' ', 00H
	DB	'%', 00H, 's', 00H, 00H, 00H
	ORG $+2
$SG76130 DB	'E', 00H, 'n', 00H, 'd', 00H, ' ', 00H, 'A', 00H, 'p', 00H
	DB	'p', 00H, 'l', 00H, 'i', 00H, 'c', 00H, 'a', 00H, 't', 00H, 'i'
	DB	00H, 'o', 00H, 'n', 00H, ' ', 00H, 'M', 00H, 'o', 00H, 'd', 00H
	DB	'u', 00H, 'l', 00H, 'e', 00H, ' ', 00H, 'S', 00H, 't', 00H, 'a'
	DB	00H, 't', 00H, 'e', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H
	DB	'-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-'
	DB	00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H
	DB	'-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-', 00H, '-'
	DB	00H, '-', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77541	DD	019930520H
	DD	02H
	DD	FLAT:$T77543
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77543	DD	0ffffffffH
	DD	FLAT:$L77538
	DD	00H
	DD	FLAT:$L77539
xdata$x	ENDS
;	COMDAT ?LogAppModuleState@FileMisc@@YAXW4FB_MODULE_SORT@@@Z
_TEXT	SEGMENT
_nSort$ = 8
_aModulePaths$76115 = -32
_nNumMods$76116 = -40
_nMod$76123 = -36
_hMod$76127 = -44
$T77537 = -48
__$EHRec$ = -12
?LogAppModuleState@FileMisc@@YAXW4FB_MODULE_SORT@@@Z PROC NEAR ; FileMisc::LogAppModuleState, COMDAT

; 1652 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77542
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 44					; 0000002cH
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-56]
	mov	ecx, 11					; 0000000bH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1653 : 	if (IsLoggingEnabled())

	call	?IsLoggingEnabledW@FileMisc@@YAHXZ	; FileMisc::IsLoggingEnabledW
	test	eax, eax
	je	$L76114

; 1655 : 		CStringArray aModulePaths;

	lea	ecx, DWORD PTR _aModulePaths$76115[ebp]
	call	??0CStringArray@@QAE@XZ			; CStringArray::CStringArray
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1656 : 
; 1657 : 		int nNumMods = GetAppModuleFileNames(aModulePaths, nSort);

	mov	eax, DWORD PTR _nSort$[ebp]
	push	eax
	lea	ecx, DWORD PTR _aModulePaths$76115[ebp]
	push	ecx
	call	?GetAppModuleFileNames@FileMisc@@YAHAAVCStringArray@@W4FB_MODULE_SORT@@@Z ; FileMisc::GetAppModuleFileNames
	add	esp, 8
	mov	DWORD PTR _nNumMods$76116[ebp], eax
$L76117:

; 1658 : 		ASSERT(nNumMods);

	cmp	DWORD PTR _nNumMods$76116[ebp], 0
	jne	SHORT $L76120
	push	1658					; 0000067aH
	push	OFFSET FLAT:$SG76121
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76120
	int	3
$L76120:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76117

; 1659 : 
; 1660 : 		// header
; 1661 : 		LogTextRaw(_T("Begin Application Module State-----------------------"));

	push	OFFSET FLAT:$SG76122
	call	?LogTextRaw@FileMisc@@YAXPBG@Z		; FileMisc::LogTextRaw
	add	esp, 4

; 1662 : 
; 1663 : 		for (int nMod = 0; nMod < nNumMods; nMod++)

	mov	DWORD PTR _nMod$76123[ebp], 0
	jmp	SHORT $L76124
$L76125:
	mov	eax, DWORD PTR _nMod$76123[ebp]
	add	eax, 1
	mov	DWORD PTR _nMod$76123[ebp], eax
$L76124:
	mov	ecx, DWORD PTR _nMod$76123[ebp]
	cmp	ecx, DWORD PTR _nNumMods$76116[ebp]
	jge	SHORT $L76126

; 1665 : 			HMODULE hMod = GetModuleHandle(aModulePaths[nMod]);

	mov	edx, DWORD PTR _nMod$76123[ebp]
	push	edx
	lea	ecx, DWORD PTR _aModulePaths$76115[ebp]
	call	??ACStringArray@@QAEAAVCString@@H@Z	; CStringArray::operator[]
	mov	ecx, eax
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__GetModuleHandleW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hMod$76127[ebp], eax

; 1666 : 
; 1667 : 			LogText(_T("Handle = %08X, Path = %s, Ver = %s"), 
; 1668 : 					hMod, 
; 1669 : 					aModulePaths[nMod], 
; 1670 : 					GetModuleVersion(hMod));

	mov	eax, DWORD PTR _hMod$76127[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77537[ebp]
	push	ecx
	call	?GetModuleVersion@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z ; FileMisc::GetModuleVersion
	add	esp, 8
	mov	DWORD PTR -52+[ebp], eax
	mov	edx, DWORD PTR -52+[ebp]
	mov	DWORD PTR -56+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	eax, DWORD PTR -56+[ebp]
	mov	ecx, DWORD PTR [eax]
	push	ecx
	mov	edx, DWORD PTR _nMod$76123[ebp]
	push	edx
	lea	ecx, DWORD PTR _aModulePaths$76115[ebp]
	call	??ACStringArray@@QAEAAVCString@@H@Z	; CStringArray::operator[]
	mov	eax, DWORD PTR [eax]
	push	eax
	mov	ecx, DWORD PTR _hMod$76127[ebp]
	push	ecx
	push	OFFSET FLAT:$SG76129
	call	?LogText@FileMisc@@YAXPBGZZ		; FileMisc::LogText
	add	esp, 16					; 00000010H
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T77537[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1671 : 		}

	jmp	$L76125
$L76126:

; 1672 : 
; 1673 : 		// footer
; 1674 : 		LogTextRaw(_T("End Application Module State-------------------------"));

	push	OFFSET FLAT:$SG76130
	call	?LogTextRaw@FileMisc@@YAXPBG@Z		; FileMisc::LogTextRaw
	add	esp, 4

; 1675 : 	}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _aModulePaths$76115[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
$L76114:

; 1676 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 56					; 00000038H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77538:
	lea	ecx, DWORD PTR _aModulePaths$76115[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	ret	0
$L77539:
	lea	ecx, DWORD PTR $T77537[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77542:
	mov	eax, OFFSET FLAT:$T77541
	jmp	___CxxFrameHandler
text$x	ENDS
?LogAppModuleState@FileMisc@@YAXW4FB_MODULE_SORT@@@Z ENDP ; FileMisc::LogAppModuleState
;	COMDAT ?GetAppVersion@FileMisc@@YA?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
$T77550 = -4
?GetAppVersion@FileMisc@@YA?AVCString@@XZ PROC NEAR	; FileMisc::GetAppVersion, COMDAT

; 1679 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T77550[ebp], 0

; 1680 : 	return GetModuleVersion();

	push	0
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	call	?GetModuleVersion@FileMisc@@YA?AVCString@@PAUHINSTANCE__@@@Z ; FileMisc::GetModuleVersion
	add	esp, 8
	mov	ecx, DWORD PTR $T77550[ebp]
	or	ecx, 1
	mov	DWORD PTR $T77550[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1681 : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetAppVersion@FileMisc@@YA?AVCString@@XZ ENDP		; FileMisc::GetAppVersion
_TEXT	ENDS
PUBLIC	?GetAppVersion@FileMisc@@YAHAAVCUIntArray@@@Z	; FileMisc::GetAppVersion
;	COMDAT ?GetAppVersion@FileMisc@@YAHAAVCUIntArray@@@Z
_TEXT	SEGMENT
_aVersionParts$ = 8
?GetAppVersion@FileMisc@@YAHAAVCUIntArray@@@Z PROC NEAR	; FileMisc::GetAppVersion, COMDAT

; 1684 : {

	push	ebp
	mov	ebp, esp

; 1685 : 	return GetModuleVersion(NULL, aVersionParts);

	mov	eax, DWORD PTR _aVersionParts$[ebp]
	push	eax
	push	0
	call	?GetModuleVersion@FileMisc@@YAHPAUHINSTANCE__@@AAVCUIntArray@@@Z ; FileMisc::GetModuleVersion
	add	esp, 8

; 1686 : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?GetAppVersion@FileMisc@@YAHAAVCUIntArray@@@Z ENDP	; FileMisc::GetAppVersion
_TEXT	ENDS
PUBLIC	?SplitVersionNumber@FileMisc@@YAHPBGAAVCUIntArray@@@Z ; FileMisc::SplitVersionNumber
EXTRN	?GetSize@CUIntArray@@QBEHXZ:NEAR		; CUIntArray::GetSize
EXTRN	?InsertAt@CUIntArray@@QAEXHIH@Z:NEAR		; CUIntArray::InsertAt
EXTRN	__imp___wtoi:NEAR
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG76144 DB	'.', 00H, 00H, 00H
$SG76145 DB	',', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77564	DD	019930520H
	DD	03H
	DD	FLAT:$T77566
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77566	DD	0ffffffffH
	DD	FLAT:$L77560
	DD	00H
	DD	FLAT:$L77561
	DD	01H
	DD	FLAT:$L77562
xdata$x	ENDS
;	COMDAT ?SplitVersionNumber@FileMisc@@YAHPBGAAVCUIntArray@@@Z
_TEXT	SEGMENT
_szVersion$ = 8
_aVersionParts$ = 12
_aParts$ = -40
_sVersion$ = -16
_nPart$ = -20
$T77556 = -44
$T77557 = -48
$T77558 = -52
$T77559 = -56
__$EHRec$ = -12
?SplitVersionNumber@FileMisc@@YAHPBGAAVCUIntArray@@@Z PROC NEAR ; FileMisc::SplitVersionNumber, COMDAT

; 1689 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77565
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 44					; 0000002cH
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-56]
	mov	ecx, 11					; 0000000bH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1690 : 	CStringArray aParts;

	lea	ecx, DWORD PTR _aParts$[ebp]
	call	??0CStringArray@@QAE@XZ			; CStringArray::CStringArray
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1691 : 	
; 1692 : 	// replace commas with periods to simplfy things
; 1693 : 	CString sVersion(szVersion);

	mov	eax, DWORD PTR _szVersion$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sVersion$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 1694 : 	sVersion.Replace(_T(","), _T("."));

	push	OFFSET FLAT:$SG76144
	push	OFFSET FLAT:$SG76145
	lea	ecx, DWORD PTR _sVersion$[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 1695 : 
; 1696 : 	if (!Misc::Split(szVersion, aParts, '.', TRUE))

	mov	ecx, DWORD PTR _szVersion$[ebp]
	push	ecx
	lea	ecx, DWORD PTR $T77557[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	push	1
	push	46					; 0000002eH
	lea	edx, DWORD PTR _aParts$[ebp]
	push	edx
	lea	eax, DWORD PTR $T77557[ebp]
	push	eax
	call	?Split@Misc@@YAHABVCString@@AAVCStringArray@@GH@Z ; Misc::Split
	add	esp, 16					; 00000010H
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	BYTE PTR $T77556[ebp], al
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T77557[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	ecx, DWORD PTR $T77556[ebp]
	and	ecx, 255				; 000000ffH
	test	ecx, ecx
	je	SHORT $L76147

; 1697 : 		return 0;

	mov	DWORD PTR $T77558[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sVersion$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _aParts$[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	mov	eax, DWORD PTR $T77558[ebp]
	jmp	$L76141
$L76147:

; 1698 : 
; 1699 : 	aVersionParts.RemoveAll();

	mov	ecx, DWORD PTR _aVersionParts$[ebp]
	call	?RemoveAll@CUIntArray@@QAEXXZ		; CUIntArray::RemoveAll

; 1700 : 
; 1701 : 	int nPart = aParts.GetSize();

	lea	ecx, DWORD PTR _aParts$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nPart$[ebp], eax
$L76152:

; 1702 : 
; 1703 : 	while (nPart--)

	mov	edx, DWORD PTR _nPart$[ebp]
	mov	eax, DWORD PTR _nPart$[ebp]
	sub	eax, 1
	mov	DWORD PTR _nPart$[ebp], eax
	test	edx, edx
	je	SHORT $L76153

; 1704 : 		aVersionParts.InsertAt(0, _ttoi(aParts[nPart]));

	push	1
	mov	ecx, DWORD PTR _nPart$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _aParts$[ebp]
	call	??ACStringArray@@QAEAAVCString@@H@Z	; CStringArray::operator[]
	mov	ecx, eax
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp___wtoi
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	push	eax
	push	0
	mov	ecx, DWORD PTR _aVersionParts$[ebp]
	call	?InsertAt@CUIntArray@@QAEXHIH@Z		; CUIntArray::InsertAt
	jmp	SHORT $L76152
$L76153:

; 1705 :  
; 1706 : 	return aVersionParts.GetSize();

	mov	ecx, DWORD PTR _aVersionParts$[ebp]
	call	?GetSize@CUIntArray@@QBEHXZ		; CUIntArray::GetSize
	mov	DWORD PTR $T77559[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sVersion$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _aParts$[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	mov	eax, DWORD PTR $T77559[ebp]
$L76141:

; 1707 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 56					; 00000038H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77560:
	lea	ecx, DWORD PTR _aParts$[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	ret	0
$L77561:
	lea	ecx, DWORD PTR _sVersion$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77562:
	lea	ecx, DWORD PTR $T77557[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77565:
	mov	eax, OFFSET FLAT:$T77564
	jmp	___CxxFrameHandler
text$x	ENDS
?SplitVersionNumber@FileMisc@@YAHPBGAAVCUIntArray@@@Z ENDP ; FileMisc::SplitVersionNumber
PUBLIC	?CompareVersions@FileMisc@@YAHPBG0H@Z		; FileMisc::CompareVersions
PUBLIC	?CompareVersions@FileMisc@@YAHAAVCUIntArray@@0H@Z ; FileMisc::CompareVersions
EXTRN	__imp__isdigit:NEAR
EXTRN	??0CUIntArray@@QAE@XZ:NEAR			; CUIntArray::CUIntArray
EXTRN	??1CUIntArray@@UAE@XZ:NEAR			; CUIntArray::~CUIntArray
EXTRN	__imp__lstrcmpW@8:NEAR
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77576	DD	019930520H
	DD	02H
	DD	FLAT:$T77578
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77578	DD	0ffffffffH
	DD	FLAT:$L77573
	DD	00H
	DD	FLAT:$L77574
xdata$x	ENDS
;	COMDAT ?CompareVersions@FileMisc@@YAHPBG0H@Z
_TEXT	SEGMENT
_szVersion1$ = 8
_szVersion2$ = 12
_nNumItems$ = 16
_aVer1$ = -32
_aVer2$ = -52
$T77572 = -56
__$EHRec$ = -12
?CompareVersions@FileMisc@@YAHPBG0H@Z PROC NEAR		; FileMisc::CompareVersions, COMDAT

; 1711 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77577
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 44					; 0000002cH
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-56]
	mov	ecx, 11					; 0000000bH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1712 : 	// if the first character of either string is not a number
; 1713 : 	// then fall back on a standard string comparison
; 1714 : 	if (!szVersion1 || !isdigit(szVersion1[0]) || !szVersion2 || !isdigit(szVersion2[0]))

	cmp	DWORD PTR _szVersion1$[ebp], 0
	je	SHORT $L76161
	mov	eax, DWORD PTR _szVersion1$[ebp]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax]
	mov	esi, esp
	push	ecx
	call	DWORD PTR __imp__isdigit
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L76161
	cmp	DWORD PTR _szVersion2$[ebp], 0
	je	SHORT $L76161
	mov	edx, DWORD PTR _szVersion2$[ebp]
	xor	eax, eax
	mov	ax, WORD PTR [edx]
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__isdigit
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76160
$L76161:

; 1715 : 		return lstrcmp(szVersion1, szVersion2);

	mov	esi, esp
	mov	ecx, DWORD PTR _szVersion2$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szVersion1$[ebp]
	push	edx
	call	DWORD PTR __imp__lstrcmpW@8
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L76159
$L76160:

; 1716 : 
; 1717 : 	CUIntArray aVer1, aVer2;

	lea	ecx, DWORD PTR _aVer1$[ebp]
	call	??0CUIntArray@@QAE@XZ			; CUIntArray::CUIntArray
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _aVer2$[ebp]
	call	??0CUIntArray@@QAE@XZ			; CUIntArray::CUIntArray
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 1718 : 
; 1719 : 	SplitVersionNumber(szVersion1, aVer1);

	lea	eax, DWORD PTR _aVer1$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szVersion1$[ebp]
	push	ecx
	call	?SplitVersionNumber@FileMisc@@YAHPBGAAVCUIntArray@@@Z ; FileMisc::SplitVersionNumber
	add	esp, 8

; 1720 : 	SplitVersionNumber(szVersion2, aVer2);

	lea	edx, DWORD PTR _aVer2$[ebp]
	push	edx
	mov	eax, DWORD PTR _szVersion2$[ebp]
	push	eax
	call	?SplitVersionNumber@FileMisc@@YAHPBGAAVCUIntArray@@@Z ; FileMisc::SplitVersionNumber
	add	esp, 8

; 1721 : 
; 1722 : 	return CompareVersions(aVer1, aVer2, nNumItems);

	mov	ecx, DWORD PTR _nNumItems$[ebp]
	push	ecx
	lea	edx, DWORD PTR _aVer2$[ebp]
	push	edx
	lea	eax, DWORD PTR _aVer1$[ebp]
	push	eax
	call	?CompareVersions@FileMisc@@YAHAAVCUIntArray@@0H@Z ; FileMisc::CompareVersions
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77572[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _aVer2$[ebp]
	call	??1CUIntArray@@UAE@XZ			; CUIntArray::~CUIntArray
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _aVer1$[ebp]
	call	??1CUIntArray@@UAE@XZ			; CUIntArray::~CUIntArray
	mov	eax, DWORD PTR $T77572[ebp]
$L76159:

; 1723 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 56					; 00000038H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77573:
	lea	ecx, DWORD PTR _aVer1$[ebp]
	call	??1CUIntArray@@UAE@XZ			; CUIntArray::~CUIntArray
	ret	0
$L77574:
	lea	ecx, DWORD PTR _aVer2$[ebp]
	call	??1CUIntArray@@UAE@XZ			; CUIntArray::~CUIntArray
	ret	0
$L77577:
	mov	eax, OFFSET FLAT:$T77576
	jmp	___CxxFrameHandler
text$x	ENDS
?CompareVersions@FileMisc@@YAHPBG0H@Z ENDP		; FileMisc::CompareVersions
EXTRN	??ACUIntArray@@QAEAAIH@Z:NEAR			; CUIntArray::operator[]
;	COMDAT ?CompareVersions@FileMisc@@YAHAAVCUIntArray@@0H@Z
_TEXT	SEGMENT
_aVersion1$ = 8
_aVersion2$ = 12
_nNumItems$ = 16
_nNumVer1$ = -12
_nNumVer2$ = -4
_nItem$ = -8
?CompareVersions@FileMisc@@YAHAAVCUIntArray@@0H@Z PROC NEAR ; FileMisc::CompareVersions, COMDAT

; 1726 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 28					; 0000001cH
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 1727 : 	int nNumVer1 = aVersion1.GetSize();

	mov	ecx, DWORD PTR _aVersion1$[ebp]
	call	?GetSize@CUIntArray@@QBEHXZ		; CUIntArray::GetSize
	mov	DWORD PTR _nNumVer1$[ebp], eax

; 1728 : 	int nNumVer2 = aVersion2.GetSize();

	mov	ecx, DWORD PTR _aVersion2$[ebp]
	call	?GetSize@CUIntArray@@QBEHXZ		; CUIntArray::GetSize
	mov	DWORD PTR _nNumVer2$[ebp], eax

; 1729 : 
; 1730 : 	if (nNumItems > 0)

	cmp	DWORD PTR _nNumItems$[ebp], 0
	jle	SHORT $L76172

; 1732 : 		nNumItems = min(nNumVer1, min(nNumVer2, nNumItems));

	mov	eax, DWORD PTR _nNumVer2$[ebp]
	cmp	eax, DWORD PTR _nNumItems$[ebp]
	jge	SHORT $L77583
	mov	ecx, DWORD PTR _nNumVer2$[ebp]
	mov	DWORD PTR -16+[ebp], ecx
	jmp	SHORT $L77584
$L77583:
	mov	edx, DWORD PTR _nNumItems$[ebp]
	mov	DWORD PTR -16+[ebp], edx
$L77584:
	mov	eax, DWORD PTR _nNumVer1$[ebp]
	cmp	eax, DWORD PTR -16+[ebp]
	jge	SHORT $L77587
	mov	ecx, DWORD PTR _nNumVer1$[ebp]
	mov	DWORD PTR -20+[ebp], ecx
	jmp	SHORT $L77588
$L77587:
	mov	edx, DWORD PTR _nNumVer2$[ebp]
	cmp	edx, DWORD PTR _nNumItems$[ebp]
	jge	SHORT $L77585
	mov	eax, DWORD PTR _nNumVer2$[ebp]
	mov	DWORD PTR -24+[ebp], eax
	jmp	SHORT $L77586
$L77585:
	mov	ecx, DWORD PTR _nNumItems$[ebp]
	mov	DWORD PTR -24+[ebp], ecx
$L77586:
	mov	edx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -20+[ebp], edx
$L77588:
	mov	eax, DWORD PTR -20+[ebp]
	mov	DWORD PTR _nNumItems$[ebp], eax

; 1734 : 	else

	jmp	SHORT $L76173
$L76172:

; 1736 : 		nNumItems = min(nNumVer1, nNumVer2);

	mov	ecx, DWORD PTR _nNumVer1$[ebp]
	cmp	ecx, DWORD PTR _nNumVer2$[ebp]
	jge	SHORT $L77589
	mov	edx, DWORD PTR _nNumVer1$[ebp]
	mov	DWORD PTR -28+[ebp], edx
	jmp	SHORT $L77590
$L77589:
	mov	eax, DWORD PTR _nNumVer2$[ebp]
	mov	DWORD PTR -28+[ebp], eax
$L77590:
	mov	ecx, DWORD PTR -28+[ebp]
	mov	DWORD PTR _nNumItems$[ebp], ecx
$L76173:

; 1738 : 
; 1739 : 	// compare starting from the front
; 1740 : 	for (int nItem = 0; nItem < nNumItems; nItem++)

	mov	DWORD PTR _nItem$[ebp], 0
	jmp	SHORT $L76175
$L76176:
	mov	edx, DWORD PTR _nItem$[ebp]
	add	edx, 1
	mov	DWORD PTR _nItem$[ebp], edx
$L76175:
	mov	eax, DWORD PTR _nItem$[ebp]
	cmp	eax, DWORD PTR _nNumItems$[ebp]
	jge	SHORT $L76177

; 1742 : 		if (aVersion1[nItem] < aVersion2[nItem])

	mov	ecx, DWORD PTR _nItem$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _aVersion1$[ebp]
	call	??ACUIntArray@@QAEAAIH@Z		; CUIntArray::operator[]
	mov	esi, eax
	mov	edx, DWORD PTR _nItem$[ebp]
	push	edx
	mov	ecx, DWORD PTR _aVersion2$[ebp]
	call	??ACUIntArray@@QAEAAIH@Z		; CUIntArray::operator[]
	mov	ecx, DWORD PTR [esi]
	cmp	ecx, DWORD PTR [eax]
	jae	SHORT $L76178

; 1743 : 			return -1;

	or	eax, -1
	jmp	SHORT $L76169
$L76178:

; 1744 : 
; 1745 : 		else if (aVersion1[nItem] > aVersion2[nItem])

	mov	edx, DWORD PTR _nItem$[ebp]
	push	edx
	mov	ecx, DWORD PTR _aVersion1$[ebp]
	call	??ACUIntArray@@QAEAAIH@Z		; CUIntArray::operator[]
	mov	esi, eax
	mov	eax, DWORD PTR _nItem$[ebp]
	push	eax
	mov	ecx, DWORD PTR _aVersion2$[ebp]
	call	??ACUIntArray@@QAEAAIH@Z		; CUIntArray::operator[]
	mov	ecx, DWORD PTR [esi]
	cmp	ecx, DWORD PTR [eax]
	jbe	SHORT $L76180

; 1746 : 			return 1;

	mov	eax, 1
	jmp	SHORT $L76169
$L76180:

; 1747 : 
; 1748 : 		// else try next item
; 1749 : 	}

	jmp	SHORT $L76176
$L76177:

; 1750 : 
; 1751 : 	// if we got here then compare array lengths
; 1752 : 	if (nNumItems <= 0)

	cmp	DWORD PTR _nNumItems$[ebp], 0
	jg	SHORT $L76181

; 1754 : 		return (nNumVer1 - nNumVer2);

	mov	eax, DWORD PTR _nNumVer1$[ebp]
	sub	eax, DWORD PTR _nNumVer2$[ebp]
	jmp	SHORT $L76169
$L76181:

; 1756 : 
; 1757 : 	// else
; 1758 : 	return 0;

	xor	eax, eax
$L76169:

; 1759 : }

	pop	esi
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?CompareVersions@FileMisc@@YAHAAVCUIntArray@@0H@Z ENDP	; FileMisc::CompareVersions
_TEXT	ENDS
EXTRN	__imp__GetWindowsDirectoryW@8:NEAR
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77598	DD	019930520H
	DD	02H
	DD	FLAT:$T77600
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77600	DD	0ffffffffH
	DD	FLAT:$L77594
	DD	00H
	DD	FLAT:$L77593
xdata$x	ENDS
;	COMDAT ?GetWindowsFolder@FileMisc@@YA?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_sFolder$ = -16
$T77595 = -20
__$EHRec$ = -12
?GetWindowsFolder@FileMisc@@YA?AVCString@@XZ PROC NEAR	; FileMisc::GetWindowsFolder, COMDAT

; 1762 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77599
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T77595[ebp], 0

; 1763 : 	CString sFolder;

	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1764 : 
; 1765 : 	::GetWindowsDirectory(sFolder.GetBuffer(MAX_PATH+1), MAX_PATH);

	mov	esi, esp
	push	260					; 00000104H
	push	261					; 00000105H
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	call	DWORD PTR __imp__GetWindowsDirectoryW@8
	cmp	esi, esp
	call	__chkesp

; 1766 : 	sFolder.ReleaseBuffer();

	push	-1
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 1767 : 
; 1768 : 	return UnterminatePath(sFolder);

	lea	eax, DWORD PTR _sFolder$[ebp]
	push	eax
	call	?UnterminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ; FileMisc::UnterminatePath
	add	esp, 4
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T77595[ebp]
	or	ecx, 1
	mov	DWORD PTR $T77595[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1769 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77593:
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77594:
	mov	eax, DWORD PTR $T77595[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77596
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77596:
	ret	0
$L77599:
	mov	eax, OFFSET FLAT:$T77598
	jmp	___CxxFrameHandler
text$x	ENDS
?GetWindowsFolder@FileMisc@@YA?AVCString@@XZ ENDP	; FileMisc::GetWindowsFolder
PUBLIC	?GetWindowsSystemFolder@FileMisc@@YA?AVCString@@XZ ; FileMisc::GetWindowsSystemFolder
EXTRN	__imp__GetSystemDirectoryW@8:NEAR
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77610	DD	019930520H
	DD	02H
	DD	FLAT:$T77612
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77612	DD	0ffffffffH
	DD	FLAT:$L77606
	DD	00H
	DD	FLAT:$L77605
xdata$x	ENDS
;	COMDAT ?GetWindowsSystemFolder@FileMisc@@YA?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_sFolder$ = -16
$T77607 = -20
__$EHRec$ = -12
?GetWindowsSystemFolder@FileMisc@@YA?AVCString@@XZ PROC NEAR ; FileMisc::GetWindowsSystemFolder, COMDAT

; 1772 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77611
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T77607[ebp], 0

; 1773 : 	CString sFolder;

	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1774 : 
; 1775 : 	::GetSystemDirectory(sFolder.GetBuffer(MAX_PATH+1), MAX_PATH);

	mov	esi, esp
	push	260					; 00000104H
	push	261					; 00000105H
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	call	DWORD PTR __imp__GetSystemDirectoryW@8
	cmp	esi, esp
	call	__chkesp

; 1776 : 	sFolder.ReleaseBuffer();

	push	-1
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 1777 : 
; 1778 : 	return UnterminatePath(sFolder);

	lea	eax, DWORD PTR _sFolder$[ebp]
	push	eax
	call	?UnterminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ; FileMisc::UnterminatePath
	add	esp, 4
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T77607[ebp]
	or	ecx, 1
	mov	DWORD PTR $T77607[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1779 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77605:
	lea	ecx, DWORD PTR _sFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77606:
	mov	eax, DWORD PTR $T77607[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77608
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77608:
	ret	0
$L77611:
	mov	eax, OFFSET FLAT:$T77610
	jmp	___CxxFrameHandler
text$x	ENDS
?GetWindowsSystemFolder@FileMisc@@YA?AVCString@@XZ ENDP	; FileMisc::GetWindowsSystemFolder
PUBLIC	?ExtractResource@FileMisc@@YAHPBGI0ABVCString@@@Z ; FileMisc::ExtractResource
EXTRN	__imp__LoadLibraryExW@12:NEAR
;	COMDAT ?ExtractResource@FileMisc@@YAHPBGI0ABVCString@@@Z
_TEXT	SEGMENT
_szModulePath$ = 8
_nID$ = 12
_szType$ = 16
_sTempFilePath$ = 20
_hModule$ = -4
?ExtractResource@FileMisc@@YAHPBGI0ABVCString@@@Z PROC NEAR ; FileMisc::ExtractResource, COMDAT

; 1782 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1783 : 	HMODULE hModule = LoadLibraryEx(szModulePath, NULL, LOAD_LIBRARY_AS_DATAFILE);

	mov	esi, esp
	push	2
	push	0
	mov	eax, DWORD PTR _szModulePath$[ebp]
	push	eax
	call	DWORD PTR __imp__LoadLibraryExW@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hModule$[ebp], eax

; 1784 : 
; 1785 : 	if (!hModule)

	cmp	DWORD PTR _hModule$[ebp], 0
	jne	SHORT $L76197

; 1786 : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L76195
$L76197:

; 1787 : 
; 1788 : 	// else
; 1789 : 	return ExtractResource(nID, szType, sTempFilePath, hModule);

	mov	ecx, DWORD PTR _hModule$[ebp]
	push	ecx
	mov	edx, DWORD PTR _sTempFilePath$[ebp]
	push	edx
	mov	eax, DWORD PTR _szType$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nID$[ebp]
	push	ecx
	call	?ExtractResource@FileMisc@@YAHIPBGABVCString@@PAUHINSTANCE__@@@Z ; FileMisc::ExtractResource
	add	esp, 16					; 00000010H
$L76195:

; 1790 : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?ExtractResource@FileMisc@@YAHPBGI0ABVCString@@@Z ENDP	; FileMisc::ExtractResource
_TEXT	ENDS
PUBLIC	?GetExtension@FileMisc@@YA?AVCString@@PBG@Z	; FileMisc::GetExtension
EXTRN	??ACString@@QBEGH@Z:NEAR			; CString::operator[]
EXTRN	?CompareNoCase@CString@@QBEHPBG@Z:NEAR		; CString::CompareNoCase
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77624	DD	019930520H
	DD	02H
	DD	FLAT:$T77626
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77626	DD	0ffffffffH
	DD	FLAT:$L77621
	DD	00H
	DD	FLAT:$L77622
xdata$x	ENDS
;	COMDAT ?HasExtension@FileMisc@@YAHPBG0@Z
_TEXT	SEGMENT
_szFilePath$ = 8
_szExt$ = 12
_sExt$ = -16
$T77619 = -20
$T77620 = -24
__$EHRec$ = -12
?HasExtension@FileMisc@@YAHPBG0@Z PROC NEAR		; FileMisc::HasExtension, COMDAT

; 1793 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77625
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 20					; 00000014H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 1794 : 	if (!szExt || !szFilePath)

	cmp	DWORD PTR _szExt$[ebp], 0
	je	SHORT $L76203
	cmp	DWORD PTR _szFilePath$[ebp], 0
	jne	SHORT $L76202
$L76203:

; 1795 : 		return FALSE;

	xor	eax, eax
	jmp	$L76201
$L76202:

; 1796 : 
; 1797 : 	if (szExt[0] == '.')

	mov	eax, DWORD PTR _szExt$[ebp]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax]
	cmp	ecx, 46					; 0000002eH
	jne	SHORT $L76204

; 1798 : 		szExt++;

	mov	edx, DWORD PTR _szExt$[ebp]
	add	edx, 2
	mov	DWORD PTR _szExt$[ebp], edx
$L76204:

; 1799 : 
; 1800 : 	CString sExt = GetExtension(szFilePath);

	mov	eax, DWORD PTR _szFilePath$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sExt$[ebp]
	push	ecx
	call	?GetExtension@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::GetExtension
	add	esp, 8
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1801 : 
; 1802 : 	if (sExt.GetLength() && sExt[0] == '.')

	lea	ecx, DWORD PTR _sExt$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L76207
	push	0
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	and	eax, 65535				; 0000ffffH
	cmp	eax, 46					; 0000002eH
	jne	SHORT $L76207

; 1803 : 		sExt = sExt.Mid(1);

	push	1
	lea	edx, DWORD PTR $T77619[ebp]
	push	edx
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	?Mid@CString@@QBE?AV1@H@Z		; CString::Mid
	mov	DWORD PTR -28+[ebp], eax
	mov	eax, DWORD PTR -28+[ebp]
	mov	DWORD PTR -32+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -32+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T77619[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76207:

; 1804 : 
; 1805 : 	return (sExt.CompareNoCase(szExt) == 0);

	mov	edx, DWORD PTR _szExt$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	?CompareNoCase@CString@@QBEHPBG@Z	; CString::CompareNoCase
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR $T77620[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77620[ebp]
$L76201:

; 1806 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77621:
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77622:
	lea	ecx, DWORD PTR $T77619[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77625:
	mov	eax, OFFSET FLAT:$T77624
	jmp	___CxxFrameHandler
text$x	ENDS
?HasExtension@FileMisc@@YAHPBG0@Z ENDP			; FileMisc::HasExtension
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77636	DD	019930520H
	DD	02H
	DD	FLAT:$T77638
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77638	DD	0ffffffffH
	DD	FLAT:$L77632
	DD	00H
	DD	FLAT:$L77631
xdata$x	ENDS
;	COMDAT ?GetExtension@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT
_szFilePath$ = 12
___$ReturnUdt$ = 8
_sExt$ = -16
$T77633 = -20
__$EHRec$ = -12
?GetExtension@FileMisc@@YA?AVCString@@PBG@Z PROC NEAR	; FileMisc::GetExtension, COMDAT

; 1809 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77637
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T77633[ebp], 0

; 1810 : 	CString sExt;

	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1811 : 	SplitPath(szFilePath, NULL, NULL, NULL, &sExt);

	lea	eax, DWORD PTR _sExt$[ebp]
	push	eax
	push	0
	push	0
	push	0
	mov	ecx, DWORD PTR _szFilePath$[ebp]
	push	ecx
	call	?SplitPath@FileMisc@@YAXPBGPAVCString@@111@Z ; FileMisc::SplitPath
	add	esp, 20					; 00000014H

; 1812 : 
; 1813 : 	return sExt;

	lea	edx, DWORD PTR _sExt$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T77633[ebp]
	or	al, 1
	mov	DWORD PTR $T77633[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1814 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77631:
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77632:
	mov	eax, DWORD PTR $T77633[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77634
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77634:
	ret	0
$L77637:
	mov	eax, OFFSET FLAT:$T77636
	jmp	___CxxFrameHandler
text$x	ENDS
?GetExtension@FileMisc@@YA?AVCString@@PBG@Z ENDP	; FileMisc::GetExtension
EXTRN	__imp___wsplitpath:NEAR
;	COMDAT ?SplitPath@FileMisc@@YAXPBGPAVCString@@111@Z
_TEXT	SEGMENT
_szPath$ = 8
_pDrive$ = 12
_pDir$ = 16
_pFName$ = 20
_pExt$ = 24
_szDrive$ = -1040
_szFolder$ = -1032
_szFile$ = -1556
_szExt$ = -516
?SplitPath@FileMisc@@YAXPBGPAVCString@@111@Z PROC NEAR	; FileMisc::SplitPath, COMDAT

; 1817 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 1556				; 00000614H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-1556]
	mov	ecx, 389				; 00000185H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1818 : 	TCHAR szDrive[_MAX_DRIVE+1] = { 0 }, 

	mov	WORD PTR _szDrive$[ebp], 0
	xor	eax, eax
	mov	DWORD PTR _szDrive$[ebp+2], eax
	mov	WORD PTR _szDrive$[ebp+6], ax

; 1819 : 			szFolder[_MAX_DIR+1] = { 0 }, 

	mov	WORD PTR _szFolder$[ebp], 0
	mov	ecx, 128				; 00000080H
	xor	eax, eax
	lea	edi, DWORD PTR _szFolder$[ebp+2]
	rep stosd

; 1820 : 			szFile[_MAX_FNAME+1] = { 0 }, 

	mov	WORD PTR _szFile$[ebp], 0
	mov	ecx, 128				; 00000080H
	xor	eax, eax
	lea	edi, DWORD PTR _szFile$[ebp+2]
	rep stosd

; 1821 : 			szExt[_MAX_EXT+1] = { 0 };

	mov	WORD PTR _szExt$[ebp], 0
	mov	ecx, 128				; 00000080H
	xor	eax, eax
	lea	edi, DWORD PTR _szExt$[ebp+2]
	rep stosd

; 1822 : 
; 1823 : #if _MSC_VER >= 1400
; 1824 : 	_tsplitpath_s(szPath, szDrive,_MAX_DRIVE, szFolder,_MAX_DIR, szFile, _MAX_FNAME, szExt, _MAX_EXT);
; 1825 : #else
; 1826 : 	_tsplitpath(szPath, szDrive, szFolder, szFile, szExt);

	mov	esi, esp
	lea	ecx, DWORD PTR _szExt$[ebp]
	push	ecx
	lea	edx, DWORD PTR _szFile$[ebp]
	push	edx
	lea	eax, DWORD PTR _szFolder$[ebp]
	push	eax
	lea	ecx, DWORD PTR _szDrive$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szPath$[ebp]
	push	edx
	call	DWORD PTR __imp___wsplitpath
	add	esp, 20					; 00000014H
	cmp	esi, esp
	call	__chkesp

; 1827 : #endif
; 1828 : 
; 1829 : 	if (pDrive)

	cmp	DWORD PTR _pDrive$[ebp], 0
	je	SHORT $L76226

; 1830 : 		*pDrive = szDrive;

	lea	eax, DWORD PTR _szDrive$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDrive$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L76226:

; 1831 : 
; 1832 : 	if (pDir)

	cmp	DWORD PTR _pDir$[ebp], 0
	je	SHORT $L76227

; 1833 : 		*pDir = szFolder;

	lea	ecx, DWORD PTR _szFolder$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDir$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L76227:

; 1834 : 
; 1835 : 	if (pFName)

	cmp	DWORD PTR _pFName$[ebp], 0
	je	SHORT $L76228

; 1836 : 		*pFName = szFile;

	lea	edx, DWORD PTR _szFile$[ebp]
	push	edx
	mov	ecx, DWORD PTR _pFName$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L76228:

; 1837 : 
; 1838 : 	if (pExt)

	cmp	DWORD PTR _pExt$[ebp], 0
	je	SHORT $L76229

; 1839 : 		*pExt = szExt;

	lea	eax, DWORD PTR _szExt$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pExt$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L76229:

; 1840 : }

	pop	edi
	pop	esi
	add	esp, 1556				; 00000614H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?SplitPath@FileMisc@@YAXPBGPAVCString@@111@Z ENDP	; FileMisc::SplitPath
_TEXT	ENDS
EXTRN	__imp___wmakepath:NEAR
;	COMDAT ?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z
_TEXT	SEGMENT
_sPath$ = 8
_szDrive$ = 12
_szDir$ = 16
_szFName$ = 20
_szExt$ = 24
_szPath$ = -524
?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z PROC NEAR ; FileMisc::MakePath, COMDAT

; 1843 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 524				; 0000020cH
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-524]
	mov	ecx, 131				; 00000083H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1844 : 	TCHAR szPath[MAX_PATH+1] = { 0 };

	mov	WORD PTR _szPath$[ebp], 0
	mov	ecx, 130				; 00000082H
	xor	eax, eax
	lea	edi, DWORD PTR _szPath$[ebp+2]
	rep stosd

; 1845 : 
; 1846 : #if _MSC_VER >= 1400
; 1847 : 	_tmakepath_s(szPath, MAX_PATH, szDrive, szDir, szFName, szExt);
; 1848 : #else
; 1849 : 	_tmakepath(szPath, szDrive, szDir, szFName, szExt);

	mov	esi, esp
	mov	eax, DWORD PTR _szExt$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szFName$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szDir$[ebp]
	push	edx
	mov	eax, DWORD PTR _szDrive$[ebp]
	push	eax
	lea	ecx, DWORD PTR _szPath$[ebp]
	push	ecx
	call	DWORD PTR __imp___wmakepath
	add	esp, 20					; 00000014H
	cmp	esi, esp
	call	__chkesp

; 1850 : #endif
; 1851 : 
; 1852 : 	sPath = szPath;

	lea	edx, DWORD PTR _szPath$[ebp]
	push	edx
	mov	ecx, DWORD PTR _sPath$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 1853 : 
; 1854 : 	return sPath;

	mov	eax, DWORD PTR _sPath$[ebp]

; 1855 : }

	pop	edi
	pop	esi
	add	esp, 524				; 0000020cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z ENDP	; FileMisc::MakePath
_TEXT	ENDS
PUBLIC	?GetFullPath@FileMisc@@YA?AVCString@@ABV2@0@Z	; FileMisc::GetFullPath
EXTRN	__imp__PathCanonicalizeW@8:NEAR
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG76249 DB	':', 00H, '\', 00H, 00H, 00H
	ORG $+2
$SG76250 DB	'\', 00H, '\', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77664	DD	019930520H
	DD	06H
	DD	FLAT:$T77666
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77666	DD	0ffffffffH
	DD	FLAT:$L77660
	DD	00H
	DD	FLAT:$L77653
	DD	01H
	DD	FLAT:$L77654
	DD	02H
	DD	FLAT:$L77655
	DD	01H
	DD	FLAT:$L77656
	DD	01H
	DD	FLAT:$L77658
xdata$x	ENDS
;	COMDAT ?GetFullPath@FileMisc@@YA?AVCString@@ABV2@0@Z
_TEXT	SEGMENT
_sFilePath$ = 12
_sRelativeToFolder$ = 16
___$ReturnUdt$ = 8
_sFullPath$ = -540
_sDrive$76251 = -548
_szFullPath$ = -536
_bRes$ = -544
$T77647 = -552
$T77648 = -556
$T77649 = -560
$T77650 = -564
$T77657 = -568
__$EHRec$ = -12
?GetFullPath@FileMisc@@YA?AVCString@@ABV2@0@Z PROC NEAR	; FileMisc::GetFullPath, COMDAT

; 1858 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77665
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 584				; 00000248H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-596]
	mov	ecx, 146				; 00000092H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T77657[ebp], 0

; 1859 : 	CString sFullPath;

	lea	ecx, DWORD PTR _sFullPath$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1860 : 
; 1861 : 	if (::PathIsRelative(sFilePath))

	mov	ecx, DWORD PTR _sFilePath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__PathIsRelativeW@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	$L76244

; 1863 : 		sFullPath = FileMisc::TerminatePath(sRelativeToFolder) + sFilePath;

	mov	eax, DWORD PTR _sFilePath$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sRelativeToFolder$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR $T77647[ebp]
	push	ecx
	call	?TerminatePath@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::TerminatePath
	add	esp, 8
	mov	DWORD PTR -572+[ebp], eax
	mov	edx, DWORD PTR -572+[ebp]
	mov	DWORD PTR -576+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	eax, DWORD PTR -576+[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77648[ebp]
	push	ecx
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -580+[ebp], eax
	mov	edx, DWORD PTR -580+[ebp]
	mov	DWORD PTR -584+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	eax, DWORD PTR -584+[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFullPath$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T77648[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T77647[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1865 : 	else // special case: filename has no drive letter and is not UNC

	jmp	$L76252
$L76244:

; 1867 : 		if ((sFilePath.Find(_T(":\\")) == -1) && (sFilePath.Find(_T("\\\\")) == -1))

	push	OFFSET FLAT:$SG76249
	mov	ecx, DWORD PTR _sFilePath$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	cmp	eax, -1
	jne	SHORT $L76248
	push	OFFSET FLAT:$SG76250
	mov	ecx, DWORD PTR _sFilePath$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	cmp	eax, -1
	jne	SHORT $L76248

; 1869 : 			CString sDrive;

	lea	ecx, DWORD PTR _sDrive$76251[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 4

; 1870 : 
; 1871 : 			SplitPath(sRelativeToFolder, &sDrive);

	push	0
	push	0
	push	0
	lea	ecx, DWORD PTR _sDrive$76251[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sRelativeToFolder$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?SplitPath@FileMisc@@YAXPBGPAVCString@@111@Z ; FileMisc::SplitPath
	add	esp, 20					; 00000014H

; 1872 : 			MakePath(sFullPath, sDrive, NULL, sFilePath);

	push	0
	mov	ecx, DWORD PTR _sFilePath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	0
	lea	ecx, DWORD PTR _sDrive$76251[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	edx, DWORD PTR _sFullPath$[ebp]
	push	edx
	call	?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z ; FileMisc::MakePath
	add	esp, 20					; 00000014H

; 1873 : 		}

	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sDrive$76251[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1874 : 		else

	jmp	SHORT $L76252
$L76248:

; 1876 : 			sFullPath = sFilePath;

	mov	eax, DWORD PTR _sFilePath$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFullPath$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
$L76252:

; 1879 : 
; 1880 : 	TCHAR szFullPath[MAX_PATH+1] = { 0 };

	mov	WORD PTR _szFullPath$[ebp], 0
	mov	ecx, 130				; 00000082H
	xor	eax, eax
	lea	edi, DWORD PTR _szFullPath$[ebp+2]
	rep stosd

; 1881 : 	BOOL bRes = ::PathCanonicalize(szFullPath, sFullPath);

	lea	ecx, DWORD PTR _sFullPath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	lea	ecx, DWORD PTR _szFullPath$[ebp]
	push	ecx
	call	DWORD PTR __imp__PathCanonicalizeW@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bRes$[ebp], eax

; 1882 : 
; 1883 : 	return (bRes ? CString(szFullPath) : sFullPath);

	cmp	DWORD PTR _bRes$[ebp], 0
	je	SHORT $L77651
	lea	edx, DWORD PTR _szFullPath$[ebp]
	push	edx
	lea	ecx, DWORD PTR $T77650[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR -588+[ebp], eax
	mov	eax, DWORD PTR $T77657[ebp]
	or	al, 1
	mov	DWORD PTR $T77657[ebp], eax
	mov	ecx, DWORD PTR -588+[ebp]
	mov	DWORD PTR -592+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	mov	edx, DWORD PTR -592+[ebp]
	mov	DWORD PTR -596+[ebp], edx
	jmp	SHORT $L77652
$L77651:
	lea	eax, DWORD PTR _sFullPath$[ebp]
	mov	DWORD PTR -596+[ebp], eax
$L77652:
	mov	ecx, DWORD PTR -596+[ebp]
	mov	DWORD PTR $T77649[ebp], ecx
	mov	edx, DWORD PTR $T77649[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T77657[ebp]
	or	al, 2
	mov	DWORD PTR $T77657[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR $T77657[ebp]
	and	ecx, 1
	test	ecx, ecx
	je	SHORT $L77662
	and	DWORD PTR $T77657[ebp], -2		; fffffffeH
	lea	ecx, DWORD PTR $T77650[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77662:
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sFullPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1884 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 596				; 00000254H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77653:
	lea	ecx, DWORD PTR _sFullPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77654:
	lea	ecx, DWORD PTR $T77647[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77655:
	lea	ecx, DWORD PTR $T77648[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77656:
	lea	ecx, DWORD PTR _sDrive$76251[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77658:
	mov	eax, DWORD PTR $T77657[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77659
	lea	ecx, DWORD PTR $T77650[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77659:
	ret	0
$L77660:
	mov	eax, DWORD PTR $T77657[ebp]
	and	eax, 2
	test	eax, eax
	je	$L77661
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77661:
	ret	0
$L77665:
	mov	eax, OFFSET FLAT:$T77664
	jmp	___CxxFrameHandler
text$x	ENDS
?GetFullPath@FileMisc@@YA?AVCString@@ABV2@0@Z ENDP	; FileMisc::GetFullPath
PUBLIC	?MakeFullPath@FileMisc@@YAAAVCString@@AAV2@ABV2@@Z ; FileMisc::MakeFullPath
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77675	DD	019930520H
	DD	01H
	DD	FLAT:$T77677
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77677	DD	0ffffffffH
	DD	FLAT:$L77673
xdata$x	ENDS
;	COMDAT ?MakeFullPath@FileMisc@@YAAAVCString@@AAV2@ABV2@@Z
_TEXT	SEGMENT
_sFilePath$ = 8
_sRelativeToFolder$ = 12
$T77672 = -16
__$EHRec$ = -12
?MakeFullPath@FileMisc@@YAAAVCString@@AAV2@ABV2@@Z PROC NEAR ; FileMisc::MakeFullPath, COMDAT

; 1887 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77676
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH

; 1888 : 	sFilePath = GetFullPath(sFilePath, sRelativeToFolder);

	mov	eax, DWORD PTR _sRelativeToFolder$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sFilePath$[ebp]
	push	ecx
	lea	edx, DWORD PTR $T77672[ebp]
	push	edx
	call	?GetFullPath@FileMisc@@YA?AVCString@@ABV2@0@Z ; FileMisc::GetFullPath
	add	esp, 12					; 0000000cH
	mov	DWORD PTR -20+[ebp], eax
	mov	eax, DWORD PTR -20+[ebp]
	mov	DWORD PTR -24+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -24+[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sFilePath$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T77672[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1889 : 	return sFilePath;

	mov	eax, DWORD PTR _sFilePath$[ebp]

; 1890 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77673:
	lea	ecx, DWORD PTR $T77672[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77676:
	mov	eax, OFFSET FLAT:$T77675
	jmp	___CxxFrameHandler
text$x	ENDS
?MakeFullPath@FileMisc@@YAAAVCString@@AAV2@ABV2@@Z ENDP	; FileMisc::MakeFullPath
PUBLIC	?GetRelativePath@FileMisc@@YA?AVCString@@ABV2@0H@Z ; FileMisc::GetRelativePath
EXTRN	__imp__PathRelativePathToW@20:NEAR
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77695	DD	019930520H
	DD	03H
	DD	FLAT:$T77697
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77697	DD	0ffffffffH
	DD	FLAT:$L77687
	DD	00H
	DD	FLAT:$L77690
	DD	00H
	DD	FLAT:$L77691
xdata$x	ENDS
;	COMDAT ?GetRelativePath@FileMisc@@YA?AVCString@@ABV2@0H@Z
_TEXT	SEGMENT
_sFilePath$ = 12
_sRelativeToFolder$ = 16
_bFolder$ = 20
___$ReturnUdt$ = 8
_szRelPath$ = -536
_bRes$ = -540
$T77682 = -544
$T77683 = -548
$T77684 = -552
$T77688 = -556
__$EHRec$ = -12
?GetRelativePath@FileMisc@@YA?AVCString@@ABV2@0H@Z PROC NEAR ; FileMisc::GetRelativePath, COMDAT

; 1893 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77696
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 564				; 00000234H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-576]
	mov	ecx, 141				; 0000008dH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T77688[ebp], 0

; 1894 : 	if (::PathIsRelative(sFilePath))

	mov	ecx, DWORD PTR _sFilePath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__PathIsRelativeW@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L76268

; 1895 : 		return sFilePath;

	mov	eax, DWORD PTR _sFilePath$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T77688[ebp]
	or	ecx, 1
	mov	DWORD PTR $T77688[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L76267
$L76268:

; 1896 : 
; 1897 : 	TCHAR szRelPath[MAX_PATH+1] = { 0 };

	mov	WORD PTR _szRelPath$[ebp], 0
	mov	ecx, 130				; 00000082H
	xor	eax, eax
	lea	edi, DWORD PTR _szRelPath$[ebp+2]
	rep stosd

; 1898 : 
; 1899 : 	BOOL bRes = ::PathRelativePathTo(szRelPath, 
; 1900 : 									UnterminatePath(sRelativeToFolder), FILE_ATTRIBUTE_DIRECTORY, 
; 1901 : 									sFilePath, bFolder ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL);

	mov	edx, DWORD PTR _bFolder$[ebp]
	neg	edx
	sbb	edx, edx
	and	edx, -112				; ffffff90H
	add	edx, 128				; 00000080H
	mov	esi, esp
	push	edx
	mov	ecx, DWORD PTR _sFilePath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	16					; 00000010H
	mov	ecx, DWORD PTR _sRelativeToFolder$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	eax, DWORD PTR $T77682[ebp]
	push	eax
	call	?UnterminatePath@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::UnterminatePath
	add	esp, 8
	mov	DWORD PTR -560+[ebp], eax
	mov	ecx, DWORD PTR -560+[ebp]
	mov	DWORD PTR -564+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -564+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	edx, DWORD PTR _szRelPath$[ebp]
	push	edx
	call	DWORD PTR __imp__PathRelativePathToW@20
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bRes$[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T77682[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1902 : 
; 1903 : 	return bRes ? CString(szRelPath) : sFilePath;

	cmp	DWORD PTR _bRes$[ebp], 0
	je	SHORT $L77685
	lea	eax, DWORD PTR _szRelPath$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77684[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR -568+[ebp], eax
	mov	ecx, DWORD PTR $T77688[ebp]
	or	ecx, 2
	mov	DWORD PTR $T77688[ebp], ecx
	mov	edx, DWORD PTR -568+[ebp]
	mov	DWORD PTR -572+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 2
	mov	eax, DWORD PTR -572+[ebp]
	mov	DWORD PTR -576+[ebp], eax
	jmp	SHORT $L77686
$L77685:
	mov	ecx, DWORD PTR _sFilePath$[ebp]
	mov	DWORD PTR -576+[ebp], ecx
$L77686:
	mov	edx, DWORD PTR -576+[ebp]
	mov	DWORD PTR $T77683[ebp], edx
	mov	eax, DWORD PTR $T77683[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T77688[ebp]
	or	ecx, 1
	mov	DWORD PTR $T77688[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	edx, DWORD PTR $T77688[ebp]
	and	edx, 2
	test	edx, edx
	je	SHORT $L77693
	and	DWORD PTR $T77688[ebp], -3		; fffffffdH
	lea	ecx, DWORD PTR $T77684[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77693:
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L76267:

; 1904 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 576				; 00000240H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77687:
	mov	eax, DWORD PTR $T77688[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77689
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77689:
	ret	0
$L77690:
	lea	ecx, DWORD PTR $T77682[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77691:
	mov	eax, DWORD PTR $T77688[ebp]
	and	eax, 2
	test	eax, eax
	je	$L77692
	lea	ecx, DWORD PTR $T77684[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77692:
	ret	0
$L77696:
	mov	eax, OFFSET FLAT:$T77695
	jmp	___CxxFrameHandler
text$x	ENDS
?GetRelativePath@FileMisc@@YA?AVCString@@ABV2@0H@Z ENDP	; FileMisc::GetRelativePath
PUBLIC	?MakeRelativePath@FileMisc@@YAAAVCString@@AAV2@ABV2@H@Z ; FileMisc::MakeRelativePath
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77705	DD	019930520H
	DD	01H
	DD	FLAT:$T77707
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77707	DD	0ffffffffH
	DD	FLAT:$L77703
xdata$x	ENDS
;	COMDAT ?MakeRelativePath@FileMisc@@YAAAVCString@@AAV2@ABV2@H@Z
_TEXT	SEGMENT
_sFilePath$ = 8
_sRelativeToFolder$ = 12
_bFolder$ = 16
$T77702 = -16
__$EHRec$ = -12
?MakeRelativePath@FileMisc@@YAAAVCString@@AAV2@ABV2@H@Z PROC NEAR ; FileMisc::MakeRelativePath, COMDAT

; 1907 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77706
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH

; 1908 : 	sFilePath = GetRelativePath(sFilePath, sRelativeToFolder, bFolder);

	mov	eax, DWORD PTR _bFolder$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sRelativeToFolder$[ebp]
	push	ecx
	mov	edx, DWORD PTR _sFilePath$[ebp]
	push	edx
	lea	eax, DWORD PTR $T77702[ebp]
	push	eax
	call	?GetRelativePath@FileMisc@@YA?AVCString@@ABV2@0H@Z ; FileMisc::GetRelativePath
	add	esp, 16					; 00000010H
	mov	DWORD PTR -20+[ebp], eax
	mov	ecx, DWORD PTR -20+[ebp]
	mov	DWORD PTR -24+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	edx, DWORD PTR -24+[ebp]
	push	edx
	mov	ecx, DWORD PTR _sFilePath$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T77702[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1909 : 	return sFilePath;

	mov	eax, DWORD PTR _sFilePath$[ebp]

; 1910 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77703:
	lea	ecx, DWORD PTR $T77702[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77706:
	mov	eax, OFFSET FLAT:$T77705
	jmp	___CxxFrameHandler
text$x	ENDS
?MakeRelativePath@FileMisc@@YAAAVCString@@AAV2@ABV2@H@Z ENDP ; FileMisc::MakeRelativePath
PUBLIC	?IsSamePath@FileMisc@@YAHABVCString@@0@Z	; FileMisc::IsSamePath
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77720	DD	019930520H
	DD	06H
	DD	FLAT:$T77722
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77722	DD	0ffffffffH
	DD	FLAT:$L77715
	DD	00H
	DD	FLAT:$L77716
	DD	0ffffffffH
	DD	FLAT:$L77716
	DD	02H
	DD	FLAT:$L77717
	DD	03H
	DD	FLAT:$L77718
	DD	02H
	DD	FLAT:$L77718
xdata$x	ENDS
;	COMDAT ?IsSamePath@FileMisc@@YAHABVCString@@0@Z
_TEXT	SEGMENT
_sPath1$ = 8
_sPath2$ = 12
_sFullPath1$ = -16
_sFullPath2$ = -20
$T77712 = -24
$T77713 = -28
$T77714 = -32
__$EHRec$ = -12
?IsSamePath@FileMisc@@YAHABVCString@@0@Z PROC NEAR	; FileMisc::IsSamePath, COMDAT

; 1913 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77721
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 36					; 00000024H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-48], eax
	mov	DWORD PTR [ebp-44], eax
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 1914 : 	CString sFullPath1 = GetFullPath(sPath1);

	lea	eax, DWORD PTR $T77712[ebp]
	push	eax
	call	?GetCwd@FileMisc@@YA?AVCString@@XZ	; FileMisc::GetCwd
	add	esp, 4
	mov	DWORD PTR -36+[ebp], eax
	mov	ecx, DWORD PTR -36+[ebp]
	mov	DWORD PTR -40+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	edx, DWORD PTR -40+[ebp]
	push	edx
	mov	eax, DWORD PTR _sPath1$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFullPath1$[ebp]
	push	ecx
	call	?GetFullPath@FileMisc@@YA?AVCString@@ABV2@0@Z ; FileMisc::GetFullPath
	add	esp, 12					; 0000000cH
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T77712[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1915 : 	CString sFullPath2 = GetFullPath(sPath2);

	lea	edx, DWORD PTR $T77713[ebp]
	push	edx
	call	?GetCwd@FileMisc@@YA?AVCString@@XZ	; FileMisc::GetCwd
	add	esp, 4
	mov	DWORD PTR -44+[ebp], eax
	mov	eax, DWORD PTR -44+[ebp]
	mov	DWORD PTR -48+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR -48+[ebp]
	push	ecx
	mov	edx, DWORD PTR _sPath2$[ebp]
	push	edx
	lea	eax, DWORD PTR _sFullPath2$[ebp]
	push	eax
	call	?GetFullPath@FileMisc@@YA?AVCString@@ABV2@0@Z ; FileMisc::GetFullPath
	add	esp, 12					; 0000000cH
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	lea	ecx, DWORD PTR $T77713[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 1916 : 
; 1917 : 	return (sFullPath1.CompareNoCase(sFullPath2) == 0);

	lea	ecx, DWORD PTR _sFullPath2$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sFullPath1$[ebp]
	call	?CompareNoCase@CString@@QBEHPBG@Z	; CString::CompareNoCase
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR $T77714[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sFullPath2$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sFullPath1$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77714[ebp]

; 1918 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 48					; 00000030H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77715:
	lea	ecx, DWORD PTR $T77712[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77716:
	lea	ecx, DWORD PTR _sFullPath1$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77717:
	lea	ecx, DWORD PTR $T77713[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77718:
	lea	ecx, DWORD PTR _sFullPath2$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77721:
	mov	eax, OFFSET FLAT:$T77720
	jmp	___CxxFrameHandler
text$x	ENDS
?IsSamePath@FileMisc@@YAHABVCString@@0@Z ENDP		; FileMisc::IsSamePath
PUBLIC	?GetSpecialFolder@FileMisc@@YAHHAAVCString@@H@Z	; FileMisc::GetSpecialFolder
EXTRN	__imp__SHGetSpecialFolderPathW@16:NEAR
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG76301 DB	'D:\_CODE\Shared\FILEMISC.CPP', 00H
data	ENDS
;	COMDAT ?GetSpecialFolder@FileMisc@@YAHHAAVCString@@H@Z
_TEXT	SEGMENT
_nCSIDL$ = 8
_sFolder$ = 12
_bAutoCreate$ = 16
_bRes$ = -4
?GetSpecialFolder@FileMisc@@YAHHAAVCString@@H@Z PROC NEAR ; FileMisc::GetSpecialFolder, COMDAT

; 1921 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1922 : 	BOOL bRes = SHGetSpecialFolderPath(NULL, sFolder.GetBuffer(MAX_PATH), nCSIDL, bAutoCreate);

	mov	esi, esp
	mov	eax, DWORD PTR _bAutoCreate$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nCSIDL$[ebp]
	push	ecx
	push	260					; 00000104H
	mov	ecx, DWORD PTR _sFolder$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	push	0
	call	DWORD PTR __imp__SHGetSpecialFolderPathW@16
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bRes$[ebp], eax

; 1923 : 	sFolder.ReleaseBuffer();

	push	-1
	mov	ecx, DWORD PTR _sFolder$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer
$L76297:

; 1924 : 
; 1925 : 	// ought not to fail
; 1926 : 	ASSERT(bRes);

	cmp	DWORD PTR _bRes$[ebp], 0
	jne	SHORT $L76300
	push	1926					; 00000786H
	push	OFFSET FLAT:$SG76301
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76300
	int	3
$L76300:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76297

; 1927 : 
; 1928 : 	return bRes;

	mov	eax, DWORD PTR _bRes$[ebp]

; 1929 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetSpecialFolder@FileMisc@@YAHHAAVCString@@H@Z ENDP	; FileMisc::GetSpecialFolder
_TEXT	ENDS
;	COMDAT ?GetSpecialFilePath@FileMisc@@YAHHABVCString@@AAV2@@Z
_TEXT	SEGMENT
_nCSIDL$ = 8
_sFileName$ = 12
_sFilePath$ = 16
?GetSpecialFilePath@FileMisc@@YAHHABVCString@@AAV2@@Z PROC NEAR ; FileMisc::GetSpecialFilePath, COMDAT

; 1932 : {

	push	ebp
	mov	ebp, esp

; 1933 : 	if (GetSpecialFolder(nCSIDL, sFilePath))

	push	0
	mov	eax, DWORD PTR _sFilePath$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nCSIDL$[ebp]
	push	ecx
	call	?GetSpecialFolder@FileMisc@@YAHHAAVCString@@H@Z ; FileMisc::GetSpecialFolder
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	SHORT $L76307

; 1935 : 		// append filename if present
; 1936 : 		if (!sFileName.IsEmpty())

	mov	ecx, DWORD PTR _sFileName$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L76308

; 1937 : 			TerminatePath(sFilePath) += sFileName;

	mov	edx, DWORD PTR _sFileName$[ebp]
	push	edx
	mov	eax, DWORD PTR _sFilePath$[ebp]
	push	eax
	call	?TerminatePath@FileMisc@@YAAAVCString@@AAV2@@Z ; FileMisc::TerminatePath
	add	esp, 4
	mov	ecx, eax
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
$L76308:

; 1938 : 
; 1939 : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76306
$L76307:

; 1941 : 
; 1942 : 	// else
; 1943 : 	return FALSE;

	xor	eax, eax
$L76306:

; 1944 : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?GetSpecialFilePath@FileMisc@@YAHHABVCString@@AAV2@@Z ENDP ; FileMisc::GetSpecialFilePath
_TEXT	ENDS
PUBLIC	?IsPathInSpecialFolder@FileMisc@@YAHABVCString@@H@Z ; FileMisc::IsPathInSpecialFolder
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77738	DD	019930520H
	DD	02H
	DD	FLAT:$T77740
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77740	DD	0ffffffffH
	DD	FLAT:$L77735
	DD	00H
	DD	FLAT:$L77736
xdata$x	ENDS
;	COMDAT ?IsPathInSpecialFolder@FileMisc@@YAHABVCString@@H@Z
_TEXT	SEGMENT
_sFilePath$ = 8
_nCSIDL$ = 12
_sSpecialFolder$ = -16
$T77732 = -20
$T77733 = -24
$T77734 = -28
__$EHRec$ = -12
?IsPathInSpecialFolder@FileMisc@@YAHABVCString@@H@Z PROC NEAR ; FileMisc::IsPathInSpecialFolder, COMDAT

; 1947 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77739
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 1948 : 	CString sSpecialFolder;

	lea	ecx, DWORD PTR _sSpecialFolder$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1949 : 	
; 1950 : 	if (!GetSpecialFolder(nCSIDL, sSpecialFolder))

	push	0
	lea	eax, DWORD PTR _sSpecialFolder$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nCSIDL$[ebp]
	push	ecx
	call	?GetSpecialFolder@FileMisc@@YAHHAAVCString@@H@Z ; FileMisc::GetSpecialFolder
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	SHORT $L76314

; 1951 : 		return FALSE;

	mov	DWORD PTR $T77732[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sSpecialFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77732[ebp]
	jmp	SHORT $L76312
$L76314:

; 1952 : 	
; 1953 : 	return (sFilePath.Left(sSpecialFolder.GetLength()).CompareNoCase(sSpecialFolder) == 0);

	lea	ecx, DWORD PTR _sSpecialFolder$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sSpecialFolder$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	push	eax
	lea	edx, DWORD PTR $T77734[ebp]
	push	edx
	mov	ecx, DWORD PTR _sFilePath$[ebp]
	call	?Left@CString@@QBE?AV1@H@Z		; CString::Left
	mov	DWORD PTR -32+[ebp], eax
	mov	eax, DWORD PTR -32+[ebp]
	mov	DWORD PTR -36+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -36+[ebp]
	call	?CompareNoCase@CString@@QBEHPBG@Z	; CString::CompareNoCase
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR $T77733[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T77734[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sSpecialFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77733[ebp]
$L76312:

; 1954 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77735:
	lea	ecx, DWORD PTR _sSpecialFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77736:
	lea	ecx, DWORD PTR $T77734[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77739:
	mov	eax, OFFSET FLAT:$T77738
	jmp	___CxxFrameHandler
text$x	ENDS
?IsPathInSpecialFolder@FileMisc@@YAHABVCString@@H@Z ENDP ; FileMisc::IsPathInSpecialFolder
PUBLIC	?IsAppInProgramFiles@FileMisc@@YAHXZ		; FileMisc::IsAppInProgramFiles
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77749	DD	019930520H
	DD	01H
	DD	FLAT:$T77751
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77751	DD	0ffffffffH
	DD	FLAT:$L77747
xdata$x	ENDS
;	COMDAT ?IsAppInProgramFiles@FileMisc@@YAHXZ
_TEXT	SEGMENT
$T77745 = -16
$T77746 = -20
__$EHRec$ = -12
?IsAppInProgramFiles@FileMisc@@YAHXZ PROC NEAR		; FileMisc::IsAppInProgramFiles, COMDAT

; 1957 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77750
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 1958 : 	return IsPathInSpecialFolder(GetAppFileName(), CSIDL_PROGRAM_FILES);

	push	38					; 00000026H
	lea	eax, DWORD PTR $T77746[ebp]
	push	eax
	call	?GetAppFileName@FileMisc@@YA?AVCString@@XZ ; FileMisc::GetAppFileName
	add	esp, 4
	mov	DWORD PTR -24+[ebp], eax
	mov	ecx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	edx, DWORD PTR -28+[ebp]
	push	edx
	call	?IsPathInSpecialFolder@FileMisc@@YAHABVCString@@H@Z ; FileMisc::IsPathInSpecialFolder
	add	esp, 8
	mov	DWORD PTR $T77745[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T77746[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77745[ebp]

; 1959 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77747:
	lea	ecx, DWORD PTR $T77746[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77750:
	mov	eax, OFFSET FLAT:$T77749
	jmp	___CxxFrameHandler
text$x	ENDS
?IsAppInProgramFiles@FileMisc@@YAHXZ ENDP		; FileMisc::IsAppInProgramFiles
PUBLIC	?ResolveShortcut@FileMisc@@YA?AVCString@@PBG@Z	; FileMisc::ResolveShortcut
EXTRN	__imp__CoInitialize@4:NEAR
EXTRN	__imp__CoUninitialize@0:NEAR
EXTRN	_IID_IPersistFile:BYTE
EXTRN	__imp__CoCreateInstance@20:NEAR
EXTRN	_CLSID_ShellLink:BYTE
EXTRN	_IID_IShellLinkW:BYTE
EXTRN	__imp__wcscpy:NEAR
EXTRN	__imp__lstrcpyW@8:NEAR
_BSS	SEGMENT
	ALIGN	4

$SG76327 DB	01H DUP (?)
_BSS	ENDS
;	COMDAT xdata$x
; File D:\_CODE\Shared\FILEMISC.CPP
xdata$x	SEGMENT
$T77763	DD	019930520H
	DD	03H
	DD	FLAT:$T77765
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77765	DD	0ffffffffH
	DD	FLAT:$L77757
	DD	00H
	DD	FLAT:$L77760
	DD	01H
	DD	FLAT:$L77761
xdata$x	ENDS
;	COMDAT ?ResolveShortcut@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT
_szShortcut$ = 12
___$ReturnUdt$ = 8
_hResult$ = -24
_psl$ = -16
_sTarget$ = -20
_ppf$76334 = -28
_wsz$76338 = -552
_szPath$76343 = -1076
_wfd$76344 = -1668
$T77756 = -1672
$T77758 = -1676
__$EHRec$ = -12
?ResolveShortcut@FileMisc@@YA?AVCString@@PBG@Z PROC NEAR ; FileMisc::ResolveShortcut, COMDAT

; 1962 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77764
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 1672				; 00000688H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-1684]
	mov	ecx, 418				; 000001a2H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T77758[ebp], 0

; 1963 : 	// start by checking its a valid file
; 1964 : 	if (!FileExists(szShortcut))

	mov	eax, DWORD PTR _szShortcut$[ebp]
	push	eax
	call	?FileExists@FileMisc@@YAHPBG@Z		; FileMisc::FileExists
	add	esp, 4
	test	eax, eax
	jne	SHORT $L76326

; 1965 : 		return "";

	push	OFFSET FLAT:$SG76327
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	ecx, DWORD PTR $T77758[ebp]
	or	ecx, 1
	mov	DWORD PTR $T77758[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L76325
$L76326:

; 1966 : 
; 1967 :     CoInitialize(NULL);

	mov	esi, esp
	push	0
	call	DWORD PTR __imp__CoInitialize@4
	cmp	esi, esp
	call	__chkesp

; 1968 : 
; 1969 : 	HRESULT hResult;
; 1970 : 	IShellLink*	psl;
; 1971 : 	CString sTarget(szShortcut);

	mov	edx, DWORD PTR _szShortcut$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sTarget$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1972 : 	
; 1973 : 	hResult = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
; 1974 : 								IID_IShellLink, (LPVOID*)&psl);

	mov	esi, esp
	lea	eax, DWORD PTR _psl$[ebp]
	push	eax
	push	OFFSET FLAT:_IID_IShellLinkW
	push	1
	push	0
	push	OFFSET FLAT:_CLSID_ShellLink
	call	DWORD PTR __imp__CoCreateInstance@20
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hResult$[ebp], eax

; 1975 : 	
; 1976 : 	if (SUCCEEDED(hResult))

	cmp	DWORD PTR _hResult$[ebp], 0
	jl	$L76333

; 1978 : 		LPPERSISTFILE ppf;
; 1979 : 		
; 1980 : 		hResult = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

	mov	esi, esp
	lea	ecx, DWORD PTR _ppf$76334[ebp]
	push	ecx
	push	OFFSET FLAT:_IID_IPersistFile
	mov	edx, DWORD PTR _psl$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _psl$[ebp]
	push	ecx
	call	DWORD PTR [eax]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hResult$[ebp], eax

; 1981 : 
; 1982 : 		if (SUCCEEDED(hResult))

	cmp	DWORD PTR _hResult$[ebp], 0
	jl	$L76337

; 1984 : 			WCHAR wsz[MAX_PATH+1] = { 0 };	// buffer for Unicode string

	mov	WORD PTR _wsz$76338[ebp], 0
	mov	ecx, 130				; 00000082H
	xor	eax, eax
	lea	edi, DWORD PTR _wsz$76338[ebp+2]
	rep stosd

; 1985 : #ifndef _UNICODE
; 1986 : 			MultiByteToWideChar(CP_ACP, 0, szShortcut, -1, wsz, MAX_PATH);
; 1987 : #else
; 1988 : 			lstrcpy(wsz, szShortcut);

	mov	esi, esp
	mov	edx, DWORD PTR _szShortcut$[ebp]
	push	edx
	lea	eax, DWORD PTR _wsz$76338[ebp]
	push	eax
	call	DWORD PTR __imp__lstrcpyW@8
	cmp	esi, esp
	call	__chkesp

; 1989 : #endif
; 1990 : 			hResult = ppf->Load(wsz, STGM_READ);

	mov	esi, esp
	push	0
	lea	ecx, DWORD PTR _wsz$76338[ebp]
	push	ecx
	mov	edx, DWORD PTR _ppf$76334[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _ppf$76334[ebp]
	push	ecx
	call	DWORD PTR [eax+20]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hResult$[ebp], eax

; 1991 : 			
; 1992 : 			if (SUCCEEDED(hResult))

	cmp	DWORD PTR _hResult$[ebp], 0
	jl	$L76346

; 1994 : 				hResult = psl->Resolve(NULL, SLR_ANY_MATCH | SLR_NO_UI);

	mov	esi, esp
	push	3
	push	0
	mov	edx, DWORD PTR _psl$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _psl$[ebp]
	push	ecx
	call	DWORD PTR [eax+76]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hResult$[ebp], eax

; 1995 : 
; 1996 : 				if (SUCCEEDED(hResult))

	cmp	DWORD PTR _hResult$[ebp], 0
	jl	$L76346

; 1998 : 					TCHAR szPath[MAX_PATH+1] = { 0 };

	mov	WORD PTR _szPath$76343[ebp], 0
	mov	ecx, 130				; 00000082H
	xor	eax, eax
	lea	edi, DWORD PTR _szPath$76343[ebp+2]
	rep stosd

; 1999 : 					WIN32_FIND_DATA wfd;
; 2000 : //fabio_2005
; 2001 : #if _MSC_VER >= 1400
; 2002 : 					_tcscpy_s(szPath, szShortcut);
; 2003 : #else
; 2004 : 					_tcscpy(szPath, szShortcut);

	mov	esi, esp
	mov	edx, DWORD PTR _szShortcut$[ebp]
	push	edx
	lea	eax, DWORD PTR _szPath$76343[ebp]
	push	eax
	call	DWORD PTR __imp__wcscpy
	add	esp, 8
	cmp	esi, esp
	call	__chkesp

; 2005 : #endif
; 2006 : 					hResult = psl->GetPath(szPath, MAX_PATH, &wfd, SLGP_SHORTPATH);

	mov	esi, esp
	push	1
	lea	ecx, DWORD PTR _wfd$76344[ebp]
	push	ecx
	push	260					; 00000104H
	lea	edx, DWORD PTR _szPath$76343[ebp]
	push	edx
	mov	eax, DWORD PTR _psl$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR _psl$[ebp]
	push	edx
	call	DWORD PTR [ecx+12]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hResult$[ebp], eax

; 2007 : 
; 2008 : 					if (SUCCEEDED(hResult))

	cmp	DWORD PTR _hResult$[ebp], 0
	jl	SHORT $L76346

; 2009 : 						sTarget = CString(szPath);

	lea	eax, DWORD PTR _szPath$76343[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77756[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR -1680+[ebp], eax
	mov	ecx, DWORD PTR -1680+[ebp]
	mov	DWORD PTR -1684+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	edx, DWORD PTR -1684+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sTarget$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T77756[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76346:

; 2012 : 		
; 2013 : 			ppf->Release();

	mov	eax, DWORD PTR _ppf$76334[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	esi, esp
	mov	edx, DWORD PTR _ppf$76334[ebp]
	push	edx
	call	DWORD PTR [ecx+8]
	cmp	esi, esp
	call	__chkesp
$L76337:

; 2015 : 		
; 2016 : 		psl->Release();

	mov	eax, DWORD PTR _psl$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	esi, esp
	mov	edx, DWORD PTR _psl$[ebp]
	push	edx
	call	DWORD PTR [ecx+8]
	cmp	esi, esp
	call	__chkesp
$L76333:

; 2018 : 
; 2019 : 	CoUninitialize();

	mov	esi, esp
	call	DWORD PTR __imp__CoUninitialize@0
	cmp	esi, esp
	call	__chkesp

; 2020 : 	
; 2021 : 	return sTarget;

	lea	eax, DWORD PTR _sTarget$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T77758[ebp]
	or	ecx, 1
	mov	DWORD PTR $T77758[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sTarget$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L76325:

; 2022 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 1684				; 00000694H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77757:
	mov	eax, DWORD PTR $T77758[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77759
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77759:
	ret	0
$L77760:
	lea	ecx, DWORD PTR _sTarget$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77761:
	lea	ecx, DWORD PTR $T77756[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77764:
	mov	eax, OFFSET FLAT:$T77763
	jmp	___CxxFrameHandler
text$x	ENDS
?ResolveShortcut@FileMisc@@YA?AVCString@@PBG@Z ENDP	; FileMisc::ResolveShortcut
PUBLIC	?GetLongPathNameW@FileMisc@@YA?AVCString@@PBG@Z	; FileMisc::GetLongPathNameW
_BSS	SEGMENT
	ALIGN	4

_?hLib@?1??GetLongPathNameW@FileMisc@@YA?AVCString@@PBG@Z@4PAUHINSTANCE__@@A DD 01H DUP (?)
_?$S279@?1??GetLongPathNameW@FileMisc@@YA?AVCString@@PBG@Z@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG76358 DB	'k', 00H, 'e', 00H, 'r', 00H, 'n', 00H, 'e', 00H, 'l', 00H
	DB	'3', 00H, '2', 00H, '.', 00H, 'd', 00H, 'l', 00H, 'l', 00H, 00H
	DB	00H
	ORG $+2
$SG76366 DB	'GetLongPathNameW', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77775	DD	019930520H
	DD	02H
	DD	FLAT:$T77777
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77777	DD	0ffffffffH
	DD	FLAT:$L77771
	DD	00H
	DD	FLAT:$L77770
xdata$x	ENDS
;	COMDAT ?GetLongPathNameW@FileMisc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT
_szShortPath$ = 12
___$ReturnUdt$ = 8
_sLongPath$ = -16
_pFN$76364 = -20
_szLongPath$76368 = -544
$T77772 = -548
__$EHRec$ = -12
?GetLongPathNameW@FileMisc@@YA?AVCString@@PBG@Z PROC NEAR ; FileMisc::GetLongPathNameW, COMDAT

; 2026 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77776
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 536				; 00000218H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-548]
	mov	ecx, 134				; 00000086H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T77772[ebp], 0

; 2027 : 	CString sLongPath(szShortPath);

	mov	eax, DWORD PTR _szShortPath$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sLongPath$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 2028 : 
; 2029 : 	// must link dynamically to kernel32 else problem with win95/NT4
; 2030 : 	static HMODULE hLib = LoadLibrary(_T("kernel32.dll"));

	xor	ecx, ecx
	mov	cl, BYTE PTR _?$S279@?1??GetLongPathNameW@FileMisc@@YA?AVCString@@PBG@Z@4EA
	and	ecx, 1
	test	ecx, ecx
	jne	SHORT $L76357
	mov	dl, BYTE PTR _?$S279@?1??GetLongPathNameW@FileMisc@@YA?AVCString@@PBG@Z@4EA
	or	dl, 1
	mov	BYTE PTR _?$S279@?1??GetLongPathNameW@FileMisc@@YA?AVCString@@PBG@Z@4EA, dl
	mov	esi, esp
	push	OFFSET FLAT:$SG76358
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?hLib@?1??GetLongPathNameW@FileMisc@@YA?AVCString@@PBG@Z@4PAUHINSTANCE__@@A, eax
$L76357:

; 2031 : 
; 2032 : 	if (hLib)

	cmp	DWORD PTR _?hLib@?1??GetLongPathNameW@FileMisc@@YA?AVCString@@PBG@Z@4PAUHINSTANCE__@@A, 0
	je	SHORT $L76367

; 2034 : 		typedef DWORD (WINAPI *FNGETLONGPATHNAME)(LPCTSTR, LPTSTR, DWORD);
; 2035 : 
; 2036 : #ifdef _UNICODE
; 2037 : 		FNGETLONGPATHNAME pFN = (FNGETLONGPATHNAME)GetProcAddress(hLib, "GetLongPathNameW");

	mov	esi, esp
	push	OFFSET FLAT:$SG76366
	mov	eax, DWORD PTR _?hLib@?1??GetLongPathNameW@FileMisc@@YA?AVCString@@PBG@Z@4PAUHINSTANCE__@@A
	push	eax
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pFN$76364[ebp], eax

; 2038 : #else
; 2039 : 		FNGETLONGPATHNAME pFN = (FNGETLONGPATHNAME)GetProcAddress(hLib, "GetLongPathNameA");
; 2040 : #endif
; 2041 : 		if (pFN)

	cmp	DWORD PTR _pFN$76364[ebp], 0
	je	SHORT $L76367

; 2043 : 			TCHAR szLongPath[MAX_PATH+1] = { 0 };

	mov	WORD PTR _szLongPath$76368[ebp], 0
	mov	ecx, 130				; 00000082H
	xor	eax, eax
	lea	edi, DWORD PTR _szLongPath$76368[ebp+2]
	rep stosd

; 2044 : 			pFN(szShortPath, szLongPath, MAX_PATH);

	mov	esi, esp
	push	260					; 00000104H
	lea	ecx, DWORD PTR _szLongPath$76368[ebp]
	push	ecx
	mov	edx, DWORD PTR _szShortPath$[ebp]
	push	edx
	call	DWORD PTR _pFN$76364[ebp]
	cmp	esi, esp
	call	__chkesp

; 2045 : 
; 2046 : 			sLongPath = szLongPath;

	lea	eax, DWORD PTR _szLongPath$76368[ebp]
	push	eax
	lea	ecx, DWORD PTR _sLongPath$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L76367:

; 2049 : 
; 2050 : 	return sLongPath;

	lea	ecx, DWORD PTR _sLongPath$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T77772[ebp]
	or	edx, 1
	mov	DWORD PTR $T77772[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sLongPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 2051 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 548				; 00000224H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77770:
	lea	ecx, DWORD PTR _sLongPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77771:
	mov	eax, DWORD PTR $T77772[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77773
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77773:
	ret	0
$L77776:
	mov	eax, OFFSET FLAT:$T77775
	jmp	___CxxFrameHandler
text$x	ENDS
?GetLongPathNameW@FileMisc@@YA?AVCString@@PBG@Z ENDP	; FileMisc::GetLongPathNameW
PUBLIC	?GetDropFilePaths@FileMisc@@YAHPAVCOleDataObject@@AAVCStringArray@@@Z ; FileMisc::GetDropFilePaths
EXTRN	__imp__GlobalLock@4:NEAR
EXTRN	__imp__GlobalUnlock@4:NEAR
EXTRN	?GetGlobalData@COleDataObject@@QAEPAXGPAUtagFORMATETC@@@Z:NEAR ; COleDataObject::GetGlobalData
EXTRN	?IsDataAvailable@COleDataObject@@QAEHGPAUtagFORMATETC@@@Z:NEAR ; COleDataObject::IsDataAvailable
EXTRN	__imp__RegisterClipboardFormatW@4:NEAR
_BSS	SEGMENT
	ALIGN	4

_?CF_FILENAME@?7??GetDropFilePaths@FileMisc@@YAHPAVCOleDataObject@@AAVCStringArray@@@Z@4GA DW 01H DUP (?)
	ALIGN	4

_?$S280@?7??GetDropFilePaths@FileMisc@@YAHPAVCOleDataObject@@AAVCStringArray@@@Z@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\FILEMISC.CPP
data	SEGMENT
$SG76385 DB	'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'N', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 'W', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77785	DD	019930520H
	DD	01H
	DD	FLAT:$T77787
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77787	DD	0ffffffffH
	DD	FLAT:$L77783
xdata$x	ENDS
;	COMDAT ?GetDropFilePaths@FileMisc@@YAHPAVCOleDataObject@@AAVCStringArray@@@Z
_TEXT	SEGMENT
_pObject$ = 8
_aFiles$ = 12
_nRet$ = -16
_hGlobal$76375 = -20
_hDrop$76377 = -24
_hGlobal$76387 = -28
_hDrop$76389 = -32
_sFilename$76391 = -36
__$EHRec$ = -12
?GetDropFilePaths@FileMisc@@YAHPAVCOleDataObject@@AAVCStringArray@@@Z PROC NEAR ; FileMisc::GetDropFilePaths, COMDAT

; 2054 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77786
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 2055 : 	int nRet = 0;

	mov	DWORD PTR _nRet$[ebp], 0

; 2056 : 	aFiles.RemoveAll();

	mov	ecx, DWORD PTR _aFiles$[ebp]
	call	?RemoveAll@CStringArray@@QAEXXZ		; CStringArray::RemoveAll

; 2057 : 
; 2058 : 	// first try
; 2059 : 	if (pObject->IsDataAvailable(CF_HDROP)) 

	push	0
	push	15					; 0000000fH
	mov	ecx, DWORD PTR _pObject$[ebp]
	call	?IsDataAvailable@COleDataObject@@QAEHGPAUtagFORMATETC@@@Z ; COleDataObject::IsDataAvailable
	test	eax, eax
	je	SHORT $L76376

; 2061 : 		HGLOBAL hGlobal = pObject->GetGlobalData(CF_HDROP);

	push	0
	push	15					; 0000000fH
	mov	ecx, DWORD PTR _pObject$[ebp]
	call	?GetGlobalData@COleDataObject@@QAEPAXGPAUtagFORMATETC@@@Z ; COleDataObject::GetGlobalData
	mov	DWORD PTR _hGlobal$76375[ebp], eax

; 2062 : 
; 2063 : 		if (hGlobal)

	cmp	DWORD PTR _hGlobal$76375[ebp], 0
	je	SHORT $L76376

; 2065 : 			HDROP hDrop = (HDROP)GlobalLock(hGlobal);

	mov	esi, esp
	mov	eax, DWORD PTR _hGlobal$76375[ebp]
	push	eax
	call	DWORD PTR __imp__GlobalLock@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hDrop$76377[ebp], eax

; 2066 : 			nRet = FileMisc::GetDropFilePaths(hDrop, aFiles);

	mov	ecx, DWORD PTR _aFiles$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hDrop$76377[ebp]
	push	edx
	call	?GetDropFilePaths@FileMisc@@YAHPAUHDROP__@@AAVCStringArray@@@Z ; FileMisc::GetDropFilePaths
	add	esp, 8
	mov	DWORD PTR _nRet$[ebp], eax

; 2067 : 
; 2068 : 			GlobalUnlock(hGlobal);

	mov	esi, esp
	mov	eax, DWORD PTR _hGlobal$76375[ebp]
	push	eax
	call	DWORD PTR __imp__GlobalUnlock@4
	cmp	esi, esp
	call	__chkesp
$L76376:

; 2071 : 
; 2072 : 	// second try
; 2073 : #ifdef _UNICODE
; 2074 : #	define CFSTR_FILENAME2 _T("FileNameW")
; 2075 : #else
; 2076 : #	define CFSTR_FILENAME2 _T("FileName")
; 2077 : #endif
; 2078 : 
; 2079 : 	static CLIPFORMAT CF_FILENAME = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_FILENAME2);

	xor	ecx, ecx
	mov	cl, BYTE PTR _?$S280@?7??GetDropFilePaths@FileMisc@@YAHPAVCOleDataObject@@AAVCStringArray@@@Z@4EA
	and	ecx, 1
	test	ecx, ecx
	jne	SHORT $L76384
	mov	dl, BYTE PTR _?$S280@?7??GetDropFilePaths@FileMisc@@YAHPAVCOleDataObject@@AAVCStringArray@@@Z@4EA
	or	dl, 1
	mov	BYTE PTR _?$S280@?7??GetDropFilePaths@FileMisc@@YAHPAVCOleDataObject@@AAVCStringArray@@@Z@4EA, dl
	mov	esi, esp
	push	OFFSET FLAT:$SG76385
	call	DWORD PTR __imp__RegisterClipboardFormatW@4
	cmp	esi, esp
	call	__chkesp
	mov	WORD PTR _?CF_FILENAME@?7??GetDropFilePaths@FileMisc@@YAHPAVCOleDataObject@@AAVCStringArray@@@Z@4GA, ax
$L76384:

; 2080 : 
; 2081 : 	if (nRet == 0 && pObject->IsDataAvailable(CF_FILENAME))

	cmp	DWORD PTR _nRet$[ebp], 0
	jne	$L76386
	push	0
	mov	ax, WORD PTR _?CF_FILENAME@?7??GetDropFilePaths@FileMisc@@YAHPAVCOleDataObject@@AAVCStringArray@@@Z@4GA
	push	eax
	mov	ecx, DWORD PTR _pObject$[ebp]
	call	?IsDataAvailable@COleDataObject@@QAEHGPAUtagFORMATETC@@@Z ; COleDataObject::IsDataAvailable
	test	eax, eax
	je	$L76386

; 2083 : 		HGLOBAL hGlobal = pObject->GetGlobalData(CF_FILENAME);

	push	0
	mov	cx, WORD PTR _?CF_FILENAME@?7??GetDropFilePaths@FileMisc@@YAHPAVCOleDataObject@@AAVCStringArray@@@Z@4GA
	push	ecx
	mov	ecx, DWORD PTR _pObject$[ebp]
	call	?GetGlobalData@COleDataObject@@QAEPAXGPAUtagFORMATETC@@@Z ; COleDataObject::GetGlobalData
	mov	DWORD PTR _hGlobal$76387[ebp], eax

; 2084 : 
; 2085 : 		if (hGlobal)

	cmp	DWORD PTR _hGlobal$76387[ebp], 0
	je	SHORT $L76388

; 2087 : 			HDROP hDrop = (HDROP)GlobalLock(hGlobal);

	mov	esi, esp
	mov	edx, DWORD PTR _hGlobal$76387[ebp]
	push	edx
	call	DWORD PTR __imp__GlobalLock@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hDrop$76389[ebp], eax

; 2088 : 			CString sFilename = GetLongPathName((LPCTSTR)hDrop); // copy

	mov	eax, DWORD PTR _hDrop$76389[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFilename$76391[ebp]
	push	ecx
	call	?GetLongPathNameW@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::GetLongPathNameW
	add	esp, 8
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 2089 : 			aFiles.Add(sFilename);

	lea	edx, DWORD PTR _sFilename$76391[ebp]
	push	edx
	mov	ecx, DWORD PTR _aFiles$[ebp]
	call	?Add@CStringArray@@QAEHABVCString@@@Z	; CStringArray::Add

; 2090 : 
; 2091 : 			GlobalUnlock(hGlobal);

	mov	esi, esp
	mov	eax, DWORD PTR _hGlobal$76387[ebp]
	push	eax
	call	DWORD PTR __imp__GlobalUnlock@4
	cmp	esi, esp
	call	__chkesp

; 2092 : 		}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sFilename$76391[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76388:

; 2093 : 
; 2094 : 		nRet = aFiles.GetSize();

	mov	ecx, DWORD PTR _aFiles$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nRet$[ebp], eax
$L76386:

; 2096 : 
; 2097 : 	return nRet;

	mov	eax, DWORD PTR _nRet$[ebp]

; 2098 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77783:
	lea	ecx, DWORD PTR _sFilename$76391[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77786:
	mov	eax, OFFSET FLAT:$T77785
	jmp	___CxxFrameHandler
text$x	ENDS
?GetDropFilePaths@FileMisc@@YAHPAVCOleDataObject@@AAVCStringArray@@@Z ENDP ; FileMisc::GetDropFilePaths
END

	TITLE	D:\_CODE\Shared\Misc.cpp
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
;	COMDAT ??_C@_08PEKM@lpa?5?$CB?$DN?50?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0EK@BMNG@C?3?2Program?5Files?5?$CIx86?$CJ?2Microsoft@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_08NDGO@lpw?5?$CB?$DN?50?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0BA@DNCN@lpDevModeW?5?$CB?$DN?50?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0L@BLIP@lptmW?5?$CB?$DN?50?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0L@DMEN@lptmA?5?$CB?$DN?50?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0BA@BKOP@lpDevModeA?5?$CB?$DN?50?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0EK@MDCF@C?3?2Program?5Files?5?$CIx86?$CJ?2Microsoft@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
text$AFX_COL1	SEGMENT PARA USE32 PUBLIC ''
text$AFX_COL1	ENDS
;	COMDAT ??_G?$CTypedPtrList@VCObList@@PAV1@@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$_CTypedPtrList@VCObList@@PAV1@@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CTypedPtrList@VCObList@@PAV1@@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CTypedPtrList@VCPtrList@@PAV1@@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$_CTypedPtrList@VCPtrList@@PAV1@@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CTypedPtrList@VCPtrList@@PAV1@@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AtlA2WHelper@@YGPAGPAGPBDHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AtlW2AHelper@@YGPADPADPBGHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?A2WBSTR@@YAPAGPBDH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FormatGetLastError@Misc@@YA?AVCString@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CopyTexttoClipboard@Misc@@YAHABVCString@@PAUHWND__@@IH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetClipboardText@Misc@@YA?AVCString@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ClipboardHasText@Misc@@YAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ClipboardHasFormat@Misc@@YAHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FilterString@Misc@@YAHAAVCString@@ABV2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewGuid@Misc@@YA?AVCString@@PAU_GUID@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsGuid@Misc@@YAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GuidFromString@Misc@@YAHPBGAAU_GUID@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GuidToString@Misc@@YAHABU_GUID@@AAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GuidIsNull@Misc@@YAHABU_GUID@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SameGuids@Misc@@YAHABU_GUID@@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NullGuid@Misc@@YAXAAU_GUID@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessMsgLoop@Misc@@YAXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FormatComputerNameAndUser@Misc@@YA?AVCString@@D@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E273
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetComputerNameW@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E275
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetUserNameW@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E277
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetListSeparator@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E279
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FormatArray@Misc@@YA?AVCString@@ABVCStringArray@@G@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FormatArray@Misc@@YA?AVCString@@ABVCStringArray@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FormatArray@Misc@@YA?AVCString@@ABVCDWordArray@@G@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FormatArrayAsNumberedList@Misc@@YA?AVCString@@ABVCStringArray@@PBGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FormatArray@Misc@@YA?AVCString@@ABVCDWordArray@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Trace@Misc@@YAXABVCStringArray@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTotalLength@Misc@@YAHABVCStringArray@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Split@Misc@@YAHAAVCString@@0GH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Split@Misc@@YAHABVCString@@AAVCStringArray@@GH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Split@Misc@@YAHABVCString@@AAVCStringArray@@PBGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MatchAll@Misc@@YAHABVCStringArray@@0HH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MatchAny@Misc@@YAHABVCStringArray@@0HH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Find@Misc@@YAHABVCStringArray@@PBGHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E281
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveEmptyItems@Misc@@YAHAAVCStringArray@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveItems@Misc@@YAHABVCStringArray@@AAV2@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveItem@Misc@@YAHPBGAAVCStringArray@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddUniqueItems@Misc@@YAHABVCStringArray@@AAV2@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddUniqueItem@Misc@@YAHABVCString@@AAVCStringArray@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAM@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E283
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetPM@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E285
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTimeFormatW@Misc@@YA?AVCString@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E287
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTimeSeparator@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E289
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetDateSeparator@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E291
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetShortDateFormat@Misc@@YA?AVCString@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E293
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetDecimalSeparator@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E295
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsMetricMeasurementSystem@Misc@@YAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EncodeAsMultiByte@Misc@@YAAAVCString@@AAV2@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EncodeAsUnicode@Misc@@YAAAVCString@@AAV2@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?WideToMultiByte@Misc@@YAPADPBGI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?WideToMultiByte@Misc@@YAPADPBGAAHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MultiByteToWide@Misc@@YAPAGPBDI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MultiByteToWide@Misc@@YAPAGPBDAAHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Round@Misc@@YANN@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Round@Misc@@YAMM@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsNumber@Misc@@YAHABVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E297
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsSymbol@Misc@@YAHABVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetLongest@Misc@@YAABVCString@@ABV2@0H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Atof@Misc@@YANABVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsWorkStationLocked@Misc@@YAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsScreenSaverActive@Misc@@YAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsScreenReaderActive@Misc@@YAHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsMSNarratorActive@Misc@@YAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetUserDefaultUILanguage@Misc@@YAGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetDefCharset@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LCMapStringW@Misc@@YAHAAVCString@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MakeUpper@Misc@@YAXAAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MakeLower@Misc@@YAXAAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ToUpper@Misc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ToLower@Misc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FindWord@Misc@@YAHPBG0HH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ParseSearchString@Misc@@YAHPBGAAVCStringArray@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Format@Misc@@YA?AVCString@@NHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Format@Misc@@YA?AVCString@@HPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Format@Misc@@YA?AVCString@@KPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FormatCost@Misc@@YA?AVCString@@N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetKeyName@Misc@@YA?AVCString@@GH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?KeyIsPressed@Misc@@YAHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsCursorKeyPressed@Misc@@YAHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsCursorKey@Misc@@YAHKK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ModKeysArePressed@Misc@@YAHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ModifyFlags@Misc@@YAHKKK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MakeKey@Misc@@YA?AVCString@@ABV2@HPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MakeKey@Misc@@YA?AVCString@@ABV2@PBG1@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NaturalCompare@Misc@@YAHPBG0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?StringSortProc@@YAHPBX0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SortArray@Misc@@YAXAAVCStringArray@@P6AHPBX1@Z@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DWordSortProc@@YAHPBX0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SortArray@Misc@@YAXAAVCDWordArray@@P6AHPBX1@Z@Z
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
;	COMDAT ??0?$_CTypedPtrList@VCObList@@PAV1@@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$_CTypedPtrList@VCPtrList@@PAV1@@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$_CTypedPtrList@VCObList@@PAV1@@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$_CTypedPtrList@VCPtrList@@PAV1@@@UAEPAXI@Z
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
;	COMDAT ??_7?$_CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
_BSS	SEGMENT
_?NULLGUID@?1??GuidIsNull@Misc@@YAHABU_GUID@@@Z@4U3@A DB 010H DUP (?)
_BSS	ENDS
_DATA	SEGMENT
__szAfxTempl DB	'afxtempl.h', 00H
	ORG $+1
_THIS_FILE DB	'D:\_CODE\Shared\Misc.cpp', 00H
_DATA	ENDS
PUBLIC	?FormatGetLastError@Misc@@YA?AVCString@@K@Z	; Misc::FormatGetLastError
EXTRN	__imp__LocalFree@4:NEAR
EXTRN	__imp__GetLastError@0:NEAR
EXTRN	__chkesp:NEAR
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	__imp__FormatMessageW@28:NEAR
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT xdata$x
; File D:\_CODE\Shared\Misc.cpp
xdata$x	SEGMENT
$T76344	DD	019930520H
	DD	02H
	DD	FLAT:$T76348
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76348	DD	0ffffffffH
	DD	FLAT:$L76338
	DD	00H
	DD	FLAT:$L76337
xdata$x	ENDS
;	COMDAT ?FormatGetLastError@Misc@@YA?AVCString@@K@Z
_TEXT	SEGMENT
_dwLastErr$ = 12
___$ReturnUdt$ = 8
_lpMessage$ = -20
_dwErrCode$ = -16
_sError$ = -24
$T76339 = -28
__$EHRec$ = -12
?FormatGetLastError@Misc@@YA?AVCString@@K@Z PROC NEAR	; Misc::FormatGetLastError, COMDAT

; 37   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76346
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR $T76339[ebp], 0

; 38   : 	if (dwLastErr == -1)

	cmp	DWORD PTR _dwLastErr$[ebp], -1
	jne	SHORT $L74896

; 39   : 		dwLastErr = GetLastError();

	mov	esi, esp
	call	DWORD PTR __imp__GetLastError@0
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _dwLastErr$[ebp], eax
$L74896:

; 42   : 	DWORD dwErrCode = GetLastError();

	mov	esi, esp
	call	DWORD PTR __imp__GetLastError@0
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _dwErrCode$[ebp], eax

; 43   : 	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
; 44   : 				   FORMAT_MESSAGE_FROM_SYSTEM,
; 45   : 				   NULL,               // no source buffer needed
; 46   : 				   dwErrCode,          // error code for this message
; 47   : 				   NULL,               // default language ID
; 48   : 				   (LPTSTR)&lpMessage, // allocated by fcn
; 49   : 				   NULL,               // minimum size of buffer
; 50   : 				   NULL);              // no inserts

	mov	esi, esp
	push	0
	push	0
	lea	eax, DWORD PTR _lpMessage$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _dwErrCode$[ebp]
	push	ecx
	push	0
	push	4352					; 00001100H
	call	DWORD PTR __imp__FormatMessageW@28
	cmp	esi, esp
	call	__chkesp

; 51   : 
; 52   : 
; 53   : 	CString sError(lpMessage);

	mov	edx, DWORD PTR _lpMessage$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sError$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 54   : 	LocalFree(lpMessage);

	mov	esi, esp
	mov	eax, DWORD PTR _lpMessage$[ebp]
	push	eax
	call	DWORD PTR __imp__LocalFree@4
	cmp	esi, esp
	call	__chkesp

; 55   : 
; 56   : 	return sError;

	lea	ecx, DWORD PTR _sError$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76339[ebp]
	or	edx, 1
	mov	DWORD PTR $T76339[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sError$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 57   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76337:
	lea	ecx, DWORD PTR _sError$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76338:
	mov	eax, DWORD PTR $T76339[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76340
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76340:
	ret	0
$L76346:
	mov	eax, OFFSET FLAT:$T76344
	jmp	___CxxFrameHandler
text$x	ENDS
?FormatGetLastError@Misc@@YA?AVCString@@K@Z ENDP	; Misc::FormatGetLastError
PUBLIC	?CopyTexttoClipboard@Misc@@YAHABVCString@@PAUHWND__@@IH@Z ; Misc::CopyTexttoClipboard
EXTRN	__imp__GlobalAlloc@8:NEAR
EXTRN	__imp__GlobalLock@4:NEAR
EXTRN	__imp__GlobalUnlock@4:NEAR
EXTRN	__imp__GlobalFree@4:NEAR
EXTRN	_memcpy:NEAR
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	__imp__OpenClipboard@4:NEAR
EXTRN	__imp__CloseClipboard@0:NEAR
EXTRN	__imp__SetClipboardData@8:NEAR
EXTRN	__imp__EmptyClipboard@0:NEAR
;	COMDAT ?CopyTexttoClipboard@Misc@@YAHABVCString@@PAUHWND__@@IH@Z
_TEXT	SEGMENT
_sText$ = 8
_hwnd$ = 12
_nFormat$ = 16
_bClear$ = 20
_hglbCopy$ = -8
_bResult$ = -4
_lptstrCopy$74917 = -12
?CopyTexttoClipboard@Misc@@YAHABVCString@@PAUHWND__@@IH@Z PROC NEAR ; Misc::CopyTexttoClipboard, COMDAT

; 60   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L74907:

; 61   : 	ASSERT(hwnd);

	cmp	DWORD PTR _hwnd$[ebp], 0
	jne	SHORT $L74910
	push	61					; 0000003dH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74910
	int	3
$L74910:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74907

; 62   : 
; 63   : 	if (!::OpenClipboard(hwnd)) 

	mov	esi, esp
	mov	ecx, DWORD PTR _hwnd$[ebp]
	push	ecx
	call	DWORD PTR __imp__OpenClipboard@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L74911

; 64   : 		return FALSE; 

	xor	eax, eax
	jmp	$L74906
$L74911:

; 65   : 	
; 66   : 	HGLOBAL hglbCopy = NULL; 

	mov	DWORD PTR _hglbCopy$[ebp], 0

; 67   : 	BOOL bResult = FALSE;

	mov	DWORD PTR _bResult$[ebp], 0

; 68   : 
; 69   : 	if (bClear)

	cmp	DWORD PTR _bClear$[ebp], 0
	je	SHORT $L74914

; 70   : 		::EmptyClipboard(); 

	mov	esi, esp
	call	DWORD PTR __imp__EmptyClipboard@0
	cmp	esi, esp
	call	__chkesp
$L74914:

; 71   : 	
; 72   : 	// Allocate a global memory object for the text. 
; 73   : 	hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, (sText.GetLength() + 1) * sizeof(TCHAR)); 

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	lea	edx, DWORD PTR [eax+eax+2]
	mov	esi, esp
	push	edx
	push	2
	call	DWORD PTR __imp__GlobalAlloc@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hglbCopy$[ebp], eax

; 74   : 	
; 75   : 	if (hglbCopy) 

	cmp	DWORD PTR _hglbCopy$[ebp], 0
	je	$L74916

; 77   : 		// Lock the handle and copy the text to the buffer. 
; 78   : 		LPTSTR lptstrCopy = (LPTSTR)::GlobalLock(hglbCopy); 

	mov	esi, esp
	mov	eax, DWORD PTR _hglbCopy$[ebp]
	push	eax
	call	DWORD PTR __imp__GlobalLock@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lptstrCopy$74917[ebp], eax

; 79   : 		memcpy(lptstrCopy, (LPVOID)(LPCTSTR)sText, sText.GetLength() * sizeof(TCHAR)); 

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	shl	eax, 1
	push	eax
	mov	ecx, DWORD PTR _sText$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _lptstrCopy$74917[ebp]
	push	ecx
	call	_memcpy
	add	esp, 12					; 0000000cH

; 80   : 		lptstrCopy[sText.GetLength()] = (TCHAR) 0;    // null character 

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	edx, DWORD PTR _lptstrCopy$74917[ebp]
	mov	WORD PTR [edx+eax*2], 0

; 81   : 		
; 82   : 		::GlobalUnlock(hglbCopy); 

	mov	esi, esp
	mov	eax, DWORD PTR _hglbCopy$[ebp]
	push	eax
	call	DWORD PTR __imp__GlobalUnlock@4
	cmp	esi, esp
	call	__chkesp

; 83   : 		
; 84   : 		// Place the handle on the clipboard. 
; 85   : 		if (nFormat == 0)

	cmp	DWORD PTR _nFormat$[ebp], 0
	jne	SHORT $L74923

; 87   : #ifndef _UNICODE
; 88   : 			nFormat = CF_TEXT;
; 89   : #else
; 90   : 			nFormat = CF_UNICODETEXT;

	mov	DWORD PTR _nFormat$[ebp], 13		; 0000000dH
$L74923:

; 93   : 		
; 94   : 		if (::SetClipboardData(nFormat, hglbCopy) == NULL)

	mov	esi, esp
	mov	ecx, DWORD PTR _hglbCopy$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nFormat$[ebp]
	push	edx
	call	DWORD PTR __imp__SetClipboardData@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L74924

; 95   : 			::GlobalFree(hglbCopy);			

	mov	esi, esp
	mov	eax, DWORD PTR _hglbCopy$[ebp]
	push	eax
	call	DWORD PTR __imp__GlobalFree@4
	cmp	esi, esp
	call	__chkesp
$L74924:

; 96   : 		
; 97   : 		hglbCopy = NULL;

	mov	DWORD PTR _hglbCopy$[ebp], 0

; 98   : 		bResult = TRUE;

	mov	DWORD PTR _bResult$[ebp], 1
$L74916:

; 100  : 	
; 101  : 	::CloseClipboard(); // always

	mov	esi, esp
	call	DWORD PTR __imp__CloseClipboard@0
	cmp	esi, esp
	call	__chkesp

; 102  : 
; 103  : 	return bResult;

	mov	eax, DWORD PTR _bResult$[ebp]
$L74906:

; 104  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?CopyTexttoClipboard@Misc@@YAHABVCString@@PAUHWND__@@IH@Z ENDP ; Misc::CopyTexttoClipboard
_TEXT	ENDS
PUBLIC	?GetClipboardText@Misc@@YA?AVCString@@I@Z	; Misc::GetClipboardText
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	??0CString@@QAE@PBD@Z:NEAR			; CString::CString
EXTRN	??4CString@@QAEABV0@PBG@Z:NEAR			; CString::operator=
EXTRN	__imp__GetClipboardData@4:NEAR
_BSS	SEGMENT
$SG74930 DB	01H DUP (?)
_BSS	ENDS
;	COMDAT xdata$x
; File D:\_CODE\Shared\Misc.cpp
xdata$x	SEGMENT
$T76361	DD	019930520H
	DD	02H
	DD	FLAT:$T76363
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76363	DD	0ffffffffH
	DD	FLAT:$L76356
	DD	00H
	DD	FLAT:$L76359
xdata$x	ENDS
;	COMDAT ?GetClipboardText@Misc@@YA?AVCString@@I@Z
_TEXT	SEGMENT
_nFormat$ = 12
___$ReturnUdt$ = 8
_sText$ = -20
_hData$ = -16
$T76357 = -24
__$EHRec$ = -12
?GetClipboardText@Misc@@YA?AVCString@@I@Z PROC NEAR	; Misc::GetClipboardText, COMDAT

; 107  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76362
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T76357[ebp], 0

; 108  : 	if (!::OpenClipboard(NULL)) 

	mov	esi, esp
	push	0
	call	DWORD PTR __imp__OpenClipboard@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L74929

; 109  : 		return ""; 

	push	OFFSET FLAT:$SG74930
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	eax, DWORD PTR $T76357[ebp]
	or	al, 1
	mov	DWORD PTR $T76357[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L74928
$L74929:

; 110  : 
; 111  : 	CString sText;

	lea	ecx, DWORD PTR _sText$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 112  : 
; 113  : 	if (nFormat == 0)

	cmp	DWORD PTR _nFormat$[ebp], 0
	jne	SHORT $L74932

; 115  : #ifndef _UNICODE
; 116  : 		nFormat = CF_TEXT;
; 117  : #else
; 118  : 		nFormat = CF_UNICODETEXT;

	mov	DWORD PTR _nFormat$[ebp], 13		; 0000000dH
$L74932:

; 121  : 	
; 122  : 	HANDLE hData = ::GetClipboardData(nFormat);

	mov	esi, esp
	mov	ecx, DWORD PTR _nFormat$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetClipboardData@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hData$[ebp], eax

; 123  : 	sText = (LPCTSTR)GlobalLock(hData);

	mov	esi, esp
	mov	edx, DWORD PTR _hData$[ebp]
	push	edx
	call	DWORD PTR __imp__GlobalLock@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 124  : 
; 125  : 	::GlobalUnlock(hData);

	mov	esi, esp
	mov	eax, DWORD PTR _hData$[ebp]
	push	eax
	call	DWORD PTR __imp__GlobalUnlock@4
	cmp	esi, esp
	call	__chkesp

; 126  : 	::CloseClipboard(); // always

	mov	esi, esp
	call	DWORD PTR __imp__CloseClipboard@0
	cmp	esi, esp
	call	__chkesp

; 127  : 
; 128  : 	return sText;

	lea	ecx, DWORD PTR _sText$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76357[ebp]
	or	edx, 1
	mov	DWORD PTR $T76357[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L74928:

; 129  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76356:
	mov	eax, DWORD PTR $T76357[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76358
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76358:
	ret	0
$L76359:
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76362:
	mov	eax, OFFSET FLAT:$T76361
	jmp	___CxxFrameHandler
text$x	ENDS
?GetClipboardText@Misc@@YA?AVCString@@I@Z ENDP		; Misc::GetClipboardText
PUBLIC	?ClipboardHasFormat@Misc@@YAHI@Z		; Misc::ClipboardHasFormat
PUBLIC	?ClipboardHasText@Misc@@YAHXZ			; Misc::ClipboardHasText
;	COMDAT ?ClipboardHasText@Misc@@YAHXZ
_TEXT	SEGMENT
_nFormat$ = -4
?ClipboardHasText@Misc@@YAHXZ PROC NEAR			; Misc::ClipboardHasText, COMDAT

; 132  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 133  : #ifndef _UNICODE
; 134  : 	int nFormat = CF_TEXT;
; 135  : #else
; 136  : 	int	nFormat = CF_UNICODETEXT;

	mov	DWORD PTR _nFormat$[ebp], 13		; 0000000dH

; 137  : #endif
; 138  : 
; 139  : 	return ClipboardHasFormat(nFormat);

	mov	eax, DWORD PTR _nFormat$[ebp]
	push	eax
	call	?ClipboardHasFormat@Misc@@YAHI@Z	; Misc::ClipboardHasFormat
	add	esp, 4

; 140  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?ClipboardHasText@Misc@@YAHXZ ENDP			; Misc::ClipboardHasText
_TEXT	ENDS
EXTRN	__imp__EnumClipboardFormats@4:NEAR
;	COMDAT ?ClipboardHasFormat@Misc@@YAHI@Z
_TEXT	SEGMENT
_nFormat$ = 8
_nFmt$ = -4
?ClipboardHasFormat@Misc@@YAHI@Z PROC NEAR		; Misc::ClipboardHasFormat, COMDAT

; 143  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 144  : 	if (!::OpenClipboard(NULL))

	mov	esi, esp
	push	0
	call	DWORD PTR __imp__OpenClipboard@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L74941

; 145  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L74940
$L74941:

; 146  : 
; 147  : 	UINT nFmt = nFmt = ::EnumClipboardFormats(0);

	mov	esi, esp
	push	0
	call	DWORD PTR __imp__EnumClipboardFormats@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nFmt$[ebp], eax
	mov	eax, DWORD PTR _nFmt$[ebp]
	mov	DWORD PTR _nFmt$[ebp], eax
$L74944:

; 148  : 
; 149  : 	while (nFmt) 

	cmp	DWORD PTR _nFmt$[ebp], 0
	je	SHORT $L74945

; 151  : 		if (nFmt == nFormat)

	mov	ecx, DWORD PTR _nFmt$[ebp]
	cmp	ecx, DWORD PTR _nFormat$[ebp]
	jne	SHORT $L74946

; 152  : 			break;

	jmp	SHORT $L74945
$L74946:

; 153  : 		
; 154  : 		nFmt = ::EnumClipboardFormats(nFmt);

	mov	esi, esp
	mov	edx, DWORD PTR _nFmt$[ebp]
	push	edx
	call	DWORD PTR __imp__EnumClipboardFormats@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nFmt$[ebp], eax

; 155  : 	}

	jmp	SHORT $L74944
$L74945:

; 156  : 
; 157  : 	::CloseClipboard(); // always

	mov	esi, esp
	call	DWORD PTR __imp__CloseClipboard@0
	cmp	esi, esp
	call	__chkesp

; 158  : 
; 159  : 	return (nFormat && nFmt == nFormat);

	cmp	DWORD PTR _nFormat$[ebp], 0
	je	SHORT $L76370
	mov	eax, DWORD PTR _nFmt$[ebp]
	cmp	eax, DWORD PTR _nFormat$[ebp]
	jne	SHORT $L76370
	mov	DWORD PTR -8+[ebp], 1
	jmp	SHORT $L76371
$L76370:
	mov	DWORD PTR -8+[ebp], 0
$L76371:
	mov	eax, DWORD PTR -8+[ebp]
$L74940:

; 160  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?ClipboardHasFormat@Misc@@YAHI@Z ENDP			; Misc::ClipboardHasFormat
_TEXT	ENDS
PUBLIC	?FilterString@Misc@@YAHAAVCString@@ABV2@@Z	; Misc::FilterString
EXTRN	??ACString@@QBEGH@Z:NEAR			; CString::operator[]
EXTRN	?Delete@CString@@QAEHHH@Z:NEAR			; CString::Delete
EXTRN	?Find@CString@@QBEHG@Z:NEAR			; CString::Find
;	COMDAT ?FilterString@Misc@@YAHAAVCString@@ABV2@@Z
_TEXT	SEGMENT
_sText$ = 8
_sFilter$ = 12
_nPos$ = -4
?FilterString@Misc@@YAHAAVCString@@ABV2@@Z PROC NEAR	; Misc::FilterString, COMDAT

; 163  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 164  : 	int nPos = sText.GetLength();

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR _nPos$[ebp], eax
$L74953:

; 165  : 
; 166  : 	while (nPos--)

	mov	eax, DWORD PTR _nPos$[ebp]
	mov	ecx, DWORD PTR _nPos$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nPos$[ebp], ecx
	test	eax, eax
	je	SHORT $L74954

; 168  : 		if (sFilter.Find(sText[nPos]) == -1)

	mov	edx, DWORD PTR _nPos$[ebp]
	push	edx
	mov	ecx, DWORD PTR _sText$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	push	eax
	mov	ecx, DWORD PTR _sFilter$[ebp]
	call	?Find@CString@@QBEHG@Z			; CString::Find
	cmp	eax, -1
	jne	SHORT $L74955

; 169  : 			sText.Delete(nPos);

	push	1
	mov	eax, DWORD PTR _nPos$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sText$[ebp]
	call	?Delete@CString@@QAEHHH@Z		; CString::Delete
$L74955:

; 170  : 	}

	jmp	SHORT $L74953
$L74954:

; 171  : 
; 172  : 	return sText.GetLength();

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength

; 173  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?FilterString@Misc@@YAHAAVCString@@ABV2@@Z ENDP		; Misc::FilterString
_TEXT	ENDS
PUBLIC	?GuidToString@Misc@@YAHABU_GUID@@AAVCString@@@Z	; Misc::GuidToString
PUBLIC	?NewGuid@Misc@@YA?AVCString@@PAU_GUID@@@Z	; Misc::NewGuid
EXTRN	__imp__UuidCreate@4:NEAR
_BSS	SEGMENT
	ALIGN	4

$SG74966 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT xdata$x
; File D:\_CODE\Shared\Misc.cpp
xdata$x	SEGMENT
$T76381	DD	019930520H
	DD	01H
	DD	FLAT:$T76383
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76383	DD	0ffffffffH
	DD	FLAT:$L76376
xdata$x	ENDS
;	COMDAT ?NewGuid@Misc@@YA?AVCString@@PAU_GUID@@@Z
_TEXT	SEGMENT
_pGuid$ = 12
___$ReturnUdt$ = 8
_guid$ = -28
_sGuid$74963 = -32
$T76378 = -36
__$EHRec$ = -12
?NewGuid@Misc@@YA?AVCString@@PAU_GUID@@@Z PROC NEAR	; Misc::NewGuid, COMDAT

; 176  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76382
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
	mov	DWORD PTR $T76378[ebp], 0

; 177  : 	GUID guid = { 0 };

	mov	DWORD PTR _guid$[ebp], 0
	xor	eax, eax
	mov	DWORD PTR _guid$[ebp+4], eax
	mov	DWORD PTR _guid$[ebp+8], eax
	mov	DWORD PTR _guid$[ebp+12], eax

; 178  : 
; 179  : 	if (RPC_S_OK == UuidCreate((GUID*)&guid))

	mov	esi, esp
	lea	ecx, DWORD PTR _guid$[ebp]
	push	ecx
	call	DWORD PTR __imp__UuidCreate@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L74962

; 181  : 		CString sGuid;

	lea	ecx, DWORD PTR _sGuid$74963[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 182  : 
; 183  : 		if (GuidToString(guid, sGuid))

	lea	edx, DWORD PTR _sGuid$74963[ebp]
	push	edx
	lea	eax, DWORD PTR _guid$[ebp]
	push	eax
	call	?GuidToString@Misc@@YAHABU_GUID@@AAVCString@@@Z ; Misc::GuidToString
	add	esp, 8
	test	eax, eax
	je	SHORT $L74964

; 185  : 			if (pGuid)

	cmp	DWORD PTR _pGuid$[ebp], 0
	je	SHORT $L74965

; 186  : 				*pGuid = guid;

	mov	ecx, DWORD PTR _pGuid$[ebp]
	mov	edx, DWORD PTR _guid$[ebp]
	mov	DWORD PTR [ecx], edx
	mov	eax, DWORD PTR _guid$[ebp+4]
	mov	DWORD PTR [ecx+4], eax
	mov	edx, DWORD PTR _guid$[ebp+8]
	mov	DWORD PTR [ecx+8], edx
	mov	eax, DWORD PTR _guid$[ebp+12]
	mov	DWORD PTR [ecx+12], eax
$L74965:

; 187  : 
; 188  : 			return sGuid;

	lea	ecx, DWORD PTR _sGuid$74963[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76378[ebp]
	or	edx, 1
	mov	DWORD PTR $T76378[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sGuid$74963[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L74959
$L74964:

; 190  : 	}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sGuid$74963[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74962:

; 191  : 
; 192  : 	// else
; 193  : 	return _T("");

	push	OFFSET FLAT:$SG74966
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	eax, DWORD PTR $T76378[ebp]
	or	al, 1
	mov	DWORD PTR $T76378[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L74959:

; 194  : }

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
$L76376:
	lea	ecx, DWORD PTR _sGuid$74963[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76382:
	mov	eax, OFFSET FLAT:$T76381
	jmp	___CxxFrameHandler
text$x	ENDS
?NewGuid@Misc@@YA?AVCString@@PAU_GUID@@@Z ENDP		; Misc::NewGuid
PUBLIC	?GuidFromString@Misc@@YAHPBGAAU_GUID@@@Z	; Misc::GuidFromString
PUBLIC	?IsGuid@Misc@@YAHPBG@Z				; Misc::IsGuid
;	COMDAT ?IsGuid@Misc@@YAHPBG@Z
_TEXT	SEGMENT
_szGuid$ = 8
_guid$ = -16
?IsGuid@Misc@@YAHPBG@Z PROC NEAR			; Misc::IsGuid, COMDAT

; 197  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 198  : 	GUID guid;
; 199  : 
; 200  : 	return GuidFromString(szGuid, guid);

	lea	eax, DWORD PTR _guid$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szGuid$[ebp]
	push	ecx
	call	?GuidFromString@Misc@@YAHPBGAAU_GUID@@@Z ; Misc::GuidFromString
	add	esp, 8

; 201  : }

	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsGuid@Misc@@YAHPBG@Z ENDP				; Misc::IsGuid
_TEXT	ENDS
PUBLIC	?NullGuid@Misc@@YAXAAU_GUID@@@Z			; Misc::NullGuid
EXTRN	__imp__UuidFromStringW@8:NEAR
;	COMDAT ?GuidFromString@Misc@@YAHPBGAAU_GUID@@@Z
_TEXT	SEGMENT
_szGuid$ = 8
_guid$ = 12
_rpcs$ = -4
?GuidFromString@Misc@@YAHPBGAAU_GUID@@@Z PROC NEAR	; Misc::GuidFromString, COMDAT

; 204  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 205  : #pragma comment(lib, "Rpcrt4.lib")
; 206  : 
; 207  : 	RPC_STATUS rpcs = UuidFromString((RPC_TSTR)szGuid, &guid);

	mov	esi, esp
	mov	eax, DWORD PTR _guid$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szGuid$[ebp]
	push	ecx
	call	DWORD PTR __imp__UuidFromStringW@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _rpcs$[ebp], eax

; 208  : 
; 209  : 	if (rpcs != RPC_S_OK)

	cmp	DWORD PTR _rpcs$[ebp], 0
	je	SHORT $L74977

; 211  : 		NullGuid(guid);

	mov	edx, DWORD PTR _guid$[ebp]
	push	edx
	call	?NullGuid@Misc@@YAXAAU_GUID@@@Z		; Misc::NullGuid
	add	esp, 4

; 212  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L74974
$L74977:

; 214  : 
; 215  : 	return TRUE;

	mov	eax, 1
$L74974:

; 216  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GuidFromString@Misc@@YAHPBGAAU_GUID@@@Z ENDP		; Misc::GuidFromString
_TEXT	ENDS
EXTRN	__imp__RpcStringFreeW@4:NEAR
EXTRN	__imp__UuidToStringW@8:NEAR
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
EXTRN	?Empty@CString@@QAEXXZ:NEAR			; CString::Empty
;	COMDAT ?GuidToString@Misc@@YAHABU_GUID@@AAVCString@@@Z
_TEXT	SEGMENT
_guid$ = 8
_sGuid$ = 12
_pszGuid$ = -4
?GuidToString@Misc@@YAHABU_GUID@@AAVCString@@@Z PROC NEAR ; Misc::GuidToString, COMDAT

; 219  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 220  : #pragma comment(lib, "Rpcrt4.lib")
; 221  : 
; 222  : 	RPC_TSTR pszGuid = NULL;

	mov	DWORD PTR _pszGuid$[ebp], 0

; 223  : 	
; 224  : 	if (RPC_S_OK == UuidToString((GUID*)&guid, &pszGuid))

	mov	esi, esp
	lea	eax, DWORD PTR _pszGuid$[ebp]
	push	eax
	mov	ecx, DWORD PTR _guid$[ebp]
	push	ecx
	call	DWORD PTR __imp__UuidToStringW@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L74984

; 225  : 		sGuid = (LPCTSTR)pszGuid;

	mov	edx, DWORD PTR _pszGuid$[ebp]
	push	edx
	mov	ecx, DWORD PTR _sGuid$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 226  : 	else

	jmp	SHORT $L74986
$L74984:

; 227  : 		sGuid.Empty();

	mov	ecx, DWORD PTR _sGuid$[ebp]
	call	?Empty@CString@@QAEXXZ			; CString::Empty
$L74986:

; 228  : 	
; 229  : 	RpcStringFree(&pszGuid);

	mov	esi, esp
	lea	eax, DWORD PTR _pszGuid$[ebp]
	push	eax
	call	DWORD PTR __imp__RpcStringFreeW@4
	cmp	esi, esp
	call	__chkesp

; 230  : 	
; 231  : 	return !sGuid.IsEmpty();

	mov	ecx, DWORD PTR _sGuid$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	neg	eax
	sbb	eax, eax
	inc	eax

; 232  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GuidToString@Misc@@YAHABU_GUID@@AAVCString@@@Z ENDP	; Misc::GuidToString
_TEXT	ENDS
PUBLIC	?GuidIsNull@Misc@@YAHABU_GUID@@@Z		; Misc::GuidIsNull
PUBLIC	?SameGuids@Misc@@YAHABU_GUID@@0@Z		; Misc::SameGuids
;	COMDAT ?GuidIsNull@Misc@@YAHABU_GUID@@@Z
_TEXT	SEGMENT
_guid$ = 8
?GuidIsNull@Misc@@YAHABU_GUID@@@Z PROC NEAR		; Misc::GuidIsNull, COMDAT

; 235  : {

	push	ebp
	mov	ebp, esp

; 236  : 	static GUID NULLGUID = { 0 };
; 237  : 	
; 238  : 	return SameGuids(guid, NULLGUID);

	push	OFFSET FLAT:_?NULLGUID@?1??GuidIsNull@Misc@@YAHABU_GUID@@@Z@4U3@A
	mov	eax, DWORD PTR _guid$[ebp]
	push	eax
	call	?SameGuids@Misc@@YAHABU_GUID@@0@Z	; Misc::SameGuids
	add	esp, 8

; 239  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?GuidIsNull@Misc@@YAHABU_GUID@@@Z ENDP			; Misc::GuidIsNull
_TEXT	ENDS
EXTRN	_memcmp:NEAR
;	COMDAT ?SameGuids@Misc@@YAHABU_GUID@@0@Z
_TEXT	SEGMENT
_guid1$ = 8
_guid2$ = 12
?SameGuids@Misc@@YAHABU_GUID@@0@Z PROC NEAR		; Misc::SameGuids, COMDAT

; 242  : {

	push	ebp
	mov	ebp, esp

; 243  : 	return (memcmp(&guid1, &guid2, sizeof(GUID)) == 0);

	push	16					; 00000010H
	mov	eax, DWORD PTR _guid2$[ebp]
	push	eax
	mov	ecx, DWORD PTR _guid1$[ebp]
	push	ecx
	call	_memcmp
	add	esp, 12					; 0000000cH
	neg	eax
	sbb	eax, eax
	inc	eax

; 244  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?SameGuids@Misc@@YAHABU_GUID@@0@Z ENDP			; Misc::SameGuids
_TEXT	ENDS
EXTRN	_memset:NEAR
;	COMDAT ?NullGuid@Misc@@YAXAAU_GUID@@@Z
_TEXT	SEGMENT
_guid$ = 8
?NullGuid@Misc@@YAXAAU_GUID@@@Z PROC NEAR		; Misc::NullGuid, COMDAT

; 247  : {

	push	ebp
	mov	ebp, esp

; 248  :    ZeroMemory(&guid, sizeof(guid));

	push	16					; 00000010H
	push	0
	mov	eax, DWORD PTR _guid$[ebp]
	push	eax
	call	_memset
	add	esp, 12					; 0000000cH

; 249  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?NullGuid@Misc@@YAXAAU_GUID@@@Z ENDP			; Misc::NullGuid
_TEXT	ENDS
PUBLIC	?ProcessMsgLoop@Misc@@YAXXZ			; Misc::ProcessMsgLoop
EXTRN	__imp__IsDialogMessageW@8:NEAR
EXTRN	__imp__TranslateMessage@4:NEAR
EXTRN	__imp__DispatchMessageW@4:NEAR
EXTRN	__imp__PeekMessageW@20:NEAR
;	COMDAT ?ProcessMsgLoop@Misc@@YAXXZ
_TEXT	SEGMENT
_msg$ = -28
?ProcessMsgLoop@Misc@@YAXXZ PROC NEAR			; Misc::ProcessMsgLoop, COMDAT

; 252  : {

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
$L75004:

; 253  : 	MSG msg;
; 254  : 
; 255  : 	while (::PeekMessage((LPMSG) &msg, NULL, 0, 0, PM_REMOVE))

	mov	esi, esp
	push	1
	push	0
	push	0
	push	0
	lea	eax, DWORD PTR _msg$[ebp]
	push	eax
	call	DWORD PTR __imp__PeekMessageW@20
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L75005

; 257  : 		if (::IsDialogMessage(msg.hwnd, (LPMSG)&msg))

	mov	esi, esp
	lea	ecx, DWORD PTR _msg$[ebp]
	push	ecx
	mov	edx, DWORD PTR _msg$[ebp]
	push	edx
	call	DWORD PTR __imp__IsDialogMessageW@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L75008

; 261  : 		else

	jmp	SHORT $L75009
$L75008:

; 263  : 			::TranslateMessage(&msg);

	mov	esi, esp
	lea	eax, DWORD PTR _msg$[ebp]
	push	eax
	call	DWORD PTR __imp__TranslateMessage@4
	cmp	esi, esp
	call	__chkesp

; 264  : 			::DispatchMessage(&msg);

	mov	esi, esp
	lea	ecx, DWORD PTR _msg$[ebp]
	push	ecx
	call	DWORD PTR __imp__DispatchMessageW@4
	cmp	esi, esp
	call	__chkesp
$L75009:

; 266  : 	}

	jmp	SHORT $L75004
$L75005:

; 267  : }

	pop	esi
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?ProcessMsgLoop@Misc@@YAXXZ ENDP			; Misc::ProcessMsgLoop
_TEXT	ENDS
PUBLIC	?FormatComputerNameAndUser@Misc@@YA?AVCString@@D@Z ; Misc::FormatComputerNameAndUser
PUBLIC	?GetComputerNameW@Misc@@YA?AVCString@@XZ	; Misc::GetComputerNameW
PUBLIC	?GetUserNameW@Misc@@YA?AVCString@@XZ		; Misc::GetUserNameW
EXTRN	_atexit:NEAR
EXTRN	?Format@CString@@QAAXPBGZZ:NEAR			; CString::Format
_BSS	SEGMENT
	ALIGN	4

_?sMachineAndUser@?1??FormatComputerNameAndUser@Misc@@YA?AVCString@@D@Z@4V3@A DD 01H DUP (?)
_?$S272@?1??FormatComputerNameAndUser@Misc@@YA?AVCString@@D@Z@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75023 DB	'%', 00H, 's', 00H, '%', 00H, 'c', 00H, '%', 00H, 's', 00H
	DB	00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76410	DD	019930520H
	DD	02H
	DD	FLAT:$T76412
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76412	DD	0ffffffffH
	DD	FLAT:$L76404
	DD	00H
	DD	FLAT:$L76405
xdata$x	ENDS
;	COMDAT ?FormatComputerNameAndUser@Misc@@YA?AVCString@@D@Z
_TEXT	SEGMENT
_cSeparator$ = 12
___$ReturnUdt$ = 8
$T76402 = -16
$T76403 = -20
$T76407 = -24
__$EHRec$ = -12
?FormatComputerNameAndUser@Misc@@YA?AVCString@@D@Z PROC NEAR ; Misc::FormatComputerNameAndUser, COMDAT

; 270  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76411
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
	mov	DWORD PTR $T76407[ebp], 0

; 271  : 	static CString sMachineAndUser;

	xor	eax, eax
	mov	al, BYTE PTR _?$S272@?1??FormatComputerNameAndUser@Misc@@YA?AVCString@@D@Z@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75018
	mov	cl, BYTE PTR _?$S272@?1??FormatComputerNameAndUser@Misc@@YA?AVCString@@D@Z@4EA
	or	cl, 1
	mov	BYTE PTR _?$S272@?1??FormatComputerNameAndUser@Misc@@YA?AVCString@@D@Z@4EA, cl
	mov	ecx, OFFSET FLAT:_?sMachineAndUser@?1??FormatComputerNameAndUser@Misc@@YA?AVCString@@D@Z@4V3@A
	call	??0CString@@QAE@XZ			; CString::CString
	push	OFFSET FLAT:_$E273
	call	_atexit
	add	esp, 4
$L75018:

; 272  : 	
; 273  : 	if (sMachineAndUser.IsEmpty())

	mov	ecx, OFFSET FLAT:_?sMachineAndUser@?1??FormatComputerNameAndUser@Misc@@YA?AVCString@@D@Z@4V3@A
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75020

; 274  : 		sMachineAndUser.Format(_T("%s%c%s"), GetComputerName(), cSeparator, GetUserName());

	lea	edx, DWORD PTR $T76402[ebp]
	push	edx
	call	?GetUserNameW@Misc@@YA?AVCString@@XZ	; Misc::GetUserNameW
	add	esp, 4
	mov	DWORD PTR -28+[ebp], eax
	mov	eax, DWORD PTR -28+[ebp]
	mov	DWORD PTR -32+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -32+[ebp]
	mov	edx, DWORD PTR [ecx]
	push	edx
	movsx	eax, BYTE PTR _cSeparator$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T76403[ebp]
	push	ecx
	call	?GetComputerNameW@Misc@@YA?AVCString@@XZ ; Misc::GetComputerNameW
	add	esp, 4
	mov	DWORD PTR -36+[ebp], eax
	mov	edx, DWORD PTR -36+[ebp]
	mov	DWORD PTR -40+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	eax, DWORD PTR -40+[ebp]
	mov	ecx, DWORD PTR [eax]
	push	ecx
	push	OFFSET FLAT:$SG75023
	push	OFFSET FLAT:_?sMachineAndUser@?1??FormatComputerNameAndUser@Misc@@YA?AVCString@@D@Z@4V3@A
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 20					; 00000014H
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T76403[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76402[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75020:

; 275  : 
; 276  : 	return sMachineAndUser;

	push	OFFSET FLAT:_?sMachineAndUser@?1??FormatComputerNameAndUser@Misc@@YA?AVCString@@D@Z@4V3@A
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76407[ebp]
	or	edx, 1
	mov	DWORD PTR $T76407[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 277  : }

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
$L76404:
	lea	ecx, DWORD PTR $T76402[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76405:
	lea	ecx, DWORD PTR $T76403[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76411:
	mov	eax, OFFSET FLAT:$T76410
	jmp	___CxxFrameHandler
text$x	ENDS
?FormatComputerNameAndUser@Misc@@YA?AVCString@@D@Z ENDP	; Misc::FormatComputerNameAndUser
;	COMDAT _$E273
_TEXT	SEGMENT
_$E273	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?sMachineAndUser@?1??FormatComputerNameAndUser@Misc@@YA?AVCString@@D@Z@4V3@A
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E273	ENDP
_TEXT	ENDS
EXTRN	__imp__GetComputerNameW@8:NEAR
EXTRN	?GetBuffer@CString@@QAEPAGH@Z:NEAR		; CString::GetBuffer
EXTRN	?ReleaseBuffer@CString@@QAEXH@Z:NEAR		; CString::ReleaseBuffer
_BSS	SEGMENT
	ALIGN	4

_?sMachine@?1??GetComputerNameW@Misc@@YA?AVCString@@XZ@4V3@A DD 01H DUP (?)
_?$S274@?1??GetComputerNameW@Misc@@YA?AVCString@@XZ@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT ?GetComputerNameW@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_LEN$75035 = -4
$T76421 = -8
?GetComputerNameW@Misc@@YA?AVCString@@XZ PROC NEAR	; Misc::GetComputerNameW, COMDAT

; 280  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T76421[ebp], 0

; 281  : 	static CString sMachine;

	xor	eax, eax
	mov	al, BYTE PTR _?$S274@?1??GetComputerNameW@Misc@@YA?AVCString@@XZ@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75032
	mov	cl, BYTE PTR _?$S274@?1??GetComputerNameW@Misc@@YA?AVCString@@XZ@4EA
	or	cl, 1
	mov	BYTE PTR _?$S274@?1??GetComputerNameW@Misc@@YA?AVCString@@XZ@4EA, cl
	mov	ecx, OFFSET FLAT:_?sMachine@?1??GetComputerNameW@Misc@@YA?AVCString@@XZ@4V3@A
	call	??0CString@@QAE@XZ			; CString::CString
	push	OFFSET FLAT:_$E275
	call	_atexit
	add	esp, 4
$L75032:

; 282  : 
; 283  : 	if (sMachine.IsEmpty()) // init first time only

	mov	ecx, OFFSET FLAT:_?sMachine@?1??GetComputerNameW@Misc@@YA?AVCString@@XZ@4V3@A
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75034

; 285  : 		DWORD LEN = MAX_COMPUTERNAME_LENGTH + 1;

	mov	DWORD PTR _LEN$75035[ebp], 16		; 00000010H

; 286  : 		
; 287  : 		::GetComputerName(sMachine.GetBuffer(LEN), &LEN);

	mov	esi, esp
	lea	edx, DWORD PTR _LEN$75035[ebp]
	push	edx
	mov	eax, DWORD PTR _LEN$75035[ebp]
	push	eax
	mov	ecx, OFFSET FLAT:_?sMachine@?1??GetComputerNameW@Misc@@YA?AVCString@@XZ@4V3@A
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	call	DWORD PTR __imp__GetComputerNameW@8
	cmp	esi, esp
	call	__chkesp

; 288  : 		sMachine.ReleaseBuffer();

	push	-1
	mov	ecx, OFFSET FLAT:_?sMachine@?1??GetComputerNameW@Misc@@YA?AVCString@@XZ@4V3@A
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer
$L75034:

; 290  : 
; 291  : 	return sMachine;

	push	OFFSET FLAT:_?sMachine@?1??GetComputerNameW@Misc@@YA?AVCString@@XZ@4V3@A
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T76421[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76421[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 292  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetComputerNameW@Misc@@YA?AVCString@@XZ ENDP		; Misc::GetComputerNameW
_TEXT	ENDS
;	COMDAT _$E275
_TEXT	SEGMENT
_$E275	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?sMachine@?1??GetComputerNameW@Misc@@YA?AVCString@@XZ@4V3@A
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E275	ENDP
_TEXT	ENDS
EXTRN	__imp__GetUserNameW@8:NEAR
_BSS	SEGMENT
	ALIGN	4

_?sUser@?1??GetUserNameW@Misc@@YA?AVCString@@XZ@4V3@A DD 01H DUP (?)
_?$S276@?1??GetUserNameW@Misc@@YA?AVCString@@XZ@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT ?GetUserNameW@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_LEN$75047 = -4
$T76428 = -8
?GetUserNameW@Misc@@YA?AVCString@@XZ PROC NEAR		; Misc::GetUserNameW, COMDAT

; 295  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T76428[ebp], 0

; 296  : 	static CString sUser;

	xor	eax, eax
	mov	al, BYTE PTR _?$S276@?1??GetUserNameW@Misc@@YA?AVCString@@XZ@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75044
	mov	cl, BYTE PTR _?$S276@?1??GetUserNameW@Misc@@YA?AVCString@@XZ@4EA
	or	cl, 1
	mov	BYTE PTR _?$S276@?1??GetUserNameW@Misc@@YA?AVCString@@XZ@4EA, cl
	mov	ecx, OFFSET FLAT:_?sUser@?1??GetUserNameW@Misc@@YA?AVCString@@XZ@4V3@A
	call	??0CString@@QAE@XZ			; CString::CString
	push	OFFSET FLAT:_$E277
	call	_atexit
	add	esp, 4
$L75044:

; 297  : 
; 298  : 	if (sUser.IsEmpty()) // init first time only

	mov	ecx, OFFSET FLAT:_?sUser@?1??GetUserNameW@Misc@@YA?AVCString@@XZ@4V3@A
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75046

; 300  : 		DWORD LEN = UNLEN + 1;

	mov	DWORD PTR _LEN$75047[ebp], 257		; 00000101H

; 301  : 		
; 302  : 		::GetUserName(sUser.GetBuffer(LEN), &LEN);

	mov	esi, esp
	lea	edx, DWORD PTR _LEN$75047[ebp]
	push	edx
	mov	eax, DWORD PTR _LEN$75047[ebp]
	push	eax
	mov	ecx, OFFSET FLAT:_?sUser@?1??GetUserNameW@Misc@@YA?AVCString@@XZ@4V3@A
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	call	DWORD PTR __imp__GetUserNameW@8
	cmp	esi, esp
	call	__chkesp

; 303  : 		sUser.ReleaseBuffer();

	push	-1
	mov	ecx, OFFSET FLAT:_?sUser@?1??GetUserNameW@Misc@@YA?AVCString@@XZ@4V3@A
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer
$L75046:

; 305  : 
; 306  : 	return sUser;

	push	OFFSET FLAT:_?sUser@?1??GetUserNameW@Misc@@YA?AVCString@@XZ@4V3@A
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T76428[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76428[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 307  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetUserNameW@Misc@@YA?AVCString@@XZ ENDP		; Misc::GetUserNameW
_TEXT	ENDS
;	COMDAT _$E277
_TEXT	SEGMENT
_$E277	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?sUser@?1??GetUserNameW@Misc@@YA?AVCString@@XZ@4V3@A
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E277	ENDP
_TEXT	ENDS
PUBLIC	?GetListSeparator@Misc@@YA?AVCString@@XZ	; Misc::GetListSeparator
PUBLIC	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z		; Misc::Trim
EXTRN	__imp__GetLocaleInfoW@16:NEAR
EXTRN	??4CString@@QAEABV0@D@Z:NEAR			; CString::operator=
_BSS	SEGMENT
	ALIGN	4

_?sSep@?1??GetListSeparator@Misc@@YA?AVCString@@XZ@4V3@A DD 01H DUP (?)
_?$S278@?1??GetListSeparator@Misc@@YA?AVCString@@XZ@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT ?GetListSeparator@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_BUFLEN$ = -4
$T76435 = -8
?GetListSeparator@Misc@@YA?AVCString@@XZ PROC NEAR	; Misc::GetListSeparator, COMDAT

; 310  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T76435[ebp], 0

; 311  : 	static CString sSep;

	xor	eax, eax
	mov	al, BYTE PTR _?$S278@?1??GetListSeparator@Misc@@YA?AVCString@@XZ@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75056
	mov	cl, BYTE PTR _?$S278@?1??GetListSeparator@Misc@@YA?AVCString@@XZ@4EA
	or	cl, 1
	mov	BYTE PTR _?$S278@?1??GetListSeparator@Misc@@YA?AVCString@@XZ@4EA, cl
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetListSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	??0CString@@QAE@XZ			; CString::CString
	push	OFFSET FLAT:_$E279
	call	_atexit
	add	esp, 4
$L75056:

; 312  : 	const int BUFLEN = 10;

	mov	DWORD PTR _BUFLEN$[ebp], 10		; 0000000aH

; 313  : 	
; 314  : 	if (sSep.IsEmpty()) // init first time only

	mov	ecx, OFFSET FLAT:_?sSep@?1??GetListSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75067

; 316  : 		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLIST, sSep.GetBuffer(BUFLEN), BUFLEN - 1);

	mov	esi, esp
	push	9
	push	10					; 0000000aH
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetListSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	push	12					; 0000000cH
	push	1024					; 00000400H
	call	DWORD PTR __imp__GetLocaleInfoW@16
	cmp	esi, esp
	call	__chkesp

; 317  : 		sSep.ReleaseBuffer();

	push	-1
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetListSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 318  : 		
; 319  : 		// Trim extra spaces
; 320  : 		Trim(sSep);

	push	0
	push	OFFSET FLAT:_?sSep@?1??GetListSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z	; Misc::Trim
	add	esp, 8

; 321  : 		
; 322  : 		// If none found, use a comma
; 323  : 		if (!sSep.GetLength())

	mov	ecx, OFFSET FLAT:_?sSep@?1??GetListSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	jne	SHORT $L75067

; 324  : 			sSep = ',';

	push	44					; 0000002cH
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetListSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	??4CString@@QAEABV0@D@Z			; CString::operator=
$L75067:

; 326  : 
; 327  : 	return sSep;

	push	OFFSET FLAT:_?sSep@?1??GetListSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76435[ebp]
	or	edx, 1
	mov	DWORD PTR $T76435[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 328  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetListSeparator@Misc@@YA?AVCString@@XZ ENDP		; Misc::GetListSeparator
_TEXT	ENDS
;	COMDAT _$E279
_TEXT	SEGMENT
_$E279	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetListSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E279	ENDP
_TEXT	ENDS
PUBLIC	?FormatArray@Misc@@YA?AVCString@@ABVCStringArray@@PBG@Z ; Misc::FormatArray
PUBLIC	?FormatArray@Misc@@YA?AVCString@@ABVCStringArray@@G@Z ; Misc::FormatArray
;	COMDAT ?FormatArray@Misc@@YA?AVCString@@ABVCStringArray@@G@Z
_TEXT	SEGMENT
_array$ = 12
_cSep$ = 16
___$ReturnUdt$ = 8
_szSep$ = -4
$T76442 = -8
?FormatArray@Misc@@YA?AVCString@@ABVCStringArray@@G@Z PROC NEAR ; Misc::FormatArray, COMDAT

; 331  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T76442[ebp], 0

; 332  : 	TCHAR szSep[2] = { cSep, 0 };

	mov	ax, WORD PTR _cSep$[ebp]
	mov	WORD PTR _szSep$[ebp], ax
	mov	WORD PTR _szSep$[ebp+2], 0

; 333  : 
; 334  : 	return FormatArray(array, szSep);

	lea	ecx, DWORD PTR _szSep$[ebp]
	push	ecx
	mov	edx, DWORD PTR _array$[ebp]
	push	edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	call	?FormatArray@Misc@@YA?AVCString@@ABVCStringArray@@PBG@Z ; Misc::FormatArray
	add	esp, 12					; 0000000cH
	mov	ecx, DWORD PTR $T76442[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76442[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 335  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?FormatArray@Misc@@YA?AVCString@@ABVCStringArray@@G@Z ENDP ; Misc::FormatArray
_TEXT	ENDS
PUBLIC	?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z ; Misc::GetItem
EXTRN	?GetSize@CStringArray@@QBEHXZ:NEAR		; CStringArray::GetSize
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
EXTRN	??YCString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator+=
EXTRN	??H@YG?AVCString@@ABV0@D@Z:NEAR			; operator+
_BSS	SEGMENT
	ALIGN	4

$SG75083 DB	01H DUP (?)
_BSS	ENDS
;	COMDAT xdata$x
; File D:\_CODE\Shared\Misc.cpp
xdata$x	SEGMENT
$T76456	DD	019930520H
	DD	05H
	DD	FLAT:$T76458
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76458	DD	0ffffffffH
	DD	FLAT:$L76448
	DD	00H
	DD	FLAT:$L76451
	DD	01H
	DD	FLAT:$L76452
	DD	02H
	DD	FLAT:$L76453
	DD	01H
	DD	FLAT:$L76454
xdata$x	ENDS
;	COMDAT ?FormatArray@Misc@@YA?AVCString@@ABVCStringArray@@PBG@Z
_TEXT	SEGMENT
_array$ = 12
_szSep$ = 16
___$ReturnUdt$ = 8
_nCount$ = -24
_sSep$ = -28
_sText$ = -16
_nItem$ = -20
_sItem$75093 = -32
$T76446 = -36
$T76447 = -40
$T76449 = -44
__$EHRec$ = -12
?FormatArray@Misc@@YA?AVCString@@ABVCStringArray@@PBG@Z PROC NEAR ; Misc::FormatArray, COMDAT

; 338  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76457
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 48					; 00000030H
	push	edi
	lea	edi, DWORD PTR [ebp-60]
	mov	ecx, 12					; 0000000cH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T76449[ebp], 0

; 339  : 	int nCount = array.GetSize();

	mov	ecx, DWORD PTR _array$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nCount$[ebp], eax

; 340  : 
; 341  : 	if (nCount == 0)

	cmp	DWORD PTR _nCount$[ebp], 0
	jne	SHORT $L75082

; 342  : 		return "";

	push	OFFSET FLAT:$SG75083
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	eax, DWORD PTR $T76449[ebp]
	or	al, 1
	mov	DWORD PTR $T76449[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75080
$L75082:

; 343  : 
; 344  : 	CString sSep(szSep);

	mov	ecx, DWORD PTR _szSep$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 345  : 
; 346  : 	if (!szSep)

	cmp	DWORD PTR _szSep$[ebp], 0
	jne	SHORT $L75085

; 347  : 		sSep = GetListSeparator() + ' ';

	push	32					; 00000020H
	lea	edx, DWORD PTR $T76446[ebp]
	push	edx
	call	?GetListSeparator@Misc@@YA?AVCString@@XZ ; Misc::GetListSeparator
	add	esp, 4
	mov	DWORD PTR -48+[ebp], eax
	mov	eax, DWORD PTR -48+[ebp]
	mov	DWORD PTR -52+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR -52+[ebp]
	push	ecx
	lea	edx, DWORD PTR $T76447[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@D@Z		; operator+
	mov	DWORD PTR -56+[ebp], eax
	mov	eax, DWORD PTR -56+[ebp]
	mov	DWORD PTR -60+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR -60+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T76447[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T76446[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75085:

; 348  : 
; 349  : 	CString sText;

	lea	ecx, DWORD PTR _sText$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 4

; 350  : 
; 351  : 	for (int nItem = 0; nItem < nCount; nItem++)

	mov	DWORD PTR _nItem$[ebp], 0
	jmp	SHORT $L75090
$L75091:
	mov	edx, DWORD PTR _nItem$[ebp]
	add	edx, 1
	mov	DWORD PTR _nItem$[ebp], edx
$L75090:
	mov	eax, DWORD PTR _nItem$[ebp]
	cmp	eax, DWORD PTR _nCount$[ebp]
	jge	SHORT $L75092

; 353  : 		const CString& sItem = GetItem(array, nItem);

	mov	ecx, DWORD PTR _nItem$[ebp]
	push	ecx
	mov	edx, DWORD PTR _array$[ebp]
	push	edx
	call	?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z ; Misc::GetItem
	add	esp, 8
	mov	DWORD PTR _sItem$75093[ebp], eax

; 354  : 
; 355  : 		if (nItem > 0 && !sItem.IsEmpty())

	cmp	DWORD PTR _nItem$[ebp], 0
	jle	SHORT $L75094
	mov	ecx, DWORD PTR _sItem$75093[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75094

; 356  : 			sText += sSep;

	lea	eax, DWORD PTR _sSep$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
$L75094:

; 357  : 
; 358  : 		sText += sItem;

	mov	ecx, DWORD PTR _sItem$75093[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=

; 359  : 	}

	jmp	SHORT $L75091
$L75092:

; 360  : 
; 361  : 	return sText;

	lea	edx, DWORD PTR _sText$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T76449[ebp]
	or	al, 1
	mov	DWORD PTR $T76449[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L75080:

; 362  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 60					; 0000003cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76448:
	mov	eax, DWORD PTR $T76449[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76450
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76450:
	ret	0
$L76451:
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76452:
	lea	ecx, DWORD PTR $T76446[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76453:
	lea	ecx, DWORD PTR $T76447[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76454:
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76457:
	mov	eax, OFFSET FLAT:$T76456
	jmp	___CxxFrameHandler
text$x	ENDS
?FormatArray@Misc@@YA?AVCString@@ABVCStringArray@@PBG@Z ENDP ; Misc::FormatArray
PUBLIC	?FormatArray@Misc@@YA?AVCString@@ABVCDWordArray@@PBG@Z ; Misc::FormatArray
PUBLIC	?FormatArray@Misc@@YA?AVCString@@ABVCDWordArray@@G@Z ; Misc::FormatArray
;	COMDAT ?FormatArray@Misc@@YA?AVCString@@ABVCDWordArray@@G@Z
_TEXT	SEGMENT
_array$ = 12
_cSep$ = 16
___$ReturnUdt$ = 8
_szSep$ = -4
$T76464 = -8
?FormatArray@Misc@@YA?AVCString@@ABVCDWordArray@@G@Z PROC NEAR ; Misc::FormatArray, COMDAT

; 365  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T76464[ebp], 0

; 366  : 	TCHAR szSep[2] = { cSep, 0 };

	mov	ax, WORD PTR _cSep$[ebp]
	mov	WORD PTR _szSep$[ebp], ax
	mov	WORD PTR _szSep$[ebp+2], 0

; 367  : 
; 368  : 	return FormatArray(array, szSep);

	lea	ecx, DWORD PTR _szSep$[ebp]
	push	ecx
	mov	edx, DWORD PTR _array$[ebp]
	push	edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	call	?FormatArray@Misc@@YA?AVCString@@ABVCDWordArray@@PBG@Z ; Misc::FormatArray
	add	esp, 12					; 0000000cH
	mov	ecx, DWORD PTR $T76464[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76464[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 369  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?FormatArray@Misc@@YA?AVCString@@ABVCDWordArray@@G@Z ENDP ; Misc::FormatArray
_TEXT	ENDS
PUBLIC	?FormatArrayAsNumberedList@Misc@@YA?AVCString@@ABVCStringArray@@PBGH@Z ; Misc::FormatArrayAsNumberedList
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75119 DB	'%', 00H, 'd', 00H, '%', 00H, 's', 00H, '%', 00H, 's', 00H
	DB	0aH, 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76474	DD	019930520H
	DD	03H
	DD	FLAT:$T76476
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76476	DD	0ffffffffH
	DD	FLAT:$L76470
	DD	00H
	DD	FLAT:$L76468
	DD	01H
	DD	FLAT:$L76469
xdata$x	ENDS
;	COMDAT ?FormatArrayAsNumberedList@Misc@@YA?AVCString@@ABVCStringArray@@PBGH@Z
_TEXT	SEGMENT
_array$ = 12
_szDelim$ = 16
_nStart$ = 20
___$ReturnUdt$ = 8
_nCount$ = -28
_sList$ = -20
_sItem$ = -16
_nItem$ = -24
$T76471 = -32
__$EHRec$ = -12
?FormatArrayAsNumberedList@Misc@@YA?AVCString@@ABVCStringArray@@PBGH@Z PROC NEAR ; Misc::FormatArrayAsNumberedList, COMDAT

; 372  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76475
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 20					; 00000014H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR $T76471[ebp], 0
$L75108:

; 373  : 	ASSERT(szDelim);

	cmp	DWORD PTR _szDelim$[ebp], 0
	jne	SHORT $L75111
	push	373					; 00000175H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75111
	int	3
$L75111:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75108

; 374  : 
; 375  : 	int nCount = array.GetSize();

	mov	ecx, DWORD PTR _array$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nCount$[ebp], eax

; 376  : 	CString sList, sItem;

	lea	ecx, DWORD PTR _sList$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sItem$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 377  : 
; 378  : 	for (int nItem = 0; nItem < nCount; nItem++)

	mov	DWORD PTR _nItem$[ebp], 0
	jmp	SHORT $L75116
$L75117:
	mov	ecx, DWORD PTR _nItem$[ebp]
	add	ecx, 1
	mov	DWORD PTR _nItem$[ebp], ecx
$L75116:
	mov	edx, DWORD PTR _nItem$[ebp]
	cmp	edx, DWORD PTR _nCount$[ebp]
	jge	SHORT $L75118

; 380  : 		sItem.Format(_T("%d%s%s\n"), (nItem + nStart), szDelim, GetItem(array, nItem));

	mov	eax, DWORD PTR _nItem$[ebp]
	push	eax
	mov	ecx, DWORD PTR _array$[ebp]
	push	ecx
	call	?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z ; Misc::GetItem
	add	esp, 8
	mov	edx, DWORD PTR [eax]
	push	edx
	mov	eax, DWORD PTR _szDelim$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nItem$[ebp]
	add	ecx, DWORD PTR _nStart$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG75119
	lea	edx, DWORD PTR _sItem$[ebp]
	push	edx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 20					; 00000014H

; 381  : 
; 382  : 		sList += sItem;

	lea	eax, DWORD PTR _sItem$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sList$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=

; 383  : 	}

	jmp	SHORT $L75117
$L75118:

; 384  : 
; 385  : 	return sList;

	lea	ecx, DWORD PTR _sList$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76471[ebp]
	or	edx, 1
	mov	DWORD PTR $T76471[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sItem$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sList$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 386  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76468:
	lea	ecx, DWORD PTR _sList$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76469:
	lea	ecx, DWORD PTR _sItem$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76470:
	mov	eax, DWORD PTR $T76471[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76472
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76472:
	ret	0
$L76475:
	mov	eax, OFFSET FLAT:$T76474
	jmp	___CxxFrameHandler
text$x	ENDS
?FormatArrayAsNumberedList@Misc@@YA?AVCString@@ABVCStringArray@@PBGH@Z ENDP ; Misc::FormatArrayAsNumberedList
EXTRN	?GetSize@CDWordArray@@QBEHXZ:NEAR		; CDWordArray::GetSize
EXTRN	??ACDWordArray@@QBEKH@Z:NEAR			; CDWordArray::operator[]
_BSS	SEGMENT
	ALIGN	4

$SG75127 DB	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75138 DB	'%', 00H, 'l', 00H, 'u', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76493	DD	019930520H
	DD	06H
	DD	FLAT:$T76495
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76495	DD	0ffffffffH
	DD	FLAT:$L76484
	DD	00H
	DD	FLAT:$L76487
	DD	01H
	DD	FLAT:$L76488
	DD	02H
	DD	FLAT:$L76489
	DD	01H
	DD	FLAT:$L76490
	DD	04H
	DD	FLAT:$L76491
xdata$x	ENDS
;	COMDAT ?FormatArray@Misc@@YA?AVCString@@ABVCDWordArray@@PBG@Z
_TEXT	SEGMENT
_array$ = 12
_szSep$ = 16
___$ReturnUdt$ = 8
_nCount$ = -24
_sSep$ = -28
_sText$ = -16
_nItem$ = -20
_sItem$75137 = -32
$T76482 = -36
$T76483 = -40
$T76485 = -44
__$EHRec$ = -12
?FormatArray@Misc@@YA?AVCString@@ABVCDWordArray@@PBG@Z PROC NEAR ; Misc::FormatArray, COMDAT

; 389  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76494
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 48					; 00000030H
	push	edi
	lea	edi, DWORD PTR [ebp-60]
	mov	ecx, 12					; 0000000cH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T76485[ebp], 0

; 390  : 	int nCount = array.GetSize();

	mov	ecx, DWORD PTR _array$[ebp]
	call	?GetSize@CDWordArray@@QBEHXZ		; CDWordArray::GetSize
	mov	DWORD PTR _nCount$[ebp], eax

; 391  : 
; 392  : 	if (nCount == 0)

	cmp	DWORD PTR _nCount$[ebp], 0
	jne	SHORT $L75126

; 393  : 		return "";

	push	OFFSET FLAT:$SG75127
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	eax, DWORD PTR $T76485[ebp]
	or	al, 1
	mov	DWORD PTR $T76485[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75124
$L75126:

; 394  : 
; 395  : 	CString sSep(szSep);

	mov	ecx, DWORD PTR _szSep$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 396  : 
; 397  : 	if (!szSep)

	cmp	DWORD PTR _szSep$[ebp], 0
	jne	SHORT $L75129

; 398  : 		sSep = GetListSeparator() + ' ';

	push	32					; 00000020H
	lea	edx, DWORD PTR $T76482[ebp]
	push	edx
	call	?GetListSeparator@Misc@@YA?AVCString@@XZ ; Misc::GetListSeparator
	add	esp, 4
	mov	DWORD PTR -48+[ebp], eax
	mov	eax, DWORD PTR -48+[ebp]
	mov	DWORD PTR -52+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR -52+[ebp]
	push	ecx
	lea	edx, DWORD PTR $T76483[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@D@Z		; operator+
	mov	DWORD PTR -56+[ebp], eax
	mov	eax, DWORD PTR -56+[ebp]
	mov	DWORD PTR -60+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR -60+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T76483[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T76482[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75129:

; 399  : 
; 400  : 	CString sText;

	lea	ecx, DWORD PTR _sText$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 4

; 401  : 
; 402  : 	for (int nItem = 0; nItem < nCount; nItem++)

	mov	DWORD PTR _nItem$[ebp], 0
	jmp	SHORT $L75134
$L75135:
	mov	edx, DWORD PTR _nItem$[ebp]
	add	edx, 1
	mov	DWORD PTR _nItem$[ebp], edx
$L75134:
	mov	eax, DWORD PTR _nItem$[ebp]
	cmp	eax, DWORD PTR _nCount$[ebp]
	jge	SHORT $L75136

; 404  : 		CString sItem;

	lea	ecx, DWORD PTR _sItem$75137[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 5

; 405  : 		sItem.Format(_T("%lu"), array[nItem]);

	mov	ecx, DWORD PTR _nItem$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _array$[ebp]
	call	??ACDWordArray@@QBEKH@Z			; CDWordArray::operator[]
	push	eax
	push	OFFSET FLAT:$SG75138
	lea	edx, DWORD PTR _sItem$75137[ebp]
	push	edx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 12					; 0000000cH

; 406  : 
; 407  : 		if (nItem > 0)

	cmp	DWORD PTR _nItem$[ebp], 0
	jle	SHORT $L75139

; 408  : 			sText += sSep;

	lea	eax, DWORD PTR _sSep$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
$L75139:

; 409  : 
; 410  : 		sText += sItem;

	lea	ecx, DWORD PTR _sItem$75137[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=

; 411  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR _sItem$75137[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	SHORT $L75135
$L75136:

; 412  : 
; 413  : 	return sText;

	lea	edx, DWORD PTR _sText$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T76485[ebp]
	or	al, 1
	mov	DWORD PTR $T76485[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L75124:

; 414  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 60					; 0000003cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76484:
	mov	eax, DWORD PTR $T76485[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76486
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76486:
	ret	0
$L76487:
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76488:
	lea	ecx, DWORD PTR $T76482[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76489:
	lea	ecx, DWORD PTR $T76483[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76490:
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76491:
	lea	ecx, DWORD PTR _sItem$75137[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76494:
	mov	eax, OFFSET FLAT:$T76493
	jmp	___CxxFrameHandler
text$x	ENDS
?FormatArray@Misc@@YA?AVCString@@ABVCDWordArray@@PBG@Z ENDP ; Misc::FormatArray
PUBLIC	?Trace@Misc@@YAXABVCStringArray@@@Z		; Misc::Trace
EXTRN	?AfxTrace@@YAXPBGZZ:NEAR			; AfxTrace
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75148 DB	'%', 00H, 's', 00H, ',', 00H, ' ', 00H, 00H, 00H
	ORG $+2
$SG75149 DB	0aH, 00H, 00H, 00H
data	ENDS
;	COMDAT ?Trace@Misc@@YAXABVCStringArray@@@Z
_TEXT	SEGMENT
_array$ = 8
_nCount$ = -8
_nItem$ = -4
?Trace@Misc@@YAXABVCStringArray@@@Z PROC NEAR		; Misc::Trace, COMDAT

; 417  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 418  : #ifdef _DEBUG
; 419  : 	int nCount = array.GetSize();

	mov	ecx, DWORD PTR _array$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nCount$[ebp], eax

; 420  : 
; 421  : 	for (int nItem = 0; nItem < nCount; nItem++)

	mov	DWORD PTR _nItem$[ebp], 0
	jmp	SHORT $L75145
$L75146:
	mov	eax, DWORD PTR _nItem$[ebp]
	add	eax, 1
	mov	DWORD PTR _nItem$[ebp], eax
$L75145:
	mov	ecx, DWORD PTR _nItem$[ebp]
	cmp	ecx, DWORD PTR _nCount$[ebp]
	jge	SHORT $L75147

; 422  : 		TRACE(_T("%s, "), GetItem(array, nItem));

	mov	edx, DWORD PTR _nItem$[ebp]
	push	edx
	mov	eax, DWORD PTR _array$[ebp]
	push	eax
	call	?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z ; Misc::GetItem
	add	esp, 8
	mov	ecx, DWORD PTR [eax]
	push	ecx
	push	OFFSET FLAT:$SG75148
	call	?AfxTrace@@YAXPBGZZ			; AfxTrace
	add	esp, 8
	jmp	SHORT $L75146
$L75147:

; 423  : 
; 424  : 	TRACE(_T("\n"));

	push	OFFSET FLAT:$SG75149
	call	?AfxTrace@@YAXPBGZZ			; AfxTrace
	add	esp, 4

; 425  : #else
; 426  : 	UNUSED(array);
; 427  : #endif
; 428  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Trace@Misc@@YAXABVCStringArray@@@Z ENDP		; Misc::Trace
_TEXT	ENDS
PUBLIC	?GetTotalLength@Misc@@YAHABVCStringArray@@@Z	; Misc::GetTotalLength
;	COMDAT ?GetTotalLength@Misc@@YAHABVCStringArray@@@Z
_TEXT	SEGMENT
_array$ = 8
_nLength$ = -4
_nItem$ = -8
_sItem$75158 = -12
?GetTotalLength@Misc@@YAHABVCStringArray@@@Z PROC NEAR	; Misc::GetTotalLength, COMDAT

; 431  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 432  : 	int nLength = 0;

	mov	DWORD PTR _nLength$[ebp], 0

; 433  : 	int nItem = array.GetSize();

	mov	ecx, DWORD PTR _array$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nItem$[ebp], eax
$L75156:

; 434  : 
; 435  : 	while (nItem--)

	mov	eax, DWORD PTR _nItem$[ebp]
	mov	ecx, DWORD PTR _nItem$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nItem$[ebp], ecx
	test	eax, eax
	je	SHORT $L75157

; 437  : 		const CString& sItem = GetItem(array, nItem);

	mov	edx, DWORD PTR _nItem$[ebp]
	push	edx
	mov	eax, DWORD PTR _array$[ebp]
	push	eax
	call	?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z ; Misc::GetItem
	add	esp, 8
	mov	DWORD PTR _sItem$75158[ebp], eax

; 438  : 		nLength += sItem.GetLength();

	mov	ecx, DWORD PTR _sItem$75158[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	ecx, DWORD PTR _nLength$[ebp]
	add	ecx, eax
	mov	DWORD PTR _nLength$[ebp], ecx

; 439  : 	}

	jmp	SHORT $L75156
$L75157:

; 440  : 
; 441  : 	return nLength;

	mov	eax, DWORD PTR _nLength$[ebp]

; 442  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetTotalLength@Misc@@YAHABVCStringArray@@@Z ENDP	; Misc::GetTotalLength
_TEXT	ENDS
PUBLIC	?Split@Misc@@YAHAAVCString@@0GH@Z		; Misc::Split
EXTRN	?Mid@CString@@QBE?AV1@H@Z:NEAR			; CString::Mid
EXTRN	?Left@CString@@QBE?AV1@H@Z:NEAR			; CString::Left
;	COMDAT xdata$x
; File D:\_CODE\Shared\Misc.cpp
xdata$x	SEGMENT
$T76511	DD	019930520H
	DD	02H
	DD	FLAT:$T76513
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76513	DD	0ffffffffH
	DD	FLAT:$L76508
	DD	0ffffffffH
	DD	FLAT:$L76509
xdata$x	ENDS
;	COMDAT ?Split@Misc@@YAHAAVCString@@0GH@Z
_TEXT	SEGMENT
_sText$ = 8
_sRest$ = 12
_cDelim$ = 16
_bTrim$ = 20
_nDelim$ = -16
$T76506 = -20
$T76507 = -24
__$EHRec$ = -12
?Split@Misc@@YAHAAVCString@@0GH@Z PROC NEAR		; Misc::Split, COMDAT

; 445  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76512
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

; 446  : 	int nDelim = sText.Find(cDelim);

	mov	ax, WORD PTR _cDelim$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sText$[ebp]
	call	?Find@CString@@QBEHG@Z			; CString::Find
	mov	DWORD PTR _nDelim$[ebp], eax

; 447  : 
; 448  : 	if (nDelim == -1)

	cmp	DWORD PTR _nDelim$[ebp], -1
	jne	SHORT $L75166

; 449  : 		return FALSE;

	xor	eax, eax
	jmp	$L75164
$L75166:

; 450  : 
; 451  : 	// else
; 452  : 	sRest = sText.Mid(nDelim + 1);

	mov	ecx, DWORD PTR _nDelim$[ebp]
	add	ecx, 1
	push	ecx
	lea	edx, DWORD PTR $T76506[ebp]
	push	edx
	mov	ecx, DWORD PTR _sText$[ebp]
	call	?Mid@CString@@QBE?AV1@H@Z		; CString::Mid
	mov	DWORD PTR -28+[ebp], eax
	mov	eax, DWORD PTR -28+[ebp]
	mov	DWORD PTR -32+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -32+[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sRest$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76506[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 453  : 	sText = sText.Left(nDelim);

	mov	edx, DWORD PTR _nDelim$[ebp]
	push	edx
	lea	eax, DWORD PTR $T76507[ebp]
	push	eax
	mov	ecx, DWORD PTR _sText$[ebp]
	call	?Left@CString@@QBE?AV1@H@Z		; CString::Left
	mov	DWORD PTR -36+[ebp], eax
	mov	ecx, DWORD PTR -36+[ebp]
	mov	DWORD PTR -40+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	mov	edx, DWORD PTR -40+[ebp]
	push	edx
	mov	ecx, DWORD PTR _sText$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76507[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 454  : 
; 455  : 	if (bTrim)

	cmp	DWORD PTR _bTrim$[ebp], 0
	je	SHORT $L75169

; 457  : 		Trim(sText);

	push	0
	mov	eax, DWORD PTR _sText$[ebp]
	push	eax
	call	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z	; Misc::Trim
	add	esp, 8

; 458  : 		Trim(sRest);

	push	0
	mov	ecx, DWORD PTR _sRest$[ebp]
	push	ecx
	call	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z	; Misc::Trim
	add	esp, 8
$L75169:

; 460  : 
; 461  : 	return TRUE;

	mov	eax, 1
$L75164:

; 462  : }

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
$L76508:
	lea	ecx, DWORD PTR $T76506[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76509:
	lea	ecx, DWORD PTR $T76507[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76512:
	mov	eax, OFFSET FLAT:$T76511
	jmp	___CxxFrameHandler
text$x	ENDS
?Split@Misc@@YAHAAVCString@@0GH@Z ENDP			; Misc::Split
EXTRN	?TrimRight@CString@@QAEXXZ:NEAR			; CString::TrimRight
EXTRN	?TrimLeft@CString@@QAEXXZ:NEAR			; CString::TrimLeft
EXTRN	?TrimRight@CString@@QAEXPBG@Z:NEAR		; CString::TrimRight
EXTRN	?TrimLeft@CString@@QAEXPBG@Z:NEAR		; CString::TrimLeft
;	COMDAT ?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z
_TEXT	SEGMENT
_sText$ = 8
_lpszTargets$ = 12
?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z PROC NEAR		; Misc::Trim, COMDAT

; 465  : {

	push	ebp
	mov	ebp, esp

; 466  : 	if (lpszTargets)

	cmp	DWORD PTR _lpszTargets$[ebp], 0
	je	SHORT $L75174

; 468  : 		sText.TrimLeft(lpszTargets);

	mov	eax, DWORD PTR _lpszTargets$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sText$[ebp]
	call	?TrimLeft@CString@@QAEXPBG@Z		; CString::TrimLeft

; 469  : 		sText.TrimRight(lpszTargets);

	mov	ecx, DWORD PTR _lpszTargets$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sText$[ebp]
	call	?TrimRight@CString@@QAEXPBG@Z		; CString::TrimRight

; 471  : 	else

	jmp	SHORT $L75175
$L75174:

; 473  : 		sText.TrimLeft();

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?TrimLeft@CString@@QAEXXZ		; CString::TrimLeft

; 474  : 		sText.TrimRight();

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?TrimRight@CString@@QAEXXZ		; CString::TrimRight
$L75175:

; 476  : 
; 477  : 	return sText;

	mov	eax, DWORD PTR _sText$[ebp]

; 478  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z ENDP		; Misc::Trim
_TEXT	ENDS
PUBLIC	?Split@Misc@@YAHABVCString@@AAVCStringArray@@PBGH@Z ; Misc::Split
PUBLIC	?Split@Misc@@YAHABVCString@@AAVCStringArray@@GH@Z ; Misc::Split
;	COMDAT ?Split@Misc@@YAHABVCString@@AAVCStringArray@@GH@Z
_TEXT	SEGMENT
_sText$ = 8
_aValues$ = 12
_cDelim$ = 16
_bAllowEmpty$ = 20
_szSep$ = -4
?Split@Misc@@YAHABVCString@@AAVCStringArray@@GH@Z PROC NEAR ; Misc::Split, COMDAT

; 481  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 482  : 	TCHAR szSep[2] = { cDelim, 0 };

	mov	ax, WORD PTR _cDelim$[ebp]
	mov	WORD PTR _szSep$[ebp], ax
	mov	WORD PTR _szSep$[ebp+2], 0

; 483  : 
; 484  : 	return Split(sText, aValues, szSep, bAllowEmpty);

	mov	ecx, DWORD PTR _bAllowEmpty$[ebp]
	push	ecx
	lea	edx, DWORD PTR _szSep$[ebp]
	push	edx
	mov	eax, DWORD PTR _aValues$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sText$[ebp]
	push	ecx
	call	?Split@Misc@@YAHABVCString@@AAVCStringArray@@PBGH@Z ; Misc::Split
	add	esp, 16					; 00000010H

; 485  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Split@Misc@@YAHABVCString@@AAVCStringArray@@GH@Z ENDP	; Misc::Split
_TEXT	ENDS
EXTRN	?RemoveAll@CStringArray@@QAEXXZ:NEAR		; CStringArray::RemoveAll
EXTRN	?Add@CStringArray@@QAEHABVCString@@@Z:NEAR	; CStringArray::Add
EXTRN	??YCString@@QAEABV0@G@Z:NEAR			; CString::operator+=
EXTRN	?Find@CString@@QBEHPBGH@Z:NEAR			; CString::Find
;	COMDAT xdata$x
; File D:\_CODE\Shared\Misc.cpp
xdata$x	SEGMENT
$T76528	DD	019930520H
	DD	03H
	DD	FLAT:$T76530
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76530	DD	0ffffffffH
	DD	FLAT:$L76524
	DD	00H
	DD	FLAT:$L76525
	DD	00H
	DD	FLAT:$L76526
xdata$x	ENDS
;	COMDAT ?Split@Misc@@YAHABVCString@@AAVCStringArray@@PBGH@Z
_TEXT	SEGMENT
_sText$ = 8
_aValues$ = 12
_szSep$ = 16
_bAllowEmpty$ = 20
_sSep$ = -36
_nLen$ = -32
_nSepLen$ = -40
_bInQuotes$ = -20
_bAddWord$ = -16
_sWord$ = -24
_nPos$ = -28
_chr$75201 = -44
$T76522 = -48
$T76523 = -52
__$EHRec$ = -12
?Split@Misc@@YAHABVCString@@AAVCStringArray@@PBGH@Z PROC NEAR ; Misc::Split, COMDAT

; 488  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76529
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 48					; 00000030H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-60]
	mov	ecx, 12					; 0000000cH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 489  : 	CString sSep(szSep);

	mov	eax, DWORD PTR _szSep$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 490  : 
; 491  : 	if (sSep.IsEmpty())

	lea	ecx, DWORD PTR _sSep$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75190

; 492  : 		sSep = GetListSeparator();

	lea	ecx, DWORD PTR $T76522[ebp]
	push	ecx
	call	?GetListSeparator@Misc@@YA?AVCString@@XZ ; Misc::GetListSeparator
	add	esp, 4
	mov	DWORD PTR -56+[ebp], eax
	mov	edx, DWORD PTR -56+[ebp]
	mov	DWORD PTR -60+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	eax, DWORD PTR -60+[ebp]
	push	eax
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T76522[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75190:

; 493  : 
; 494  : 	aValues.RemoveAll();

	mov	ecx, DWORD PTR _aValues$[ebp]
	call	?RemoveAll@CStringArray@@QAEXXZ		; CStringArray::RemoveAll

; 495  : 
; 496  : 	// parse on separator unless enclosed in double-quotes
; 497  : 	int nLen = sText.GetLength(), nSepLen = sSep.GetLength();

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR _nLen$[ebp], eax
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR _nSepLen$[ebp], eax

; 498  : 	BOOL bInQuotes = FALSE, bAddWord = FALSE;

	mov	DWORD PTR _bInQuotes$[ebp], 0
	mov	DWORD PTR _bAddWord$[ebp], 0

; 499  : 	CString sWord;

	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 500  : 
; 501  : 	for (int nPos = 0; nPos < nLen; nPos++)

	mov	DWORD PTR _nPos$[ebp], 0
	jmp	SHORT $L75198
$L75199:
	mov	ecx, DWORD PTR _nPos$[ebp]
	add	ecx, 1
	mov	DWORD PTR _nPos$[ebp], ecx
$L75198:
	mov	edx, DWORD PTR _nPos$[ebp]
	cmp	edx, DWORD PTR _nLen$[ebp]
	jge	$L75200

; 503  : 		TCHAR chr = sText[nPos];

	mov	eax, DWORD PTR _nPos$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sText$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	mov	WORD PTR _chr$75201[ebp], ax

; 504  : 
; 505  : 		// check if we're at the beginning of a separator string
; 506  : 		if (chr == sSep[0] && (nSepLen == 1 || sText.Find(sSep, nPos) == nPos))

	mov	esi, DWORD PTR _chr$75201[ebp]
	and	esi, 65535				; 0000ffffH
	push	0
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	and	eax, 65535				; 0000ffffH
	cmp	esi, eax
	jne	SHORT $L75202
	cmp	DWORD PTR _nSepLen$[ebp], 1
	je	SHORT $L75203
	mov	ecx, DWORD PTR _nPos$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _sText$[ebp]
	call	?Find@CString@@QBEHPBGH@Z		; CString::Find
	cmp	eax, DWORD PTR _nPos$[ebp]
	jne	SHORT $L75202
$L75203:

; 508  : 			if (bInQuotes)

	cmp	DWORD PTR _bInQuotes$[ebp], 0
	je	SHORT $L75204

; 509  : 				sWord += sSep;

	lea	edx, DWORD PTR _sSep$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=

; 510  : 			else

	jmp	SHORT $L75205
$L75204:

; 511  : 				bAddWord = TRUE;

	mov	DWORD PTR _bAddWord$[ebp], 1
$L75205:

; 512  : 
; 513  : 			nPos += nSepLen - 1; // minus 1 because the loop also increments

	mov	eax, DWORD PTR _nSepLen$[ebp]
	mov	ecx, DWORD PTR _nPos$[ebp]
	lea	edx, DWORD PTR [ecx+eax-1]
	mov	DWORD PTR _nPos$[ebp], edx

; 515  : 		else if (chr == '\"') // double quote

	jmp	SHORT $L75209
$L75202:
	mov	eax, DWORD PTR _chr$75201[ebp]
	and	eax, 65535				; 0000ffffH
	cmp	eax, 34					; 00000022H
	jne	SHORT $L75207

; 517  : 			// flip bInQuotes
; 518  : 			bInQuotes = !bInQuotes;

	xor	ecx, ecx
	cmp	DWORD PTR _bInQuotes$[ebp], 0
	sete	cl
	mov	DWORD PTR _bInQuotes$[ebp], ecx

; 519  : 
; 520  : 			// Workaround to handle Quoted empty string as last word
; 521  : 			if (!bInQuotes && bAllowEmpty && sWord.IsEmpty())

	cmp	DWORD PTR _bInQuotes$[ebp], 0
	jne	SHORT $L75208
	cmp	DWORD PTR _bAllowEmpty$[ebp], 0
	je	SHORT $L75208
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75208

; 522  : 				sWord = ' ';

	push	32					; 00000020H
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??4CString@@QAEABV0@D@Z			; CString::operator=
$L75208:

; 524  : 		else // everything else

	jmp	SHORT $L75209
$L75207:

; 526  : 			sWord += chr;

	mov	dx, WORD PTR _chr$75201[ebp]
	push	edx
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??YCString@@QAEABV0@G@Z			; CString::operator+=

; 527  : 
; 528  : 			// also if its the last char then add it
; 529  : 			bAddWord = (nPos == nLen - 1);

	mov	eax, DWORD PTR _nLen$[ebp]
	sub	eax, 1
	xor	ecx, ecx
	cmp	DWORD PTR _nPos$[ebp], eax
	sete	cl
	mov	DWORD PTR _bAddWord$[ebp], ecx
$L75209:

; 531  : 
; 532  : 		if (bAddWord)

	cmp	DWORD PTR _bAddWord$[ebp], 0
	je	SHORT $L75210

; 534  : 			Trim(sWord);

	push	0
	lea	edx, DWORD PTR _sWord$[ebp]
	push	edx
	call	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z	; Misc::Trim
	add	esp, 8

; 535  : 			
; 536  : 			if (bAllowEmpty || !sWord.IsEmpty())

	cmp	DWORD PTR _bAllowEmpty$[ebp], 0
	jne	SHORT $L75212
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75211
$L75212:

; 537  : 				aValues.Add(sWord);

	lea	eax, DWORD PTR _sWord$[ebp]
	push	eax
	mov	ecx, DWORD PTR _aValues$[ebp]
	call	?Add@CStringArray@@QAEHABVCString@@@Z	; CStringArray::Add
$L75211:

; 538  : 			
; 539  : 			sWord.Empty(); // next word

	lea	ecx, DWORD PTR _sWord$[ebp]
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 540  : 			bAddWord = FALSE;

	mov	DWORD PTR _bAddWord$[ebp], 0
$L75210:

; 542  : 	}

	jmp	$L75199
$L75200:

; 543  : 
; 544  : 	// if the string ends with a separator and allow empty then add an empty string
; 545  : 	if (!sWord.IsEmpty())

	lea	ecx, DWORD PTR _sWord$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75213

; 547  : 		Trim(sWord);

	push	0
	lea	ecx, DWORD PTR _sWord$[ebp]
	push	ecx
	call	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z	; Misc::Trim
	add	esp, 8

; 548  : 		aValues.Add(sWord);

	lea	edx, DWORD PTR _sWord$[ebp]
	push	edx
	mov	ecx, DWORD PTR _aValues$[ebp]
	call	?Add@CStringArray@@QAEHABVCString@@@Z	; CStringArray::Add

; 550  : 	else if (bAllowEmpty && nLen)

	jmp	SHORT $L75216
$L75213:
	cmp	DWORD PTR _bAllowEmpty$[ebp], 0
	je	SHORT $L75216
	cmp	DWORD PTR _nLen$[ebp], 0
	je	SHORT $L75216

; 552  : 		if (sText.Find(sSep, nLen - nSepLen) == (nLen - nSepLen))

	mov	eax, DWORD PTR _nLen$[ebp]
	sub	eax, DWORD PTR _nSepLen$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _sText$[ebp]
	call	?Find@CString@@QBEHPBGH@Z		; CString::Find
	mov	ecx, DWORD PTR _nLen$[ebp]
	sub	ecx, DWORD PTR _nSepLen$[ebp]
	cmp	eax, ecx
	jne	SHORT $L75216

; 553  : 			aValues.Add(sWord);

	lea	edx, DWORD PTR _sWord$[ebp]
	push	edx
	mov	ecx, DWORD PTR _aValues$[ebp]
	call	?Add@CStringArray@@QAEHABVCString@@@Z	; CStringArray::Add
$L75216:

; 555  : 
; 556  : 	return aValues.GetSize();

	mov	ecx, DWORD PTR _aValues$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR $T76523[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76523[ebp]

; 557  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 60					; 0000003cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76524:
	lea	ecx, DWORD PTR _sSep$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76525:
	lea	ecx, DWORD PTR $T76522[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76526:
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76529:
	mov	eax, OFFSET FLAT:$T76528
	jmp	___CxxFrameHandler
text$x	ENDS
?Split@Misc@@YAHABVCString@@AAVCStringArray@@PBGH@Z ENDP ; Misc::Split
PUBLIC	?MatchAll@Misc@@YAHABVCStringArray@@0HH@Z	; Misc::MatchAll
PUBLIC	?Find@Misc@@YAHABVCStringArray@@PBGHH@Z		; Misc::Find
PUBLIC	?NaturalCompare@Misc@@YAHPBG0@Z			; Misc::NaturalCompare
EXTRN	??9@YG_NABVCString@@0@Z:NEAR			; operator!=
;	COMDAT ?MatchAll@Misc@@YAHABVCStringArray@@0HH@Z
_TEXT	SEGMENT
_array1$ = 8
_array2$ = 12
_bOrderSensitive$ = 16
_bCaseSensitive$ = 20
_nSize1$ = -8
_nSize2$ = -12
_nItem$75228 = -16
_sItem1$75232 = -20
_sItem2$75233 = -24
_nItem1$ = -4
_sItem1$75242 = -28
?MatchAll@Misc@@YAHABVCStringArray@@0HH@Z PROC NEAR	; Misc::MatchAll, COMDAT

; 560  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 28					; 0000001cH
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 561  : 	int nSize1 = array1.GetSize();

	mov	ecx, DWORD PTR _array1$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nSize1$[ebp], eax

; 562  : 	int nSize2 = array2.GetSize();

	mov	ecx, DWORD PTR _array2$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nSize2$[ebp], eax

; 563  : 
; 564  : 	if (nSize1 != nSize2)

	mov	eax, DWORD PTR _nSize1$[ebp]
	cmp	eax, DWORD PTR _nSize2$[ebp]
	je	SHORT $L75226

; 565  : 		return 0;

	xor	eax, eax
	jmp	$L75223
$L75226:

; 566  : 
; 567  : 	if (bOrderSensitive)

	cmp	DWORD PTR _bOrderSensitive$[ebp], 0
	je	$L75227

; 569  : 		for (int nItem = 0; nItem < nSize1; nItem++)

	mov	DWORD PTR _nItem$75228[ebp], 0
	jmp	SHORT $L75229
$L75230:
	mov	ecx, DWORD PTR _nItem$75228[ebp]
	add	ecx, 1
	mov	DWORD PTR _nItem$75228[ebp], ecx
$L75229:
	mov	edx, DWORD PTR _nItem$75228[ebp]
	cmp	edx, DWORD PTR _nSize1$[ebp]
	jge	SHORT $L75231

; 571  : 			const CString& sItem1 = GetItem(array1, nItem);

	mov	eax, DWORD PTR _nItem$75228[ebp]
	push	eax
	mov	ecx, DWORD PTR _array1$[ebp]
	push	ecx
	call	?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z ; Misc::GetItem
	add	esp, 8
	mov	DWORD PTR _sItem1$75232[ebp], eax

; 572  : 			const CString& sItem2 = GetItem(array2, nItem);

	mov	edx, DWORD PTR _nItem$75228[ebp]
	push	edx
	mov	eax, DWORD PTR _array2$[ebp]
	push	eax
	call	?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z ; Misc::GetItem
	add	esp, 8
	mov	DWORD PTR _sItem2$75233[ebp], eax

; 573  : 
; 574  : 			// check for non-equality
; 575  : 			if (bCaseSensitive)

	cmp	DWORD PTR _bCaseSensitive$[ebp], 0
	je	SHORT $L75234

; 577  : 				if (sItem1 != sItem2)

	mov	ecx, DWORD PTR _sItem2$75233[ebp]
	push	ecx
	mov	edx, DWORD PTR _sItem1$75232[ebp]
	push	edx
	call	??9@YG_NABVCString@@0@Z			; operator!=
	and	eax, 255				; 000000ffH
	test	eax, eax
	je	SHORT $L75235

; 578  : 					return FALSE;

	xor	eax, eax
	jmp	$L75223
$L75235:

; 580  : 			else 

	jmp	SHORT $L75237
$L75234:

; 582  : 				if (NaturalCompare(sItem1, sItem2) != 0)

	mov	ecx, DWORD PTR _sItem2$75233[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _sItem1$75232[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?NaturalCompare@Misc@@YAHPBG0@Z		; Misc::NaturalCompare
	add	esp, 8
	test	eax, eax
	je	SHORT $L75237

; 583  : 					return FALSE;

	xor	eax, eax
	jmp	SHORT $L75223
$L75237:

; 585  : 		}

	jmp	SHORT $L75230
$L75231:

; 586  : 
; 587  : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L75223
$L75227:

; 589  : 
; 590  : 	// else order not important
; 591  : 	for (int nItem1 = 0; nItem1 < nSize1; nItem1++)

	mov	DWORD PTR _nItem1$[ebp], 0
	jmp	SHORT $L75239
$L75240:
	mov	eax, DWORD PTR _nItem1$[ebp]
	add	eax, 1
	mov	DWORD PTR _nItem1$[ebp], eax
$L75239:
	mov	ecx, DWORD PTR _nItem1$[ebp]
	cmp	ecx, DWORD PTR _nSize1$[ebp]
	jge	SHORT $L75241

; 593  : 		const CString& sItem1 = GetItem(array1, nItem1);

	mov	edx, DWORD PTR _nItem1$[ebp]
	push	edx
	mov	eax, DWORD PTR _array1$[ebp]
	push	eax
	call	?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z ; Misc::GetItem
	add	esp, 8
	mov	DWORD PTR _sItem1$75242[ebp], eax

; 594  : 
; 595  : 		// look for matching item in array2
; 596  : 		if (Find(array2, sItem1, bCaseSensitive, FALSE) == -1)

	push	0
	mov	ecx, DWORD PTR _bCaseSensitive$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sItem1$75242[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	edx, DWORD PTR _array2$[ebp]
	push	edx
	call	?Find@Misc@@YAHABVCStringArray@@PBGHH@Z	; Misc::Find
	add	esp, 16					; 00000010H
	cmp	eax, -1
	jne	SHORT $L75243

; 598  : 			// no-match found == not the same
; 599  : 			return FALSE;

	xor	eax, eax
	jmp	SHORT $L75223
$L75243:

; 601  : 	}

	jmp	SHORT $L75240
$L75241:

; 602  : 
; 603  : 	return TRUE;

	mov	eax, 1
$L75223:

; 604  : }

	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?MatchAll@Misc@@YAHABVCStringArray@@0HH@Z ENDP		; Misc::MatchAll
_TEXT	ENDS
PUBLIC	?MatchAny@Misc@@YAHABVCStringArray@@0HH@Z	; Misc::MatchAny
;	COMDAT ?MatchAny@Misc@@YAHABVCStringArray@@0HH@Z
_TEXT	SEGMENT
_array1$ = 8
_array2$ = 12
_bCaseSensitive$ = 16
_bPartialOK$ = 20
_nSize1$ = -8
_nItem1$ = -4
_sItem1$75255 = -12
?MatchAny@Misc@@YAHABVCStringArray@@0HH@Z PROC NEAR	; Misc::MatchAny, COMDAT

; 607  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 608  : 	int nSize1 = array1.GetSize();

	mov	ecx, DWORD PTR _array1$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nSize1$[ebp], eax

; 609  : 
; 610  : 	for (int nItem1 = 0; nItem1 < nSize1; nItem1++)

	mov	DWORD PTR _nItem1$[ebp], 0
	jmp	SHORT $L75252
$L75253:
	mov	eax, DWORD PTR _nItem1$[ebp]
	add	eax, 1
	mov	DWORD PTR _nItem1$[ebp], eax
$L75252:
	mov	ecx, DWORD PTR _nItem1$[ebp]
	cmp	ecx, DWORD PTR _nSize1$[ebp]
	jge	SHORT $L75254

; 612  : 		const CString& sItem1 = GetItem(array1, nItem1);

	mov	edx, DWORD PTR _nItem1$[ebp]
	push	edx
	mov	eax, DWORD PTR _array1$[ebp]
	push	eax
	call	?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z ; Misc::GetItem
	add	esp, 8
	mov	DWORD PTR _sItem1$75255[ebp], eax

; 613  : 
; 614  : 		// look for matching item
; 615  : 		if (Find(array2, sItem1, bCaseSensitive, bPartialOK) != -1)

	mov	ecx, DWORD PTR _bPartialOK$[ebp]
	push	ecx
	mov	edx, DWORD PTR _bCaseSensitive$[ebp]
	push	edx
	mov	ecx, DWORD PTR _sItem1$75255[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	eax, DWORD PTR _array2$[ebp]
	push	eax
	call	?Find@Misc@@YAHABVCStringArray@@PBGHH@Z	; Misc::Find
	add	esp, 16					; 00000010H
	cmp	eax, -1
	je	SHORT $L75256

; 616  : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L75249
$L75256:

; 617  : 	}

	jmp	SHORT $L75253
$L75254:

; 618  : 	
; 619  : 	return FALSE;

	xor	eax, eax
$L75249:

; 620  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?MatchAny@Misc@@YAHABVCStringArray@@0HH@Z ENDP		; Misc::MatchAny
_TEXT	ENDS
PUBLIC	?FindWord@Misc@@YAHPBG0HH@Z			; Misc::FindWord
EXTRN	?Find@CString@@QBEHPBG@Z:NEAR			; CString::Find
EXTRN	??8@YG_NABVCString@@PBG@Z:NEAR			; operator==
;	COMDAT ?Find@Misc@@YAHABVCStringArray@@PBGHH@Z
_TEXT	SEGMENT
_array$ = 8
_szItem$ = 12
_bCaseSensitive$ = 16
_bPartialOK$ = 20
_nSize$ = -4
_nItem$ = -8
_sArrItem$75273 = -12
?Find@Misc@@YAHABVCStringArray@@PBGHH@Z PROC NEAR	; Misc::Find, COMDAT

; 623  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L75263:

; 624  : 	ASSERT (szItem);

	cmp	DWORD PTR _szItem$[ebp], 0
	jne	SHORT $L75266
	push	624					; 00000270H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75266
	int	3
$L75266:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75263

; 625  : 
; 626  : 	if (szItem == NULL)

	cmp	DWORD PTR _szItem$[ebp], 0
	jne	SHORT $L75267

; 627  : 		return -1;

	or	eax, -1
	jmp	$L75262
$L75267:

; 628  : 
; 629  : 	int nSize = array.GetSize();

	mov	ecx, DWORD PTR _array$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nSize$[ebp], eax

; 630  : 
; 631  : 	for (int nItem = 0; nItem < nSize; nItem++)

	mov	DWORD PTR _nItem$[ebp], 0
	jmp	SHORT $L75270
$L75271:
	mov	ecx, DWORD PTR _nItem$[ebp]
	add	ecx, 1
	mov	DWORD PTR _nItem$[ebp], ecx
$L75270:
	mov	edx, DWORD PTR _nItem$[ebp]
	cmp	edx, DWORD PTR _nSize$[ebp]
	jge	$L75272

; 633  : 		// look for matching item
; 634  : 		const CString& sArrItem = GetItem(array, nItem);

	mov	eax, DWORD PTR _nItem$[ebp]
	push	eax
	mov	ecx, DWORD PTR _array$[ebp]
	push	ecx
	call	?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z ; Misc::GetItem
	add	esp, 8
	mov	DWORD PTR _sArrItem$75273[ebp], eax

; 635  : 
; 636  : 		// special case: empty item
; 637  : 		if (szItem[0] == 0)

	mov	edx, DWORD PTR _szItem$[ebp]
	xor	eax, eax
	mov	ax, WORD PTR [edx]
	test	eax, eax
	jne	SHORT $L75274

; 639  : 			if (sArrItem.IsEmpty())

	mov	ecx, DWORD PTR _sArrItem$75273[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75275

; 640  : 				return nItem;

	mov	eax, DWORD PTR _nItem$[ebp]
	jmp	$L75262
$L75275:

; 642  : 		else if (bCaseSensitive)

	jmp	$L75286
$L75274:
	cmp	DWORD PTR _bCaseSensitive$[ebp], 0
	je	SHORT $L75277

; 644  : 			if (bPartialOK)

	cmp	DWORD PTR _bPartialOK$[ebp], 0
	je	SHORT $L75278

; 646  : 				if (sArrItem.Find(szItem) != -1)

	mov	ecx, DWORD PTR _szItem$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sArrItem$75273[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	cmp	eax, -1
	je	SHORT $L75279

; 647  : 					return nItem;

	mov	eax, DWORD PTR _nItem$[ebp]
	jmp	SHORT $L75262
$L75279:

; 649  : 			else

	jmp	SHORT $L75281
$L75278:

; 651  : 				if (sArrItem == szItem)

	mov	edx, DWORD PTR _szItem$[ebp]
	push	edx
	mov	eax, DWORD PTR _sArrItem$75273[ebp]
	push	eax
	call	??8@YG_NABVCString@@PBG@Z		; operator==
	and	eax, 255				; 000000ffH
	test	eax, eax
	je	SHORT $L75281

; 652  : 					return nItem;

	mov	eax, DWORD PTR _nItem$[ebp]
	jmp	SHORT $L75262
$L75281:

; 655  : 		else // case sensitive

	jmp	SHORT $L75286
$L75277:

; 657  : 			if (bPartialOK)

	cmp	DWORD PTR _bPartialOK$[ebp], 0
	je	SHORT $L75283

; 659  : 				if (FindWord(szItem, sArrItem, TRUE, FALSE))

	push	0
	push	1
	mov	ecx, DWORD PTR _sArrItem$75273[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _szItem$[ebp]
	push	ecx
	call	?FindWord@Misc@@YAHPBG0HH@Z		; Misc::FindWord
	add	esp, 16					; 00000010H
	test	eax, eax
	je	SHORT $L75284

; 660  : 					return nItem;

	mov	eax, DWORD PTR _nItem$[ebp]
	jmp	SHORT $L75262
$L75284:

; 662  : 			else

	jmp	SHORT $L75286
$L75283:

; 664  : 				if (NaturalCompare(sArrItem, szItem) == 0)

	mov	edx, DWORD PTR _szItem$[ebp]
	push	edx
	mov	ecx, DWORD PTR _sArrItem$75273[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?NaturalCompare@Misc@@YAHPBG0@Z		; Misc::NaturalCompare
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75286

; 665  : 					return nItem;

	mov	eax, DWORD PTR _nItem$[ebp]
	jmp	SHORT $L75262
$L75286:

; 668  : 	}

	jmp	$L75271
$L75272:

; 669  : 
; 670  : 	return -1;

	or	eax, -1
$L75262:

; 671  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Find@Misc@@YAHABVCStringArray@@PBGHH@Z ENDP		; Misc::Find
_TEXT	ENDS
EXTRN	?GetData@CStringArray@@QBEPBVCString@@XZ:NEAR	; CStringArray::GetData
_BSS	SEGMENT
	ALIGN	4

_?sDummy@?8??GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z@4V3@A DD 01H DUP (?)
_?$S280@?8??GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT ?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z
_TEXT	SEGMENT
_array$ = 8
_nItem$ = 12
?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z PROC NEAR ; Misc::GetItem, COMDAT

; 674  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L75291:

; 675  : 	ASSERT(nItem >= 0 && nItem < array.GetSize());

	cmp	DWORD PTR _nItem$[ebp], 0
	jl	SHORT $L75295
	mov	ecx, DWORD PTR _array$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	cmp	DWORD PTR _nItem$[ebp], eax
	jl	SHORT $L75294
$L75295:
	push	675					; 000002a3H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75294
	int	3
$L75294:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75291

; 676  : 
; 677  : 	if (nItem < 0 || nItem >= array.GetSize())

	cmp	DWORD PTR _nItem$[ebp], 0
	jl	SHORT $L75297
	mov	ecx, DWORD PTR _array$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	cmp	DWORD PTR _nItem$[ebp], eax
	jl	SHORT $L75296
$L75297:

; 679  : 		static CString sDummy;

	xor	ecx, ecx
	mov	cl, BYTE PTR _?$S280@?8??GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z@4EA
	and	ecx, 1
	test	ecx, ecx
	jne	SHORT $L75302
	mov	dl, BYTE PTR _?$S280@?8??GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z@4EA
	or	dl, 1
	mov	BYTE PTR _?$S280@?8??GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z@4EA, dl
	mov	ecx, OFFSET FLAT:_?sDummy@?8??GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z@4V3@A
	call	??0CString@@QAE@XZ			; CString::CString
	push	OFFSET FLAT:_$E281
	call	_atexit
	add	esp, 4
$L75302:

; 680  : 		return sDummy;

	mov	eax, OFFSET FLAT:_?sDummy@?8??GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z@4V3@A
	jmp	SHORT $L75290
$L75296:

; 682  : 
; 683  : 	return array.GetData()[nItem];

	mov	ecx, DWORD PTR _array$[ebp]
	call	?GetData@CStringArray@@QBEPBVCString@@XZ ; CStringArray::GetData
	mov	ecx, DWORD PTR _nItem$[ebp]
	lea	eax, DWORD PTR [eax+ecx*4]
$L75290:

; 684  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z ENDP	; Misc::GetItem
_TEXT	ENDS
;	COMDAT _$E281
_TEXT	SEGMENT
_$E281	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?sDummy@?8??GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z@4V3@A
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E281	ENDP
_TEXT	ENDS
PUBLIC	?RemoveEmptyItems@Misc@@YAHAAVCStringArray@@@Z	; Misc::RemoveEmptyItems
EXTRN	??ACStringArray@@QAEAAVCString@@H@Z:NEAR	; CStringArray::operator[]
EXTRN	?RemoveAt@CStringArray@@QAEXHH@Z:NEAR		; CStringArray::RemoveAt
;	COMDAT ?RemoveEmptyItems@Misc@@YAHAAVCStringArray@@@Z
_TEXT	SEGMENT
_aFrom$ = 8
_nRemoved$ = -8
_nItem$ = -4
_sItem$75313 = -12
?RemoveEmptyItems@Misc@@YAHAAVCStringArray@@@Z PROC NEAR ; Misc::RemoveEmptyItems, COMDAT

; 687  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 688  : 	int nRemoved = 0; // counter

	mov	DWORD PTR _nRemoved$[ebp], 0

; 689  : 	int nItem = aFrom.GetSize();

	mov	ecx, DWORD PTR _aFrom$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nItem$[ebp], eax
$L75311:

; 690  : 
; 691  : 	while (nItem--)

	mov	eax, DWORD PTR _nItem$[ebp]
	mov	ecx, DWORD PTR _nItem$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nItem$[ebp], ecx
	test	eax, eax
	je	SHORT $L75312

; 693  : 		CString& sItem = aFrom[nItem];

	mov	edx, DWORD PTR _nItem$[ebp]
	push	edx
	mov	ecx, DWORD PTR _aFrom$[ebp]
	call	??ACStringArray@@QAEAAVCString@@H@Z	; CStringArray::operator[]
	mov	DWORD PTR _sItem$75313[ebp], eax

; 694  : 		Trim(sItem);

	push	0
	mov	eax, DWORD PTR _sItem$75313[ebp]
	push	eax
	call	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z	; Misc::Trim
	add	esp, 8

; 695  : 
; 696  : 		if (sItem.IsEmpty())

	mov	ecx, DWORD PTR _sItem$75313[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75314

; 698  : 			aFrom.RemoveAt(nItem);

	push	1
	mov	ecx, DWORD PTR _nItem$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _aFrom$[ebp]
	call	?RemoveAt@CStringArray@@QAEXHH@Z	; CStringArray::RemoveAt

; 699  : 			nRemoved++;

	mov	edx, DWORD PTR _nRemoved$[ebp]
	add	edx, 1
	mov	DWORD PTR _nRemoved$[ebp], edx
$L75314:

; 701  : 	}

	jmp	SHORT $L75311
$L75312:

; 702  : 
; 703  : 	return nRemoved;

	mov	eax, DWORD PTR _nRemoved$[ebp]

; 704  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?RemoveEmptyItems@Misc@@YAHAAVCStringArray@@@Z ENDP	; Misc::RemoveEmptyItems
_TEXT	ENDS
PUBLIC	?RemoveItems@Misc@@YAHABVCStringArray@@AAV2@H@Z	; Misc::RemoveItems
PUBLIC	?RemoveItem@Misc@@YAHPBGAAVCStringArray@@H@Z	; Misc::RemoveItem
;	COMDAT ?RemoveItems@Misc@@YAHABVCStringArray@@AAV2@H@Z
_TEXT	SEGMENT
_array$ = 8
_aFrom$ = 12
_bCaseSensitive$ = 16
_nRemoved$ = -8
_nItem$ = -4
_sItem$75325 = -12
?RemoveItems@Misc@@YAHABVCStringArray@@AAV2@H@Z PROC NEAR ; Misc::RemoveItems, COMDAT

; 707  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 708  : 	int nRemoved = 0; // counter

	mov	DWORD PTR _nRemoved$[ebp], 0

; 709  : 	int nItem = array.GetSize();

	mov	ecx, DWORD PTR _array$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nItem$[ebp], eax
$L75323:

; 710  : 
; 711  : 	while (nItem--)

	mov	eax, DWORD PTR _nItem$[ebp]
	mov	ecx, DWORD PTR _nItem$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nItem$[ebp], ecx
	test	eax, eax
	je	SHORT $L75324

; 713  : 		const CString& sItem = GetItem(array, nItem);

	mov	edx, DWORD PTR _nItem$[ebp]
	push	edx
	mov	eax, DWORD PTR _array$[ebp]
	push	eax
	call	?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z ; Misc::GetItem
	add	esp, 8
	mov	DWORD PTR _sItem$75325[ebp], eax

; 714  : 
; 715  : 		if (RemoveItem(sItem, aFrom, bCaseSensitive))

	mov	ecx, DWORD PTR _bCaseSensitive$[ebp]
	push	ecx
	mov	edx, DWORD PTR _aFrom$[ebp]
	push	edx
	mov	ecx, DWORD PTR _sItem$75325[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?RemoveItem@Misc@@YAHPBGAAVCStringArray@@H@Z ; Misc::RemoveItem
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	SHORT $L75326

; 716  : 			nRemoved++;

	mov	eax, DWORD PTR _nRemoved$[ebp]
	add	eax, 1
	mov	DWORD PTR _nRemoved$[ebp], eax
$L75326:

; 717  : 	}

	jmp	SHORT $L75323
$L75324:

; 718  : 
; 719  : 	return nRemoved;

	mov	eax, DWORD PTR _nRemoved$[ebp]

; 720  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?RemoveItems@Misc@@YAHABVCStringArray@@AAV2@H@Z ENDP	; Misc::RemoveItems
_TEXT	ENDS
;	COMDAT ?RemoveItem@Misc@@YAHPBGAAVCStringArray@@H@Z
_TEXT	SEGMENT
_szItem$ = 8
_aFrom$ = 12
_bCaseSensitive$ = 16
_nFind$ = -4
?RemoveItem@Misc@@YAHPBGAAVCStringArray@@H@Z PROC NEAR	; Misc::RemoveItem, COMDAT

; 723  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 724  : 	int nFind = Find(aFrom, szItem, bCaseSensitive, FALSE);

	push	0
	mov	eax, DWORD PTR _bCaseSensitive$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szItem$[ebp]
	push	ecx
	mov	edx, DWORD PTR _aFrom$[ebp]
	push	edx
	call	?Find@Misc@@YAHABVCStringArray@@PBGHH@Z	; Misc::Find
	add	esp, 16					; 00000010H
	mov	DWORD PTR _nFind$[ebp], eax

; 725  : 	
; 726  : 	if (nFind != -1)

	cmp	DWORD PTR _nFind$[ebp], -1
	je	SHORT $L75333

; 727  : 		aFrom.RemoveAt(nFind);

	push	1
	mov	eax, DWORD PTR _nFind$[ebp]
	push	eax
	mov	ecx, DWORD PTR _aFrom$[ebp]
	call	?RemoveAt@CStringArray@@QAEXHH@Z	; CStringArray::RemoveAt
$L75333:

; 728  : 
; 729  : 	return (nFind != -1);

	xor	eax, eax
	cmp	DWORD PTR _nFind$[ebp], -1
	setne	al

; 730  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?RemoveItem@Misc@@YAHPBGAAVCStringArray@@H@Z ENDP	; Misc::RemoveItem
_TEXT	ENDS
PUBLIC	?AddUniqueItems@Misc@@YAHABVCStringArray@@AAV2@H@Z ; Misc::AddUniqueItems
PUBLIC	?AddUniqueItem@Misc@@YAHABVCString@@AAVCStringArray@@H@Z ; Misc::AddUniqueItem
;	COMDAT ?AddUniqueItems@Misc@@YAHABVCStringArray@@AAV2@H@Z
_TEXT	SEGMENT
_array$ = 8
_aTo$ = 12
_bCaseSensitive$ = 16
_nAdded$ = -4
_nItem$ = -8
_sItem$75344 = -12
?AddUniqueItems@Misc@@YAHABVCStringArray@@AAV2@H@Z PROC NEAR ; Misc::AddUniqueItems, COMDAT

; 733  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 734  : 	int nAdded = 0; // counter

	mov	DWORD PTR _nAdded$[ebp], 0

; 735  : 	int nItem = array.GetSize();

	mov	ecx, DWORD PTR _array$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nItem$[ebp], eax
$L75342:

; 736  : 
; 737  : 	while (nItem--)

	mov	eax, DWORD PTR _nItem$[ebp]
	mov	ecx, DWORD PTR _nItem$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nItem$[ebp], ecx
	test	eax, eax
	je	SHORT $L75343

; 739  : 		const CString& sItem = GetItem(array, nItem);

	mov	edx, DWORD PTR _nItem$[ebp]
	push	edx
	mov	eax, DWORD PTR _array$[ebp]
	push	eax
	call	?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z ; Misc::GetItem
	add	esp, 8
	mov	DWORD PTR _sItem$75344[ebp], eax

; 740  : 
; 741  : 		if (AddUniqueItem(sItem, aTo, bCaseSensitive))

	mov	ecx, DWORD PTR _bCaseSensitive$[ebp]
	push	ecx
	mov	edx, DWORD PTR _aTo$[ebp]
	push	edx
	mov	eax, DWORD PTR _sItem$75344[ebp]
	push	eax
	call	?AddUniqueItem@Misc@@YAHABVCString@@AAVCStringArray@@H@Z ; Misc::AddUniqueItem
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	SHORT $L75345

; 742  : 			nAdded++;

	mov	ecx, DWORD PTR _nAdded$[ebp]
	add	ecx, 1
	mov	DWORD PTR _nAdded$[ebp], ecx
$L75345:

; 743  : 	}

	jmp	SHORT $L75342
$L75343:

; 744  : 
; 745  : 	return nAdded;

	mov	eax, DWORD PTR _nAdded$[ebp]

; 746  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?AddUniqueItems@Misc@@YAHABVCStringArray@@AAV2@H@Z ENDP	; Misc::AddUniqueItems
_TEXT	ENDS
;	COMDAT ?AddUniqueItem@Misc@@YAHABVCString@@AAVCStringArray@@H@Z
_TEXT	SEGMENT
_sItem$ = 8
_aTo$ = 12
_bCaseSensitive$ = 16
_nFind$ = -4
?AddUniqueItem@Misc@@YAHABVCString@@AAVCStringArray@@H@Z PROC NEAR ; Misc::AddUniqueItem, COMDAT

; 749  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 750  : 	if (sItem.IsEmpty())

	mov	ecx, DWORD PTR _sItem$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75351

; 751  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75350
$L75351:

; 752  : 
; 753  : 	int nFind = Find(aTo, sItem, bCaseSensitive, FALSE);

	push	0
	mov	eax, DWORD PTR _bCaseSensitive$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sItem$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _aTo$[ebp]
	push	ecx
	call	?Find@Misc@@YAHABVCStringArray@@PBGHH@Z	; Misc::Find
	add	esp, 16					; 00000010H
	mov	DWORD PTR _nFind$[ebp], eax

; 754  : 
; 755  : 	if (nFind == -1) // doesn't already exist

	cmp	DWORD PTR _nFind$[ebp], -1
	jne	SHORT $L75353

; 757  : 		aTo.Add(sItem);

	mov	edx, DWORD PTR _sItem$[ebp]
	push	edx
	mov	ecx, DWORD PTR _aTo$[ebp]
	call	?Add@CStringArray@@QAEHABVCString@@@Z	; CStringArray::Add

; 758  : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L75350
$L75353:

; 760  : 
; 761  : 	return FALSE; // not added

	xor	eax, eax
$L75350:

; 762  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?AddUniqueItem@Misc@@YAHABVCString@@AAVCStringArray@@H@Z ENDP ; Misc::AddUniqueItem
_TEXT	ENDS
PUBLIC	?GetAM@Misc@@YA?AVCString@@XZ			; Misc::GetAM
_BSS	SEGMENT
	ALIGN	4

_?sAM@?1??GetAM@Misc@@YA?AVCString@@XZ@4V3@A DD 01H DUP (?)
_?$S282@?1??GetAM@Misc@@YA?AVCString@@XZ@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT ?GetAM@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_BUFLEN$ = -4
$T76556 = -8
?GetAM@Misc@@YA?AVCString@@XZ PROC NEAR			; Misc::GetAM, COMDAT

; 765  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T76556[ebp], 0

; 766  : 	static CString sAM;

	xor	eax, eax
	mov	al, BYTE PTR _?$S282@?1??GetAM@Misc@@YA?AVCString@@XZ@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75361
	mov	cl, BYTE PTR _?$S282@?1??GetAM@Misc@@YA?AVCString@@XZ@4EA
	or	cl, 1
	mov	BYTE PTR _?$S282@?1??GetAM@Misc@@YA?AVCString@@XZ@4EA, cl
	mov	ecx, OFFSET FLAT:_?sAM@?1??GetAM@Misc@@YA?AVCString@@XZ@4V3@A
	call	??0CString@@QAE@XZ			; CString::CString
	push	OFFSET FLAT:_$E283
	call	_atexit
	add	esp, 4
$L75361:

; 767  : 	const int BUFLEN = 10;

	mov	DWORD PTR _BUFLEN$[ebp], 10		; 0000000aH

; 768  : 
; 769  : 	if (sAM.IsEmpty()) // init first time only

	mov	ecx, OFFSET FLAT:_?sAM@?1??GetAM@Misc@@YA?AVCString@@XZ@4V3@A
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75364

; 771  : 		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_S1159, sAM.GetBuffer(BUFLEN), BUFLEN - 1);

	mov	esi, esp
	push	9
	push	10					; 0000000aH
	mov	ecx, OFFSET FLAT:_?sAM@?1??GetAM@Misc@@YA?AVCString@@XZ@4V3@A
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	push	40					; 00000028H
	push	1024					; 00000400H
	call	DWORD PTR __imp__GetLocaleInfoW@16
	cmp	esi, esp
	call	__chkesp

; 772  : 		sAM.ReleaseBuffer();

	push	-1
	mov	ecx, OFFSET FLAT:_?sAM@?1??GetAM@Misc@@YA?AVCString@@XZ@4V3@A
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer
$L75364:

; 774  : 
; 775  : 	return sAM;

	push	OFFSET FLAT:_?sAM@?1??GetAM@Misc@@YA?AVCString@@XZ@4V3@A
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76556[ebp]
	or	edx, 1
	mov	DWORD PTR $T76556[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 776  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetAM@Misc@@YA?AVCString@@XZ ENDP			; Misc::GetAM
_TEXT	ENDS
;	COMDAT _$E283
_TEXT	SEGMENT
_$E283	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?sAM@?1??GetAM@Misc@@YA?AVCString@@XZ@4V3@A
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E283	ENDP
_TEXT	ENDS
PUBLIC	?GetPM@Misc@@YA?AVCString@@XZ			; Misc::GetPM
_BSS	SEGMENT
	ALIGN	4

_?sPM@?1??GetPM@Misc@@YA?AVCString@@XZ@4V3@A DD 01H DUP (?)
_?$S284@?1??GetPM@Misc@@YA?AVCString@@XZ@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT ?GetPM@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_BUFLEN$ = -4
$T76563 = -8
?GetPM@Misc@@YA?AVCString@@XZ PROC NEAR			; Misc::GetPM, COMDAT

; 779  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T76563[ebp], 0

; 780  : 	static CString sPM;

	xor	eax, eax
	mov	al, BYTE PTR _?$S284@?1??GetPM@Misc@@YA?AVCString@@XZ@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75380
	mov	cl, BYTE PTR _?$S284@?1??GetPM@Misc@@YA?AVCString@@XZ@4EA
	or	cl, 1
	mov	BYTE PTR _?$S284@?1??GetPM@Misc@@YA?AVCString@@XZ@4EA, cl
	mov	ecx, OFFSET FLAT:_?sPM@?1??GetPM@Misc@@YA?AVCString@@XZ@4V3@A
	call	??0CString@@QAE@XZ			; CString::CString
	push	OFFSET FLAT:_$E285
	call	_atexit
	add	esp, 4
$L75380:

; 781  : 	const int BUFLEN = 10;

	mov	DWORD PTR _BUFLEN$[ebp], 10		; 0000000aH

; 782  : 
; 783  : 	if (sPM.IsEmpty()) // init first time only

	mov	ecx, OFFSET FLAT:_?sPM@?1??GetPM@Misc@@YA?AVCString@@XZ@4V3@A
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75383

; 785  : 		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_S2359, sPM.GetBuffer(BUFLEN), BUFLEN - 1);

	mov	esi, esp
	push	9
	push	10					; 0000000aH
	mov	ecx, OFFSET FLAT:_?sPM@?1??GetPM@Misc@@YA?AVCString@@XZ@4V3@A
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	push	41					; 00000029H
	push	1024					; 00000400H
	call	DWORD PTR __imp__GetLocaleInfoW@16
	cmp	esi, esp
	call	__chkesp

; 786  : 		sPM.ReleaseBuffer();

	push	-1
	mov	ecx, OFFSET FLAT:_?sPM@?1??GetPM@Misc@@YA?AVCString@@XZ@4V3@A
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer
$L75383:

; 788  : 
; 789  : 	return sPM;

	push	OFFSET FLAT:_?sPM@?1??GetPM@Misc@@YA?AVCString@@XZ@4V3@A
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76563[ebp]
	or	edx, 1
	mov	DWORD PTR $T76563[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 790  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetPM@Misc@@YA?AVCString@@XZ ENDP			; Misc::GetPM
_TEXT	ENDS
;	COMDAT _$E285
_TEXT	SEGMENT
_$E285	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?sPM@?1??GetPM@Misc@@YA?AVCString@@XZ@4V3@A
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E285	ENDP
_TEXT	ENDS
PUBLIC	?GetTimeSeparator@Misc@@YA?AVCString@@XZ	; Misc::GetTimeSeparator
PUBLIC	?GetTimeFormatW@Misc@@YA?AVCString@@H@Z		; Misc::GetTimeFormatW
EXTRN	??H@YG?AVCString@@ABV0@0@Z:NEAR			; operator+
EXTRN	?Replace@CString@@QAEHPBG0@Z:NEAR		; CString::Replace
_BSS	SEGMENT
	ALIGN	4

_?sFormat@?1??GetTimeFormatW@Misc@@YA?AVCString@@H@Z@4V3@A DD 01H DUP (?)
_?$S286@?1??GetTimeFormatW@Misc@@YA?AVCString@@H@Z@4EA DB 01H DUP (?)
	ALIGN	4

$SG75418 DW	01H DUP (?)
	ALIGN	4

$SG75423 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75419 DB	'ss', 00H
	ORG $+1
$SG75424 DB	's', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76587	DD	019930520H
	DD	07H
	DD	FLAT:$T76589
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76589	DD	0ffffffffH
	DD	FLAT:$L76576
	DD	00H
	DD	FLAT:$L76577
	DD	01H
	DD	FLAT:$L76578
	DD	02H
	DD	FLAT:$L76579
	DD	00H
	DD	FLAT:$L76580
	DD	04H
	DD	FLAT:$L76581
	DD	05H
	DD	FLAT:$L76582
xdata$x	ENDS
;	COMDAT ?GetTimeFormatW@Misc@@YA?AVCString@@H@Z
_TEXT	SEGMENT
_bIncSeconds$ = 12
___$ReturnUdt$ = 8
_BUFLEN$ = -16
_sTemp$75412 = -20
$T76569 = -24
$T76570 = -28
$T76571 = -32
$T76572 = -36
$T76573 = -40
$T76574 = -44
$T76575 = -48
$T76584 = -52
__$EHRec$ = -12
?GetTimeFormatW@Misc@@YA?AVCString@@H@Z PROC NEAR	; Misc::GetTimeFormatW, COMDAT

; 793  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76588
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 72					; 00000048H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-84]
	mov	ecx, 18					; 00000012H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T76584[ebp], 0

; 794  : 	static CString sFormat;

	xor	eax, eax
	mov	al, BYTE PTR _?$S286@?1??GetTimeFormatW@Misc@@YA?AVCString@@H@Z@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75400
	mov	cl, BYTE PTR _?$S286@?1??GetTimeFormatW@Misc@@YA?AVCString@@H@Z@4EA
	or	cl, 1
	mov	BYTE PTR _?$S286@?1??GetTimeFormatW@Misc@@YA?AVCString@@H@Z@4EA, cl
	mov	ecx, OFFSET FLAT:_?sFormat@?1??GetTimeFormatW@Misc@@YA?AVCString@@H@Z@4V3@A
	call	??0CString@@QAE@XZ			; CString::CString
	push	OFFSET FLAT:_$E287
	call	_atexit
	add	esp, 4
$L75400:

; 795  : 	const int BUFLEN = 100;

	mov	DWORD PTR _BUFLEN$[ebp], 100		; 00000064H

; 796  : 
; 797  : 	if (sFormat.IsEmpty()) // init first time only

	mov	ecx, OFFSET FLAT:_?sFormat@?1??GetTimeFormatW@Misc@@YA?AVCString@@H@Z@4V3@A
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75403

; 799  : 		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIMEFORMAT, sFormat.GetBuffer(BUFLEN), BUFLEN - 1);

	mov	esi, esp
	push	99					; 00000063H
	push	100					; 00000064H
	mov	ecx, OFFSET FLAT:_?sFormat@?1??GetTimeFormatW@Misc@@YA?AVCString@@H@Z@4V3@A
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	push	4099					; 00001003H
	push	1024					; 00000400H
	call	DWORD PTR __imp__GetLocaleInfoW@16
	cmp	esi, esp
	call	__chkesp

; 800  : 		sFormat.ReleaseBuffer();

	push	-1
	mov	ecx, OFFSET FLAT:_?sFormat@?1??GetTimeFormatW@Misc@@YA?AVCString@@H@Z@4V3@A
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer
$L75403:

; 802  : 
; 803  : 	if (!bIncSeconds)

	cmp	DWORD PTR _bIncSeconds$[ebp], 0
	jne	$L75411

; 805  : 		CString sTemp(sFormat);

	push	OFFSET FLAT:_?sFormat@?1??GetTimeFormatW@Misc@@YA?AVCString@@H@Z@4V3@A
	lea	ecx, DWORD PTR _sTemp$75412[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 806  : 
; 807  : 		if (!sTemp.Replace(GetTimeSeparator() + "ss", _T("")))

	push	OFFSET FLAT:$SG75419
	lea	ecx, DWORD PTR $T76570[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	push	OFFSET FLAT:$SG75418
	lea	edx, DWORD PTR $T76570[ebp]
	push	edx
	lea	eax, DWORD PTR $T76571[ebp]
	push	eax
	call	?GetTimeSeparator@Misc@@YA?AVCString@@XZ ; Misc::GetTimeSeparator
	add	esp, 4
	mov	DWORD PTR -56+[ebp], eax
	mov	ecx, DWORD PTR -56+[ebp]
	mov	DWORD PTR -60+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	edx, DWORD PTR -60+[ebp]
	push	edx
	lea	eax, DWORD PTR $T76572[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -64+[ebp], eax
	mov	ecx, DWORD PTR -64+[ebp]
	mov	DWORD PTR -68+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR -68+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sTemp$75412[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	BYTE PTR $T76569[ebp], al
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T76572[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T76571[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T76570[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	edx, DWORD PTR $T76569[ebp]
	and	edx, 255				; 000000ffH
	test	edx, edx
	je	$L75416

; 808  : 			sTemp.Replace(GetTimeSeparator() + "s", _T(""));

	push	OFFSET FLAT:$SG75424
	lea	ecx, DWORD PTR $T76573[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	push	OFFSET FLAT:$SG75423
	lea	eax, DWORD PTR $T76573[ebp]
	push	eax
	lea	ecx, DWORD PTR $T76574[ebp]
	push	ecx
	call	?GetTimeSeparator@Misc@@YA?AVCString@@XZ ; Misc::GetTimeSeparator
	add	esp, 4
	mov	DWORD PTR -72+[ebp], eax
	mov	edx, DWORD PTR -72+[ebp]
	mov	DWORD PTR -76+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	mov	eax, DWORD PTR -76+[ebp]
	push	eax
	lea	ecx, DWORD PTR $T76575[ebp]
	push	ecx
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -80+[ebp], eax
	mov	edx, DWORD PTR -80+[ebp]
	mov	DWORD PTR -84+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	mov	ecx, DWORD PTR -84+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sTemp$75412[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	lea	ecx, DWORD PTR $T76575[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR $T76574[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T76573[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75416:

; 809  : 
; 810  : 		return sTemp;

	lea	eax, DWORD PTR _sTemp$75412[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T76584[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76584[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sTemp$75412[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L75395
$L75411:

; 812  : 
; 813  : 	return sFormat;

	push	OFFSET FLAT:_?sFormat@?1??GetTimeFormatW@Misc@@YA?AVCString@@H@Z@4V3@A
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76584[ebp]
	or	edx, 1
	mov	DWORD PTR $T76584[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L75395:

; 814  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 84					; 00000054H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76576:
	lea	ecx, DWORD PTR _sTemp$75412[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76577:
	lea	ecx, DWORD PTR $T76570[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76578:
	lea	ecx, DWORD PTR $T76571[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76579:
	lea	ecx, DWORD PTR $T76572[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76580:
	lea	ecx, DWORD PTR $T76573[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76581:
	lea	ecx, DWORD PTR $T76574[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76582:
	lea	ecx, DWORD PTR $T76575[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76588:
	mov	eax, OFFSET FLAT:$T76587
	jmp	___CxxFrameHandler
text$x	ENDS
?GetTimeFormatW@Misc@@YA?AVCString@@H@Z ENDP		; Misc::GetTimeFormatW
;	COMDAT _$E287
_TEXT	SEGMENT
_$E287	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?sFormat@?1??GetTimeFormatW@Misc@@YA?AVCString@@H@Z@4V3@A
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E287	ENDP
_TEXT	ENDS
EXTRN	??4CString@@QAEABV0@PBD@Z:NEAR			; CString::operator=
_BSS	SEGMENT
	ALIGN	4

_?sSep@?1??GetTimeSeparator@Misc@@YA?AVCString@@XZ@4V3@A DD 01H DUP (?)
_?$S288@?1??GetTimeSeparator@Misc@@YA?AVCString@@XZ@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75445 DB	':', 00H
data	ENDS
;	COMDAT ?GetTimeSeparator@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_BUFLEN$ = -4
$T76598 = -8
?GetTimeSeparator@Misc@@YA?AVCString@@XZ PROC NEAR	; Misc::GetTimeSeparator, COMDAT

; 817  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T76598[ebp], 0

; 818  : 	static CString sSep;

	xor	eax, eax
	mov	al, BYTE PTR _?$S288@?1??GetTimeSeparator@Misc@@YA?AVCString@@XZ@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75433
	mov	cl, BYTE PTR _?$S288@?1??GetTimeSeparator@Misc@@YA?AVCString@@XZ@4EA
	or	cl, 1
	mov	BYTE PTR _?$S288@?1??GetTimeSeparator@Misc@@YA?AVCString@@XZ@4EA, cl
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetTimeSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	??0CString@@QAE@XZ			; CString::CString
	push	OFFSET FLAT:_$E289
	call	_atexit
	add	esp, 4
$L75433:

; 819  : 	const int BUFLEN = 10;

	mov	DWORD PTR _BUFLEN$[ebp], 10		; 0000000aH

; 820  : 
; 821  : 	if (sSep.IsEmpty()) // init first time only

	mov	ecx, OFFSET FLAT:_?sSep@?1??GetTimeSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75444

; 823  : 		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIME , sSep.GetBuffer(BUFLEN), BUFLEN - 1);

	mov	esi, esp
	push	9
	push	10					; 0000000aH
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetTimeSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	push	30					; 0000001eH
	push	1024					; 00000400H
	call	DWORD PTR __imp__GetLocaleInfoW@16
	cmp	esi, esp
	call	__chkesp

; 824  : 		sSep.ReleaseBuffer();

	push	-1
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetTimeSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 825  : 
; 826  : 		// Trim extra spaces
; 827  : 		Trim(sSep);

	push	0
	push	OFFSET FLAT:_?sSep@?1??GetTimeSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z	; Misc::Trim
	add	esp, 8

; 828  : 		
; 829  : 		// If none found, use a dot
; 830  : 		if (!sSep.GetLength())

	mov	ecx, OFFSET FLAT:_?sSep@?1??GetTimeSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	jne	SHORT $L75444

; 831  : 			sSep = ":";

	push	OFFSET FLAT:$SG75445
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetTimeSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	??4CString@@QAEABV0@PBD@Z		; CString::operator=
$L75444:

; 833  : 
; 834  : 	return sSep;

	push	OFFSET FLAT:_?sSep@?1??GetTimeSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76598[ebp]
	or	edx, 1
	mov	DWORD PTR $T76598[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 835  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetTimeSeparator@Misc@@YA?AVCString@@XZ ENDP		; Misc::GetTimeSeparator
_TEXT	ENDS
;	COMDAT _$E289
_TEXT	SEGMENT
_$E289	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetTimeSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E289	ENDP
_TEXT	ENDS
PUBLIC	?GetDateSeparator@Misc@@YA?AVCString@@XZ	; Misc::GetDateSeparator
_BSS	SEGMENT
	ALIGN	4

_?sSep@?1??GetDateSeparator@Misc@@YA?AVCString@@XZ@4V3@A DD 01H DUP (?)
_?$S290@?1??GetDateSeparator@Misc@@YA?AVCString@@XZ@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75466 DB	'/', 00H
data	ENDS
;	COMDAT ?GetDateSeparator@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_BUFLEN$ = -4
$T76606 = -8
?GetDateSeparator@Misc@@YA?AVCString@@XZ PROC NEAR	; Misc::GetDateSeparator, COMDAT

; 838  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T76606[ebp], 0

; 839  : 	static CString sSep;

	xor	eax, eax
	mov	al, BYTE PTR _?$S290@?1??GetDateSeparator@Misc@@YA?AVCString@@XZ@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75454
	mov	cl, BYTE PTR _?$S290@?1??GetDateSeparator@Misc@@YA?AVCString@@XZ@4EA
	or	cl, 1
	mov	BYTE PTR _?$S290@?1??GetDateSeparator@Misc@@YA?AVCString@@XZ@4EA, cl
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetDateSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	??0CString@@QAE@XZ			; CString::CString
	push	OFFSET FLAT:_$E291
	call	_atexit
	add	esp, 4
$L75454:

; 840  : 	const int BUFLEN = 10;

	mov	DWORD PTR _BUFLEN$[ebp], 10		; 0000000aH

; 841  : 
; 842  : 	if (sSep.IsEmpty()) // init first time only

	mov	ecx, OFFSET FLAT:_?sSep@?1??GetDateSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75465

; 844  : 		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDATE , sSep.GetBuffer(BUFLEN), BUFLEN - 1);

	mov	esi, esp
	push	9
	push	10					; 0000000aH
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetDateSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	push	29					; 0000001dH
	push	1024					; 00000400H
	call	DWORD PTR __imp__GetLocaleInfoW@16
	cmp	esi, esp
	call	__chkesp

; 845  : 		sSep.ReleaseBuffer();

	push	-1
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetDateSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 846  : 
; 847  : 		// Trim extra spaces
; 848  : 		Trim(sSep);

	push	0
	push	OFFSET FLAT:_?sSep@?1??GetDateSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z	; Misc::Trim
	add	esp, 8

; 849  : 		
; 850  : 		// If none found, use a slash
; 851  : 		if (!sSep.GetLength())

	mov	ecx, OFFSET FLAT:_?sSep@?1??GetDateSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	jne	SHORT $L75465

; 852  : 			sSep = "/";

	push	OFFSET FLAT:$SG75466
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetDateSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	??4CString@@QAEABV0@PBD@Z		; CString::operator=
$L75465:

; 854  : 
; 855  : 	return sSep;

	push	OFFSET FLAT:_?sSep@?1??GetDateSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76606[ebp]
	or	edx, 1
	mov	DWORD PTR $T76606[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 856  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetDateSeparator@Misc@@YA?AVCString@@XZ ENDP		; Misc::GetDateSeparator
_TEXT	ENDS
;	COMDAT _$E291
_TEXT	SEGMENT
_$E291	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetDateSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E291	ENDP
_TEXT	ENDS
PUBLIC	?GetShortDateFormat@Misc@@YA?AVCString@@H@Z	; Misc::GetShortDateFormat
_BSS	SEGMENT
	ALIGN	4

_?sFormat@?1??GetShortDateFormat@Misc@@YA?AVCString@@H@Z@4V3@A DD 01H DUP (?)
_?$S292@?1??GetShortDateFormat@Misc@@YA?AVCString@@H@Z@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75491 DB	'ddd ', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76620	DD	019930520H
	DD	03H
	DD	FLAT:$T76622
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76622	DD	0ffffffffH
	DD	FLAT:$L76614
	DD	00H
	DD	FLAT:$L76615
	DD	0ffffffffH
	DD	FLAT:$L76615
xdata$x	ENDS
;	COMDAT ?GetShortDateFormat@Misc@@YA?AVCString@@H@Z
_TEXT	SEGMENT
_bIncDOW$ = 12
___$ReturnUdt$ = 8
_BUFLEN$ = -16
_sTemp$75488 = -20
$T76613 = -24
$T76617 = -28
__$EHRec$ = -12
?GetShortDateFormat@Misc@@YA?AVCString@@H@Z PROC NEAR	; Misc::GetShortDateFormat, COMDAT

; 859  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76621
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR $T76617[ebp], 0

; 860  : 	static CString sFormat;

	xor	eax, eax
	mov	al, BYTE PTR _?$S292@?1??GetShortDateFormat@Misc@@YA?AVCString@@H@Z@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75476
	mov	cl, BYTE PTR _?$S292@?1??GetShortDateFormat@Misc@@YA?AVCString@@H@Z@4EA
	or	cl, 1
	mov	BYTE PTR _?$S292@?1??GetShortDateFormat@Misc@@YA?AVCString@@H@Z@4EA, cl
	mov	ecx, OFFSET FLAT:_?sFormat@?1??GetShortDateFormat@Misc@@YA?AVCString@@H@Z@4V3@A
	call	??0CString@@QAE@XZ			; CString::CString
	push	OFFSET FLAT:_$E293
	call	_atexit
	add	esp, 4
$L75476:

; 861  : 	const int BUFLEN = 100;

	mov	DWORD PTR _BUFLEN$[ebp], 100		; 00000064H

; 862  : 
; 863  : 	if (sFormat.IsEmpty()) // init first time only

	mov	ecx, OFFSET FLAT:_?sFormat@?1??GetShortDateFormat@Misc@@YA?AVCString@@H@Z@4V3@A
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75479

; 865  : 		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SSHORTDATE, sFormat.GetBuffer(BUFLEN), BUFLEN - 1);

	mov	esi, esp
	push	99					; 00000063H
	push	100					; 00000064H
	mov	ecx, OFFSET FLAT:_?sFormat@?1??GetShortDateFormat@Misc@@YA?AVCString@@H@Z@4V3@A
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	push	31					; 0000001fH
	push	1024					; 00000400H
	call	DWORD PTR __imp__GetLocaleInfoW@16
	cmp	esi, esp
	call	__chkesp

; 866  : 		sFormat.ReleaseBuffer();

	push	-1
	mov	ecx, OFFSET FLAT:_?sFormat@?1??GetShortDateFormat@Misc@@YA?AVCString@@H@Z@4V3@A
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer
$L75479:

; 868  : 
; 869  : 	if (bIncDOW)

	cmp	DWORD PTR _bIncDOW$[ebp], 0
	je	SHORT $L75487

; 871  : 		CString sTemp = "ddd "+ sFormat;

	push	OFFSET FLAT:$SG75491
	lea	ecx, DWORD PTR $T76613[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	push	OFFSET FLAT:_?sFormat@?1??GetShortDateFormat@Misc@@YA?AVCString@@H@Z@4V3@A
	lea	edx, DWORD PTR $T76613[ebp]
	push	edx
	lea	eax, DWORD PTR _sTemp$75488[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T76613[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 872  : 		return sTemp;

	lea	ecx, DWORD PTR _sTemp$75488[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76617[ebp]
	or	edx, 1
	mov	DWORD PTR $T76617[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sTemp$75488[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L75471
$L75487:

; 874  : 
; 875  : 	return sFormat;

	push	OFFSET FLAT:_?sFormat@?1??GetShortDateFormat@Misc@@YA?AVCString@@H@Z@4V3@A
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T76617[ebp]
	or	al, 1
	mov	DWORD PTR $T76617[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L75471:

; 876  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76614:
	lea	ecx, DWORD PTR $T76613[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76615:
	lea	ecx, DWORD PTR _sTemp$75488[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76621:
	mov	eax, OFFSET FLAT:$T76620
	jmp	___CxxFrameHandler
text$x	ENDS
?GetShortDateFormat@Misc@@YA?AVCString@@H@Z ENDP	; Misc::GetShortDateFormat
;	COMDAT _$E293
_TEXT	SEGMENT
_$E293	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?sFormat@?1??GetShortDateFormat@Misc@@YA?AVCString@@H@Z@4V3@A
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E293	ENDP
_TEXT	ENDS
PUBLIC	?GetDecimalSeparator@Misc@@YA?AVCString@@XZ	; Misc::GetDecimalSeparator
_BSS	SEGMENT
	ALIGN	4

_?sSep@?1??GetDecimalSeparator@Misc@@YA?AVCString@@XZ@4V3@A DD 01H DUP (?)
_?$S294@?1??GetDecimalSeparator@Misc@@YA?AVCString@@XZ@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75512 DB	'.', 00H
data	ENDS
;	COMDAT ?GetDecimalSeparator@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_BUFLEN$ = -4
$T76631 = -8
?GetDecimalSeparator@Misc@@YA?AVCString@@XZ PROC NEAR	; Misc::GetDecimalSeparator, COMDAT

; 879  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T76631[ebp], 0

; 880  : 	static CString sSep;

	xor	eax, eax
	mov	al, BYTE PTR _?$S294@?1??GetDecimalSeparator@Misc@@YA?AVCString@@XZ@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75500
	mov	cl, BYTE PTR _?$S294@?1??GetDecimalSeparator@Misc@@YA?AVCString@@XZ@4EA
	or	cl, 1
	mov	BYTE PTR _?$S294@?1??GetDecimalSeparator@Misc@@YA?AVCString@@XZ@4EA, cl
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetDecimalSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	??0CString@@QAE@XZ			; CString::CString
	push	OFFSET FLAT:_$E295
	call	_atexit
	add	esp, 4
$L75500:

; 881  : 	const int BUFLEN = 10;

	mov	DWORD PTR _BUFLEN$[ebp], 10		; 0000000aH

; 882  : 
; 883  : 	if (sSep.IsEmpty()) // init first time only

	mov	ecx, OFFSET FLAT:_?sSep@?1??GetDecimalSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75511

; 885  : 		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, sSep.GetBuffer(BUFLEN), BUFLEN - 1);

	mov	esi, esp
	push	9
	push	10					; 0000000aH
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetDecimalSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	push	14					; 0000000eH
	push	1024					; 00000400H
	call	DWORD PTR __imp__GetLocaleInfoW@16
	cmp	esi, esp
	call	__chkesp

; 886  : 		sSep.ReleaseBuffer();

	push	-1
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetDecimalSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 887  : 
; 888  : 		// Trim extra spaces
; 889  : 		Trim(sSep);

	push	0
	push	OFFSET FLAT:_?sSep@?1??GetDecimalSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z	; Misc::Trim
	add	esp, 8

; 890  : 		
; 891  : 		// If none found, use a dot
; 892  : 		if (!sSep.GetLength())

	mov	ecx, OFFSET FLAT:_?sSep@?1??GetDecimalSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	jne	SHORT $L75511

; 893  : 			sSep = ".";

	push	OFFSET FLAT:$SG75512
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetDecimalSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	??4CString@@QAEABV0@PBD@Z		; CString::operator=
$L75511:

; 895  : 
; 896  : 	return sSep;

	push	OFFSET FLAT:_?sSep@?1??GetDecimalSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76631[ebp]
	or	edx, 1
	mov	DWORD PTR $T76631[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 897  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetDecimalSeparator@Misc@@YA?AVCString@@XZ ENDP	; Misc::GetDecimalSeparator
_TEXT	ENDS
;	COMDAT _$E295
_TEXT	SEGMENT
_$E295	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?sSep@?1??GetDecimalSeparator@Misc@@YA?AVCString@@XZ@4V3@A
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E295	ENDP
_TEXT	ENDS
PUBLIC	?IsMetricMeasurementSystem@Misc@@YAHXZ		; Misc::IsMetricMeasurementSystem
EXTRN	__imp___wtoi:NEAR
;	COMDAT xdata$x
; File D:\_CODE\Shared\Misc.cpp
xdata$x	SEGMENT
$T76641	DD	019930520H
	DD	01H
	DD	FLAT:$T76643
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76643	DD	0ffffffffH
	DD	FLAT:$L76639
xdata$x	ENDS
;	COMDAT ?IsMetricMeasurementSystem@Misc@@YAHXZ
_TEXT	SEGMENT
_BUFLEN$ = -16
_sSystem$ = -20
$T76638 = -24
__$EHRec$ = -12
?IsMetricMeasurementSystem@Misc@@YAHXZ PROC NEAR	; Misc::IsMetricMeasurementSystem, COMDAT

; 900  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76642
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH

; 901  : 	const int BUFLEN = 2;

	mov	DWORD PTR _BUFLEN$[ebp], 2

; 902  : 	CString sSystem;

	lea	ecx, DWORD PTR _sSystem$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 903  : 	
; 904  : 	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IMEASURE, sSystem.GetBuffer(BUFLEN), BUFLEN - 1);

	mov	esi, esp
	push	1
	push	2
	lea	ecx, DWORD PTR _sSystem$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	push	13					; 0000000dH
	push	1024					; 00000400H
	call	DWORD PTR __imp__GetLocaleInfoW@16
	cmp	esi, esp
	call	__chkesp

; 905  : 	sSystem.ReleaseBuffer(BUFLEN - 1);

	push	1
	lea	ecx, DWORD PTR _sSystem$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 906  : 	
; 907  : 	return (_ttoi(sSystem) == 0);

	lea	ecx, DWORD PTR _sSystem$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp___wtoi
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR $T76638[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sSystem$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76638[ebp]

; 908  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76639:
	lea	ecx, DWORD PTR _sSystem$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76642:
	mov	eax, OFFSET FLAT:$T76641
	jmp	___CxxFrameHandler
text$x	ENDS
?IsMetricMeasurementSystem@Misc@@YAHXZ ENDP		; Misc::IsMetricMeasurementSystem
PUBLIC	?WideToMultiByte@Misc@@YAPADPBGAAHI@Z		; Misc::WideToMultiByte
PUBLIC	?EncodeAsMultiByte@Misc@@YAAAVCString@@AAV2@I@Z	; Misc::EncodeAsMultiByte
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ?EncodeAsMultiByte@Misc@@YAAAVCString@@AAV2@I@Z
_TEXT	SEGMENT
_sUnicode$ = 8
_nCodePage$ = 12
_nLen$ = -16
_szMB$ = -4
_nBytes$ = -12
_szOutput$ = -8
$T76648 = -20
?EncodeAsMultiByte@Misc@@YAAAVCString@@AAV2@I@Z PROC NEAR ; Misc::EncodeAsMultiByte, COMDAT

; 911  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 20					; 00000014H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 912  : 	// calc number of unicode chars in string
; 913  : #ifdef _UNICODE
; 914  : 	int nLen = sUnicode.GetLength();

	mov	ecx, DWORD PTR _sUnicode$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR _nLen$[ebp], eax

; 915  : #else
; 916  : 	int nLen = ((sUnicode.GetLength() + 1) / sizeof(WCHAR));
; 917  : #endif
; 918  : 	
; 919  : 	char* szMB = WideToMultiByte((LPCWSTR)(LPCTSTR)sUnicode, nLen, nCodePage);

	mov	eax, DWORD PTR _nCodePage$[ebp]
	push	eax
	lea	ecx, DWORD PTR _nLen$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sUnicode$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?WideToMultiByte@Misc@@YAPADPBGAAHI@Z	; Misc::WideToMultiByte
	add	esp, 12					; 0000000cH
	mov	DWORD PTR _szMB$[ebp], eax

; 920  : 	// nLen is now the number of returned multibyte chars
; 921  : 	
; 922  : 	// calc number of bytes required to hold multibyte chars
; 923  : 	int nBytes = nLen;

	mov	edx, DWORD PTR _nLen$[ebp]
	mov	DWORD PTR _nBytes$[ebp], edx

; 924  : 	
; 925  : #ifdef _UNICODE
; 926  : 	// calc number of chars required to hold multibyte chars
; 927  : 	nLen = ((nLen + 1) / sizeof(WCHAR));

	mov	eax, DWORD PTR _nLen$[ebp]
	add	eax, 1
	shr	eax, 1
	mov	DWORD PTR _nLen$[ebp], eax

; 928  : #endif
; 929  : 	
; 930  : 	LPTSTR szOutput = sUnicode.GetBuffer(nLen);

	mov	ecx, DWORD PTR _nLen$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sUnicode$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	mov	DWORD PTR _szOutput$[ebp], eax

; 931  : 	
; 932  : 	ZeroMemory(szOutput, nBytes);

	mov	edx, DWORD PTR _nBytes$[ebp]
	push	edx
	push	0
	mov	eax, DWORD PTR _szOutput$[ebp]
	push	eax
	call	_memset
	add	esp, 12					; 0000000cH

; 933  : 	CopyMemory(szOutput, szMB, nBytes);

	mov	ecx, DWORD PTR _nBytes$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szMB$[ebp]
	push	edx
	mov	eax, DWORD PTR _szOutput$[ebp]
	push	eax
	call	_memcpy
	add	esp, 12					; 0000000cH

; 934  : 	
; 935  : 	sUnicode.ReleaseBuffer(nLen);

	mov	ecx, DWORD PTR _nLen$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sUnicode$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 936  : 	delete [] szMB;

	mov	edx, DWORD PTR _szMB$[ebp]
	mov	DWORD PTR $T76648[ebp], edx
	mov	eax, DWORD PTR $T76648[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 937  : 	
; 938  : 	return sUnicode;

	mov	eax, DWORD PTR _sUnicode$[ebp]

; 939  : }

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?EncodeAsMultiByte@Misc@@YAAAVCString@@AAV2@I@Z ENDP	; Misc::EncodeAsMultiByte
_TEXT	ENDS
PUBLIC	?MultiByteToWide@Misc@@YAPAGPBDAAHI@Z		; Misc::MultiByteToWide
PUBLIC	?EncodeAsUnicode@Misc@@YAAAVCString@@AAV2@I@Z	; Misc::EncodeAsUnicode
;	COMDAT ?EncodeAsUnicode@Misc@@YAAAVCString@@AAV2@I@Z
_TEXT	SEGMENT
_sMultibyte$ = 8
_nCodePage$ = 12
_nLen$ = -16
_szUnicode$ = -12
_nBytes$ = -8
_szOutput$ = -4
$T76651 = -20
?EncodeAsUnicode@Misc@@YAAAVCString@@AAV2@I@Z PROC NEAR	; Misc::EncodeAsUnicode, COMDAT

; 942  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 20					; 00000014H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 943  : 	// calc number of multibyte chars in string
; 944  : 	int nLen = (sMultibyte.GetLength() * sizeof(TCHAR));

	mov	ecx, DWORD PTR _sMultibyte$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	shl	eax, 1
	mov	DWORD PTR _nLen$[ebp], eax

; 945  : 	
; 946  : 	WCHAR* szUnicode = MultiByteToWide((LPCSTR)(LPCTSTR)sMultibyte, nLen, nCodePage);

	mov	eax, DWORD PTR _nCodePage$[ebp]
	push	eax
	lea	ecx, DWORD PTR _nLen$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sMultibyte$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?MultiByteToWide@Misc@@YAPAGPBDAAHI@Z	; Misc::MultiByteToWide
	add	esp, 12					; 0000000cH
	mov	DWORD PTR _szUnicode$[ebp], eax

; 947  : 	// nLen is now the number of returned unicode chars
; 948  : 	
; 949  : 	// calc number of bytes required to hold unicode chars
; 950  : 	int nBytes = (nLen * sizeof(WCHAR));

	mov	edx, DWORD PTR _nLen$[ebp]
	shl	edx, 1
	mov	DWORD PTR _nBytes$[ebp], edx

; 951  : 	
; 952  : 	// calc number of chars needed to hold unicode chars
; 953  : #ifndef _UNICODE
; 954  : 	nLen *= sizeof(WCHAR);
; 955  : #endif
; 956  : 	
; 957  : 	LPTSTR szOutput = sMultibyte.GetBuffer(nLen);

	mov	eax, DWORD PTR _nLen$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sMultibyte$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	mov	DWORD PTR _szOutput$[ebp], eax

; 958  : 	
; 959  : 	ZeroMemory(szOutput, nBytes); 

	mov	ecx, DWORD PTR _nBytes$[ebp]
	push	ecx
	push	0
	mov	edx, DWORD PTR _szOutput$[ebp]
	push	edx
	call	_memset
	add	esp, 12					; 0000000cH

; 960  : 	CopyMemory(szOutput, szUnicode, nBytes);

	mov	eax, DWORD PTR _nBytes$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szUnicode$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szOutput$[ebp]
	push	edx
	call	_memcpy
	add	esp, 12					; 0000000cH

; 961  : 	
; 962  : 	sMultibyte.ReleaseBuffer(nLen);

	mov	eax, DWORD PTR _nLen$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sMultibyte$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 963  : 	delete [] szUnicode;

	mov	ecx, DWORD PTR _szUnicode$[ebp]
	mov	DWORD PTR $T76651[ebp], ecx
	mov	edx, DWORD PTR $T76651[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 964  : 	
; 965  : 	return sMultibyte;

	mov	eax, DWORD PTR _sMultibyte$[ebp]

; 966  : }

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?EncodeAsUnicode@Misc@@YAAAVCString@@AAV2@I@Z ENDP	; Misc::EncodeAsUnicode
_TEXT	ENDS
PUBLIC	?WideToMultiByte@Misc@@YAPADPBGI@Z		; Misc::WideToMultiByte
EXTRN	__imp__wcslen:NEAR
;	COMDAT ?WideToMultiByte@Misc@@YAPADPBGI@Z
_TEXT	SEGMENT
_szFrom$ = 8
_nCodePage$ = 12
_nLen$ = -4
?WideToMultiByte@Misc@@YAPADPBGI@Z PROC NEAR		; Misc::WideToMultiByte, COMDAT

; 969  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 970  : 	int nLen = wcslen(szFrom);

	mov	esi, esp
	mov	eax, DWORD PTR _szFrom$[ebp]
	push	eax
	call	DWORD PTR __imp__wcslen
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nLen$[ebp], eax

; 971  : 	return WideToMultiByte(szFrom, nLen, nCodePage);

	mov	ecx, DWORD PTR _nCodePage$[ebp]
	push	ecx
	lea	edx, DWORD PTR _nLen$[ebp]
	push	edx
	mov	eax, DWORD PTR _szFrom$[ebp]
	push	eax
	call	?WideToMultiByte@Misc@@YAPADPBGAAHI@Z	; Misc::WideToMultiByte
	add	esp, 12					; 0000000cH

; 972  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?WideToMultiByte@Misc@@YAPADPBGI@Z ENDP			; Misc::WideToMultiByte
_TEXT	ENDS
EXTRN	__imp__WideCharToMultiByte@32:NEAR
EXTRN	??2@YAPAXIPBDH@Z:NEAR				; operator new
;	COMDAT ?WideToMultiByte@Misc@@YAPADPBGAAHI@Z
_TEXT	SEGMENT
_szFrom$ = 8
_nLength$ = 12
_nCodePage$ = 16
_clen$ = -8
_cTo$ = -4
$T76656 = -12
?WideToMultiByte@Misc@@YAPADPBGAAHI@Z PROC NEAR		; Misc::WideToMultiByte, COMDAT

; 975  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 976  : 	int clen = WideCharToMultiByte(nCodePage, 0, szFrom, nLength, NULL, 0, NULL, NULL);

	mov	esi, esp
	push	0
	push	0
	push	0
	push	0
	mov	eax, DWORD PTR _nLength$[ebp]
	mov	ecx, DWORD PTR [eax]
	push	ecx
	mov	edx, DWORD PTR _szFrom$[ebp]
	push	edx
	push	0
	mov	eax, DWORD PTR _nCodePage$[ebp]
	push	eax
	call	DWORD PTR __imp__WideCharToMultiByte@32
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _clen$[ebp], eax
$L75562:

; 977  : 
; 978  : 	// if clen == 0, some unknown codepage was probably used.
; 979  : 	ASSERT((nLength && clen) || (!nLength && !clen));

	mov	ecx, DWORD PTR _nLength$[ebp]
	cmp	DWORD PTR [ecx], 0
	je	SHORT $L75566
	cmp	DWORD PTR _clen$[ebp], 0
	jne	SHORT $L75565
$L75566:
	mov	edx, DWORD PTR _nLength$[ebp]
	cmp	DWORD PTR [edx], 0
	jne	SHORT $L75567
	cmp	DWORD PTR _clen$[ebp], 0
	je	SHORT $L75565
$L75567:
	push	979					; 000003d3H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75565
	int	3
$L75565:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75562

; 980  : 
; 981  : 	if (clen == 0 && nLength) 

	cmp	DWORD PTR _clen$[ebp], 0
	jne	SHORT $L75568
	mov	ecx, DWORD PTR _nLength$[ebp]
	cmp	DWORD PTR [ecx], 0
	je	SHORT $L75568

; 982  : 		return NULL;

	xor	eax, eax
	jmp	SHORT $L75560
$L75568:

; 983  : 
; 984  : 	char* cTo = new char[clen+1];

	push	984					; 000003d8H
	push	OFFSET FLAT:_THIS_FILE
	mov	edx, DWORD PTR _clen$[ebp]
	add	edx, 1
	push	edx
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T76656[ebp], eax
	mov	eax, DWORD PTR $T76656[ebp]
	mov	DWORD PTR _cTo$[ebp], eax

; 985  : 	cTo[clen] = 0; // null terminator

	mov	ecx, DWORD PTR _cTo$[ebp]
	add	ecx, DWORD PTR _clen$[ebp]
	mov	BYTE PTR [ecx], 0

; 986  : 
; 987  : 	if (clen)

	cmp	DWORD PTR _clen$[ebp], 0
	je	SHORT $L75572

; 988  : 		WideCharToMultiByte(nCodePage, 0, szFrom, nLength, cTo, clen, NULL, NULL);

	mov	esi, esp
	push	0
	push	0
	mov	edx, DWORD PTR _clen$[ebp]
	push	edx
	mov	eax, DWORD PTR _cTo$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nLength$[ebp]
	mov	edx, DWORD PTR [ecx]
	push	edx
	mov	eax, DWORD PTR _szFrom$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _nCodePage$[ebp]
	push	ecx
	call	DWORD PTR __imp__WideCharToMultiByte@32
	cmp	esi, esp
	call	__chkesp
$L75572:

; 989  : 
; 990  : 	nLength = clen;

	mov	edx, DWORD PTR _nLength$[ebp]
	mov	eax, DWORD PTR _clen$[ebp]
	mov	DWORD PTR [edx], eax

; 991  : 
; 992  : 	return cTo;

	mov	eax, DWORD PTR _cTo$[ebp]
$L75560:

; 993  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?WideToMultiByte@Misc@@YAPADPBGAAHI@Z ENDP		; Misc::WideToMultiByte
_TEXT	ENDS
PUBLIC	?MultiByteToWide@Misc@@YAPAGPBDI@Z		; Misc::MultiByteToWide
EXTRN	_strlen:NEAR
;	COMDAT ?MultiByteToWide@Misc@@YAPAGPBDI@Z
_TEXT	SEGMENT
_szFrom$ = 8
_nCodePage$ = 12
_nLen$ = -4
?MultiByteToWide@Misc@@YAPAGPBDI@Z PROC NEAR		; Misc::MultiByteToWide, COMDAT

; 996  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 997  : 	int nLen = strlen(szFrom);

	mov	eax, DWORD PTR _szFrom$[ebp]
	push	eax
	call	_strlen
	add	esp, 4
	mov	DWORD PTR _nLen$[ebp], eax

; 998  : 	return MultiByteToWide(szFrom, nLen, nCodePage);

	mov	ecx, DWORD PTR _nCodePage$[ebp]
	push	ecx
	lea	edx, DWORD PTR _nLen$[ebp]
	push	edx
	mov	eax, DWORD PTR _szFrom$[ebp]
	push	eax
	call	?MultiByteToWide@Misc@@YAPAGPBDAAHI@Z	; Misc::MultiByteToWide
	add	esp, 12					; 0000000cH

; 999  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?MultiByteToWide@Misc@@YAPAGPBDI@Z ENDP			; Misc::MultiByteToWide
_TEXT	ENDS
EXTRN	__imp__MultiByteToWideChar@24:NEAR
;	COMDAT ?MultiByteToWide@Misc@@YAPAGPBDAAHI@Z
_TEXT	SEGMENT
_szFrom$ = 8
_nLength$ = 12
_nCodepage$ = 16
_wlen$ = -8
_wTo$ = -4
$T76661 = -12
?MultiByteToWide@Misc@@YAPAGPBDAAHI@Z PROC NEAR		; Misc::MultiByteToWide, COMDAT

; 1002 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1003 : 	// Use api convert routine
; 1004 : 	int wlen = MultiByteToWideChar(nCodepage, 0, szFrom, nLength, NULL, 0);

	mov	esi, esp
	push	0
	push	0
	mov	eax, DWORD PTR _nLength$[ebp]
	mov	ecx, DWORD PTR [eax]
	push	ecx
	mov	edx, DWORD PTR _szFrom$[ebp]
	push	edx
	push	0
	mov	eax, DWORD PTR _nCodepage$[ebp]
	push	eax
	call	DWORD PTR __imp__MultiByteToWideChar@24
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _wlen$[ebp], eax
$L75584:

; 1005 : 
; 1006 : 	// if wlen == 0, some unknown codepage was probably used.
; 1007 : 	ASSERT((nLength && wlen) || (!nLength && !wlen));

	mov	ecx, DWORD PTR _nLength$[ebp]
	cmp	DWORD PTR [ecx], 0
	je	SHORT $L75588
	cmp	DWORD PTR _wlen$[ebp], 0
	jne	SHORT $L75587
$L75588:
	mov	edx, DWORD PTR _nLength$[ebp]
	cmp	DWORD PTR [edx], 0
	jne	SHORT $L75589
	cmp	DWORD PTR _wlen$[ebp], 0
	je	SHORT $L75587
$L75589:
	push	1007					; 000003efH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75587
	int	3
$L75587:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75584

; 1008 : 
; 1009 : 	if (wlen == 0 && nLength) 

	cmp	DWORD PTR _wlen$[ebp], 0
	jne	SHORT $L75590
	mov	ecx, DWORD PTR _nLength$[ebp]
	cmp	DWORD PTR [ecx], 0
	je	SHORT $L75590

; 1010 : 		return NULL;

	xor	eax, eax
	jmp	SHORT $L75582
$L75590:

; 1011 : 
; 1012 : 	WCHAR* wTo = new WCHAR[wlen+1];

	push	1012					; 000003f4H
	push	OFFSET FLAT:_THIS_FILE
	mov	edx, DWORD PTR _wlen$[ebp]
	lea	eax, DWORD PTR [edx+edx+2]
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T76661[ebp], eax
	mov	ecx, DWORD PTR $T76661[ebp]
	mov	DWORD PTR _wTo$[ebp], ecx

; 1013 : 	wTo[wlen] = 0; // null terminator

	mov	edx, DWORD PTR _wlen$[ebp]
	mov	eax, DWORD PTR _wTo$[ebp]
	mov	WORD PTR [eax+edx*2], 0

; 1014 : 
; 1015 : 	if (wlen)

	cmp	DWORD PTR _wlen$[ebp], 0
	je	SHORT $L75594

; 1016 : 		MultiByteToWideChar(nCodepage, 0, szFrom, nLength, wTo, wlen);

	mov	esi, esp
	mov	ecx, DWORD PTR _wlen$[ebp]
	push	ecx
	mov	edx, DWORD PTR _wTo$[ebp]
	push	edx
	mov	eax, DWORD PTR _nLength$[ebp]
	mov	ecx, DWORD PTR [eax]
	push	ecx
	mov	edx, DWORD PTR _szFrom$[ebp]
	push	edx
	push	0
	mov	eax, DWORD PTR _nCodepage$[ebp]
	push	eax
	call	DWORD PTR __imp__MultiByteToWideChar@24
	cmp	esi, esp
	call	__chkesp
$L75594:

; 1017 : 
; 1018 : 	nLength = wlen;

	mov	ecx, DWORD PTR _nLength$[ebp]
	mov	edx, DWORD PTR _wlen$[ebp]
	mov	DWORD PTR [ecx], edx

; 1019 : 
; 1020 : 	return wTo;

	mov	eax, DWORD PTR _wTo$[ebp]
$L75582:

; 1021 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?MultiByteToWide@Misc@@YAPAGPBDAAHI@Z ENDP		; Misc::MultiByteToWide
_TEXT	ENDS
PUBLIC	?Round@Misc@@YANN@Z				; Misc::Round
PUBLIC	__real@8@00000000000000000000
PUBLIC	__real@8@3ffe8000000000000000
PUBLIC	__real@8@bffe8000000000000000
EXTRN	__imp__ceil:NEAR
EXTRN	__imp__floor:NEAR
EXTRN	__ftol:NEAR
EXTRN	__fltused:NEAR
;	COMDAT __real@8@00000000000000000000
; File D:\_CODE\Shared\Misc.cpp
CONST	SEGMENT
__real@8@00000000000000000000 DQ 00000000000000000r ; 0
CONST	ENDS
;	COMDAT __real@8@3ffe8000000000000000
CONST	SEGMENT
__real@8@3ffe8000000000000000 DQ 03fe0000000000000r ; 0.5
CONST	ENDS
;	COMDAT __real@8@bffe8000000000000000
CONST	SEGMENT
__real@8@bffe8000000000000000 DQ 0bfe0000000000000r ; -0.5
CONST	ENDS
;	COMDAT ?Round@Misc@@YANN@Z
_TEXT	SEGMENT
_dValue$ = 8
?Round@Misc@@YANN@Z PROC NEAR				; Misc::Round, COMDAT

; 1024 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1025 : 	if (dValue > 0)

	fld	QWORD PTR _dValue$[ebp]
	fcomp	QWORD PTR __real@8@00000000000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L75598

; 1027 : 		if ((dValue - (int)dValue) > 0.5)

	fld	QWORD PTR _dValue$[ebp]
	call	__ftol
	mov	DWORD PTR -4+[ebp], eax
	fild	DWORD PTR -4+[ebp]
	fsubr	QWORD PTR _dValue$[ebp]
	fcomp	QWORD PTR __real@8@3ffe8000000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L75600

; 1028 : 			return ceil(dValue);

	mov	esi, esp
	mov	eax, DWORD PTR _dValue$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _dValue$[ebp]
	push	ecx
	call	DWORD PTR __imp__ceil
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75605
$L75600:

; 1030 : 			return floor(dValue);

	mov	esi, esp
	mov	edx, DWORD PTR _dValue$[ebp+4]
	push	edx
	mov	eax, DWORD PTR _dValue$[ebp]
	push	eax
	call	DWORD PTR __imp__floor
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75605
$L75598:

; 1034 : 		if ((dValue - (int)dValue) > -0.5)

	fld	QWORD PTR _dValue$[ebp]
	call	__ftol
	mov	DWORD PTR -8+[ebp], eax
	fild	DWORD PTR -8+[ebp]
	fsubr	QWORD PTR _dValue$[ebp]
	fcomp	QWORD PTR __real@8@bffe8000000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L75604

; 1035 : 			return floor(dValue);

	mov	esi, esp
	mov	ecx, DWORD PTR _dValue$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _dValue$[ebp]
	push	edx
	call	DWORD PTR __imp__floor
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75605
$L75604:

; 1037 : 			return ceil(dValue);

	mov	esi, esp
	mov	eax, DWORD PTR _dValue$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _dValue$[ebp]
	push	ecx
	call	DWORD PTR __imp__ceil
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
$L75605:

; 1039 : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Round@Misc@@YANN@Z ENDP				; Misc::Round
_TEXT	ENDS
PUBLIC	?Round@Misc@@YAMM@Z				; Misc::Round
;	COMDAT ?Round@Misc@@YAMM@Z
_TEXT	SEGMENT
_fValue$ = 8
?Round@Misc@@YAMM@Z PROC NEAR				; Misc::Round, COMDAT

; 1042 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1043 : 	return (float)Round((double)fValue);

	fld	DWORD PTR _fValue$[ebp]
	sub	esp, 8
	fstp	QWORD PTR [esp]
	call	?Round@Misc@@YANN@Z			; Misc::Round
	add	esp, 8
	fst	DWORD PTR -4+[ebp]

; 1044 : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Round@Misc@@YAMM@Z ENDP				; Misc::Round
_TEXT	ENDS
PUBLIC	?IsNumber@Misc@@YAHABVCString@@@Z		; Misc::IsNumber
EXTRN	__imp__iswdigit:NEAR
_BSS	SEGMENT
	ALIGN	4

_?DELIMS@?3??IsNumber@Misc@@YAHABVCString@@@Z@4V3@B DD 01H DUP (?)
_?$S296@?3??IsNumber@Misc@@YAHABVCString@@@Z@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75620 DB	'+', 00H, '-', 00H, '.', 00H, 00H, 00H
data	ENDS
;	COMDAT ?IsNumber@Misc@@YAHABVCString@@@Z
_TEXT	SEGMENT
_sValue$ = 8
_nChar$ = -4
_chr$75626 = -8
?IsNumber@Misc@@YAHABVCString@@@Z PROC NEAR		; Misc::IsNumber, COMDAT

; 1047 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1048 : 	if (sValue.IsEmpty())

	mov	ecx, DWORD PTR _sValue$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75614

; 1049 : 		return FALSE;

	xor	eax, eax
	jmp	$L75613
$L75614:

; 1050 : 
; 1051 : 	static const CString DELIMS(_T("+-."));

	xor	eax, eax
	mov	al, BYTE PTR _?$S296@?3??IsNumber@Misc@@YAHABVCString@@@Z@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75619
	mov	cl, BYTE PTR _?$S296@?3??IsNumber@Misc@@YAHABVCString@@@Z@4EA
	or	cl, 1
	mov	BYTE PTR _?$S296@?3??IsNumber@Misc@@YAHABVCString@@@Z@4EA, cl
	push	OFFSET FLAT:$SG75620
	mov	ecx, OFFSET FLAT:_?DELIMS@?3??IsNumber@Misc@@YAHABVCString@@@Z@4V3@B
	call	??0CString@@QAE@PBG@Z			; CString::CString
	push	OFFSET FLAT:_$E297
	call	_atexit
	add	esp, 4
$L75619:

; 1052 : 
; 1053 : 	for (int nChar = 0; nChar < sValue.GetLength(); nChar++)

	mov	DWORD PTR _nChar$[ebp], 0
	jmp	SHORT $L75623
$L75624:
	mov	edx, DWORD PTR _nChar$[ebp]
	add	edx, 1
	mov	DWORD PTR _nChar$[ebp], edx
$L75623:
	mov	ecx, DWORD PTR _sValue$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	cmp	DWORD PTR _nChar$[ebp], eax
	jge	SHORT $L75625

; 1055 : 		TCHAR chr = sValue[nChar];

	mov	eax, DWORD PTR _nChar$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sValue$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	mov	WORD PTR _chr$75626[ebp], ax

; 1056 : 
; 1057 : 		if (!_istdigit(chr) && DELIMS.Find(chr) == -1)

	mov	esi, esp
	mov	cx, WORD PTR _chr$75626[ebp]
	push	ecx
	call	DWORD PTR __imp__iswdigit
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75627
	mov	dx, WORD PTR _chr$75626[ebp]
	push	edx
	mov	ecx, OFFSET FLAT:_?DELIMS@?3??IsNumber@Misc@@YAHABVCString@@@Z@4V3@B
	call	?Find@CString@@QBEHG@Z			; CString::Find
	cmp	eax, -1
	jne	SHORT $L75627

; 1058 : 			return FALSE;

	xor	eax, eax
	jmp	SHORT $L75613
$L75627:

; 1059 : 	}

	jmp	SHORT $L75624
$L75625:

; 1060 : 
; 1061 : 	return TRUE;

	mov	eax, 1
$L75613:

; 1062 : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsNumber@Misc@@YAHABVCString@@@Z ENDP			; Misc::IsNumber
_TEXT	ENDS
;	COMDAT _$E297
_TEXT	SEGMENT
_$E297	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?DELIMS@?3??IsNumber@Misc@@YAHABVCString@@@Z@4V3@B
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E297	ENDP
_TEXT	ENDS
PUBLIC	?IsSymbol@Misc@@YAHABVCString@@@Z		; Misc::IsSymbol
EXTRN	__imp__iswalnum:NEAR
;	COMDAT ?IsSymbol@Misc@@YAHABVCString@@@Z
_TEXT	SEGMENT
_sValue$ = 8
_nChar$ = -4
_chr$75637 = -8
?IsSymbol@Misc@@YAHABVCString@@@Z PROC NEAR		; Misc::IsSymbol, COMDAT

; 1065 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1066 : 	if (sValue.IsEmpty())

	mov	ecx, DWORD PTR _sValue$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75632

; 1067 : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75631
$L75632:

; 1068 : 
; 1069 : 	for (int nChar = 0; nChar < sValue.GetLength(); nChar++)

	mov	DWORD PTR _nChar$[ebp], 0
	jmp	SHORT $L75634
$L75635:
	mov	eax, DWORD PTR _nChar$[ebp]
	add	eax, 1
	mov	DWORD PTR _nChar$[ebp], eax
$L75634:
	mov	ecx, DWORD PTR _sValue$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	cmp	DWORD PTR _nChar$[ebp], eax
	jge	SHORT $L75636

; 1071 : 		TCHAR chr = sValue[nChar];

	mov	ecx, DWORD PTR _nChar$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sValue$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	mov	WORD PTR _chr$75637[ebp], ax

; 1072 : 
; 1073 : 		if (_istalnum(chr)) // alpha-Numeric

	mov	esi, esp
	mov	dx, WORD PTR _chr$75637[ebp]
	push	edx
	call	DWORD PTR __imp__iswalnum
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L75638

; 1074 : 			return FALSE;

	xor	eax, eax
	jmp	SHORT $L75631
$L75638:

; 1075 : 	}

	jmp	SHORT $L75635
$L75636:

; 1076 : 
; 1077 : 	return TRUE;

	mov	eax, 1
$L75631:

; 1078 : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsSymbol@Misc@@YAHABVCString@@@Z ENDP			; Misc::IsSymbol
_TEXT	ENDS
PUBLIC	?Atof@Misc@@YANABVCString@@@Z			; Misc::Atof
PUBLIC	?GetLongest@Misc@@YAABVCString@@ABV2@0H@Z	; Misc::GetLongest
EXTRN	_fabs:NEAR
;	COMDAT ?GetLongest@Misc@@YAABVCString@@ABV2@0H@Z
_TEXT	SEGMENT
_str1$ = 8
_str2$ = 12
_bAsDouble$ = 16
_dStr1$75645 = -8
_dStr2$75646 = -16
$T76683 = -20
$T76686 = -24
?GetLongest@Misc@@YAABVCString@@ABV2@0H@Z PROC NEAR	; Misc::GetLongest, COMDAT

; 1081 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 32					; 00000020H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 1082 : 	if (bAsDouble)

	cmp	DWORD PTR _bAsDouble$[ebp], 0
	je	SHORT $L75644

; 1084 : 		// check as numbers
; 1085 : 		double dStr1 = fabs(Atof(str1)), dStr2 = fabs(Atof(str2));

	mov	eax, DWORD PTR _str1$[ebp]
	push	eax
	call	?Atof@Misc@@YANABVCString@@@Z		; Misc::Atof
	add	esp, -4					; fffffffcH
	fstp	QWORD PTR [esp]
	call	_fabs
	add	esp, 8
	fstp	QWORD PTR _dStr1$75645[ebp]
	mov	ecx, DWORD PTR _str2$[ebp]
	push	ecx
	call	?Atof@Misc@@YANABVCString@@@Z		; Misc::Atof
	add	esp, -4					; fffffffcH
	fstp	QWORD PTR [esp]
	call	_fabs
	add	esp, 8
	fstp	QWORD PTR _dStr2$75646[ebp]

; 1086 : 
; 1087 : 		return (dStr1 > dStr2) ? str1 : str2;

	fld	QWORD PTR _dStr1$75645[ebp]
	fcomp	QWORD PTR _dStr2$75646[ebp]
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L76684
	mov	edx, DWORD PTR _str1$[ebp]
	mov	DWORD PTR -28+[ebp], edx
	jmp	SHORT $L76685
$L76684:
	mov	eax, DWORD PTR _str2$[ebp]
	mov	DWORD PTR -28+[ebp], eax
$L76685:
	mov	ecx, DWORD PTR -28+[ebp]
	mov	DWORD PTR $T76683[ebp], ecx
	mov	eax, DWORD PTR $T76683[ebp]
	jmp	SHORT $L75643
$L75644:

; 1089 : 
; 1090 : 	// all else
; 1091 : 	return (str1.GetLength() > str2.GetLength()) ? str1 : str2;

	mov	ecx, DWORD PTR _str1$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	esi, eax
	mov	ecx, DWORD PTR _str2$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	cmp	esi, eax
	jle	SHORT $L76687
	mov	edx, DWORD PTR _str1$[ebp]
	mov	DWORD PTR -32+[ebp], edx
	jmp	SHORT $L76688
$L76687:
	mov	eax, DWORD PTR _str2$[ebp]
	mov	DWORD PTR -32+[ebp], eax
$L76688:
	mov	ecx, DWORD PTR -32+[ebp]
	mov	DWORD PTR $T76686[ebp], ecx
	mov	eax, DWORD PTR $T76686[ebp]
$L75643:

; 1092 : }

	pop	esi
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetLongest@Misc@@YAABVCString@@ABV2@0H@Z ENDP		; Misc::GetLongest
_TEXT	ENDS
EXTRN	__imp__free:NEAR
EXTRN	__imp__wcstod:NEAR
EXTRN	__imp___strdup:NEAR
EXTRN	__imp__setlocale:NEAR
_BSS	SEGMENT
	ALIGN	4

$SG75658 DB	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75656 DB	'English', 00H
data	ENDS
;	COMDAT ?Atof@Misc@@YANABVCString@@@Z
_TEXT	SEGMENT
_sValue$ = 8
_szLocale$ = -12
_dVal$ = -8
?Atof@Misc@@YANABVCString@@@Z PROC NEAR			; Misc::Atof, COMDAT

; 1095 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1096 : 	// needs special care to handle decimal point properly
; 1097 : 	// especially since we've no way of knowing how it is encoded.
; 1098 : 	// so we assume that if a period is present then it's encoded
; 1099 : 	// in 'english' else it's in native format
; 1100 : 	char* szLocale = _strdup(setlocale(LC_NUMERIC, NULL));

	mov	esi, esp
	push	0
	push	4
	call	DWORD PTR __imp__setlocale
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp___strdup
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _szLocale$[ebp], eax

; 1101 : 	
; 1102 : 	if (sValue.Find('.') != -1)

	push	46					; 0000002eH
	mov	ecx, DWORD PTR _sValue$[ebp]
	call	?Find@CString@@QBEHG@Z			; CString::Find
	cmp	eax, -1
	je	SHORT $L75655

; 1103 : 		setlocale(LC_NUMERIC, "English");

	mov	esi, esp
	push	OFFSET FLAT:$SG75656
	push	4
	call	DWORD PTR __imp__setlocale
	add	esp, 8
	cmp	esi, esp
	call	__chkesp

; 1104 : 	else

	jmp	SHORT $L75657
$L75655:

; 1105 : 		setlocale(LC_NUMERIC, "");

	mov	esi, esp
	push	OFFSET FLAT:$SG75658
	push	4
	call	DWORD PTR __imp__setlocale
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
$L75657:

; 1106 : 
; 1107 : 	double dVal = _tcstod(sValue, NULL);

	mov	esi, esp
	push	0
	mov	ecx, DWORD PTR _sValue$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	DWORD PTR __imp__wcstod
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	fstp	QWORD PTR _dVal$[ebp]

; 1108 : 
; 1109 : 	// restore locale
; 1110 : 	setlocale(LC_NUMERIC, szLocale);

	mov	esi, esp
	mov	eax, DWORD PTR _szLocale$[ebp]
	push	eax
	push	4
	call	DWORD PTR __imp__setlocale
	add	esp, 8
	cmp	esi, esp
	call	__chkesp

; 1111 : 	free(szLocale);

	mov	esi, esp
	mov	ecx, DWORD PTR _szLocale$[ebp]
	push	ecx
	call	DWORD PTR __imp__free
	add	esp, 4
	cmp	esi, esp
	call	__chkesp

; 1112 : 	
; 1113 : 	return dVal; 

	fld	QWORD PTR _dVal$[ebp]

; 1114 : }

	pop	esi
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Atof@Misc@@YANABVCString@@@Z ENDP			; Misc::Atof
_TEXT	ENDS
PUBLIC	?ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z ; Misc::ShutdownBlockReasonCreate
EXTRN	__imp__GetProcAddress@8:NEAR
EXTRN	__imp__LoadLibraryW@4:NEAR
_BSS	SEGMENT
	ALIGN	4

_?hUser32@?1??ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z@4PAUHINSTANCE__@@A DD 01H DUP (?)
_?$S298@?1??ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z@4EA DB 01H DUP (?)
	ALIGN	4

_?fnCreate@?4??ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z@4P6GH01@ZA DD 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75672 DB	'u', 00H, 's', 00H, 'e', 00H, 'r', 00H, '3', 00H, '2', 00H
	DB	'.', 00H, 'd', 00H, 'l', 00H, 'l', 00H, 00H, 00H
	ORG $+2
$SG75678 DB	'ShutdownBlockReasonCreate', 00H
data	ENDS
;	COMDAT ?ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z
_TEXT	SEGMENT
_hWnd$ = 8
_szReason$ = 12
_wszReason$75680 = -4
_bRes$75681 = -8
?ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z PROC NEAR ; Misc::ShutdownBlockReasonCreate, COMDAT

; 1117 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1118 : 	typedef BOOL (WINAPI *PFNSHUTDOWNBLOCKREASONCREATE)(HWND, LPCWSTR);
; 1119 : 
; 1120 : 	// load user32.dll once only
; 1121 : 	static HMODULE hUser32 = LoadLibrary(_T("user32.dll"));

	xor	eax, eax
	mov	al, BYTE PTR _?$S298@?1??ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75671
	mov	cl, BYTE PTR _?$S298@?1??ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z@4EA
	or	cl, 1
	mov	BYTE PTR _?$S298@?1??ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z@4EA, cl
	mov	esi, esp
	push	OFFSET FLAT:$SG75672
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?hUser32@?1??ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z@4PAUHINSTANCE__@@A, eax
$L75671:

; 1122 : 
; 1123 : 	if (hUser32)

	cmp	DWORD PTR _?hUser32@?1??ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z@4PAUHINSTANCE__@@A, 0
	je	SHORT $L75679

; 1125 : 		static PFNSHUTDOWNBLOCKREASONCREATE fnCreate = (PFNSHUTDOWNBLOCKREASONCREATE)GetProcAddress(hUser32, "ShutdownBlockReasonCreate");

	xor	edx, edx
	mov	dl, BYTE PTR _?$S298@?1??ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z@4EA
	and	edx, 2
	test	edx, edx
	jne	SHORT $L75677
	mov	al, BYTE PTR _?$S298@?1??ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z@4EA
	or	al, 2
	mov	BYTE PTR _?$S298@?1??ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z@4EA, al
	mov	esi, esp
	push	OFFSET FLAT:$SG75678
	mov	ecx, DWORD PTR _?hUser32@?1??ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z@4PAUHINSTANCE__@@A
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?fnCreate@?4??ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z@4P6GH01@ZA, eax
$L75677:

; 1126 : 
; 1127 : 		if (fnCreate)

	cmp	DWORD PTR _?fnCreate@?4??ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z@4P6GH01@ZA, 0
	je	SHORT $L75679

; 1129 : #ifdef _UNICODE
; 1130 : 			LPCWSTR wszReason = szReason;

	mov	edx, DWORD PTR _szReason$[ebp]
	mov	DWORD PTR _wszReason$75680[ebp], edx

; 1131 : #else
; 1132 : 			int nLength = lstrlen(szReason);
; 1133 : 			LPWSTR wszReason = MultiByteToWide(szReason, nLength);
; 1134 : #endif
; 1135 : 
; 1136 : 			BOOL bRes = fnCreate(hWnd, wszReason);

	mov	esi, esp
	mov	eax, DWORD PTR _wszReason$75680[ebp]
	push	eax
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	call	DWORD PTR _?fnCreate@?4??ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z@4P6GH01@ZA
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bRes$75681[ebp], eax

; 1137 : 
; 1138 : #ifndef _UNICODE
; 1139 : 			delete [] wszReason;
; 1140 : #endif
; 1141 : 			return bRes;

	mov	eax, DWORD PTR _bRes$75681[ebp]
	jmp	SHORT $L75663
$L75679:

; 1144 : 
; 1145 : 	// must be < XP
; 1146 : 	return FALSE;

	xor	eax, eax
$L75663:

; 1147 : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?ShutdownBlockReasonCreate@Misc@@YAHPAUHWND__@@PBG@Z ENDP ; Misc::ShutdownBlockReasonCreate
_TEXT	ENDS
PUBLIC	?ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z ; Misc::ShutdownBlockReasonDestroy
_BSS	SEGMENT
_?hUser32@?1??ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z@4PAUHINSTANCE__@@A DD 01H DUP (?)
_?$S299@?1??ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z@4EA DB 01H DUP (?)
	ALIGN	4

_?fnDestroy@?4??ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z@4P6GH0@ZA DD 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75692 DB	'u', 00H, 's', 00H, 'e', 00H, 'r', 00H, '3', 00H, '2', 00H
	DB	'.', 00H, 'd', 00H, 'l', 00H, 'l', 00H, 00H, 00H
	ORG $+2
$SG75698 DB	'ShutdownBlockReasonDestroy', 00H
data	ENDS
;	COMDAT ?ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
?ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z PROC NEAR ; Misc::ShutdownBlockReasonDestroy, COMDAT

; 1150 : {

	push	ebp
	mov	ebp, esp
	push	esi

; 1151 : 	typedef BOOL (WINAPI *PFNSHUTDOWNBLOCKREASONDESTROY)(HWND);
; 1152 : 
; 1153 : 	// load user32.dll once only
; 1154 : 	static HMODULE hUser32 = LoadLibrary(_T("user32.dll"));

	xor	eax, eax
	mov	al, BYTE PTR _?$S299@?1??ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75691
	mov	cl, BYTE PTR _?$S299@?1??ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z@4EA
	or	cl, 1
	mov	BYTE PTR _?$S299@?1??ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z@4EA, cl
	mov	esi, esp
	push	OFFSET FLAT:$SG75692
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?hUser32@?1??ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z@4PAUHINSTANCE__@@A, eax
$L75691:

; 1155 : 
; 1156 : 	if (hUser32)

	cmp	DWORD PTR _?hUser32@?1??ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z@4PAUHINSTANCE__@@A, 0
	je	SHORT $L75699

; 1158 : 		static PFNSHUTDOWNBLOCKREASONDESTROY fnDestroy = (PFNSHUTDOWNBLOCKREASONDESTROY)GetProcAddress(hUser32, "ShutdownBlockReasonDestroy");

	xor	edx, edx
	mov	dl, BYTE PTR _?$S299@?1??ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z@4EA
	and	edx, 2
	test	edx, edx
	jne	SHORT $L75697
	mov	al, BYTE PTR _?$S299@?1??ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z@4EA
	or	al, 2
	mov	BYTE PTR _?$S299@?1??ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z@4EA, al
	mov	esi, esp
	push	OFFSET FLAT:$SG75698
	mov	ecx, DWORD PTR _?hUser32@?1??ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z@4PAUHINSTANCE__@@A
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?fnDestroy@?4??ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z@4P6GH0@ZA, eax
$L75697:

; 1159 : 
; 1160 : 		if (fnDestroy)

	cmp	DWORD PTR _?fnDestroy@?4??ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z@4P6GH0@ZA, 0
	je	SHORT $L75699

; 1161 : 			return fnDestroy(hWnd);

	mov	esi, esp
	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	call	DWORD PTR _?fnDestroy@?4??ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z@4P6GH0@ZA
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75684
$L75699:

; 1163 : 
; 1164 : 	// must be < XP
; 1165 : 	return FALSE;

	xor	eax, eax
$L75684:

; 1166 : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?ShutdownBlockReasonDestroy@Misc@@YAHPAUHWND__@@@Z ENDP	; Misc::ShutdownBlockReasonDestroy
_TEXT	ENDS
PUBLIC	?IsWorkStationLocked@Misc@@YAHXZ		; Misc::IsWorkStationLocked
_BSS	SEGMENT
_?hUser32@?1??IsWorkStationLocked@Misc@@YAHXZ@4PAUHINSTANCE__@@A DD 01H DUP (?)
_?$S300@?1??IsWorkStationLocked@Misc@@YAHXZ@4EA DB 01H DUP (?)
	ALIGN	4

_?fnOpenDesktop@?4??IsWorkStationLocked@Misc@@YAHXZ@4P6GPAUHDESK__@@PADKHK@ZA DD 01H DUP (?)
_?fnCloseDesktop@?4??IsWorkStationLocked@Misc@@YAHXZ@4P6GHPAUHDESK__@@@ZA DD 01H DUP (?)
_?fnSwitchDesktop@?4??IsWorkStationLocked@Misc@@YAHXZ@4P6GHPAUHDESK__@@@ZA DD 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75716 DB	'u', 00H, 's', 00H, 'e', 00H, 'r', 00H, '3', 00H, '2', 00H
	DB	'.', 00H, 'd', 00H, 'l', 00H, 'l', 00H, 00H, 00H
	ORG $+2
$SG75722 DB	'OpenDesktopA', 00H
	ORG $+3
$SG75727 DB	'CloseDesktop', 00H
	ORG $+3
$SG75732 DB	'SwitchDesktop', 00H
	ORG $+2
$SG75735 DB	'Default', 00H
data	ENDS
;	COMDAT ?IsWorkStationLocked@Misc@@YAHXZ
_TEXT	SEGMENT
_hDesk$75734 = -4
_bLocked$75737 = -8
?IsWorkStationLocked@Misc@@YAHXZ PROC NEAR		; Misc::IsWorkStationLocked, COMDAT

; 1169 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1170 : 	// note: we can't call OpenInputDesktop directly because it's not
; 1171 : 	// available on win 9x
; 1172 : 	typedef HDESK (WINAPI *PFNOPENDESKTOP)(LPSTR lpszDesktop, DWORD dwFlags, BOOL fInherit, ACCESS_MASK dwDesiredAccess);
; 1173 : 	typedef BOOL (WINAPI *PFNCLOSEDESKTOP)(HDESK hDesk);
; 1174 : 	typedef BOOL (WINAPI *PFNSWITCHDESKTOP)(HDESK hDesk);
; 1175 : 
; 1176 : 	// load user32.dll once only
; 1177 : 	static HMODULE hUser32 = LoadLibrary(_T("user32.dll"));

	xor	eax, eax
	mov	al, BYTE PTR _?$S300@?1??IsWorkStationLocked@Misc@@YAHXZ@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75715
	mov	cl, BYTE PTR _?$S300@?1??IsWorkStationLocked@Misc@@YAHXZ@4EA
	or	cl, 1
	mov	BYTE PTR _?$S300@?1??IsWorkStationLocked@Misc@@YAHXZ@4EA, cl
	mov	esi, esp
	push	OFFSET FLAT:$SG75716
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?hUser32@?1??IsWorkStationLocked@Misc@@YAHXZ@4PAUHINSTANCE__@@A, eax
$L75715:

; 1178 : 
; 1179 : 	if (hUser32)

	cmp	DWORD PTR _?hUser32@?1??IsWorkStationLocked@Misc@@YAHXZ@4PAUHINSTANCE__@@A, 0
	je	$L75736

; 1181 : 		static PFNOPENDESKTOP fnOpenDesktop = (PFNOPENDESKTOP)GetProcAddress(hUser32, "OpenDesktopA");

	xor	edx, edx
	mov	dl, BYTE PTR _?$S300@?1??IsWorkStationLocked@Misc@@YAHXZ@4EA
	and	edx, 2
	test	edx, edx
	jne	SHORT $L75721
	mov	al, BYTE PTR _?$S300@?1??IsWorkStationLocked@Misc@@YAHXZ@4EA
	or	al, 2
	mov	BYTE PTR _?$S300@?1??IsWorkStationLocked@Misc@@YAHXZ@4EA, al
	mov	esi, esp
	push	OFFSET FLAT:$SG75722
	mov	ecx, DWORD PTR _?hUser32@?1??IsWorkStationLocked@Misc@@YAHXZ@4PAUHINSTANCE__@@A
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?fnOpenDesktop@?4??IsWorkStationLocked@Misc@@YAHXZ@4P6GPAUHDESK__@@PADKHK@ZA, eax
$L75721:

; 1182 : 		static PFNCLOSEDESKTOP fnCloseDesktop = (PFNCLOSEDESKTOP)GetProcAddress(hUser32, "CloseDesktop");

	xor	edx, edx
	mov	dl, BYTE PTR _?$S300@?1??IsWorkStationLocked@Misc@@YAHXZ@4EA
	and	edx, 4
	test	edx, edx
	jne	SHORT $L75726
	mov	al, BYTE PTR _?$S300@?1??IsWorkStationLocked@Misc@@YAHXZ@4EA
	or	al, 4
	mov	BYTE PTR _?$S300@?1??IsWorkStationLocked@Misc@@YAHXZ@4EA, al
	mov	esi, esp
	push	OFFSET FLAT:$SG75727
	mov	ecx, DWORD PTR _?hUser32@?1??IsWorkStationLocked@Misc@@YAHXZ@4PAUHINSTANCE__@@A
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?fnCloseDesktop@?4??IsWorkStationLocked@Misc@@YAHXZ@4P6GHPAUHDESK__@@@ZA, eax
$L75726:

; 1183 : 		static PFNSWITCHDESKTOP fnSwitchDesktop = (PFNSWITCHDESKTOP)GetProcAddress(hUser32, "SwitchDesktop");

	xor	edx, edx
	mov	dl, BYTE PTR _?$S300@?1??IsWorkStationLocked@Misc@@YAHXZ@4EA
	and	edx, 8
	test	edx, edx
	jne	SHORT $L75731
	mov	al, BYTE PTR _?$S300@?1??IsWorkStationLocked@Misc@@YAHXZ@4EA
	or	al, 8
	mov	BYTE PTR _?$S300@?1??IsWorkStationLocked@Misc@@YAHXZ@4EA, al
	mov	esi, esp
	push	OFFSET FLAT:$SG75732
	mov	ecx, DWORD PTR _?hUser32@?1??IsWorkStationLocked@Misc@@YAHXZ@4PAUHINSTANCE__@@A
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?fnSwitchDesktop@?4??IsWorkStationLocked@Misc@@YAHXZ@4P6GHPAUHDESK__@@@ZA, eax
$L75731:

; 1184 : 
; 1185 : 		if (fnOpenDesktop && fnCloseDesktop && fnSwitchDesktop)

	cmp	DWORD PTR _?fnOpenDesktop@?4??IsWorkStationLocked@Misc@@YAHXZ@4P6GPAUHDESK__@@PADKHK@ZA, 0
	je	SHORT $L75736
	cmp	DWORD PTR _?fnCloseDesktop@?4??IsWorkStationLocked@Misc@@YAHXZ@4P6GHPAUHDESK__@@@ZA, 0
	je	SHORT $L75736
	cmp	DWORD PTR _?fnSwitchDesktop@?4??IsWorkStationLocked@Misc@@YAHXZ@4P6GHPAUHDESK__@@@ZA, 0
	je	SHORT $L75736

; 1187 : 			HDESK hDesk = fnOpenDesktop("Default", 0, FALSE, DESKTOP_SWITCHDESKTOP);

	mov	esi, esp
	push	256					; 00000100H
	push	0
	push	0
	push	OFFSET FLAT:$SG75735
	call	DWORD PTR _?fnOpenDesktop@?4??IsWorkStationLocked@Misc@@YAHXZ@4P6GPAUHDESK__@@PADKHK@ZA
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hDesk$75734[ebp], eax

; 1188 : 
; 1189 : 			if (hDesk)

	cmp	DWORD PTR _hDesk$75734[ebp], 0
	je	SHORT $L75736

; 1191 : 				BOOL bLocked = !fnSwitchDesktop(hDesk);

	mov	esi, esp
	mov	edx, DWORD PTR _hDesk$75734[ebp]
	push	edx
	call	DWORD PTR _?fnSwitchDesktop@?4??IsWorkStationLocked@Misc@@YAHXZ@4P6GHPAUHDESK__@@@ZA
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR _bLocked$75737[ebp], eax

; 1192 : 					
; 1193 : 				// cleanup
; 1194 : 				fnCloseDesktop(hDesk);

	mov	esi, esp
	mov	eax, DWORD PTR _hDesk$75734[ebp]
	push	eax
	call	DWORD PTR _?fnCloseDesktop@?4??IsWorkStationLocked@Misc@@YAHXZ@4P6GHPAUHDESK__@@@ZA
	cmp	esi, esp
	call	__chkesp

; 1195 : 
; 1196 : 				return bLocked;

	mov	eax, DWORD PTR _bLocked$75737[ebp]
	jmp	SHORT $L75701
$L75736:

; 1200 : 
; 1201 : 	// must be win9x
; 1202 : 	return FALSE;

	xor	eax, eax
$L75701:

; 1203 : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsWorkStationLocked@Misc@@YAHXZ ENDP			; Misc::IsWorkStationLocked
_TEXT	ENDS
PUBLIC	?IsScreenSaverActive@Misc@@YAHXZ		; Misc::IsScreenSaverActive
EXTRN	__imp__SystemParametersInfoW@16:NEAR
;	COMDAT ?IsScreenSaverActive@Misc@@YAHXZ
_TEXT	SEGMENT
_bActive$ = -4
?IsScreenSaverActive@Misc@@YAHXZ PROC NEAR		; Misc::IsScreenSaverActive, COMDAT

; 1206 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1207 : 	BOOL bActive = FALSE;

	mov	DWORD PTR _bActive$[ebp], 0

; 1208 : 	
; 1209 : 	return (::SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &bActive, 0) && bActive); 

	mov	esi, esp
	push	0
	lea	eax, DWORD PTR _bActive$[ebp]
	push	eax
	push	0
	push	114					; 00000072H
	call	DWORD PTR __imp__SystemParametersInfoW@16
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L76703
	cmp	DWORD PTR _bActive$[ebp], 0
	je	SHORT $L76703
	mov	DWORD PTR -8+[ebp], 1
	jmp	SHORT $L76704
$L76703:
	mov	DWORD PTR -8+[ebp], 0
$L76704:
	mov	eax, DWORD PTR -8+[ebp]

; 1210 : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsScreenSaverActive@Misc@@YAHXZ ENDP			; Misc::IsScreenSaverActive
_TEXT	ENDS
PUBLIC	?IsScreenReaderActive@Misc@@YAHH@Z		; Misc::IsScreenReaderActive
PUBLIC	?IsMSNarratorActive@Misc@@YAHXZ			; Misc::IsMSNarratorActive
;	COMDAT ?IsScreenReaderActive@Misc@@YAHH@Z
_TEXT	SEGMENT
_bCheckForMSNarrator$ = 8
_bActive$ = -4
?IsScreenReaderActive@Misc@@YAHH@Z PROC NEAR		; Misc::IsScreenReaderActive, COMDAT

; 1213 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1214 :     BOOL bActive = FALSE;

	mov	DWORD PTR _bActive$[ebp], 0

; 1215 : 
; 1216 : 	if (::SystemParametersInfo(SPI_GETSCREENREADER, 0, &bActive, 0) && bActive)

	mov	esi, esp
	push	0
	lea	eax, DWORD PTR _bActive$[ebp]
	push	eax
	push	0
	push	70					; 00000046H
	call	DWORD PTR __imp__SystemParametersInfoW@16
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L75745
	cmp	DWORD PTR _bActive$[ebp], 0
	je	SHORT $L75745

; 1217 : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L75743
$L75745:

; 1218 : 
; 1219 : 	// else
; 1220 : 	if (bCheckForMSNarrator && IsMSNarratorActive())

	cmp	DWORD PTR _bCheckForMSNarrator$[ebp], 0
	je	SHORT $L75746
	call	?IsMSNarratorActive@Misc@@YAHXZ		; Misc::IsMSNarratorActive
	test	eax, eax
	je	SHORT $L75746

; 1221 : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L75743
$L75746:

; 1222 : 
; 1223 : 	// else
; 1224 : 	return FALSE;

	xor	eax, eax
$L75743:

; 1225 : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsScreenReaderActive@Misc@@YAHH@Z ENDP			; Misc::IsScreenReaderActive
_TEXT	ENDS
EXTRN	??0CMutex@@QAE@HPBGPAU_SECURITY_ATTRIBUTES@@@Z:NEAR ; CMutex::CMutex
EXTRN	??1CMutex@@UAE@XZ:NEAR				; CMutex::~CMutex
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75750 DB	'N', 00H, 'a', 00H, 'r', 00H, 'r', 00H, 'a', 00H, 't', 00H
	DB	'o', 00H, 'r', 00H, 'R', 00H, 'u', 00H, 'n', 00H, 'n', 00H, 'i'
	DB	00H, 'n', 00H, 'g', 00H, 00H, 00H
data	ENDS
;	COMDAT ?IsMSNarratorActive@Misc@@YAHXZ
_TEXT	SEGMENT
$T76709 = -16
_mutex$ = -12
?IsMSNarratorActive@Misc@@YAHXZ PROC NEAR		; Misc::IsMSNarratorActive, COMDAT

; 1228 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 1229 : 	CMutex mutex(FALSE, _T("NarratorRunning"));

	push	0
	push	OFFSET FLAT:$SG75750
	push	0
	lea	ecx, DWORD PTR _mutex$[ebp]
	call	??0CMutex@@QAE@HPBGPAU_SECURITY_ATTRIBUTES@@@Z ; CMutex::CMutex

; 1230 : 
; 1231 : 	return (GetLastError() == ERROR_ALREADY_EXISTS);

	mov	esi, esp
	call	DWORD PTR __imp__GetLastError@0
	cmp	esi, esp
	call	__chkesp
	xor	ecx, ecx
	cmp	eax, 183				; 000000b7H
	sete	cl
	mov	DWORD PTR $T76709[ebp], ecx
	lea	ecx, DWORD PTR _mutex$[ebp]
	call	??1CMutex@@UAE@XZ			; CMutex::~CMutex
	mov	eax, DWORD PTR $T76709[ebp]

; 1232 : }

	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsMSNarratorActive@Misc@@YAHXZ ENDP			; Misc::IsMSNarratorActive
_TEXT	ENDS
PUBLIC	?GetUserDefaultUILanguage@Misc@@YAGXZ		; Misc::GetUserDefaultUILanguage
_BSS	SEGMENT
_?hLib@?1??GetUserDefaultUILanguage@Misc@@YAGXZ@4PAUHINSTANCE__@@A DD 01H DUP (?)
_?$S301@?1??GetUserDefaultUILanguage@Misc@@YAGXZ@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75761 DB	'k', 00H, 'e', 00H, 'r', 00H, 'n', 00H, 'e', 00H, 'l', 00H
	DB	'3', 00H, '2', 00H, '.', 00H, 'd', 00H, 'l', 00H, 'l', 00H, 00H
	DB	00H
	ORG $+2
$SG75766 DB	'GetUserDefaultUILanguage', 00H
data	ENDS
;	COMDAT ?GetUserDefaultUILanguage@Misc@@YAGXZ
_TEXT	SEGMENT
_nLangID$ = -4
_pFN$75764 = -8
?GetUserDefaultUILanguage@Misc@@YAGXZ PROC NEAR		; Misc::GetUserDefaultUILanguage, COMDAT

; 1235 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1236 : 	typedef LANGID (WINAPI *FNGETUSERDEFAULTUILANGUAGE)(VOID);
; 1237 : 
; 1238 : 	// must link dynamically to kernel32 else problem with win95/NT4
; 1239 : 	static HMODULE hLib = LoadLibrary(_T("kernel32.dll"));

	xor	eax, eax
	mov	al, BYTE PTR _?$S301@?1??GetUserDefaultUILanguage@Misc@@YAGXZ@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75760
	mov	cl, BYTE PTR _?$S301@?1??GetUserDefaultUILanguage@Misc@@YAGXZ@4EA
	or	cl, 1
	mov	BYTE PTR _?$S301@?1??GetUserDefaultUILanguage@Misc@@YAGXZ@4EA, cl
	mov	esi, esp
	push	OFFSET FLAT:$SG75761
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?hLib@?1??GetUserDefaultUILanguage@Misc@@YAGXZ@4PAUHINSTANCE__@@A, eax
$L75760:

; 1240 : 	LANGID nLangID = 0;

	mov	WORD PTR _nLangID$[ebp], 0

; 1241 : 
; 1242 : 	if (hLib)

	cmp	DWORD PTR _?hLib@?1??GetUserDefaultUILanguage@Misc@@YAGXZ@4PAUHINSTANCE__@@A, 0
	je	SHORT $L75767

; 1244 : 		FNGETUSERDEFAULTUILANGUAGE pFN = (FNGETUSERDEFAULTUILANGUAGE)GetProcAddress(hLib, "GetUserDefaultUILanguage");

	mov	esi, esp
	push	OFFSET FLAT:$SG75766
	mov	edx, DWORD PTR _?hLib@?1??GetUserDefaultUILanguage@Misc@@YAGXZ@4PAUHINSTANCE__@@A
	push	edx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pFN$75764[ebp], eax

; 1245 : 
; 1246 : 		if (pFN)

	cmp	DWORD PTR _pFN$75764[ebp], 0
	je	SHORT $L75767

; 1247 : 			nLangID = pFN();

	mov	esi, esp
	call	DWORD PTR _pFN$75764[ebp]
	cmp	esi, esp
	call	__chkesp
	mov	WORD PTR _nLangID$[ebp], ax
$L75767:

; 1249 : 
; 1250 : 	return nLangID;

	mov	ax, WORD PTR _nLangID$[ebp]

; 1251 : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetUserDefaultUILanguage@Misc@@YAGXZ ENDP		; Misc::GetUserDefaultUILanguage
_TEXT	ENDS
PUBLIC	?GetDefCharset@Misc@@YA?AVCString@@XZ		; Misc::GetDefCharset
;	COMDAT xdata$x
; File D:\_CODE\Shared\Misc.cpp
xdata$x	SEGMENT
$T76722	DD	019930520H
	DD	02H
	DD	FLAT:$T76724
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76724	DD	0ffffffffH
	DD	FLAT:$L76718
	DD	00H
	DD	FLAT:$L76717
xdata$x	ENDS
;	COMDAT ?GetDefCharset@Misc@@YA?AVCString@@XZ
_TEXT	SEGMENT
$T76719 = -20
__$EHRec$ = -12
___$ReturnUdt$ = 8
_sDefCharset$ = -16
?GetDefCharset@Misc@@YA?AVCString@@XZ PROC NEAR		; Misc::GetDefCharset, COMDAT

; 1254 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76723
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T76719[ebp], 0

; 1255 : 	CString sDefCharset;

	lea	ecx, DWORD PTR _sDefCharset$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1256 : 	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, sDefCharset.GetBuffer(7), 6);

	mov	esi, esp
	push	6
	push	7
	lea	ecx, DWORD PTR _sDefCharset$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	push	4100					; 00001004H
	push	1024					; 00000400H
	call	DWORD PTR __imp__GetLocaleInfoW@16
	cmp	esi, esp
	call	__chkesp

; 1257 : 	sDefCharset.ReleaseBuffer();

	push	-1
	lea	ecx, DWORD PTR _sDefCharset$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 1258 : 
; 1259 : 	return sDefCharset;

	lea	eax, DWORD PTR _sDefCharset$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T76719[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76719[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sDefCharset$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1260 : }

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
$L76717:
	lea	ecx, DWORD PTR _sDefCharset$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76718:
	mov	eax, DWORD PTR $T76719[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76720
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76720:
	ret	0
$L76723:
	mov	eax, OFFSET FLAT:$T76722
	jmp	___CxxFrameHandler
text$x	ENDS
?GetDefCharset@Misc@@YA?AVCString@@XZ ENDP		; Misc::GetDefCharset
PUBLIC	?LCMapStringW@Misc@@YAHAAVCString@@K@Z		; Misc::LCMapStringW
EXTRN	__imp__LCMapStringW@24:NEAR
;	COMDAT xdata$x
; File D:\_CODE\Shared\Misc.cpp
xdata$x	SEGMENT
$T76732	DD	019930520H
	DD	01H
	DD	FLAT:$T76734
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76734	DD	0ffffffffH
	DD	FLAT:$L76730
xdata$x	ENDS
;	COMDAT ?LCMapStringW@Misc@@YAHAAVCString@@K@Z
_TEXT	SEGMENT
_sText$ = 8
_dwMapFlags$ = 12
_nLen$ = -28
_sMapped$ = -20
_wLangID$ = -24
_dwLCID$ = -16
_bRes$ = -32
$T76729 = -36
__$EHRec$ = -12
?LCMapStringW@Misc@@YAHAAVCString@@K@Z PROC NEAR	; Misc::LCMapStringW, COMDAT

; 1263 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76733
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

; 1264 : 	int nLen = sText.GetLength();

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR _nLen$[ebp], eax

; 1265 : 	CString sMapped;

	lea	ecx, DWORD PTR _sMapped$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1266 : 
; 1267 : 	WORD wLangID = MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT);

	mov	WORD PTR _wLangID$[ebp], 2048		; 00000800H

; 1268 : 	DWORD dwLCID = MAKELCID(wLangID, SORT_DEFAULT);

	mov	eax, DWORD PTR _wLangID$[ebp]
	and	eax, 65535				; 0000ffffH
	mov	DWORD PTR _dwLCID$[ebp], eax

; 1269 : 
; 1270 : 	BOOL bRes = ::LCMapString(dwLCID, 
; 1271 : 								dwMapFlags, 
; 1272 : 								(LPCTSTR)sText, 
; 1273 : 								-1, 
; 1274 : 								sMapped.GetBuffer(nLen+1), 
; 1275 : 								nLen+1);

	mov	ecx, DWORD PTR _nLen$[ebp]
	add	ecx, 1
	mov	esi, esp
	push	ecx
	mov	edx, DWORD PTR _nLen$[ebp]
	add	edx, 1
	push	edx
	lea	ecx, DWORD PTR _sMapped$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	push	-1
	mov	ecx, DWORD PTR _sText$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	eax, DWORD PTR _dwMapFlags$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwLCID$[ebp]
	push	ecx
	call	DWORD PTR __imp__LCMapStringW@24
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bRes$[ebp], eax

; 1276 : 	sMapped.ReleaseBuffer();

	push	-1
	lea	ecx, DWORD PTR _sMapped$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 1277 : 
; 1278 : 	if (bRes)

	cmp	DWORD PTR _bRes$[ebp], 0
	je	SHORT $L75796

; 1279 : 		sText = sMapped;

	lea	edx, DWORD PTR _sMapped$[ebp]
	push	edx
	mov	ecx, DWORD PTR _sText$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
$L75796:

; 1280 : 
; 1281 : 	return bRes;

	mov	eax, DWORD PTR _bRes$[ebp]
	mov	DWORD PTR $T76729[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sMapped$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76729[ebp]

; 1282 : }

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
$L76730:
	lea	ecx, DWORD PTR _sMapped$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76733:
	mov	eax, OFFSET FLAT:$T76732
	jmp	___CxxFrameHandler
text$x	ENDS
?LCMapStringW@Misc@@YAHAAVCString@@K@Z ENDP		; Misc::LCMapStringW
PUBLIC	?MakeUpper@Misc@@YAXAAVCString@@@Z		; Misc::MakeUpper
EXTRN	?MakeUpper@CString@@QAEXXZ:NEAR			; CString::MakeUpper
;	COMDAT ?MakeUpper@Misc@@YAXAAVCString@@@Z
_TEXT	SEGMENT
_sText$ = 8
?MakeUpper@Misc@@YAXAAVCString@@@Z PROC NEAR		; Misc::MakeUpper, COMDAT

; 1285 : {

	push	ebp
	mov	ebp, esp

; 1286 : 	if (!LCMapString(sText, LCMAP_UPPERCASE | LCMAP_LINGUISTIC_CASING))

	push	16777728				; 01000200H
	mov	eax, DWORD PTR _sText$[ebp]
	push	eax
	call	?LCMapStringW@Misc@@YAHAAVCString@@K@Z	; Misc::LCMapStringW
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75801

; 1287 : 		sText.MakeUpper();

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?MakeUpper@CString@@QAEXXZ		; CString::MakeUpper
$L75801:

; 1288 : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?MakeUpper@Misc@@YAXAAVCString@@@Z ENDP			; Misc::MakeUpper
_TEXT	ENDS
PUBLIC	?MakeLower@Misc@@YAXAAVCString@@@Z		; Misc::MakeLower
EXTRN	?MakeLower@CString@@QAEXXZ:NEAR			; CString::MakeLower
;	COMDAT ?MakeLower@Misc@@YAXAAVCString@@@Z
_TEXT	SEGMENT
_sText$ = 8
?MakeLower@Misc@@YAXAAVCString@@@Z PROC NEAR		; Misc::MakeLower, COMDAT

; 1291 : {

	push	ebp
	mov	ebp, esp

; 1292 : 	if (!LCMapString(sText, LCMAP_LOWERCASE | LCMAP_LINGUISTIC_CASING))

	push	16777472				; 01000100H
	mov	eax, DWORD PTR _sText$[ebp]
	push	eax
	call	?LCMapStringW@Misc@@YAHAAVCString@@K@Z	; Misc::LCMapStringW
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75805

; 1293 : 		sText.MakeLower();

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
$L75805:

; 1294 : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?MakeLower@Misc@@YAXAAVCString@@@Z ENDP			; Misc::MakeLower
_TEXT	ENDS
PUBLIC	?ToUpper@Misc@@YA?AVCString@@PBG@Z		; Misc::ToUpper
;	COMDAT xdata$x
; File D:\_CODE\Shared\Misc.cpp
xdata$x	SEGMENT
$T76748	DD	019930520H
	DD	02H
	DD	FLAT:$T76750
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76750	DD	0ffffffffH
	DD	FLAT:$L76744
	DD	00H
	DD	FLAT:$L76743
xdata$x	ENDS
;	COMDAT ?ToUpper@Misc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT
_szText$ = 12
___$ReturnUdt$ = 8
_sText$ = -16
$T76745 = -20
__$EHRec$ = -12
?ToUpper@Misc@@YA?AVCString@@PBG@Z PROC NEAR		; Misc::ToUpper, COMDAT

; 1297 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76749
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T76745[ebp], 0

; 1298 : 	CString sText(szText);

	mov	eax, DWORD PTR _szText$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1299 : 	MakeUpper(sText);

	lea	ecx, DWORD PTR _sText$[ebp]
	push	ecx
	call	?MakeUpper@Misc@@YAXAAVCString@@@Z	; Misc::MakeUpper
	add	esp, 4

; 1300 : 
; 1301 : 	return sText;

	lea	edx, DWORD PTR _sText$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T76745[ebp]
	or	al, 1
	mov	DWORD PTR $T76745[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1302 : }

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
$L76743:
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76744:
	mov	eax, DWORD PTR $T76745[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76746
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76746:
	ret	0
$L76749:
	mov	eax, OFFSET FLAT:$T76748
	jmp	___CxxFrameHandler
text$x	ENDS
?ToUpper@Misc@@YA?AVCString@@PBG@Z ENDP			; Misc::ToUpper
PUBLIC	?ToLower@Misc@@YA?AVCString@@PBG@Z		; Misc::ToLower
;	COMDAT xdata$x
; File D:\_CODE\Shared\Misc.cpp
xdata$x	SEGMENT
$T76760	DD	019930520H
	DD	02H
	DD	FLAT:$T76762
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76762	DD	0ffffffffH
	DD	FLAT:$L76756
	DD	00H
	DD	FLAT:$L76755
xdata$x	ENDS
;	COMDAT ?ToLower@Misc@@YA?AVCString@@PBG@Z
_TEXT	SEGMENT
_szText$ = 12
___$ReturnUdt$ = 8
_sText$ = -16
$T76757 = -20
__$EHRec$ = -12
?ToLower@Misc@@YA?AVCString@@PBG@Z PROC NEAR		; Misc::ToLower, COMDAT

; 1305 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76761
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T76757[ebp], 0

; 1306 : 	CString sText(szText);

	mov	eax, DWORD PTR _szText$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1307 : 	MakeLower(sText);

	lea	ecx, DWORD PTR _sText$[ebp]
	push	ecx
	call	?MakeLower@Misc@@YAXAAVCString@@@Z	; Misc::MakeLower
	add	esp, 4

; 1308 : 	
; 1309 : 	return sText;

	lea	edx, DWORD PTR _sText$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T76757[ebp]
	or	al, 1
	mov	DWORD PTR $T76757[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1310 : }

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
$L76755:
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76756:
	mov	eax, DWORD PTR $T76757[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76758
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76758:
	ret	0
$L76761:
	mov	eax, OFFSET FLAT:$T76760
	jmp	___CxxFrameHandler
text$x	ENDS
?ToLower@Misc@@YA?AVCString@@PBG@Z ENDP			; Misc::ToLower
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75833 DB	'()-\/{}[]:;,. ?"''', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76775	DD	019930520H
	DD	03H
	DD	FLAT:$T76777
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76777	DD	0ffffffffH
	DD	FLAT:$L76771
	DD	00H
	DD	FLAT:$L76772
	DD	01H
	DD	FLAT:$L76773
xdata$x	ENDS
;	COMDAT ?FindWord@Misc@@YAHPBG0HH@Z
_TEXT	SEGMENT
_szWord$ = 8
_szText$ = 12
_bCaseSensitive$ = 16
_bMatchWholeWord$ = 20
_sWord$ = -24
_sText$ = -20
_nFind$ = -16
_DELIMS$75832 = -36
_cPrevChar$75834 = -32
_cNextChar$75835 = -28
$T76767 = -40
$T76768 = -44
$T76769 = -48
$T76770 = -52
__$EHRec$ = -12
?FindWord@Misc@@YAHPBG0HH@Z PROC NEAR			; Misc::FindWord, COMDAT

; 1313 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76776
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 40					; 00000028H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-52]
	mov	ecx, 10					; 0000000aH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1314 : 	CString sWord(szWord), sText(szText);

	mov	eax, DWORD PTR _szWord$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _szText$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 1315 : 	
; 1316 : 	if (sWord.GetLength() > sText.GetLength())

	lea	ecx, DWORD PTR _sWord$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	esi, eax
	lea	ecx, DWORD PTR _sText$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	cmp	esi, eax
	jle	SHORT $L75824

; 1317 : 		return FALSE;

	mov	DWORD PTR $T76767[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76767[ebp]
	jmp	$L75821
$L75824:

; 1318 : 	
; 1319 : 	Trim(sWord);

	push	0
	lea	edx, DWORD PTR _sWord$[ebp]
	push	edx
	call	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z	; Misc::Trim
	add	esp, 8

; 1320 : 	
; 1321 : 	if (!bCaseSensitive)

	cmp	DWORD PTR _bCaseSensitive$[ebp], 0
	jne	SHORT $L75826

; 1323 : 		MakeUpper(sWord);

	lea	eax, DWORD PTR _sWord$[ebp]
	push	eax
	call	?MakeUpper@Misc@@YAXAAVCString@@@Z	; Misc::MakeUpper
	add	esp, 4

; 1324 : 		MakeUpper(sText);

	lea	ecx, DWORD PTR _sText$[ebp]
	push	ecx
	call	?MakeUpper@Misc@@YAXAAVCString@@@Z	; Misc::MakeUpper
	add	esp, 4
$L75826:

; 1326 : 	
; 1327 : 	int nFind = sText.Find(sWord);

	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sText$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	mov	DWORD PTR _nFind$[ebp], eax

; 1328 : 	
; 1329 : 	if (nFind == -1)

	cmp	DWORD PTR _nFind$[ebp], -1
	jne	SHORT $L75828

; 1330 : 		return FALSE;

	mov	DWORD PTR $T76768[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76768[ebp]
	jmp	$L75821
$L75828:

; 1331 : 	
; 1332 : 	else if (bMatchWholeWord) // test whole word

	cmp	DWORD PTR _bMatchWholeWord$[ebp], 0
	je	$L75831

; 1334 : 		const CString DELIMS("()-\\/{}[]:;,. ?\"'");

	push	OFFSET FLAT:$SG75833
	lea	ecx, DWORD PTR _DELIMS$75832[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 1335 : 		
; 1336 : 		// prior and next chars must be delimeters
; 1337 : 		TCHAR cPrevChar = 0, cNextChar = 0;

	mov	WORD PTR _cPrevChar$75834[ebp], 0
	mov	WORD PTR _cNextChar$75835[ebp], 0

; 1338 : 		
; 1339 : 		// prev
; 1340 : 		if (nFind == 0) // word starts at start

	cmp	DWORD PTR _nFind$[ebp], 0
	jne	SHORT $L75836

; 1341 : 			cPrevChar = ' '; // known delim

	mov	WORD PTR _cPrevChar$75834[ebp], 32	; 00000020H

; 1342 : 		else

	jmp	SHORT $L75837
$L75836:

; 1343 : 			cPrevChar = sText[nFind - 1];

	mov	edx, DWORD PTR _nFind$[ebp]
	sub	edx, 1
	push	edx
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	mov	WORD PTR _cPrevChar$75834[ebp], ax
$L75837:

; 1344 : 		
; 1345 : 		// next
; 1346 : 		if ((nFind + sWord.GetLength()) < sText.GetLength())

	lea	ecx, DWORD PTR _sWord$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	esi, DWORD PTR _nFind$[ebp]
	add	esi, eax
	lea	ecx, DWORD PTR _sText$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	cmp	esi, eax
	jge	SHORT $L75838

; 1347 : 			cNextChar = sText[nFind + sWord.GetLength()];

	lea	ecx, DWORD PTR _sWord$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	ecx, DWORD PTR _nFind$[ebp]
	add	ecx, eax
	push	ecx
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	mov	WORD PTR _cNextChar$75835[ebp], ax

; 1348 : 		else

	jmp	SHORT $L75839
$L75838:

; 1349 : 			cNextChar = ' '; // known delim

	mov	WORD PTR _cNextChar$75835[ebp], 32	; 00000020H
$L75839:

; 1350 : 		
; 1351 : 		if (DELIMS.Find(cPrevChar) == -1 || DELIMS.Find(cNextChar) == -1)

	mov	dx, WORD PTR _cPrevChar$75834[ebp]
	push	edx
	lea	ecx, DWORD PTR _DELIMS$75832[ebp]
	call	?Find@CString@@QBEHG@Z			; CString::Find
	cmp	eax, -1
	je	SHORT $L75841
	mov	ax, WORD PTR _cNextChar$75835[ebp]
	push	eax
	lea	ecx, DWORD PTR _DELIMS$75832[ebp]
	call	?Find@CString@@QBEHG@Z			; CString::Find
	cmp	eax, -1
	jne	SHORT $L75840
$L75841:

; 1352 : 			return FALSE;

	mov	DWORD PTR $T76769[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _DELIMS$75832[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76769[ebp]
	jmp	SHORT $L75821
$L75840:

; 1353 : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _DELIMS$75832[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75831:

; 1354 : 	
; 1355 : 	return TRUE;

	mov	DWORD PTR $T76770[ebp], 1
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76770[ebp]
$L75821:

; 1356 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 52					; 00000034H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76771:
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76772:
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76773:
	lea	ecx, DWORD PTR _DELIMS$75832[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76776:
	mov	eax, OFFSET FLAT:$T76775
	jmp	___CxxFrameHandler
text$x	ENDS
?FindWord@Misc@@YAHPBG0HH@Z ENDP			; Misc::FindWord
PUBLIC	?ParseSearchString@Misc@@YAHPBGAAVCStringArray@@@Z ; Misc::ParseSearchString
EXTRN	__imp__lstrlenW@4:NEAR
;	COMDAT xdata$x
; File D:\_CODE\Shared\Misc.cpp
xdata$x	SEGMENT
$T76786	DD	019930520H
	DD	01H
	DD	FLAT:$T76788
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76788	DD	0ffffffffH
	DD	FLAT:$L76784
xdata$x	ENDS
;	COMDAT ?ParseSearchString@Misc@@YAHPBGAAVCStringArray@@@Z
_TEXT	SEGMENT
_szLookFor$ = 8
_aWords$ = 12
_nLen$ = -32
_bInQuotes$ = -20
_bAddWord$ = -16
_sWord$ = -24
_nPos$ = -28
$T76783 = -36
__$EHRec$ = -12
?ParseSearchString@Misc@@YAHPBGAAVCStringArray@@@Z PROC NEAR ; Misc::ParseSearchString, COMDAT

; 1359 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76787
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 28					; 0000001cH
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 1360 : 	aWords.RemoveAll();

	mov	ecx, DWORD PTR _aWords$[ebp]
	call	?RemoveAll@CStringArray@@QAEXXZ		; CStringArray::RemoveAll

; 1361 : 	
; 1362 : 	// parse on spaces unless enclosed in double-quotes
; 1363 : 	int nLen = lstrlen(szLookFor);

	mov	esi, esp
	mov	eax, DWORD PTR _szLookFor$[ebp]
	push	eax
	call	DWORD PTR __imp__lstrlenW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nLen$[ebp], eax

; 1364 : 	BOOL bInQuotes = FALSE, bAddWord = FALSE;

	mov	DWORD PTR _bInQuotes$[ebp], 0
	mov	DWORD PTR _bAddWord$[ebp], 0

; 1365 : 	CString sWord;

	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1366 : 	
; 1367 : 	for (int nPos = 0; nPos < nLen; nPos++)

	mov	DWORD PTR _nPos$[ebp], 0
	jmp	SHORT $L75853
$L75854:
	mov	ecx, DWORD PTR _nPos$[ebp]
	add	ecx, 1
	mov	DWORD PTR _nPos$[ebp], ecx
$L75853:
	mov	edx, DWORD PTR _nPos$[ebp]
	cmp	edx, DWORD PTR _nLen$[ebp]
	jge	$L75855

; 1370 : 		{

	mov	eax, DWORD PTR _nPos$[ebp]
	mov	ecx, DWORD PTR _szLookFor$[ebp]
	xor	edx, edx
	mov	dx, WORD PTR [ecx+eax*2]
	mov	DWORD PTR -40+[ebp], edx
	cmp	DWORD PTR -40+[ebp], 32			; 00000020H
	je	SHORT $L75860
	cmp	DWORD PTR -40+[ebp], 34			; 00000022H
	je	SHORT $L75863
	jmp	SHORT $L75864
$L75860:

; 1371 : 		case ' ': // word break
; 1372 : 			if (bInQuotes)

	cmp	DWORD PTR _bInQuotes$[ebp], 0
	je	SHORT $L75861

; 1373 : 				sWord += szLookFor[nPos];

	mov	eax, DWORD PTR _nPos$[ebp]
	mov	ecx, DWORD PTR _szLookFor$[ebp]
	mov	dx, WORD PTR [ecx+eax*2]
	push	edx
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??YCString@@QAEABV0@G@Z			; CString::operator+=

; 1374 : 			else

	jmp	SHORT $L75862
$L75861:

; 1375 : 				bAddWord = TRUE;

	mov	DWORD PTR _bAddWord$[ebp], 1
$L75862:

; 1376 : 			break;

	jmp	SHORT $L75857
$L75863:

; 1377 : 			
; 1378 : 		case '\"':
; 1379 : 			// whether its the start or end we add the current word
; 1380 : 			// and flip bInQuotes
; 1381 : 			bInQuotes = !bInQuotes;

	xor	eax, eax
	cmp	DWORD PTR _bInQuotes$[ebp], 0
	sete	al
	mov	DWORD PTR _bInQuotes$[ebp], eax

; 1382 : 			bAddWord = TRUE;

	mov	DWORD PTR _bAddWord$[ebp], 1

; 1383 : 			break;

	jmp	SHORT $L75857
$L75864:

; 1384 : 			
; 1385 : 		default: // everything else
; 1386 : 			sWord += szLookFor[nPos];

	mov	ecx, DWORD PTR _nPos$[ebp]
	mov	edx, DWORD PTR _szLookFor$[ebp]
	mov	ax, WORD PTR [edx+ecx*2]
	push	eax
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??YCString@@QAEABV0@G@Z			; CString::operator+=

; 1387 : 			
; 1388 : 			// also if its the last char then add it
; 1389 : 			bAddWord = (nPos == nLen - 1);

	mov	ecx, DWORD PTR _nLen$[ebp]
	sub	ecx, 1
	xor	edx, edx
	cmp	DWORD PTR _nPos$[ebp], ecx
	sete	dl
	mov	DWORD PTR _bAddWord$[ebp], edx
$L75857:

; 1392 : 		
; 1393 : 		if (bAddWord)

	cmp	DWORD PTR _bAddWord$[ebp], 0
	je	SHORT $L75865

; 1395 : 			Trim(sWord);

	push	0
	lea	eax, DWORD PTR _sWord$[ebp]
	push	eax
	call	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z	; Misc::Trim
	add	esp, 8

; 1396 : 			
; 1397 : 			if (!sWord.IsEmpty())

	lea	ecx, DWORD PTR _sWord$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75866

; 1398 : 				aWords.Add(sWord);

	lea	ecx, DWORD PTR _sWord$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _aWords$[ebp]
	call	?Add@CStringArray@@QAEHABVCString@@@Z	; CStringArray::Add
$L75866:

; 1399 : 			
; 1400 : 			sWord.Empty(); // next word

	lea	ecx, DWORD PTR _sWord$[ebp]
	call	?Empty@CString@@QAEXXZ			; CString::Empty
$L75865:

; 1402 : 	}

	jmp	$L75854
$L75855:

; 1403 : 	
; 1404 : 	return aWords.GetSize();

	mov	ecx, DWORD PTR _aWords$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR $T76783[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76783[ebp]

; 1405 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76784:
	lea	ecx, DWORD PTR _sWord$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76787:
	mov	eax, OFFSET FLAT:$T76786
	jmp	___CxxFrameHandler
text$x	ENDS
?ParseSearchString@Misc@@YAHPBGAAVCStringArray@@@Z ENDP	; Misc::ParseSearchString
PUBLIC	?Format@Misc@@YA?AVCString@@NHPBG@Z		; Misc::Format
_BSS	SEGMENT
	ALIGN	4

$SG75875 DB	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75878 DB	'%', 00H, 'f', 00H, '%', 00H, 's', 00H, 00H, 00H
	ORG $+2
$SG75880 DB	'%', 00H, '.', 00H, '*', 00H, 'f', 00H, '%', 00H, 's', 00H
	DB	00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76798	DD	019930520H
	DD	02H
	DD	FLAT:$T76800
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76800	DD	0ffffffffH
	DD	FLAT:$L76794
	DD	00H
	DD	FLAT:$L76793
xdata$x	ENDS
;	COMDAT ?Format@Misc@@YA?AVCString@@NHPBG@Z
_TEXT	SEGMENT
_dVal$ = 12
_nDecPlaces$ = 20
_szTrail$ = 24
___$ReturnUdt$ = 8
_szLocale$ = -20
_sValue$ = -16
$T76795 = -24
__$EHRec$ = -12
?Format@Misc@@YA?AVCString@@NHPBG@Z PROC NEAR		; Misc::Format, COMDAT

; 1408 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76799
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T76795[ebp], 0

; 1409 : 	char* szLocale = _strdup(setlocale(LC_NUMERIC, NULL)); // current locale

	mov	esi, esp
	push	0
	push	4
	call	DWORD PTR __imp__setlocale
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp___strdup
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _szLocale$[ebp], eax

; 1410 : 	setlocale(LC_NUMERIC, ""); // local default

	mov	esi, esp
	push	OFFSET FLAT:$SG75875
	push	4
	call	DWORD PTR __imp__setlocale
	add	esp, 8
	cmp	esi, esp
	call	__chkesp

; 1411 : 
; 1412 : 	CString sValue;

	lea	ecx, DWORD PTR _sValue$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1413 : 
; 1414 : 	if (nDecPlaces < 0)

	cmp	DWORD PTR _nDecPlaces$[ebp], 0
	jge	SHORT $L75877

; 1415 : 		sValue.Format(_T("%f%s"), dVal, szTrail);

	mov	eax, DWORD PTR _szTrail$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dVal$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _dVal$[ebp]
	push	edx
	push	OFFSET FLAT:$SG75878
	lea	eax, DWORD PTR _sValue$[ebp]
	push	eax
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 20					; 00000014H

; 1416 : 	else

	jmp	SHORT $L75879
$L75877:

; 1417 : 		sValue.Format(_T("%.*f%s"), nDecPlaces, dVal, szTrail);

	mov	ecx, DWORD PTR _szTrail$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dVal$[ebp+4]
	push	edx
	mov	eax, DWORD PTR _dVal$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nDecPlaces$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG75880
	lea	edx, DWORD PTR _sValue$[ebp]
	push	edx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 24					; 00000018H
$L75879:

; 1418 : 				
; 1419 : 	// restore locale
; 1420 : 	setlocale(LC_NUMERIC, szLocale);

	mov	esi, esp
	mov	eax, DWORD PTR _szLocale$[ebp]
	push	eax
	push	4
	call	DWORD PTR __imp__setlocale
	add	esp, 8
	cmp	esi, esp
	call	__chkesp

; 1421 : 	free(szLocale);

	mov	esi, esp
	mov	ecx, DWORD PTR _szLocale$[ebp]
	push	ecx
	call	DWORD PTR __imp__free
	add	esp, 4
	cmp	esi, esp
	call	__chkesp

; 1422 : 
; 1423 : 	return sValue;

	lea	edx, DWORD PTR _sValue$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T76795[ebp]
	or	al, 1
	mov	DWORD PTR $T76795[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sValue$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1424 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76793:
	lea	ecx, DWORD PTR _sValue$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76794:
	mov	eax, DWORD PTR $T76795[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76796
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76796:
	ret	0
$L76799:
	mov	eax, OFFSET FLAT:$T76798
	jmp	___CxxFrameHandler
text$x	ENDS
?Format@Misc@@YA?AVCString@@NHPBG@Z ENDP		; Misc::Format
PUBLIC	?Format@Misc@@YA?AVCString@@HPBG@Z		; Misc::Format
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75887 DB	'%', 00H, 'l', 00H, 'd', 00H, '%', 00H, 's', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76811	DD	019930520H
	DD	02H
	DD	FLAT:$T76813
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76813	DD	0ffffffffH
	DD	FLAT:$L76807
	DD	00H
	DD	FLAT:$L76806
xdata$x	ENDS
;	COMDAT ?Format@Misc@@YA?AVCString@@HPBG@Z
_TEXT	SEGMENT
$T76808 = -20
__$EHRec$ = -12
_nVal$ = 12
_szTrail$ = 16
___$ReturnUdt$ = 8
_sValue$ = -16
?Format@Misc@@YA?AVCString@@HPBG@Z PROC NEAR		; Misc::Format, COMDAT

; 1427 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76812
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T76808[ebp], 0

; 1428 : 	CString sValue;

	lea	ecx, DWORD PTR _sValue$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1429 : 	sValue.Format(_T("%ld%s"), nVal, szTrail);

	mov	eax, DWORD PTR _szTrail$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nVal$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG75887
	lea	edx, DWORD PTR _sValue$[ebp]
	push	edx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 16					; 00000010H

; 1430 : 
; 1431 : 	return sValue;

	lea	eax, DWORD PTR _sValue$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T76808[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76808[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sValue$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1432 : }

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
$L76806:
	lea	ecx, DWORD PTR _sValue$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76807:
	mov	eax, DWORD PTR $T76808[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76809
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76809:
	ret	0
$L76812:
	mov	eax, OFFSET FLAT:$T76811
	jmp	___CxxFrameHandler
text$x	ENDS
?Format@Misc@@YA?AVCString@@HPBG@Z ENDP			; Misc::Format
PUBLIC	?Format@Misc@@YA?AVCString@@KPBG@Z		; Misc::Format
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75894 DB	'%', 00H, 'l', 00H, 'u', 00H, '%', 00H, 's', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76824	DD	019930520H
	DD	02H
	DD	FLAT:$T76826
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76826	DD	0ffffffffH
	DD	FLAT:$L76820
	DD	00H
	DD	FLAT:$L76819
xdata$x	ENDS
;	COMDAT ?Format@Misc@@YA?AVCString@@KPBG@Z
_TEXT	SEGMENT
$T76821 = -20
__$EHRec$ = -12
_dwVal$ = 12
_szTrail$ = 16
___$ReturnUdt$ = 8
_sValue$ = -16
?Format@Misc@@YA?AVCString@@KPBG@Z PROC NEAR		; Misc::Format, COMDAT

; 1435 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76825
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T76821[ebp], 0

; 1436 : 	CString sValue;

	lea	ecx, DWORD PTR _sValue$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1437 : 	sValue.Format(_T("%lu%s"), dwVal, szTrail);

	mov	eax, DWORD PTR _szTrail$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwVal$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG75894
	lea	edx, DWORD PTR _sValue$[ebp]
	push	edx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 16					; 00000010H

; 1438 : 
; 1439 : 	return sValue;

	lea	eax, DWORD PTR _sValue$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T76821[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76821[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sValue$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1440 : }

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
$L76819:
	lea	ecx, DWORD PTR _sValue$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76820:
	mov	eax, DWORD PTR $T76821[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76822
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76822:
	ret	0
$L76825:
	mov	eax, OFFSET FLAT:$T76824
	jmp	___CxxFrameHandler
text$x	ENDS
?Format@Misc@@YA?AVCString@@KPBG@Z ENDP			; Misc::Format
PUBLIC	?FormatCost@Misc@@YA?AVCString@@N@Z		; Misc::FormatCost
EXTRN	__imp__GetCurrencyFormatW@24:NEAR
_BSS	SEGMENT
	ALIGN	4

$SG75902 DB	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75900 DB	'%', 00H, '.', 00H, '6', 00H, 'f', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76837	DD	019930520H
	DD	02H
	DD	FLAT:$T76839
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76839	DD	0ffffffffH
	DD	FLAT:$L76833
	DD	00H
	DD	FLAT:$L76832
xdata$x	ENDS
;	COMDAT ?FormatCost@Misc@@YA?AVCString@@N@Z
_TEXT	SEGMENT
$T76834 = -232
__$EHRec$ = -12
_dCost$ = 12
___$ReturnUdt$ = 8
_sValue$ = -16
_szLocale$ = -228
_BUFSIZE$ = -224
_szCost$ = -220
?FormatCost@Misc@@YA?AVCString@@N@Z PROC NEAR		; Misc::FormatCost, COMDAT

; 1443 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76838
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 220				; 000000dcH
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-232]
	mov	ecx, 55					; 00000037H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T76834[ebp], 0

; 1444 : 	CString sValue;

	lea	ecx, DWORD PTR _sValue$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1445 : 	sValue.Format(_T("%.6f"), dCost);

	mov	eax, DWORD PTR _dCost$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _dCost$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG75900
	lea	edx, DWORD PTR _sValue$[ebp]
	push	edx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 16					; 00000010H

; 1446 : 
; 1447 : 	char* szLocale = _strdup(setlocale(LC_NUMERIC, NULL)); // current locale

	mov	esi, esp
	push	0
	push	4
	call	DWORD PTR __imp__setlocale
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp___strdup
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _szLocale$[ebp], eax

; 1448 : 	setlocale(LC_NUMERIC, ""); // local default

	mov	esi, esp
	push	OFFSET FLAT:$SG75902
	push	4
	call	DWORD PTR __imp__setlocale
	add	esp, 8
	cmp	esi, esp
	call	__chkesp

; 1449 : 
; 1450 : 	const UINT BUFSIZE = 100;

	mov	DWORD PTR _BUFSIZE$[ebp], 100		; 00000064H

; 1451 : 	TCHAR szCost[BUFSIZE + 1];
; 1452 : 
; 1453 : 	GetCurrencyFormat(NULL, 0, sValue, NULL, szCost, BUFSIZE);

	mov	esi, esp
	push	100					; 00000064H
	lea	eax, DWORD PTR _szCost$[ebp]
	push	eax
	push	0
	lea	ecx, DWORD PTR _sValue$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	0
	push	0
	call	DWORD PTR __imp__GetCurrencyFormatW@24
	cmp	esi, esp
	call	__chkesp

; 1454 : 	sValue = szCost;

	lea	ecx, DWORD PTR _szCost$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sValue$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 1455 : 				
; 1456 : 	// restore locale
; 1457 : 	setlocale(LC_NUMERIC, szLocale);

	mov	esi, esp
	mov	edx, DWORD PTR _szLocale$[ebp]
	push	edx
	push	4
	call	DWORD PTR __imp__setlocale
	add	esp, 8
	cmp	esi, esp
	call	__chkesp

; 1458 : 	free(szLocale);

	mov	esi, esp
	mov	eax, DWORD PTR _szLocale$[ebp]
	push	eax
	call	DWORD PTR __imp__free
	add	esp, 4
	cmp	esi, esp
	call	__chkesp

; 1459 : 
; 1460 : 	return sValue;

	lea	ecx, DWORD PTR _sValue$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76834[ebp]
	or	edx, 1
	mov	DWORD PTR $T76834[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sValue$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1461 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 232				; 000000e8H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76832:
	lea	ecx, DWORD PTR _sValue$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76833:
	mov	eax, DWORD PTR $T76834[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76835
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76835:
	ret	0
$L76838:
	mov	eax, OFFSET FLAT:$T76837
	jmp	___CxxFrameHandler
text$x	ENDS
?FormatCost@Misc@@YA?AVCString@@N@Z ENDP		; Misc::FormatCost
PUBLIC	?GetKeyName@Misc@@YA?AVCString@@GH@Z		; Misc::GetKeyName
EXTRN	__imp__GetKeyNameTextW@12:NEAR
EXTRN	__imp__MapVirtualKeyW@8:NEAR
;	COMDAT ?GetKeyName@Misc@@YA?AVCString@@GH@Z
_TEXT	SEGMENT
$T76846 = -144
_wVirtKeyCode$ = 12
_bExtended$ = 16
___$ReturnUdt$ = 8
_KEYNAMELEN$ = -140
_szKeyName$ = -132
_wScanCode$ = -136
_lParam$ = -4
?GetKeyName@Misc@@YA?AVCString@@GH@Z PROC NEAR		; Misc::GetKeyName, COMDAT

; 1464 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 144				; 00000090H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-144]
	mov	ecx, 36					; 00000024H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T76846[ebp], 0

; 1465 : 	const int KEYNAMELEN = 64;

	mov	DWORD PTR _KEYNAMELEN$[ebp], 64		; 00000040H

; 1466 : 	TCHAR szKeyName[64] = { 0 };

	mov	WORD PTR _szKeyName$[ebp], 0
	mov	ecx, 31					; 0000001fH
	xor	eax, eax
	lea	edi, DWORD PTR _szKeyName$[ebp+2]
	rep stosd
	stosw

; 1467 : 	
; 1468 : 	WORD wScanCode = (WORD)MapVirtualKey(wVirtKeyCode, 0);

	mov	esi, esp
	push	0
	mov	eax, DWORD PTR _wVirtKeyCode$[ebp]
	and	eax, 65535				; 0000ffffH
	push	eax
	call	DWORD PTR __imp__MapVirtualKeyW@8
	cmp	esi, esp
	call	__chkesp
	mov	WORD PTR _wScanCode$[ebp], ax

; 1469 : 	
; 1470 : 	// build lParam to send to GetKeyNameText
; 1471 : 	LPARAM lParam = (wScanCode * 0x00010000);

	mov	ecx, DWORD PTR _wScanCode$[ebp]
	and	ecx, 65535				; 0000ffffH
	shl	ecx, 16					; 00000010H
	mov	DWORD PTR _lParam$[ebp], ecx

; 1472 : 	
; 1473 : 	if (bExtended)

	cmp	DWORD PTR _bExtended$[ebp], 0
	je	SHORT $L75915

; 1474 : 		lParam += 0x01000000;

	mov	edx, DWORD PTR _lParam$[ebp]
	add	edx, 16777216				; 01000000H
	mov	DWORD PTR _lParam$[ebp], edx
$L75915:

; 1475 : 	
; 1476 : 	GetKeyNameText(lParam, szKeyName, KEYNAMELEN);

	mov	esi, esp
	push	64					; 00000040H
	lea	eax, DWORD PTR _szKeyName$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lParam$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetKeyNameTextW@12
	cmp	esi, esp
	call	__chkesp

; 1477 : 	return szKeyName;

	lea	edx, DWORD PTR _szKeyName$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	eax, DWORD PTR $T76846[ebp]
	or	al, 1
	mov	DWORD PTR $T76846[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1478 : }

	pop	edi
	pop	esi
	add	esp, 144				; 00000090H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetKeyName@Misc@@YA?AVCString@@GH@Z ENDP		; Misc::GetKeyName
_TEXT	ENDS
PUBLIC	?KeyIsPressed@Misc@@YAHK@Z			; Misc::KeyIsPressed
EXTRN	__imp__GetKeyState@4:NEAR
;	COMDAT ?KeyIsPressed@Misc@@YAHK@Z
_TEXT	SEGMENT
_dwVirtKey$ = 8
?KeyIsPressed@Misc@@YAHK@Z PROC NEAR			; Misc::KeyIsPressed, COMDAT

; 1481 : { 

	push	ebp
	mov	ebp, esp
	push	esi

; 1482 : 	return (GetKeyState(dwVirtKey) & 0x8000); 

	mov	esi, esp
	mov	eax, DWORD PTR _dwVirtKey$[ebp]
	push	eax
	call	DWORD PTR __imp__GetKeyState@4
	cmp	esi, esp
	call	__chkesp
	movsx	eax, ax
	and	eax, 32768				; 00008000H

; 1483 : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?KeyIsPressed@Misc@@YAHK@Z ENDP				; Misc::KeyIsPressed
_TEXT	ENDS
PUBLIC	?IsCursorKeyPressed@Misc@@YAHK@Z		; Misc::IsCursorKeyPressed
;	COMDAT ?IsCursorKeyPressed@Misc@@YAHK@Z
_TEXT	SEGMENT
_dwKeys$ = 8
?IsCursorKeyPressed@Misc@@YAHK@Z PROC NEAR		; Misc::IsCursorKeyPressed, COMDAT

; 1486 : {

	push	ebp
	mov	ebp, esp

; 1487 : 	if (dwKeys & MKC_LEFTRIGHT)

	mov	eax, DWORD PTR _dwKeys$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75923

; 1490 : 			KeyIsPressed(VK_HOME) || KeyIsPressed(VK_END))

	push	39					; 00000027H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75924
	push	37					; 00000025H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75924
	push	36					; 00000024H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75924
	push	35					; 00000023H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	je	SHORT $L75923
$L75924:

; 1491 : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L75921
$L75923:

; 1493 : 	
; 1494 : 	if (dwKeys & MKC_UPDOWN)

	mov	ecx, DWORD PTR _dwKeys$[ebp]
	and	ecx, 2
	test	ecx, ecx
	je	SHORT $L75926

; 1497 : 			KeyIsPressed(VK_UP) || KeyIsPressed(VK_PRIOR))

	push	34					; 00000022H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75927
	push	40					; 00000028H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75927
	push	38					; 00000026H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75927
	push	33					; 00000021H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	je	SHORT $L75926
$L75927:

; 1498 : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L75921
$L75926:

; 1500 : 	
; 1501 : 	// else 
; 1502 : 	return FALSE;

	xor	eax, eax
$L75921:

; 1503 : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?IsCursorKeyPressed@Misc@@YAHK@Z ENDP			; Misc::IsCursorKeyPressed
_TEXT	ENDS
PUBLIC	?IsCursorKey@Misc@@YAHKK@Z			; Misc::IsCursorKey
;	COMDAT ?IsCursorKey@Misc@@YAHKK@Z
_TEXT	SEGMENT
_dwVirtKey$ = 8
_dwKeys$ = 12
?IsCursorKey@Misc@@YAHKK@Z PROC NEAR			; Misc::IsCursorKey, COMDAT

; 1506 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1507 : 	if (dwKeys & MKC_LEFTRIGHT)

	mov	eax, DWORD PTR _dwKeys$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75934

; 1510 : 		{

	mov	ecx, DWORD PTR _dwVirtKey$[ebp]
	mov	DWORD PTR -4+[ebp], ecx
	cmp	DWORD PTR -4+[ebp], 35			; 00000023H
	jb	SHORT $L75934
	cmp	DWORD PTR -4+[ebp], 37			; 00000025H
	jbe	SHORT $L75937
	cmp	DWORD PTR -4+[ebp], 39			; 00000027H
	je	SHORT $L75937
	jmp	SHORT $L75934
$L75937:

; 1511 : 		case VK_RIGHT:
; 1512 : 		case VK_LEFT:
; 1513 : 		case VK_HOME:
; 1514 : 		case VK_END:
; 1515 : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L75931
$L75934:

; 1518 : 	
; 1519 : 	if (dwKeys & MKC_UPDOWN)

	mov	edx, DWORD PTR _dwKeys$[ebp]
	and	edx, 2
	test	edx, edx
	je	SHORT $L75940

; 1522 : 		{

	mov	eax, DWORD PTR _dwVirtKey$[ebp]
	mov	DWORD PTR -8+[ebp], eax
	mov	ecx, DWORD PTR -8+[ebp]
	sub	ecx, 33					; 00000021H
	mov	DWORD PTR -8+[ebp], ecx
	cmp	DWORD PTR -8+[ebp], 7
	ja	SHORT $L75940
	mov	edx, DWORD PTR -8+[ebp]
	jmp	DWORD PTR $L76854[edx*4]
$L75943:

; 1523 : 		case VK_NEXT:  
; 1524 : 		case VK_DOWN:
; 1525 : 		case VK_UP:
; 1526 : 		case VK_PRIOR: 
; 1527 : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L75931
$L75940:

; 1530 : 	
; 1531 : 	// else 
; 1532 : 	return FALSE;

	xor	eax, eax
$L75931:

; 1533 : }

	mov	esp, ebp
	pop	ebp
	ret	0
$L76854:
	DD	$L75943
	DD	$L75943
	DD	$L75940
	DD	$L75940
	DD	$L75940
	DD	$L75943
	DD	$L75940
	DD	$L75943
?IsCursorKey@Misc@@YAHKK@Z ENDP				; Misc::IsCursorKey
_TEXT	ENDS
PUBLIC	?ModKeysArePressed@Misc@@YAHK@Z			; Misc::ModKeysArePressed
;	COMDAT ?ModKeysArePressed@Misc@@YAHK@Z
_TEXT	SEGMENT
_dwKeys$ = 8
?ModKeysArePressed@Misc@@YAHK@Z PROC NEAR		; Misc::ModKeysArePressed, COMDAT

; 1536 : {

	push	ebp
	mov	ebp, esp

; 1537 : 	// test for failure
; 1538 : 	if (dwKeys & MKS_CTRL)

	mov	eax, DWORD PTR _dwKeys$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75947

; 1540 : 		if (!KeyIsPressed(VK_CONTROL))

	push	17					; 00000011H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75948

; 1541 : 			return FALSE;

	xor	eax, eax
	jmp	SHORT $L75946
$L75948:

; 1543 : 	else if (KeyIsPressed(VK_CONTROL))

	jmp	SHORT $L75950
$L75947:
	push	17					; 00000011H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	je	SHORT $L75950

; 1545 : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75946
$L75950:

; 1547 : 
; 1548 : 	if (dwKeys & MKS_SHIFT)

	mov	ecx, DWORD PTR _dwKeys$[ebp]
	and	ecx, 2
	test	ecx, ecx
	je	SHORT $L75951

; 1550 : 		if (!KeyIsPressed(VK_SHIFT))

	push	16					; 00000010H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75952

; 1551 : 			return FALSE;

	xor	eax, eax
	jmp	SHORT $L75946
$L75952:

; 1553 : 	else if (KeyIsPressed(VK_SHIFT))

	jmp	SHORT $L75954
$L75951:
	push	16					; 00000010H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	je	SHORT $L75954

; 1555 : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75946
$L75954:

; 1557 : 
; 1558 : 	if (dwKeys & MKS_ALT)

	mov	edx, DWORD PTR _dwKeys$[ebp]
	and	edx, 4
	test	edx, edx
	je	SHORT $L75955

; 1560 : 		if (!KeyIsPressed(VK_MENU))

	push	18					; 00000012H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75956

; 1561 : 			return FALSE;

	xor	eax, eax
	jmp	SHORT $L75946
$L75956:

; 1563 : 	else if (KeyIsPressed(VK_MENU))

	jmp	SHORT $L75958
$L75955:
	push	18					; 00000012H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	je	SHORT $L75958

; 1565 : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75946
$L75958:

; 1567 : 
; 1568 : 	return TRUE;

	mov	eax, 1
$L75946:

; 1569 : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?ModKeysArePressed@Misc@@YAHK@Z ENDP			; Misc::ModKeysArePressed
_TEXT	ENDS
PUBLIC	?ModifyFlags@Misc@@YAHKKK@Z			; Misc::ModifyFlags
;	COMDAT ?ModifyFlags@Misc@@YAHKKK@Z
_TEXT	SEGMENT
_dwFlags$ = 8
_dwRemove$ = 12
_dwAdd$ = 16
_dwOldFlags$ = -4
?ModifyFlags@Misc@@YAHKKK@Z PROC NEAR			; Misc::ModifyFlags, COMDAT

; 1572 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1573 : 	DWORD dwOldFlags = dwFlags;

	mov	eax, DWORD PTR _dwFlags$[ebp]
	mov	DWORD PTR _dwOldFlags$[ebp], eax

; 1574 : 	
; 1575 : 	dwFlags |= dwAdd;

	mov	ecx, DWORD PTR _dwFlags$[ebp]
	or	ecx, DWORD PTR _dwAdd$[ebp]
	mov	DWORD PTR _dwFlags$[ebp], ecx

; 1576 : 	dwFlags &= ~dwRemove;

	mov	edx, DWORD PTR _dwRemove$[ebp]
	not	edx
	mov	eax, DWORD PTR _dwFlags$[ebp]
	and	eax, edx
	mov	DWORD PTR _dwFlags$[ebp], eax

; 1577 : 	
; 1578 : 	return (dwFlags != dwOldFlags);

	mov	ecx, DWORD PTR _dwFlags$[ebp]
	xor	eax, eax
	cmp	ecx, DWORD PTR _dwOldFlags$[ebp]
	setne	al

; 1579 : }

	mov	esp, ebp
	pop	ebp
	ret	0
?ModifyFlags@Misc@@YAHKKK@Z ENDP			; Misc::ModifyFlags
_TEXT	ENDS
PUBLIC	?MakeKey@Misc@@YA?AVCString@@ABV2@HPBG@Z	; Misc::MakeKey
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75978 DB	'%', 00H, 's', 00H, '\', 00H, '%', 00H, 's', 00H, 00H, 00H
$SG75979 DB	'\', 00H, 00H, 00H
$SG75980 DB	'\', 00H, '\', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
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
;	COMDAT ?MakeKey@Misc@@YA?AVCString@@ABV2@HPBG@Z
_TEXT	SEGMENT
$T76864 = -24
__$EHRec$ = -12
_sFormat$ = 12
_nKeyVal$ = 16
_szParentKey$ = 20
___$ReturnUdt$ = 8
_sKey$ = -16
_sTemp$75977 = -20
?MakeKey@Misc@@YA?AVCString@@ABV2@HPBG@Z PROC NEAR	; Misc::MakeKey, COMDAT

; 1582 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76868
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T76864[ebp], 0
$L75971:

; 1583 : 	ASSERT(!sFormat.IsEmpty());

	mov	ecx, DWORD PTR _sFormat$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L75974
	push	1583					; 0000062fH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75974
	int	3
$L75974:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75971

; 1584 : 
; 1585 : 	CString sKey;

	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1586 : 	sKey.Format(sFormat, nKeyVal);

	mov	ecx, DWORD PTR _nKeyVal$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sFormat$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	edx, DWORD PTR _sKey$[ebp]
	push	edx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 12					; 0000000cH

; 1587 : 
; 1588 : 	if (szParentKey && *szParentKey)

	cmp	DWORD PTR _szParentKey$[ebp], 0
	je	SHORT $L75976
	mov	eax, DWORD PTR _szParentKey$[ebp]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax]
	test	ecx, ecx
	je	SHORT $L75976

; 1590 : 		CString sTemp;

	lea	ecx, DWORD PTR _sTemp$75977[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 1591 : 		sTemp.Format(_T("%s\\%s"), szParentKey, sKey);

	mov	edx, DWORD PTR _sKey$[ebp]
	push	edx
	mov	eax, DWORD PTR _szParentKey$[ebp]
	push	eax
	push	OFFSET FLAT:$SG75978
	lea	ecx, DWORD PTR _sTemp$75977[ebp]
	push	ecx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 16					; 00000010H

; 1592 : 
; 1593 : 		// remove doubled-up back slashes
; 1594 : 		sTemp.Replace(_T("\\\\"), _T("\\"));

	push	OFFSET FLAT:$SG75979
	push	OFFSET FLAT:$SG75980
	lea	ecx, DWORD PTR _sTemp$75977[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 1595 : 		sKey = sTemp;

	lea	edx, DWORD PTR _sTemp$75977[ebp]
	push	edx
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=

; 1596 : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sTemp$75977[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75976:

; 1597 : 
; 1598 : 	return sKey;

	lea	eax, DWORD PTR _sKey$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T76864[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76864[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1599 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
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
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76862:
	lea	ecx, DWORD PTR _sTemp$75977[ebp]
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
?MakeKey@Misc@@YA?AVCString@@ABV2@HPBG@Z ENDP		; Misc::MakeKey
PUBLIC	?MakeKey@Misc@@YA?AVCString@@ABV2@PBG1@Z	; Misc::MakeKey
;	COMDAT xdata$x
; File D:\_CODE\Shared\Misc.cpp
xdata$x	SEGMENT
$T76880	DD	019930520H
	DD	02H
	DD	FLAT:$T76882
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76882	DD	0ffffffffH
	DD	FLAT:$L76876
	DD	00H
	DD	FLAT:$L76875
xdata$x	ENDS
;	COMDAT ?MakeKey@Misc@@YA?AVCString@@ABV2@PBG1@Z
_TEXT	SEGMENT
$T76877 = -20
__$EHRec$ = -12
_sFormat$ = 12
_szKeyVal$ = 16
_szParentKey$ = 20
___$ReturnUdt$ = 8
_sKey$ = -16
?MakeKey@Misc@@YA?AVCString@@ABV2@PBG1@Z PROC NEAR	; Misc::MakeKey, COMDAT

; 1602 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76881
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T76877[ebp], 0

; 1603 : 	CString sKey;

	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1604 : 	sKey.Format(sFormat, szKeyVal);

	mov	eax, DWORD PTR _szKeyVal$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sFormat$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sKey$[ebp]
	push	ecx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 12					; 0000000cH

; 1605 : 
; 1606 : 	return MakeKey(sKey, 0, szParentKey);

	mov	edx, DWORD PTR _szParentKey$[ebp]
	push	edx
	push	0
	lea	eax, DWORD PTR _sKey$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	call	?MakeKey@Misc@@YA?AVCString@@ABV2@HPBG@Z ; Misc::MakeKey
	add	esp, 16					; 00000010H
	mov	edx, DWORD PTR $T76877[ebp]
	or	edx, 1
	mov	DWORD PTR $T76877[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1607 : }

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
$L76875:
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76876:
	mov	eax, DWORD PTR $T76877[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76878
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76878:
	ret	0
$L76881:
	mov	eax, OFFSET FLAT:$T76880
	jmp	___CxxFrameHandler
text$x	ENDS
?MakeKey@Misc@@YA?AVCString@@ABV2@PBG1@Z ENDP		; Misc::MakeKey
EXTRN	__imp___wcsicmp:NEAR
_BSS	SEGMENT
	ALIGN	4

_?hShlwapi@?1??NaturalCompare@Misc@@YAHPBG0@Z@4PAUHINSTANCE__@@A DD 01H DUP (?)
_?$S302@?1??NaturalCompare@Misc@@YAHPBG0@Z@4EA DB 01H DUP (?)
	ALIGN	4

_?pFn@?1??NaturalCompare@Misc@@YAHPBG0@Z@4P6GH00@ZA DD 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Misc.cpp
data	SEGMENT
$SG75998 DB	'S', 00H, 'h', 00H, 'l', 00H, 'w', 00H, 'a', 00H, 'p', 00H
	DB	'i', 00H, '.', 00H, 'd', 00H, 'l', 00H, 'l', 00H, 00H, 00H
$SG76006 DB	'StrCmpLogicalW', 00H
data	ENDS
;	COMDAT ?NaturalCompare@Misc@@YAHPBG0@Z
_TEXT	SEGMENT
_szString1$ = 8
_szString2$ = 12
?NaturalCompare@Misc@@YAHPBG0@Z PROC NEAR		; Misc::NaturalCompare, COMDAT

; 1610 : {

	push	ebp
	mov	ebp, esp
	push	esi

; 1611 : #ifdef _UNICODE
; 1612 : 	// initialize once only per session
; 1613 : 	static HMODULE hShlwapi = ::LoadLibrary(_T("Shlwapi.dll"));

	xor	eax, eax
	mov	al, BYTE PTR _?$S302@?1??NaturalCompare@Misc@@YAHPBG0@Z@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75997
	mov	cl, BYTE PTR _?$S302@?1??NaturalCompare@Misc@@YAHPBG0@Z@4EA
	or	cl, 1
	mov	BYTE PTR _?$S302@?1??NaturalCompare@Misc@@YAHPBG0@Z@4EA, cl
	mov	esi, esp
	push	OFFSET FLAT:$SG75998
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?hShlwapi@?1??NaturalCompare@Misc@@YAHPBG0@Z@4PAUHINSTANCE__@@A, eax
$L75997:

; 1614 : 	   
; 1615 : 	typedef int (WINAPI *PFNSTRCMPLOGICAL)(PCWSTR, PCWSTR);
; 1616 : 	static PFNSTRCMPLOGICAL pFn = (PFNSTRCMPLOGICAL)::GetProcAddress(hShlwapi, "StrCmpLogicalW");

	xor	edx, edx
	mov	dl, BYTE PTR _?$S302@?1??NaturalCompare@Misc@@YAHPBG0@Z@4EA
	and	edx, 2
	test	edx, edx
	jne	SHORT $L76005
	mov	al, BYTE PTR _?$S302@?1??NaturalCompare@Misc@@YAHPBG0@Z@4EA
	or	al, 2
	mov	BYTE PTR _?$S302@?1??NaturalCompare@Misc@@YAHPBG0@Z@4EA, al
	mov	esi, esp
	push	OFFSET FLAT:$SG76006
	mov	ecx, DWORD PTR _?hShlwapi@?1??NaturalCompare@Misc@@YAHPBG0@Z@4PAUHINSTANCE__@@A
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?pFn@?1??NaturalCompare@Misc@@YAHPBG0@Z@4P6GH00@ZA, eax
$L76005:

; 1617 : 	
; 1618 : 	if (pFn)

	cmp	DWORD PTR _?pFn@?1??NaturalCompare@Misc@@YAHPBG0@Z@4P6GH00@ZA, 0
	je	SHORT $L76007

; 1619 : 		return pFn(szString1, szString2);

	mov	esi, esp
	mov	edx, DWORD PTR _szString2$[ebp]
	push	edx
	mov	eax, DWORD PTR _szString1$[ebp]
	push	eax
	call	DWORD PTR _?pFn@?1??NaturalCompare@Misc@@YAHPBG0@Z@4P6GH00@ZA
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75992
$L76007:

; 1620 : #endif
; 1621 : 	
; 1622 : 	// all else
; 1623 : 	return _tcsicmp(szString1, szString2);

	mov	esi, esp
	mov	ecx, DWORD PTR _szString2$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szString1$[ebp]
	push	edx
	call	DWORD PTR __imp___wcsicmp
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
$L75992:

; 1624 : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?NaturalCompare@Misc@@YAHPBG0@Z ENDP			; Misc::NaturalCompare
_TEXT	ENDS
PUBLIC	?StringSortProc@@YAHPBX0@Z			; StringSortProc
;	COMDAT ?StringSortProc@@YAHPBX0@Z
_TEXT	SEGMENT
_v1$ = 8
_v2$ = 12
_pStr1$ = -4
_pStr2$ = -8
?StringSortProc@@YAHPBX0@Z PROC NEAR			; StringSortProc, COMDAT

; 1630 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1631 : 	CString* pStr1 = (CString*)v1;

	mov	eax, DWORD PTR _v1$[ebp]
	mov	DWORD PTR _pStr1$[ebp], eax

; 1632 : 	CString* pStr2 = (CString*)v2;

	mov	ecx, DWORD PTR _v2$[ebp]
	mov	DWORD PTR _pStr2$[ebp], ecx

; 1633 : 
; 1634 : 	return Misc::NaturalCompare(*pStr1, *pStr2);

	mov	ecx, DWORD PTR _pStr2$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _pStr1$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?NaturalCompare@Misc@@YAHPBG0@Z		; Misc::NaturalCompare
	add	esp, 8

; 1635 : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?StringSortProc@@YAHPBX0@Z ENDP				; StringSortProc
_TEXT	ENDS
PUBLIC	?SortArray@Misc@@YAXAAVCStringArray@@P6AHPBX1@Z@Z ; Misc::SortArray
EXTRN	__imp__qsort:NEAR
EXTRN	?GetData@CStringArray@@QAEPAVCString@@XZ:NEAR	; CStringArray::GetData
;	COMDAT ?SortArray@Misc@@YAXAAVCStringArray@@P6AHPBX1@Z@Z
_TEXT	SEGMENT
_array$ = 8
_pSortProc$ = 12
?SortArray@Misc@@YAXAAVCStringArray@@P6AHPBX1@Z@Z PROC NEAR ; Misc::SortArray, COMDAT

; 1638 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1639 : 	qsort(array.GetData(), array.GetSize(), sizeof(CString*), pSortProc ? pSortProc : StringSortProc);

	cmp	DWORD PTR _pSortProc$[ebp], 0
	je	SHORT $L76892
	mov	eax, DWORD PTR _pSortProc$[ebp]
	mov	DWORD PTR -4+[ebp], eax
	jmp	SHORT $L76893
$L76892:
	mov	DWORD PTR -4+[ebp], OFFSET FLAT:?StringSortProc@@YAHPBX0@Z ; StringSortProc
$L76893:
	mov	esi, esp
	mov	ecx, DWORD PTR -4+[ebp]
	push	ecx
	push	4
	mov	ecx, DWORD PTR _array$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	push	eax
	mov	ecx, DWORD PTR _array$[ebp]
	call	?GetData@CStringArray@@QAEPAVCString@@XZ ; CStringArray::GetData
	push	eax
	call	DWORD PTR __imp__qsort
	add	esp, 16					; 00000010H
	cmp	esi, esp
	call	__chkesp

; 1640 : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?SortArray@Misc@@YAXAAVCStringArray@@P6AHPBX1@Z@Z ENDP	; Misc::SortArray
_TEXT	ENDS
PUBLIC	?DWordSortProc@@YAHPBX0@Z			; DWordSortProc
;	COMDAT ?DWordSortProc@@YAHPBX0@Z
_TEXT	SEGMENT
_v1$ = 8
_v2$ = 12
_pDW1$ = -4
_pDW2$ = -8
?DWordSortProc@@YAHPBX0@Z PROC NEAR			; DWordSortProc, COMDAT

; 1646 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1647 : 	DWORD* pDW1 = (DWORD*)v1;

	mov	eax, DWORD PTR _v1$[ebp]
	mov	DWORD PTR _pDW1$[ebp], eax

; 1648 : 	DWORD* pDW2 = (DWORD*)v2;

	mov	ecx, DWORD PTR _v2$[ebp]
	mov	DWORD PTR _pDW2$[ebp], ecx

; 1649 : 
; 1650 : 	if (*pDW1 < *pDW2)

	mov	edx, DWORD PTR _pDW1$[ebp]
	mov	eax, DWORD PTR _pDW2$[ebp]
	mov	ecx, DWORD PTR [edx]
	cmp	ecx, DWORD PTR [eax]
	jae	SHORT $L76029

; 1651 : 		return -1;

	or	eax, -1
	jmp	SHORT $L76024
$L76029:

; 1652 : 	
; 1653 : 	else if (*pDW1 > *pDW2)

	mov	edx, DWORD PTR _pDW1$[ebp]
	mov	eax, DWORD PTR _pDW2$[ebp]
	mov	ecx, DWORD PTR [edx]
	cmp	ecx, DWORD PTR [eax]
	jbe	SHORT $L76031

; 1654 : 		return 1;

	mov	eax, 1
	jmp	SHORT $L76024
$L76031:

; 1655 : 
; 1656 : 	// else
; 1657 : 	return 0;

	xor	eax, eax
$L76024:

; 1658 : }

	mov	esp, ebp
	pop	ebp
	ret	0
?DWordSortProc@@YAHPBX0@Z ENDP				; DWordSortProc
_TEXT	ENDS
PUBLIC	?SortArray@Misc@@YAXAAVCDWordArray@@P6AHPBX1@Z@Z ; Misc::SortArray
EXTRN	?GetData@CDWordArray@@QAEPAKXZ:NEAR		; CDWordArray::GetData
;	COMDAT ?SortArray@Misc@@YAXAAVCDWordArray@@P6AHPBX1@Z@Z
_TEXT	SEGMENT
_array$ = 8
_pSortProc$ = 12
?SortArray@Misc@@YAXAAVCDWordArray@@P6AHPBX1@Z@Z PROC NEAR ; Misc::SortArray, COMDAT

; 1661 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1662 : 	qsort(array.GetData(), array.GetSize(), sizeof(DWORD*), pSortProc ? pSortProc : DWordSortProc);

	cmp	DWORD PTR _pSortProc$[ebp], 0
	je	SHORT $L76898
	mov	eax, DWORD PTR _pSortProc$[ebp]
	mov	DWORD PTR -4+[ebp], eax
	jmp	SHORT $L76899
$L76898:
	mov	DWORD PTR -4+[ebp], OFFSET FLAT:?DWordSortProc@@YAHPBX0@Z ; DWordSortProc
$L76899:
	mov	esi, esp
	mov	ecx, DWORD PTR -4+[ebp]
	push	ecx
	push	4
	mov	ecx, DWORD PTR _array$[ebp]
	call	?GetSize@CDWordArray@@QBEHXZ		; CDWordArray::GetSize
	push	eax
	mov	ecx, DWORD PTR _array$[ebp]
	call	?GetData@CDWordArray@@QAEPAKXZ		; CDWordArray::GetData
	push	eax
	call	DWORD PTR __imp__qsort
	add	esp, 16					; 00000010H
	cmp	esi, esp
	call	__chkesp

; 1663 : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?SortArray@Misc@@YAXAAVCDWordArray@@P6AHPBX1@Z@Z ENDP	; Misc::SortArray
_TEXT	ENDS
END

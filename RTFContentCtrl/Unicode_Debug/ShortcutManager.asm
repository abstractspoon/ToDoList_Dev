	TITLE	D:\_CODE\Shared\ShortcutManager.cpp
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
;	COMDAT ??_C@_0BL@KPKN@D?3?2_CODE?2Shared?2Subclass?4h?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0BP@DMPF@D?3?2_CODE?2Shared?2rcctrlparser?4h?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0BN@CJLN@D?3?2_CODE?2Shared?2runtimedlg?4h?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_00A@?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0EK@MDCF@C?3?2Program?5Files?5?$CIx86?$CJ?2Microsoft@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_05KIFI@with?5?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_09LOPF@?5elements?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_03DANH@?6?7?$FL?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_04MNLE@?$FN?5?$DN?5?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_01BJG@?6?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
text$AFX_COL1	SEGMENT PARA USE32 PUBLIC ''
text$AFX_COL1	ENDS
;	COMDAT ?IsValid@CSubclassWnd@@UBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetCWnd@CSubclassWnd@@QBEPAVCWnd@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetExStyle@CSubclassWnd@@QBEKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetStyle@CSubclassWnd@@QBEKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Invalidate@CSubclassWnd@@QBEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsWindowEnabled@CSubclassWnd@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsWindowVisible@CSubclassWnd@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PreDetachWindow@CSubclassWnd@@MAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PostDetachWindow@CSubclassWnd@@MAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsHooked@CSubclassWnd@@MBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsValidHook@CSubclassWnd@@MBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Redraw@CSubclassWnd@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScWindowProc@CSubclasser@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTopSubclasser@CSubclasser@@MAEPAV1@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScPreDetachWindow@CSubclasser@@MAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScPostDetachWindow@CSubclasser@@MAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?data@CPlex@@QAEPAXXZ
text$AFX_COL1	SEGMENT PARA USE32 PUBLIC ''
text$AFX_COL1	ENDS
;	COMDAT ??2@YAPAXIPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??3@YAXPAX0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
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
;	COMDAT ??0RTCONTROL@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0RGBX@@QAE@EEE@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4RGBX@@QAEABV0@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Luminance@RGBX@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MakeGray@RGBX@@QAEXNNN@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Hue2Triplet@RGBX@@KAEMMM@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CShortcutManager@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCShortcutManager@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CShortcutManager@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Initialize@CShortcutManager@@QAEHPAVCWnd@@PBVIPreferences@@GG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Release@CShortcutManager@@QAEHPAVIPreferences@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetShortcut@CShortcutManager@@QAEXIGG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetShortcut@CShortcutManager@@QAEXIK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddShortcut@CShortcutManager@@QAEHIK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddShortcut@CShortcutManager@@QAEHIGG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ValidateModifiers@CShortcutManager@@QBEGGG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetShortcut@CShortcutManager@@IBEKGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessMessage@CShortcutManager@@QBEIPBUtagMSG@@PAK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsEditShortcut@CShortcutManager@@KAHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?WindowProc@CShortcutManager@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PrepareMenuItems@CShortcutManager@@IBEXPAVCMenu@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?BuildMapping@CShortcutManager@@QAEHIAAVCStringArray@@D@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?BuildMapping@CShortcutManager@@IAEHPAVCMenu@@PBGAAVCStringArray@@D@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetCommandID@CShortcutManager@@QAEIK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetShortcut@CShortcutManager@@QBEKI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DeleteShortcut@CShortcutManager@@QAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetShortcutTextByCmd@CShortcutManager@@QAE?AVCString@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetShortcutText@CShortcutManager@@SA?AVCString@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadSettings@CShortcutManager@@QAEXPBVIPreferences@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SaveSettings@CShortcutManager@@QBEXPAVIPreferences@@@Z
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
;	COMDAT ??0?$CMap@KKIAAI@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lookup@?$CMap@KKIAAI@@QBEHKAAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CMap@KKIAAI@@QAEAAIK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveKey@?$CMap@KKIAAI@@QAEHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CMap@KKIAAI@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CMap@KKIAAI@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CMap@KKIAAI@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CMap@KKIAAI@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$CMap@IIKAAK@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetCount@?$CMap@IIKAAK@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lookup@?$CMap@IIKAAK@@QBEHIAAK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CMap@IIKAAK@@QAEAAKI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveKey@?$CMap@IIKAAK@@QAEHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetStartPosition@?$CMap@IIKAAK@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNextAssoc@?$CMap@IIKAAK@@QBEXAAPAU__POSITION@@AAIAAK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CMap@IIKAAK@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CMap@IIKAAK@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CMap@IIKAAK@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CMap@IIKAAK@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CList@VRTCONTROL@@AAV1@@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CList@VRTCONTROL@@AAV1@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CList@VRTCONTROL@@AAV1@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CList@VRTCONTROL@@AAV1@@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CMap@IIHH@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CMap@IIHH@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CMap@IIHH@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CMap@IIHH@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@KKIAAI@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@IIKAAK@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CList@VRTCONTROL@@AAV1@@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1RTCONTROL@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@IIHH@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@KKIAAI@@QAEXKAAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@KKIAAI@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetStartPosition@?$CMap@KKIAAI@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNextAssoc@?$CMap@KKIAAI@@QBEXAAPAU__POSITION@@AAKAAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InitHashTable@?$CMap@KKIAAI@@QAEXIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewAssoc@?$CMap@KKIAAI@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FreeAssoc@?$CMap@KKIAAI@@IAEXPAUCAssoc@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAssocAt@?$CMap@KKIAAI@@IBEPAUCAssoc@1@KAAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HashKey@@YGIK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CompareElements@@YGHPBK0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAKH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBKH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@IIKAAK@@QAEXIAAK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@IIKAAK@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InitHashTable@?$CMap@IIKAAK@@QAEXIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewAssoc@?$CMap@IIKAAK@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FreeAssoc@?$CMap@IIKAAK@@IAEXPAUCAssoc@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAssocAt@?$CMap@IIKAAK@@IBEPAUCAssoc@1@IAAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HashKey@@YGII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CompareElements@@YGHPBI0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddTail@?$CList@VRTCONTROL@@AAV1@@@QAEPAU__POSITION@@AAVRTCONTROL@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CList@VRTCONTROL@@AAV1@@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetHeadPosition@?$CList@VRTCONTROL@@AAV1@@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNext@?$CList@VRTCONTROL@@AAV1@@@QAEAAVRTCONTROL@@AAPAU__POSITION@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAVRTCONTROL@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBVRTCONTROL@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@IIHH@@QAEXIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@IIHH@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetStartPosition@?$CMap@IIHH@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNextAssoc@?$CMap@IIHH@@QBEXAAPAU__POSITION@@AAIAAH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4RTCONTROL@@QAEAAV0@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAKH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAKH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewNode@?$CList@VRTCONTROL@@AAV1@@@IAEPAUCNode@1@PAU21@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAVRTCONTROL@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CMap@IIHH@@QAEAAHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GRTCONTROL@@QAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAVRTCONTROL@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InitHashTable@?$CMap@IIHH@@QAEXIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewAssoc@?$CMap@IIHH@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAssocAt@?$CMap@IIHH@@IBEPAUCAssoc@1@IAAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAHH@Z
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
;	COMDAT ??_7?$CMap@IIHH@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CList@VRTCONTROL@@AAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CMap@IIKAAK@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CMap@KKIAAI@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CShortcutManager@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CSubclasser@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
_DATA	SEGMENT
__szAfxTempl DB	'afxtempl.h', 00H
	ORG $+1
_THIS_FILE DB	'D:\_CODE\Shared\ShortcutManager.cpp', 00H
_DATA	ENDS
PUBLIC	??0CShortcutManager@@QAE@H@Z			; CShortcutManager::CShortcutManager
PUBLIC	??0?$CMap@KKIAAI@@QAE@H@Z			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::CMap<unsigned long,unsigned long,unsigned int,unsigned int &>
PUBLIC	?IsValid@CSubclassWnd@@UBEHXZ			; CSubclassWnd::IsValid
PUBLIC	??1?$CMap@KKIAAI@@UAE@XZ			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::~CMap<unsigned long,unsigned long,unsigned int,unsigned int &>
PUBLIC	?PreDetachWindow@CSubclassWnd@@MAEXXZ		; CSubclassWnd::PreDetachWindow
PUBLIC	?PostDetachWindow@CSubclassWnd@@MAEXXZ		; CSubclassWnd::PostDetachWindow
PUBLIC	?IsHooked@CSubclassWnd@@MBEHXZ			; CSubclassWnd::IsHooked
PUBLIC	?IsValidHook@CSubclassWnd@@MBEHXZ		; CSubclassWnd::IsValidHook
PUBLIC	?Redraw@CSubclassWnd@@MBEXXZ			; CSubclassWnd::Redraw
PUBLIC	??0?$CMap@IIKAAK@@QAE@H@Z			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::CMap<unsigned int,unsigned int,unsigned long,unsigned long &>
PUBLIC	?WindowProc@CShortcutManager@@MAEJPAUHWND__@@IIJ@Z ; CShortcutManager::WindowProc
PUBLIC	??_7CShortcutManager@@6B@			; CShortcutManager::`vftable'
PUBLIC	??_GCShortcutManager@@UAEPAXI@Z			; CShortcutManager::`scalar deleting destructor'
PUBLIC	??_ECShortcutManager@@UAEPAXI@Z			; CShortcutManager::`vector deleting destructor'
EXTRN	__except_list:DWORD
EXTRN	__chkesp:NEAR
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CSubclassWnd@@QAE@XZ:NEAR			; CSubclassWnd::CSubclassWnd
EXTRN	??1CSubclassWnd@@UAE@XZ:NEAR			; CSubclassWnd::~CSubclassWnd
EXTRN	?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z:NEAR ; CSubclassWnd::HookWindow
EXTRN	?PostMessageW@CSubclassWnd@@MBEHIIJ@Z:NEAR	; CSubclassWnd::PostMessageW
EXTRN	?SendMessageW@CSubclassWnd@@MBEHIIJ@Z:NEAR	; CSubclassWnd::SendMessageW
;	COMDAT ??_7CShortcutManager@@6B@
; File D:\_CODE\Shared\ShortcutManager.cpp
CONST	SEGMENT
??_7CShortcutManager@@6B@ DD FLAT:??_ECShortcutManager@@UAEPAXI@Z ; CShortcutManager::`vftable'
	DD	FLAT:?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z
	DD	FLAT:?IsValid@CSubclassWnd@@UBEHXZ
	DD	FLAT:?PreDetachWindow@CSubclassWnd@@MAEXXZ
	DD	FLAT:?PostDetachWindow@CSubclassWnd@@MAEXXZ
	DD	FLAT:?IsHooked@CSubclassWnd@@MBEHXZ
	DD	FLAT:?IsValidHook@CSubclassWnd@@MBEHXZ
	DD	FLAT:?Redraw@CSubclassWnd@@MBEXXZ
	DD	FLAT:?PostMessageW@CSubclassWnd@@MBEHIIJ@Z
	DD	FLAT:?SendMessageW@CSubclassWnd@@MBEHIIJ@Z
	DD	FLAT:?WindowProc@CShortcutManager@@MAEJPAUHWND__@@IIJ@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77888	DD	019930520H
	DD	02H
	DD	FLAT:$T77892
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77892	DD	0ffffffffH
	DD	FLAT:$L77882
	DD	00H
	DD	FLAT:$L77883
xdata$x	ENDS
;	COMDAT ??0CShortcutManager@@QAE@H@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_bAutoSendCmds$ = 8
_this$ = -16
??0CShortcutManager@@QAE@H@Z PROC NEAR			; CShortcutManager::CShortcutManager, COMDAT

; 36   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77889
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CSubclassWnd@@QAE@XZ			; CSubclassWnd::CSubclassWnd
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	push	10					; 0000000aH
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	??0?$CMap@KKIAAI@@QAE@H@Z		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::CMap<unsigned long,unsigned long,unsigned int,unsigned int &>
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	push	10					; 0000000aH
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??0?$CMap@IIKAAK@@QAE@H@Z		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::CMap<unsigned int,unsigned int,unsigned long,unsigned long &>
	mov	eax, DWORD PTR _this$[ebp]
	mov	WORD PTR [eax+88], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	WORD PTR [ecx+90], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _bAutoSendCmds$[ebp]
	mov	DWORD PTR [edx+92], eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CShortcutManager@@6B@ ; CShortcutManager::`vftable'

; 37   : 
; 38   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77882:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CSubclassWnd@@UAE@XZ			; CSubclassWnd::~CSubclassWnd
	ret	0
$L77883:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	??1?$CMap@KKIAAI@@UAE@XZ		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::~CMap<unsigned long,unsigned long,unsigned int,unsigned int &>
	ret	0
$L77889:
	mov	eax, OFFSET FLAT:$T77888
	jmp	___CxxFrameHandler
text$x	ENDS
??0CShortcutManager@@QAE@H@Z ENDP			; CShortcutManager::CShortcutManager
;	COMDAT ?IsValid@CSubclassWnd@@UBEHXZ
_TEXT	SEGMENT
_this$ = -4
?IsValid@CSubclassWnd@@UBEHXZ PROC NEAR			; CSubclassWnd::IsValid, COMDAT

; 56   : 	virtual BOOL IsValid() const { return IsValidHook(); }

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+24]
	cmp	esi, esp
	call	__chkesp
	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsValid@CSubclassWnd@@UBEHXZ ENDP			; CSubclassWnd::IsValid
_TEXT	ENDS
;	COMDAT ?PreDetachWindow@CSubclassWnd@@MAEXXZ
_TEXT	SEGMENT
_this$ = -4
?PreDetachWindow@CSubclassWnd@@MAEXXZ PROC NEAR		; CSubclassWnd::PreDetachWindow, COMDAT

; 96   : 	virtual void PreDetachWindow() { }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esp, ebp
	pop	ebp
	ret	0
?PreDetachWindow@CSubclassWnd@@MAEXXZ ENDP		; CSubclassWnd::PreDetachWindow
_TEXT	ENDS
;	COMDAT ?PostDetachWindow@CSubclassWnd@@MAEXXZ
_TEXT	SEGMENT
_this$ = -4
?PostDetachWindow@CSubclassWnd@@MAEXXZ PROC NEAR	; CSubclassWnd::PostDetachWindow, COMDAT

; 97   : 	virtual void PostDetachWindow() { }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esp, ebp
	pop	ebp
	ret	0
?PostDetachWindow@CSubclassWnd@@MAEXXZ ENDP		; CSubclassWnd::PostDetachWindow
_TEXT	ENDS
;	COMDAT ?IsHooked@CSubclassWnd@@MBEHXZ
_TEXT	SEGMENT
_this$ = -4
?IsHooked@CSubclassWnd@@MBEHXZ PROC NEAR		; CSubclassWnd::IsHooked, COMDAT

; 100  : 	virtual BOOL IsHooked() const { return m_hWndHooked != NULL; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	cmp	DWORD PTR [eax+4], 0
	setne	cl
	mov	eax, ecx
	mov	esp, ebp
	pop	ebp
	ret	0
?IsHooked@CSubclassWnd@@MBEHXZ ENDP			; CSubclassWnd::IsHooked
_TEXT	ENDS
EXTRN	__imp__IsWindow@4:NEAR
;	COMDAT ?IsValidHook@CSubclassWnd@@MBEHXZ
_TEXT	SEGMENT
_this$ = -4
?IsValidHook@CSubclassWnd@@MBEHXZ PROC NEAR		; CSubclassWnd::IsValidHook, COMDAT

; 101  : 	virtual BOOL IsValidHook() const { return ::IsWindow(m_hWndHooked); }

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsValidHook@CSubclassWnd@@MBEHXZ ENDP			; CSubclassWnd::IsValidHook
_TEXT	ENDS
PUBLIC	?Invalidate@CSubclassWnd@@QBEXH@Z		; CSubclassWnd::Invalidate
;	COMDAT ?Redraw@CSubclassWnd@@MBEXXZ
_TEXT	SEGMENT
_this$ = -4
?Redraw@CSubclassWnd@@MBEXXZ PROC NEAR			; CSubclassWnd::Redraw, COMDAT

; 113  : 	virtual void Redraw() const { Invalidate(); }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Invalidate@CSubclassWnd@@QBEXH@Z	; CSubclassWnd::Invalidate
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Redraw@CSubclassWnd@@MBEXXZ ENDP			; CSubclassWnd::Redraw
_TEXT	ENDS
EXTRN	__imp__InvalidateRect@12:NEAR
;	COMDAT ?Invalidate@CSubclassWnd@@QBEXH@Z
_TEXT	SEGMENT
_bErase$ = 8
_this$ = -4
?Invalidate@CSubclassWnd@@QBEXH@Z PROC NEAR		; CSubclassWnd::Invalidate, COMDAT

; 71   : 	inline void Invalidate(BOOL bErase = TRUE) const { ::InvalidateRect(m_hWndHooked, NULL, bErase); }

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esi, esp
	mov	eax, DWORD PTR _bErase$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR __imp__InvalidateRect@12
	cmp	esi, esp
	call	__chkesp
	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Invalidate@CSubclassWnd@@QBEXH@Z ENDP			; CSubclassWnd::Invalidate
_TEXT	ENDS
PUBLIC	??1CShortcutManager@@UAE@XZ			; CShortcutManager::~CShortcutManager
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCShortcutManager@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCShortcutManager@@UAEPAXI@Z PROC NEAR		; CShortcutManager::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CShortcutManager@@UAE@XZ		; CShortcutManager::~CShortcutManager
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L76055
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L76055:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCShortcutManager@@UAEPAXI@Z ENDP			; CShortcutManager::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??1?$CMap@IIKAAK@@UAE@XZ			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::~CMap<unsigned int,unsigned int,unsigned long,unsigned long &>
;	COMDAT xdata$x
; File D:\_CODE\Shared\ShortcutManager.cpp
xdata$x	SEGMENT
$T77918	DD	019930520H
	DD	02H
	DD	FLAT:$T77920
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77920	DD	0ffffffffH
	DD	FLAT:$L77914
	DD	00H
	DD	FLAT:$L77915
xdata$x	ENDS
;	COMDAT ??1CShortcutManager@@UAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??1CShortcutManager@@UAE@XZ PROC NEAR			; CShortcutManager::~CShortcutManager, COMDAT

; 41   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77919
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CShortcutManager@@6B@ ; CShortcutManager::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 42   : 
; 43   : }

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??1?$CMap@IIKAAK@@UAE@XZ		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::~CMap<unsigned int,unsigned int,unsigned long,unsigned long &>
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	??1?$CMap@KKIAAI@@UAE@XZ		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::~CMap<unsigned long,unsigned long,unsigned int,unsigned int &>
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CSubclassWnd@@UAE@XZ			; CSubclassWnd::~CSubclassWnd
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
$L77914:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CSubclassWnd@@UAE@XZ			; CSubclassWnd::~CSubclassWnd
	ret	0
$L77915:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	??1?$CMap@KKIAAI@@UAE@XZ		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::~CMap<unsigned long,unsigned long,unsigned int,unsigned int &>
	ret	0
$L77919:
	mov	eax, OFFSET FLAT:$T77918
	jmp	___CxxFrameHandler
text$x	ENDS
??1CShortcutManager@@UAE@XZ ENDP			; CShortcutManager::~CShortcutManager
PUBLIC	?Initialize@CShortcutManager@@QAEHPAVCWnd@@PBVIPreferences@@GG@Z ; CShortcutManager::Initialize
PUBLIC	?LoadSettings@CShortcutManager@@QAEXPBVIPreferences@@@Z ; CShortcutManager::LoadSettings
EXTRN	??BCWnd@@QBEPAUHWND__@@XZ:NEAR			; CWnd::operator HWND__ *
;	COMDAT ?Initialize@CShortcutManager@@QAEHPAVCWnd@@PBVIPreferences@@GG@Z
_TEXT	SEGMENT
_pOwner$ = 8
_pPrefs$ = 12
_wInvalidComb$ = 16
_wFallbackModifiers$ = 20
_this$ = -4
?Initialize@CShortcutManager@@QAEHPAVCWnd@@PBVIPreferences@@GG@Z PROC NEAR ; CShortcutManager::Initialize, COMDAT

; 46   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 47   : 	if (!IsHooked() && pOwner && HookWindow(*pOwner))

	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+20]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76066
	cmp	DWORD PTR _pOwner$[ebp], 0
	je	SHORT $L76066
	mov	esi, esp
	push	0
	mov	ecx, DWORD PTR _pOwner$[ebp]
	call	??BCWnd@@QBEPAUHWND__@@XZ		; CWnd::operator HWND__ *
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L76066

; 49   : 		m_wInvalidComb = wInvalidComb;

	mov	eax, DWORD PTR _this$[ebp]
	mov	cx, WORD PTR _wInvalidComb$[ebp]
	mov	WORD PTR [eax+88], cx

; 50   : 		m_wFallbackModifiers = wFallbackModifiers;

	mov	edx, DWORD PTR _this$[ebp]
	mov	ax, WORD PTR _wFallbackModifiers$[ebp]
	mov	WORD PTR [edx+90], ax

; 51   : 		
; 52   : 		if (pPrefs)

	cmp	DWORD PTR _pPrefs$[ebp], 0
	je	SHORT $L76067

; 53   : 			LoadSettings(pPrefs);

	mov	ecx, DWORD PTR _pPrefs$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadSettings@CShortcutManager@@QAEXPBVIPreferences@@@Z ; CShortcutManager::LoadSettings
$L76067:

; 54   : 		
; 55   : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76065
$L76066:

; 57   : 
; 58   : 	return FALSE;

	xor	eax, eax
$L76065:

; 59   : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?Initialize@CShortcutManager@@QAEHPAVCWnd@@PBVIPreferences@@GG@Z ENDP ; CShortcutManager::Initialize
_TEXT	ENDS
PUBLIC	?Release@CShortcutManager@@QAEHPAVIPreferences@@@Z ; CShortcutManager::Release
PUBLIC	?SaveSettings@CShortcutManager@@QBEXPAVIPreferences@@@Z ; CShortcutManager::SaveSettings
;	COMDAT ?Release@CShortcutManager@@QAEHPAVIPreferences@@@Z
_TEXT	SEGMENT
_pPrefs$ = 8
_this$ = -4
?Release@CShortcutManager@@QAEHPAVIPreferences@@@Z PROC NEAR ; CShortcutManager::Release, COMDAT

; 62   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 63   : 	if (!IsHooked())

	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+20]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76072

; 64   : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76071
$L76072:

; 65   : 
; 66   : 	if (pPrefs)

	cmp	DWORD PTR _pPrefs$[ebp], 0
	je	SHORT $L76073

; 67   : 		SaveSettings(pPrefs);

	mov	eax, DWORD PTR _pPrefs$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SaveSettings@CShortcutManager@@QBEXPAVIPreferences@@@Z ; CShortcutManager::SaveSettings
$L76073:

; 68   : 
; 69   : 	return HookWindow(NULL);

	mov	esi, esp
	push	0
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp
$L76071:

; 70   : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Release@CShortcutManager@@QAEHPAVIPreferences@@@Z ENDP	; CShortcutManager::Release
_TEXT	ENDS
PUBLIC	?AddShortcut@CShortcutManager@@QAEHIGG@Z	; CShortcutManager::AddShortcut
PUBLIC	?SetShortcut@CShortcutManager@@QAEXIGG@Z	; CShortcutManager::SetShortcut
PUBLIC	?Lookup@?$CMap@KKIAAI@@QBEHKAAI@Z		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::Lookup
PUBLIC	?RemoveKey@?$CMap@KKIAAI@@QAEHK@Z		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::RemoveKey
PUBLIC	?Lookup@?$CMap@IIKAAK@@QBEHIAAK@Z		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::Lookup
PUBLIC	??A?$CMap@IIKAAK@@QAEAAKI@Z			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::operator[]
;	COMDAT ?SetShortcut@CShortcutManager@@QAEXIGG@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_wVirtKeyCode$ = 12
_wModifiers$ = 16
_this$ = -12
_nOtherCmdID$ = -8
_dwShortcut$ = -4
?SetShortcut@CShortcutManager@@QAEXIGG@Z PROC NEAR	; CShortcutManager::SetShortcut, COMDAT

; 73   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 74   : 	UINT nOtherCmdID = 0;

	mov	DWORD PTR _nOtherCmdID$[ebp], 0

; 75   : 	DWORD dwShortcut = MAKELONG(wVirtKeyCode, wModifiers);

	mov	eax, DWORD PTR _wVirtKeyCode$[ebp]
	and	eax, 65535				; 0000ffffH
	mov	ecx, DWORD PTR _wModifiers$[ebp]
	and	ecx, 65535				; 0000ffffH
	shl	ecx, 16					; 00000010H
	or	eax, ecx
	mov	DWORD PTR _dwShortcut$[ebp], eax

; 76   : 
; 77   : 	// if the shortcut == 0 then remove the existing shortcut associated with nCmdID
; 78   : 	if (!dwShortcut)

	cmp	DWORD PTR _dwShortcut$[ebp], 0
	jne	SHORT $L76086

; 80   : 		m_mapID2Shortcut.Lookup(nCmdID, dwShortcut);

	lea	edx, DWORD PTR _dwShortcut$[ebp]
	push	edx
	mov	eax, DWORD PTR _nCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	?Lookup@?$CMap@IIKAAK@@QBEHIAAK@Z	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::Lookup

; 81   : 
; 82   : 		m_mapShortcut2ID.RemoveKey(dwShortcut);

	mov	ecx, DWORD PTR _dwShortcut$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	?RemoveKey@?$CMap@KKIAAI@@QAEHK@Z	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::RemoveKey

; 83   : 
; 84   : 		// mark these commands explicitly as having no shortcut so that
; 85   : 		// the user's intent is clear. 
; 86   : 		m_mapID2Shortcut[nCmdID] = NO_SHORTCUT;

	mov	edx, DWORD PTR _nCmdID$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??A?$CMap@IIKAAK@@QAEAAKI@Z		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::operator[]
	mov	DWORD PTR [eax], -1

; 87   : 
; 88   : 		return;

	jmp	SHORT $L76079
$L76086:

; 90   : 	// check for existing cmds using this shortcut to remove
; 91   : 	else if (m_mapShortcut2ID.Lookup(dwShortcut, nOtherCmdID))

	lea	eax, DWORD PTR _nOtherCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwShortcut$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	?Lookup@?$CMap@KKIAAI@@QBEHKAAI@Z	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::Lookup
	test	eax, eax
	je	SHORT $L76088

; 93   : 		m_mapShortcut2ID.RemoveKey(dwShortcut);

	mov	edx, DWORD PTR _dwShortcut$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	?RemoveKey@?$CMap@KKIAAI@@QAEHK@Z	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::RemoveKey

; 94   : 
; 95   : 		// mark these commands explicitly as having no shortcut so that
; 96   : 		// they subsequently will not be overwritten 
; 97   : 		m_mapID2Shortcut[nOtherCmdID] = NO_SHORTCUT;

	mov	eax, DWORD PTR _nOtherCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??A?$CMap@IIKAAK@@QAEAAKI@Z		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::operator[]
	mov	DWORD PTR [eax], -1
$L76088:

; 99   : 
; 100  : 	// then simple add
; 101  : 	AddShortcut(nCmdID, wVirtKeyCode, wModifiers);

	mov	cx, WORD PTR _wModifiers$[ebp]
	push	ecx
	mov	dx, WORD PTR _wVirtKeyCode$[ebp]
	push	edx
	mov	eax, DWORD PTR _nCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddShortcut@CShortcutManager@@QAEHIGG@Z ; CShortcutManager::AddShortcut
$L76079:

; 102  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?SetShortcut@CShortcutManager@@QAEXIGG@Z ENDP		; CShortcutManager::SetShortcut
_TEXT	ENDS
PUBLIC	?SetShortcut@CShortcutManager@@QAEXIK@Z		; CShortcutManager::SetShortcut
;	COMDAT ?SetShortcut@CShortcutManager@@QAEXIK@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_dwShortcut$ = 12
_this$ = -4
?SetShortcut@CShortcutManager@@QAEXIK@Z PROC NEAR	; CShortcutManager::SetShortcut, COMDAT

; 105  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 106  : 	SetShortcut(nCmdID, LOWORD(dwShortcut), HIWORD(dwShortcut));

	mov	eax, DWORD PTR _dwShortcut$[ebp]
	shr	eax, 16					; 00000010H
	and	eax, 65535				; 0000ffffH
	push	eax
	mov	cx, WORD PTR _dwShortcut$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nCmdID$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetShortcut@CShortcutManager@@QAEXIGG@Z ; CShortcutManager::SetShortcut

; 107  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SetShortcut@CShortcutManager@@QAEXIK@Z ENDP		; CShortcutManager::SetShortcut
_TEXT	ENDS
PUBLIC	?AddShortcut@CShortcutManager@@QAEHIK@Z		; CShortcutManager::AddShortcut
;	COMDAT ?AddShortcut@CShortcutManager@@QAEHIK@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_dwShortcut$ = 12
_this$ = -4
?AddShortcut@CShortcutManager@@QAEHIK@Z PROC NEAR	; CShortcutManager::AddShortcut, COMDAT

; 110  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 111  : 	return AddShortcut(nCmdID, LOWORD(dwShortcut), HIWORD(dwShortcut));

	mov	eax, DWORD PTR _dwShortcut$[ebp]
	shr	eax, 16					; 00000010H
	and	eax, 65535				; 0000ffffH
	push	eax
	mov	cx, WORD PTR _dwShortcut$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nCmdID$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddShortcut@CShortcutManager@@QAEHIGG@Z ; CShortcutManager::AddShortcut

; 112  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?AddShortcut@CShortcutManager@@QAEHIK@Z ENDP		; CShortcutManager::AddShortcut
_TEXT	ENDS
PUBLIC	?ValidateModifiers@CShortcutManager@@QBEGGG@Z	; CShortcutManager::ValidateModifiers
PUBLIC	??A?$CMap@KKIAAI@@QAEAAIK@Z			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::operator[]
;	COMDAT ?AddShortcut@CShortcutManager@@QAEHIGG@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_wVirtKeyCode$ = 12
_wModifiers$ = 16
_this$ = -16
_dwShortcut$ = -4
_nOtherCmdID$ = -8
_dwOtherShortcut$ = -12
?AddShortcut@CShortcutManager@@QAEHIGG@Z PROC NEAR	; CShortcutManager::AddShortcut, COMDAT

; 115  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 116  : 	// test for invalid modifiers
; 117  : 	if (ValidateModifiers(wModifiers, wVirtKeyCode) != wModifiers)

	mov	ax, WORD PTR _wVirtKeyCode$[ebp]
	push	eax
	mov	cx, WORD PTR _wModifiers$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ValidateModifiers@CShortcutManager@@QBEGGG@Z ; CShortcutManager::ValidateModifiers
	and	eax, 65535				; 0000ffffH
	mov	edx, DWORD PTR _wModifiers$[ebp]
	and	edx, 65535				; 0000ffffH
	cmp	eax, edx
	je	SHORT $L76111

; 118  : 		return FALSE;

	xor	eax, eax
	jmp	$L76110
$L76111:

; 119  : 
; 120  : 	// check for existing cmds using this shortcut
; 121  : 	DWORD dwShortcut = MAKELONG(wVirtKeyCode, wModifiers);

	mov	eax, DWORD PTR _wVirtKeyCode$[ebp]
	and	eax, 65535				; 0000ffffH
	mov	ecx, DWORD PTR _wModifiers$[ebp]
	and	ecx, 65535				; 0000ffffH
	shl	ecx, 16					; 00000010H
	or	eax, ecx
	mov	DWORD PTR _dwShortcut$[ebp], eax

; 122  : 
; 123  : 	if (!nCmdID || !dwShortcut)

	cmp	DWORD PTR _nCmdID$[ebp], 0
	je	SHORT $L76118
	cmp	DWORD PTR _dwShortcut$[ebp], 0
	jne	SHORT $L76117
$L76118:

; 124  : 		return FALSE;

	xor	eax, eax
	jmp	$L76110
$L76117:

; 125  : 
; 126  : 	UINT nOtherCmdID = 0;

	mov	DWORD PTR _nOtherCmdID$[ebp], 0

; 127  : 
; 128  : 	if (m_mapShortcut2ID.Lookup(dwShortcut, nOtherCmdID) && nOtherCmdID)

	lea	edx, DWORD PTR _nOtherCmdID$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwShortcut$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	?Lookup@?$CMap@KKIAAI@@QBEHKAAI@Z	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::Lookup
	test	eax, eax
	je	SHORT $L76120
	cmp	DWORD PTR _nOtherCmdID$[ebp], 0
	je	SHORT $L76120

; 129  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L76110
$L76120:

; 130  : 
; 131  : 	// check for existing shortcut on this cmd that we'll need to clean up
; 132  : 	DWORD dwOtherShortcut = 0;

	mov	DWORD PTR _dwOtherShortcut$[ebp], 0

; 133  : 
; 134  : 	if (m_mapID2Shortcut.Lookup(nCmdID, dwOtherShortcut))

	lea	ecx, DWORD PTR _dwOtherShortcut$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nCmdID$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	?Lookup@?$CMap@IIKAAK@@QBEHIAAK@Z	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::Lookup
	test	eax, eax
	je	SHORT $L76122

; 135  : 		m_mapShortcut2ID.RemoveKey(dwOtherShortcut);

	mov	eax, DWORD PTR _dwOtherShortcut$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	?RemoveKey@?$CMap@KKIAAI@@QAEHK@Z	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::RemoveKey
$L76122:

; 136  : 
; 137  : 	m_mapShortcut2ID[dwShortcut] = nCmdID;

	mov	ecx, DWORD PTR _dwShortcut$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	??A?$CMap@KKIAAI@@QAEAAIK@Z		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::operator[]
	mov	edx, DWORD PTR _nCmdID$[ebp]
	mov	DWORD PTR [eax], edx

; 138  : 	m_mapID2Shortcut[nCmdID] = dwShortcut;

	mov	eax, DWORD PTR _nCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??A?$CMap@IIKAAK@@QAEAAKI@Z		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::operator[]
	mov	ecx, DWORD PTR _dwShortcut$[ebp]
	mov	DWORD PTR [eax], ecx

; 139  : 
; 140  : 	return TRUE;

	mov	eax, 1
$L76110:

; 141  : }

	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?AddShortcut@CShortcutManager@@QAEHIGG@Z ENDP		; CShortcutManager::AddShortcut
_TEXT	ENDS
;	COMDAT ?ValidateModifiers@CShortcutManager@@QBEGGG@Z
_TEXT	SEGMENT
_wModifiers$ = 8
_wVirtKeyCode$ = 12
_this$ = -24
_bCtrl$ = -20
_bShift$ = -16
_bAlt$ = -4
_bExtended$ = -12
_bFail$ = -8
?ValidateModifiers@CShortcutManager@@QBEGGG@Z PROC NEAR	; CShortcutManager::ValidateModifiers, COMDAT

; 144  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 56					; 00000038H
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-56]
	mov	ecx, 14					; 0000000eH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 145  : 	if (!m_wInvalidComb) // optimization

	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax+88]
	test	ecx, ecx
	jne	SHORT $L76128

; 146  : 		return wModifiers;

	mov	ax, WORD PTR _wModifiers$[ebp]
	jmp	$L76127
$L76128:

; 147  : 
; 148  : 	// check for our special modifiers first
; 149  : 	if ((m_wInvalidComb & HKCOMB_EXFKEYS) && (wVirtKeyCode >= VK_F1 && wVirtKeyCode <= VK_F24))

	mov	edx, DWORD PTR _this$[ebp]
	xor	eax, eax
	mov	ax, WORD PTR [edx+88]
	and	eax, 256				; 00000100H
	test	eax, eax
	je	SHORT $L76129
	mov	ecx, DWORD PTR _wVirtKeyCode$[ebp]
	and	ecx, 65535				; 0000ffffH
	cmp	ecx, 112				; 00000070H
	jl	SHORT $L76129
	mov	edx, DWORD PTR _wVirtKeyCode$[ebp]
	and	edx, 65535				; 0000ffffH
	cmp	edx, 135				; 00000087H
	jg	SHORT $L76129

; 150  : 		return wModifiers;

	mov	ax, WORD PTR _wModifiers$[ebp]
	jmp	$L76127
$L76129:

; 151  : 
; 152  : 	// test for invalid combinations
; 153  : 	BOOL bCtrl = (wModifiers & HOTKEYF_CONTROL);

	mov	eax, DWORD PTR _wModifiers$[ebp]
	and	eax, 65535				; 0000ffffH
	and	eax, 2
	mov	DWORD PTR _bCtrl$[ebp], eax

; 154  : 	BOOL bShift = (wModifiers & HOTKEYF_SHIFT);

	mov	ecx, DWORD PTR _wModifiers$[ebp]
	and	ecx, 65535				; 0000ffffH
	and	ecx, 1
	mov	DWORD PTR _bShift$[ebp], ecx

; 155  : 	BOOL bAlt = (wModifiers & HOTKEYF_ALT);

	mov	edx, DWORD PTR _wModifiers$[ebp]
	and	edx, 65535				; 0000ffffH
	and	edx, 4
	mov	DWORD PTR _bAlt$[ebp], edx

; 156  : 	BOOL bExtended = (wModifiers & HOTKEYF_EXT);

	mov	eax, DWORD PTR _wModifiers$[ebp]
	and	eax, 65535				; 0000ffffH
	and	eax, 8
	mov	DWORD PTR _bExtended$[ebp], eax

; 157  : 
; 158  : 	BOOL bFail = ((m_wInvalidComb & HKCOMB_NONE) && !bCtrl && !bShift && !bAlt);

	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dx, WORD PTR [ecx+88]
	and	edx, 1
	test	edx, edx
	je	SHORT $L77937
	cmp	DWORD PTR _bCtrl$[ebp], 0
	jne	SHORT $L77937
	cmp	DWORD PTR _bShift$[ebp], 0
	jne	SHORT $L77937
	cmp	DWORD PTR _bAlt$[ebp], 0
	jne	SHORT $L77937
	mov	DWORD PTR -28+[ebp], 1
	jmp	SHORT $L77938
$L77937:
	mov	DWORD PTR -28+[ebp], 0
$L77938:
	mov	eax, DWORD PTR -28+[ebp]
	mov	DWORD PTR _bFail$[ebp], eax

; 159  : 
; 160  : 	bFail |= ((m_wInvalidComb & HKCOMB_S) && !bCtrl && bShift && !bAlt);

	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dx, WORD PTR [ecx+88]
	and	edx, 2
	test	edx, edx
	je	SHORT $L77939
	cmp	DWORD PTR _bCtrl$[ebp], 0
	jne	SHORT $L77939
	cmp	DWORD PTR _bShift$[ebp], 0
	je	SHORT $L77939
	cmp	DWORD PTR _bAlt$[ebp], 0
	jne	SHORT $L77939
	mov	DWORD PTR -32+[ebp], 1
	jmp	SHORT $L77940
$L77939:
	mov	DWORD PTR -32+[ebp], 0
$L77940:
	mov	eax, DWORD PTR _bFail$[ebp]
	or	eax, DWORD PTR -32+[ebp]
	mov	DWORD PTR _bFail$[ebp], eax

; 161  : 	bFail |= ((m_wInvalidComb & HKCOMB_C) && bCtrl && !bShift && !bAlt);

	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dx, WORD PTR [ecx+88]
	and	edx, 4
	test	edx, edx
	je	SHORT $L77941
	cmp	DWORD PTR _bCtrl$[ebp], 0
	je	SHORT $L77941
	cmp	DWORD PTR _bShift$[ebp], 0
	jne	SHORT $L77941
	cmp	DWORD PTR _bAlt$[ebp], 0
	jne	SHORT $L77941
	mov	DWORD PTR -36+[ebp], 1
	jmp	SHORT $L77942
$L77941:
	mov	DWORD PTR -36+[ebp], 0
$L77942:
	mov	eax, DWORD PTR _bFail$[ebp]
	or	eax, DWORD PTR -36+[ebp]
	mov	DWORD PTR _bFail$[ebp], eax

; 162  : 	bFail |= ((m_wInvalidComb & HKCOMB_A) && !bCtrl && !bShift && bAlt);

	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dx, WORD PTR [ecx+88]
	and	edx, 8
	test	edx, edx
	je	SHORT $L77943
	cmp	DWORD PTR _bCtrl$[ebp], 0
	jne	SHORT $L77943
	cmp	DWORD PTR _bShift$[ebp], 0
	jne	SHORT $L77943
	cmp	DWORD PTR _bAlt$[ebp], 0
	je	SHORT $L77943
	mov	DWORD PTR -40+[ebp], 1
	jmp	SHORT $L77944
$L77943:
	mov	DWORD PTR -40+[ebp], 0
$L77944:
	mov	eax, DWORD PTR _bFail$[ebp]
	or	eax, DWORD PTR -40+[ebp]
	mov	DWORD PTR _bFail$[ebp], eax

; 163  : 	bFail |= ((m_wInvalidComb & HKCOMB_SC) && bCtrl && bShift && !bAlt);

	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dx, WORD PTR [ecx+88]
	and	edx, 16					; 00000010H
	test	edx, edx
	je	SHORT $L77945
	cmp	DWORD PTR _bCtrl$[ebp], 0
	je	SHORT $L77945
	cmp	DWORD PTR _bShift$[ebp], 0
	je	SHORT $L77945
	cmp	DWORD PTR _bAlt$[ebp], 0
	jne	SHORT $L77945
	mov	DWORD PTR -44+[ebp], 1
	jmp	SHORT $L77946
$L77945:
	mov	DWORD PTR -44+[ebp], 0
$L77946:
	mov	eax, DWORD PTR _bFail$[ebp]
	or	eax, DWORD PTR -44+[ebp]
	mov	DWORD PTR _bFail$[ebp], eax

; 164  : 	bFail |= ((m_wInvalidComb & HKCOMB_SA) && !bCtrl && bShift && bAlt);

	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dx, WORD PTR [ecx+88]
	and	edx, 32					; 00000020H
	test	edx, edx
	je	SHORT $L77947
	cmp	DWORD PTR _bCtrl$[ebp], 0
	jne	SHORT $L77947
	cmp	DWORD PTR _bShift$[ebp], 0
	je	SHORT $L77947
	cmp	DWORD PTR _bAlt$[ebp], 0
	je	SHORT $L77947
	mov	DWORD PTR -48+[ebp], 1
	jmp	SHORT $L77948
$L77947:
	mov	DWORD PTR -48+[ebp], 0
$L77948:
	mov	eax, DWORD PTR _bFail$[ebp]
	or	eax, DWORD PTR -48+[ebp]
	mov	DWORD PTR _bFail$[ebp], eax

; 165  : 	bFail |= ((m_wInvalidComb & HKCOMB_CA) && bCtrl && !bShift && bAlt);

	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dx, WORD PTR [ecx+88]
	and	edx, 64					; 00000040H
	test	edx, edx
	je	SHORT $L77949
	cmp	DWORD PTR _bCtrl$[ebp], 0
	je	SHORT $L77949
	cmp	DWORD PTR _bShift$[ebp], 0
	jne	SHORT $L77949
	cmp	DWORD PTR _bAlt$[ebp], 0
	je	SHORT $L77949
	mov	DWORD PTR -52+[ebp], 1
	jmp	SHORT $L77950
$L77949:
	mov	DWORD PTR -52+[ebp], 0
$L77950:
	mov	eax, DWORD PTR _bFail$[ebp]
	or	eax, DWORD PTR -52+[ebp]
	mov	DWORD PTR _bFail$[ebp], eax

; 166  : 	bFail |= ((m_wInvalidComb & HKCOMB_SCA) && bCtrl && bShift && bAlt);

	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dx, WORD PTR [ecx+88]
	and	edx, 128				; 00000080H
	test	edx, edx
	je	SHORT $L77951
	cmp	DWORD PTR _bCtrl$[ebp], 0
	je	SHORT $L77951
	cmp	DWORD PTR _bShift$[ebp], 0
	je	SHORT $L77951
	cmp	DWORD PTR _bAlt$[ebp], 0
	je	SHORT $L77951
	mov	DWORD PTR -56+[ebp], 1
	jmp	SHORT $L77952
$L77951:
	mov	DWORD PTR -56+[ebp], 0
$L77952:
	mov	eax, DWORD PTR _bFail$[ebp]
	or	eax, DWORD PTR -56+[ebp]
	mov	DWORD PTR _bFail$[ebp], eax

; 167  : 
; 168  : 	if (bFail)

	cmp	DWORD PTR _bFail$[ebp], 0
	je	SHORT $L76135

; 169  : 		return (WORD)(m_wFallbackModifiers | (bExtended ? HOTKEYF_EXT : 0x0));

	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dx, WORD PTR [ecx+90]
	mov	eax, edx
	mov	ecx, DWORD PTR _bExtended$[ebp]
	neg	ecx
	sbb	ecx, ecx
	and	ecx, 8
	or	eax, ecx
	jmp	SHORT $L76127
$L76135:

; 170  : 
; 171  : 	// else ok
; 172  : 	return wModifiers;

	mov	ax, WORD PTR _wModifiers$[ebp]
$L76127:

; 173  : }

	pop	edi
	mov	esp, ebp
	pop	ebp
	ret	8
?ValidateModifiers@CShortcutManager@@QBEGGG@Z ENDP	; CShortcutManager::ValidateModifiers
_TEXT	ENDS
PUBLIC	?GetShortcut@CShortcutManager@@IBEKGH@Z		; CShortcutManager::GetShortcut
EXTRN	?KeyIsPressed@Misc@@YAHK@Z:NEAR			; Misc::KeyIsPressed
;	COMDAT ?GetShortcut@CShortcutManager@@IBEKGH@Z
_TEXT	SEGMENT
_wVirtKeyCode$ = 8
_bExtended$ = 12
_this$ = -20
_bCtrl$ = -12
_bShift$ = -8
_bAlt$ = -4
_wModifiers$ = -16
?GetShortcut@CShortcutManager@@IBEKGH@Z PROC NEAR	; CShortcutManager::GetShortcut, COMDAT

; 176  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 20					; 00000014H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 177  : 	BOOL bCtrl = Misc::KeyIsPressed(VK_CONTROL);

	push	17					; 00000011H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	mov	DWORD PTR _bCtrl$[ebp], eax

; 178  : 	BOOL bShift = Misc::KeyIsPressed(VK_SHIFT);

	push	16					; 00000010H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	mov	DWORD PTR _bShift$[ebp], eax

; 179  : 	BOOL bAlt = Misc::KeyIsPressed(VK_MENU);

	push	18					; 00000012H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	mov	DWORD PTR _bAlt$[ebp], eax

; 180  : 
; 181  : 	WORD wModifiers = (WORD)((bCtrl ? HOTKEYF_CONTROL : 0) |
; 182  : 						(bShift ? HOTKEYF_SHIFT : 0) |
; 183  : 						(bAlt ? HOTKEYF_ALT : 0) |
; 184  : 						(bExtended ? HOTKEYF_EXT : 0));

	mov	eax, DWORD PTR _bCtrl$[ebp]
	neg	eax
	sbb	eax, eax
	and	eax, 2
	xor	ecx, ecx
	cmp	DWORD PTR _bShift$[ebp], 0
	setne	cl
	or	eax, ecx
	mov	edx, DWORD PTR _bAlt$[ebp]
	neg	edx
	sbb	edx, edx
	and	edx, 4
	or	eax, edx
	mov	ecx, DWORD PTR _bExtended$[ebp]
	neg	ecx
	sbb	ecx, ecx
	and	ecx, 8
	or	eax, ecx
	mov	WORD PTR _wModifiers$[ebp], ax

; 185  : 
; 186  : 	return MAKELONG(wVirtKeyCode, wModifiers);

	mov	eax, DWORD PTR _wVirtKeyCode$[ebp]
	and	eax, 65535				; 0000ffffH
	mov	edx, DWORD PTR _wModifiers$[ebp]
	and	edx, 65535				; 0000ffffH
	shl	edx, 16					; 00000010H
	or	eax, edx

; 187  : }

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?GetShortcut@CShortcutManager@@IBEKGH@Z ENDP		; CShortcutManager::GetShortcut
_TEXT	ENDS
PUBLIC	?ProcessMessage@CShortcutManager@@QBEIPBUtagMSG@@PAK@Z ; CShortcutManager::ProcessMessage
PUBLIC	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ		; CSubclassWnd::GetHwnd
PUBLIC	?IsWindowEnabled@CSubclassWnd@@QBEHXZ		; CSubclassWnd::IsWindowEnabled
PUBLIC	?IsWindowVisible@CSubclassWnd@@QBEHXZ		; CSubclassWnd::IsWindowVisible
PUBLIC	?IsEditShortcut@CShortcutManager@@KAHK@Z	; CShortcutManager::IsEditShortcut
EXTRN	__imp__GetWindowLongW@8:NEAR
EXTRN	?IsClass@CWinClasses@@SAHPAUHWND__@@PBG@Z:NEAR	; CWinClasses::IsClass
EXTRN	?IsEditControl@CWinClasses@@SAHPAUHWND__@@@Z:NEAR ; CWinClasses::IsEditControl
EXTRN	__imp__IsChild@8:NEAR
;	COMDAT data
; File D:\_CODE\Shared\ShortcutManager.cpp
data	SEGMENT
$SG76165 DB	'E', 00H, 'd', 00H, 'i', 00H, 't', 00H, 00H, 00H
	ORG $+2
$SG76175 DB	'm', 00H, 's', 00H, 'c', 00H, 't', 00H, 'l', 00H, 's', 00H
	DB	'_', 00H, 'h', 00H, 'o', 00H, 't', 00H, 'k', 00H, 'e', 00H, 'y'
	DB	00H, '3', 00H, '2', 00H, 00H, 00H
data	ENDS
;	COMDAT ?ProcessMessage@CShortcutManager@@QBEIPBUtagMSG@@PAK@Z
_TEXT	SEGMENT
_pMsg$ = 8
_pShortcut$ = 12
_this$ = -36
_hFocus$ = -8
_hMainWnd$ = -4
_nStyle$76162 = -12
_bEdit$76176 = -24
_bExtKey$76178 = -20
_dwShortcut$76179 = -16
_nCmdID$76181 = -28
_wModifiers$76186 = -32
?ProcessMessage@CShortcutManager@@QBEIPBUtagMSG@@PAK@Z PROC NEAR ; CShortcutManager::ProcessMessage, COMDAT

; 191  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 40					; 00000028H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-40]
	mov	ecx, 10					; 0000000aH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 192  : 	// only process accelerators if we are enabled and visible
; 193  : 	if (!IsWindowEnabled() || !IsWindowVisible())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsWindowEnabled@CSubclassWnd@@QBEHXZ	; CSubclassWnd::IsWindowEnabled
	test	eax, eax
	je	SHORT $L76157
	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsWindowVisible@CSubclassWnd@@QBEHXZ	; CSubclassWnd::IsWindowVisible
	test	eax, eax
	jne	SHORT $L76156
$L76157:

; 194  : 		return FALSE;

	xor	eax, eax
	jmp	$L76155
$L76156:

; 195  : 
; 196  : 	// we only process keypresses
; 197  : 	if (pMsg->message != WM_KEYDOWN && pMsg->message != WM_SYSKEYDOWN)

	mov	eax, DWORD PTR _pMsg$[ebp]
	cmp	DWORD PTR [eax+4], 256			; 00000100H
	je	SHORT $L76158
	mov	ecx, DWORD PTR _pMsg$[ebp]
	cmp	DWORD PTR [ecx+4], 260			; 00000104H
	je	SHORT $L76158

; 198  : 		return FALSE;

	xor	eax, eax
	jmp	$L76155
$L76158:

; 199  : 
; 200  : 	// also check that it's one of our children with the focus
; 201  : 	// not another popup window, unless it's an edit control
; 202  : 	HWND hFocus = pMsg->hwnd;

	mov	edx, DWORD PTR _pMsg$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _hFocus$[ebp], eax

; 203  : 	HWND hMainWnd = GetHwnd();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ	; CSubclassWnd::GetHwnd
	mov	DWORD PTR _hMainWnd$[ebp], eax

; 204  : 
; 205  : 	if ((hFocus != hMainWnd) && !::IsChild(hMainWnd, hFocus))

	mov	ecx, DWORD PTR _hFocus$[ebp]
	cmp	ecx, DWORD PTR _hMainWnd$[ebp]
	je	SHORT $L76163
	mov	esi, esp
	mov	edx, DWORD PTR _hFocus$[ebp]
	push	edx
	mov	eax, DWORD PTR _hMainWnd$[ebp]
	push	eax
	call	DWORD PTR __imp__IsChild@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76163

; 207  : 		UINT nStyle = ::GetWindowLong(hFocus, GWL_STYLE);

	mov	esi, esp
	push	-16					; fffffff0H
	mov	ecx, DWORD PTR _hFocus$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetWindowLongW@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nStyle$76162[ebp], eax

; 208  : 		
; 209  : 		if (!(nStyle & WS_POPUP) || !CWinClasses::IsClass(hFocus, WC_EDIT))

	mov	edx, DWORD PTR _nStyle$76162[ebp]
	and	edx, -2147483648			; 80000000H
	test	edx, edx
	je	SHORT $L76164
	push	OFFSET FLAT:$SG76165
	mov	eax, DWORD PTR _hFocus$[ebp]
	push	eax
	call	?IsClass@CWinClasses@@SAHPAUHWND__@@PBG@Z ; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	jne	SHORT $L76163
$L76164:

; 210  : 			return FALSE;

	xor	eax, eax
	jmp	$L76155
$L76163:

; 214  : 	{

	mov	ecx, DWORD PTR _pMsg$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	DWORD PTR -40+[ebp], edx
	mov	eax, DWORD PTR -40+[ebp]
	sub	eax, 3
	mov	DWORD PTR -40+[ebp], eax
	cmp	DWORD PTR -40+[ebp], 142		; 0000008eH
	ja	SHORT $L76173
	mov	edx, DWORD PTR -40+[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR $L77957[edx]
	jmp	DWORD PTR $L77958[ecx*4]
$L76170:

; 215  : 	case VK_CONTROL:
; 216  : 	case VK_SHIFT:
; 217  : 	case VK_MENU:
; 218  : 	case VK_NUMLOCK:
; 219  : 	case VK_SCROLL:
; 220  : 	case VK_CAPITAL:
; 221  : 		return FALSE;

	xor	eax, eax
	jmp	$L76155
$L76171:

; 222  : 		
; 223  : 		// don't handle return/cancel keys
; 224  : 	case VK_RETURN:
; 225  : 	case VK_CANCEL:
; 226  : 		return FALSE;

	xor	eax, eax
	jmp	$L76155
$L76172:

; 227  : 
; 228  : 	case VK_MBUTTON:
; 229  : 		break;

	jmp	$L76167
$L76173:

; 234  : 			// don't process messages destined for hotkey controls!
; 235  : 			if (CWinClasses::IsClass(hFocus, WC_HOTKEY))

	push	OFFSET FLAT:$SG76175
	mov	eax, DWORD PTR _hFocus$[ebp]
	push	eax
	call	?IsClass@CWinClasses@@SAHPAUHWND__@@PBG@Z ; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L76174

; 236  : 				return FALSE;

	xor	eax, eax
	jmp	$L76155
$L76174:

; 237  : 
; 238  : 			// don't process AltGr if destined for edit control
; 239  : 			BOOL bEdit = CWinClasses::IsEditControl(hFocus);

	mov	ecx, DWORD PTR _hFocus$[ebp]
	push	ecx
	call	?IsEditControl@CWinClasses@@SAHPAUHWND__@@@Z ; CWinClasses::IsEditControl
	add	esp, 4
	mov	DWORD PTR _bEdit$76176[ebp], eax

; 240  : 
; 241  : 			if (bEdit && Misc::KeyIsPressed(VK_RMENU))

	cmp	DWORD PTR _bEdit$76176[ebp], 0
	je	SHORT $L76177
	push	165					; 000000a5H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	je	SHORT $L76177

; 242  : 				return FALSE;

	xor	eax, eax
	jmp	$L76155
$L76177:

; 243  : 			
; 244  : 			// get DWORD shortcut
; 245  : 			BOOL bExtKey = (pMsg->lParam & 0x01000000);

	mov	edx, DWORD PTR _pMsg$[ebp]
	mov	eax, DWORD PTR [edx+12]
	and	eax, 16777216				; 01000000H
	mov	DWORD PTR _bExtKey$76178[ebp], eax

; 246  : 			DWORD dwShortcut = GetShortcut((WORD)pMsg->wParam, bExtKey);

	mov	ecx, DWORD PTR _bExtKey$76178[ebp]
	push	ecx
	mov	edx, DWORD PTR _pMsg$[ebp]
	mov	ax, WORD PTR [edx+8]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetShortcut@CShortcutManager@@IBEKGH@Z	; CShortcutManager::GetShortcut
	mov	DWORD PTR _dwShortcut$76179[ebp], eax

; 247  : 			
; 248  : 			// look it up
; 249  : 			UINT nCmdID = 0;

	mov	DWORD PTR _nCmdID$76181[ebp], 0

; 250  : 			
; 251  : 			if (!m_mapShortcut2ID.Lookup(dwShortcut, nCmdID) || !nCmdID)

	lea	ecx, DWORD PTR _nCmdID$76181[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwShortcut$76179[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	?Lookup@?$CMap@KKIAAI@@QBEHKAAI@Z	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::Lookup
	test	eax, eax
	je	SHORT $L76183
	cmp	DWORD PTR _nCmdID$76181[ebp], 0
	jne	SHORT $L76182
$L76183:

; 252  : 				return FALSE;

	xor	eax, eax
	jmp	$L76155
$L76182:

; 253  : 			
; 254  : 			// check if HKCOMB_EDITCTRLS is set and a edit has the focus
; 255  : 			// and the shortcut clashes
; 256  : 			if (bEdit && (m_wInvalidComb & HKCOMB_EDITCTRLS))

	cmp	DWORD PTR _bEdit$76176[ebp], 0
	je	SHORT $L76191
	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax+88]
	and	ecx, 512				; 00000200H
	test	ecx, ecx
	je	SHORT $L76191

; 258  : 				// 1. check does not clash with edit shortcuts
; 259  : 				if (IsEditShortcut(dwShortcut))

	mov	edx, DWORD PTR _dwShortcut$76179[ebp]
	push	edx
	call	?IsEditShortcut@CShortcutManager@@KAHK@Z ; CShortcutManager::IsEditShortcut
	add	esp, 4
	test	eax, eax
	je	SHORT $L76185

; 260  : 					return FALSE;

	xor	eax, eax
	jmp	SHORT $L76155
$L76185:

; 261  : 				
; 262  : 				//WORD wVirtKeyCode = LOWORD(dwShortcut);
; 263  : 				WORD wModifiers = HIWORD(dwShortcut);

	mov	eax, DWORD PTR _dwShortcut$76179[ebp]
	shr	eax, 16					; 00000010H
	and	eax, 65535				; 0000ffffH
	mov	WORD PTR _wModifiers$76186[ebp], ax

; 264  : 				
; 265  : 				// 2. can be a function key
; 266  : 				if (pMsg->wParam >= VK_F1 && pMsg->wParam <= VK_F24)

	mov	ecx, DWORD PTR _pMsg$[ebp]
	cmp	DWORD PTR [ecx+8], 112			; 00000070H
	jb	SHORT $L76189
	mov	edx, DWORD PTR _pMsg$[ebp]
	cmp	DWORD PTR [edx+8], 135			; 00000087H
	ja	SHORT $L76189

; 270  : 				// 3. else must have <ctrl> or <alt>
; 271  : 				else if (!(wModifiers & (HOTKEYF_ALT | HOTKEYF_CONTROL)))

	jmp	SHORT $L76191
$L76189:
	mov	eax, DWORD PTR _wModifiers$76186[ebp]
	and	eax, 65535				; 0000ffffH
	and	eax, 6
	test	eax, eax
	jne	SHORT $L76191

; 272  : 					return FALSE;

	xor	eax, eax
	jmp	SHORT $L76155
$L76191:

; 274  : 			
; 275  : 			// return command ID
; 276  : 			if (m_bAutoSendCmds)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+92], 0
	je	SHORT $L76192

; 277  : 				SendMessage(WM_COMMAND, nCmdID);

	mov	esi, esp
	push	0
	mov	edx, DWORD PTR _nCmdID$76181[ebp]
	push	edx
	push	273					; 00000111H
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+36]
	cmp	esi, esp
	call	__chkesp
$L76192:

; 278  : 
; 279  : 			if (pShortcut)

	cmp	DWORD PTR _pShortcut$[ebp], 0
	je	SHORT $L76193

; 280  : 				*pShortcut = dwShortcut;

	mov	eax, DWORD PTR _pShortcut$[ebp]
	mov	ecx, DWORD PTR _dwShortcut$76179[ebp]
	mov	DWORD PTR [eax], ecx
$L76193:

; 281  : 			
; 282  : 			return nCmdID;

	mov	eax, DWORD PTR _nCmdID$76181[ebp]
	jmp	SHORT $L76155
$L76167:

; 285  : 	
; 286  : 	return FALSE;

	xor	eax, eax
$L76155:

; 287  : }

	pop	edi
	pop	esi
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
$L77958:
	DD	$L76171
	DD	$L76172
	DD	$L76170
	DD	$L76173
$L77957:
	DB	0
	DB	1
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	0
	DB	3
	DB	3
	DB	2
	DB	2
	DB	2
	DB	3
	DB	2
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	3
	DB	2
	DB	2
?ProcessMessage@CShortcutManager@@QBEIPBUtagMSG@@PAK@Z ENDP ; CShortcutManager::ProcessMessage
_TEXT	ENDS
;	COMDAT ?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ PROC NEAR	; CSubclassWnd::GetHwnd, COMDAT

; 58   : 	inline HWND GetHwnd() const { return m_hWndHooked; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+4]
	mov	esp, ebp
	pop	ebp
	ret	0
?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ ENDP		; CSubclassWnd::GetHwnd
_TEXT	ENDS
EXTRN	__imp__IsWindowEnabled@4:NEAR
;	COMDAT ?IsWindowEnabled@CSubclassWnd@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?IsWindowEnabled@CSubclassWnd@@QBEHXZ PROC NEAR		; CSubclassWnd::IsWindowEnabled, COMDAT

; 72   : 	inline BOOL IsWindowEnabled() const { return ::IsWindowEnabled(m_hWndHooked); }

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__IsWindowEnabled@4
	cmp	esi, esp
	call	__chkesp
	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsWindowEnabled@CSubclassWnd@@QBEHXZ ENDP		; CSubclassWnd::IsWindowEnabled
_TEXT	ENDS
EXTRN	__imp__IsWindowVisible@4:NEAR
;	COMDAT ?IsWindowVisible@CSubclassWnd@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?IsWindowVisible@CSubclassWnd@@QBEHXZ PROC NEAR		; CSubclassWnd::IsWindowVisible, COMDAT

; 73   : 	inline BOOL IsWindowVisible() const { return ::IsWindowVisible(m_hWndHooked); }

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__IsWindowVisible@4
	cmp	esi, esp
	call	__chkesp
	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsWindowVisible@CSubclassWnd@@QBEHXZ ENDP		; CSubclassWnd::IsWindowVisible
_TEXT	ENDS
;	COMDAT ?IsEditShortcut@CShortcutManager@@KAHK@Z
_TEXT	SEGMENT
_dwShortcut$ = 8
?IsEditShortcut@CShortcutManager@@KAHK@Z PROC NEAR	; CShortcutManager::IsEditShortcut, COMDAT

; 290  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 292  : 	{

	mov	eax, DWORD PTR _dwShortcut$[ebp]
	mov	DWORD PTR -4+[ebp], eax
	cmp	DWORD PTR -4+[ebp], 131160		; 00020058H
	ja	SHORT $L77968
	cmp	DWORD PTR -4+[ebp], 131160		; 00020058H
	je	SHORT $L76205
	cmp	DWORD PTR -4+[ebp], 46			; 0000002eH
	je	SHORT $L76205
	cmp	DWORD PTR -4+[ebp], 131139		; 00020043H
	je	SHORT $L76205
	cmp	DWORD PTR -4+[ebp], 131158		; 00020056H
	je	SHORT $L76205
	jmp	SHORT $L76198
$L77968:
	cmp	DWORD PTR -4+[ebp], 655397		; 000a0025H
	je	SHORT $L76205
	cmp	DWORD PTR -4+[ebp], 655399		; 000a0027H
	je	SHORT $L76205
	jmp	SHORT $L76198
$L76205:

; 293  : 	case MAKELONG('C', HOTKEYF_CONTROL): // copy
; 294  : 	case MAKELONG('V', HOTKEYF_CONTROL): // paste
; 295  : 	case MAKELONG('X', HOTKEYF_CONTROL): // cut
; 296  : //	case MAKELONG('Z', HOTKEYF_CONTROL): // undo
; 297  : 	case MAKELONG(VK_LEFT, HOTKEYF_CONTROL | HOTKEYF_EXT): // left one word
; 298  : 	case MAKELONG(VK_RIGHT, HOTKEYF_CONTROL | HOTKEYF_EXT): // right one word
; 299  : 	case MAKELONG(VK_DELETE, 0):
; 300  : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76196
$L76198:

; 302  : 
; 303  : 	// all else
; 304  : 	return FALSE;

	xor	eax, eax
$L76196:

; 305  : }

	mov	esp, ebp
	pop	ebp
	ret	0
?IsEditShortcut@CShortcutManager@@KAHK@Z ENDP		; CShortcutManager::IsEditShortcut
_TEXT	ENDS
PUBLIC	?PrepareMenuItems@CShortcutManager@@IBEXPAVCMenu@@@Z ; CShortcutManager::PrepareMenuItems
EXTRN	?FromHandle@CMenu@@SGPAV1@PAUHMENU__@@@Z:NEAR	; CMenu::FromHandle
EXTRN	?WindowProc@CSubclassWnd@@MAEJPAUHWND__@@IIJ@Z:NEAR ; CSubclassWnd::WindowProc
EXTRN	?Default@CSubclassWnd@@IAEJXZ:NEAR		; CSubclassWnd::Default
;	COMDAT ?WindowProc@CShortcutManager@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT
_hRealWnd$ = 8
_msg$ = 12
_wp$ = 16
_lp$ = 20
_this$ = -12
_lr$76238 = -4
_lr$76241 = -8
?WindowProc@CShortcutManager@@MAEJPAUHWND__@@IIJ@Z PROC NEAR ; CShortcutManager::WindowProc, COMDAT

; 308  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 310  : 	{

	mov	eax, DWORD PTR _msg$[ebp]
	mov	DWORD PTR -16+[ebp], eax
	cmp	DWORD PTR -16+[ebp], 2
	je	SHORT $L76240
	cmp	DWORD PTR -16+[ebp], 279		; 00000117H
	je	SHORT $L76237
	jmp	SHORT $L76234
$L76237:

; 313  : 			// default processing so all text changes 
; 314  : 			// are complete before we have a go
; 315  : 			LRESULT lr = Default();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Default@CSubclassWnd@@IAEJXZ		; CSubclassWnd::Default
	mov	DWORD PTR _lr$76238[ebp], eax

; 316  : 			PrepareMenuItems(CMenu::FromHandle((HMENU)wp));

	mov	ecx, DWORD PTR _wp$[ebp]
	push	ecx
	call	?FromHandle@CMenu@@SGPAV1@PAUHMENU__@@@Z ; CMenu::FromHandle
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?PrepareMenuItems@CShortcutManager@@IBEXPAVCMenu@@@Z ; CShortcutManager::PrepareMenuItems

; 317  : 
; 318  : 			return lr;

	mov	eax, DWORD PTR _lr$76238[ebp]
	jmp	SHORT $L76232
$L76240:

; 324  : 			// must call rest of chain first 
; 325  : 			LRESULT lr =  CSubclassWnd::WindowProc(hRealWnd, msg, wp, lp);

	mov	edx, DWORD PTR _lp$[ebp]
	push	edx
	mov	eax, DWORD PTR _wp$[ebp]
	push	eax
	mov	ecx, DWORD PTR _msg$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hRealWnd$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?WindowProc@CSubclassWnd@@MAEJPAUHWND__@@IIJ@Z ; CSubclassWnd::WindowProc
	mov	DWORD PTR _lr$76241[ebp], eax

; 326  : 			HookWindow(NULL);

	mov	esi, esp
	push	0
	push	0
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp

; 327  : 
; 328  : 			return lr;

	mov	eax, DWORD PTR _lr$76241[ebp]
	jmp	SHORT $L76232
$L76234:

; 332  : 
; 333  : 	return CSubclassWnd::WindowProc(hRealWnd, msg, wp, lp);

	mov	eax, DWORD PTR _lp$[ebp]
	push	eax
	mov	ecx, DWORD PTR _wp$[ebp]
	push	ecx
	mov	edx, DWORD PTR _msg$[ebp]
	push	edx
	mov	eax, DWORD PTR _hRealWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?WindowProc@CSubclassWnd@@MAEJPAUHWND__@@IIJ@Z ; CSubclassWnd::WindowProc
$L76232:

; 334  : }

	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?WindowProc@CShortcutManager@@MAEJPAUHWND__@@IIJ@Z ENDP	; CShortcutManager::WindowProc
_TEXT	ENDS
PUBLIC	?GetShortcut@CShortcutManager@@QBEKI@Z		; CShortcutManager::GetShortcut
PUBLIC	?GetShortcutText@CShortcutManager@@SA?AVCString@@K@Z ; CShortcutManager::GetShortcutText
EXTRN	?GetSafeHmenu@CMenu@@QBEPAUHMENU__@@XZ:NEAR	; CMenu::GetSafeHmenu
EXTRN	?GetMenuItemCount@CMenu@@QBEIXZ:NEAR		; CMenu::GetMenuItemCount
EXTRN	?GetMenuItemID@CMenu@@QBEIH@Z:NEAR		; CMenu::GetMenuItemID
EXTRN	__imp__GetMenuItemInfoW@16:NEAR
EXTRN	__imp__SetMenuItemInfoW@16:NEAR
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
EXTRN	??YCString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator+=
EXTRN	??YCString@@QAEABV0@D@Z:NEAR			; CString::operator+=
EXTRN	?Left@CString@@QBE?AV1@H@Z:NEAR			; CString::Left
EXTRN	?Find@CString@@QBEHG@Z:NEAR			; CString::Find
EXTRN	?GetBuffer@CString@@QAEPAGH@Z:NEAR		; CString::GetBuffer
EXTRN	?ReleaseBuffer@CString@@QAEXH@Z:NEAR		; CString::ReleaseBuffer
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT xdata$x
; File D:\_CODE\Shared\ShortcutManager.cpp
xdata$x	SEGMENT
$T77978	DD	019930520H
	DD	03H
	DD	FLAT:$T77980
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77980	DD	0ffffffffH
	DD	FLAT:$L77974
	DD	00H
	DD	FLAT:$L77975
	DD	00H
	DD	FLAT:$L77976
xdata$x	ENDS
;	COMDAT ?PrepareMenuItems@CShortcutManager@@IBEXPAVCMenu@@@Z
_TEXT	SEGMENT
$T77973 = -88
__$EHRec$ = -12
_pMenu$ = 8
_this$ = -92
_nItem$ = -16
_nCmdID$76252 = -76
_dwShortcut$76253 = -20
_minfo$76257 = -68
_sMenuText$76258 = -84
_nTab$76260 = -72
_sShortcut$76265 = -80
?PrepareMenuItems@CShortcutManager@@IBEXPAVCMenu@@@Z PROC NEAR ; CShortcutManager::PrepareMenuItems, COMDAT

; 337  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77979
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 88					; 00000058H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-100]
	mov	ecx, 22					; 00000016H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 338  : 	if (!pMenu || !pMenu->GetSafeHmenu())

	cmp	DWORD PTR _pMenu$[ebp], 0
	je	SHORT $L76247
	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetSafeHmenu@CMenu@@QBEPAUHMENU__@@XZ	; CMenu::GetSafeHmenu
	test	eax, eax
	jne	SHORT $L76246
$L76247:

; 339  : 		return;

	jmp	$L76245
$L76246:

; 340  : 
; 341  : 	// we iterate all the menu items
; 342  : 	// if we find a match we get the menu text and add the shortcut
; 343  : 	// first removing any existing one
; 344  : 	int nItem = pMenu->GetMenuItemCount();

	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetMenuItemCount@CMenu@@QBEIXZ		; CMenu::GetMenuItemCount
	mov	DWORD PTR _nItem$[ebp], eax
$L76250:

; 345  : 
; 346  : 	while (nItem--)

	mov	eax, DWORD PTR _nItem$[ebp]
	mov	ecx, DWORD PTR _nItem$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nItem$[ebp], ecx
	test	eax, eax
	je	$L76251

; 348  : 		UINT nCmdID = pMenu->GetMenuItemID(nItem);

	mov	edx, DWORD PTR _nItem$[ebp]
	push	edx
	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetMenuItemID@CMenu@@QBEIH@Z		; CMenu::GetMenuItemID
	mov	DWORD PTR _nCmdID$76252[ebp], eax

; 349  : 		DWORD dwShortcut = GetShortcut(nCmdID);

	mov	eax, DWORD PTR _nCmdID$76252[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetShortcut@CShortcutManager@@QBEKI@Z	; CShortcutManager::GetShortcut
	mov	DWORD PTR _dwShortcut$76253[ebp], eax

; 350  : 
; 351  : 		if (!nCmdID || nCmdID == (UINT)-1)

	cmp	DWORD PTR _nCmdID$76252[ebp], 0
	je	SHORT $L76256
	cmp	DWORD PTR _nCmdID$76252[ebp], -1
	jne	SHORT $L76255
$L76256:

; 352  : 			continue;

	jmp	SHORT $L76250
$L76255:

; 359  : 		minfo.cbSize = sizeof(minfo);

	mov	DWORD PTR _minfo$76257[ebp], 48		; 00000030H

; 360  :         minfo.fMask = MIIM_FTYPE | MIIM_STRING;

	mov	DWORD PTR _minfo$76257[ebp+4], 320	; 00000140H

; 361  :         minfo.fType = MFT_STRING;

	mov	DWORD PTR _minfo$76257[ebp+8], 0

; 362  : 		minfo.dwTypeData = NULL;

	mov	DWORD PTR _minfo$76257[ebp+36], 0

; 363  : 		minfo.cch = 0;

	mov	DWORD PTR _minfo$76257[ebp+40], 0

; 364  : 
; 365  :         ::GetMenuItemInfo(pMenu->GetSafeHmenu(), nItem, TRUE, &minfo);

	mov	esi, esp
	lea	ecx, DWORD PTR _minfo$76257[ebp]
	push	ecx
	push	1
	mov	edx, DWORD PTR _nItem$[ebp]
	push	edx
	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetSafeHmenu@CMenu@@QBEPAUHMENU__@@XZ	; CMenu::GetSafeHmenu
	push	eax
	call	DWORD PTR __imp__GetMenuItemInfoW@16
	cmp	esi, esp
	call	__chkesp

; 366  : 		CString sMenuText;

	lea	ecx, DWORD PTR _sMenuText$76258[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 367  : 
; 368  : 		if (!minfo.cch)

	cmp	DWORD PTR _minfo$76257[ebp+40], 0
	jne	SHORT $L76259

; 369  : 			continue; // ??

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sMenuText$76258[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	$L76250
$L76259:

; 370  : 
; 371  : 		minfo.cch++;

	mov	eax, DWORD PTR _minfo$76257[ebp+40]
	add	eax, 1
	mov	DWORD PTR _minfo$76257[ebp+40], eax

; 372  : 		minfo.dwTypeData = sMenuText.GetBuffer(minfo.cch);

	mov	ecx, DWORD PTR _minfo$76257[ebp+40]
	push	ecx
	lea	ecx, DWORD PTR _sMenuText$76258[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	mov	DWORD PTR _minfo$76257[ebp+36], eax

; 373  : 		::GetMenuItemInfo(pMenu->GetSafeHmenu(), nItem, TRUE, &minfo);

	mov	esi, esp
	lea	edx, DWORD PTR _minfo$76257[ebp]
	push	edx
	push	1
	mov	eax, DWORD PTR _nItem$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetSafeHmenu@CMenu@@QBEPAUHMENU__@@XZ	; CMenu::GetSafeHmenu
	push	eax
	call	DWORD PTR __imp__GetMenuItemInfoW@16
	cmp	esi, esp
	call	__chkesp

; 374  : 		sMenuText.ReleaseBuffer();

	push	-1
	lea	ecx, DWORD PTR _sMenuText$76258[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 375  : 
; 376  : 		// look for '\t' indicating existing hint
; 377  : 		int nTab = sMenuText.Find('\t');

	push	9
	lea	ecx, DWORD PTR _sMenuText$76258[ebp]
	call	?Find@CString@@QBEHG@Z			; CString::Find
	mov	DWORD PTR _nTab$76260[ebp], eax

; 378  : 		
; 379  : 		// remove it
; 380  : 		if (nTab >= 0)

	cmp	DWORD PTR _nTab$76260[ebp], 0
	jl	SHORT $L76261

; 381  : 			sMenuText = sMenuText.Left(nTab);

	mov	ecx, DWORD PTR _nTab$76260[ebp]
	push	ecx
	lea	edx, DWORD PTR $T77973[ebp]
	push	edx
	lea	ecx, DWORD PTR _sMenuText$76258[ebp]
	call	?Left@CString@@QBE?AV1@H@Z		; CString::Left
	mov	DWORD PTR -96+[ebp], eax
	mov	eax, DWORD PTR -96+[ebp]
	mov	DWORD PTR -100+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -100+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sMenuText$76258[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T77973[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 382  : 		// else if it didn't have one and it has no shortcut then continue
; 383  : 		else if (!dwShortcut)

	jmp	SHORT $L76264
$L76261:
	cmp	DWORD PTR _dwShortcut$76253[ebp], 0
	jne	SHORT $L76264

; 384  : 			continue;

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sMenuText$76258[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	$L76250
$L76264:

; 385  : 		
; 386  : 		// add new hint
; 387  : 		CString sShortcut = GetShortcutText(dwShortcut);

	mov	edx, DWORD PTR _dwShortcut$76253[ebp]
	push	edx
	lea	eax, DWORD PTR _sShortcut$76265[ebp]
	push	eax
	call	?GetShortcutText@CShortcutManager@@SA?AVCString@@K@Z ; CShortcutManager::GetShortcutText
	add	esp, 8
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 388  : 
; 389  : 		if (!sShortcut.IsEmpty())

	lea	ecx, DWORD PTR _sShortcut$76265[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L76267

; 391  : 			sMenuText += '\t';

	push	9
	lea	ecx, DWORD PTR _sMenuText$76258[ebp]
	call	??YCString@@QAEABV0@D@Z			; CString::operator+=

; 392  : 			sMenuText += sShortcut;

	lea	ecx, DWORD PTR _sShortcut$76265[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sMenuText$76258[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
$L76267:

; 394  : 
; 395  : 		// update menu item text
; 396  : 		minfo.dwTypeData = (LPTSTR)(LPCTSTR)sMenuText;

	lea	ecx, DWORD PTR _sMenuText$76258[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR _minfo$76257[ebp+36], eax

; 397  : 		
; 398  :         ::SetMenuItemInfo(pMenu->GetSafeHmenu(), nItem, TRUE, &minfo);

	mov	esi, esp
	lea	edx, DWORD PTR _minfo$76257[ebp]
	push	edx
	push	1
	mov	eax, DWORD PTR _nItem$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetSafeHmenu@CMenu@@QBEPAUHMENU__@@XZ	; CMenu::GetSafeHmenu
	push	eax
	call	DWORD PTR __imp__SetMenuItemInfoW@16
	cmp	esi, esp
	call	__chkesp

; 399  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sShortcut$76265[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sMenuText$76258[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	$L76250
$L76251:
$L76245:

; 400  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 100				; 00000064H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77974:
	lea	ecx, DWORD PTR _sMenuText$76258[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77975:
	lea	ecx, DWORD PTR $T77973[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77976:
	lea	ecx, DWORD PTR _sShortcut$76265[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77979:
	mov	eax, OFFSET FLAT:$T77978
	jmp	___CxxFrameHandler
text$x	ENDS
?PrepareMenuItems@CShortcutManager@@IBEXPAVCMenu@@@Z ENDP ; CShortcutManager::PrepareMenuItems
PUBLIC	?BuildMapping@CShortcutManager@@QAEHIAAVCStringArray@@D@Z ; CShortcutManager::BuildMapping
PUBLIC	?BuildMapping@CShortcutManager@@IAEHPAVCMenu@@PBGAAVCStringArray@@D@Z ; CShortcutManager::BuildMapping
EXTRN	??0CEnMenu@@QAE@XZ:NEAR				; CEnMenu::CEnMenu
EXTRN	??1CEnMenu@@UAE@XZ:NEAR				; CEnMenu::~CEnMenu
EXTRN	?LoadMenuW@CEnMenu@@QAEHIPAUHWND__@@H@Z:NEAR	; CEnMenu::LoadMenuW
;	COMDAT xdata$x
; File D:\_CODE\Shared\ShortcutManager.cpp
xdata$x	SEGMENT
$T77989	DD	019930520H
	DD	01H
	DD	FLAT:$T77991
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77991	DD	0ffffffffH
	DD	FLAT:$L77987
xdata$x	ENDS
;	COMDAT ?BuildMapping@CShortcutManager@@QAEHIAAVCStringArray@@D@Z
_TEXT	SEGMENT
$T77985 = -60
$T77986 = -64
__$EHRec$ = -12
_nMenuID$ = 8
_aMapping$ = 12
_cDelim$ = 16
_this$ = -68
_menu$ = -56
?BuildMapping@CShortcutManager@@QAEHIAAVCStringArray@@D@Z PROC NEAR ; CShortcutManager::BuildMapping, COMDAT

; 403  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77990
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 56					; 00000038H
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-68]
	mov	ecx, 14					; 0000000eH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 404  : 	CEnMenu menu;

	lea	ecx, DWORD PTR _menu$[ebp]
	call	??0CEnMenu@@QAE@XZ			; CEnMenu::CEnMenu
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 405  : 
; 406  : 	if (!menu.LoadMenu(nMenuID, GetHwnd(), TRUE) || !menu.GetMenuItemCount())

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ	; CSubclassWnd::GetHwnd
	push	eax
	mov	eax, DWORD PTR _nMenuID$[ebp]
	push	eax
	lea	ecx, DWORD PTR _menu$[ebp]
	call	?LoadMenuW@CEnMenu@@QAEHIPAUHWND__@@H@Z	; CEnMenu::LoadMenuW
	test	eax, eax
	je	SHORT $L76278
	lea	ecx, DWORD PTR _menu$[ebp]
	call	?GetMenuItemCount@CMenu@@QBEIXZ		; CMenu::GetMenuItemCount
	test	eax, eax
	jne	SHORT $L76277
$L76278:

; 407  : 		return 0;

	mov	DWORD PTR $T77985[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _menu$[ebp]
	call	??1CEnMenu@@UAE@XZ			; CEnMenu::~CEnMenu
	mov	eax, DWORD PTR $T77985[ebp]
	jmp	SHORT $L76275
$L76277:

; 408  : 
; 409  : 	return BuildMapping(&menu, NULL, aMapping, cDelim);

	mov	cl, BYTE PTR _cDelim$[ebp]
	push	ecx
	mov	edx, DWORD PTR _aMapping$[ebp]
	push	edx
	push	0
	lea	eax, DWORD PTR _menu$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?BuildMapping@CShortcutManager@@IAEHPAVCMenu@@PBGAAVCStringArray@@D@Z ; CShortcutManager::BuildMapping
	mov	DWORD PTR $T77986[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _menu$[ebp]
	call	??1CEnMenu@@UAE@XZ			; CEnMenu::~CEnMenu
	mov	eax, DWORD PTR $T77986[ebp]
$L76275:

; 410  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 68					; 00000044H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77987:
	lea	ecx, DWORD PTR _menu$[ebp]
	call	??1CEnMenu@@UAE@XZ			; CEnMenu::~CEnMenu
	ret	0
$L77990:
	mov	eax, OFFSET FLAT:$T77989
	jmp	___CxxFrameHandler
text$x	ENDS
?BuildMapping@CShortcutManager@@QAEHIAAVCStringArray@@D@Z ENDP ; CShortcutManager::BuildMapping
EXTRN	?GetMenuStringW@CMenu@@QBEHIAAVCString@@I@Z:NEAR ; CMenu::GetMenuStringW
EXTRN	?GetSubMenu@CMenu@@QBEPAV1@H@Z:NEAR		; CMenu::GetSubMenu
EXTRN	?GetSize@CStringArray@@QBEHXZ:NEAR		; CStringArray::GetSize
EXTRN	?Add@CStringArray@@QAEHABVCString@@@Z:NEAR	; CStringArray::Add
EXTRN	??ACStringArray@@QAEAAVCString@@H@Z:NEAR	; CStringArray::operator[]
EXTRN	??0CString@@QAE@PBD@Z:NEAR			; CString::CString
EXTRN	?Replace@CString@@QAEHPBG0@Z:NEAR		; CString::Replace
EXTRN	?Remove@CString@@QAEHG@Z:NEAR			; CString::Remove
EXTRN	?Format@CString@@QAAXPBGZZ:NEAR			; CString::Format
_BSS	SEGMENT
$SG76315 DB	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\ShortcutManager.cpp
data	SEGMENT
$SG76296 DB	'%', 00H, 's', 00H, ' ', 00H, '>', 00H, ' ', 00H, '%', 00H
	DB	's', 00H, 00H, 00H
$SG76308 DB	'%', 00H, 's', 00H, '%', 00H, 'c', 00H, '%', 00H, 's', 00H
	DB	00H, 00H
	ORG $+2
$SG76309 DB	'~', 00H, '~', 00H, 00H, 00H
	ORG $+2
$SG76310 DB	'&', 00H, '&', 00H, 00H, 00H
	ORG $+2
$SG76311 DB	'&', 00H, '&', 00H, 00H, 00H
	ORG $+2
$SG76312 DB	'~', 00H, '~', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T78003	DD	019930520H
	DD	05H
	DD	FLAT:$T78005
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T78005	DD	0ffffffffH
	DD	FLAT:$L77997
	DD	00H
	DD	FLAT:$L77998
	DD	01H
	DD	FLAT:$L77999
	DD	02H
	DD	FLAT:$L78000
	DD	0ffffffffH
	DD	FLAT:$L78001
xdata$x	ENDS
;	COMDAT ?BuildMapping@CShortcutManager@@IAEHPAVCMenu@@PBGAAVCStringArray@@D@Z
_TEXT	SEGMENT
$T77996 = -52
__$EHRec$ = -12
_pMenu$ = 8
_szParentName$ = 12
_aMapping$ = 16
_cDelim$ = 20
_this$ = -56
_nItems$ = -20
_nItem$ = -16
_sMenuText$76293 = -28
_sItemText$76294 = -32
_nCmdID$76298 = -24
_pSubMenu$76301 = -36
_dwShortcut$76303 = -40
_sShortcut$76305 = -48
_sItem$76307 = -44
?BuildMapping@CShortcutManager@@IAEHPAVCMenu@@PBGAAVCStringArray@@D@Z PROC NEAR ; CShortcutManager::BuildMapping, COMDAT

; 414  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L78004
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 44					; 0000002cH
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-56]
	mov	ecx, 11					; 0000000bH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 415  : 	int nItems = pMenu->GetMenuItemCount();

	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetMenuItemCount@CMenu@@QBEIXZ		; CMenu::GetMenuItemCount
	mov	DWORD PTR _nItems$[ebp], eax

; 416  : 
; 417  : 	for (int nItem = 0; nItem < nItems; nItem++)

	mov	DWORD PTR _nItem$[ebp], 0
	jmp	SHORT $L76290
$L76291:
	mov	eax, DWORD PTR _nItem$[ebp]
	add	eax, 1
	mov	DWORD PTR _nItem$[ebp], eax
$L76290:
	mov	ecx, DWORD PTR _nItem$[ebp]
	cmp	ecx, DWORD PTR _nItems$[ebp]
	jge	$L76292

; 419  : 		CString sMenuText, sItemText;

	lea	ecx, DWORD PTR _sMenuText$76293[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sItemText$76294[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 420  : 
; 421  : 		pMenu->GetMenuString(nItem, sMenuText, MF_BYPOSITION);

	push	1024					; 00000400H
	lea	edx, DWORD PTR _sMenuText$76293[ebp]
	push	edx
	mov	eax, DWORD PTR _nItem$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetMenuStringW@CMenu@@QBEHIAAVCString@@I@Z ; CMenu::GetMenuStringW

; 422  : 
; 423  : 		if (szParentName && *szParentName)

	cmp	DWORD PTR _szParentName$[ebp], 0
	je	SHORT $L76295
	mov	ecx, DWORD PTR _szParentName$[ebp]
	xor	edx, edx
	mov	dx, WORD PTR [ecx]
	test	edx, edx
	je	SHORT $L76295

; 424  : 			sItemText.Format(_T("%s > %s"), szParentName, sMenuText);

	mov	eax, DWORD PTR _sMenuText$76293[ebp]
	push	eax
	mov	ecx, DWORD PTR _szParentName$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG76296
	lea	edx, DWORD PTR _sItemText$76294[ebp]
	push	edx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 16					; 00000010H

; 425  : 		else

	jmp	SHORT $L76297
$L76295:

; 426  : 			sItemText = sMenuText;

	lea	eax, DWORD PTR _sMenuText$76293[ebp]
	push	eax
	lea	ecx, DWORD PTR _sItemText$76294[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
$L76297:

; 427  : 
; 428  : 		UINT nCmdID = pMenu->GetMenuItemID(nItem);

	mov	ecx, DWORD PTR _nItem$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetMenuItemID@CMenu@@QBEIH@Z		; CMenu::GetMenuItemID
	mov	DWORD PTR _nCmdID$76298[ebp], eax

; 429  : 
; 430  : 		if (nCmdID == (UINT)-1) // sub menu

	cmp	DWORD PTR _nCmdID$76298[ebp], -1
	jne	SHORT $L76300

; 432  : 			CMenu* pSubMenu = pMenu->GetSubMenu(nItem);

	mov	edx, DWORD PTR _nItem$[ebp]
	push	edx
	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetSubMenu@CMenu@@QBEPAV1@H@Z		; CMenu::GetSubMenu
	mov	DWORD PTR _pSubMenu$76301[ebp], eax

; 433  : 
; 434  : 			BuildMapping(pSubMenu, sItemText, aMapping, cDelim);

	mov	al, BYTE PTR _cDelim$[ebp]
	push	eax
	mov	ecx, DWORD PTR _aMapping$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sItemText$76294[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	edx, DWORD PTR _pSubMenu$76301[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?BuildMapping@CShortcutManager@@IAEHPAVCMenu@@PBGAAVCStringArray@@D@Z ; CShortcutManager::BuildMapping

; 436  : 		else

	jmp	$L76304
$L76300:

; 438  : 			DWORD dwShortcut = GetShortcut(nCmdID);

	mov	eax, DWORD PTR _nCmdID$76298[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetShortcut@CShortcutManager@@QBEKI@Z	; CShortcutManager::GetShortcut
	mov	DWORD PTR _dwShortcut$76303[ebp], eax

; 439  : 
; 440  : 			if (dwShortcut)

	cmp	DWORD PTR _dwShortcut$76303[ebp], 0
	je	$L76304

; 442  : 				CString sShortcut = GetShortcutText(dwShortcut), sItem;

	mov	ecx, DWORD PTR _dwShortcut$76303[ebp]
	push	ecx
	lea	edx, DWORD PTR _sShortcut$76305[ebp]
	push	edx
	call	?GetShortcutText@CShortcutManager@@SA?AVCString@@K@Z ; CShortcutManager::GetShortcutText
	add	esp, 8
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sItem$76307[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3

; 443  : 				sItem.Format(_T("%s%c%s"), sShortcut, cDelim, sItemText);

	mov	eax, DWORD PTR _sItemText$76294[ebp]
	push	eax
	movsx	ecx, BYTE PTR _cDelim$[ebp]
	push	ecx
	mov	edx, DWORD PTR _sShortcut$76305[ebp]
	push	edx
	push	OFFSET FLAT:$SG76308
	lea	eax, DWORD PTR _sItem$76307[ebp]
	push	eax
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 20					; 00000014H

; 444  : 
; 445  : 				// remove single '&'
; 446  : 				// rather painful way to do it to avoid replacing '&&'
; 447  : 				sItem.Replace(_T("&&"), _T("~~"));

	push	OFFSET FLAT:$SG76309
	push	OFFSET FLAT:$SG76310
	lea	ecx, DWORD PTR _sItem$76307[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 448  : 				sItem.Remove('&');

	push	38					; 00000026H
	lea	ecx, DWORD PTR _sItem$76307[ebp]
	call	?Remove@CString@@QAEHG@Z		; CString::Remove

; 449  : 				sItem.Replace(_T("~~"), _T("&&"));

	push	OFFSET FLAT:$SG76311
	push	OFFSET FLAT:$SG76312
	lea	ecx, DWORD PTR _sItem$76307[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 450  : 
; 451  : 				aMapping.Add(sItem);

	lea	ecx, DWORD PTR _sItem$76307[ebp]
	push	ecx
	mov	ecx, DWORD PTR _aMapping$[ebp]
	call	?Add@CStringArray@@QAEHABVCString@@@Z	; CStringArray::Add

; 452  : 			}

	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sItem$76307[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sShortcut$76305[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76304:

; 454  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sItemText$76294[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sMenuText$76293[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	$L76291
$L76292:

; 455  : 	
; 456  : 	// add a space between sections unless already added
; 457  : 	if (aMapping.GetSize() && !aMapping[aMapping.GetSize() - 1].IsEmpty())

	mov	ecx, DWORD PTR _aMapping$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	test	eax, eax
	je	SHORT $L76313
	mov	ecx, DWORD PTR _aMapping$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	sub	eax, 1
	push	eax
	mov	ecx, DWORD PTR _aMapping$[ebp]
	call	??ACStringArray@@QAEAAVCString@@H@Z	; CStringArray::operator[]
	mov	ecx, eax
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L76313

; 458  : 		aMapping.Add("");

	push	OFFSET FLAT:$SG76315
	lea	ecx, DWORD PTR $T77996[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 4
	lea	edx, DWORD PTR $T77996[ebp]
	push	edx
	mov	ecx, DWORD PTR _aMapping$[ebp]
	call	?Add@CStringArray@@QAEHABVCString@@@Z	; CStringArray::Add
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T77996[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76313:

; 459  : 
; 460  : 	return aMapping.GetSize();

	mov	ecx, DWORD PTR _aMapping$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize

; 461  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 56					; 00000038H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77997:
	lea	ecx, DWORD PTR _sMenuText$76293[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77998:
	lea	ecx, DWORD PTR _sItemText$76294[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77999:
	lea	ecx, DWORD PTR _sShortcut$76305[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L78000:
	lea	ecx, DWORD PTR _sItem$76307[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L78001:
	lea	ecx, DWORD PTR $T77996[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L78004:
	mov	eax, OFFSET FLAT:$T78003
	jmp	___CxxFrameHandler
text$x	ENDS
?BuildMapping@CShortcutManager@@IAEHPAVCMenu@@PBGAAVCStringArray@@D@Z ENDP ; CShortcutManager::BuildMapping
PUBLIC	?GetCommandID@CShortcutManager@@QAEIK@Z		; CShortcutManager::GetCommandID
;	COMDAT ?GetCommandID@CShortcutManager@@QAEIK@Z
_TEXT	SEGMENT
_dwShortcut$ = 8
_this$ = -8
_nCmdID$ = -4
?GetCommandID@CShortcutManager@@QAEIK@Z PROC NEAR	; CShortcutManager::GetCommandID, COMDAT

; 464  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 465  : 	UINT nCmdID = 0;

	mov	DWORD PTR _nCmdID$[ebp], 0

; 466  : 
; 467  : 	m_mapShortcut2ID.Lookup(dwShortcut, nCmdID);

	lea	eax, DWORD PTR _nCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwShortcut$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	?Lookup@?$CMap@KKIAAI@@QBEHKAAI@Z	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::Lookup

; 468  : 
; 469  : 	return nCmdID;

	mov	eax, DWORD PTR _nCmdID$[ebp]

; 470  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetCommandID@CShortcutManager@@QAEIK@Z ENDP		; CShortcutManager::GetCommandID
_TEXT	ENDS
;	COMDAT ?GetShortcut@CShortcutManager@@QBEKI@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_this$ = -8
_dwShortcut$ = -4
?GetShortcut@CShortcutManager@@QBEKI@Z PROC NEAR	; CShortcutManager::GetShortcut, COMDAT

; 473  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 474  : 	DWORD dwShortcut = 0;

	mov	DWORD PTR _dwShortcut$[ebp], 0

; 475  : 
; 476  : 	m_mapID2Shortcut.Lookup(nCmdID, dwShortcut);

	lea	eax, DWORD PTR _dwShortcut$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nCmdID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	?Lookup@?$CMap@IIKAAK@@QBEHIAAK@Z	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::Lookup

; 477  : 	
; 478  : 	return (dwShortcut == NO_SHORTCUT) ? 0 : dwShortcut;

	mov	eax, DWORD PTR _dwShortcut$[ebp]
	sub	eax, -1
	neg	eax
	sbb	eax, eax
	and	eax, DWORD PTR _dwShortcut$[ebp]

; 479  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetShortcut@CShortcutManager@@QBEKI@Z ENDP		; CShortcutManager::GetShortcut
_TEXT	ENDS
PUBLIC	?DeleteShortcut@CShortcutManager@@QAEXI@Z	; CShortcutManager::DeleteShortcut
PUBLIC	?RemoveKey@?$CMap@IIKAAK@@QAEHI@Z		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::RemoveKey
;	COMDAT ?DeleteShortcut@CShortcutManager@@QAEXI@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_this$ = -8
_dwShortcut$ = -4
?DeleteShortcut@CShortcutManager@@QAEXI@Z PROC NEAR	; CShortcutManager::DeleteShortcut, COMDAT

; 482  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 483  : 	DWORD dwShortcut = 0;

	mov	DWORD PTR _dwShortcut$[ebp], 0

; 484  : 
; 485  : 	if (m_mapID2Shortcut.Lookup(nCmdID, dwShortcut))

	lea	eax, DWORD PTR _dwShortcut$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nCmdID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	?Lookup@?$CMap@IIKAAK@@QBEHIAAK@Z	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::Lookup
	test	eax, eax
	je	SHORT $L76331

; 487  : 		m_mapID2Shortcut.RemoveKey(nCmdID);

	mov	edx, DWORD PTR _nCmdID$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	?RemoveKey@?$CMap@IIKAAK@@QAEHI@Z	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::RemoveKey

; 488  : 
; 489  : 		// remove reverse mapping too
; 490  : 		m_mapShortcut2ID.RemoveKey(dwShortcut);

	mov	eax, DWORD PTR _dwShortcut$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	?RemoveKey@?$CMap@KKIAAI@@QAEHK@Z	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::RemoveKey
$L76331:

; 492  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?DeleteShortcut@CShortcutManager@@QAEXI@Z ENDP		; CShortcutManager::DeleteShortcut
_TEXT	ENDS
PUBLIC	?GetShortcutTextByCmd@CShortcutManager@@QAE?AVCString@@I@Z ; CShortcutManager::GetShortcutTextByCmd
;	COMDAT ?GetShortcutTextByCmd@CShortcutManager@@QAE?AVCString@@I@Z
_TEXT	SEGMENT
$T78018 = -4
_nCmdID$ = 12
___$ReturnUdt$ = 8
_this$ = -8
?GetShortcutTextByCmd@CShortcutManager@@QAE?AVCString@@I@Z PROC NEAR ; CShortcutManager::GetShortcutTextByCmd, COMDAT

; 495  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T78018[ebp], 0

; 496  : 	return GetShortcutText(GetShortcut(nCmdID));

	mov	eax, DWORD PTR _nCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetShortcut@CShortcutManager@@QBEKI@Z	; CShortcutManager::GetShortcut
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	call	?GetShortcutText@CShortcutManager@@SA?AVCString@@K@Z ; CShortcutManager::GetShortcutText
	add	esp, 8
	mov	edx, DWORD PTR $T78018[ebp]
	or	edx, 1
	mov	DWORD PTR $T78018[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 497  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?GetShortcutTextByCmd@CShortcutManager@@QAE?AVCString@@I@Z ENDP ; CShortcutManager::GetShortcutTextByCmd
_TEXT	ENDS
EXTRN	?GetKeyName@Misc@@YA?AVCString@@GH@Z:NEAR	; Misc::GetKeyName
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
EXTRN	?Empty@CString@@QAEXXZ:NEAR			; CString::Empty
EXTRN	??YCString@@QAEABV0@PBG@Z:NEAR			; CString::operator+=
_BSS	SEGMENT
	ALIGN	4

$SG76344 DB	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\ShortcutManager.cpp
data	SEGMENT
$SG76353 DB	'+', 00H, 00H, 00H
$SG76356 DB	'+', 00H, 00H, 00H
$SG76359 DB	'+', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T78034	DD	019930520H
	DD	06H
	DD	FLAT:$T78036
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T78036	DD	0ffffffffH
	DD	FLAT:$L78025
	DD	00H
	DD	FLAT:$L78028
	DD	01H
	DD	FLAT:$L78029
	DD	01H
	DD	FLAT:$L78030
	DD	01H
	DD	FLAT:$L78031
	DD	01H
	DD	FLAT:$L78032
xdata$x	ENDS
;	COMDAT ?GetShortcutText@CShortcutManager@@SA?AVCString@@K@Z
_TEXT	SEGMENT
$T78022 = -32
$T78023 = -36
$T78024 = -40
$T78026 = -44
__$EHRec$ = -12
_dwShortcut$ = 12
___$ReturnUdt$ = 8
_sText$ = -24
_wVirtKeyCode$ = -20
_wModifiers$ = -28
_sKey$ = -16
?GetShortcutText@CShortcutManager@@SA?AVCString@@K@Z PROC NEAR ; CShortcutManager::GetShortcutText, COMDAT

; 500  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L78035
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 56					; 00000038H
	push	edi
	lea	edi, DWORD PTR [ebp-68]
	mov	ecx, 14					; 0000000eH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T78026[ebp], 0

; 501  : 	if (!dwShortcut || dwShortcut == NO_SHORTCUT)

	cmp	DWORD PTR _dwShortcut$[ebp], 0
	je	SHORT $L76343
	cmp	DWORD PTR _dwShortcut$[ebp], -1
	jne	SHORT $L76342
$L76343:

; 502  : 		return "";

	push	OFFSET FLAT:$SG76344
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	eax, DWORD PTR $T78026[ebp]
	or	al, 1
	mov	DWORD PTR $T78026[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L76341
$L76342:

; 503  : 
; 504  : 	CString sText;

	lea	ecx, DWORD PTR _sText$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 505  : 
; 506  : 	WORD wVirtKeyCode = LOWORD(dwShortcut);

	mov	cx, WORD PTR _dwShortcut$[ebp]
	mov	WORD PTR _wVirtKeyCode$[ebp], cx

; 507  : 	WORD wModifiers = HIWORD(dwShortcut);

	mov	edx, DWORD PTR _dwShortcut$[ebp]
	shr	edx, 16					; 00000010H
	and	edx, 65535				; 0000ffffH
	mov	WORD PTR _wModifiers$[ebp], dx

; 508  : 
; 509  : 	if (wModifiers & HOTKEYF_CONTROL)

	mov	eax, DWORD PTR _wModifiers$[ebp]
	and	eax, 65535				; 0000ffffH
	and	eax, 2
	test	eax, eax
	je	SHORT $L76351

; 511  : 		sText += Misc::GetKeyName(VK_CONTROL);

	push	0
	push	17					; 00000011H
	lea	ecx, DWORD PTR $T78022[ebp]
	push	ecx
	call	?GetKeyName@Misc@@YA?AVCString@@GH@Z	; Misc::GetKeyName
	add	esp, 12					; 0000000cH
	mov	DWORD PTR -48+[ebp], eax
	mov	edx, DWORD PTR -48+[ebp]
	mov	DWORD PTR -52+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	eax, DWORD PTR -52+[ebp]
	push	eax
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T78022[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 512  : 		sText += _T("+");

	push	OFFSET FLAT:$SG76353
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=
$L76351:

; 514  : 
; 515  : 	if (wModifiers & HOTKEYF_SHIFT)

	mov	ecx, DWORD PTR _wModifiers$[ebp]
	and	ecx, 65535				; 0000ffffH
	and	ecx, 1
	test	ecx, ecx
	je	SHORT $L76354

; 517  : 		sText += Misc::GetKeyName(VK_SHIFT);

	push	0
	push	16					; 00000010H
	lea	edx, DWORD PTR $T78023[ebp]
	push	edx
	call	?GetKeyName@Misc@@YA?AVCString@@GH@Z	; Misc::GetKeyName
	add	esp, 12					; 0000000cH
	mov	DWORD PTR -56+[ebp], eax
	mov	eax, DWORD PTR -56+[ebp]
	mov	DWORD PTR -60+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR -60+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T78023[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 518  : 		sText += _T("+");

	push	OFFSET FLAT:$SG76356
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=
$L76354:

; 520  : 
; 521  : 	if (wModifiers & HOTKEYF_ALT)

	mov	edx, DWORD PTR _wModifiers$[ebp]
	and	edx, 65535				; 0000ffffH
	and	edx, 4
	test	edx, edx
	je	SHORT $L76357

; 523  : 		sText += Misc::GetKeyName(VK_MENU);

	push	0
	push	18					; 00000012H
	lea	eax, DWORD PTR $T78024[ebp]
	push	eax
	call	?GetKeyName@Misc@@YA?AVCString@@GH@Z	; Misc::GetKeyName
	add	esp, 12					; 0000000cH
	mov	DWORD PTR -64+[ebp], eax
	mov	ecx, DWORD PTR -64+[ebp]
	mov	DWORD PTR -68+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	edx, DWORD PTR -68+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T78024[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 524  : 		sText += _T("+");

	push	OFFSET FLAT:$SG76359
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=
$L76357:

; 526  : 
; 527  : 	CString sKey = Misc::GetKeyName(wVirtKeyCode, (wModifiers & HOTKEYF_EXT));

	mov	eax, DWORD PTR _wModifiers$[ebp]
	and	eax, 65535				; 0000ffffH
	and	eax, 8
	push	eax
	mov	cx, WORD PTR _wVirtKeyCode$[ebp]
	push	ecx
	lea	edx, DWORD PTR _sKey$[ebp]
	push	edx
	call	?GetKeyName@Misc@@YA?AVCString@@GH@Z	; Misc::GetKeyName
	add	esp, 12					; 0000000cH
	mov	BYTE PTR __$EHRec$[ebp+8], 5

; 528  : 
; 529  : 	if (!sKey.IsEmpty())

	lea	ecx, DWORD PTR _sKey$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L76362

; 530  : 		sText += sKey;

	lea	eax, DWORD PTR _sKey$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=

; 531  : 	else

	jmp	SHORT $L76363
$L76362:

; 532  : 		sText.Empty();

	lea	ecx, DWORD PTR _sText$[ebp]
	call	?Empty@CString@@QAEXXZ			; CString::Empty
$L76363:

; 533  : 
; 534  : 	return sText;

	lea	ecx, DWORD PTR _sText$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T78026[ebp]
	or	edx, 1
	mov	DWORD PTR $T78026[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L76341:

; 535  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 68					; 00000044H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L78025:
	mov	eax, DWORD PTR $T78026[ebp]
	and	eax, 1
	test	eax, eax
	je	$L78027
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L78027:
	ret	0
$L78028:
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L78029:
	lea	ecx, DWORD PTR $T78022[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L78030:
	lea	ecx, DWORD PTR $T78023[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L78031:
	lea	ecx, DWORD PTR $T78024[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L78032:
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L78035:
	mov	eax, OFFSET FLAT:$T78034
	jmp	___CxxFrameHandler
text$x	ENDS
?GetShortcutText@CShortcutManager@@SA?AVCString@@K@Z ENDP ; CShortcutManager::GetShortcutText
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
;	COMDAT data
; File D:\_CODE\Shared\ShortcutManager.cpp
data	SEGMENT
$SG76373 DB	'N', 00H, 'u', 00H, 'm', 00H, 'I', 00H, 't', 00H, 'e', 00H
	DB	'm', 00H, 's', 00H, 00H, 00H
	ORG $+2
$SG76374 DB	'K', 00H, 'e', 00H, 'y', 00H, 'b', 00H, 'o', 00H, 'a', 00H
	DB	'r', 00H, 'd', 00H, 'S', 00H, 'h', 00H, 'o', 00H, 'r', 00H, 't'
	DB	00H, 'c', 00H, 'u', 00H, 't', 00H, 's', 00H, 00H, 00H
$SG76379 DB	'K', 00H, 'e', 00H, 'y', 00H, 'b', 00H, 'o', 00H, 'a', 00H
	DB	'r', 00H, 'd', 00H, 'S', 00H, 'h', 00H, 'o', 00H, 'r', 00H, 't'
	DB	00H, 'c', 00H, 'u', 00H, 't', 00H, 's', 00H, '\', 00H, 'I', 00H
	DB	't', 00H, 'e', 00H, 'm', 00H, '%', 00H, '0', 00H, '2', 00H, 'd'
	DB	00H, 00H, 00H
	ORG $+2
$SG76382 DB	'C', 00H, 'm', 00H, 'd', 00H, 'I', 00H, 'D', 00H, 00H, 00H
$SG76385 DB	'S', 00H, 'h', 00H, 'o', 00H, 'r', 00H, 't', 00H, 'c', 00H
	DB	'u', 00H, 't', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T78044	DD	019930520H
	DD	01H
	DD	FLAT:$T78046
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T78046	DD	0ffffffffH
	DD	FLAT:$L78042
xdata$x	ENDS
;	COMDAT ?LoadSettings@CShortcutManager@@QAEXPBVIPreferences@@@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_pPrefs$ = 8
_this$ = -32
_nItem$ = -16
_sKey$76378 = -24
_nCmdID$76380 = -28
_dwShortcut$76383 = -20
?LoadSettings@CShortcutManager@@QAEXPBVIPreferences@@@Z PROC NEAR ; CShortcutManager::LoadSettings, COMDAT

; 538  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L78045
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
	mov	DWORD PTR _this$[ebp], ecx
$L76368:

; 539  : 	ASSERT(pPrefs);

	cmp	DWORD PTR _pPrefs$[ebp], 0
	jne	SHORT $L76371
	push	539					; 0000021bH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76371
	int	3
$L76371:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76368

; 540  : 
; 541  : 	// load shortcuts overriding any defaults
; 542  : 	int nItem = pPrefs->GetProfileInt(_T("KeyboardShortcuts"), _T("NumItems"), 0);

	mov	esi, esp
	push	0
	push	OFFSET FLAT:$SG76373
	push	OFFSET FLAT:$SG76374
	mov	ecx, DWORD PTR _pPrefs$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _pPrefs$[ebp]
	call	DWORD PTR [edx]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nItem$[ebp], eax
$L76376:

; 543  : 
; 544  : 	while (nItem--)

	mov	eax, DWORD PTR _nItem$[ebp]
	mov	ecx, DWORD PTR _nItem$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nItem$[ebp], ecx
	test	eax, eax
	je	$L76377

; 546  : 		CString sKey;

	lea	ecx, DWORD PTR _sKey$76378[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 547  : 		sKey.Format(_T("KeyboardShortcuts\\Item%02d"), nItem);

	mov	edx, DWORD PTR _nItem$[ebp]
	push	edx
	push	OFFSET FLAT:$SG76379
	lea	eax, DWORD PTR _sKey$76378[ebp]
	push	eax
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 12					; 0000000cH

; 548  : 
; 549  : 		UINT nCmdID = (UINT)pPrefs->GetProfileInt(sKey, _T("CmdID"), 0);

	mov	esi, esp
	push	0
	push	OFFSET FLAT:$SG76382
	lea	ecx, DWORD PTR _sKey$76378[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _pPrefs$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _pPrefs$[ebp]
	call	DWORD PTR [edx]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nCmdID$76380[ebp], eax

; 550  : 		DWORD dwShortcut = (DWORD)pPrefs->GetProfileInt(sKey, _T("Shortcut"), 0);

	mov	esi, esp
	push	0
	push	OFFSET FLAT:$SG76385
	lea	ecx, DWORD PTR _sKey$76378[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	eax, DWORD PTR _pPrefs$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _pPrefs$[ebp]
	call	DWORD PTR [edx]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _dwShortcut$76383[ebp], eax

; 551  : 
; 552  : 		if (nCmdID && dwShortcut)

	cmp	DWORD PTR _nCmdID$76380[ebp], 0
	je	SHORT $L76386
	cmp	DWORD PTR _dwShortcut$76383[ebp], 0
	je	SHORT $L76386

; 553  : 			SetShortcut(nCmdID, dwShortcut);

	mov	eax, DWORD PTR _dwShortcut$76383[ebp]
	push	eax
	mov	ecx, DWORD PTR _nCmdID$76380[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetShortcut@CShortcutManager@@QAEXIK@Z	; CShortcutManager::SetShortcut
$L76386:

; 554  : 	}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sKey$76378[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	$L76376
$L76377:

; 555  : }

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
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L78042:
	lea	ecx, DWORD PTR _sKey$76378[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L78045:
	mov	eax, OFFSET FLAT:$T78044
	jmp	___CxxFrameHandler
text$x	ENDS
?LoadSettings@CShortcutManager@@QAEXPBVIPreferences@@@Z ENDP ; CShortcutManager::LoadSettings
PUBLIC	?GetCount@?$CMap@IIKAAK@@QBEHXZ			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetCount
PUBLIC	?GetStartPosition@?$CMap@IIKAAK@@QBEPAU__POSITION@@XZ ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetStartPosition
PUBLIC	?GetNextAssoc@?$CMap@IIKAAK@@QBEXAAPAU__POSITION@@AAIAAK@Z ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetNextAssoc
;	COMDAT data
; File D:\_CODE\Shared\ShortcutManager.cpp
data	SEGMENT
$SG76395 DB	'N', 00H, 'u', 00H, 'm', 00H, 'I', 00H, 't', 00H, 'e', 00H
	DB	'm', 00H, 's', 00H, 00H, 00H
	ORG $+2
$SG76396 DB	'K', 00H, 'e', 00H, 'y', 00H, 'b', 00H, 'o', 00H, 'a', 00H
	DB	'r', 00H, 'd', 00H, 'S', 00H, 'h', 00H, 'o', 00H, 'r', 00H, 't'
	DB	00H, 'c', 00H, 'u', 00H, 't', 00H, 's', 00H, 00H, 00H
$SG76406 DB	'K', 00H, 'e', 00H, 'y', 00H, 'b', 00H, 'o', 00H, 'a', 00H
	DB	'r', 00H, 'd', 00H, 'S', 00H, 'h', 00H, 'o', 00H, 'r', 00H, 't'
	DB	00H, 'c', 00H, 'u', 00H, 't', 00H, 's', 00H, '\', 00H, 'I', 00H
	DB	't', 00H, 'e', 00H, 'm', 00H, '%', 00H, '0', 00H, '2', 00H, 'd'
	DB	00H, 00H, 00H
	ORG $+2
$SG76407 DB	'C', 00H, 'm', 00H, 'd', 00H, 'I', 00H, 'D', 00H, 00H, 00H
$SG76408 DB	'S', 00H, 'h', 00H, 'o', 00H, 'r', 00H, 't', 00H, 'c', 00H
	DB	'u', 00H, 't', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T78054	DD	019930520H
	DD	01H
	DD	FLAT:$T78056
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T78056	DD	0ffffffffH
	DD	FLAT:$L78052
xdata$x	ENDS
;	COMDAT ?SaveSettings@CShortcutManager@@QBEXPAVIPreferences@@@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_pPrefs$ = 8
_this$ = -36
_pos$ = -20
_nItem$ = -16
_nCmdID$76402 = -28
_dwShortcut$76403 = -24
_sKey$76405 = -32
?SaveSettings@CShortcutManager@@QBEXPAVIPreferences@@@Z PROC NEAR ; CShortcutManager::SaveSettings, COMDAT

; 558  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L78055
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx
$L76391:

; 559  : 	ASSERT(pPrefs);

	cmp	DWORD PTR _pPrefs$[ebp], 0
	jne	SHORT $L76394
	push	559					; 0000022fH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76394
	int	3
$L76394:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76391

; 560  : 
; 561  : 	pPrefs->WriteProfileInt(_T("KeyboardShortcuts"), _T("NumItems"), m_mapID2Shortcut.GetCount());

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	?GetCount@?$CMap@IIKAAK@@QBEHXZ		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetCount
	mov	esi, esp
	push	eax
	push	OFFSET FLAT:$SG76395
	push	OFFSET FLAT:$SG76396
	mov	ecx, DWORD PTR _pPrefs$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _pPrefs$[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp

; 562  : 
; 563  : 	POSITION pos = m_mapID2Shortcut.GetStartPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	?GetStartPosition@?$CMap@IIKAAK@@QBEPAU__POSITION@@XZ ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetStartPosition
	mov	DWORD PTR _pos$[ebp], eax

; 564  : 	int nItem = 0;

	mov	DWORD PTR _nItem$[ebp], 0
$L76400:

; 565  : 
; 566  : 	while (pos)

	cmp	DWORD PTR _pos$[ebp], 0
	je	$L76401

; 568  : 		UINT nCmdID = 0;

	mov	DWORD PTR _nCmdID$76402[ebp], 0

; 569  : 		DWORD dwShortcut = 0;

	mov	DWORD PTR _dwShortcut$76403[ebp], 0

; 570  : 
; 571  : 		m_mapID2Shortcut.GetNextAssoc(pos, nCmdID, dwShortcut);

	lea	eax, DWORD PTR _dwShortcut$76403[ebp]
	push	eax
	lea	ecx, DWORD PTR _nCmdID$76402[ebp]
	push	ecx
	lea	edx, DWORD PTR _pos$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	?GetNextAssoc@?$CMap@IIKAAK@@QBEXAAPAU__POSITION@@AAIAAK@Z ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetNextAssoc

; 572  : 
; 573  : 		if (nCmdID && dwShortcut)

	cmp	DWORD PTR _nCmdID$76402[ebp], 0
	je	$L76404
	cmp	DWORD PTR _dwShortcut$76403[ebp], 0
	je	$L76404

; 575  : 			CString sKey;

	lea	ecx, DWORD PTR _sKey$76405[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 576  : 			sKey.Format(_T("KeyboardShortcuts\\Item%02d"), nItem);

	mov	eax, DWORD PTR _nItem$[ebp]
	push	eax
	push	OFFSET FLAT:$SG76406
	lea	ecx, DWORD PTR _sKey$76405[ebp]
	push	ecx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 12					; 0000000cH

; 577  : 
; 578  : 			pPrefs->WriteProfileInt(sKey, _T("CmdID"), nCmdID);

	mov	esi, esp
	mov	edx, DWORD PTR _nCmdID$76402[ebp]
	push	edx
	push	OFFSET FLAT:$SG76407
	lea	ecx, DWORD PTR _sKey$76405[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	eax, DWORD PTR _pPrefs$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _pPrefs$[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp

; 579  : 			pPrefs->WriteProfileInt(sKey, _T("Shortcut"), dwShortcut);

	mov	esi, esp
	mov	eax, DWORD PTR _dwShortcut$76403[ebp]
	push	eax
	push	OFFSET FLAT:$SG76408
	lea	ecx, DWORD PTR _sKey$76405[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _pPrefs$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _pPrefs$[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp

; 580  : 
; 581  : 			nItem++;

	mov	eax, DWORD PTR _nItem$[ebp]
	add	eax, 1
	mov	DWORD PTR _nItem$[ebp], eax

; 582  : 		}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sKey$76405[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76404:

; 583  : 	}

	jmp	$L76400
$L76401:

; 584  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L78052:
	lea	ecx, DWORD PTR _sKey$76405[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L78055:
	mov	eax, OFFSET FLAT:$T78054
	jmp	___CxxFrameHandler
text$x	ENDS
?SaveSettings@CShortcutManager@@QBEXPAVIPreferences@@@Z ENDP ; CShortcutManager::SaveSettings
PUBLIC	?Serialize@?$CMap@KKIAAI@@UAEXAAVCArchive@@@Z	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::Serialize
PUBLIC	?Dump@?$CMap@KKIAAI@@UBEXAAVCDumpContext@@@Z	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::Dump
PUBLIC	?AssertValid@?$CMap@KKIAAI@@UBEXXZ		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::AssertValid
PUBLIC	??_7?$CMap@KKIAAI@@6B@				; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::`vftable'
PUBLIC	??_G?$CMap@KKIAAI@@UAEPAXI@Z			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::`scalar deleting destructor'
PUBLIC	??_E?$CMap@KKIAAI@@UAEPAXI@Z			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::`vector deleting destructor'
EXTRN	?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ:NEAR ; CObject::GetRuntimeClass
EXTRN	??1CObject@@UAE@XZ:NEAR				; CObject::~CObject
EXTRN	??0CObject@@IAE@XZ:NEAR				; CObject::CObject
;	COMDAT ??_7?$CMap@KKIAAI@@6B@
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
CONST	SEGMENT
??_7?$CMap@KKIAAI@@6B@ DD FLAT:?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::`vftable'
	DD	FLAT:??_E?$CMap@KKIAAI@@UAEPAXI@Z
	DD	FLAT:?Serialize@?$CMap@KKIAAI@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@?$CMap@KKIAAI@@UBEXXZ
	DD	FLAT:?Dump@?$CMap@KKIAAI@@UBEXAAVCDumpContext@@@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T78064	DD	019930520H
	DD	01H
	DD	FLAT:$T78066
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T78066	DD	0ffffffffH
	DD	FLAT:$L78062
xdata$x	ENDS
;	COMDAT ??0?$CMap@KKIAAI@@QAE@H@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_nBlockSize$ = 8
_this$ = -16
??0?$CMap@KKIAAI@@QAE@H@Z PROC NEAR			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::CMap<unsigned long,unsigned long,unsigned int,unsigned int &>, COMDAT

; 1201 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L78065
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CObject@@IAE@XZ			; CObject::CObject
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CMap@KKIAAI@@6B@ ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::`vftable'
$L76497:

; 1202 : 	ASSERT(nBlockSize > 0);

	cmp	DWORD PTR _nBlockSize$[ebp], 0
	jg	SHORT $L76500
	push	1202					; 000004b2H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76500
	int	3
$L76500:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76497

; 1203 : 
; 1204 : 	m_pHashTable = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0

; 1205 : 	m_nHashTableSize = 17;  // default size

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+8], 17			; 00000011H

; 1206 : 	m_nCount = 0;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+12], 0

; 1207 : 	m_pFreeList = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+16], 0

; 1208 : 	m_pBlocks = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+20], 0

; 1209 : 	m_nBlockSize = nBlockSize;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nBlockSize$[ebp]
	mov	DWORD PTR [ecx+24], edx

; 1210 : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L78062:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L78065:
	mov	eax, OFFSET FLAT:$T78064
	jmp	___CxxFrameHandler
text$x	ENDS
??0?$CMap@KKIAAI@@QAE@H@Z ENDP				; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::CMap<unsigned long,unsigned long,unsigned int,unsigned int &>
PUBLIC	?GetAssocAt@?$CMap@KKIAAI@@IBEPAUCAssoc@1@KAAI@Z ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::GetAssocAt
EXTRN	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z:NEAR ; AfxAssertValidObject
;	COMDAT ?Lookup@?$CMap@KKIAAI@@QBEHKAAI@Z
_TEXT	SEGMENT
_key$ = 8
_rValue$ = 12
_this$ = -12
_nHash$ = -8
_pAssoc$ = -4
?Lookup@?$CMap@KKIAAI@@QBEHKAAI@Z PROC NEAR		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::Lookup, COMDAT

; 1341 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1342 : 	ASSERT_VALID(this);

	push	1342					; 0000053eH
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1343 : 
; 1344 : 	UINT nHash;
; 1345 : 	CAssoc* pAssoc = GetAssocAt(key, nHash);

	lea	ecx, DWORD PTR _nHash$[ebp]
	push	ecx
	mov	edx, DWORD PTR _key$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetAssocAt@?$CMap@KKIAAI@@IBEPAUCAssoc@1@KAAI@Z ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::GetAssocAt
	mov	DWORD PTR _pAssoc$[ebp], eax

; 1346 : 	if (pAssoc == NULL)

	cmp	DWORD PTR _pAssoc$[ebp], 0
	jne	SHORT $L76508

; 1347 : 		return FALSE;  // not in map

	xor	eax, eax
	jmp	SHORT $L76505
$L76508:

; 1348 : 
; 1349 : 	rValue = pAssoc->value;

	mov	eax, DWORD PTR _rValue$[ebp]
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	mov	DWORD PTR [eax], edx

; 1350 : 	return TRUE;

	mov	eax, 1
$L76505:

; 1351 : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Lookup@?$CMap@KKIAAI@@QBEHKAAI@Z ENDP			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::Lookup
_TEXT	ENDS
PUBLIC	?InitHashTable@?$CMap@KKIAAI@@QAEXIH@Z		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::InitHashTable
PUBLIC	?NewAssoc@?$CMap@KKIAAI@@IAEPAUCAssoc@1@XZ	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::NewAssoc
;	COMDAT ??A?$CMap@KKIAAI@@QAEAAIK@Z
_TEXT	SEGMENT
_key$ = 8
_this$ = -12
_nHash$ = -8
_pAssoc$ = -4
??A?$CMap@KKIAAI@@QAEAAIK@Z PROC NEAR			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::operator[], COMDAT

; 1355 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1356 : 	ASSERT_VALID(this);

	push	1356					; 0000054cH
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1357 : 
; 1358 : 	UINT nHash;
; 1359 : 	CAssoc* pAssoc;
; 1360 : 	if ((pAssoc = GetAssocAt(key, nHash)) == NULL)

	lea	ecx, DWORD PTR _nHash$[ebp]
	push	ecx
	mov	edx, DWORD PTR _key$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetAssocAt@?$CMap@KKIAAI@@IBEPAUCAssoc@1@KAAI@Z ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::GetAssocAt
	mov	DWORD PTR _pAssoc$[ebp], eax
	cmp	DWORD PTR _pAssoc$[ebp], 0
	jne	SHORT $L76515

; 1362 : 		if (m_pHashTable == NULL)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L76516

; 1363 : 			InitHashTable(m_nHashTableSize);

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?InitHashTable@?$CMap@KKIAAI@@QAEXIH@Z	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::InitHashTable
$L76516:

; 1364 : 
; 1365 : 		// it doesn't exist, add a new Association
; 1366 : 		pAssoc = NewAssoc();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?NewAssoc@?$CMap@KKIAAI@@IAEPAUCAssoc@1@XZ ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::NewAssoc
	mov	DWORD PTR _pAssoc$[ebp], eax

; 1367 : 		pAssoc->nHashValue = nHash;

	mov	eax, DWORD PTR _pAssoc$[ebp]
	mov	ecx, DWORD PTR _nHash$[ebp]
	mov	DWORD PTR [eax+4], ecx

; 1368 : 		pAssoc->key = key;

	mov	edx, DWORD PTR _pAssoc$[ebp]
	mov	eax, DWORD PTR _key$[ebp]
	mov	DWORD PTR [edx+8], eax

; 1369 : 		// 'pAssoc->value' is a constructed object, nothing more
; 1370 : 
; 1371 : 		// put into hash table
; 1372 : 		pAssoc->pNext = m_pHashTable[nHash];

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR _pAssoc$[ebp]
	mov	ecx, DWORD PTR _nHash$[ebp]
	mov	edx, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR [eax], edx

; 1373 : 		m_pHashTable[nHash] = pAssoc;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _nHash$[ebp]
	mov	eax, DWORD PTR _pAssoc$[ebp]
	mov	DWORD PTR [ecx+edx*4], eax
$L76515:

; 1375 : 	return pAssoc->value;  // return new reference

	mov	eax, DWORD PTR _pAssoc$[ebp]
	add	eax, 12					; 0000000cH

; 1376 : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??A?$CMap@KKIAAI@@QAEAAIK@Z ENDP			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::operator[]
_TEXT	ENDS
PUBLIC	?FreeAssoc@?$CMap@KKIAAI@@IAEXPAUCAssoc@1@@Z	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::FreeAssoc
PUBLIC	?HashKey@@YGIK@Z				; HashKey
PUBLIC	?CompareElements@@YGHPBK0@Z			; CompareElements
;	COMDAT ?RemoveKey@?$CMap@KKIAAI@@QAEHK@Z
_TEXT	SEGMENT
_key$ = 8
_this$ = -12
_ppAssocPrev$ = -4
_pAssoc$ = -8
?RemoveKey@?$CMap@KKIAAI@@QAEHK@Z PROC NEAR		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::RemoveKey, COMDAT

; 1381 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1382 : 	ASSERT_VALID(this);

	push	1382					; 00000566H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1383 : 
; 1384 : 	if (m_pHashTable == NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	jne	SHORT $L76521

; 1385 : 		return FALSE;  // nothing in the table

	xor	eax, eax
	jmp	SHORT $L76520
$L76521:

; 1388 : 	ppAssocPrev = &m_pHashTable[HashKey<ARG_KEY>(key) % m_nHashTableSize];

	mov	edx, DWORD PTR _key$[ebp]
	push	edx
	call	?HashKey@@YGIK@Z			; HashKey
	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	div	DWORD PTR [ecx+8]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	lea	edx, DWORD PTR [ecx+edx*4]
	mov	DWORD PTR _ppAssocPrev$[ebp], edx

; 1389 : 
; 1390 : 	CAssoc* pAssoc;
; 1391 : 	for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)

	mov	eax, DWORD PTR _ppAssocPrev$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssoc$[ebp], ecx
	jmp	SHORT $L76528
$L76529:
	mov	edx, DWORD PTR _pAssoc$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _pAssoc$[ebp], eax
$L76528:
	cmp	DWORD PTR _pAssoc$[ebp], 0
	je	SHORT $L76530

; 1393 : 		if (CompareElements(&pAssoc->key, &key))

	lea	ecx, DWORD PTR _key$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pAssoc$[ebp]
	add	edx, 8
	push	edx
	call	?CompareElements@@YGHPBK0@Z		; CompareElements
	test	eax, eax
	je	SHORT $L76537

; 1395 : 			// remove it
; 1396 : 			*ppAssocPrev = pAssoc->pNext;  // remove from list

	mov	eax, DWORD PTR _ppAssocPrev$[ebp]
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR [eax], edx

; 1397 : 			FreeAssoc(pAssoc);

	mov	eax, DWORD PTR _pAssoc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FreeAssoc@?$CMap@KKIAAI@@IAEXPAUCAssoc@1@@Z ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::FreeAssoc

; 1398 : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L76520
$L76537:

; 1400 : 		ppAssocPrev = &pAssoc->pNext;

	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	DWORD PTR _ppAssocPrev$[ebp], ecx

; 1401 : 	}

	jmp	SHORT $L76529
$L76530:

; 1402 : 	return FALSE;  // not found

	xor	eax, eax
$L76520:

; 1403 : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?RemoveKey@?$CMap@KKIAAI@@QAEHK@Z ENDP			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::RemoveKey
_TEXT	ENDS
PUBLIC	?RemoveAll@?$CMap@KKIAAI@@QAEXXZ		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::RemoveAll
;	COMDAT xdata$x
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
xdata$x	SEGMENT
$T78079	DD	019930520H
	DD	01H
	DD	FLAT:$T78081
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T78081	DD	0ffffffffH
	DD	FLAT:$L78077
xdata$x	ENDS
;	COMDAT ??1?$CMap@KKIAAI@@UAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??1?$CMap@KKIAAI@@UAE@XZ PROC NEAR			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::~CMap<unsigned long,unsigned long,unsigned int,unsigned int &>, COMDAT

; 1270 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L78080
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CMap@KKIAAI@@6B@ ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1271 : 	RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CMap@KKIAAI@@QAEXXZ	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::RemoveAll
$L76541:

; 1272 : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L76544
	push	1272					; 000004f8H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76544
	int	3
$L76544:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76541

; 1273 : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L78077:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L78080:
	mov	eax, OFFSET FLAT:$T78079
	jmp	___CxxFrameHandler
text$x	ENDS
??1?$CMap@KKIAAI@@UAE@XZ ENDP				; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::~CMap<unsigned long,unsigned long,unsigned int,unsigned int &>
PUBLIC	?SetAt@?$CMap@KKIAAI@@QAEXKAAI@Z		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::SetAt
PUBLIC	?SerializeElements@@YGXAAVCArchive@@PAKH@Z	; SerializeElements
PUBLIC	?SerializeElements@@YGXAAVCArchive@@PAIH@Z	; SerializeElements
EXTRN	?ReadCount@CArchive@@QAEKXZ:NEAR		; CArchive::ReadCount
EXTRN	?WriteCount@CArchive@@QAEXK@Z:NEAR		; CArchive::WriteCount
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	?IsStoring@CArchive@@QBEHXZ:NEAR		; CArchive::IsStoring
;	COMDAT ?Serialize@?$CMap@KKIAAI@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT
_ar$ = 8
_this$ = -24
_nHash$76555 = -4
_pAssoc$76559 = -8
_nNewCount$76576 = -12
_newKey$76580 = -16
_newValue$76581 = -20
?Serialize@?$CMap@KKIAAI@@UAEXAAVCArchive@@@Z PROC NEAR	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::Serialize, COMDAT

; 1445 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 24					; 00000018H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 1446 : 	ASSERT_VALID(this);

	push	1446					; 000005a6H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1447 : 
; 1448 : 	CObject::Serialize(ar);

	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Serialize@CObject@@UAEXAAVCArchive@@@Z	; CObject::Serialize

; 1449 : 
; 1450 : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	$L76549

; 1452 : 		ar.WriteCount(m_nCount);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?WriteCount@CArchive@@QAEXK@Z		; CArchive::WriteCount

; 1453 : 		if (m_nCount == 0)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jne	SHORT $L76550

; 1454 : 			return;  // nothing more to do

	jmp	$L76548
$L76550:

; 1455 : 
; 1456 : 		ASSERT(m_pHashTable != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	jne	SHORT $L76554
	push	1456					; 000005b0H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76554
	int	3
$L76554:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76550

; 1457 : 		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)

	mov	DWORD PTR _nHash$76555[ebp], 0
	jmp	SHORT $L76556
$L76557:
	mov	ecx, DWORD PTR _nHash$76555[ebp]
	add	ecx, 1
	mov	DWORD PTR _nHash$76555[ebp], ecx
$L76556:
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nHash$76555[ebp]
	cmp	eax, DWORD PTR [edx+8]
	jae	SHORT $L76558

; 1459 : 			CAssoc* pAssoc;
; 1460 : 			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR _nHash$76555[ebp]
	mov	ecx, DWORD PTR [edx+eax*4]
	mov	DWORD PTR _pAssoc$76559[ebp], ecx

; 1461 : 			  pAssoc = pAssoc->pNext)

	jmp	SHORT $L76560
$L76561:
	mov	edx, DWORD PTR _pAssoc$76559[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _pAssoc$76559[ebp], eax
$L76560:
	cmp	DWORD PTR _pAssoc$76559[ebp], 0
	je	SHORT $L76562

; 1463 : 				SerializeElements<KEY>(ar, &pAssoc->key, 1);

	push	1
	mov	ecx, DWORD PTR _pAssoc$76559[ebp]
	add	ecx, 8
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAKH@Z ; SerializeElements

; 1464 : 				SerializeElements<VALUE>(ar, &pAssoc->value, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$76559[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	call	?SerializeElements@@YGXAAVCArchive@@PAIH@Z ; SerializeElements

; 1465 : 			}

	jmp	SHORT $L76561
$L76562:

; 1466 : 		}

	jmp	SHORT $L76557
$L76558:

; 1468 : 	else

	jmp	SHORT $L76579
$L76549:

; 1470 : 		DWORD nNewCount = ar.ReadCount();

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?ReadCount@CArchive@@QAEKXZ		; CArchive::ReadCount
	mov	DWORD PTR _nNewCount$76576[ebp], eax
$L76578:

; 1471 : 		while (nNewCount--)

	mov	edx, DWORD PTR _nNewCount$76576[ebp]
	mov	eax, DWORD PTR _nNewCount$76576[ebp]
	sub	eax, 1
	mov	DWORD PTR _nNewCount$76576[ebp], eax
	test	edx, edx
	je	SHORT $L76579

; 1473 : 			KEY newKey;
; 1474 : 			VALUE newValue;
; 1475 : 			SerializeElements<KEY>(ar, &newKey, 1);

	push	1
	lea	ecx, DWORD PTR _newKey$76580[ebp]
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAKH@Z ; SerializeElements

; 1476 : 			SerializeElements<VALUE>(ar, &newValue, 1);

	push	1
	lea	eax, DWORD PTR _newValue$76581[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	call	?SerializeElements@@YGXAAVCArchive@@PAIH@Z ; SerializeElements

; 1477 : 			SetAt(newKey, newValue);

	lea	edx, DWORD PTR _newValue$76581[ebp]
	push	edx
	mov	eax, DWORD PTR _newKey$76580[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetAt@?$CMap@KKIAAI@@QAEXKAAI@Z	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::SetAt

; 1478 : 		}

	jmp	SHORT $L76578
$L76579:
$L76548:

; 1480 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Serialize@?$CMap@KKIAAI@@UAEXAAVCArchive@@@Z ENDP	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::Serialize
_TEXT	ENDS
PUBLIC	?GetStartPosition@?$CMap@KKIAAI@@QBEPAU__POSITION@@XZ ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::GetStartPosition
PUBLIC	?GetNextAssoc@?$CMap@KKIAAI@@QBEXAAPAU__POSITION@@AAKAAI@Z ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::GetNextAssoc
PUBLIC	??_C@_05KIFI@with?5?$AA@			; `string'
PUBLIC	??_C@_09LOPF@?5elements?$AA@			; `string'
PUBLIC	??_C@_03DANH@?6?7?$FL?$AA@			; `string'
PUBLIC	?DumpElements@@YGXAAVCDumpContext@@PBKH@Z	; DumpElements
PUBLIC	??_C@_04MNLE@?$FN?5?$DN?5?$AA@			; `string'
PUBLIC	?DumpElements@@YGXAAVCDumpContext@@PBIH@Z	; DumpElements
PUBLIC	??_C@_01BJG@?6?$AA@				; `string'
EXTRN	?Dump@CObject@@UBEXAAVCDumpContext@@@Z:NEAR	; CObject::Dump
EXTRN	?GetDepth@CDumpContext@@QBEHXZ:NEAR		; CDumpContext::GetDepth
EXTRN	??6CDumpContext@@QAEAAV0@PBD@Z:NEAR		; CDumpContext::operator<<
EXTRN	??6CDumpContext@@QAEAAV0@H@Z:NEAR		; CDumpContext::operator<<
;	COMDAT ??_C@_05KIFI@with?5?$AA@
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
_DATA	SEGMENT
??_C@_05KIFI@with?5?$AA@ DB 'with ', 00H		; `string'
_DATA	ENDS
;	COMDAT ??_C@_09LOPF@?5elements?$AA@
_DATA	SEGMENT
??_C@_09LOPF@?5elements?$AA@ DB ' elements', 00H	; `string'
_DATA	ENDS
;	COMDAT ??_C@_03DANH@?6?7?$FL?$AA@
_DATA	SEGMENT
??_C@_03DANH@?6?7?$FL?$AA@ DB 0aH, 09H, '[', 00H	; `string'
_DATA	ENDS
;	COMDAT ??_C@_04MNLE@?$FN?5?$DN?5?$AA@
_DATA	SEGMENT
??_C@_04MNLE@?$FN?5?$DN?5?$AA@ DB '] = ', 00H		; `string'
_DATA	ENDS
;	COMDAT ??_C@_01BJG@?6?$AA@
_DATA	SEGMENT
??_C@_01BJG@?6?$AA@ DB 0aH, 00H				; `string'
_DATA	ENDS
;	COMDAT ?Dump@?$CMap@KKIAAI@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT
_dc$ = 8
_this$ = -16
_key$76599 = -8
_val$76600 = -4
_pos$76601 = -12
?Dump@?$CMap@KKIAAI@@UBEXAAVCDumpContext@@@Z PROC NEAR	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::Dump, COMDAT

; 1485 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 1486 : 	CObject::Dump(dc);

	mov	eax, DWORD PTR _dc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Dump@CObject@@UBEXAAVCDumpContext@@@Z	; CObject::Dump

; 1487 : 
; 1488 : 	dc << "with " << m_nCount << " elements";

	push	OFFSET FLAT:??_C@_09LOPF@?5elements?$AA@ ; `string'
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	push	edx
	push	OFFSET FLAT:??_C@_05KIFI@with?5?$AA@	; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<
	mov	ecx, eax
	call	??6CDumpContext@@QAEAAV0@H@Z		; CDumpContext::operator<<
	mov	ecx, eax
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1489 : 	if (dc.GetDepth() > 0)

	mov	ecx, DWORD PTR _dc$[ebp]
	call	?GetDepth@CDumpContext@@QBEHXZ		; CDumpContext::GetDepth
	test	eax, eax
	jle	SHORT $L76604

; 1491 : 		// Dump in format "[key] -> value"
; 1492 : 		KEY key;
; 1493 : 		VALUE val;
; 1494 : 
; 1495 : 		POSITION pos = GetStartPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetStartPosition@?$CMap@KKIAAI@@QBEPAU__POSITION@@XZ ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::GetStartPosition
	mov	DWORD PTR _pos$76601[ebp], eax
$L76603:

; 1496 : 		while (pos != NULL)

	cmp	DWORD PTR _pos$76601[ebp], 0
	je	SHORT $L76604

; 1498 : 			GetNextAssoc(pos, key, val);

	lea	eax, DWORD PTR _val$76600[ebp]
	push	eax
	lea	ecx, DWORD PTR _key$76599[ebp]
	push	ecx
	lea	edx, DWORD PTR _pos$76601[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetNextAssoc@?$CMap@KKIAAI@@QBEXAAPAU__POSITION@@AAKAAI@Z ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::GetNextAssoc

; 1499 : 			dc << "\n\t[";

	push	OFFSET FLAT:??_C@_03DANH@?6?7?$FL?$AA@	; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1500 : 			DumpElements<KEY>(dc, &key, 1);

	push	1
	lea	eax, DWORD PTR _key$76599[ebp]
	push	eax
	mov	ecx, DWORD PTR _dc$[ebp]
	push	ecx
	call	?DumpElements@@YGXAAVCDumpContext@@PBKH@Z ; DumpElements

; 1501 : 			dc << "] = ";

	push	OFFSET FLAT:??_C@_04MNLE@?$FN?5?$DN?5?$AA@ ; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1502 : 			DumpElements<VALUE>(dc, &val, 1);

	push	1
	lea	edx, DWORD PTR _val$76600[ebp]
	push	edx
	mov	eax, DWORD PTR _dc$[ebp]
	push	eax
	call	?DumpElements@@YGXAAVCDumpContext@@PBIH@Z ; DumpElements

; 1503 : 		}

	jmp	SHORT $L76603
$L76604:

; 1505 : 
; 1506 : 	dc << "\n";

	push	OFFSET FLAT:??_C@_01BJG@?6?$AA@		; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1507 : }

	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Dump@?$CMap@KKIAAI@@UBEXAAVCDumpContext@@@Z ENDP	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::Dump
_TEXT	ENDS
EXTRN	?AssertValid@CObject@@UBEXXZ:NEAR		; CObject::AssertValid
;	COMDAT ?AssertValid@?$CMap@KKIAAI@@UBEXXZ
_TEXT	SEGMENT
_this$ = -4
?AssertValid@?$CMap@KKIAAI@@UBEXXZ PROC NEAR		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::AssertValid, COMDAT

; 1511 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1512 : 	CObject::AssertValid();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?AssertValid@CObject@@UBEXXZ		; CObject::AssertValid
$L76623:

; 1513 : 
; 1514 : 	ASSERT(m_nHashTableSize > 0);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	ja	SHORT $L76626
	push	1514					; 000005eaH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76626
	int	3
$L76626:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76623
$L76625:

; 1515 : 	ASSERT(m_nCount == 0 || m_pHashTable != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+12], 0
	je	SHORT $L76630
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L76630
	push	1515					; 000005ebH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76630
	int	3
$L76630:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76625

; 1516 : 		// non-empty map should have hash table
; 1517 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?AssertValid@?$CMap@KKIAAI@@UBEXXZ ENDP			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::AssertValid
_TEXT	ENDS
PUBLIC	?Serialize@?$CMap@IIKAAK@@UAEXAAVCArchive@@@Z	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::Serialize
PUBLIC	?Dump@?$CMap@IIKAAK@@UBEXAAVCDumpContext@@@Z	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::Dump
PUBLIC	?AssertValid@?$CMap@IIKAAK@@UBEXXZ		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::AssertValid
PUBLIC	??_7?$CMap@IIKAAK@@6B@				; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::`vftable'
PUBLIC	??_G?$CMap@IIKAAK@@UAEPAXI@Z			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::`scalar deleting destructor'
PUBLIC	??_E?$CMap@IIKAAK@@UAEPAXI@Z			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::`vector deleting destructor'
;	COMDAT ??_7?$CMap@IIKAAK@@6B@
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
CONST	SEGMENT
??_7?$CMap@IIKAAK@@6B@ DD FLAT:?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::`vftable'
	DD	FLAT:??_E?$CMap@IIKAAK@@UAEPAXI@Z
	DD	FLAT:?Serialize@?$CMap@IIKAAK@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@?$CMap@IIKAAK@@UBEXXZ
	DD	FLAT:?Dump@?$CMap@IIKAAK@@UBEXAAVCDumpContext@@@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T78094	DD	019930520H
	DD	01H
	DD	FLAT:$T78096
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T78096	DD	0ffffffffH
	DD	FLAT:$L78092
xdata$x	ENDS
;	COMDAT ??0?$CMap@IIKAAK@@QAE@H@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_nBlockSize$ = 8
_this$ = -16
??0?$CMap@IIKAAK@@QAE@H@Z PROC NEAR			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::CMap<unsigned int,unsigned int,unsigned long,unsigned long &>, COMDAT

; 1201 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L78095
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CObject@@IAE@XZ			; CObject::CObject
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CMap@IIKAAK@@6B@ ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::`vftable'
$L76635:

; 1202 : 	ASSERT(nBlockSize > 0);

	cmp	DWORD PTR _nBlockSize$[ebp], 0
	jg	SHORT $L76638
	push	1202					; 000004b2H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76638
	int	3
$L76638:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76635

; 1203 : 
; 1204 : 	m_pHashTable = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0

; 1205 : 	m_nHashTableSize = 17;  // default size

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+8], 17			; 00000011H

; 1206 : 	m_nCount = 0;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+12], 0

; 1207 : 	m_pFreeList = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+16], 0

; 1208 : 	m_pBlocks = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+20], 0

; 1209 : 	m_nBlockSize = nBlockSize;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nBlockSize$[ebp]
	mov	DWORD PTR [ecx+24], edx

; 1210 : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L78092:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L78095:
	mov	eax, OFFSET FLAT:$T78094
	jmp	___CxxFrameHandler
text$x	ENDS
??0?$CMap@IIKAAK@@QAE@H@Z ENDP				; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::CMap<unsigned int,unsigned int,unsigned long,unsigned long &>
;	COMDAT ?GetCount@?$CMap@IIKAAK@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?GetCount@?$CMap@IIKAAK@@QBEHXZ PROC NEAR		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetCount, COMDAT

; 1182 : 	{ return m_nCount; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+12]
	mov	esp, ebp
	pop	ebp
	ret	0
?GetCount@?$CMap@IIKAAK@@QBEHXZ ENDP			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetCount
_TEXT	ENDS
PUBLIC	?GetAssocAt@?$CMap@IIKAAK@@IBEPAUCAssoc@1@IAAI@Z ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetAssocAt
;	COMDAT ?Lookup@?$CMap@IIKAAK@@QBEHIAAK@Z
_TEXT	SEGMENT
_key$ = 8
_rValue$ = 12
_this$ = -12
_nHash$ = -8
_pAssoc$ = -4
?Lookup@?$CMap@IIKAAK@@QBEHIAAK@Z PROC NEAR		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::Lookup, COMDAT

; 1341 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1342 : 	ASSERT_VALID(this);

	push	1342					; 0000053eH
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1343 : 
; 1344 : 	UINT nHash;
; 1345 : 	CAssoc* pAssoc = GetAssocAt(key, nHash);

	lea	ecx, DWORD PTR _nHash$[ebp]
	push	ecx
	mov	edx, DWORD PTR _key$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetAssocAt@?$CMap@IIKAAK@@IBEPAUCAssoc@1@IAAI@Z ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetAssocAt
	mov	DWORD PTR _pAssoc$[ebp], eax

; 1346 : 	if (pAssoc == NULL)

	cmp	DWORD PTR _pAssoc$[ebp], 0
	jne	SHORT $L76649

; 1347 : 		return FALSE;  // not in map

	xor	eax, eax
	jmp	SHORT $L76646
$L76649:

; 1348 : 
; 1349 : 	rValue = pAssoc->value;

	mov	eax, DWORD PTR _rValue$[ebp]
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	mov	DWORD PTR [eax], edx

; 1350 : 	return TRUE;

	mov	eax, 1
$L76646:

; 1351 : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Lookup@?$CMap@IIKAAK@@QBEHIAAK@Z ENDP			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::Lookup
_TEXT	ENDS
PUBLIC	?InitHashTable@?$CMap@IIKAAK@@QAEXIH@Z		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::InitHashTable
PUBLIC	?NewAssoc@?$CMap@IIKAAK@@IAEPAUCAssoc@1@XZ	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::NewAssoc
;	COMDAT ??A?$CMap@IIKAAK@@QAEAAKI@Z
_TEXT	SEGMENT
_key$ = 8
_this$ = -12
_nHash$ = -8
_pAssoc$ = -4
??A?$CMap@IIKAAK@@QAEAAKI@Z PROC NEAR			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::operator[], COMDAT

; 1355 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1356 : 	ASSERT_VALID(this);

	push	1356					; 0000054cH
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1357 : 
; 1358 : 	UINT nHash;
; 1359 : 	CAssoc* pAssoc;
; 1360 : 	if ((pAssoc = GetAssocAt(key, nHash)) == NULL)

	lea	ecx, DWORD PTR _nHash$[ebp]
	push	ecx
	mov	edx, DWORD PTR _key$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetAssocAt@?$CMap@IIKAAK@@IBEPAUCAssoc@1@IAAI@Z ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetAssocAt
	mov	DWORD PTR _pAssoc$[ebp], eax
	cmp	DWORD PTR _pAssoc$[ebp], 0
	jne	SHORT $L76656

; 1362 : 		if (m_pHashTable == NULL)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L76657

; 1363 : 			InitHashTable(m_nHashTableSize);

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?InitHashTable@?$CMap@IIKAAK@@QAEXIH@Z	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::InitHashTable
$L76657:

; 1364 : 
; 1365 : 		// it doesn't exist, add a new Association
; 1366 : 		pAssoc = NewAssoc();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?NewAssoc@?$CMap@IIKAAK@@IAEPAUCAssoc@1@XZ ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::NewAssoc
	mov	DWORD PTR _pAssoc$[ebp], eax

; 1367 : 		pAssoc->nHashValue = nHash;

	mov	eax, DWORD PTR _pAssoc$[ebp]
	mov	ecx, DWORD PTR _nHash$[ebp]
	mov	DWORD PTR [eax+4], ecx

; 1368 : 		pAssoc->key = key;

	mov	edx, DWORD PTR _pAssoc$[ebp]
	mov	eax, DWORD PTR _key$[ebp]
	mov	DWORD PTR [edx+8], eax

; 1369 : 		// 'pAssoc->value' is a constructed object, nothing more
; 1370 : 
; 1371 : 		// put into hash table
; 1372 : 		pAssoc->pNext = m_pHashTable[nHash];

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR _pAssoc$[ebp]
	mov	ecx, DWORD PTR _nHash$[ebp]
	mov	edx, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR [eax], edx

; 1373 : 		m_pHashTable[nHash] = pAssoc;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _nHash$[ebp]
	mov	eax, DWORD PTR _pAssoc$[ebp]
	mov	DWORD PTR [ecx+edx*4], eax
$L76656:

; 1375 : 	return pAssoc->value;  // return new reference

	mov	eax, DWORD PTR _pAssoc$[ebp]
	add	eax, 12					; 0000000cH

; 1376 : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??A?$CMap@IIKAAK@@QAEAAKI@Z ENDP			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::operator[]
_TEXT	ENDS
PUBLIC	?FreeAssoc@?$CMap@IIKAAK@@IAEXPAUCAssoc@1@@Z	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::FreeAssoc
PUBLIC	?HashKey@@YGII@Z				; HashKey
PUBLIC	?CompareElements@@YGHPBI0@Z			; CompareElements
;	COMDAT ?RemoveKey@?$CMap@IIKAAK@@QAEHI@Z
_TEXT	SEGMENT
_key$ = 8
_this$ = -12
_ppAssocPrev$ = -4
_pAssoc$ = -8
?RemoveKey@?$CMap@IIKAAK@@QAEHI@Z PROC NEAR		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::RemoveKey, COMDAT

; 1381 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1382 : 	ASSERT_VALID(this);

	push	1382					; 00000566H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1383 : 
; 1384 : 	if (m_pHashTable == NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	jne	SHORT $L76662

; 1385 : 		return FALSE;  // nothing in the table

	xor	eax, eax
	jmp	SHORT $L76661
$L76662:

; 1388 : 	ppAssocPrev = &m_pHashTable[HashKey<ARG_KEY>(key) % m_nHashTableSize];

	mov	edx, DWORD PTR _key$[ebp]
	push	edx
	call	?HashKey@@YGII@Z			; HashKey
	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	div	DWORD PTR [ecx+8]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	lea	edx, DWORD PTR [ecx+edx*4]
	mov	DWORD PTR _ppAssocPrev$[ebp], edx

; 1389 : 
; 1390 : 	CAssoc* pAssoc;
; 1391 : 	for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)

	mov	eax, DWORD PTR _ppAssocPrev$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssoc$[ebp], ecx
	jmp	SHORT $L76669
$L76670:
	mov	edx, DWORD PTR _pAssoc$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _pAssoc$[ebp], eax
$L76669:
	cmp	DWORD PTR _pAssoc$[ebp], 0
	je	SHORT $L76671

; 1393 : 		if (CompareElements(&pAssoc->key, &key))

	lea	ecx, DWORD PTR _key$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pAssoc$[ebp]
	add	edx, 8
	push	edx
	call	?CompareElements@@YGHPBI0@Z		; CompareElements
	test	eax, eax
	je	SHORT $L76678

; 1395 : 			// remove it
; 1396 : 			*ppAssocPrev = pAssoc->pNext;  // remove from list

	mov	eax, DWORD PTR _ppAssocPrev$[ebp]
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR [eax], edx

; 1397 : 			FreeAssoc(pAssoc);

	mov	eax, DWORD PTR _pAssoc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FreeAssoc@?$CMap@IIKAAK@@IAEXPAUCAssoc@1@@Z ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::FreeAssoc

; 1398 : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L76661
$L76678:

; 1400 : 		ppAssocPrev = &pAssoc->pNext;

	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	DWORD PTR _ppAssocPrev$[ebp], ecx

; 1401 : 	}

	jmp	SHORT $L76670
$L76671:

; 1402 : 	return FALSE;  // not found

	xor	eax, eax
$L76661:

; 1403 : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?RemoveKey@?$CMap@IIKAAK@@QAEHI@Z ENDP			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::RemoveKey
_TEXT	ENDS
;	COMDAT ?GetStartPosition@?$CMap@IIKAAK@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetStartPosition@?$CMap@IIKAAK@@QBEPAU__POSITION@@XZ PROC NEAR ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetStartPosition, COMDAT

; 1191 : 	{ return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+12]
	neg	eax
	sbb	eax, eax
	mov	esp, ebp
	pop	ebp
	ret	0
?GetStartPosition@?$CMap@IIKAAK@@QBEPAU__POSITION@@XZ ENDP ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetStartPosition
_TEXT	ENDS
EXTRN	?AfxIsValidAddress@@YGHPBXIH@Z:NEAR		; AfxIsValidAddress
;	COMDAT ?GetNextAssoc@?$CMap@IIKAAK@@QBEXAAPAU__POSITION@@AAIAAK@Z
_TEXT	SEGMENT
_rNextPosition$ = 8
_rKey$ = 12
_rValue$ = 16
_this$ = -20
_pAssocRet$ = -4
_nBucket$76702 = -12
_pAssocNext$ = -8
_nBucket$76718 = -16
?GetNextAssoc@?$CMap@IIKAAK@@QBEXAAPAU__POSITION@@AAIAAK@Z PROC NEAR ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetNextAssoc, COMDAT

; 1408 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 20					; 00000014H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 1409 : 	ASSERT_VALID(this);

	push	1409					; 00000581H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject
$L76689:

; 1410 : 	ASSERT(m_pHashTable != NULL);  // never call on empty map

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	jne	SHORT $L76692
	push	1410					; 00000582H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76692
	int	3
$L76692:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76689

; 1411 : 
; 1412 : 	CAssoc* pAssocRet = (CAssoc*)rNextPosition;

	mov	eax, DWORD PTR _rNextPosition$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssocRet$[ebp], ecx
$L76695:

; 1413 : 	ASSERT(pAssocRet != NULL);

	cmp	DWORD PTR _pAssocRet$[ebp], 0
	jne	SHORT $L76698
	push	1413					; 00000585H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76698
	int	3
$L76698:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76695

; 1414 : 
; 1415 : 	if (pAssocRet == (CAssoc*) BEFORE_START_POSITION)

	cmp	DWORD PTR _pAssocRet$[ebp], -1
	jne	SHORT $L76709

; 1417 : 		// find the first association
; 1418 : 		for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)

	mov	DWORD PTR _nBucket$76702[ebp], 0
	jmp	SHORT $L76703
$L76704:
	mov	eax, DWORD PTR _nBucket$76702[ebp]
	add	eax, 1
	mov	DWORD PTR _nBucket$76702[ebp], eax
$L76703:
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nBucket$76702[ebp]
	cmp	edx, DWORD PTR [ecx+8]
	jae	SHORT $L76705

; 1419 : 			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _nBucket$76702[ebp]
	mov	eax, DWORD PTR [ecx+edx*4]
	mov	DWORD PTR _pAssocRet$[ebp], eax
	cmp	DWORD PTR _pAssocRet$[ebp], 0
	je	SHORT $L76706

; 1420 : 				break;

	jmp	SHORT $L76705
$L76706:

; 1421 : 		ASSERT(pAssocRet != NULL);  // must find something

	jmp	SHORT $L76704
$L76705:
	cmp	DWORD PTR _pAssocRet$[ebp], 0
	jne	SHORT $L76710
	push	1421					; 0000058dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76710
	int	3
$L76710:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76705
$L76709:

; 1423 : 
; 1424 : 	// find next association
; 1425 : 	ASSERT(AfxIsValidAddress(pAssocRet, sizeof(CAssoc)));

	push	1
	push	16					; 00000010H
	mov	edx, DWORD PTR _pAssocRet$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76715
	push	1425					; 00000591H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76715
	int	3
$L76715:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76709

; 1426 : 	CAssoc* pAssocNext;
; 1427 : 	if ((pAssocNext = pAssocRet->pNext) == NULL)

	mov	ecx, DWORD PTR _pAssocRet$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _pAssocNext$[ebp], edx
	cmp	DWORD PTR _pAssocNext$[ebp], 0
	jne	SHORT $L76721

; 1429 : 		// go to next bucket
; 1430 : 		for (UINT nBucket = pAssocRet->nHashValue + 1;

	mov	eax, DWORD PTR _pAssocRet$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	add	ecx, 1
	mov	DWORD PTR _nBucket$76718[ebp], ecx

; 1431 : 		  nBucket < m_nHashTableSize; nBucket++)

	jmp	SHORT $L76719
$L76720:
	mov	edx, DWORD PTR _nBucket$76718[ebp]
	add	edx, 1
	mov	DWORD PTR _nBucket$76718[ebp], edx
$L76719:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nBucket$76718[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jae	SHORT $L76721

; 1432 : 			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _nBucket$76718[ebp]
	mov	edx, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR _pAssocNext$[ebp], edx
	cmp	DWORD PTR _pAssocNext$[ebp], 0
	je	SHORT $L76722

; 1433 : 				break;

	jmp	SHORT $L76721
$L76722:

; 1434 : 	}

	jmp	SHORT $L76720
$L76721:

; 1435 : 
; 1436 : 	rNextPosition = (POSITION) pAssocNext;

	mov	eax, DWORD PTR _rNextPosition$[ebp]
	mov	ecx, DWORD PTR _pAssocNext$[ebp]
	mov	DWORD PTR [eax], ecx

; 1437 : 
; 1438 : 	// fill in return data
; 1439 : 	rKey = pAssocRet->key;

	mov	edx, DWORD PTR _rKey$[ebp]
	mov	eax, DWORD PTR _pAssocRet$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR [edx], ecx

; 1440 : 	rValue = pAssocRet->value;

	mov	edx, DWORD PTR _rValue$[ebp]
	mov	eax, DWORD PTR _pAssocRet$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR [edx], ecx

; 1441 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?GetNextAssoc@?$CMap@IIKAAK@@QBEXAAPAU__POSITION@@AAIAAK@Z ENDP ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetNextAssoc
_TEXT	ENDS
PUBLIC	?RemoveAll@?$CMap@IIKAAK@@QAEXXZ		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::RemoveAll
;	COMDAT xdata$x
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
xdata$x	SEGMENT
$T78115	DD	019930520H
	DD	01H
	DD	FLAT:$T78117
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T78117	DD	0ffffffffH
	DD	FLAT:$L78113
xdata$x	ENDS
;	COMDAT ??1?$CMap@IIKAAK@@UAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??1?$CMap@IIKAAK@@UAE@XZ PROC NEAR			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::~CMap<unsigned int,unsigned int,unsigned long,unsigned long &>, COMDAT

; 1270 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L78116
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CMap@IIKAAK@@6B@ ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1271 : 	RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CMap@IIKAAK@@QAEXXZ	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::RemoveAll
$L76727:

; 1272 : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L76730
	push	1272					; 000004f8H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76730
	int	3
$L76730:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76727

; 1273 : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L78113:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L78116:
	mov	eax, OFFSET FLAT:$T78115
	jmp	___CxxFrameHandler
text$x	ENDS
??1?$CMap@IIKAAK@@UAE@XZ ENDP				; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::~CMap<unsigned int,unsigned int,unsigned long,unsigned long &>
PUBLIC	?SetAt@?$CMap@IIKAAK@@QAEXIAAK@Z		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::SetAt
;	COMDAT ?Serialize@?$CMap@IIKAAK@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT
_ar$ = 8
_this$ = -24
_nHash$76741 = -4
_pAssoc$76745 = -8
_nNewCount$76760 = -12
_newKey$76764 = -16
_newValue$76765 = -20
?Serialize@?$CMap@IIKAAK@@UAEXAAVCArchive@@@Z PROC NEAR	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::Serialize, COMDAT

; 1445 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 24					; 00000018H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 1446 : 	ASSERT_VALID(this);

	push	1446					; 000005a6H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1447 : 
; 1448 : 	CObject::Serialize(ar);

	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Serialize@CObject@@UAEXAAVCArchive@@@Z	; CObject::Serialize

; 1449 : 
; 1450 : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	$L76735

; 1452 : 		ar.WriteCount(m_nCount);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?WriteCount@CArchive@@QAEXK@Z		; CArchive::WriteCount

; 1453 : 		if (m_nCount == 0)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jne	SHORT $L76736

; 1454 : 			return;  // nothing more to do

	jmp	$L76734
$L76736:

; 1455 : 
; 1456 : 		ASSERT(m_pHashTable != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	jne	SHORT $L76740
	push	1456					; 000005b0H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76740
	int	3
$L76740:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76736

; 1457 : 		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)

	mov	DWORD PTR _nHash$76741[ebp], 0
	jmp	SHORT $L76742
$L76743:
	mov	ecx, DWORD PTR _nHash$76741[ebp]
	add	ecx, 1
	mov	DWORD PTR _nHash$76741[ebp], ecx
$L76742:
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nHash$76741[ebp]
	cmp	eax, DWORD PTR [edx+8]
	jae	SHORT $L76744

; 1459 : 			CAssoc* pAssoc;
; 1460 : 			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR _nHash$76741[ebp]
	mov	ecx, DWORD PTR [edx+eax*4]
	mov	DWORD PTR _pAssoc$76745[ebp], ecx

; 1461 : 			  pAssoc = pAssoc->pNext)

	jmp	SHORT $L76746
$L76747:
	mov	edx, DWORD PTR _pAssoc$76745[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _pAssoc$76745[ebp], eax
$L76746:
	cmp	DWORD PTR _pAssoc$76745[ebp], 0
	je	SHORT $L76748

; 1463 : 				SerializeElements<KEY>(ar, &pAssoc->key, 1);

	push	1
	mov	ecx, DWORD PTR _pAssoc$76745[ebp]
	add	ecx, 8
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAIH@Z ; SerializeElements

; 1464 : 				SerializeElements<VALUE>(ar, &pAssoc->value, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$76745[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	call	?SerializeElements@@YGXAAVCArchive@@PAKH@Z ; SerializeElements

; 1465 : 			}

	jmp	SHORT $L76747
$L76748:

; 1466 : 		}

	jmp	SHORT $L76743
$L76744:

; 1468 : 	else

	jmp	SHORT $L76763
$L76735:

; 1470 : 		DWORD nNewCount = ar.ReadCount();

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?ReadCount@CArchive@@QAEKXZ		; CArchive::ReadCount
	mov	DWORD PTR _nNewCount$76760[ebp], eax
$L76762:

; 1471 : 		while (nNewCount--)

	mov	edx, DWORD PTR _nNewCount$76760[ebp]
	mov	eax, DWORD PTR _nNewCount$76760[ebp]
	sub	eax, 1
	mov	DWORD PTR _nNewCount$76760[ebp], eax
	test	edx, edx
	je	SHORT $L76763

; 1473 : 			KEY newKey;
; 1474 : 			VALUE newValue;
; 1475 : 			SerializeElements<KEY>(ar, &newKey, 1);

	push	1
	lea	ecx, DWORD PTR _newKey$76764[ebp]
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAIH@Z ; SerializeElements

; 1476 : 			SerializeElements<VALUE>(ar, &newValue, 1);

	push	1
	lea	eax, DWORD PTR _newValue$76765[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	call	?SerializeElements@@YGXAAVCArchive@@PAKH@Z ; SerializeElements

; 1477 : 			SetAt(newKey, newValue);

	lea	edx, DWORD PTR _newValue$76765[ebp]
	push	edx
	mov	eax, DWORD PTR _newKey$76764[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetAt@?$CMap@IIKAAK@@QAEXIAAK@Z	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::SetAt

; 1478 : 		}

	jmp	SHORT $L76762
$L76763:
$L76734:

; 1480 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Serialize@?$CMap@IIKAAK@@UAEXAAVCArchive@@@Z ENDP	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::Serialize
_TEXT	ENDS
;	COMDAT ?Dump@?$CMap@IIKAAK@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT
_dc$ = 8
_this$ = -16
_key$76781 = -8
_val$76782 = -4
_pos$76783 = -12
?Dump@?$CMap@IIKAAK@@UBEXAAVCDumpContext@@@Z PROC NEAR	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::Dump, COMDAT

; 1485 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 1486 : 	CObject::Dump(dc);

	mov	eax, DWORD PTR _dc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Dump@CObject@@UBEXAAVCDumpContext@@@Z	; CObject::Dump

; 1487 : 
; 1488 : 	dc << "with " << m_nCount << " elements";

	push	OFFSET FLAT:??_C@_09LOPF@?5elements?$AA@ ; `string'
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	push	edx
	push	OFFSET FLAT:??_C@_05KIFI@with?5?$AA@	; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<
	mov	ecx, eax
	call	??6CDumpContext@@QAEAAV0@H@Z		; CDumpContext::operator<<
	mov	ecx, eax
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1489 : 	if (dc.GetDepth() > 0)

	mov	ecx, DWORD PTR _dc$[ebp]
	call	?GetDepth@CDumpContext@@QBEHXZ		; CDumpContext::GetDepth
	test	eax, eax
	jle	SHORT $L76786

; 1491 : 		// Dump in format "[key] -> value"
; 1492 : 		KEY key;
; 1493 : 		VALUE val;
; 1494 : 
; 1495 : 		POSITION pos = GetStartPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetStartPosition@?$CMap@IIKAAK@@QBEPAU__POSITION@@XZ ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetStartPosition
	mov	DWORD PTR _pos$76783[ebp], eax
$L76785:

; 1496 : 		while (pos != NULL)

	cmp	DWORD PTR _pos$76783[ebp], 0
	je	SHORT $L76786

; 1498 : 			GetNextAssoc(pos, key, val);

	lea	eax, DWORD PTR _val$76782[ebp]
	push	eax
	lea	ecx, DWORD PTR _key$76781[ebp]
	push	ecx
	lea	edx, DWORD PTR _pos$76783[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetNextAssoc@?$CMap@IIKAAK@@QBEXAAPAU__POSITION@@AAIAAK@Z ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetNextAssoc

; 1499 : 			dc << "\n\t[";

	push	OFFSET FLAT:??_C@_03DANH@?6?7?$FL?$AA@	; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1500 : 			DumpElements<KEY>(dc, &key, 1);

	push	1
	lea	eax, DWORD PTR _key$76781[ebp]
	push	eax
	mov	ecx, DWORD PTR _dc$[ebp]
	push	ecx
	call	?DumpElements@@YGXAAVCDumpContext@@PBIH@Z ; DumpElements

; 1501 : 			dc << "] = ";

	push	OFFSET FLAT:??_C@_04MNLE@?$FN?5?$DN?5?$AA@ ; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1502 : 			DumpElements<VALUE>(dc, &val, 1);

	push	1
	lea	edx, DWORD PTR _val$76782[ebp]
	push	edx
	mov	eax, DWORD PTR _dc$[ebp]
	push	eax
	call	?DumpElements@@YGXAAVCDumpContext@@PBKH@Z ; DumpElements

; 1503 : 		}

	jmp	SHORT $L76785
$L76786:

; 1505 : 
; 1506 : 	dc << "\n";

	push	OFFSET FLAT:??_C@_01BJG@?6?$AA@		; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1507 : }

	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Dump@?$CMap@IIKAAK@@UBEXAAVCDumpContext@@@Z ENDP	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::Dump
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CMap@IIKAAK@@UBEXXZ
_TEXT	SEGMENT
_this$ = -4
?AssertValid@?$CMap@IIKAAK@@UBEXXZ PROC NEAR		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::AssertValid, COMDAT

; 1511 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1512 : 	CObject::AssertValid();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?AssertValid@CObject@@UBEXXZ		; CObject::AssertValid
$L76800:

; 1513 : 
; 1514 : 	ASSERT(m_nHashTableSize > 0);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	ja	SHORT $L76803
	push	1514					; 000005eaH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76803
	int	3
$L76803:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76800
$L76802:

; 1515 : 	ASSERT(m_nCount == 0 || m_pHashTable != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+12], 0
	je	SHORT $L76807
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L76807
	push	1515					; 000005ebH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76807
	int	3
$L76807:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76802

; 1516 : 		// non-empty map should have hash table
; 1517 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?AssertValid@?$CMap@IIKAAK@@UBEXXZ ENDP			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::AssertValid
_TEXT	ENDS
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_G?$CMap@KKIAAI@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_G?$CMap@KKIAAI@@UAEPAXI@Z PROC NEAR			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1?$CMap@KKIAAI@@UAE@XZ		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::~CMap<unsigned long,unsigned long,unsigned int,unsigned int &>
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L76977
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L76977:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_G?$CMap@KKIAAI@@UAEPAXI@Z ENDP			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@IIKAAK@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_G?$CMap@IIKAAK@@UAEPAXI@Z PROC NEAR			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1?$CMap@IIKAAK@@UAE@XZ		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::~CMap<unsigned int,unsigned int,unsigned long,unsigned long &>
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L76980
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L76980:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_G?$CMap@IIKAAK@@UAEPAXI@Z ENDP			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@KKIAAI@@QAEXKAAI@Z
_TEXT	SEGMENT
_key$ = 8
_newValue$ = 12
_this$ = -4
?SetAt@?$CMap@KKIAAI@@QAEXKAAI@Z PROC NEAR		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::SetAt, COMDAT

; 1188 : 	{ (*this)[key] = newValue; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _key$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??A?$CMap@KKIAAI@@QAEAAIK@Z		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::operator[]
	mov	ecx, DWORD PTR _newValue$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR [eax], edx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SetAt@?$CMap@KKIAAI@@QAEXKAAI@Z ENDP			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::SetAt
_TEXT	ENDS
PUBLIC	?DestructElements@@YGXPAIH@Z			; DestructElements
PUBLIC	?DestructElements@@YGXPAKH@Z			; DestructElements
EXTRN	?FreeDataChain@CPlex@@QAEXXZ:NEAR		; CPlex::FreeDataChain
;	COMDAT ?RemoveAll@?$CMap@KKIAAI@@QAEXXZ
_TEXT	SEGMENT
_this$ = -16
_nHash$76998 = -4
_pAssoc$77002 = -8
$T78134 = -12
?RemoveAll@?$CMap@KKIAAI@@QAEXXZ PROC NEAR		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::RemoveAll, COMDAT

; 1240 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 1241 : 	ASSERT_VALID(this);

	push	1241					; 000004d9H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1242 : 
; 1243 : 	if (m_pHashTable != NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L77001

; 1245 : 		// destroy elements (values and keys)
; 1246 : 		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)

	mov	DWORD PTR _nHash$76998[ebp], 0
	jmp	SHORT $L76999
$L77000:
	mov	edx, DWORD PTR _nHash$76998[ebp]
	add	edx, 1
	mov	DWORD PTR _nHash$76998[ebp], edx
$L76999:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nHash$76998[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jae	SHORT $L77001

; 1248 : 			CAssoc* pAssoc;
; 1249 : 			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _nHash$76998[ebp]
	mov	edx, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR _pAssoc$77002[ebp], edx

; 1250 : 			  pAssoc = pAssoc->pNext)

	jmp	SHORT $L77003
$L77004:
	mov	eax, DWORD PTR _pAssoc$77002[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssoc$77002[ebp], ecx
$L77003:
	cmp	DWORD PTR _pAssoc$77002[ebp], 0
	je	SHORT $L77005

; 1252 : 				DestructElements<VALUE>(&pAssoc->value, 1);

	push	1
	mov	edx, DWORD PTR _pAssoc$77002[ebp]
	add	edx, 12					; 0000000cH
	push	edx
	call	?DestructElements@@YGXPAIH@Z		; DestructElements

; 1253 : 				DestructElements<KEY>(&pAssoc->key, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$77002[ebp]
	add	eax, 8
	push	eax
	call	?DestructElements@@YGXPAKH@Z		; DestructElements

; 1254 : 			}

	jmp	SHORT $L77004
$L77005:

; 1255 : 		}

	jmp	SHORT $L77000
$L77001:

; 1257 : 
; 1258 : 	// free hash table
; 1259 : 	delete[] m_pHashTable;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR $T78134[ebp], edx
	mov	eax, DWORD PTR $T78134[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 1260 : 	m_pHashTable = NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+4], 0

; 1261 : 
; 1262 : 	m_nCount = 0;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12], 0

; 1263 : 	m_pFreeList = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+16], 0

; 1264 : 	m_pBlocks->FreeDataChain();

	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+20]
	call	?FreeDataChain@CPlex@@QAEXXZ		; CPlex::FreeDataChain

; 1265 : 	m_pBlocks = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+20], 0

; 1266 : }

	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?RemoveAll@?$CMap@KKIAAI@@QAEXXZ ENDP			; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::RemoveAll
_TEXT	ENDS
;	COMDAT ?GetStartPosition@?$CMap@KKIAAI@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetStartPosition@?$CMap@KKIAAI@@QBEPAU__POSITION@@XZ PROC NEAR ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::GetStartPosition, COMDAT

; 1191 : 	{ return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+12]
	neg	eax
	sbb	eax, eax
	mov	esp, ebp
	pop	ebp
	ret	0
?GetStartPosition@?$CMap@KKIAAI@@QBEPAU__POSITION@@XZ ENDP ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::GetStartPosition
_TEXT	ENDS
;	COMDAT ?GetNextAssoc@?$CMap@KKIAAI@@QBEXAAPAU__POSITION@@AAKAAI@Z
_TEXT	SEGMENT
_rNextPosition$ = 8
_rKey$ = 12
_rValue$ = 16
_this$ = -20
_pAssocRet$ = -4
_nBucket$77040 = -12
_pAssocNext$ = -8
_nBucket$77056 = -16
?GetNextAssoc@?$CMap@KKIAAI@@QBEXAAPAU__POSITION@@AAKAAI@Z PROC NEAR ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::GetNextAssoc, COMDAT

; 1408 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 20					; 00000014H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 1409 : 	ASSERT_VALID(this);

	push	1409					; 00000581H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject
$L77027:

; 1410 : 	ASSERT(m_pHashTable != NULL);  // never call on empty map

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	jne	SHORT $L77030
	push	1410					; 00000582H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77030
	int	3
$L77030:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77027

; 1411 : 
; 1412 : 	CAssoc* pAssocRet = (CAssoc*)rNextPosition;

	mov	eax, DWORD PTR _rNextPosition$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssocRet$[ebp], ecx
$L77033:

; 1413 : 	ASSERT(pAssocRet != NULL);

	cmp	DWORD PTR _pAssocRet$[ebp], 0
	jne	SHORT $L77036
	push	1413					; 00000585H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77036
	int	3
$L77036:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77033

; 1414 : 
; 1415 : 	if (pAssocRet == (CAssoc*) BEFORE_START_POSITION)

	cmp	DWORD PTR _pAssocRet$[ebp], -1
	jne	SHORT $L77047

; 1417 : 		// find the first association
; 1418 : 		for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)

	mov	DWORD PTR _nBucket$77040[ebp], 0
	jmp	SHORT $L77041
$L77042:
	mov	eax, DWORD PTR _nBucket$77040[ebp]
	add	eax, 1
	mov	DWORD PTR _nBucket$77040[ebp], eax
$L77041:
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nBucket$77040[ebp]
	cmp	edx, DWORD PTR [ecx+8]
	jae	SHORT $L77043

; 1419 : 			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _nBucket$77040[ebp]
	mov	eax, DWORD PTR [ecx+edx*4]
	mov	DWORD PTR _pAssocRet$[ebp], eax
	cmp	DWORD PTR _pAssocRet$[ebp], 0
	je	SHORT $L77044

; 1420 : 				break;

	jmp	SHORT $L77043
$L77044:

; 1421 : 		ASSERT(pAssocRet != NULL);  // must find something

	jmp	SHORT $L77042
$L77043:
	cmp	DWORD PTR _pAssocRet$[ebp], 0
	jne	SHORT $L77048
	push	1421					; 0000058dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77048
	int	3
$L77048:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L77043
$L77047:

; 1423 : 
; 1424 : 	// find next association
; 1425 : 	ASSERT(AfxIsValidAddress(pAssocRet, sizeof(CAssoc)));

	push	1
	push	16					; 00000010H
	mov	edx, DWORD PTR _pAssocRet$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77053
	push	1425					; 00000591H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77053
	int	3
$L77053:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L77047

; 1426 : 	CAssoc* pAssocNext;
; 1427 : 	if ((pAssocNext = pAssocRet->pNext) == NULL)

	mov	ecx, DWORD PTR _pAssocRet$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _pAssocNext$[ebp], edx
	cmp	DWORD PTR _pAssocNext$[ebp], 0
	jne	SHORT $L77059

; 1429 : 		// go to next bucket
; 1430 : 		for (UINT nBucket = pAssocRet->nHashValue + 1;

	mov	eax, DWORD PTR _pAssocRet$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	add	ecx, 1
	mov	DWORD PTR _nBucket$77056[ebp], ecx

; 1431 : 		  nBucket < m_nHashTableSize; nBucket++)

	jmp	SHORT $L77057
$L77058:
	mov	edx, DWORD PTR _nBucket$77056[ebp]
	add	edx, 1
	mov	DWORD PTR _nBucket$77056[ebp], edx
$L77057:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nBucket$77056[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jae	SHORT $L77059

; 1432 : 			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _nBucket$77056[ebp]
	mov	edx, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR _pAssocNext$[ebp], edx
	cmp	DWORD PTR _pAssocNext$[ebp], 0
	je	SHORT $L77060

; 1433 : 				break;

	jmp	SHORT $L77059
$L77060:

; 1434 : 	}

	jmp	SHORT $L77058
$L77059:

; 1435 : 
; 1436 : 	rNextPosition = (POSITION) pAssocNext;

	mov	eax, DWORD PTR _rNextPosition$[ebp]
	mov	ecx, DWORD PTR _pAssocNext$[ebp]
	mov	DWORD PTR [eax], ecx

; 1437 : 
; 1438 : 	// fill in return data
; 1439 : 	rKey = pAssocRet->key;

	mov	edx, DWORD PTR _rKey$[ebp]
	mov	eax, DWORD PTR _pAssocRet$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR [edx], ecx

; 1440 : 	rValue = pAssocRet->value;

	mov	edx, DWORD PTR _rValue$[ebp]
	mov	eax, DWORD PTR _pAssocRet$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR [edx], ecx

; 1441 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?GetNextAssoc@?$CMap@KKIAAI@@QBEXAAPAU__POSITION@@AAKAAI@Z ENDP ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::GetNextAssoc
_TEXT	ENDS
EXTRN	_memset:NEAR
EXTRN	??2@YAPAXIPBDH@Z:NEAR				; operator new
;	COMDAT ?InitHashTable@?$CMap@KKIAAI@@QAEXIH@Z
_TEXT	SEGMENT
_nHashSize$ = 8
_bAllocNow$ = 12
_this$ = -12
$T78141 = -4
$T78142 = -8
?InitHashTable@?$CMap@KKIAAI@@QAEXIH@Z PROC NEAR	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::InitHashTable, COMDAT

; 1218 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1219 : 	ASSERT_VALID(this);

	push	1219					; 000004c3H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject
$L77067:

; 1220 : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L77070
	push	1220					; 000004c4H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77070
	int	3
$L77070:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77067
$L77069:

; 1221 : 	ASSERT(nHashSize > 0);

	cmp	DWORD PTR _nHashSize$[ebp], 0
	ja	SHORT $L77074
	push	1221					; 000004c5H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77074
	int	3
$L77074:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L77069

; 1222 : 
; 1223 : 	if (m_pHashTable != NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L77075

; 1225 : 		// free hash table
; 1226 : 		delete[] m_pHashTable;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR $T78141[ebp], eax
	mov	ecx, DWORD PTR $T78141[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 1227 : 		m_pHashTable = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0
$L77075:

; 1229 : 
; 1230 : 	if (bAllocNow)

	cmp	DWORD PTR _bAllocNow$[ebp], 0
	je	SHORT $L77077

; 1232 : 		m_pHashTable = new CAssoc* [nHashSize];

	push	1232					; 000004d0H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _nHashSize$[ebp]
	shl	eax, 2
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T78142[ebp], eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR $T78142[ebp]
	mov	DWORD PTR [ecx+4], edx

; 1233 : 		memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);

	mov	eax, DWORD PTR _nHashSize$[ebp]
	shl	eax, 2
	push	eax
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	_memset
	add	esp, 12					; 0000000cH
$L77077:

; 1235 : 	m_nHashTableSize = nHashSize;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nHashSize$[ebp]
	mov	DWORD PTR [eax+8], ecx

; 1236 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?InitHashTable@?$CMap@KKIAAI@@QAEXIH@Z ENDP		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::InitHashTable
_TEXT	ENDS
PUBLIC	?ConstructElements@@YGXPAKH@Z			; ConstructElements
PUBLIC	?ConstructElements@@YGXPAIH@Z			; ConstructElements
PUBLIC	?data@CPlex@@QAEPAXXZ				; CPlex::data
EXTRN	?Create@CPlex@@SGPAU1@AAPAU1@II@Z:NEAR		; CPlex::Create
;	COMDAT ?NewAssoc@?$CMap@KKIAAI@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT
_this$ = -20
_newBlock$77085 = -8
_pAssoc$77087 = -16
_i$77089 = -12
_pAssoc$ = -4
?NewAssoc@?$CMap@KKIAAI@@IAEPAUCAssoc@1@XZ PROC NEAR	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::NewAssoc, COMDAT

; 1278 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 20					; 00000014H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 1279 : 	if (m_pFreeList == NULL)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	jne	SHORT $L77092

; 1281 : 		// add another block
; 1282 : 		CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CMap::CAssoc));

	push	16					; 00000010H
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 20					; 00000014H
	push	eax
	call	?Create@CPlex@@SGPAU1@AAPAU1@II@Z	; CPlex::Create
	mov	DWORD PTR _newBlock$77085[ebp], eax

; 1283 : 		// chain them into free list
; 1284 : 		CMap::CAssoc* pAssoc = (CMap::CAssoc*) newBlock->data();

	mov	ecx, DWORD PTR _newBlock$77085[ebp]
	call	?data@CPlex@@QAEPAXXZ			; CPlex::data
	mov	DWORD PTR _pAssoc$77087[ebp], eax

; 1285 : 		// free in reverse order to make it easier to debug
; 1286 : 		pAssoc += m_nBlockSize - 1;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	shl	edx, 4
	mov	eax, DWORD PTR _pAssoc$77087[ebp]
	add	eax, edx
	mov	DWORD PTR _pAssoc$77087[ebp], eax

; 1287 : 		for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	mov	DWORD PTR _i$77089[ebp], edx
	jmp	SHORT $L77090
$L77091:
	mov	eax, DWORD PTR _i$77089[ebp]
	sub	eax, 1
	mov	DWORD PTR _i$77089[ebp], eax
	mov	ecx, DWORD PTR _pAssoc$77087[ebp]
	sub	ecx, 16					; 00000010H
	mov	DWORD PTR _pAssoc$77087[ebp], ecx
$L77090:
	cmp	DWORD PTR _i$77089[ebp], 0
	jl	SHORT $L77092

; 1289 : 			pAssoc->pNext = m_pFreeList;

	mov	edx, DWORD PTR _pAssoc$77087[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR [edx], ecx

; 1290 : 			m_pFreeList = pAssoc;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _pAssoc$77087[ebp]
	mov	DWORD PTR [edx+16], eax

; 1291 : 		}

	jmp	SHORT $L77091
$L77092:

; 1293 : 	ASSERT(m_pFreeList != NULL);  // we must have something

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+16], 0
	jne	SHORT $L77096
	push	1293					; 0000050dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77096
	int	3
$L77096:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77092

; 1294 : 
; 1295 : 	CMap::CAssoc* pAssoc = m_pFreeList;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR _pAssoc$[ebp], ecx

; 1296 : 	m_pFreeList = m_pFreeList->pNext;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	DWORD PTR [ecx+16], edx

; 1297 : 	m_nCount++;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	add	ecx, 1
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12], ecx
$L77098:

; 1298 : 	ASSERT(m_nCount > 0);  // make sure we don't overflow

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jg	SHORT $L77101
	push	1298					; 00000512H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77101
	int	3
$L77101:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L77098

; 1299 : 	ConstructElements<KEY>(&pAssoc->key, 1);

	push	1
	mov	edx, DWORD PTR _pAssoc$[ebp]
	add	edx, 8
	push	edx
	call	?ConstructElements@@YGXPAKH@Z		; ConstructElements

; 1300 : 	ConstructElements<VALUE>(&pAssoc->value, 1);   // special construct values

	push	1
	mov	eax, DWORD PTR _pAssoc$[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	call	?ConstructElements@@YGXPAIH@Z		; ConstructElements

; 1301 : 	return pAssoc;

	mov	eax, DWORD PTR _pAssoc$[ebp]

; 1302 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?NewAssoc@?$CMap@KKIAAI@@IAEPAUCAssoc@1@XZ ENDP		; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::NewAssoc
_TEXT	ENDS
;	COMDAT ?data@CPlex@@QAEPAXXZ
text$AFX_COL1	SEGMENT
_this$ = -4
?data@CPlex@@QAEPAXXZ PROC NEAR				; CPlex::data, COMDAT

; 34   : 	void* data() { return this+1; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 4
	mov	esp, ebp
	pop	ebp
	ret	0
?data@CPlex@@QAEPAXXZ ENDP				; CPlex::data
text$AFX_COL1	ENDS
;	COMDAT ?FreeAssoc@?$CMap@KKIAAI@@IAEXPAUCAssoc@1@@Z
_TEXT	SEGMENT
_pAssoc$ = 8
_this$ = -4
?FreeAssoc@?$CMap@KKIAAI@@IAEXPAUCAssoc@1@@Z PROC NEAR	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::FreeAssoc, COMDAT

; 1306 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1307 : 	DestructElements<VALUE>(&pAssoc->value, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	call	?DestructElements@@YGXPAIH@Z		; DestructElements

; 1308 : 	DestructElements<KEY>(&pAssoc->key, 1);

	push	1
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	add	ecx, 8
	push	ecx
	call	?DestructElements@@YGXPAKH@Z		; DestructElements

; 1309 : 	pAssoc->pNext = m_pFreeList;

	mov	edx, DWORD PTR _pAssoc$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR [edx], ecx

; 1310 : 	m_pFreeList = pAssoc;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _pAssoc$[ebp]
	mov	DWORD PTR [edx+16], eax

; 1311 : 	m_nCount--;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	sub	edx, 1
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+12], edx
$L77124:

; 1312 : 	ASSERT(m_nCount >= 0);  // make sure we don't underflow

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jge	SHORT $L77127
	push	1312					; 00000520H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77127
	int	3
$L77127:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77124

; 1313 : 
; 1314 : 	// if no more elements, cleanup completely
; 1315 : 	if (m_nCount == 0)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jne	SHORT $L77128

; 1316 : 		RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CMap@KKIAAI@@QAEXXZ	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::RemoveAll
$L77128:

; 1317 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?FreeAssoc@?$CMap@KKIAAI@@IAEXPAUCAssoc@1@@Z ENDP	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::FreeAssoc
_TEXT	ENDS
;	COMDAT ?GetAssocAt@?$CMap@KKIAAI@@IBEPAUCAssoc@1@KAAI@Z
_TEXT	SEGMENT
_key$ = 8
_nHash$ = 12
_this$ = -8
_pAssoc$ = -4
?GetAssocAt@?$CMap@KKIAAI@@IBEPAUCAssoc@1@KAAI@Z PROC NEAR ; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::GetAssocAt, COMDAT

; 1323 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1324 : 	nHash = HashKey<ARG_KEY>(key) % m_nHashTableSize;

	mov	eax, DWORD PTR _key$[ebp]
	push	eax
	call	?HashKey@@YGIK@Z			; HashKey
	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	div	DWORD PTR [ecx+8]
	mov	eax, DWORD PTR _nHash$[ebp]
	mov	DWORD PTR [eax], edx

; 1325 : 
; 1326 : 	if (m_pHashTable == NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	jne	SHORT $L77137

; 1327 : 		return NULL;

	xor	eax, eax
	jmp	SHORT $L77133
$L77137:

; 1331 : 	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)

	mov	edx, DWORD PTR _nHash$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR [edx+eax*4]
	mov	DWORD PTR _pAssoc$[ebp], eax
	jmp	SHORT $L77139
$L77140:
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _pAssoc$[ebp], edx
$L77139:
	cmp	DWORD PTR _pAssoc$[ebp], 0
	je	SHORT $L77141

; 1333 : 		if (CompareElements(&pAssoc->key, &key))

	lea	eax, DWORD PTR _key$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	add	ecx, 8
	push	ecx
	call	?CompareElements@@YGHPBK0@Z		; CompareElements
	test	eax, eax
	je	SHORT $L77147

; 1334 : 			return pAssoc;

	mov	eax, DWORD PTR _pAssoc$[ebp]
	jmp	SHORT $L77133
$L77147:

; 1335 : 	}

	jmp	SHORT $L77140
$L77141:

; 1336 : 	return NULL;

	xor	eax, eax
$L77133:

; 1337 : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?GetAssocAt@?$CMap@KKIAAI@@IBEPAUCAssoc@1@KAAI@Z ENDP	; CMap<unsigned long,unsigned long,unsigned int,unsigned int &>::GetAssocAt
_TEXT	ENDS
;	COMDAT ?HashKey@@YGIK@Z
_TEXT	SEGMENT
_key$ = 8
?HashKey@@YGIK@Z PROC NEAR				; HashKey, COMDAT

; 127  : {

	push	ebp
	mov	ebp, esp

; 128  : 	// default identity hash - works for most primitive values
; 129  : 	return ((UINT)(void*)(DWORD)key) >> 4;

	mov	eax, DWORD PTR _key$[ebp]
	shr	eax, 4

; 130  : }

	pop	ebp
	ret	4
?HashKey@@YGIK@Z ENDP					; HashKey
_TEXT	ENDS
;	COMDAT ?CompareElements@@YGHPBK0@Z
_TEXT	SEGMENT
_pElement1$ = 8
_pElement2$ = 12
?CompareElements@@YGHPBK0@Z PROC NEAR			; CompareElements, COMDAT

; 118  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L77153:

; 119  : 	ASSERT(AfxIsValidAddress(pElement1, sizeof(TYPE), FALSE));

	push	0
	push	4
	mov	eax, DWORD PTR _pElement1$[ebp]
	push	eax
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77157
	push	119					; 00000077H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77157
	int	3
$L77157:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L77153
$L77155:

; 120  : 	ASSERT(AfxIsValidAddress(pElement2, sizeof(ARG_TYPE), FALSE));

	push	0
	push	4
	mov	edx, DWORD PTR _pElement2$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77162
	push	120					; 00000078H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77162
	int	3
$L77162:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L77155

; 121  : 
; 122  : 	return *pElement1 == *pElement2;

	mov	ecx, DWORD PTR _pElement1$[ebp]
	mov	edx, DWORD PTR _pElement2$[ebp]
	mov	eax, DWORD PTR [ecx]
	xor	ecx, ecx
	cmp	eax, DWORD PTR [edx]
	sete	cl
	mov	eax, ecx

; 123  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?CompareElements@@YGHPBK0@Z ENDP			; CompareElements
_TEXT	ENDS
EXTRN	?Read@CArchive@@QAEIPAXI@Z:NEAR			; CArchive::Read
EXTRN	?Write@CArchive@@QAEXPBXI@Z:NEAR		; CArchive::Write
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAKH@Z
_TEXT	SEGMENT
_ar$ = 8
_pElements$ = 12
_nCount$ = 16
?SerializeElements@@YGXAAVCArchive@@PAKH@Z PROC NEAR	; SerializeElements, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L77164:

; 92   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77168
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77168
	push	93					; 0000005dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77168
	int	3
$L77168:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77164

; 93   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 94   : 
; 95   : 	// default is bit-wise read/write
; 96   : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L77169

; 97   : 		ar.Write((void*)pElements, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Write@CArchive@@QAEXPBXI@Z		; CArchive::Write

; 98   : 	else

	jmp	SHORT $L77172
$L77169:

; 99   : 		ar.Read((void*)pElements, nCount * sizeof(TYPE));

	mov	edx, DWORD PTR _nCount$[ebp]
	shl	edx, 2
	push	edx
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Read@CArchive@@QAEIPAXI@Z		; CArchive::Read
$L77172:

; 100  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	12					; 0000000cH
?SerializeElements@@YGXAAVCArchive@@PAKH@Z ENDP		; SerializeElements
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAIH@Z
_TEXT	SEGMENT
_ar$ = 8
_pElements$ = 12
_nCount$ = 16
?SerializeElements@@YGXAAVCArchive@@PAIH@Z PROC NEAR	; SerializeElements, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L77176:

; 92   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77180
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77180
	push	93					; 0000005dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77180
	int	3
$L77180:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77176

; 93   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 94   : 
; 95   : 	// default is bit-wise read/write
; 96   : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L77181

; 97   : 		ar.Write((void*)pElements, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Write@CArchive@@QAEXPBXI@Z		; CArchive::Write

; 98   : 	else

	jmp	SHORT $L77184
$L77181:

; 99   : 		ar.Read((void*)pElements, nCount * sizeof(TYPE));

	mov	edx, DWORD PTR _nCount$[ebp]
	shl	edx, 2
	push	edx
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Read@CArchive@@QAEIPAXI@Z		; CArchive::Read
$L77184:

; 100  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	12					; 0000000cH
?SerializeElements@@YGXAAVCArchive@@PAIH@Z ENDP		; SerializeElements
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBKH@Z
_TEXT	SEGMENT
_pElements$ = 12
_nCount$ = 16
?DumpElements@@YGXAAVCDumpContext@@PBKH@Z PROC NEAR	; DumpElements, COMDAT

; 105  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L77188:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77192
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77192
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77192
	int	3
$L77192:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77188

; 107  : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE), FALSE));
; 108  : 	&dc; // not used
; 109  : 	pElements;  // not used
; 110  : 	nCount; // not used
; 111  : 
; 112  : 	// default does nothing
; 113  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	12					; 0000000cH
?DumpElements@@YGXAAVCDumpContext@@PBKH@Z ENDP		; DumpElements
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBIH@Z
_TEXT	SEGMENT
_pElements$ = 12
_nCount$ = 16
?DumpElements@@YGXAAVCDumpContext@@PBIH@Z PROC NEAR	; DumpElements, COMDAT

; 105  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L77194:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77198
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77198
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77198
	int	3
$L77198:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77194

; 107  : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE), FALSE));
; 108  : 	&dc; // not used
; 109  : 	pElements;  // not used
; 110  : 	nCount; // not used
; 111  : 
; 112  : 	// default does nothing
; 113  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	12					; 0000000cH
?DumpElements@@YGXAAVCDumpContext@@PBIH@Z ENDP		; DumpElements
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@IIKAAK@@QAEXIAAK@Z
_TEXT	SEGMENT
_key$ = 8
_newValue$ = 12
_this$ = -4
?SetAt@?$CMap@IIKAAK@@QAEXIAAK@Z PROC NEAR		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::SetAt, COMDAT

; 1188 : 	{ (*this)[key] = newValue; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _key$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??A?$CMap@IIKAAK@@QAEAAKI@Z		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::operator[]
	mov	ecx, DWORD PTR _newValue$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR [eax], edx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SetAt@?$CMap@IIKAAK@@QAEXIAAK@Z ENDP			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::SetAt
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@IIKAAK@@QAEXXZ
_TEXT	SEGMENT
$T78167 = -12
_this$ = -16
_nHash$77208 = -4
_pAssoc$77212 = -8
?RemoveAll@?$CMap@IIKAAK@@QAEXXZ PROC NEAR		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::RemoveAll, COMDAT

; 1240 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 1241 : 	ASSERT_VALID(this);

	push	1241					; 000004d9H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1242 : 
; 1243 : 	if (m_pHashTable != NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L77211

; 1245 : 		// destroy elements (values and keys)
; 1246 : 		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)

	mov	DWORD PTR _nHash$77208[ebp], 0
	jmp	SHORT $L77209
$L77210:
	mov	edx, DWORD PTR _nHash$77208[ebp]
	add	edx, 1
	mov	DWORD PTR _nHash$77208[ebp], edx
$L77209:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nHash$77208[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jae	SHORT $L77211

; 1248 : 			CAssoc* pAssoc;
; 1249 : 			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _nHash$77208[ebp]
	mov	edx, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR _pAssoc$77212[ebp], edx

; 1250 : 			  pAssoc = pAssoc->pNext)

	jmp	SHORT $L77213
$L77214:
	mov	eax, DWORD PTR _pAssoc$77212[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssoc$77212[ebp], ecx
$L77213:
	cmp	DWORD PTR _pAssoc$77212[ebp], 0
	je	SHORT $L77215

; 1252 : 				DestructElements<VALUE>(&pAssoc->value, 1);

	push	1
	mov	edx, DWORD PTR _pAssoc$77212[ebp]
	add	edx, 12					; 0000000cH
	push	edx
	call	?DestructElements@@YGXPAKH@Z		; DestructElements

; 1253 : 				DestructElements<KEY>(&pAssoc->key, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$77212[ebp]
	add	eax, 8
	push	eax
	call	?DestructElements@@YGXPAIH@Z		; DestructElements

; 1254 : 			}

	jmp	SHORT $L77214
$L77215:

; 1255 : 		}

	jmp	SHORT $L77210
$L77211:

; 1257 : 
; 1258 : 	// free hash table
; 1259 : 	delete[] m_pHashTable;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR $T78167[ebp], edx
	mov	eax, DWORD PTR $T78167[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 1260 : 	m_pHashTable = NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+4], 0

; 1261 : 
; 1262 : 	m_nCount = 0;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12], 0

; 1263 : 	m_pFreeList = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+16], 0

; 1264 : 	m_pBlocks->FreeDataChain();

	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+20]
	call	?FreeDataChain@CPlex@@QAEXXZ		; CPlex::FreeDataChain

; 1265 : 	m_pBlocks = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+20], 0

; 1266 : }

	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?RemoveAll@?$CMap@IIKAAK@@QAEXXZ ENDP			; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::RemoveAll
_TEXT	ENDS
;	COMDAT ?InitHashTable@?$CMap@IIKAAK@@QAEXIH@Z
_TEXT	SEGMENT
$T78170 = -4
$T78171 = -8
_nHashSize$ = 8
_bAllocNow$ = 12
_this$ = -12
?InitHashTable@?$CMap@IIKAAK@@QAEXIH@Z PROC NEAR	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::InitHashTable, COMDAT

; 1218 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1219 : 	ASSERT_VALID(this);

	push	1219					; 000004c3H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject
$L77230:

; 1220 : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L77233
	push	1220					; 000004c4H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77233
	int	3
$L77233:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77230
$L77232:

; 1221 : 	ASSERT(nHashSize > 0);

	cmp	DWORD PTR _nHashSize$[ebp], 0
	ja	SHORT $L77237
	push	1221					; 000004c5H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77237
	int	3
$L77237:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L77232

; 1222 : 
; 1223 : 	if (m_pHashTable != NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L77238

; 1225 : 		// free hash table
; 1226 : 		delete[] m_pHashTable;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR $T78170[ebp], eax
	mov	ecx, DWORD PTR $T78170[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 1227 : 		m_pHashTable = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0
$L77238:

; 1229 : 
; 1230 : 	if (bAllocNow)

	cmp	DWORD PTR _bAllocNow$[ebp], 0
	je	SHORT $L77240

; 1232 : 		m_pHashTable = new CAssoc* [nHashSize];

	push	1232					; 000004d0H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _nHashSize$[ebp]
	shl	eax, 2
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T78171[ebp], eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR $T78171[ebp]
	mov	DWORD PTR [ecx+4], edx

; 1233 : 		memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);

	mov	eax, DWORD PTR _nHashSize$[ebp]
	shl	eax, 2
	push	eax
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	_memset
	add	esp, 12					; 0000000cH
$L77240:

; 1235 : 	m_nHashTableSize = nHashSize;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nHashSize$[ebp]
	mov	DWORD PTR [eax+8], ecx

; 1236 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?InitHashTable@?$CMap@IIKAAK@@QAEXIH@Z ENDP		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::InitHashTable
_TEXT	ENDS
;	COMDAT ?NewAssoc@?$CMap@IIKAAK@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT
_this$ = -20
_newBlock$77248 = -8
_pAssoc$77250 = -16
_i$77252 = -12
_pAssoc$ = -4
?NewAssoc@?$CMap@IIKAAK@@IAEPAUCAssoc@1@XZ PROC NEAR	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::NewAssoc, COMDAT

; 1278 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 20					; 00000014H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 1279 : 	if (m_pFreeList == NULL)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	jne	SHORT $L77255

; 1281 : 		// add another block
; 1282 : 		CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CMap::CAssoc));

	push	16					; 00000010H
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 20					; 00000014H
	push	eax
	call	?Create@CPlex@@SGPAU1@AAPAU1@II@Z	; CPlex::Create
	mov	DWORD PTR _newBlock$77248[ebp], eax

; 1283 : 		// chain them into free list
; 1284 : 		CMap::CAssoc* pAssoc = (CMap::CAssoc*) newBlock->data();

	mov	ecx, DWORD PTR _newBlock$77248[ebp]
	call	?data@CPlex@@QAEPAXXZ			; CPlex::data
	mov	DWORD PTR _pAssoc$77250[ebp], eax

; 1285 : 		// free in reverse order to make it easier to debug
; 1286 : 		pAssoc += m_nBlockSize - 1;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	shl	edx, 4
	mov	eax, DWORD PTR _pAssoc$77250[ebp]
	add	eax, edx
	mov	DWORD PTR _pAssoc$77250[ebp], eax

; 1287 : 		for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	mov	DWORD PTR _i$77252[ebp], edx
	jmp	SHORT $L77253
$L77254:
	mov	eax, DWORD PTR _i$77252[ebp]
	sub	eax, 1
	mov	DWORD PTR _i$77252[ebp], eax
	mov	ecx, DWORD PTR _pAssoc$77250[ebp]
	sub	ecx, 16					; 00000010H
	mov	DWORD PTR _pAssoc$77250[ebp], ecx
$L77253:
	cmp	DWORD PTR _i$77252[ebp], 0
	jl	SHORT $L77255

; 1289 : 			pAssoc->pNext = m_pFreeList;

	mov	edx, DWORD PTR _pAssoc$77250[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR [edx], ecx

; 1290 : 			m_pFreeList = pAssoc;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _pAssoc$77250[ebp]
	mov	DWORD PTR [edx+16], eax

; 1291 : 		}

	jmp	SHORT $L77254
$L77255:

; 1293 : 	ASSERT(m_pFreeList != NULL);  // we must have something

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+16], 0
	jne	SHORT $L77259
	push	1293					; 0000050dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77259
	int	3
$L77259:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77255

; 1294 : 
; 1295 : 	CMap::CAssoc* pAssoc = m_pFreeList;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR _pAssoc$[ebp], ecx

; 1296 : 	m_pFreeList = m_pFreeList->pNext;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	DWORD PTR [ecx+16], edx

; 1297 : 	m_nCount++;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	add	ecx, 1
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12], ecx
$L77261:

; 1298 : 	ASSERT(m_nCount > 0);  // make sure we don't overflow

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jg	SHORT $L77264
	push	1298					; 00000512H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77264
	int	3
$L77264:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L77261

; 1299 : 	ConstructElements<KEY>(&pAssoc->key, 1);

	push	1
	mov	edx, DWORD PTR _pAssoc$[ebp]
	add	edx, 8
	push	edx
	call	?ConstructElements@@YGXPAIH@Z		; ConstructElements

; 1300 : 	ConstructElements<VALUE>(&pAssoc->value, 1);   // special construct values

	push	1
	mov	eax, DWORD PTR _pAssoc$[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	call	?ConstructElements@@YGXPAKH@Z		; ConstructElements

; 1301 : 	return pAssoc;

	mov	eax, DWORD PTR _pAssoc$[ebp]

; 1302 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?NewAssoc@?$CMap@IIKAAK@@IAEPAUCAssoc@1@XZ ENDP		; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::NewAssoc
_TEXT	ENDS
;	COMDAT ?FreeAssoc@?$CMap@IIKAAK@@IAEXPAUCAssoc@1@@Z
_TEXT	SEGMENT
_pAssoc$ = 8
_this$ = -4
?FreeAssoc@?$CMap@IIKAAK@@IAEXPAUCAssoc@1@@Z PROC NEAR	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::FreeAssoc, COMDAT

; 1306 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1307 : 	DestructElements<VALUE>(&pAssoc->value, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	call	?DestructElements@@YGXPAKH@Z		; DestructElements

; 1308 : 	DestructElements<KEY>(&pAssoc->key, 1);

	push	1
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	add	ecx, 8
	push	ecx
	call	?DestructElements@@YGXPAIH@Z		; DestructElements

; 1309 : 	pAssoc->pNext = m_pFreeList;

	mov	edx, DWORD PTR _pAssoc$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR [edx], ecx

; 1310 : 	m_pFreeList = pAssoc;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _pAssoc$[ebp]
	mov	DWORD PTR [edx+16], eax

; 1311 : 	m_nCount--;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	sub	edx, 1
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+12], edx
$L77285:

; 1312 : 	ASSERT(m_nCount >= 0);  // make sure we don't underflow

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jge	SHORT $L77288
	push	1312					; 00000520H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77288
	int	3
$L77288:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77285

; 1313 : 
; 1314 : 	// if no more elements, cleanup completely
; 1315 : 	if (m_nCount == 0)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jne	SHORT $L77289

; 1316 : 		RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CMap@IIKAAK@@QAEXXZ	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::RemoveAll
$L77289:

; 1317 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?FreeAssoc@?$CMap@IIKAAK@@IAEXPAUCAssoc@1@@Z ENDP	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::FreeAssoc
_TEXT	ENDS
;	COMDAT ?GetAssocAt@?$CMap@IIKAAK@@IBEPAUCAssoc@1@IAAI@Z
_TEXT	SEGMENT
_key$ = 8
_nHash$ = 12
_this$ = -8
_pAssoc$ = -4
?GetAssocAt@?$CMap@IIKAAK@@IBEPAUCAssoc@1@IAAI@Z PROC NEAR ; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetAssocAt, COMDAT

; 1323 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1324 : 	nHash = HashKey<ARG_KEY>(key) % m_nHashTableSize;

	mov	eax, DWORD PTR _key$[ebp]
	push	eax
	call	?HashKey@@YGII@Z			; HashKey
	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	div	DWORD PTR [ecx+8]
	mov	eax, DWORD PTR _nHash$[ebp]
	mov	DWORD PTR [eax], edx

; 1325 : 
; 1326 : 	if (m_pHashTable == NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	jne	SHORT $L77298

; 1327 : 		return NULL;

	xor	eax, eax
	jmp	SHORT $L77294
$L77298:

; 1331 : 	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)

	mov	edx, DWORD PTR _nHash$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR [edx+eax*4]
	mov	DWORD PTR _pAssoc$[ebp], eax
	jmp	SHORT $L77300
$L77301:
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _pAssoc$[ebp], edx
$L77300:
	cmp	DWORD PTR _pAssoc$[ebp], 0
	je	SHORT $L77302

; 1333 : 		if (CompareElements(&pAssoc->key, &key))

	lea	eax, DWORD PTR _key$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	add	ecx, 8
	push	ecx
	call	?CompareElements@@YGHPBI0@Z		; CompareElements
	test	eax, eax
	je	SHORT $L77308

; 1334 : 			return pAssoc;

	mov	eax, DWORD PTR _pAssoc$[ebp]
	jmp	SHORT $L77294
$L77308:

; 1335 : 	}

	jmp	SHORT $L77301
$L77302:

; 1336 : 	return NULL;

	xor	eax, eax
$L77294:

; 1337 : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?GetAssocAt@?$CMap@IIKAAK@@IBEPAUCAssoc@1@IAAI@Z ENDP	; CMap<unsigned int,unsigned int,unsigned long,unsigned long &>::GetAssocAt
_TEXT	ENDS
;	COMDAT ?HashKey@@YGII@Z
_TEXT	SEGMENT
_key$ = 8
?HashKey@@YGII@Z PROC NEAR				; HashKey, COMDAT

; 127  : {

	push	ebp
	mov	ebp, esp

; 128  : 	// default identity hash - works for most primitive values
; 129  : 	return ((UINT)(void*)(DWORD)key) >> 4;

	mov	eax, DWORD PTR _key$[ebp]
	shr	eax, 4

; 130  : }

	pop	ebp
	ret	4
?HashKey@@YGII@Z ENDP					; HashKey
_TEXT	ENDS
;	COMDAT ?CompareElements@@YGHPBI0@Z
_TEXT	SEGMENT
_pElement1$ = 8
_pElement2$ = 12
?CompareElements@@YGHPBI0@Z PROC NEAR			; CompareElements, COMDAT

; 118  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L77314:

; 119  : 	ASSERT(AfxIsValidAddress(pElement1, sizeof(TYPE), FALSE));

	push	0
	push	4
	mov	eax, DWORD PTR _pElement1$[ebp]
	push	eax
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77318
	push	119					; 00000077H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77318
	int	3
$L77318:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L77314
$L77316:

; 120  : 	ASSERT(AfxIsValidAddress(pElement2, sizeof(ARG_TYPE), FALSE));

	push	0
	push	4
	mov	edx, DWORD PTR _pElement2$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77323
	push	120					; 00000078H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77323
	int	3
$L77323:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L77316

; 121  : 
; 122  : 	return *pElement1 == *pElement2;

	mov	ecx, DWORD PTR _pElement1$[ebp]
	mov	edx, DWORD PTR _pElement2$[ebp]
	mov	eax, DWORD PTR [ecx]
	xor	ecx, ecx
	cmp	eax, DWORD PTR [edx]
	sete	cl
	mov	eax, ecx

; 123  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?CompareElements@@YGHPBI0@Z ENDP			; CompareElements
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAIH@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
?DestructElements@@YGXPAIH@Z PROC NEAR			; DestructElements, COMDAT

; 67   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L77472:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77476
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77476
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77476
	int	3
$L77476:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77472

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L77477
$L77478:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 4
	mov	DWORD PTR _pElements$[ebp], eax
$L77477:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L77479

; 73   : 		pElements->~TYPE();

	jmp	SHORT $L77478
$L77479:

; 74   : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?DestructElements@@YGXPAIH@Z ENDP			; DestructElements
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAKH@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
?DestructElements@@YGXPAKH@Z PROC NEAR			; DestructElements, COMDAT

; 67   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L77481:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77485
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77485
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77485
	int	3
$L77485:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77481

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L77486
$L77487:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 4
	mov	DWORD PTR _pElements$[ebp], eax
$L77486:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L77488

; 73   : 		pElements->~TYPE();

	jmp	SHORT $L77487
$L77488:

; 74   : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?DestructElements@@YGXPAKH@Z ENDP			; DestructElements
_TEXT	ENDS
PUBLIC	??2@YAPAXIPAX@Z					; operator new
;	COMDAT ?ConstructElements@@YGXPAKH@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
$T78188 = -4
?ConstructElements@@YGXPAKH@Z PROC NEAR			; ConstructElements, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L77490:

; 54   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77494
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77494
	push	55					; 00000037H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77494
	int	3
$L77494:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77490

; 55   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 56   : 
; 57   : 	// first do bit-wise zero initialization
; 58   : 	memset((void*)pElements, 0, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	push	0
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	_memset
	add	esp, 12					; 0000000cH

; 59   : 
; 60   : 	// then call the constructor(s)
; 61   : 	for (; nCount--; pElements++)

	jmp	SHORT $L77497
$L77498:
	mov	edx, DWORD PTR _pElements$[ebp]
	add	edx, 4
	mov	DWORD PTR _pElements$[ebp], edx
$L77497:
	mov	eax, DWORD PTR _nCount$[ebp]
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
	test	eax, eax
	je	SHORT $L77499

; 62   : 		::new((void*)pElements) TYPE;

	mov	edx, DWORD PTR _pElements$[ebp]
	push	edx
	push	4
	call	??2@YAPAXIPAX@Z				; operator new
	add	esp, 8
	mov	DWORD PTR $T78188[ebp], eax
	jmp	SHORT $L77498
$L77499:

; 63   : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?ConstructElements@@YGXPAKH@Z ENDP			; ConstructElements
_TEXT	ENDS
;	COMDAT ??2@YAPAXIPAX@Z
_TEXT	SEGMENT
__P$ = 12
??2@YAPAXIPAX@Z PROC NEAR				; operator new, COMDAT

; 76   :         {return (_P); }

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR __P$[ebp]
	pop	ebp
	ret	0
??2@YAPAXIPAX@Z ENDP					; operator new
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAIH@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
$T78193 = -4
?ConstructElements@@YGXPAIH@Z PROC NEAR			; ConstructElements, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L77504:

; 54   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77508
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77508
	push	55					; 00000037H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77508
	int	3
$L77508:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77504

; 55   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 56   : 
; 57   : 	// first do bit-wise zero initialization
; 58   : 	memset((void*)pElements, 0, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	push	0
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	_memset
	add	esp, 12					; 0000000cH

; 59   : 
; 60   : 	// then call the constructor(s)
; 61   : 	for (; nCount--; pElements++)

	jmp	SHORT $L77511
$L77512:
	mov	edx, DWORD PTR _pElements$[ebp]
	add	edx, 4
	mov	DWORD PTR _pElements$[ebp], edx
$L77511:
	mov	eax, DWORD PTR _nCount$[ebp]
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
	test	eax, eax
	je	SHORT $L77513

; 62   : 		::new((void*)pElements) TYPE;

	mov	edx, DWORD PTR _pElements$[ebp]
	push	edx
	push	4
	call	??2@YAPAXIPAX@Z				; operator new
	add	esp, 8
	mov	DWORD PTR $T78193[ebp], eax
	jmp	SHORT $L77512
$L77513:

; 63   : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?ConstructElements@@YGXPAIH@Z ENDP			; ConstructElements
_TEXT	ENDS
END

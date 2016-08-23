	TITLE	D:\_CODE\Shared\MenuIconMgr.cpp
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
;	COMDAT ??_C@_0CF@GNII@D?3?2_CODE?2Shared?2?4?4?2shared?2Subcla@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0BP@DDHI@D?3?2_CODE?2Shared?2graphicsmisc?4h?$AA@
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
;	COMDAT ??BCOSVersion@@QAE?AW4OSVERSION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CMenuIconMgr@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCMenuIconMgr@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CMenuIconMgr@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ImageMap@CMenuIconMgr@@AAEAAV?$CMap@IIPAUHICON__@@PAU1@@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ImageMap@CMenuIconMgr@@ABEABV?$CMap@IIPAUHICON__@@PAU1@@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Initialize@CMenuIconMgr@@QAEHPAVCWnd@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Release@CMenuIconMgr@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ClearImages@CMenuIconMgr@@IAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ClearImages@CMenuIconMgr@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CleanupBitmaps@CMenuIconMgr@@AAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddImages@CMenuIconMgr@@QAEHABVCToolBar@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@ABVCImageList@@PBV3@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@IHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@ABVCString@@HK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddImages@CMenuIconMgr@@IAEHABVCUIntArray@@AAVCBitmap@@1HK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HasImageID@CMenuIconMgr@@QBEHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ChangeImageID@CMenuIconMgr@@QAEHII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadItemIcon@CMenuIconMgr@@IBEPAUHICON__@@IH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadItemImage@CMenuIconMgr@@IAEPAXIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddImage@CMenuIconMgr@@QAEHIABVCImageList@@HH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DeleteImage@CMenuIconMgr@@QAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?WindowProc@CMenuIconMgr@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnInitMenuPopup@CMenuIconMgr@@IAEXPAVCMenu@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnDrawItem@CMenuIconMgr@@IAEHHPAUtagDRAWITEMSTRUCT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnMeasureItem@CMenuIconMgr@@IAEHHPAUtagMEASUREITEMSTRUCT@@@Z
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
;	COMDAT ??0?$CMap@IIPAUHICON__@@PAU1@@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lookup@?$CMap@IIPAUHICON__@@PAU1@@@QBEHIAAPAUHICON__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CMap@IIPAUHICON__@@PAU1@@@QAEAAPAUHICON__@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveKey@?$CMap@IIPAUHICON__@@PAU1@@@QAEHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@IIPAUHICON__@@PAU1@@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetStartPosition@?$CMap@IIPAUHICON__@@PAU1@@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNextAssoc@?$CMap@IIPAUHICON__@@PAU1@@@QBEXAAPAU__POSITION@@AAIAAPAUHICON__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CMap@IIPAUHICON__@@PAU1@@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CMap@IIPAUHICON__@@PAU1@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CMap@IIPAUHICON__@@PAU1@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CMap@IIPAUHICON__@@PAU1@@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lookup@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEHPAUHICON__@@AAPAUHBITMAP__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEAAPAUHBITMAP__@@PAUHICON__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetStartPosition@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNextAssoc@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEXAAPAU__POSITION@@AAPAUHICON__@@AAPAUHBITMAP__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@IIPAUHICON__@@PAU1@@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@IIPAUHICON__@@PAU1@@@QAEXIPAUHICON__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InitHashTable@?$CMap@IIPAUHICON__@@PAU1@@@QAEXIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewAssoc@?$CMap@IIPAUHICON__@@PAU1@@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FreeAssoc@?$CMap@IIPAUHICON__@@PAU1@@@IAEXPAUCAssoc@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAssocAt@?$CMap@IIPAUHICON__@@PAU1@@@IBEPAUCAssoc@1@IAAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HashKey@@YGII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CompareElements@@YGHPBI0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAPAUHICON__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAPAUHICON__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBQAUHICON__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXPAUHICON__@@PAUHBITMAP__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InitHashTable@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewAssoc@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAssocAt@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@IBEPAUCAssoc@1@PAUHICON__@@AAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAPAUHBITMAP__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAPAUHBITMAP__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBQAUHBITMAP__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAPAUHICON__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAPAUHBITMAP__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HashKey@@YGIPAUHICON__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CompareElements@@YGHPBQAUHICON__@@0@Z
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
;	COMDAT ??_7?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CMap@IIPAUHICON__@@PAU1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CMenuIconMgr@@6B@
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
_DATA	ENDS
PUBLIC	??0CMenuIconMgr@@QAE@XZ				; CMenuIconMgr::CMenuIconMgr
PUBLIC	??0?$CMap@IIPAUHICON__@@PAU1@@@QAE@H@Z		; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>
PUBLIC	?IsValid@CSubclassWnd@@UBEHXZ			; CSubclassWnd::IsValid
PUBLIC	??1?$CMap@IIPAUHICON__@@PAU1@@@UAE@XZ		; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::~CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>
PUBLIC	?PreDetachWindow@CSubclassWnd@@MAEXXZ		; CSubclassWnd::PreDetachWindow
PUBLIC	?PostDetachWindow@CSubclassWnd@@MAEXXZ		; CSubclassWnd::PostDetachWindow
PUBLIC	?WindowProc@CMenuIconMgr@@MAEJPAUHWND__@@IIJ@Z	; CMenuIconMgr::WindowProc
PUBLIC	?IsHooked@CSubclassWnd@@MBEHXZ			; CSubclassWnd::IsHooked
PUBLIC	?IsValidHook@CSubclassWnd@@MBEHXZ		; CSubclassWnd::IsValidHook
PUBLIC	?Redraw@CSubclassWnd@@MBEXXZ			; CSubclassWnd::Redraw
PUBLIC	??0?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAE@H@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>
PUBLIC	??1?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAE@XZ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::~CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>
PUBLIC	??_7CMenuIconMgr@@6B@				; CMenuIconMgr::`vftable'
PUBLIC	??_GCMenuIconMgr@@UAEPAXI@Z			; CMenuIconMgr::`scalar deleting destructor'
PUBLIC	??_ECMenuIconMgr@@UAEPAXI@Z			; CMenuIconMgr::`vector deleting destructor'
PUBLIC	??BCOSVersion@@QAE?AW4OSVERSION@@XZ		; COSVersion::operator enum OSVERSION
EXTRN	??0CSubclassWnd@@QAE@XZ:NEAR			; CSubclassWnd::CSubclassWnd
EXTRN	??1CSubclassWnd@@UAE@XZ:NEAR			; CSubclassWnd::~CSubclassWnd
EXTRN	?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z:NEAR ; CSubclassWnd::HookWindow
EXTRN	?PostMessageW@CSubclassWnd@@MBEHIIJ@Z:NEAR	; CSubclassWnd::PostMessageW
EXTRN	?SendMessageW@CSubclassWnd@@MBEHIIJ@Z:NEAR	; CSubclassWnd::SendMessageW
EXTRN	??0COSVersion@@QAE@XZ:NEAR			; COSVersion::COSVersion
EXTRN	??1COSVersion@@UAE@XZ:NEAR			; COSVersion::~COSVersion
EXTRN	__except_list:DWORD
EXTRN	__chkesp:NEAR
EXTRN	___CxxFrameHandler:NEAR
;	COMDAT ??_7CMenuIconMgr@@6B@
; File D:\_CODE\Shared\MenuIconMgr.cpp
CONST	SEGMENT
??_7CMenuIconMgr@@6B@ DD FLAT:??_ECMenuIconMgr@@UAEPAXI@Z ; CMenuIconMgr::`vftable'
	DD	FLAT:?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z
	DD	FLAT:?IsValid@CSubclassWnd@@UBEHXZ
	DD	FLAT:?PreDetachWindow@CSubclassWnd@@MAEXXZ
	DD	FLAT:?PostDetachWindow@CSubclassWnd@@MAEXXZ
	DD	FLAT:?IsHooked@CSubclassWnd@@MBEHXZ
	DD	FLAT:?IsValidHook@CSubclassWnd@@MBEHXZ
	DD	FLAT:?Redraw@CSubclassWnd@@MBEXXZ
	DD	FLAT:?PostMessageW@CSubclassWnd@@MBEHIIJ@Z
	DD	FLAT:?SendMessageW@CSubclassWnd@@MBEHIIJ@Z
	DD	FLAT:?WindowProc@CMenuIconMgr@@MAEJPAUHWND__@@IIJ@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77596	DD	019930520H
	DD	05H
	DD	FLAT:$T77600
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77600	DD	0ffffffffH
	DD	FLAT:$L77588
	DD	00H
	DD	FLAT:$L77589
	DD	01H
	DD	FLAT:$L77590
	DD	02H
	DD	FLAT:$L77591
	DD	03H
	DD	FLAT:$L77592
xdata$x	ENDS
;	COMDAT ??0CMenuIconMgr@@QAE@XZ
_TEXT	SEGMENT
_this$ = -20
$T77587 = -16
__$EHRec$ = -12
??0CMenuIconMgr@@QAE@XZ PROC NEAR			; CMenuIconMgr::CMenuIconMgr, COMDAT

; 25   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77597
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CSubclassWnd@@QAE@XZ			; CSubclassWnd::CSubclassWnd
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	push	10					; 0000000aH
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	??0?$CMap@IIPAUHICON__@@PAU1@@@QAE@H@Z	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	push	10					; 0000000aH
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??0?$CMap@IIPAUHICON__@@PAU1@@@QAE@H@Z	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	push	10					; 0000000aH
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 88					; 00000058H
	call	??0?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAE@H@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CMenuIconMgr@@6B@ ; CMenuIconMgr::`vftable'

; 26   : 	m_bVistaPlus = (COSVersion() > OSV_XP);

	lea	ecx, DWORD PTR $T77587[ebp]
	call	??0COSVersion@@QAE@XZ			; COSVersion::COSVersion
	mov	DWORD PTR -24+[ebp], eax
	mov	ecx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	ecx, DWORD PTR -28+[ebp]
	call	??BCOSVersion@@QAE?AW4OSVERSION@@XZ	; COSVersion::operator enum OSVERSION
	xor	edx, edx
	cmp	eax, 5
	setg	dl
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+116], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR $T77587[ebp]
	call	??1COSVersion@@UAE@XZ			; COSVersion::~COSVersion

; 27   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
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
$L77588:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CSubclassWnd@@UAE@XZ			; CSubclassWnd::~CSubclassWnd
	ret	0
$L77589:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	??1?$CMap@IIPAUHICON__@@PAU1@@@UAE@XZ	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::~CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>
	ret	0
$L77590:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??1?$CMap@IIPAUHICON__@@PAU1@@@UAE@XZ	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::~CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>
	ret	0
$L77591:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 88					; 00000058H
	call	??1?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAE@XZ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::~CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>
	ret	0
$L77592:
	lea	ecx, DWORD PTR $T77587[ebp]
	call	??1COSVersion@@UAE@XZ			; COSVersion::~COSVersion
	ret	0
$L77597:
	mov	eax, OFFSET FLAT:$T77596
	jmp	___CxxFrameHandler
text$x	ENDS
??0CMenuIconMgr@@QAE@XZ ENDP				; CMenuIconMgr::CMenuIconMgr
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
EXTRN	?GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ:NEAR ; COSVersion::GetOSVersion
;	COMDAT ??BCOSVersion@@QAE?AW4OSVERSION@@XZ
_TEXT	SEGMENT
_this$ = -4
??BCOSVersion@@QAE?AW4OSVERSION@@XZ PROC NEAR		; COSVersion::operator enum OSVERSION, COMDAT

; 35   : 	operator OSVERSION() { return GetOSVersion(); }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ ; COSVersion::GetOSVersion
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??BCOSVersion@@QAE?AW4OSVERSION@@XZ ENDP		; COSVersion::operator enum OSVERSION
_TEXT	ENDS
PUBLIC	??1CMenuIconMgr@@UAE@XZ				; CMenuIconMgr::~CMenuIconMgr
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCMenuIconMgr@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCMenuIconMgr@@UAEPAXI@Z PROC NEAR			; CMenuIconMgr::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CMenuIconMgr@@UAE@XZ			; CMenuIconMgr::~CMenuIconMgr
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L76358
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L76358:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCMenuIconMgr@@UAEPAXI@Z ENDP			; CMenuIconMgr::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	?ClearImages@CMenuIconMgr@@QAEXXZ		; CMenuIconMgr::ClearImages
;	COMDAT xdata$x
; File D:\_CODE\Shared\MenuIconMgr.cpp
xdata$x	SEGMENT
$T77629	DD	019930520H
	DD	04H
	DD	FLAT:$T77631
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77631	DD	0ffffffffH
	DD	FLAT:$L77624
	DD	00H
	DD	FLAT:$L77625
	DD	01H
	DD	FLAT:$L77626
	DD	02H
	DD	FLAT:$L77627
xdata$x	ENDS
;	COMDAT ??1CMenuIconMgr@@UAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1CMenuIconMgr@@UAE@XZ PROC NEAR			; CMenuIconMgr::~CMenuIconMgr, COMDAT

; 30   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77630
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CMenuIconMgr@@6B@ ; CMenuIconMgr::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 3

; 31   : 	// cleanup icons
; 32   : 	ClearImages();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?ClearImages@CMenuIconMgr@@QAEXXZ	; CMenuIconMgr::ClearImages

; 33   : }

	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 88					; 00000058H
	call	??1?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAE@XZ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::~CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??1?$CMap@IIPAUHICON__@@PAU1@@@UAE@XZ	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::~CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	??1?$CMap@IIPAUHICON__@@PAU1@@@UAE@XZ	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::~CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>
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
$L77624:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CSubclassWnd@@UAE@XZ			; CSubclassWnd::~CSubclassWnd
	ret	0
$L77625:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	??1?$CMap@IIPAUHICON__@@PAU1@@@UAE@XZ	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::~CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>
	ret	0
$L77626:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??1?$CMap@IIPAUHICON__@@PAU1@@@UAE@XZ	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::~CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>
	ret	0
$L77627:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 88					; 00000058H
	call	??1?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAE@XZ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::~CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>
	ret	0
$L77630:
	mov	eax, OFFSET FLAT:$T77629
	jmp	___CxxFrameHandler
text$x	ENDS
??1CMenuIconMgr@@UAE@XZ ENDP				; CMenuIconMgr::~CMenuIconMgr
PUBLIC	?ImageMap@CMenuIconMgr@@AAEAAV?$CMap@IIPAUHICON__@@PAU1@@@H@Z ; CMenuIconMgr::ImageMap
;	COMDAT ?ImageMap@CMenuIconMgr@@AAEAAV?$CMap@IIPAUHICON__@@PAU1@@@H@Z
_TEXT	SEGMENT
_bNormal$ = 8
_this$ = -8
$T77636 = -4
?ImageMap@CMenuIconMgr@@AAEAAV?$CMap@IIPAUHICON__@@PAU1@@@H@Z PROC NEAR ; CMenuIconMgr::ImageMap, COMDAT

; 36   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 37   : 	return (bNormal ? m_mapID2Icon : m_mapID2DisabledIcon); 

	cmp	DWORD PTR _bNormal$[ebp], 0
	je	SHORT $L77637
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 32					; 00000020H
	mov	DWORD PTR -12+[ebp], eax
	jmp	SHORT $L77638
$L77637:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	mov	DWORD PTR -12+[ebp], ecx
$L77638:
	mov	edx, DWORD PTR -12+[ebp]
	mov	DWORD PTR $T77636[ebp], edx
	mov	eax, DWORD PTR $T77636[ebp]

; 38   : }

	mov	esp, ebp
	pop	ebp
	ret	4
?ImageMap@CMenuIconMgr@@AAEAAV?$CMap@IIPAUHICON__@@PAU1@@@H@Z ENDP ; CMenuIconMgr::ImageMap
_TEXT	ENDS
PUBLIC	?ImageMap@CMenuIconMgr@@ABEABV?$CMap@IIPAUHICON__@@PAU1@@@H@Z ; CMenuIconMgr::ImageMap
;	COMDAT ?ImageMap@CMenuIconMgr@@ABEABV?$CMap@IIPAUHICON__@@PAU1@@@H@Z
_TEXT	SEGMENT
_bNormal$ = 8
_this$ = -8
$T77641 = -4
?ImageMap@CMenuIconMgr@@ABEABV?$CMap@IIPAUHICON__@@PAU1@@@H@Z PROC NEAR ; CMenuIconMgr::ImageMap, COMDAT

; 41   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 42   : 	return (bNormal ? m_mapID2Icon : m_mapID2DisabledIcon); 

	cmp	DWORD PTR _bNormal$[ebp], 0
	je	SHORT $L77642
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 32					; 00000020H
	mov	DWORD PTR -12+[ebp], eax
	jmp	SHORT $L77643
$L77642:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	mov	DWORD PTR -12+[ebp], ecx
$L77643:
	mov	edx, DWORD PTR -12+[ebp]
	mov	DWORD PTR $T77641[ebp], edx
	mov	eax, DWORD PTR $T77641[ebp]

; 43   : }

	mov	esp, ebp
	pop	ebp
	ret	4
?ImageMap@CMenuIconMgr@@ABEABV?$CMap@IIPAUHICON__@@PAU1@@@H@Z ENDP ; CMenuIconMgr::ImageMap
_TEXT	ENDS
PUBLIC	?Initialize@CMenuIconMgr@@QAEHPAVCWnd@@@Z	; CMenuIconMgr::Initialize
EXTRN	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ:NEAR	; CWnd::GetSafeHwnd
;	COMDAT ?Initialize@CMenuIconMgr@@QAEHPAVCWnd@@@Z
_TEXT	SEGMENT
_pWnd$ = 8
_this$ = -4
?Initialize@CMenuIconMgr@@QAEHPAVCWnd@@@Z PROC NEAR	; CMenuIconMgr::Initialize, COMDAT

; 46   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 47   : 	if (!IsHooked())

	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+20]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76378

; 48   : 		return HookWindow(pWnd->GetSafeHwnd());

	mov	esi, esp
	push	0
	mov	ecx, DWORD PTR _pWnd$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L76377
$L76378:

; 49   : 	
; 50   : 	// else
; 51   : 	return TRUE;

	mov	eax, 1
$L76377:

; 52   : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Initialize@CMenuIconMgr@@QAEHPAVCWnd@@@Z ENDP		; CMenuIconMgr::Initialize
_TEXT	ENDS
PUBLIC	?Release@CMenuIconMgr@@QAEXXZ			; CMenuIconMgr::Release
;	COMDAT ?Release@CMenuIconMgr@@QAEXXZ
_TEXT	SEGMENT
_this$ = -4
?Release@CMenuIconMgr@@QAEXXZ PROC NEAR			; CMenuIconMgr::Release, COMDAT

; 55   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 56   : 	if (IsHooked())

	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+20]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L76382

; 58   : 		ClearImages();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?ClearImages@CMenuIconMgr@@QAEXXZ	; CMenuIconMgr::ClearImages

; 59   : 		CSubclassWnd::HookWindow(NULL);

	push	0
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z ; CSubclassWnd::HookWindow
$L76382:

; 61   : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Release@CMenuIconMgr@@QAEXXZ ENDP			; CMenuIconMgr::Release
_TEXT	ENDS
PUBLIC	?RemoveAll@?$CMap@IIPAUHICON__@@PAU1@@@QAEXXZ	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::RemoveAll
PUBLIC	?GetStartPosition@?$CMap@IIPAUHICON__@@PAU1@@@QBEPAU__POSITION@@XZ ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::GetStartPosition
PUBLIC	?GetNextAssoc@?$CMap@IIPAUHICON__@@PAU1@@@QBEXAAPAU__POSITION@@AAIAAPAUHICON__@@@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::GetNextAssoc
PUBLIC	?ClearImages@CMenuIconMgr@@IAEXH@Z		; CMenuIconMgr::ClearImages
EXTRN	__imp__DestroyIcon@4:NEAR
;	COMDAT ?ClearImages@CMenuIconMgr@@IAEXH@Z
_TEXT	SEGMENT
_bNormal$ = 8
_this$ = -16
_pos$ = -4
_nCmdID$76391 = -12
_hIcon$76392 = -8
?ClearImages@CMenuIconMgr@@IAEXH@Z PROC NEAR		; CMenuIconMgr::ClearImages, COMDAT

; 64   : {

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

; 65   : 	POSITION pos = ImageMap(bNormal).GetStartPosition();

	mov	eax, DWORD PTR _bNormal$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ImageMap@CMenuIconMgr@@AAEAAV?$CMap@IIPAUHICON__@@PAU1@@@H@Z ; CMenuIconMgr::ImageMap
	mov	ecx, eax
	call	?GetStartPosition@?$CMap@IIPAUHICON__@@PAU1@@@QBEPAU__POSITION@@XZ ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::GetStartPosition
	mov	DWORD PTR _pos$[ebp], eax
$L76389:

; 66   : 	
; 67   : 	while (pos)

	cmp	DWORD PTR _pos$[ebp], 0
	je	SHORT $L76390

; 69   : 		UINT nCmdID = 0;

	mov	DWORD PTR _nCmdID$76391[ebp], 0

; 70   : 		HICON hIcon = NULL;

	mov	DWORD PTR _hIcon$76392[ebp], 0

; 71   : 		
; 72   : 		ImageMap(bNormal).GetNextAssoc(pos, nCmdID, hIcon);

	lea	ecx, DWORD PTR _hIcon$76392[ebp]
	push	ecx
	lea	edx, DWORD PTR _nCmdID$76391[ebp]
	push	edx
	lea	eax, DWORD PTR _pos$[ebp]
	push	eax
	mov	ecx, DWORD PTR _bNormal$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ImageMap@CMenuIconMgr@@AAEAAV?$CMap@IIPAUHICON__@@PAU1@@@H@Z ; CMenuIconMgr::ImageMap
	mov	ecx, eax
	call	?GetNextAssoc@?$CMap@IIPAUHICON__@@PAU1@@@QBEXAAPAU__POSITION@@AAIAAPAUHICON__@@@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::GetNextAssoc

; 73   : 		::DestroyIcon(hIcon);

	mov	esi, esp
	mov	edx, DWORD PTR _hIcon$76392[ebp]
	push	edx
	call	DWORD PTR __imp__DestroyIcon@4
	cmp	esi, esp
	call	__chkesp

; 74   : 	}

	jmp	SHORT $L76389
$L76390:

; 75   : 	
; 76   : 	ImageMap(bNormal).RemoveAll();

	mov	eax, DWORD PTR _bNormal$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ImageMap@CMenuIconMgr@@AAEAAV?$CMap@IIPAUHICON__@@PAU1@@@H@Z ; CMenuIconMgr::ImageMap
	mov	ecx, eax
	call	?RemoveAll@?$CMap@IIPAUHICON__@@PAU1@@@QAEXXZ ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::RemoveAll

; 77   : }

	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ClearImages@CMenuIconMgr@@IAEXH@Z ENDP			; CMenuIconMgr::ClearImages
_TEXT	ENDS
PUBLIC	?CleanupBitmaps@CMenuIconMgr@@AAEXXZ		; CMenuIconMgr::CleanupBitmaps
;	COMDAT ?ClearImages@CMenuIconMgr@@QAEXXZ
_TEXT	SEGMENT
_this$ = -4
?ClearImages@CMenuIconMgr@@QAEXXZ PROC NEAR		; CMenuIconMgr::ClearImages, COMDAT

; 80   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 81   : 	ClearImages(TRUE);

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ClearImages@CMenuIconMgr@@IAEXH@Z	; CMenuIconMgr::ClearImages

; 82   : 	ClearImages(FALSE);

	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ClearImages@CMenuIconMgr@@IAEXH@Z	; CMenuIconMgr::ClearImages

; 83   : 
; 84   : 	CleanupBitmaps();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?CleanupBitmaps@CMenuIconMgr@@AAEXXZ	; CMenuIconMgr::CleanupBitmaps

; 85   : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?ClearImages@CMenuIconMgr@@QAEXXZ ENDP			; CMenuIconMgr::ClearImages
_TEXT	ENDS
PUBLIC	?RemoveAll@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXXZ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::RemoveAll
PUBLIC	?GetStartPosition@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEPAU__POSITION@@XZ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::GetStartPosition
PUBLIC	?GetNextAssoc@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEXAAPAU__POSITION@@AAPAUHICON__@@AAPAUHBITMAP__@@@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::GetNextAssoc
EXTRN	__imp__DeleteObject@4:NEAR
;	COMDAT ?CleanupBitmaps@CMenuIconMgr@@AAEXXZ
_TEXT	SEGMENT
_this$ = -16
_pos$ = -4
_hIcon$76403 = -8
_hBmp$76404 = -12
?CleanupBitmaps@CMenuIconMgr@@AAEXXZ PROC NEAR		; CMenuIconMgr::CleanupBitmaps, COMDAT

; 88   : {

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

; 89   : 	POSITION pos = m_mapIcon2Bmp.GetStartPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 88					; 00000058H
	call	?GetStartPosition@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEPAU__POSITION@@XZ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::GetStartPosition
	mov	DWORD PTR _pos$[ebp], eax
$L76401:

; 90   : 	
; 91   : 	while (pos)

	cmp	DWORD PTR _pos$[ebp], 0
	je	SHORT $L76402

; 93   : 		HICON hIcon = NULL;

	mov	DWORD PTR _hIcon$76403[ebp], 0

; 94   : 		HBITMAP hBmp = NULL;

	mov	DWORD PTR _hBmp$76404[ebp], 0

; 95   : 		
; 96   : 		m_mapIcon2Bmp.GetNextAssoc(pos, hIcon, hBmp);

	lea	eax, DWORD PTR _hBmp$76404[ebp]
	push	eax
	lea	ecx, DWORD PTR _hIcon$76403[ebp]
	push	ecx
	lea	edx, DWORD PTR _pos$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 88					; 00000058H
	call	?GetNextAssoc@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEXAAPAU__POSITION@@AAPAUHICON__@@AAPAUHBITMAP__@@@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::GetNextAssoc

; 97   : 		::DeleteObject(hBmp);

	mov	esi, esp
	mov	eax, DWORD PTR _hBmp$76404[ebp]
	push	eax
	call	DWORD PTR __imp__DeleteObject@4
	cmp	esi, esp
	call	__chkesp

; 98   : 	}

	jmp	SHORT $L76401
$L76402:

; 99   : 	
; 100  : 	m_mapIcon2Bmp.RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 88					; 00000058H
	call	?RemoveAll@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXXZ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::RemoveAll

; 101  : }

	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?CleanupBitmaps@CMenuIconMgr@@AAEXXZ ENDP		; CMenuIconMgr::CleanupBitmaps
_TEXT	ENDS
PUBLIC	?AddImages@CMenuIconMgr@@QAEHABVCToolBar@@@Z	; CMenuIconMgr::AddImages
PUBLIC	??A?$CMap@IIPAUHICON__@@PAU1@@@QAEAAPAUHICON__@@I@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::operator[]
EXTRN	?GetItemID@CToolBar@@QBEIH@Z:NEAR		; CToolBar::GetItemID
EXTRN	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ:NEAR ; CToolBar::GetToolBarCtrl
EXTRN	?ExtractIconW@CImageList@@QAEPAUHICON__@@H@Z:NEAR ; CImageList::ExtractIconW
EXTRN	?GetButtonCount@CToolBarCtrl@@QBEHXZ:NEAR	; CToolBarCtrl::GetButtonCount
EXTRN	?GetDisabledImageList@CToolBarCtrl@@QBEPAVCImageList@@XZ:NEAR ; CToolBarCtrl::GetDisabledImageList
EXTRN	?GetImageList@CToolBarCtrl@@QBEPAVCImageList@@XZ:NEAR ; CToolBarCtrl::GetImageList
;	COMDAT ?AddImages@CMenuIconMgr@@QAEHABVCToolBar@@@Z
_TEXT	SEGMENT
_toolbar$ = 8
_this$ = -32
_tbc$ = -12
_nBtnCount$ = -24
_nImage$ = -16
_pIL$ = -20
_pILDis$ = -4
_nBtn$ = -8
_nCmdID$76418 = -28
?AddImages@CMenuIconMgr@@QAEHABVCToolBar@@@Z PROC NEAR	; CMenuIconMgr::AddImages, COMDAT

; 104  : {

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
	mov	DWORD PTR _this$[ebp], ecx

; 105  : 	// iterate the non-separator items extracting their images 
; 106  : 	// from the imagelist
; 107  : 	const CToolBarCtrl& tbc = toolbar.GetToolBarCtrl();

	mov	ecx, DWORD PTR _toolbar$[ebp]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	DWORD PTR _tbc$[ebp], eax

; 108  : 	int nBtnCount = tbc.GetButtonCount(), nImage = 0;

	mov	ecx, DWORD PTR _tbc$[ebp]
	call	?GetButtonCount@CToolBarCtrl@@QBEHXZ	; CToolBarCtrl::GetButtonCount
	mov	DWORD PTR _nBtnCount$[ebp], eax
	mov	DWORD PTR _nImage$[ebp], 0

; 109  : 
; 110  : 	CImageList* pIL = tbc.GetImageList();

	mov	ecx, DWORD PTR _tbc$[ebp]
	call	?GetImageList@CToolBarCtrl@@QBEPAVCImageList@@XZ ; CToolBarCtrl::GetImageList
	mov	DWORD PTR _pIL$[ebp], eax

; 111  : 	CImageList* pILDis = tbc.GetDisabledImageList();

	mov	ecx, DWORD PTR _tbc$[ebp]
	call	?GetDisabledImageList@CToolBarCtrl@@QBEPAVCImageList@@XZ ; CToolBarCtrl::GetDisabledImageList
	mov	DWORD PTR _pILDis$[ebp], eax

; 112  : 	
; 113  : 	for (int nBtn = 0; nBtn < nBtnCount; nBtn++)

	mov	DWORD PTR _nBtn$[ebp], 0
	jmp	SHORT $L76415
$L76416:
	mov	eax, DWORD PTR _nBtn$[ebp]
	add	eax, 1
	mov	DWORD PTR _nBtn$[ebp], eax
$L76415:
	mov	ecx, DWORD PTR _nBtn$[ebp]
	cmp	ecx, DWORD PTR _nBtnCount$[ebp]
	jge	SHORT $L76417

; 115  : 		UINT nCmdID = toolbar.GetItemID(nBtn);

	mov	edx, DWORD PTR _nBtn$[ebp]
	push	edx
	mov	ecx, DWORD PTR _toolbar$[ebp]
	call	?GetItemID@CToolBar@@QBEIH@Z		; CToolBar::GetItemID
	mov	DWORD PTR _nCmdID$76418[ebp], eax

; 116  : 		
; 117  : 		if (nCmdID != ID_SEPARATOR)

	cmp	DWORD PTR _nCmdID$76418[ebp], 0
	je	SHORT $L76419

; 119  : 			m_mapID2Icon[nCmdID] = pIL->ExtractIcon(nImage);

	mov	eax, DWORD PTR _nImage$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pIL$[ebp]
	call	?ExtractIconW@CImageList@@QAEPAUHICON__@@H@Z ; CImageList::ExtractIconW
	mov	esi, eax
	mov	ecx, DWORD PTR _nCmdID$76418[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	??A?$CMap@IIPAUHICON__@@PAU1@@@QAEAAPAUHICON__@@I@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::operator[]
	mov	DWORD PTR [eax], esi

; 120  : 
; 121  : 			if (pILDis)

	cmp	DWORD PTR _pILDis$[ebp], 0
	je	SHORT $L76420

; 122  : 				m_mapID2DisabledIcon[nCmdID] = pILDis->ExtractIcon(nImage);

	mov	edx, DWORD PTR _nImage$[ebp]
	push	edx
	mov	ecx, DWORD PTR _pILDis$[ebp]
	call	?ExtractIconW@CImageList@@QAEPAUHICON__@@H@Z ; CImageList::ExtractIconW
	mov	esi, eax
	mov	eax, DWORD PTR _nCmdID$76418[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??A?$CMap@IIPAUHICON__@@PAU1@@@QAEAAPAUHICON__@@I@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::operator[]
	mov	DWORD PTR [eax], esi
$L76420:

; 123  : 
; 124  : 			nImage++;

	mov	ecx, DWORD PTR _nImage$[ebp]
	add	ecx, 1
	mov	DWORD PTR _nImage$[ebp], ecx
$L76419:

; 126  : 	}

	jmp	SHORT $L76416
$L76417:

; 127  : 	
; 128  : 	return nImage;

	mov	eax, DWORD PTR _nImage$[ebp]

; 129  : }

	pop	esi
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?AddImages@CMenuIconMgr@@QAEHABVCToolBar@@@Z ENDP	; CMenuIconMgr::AddImages
_TEXT	ENDS
PUBLIC	?SetImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z	; CMenuIconMgr::SetImage
PUBLIC	?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@ABVCImageList@@PBV3@@Z ; CMenuIconMgr::AddImages
EXTRN	?GetSize@CUIntArray@@QBEHXZ:NEAR		; CUIntArray::GetSize
EXTRN	??ACUIntArray@@QBEIH@Z:NEAR			; CUIntArray::operator[]
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	?GetImageCount@CImageList@@QBEHXZ:NEAR		; CImageList::GetImageCount
;	COMDAT data
; File D:\_CODE\Shared\MenuIconMgr.cpp
data	SEGMENT
$SG76431 DB	'D:\_CODE\Shared\MenuIconMgr.cpp', 00H
$SG76436 DB	'D:\_CODE\Shared\MenuIconMgr.cpp', 00H
data	ENDS
;	COMDAT ?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@ABVCImageList@@PBV3@@Z
_TEXT	SEGMENT
_aCmdIDs$ = 8
_il$ = 12
_pILDisabled$ = 16
_this$ = -20
_nBtnCount$ = -16
_pIL$ = -12
_pILDis$ = -4
_nBtn$ = -8
?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@ABVCImageList@@PBV3@@Z PROC NEAR ; CMenuIconMgr::AddImages, COMDAT

; 132  : {

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
$L76427:

; 133  : 	ASSERT (aCmdIDs.GetSize() == il.GetImageCount());

	mov	ecx, DWORD PTR _aCmdIDs$[ebp]
	call	?GetSize@CUIntArray@@QBEHXZ		; CUIntArray::GetSize
	mov	esi, eax
	mov	ecx, DWORD PTR _il$[ebp]
	call	?GetImageCount@CImageList@@QBEHXZ	; CImageList::GetImageCount
	cmp	esi, eax
	je	SHORT $L76430
	push	133					; 00000085H
	push	OFFSET FLAT:$SG76431
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76430
	int	3
$L76430:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76427
$L76429:

; 134  : 	ASSERT (!pILDisabled || (aCmdIDs.GetSize() == pILDisabled->GetImageCount()) || pILDisabled->GetImageCount() == 0);

	cmp	DWORD PTR _pILDisabled$[ebp], 0
	je	SHORT $L76435
	mov	ecx, DWORD PTR _aCmdIDs$[ebp]
	call	?GetSize@CUIntArray@@QBEHXZ		; CUIntArray::GetSize
	mov	esi, eax
	mov	ecx, DWORD PTR _pILDisabled$[ebp]
	call	?GetImageCount@CImageList@@QBEHXZ	; CImageList::GetImageCount
	cmp	esi, eax
	je	SHORT $L76435
	mov	ecx, DWORD PTR _pILDisabled$[ebp]
	call	?GetImageCount@CImageList@@QBEHXZ	; CImageList::GetImageCount
	test	eax, eax
	je	SHORT $L76435
	push	134					; 00000086H
	push	OFFSET FLAT:$SG76436
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76435
	int	3
$L76435:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76429

; 135  : 	
; 136  : 	if (aCmdIDs.GetSize() != il.GetImageCount())

	mov	ecx, DWORD PTR _aCmdIDs$[ebp]
	call	?GetSize@CUIntArray@@QBEHXZ		; CUIntArray::GetSize
	mov	esi, eax
	mov	ecx, DWORD PTR _il$[ebp]
	call	?GetImageCount@CImageList@@QBEHXZ	; CImageList::GetImageCount
	cmp	esi, eax
	je	SHORT $L76437

; 137  : 		return 0;

	xor	eax, eax
	jmp	$L76426
$L76437:

; 138  : 	
; 139  : 	if (pILDisabled && pILDisabled->GetImageCount() && aCmdIDs.GetSize() != pILDisabled->GetImageCount())

	cmp	DWORD PTR _pILDisabled$[ebp], 0
	je	SHORT $L76438
	mov	ecx, DWORD PTR _pILDisabled$[ebp]
	call	?GetImageCount@CImageList@@QBEHXZ	; CImageList::GetImageCount
	test	eax, eax
	je	SHORT $L76438
	mov	ecx, DWORD PTR _aCmdIDs$[ebp]
	call	?GetSize@CUIntArray@@QBEHXZ		; CUIntArray::GetSize
	mov	esi, eax
	mov	ecx, DWORD PTR _pILDisabled$[ebp]
	call	?GetImageCount@CImageList@@QBEHXZ	; CImageList::GetImageCount
	cmp	esi, eax
	je	SHORT $L76438

; 140  : 		return 0;

	xor	eax, eax
	jmp	$L76426
$L76438:

; 141  : 	
; 142  : 	int nBtnCount = aCmdIDs.GetSize();

	mov	ecx, DWORD PTR _aCmdIDs$[ebp]
	call	?GetSize@CUIntArray@@QBEHXZ		; CUIntArray::GetSize
	mov	DWORD PTR _nBtnCount$[ebp], eax

; 143  : 
; 144  : 	CImageList* pIL = const_cast<CImageList*>(&il);

	mov	edx, DWORD PTR _il$[ebp]
	mov	DWORD PTR _pIL$[ebp], edx

; 145  : 	CImageList* pILDis = const_cast<CImageList*>(pILDisabled);

	mov	eax, DWORD PTR _pILDisabled$[ebp]
	mov	DWORD PTR _pILDis$[ebp], eax

; 146  : 	
; 147  : 	for (int nBtn = 0; nBtn < nBtnCount; nBtn++)

	mov	DWORD PTR _nBtn$[ebp], 0
	jmp	SHORT $L76445
$L76446:
	mov	ecx, DWORD PTR _nBtn$[ebp]
	add	ecx, 1
	mov	DWORD PTR _nBtn$[ebp], ecx
$L76445:
	mov	edx, DWORD PTR _nBtn$[ebp]
	cmp	edx, DWORD PTR _nBtnCount$[ebp]
	jge	SHORT $L76447

; 149  : 		SetImage(aCmdIDs[nBtn], pIL->ExtractIcon(nBtn), TRUE);

	push	1
	mov	eax, DWORD PTR _nBtn$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pIL$[ebp]
	call	?ExtractIconW@CImageList@@QAEPAUHICON__@@H@Z ; CImageList::ExtractIconW
	push	eax
	mov	ecx, DWORD PTR _nBtn$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _aCmdIDs$[ebp]
	call	??ACUIntArray@@QBEIH@Z			; CUIntArray::operator[]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z ; CMenuIconMgr::SetImage

; 150  : 
; 151  : 		if (pILDis && pILDis->GetImageCount())

	cmp	DWORD PTR _pILDis$[ebp], 0
	je	SHORT $L76448
	mov	ecx, DWORD PTR _pILDis$[ebp]
	call	?GetImageCount@CImageList@@QBEHXZ	; CImageList::GetImageCount
	test	eax, eax
	je	SHORT $L76448

; 152  : 			SetImage(aCmdIDs[nBtn], pILDis->ExtractIcon(nBtn), FALSE);

	push	0
	mov	edx, DWORD PTR _nBtn$[ebp]
	push	edx
	mov	ecx, DWORD PTR _pILDis$[ebp]
	call	?ExtractIconW@CImageList@@QAEPAUHICON__@@H@Z ; CImageList::ExtractIconW
	push	eax
	mov	eax, DWORD PTR _nBtn$[ebp]
	push	eax
	mov	ecx, DWORD PTR _aCmdIDs$[ebp]
	call	??ACUIntArray@@QBEIH@Z			; CUIntArray::operator[]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z ; CMenuIconMgr::SetImage
$L76448:

; 153  : 	}

	jmp	SHORT $L76446
$L76447:

; 154  : 	   
; 155  : 	return nBtnCount;

	mov	eax, DWORD PTR _nBtnCount$[ebp]
$L76426:

; 156  : 	
; 157  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@ABVCImageList@@PBV3@@Z ENDP ; CMenuIconMgr::AddImages
_TEXT	ENDS
PUBLIC	?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@IHK@Z ; CMenuIconMgr::AddImages
PUBLIC	?AddImages@CMenuIconMgr@@IAEHABVCUIntArray@@AAVCBitmap@@1HK@Z ; CMenuIconMgr::AddImages
EXTRN	??0CBitmap@@QAE@XZ:NEAR				; CBitmap::CBitmap
EXTRN	?LoadBitmapW@CBitmap@@QAEHI@Z:NEAR		; CBitmap::LoadBitmapW
EXTRN	??1CBitmap@@UAE@XZ:NEAR				; CBitmap::~CBitmap
EXTRN	?GrayScale@CEnBitmapEx@@SAHAAVCBitmap@@K@Z:NEAR	; CEnBitmapEx::GrayScale
;	COMDAT xdata$x
; File D:\_CODE\Shared\MenuIconMgr.cpp
xdata$x	SEGMENT
$T77665	DD	019930520H
	DD	02H
	DD	FLAT:$T77667
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77667	DD	0ffffffffH
	DD	FLAT:$L77662
	DD	00H
	DD	FLAT:$L77663
xdata$x	ENDS
;	COMDAT ?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@IHK@Z
_TEXT	SEGMENT
_aCmdIDs$ = 8
_nIDBitmap$ = 12
_nCx$ = 16
_crMask$ = 20
_this$ = -36
_bm$ = -28
_bmDis$ = -20
$T77661 = -32
__$EHRec$ = -12
?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@IHK@Z PROC NEAR ; CMenuIconMgr::AddImages, COMDAT

; 160  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77666
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
	mov	DWORD PTR _this$[ebp], ecx

; 161  : 	CBitmap bm, bmDis;

	lea	ecx, DWORD PTR _bm$[ebp]
	call	??0CBitmap@@QAE@XZ			; CBitmap::CBitmap
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _bmDis$[ebp]
	call	??0CBitmap@@QAE@XZ			; CBitmap::CBitmap
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 162  : 
; 163  : 	bm.LoadBitmap(nIDBitmap);

	mov	eax, DWORD PTR _nIDBitmap$[ebp]
	push	eax
	lea	ecx, DWORD PTR _bm$[ebp]
	call	?LoadBitmapW@CBitmap@@QAEHI@Z		; CBitmap::LoadBitmapW

; 164  : 
; 165  : 	if (bmDis.LoadBitmap(nIDBitmap))

	mov	ecx, DWORD PTR _nIDBitmap$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _bmDis$[ebp]
	call	?LoadBitmapW@CBitmap@@QAEHI@Z		; CBitmap::LoadBitmapW
	test	eax, eax
	je	SHORT $L76458

; 166  : 		CEnBitmapEx::GrayScale(bmDis, crMask);

	mov	edx, DWORD PTR _crMask$[ebp]
	push	edx
	lea	eax, DWORD PTR _bmDis$[ebp]
	push	eax
	call	?GrayScale@CEnBitmapEx@@SAHAAVCBitmap@@K@Z ; CEnBitmapEx::GrayScale
	add	esp, 8
$L76458:

; 167  : 
; 168  : 	return AddImages(aCmdIDs, bm, bmDis, nCx, crMask);

	mov	ecx, DWORD PTR _crMask$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nCx$[ebp]
	push	edx
	lea	eax, DWORD PTR _bmDis$[ebp]
	push	eax
	lea	ecx, DWORD PTR _bm$[ebp]
	push	ecx
	mov	edx, DWORD PTR _aCmdIDs$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddImages@CMenuIconMgr@@IAEHABVCUIntArray@@AAVCBitmap@@1HK@Z ; CMenuIconMgr::AddImages
	mov	DWORD PTR $T77661[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _bmDis$[ebp]
	call	??1CBitmap@@UAE@XZ			; CBitmap::~CBitmap
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _bm$[ebp]
	call	??1CBitmap@@UAE@XZ			; CBitmap::~CBitmap
	mov	eax, DWORD PTR $T77661[ebp]

; 169  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77662:
	lea	ecx, DWORD PTR _bm$[ebp]
	call	??1CBitmap@@UAE@XZ			; CBitmap::~CBitmap
	ret	0
$L77663:
	lea	ecx, DWORD PTR _bmDis$[ebp]
	call	??1CBitmap@@UAE@XZ			; CBitmap::~CBitmap
	ret	0
$L77666:
	mov	eax, OFFSET FLAT:$T77665
	jmp	___CxxFrameHandler
text$x	ENDS
?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@IHK@Z ENDP	; CMenuIconMgr::AddImages
PUBLIC	?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@ABVCString@@HK@Z ; CMenuIconMgr::AddImages
EXTRN	??0CEnBitmap@@QAE@K@Z:NEAR			; CEnBitmap::CEnBitmap
EXTRN	??1CEnBitmap@@UAE@XZ:NEAR			; CEnBitmap::~CEnBitmap
EXTRN	?LoadImageW@CEnBitmap@@QAEHPBGK@Z:NEAR		; CEnBitmap::LoadImageW
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
;	COMDAT xdata$x
; File D:\_CODE\Shared\MenuIconMgr.cpp
xdata$x	SEGMENT
$T77676	DD	019930520H
	DD	02H
	DD	FLAT:$T77678
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77678	DD	0ffffffffH
	DD	FLAT:$L77673
	DD	00H
	DD	FLAT:$L77674
xdata$x	ENDS
;	COMDAT ?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@ABVCString@@HK@Z
_TEXT	SEGMENT
_aCmdIDs$ = 8
_sImagePath$ = 12
_nCx$ = 16
_crMask$ = 20
_this$ = -60
_bm$ = -52
_bmDis$ = -32
$T77672 = -56
__$EHRec$ = -12
?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@ABVCString@@HK@Z PROC NEAR ; CMenuIconMgr::AddImages, COMDAT

; 172  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77677
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 48					; 00000030H
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-60]
	mov	ecx, 12					; 0000000cH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 173  : 	CEnBitmap bm, bmDis;

	push	16777215				; 00ffffffH
	lea	ecx, DWORD PTR _bm$[ebp]
	call	??0CEnBitmap@@QAE@K@Z			; CEnBitmap::CEnBitmap
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	push	16777215				; 00ffffffH
	lea	ecx, DWORD PTR _bmDis$[ebp]
	call	??0CEnBitmap@@QAE@K@Z			; CEnBitmap::CEnBitmap
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 174  : 
; 175  : 	bm.LoadImage(sImagePath, crMask);

	mov	eax, DWORD PTR _crMask$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sImagePath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _bm$[ebp]
	call	?LoadImageW@CEnBitmap@@QAEHPBGK@Z	; CEnBitmap::LoadImageW

; 176  : 
; 177  : 	if (bmDis.LoadImage(sImagePath, crMask))

	mov	ecx, DWORD PTR _crMask$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sImagePath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _bmDis$[ebp]
	call	?LoadImageW@CEnBitmap@@QAEHPBGK@Z	; CEnBitmap::LoadImageW
	test	eax, eax
	je	SHORT $L76469

; 178  : 		CEnBitmapEx::GrayScale(bmDis, crMask);

	mov	edx, DWORD PTR _crMask$[ebp]
	push	edx
	lea	eax, DWORD PTR _bmDis$[ebp]
	push	eax
	call	?GrayScale@CEnBitmapEx@@SAHAAVCBitmap@@K@Z ; CEnBitmapEx::GrayScale
	add	esp, 8
$L76469:

; 179  : 
; 180  : 	return AddImages(aCmdIDs, bm, bmDis, nCx, crMask);

	mov	ecx, DWORD PTR _crMask$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nCx$[ebp]
	push	edx
	lea	eax, DWORD PTR _bmDis$[ebp]
	push	eax
	lea	ecx, DWORD PTR _bm$[ebp]
	push	ecx
	mov	edx, DWORD PTR _aCmdIDs$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddImages@CMenuIconMgr@@IAEHABVCUIntArray@@AAVCBitmap@@1HK@Z ; CMenuIconMgr::AddImages
	mov	DWORD PTR $T77672[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _bmDis$[ebp]
	call	??1CEnBitmap@@UAE@XZ			; CEnBitmap::~CEnBitmap
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _bm$[ebp]
	call	??1CEnBitmap@@UAE@XZ			; CEnBitmap::~CEnBitmap
	mov	eax, DWORD PTR $T77672[ebp]

; 181  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 60					; 0000003cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77673:
	lea	ecx, DWORD PTR _bm$[ebp]
	call	??1CEnBitmap@@UAE@XZ			; CEnBitmap::~CEnBitmap
	ret	0
$L77674:
	lea	ecx, DWORD PTR _bmDis$[ebp]
	call	??1CEnBitmap@@UAE@XZ			; CEnBitmap::~CEnBitmap
	ret	0
$L77677:
	mov	eax, OFFSET FLAT:$T77676
	jmp	___CxxFrameHandler
text$x	ENDS
?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@ABVCString@@HK@Z ENDP ; CMenuIconMgr::AddImages
EXTRN	??0CImageList@@QAE@XZ:NEAR			; CImageList::CImageList
EXTRN	?Create@CImageList@@QAEHHHIHH@Z:NEAR		; CImageList::Create
EXTRN	?Add@CImageList@@QAEHPAVCBitmap@@K@Z:NEAR	; CImageList::Add
EXTRN	??1CImageList@@UAE@XZ:NEAR			; CImageList::~CImageList
EXTRN	?GetSafeHandle@CGdiObject@@QBEPAXXZ:NEAR	; CGdiObject::GetSafeHandle
;	COMDAT xdata$x
; File D:\_CODE\Shared\MenuIconMgr.cpp
xdata$x	SEGMENT
$T77687	DD	019930520H
	DD	02H
	DD	FLAT:$T77689
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77689	DD	0ffffffffH
	DD	FLAT:$L77684
	DD	00H
	DD	FLAT:$L77685
xdata$x	ENDS
;	COMDAT ?AddImages@CMenuIconMgr@@IAEHABVCUIntArray@@AAVCBitmap@@1HK@Z
_TEXT	SEGMENT
_aCmdIDs$ = 8
_bm$ = 12
_bmDis$ = 16
_nCx$ = 20
_crMask$ = 24
_this$ = -36
_il$76480 = -28
_ilDis$76481 = -20
$T77683 = -32
__$EHRec$ = -12
?AddImages@CMenuIconMgr@@IAEHABVCUIntArray@@AAVCBitmap@@1HK@Z PROC NEAR ; CMenuIconMgr::AddImages, COMDAT

; 184  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77688
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
	mov	DWORD PTR _this$[ebp], ecx

; 185  : 	if (bm.GetSafeHandle())

	mov	ecx, DWORD PTR _bm$[ebp]
	call	?GetSafeHandle@CGdiObject@@QBEPAXXZ	; CGdiObject::GetSafeHandle
	test	eax, eax
	je	$L76479

; 187  : 		CImageList il, ilDis;

	lea	ecx, DWORD PTR _il$76480[ebp]
	call	??0CImageList@@QAE@XZ			; CImageList::CImageList
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _ilDis$76481[ebp]
	call	??0CImageList@@QAE@XZ			; CImageList::CImageList
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 188  : 
; 189  : 		if (il.Create(nCx, 16, ILC_COLOR32 | ILC_MASK, 0, 1))

	push	1
	push	0
	push	33					; 00000021H
	push	16					; 00000010H
	mov	eax, DWORD PTR _nCx$[ebp]
	push	eax
	lea	ecx, DWORD PTR _il$76480[ebp]
	call	?Create@CImageList@@QAEHHHIHH@Z		; CImageList::Create
	test	eax, eax
	je	SHORT $L76482

; 190  : 			il.Add(&bm, crMask);

	mov	ecx, DWORD PTR _crMask$[ebp]
	push	ecx
	mov	edx, DWORD PTR _bm$[ebp]
	push	edx
	lea	ecx, DWORD PTR _il$76480[ebp]
	call	?Add@CImageList@@QAEHPAVCBitmap@@K@Z	; CImageList::Add
$L76482:

; 191  : 
; 192  : 		if (ilDis.Create(nCx, 16, ILC_COLOR32 | ILC_MASK, 0, 1))

	push	1
	push	0
	push	33					; 00000021H
	push	16					; 00000010H
	mov	eax, DWORD PTR _nCx$[ebp]
	push	eax
	lea	ecx, DWORD PTR _ilDis$76481[ebp]
	call	?Create@CImageList@@QAEHHHIHH@Z		; CImageList::Create
	test	eax, eax
	je	SHORT $L76483

; 193  : 			ilDis.Add(&bmDis, crMask);

	mov	ecx, DWORD PTR _crMask$[ebp]
	push	ecx
	mov	edx, DWORD PTR _bmDis$[ebp]
	push	edx
	lea	ecx, DWORD PTR _ilDis$76481[ebp]
	call	?Add@CImageList@@QAEHPAVCBitmap@@K@Z	; CImageList::Add
$L76483:

; 194  : 
; 195  : 		return AddImages(aCmdIDs, il, &ilDis);

	lea	eax, DWORD PTR _ilDis$76481[ebp]
	push	eax
	lea	ecx, DWORD PTR _il$76480[ebp]
	push	ecx
	mov	edx, DWORD PTR _aCmdIDs$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddImages@CMenuIconMgr@@QAEHABVCUIntArray@@ABVCImageList@@PBV3@@Z ; CMenuIconMgr::AddImages
	mov	DWORD PTR $T77683[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _ilDis$76481[ebp]
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _il$76480[ebp]
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	mov	eax, DWORD PTR $T77683[ebp]
	jmp	SHORT $L76478
$L76479:

; 197  : 	
; 198  : 	return 0;

	xor	eax, eax
$L76478:

; 199  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	20					; 00000014H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77684:
	lea	ecx, DWORD PTR _il$76480[ebp]
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	ret	0
$L77685:
	lea	ecx, DWORD PTR _ilDis$76481[ebp]
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	ret	0
$L77688:
	mov	eax, OFFSET FLAT:$T77687
	jmp	___CxxFrameHandler
text$x	ENDS
?AddImages@CMenuIconMgr@@IAEHABVCUIntArray@@AAVCBitmap@@1HK@Z ENDP ; CMenuIconMgr::AddImages
PUBLIC	?HasImageID@CMenuIconMgr@@QBEHI@Z		; CMenuIconMgr::HasImageID
PUBLIC	?LoadItemIcon@CMenuIconMgr@@IBEPAUHICON__@@IH@Z	; CMenuIconMgr::LoadItemIcon
;	COMDAT ?HasImageID@CMenuIconMgr@@QBEHI@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_this$ = -4
?HasImageID@CMenuIconMgr@@QBEHI@Z PROC NEAR		; CMenuIconMgr::HasImageID, COMDAT

; 202  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 203  : 	return (LoadItemIcon(nCmdID, TRUE) || LoadItemIcon(nCmdID, FALSE));

	push	1
	mov	eax, DWORD PTR _nCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadItemIcon@CMenuIconMgr@@IBEPAUHICON__@@IH@Z ; CMenuIconMgr::LoadItemIcon
	test	eax, eax
	jne	SHORT $L77694
	push	0
	mov	ecx, DWORD PTR _nCmdID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadItemIcon@CMenuIconMgr@@IBEPAUHICON__@@IH@Z ; CMenuIconMgr::LoadItemIcon
	test	eax, eax
	jne	SHORT $L77694
	mov	DWORD PTR -8+[ebp], 0
	jmp	SHORT $L77695
$L77694:
	mov	DWORD PTR -8+[ebp], 1
$L77695:
	mov	eax, DWORD PTR -8+[ebp]

; 204  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?HasImageID@CMenuIconMgr@@QBEHI@Z ENDP			; CMenuIconMgr::HasImageID
_TEXT	ENDS
PUBLIC	?ChangeImageID@CMenuIconMgr@@QAEHII@Z		; CMenuIconMgr::ChangeImageID
PUBLIC	?RemoveKey@?$CMap@IIPAUHICON__@@PAU1@@@QAEHI@Z	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::RemoveKey
;	COMDAT ?ChangeImageID@CMenuIconMgr@@QAEHII@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_nNewCmdID$ = 12
_this$ = -8
_hIcon$ = -4
?ChangeImageID@CMenuIconMgr@@QAEHII@Z PROC NEAR		; CMenuIconMgr::ChangeImageID, COMDAT

; 207  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 208  : 	if (!HasImageID(nCmdID))

	mov	eax, DWORD PTR _nCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?HasImageID@CMenuIconMgr@@QBEHI@Z	; CMenuIconMgr::HasImageID
	test	eax, eax
	jne	SHORT $L76494

; 209  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L76493
$L76494:

; 210  : 
; 211  : 	// normal
; 212  : 	HICON hIcon = LoadItemIcon(nCmdID, TRUE);

	push	1
	mov	ecx, DWORD PTR _nCmdID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadItemIcon@CMenuIconMgr@@IBEPAUHICON__@@IH@Z ; CMenuIconMgr::LoadItemIcon
	mov	DWORD PTR _hIcon$[ebp], eax

; 213  : 	
; 214  : 	if (hIcon)

	cmp	DWORD PTR _hIcon$[ebp], 0
	je	SHORT $L76496

; 216  : 		m_mapID2Icon.RemoveKey(nCmdID);

	mov	edx, DWORD PTR _nCmdID$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	?RemoveKey@?$CMap@IIPAUHICON__@@PAU1@@@QAEHI@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::RemoveKey

; 217  : 		m_mapID2Icon[nNewCmdID] = hIcon;

	mov	eax, DWORD PTR _nNewCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	??A?$CMap@IIPAUHICON__@@PAU1@@@QAEAAPAUHICON__@@I@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::operator[]
	mov	ecx, DWORD PTR _hIcon$[ebp]
	mov	DWORD PTR [eax], ecx
$L76496:

; 219  : 
; 220  : 	// disabled
; 221  : 	hIcon = LoadItemIcon(nCmdID, FALSE);

	push	0
	mov	edx, DWORD PTR _nCmdID$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadItemIcon@CMenuIconMgr@@IBEPAUHICON__@@IH@Z ; CMenuIconMgr::LoadItemIcon
	mov	DWORD PTR _hIcon$[ebp], eax

; 222  : 		
; 223  : 	if (hIcon)

	cmp	DWORD PTR _hIcon$[ebp], 0
	je	SHORT $L76497

; 225  : 		m_mapID2DisabledIcon.RemoveKey(nCmdID);

	mov	eax, DWORD PTR _nCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	?RemoveKey@?$CMap@IIPAUHICON__@@PAU1@@@QAEHI@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::RemoveKey

; 226  : 		m_mapID2DisabledIcon[nNewCmdID] = hIcon;

	mov	ecx, DWORD PTR _nNewCmdID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??A?$CMap@IIPAUHICON__@@PAU1@@@QAEAAPAUHICON__@@I@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::operator[]
	mov	edx, DWORD PTR _hIcon$[ebp]
	mov	DWORD PTR [eax], edx
$L76497:

; 228  : 
; 229  : 	return TRUE;

	mov	eax, 1
$L76493:

; 230  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?ChangeImageID@CMenuIconMgr@@QAEHII@Z ENDP		; CMenuIconMgr::ChangeImageID
_TEXT	ENDS
PUBLIC	?Lookup@?$CMap@IIPAUHICON__@@PAU1@@@QBEHIAAPAUHICON__@@@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::Lookup
;	COMDAT ?LoadItemIcon@CMenuIconMgr@@IBEPAUHICON__@@IH@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_bNormal$ = 12
_this$ = -8
_hIcon$ = -4
?LoadItemIcon@CMenuIconMgr@@IBEPAUHICON__@@IH@Z PROC NEAR ; CMenuIconMgr::LoadItemIcon, COMDAT

; 233  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 234  : 	HICON hIcon = NULL;

	mov	DWORD PTR _hIcon$[ebp], 0

; 235  : 	
; 236  : 	ImageMap(bNormal).Lookup(nCmdID, hIcon);

	lea	eax, DWORD PTR _hIcon$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nCmdID$[ebp]
	push	ecx
	mov	edx, DWORD PTR _bNormal$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ImageMap@CMenuIconMgr@@ABEABV?$CMap@IIPAUHICON__@@PAU1@@@H@Z ; CMenuIconMgr::ImageMap
	mov	ecx, eax
	call	?Lookup@?$CMap@IIPAUHICON__@@PAU1@@@QBEHIAAPAUHICON__@@@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::Lookup

; 237  : 
; 238  : 	// fallback for disabled icon
; 239  : 	if (hIcon == NULL && !bNormal)

	cmp	DWORD PTR _hIcon$[ebp], 0
	jne	SHORT $L76504
	cmp	DWORD PTR _bNormal$[ebp], 0
	jne	SHORT $L76504

; 240  : 		ImageMap(TRUE).Lookup(nCmdID, hIcon);

	lea	eax, DWORD PTR _hIcon$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nCmdID$[ebp]
	push	ecx
	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ImageMap@CMenuIconMgr@@ABEABV?$CMap@IIPAUHICON__@@PAU1@@@H@Z ; CMenuIconMgr::ImageMap
	mov	ecx, eax
	call	?Lookup@?$CMap@IIPAUHICON__@@PAU1@@@QBEHIAAPAUHICON__@@@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::Lookup
$L76504:

; 241  : 
; 242  : 	return hIcon;

	mov	eax, DWORD PTR _hIcon$[ebp]

; 243  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?LoadItemIcon@CMenuIconMgr@@IBEPAUHICON__@@IH@Z ENDP	; CMenuIconMgr::LoadItemIcon
_TEXT	ENDS
PUBLIC	?LoadItemImage@CMenuIconMgr@@IAEPAXIH@Z		; CMenuIconMgr::LoadItemImage
PUBLIC	?Lookup@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEHPAUHICON__@@AAPAUHBITMAP__@@@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::Lookup
PUBLIC	??A?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEAAPAUHBITMAP__@@PAUHICON__@@@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::operator[]
EXTRN	?IconToPARGB32Bitmap@GraphicsMisc@@YAPAUHBITMAP__@@PAUHICON__@@@Z:NEAR ; GraphicsMisc::IconToPARGB32Bitmap
;	COMDAT data
; File D:\_CODE\Shared\MenuIconMgr.cpp
data	SEGMENT
$SG76520 DB	'D:\_CODE\Shared\MenuIconMgr.cpp', 00H
data	ENDS
;	COMDAT ?LoadItemImage@CMenuIconMgr@@IAEPAXIH@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_bNormal$ = 12
_this$ = -16
_hIcon$ = -4
_hBmp$76512 = -8
_nBreakpoint$76515 = -12
?LoadItemImage@CMenuIconMgr@@IAEPAXIH@Z PROC NEAR	; CMenuIconMgr::LoadItemImage, COMDAT

; 246  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 247  : 	HICON hIcon = LoadItemIcon(nCmdID, bNormal);

	mov	eax, DWORD PTR _bNormal$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nCmdID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadItemIcon@CMenuIconMgr@@IBEPAUHICON__@@IH@Z ; CMenuIconMgr::LoadItemIcon
	mov	DWORD PTR _hIcon$[ebp], eax

; 248  : 	
; 249  : 	if (hIcon && m_bVistaPlus)

	cmp	DWORD PTR _hIcon$[ebp], 0
	je	SHORT $L76511
	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+116], 0
	je	SHORT $L76511

; 251  : 		HBITMAP hBmp = NULL;

	mov	DWORD PTR _hBmp$76512[ebp], 0

; 252  : 
; 253  : 		if (!m_mapIcon2Bmp.Lookup(hIcon, hBmp))

	lea	eax, DWORD PTR _hBmp$76512[ebp]
	push	eax
	mov	ecx, DWORD PTR _hIcon$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 88					; 00000058H
	call	?Lookup@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEHPAUHICON__@@AAPAUHBITMAP__@@@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::Lookup
	test	eax, eax
	jne	SHORT $L76513

; 255  : 			hBmp = GraphicsMisc::IconToPARGB32Bitmap(hIcon);

	mov	edx, DWORD PTR _hIcon$[ebp]
	push	edx
	call	?IconToPARGB32Bitmap@GraphicsMisc@@YAPAUHBITMAP__@@PAUHICON__@@@Z ; GraphicsMisc::IconToPARGB32Bitmap
	add	esp, 4
	mov	DWORD PTR _hBmp$76512[ebp], eax

; 256  : 			m_mapIcon2Bmp[hIcon] = hBmp;

	mov	eax, DWORD PTR _hIcon$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 88					; 00000058H
	call	??A?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEAAPAUHBITMAP__@@PAUHICON__@@@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::operator[]
	mov	ecx, DWORD PTR _hBmp$76512[ebp]
	mov	DWORD PTR [eax], ecx

; 258  : #ifdef _DEBUG
; 259  : 		else

	jmp	SHORT $L76514
$L76513:

; 261  : 			int nBreakpoint = 0;

	mov	DWORD PTR _nBreakpoint$76515[ebp], 0
$L76514:

; 264  : #endif
; 265  : 
; 266  : 		ASSERT(hBmp);

	cmp	DWORD PTR _hBmp$76512[ebp], 0
	jne	SHORT $L76519
	push	266					; 0000010aH
	push	OFFSET FLAT:$SG76520
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76519
	int	3
$L76519:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76514

; 267  : 		return hBmp;

	mov	eax, DWORD PTR _hBmp$76512[ebp]
	jmp	SHORT $L76509
$L76511:

; 269  : 	
; 270  : 	return hIcon;

	mov	eax, DWORD PTR _hIcon$[ebp]
$L76509:

; 271  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?LoadItemImage@CMenuIconMgr@@IAEPAXIH@Z ENDP		; CMenuIconMgr::LoadItemImage
_TEXT	ENDS
PUBLIC	?AddImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z	; CMenuIconMgr::AddImage
EXTRN	?Add@CImageList@@QAEHPAUHICON__@@@Z:NEAR	; CImageList::Add
;	COMDAT xdata$x
; File D:\_CODE\Shared\MenuIconMgr.cpp
xdata$x	SEGMENT
$T77709	DD	019930520H
	DD	01H
	DD	FLAT:$T77711
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77711	DD	0ffffffffH
	DD	FLAT:$L77707
xdata$x	ENDS
;	COMDAT ?AddImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_hIcon$ = 12
_bNormal$ = 16
_this$ = -32
_il$ = -20
$T77705 = -24
$T77706 = -28
__$EHRec$ = -12
?AddImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z PROC NEAR	; CMenuIconMgr::AddImage, COMDAT

; 274  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77710
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
	mov	DWORD PTR _this$[ebp], ecx

; 275  : 	// we copy the icon's small image
; 276  : 	CImageList il;

	lea	ecx, DWORD PTR _il$[ebp]
	call	??0CImageList@@QAE@XZ			; CImageList::CImageList
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 277  : 	
; 278  : 	if (il.Create(16, 16, ILC_COLOR32 | ILC_MASK, 0, 1))

	push	1
	push	0
	push	33					; 00000021H
	push	16					; 00000010H
	push	16					; 00000010H
	lea	ecx, DWORD PTR _il$[ebp]
	call	?Create@CImageList@@QAEHHHIHH@Z		; CImageList::Create
	test	eax, eax
	je	SHORT $L76528

; 280  : 		il.Add(hIcon);

	mov	eax, DWORD PTR _hIcon$[ebp]
	push	eax
	lea	ecx, DWORD PTR _il$[ebp]
	call	?Add@CImageList@@QAEHPAUHICON__@@@Z	; CImageList::Add

; 281  : 		return SetImage(nCmdID, il.ExtractIcon(0), bNormal);

	mov	ecx, DWORD PTR _bNormal$[ebp]
	push	ecx
	push	0
	lea	ecx, DWORD PTR _il$[ebp]
	call	?ExtractIconW@CImageList@@QAEPAUHICON__@@H@Z ; CImageList::ExtractIconW
	push	eax
	mov	edx, DWORD PTR _nCmdID$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z ; CMenuIconMgr::SetImage
	mov	DWORD PTR $T77705[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _il$[ebp]
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	mov	eax, DWORD PTR $T77705[ebp]
	jmp	SHORT $L76526
$L76528:

; 283  : 	
; 284  : 	return FALSE;

	mov	DWORD PTR $T77706[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _il$[ebp]
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	mov	eax, DWORD PTR $T77706[ebp]
$L76526:

; 285  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77707:
	lea	ecx, DWORD PTR _il$[ebp]
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	ret	0
$L77710:
	mov	eax, OFFSET FLAT:$T77709
	jmp	___CxxFrameHandler
text$x	ENDS
?AddImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z ENDP	; CMenuIconMgr::AddImage
PUBLIC	?AddImage@CMenuIconMgr@@QAEHIABVCImageList@@HH@Z ; CMenuIconMgr::AddImage
;	COMDAT ?AddImage@CMenuIconMgr@@QAEHIABVCImageList@@HH@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_il$ = 12
_nImage$ = 16
_bNormal$ = 20
_this$ = -8
_pIL$ = -4
?AddImage@CMenuIconMgr@@QAEHIABVCImageList@@HH@Z PROC NEAR ; CMenuIconMgr::AddImage, COMDAT

; 288  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 289  : 	CImageList* pIL = const_cast<CImageList*>(&il);

	mov	eax, DWORD PTR _il$[ebp]
	mov	DWORD PTR _pIL$[ebp], eax

; 290  : 
; 291  : 	return SetImage(nCmdID, pIL->ExtractIcon(nImage), bNormal);

	mov	ecx, DWORD PTR _bNormal$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nImage$[ebp]
	push	edx
	mov	ecx, DWORD PTR _pIL$[ebp]
	call	?ExtractIconW@CImageList@@QAEPAUHICON__@@H@Z ; CImageList::ExtractIconW
	push	eax
	mov	eax, DWORD PTR _nCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z ; CMenuIconMgr::SetImage

; 292  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?AddImage@CMenuIconMgr@@QAEHIABVCImageList@@HH@Z ENDP	; CMenuIconMgr::AddImage
_TEXT	ENDS
PUBLIC	?DeleteImage@CMenuIconMgr@@QAEXI@Z		; CMenuIconMgr::DeleteImage
;	COMDAT ?DeleteImage@CMenuIconMgr@@QAEXI@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_this$ = -4
?DeleteImage@CMenuIconMgr@@QAEXI@Z PROC NEAR		; CMenuIconMgr::DeleteImage, COMDAT

; 295  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 296  : 	SetImage(nCmdID, NULL, TRUE);

	push	1
	push	0
	mov	eax, DWORD PTR _nCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z ; CMenuIconMgr::SetImage

; 297  : 	SetImage(nCmdID, NULL, FALSE);

	push	0
	push	0
	mov	ecx, DWORD PTR _nCmdID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z ; CMenuIconMgr::SetImage

; 298  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?DeleteImage@CMenuIconMgr@@QAEXI@Z ENDP			; CMenuIconMgr::DeleteImage
_TEXT	ENDS
;	COMDAT ?SetImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_hIcon$ = 12
_bNormal$ = 16
_this$ = -4
?SetImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z PROC NEAR	; CMenuIconMgr::SetImage, COMDAT

; 301  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 302  : 	if (nCmdID)

	cmp	DWORD PTR _nCmdID$[ebp], 0
	je	SHORT $L76550

; 304  : 		if (hIcon)

	cmp	DWORD PTR _hIcon$[ebp], 0
	je	SHORT $L76551

; 306  : 			ImageMap(bNormal)[nCmdID] = hIcon;

	mov	eax, DWORD PTR _nCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _bNormal$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ImageMap@CMenuIconMgr@@AAEAAV?$CMap@IIPAUHICON__@@PAU1@@@H@Z ; CMenuIconMgr::ImageMap
	mov	ecx, eax
	call	??A?$CMap@IIPAUHICON__@@PAU1@@@QAEAAPAUHICON__@@I@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::operator[]
	mov	edx, DWORD PTR _hIcon$[ebp]
	mov	DWORD PTR [eax], edx

; 308  : 		else

	jmp	SHORT $L76552
$L76551:

; 310  : 			::DestroyIcon(LoadItemIcon(nCmdID, bNormal));

	mov	eax, DWORD PTR _bNormal$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nCmdID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadItemIcon@CMenuIconMgr@@IBEPAUHICON__@@IH@Z ; CMenuIconMgr::LoadItemIcon
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__DestroyIcon@4
	cmp	esi, esp
	call	__chkesp

; 311  : 			ImageMap(bNormal).RemoveKey(nCmdID);

	mov	edx, DWORD PTR _nCmdID$[ebp]
	push	edx
	mov	eax, DWORD PTR _bNormal$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ImageMap@CMenuIconMgr@@AAEAAV?$CMap@IIPAUHICON__@@PAU1@@@H@Z ; CMenuIconMgr::ImageMap
	mov	ecx, eax
	call	?RemoveKey@?$CMap@IIPAUHICON__@@PAU1@@@QAEHI@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::RemoveKey
$L76552:

; 313  : 
; 314  : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76549
$L76550:

; 316  : 	
; 317  : 	return FALSE;

	xor	eax, eax
$L76549:

; 318  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?SetImage@CMenuIconMgr@@QAEHIPAUHICON__@@H@Z ENDP	; CMenuIconMgr::SetImage
_TEXT	ENDS
PUBLIC	?OnDrawItem@CMenuIconMgr@@IAEHHPAUtagDRAWITEMSTRUCT@@@Z ; CMenuIconMgr::OnDrawItem
PUBLIC	?OnInitMenuPopup@CMenuIconMgr@@IAEXPAVCMenu@@@Z	; CMenuIconMgr::OnInitMenuPopup
PUBLIC	?OnMeasureItem@CMenuIconMgr@@IAEHHPAUtagMEASUREITEMSTRUCT@@@Z ; CMenuIconMgr::OnMeasureItem
EXTRN	?FromHandle@CMenu@@SGPAV1@PAUHMENU__@@@Z:NEAR	; CMenu::FromHandle
EXTRN	?Default@CSubclassWnd@@IAEJXZ:NEAR		; CSubclassWnd::Default
;	COMDAT ?WindowProc@CMenuIconMgr@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT
_msg$ = 12
_wp$ = 16
_lp$ = 20
_this$ = -8
_lr$76568 = -4
?WindowProc@CMenuIconMgr@@MAEJPAUHWND__@@IIJ@Z PROC NEAR ; CMenuIconMgr::WindowProc, COMDAT

; 321  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 323  : 	{

	mov	eax, DWORD PTR _msg$[ebp]
	mov	DWORD PTR -12+[ebp], eax
	cmp	DWORD PTR -12+[ebp], 43			; 0000002bH
	je	SHORT $L76570
	cmp	DWORD PTR -12+[ebp], 44			; 0000002cH
	je	SHORT $L76573
	cmp	DWORD PTR -12+[ebp], 279		; 00000117H
	je	SHORT $L76564
	jmp	SHORT $L76561
$L76564:

; 324  : 	case WM_INITMENUPOPUP:
; 325  : 		if (!HIWORD(lp)) // let windows look after the system menu 

	mov	ecx, DWORD PTR _lp$[ebp]
	shr	ecx, 16					; 00000010H
	and	ecx, 65535				; 0000ffffH
	and	ecx, 65535				; 0000ffffH
	test	ecx, ecx
	jne	SHORT $L76567

; 327  : 			LRESULT lr = CSubclassWnd::Default();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Default@CSubclassWnd@@IAEJXZ		; CSubclassWnd::Default
	mov	DWORD PTR _lr$76568[ebp], eax

; 328  : 			OnInitMenuPopup(CMenu::FromHandle((HMENU)wp));

	mov	edx, DWORD PTR _wp$[ebp]
	push	edx
	call	?FromHandle@CMenu@@SGPAV1@PAUHMENU__@@@Z ; CMenu::FromHandle
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnInitMenuPopup@CMenuIconMgr@@IAEXPAVCMenu@@@Z ; CMenuIconMgr::OnInitMenuPopup

; 329  : 			return lr;

	mov	eax, DWORD PTR _lr$76568[ebp]
	jmp	SHORT $L76559
$L76567:

; 331  : 		break;

	jmp	SHORT $L76561
$L76570:

; 332  : 		
; 333  : 	case WM_DRAWITEM:
; 334  : 		if (OnDrawItem(wp, (LPDRAWITEMSTRUCT)lp))

	mov	eax, DWORD PTR _lp$[ebp]
	push	eax
	mov	ecx, DWORD PTR _wp$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnDrawItem@CMenuIconMgr@@IAEHHPAUtagDRAWITEMSTRUCT@@@Z ; CMenuIconMgr::OnDrawItem
	test	eax, eax
	je	SHORT $L76572

; 335  : 			return 0L;

	xor	eax, eax
	jmp	SHORT $L76559
$L76572:

; 336  : 		break;

	jmp	SHORT $L76561
$L76573:

; 337  : 		
; 338  : 	case WM_MEASUREITEM:
; 339  : 		if (OnMeasureItem(wp, (LPMEASUREITEMSTRUCT)lp))

	mov	edx, DWORD PTR _lp$[ebp]
	push	edx
	mov	eax, DWORD PTR _wp$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnMeasureItem@CMenuIconMgr@@IAEHHPAUtagMEASUREITEMSTRUCT@@@Z ; CMenuIconMgr::OnMeasureItem
	test	eax, eax
	je	SHORT $L76575

; 340  : 			return 0L;

	xor	eax, eax
	jmp	SHORT $L76559
$L76575:
$L76561:

; 343  : 	
; 344  : 	return CSubclassWnd::Default();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Default@CSubclassWnd@@IAEJXZ		; CSubclassWnd::Default
$L76559:

; 345  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?WindowProc@CMenuIconMgr@@MAEJPAUHWND__@@IIJ@Z ENDP	; CMenuIconMgr::WindowProc
_TEXT	ENDS
EXTRN	?GetSafeHmenu@CMenu@@QBEPAUHMENU__@@XZ:NEAR	; CMenu::GetSafeHmenu
EXTRN	?GetMenuItemCount@CMenu@@QBEIXZ:NEAR		; CMenu::GetMenuItemCount
EXTRN	?GetMenuItemInfoW@CMenu@@QAEHIPAUtagMENUITEMINFOW@@H@Z:NEAR ; CMenu::GetMenuItemInfoW
EXTRN	__imp__SetMenuInfo@8:NEAR
EXTRN	__imp__SetMenuItemInfoW@16:NEAR
;	COMDAT data
; File D:\_CODE\Shared\MenuIconMgr.cpp
data	SEGMENT
$SG76584 DB	'D:\_CODE\Shared\MenuIconMgr.cpp', 00H
data	ENDS
;	COMDAT ?OnInitMenuPopup@CMenuIconMgr@@IAEXPAVCMenu@@@Z
_TEXT	SEGMENT
_pMenu$ = 8
_this$ = -88
_mnfo$ = -76
_minfo$ = -48
_pos$ = -80
_hImage$76592 = -84
?OnInitMenuPopup@CMenuIconMgr@@IAEXPAVCMenu@@@Z PROC NEAR ; CMenuIconMgr::OnInitMenuPopup, COMDAT

; 348  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 88					; 00000058H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-88]
	mov	ecx, 22					; 00000016H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L76580:

; 349  : 	ASSERT (pMenu);

	cmp	DWORD PTR _pMenu$[ebp], 0
	jne	SHORT $L76583
	push	349					; 0000015dH
	push	OFFSET FLAT:$SG76584
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76583
	int	3
$L76583:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76580

; 350  : 	
; 351  :     MENUINFO mnfo;
; 352  :     mnfo.cbSize = sizeof(mnfo);

	mov	DWORD PTR _mnfo$[ebp], 28		; 0000001cH

; 353  :     mnfo.fMask = MIM_STYLE;

	mov	DWORD PTR _mnfo$[ebp+4], 16		; 00000010H

; 354  :     mnfo.dwStyle = MNS_CHECKORBMP | MNS_AUTODISMISS;

	mov	DWORD PTR _mnfo$[ebp+8], 335544320	; 14000000H

; 355  : 	::SetMenuInfo(pMenu->GetSafeHmenu(), &mnfo);

	mov	esi, esp
	lea	ecx, DWORD PTR _mnfo$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetSafeHmenu@CMenu@@QBEPAUHMENU__@@XZ	; CMenu::GetSafeHmenu
	push	eax
	call	DWORD PTR __imp__SetMenuInfo@8
	cmp	esi, esp
	call	__chkesp

; 356  : 	
; 357  :     MENUITEMINFO minfo;
; 358  :     minfo.cbSize = sizeof(minfo);

	mov	DWORD PTR _minfo$[ebp], 48		; 00000030H

; 359  : 	
; 360  :     for (UINT pos=0; pos<pMenu->GetMenuItemCount(); pos++)

	mov	DWORD PTR _pos$[ebp], 0
	jmp	SHORT $L76588
$L76589:
	mov	edx, DWORD PTR _pos$[ebp]
	add	edx, 1
	mov	DWORD PTR _pos$[ebp], edx
$L76588:
	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetMenuItemCount@CMenu@@QBEIXZ		; CMenu::GetMenuItemCount
	cmp	DWORD PTR _pos$[ebp], eax
	jae	$L76590

; 362  :         minfo.fMask = MIIM_FTYPE | MIIM_ID;

	mov	DWORD PTR _minfo$[ebp+4], 258		; 00000102H

; 363  :         pMenu->GetMenuItemInfo(pos, &minfo, TRUE);

	push	1
	lea	eax, DWORD PTR _minfo$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pos$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetMenuItemInfoW@CMenu@@QAEHIPAUtagMENUITEMINFOW@@H@Z ; CMenu::GetMenuItemInfoW

; 364  : 		
; 365  : 		if (!(minfo.fType & MFT_OWNERDRAW))

	mov	edx, DWORD PTR _minfo$[ebp+8]
	and	edx, 256				; 00000100H
	test	edx, edx
	jne	SHORT $L76591

; 367  : 			HANDLE hImage = LoadItemImage(minfo.wID, TRUE);

	push	1
	mov	eax, DWORD PTR _minfo$[ebp+16]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadItemImage@CMenuIconMgr@@IAEPAXIH@Z	; CMenuIconMgr::LoadItemImage
	mov	DWORD PTR _hImage$76592[ebp], eax

; 368  : 			minfo.fMask = MIIM_BITMAP | MIIM_DATA;

	mov	DWORD PTR _minfo$[ebp+4], 160		; 000000a0H

; 369  : 			
; 370  : 			if (hImage && !m_bVistaPlus)

	cmp	DWORD PTR _hImage$76592[ebp], 0
	je	SHORT $L76593
	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+116], 0
	jne	SHORT $L76593

; 371  : 				minfo.hbmpItem = HBMMENU_CALLBACK;

	mov	DWORD PTR _minfo$[ebp+44], -1

; 372  : 			else

	jmp	SHORT $L76595
$L76593:

; 373  : 				minfo.hbmpItem = (HBITMAP)hImage;

	mov	edx, DWORD PTR _hImage$76592[ebp]
	mov	DWORD PTR _minfo$[ebp+44], edx
$L76595:

; 374  : 			
; 375  : 			::SetMenuItemInfo(pMenu->GetSafeHmenu(), pos, TRUE, &minfo);

	mov	esi, esp
	lea	eax, DWORD PTR _minfo$[ebp]
	push	eax
	push	1
	mov	ecx, DWORD PTR _pos$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetSafeHmenu@CMenu@@QBEPAUHMENU__@@XZ	; CMenu::GetSafeHmenu
	push	eax
	call	DWORD PTR __imp__SetMenuItemInfoW@16
	cmp	esi, esp
	call	__chkesp
$L76591:

; 377  :     }

	jmp	$L76589
$L76590:

; 378  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 88					; 00000058H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?OnInitMenuPopup@CMenuIconMgr@@IAEXPAVCMenu@@@Z ENDP	; CMenuIconMgr::OnInitMenuPopup
_TEXT	ENDS
EXTRN	__imp__DrawIconEx@36:NEAR
EXTRN	?GetIconSize@GraphicsMisc@@YA?AVCSize@@PAUHICON__@@@Z:NEAR ; GraphicsMisc::GetIconSize
;	COMDAT ?OnDrawItem@CMenuIconMgr@@IAEHHPAUtagDRAWITEMSTRUCT@@@Z
_TEXT	SEGMENT
_lpdis$ = 12
_this$ = -20
_bDisabled$ = -8
_hIcon$ = -4
_sIcon$76608 = -16
?OnDrawItem@CMenuIconMgr@@IAEHHPAUtagDRAWITEMSTRUCT@@@Z PROC NEAR ; CMenuIconMgr::OnDrawItem, COMDAT

; 381  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 20					; 00000014H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 382  : 	// sanity check
; 383  : 	if (m_bVistaPlus)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+116], 0
	je	SHORT $L76602

; 384  : 		return FALSE; // someone else needs to handle

	xor	eax, eax
	jmp	$L76601
$L76602:

; 385  : 
; 386  :     if ((lpdis == NULL) || (lpdis->CtlType != ODT_MENU))

	cmp	DWORD PTR _lpdis$[ebp], 0
	je	SHORT $L76604
	mov	ecx, DWORD PTR _lpdis$[ebp]
	cmp	DWORD PTR [ecx], 1
	je	SHORT $L76603
$L76604:

; 387  :         return FALSE; // not for a menu

	xor	eax, eax
	jmp	SHORT $L76601
$L76603:

; 388  : 	
; 389  : 	BOOL bDisabled = (lpdis->itemState & (ODS_GRAYED | ODS_DISABLED));

	mov	edx, DWORD PTR _lpdis$[ebp]
	mov	eax, DWORD PTR [edx+16]
	and	eax, 6
	mov	DWORD PTR _bDisabled$[ebp], eax

; 390  :     HICON hIcon = LoadItemIcon(lpdis->itemID, !bDisabled);

	xor	ecx, ecx
	cmp	DWORD PTR _bDisabled$[ebp], 0
	sete	cl
	push	ecx
	mov	edx, DWORD PTR _lpdis$[ebp]
	mov	eax, DWORD PTR [edx+8]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadItemIcon@CMenuIconMgr@@IBEPAUHICON__@@IH@Z ; CMenuIconMgr::LoadItemIcon
	mov	DWORD PTR _hIcon$[ebp], eax

; 391  : 	
; 392  :     if (hIcon)

	cmp	DWORD PTR _hIcon$[ebp], 0
	je	SHORT $L76607

; 394  :         CSize sIcon = GraphicsMisc::GetIconSize(hIcon);

	mov	ecx, DWORD PTR _hIcon$[ebp]
	push	ecx
	lea	edx, DWORD PTR _sIcon$76608[ebp]
	push	edx
	call	?GetIconSize@GraphicsMisc@@YA?AVCSize@@PAUHICON__@@@Z ; GraphicsMisc::GetIconSize
	add	esp, 8

; 395  : 		
; 396  :         ::DrawIconEx(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, hIcon, 
; 397  : 					sIcon.cx, sIcon.cy, 0, NULL, DI_IMAGE | DI_MASK);

	mov	esi, esp
	push	3
	push	0
	push	0
	mov	eax, DWORD PTR _sIcon$76608[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sIcon$76608[ebp]
	push	ecx
	mov	edx, DWORD PTR _hIcon$[ebp]
	push	edx
	mov	eax, DWORD PTR _lpdis$[ebp]
	mov	ecx, DWORD PTR [eax+32]
	push	ecx
	mov	edx, DWORD PTR _lpdis$[ebp]
	mov	eax, DWORD PTR [edx+28]
	push	eax
	mov	ecx, DWORD PTR _lpdis$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	push	edx
	call	DWORD PTR __imp__DrawIconEx@36
	cmp	esi, esp
	call	__chkesp

; 398  : 	
; 399  : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76601
$L76607:

; 401  : 	
; 402  : 	return FALSE;

	xor	eax, eax
$L76601:

; 403  : }

	pop	esi
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?OnDrawItem@CMenuIconMgr@@IAEHHPAUtagDRAWITEMSTRUCT@@@Z ENDP ; CMenuIconMgr::OnDrawItem
_TEXT	ENDS
;	COMDAT ?OnMeasureItem@CMenuIconMgr@@IAEHHPAUtagMEASUREITEMSTRUCT@@@Z
_TEXT	SEGMENT
_lpmis$ = 12
_this$ = -16
_hIcon$ = -4
_sIcon$76620 = -12
?OnMeasureItem@CMenuIconMgr@@IAEHHPAUtagMEASUREITEMSTRUCT@@@Z PROC NEAR ; CMenuIconMgr::OnMeasureItem, COMDAT

; 406  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 407  : 	// sanity check
; 408  : 	if (m_bVistaPlus)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+116], 0
	je	SHORT $L76615

; 409  : 		return FALSE; // someone else needs to handle

	xor	eax, eax
	jmp	SHORT $L76614
$L76615:

; 410  : 
; 411  :     if ((lpmis==NULL) || (lpmis->CtlType != ODT_MENU))

	cmp	DWORD PTR _lpmis$[ebp], 0
	je	SHORT $L76617
	mov	ecx, DWORD PTR _lpmis$[ebp]
	cmp	DWORD PTR [ecx], 1
	je	SHORT $L76616
$L76617:

; 412  :         return FALSE; // not for a menu

	xor	eax, eax
	jmp	SHORT $L76614
$L76616:

; 413  : 	
; 414  :     HICON hIcon = LoadItemIcon(lpmis->itemID, TRUE);

	push	1
	mov	edx, DWORD PTR _lpmis$[ebp]
	mov	eax, DWORD PTR [edx+8]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadItemIcon@CMenuIconMgr@@IBEPAUHICON__@@IH@Z ; CMenuIconMgr::LoadItemIcon
	mov	DWORD PTR _hIcon$[ebp], eax

; 415  : 	
; 416  :     if (hIcon)

	cmp	DWORD PTR _hIcon$[ebp], 0
	je	SHORT $L76619

; 418  : 		CSize sIcon = GraphicsMisc::GetIconSize(hIcon);

	mov	ecx, DWORD PTR _hIcon$[ebp]
	push	ecx
	lea	edx, DWORD PTR _sIcon$76620[ebp]
	push	edx
	call	?GetIconSize@GraphicsMisc@@YA?AVCSize@@PAUHICON__@@@Z ; GraphicsMisc::GetIconSize
	add	esp, 8

; 419  : 
; 420  :         lpmis->itemWidth = sIcon.cx;

	mov	eax, DWORD PTR _lpmis$[ebp]
	mov	ecx, DWORD PTR _sIcon$76620[ebp]
	mov	DWORD PTR [eax+12], ecx

; 421  :         lpmis->itemHeight = sIcon.cy;

	mov	edx, DWORD PTR _lpmis$[ebp]
	mov	eax, DWORD PTR _sIcon$76620[ebp+4]
	mov	DWORD PTR [edx+16], eax

; 422  : 		
; 423  : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76614
$L76619:

; 425  : 	
; 426  : 	return FALSE;

	xor	eax, eax
$L76614:

; 427  : }

	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?OnMeasureItem@CMenuIconMgr@@IAEHHPAUtagMEASUREITEMSTRUCT@@@Z ENDP ; CMenuIconMgr::OnMeasureItem
_TEXT	ENDS
PUBLIC	?Serialize@?$CMap@IIPAUHICON__@@PAU1@@@UAEXAAVCArchive@@@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::Serialize
PUBLIC	?Dump@?$CMap@IIPAUHICON__@@PAU1@@@UBEXAAVCDumpContext@@@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::Dump
PUBLIC	?AssertValid@?$CMap@IIPAUHICON__@@PAU1@@@UBEXXZ	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::AssertValid
PUBLIC	??_7?$CMap@IIPAUHICON__@@PAU1@@@6B@		; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::`vftable'
PUBLIC	??_G?$CMap@IIPAUHICON__@@PAU1@@@UAEPAXI@Z	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::`scalar deleting destructor'
PUBLIC	??_E?$CMap@IIPAUHICON__@@PAU1@@@UAEPAXI@Z	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::`vector deleting destructor'
EXTRN	?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ:NEAR ; CObject::GetRuntimeClass
EXTRN	??1CObject@@UAE@XZ:NEAR				; CObject::~CObject
EXTRN	??0CObject@@IAE@XZ:NEAR				; CObject::CObject
;	COMDAT ??_7?$CMap@IIPAUHICON__@@PAU1@@@6B@
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
CONST	SEGMENT
??_7?$CMap@IIPAUHICON__@@PAU1@@@6B@ DD FLAT:?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::`vftable'
	DD	FLAT:??_E?$CMap@IIPAUHICON__@@PAU1@@@UAEPAXI@Z
	DD	FLAT:?Serialize@?$CMap@IIPAUHICON__@@PAU1@@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@?$CMap@IIPAUHICON__@@PAU1@@@UBEXXZ
	DD	FLAT:?Dump@?$CMap@IIPAUHICON__@@PAU1@@@UBEXAAVCDumpContext@@@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77733	DD	019930520H
	DD	01H
	DD	FLAT:$T77735
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77735	DD	0ffffffffH
	DD	FLAT:$L77731
xdata$x	ENDS
;	COMDAT ??0?$CMap@IIPAUHICON__@@PAU1@@@QAE@H@Z
_TEXT	SEGMENT
_nBlockSize$ = 8
__$EHRec$ = -12
_this$ = -16
??0?$CMap@IIPAUHICON__@@PAU1@@@QAE@H@Z PROC NEAR	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>, COMDAT

; 1201 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77734
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
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CMap@IIPAUHICON__@@PAU1@@@6B@ ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::`vftable'
$L76710:

; 1202 : 	ASSERT(nBlockSize > 0);

	cmp	DWORD PTR _nBlockSize$[ebp], 0
	jg	SHORT $L76713
	push	1202					; 000004b2H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76713
	int	3
$L76713:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76710

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
$L77731:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L77734:
	mov	eax, OFFSET FLAT:$T77733
	jmp	___CxxFrameHandler
text$x	ENDS
??0?$CMap@IIPAUHICON__@@PAU1@@@QAE@H@Z ENDP		; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>
PUBLIC	?GetAssocAt@?$CMap@IIPAUHICON__@@PAU1@@@IBEPAUCAssoc@1@IAAI@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::GetAssocAt
EXTRN	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z:NEAR ; AfxAssertValidObject
;	COMDAT ?Lookup@?$CMap@IIPAUHICON__@@PAU1@@@QBEHIAAPAUHICON__@@@Z
_TEXT	SEGMENT
_key$ = 8
_rValue$ = 12
_this$ = -12
_nHash$ = -8
_pAssoc$ = -4
?Lookup@?$CMap@IIPAUHICON__@@PAU1@@@QBEHIAAPAUHICON__@@@Z PROC NEAR ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::Lookup, COMDAT

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
	call	?GetAssocAt@?$CMap@IIPAUHICON__@@PAU1@@@IBEPAUCAssoc@1@IAAI@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::GetAssocAt
	mov	DWORD PTR _pAssoc$[ebp], eax

; 1346 : 	if (pAssoc == NULL)

	cmp	DWORD PTR _pAssoc$[ebp], 0
	jne	SHORT $L76721

; 1347 : 		return FALSE;  // not in map

	xor	eax, eax
	jmp	SHORT $L76718
$L76721:

; 1348 : 
; 1349 : 	rValue = pAssoc->value;

	mov	eax, DWORD PTR _rValue$[ebp]
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	mov	DWORD PTR [eax], edx

; 1350 : 	return TRUE;

	mov	eax, 1
$L76718:

; 1351 : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Lookup@?$CMap@IIPAUHICON__@@PAU1@@@QBEHIAAPAUHICON__@@@Z ENDP ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::Lookup
_TEXT	ENDS
PUBLIC	?InitHashTable@?$CMap@IIPAUHICON__@@PAU1@@@QAEXIH@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::InitHashTable
PUBLIC	?NewAssoc@?$CMap@IIPAUHICON__@@PAU1@@@IAEPAUCAssoc@1@XZ ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::NewAssoc
;	COMDAT ??A?$CMap@IIPAUHICON__@@PAU1@@@QAEAAPAUHICON__@@I@Z
_TEXT	SEGMENT
_key$ = 8
_this$ = -12
_nHash$ = -8
_pAssoc$ = -4
??A?$CMap@IIPAUHICON__@@PAU1@@@QAEAAPAUHICON__@@I@Z PROC NEAR ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::operator[], COMDAT

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
	call	?GetAssocAt@?$CMap@IIPAUHICON__@@PAU1@@@IBEPAUCAssoc@1@IAAI@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::GetAssocAt
	mov	DWORD PTR _pAssoc$[ebp], eax
	cmp	DWORD PTR _pAssoc$[ebp], 0
	jne	SHORT $L76728

; 1362 : 		if (m_pHashTable == NULL)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L76729

; 1363 : 			InitHashTable(m_nHashTableSize);

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?InitHashTable@?$CMap@IIPAUHICON__@@PAU1@@@QAEXIH@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::InitHashTable
$L76729:

; 1364 : 
; 1365 : 		// it doesn't exist, add a new Association
; 1366 : 		pAssoc = NewAssoc();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?NewAssoc@?$CMap@IIPAUHICON__@@PAU1@@@IAEPAUCAssoc@1@XZ ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::NewAssoc
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
$L76728:

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
??A?$CMap@IIPAUHICON__@@PAU1@@@QAEAAPAUHICON__@@I@Z ENDP ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::operator[]
_TEXT	ENDS
PUBLIC	?FreeAssoc@?$CMap@IIPAUHICON__@@PAU1@@@IAEXPAUCAssoc@1@@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::FreeAssoc
PUBLIC	?HashKey@@YGII@Z				; HashKey
PUBLIC	?CompareElements@@YGHPBI0@Z			; CompareElements
;	COMDAT ?RemoveKey@?$CMap@IIPAUHICON__@@PAU1@@@QAEHI@Z
_TEXT	SEGMENT
_key$ = 8
_this$ = -12
_ppAssocPrev$ = -4
_pAssoc$ = -8
?RemoveKey@?$CMap@IIPAUHICON__@@PAU1@@@QAEHI@Z PROC NEAR ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::RemoveKey, COMDAT

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
	jne	SHORT $L76734

; 1385 : 		return FALSE;  // nothing in the table

	xor	eax, eax
	jmp	SHORT $L76733
$L76734:

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
	jmp	SHORT $L76741
$L76742:
	mov	edx, DWORD PTR _pAssoc$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _pAssoc$[ebp], eax
$L76741:
	cmp	DWORD PTR _pAssoc$[ebp], 0
	je	SHORT $L76743

; 1393 : 		if (CompareElements(&pAssoc->key, &key))

	lea	ecx, DWORD PTR _key$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pAssoc$[ebp]
	add	edx, 8
	push	edx
	call	?CompareElements@@YGHPBI0@Z		; CompareElements
	test	eax, eax
	je	SHORT $L76750

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
	call	?FreeAssoc@?$CMap@IIPAUHICON__@@PAU1@@@IAEXPAUCAssoc@1@@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::FreeAssoc

; 1398 : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L76733
$L76750:

; 1400 : 		ppAssocPrev = &pAssoc->pNext;

	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	DWORD PTR _ppAssocPrev$[ebp], ecx

; 1401 : 	}

	jmp	SHORT $L76742
$L76743:

; 1402 : 	return FALSE;  // not found

	xor	eax, eax
$L76733:

; 1403 : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?RemoveKey@?$CMap@IIPAUHICON__@@PAU1@@@QAEHI@Z ENDP	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::RemoveKey
_TEXT	ENDS
PUBLIC	?DestructElements@@YGXPAPAUHICON__@@H@Z		; DestructElements
PUBLIC	?DestructElements@@YGXPAIH@Z			; DestructElements
EXTRN	?FreeDataChain@CPlex@@QAEXXZ:NEAR		; CPlex::FreeDataChain
;	COMDAT ?RemoveAll@?$CMap@IIPAUHICON__@@PAU1@@@QAEXXZ
_TEXT	SEGMENT
$T77746 = -12
_this$ = -16
_nHash$76755 = -4
_pAssoc$76759 = -8
?RemoveAll@?$CMap@IIPAUHICON__@@PAU1@@@QAEXXZ PROC NEAR	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::RemoveAll, COMDAT

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
	je	SHORT $L76758

; 1245 : 		// destroy elements (values and keys)
; 1246 : 		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)

	mov	DWORD PTR _nHash$76755[ebp], 0
	jmp	SHORT $L76756
$L76757:
	mov	edx, DWORD PTR _nHash$76755[ebp]
	add	edx, 1
	mov	DWORD PTR _nHash$76755[ebp], edx
$L76756:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nHash$76755[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jae	SHORT $L76758

; 1248 : 			CAssoc* pAssoc;
; 1249 : 			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _nHash$76755[ebp]
	mov	edx, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR _pAssoc$76759[ebp], edx

; 1250 : 			  pAssoc = pAssoc->pNext)

	jmp	SHORT $L76760
$L76761:
	mov	eax, DWORD PTR _pAssoc$76759[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssoc$76759[ebp], ecx
$L76760:
	cmp	DWORD PTR _pAssoc$76759[ebp], 0
	je	SHORT $L76762

; 1252 : 				DestructElements<VALUE>(&pAssoc->value, 1);

	push	1
	mov	edx, DWORD PTR _pAssoc$76759[ebp]
	add	edx, 12					; 0000000cH
	push	edx
	call	?DestructElements@@YGXPAPAUHICON__@@H@Z	; DestructElements

; 1253 : 				DestructElements<KEY>(&pAssoc->key, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$76759[ebp]
	add	eax, 8
	push	eax
	call	?DestructElements@@YGXPAIH@Z		; DestructElements

; 1254 : 			}

	jmp	SHORT $L76761
$L76762:

; 1255 : 		}

	jmp	SHORT $L76757
$L76758:

; 1257 : 
; 1258 : 	// free hash table
; 1259 : 	delete[] m_pHashTable;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR $T77746[ebp], edx
	mov	eax, DWORD PTR $T77746[ebp]
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
?RemoveAll@?$CMap@IIPAUHICON__@@PAU1@@@QAEXXZ ENDP	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::RemoveAll
_TEXT	ENDS
;	COMDAT ?GetStartPosition@?$CMap@IIPAUHICON__@@PAU1@@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetStartPosition@?$CMap@IIPAUHICON__@@PAU1@@@QBEPAU__POSITION@@XZ PROC NEAR ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::GetStartPosition, COMDAT

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
?GetStartPosition@?$CMap@IIPAUHICON__@@PAU1@@@QBEPAU__POSITION@@XZ ENDP ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::GetStartPosition
_TEXT	ENDS
EXTRN	?AfxIsValidAddress@@YGHPBXIH@Z:NEAR		; AfxIsValidAddress
;	COMDAT ?GetNextAssoc@?$CMap@IIPAUHICON__@@PAU1@@@QBEXAAPAU__POSITION@@AAIAAPAUHICON__@@@Z
_TEXT	SEGMENT
_pAssocNext$ = -8
_nBucket$76813 = -16
_rNextPosition$ = 8
_rKey$ = 12
_rValue$ = 16
_this$ = -20
_pAssocRet$ = -4
_nBucket$76797 = -12
?GetNextAssoc@?$CMap@IIPAUHICON__@@PAU1@@@QBEXAAPAU__POSITION@@AAIAAPAUHICON__@@@Z PROC NEAR ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::GetNextAssoc, COMDAT

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
$L76784:

; 1410 : 	ASSERT(m_pHashTable != NULL);  // never call on empty map

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	jne	SHORT $L76787
	push	1410					; 00000582H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76787
	int	3
$L76787:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76784

; 1411 : 
; 1412 : 	CAssoc* pAssocRet = (CAssoc*)rNextPosition;

	mov	eax, DWORD PTR _rNextPosition$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssocRet$[ebp], ecx
$L76790:

; 1413 : 	ASSERT(pAssocRet != NULL);

	cmp	DWORD PTR _pAssocRet$[ebp], 0
	jne	SHORT $L76793
	push	1413					; 00000585H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76793
	int	3
$L76793:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76790

; 1414 : 
; 1415 : 	if (pAssocRet == (CAssoc*) BEFORE_START_POSITION)

	cmp	DWORD PTR _pAssocRet$[ebp], -1
	jne	SHORT $L76804

; 1417 : 		// find the first association
; 1418 : 		for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)

	mov	DWORD PTR _nBucket$76797[ebp], 0
	jmp	SHORT $L76798
$L76799:
	mov	eax, DWORD PTR _nBucket$76797[ebp]
	add	eax, 1
	mov	DWORD PTR _nBucket$76797[ebp], eax
$L76798:
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nBucket$76797[ebp]
	cmp	edx, DWORD PTR [ecx+8]
	jae	SHORT $L76800

; 1419 : 			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _nBucket$76797[ebp]
	mov	eax, DWORD PTR [ecx+edx*4]
	mov	DWORD PTR _pAssocRet$[ebp], eax
	cmp	DWORD PTR _pAssocRet$[ebp], 0
	je	SHORT $L76801

; 1420 : 				break;

	jmp	SHORT $L76800
$L76801:

; 1421 : 		ASSERT(pAssocRet != NULL);  // must find something

	jmp	SHORT $L76799
$L76800:
	cmp	DWORD PTR _pAssocRet$[ebp], 0
	jne	SHORT $L76805
	push	1421					; 0000058dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76805
	int	3
$L76805:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76800
$L76804:

; 1423 : 
; 1424 : 	// find next association
; 1425 : 	ASSERT(AfxIsValidAddress(pAssocRet, sizeof(CAssoc)));

	push	1
	push	16					; 00000010H
	mov	edx, DWORD PTR _pAssocRet$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76810
	push	1425					; 00000591H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76810
	int	3
$L76810:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76804

; 1426 : 	CAssoc* pAssocNext;
; 1427 : 	if ((pAssocNext = pAssocRet->pNext) == NULL)

	mov	ecx, DWORD PTR _pAssocRet$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _pAssocNext$[ebp], edx
	cmp	DWORD PTR _pAssocNext$[ebp], 0
	jne	SHORT $L76816

; 1429 : 		// go to next bucket
; 1430 : 		for (UINT nBucket = pAssocRet->nHashValue + 1;

	mov	eax, DWORD PTR _pAssocRet$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	add	ecx, 1
	mov	DWORD PTR _nBucket$76813[ebp], ecx

; 1431 : 		  nBucket < m_nHashTableSize; nBucket++)

	jmp	SHORT $L76814
$L76815:
	mov	edx, DWORD PTR _nBucket$76813[ebp]
	add	edx, 1
	mov	DWORD PTR _nBucket$76813[ebp], edx
$L76814:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nBucket$76813[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jae	SHORT $L76816

; 1432 : 			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _nBucket$76813[ebp]
	mov	edx, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR _pAssocNext$[ebp], edx
	cmp	DWORD PTR _pAssocNext$[ebp], 0
	je	SHORT $L76817

; 1433 : 				break;

	jmp	SHORT $L76816
$L76817:

; 1434 : 	}

	jmp	SHORT $L76815
$L76816:

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
?GetNextAssoc@?$CMap@IIPAUHICON__@@PAU1@@@QBEXAAPAU__POSITION@@AAIAAPAUHICON__@@@Z ENDP ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::GetNextAssoc
_TEXT	ENDS
;	COMDAT xdata$x
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
xdata$x	SEGMENT
$T77755	DD	019930520H
	DD	01H
	DD	FLAT:$T77757
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77757	DD	0ffffffffH
	DD	FLAT:$L77753
xdata$x	ENDS
;	COMDAT ??1?$CMap@IIPAUHICON__@@PAU1@@@UAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1?$CMap@IIPAUHICON__@@PAU1@@@UAE@XZ PROC NEAR		; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::~CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>, COMDAT

; 1270 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77756
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
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CMap@IIPAUHICON__@@PAU1@@@6B@ ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1271 : 	RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CMap@IIPAUHICON__@@PAU1@@@QAEXXZ ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::RemoveAll
$L76822:

; 1272 : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L76825
	push	1272					; 000004f8H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76825
	int	3
$L76825:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76822

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
$L77753:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L77756:
	mov	eax, OFFSET FLAT:$T77755
	jmp	___CxxFrameHandler
text$x	ENDS
??1?$CMap@IIPAUHICON__@@PAU1@@@UAE@XZ ENDP		; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::~CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>
PUBLIC	?SerializeElements@@YGXAAVCArchive@@PAIH@Z	; SerializeElements
PUBLIC	?SerializeElements@@YGXAAVCArchive@@PAPAUHICON__@@H@Z ; SerializeElements
PUBLIC	?SetAt@?$CMap@IIPAUHICON__@@PAU1@@@QAEXIPAUHICON__@@@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::SetAt
EXTRN	?ReadCount@CArchive@@QAEKXZ:NEAR		; CArchive::ReadCount
EXTRN	?WriteCount@CArchive@@QAEXK@Z:NEAR		; CArchive::WriteCount
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	?IsStoring@CArchive@@QBEHXZ:NEAR		; CArchive::IsStoring
;	COMDAT ?Serialize@?$CMap@IIPAUHICON__@@PAU1@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT
_ar$ = 8
_this$ = -24
_nHash$76836 = -4
_pAssoc$76840 = -8
_nNewCount$76857 = -12
_newKey$76861 = -16
_newValue$76862 = -20
?Serialize@?$CMap@IIPAUHICON__@@PAU1@@@UAEXAAVCArchive@@@Z PROC NEAR ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::Serialize, COMDAT

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
	je	$L76830

; 1452 : 		ar.WriteCount(m_nCount);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?WriteCount@CArchive@@QAEXK@Z		; CArchive::WriteCount

; 1453 : 		if (m_nCount == 0)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jne	SHORT $L76831

; 1454 : 			return;  // nothing more to do

	jmp	$L76829
$L76831:

; 1455 : 
; 1456 : 		ASSERT(m_pHashTable != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	jne	SHORT $L76835
	push	1456					; 000005b0H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76835
	int	3
$L76835:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76831

; 1457 : 		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)

	mov	DWORD PTR _nHash$76836[ebp], 0
	jmp	SHORT $L76837
$L76838:
	mov	ecx, DWORD PTR _nHash$76836[ebp]
	add	ecx, 1
	mov	DWORD PTR _nHash$76836[ebp], ecx
$L76837:
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nHash$76836[ebp]
	cmp	eax, DWORD PTR [edx+8]
	jae	SHORT $L76839

; 1459 : 			CAssoc* pAssoc;
; 1460 : 			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR _nHash$76836[ebp]
	mov	ecx, DWORD PTR [edx+eax*4]
	mov	DWORD PTR _pAssoc$76840[ebp], ecx

; 1461 : 			  pAssoc = pAssoc->pNext)

	jmp	SHORT $L76841
$L76842:
	mov	edx, DWORD PTR _pAssoc$76840[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _pAssoc$76840[ebp], eax
$L76841:
	cmp	DWORD PTR _pAssoc$76840[ebp], 0
	je	SHORT $L76843

; 1463 : 				SerializeElements<KEY>(ar, &pAssoc->key, 1);

	push	1
	mov	ecx, DWORD PTR _pAssoc$76840[ebp]
	add	ecx, 8
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAIH@Z ; SerializeElements

; 1464 : 				SerializeElements<VALUE>(ar, &pAssoc->value, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$76840[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	call	?SerializeElements@@YGXAAVCArchive@@PAPAUHICON__@@H@Z ; SerializeElements

; 1465 : 			}

	jmp	SHORT $L76842
$L76843:

; 1466 : 		}

	jmp	SHORT $L76838
$L76839:

; 1468 : 	else

	jmp	SHORT $L76860
$L76830:

; 1470 : 		DWORD nNewCount = ar.ReadCount();

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?ReadCount@CArchive@@QAEKXZ		; CArchive::ReadCount
	mov	DWORD PTR _nNewCount$76857[ebp], eax
$L76859:

; 1471 : 		while (nNewCount--)

	mov	edx, DWORD PTR _nNewCount$76857[ebp]
	mov	eax, DWORD PTR _nNewCount$76857[ebp]
	sub	eax, 1
	mov	DWORD PTR _nNewCount$76857[ebp], eax
	test	edx, edx
	je	SHORT $L76860

; 1473 : 			KEY newKey;
; 1474 : 			VALUE newValue;
; 1475 : 			SerializeElements<KEY>(ar, &newKey, 1);

	push	1
	lea	ecx, DWORD PTR _newKey$76861[ebp]
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAIH@Z ; SerializeElements

; 1476 : 			SerializeElements<VALUE>(ar, &newValue, 1);

	push	1
	lea	eax, DWORD PTR _newValue$76862[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	call	?SerializeElements@@YGXAAVCArchive@@PAPAUHICON__@@H@Z ; SerializeElements

; 1477 : 			SetAt(newKey, newValue);

	mov	edx, DWORD PTR _newValue$76862[ebp]
	push	edx
	mov	eax, DWORD PTR _newKey$76861[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetAt@?$CMap@IIPAUHICON__@@PAU1@@@QAEXIPAUHICON__@@@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::SetAt

; 1478 : 		}

	jmp	SHORT $L76859
$L76860:
$L76829:

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
?Serialize@?$CMap@IIPAUHICON__@@PAU1@@@UAEXAAVCArchive@@@Z ENDP ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::Serialize
_TEXT	ENDS
PUBLIC	??_C@_05KIFI@with?5?$AA@			; `string'
PUBLIC	??_C@_09LOPF@?5elements?$AA@			; `string'
PUBLIC	??_C@_03DANH@?6?7?$FL?$AA@			; `string'
PUBLIC	?DumpElements@@YGXAAVCDumpContext@@PBIH@Z	; DumpElements
PUBLIC	??_C@_04MNLE@?$FN?5?$DN?5?$AA@			; `string'
PUBLIC	?DumpElements@@YGXAAVCDumpContext@@PBQAUHICON__@@H@Z ; DumpElements
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
;	COMDAT ?Dump@?$CMap@IIPAUHICON__@@PAU1@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT
_dc$ = 8
_this$ = -16
_key$76880 = -8
_val$76881 = -4
_pos$76882 = -12
?Dump@?$CMap@IIPAUHICON__@@PAU1@@@UBEXAAVCDumpContext@@@Z PROC NEAR ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::Dump, COMDAT

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
	jle	SHORT $L76885

; 1491 : 		// Dump in format "[key] -> value"
; 1492 : 		KEY key;
; 1493 : 		VALUE val;
; 1494 : 
; 1495 : 		POSITION pos = GetStartPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetStartPosition@?$CMap@IIPAUHICON__@@PAU1@@@QBEPAU__POSITION@@XZ ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::GetStartPosition
	mov	DWORD PTR _pos$76882[ebp], eax
$L76884:

; 1496 : 		while (pos != NULL)

	cmp	DWORD PTR _pos$76882[ebp], 0
	je	SHORT $L76885

; 1498 : 			GetNextAssoc(pos, key, val);

	lea	eax, DWORD PTR _val$76881[ebp]
	push	eax
	lea	ecx, DWORD PTR _key$76880[ebp]
	push	ecx
	lea	edx, DWORD PTR _pos$76882[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetNextAssoc@?$CMap@IIPAUHICON__@@PAU1@@@QBEXAAPAU__POSITION@@AAIAAPAUHICON__@@@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::GetNextAssoc

; 1499 : 			dc << "\n\t[";

	push	OFFSET FLAT:??_C@_03DANH@?6?7?$FL?$AA@	; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1500 : 			DumpElements<KEY>(dc, &key, 1);

	push	1
	lea	eax, DWORD PTR _key$76880[ebp]
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
	lea	edx, DWORD PTR _val$76881[ebp]
	push	edx
	mov	eax, DWORD PTR _dc$[ebp]
	push	eax
	call	?DumpElements@@YGXAAVCDumpContext@@PBQAUHICON__@@H@Z ; DumpElements

; 1503 : 		}

	jmp	SHORT $L76884
$L76885:

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
?Dump@?$CMap@IIPAUHICON__@@PAU1@@@UBEXAAVCDumpContext@@@Z ENDP ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::Dump
_TEXT	ENDS
EXTRN	?AssertValid@CObject@@UBEXXZ:NEAR		; CObject::AssertValid
;	COMDAT ?AssertValid@?$CMap@IIPAUHICON__@@PAU1@@@UBEXXZ
_TEXT	SEGMENT
_this$ = -4
?AssertValid@?$CMap@IIPAUHICON__@@PAU1@@@UBEXXZ PROC NEAR ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::AssertValid, COMDAT

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
$L76904:

; 1513 : 
; 1514 : 	ASSERT(m_nHashTableSize > 0);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	ja	SHORT $L76907
	push	1514					; 000005eaH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76907
	int	3
$L76907:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76904
$L76906:

; 1515 : 	ASSERT(m_nCount == 0 || m_pHashTable != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+12], 0
	je	SHORT $L76911
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L76911
	push	1515					; 000005ebH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76911
	int	3
$L76911:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76906

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
?AssertValid@?$CMap@IIPAUHICON__@@PAU1@@@UBEXXZ ENDP	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::AssertValid
_TEXT	ENDS
PUBLIC	?Serialize@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAEXAAVCArchive@@@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::Serialize
PUBLIC	?Dump@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UBEXAAVCDumpContext@@@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::Dump
PUBLIC	?AssertValid@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UBEXXZ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::AssertValid
PUBLIC	??_7?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@6B@ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::`vftable'
PUBLIC	??_G?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAEPAXI@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::`scalar deleting destructor'
PUBLIC	??_E?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAEPAXI@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::`vector deleting destructor'
;	COMDAT ??_7?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@6B@
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
CONST	SEGMENT
??_7?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@6B@ DD FLAT:?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::`vftable'
	DD	FLAT:??_E?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAEPAXI@Z
	DD	FLAT:?Serialize@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UBEXXZ
	DD	FLAT:?Dump@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UBEXAAVCDumpContext@@@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77770	DD	019930520H
	DD	01H
	DD	FLAT:$T77772
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77772	DD	0ffffffffH
	DD	FLAT:$L77768
xdata$x	ENDS
;	COMDAT ??0?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAE@H@Z
_TEXT	SEGMENT
_nBlockSize$ = 8
_this$ = -16
__$EHRec$ = -12
??0?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAE@H@Z PROC NEAR ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>, COMDAT

; 1201 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77771
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
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@6B@ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::`vftable'
$L76916:

; 1202 : 	ASSERT(nBlockSize > 0);

	cmp	DWORD PTR _nBlockSize$[ebp], 0
	jg	SHORT $L76919
	push	1202					; 000004b2H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76919
	int	3
$L76919:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76916

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
$L77768:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L77771:
	mov	eax, OFFSET FLAT:$T77770
	jmp	___CxxFrameHandler
text$x	ENDS
??0?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAE@H@Z ENDP ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>
PUBLIC	?GetAssocAt@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@IBEPAUCAssoc@1@PAUHICON__@@AAI@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::GetAssocAt
;	COMDAT ?Lookup@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEHPAUHICON__@@AAPAUHBITMAP__@@@Z
_TEXT	SEGMENT
_key$ = 8
_rValue$ = 12
_this$ = -12
_nHash$ = -8
_pAssoc$ = -4
?Lookup@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEHPAUHICON__@@AAPAUHBITMAP__@@@Z PROC NEAR ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::Lookup, COMDAT

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
	call	?GetAssocAt@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@IBEPAUCAssoc@1@PAUHICON__@@AAI@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::GetAssocAt
	mov	DWORD PTR _pAssoc$[ebp], eax

; 1346 : 	if (pAssoc == NULL)

	cmp	DWORD PTR _pAssoc$[ebp], 0
	jne	SHORT $L76927

; 1347 : 		return FALSE;  // not in map

	xor	eax, eax
	jmp	SHORT $L76924
$L76927:

; 1348 : 
; 1349 : 	rValue = pAssoc->value;

	mov	eax, DWORD PTR _rValue$[ebp]
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	mov	DWORD PTR [eax], edx

; 1350 : 	return TRUE;

	mov	eax, 1
$L76924:

; 1351 : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Lookup@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEHPAUHICON__@@AAPAUHBITMAP__@@@Z ENDP ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::Lookup
_TEXT	ENDS
PUBLIC	?InitHashTable@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXIH@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::InitHashTable
PUBLIC	?NewAssoc@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@IAEPAUCAssoc@1@XZ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::NewAssoc
;	COMDAT ??A?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEAAPAUHBITMAP__@@PAUHICON__@@@Z
_TEXT	SEGMENT
_key$ = 8
_this$ = -12
_nHash$ = -8
_pAssoc$ = -4
??A?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEAAPAUHBITMAP__@@PAUHICON__@@@Z PROC NEAR ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::operator[], COMDAT

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
	call	?GetAssocAt@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@IBEPAUCAssoc@1@PAUHICON__@@AAI@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::GetAssocAt
	mov	DWORD PTR _pAssoc$[ebp], eax
	cmp	DWORD PTR _pAssoc$[ebp], 0
	jne	SHORT $L76934

; 1362 : 		if (m_pHashTable == NULL)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L76935

; 1363 : 			InitHashTable(m_nHashTableSize);

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?InitHashTable@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXIH@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::InitHashTable
$L76935:

; 1364 : 
; 1365 : 		// it doesn't exist, add a new Association
; 1366 : 		pAssoc = NewAssoc();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?NewAssoc@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@IAEPAUCAssoc@1@XZ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::NewAssoc
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
$L76934:

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
??A?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEAAPAUHBITMAP__@@PAUHICON__@@@Z ENDP ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::operator[]
_TEXT	ENDS
PUBLIC	?DestructElements@@YGXPAPAUHBITMAP__@@H@Z	; DestructElements
;	COMDAT ?RemoveAll@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXXZ
_TEXT	SEGMENT
_this$ = -16
_nHash$76940 = -4
_pAssoc$76944 = -8
$T77781 = -12
?RemoveAll@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXXZ PROC NEAR ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::RemoveAll, COMDAT

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
	je	SHORT $L76943

; 1245 : 		// destroy elements (values and keys)
; 1246 : 		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)

	mov	DWORD PTR _nHash$76940[ebp], 0
	jmp	SHORT $L76941
$L76942:
	mov	edx, DWORD PTR _nHash$76940[ebp]
	add	edx, 1
	mov	DWORD PTR _nHash$76940[ebp], edx
$L76941:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nHash$76940[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jae	SHORT $L76943

; 1248 : 			CAssoc* pAssoc;
; 1249 : 			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _nHash$76940[ebp]
	mov	edx, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR _pAssoc$76944[ebp], edx

; 1250 : 			  pAssoc = pAssoc->pNext)

	jmp	SHORT $L76945
$L76946:
	mov	eax, DWORD PTR _pAssoc$76944[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssoc$76944[ebp], ecx
$L76945:
	cmp	DWORD PTR _pAssoc$76944[ebp], 0
	je	SHORT $L76947

; 1252 : 				DestructElements<VALUE>(&pAssoc->value, 1);

	push	1
	mov	edx, DWORD PTR _pAssoc$76944[ebp]
	add	edx, 12					; 0000000cH
	push	edx
	call	?DestructElements@@YGXPAPAUHBITMAP__@@H@Z ; DestructElements

; 1253 : 				DestructElements<KEY>(&pAssoc->key, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$76944[ebp]
	add	eax, 8
	push	eax
	call	?DestructElements@@YGXPAPAUHICON__@@H@Z	; DestructElements

; 1254 : 			}

	jmp	SHORT $L76946
$L76947:

; 1255 : 		}

	jmp	SHORT $L76942
$L76943:

; 1257 : 
; 1258 : 	// free hash table
; 1259 : 	delete[] m_pHashTable;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR $T77781[ebp], edx
	mov	eax, DWORD PTR $T77781[ebp]
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
?RemoveAll@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXXZ ENDP ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::RemoveAll
_TEXT	ENDS
;	COMDAT ?GetStartPosition@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetStartPosition@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEPAU__POSITION@@XZ PROC NEAR ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::GetStartPosition, COMDAT

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
?GetStartPosition@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEPAU__POSITION@@XZ ENDP ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::GetStartPosition
_TEXT	ENDS
;	COMDAT ?GetNextAssoc@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEXAAPAU__POSITION@@AAPAUHICON__@@AAPAUHBITMAP__@@@Z
_TEXT	SEGMENT
_rNextPosition$ = 8
_rKey$ = 12
_rValue$ = 16
_this$ = -20
_pAssocRet$ = -4
_nBucket$76981 = -12
_pAssocNext$ = -8
_nBucket$76997 = -16
?GetNextAssoc@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEXAAPAU__POSITION@@AAPAUHICON__@@AAPAUHBITMAP__@@@Z PROC NEAR ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::GetNextAssoc, COMDAT

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
$L76968:

; 1410 : 	ASSERT(m_pHashTable != NULL);  // never call on empty map

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	jne	SHORT $L76971
	push	1410					; 00000582H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76971
	int	3
$L76971:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76968

; 1411 : 
; 1412 : 	CAssoc* pAssocRet = (CAssoc*)rNextPosition;

	mov	eax, DWORD PTR _rNextPosition$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssocRet$[ebp], ecx
$L76974:

; 1413 : 	ASSERT(pAssocRet != NULL);

	cmp	DWORD PTR _pAssocRet$[ebp], 0
	jne	SHORT $L76977
	push	1413					; 00000585H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76977
	int	3
$L76977:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76974

; 1414 : 
; 1415 : 	if (pAssocRet == (CAssoc*) BEFORE_START_POSITION)

	cmp	DWORD PTR _pAssocRet$[ebp], -1
	jne	SHORT $L76988

; 1417 : 		// find the first association
; 1418 : 		for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)

	mov	DWORD PTR _nBucket$76981[ebp], 0
	jmp	SHORT $L76982
$L76983:
	mov	eax, DWORD PTR _nBucket$76981[ebp]
	add	eax, 1
	mov	DWORD PTR _nBucket$76981[ebp], eax
$L76982:
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nBucket$76981[ebp]
	cmp	edx, DWORD PTR [ecx+8]
	jae	SHORT $L76984

; 1419 : 			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _nBucket$76981[ebp]
	mov	eax, DWORD PTR [ecx+edx*4]
	mov	DWORD PTR _pAssocRet$[ebp], eax
	cmp	DWORD PTR _pAssocRet$[ebp], 0
	je	SHORT $L76985

; 1420 : 				break;

	jmp	SHORT $L76984
$L76985:

; 1421 : 		ASSERT(pAssocRet != NULL);  // must find something

	jmp	SHORT $L76983
$L76984:
	cmp	DWORD PTR _pAssocRet$[ebp], 0
	jne	SHORT $L76989
	push	1421					; 0000058dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76989
	int	3
$L76989:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76984
$L76988:

; 1423 : 
; 1424 : 	// find next association
; 1425 : 	ASSERT(AfxIsValidAddress(pAssocRet, sizeof(CAssoc)));

	push	1
	push	16					; 00000010H
	mov	edx, DWORD PTR _pAssocRet$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76994
	push	1425					; 00000591H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76994
	int	3
$L76994:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76988

; 1426 : 	CAssoc* pAssocNext;
; 1427 : 	if ((pAssocNext = pAssocRet->pNext) == NULL)

	mov	ecx, DWORD PTR _pAssocRet$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _pAssocNext$[ebp], edx
	cmp	DWORD PTR _pAssocNext$[ebp], 0
	jne	SHORT $L77000

; 1429 : 		// go to next bucket
; 1430 : 		for (UINT nBucket = pAssocRet->nHashValue + 1;

	mov	eax, DWORD PTR _pAssocRet$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	add	ecx, 1
	mov	DWORD PTR _nBucket$76997[ebp], ecx

; 1431 : 		  nBucket < m_nHashTableSize; nBucket++)

	jmp	SHORT $L76998
$L76999:
	mov	edx, DWORD PTR _nBucket$76997[ebp]
	add	edx, 1
	mov	DWORD PTR _nBucket$76997[ebp], edx
$L76998:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nBucket$76997[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jae	SHORT $L77000

; 1432 : 			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _nBucket$76997[ebp]
	mov	edx, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR _pAssocNext$[ebp], edx
	cmp	DWORD PTR _pAssocNext$[ebp], 0
	je	SHORT $L77001

; 1433 : 				break;

	jmp	SHORT $L77000
$L77001:

; 1434 : 	}

	jmp	SHORT $L76999
$L77000:

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
?GetNextAssoc@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEXAAPAU__POSITION@@AAPAUHICON__@@AAPAUHBITMAP__@@@Z ENDP ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::GetNextAssoc
_TEXT	ENDS
;	COMDAT xdata$x
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
xdata$x	SEGMENT
$T77790	DD	019930520H
	DD	01H
	DD	FLAT:$T77792
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77792	DD	0ffffffffH
	DD	FLAT:$L77788
xdata$x	ENDS
;	COMDAT ??1?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAE@XZ PROC NEAR ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::~CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>, COMDAT

; 1270 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77791
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
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@6B@ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1271 : 	RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXXZ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::RemoveAll
$L77006:

; 1272 : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L77009
	push	1272					; 000004f8H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77009
	int	3
$L77009:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77006

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
$L77788:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L77791:
	mov	eax, OFFSET FLAT:$T77790
	jmp	___CxxFrameHandler
text$x	ENDS
??1?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAE@XZ ENDP ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::~CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>
PUBLIC	?SerializeElements@@YGXAAVCArchive@@PAPAUHBITMAP__@@H@Z ; SerializeElements
PUBLIC	?SetAt@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXPAUHICON__@@PAUHBITMAP__@@@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::SetAt
;	COMDAT ?Serialize@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT
_ar$ = 8
_this$ = -24
_nHash$77020 = -4
_pAssoc$77024 = -8
_nNewCount$77040 = -12
_newKey$77044 = -16
_newValue$77045 = -20
?Serialize@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAEXAAVCArchive@@@Z PROC NEAR ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::Serialize, COMDAT

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
	je	$L77014

; 1452 : 		ar.WriteCount(m_nCount);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?WriteCount@CArchive@@QAEXK@Z		; CArchive::WriteCount

; 1453 : 		if (m_nCount == 0)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jne	SHORT $L77015

; 1454 : 			return;  // nothing more to do

	jmp	$L77013
$L77015:

; 1455 : 
; 1456 : 		ASSERT(m_pHashTable != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	jne	SHORT $L77019
	push	1456					; 000005b0H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77019
	int	3
$L77019:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L77015

; 1457 : 		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)

	mov	DWORD PTR _nHash$77020[ebp], 0
	jmp	SHORT $L77021
$L77022:
	mov	ecx, DWORD PTR _nHash$77020[ebp]
	add	ecx, 1
	mov	DWORD PTR _nHash$77020[ebp], ecx
$L77021:
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nHash$77020[ebp]
	cmp	eax, DWORD PTR [edx+8]
	jae	SHORT $L77023

; 1459 : 			CAssoc* pAssoc;
; 1460 : 			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR _nHash$77020[ebp]
	mov	ecx, DWORD PTR [edx+eax*4]
	mov	DWORD PTR _pAssoc$77024[ebp], ecx

; 1461 : 			  pAssoc = pAssoc->pNext)

	jmp	SHORT $L77025
$L77026:
	mov	edx, DWORD PTR _pAssoc$77024[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _pAssoc$77024[ebp], eax
$L77025:
	cmp	DWORD PTR _pAssoc$77024[ebp], 0
	je	SHORT $L77027

; 1463 : 				SerializeElements<KEY>(ar, &pAssoc->key, 1);

	push	1
	mov	ecx, DWORD PTR _pAssoc$77024[ebp]
	add	ecx, 8
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAPAUHICON__@@H@Z ; SerializeElements

; 1464 : 				SerializeElements<VALUE>(ar, &pAssoc->value, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$77024[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	call	?SerializeElements@@YGXAAVCArchive@@PAPAUHBITMAP__@@H@Z ; SerializeElements

; 1465 : 			}

	jmp	SHORT $L77026
$L77027:

; 1466 : 		}

	jmp	SHORT $L77022
$L77023:

; 1468 : 	else

	jmp	SHORT $L77043
$L77014:

; 1470 : 		DWORD nNewCount = ar.ReadCount();

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?ReadCount@CArchive@@QAEKXZ		; CArchive::ReadCount
	mov	DWORD PTR _nNewCount$77040[ebp], eax
$L77042:

; 1471 : 		while (nNewCount--)

	mov	edx, DWORD PTR _nNewCount$77040[ebp]
	mov	eax, DWORD PTR _nNewCount$77040[ebp]
	sub	eax, 1
	mov	DWORD PTR _nNewCount$77040[ebp], eax
	test	edx, edx
	je	SHORT $L77043

; 1473 : 			KEY newKey;
; 1474 : 			VALUE newValue;
; 1475 : 			SerializeElements<KEY>(ar, &newKey, 1);

	push	1
	lea	ecx, DWORD PTR _newKey$77044[ebp]
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAPAUHICON__@@H@Z ; SerializeElements

; 1476 : 			SerializeElements<VALUE>(ar, &newValue, 1);

	push	1
	lea	eax, DWORD PTR _newValue$77045[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	call	?SerializeElements@@YGXAAVCArchive@@PAPAUHBITMAP__@@H@Z ; SerializeElements

; 1477 : 			SetAt(newKey, newValue);

	mov	edx, DWORD PTR _newValue$77045[ebp]
	push	edx
	mov	eax, DWORD PTR _newKey$77044[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetAt@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXPAUHICON__@@PAUHBITMAP__@@@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::SetAt

; 1478 : 		}

	jmp	SHORT $L77042
$L77043:
$L77013:

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
?Serialize@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAEXAAVCArchive@@@Z ENDP ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::Serialize
_TEXT	ENDS
PUBLIC	?DumpElements@@YGXAAVCDumpContext@@PBQAUHBITMAP__@@H@Z ; DumpElements
;	COMDAT ?Dump@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT
_dc$ = 8
_this$ = -16
_key$77061 = -8
_val$77062 = -4
_pos$77063 = -12
?Dump@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UBEXAAVCDumpContext@@@Z PROC NEAR ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::Dump, COMDAT

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
	jle	SHORT $L77066

; 1491 : 		// Dump in format "[key] -> value"
; 1492 : 		KEY key;
; 1493 : 		VALUE val;
; 1494 : 
; 1495 : 		POSITION pos = GetStartPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetStartPosition@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEPAU__POSITION@@XZ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::GetStartPosition
	mov	DWORD PTR _pos$77063[ebp], eax
$L77065:

; 1496 : 		while (pos != NULL)

	cmp	DWORD PTR _pos$77063[ebp], 0
	je	SHORT $L77066

; 1498 : 			GetNextAssoc(pos, key, val);

	lea	eax, DWORD PTR _val$77062[ebp]
	push	eax
	lea	ecx, DWORD PTR _key$77061[ebp]
	push	ecx
	lea	edx, DWORD PTR _pos$77063[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetNextAssoc@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QBEXAAPAU__POSITION@@AAPAUHICON__@@AAPAUHBITMAP__@@@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::GetNextAssoc

; 1499 : 			dc << "\n\t[";

	push	OFFSET FLAT:??_C@_03DANH@?6?7?$FL?$AA@	; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1500 : 			DumpElements<KEY>(dc, &key, 1);

	push	1
	lea	eax, DWORD PTR _key$77061[ebp]
	push	eax
	mov	ecx, DWORD PTR _dc$[ebp]
	push	ecx
	call	?DumpElements@@YGXAAVCDumpContext@@PBQAUHICON__@@H@Z ; DumpElements

; 1501 : 			dc << "] = ";

	push	OFFSET FLAT:??_C@_04MNLE@?$FN?5?$DN?5?$AA@ ; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1502 : 			DumpElements<VALUE>(dc, &val, 1);

	push	1
	lea	edx, DWORD PTR _val$77062[ebp]
	push	edx
	mov	eax, DWORD PTR _dc$[ebp]
	push	eax
	call	?DumpElements@@YGXAAVCDumpContext@@PBQAUHBITMAP__@@H@Z ; DumpElements

; 1503 : 		}

	jmp	SHORT $L77065
$L77066:

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
?Dump@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UBEXAAVCDumpContext@@@Z ENDP ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::Dump
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UBEXXZ
_TEXT	SEGMENT
_this$ = -4
?AssertValid@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UBEXXZ PROC NEAR ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::AssertValid, COMDAT

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
$L77081:

; 1513 : 
; 1514 : 	ASSERT(m_nHashTableSize > 0);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	ja	SHORT $L77084
	push	1514					; 000005eaH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77084
	int	3
$L77084:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L77081
$L77083:

; 1515 : 	ASSERT(m_nCount == 0 || m_pHashTable != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+12], 0
	je	SHORT $L77088
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L77088
	push	1515					; 000005ebH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77088
	int	3
$L77088:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L77083

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
?AssertValid@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UBEXXZ ENDP ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::AssertValid
_TEXT	ENDS
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_G?$CMap@IIPAUHICON__@@PAU1@@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_G?$CMap@IIPAUHICON__@@PAU1@@@UAEPAXI@Z PROC NEAR	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1?$CMap@IIPAUHICON__@@PAU1@@@UAE@XZ	; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::~CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L77093
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L77093:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_G?$CMap@IIPAUHICON__@@PAU1@@@UAEPAXI@Z ENDP		; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_G?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAEPAXI@Z PROC NEAR ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAE@XZ ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::~CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L77096
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L77096:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_G?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@UAEPAXI@Z ENDP ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@IIPAUHICON__@@PAU1@@@QAEXIPAUHICON__@@@Z
_TEXT	SEGMENT
_key$ = 8
_newValue$ = 12
_this$ = -4
?SetAt@?$CMap@IIPAUHICON__@@PAU1@@@QAEXIPAUHICON__@@@Z PROC NEAR ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::SetAt, COMDAT

; 1188 : 	{ (*this)[key] = newValue; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _key$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??A?$CMap@IIPAUHICON__@@PAU1@@@QAEAAPAUHICON__@@I@Z ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::operator[]
	mov	ecx, DWORD PTR _newValue$[ebp]
	mov	DWORD PTR [eax], ecx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SetAt@?$CMap@IIPAUHICON__@@PAU1@@@QAEXIPAUHICON__@@@Z ENDP ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::SetAt
_TEXT	ENDS
EXTRN	_memset:NEAR
EXTRN	??2@YAPAXIPBDH@Z:NEAR				; operator new
;	COMDAT ?InitHashTable@?$CMap@IIPAUHICON__@@PAU1@@@QAEXIH@Z
_TEXT	SEGMENT
_nHashSize$ = 8
_bAllocNow$ = 12
_this$ = -12
$T77809 = -4
$T77810 = -8
?InitHashTable@?$CMap@IIPAUHICON__@@PAU1@@@QAEXIH@Z PROC NEAR ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::InitHashTable, COMDAT

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
$L77107:

; 1220 : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L77110
	push	1220					; 000004c4H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77110
	int	3
$L77110:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77107
$L77109:

; 1221 : 	ASSERT(nHashSize > 0);

	cmp	DWORD PTR _nHashSize$[ebp], 0
	ja	SHORT $L77114
	push	1221					; 000004c5H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77114
	int	3
$L77114:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L77109

; 1222 : 
; 1223 : 	if (m_pHashTable != NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L77115

; 1225 : 		// free hash table
; 1226 : 		delete[] m_pHashTable;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR $T77809[ebp], eax
	mov	ecx, DWORD PTR $T77809[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 1227 : 		m_pHashTable = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0
$L77115:

; 1229 : 
; 1230 : 	if (bAllocNow)

	cmp	DWORD PTR _bAllocNow$[ebp], 0
	je	SHORT $L77117

; 1232 : 		m_pHashTable = new CAssoc* [nHashSize];

	push	1232					; 000004d0H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _nHashSize$[ebp]
	shl	eax, 2
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77810[ebp], eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR $T77810[ebp]
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
$L77117:

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
?InitHashTable@?$CMap@IIPAUHICON__@@PAU1@@@QAEXIH@Z ENDP ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::InitHashTable
_TEXT	ENDS
PUBLIC	?ConstructElements@@YGXPAIH@Z			; ConstructElements
PUBLIC	?ConstructElements@@YGXPAPAUHICON__@@H@Z	; ConstructElements
PUBLIC	?data@CPlex@@QAEPAXXZ				; CPlex::data
EXTRN	?Create@CPlex@@SGPAU1@AAPAU1@II@Z:NEAR		; CPlex::Create
;	COMDAT ?NewAssoc@?$CMap@IIPAUHICON__@@PAU1@@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT
_this$ = -20
_newBlock$77125 = -8
_pAssoc$77127 = -16
_i$77129 = -12
_pAssoc$ = -4
?NewAssoc@?$CMap@IIPAUHICON__@@PAU1@@@IAEPAUCAssoc@1@XZ PROC NEAR ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::NewAssoc, COMDAT

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
	jne	SHORT $L77132

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
	mov	DWORD PTR _newBlock$77125[ebp], eax

; 1283 : 		// chain them into free list
; 1284 : 		CMap::CAssoc* pAssoc = (CMap::CAssoc*) newBlock->data();

	mov	ecx, DWORD PTR _newBlock$77125[ebp]
	call	?data@CPlex@@QAEPAXXZ			; CPlex::data
	mov	DWORD PTR _pAssoc$77127[ebp], eax

; 1285 : 		// free in reverse order to make it easier to debug
; 1286 : 		pAssoc += m_nBlockSize - 1;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	shl	edx, 4
	mov	eax, DWORD PTR _pAssoc$77127[ebp]
	add	eax, edx
	mov	DWORD PTR _pAssoc$77127[ebp], eax

; 1287 : 		for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	mov	DWORD PTR _i$77129[ebp], edx
	jmp	SHORT $L77130
$L77131:
	mov	eax, DWORD PTR _i$77129[ebp]
	sub	eax, 1
	mov	DWORD PTR _i$77129[ebp], eax
	mov	ecx, DWORD PTR _pAssoc$77127[ebp]
	sub	ecx, 16					; 00000010H
	mov	DWORD PTR _pAssoc$77127[ebp], ecx
$L77130:
	cmp	DWORD PTR _i$77129[ebp], 0
	jl	SHORT $L77132

; 1289 : 			pAssoc->pNext = m_pFreeList;

	mov	edx, DWORD PTR _pAssoc$77127[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR [edx], ecx

; 1290 : 			m_pFreeList = pAssoc;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _pAssoc$77127[ebp]
	mov	DWORD PTR [edx+16], eax

; 1291 : 		}

	jmp	SHORT $L77131
$L77132:

; 1293 : 	ASSERT(m_pFreeList != NULL);  // we must have something

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+16], 0
	jne	SHORT $L77136
	push	1293					; 0000050dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77136
	int	3
$L77136:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77132

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
$L77138:

; 1298 : 	ASSERT(m_nCount > 0);  // make sure we don't overflow

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jg	SHORT $L77141
	push	1298					; 00000512H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77141
	int	3
$L77141:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L77138

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
	call	?ConstructElements@@YGXPAPAUHICON__@@H@Z ; ConstructElements

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
?NewAssoc@?$CMap@IIPAUHICON__@@PAU1@@@IAEPAUCAssoc@1@XZ ENDP ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::NewAssoc
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
;	COMDAT ?FreeAssoc@?$CMap@IIPAUHICON__@@PAU1@@@IAEXPAUCAssoc@1@@Z
_TEXT	SEGMENT
_pAssoc$ = 8
_this$ = -4
?FreeAssoc@?$CMap@IIPAUHICON__@@PAU1@@@IAEXPAUCAssoc@1@@Z PROC NEAR ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::FreeAssoc, COMDAT

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
	call	?DestructElements@@YGXPAPAUHICON__@@H@Z	; DestructElements

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
$L77164:

; 1312 : 	ASSERT(m_nCount >= 0);  // make sure we don't underflow

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jge	SHORT $L77167
	push	1312					; 00000520H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77167
	int	3
$L77167:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77164

; 1313 : 
; 1314 : 	// if no more elements, cleanup completely
; 1315 : 	if (m_nCount == 0)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jne	SHORT $L77168

; 1316 : 		RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CMap@IIPAUHICON__@@PAU1@@@QAEXXZ ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::RemoveAll
$L77168:

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
?FreeAssoc@?$CMap@IIPAUHICON__@@PAU1@@@IAEXPAUCAssoc@1@@Z ENDP ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::FreeAssoc
_TEXT	ENDS
;	COMDAT ?GetAssocAt@?$CMap@IIPAUHICON__@@PAU1@@@IBEPAUCAssoc@1@IAAI@Z
_TEXT	SEGMENT
_key$ = 8
_nHash$ = 12
_this$ = -8
_pAssoc$ = -4
?GetAssocAt@?$CMap@IIPAUHICON__@@PAU1@@@IBEPAUCAssoc@1@IAAI@Z PROC NEAR ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::GetAssocAt, COMDAT

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
	jne	SHORT $L77177

; 1327 : 		return NULL;

	xor	eax, eax
	jmp	SHORT $L77173
$L77177:

; 1331 : 	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)

	mov	edx, DWORD PTR _nHash$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR [edx+eax*4]
	mov	DWORD PTR _pAssoc$[ebp], eax
	jmp	SHORT $L77179
$L77180:
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _pAssoc$[ebp], edx
$L77179:
	cmp	DWORD PTR _pAssoc$[ebp], 0
	je	SHORT $L77181

; 1333 : 		if (CompareElements(&pAssoc->key, &key))

	lea	eax, DWORD PTR _key$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	add	ecx, 8
	push	ecx
	call	?CompareElements@@YGHPBI0@Z		; CompareElements
	test	eax, eax
	je	SHORT $L77187

; 1334 : 			return pAssoc;

	mov	eax, DWORD PTR _pAssoc$[ebp]
	jmp	SHORT $L77173
$L77187:

; 1335 : 	}

	jmp	SHORT $L77180
$L77181:

; 1336 : 	return NULL;

	xor	eax, eax
$L77173:

; 1337 : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?GetAssocAt@?$CMap@IIPAUHICON__@@PAU1@@@IBEPAUCAssoc@1@IAAI@Z ENDP ; CMap<unsigned int,unsigned int,HICON__ *,HICON__ *>::GetAssocAt
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
$L77193:

; 119  : 	ASSERT(AfxIsValidAddress(pElement1, sizeof(TYPE), FALSE));

	push	0
	push	4
	mov	eax, DWORD PTR _pElement1$[ebp]
	push	eax
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77197
	push	119					; 00000077H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77197
	int	3
$L77197:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L77193
$L77195:

; 120  : 	ASSERT(AfxIsValidAddress(pElement2, sizeof(ARG_TYPE), FALSE));

	push	0
	push	4
	mov	edx, DWORD PTR _pElement2$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77202
	push	120					; 00000078H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77202
	int	3
$L77202:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L77195

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
;	COMDAT ?DestructElements@@YGXPAPAUHICON__@@H@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
?DestructElements@@YGXPAPAUHICON__@@H@Z PROC NEAR	; DestructElements, COMDAT

; 67   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L77204:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77208
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77208
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77208
	int	3
$L77208:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77204

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L77209
$L77210:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 4
	mov	DWORD PTR _pElements$[ebp], eax
$L77209:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L77211

; 73   : 		pElements->~TYPE();

	jmp	SHORT $L77210
$L77211:

; 74   : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?DestructElements@@YGXPAPAUHICON__@@H@Z ENDP		; DestructElements
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
$L77213:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77217
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77217
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77217
	int	3
$L77217:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77213

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L77218
$L77219:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 4
	mov	DWORD PTR _pElements$[ebp], eax
$L77218:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L77220

; 73   : 		pElements->~TYPE();

	jmp	SHORT $L77219
$L77220:

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
EXTRN	?Read@CArchive@@QAEIPAXI@Z:NEAR			; CArchive::Read
EXTRN	?Write@CArchive@@QAEXPBXI@Z:NEAR		; CArchive::Write
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
$L77222:

; 92   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77226
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77226
	push	93					; 0000005dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77226
	int	3
$L77226:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77222

; 93   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 94   : 
; 95   : 	// default is bit-wise read/write
; 96   : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L77227

; 97   : 		ar.Write((void*)pElements, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Write@CArchive@@QAEXPBXI@Z		; CArchive::Write

; 98   : 	else

	jmp	SHORT $L77230
$L77227:

; 99   : 		ar.Read((void*)pElements, nCount * sizeof(TYPE));

	mov	edx, DWORD PTR _nCount$[ebp]
	shl	edx, 2
	push	edx
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Read@CArchive@@QAEIPAXI@Z		; CArchive::Read
$L77230:

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
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAPAUHICON__@@H@Z
_TEXT	SEGMENT
_ar$ = 8
_pElements$ = 12
_nCount$ = 16
?SerializeElements@@YGXAAVCArchive@@PAPAUHICON__@@H@Z PROC NEAR ; SerializeElements, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L77234:

; 92   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77238
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77238
	push	93					; 0000005dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77238
	int	3
$L77238:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77234

; 93   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 94   : 
; 95   : 	// default is bit-wise read/write
; 96   : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L77239

; 97   : 		ar.Write((void*)pElements, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Write@CArchive@@QAEXPBXI@Z		; CArchive::Write

; 98   : 	else

	jmp	SHORT $L77242
$L77239:

; 99   : 		ar.Read((void*)pElements, nCount * sizeof(TYPE));

	mov	edx, DWORD PTR _nCount$[ebp]
	shl	edx, 2
	push	edx
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Read@CArchive@@QAEIPAXI@Z		; CArchive::Read
$L77242:

; 100  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	12					; 0000000cH
?SerializeElements@@YGXAAVCArchive@@PAPAUHICON__@@H@Z ENDP ; SerializeElements
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
$L77246:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77250
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77250
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77250
	int	3
$L77250:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77246

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
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBQAUHICON__@@H@Z
_TEXT	SEGMENT
_pElements$ = 12
_nCount$ = 16
?DumpElements@@YGXAAVCDumpContext@@PBQAUHICON__@@H@Z PROC NEAR ; DumpElements, COMDAT

; 105  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L77252:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77256
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77256
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77256
	int	3
$L77256:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77252

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
?DumpElements@@YGXAAVCDumpContext@@PBQAUHICON__@@H@Z ENDP ; DumpElements
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXPAUHICON__@@PAUHBITMAP__@@@Z
_TEXT	SEGMENT
_key$ = 8
_newValue$ = 12
_this$ = -4
?SetAt@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXPAUHICON__@@PAUHBITMAP__@@@Z PROC NEAR ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::SetAt, COMDAT

; 1188 : 	{ (*this)[key] = newValue; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _key$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??A?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEAAPAUHBITMAP__@@PAUHICON__@@@Z ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::operator[]
	mov	ecx, DWORD PTR _newValue$[ebp]
	mov	DWORD PTR [eax], ecx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SetAt@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXPAUHICON__@@PAUHBITMAP__@@@Z ENDP ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::SetAt
_TEXT	ENDS
;	COMDAT ?InitHashTable@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXIH@Z
_TEXT	SEGMENT
$T77839 = -4
$T77840 = -8
_nHashSize$ = 8
_bAllocNow$ = 12
_this$ = -12
?InitHashTable@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXIH@Z PROC NEAR ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::InitHashTable, COMDAT

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
$L77267:

; 1220 : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L77270
	push	1220					; 000004c4H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77270
	int	3
$L77270:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77267
$L77269:

; 1221 : 	ASSERT(nHashSize > 0);

	cmp	DWORD PTR _nHashSize$[ebp], 0
	ja	SHORT $L77274
	push	1221					; 000004c5H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77274
	int	3
$L77274:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L77269

; 1222 : 
; 1223 : 	if (m_pHashTable != NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L77275

; 1225 : 		// free hash table
; 1226 : 		delete[] m_pHashTable;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR $T77839[ebp], eax
	mov	ecx, DWORD PTR $T77839[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 1227 : 		m_pHashTable = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0
$L77275:

; 1229 : 
; 1230 : 	if (bAllocNow)

	cmp	DWORD PTR _bAllocNow$[ebp], 0
	je	SHORT $L77277

; 1232 : 		m_pHashTable = new CAssoc* [nHashSize];

	push	1232					; 000004d0H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _nHashSize$[ebp]
	shl	eax, 2
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77840[ebp], eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR $T77840[ebp]
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
$L77277:

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
?InitHashTable@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@QAEXIH@Z ENDP ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::InitHashTable
_TEXT	ENDS
PUBLIC	?ConstructElements@@YGXPAPAUHBITMAP__@@H@Z	; ConstructElements
;	COMDAT ?NewAssoc@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT
_this$ = -20
_newBlock$77285 = -8
_pAssoc$77287 = -16
_i$77289 = -12
_pAssoc$ = -4
?NewAssoc@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@IAEPAUCAssoc@1@XZ PROC NEAR ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::NewAssoc, COMDAT

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
	jne	SHORT $L77292

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
	mov	DWORD PTR _newBlock$77285[ebp], eax

; 1283 : 		// chain them into free list
; 1284 : 		CMap::CAssoc* pAssoc = (CMap::CAssoc*) newBlock->data();

	mov	ecx, DWORD PTR _newBlock$77285[ebp]
	call	?data@CPlex@@QAEPAXXZ			; CPlex::data
	mov	DWORD PTR _pAssoc$77287[ebp], eax

; 1285 : 		// free in reverse order to make it easier to debug
; 1286 : 		pAssoc += m_nBlockSize - 1;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	shl	edx, 4
	mov	eax, DWORD PTR _pAssoc$77287[ebp]
	add	eax, edx
	mov	DWORD PTR _pAssoc$77287[ebp], eax

; 1287 : 		for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	mov	DWORD PTR _i$77289[ebp], edx
	jmp	SHORT $L77290
$L77291:
	mov	eax, DWORD PTR _i$77289[ebp]
	sub	eax, 1
	mov	DWORD PTR _i$77289[ebp], eax
	mov	ecx, DWORD PTR _pAssoc$77287[ebp]
	sub	ecx, 16					; 00000010H
	mov	DWORD PTR _pAssoc$77287[ebp], ecx
$L77290:
	cmp	DWORD PTR _i$77289[ebp], 0
	jl	SHORT $L77292

; 1289 : 			pAssoc->pNext = m_pFreeList;

	mov	edx, DWORD PTR _pAssoc$77287[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR [edx], ecx

; 1290 : 			m_pFreeList = pAssoc;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _pAssoc$77287[ebp]
	mov	DWORD PTR [edx+16], eax

; 1291 : 		}

	jmp	SHORT $L77291
$L77292:

; 1293 : 	ASSERT(m_pFreeList != NULL);  // we must have something

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+16], 0
	jne	SHORT $L77296
	push	1293					; 0000050dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77296
	int	3
$L77296:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77292

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
$L77298:

; 1298 : 	ASSERT(m_nCount > 0);  // make sure we don't overflow

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jg	SHORT $L77301
	push	1298					; 00000512H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77301
	int	3
$L77301:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L77298

; 1299 : 	ConstructElements<KEY>(&pAssoc->key, 1);

	push	1
	mov	edx, DWORD PTR _pAssoc$[ebp]
	add	edx, 8
	push	edx
	call	?ConstructElements@@YGXPAPAUHICON__@@H@Z ; ConstructElements

; 1300 : 	ConstructElements<VALUE>(&pAssoc->value, 1);   // special construct values

	push	1
	mov	eax, DWORD PTR _pAssoc$[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	call	?ConstructElements@@YGXPAPAUHBITMAP__@@H@Z ; ConstructElements

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
?NewAssoc@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@IAEPAUCAssoc@1@XZ ENDP ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::NewAssoc
_TEXT	ENDS
PUBLIC	?HashKey@@YGIPAUHICON__@@@Z			; HashKey
PUBLIC	?CompareElements@@YGHPBQAUHICON__@@0@Z		; CompareElements
;	COMDAT ?GetAssocAt@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@IBEPAUCAssoc@1@PAUHICON__@@AAI@Z
_TEXT	SEGMENT
_key$ = 8
_nHash$ = 12
_this$ = -8
_pAssoc$ = -4
?GetAssocAt@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@IBEPAUCAssoc@1@PAUHICON__@@AAI@Z PROC NEAR ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::GetAssocAt, COMDAT

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
	call	?HashKey@@YGIPAUHICON__@@@Z		; HashKey
	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	div	DWORD PTR [ecx+8]
	mov	eax, DWORD PTR _nHash$[ebp]
	mov	DWORD PTR [eax], edx

; 1325 : 
; 1326 : 	if (m_pHashTable == NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	jne	SHORT $L77320

; 1327 : 		return NULL;

	xor	eax, eax
	jmp	SHORT $L77315
$L77320:

; 1331 : 	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)

	mov	edx, DWORD PTR _nHash$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR [edx+eax*4]
	mov	DWORD PTR _pAssoc$[ebp], eax
	jmp	SHORT $L77322
$L77323:
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _pAssoc$[ebp], edx
$L77322:
	cmp	DWORD PTR _pAssoc$[ebp], 0
	je	SHORT $L77324

; 1333 : 		if (CompareElements(&pAssoc->key, &key))

	lea	eax, DWORD PTR _key$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	add	ecx, 8
	push	ecx
	call	?CompareElements@@YGHPBQAUHICON__@@0@Z	; CompareElements
	test	eax, eax
	je	SHORT $L77331

; 1334 : 			return pAssoc;

	mov	eax, DWORD PTR _pAssoc$[ebp]
	jmp	SHORT $L77315
$L77331:

; 1335 : 	}

	jmp	SHORT $L77323
$L77324:

; 1336 : 	return NULL;

	xor	eax, eax
$L77315:

; 1337 : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?GetAssocAt@?$CMap@PAUHICON__@@PAU1@PAUHBITMAP__@@PAU2@@@IBEPAUCAssoc@1@PAUHICON__@@AAI@Z ENDP ; CMap<HICON__ *,HICON__ *,HBITMAP__ *,HBITMAP__ *>::GetAssocAt
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAPAUHBITMAP__@@H@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
?DestructElements@@YGXPAPAUHBITMAP__@@H@Z PROC NEAR	; DestructElements, COMDAT

; 67   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L77333:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77337
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77337
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77337
	int	3
$L77337:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77333

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L77338
$L77339:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 4
	mov	DWORD PTR _pElements$[ebp], eax
$L77338:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L77340

; 73   : 		pElements->~TYPE();

	jmp	SHORT $L77339
$L77340:

; 74   : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?DestructElements@@YGXPAPAUHBITMAP__@@H@Z ENDP		; DestructElements
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAPAUHBITMAP__@@H@Z
_TEXT	SEGMENT
_ar$ = 8
_pElements$ = 12
_nCount$ = 16
?SerializeElements@@YGXAAVCArchive@@PAPAUHBITMAP__@@H@Z PROC NEAR ; SerializeElements, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L77342:

; 92   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77346
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77346
	push	93					; 0000005dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77346
	int	3
$L77346:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77342

; 93   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 94   : 
; 95   : 	// default is bit-wise read/write
; 96   : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L77347

; 97   : 		ar.Write((void*)pElements, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Write@CArchive@@QAEXPBXI@Z		; CArchive::Write

; 98   : 	else

	jmp	SHORT $L77350
$L77347:

; 99   : 		ar.Read((void*)pElements, nCount * sizeof(TYPE));

	mov	edx, DWORD PTR _nCount$[ebp]
	shl	edx, 2
	push	edx
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Read@CArchive@@QAEIPAXI@Z		; CArchive::Read
$L77350:

; 100  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	12					; 0000000cH
?SerializeElements@@YGXAAVCArchive@@PAPAUHBITMAP__@@H@Z ENDP ; SerializeElements
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBQAUHBITMAP__@@H@Z
_TEXT	SEGMENT
_pElements$ = 12
_nCount$ = 16
?DumpElements@@YGXAAVCDumpContext@@PBQAUHBITMAP__@@H@Z PROC NEAR ; DumpElements, COMDAT

; 105  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L77354:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77358
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77358
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77358
	int	3
$L77358:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77354

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
?DumpElements@@YGXAAVCDumpContext@@PBQAUHBITMAP__@@H@Z ENDP ; DumpElements
_TEXT	ENDS
PUBLIC	??2@YAPAXIPAX@Z					; operator new
;	COMDAT ?ConstructElements@@YGXPAIH@Z
_TEXT	SEGMENT
$T77853 = -4
_pElements$ = 8
_nCount$ = 12
?ConstructElements@@YGXPAIH@Z PROC NEAR			; ConstructElements, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L77360:

; 54   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77364
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77364
	push	55					; 00000037H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77364
	int	3
$L77364:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77360

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

	jmp	SHORT $L77367
$L77368:
	mov	edx, DWORD PTR _pElements$[ebp]
	add	edx, 4
	mov	DWORD PTR _pElements$[ebp], edx
$L77367:
	mov	eax, DWORD PTR _nCount$[ebp]
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
	test	eax, eax
	je	SHORT $L77369

; 62   : 		::new((void*)pElements) TYPE;

	mov	edx, DWORD PTR _pElements$[ebp]
	push	edx
	push	4
	call	??2@YAPAXIPAX@Z				; operator new
	add	esp, 8
	mov	DWORD PTR $T77853[ebp], eax
	jmp	SHORT $L77368
$L77369:

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
;	COMDAT ?ConstructElements@@YGXPAPAUHICON__@@H@Z
_TEXT	SEGMENT
$T77858 = -4
_pElements$ = 8
_nCount$ = 12
?ConstructElements@@YGXPAPAUHICON__@@H@Z PROC NEAR	; ConstructElements, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L77374:

; 54   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77378
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77378
	push	55					; 00000037H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77378
	int	3
$L77378:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77374

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

	jmp	SHORT $L77381
$L77382:
	mov	edx, DWORD PTR _pElements$[ebp]
	add	edx, 4
	mov	DWORD PTR _pElements$[ebp], edx
$L77381:
	mov	eax, DWORD PTR _nCount$[ebp]
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
	test	eax, eax
	je	SHORT $L77383

; 62   : 		::new((void*)pElements) TYPE;

	mov	edx, DWORD PTR _pElements$[ebp]
	push	edx
	push	4
	call	??2@YAPAXIPAX@Z				; operator new
	add	esp, 8
	mov	DWORD PTR $T77858[ebp], eax
	jmp	SHORT $L77382
$L77383:

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
?ConstructElements@@YGXPAPAUHICON__@@H@Z ENDP		; ConstructElements
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAPAUHBITMAP__@@H@Z
_TEXT	SEGMENT
$T77861 = -4
_pElements$ = 8
_nCount$ = 12
?ConstructElements@@YGXPAPAUHBITMAP__@@H@Z PROC NEAR	; ConstructElements, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L77388:

; 54   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L77392
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77392
	push	55					; 00000037H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77392
	int	3
$L77392:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77388

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

	jmp	SHORT $L77395
$L77396:
	mov	edx, DWORD PTR _pElements$[ebp]
	add	edx, 4
	mov	DWORD PTR _pElements$[ebp], edx
$L77395:
	mov	eax, DWORD PTR _nCount$[ebp]
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
	test	eax, eax
	je	SHORT $L77397

; 62   : 		::new((void*)pElements) TYPE;

	mov	edx, DWORD PTR _pElements$[ebp]
	push	edx
	push	4
	call	??2@YAPAXIPAX@Z				; operator new
	add	esp, 8
	mov	DWORD PTR $T77861[ebp], eax
	jmp	SHORT $L77396
$L77397:

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
?ConstructElements@@YGXPAPAUHBITMAP__@@H@Z ENDP		; ConstructElements
_TEXT	ENDS
;	COMDAT ?HashKey@@YGIPAUHICON__@@@Z
_TEXT	SEGMENT
_key$ = 8
?HashKey@@YGIPAUHICON__@@@Z PROC NEAR			; HashKey, COMDAT

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
?HashKey@@YGIPAUHICON__@@@Z ENDP			; HashKey
_TEXT	ENDS
;	COMDAT ?CompareElements@@YGHPBQAUHICON__@@0@Z
_TEXT	SEGMENT
_pElement1$ = 8
_pElement2$ = 12
?CompareElements@@YGHPBQAUHICON__@@0@Z PROC NEAR	; CompareElements, COMDAT

; 118  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L77406:

; 119  : 	ASSERT(AfxIsValidAddress(pElement1, sizeof(TYPE), FALSE));

	push	0
	push	4
	mov	eax, DWORD PTR _pElement1$[ebp]
	push	eax
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77410
	push	119					; 00000077H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77410
	int	3
$L77410:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L77406
$L77408:

; 120  : 	ASSERT(AfxIsValidAddress(pElement2, sizeof(ARG_TYPE), FALSE));

	push	0
	push	4
	mov	edx, DWORD PTR _pElement2$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L77415
	push	120					; 00000078H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77415
	int	3
$L77415:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L77408

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
?CompareElements@@YGHPBQAUHICON__@@0@Z ENDP		; CompareElements
_TEXT	ENDS
END

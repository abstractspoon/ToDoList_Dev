	TITLE	D:\_CODE\Shared\ToolbarHelper.cpp
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
;	COMDAT ??_C@_0EJ@KGBI@C?3?2Program?5Files?5?$CIx86?$CJ?2Microsoft@
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
;	COMDAT ??0CSubclasser@@IAE@XZ
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
;	COMDAT ?ScHookWindow@CSubclasser@@IAEHPAUHWND__@@@Z
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
;	COMDAT ?AtlA2WHelper@@YGPAGPAGPBDHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AtlW2AHelper@@YGPADPADPBGHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?A2WBSTR@@YAPAGPBDH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CCmdUITH@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Enable@CCmdUITH@@UAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetCheck@CCmdUITH@@UAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetRadio@CCmdUITH@@UAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetText@CCmdUITH@@UAEXPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CToolbarHelper@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCToolbarHelper@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CSubclasser@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CToolbarHelper@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Initialize@CToolbarHelper@@QAEHPAVCToolBar@@PAVCWnd@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Release@CToolbarHelper@@QAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetDropButton@CToolbarHelper@@QAEHIIHID@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetTooltip@CToolbarHelper@@QAEHII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetTooltip@CToolbarHelper@@QAEHIPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetButton@CToolbarHelper@@IAEHIIHIGPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ClearDropButton@CToolbarHelper@@QAEHIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetDefaultMenuID@CToolbarHelper@@QAEHII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DisplayDropMenu@CToolbarHelper@@IAEHIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E281
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScWindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InitTooltips@CToolbarHelper@@IAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RefreshTooltipRects@CToolbarHelper@@IAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PrepareMenuItems@CToolbarHelper@@SAXPAVCMenu@@PAVCWnd@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsCmdEnabled@CToolbarHelper@@IBEHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTip@CToolbarHelper@@IBE?AVCString@@IPAUtagPOINT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTip@CToolbarHelper@@SA?AVCString@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EnableMultilineText@CToolbarHelper@@QAEXHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessMessage@CToolbarHelper@@QAEHPAUtagMSG@@@Z
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
;	COMDAT ??0?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lookup@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEHIAAUTHButton@CToolbarHelper@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEAAUTHButton@CToolbarHelper@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetStartPosition@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNextAssoc@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEXAAPAU__POSITION@@AAIAAUTHButton@CToolbarHelper@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXIAAUTHButton@CToolbarHelper@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InitHashTable@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewAssoc@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAssocAt@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@IBEPAUCAssoc@1@IAAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAUTHButton@CToolbarHelper@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAUTHButton@CToolbarHelper@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBUTHButton@CToolbarHelper@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAUTHButton@CToolbarHelper@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HashKey@@YGII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CompareElements@@YGHPBI0@Z
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
;	COMDAT ??_7?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CToolbarHelper@@6BCSubclasser@@@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CToolbarHelper@@6BCSubclassWnd@@@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CCmdUITH@@6B@
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
_THIS_FILE DB	'D:\_CODE\Shared\ToolbarHelper.cpp', 00H
_DATA	ENDS
PUBLIC	??0CToolbarHelper@@QAE@XZ			; CToolbarHelper::CToolbarHelper
PUBLIC	??0?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAE@H@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>
PUBLIC	?IsValid@CSubclassWnd@@UBEHXZ			; CSubclassWnd::IsValid
PUBLIC	??1?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAE@XZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::~CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>
PUBLIC	?PreDetachWindow@CSubclassWnd@@MAEXXZ		; CSubclassWnd::PreDetachWindow
PUBLIC	?PostDetachWindow@CSubclassWnd@@MAEXXZ		; CSubclassWnd::PostDetachWindow
PUBLIC	?IsHooked@CSubclassWnd@@MBEHXZ			; CSubclassWnd::IsHooked
PUBLIC	?IsValidHook@CSubclassWnd@@MBEHXZ		; CSubclassWnd::IsValidHook
PUBLIC	?WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z ; CToolbarHelper::WindowProc
PUBLIC	?Redraw@CSubclassWnd@@MBEXXZ			; CSubclassWnd::Redraw
PUBLIC	?ScWindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z ; CToolbarHelper::ScWindowProc
PUBLIC	??_7CToolbarHelper@@6BCSubclassWnd@@@		; CToolbarHelper::`vftable'
PUBLIC	??_7CToolbarHelper@@6BCSubclasser@@@		; CToolbarHelper::`vftable'
PUBLIC	??_GCToolbarHelper@@UAEPAXI@Z			; CToolbarHelper::`scalar deleting destructor'
PUBLIC	??_ECToolbarHelper@@UAEPAXI@Z			; CToolbarHelper::`vector deleting destructor'
PUBLIC	??0CSubclasser@@IAE@XZ				; CSubclasser::CSubclasser
PUBLIC	?GetTopSubclasser@CSubclasser@@MAEPAV1@XZ	; CSubclasser::GetTopSubclasser
PUBLIC	?ScPreDetachWindow@CSubclasser@@MAEXXZ		; CSubclasser::ScPreDetachWindow
PUBLIC	?ScPostDetachWindow@CSubclasser@@MAEXXZ		; CSubclasser::ScPostDetachWindow
PUBLIC	??1CSubclasser@@QAE@XZ				; CSubclasser::~CSubclasser
EXTRN	??0CSubclassWnd@@QAE@XZ:NEAR			; CSubclassWnd::CSubclassWnd
EXTRN	??1CSubclassWnd@@UAE@XZ:NEAR			; CSubclassWnd::~CSubclassWnd
EXTRN	?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z:NEAR ; CSubclassWnd::HookWindow
EXTRN	?PostMessageW@CSubclassWnd@@MBEHIIJ@Z:NEAR	; CSubclassWnd::PostMessageW
EXTRN	?SendMessageW@CSubclassWnd@@MBEHIIJ@Z:NEAR	; CSubclassWnd::SendMessageW
EXTRN	??0CToolTipCtrl@@QAE@XZ:NEAR			; CToolTipCtrl::CToolTipCtrl
EXTRN	__except_list:DWORD
EXTRN	__chkesp:NEAR
EXTRN	___CxxFrameHandler:NEAR
;	COMDAT ??_7CToolbarHelper@@6BCSubclassWnd@@@
; File D:\_CODE\Shared\ToolbarHelper.cpp
CONST	SEGMENT
??_7CToolbarHelper@@6BCSubclassWnd@@@ DD FLAT:??_ECToolbarHelper@@UAEPAXI@Z ; CToolbarHelper::`vftable'
	DD	FLAT:?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z
	DD	FLAT:?IsValid@CSubclassWnd@@UBEHXZ
	DD	FLAT:?PreDetachWindow@CSubclassWnd@@MAEXXZ
	DD	FLAT:?PostDetachWindow@CSubclassWnd@@MAEXXZ
	DD	FLAT:?IsHooked@CSubclassWnd@@MBEHXZ
	DD	FLAT:?IsValidHook@CSubclassWnd@@MBEHXZ
	DD	FLAT:?Redraw@CSubclassWnd@@MBEXXZ
	DD	FLAT:?PostMessageW@CSubclassWnd@@MBEHIIJ@Z
	DD	FLAT:?SendMessageW@CSubclassWnd@@MBEHIIJ@Z
	DD	FLAT:?WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z
CONST	ENDS
;	COMDAT ??_7CToolbarHelper@@6BCSubclasser@@@
CONST	SEGMENT
??_7CToolbarHelper@@6BCSubclasser@@@ DD FLAT:?ScWindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z ; CToolbarHelper::`vftable'
	DD	FLAT:?GetTopSubclasser@CSubclasser@@MAEPAV1@XZ
	DD	FLAT:?ScPreDetachWindow@CSubclasser@@MAEXXZ
	DD	FLAT:?ScPostDetachWindow@CSubclasser@@MAEXXZ
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76595	DD	019930520H
	DD	03H
	DD	FLAT:$T76599
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76599	DD	0ffffffffH
	DD	FLAT:$L76588
	DD	00H
	DD	FLAT:$L76589
	DD	01H
	DD	FLAT:$L76590
xdata$x	ENDS
;	COMDAT ??0CToolbarHelper@@QAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??0CToolbarHelper@@QAE@XZ PROC NEAR			; CToolbarHelper::CToolbarHelper, COMDAT

; 39   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76596
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CSubclassWnd@@QAE@XZ			; CSubclassWnd::CSubclassWnd
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	??0CSubclasser@@IAE@XZ			; CSubclasser::CSubclasser
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+68], 0
	push	10					; 0000000aH
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	??0?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAE@H@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+100], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+104], 200		; 000000c8H
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 108				; 0000006cH
	call	??0CToolTipCtrl@@QAE@XZ			; CToolTipCtrl::CToolTipCtrl
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CToolbarHelper@@6BCSubclassWnd@@@ ; CToolbarHelper::`vftable'
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+32], OFFSET FLAT:??_7CToolbarHelper@@6BCSubclasser@@@ ; CToolbarHelper::`vftable'

; 40   : 
; 41   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
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
$L76588:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CSubclassWnd@@UAE@XZ			; CSubclassWnd::~CSubclassWnd
	ret	0
$L76589:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	??1CSubclasser@@QAE@XZ			; CSubclasser::~CSubclasser
	ret	0
$L76590:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	??1?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAE@XZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::~CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>
	ret	0
$L76596:
	mov	eax, OFFSET FLAT:$T76595
	jmp	___CxxFrameHandler
text$x	ENDS
??0CToolbarHelper@@QAE@XZ ENDP				; CToolbarHelper::CToolbarHelper
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
PUBLIC	?ScWindowProc@CSubclasser@@MAEJPAUHWND__@@IIJ@Z	; CSubclasser::ScWindowProc
PUBLIC	??_7CSubclasser@@6B@				; CSubclasser::`vftable'
;	COMDAT ??_7CSubclasser@@6B@
; File D:\_CODE\Shared\Subclass.h
CONST	SEGMENT
??_7CSubclasser@@6B@ DD FLAT:?ScWindowProc@CSubclasser@@MAEJPAUHWND__@@IIJ@Z ; CSubclasser::`vftable'
	DD	FLAT:?GetTopSubclasser@CSubclasser@@MAEPAV1@XZ
	DD	FLAT:?ScPreDetachWindow@CSubclasser@@MAEXXZ
	DD	FLAT:?ScPostDetachWindow@CSubclasser@@MAEXXZ
CONST	ENDS
;	COMDAT ??0CSubclasser@@IAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0CSubclasser@@IAE@XZ PROC NEAR			; CSubclasser::CSubclasser, COMDAT

; 134  : 	CSubclasser() {}

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??0CSubclassWnd@@QAE@XZ			; CSubclassWnd::CSubclassWnd
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CSubclasser@@6B@ ; CSubclasser::`vftable'
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CSubclasser@@IAE@XZ ENDP				; CSubclasser::CSubclasser
_TEXT	ENDS
PUBLIC	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ		; CSubclassWnd::GetHwnd
PUBLIC	??_C@_0BL@KPKN@D?3?2_CODE?2Shared?2Subclass?4h?$AA@ ; `string'
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
;	COMDAT ??_C@_0BL@KPKN@D?3?2_CODE?2Shared?2Subclass?4h?$AA@
; File D:\_CODE\Shared\Subclass.h
_DATA	SEGMENT
??_C@_0BL@KPKN@D?3?2_CODE?2Shared?2Subclass?4h?$AA@ DB 'D:\_CODE\Shared\S'
	DB	'ubclass.h', 00H				; `string'
_DATA	ENDS
;	COMDAT ?ScWindowProc@CSubclasser@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT
_hRealWnd$ = 8
_msg$ = 12
_wp$ = 16
_lp$ = 20
_this$ = -4
?ScWindowProc@CSubclasser@@MAEJPAUHWND__@@IIJ@Z PROC NEAR ; CSubclasser::ScWindowProc, COMDAT

; 137  : 	{

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L73257:

; 138  : 		ASSERT(hRealWnd == m_subclass.GetHwnd()); 

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ	; CSubclassWnd::GetHwnd
	cmp	DWORD PTR _hRealWnd$[ebp], eax
	je	SHORT $L73260
	push	138					; 0000008aH
	push	OFFSET FLAT:??_C@_0BL@KPKN@D?3?2_CODE?2Shared?2Subclass?4h?$AA@ ; `string'
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73260
	int	3
$L73260:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73257

; 139  : 		return m_subclass.WindowProc(hRealWnd, msg, wp, lp); 

	mov	esi, esp
	mov	ecx, DWORD PTR _lp$[ebp]
	push	ecx
	mov	edx, DWORD PTR _wp$[ebp]
	push	edx
	mov	eax, DWORD PTR _msg$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hRealWnd$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	call	DWORD PTR [eax+40]
	cmp	esi, esp
	call	__chkesp

; 140  : 	}

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?ScWindowProc@CSubclasser@@MAEJPAUHWND__@@IIJ@Z ENDP	; CSubclasser::ScWindowProc
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
;	COMDAT ?GetTopSubclasser@CSubclasser@@MAEPAV1@XZ
_TEXT	SEGMENT
_this$ = -4
?GetTopSubclasser@CSubclasser@@MAEPAV1@XZ PROC NEAR	; CSubclasser::GetTopSubclasser, COMDAT

; 142  : 	virtual CSubclasser* GetTopSubclasser() { return this; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	0
?GetTopSubclasser@CSubclasser@@MAEPAV1@XZ ENDP		; CSubclasser::GetTopSubclasser
_TEXT	ENDS
;	COMDAT ?ScPreDetachWindow@CSubclasser@@MAEXXZ
_TEXT	SEGMENT
_this$ = -4
?ScPreDetachWindow@CSubclasser@@MAEXXZ PROC NEAR	; CSubclasser::ScPreDetachWindow, COMDAT

; 146  : 	virtual void ScPreDetachWindow() { }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esp, ebp
	pop	ebp
	ret	0
?ScPreDetachWindow@CSubclasser@@MAEXXZ ENDP		; CSubclasser::ScPreDetachWindow
_TEXT	ENDS
;	COMDAT ?ScPostDetachWindow@CSubclasser@@MAEXXZ
_TEXT	SEGMENT
_this$ = -4
?ScPostDetachWindow@CSubclasser@@MAEXXZ PROC NEAR	; CSubclasser::ScPostDetachWindow, COMDAT

; 147  : 	virtual void ScPostDetachWindow() { }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esp, ebp
	pop	ebp
	ret	0
?ScPostDetachWindow@CSubclasser@@MAEXXZ ENDP		; CSubclasser::ScPostDetachWindow
_TEXT	ENDS
PUBLIC	??1CToolbarHelper@@UAE@XZ			; CToolbarHelper::~CToolbarHelper
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCToolbarHelper@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCToolbarHelper@@UAEPAXI@Z PROC NEAR			; CToolbarHelper::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CToolbarHelper@@UAE@XZ		; CToolbarHelper::~CToolbarHelper
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75675
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75675:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCToolbarHelper@@UAEPAXI@Z ENDP			; CToolbarHelper::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CSubclasser@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CSubclasser@@QAE@XZ PROC NEAR			; CSubclasser::~CSubclasser, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??1CSubclassWnd@@UAE@XZ			; CSubclassWnd::~CSubclassWnd
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CSubclasser@@QAE@XZ ENDP				; CSubclasser::~CSubclasser
_TEXT	ENDS
EXTRN	??1CToolTipCtrl@@UAE@XZ:NEAR			; CToolTipCtrl::~CToolTipCtrl
;	COMDAT xdata$x
; File D:\_CODE\Shared\ToolbarHelper.cpp
xdata$x	SEGMENT
$T76646	DD	019930520H
	DD	03H
	DD	FLAT:$T76648
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76648	DD	0ffffffffH
	DD	FLAT:$L76639
	DD	00H
	DD	FLAT:$L76642
	DD	01H
	DD	FLAT:$L76643
xdata$x	ENDS
;	COMDAT ??1CToolbarHelper@@UAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??1CToolbarHelper@@UAE@XZ PROC NEAR			; CToolbarHelper::~CToolbarHelper, COMDAT

; 44   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76647
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CToolbarHelper@@6BCSubclassWnd@@@ ; CToolbarHelper::`vftable'
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+32], OFFSET FLAT:??_7CToolbarHelper@@6BCSubclasser@@@ ; CToolbarHelper::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 2

; 45   : 
; 46   : }

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 108				; 0000006cH
	call	??1CToolTipCtrl@@UAE@XZ			; CToolTipCtrl::~CToolTipCtrl
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	??1?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAE@XZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::~CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	cmp	DWORD PTR _this$[ebp], 0
	je	SHORT $L76637
	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 32					; 00000020H
	mov	DWORD PTR -20+[ebp], edx
	jmp	SHORT $L76638
$L76637:
	mov	DWORD PTR -20+[ebp], 0
$L76638:
	mov	ecx, DWORD PTR -20+[ebp]
	call	??1CSubclasser@@QAE@XZ			; CSubclasser::~CSubclasser
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CSubclassWnd@@UAE@XZ			; CSubclassWnd::~CSubclassWnd
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
$L76639:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CSubclassWnd@@UAE@XZ			; CSubclassWnd::~CSubclassWnd
	ret	0
$L76642:
	cmp	DWORD PTR _this$[ebp], 0
	je	$L76640
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 32					; 00000020H
	mov	DWORD PTR -20+[ebp], eax
	jmp	$L76641
$L76640:
	mov	DWORD PTR -20+[ebp], 0
$L76641:
	mov	ecx, DWORD PTR -20+[ebp]
	call	??1CSubclasser@@QAE@XZ			; CSubclasser::~CSubclasser
	ret	0
$L76643:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	??1?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAE@XZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::~CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>
	ret	0
$L76647:
	mov	eax, OFFSET FLAT:$T76646
	jmp	___CxxFrameHandler
text$x	ENDS
??1CToolbarHelper@@UAE@XZ ENDP				; CToolbarHelper::~CToolbarHelper
PUBLIC	?Initialize@CToolbarHelper@@QAEHPAVCToolBar@@PAVCWnd@@@Z ; CToolbarHelper::Initialize
PUBLIC	?InitTooltips@CToolbarHelper@@IAEXXZ		; CToolbarHelper::InitTooltips
EXTRN	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z:NEAR ; AfxAssertValidObject
EXTRN	??BCWnd@@QBEPAUHWND__@@XZ:NEAR			; CWnd::operator HWND__ *
;	COMDAT ?Initialize@CToolbarHelper@@QAEHPAVCToolBar@@PAVCWnd@@@Z
_TEXT	SEGMENT
_pToolbar$ = 8
_pToolbarParent$ = 12
_this$ = -4
?Initialize@CToolbarHelper@@QAEHPAVCToolBar@@PAVCWnd@@@Z PROC NEAR ; CToolbarHelper::Initialize, COMDAT

; 49   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 50   : 	if (!pToolbarParent || !HookWindow(*pToolbarParent))

	cmp	DWORD PTR _pToolbarParent$[ebp], 0
	je	SHORT $L75687
	mov	esi, esp
	push	0
	mov	ecx, DWORD PTR _pToolbarParent$[ebp]
	call	??BCWnd@@QBEPAUHWND__@@XZ		; CWnd::operator HWND__ *
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75686
$L75687:

; 51   : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75685
$L75686:

; 52   : 
; 53   : 	ASSERT_VALID(pToolbar);

	push	53					; 00000035H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _pToolbar$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 54   : 	m_pToolbar = pToolbar;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _pToolbar$[ebp]
	mov	DWORD PTR [ecx+68], edx

; 55   : 
; 56   : 	InitTooltips();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?InitTooltips@CToolbarHelper@@IAEXXZ	; CToolbarHelper::InitTooltips

; 57   : 
; 58   : 	return TRUE;

	mov	eax, 1
$L75685:

; 59   : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Initialize@CToolbarHelper@@QAEHPAVCToolBar@@PAVCWnd@@@Z ENDP ; CToolbarHelper::Initialize
_TEXT	ENDS
PUBLIC	?Release@CToolbarHelper@@QAEHH@Z		; CToolbarHelper::Release
PUBLIC	?ClearDropButton@CToolbarHelper@@QAEHIH@Z	; CToolbarHelper::ClearDropButton
PUBLIC	?RemoveAll@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXXZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::RemoveAll
PUBLIC	?GetStartPosition@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEPAU__POSITION@@XZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetStartPosition
PUBLIC	?GetNextAssoc@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEXAAPAU__POSITION@@AAIAAUTHButton@CToolbarHelper@@@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetNextAssoc
PUBLIC	?ScHookWindow@CSubclasser@@IAEHPAUHWND__@@@Z	; CSubclasser::ScHookWindow
;	COMDAT ?Release@CToolbarHelper@@QAEHH@Z
_TEXT	SEGMENT
_bClearDropBtns$ = 8
_this$ = -284
_pos$75694 = -4
_dm$75698 = -276
_nCmdID$75699 = -280
?Release@CToolbarHelper@@QAEHH@Z PROC NEAR		; CToolbarHelper::Release, COMDAT

; 62   : { 

	push	ebp
	mov	ebp, esp
	sub	esp, 284				; 0000011cH
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-284]
	mov	ecx, 71					; 00000047H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 63   : 	if (HookWindow(NULL) && ScHookWindow(NULL))

	mov	esi, esp
	push	0
	push	0
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	$L75692
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	?ScHookWindow@CSubclasser@@IAEHPAUHWND__@@@Z ; CSubclasser::ScHookWindow
	test	eax, eax
	je	$L75692

; 65   : 		if (bClearDropBtns)

	cmp	DWORD PTR _bClearDropBtns$[ebp], 0
	je	SHORT $L75697

; 67   : 			// iterate the buttons the hard way
; 68   : 			POSITION pos = m_mapTHButtons.GetStartPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	?GetStartPosition@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEPAU__POSITION@@XZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetStartPosition
	mov	DWORD PTR _pos$75694[ebp], eax
$L75696:

; 69   : 
; 70   : 			while (pos)

	cmp	DWORD PTR _pos$75694[ebp], 0
	je	SHORT $L75697

; 72   : 				THButton dm;
; 73   : 				UINT nCmdID = 0;

	mov	DWORD PTR _nCmdID$75699[ebp], 0

; 74   : 
; 75   : 				m_mapTHButtons.GetNextAssoc(pos, nCmdID, dm);

	lea	eax, DWORD PTR _dm$75698[ebp]
	push	eax
	lea	ecx, DWORD PTR _nCmdID$75699[ebp]
	push	ecx
	lea	edx, DWORD PTR _pos$75694[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	?GetNextAssoc@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEXAAPAU__POSITION@@AAIAAUTHButton@CToolbarHelper@@@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetNextAssoc

; 76   : 				ClearDropButton(nCmdID, FALSE);

	push	0
	mov	eax, DWORD PTR _nCmdID$75699[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ClearDropButton@CToolbarHelper@@QAEHIH@Z ; CToolbarHelper::ClearDropButton

; 77   : 			}

	jmp	SHORT $L75696
$L75697:

; 79   : 
; 80   : 		m_pToolbar = NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+68], 0

; 81   : 		m_mapTHButtons.RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	?RemoveAll@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXXZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::RemoveAll

; 82   : 		m_tt.DestroyWindow();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 108				; 0000006cH
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+108]
	mov	esi, esp
	call	DWORD PTR [eax+96]
	cmp	esi, esp
	call	__chkesp

; 83   : 
; 84   : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L75691
$L75692:

; 86   : 
; 87   : 	return FALSE;

	xor	eax, eax
$L75691:

; 88   : }

	pop	edi
	pop	esi
	add	esp, 284				; 0000011cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Release@CToolbarHelper@@QAEHH@Z ENDP			; CToolbarHelper::Release
_TEXT	ENDS
;	COMDAT ?ScHookWindow@CSubclasser@@IAEHPAUHWND__@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
_this$ = -4
?ScHookWindow@CSubclasser@@IAEHPAUHWND__@@@Z PROC NEAR	; CSubclasser::ScHookWindow, COMDAT

; 151  : 	inline BOOL ScHookWindow(HWND hWnd) { return m_subclass.HookWindow(hWnd, GetTopSubclasser()); }

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
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp
	mov	esi, esp
	push	eax
	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	call	DWORD PTR [eax+4]
	cmp	esi, esp
	call	__chkesp
	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ScHookWindow@CSubclasser@@IAEHPAUHWND__@@@Z ENDP	; CSubclasser::ScHookWindow
_TEXT	ENDS
PUBLIC	?SetDropButton@CToolbarHelper@@QAEHIIHID@Z	; CToolbarHelper::SetDropButton
PUBLIC	?Lookup@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEHIAAUTHButton@CToolbarHelper@@@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::Lookup
PUBLIC	?SetButton@CToolbarHelper@@IAEHIIHIGPBG@Z	; CToolbarHelper::SetButton
_BSS	SEGMENT
$SG75711 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT ?SetDropButton@CToolbarHelper@@QAEHIIHID@Z
_TEXT	SEGMENT
_nBtnCmdID$ = 8
_nMenuID$ = 12
_nSubMenu$ = 16
_nDefCmdID$ = 20
_cHotkey$ = 24
_this$ = -276
_dm$ = -272
?SetDropButton@CToolbarHelper@@QAEHIIHID@Z PROC NEAR	; CToolbarHelper::SetDropButton, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 276				; 00000114H
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-276]
	mov	ecx, 69					; 00000045H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 92   : 	THButton dm;
; 93   : 
; 94   : 	if (m_mapTHButtons.Lookup(nBtnCmdID, dm))

	lea	eax, DWORD PTR _dm$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nBtnCmdID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	?Lookup@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEHIAAUTHButton@CToolbarHelper@@@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::Lookup
	test	eax, eax
	je	SHORT $L75709

; 95   : 		return SetButton(nBtnCmdID, nMenuID, nSubMenu, nDefCmdID, cHotkey, dm.szTip);

	lea	edx, DWORD PTR _dm$[ebp+14]
	push	edx
	movsx	ax, BYTE PTR _cHotkey$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nDefCmdID$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nSubMenu$[ebp]
	push	edx
	mov	eax, DWORD PTR _nMenuID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nBtnCmdID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetButton@CToolbarHelper@@IAEHIIHIGPBG@Z ; CToolbarHelper::SetButton
	jmp	SHORT $L75710
$L75709:

; 97   : 		return SetButton(nBtnCmdID, nMenuID, nSubMenu, nDefCmdID, cHotkey, _T(""));

	push	OFFSET FLAT:$SG75711
	movsx	dx, BYTE PTR _cHotkey$[ebp]
	push	edx
	mov	eax, DWORD PTR _nDefCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nSubMenu$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nMenuID$[ebp]
	push	edx
	mov	eax, DWORD PTR _nBtnCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetButton@CToolbarHelper@@IAEHIIHIGPBG@Z ; CToolbarHelper::SetButton
$L75710:

; 98   : }

	pop	edi
	add	esp, 276				; 00000114H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	20					; 00000014H
?SetDropButton@CToolbarHelper@@QAEHIIHID@Z ENDP		; CToolbarHelper::SetDropButton
_TEXT	ENDS
PUBLIC	?SetTooltip@CToolbarHelper@@QAEHIPBG@Z		; CToolbarHelper::SetTooltip
PUBLIC	?SetTooltip@CToolbarHelper@@QAEHII@Z		; CToolbarHelper::SetTooltip
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	?LoadStringW@CString@@QAEHI@Z:NEAR		; CString::LoadStringW
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT xdata$x
; File D:\_CODE\Shared\ToolbarHelper.cpp
xdata$x	SEGMENT
$T76665	DD	019930520H
	DD	01H
	DD	FLAT:$T76667
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76667	DD	0ffffffffH
	DD	FLAT:$L76663
xdata$x	ENDS
;	COMDAT ?SetTooltip@CToolbarHelper@@QAEHII@Z
_TEXT	SEGMENT
$T76661 = -20
$T76662 = -24
__$EHRec$ = -12
_nBtnCmdID$ = 8
_nIDTip$ = 12
_this$ = -28
_sTip$ = -16
?SetTooltip@CToolbarHelper@@QAEHII@Z PROC NEAR		; CToolbarHelper::SetTooltip, COMDAT

; 101  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76666
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

; 102  : 	CString sTip;

	lea	ecx, DWORD PTR _sTip$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 103  : 
; 104  : 	if (sTip.LoadString(nIDTip))

	mov	eax, DWORD PTR _nIDTip$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	?LoadStringW@CString@@QAEHI@Z		; CString::LoadStringW
	test	eax, eax
	je	SHORT $L75718

; 105  : 		return SetTooltip(nBtnCmdID, sTip);

	lea	ecx, DWORD PTR _sTip$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _nBtnCmdID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetTooltip@CToolbarHelper@@QAEHIPBG@Z	; CToolbarHelper::SetTooltip
	mov	DWORD PTR $T76661[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76661[ebp]
	jmp	SHORT $L75716
$L75718:

; 106  : 
; 107  : 	return FALSE;

	mov	DWORD PTR $T76662[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76662[ebp]
$L75716:

; 108  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76663:
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76666:
	mov	eax, OFFSET FLAT:$T76665
	jmp	___CxxFrameHandler
text$x	ENDS
?SetTooltip@CToolbarHelper@@QAEHII@Z ENDP		; CToolbarHelper::SetTooltip
;	COMDAT ?SetTooltip@CToolbarHelper@@QAEHIPBG@Z
_TEXT	SEGMENT
_nBtnCmdID$ = 8
_szTip$ = 12
_this$ = -276
_dm$ = -272
?SetTooltip@CToolbarHelper@@QAEHIPBG@Z PROC NEAR	; CToolbarHelper::SetTooltip, COMDAT

; 111  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 276				; 00000114H
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-276]
	mov	ecx, 69					; 00000045H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 112  : 	THButton dm;
; 113  : 
; 114  : 	if (m_mapTHButtons.Lookup(nBtnCmdID, dm))

	lea	eax, DWORD PTR _dm$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nBtnCmdID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	?Lookup@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEHIAAUTHButton@CToolbarHelper@@@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::Lookup
	test	eax, eax
	je	SHORT $L75727

; 115  : 		return SetButton(nBtnCmdID, dm.nMenuID, dm.nSubMenu, dm.nDefCmdID, dm.cHotKey, szTip);

	mov	edx, DWORD PTR _szTip$[ebp]
	push	edx
	mov	ax, WORD PTR _dm$[ebp+12]
	push	eax
	mov	ecx, DWORD PTR _dm$[ebp+8]
	push	ecx
	mov	edx, DWORD PTR _dm$[ebp+4]
	push	edx
	mov	eax, DWORD PTR _dm$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nBtnCmdID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetButton@CToolbarHelper@@IAEHIIHIGPBG@Z ; CToolbarHelper::SetButton
	jmp	SHORT $L75728
$L75727:

; 117  : 		return SetButton(nBtnCmdID, 0, 0, 0, 0, szTip);

	mov	edx, DWORD PTR _szTip$[ebp]
	push	edx
	push	0
	push	0
	push	0
	push	0
	mov	eax, DWORD PTR _nBtnCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetButton@CToolbarHelper@@IAEHIIHIGPBG@Z ; CToolbarHelper::SetButton
$L75728:

; 118  : }

	pop	edi
	add	esp, 276				; 00000114H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SetTooltip@CToolbarHelper@@QAEHIPBG@Z ENDP		; CToolbarHelper::SetTooltip
_TEXT	ENDS
PUBLIC	??A?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEAAUTHButton@CToolbarHelper@@I@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::operator[]
EXTRN	?CommandToIndex@CToolBar@@QBEHI@Z:NEAR		; CToolBar::CommandToIndex
EXTRN	?RedrawWindow@CWnd@@QAEHPBUtagRECT@@PAVCRgn@@I@Z:NEAR ; CWnd::RedrawWindow
EXTRN	?GetButtonStyle@CToolBar@@QBEIH@Z:NEAR		; CToolBar::GetButtonStyle
EXTRN	?SetButtonStyle@CToolBar@@QAEXHI@Z:NEAR		; CToolBar::SetButtonStyle
EXTRN	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ:NEAR ; CToolBar::GetToolBarCtrl
EXTRN	__imp__wcsncpy:NEAR
EXTRN	?SetExtendedStyle@CToolBarCtrl@@QAEKK@Z:NEAR	; CToolBarCtrl::SetExtendedStyle
;	COMDAT ?SetButton@CToolbarHelper@@IAEHIIHIGPBG@Z
_TEXT	SEGMENT
_nBtnCmdID$ = 8
_nMenuID$ = 12
_nSubMenu$ = 16
_nDefCmdID$ = 20
_cHotkey$ = 24
_szTip$ = 28
_this$ = -284
_nIndex$ = -276
_dwStyle$75741 = -280
_dm$ = -272
?SetButton@CToolbarHelper@@IAEHIIHIGPBG@Z PROC NEAR	; CToolbarHelper::SetButton, COMDAT

; 121  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 284				; 0000011cH
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-284]
	mov	ecx, 71					; 00000047H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 122  : 	int nIndex = m_pToolbar->CommandToIndex(nBtnCmdID);

	mov	eax, DWORD PTR _nBtnCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+68]
	call	?CommandToIndex@CToolBar@@QBEHI@Z	; CToolBar::CommandToIndex
	mov	DWORD PTR _nIndex$[ebp], eax

; 123  : 
; 124  : 	if (nIndex == -1)

	cmp	DWORD PTR _nIndex$[ebp], -1
	jne	SHORT $L75739

; 125  : 		return FALSE;

	xor	eax, eax
	jmp	$L75737
$L75739:

; 126  : 
; 127  : 	if (nMenuID)

	cmp	DWORD PTR _nMenuID$[ebp], 0
	je	SHORT $L75740

; 129  : 		m_pToolbar->GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	push	1
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+68]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?SetExtendedStyle@CToolBarCtrl@@QAEKK@Z	; CToolBarCtrl::SetExtendedStyle

; 130  : 
; 131  : 		DWORD dwStyle = m_pToolbar->GetButtonStyle(nIndex) | TBSTYLE_DROPDOWN;

	mov	eax, DWORD PTR _nIndex$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+68]
	call	?GetButtonStyle@CToolBar@@QBEIH@Z	; CToolBar::GetButtonStyle
	or	al, 8
	mov	DWORD PTR _dwStyle$75741[ebp], eax

; 132  : 		m_pToolbar->SetButtonStyle(nIndex, dwStyle);

	mov	edx, DWORD PTR _dwStyle$75741[ebp]
	push	edx
	mov	eax, DWORD PTR _nIndex$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+68]
	call	?SetButtonStyle@CToolBar@@QAEXHI@Z	; CToolBar::SetButtonStyle
$L75740:

; 134  : 
; 135  : 	THButton dm = { nMenuID, nSubMenu, nDefCmdID, cHotkey, 0 };

	mov	edx, DWORD PTR _nMenuID$[ebp]
	mov	DWORD PTR _dm$[ebp], edx
	mov	eax, DWORD PTR _nSubMenu$[ebp]
	mov	DWORD PTR _dm$[ebp+4], eax
	mov	ecx, DWORD PTR _nDefCmdID$[ebp]
	mov	DWORD PTR _dm$[ebp+8], ecx
	mov	dx, WORD PTR _cHotkey$[ebp]
	mov	WORD PTR _dm$[ebp+12], dx
	mov	WORD PTR _dm$[ebp+14], 0
	mov	ecx, 63					; 0000003fH
	xor	eax, eax
	lea	edi, DWORD PTR _dm$[ebp+18]
	rep stosd
	stosw

; 136  : 
; 137  : 	//fabio_2005
; 138  : #if _MSC_VER >= 1400
; 139  : 	_tcsncpy_s(dm.szTip, 128, szTip, sizeof(dm.szTip) - 1);
; 140  : #else
; 141  : 	_tcsncpy(dm.szTip, szTip, sizeof(dm.szTip) - 1);

	mov	esi, esp
	push	255					; 000000ffH
	mov	eax, DWORD PTR _szTip$[ebp]
	push	eax
	lea	ecx, DWORD PTR _dm$[ebp+14]
	push	ecx
	call	DWORD PTR __imp__wcsncpy
	add	esp, 12					; 0000000cH
	cmp	esi, esp
	call	__chkesp

; 142  : #endif
; 143  : 
; 144  : 	m_mapTHButtons[nBtnCmdID] = dm;

	mov	edx, DWORD PTR _nBtnCmdID$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	??A?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEAAUTHButton@CToolbarHelper@@I@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::operator[]
	mov	edi, eax
	mov	ecx, 68					; 00000044H
	lea	esi, DWORD PTR _dm$[ebp]
	rep movsd

; 145  : 
; 146  : 	m_pToolbar->RedrawWindow();

	push	261					; 00000105H
	push	0
	push	0
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+68]
	call	?RedrawWindow@CWnd@@QAEHPBUtagRECT@@PAVCRgn@@I@Z ; CWnd::RedrawWindow

; 147  : 
; 148  : 	return TRUE;

	mov	eax, 1
$L75737:

; 149  : }

	pop	edi
	pop	esi
	add	esp, 284				; 0000011cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	24					; 00000018H
?SetButton@CToolbarHelper@@IAEHIIHIGPBG@Z ENDP		; CToolbarHelper::SetButton
_TEXT	ENDS
;	COMDAT ?ClearDropButton@CToolbarHelper@@QAEHIH@Z
_TEXT	SEGMENT
_nBtnCmdID$ = 8
_bRedraw$ = 12
_this$ = -12
_nIndex$ = -4
_dwStyle$ = -8
?ClearDropButton@CToolbarHelper@@QAEHIH@Z PROC NEAR	; CToolbarHelper::ClearDropButton, COMDAT

; 152  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 153  : 	int nIndex = m_pToolbar->CommandToIndex(nBtnCmdID);

	mov	eax, DWORD PTR _nBtnCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+68]
	call	?CommandToIndex@CToolBar@@QBEHI@Z	; CToolBar::CommandToIndex
	mov	DWORD PTR _nIndex$[ebp], eax

; 154  : 
; 155  : 	if (nIndex == -1)

	cmp	DWORD PTR _nIndex$[ebp], -1
	jne	SHORT $L75749

; 156  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75747
$L75749:

; 157  : 
; 158  : 	DWORD dwStyle = m_pToolbar->GetButtonStyle(nIndex) & ~TBSTYLE_DROPDOWN;

	mov	edx, DWORD PTR _nIndex$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+68]
	call	?GetButtonStyle@CToolBar@@QBEIH@Z	; CToolBar::GetButtonStyle
	and	al, -9					; fffffff7H
	mov	DWORD PTR _dwStyle$[ebp], eax

; 159  : 	m_pToolbar->SetButtonStyle(nIndex, dwStyle);

	mov	ecx, DWORD PTR _dwStyle$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nIndex$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+68]
	call	?SetButtonStyle@CToolBar@@QAEXHI@Z	; CToolBar::SetButtonStyle

; 160  : 
; 161  : 	if (bRedraw)

	cmp	DWORD PTR _bRedraw$[ebp], 0
	je	SHORT $L75751

; 162  : 		m_pToolbar->RedrawWindow();

	push	261					; 00000105H
	push	0
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+68]
	call	?RedrawWindow@CWnd@@QAEHPBUtagRECT@@PAVCRgn@@I@Z ; CWnd::RedrawWindow
$L75751:

; 163  : 
; 164  : 	return TRUE;

	mov	eax, 1
$L75747:

; 165  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?ClearDropButton@CToolbarHelper@@QAEHIH@Z ENDP		; CToolbarHelper::ClearDropButton
_TEXT	ENDS
PUBLIC	?SetDefaultMenuID@CToolbarHelper@@QAEHII@Z	; CToolbarHelper::SetDefaultMenuID
;	COMDAT ?SetDefaultMenuID@CToolbarHelper@@QAEHII@Z
_TEXT	SEGMENT
_nBtnCmdID$ = 8
_nDefCmdID$ = 12
_this$ = -276
_dm$ = -272
?SetDefaultMenuID@CToolbarHelper@@QAEHII@Z PROC NEAR	; CToolbarHelper::SetDefaultMenuID, COMDAT

; 168  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 276				; 00000114H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-276]
	mov	ecx, 69					; 00000045H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 169  : 	THButton dm;
; 170  : 	
; 171  : 	if (m_mapTHButtons.Lookup(nBtnCmdID, dm))

	lea	eax, DWORD PTR _dm$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nBtnCmdID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	?Lookup@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEHIAAUTHButton@CToolbarHelper@@@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::Lookup
	test	eax, eax
	je	SHORT $L75758

; 173  : 		dm.nDefCmdID = nDefCmdID;

	mov	edx, DWORD PTR _nDefCmdID$[ebp]
	mov	DWORD PTR _dm$[ebp+8], edx

; 174  : 		m_mapTHButtons[nBtnCmdID] = dm;

	mov	eax, DWORD PTR _nBtnCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	??A?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEAAUTHButton@CToolbarHelper@@I@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::operator[]
	mov	edi, eax
	mov	ecx, 68					; 00000044H
	lea	esi, DWORD PTR _dm$[ebp]
	rep movsd

; 175  : 
; 176  : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L75756
$L75758:

; 178  : 
; 179  : 	return FALSE;

	xor	eax, eax
$L75756:

; 180  : }

	pop	edi
	pop	esi
	add	esp, 276				; 00000114H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SetDefaultMenuID@CToolbarHelper@@QAEHII@Z ENDP		; CToolbarHelper::SetDefaultMenuID
_TEXT	ENDS
PUBLIC	?PrepareMenuItems@CToolbarHelper@@SAXPAVCMenu@@PAVCWnd@@@Z ; CToolbarHelper::PrepareMenuItems
PUBLIC	?GetCWnd@CSubclassWnd@@QBEPAVCWnd@@XZ		; CSubclassWnd::GetCWnd
PUBLIC	?DisplayDropMenu@CToolbarHelper@@IAEHIH@Z	; CToolbarHelper::DisplayDropMenu
EXTRN	?PressButton@CToolBarCtrl@@QAEHHH@Z:NEAR	; CToolBarCtrl::PressButton
EXTRN	??0CMenu@@QAE@XZ:NEAR				; CMenu::CMenu
EXTRN	?LoadMenuW@CMenu@@QAEHI@Z:NEAR			; CMenu::LoadMenuW
EXTRN	?TrackPopupMenu@CMenu@@QAEHIHHPAVCWnd@@PBUtagRECT@@@Z:NEAR ; CMenu::TrackPopupMenu
EXTRN	?ClientToScreen@CWnd@@QBEXPAUtagRECT@@@Z:NEAR	; CWnd::ClientToScreen
EXTRN	?GetSubMenu@CMenu@@QBEPAV1@H@Z:NEAR		; CMenu::GetSubMenu
EXTRN	?SetDefaultItem@CMenu@@QAEHIH@Z:NEAR		; CMenu::SetDefaultItem
EXTRN	??1CMenu@@UAE@XZ:NEAR				; CMenu::~CMenu
EXTRN	??0CRect@@QAE@XZ:NEAR				; CRect::CRect
EXTRN	??BCRect@@QAEPAUtagRECT@@XZ:NEAR		; CRect::operator tagRECT *
;	COMDAT xdata$x
; File D:\_CODE\Shared\ToolbarHelper.cpp
xdata$x	SEGMENT
$T76683	DD	019930520H
	DD	01H
	DD	FLAT:$T76685
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76685	DD	0ffffffffH
	DD	FLAT:$L76681
xdata$x	ENDS
;	COMDAT ?DisplayDropMenu@CToolbarHelper@@IAEHIH@Z
_TEXT	SEGMENT
$T76680 = -320
__$EHRec$ = -12
_nCmdID$ = 8
_bPressBtn$ = 12
_this$ = -324
_dm$ = -284
_menu$75766 = -296
_pSubMenu$75767 = -288
_rItem$75770 = -312
_nIndex$75771 = -316
?DisplayDropMenu@CToolbarHelper@@IAEHIH@Z PROC NEAR	; CToolbarHelper::DisplayDropMenu, COMDAT

; 183  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76684
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 312				; 00000138H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-324]
	mov	ecx, 78					; 0000004eH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 184  : 	// see if we have a menu for it
; 185  : 	THButton dm;
; 186  : 	
; 187  : 	if (m_mapTHButtons.Lookup(nCmdID, dm) && dm.nMenuID)

	lea	eax, DWORD PTR _dm$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nCmdID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	?Lookup@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEHIAAUTHButton@CToolbarHelper@@@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::Lookup
	test	eax, eax
	je	$L75765
	cmp	DWORD PTR _dm$[ebp], 0
	je	$L75765

; 189  : 		CMenu menu, *pSubMenu;

	lea	ecx, DWORD PTR _menu$75766[ebp]
	call	??0CMenu@@QAE@XZ			; CMenu::CMenu
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 190  : 		
; 191  : 		if (menu.LoadMenu(dm.nMenuID))

	mov	edx, DWORD PTR _dm$[ebp]
	push	edx
	lea	ecx, DWORD PTR _menu$75766[ebp]
	call	?LoadMenuW@CMenu@@QAEHI@Z		; CMenu::LoadMenuW
	test	eax, eax
	je	$L75769

; 193  : 			pSubMenu = menu.GetSubMenu(dm.nSubMenu);

	mov	eax, DWORD PTR _dm$[ebp+4]
	push	eax
	lea	ecx, DWORD PTR _menu$75766[ebp]
	call	?GetSubMenu@CMenu@@QBEPAV1@H@Z		; CMenu::GetSubMenu
	mov	DWORD PTR _pSubMenu$75767[ebp], eax

; 194  : 			
; 195  : 			if (pSubMenu)

	cmp	DWORD PTR _pSubMenu$75767[ebp], 0
	je	$L75769

; 197  : 				PrepareMenuItems(pSubMenu, GetCWnd());

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCWnd@CSubclassWnd@@QBEPAVCWnd@@XZ	; CSubclassWnd::GetCWnd
	push	eax
	mov	ecx, DWORD PTR _pSubMenu$75767[ebp]
	push	ecx
	call	?PrepareMenuItems@CToolbarHelper@@SAXPAVCMenu@@PAVCWnd@@@Z ; CToolbarHelper::PrepareMenuItems
	add	esp, 8

; 198  : 				pSubMenu->SetDefaultItem(dm.nDefCmdID);

	push	0
	mov	edx, DWORD PTR _dm$[ebp+8]
	push	edx
	mov	ecx, DWORD PTR _pSubMenu$75767[ebp]
	call	?SetDefaultItem@CMenu@@QAEHIH@Z		; CMenu::SetDefaultItem

; 199  : 				
; 200  : 				CRect rItem;

	lea	ecx, DWORD PTR _rItem$75770[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 201  : 				int nIndex = m_pToolbar->CommandToIndex(nCmdID);

	mov	eax, DWORD PTR _nCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+68]
	call	?CommandToIndex@CToolBar@@QBEHI@Z	; CToolBar::CommandToIndex
	mov	DWORD PTR _nIndex$75771[ebp], eax

; 202  : 
; 203  : 				m_pToolbar->GetItemRect(nIndex, rItem);

	lea	ecx, DWORD PTR _rItem$75770[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	mov	esi, esp
	push	eax
	mov	edx, DWORD PTR _nIndex$75771[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+68]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+68]
	mov	edx, DWORD PTR [eax]
	call	DWORD PTR [edx+232]
	cmp	esi, esp
	call	__chkesp

; 204  : 				m_pToolbar->ClientToScreen(rItem);

	lea	ecx, DWORD PTR _rItem$75770[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+68]
	call	?ClientToScreen@CWnd@@QBEXPAUtagRECT@@@Z ; CWnd::ClientToScreen

; 205  : 			
; 206  : 				if (bPressBtn)

	cmp	DWORD PTR _bPressBtn$[ebp], 0
	je	SHORT $L75772

; 207  : 					m_pToolbar->GetToolBarCtrl().PressButton(nCmdID, TRUE);

	push	1
	mov	ecx, DWORD PTR _nCmdID$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+68]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?PressButton@CToolBarCtrl@@QAEHHH@Z	; CToolBarCtrl::PressButton
$L75772:

; 208  : 
; 209  : 				pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rItem.left, rItem.bottom, GetCWnd());

	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCWnd@CSubclassWnd@@QBEPAVCWnd@@XZ	; CSubclassWnd::GetCWnd
	push	eax
	mov	eax, DWORD PTR _rItem$75770[ebp+12]
	push	eax
	mov	ecx, DWORD PTR _rItem$75770[ebp]
	push	ecx
	push	0
	mov	ecx, DWORD PTR _pSubMenu$75767[ebp]
	call	?TrackPopupMenu@CMenu@@QAEHIHHPAVCWnd@@PBUtagRECT@@@Z ; CMenu::TrackPopupMenu

; 210  : 
; 211  : 				if (bPressBtn)

	cmp	DWORD PTR _bPressBtn$[ebp], 0
	je	SHORT $L75773

; 212  : 					m_pToolbar->GetToolBarCtrl().PressButton(nCmdID, FALSE);

	push	0
	mov	edx, DWORD PTR _nCmdID$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+68]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?PressButton@CToolBarCtrl@@QAEHHH@Z	; CToolBarCtrl::PressButton
$L75773:

; 213  : 				
; 214  : 				return TRUE; // we handled it

	mov	DWORD PTR $T76680[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _menu$75766[ebp]
	call	??1CMenu@@UAE@XZ			; CMenu::~CMenu
	mov	eax, DWORD PTR $T76680[ebp]
	jmp	SHORT $L75763
$L75769:

; 217  : 	}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _menu$75766[ebp]
	call	??1CMenu@@UAE@XZ			; CMenu::~CMenu
$L75765:

; 218  : 
; 219  : 	return FALSE;

	xor	eax, eax
$L75763:

; 220  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 324				; 00000144H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76681:
	lea	ecx, DWORD PTR _menu$75766[ebp]
	call	??1CMenu@@UAE@XZ			; CMenu::~CMenu
	ret	0
$L76684:
	mov	eax, OFFSET FLAT:$T76683
	jmp	___CxxFrameHandler
text$x	ENDS
?DisplayDropMenu@CToolbarHelper@@IAEHIH@Z ENDP		; CToolbarHelper::DisplayDropMenu
EXTRN	?FromHandle@CWnd@@SGPAV1@PAUHWND__@@@Z:NEAR	; CWnd::FromHandle
;	COMDAT ?GetCWnd@CSubclassWnd@@QBEPAVCWnd@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetCWnd@CSubclassWnd@@QBEPAVCWnd@@XZ PROC NEAR		; CSubclassWnd::GetCWnd, COMDAT

; 59   : 	inline CWnd* GetCWnd() const { return CWnd::FromHandle(m_hWndHooked); }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	?FromHandle@CWnd@@SGPAV1@PAUHWND__@@@Z	; CWnd::FromHandle
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetCWnd@CSubclassWnd@@QBEPAVCWnd@@XZ ENDP		; CSubclassWnd::GetCWnd
_TEXT	ENDS
PUBLIC	?GetTip@CToolbarHelper@@IBE?AVCString@@IPAUtagPOINT@@@Z ; CToolbarHelper::GetTip
PUBLIC	?IsCmdEnabled@CToolbarHelper@@IBEHI@Z		; CToolbarHelper::IsCmdEnabled
EXTRN	?SendMessageW@CWnd@@QAEJIIJ@Z:NEAR		; CWnd::SendMessageW
EXTRN	_atexit:NEAR
EXTRN	?ScreenToClient@CWnd@@QBEXPAUtagPOINT@@@Z:NEAR	; CWnd::ScreenToClient
EXTRN	__imp__GetMessagePos@0:NEAR
EXTRN	?WindowProc@CSubclassWnd@@MAEJPAUHWND__@@IIJ@Z:NEAR ; CSubclassWnd::WindowProc
EXTRN	??0CPoint@@QAE@K@Z:NEAR				; CPoint::CPoint
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
EXTRN	__imp__GetDlgCtrlID@4:NEAR
EXTRN	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ:NEAR	; CWnd::GetSafeHwnd
_BSS	SEGMENT
	ALIGN	4

_?sTipText@?BB@??WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z@4VCString@@A DD 01H DUP (?)
_?$S280@?BB@??WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT xdata$x
; File D:\_CODE\Shared\ToolbarHelper.cpp
xdata$x	SEGMENT
$T76697	DD	019930520H
	DD	01H
	DD	FLAT:$T76699
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76699	DD	0ffffffffH
	DD	FLAT:$L76695
xdata$x	ENDS
;	COMDAT ?WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT
_msg$ = 12
_wp$ = 16
_lp$ = 20
_this$ = -336
_pNMHDR$75787 = -16
_pNMTB$75795 = -20
_pTTT$75800 = -28
_nID$75803 = -24
_point$75806 = -36
_pTooltipCtrl$75818 = -40
_hCtrlFrom$75824 = -44
_dm$75827 = -316
_nCmdID$75828 = -320
_bRes$75840 = -324
_lr$75844 = -328
$T76692 = -332
__$EHRec$ = -12
_hRealWnd$ = 8
?WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z PROC NEAR ; CToolbarHelper::WindowProc, COMDAT

; 223  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76698
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 344				; 00000158H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-356]
	mov	ecx, 86					; 00000056H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 225  : 	{

	mov	eax, DWORD PTR _msg$[ebp]
	mov	DWORD PTR -340+[ebp], eax
	cmp	DWORD PTR -340+[ebp], 2
	je	$L75843
	cmp	DWORD PTR -340+[ebp], 78		; 0000004eH
	je	SHORT $L75786
	cmp	DWORD PTR -340+[ebp], 273		; 00000111H
	je	$L75823
	jmp	$L75783
$L75786:

; 228  : 			LPNMHDR pNMHDR = (LPNMHDR)lp;

	mov	ecx, DWORD PTR _lp$[ebp]
	mov	DWORD PTR _pNMHDR$75787[ebp], ecx

; 231  : 			{

	mov	edx, DWORD PTR _pNMHDR$75787[ebp]
	mov	eax, DWORD PTR [edx+8]
	mov	DWORD PTR -344+[ebp], eax
	cmp	DWORD PTR -344+[ebp], -710		; fffffd3aH
	je	SHORT $L75793
	cmp	DWORD PTR -344+[ebp], -530		; fffffdeeH
	je	SHORT $L75799
	cmp	DWORD PTR -344+[ebp], -521		; fffffdf7H
	je	$L75817
	jmp	$L75790
$L75793:

; 232  : 			case TBN_DROPDOWN:
; 233  : 				// check its our toolbar
; 234  : 				if (pNMHDR->hwndFrom == m_pToolbar->GetSafeHwnd())

	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+68]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	mov	edx, DWORD PTR _pNMHDR$75787[ebp]
	cmp	DWORD PTR [edx], eax
	jne	SHORT $L75798

; 236  : 					// load the menu
; 237  : 					LPNMTOOLBAR pNMTB = (LPNMTOOLBAR)pNMHDR;

	mov	eax, DWORD PTR _pNMHDR$75787[ebp]
	mov	DWORD PTR _pNMTB$75795[ebp], eax

; 238  : 					
; 239  : 					if (DisplayDropMenu((UINT)pNMTB->iItem))

	push	0
	mov	ecx, DWORD PTR _pNMTB$75795[ebp]
	mov	edx, DWORD PTR [ecx+12]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DisplayDropMenu@CToolbarHelper@@IAEHIH@Z ; CToolbarHelper::DisplayDropMenu
	test	eax, eax
	je	SHORT $L75798

; 240  : 						return FALSE; // we handled it

	xor	eax, eax
	jmp	$L75781
$L75798:

; 242  : 				break;

	jmp	$L75790
$L75799:

; 250  : 					// to be thorough we will need to handle UNICODE versions of the message also !!
; 251  : 					TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;

	mov	eax, DWORD PTR _pNMHDR$75787[ebp]
	mov	DWORD PTR _pTTT$75800[ebp], eax

; 252  : 
; 253  : 					// only handle this if it's not already been done
; 254  : 					if (pTTT->lpszText && *(pTTT->lpszText))

	mov	ecx, DWORD PTR _pTTT$75800[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L75802
	mov	edx, DWORD PTR _pTTT$75800[ebp]
	mov	eax, DWORD PTR [edx+12]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax]
	test	ecx, ecx
	je	SHORT $L75802

; 255  : 						break;

	jmp	$L75790
$L75802:

; 256  : 										
; 257  : 					UINT nID = pNMHDR->idFrom;

	mov	edx, DWORD PTR _pNMHDR$75787[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR _nID$75803[ebp], eax

; 258  : 					
; 259  : 					if (pTTT->uFlags & TTF_IDISHWND) // idFrom is actually the HWND of the tool 

	mov	ecx, DWORD PTR _pTTT$75800[ebp]
	mov	edx, DWORD PTR [ecx+180]
	and	edx, 1
	test	edx, edx
	je	SHORT $L75804

; 260  : 						nID = ::GetDlgCtrlID((HWND)nID);

	mov	esi, esp
	mov	eax, DWORD PTR _nID$75803[ebp]
	push	eax
	call	DWORD PTR __imp__GetDlgCtrlID@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nID$75803[ebp], eax
$L75804:

; 261  : 
; 262  : 					// get cursor pos
; 263  : 					CPoint point(::GetMessagePos());

	mov	esi, esp
	call	DWORD PTR __imp__GetMessagePos@0
	cmp	esi, esp
	call	__chkesp
	push	eax
	lea	ecx, DWORD PTR _point$75806[ebp]
	call	??0CPoint@@QAE@K@Z			; CPoint::CPoint

; 264  : 					m_pToolbar->ScreenToClient(&point);

	lea	ecx, DWORD PTR _point$75806[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+68]
	call	?ScreenToClient@CWnd@@QBEXPAUtagPOINT@@@Z ; CWnd::ScreenToClient

; 265  : 					
; 266  : 					// get tip
; 267  : 					static CString sTipText;

	xor	eax, eax
	mov	al, BYTE PTR _?$S280@?BB@??WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L75811
	mov	cl, BYTE PTR _?$S280@?BB@??WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z@4EA
	or	cl, 1
	mov	BYTE PTR _?$S280@?BB@??WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z@4EA, cl
	mov	ecx, OFFSET FLAT:_?sTipText@?BB@??WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z@4VCString@@A
	call	??0CString@@QAE@XZ			; CString::CString
	push	OFFSET FLAT:_$E281
	call	_atexit
	add	esp, 4
$L75811:

; 268  : 										
; 269  : 					sTipText = GetTip(nID, &point);

	lea	edx, DWORD PTR _point$75806[ebp]
	push	edx
	mov	eax, DWORD PTR _nID$75803[ebp]
	push	eax
	lea	ecx, DWORD PTR $T76692[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetTip@CToolbarHelper@@IBE?AVCString@@IPAUtagPOINT@@@Z ; CToolbarHelper::GetTip
	mov	DWORD PTR -348+[ebp], eax
	mov	edx, DWORD PTR -348+[ebp]
	mov	DWORD PTR -352+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR -352+[ebp]
	push	eax
	mov	ecx, OFFSET FLAT:_?sTipText@?BB@??WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z@4VCString@@A
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76692[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 270  : 					
; 271  : 					if (!sTipText.IsEmpty()) // will be zero on a separator

	mov	ecx, OFFSET FLAT:_?sTipText@?BB@??WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z@4VCString@@A
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75814

; 273  : 						pTTT->lpszText = (LPTSTR)(LPCTSTR)sTipText;

	mov	ecx, OFFSET FLAT:_?sTipText@?BB@??WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z@4VCString@@A
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	ecx, DWORD PTR _pTTT$75800[ebp]
	mov	DWORD PTR [ecx+12], eax

; 274  : 						return TRUE;

	mov	eax, 1
	jmp	$L75781
$L75814:

; 277  : 				break;

	jmp	SHORT $L75790
$L75817:

; 281  : 					CWnd* pTooltipCtrl = CWnd::FromHandle(pNMHDR->hwndFrom);

	mov	edx, DWORD PTR _pNMHDR$75787[ebp]
	mov	eax, DWORD PTR [edx]
	push	eax
	call	?FromHandle@CWnd@@SGPAV1@PAUHWND__@@@Z	; CWnd::FromHandle
	mov	DWORD PTR _pTooltipCtrl$75818[ebp], eax
$L75819:

; 282  : 					ASSERT (pTooltipCtrl);

	cmp	DWORD PTR _pTooltipCtrl$75818[ebp], 0
	jne	SHORT $L75822
	push	282					; 0000011aH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75822
	int	3
$L75822:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75819

; 283  : 
; 284  : 					/*int nWidth = */pTooltipCtrl->SendMessage(TTM_SETMAXTIPWIDTH, 0, 
; 285  : 															m_bMultiline ? m_nMultilineWidth : UINT_MAX);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+100], 0
	je	SHORT $L76693
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+104]
	mov	DWORD PTR -356+[ebp], ecx
	jmp	SHORT $L76694
$L76693:
	mov	DWORD PTR -356+[ebp], -1
$L76694:
	mov	edx, DWORD PTR -356+[ebp]
	push	edx
	push	0
	push	1048					; 00000418H
	mov	ecx, DWORD PTR _pTooltipCtrl$75818[ebp]
	call	?SendMessageW@CWnd@@QAEJIIJ@Z		; CWnd::SendMessageW
$L75790:

; 290  : 		break;

	jmp	$L75783
$L75823:

; 294  : 			HWND hCtrlFrom = (HWND)lp;

	mov	eax, DWORD PTR _lp$[ebp]
	mov	DWORD PTR _hCtrlFrom$75824[ebp], eax

; 295  : 
; 296  : 			// if m_pToolbar sent the command and we have a mapping for it then
; 297  : 			// change it to the default cmd for that button
; 298  : 			if (hCtrlFrom == *m_pToolbar)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+68]
	call	??BCWnd@@QBEPAUHWND__@@XZ		; CWnd::operator HWND__ *
	cmp	DWORD PTR _hCtrlFrom$75824[ebp], eax
	jne	$L75841

; 300  : 				THButton dm;
; 301  : 				UINT nCmdID = LOWORD(wp);

	mov	edx, DWORD PTR _wp$[ebp]
	and	edx, 65535				; 0000ffffH
	mov	DWORD PTR _nCmdID$75828[ebp], edx

; 302  : 
; 303  : 				if (m_mapTHButtons.Lookup(nCmdID, dm))

	lea	eax, DWORD PTR _dm$75827[ebp]
	push	eax
	mov	ecx, DWORD PTR _nCmdID$75828[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	?Lookup@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEHIAAUTHButton@CToolbarHelper@@@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::Lookup
	test	eax, eax
	je	SHORT $L75841

; 305  : 					// if we have an enabled default command then send it
; 306  : 					if (dm.nDefCmdID && IsCmdEnabled(dm.nDefCmdID))

	cmp	DWORD PTR _dm$75827[ebp+8], 0
	je	SHORT $L75831
	mov	edx, DWORD PTR _dm$75827[ebp+8]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsCmdEnabled@CToolbarHelper@@IBEHI@Z	; CToolbarHelper::IsCmdEnabled
	test	eax, eax
	je	SHORT $L75831

; 307  : 						wp = MAKEWPARAM(dm.nDefCmdID, HIWORD(wp));

	mov	eax, DWORD PTR _dm$75827[ebp+8]
	and	eax, 65535				; 0000ffffH
	mov	ecx, DWORD PTR _wp$[ebp]
	shr	ecx, 16					; 00000010H
	and	ecx, 65535				; 0000ffffH
	and	ecx, 65535				; 0000ffffH
	shl	ecx, 16					; 00000010H
	or	eax, ecx
	mov	DWORD PTR _wp$[ebp], eax

; 308  : 					else

	jmp	SHORT $L75841
$L75831:

; 310  : 						BOOL bRes = DisplayDropMenu(nCmdID, TRUE);

	push	1
	mov	edx, DWORD PTR _nCmdID$75828[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DisplayDropMenu@CToolbarHelper@@IAEHIH@Z ; CToolbarHelper::DisplayDropMenu
	mov	DWORD PTR _bRes$75840[ebp], eax

; 311  : 
; 312  : 						if (bRes)

	cmp	DWORD PTR _bRes$75840[ebp], 0
	je	SHORT $L75841

; 313  : 							return 0L; // we handled it

	xor	eax, eax
	jmp	SHORT $L75781
$L75841:

; 318  : 		break;

	jmp	SHORT $L75783
$L75843:

; 326  : 			// must call rest of chain first
; 327  : 			LRESULT lr =  CSubclassWnd::WindowProc(hRealWnd, msg, wp, lp);

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
	mov	DWORD PTR _lr$75844[ebp], eax

; 328  : 			HookWindow(NULL);

	mov	esi, esp
	push	0
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp

; 329  : 			return lr;

	mov	eax, DWORD PTR _lr$75844[ebp]
	jmp	SHORT $L75781
$L75783:

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
$L75781:

; 334  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 356				; 00000164H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76695:
	lea	ecx, DWORD PTR $T76692[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76698:
	mov	eax, OFFSET FLAT:$T76697
	jmp	___CxxFrameHandler
text$x	ENDS
?WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z ENDP	; CToolbarHelper::WindowProc
;	COMDAT _$E281
_TEXT	SEGMENT
_$E281	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?sTipText@?BB@??WindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z@4VCString@@A
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E281	ENDP
_TEXT	ENDS
PUBLIC	?RefreshTooltipRects@CToolbarHelper@@IAEXXZ	; CToolbarHelper::RefreshTooltipRects
EXTRN	?GetCurrentMessage@CSubclassWnd@@IAEPBUtagMSG@@XZ:NEAR ; CSubclassWnd::GetCurrentMessage
EXTRN	?ScDefault@CSubclasser@@IAEJPAUHWND__@@@Z:NEAR	; CSubclasser::ScDefault
EXTRN	?RelayEvent@CToolTipCtrl@@QAEXPAUtagMSG@@@Z:NEAR ; CToolTipCtrl::RelayEvent
;	COMDAT ?ScWindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT
_hRealWnd$ = 8
_msg$ = 12
_wp$ = 16
_lp$ = 20
_this$ = -8
_lr$75860 = -4
?ScWindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z PROC NEAR ; CToolbarHelper::ScWindowProc, COMDAT

; 337  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 339  : 	{

	mov	eax, DWORD PTR _msg$[ebp]
	mov	DWORD PTR -12+[ebp], eax
	cmp	DWORD PTR -12+[ebp], 5
	je	SHORT $L75859
	cmp	DWORD PTR -12+[ebp], 512		; 00000200H
	je	SHORT $L75857
	cmp	DWORD PTR -12+[ebp], 675		; 000002a3H
	je	SHORT $L75857
	jmp	SHORT $L75854
$L75857:

; 340  : 	case WM_MOUSEMOVE:
; 341  : 	case WM_MOUSELEAVE:
; 342  : 		m_tt.RelayEvent(const_cast<MSG*>(GetCurrentMessage()));

	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 32					; 00000020H
	call	?GetCurrentMessage@CSubclassWnd@@IAEPBUtagMSG@@XZ ; CSubclassWnd::GetCurrentMessage
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 76					; 0000004cH
	call	?RelayEvent@CToolTipCtrl@@QAEXPAUtagMSG@@@Z ; CToolTipCtrl::RelayEvent

; 343  : 		break;

	jmp	SHORT $L75854
$L75859:

; 347  : 			LRESULT lr = CSubclasser::ScWindowProc(hRealWnd, msg, wp, lp);

	mov	ecx, DWORD PTR _lp$[ebp]
	push	ecx
	mov	edx, DWORD PTR _wp$[ebp]
	push	edx
	mov	eax, DWORD PTR _msg$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hRealWnd$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ScWindowProc@CSubclasser@@MAEJPAUHWND__@@IIJ@Z ; CSubclasser::ScWindowProc
	mov	DWORD PTR _lr$75860[ebp], eax

; 348  : 			RefreshTooltipRects();

	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 32					; 00000020H
	call	?RefreshTooltipRects@CToolbarHelper@@IAEXXZ ; CToolbarHelper::RefreshTooltipRects

; 349  : 			return lr;

	mov	eax, DWORD PTR _lr$75860[ebp]
	jmp	SHORT $L75852
$L75854:

; 352  : 
; 353  : 	return CSubclasser::ScDefault(hRealWnd);

	mov	edx, DWORD PTR _hRealWnd$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ScDefault@CSubclasser@@IAEJPAUHWND__@@@Z ; CSubclasser::ScDefault
$L75852:

; 354  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?ScWindowProc@CToolbarHelper@@MAEJPAUHWND__@@IIJ@Z ENDP	; CToolbarHelper::ScWindowProc
_TEXT	ENDS
EXTRN	?GetItemID@CToolBar@@QBEIH@Z:NEAR		; CToolBar::GetItemID
EXTRN	?Create@CToolTipCtrl@@QAEHPAVCWnd@@K@Z:NEAR	; CToolTipCtrl::Create
EXTRN	?Activate@CToolTipCtrl@@QAEXH@Z:NEAR		; CToolTipCtrl::Activate
EXTRN	?AddTool@CToolTipCtrl@@QAEHPAVCWnd@@PBGPBUtagRECT@@I@Z:NEAR ; CToolTipCtrl::AddTool
EXTRN	?GetButtonCount@CToolBarCtrl@@QBEHXZ:NEAR	; CToolBarCtrl::GetButtonCount
EXTRN	?GetBarStyle@CControlBar@@QAEKXZ:NEAR		; CControlBar::GetBarStyle
EXTRN	?SetBarStyle@CControlBar@@QAEXK@Z:NEAR		; CControlBar::SetBarStyle
;	COMDAT ?InitTooltips@CToolbarHelper@@IAEXXZ
_TEXT	SEGMENT
_this$ = -28
_nBtnCount$ = -8
_nBtn$ = -4
_rBtn$75871 = -24
?InitTooltips@CToolbarHelper@@IAEXXZ PROC NEAR		; CToolbarHelper::InitTooltips, COMDAT

; 357  : {

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
	mov	DWORD PTR _this$[ebp], ecx

; 358  : 	if (!m_tt.Create(GetCWnd(), TTS_ALWAYSTIP))

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCWnd@CSubclassWnd@@QBEPAVCWnd@@XZ	; CSubclassWnd::GetCWnd
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 108				; 0000006cH
	call	?Create@CToolTipCtrl@@QAEHPAVCWnd@@K@Z	; CToolTipCtrl::Create
	test	eax, eax
	jne	SHORT $L75864

; 359  : 		return;

	jmp	$L75863
$L75864:

; 360  : 
; 361  : 	// hook the toolbar for mouse messages
; 362  : 	ScHookWindow(m_pToolbar->GetSafeHwnd());

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+68]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 32					; 00000020H
	call	?ScHookWindow@CSubclasser@@IAEHPAUHWND__@@@Z ; CSubclasser::ScHookWindow

; 363  : 
; 364  : 	// turn off default tooltips
; 365  : 	m_pToolbar->SetBarStyle(m_pToolbar->GetBarStyle() & ~CBRS_TOOLTIPS);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+68]
	call	?GetBarStyle@CControlBar@@QAEKXZ	; CControlBar::GetBarStyle
	and	al, -17					; ffffffefH
	push	eax
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+68]
	call	?SetBarStyle@CControlBar@@QAEXK@Z	; CControlBar::SetBarStyle

; 366  : 
; 367  : 	// and activate it
; 368  : 	m_tt.Activate(TRUE);

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 108				; 0000006cH
	call	?Activate@CToolTipCtrl@@QAEXH@Z		; CToolTipCtrl::Activate

; 369  : 
; 370  : 	// set up tools for each of the toolar buttons
; 371  : 	int nBtnCount = m_pToolbar->GetToolBarCtrl().GetButtonCount();

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+68]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?GetButtonCount@CToolBarCtrl@@QBEHXZ	; CToolBarCtrl::GetButtonCount
	mov	DWORD PTR _nBtnCount$[ebp], eax

; 372  : 
; 373  : 	for (int nBtn = 0; nBtn < nBtnCount; nBtn++)

	mov	DWORD PTR _nBtn$[ebp], 0
	jmp	SHORT $L75867
$L75868:
	mov	ecx, DWORD PTR _nBtn$[ebp]
	add	ecx, 1
	mov	DWORD PTR _nBtn$[ebp], ecx
$L75867:
	mov	edx, DWORD PTR _nBtn$[ebp]
	cmp	edx, DWORD PTR _nBtnCount$[ebp]
	jge	SHORT $L75869

; 375  : 		if (m_pToolbar->GetItemID(nBtn) != ID_SEPARATOR)

	mov	eax, DWORD PTR _nBtn$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+68]
	call	?GetItemID@CToolBar@@QBEIH@Z		; CToolBar::GetItemID
	test	eax, eax
	je	SHORT $L75870

; 377  : 			CRect rBtn;

	lea	ecx, DWORD PTR _rBtn$75871[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 378  : 			m_pToolbar->GetItemRect(nBtn, rBtn);

	lea	ecx, DWORD PTR _rBtn$75871[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	mov	esi, esp
	push	eax
	mov	edx, DWORD PTR _nBtn$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+68]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+68]
	mov	edx, DWORD PTR [eax]
	call	DWORD PTR [edx+232]
	cmp	esi, esp
	call	__chkesp

; 379  : 
; 380  : 			m_tt.AddTool(m_pToolbar, LPSTR_TEXTCALLBACK, rBtn, m_pToolbar->GetItemID(nBtn));

	mov	eax, DWORD PTR _nBtn$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+68]
	call	?GetItemID@CToolBar@@QBEIH@Z		; CToolBar::GetItemID
	push	eax
	lea	ecx, DWORD PTR _rBtn$75871[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	push	-1
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+68]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 108				; 0000006cH
	call	?AddTool@CToolTipCtrl@@QAEHPAVCWnd@@PBGPBUtagRECT@@I@Z ; CToolTipCtrl::AddTool
$L75870:

; 382  : 	}

	jmp	$L75868
$L75869:
$L75863:

; 383  : }

	pop	esi
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?InitTooltips@CToolbarHelper@@IAEXXZ ENDP		; CToolbarHelper::InitTooltips
_TEXT	ENDS
EXTRN	?SetToolRect@CToolTipCtrl@@QAEXPAVCWnd@@IPBUtagRECT@@@Z:NEAR ; CToolTipCtrl::SetToolRect
;	COMDAT ?RefreshTooltipRects@CToolbarHelper@@IAEXXZ
_TEXT	SEGMENT
_this$ = -28
_nBtnCount$ = -8
_nBtn$ = -4
_rBtn$75882 = -24
?RefreshTooltipRects@CToolbarHelper@@IAEXXZ PROC NEAR	; CToolbarHelper::RefreshTooltipRects, COMDAT

; 386  : {

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
	mov	DWORD PTR _this$[ebp], ecx

; 387  : 	int nBtnCount = m_pToolbar->GetToolBarCtrl().GetButtonCount();

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+68]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?GetButtonCount@CToolBarCtrl@@QBEHXZ	; CToolBarCtrl::GetButtonCount
	mov	DWORD PTR _nBtnCount$[ebp], eax

; 388  : 
; 389  : 	for (int nBtn = 0; nBtn < nBtnCount; nBtn++)

	mov	DWORD PTR _nBtn$[ebp], 0
	jmp	SHORT $L75878
$L75879:
	mov	ecx, DWORD PTR _nBtn$[ebp]
	add	ecx, 1
	mov	DWORD PTR _nBtn$[ebp], ecx
$L75878:
	mov	edx, DWORD PTR _nBtn$[ebp]
	cmp	edx, DWORD PTR _nBtnCount$[ebp]
	jge	SHORT $L75880

; 391  : 		if (m_pToolbar->GetItemID(nBtn) != ID_SEPARATOR)

	mov	eax, DWORD PTR _nBtn$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+68]
	call	?GetItemID@CToolBar@@QBEIH@Z		; CToolBar::GetItemID
	test	eax, eax
	je	SHORT $L75881

; 393  : 			CRect rBtn;

	lea	ecx, DWORD PTR _rBtn$75882[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 394  : 			m_pToolbar->GetItemRect(nBtn, rBtn);

	lea	ecx, DWORD PTR _rBtn$75882[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	mov	esi, esp
	push	eax
	mov	edx, DWORD PTR _nBtn$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+68]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+68]
	mov	edx, DWORD PTR [eax]
	call	DWORD PTR [edx+232]
	cmp	esi, esp
	call	__chkesp

; 395  : 
; 396  : 			m_tt.SetToolRect(m_pToolbar, m_pToolbar->GetItemID(nBtn), rBtn);

	lea	ecx, DWORD PTR _rBtn$75882[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	eax, DWORD PTR _nBtn$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+68]
	call	?GetItemID@CToolBar@@QBEIH@Z		; CToolBar::GetItemID
	push	eax
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+68]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 108				; 0000006cH
	call	?SetToolRect@CToolTipCtrl@@QAEXPAVCWnd@@IPBUtagRECT@@@Z ; CToolTipCtrl::SetToolRect
$L75881:

; 398  : 	}

	jmp	$L75879
$L75880:

; 399  : }

	pop	esi
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?RefreshTooltipRects@CToolbarHelper@@IAEXXZ ENDP	; CToolbarHelper::RefreshTooltipRects
_TEXT	ENDS
EXTRN	?GetMenuItemCount@CMenu@@QBEIXZ:NEAR		; CMenu::GetMenuItemCount
EXTRN	?GetMenuItemID@CMenu@@QBEIH@Z:NEAR		; CMenu::GetMenuItemID
EXTRN	??0CCmdUI@@QAE@XZ:NEAR				; CCmdUI::CCmdUI
;	COMDAT ?PrepareMenuItems@CToolbarHelper@@SAXPAVCMenu@@PAVCWnd@@@Z
_TEXT	SEGMENT
_pMenu$ = 8
_pWnd$ = 12
_state$ = -40
_nCmdID$75891 = -44
_pSubMenu$75894 = -48
?PrepareMenuItems@CToolbarHelper@@SAXPAVCMenu@@PAVCWnd@@@Z PROC NEAR ; CToolbarHelper::PrepareMenuItems, COMDAT

; 403  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 48					; 00000030H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-48]
	mov	ecx, 12					; 0000000cH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 404  : 	// update item states
; 405  : 	CCmdUI state;

	lea	ecx, DWORD PTR _state$[ebp]
	call	??0CCmdUI@@QAE@XZ			; CCmdUI::CCmdUI

; 406  : 	
; 407  : 	state.m_pMenu = pMenu;

	mov	eax, DWORD PTR _pMenu$[ebp]
	mov	DWORD PTR _state$[ebp+12], eax

; 408  : 	state.m_nIndexMax = pMenu->GetMenuItemCount();

	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetMenuItemCount@CMenu@@QBEIXZ		; CMenu::GetMenuItemCount
	mov	DWORD PTR _state$[ebp+32], eax

; 409  : 	
; 410  : 	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++)

	mov	DWORD PTR _state$[ebp+8], 0
	jmp	SHORT $L75888
$L75889:
	mov	ecx, DWORD PTR _state$[ebp+8]
	add	ecx, 1
	mov	DWORD PTR _state$[ebp+8], ecx
$L75888:
	mov	edx, DWORD PTR _state$[ebp+8]
	cmp	edx, DWORD PTR _state$[ebp+32]
	jae	SHORT $L75890

; 412  : 		UINT nCmdID = pMenu->GetMenuItemID(state.m_nIndex);

	mov	eax, DWORD PTR _state$[ebp+8]
	push	eax
	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetMenuItemID@CMenu@@QBEIH@Z		; CMenu::GetMenuItemID
	mov	DWORD PTR _nCmdID$75891[ebp], eax

; 413  : 		
; 414  : 		if (nCmdID == (UINT)-1) // submenu

	cmp	DWORD PTR _nCmdID$75891[ebp], -1
	jne	SHORT $L75893

; 416  : 			CMenu* pSubMenu = pMenu->GetSubMenu(state.m_nIndex);

	mov	ecx, DWORD PTR _state$[ebp+8]
	push	ecx
	mov	ecx, DWORD PTR _pMenu$[ebp]
	call	?GetSubMenu@CMenu@@QBEPAV1@H@Z		; CMenu::GetSubMenu
	mov	DWORD PTR _pSubMenu$75894[ebp], eax

; 417  : 
; 418  : 			if (pSubMenu)

	cmp	DWORD PTR _pSubMenu$75894[ebp], 0
	je	SHORT $L75895

; 419  : 				PrepareMenuItems(pSubMenu, pWnd);

	mov	edx, DWORD PTR _pWnd$[ebp]
	push	edx
	mov	eax, DWORD PTR _pSubMenu$75894[ebp]
	push	eax
	call	?PrepareMenuItems@CToolbarHelper@@SAXPAVCMenu@@PAVCWnd@@@Z ; CToolbarHelper::PrepareMenuItems
	add	esp, 8
$L75895:

; 421  : 		else if (nCmdID != 0)

	jmp	SHORT $L75897
$L75893:
	cmp	DWORD PTR _nCmdID$75891[ebp], 0
	je	SHORT $L75897

; 423  : 			state.m_nID = nCmdID;

	mov	ecx, DWORD PTR _nCmdID$75891[ebp]
	mov	DWORD PTR _state$[ebp+4], ecx

; 424  : 			pWnd->OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL);

	mov	esi, esp
	push	0
	lea	edx, DWORD PTR _state$[ebp]
	push	edx
	push	-1
	mov	eax, DWORD PTR _state$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _pWnd$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _pWnd$[ebp]
	call	DWORD PTR [edx+20]
	cmp	esi, esp
	call	__chkesp
$L75897:

; 426  : 	}

	jmp	SHORT $L75889
$L75890:

; 427  : }

	pop	edi
	pop	esi
	add	esp, 48					; 00000030H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?PrepareMenuItems@CToolbarHelper@@SAXPAVCMenu@@PAVCWnd@@@Z ENDP ; CToolbarHelper::PrepareMenuItems
_TEXT	ENDS
PUBLIC	??0CCmdUITH@@QAE@XZ				; CCmdUITH::CCmdUITH
;	COMDAT ?IsCmdEnabled@CToolbarHelper@@IBEHI@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_this$ = -52
_pParent$ = -4
_state$75905 = -48
?IsCmdEnabled@CToolbarHelper@@IBEHI@Z PROC NEAR		; CToolbarHelper::IsCmdEnabled, COMDAT

; 430  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 52					; 00000034H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-52]
	mov	ecx, 13					; 0000000dH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 431  : 	CWnd* pParent = CWnd::FromHandle(GetHwnd());

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ	; CSubclassWnd::GetHwnd
	push	eax
	call	?FromHandle@CWnd@@SGPAV1@PAUHWND__@@@Z	; CWnd::FromHandle
	mov	DWORD PTR _pParent$[ebp], eax

; 432  : 
; 433  : 	if (pParent)

	cmp	DWORD PTR _pParent$[ebp], 0
	je	SHORT $L75904

; 435  : 		CCmdUITH state;

	lea	ecx, DWORD PTR _state$75905[ebp]
	call	??0CCmdUITH@@QAE@XZ			; CCmdUITH::CCmdUITH

; 436  : 		
; 437  : 		state.m_nIndexMax = 1;

	mov	DWORD PTR _state$75905[ebp+32], 1

; 438  : 		state.m_nIndex = 0;

	mov	DWORD PTR _state$75905[ebp+8], 0

; 439  : 		state.m_nID = nCmdID;

	mov	eax, DWORD PTR _nCmdID$[ebp]
	mov	DWORD PTR _state$75905[ebp+4], eax

; 440  : 
; 441  : 		pParent->OnCmdMsg(nCmdID, CN_UPDATE_COMMAND_UI, &state, NULL);

	mov	esi, esp
	push	0
	lea	ecx, DWORD PTR _state$75905[ebp]
	push	ecx
	push	-1
	mov	edx, DWORD PTR _nCmdID$[ebp]
	push	edx
	mov	eax, DWORD PTR _pParent$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _pParent$[ebp]
	call	DWORD PTR [edx+20]
	cmp	esi, esp
	call	__chkesp

; 442  : 
; 443  : 		return state.m_bEnabled;

	mov	eax, DWORD PTR _state$75905[ebp+40]
	jmp	SHORT $L75902
$L75904:

; 445  : 
; 446  : 	return TRUE;

	mov	eax, 1
$L75902:

; 447  : }

	pop	edi
	pop	esi
	add	esp, 52					; 00000034H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?IsCmdEnabled@CToolbarHelper@@IBEHI@Z ENDP		; CToolbarHelper::IsCmdEnabled
_TEXT	ENDS
PUBLIC	?Enable@CCmdUITH@@UAEXH@Z			; CCmdUITH::Enable
PUBLIC	?SetCheck@CCmdUITH@@UAEXH@Z			; CCmdUITH::SetCheck
PUBLIC	?SetRadio@CCmdUITH@@UAEXH@Z			; CCmdUITH::SetRadio
PUBLIC	?SetText@CCmdUITH@@UAEXPBG@Z			; CCmdUITH::SetText
PUBLIC	??_7CCmdUITH@@6B@				; CCmdUITH::`vftable'
;	COMDAT ??_7CCmdUITH@@6B@
; File D:\_CODE\Shared\ToolbarHelper.cpp
CONST	SEGMENT
??_7CCmdUITH@@6B@ DD FLAT:?Enable@CCmdUITH@@UAEXH@Z	; CCmdUITH::`vftable'
	DD	FLAT:?SetCheck@CCmdUITH@@UAEXH@Z
	DD	FLAT:?SetRadio@CCmdUITH@@UAEXH@Z
	DD	FLAT:?SetText@CCmdUITH@@UAEXPBG@Z
CONST	ENDS
;	COMDAT ??0CCmdUITH@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0CCmdUITH@@QAE@XZ PROC NEAR				; CCmdUITH::CCmdUITH, COMDAT

; 24   : 	CCmdUITH() { m_bEnabled = TRUE; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CCmdUI@@QAE@XZ			; CCmdUI::CCmdUI
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CCmdUITH@@6B@ ; CCmdUITH::`vftable'
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+40], 1
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CCmdUITH@@QAE@XZ ENDP				; CCmdUITH::CCmdUITH
_TEXT	ENDS
;	COMDAT ?Enable@CCmdUITH@@UAEXH@Z
_TEXT	SEGMENT
_bOn$ = 8
_this$ = -4
?Enable@CCmdUITH@@UAEXH@Z PROC NEAR			; CCmdUITH::Enable, COMDAT

; 25   : 	virtual void Enable(BOOL bOn = TRUE) { m_bEnabled = bOn; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _bOn$[ebp]
	mov	DWORD PTR [eax+40], ecx
	mov	esp, ebp
	pop	ebp
	ret	4
?Enable@CCmdUITH@@UAEXH@Z ENDP				; CCmdUITH::Enable
_TEXT	ENDS
;	COMDAT ?SetCheck@CCmdUITH@@UAEXH@Z
_TEXT	SEGMENT
_this$ = -4
?SetCheck@CCmdUITH@@UAEXH@Z PROC NEAR			; CCmdUITH::SetCheck, COMDAT

; 26   : 	virtual void SetCheck(int /*nCheck*/ = 1) {} // dummy

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esp, ebp
	pop	ebp
	ret	4
?SetCheck@CCmdUITH@@UAEXH@Z ENDP			; CCmdUITH::SetCheck
_TEXT	ENDS
;	COMDAT ?SetRadio@CCmdUITH@@UAEXH@Z
_TEXT	SEGMENT
_this$ = -4
?SetRadio@CCmdUITH@@UAEXH@Z PROC NEAR			; CCmdUITH::SetRadio, COMDAT

; 27   : 	virtual void SetRadio(BOOL /*bOn*/ = TRUE) {} // dummy

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esp, ebp
	pop	ebp
	ret	4
?SetRadio@CCmdUITH@@UAEXH@Z ENDP			; CCmdUITH::SetRadio
_TEXT	ENDS
;	COMDAT ?SetText@CCmdUITH@@UAEXPBG@Z
_TEXT	SEGMENT
_this$ = -4
?SetText@CCmdUITH@@UAEXPBG@Z PROC NEAR			; CCmdUITH::SetText, COMDAT

; 28   : 	virtual void SetText(LPCTSTR /*lpszText*/) {} // dummy

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esp, ebp
	pop	ebp
	ret	4
?SetText@CCmdUITH@@UAEXPBG@Z ENDP			; CCmdUITH::SetText
_TEXT	ENDS
PUBLIC	?GetTip@CToolbarHelper@@SA?AVCString@@I@Z	; CToolbarHelper::GetTip
EXTRN	??0CSize@@QAE@K@Z:NEAR				; CSize::CSize
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
EXTRN	??0CString@@QAE@PBD@Z:NEAR			; CString::CString
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	?PtInRect@CRect@@QBEHUtagPOINT@@@Z:NEAR		; CRect::PtInRect
EXTRN	?GetRect@CToolBarCtrl@@QBEHHPAUtagRECT@@@Z:NEAR	; CToolBarCtrl::GetRect
EXTRN	?GetButtonSize@CToolBarCtrl@@QBEKXZ:NEAR	; CToolBarCtrl::GetButtonSize
EXTRN	__imp__lstrlenW@4:NEAR
_BSS	SEGMENT
	ALIGN	4

$SG75914 DB	01H DUP (?)
_BSS	ENDS
;	COMDAT xdata$x
; File D:\_CODE\Shared\ToolbarHelper.cpp
xdata$x	SEGMENT
$T76733	DD	019930520H
	DD	03H
	DD	FLAT:$T76735
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76735	DD	0ffffffffH
	DD	FLAT:$L76727
	DD	00H
	DD	FLAT:$L76730
	DD	01H
	DD	FLAT:$L76731
xdata$x	ENDS
;	COMDAT ?GetTip@CToolbarHelper@@IBE?AVCString@@IPAUtagPOINT@@@Z
_TEXT	SEGMENT
_nID$ = 12
_pPoint$ = 16
___$ReturnUdt$ = 8
_this$ = -328
_bOverDropBtn$ = -288
_sizeBtn$75917 = -300
_rButton$75918 = -316
_sTip$ = -292
_dm$ = -284
$T76726 = -320
$T76728 = -324
__$EHRec$ = -12
?GetTip@CToolbarHelper@@IBE?AVCString@@IPAUtagPOINT@@@Z PROC NEAR ; CToolbarHelper::GetTip, COMDAT

; 450  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76734
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 324				; 00000144H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-336]
	mov	ecx, 81					; 00000051H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T76728[ebp], 0

; 451  : 	if (!nID)

	cmp	DWORD PTR _nID$[ebp], 0
	jne	SHORT $L75913

; 452  : 		return ""; // separator

	push	OFFSET FLAT:$SG75914
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	eax, DWORD PTR $T76728[ebp]
	or	al, 1
	mov	DWORD PTR $T76728[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75912
$L75913:

; 453  : 
; 454  : 	// are we over the dropbutton?
; 455  : 	BOOL bOverDropBtn = FALSE;

	mov	DWORD PTR _bOverDropBtn$[ebp], 0

; 456  : 
; 457  : 	if (pPoint)

	cmp	DWORD PTR _pPoint$[ebp], 0
	je	$L75919

; 459  : 		CSize sizeBtn(m_pToolbar->GetToolBarCtrl().GetButtonSize());

	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+68]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?GetButtonSize@CToolBarCtrl@@QBEKXZ	; CToolBarCtrl::GetButtonSize
	push	eax
	lea	ecx, DWORD PTR _sizeBtn$75917[ebp]
	call	??0CSize@@QAE@K@Z			; CSize::CSize

; 460  : 		CRect rButton;

	lea	ecx, DWORD PTR _rButton$75918[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 461  : 
; 462  : 		m_pToolbar->GetToolBarCtrl().GetRect(nID, rButton);

	lea	ecx, DWORD PTR _rButton$75918[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	edx, DWORD PTR _nID$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+68]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?GetRect@CToolBarCtrl@@QBEHHPAUtagRECT@@@Z ; CToolBarCtrl::GetRect

; 463  : 		rButton.left += sizeBtn.cx;

	mov	ecx, DWORD PTR _rButton$75918[ebp]
	add	ecx, DWORD PTR _sizeBtn$75917[ebp]
	mov	DWORD PTR _rButton$75918[ebp], ecx

; 464  : 
; 465  : 		if (rButton.PtInRect(*pPoint))

	mov	edx, DWORD PTR _pPoint$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	mov	ecx, DWORD PTR [edx]
	push	ecx
	lea	ecx, DWORD PTR _rButton$75918[ebp]
	call	?PtInRect@CRect@@QBEHUtagPOINT@@@Z	; CRect::PtInRect
	test	eax, eax
	je	SHORT $L75919

; 466  : 			bOverDropBtn = TRUE;

	mov	DWORD PTR _bOverDropBtn$[ebp], 1
$L75919:

; 468  : 
; 469  : 	CString sTip;

	lea	ecx, DWORD PTR _sTip$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 470  : 
; 471  : 	// do we have a mapping for this
; 472  : 	THButton dm;
; 473  : 				
; 474  : 	if (m_mapTHButtons.Lookup(nID, dm))

	lea	edx, DWORD PTR _dm$[ebp]
	push	edx
	mov	eax, DWORD PTR _nID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	?Lookup@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEHIAAUTHButton@CToolbarHelper@@@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::Lookup
	test	eax, eax
	je	$L75927

; 476  : 		if (!bOverDropBtn)

	cmp	DWORD PTR _bOverDropBtn$[ebp], 0
	jne	$L75926

; 478  : 			// try the default item first
; 479  : 			if (dm.nDefCmdID && IsCmdEnabled(dm.nDefCmdID)/* && (dm.nDefCmdID != nID)*/)

	cmp	DWORD PTR _dm$[ebp+8], 0
	je	$L75926
	mov	ecx, DWORD PTR _dm$[ebp+8]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsCmdEnabled@CToolbarHelper@@IBEHI@Z	; CToolbarHelper::IsCmdEnabled
	test	eax, eax
	je	$L75926

; 481  : 				sTip = GetTip(dm.nDefCmdID);

	mov	edx, DWORD PTR _dm$[ebp+8]
	push	edx
	lea	eax, DWORD PTR $T76726[ebp]
	push	eax
	call	?GetTip@CToolbarHelper@@SA?AVCString@@I@Z ; CToolbarHelper::GetTip
	add	esp, 8
	mov	DWORD PTR -332+[ebp], eax
	mov	ecx, DWORD PTR -332+[ebp]
	mov	DWORD PTR -336+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	edx, DWORD PTR -336+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T76726[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 482  : 
; 483  : 				if (!sTip.IsEmpty())

	lea	ecx, DWORD PTR _sTip$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75926

; 484  : 					return sTip;

	lea	eax, DWORD PTR _sTip$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T76728[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76728[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L75912
$L75926:

; 487  : 
; 488  : 		// try override tip
; 489  : 		if (lstrlen(dm.szTip))

	mov	esi, esp
	lea	edx, DWORD PTR _dm$[ebp+14]
	push	edx
	call	DWORD PTR __imp__lstrlenW@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L75927

; 490  : 			return dm.szTip;

	lea	eax, DWORD PTR _dm$[ebp+14]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	ecx, DWORD PTR $T76728[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76728[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L75912
$L75927:

; 492  : 
; 493  : 	return GetTip(nID);

	mov	edx, DWORD PTR _nID$[ebp]
	push	edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	call	?GetTip@CToolbarHelper@@SA?AVCString@@I@Z ; CToolbarHelper::GetTip
	add	esp, 8
	mov	ecx, DWORD PTR $T76728[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76728[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L75912:

; 494  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 336				; 00000150H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76727:
	mov	eax, DWORD PTR $T76728[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76729
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76729:
	ret	0
$L76730:
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76731:
	lea	ecx, DWORD PTR $T76726[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76734:
	mov	eax, OFFSET FLAT:$T76733
	jmp	___CxxFrameHandler
text$x	ENDS
?GetTip@CToolbarHelper@@IBE?AVCString@@IPAUtagPOINT@@@Z ENDP ; CToolbarHelper::GetTip
EXTRN	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z:NEAR	; Misc::Trim
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	?Right@CString@@QBE?AV1@H@Z:NEAR		; CString::Right
EXTRN	?Replace@CString@@QAEHPBG0@Z:NEAR		; CString::Replace
EXTRN	?Find@CString@@QBEHG@Z:NEAR			; CString::Find
_BSS	SEGMENT
	ALIGN	4

$SG75939 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\ToolbarHelper.cpp
data	SEGMENT
$SG75940 DB	'.', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76747	DD	019930520H
	DD	03H
	DD	FLAT:$T76749
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76749	DD	0ffffffffH
	DD	FLAT:$L76743
	DD	00H
	DD	FLAT:$L76741
	DD	01H
	DD	FLAT:$L76742
xdata$x	ENDS
;	COMDAT ?GetTip@CToolbarHelper@@SA?AVCString@@I@Z
_TEXT	SEGMENT
_nID$ = 12
___$ReturnUdt$ = 8
_sTip$ = -16
_nStartTip$75935 = -20
$T76740 = -24
$T76744 = -28
__$EHRec$ = -12
?GetTip@CToolbarHelper@@SA?AVCString@@I@Z PROC NEAR	; CToolbarHelper::GetTip, COMDAT

; 497  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76748
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
	mov	DWORD PTR $T76744[ebp], 0

; 498  : 	CString sTip;

	lea	ecx, DWORD PTR _sTip$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 499  : 
; 500  : 	if (nID > 0 && sTip.LoadString(nID) && !sTip.IsEmpty())

	cmp	DWORD PTR _nID$[ebp], 0
	jbe	$L75934
	mov	eax, DWORD PTR _nID$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	?LoadStringW@CString@@QAEHI@Z		; CString::LoadStringW
	test	eax, eax
	je	$L75934
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75934

; 502  : 		// tip text starts at '\n' 
; 503  : 		int nStartTip = sTip.Find('\n');

	push	10					; 0000000aH
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	?Find@CString@@QBEHG@Z			; CString::Find
	mov	DWORD PTR _nStartTip$75935[ebp], eax

; 504  : 		
; 505  : 		if (nStartTip != -1) 

	cmp	DWORD PTR _nStartTip$75935[ebp], -1
	je	SHORT $L75936

; 506  : 			sTip = sTip.Right(sTip.GetLength() - nStartTip - 1);

	lea	ecx, DWORD PTR _sTip$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	sub	eax, DWORD PTR _nStartTip$75935[ebp]
	sub	eax, 1
	push	eax
	lea	ecx, DWORD PTR $T76740[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	?Right@CString@@QBE?AV1@H@Z		; CString::Right
	mov	DWORD PTR -32+[ebp], eax
	mov	edx, DWORD PTR -32+[ebp]
	mov	DWORD PTR -36+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	eax, DWORD PTR -36+[ebp]
	push	eax
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T76740[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 507  : 
; 508  : 		else // strip '...' if present

	jmp	SHORT $L75938
$L75936:

; 509  : 			sTip.Replace(_T("."), _T(""));

	push	OFFSET FLAT:$SG75939
	push	OFFSET FLAT:$SG75940
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace
$L75938:

; 510  : 
; 511  : 		Misc::Trim(sTip);

	push	0
	lea	ecx, DWORD PTR _sTip$[ebp]
	push	ecx
	call	?Trim@Misc@@YAAAVCString@@AAV2@PBG@Z	; Misc::Trim
	add	esp, 8
$L75934:

; 513  : 	
; 514  : 	return sTip;

	lea	edx, DWORD PTR _sTip$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T76744[ebp]
	or	al, 1
	mov	DWORD PTR $T76744[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 515  : }

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
$L76741:
	lea	ecx, DWORD PTR _sTip$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76742:
	lea	ecx, DWORD PTR $T76740[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76743:
	mov	eax, DWORD PTR $T76744[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76745
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76745:
	ret	0
$L76748:
	mov	eax, OFFSET FLAT:$T76747
	jmp	___CxxFrameHandler
text$x	ENDS
?GetTip@CToolbarHelper@@SA?AVCString@@I@Z ENDP		; CToolbarHelper::GetTip
PUBLIC	?EnableMultilineText@CToolbarHelper@@QAEXHH@Z	; CToolbarHelper::EnableMultilineText
;	COMDAT ?EnableMultilineText@CToolbarHelper@@QAEXHH@Z
_TEXT	SEGMENT
_bEnable$ = 8
_nWidth$ = 12
_this$ = -4
?EnableMultilineText@CToolbarHelper@@QAEXHH@Z PROC NEAR	; CToolbarHelper::EnableMultilineText, COMDAT

; 518  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 519  : 	m_bMultiline = bEnable;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _bEnable$[ebp]
	mov	DWORD PTR [eax+100], ecx

; 520  : 	m_nMultilineWidth = nWidth;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nWidth$[ebp]
	mov	DWORD PTR [edx+104], eax

; 521  : }

	mov	esp, ebp
	pop	ebp
	ret	8
?EnableMultilineText@CToolbarHelper@@QAEXHH@Z ENDP	; CToolbarHelper::EnableMultilineText
_TEXT	ENDS
PUBLIC	?ProcessMessage@CToolbarHelper@@QAEHPAUtagMSG@@@Z ; CToolbarHelper::ProcessMessage
PUBLIC	?IsWindowEnabled@CSubclassWnd@@QBEHXZ		; CSubclassWnd::IsWindowEnabled
EXTRN	?IsWindowEnabled@CWnd@@QBEHXZ:NEAR		; CWnd::IsWindowEnabled
EXTRN	__imp__isupper:NEAR
EXTRN	__imp__islower:NEAR
EXTRN	__imp__toupper:NEAR
EXTRN	__imp__tolower:NEAR
EXTRN	?KeyIsPressed@Misc@@YAHK@Z:NEAR			; Misc::KeyIsPressed
;	COMDAT ?ProcessMessage@CToolbarHelper@@QAEHPAUtagMSG@@@Z
_TEXT	SEGMENT
_pMsg$ = 8
_this$ = -296
_nKey$75953 = -4
_cLower$75955 = -12
_cUpper$75956 = -8
_pos$75966 = -16
_dm$75970 = -288
_nCmdID$75971 = -292
?ProcessMessage@CToolbarHelper@@QAEHPAUtagMSG@@@Z PROC NEAR ; CToolbarHelper::ProcessMessage, COMDAT

; 524  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 296				; 00000128H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-296]
	mov	ecx, 74					; 0000004aH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 525  : 	if (!IsWindowEnabled() || !m_pToolbar->IsWindowEnabled())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsWindowEnabled@CSubclassWnd@@QBEHXZ	; CSubclassWnd::IsWindowEnabled
	test	eax, eax
	je	SHORT $L75951
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+68]
	call	?IsWindowEnabled@CWnd@@QBEHXZ		; CWnd::IsWindowEnabled
	test	eax, eax
	jne	SHORT $L75950
$L75951:

; 526  : 		return FALSE;

	xor	eax, eax
	jmp	$L75949
$L75950:

; 527  : 
; 528  : 	// see if key press matches any hotkey
; 529  : 	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam != VK_MENU && Misc::KeyIsPressed(VK_MENU))

	mov	ecx, DWORD PTR _pMsg$[ebp]
	cmp	DWORD PTR [ecx+4], 260			; 00000104H
	jne	$L75969
	mov	edx, DWORD PTR _pMsg$[ebp]
	cmp	DWORD PTR [edx+8], 18			; 00000012H
	je	$L75969
	push	18					; 00000012H
	call	?KeyIsPressed@Misc@@YAHK@Z		; Misc::KeyIsPressed
	add	esp, 4
	test	eax, eax
	je	$L75969

; 531  : 		int nKey = (int)pMsg->wParam;

	mov	eax, DWORD PTR _pMsg$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR _nKey$75953[ebp], ecx

; 532  : 		char cLower = 0, cUpper = 0;

	mov	BYTE PTR _cLower$75955[ebp], 0
	mov	BYTE PTR _cUpper$75956[ebp], 0

; 533  : 
; 534  : 		if (islower(nKey))

	mov	esi, esp
	mov	edx, DWORD PTR _nKey$75953[ebp]
	push	edx
	call	DWORD PTR __imp__islower
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L75957

; 536  : 			cLower = (char)nKey;

	mov	al, BYTE PTR _nKey$75953[ebp]
	mov	BYTE PTR _cLower$75955[ebp], al

; 537  : 			cUpper = (char)toupper(nKey);

	mov	esi, esp
	mov	ecx, DWORD PTR _nKey$75953[ebp]
	push	ecx
	call	DWORD PTR __imp__toupper
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	mov	BYTE PTR _cUpper$75956[ebp], al

; 539  : 		else if (isupper(nKey))

	jmp	SHORT $L75964
$L75957:
	mov	esi, esp
	mov	edx, DWORD PTR _nKey$75953[ebp]
	push	edx
	call	DWORD PTR __imp__isupper
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L75961

; 541  : 			cUpper = (char)nKey;

	mov	al, BYTE PTR _nKey$75953[ebp]
	mov	BYTE PTR _cUpper$75956[ebp], al

; 542  : 			cLower = (char)tolower(nKey);

	mov	esi, esp
	mov	ecx, DWORD PTR _nKey$75953[ebp]
	push	ecx
	call	DWORD PTR __imp__tolower
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	mov	BYTE PTR _cLower$75955[ebp], al

; 544  : 		else

	jmp	SHORT $L75964
$L75961:

; 545  : 			cUpper = cLower = (char)nKey;

	mov	dl, BYTE PTR _nKey$75953[ebp]
	mov	BYTE PTR _cLower$75955[ebp], dl
	mov	al, BYTE PTR _cLower$75955[ebp]
	mov	BYTE PTR _cUpper$75956[ebp], al
$L75964:

; 546  : 
; 547  : 		// iterate the buttons the hard way
; 548  : 		POSITION pos = m_mapTHButtons.GetStartPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	?GetStartPosition@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEPAU__POSITION@@XZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetStartPosition
	mov	DWORD PTR _pos$75966[ebp], eax
$L75968:

; 549  : 
; 550  : 		while (pos)

	cmp	DWORD PTR _pos$75966[ebp], 0
	je	SHORT $L75969

; 552  : 			THButton dm;
; 553  : 			UINT nCmdID = 0;

	mov	DWORD PTR _nCmdID$75971[ebp], 0

; 554  : 
; 555  : 			m_mapTHButtons.GetNextAssoc(pos, nCmdID, dm);

	lea	ecx, DWORD PTR _dm$75970[ebp]
	push	ecx
	lea	edx, DWORD PTR _nCmdID$75971[ebp]
	push	edx
	lea	eax, DWORD PTR _pos$75966[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	?GetNextAssoc@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEXAAPAU__POSITION@@AAIAAUTHButton@CToolbarHelper@@@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetNextAssoc

; 556  : 
; 557  : 			if (nCmdID && (dm.cHotKey == cLower || dm.cHotKey == cUpper))

	cmp	DWORD PTR _nCmdID$75971[ebp], 0
	je	SHORT $L75972
	mov	ecx, DWORD PTR _dm$75970[ebp+12]
	and	ecx, 65535				; 0000ffffH
	movsx	edx, BYTE PTR _cLower$75955[ebp]
	cmp	ecx, edx
	je	SHORT $L75973
	mov	eax, DWORD PTR _dm$75970[ebp+12]
	and	eax, 65535				; 0000ffffH
	movsx	ecx, BYTE PTR _cUpper$75956[ebp]
	cmp	eax, ecx
	jne	SHORT $L75972
$L75973:

; 559  : 				DisplayDropMenu(nCmdID, TRUE);

	push	1
	mov	edx, DWORD PTR _nCmdID$75971[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DisplayDropMenu@CToolbarHelper@@IAEHIH@Z ; CToolbarHelper::DisplayDropMenu

; 560  : 				return TRUE;

	mov	eax, 1
	jmp	SHORT $L75949
$L75972:

; 562  : 		}

	jmp	SHORT $L75968
$L75969:

; 564  : 
; 565  : 	return FALSE;

	xor	eax, eax
$L75949:

; 566  : }

	pop	edi
	pop	esi
	add	esp, 296				; 00000128H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ProcessMessage@CToolbarHelper@@QAEHPAUtagMSG@@@Z ENDP	; CToolbarHelper::ProcessMessage
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
PUBLIC	?Serialize@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAEXAAVCArchive@@@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::Serialize
PUBLIC	?Dump@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UBEXAAVCDumpContext@@@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::Dump
PUBLIC	?AssertValid@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UBEXXZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::AssertValid
PUBLIC	??_7?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@6B@ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::`vftable'
PUBLIC	??_G?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAEPAXI@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::`scalar deleting destructor'
PUBLIC	??_E?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAEPAXI@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::`vector deleting destructor'
EXTRN	?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ:NEAR ; CObject::GetRuntimeClass
EXTRN	??1CObject@@UAE@XZ:NEAR				; CObject::~CObject
EXTRN	??0CObject@@IAE@XZ:NEAR				; CObject::CObject
;	COMDAT ??_7?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@6B@
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
CONST	SEGMENT
??_7?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@6B@ DD FLAT:?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::`vftable'
	DD	FLAT:??_E?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAEPAXI@Z
	DD	FLAT:?Serialize@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UBEXXZ
	DD	FLAT:?Dump@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UBEXAAVCDumpContext@@@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76763	DD	019930520H
	DD	01H
	DD	FLAT:$T76765
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76765	DD	0ffffffffH
	DD	FLAT:$L76761
xdata$x	ENDS
;	COMDAT ??0?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAE@H@Z
_TEXT	SEGMENT
_nBlockSize$ = 8
_this$ = -16
__$EHRec$ = -12
??0?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAE@H@Z PROC NEAR ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>, COMDAT

; 1201 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76764
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
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@6B@ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::`vftable'
$L76062:

; 1202 : 	ASSERT(nBlockSize > 0);

	cmp	DWORD PTR _nBlockSize$[ebp], 0
	jg	SHORT $L76065
	push	1202					; 000004b2H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76065
	int	3
$L76065:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76062

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
$L76761:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L76764:
	mov	eax, OFFSET FLAT:$T76763
	jmp	___CxxFrameHandler
text$x	ENDS
??0?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAE@H@Z ENDP ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>
PUBLIC	?GetAssocAt@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@IBEPAUCAssoc@1@IAAI@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetAssocAt
;	COMDAT ?Lookup@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEHIAAUTHButton@CToolbarHelper@@@Z
_TEXT	SEGMENT
_key$ = 8
_rValue$ = 12
_this$ = -12
_nHash$ = -8
_pAssoc$ = -4
?Lookup@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEHIAAUTHButton@CToolbarHelper@@@Z PROC NEAR ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::Lookup, COMDAT

; 1341 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi
	push	edi
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
	call	?GetAssocAt@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@IBEPAUCAssoc@1@IAAI@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetAssocAt
	mov	DWORD PTR _pAssoc$[ebp], eax

; 1346 : 	if (pAssoc == NULL)

	cmp	DWORD PTR _pAssoc$[ebp], 0
	jne	SHORT $L76073

; 1347 : 		return FALSE;  // not in map

	xor	eax, eax
	jmp	SHORT $L76070
$L76073:

; 1348 : 
; 1349 : 	rValue = pAssoc->value;

	mov	esi, DWORD PTR _pAssoc$[ebp]
	add	esi, 12					; 0000000cH
	mov	ecx, 68					; 00000044H
	mov	edi, DWORD PTR _rValue$[ebp]
	rep movsd

; 1350 : 	return TRUE;

	mov	eax, 1
$L76070:

; 1351 : }

	pop	edi
	pop	esi
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Lookup@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEHIAAUTHButton@CToolbarHelper@@@Z ENDP ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::Lookup
_TEXT	ENDS
PUBLIC	?InitHashTable@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXIH@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::InitHashTable
PUBLIC	?NewAssoc@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@IAEPAUCAssoc@1@XZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::NewAssoc
;	COMDAT ??A?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEAAUTHButton@CToolbarHelper@@I@Z
_TEXT	SEGMENT
_key$ = 8
_this$ = -12
_nHash$ = -8
_pAssoc$ = -4
??A?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEAAUTHButton@CToolbarHelper@@I@Z PROC NEAR ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::operator[], COMDAT

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
	call	?GetAssocAt@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@IBEPAUCAssoc@1@IAAI@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetAssocAt
	mov	DWORD PTR _pAssoc$[ebp], eax
	cmp	DWORD PTR _pAssoc$[ebp], 0
	jne	SHORT $L76080

; 1362 : 		if (m_pHashTable == NULL)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L76081

; 1363 : 			InitHashTable(m_nHashTableSize);

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?InitHashTable@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXIH@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::InitHashTable
$L76081:

; 1364 : 
; 1365 : 		// it doesn't exist, add a new Association
; 1366 : 		pAssoc = NewAssoc();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?NewAssoc@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@IAEPAUCAssoc@1@XZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::NewAssoc
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
$L76080:

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
??A?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEAAUTHButton@CToolbarHelper@@I@Z ENDP ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::operator[]
_TEXT	ENDS
PUBLIC	?DestructElements@@YGXPAUTHButton@CToolbarHelper@@H@Z ; DestructElements
PUBLIC	?DestructElements@@YGXPAIH@Z			; DestructElements
EXTRN	?FreeDataChain@CPlex@@QAEXXZ:NEAR		; CPlex::FreeDataChain
;	COMDAT ?RemoveAll@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXXZ
_TEXT	SEGMENT
_this$ = -16
_nHash$76086 = -4
_pAssoc$76090 = -8
$T76774 = -12
?RemoveAll@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXXZ PROC NEAR ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::RemoveAll, COMDAT

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
	je	SHORT $L76089

; 1245 : 		// destroy elements (values and keys)
; 1246 : 		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)

	mov	DWORD PTR _nHash$76086[ebp], 0
	jmp	SHORT $L76087
$L76088:
	mov	edx, DWORD PTR _nHash$76086[ebp]
	add	edx, 1
	mov	DWORD PTR _nHash$76086[ebp], edx
$L76087:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nHash$76086[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jae	SHORT $L76089

; 1248 : 			CAssoc* pAssoc;
; 1249 : 			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _nHash$76086[ebp]
	mov	edx, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR _pAssoc$76090[ebp], edx

; 1250 : 			  pAssoc = pAssoc->pNext)

	jmp	SHORT $L76091
$L76092:
	mov	eax, DWORD PTR _pAssoc$76090[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssoc$76090[ebp], ecx
$L76091:
	cmp	DWORD PTR _pAssoc$76090[ebp], 0
	je	SHORT $L76093

; 1252 : 				DestructElements<VALUE>(&pAssoc->value, 1);

	push	1
	mov	edx, DWORD PTR _pAssoc$76090[ebp]
	add	edx, 12					; 0000000cH
	push	edx
	call	?DestructElements@@YGXPAUTHButton@CToolbarHelper@@H@Z ; DestructElements

; 1253 : 				DestructElements<KEY>(&pAssoc->key, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$76090[ebp]
	add	eax, 8
	push	eax
	call	?DestructElements@@YGXPAIH@Z		; DestructElements

; 1254 : 			}

	jmp	SHORT $L76092
$L76093:

; 1255 : 		}

	jmp	SHORT $L76088
$L76089:

; 1257 : 
; 1258 : 	// free hash table
; 1259 : 	delete[] m_pHashTable;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR $T76774[ebp], edx
	mov	eax, DWORD PTR $T76774[ebp]
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
?RemoveAll@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXXZ ENDP ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::RemoveAll
_TEXT	ENDS
;	COMDAT ?GetStartPosition@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetStartPosition@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEPAU__POSITION@@XZ PROC NEAR ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetStartPosition, COMDAT

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
?GetStartPosition@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEPAU__POSITION@@XZ ENDP ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetStartPosition
_TEXT	ENDS
EXTRN	?AfxIsValidAddress@@YGHPBXIH@Z:NEAR		; AfxIsValidAddress
;	COMDAT ?GetNextAssoc@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEXAAPAU__POSITION@@AAIAAUTHButton@CToolbarHelper@@@Z
_TEXT	SEGMENT
_rNextPosition$ = 8
_rKey$ = 12
_rValue$ = 16
_this$ = -20
_pAssocRet$ = -4
_nBucket$76128 = -12
_pAssocNext$ = -8
_nBucket$76144 = -16
?GetNextAssoc@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEXAAPAU__POSITION@@AAIAAUTHButton@CToolbarHelper@@@Z PROC NEAR ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetNextAssoc, COMDAT

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
$L76115:

; 1410 : 	ASSERT(m_pHashTable != NULL);  // never call on empty map

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	jne	SHORT $L76118
	push	1410					; 00000582H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76118
	int	3
$L76118:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76115

; 1411 : 
; 1412 : 	CAssoc* pAssocRet = (CAssoc*)rNextPosition;

	mov	eax, DWORD PTR _rNextPosition$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssocRet$[ebp], ecx
$L76121:

; 1413 : 	ASSERT(pAssocRet != NULL);

	cmp	DWORD PTR _pAssocRet$[ebp], 0
	jne	SHORT $L76124
	push	1413					; 00000585H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76124
	int	3
$L76124:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76121

; 1414 : 
; 1415 : 	if (pAssocRet == (CAssoc*) BEFORE_START_POSITION)

	cmp	DWORD PTR _pAssocRet$[ebp], -1
	jne	SHORT $L76135

; 1417 : 		// find the first association
; 1418 : 		for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)

	mov	DWORD PTR _nBucket$76128[ebp], 0
	jmp	SHORT $L76129
$L76130:
	mov	eax, DWORD PTR _nBucket$76128[ebp]
	add	eax, 1
	mov	DWORD PTR _nBucket$76128[ebp], eax
$L76129:
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nBucket$76128[ebp]
	cmp	edx, DWORD PTR [ecx+8]
	jae	SHORT $L76131

; 1419 : 			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _nBucket$76128[ebp]
	mov	eax, DWORD PTR [ecx+edx*4]
	mov	DWORD PTR _pAssocRet$[ebp], eax
	cmp	DWORD PTR _pAssocRet$[ebp], 0
	je	SHORT $L76132

; 1420 : 				break;

	jmp	SHORT $L76131
$L76132:

; 1421 : 		ASSERT(pAssocRet != NULL);  // must find something

	jmp	SHORT $L76130
$L76131:
	cmp	DWORD PTR _pAssocRet$[ebp], 0
	jne	SHORT $L76136
	push	1421					; 0000058dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76136
	int	3
$L76136:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76131
$L76135:

; 1423 : 
; 1424 : 	// find next association
; 1425 : 	ASSERT(AfxIsValidAddress(pAssocRet, sizeof(CAssoc)));

	push	1
	push	284					; 0000011cH
	mov	edx, DWORD PTR _pAssocRet$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76141
	push	1425					; 00000591H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76141
	int	3
$L76141:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76135

; 1426 : 	CAssoc* pAssocNext;
; 1427 : 	if ((pAssocNext = pAssocRet->pNext) == NULL)

	mov	ecx, DWORD PTR _pAssocRet$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _pAssocNext$[ebp], edx
	cmp	DWORD PTR _pAssocNext$[ebp], 0
	jne	SHORT $L76147

; 1429 : 		// go to next bucket
; 1430 : 		for (UINT nBucket = pAssocRet->nHashValue + 1;

	mov	eax, DWORD PTR _pAssocRet$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	add	ecx, 1
	mov	DWORD PTR _nBucket$76144[ebp], ecx

; 1431 : 		  nBucket < m_nHashTableSize; nBucket++)

	jmp	SHORT $L76145
$L76146:
	mov	edx, DWORD PTR _nBucket$76144[ebp]
	add	edx, 1
	mov	DWORD PTR _nBucket$76144[ebp], edx
$L76145:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nBucket$76144[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jae	SHORT $L76147

; 1432 : 			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _nBucket$76144[ebp]
	mov	edx, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR _pAssocNext$[ebp], edx
	cmp	DWORD PTR _pAssocNext$[ebp], 0
	je	SHORT $L76148

; 1433 : 				break;

	jmp	SHORT $L76147
$L76148:

; 1434 : 	}

	jmp	SHORT $L76146
$L76147:

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

	mov	esi, DWORD PTR _pAssocRet$[ebp]
	add	esi, 12					; 0000000cH
	mov	ecx, 68					; 00000044H
	mov	edi, DWORD PTR _rValue$[ebp]
	rep movsd

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
?GetNextAssoc@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEXAAPAU__POSITION@@AAIAAUTHButton@CToolbarHelper@@@Z ENDP ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetNextAssoc
_TEXT	ENDS
;	COMDAT xdata$x
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
xdata$x	SEGMENT
$T76783	DD	019930520H
	DD	01H
	DD	FLAT:$T76785
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76785	DD	0ffffffffH
	DD	FLAT:$L76781
xdata$x	ENDS
;	COMDAT ??1?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAE@XZ PROC NEAR ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::~CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>, COMDAT

; 1270 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76784
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
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@6B@ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1271 : 	RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXXZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::RemoveAll
$L76153:

; 1272 : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L76156
	push	1272					; 000004f8H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76156
	int	3
$L76156:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76153

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
$L76781:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L76784:
	mov	eax, OFFSET FLAT:$T76783
	jmp	___CxxFrameHandler
text$x	ENDS
??1?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAE@XZ ENDP ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::~CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>
PUBLIC	?SetAt@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXIAAUTHButton@CToolbarHelper@@@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::SetAt
PUBLIC	?SerializeElements@@YGXAAVCArchive@@PAIH@Z	; SerializeElements
PUBLIC	?SerializeElements@@YGXAAVCArchive@@PAUTHButton@CToolbarHelper@@H@Z ; SerializeElements
EXTRN	?ReadCount@CArchive@@QAEKXZ:NEAR		; CArchive::ReadCount
EXTRN	?WriteCount@CArchive@@QAEXK@Z:NEAR		; CArchive::WriteCount
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	?IsStoring@CArchive@@QBEHXZ:NEAR		; CArchive::IsStoring
;	COMDAT ?Serialize@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT
_ar$ = 8
_this$ = -292
_nHash$76167 = -4
_pAssoc$76171 = -8
_nNewCount$76188 = -12
_newKey$76192 = -16
_newValue$76193 = -288
?Serialize@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAEXAAVCArchive@@@Z PROC NEAR ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::Serialize, COMDAT

; 1445 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 292				; 00000124H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-292]
	mov	ecx, 73					; 00000049H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
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
	je	$L76161

; 1452 : 		ar.WriteCount(m_nCount);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?WriteCount@CArchive@@QAEXK@Z		; CArchive::WriteCount

; 1453 : 		if (m_nCount == 0)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jne	SHORT $L76162

; 1454 : 			return;  // nothing more to do

	jmp	$L76160
$L76162:

; 1455 : 
; 1456 : 		ASSERT(m_pHashTable != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	jne	SHORT $L76166
	push	1456					; 000005b0H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76166
	int	3
$L76166:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76162

; 1457 : 		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)

	mov	DWORD PTR _nHash$76167[ebp], 0
	jmp	SHORT $L76168
$L76169:
	mov	ecx, DWORD PTR _nHash$76167[ebp]
	add	ecx, 1
	mov	DWORD PTR _nHash$76167[ebp], ecx
$L76168:
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nHash$76167[ebp]
	cmp	eax, DWORD PTR [edx+8]
	jae	SHORT $L76170

; 1459 : 			CAssoc* pAssoc;
; 1460 : 			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR _nHash$76167[ebp]
	mov	ecx, DWORD PTR [edx+eax*4]
	mov	DWORD PTR _pAssoc$76171[ebp], ecx

; 1461 : 			  pAssoc = pAssoc->pNext)

	jmp	SHORT $L76172
$L76173:
	mov	edx, DWORD PTR _pAssoc$76171[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _pAssoc$76171[ebp], eax
$L76172:
	cmp	DWORD PTR _pAssoc$76171[ebp], 0
	je	SHORT $L76174

; 1463 : 				SerializeElements<KEY>(ar, &pAssoc->key, 1);

	push	1
	mov	ecx, DWORD PTR _pAssoc$76171[ebp]
	add	ecx, 8
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAIH@Z ; SerializeElements

; 1464 : 				SerializeElements<VALUE>(ar, &pAssoc->value, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$76171[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	call	?SerializeElements@@YGXAAVCArchive@@PAUTHButton@CToolbarHelper@@H@Z ; SerializeElements

; 1465 : 			}

	jmp	SHORT $L76173
$L76174:

; 1466 : 		}

	jmp	SHORT $L76169
$L76170:

; 1468 : 	else

	jmp	SHORT $L76191
$L76161:

; 1470 : 		DWORD nNewCount = ar.ReadCount();

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?ReadCount@CArchive@@QAEKXZ		; CArchive::ReadCount
	mov	DWORD PTR _nNewCount$76188[ebp], eax
$L76190:

; 1471 : 		while (nNewCount--)

	mov	edx, DWORD PTR _nNewCount$76188[ebp]
	mov	eax, DWORD PTR _nNewCount$76188[ebp]
	sub	eax, 1
	mov	DWORD PTR _nNewCount$76188[ebp], eax
	test	edx, edx
	je	SHORT $L76191

; 1473 : 			KEY newKey;
; 1474 : 			VALUE newValue;
; 1475 : 			SerializeElements<KEY>(ar, &newKey, 1);

	push	1
	lea	ecx, DWORD PTR _newKey$76192[ebp]
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAIH@Z ; SerializeElements

; 1476 : 			SerializeElements<VALUE>(ar, &newValue, 1);

	push	1
	lea	eax, DWORD PTR _newValue$76193[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	call	?SerializeElements@@YGXAAVCArchive@@PAUTHButton@CToolbarHelper@@H@Z ; SerializeElements

; 1477 : 			SetAt(newKey, newValue);

	lea	edx, DWORD PTR _newValue$76193[ebp]
	push	edx
	mov	eax, DWORD PTR _newKey$76192[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetAt@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXIAAUTHButton@CToolbarHelper@@@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::SetAt

; 1478 : 		}

	jmp	SHORT $L76190
$L76191:
$L76160:

; 1480 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 292				; 00000124H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Serialize@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAEXAAVCArchive@@@Z ENDP ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::Serialize
_TEXT	ENDS
PUBLIC	??_C@_05KIFI@with?5?$AA@			; `string'
PUBLIC	??_C@_09LOPF@?5elements?$AA@			; `string'
PUBLIC	??_C@_03DANH@?6?7?$FL?$AA@			; `string'
PUBLIC	?DumpElements@@YGXAAVCDumpContext@@PBIH@Z	; DumpElements
PUBLIC	??_C@_04MNLE@?$FN?5?$DN?5?$AA@			; `string'
PUBLIC	?DumpElements@@YGXAAVCDumpContext@@PBUTHButton@CToolbarHelper@@H@Z ; DumpElements
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
;	COMDAT ?Dump@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT
_dc$ = 8
_this$ = -284
_key$76211 = -276
_val$76212 = -272
_pos$76213 = -280
?Dump@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UBEXAAVCDumpContext@@@Z PROC NEAR ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::Dump, COMDAT

; 1485 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 284				; 0000011cH
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-284]
	mov	ecx, 71					; 00000047H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
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
	jle	SHORT $L76216

; 1491 : 		// Dump in format "[key] -> value"
; 1492 : 		KEY key;
; 1493 : 		VALUE val;
; 1494 : 
; 1495 : 		POSITION pos = GetStartPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetStartPosition@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEPAU__POSITION@@XZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetStartPosition
	mov	DWORD PTR _pos$76213[ebp], eax
$L76215:

; 1496 : 		while (pos != NULL)

	cmp	DWORD PTR _pos$76213[ebp], 0
	je	SHORT $L76216

; 1498 : 			GetNextAssoc(pos, key, val);

	lea	eax, DWORD PTR _val$76212[ebp]
	push	eax
	lea	ecx, DWORD PTR _key$76211[ebp]
	push	ecx
	lea	edx, DWORD PTR _pos$76213[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetNextAssoc@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QBEXAAPAU__POSITION@@AAIAAUTHButton@CToolbarHelper@@@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetNextAssoc

; 1499 : 			dc << "\n\t[";

	push	OFFSET FLAT:??_C@_03DANH@?6?7?$FL?$AA@	; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1500 : 			DumpElements<KEY>(dc, &key, 1);

	push	1
	lea	eax, DWORD PTR _key$76211[ebp]
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
	lea	edx, DWORD PTR _val$76212[ebp]
	push	edx
	mov	eax, DWORD PTR _dc$[ebp]
	push	eax
	call	?DumpElements@@YGXAAVCDumpContext@@PBUTHButton@CToolbarHelper@@H@Z ; DumpElements

; 1503 : 		}

	jmp	SHORT $L76215
$L76216:

; 1505 : 
; 1506 : 	dc << "\n";

	push	OFFSET FLAT:??_C@_01BJG@?6?$AA@		; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1507 : }

	pop	edi
	add	esp, 284				; 0000011cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Dump@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UBEXAAVCDumpContext@@@Z ENDP ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::Dump
_TEXT	ENDS
EXTRN	?AssertValid@CObject@@UBEXXZ:NEAR		; CObject::AssertValid
;	COMDAT ?AssertValid@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UBEXXZ
_TEXT	SEGMENT
_this$ = -4
?AssertValid@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UBEXXZ PROC NEAR ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::AssertValid, COMDAT

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
$L76235:

; 1513 : 
; 1514 : 	ASSERT(m_nHashTableSize > 0);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	ja	SHORT $L76238
	push	1514					; 000005eaH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76238
	int	3
$L76238:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76235
$L76237:

; 1515 : 	ASSERT(m_nCount == 0 || m_pHashTable != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+12], 0
	je	SHORT $L76242
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L76242
	push	1515					; 000005ebH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76242
	int	3
$L76242:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76237

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
?AssertValid@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UBEXXZ ENDP ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::AssertValid
_TEXT	ENDS
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_G?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_G?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAEPAXI@Z PROC NEAR ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAE@XZ ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::~CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L76246
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L76246:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_G?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@UAEPAXI@Z ENDP ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXIAAUTHButton@CToolbarHelper@@@Z
_TEXT	SEGMENT
_key$ = 8
_newValue$ = 12
_this$ = -4
?SetAt@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXIAAUTHButton@CToolbarHelper@@@Z PROC NEAR ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::SetAt, COMDAT

; 1188 : 	{ (*this)[key] = newValue; }

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esi, DWORD PTR _newValue$[ebp]
	mov	eax, DWORD PTR _key$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??A?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEAAUTHButton@CToolbarHelper@@I@Z ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::operator[]
	mov	edi, eax
	mov	ecx, 68					; 00000044H
	rep movsd
	pop	edi
	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SetAt@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXIAAUTHButton@CToolbarHelper@@@Z ENDP ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::SetAt
_TEXT	ENDS
EXTRN	_memset:NEAR
EXTRN	??2@YAPAXIPBDH@Z:NEAR				; operator new
;	COMDAT ?InitHashTable@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXIH@Z
_TEXT	SEGMENT
$T76800 = -4
$T76801 = -8
_nHashSize$ = 8
_bAllocNow$ = 12
_this$ = -12
?InitHashTable@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXIH@Z PROC NEAR ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::InitHashTable, COMDAT

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
$L76257:

; 1220 : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L76260
	push	1220					; 000004c4H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76260
	int	3
$L76260:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76257
$L76259:

; 1221 : 	ASSERT(nHashSize > 0);

	cmp	DWORD PTR _nHashSize$[ebp], 0
	ja	SHORT $L76264
	push	1221					; 000004c5H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76264
	int	3
$L76264:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76259

; 1222 : 
; 1223 : 	if (m_pHashTable != NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L76265

; 1225 : 		// free hash table
; 1226 : 		delete[] m_pHashTable;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR $T76800[ebp], eax
	mov	ecx, DWORD PTR $T76800[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 1227 : 		m_pHashTable = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0
$L76265:

; 1229 : 
; 1230 : 	if (bAllocNow)

	cmp	DWORD PTR _bAllocNow$[ebp], 0
	je	SHORT $L76267

; 1232 : 		m_pHashTable = new CAssoc* [nHashSize];

	push	1232					; 000004d0H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _nHashSize$[ebp]
	shl	eax, 2
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T76801[ebp], eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR $T76801[ebp]
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
$L76267:

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
?InitHashTable@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@QAEXIH@Z ENDP ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::InitHashTable
_TEXT	ENDS
PUBLIC	?ConstructElements@@YGXPAIH@Z			; ConstructElements
PUBLIC	?ConstructElements@@YGXPAUTHButton@CToolbarHelper@@H@Z ; ConstructElements
PUBLIC	?data@CPlex@@QAEPAXXZ				; CPlex::data
EXTRN	?Create@CPlex@@SGPAU1@AAPAU1@II@Z:NEAR		; CPlex::Create
;	COMDAT ?NewAssoc@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT
_this$ = -20
_newBlock$76275 = -8
_pAssoc$76277 = -16
_i$76279 = -12
_pAssoc$ = -4
?NewAssoc@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@IAEPAUCAssoc@1@XZ PROC NEAR ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::NewAssoc, COMDAT

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
	jne	SHORT $L76282

; 1281 : 		// add another block
; 1282 : 		CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CMap::CAssoc));

	push	284					; 0000011cH
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 20					; 00000014H
	push	eax
	call	?Create@CPlex@@SGPAU1@AAPAU1@II@Z	; CPlex::Create
	mov	DWORD PTR _newBlock$76275[ebp], eax

; 1283 : 		// chain them into free list
; 1284 : 		CMap::CAssoc* pAssoc = (CMap::CAssoc*) newBlock->data();

	mov	ecx, DWORD PTR _newBlock$76275[ebp]
	call	?data@CPlex@@QAEPAXXZ			; CPlex::data
	mov	DWORD PTR _pAssoc$76277[ebp], eax

; 1285 : 		// free in reverse order to make it easier to debug
; 1286 : 		pAssoc += m_nBlockSize - 1;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	imul	edx, 284				; 0000011cH
	mov	eax, DWORD PTR _pAssoc$76277[ebp]
	add	eax, edx
	mov	DWORD PTR _pAssoc$76277[ebp], eax

; 1287 : 		for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	mov	DWORD PTR _i$76279[ebp], edx
	jmp	SHORT $L76280
$L76281:
	mov	eax, DWORD PTR _i$76279[ebp]
	sub	eax, 1
	mov	DWORD PTR _i$76279[ebp], eax
	mov	ecx, DWORD PTR _pAssoc$76277[ebp]
	sub	ecx, 284				; 0000011cH
	mov	DWORD PTR _pAssoc$76277[ebp], ecx
$L76280:
	cmp	DWORD PTR _i$76279[ebp], 0
	jl	SHORT $L76282

; 1289 : 			pAssoc->pNext = m_pFreeList;

	mov	edx, DWORD PTR _pAssoc$76277[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR [edx], ecx

; 1290 : 			m_pFreeList = pAssoc;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _pAssoc$76277[ebp]
	mov	DWORD PTR [edx+16], eax

; 1291 : 		}

	jmp	SHORT $L76281
$L76282:

; 1293 : 	ASSERT(m_pFreeList != NULL);  // we must have something

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+16], 0
	jne	SHORT $L76286
	push	1293					; 0000050dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76286
	int	3
$L76286:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76282

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
$L76288:

; 1298 : 	ASSERT(m_nCount > 0);  // make sure we don't overflow

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jg	SHORT $L76291
	push	1298					; 00000512H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76291
	int	3
$L76291:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76288

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
	call	?ConstructElements@@YGXPAUTHButton@CToolbarHelper@@H@Z ; ConstructElements

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
?NewAssoc@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@IAEPAUCAssoc@1@XZ ENDP ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::NewAssoc
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
PUBLIC	?HashKey@@YGII@Z				; HashKey
PUBLIC	?CompareElements@@YGHPBI0@Z			; CompareElements
;	COMDAT ?GetAssocAt@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@IBEPAUCAssoc@1@IAAI@Z
_TEXT	SEGMENT
_key$ = 8
_nHash$ = 12
_this$ = -8
_pAssoc$ = -4
?GetAssocAt@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@IBEPAUCAssoc@1@IAAI@Z PROC NEAR ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetAssocAt, COMDAT

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
	jne	SHORT $L76311

; 1327 : 		return NULL;

	xor	eax, eax
	jmp	SHORT $L76306
$L76311:

; 1331 : 	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)

	mov	edx, DWORD PTR _nHash$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR [edx+eax*4]
	mov	DWORD PTR _pAssoc$[ebp], eax
	jmp	SHORT $L76313
$L76314:
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _pAssoc$[ebp], edx
$L76313:
	cmp	DWORD PTR _pAssoc$[ebp], 0
	je	SHORT $L76315

; 1333 : 		if (CompareElements(&pAssoc->key, &key))

	lea	eax, DWORD PTR _key$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	add	ecx, 8
	push	ecx
	call	?CompareElements@@YGHPBI0@Z		; CompareElements
	test	eax, eax
	je	SHORT $L76322

; 1334 : 			return pAssoc;

	mov	eax, DWORD PTR _pAssoc$[ebp]
	jmp	SHORT $L76306
$L76322:

; 1335 : 	}

	jmp	SHORT $L76314
$L76315:

; 1336 : 	return NULL;

	xor	eax, eax
$L76306:

; 1337 : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?GetAssocAt@?$CMap@IIUTHButton@CToolbarHelper@@AAU12@@@IBEPAUCAssoc@1@IAAI@Z ENDP ; CMap<unsigned int,unsigned int,CToolbarHelper::THButton,CToolbarHelper::THButton &>::GetAssocAt
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAUTHButton@CToolbarHelper@@H@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
?DestructElements@@YGXPAUTHButton@CToolbarHelper@@H@Z PROC NEAR ; DestructElements, COMDAT

; 67   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L76324:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76328
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	imul	eax, 272				; 00000110H
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76328
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76328
	int	3
$L76328:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76324

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L76329
$L76330:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 272				; 00000110H
	mov	DWORD PTR _pElements$[ebp], eax
$L76329:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L76331

; 73   : 		pElements->~TYPE();

	jmp	SHORT $L76330
$L76331:

; 74   : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?DestructElements@@YGXPAUTHButton@CToolbarHelper@@H@Z ENDP ; DestructElements
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
$L76333:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76337
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76337
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76337
	int	3
$L76337:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76333

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L76338
$L76339:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 4
	mov	DWORD PTR _pElements$[ebp], eax
$L76338:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L76340

; 73   : 		pElements->~TYPE();

	jmp	SHORT $L76339
$L76340:

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
$L76342:

; 92   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76346
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76346
	push	93					; 0000005dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76346
	int	3
$L76346:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76342

; 93   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 94   : 
; 95   : 	// default is bit-wise read/write
; 96   : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L76347

; 97   : 		ar.Write((void*)pElements, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Write@CArchive@@QAEXPBXI@Z		; CArchive::Write

; 98   : 	else

	jmp	SHORT $L76350
$L76347:

; 99   : 		ar.Read((void*)pElements, nCount * sizeof(TYPE));

	mov	edx, DWORD PTR _nCount$[ebp]
	shl	edx, 2
	push	edx
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Read@CArchive@@QAEIPAXI@Z		; CArchive::Read
$L76350:

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
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAUTHButton@CToolbarHelper@@H@Z
_TEXT	SEGMENT
_ar$ = 8
_pElements$ = 12
_nCount$ = 16
?SerializeElements@@YGXAAVCArchive@@PAUTHButton@CToolbarHelper@@H@Z PROC NEAR ; SerializeElements, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L76354:

; 92   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76358
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	imul	eax, 272				; 00000110H
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76358
	push	93					; 0000005dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76358
	int	3
$L76358:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76354

; 93   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 94   : 
; 95   : 	// default is bit-wise read/write
; 96   : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L76359

; 97   : 		ar.Write((void*)pElements, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	imul	eax, 272				; 00000110H
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Write@CArchive@@QAEXPBXI@Z		; CArchive::Write

; 98   : 	else

	jmp	SHORT $L76362
$L76359:

; 99   : 		ar.Read((void*)pElements, nCount * sizeof(TYPE));

	mov	edx, DWORD PTR _nCount$[ebp]
	imul	edx, 272				; 00000110H
	push	edx
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Read@CArchive@@QAEIPAXI@Z		; CArchive::Read
$L76362:

; 100  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	12					; 0000000cH
?SerializeElements@@YGXAAVCArchive@@PAUTHButton@CToolbarHelper@@H@Z ENDP ; SerializeElements
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
$L76366:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76370
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76370
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76370
	int	3
$L76370:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76366

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
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBUTHButton@CToolbarHelper@@H@Z
_TEXT	SEGMENT
_pElements$ = 12
_nCount$ = 16
?DumpElements@@YGXAAVCDumpContext@@PBUTHButton@CToolbarHelper@@H@Z PROC NEAR ; DumpElements, COMDAT

; 105  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L76372:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76376
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	imul	eax, 272				; 00000110H
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76376
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76376
	int	3
$L76376:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76372

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
?DumpElements@@YGXAAVCDumpContext@@PBUTHButton@CToolbarHelper@@H@Z ENDP ; DumpElements
_TEXT	ENDS
PUBLIC	??2@YAPAXIPAX@Z					; operator new
;	COMDAT ?ConstructElements@@YGXPAIH@Z
_TEXT	SEGMENT
$T76822 = -4
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
$L76378:

; 54   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76382
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76382
	push	55					; 00000037H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76382
	int	3
$L76382:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76378

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

	jmp	SHORT $L76385
$L76386:
	mov	edx, DWORD PTR _pElements$[ebp]
	add	edx, 4
	mov	DWORD PTR _pElements$[ebp], edx
$L76385:
	mov	eax, DWORD PTR _nCount$[ebp]
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
	test	eax, eax
	je	SHORT $L76387

; 62   : 		::new((void*)pElements) TYPE;

	mov	edx, DWORD PTR _pElements$[ebp]
	push	edx
	push	4
	call	??2@YAPAXIPAX@Z				; operator new
	add	esp, 8
	mov	DWORD PTR $T76822[ebp], eax
	jmp	SHORT $L76386
$L76387:

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
;	COMDAT ?ConstructElements@@YGXPAUTHButton@CToolbarHelper@@H@Z
_TEXT	SEGMENT
$T76827 = -4
_pElements$ = 8
_nCount$ = 12
?ConstructElements@@YGXPAUTHButton@CToolbarHelper@@H@Z PROC NEAR ; ConstructElements, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L76392:

; 54   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76396
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	imul	eax, 272				; 00000110H
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76396
	push	55					; 00000037H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76396
	int	3
$L76396:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76392

; 55   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 56   : 
; 57   : 	// first do bit-wise zero initialization
; 58   : 	memset((void*)pElements, 0, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	imul	eax, 272				; 00000110H
	push	eax
	push	0
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	_memset
	add	esp, 12					; 0000000cH

; 59   : 
; 60   : 	// then call the constructor(s)
; 61   : 	for (; nCount--; pElements++)

	jmp	SHORT $L76399
$L76400:
	mov	edx, DWORD PTR _pElements$[ebp]
	add	edx, 272				; 00000110H
	mov	DWORD PTR _pElements$[ebp], edx
$L76399:
	mov	eax, DWORD PTR _nCount$[ebp]
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
	test	eax, eax
	je	SHORT $L76401

; 62   : 		::new((void*)pElements) TYPE;

	mov	edx, DWORD PTR _pElements$[ebp]
	push	edx
	push	272					; 00000110H
	call	??2@YAPAXIPAX@Z				; operator new
	add	esp, 8
	mov	DWORD PTR $T76827[ebp], eax
	jmp	SHORT $L76400
$L76401:

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
?ConstructElements@@YGXPAUTHButton@CToolbarHelper@@H@Z ENDP ; ConstructElements
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
$L76410:

; 119  : 	ASSERT(AfxIsValidAddress(pElement1, sizeof(TYPE), FALSE));

	push	0
	push	4
	mov	eax, DWORD PTR _pElement1$[ebp]
	push	eax
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76414
	push	119					; 00000077H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76414
	int	3
$L76414:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76410
$L76412:

; 120  : 	ASSERT(AfxIsValidAddress(pElement2, sizeof(ARG_TYPE), FALSE));

	push	0
	push	4
	mov	edx, DWORD PTR _pElement2$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76419
	push	120					; 00000078H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76419
	int	3
$L76419:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76412

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
END

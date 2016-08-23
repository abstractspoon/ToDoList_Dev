	TITLE	D:\_CODE\Shared\RichEditHelper.cpp
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
;	COMDAT ??_C@_05KIFI@with?5?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_09LOPF@?5elements?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_01BJG@?6?$AA@
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
text$AFX_COL1	SEGMENT PARA USE32 PUBLIC ''
text$AFX_COL1	ENDS
;	COMDAT ?Valid@CTextDocument@@QBEHXZ
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
;	COMDAT ?HasFlag@Misc@@YAHKK@Z
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
;	COMDAT _$E268
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E269
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AtlA2WHelper@@YGPAGPAGPBDHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AtlW2AHelper@@YGPADPADPBGHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?T2OLE@@YAPAGPAG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?A2WBSTR@@YAPAGPBDH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CReBase@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCReBase@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CReBase@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CReSaveCaret@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCReSaveCaret@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CReSaveCaret@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CTextDocument@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCTextDocument@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CTextDocument@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Undo@CTextDocument@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Redo@CTextDocument@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CRePauseUndo@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCRePauseUndo@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CRePauseUndo@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateRichEdit20@CRichEditHelper@@SAHAAVCWnd@@KABUtagRECT@@PAV2@IKH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateRichEdit50@CRichEditHelper@@SAHAAVCWnd@@KABUtagRECT@@PAV2@IKH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateRichEdit@CRichEditHelper@@KAHAAVCWnd@@PBGKABUtagRECT@@PAV2@IKH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InitRichEdit@CRichEditHelper@@SAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ClearUndo@CRichEditHelper@@SAXPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PasteFiles@CRichEditHelper@@SAHPAUHWND__@@ABVCStringArray@@W4RE_PASTE@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PasteFile@CRichEditHelper@@SAHPAUHWND__@@PBGW4RE_PASTE@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PasteFileInternal@CRichEditHelper@@KAHPAUHWND__@@PBGW4RE_PASTE@@AAH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CReFileObject@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCReFileObject@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CReFileObject@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Create@CReFileObject@@IAEHPBGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetReObject@CReFileObject@@IBEHAAU_reobject@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Reset@CReFileObject@@IAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Insert@CReFileObject@@QAEHPBGH@Z
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
;	COMDAT ??1?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddTail@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEPAU__POSITION@@AAUClipboardData@CClipboardBackup@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetHeadPosition@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNext@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEAAUClipboardData@CClipboardBackup@@AAPAU__POSITION@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAUClipboardData@CClipboardBackup@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBUClipboardData@CClipboardBackup@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewNode@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@IAEPAUCNode@1@PAU21@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAUClipboardData@CClipboardBackup@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAUClipboardData@CClipboardBackup@@H@Z
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
;	COMDAT ??_7?$CList@UClipboardData@CClipboardBackup@@AAU12@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CReFileObject@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CRePauseUndo@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CTextDocument@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CReSaveCaret@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CReBase@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS, CRT$XCA, CRT$XCU, CRT$XCL, CRT$XCC, CRT$XCZ
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	_IID_ITextDocument
_BSS	SEGMENT
_?hRichEdit20@?1??InitRichEdit@CRichEditHelper@@SAHXZ@4PAUHINSTANCE__@@A DD 01H DUP (?)
_?hRichEdit50@?1??InitRichEdit@CRichEditHelper@@SAHXZ@4PAUHINSTANCE__@@A DD 01H DUP (?)
_BSS	ENDS
_DATA	SEGMENT
__szAfxTempl DB	'afxtempl.h', 00H
_DATA	ENDS
CRT$XCU	SEGMENT
_$S270	DD	FLAT:_$E269
CRT$XCU	ENDS
_DATA	SEGMENT
	ORG $+1
_THIS_FILE DB	'D:\_CODE\Shared\RichEditHelper.cpp', 00H
_DATA	ENDS
CONST	SEGMENT
_IID_ITextDocument DD 08cc497c0H
	DW	0a1dfH
	DW	011ceH
	DB	080H
	DB	098H
	DB	00H
	DB	0aaH
	DB	00H
	DB	047H
	DB	0beH
	DB	05dH
CONST	ENDS
_DATA	SEGMENT
	ORG $+1
$SG77828 DB	'H', 00H, 'T', 00H, 'M', 00H, 'L', 00H, ' ', 00H, 'F', 00H
	DB	'o', 00H, 'r', 00H, 'm', 00H, 'a', 00H, 't', 00H, 00H, 00H
_DATA	ENDS
EXTRN	__chkesp:NEAR
;	COMDAT _$E269
_TEXT	SEGMENT
_$E269	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E268
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E269	ENDP
_TEXT	ENDS
EXTRN	__imp__RegisterClipboardFormatW@4:NEAR
_BSS	SEGMENT
_CF_HTML DD	01H DUP (?)
_BSS	ENDS
;	COMDAT _$E268
_TEXT	SEGMENT
_$E268	PROC NEAR					; COMDAT

; 42   : static UINT CF_HTML = ::RegisterClipboardFormat(_T("HTML Format"));

	push	ebp
	mov	ebp, esp
	push	esi
	mov	esi, esp
	push	OFFSET FLAT:$SG77828
	call	DWORD PTR __imp__RegisterClipboardFormatW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _CF_HTML, eax
	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E268	ENDP
_TEXT	ENDS
PUBLIC	??0CReBase@@QAE@PAUHWND__@@@Z			; CReBase::CReBase
PUBLIC	??_7CReBase@@6B@				; CReBase::`vftable'
PUBLIC	??_GCReBase@@UAEPAXI@Z				; CReBase::`scalar deleting destructor'
PUBLIC	??_ECReBase@@UAEPAXI@Z				; CReBase::`vector deleting destructor'
;	COMDAT ??_7CReBase@@6B@
; File D:\_CODE\Shared\RichEditHelper.cpp
CONST	SEGMENT
??_7CReBase@@6B@ DD FLAT:??_ECReBase@@UAEPAXI@Z		; CReBase::`vftable'
CONST	ENDS
;	COMDAT ??0CReBase@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT
_hwndRichEdit$ = 8
_this$ = -4
??0CReBase@@QAE@PAUHWND__@@@Z PROC NEAR			; CReBase::CReBase, COMDAT

; 35   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _hwndRichEdit$[ebp]
	mov	DWORD PTR [eax+4], ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7CReBase@@6B@ ; CReBase::`vftable'

; 36   : }

	mov	eax, DWORD PTR _this$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	4
??0CReBase@@QAE@PAUHWND__@@@Z ENDP			; CReBase::CReBase
_TEXT	ENDS
PUBLIC	??1CReBase@@UAE@XZ				; CReBase::~CReBase
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCReBase@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCReBase@@UAEPAXI@Z PROC NEAR			; CReBase::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CReBase@@UAE@XZ			; CReBase::~CReBase
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L78215
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L78215:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCReBase@@UAEPAXI@Z ENDP				; CReBase::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CReBase@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CReBase@@UAE@XZ PROC NEAR				; CReBase::~CReBase, COMDAT

; 39   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CReBase@@6B@ ; CReBase::`vftable'

; 40   : 	m_hwndRichedit = NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+4], 0

; 41   : }

	mov	esp, ebp
	pop	ebp
	ret	0
??1CReBase@@UAE@XZ ENDP					; CReBase::~CReBase
_TEXT	ENDS
PUBLIC	??0CReSaveCaret@@QAE@PAUHWND__@@@Z		; CReSaveCaret::CReSaveCaret
PUBLIC	??_7CReSaveCaret@@6B@				; CReSaveCaret::`vftable'
PUBLIC	??_GCReSaveCaret@@UAEPAXI@Z			; CReSaveCaret::`scalar deleting destructor'
PUBLIC	??_ECReSaveCaret@@UAEPAXI@Z			; CReSaveCaret::`vector deleting destructor'
EXTRN	__imp__SendMessageW@16:NEAR
;	COMDAT ??_7CReSaveCaret@@6B@
; File D:\_CODE\Shared\RichEditHelper.cpp
CONST	SEGMENT
??_7CReSaveCaret@@6B@ DD FLAT:??_ECReSaveCaret@@UAEPAXI@Z ; CReSaveCaret::`vftable'
CONST	ENDS
;	COMDAT ??0CReSaveCaret@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT
_hwndRichEdit$ = 8
_this$ = -12
_cr$ = -8
??0CReSaveCaret@@QAE@PAUHWND__@@@Z PROC NEAR		; CReSaveCaret::CReSaveCaret, COMDAT

; 46   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _hwndRichEdit$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CReBase@@QAE@PAUHWND__@@@Z		; CReBase::CReBase
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CReSaveCaret@@6B@ ; CReSaveCaret::`vftable'

; 47   : 	CHARRANGE cr;
; 48   : 	SendMessage(m_hwndRichedit, EM_EXGETSEL, 0, (LPARAM)&cr);

	mov	esi, esp
	lea	edx, DWORD PTR _cr$[ebp]
	push	edx
	push	0
	push	1076					; 00000434H
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 49   : 
; 50   : 	// if the caret is at the end of the text then set it to LONG_MAX
; 51   : 	if (cr.cpMax == SendMessage(m_hwndRichedit, WM_GETTEXTLENGTH, 0, 0))

	mov	esi, esp
	push	0
	push	0
	push	14					; 0000000eH
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp
	cmp	DWORD PTR _cr$[ebp+4], eax
	jne	SHORT $L78225

; 52   : 		m_lCaret = LONG_MAX;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+8], 2147483647		; 7fffffffH

; 53   : 	else

	jmp	SHORT $L78226
$L78225:

; 54   : 		m_lCaret = cr.cpMax;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _cr$[ebp+4]
	mov	DWORD PTR [edx+8], eax
$L78226:

; 55   : }

	mov	eax, DWORD PTR _this$[ebp]
	pop	esi
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0CReSaveCaret@@QAE@PAUHWND__@@@Z ENDP			; CReSaveCaret::CReSaveCaret
_TEXT	ENDS
PUBLIC	??1CReSaveCaret@@UAE@XZ				; CReSaveCaret::~CReSaveCaret
;	COMDAT ??_GCReSaveCaret@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCReSaveCaret@@UAEPAXI@Z PROC NEAR			; CReSaveCaret::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CReSaveCaret@@UAE@XZ			; CReSaveCaret::~CReSaveCaret
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L78230
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L78230:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCReSaveCaret@@UAEPAXI@Z ENDP			; CReSaveCaret::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CReSaveCaret@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CReSaveCaret@@UAE@XZ PROC NEAR			; CReSaveCaret::~CReSaveCaret, COMDAT

; 58   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CReSaveCaret@@6B@ ; CReSaveCaret::`vftable'

; 59   : 	SendMessage(m_hwndRichedit, EM_SETSEL, m_lCaret, m_lCaret);

	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	push	ecx
	push	177					; 000000b1H
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 60   : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CReBase@@UAE@XZ			; CReBase::~CReBase
	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CReSaveCaret@@UAE@XZ ENDP				; CReSaveCaret::~CReSaveCaret
_TEXT	ENDS
PUBLIC	??0CTextDocument@@QAE@PAUHWND__@@@Z		; CTextDocument::CTextDocument
PUBLIC	??_7CTextDocument@@6B@				; CTextDocument::`vftable'
PUBLIC	??_GCTextDocument@@UAEPAXI@Z			; CTextDocument::`scalar deleting destructor'
PUBLIC	??_ECTextDocument@@UAEPAXI@Z			; CTextDocument::`vector deleting destructor'
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
;	COMDAT ??_7CTextDocument@@6B@
; File D:\_CODE\Shared\RichEditHelper.cpp
CONST	SEGMENT
??_7CTextDocument@@6B@ DD FLAT:??_ECTextDocument@@UAEPAXI@Z ; CTextDocument::`vftable'
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T78899	DD	019930520H
	DD	01H
	DD	FLAT:$T78902
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T78902	DD	0ffffffffH
	DD	FLAT:$L78895
xdata$x	ENDS
;	COMDAT ??0CTextDocument@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_hwndRichEdit$ = 8
_this$ = -16
??0CTextDocument@@QAE@PAUHWND__@@@Z PROC NEAR		; CTextDocument::CTextDocument, COMDAT

; 65   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L78900
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _hwndRichEdit$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CReBase@@QAE@PAUHWND__@@@Z		; CReBase::CReBase
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+8], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CTextDocument@@6B@ ; CTextDocument::`vftable'

; 66   : 	SendMessage(hwndRichEdit, EM_GETOLEINTERFACE, 0, (LPARAM)&m_pRichEditOle);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	mov	esi, esp
	push	ecx
	push	0
	push	1084					; 0000043cH
	mov	edx, DWORD PTR _hwndRichEdit$[ebp]
	push	edx
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 67   : 
; 68   : 	if (m_pRichEditOle)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	je	SHORT $L78239

; 69   : 		m_pRichEditOle->QueryInterface(IID_ITextDocument, (void**)&m_pDoc);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	mov	esi, esp
	push	ecx
	push	OFFSET FLAT:_IID_ITextDocument
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	mov	ecx, DWORD PTR [edx]
	push	eax
	call	DWORD PTR [ecx]
	cmp	esi, esp
	call	__chkesp
$L78239:

; 70   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L78895:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CReBase@@UAE@XZ			; CReBase::~CReBase
	ret	0
$L78900:
	mov	eax, OFFSET FLAT:$T78899
	jmp	___CxxFrameHandler
text$x	ENDS
??0CTextDocument@@QAE@PAUHWND__@@@Z ENDP		; CTextDocument::CTextDocument
PUBLIC	??1CTextDocument@@UAE@XZ			; CTextDocument::~CTextDocument
;	COMDAT ??_GCTextDocument@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCTextDocument@@UAEPAXI@Z PROC NEAR			; CTextDocument::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CTextDocument@@UAE@XZ		; CTextDocument::~CTextDocument
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L78244
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L78244:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCTextDocument@@UAEPAXI@Z ENDP			; CTextDocument::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT xdata$x
; File D:\_CODE\Shared\RichEditHelper.cpp
xdata$x	SEGMENT
$T78912	DD	019930520H
	DD	01H
	DD	FLAT:$T78914
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T78914	DD	0ffffffffH
	DD	FLAT:$L78910
xdata$x	ENDS
;	COMDAT ??1CTextDocument@@UAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??1CTextDocument@@UAE@XZ PROC NEAR			; CTextDocument::~CTextDocument, COMDAT

; 73   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L78913
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CTextDocument@@6B@ ; CTextDocument::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 74   : 	if (m_pDoc)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+8], 0
	je	SHORT $L78248

; 75   : 		m_pDoc->Release();

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	ecx, DWORD PTR [edx]
	mov	esi, esp
	push	eax
	call	DWORD PTR [ecx+8]
	cmp	esi, esp
	call	__chkesp
$L78248:

; 76   : 
; 77   : 	if (m_pRichEditOle)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+12], 0
	je	SHORT $L78249

; 78   : 		m_pRichEditOle->Release();

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	push	ecx
	call	DWORD PTR [edx+8]
	cmp	esi, esp
	call	__chkesp
$L78249:

; 79   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CReBase@@UAE@XZ			; CReBase::~CReBase
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
$L78910:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CReBase@@UAE@XZ			; CReBase::~CReBase
	ret	0
$L78913:
	mov	eax, OFFSET FLAT:$T78912
	jmp	___CxxFrameHandler
text$x	ENDS
??1CTextDocument@@UAE@XZ ENDP				; CTextDocument::~CTextDocument
PUBLIC	?Valid@CTextDocument@@QBEHXZ			; CTextDocument::Valid
PUBLIC	?Undo@CTextDocument@@QAEHXZ			; CTextDocument::Undo
;	COMDAT ?Undo@CTextDocument@@QAEHXZ
_TEXT	SEGMENT
_this$ = -4
?Undo@CTextDocument@@QAEHXZ PROC NEAR			; CTextDocument::Undo, COMDAT

; 82   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 83   : 	return Valid() && (m_pDoc->Undo(1, NULL) == S_OK);

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Valid@CTextDocument@@QBEHXZ		; CTextDocument::Valid
	test	eax, eax
	je	SHORT $L78919
	mov	esi, esp
	push	0
	push	1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	mov	edx, DWORD PTR [eax]
	push	ecx
	call	DWORD PTR [edx+88]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L78919
	mov	DWORD PTR -8+[ebp], 1
	jmp	SHORT $L78920
$L78919:
	mov	DWORD PTR -8+[ebp], 0
$L78920:
	mov	eax, DWORD PTR -8+[ebp]

; 84   : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Undo@CTextDocument@@QAEHXZ ENDP			; CTextDocument::Undo
_TEXT	ENDS
;	COMDAT ?Valid@CTextDocument@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?Valid@CTextDocument@@QBEHXZ PROC NEAR			; CTextDocument::Valid, COMDAT

; 47   : 	BOOL Valid() const { return (m_pDoc != NULL);	}

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	cmp	DWORD PTR [eax+8], 0
	setne	cl
	mov	eax, ecx
	mov	esp, ebp
	pop	ebp
	ret	0
?Valid@CTextDocument@@QBEHXZ ENDP			; CTextDocument::Valid
_TEXT	ENDS
PUBLIC	?Redo@CTextDocument@@QAEHXZ			; CTextDocument::Redo
;	COMDAT ?Redo@CTextDocument@@QAEHXZ
_TEXT	SEGMENT
_this$ = -4
?Redo@CTextDocument@@QAEHXZ PROC NEAR			; CTextDocument::Redo, COMDAT

; 87   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 88   : 	return Valid() && (m_pDoc->Redo(1, NULL) == S_OK);

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Valid@CTextDocument@@QBEHXZ		; CTextDocument::Valid
	test	eax, eax
	je	SHORT $L78925
	mov	esi, esp
	push	0
	push	1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	mov	edx, DWORD PTR [eax]
	push	ecx
	call	DWORD PTR [edx+92]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L78925
	mov	DWORD PTR -8+[ebp], 1
	jmp	SHORT $L78926
$L78925:
	mov	DWORD PTR -8+[ebp], 0
$L78926:
	mov	eax, DWORD PTR -8+[ebp]

; 89   : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Redo@CTextDocument@@QAEHXZ ENDP			; CTextDocument::Redo
_TEXT	ENDS
PUBLIC	??0CRePauseUndo@@QAE@PAUHWND__@@@Z		; CRePauseUndo::CRePauseUndo
PUBLIC	??_7CRePauseUndo@@6B@				; CRePauseUndo::`vftable'
PUBLIC	??_GCRePauseUndo@@UAEPAXI@Z			; CRePauseUndo::`scalar deleting destructor'
PUBLIC	??_ECRePauseUndo@@UAEPAXI@Z			; CRePauseUndo::`vector deleting destructor'
;	COMDAT ??_7CRePauseUndo@@6B@
; File D:\_CODE\Shared\RichEditHelper.cpp
CONST	SEGMENT
??_7CRePauseUndo@@6B@ DD FLAT:??_ECRePauseUndo@@UAEPAXI@Z ; CRePauseUndo::`vftable'
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T78931	DD	019930520H
	DD	01H
	DD	FLAT:$T78933
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T78933	DD	0ffffffffH
	DD	FLAT:$L78929
xdata$x	ENDS
;	COMDAT ??0CRePauseUndo@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_hwndRichEdit$ = 8
_this$ = -16
??0CRePauseUndo@@QAE@PAUHWND__@@@Z PROC NEAR		; CRePauseUndo::CRePauseUndo, COMDAT

; 94   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L78932
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _hwndRichEdit$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CTextDocument@@QAE@PAUHWND__@@@Z	; CTextDocument::CTextDocument
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CRePauseUndo@@6B@ ; CRePauseUndo::`vftable'

; 95   : 	if (m_hwndRichedit && Valid())

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	je	SHORT $L78262
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Valid@CTextDocument@@QBEHXZ		; CTextDocument::Valid
	test	eax, eax
	je	SHORT $L78262

; 96   : 		m_pDoc->Undo(tomSuspend, NULL);

	mov	esi, esp
	push	0
	push	-9999995				; ff676985H
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	mov	edx, DWORD PTR [eax]
	push	ecx
	call	DWORD PTR [edx+88]
	cmp	esi, esp
	call	__chkesp
$L78262:

; 97   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L78929:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CTextDocument@@UAE@XZ		; CTextDocument::~CTextDocument
	ret	0
$L78932:
	mov	eax, OFFSET FLAT:$T78931
	jmp	___CxxFrameHandler
text$x	ENDS
??0CRePauseUndo@@QAE@PAUHWND__@@@Z ENDP			; CRePauseUndo::CRePauseUndo
PUBLIC	??1CRePauseUndo@@UAE@XZ				; CRePauseUndo::~CRePauseUndo
;	COMDAT ??_GCRePauseUndo@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCRePauseUndo@@UAEPAXI@Z PROC NEAR			; CRePauseUndo::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CRePauseUndo@@UAE@XZ			; CRePauseUndo::~CRePauseUndo
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L78266
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L78266:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCRePauseUndo@@UAEPAXI@Z ENDP			; CRePauseUndo::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT xdata$x
; File D:\_CODE\Shared\RichEditHelper.cpp
xdata$x	SEGMENT
$T78942	DD	019930520H
	DD	01H
	DD	FLAT:$T78944
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T78944	DD	0ffffffffH
	DD	FLAT:$L78940
xdata$x	ENDS
;	COMDAT ??1CRePauseUndo@@UAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??1CRePauseUndo@@UAE@XZ PROC NEAR			; CRePauseUndo::~CRePauseUndo, COMDAT

; 100  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L78943
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CRePauseUndo@@6B@ ; CRePauseUndo::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 101  : 	if (m_hwndRichedit && Valid())

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L78270
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Valid@CTextDocument@@QBEHXZ		; CTextDocument::Valid
	test	eax, eax
	je	SHORT $L78270

; 102  : 		m_pDoc->Undo(tomResume, NULL);

	mov	esi, esp
	push	0
	push	-9999994				; ff676986H
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	ecx, DWORD PTR [edx]
	push	eax
	call	DWORD PTR [ecx+88]
	cmp	esi, esp
	call	__chkesp
$L78270:

; 103  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CTextDocument@@UAE@XZ		; CTextDocument::~CTextDocument
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
$L78940:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CTextDocument@@UAE@XZ		; CTextDocument::~CTextDocument
	ret	0
$L78943:
	mov	eax, OFFSET FLAT:$T78942
	jmp	___CxxFrameHandler
text$x	ENDS
??1CRePauseUndo@@UAE@XZ ENDP				; CRePauseUndo::~CRePauseUndo
PUBLIC	?CreateRichEdit20@CRichEditHelper@@SAHAAVCWnd@@KABUtagRECT@@PAV2@IKH@Z ; CRichEditHelper::CreateRichEdit20
PUBLIC	?CreateRichEdit@CRichEditHelper@@KAHAAVCWnd@@PBGKABUtagRECT@@PAV2@IKH@Z ; CRichEditHelper::CreateRichEdit
;	COMDAT data
; File D:\_CODE\Shared\RichEditHelper.cpp
data	SEGMENT
$SG78280 DB	'R', 00H, 'i', 00H, 'c', 00H, 'h', 00H, 'E', 00H, 'd', 00H
	DB	'i', 00H, 't', 00H, '2', 00H, '0', 00H, 'W', 00H, 00H, 00H
data	ENDS
;	COMDAT ?CreateRichEdit20@CRichEditHelper@@SAHAAVCWnd@@KABUtagRECT@@PAV2@IKH@Z
_TEXT	SEGMENT
_wnd$ = 8
_dwStyle$ = 12
_rect$ = 16
_pParentWnd$ = 20
_nID$ = 24
_dwExStyle$ = 28
_bAutoRTL$ = 32
?CreateRichEdit20@CRichEditHelper@@SAHAAVCWnd@@KABUtagRECT@@PAV2@IKH@Z PROC NEAR ; CRichEditHelper::CreateRichEdit20, COMDAT

; 108  : {

	push	ebp
	mov	ebp, esp

; 109  : 	return CreateRichEdit(wnd, WC_RICHEDIT20, dwStyle, rect, pParentWnd, nID, dwExStyle, bAutoRTL);

	mov	eax, DWORD PTR _bAutoRTL$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwExStyle$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nID$[ebp]
	push	edx
	mov	eax, DWORD PTR _pParentWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _rect$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwStyle$[ebp]
	push	edx
	push	OFFSET FLAT:$SG78280
	mov	eax, DWORD PTR _wnd$[ebp]
	push	eax
	call	?CreateRichEdit@CRichEditHelper@@KAHAAVCWnd@@PBGKABUtagRECT@@PAV2@IKH@Z ; CRichEditHelper::CreateRichEdit
	add	esp, 32					; 00000020H

; 110  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?CreateRichEdit20@CRichEditHelper@@SAHAAVCWnd@@KABUtagRECT@@PAV2@IKH@Z ENDP ; CRichEditHelper::CreateRichEdit20
_TEXT	ENDS
PUBLIC	?CreateRichEdit50@CRichEditHelper@@SAHAAVCWnd@@KABUtagRECT@@PAV2@IKH@Z ; CRichEditHelper::CreateRichEdit50
;	COMDAT data
; File D:\_CODE\Shared\RichEditHelper.cpp
data	SEGMENT
$SG78290 DB	'R', 00H, 'i', 00H, 'c', 00H, 'h', 00H, 'E', 00H, 'd', 00H
	DB	'i', 00H, 't', 00H, '5', 00H, '0', 00H, 'W', 00H, 00H, 00H
data	ENDS
;	COMDAT ?CreateRichEdit50@CRichEditHelper@@SAHAAVCWnd@@KABUtagRECT@@PAV2@IKH@Z
_TEXT	SEGMENT
_wnd$ = 8
_dwStyle$ = 12
_rect$ = 16
_pParentWnd$ = 20
_nID$ = 24
_dwExStyle$ = 28
_bAutoRTL$ = 32
?CreateRichEdit50@CRichEditHelper@@SAHAAVCWnd@@KABUtagRECT@@PAV2@IKH@Z PROC NEAR ; CRichEditHelper::CreateRichEdit50, COMDAT

; 113  : {

	push	ebp
	mov	ebp, esp

; 114  : 	return CreateRichEdit(wnd, WC_RICHEDIT50, dwStyle, rect, pParentWnd, nID, dwExStyle, bAutoRTL);

	mov	eax, DWORD PTR _bAutoRTL$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwExStyle$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nID$[ebp]
	push	edx
	mov	eax, DWORD PTR _pParentWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _rect$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwStyle$[ebp]
	push	edx
	push	OFFSET FLAT:$SG78290
	mov	eax, DWORD PTR _wnd$[ebp]
	push	eax
	call	?CreateRichEdit@CRichEditHelper@@KAHAAVCWnd@@PBGKABUtagRECT@@PAV2@IKH@Z ; CRichEditHelper::CreateRichEdit
	add	esp, 32					; 00000020H

; 115  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?CreateRichEdit50@CRichEditHelper@@SAHAAVCWnd@@KABUtagRECT@@PAV2@IKH@Z ENDP ; CRichEditHelper::CreateRichEdit50
_TEXT	ENDS
PUBLIC	?HasFlag@Misc@@YAHKK@Z				; Misc::HasFlag
PUBLIC	?InitRichEdit@CRichEditHelper@@SAHXZ		; CRichEditHelper::InitRichEdit
EXTRN	?CreateEx@CWnd@@QAEHKPBG0KABUtagRECT@@PAV1@IPAX@Z:NEAR ; CWnd::CreateEx
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	?GetExStyle@CWnd@@QBEKXZ:NEAR			; CWnd::GetExStyle
_BSS	SEGMENT
$SG78308 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT ?CreateRichEdit@CRichEditHelper@@KAHAAVCWnd@@PBGKABUtagRECT@@PAV2@IKH@Z
_TEXT	SEGMENT
_wnd$ = 8
_szClass$ = 12
_dwStyle$ = 16
_rect$ = 20
_pParentWnd$ = 24
_nID$ = 28
_dwExStyle$ = 32
_bAutoRTL$ = 36
_dwParentExStyle$78306 = -4
?CreateRichEdit@CRichEditHelper@@KAHAAVCWnd@@PBGKABUtagRECT@@PAV2@IKH@Z PROC NEAR ; CRichEditHelper::CreateRichEdit, COMDAT

; 118  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 119  : 	InitRichEdit();

	call	?InitRichEdit@CRichEditHelper@@SAHXZ	; CRichEditHelper::InitRichEdit

; 120  : 	
; 121  : 	if (bAutoRTL)

	cmp	DWORD PTR _bAutoRTL$[ebp], 0
	je	SHORT $L78307
$L78302:

; 123  : 		ASSERT(pParentWnd);

	cmp	DWORD PTR _pParentWnd$[ebp], 0
	jne	SHORT $L78305
	push	123					; 0000007bH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L78305
	int	3
$L78305:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L78302

; 124  : 		DWORD dwParentExStyle = pParentWnd->GetExStyle();

	mov	ecx, DWORD PTR _pParentWnd$[ebp]
	call	?GetExStyle@CWnd@@QBEKXZ		; CWnd::GetExStyle
	mov	DWORD PTR _dwParentExStyle$78306[ebp], eax

; 125  : 		
; 126  : 		if (Misc::HasFlag(dwParentExStyle, WS_EX_RTLREADING))

	push	8192					; 00002000H
	mov	ecx, DWORD PTR _dwParentExStyle$78306[ebp]
	push	ecx
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	test	eax, eax
	je	SHORT $L78307

; 127  : 			dwExStyle |= WS_EX_RTLREADING;

	mov	edx, DWORD PTR _dwExStyle$[ebp]
	or	dh, 32					; 00000020H
	mov	DWORD PTR _dwExStyle$[ebp], edx
$L78307:

; 129  : 
; 130  : 	return wnd.CreateEx(dwExStyle, szClass, _T(""), dwStyle, rect, pParentWnd, nID);

	push	0
	mov	eax, DWORD PTR _nID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pParentWnd$[ebp]
	push	ecx
	mov	edx, DWORD PTR _rect$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwStyle$[ebp]
	push	eax
	push	OFFSET FLAT:$SG78308
	mov	ecx, DWORD PTR _szClass$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwExStyle$[ebp]
	push	edx
	mov	ecx, DWORD PTR _wnd$[ebp]
	call	?CreateEx@CWnd@@QAEHKPBG0KABUtagRECT@@PAV1@IPAX@Z ; CWnd::CreateEx

; 131  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?CreateRichEdit@CRichEditHelper@@KAHAAVCWnd@@PBGKABUtagRECT@@PAV2@IKH@Z ENDP ; CRichEditHelper::CreateRichEdit
_TEXT	ENDS
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
EXTRN	__imp__LoadLibraryW@4:NEAR
EXTRN	?AfxInitRichEdit@@YGHXZ:NEAR			; AfxInitRichEdit
;	COMDAT data
; File D:\_CODE\Shared\RichEditHelper.cpp
data	SEGMENT
$SG78317 DB	'r', 00H, 'i', 00H, 'c', 00H, 'h', 00H, 'e', 00H, 'd', 00H
	DB	'2', 00H, '0', 00H, '.', 00H, 'd', 00H, 'l', 00H, 'l', 00H, 00H
	DB	00H
	ORG $+2
$SG78319 DB	'm', 00H, 's', 00H, 'f', 00H, 't', 00H, 'e', 00H, 'd', 00H
	DB	'i', 00H, 't', 00H, '.', 00H, 'd', 00H, 'l', 00H, 'l', 00H, 00H
	DB	00H
data	ENDS
;	COMDAT ?InitRichEdit@CRichEditHelper@@SAHXZ
_TEXT	SEGMENT
?InitRichEdit@CRichEditHelper@@SAHXZ PROC NEAR		; CRichEditHelper::InitRichEdit, COMDAT

; 134  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 135  : 	static HINSTANCE hRichEdit20 = NULL;
; 136  : 	static HINSTANCE hRichEdit50 = NULL;
; 137  : 
; 138  : 	if (!AfxInitRichEdit())

	call	?AfxInitRichEdit@@YGHXZ			; AfxInitRichEdit
	test	eax, eax
	jne	SHORT $L78315

; 139  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L78310
$L78315:

; 140  : 
; 141  : 	if (!hRichEdit20)

	cmp	DWORD PTR _?hRichEdit20@?1??InitRichEdit@CRichEditHelper@@SAHXZ@4PAUHINSTANCE__@@A, 0
	jne	SHORT $L78316

; 142  : 		hRichEdit20 = LoadLibrary(_T("riched20.dll"));

	mov	esi, esp
	push	OFFSET FLAT:$SG78317
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?hRichEdit20@?1??InitRichEdit@CRichEditHelper@@SAHXZ@4PAUHINSTANCE__@@A, eax
$L78316:

; 143  : 
; 144  : 	if (!hRichEdit50)

	cmp	DWORD PTR _?hRichEdit50@?1??InitRichEdit@CRichEditHelper@@SAHXZ@4PAUHINSTANCE__@@A, 0
	jne	SHORT $L78318

; 145  : 		hRichEdit50 = LoadLibrary(_T("msftedit.dll"));

	mov	esi, esp
	push	OFFSET FLAT:$SG78319
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?hRichEdit50@?1??InitRichEdit@CRichEditHelper@@SAHXZ@4PAUHINSTANCE__@@A, eax
$L78318:

; 146  : 
; 147  : 	return (hRichEdit20 != NULL || hRichEdit50 != NULL);

	cmp	DWORD PTR _?hRichEdit20@?1??InitRichEdit@CRichEditHelper@@SAHXZ@4PAUHINSTANCE__@@A, 0
	jne	SHORT $L78959
	cmp	DWORD PTR _?hRichEdit50@?1??InitRichEdit@CRichEditHelper@@SAHXZ@4PAUHINSTANCE__@@A, 0
	jne	SHORT $L78959
	mov	DWORD PTR -4+[ebp], 0
	jmp	SHORT $L78960
$L78959:
	mov	DWORD PTR -4+[ebp], 1
$L78960:
	mov	eax, DWORD PTR -4+[ebp]
$L78310:

; 148  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?InitRichEdit@CRichEditHelper@@SAHXZ ENDP		; CRichEditHelper::InitRichEdit
_TEXT	ENDS
PUBLIC	?ClearUndo@CRichEditHelper@@SAXPAUHWND__@@@Z	; CRichEditHelper::ClearUndo
;	COMDAT ?ClearUndo@CRichEditHelper@@SAXPAUHWND__@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
_pDoc$ = -8
_pUnk$ = -4
?ClearUndo@CRichEditHelper@@SAXPAUHWND__@@@Z PROC NEAR	; CRichEditHelper::ClearUndo, COMDAT

; 151  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L78323:

; 152  : 	ASSERT(hWnd);

	cmp	DWORD PTR _hWnd$[ebp], 0
	jne	SHORT $L78326
	push	152					; 00000098H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L78326
	int	3
$L78326:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L78323

; 153  : 
; 154  : 	ITextDocument *pDoc;
; 155  : 	IUnknown *pUnk = NULL;

	mov	DWORD PTR _pUnk$[ebp], 0

; 156  : 	
; 157  : 	SendMessage(hWnd, EM_GETOLEINTERFACE, 0, (LPARAM)&pUnk);

	mov	esi, esp
	lea	ecx, DWORD PTR _pUnk$[ebp]
	push	ecx
	push	0
	push	1084					; 0000043cH
	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 158  : 	
; 159  : 	if (pUnk)

	cmp	DWORD PTR _pUnk$[ebp], 0
	je	SHORT $L78330

; 161  : 		if (pUnk->QueryInterface(IID_ITextDocument, (void**)&pDoc) == NOERROR)

	mov	esi, esp
	lea	eax, DWORD PTR _pDoc$[ebp]
	push	eax
	push	OFFSET FLAT:_IID_ITextDocument
	mov	ecx, DWORD PTR _pUnk$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	eax, DWORD PTR _pUnk$[ebp]
	push	eax
	call	DWORD PTR [edx]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L78332

; 163  : 			pDoc->Undo(tomFalse, NULL);	// Prevents Undo and empties buffer.

	mov	esi, esp
	push	0
	push	0
	mov	ecx, DWORD PTR _pDoc$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	eax, DWORD PTR _pDoc$[ebp]
	push	eax
	call	DWORD PTR [edx+88]
	cmp	esi, esp
	call	__chkesp

; 164  : 			pDoc->Undo(tomTrue, NULL);  // Restarts Undo again

	mov	esi, esp
	push	0
	push	-1
	mov	ecx, DWORD PTR _pDoc$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	eax, DWORD PTR _pDoc$[ebp]
	push	eax
	call	DWORD PTR [edx+88]
	cmp	esi, esp
	call	__chkesp

; 165  : 			pDoc->Release();

	mov	ecx, DWORD PTR _pDoc$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	esi, esp
	mov	eax, DWORD PTR _pDoc$[ebp]
	push	eax
	call	DWORD PTR [edx+8]
	cmp	esi, esp
	call	__chkesp
$L78332:

; 167  : 		pUnk->Release();

	mov	ecx, DWORD PTR _pUnk$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	esi, esp
	mov	eax, DWORD PTR _pUnk$[ebp]
	push	eax
	call	DWORD PTR [edx+8]
	cmp	esi, esp
	call	__chkesp
$L78330:

; 169  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?ClearUndo@CRichEditHelper@@SAXPAUHWND__@@@Z ENDP	; CRichEditHelper::ClearUndo
_TEXT	ENDS
PUBLIC	?PasteFiles@CRichEditHelper@@SAHPAUHWND__@@ABVCStringArray@@W4RE_PASTE@@@Z ; CRichEditHelper::PasteFiles
PUBLIC	?PasteFileInternal@CRichEditHelper@@KAHPAUHWND__@@PBGW4RE_PASTE@@AAH@Z ; CRichEditHelper::PasteFileInternal
EXTRN	?GetSize@CStringArray@@QBEHXZ:NEAR		; CStringArray::GetSize
EXTRN	?Backup@CClipboardBackup@@QAEHXZ:NEAR		; CClipboardBackup::Backup
EXTRN	?Restore@CClipboardBackup@@QAEHXZ:NEAR		; CClipboardBackup::Restore
EXTRN	??0CClipboardBackup@@QAE@PAUHWND__@@@Z:NEAR	; CClipboardBackup::CClipboardBackup
EXTRN	??1CClipboardBackup@@QAE@XZ:NEAR		; CClipboardBackup::~CClipboardBackup
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z:NEAR ; Misc::GetItem
;	COMDAT xdata$x
; File D:\_CODE\Shared\RichEditHelper.cpp
xdata$x	SEGMENT
$T78969	DD	019930520H
	DD	01H
	DD	FLAT:$T78971
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T78971	DD	0ffffffffH
	DD	FLAT:$L78967
xdata$x	ENDS
;	COMDAT ?PasteFiles@CRichEditHelper@@SAHPAUHWND__@@ABVCStringArray@@W4RE_PASTE@@@Z
_TEXT	SEGMENT
$T78966 = -76
__$EHRec$ = -12
_hWnd$ = 8
_aFiles$ = 12
_nPasteHow$ = 16
_cbb$ = -48
_bRestoreClipboard$ = -56
_nFileCount$ = -52
_nFilesPasted$ = -64
_i$ = -60
_sFile$78347 = -72
_bUsedClipboard$78348 = -68
?PasteFiles@CRichEditHelper@@SAHPAUHWND__@@ABVCStringArray@@W4RE_PASTE@@@Z PROC NEAR ; CRichEditHelper::PasteFiles, COMDAT

; 172  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L78970
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 64					; 00000040H
	push	edi
	lea	edi, DWORD PTR [ebp-76]
	mov	ecx, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 173  : 	if (aFiles.GetSize() == 0)

	mov	ecx, DWORD PTR _aFiles$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	test	eax, eax
	jne	SHORT $L78338

; 174  : 		return FALSE;

	xor	eax, eax
	jmp	$L78337
$L78338:

; 175  : 
; 176  : 	// save backup because we may be overwriting it
; 177  :  	CClipboardBackup cbb(hWnd);

	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	lea	ecx, DWORD PTR _cbb$[ebp]
	call	??0CClipboardBackup@@QAE@PAUHWND__@@@Z	; CClipboardBackup::CClipboardBackup
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 178  :  	cbb.Backup();

	lea	ecx, DWORD PTR _cbb$[ebp]
	call	?Backup@CClipboardBackup@@QAEHXZ	; CClipboardBackup::Backup

; 179  : 
; 180  : 	BOOL bRestoreClipboard = FALSE;

	mov	DWORD PTR _bRestoreClipboard$[ebp], 0

; 181  : 
; 182  : 	UINT nFileCount = aFiles.GetSize(), nFilesPasted = 0;

	mov	ecx, DWORD PTR _aFiles$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nFileCount$[ebp], eax
	mov	DWORD PTR _nFilesPasted$[ebp], 0

; 183  : 	
; 184  : 	for (UINT i=0; i < nFileCount; i++)

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L78344
$L78345:
	mov	ecx, DWORD PTR _i$[ebp]
	add	ecx, 1
	mov	DWORD PTR _i$[ebp], ecx
$L78344:
	mov	edx, DWORD PTR _i$[ebp]
	cmp	edx, DWORD PTR _nFileCount$[ebp]
	jae	SHORT $L78346

; 186  : 		const CString& sFile = Misc::GetItem(aFiles, i);

	mov	eax, DWORD PTR _i$[ebp]
	push	eax
	mov	ecx, DWORD PTR _aFiles$[ebp]
	push	ecx
	call	?GetItem@Misc@@YAABVCString@@ABVCStringArray@@H@Z ; Misc::GetItem
	add	esp, 8
	mov	DWORD PTR _sFile$78347[ebp], eax

; 187  : 		BOOL bUsedClipboard = FALSE;

	mov	DWORD PTR _bUsedClipboard$78348[ebp], 0

; 188  : 
; 189  : 		if (PasteFileInternal(hWnd, sFile, nPasteHow, bUsedClipboard))

	lea	edx, DWORD PTR _bUsedClipboard$78348[ebp]
	push	edx
	mov	eax, DWORD PTR _nPasteHow$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sFile$78347[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	call	?PasteFileInternal@CRichEditHelper@@KAHPAUHWND__@@PBGW4RE_PASTE@@AAH@Z ; CRichEditHelper::PasteFileInternal
	add	esp, 16					; 00000010H
	test	eax, eax
	je	SHORT $L78349

; 191  : 			bRestoreClipboard |= bUsedClipboard;

	mov	edx, DWORD PTR _bRestoreClipboard$[ebp]
	or	edx, DWORD PTR _bUsedClipboard$78348[ebp]
	mov	DWORD PTR _bRestoreClipboard$[ebp], edx

; 192  : 			nFilesPasted++;

	mov	eax, DWORD PTR _nFilesPasted$[ebp]
	add	eax, 1
	mov	DWORD PTR _nFilesPasted$[ebp], eax
$L78349:

; 194  : 	}

	jmp	SHORT $L78345
$L78346:

; 195  : 
; 196  : 	if (bRestoreClipboard)

	cmp	DWORD PTR _bRestoreClipboard$[ebp], 0
	je	SHORT $L78350

; 197  : 		cbb.Restore();

	lea	ecx, DWORD PTR _cbb$[ebp]
	call	?Restore@CClipboardBackup@@QAEHXZ	; CClipboardBackup::Restore
$L78350:

; 198  : 
; 199  : 	return (nFileCount == nFilesPasted);

	mov	ecx, DWORD PTR _nFileCount$[ebp]
	xor	edx, edx
	cmp	ecx, DWORD PTR _nFilesPasted$[ebp]
	sete	dl
	mov	DWORD PTR $T78966[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _cbb$[ebp]
	call	??1CClipboardBackup@@QAE@XZ		; CClipboardBackup::~CClipboardBackup
	mov	eax, DWORD PTR $T78966[ebp]
$L78337:

; 200  : }

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
$L78967:
	lea	ecx, DWORD PTR _cbb$[ebp]
	call	??1CClipboardBackup@@QAE@XZ		; CClipboardBackup::~CClipboardBackup
	ret	0
$L78970:
	mov	eax, OFFSET FLAT:$T78969
	jmp	___CxxFrameHandler
text$x	ENDS
?PasteFiles@CRichEditHelper@@SAHPAUHWND__@@ABVCStringArray@@W4RE_PASTE@@@Z ENDP ; CRichEditHelper::PasteFiles
PUBLIC	?PasteFile@CRichEditHelper@@SAHPAUHWND__@@PBGW4RE_PASTE@@@Z ; CRichEditHelper::PasteFile
EXTRN	??0CStringArray@@QAE@XZ:NEAR			; CStringArray::CStringArray
EXTRN	?Add@CStringArray@@QAEHPBG@Z:NEAR		; CStringArray::Add
EXTRN	??1CStringArray@@UAE@XZ:NEAR			; CStringArray::~CStringArray
;	COMDAT xdata$x
; File D:\_CODE\Shared\RichEditHelper.cpp
xdata$x	SEGMENT
$T78979	DD	019930520H
	DD	01H
	DD	FLAT:$T78981
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T78981	DD	0ffffffffH
	DD	FLAT:$L78977
xdata$x	ENDS
;	COMDAT ?PasteFile@CRichEditHelper@@SAHPAUHWND__@@PBGW4RE_PASTE@@@Z
_TEXT	SEGMENT
$T78976 = -36
__$EHRec$ = -12
_hWnd$ = 8
_szFilePath$ = 12
_nPasteHow$ = 16
_aFiles$ = -32
?PasteFile@CRichEditHelper@@SAHPAUHWND__@@PBGW4RE_PASTE@@@Z PROC NEAR ; CRichEditHelper::PasteFile, COMDAT

; 203  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L78980
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

; 204  : 	CStringArray aFiles;

	lea	ecx, DWORD PTR _aFiles$[ebp]
	call	??0CStringArray@@QAE@XZ			; CStringArray::CStringArray
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 205  : 	aFiles.Add(szFilePath);

	mov	eax, DWORD PTR _szFilePath$[ebp]
	push	eax
	lea	ecx, DWORD PTR _aFiles$[ebp]
	call	?Add@CStringArray@@QAEHPBG@Z		; CStringArray::Add

; 206  : 
; 207  : 	return PasteFiles(hWnd, aFiles, nPasteHow);

	mov	ecx, DWORD PTR _nPasteHow$[ebp]
	push	ecx
	lea	edx, DWORD PTR _aFiles$[ebp]
	push	edx
	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	call	?PasteFiles@CRichEditHelper@@SAHPAUHWND__@@ABVCStringArray@@W4RE_PASTE@@@Z ; CRichEditHelper::PasteFiles
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T78976[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _aFiles$[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	mov	eax, DWORD PTR $T78976[ebp]

; 208  : }

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
$L78977:
	lea	ecx, DWORD PTR _aFiles$[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	ret	0
$L78980:
	mov	eax, OFFSET FLAT:$T78979
	jmp	___CxxFrameHandler
text$x	ENDS
?PasteFile@CRichEditHelper@@SAHPAUHWND__@@PBGW4RE_PASTE@@@Z ENDP ; CRichEditHelper::PasteFile
PUBLIC	??0CReFileObject@@QAE@PAUHWND__@@@Z		; CReFileObject::CReFileObject
PUBLIC	??1CReFileObject@@UAE@XZ			; CReFileObject::~CReFileObject
PUBLIC	?Insert@CReFileObject@@QAEHPBGH@Z		; CReFileObject::Insert
EXTRN	?FolderExists@FileMisc@@YAHPBG@Z:NEAR		; FileMisc::FolderExists
EXTRN	?PathExists@FileMisc@@YAHPBG@Z:NEAR		; FileMisc::PathExists
EXTRN	?HasExtension@FileMisc@@YAHPBG0@Z:NEAR		; FileMisc::HasExtension
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
EXTRN	??YCString@@QAEABV0@PBG@Z:NEAR			; CString::operator+=
EXTRN	??H@YG?AVCString@@DABV0@@Z:NEAR			; operator+
EXTRN	??H@YG?AVCString@@ABV0@PBG@Z:NEAR		; operator+
EXTRN	??H@YG?AVCString@@PBGABV0@@Z:NEAR		; operator+
EXTRN	?Replace@CString@@QAEHGG@Z:NEAR			; CString::Replace
EXTRN	?Find@CString@@QBEHG@Z:NEAR			; CString::Find
EXTRN	?CopyImageFileToClipboard@CEnBitmap@@SAHPAUHWND__@@PBGK@Z:NEAR ; CEnBitmap::CopyImageFileToClipboard
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
EXTRN	?GetFileType@CEnBitmap@@SA?AW4EB_IMAGETYPE@@PBG@Z:NEAR ; CEnBitmap::GetFileType
;	COMDAT data
; File D:\_CODE\Shared\RichEditHelper.cpp
data	SEGMENT
$SG78377 DB	'e', 00H, 'x', 00H, 'e', 00H, 00H, 00H
$SG78381 DB	'f', 00H, 'i', 00H, 'l', 00H, 'e', 00H, ':', 00H, '/', 00H
	DB	'/', 00H, 00H, 00H
$SG78385 DB	'>', 00H, 00H, 00H
$SG78386 DB	' ', 00H, '<', 00H, 00H, 00H
	ORG $+2
$SG78389 DB	0dH, 00H, 0aH, 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T78999	DD	019930520H
	DD	06H
	DD	FLAT:$T79002
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T79002	DD	0ffffffffH
	DD	FLAT:$L78992
	DD	0ffffffffH
	DD	FLAT:$L78993
	DD	01H
	DD	FLAT:$L78994
	DD	01H
	DD	FLAT:$L78995
	DD	03H
	DD	FLAT:$L78996
	DD	01H
	DD	FLAT:$L78997
xdata$x	ENDS
;	COMDAT ?PasteFileInternal@CRichEditHelper@@KAHPAUHWND__@@PBGW4RE_PASTE@@AAH@Z
_TEXT	SEGMENT
$T78986 = -20
$T78987 = -44
$T78988 = -48
$T78989 = -52
$T78990 = -56
$T78991 = -60
__$EHRec$ = -12
_hWnd$ = 8
_szFilePath$ = 12
_nPasteHow$ = 16
_bUsedClipboard$ = 20
_sLink$78379 = -16
?PasteFileInternal@CRichEditHelper@@KAHPAUHWND__@@PBGW4RE_PASTE@@AAH@Z PROC NEAR ; CRichEditHelper::PasteFileInternal, COMDAT

; 211  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L79000
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 92					; 0000005cH
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-104]
	mov	ecx, 23					; 00000017H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 212  : 	bUsedClipboard = FALSE;

	mov	eax, DWORD PTR _bUsedClipboard$[ebp]
	mov	DWORD PTR [eax], 0

; 213  : 
; 214  : 	if (FileMisc::PathExists(szFilePath)) // can be file or folder

	mov	ecx, DWORD PTR _szFilePath$[ebp]
	push	ecx
	call	?PathExists@FileMisc@@YAHPBG@Z		; FileMisc::PathExists
	add	esp, 4
	test	eax, eax
	je	$L78367

; 217  : 		{

	mov	edx, DWORD PTR _nPasteHow$[ebp]
	mov	DWORD PTR -64+[ebp], edx
	cmp	DWORD PTR -64+[ebp], 3
	ja	$L78392
	mov	eax, DWORD PTR -64+[ebp]
	jmp	DWORD PTR $L79001[eax*4]
$L78370:

; 218  : 		case REP_ASIMAGE:
; 219  : 			if (CEnBitmap::GetFileType(szFilePath) != FT_UNKNOWN)

	mov	ecx, DWORD PTR _szFilePath$[ebp]
	push	ecx
	call	?GetFileType@CEnBitmap@@SA?AW4EB_IMAGETYPE@@PBG@Z ; CEnBitmap::GetFileType
	add	esp, 4
	cmp	eax, 5
	je	SHORT $L78372

; 221  : 				if (CEnBitmap::CopyImageFileToClipboard(hWnd, szFilePath))

	push	-1
	mov	edx, DWORD PTR _szFilePath$[ebp]
	push	edx
	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	call	?CopyImageFileToClipboard@CEnBitmap@@SAHPAUHWND__@@PBGK@Z ; CEnBitmap::CopyImageFileToClipboard
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	SHORT $L78372

; 223  : 					::SendMessage(hWnd, EM_PASTESPECIAL, CF_BITMAP, NULL);

	mov	esi, esp
	push	0
	push	2
	push	1088					; 00000440H
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 224  : 
; 225  : 					bUsedClipboard = TRUE;

	mov	edx, DWORD PTR _bUsedClipboard$[ebp]
	mov	DWORD PTR [edx], 1

; 226  : 					return TRUE;

	mov	eax, 1
	jmp	$L78364
$L78372:

; 236  : 				!FileMisc::HasExtension(szFilePath, _T("exe")) && 
; 237  : 				CReFileObject(hWnd).Insert(szFilePath, (nPasteHow == REP_ASICONLINK)))

	mov	eax, DWORD PTR _szFilePath$[ebp]
	push	eax
	call	?FolderExists@FileMisc@@YAHPBG@Z	; FileMisc::FolderExists
	add	esp, 4
	test	eax, eax
	jne	SHORT $L78376
	push	OFFSET FLAT:$SG78377
	mov	ecx, DWORD PTR _szFilePath$[ebp]
	push	ecx
	call	?HasExtension@FileMisc@@YAHPBG0@Z	; FileMisc::HasExtension
	add	esp, 8
	test	eax, eax
	jne	SHORT $L78376
	xor	edx, edx
	cmp	DWORD PTR _nPasteHow$[ebp], 2
	sete	dl
	push	edx
	mov	eax, DWORD PTR _szFilePath$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	lea	ecx, DWORD PTR $T78987[ebp]
	call	??0CReFileObject@@QAE@PAUHWND__@@@Z	; CReFileObject::CReFileObject
	mov	DWORD PTR -68+[ebp], eax
	mov	edx, DWORD PTR -68+[ebp]
	mov	DWORD PTR -72+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -72+[ebp]
	call	?Insert@CReFileObject@@QAEHPBGH@Z	; CReFileObject::Insert
	mov	DWORD PTR $T78986[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T78987[ebp]
	call	??1CReFileObject@@UAE@XZ		; CReFileObject::~CReFileObject
	cmp	DWORD PTR $T78986[ebp], 0
	je	SHORT $L78376

; 239  : 				return TRUE;

	mov	eax, 1
	jmp	$L78364
$L78376:

; 245  : 				CString sLink(szFilePath);

	mov	eax, DWORD PTR _szFilePath$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sLink$78379[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 246  : 				sLink = _T("file://") + sLink;

	lea	ecx, DWORD PTR _sLink$78379[ebp]
	push	ecx
	push	OFFSET FLAT:$SG78381
	lea	edx, DWORD PTR $T78988[ebp]
	push	edx
	call	??H@YG?AVCString@@PBGABV0@@Z		; operator+
	mov	DWORD PTR -76+[ebp], eax
	mov	eax, DWORD PTR -76+[ebp]
	mov	DWORD PTR -80+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR -80+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sLink$78379[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T78988[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 247  : 				sLink.Replace('\\', '/');

	push	47					; 0000002fH
	push	92					; 0000005cH
	lea	ecx, DWORD PTR _sLink$78379[ebp]
	call	?Replace@CString@@QAEHGG@Z		; CString::Replace

; 248  : 	
; 249  : 				// if the path contains spaces then brace it
; 250  : 				if (sLink.Find(' ') != -1)

	push	32					; 00000020H
	lea	ecx, DWORD PTR _sLink$78379[ebp]
	call	?Find@CString@@QBEHG@Z			; CString::Find
	cmp	eax, -1
	je	SHORT $L78382

; 252  : 					sLink = _T(" <") + sLink + _T(">");

	push	OFFSET FLAT:$SG78385
	lea	edx, DWORD PTR _sLink$78379[ebp]
	push	edx
	push	OFFSET FLAT:$SG78386
	lea	eax, DWORD PTR $T78989[ebp]
	push	eax
	call	??H@YG?AVCString@@PBGABV0@@Z		; operator+
	mov	DWORD PTR -84+[ebp], eax
	mov	ecx, DWORD PTR -84+[ebp]
	mov	DWORD PTR -88+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	edx, DWORD PTR -88+[ebp]
	push	edx
	lea	eax, DWORD PTR $T78990[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -92+[ebp], eax
	mov	ecx, DWORD PTR -92+[ebp]
	mov	DWORD PTR -96+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	edx, DWORD PTR -96+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sLink$78379[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR $T78990[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T78989[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 254  : 				else // add leading space

	jmp	SHORT $L78387
$L78382:

; 256  : 					sLink = ' ' + sLink;

	lea	eax, DWORD PTR _sLink$78379[ebp]
	push	eax
	push	32					; 00000020H
	lea	ecx, DWORD PTR $T78991[ebp]
	push	ecx
	call	??H@YG?AVCString@@DABV0@@Z		; operator+
	mov	DWORD PTR -100+[ebp], eax
	mov	edx, DWORD PTR -100+[ebp]
	mov	DWORD PTR -104+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	mov	eax, DWORD PTR -104+[ebp]
	push	eax
	lea	ecx, DWORD PTR _sLink$78379[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T78991[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L78387:

; 258  : 
; 259  : 				// newline
; 260  : 				sLink += _T("\r\n");

	push	OFFSET FLAT:$SG78389
	lea	ecx, DWORD PTR _sLink$78379[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=

; 261  : 	
; 262  : 				::SendMessage(hWnd, EM_REPLACESEL, TRUE, (LPARAM)(LPCTSTR)sLink);

	lea	ecx, DWORD PTR _sLink$78379[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	push	1
	push	194					; 000000c2H
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 263  : 			}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sLink$78379[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L78392:
$L78367:

; 270  : 
; 271  : 	// all else
; 272  :     return FALSE;

	xor	eax, eax
$L78364:

; 273  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 104				; 00000068H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
$L79001:
	DD	$L78376
	DD	$L78372
	DD	$L78372
	DD	$L78370
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L78992:
	lea	ecx, DWORD PTR $T78987[ebp]
	call	??1CReFileObject@@UAE@XZ		; CReFileObject::~CReFileObject
	ret	0
$L78993:
	lea	ecx, DWORD PTR _sLink$78379[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L78994:
	lea	ecx, DWORD PTR $T78988[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L78995:
	lea	ecx, DWORD PTR $T78989[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L78996:
	lea	ecx, DWORD PTR $T78990[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L78997:
	lea	ecx, DWORD PTR $T78991[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L79000:
	mov	eax, OFFSET FLAT:$T78999
	jmp	___CxxFrameHandler
text$x	ENDS
?PasteFileInternal@CRichEditHelper@@KAHPAUHWND__@@PBGW4RE_PASTE@@AAH@Z ENDP ; CRichEditHelper::PasteFileInternal
PUBLIC	??_7CReFileObject@@6B@				; CReFileObject::`vftable'
PUBLIC	??_GCReFileObject@@UAEPAXI@Z			; CReFileObject::`scalar deleting destructor'
PUBLIC	??_ECReFileObject@@UAEPAXI@Z			; CReFileObject::`vector deleting destructor'
;	COMDAT ??_7CReFileObject@@6B@
; File D:\_CODE\Shared\RichEditHelper.cpp
CONST	SEGMENT
??_7CReFileObject@@6B@ DD FLAT:??_ECReFileObject@@UAEPAXI@Z ; CReFileObject::`vftable'
CONST	ENDS
;	COMDAT ??0CReFileObject@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT
_hwndRichEdit$ = 8
_this$ = -4
??0CReFileObject@@QAE@PAUHWND__@@@Z PROC NEAR		; CReFileObject::CReFileObject, COMDAT

; 280  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _hwndRichEdit$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CReBase@@QAE@PAUHWND__@@@Z		; CReBase::CReBase
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+8], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+16], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+20], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7CReFileObject@@6B@ ; CReFileObject::`vftable'

; 281  : 	SendMessage(m_hwndRichedit, EM_GETOLEINTERFACE, 0, (LPARAM)&m_pRichEditOle);

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 8
	mov	esi, esp
	push	eax
	push	0
	push	1084					; 0000043cH
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 282  : }

	mov	eax, DWORD PTR _this$[ebp]
	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0CReFileObject@@QAE@PAUHWND__@@@Z ENDP		; CReFileObject::CReFileObject
_TEXT	ENDS
;	COMDAT ??_GCReFileObject@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCReFileObject@@UAEPAXI@Z PROC NEAR			; CReFileObject::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CReFileObject@@UAE@XZ		; CReFileObject::~CReFileObject
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L78401
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L78401:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCReFileObject@@UAEPAXI@Z ENDP			; CReFileObject::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	?Reset@CReFileObject@@IAEXXZ			; CReFileObject::Reset
;	COMDAT xdata$x
; File D:\_CODE\Shared\RichEditHelper.cpp
xdata$x	SEGMENT
$T79015	DD	019930520H
	DD	01H
	DD	FLAT:$T79017
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T79017	DD	0ffffffffH
	DD	FLAT:$L79013
xdata$x	ENDS
;	COMDAT ??1CReFileObject@@UAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??1CReFileObject@@UAE@XZ PROC NEAR			; CReFileObject::~CReFileObject, COMDAT

; 285  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L79016
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CReFileObject@@6B@ ; CReFileObject::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 286  :    Reset();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Reset@CReFileObject@@IAEXXZ		; CReFileObject::Reset

; 287  :    RELEASE_INTERFACE(m_pRichEditOle);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+8], 0
	je	SHORT $L78405
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	ecx, DWORD PTR [edx]
	mov	esi, esp
	push	eax
	call	DWORD PTR [ecx+8]
	cmp	esi, esp
	call	__chkesp
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+8], 0
$L78405:

; 288  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CReBase@@UAE@XZ			; CReBase::~CReBase
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
$L79013:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CReBase@@UAE@XZ			; CReBase::~CReBase
	ret	0
$L79016:
	mov	eax, OFFSET FLAT:$T79015
	jmp	___CxxFrameHandler
text$x	ENDS
??1CReFileObject@@UAE@XZ ENDP				; CReFileObject::~CReFileObject
PUBLIC	?T2OLE@@YAPAGPAG@Z				; T2OLE
PUBLIC	?Create@CReFileObject@@IAEHPBGH@Z		; CReFileObject::Create
EXTRN	__imp__CreateILockBytesOnHGlobal@12:NEAR
EXTRN	_GUID_NULL:BYTE
EXTRN	_IID_IOleObject:BYTE
EXTRN	?FileExists@FileMisc@@YAHPBG@Z:NEAR		; FileMisc::FileExists
EXTRN	_IID_IUnknown:BYTE
EXTRN	__imp__StgCreateDocfileOnILockBytes@16:NEAR
EXTRN	__imp__OleCreateLinkToFile@28:NEAR
EXTRN	__imp__OleCreateFromFile@32:NEAR
EXTRN	__imp__OleSetContainedObject@8:NEAR
;	COMDAT ?Create@CReFileObject@@IAEHPBGH@Z
_TEXT	SEGMENT
_szFilePath$ = 8
_bLink$ = 12
_this$ = -68
__convert$ = -60
__acp$ = -8
__lpw$ = -4
__lpa$ = -56
_pLockBytes$ = -16
_clsid$ = -52
_sc$ = -12
_formatEtc$ = -36
_pUnk$78436 = -64
?Create@CReFileObject@@IAEHPBGH@Z PROC NEAR		; CReFileObject::Create, COMDAT

; 291  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 68					; 00000044H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-68]
	mov	ecx, 17					; 00000011H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 292  :    if (!m_pRichEditOle)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	jne	SHORT $L78411

; 293  :       return FALSE;

	xor	eax, eax
	jmp	$L78410
$L78411:

; 294  :    
; 295  :    if (!FileMisc::FileExists(szFilePath))

	mov	ecx, DWORD PTR _szFilePath$[ebp]
	push	ecx
	call	?FileExists@FileMisc@@YAHPBG@Z		; FileMisc::FileExists
	add	esp, 4
	test	eax, eax
	jne	SHORT $L78412

; 296  :       return FALSE;

	xor	eax, eax
	jmp	$L78410
$L78412:

; 297  : 
; 298  :    Reset();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Reset@CReFileObject@@IAEXXZ		; CReFileObject::Reset

; 299  : 
; 300  :    USES_CONVERSION;

	mov	DWORD PTR __convert$[ebp], 0
	mov	DWORD PTR __acp$[ebp], 0
	mov	DWORD PTR __lpw$[ebp], 0
	mov	DWORD PTR __lpa$[ebp], 0

; 301  : 
; 302  :    LPLOCKBYTES pLockBytes = NULL;

	mov	DWORD PTR _pLockBytes$[ebp], 0

; 303  :    CLSID clsid = CLSID_NULL;

	mov	edx, DWORD PTR _GUID_NULL
	mov	DWORD PTR _clsid$[ebp], edx
	mov	eax, DWORD PTR _GUID_NULL+4
	mov	DWORD PTR _clsid$[ebp+4], eax
	mov	ecx, DWORD PTR _GUID_NULL+8
	mov	DWORD PTR _clsid$[ebp+8], ecx
	mov	edx, DWORD PTR _GUID_NULL+12
	mov	DWORD PTR _clsid$[ebp+12], edx

; 304  : 
; 305  :    SCODE sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &pLockBytes);

	mov	esi, esp
	lea	eax, DWORD PTR _pLockBytes$[ebp]
	push	eax
	push	1
	push	0
	call	DWORD PTR __imp__CreateILockBytesOnHGlobal@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _sc$[ebp], eax
$L78420:

; 306  :    ASSERT (pLockBytes);

	cmp	DWORD PTR _pLockBytes$[ebp], 0
	jne	SHORT $L78423
	push	306					; 00000132H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L78423
	int	3
$L78423:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L78420

; 307  : 
; 308  :    if (sc != S_OK)

	cmp	DWORD PTR _sc$[ebp], 0
	je	SHORT $L78425

; 309  :       return FALSE;

	xor	eax, eax
	jmp	$L78410
$L78425:

; 312  :             STGM_SHARE_EXCLUSIVE |
; 313  :             STGM_CREATE |
; 314  :             STGM_READWRITE,
; 315  :             0, &m_pStorage);

	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 16					; 00000010H
	mov	esi, esp
	push	edx
	push	0
	push	4114					; 00001012H
	mov	eax, DWORD PTR _pLockBytes$[ebp]
	push	eax
	call	DWORD PTR __imp__StgCreateDocfileOnILockBytes@16
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _sc$[ebp], eax

; 316  : 
; 317  :    if (sc != S_OK)

	cmp	DWORD PTR _sc$[ebp], 0
	je	SHORT $L78427

; 319  :       pLockBytes->Release();

	mov	ecx, DWORD PTR _pLockBytes$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	esi, esp
	mov	eax, DWORD PTR _pLockBytes$[ebp]
	push	eax
	call	DWORD PTR [edx+8]
	cmp	esi, esp
	call	__chkesp

; 320  :       return FALSE;

	xor	eax, eax
	jmp	$L78410
$L78427:

; 324  : 
; 325  :    formatEtc.cfFormat = 0;

	mov	WORD PTR _formatEtc$[ebp], 0

; 326  :    formatEtc.ptd = NULL;

	mov	DWORD PTR _formatEtc$[ebp+4], 0

; 327  :    formatEtc.dwAspect = DVASPECT_ICON;

	mov	DWORD PTR _formatEtc$[ebp+8], 4

; 328  :    formatEtc.lindex = -1;

	mov	DWORD PTR _formatEtc$[ebp+12], -1

; 329  :    formatEtc.tymed = TYMED_NULL;

	mov	DWORD PTR _formatEtc$[ebp+16], 0

; 330  : 
; 331  :    // create object
; 332  :    m_pRichEditOle->GetClientSite(&m_pClientSite);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 20					; 00000014H
	mov	esi, esp
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	ecx, DWORD PTR [edx]
	push	eax
	call	DWORD PTR [ecx+12]
	cmp	esi, esp
	call	__chkesp

; 333  : 
; 334  :    if (bLink)

	cmp	DWORD PTR _bLink$[ebp], 0
	je	SHORT $L78429

; 336  : 	   sc = ::OleCreateLinkToFile(T2OLE((LPTSTR)szFilePath),
; 337  : 							   IID_IUnknown,
; 338  : 							   OLERENDER_DRAW,
; 339  : 							   &formatEtc,
; 340  : 							   m_pClientSite,
; 341  : 							   m_pStorage,
; 342  : 							   (void**)&m_pObject);

	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 12					; 0000000cH
	mov	esi, esp
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+20]
	push	eax
	lea	ecx, DWORD PTR _formatEtc$[ebp]
	push	ecx
	push	1
	push	OFFSET FLAT:_IID_IUnknown
	mov	edx, DWORD PTR _szFilePath$[ebp]
	push	edx
	call	?T2OLE@@YAPAGPAG@Z			; T2OLE
	add	esp, 4
	push	eax
	call	DWORD PTR __imp__OleCreateLinkToFile@28
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _sc$[ebp], eax

; 344  :    else

	jmp	SHORT $L78432
$L78429:

; 346  : 	   sc = ::OleCreateFromFile(clsid, T2OLE((LPTSTR)szFilePath),
; 347  : 							   IID_IUnknown,
; 348  : 							   OLERENDER_DRAW,
; 349  : 							   &formatEtc,
; 350  : 							   m_pClientSite,
; 351  : 							   m_pStorage,
; 352  : 							   (void**)&m_pObject);

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 12					; 0000000cH
	mov	esi, esp
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+16]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+20]
	push	ecx
	lea	edx, DWORD PTR _formatEtc$[ebp]
	push	edx
	push	1
	push	OFFSET FLAT:_IID_IUnknown
	mov	eax, DWORD PTR _szFilePath$[ebp]
	push	eax
	call	?T2OLE@@YAPAGPAG@Z			; T2OLE
	add	esp, 4
	push	eax
	lea	ecx, DWORD PTR _clsid$[ebp]
	push	ecx
	call	DWORD PTR __imp__OleCreateFromFile@32
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _sc$[ebp], eax
$L78432:

; 354  : 
; 355  :    if (m_pObject)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+12], 0
	je	SHORT $L78435

; 357  :       // convert to IOleObject
; 358  :       LPUNKNOWN pUnk = m_pObject;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR _pUnk$78436[ebp], ecx

; 359  : 
; 360  :       pUnk->QueryInterface(IID_IOleObject, (void**)&m_pObject);

	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 12					; 0000000cH
	mov	esi, esp
	push	edx
	push	OFFSET FLAT:_IID_IOleObject
	mov	eax, DWORD PTR _pUnk$78436[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR _pUnk$78436[ebp]
	push	edx
	call	DWORD PTR [ecx]
	cmp	esi, esp
	call	__chkesp

; 361  :       pUnk->Release();

	mov	eax, DWORD PTR _pUnk$78436[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	esi, esp
	mov	edx, DWORD PTR _pUnk$78436[ebp]
	push	edx
	call	DWORD PTR [ecx+8]
	cmp	esi, esp
	call	__chkesp
$L78435:

; 363  : 
; 364  :    if (!m_pObject)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jne	SHORT $L78438

; 365  :       Reset();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Reset@CReFileObject@@IAEXXZ		; CReFileObject::Reset

; 366  :    else

	jmp	SHORT $L78439
$L78438:

; 367  : 	   OleSetContainedObject(m_pObject, TRUE);

	mov	esi, esp
	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	push	edx
	call	DWORD PTR __imp__OleSetContainedObject@8
	cmp	esi, esp
	call	__chkesp
$L78439:

; 368  : 
; 369  :    return (m_pObject != NULL);

	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	cmp	DWORD PTR [eax+12], 0
	setne	cl
	mov	eax, ecx
$L78410:

; 370  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 68					; 00000044H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Create@CReFileObject@@IAEHPBGH@Z ENDP			; CReFileObject::Create
_TEXT	ENDS
;	COMDAT ?T2OLE@@YAPAGPAG@Z
_TEXT	SEGMENT
_lp$ = 8
?T2OLE@@YAPAGPAG@Z PROC NEAR				; T2OLE, COMDAT

; 127  : 	inline LPOLESTR T2OLE(LPTSTR lp) { return lp; }

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR _lp$[ebp]
	pop	ebp
	ret	0
?T2OLE@@YAPAGPAG@Z ENDP					; T2OLE
_TEXT	ENDS
PUBLIC	?GetReObject@CReFileObject@@IBEHAAU_reobject@@@Z ; CReFileObject::GetReObject
EXTRN	_memset:NEAR
;	COMDAT ?GetReObject@CReFileObject@@IBEHAAU_reobject@@@Z
_TEXT	SEGMENT
_reObj$ = 8
_this$ = -24
_clsid$ = -20
_sc$ = -4
?GetReObject@CReFileObject@@IBEHAAU_reobject@@@Z PROC NEAR ; CReFileObject::GetReObject, COMDAT

; 373  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 24					; 00000018H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 374  :    if (!m_pObject)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jne	SHORT $L78444

; 375  :       return FALSE;

	xor	eax, eax
	jmp	$L78443
$L78444:

; 376  : 
; 377  :    ZeroMemory(&reObj, sizeof(reObj));

	push	56					; 00000038H
	push	0
	mov	ecx, DWORD PTR _reObj$[ebp]
	push	ecx
	call	_memset
	add	esp, 12					; 0000000cH

; 378  : 
; 379  :    reObj.cbStruct = sizeof(reObj);

	mov	edx, DWORD PTR _reObj$[ebp]
	mov	DWORD PTR [edx], 56			; 00000038H

; 380  : 
; 381  :    CLSID clsid;
; 382  :    SCODE sc = m_pObject->GetUserClassID(&clsid);

	mov	esi, esp
	lea	eax, DWORD PTR _clsid$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	eax, DWORD PTR [ecx]
	push	edx
	call	DWORD PTR [eax+60]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _sc$[ebp], eax

; 383  : 
; 384  :    if (sc != S_OK)

	cmp	DWORD PTR _sc$[ebp], 0
	je	SHORT $L78448

; 385  :       return FALSE;

	xor	eax, eax
	jmp	$L78443
$L78448:

; 386  : 
; 387  :    reObj.clsid = clsid;

	mov	ecx, DWORD PTR _reObj$[ebp]
	add	ecx, 8
	mov	edx, DWORD PTR _clsid$[ebp]
	mov	DWORD PTR [ecx], edx
	mov	eax, DWORD PTR _clsid$[ebp+4]
	mov	DWORD PTR [ecx+4], eax
	mov	edx, DWORD PTR _clsid$[ebp+8]
	mov	DWORD PTR [ecx+8], edx
	mov	eax, DWORD PTR _clsid$[ebp+12]
	mov	DWORD PTR [ecx+12], eax

; 388  :    reObj.cp = REO_CP_SELECTION;

	mov	ecx, DWORD PTR _reObj$[ebp]
	mov	DWORD PTR [ecx+4], -1

; 389  :    reObj.dvaspect = DVASPECT_ICON;

	mov	edx, DWORD PTR _reObj$[ebp]
	mov	DWORD PTR [edx+44], 4

; 390  :    reObj.dwFlags = REO_RESIZABLE | REO_BELOWBASELINE;

	mov	eax, DWORD PTR _reObj$[ebp]
	mov	DWORD PTR [eax+48], 3

; 391  :    reObj.dwUser = 0;

	mov	ecx, DWORD PTR _reObj$[ebp]
	mov	DWORD PTR [ecx+52], 0

; 392  :    reObj.poleobj = m_pObject;

	mov	edx, DWORD PTR _reObj$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR [edx+24], ecx

; 393  :    reObj.polesite = m_pClientSite;

	mov	edx, DWORD PTR _reObj$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+20]
	mov	DWORD PTR [edx+32], ecx

; 394  :    reObj.pstg = m_pStorage;

	mov	edx, DWORD PTR _reObj$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR [edx+28], ecx

; 395  :    reObj.sizel.cx = reObj.sizel.cy = 0;

	mov	edx, DWORD PTR _reObj$[ebp]
	mov	DWORD PTR [edx+40], 0
	mov	eax, DWORD PTR _reObj$[ebp]
	mov	DWORD PTR [eax+36], 0

; 396  : 
; 397  :    return TRUE;

	mov	eax, 1
$L78443:

; 398  : }

	pop	esi
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetReObject@CReFileObject@@IBEHAAU_reobject@@@Z ENDP	; CReFileObject::GetReObject
_TEXT	ENDS
;	COMDAT ?Reset@CReFileObject@@IAEXXZ
_TEXT	SEGMENT
_this$ = -4
?Reset@CReFileObject@@IAEXXZ PROC NEAR			; CReFileObject::Reset, COMDAT

; 402  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 403  :    RELEASE_INTERFACE(m_pObject);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	je	SHORT $L78453
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	eax, DWORD PTR [ecx]
	mov	esi, esp
	push	edx
	call	DWORD PTR [eax+8]
	cmp	esi, esp
	call	__chkesp
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+12], 0
$L78453:

; 404  :    RELEASE_INTERFACE(m_pStorage);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+16], 0
	je	SHORT $L78454
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	push	ecx
	call	DWORD PTR [edx+8]
	cmp	esi, esp
	call	__chkesp
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+16], 0
$L78454:

; 405  :    RELEASE_INTERFACE(m_pClientSite);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+20], 0
	je	SHORT $L78455
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+20]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+20]
	mov	ecx, DWORD PTR [edx]
	mov	esi, esp
	push	eax
	call	DWORD PTR [ecx+8]
	cmp	esi, esp
	call	__chkesp
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+20], 0
$L78455:

; 406  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Reset@CReFileObject@@IAEXXZ ENDP			; CReFileObject::Reset
_TEXT	ENDS
;	COMDAT ?Insert@CReFileObject@@QAEHPBGH@Z
_TEXT	SEGMENT
_szFilePath$ = 8
_bLink$ = 12
_this$ = -60
_reObj$78462 = -56
?Insert@CReFileObject@@QAEHPBGH@Z PROC NEAR		; CReFileObject::Insert, COMDAT

; 409  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 60					; 0000003cH
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-60]
	mov	ecx, 15					; 0000000fH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 410  :    if (Create(szFilePath, bLink))

	mov	eax, DWORD PTR _bLink$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szFilePath$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Create@CReFileObject@@IAEHPBGH@Z	; CReFileObject::Create
	test	eax, eax
	je	SHORT $L78463

; 412  :       REOBJECT reObj;
; 413  : 
; 414  :       if (GetReObject(reObj))

	lea	edx, DWORD PTR _reObj$78462[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetReObject@CReFileObject@@IBEHAAU_reobject@@@Z ; CReFileObject::GetReObject
	test	eax, eax
	je	SHORT $L78463

; 415  :          return (S_OK == m_pRichEditOle->InsertObject(&reObj));

	mov	esi, esp
	lea	eax, DWORD PTR _reObj$78462[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	eax, DWORD PTR [ecx]
	push	edx
	call	DWORD PTR [eax+28]
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax
	jmp	SHORT $L78460
$L78463:

; 417  : 
; 418  :    return FALSE;

	xor	eax, eax
$L78460:

; 419  : }

	pop	edi
	pop	esi
	add	esp, 60					; 0000003cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Insert@CReFileObject@@QAEHPBGH@Z ENDP			; CReFileObject::Insert
_TEXT	ENDS
END

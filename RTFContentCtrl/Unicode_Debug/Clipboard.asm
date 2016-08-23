	TITLE	D:\_CODE\Shared\Clipboard.cpp
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
;	COMDAT ??_C@_05KIFI@with?5?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_09LOPF@?5elements?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_01BJG@?6?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_11A@?$AA?$AA@
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
;	COMDAT _$E267
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E268
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CLIPBOARDDATA@@QAE@ABVCString@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CClipboard@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CClipboard@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CClipboard@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Open@CClipboard@@QAEHPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Open@CClipboard@@IAEHPAUHWND__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Close@CClipboard@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Clear@CClipboard@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetData@CClipboard@@QBEHAAV?$CArray@UCLIPBOARDDATA@@AAU1@@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CLIPBOARDDATA@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetData@CClipboard@@QBEHABV?$CArray@UCLIPBOARDDATA@@AAU1@@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetData@CClipboard@@QBEHIPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetData@CClipboard@@QBEHABVCBitmap@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HasFormat@CClipboard@@SAHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PackageContent@CClipboard@@SAAAVCString@@AAV2@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?UnpackageContent@CClipboard@@SAAAVCString@@AAV2@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E275
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E274
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FormatAndReplace@CClipboard@@KAXHHABVCString@@AAV2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFormat@CClipboard@@KAII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetText@CClipboard@@QBE?AVCString@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetText@CClipboard@@QBEHAAVCString@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetText@CClipboard@@QBEHABVCString@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetDropFilePaths@CClipboard@@QBEHAAVCStringArray@@@Z
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
;	COMDAT ?GetSize@?$CArray@UCLIPBOARDDATA@@AAU1@@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Add@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEHAAUCLIPBOARDDATA@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CArray@UCLIPBOARDDATA@@AAU1@@@QBE?AUCLIPBOARDDATA@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CArray@UCLIPBOARDDATA@@AAU1@@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CArray@UCLIPBOARDDATA@@AAU1@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CArray@UCLIPBOARDDATA@@AAU1@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CArray@UCLIPBOARDDATA@@AAU1@@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CArray@UCLIPBOARDDATA@@AAU1@@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetSize@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAt@?$CArray@UCLIPBOARDDATA@@AAU1@@@QBE?AUCLIPBOARDDATA@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetAtGrow@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXHAAUCLIPBOARDDATA@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAUCLIPBOARDDATA@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAUCLIPBOARDDATA@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBUCLIPBOARDDATA@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CLIPBOARDDATA@@QAE@ABU0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4CLIPBOARDDATA@@QAEAAU0@ABU0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCLIPBOARDDATA@@QAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAUCLIPBOARDDATA@@H@Z
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
;	COMDAT ??_7?$CArray@UCLIPBOARDDATA@@AAU1@@@6B@
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
FLAT	GROUP _DATA, CONST, _BSS, CRT$XCA, CRT$XCU, CRT$XCL, CRT$XCC, CRT$XCZ
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
_DATA	SEGMENT
__szAfxTempl DB	'afxtempl.h', 00H
_DATA	ENDS
CRT$XCU	SEGMENT
_$S269	DD	FLAT:_$E268
CRT$XCU	ENDS
_DATA	SEGMENT
	ORG $+1
$SG73784 DB	'H', 00H, 'T', 00H, 'M', 00H, 'L', 00H, ' ', 00H, 'F', 00H
	DB	'o', 00H, 'r', 00H, 'm', 00H, 'a', 00H, 't', 00H, 00H, 00H
_DATA	ENDS
EXTRN	__chkesp:NEAR
;	COMDAT _$E268
_TEXT	SEGMENT
_$E268	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E267
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E268	ENDP
_TEXT	ENDS
EXTRN	__imp__RegisterClipboardFormatW@4:NEAR
_BSS	SEGMENT
_CF_HTML DD	01H DUP (?)
_BSS	ENDS
;	COMDAT _$E267
_TEXT	SEGMENT
_$E267	PROC NEAR					; COMDAT

; 42   : static UINT CF_HTML = ::RegisterClipboardFormat(_T("HTML Format"));

	push	ebp
	mov	ebp, esp
	push	esi
	mov	esi, esp
	push	OFFSET FLAT:$SG73784
	call	DWORD PTR __imp__RegisterClipboardFormatW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _CF_HTML, eax
	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E267	ENDP
_TEXT	ENDS
PUBLIC	??0CClipboard@@QAE@XZ				; CClipboard::CClipboard
PUBLIC	?Open@CClipboard@@IAEHPAUHWND__@@H@Z		; CClipboard::Open
;	COMDAT ??0CClipboard@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0CClipboard@@QAE@XZ PROC NEAR				; CClipboard::CClipboard, COMDAT

; 15   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+4], 0

; 16   : 	// reading can be done anonymously
; 17   : 	Open(NULL, FALSE);

	push	0
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Open@CClipboard@@IAEHPAUHWND__@@H@Z	; CClipboard::Open

; 18   : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CClipboard@@QAE@XZ ENDP				; CClipboard::CClipboard
_TEXT	ENDS
PUBLIC	??0CClipboard@@QAE@PAUHWND__@@@Z		; CClipboard::CClipboard
PUBLIC	?Open@CClipboard@@QAEHPAUHWND__@@@Z		; CClipboard::Open
;	COMDAT ??0CClipboard@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
_this$ = -4
??0CClipboard@@QAE@PAUHWND__@@@Z PROC NEAR		; CClipboard::CClipboard, COMDAT

; 21   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+4], 0

; 22   : 	Open(hWnd);

	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Open@CClipboard@@QAEHPAUHWND__@@@Z	; CClipboard::Open

; 23   : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0CClipboard@@QAE@PAUHWND__@@@Z ENDP			; CClipboard::CClipboard
_TEXT	ENDS
PUBLIC	??1CClipboard@@QAE@XZ				; CClipboard::~CClipboard
PUBLIC	?Close@CClipboard@@QAEHXZ			; CClipboard::Close
;	COMDAT ??1CClipboard@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CClipboard@@QAE@XZ PROC NEAR				; CClipboard::~CClipboard, COMDAT

; 26   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 27   : 	Close();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Close@CClipboard@@QAEHXZ		; CClipboard::Close

; 28   : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CClipboard@@QAE@XZ ENDP				; CClipboard::~CClipboard
_TEXT	ENDS
;	COMDAT ?Open@CClipboard@@QAEHPAUHWND__@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
_this$ = -4
?Open@CClipboard@@QAEHPAUHWND__@@@Z PROC NEAR		; CClipboard::Open, COMDAT

; 31   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 32   : 	return Open(hWnd, TRUE);

	push	1
	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Open@CClipboard@@IAEHPAUHWND__@@H@Z	; CClipboard::Open

; 33   : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Open@CClipboard@@QAEHPAUHWND__@@@Z ENDP		; CClipboard::Open
_TEXT	ENDS
PUBLIC	?Clear@CClipboard@@QBEHXZ			; CClipboard::Clear
EXTRN	__imp__IsWindow@4:NEAR
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	__imp__OpenClipboard@4:NEAR
;	COMDAT data
; File D:\_CODE\Shared\Clipboard.cpp
data	SEGMENT
$SG74541 DB	'D:\_CODE\Shared\Clipboard.cpp', 00H
	ORG $+2
$SG74553 DB	'D:\_CODE\Shared\Clipboard.cpp', 00H
data	ENDS
;	COMDAT ?Open@CClipboard@@IAEHPAUHWND__@@H@Z
_TEXT	SEGMENT
_hWnd$ = 8
_bClear$ = 12
_this$ = -4
?Open@CClipboard@@IAEHPAUHWND__@@H@Z PROC NEAR		; CClipboard::Open, COMDAT

; 37   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74535:

; 38   : 	// sanity check
; 39   : 	ASSERT((::IsWindow(hWnd) && bClear) || (!hWnd && !bClear));

	mov	esi, esp
	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L74539
	cmp	DWORD PTR _bClear$[ebp], 0
	jne	SHORT $L74538
$L74539:
	cmp	DWORD PTR _hWnd$[ebp], 0
	jne	SHORT $L74540
	cmp	DWORD PTR _bClear$[ebp], 0
	je	SHORT $L74538
$L74540:
	push	39					; 00000027H
	push	OFFSET FLAT:$SG74541
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74538
	int	3
$L74538:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74535

; 40   : 
; 41   : 	if ((::IsWindow(hWnd) && !bClear) || (!hWnd && bClear))

	mov	esi, esp
	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L74544
	cmp	DWORD PTR _bClear$[ebp], 0
	je	SHORT $L74543
$L74544:
	cmp	DWORD PTR _hWnd$[ebp], 0
	jne	SHORT $L74542
	cmp	DWORD PTR _bClear$[ebp], 0
	je	SHORT $L74542
$L74543:

; 42   : 		return FALSE;

	xor	eax, eax
	jmp	$L74534
$L74542:

; 43   : 
; 44   : 	if (m_bOpen && hWnd && (hWnd != m_hWnd))

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74545
	cmp	DWORD PTR _hWnd$[ebp], 0
	je	SHORT $L74545
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _hWnd$[ebp]
	cmp	edx, DWORD PTR [ecx]
	je	SHORT $L74545

; 45   : 		Close();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Close@CClipboard@@QAEHXZ		; CClipboard::Close
$L74545:

; 46   : 	
; 47   : 	if (::OpenClipboard(hWnd))

	mov	esi, esp
	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	call	DWORD PTR __imp__OpenClipboard@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L74546

; 49   : 		m_hWnd = hWnd;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _hWnd$[ebp]
	mov	DWORD PTR [ecx], edx

; 50   : 		m_bOpen = TRUE;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+4], 1

; 51   : 			
; 52   : 		if (bClear)

	cmp	DWORD PTR _bClear$[ebp], 0
	je	SHORT $L74547

; 53   : 			Clear();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Clear@CClipboard@@QBEHXZ		; CClipboard::Clear
$L74547:

; 55   : 	else

	jmp	SHORT $L74551
$L74546:

; 56   : 		ASSERT(0);

	mov	ecx, 1
	test	ecx, ecx
	je	SHORT $L74552
	push	56					; 00000038H
	push	OFFSET FLAT:$SG74553
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74552
	int	3
$L74552:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74546
$L74551:

; 57   : 	
; 58   : 	return m_bOpen;

	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+4]
$L74534:

; 59   : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Open@CClipboard@@IAEHPAUHWND__@@H@Z ENDP		; CClipboard::Open
_TEXT	ENDS
EXTRN	__imp__CloseClipboard@0:NEAR
;	COMDAT ?Close@CClipboard@@QAEHXZ
_TEXT	SEGMENT
_this$ = -4
?Close@CClipboard@@QAEHXZ PROC NEAR			; CClipboard::Close, COMDAT

; 62   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 63   : 	if (m_bOpen && ::CloseClipboard())

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74557
	mov	esi, esp
	call	DWORD PTR __imp__CloseClipboard@0
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L74557

; 65   : 		m_bOpen = FALSE;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+4], 0

; 66   : 		m_hWnd = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], 0

; 67   : 
; 68   : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L74556
$L74557:

; 70   : 	// else already closed or never opened
; 71   : 	
; 72   : 	return FALSE;

	xor	eax, eax
$L74556:

; 73   : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Close@CClipboard@@QAEHXZ ENDP				; CClipboard::Close
_TEXT	ENDS
EXTRN	__imp__EmptyClipboard@0:NEAR
;	COMDAT data
; File D:\_CODE\Shared\Clipboard.cpp
data	SEGMENT
$SG74566 DB	'D:\_CODE\Shared\Clipboard.cpp', 00H
data	ENDS
;	COMDAT ?Clear@CClipboard@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?Clear@CClipboard@@QBEHXZ PROC NEAR			; CClipboard::Clear, COMDAT

; 76   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74561:

; 77   : 	// sanity checks
; 78   : 	ASSERT(m_bOpen && m_hWnd);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74565
	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L74564
$L74565:
	push	78					; 0000004eH
	push	OFFSET FLAT:$SG74566
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74564
	int	3
$L74564:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74561

; 79   : 	
; 80   : 	if (!m_bOpen || (m_hWnd == NULL))

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74568
	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L74567
$L74568:

; 81   : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L74560
$L74567:

; 82   : 	
; 83   : 	return ::EmptyClipboard();

	mov	esi, esp
	call	DWORD PTR __imp__EmptyClipboard@0
	cmp	esi, esp
	call	__chkesp
$L74560:

; 84   : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Clear@CClipboard@@QBEHXZ ENDP				; CClipboard::Clear
_TEXT	ENDS
PUBLIC	??0CLIPBOARDDATA@@QAE@ABVCString@@I@Z		; CLIPBOARDDATA::CLIPBOARDDATA
PUBLIC	??1CLIPBOARDDATA@@QAE@XZ			; CLIPBOARDDATA::~CLIPBOARDDATA
PUBLIC	?GetText@CClipboard@@QBEHAAVCString@@I@Z	; CClipboard::GetText
PUBLIC	?GetData@CClipboard@@QBEHAAV?$CArray@UCLIPBOARDDATA@@AAU1@@@@Z ; CClipboard::GetData
PUBLIC	?GetSize@?$CArray@UCLIPBOARDDATA@@AAU1@@@QBEHXZ	; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::GetSize
PUBLIC	?RemoveAll@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXXZ ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::RemoveAll
PUBLIC	?Add@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEHAAUCLIPBOARDDATA@@@Z ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::Add
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	__imp__EnumClipboardFormats@4:NEAR
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
_BSS	SEGMENT
$SG74644 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Clipboard.cpp
data	SEGMENT
$SG74577 DB	'D:\_CODE\Shared\Clipboard.cpp', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T75300	DD	019930520H
	DD	03H
	DD	FLAT:$T75303
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75303	DD	0ffffffffH
	DD	FLAT:$L75295
	DD	00H
	DD	FLAT:$L75296
	DD	0ffffffffH
	DD	FLAT:$L75296
xdata$x	ENDS
;	COMDAT ?GetData@CClipboard@@QBEHAAV?$CArray@UCLIPBOARDDATA@@AAU1@@@@Z
_TEXT	SEGMENT
$T75293 = -28
$T75294 = -32
__$EHRec$ = -12
_aData$ = 8
_this$ = -36
_nFmt$ = -16
_data$ = -24
?GetData@CClipboard@@QBEHAAV?$CArray@UCLIPBOARDDATA@@AAU1@@@@Z PROC NEAR ; CClipboard::GetData, COMDAT

; 87   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75301
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
$L74573:

; 88   : 	ASSERT(m_bOpen);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74576
	push	88					; 00000058H
	push	OFFSET FLAT:$SG74577
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74576
	int	3
$L74576:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74573

; 89   : 
; 90   : 	if (!m_bOpen)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	jne	SHORT $L74578

; 91   : 		return 0;

	xor	eax, eax
	jmp	$L74572
$L74578:

; 92   : 
; 93   : 	aData.RemoveAll();

	mov	ecx, DWORD PTR _aData$[ebp]
	call	?RemoveAll@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXXZ ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::RemoveAll

; 94   : 
; 95   : 	UINT nFmt = ::EnumClipboardFormats(0);

	mov	esi, esp
	push	0
	call	DWORD PTR __imp__EnumClipboardFormats@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nFmt$[ebp], eax

; 96   : 	CLIPBOARDDATA data;

	push	OFFSET FLAT:$SG74644
	lea	ecx, DWORD PTR $T75293[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	push	0
	lea	eax, DWORD PTR $T75293[ebp]
	push	eax
	lea	ecx, DWORD PTR _data$[ebp]
	call	??0CLIPBOARDDATA@@QAE@ABVCString@@I@Z	; CLIPBOARDDATA::CLIPBOARDDATA
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T75293[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74646:

; 97   : 
; 98   : 	while (nFmt)

	cmp	DWORD PTR _nFmt$[ebp], 0
	je	SHORT $L74647

; 100  : 		if (!GetText(data.sData, nFmt))

	mov	ecx, DWORD PTR _nFmt$[ebp]
	push	ecx
	lea	edx, DWORD PTR _data$[ebp+4]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetText@CClipboard@@QBEHAAVCString@@I@Z ; CClipboard::GetText
	test	eax, eax
	jne	SHORT $L74648

; 101  : 			continue;

	jmp	SHORT $L74646
$L74648:

; 102  : 
; 103  : 		data.nFormat = nFmt;

	mov	eax, DWORD PTR _nFmt$[ebp]
	mov	DWORD PTR _data$[ebp], eax

; 104  : 		aData.Add(data);

	lea	ecx, DWORD PTR _data$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _aData$[ebp]
	call	?Add@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEHAAUCLIPBOARDDATA@@@Z ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::Add

; 105  : 
; 106  : 		// next
; 107  : 		nFmt = ::EnumClipboardFormats(nFmt);

	mov	esi, esp
	mov	edx, DWORD PTR _nFmt$[ebp]
	push	edx
	call	DWORD PTR __imp__EnumClipboardFormats@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nFmt$[ebp], eax

; 108  : 	}

	jmp	SHORT $L74646
$L74647:

; 109  : 
; 110  : 	return aData.GetSize();

	mov	ecx, DWORD PTR _aData$[ebp]
	call	?GetSize@?$CArray@UCLIPBOARDDATA@@AAU1@@@QBEHXZ ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::GetSize
	mov	DWORD PTR $T75294[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _data$[ebp]
	call	??1CLIPBOARDDATA@@QAE@XZ		; CLIPBOARDDATA::~CLIPBOARDDATA
	mov	eax, DWORD PTR $T75294[ebp]
$L74572:

; 111  : }

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
$L75295:
	lea	ecx, DWORD PTR $T75293[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75296:
	lea	ecx, DWORD PTR _data$[ebp]
	call	??1CLIPBOARDDATA@@QAE@XZ		; CLIPBOARDDATA::~CLIPBOARDDATA
	ret	0
$L75301:
	mov	eax, OFFSET FLAT:$T75300
	jmp	___CxxFrameHandler
text$x	ENDS
?GetData@CClipboard@@QBEHAAV?$CArray@UCLIPBOARDDATA@@AAU1@@@@Z ENDP ; CClipboard::GetData
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
;	COMDAT ??0CLIPBOARDDATA@@QAE@ABVCString@@I@Z
_TEXT	SEGMENT
_str$ = 8
_nFmt$ = 12
_this$ = -4
??0CLIPBOARDDATA@@QAE@ABVCString@@I@Z PROC NEAR		; CLIPBOARDDATA::CLIPBOARDDATA, COMDAT

; 48   : 	CLIPBOARDDATA(const CString& str = _T(""), UINT nFmt = 0) : sData(str), nFormat(nFmt) {}

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nFmt$[ebp]
	mov	DWORD PTR [eax], ecx
	mov	edx, DWORD PTR _str$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
??0CLIPBOARDDATA@@QAE@ABVCString@@I@Z ENDP		; CLIPBOARDDATA::CLIPBOARDDATA
_TEXT	ENDS
;	COMDAT ??1CLIPBOARDDATA@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CLIPBOARDDATA@@QAE@XZ PROC NEAR			; CLIPBOARDDATA::~CLIPBOARDDATA, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??1CString@@QAE@XZ			; CString::~CString
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CLIPBOARDDATA@@QAE@XZ ENDP				; CLIPBOARDDATA::~CLIPBOARDDATA
_TEXT	ENDS
PUBLIC	?SetText@CClipboard@@QBEHABVCString@@I@Z	; CClipboard::SetText
PUBLIC	?SetData@CClipboard@@QBEHABV?$CArray@UCLIPBOARDDATA@@AAU1@@@@Z ; CClipboard::SetData
PUBLIC	??A?$CArray@UCLIPBOARDDATA@@AAU1@@@QBE?AUCLIPBOARDDATA@@H@Z ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::operator[]
;	COMDAT data
; File D:\_CODE\Shared\Clipboard.cpp
data	SEGMENT
$SG74661 DB	'D:\_CODE\Shared\Clipboard.cpp', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T75319	DD	019930520H
	DD	01H
	DD	FLAT:$T75321
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75321	DD	0ffffffffH
	DD	FLAT:$L75317
xdata$x	ENDS
;	COMDAT ?SetData@CClipboard@@QBEHABV?$CArray@UCLIPBOARDDATA@@AAU1@@@@Z
_TEXT	SEGMENT
$T75316 = -32
__$EHRec$ = -12
_aData$ = 8
_this$ = -36
_nItem$ = -16
_clipData$74668 = -20
_$S272$74670 = -28
?SetData@CClipboard@@QBEHABV?$CArray@UCLIPBOARDDATA@@AAU1@@@@Z PROC NEAR ; CClipboard::SetData, COMDAT

; 114  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75320
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
$L74656:

; 115  : 	// sanity checks
; 116  : 	ASSERT(m_bOpen && m_hWnd);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74660
	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L74659
$L74660:
	push	116					; 00000074H
	push	OFFSET FLAT:$SG74661
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74659
	int	3
$L74659:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74656

; 117  : 	
; 118  : 	if (!m_bOpen || (m_hWnd == NULL))

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74663
	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L74662
$L74663:

; 119  : 		return FALSE;

	xor	eax, eax
	jmp	$L74655
$L74662:

; 120  : 
; 121  : 	int nItem = aData.GetSize();

	mov	ecx, DWORD PTR _aData$[ebp]
	call	?GetSize@?$CArray@UCLIPBOARDDATA@@AAU1@@@QBEHXZ ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::GetSize
	mov	DWORD PTR _nItem$[ebp], eax
$L74666:

; 122  : 
; 123  : 	while (nItem--)

	mov	edx, DWORD PTR _nItem$[ebp]
	mov	eax, DWORD PTR _nItem$[ebp]
	sub	eax, 1
	mov	DWORD PTR _nItem$[ebp], eax
	test	edx, edx
	je	SHORT $L74667

; 125  : 		const CLIPBOARDDATA& clipData = aData[nItem];

	mov	ecx, DWORD PTR _nItem$[ebp]
	push	ecx
	lea	edx, DWORD PTR _$S272$74670[ebp]
	push	edx
	mov	ecx, DWORD PTR _aData$[ebp]
	call	??A?$CArray@UCLIPBOARDDATA@@AAU1@@@QBE?AUCLIPBOARDDATA@@H@Z ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::operator[]
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	lea	eax, DWORD PTR _$S272$74670[ebp]
	mov	DWORD PTR _clipData$74668[ebp], eax

; 126  : 		
; 127  : 		if (!SetText(clipData.sData, clipData.nFormat))

	mov	ecx, DWORD PTR _clipData$74668[ebp]
	mov	edx, DWORD PTR [ecx]
	push	edx
	mov	eax, DWORD PTR _clipData$74668[ebp]
	add	eax, 4
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetText@CClipboard@@QBEHABVCString@@I@Z ; CClipboard::SetText
	test	eax, eax
	jne	SHORT $L74671

; 128  : 			return FALSE;

	mov	DWORD PTR $T75316[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _$S272$74670[ebp]
	call	??1CLIPBOARDDATA@@QAE@XZ		; CLIPBOARDDATA::~CLIPBOARDDATA
	mov	eax, DWORD PTR $T75316[ebp]
	jmp	SHORT $L74655
$L74671:

; 129  : 	}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _$S272$74670[ebp]
	call	??1CLIPBOARDDATA@@QAE@XZ		; CLIPBOARDDATA::~CLIPBOARDDATA
	jmp	SHORT $L74666
$L74667:

; 130  : 
; 131  : 	return TRUE;

	mov	eax, 1
$L74655:

; 132  : }

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
$L75317:
	lea	ecx, DWORD PTR _$S272$74670[ebp]
	call	??1CLIPBOARDDATA@@QAE@XZ		; CLIPBOARDDATA::~CLIPBOARDDATA
	ret	0
$L75320:
	mov	eax, OFFSET FLAT:$T75319
	jmp	___CxxFrameHandler
text$x	ENDS
?SetData@CClipboard@@QBEHABV?$CArray@UCLIPBOARDDATA@@AAU1@@@@Z ENDP ; CClipboard::SetData
PUBLIC	?SetData@CClipboard@@QBEHIPAX@Z			; CClipboard::SetData
EXTRN	__imp__SetClipboardData@8:NEAR
;	COMDAT data
; File D:\_CODE\Shared\Clipboard.cpp
data	SEGMENT
$SG74683 DB	'D:\_CODE\Shared\Clipboard.cpp', 00H
	ORG $+2
$SG74690 DB	'D:\_CODE\Shared\Clipboard.cpp', 00H
data	ENDS
;	COMDAT ?SetData@CClipboard@@QBEHIPAX@Z
_TEXT	SEGMENT
_uFormat$ = 8
_hMem$ = 12
_this$ = -4
?SetData@CClipboard@@QBEHIPAX@Z PROC NEAR		; CClipboard::SetData, COMDAT

; 135  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74678:

; 136  : 	// sanity checks
; 137  : 	ASSERT(m_bOpen && m_hWnd);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74682
	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L74681
$L74682:
	push	137					; 00000089H
	push	OFFSET FLAT:$SG74683
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74681
	int	3
$L74681:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74678

; 138  : 	
; 139  : 	if (!m_bOpen || (m_hWnd == NULL))

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74685
	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L74684
$L74685:

; 140  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L74677
$L74684:

; 141  : 
; 142  : 	ASSERT(hMem);

	cmp	DWORD PTR _hMem$[ebp], 0
	jne	SHORT $L74689
	push	142					; 0000008eH
	push	OFFSET FLAT:$SG74690
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74689
	int	3
$L74689:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74684

; 143  : 
; 144  : 	if (!hMem)

	cmp	DWORD PTR _hMem$[ebp], 0
	jne	SHORT $L74691

; 145  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L74677
$L74691:

; 146  : 	
; 147  : 	return (::SetClipboardData(uFormat, hMem) == hMem);

	mov	esi, esp
	mov	eax, DWORD PTR _hMem$[ebp]
	push	eax
	mov	ecx, DWORD PTR _uFormat$[ebp]
	push	ecx
	call	DWORD PTR __imp__SetClipboardData@8
	cmp	esi, esp
	call	__chkesp
	xor	edx, edx
	cmp	eax, DWORD PTR _hMem$[ebp]
	sete	dl
	mov	eax, edx
$L74677:

; 148  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SetData@CClipboard@@QBEHIPAX@Z ENDP			; CClipboard::SetData
_TEXT	ENDS
PUBLIC	?SetData@CClipboard@@QBEHABVCBitmap@@@Z		; CClipboard::SetData
EXTRN	?GetSafeHandle@CGdiObject@@QBEPAXXZ:NEAR	; CGdiObject::GetSafeHandle
;	COMDAT ?SetData@CClipboard@@QBEHABVCBitmap@@@Z
_TEXT	SEGMENT
_bm$ = 8
_this$ = -4
?SetData@CClipboard@@QBEHABVCBitmap@@@Z PROC NEAR	; CClipboard::SetData, COMDAT

; 151  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 152  : 	return SetData(CF_BITMAP, (HANDLE)bm.GetSafeHandle());

	mov	ecx, DWORD PTR _bm$[ebp]
	call	?GetSafeHandle@CGdiObject@@QBEPAXXZ	; CGdiObject::GetSafeHandle
	push	eax
	push	2
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetData@CClipboard@@QBEHIPAX@Z		; CClipboard::SetData

; 153  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?SetData@CClipboard@@QBEHABVCBitmap@@@Z ENDP		; CClipboard::SetData
_TEXT	ENDS
PUBLIC	?HasFormat@CClipboard@@SAHI@Z			; CClipboard::HasFormat
EXTRN	__imp__IsClipboardFormatAvailable@4:NEAR
;	COMDAT ?HasFormat@CClipboard@@SAHI@Z
_TEXT	SEGMENT
_nFormat$ = 8
?HasFormat@CClipboard@@SAHI@Z PROC NEAR			; CClipboard::HasFormat, COMDAT

; 156  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 157  : 	return (::IsClipboardFormatAvailable(nFormat));

	mov	esi, esp
	mov	eax, DWORD PTR _nFormat$[ebp]
	push	eax
	call	DWORD PTR __imp__IsClipboardFormatAvailable@4
	cmp	esi, esp
	call	__chkesp

; 158  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?HasFormat@CClipboard@@SAHI@Z ENDP			; CClipboard::HasFormat
_TEXT	ENDS
PUBLIC	?PackageContent@CClipboard@@SAAAVCString@@AAV2@I@Z ; CClipboard::PackageContent
PUBLIC	?FormatAndReplace@CClipboard@@KAXHHABVCString@@AAV2@@Z ; CClipboard::FormatAndReplace
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
EXTRN	??YCString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator+=
EXTRN	??YCString@@QAEABV0@PBG@Z:NEAR			; CString::operator+=
EXTRN	?TrimRight@CString@@QAEXXZ:NEAR			; CString::TrimRight
EXTRN	?Replace@CString@@QAEHPBG0@Z:NEAR		; CString::Replace
;	COMDAT data
; File D:\_CODE\Shared\Clipboard.cpp
data	SEGMENT
$SG74706 DB	'V', 00H, 'e', 00H, 'r', 00H, 's', 00H, 'i', 00H, 'o', 00H
	DB	'n', 00H, ':', 00H, '0', 00H, '.', 00H, '9', 00H, 0aH, 00H, 'S'
	DB	00H, 't', 00H, 'a', 00H, 'r', 00H, 't', 00H, 'H', 00H, 'T', 00H
	DB	'M', 00H, 'L', 00H, ':', 00H, 'B', 00H, 'B', 00H, 'B', 00H, 'B'
	DB	00H, 'B', 00H, 'B', 00H, 'B', 00H, 'B', 00H, 'B', 00H, 'B', 00H
	DB	0aH, 00H, 'E', 00H, 'n', 00H, 'd', 00H, 'H', 00H, 'T', 00H, 'M'
	DB	00H, 'L', 00H, ':', 00H, 'C', 00H, 'C', 00H, 'C', 00H, 'C', 00H
	DB	'C', 00H, 'C', 00H, 'C', 00H, 'C', 00H, 'C', 00H, 'C', 00H, 0aH
	DB	00H, 'S', 00H, 't', 00H, 'a', 00H, 'r', 00H, 't', 00H, 'F', 00H
	DB	'r', 00H, 'a', 00H, 'g', 00H, 'm', 00H, 'e', 00H, 'n', 00H, 't'
	DB	00H, ':', 00H, 'D', 00H, 'D', 00H, 'D', 00H, 'D', 00H, 'D', 00H
	DB	'D', 00H, 'D', 00H, 'D', 00H, 'D', 00H, 'D', 00H, 0aH, 00H, 'E'
	DB	00H, 'n', 00H, 'd', 00H, 'F', 00H, 'r', 00H, 'a', 00H, 'g', 00H
	DB	'm', 00H, 'e', 00H, 'n', 00H, 't', 00H, ':', 00H, 'E', 00H, 'E'
	DB	00H, 'E', 00H, 'E', 00H, 'E', 00H, 'E', 00H, 'E', 00H, 'E', 00H
	DB	'E', 00H, 'E', 00H, 0aH, 00H, 00H, 00H
	ORG $+2
$SG74708 DB	'B', 00H, 'B', 00H, 'B', 00H, 'B', 00H, 'B', 00H, 'B', 00H
	DB	'B', 00H, 'B', 00H, 'B', 00H, 'B', 00H, 00H, 00H
	ORG $+2
$SG74709 DB	'<', 00H, 'h', 00H, 't', 00H, 'm', 00H, 'l', 00H, '>', 00H
	DB	0aH, 00H, '<', 00H, 'b', 00H, 'o', 00H, 'd', 00H, 'y', 00H, '>'
	DB	00H, 0aH, 00H, '<', 00H, '!', 00H, '-', 00H, '-', 00H, 'S', 00H
	DB	't', 00H, 'a', 00H, 'r', 00H, 't', 00H, 'F', 00H, 'r', 00H, 'a'
	DB	00H, 'g', 00H, 'm', 00H, 'e', 00H, 'n', 00H, 't', 00H, '-', 00H
	DB	'-', 00H, '>', 00H, 00H, 00H
	ORG $+2
$SG74711 DB	'D', 00H, 'D', 00H, 'D', 00H, 'D', 00H, 'D', 00H, 'D', 00H
	DB	'D', 00H, 'D', 00H, 'D', 00H, 'D', 00H, 00H, 00H
	ORG $+2
$SG74712 DB	0aH, 00H, 00H, 00H
$SG74713 DB	0dH, 00H, 0aH, 00H, 00H, 00H
	ORG $+2
$SG74715 DB	'E', 00H, 'E', 00H, 'E', 00H, 'E', 00H, 'E', 00H, 'E', 00H
	DB	'E', 00H, 'E', 00H, 'E', 00H, 'E', 00H, 00H, 00H
	ORG $+2
$SG74716 DB	'<', 00H, '!', 00H, '-', 00H, '-', 00H, 'E', 00H, 'n', 00H
	DB	'd', 00H, 'F', 00H, 'r', 00H, 'a', 00H, 'g', 00H, 'm', 00H, 'e'
	DB	00H, 'n', 00H, 't', 00H, '-', 00H, '-', 00H, '>', 00H, 0aH, 00H
	DB	'<', 00H, '/', 00H, 'b', 00H, 'o', 00H, 'd', 00H, 'y', 00H, '>'
	DB	00H, 0aH, 00H, '<', 00H, '/', 00H, 'h', 00H, 't', 00H, 'm', 00H
	DB	'l', 00H, '>', 00H, 00H, 00H
	ORG $+2
$SG74718 DB	'C', 00H, 'C', 00H, 'C', 00H, 'C', 00H, 'C', 00H, 'C', 00H
	DB	'C', 00H, 'C', 00H, 'C', 00H, 'C', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T75344	DD	019930520H
	DD	05H
	DD	FLAT:$T75346
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75346	DD	0ffffffffH
	DD	FLAT:$L75338
	DD	00H
	DD	FLAT:$L75339
	DD	00H
	DD	FLAT:$L75340
	DD	00H
	DD	FLAT:$L75341
	DD	00H
	DD	FLAT:$L75342
xdata$x	ENDS
;	COMDAT ?PackageContent@CClipboard@@SAAAVCString@@AAV2@I@Z
_TEXT	SEGMENT
$T75334 = -20
$T75335 = -24
$T75336 = -28
$T75337 = -32
__$EHRec$ = -12
_sContent$ = 8
_nFormat$ = 12
_sHeader$74705 = -16
?PackageContent@CClipboard@@SAAAVCString@@AAV2@I@Z PROC NEAR ; CClipboard::PackageContent, COMDAT

; 161  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75345
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

; 162  : 	if (nFormat == CF_HTML)

	mov	eax, DWORD PTR _nFormat$[ebp]
	cmp	eax, DWORD PTR _CF_HTML
	jne	$L74704

; 164  : 		CString sHeader = _T("Version:0.9\n")
; 165  : 							_T("StartHTML:BBBBBBBBBB\n")
; 166  : 							_T("EndHTML:CCCCCCCCCC\n")
; 167  : 							_T("StartFragment:DDDDDDDDDD\n")
; 168  : 							_T("EndFragment:EEEEEEEEEE\n");

	push	OFFSET FLAT:$SG74706
	lea	ecx, DWORD PTR _sHeader$74705[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 169  : 		
; 170  : 		// Start Html
; 171  : 		FormatAndReplace(sHeader.GetLength(), 10, _T("BBBBBBBBBB"), sHeader);

	push	OFFSET FLAT:$SG74708
	lea	ecx, DWORD PTR $T75334[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sHeader$74705[ebp]
	push	ecx
	lea	edx, DWORD PTR $T75334[ebp]
	push	edx
	push	10					; 0000000aH
	lea	ecx, DWORD PTR _sHeader$74705[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	push	eax
	call	?FormatAndReplace@CClipboard@@KAXHHABVCString@@AAV2@@Z ; CClipboard::FormatAndReplace
	add	esp, 16					; 00000010H
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T75334[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 172  : 
; 173  : 		// Start Fragment
; 174  : 		sHeader += _T("<html>\n<body>\n<!--StartFragment-->");

	push	OFFSET FLAT:$SG74709
	lea	ecx, DWORD PTR _sHeader$74705[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=

; 175  : 		FormatAndReplace(sHeader.GetLength(), 10, _T("DDDDDDDDDD"), sHeader);

	push	OFFSET FLAT:$SG74711
	lea	ecx, DWORD PTR $T75335[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	eax, DWORD PTR _sHeader$74705[ebp]
	push	eax
	lea	ecx, DWORD PTR $T75335[ebp]
	push	ecx
	push	10					; 0000000aH
	lea	ecx, DWORD PTR _sHeader$74705[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	push	eax
	call	?FormatAndReplace@CClipboard@@KAXHHABVCString@@AAV2@@Z ; CClipboard::FormatAndReplace
	add	esp, 16					; 00000010H
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T75335[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 176  : 
; 177  : 		// End Fragment
; 178  : 		sHeader += sContent;

	mov	edx, DWORD PTR _sContent$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sHeader$74705[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=

; 179  : 		sHeader.TrimRight();

	lea	ecx, DWORD PTR _sHeader$74705[ebp]
	call	?TrimRight@CString@@QAEXXZ		; CString::TrimRight

; 180  : 		sHeader.Replace(_T("\r\n"), _T("\n"));

	push	OFFSET FLAT:$SG74712
	push	OFFSET FLAT:$SG74713
	lea	ecx, DWORD PTR _sHeader$74705[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 181  : 		FormatAndReplace(sHeader.GetLength(), 10, _T("EEEEEEEEEE"), sHeader);

	push	OFFSET FLAT:$SG74715
	lea	ecx, DWORD PTR $T75336[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	eax, DWORD PTR _sHeader$74705[ebp]
	push	eax
	lea	ecx, DWORD PTR $T75336[ebp]
	push	ecx
	push	10					; 0000000aH
	lea	ecx, DWORD PTR _sHeader$74705[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	push	eax
	call	?FormatAndReplace@CClipboard@@KAXHHABVCString@@AAV2@@Z ; CClipboard::FormatAndReplace
	add	esp, 16					; 00000010H
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T75336[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 182  : 
; 183  : 		// End Html
; 184  : 		sHeader += _T("<!--EndFragment-->\n</body>\n</html>");

	push	OFFSET FLAT:$SG74716
	lea	ecx, DWORD PTR _sHeader$74705[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=

; 185  : 		FormatAndReplace(sHeader.GetLength(), 10, _T("CCCCCCCCCC"), sHeader);

	push	OFFSET FLAT:$SG74718
	lea	ecx, DWORD PTR $T75337[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	edx, DWORD PTR _sHeader$74705[ebp]
	push	edx
	lea	eax, DWORD PTR $T75337[ebp]
	push	eax
	push	10					; 0000000aH
	lea	ecx, DWORD PTR _sHeader$74705[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	push	eax
	call	?FormatAndReplace@CClipboard@@KAXHHABVCString@@AAV2@@Z ; CClipboard::FormatAndReplace
	add	esp, 16					; 00000010H
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T75337[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 186  : 
; 187  : 		// copy back final result
; 188  : 		sContent = sHeader;

	lea	ecx, DWORD PTR _sHeader$74705[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sContent$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=

; 189  : 	}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sHeader$74705[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74704:

; 196  : 
; 197  : 	return sContent;

	mov	eax, DWORD PTR _sContent$[ebp]

; 198  : }

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
$L75338:
	lea	ecx, DWORD PTR _sHeader$74705[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75339:
	lea	ecx, DWORD PTR $T75334[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75340:
	lea	ecx, DWORD PTR $T75335[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75341:
	lea	ecx, DWORD PTR $T75336[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75342:
	lea	ecx, DWORD PTR $T75337[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75345:
	mov	eax, OFFSET FLAT:$T75344
	jmp	___CxxFrameHandler
text$x	ENDS
?PackageContent@CClipboard@@SAAAVCString@@AAV2@I@Z ENDP	; CClipboard::PackageContent
PUBLIC	?UnpackageContent@CClipboard@@SAAAVCString@@AAV2@I@Z ; CClipboard::UnpackageContent
EXTRN	_atexit:NEAR
EXTRN	__imp___wtoi:NEAR
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	?Mid@CString@@QBE?AV1@HH@Z:NEAR			; CString::Mid
EXTRN	?Find@CString@@QBEHPBG@Z:NEAR			; CString::Find
_BSS	SEGMENT
	ALIGN	4

_?STARTHTML@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4V3@A DD 01H DUP (?)
_?$S273@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4EA DB 01H DUP (?)
	ALIGN	4

_?ENDHTML@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4V3@A DD 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Clipboard.cpp
data	SEGMENT
$SG74730 DB	'S', 00H, 't', 00H, 'a', 00H, 'r', 00H, 't', 00H, 'H', 00H
	DB	'T', 00H, 'M', 00H, 'L', 00H, ':', 00H, 00H, 00H
	ORG $+2
$SG74735 DB	'E', 00H, 'n', 00H, 'd', 00H, 'H', 00H, 'T', 00H, 'M', 00H
	DB	'L', 00H, ':', 00H, 00H, 00H
	ORG $+2
$SG74744 DB	'D:\_CODE\Shared\Clipboard.cpp', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T75355	DD	019930520H
	DD	01H
	DD	FLAT:$T75357
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75357	DD	0ffffffffH
	DD	FLAT:$L75353
xdata$x	ENDS
;	COMDAT ?UnpackageContent@CClipboard@@SAAAVCString@@AAV2@I@Z
_TEXT	SEGMENT
$T75352 = -32
__$EHRec$ = -12
_sContent$ = 8
_nFormat$ = 12
_nStart$74737 = -16
_nEnd$74739 = -20
_szStart$74746 = -28
_szEnd$74748 = -24
?UnpackageContent@CClipboard@@SAAAVCString@@AAV2@I@Z PROC NEAR ; CClipboard::UnpackageContent, COMDAT

; 201  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75356
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

; 202  : 	if (nFormat == CF_HTML)

	mov	eax, DWORD PTR _nFormat$[ebp]
	cmp	eax, DWORD PTR _CF_HTML
	jne	$L74724

; 204  : 		static CString STARTHTML = _T("StartHTML:");

	xor	ecx, ecx
	mov	cl, BYTE PTR _?$S273@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4EA
	and	ecx, 1
	test	ecx, ecx
	jne	SHORT $L74729
	mov	dl, BYTE PTR _?$S273@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4EA
	or	dl, 1
	mov	BYTE PTR _?$S273@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4EA, dl
	push	OFFSET FLAT:$SG74730
	mov	ecx, OFFSET FLAT:_?STARTHTML@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4V3@A
	call	??0CString@@QAE@PBG@Z			; CString::CString
	push	OFFSET FLAT:_$E274
	call	_atexit
	add	esp, 4
$L74729:

; 205  : 		static CString ENDHTML = _T("EndHTML:");

	xor	eax, eax
	mov	al, BYTE PTR _?$S273@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4EA
	and	eax, 2
	test	eax, eax
	jne	SHORT $L74734
	mov	cl, BYTE PTR _?$S273@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4EA
	or	cl, 2
	mov	BYTE PTR _?$S273@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4EA, cl
	push	OFFSET FLAT:$SG74735
	mov	ecx, OFFSET FLAT:_?ENDHTML@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4V3@A
	call	??0CString@@QAE@PBG@Z			; CString::CString
	push	OFFSET FLAT:_$E275
	call	_atexit
	add	esp, 4
$L74734:

; 206  : 		
; 207  : 		// check that it is packaged
; 208  : 		int nStart = sContent.Find(STARTHTML);

	mov	ecx, OFFSET FLAT:_?STARTHTML@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4V3@A
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _sContent$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	mov	DWORD PTR _nStart$74737[ebp], eax

; 209  : 		
; 210  : 		if (nStart != -1)

	cmp	DWORD PTR _nStart$74737[ebp], -1
	je	$L74745

; 212  : 			int nEnd = sContent.Find(ENDHTML);

	mov	ecx, OFFSET FLAT:_?ENDHTML@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4V3@A
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _sContent$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	mov	DWORD PTR _nEnd$74739[ebp], eax
$L74740:

; 213  : 			ASSERT (nEnd > nStart);

	mov	edx, DWORD PTR _nEnd$74739[ebp]
	cmp	edx, DWORD PTR _nStart$74737[ebp]
	jg	SHORT $L74743
	push	213					; 000000d5H
	push	OFFSET FLAT:$SG74744
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74743
	int	3
$L74743:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74740

; 214  : 			
; 215  : 			if (nEnd > nStart)

	mov	ecx, DWORD PTR _nEnd$74739[ebp]
	cmp	ecx, DWORD PTR _nStart$74737[ebp]
	jle	$L74745

; 217  : 				// convert to char offsets
; 218  : 				LPCTSTR szStart = (((LPCTSTR)sContent) + nStart + STARTHTML.GetLength());

	mov	ecx, DWORD PTR _sContent$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	edx, DWORD PTR _nStart$74737[ebp]
	lea	esi, DWORD PTR [eax+edx*2]
	mov	ecx, OFFSET FLAT:_?STARTHTML@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4V3@A
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	lea	eax, DWORD PTR [esi+eax*2]
	mov	DWORD PTR _szStart$74746[ebp], eax

; 219  : 				LPCTSTR szEnd = (((LPCTSTR)sContent) + nEnd + ENDHTML.GetLength());

	mov	ecx, DWORD PTR _sContent$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	ecx, DWORD PTR _nEnd$74739[ebp]
	lea	esi, DWORD PTR [eax+ecx*2]
	mov	ecx, OFFSET FLAT:_?ENDHTML@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4V3@A
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	lea	edx, DWORD PTR [esi+eax*2]
	mov	DWORD PTR _szEnd$74748[ebp], edx

; 220  : 
; 221  : 				nStart = _ttoi(szStart);

	mov	esi, esp
	mov	eax, DWORD PTR _szStart$74746[ebp]
	push	eax
	call	DWORD PTR __imp___wtoi
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nStart$74737[ebp], eax

; 222  : 				nEnd = _ttoi(szEnd);

	mov	esi, esp
	mov	ecx, DWORD PTR _szEnd$74748[ebp]
	push	ecx
	call	DWORD PTR __imp___wtoi
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nEnd$74739[ebp], eax

; 223  : 				
; 224  : 				sContent = sContent.Mid(nStart, (nEnd - nStart));

	mov	edx, DWORD PTR _nEnd$74739[ebp]
	sub	edx, DWORD PTR _nStart$74737[ebp]
	push	edx
	mov	eax, DWORD PTR _nStart$74737[ebp]
	push	eax
	lea	ecx, DWORD PTR $T75352[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sContent$[ebp]
	call	?Mid@CString@@QBE?AV1@HH@Z		; CString::Mid
	mov	DWORD PTR -36+[ebp], eax
	mov	edx, DWORD PTR -36+[ebp]
	mov	DWORD PTR -40+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR -40+[ebp]
	push	eax
	mov	ecx, DWORD PTR _sContent$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T75352[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74745:
$L74724:

; 234  : 	
; 235  : 	return sContent;

	mov	eax, DWORD PTR _sContent$[ebp]

; 236  : }

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
$L75353:
	lea	ecx, DWORD PTR $T75352[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75356:
	mov	eax, OFFSET FLAT:$T75355
	jmp	___CxxFrameHandler
text$x	ENDS
?UnpackageContent@CClipboard@@SAAAVCString@@AAV2@I@Z ENDP ; CClipboard::UnpackageContent
;	COMDAT _$E275
_TEXT	SEGMENT
_$E275	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?ENDHTML@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4V3@A
	call	??1CString@@QAE@XZ			; CString::~CString
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
	mov	ecx, OFFSET FLAT:_?STARTHTML@?4??UnpackageContent@CClipboard@@SAAAVCString@@AAV3@I@Z@4V3@A
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E274	ENDP
_TEXT	ENDS
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	?Format@CString@@QAAXPBGZZ:NEAR			; CString::Format
;	COMDAT data
; File D:\_CODE\Shared\Clipboard.cpp
data	SEGMENT
$SG74764 DB	'D:\_CODE\Shared\Clipboard.cpp', 00H
	ORG $+2
$SG74766 DB	'%', 00H, '0', 00H, '*', 00H, 'd', 00H, 00H, 00H
	ORG $+2
$SG74771 DB	'D:\_CODE\Shared\Clipboard.cpp', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T75369	DD	019930520H
	DD	01H
	DD	FLAT:$T75371
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75371	DD	0ffffffffH
	DD	FLAT:$L75367
xdata$x	ENDS
;	COMDAT ?FormatAndReplace@CClipboard@@KAXHHABVCString@@AAV2@@Z
_TEXT	SEGMENT
_nVal$ = 8
_nWidth$ = 12
_sKey$ = 16
_sOutput$ = 20
_sVal$ = -16
__$EHRec$ = -12
?FormatAndReplace@CClipboard@@KAXHHABVCString@@AAV2@@Z PROC NEAR ; CClipboard::FormatAndReplace, COMDAT

; 239  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75370
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
$L74760:

; 240  : 	ASSERT(sOutput.Find(sKey) != -1);

	mov	ecx, DWORD PTR _sKey$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _sOutput$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	cmp	eax, -1
	jne	SHORT $L74763
	push	240					; 000000f0H
	push	OFFSET FLAT:$SG74764
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74763
	int	3
$L74763:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74760

; 241  : 
; 242  : 	CString sVal;

	lea	ecx, DWORD PTR _sVal$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 243  : 	sVal.Format(_T("%0*d"), nWidth, nVal);

	mov	ecx, DWORD PTR _nVal$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nWidth$[ebp]
	push	edx
	push	OFFSET FLAT:$SG74766
	lea	eax, DWORD PTR _sVal$[ebp]
	push	eax
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 16					; 00000010H
$L74767:

; 244  : 
; 245  : 	ASSERT(sVal.GetLength() == nWidth);

	lea	ecx, DWORD PTR _sVal$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	cmp	eax, DWORD PTR _nWidth$[ebp]
	je	SHORT $L74770
	push	245					; 000000f5H
	push	OFFSET FLAT:$SG74771
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74770
	int	3
$L74770:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74767

; 246  : 
; 247  : 	sOutput.Replace(sKey, sVal);

	lea	ecx, DWORD PTR _sVal$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _sKey$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _sOutput$[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 248  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sVal$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
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
$L75367:
	lea	ecx, DWORD PTR _sVal$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75370:
	mov	eax, OFFSET FLAT:$T75369
	jmp	___CxxFrameHandler
text$x	ENDS
?FormatAndReplace@CClipboard@@KAXHHABVCString@@AAV2@@Z ENDP ; CClipboard::FormatAndReplace
PUBLIC	?GetFormat@CClipboard@@KAII@Z			; CClipboard::GetFormat
;	COMDAT ?GetFormat@CClipboard@@KAII@Z
_TEXT	SEGMENT
_nFormat$ = 8
?GetFormat@CClipboard@@KAII@Z PROC NEAR			; CClipboard::GetFormat, COMDAT

; 251  : {

	push	ebp
	mov	ebp, esp

; 252  : 	if (nFormat == 0)

	cmp	DWORD PTR _nFormat$[ebp], 0
	jne	SHORT $L74775

; 254  : #ifndef _UNICODE
; 255  : 		nFormat = CF_TEXT;
; 256  : #else
; 257  : 		nFormat = CF_UNICODETEXT;

	mov	DWORD PTR _nFormat$[ebp], 13		; 0000000dH
$L74775:

; 260  : 
; 261  : 	return nFormat;

	mov	eax, DWORD PTR _nFormat$[ebp]

; 262  : }

	pop	ebp
	ret	0
?GetFormat@CClipboard@@KAII@Z ENDP			; CClipboard::GetFormat
_TEXT	ENDS
PUBLIC	?GetText@CClipboard@@QBE?AVCString@@I@Z		; CClipboard::GetText
;	COMDAT xdata$x
; File D:\_CODE\Shared\Clipboard.cpp
xdata$x	SEGMENT
$T75384	DD	019930520H
	DD	02H
	DD	FLAT:$T75386
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75386	DD	0ffffffffH
	DD	FLAT:$L75380
	DD	00H
	DD	FLAT:$L75379
xdata$x	ENDS
;	COMDAT ?GetText@CClipboard@@QBE?AVCString@@I@Z
_TEXT	SEGMENT
_nFormat$ = 12
___$ReturnUdt$ = 8
_this$ = -24
_sText$ = -16
$T75381 = -20
__$EHRec$ = -12
?GetText@CClipboard@@QBE?AVCString@@I@Z PROC NEAR	; CClipboard::GetText, COMDAT

; 265  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75385
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T75381[ebp], 0

; 266  : 	CString sText;

	lea	ecx, DWORD PTR _sText$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 267  : 	GetText(sText, nFormat);

	mov	eax, DWORD PTR _nFormat$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sText$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetText@CClipboard@@QBEHAAVCString@@I@Z ; CClipboard::GetText

; 268  : 
; 269  : 	return sText;

	lea	edx, DWORD PTR _sText$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T75381[ebp]
	or	al, 1
	mov	DWORD PTR $T75381[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 270  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75379:
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75380:
	mov	eax, DWORD PTR $T75381[ebp]
	and	eax, 1
	test	eax, eax
	je	$L75382
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75382:
	ret	0
$L75385:
	mov	eax, OFFSET FLAT:$T75384
	jmp	___CxxFrameHandler
text$x	ENDS
?GetText@CClipboard@@QBE?AVCString@@I@Z ENDP		; CClipboard::GetText
EXTRN	__imp__GlobalLock@4:NEAR
EXTRN	__imp__GlobalUnlock@4:NEAR
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
EXTRN	?Empty@CString@@QAEXXZ:NEAR			; CString::Empty
EXTRN	??4CString@@QAEABV0@PBG@Z:NEAR			; CString::operator=
EXTRN	__imp__GetClipboardData@4:NEAR
;	COMDAT data
; File D:\_CODE\Shared\Clipboard.cpp
data	SEGMENT
$SG74791 DB	'D:\_CODE\Shared\Clipboard.cpp', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T75394	DD	019930520H
	DD	02H
	DD	FLAT:$T75396
	DD	01H
	DD	FLAT:$T75397
	DD	2 DUP(00H)
	ORG $+4
$T75396	DD	0ffffffffH
	DD	00H
	DD	0ffffffffH
	DD	00H
$T75397	DD	00H
	DD	00H
	DD	01H
	DD	01H
	DD	FLAT:$T75398
	ORG $+4
$T75398	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L75392
xdata$x	ENDS
;	COMDAT ?GetText@CClipboard@@QBEHAAVCString@@I@Z
_TEXT	SEGMENT
_sText$ = 8
_nFormat$ = 12
_this$ = -24
_hData$74794 = -20
__$EHRec$ = -16
?GetText@CClipboard@@QBEHAAVCString@@I@Z PROC NEAR	; CClipboard::GetText, COMDAT

; 273  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75395
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR __$EHRec$[ebp], esp
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74787:

; 274  : 	ASSERT(m_bOpen);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74790
	push	274					; 00000112H
	push	OFFSET FLAT:$SG74791
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74790
	int	3
$L74790:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74787

; 275  : 	
; 276  : 	if (!m_bOpen)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	jne	SHORT $L74792

; 277  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L74786
$L74792:

; 278  : 
; 279  : 	nFormat = GetFormat(nFormat);

	mov	eax, DWORD PTR _nFormat$[ebp]
	push	eax
	call	?GetFormat@CClipboard@@KAII@Z		; CClipboard::GetFormat
	add	esp, 4
	mov	DWORD PTR _nFormat$[ebp], eax

; 280  : 	
; 281  : 	try
; 282  : 	{

	mov	DWORD PTR __$EHRec$[ebp+12], 0

; 283  : 		HANDLE hData = ::GetClipboardData(nFormat);

	mov	esi, esp
	mov	ecx, DWORD PTR _nFormat$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetClipboardData@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hData$74794[ebp], eax

; 284  : 		
; 285  : 		sText = (LPCTSTR)GlobalLock(hData);

	mov	esi, esp
	mov	edx, DWORD PTR _hData$74794[ebp]
	push	edx
	call	DWORD PTR __imp__GlobalLock@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	mov	ecx, DWORD PTR _sText$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 286  : 		::GlobalUnlock(hData);

	mov	esi, esp
	mov	eax, DWORD PTR _hData$74794[ebp]
	push	eax
	call	DWORD PTR __imp__GlobalUnlock@4
	cmp	esi, esp
	call	__chkesp

; 288  : 	catch(...)

	jmp	SHORT $L74793
$L75392:

; 289  : 	{
; 290  : 		sText.Empty();

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 291  : 	}

	mov	eax, $L74793
	ret	0
$L74793:

; 292  : 	
; 293  : 	return !sText.IsEmpty();

	mov	DWORD PTR __$EHRec$[ebp+12], -1
	mov	ecx, DWORD PTR _sText$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	neg	eax
	sbb	eax, eax
	inc	eax
$L74786:

; 294  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp+4]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75395:
	mov	eax, OFFSET FLAT:$T75394
	jmp	___CxxFrameHandler
text$x	ENDS
?GetText@CClipboard@@QBEHAAVCString@@I@Z ENDP		; CClipboard::GetText
EXTRN	__imp__GlobalAlloc@8:NEAR
EXTRN	__imp__GlobalFree@4:NEAR
EXTRN	_memcpy:NEAR
;	COMDAT data
; File D:\_CODE\Shared\Clipboard.cpp
data	SEGMENT
$SG74806 DB	'D:\_CODE\Shared\Clipboard.cpp', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T75407	DD	019930520H
	DD	02H
	DD	FLAT:$T75409
	DD	01H
	DD	FLAT:$T75410
	DD	2 DUP(00H)
	ORG $+4
$T75409	DD	0ffffffffH
	DD	00H
	DD	0ffffffffH
	DD	00H
$T75410	DD	00H
	DD	00H
	DD	01H
	DD	01H
	DD	FLAT:$T75411
	ORG $+4
$T75411	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L75405
xdata$x	ENDS
;	COMDAT ?SetText@CClipboard@@QBEHABVCString@@I@Z
_TEXT	SEGMENT
_sText$ = 8
_nFormat$ = 12
_this$ = -36
_hglbCopy$ = -24
_bResult$ = -20
_nBytes$74812 = -28
_pCopy$74815 = -32
__$EHRec$ = -16
?SetText@CClipboard@@QBEHABVCString@@I@Z PROC NEAR	; CClipboard::SetText, COMDAT

; 297  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75408
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	sub	esp, 32					; 00000020H
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR __$EHRec$[ebp], esp
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-48], eax
	mov	DWORD PTR [ebp-44], eax
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR _this$[ebp], ecx
$L74801:

; 298  : 	// sanity checks
; 299  : 	ASSERT(m_bOpen && m_hWnd);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74805
	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L74804
$L74805:
	push	299					; 0000012bH
	push	OFFSET FLAT:$SG74806
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74804
	int	3
$L74804:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74801

; 300  : 	
; 301  : 	if (!m_bOpen || (m_hWnd == NULL))

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74808
	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L74807
$L74808:

; 302  : 		return FALSE;

	xor	eax, eax
	jmp	$L74800
$L74807:

; 303  : 
; 304  : 	nFormat = GetFormat(nFormat);

	mov	edx, DWORD PTR _nFormat$[ebp]
	push	edx
	call	?GetFormat@CClipboard@@KAII@Z		; CClipboard::GetFormat
	add	esp, 4
	mov	DWORD PTR _nFormat$[ebp], eax

; 305  : 	
; 306  : 	HGLOBAL hglbCopy = NULL; 

	mov	DWORD PTR _hglbCopy$[ebp], 0

; 307  : 	BOOL bResult = FALSE;

	mov	DWORD PTR _bResult$[ebp], 0

; 308  : 	
; 309  : 	try
; 310  : 	{

	mov	DWORD PTR __$EHRec$[ebp+12], 0

; 311  : 		// Allocate a global memory object for the text. 
; 312  : 		size_t nBytes = ((sText.GetLength() + 1) * sizeof(TCHAR));

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR -40+[ebp], eax
	mov	eax, DWORD PTR -40+[ebp]
	lea	ecx, DWORD PTR [eax+eax+2]
	mov	DWORD PTR _nBytes$74812[ebp], ecx

; 313  : 		hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, nBytes); 

	mov	esi, esp
	mov	edx, DWORD PTR _nBytes$74812[ebp]
	push	edx
	push	2
	call	DWORD PTR __imp__GlobalAlloc@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hglbCopy$[ebp], eax

; 314  : 		
; 315  : 		if (hglbCopy) 

	cmp	DWORD PTR _hglbCopy$[ebp], 0
	je	SHORT $L74814

; 317  : 			// Lock the handle and copy the text to the buffer.
; 318  : 			LPVOID pCopy = ::GlobalLock(hglbCopy);

	mov	esi, esp
	mov	eax, DWORD PTR _hglbCopy$[ebp]
	push	eax
	call	DWORD PTR __imp__GlobalLock@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pCopy$74815[ebp], eax

; 319  : 			CopyMemory(pCopy, (LPCTSTR)sText, nBytes); 

	mov	ecx, DWORD PTR _sText$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -44+[ebp], eax
	mov	ecx, DWORD PTR _nBytes$74812[ebp]
	push	ecx
	mov	edx, DWORD PTR -44+[ebp]
	push	edx
	mov	eax, DWORD PTR _pCopy$74815[ebp]
	push	eax
	call	_memcpy
	add	esp, 12					; 0000000cH

; 320  : 			::GlobalUnlock(hglbCopy); 

	mov	esi, esp
	mov	ecx, DWORD PTR _hglbCopy$[ebp]
	push	ecx
	call	DWORD PTR __imp__GlobalUnlock@4
	cmp	esi, esp
	call	__chkesp

; 321  : 			
; 322  : 			// Place the handle on the clipboard. 
; 323  : 			bResult = SetData(nFormat, hglbCopy);

	mov	edx, DWORD PTR _hglbCopy$[ebp]
	push	edx
	mov	eax, DWORD PTR _nFormat$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetData@CClipboard@@QBEHIPAX@Z		; CClipboard::SetData
	mov	DWORD PTR -48+[ebp], eax
	mov	ecx, DWORD PTR -48+[ebp]
	mov	DWORD PTR _bResult$[ebp], ecx
$L74814:

; 326  : 	catch(...)

	jmp	SHORT $L74811
$L75405:

; 327  : 	{
; 328  : 	}

	mov	eax, $L74811
	ret	0
$L74811:

; 329  : 
; 330  : 	// cleanup
; 331  : 	if (!bResult && hglbCopy)

	mov	DWORD PTR __$EHRec$[ebp+12], -1
	cmp	DWORD PTR _bResult$[ebp], 0
	jne	SHORT $L74817
	cmp	DWORD PTR _hglbCopy$[ebp], 0
	je	SHORT $L74817

; 333  : 		::GlobalFree(hglbCopy);

	mov	esi, esp
	mov	edx, DWORD PTR _hglbCopy$[ebp]
	push	edx
	call	DWORD PTR __imp__GlobalFree@4
	cmp	esi, esp
	call	__chkesp
$L74817:

; 335  : 	
; 336  : 	return bResult;

	mov	eax, DWORD PTR _bResult$[ebp]
$L74800:

; 337  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp+4]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 48					; 00000030H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75408:
	mov	eax, OFFSET FLAT:$T75407
	jmp	___CxxFrameHandler
text$x	ENDS
?SetText@CClipboard@@QBEHABVCString@@I@Z ENDP		; CClipboard::SetText
PUBLIC	?GetDropFilePaths@CClipboard@@QBEHAAVCStringArray@@@Z ; CClipboard::GetDropFilePaths
EXTRN	?RemoveAll@CStringArray@@QAEXXZ:NEAR		; CStringArray::RemoveAll
EXTRN	?GetDropFilePaths@FileMisc@@YAHPAUHDROP__@@AAVCStringArray@@@Z:NEAR ; FileMisc::GetDropFilePaths
;	COMDAT data
; File D:\_CODE\Shared\Clipboard.cpp
data	SEGMENT
$SG74826 DB	'D:\_CODE\Shared\Clipboard.cpp', 00H
data	ENDS
;	COMDAT ?GetDropFilePaths@CClipboard@@QBEHAAVCStringArray@@@Z
_TEXT	SEGMENT
_aFilePaths$ = 8
_this$ = -8
_hData$ = -4
?GetDropFilePaths@CClipboard@@QBEHAAVCStringArray@@@Z PROC NEAR ; CClipboard::GetDropFilePaths, COMDAT

; 340  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74822:

; 341  : 	ASSERT(m_bOpen);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74825
	push	341					; 00000155H
	push	OFFSET FLAT:$SG74826
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74825
	int	3
$L74825:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74822

; 342  : 	
; 343  : 	if (!m_bOpen)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	jne	SHORT $L74827

; 344  : 		return -1; // error

	or	eax, -1
	jmp	SHORT $L74821
$L74827:

; 345  : 	
; 346  : 	aFilePaths.RemoveAll();

	mov	ecx, DWORD PTR _aFilePaths$[ebp]
	call	?RemoveAll@CStringArray@@QAEXXZ		; CStringArray::RemoveAll

; 347  : 	
; 348  : 	HANDLE hData = ::GetClipboardData(CF_HDROP);

	mov	esi, esp
	push	15					; 0000000fH
	call	DWORD PTR __imp__GetClipboardData@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hData$[ebp], eax

; 349  : 	
; 350  : 	if (hData == NULL)

	cmp	DWORD PTR _hData$[ebp], 0
	jne	SHORT $L74829

; 351  : 		return 0;

	xor	eax, eax
	jmp	SHORT $L74821
$L74829:

; 352  : 	
; 353  : 	return FileMisc::GetDropFilePaths((HDROP)hData, aFilePaths);

	mov	eax, DWORD PTR _aFilePaths$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hData$[ebp]
	push	ecx
	call	?GetDropFilePaths@FileMisc@@YAHPAUHDROP__@@AAVCStringArray@@@Z ; FileMisc::GetDropFilePaths
	add	esp, 8
$L74821:

; 354  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetDropFilePaths@CClipboard@@QBEHAAVCStringArray@@@Z ENDP ; CClipboard::GetDropFilePaths
_TEXT	ENDS
;	COMDAT ?GetSize@?$CArray@UCLIPBOARDDATA@@AAU1@@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?GetSize@?$CArray@UCLIPBOARDDATA@@AAU1@@@QBEHXZ PROC NEAR ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::GetSize, COMDAT

; 245  : 	{ return m_nSize; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+8]
	mov	esp, ebp
	pop	ebp
	ret	0
?GetSize@?$CArray@UCLIPBOARDDATA@@AAU1@@@QBEHXZ ENDP	; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::GetSize
_TEXT	ENDS
PUBLIC	?SetSize@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXHH@Z ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::SetSize
;	COMDAT ?RemoveAll@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXXZ
_TEXT	SEGMENT
_this$ = -4
?RemoveAll@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXXZ PROC NEAR ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::RemoveAll, COMDAT

; 251  : 	{ SetSize(0, -1); }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	-1
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetSize@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXHH@Z ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::SetSize
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?RemoveAll@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXXZ ENDP	; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::RemoveAll
_TEXT	ENDS
PUBLIC	?SetAtGrow@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXHAAUCLIPBOARDDATA@@@Z ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::SetAtGrow
;	COMDAT ?Add@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEHAAUCLIPBOARDDATA@@@Z
_TEXT	SEGMENT
_newElement$ = 8
_this$ = -8
_nIndex$ = -4
?Add@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEHAAUCLIPBOARDDATA@@@Z PROC NEAR ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::Add, COMDAT

; 272  : 	{ int nIndex = m_nSize;

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR _nIndex$[ebp], ecx

; 273  : 		SetAtGrow(nIndex, newElement);

	mov	edx, DWORD PTR _newElement$[ebp]
	push	edx
	mov	eax, DWORD PTR _nIndex$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetAtGrow@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXHAAUCLIPBOARDDATA@@@Z ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::SetAtGrow

; 274  : 		return nIndex; }

	mov	eax, DWORD PTR _nIndex$[ebp]
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Add@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEHAAUCLIPBOARDDATA@@@Z ENDP ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::Add
_TEXT	ENDS
PUBLIC	?GetAt@?$CArray@UCLIPBOARDDATA@@AAU1@@@QBE?AUCLIPBOARDDATA@@H@Z ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::GetAt
;	COMDAT ??A?$CArray@UCLIPBOARDDATA@@AAU1@@@QBE?AUCLIPBOARDDATA@@H@Z
_TEXT	SEGMENT
_nIndex$ = 12
___$ReturnUdt$ = 8
_this$ = -8
$T75427 = -4
??A?$CArray@UCLIPBOARDDATA@@AAU1@@@QBE?AUCLIPBOARDDATA@@H@Z PROC NEAR ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::operator[], COMDAT

; 277  : 	{ return GetAt(nIndex); }

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T75427[ebp], 0
	mov	eax, DWORD PTR _nIndex$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetAt@?$CArray@UCLIPBOARDDATA@@AAU1@@@QBE?AUCLIPBOARDDATA@@H@Z ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::GetAt
	mov	edx, DWORD PTR $T75427[ebp]
	or	edx, 1
	mov	DWORD PTR $T75427[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
??A?$CArray@UCLIPBOARDDATA@@AAU1@@@QBE?AUCLIPBOARDDATA@@H@Z ENDP ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::operator[]
_TEXT	ENDS
PUBLIC	?DestructElements@@YGXPAUCLIPBOARDDATA@@H@Z	; DestructElements
PUBLIC	?ConstructElements@@YGXPAUCLIPBOARDDATA@@H@Z	; ConstructElements
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
EXTRN	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z:NEAR ; AfxAssertValidObject
EXTRN	??2@YAPAXIPBDH@Z:NEAR				; operator new
;	COMDAT ?SetSize@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXHH@Z
_TEXT	SEGMENT
_nNewSize$ = 8
_nGrowBy$ = 12
_this$ = -32
_nGrowBy$75052 = -8
_nNewMax$75054 = -12
_pNewData$75066 = -4
$T75431 = -16
$T75432 = -20
$T75437 = -24
$T75438 = -28
?SetSize@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXHH@Z PROC NEAR ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::SetSize, COMDAT

; 306  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 40					; 00000028H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-40]
	mov	ecx, 10					; 0000000aH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 307  : 	ASSERT_VALID(this);

	push	307					; 00000133H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject
$L75009:

; 308  : 	ASSERT(nNewSize >= 0);

	cmp	DWORD PTR _nNewSize$[ebp], 0
	jge	SHORT $L75012
	push	308					; 00000134H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75012
	int	3
$L75012:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75009

; 309  : 
; 310  : 	if (nGrowBy != -1)

	cmp	DWORD PTR _nGrowBy$[ebp], -1
	je	SHORT $L75013

; 311  : 		m_nGrowBy = nGrowBy;  // set new size

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nGrowBy$[ebp]
	mov	DWORD PTR [edx+16], eax
$L75013:

; 312  : 
; 313  : 	if (nNewSize == 0)

	cmp	DWORD PTR _nNewSize$[ebp], 0
	jne	SHORT $L75014

; 315  : 		// shrink to nothing
; 316  : 		if (m_pData != NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L75015

; 318  : 			DestructElements<TYPE>(m_pData, m_nSize);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	?DestructElements@@YGXPAUCLIPBOARDDATA@@H@Z ; DestructElements

; 319  : 			delete[] (BYTE*)m_pData;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR $T75431[ebp], ecx
	mov	edx, DWORD PTR $T75431[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 320  : 			m_pData = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+4], 0
$L75015:

; 322  : 		m_nSize = m_nMaxSize = 0;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+12], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+8], 0

; 324  : 	else if (m_pData == NULL)

	jmp	$L75051
$L75014:
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L75023
$L75024:

; 326  : 		// create one with exact size
; 327  : #ifdef SIZE_T_MAX
; 328  : 		ASSERT(nNewSize <= SIZE_T_MAX/sizeof(TYPE));    // no overflow

	cmp	DWORD PTR _nNewSize$[ebp], 536870911	; 1fffffffH
	jbe	SHORT $L75028
	push	328					; 00000148H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75028
	int	3
$L75028:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75024

; 329  : #endif
; 330  : 		m_pData = (TYPE*) new BYTE[nNewSize * sizeof(TYPE)];

	push	330					; 0000014aH
	push	OFFSET FLAT:__szAfxTempl
	mov	edx, DWORD PTR _nNewSize$[ebp]
	shl	edx, 3
	push	edx
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T75432[ebp], eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR $T75432[ebp]
	mov	DWORD PTR [eax+4], ecx

; 331  : 		ConstructElements<TYPE>(m_pData, nNewSize);

	mov	edx, DWORD PTR _nNewSize$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	?ConstructElements@@YGXPAUCLIPBOARDDATA@@H@Z ; ConstructElements

; 332  : 		m_nSize = m_nMaxSize = nNewSize;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nNewSize$[ebp]
	mov	DWORD PTR [edx+12], eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nNewSize$[ebp]
	mov	DWORD PTR [ecx+8], edx

; 334  : 	else if (nNewSize <= m_nMaxSize)

	jmp	$L75051
$L75023:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nNewSize$[ebp]
	cmp	ecx, DWORD PTR [eax+12]
	jg	SHORT $L75039

; 336  : 		// it fits
; 337  : 		if (nNewSize > m_nSize)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nNewSize$[ebp]
	cmp	eax, DWORD PTR [edx+8]
	jle	SHORT $L75040

; 339  : 			// initialize the new elements
; 340  : 			ConstructElements<TYPE>(&m_pData[m_nSize], nNewSize-m_nSize);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nNewSize$[ebp]
	sub	edx, DWORD PTR [ecx+8]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	lea	ecx, DWORD PTR [eax+ecx*8]
	push	ecx
	call	?ConstructElements@@YGXPAUCLIPBOARDDATA@@H@Z ; ConstructElements

; 342  : 		else if (m_nSize > nNewSize)

	jmp	SHORT $L75046
$L75040:
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	cmp	eax, DWORD PTR _nNewSize$[ebp]
	jle	SHORT $L75046

; 344  : 			// destroy the old elements
; 345  : 			DestructElements<TYPE>(&m_pData[nNewSize], m_nSize-nNewSize);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	sub	edx, DWORD PTR _nNewSize$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _nNewSize$[ebp]
	lea	eax, DWORD PTR [ecx+edx*8]
	push	eax
	call	?DestructElements@@YGXPAUCLIPBOARDDATA@@H@Z ; DestructElements
$L75046:

; 347  : 		m_nSize = nNewSize;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nNewSize$[ebp]
	mov	DWORD PTR [ecx+8], edx

; 349  : 	else

	jmp	$L75051
$L75039:

; 351  : 		// otherwise, grow array
; 352  : 		int nGrowBy = m_nGrowBy;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR _nGrowBy$75052[ebp], ecx

; 353  : 		if (nGrowBy == 0)

	cmp	DWORD PTR _nGrowBy$75052[ebp], 0
	jne	SHORT $L75053

; 355  : 			// heuristically determine growth when nGrowBy == 0
; 356  : 			//  (this avoids heap fragmentation in many situations)
; 357  : 			nGrowBy = m_nSize / 8;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	cdq
	and	edx, 7
	add	eax, edx
	sar	eax, 3
	mov	DWORD PTR _nGrowBy$75052[ebp], eax

; 358  : 			nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);

	cmp	DWORD PTR _nGrowBy$75052[ebp], 4
	jge	SHORT $L75435
	mov	DWORD PTR -36+[ebp], 4
	jmp	SHORT $L75436
$L75435:
	cmp	DWORD PTR _nGrowBy$75052[ebp], 1024	; 00000400H
	jle	SHORT $L75433
	mov	DWORD PTR -40+[ebp], 1024		; 00000400H
	jmp	SHORT $L75434
$L75433:
	mov	eax, DWORD PTR _nGrowBy$75052[ebp]
	mov	DWORD PTR -40+[ebp], eax
$L75434:
	mov	ecx, DWORD PTR -40+[ebp]
	mov	DWORD PTR -36+[ebp], ecx
$L75436:
	mov	edx, DWORD PTR -36+[ebp]
	mov	DWORD PTR _nGrowBy$75052[ebp], edx
$L75053:

; 361  : 		if (nNewSize < m_nMaxSize + nGrowBy)

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	add	ecx, DWORD PTR _nGrowBy$75052[ebp]
	cmp	DWORD PTR _nNewSize$[ebp], ecx
	jge	SHORT $L75055

; 362  : 			nNewMax = m_nMaxSize + nGrowBy;  // granularity

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	add	eax, DWORD PTR _nGrowBy$75052[ebp]
	mov	DWORD PTR _nNewMax$75054[ebp], eax

; 363  : 		else

	jmp	SHORT $L75056
$L75055:

; 364  : 			nNewMax = nNewSize;  // no slush

	mov	ecx, DWORD PTR _nNewSize$[ebp]
	mov	DWORD PTR _nNewMax$75054[ebp], ecx
$L75056:

; 365  : 
; 366  : 		ASSERT(nNewMax >= m_nMaxSize);  // no wrap around

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nNewMax$75054[ebp]
	cmp	eax, DWORD PTR [edx+12]
	jge	SHORT $L75060
	push	366					; 0000016eH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75060
	int	3
$L75060:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75056
$L75059:

; 367  : #ifdef SIZE_T_MAX
; 368  : 		ASSERT(nNewMax <= SIZE_T_MAX/sizeof(TYPE)); // no overflow

	cmp	DWORD PTR _nNewMax$75054[ebp], 536870911 ; 1fffffffH
	jbe	SHORT $L75065
	push	368					; 00000170H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75065
	int	3
$L75065:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75059

; 369  : #endif
; 370  : 		TYPE* pNewData = (TYPE*) new BYTE[nNewMax * sizeof(TYPE)];

	push	370					; 00000172H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _nNewMax$75054[ebp]
	shl	eax, 3
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T75437[ebp], eax
	mov	ecx, DWORD PTR $T75437[ebp]
	mov	DWORD PTR _pNewData$75066[ebp], ecx

; 371  : 
; 372  : 		// copy new data from old
; 373  : 		memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	shl	eax, 3
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	mov	eax, DWORD PTR _pNewData$75066[ebp]
	push	eax
	call	_memcpy
	add	esp, 12					; 0000000cH
$L75072:

; 374  : 
; 375  : 		// construct remaining elements
; 376  : 		ASSERT(nNewSize > m_nSize);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nNewSize$[ebp]
	cmp	edx, DWORD PTR [ecx+8]
	jg	SHORT $L75075
	push	376					; 00000178H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75075
	int	3
$L75075:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75072

; 377  : 		ConstructElements<TYPE>(&pNewData[m_nSize], nNewSize-m_nSize);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nNewSize$[ebp]
	sub	edx, DWORD PTR [ecx+8]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	edx, DWORD PTR _pNewData$75066[ebp]
	lea	eax, DWORD PTR [edx+ecx*8]
	push	eax
	call	?ConstructElements@@YGXPAUCLIPBOARDDATA@@H@Z ; ConstructElements

; 378  : 
; 379  : 		// get rid of old stuff (note: no destructors called)
; 380  : 		delete[] (BYTE*)m_pData;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR $T75438[ebp], edx
	mov	eax, DWORD PTR $T75438[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 381  : 		m_pData = pNewData;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _pNewData$75066[ebp]
	mov	DWORD PTR [ecx+4], edx

; 382  : 		m_nSize = nNewSize;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nNewSize$[ebp]
	mov	DWORD PTR [eax+8], ecx

; 383  : 		m_nMaxSize = nNewMax;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nNewMax$75054[ebp]
	mov	DWORD PTR [edx+12], eax
$L75051:

; 385  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SetSize@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXHH@Z ENDP	; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::SetSize
_TEXT	ENDS
PUBLIC	??0CLIPBOARDDATA@@QAE@ABU0@@Z			; CLIPBOARDDATA::CLIPBOARDDATA
;	COMDAT ?GetAt@?$CArray@UCLIPBOARDDATA@@AAU1@@@QBE?AUCLIPBOARDDATA@@H@Z
_TEXT	SEGMENT
_nIndex$ = 12
___$ReturnUdt$ = 8
_this$ = -8
$T75442 = -4
?GetAt@?$CArray@UCLIPBOARDDATA@@AAU1@@@QBE?AUCLIPBOARDDATA@@H@Z PROC NEAR ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::GetAt, COMDAT

; 254  : 	{ ASSERT(nIndex >= 0 && nIndex < m_nSize);

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T75442[ebp], 0
$L75087:
	cmp	DWORD PTR _nIndex$[ebp], 0
	jl	SHORT $L75091
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nIndex$[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jl	SHORT $L75090
$L75091:
	push	254					; 000000feH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75090
	int	3
$L75090:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75087

; 255  : 		return m_pData[nIndex]; }

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _nIndex$[ebp]
	lea	eax, DWORD PTR [ecx+edx*8]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CLIPBOARDDATA@@QAE@ABU0@@Z		; CLIPBOARDDATA::CLIPBOARDDATA
	mov	ecx, DWORD PTR $T75442[ebp]
	or	ecx, 1
	mov	DWORD PTR $T75442[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?GetAt@?$CArray@UCLIPBOARDDATA@@AAU1@@@QBE?AUCLIPBOARDDATA@@H@Z ENDP ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::GetAt
_TEXT	ENDS
PUBLIC	??4CLIPBOARDDATA@@QAEAAU0@ABU0@@Z		; CLIPBOARDDATA::operator=
;	COMDAT ?SetAtGrow@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXHAAUCLIPBOARDDATA@@@Z
_TEXT	SEGMENT
_nIndex$ = 8
_newElement$ = 12
_this$ = -4
?SetAtGrow@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXHAAUCLIPBOARDDATA@@@Z PROC NEAR ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::SetAtGrow, COMDAT

; 437  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 438  : 	ASSERT_VALID(this);

	push	438					; 000001b6H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject
$L75097:

; 439  : 	ASSERT(nIndex >= 0);

	cmp	DWORD PTR _nIndex$[ebp], 0
	jge	SHORT $L75100
	push	439					; 000001b7H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75100
	int	3
$L75100:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75097

; 440  : 
; 441  : 	if (nIndex >= m_nSize)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nIndex$[ebp]
	cmp	eax, DWORD PTR [edx+8]
	jl	SHORT $L75101

; 442  : 		SetSize(nIndex+1, -1);

	push	-1
	mov	ecx, DWORD PTR _nIndex$[ebp]
	add	ecx, 1
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetSize@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXHH@Z ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::SetSize
$L75101:

; 443  : 	m_pData[nIndex] = newElement;

	mov	edx, DWORD PTR _newElement$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _nIndex$[ebp]
	lea	ecx, DWORD PTR [ecx+edx*8]
	call	??4CLIPBOARDDATA@@QAEAAU0@ABU0@@Z	; CLIPBOARDDATA::operator=

; 444  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SetAtGrow@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXHAAUCLIPBOARDDATA@@@Z ENDP ; CArray<CLIPBOARDDATA,CLIPBOARDDATA &>::SetAtGrow
_TEXT	ENDS
PUBLIC	??_GCLIPBOARDDATA@@QAEPAXI@Z			; CLIPBOARDDATA::`scalar deleting destructor'
EXTRN	?AfxIsValidAddress@@YGHPBXIH@Z:NEAR		; AfxIsValidAddress
;	COMDAT ?DestructElements@@YGXPAUCLIPBOARDDATA@@H@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
?DestructElements@@YGXPAUCLIPBOARDDATA@@H@Z PROC NEAR	; DestructElements, COMDAT

; 67   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L75103:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L75107
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 3
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L75107
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75107
	int	3
$L75107:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75103

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L75108
$L75109:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 8
	mov	DWORD PTR _pElements$[ebp], eax
$L75108:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L75110

; 73   : 		pElements->~TYPE();

	push	0
	mov	ecx, DWORD PTR _pElements$[ebp]
	call	??_GCLIPBOARDDATA@@QAEPAXI@Z		; CLIPBOARDDATA::`scalar deleting destructor'
	jmp	SHORT $L75109
$L75110:

; 74   : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?DestructElements@@YGXPAUCLIPBOARDDATA@@H@Z ENDP	; DestructElements
_TEXT	ENDS
;	COMDAT ??0CLIPBOARDDATA@@QAE@ABU0@@Z
_TEXT	SEGMENT
___that$ = 8
_this$ = -4
??0CLIPBOARDDATA@@QAE@ABU0@@Z PROC NEAR			; CLIPBOARDDATA::CLIPBOARDDATA, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR ___that$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR [eax], edx
	mov	eax, DWORD PTR ___that$[ebp]
	add	eax, 4
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0CLIPBOARDDATA@@QAE@ABU0@@Z ENDP			; CLIPBOARDDATA::CLIPBOARDDATA
_TEXT	ENDS
;	COMDAT ??4CLIPBOARDDATA@@QAEAAU0@ABU0@@Z
_TEXT	SEGMENT
___that$ = 8
_this$ = -4
??4CLIPBOARDDATA@@QAEAAU0@ABU0@@Z PROC NEAR		; CLIPBOARDDATA::operator=, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR ___that$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR [eax], edx
	mov	eax, DWORD PTR ___that$[ebp]
	add	eax, 4
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??4CLIPBOARDDATA@@QAEAAU0@ABU0@@Z ENDP			; CLIPBOARDDATA::operator=
_TEXT	ENDS
;	COMDAT ??_GCLIPBOARDDATA@@QAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCLIPBOARDDATA@@QAEPAXI@Z PROC NEAR			; CLIPBOARDDATA::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CLIPBOARDDATA@@QAE@XZ		; CLIPBOARDDATA::~CLIPBOARDDATA
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75138
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75138:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCLIPBOARDDATA@@QAEPAXI@Z ENDP			; CLIPBOARDDATA::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??2@YAPAXIPAX@Z					; operator new
PUBLIC	??3@YAXPAX0@Z					; operator delete
PUBLIC	??_C@_11A@?$AA?$AA@				; `string'
EXTRN	_memset:NEAR
;	COMDAT ??_C@_11A@?$AA?$AA@
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
_BSS	SEGMENT
??_C@_11A@?$AA?$AA@ DW 01H DUP (?)			; `string'
_BSS	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T75468	DD	019930520H
	DD	03H
	DD	FLAT:$T75470
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75470	DD	0ffffffffH
	DD	FLAT:$L75462
	DD	00H
	DD	FLAT:$L75464
	DD	0ffffffffH
	DD	FLAT:$L75464
xdata$x	ENDS
;	COMDAT ?ConstructElements@@YGXPAUCLIPBOARDDATA@@H@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
$T75457 = -16
$T75458 = -20
$T75459 = -24
$T75463 = -28
__$EHRec$ = -12
?ConstructElements@@YGXPAUCLIPBOARDDATA@@H@Z PROC NEAR	; ConstructElements, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75469
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
	mov	DWORD PTR $T75463[ebp], 0
$L75140:

; 54   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L75144
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 3
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L75144
	push	55					; 00000037H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75144
	int	3
$L75144:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75140

; 55   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 56   : 
; 57   : 	// first do bit-wise zero initialization
; 58   : 	memset((void*)pElements, 0, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 3
	push	eax
	push	0
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	_memset
	add	esp, 12					; 0000000cH

; 59   : 
; 60   : 	// then call the constructor(s)
; 61   : 	for (; nCount--; pElements++)

	jmp	SHORT $L75147
$L75148:
	mov	edx, DWORD PTR _pElements$[ebp]
	add	edx, 8
	mov	DWORD PTR _pElements$[ebp], edx
$L75147:
	mov	eax, DWORD PTR _nCount$[ebp]
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
	test	eax, eax
	je	SHORT $L75149

; 62   : 		::new((void*)pElements) TYPE;

	mov	edx, DWORD PTR _pElements$[ebp]
	push	edx
	push	8
	call	??2@YAPAXIPAX@Z				; operator new
	add	esp, 8
	mov	DWORD PTR $T75458[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	cmp	DWORD PTR $T75458[ebp], 0
	je	SHORT $L75460
	push	OFFSET FLAT:??_C@_11A@?$AA?$AA@		; `string'
	lea	ecx, DWORD PTR $T75459[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	eax, DWORD PTR $T75463[ebp]
	or	al, 1
	mov	DWORD PTR $T75463[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	push	0
	lea	ecx, DWORD PTR $T75459[ebp]
	push	ecx
	mov	ecx, DWORD PTR $T75458[ebp]
	call	??0CLIPBOARDDATA@@QAE@ABVCString@@I@Z	; CLIPBOARDDATA::CLIPBOARDDATA
	mov	DWORD PTR -32+[ebp], eax
	jmp	SHORT $L75461
$L75460:
	mov	DWORD PTR -32+[ebp], 0
$L75461:
	mov	edx, DWORD PTR -32+[ebp]
	mov	DWORD PTR $T75457[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR $T75463[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75466
	and	DWORD PTR $T75463[ebp], -2		; fffffffeH
	lea	ecx, DWORD PTR $T75459[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75466:
	jmp	$L75148
$L75149:

; 63   : }

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
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75462:
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR $T75458[ebp]
	push	ecx
	call	??3@YAXPAX0@Z				; operator delete
	add	esp, 8
	ret	0
$L75464:
	mov	eax, DWORD PTR $T75463[ebp]
	and	eax, 1
	test	eax, eax
	je	$L75465
	lea	ecx, DWORD PTR $T75459[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75465:
	ret	0
$L75469:
	mov	eax, OFFSET FLAT:$T75468
	jmp	___CxxFrameHandler
text$x	ENDS
?ConstructElements@@YGXPAUCLIPBOARDDATA@@H@Z ENDP	; ConstructElements
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
;	COMDAT ??3@YAXPAX0@Z
_TEXT	SEGMENT
??3@YAXPAX0@Z PROC NEAR					; operator delete, COMDAT

; 79   : 	{return; }

	push	ebp
	mov	ebp, esp
	pop	ebp
	ret	0
??3@YAXPAX0@Z ENDP					; operator delete
_TEXT	ENDS
END

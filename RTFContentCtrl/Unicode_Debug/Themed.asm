	TITLE	D:\_CODE\Shared\Themed.cpp
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
;	COMDAT ??0CThemed@@QAE@PAVCWnd@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCThemed@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CThemed@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InitUxTheme@CThemed@@KAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SupportsTheming@CThemed@@SAHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?BeginBufferedPaint@CThemed@@SAPAXPAUHDC__@@PBUtagRECT@@W4TH_BUFFERFORMAT@@PAUTH_PAINTPARAMS@@PAPAU2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetBufferedPaintBits@CThemed@@SAHPAXPAPAUtagRGBQUAD@@PAH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EndBufferedPaint@CThemed@@SAHPAXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Open@CThemed@@QAEHPBVCWnd@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Close@CThemed@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AreControlsThemed@CThemed@@SAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsNonClientThemed@CThemed@@SAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsWebContentThemed@CThemed@@SAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawFrameControl@CThemed@@SAHPBVCWnd@@PAVCDC@@PAUtagRECT@@IIPBU4@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawEdge@CThemed@@SAHPBVCWnd@@PAVCDC@@PAUtagRECT@@IIII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawBackground@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawParentBackground@CThemed@@QAEHPBVCWnd@@PAVCDC@@PAUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawTextW@CThemed@@QAEHPAVCDC@@HHABVCString@@KKABVCRect@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawEdge@CThemed@@QAEHPAVCDC@@HHABVCRect@@IIPAUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawIcon@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBVCImageList@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawBorder@CThemed@@QAEHPAVCDC@@HABVCRect@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSize@CThemed@@QAEHHHAAVCSize@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTextExtent@CThemed@@QAEHPAVCDC@@HHABVCString@@KAAVCRect@@PBUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?BuildImageList@CThemed@@QAEHAAVCImageList@@HQBHHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetThemeColor@CThemed@@QAEKHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetThemeBackgroundContentRect@CThemed@@QAEHPAVCDC@@HHABVCRect@@AAV3@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsThemeActive@CThemed@@SAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsAppThemed@CThemed@@SAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAppThemeProperties@CThemed@@KAKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OpenThemeData@CThemed@@IAEPAXPAUHWND__@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CloseThemeData@CThemed@@IAEHPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawThemeBackground@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@1@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawThemeParentBackground@CThemed@@IAEHPAUHWND__@@PAUHDC__@@PAUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawThemeText@CThemed@@IAEHPAUHDC__@@HHPBGHKKPBUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawThemeEdge@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@IIPAU3@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawThemeIcon@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@PAU_IMAGELIST@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawThemeBorder@CThemed@@IAEHPAUHDC__@@HPBUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetThemePartSize@CThemed@@IAEHHHW4THEMESIZE@@PAUtagSIZE@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetThemeTextExtent@CThemed@@IAEHPAUHDC__@@HHPBGHKPBUtagRECT@@PAU3@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetThemeColor@CThemed@@IAEHHHHPAK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetThemeBackgroundContentRect@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@PAU3@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetThemeClassPartState@CThemed@@KAHHHAAVCString@@AAH1@Z
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
;	COMDAT ??_7CThemed@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A		; CThemed::s_hUxTheme
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\Shared\Themed.cpp', 00H
	ORG $+1
?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A DD 0ffffffffH	; CThemed::s_hUxTheme
_DATA	ENDS
PUBLIC	??0CThemed@@QAE@PAVCWnd@@PBG@Z			; CThemed::CThemed
PUBLIC	?Open@CThemed@@QAEHPBVCWnd@@PBG@Z		; CThemed::Open
PUBLIC	?InitUxTheme@CThemed@@KAHXZ			; CThemed::InitUxTheme
PUBLIC	??_7CThemed@@6B@				; CThemed::`vftable'
PUBLIC	??_GCThemed@@UAEPAXI@Z				; CThemed::`scalar deleting destructor'
PUBLIC	??_ECThemed@@UAEPAXI@Z				; CThemed::`vector deleting destructor'
EXTRN	__chkesp:NEAR
EXTRN	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ:NEAR	; CWnd::GetSafeHwnd
;	COMDAT ??_7CThemed@@6B@
; File D:\_CODE\Shared\Themed.cpp
CONST	SEGMENT
??_7CThemed@@6B@ DD FLAT:??_ECThemed@@UAEPAXI@Z		; CThemed::`vftable'
CONST	ENDS
;	COMDAT ??0CThemed@@QAE@PAVCWnd@@PBG@Z
_TEXT	SEGMENT
_pWnd$ = 8
_szClassList$ = 12
_this$ = -4
??0CThemed@@QAE@PAVCWnd@@PBG@Z PROC NEAR		; CThemed::CThemed, COMDAT

; 65   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+4], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+8], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7CThemed@@6B@ ; CThemed::`vftable'

; 66   : 	InitUxTheme();

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme

; 67   : 	
; 68   : 	if (pWnd && pWnd->GetSafeHwnd())

	cmp	DWORD PTR _pWnd$[ebp], 0
	je	SHORT $L74598
	mov	ecx, DWORD PTR _pWnd$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	test	eax, eax
	je	SHORT $L74598

; 69   : 		Open(pWnd, szClassList);

	mov	eax, DWORD PTR _szClassList$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pWnd$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Open@CThemed@@QAEHPBVCWnd@@PBG@Z	; CThemed::Open
$L74598:

; 70   : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
??0CThemed@@QAE@PAVCWnd@@PBG@Z ENDP			; CThemed::CThemed
_TEXT	ENDS
PUBLIC	??1CThemed@@UAE@XZ				; CThemed::~CThemed
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCThemed@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCThemed@@UAEPAXI@Z PROC NEAR			; CThemed::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L74602
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L74602:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCThemed@@UAEPAXI@Z ENDP				; CThemed::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	?Close@CThemed@@QAEHXZ				; CThemed::Close
;	COMDAT ??1CThemed@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CThemed@@UAE@XZ PROC NEAR				; CThemed::~CThemed, COMDAT

; 73   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CThemed@@6B@ ; CThemed::`vftable'

; 74   : 	Close();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Close@CThemed@@QAEHXZ			; CThemed::Close

; 75   : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CThemed@@UAE@XZ ENDP					; CThemed::~CThemed
_TEXT	ENDS
EXTRN	__imp__LoadLibraryW@4:NEAR
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG74609 DB	'U', 00H, 'x', 00H, 'T', 00H, 'h', 00H, 'e', 00H, 'm', 00H
	DB	'e', 00H, '.', 00H, 'd', 00H, 'l', 00H, 'l', 00H, 00H, 00H
data	ENDS
;	COMDAT ?InitUxTheme@CThemed@@KAHXZ
_TEXT	SEGMENT
?InitUxTheme@CThemed@@KAHXZ PROC NEAR			; CThemed::InitUxTheme, COMDAT

; 78   : {

	push	ebp
	mov	ebp, esp
	push	esi

; 79   : 	if (s_hUxTheme == HMODULE(-1)) // one time only

	cmp	DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A, -1 ; CThemed::s_hUxTheme
	jne	SHORT $L74608

; 80   : 		s_hUxTheme = LoadLibrary(_T("UxTheme.dll"));

	mov	esi, esp
	push	OFFSET FLAT:$SG74609
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A, eax ; CThemed::s_hUxTheme
$L74608:

; 81   : 	
; 82   : 	return (NULL != s_hUxTheme);

	xor	eax, eax
	cmp	DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A, 0 ; CThemed::s_hUxTheme
	setne	al

; 83   : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?InitUxTheme@CThemed@@KAHXZ ENDP			; CThemed::InitUxTheme
_TEXT	ENDS
PUBLIC	?IsThemeActive@CThemed@@SAHXZ			; CThemed::IsThemeActive
PUBLIC	?SupportsTheming@CThemed@@SAHH@Z		; CThemed::SupportsTheming
PUBLIC	?GetAppThemeProperties@CThemed@@KAKXZ		; CThemed::GetAppThemeProperties
;	COMDAT ?SupportsTheming@CThemed@@SAHH@Z
_TEXT	SEGMENT
_nFlag$ = 8
?SupportsTheming@CThemed@@SAHH@Z PROC NEAR		; CThemed::SupportsTheming, COMDAT

; 86   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 87   : 	return (IsThemeActive() && (GetAppThemeProperties() & nFlag));

	call	?IsThemeActive@CThemed@@SAHXZ		; CThemed::IsThemeActive
	test	eax, eax
	je	SHORT $L75257
	call	?GetAppThemeProperties@CThemed@@KAKXZ	; CThemed::GetAppThemeProperties
	and	eax, DWORD PTR _nFlag$[ebp]
	test	eax, eax
	je	SHORT $L75257
	mov	DWORD PTR -4+[ebp], 1
	jmp	SHORT $L75258
$L75257:
	mov	DWORD PTR -4+[ebp], 0
$L75258:
	mov	eax, DWORD PTR -4+[ebp]

; 88   : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?SupportsTheming@CThemed@@SAHH@Z ENDP			; CThemed::SupportsTheming
_TEXT	ENDS
PUBLIC	?BeginBufferedPaint@CThemed@@SAPAXPAUHDC__@@PBUtagRECT@@W4TH_BUFFERFORMAT@@PAUTH_PAINTPARAMS@@PAPAU2@@Z ; CThemed::BeginBufferedPaint
EXTRN	__imp__GetProcAddress@8:NEAR
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG74623 DB	'BeginBufferedPaint', 00H
data	ENDS
;	COMDAT ?BeginBufferedPaint@CThemed@@SAPAXPAUHDC__@@PBUtagRECT@@W4TH_BUFFERFORMAT@@PAUTH_PAINTPARAMS@@PAPAU2@@Z
_TEXT	SEGMENT
_hdcTarget$ = 8
_rTarget$ = 12
_dwFormat$ = 16
_pParams$ = 20
_hdcBuffered$ = 24
_fnBeginBufferedPaint$74621 = -4
?BeginBufferedPaint@CThemed@@SAPAXPAUHDC__@@PBUtagRECT@@W4TH_BUFFERFORMAT@@PAUTH_PAINTPARAMS@@PAPAU2@@Z PROC NEAR ; CThemed::BeginBufferedPaint, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 92   : 	if (InitUxTheme())

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L74624

; 94   : 		PFNBEGINBUFFEREDPAINT fnBeginBufferedPaint = (PFNBEGINBUFFEREDPAINT)GetProcAddress(s_hUxTheme, "BeginBufferedPaint");

	mov	esi, esp
	push	OFFSET FLAT:$SG74623
	mov	eax, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	eax
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnBeginBufferedPaint$74621[ebp], eax

; 95   : 		
; 96   : 		if (fnBeginBufferedPaint)

	cmp	DWORD PTR _fnBeginBufferedPaint$74621[ebp], 0
	je	SHORT $L74624

; 97   : 			return (HPAINTBUFFER)fnBeginBufferedPaint(hdcTarget, rTarget, dwFormat, pParams, hdcBuffered);

	mov	esi, esp
	mov	ecx, DWORD PTR _hdcBuffered$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pParams$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwFormat$[ebp]
	push	eax
	mov	ecx, DWORD PTR _rTarget$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hdcTarget$[ebp]
	push	edx
	call	DWORD PTR _fnBeginBufferedPaint$74621[ebp]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L74619
$L74624:

; 99   : 	
; 100  : 	return NULL;

	xor	eax, eax
$L74619:

; 101  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?BeginBufferedPaint@CThemed@@SAPAXPAUHDC__@@PBUtagRECT@@W4TH_BUFFERFORMAT@@PAUTH_PAINTPARAMS@@PAPAU2@@Z ENDP ; CThemed::BeginBufferedPaint
_TEXT	ENDS
PUBLIC	?GetBufferedPaintBits@CThemed@@SAHPAXPAPAUtagRGBQUAD@@PAH@Z ; CThemed::GetBufferedPaintBits
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG74634 DB	'GetBufferedPaintBits', 00H
data	ENDS
;	COMDAT ?GetBufferedPaintBits@CThemed@@SAHPAXPAPAUtagRGBQUAD@@PAH@Z
_TEXT	SEGMENT
_pb$ = 8
_pBits$ = 12
_nBitsCount$ = 16
_fnGetBufferedPaintBits$74632 = -4
?GetBufferedPaintBits@CThemed@@SAHPAXPAPAUtagRGBQUAD@@PAH@Z PROC NEAR ; CThemed::GetBufferedPaintBits, COMDAT

; 104  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 105  : 	if (InitUxTheme())

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L74635

; 107  : 		PFNGETBUFFEREDPAINTBITS fnGetBufferedPaintBits = (PFNGETBUFFEREDPAINTBITS)GetProcAddress(s_hUxTheme, "GetBufferedPaintBits");

	mov	esi, esp
	push	OFFSET FLAT:$SG74634
	mov	eax, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	eax
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnGetBufferedPaintBits$74632[ebp], eax

; 108  : 		
; 109  : 		if (fnGetBufferedPaintBits)

	cmp	DWORD PTR _fnGetBufferedPaintBits$74632[ebp], 0
	je	SHORT $L74635

; 110  : 			return (SUCCEEDED(fnGetBufferedPaintBits(pb, pBits, nBitsCount)));

	mov	esi, esp
	mov	ecx, DWORD PTR _nBitsCount$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pBits$[ebp]
	push	edx
	mov	eax, DWORD PTR _pb$[ebp]
	push	eax
	call	DWORD PTR _fnGetBufferedPaintBits$74632[ebp]
	cmp	esi, esp
	call	__chkesp
	xor	ecx, ecx
	test	eax, eax
	setge	cl
	mov	eax, ecx
	jmp	SHORT $L74630
$L74635:

; 112  : 	
; 113  : 	return FALSE;

	xor	eax, eax
$L74630:

; 114  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetBufferedPaintBits@CThemed@@SAHPAXPAPAUtagRGBQUAD@@PAH@Z ENDP ; CThemed::GetBufferedPaintBits
_TEXT	ENDS
PUBLIC	?EndBufferedPaint@CThemed@@SAHPAXH@Z		; CThemed::EndBufferedPaint
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG74644 DB	'EndBufferedPaint', 00H
data	ENDS
;	COMDAT ?EndBufferedPaint@CThemed@@SAHPAXH@Z
_TEXT	SEGMENT
_pb$ = 8
_bUpdatetarget$ = 12
_fnEndBufferedPaint$74642 = -4
?EndBufferedPaint@CThemed@@SAHPAXH@Z PROC NEAR		; CThemed::EndBufferedPaint, COMDAT

; 117  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 118  : 	if (InitUxTheme())

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L74645

; 120  : 		PFNENDBUFFEREDPAINT fnEndBufferedPaint = (PFNENDBUFFEREDPAINT)GetProcAddress(s_hUxTheme, "EndBufferedPaint");

	mov	esi, esp
	push	OFFSET FLAT:$SG74644
	mov	eax, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	eax
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnEndBufferedPaint$74642[ebp], eax

; 121  : 		
; 122  : 		if (fnEndBufferedPaint)

	cmp	DWORD PTR _fnEndBufferedPaint$74642[ebp], 0
	je	SHORT $L74645

; 123  : 			return (SUCCEEDED(fnEndBufferedPaint(pb, bUpdatetarget)));

	mov	esi, esp
	mov	ecx, DWORD PTR _bUpdatetarget$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pb$[ebp]
	push	edx
	call	DWORD PTR _fnEndBufferedPaint$74642[ebp]
	cmp	esi, esp
	call	__chkesp
	xor	ecx, ecx
	test	eax, eax
	setge	cl
	mov	eax, ecx
	jmp	SHORT $L74640
$L74645:

; 125  : 	
; 126  : 	return FALSE;

	xor	eax, eax
$L74640:

; 127  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?EndBufferedPaint@CThemed@@SAHPAXH@Z ENDP		; CThemed::EndBufferedPaint
_TEXT	ENDS
PUBLIC	?OpenThemeData@CThemed@@IAEPAXPAUHWND__@@PBG@Z	; CThemed::OpenThemeData
EXTRN	__imp__GetClassNameW@12:NEAR
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	?GetBuffer@CString@@QAEPAGH@Z:NEAR		; CString::GetBuffer
EXTRN	?ReleaseBuffer@CString@@QAEXH@Z:NEAR		; CString::ReleaseBuffer
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG74654 DB	'U', 00H, 'x', 00H, 'T', 00H, 'h', 00H, 'e', 00H, 'm', 00H
	DB	'e', 00H, '.', 00H, 'd', 00H, 'l', 00H, 'l', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T75275	DD	019930520H
	DD	01H
	DD	FLAT:$T75278
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75278	DD	0ffffffffH
	DD	FLAT:$L75271
xdata$x	ENDS
;	COMDAT ?Open@CThemed@@QAEHPBVCWnd@@PBG@Z
_TEXT	SEGMENT
$T75270 = -20
__$EHRec$ = -12
_pWnd$ = 8
_szClassList$ = 12
_this$ = -24
_sClassList$74657 = -16
?Open@CThemed@@QAEHPBVCWnd@@PBG@Z PROC NEAR		; CThemed::Open, COMDAT

; 130  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75276
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 131  : 	if (pWnd && pWnd->GetSafeHwnd())

	cmp	DWORD PTR _pWnd$[ebp], 0
	je	$L74652
	mov	ecx, DWORD PTR _pWnd$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	test	eax, eax
	je	$L74652

; 133  : 		if (!s_hUxTheme)

	cmp	DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A, 0 ; CThemed::s_hUxTheme
	jne	SHORT $L74653

; 135  : 			s_hUxTheme = LoadLibrary(_T("UxTheme.dll"));

	mov	esi, esp
	push	OFFSET FLAT:$SG74654
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A, eax ; CThemed::s_hUxTheme

; 136  : 
; 137  : 			if (!s_hUxTheme)

	cmp	DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A, 0 ; CThemed::s_hUxTheme
	jne	SHORT $L74655

; 138  : 				return FALSE;

	xor	eax, eax
	jmp	$L74651
$L74655:

; 140  : 		else

	jmp	SHORT $L74656
$L74653:

; 141  : 			Close();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Close@CThemed@@QAEHXZ			; CThemed::Close
$L74656:

; 142  : 		
; 143  : 		CString sClassList(szClassList);

	mov	eax, DWORD PTR _szClassList$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sClassList$74657[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 144  : 		
; 145  : 		if (sClassList.IsEmpty())

	lea	ecx, DWORD PTR _sClassList$74657[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L74658

; 147  : 			::GetClassName(pWnd->GetSafeHwnd(), sClassList.GetBuffer(128), 128);

	mov	esi, esp
	push	128					; 00000080H
	push	128					; 00000080H
	lea	ecx, DWORD PTR _sClassList$74657[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	mov	ecx, DWORD PTR _pWnd$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	push	eax
	call	DWORD PTR __imp__GetClassNameW@12
	cmp	esi, esp
	call	__chkesp

; 148  : 			sClassList.ReleaseBuffer();

	push	-1
	lea	ecx, DWORD PTR _sClassList$74657[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer
$L74658:

; 150  : 		
; 151  : 		m_hTheme = OpenThemeData(pWnd->GetSafeHwnd(), WSTR(sClassList));

	lea	ecx, DWORD PTR _sClassList$74657[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _pWnd$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?OpenThemeData@CThemed@@IAEPAXPAUHWND__@@PBG@Z ; CThemed::OpenThemeData
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+4], eax

; 152  : 		m_hWnd = pWnd->GetSafeHwnd();

	mov	ecx, DWORD PTR _pWnd$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+8], eax

; 153  : 		
; 154  : 		return (NULL != m_hTheme);

	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	cmp	DWORD PTR [eax+4], 0
	setne	cl
	mov	DWORD PTR $T75270[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sClassList$74657[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T75270[ebp]
	jmp	SHORT $L74651
$L74652:

; 156  : 	
; 157  : 	return FALSE;

	xor	eax, eax
$L74651:

; 158  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75271:
	lea	ecx, DWORD PTR _sClassList$74657[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75276:
	mov	eax, OFFSET FLAT:$T75275
	jmp	___CxxFrameHandler
text$x	ENDS
?Open@CThemed@@QAEHPBVCWnd@@PBG@Z ENDP			; CThemed::Open
PUBLIC	?CloseThemeData@CThemed@@IAEHPAX@Z		; CThemed::CloseThemeData
;	COMDAT ?Close@CThemed@@QAEHXZ
_TEXT	SEGMENT
_this$ = -4
?Close@CThemed@@QAEHXZ PROC NEAR			; CThemed::Close, COMDAT

; 161  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 162  : 	if (CloseThemeData(m_hTheme))

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?CloseThemeData@CThemed@@IAEHPAX@Z	; CThemed::CloseThemeData
	test	eax, eax
	je	SHORT $L74663

; 163  : 		m_hTheme = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0
$L74663:

; 164  : 	
; 165  : 	return (NULL == m_hTheme);

	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	cmp	DWORD PTR [eax+4], 0
	sete	cl
	mov	eax, ecx

; 166  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Close@CThemed@@QAEHXZ ENDP				; CThemed::Close
_TEXT	ENDS
PUBLIC	?AreControlsThemed@CThemed@@SAHXZ		; CThemed::AreControlsThemed
;	COMDAT ?AreControlsThemed@CThemed@@SAHXZ
_TEXT	SEGMENT
?AreControlsThemed@CThemed@@SAHXZ PROC NEAR		; CThemed::AreControlsThemed, COMDAT

; 169  : {

	push	ebp
	mov	ebp, esp

; 170  : 	return SupportsTheming(STAP_ALLOW_CONTROLS);

	push	2
	call	?SupportsTheming@CThemed@@SAHH@Z	; CThemed::SupportsTheming
	add	esp, 4

; 171  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?AreControlsThemed@CThemed@@SAHXZ ENDP			; CThemed::AreControlsThemed
_TEXT	ENDS
PUBLIC	?IsNonClientThemed@CThemed@@SAHXZ		; CThemed::IsNonClientThemed
;	COMDAT ?IsNonClientThemed@CThemed@@SAHXZ
_TEXT	SEGMENT
?IsNonClientThemed@CThemed@@SAHXZ PROC NEAR		; CThemed::IsNonClientThemed, COMDAT

; 174  : {

	push	ebp
	mov	ebp, esp

; 175  : 	return SupportsTheming(STAP_ALLOW_NONCLIENT);

	push	1
	call	?SupportsTheming@CThemed@@SAHH@Z	; CThemed::SupportsTheming
	add	esp, 4

; 176  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?IsNonClientThemed@CThemed@@SAHXZ ENDP			; CThemed::IsNonClientThemed
_TEXT	ENDS
PUBLIC	?IsWebContentThemed@CThemed@@SAHXZ		; CThemed::IsWebContentThemed
;	COMDAT ?IsWebContentThemed@CThemed@@SAHXZ
_TEXT	SEGMENT
?IsWebContentThemed@CThemed@@SAHXZ PROC NEAR		; CThemed::IsWebContentThemed, COMDAT

; 179  : {

	push	ebp
	mov	ebp, esp

; 180  : 	return SupportsTheming(STAP_ALLOW_WEBCONTENT);

	push	4
	call	?SupportsTheming@CThemed@@SAHH@Z	; CThemed::SupportsTheming
	add	esp, 4

; 181  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?IsWebContentThemed@CThemed@@SAHXZ ENDP			; CThemed::IsWebContentThemed
_TEXT	ENDS
PUBLIC	?DrawFrameControl@CThemed@@SAHPBVCWnd@@PAVCDC@@PAUtagRECT@@IIPBU4@@Z ; CThemed::DrawFrameControl
PUBLIC	?DrawParentBackground@CThemed@@QAEHPBVCWnd@@PAVCDC@@PAUtagRECT@@@Z ; CThemed::DrawParentBackground
PUBLIC	?DrawBackground@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBUtagRECT@@@Z ; CThemed::DrawBackground
PUBLIC	?GetThemeClassPartState@CThemed@@KAHHHAAVCString@@AAH1@Z ; CThemed::GetThemeClassPartState
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	??0CRect@@QAE@PBUtagRECT@@@Z:NEAR		; CRect::CRect
EXTRN	?DrawFrameControl@CDC@@QAEHPAUtagRECT@@II@Z:NEAR ; CDC::DrawFrameControl
;	COMDAT xdata$x
; File D:\_CODE\Shared\Themed.cpp
xdata$x	SEGMENT
$T75303	DD	019930520H
	DD	02H
	DD	FLAT:$T75305
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75305	DD	0ffffffffH
	DD	FLAT:$L75300
	DD	00H
	DD	FLAT:$L75301
xdata$x	ENDS
;	COMDAT ?DrawFrameControl@CThemed@@SAHPBVCWnd@@PAVCDC@@PAUtagRECT@@IIPBU4@@Z
_TEXT	SEGMENT
$T75293 = -40
$T75294 = -44
$T75297 = -60
$T75298 = -64
$T75299 = -68
__$EHRec$ = -12
_pWnd$ = 8
_pDC$ = 12
_pRect$ = 16
_nType$ = 20
_nState$ = 24
_pClip$ = 28
_th$ = -24
_sThClass$74680 = -36
_nThPart$74681 = -32
_nThState$74682 = -28
?DrawFrameControl@CThemed@@SAHPBVCWnd@@PAVCDC@@PAUtagRECT@@IIPBU4@@Z PROC NEAR ; CThemed::DrawFrameControl, COMDAT

; 185  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75304
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 60					; 0000003cH
	push	edi
	lea	edi, DWORD PTR [ebp-72]
	mov	ecx, 15					; 0000000fH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 186  : 	CThemed th;

	push	0
	push	0
	lea	ecx, DWORD PTR _th$[ebp]
	call	??0CThemed@@QAE@PAVCWnd@@PBG@Z		; CThemed::CThemed
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 187  : 	
; 188  : 	if (th.SupportsTheming(STAP_ALLOW_CONTROLS) && th.SupportsTheming(STAP_ALLOW_NONCLIENT))

	push	2
	call	?SupportsTheming@CThemed@@SAHH@Z	; CThemed::SupportsTheming
	add	esp, 4
	test	eax, eax
	je	$L74679
	push	1
	call	?SupportsTheming@CThemed@@SAHH@Z	; CThemed::SupportsTheming
	add	esp, 4
	test	eax, eax
	je	$L74679

; 190  : 		// determine the appropriate class, part and state
; 191  : 		CString sThClass;

	lea	ecx, DWORD PTR _sThClass$74680[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 192  : 		int nThPart = 0, nThState = 0;

	mov	DWORD PTR _nThPart$74681[ebp], 0
	mov	DWORD PTR _nThState$74682[ebp], 0

; 193  : 		
; 194  : 		if (!th.GetThemeClassPartState(nType, nState, sThClass, nThPart, nThState))

	lea	eax, DWORD PTR _nThState$74682[ebp]
	push	eax
	lea	ecx, DWORD PTR _nThPart$74681[ebp]
	push	ecx
	lea	edx, DWORD PTR _sThClass$74680[ebp]
	push	edx
	mov	eax, DWORD PTR _nState$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nType$[ebp]
	push	ecx
	call	?GetThemeClassPartState@CThemed@@KAHHHAAVCString@@AAH1@Z ; CThemed::GetThemeClassPartState
	add	esp, 20					; 00000014H
	test	eax, eax
	jne	SHORT $L74683

; 195  : 			return FALSE;

	mov	DWORD PTR $T75293[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sThClass$74680[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _th$[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	mov	eax, DWORD PTR $T75293[ebp]
	jmp	$L74677
$L74683:

; 196  : 		
; 197  : 		if (!th.Open(pWnd, sThClass))

	lea	ecx, DWORD PTR _sThClass$74680[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	edx, DWORD PTR _pWnd$[ebp]
	push	edx
	lea	ecx, DWORD PTR _th$[ebp]
	call	?Open@CThemed@@QAEHPBVCWnd@@PBG@Z	; CThemed::Open
	test	eax, eax
	jne	SHORT $L74685

; 198  : 			return FALSE;

	mov	DWORD PTR $T75294[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sThClass$74680[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _th$[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	mov	eax, DWORD PTR $T75294[ebp]
	jmp	$L74677
$L74685:

; 199  : 		
; 200  : 		th.DrawParentBackground(pWnd, pDC, (LPRECT)(pClip ? pClip : pRect));

	cmp	DWORD PTR _pClip$[ebp], 0
	je	SHORT $L75295
	mov	eax, DWORD PTR _pClip$[ebp]
	mov	DWORD PTR -72+[ebp], eax
	jmp	SHORT $L75296
$L75295:
	mov	ecx, DWORD PTR _pRect$[ebp]
	mov	DWORD PTR -72+[ebp], ecx
$L75296:
	mov	edx, DWORD PTR -72+[ebp]
	push	edx
	mov	eax, DWORD PTR _pDC$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pWnd$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _th$[ebp]
	call	?DrawParentBackground@CThemed@@QAEHPBVCWnd@@PAVCDC@@PAUtagRECT@@@Z ; CThemed::DrawParentBackground

; 201  : 		th.DrawBackground(pDC, nThPart, nThState, pRect, pClip);

	mov	edx, DWORD PTR _pRect$[ebp]
	push	edx
	lea	ecx, DWORD PTR $T75297[ebp]
	call	??0CRect@@QAE@PBUtagRECT@@@Z		; CRect::CRect
	mov	eax, DWORD PTR _pClip$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T75297[ebp]
	push	ecx
	mov	edx, DWORD PTR _nThState$74682[ebp]
	push	edx
	mov	eax, DWORD PTR _nThPart$74681[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _th$[ebp]
	call	?DrawBackground@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBUtagRECT@@@Z ; CThemed::DrawBackground

; 202  : 		
; 203  : 		return TRUE;

	mov	DWORD PTR $T75298[ebp], 1
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sThClass$74680[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _th$[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	mov	eax, DWORD PTR $T75298[ebp]
	jmp	SHORT $L74677
$L74679:

; 206  : 		return pDC->DrawFrameControl(pRect, nType, nState);

	mov	edx, DWORD PTR _nState$[ebp]
	push	edx
	mov	eax, DWORD PTR _nType$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pRect$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?DrawFrameControl@CDC@@QAEHPAUtagRECT@@II@Z ; CDC::DrawFrameControl
	mov	DWORD PTR $T75299[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _th$[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	mov	eax, DWORD PTR $T75299[ebp]
$L74677:

; 207  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 72					; 00000048H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75300:
	lea	ecx, DWORD PTR _th$[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	ret	0
$L75301:
	lea	ecx, DWORD PTR _sThClass$74680[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75304:
	mov	eax, OFFSET FLAT:$T75303
	jmp	___CxxFrameHandler
text$x	ENDS
?DrawFrameControl@CThemed@@SAHPBVCWnd@@PAVCDC@@PAUtagRECT@@IIPBU4@@Z ENDP ; CThemed::DrawFrameControl
PUBLIC	?DrawEdge@CThemed@@SAHPBVCWnd@@PAVCDC@@PAUtagRECT@@IIII@Z ; CThemed::DrawEdge
PUBLIC	?DrawEdge@CThemed@@QAEHPAVCDC@@HHABVCRect@@IIPAUtagRECT@@@Z ; CThemed::DrawEdge
;	COMDAT xdata$x
; File D:\_CODE\Shared\Themed.cpp
xdata$x	SEGMENT
$T75318	DD	019930520H
	DD	02H
	DD	FLAT:$T75320
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75320	DD	0ffffffffH
	DD	FLAT:$L75315
	DD	00H
	DD	FLAT:$L75316
xdata$x	ENDS
;	COMDAT ?DrawEdge@CThemed@@SAHPBVCWnd@@PAVCDC@@PAUtagRECT@@IIII@Z
_TEXT	SEGMENT
$T75310 = -40
$T75311 = -44
$T75312 = -60
$T75313 = -64
$T75314 = -68
__$EHRec$ = -12
_pWnd$ = 8
_pDC$ = 12
_pRect$ = 16
_nType$ = 20
_nState$ = 24
_nEdge$ = 28
_nFlags$ = 32
_th$ = -24
_sThClass$74703 = -36
_nThPart$74704 = -32
_nThState$74705 = -28
?DrawEdge@CThemed@@SAHPBVCWnd@@PAVCDC@@PAUtagRECT@@IIII@Z PROC NEAR ; CThemed::DrawEdge, COMDAT

; 210  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75319
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 56					; 00000038H
	push	edi
	lea	edi, DWORD PTR [ebp-68]
	mov	ecx, 14					; 0000000eH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 211  : 	CThemed th;

	push	0
	push	0
	lea	ecx, DWORD PTR _th$[ebp]
	call	??0CThemed@@QAE@PAVCWnd@@PBG@Z		; CThemed::CThemed
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 212  : 	
; 213  : 	if (th.SupportsTheming(STAP_ALLOW_CONTROLS) && th.SupportsTheming(STAP_ALLOW_NONCLIENT))

	push	2
	call	?SupportsTheming@CThemed@@SAHH@Z	; CThemed::SupportsTheming
	add	esp, 4
	test	eax, eax
	je	$L74702
	push	1
	call	?SupportsTheming@CThemed@@SAHH@Z	; CThemed::SupportsTheming
	add	esp, 4
	test	eax, eax
	je	$L74702

; 215  : 		// determine the appropriate class, part and state
; 216  : 		CString sThClass;

	lea	ecx, DWORD PTR _sThClass$74703[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 217  : 		int nThPart = 0, nThState = 0;

	mov	DWORD PTR _nThPart$74704[ebp], 0
	mov	DWORD PTR _nThState$74705[ebp], 0

; 218  : 		
; 219  : 		if (!th.GetThemeClassPartState(nType, nState, sThClass, nThPart, nThState))

	lea	eax, DWORD PTR _nThState$74705[ebp]
	push	eax
	lea	ecx, DWORD PTR _nThPart$74704[ebp]
	push	ecx
	lea	edx, DWORD PTR _sThClass$74703[ebp]
	push	edx
	mov	eax, DWORD PTR _nState$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nType$[ebp]
	push	ecx
	call	?GetThemeClassPartState@CThemed@@KAHHHAAVCString@@AAH1@Z ; CThemed::GetThemeClassPartState
	add	esp, 20					; 00000014H
	test	eax, eax
	jne	SHORT $L74706

; 220  : 			return FALSE;

	mov	DWORD PTR $T75310[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sThClass$74703[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _th$[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	mov	eax, DWORD PTR $T75310[ebp]
	jmp	$L74700
$L74706:

; 221  : 		
; 222  : 		if (!th.Open(pWnd, sThClass))

	lea	ecx, DWORD PTR _sThClass$74703[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	edx, DWORD PTR _pWnd$[ebp]
	push	edx
	lea	ecx, DWORD PTR _th$[ebp]
	call	?Open@CThemed@@QAEHPBVCWnd@@PBG@Z	; CThemed::Open
	test	eax, eax
	jne	SHORT $L74708

; 223  : 			return FALSE;

	mov	DWORD PTR $T75311[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sThClass$74703[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _th$[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	mov	eax, DWORD PTR $T75311[ebp]
	jmp	SHORT $L74700
$L74708:

; 224  : 		
; 225  : 		th.DrawEdge(pDC, nThPart, nThState, pRect, nEdge, nFlags);

	mov	eax, DWORD PTR _pRect$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T75312[ebp]
	call	??0CRect@@QAE@PBUtagRECT@@@Z		; CRect::CRect
	push	0
	mov	ecx, DWORD PTR _nFlags$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nEdge$[ebp]
	push	edx
	lea	eax, DWORD PTR $T75312[ebp]
	push	eax
	mov	ecx, DWORD PTR _nThState$74705[ebp]
	push	ecx
	mov	edx, DWORD PTR _nThPart$74704[ebp]
	push	edx
	mov	eax, DWORD PTR _pDC$[ebp]
	push	eax
	lea	ecx, DWORD PTR _th$[ebp]
	call	?DrawEdge@CThemed@@QAEHPAVCDC@@HHABVCRect@@IIPAUtagRECT@@@Z ; CThemed::DrawEdge

; 226  : 		
; 227  : 		return TRUE;

	mov	DWORD PTR $T75313[ebp], 1
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sThClass$74703[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _th$[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	mov	eax, DWORD PTR $T75313[ebp]
	jmp	SHORT $L74700
$L74702:

; 230  : 		return pDC->DrawFrameControl(pRect, nType, nState);

	mov	ecx, DWORD PTR _nState$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nType$[ebp]
	push	edx
	mov	eax, DWORD PTR _pRect$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?DrawFrameControl@CDC@@QAEHPAUtagRECT@@II@Z ; CDC::DrawFrameControl
	mov	DWORD PTR $T75314[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _th$[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	mov	eax, DWORD PTR $T75314[ebp]
$L74700:

; 231  : }

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
$L75315:
	lea	ecx, DWORD PTR _th$[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	ret	0
$L75316:
	lea	ecx, DWORD PTR _sThClass$74703[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75319:
	mov	eax, OFFSET FLAT:$T75318
	jmp	___CxxFrameHandler
text$x	ENDS
?DrawEdge@CThemed@@SAHPBVCWnd@@PAVCDC@@PAUtagRECT@@IIII@Z ENDP ; CThemed::DrawEdge
PUBLIC	?DrawThemeBackground@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@1@Z ; CThemed::DrawThemeBackground
EXTRN	??BCDC@@QBEPAUHDC__@@XZ:NEAR			; CDC::operator HDC__ *
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z:NEAR ; AfxAssertValidObject
EXTRN	??BCRect@@QBEPBUtagRECT@@XZ:NEAR		; CRect::operator tagRECT const *
;	COMDAT ?DrawBackground@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBUtagRECT@@@Z
_TEXT	SEGMENT
_pDC$ = 8
_nPart$ = 12
_nState$ = 16
_rect$ = 20
_prClip$ = 24
_this$ = -4
?DrawBackground@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBUtagRECT@@@Z PROC NEAR ; CThemed::DrawBackground, COMDAT

; 234  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74722:

; 235  : 	ASSERT (m_hTheme);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74725
	push	235					; 000000ebH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74725
	int	3
$L74725:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74722

; 236  : 	ASSERT_VALID (pDC);

	push	236					; 000000ecH
	push	OFFSET FLAT:_THIS_FILE
	mov	edx, DWORD PTR _pDC$[ebp]
	push	edx
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 237  : 	
; 238  : 	return DrawThemeBackground(*pDC, nPart, nState, rect, prClip);

	mov	eax, DWORD PTR _prClip$[ebp]
	push	eax
	mov	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QBEPBUtagRECT@@XZ		; CRect::operator tagRECT const *
	push	eax
	mov	ecx, DWORD PTR _nState$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nPart$[ebp]
	push	edx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	??BCDC@@QBEPAUHDC__@@XZ			; CDC::operator HDC__ *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DrawThemeBackground@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@1@Z ; CThemed::DrawThemeBackground

; 239  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	20					; 00000014H
?DrawBackground@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBUtagRECT@@@Z ENDP ; CThemed::DrawBackground
_TEXT	ENDS
PUBLIC	?DrawThemeParentBackground@CThemed@@IAEHPAUHWND__@@PAUHDC__@@PAUtagRECT@@@Z ; CThemed::DrawThemeParentBackground
EXTRN	??BCWnd@@QBEPAUHWND__@@XZ:NEAR			; CWnd::operator HWND__ *
;	COMDAT ?DrawParentBackground@CThemed@@QAEHPBVCWnd@@PAVCDC@@PAUtagRECT@@@Z
_TEXT	SEGMENT
_pWnd$ = 8
_pDC$ = 12
_pRect$ = 16
_this$ = -4
?DrawParentBackground@CThemed@@QAEHPBVCWnd@@PAVCDC@@PAUtagRECT@@@Z PROC NEAR ; CThemed::DrawParentBackground, COMDAT

; 242  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74732:

; 243  : 	ASSERT (m_hTheme);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74735
	push	243					; 000000f3H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74735
	int	3
$L74735:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74732

; 244  : 	ASSERT_VALID (pDC);

	push	244					; 000000f4H
	push	OFFSET FLAT:_THIS_FILE
	mov	edx, DWORD PTR _pDC$[ebp]
	push	edx
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 245  : 	ASSERT_VALID (pWnd);

	push	245					; 000000f5H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _pWnd$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 246  : 	
; 247  : 	return DrawThemeParentBackground(*pWnd, *pDC, pRect);

	mov	ecx, DWORD PTR _pRect$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	??BCDC@@QBEPAUHDC__@@XZ			; CDC::operator HDC__ *
	push	eax
	mov	ecx, DWORD PTR _pWnd$[ebp]
	call	??BCWnd@@QBEPAUHWND__@@XZ		; CWnd::operator HWND__ *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DrawThemeParentBackground@CThemed@@IAEHPAUHWND__@@PAUHDC__@@PAUtagRECT@@@Z ; CThemed::DrawThemeParentBackground

; 248  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?DrawParentBackground@CThemed@@QAEHPBVCWnd@@PAVCDC@@PAUtagRECT@@@Z ENDP ; CThemed::DrawParentBackground
_TEXT	ENDS
PUBLIC	?DrawTextW@CThemed@@QAEHPAVCDC@@HHABVCString@@KKABVCRect@@@Z ; CThemed::DrawTextW
PUBLIC	?DrawThemeText@CThemed@@IAEHPAUHDC__@@HHPBGHKKPBUtagRECT@@@Z ; CThemed::DrawThemeText
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
;	COMDAT ?DrawTextW@CThemed@@QAEHPAVCDC@@HHABVCString@@KKABVCRect@@@Z
_TEXT	SEGMENT
_pDC$ = 8
_nPart$ = 12
_nState$ = 16
_sText$ = 20
_dwTextFlags$ = 24
_dwTextFlags2$ = 28
_rect$ = 32
_this$ = -4
?DrawTextW@CThemed@@QAEHPAVCDC@@HHABVCString@@KKABVCRect@@@Z PROC NEAR ; CThemed::DrawTextW, COMDAT

; 251  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74746:

; 252  : 	ASSERT (m_hTheme);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74749
	push	252					; 000000fcH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74749
	int	3
$L74749:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74746

; 253  : 	ASSERT_VALID (pDC);

	push	253					; 000000fdH
	push	OFFSET FLAT:_THIS_FILE
	mov	edx, DWORD PTR _pDC$[ebp]
	push	edx
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 254  : 	
; 255  : 	return DrawThemeText(*pDC, nPart, nState, WSTR(sText), sText.GetLength(), dwTextFlags, dwTextFlags2, rect);

	mov	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QBEPBUtagRECT@@XZ		; CRect::operator tagRECT const *
	push	eax
	mov	eax, DWORD PTR _dwTextFlags2$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwTextFlags$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sText$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	push	eax
	mov	ecx, DWORD PTR _sText$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	edx, DWORD PTR _nState$[ebp]
	push	edx
	mov	eax, DWORD PTR _nPart$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	??BCDC@@QBEPAUHDC__@@XZ			; CDC::operator HDC__ *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DrawThemeText@CThemed@@IAEHPAUHDC__@@HHPBGHKKPBUtagRECT@@@Z ; CThemed::DrawThemeText

; 256  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?DrawTextW@CThemed@@QAEHPAVCDC@@HHABVCString@@KKABVCRect@@@Z ENDP ; CThemed::DrawTextW
_TEXT	ENDS
PUBLIC	?DrawThemeEdge@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@IIPAU3@@Z ; CThemed::DrawThemeEdge
;	COMDAT ?DrawEdge@CThemed@@QAEHPAVCDC@@HHABVCRect@@IIPAUtagRECT@@@Z
_TEXT	SEGMENT
_nState$ = 16
_rDest$ = 20
_nEdge$ = 24
_nFlags$ = 28
_prContent$ = 32
_this$ = -4
_pDC$ = 8
_nPart$ = 12
?DrawEdge@CThemed@@QAEHPAVCDC@@HHABVCRect@@IIPAUtagRECT@@@Z PROC NEAR ; CThemed::DrawEdge, COMDAT

; 259  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74760:

; 260  : 	ASSERT (m_hTheme);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74763
	push	260					; 00000104H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74763
	int	3
$L74763:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74760

; 261  : 	ASSERT_VALID (pDC);

	push	261					; 00000105H
	push	OFFSET FLAT:_THIS_FILE
	mov	edx, DWORD PTR _pDC$[ebp]
	push	edx
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 262  : 	
; 263  : 	return DrawThemeEdge(*pDC, nPart, nState, rDest, nEdge, nFlags, prContent);

	mov	eax, DWORD PTR _prContent$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nFlags$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nEdge$[ebp]
	push	edx
	mov	ecx, DWORD PTR _rDest$[ebp]
	call	??BCRect@@QBEPBUtagRECT@@XZ		; CRect::operator tagRECT const *
	push	eax
	mov	eax, DWORD PTR _nState$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nPart$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	??BCDC@@QBEPAUHDC__@@XZ			; CDC::operator HDC__ *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DrawThemeEdge@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@IIPAU3@@Z ; CThemed::DrawThemeEdge

; 264  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?DrawEdge@CThemed@@QAEHPAVCDC@@HHABVCRect@@IIPAUtagRECT@@@Z ENDP ; CThemed::DrawEdge
_TEXT	ENDS
PUBLIC	?DrawIcon@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBVCImageList@@H@Z ; CThemed::DrawIcon
PUBLIC	?DrawThemeIcon@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@PAU_IMAGELIST@@H@Z ; CThemed::DrawThemeIcon
EXTRN	??BCImageList@@QBEPAU_IMAGELIST@@XZ:NEAR	; CImageList::operator _IMAGELIST *
;	COMDAT ?DrawIcon@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBVCImageList@@H@Z
_TEXT	SEGMENT
_pDC$ = 8
_nPart$ = 12
_nState$ = 16
_rect$ = 20
_pIL$ = 24
_nImage$ = 28
_this$ = -4
?DrawIcon@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBVCImageList@@H@Z PROC NEAR ; CThemed::DrawIcon, COMDAT

; 267  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74773:

; 268  : 	ASSERT (m_hTheme);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74776
	push	268					; 0000010cH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74776
	int	3
$L74776:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74773

; 269  : 	ASSERT_VALID (pDC);

	push	269					; 0000010dH
	push	OFFSET FLAT:_THIS_FILE
	mov	edx, DWORD PTR _pDC$[ebp]
	push	edx
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 270  : 	ASSERT_VALID (pIL);

	push	270					; 0000010eH
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _pIL$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 271  : 	
; 272  : 	return DrawThemeIcon(*pDC, nPart, nState, rect, *pIL, nImage);

	mov	ecx, DWORD PTR _nImage$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pIL$[ebp]
	call	??BCImageList@@QBEPAU_IMAGELIST@@XZ	; CImageList::operator _IMAGELIST *
	push	eax
	mov	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QBEPBUtagRECT@@XZ		; CRect::operator tagRECT const *
	push	eax
	mov	edx, DWORD PTR _nState$[ebp]
	push	edx
	mov	eax, DWORD PTR _nPart$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	??BCDC@@QBEPAUHDC__@@XZ			; CDC::operator HDC__ *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DrawThemeIcon@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@PAU_IMAGELIST@@H@Z ; CThemed::DrawThemeIcon

; 273  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	24					; 00000018H
?DrawIcon@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBVCImageList@@H@Z ENDP ; CThemed::DrawIcon
_TEXT	ENDS
PUBLIC	?DrawBorder@CThemed@@QAEHPAVCDC@@HABVCRect@@@Z	; CThemed::DrawBorder
PUBLIC	?DrawThemeBorder@CThemed@@IAEHPAUHDC__@@HPBUtagRECT@@@Z ; CThemed::DrawThemeBorder
;	COMDAT ?DrawBorder@CThemed@@QAEHPAVCDC@@HABVCRect@@@Z
_TEXT	SEGMENT
_pDC$ = 8
_nState$ = 12
_rect$ = 16
_this$ = -4
?DrawBorder@CThemed@@QAEHPAVCDC@@HABVCRect@@@Z PROC NEAR ; CThemed::DrawBorder, COMDAT

; 276  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74783:

; 277  : 	ASSERT (m_hTheme);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74786
	push	277					; 00000115H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74786
	int	3
$L74786:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74783

; 278  : 	ASSERT_VALID (pDC);

	push	278					; 00000116H
	push	OFFSET FLAT:_THIS_FILE
	mov	edx, DWORD PTR _pDC$[ebp]
	push	edx
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 279  : 	
; 280  : 	return DrawThemeBorder(*pDC, nState, rect);

	mov	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QBEPBUtagRECT@@XZ		; CRect::operator tagRECT const *
	push	eax
	mov	eax, DWORD PTR _nState$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	??BCDC@@QBEPAUHDC__@@XZ			; CDC::operator HDC__ *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DrawThemeBorder@CThemed@@IAEHPAUHDC__@@HPBUtagRECT@@@Z ; CThemed::DrawThemeBorder

; 281  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?DrawBorder@CThemed@@QAEHPAVCDC@@HABVCRect@@@Z ENDP	; CThemed::DrawBorder
_TEXT	ENDS
PUBLIC	?GetSize@CThemed@@QAEHHHAAVCSize@@H@Z		; CThemed::GetSize
PUBLIC	?GetThemePartSize@CThemed@@IAEHHHW4THEMESIZE@@PAUtagSIZE@@@Z ; CThemed::GetThemePartSize
;	COMDAT ?GetSize@CThemed@@QAEHHHAAVCSize@@H@Z
_TEXT	SEGMENT
_nPart$ = 8
_nState$ = 12
_size$ = 16
_bMin$ = 20
_this$ = -4
?GetSize@CThemed@@QAEHHHAAVCSize@@H@Z PROC NEAR		; CThemed::GetSize, COMDAT

; 284  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74794:

; 285  : 	ASSERT (m_hTheme);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74797
	push	285					; 0000011dH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74797
	int	3
$L74797:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74794

; 286  : 	
; 287  : 	return GetThemePartSize(nPart, nState, bMin ? TS_MIN : TS_TRUE, &size);

	mov	edx, DWORD PTR _size$[ebp]
	push	edx
	xor	eax, eax
	cmp	DWORD PTR _bMin$[ebp], 0
	sete	al
	push	eax
	mov	ecx, DWORD PTR _nState$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nPart$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetThemePartSize@CThemed@@IAEHHHW4THEMESIZE@@PAUtagSIZE@@@Z ; CThemed::GetThemePartSize

; 288  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?GetSize@CThemed@@QAEHHHAAVCSize@@H@Z ENDP		; CThemed::GetSize
_TEXT	ENDS
PUBLIC	?GetTextExtent@CThemed@@QAEHPAVCDC@@HHABVCString@@KAAVCRect@@PBUtagRECT@@@Z ; CThemed::GetTextExtent
PUBLIC	?GetThemeTextExtent@CThemed@@IAEHPAUHDC__@@HHPBGHKPBUtagRECT@@PAU3@@Z ; CThemed::GetThemeTextExtent
EXTRN	??BCRect@@QAEPAUtagRECT@@XZ:NEAR		; CRect::operator tagRECT *
;	COMDAT ?GetTextExtent@CThemed@@QAEHPAVCDC@@HHABVCString@@KAAVCRect@@PBUtagRECT@@@Z
_TEXT	SEGMENT
_pDC$ = 8
_nPart$ = 12
_nState$ = 16
_sText$ = 20
_dwTextFlags$ = 24
_rExtent$ = 28
_prBounding$ = 32
_this$ = -4
?GetTextExtent@CThemed@@QAEHPAVCDC@@HHABVCString@@KAAVCRect@@PBUtagRECT@@@Z PROC NEAR ; CThemed::GetTextExtent, COMDAT

; 291  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74808:

; 292  : 	ASSERT (m_hTheme);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74811
	push	292					; 00000124H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74811
	int	3
$L74811:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74808

; 293  : 	
; 294  : 	return GetThemeTextExtent(pDC ? *pDC : (HDC)NULL, nPart, nState, WSTR(sText), sText.GetLength(), dwTextFlags, prBounding, rExtent);

	cmp	DWORD PTR _pDC$[ebp], 0
	je	SHORT $L75339
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	??BCDC@@QBEPAUHDC__@@XZ			; CDC::operator HDC__ *
	mov	DWORD PTR -8+[ebp], eax
	jmp	SHORT $L75340
$L75339:
	mov	DWORD PTR -8+[ebp], 0
$L75340:
	mov	ecx, DWORD PTR _rExtent$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	edx, DWORD PTR _prBounding$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwTextFlags$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sText$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	push	eax
	mov	ecx, DWORD PTR _sText$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _nState$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nPart$[ebp]
	push	edx
	mov	eax, DWORD PTR -8+[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetThemeTextExtent@CThemed@@IAEHPAUHDC__@@HHPBGHKPBUtagRECT@@PAU3@@Z ; CThemed::GetThemeTextExtent

; 295  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?GetTextExtent@CThemed@@QAEHPAVCDC@@HHABVCString@@KAAVCRect@@PBUtagRECT@@@Z ENDP ; CThemed::GetTextExtent
_TEXT	ENDS
PUBLIC	?BuildImageList@CThemed@@QAEHAAVCImageList@@HQBHHK@Z ; CThemed::BuildImageList
EXTRN	?FromHandle@CWnd@@SGPAV1@PAUHWND__@@@Z:NEAR	; CWnd::FromHandle
EXTRN	?FillSolidRect@CDC@@QAEXHHHHK@Z:NEAR		; CDC::FillSolidRect
EXTRN	__imp__GetDesktopWindow@0:NEAR
EXTRN	??1CDC@@UAE@XZ:NEAR				; CDC::~CDC
EXTRN	??0CClientDC@@QAE@PAVCWnd@@@Z:NEAR		; CClientDC::CClientDC
EXTRN	??1CClientDC@@UAE@XZ:NEAR			; CClientDC::~CClientDC
EXTRN	??0CBitmap@@QAE@XZ:NEAR				; CBitmap::CBitmap
EXTRN	?CreateCompatibleBitmap@CBitmap@@QAEHPAVCDC@@HH@Z:NEAR ; CBitmap::CreateCompatibleBitmap
EXTRN	??1CBitmap@@UAE@XZ:NEAR				; CBitmap::~CBitmap
EXTRN	??0CDC@@QAE@XZ:NEAR				; CDC::CDC
EXTRN	?CreateCompatibleDC@CDC@@QAEHPAV1@@Z:NEAR	; CDC::CreateCompatibleDC
EXTRN	?SelectObject@CDC@@QAEPAVCBitmap@@PAV2@@Z:NEAR	; CDC::SelectObject
EXTRN	?SelectObject@CDC@@QAEPAVCGdiObject@@PAV2@@Z:NEAR ; CDC::SelectObject
EXTRN	??0CSize@@QAE@XZ:NEAR				; CSize::CSize
EXTRN	??0CRect@@QAE@HHHH@Z:NEAR			; CRect::CRect
EXTRN	?Create@CImageList@@QAEHHHIHH@Z:NEAR		; CImageList::Create
EXTRN	?GetSafeHandle@CImageList@@QBEPAU_IMAGELIST@@XZ:NEAR ; CImageList::GetSafeHandle
EXTRN	?Add@CImageList@@QAEHPAVCBitmap@@K@Z:NEAR	; CImageList::Add
EXTRN	?OffsetRect@CRect@@QAEXHH@Z:NEAR		; CRect::OffsetRect
;	COMDAT xdata$x
; File D:\_CODE\Shared\Themed.cpp
xdata$x	SEGMENT
$T75354	DD	019930520H
	DD	03H
	DD	FLAT:$T75356
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75356	DD	0ffffffffH
	DD	FLAT:$L75350
	DD	00H
	DD	FLAT:$L75351
	DD	01H
	DD	FLAT:$L75352
xdata$x	ENDS
;	COMDAT ?BuildImageList@CThemed@@QAEHAAVCImageList@@HQBHHK@Z
_TEXT	SEGMENT
_il$ = 8
_nPart$ = 12
_nStates$ = 16
_nNumStates$ = 20
_crMask$ = 24
_this$ = -112
_size$ = -24
_BORDER$ = -28
_hWnd$ = -16
_dc$ = -48
_dcBack$ = -64
_bitmap$74833 = -72
_nImWidth$74834 = -76
_nImHeight$74835 = -80
_pOld$74837 = -100
_rState$74838 = -96
_nState$74839 = -104
$T75349 = -108
__$EHRec$ = -12
?BuildImageList@CThemed@@QAEHAAVCImageList@@HQBHHK@Z PROC NEAR ; CThemed::BuildImageList, COMDAT

; 298  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75355
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 112				; 00000070H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-124]
	mov	ecx, 28					; 0000001cH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L74821:

; 299  : 	ASSERT (!il.GetSafeHandle());

	mov	ecx, DWORD PTR _il$[ebp]
	call	?GetSafeHandle@CImageList@@QBEPAU_IMAGELIST@@XZ ; CImageList::GetSafeHandle
	test	eax, eax
	je	SHORT $L74824
	push	299					; 0000012bH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74824
	int	3
$L74824:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74821

; 300  : 	
; 301  : 	if (il.GetSafeHandle())

	mov	ecx, DWORD PTR _il$[ebp]
	call	?GetSafeHandle@CImageList@@QBEPAU_IMAGELIST@@XZ ; CImageList::GetSafeHandle
	test	eax, eax
	je	SHORT $L74825

; 302  : 		return FALSE;

	xor	eax, eax
	jmp	$L74820
$L74825:

; 303  : 	
; 304  : 	if (!nNumStates)

	cmp	DWORD PTR _nNumStates$[ebp], 0
	jne	SHORT $L74826

; 305  : 		return FALSE;

	xor	eax, eax
	jmp	$L74820
$L74826:

; 306  : 	
; 307  : 	CSize size;

	lea	ecx, DWORD PTR _size$[ebp]
	call	??0CSize@@QAE@XZ			; CSize::CSize

; 308  : 	GetSize(nPart, 1, size);

	push	0
	lea	ecx, DWORD PTR _size$[ebp]
	push	ecx
	push	1
	mov	edx, DWORD PTR _nPart$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSize@CThemed@@QAEHHHAAVCSize@@H@Z	; CThemed::GetSize

; 309  : 
; 310  : 	// little cheat for check boxes
; 311  : 	const UINT BORDER = (nPart == BP_CHECKBOX) ? 1 : 0;

	xor	eax, eax
	cmp	DWORD PTR _nPart$[ebp], 3
	sete	al
	mov	DWORD PTR _BORDER$[ebp], eax

; 312  : 
; 313  : 	// create a bitmap containing the required images
; 314  : 	HWND hWnd = m_hWnd ? m_hWnd : GetDesktopWindow();

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+8], 0
	je	SHORT $L75343
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	mov	DWORD PTR -116+[ebp], eax
	jmp	SHORT $L75344
$L75343:
	mov	esi, esp
	call	DWORD PTR __imp__GetDesktopWindow@0
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR -116+[ebp], eax
$L75344:
	mov	ecx, DWORD PTR -116+[ebp]
	mov	DWORD PTR _hWnd$[ebp], ecx

; 315  : 	CClientDC dc(CWnd::FromHandle(hWnd));

	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	call	?FromHandle@CWnd@@SGPAV1@PAUHWND__@@@Z	; CWnd::FromHandle
	push	eax
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??0CClientDC@@QAE@PAVCWnd@@@Z		; CClientDC::CClientDC
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 316  : 	CDC dcBack;

	lea	ecx, DWORD PTR _dcBack$[ebp]
	call	??0CDC@@QAE@XZ				; CDC::CDC
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 317  : 	
; 318  : 	if (dcBack.CreateCompatibleDC(&dc))

	lea	eax, DWORD PTR _dc$[ebp]
	push	eax
	lea	ecx, DWORD PTR _dcBack$[ebp]
	call	?CreateCompatibleDC@CDC@@QAEHPAV1@@Z	; CDC::CreateCompatibleDC
	test	eax, eax
	je	$L74832

; 320  : 		CBitmap bitmap;

	lea	ecx, DWORD PTR _bitmap$74833[ebp]
	call	??0CBitmap@@QAE@XZ			; CBitmap::CBitmap
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 321  : 
; 322  : 		int nImWidth = max(16, (size.cx + 2 * BORDER));

	mov	ecx, DWORD PTR _BORDER$[ebp]
	mov	edx, DWORD PTR _size$[ebp]
	lea	eax, DWORD PTR [edx+ecx*2]
	cmp	eax, 16					; 00000010H
	jae	SHORT $L75345
	mov	DWORD PTR -120+[ebp], 16		; 00000010H
	jmp	SHORT $L75346
$L75345:
	mov	ecx, DWORD PTR _BORDER$[ebp]
	mov	edx, DWORD PTR _size$[ebp]
	lea	eax, DWORD PTR [edx+ecx*2]
	mov	DWORD PTR -120+[ebp], eax
$L75346:
	mov	ecx, DWORD PTR -120+[ebp]
	mov	DWORD PTR _nImWidth$74834[ebp], ecx

; 323  : 		int nImHeight = max(16, (size.cy + 2 * BORDER));

	mov	edx, DWORD PTR _BORDER$[ebp]
	mov	eax, DWORD PTR _size$[ebp+4]
	lea	ecx, DWORD PTR [eax+edx*2]
	cmp	ecx, 16					; 00000010H
	jae	SHORT $L75347
	mov	DWORD PTR -124+[ebp], 16		; 00000010H
	jmp	SHORT $L75348
$L75347:
	mov	edx, DWORD PTR _BORDER$[ebp]
	mov	eax, DWORD PTR _size$[ebp+4]
	lea	ecx, DWORD PTR [eax+edx*2]
	mov	DWORD PTR -124+[ebp], ecx
$L75348:
	mov	edx, DWORD PTR -124+[ebp]
	mov	DWORD PTR _nImHeight$74835[ebp], edx

; 324  : 		
; 325  : 		if (bitmap.CreateCompatibleBitmap(&dc, nImWidth * nNumStates, nImHeight))

	mov	eax, DWORD PTR _nImHeight$74835[ebp]
	push	eax
	mov	ecx, DWORD PTR _nImWidth$74834[ebp]
	imul	ecx, DWORD PTR _nNumStates$[ebp]
	push	ecx
	lea	edx, DWORD PTR _dc$[ebp]
	push	edx
	lea	ecx, DWORD PTR _bitmap$74833[ebp]
	call	?CreateCompatibleBitmap@CBitmap@@QAEHPAVCDC@@HH@Z ; CBitmap::CreateCompatibleBitmap
	test	eax, eax
	je	$L74844

; 327  : 			CGdiObject* pOld = dcBack.SelectObject(&bitmap);

	lea	eax, DWORD PTR _bitmap$74833[ebp]
	push	eax
	lea	ecx, DWORD PTR _dcBack$[ebp]
	call	?SelectObject@CDC@@QAEPAVCBitmap@@PAV2@@Z ; CDC::SelectObject
	mov	DWORD PTR _pOld$74837[ebp], eax

; 328  : 			dcBack.FillSolidRect(0, 0, nImWidth * nNumStates, nImHeight, crMask);

	mov	ecx, DWORD PTR _crMask$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nImHeight$74835[ebp]
	push	edx
	mov	eax, DWORD PTR _nImWidth$74834[ebp]
	imul	eax, DWORD PTR _nNumStates$[ebp]
	push	eax
	push	0
	push	0
	lea	ecx, DWORD PTR _dcBack$[ebp]
	call	?FillSolidRect@CDC@@QAEXHHHHK@Z		; CDC::FillSolidRect

; 329  : 			
; 330  : 			CRect rState(BORDER, BORDER, size.cx + BORDER, size.cy + BORDER);

	mov	ecx, DWORD PTR _size$[ebp+4]
	add	ecx, DWORD PTR _BORDER$[ebp]
	push	ecx
	mov	edx, DWORD PTR _size$[ebp]
	add	edx, DWORD PTR _BORDER$[ebp]
	push	edx
	mov	eax, DWORD PTR _BORDER$[ebp]
	push	eax
	mov	ecx, DWORD PTR _BORDER$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _rState$74838[ebp]
	call	??0CRect@@QAE@HHHH@Z			; CRect::CRect

; 331  : 
; 332  : 			for (int nState = 0; nState < nNumStates; nState++)

	mov	DWORD PTR _nState$74839[ebp], 0
	jmp	SHORT $L74840
$L74841:
	mov	edx, DWORD PTR _nState$74839[ebp]
	add	edx, 1
	mov	DWORD PTR _nState$74839[ebp], edx
$L74840:
	mov	eax, DWORD PTR _nState$74839[ebp]
	cmp	eax, DWORD PTR _nNumStates$[ebp]
	jge	SHORT $L74842

; 334  : 				if (nStates[nState] != -1)

	mov	ecx, DWORD PTR _nState$74839[ebp]
	mov	edx, DWORD PTR _nStates$[ebp]
	cmp	DWORD PTR [edx+ecx*4], -1
	je	SHORT $L74843

; 335  : 					DrawBackground(&dcBack, nPart, nStates[nState], rState);

	push	0
	lea	eax, DWORD PTR _rState$74838[ebp]
	push	eax
	mov	ecx, DWORD PTR _nState$74839[ebp]
	mov	edx, DWORD PTR _nStates$[ebp]
	mov	eax, DWORD PTR [edx+ecx*4]
	push	eax
	mov	ecx, DWORD PTR _nPart$[ebp]
	push	ecx
	lea	edx, DWORD PTR _dcBack$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DrawBackground@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBUtagRECT@@@Z ; CThemed::DrawBackground
$L74843:

; 336  : 				
; 337  : 				// next state
; 338  : 				rState.OffsetRect(nImWidth, 0);

	push	0
	mov	eax, DWORD PTR _nImWidth$74834[ebp]
	push	eax
	lea	ecx, DWORD PTR _rState$74838[ebp]
	call	?OffsetRect@CRect@@QAEXHH@Z		; CRect::OffsetRect

; 339  : 			}

	jmp	SHORT $L74841
$L74842:

; 340  : 			
; 341  : 			dcBack.SelectObject(pOld);

	mov	ecx, DWORD PTR _pOld$74837[ebp]
	push	ecx
	lea	ecx, DWORD PTR _dcBack$[ebp]
	call	?SelectObject@CDC@@QAEPAVCGdiObject@@PAV2@@Z ; CDC::SelectObject

; 342  : 			
; 343  : 			// create imagelist
; 344  : 			if (il.Create(nImWidth, nImHeight, ILC_COLOR32 | ILC_MASK, 0, 1))

	push	1
	push	0
	push	33					; 00000021H
	mov	edx, DWORD PTR _nImHeight$74835[ebp]
	push	edx
	mov	eax, DWORD PTR _nImWidth$74834[ebp]
	push	eax
	mov	ecx, DWORD PTR _il$[ebp]
	call	?Create@CImageList@@QAEHHHIHH@Z		; CImageList::Create
	test	eax, eax
	je	SHORT $L74844

; 345  : 				il.Add(&bitmap, crMask);

	mov	ecx, DWORD PTR _crMask$[ebp]
	push	ecx
	lea	edx, DWORD PTR _bitmap$74833[ebp]
	push	edx
	mov	ecx, DWORD PTR _il$[ebp]
	call	?Add@CImageList@@QAEHPAVCBitmap@@K@Z	; CImageList::Add
$L74844:

; 347  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _bitmap$74833[ebp]
	call	??1CBitmap@@UAE@XZ			; CBitmap::~CBitmap
$L74832:

; 348  : 	
; 349  : 	return (il.GetSafeHandle() != NULL);

	mov	ecx, DWORD PTR _il$[ebp]
	call	?GetSafeHandle@CImageList@@QBEPAU_IMAGELIST@@XZ ; CImageList::GetSafeHandle
	neg	eax
	sbb	eax, eax
	neg	eax
	mov	DWORD PTR $T75349[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _dcBack$[ebp]
	call	??1CDC@@UAE@XZ				; CDC::~CDC
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??1CClientDC@@UAE@XZ			; CClientDC::~CClientDC
	mov	eax, DWORD PTR $T75349[ebp]
$L74820:

; 350  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 124				; 0000007cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	20					; 00000014H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75350:
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??1CClientDC@@UAE@XZ			; CClientDC::~CClientDC
	ret	0
$L75351:
	lea	ecx, DWORD PTR _dcBack$[ebp]
	call	??1CDC@@UAE@XZ				; CDC::~CDC
	ret	0
$L75352:
	lea	ecx, DWORD PTR _bitmap$74833[ebp]
	call	??1CBitmap@@UAE@XZ			; CBitmap::~CBitmap
	ret	0
$L75355:
	mov	eax, OFFSET FLAT:$T75354
	jmp	___CxxFrameHandler
text$x	ENDS
?BuildImageList@CThemed@@QAEHAAVCImageList@@HQBHHK@Z ENDP ; CThemed::BuildImageList
PUBLIC	?GetThemeColor@CThemed@@QAEKHHH@Z		; CThemed::GetThemeColor
PUBLIC	?GetThemeColor@CThemed@@IAEHHHHPAK@Z		; CThemed::GetThemeColor
;	COMDAT ?GetThemeColor@CThemed@@QAEKHHH@Z
_TEXT	SEGMENT
_nPart$ = 8
_nState$ = 12
_nProp$ = 16
_this$ = -8
_color$ = -4
?GetThemeColor@CThemed@@QAEKHHH@Z PROC NEAR		; CThemed::GetThemeColor, COMDAT

; 353  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74852:

; 354  : 	ASSERT (m_hTheme);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74855
	push	354					; 00000162H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74855
	int	3
$L74855:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74852

; 355  : 
; 356  : 	COLORREF color;
; 357  : 	
; 358  : 	if (GetThemeColor(nPart, nState, nProp, &color))

	lea	edx, DWORD PTR _color$[ebp]
	push	edx
	mov	eax, DWORD PTR _nProp$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nState$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nPart$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetThemeColor@CThemed@@IAEHHHHPAK@Z	; CThemed::GetThemeColor
	test	eax, eax
	je	SHORT $L74857

; 359  : 		return color;

	mov	eax, DWORD PTR _color$[ebp]
	jmp	SHORT $L74851
$L74857:

; 360  : 
; 361  : 	return 0; // black

	xor	eax, eax
$L74851:

; 362  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?GetThemeColor@CThemed@@QAEKHHH@Z ENDP			; CThemed::GetThemeColor
_TEXT	ENDS
PUBLIC	?GetThemeBackgroundContentRect@CThemed@@QAEHPAVCDC@@HHABVCRect@@AAV3@@Z ; CThemed::GetThemeBackgroundContentRect
PUBLIC	?GetThemeBackgroundContentRect@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@PAU3@@Z ; CThemed::GetThemeBackgroundContentRect
EXTRN	?GetSafeHdc@CDC@@QBEPAUHDC__@@XZ:NEAR		; CDC::GetSafeHdc
;	COMDAT ?GetThemeBackgroundContentRect@CThemed@@QAEHPAVCDC@@HHABVCRect@@AAV3@@Z
_TEXT	SEGMENT
_pDC$ = 8
_nPart$ = 12
_nState$ = 16
_rBounding$ = 20
_rContent$ = 24
_this$ = -4
?GetThemeBackgroundContentRect@CThemed@@QAEHPAVCDC@@HHABVCRect@@AAV3@@Z PROC NEAR ; CThemed::GetThemeBackgroundContentRect, COMDAT

; 365  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74866:

; 366  : 	ASSERT (m_hTheme);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74869
	push	366					; 0000016eH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74869
	int	3
$L74869:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74866

; 367  : 	ASSERT_VALID (pDC);

	push	367					; 0000016fH
	push	OFFSET FLAT:_THIS_FILE
	mov	edx, DWORD PTR _pDC$[ebp]
	push	edx
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 368  : 
; 369  : 	return GetThemeBackgroundContentRect(pDC->GetSafeHdc(), nPart, nState, rBounding, rContent);

	mov	ecx, DWORD PTR _rContent$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _rBounding$[ebp]
	call	??BCRect@@QBEPBUtagRECT@@XZ		; CRect::operator tagRECT const *
	push	eax
	mov	eax, DWORD PTR _nState$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nPart$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetSafeHdc@CDC@@QBEPAUHDC__@@XZ	; CDC::GetSafeHdc
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetThemeBackgroundContentRect@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@PAU3@@Z ; CThemed::GetThemeBackgroundContentRect

; 370  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	20					; 00000014H
?GetThemeBackgroundContentRect@CThemed@@QAEHPAVCDC@@HHABVCRect@@AAV3@@Z ENDP ; CThemed::GetThemeBackgroundContentRect
_TEXT	ENDS
PUBLIC	?IsAppThemed@CThemed@@SAHXZ			; CThemed::IsAppThemed
;	COMDAT ?IsThemeActive@CThemed@@SAHXZ
_TEXT	SEGMENT
?IsThemeActive@CThemed@@SAHXZ PROC NEAR			; CThemed::IsThemeActive, COMDAT

; 375  : {

	push	ebp
	mov	ebp, esp

; 376  : 	return IsAppThemed();

	call	?IsAppThemed@CThemed@@SAHXZ		; CThemed::IsAppThemed

; 377  : // 	if (InitUxTheme())
; 378  : // 	{
; 379  : // 		PFNISTHEMEACTIVE fnIsThemeActive = (PFNISTHEMEACTIVE)GetProcAddress(s_hUxTheme, "IsThemeActive");
; 380  : // 		
; 381  : // 		if (fnIsThemeActive)
; 382  : // 			return fnIsThemeActive();
; 383  : // 	}
; 384  : // 	
; 385  : // 	return FALSE;
; 386  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?IsThemeActive@CThemed@@SAHXZ ENDP			; CThemed::IsThemeActive
_TEXT	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG74877 DB	'IsAppThemed', 00H
data	ENDS
;	COMDAT ?IsAppThemed@CThemed@@SAHXZ
_TEXT	SEGMENT
_fnIsAppThemed$74875 = -4
?IsAppThemed@CThemed@@SAHXZ PROC NEAR			; CThemed::IsAppThemed, COMDAT

; 389  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 390  : 	if (InitUxTheme())

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L74878

; 392  : 		PFNISAPPTHEMED fnIsAppThemed = (PFNISAPPTHEMED)GetProcAddress(s_hUxTheme, "IsAppThemed");

	mov	esi, esp
	push	OFFSET FLAT:$SG74877
	mov	eax, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	eax
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnIsAppThemed$74875[ebp], eax

; 393  : 		
; 394  : 		if (fnIsAppThemed)

	cmp	DWORD PTR _fnIsAppThemed$74875[ebp], 0
	je	SHORT $L74878

; 395  : 			return fnIsAppThemed();

	mov	esi, esp
	call	DWORD PTR _fnIsAppThemed$74875[ebp]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L74873
$L74878:

; 397  : 	
; 398  : 	return FALSE;

	xor	eax, eax
$L74873:

; 399  : 
; 400  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsAppThemed@CThemed@@SAHXZ ENDP			; CThemed::IsAppThemed
_TEXT	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG74884 DB	'GetThemeAppProperties', 00H
data	ENDS
;	COMDAT ?GetAppThemeProperties@CThemed@@KAKXZ
_TEXT	SEGMENT
_fnGetThemeAppProperties$74882 = -4
?GetAppThemeProperties@CThemed@@KAKXZ PROC NEAR		; CThemed::GetAppThemeProperties, COMDAT

; 403  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 404  : 	if (InitUxTheme())

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L74885

; 406  : 		PFNGETTHEMEAPPPROPERTIES fnGetThemeAppProperties = (PFNGETTHEMEAPPPROPERTIES)GetProcAddress(s_hUxTheme, "GetThemeAppProperties");

	mov	esi, esp
	push	OFFSET FLAT:$SG74884
	mov	eax, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	eax
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnGetThemeAppProperties$74882[ebp], eax

; 407  : 		
; 408  : 		if (fnGetThemeAppProperties)

	cmp	DWORD PTR _fnGetThemeAppProperties$74882[ebp], 0
	je	SHORT $L74885

; 409  : 			return fnGetThemeAppProperties();

	mov	esi, esp
	call	DWORD PTR _fnGetThemeAppProperties$74882[ebp]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L74880
$L74885:

; 411  : 	
; 412  : 	return 0;

	xor	eax, eax
$L74880:

; 413  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetAppThemeProperties@CThemed@@KAKXZ ENDP		; CThemed::GetAppThemeProperties
_TEXT	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG74894 DB	'OpenThemeData', 00H
data	ENDS
;	COMDAT ?OpenThemeData@CThemed@@IAEPAXPAUHWND__@@PBG@Z
_TEXT	SEGMENT
_hwnd$ = 8
_pszClassList$ = 12
_this$ = -8
_fnOpenThemeData$74892 = -4
?OpenThemeData@CThemed@@IAEPAXPAUHWND__@@PBG@Z PROC NEAR ; CThemed::OpenThemeData, COMDAT

; 416  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 417  : 	if (InitUxTheme())

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L74895

; 419  : 		PFNOPENTHEMEDATA fnOpenThemeData = (PFNOPENTHEMEDATA)GetProcAddress(s_hUxTheme, "OpenThemeData");

	mov	esi, esp
	push	OFFSET FLAT:$SG74894
	mov	eax, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	eax
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnOpenThemeData$74892[ebp], eax

; 420  : 		
; 421  : 		if (fnOpenThemeData)

	cmp	DWORD PTR _fnOpenThemeData$74892[ebp], 0
	je	SHORT $L74895

; 422  : 			return fnOpenThemeData(hwnd, pszClassList);

	mov	esi, esp
	mov	ecx, DWORD PTR _pszClassList$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hwnd$[ebp]
	push	edx
	call	DWORD PTR _fnOpenThemeData$74892[ebp]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L74890
$L74895:

; 424  : 	
; 425  : 	return NULL;

	xor	eax, eax
$L74890:

; 426  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?OpenThemeData@CThemed@@IAEPAXPAUHWND__@@PBG@Z ENDP	; CThemed::OpenThemeData
_TEXT	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG74903 DB	'CloseThemeData', 00H
data	ENDS
;	COMDAT ?CloseThemeData@CThemed@@IAEHPAX@Z
_TEXT	SEGMENT
_hTheme$ = 8
_this$ = -8
_fnCloseThemeData$74901 = -4
?CloseThemeData@CThemed@@IAEHPAX@Z PROC NEAR		; CThemed::CloseThemeData, COMDAT

; 429  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 430  : 	if (InitUxTheme() && hTheme)

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L74904
	cmp	DWORD PTR _hTheme$[ebp], 0
	je	SHORT $L74904

; 432  : 		PFNCLOSETHEMEDATA fnCloseThemeData = (PFNCLOSETHEMEDATA)GetProcAddress(s_hUxTheme, "CloseThemeData");

	mov	esi, esp
	push	OFFSET FLAT:$SG74903
	mov	eax, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	eax
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnCloseThemeData$74901[ebp], eax

; 433  : 		
; 434  : 		if (fnCloseThemeData)

	cmp	DWORD PTR _fnCloseThemeData$74901[ebp], 0
	je	SHORT $L74904

; 435  : 			return SUCCEEDED(fnCloseThemeData(m_hTheme));

	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR _fnCloseThemeData$74901[ebp]
	cmp	esi, esp
	call	__chkesp
	xor	ecx, ecx
	test	eax, eax
	setge	cl
	mov	eax, ecx
	jmp	SHORT $L74899
$L74904:

; 437  : 	
; 438  : 	return FALSE;

	xor	eax, eax
$L74899:

; 439  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?CloseThemeData@CThemed@@IAEHPAX@Z ENDP			; CThemed::CloseThemeData
_TEXT	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG74917 DB	'DrawThemeBackground', 00H
data	ENDS
;	COMDAT ?DrawThemeBackground@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@1@Z
_TEXT	SEGMENT
_hdc$ = 8
_iPartId$ = 12
_iStateId$ = 16
_pRect$ = 20
_pClipRect$ = 24
_this$ = -8
_fnDrawThemeBackground$74915 = -4
?DrawThemeBackground@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@1@Z PROC NEAR ; CThemed::DrawThemeBackground, COMDAT

; 443  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 444  : 	if (InitUxTheme() && m_hTheme)

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L74918
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74918

; 446  : 		PFNDRAWTHEMEBACKGROUND fnDrawThemeBackground = (PFNDRAWTHEMEBACKGROUND)GetProcAddress(s_hUxTheme, "DrawThemeBackground");

	mov	esi, esp
	push	OFFSET FLAT:$SG74917
	mov	ecx, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnDrawThemeBackground$74915[ebp], eax

; 447  : 		
; 448  : 		if (fnDrawThemeBackground)

	cmp	DWORD PTR _fnDrawThemeBackground$74915[ebp], 0
	je	SHORT $L74918

; 449  : 			return (SUCCEEDED(fnDrawThemeBackground(m_hTheme, hdc, iPartId, iStateId, pRect, pClipRect)));

	mov	esi, esp
	mov	edx, DWORD PTR _pClipRect$[ebp]
	push	edx
	mov	eax, DWORD PTR _pRect$[ebp]
	push	eax
	mov	ecx, DWORD PTR _iStateId$[ebp]
	push	ecx
	mov	edx, DWORD PTR _iPartId$[ebp]
	push	edx
	mov	eax, DWORD PTR _hdc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR _fnDrawThemeBackground$74915[ebp]
	cmp	esi, esp
	call	__chkesp
	xor	ecx, ecx
	test	eax, eax
	setge	cl
	mov	eax, ecx
	jmp	SHORT $L74913
$L74918:

; 451  : 	
; 452  : 	return FALSE;

	xor	eax, eax
$L74913:

; 453  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	20					; 00000014H
?DrawThemeBackground@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@1@Z ENDP ; CThemed::DrawThemeBackground
_TEXT	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG74929 DB	'DrawThemeParentBackground', 00H
data	ENDS
;	COMDAT ?DrawThemeParentBackground@CThemed@@IAEHPAUHWND__@@PAUHDC__@@PAUtagRECT@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
_hdc$ = 12
_pRect$ = 16
_this$ = -8
_fnDrawThemeParentBackground$74927 = -4
?DrawThemeParentBackground@CThemed@@IAEHPAUHWND__@@PAUHDC__@@PAUtagRECT@@@Z PROC NEAR ; CThemed::DrawThemeParentBackground, COMDAT

; 456  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 457  : 	if (InitUxTheme() && m_hTheme)

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L74930
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74930

; 459  : 		PFNDRAWTHEMEPARENTBACKGROUND fnDrawThemeParentBackground = (PFNDRAWTHEMEPARENTBACKGROUND)GetProcAddress(s_hUxTheme, "DrawThemeParentBackground");

	mov	esi, esp
	push	OFFSET FLAT:$SG74929
	mov	ecx, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnDrawThemeParentBackground$74927[ebp], eax

; 460  : 		
; 461  : 		if (fnDrawThemeParentBackground)

	cmp	DWORD PTR _fnDrawThemeParentBackground$74927[ebp], 0
	je	SHORT $L74930

; 462  : 			return (SUCCEEDED(fnDrawThemeParentBackground(hWnd, hdc, pRect)));

	mov	esi, esp
	mov	edx, DWORD PTR _pRect$[ebp]
	push	edx
	mov	eax, DWORD PTR _hdc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	call	DWORD PTR _fnDrawThemeParentBackground$74927[ebp]
	cmp	esi, esp
	call	__chkesp
	xor	edx, edx
	test	eax, eax
	setge	dl
	mov	eax, edx
	jmp	SHORT $L74925
$L74930:

; 464  : 	
; 465  : 	return FALSE;

	xor	eax, eax
$L74925:

; 466  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?DrawThemeParentBackground@CThemed@@IAEHPAUHWND__@@PAUHDC__@@PAUtagRECT@@@Z ENDP ; CThemed::DrawThemeParentBackground
_TEXT	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG74946 DB	'DrawThemeText', 00H
data	ENDS
;	COMDAT ?DrawThemeText@CThemed@@IAEHPAUHDC__@@HHPBGHKKPBUtagRECT@@@Z
_TEXT	SEGMENT
_hdc$ = 8
_iPartId$ = 12
_iStateId$ = 16
_pszText$ = 20
_iCharCount$ = 24
_dwTextFlags$ = 28
_dwTextFlags2$ = 32
_pRect$ = 36
_this$ = -8
_fnDrawThemeText$74944 = -4
?DrawThemeText@CThemed@@IAEHPAUHDC__@@HHPBGHKKPBUtagRECT@@@Z PROC NEAR ; CThemed::DrawThemeText, COMDAT

; 470  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 471  : 	if (InitUxTheme() && m_hTheme)

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L74947
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74947

; 473  : 		PFNDRAWTHEMETEXT fnDrawThemeText = (PFNDRAWTHEMETEXT)GetProcAddress(s_hUxTheme, "DrawThemeText");

	mov	esi, esp
	push	OFFSET FLAT:$SG74946
	mov	ecx, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnDrawThemeText$74944[ebp], eax

; 474  : 		
; 475  : 		if (fnDrawThemeText)

	cmp	DWORD PTR _fnDrawThemeText$74944[ebp], 0
	je	SHORT $L74947

; 476  : 			return (SUCCEEDED(fnDrawThemeText(m_hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, dwTextFlags2, pRect)));

	mov	esi, esp
	mov	edx, DWORD PTR _pRect$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwTextFlags2$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwTextFlags$[ebp]
	push	ecx
	mov	edx, DWORD PTR _iCharCount$[ebp]
	push	edx
	mov	eax, DWORD PTR _pszText$[ebp]
	push	eax
	mov	ecx, DWORD PTR _iStateId$[ebp]
	push	ecx
	mov	edx, DWORD PTR _iPartId$[ebp]
	push	edx
	mov	eax, DWORD PTR _hdc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR _fnDrawThemeText$74944[ebp]
	cmp	esi, esp
	call	__chkesp
	xor	ecx, ecx
	test	eax, eax
	setge	cl
	mov	eax, ecx
	jmp	SHORT $L74942
$L74947:

; 478  : 	
; 479  : 	return FALSE;

	xor	eax, eax
$L74942:

; 480  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	32					; 00000020H
?DrawThemeText@CThemed@@IAEHPAUHDC__@@HHPBGHKKPBUtagRECT@@@Z ENDP ; CThemed::DrawThemeText
_TEXT	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG74962 DB	'DrawThemeEdge', 00H
data	ENDS
;	COMDAT ?DrawThemeEdge@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@IIPAU3@@Z
_TEXT	SEGMENT
_hdc$ = 8
_iPartId$ = 12
_iStateId$ = 16
_pDestRect$ = 20
_uEdge$ = 24
_uFlags$ = 28
_pContentRect$ = 32
_this$ = -8
_fnDrawThemeEdge$74960 = -4
?DrawThemeEdge@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@IIPAU3@@Z PROC NEAR ; CThemed::DrawThemeEdge, COMDAT

; 484  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 485  : 	if (InitUxTheme() && m_hTheme)

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L74963
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74963

; 487  : 		PFNDRAWTHEMEEDGE fnDrawThemeEdge = (PFNDRAWTHEMEEDGE)GetProcAddress(s_hUxTheme, "DrawThemeEdge");

	mov	esi, esp
	push	OFFSET FLAT:$SG74962
	mov	ecx, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnDrawThemeEdge$74960[ebp], eax

; 488  : 		
; 489  : 		if (fnDrawThemeEdge)

	cmp	DWORD PTR _fnDrawThemeEdge$74960[ebp], 0
	je	SHORT $L74963

; 490  : 			return (SUCCEEDED(fnDrawThemeEdge(m_hTheme, hdc, iPartId, iStateId, pDestRect, uEdge, uFlags, pContentRect)));

	mov	esi, esp
	mov	edx, DWORD PTR _pContentRect$[ebp]
	push	edx
	mov	eax, DWORD PTR _uFlags$[ebp]
	push	eax
	mov	ecx, DWORD PTR _uEdge$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pDestRect$[ebp]
	push	edx
	mov	eax, DWORD PTR _iStateId$[ebp]
	push	eax
	mov	ecx, DWORD PTR _iPartId$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hdc$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR _fnDrawThemeEdge$74960[ebp]
	cmp	esi, esp
	call	__chkesp
	xor	edx, edx
	test	eax, eax
	setge	dl
	mov	eax, edx
	jmp	SHORT $L74958
$L74963:

; 492  : 	
; 493  : 	return FALSE;

	xor	eax, eax
$L74958:

; 494  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?DrawThemeEdge@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@IIPAU3@@Z ENDP ; CThemed::DrawThemeEdge
_TEXT	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG74977 DB	'DrawThemeIcon', 00H
data	ENDS
;	COMDAT ?DrawThemeIcon@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@PAU_IMAGELIST@@H@Z
_TEXT	SEGMENT
_hdc$ = 8
_iPartId$ = 12
_iStateId$ = 16
_pRect$ = 20
_himl$ = 24
_iImageIndex$ = 28
_this$ = -8
_fnDrawThemeIcon$74975 = -4
?DrawThemeIcon@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@PAU_IMAGELIST@@H@Z PROC NEAR ; CThemed::DrawThemeIcon, COMDAT

; 498  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 499  : 	if (InitUxTheme() && m_hTheme)

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L74978
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74978

; 501  : 		PFNDRAWTHEMEICON fnDrawThemeIcon = (PFNDRAWTHEMEICON)GetProcAddress(s_hUxTheme, "DrawThemeIcon");

	mov	esi, esp
	push	OFFSET FLAT:$SG74977
	mov	ecx, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnDrawThemeIcon$74975[ebp], eax

; 502  : 		
; 503  : 		if (fnDrawThemeIcon)

	cmp	DWORD PTR _fnDrawThemeIcon$74975[ebp], 0
	je	SHORT $L74978

; 504  : 			return (SUCCEEDED(fnDrawThemeIcon(m_hTheme, hdc, iPartId, iStateId, pRect, himl, iImageIndex)));

	mov	esi, esp
	mov	edx, DWORD PTR _iImageIndex$[ebp]
	push	edx
	mov	eax, DWORD PTR _himl$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pRect$[ebp]
	push	ecx
	mov	edx, DWORD PTR _iStateId$[ebp]
	push	edx
	mov	eax, DWORD PTR _iPartId$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hdc$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	DWORD PTR _fnDrawThemeIcon$74975[ebp]
	cmp	esi, esp
	call	__chkesp
	xor	ecx, ecx
	test	eax, eax
	setge	cl
	mov	eax, ecx
	jmp	SHORT $L74973
$L74978:

; 506  : 	
; 507  : 	return FALSE;

	xor	eax, eax
$L74973:

; 508  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	24					; 00000018H
?DrawThemeIcon@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@PAU_IMAGELIST@@H@Z ENDP ; CThemed::DrawThemeIcon
_TEXT	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG74989 DB	'DrawThemeBorder', 00H
data	ENDS
;	COMDAT ?DrawThemeBorder@CThemed@@IAEHPAUHDC__@@HPBUtagRECT@@@Z
_TEXT	SEGMENT
_hdc$ = 8
_iStateId$ = 12
_pRect$ = 16
_this$ = -8
_fnDrawThemeBorder$74987 = -4
?DrawThemeBorder@CThemed@@IAEHPAUHDC__@@HPBUtagRECT@@@Z PROC NEAR ; CThemed::DrawThemeBorder, COMDAT

; 511  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 512  : 	if (InitUxTheme() && m_hTheme)

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L74990
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74990

; 514  : 		PFNDRAWTHEMEBORDER fnDrawThemeBorder = (PFNDRAWTHEMEBORDER)GetProcAddress(s_hUxTheme, "DrawThemeBorder");

	mov	esi, esp
	push	OFFSET FLAT:$SG74989
	mov	ecx, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnDrawThemeBorder$74987[ebp], eax

; 515  : 		
; 516  : 		if (fnDrawThemeBorder)

	cmp	DWORD PTR _fnDrawThemeBorder$74987[ebp], 0
	je	SHORT $L74990

; 517  : 			return (SUCCEEDED(fnDrawThemeBorder(m_hTheme, hdc, iStateId, pRect)));

	mov	esi, esp
	mov	edx, DWORD PTR _pRect$[ebp]
	push	edx
	mov	eax, DWORD PTR _iStateId$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hdc$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	DWORD PTR _fnDrawThemeBorder$74987[ebp]
	cmp	esi, esp
	call	__chkesp
	xor	ecx, ecx
	test	eax, eax
	setge	cl
	mov	eax, ecx
	jmp	SHORT $L74985
$L74990:

; 519  : 	
; 520  : 	return FALSE;

	xor	eax, eax
$L74985:

; 521  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?DrawThemeBorder@CThemed@@IAEHPAUHDC__@@HPBUtagRECT@@@Z ENDP ; CThemed::DrawThemeBorder
_TEXT	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG75002 DB	'GetThemePartSize', 00H
data	ENDS
;	COMDAT ?GetThemePartSize@CThemed@@IAEHHHW4THEMESIZE@@PAUtagSIZE@@@Z
_TEXT	SEGMENT
_iPartId$ = 8
_iStateId$ = 12
_eSize$ = 16
_psz$ = 20
_this$ = -8
_fnGetThemePartSize$75000 = -4
?GetThemePartSize@CThemed@@IAEHHHW4THEMESIZE@@PAUtagSIZE@@@Z PROC NEAR ; CThemed::GetThemePartSize, COMDAT

; 524  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 525  : 	if (InitUxTheme() && m_hTheme)

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L75003
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L75003

; 527  : 		PFNGETTHEMEPARTSIZE fnGetThemePartSize = (PFNGETTHEMEPARTSIZE)GetProcAddress(s_hUxTheme, "GetThemePartSize");

	mov	esi, esp
	push	OFFSET FLAT:$SG75002
	mov	ecx, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnGetThemePartSize$75000[ebp], eax

; 528  : 		
; 529  : 		if (fnGetThemePartSize)

	cmp	DWORD PTR _fnGetThemePartSize$75000[ebp], 0
	je	SHORT $L75003

; 530  : 			return (SUCCEEDED(fnGetThemePartSize(m_hTheme, NULL, iPartId, iStateId, NULL, eSize, psz)));

	mov	esi, esp
	mov	edx, DWORD PTR _psz$[ebp]
	push	edx
	mov	eax, DWORD PTR _eSize$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _iStateId$[ebp]
	push	ecx
	mov	edx, DWORD PTR _iPartId$[ebp]
	push	edx
	push	0
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR _fnGetThemePartSize$75000[ebp]
	cmp	esi, esp
	call	__chkesp
	xor	edx, edx
	test	eax, eax
	setge	dl
	mov	eax, edx
	jmp	SHORT $L74998
$L75003:

; 532  : 	
; 533  : 	return FALSE;

	xor	eax, eax
$L74998:

; 534  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?GetThemePartSize@CThemed@@IAEHHHW4THEMESIZE@@PAUtagSIZE@@@Z ENDP ; CThemed::GetThemePartSize
_TEXT	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG75019 DB	'GetThemeTextExtent', 00H
data	ENDS
;	COMDAT ?GetThemeTextExtent@CThemed@@IAEHPAUHDC__@@HHPBGHKPBUtagRECT@@PAU3@@Z
_TEXT	SEGMENT
_hdc$ = 8
_iPartId$ = 12
_iStateId$ = 16
_pszText$ = 20
_iCharCount$ = 24
_dwTextFlags$ = 28
_pBoundingRect$ = 32
_pExtentRect$ = 36
_this$ = -8
_fnGetThemeTextExtent$75017 = -4
?GetThemeTextExtent@CThemed@@IAEHPAUHDC__@@HHPBGHKPBUtagRECT@@PAU3@@Z PROC NEAR ; CThemed::GetThemeTextExtent, COMDAT

; 538  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 539  : 	if (InitUxTheme() && m_hTheme)

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L75020
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L75020

; 541  : 		PFNGETTHEMETEXTEXTENT fnGetThemeTextExtent = (PFNGETTHEMETEXTEXTENT)GetProcAddress(s_hUxTheme, "GetThemeTextExtent");

	mov	esi, esp
	push	OFFSET FLAT:$SG75019
	mov	ecx, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnGetThemeTextExtent$75017[ebp], eax

; 542  : 		
; 543  : 		if (fnGetThemeTextExtent)

	cmp	DWORD PTR _fnGetThemeTextExtent$75017[ebp], 0
	je	SHORT $L75020

; 544  : 			return (SUCCEEDED(fnGetThemeTextExtent(m_hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, pBoundingRect, pExtentRect)));

	mov	esi, esp
	mov	edx, DWORD PTR _pExtentRect$[ebp]
	push	edx
	mov	eax, DWORD PTR _pBoundingRect$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwTextFlags$[ebp]
	push	ecx
	mov	edx, DWORD PTR _iCharCount$[ebp]
	push	edx
	mov	eax, DWORD PTR _pszText$[ebp]
	push	eax
	mov	ecx, DWORD PTR _iStateId$[ebp]
	push	ecx
	mov	edx, DWORD PTR _iPartId$[ebp]
	push	edx
	mov	eax, DWORD PTR _hdc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR _fnGetThemeTextExtent$75017[ebp]
	cmp	esi, esp
	call	__chkesp
	xor	ecx, ecx
	test	eax, eax
	setge	cl
	mov	eax, ecx
	jmp	SHORT $L75015
$L75020:

; 546  : 	
; 547  : 	return FALSE;

	xor	eax, eax
$L75015:

; 548  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	32					; 00000020H
?GetThemeTextExtent@CThemed@@IAEHPAUHDC__@@HHPBGHKPBUtagRECT@@PAU3@@Z ENDP ; CThemed::GetThemeTextExtent
_TEXT	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG75032 DB	'GetThemeColor', 00H
data	ENDS
;	COMDAT ?GetThemeColor@CThemed@@IAEHHHHPAK@Z
_TEXT	SEGMENT
_iPartId$ = 8
_iStateId$ = 12
_iPropId$ = 16
_pColor$ = 20
_this$ = -8
_fnGetThemeColor$75030 = -4
?GetThemeColor@CThemed@@IAEHHHHPAK@Z PROC NEAR		; CThemed::GetThemeColor, COMDAT

; 551  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 552  : 	if (InitUxTheme() && m_hTheme)

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L75033
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L75033

; 554  : 		PFNGETTHEMECOLOR fnGetThemeColor = (PFNGETTHEMECOLOR)GetProcAddress(s_hUxTheme, "GetThemeColor");

	mov	esi, esp
	push	OFFSET FLAT:$SG75032
	mov	ecx, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnGetThemeColor$75030[ebp], eax

; 555  : 		
; 556  : 		if (fnGetThemeColor)

	cmp	DWORD PTR _fnGetThemeColor$75030[ebp], 0
	je	SHORT $L75033

; 557  : 			return (SUCCEEDED(fnGetThemeColor(m_hTheme, iPartId, iStateId, iPropId, pColor)));

	mov	esi, esp
	mov	edx, DWORD PTR _pColor$[ebp]
	push	edx
	mov	eax, DWORD PTR _iPropId$[ebp]
	push	eax
	mov	ecx, DWORD PTR _iStateId$[ebp]
	push	ecx
	mov	edx, DWORD PTR _iPartId$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR _fnGetThemeColor$75030[ebp]
	cmp	esi, esp
	call	__chkesp
	xor	edx, edx
	test	eax, eax
	setge	dl
	mov	eax, edx
	jmp	SHORT $L75028
$L75033:

; 559  : 	
; 560  : 	return FALSE;

	xor	eax, eax
$L75028:

; 561  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?GetThemeColor@CThemed@@IAEHHHHPAK@Z ENDP		; CThemed::GetThemeColor
_TEXT	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG75046 DB	'GetThemeBackgroundContentRect', 00H
data	ENDS
;	COMDAT ?GetThemeBackgroundContentRect@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@PAU3@@Z
_TEXT	SEGMENT
_hdc$ = 8
_iPartId$ = 12
_iStateId$ = 16
_pBoundingRect$ = 20
_pContentRect$ = 24
_this$ = -8
_fnGetContentRect$75044 = -4
?GetThemeBackgroundContentRect@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@PAU3@@Z PROC NEAR ; CThemed::GetThemeBackgroundContentRect, COMDAT

; 564  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 565  : 	if (InitUxTheme() && m_hTheme)

	call	?InitUxTheme@CThemed@@KAHXZ		; CThemed::InitUxTheme
	test	eax, eax
	je	SHORT $L75047
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L75047

; 567  : 		PFNGETTHEMEBACKGROUNDCONTENTRECT fnGetContentRect = (PFNGETTHEMEBACKGROUNDCONTENTRECT)
; 568  : 															GetProcAddress(s_hUxTheme, "GetThemeBackgroundContentRect");

	mov	esi, esp
	push	OFFSET FLAT:$SG75046
	mov	ecx, DWORD PTR ?s_hUxTheme@CThemed@@1PAUHINSTANCE__@@A ; CThemed::s_hUxTheme
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _fnGetContentRect$75044[ebp], eax

; 569  : 		
; 570  : 		if (fnGetContentRect)

	cmp	DWORD PTR _fnGetContentRect$75044[ebp], 0
	je	SHORT $L75047

; 571  : 			return (SUCCEEDED(fnGetContentRect(m_hTheme, hdc, iPartId, iStateId, pBoundingRect, pContentRect)));

	mov	esi, esp
	mov	edx, DWORD PTR _pContentRect$[ebp]
	push	edx
	mov	eax, DWORD PTR _pBoundingRect$[ebp]
	push	eax
	mov	ecx, DWORD PTR _iStateId$[ebp]
	push	ecx
	mov	edx, DWORD PTR _iPartId$[ebp]
	push	edx
	mov	eax, DWORD PTR _hdc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR _fnGetContentRect$75044[ebp]
	cmp	esi, esp
	call	__chkesp
	xor	ecx, ecx
	test	eax, eax
	setge	cl
	mov	eax, ecx
	jmp	SHORT $L75042
$L75047:

; 573  : 	
; 574  : 	return FALSE;

	xor	eax, eax
$L75042:

; 575  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	20					; 00000014H
?GetThemeBackgroundContentRect@CThemed@@IAEHPAUHDC__@@HHPBUtagRECT@@PAU3@@Z ENDP ; CThemed::GetThemeBackgroundContentRect
_TEXT	ENDS
EXTRN	?Empty@CString@@QAEXXZ:NEAR			; CString::Empty
EXTRN	??4CString@@QAEABV0@PBD@Z:NEAR			; CString::operator=
;	COMDAT data
; File D:\_CODE\Shared\Themed.cpp
data	SEGMENT
$SG75061 DB	'BUTTON', 00H
	ORG $+1
$SG75086 DB	'COMBOBOX', 00H
	ORG $+3
$SG75094 DB	'SCROLLBAR', 00H
data	ENDS
;	COMDAT ?GetThemeClassPartState@CThemed@@KAHHHAAVCString@@AAH1@Z
_TEXT	SEGMENT
_nType$ = 8
_nState$ = 12
_sThClass$ = 16
_nThPart$ = 20
_nThState$ = 24
?GetThemeClassPartState@CThemed@@KAHHHAAVCString@@AAH1@Z PROC NEAR ; CThemed::GetThemeClassPartState, COMDAT

; 580  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 581  : 	sThClass.Empty();

	mov	ecx, DWORD PTR _sThClass$[ebp]
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 582  : 	nThPart = 0;

	mov	eax, DWORD PTR _nThPart$[ebp]
	mov	DWORD PTR [eax], 0

; 583  : 	nThState = 0;

	mov	ecx, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [ecx], 0

; 586  : 	{

	mov	edx, DWORD PTR _nType$[ebp]
	mov	DWORD PTR -4+[ebp], edx
	cmp	DWORD PTR -4+[ebp], 3
	je	$L75084
	cmp	DWORD PTR -4+[ebp], 4
	je	SHORT $L75060
	jmp	$L75057
$L75060:

; 589  : 			sThClass = "BUTTON";

	push	OFFSET FLAT:$SG75061
	mov	ecx, DWORD PTR _sThClass$[ebp]
	call	??4CString@@QAEABV0@PBD@Z		; CString::operator=

; 590  : 			nThState = PBS_NORMAL;

	mov	eax, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [eax], 1

; 591  : 			
; 592  : 			if (nState & DFCS_BUTTONPUSH) 

	mov	ecx, DWORD PTR _nState$[ebp]
	and	ecx, 16					; 00000010H
	test	ecx, ecx
	je	SHORT $L75062

; 594  : 				nThPart = BP_PUSHBUTTON;

	mov	edx, DWORD PTR _nThPart$[ebp]
	mov	DWORD PTR [edx], 1

; 595  : 				
; 596  : 				if (nState & (DFCS_CHECKED | DFCS_PUSHED))

	mov	eax, DWORD PTR _nState$[ebp]
	and	eax, 1536				; 00000600H
	test	eax, eax
	je	SHORT $L75063

; 597  : 					nThState = PBS_PRESSED;

	mov	ecx, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [ecx], 3

; 598  : 				
; 599  : 				else if (nState & DFCS_INACTIVE)

	jmp	SHORT $L75067
$L75063:
	mov	edx, DWORD PTR _nState$[ebp]
	and	edx, 256				; 00000100H
	test	edx, edx
	je	SHORT $L75065

; 600  : 					nThState = PBS_DISABLED;

	mov	eax, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [eax], 4

; 601  : 				
; 602  : 				else if (nState & DFCS_HOT)

	jmp	SHORT $L75067
$L75065:
	mov	ecx, DWORD PTR _nState$[ebp]
	and	ecx, 4096				; 00001000H
	test	ecx, ecx
	je	SHORT $L75067

; 603  : 					nThState = PBS_HOT;

	mov	edx, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [edx], 2
$L75067:

; 605  : 			/*			else if (nState & DFCS_BUTTONRADIO) 
; 606  : 			{
; 607  : 			nThPart = BP_RADIOBUTTON;
; 608  : 			}*/
; 609  : 			else if ((nState & DFCS_BUTTONCHECK) == DFCS_BUTTONCHECK) 

	jmp	$L75080
$L75062:
	xor	eax, eax
	test	eax, eax
	jne	$L75069

; 611  : 				nThPart = BP_CHECKBOX;

	mov	ecx, DWORD PTR _nThPart$[ebp]
	mov	DWORD PTR [ecx], 3

; 612  : 				
; 613  : 				if (nState & (DFCS_CHECKED | DFCS_PUSHED))

	mov	edx, DWORD PTR _nState$[ebp]
	and	edx, 1536				; 00000600H
	test	edx, edx
	je	SHORT $L75070

; 615  : 					if (nState & DFCS_INACTIVE)

	mov	eax, DWORD PTR _nState$[ebp]
	and	eax, 256				; 00000100H
	test	eax, eax
	je	SHORT $L75071

; 616  : 						nThState = CBS_CHECKEDDISABLED;

	mov	ecx, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [ecx], 8

; 617  : 
; 618  : 					else if (nState & DFCS_HOT)

	jmp	SHORT $L75074
$L75071:
	mov	edx, DWORD PTR _nState$[ebp]
	and	edx, 4096				; 00001000H
	test	edx, edx
	je	SHORT $L75073

; 619  : 						nThState = CBS_CHECKEDHOT;

	mov	eax, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [eax], 6

; 620  : 					else

	jmp	SHORT $L75074
$L75073:

; 621  : 						nThState = CBS_CHECKEDNORMAL;

	mov	ecx, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [ecx], 5
$L75074:

; 623  : 				else

	jmp	SHORT $L75079
$L75070:

; 625  : 					if (nState & DFCS_INACTIVE)

	mov	edx, DWORD PTR _nState$[ebp]
	and	edx, 256				; 00000100H
	test	edx, edx
	je	SHORT $L75076

; 626  : 						nThState = CBS_UNCHECKEDDISABLED;

	mov	eax, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [eax], 4

; 627  : 
; 628  : 					else if (nState & DFCS_HOT)

	jmp	SHORT $L75079
$L75076:
	mov	ecx, DWORD PTR _nState$[ebp]
	and	ecx, 4096				; 00001000H
	test	ecx, ecx
	je	SHORT $L75078

; 629  : 						nThState = CBS_UNCHECKEDHOT;

	mov	edx, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [edx], 2

; 630  : 					else

	jmp	SHORT $L75079
$L75078:

; 631  : 						nThState = CBS_UNCHECKEDNORMAL;

	mov	eax, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [eax], 1
$L75079:

; 634  : 			else 

	jmp	SHORT $L75080
$L75069:

; 635  : 				return FALSE;

	xor	eax, eax
	jmp	$L75055
$L75080:

; 637  : 		break;

	jmp	$L75057
$L75084:

; 650  : 			if (nState & DFCS_SCROLLCOMBOBOX) 

	mov	ecx, DWORD PTR _nState$[ebp]
	and	ecx, 5
	test	ecx, ecx
	je	SHORT $L75085

; 652  : 				sThClass = "COMBOBOX";

	push	OFFSET FLAT:$SG75086
	mov	ecx, DWORD PTR _sThClass$[ebp]
	call	??4CString@@QAEABV0@PBD@Z		; CString::operator=

; 653  : 				nThPart = CP_DROPDOWNBUTTON;

	mov	edx, DWORD PTR _nThPart$[ebp]
	mov	DWORD PTR [edx], 1

; 654  : 				nThState = CBXS_NORMAL;

	mov	eax, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [eax], 1

; 655  : 				
; 656  : 				if (nState & (DFCS_CHECKED | DFCS_PUSHED))

	mov	ecx, DWORD PTR _nState$[ebp]
	and	ecx, 1536				; 00000600H
	test	ecx, ecx
	je	SHORT $L75087

; 657  : 					nThState = CBXS_PRESSED;

	mov	edx, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [edx], 3

; 658  : 				
; 659  : 				else if (nState & DFCS_INACTIVE)

	jmp	SHORT $L75091
$L75087:
	mov	eax, DWORD PTR _nState$[ebp]
	and	eax, 256				; 00000100H
	test	eax, eax
	je	SHORT $L75089

; 660  : 					nThState = CBXS_DISABLED;

	mov	ecx, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [ecx], 4

; 661  : 				
; 662  : 				else if (nState & DFCS_HOT)

	jmp	SHORT $L75091
$L75089:
	mov	edx, DWORD PTR _nState$[ebp]
	and	edx, 4096				; 00001000H
	test	edx, edx
	je	SHORT $L75091

; 663  : 					nThState = CBXS_HOT;

	mov	eax, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [eax], 2
$L75091:

; 665  : 			else if (nState & DFCS_SCROLLSIZEGRIP)

	jmp	SHORT $L75093
$L75085:
	mov	ecx, DWORD PTR _nState$[ebp]
	and	ecx, 8
	test	ecx, ecx
	je	SHORT $L75093

; 667  : 				sThClass = "SCROLLBAR";

	push	OFFSET FLAT:$SG75094
	mov	ecx, DWORD PTR _sThClass$[ebp]
	call	??4CString@@QAEABV0@PBD@Z		; CString::operator=

; 668  : 				nThPart = SBP_SIZEBOX;

	mov	edx, DWORD PTR _nThPart$[ebp]
	mov	DWORD PTR [edx], 10			; 0000000aH

; 669  : 				nThState = (nState & DFCS_SCROLLLEFT) ? SZB_LEFTALIGN : SZB_RIGHTALIGN;

	mov	eax, DWORD PTR _nState$[ebp]
	and	eax, 2
	neg	eax
	sbb	eax, eax
	neg	eax
	add	eax, 1
	mov	ecx, DWORD PTR _nThState$[ebp]
	mov	DWORD PTR [ecx], eax
$L75093:
$L75057:

; 675  : 	
; 676  : 	return (!sThClass.IsEmpty() && (nThPart && nThState));

	mov	ecx, DWORD PTR _sThClass$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75409
	mov	edx, DWORD PTR _nThPart$[ebp]
	cmp	DWORD PTR [edx], 0
	je	SHORT $L75409
	mov	eax, DWORD PTR _nThState$[ebp]
	cmp	DWORD PTR [eax], 0
	je	SHORT $L75409
	mov	DWORD PTR -8+[ebp], 1
	jmp	SHORT $L75410
$L75409:
	mov	DWORD PTR -8+[ebp], 0
$L75410:
	mov	eax, DWORD PTR -8+[ebp]
$L75055:

; 677  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetThemeClassPartState@CThemed@@KAHHHAAVCString@@AAH1@Z ENDP ; CThemed::GetThemeClassPartState
_TEXT	ENDS
END

	TITLE	D:\_CODE\Shared\RichEditNcBorder.cpp
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
;	COMDAT ??_C@_0EK@MDCF@C?3?2Program?5Files?5?$CIx86?$CJ?2Microsoft@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
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
;	COMDAT ?HasExStyle@CSubclassWnd@@QBEHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HasStyle@CSubclassWnd@@QBEHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetWindowRect@CSubclassWnd@@QBEXPAUtagRECT@@@Z
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
;	COMDAT ??0CRichEditNcBorder@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCRichEditNcBorder@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CRichEditNcBorder@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Initialize@CRichEditNcBorder@@QAEHPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?WindowProc@CRichEditNcBorder@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NcDrawBorder@CRichEditNcBorder@@IAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?VerifyThemedBorderState@CRichEditNcBorder@@IAEXXZ
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
;	COMDAT ??_7CRichEditNcBorder@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CSubclasser@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\Shared\RichEditNcBorder.cpp', 00H
_DATA	ENDS
PUBLIC	?IsValid@CSubclassWnd@@UBEHXZ			; CSubclassWnd::IsValid
PUBLIC	?PreDetachWindow@CSubclassWnd@@MAEXXZ		; CSubclassWnd::PreDetachWindow
PUBLIC	?PostDetachWindow@CSubclassWnd@@MAEXXZ		; CSubclassWnd::PostDetachWindow
PUBLIC	?IsHooked@CSubclassWnd@@MBEHXZ			; CSubclassWnd::IsHooked
PUBLIC	?IsValidHook@CSubclassWnd@@MBEHXZ		; CSubclassWnd::IsValidHook
PUBLIC	?Redraw@CSubclassWnd@@MBEXXZ			; CSubclassWnd::Redraw
PUBLIC	??0CRichEditNcBorder@@QAE@XZ			; CRichEditNcBorder::CRichEditNcBorder
PUBLIC	?WindowProc@CRichEditNcBorder@@MAEJPAUHWND__@@IIJ@Z ; CRichEditNcBorder::WindowProc
PUBLIC	??_7CRichEditNcBorder@@6B@			; CRichEditNcBorder::`vftable'
PUBLIC	??_GCRichEditNcBorder@@UAEPAXI@Z		; CRichEditNcBorder::`scalar deleting destructor'
PUBLIC	??_ECRichEditNcBorder@@UAEPAXI@Z		; CRichEditNcBorder::`vector deleting destructor'
EXTRN	__chkesp:NEAR
EXTRN	??0CSubclassWnd@@QAE@XZ:NEAR			; CSubclassWnd::CSubclassWnd
EXTRN	?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z:NEAR ; CSubclassWnd::HookWindow
EXTRN	?PostMessageW@CSubclassWnd@@MBEHIIJ@Z:NEAR	; CSubclassWnd::PostMessageW
EXTRN	?SendMessageW@CSubclassWnd@@MBEHIIJ@Z:NEAR	; CSubclassWnd::SendMessageW
;	COMDAT ??_7CRichEditNcBorder@@6B@
; File D:\_CODE\Shared\RichEditNcBorder.cpp
CONST	SEGMENT
??_7CRichEditNcBorder@@6B@ DD FLAT:??_ECRichEditNcBorder@@UAEPAXI@Z ; CRichEditNcBorder::`vftable'
	DD	FLAT:?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z
	DD	FLAT:?IsValid@CSubclassWnd@@UBEHXZ
	DD	FLAT:?PreDetachWindow@CSubclassWnd@@MAEXXZ
	DD	FLAT:?PostDetachWindow@CSubclassWnd@@MAEXXZ
	DD	FLAT:?IsHooked@CSubclassWnd@@MBEHXZ
	DD	FLAT:?IsValidHook@CSubclassWnd@@MBEHXZ
	DD	FLAT:?Redraw@CSubclassWnd@@MBEXXZ
	DD	FLAT:?PostMessageW@CSubclassWnd@@MBEHIIJ@Z
	DD	FLAT:?SendMessageW@CSubclassWnd@@MBEHIIJ@Z
	DD	FLAT:?WindowProc@CRichEditNcBorder@@MAEJPAUHWND__@@IIJ@Z
CONST	ENDS
;	COMDAT ??0CRichEditNcBorder@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0CRichEditNcBorder@@QAE@XZ PROC NEAR			; CRichEditNcBorder::CRichEditNcBorder, COMDAT

; 25   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CSubclassWnd@@QAE@XZ			; CSubclassWnd::CSubclassWnd
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+32], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CRichEditNcBorder@@6B@ ; CRichEditNcBorder::`vftable'

; 26   : 
; 27   : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CRichEditNcBorder@@QAE@XZ ENDP			; CRichEditNcBorder::CRichEditNcBorder
_TEXT	ENDS
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
PUBLIC	??1CRichEditNcBorder@@UAE@XZ			; CRichEditNcBorder::~CRichEditNcBorder
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCRichEditNcBorder@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCRichEditNcBorder@@UAEPAXI@Z PROC NEAR		; CRichEditNcBorder::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CRichEditNcBorder@@UAE@XZ		; CRichEditNcBorder::~CRichEditNcBorder
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L74763
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L74763:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCRichEditNcBorder@@UAEPAXI@Z ENDP			; CRichEditNcBorder::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	??1CSubclassWnd@@UAE@XZ:NEAR			; CSubclassWnd::~CSubclassWnd
;	COMDAT ??1CRichEditNcBorder@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CRichEditNcBorder@@UAE@XZ PROC NEAR			; CRichEditNcBorder::~CRichEditNcBorder, COMDAT

; 30   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CRichEditNcBorder@@6B@ ; CRichEditNcBorder::`vftable'

; 31   : 
; 32   : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CSubclassWnd@@UAE@XZ			; CSubclassWnd::~CSubclassWnd
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CRichEditNcBorder@@UAE@XZ ENDP			; CRichEditNcBorder::~CRichEditNcBorder
_TEXT	ENDS
PUBLIC	?Initialize@CRichEditNcBorder@@QAEHPAUHWND__@@@Z ; CRichEditNcBorder::Initialize
PUBLIC	?VerifyThemedBorderState@CRichEditNcBorder@@IAEXXZ ; CRichEditNcBorder::VerifyThemedBorderState
;	COMDAT ?Initialize@CRichEditNcBorder@@QAEHPAUHWND__@@@Z
_TEXT	SEGMENT
_hwndRichEdit$ = 8
_this$ = -4
?Initialize@CRichEditNcBorder@@QAEHPAUHWND__@@@Z PROC NEAR ; CRichEditNcBorder::Initialize, COMDAT

; 35   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 36   : 	if (hwndRichEdit) // hook

	cmp	DWORD PTR _hwndRichEdit$[ebp], 0
	je	SHORT $L74771

; 38   : 		if (IsHooked())

	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+20]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L74772

; 39   : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L74770
$L74772:

; 40   : 
; 41   : 		// else
; 42   : 		if (HookWindow(hwndRichEdit))

	mov	esi, esp
	push	0
	mov	eax, DWORD PTR _hwndRichEdit$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L74773

; 44   : 			VerifyThemedBorderState();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?VerifyThemedBorderState@CRichEditNcBorder@@IAEXXZ ; CRichEditNcBorder::VerifyThemedBorderState

; 45   : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L74770
$L74773:

; 47   : 	
; 48   : 		// else
; 49   : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L74770
$L74771:

; 51   : 
; 52   : 	// else unhook
; 53   : 	if (IsHooked())

	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+20]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L74774

; 54   : 		return HookWindow(NULL);

	mov	esi, esp
	push	0
	push	0
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L74770
$L74774:

; 55   : 
; 56   : 	// else
; 57   : 	return TRUE;

	mov	eax, 1
$L74770:

; 58   : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Initialize@CRichEditNcBorder@@QAEHPAUHWND__@@@Z ENDP	; CRichEditNcBorder::Initialize
_TEXT	ENDS
PUBLIC	?NcDrawBorder@CRichEditNcBorder@@IAEXXZ		; CRichEditNcBorder::NcDrawBorder
EXTRN	__imp__InflateRect@12:NEAR
EXTRN	?WindowProc@CSubclassWnd@@MAEJPAUHWND__@@IIJ@Z:NEAR ; CSubclassWnd::WindowProc
EXTRN	__imp__RedrawWindow@16:NEAR
;	COMDAT ?WindowProc@CRichEditNcBorder@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT
_hRealWnd$ = 8
_msg$ = 12
_wp$ = 16
_lp$ = 20
_this$ = -8
_pNCP$74791 = -4
?WindowProc@CRichEditNcBorder@@MAEJPAUHWND__@@IIJ@Z PROC NEAR ; CRichEditNcBorder::WindowProc, COMDAT

; 61   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 63   : 	{

	mov	eax, DWORD PTR _msg$[ebp]
	mov	DWORD PTR -12+[ebp], eax
	cmp	DWORD PTR -12+[ebp], 131		; 00000083H
	ja	SHORT $L74992
	cmp	DWORD PTR -12+[ebp], 131		; 00000083H
	je	SHORT $L74789
	cmp	DWORD PTR -12+[ebp], 10			; 0000000aH
	je	$L74793
	cmp	DWORD PTR -12+[ebp], 125		; 0000007dH
	je	SHORT $L74788
	jmp	$L74783
$L74992:
	cmp	DWORD PTR -12+[ebp], 133		; 00000085H
	je	SHORT $L74786
	cmp	DWORD PTR -12+[ebp], 794		; 0000031aH
	je	SHORT $L74787
	jmp	$L74783
$L74786:

; 64   : 	case WM_NCPAINT:
; 65   : 		NcDrawBorder();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?NcDrawBorder@CRichEditNcBorder@@IAEXXZ	; CRichEditNcBorder::NcDrawBorder

; 66   : 		return 0L;

	xor	eax, eax
	jmp	$L74781
$L74787:

; 67   : 
; 68   : 	case WM_THEMECHANGED:
; 69   : 		VerifyThemedBorderState();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?VerifyThemedBorderState@CRichEditNcBorder@@IAEXXZ ; CRichEditNcBorder::VerifyThemedBorderState

; 70   : 		break;

	jmp	SHORT $L74783
$L74788:

; 71   : 
; 72   : 	case WM_STYLECHANGED:
; 73   : 		VerifyThemedBorderState();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?VerifyThemedBorderState@CRichEditNcBorder@@IAEXXZ ; CRichEditNcBorder::VerifyThemedBorderState

; 74   : 		break;

	jmp	SHORT $L74783
$L74789:

; 75   : 
; 76   : 	case WM_NCCALCSIZE:
; 77   : 		// If wParam is FALSE, we don't need to make any calculation
; 78   : 		if (m_bThemedBorder && wp)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+32], 0
	je	SHORT $L74790
	cmp	DWORD PTR _wp$[ebp], 0
	je	SHORT $L74790

; 80   : 			NCCALCSIZE_PARAMS *pNCP = (NCCALCSIZE_PARAMS*)lp;

	mov	edx, DWORD PTR _lp$[ebp]
	mov	DWORD PTR _pNCP$74791[ebp], edx

; 81   : 
; 82   : 			// Ask the control to first calculate the space it needs
; 83   : 			CSubclassWnd::WindowProc(hRealWnd, msg, wp, lp);

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

; 84   : 			
; 85   : 			// Alter the size for our own border
; 86   : 			::InflateRect(&(pNCP->rgrc[0]), -1, -1);

	mov	esi, esp
	push	-1
	push	-1
	mov	ecx, DWORD PTR _pNCP$74791[ebp]
	push	ecx
	call	DWORD PTR __imp__InflateRect@12
	cmp	esi, esp
	call	__chkesp

; 87   : 
; 88   : 			return WVR_REDRAW; // and redraw

	mov	eax, 768				; 00000300H
	jmp	SHORT $L74781
$L74790:

; 90   : 		break;

	jmp	SHORT $L74783
$L74793:

; 91   : 
; 92   : 	case WM_ENABLE:
; 93   : 		if (m_bThemedBorder)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+32], 0
	je	SHORT $L74794

; 94   : 			RedrawWindow(hRealWnd, NULL, NULL, RDW_INVALIDATE | RDW_NOCHILDREN | RDW_UPDATENOW | RDW_FRAME);

	mov	esi, esp
	push	1345					; 00000541H
	push	0
	push	0
	mov	eax, DWORD PTR _hRealWnd$[ebp]
	push	eax
	call	DWORD PTR __imp__RedrawWindow@16
	cmp	esi, esp
	call	__chkesp
$L74794:
$L74783:

; 96   : 
; 97   : 	return CSubclassWnd::WindowProc(hRealWnd, msg, wp, lp);

	mov	ecx, DWORD PTR _lp$[ebp]
	push	ecx
	mov	edx, DWORD PTR _wp$[ebp]
	push	edx
	mov	eax, DWORD PTR _msg$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hRealWnd$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?WindowProc@CSubclassWnd@@MAEJPAUHWND__@@IIJ@Z ; CSubclassWnd::WindowProc
$L74781:

; 98   : }

	pop	esi
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?WindowProc@CRichEditNcBorder@@MAEJPAUHWND__@@IIJ@Z ENDP ; CRichEditNcBorder::WindowProc
_TEXT	ENDS
PUBLIC	?GetCWnd@CSubclassWnd@@QBEPAVCWnd@@XZ		; CSubclassWnd::GetCWnd
PUBLIC	?HasStyle@CSubclassWnd@@QBEHK@Z			; CSubclassWnd::HasStyle
PUBLIC	?GetWindowRect@CSubclassWnd@@QBEXPAUtagRECT@@@Z	; CSubclassWnd::GetWindowRect
PUBLIC	?IsWindowEnabled@CSubclassWnd@@QBEHXZ		; CSubclassWnd::IsWindowEnabled
EXTRN	??0CWindowDC@@QAE@PAVCWnd@@@Z:NEAR		; CWindowDC::CWindowDC
EXTRN	??1CWindowDC@@UAE@XZ:NEAR			; CWindowDC::~CWindowDC
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	?Default@CSubclassWnd@@IAEJXZ:NEAR		; CSubclassWnd::Default
EXTRN	?ExcludeClipRect@CDC@@QAEHPBUtagRECT@@@Z:NEAR	; CDC::ExcludeClipRect
EXTRN	??0CRect@@QAE@XZ:NEAR				; CRect::CRect
EXTRN	??BCRect@@QAEPAUtagRECT@@XZ:NEAR		; CRect::operator tagRECT *
EXTRN	??0CThemed@@QAE@PAVCWnd@@PBG@Z:NEAR		; CThemed::CThemed
EXTRN	??1CThemed@@UAE@XZ:NEAR				; CThemed::~CThemed
EXTRN	?OffsetRect@CRect@@QAEXHH@Z:NEAR		; CRect::OffsetRect
EXTRN	?DrawBackground@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBUtagRECT@@@Z:NEAR ; CThemed::DrawBackground
EXTRN	?GetThemeBackgroundContentRect@CThemed@@QAEHPAVCDC@@HHABVCRect@@AAV3@@Z:NEAR ; CThemed::GetThemeBackgroundContentRect
;	COMDAT data
; File D:\_CODE\Shared\RichEditNcBorder.cpp
data	SEGMENT
$SG74800 DB	'E', 00H, 'd', 00H, 'i', 00H, 't', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T75000	DD	019930520H
	DD	02H
	DD	FLAT:$T75003
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75003	DD	0ffffffffH
	DD	FLAT:$L74995
	DD	00H
	DD	FLAT:$L74996
xdata$x	ENDS
;	COMDAT ?NcDrawBorder@CRichEditNcBorder@@IAEXXZ
_TEXT	SEGMENT
_this$ = -84
_th$74799 = -80
_dc$74801 = -64
_rBorder$74802 = -44
_rClient$74803 = -28
_nState$74804 = -68
__$EHRec$ = -12
?NcDrawBorder@CRichEditNcBorder@@IAEXXZ PROC NEAR	; CRichEditNcBorder::NcDrawBorder, COMDAT

; 101  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75001
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 72					; 00000048H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-84]
	mov	ecx, 18					; 00000012H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 102  : 	Default();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Default@CSubclassWnd@@IAEJXZ		; CSubclassWnd::Default

; 103  : 
; 104  : 	if (m_bThemedBorder)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+32], 0
	je	$L74798

; 106  : 		CThemed th(GetCWnd(), WC_EDIT);

	push	OFFSET FLAT:$SG74800
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCWnd@CSubclassWnd@@QBEPAVCWnd@@XZ	; CSubclassWnd::GetCWnd
	push	eax
	lea	ecx, DWORD PTR _th$74799[ebp]
	call	??0CThemed@@QAE@PAVCWnd@@PBG@Z		; CThemed::CThemed
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 107  : 		CWindowDC dc(GetCWnd());

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCWnd@CSubclassWnd@@QBEPAVCWnd@@XZ	; CSubclassWnd::GetCWnd
	push	eax
	lea	ecx, DWORD PTR _dc$74801[ebp]
	call	??0CWindowDC@@QAE@PAVCWnd@@@Z		; CWindowDC::CWindowDC
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 108  : 
; 109  : 		CRect rBorder, rClient;

	lea	ecx, DWORD PTR _rBorder$74802[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect
	lea	ecx, DWORD PTR _rClient$74803[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 110  : 		GetWindowRect(rBorder);

	lea	ecx, DWORD PTR _rBorder$74802[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetWindowRect@CSubclassWnd@@QBEXPAUtagRECT@@@Z ; CSubclassWnd::GetWindowRect

; 111  : 
; 112  : 		th.GetThemeBackgroundContentRect(&dc, EP_EDITTEXT, ETS_NORMAL, rBorder, rClient);

	lea	ecx, DWORD PTR _rClient$74803[ebp]
	push	ecx
	lea	edx, DWORD PTR _rBorder$74802[ebp]
	push	edx
	push	1
	push	1
	lea	eax, DWORD PTR _dc$74801[ebp]
	push	eax
	lea	ecx, DWORD PTR _th$74799[ebp]
	call	?GetThemeBackgroundContentRect@CThemed@@QAEHPAVCDC@@HHABVCRect@@AAV3@@Z ; CThemed::GetThemeBackgroundContentRect

; 113  : 		
; 114  : 		// convert to window coordinates
; 115  : 		rClient.OffsetRect(-rBorder.left, -rBorder.top);

	mov	ecx, DWORD PTR _rBorder$74802[ebp+4]
	neg	ecx
	push	ecx
	mov	edx, DWORD PTR _rBorder$74802[ebp]
	neg	edx
	push	edx
	lea	ecx, DWORD PTR _rClient$74803[ebp]
	call	?OffsetRect@CRect@@QAEXHH@Z		; CRect::OffsetRect

; 116  : 		rBorder.OffsetRect(-rBorder.left, -rBorder.top);

	mov	eax, DWORD PTR _rBorder$74802[ebp+4]
	neg	eax
	push	eax
	mov	ecx, DWORD PTR _rBorder$74802[ebp]
	neg	ecx
	push	ecx
	lea	ecx, DWORD PTR _rBorder$74802[ebp]
	call	?OffsetRect@CRect@@QAEXHH@Z		; CRect::OffsetRect

; 117  : 
; 118  : 		dc.ExcludeClipRect(rClient);

	lea	ecx, DWORD PTR _rClient$74803[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	lea	ecx, DWORD PTR _dc$74801[ebp]
	call	?ExcludeClipRect@CDC@@QAEHPBUtagRECT@@@Z ; CDC::ExcludeClipRect

; 119  : 
; 120  : 		// determine the current border state
; 121  : 		int nState;
; 122  : 
; 123  : 		if (!IsWindowEnabled())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsWindowEnabled@CSubclassWnd@@QBEHXZ	; CSubclassWnd::IsWindowEnabled
	test	eax, eax
	jne	SHORT $L74805

; 124  : 			nState = ETS_DISABLED;

	mov	DWORD PTR _nState$74804[ebp], 4

; 125  : 
; 126  : 		else if (HasStyle(ES_READONLY) || SendMessage(EM_GETOPTIONS, NULL, NULL) & ECO_READONLY)

	jmp	SHORT $L74809
$L74805:
	push	2048					; 00000800H
	mov	ecx, DWORD PTR _this$[ebp]
	call	?HasStyle@CSubclassWnd@@QBEHK@Z		; CSubclassWnd::HasStyle
	test	eax, eax
	jne	SHORT $L74808
	mov	esi, esp
	push	0
	push	0
	push	1102					; 0000044eH
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [eax+36]
	cmp	esi, esp
	call	__chkesp
	and	eax, 2048				; 00000800H
	test	eax, eax
	je	SHORT $L74807
$L74808:

; 127  : 			nState = ETS_READONLY;

	mov	DWORD PTR _nState$74804[ebp], 6

; 128  : 
; 129  : 		else

	jmp	SHORT $L74809
$L74807:

; 130  : 			nState = ETS_NORMAL;

	mov	DWORD PTR _nState$74804[ebp], 1
$L74809:

; 131  : 		
; 132  : 		th.DrawBackground(&dc, EP_EDITTEXT, nState, rBorder);

	push	0
	lea	ecx, DWORD PTR _rBorder$74802[ebp]
	push	ecx
	mov	edx, DWORD PTR _nState$74804[ebp]
	push	edx
	push	1
	lea	eax, DWORD PTR _dc$74801[ebp]
	push	eax
	lea	ecx, DWORD PTR _th$74799[ebp]
	call	?DrawBackground@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBUtagRECT@@@Z ; CThemed::DrawBackground

; 133  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _dc$74801[ebp]
	call	??1CWindowDC@@UAE@XZ			; CWindowDC::~CWindowDC
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _th$74799[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
$L74798:

; 134  : }

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
$L74995:
	lea	ecx, DWORD PTR _th$74799[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	ret	0
$L74996:
	lea	ecx, DWORD PTR _dc$74801[ebp]
	call	??1CWindowDC@@UAE@XZ			; CWindowDC::~CWindowDC
	ret	0
$L75001:
	mov	eax, OFFSET FLAT:$T75000
	jmp	___CxxFrameHandler
text$x	ENDS
?NcDrawBorder@CRichEditNcBorder@@IAEXXZ ENDP		; CRichEditNcBorder::NcDrawBorder
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
PUBLIC	?GetStyle@CSubclassWnd@@QBEKXZ			; CSubclassWnd::GetStyle
;	COMDAT ?HasStyle@CSubclassWnd@@QBEHK@Z
_TEXT	SEGMENT
_dwStyle$ = 8
_this$ = -4
?HasStyle@CSubclassWnd@@QBEHK@Z PROC NEAR		; CSubclassWnd::HasStyle, COMDAT

; 66   : 	inline BOOL HasStyle(DWORD dwStyle) const { return (GetStyle() & dwStyle) == dwStyle; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetStyle@CSubclassWnd@@QBEKXZ		; CSubclassWnd::GetStyle
	and	eax, DWORD PTR _dwStyle$[ebp]
	xor	ecx, ecx
	cmp	eax, DWORD PTR _dwStyle$[ebp]
	sete	cl
	mov	eax, ecx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?HasStyle@CSubclassWnd@@QBEHK@Z ENDP			; CSubclassWnd::HasStyle
_TEXT	ENDS
EXTRN	__imp__GetWindowLongW@8:NEAR
;	COMDAT ?GetStyle@CSubclassWnd@@QBEKXZ
_TEXT	SEGMENT
_this$ = -4
?GetStyle@CSubclassWnd@@QBEKXZ PROC NEAR		; CSubclassWnd::GetStyle, COMDAT

; 64   : 	inline DWORD GetStyle() const { return ::GetWindowLong(m_hWndHooked, GWL_STYLE); }

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esi, esp
	push	-16					; fffffff0H
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__GetWindowLongW@8
	cmp	esi, esp
	call	__chkesp
	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetStyle@CSubclassWnd@@QBEKXZ ENDP			; CSubclassWnd::GetStyle
_TEXT	ENDS
EXTRN	__imp__GetWindowRect@8:NEAR
;	COMDAT ?GetWindowRect@CSubclassWnd@@QBEXPAUtagRECT@@@Z
_TEXT	SEGMENT
_pRect$ = 8
_this$ = -4
?GetWindowRect@CSubclassWnd@@QBEXPAUtagRECT@@@Z PROC NEAR ; CSubclassWnd::GetWindowRect, COMDAT

; 70   : 	inline void GetWindowRect(LPRECT pRect) const { ::GetWindowRect(m_hWndHooked, pRect); }

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esi, esp
	mov	eax, DWORD PTR _pRect$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR __imp__GetWindowRect@8
	cmp	esi, esp
	call	__chkesp
	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetWindowRect@CSubclassWnd@@QBEXPAUtagRECT@@@Z ENDP	; CSubclassWnd::GetWindowRect
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
PUBLIC	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ		; CSubclassWnd::GetHwnd
PUBLIC	?HasExStyle@CSubclassWnd@@QBEHK@Z		; CSubclassWnd::HasExStyle
EXTRN	__imp__SetWindowPos@28:NEAR
EXTRN	?AreControlsThemed@CThemed@@SAHXZ:NEAR		; CThemed::AreControlsThemed
EXTRN	?ModifyStyle@CWnd@@SGHPAUHWND__@@KKI@Z:NEAR	; CWnd::ModifyStyle
EXTRN	?ModifyStyleEx@CWnd@@SGHPAUHWND__@@KKI@Z:NEAR	; CWnd::ModifyStyleEx
;	COMDAT xdata$x
; File D:\_CODE\Shared\RichEditNcBorder.cpp
xdata$x	SEGMENT
$T75024	DD	019930520H
	DD	01H
	DD	FLAT:$T75026
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75026	DD	0ffffffffH
	DD	FLAT:$L75022
xdata$x	ENDS
;	COMDAT ?VerifyThemedBorderState@CRichEditNcBorder@@IAEXXZ
_TEXT	SEGMENT
_this$ = -36
_bHadThemedBorder$ = -16
$T75020 = -20
$T75021 = -32
__$EHRec$ = -12
?VerifyThemedBorderState@CRichEditNcBorder@@IAEXXZ PROC NEAR ; CRichEditNcBorder::VerifyThemedBorderState, COMDAT

; 137  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75025
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
	mov	DWORD PTR _this$[ebp], ecx

; 138  : 	BOOL bHadThemedBorder = m_bThemedBorder;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+32]
	mov	DWORD PTR _bHadThemedBorder$[ebp], ecx

; 139  : 	m_bThemedBorder = FALSE;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+32], 0

; 140  : 	
; 141  : 	// First, check if the control is supposed to have a border
; 142  : 	if (bHadThemedBorder || HasStyle(WS_BORDER) || HasExStyle(WS_EX_CLIENTEDGE))

	cmp	DWORD PTR _bHadThemedBorder$[ebp], 0
	jne	SHORT $L74815
	push	8388608					; 00800000H
	mov	ecx, DWORD PTR _this$[ebp]
	call	?HasStyle@CSubclassWnd@@QBEHK@Z		; CSubclassWnd::HasStyle
	test	eax, eax
	jne	SHORT $L74815
	push	512					; 00000200H
	mov	ecx, DWORD PTR _this$[ebp]
	call	?HasExStyle@CSubclassWnd@@QBEHK@Z	; CSubclassWnd::HasExStyle
	test	eax, eax
	je	$L74819
$L74815:

; 144  : 		// Check if a theme is presently active
; 145  : 		if (CThemed().AreControlsThemed())

	push	0
	push	0
	lea	ecx, DWORD PTR $T75021[ebp]
	call	??0CThemed@@QAE@PAVCWnd@@PBG@Z		; CThemed::CThemed
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	call	?AreControlsThemed@CThemed@@SAHXZ	; CThemed::AreControlsThemed
	mov	DWORD PTR $T75020[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T75021[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	cmp	DWORD PTR $T75020[ebp], 0
	je	SHORT $L74817

; 147  : 			// Remove the border style, we don't want the control to draw its own border
; 148  : 			m_bThemedBorder = TRUE;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+32], 1

; 149  : 			
; 150  : 			CWnd::ModifyStyle(GetHwnd(), WS_BORDER, 0, 0);

	push	0
	push	0
	push	8388608					; 00800000H
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ	; CSubclassWnd::GetHwnd
	push	eax
	call	?ModifyStyle@CWnd@@SGHPAUHWND__@@KKI@Z	; CWnd::ModifyStyle

; 151  : 			CWnd::ModifyStyleEx(GetHwnd(), WS_EX_CLIENTEDGE, 0, 0);

	push	0
	push	0
	push	512					; 00000200H
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ	; CSubclassWnd::GetHwnd
	push	eax
	call	?ModifyStyleEx@CWnd@@SGHPAUHWND__@@KKI@Z ; CWnd::ModifyStyleEx

; 153  : 		else // restore the border

	jmp	SHORT $L74819
$L74817:

; 154  : 			CWnd::ModifyStyleEx(GetHwnd(), 0, WS_EX_CLIENTEDGE, 0);

	push	0
	push	512					; 00000200H
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ	; CSubclassWnd::GetHwnd
	push	eax
	call	?ModifyStyleEx@CWnd@@SGHPAUHWND__@@KKI@Z ; CWnd::ModifyStyleEx
$L74819:

; 156  : 
; 157  : 	// Recalculate the NC area and repaint the window
; 158  : 	SetWindowPos(GetHwnd(), NULL, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

	mov	esi, esp
	push	55					; 00000037H
	push	0
	push	0
	push	0
	push	0
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ	; CSubclassWnd::GetHwnd
	push	eax
	call	DWORD PTR __imp__SetWindowPos@28
	cmp	esi, esp
	call	__chkesp

; 159  : 	RedrawWindow(GetHwnd(), NULL, NULL, RDW_INVALIDATE | RDW_NOCHILDREN | RDW_UPDATENOW | RDW_FRAME);

	mov	esi, esp
	push	1345					; 00000541H
	push	0
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ	; CSubclassWnd::GetHwnd
	push	eax
	call	DWORD PTR __imp__RedrawWindow@16
	cmp	esi, esp
	call	__chkesp

; 160  : }

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
$L75022:
	lea	ecx, DWORD PTR $T75021[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	ret	0
$L75025:
	mov	eax, OFFSET FLAT:$T75024
	jmp	___CxxFrameHandler
text$x	ENDS
?VerifyThemedBorderState@CRichEditNcBorder@@IAEXXZ ENDP	; CRichEditNcBorder::VerifyThemedBorderState
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
PUBLIC	?GetExStyle@CSubclassWnd@@QBEKXZ		; CSubclassWnd::GetExStyle
;	COMDAT ?HasExStyle@CSubclassWnd@@QBEHK@Z
_TEXT	SEGMENT
_dwExStyle$ = 8
_this$ = -4
?HasExStyle@CSubclassWnd@@QBEHK@Z PROC NEAR		; CSubclassWnd::HasExStyle, COMDAT

; 65   : 	inline BOOL HasExStyle(DWORD dwExStyle) const { return (GetExStyle() & dwExStyle) == dwExStyle; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetExStyle@CSubclassWnd@@QBEKXZ	; CSubclassWnd::GetExStyle
	and	eax, DWORD PTR _dwExStyle$[ebp]
	xor	ecx, ecx
	cmp	eax, DWORD PTR _dwExStyle$[ebp]
	sete	cl
	mov	eax, ecx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?HasExStyle@CSubclassWnd@@QBEHK@Z ENDP			; CSubclassWnd::HasExStyle
_TEXT	ENDS
;	COMDAT ?GetExStyle@CSubclassWnd@@QBEKXZ
_TEXT	SEGMENT
_this$ = -4
?GetExStyle@CSubclassWnd@@QBEKXZ PROC NEAR		; CSubclassWnd::GetExStyle, COMDAT

; 63   : 	inline DWORD GetExStyle() const { return ::GetWindowLong(m_hWndHooked, GWL_EXSTYLE); }

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esi, esp
	push	-20					; ffffffecH
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__GetWindowLongW@8
	cmp	esi, esp
	call	__chkesp
	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetExStyle@CSubclassWnd@@QBEKXZ ENDP			; CSubclassWnd::GetExStyle
_TEXT	ENDS
END

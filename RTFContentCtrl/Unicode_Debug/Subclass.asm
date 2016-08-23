	TITLE	D:\_CODE\Shared\Subclass.cpp
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
_BSS	SEGMENT PARA USE32 PUBLIC 'BSS'
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
;	COMDAT ?GetWindowRect@CSubclassWnd@@QBEXPAUtagRECT@@@Z
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
;	COMDAT ??0CSubclassWnd@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCSubclassWnd@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CSubclassWnd@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?WindowProc@CSubclassWnd@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Default@CSubclassWnd@@IAEJXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetCurrentMessage@CSubclassWnd@@IAEPBUtagMSG@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HookWndProc@CSubclassWnd@@KGJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PostMessageW@CSubclassWnd@@MBEHIIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SendMessageW@CSubclassWnd@@MBEHIIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E270
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsValid@CSubclassWnd@@KAHPBV1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ClientToWindow@CSubclassWnd@@IBEXPAUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScreenToClient@CSubclassWnd@@IBEXPAUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ClientToScreen@CSubclassWnd@@IBEXPAUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScreenToWindow@CSubclassWnd@@IBEXPAUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ClientToWindow@CSubclassWnd@@IBEXPAUtagPOINT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScreenToWindow@CSubclassWnd@@IBEXPAUtagPOINT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScreenToClient@CSubclassWnd@@IBEXPAUtagPOINT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ClientToScreen@CSubclassWnd@@IBEXPAUtagPOINT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScDefault@CSubclasser@@IAEJPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CSubclassWndMap@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCSubclassWndMap@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CSubclassWndMap@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetHookMap@CSubclassWndMap@@SAAAV1@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E272
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Add@CSubclassWndMap@@QAEXPAUHWND__@@PAVCSubclassWnd@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Remove@CSubclassWndMap@@QAEXPAVCSubclassWnd@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@CSubclassWndMap@@QAEXPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lookup@CSubclassWndMap@@QAEPAVCSubclassWnd@@PAUHWND__@@@Z
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
;	COMDAT ??_7CSubclassWndMap@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CSubclassWnd@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CSubclasser@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?s_pCallback@CSubclassWnd@@1PAVISubclassCallback@@A ; CSubclassWnd::s_pCallback
_BSS	SEGMENT
?s_pCallback@CSubclassWnd@@1PAVISubclassCallback@@A DD 01H DUP (?) ; CSubclassWnd::s_pCallback
_BSS	ENDS
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\Shared\Subclass.cpp', 00H
_DATA	ENDS
PUBLIC	??0CSubclassWnd@@QAE@XZ				; CSubclassWnd::CSubclassWnd
PUBLIC	?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z ; CSubclassWnd::HookWindow
PUBLIC	?IsValid@CSubclassWnd@@UBEHXZ			; CSubclassWnd::IsValid
PUBLIC	?PreDetachWindow@CSubclassWnd@@MAEXXZ		; CSubclassWnd::PreDetachWindow
PUBLIC	?PostDetachWindow@CSubclassWnd@@MAEXXZ		; CSubclassWnd::PostDetachWindow
PUBLIC	?IsHooked@CSubclassWnd@@MBEHXZ			; CSubclassWnd::IsHooked
PUBLIC	?IsValidHook@CSubclassWnd@@MBEHXZ		; CSubclassWnd::IsValidHook
PUBLIC	?Redraw@CSubclassWnd@@MBEXXZ			; CSubclassWnd::Redraw
PUBLIC	?PostMessageW@CSubclassWnd@@MBEHIIJ@Z		; CSubclassWnd::PostMessageW
PUBLIC	?SendMessageW@CSubclassWnd@@MBEHIIJ@Z		; CSubclassWnd::SendMessageW
PUBLIC	?WindowProc@CSubclassWnd@@MAEJPAUHWND__@@IIJ@Z	; CSubclassWnd::WindowProc
PUBLIC	??_7CSubclassWnd@@6B@				; CSubclassWnd::`vftable'
PUBLIC	??_GCSubclassWnd@@UAEPAXI@Z			; CSubclassWnd::`scalar deleting destructor'
PUBLIC	??_ECSubclassWnd@@UAEPAXI@Z			; CSubclassWnd::`vector deleting destructor'
EXTRN	__chkesp:NEAR
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
;	COMDAT ??_7CSubclassWnd@@6B@
; File D:\_CODE\Shared\Subclass.cpp
CONST	SEGMENT
??_7CSubclassWnd@@6B@ DD FLAT:??_ECSubclassWnd@@UAEPAXI@Z ; CSubclassWnd::`vftable'
	DD	FLAT:?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z
	DD	FLAT:?IsValid@CSubclassWnd@@UBEHXZ
	DD	FLAT:?PreDetachWindow@CSubclassWnd@@MAEXXZ
	DD	FLAT:?PostDetachWindow@CSubclassWnd@@MAEXXZ
	DD	FLAT:?IsHooked@CSubclassWnd@@MBEHXZ
	DD	FLAT:?IsValidHook@CSubclassWnd@@MBEHXZ
	DD	FLAT:?Redraw@CSubclassWnd@@MBEXXZ
	DD	FLAT:?PostMessageW@CSubclassWnd@@MBEHIIJ@Z
	DD	FLAT:?SendMessageW@CSubclassWnd@@MBEHIIJ@Z
	DD	FLAT:?WindowProc@CSubclassWnd@@MAEJPAUHWND__@@IIJ@Z
CONST	ENDS
;	COMDAT ??0CSubclassWnd@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0CSubclassWnd@@QAE@XZ PROC NEAR			; CSubclassWnd::CSubclassWnd, COMDAT

; 46   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+20], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+24], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 28					; 0000001cH
	call	??0CString@@QAE@XZ			; CString::CString
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7CSubclassWnd@@6B@ ; CSubclassWnd::`vftable'

; 47   : 	m_pNext = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+12], 0

; 48   : 	m_pOldWndProc = NULL;	

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+8], 0

; 49   : 	m_hWndHooked  = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0

; 50   : 	m_pSubclasser = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+16], 0

; 51   : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CSubclassWnd@@QAE@XZ ENDP				; CSubclassWnd::CSubclassWnd
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
PUBLIC	??1CSubclassWnd@@UAE@XZ				; CSubclassWnd::~CSubclassWnd
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCSubclassWnd@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCSubclassWnd@@UAEPAXI@Z PROC NEAR			; CSubclassWnd::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CSubclassWnd@@UAE@XZ			; CSubclassWnd::~CSubclassWnd
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73381
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73381:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCSubclassWnd@@UAEPAXI@Z ENDP			; CSubclassWnd::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT xdata$x
; File D:\_CODE\Shared\Subclass.cpp
xdata$x	SEGMENT
$T73846	DD	019930520H
	DD	01H
	DD	FLAT:$T73849
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73849	DD	0ffffffffH
	DD	FLAT:$L73842
xdata$x	ENDS
;	COMDAT ??1CSubclassWnd@@UAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??1CSubclassWnd@@UAE@XZ PROC NEAR			; CSubclassWnd::~CSubclassWnd, COMDAT

; 54   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73847
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
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CSubclassWnd@@6B@ ; CSubclassWnd::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 55   : 	if (m_hWndHooked)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L73385

; 56   : 		HookWindow((HWND)NULL);

	push	0
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z ; CSubclassWnd::HookWindow
$L73385:

; 57   : 
; 58   : 	ASSERT(m_hWndHooked==NULL);		// can't destroy while still hooked!

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	je	SHORT $L73390
	push	58					; 0000003aH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73390
	int	3
$L73390:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73385
$L73389:

; 59   : 	ASSERT(m_pOldWndProc==NULL);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+8], 0
	je	SHORT $L73394
	push	59					; 0000003bH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73394
	int	3
$L73394:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L73389

; 60   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 28					; 0000001cH
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
$L73842:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 28					; 0000001cH
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73847:
	mov	eax, OFFSET FLAT:$T73846
	jmp	___CxxFrameHandler
text$x	ENDS
??1CSubclassWnd@@UAE@XZ ENDP				; CSubclassWnd::~CSubclassWnd
PUBLIC	?GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ ; CSubclassWnd::GetValidMap
PUBLIC	?GetHookMap@CSubclassWndMap@@SAAAV1@XZ		; CSubclassWndMap::GetHookMap
PUBLIC	?Add@CSubclassWndMap@@QAEXPAUHWND__@@PAVCSubclassWnd@@@Z ; CSubclassWndMap::Add
PUBLIC	?Remove@CSubclassWndMap@@QAEXPAVCSubclassWnd@@@Z ; CSubclassWndMap::Remove
EXTRN	?FromHandlePermanent@CWnd@@SGPAV1@PAUHWND__@@@Z:NEAR ; CWnd::FromHandlePermanent
EXTRN	?SubclassWindow@CWnd@@QAEHPAUHWND__@@@Z:NEAR	; CWnd::SubclassWindow
EXTRN	?UnsubclassWindow@CWnd@@QAEPAUHWND__@@XZ:NEAR	; CWnd::UnsubclassWindow
EXTRN	?SetAt@CMapPtrToPtr@@QAEXPAX0@Z:NEAR		; CMapPtrToPtr::SetAt
EXTRN	?RemoveKey@CMapPtrToPtr@@QAEHPAX@Z:NEAR		; CMapPtrToPtr::RemoveKey
EXTRN	?GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z:NEAR ; CWinClasses::GetClass
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
;	COMDAT xdata$x
; File D:\_CODE\Shared\Subclass.cpp
xdata$x	SEGMENT
$T73858	DD	019930520H
	DD	01H
	DD	FLAT:$T73860
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73860	DD	0ffffffffH
	DD	FLAT:$L73856
xdata$x	ENDS
;	COMDAT ?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z
_TEXT	SEGMENT
$T73855 = -20
__$EHRec$ = -12
_hWnd$ = 8
_pSubclasser$ = 12
_this$ = -24
_pPerm$73415 = -16
?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z PROC NEAR ; CSubclassWnd::HookWindow, COMDAT

; 69   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73859
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

; 70   : 	if (hWnd) 

	cmp	DWORD PTR _hWnd$[ebp], 0
	je	$L73400
$L73401:

; 72   : 		// Hook the window
; 73   : 		ASSERT(m_hWndHooked == NULL);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L73404
	push	73					; 00000049H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73404
	int	3
$L73404:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73401

; 74   : 
; 75   : 		if (m_hWndHooked) // only once

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	je	SHORT $L73405

; 76   : 			return FALSE;

	xor	eax, eax
	jmp	$L73399
$L73405:

; 77   : 
; 78   : 		ASSERT(::IsWindow(hWnd));

	mov	esi, esp
	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L73409
	push	78					; 0000004eH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73409
	int	3
$L73409:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73405

; 79   : 
; 80   : 		if (!::IsWindow(hWnd))

	mov	esi, esp
	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L73410

; 81   : 			return FALSE;

	xor	eax, eax
	jmp	$L73399
$L73410:

; 82   : 
; 83   : 		m_hWndHooked = hWnd;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _hWnd$[ebp]
	mov	DWORD PTR [eax+4], ecx

; 84   : 
; 85   : 		theHookMap.Add(m_hWndHooked, this);			// Add to map of hooks

	mov	edx, DWORD PTR _this$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	?GetHookMap@CSubclassWndMap@@SAAAV1@XZ	; CSubclassWndMap::GetHookMap
	mov	ecx, eax
	call	?Add@CSubclassWndMap@@QAEXPAUHWND__@@PAVCSubclassWnd@@@Z ; CSubclassWndMap::Add

; 86   : 		theSafeMap.SetAt((void*)this, NULL);

	push	0
	mov	edx, DWORD PTR _this$[ebp]
	push	edx
	call	?GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ ; CSubclassWnd::GetValidMap
	mov	ecx, eax
	call	?SetAt@CMapPtrToPtr@@QAEXPAX0@Z		; CMapPtrToPtr::SetAt

; 87   : 
; 88   : 		m_pSubclasser = pSubclasser;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _pSubclasser$[ebp]
	mov	DWORD PTR [eax+16], ecx

; 89   : 		m_bPreMFCSubclass = (CWnd::FromHandlePermanent(hWnd) == NULL);

	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	call	?FromHandlePermanent@CWnd@@SGPAV1@PAUHWND__@@@Z ; CWnd::FromHandlePermanent
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+20], eax

; 90   : 
; 91   : #ifdef _DEBUG
; 92   : 		m_sClass = CWinClasses::GetClass(hWnd);

	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	lea	eax, DWORD PTR $T73855[ebp]
	push	eax
	call	?GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z ; CWinClasses::GetClass
	add	esp, 8
	mov	DWORD PTR -28+[ebp], eax
	mov	ecx, DWORD PTR -28+[ebp]
	mov	DWORD PTR -32+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	edx, DWORD PTR -32+[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 28					; 0000001cH
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T73855[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 95   : 	else 

	jmp	$L73413
$L73400:

; 97   : 		// Unhook the window
; 98   : 		if (m_hWndHooked) 

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L73417

; 100  : 			// if we hooked this window PRIOR to MFC subclassing via DDX_Control,
; 101  : 			// then MFC will assert when it destructs the attached control because
; 102  : 			// the HWND does not get correctly detached presumably because the WndProc
; 103  : 			// has been mucked about by MFC. 
; 104  : 			// so we unsubclass and then resubclass to keep MFC happy, although
; 105  : 			// the app using this may need more care in the implementation of
; 106  : 			// their PreSubclassWindow() overrides
; 107  : 			CWnd* pPerm = CWnd::FromHandlePermanent(m_hWndHooked);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	?FromHandlePermanent@CWnd@@SGPAV1@PAUHWND__@@@Z ; CWnd::FromHandlePermanent
	mov	DWORD PTR _pPerm$73415[ebp], eax

; 108  : 
; 109  : 			if (m_bPreMFCSubclass && pPerm)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+20], 0
	je	SHORT $L73416
	cmp	DWORD PTR _pPerm$73415[ebp], 0
	je	SHORT $L73416

; 110  : 				pPerm->UnsubclassWindow();

	mov	ecx, DWORD PTR _pPerm$73415[ebp]
	call	?UnsubclassWindow@CWnd@@QAEPAUHWND__@@XZ ; CWnd::UnsubclassWindow
$L73416:

; 111  : 
; 112  : 			theHookMap.Remove(this);				// Remove from map

	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	?GetHookMap@CSubclassWndMap@@SAAAV1@XZ	; CSubclassWndMap::GetHookMap
	mov	ecx, eax
	call	?Remove@CSubclassWndMap@@QAEXPAVCSubclassWnd@@@Z ; CSubclassWndMap::Remove

; 113  : 
; 114  : 			if (m_bPreMFCSubclass && pPerm)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+20], 0
	je	SHORT $L73417
	cmp	DWORD PTR _pPerm$73415[ebp], 0
	je	SHORT $L73417

; 115  : 				pPerm->SubclassWindow(m_hWndHooked);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	mov	ecx, DWORD PTR _pPerm$73415[ebp]
	call	?SubclassWindow@CWnd@@QAEHPAUHWND__@@@Z	; CWnd::SubclassWindow
$L73417:

; 117  : 
; 118  : 		theSafeMap.RemoveKey((void*)this);

	mov	edx, DWORD PTR _this$[ebp]
	push	edx
	call	?GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ ; CSubclassWnd::GetValidMap
	mov	ecx, eax
	call	?RemoveKey@CMapPtrToPtr@@QAEHPAX@Z	; CMapPtrToPtr::RemoveKey

; 119  : 
; 120  : 		m_pOldWndProc = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+8], 0

; 121  : 		m_pSubclasser = NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+16], 0

; 122  : 		m_hWndHooked = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0

; 123  : 		m_pNext = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+12], 0

; 124  : 		m_bPreMFCSubclass = FALSE;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+20], 0
$L73413:

; 126  : 
; 127  : 	return TRUE;

	mov	eax, 1
$L73399:

; 128  : }

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
$L73856:
	lea	ecx, DWORD PTR $T73855[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73859:
	mov	eax, OFFSET FLAT:$T73858
	jmp	___CxxFrameHandler
text$x	ENDS
?HookWindow@CSubclassWnd@@UAEHPAUHWND__@@PAVCSubclasser@@@Z ENDP ; CSubclassWnd::HookWindow
EXTRN	__imp__CallWindowProcW@20:NEAR
EXTRN	?AfxTrace@@YAXPBGZZ:NEAR			; AfxTrace
;	COMDAT data
; File D:\_CODE\Shared\Subclass.cpp
data	SEGMENT
$SG73432 DB	'C', 00H, 'S', 00H, 'u', 00H, 'b', 00H, 'c', 00H, 'l', 00H
	DB	'a', 00H, 's', 00H, 's', 00H, 'W', 00H, 'n', 00H, 'd', 00H, ':'
	DB	00H, ':', 00H, 'W', 00H, 'i', 00H, 'n', 00H, 'd', 00H, 'o', 00H
	DB	'w', 00H, 'P', 00H, 'r', 00H, 'o', 00H, 'c', 00H, '(', 00H, 'm'
	DB	00H, 's', 00H, 'g', 00H, ' ', 00H, '=', 00H, ' ', 00H, '%', 00H
	DB	'd', 00H, ')', 00H, 0aH, 00H, 00H, 00H
data	ENDS
;	COMDAT ?WindowProc@CSubclassWnd@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT
_msg$ = 12
_wp$ = 16
_lp$ = 20
_this$ = -4
?WindowProc@CSubclassWnd@@MAEJPAUHWND__@@IIJ@Z PROC NEAR ; CSubclassWnd::WindowProc, COMDAT

; 139  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L73426:

; 140  : 	ASSERT (::IsWindow(m_hWndHooked));

	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L73429
	push	140					; 0000008cH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73429
	int	3
$L73429:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L73426

; 141  : 
; 142  : 	if (!::IsWindow(m_hWndHooked))

	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L73430

; 143  : 		return 0;

	xor	eax, eax
	jmp	$L73440
$L73430:

; 144  : 
; 145  : #ifdef _DEBUG
; 146  : 	if (m_bTracing)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+24], 0
	je	SHORT $L73431

; 147  : 		TRACE(_T("CSubclassWnd::WindowProc(msg = %d)\n"), msg);

	mov	eax, DWORD PTR _msg$[ebp]
	push	eax
	push	OFFSET FLAT:$SG73432
	call	?AfxTrace@@YAXPBGZZ			; AfxTrace
	add	esp, 8
$L73431:

; 148  : #endif
; 149  : 
; 150  : 	ASSERT(m_pOldWndProc);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+8], 0
	jne	SHORT $L73436
	push	150					; 00000096H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73436
	int	3
$L73436:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L73431

; 151  : 
; 152  : 	if (m_pNext)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	je	SHORT $L73437

; 154  : 		if (m_pNext->m_pSubclasser)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	cmp	DWORD PTR [edx+16], 0
	je	SHORT $L73438

; 155  : 			return m_pNext->m_pSubclasser->ScWindowProc(m_hWndHooked, msg, wp, lp);

	mov	esi, esp
	mov	eax, DWORD PTR _lp$[ebp]
	push	eax
	mov	ecx, DWORD PTR _wp$[ebp]
	push	ecx
	mov	edx, DWORD PTR _msg$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	mov	ecx, DWORD PTR [eax+16]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	mov	edx, DWORD PTR [eax+16]
	mov	eax, DWORD PTR [edx]
	call	DWORD PTR [eax]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L73440
$L73438:

; 157  : 			return m_pNext->WindowProc(m_hWndHooked, msg, wp, lp);

	mov	esi, esp
	mov	ecx, DWORD PTR _lp$[ebp]
	push	ecx
	mov	edx, DWORD PTR _wp$[ebp]
	push	edx
	mov	eax, DWORD PTR _msg$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	mov	edx, DWORD PTR [eax]
	call	DWORD PTR [edx+40]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L73440
$L73437:

; 160  : 		return ::CallWindowProc(m_pOldWndProc, m_hWndHooked, msg, wp, lp);

	mov	esi, esp
	mov	eax, DWORD PTR _lp$[ebp]
	push	eax
	mov	ecx, DWORD PTR _wp$[ebp]
	push	ecx
	mov	edx, DWORD PTR _msg$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	push	eax
	call	DWORD PTR __imp__CallWindowProcW@20
	cmp	esi, esp
	call	__chkesp
$L73440:

; 161  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?WindowProc@CSubclassWnd@@MAEJPAUHWND__@@IIJ@Z ENDP	; CSubclassWnd::WindowProc
_TEXT	ENDS
PUBLIC	?Default@CSubclassWnd@@IAEJXZ			; CSubclassWnd::Default
EXTRN	?AfxGetThreadState@@YGPAV_AFX_THREAD_STATE@@XZ:NEAR ; AfxGetThreadState
;	COMDAT ?Default@CSubclassWnd@@IAEJXZ
_TEXT	SEGMENT
_this$ = -8
_curMsg$ = -4
?Default@CSubclassWnd@@IAEJXZ PROC NEAR			; CSubclassWnd::Default, COMDAT

; 168  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L73444:

; 169  : 	ASSERT (::IsWindow(m_hWndHooked));

	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L73447
	push	169					; 000000a9H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73447
	int	3
$L73447:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L73444

; 170  : 
; 171  : 	if (!::IsWindow(m_hWndHooked))

	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L73448

; 172  : 		return 0;

	xor	eax, eax
	jmp	SHORT $L73443
$L73448:

; 173  : 
; 174  : 	// MFC stores current MSG in thread state
; 175  : 	MSG& curMsg = AfxGetThreadState()->m_lastSentMsg;

	call	?AfxGetThreadState@@YGPAV_AFX_THREAD_STATE@@XZ ; AfxGetThreadState
	add	eax, 52					; 00000034H
	mov	DWORD PTR _curMsg$[ebp], eax

; 176  : 
; 177  : 	// Note: must explicitly call CSubclassWnd::WindowProc to avoid infinite
; 178  : 	// recursion on virtual function
; 179  : 	return CSubclassWnd::WindowProc(m_hWndHooked, curMsg.message, curMsg.wParam, curMsg.lParam);

	mov	edx, DWORD PTR _curMsg$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR _curMsg$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	mov	eax, DWORD PTR _curMsg$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?WindowProc@CSubclassWnd@@MAEJPAUHWND__@@IIJ@Z ; CSubclassWnd::WindowProc
$L73443:

; 180  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Default@CSubclassWnd@@IAEJXZ ENDP			; CSubclassWnd::Default
_TEXT	ENDS
PUBLIC	?GetCurrentMessage@CSubclassWnd@@IAEPBUtagMSG@@XZ ; CSubclassWnd::GetCurrentMessage
_BSS	SEGMENT
_?curMsg@?1??GetCurrentMessage@CSubclassWnd@@IAEPBUtagMSG@@XZ@4U3@A DB 01cH DUP (?)
_BSS	ENDS
;	COMDAT ?GetCurrentMessage@CSubclassWnd@@IAEPBUtagMSG@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetCurrentMessage@CSubclassWnd@@IAEPBUtagMSG@@XZ PROC NEAR ; CSubclassWnd::GetCurrentMessage, COMDAT

; 183  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 184  : 	static MSG curMsg;
; 185  : 	
; 186  : 	curMsg = AfxGetThreadState()->m_lastSentMsg;

	call	?AfxGetThreadState@@YGPAV_AFX_THREAD_STATE@@XZ ; AfxGetThreadState
	mov	esi, eax
	add	esi, 52					; 00000034H
	mov	ecx, 7
	mov	edi, OFFSET FLAT:_?curMsg@?1??GetCurrentMessage@CSubclassWnd@@IAEPBUtagMSG@@XZ@4U3@A
	rep movsd

; 187  : 	return &curMsg;

	mov	eax, OFFSET FLAT:_?curMsg@?1??GetCurrentMessage@CSubclassWnd@@IAEPBUtagMSG@@XZ@4U3@A

; 188  : }

	pop	edi
	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetCurrentMessage@CSubclassWnd@@IAEPBUtagMSG@@XZ ENDP	; CSubclassWnd::GetCurrentMessage
_TEXT	ENDS
PUBLIC	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ		; CSubclassWnd::GetHwnd
PUBLIC	?HookWndProc@CSubclassWnd@@KGJPAUHWND__@@IIJ@Z	; CSubclassWnd::HookWndProc
PUBLIC	?RemoveAll@CSubclassWndMap@@QAEXPAUHWND__@@@Z	; CSubclassWndMap::RemoveAll
PUBLIC	?Lookup@CSubclassWndMap@@QAEPAVCSubclassWnd@@PAUHWND__@@@Z ; CSubclassWndMap::Lookup
EXTRN	?AfxGetStaticModuleState@@YGPAVAFX_MODULE_STATE@@XZ:NEAR ; AfxGetStaticModuleState
EXTRN	??0AFX_MAINTAIN_STATE2@@QAE@PAVAFX_MODULE_STATE@@@Z:NEAR ; AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2
EXTRN	??1AFX_MAINTAIN_STATE2@@QAE@XZ:NEAR		; AFX_MAINTAIN_STATE2::~AFX_MAINTAIN_STATE2
EXTRN	?Lookup@CMapPtrToPtr@@QBEHPAXAAPAX@Z:NEAR	; CMapPtrToPtr::Lookup
EXTRN	??ACMapPtrToPtr@@QAEAAPAXPAX@Z:NEAR		; CMapPtrToPtr::operator[]
EXTRN	__imp__GetClassNameW@12:NEAR
;	COMDAT xdata$x
; File D:\_CODE\Shared\Subclass.cpp
xdata$x	SEGMENT
$T73876	DD	019930520H
	DD	01H
	DD	FLAT:$T73878
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73878	DD	0ffffffffH
	DD	FLAT:$L73874
xdata$x	ENDS
;	COMDAT ?HookWndProc@CSubclassWnd@@KGJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT
$T73872 = -132
$T73873 = -136
__$EHRec$ = -12
_hwnd$ = 8
_msg$ = 12
_wp$ = 16
_lp$ = 20
__ctlState$ = -24
_curMsg$ = -16
_oldMsg$ = -60
_pSubclassWnd$ = -32
_bInHookWndProc$ = -64
_lr$ = -28
_szClass$73480 = -128
_wndproc$73481 = -68
?HookWndProc@CSubclassWnd@@KGJPAUHWND__@@IIJ@Z PROC NEAR ; CSubclassWnd::HookWndProc, COMDAT

; 195  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73877
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 124				; 0000007cH
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-136]
	mov	ecx, 31					; 0000001fH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 196  : 	// If this is a non-extension DLL, dynamically linked to MFC
; 197  : 	// then we need to set up MFC state
; 198  : #if defined(_WINDLL) && defined(_AFXDLL) && !defined(_AFXEXT)
; 199  : 	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	call	?AfxGetStaticModuleState@@YGPAVAFX_MODULE_STATE@@XZ ; AfxGetStaticModuleState
	push	eax
	lea	ecx, DWORD PTR __ctlState$[ebp]
	call	??0AFX_MAINTAIN_STATE2@@QAE@PAVAFX_MODULE_STATE@@@Z ; AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 200  : #endif
; 201  : 
; 202  : 	// Set up MFC message state just in case anyone wants it
; 203  : 	// This is just like AfxCallWindowProc, but we can't use that because
; 204  : 	// a CSubclassWnd is not a CWnd.
; 205  : 	//
; 206  : 	MSG& curMsg = AfxGetThreadState()->m_lastSentMsg;

	call	?AfxGetThreadState@@YGPAV_AFX_THREAD_STATE@@XZ ; AfxGetThreadState
	add	eax, 52					; 00000034H
	mov	DWORD PTR _curMsg$[ebp], eax

; 207  : 	MSG  oldMsg = curMsg;   // save for nesting

	mov	esi, DWORD PTR _curMsg$[ebp]
	mov	ecx, 7
	lea	edi, DWORD PTR _oldMsg$[ebp]
	rep movsd

; 208  : 	curMsg.hwnd		= hwnd;

	mov	eax, DWORD PTR _curMsg$[ebp]
	mov	ecx, DWORD PTR _hwnd$[ebp]
	mov	DWORD PTR [eax], ecx

; 209  : 	curMsg.message = msg;

	mov	edx, DWORD PTR _curMsg$[ebp]
	mov	eax, DWORD PTR _msg$[ebp]
	mov	DWORD PTR [edx+4], eax

; 210  : 	curMsg.wParam  = wp;

	mov	ecx, DWORD PTR _curMsg$[ebp]
	mov	edx, DWORD PTR _wp$[ebp]
	mov	DWORD PTR [ecx+8], edx

; 211  : 	curMsg.lParam  = lp;

	mov	eax, DWORD PTR _curMsg$[ebp]
	mov	ecx, DWORD PTR _lp$[ebp]
	mov	DWORD PTR [eax+12], ecx

; 212  : 
; 213  : 	// Get hook object for this window. Get from hook map
; 214  : 	CSubclassWnd* pSubclassWnd = theHookMap.Lookup(hwnd);

	mov	edx, DWORD PTR _hwnd$[ebp]
	push	edx
	call	?GetHookMap@CSubclassWndMap@@SAAAV1@XZ	; CSubclassWndMap::GetHookMap
	mov	ecx, eax
	call	?Lookup@CSubclassWndMap@@QAEPAVCSubclassWnd@@PAUHWND__@@@Z ; CSubclassWndMap::Lookup
	mov	DWORD PTR _pSubclassWnd$[ebp], eax

; 215  : 
; 216  : 	if (!pSubclassWnd)

	cmp	DWORD PTR _pSubclassWnd$[ebp], 0
	jne	SHORT $L73465

; 217  : 		return 0;

	mov	DWORD PTR $T73872[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR __ctlState$[ebp]
	call	??1AFX_MAINTAIN_STATE2@@QAE@XZ		; AFX_MAINTAIN_STATE2::~AFX_MAINTAIN_STATE2
	mov	eax, DWORD PTR $T73872[ebp]
	jmp	$L73460
$L73465:

; 218  : 
; 219  : 	// see if this is a re-entrant call
; 220  : 	BOOL bInHookWndProc = FALSE;

	mov	DWORD PTR _bInHookWndProc$[ebp], 0
$L73468:

; 221  : 
; 222  : 	VERIFY (theSafeMap.Lookup((void*)pSubclassWnd, (void*&)bInHookWndProc));

	lea	eax, DWORD PTR _bInHookWndProc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	push	ecx
	call	?GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ ; CSubclassWnd::GetValidMap
	mov	ecx, eax
	call	?Lookup@CMapPtrToPtr@@QBEHPAXAAPAX@Z	; CMapPtrToPtr::Lookup
	test	eax, eax
	jne	SHORT $L73473
	push	222					; 000000deH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73473
	int	3
$L73473:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L73468
$L73470:

; 223  : 	ASSERT (!bInHookWndProc);

	cmp	DWORD PTR _bInHookWndProc$[ebp], 0
	je	SHORT $L73477
	push	223					; 000000dfH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73477
	int	3
$L73477:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73470

; 224  : 
; 225  : 	LRESULT lr;
; 226  : 
; 227  : 	if (msg == WM_NCDESTROY) 

	cmp	DWORD PTR _msg$[ebp], 130		; 00000082H
	jne	SHORT $L73479

; 229  : #ifdef _DEBUG
; 230  : 		TCHAR szClass[30];
; 231  : 		GetClassName(hwnd, szClass, 30);

	mov	esi, esp
	push	30					; 0000001eH
	lea	ecx, DWORD PTR _szClass$73480[ebp]
	push	ecx
	mov	edx, DWORD PTR _hwnd$[ebp]
	push	edx
	call	DWORD PTR __imp__GetClassNameW@12
	cmp	esi, esp
	call	__chkesp

; 232  : //		TRACE ("CSubclassWnd::HookWndProc(%s, WM_NCDESTROY)\n", szClass);
; 233  : #endif
; 234  : 
; 235  : 		// Window is being destroyed: unhook all hooks (for this window)
; 236  : 		// and pass msg to orginal window proc
; 237  : 		WNDPROC wndproc = pSubclassWnd->m_pOldWndProc;

	mov	eax, DWORD PTR _pSubclassWnd$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR _wndproc$73481[ebp], ecx

; 238  : 		theHookMap.RemoveAll(hwnd);

	mov	edx, DWORD PTR _hwnd$[ebp]
	push	edx
	call	?GetHookMap@CSubclassWndMap@@SAAAV1@XZ	; CSubclassWndMap::GetHookMap
	mov	ecx, eax
	call	?RemoveAll@CSubclassWndMap@@QAEXPAUHWND__@@@Z ; CSubclassWndMap::RemoveAll

; 239  : 
; 240  : 		lr = ::CallWindowProc(wndproc, hwnd, msg, wp, lp);

	mov	esi, esp
	mov	eax, DWORD PTR _lp$[ebp]
	push	eax
	mov	ecx, DWORD PTR _wp$[ebp]
	push	ecx
	mov	edx, DWORD PTR _msg$[ebp]
	push	edx
	mov	eax, DWORD PTR _hwnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _wndproc$73481[ebp]
	push	ecx
	call	DWORD PTR __imp__CallWindowProcW@20
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lr$[ebp], eax

; 241  : 
; 242  : 		if (s_pCallback)

	cmp	DWORD PTR ?s_pCallback@CSubclassWnd@@1PAVISubclassCallback@@A, 0 ; CSubclassWnd::s_pCallback
	je	SHORT $L73482

; 243  : 			s_pCallback->PostNcDestroy(hwnd);

	mov	esi, esp
	mov	edx, DWORD PTR _hwnd$[ebp]
	push	edx
	mov	eax, DWORD PTR ?s_pCallback@CSubclassWnd@@1PAVISubclassCallback@@A ; CSubclassWnd::s_pCallback
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR ?s_pCallback@CSubclassWnd@@1PAVISubclassCallback@@A ; CSubclassWnd::s_pCallback
	call	DWORD PTR [edx]
	cmp	esi, esp
	call	__chkesp
$L73482:

; 245  : 	else 

	jmp	SHORT $L73485
$L73479:

; 247  : 		if (pSubclassWnd->m_pSubclasser)

	mov	eax, DWORD PTR _pSubclassWnd$[ebp]
	cmp	DWORD PTR [eax+16], 0
	je	SHORT $L73484

; 249  : 			// pass to msg hook
; 250  : 			lr = pSubclassWnd->m_pSubclasser->ScWindowProc(pSubclassWnd->GetHwnd(), msg, wp, lp);

	mov	esi, esp
	mov	ecx, DWORD PTR _lp$[ebp]
	push	ecx
	mov	edx, DWORD PTR _wp$[ebp]
	push	edx
	mov	eax, DWORD PTR _msg$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	call	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ	; CSubclassWnd::GetHwnd
	push	eax
	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	mov	ecx, DWORD PTR [ecx+16]
	mov	edx, DWORD PTR _pSubclassWnd$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	edx, DWORD PTR [eax]
	call	DWORD PTR [edx]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lr$[ebp], eax

; 252  : 		else

	jmp	SHORT $L73485
$L73484:

; 258  : 			// pass to msg hook
; 259  : 			lr = pSubclassWnd->WindowProc(pSubclassWnd->GetHwnd(), msg, wp, lp);

	mov	esi, esp
	mov	eax, DWORD PTR _lp$[ebp]
	push	eax
	mov	ecx, DWORD PTR _wp$[ebp]
	push	ecx
	mov	edx, DWORD PTR _msg$[ebp]
	push	edx
	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	call	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ	; CSubclassWnd::GetHwnd
	push	eax
	mov	eax, DWORD PTR _pSubclassWnd$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	call	DWORD PTR [edx+40]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lr$[ebp], eax
$L73485:

; 262  : 
; 263  : 	theSafeMap[(void*)pSubclassWnd] = NULL;

	mov	eax, DWORD PTR _pSubclassWnd$[ebp]
	push	eax
	call	?GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ ; CSubclassWnd::GetValidMap
	mov	ecx, eax
	call	??ACMapPtrToPtr@@QAEAAPAXPAX@Z		; CMapPtrToPtr::operator[]
	mov	DWORD PTR [eax], 0

; 264  : 
; 265  : 	curMsg = oldMsg;			// pop state

	mov	ecx, 7
	lea	esi, DWORD PTR _oldMsg$[ebp]
	mov	edi, DWORD PTR _curMsg$[ebp]
	rep movsd

; 266  : 	return lr;

	mov	ecx, DWORD PTR _lr$[ebp]
	mov	DWORD PTR $T73873[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR __ctlState$[ebp]
	call	??1AFX_MAINTAIN_STATE2@@QAE@XZ		; AFX_MAINTAIN_STATE2::~AFX_MAINTAIN_STATE2
	mov	eax, DWORD PTR $T73873[ebp]
$L73460:

; 267  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 136				; 00000088H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73874:
	lea	ecx, DWORD PTR __ctlState$[ebp]
	call	??1AFX_MAINTAIN_STATE2@@QAE@XZ		; AFX_MAINTAIN_STATE2::~AFX_MAINTAIN_STATE2
	ret	0
$L73877:
	mov	eax, OFFSET FLAT:$T73876
	jmp	___CxxFrameHandler
text$x	ENDS
?HookWndProc@CSubclassWnd@@KGJPAUHWND__@@IIJ@Z ENDP	; CSubclassWnd::HookWndProc
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
EXTRN	__imp__PostMessageW@16:NEAR
;	COMDAT ?PostMessageW@CSubclassWnd@@MBEHIIJ@Z
_TEXT	SEGMENT
_message$ = 8
_wParam$ = 12
_lParam$ = 16
_this$ = -4
?PostMessageW@CSubclassWnd@@MBEHIIJ@Z PROC NEAR		; CSubclassWnd::PostMessageW, COMDAT

; 270  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 271  : 	if (IsValid())

	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+8]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L73495

; 272  : 		return ::PostMessage(m_hWndHooked, message, wParam, lParam);

	mov	esi, esp
	mov	eax, DWORD PTR _lParam$[ebp]
	push	eax
	mov	ecx, DWORD PTR _wParam$[ebp]
	push	ecx
	mov	edx, DWORD PTR _message$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__PostMessageW@16
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L73494
$L73495:

; 273  : 
; 274  : 	// else
; 275  : 	return FALSE;

	xor	eax, eax
$L73494:

; 276  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?PostMessageW@CSubclassWnd@@MBEHIIJ@Z ENDP		; CSubclassWnd::PostMessageW
_TEXT	ENDS
EXTRN	__imp__SendMessageW@16:NEAR
;	COMDAT ?SendMessageW@CSubclassWnd@@MBEHIIJ@Z
_TEXT	SEGMENT
_message$ = 8
_wParam$ = 12
_lParam$ = 16
_this$ = -4
?SendMessageW@CSubclassWnd@@MBEHIIJ@Z PROC NEAR		; CSubclassWnd::SendMessageW, COMDAT

; 279  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 280  : 	if (IsValid())

	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+8]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L73502

; 281  : 		return ::SendMessage(m_hWndHooked, message, wParam, lParam);

	mov	esi, esp
	mov	eax, DWORD PTR _lParam$[ebp]
	push	eax
	mov	ecx, DWORD PTR _wParam$[ebp]
	push	ecx
	mov	edx, DWORD PTR _message$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L73501
$L73502:

; 282  : 
; 283  : 	// else
; 284  : 	return FALSE;

	xor	eax, eax
$L73501:

; 285  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?SendMessageW@CSubclassWnd@@MBEHIIJ@Z ENDP		; CSubclassWnd::SendMessageW
_TEXT	ENDS
EXTRN	??0CMapPtrToPtr@@QAE@H@Z:NEAR			; CMapPtrToPtr::CMapPtrToPtr
EXTRN	_atexit:NEAR
_BSS	SEGMENT
_?theMap@?1??GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ@4V3@A DB 01cH DUP (?)
_?$S269@?1??GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT ?GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ
_TEXT	SEGMENT
?GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ PROC NEAR ; CSubclassWnd::GetValidMap, COMDAT

; 288  : {

	push	ebp
	mov	ebp, esp

; 289  : 	// By creating theMap here, C++ doesn't instantiate it until/unless
; 290  : 	// it's ever used! This is a good trick to use in C++, to
; 291  : 	// instantiate/initialize a static object the first time it's used.
; 292  : 	//
; 293  : 	static CMapPtrToPtr theMap;

	xor	eax, eax
	mov	al, BYTE PTR _?$S269@?1??GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L73509
	mov	cl, BYTE PTR _?$S269@?1??GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ@4EA
	or	cl, 1
	mov	BYTE PTR _?$S269@?1??GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ@4EA, cl
	push	10					; 0000000aH
	mov	ecx, OFFSET FLAT:_?theMap@?1??GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ@4V3@A
	call	??0CMapPtrToPtr@@QAE@H@Z		; CMapPtrToPtr::CMapPtrToPtr
	push	OFFSET FLAT:_$E270
	call	_atexit
	add	esp, 4
$L73509:

; 294  : 	return theMap;

	mov	eax, OFFSET FLAT:_?theMap@?1??GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ@4V3@A

; 295  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ ENDP	; CSubclassWnd::GetValidMap
_TEXT	ENDS
EXTRN	??1CMapPtrToPtr@@UAE@XZ:NEAR			; CMapPtrToPtr::~CMapPtrToPtr
;	COMDAT _$E270
_TEXT	SEGMENT
_$E270	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?theMap@?1??GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ@4V3@A
	call	??1CMapPtrToPtr@@UAE@XZ			; CMapPtrToPtr::~CMapPtrToPtr
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E270	ENDP
_TEXT	ENDS
PUBLIC	?IsValid@CSubclassWnd@@KAHPBV1@@Z		; CSubclassWnd::IsValid
;	COMDAT ?IsValid@CSubclassWnd@@KAHPBV1@@Z
_TEXT	SEGMENT
_pScWnd$ = 8
_pResult$ = -4
?IsValid@CSubclassWnd@@KAHPBV1@@Z PROC NEAR		; CSubclassWnd::IsValid, COMDAT

; 298  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 299  : 	void* pResult;
; 300  : 
; 301  : 	return theSafeMap.Lookup((void*)pScWnd, pResult);

	lea	eax, DWORD PTR _pResult$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pScWnd$[ebp]
	push	ecx
	call	?GetValidMap@CSubclassWnd@@KAAAVCMapPtrToPtr@@XZ ; CSubclassWnd::GetValidMap
	mov	ecx, eax
	call	?Lookup@CMapPtrToPtr@@QBEHPAXAAPAX@Z	; CMapPtrToPtr::Lookup

; 302  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsValid@CSubclassWnd@@KAHPBV1@@Z ENDP			; CSubclassWnd::IsValid
_TEXT	ENDS
PUBLIC	?ClientToWindow@CSubclassWnd@@IBEXPAUtagRECT@@@Z ; CSubclassWnd::ClientToWindow
PUBLIC	?ClientToWindow@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ; CSubclassWnd::ClientToWindow
;	COMDAT ?ClientToWindow@CSubclassWnd@@IBEXPAUtagRECT@@@Z
_TEXT	SEGMENT
_pRect$ = 8
_this$ = -4
?ClientToWindow@CSubclassWnd@@IBEXPAUtagRECT@@@Z PROC NEAR ; CSubclassWnd::ClientToWindow, COMDAT

; 305  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 306  : 	ClientToWindow((LPPOINT)pRect);

	mov	eax, DWORD PTR _pRect$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ClientToWindow@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ; CSubclassWnd::ClientToWindow

; 307  : 	ClientToWindow(((LPPOINT)pRect) + 1);

	mov	ecx, DWORD PTR _pRect$[ebp]
	add	ecx, 8
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ClientToWindow@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ; CSubclassWnd::ClientToWindow

; 308  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ClientToWindow@CSubclassWnd@@IBEXPAUtagRECT@@@Z ENDP	; CSubclassWnd::ClientToWindow
_TEXT	ENDS
PUBLIC	?ScreenToClient@CSubclassWnd@@IBEXPAUtagRECT@@@Z ; CSubclassWnd::ScreenToClient
PUBLIC	?ScreenToClient@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ; CSubclassWnd::ScreenToClient
;	COMDAT ?ScreenToClient@CSubclassWnd@@IBEXPAUtagRECT@@@Z
_TEXT	SEGMENT
_pRect$ = 8
_this$ = -4
?ScreenToClient@CSubclassWnd@@IBEXPAUtagRECT@@@Z PROC NEAR ; CSubclassWnd::ScreenToClient, COMDAT

; 311  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 312  : 	ScreenToClient((LPPOINT)pRect);

	mov	eax, DWORD PTR _pRect$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ScreenToClient@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ; CSubclassWnd::ScreenToClient

; 313  : 	ScreenToClient(((LPPOINT)pRect) + 1);

	mov	ecx, DWORD PTR _pRect$[ebp]
	add	ecx, 8
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ScreenToClient@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ; CSubclassWnd::ScreenToClient

; 314  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ScreenToClient@CSubclassWnd@@IBEXPAUtagRECT@@@Z ENDP	; CSubclassWnd::ScreenToClient
_TEXT	ENDS
PUBLIC	?ClientToScreen@CSubclassWnd@@IBEXPAUtagRECT@@@Z ; CSubclassWnd::ClientToScreen
PUBLIC	?ClientToScreen@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ; CSubclassWnd::ClientToScreen
;	COMDAT ?ClientToScreen@CSubclassWnd@@IBEXPAUtagRECT@@@Z
_TEXT	SEGMENT
_pRect$ = 8
_this$ = -4
?ClientToScreen@CSubclassWnd@@IBEXPAUtagRECT@@@Z PROC NEAR ; CSubclassWnd::ClientToScreen, COMDAT

; 317  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 318  : 	ClientToScreen((LPPOINT)pRect);

	mov	eax, DWORD PTR _pRect$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ClientToScreen@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ; CSubclassWnd::ClientToScreen

; 319  : 	ClientToScreen(((LPPOINT)pRect) + 1);

	mov	ecx, DWORD PTR _pRect$[ebp]
	add	ecx, 8
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ClientToScreen@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ; CSubclassWnd::ClientToScreen

; 320  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ClientToScreen@CSubclassWnd@@IBEXPAUtagRECT@@@Z ENDP	; CSubclassWnd::ClientToScreen
_TEXT	ENDS
PUBLIC	?GetWindowRect@CSubclassWnd@@QBEXPAUtagRECT@@@Z	; CSubclassWnd::GetWindowRect
PUBLIC	?ScreenToWindow@CSubclassWnd@@IBEXPAUtagRECT@@@Z ; CSubclassWnd::ScreenToWindow
EXTRN	__imp__OffsetRect@12:NEAR
EXTRN	??0CRect@@QAE@XZ:NEAR				; CRect::CRect
EXTRN	??BCRect@@QAEPAUtagRECT@@XZ:NEAR		; CRect::operator tagRECT *
;	COMDAT ?ScreenToWindow@CSubclassWnd@@IBEXPAUtagRECT@@@Z
_TEXT	SEGMENT
_pRect$ = 8
_this$ = -20
_rWindow$ = -16
?ScreenToWindow@CSubclassWnd@@IBEXPAUtagRECT@@@Z PROC NEAR ; CSubclassWnd::ScreenToWindow, COMDAT

; 323  : {

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

; 324  : 	CRect rWindow;

	lea	ecx, DWORD PTR _rWindow$[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 325  : 	GetWindowRect(rWindow);

	lea	ecx, DWORD PTR _rWindow$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetWindowRect@CSubclassWnd@@QBEXPAUtagRECT@@@Z ; CSubclassWnd::GetWindowRect

; 326  : 	::OffsetRect(pRect, -rWindow.left, -rWindow.top);

	mov	eax, DWORD PTR _rWindow$[ebp+4]
	neg	eax
	mov	esi, esp
	push	eax
	mov	ecx, DWORD PTR _rWindow$[ebp]
	neg	ecx
	push	ecx
	mov	edx, DWORD PTR _pRect$[ebp]
	push	edx
	call	DWORD PTR __imp__OffsetRect@12
	cmp	esi, esp
	call	__chkesp

; 327  : }

	pop	esi
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ScreenToWindow@CSubclassWnd@@IBEXPAUtagRECT@@@Z ENDP	; CSubclassWnd::ScreenToWindow
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
PUBLIC	?ScreenToWindow@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ; CSubclassWnd::ScreenToWindow
;	COMDAT ?ClientToWindow@CSubclassWnd@@IBEXPAUtagPOINT@@@Z
_TEXT	SEGMENT
_pPoint$ = 8
_this$ = -4
?ClientToWindow@CSubclassWnd@@IBEXPAUtagPOINT@@@Z PROC NEAR ; CSubclassWnd::ClientToWindow, COMDAT

; 330  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 331  : 	ClientToScreen(pPoint);

	mov	eax, DWORD PTR _pPoint$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ClientToScreen@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ; CSubclassWnd::ClientToScreen

; 332  : 	ScreenToWindow(pPoint);

	mov	ecx, DWORD PTR _pPoint$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ScreenToWindow@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ; CSubclassWnd::ScreenToWindow

; 333  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ClientToWindow@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ENDP	; CSubclassWnd::ClientToWindow
_TEXT	ENDS
;	COMDAT ?ScreenToWindow@CSubclassWnd@@IBEXPAUtagPOINT@@@Z
_TEXT	SEGMENT
_pPoint$ = 8
_this$ = -20
_rWindow$ = -16
?ScreenToWindow@CSubclassWnd@@IBEXPAUtagPOINT@@@Z PROC NEAR ; CSubclassWnd::ScreenToWindow, COMDAT

; 336  : {

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

; 337  : 	CRect rWindow;

	lea	ecx, DWORD PTR _rWindow$[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 338  : 	GetWindowRect(rWindow);

	lea	ecx, DWORD PTR _rWindow$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetWindowRect@CSubclassWnd@@QBEXPAUtagRECT@@@Z ; CSubclassWnd::GetWindowRect

; 339  : 
; 340  : 	pPoint->x -= rWindow.left;

	mov	eax, DWORD PTR _pPoint$[ebp]
	mov	ecx, DWORD PTR [eax]
	sub	ecx, DWORD PTR _rWindow$[ebp]
	mov	edx, DWORD PTR _pPoint$[ebp]
	mov	DWORD PTR [edx], ecx

; 341  : 	pPoint->y -= rWindow.top;

	mov	eax, DWORD PTR _pPoint$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	sub	ecx, DWORD PTR _rWindow$[ebp+4]
	mov	edx, DWORD PTR _pPoint$[ebp]
	mov	DWORD PTR [edx+4], ecx

; 342  : }

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ScreenToWindow@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ENDP	; CSubclassWnd::ScreenToWindow
_TEXT	ENDS
EXTRN	__imp__ScreenToClient@8:NEAR
;	COMDAT ?ScreenToClient@CSubclassWnd@@IBEXPAUtagPOINT@@@Z
_TEXT	SEGMENT
_pPoint$ = 8
_this$ = -4
?ScreenToClient@CSubclassWnd@@IBEXPAUtagPOINT@@@Z PROC NEAR ; CSubclassWnd::ScreenToClient, COMDAT

; 345  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 346  : 	::ScreenToClient(m_hWndHooked, pPoint);

	mov	esi, esp
	mov	eax, DWORD PTR _pPoint$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR __imp__ScreenToClient@8
	cmp	esi, esp
	call	__chkesp

; 347  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ScreenToClient@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ENDP	; CSubclassWnd::ScreenToClient
_TEXT	ENDS
EXTRN	__imp__ClientToScreen@8:NEAR
;	COMDAT ?ClientToScreen@CSubclassWnd@@IBEXPAUtagPOINT@@@Z
_TEXT	SEGMENT
_pPoint$ = 8
_this$ = -4
?ClientToScreen@CSubclassWnd@@IBEXPAUtagPOINT@@@Z PROC NEAR ; CSubclassWnd::ClientToScreen, COMDAT

; 350  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 351  : 	::ClientToScreen(m_hWndHooked, pPoint);

	mov	esi, esp
	mov	eax, DWORD PTR _pPoint$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR __imp__ClientToScreen@8
	cmp	esi, esp
	call	__chkesp

; 352  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ClientToScreen@CSubclassWnd@@IBEXPAUtagPOINT@@@Z ENDP	; CSubclassWnd::ClientToScreen
_TEXT	ENDS
PUBLIC	?ScDefault@CSubclasser@@IAEJPAUHWND__@@@Z	; CSubclasser::ScDefault
;	COMDAT ?ScDefault@CSubclasser@@IAEJPAUHWND__@@@Z
_TEXT	SEGMENT
_hRealWnd$ = 8
_this$ = -16
_pSubclassWnd$ = -12
_bRestoreSC$ = -4
_lr$ = -8
?ScDefault@CSubclasser@@IAEJPAUHWND__@@@Z PROC NEAR	; CSubclasser::ScDefault, COMDAT

; 355  : { 

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

; 356  : 	if (hRealWnd == m_subclass.GetHwnd()) 

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ	; CSubclassWnd::GetHwnd
	cmp	DWORD PTR _hRealWnd$[ebp], eax
	jne	SHORT $L73561

; 357  : 		return m_subclass.Default(); 

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	?Default@CSubclassWnd@@IAEJXZ		; CSubclassWnd::Default
	jmp	$L73560
$L73561:

; 358  : 	
; 359  : 	// Get hook object for this window. Get from hook map
; 360  : 	CSubclassWnd* pSubclassWnd = theHookMap.Lookup(hRealWnd);

	mov	eax, DWORD PTR _hRealWnd$[ebp]
	push	eax
	call	?GetHookMap@CSubclassWndMap@@SAAAV1@XZ	; CSubclassWndMap::GetHookMap
	mov	ecx, eax
	call	?Lookup@CSubclassWndMap@@QAEPAVCSubclassWnd@@PAUHWND__@@@Z ; CSubclassWndMap::Lookup
	mov	DWORD PTR _pSubclassWnd$[ebp], eax
$L73563:

; 361  : 	ASSERT(pSubclassWnd);

	cmp	DWORD PTR _pSubclassWnd$[ebp], 0
	jne	SHORT $L73566
	push	361					; 00000169H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73566
	int	3
$L73566:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73563

; 362  : 
; 363  : 	BOOL bRestoreSC = FALSE;

	mov	DWORD PTR _bRestoreSC$[ebp], 0

; 364  : 
; 365  : 	if (pSubclassWnd->m_pNext && pSubclassWnd->m_pNext->m_pSubclasser == this)

	mov	edx, DWORD PTR _pSubclassWnd$[ebp]
	cmp	DWORD PTR [edx+12], 0
	je	SHORT $L73568
	mov	eax, DWORD PTR _pSubclassWnd$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	edx, DWORD PTR [ecx+16]
	cmp	edx, DWORD PTR _this$[ebp]
	jne	SHORT $L73568

; 367  : 		pSubclassWnd->m_pNext->m_pSubclasser = NULL;

	mov	eax, DWORD PTR _pSubclassWnd$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR [ecx+16], 0

; 368  : 		bRestoreSC = TRUE;

	mov	DWORD PTR _bRestoreSC$[ebp], 1
$L73568:

; 370  : 
; 371  : 	LRESULT lr = pSubclassWnd->Default();

	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	call	?Default@CSubclassWnd@@IAEJXZ		; CSubclassWnd::Default
	mov	DWORD PTR _lr$[ebp], eax

; 372  : 
; 373  : 	if (bRestoreSC)

	cmp	DWORD PTR _bRestoreSC$[ebp], 0
	je	SHORT $L73570

; 374  : 		pSubclassWnd->m_pNext->m_pSubclasser = this;

	mov	edx, DWORD PTR _pSubclassWnd$[ebp]
	mov	eax, DWORD PTR [edx+12]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+16], ecx
$L73570:

; 375  : 	
; 376  : 	return lr;

	mov	eax, DWORD PTR _lr$[ebp]
$L73560:

; 377  : } 

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ScDefault@CSubclasser@@IAEJPAUHWND__@@@Z ENDP		; CSubclasser::ScDefault
_TEXT	ENDS
PUBLIC	??0CSubclassWndMap@@QAE@XZ			; CSubclassWndMap::CSubclassWndMap
PUBLIC	??_7CSubclassWndMap@@6B@			; CSubclassWndMap::`vftable'
PUBLIC	??_GCSubclassWndMap@@UAEPAXI@Z			; CSubclassWndMap::`scalar deleting destructor'
PUBLIC	??_ECSubclassWndMap@@UAEPAXI@Z			; CSubclassWndMap::`vector deleting destructor'
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	?GetRuntimeClass@CMapPtrToPtr@@UBEPAUCRuntimeClass@@XZ:NEAR ; CMapPtrToPtr::GetRuntimeClass
EXTRN	?Dump@CMapPtrToPtr@@UBEXAAVCDumpContext@@@Z:NEAR ; CMapPtrToPtr::Dump
EXTRN	?AssertValid@CMapPtrToPtr@@UBEXXZ:NEAR		; CMapPtrToPtr::AssertValid
;	COMDAT ??_7CSubclassWndMap@@6B@
; File D:\_CODE\Shared\Subclass.cpp
CONST	SEGMENT
??_7CSubclassWndMap@@6B@ DD FLAT:?GetRuntimeClass@CMapPtrToPtr@@UBEPAUCRuntimeClass@@XZ ; CSubclassWndMap::`vftable'
	DD	FLAT:??_ECSubclassWndMap@@UAEPAXI@Z
	DD	FLAT:?Serialize@CObject@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@CMapPtrToPtr@@UBEXXZ
	DD	FLAT:?Dump@CMapPtrToPtr@@UBEXAAVCDumpContext@@@Z
CONST	ENDS
;	COMDAT ??0CSubclassWndMap@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0CSubclassWndMap@@QAE@XZ PROC NEAR			; CSubclassWndMap::CSubclassWndMap, COMDAT

; 383  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	10					; 0000000aH
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CMapPtrToPtr@@QAE@H@Z		; CMapPtrToPtr::CMapPtrToPtr
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CSubclassWndMap@@6B@ ; CSubclassWndMap::`vftable'

; 384  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CSubclassWndMap@@QAE@XZ ENDP				; CSubclassWndMap::CSubclassWndMap
_TEXT	ENDS
PUBLIC	??1CSubclassWndMap@@UAE@XZ			; CSubclassWndMap::~CSubclassWndMap
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_GCSubclassWndMap@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCSubclassWndMap@@UAEPAXI@Z PROC NEAR		; CSubclassWndMap::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CSubclassWndMap@@UAE@XZ		; CSubclassWndMap::~CSubclassWndMap
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73577
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L73577:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCSubclassWndMap@@UAEPAXI@Z ENDP			; CSubclassWndMap::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CSubclassWndMap@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CSubclassWndMap@@UAE@XZ PROC NEAR			; CSubclassWndMap::~CSubclassWndMap, COMDAT

; 387  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CSubclassWndMap@@6B@ ; CSubclassWndMap::`vftable'

; 388  : //	ASSERT(IsEmpty());	// all hooks should be removed!	
; 389  : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CMapPtrToPtr@@UAE@XZ			; CMapPtrToPtr::~CMapPtrToPtr
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CSubclassWndMap@@UAE@XZ ENDP				; CSubclassWndMap::~CSubclassWndMap
_TEXT	ENDS
_BSS	SEGMENT
	ALIGN	4

_?theMap@?1??GetHookMap@CSubclassWndMap@@SAAAV2@XZ@4V2@A DB 01cH DUP (?)
_?$S271@?1??GetHookMap@CSubclassWndMap@@SAAAV2@XZ@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT ?GetHookMap@CSubclassWndMap@@SAAAV1@XZ
_TEXT	SEGMENT
?GetHookMap@CSubclassWndMap@@SAAAV1@XZ PROC NEAR	; CSubclassWndMap::GetHookMap, COMDAT

; 395  : {

	push	ebp
	mov	ebp, esp

; 396  : 	// By creating theMap here, C++ doesn't instantiate it until/unless
; 397  : 	// it's ever used! This is a good trick to use in C++, to
; 398  : 	// instantiate/initialize a static object the first time it's used.
; 399  : 	//
; 400  : 	static CSubclassWndMap theMap;

	xor	eax, eax
	mov	al, BYTE PTR _?$S271@?1??GetHookMap@CSubclassWndMap@@SAAAV2@XZ@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L73587
	mov	cl, BYTE PTR _?$S271@?1??GetHookMap@CSubclassWndMap@@SAAAV2@XZ@4EA
	or	cl, 1
	mov	BYTE PTR _?$S271@?1??GetHookMap@CSubclassWndMap@@SAAAV2@XZ@4EA, cl
	mov	ecx, OFFSET FLAT:_?theMap@?1??GetHookMap@CSubclassWndMap@@SAAAV2@XZ@4V2@A
	call	??0CSubclassWndMap@@QAE@XZ		; CSubclassWndMap::CSubclassWndMap
	push	OFFSET FLAT:_$E272
	call	_atexit
	add	esp, 4
$L73587:

; 401  : 	return theMap;

	mov	eax, OFFSET FLAT:_?theMap@?1??GetHookMap@CSubclassWndMap@@SAAAV2@XZ@4V2@A

; 402  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?GetHookMap@CSubclassWndMap@@SAAAV1@XZ ENDP		; CSubclassWndMap::GetHookMap
_TEXT	ENDS
;	COMDAT _$E272
_TEXT	SEGMENT
_$E272	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?theMap@?1??GetHookMap@CSubclassWndMap@@SAAAV2@XZ@4V2@A
	call	??1CSubclassWndMap@@UAE@XZ		; CSubclassWndMap::~CSubclassWndMap
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E272	ENDP
_TEXT	ENDS
EXTRN	__imp__SetWindowLongW@12:NEAR
;	COMDAT ?Add@CSubclassWndMap@@QAEXPAUHWND__@@PAVCSubclassWnd@@@Z
_TEXT	SEGMENT
_hwnd$ = 8
_pSubclassWnd$ = 12
_this$ = -4
?Add@CSubclassWndMap@@QAEXPAUHWND__@@PAVCSubclassWnd@@@Z PROC NEAR ; CSubclassWndMap::Add, COMDAT

; 408  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L73595:

; 409  : 	ASSERT(hwnd && ::IsWindow(hwnd));

	cmp	DWORD PTR _hwnd$[ebp], 0
	je	SHORT $L73599
	mov	esi, esp
	mov	eax, DWORD PTR _hwnd$[ebp]
	push	eax
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L73598
$L73599:
	push	409					; 00000199H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73598
	int	3
$L73598:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73595

; 410  : 
; 411  : 	// Add to front of list
; 412  : 	pSubclassWnd->m_pNext = Lookup(hwnd);

	mov	edx, DWORD PTR _hwnd$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Lookup@CSubclassWndMap@@QAEPAVCSubclassWnd@@PAUHWND__@@@Z ; CSubclassWndMap::Lookup
	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	mov	DWORD PTR [ecx+12], eax

; 413  : 	SetAt(hwnd, pSubclassWnd);

	mov	edx, DWORD PTR _pSubclassWnd$[ebp]
	push	edx
	mov	eax, DWORD PTR _hwnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetAt@CMapPtrToPtr@@QAEXPAX0@Z		; CMapPtrToPtr::SetAt

; 414  : 	
; 415  : 	// If this is the first hook added, subclass the window
; 416  : 	if (pSubclassWnd->m_pNext == NULL) 

	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jne	SHORT $L73600

; 418  : 		//WNDPROC wndProc = (WNDPROC)GetWindowLong(hwnd, GWL_WNDPROC);
; 419  : 
; 420  : 		pSubclassWnd->m_pOldWndProc = 
; 421  : 			(WNDPROC)SetWindowLong(hwnd, GWL_WNDPROC, (DWORD)CSubclassWnd::HookWndProc);

	mov	esi, esp
	push	OFFSET FLAT:?HookWndProc@CSubclassWnd@@KGJPAUHWND__@@IIJ@Z ; CSubclassWnd::HookWndProc
	push	-4					; fffffffcH
	mov	edx, DWORD PTR _hwnd$[ebp]
	push	edx
	call	DWORD PTR __imp__SetWindowLongW@12
	cmp	esi, esp
	call	__chkesp
	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	mov	DWORD PTR [ecx+8], eax

; 423  : 	else // just copy wndproc from next hook

	jmp	SHORT $L73603
$L73600:

; 425  : 		
; 426  : 		pSubclassWnd->m_pOldWndProc = pSubclassWnd->m_pNext->m_pOldWndProc;

	mov	edx, DWORD PTR _pSubclassWnd$[ebp]
	mov	eax, DWORD PTR [edx+12]
	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	mov	edx, DWORD PTR [eax+8]
	mov	DWORD PTR [ecx+8], edx
$L73603:

; 428  : 	ASSERT(pSubclassWnd->m_pOldWndProc);

	mov	eax, DWORD PTR _pSubclassWnd$[ebp]
	cmp	DWORD PTR [eax+8], 0
	jne	SHORT $L73607
	push	428					; 000001acH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73607
	int	3
$L73607:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73603

; 429  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Add@CSubclassWndMap@@QAEXPAUHWND__@@PAVCSubclassWnd@@@Z ENDP ; CSubclassWndMap::Add
_TEXT	ENDS
;	COMDAT ?Remove@CSubclassWndMap@@QAEXPAVCSubclassWnd@@@Z
_TEXT	SEGMENT
_pUnHook$ = 8
_this$ = -12
_hwnd$ = -4
_pHook$ = -8
?Remove@CSubclassWndMap@@QAEXPAVCSubclassWnd@@@Z PROC NEAR ; CSubclassWndMap::Remove, COMDAT

; 435  : {

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

; 436  : 	HWND hwnd = pUnHook->m_hWndHooked;

	mov	eax, DWORD PTR _pUnHook$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR _hwnd$[ebp], ecx

; 437  : 	//ASSERT(hwnd && ::IsWindow(hwnd));
; 438  : 
; 439  : 	CSubclassWnd* pHook = Lookup(hwnd);

	mov	edx, DWORD PTR _hwnd$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Lookup@CSubclassWndMap@@QAEPAVCSubclassWnd@@PAUHWND__@@@Z ; CSubclassWndMap::Lookup
	mov	DWORD PTR _pHook$[ebp], eax

; 440  : //	ASSERT(pHook);
; 441  : 
; 442  : 	if (!pHook)

	cmp	DWORD PTR _pHook$[ebp], 0
	jne	SHORT $L73614

; 443  : 		return;

	jmp	$L73611
$L73614:

; 444  : 
; 445  : 	if (pHook == pUnHook) 

	mov	eax, DWORD PTR _pHook$[ebp]
	cmp	eax, DWORD PTR _pUnHook$[ebp]
	jne	SHORT $L73615

; 447  : 		// hook to remove is the one in the hash table: replace w/next
; 448  : 		if (pHook->m_pNext)

	mov	ecx, DWORD PTR _pHook$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L73616

; 449  : 			SetAt(hwnd, pHook->m_pNext);

	mov	edx, DWORD PTR _pHook$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR _hwnd$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetAt@CMapPtrToPtr@@QAEXPAX0@Z		; CMapPtrToPtr::SetAt

; 450  : 		else 

	jmp	SHORT $L73617
$L73616:

; 452  : 			// This is the last hook for this window: restore wnd proc
; 453  : 			RemoveKey(hwnd);

	mov	edx, DWORD PTR _hwnd$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveKey@CMapPtrToPtr@@QAEHPAX@Z	; CMapPtrToPtr::RemoveKey

; 454  : 			SetWindowLong(hwnd, GWL_WNDPROC, (DWORD)pHook->m_pOldWndProc);

	mov	esi, esp
	mov	eax, DWORD PTR _pHook$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	push	ecx
	push	-4					; fffffffcH
	mov	edx, DWORD PTR _hwnd$[ebp]
	push	edx
	call	DWORD PTR __imp__SetWindowLongW@12
	cmp	esi, esp
	call	__chkesp
$L73617:

; 457  : 	else 

	jmp	SHORT $L73623
$L73615:

; 459  : 		// Hook to remove is in the middle: just remove from linked list
; 460  : 		while (pHook->m_pNext && pHook->m_pNext != pUnHook)

	mov	eax, DWORD PTR _pHook$[ebp]
	cmp	DWORD PTR [eax+12], 0
	je	SHORT $L73622
	mov	ecx, DWORD PTR _pHook$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	cmp	edx, DWORD PTR _pUnHook$[ebp]
	je	SHORT $L73622

; 461  : 			pHook = pHook->m_pNext;

	mov	eax, DWORD PTR _pHook$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR _pHook$[ebp], ecx
	jmp	SHORT $L73615
$L73622:

; 462  : 
; 463  : 		if (pHook)

	cmp	DWORD PTR _pHook$[ebp], 0
	je	SHORT $L73623
$L73624:

; 465  : 			ASSERT(pHook->m_pNext == pUnHook);

	mov	edx, DWORD PTR _pHook$[ebp]
	mov	eax, DWORD PTR [edx+12]
	cmp	eax, DWORD PTR _pUnHook$[ebp]
	je	SHORT $L73627
	push	465					; 000001d1H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73627
	int	3
$L73627:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73624

; 466  : 			pHook->m_pNext = pUnHook->m_pNext;

	mov	edx, DWORD PTR _pHook$[ebp]
	mov	eax, DWORD PTR _pUnHook$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR [edx+12], ecx
$L73623:
$L73611:

; 469  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Remove@CSubclassWndMap@@QAEXPAVCSubclassWnd@@@Z ENDP	; CSubclassWndMap::Remove
_TEXT	ENDS
;	COMDAT ?RemoveAll@CSubclassWndMap@@QAEXPAUHWND__@@@Z
_TEXT	SEGMENT
_hwnd$ = 8
_this$ = -8
_pSubclassWnd$ = -4
?RemoveAll@CSubclassWndMap@@QAEXPAUHWND__@@@Z PROC NEAR	; CSubclassWndMap::RemoveAll, COMDAT

; 475  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L73634:

; 476  : 	CSubclassWnd* pSubclassWnd;
; 477  : 
; 478  : 	while ((pSubclassWnd = Lookup(hwnd))!=NULL)

	mov	eax, DWORD PTR _hwnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Lookup@CSubclassWndMap@@QAEPAVCSubclassWnd@@PAUHWND__@@@Z ; CSubclassWndMap::Lookup
	mov	DWORD PTR _pSubclassWnd$[ebp], eax
	cmp	DWORD PTR _pSubclassWnd$[ebp], 0
	je	$L73635

; 480  : 		if (pSubclassWnd->m_pSubclasser)

	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	cmp	DWORD PTR [ecx+16], 0
	je	SHORT $L73636

; 481  : 			pSubclassWnd->m_pSubclasser->ScPreDetachWindow();

	mov	edx, DWORD PTR _pSubclassWnd$[ebp]
	mov	ecx, DWORD PTR [edx+16]
	mov	eax, DWORD PTR _pSubclassWnd$[ebp]
	mov	edx, DWORD PTR [eax+16]
	mov	eax, DWORD PTR [edx]
	mov	esi, esp
	call	DWORD PTR [eax+8]
	cmp	esi, esp
	call	__chkesp

; 482  : 		else

	jmp	SHORT $L73637
$L73636:

; 483  : 			pSubclassWnd->PreDetachWindow();

	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	esi, esp
	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	call	DWORD PTR [edx+12]
	cmp	esi, esp
	call	__chkesp
$L73637:

; 484  : 
; 485  : 		pSubclassWnd->HookWindow((HWND)NULL);	// (unhook)

	mov	esi, esp
	push	0
	push	0
	mov	eax, DWORD PTR _pSubclassWnd$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp

; 486  : 
; 487  : 		if (pSubclassWnd->m_pSubclasser)

	mov	eax, DWORD PTR _pSubclassWnd$[ebp]
	cmp	DWORD PTR [eax+16], 0
	je	SHORT $L73639

; 488  : 			pSubclassWnd->m_pSubclasser->ScPostDetachWindow();

	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	mov	ecx, DWORD PTR [ecx+16]
	mov	edx, DWORD PTR _pSubclassWnd$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	call	DWORD PTR [edx+12]
	cmp	esi, esp
	call	__chkesp

; 489  : 		else

	jmp	SHORT $L73640
$L73639:

; 490  : 			pSubclassWnd->PostDetachWindow();

	mov	eax, DWORD PTR _pSubclassWnd$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR _pSubclassWnd$[ebp]
	call	DWORD PTR [edx+16]
	cmp	esi, esp
	call	__chkesp
$L73640:

; 491  : 	}

	jmp	$L73634
$L73635:

; 492  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?RemoveAll@CSubclassWndMap@@QAEXPAUHWND__@@@Z ENDP	; CSubclassWndMap::RemoveAll
_TEXT	ENDS
;	COMDAT ?Lookup@CSubclassWndMap@@QAEPAVCSubclassWnd@@PAUHWND__@@@Z
_TEXT	SEGMENT
_hwnd$ = 8
_this$ = -8
_pFound$ = -4
?Lookup@CSubclassWndMap@@QAEPAVCSubclassWnd@@PAUHWND__@@@Z PROC NEAR ; CSubclassWndMap::Lookup, COMDAT

; 498  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 499  : 	CSubclassWnd* pFound = NULL;

	mov	DWORD PTR _pFound$[ebp], 0

; 500  : 
; 501  : 	if (!CMapPtrToPtr::Lookup(hwnd, (void*&)pFound))

	lea	eax, DWORD PTR _pFound$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hwnd$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Lookup@CMapPtrToPtr@@QBEHPAXAAPAX@Z	; CMapPtrToPtr::Lookup
	test	eax, eax
	jne	SHORT $L73647

; 502  : 		return NULL;

	xor	eax, eax
	jmp	SHORT $L73644
$L73647:

; 503  : 
; 504  : //	ASSERT_KINDOF(CSubclassWnd, pFound);
; 505  : 
; 506  : 	return pFound;

	mov	eax, DWORD PTR _pFound$[ebp]
$L73644:

; 507  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Lookup@CSubclassWndMap@@QAEPAVCSubclassWnd@@PAUHWND__@@@Z ENDP ; CSubclassWndMap::Lookup
_TEXT	ENDS
END

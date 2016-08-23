	TITLE	D:\_CODE\Shared\HoldRedraw.cpp
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
;	COMDAT ?IsHeld@CHoldRedraw@@SAHPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E268
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E269
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E271
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E272
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CNcRedraw@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCNcRedraw@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CNcRedraw@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CRedrawAll@@QAE@PAUHWND__@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCRedrawAll@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CRedrawAll@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CHoldRedraw@@QAE@PAUHWND__@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCHoldRedraw@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CHoldRedraw@@UAE@XZ
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
;	COMDAT ??0?$CList@PAUHWND__@@AAPAU1@@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddTail@?$CList@PAUHWND__@@AAPAU1@@@QAEPAU__POSITION@@AAPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAt@?$CList@PAUHWND__@@AAPAU1@@@QAEXPAU__POSITION@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Find@?$CList@PAUHWND__@@AAPAU1@@@QBEPAU__POSITION@@AAPAUHWND__@@PAU2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CList@PAUHWND__@@AAPAU1@@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CList@PAUHWND__@@AAPAU1@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CList@PAUHWND__@@AAPAU1@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CList@PAUHWND__@@AAPAU1@@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CList@PAUHWND__@@AAPAU1@@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CList@PAUHWND__@@AAPAU1@@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetHeadPosition@?$CList@PAUHWND__@@AAPAU1@@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNext@?$CList@PAUHWND__@@AAPAU1@@@QAEAAPAUHWND__@@AAPAU__POSITION@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewNode@?$CList@PAUHWND__@@AAPAU1@@@IAEPAUCNode@1@PAU21@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FreeNode@?$CList@PAUHWND__@@AAPAU1@@@IAEXPAUCNode@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CompareElements@@YGHPBQAUHWND__@@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAPAUHWND__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBQAUHWND__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAPAUHWND__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAPAUHWND__@@H@Z
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
;	COMDAT ??_7?$CList@PAUHWND__@@AAPAU1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CHoldRedraw@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CRedrawAll@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CNcRedraw@@6B@
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
PUBLIC	?s_listHwnd@CHoldRedraw@@1V?$CList@PAUHWND__@@AAPAU1@@@A ; CHoldRedraw::s_listHwnd
_BSS	SEGMENT
?s_listHwnd@CHoldRedraw@@1V?$CList@PAUHWND__@@AAPAU1@@@A DB 01cH DUP (?) ; CHoldRedraw::s_listHwnd
_BSS	ENDS
_DATA	SEGMENT
__szAfxTempl DB	'afxtempl.h', 00H
_DATA	ENDS
CRT$XCU	SEGMENT
_$S273	DD	FLAT:_$E272
CRT$XCU	ENDS
EXTRN	__chkesp:NEAR
;	COMDAT _$E272
_TEXT	SEGMENT
_$E272	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E268
	call	_$E271
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E272	ENDP
_TEXT	ENDS
PUBLIC	??0?$CList@PAUHWND__@@AAPAU1@@@QAE@H@Z		; CList<HWND__ *,HWND__ * &>::CList<HWND__ *,HWND__ * &>
;	COMDAT _$E268
_TEXT	SEGMENT
_$E268	PROC NEAR					; COMDAT

; 8    : CList<HWND, HWND&> CHoldRedraw::s_listHwnd;

	push	ebp
	mov	ebp, esp
	push	10					; 0000000aH
	mov	ecx, OFFSET FLAT:?s_listHwnd@CHoldRedraw@@1V?$CList@PAUHWND__@@AAPAU1@@@A
	call	??0?$CList@PAUHWND__@@AAPAU1@@@QAE@H@Z	; CList<HWND__ *,HWND__ * &>::CList<HWND__ *,HWND__ * &>
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E268	ENDP
_TEXT	ENDS
EXTRN	_atexit:NEAR
;	COMDAT _$E271
_TEXT	SEGMENT
_$E271	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	push	OFFSET FLAT:_$E269
	call	_atexit
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E271	ENDP
_TEXT	ENDS
PUBLIC	??1?$CList@PAUHWND__@@AAPAU1@@@UAE@XZ		; CList<HWND__ *,HWND__ * &>::~CList<HWND__ *,HWND__ * &>
_BSS	SEGMENT
_$S270	DB	01H DUP (?)
_BSS	ENDS
;	COMDAT _$E269
_TEXT	SEGMENT
_$E269	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	xor	eax, eax
	mov	al, BYTE PTR _$S270
	and	eax, 1
	test	eax, eax
	jne	SHORT $L73943
	mov	cl, BYTE PTR _$S270
	or	cl, 1
	mov	BYTE PTR _$S270, cl
	mov	ecx, OFFSET FLAT:?s_listHwnd@CHoldRedraw@@1V?$CList@PAUHWND__@@AAPAU1@@@A
	call	??1?$CList@PAUHWND__@@AAPAU1@@@UAE@XZ	; CList<HWND__ *,HWND__ * &>::~CList<HWND__ *,HWND__ * &>
$L73943:
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E269	ENDP
_TEXT	ENDS
PUBLIC	??0CNcRedraw@@QAE@PAUHWND__@@@Z			; CNcRedraw::CNcRedraw
PUBLIC	??_7CNcRedraw@@6B@				; CNcRedraw::`vftable'
PUBLIC	??_GCNcRedraw@@UAEPAXI@Z			; CNcRedraw::`scalar deleting destructor'
PUBLIC	??_ECNcRedraw@@UAEPAXI@Z			; CNcRedraw::`vector deleting destructor'
;	COMDAT ??_7CNcRedraw@@6B@
; File D:\_CODE\Shared\HoldRedraw.cpp
CONST	SEGMENT
??_7CNcRedraw@@6B@ DD FLAT:??_ECNcRedraw@@UAEPAXI@Z	; CNcRedraw::`vftable'
CONST	ENDS
;	COMDAT ??0CNcRedraw@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
_this$ = -4
??0CNcRedraw@@QAE@PAUHWND__@@@Z PROC NEAR		; CNcRedraw::CNcRedraw, COMDAT

; 13   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _hWnd$[ebp]
	mov	DWORD PTR [eax+4], ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7CNcRedraw@@6B@ ; CNcRedraw::`vftable'

; 14   : }

	mov	eax, DWORD PTR _this$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	4
??0CNcRedraw@@QAE@PAUHWND__@@@Z ENDP			; CNcRedraw::CNcRedraw
_TEXT	ENDS
PUBLIC	??1CNcRedraw@@UAE@XZ				; CNcRedraw::~CNcRedraw
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCNcRedraw@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCNcRedraw@@UAEPAXI@Z PROC NEAR			; CNcRedraw::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CNcRedraw@@UAE@XZ			; CNcRedraw::~CNcRedraw
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73961
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73961:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCNcRedraw@@UAEPAXI@Z ENDP				; CNcRedraw::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	?IsHeld@CHoldRedraw@@SAHPAUHWND__@@@Z		; CHoldRedraw::IsHeld
EXTRN	__imp__SendMessageW@16:NEAR
EXTRN	__imp__IsWindowVisible@4:NEAR
;	COMDAT ??1CNcRedraw@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CNcRedraw@@UAE@XZ PROC NEAR				; CNcRedraw::~CNcRedraw, COMDAT

; 17   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CNcRedraw@@6B@ ; CNcRedraw::`vftable'

; 18   : 	if (m_hWnd && ::IsWindowVisible(m_hWnd) && !CHoldRedraw::IsHeld(m_hWnd))

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L73965
	mov	esi, esp
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	DWORD PTR __imp__IsWindowVisible@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L73965
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	?IsHeld@CHoldRedraw@@SAHPAUHWND__@@@Z	; CHoldRedraw::IsHeld
	add	esp, 4
	test	eax, eax
	jne	SHORT $L73965

; 20   : 		::SendMessage(m_hWnd, WM_NCPAINT, 0, 0);

	mov	esi, esp
	push	0
	push	0
	push	133					; 00000085H
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp
$L73965:

; 22   : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CNcRedraw@@UAE@XZ ENDP				; CNcRedraw::~CNcRedraw
_TEXT	ENDS
PUBLIC	?Find@?$CList@PAUHWND__@@AAPAU1@@@QBEPAU__POSITION@@AAPAUHWND__@@PAU2@@Z ; CList<HWND__ *,HWND__ * &>::Find
;	COMDAT ?IsHeld@CHoldRedraw@@SAHPAUHWND__@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
?IsHeld@CHoldRedraw@@SAHPAUHWND__@@@Z PROC NEAR		; CHoldRedraw::IsHeld, COMDAT

; 44   : 	static BOOL IsHeld(HWND hWnd) { return (s_listHwnd.Find(hWnd) != NULL); }

	push	ebp
	mov	ebp, esp
	push	0
	lea	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	mov	ecx, OFFSET FLAT:?s_listHwnd@CHoldRedraw@@1V?$CList@PAUHWND__@@AAPAU1@@@A
	call	?Find@?$CList@PAUHWND__@@AAPAU1@@@QBEPAU__POSITION@@AAPAUHWND__@@PAU2@@Z ; CList<HWND__ *,HWND__ * &>::Find
	neg	eax
	sbb	eax, eax
	neg	eax
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?IsHeld@CHoldRedraw@@SAHPAUHWND__@@@Z ENDP		; CHoldRedraw::IsHeld
_TEXT	ENDS
PUBLIC	??0CRedrawAll@@QAE@PAUHWND__@@K@Z		; CRedrawAll::CRedrawAll
PUBLIC	??_7CRedrawAll@@6B@				; CRedrawAll::`vftable'
PUBLIC	??_GCRedrawAll@@UAEPAXI@Z			; CRedrawAll::`scalar deleting destructor'
PUBLIC	??_ECRedrawAll@@UAEPAXI@Z			; CRedrawAll::`vector deleting destructor'
;	COMDAT ??_7CRedrawAll@@6B@
; File D:\_CODE\Shared\HoldRedraw.cpp
CONST	SEGMENT
??_7CRedrawAll@@6B@ DD FLAT:??_ECRedrawAll@@UAEPAXI@Z	; CRedrawAll::`vftable'
CONST	ENDS
;	COMDAT ??0CRedrawAll@@QAE@PAUHWND__@@K@Z
_TEXT	SEGMENT
_hWnd$ = 8
_dwFlags$ = 12
_this$ = -4
??0CRedrawAll@@QAE@PAUHWND__@@K@Z PROC NEAR		; CRedrawAll::CRedrawAll, COMDAT

; 27   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _hWnd$[ebp]
	mov	DWORD PTR [eax+4], ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _dwFlags$[ebp]
	mov	DWORD PTR [edx+8], eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CRedrawAll@@6B@ ; CRedrawAll::`vftable'

; 28   : }

	mov	eax, DWORD PTR _this$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	8
??0CRedrawAll@@QAE@PAUHWND__@@K@Z ENDP			; CRedrawAll::CRedrawAll
_TEXT	ENDS
PUBLIC	??1CRedrawAll@@UAE@XZ				; CRedrawAll::~CRedrawAll
;	COMDAT ??_GCRedrawAll@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCRedrawAll@@UAEPAXI@Z PROC NEAR			; CRedrawAll::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CRedrawAll@@UAE@XZ			; CRedrawAll::~CRedrawAll
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73974
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73974:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCRedrawAll@@UAEPAXI@Z ENDP				; CRedrawAll::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	__imp__UpdateWindow@4:NEAR
EXTRN	__imp__InvalidateRect@12:NEAR
;	COMDAT ??1CRedrawAll@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CRedrawAll@@UAE@XZ PROC NEAR				; CRedrawAll::~CRedrawAll, COMDAT

; 31   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CRedrawAll@@6B@ ; CRedrawAll::`vftable'

; 32   : 	if (m_hWnd && ::IsWindowVisible(m_hWnd))

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	$L73981
	mov	esi, esp
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	DWORD PTR __imp__IsWindowVisible@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L73981

; 34   : 		if (m_dwFlags & NCR_PAINT)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	and	edx, 2
	test	edx, edx
	je	SHORT $L73980

; 36   : 			::InvalidateRect(m_hWnd, NULL, FALSE);

	mov	esi, esp
	push	0
	push	0
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__InvalidateRect@12
	cmp	esi, esp
	call	__chkesp

; 37   : 			
; 38   : 			if (m_dwFlags & NCR_UPDATE)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73980

; 39   : 				::UpdateWindow(m_hWnd);

	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR __imp__UpdateWindow@4
	cmp	esi, esp
	call	__chkesp
$L73980:

; 41   : 		
; 42   : 		if (m_dwFlags & NCR_NCPAINT)

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	and	ecx, 4
	test	ecx, ecx
	je	SHORT $L73981

; 43   : 			::SendMessage(m_hWnd, WM_NCPAINT, 0, 0);

	mov	esi, esp
	push	0
	push	0
	push	133					; 00000085H
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp
$L73981:

; 45   : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CRedrawAll@@UAE@XZ ENDP				; CRedrawAll::~CRedrawAll
_TEXT	ENDS
PUBLIC	??0CHoldRedraw@@QAE@PAUHWND__@@K@Z		; CHoldRedraw::CHoldRedraw
PUBLIC	?AddTail@?$CList@PAUHWND__@@AAPAU1@@@QAEPAU__POSITION@@AAPAUHWND__@@@Z ; CList<HWND__ *,HWND__ * &>::AddTail
PUBLIC	??_7CHoldRedraw@@6B@				; CHoldRedraw::`vftable'
PUBLIC	??_GCHoldRedraw@@UAEPAXI@Z			; CHoldRedraw::`scalar deleting destructor'
PUBLIC	??_ECHoldRedraw@@UAEPAXI@Z			; CHoldRedraw::`vector deleting destructor'
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
;	COMDAT ??_7CHoldRedraw@@6B@
; File D:\_CODE\Shared\HoldRedraw.cpp
CONST	SEGMENT
??_7CHoldRedraw@@6B@ DD FLAT:??_ECHoldRedraw@@UAEPAXI@Z	; CHoldRedraw::`vftable'
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74480	DD	019930520H
	DD	01H
	DD	FLAT:$T74483
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74483	DD	0ffffffffH
	DD	FLAT:$L74476
xdata$x	ENDS
;	COMDAT ??0CHoldRedraw@@QAE@PAUHWND__@@K@Z
_TEXT	SEGMENT
_hWnd$ = 8
_dwFlags$ = 12
_this$ = -20
_pos$73988 = -16
__$EHRec$ = -12
??0CHoldRedraw@@QAE@PAUHWND__@@K@Z PROC NEAR		; CHoldRedraw::CHoldRedraw, COMDAT

; 50   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74481
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _dwFlags$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CRedrawAll@@QAE@PAUHWND__@@K@Z	; CRedrawAll::CRedrawAll
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7CHoldRedraw@@6B@ ; CHoldRedraw::`vftable'

; 51   : 	if (m_hWnd)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L73990

; 53   : 		// check if there is already a hold on this window
; 54   : 		POSITION pos = s_listHwnd.Find(m_hWnd);

	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	push	ecx
	mov	ecx, OFFSET FLAT:?s_listHwnd@CHoldRedraw@@1V?$CList@PAUHWND__@@AAPAU1@@@A
	call	?Find@?$CList@PAUHWND__@@AAPAU1@@@QBEPAU__POSITION@@AAPAUHWND__@@PAU2@@Z ; CList<HWND__ *,HWND__ * &>::Find
	mov	DWORD PTR _pos$73988[ebp], eax

; 55   : 		
; 56   : 		if (pos) // found

	cmp	DWORD PTR _pos$73988[ebp], 0
	je	SHORT $L73989

; 58   : 			m_hWnd = NULL; // prevent another hold being taken

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0

; 60   : 		else

	jmp	SHORT $L73990
$L73989:

; 62   : 			s_listHwnd.AddTail(m_hWnd);

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 4
	push	eax
	mov	ecx, OFFSET FLAT:?s_listHwnd@CHoldRedraw@@1V?$CList@PAUHWND__@@AAPAU1@@@A
	call	?AddTail@?$CList@PAUHWND__@@AAPAU1@@@QAEPAU__POSITION@@AAPAUHWND__@@@Z ; CList<HWND__ *,HWND__ * &>::AddTail

; 63   : 			::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);

	mov	esi, esp
	push	0
	push	0
	push	11					; 0000000bH
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp
$L73990:

; 66   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74476:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CRedrawAll@@UAE@XZ			; CRedrawAll::~CRedrawAll
	ret	0
$L74481:
	mov	eax, OFFSET FLAT:$T74480
	jmp	___CxxFrameHandler
text$x	ENDS
??0CHoldRedraw@@QAE@PAUHWND__@@K@Z ENDP			; CHoldRedraw::CHoldRedraw
PUBLIC	??1CHoldRedraw@@UAE@XZ				; CHoldRedraw::~CHoldRedraw
;	COMDAT ??_GCHoldRedraw@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCHoldRedraw@@UAEPAXI@Z PROC NEAR			; CHoldRedraw::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CHoldRedraw@@UAE@XZ			; CHoldRedraw::~CHoldRedraw
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73994
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73994:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCHoldRedraw@@UAEPAXI@Z ENDP				; CHoldRedraw::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	?RemoveAt@?$CList@PAUHWND__@@AAPAU1@@@QAEXPAU__POSITION@@@Z ; CList<HWND__ *,HWND__ * &>::RemoveAt
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
;	COMDAT data
; File D:\_CODE\Shared\HoldRedraw.cpp
data	SEGMENT
$SG74004 DB	'D:\_CODE\Shared\HoldRedraw.cpp', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74493	DD	019930520H
	DD	01H
	DD	FLAT:$T74495
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74495	DD	0ffffffffH
	DD	FLAT:$L74491
xdata$x	ENDS
;	COMDAT ??1CHoldRedraw@@UAE@XZ
_TEXT	SEGMENT
_this$ = -20
_pos$73999 = -16
__$EHRec$ = -12
??1CHoldRedraw@@UAE@XZ PROC NEAR			; CHoldRedraw::~CHoldRedraw, COMDAT

; 69   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74494
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CHoldRedraw@@6B@ ; CHoldRedraw::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 70   : 	if (m_hWnd)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L73998

; 72   : 		::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);

	mov	esi, esp
	push	0
	push	1
	push	11					; 0000000bH
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 73   : 		
; 74   : 		// remove from list
; 75   : 		POSITION pos = s_listHwnd.Find(m_hWnd);

	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	push	ecx
	mov	ecx, OFFSET FLAT:?s_listHwnd@CHoldRedraw@@1V?$CList@PAUHWND__@@AAPAU1@@@A
	call	?Find@?$CList@PAUHWND__@@AAPAU1@@@QBEPAU__POSITION@@AAPAUHWND__@@PAU2@@Z ; CList<HWND__ *,HWND__ * &>::Find
	mov	DWORD PTR _pos$73999[ebp], eax
$L74000:

; 76   : 		ASSERT(pos);

	cmp	DWORD PTR _pos$73999[ebp], 0
	jne	SHORT $L74003
	push	76					; 0000004cH
	push	OFFSET FLAT:$SG74004
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74003
	int	3
$L74003:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74000

; 77   : 		
; 78   : 		s_listHwnd.RemoveAt(pos);

	mov	eax, DWORD PTR _pos$73999[ebp]
	push	eax
	mov	ecx, OFFSET FLAT:?s_listHwnd@CHoldRedraw@@1V?$CList@PAUHWND__@@AAPAU1@@@A
	call	?RemoveAt@?$CList@PAUHWND__@@AAPAU1@@@QAEXPAU__POSITION@@@Z ; CList<HWND__ *,HWND__ * &>::RemoveAt
$L73998:

; 80   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CRedrawAll@@UAE@XZ			; CRedrawAll::~CRedrawAll
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74491:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CRedrawAll@@UAE@XZ			; CRedrawAll::~CRedrawAll
	ret	0
$L74494:
	mov	eax, OFFSET FLAT:$T74493
	jmp	___CxxFrameHandler
text$x	ENDS
??1CHoldRedraw@@UAE@XZ ENDP				; CHoldRedraw::~CHoldRedraw
PUBLIC	?Serialize@?$CList@PAUHWND__@@AAPAU1@@@UAEXAAVCArchive@@@Z ; CList<HWND__ *,HWND__ * &>::Serialize
PUBLIC	?Dump@?$CList@PAUHWND__@@AAPAU1@@@UBEXAAVCDumpContext@@@Z ; CList<HWND__ *,HWND__ * &>::Dump
PUBLIC	?AssertValid@?$CList@PAUHWND__@@AAPAU1@@@UBEXXZ	; CList<HWND__ *,HWND__ * &>::AssertValid
PUBLIC	??_7?$CList@PAUHWND__@@AAPAU1@@@6B@		; CList<HWND__ *,HWND__ * &>::`vftable'
PUBLIC	??_G?$CList@PAUHWND__@@AAPAU1@@@UAEPAXI@Z	; CList<HWND__ *,HWND__ * &>::`scalar deleting destructor'
PUBLIC	??_E?$CList@PAUHWND__@@AAPAU1@@@UAEPAXI@Z	; CList<HWND__ *,HWND__ * &>::`vector deleting destructor'
EXTRN	?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ:NEAR ; CObject::GetRuntimeClass
EXTRN	??1CObject@@UAE@XZ:NEAR				; CObject::~CObject
EXTRN	??0CObject@@IAE@XZ:NEAR				; CObject::CObject
;	COMDAT ??_7?$CList@PAUHWND__@@AAPAU1@@@6B@
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
CONST	SEGMENT
??_7?$CList@PAUHWND__@@AAPAU1@@@6B@ DD FLAT:?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ ; CList<HWND__ *,HWND__ * &>::`vftable'
	DD	FLAT:??_E?$CList@PAUHWND__@@AAPAU1@@@UAEPAXI@Z
	DD	FLAT:?Serialize@?$CList@PAUHWND__@@AAPAU1@@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@?$CList@PAUHWND__@@AAPAU1@@@UBEXXZ
	DD	FLAT:?Dump@?$CList@PAUHWND__@@AAPAU1@@@UBEXAAVCDumpContext@@@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74503	DD	019930520H
	DD	01H
	DD	FLAT:$T74505
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74505	DD	0ffffffffH
	DD	FLAT:$L74501
xdata$x	ENDS
;	COMDAT ??0?$CList@PAUHWND__@@AAPAU1@@@QAE@H@Z
_TEXT	SEGMENT
_nBlockSize$ = 8
_this$ = -16
__$EHRec$ = -12
??0?$CList@PAUHWND__@@AAPAU1@@@QAE@H@Z PROC NEAR	; CList<HWND__ *,HWND__ * &>::CList<HWND__ *,HWND__ * &>, COMDAT

; 728  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74504
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
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CList@PAUHWND__@@AAPAU1@@@6B@ ; CList<HWND__ *,HWND__ * &>::`vftable'
$L74093:

; 729  : 	ASSERT(nBlockSize > 0);

	cmp	DWORD PTR _nBlockSize$[ebp], 0
	jg	SHORT $L74096
	push	729					; 000002d9H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74096
	int	3
$L74096:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74093

; 730  : 
; 731  : 	m_nCount = 0;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12], 0

; 732  : 	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+16], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+8], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0

; 733  : 	m_pBlocks = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+20], 0

; 734  : 	m_nBlockSize = nBlockSize;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nBlockSize$[ebp]
	mov	DWORD PTR [ecx+24], edx

; 735  : }

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
$L74501:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L74504:
	mov	eax, OFFSET FLAT:$T74503
	jmp	___CxxFrameHandler
text$x	ENDS
??0?$CList@PAUHWND__@@AAPAU1@@@QAE@H@Z ENDP		; CList<HWND__ *,HWND__ * &>::CList<HWND__ *,HWND__ * &>
PUBLIC	?NewNode@?$CList@PAUHWND__@@AAPAU1@@@IAEPAUCNode@1@PAU21@0@Z ; CList<HWND__ *,HWND__ * &>::NewNode
EXTRN	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z:NEAR ; AfxAssertValidObject
;	COMDAT ?AddTail@?$CList@PAUHWND__@@AAPAU1@@@QAEPAU__POSITION@@AAPAUHWND__@@@Z
_TEXT	SEGMENT
_newElement$ = 8
_this$ = -8
_pNewNode$ = -4
?AddTail@?$CList@PAUHWND__@@AAPAU1@@@QAEPAU__POSITION@@AAPAUHWND__@@@Z PROC NEAR ; CList<HWND__ *,HWND__ * &>::AddTail, COMDAT

; 839  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 840  : 	ASSERT_VALID(this);

	push	840					; 00000348H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 841  : 
; 842  : 	CNode* pNewNode = NewNode(m_pNodeTail, NULL);

	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?NewNode@?$CList@PAUHWND__@@AAPAU1@@@IAEPAUCNode@1@PAU21@0@Z ; CList<HWND__ *,HWND__ * &>::NewNode
	mov	DWORD PTR _pNewNode$[ebp], eax

; 843  : 	pNewNode->data = newElement;

	mov	eax, DWORD PTR _pNewNode$[ebp]
	mov	ecx, DWORD PTR _newElement$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR [eax+8], edx

; 844  : 	if (m_pNodeTail != NULL)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	je	SHORT $L74102

; 845  : 		m_pNodeTail->pNext = pNewNode;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	eax, DWORD PTR _pNewNode$[ebp]
	mov	DWORD PTR [edx], eax

; 846  : 	else

	jmp	SHORT $L74103
$L74102:

; 847  : 		m_pNodeHead = pNewNode;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _pNewNode$[ebp]
	mov	DWORD PTR [ecx+4], edx
$L74103:

; 848  : 	m_pNodeTail = pNewNode;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _pNewNode$[ebp]
	mov	DWORD PTR [eax+8], ecx

; 849  : 	return (POSITION) pNewNode;

	mov	eax, DWORD PTR _pNewNode$[ebp]

; 850  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?AddTail@?$CList@PAUHWND__@@AAPAU1@@@QAEPAU__POSITION@@AAPAUHWND__@@@Z ENDP ; CList<HWND__ *,HWND__ * &>::AddTail
_TEXT	ENDS
PUBLIC	?FreeNode@?$CList@PAUHWND__@@AAPAU1@@@IAEXPAUCNode@1@@Z ; CList<HWND__ *,HWND__ * &>::FreeNode
EXTRN	?AfxIsValidAddress@@YGHPBXIH@Z:NEAR		; AfxIsValidAddress
;	COMDAT ?RemoveAt@?$CList@PAUHWND__@@AAPAU1@@@QAEXPAU__POSITION@@@Z
_TEXT	SEGMENT
_position$ = 8
_this$ = -8
_pOldNode$ = -4
?RemoveAt@?$CList@PAUHWND__@@AAPAU1@@@QAEXPAU__POSITION@@@Z PROC NEAR ; CList<HWND__ *,HWND__ * &>::RemoveAt, COMDAT

; 974  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 975  : 	ASSERT_VALID(this);

	push	975					; 000003cfH
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 976  : 
; 977  : 	CNode* pOldNode = (CNode*) position;

	mov	ecx, DWORD PTR _position$[ebp]
	mov	DWORD PTR _pOldNode$[ebp], ecx
$L74111:

; 978  : 	ASSERT(AfxIsValidAddress(pOldNode, sizeof(CNode)));

	push	1
	push	12					; 0000000cH
	mov	edx, DWORD PTR _pOldNode$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74115
	push	978					; 000003d2H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74115
	int	3
$L74115:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74111

; 979  : 
; 980  : 	// remove pOldNode from list
; 981  : 	if (pOldNode == m_pNodeHead)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _pOldNode$[ebp]
	cmp	edx, DWORD PTR [ecx+4]
	jne	SHORT $L74116

; 983  : 		m_pNodeHead = pOldNode->pNext;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _pOldNode$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR [eax+4], edx

; 985  : 	else

	jmp	SHORT $L74117
$L74116:

; 987  : 		ASSERT(AfxIsValidAddress(pOldNode->pPrev, sizeof(CNode)));

	push	1
	push	12					; 0000000cH
	mov	eax, DWORD PTR _pOldNode$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74122
	push	987					; 000003dbH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74122
	int	3
$L74122:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74116

; 988  : 		pOldNode->pPrev->pNext = pOldNode->pNext;

	mov	eax, DWORD PTR _pOldNode$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _pOldNode$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR [ecx], eax
$L74117:

; 990  : 	if (pOldNode == m_pNodeTail)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _pOldNode$[ebp]
	cmp	edx, DWORD PTR [ecx+8]
	jne	SHORT $L74123

; 992  : 		m_pNodeTail = pOldNode->pPrev;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _pOldNode$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR [eax+8], edx

; 994  : 	else

	jmp	SHORT $L74124
$L74123:

; 996  : 		ASSERT(AfxIsValidAddress(pOldNode->pNext, sizeof(CNode)));

	push	1
	push	12					; 0000000cH
	mov	eax, DWORD PTR _pOldNode$[ebp]
	mov	ecx, DWORD PTR [eax]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74129
	push	996					; 000003e4H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74129
	int	3
$L74129:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74123

; 997  : 		pOldNode->pNext->pPrev = pOldNode->pPrev;

	mov	eax, DWORD PTR _pOldNode$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR _pOldNode$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR [ecx+4], eax
$L74124:

; 999  : 	FreeNode(pOldNode);

	mov	ecx, DWORD PTR _pOldNode$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FreeNode@?$CList@PAUHWND__@@AAPAU1@@@IAEXPAUCNode@1@@Z ; CList<HWND__ *,HWND__ * &>::FreeNode

; 1000 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?RemoveAt@?$CList@PAUHWND__@@AAPAU1@@@QAEXPAU__POSITION@@@Z ENDP ; CList<HWND__ *,HWND__ * &>::RemoveAt
_TEXT	ENDS
PUBLIC	?CompareElements@@YGHPBQAUHWND__@@0@Z		; CompareElements
;	COMDAT ?Find@?$CList@PAUHWND__@@AAPAU1@@@QBEPAU__POSITION@@AAPAUHWND__@@PAU2@@Z
_TEXT	SEGMENT
_searchValue$ = 8
_startAfter$ = 12
_this$ = -8
_pNode$ = -4
?Find@?$CList@PAUHWND__@@AAPAU1@@@QBEPAU__POSITION@@AAPAUHWND__@@PAU2@@Z PROC NEAR ; CList<HWND__ *,HWND__ * &>::Find, COMDAT

; 1021 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1022 : 	ASSERT_VALID(this);

	push	1022					; 000003feH
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1023 : 
; 1024 : 	CNode* pNode = (CNode*) startAfter;

	mov	ecx, DWORD PTR _startAfter$[ebp]
	mov	DWORD PTR _pNode$[ebp], ecx

; 1025 : 	if (pNode == NULL)

	cmp	DWORD PTR _pNode$[ebp], 0
	jne	SHORT $L74137

; 1027 : 		pNode = m_pNodeHead;  // start at head

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR _pNode$[ebp], eax

; 1029 : 	else

	jmp	SHORT $L74138
$L74137:

; 1031 : 		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));

	push	1
	push	12					; 0000000cH
	mov	ecx, DWORD PTR _pNode$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74143
	push	1031					; 00000407H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74143
	int	3
$L74143:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74137

; 1032 : 		pNode = pNode->pNext;  // start after the one specified

	mov	eax, DWORD PTR _pNode$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pNode$[ebp], ecx
$L74138:

; 1034 : 
; 1035 : 	for (; pNode != NULL; pNode = pNode->pNext)

	jmp	SHORT $L74144
$L74145:
	mov	edx, DWORD PTR _pNode$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _pNode$[ebp], eax
$L74144:
	cmp	DWORD PTR _pNode$[ebp], 0
	je	SHORT $L74146

; 1036 : 		if (CompareElements<TYPE>(&pNode->data, &searchValue))

	mov	ecx, DWORD PTR _searchValue$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pNode$[ebp]
	add	edx, 8
	push	edx
	call	?CompareElements@@YGHPBQAUHWND__@@0@Z	; CompareElements
	test	eax, eax
	je	SHORT $L74153

; 1037 : 			return (POSITION)pNode;

	mov	eax, DWORD PTR _pNode$[ebp]
	jmp	SHORT $L74134
$L74153:

; 1038 : 	return NULL;

	jmp	SHORT $L74145
$L74146:
	xor	eax, eax
$L74134:

; 1039 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Find@?$CList@PAUHWND__@@AAPAU1@@@QBEPAU__POSITION@@AAPAUHWND__@@PAU2@@Z ENDP ; CList<HWND__ *,HWND__ * &>::Find
_TEXT	ENDS
PUBLIC	?RemoveAll@?$CList@PAUHWND__@@AAPAU1@@@QAEXXZ	; CList<HWND__ *,HWND__ * &>::RemoveAll
;	COMDAT xdata$x
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
xdata$x	SEGMENT
$T74518	DD	019930520H
	DD	01H
	DD	FLAT:$T74520
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74520	DD	0ffffffffH
	DD	FLAT:$L74516
xdata$x	ENDS
;	COMDAT ??1?$CList@PAUHWND__@@AAPAU1@@@UAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1?$CList@PAUHWND__@@AAPAU1@@@UAE@XZ PROC NEAR		; CList<HWND__ *,HWND__ * &>::~CList<HWND__ *,HWND__ * &>, COMDAT

; 755  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74519
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
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CList@PAUHWND__@@AAPAU1@@@6B@ ; CList<HWND__ *,HWND__ * &>::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 756  : 	RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CList@PAUHWND__@@AAPAU1@@@QAEXXZ ; CList<HWND__ *,HWND__ * &>::RemoveAll
$L74158:

; 757  : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L74161
	push	757					; 000002f5H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74161
	int	3
$L74161:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74158

; 758  : }

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
$L74516:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L74519:
	mov	eax, OFFSET FLAT:$T74518
	jmp	___CxxFrameHandler
text$x	ENDS
??1?$CList@PAUHWND__@@AAPAU1@@@UAE@XZ ENDP		; CList<HWND__ *,HWND__ * &>::~CList<HWND__ *,HWND__ * &>
PUBLIC	?SerializeElements@@YGXAAVCArchive@@PAPAUHWND__@@H@Z ; SerializeElements
EXTRN	?ReadCount@CArchive@@QAEKXZ:NEAR		; CArchive::ReadCount
EXTRN	?WriteCount@CArchive@@QAEXK@Z:NEAR		; CArchive::WriteCount
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	?IsStoring@CArchive@@QBEHXZ:NEAR		; CArchive::IsStoring
;	COMDAT ?Serialize@?$CList@PAUHWND__@@AAPAU1@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT
_ar$ = 8
_this$ = -16
_pNode$74167 = -4
_nNewCount$74183 = -8
_newData$74187 = -12
?Serialize@?$CList@PAUHWND__@@AAPAU1@@@UAEXAAVCArchive@@@Z PROC NEAR ; CList<HWND__ *,HWND__ * &>::Serialize, COMDAT

; 1043 : {

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

; 1044 : 	ASSERT_VALID(this);

	push	1044					; 00000414H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1045 : 
; 1046 : 	CObject::Serialize(ar);

	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Serialize@CObject@@UAEXAAVCArchive@@@Z	; CObject::Serialize

; 1047 : 
; 1048 : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L74166

; 1050 : 		ar.WriteCount(m_nCount);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?WriteCount@CArchive@@QAEXK@Z		; CArchive::WriteCount

; 1051 : 		for (CNode* pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR _pNode$74167[ebp], edx
	jmp	SHORT $L74168
$L74169:
	mov	eax, DWORD PTR _pNode$74167[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pNode$74167[ebp], ecx
$L74168:
	cmp	DWORD PTR _pNode$74167[ebp], 0
	je	SHORT $L74170
$L74171:

; 1053 : 			ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));

	push	1
	push	12					; 0000000cH
	mov	edx, DWORD PTR _pNode$74167[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74175
	push	1053					; 0000041dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74175
	int	3
$L74175:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74171

; 1054 : 			SerializeElements<TYPE>(ar, &pNode->data, 1);

	push	1
	mov	ecx, DWORD PTR _pNode$74167[ebp]
	add	ecx, 8
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAPAUHWND__@@H@Z ; SerializeElements

; 1055 : 		}

	jmp	SHORT $L74169
$L74170:

; 1057 : 	else

	jmp	SHORT $L74186
$L74166:

; 1059 : 		DWORD nNewCount = ar.ReadCount();

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?ReadCount@CArchive@@QAEKXZ		; CArchive::ReadCount
	mov	DWORD PTR _nNewCount$74183[ebp], eax
$L74185:

; 1060 : 		while (nNewCount--)

	mov	eax, DWORD PTR _nNewCount$74183[ebp]
	mov	ecx, DWORD PTR _nNewCount$74183[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nNewCount$74183[ebp], ecx
	test	eax, eax
	je	SHORT $L74186

; 1062 : 			TYPE newData;
; 1063 : 			SerializeElements<TYPE>(ar, &newData, 1);

	push	1
	lea	edx, DWORD PTR _newData$74187[ebp]
	push	edx
	mov	eax, DWORD PTR _ar$[ebp]
	push	eax
	call	?SerializeElements@@YGXAAVCArchive@@PAPAUHWND__@@H@Z ; SerializeElements

; 1064 : 			AddTail(newData);

	lea	ecx, DWORD PTR _newData$74187[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddTail@?$CList@PAUHWND__@@AAPAU1@@@QAEPAU__POSITION@@AAPAUHWND__@@@Z ; CList<HWND__ *,HWND__ * &>::AddTail

; 1065 : 		}

	jmp	SHORT $L74185
$L74186:

; 1067 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Serialize@?$CList@PAUHWND__@@AAPAU1@@@UAEXAAVCArchive@@@Z ENDP ; CList<HWND__ *,HWND__ * &>::Serialize
_TEXT	ENDS
PUBLIC	?GetHeadPosition@?$CList@PAUHWND__@@AAPAU1@@@QBEPAU__POSITION@@XZ ; CList<HWND__ *,HWND__ * &>::GetHeadPosition
PUBLIC	?GetNext@?$CList@PAUHWND__@@AAPAU1@@@QAEAAPAUHWND__@@AAPAU__POSITION@@@Z ; CList<HWND__ *,HWND__ * &>::GetNext
PUBLIC	??_C@_05KIFI@with?5?$AA@			; `string'
PUBLIC	??_C@_09LOPF@?5elements?$AA@			; `string'
PUBLIC	??_C@_01BJG@?6?$AA@				; `string'
PUBLIC	?DumpElements@@YGXAAVCDumpContext@@PBQAUHWND__@@H@Z ; DumpElements
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
;	COMDAT ??_C@_01BJG@?6?$AA@
_DATA	SEGMENT
??_C@_01BJG@?6?$AA@ DB 0aH, 00H				; `string'
_DATA	ENDS
;	COMDAT ?Dump@?$CList@PAUHWND__@@AAPAU1@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT
_dc$ = 8
_this$ = -8
_pos$74200 = -4
?Dump@?$CList@PAUHWND__@@AAPAU1@@@UBEXAAVCDumpContext@@@Z PROC NEAR ; CList<HWND__ *,HWND__ * &>::Dump, COMDAT

; 1072 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1073 : 	CObject::Dump(dc);

	mov	eax, DWORD PTR _dc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Dump@CObject@@UBEXAAVCDumpContext@@@Z	; CObject::Dump

; 1074 : 
; 1075 : 	dc << "with " << m_nCount << " elements";

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

; 1076 : 	if (dc.GetDepth() > 0)

	mov	ecx, DWORD PTR _dc$[ebp]
	call	?GetDepth@CDumpContext@@QBEHXZ		; CDumpContext::GetDepth
	test	eax, eax
	jle	SHORT $L74203

; 1078 : 		POSITION pos = GetHeadPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetHeadPosition@?$CList@PAUHWND__@@AAPAU1@@@QBEPAU__POSITION@@XZ ; CList<HWND__ *,HWND__ * &>::GetHeadPosition
	mov	DWORD PTR _pos$74200[ebp], eax
$L74202:

; 1079 : 		while (pos != NULL)

	cmp	DWORD PTR _pos$74200[ebp], 0
	je	SHORT $L74203

; 1081 : 			dc << "\n";

	push	OFFSET FLAT:??_C@_01BJG@?6?$AA@		; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1082 : 			DumpElements<TYPE>(dc, &((CList*)this)->GetNext(pos), 1);

	push	1
	lea	eax, DWORD PTR _pos$74200[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetNext@?$CList@PAUHWND__@@AAPAU1@@@QAEAAPAUHWND__@@AAPAU__POSITION@@@Z ; CList<HWND__ *,HWND__ * &>::GetNext
	push	eax
	mov	ecx, DWORD PTR _dc$[ebp]
	push	ecx
	call	?DumpElements@@YGXAAVCDumpContext@@PBQAUHWND__@@H@Z ; DumpElements

; 1083 : 		}

	jmp	SHORT $L74202
$L74203:

; 1085 : 
; 1086 : 	dc << "\n";

	push	OFFSET FLAT:??_C@_01BJG@?6?$AA@		; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1087 : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Dump@?$CList@PAUHWND__@@AAPAU1@@@UBEXAAVCDumpContext@@@Z ENDP ; CList<HWND__ *,HWND__ * &>::Dump
_TEXT	ENDS
EXTRN	?AssertValid@CObject@@UBEXXZ:NEAR		; CObject::AssertValid
;	COMDAT ?AssertValid@?$CList@PAUHWND__@@AAPAU1@@@UBEXXZ
_TEXT	SEGMENT
_this$ = -4
?AssertValid@?$CList@PAUHWND__@@AAPAU1@@@UBEXXZ PROC NEAR ; CList<HWND__ *,HWND__ * &>::AssertValid, COMDAT

; 1091 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1092 : 	CObject::AssertValid();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?AssertValid@CObject@@UBEXXZ		; CObject::AssertValid

; 1093 : 
; 1094 : 	if (m_nCount == 0)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jne	SHORT $L74215
$L74216:

; 1096 : 		// empty list
; 1097 : 		ASSERT(m_pNodeHead == NULL);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L74219
	push	1097					; 00000449H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74219
	int	3
$L74219:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74216
$L74218:

; 1098 : 		ASSERT(m_pNodeTail == NULL);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	je	SHORT $L74223
	push	1098					; 0000044aH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74223
	int	3
$L74223:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74218

; 1100 : 	else

	jmp	SHORT $L74232
$L74215:

; 1102 : 		// non-empty list
; 1103 : 		ASSERT(AfxIsValidAddress(m_pNodeHead, sizeof(CNode)));

	push	1
	push	12					; 0000000cH
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74229
	push	1103					; 0000044fH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74229
	int	3
$L74229:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74215
$L74227:

; 1104 : 		ASSERT(AfxIsValidAddress(m_pNodeTail, sizeof(CNode)));

	push	1
	push	12					; 0000000cH
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	push	eax
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74234
	push	1104					; 00000450H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74234
	int	3
$L74234:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74227
$L74232:

; 1106 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?AssertValid@?$CList@PAUHWND__@@AAPAU1@@@UBEXXZ ENDP	; CList<HWND__ *,HWND__ * &>::AssertValid
_TEXT	ENDS
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_G?$CList@PAUHWND__@@AAPAU1@@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_G?$CList@PAUHWND__@@AAPAU1@@@UAEPAXI@Z PROC NEAR	; CList<HWND__ *,HWND__ * &>::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1?$CList@PAUHWND__@@AAPAU1@@@UAE@XZ	; CList<HWND__ *,HWND__ * &>::~CList<HWND__ *,HWND__ * &>
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L74238
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L74238:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_G?$CList@PAUHWND__@@AAPAU1@@@UAEPAXI@Z ENDP		; CList<HWND__ *,HWND__ * &>::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	?DestructElements@@YGXPAPAUHWND__@@H@Z		; DestructElements
EXTRN	?FreeDataChain@CPlex@@QAEXXZ:NEAR		; CPlex::FreeDataChain
;	COMDAT ?RemoveAll@?$CList@PAUHWND__@@AAPAU1@@@QAEXXZ
_TEXT	SEGMENT
_this$ = -8
_pNode$ = -4
?RemoveAll@?$CList@PAUHWND__@@AAPAU1@@@QAEXXZ PROC NEAR	; CList<HWND__ *,HWND__ * &>::RemoveAll, COMDAT

; 739  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 740  : 	ASSERT_VALID(this);

	push	740					; 000002e4H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 741  : 
; 742  : 	// destroy elements
; 743  : 	CNode* pNode;
; 744  : 	for (pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR _pNode$[ebp], edx
	jmp	SHORT $L74243
$L74244:
	mov	eax, DWORD PTR _pNode$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pNode$[ebp], ecx
$L74243:
	cmp	DWORD PTR _pNode$[ebp], 0
	je	SHORT $L74245

; 745  : 		DestructElements<TYPE>(&pNode->data, 1);

	push	1
	mov	edx, DWORD PTR _pNode$[ebp]
	add	edx, 8
	push	edx
	call	?DestructElements@@YGXPAPAUHWND__@@H@Z	; DestructElements
	jmp	SHORT $L74244
$L74245:

; 746  : 
; 747  : 	m_nCount = 0;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+12], 0

; 748  : 	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+16], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+8], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+4], 0

; 749  : 	m_pBlocks->FreeDataChain();

	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+20]
	call	?FreeDataChain@CPlex@@QAEXXZ		; CPlex::FreeDataChain

; 750  : 	m_pBlocks = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+20], 0

; 751  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?RemoveAll@?$CList@PAUHWND__@@AAPAU1@@@QAEXXZ ENDP	; CList<HWND__ *,HWND__ * &>::RemoveAll
_TEXT	ENDS
;	COMDAT ?GetHeadPosition@?$CList@PAUHWND__@@AAPAU1@@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetHeadPosition@?$CList@PAUHWND__@@AAPAU1@@@QBEPAU__POSITION@@XZ PROC NEAR ; CList<HWND__ *,HWND__ * &>::GetHeadPosition, COMDAT

; 682  : 	{ return (POSITION) m_pNodeHead; }

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
?GetHeadPosition@?$CList@PAUHWND__@@AAPAU1@@@QBEPAU__POSITION@@XZ ENDP ; CList<HWND__ *,HWND__ * &>::GetHeadPosition
_TEXT	ENDS
;	COMDAT ?GetNext@?$CList@PAUHWND__@@AAPAU1@@@QAEAAPAUHWND__@@AAPAU__POSITION@@@Z
_TEXT	SEGMENT
_rPosition$ = 8
_this$ = -8
_pNode$ = -4
?GetNext@?$CList@PAUHWND__@@AAPAU1@@@QAEAAPAUHWND__@@AAPAU__POSITION@@@Z PROC NEAR ; CList<HWND__ *,HWND__ * &>::GetNext, COMDAT

; 688  : 	{ CNode* pNode = (CNode*) rPosition;

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _rPosition$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pNode$[ebp], ecx
$L74261:

; 689  : 		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));

	push	1
	push	12					; 0000000cH
	mov	edx, DWORD PTR _pNode$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74265
	push	689					; 000002b1H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74265
	int	3
$L74265:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74261

; 690  : 		rPosition = (POSITION) pNode->pNext;

	mov	ecx, DWORD PTR _rPosition$[ebp]
	mov	edx, DWORD PTR _pNode$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR [ecx], eax

; 691  : 		return pNode->data; }

	mov	eax, DWORD PTR _pNode$[ebp]
	add	eax, 8
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetNext@?$CList@PAUHWND__@@AAPAU1@@@QAEAAPAUHWND__@@AAPAU__POSITION@@@Z ENDP ; CList<HWND__ *,HWND__ * &>::GetNext
_TEXT	ENDS
PUBLIC	?data@CPlex@@QAEPAXXZ				; CPlex::data
PUBLIC	?ConstructElements@@YGXPAPAUHWND__@@H@Z		; ConstructElements
EXTRN	?Create@CPlex@@SGPAU1@AAPAU1@II@Z:NEAR		; CPlex::Create
;	COMDAT ?NewNode@?$CList@PAUHWND__@@AAPAU1@@@IAEPAUCNode@1@PAU21@0@Z
_TEXT	SEGMENT
_pPrev$ = 8
_pNext$ = 12
_this$ = -20
_pNewBlock$74273 = -16
_pNode$74275 = -8
_i$74277 = -12
_pNode$ = -4
?NewNode@?$CList@PAUHWND__@@AAPAU1@@@IAEPAUCNode@1@PAU21@0@Z PROC NEAR ; CList<HWND__ *,HWND__ * &>::NewNode, COMDAT

; 778  : {

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

; 779  : 	if (m_pNodeFree == NULL)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	jne	SHORT $L74280

; 781  : 		// add another block
; 782  : 		CPlex* pNewBlock = CPlex::Create(m_pBlocks, m_nBlockSize,
; 783  : 				 sizeof(CNode));

	push	12					; 0000000cH
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 20					; 00000014H
	push	eax
	call	?Create@CPlex@@SGPAU1@AAPAU1@II@Z	; CPlex::Create
	mov	DWORD PTR _pNewBlock$74273[ebp], eax

; 784  : 
; 785  : 		// chain them into free list
; 786  : 		CNode* pNode = (CNode*) pNewBlock->data();

	mov	ecx, DWORD PTR _pNewBlock$74273[ebp]
	call	?data@CPlex@@QAEPAXXZ			; CPlex::data
	mov	DWORD PTR _pNode$74275[ebp], eax

; 787  : 		// free in reverse order to make it easier to debug
; 788  : 		pNode += m_nBlockSize - 1;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	imul	edx, 12					; 0000000cH
	mov	eax, DWORD PTR _pNode$74275[ebp]
	add	eax, edx
	mov	DWORD PTR _pNode$74275[ebp], eax

; 789  : 		for (int i = m_nBlockSize-1; i >= 0; i--, pNode--)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	mov	DWORD PTR _i$74277[ebp], edx
	jmp	SHORT $L74278
$L74279:
	mov	eax, DWORD PTR _i$74277[ebp]
	sub	eax, 1
	mov	DWORD PTR _i$74277[ebp], eax
	mov	ecx, DWORD PTR _pNode$74275[ebp]
	sub	ecx, 12					; 0000000cH
	mov	DWORD PTR _pNode$74275[ebp], ecx
$L74278:
	cmp	DWORD PTR _i$74277[ebp], 0
	jl	SHORT $L74280

; 791  : 			pNode->pNext = m_pNodeFree;

	mov	edx, DWORD PTR _pNode$74275[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR [edx], ecx

; 792  : 			m_pNodeFree = pNode;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _pNode$74275[ebp]
	mov	DWORD PTR [edx+16], eax

; 793  : 		}

	jmp	SHORT $L74279
$L74280:

; 795  : 	ASSERT(m_pNodeFree != NULL);  // we must have something

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+16], 0
	jne	SHORT $L74284
	push	795					; 0000031bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74284
	int	3
$L74284:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74280

; 796  : 
; 797  : 	CList::CNode* pNode = m_pNodeFree;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR _pNode$[ebp], ecx

; 798  : 	m_pNodeFree = m_pNodeFree->pNext;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	DWORD PTR [ecx+16], edx

; 799  : 	pNode->pPrev = pPrev;

	mov	eax, DWORD PTR _pNode$[ebp]
	mov	ecx, DWORD PTR _pPrev$[ebp]
	mov	DWORD PTR [eax+4], ecx

; 800  : 	pNode->pNext = pNext;

	mov	edx, DWORD PTR _pNode$[ebp]
	mov	eax, DWORD PTR _pNext$[ebp]
	mov	DWORD PTR [edx], eax

; 801  : 	m_nCount++;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	add	edx, 1
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+12], edx
$L74286:

; 802  : 	ASSERT(m_nCount > 0);  // make sure we don't overflow

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jg	SHORT $L74289
	push	802					; 00000322H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74289
	int	3
$L74289:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74286

; 803  : 
; 804  : 	ConstructElements<TYPE>(&pNode->data, 1);

	push	1
	mov	eax, DWORD PTR _pNode$[ebp]
	add	eax, 8
	push	eax
	call	?ConstructElements@@YGXPAPAUHWND__@@H@Z	; ConstructElements

; 805  : 	return pNode;

	mov	eax, DWORD PTR _pNode$[ebp]

; 806  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?NewNode@?$CList@PAUHWND__@@AAPAU1@@@IAEPAUCNode@1@PAU21@0@Z ENDP ; CList<HWND__ *,HWND__ * &>::NewNode
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
;	COMDAT ?FreeNode@?$CList@PAUHWND__@@AAPAU1@@@IAEXPAUCNode@1@@Z
_TEXT	SEGMENT
_pNode$ = 8
_this$ = -4
?FreeNode@?$CList@PAUHWND__@@AAPAU1@@@IAEXPAUCNode@1@@Z PROC NEAR ; CList<HWND__ *,HWND__ * &>::FreeNode, COMDAT

; 810  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 811  : 	DestructElements<TYPE>(&pNode->data, 1);

	push	1
	mov	eax, DWORD PTR _pNode$[ebp]
	add	eax, 8
	push	eax
	call	?DestructElements@@YGXPAPAUHWND__@@H@Z	; DestructElements

; 812  : 	pNode->pNext = m_pNodeFree;

	mov	ecx, DWORD PTR _pNode$[ebp]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	DWORD PTR [ecx], eax

; 813  : 	m_pNodeFree = pNode;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _pNode$[ebp]
	mov	DWORD PTR [ecx+16], edx

; 814  : 	m_nCount--;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	sub	ecx, 1
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12], ecx
$L74303:

; 815  : 	ASSERT(m_nCount >= 0);  // make sure we don't underflow

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jge	SHORT $L74306
	push	815					; 0000032fH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74306
	int	3
$L74306:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74303

; 816  : 
; 817  : 	// if no more elements, cleanup completely
; 818  : 	if (m_nCount == 0)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+12], 0
	jne	SHORT $L74307

; 819  : 		RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CList@PAUHWND__@@AAPAU1@@@QAEXXZ ; CList<HWND__ *,HWND__ * &>::RemoveAll
$L74307:

; 820  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?FreeNode@?$CList@PAUHWND__@@AAPAU1@@@IAEXPAUCNode@1@@Z ENDP ; CList<HWND__ *,HWND__ * &>::FreeNode
_TEXT	ENDS
;	COMDAT ?CompareElements@@YGHPBQAUHWND__@@0@Z
_TEXT	SEGMENT
_pElement1$ = 8
_pElement2$ = 12
?CompareElements@@YGHPBQAUHWND__@@0@Z PROC NEAR		; CompareElements, COMDAT

; 118  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L74309:

; 119  : 	ASSERT(AfxIsValidAddress(pElement1, sizeof(TYPE), FALSE));

	push	0
	push	4
	mov	eax, DWORD PTR _pElement1$[ebp]
	push	eax
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74313
	push	119					; 00000077H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74313
	int	3
$L74313:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74309
$L74311:

; 120  : 	ASSERT(AfxIsValidAddress(pElement2, sizeof(ARG_TYPE), FALSE));

	push	0
	push	4
	mov	edx, DWORD PTR _pElement2$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74318
	push	120					; 00000078H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74318
	int	3
$L74318:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74311

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
?CompareElements@@YGHPBQAUHWND__@@0@Z ENDP		; CompareElements
_TEXT	ENDS
EXTRN	?Read@CArchive@@QAEIPAXI@Z:NEAR			; CArchive::Read
EXTRN	?Write@CArchive@@QAEXPBXI@Z:NEAR		; CArchive::Write
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAPAUHWND__@@H@Z
_TEXT	SEGMENT
_ar$ = 8
_pElements$ = 12
_nCount$ = 16
?SerializeElements@@YGXAAVCArchive@@PAPAUHWND__@@H@Z PROC NEAR ; SerializeElements, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L74320:

; 92   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L74324
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74324
	push	93					; 0000005dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74324
	int	3
$L74324:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74320

; 93   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 94   : 
; 95   : 	// default is bit-wise read/write
; 96   : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L74325

; 97   : 		ar.Write((void*)pElements, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Write@CArchive@@QAEXPBXI@Z		; CArchive::Write

; 98   : 	else

	jmp	SHORT $L74328
$L74325:

; 99   : 		ar.Read((void*)pElements, nCount * sizeof(TYPE));

	mov	edx, DWORD PTR _nCount$[ebp]
	shl	edx, 2
	push	edx
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Read@CArchive@@QAEIPAXI@Z		; CArchive::Read
$L74328:

; 100  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	12					; 0000000cH
?SerializeElements@@YGXAAVCArchive@@PAPAUHWND__@@H@Z ENDP ; SerializeElements
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBQAUHWND__@@H@Z
_TEXT	SEGMENT
_pElements$ = 12
_nCount$ = 16
?DumpElements@@YGXAAVCDumpContext@@PBQAUHWND__@@H@Z PROC NEAR ; DumpElements, COMDAT

; 105  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L74332:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L74336
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74336
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74336
	int	3
$L74336:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74332

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
?DumpElements@@YGXAAVCDumpContext@@PBQAUHWND__@@H@Z ENDP ; DumpElements
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAPAUHWND__@@H@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
?DestructElements@@YGXPAPAUHWND__@@H@Z PROC NEAR	; DestructElements, COMDAT

; 67   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L74338:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L74342
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74342
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74342
	int	3
$L74342:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74338

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L74343
$L74344:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 4
	mov	DWORD PTR _pElements$[ebp], eax
$L74343:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L74345

; 73   : 		pElements->~TYPE();

	jmp	SHORT $L74344
$L74345:

; 74   : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?DestructElements@@YGXPAPAUHWND__@@H@Z ENDP		; DestructElements
_TEXT	ENDS
PUBLIC	??2@YAPAXIPAX@Z					; operator new
EXTRN	_memset:NEAR
;	COMDAT ?ConstructElements@@YGXPAPAUHWND__@@H@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
$T74553 = -4
?ConstructElements@@YGXPAPAUHWND__@@H@Z PROC NEAR	; ConstructElements, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L74347:

; 54   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L74351
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74351
	push	55					; 00000037H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74351
	int	3
$L74351:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74347

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

	jmp	SHORT $L74354
$L74355:
	mov	edx, DWORD PTR _pElements$[ebp]
	add	edx, 4
	mov	DWORD PTR _pElements$[ebp], edx
$L74354:
	mov	eax, DWORD PTR _nCount$[ebp]
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
	test	eax, eax
	je	SHORT $L74356

; 62   : 		::new((void*)pElements) TYPE;

	mov	edx, DWORD PTR _pElements$[ebp]
	push	edx
	push	4
	call	??2@YAPAXIPAX@Z				; operator new
	add	esp, 8
	mov	DWORD PTR $T74553[ebp], eax
	jmp	SHORT $L74355
$L74356:

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
?ConstructElements@@YGXPAPAUHWND__@@H@Z ENDP		; ConstructElements
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
END

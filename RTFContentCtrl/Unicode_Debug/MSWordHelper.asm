	TITLE	D:\_CODE\Shared\MSWordHelper.cpp
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
;	COMDAT ?IsValid@CMSWordHelper@@QBEHXZ
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
;	COMDAT ??0_Application@WordAPI@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0Documents@WordAPI@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0_Document@WordAPI@@QAE@PAUIDispatch@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E270
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E271
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E272
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E273
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E275
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E276
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E277
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E278
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CMSWordHelper@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCMSWordHelper@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CMSWordHelper@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G_Application@WordAPI@@QAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1_Application@WordAPI@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsWordInstalled@CMSWordHelper@@SAHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetInstalledWordVersion@CMSWordHelper@@SAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RestartWord@CMSWordHelper@@KAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConvertContent@CMSWordHelper@@QAEHABVCString@@W4WdOpenFormat@WordAPI@@AAV2@W4WdSaveFormat@4@W4MsoEncoding@4@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConvertFile@CMSWordHelper@@QAEHABVCString@@W4WdOpenFormat@WordAPI@@0W4WdSaveFormat@4@W4MsoEncoding@4@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1Documents@WordAPI@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1_Document@WordAPI@@QAE@XZ
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
;	COMDAT ??_R0PAVCOleException@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_7CMSWordHelper@@6B@
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
PUBLIC	?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A ; CMSWordHelper::s_pWord
PUBLIC	?s_nRefCount@CMSWordHelper@@1HA			; CMSWordHelper::s_nRefCount
_BSS	SEGMENT
$SG77003 DW	01H DUP (?)
	ALIGN	4

?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A DD 01H DUP (?) ; CMSWordHelper::s_pWord
?s_nRefCount@CMSWordHelper@@1HA DD 01H DUP (?)		; CMSWordHelper::s_nRefCount
_BSS	ENDS
_DATA	SEGMENT
__szAfxTempl DB	'afxtempl.h', 00H
	ORG $+1
_THIS_FILE DB	'D:\_CODE\Shared\MSWordHelper.cpp', 00H
_DATA	ENDS
CRT$XCU	SEGMENT
_$S274	DD	FLAT:_$E273
_$S279	DD	FLAT:_$E278
CRT$XCU	ENDS
EXTRN	__chkesp:NEAR
;	COMDAT _$E273
_TEXT	SEGMENT
_$E273	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E270
	call	_$E272
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E273	ENDP
_TEXT	ENDS
EXTRN	??0COleVariant@@QAE@PBG@Z:NEAR			; COleVariant::COleVariant
EXTRN	??0COleVariant@@QAE@FG@Z:NEAR			; COleVariant::COleVariant
_BSS	SEGMENT
_varNull DB	010H DUP (?)
_varZero DB	010H DUP (?)
_varOne	DB	010H DUP (?)
_BSS	ENDS
;	COMDAT _$E270
_TEXT	SEGMENT
_$E270	PROC NEAR					; COMDAT

; 34   : static COleVariant varNull(_T("")), varZero((short)0), varOne((short)1);

	push	ebp
	mov	ebp, esp
	push	OFFSET FLAT:$SG77003
	mov	ecx, OFFSET FLAT:_varNull
	call	??0COleVariant@@QAE@PBG@Z		; COleVariant::COleVariant
	push	2
	push	0
	mov	ecx, OFFSET FLAT:_varZero
	call	??0COleVariant@@QAE@FG@Z		; COleVariant::COleVariant
	push	2
	push	1
	mov	ecx, OFFSET FLAT:_varOne
	call	??0COleVariant@@QAE@FG@Z		; COleVariant::COleVariant
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E270	ENDP
_TEXT	ENDS
EXTRN	_atexit:NEAR
;	COMDAT _$E272
_TEXT	SEGMENT
_$E272	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	push	OFFSET FLAT:_$E271
	call	_atexit
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E272	ENDP
_TEXT	ENDS
EXTRN	??1COleVariant@@QAE@XZ:NEAR			; COleVariant::~COleVariant
;	COMDAT _$E271
_TEXT	SEGMENT
_$E271	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_varOne
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
	mov	ecx, OFFSET FLAT:_varZero
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
	mov	ecx, OFFSET FLAT:_varNull
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E271	ENDP
_TEXT	ENDS
;	COMDAT _$E278
_TEXT	SEGMENT
_$E278	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E275
	call	_$E277
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E278	ENDP
_TEXT	ENDS
_BSS	SEGMENT
_varFalse DB	010H DUP (?)
_varTrue DB	010H DUP (?)
_BSS	ENDS
;	COMDAT _$E275
_TEXT	SEGMENT
_$E275	PROC NEAR					; COMDAT

; 35   : static COleVariant varFalse((short)FALSE), varTrue((short)TRUE);

	push	ebp
	mov	ebp, esp
	push	2
	push	0
	mov	ecx, OFFSET FLAT:_varFalse
	call	??0COleVariant@@QAE@FG@Z		; COleVariant::COleVariant
	push	2
	push	1
	mov	ecx, OFFSET FLAT:_varTrue
	call	??0COleVariant@@QAE@FG@Z		; COleVariant::COleVariant
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E275	ENDP
_TEXT	ENDS
;	COMDAT _$E277
_TEXT	SEGMENT
_$E277	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	push	OFFSET FLAT:_$E276
	call	_atexit
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E277	ENDP
_TEXT	ENDS
;	COMDAT _$E276
_TEXT	SEGMENT
_$E276	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_varTrue
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
	mov	ecx, OFFSET FLAT:_varFalse
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E276	ENDP
_TEXT	ENDS
PUBLIC	??0CMSWordHelper@@QAE@H@Z			; CMSWordHelper::CMSWordHelper
PUBLIC	?IsWordInstalled@CMSWordHelper@@SAHH@Z		; CMSWordHelper::IsWordInstalled
PUBLIC	?RestartWord@CMSWordHelper@@KAHXZ		; CMSWordHelper::RestartWord
PUBLIC	??_7CMSWordHelper@@6B@				; CMSWordHelper::`vftable'
PUBLIC	??_GCMSWordHelper@@UAEPAXI@Z			; CMSWordHelper::`scalar deleting destructor'
PUBLIC	??_ECMSWordHelper@@UAEPAXI@Z			; CMSWordHelper::`vector deleting destructor'
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CStringArray@@QAE@XZ:NEAR			; CStringArray::CStringArray
EXTRN	??1CStringArray@@UAE@XZ:NEAR			; CStringArray::~CStringArray
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
;	COMDAT ??_7CMSWordHelper@@6B@
; File D:\_CODE\Shared\MSWordHelper.cpp
CONST	SEGMENT
??_7CMSWordHelper@@6B@ DD FLAT:??_ECMSWordHelper@@UAEPAXI@Z ; CMSWordHelper::`vftable'
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77338	DD	019930520H
	DD	01H
	DD	FLAT:$T77341
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77341	DD	0ffffffffH
	DD	FLAT:$L77334
xdata$x	ENDS
;	COMDAT ??0CMSWordHelper@@QAE@H@Z
_TEXT	SEGMENT
_nMinVersion$ = 8
_this$ = -16
__$EHRec$ = -12
??0CMSWordHelper@@QAE@H@Z PROC NEAR			; CMSWordHelper::CMSWordHelper, COMDAT

; 46   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77339
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
	add	ecx, 4
	call	??0CStringArray@@QAE@XZ			; CStringArray::CStringArray
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CMSWordHelper@@6B@ ; CMSWordHelper::`vftable'

; 47   : 	if (s_pWord == NULL && IsWordInstalled(nMinVersion))

	cmp	DWORD PTR ?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A, 0 ; CMSWordHelper::s_pWord
	jne	SHORT $L77037
	mov	ecx, DWORD PTR _nMinVersion$[ebp]
	push	ecx
	call	?IsWordInstalled@CMSWordHelper@@SAHH@Z	; CMSWordHelper::IsWordInstalled
	add	esp, 4
	test	eax, eax
	je	SHORT $L77037
$L77038:

; 49   : 		ASSERT(s_nRefCount == 0);

	cmp	DWORD PTR ?s_nRefCount@CMSWordHelper@@1HA, 0 ; CMSWordHelper::s_nRefCount
	je	SHORT $L77041
	push	49					; 00000031H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L77041
	int	3
$L77041:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L77038

; 50   : 		RestartWord();

	call	?RestartWord@CMSWordHelper@@KAHXZ	; CMSWordHelper::RestartWord
$L77037:

; 52   : 
; 53   : 	// ref count even on failure
; 54   : 	s_nRefCount++;

	mov	eax, DWORD PTR ?s_nRefCount@CMSWordHelper@@1HA ; CMSWordHelper::s_nRefCount
	add	eax, 1
	mov	DWORD PTR ?s_nRefCount@CMSWordHelper@@1HA, eax ; CMSWordHelper::s_nRefCount

; 55   : }

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
$L77334:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	ret	0
$L77339:
	mov	eax, OFFSET FLAT:$T77338
	jmp	___CxxFrameHandler
text$x	ENDS
??0CMSWordHelper@@QAE@H@Z ENDP				; CMSWordHelper::CMSWordHelper
PUBLIC	??1CMSWordHelper@@UAE@XZ			; CMSWordHelper::~CMSWordHelper
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCMSWordHelper@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCMSWordHelper@@UAEPAXI@Z PROC NEAR			; CMSWordHelper::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CMSWordHelper@@UAE@XZ		; CMSWordHelper::~CMSWordHelper
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L77045
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L77045:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCMSWordHelper@@UAEPAXI@Z ENDP			; CMSWordHelper::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??_G_Application@WordAPI@@QAEPAXI@Z		; WordAPI::_Application::`scalar deleting destructor'
EXTRN	?Quit@_Application@WordAPI@@QAEXPAUtagVARIANT@@00@Z:NEAR ; WordAPI::_Application::Quit
EXTRN	??BCOleVariant@@QAEPAUtagVARIANT@@XZ:NEAR	; COleVariant::operator tagVARIANT *
;	COMDAT xdata$x
; File D:\_CODE\Shared\MSWordHelper.cpp
xdata$x	SEGMENT
$T77357	DD	019930520H
	DD	03H
	DD	FLAT:$T77359
	DD	01H
	DD	FLAT:$T77360
	DD	2 DUP(00H)
	ORG $+4
$T77359	DD	0ffffffffH
	DD	FLAT:$L77355
	DD	00H
	DD	00H
	DD	00H
	DD	00H
$T77360	DD	01H
	DD	01H
	DD	02H
	DD	01H
	DD	FLAT:$T77361
	ORG $+4
$T77361	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L77354
xdata$x	ENDS
;	COMDAT ??1CMSWordHelper@@UAE@XZ
_TEXT	SEGMENT
_this$ = -28
$T77349 = -20
$T77350 = -24
__$EHRec$ = -16
??1CMSWordHelper@@UAE@XZ PROC NEAR			; CMSWordHelper::~CMSWordHelper, COMDAT

; 58   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77358
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	sub	esp, 28					; 0000001cH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR __$EHRec$[ebp], esp
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-44], eax
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CMSWordHelper@@6B@ ; CMSWordHelper::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+12], 0

; 59   : 	s_nRefCount--;

	mov	ecx, DWORD PTR ?s_nRefCount@CMSWordHelper@@1HA ; CMSWordHelper::s_nRefCount
	sub	ecx, 1
	mov	DWORD PTR ?s_nRefCount@CMSWordHelper@@1HA, ecx ; CMSWordHelper::s_nRefCount

; 60   : 
; 61   : 	if (s_nRefCount == 0 && s_pWord)

	cmp	DWORD PTR ?s_nRefCount@CMSWordHelper@@1HA, 0 ; CMSWordHelper::s_nRefCount
	jne	$L77049
	cmp	DWORD PTR ?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A, 0 ; CMSWordHelper::s_pWord
	je	$L77049

; 63   : 		try
; 64   : 		{

	mov	BYTE PTR __$EHRec$[ebp+12], 1

; 65   : 			s_pWord->Quit(varZero,   // SaveChanges
; 66   : 						  varZero,   // OriginalFormat
; 67   : 						  varFalse); // RouteDocument

	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -32+[ebp], eax
	mov	ecx, OFFSET FLAT:_varZero
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -36+[ebp], eax
	mov	ecx, OFFSET FLAT:_varZero
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -40+[ebp], eax
	mov	edx, DWORD PTR -32+[ebp]
	push	edx
	mov	eax, DWORD PTR -36+[ebp]
	push	eax
	mov	ecx, DWORD PTR -40+[ebp]
	push	ecx
	mov	ecx, DWORD PTR ?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A ; CMSWordHelper::s_pWord
	call	?Quit@_Application@WordAPI@@QAEXPAUtagVARIANT@@00@Z ; WordAPI::_Application::Quit

; 69   : 		catch(...)

	jmp	SHORT $L77050
$L77354:

; 70   : 		{
; 71   : 			// do nothing
; 72   : 		}

	mov	eax, $L77050
	ret	0
$L77050:

; 73   : 
; 74   : 		delete s_pWord;

	mov	DWORD PTR __$EHRec$[ebp+12], 0
	mov	edx, DWORD PTR ?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A ; CMSWordHelper::s_pWord
	mov	DWORD PTR $T77350[ebp], edx
	mov	eax, DWORD PTR $T77350[ebp]
	mov	DWORD PTR $T77349[ebp], eax
	cmp	DWORD PTR $T77349[ebp], 0
	je	SHORT $L77351
	push	1
	mov	ecx, DWORD PTR $T77349[ebp]
	call	??_G_Application@WordAPI@@QAEPAXI@Z	; WordAPI::_Application::`scalar deleting destructor'
	mov	DWORD PTR -44+[ebp], eax
	jmp	SHORT $L77352
$L77351:
	mov	DWORD PTR -44+[ebp], 0
$L77352:

; 75   : 		s_pWord = NULL;

	mov	DWORD PTR ?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A, 0 ; CMSWordHelper::s_pWord
$L77049:

; 77   : }

	mov	DWORD PTR __$EHRec$[ebp+12], -1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	mov	ecx, DWORD PTR __$EHRec$[ebp+4]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 44					; 0000002cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77355:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	ret	0
$L77358:
	mov	eax, OFFSET FLAT:$T77357
	jmp	___CxxFrameHandler
text$x	ENDS
??1CMSWordHelper@@UAE@XZ ENDP				; CMSWordHelper::~CMSWordHelper
PUBLIC	??1_Application@WordAPI@@QAE@XZ			; WordAPI::_Application::~_Application
;	COMDAT ??_G_Application@WordAPI@@QAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_G_Application@WordAPI@@QAEPAXI@Z PROC NEAR		; WordAPI::_Application::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1_Application@WordAPI@@QAE@XZ		; WordAPI::_Application::~_Application
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L77056
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L77056:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_G_Application@WordAPI@@QAEPAXI@Z ENDP		; WordAPI::_Application::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	??1COleDispatchDriver@@QAE@XZ:NEAR		; COleDispatchDriver::~COleDispatchDriver
;	COMDAT ??1_Application@WordAPI@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1_Application@WordAPI@@QAE@XZ PROC NEAR		; WordAPI::_Application::~_Application, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1COleDispatchDriver@@QAE@XZ		; COleDispatchDriver::~COleDispatchDriver
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1_Application@WordAPI@@QAE@XZ ENDP			; WordAPI::_Application::~_Application
_TEXT	ENDS
PUBLIC	?GetInstalledWordVersion@CMSWordHelper@@SAHXZ	; CMSWordHelper::GetInstalledWordVersion
EXTRN	?IsRegisteredApp@CFileRegister@@SAHPBG0H@Z:NEAR	; CFileRegister::IsRegisteredApp
;	COMDAT data
; File D:\_CODE\Shared\MSWordHelper.cpp
data	SEGMENT
$SG77063 DB	'W', 00H, 'I', 00H, 'N', 00H, 'W', 00H, 'O', 00H, 'R', 00H
	DB	'D', 00H, '.', 00H, 'E', 00H, 'X', 00H, 'E', 00H, 00H, 00H
$SG77064 DB	'd', 00H, 'o', 00H, 'c', 00H, 'x', 00H, 00H, 00H
data	ENDS
;	COMDAT ?IsWordInstalled@CMSWordHelper@@SAHH@Z
_TEXT	SEGMENT
_nMinVersion$ = 8
?IsWordInstalled@CMSWordHelper@@SAHH@Z PROC NEAR	; CMSWordHelper::IsWordInstalled, COMDAT

; 80   : {

	push	ebp
	mov	ebp, esp

; 81   : 	if (CFileRegister::IsRegisteredApp(_T("docx"), _T("WINWORD.EXE"), TRUE))

	push	1
	push	OFFSET FLAT:$SG77063
	push	OFFSET FLAT:$SG77064
	call	?IsRegisteredApp@CFileRegister@@SAHPBG0H@Z ; CFileRegister::IsRegisteredApp
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	SHORT $L77062

; 83   : 		return (GetInstalledWordVersion() >= nMinVersion);

	call	?GetInstalledWordVersion@CMSWordHelper@@SAHXZ ; CMSWordHelper::GetInstalledWordVersion
	xor	ecx, ecx
	cmp	eax, DWORD PTR _nMinVersion$[ebp]
	setge	cl
	mov	eax, ecx
	jmp	SHORT $L77061
$L77062:

; 85   : 
; 86   : 	// else
; 87   : 	return FALSE;

	xor	eax, eax
$L77061:

; 88   : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?IsWordInstalled@CMSWordHelper@@SAHH@Z ENDP		; CMSWordHelper::IsWordInstalled
_TEXT	ENDS
EXTRN	?IsNumber@Misc@@YAHABVCString@@@Z:NEAR		; Misc::IsNumber
EXTRN	__imp___wtoi:NEAR
EXTRN	??0CRegKey@@QAE@XZ:NEAR				; CRegKey::CRegKey
EXTRN	??1CRegKey@@UAE@XZ:NEAR				; CRegKey::~CRegKey
EXTRN	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z:NEAR	; CRegKey::Open
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	?Read@CRegKey@@QBEJPBGAAVCString@@@Z:NEAR	; CRegKey::Read
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	?Mid@CString@@QBE?AV1@H@Z:NEAR			; CString::Mid
EXTRN	?ReverseFind@CString@@QBEHG@Z:NEAR		; CString::ReverseFind
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
_BSS	SEGMENT
$SG77072 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\MSWordHelper.cpp
data	SEGMENT
$SG77070 DB	'W', 00H, 'o', 00H, 'r', 00H, 'd', 00H, '.', 00H, 'A', 00H
	DB	'p', 00H, 'p', 00H, 'l', 00H, 'i', 00H, 'c', 00H, 'a', 00H, 't'
	DB	00H, 'i', 00H, 'o', 00H, 'n', 00H, '\', 00H, 'C', 00H, 'u', 00H
	DB	'r', 00H, 'V', 00H, 'e', 00H, 'r', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77380	DD	019930520H
	DD	03H
	DD	FLAT:$T77382
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77382	DD	0ffffffffH
	DD	FLAT:$L77376
	DD	00H
	DD	FLAT:$L77377
	DD	01H
	DD	FLAT:$L77378
xdata$x	ENDS
;	COMDAT ?GetInstalledWordVersion@CMSWordHelper@@SAHXZ
_TEXT	SEGMENT
_reg$ = -28
_sCurVer$77071 = -36
_nLastDot$77073 = -32
_sVersion$77075 = -40
$T77374 = -44
$T77375 = -48
__$EHRec$ = -12
?GetInstalledWordVersion@CMSWordHelper@@SAHXZ PROC NEAR	; CMSWordHelper::GetInstalledWordVersion, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77381
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 36					; 00000024H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-48], eax
	mov	DWORD PTR [ebp-44], eax
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 92   : 	CRegKey reg;

	lea	ecx, DWORD PTR _reg$[ebp]
	call	??0CRegKey@@QAE@XZ			; CRegKey::CRegKey
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 93   : 	
; 94   : 	if (reg.Open(HKEY_CLASSES_ROOT, _T("Word.Application\\CurVer")) == ERROR_SUCCESS)

	push	OFFSET FLAT:$SG77070
	push	-2147483648				; 80000000H
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open
	test	eax, eax
	jne	$L77069

; 96   : 		CString sCurVer;

	lea	ecx, DWORD PTR _sCurVer$77071[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 97   : 		reg.Read(_T(""), sCurVer);

	lea	eax, DWORD PTR _sCurVer$77071[ebp]
	push	eax
	push	OFFSET FLAT:$SG77072
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Read@CRegKey@@QBEJPBGAAVCString@@@Z	; CRegKey::Read

; 98   : 		
; 99   : 		int nLastDot = sCurVer.ReverseFind('.');

	push	46					; 0000002eH
	lea	ecx, DWORD PTR _sCurVer$77071[ebp]
	call	?ReverseFind@CString@@QBEHG@Z		; CString::ReverseFind
	mov	DWORD PTR _nLastDot$77073[ebp], eax

; 100  : 		
; 101  : 		if (nLastDot != -1)

	cmp	DWORD PTR _nLastDot$77073[ebp], -1
	je	SHORT $L77074

; 103  : 			CString sVersion = sCurVer.Mid(nLastDot + 1);

	mov	ecx, DWORD PTR _nLastDot$77073[ebp]
	add	ecx, 1
	push	ecx
	lea	edx, DWORD PTR _sVersion$77075[ebp]
	push	edx
	lea	ecx, DWORD PTR _sCurVer$77071[ebp]
	call	?Mid@CString@@QBE?AV1@H@Z		; CString::Mid
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 104  : 			
; 105  : 			if (Misc::IsNumber(sVersion))

	lea	eax, DWORD PTR _sVersion$77075[ebp]
	push	eax
	call	?IsNumber@Misc@@YAHABVCString@@@Z	; Misc::IsNumber
	add	esp, 4
	test	eax, eax
	je	SHORT $L77077

; 106  : 				return _ttoi(sVersion);

	lea	ecx, DWORD PTR _sVersion$77075[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp___wtoi
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR $T77374[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sVersion$77075[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sCurVer$77071[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T77374[ebp]
	jmp	SHORT $L77066
$L77077:

; 107  : 		}

	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sVersion$77075[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77074:

; 108  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sCurVer$77071[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77069:

; 109  : 
; 110  : 	// all else
; 111  : 	return -1;

	mov	DWORD PTR $T77375[ebp], -1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T77375[ebp]
$L77066:

; 112  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 48					; 00000030H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77376:
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	ret	0
$L77377:
	lea	ecx, DWORD PTR _sCurVer$77071[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77378:
	lea	ecx, DWORD PTR _sVersion$77075[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77381:
	mov	eax, OFFSET FLAT:$T77380
	jmp	___CxxFrameHandler
text$x	ENDS
?GetInstalledWordVersion@CMSWordHelper@@SAHXZ ENDP	; CMSWordHelper::GetInstalledWordVersion
PUBLIC	??0_Application@WordAPI@@QAE@XZ			; WordAPI::_Application::_Application
EXTRN	?CreateDispatch@COleDispatchDriver@@QAEHPBGPAVCOleException@@@Z:NEAR ; COleDispatchDriver::CreateDispatch
EXTRN	?DetachDispatch@COleDispatchDriver@@QAEPAUIDispatch@@XZ:NEAR ; COleDispatchDriver::DetachDispatch
EXTRN	??2@YAPAXIPBDH@Z:NEAR				; operator new
EXTRN	??3@YAXPAXPBDH@Z:NEAR				; operator delete
;	COMDAT data
; File D:\_CODE\Shared\MSWordHelper.cpp
data	SEGMENT
$SG77091 DB	'W', 00H, 'o', 00H, 'r', 00H, 'd', 00H, '.', 00H, 'A', 00H
	DB	'p', 00H, 'p', 00H, 'l', 00H, 'i', 00H, 'c', 00H, 'a', 00H, 't'
	DB	00H, 'i', 00H, 'o', 00H, 'n', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77398	DD	019930520H
	DD	01H
	DD	FLAT:$T77400
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77400	DD	0ffffffffH
	DD	FLAT:$L77396
xdata$x	ENDS
;	COMDAT ?RestartWord@CMSWordHelper@@KAHXZ
_TEXT	SEGMENT
$T77388 = -16
$T77389 = -20
$T77392 = -24
$T77393 = -28
__$EHRec$ = -12
?RestartWord@CMSWordHelper@@KAHXZ PROC NEAR		; CMSWordHelper::RestartWord, COMDAT

; 115  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77399
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

; 116  : 	if (s_pWord == NULL)

	cmp	DWORD PTR ?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A, 0 ; CMSWordHelper::s_pWord
	jne	SHORT $L77082

; 118  : 		// first time
; 119  : 		s_pWord = new WordAPI::_Application;

	push	119					; 00000077H
	push	OFFSET FLAT:_THIS_FILE
	push	8
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77389[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	cmp	DWORD PTR $T77389[ebp], 0
	je	SHORT $L77390
	mov	ecx, DWORD PTR $T77389[ebp]
	call	??0_Application@WordAPI@@QAE@XZ		; WordAPI::_Application::_Application
	mov	DWORD PTR -32+[ebp], eax
	jmp	SHORT $L77391
$L77390:
	mov	DWORD PTR -32+[ebp], 0
$L77391:
	mov	eax, DWORD PTR -32+[ebp]
	mov	DWORD PTR $T77388[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR $T77388[ebp]
	mov	DWORD PTR ?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A, ecx ; CMSWordHelper::s_pWord

; 121  : 	else

	jmp	SHORT $L77089
$L77082:

; 123  : 		// detach the interface without Releasing since we are
; 124  : 		// assuming that the interface is cactus
; 125  : 		s_pWord->DetachDispatch();

	mov	ecx, DWORD PTR ?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A ; CMSWordHelper::s_pWord
	call	?DetachDispatch@COleDispatchDriver@@QAEPAUIDispatch@@XZ ; COleDispatchDriver::DetachDispatch
$L77089:

; 127  : 
; 128  : 	// try to restart word
; 129  : 	if (!s_pWord->CreateDispatch(_T("Word.Application")))

	push	0
	push	OFFSET FLAT:$SG77091
	mov	ecx, DWORD PTR ?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A ; CMSWordHelper::s_pWord
	call	?CreateDispatch@COleDispatchDriver@@QAEHPBGPAVCOleException@@@Z ; COleDispatchDriver::CreateDispatch
	test	eax, eax
	jne	SHORT $L77090

; 131  : 		delete s_pWord;

	mov	edx, DWORD PTR ?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A ; CMSWordHelper::s_pWord
	mov	DWORD PTR $T77393[ebp], edx
	mov	eax, DWORD PTR $T77393[ebp]
	mov	DWORD PTR $T77392[ebp], eax
	cmp	DWORD PTR $T77392[ebp], 0
	je	SHORT $L77394
	push	1
	mov	ecx, DWORD PTR $T77392[ebp]
	call	??_G_Application@WordAPI@@QAEPAXI@Z	; WordAPI::_Application::`scalar deleting destructor'
	mov	DWORD PTR -36+[ebp], eax
	jmp	SHORT $L77395
$L77394:
	mov	DWORD PTR -36+[ebp], 0
$L77395:

; 132  : 		s_pWord = NULL;

	mov	DWORD PTR ?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A, 0 ; CMSWordHelper::s_pWord

; 133  : 
; 134  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L77081
$L77090:

; 136  : 
; 137  : 	// else all's well
; 138  : 	return TRUE;

	mov	eax, 1
$L77081:

; 139  : }

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
$L77396:
	push	119					; 00000077H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR $T77389[ebp]
	push	eax
	call	??3@YAXPAXPBDH@Z			; operator delete
	add	esp, 12					; 0000000cH
	ret	0
$L77399:
	mov	eax, OFFSET FLAT:$T77398
	jmp	___CxxFrameHandler
text$x	ENDS
?RestartWord@CMSWordHelper@@KAHXZ ENDP			; CMSWordHelper::RestartWord
EXTRN	??0COleDispatchDriver@@QAE@XZ:NEAR		; COleDispatchDriver::COleDispatchDriver
;	COMDAT ??0_Application@WordAPI@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0_Application@WordAPI@@QAE@XZ PROC NEAR		; WordAPI::_Application::_Application, COMDAT

; 11   : 	_Application() {}		// Calls COleDispatchDriver default constructor

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0COleDispatchDriver@@QAE@XZ		; COleDispatchDriver::COleDispatchDriver
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0_Application@WordAPI@@QAE@XZ ENDP			; WordAPI::_Application::_Application
_TEXT	ENDS
PUBLIC	?IsValid@CMSWordHelper@@QBEHXZ			; CMSWordHelper::IsValid
PUBLIC	?ConvertContent@CMSWordHelper@@QAEHABVCString@@W4WdOpenFormat@WordAPI@@AAV2@W4WdSaveFormat@4@W4MsoEncoding@4@@Z ; CMSWordHelper::ConvertContent
PUBLIC	?ConvertFile@CMSWordHelper@@QAEHABVCString@@W4WdOpenFormat@WordAPI@@0W4WdSaveFormat@4@W4MsoEncoding@4@@Z ; CMSWordHelper::ConvertFile
EXTRN	?GetTempFileNameW@FileMisc@@YA?AVCString@@PBGI@Z:NEAR ; FileMisc::GetTempFileNameW
EXTRN	?LoadFile@FileMisc@@YAHPBGAAVCString@@H@Z:NEAR	; FileMisc::LoadFile
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
EXTRN	?Empty@CString@@QAEXXZ:NEAR			; CString::Empty
EXTRN	?SaveFile@FileMisc@@YAHPBGABVCString@@W4SFE_SAVEAS@@@Z:NEAR ; FileMisc::SaveFile
EXTRN	__imp__DeleteFileW@4:NEAR
;	COMDAT data
; File D:\_CODE\Shared\MSWordHelper.cpp
data	SEGMENT
$SG77106 DB	't', 00H, 'e', 00H, 'm', 00H, 'p', 00H, 00H, 00H
	ORG $+2
$SG77109 DB	't', 00H, 'e', 00H, 'm', 00H, 'p', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77413	DD	019930520H
	DD	02H
	DD	FLAT:$T77415
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77415	DD	0ffffffffH
	DD	FLAT:$L77410
	DD	00H
	DD	FLAT:$L77411
xdata$x	ENDS
;	COMDAT ?ConvertContent@CMSWordHelper@@QAEHABVCString@@W4WdOpenFormat@WordAPI@@AAV2@W4WdSaveFormat@4@W4MsoEncoding@4@@Z
_TEXT	SEGMENT
_sInput$ = 8
_nInputFormat$ = 12
_sOutput$ = 16
_nOutputFormat$ = 20
_nEncoding$ = 24
_this$ = -32
_sInputFilename$ = -24
_sOutputFilename$ = -16
_nSaveAs$ = -20
$T77409 = -28
__$EHRec$ = -12
?ConvertContent@CMSWordHelper@@QAEHABVCString@@W4WdOpenFormat@WordAPI@@AAV2@W4WdSaveFormat@4@W4MsoEncoding@4@@Z PROC NEAR ; CMSWordHelper::ConvertContent, COMDAT

; 144  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77414
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 20					; 00000014H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 145  : 	if (sInput.IsEmpty())

	mov	ecx, DWORD PTR _sInput$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L77102

; 147  : 		sOutput.Empty();

	mov	ecx, DWORD PTR _sOutput$[ebp]
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 148  : 		return TRUE;

	mov	eax, 1
	jmp	$L77101
$L77102:

; 150  : 
; 151  : 	if (!IsValid())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsValid@CMSWordHelper@@QBEHXZ		; CMSWordHelper::IsValid
	test	eax, eax
	jne	SHORT $L77103

; 152  : 		return FALSE;

	xor	eax, eax
	jmp	$L77101
$L77103:

; 153  : 
; 154  : 	CString sInputFilename = FileMisc::GetTempFileName(_T("temp"));

	push	0
	push	OFFSET FLAT:$SG77106
	lea	eax, DWORD PTR _sInputFilename$[ebp]
	push	eax
	call	?GetTempFileNameW@FileMisc@@YA?AVCString@@PBGI@Z ; FileMisc::GetTempFileNameW
	add	esp, 12					; 0000000cH
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 155  : 	CString sOutputFilename = FileMisc::GetTempFileName(_T("temp"));

	push	0
	push	OFFSET FLAT:$SG77109
	lea	ecx, DWORD PTR _sOutputFilename$[ebp]
	push	ecx
	call	?GetTempFileNameW@FileMisc@@YA?AVCString@@PBGI@Z ; FileMisc::GetTempFileNameW
	add	esp, 12					; 0000000cH
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 156  : 	
; 157  : 	// rtf must be saved as multibyte
; 158  : 	SFE_SAVEAS nSaveAs = (nInputFormat == wdOpenFormatRTF) ? SFE_MULTIBYTE : SFE_ASCOMPILED;

	mov	edx, DWORD PTR _nInputFormat$[ebp]
	sub	edx, 3
	neg	edx
	sbb	edx, edx
	and	edx, 2
	mov	DWORD PTR _nSaveAs$[ebp], edx

; 159  : 
; 160  : 	if (FileMisc::SaveFile(sInputFilename, sInput, nSaveAs))

	mov	eax, DWORD PTR _nSaveAs$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sInput$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sInputFilename$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?SaveFile@FileMisc@@YAHPBGABVCString@@W4SFE_SAVEAS@@@Z ; FileMisc::SaveFile
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	SHORT $L77112

; 162  : 		::DeleteFile(sOutputFilename);

	lea	ecx, DWORD PTR _sOutputFilename$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__DeleteFileW@4
	cmp	esi, esp
	call	__chkesp

; 163  : 
; 164  : 		if (ConvertFile(sInputFilename, nInputFormat, sOutputFilename, nOutputFormat, nEncoding))

	mov	edx, DWORD PTR _nEncoding$[ebp]
	push	edx
	mov	eax, DWORD PTR _nOutputFormat$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sOutputFilename$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nInputFormat$[ebp]
	push	edx
	lea	eax, DWORD PTR _sInputFilename$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ConvertFile@CMSWordHelper@@QAEHABVCString@@W4WdOpenFormat@WordAPI@@0W4WdSaveFormat@4@W4MsoEncoding@4@@Z ; CMSWordHelper::ConvertFile
	test	eax, eax
	je	SHORT $L77112

; 166  : 			FileMisc::LoadFile(sOutputFilename, sOutput);

	push	0
	mov	ecx, DWORD PTR _sOutput$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sOutputFilename$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?LoadFile@FileMisc@@YAHPBGAAVCString@@H@Z ; FileMisc::LoadFile
	add	esp, 12					; 0000000cH
$L77112:

; 169  : 
; 170  : 	// cleanup
; 171  : 	::DeleteFile(sInputFilename);

	lea	ecx, DWORD PTR _sInputFilename$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__DeleteFileW@4
	cmp	esi, esp
	call	__chkesp

; 172  : 	::DeleteFile(sOutputFilename);

	lea	ecx, DWORD PTR _sOutputFilename$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__DeleteFileW@4
	cmp	esi, esp
	call	__chkesp

; 173  : 
; 174  : 	return !sOutput.IsEmpty();

	mov	ecx, DWORD PTR _sOutput$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR $T77409[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sOutputFilename$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sInputFilename$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77409[ebp]
$L77101:

; 175  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	20					; 00000014H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77410:
	lea	ecx, DWORD PTR _sInputFilename$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77411:
	lea	ecx, DWORD PTR _sOutputFilename$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77414:
	mov	eax, OFFSET FLAT:$T77413
	jmp	___CxxFrameHandler
text$x	ENDS
?ConvertContent@CMSWordHelper@@QAEHABVCString@@W4WdOpenFormat@WordAPI@@AAV2@W4WdSaveFormat@4@W4MsoEncoding@4@@Z ENDP ; CMSWordHelper::ConvertContent
;	COMDAT ?IsValid@CMSWordHelper@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?IsValid@CMSWordHelper@@QBEHXZ PROC NEAR		; CMSWordHelper::IsValid, COMDAT

; 89   : 	BOOL IsValid() const { return (s_pWord != NULL); }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	xor	eax, eax
	cmp	DWORD PTR ?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A, 0 ; CMSWordHelper::s_pWord
	setne	al
	mov	esp, ebp
	pop	ebp
	ret	0
?IsValid@CMSWordHelper@@QBEHXZ ENDP			; CMSWordHelper::IsValid
_TEXT	ENDS
PUBLIC	??1_Document@WordAPI@@QAE@XZ			; WordAPI::_Document::~_Document
PUBLIC	??_R0PAVCOleException@@@8			; COleException * `RTTI Type Descriptor'
PUBLIC	??0Documents@WordAPI@@QAE@XZ			; WordAPI::Documents::Documents
PUBLIC	??1Documents@WordAPI@@QAE@XZ			; WordAPI::Documents::~Documents
PUBLIC	??0_Document@WordAPI@@QAE@PAUIDispatch@@@Z	; WordAPI::_Document::_Document
EXTRN	?GetDocuments@_Application@WordAPI@@QAEPAUIDispatch@@XZ:NEAR ; WordAPI::_Application::GetDocuments
EXTRN	?SaveAs@_Document@WordAPI@@QAEXPAUtagVARIANT@@000000000000000@Z:NEAR ; WordAPI::_Document::SaveAs
EXTRN	?Delete@CException@@QAEXXZ:NEAR			; CException::Delete
EXTRN	?AttachDispatch@COleDispatchDriver@@QAEXPAUIDispatch@@H@Z:NEAR ; COleDispatchDriver::AttachDispatch
EXTRN	??_7type_info@@6B@:BYTE				; type_info::`vftable'
EXTRN	??0COleVariant@@QAE@JG@Z:NEAR			; COleVariant::COleVariant
EXTRN	?Open@Documents@WordAPI@@QAEPAUIDispatch@@PAUtagVARIANT@@000000000000000@Z:NEAR ; WordAPI::Documents::Open
EXTRN	?FileExists@FileMisc@@YAHPBG@Z:NEAR		; FileMisc::FileExists
EXTRN	?Close@_Document@WordAPI@@QAEXPAUtagVARIANT@@00@Z:NEAR ; WordAPI::_Document::Close
;	COMDAT ??_R0PAVCOleException@@@8
; File D:\_CODE\Shared\MSWordHelper.cpp
_DATA	SEGMENT
??_R0PAVCOleException@@@8 DD FLAT:??_7type_info@@6B@	; COleException * `RTTI Type Descriptor'
	DD	00H
	DB	'.PAVCOleException@@', 00H
_DATA	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77451	DD	019930520H
	DD	0eH
	DD	FLAT:$T77453
	DD	03H
	DD	FLAT:$T77454
	DD	2 DUP(00H)
	ORG $+4
$T77453	DD	0ffffffffH
	DD	FLAT:$L77440
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	03H
	DD	FLAT:$L77441
	DD	04H
	DD	FLAT:$L77442
	DD	05H
	DD	FLAT:$L77443
	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L77444
	DD	08H
	DD	00H
	DD	09H
	DD	FLAT:$L77445
	DD	0aH
	DD	FLAT:$L77446
	DD	0bH
	DD	FLAT:$L77447
	DD	08H
	DD	00H
$T77454	DD	01H
	DD	01H
	DD	02H
	DD	02H
	DD	FLAT:$T77455
	DD	03H
	DD	06H
	DD	07H
	DD	01H
	DD	FLAT:$T77456
	DD	09H
	DD	0cH
	DD	0dH
	DD	01H
	DD	FLAT:$T77457
	ORG $+4
$T77455	DD	00H
	DD	FLAT:??_R0PAVCOleException@@@8
	DD	0ffffffd4H
	DD	FLAT:$L77434
	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L77435
$T77456	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L77437
$T77457	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L77439
xdata$x	ENDS
;	COMDAT ?ConvertFile@CMSWordHelper@@QAEHABVCString@@W4WdOpenFormat@WordAPI@@0W4WdSaveFormat@4@W4MsoEncoding@4@@Z
_TEXT	SEGMENT
_sInputFilename$ = 8
_nInputFormat$ = 12
_sOutputFilename$ = 16
_nOutputFormat$ = 20
_nEncoding$ = 24
_this$ = -164
_docs$ = -36
_e$77127 = -44
_sc$77129 = -48
_lpd$ = -40
_doc$ = -28
_bSuccess$ = -20
$T77423 = -52
$T77424 = -56
$T77425 = -72
$T77426 = -88
$T77427 = -104
$T77428 = -108
$T77429 = -124
$T77430 = -140
$T77431 = -156
$T77432 = -160
__$EHRec$ = -16
?ConvertFile@CMSWordHelper@@QAEHABVCString@@W4WdOpenFormat@WordAPI@@0W4WdSaveFormat@4@W4MsoEncoding@4@@Z PROC NEAR ; CMSWordHelper::ConvertFile, COMDAT

; 180  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77452
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	sub	esp, 340				; 00000154H
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR __$EHRec$[ebp], esp
	push	ecx
	lea	edi, DWORD PTR [ebp-356]
	mov	ecx, 85					; 00000055H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 181  : 	if (!IsValid())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsValid@CMSWordHelper@@QBEHXZ		; CMSWordHelper::IsValid
	test	eax, eax
	jne	SHORT $L77122

; 182  : 		return FALSE;

	xor	eax, eax
	jmp	$L77121
$L77122:

; 183  : 
; 184  : 	if (!FileMisc::FileExists(sInputFilename))

	mov	ecx, DWORD PTR _sInputFilename$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?FileExists@FileMisc@@YAHPBG@Z		; FileMisc::FileExists
	add	esp, 4
	test	eax, eax
	jne	SHORT $L77123

; 185  : 		return FALSE;

	xor	eax, eax
	jmp	$L77121
$L77123:

; 186  : 
; 187  : 	// caller is responsible for ensuring output file does not exist
; 188  : 	if (FileMisc::FileExists(sOutputFilename))

	mov	ecx, DWORD PTR _sOutputFilename$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?FileExists@FileMisc@@YAHPBG@Z		; FileMisc::FileExists
	add	esp, 4
	test	eax, eax
	je	SHORT $L77124

; 189  : 		return FALSE;

	xor	eax, eax
	jmp	$L77121
$L77124:

; 190  : 
; 191  : 	// get document object first. This is a great way to validate
; 192  : 	// that the Word object is still operating.
; 193  : 	Documents docs;

	lea	ecx, DWORD PTR _docs$[ebp]
	call	??0Documents@WordAPI@@QAE@XZ		; WordAPI::Documents::Documents
	mov	DWORD PTR __$EHRec$[ebp+12], 0

; 194  : 
; 195  : 	try
; 196  : 	{

	mov	BYTE PTR __$EHRec$[ebp+12], 1

; 197  : 		docs.AttachDispatch(s_pWord->GetDocuments());

	mov	ecx, DWORD PTR ?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A ; CMSWordHelper::s_pWord
	call	?GetDocuments@_Application@WordAPI@@QAEPAUIDispatch@@XZ ; WordAPI::_Application::GetDocuments
	mov	DWORD PTR -168+[ebp], eax
	push	1
	mov	eax, DWORD PTR -168+[ebp]
	push	eax
	lea	ecx, DWORD PTR _docs$[ebp]
	call	?AttachDispatch@COleDispatchDriver@@QAEXPAUIDispatch@@H@Z ; COleDispatchDriver::AttachDispatch

; 199  : 	catch (COleException *e) 

	jmp	$L77126
$L77434:

; 200  : 	{ 
; 201  : 		SCODE sc = e->m_sc;

	mov	ecx, DWORD PTR _e$77127[ebp]
	mov	edx, DWORD PTR [ecx+12]
	mov	DWORD PTR _sc$77129[ebp], edx

; 202  : 		e->Delete(); 

	mov	ecx, DWORD PTR _e$77127[ebp]
	call	?Delete@CException@@QAEXXZ		; CException::Delete

; 203  : 
; 204  : 		// assume someone has forcibly closed word from under us
; 205  : 		if (sc == RPC_NOT_AVAIL)

	cmp	DWORD PTR _sc$77129[ebp], -2147023174	; 800706baH
	jne	SHORT $L77133

; 207  : 			// try to restart word
; 208  : 			if (!RestartWord())

	call	?RestartWord@CMSWordHelper@@KAHXZ	; CMSWordHelper::RestartWord
	test	eax, eax
	jne	SHORT $L77131

; 209  : 				return FALSE;

	mov	DWORD PTR $T77423[ebp], 0
	mov	eax, $L77448
	ret	0
$L77448:
	mov	DWORD PTR __$EHRec$[ebp+12], -1
	lea	ecx, DWORD PTR _docs$[ebp]
	call	??1Documents@WordAPI@@QAE@XZ		; WordAPI::Documents::~Documents
	mov	eax, DWORD PTR $T77423[ebp]
	jmp	$L77121
$L77131:

; 211  : 				docs.AttachDispatch(s_pWord->GetDocuments());

	push	1
	mov	ecx, DWORD PTR ?s_pWord@CMSWordHelper@@1PAV_Application@WordAPI@@A ; CMSWordHelper::s_pWord
	call	?GetDocuments@_Application@WordAPI@@QAEPAUIDispatch@@XZ ; WordAPI::_Application::GetDocuments
	push	eax
	lea	ecx, DWORD PTR _docs$[ebp]
	call	?AttachDispatch@COleDispatchDriver@@QAEXPAUIDispatch@@H@Z ; COleDispatchDriver::AttachDispatch
$L77133:

; 213  : 	}

	mov	eax, $L77126
	ret	0
$L77435:

; 215  : 	{
; 216  : 		return FALSE;

	mov	DWORD PTR $T77424[ebp], 0
	mov	eax, $L77449
	ret	0
$L77449:
	mov	DWORD PTR __$EHRec$[ebp+12], -1
	lea	ecx, DWORD PTR _docs$[ebp]
	call	??1Documents@WordAPI@@QAE@XZ		; WordAPI::Documents::~Documents
	mov	eax, DWORD PTR $T77424[ebp]
	jmp	$L77121
$L77126:

; 218  : 
; 219  : 	LPDISPATCH lpd = NULL;

	mov	DWORD PTR __$EHRec$[ebp+12], 0
	mov	DWORD PTR _lpd$[ebp], 0

; 220  : 
; 221  : 	try 
; 222  : 	{

	mov	BYTE PTR __$EHRec$[ebp+12], 3

; 223  : 		lpd = docs.Open(COleVariant(sInputFilename),		// FileName
; 224  : 						varFalse,							// ConfirmConversions
; 225  : 						varFalse,							// ReadOnly
; 226  : 						varFalse,							// AddToRecentFiles
; 227  : 						varNull,							// PasswordDocument
; 228  : 						varNull,							// PasswordTemplate
; 229  : 						varTrue,							// Revert
; 230  : 						varNull,							// WritePasswordDocument
; 231  : 						varNull,							// WritePasswordTemplate
; 232  : 						COleVariant((long)nInputFormat),	// Format
; 233  : 						COleVariant((long)nEncoding),		// Encoding
; 234  : 						varFalse,							// Visible
; 235  : 						varFalse,							// OpenAndRepair
; 236  : 						varZero,							// DocumentDirection
; 237  : 						varTrue,							// NoEncodingDialog
; 238  : 						varNull);							// XMLTransform

	mov	ecx, OFFSET FLAT:_varNull
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -172+[ebp], eax
	mov	ecx, OFFSET FLAT:_varTrue
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -176+[ebp], eax
	mov	ecx, OFFSET FLAT:_varZero
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -180+[ebp], eax
	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -184+[ebp], eax
	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -188+[ebp], eax
	push	3
	mov	eax, DWORD PTR _nEncoding$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77425[ebp]
	call	??0COleVariant@@QAE@JG@Z		; COleVariant::COleVariant
	mov	DWORD PTR -192+[ebp], eax
	mov	ecx, DWORD PTR -192+[ebp]
	mov	DWORD PTR -196+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+12], 4
	mov	ecx, DWORD PTR -196+[ebp]
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -200+[ebp], eax
	push	3
	mov	edx, DWORD PTR _nInputFormat$[ebp]
	push	edx
	lea	ecx, DWORD PTR $T77426[ebp]
	call	??0COleVariant@@QAE@JG@Z		; COleVariant::COleVariant
	mov	DWORD PTR -204+[ebp], eax
	mov	eax, DWORD PTR -204+[ebp]
	mov	DWORD PTR -208+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 5
	mov	ecx, DWORD PTR -208+[ebp]
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -212+[ebp], eax
	mov	ecx, OFFSET FLAT:_varNull
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -216+[ebp], eax
	mov	ecx, OFFSET FLAT:_varNull
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -220+[ebp], eax
	mov	ecx, OFFSET FLAT:_varTrue
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -224+[ebp], eax
	mov	ecx, OFFSET FLAT:_varNull
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -228+[ebp], eax
	mov	ecx, OFFSET FLAT:_varNull
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -232+[ebp], eax
	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -236+[ebp], eax
	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -240+[ebp], eax
	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -244+[ebp], eax
	mov	ecx, DWORD PTR _sInputFilename$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -248+[ebp], eax
	mov	ecx, DWORD PTR -248+[ebp]
	push	ecx
	lea	ecx, DWORD PTR $T77427[ebp]
	call	??0COleVariant@@QAE@PBG@Z		; COleVariant::COleVariant
	mov	DWORD PTR -252+[ebp], eax
	mov	edx, DWORD PTR -252+[ebp]
	mov	DWORD PTR -256+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+12], 6
	mov	ecx, DWORD PTR -256+[ebp]
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -260+[ebp], eax
	mov	eax, DWORD PTR -172+[ebp]
	push	eax
	mov	ecx, DWORD PTR -176+[ebp]
	push	ecx
	mov	edx, DWORD PTR -180+[ebp]
	push	edx
	mov	eax, DWORD PTR -184+[ebp]
	push	eax
	mov	ecx, DWORD PTR -188+[ebp]
	push	ecx
	mov	edx, DWORD PTR -200+[ebp]
	push	edx
	mov	eax, DWORD PTR -212+[ebp]
	push	eax
	mov	ecx, DWORD PTR -216+[ebp]
	push	ecx
	mov	edx, DWORD PTR -220+[ebp]
	push	edx
	mov	eax, DWORD PTR -224+[ebp]
	push	eax
	mov	ecx, DWORD PTR -228+[ebp]
	push	ecx
	mov	edx, DWORD PTR -232+[ebp]
	push	edx
	mov	eax, DWORD PTR -236+[ebp]
	push	eax
	mov	ecx, DWORD PTR -240+[ebp]
	push	ecx
	mov	edx, DWORD PTR -244+[ebp]
	push	edx
	mov	eax, DWORD PTR -260+[ebp]
	push	eax
	lea	ecx, DWORD PTR _docs$[ebp]
	call	?Open@Documents@WordAPI@@QAEPAUIDispatch@@PAUtagVARIANT@@000000000000000@Z ; WordAPI::Documents::Open
	mov	DWORD PTR -264+[ebp], eax
	mov	ecx, DWORD PTR -264+[ebp]
	mov	DWORD PTR _lpd$[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+12], 5
	lea	ecx, DWORD PTR $T77427[ebp]
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
	mov	BYTE PTR __$EHRec$[ebp+12], 4
	lea	ecx, DWORD PTR $T77426[ebp]
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
	mov	BYTE PTR __$EHRec$[ebp+12], 3
	lea	ecx, DWORD PTR $T77425[ebp]
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant

; 241  : 	catch (...)

	jmp	SHORT $L77136
$L77437:

; 242  : 	{
; 243  : 		// do nothing
; 244  : 	}

	mov	eax, $L77136
	ret	0
$L77136:

; 245  : 
; 246  : 	if (lpd == NULL)

	mov	DWORD PTR __$EHRec$[ebp+12], 0
	cmp	DWORD PTR _lpd$[ebp], 0
	jne	SHORT $L77142

; 247  : 		return FALSE;

	mov	DWORD PTR $T77428[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+12], -1
	lea	ecx, DWORD PTR _docs$[ebp]
	call	??1Documents@WordAPI@@QAE@XZ		; WordAPI::Documents::~Documents
	mov	eax, DWORD PTR $T77428[ebp]
	jmp	$L77121
$L77142:

; 248  : 
; 249  : 	_Document doc(lpd);

	mov	edx, DWORD PTR _lpd$[ebp]
	push	edx
	lea	ecx, DWORD PTR _doc$[ebp]
	call	??0_Document@WordAPI@@QAE@PAUIDispatch@@@Z ; WordAPI::_Document::_Document
	mov	BYTE PTR __$EHRec$[ebp+12], 8

; 250  : 	BOOL bSuccess = TRUE;

	mov	DWORD PTR _bSuccess$[ebp], 1

; 251  : 
; 252  : 	try 
; 253  : 	{

	mov	BYTE PTR __$EHRec$[ebp+12], 9

; 254  : 		doc.SaveAs(COleVariant(sOutputFilename),		// FileName
; 255  : 					COleVariant((short)nOutputFormat),	// FileFormat
; 256  : 					varFalse,							// LockComments
; 257  : 					varNull,							// Password
; 258  : 					varFalse,							// AddToRecentFiles
; 259  : 					varNull,							// WritePassword
; 260  : 					varFalse,							// ReadOnlyRecommended
; 261  : 					varFalse,							// EmbedTrueTypeFonts
; 262  : 					varFalse,							// SaveNativePictureFormat
; 263  : 					varFalse,							// SaveFormsData
; 264  : 					varFalse,							// SaveAsAOCELetter
; 265  : 					COleVariant((long)nEncoding),		// Encoding
; 266  : 					varFalse,							// InsertLineBreaks
; 267  : 					varFalse,							// AllowSubstitutions
; 268  : 					varOne,								// LineEnding
; 269  : 					varFalse);							// AddBiDiMarks

	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -268+[ebp], eax
	mov	ecx, OFFSET FLAT:_varOne
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -272+[ebp], eax
	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -276+[ebp], eax
	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -280+[ebp], eax
	push	3
	mov	eax, DWORD PTR _nEncoding$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77429[ebp]
	call	??0COleVariant@@QAE@JG@Z		; COleVariant::COleVariant
	mov	DWORD PTR -284+[ebp], eax
	mov	ecx, DWORD PTR -284+[ebp]
	mov	DWORD PTR -288+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+12], 10		; 0000000aH
	mov	ecx, DWORD PTR -288+[ebp]
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -292+[ebp], eax
	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -296+[ebp], eax
	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -300+[ebp], eax
	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -304+[ebp], eax
	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -308+[ebp], eax
	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -312+[ebp], eax
	mov	ecx, OFFSET FLAT:_varNull
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -316+[ebp], eax
	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -320+[ebp], eax
	mov	ecx, OFFSET FLAT:_varNull
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -324+[ebp], eax
	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -328+[ebp], eax
	push	2
	mov	dx, WORD PTR _nOutputFormat$[ebp]
	push	edx
	lea	ecx, DWORD PTR $T77430[ebp]
	call	??0COleVariant@@QAE@FG@Z		; COleVariant::COleVariant
	mov	DWORD PTR -332+[ebp], eax
	mov	eax, DWORD PTR -332+[ebp]
	mov	DWORD PTR -336+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 11		; 0000000bH
	mov	ecx, DWORD PTR -336+[ebp]
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -340+[ebp], eax
	mov	ecx, DWORD PTR _sOutputFilename$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -344+[ebp], eax
	mov	ecx, DWORD PTR -344+[ebp]
	push	ecx
	lea	ecx, DWORD PTR $T77431[ebp]
	call	??0COleVariant@@QAE@PBG@Z		; COleVariant::COleVariant
	mov	DWORD PTR -348+[ebp], eax
	mov	edx, DWORD PTR -348+[ebp]
	mov	DWORD PTR -352+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+12], 12		; 0000000cH
	mov	ecx, DWORD PTR -352+[ebp]
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	mov	DWORD PTR -356+[ebp], eax
	mov	eax, DWORD PTR -268+[ebp]
	push	eax
	mov	ecx, DWORD PTR -272+[ebp]
	push	ecx
	mov	edx, DWORD PTR -276+[ebp]
	push	edx
	mov	eax, DWORD PTR -280+[ebp]
	push	eax
	mov	ecx, DWORD PTR -292+[ebp]
	push	ecx
	mov	edx, DWORD PTR -296+[ebp]
	push	edx
	mov	eax, DWORD PTR -300+[ebp]
	push	eax
	mov	ecx, DWORD PTR -304+[ebp]
	push	ecx
	mov	edx, DWORD PTR -308+[ebp]
	push	edx
	mov	eax, DWORD PTR -312+[ebp]
	push	eax
	mov	ecx, DWORD PTR -316+[ebp]
	push	ecx
	mov	edx, DWORD PTR -320+[ebp]
	push	edx
	mov	eax, DWORD PTR -324+[ebp]
	push	eax
	mov	ecx, DWORD PTR -328+[ebp]
	push	ecx
	mov	edx, DWORD PTR -340+[ebp]
	push	edx
	mov	eax, DWORD PTR -356+[ebp]
	push	eax
	lea	ecx, DWORD PTR _doc$[ebp]
	call	?SaveAs@_Document@WordAPI@@QAEXPAUtagVARIANT@@000000000000000@Z ; WordAPI::_Document::SaveAs
	mov	BYTE PTR __$EHRec$[ebp+12], 11		; 0000000bH
	lea	ecx, DWORD PTR $T77431[ebp]
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
	mov	BYTE PTR __$EHRec$[ebp+12], 10		; 0000000aH
	lea	ecx, DWORD PTR $T77430[ebp]
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
	mov	BYTE PTR __$EHRec$[ebp+12], 9
	lea	ecx, DWORD PTR $T77429[ebp]
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant

; 271  : 	catch (...)

	jmp	SHORT $L77146
$L77439:

; 272  : 	{
; 273  : 		bSuccess = FALSE;

	mov	DWORD PTR _bSuccess$[ebp], 0

; 274  : 	}

	mov	eax, $L77146
	ret	0
$L77146:

; 275  : 
; 276  : 	// cleanup
; 277  : 	doc.Close(varZero,   // SaveChanges

	mov	DWORD PTR __$EHRec$[ebp+12], 8

; 278  : 			  varZero,   // OriginalFormat
; 279  : 			  varFalse); // RouteDocument

	mov	ecx, OFFSET FLAT:_varFalse
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	push	eax
	mov	ecx, OFFSET FLAT:_varZero
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	push	eax
	mov	ecx, OFFSET FLAT:_varZero
	call	??BCOleVariant@@QAEPAUtagVARIANT@@XZ	; COleVariant::operator tagVARIANT *
	push	eax
	lea	ecx, DWORD PTR _doc$[ebp]
	call	?Close@_Document@WordAPI@@QAEXPAUtagVARIANT@@00@Z ; WordAPI::_Document::Close

; 280  : 
; 281  : 	return bSuccess;

	mov	ecx, DWORD PTR _bSuccess$[ebp]
	mov	DWORD PTR $T77432[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+12], 0
	lea	ecx, DWORD PTR _doc$[ebp]
	call	??1_Document@WordAPI@@QAE@XZ		; WordAPI::_Document::~_Document
	mov	DWORD PTR __$EHRec$[ebp+12], -1
	lea	ecx, DWORD PTR _docs$[ebp]
	call	??1Documents@WordAPI@@QAE@XZ		; WordAPI::Documents::~Documents
	mov	eax, DWORD PTR $T77432[ebp]
$L77121:

; 282  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp+4]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 356				; 00000164H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	20					; 00000014H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77440:
	lea	ecx, DWORD PTR _docs$[ebp]
	call	??1Documents@WordAPI@@QAE@XZ		; WordAPI::Documents::~Documents
	ret	0
$L77441:
	lea	ecx, DWORD PTR $T77425[ebp]
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
	ret	0
$L77442:
	lea	ecx, DWORD PTR $T77426[ebp]
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
	ret	0
$L77443:
	lea	ecx, DWORD PTR $T77427[ebp]
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
	ret	0
$L77444:
	lea	ecx, DWORD PTR _doc$[ebp]
	call	??1_Document@WordAPI@@QAE@XZ		; WordAPI::_Document::~_Document
	ret	0
$L77445:
	lea	ecx, DWORD PTR $T77429[ebp]
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
	ret	0
$L77446:
	lea	ecx, DWORD PTR $T77430[ebp]
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
	ret	0
$L77447:
	lea	ecx, DWORD PTR $T77431[ebp]
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
	ret	0
$L77452:
	mov	eax, OFFSET FLAT:$T77451
	jmp	___CxxFrameHandler
text$x	ENDS
?ConvertFile@CMSWordHelper@@QAEHABVCString@@W4WdOpenFormat@WordAPI@@0W4WdSaveFormat@4@W4MsoEncoding@4@@Z ENDP ; CMSWordHelper::ConvertFile
;	COMDAT ??0Documents@WordAPI@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0Documents@WordAPI@@QAE@XZ PROC NEAR			; WordAPI::Documents::Documents, COMDAT

; 421  : 	Documents() {}		// Calls COleDispatchDriver default constructor

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0COleDispatchDriver@@QAE@XZ		; COleDispatchDriver::COleDispatchDriver
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0Documents@WordAPI@@QAE@XZ ENDP			; WordAPI::Documents::Documents
_TEXT	ENDS
EXTRN	??0COleDispatchDriver@@QAE@PAUIDispatch@@H@Z:NEAR ; COleDispatchDriver::COleDispatchDriver
;	COMDAT ??0_Document@WordAPI@@QAE@PAUIDispatch@@@Z
_TEXT	SEGMENT
_this$ = -4
_pDispatch$ = 8
??0_Document@WordAPI@@QAE@PAUIDispatch@@@Z PROC NEAR	; WordAPI::_Document::_Document, COMDAT

; 454  : 	_Document(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	1
	mov	eax, DWORD PTR _pDispatch$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0COleDispatchDriver@@QAE@PAUIDispatch@@H@Z ; COleDispatchDriver::COleDispatchDriver
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0_Document@WordAPI@@QAE@PAUIDispatch@@@Z ENDP		; WordAPI::_Document::_Document
_TEXT	ENDS
;	COMDAT ??1Documents@WordAPI@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1Documents@WordAPI@@QAE@XZ PROC NEAR			; WordAPI::Documents::~Documents, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1COleDispatchDriver@@QAE@XZ		; COleDispatchDriver::~COleDispatchDriver
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1Documents@WordAPI@@QAE@XZ ENDP			; WordAPI::Documents::~Documents
_TEXT	ENDS
;	COMDAT ??1_Document@WordAPI@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1_Document@WordAPI@@QAE@XZ PROC NEAR			; WordAPI::_Document::~_Document, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1COleDispatchDriver@@QAE@XZ		; COleDispatchDriver::~COleDispatchDriver
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1_Document@WordAPI@@QAE@XZ ENDP			; WordAPI::_Document::~_Document
_TEXT	ENDS
END

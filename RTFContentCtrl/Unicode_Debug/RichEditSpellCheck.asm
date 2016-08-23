	TITLE	D:\_CODE\Shared\RichEditSpellCheck.cpp
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
;	COMDAT _$E269
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
;	COMDAT ??0CRichEditSpellCheck@@QAE@AAVCRichEditBaseCtrl@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCRichEditSpellCheck@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0ISpellCheck@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CRichEditSpellCheck@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFirstWord@CRichEditSpellCheck@@UBEPBGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNextWord@CRichEditSpellCheck@@UBEPBGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetReferenceTextBeingChecked@CRichEditSpellCheck@@UBEPBGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetCurrentWord@CRichEditSpellCheck@@UBEPBGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetWord@CRichEditSpellCheck@@IBEHABU_charrange@@AAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SelectCurrentWord@CRichEditSpellCheck@@UAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReplaceCurrentWord@CRichEditSpellCheck@@UAEXPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ClearSelection@CRichEditSpellCheck@@UAEXXZ
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
;	COMDAT ??_7ISpellCheck@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CRichEditSpellCheck@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS, CRT$XCA, CRT$XCU, CRT$XCL, CRT$XCC, CRT$XCZ
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\Shared\RichEditSpellCheck.cpp', 00H
_DATA	ENDS
CRT$XCU	SEGMENT
_$S273	DD	FLAT:_$E272
CRT$XCU	ENDS
_DATA	SEGMENT
	ORG $+1
$SG73528 DB	' ', 09H, 0dH, 0aH, '.,:;-/?<>|~!@#$%^&*(){}+=', 0bH, 00H
_DATA	ENDS
EXTRN	__chkesp:NEAR
;	COMDAT _$E272
_TEXT	SEGMENT
_$E272	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E269
	call	_$E271
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E272	ENDP
_TEXT	ENDS
EXTRN	??0CString@@QAE@PBD@Z:NEAR			; CString::CString
_BSS	SEGMENT
_DELIMS	DD	01H DUP (?)
_BSS	ENDS
;	COMDAT _$E269
_TEXT	SEGMENT
_$E269	PROC NEAR					; COMDAT

; 19   : const CString DELIMS(" \t\r\n.,:;-/?<>|~!@#$%^&*(){}+=");

	push	ebp
	mov	ebp, esp
	push	OFFSET FLAT:$SG73528
	mov	ecx, OFFSET FLAT:_DELIMS
	call	??0CString@@QAE@PBD@Z			; CString::CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E269	ENDP
_TEXT	ENDS
EXTRN	_atexit:NEAR
;	COMDAT _$E271
_TEXT	SEGMENT
_$E271	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	push	OFFSET FLAT:_$E270
	call	_atexit
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E271	ENDP
_TEXT	ENDS
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT _$E270
_TEXT	SEGMENT
_$E270	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_DELIMS
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E270	ENDP
_TEXT	ENDS
PUBLIC	??0ISpellCheck@@QAE@XZ				; ISpellCheck::ISpellCheck
PUBLIC	??0CRichEditSpellCheck@@QAE@AAVCRichEditBaseCtrl@@@Z ; CRichEditSpellCheck::CRichEditSpellCheck
PUBLIC	?GetFirstWord@CRichEditSpellCheck@@UBEPBGXZ	; CRichEditSpellCheck::GetFirstWord
PUBLIC	?GetNextWord@CRichEditSpellCheck@@UBEPBGXZ	; CRichEditSpellCheck::GetNextWord
PUBLIC	?GetCurrentWord@CRichEditSpellCheck@@UBEPBGXZ	; CRichEditSpellCheck::GetCurrentWord
PUBLIC	?SelectCurrentWord@CRichEditSpellCheck@@UAEXXZ	; CRichEditSpellCheck::SelectCurrentWord
PUBLIC	?ReplaceCurrentWord@CRichEditSpellCheck@@UAEXPBG@Z ; CRichEditSpellCheck::ReplaceCurrentWord
PUBLIC	?ClearSelection@CRichEditSpellCheck@@UAEXXZ	; CRichEditSpellCheck::ClearSelection
PUBLIC	?GetReferenceTextBeingChecked@CRichEditSpellCheck@@UBEPBGXZ ; CRichEditSpellCheck::GetReferenceTextBeingChecked
PUBLIC	??_7CRichEditSpellCheck@@6B@			; CRichEditSpellCheck::`vftable'
PUBLIC	??_GCRichEditSpellCheck@@UAEPAXI@Z		; CRichEditSpellCheck::`scalar deleting destructor'
PUBLIC	??_ECRichEditSpellCheck@@UAEPAXI@Z		; CRichEditSpellCheck::`vector deleting destructor'
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
;	COMDAT ??_7CRichEditSpellCheck@@6B@
; File D:\_CODE\Shared\RichEditSpellCheck.cpp
CONST	SEGMENT
??_7CRichEditSpellCheck@@6B@ DD FLAT:?GetFirstWord@CRichEditSpellCheck@@UBEPBGXZ ; CRichEditSpellCheck::`vftable'
	DD	FLAT:?GetNextWord@CRichEditSpellCheck@@UBEPBGXZ
	DD	FLAT:?GetCurrentWord@CRichEditSpellCheck@@UBEPBGXZ
	DD	FLAT:?SelectCurrentWord@CRichEditSpellCheck@@UAEXXZ
	DD	FLAT:?ReplaceCurrentWord@CRichEditSpellCheck@@UAEXPBG@Z
	DD	FLAT:?ClearSelection@CRichEditSpellCheck@@UAEXXZ
	DD	FLAT:?GetReferenceTextBeingChecked@CRichEditSpellCheck@@UBEPBGXZ
	DD	FLAT:??_ECRichEditSpellCheck@@UAEPAXI@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73770	DD	019930520H
	DD	02H
	DD	FLAT:$T73773
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73773	DD	0ffffffffH
	DD	FLAT:$L73764
	DD	00H
	DD	FLAT:$L73765
xdata$x	ENDS
;	COMDAT ??0CRichEditSpellCheck@@QAE@AAVCRichEditBaseCtrl@@@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_re$ = 8
_this$ = -16
??0CRichEditSpellCheck@@QAE@AAVCRichEditBaseCtrl@@@Z PROC NEAR ; CRichEditSpellCheck::CRichEditSpellCheck, COMDAT

; 22   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73771
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0ISpellCheck@@QAE@XZ			; ISpellCheck::ISpellCheck
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _re$[ebp]
	mov	DWORD PTR [eax+4], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 20					; 00000014H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 24					; 00000018H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7CRichEditSpellCheck@@6B@ ; CRichEditSpellCheck::`vftable'

; 23   : 	m_crCurrentWord.cpMin = m_crCurrentWord.cpMax = 0;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+12], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+8], 0

; 24   : }

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
$L73764:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73765:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 20					; 00000014H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73771:
	mov	eax, OFFSET FLAT:$T73770
	jmp	___CxxFrameHandler
text$x	ENDS
??0CRichEditSpellCheck@@QAE@AAVCRichEditBaseCtrl@@@Z ENDP ; CRichEditSpellCheck::CRichEditSpellCheck
PUBLIC	??1CRichEditSpellCheck@@UAE@XZ			; CRichEditSpellCheck::~CRichEditSpellCheck
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCRichEditSpellCheck@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCRichEditSpellCheck@@UAEPAXI@Z PROC NEAR		; CRichEditSpellCheck::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CRichEditSpellCheck@@UAE@XZ		; CRichEditSpellCheck::~CRichEditSpellCheck
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73548
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73548:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCRichEditSpellCheck@@UAEPAXI@Z ENDP			; CRichEditSpellCheck::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??_7ISpellCheck@@6B@				; ISpellCheck::`vftable'
EXTRN	__purecall:NEAR
;	COMDAT ??_7ISpellCheck@@6B@
CONST	SEGMENT
??_7ISpellCheck@@6B@ DD FLAT:__purecall			; ISpellCheck::`vftable'
	DD	FLAT:__purecall
	DD	FLAT:__purecall
	DD	FLAT:__purecall
	DD	FLAT:__purecall
	DD	FLAT:__purecall
	DD	FLAT:__purecall
CONST	ENDS
;	COMDAT ??0ISpellCheck@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0ISpellCheck@@QAE@XZ PROC NEAR			; ISpellCheck::ISpellCheck, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7ISpellCheck@@6B@ ; ISpellCheck::`vftable'
	mov	eax, DWORD PTR _this$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	0
??0ISpellCheck@@QAE@XZ ENDP				; ISpellCheck::ISpellCheck
_TEXT	ENDS
;	COMDAT xdata$x
; File D:\_CODE\Shared\RichEditSpellCheck.cpp
xdata$x	SEGMENT
$T73787	DD	019930520H
	DD	02H
	DD	FLAT:$T73789
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73789	DD	0ffffffffH
	DD	FLAT:$L73783
	DD	00H
	DD	FLAT:$L73784
xdata$x	ENDS
;	COMDAT ??1CRichEditSpellCheck@@UAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??1CRichEditSpellCheck@@UAE@XZ PROC NEAR		; CRichEditSpellCheck::~CRichEditSpellCheck, COMDAT

; 27   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73788
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CRichEditSpellCheck@@6B@ ; CRichEditSpellCheck::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 28   : }

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 24					; 00000018H
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 20					; 00000014H
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	??1CString@@QAE@XZ			; CString::~CString
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
$L73783:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73784:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 20					; 00000014H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73788:
	mov	eax, OFFSET FLAT:$T73787
	jmp	___CxxFrameHandler
text$x	ENDS
??1CRichEditSpellCheck@@UAE@XZ ENDP			; CRichEditSpellCheck::~CRichEditSpellCheck
EXTRN	?Empty@CString@@QAEXXZ:NEAR			; CString::Empty
;	COMDAT ?GetFirstWord@CRichEditSpellCheck@@UBEPBGXZ
_TEXT	SEGMENT
_this$ = -4
?GetFirstWord@CRichEditSpellCheck@@UBEPBGXZ PROC NEAR	; CRichEditSpellCheck::GetFirstWord, COMDAT

; 31   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 32   : 	m_crCurrentWord.cpMin = m_crCurrentWord.cpMax = -1;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+12], -1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+8], -1

; 33   : 	m_sLeftover.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 24					; 00000018H
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 34   : 	m_sCurrentWord.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 35   : 
; 36   : 	// make sure text is current
; 37   : 	m_sText.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 20					; 00000014H
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 38   : 	GetReferenceTextBeingChecked();

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [eax+24]
	cmp	esi, esp
	call	__chkesp

; 39   : 
; 40   : 	// start
; 41   : 	return GetNextWord();

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp

; 42   : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetFirstWord@CRichEditSpellCheck@@UBEPBGXZ ENDP	; CRichEditSpellCheck::GetFirstWord
_TEXT	ENDS
PUBLIC	?GetWord@CRichEditSpellCheck@@IBEHABU_charrange@@AAVCString@@@Z ; CRichEditSpellCheck::GetWord
EXTRN	?SendMessageW@CWnd@@QAEJIIJ@Z:NEAR		; CWnd::SendMessageW
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
EXTRN	??ACString@@QBEGH@Z:NEAR			; CString::operator[]
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
EXTRN	??4CString@@QAEABV0@PBD@Z:NEAR			; CString::operator=
EXTRN	?Mid@CString@@QBE?AV1@HH@Z:NEAR			; CString::Mid
EXTRN	?Mid@CString@@QBE?AV1@H@Z:NEAR			; CString::Mid
EXTRN	?Left@CString@@QBE?AV1@H@Z:NEAR			; CString::Left
EXTRN	?TrimRight@CString@@QAEXPBG@Z:NEAR		; CString::TrimRight
EXTRN	?TrimLeft@CString@@QAEXPBG@Z:NEAR		; CString::TrimLeft
EXTRN	?Find@CString@@QBEHG@Z:NEAR			; CString::Find
EXTRN	?FindOneOf@CString@@QBEHPBG@Z:NEAR		; CString::FindOneOf
;	COMDAT data
; File D:\_CODE\Shared\RichEditSpellCheck.cpp
data	SEGMENT
$SG73586 DB	' ', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73805	DD	019930520H
	DD	04H
	DD	FLAT:$T73807
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73807	DD	0ffffffffH
	DD	FLAT:$L73800
	DD	0ffffffffH
	DD	FLAT:$L73801
	DD	0ffffffffH
	DD	FLAT:$L73802
	DD	0ffffffffH
	DD	FLAT:$L73803
xdata$x	ENDS
;	COMDAT ?GetNextWord@CRichEditSpellCheck@@UBEPBGXZ
_TEXT	SEGMENT
$T73796 = -48
$T73797 = -52
$T73798 = -56
$T73799 = -60
__$EHRec$ = -12
_this$ = -64
_cr$ = -24
_nLength$ = -16
_nStart$73563 = -32
_c$73567 = -36
_nEnd$73569 = -28
_c$73573 = -40
_nFind$73581 = -44
?GetNextWord@CRichEditSpellCheck@@UBEPBGXZ PROC NEAR	; CRichEditSpellCheck::GetNextWord, COMDAT

; 45   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73806
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 84					; 00000054H
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-96]
	mov	ecx, 21					; 00000015H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 46   : 	CHARRANGE cr = { 0 };

	mov	DWORD PTR _cr$[ebp], 0
	xor	eax, eax
	mov	DWORD PTR _cr$[ebp+4], eax

; 47   : 	int nLength = 0;

	mov	DWORD PTR _nLength$[ebp], 0

; 48   : 
; 49   : 	m_sCurrentWord.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 50   : 
; 51   : 	// have we got anything left from cleaning up
; 52   : 	// after richedit's mishandling of line endings
; 53   : 	if (!m_sLeftover.IsEmpty())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 24					; 00000018H
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	$L73562

; 55   : 		// move to start of word
; 56   : 		int nStart = 0;

	mov	DWORD PTR _nStart$73563[ebp], 0
$L73565:

; 57   : 		
; 58   : 		while (nStart < m_sLeftover.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 24					; 00000018H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	cmp	DWORD PTR _nStart$73563[ebp], eax
	jge	SHORT $L73566

; 60   : 			TCHAR c = m_sLeftover[nStart];

	mov	ecx, DWORD PTR _nStart$73563[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 24					; 00000018H
	call	??ACString@@QBEGH@Z			; CString::operator[]
	mov	WORD PTR _c$73567[ebp], ax

; 61   : 
; 62   : 			if (DELIMS.Find(c) == -1)

	mov	dx, WORD PTR _c$73567[ebp]
	push	edx
	mov	ecx, OFFSET FLAT:_DELIMS
	call	?Find@CString@@QBEHG@Z			; CString::Find
	cmp	eax, -1
	jne	SHORT $L73568

; 63   : 				break; 

	jmp	SHORT $L73566
$L73568:

; 64   : 
; 65   : 			nStart++;

	mov	eax, DWORD PTR _nStart$73563[ebp]
	add	eax, 1
	mov	DWORD PTR _nStart$73563[ebp], eax

; 66   : 		}

	jmp	SHORT $L73565
$L73566:

; 67   : 
; 68   : 		// get end of next word
; 69   : 		int nEnd = (nStart + 1);

	mov	ecx, DWORD PTR _nStart$73563[ebp]
	add	ecx, 1
	mov	DWORD PTR _nEnd$73569[ebp], ecx
$L73571:

; 70   : 
; 71   : 		while (nEnd < m_sLeftover.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 24					; 00000018H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	cmp	DWORD PTR _nEnd$73569[ebp], eax
	jge	SHORT $L73572

; 73   : 			TCHAR c = m_sLeftover[nEnd];

	mov	edx, DWORD PTR _nEnd$73569[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 24					; 00000018H
	call	??ACString@@QBEGH@Z			; CString::operator[]
	mov	WORD PTR _c$73573[ebp], ax

; 74   : 
; 75   : 			if (DELIMS.Find(c) != -1)

	mov	ax, WORD PTR _c$73573[ebp]
	push	eax
	mov	ecx, OFFSET FLAT:_DELIMS
	call	?Find@CString@@QBEHG@Z			; CString::Find
	cmp	eax, -1
	je	SHORT $L73574

; 76   : 				break; 

	jmp	SHORT $L73572
$L73574:

; 77   : 			
; 78   : 			nEnd++;

	mov	ecx, DWORD PTR _nEnd$73569[ebp]
	add	ecx, 1
	mov	DWORD PTR _nEnd$73569[ebp], ecx

; 79   : 		}

	jmp	SHORT $L73571
$L73572:

; 80   : 
; 81   : 		if (nStart < m_sLeftover.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 24					; 00000018H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	cmp	DWORD PTR _nStart$73563[ebp], eax
	jge	$L73575

; 83   : 			m_sCurrentWord = m_sLeftover.Mid(nStart, nEnd - nStart);

	mov	edx, DWORD PTR _nEnd$73569[ebp]
	sub	edx, DWORD PTR _nStart$73563[ebp]
	push	edx
	mov	eax, DWORD PTR _nStart$73563[ebp]
	push	eax
	lea	ecx, DWORD PTR $T73796[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 24					; 00000018H
	call	?Mid@CString@@QBE?AV1@HH@Z		; CString::Mid
	mov	DWORD PTR -68+[ebp], eax
	mov	edx, DWORD PTR -68+[ebp]
	mov	DWORD PTR -72+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR -72+[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T73796[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 84   : 			m_sLeftover = m_sLeftover.Mid(nEnd);

	mov	ecx, DWORD PTR _nEnd$73569[ebp]
	push	ecx
	lea	edx, DWORD PTR $T73797[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 24					; 00000018H
	call	?Mid@CString@@QBE?AV1@H@Z		; CString::Mid
	mov	DWORD PTR -76+[ebp], eax
	mov	eax, DWORD PTR -76+[ebp]
	mov	DWORD PTR -80+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -80+[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 24					; 00000018H
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T73797[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 85   : 
; 86   : 			cr.cpMin = (m_crCurrentWord.cpMax + nStart);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	add	eax, DWORD PTR _nStart$73563[ebp]
	mov	DWORD PTR _cr$[ebp], eax

; 87   : 			cr.cpMax = (m_crCurrentWord.cpMax + nEnd);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	add	edx, DWORD PTR _nEnd$73569[ebp]
	mov	DWORD PTR _cr$[ebp+4], edx

; 89   : 		else

	jmp	SHORT $L73578
$L73575:

; 91   : 			m_sLeftover.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 24					; 00000018H
	call	?Empty@CString@@QAEXXZ			; CString::Empty
$L73578:

; 93   : 
; 94   : 		nLength = m_sCurrentWord.GetLength();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR _nLength$[ebp], eax
$L73562:

; 96   : 
; 97   : 	if (nLength == 0)

	cmp	DWORD PTR _nLength$[ebp], 0
	jne	SHORT $L73579

; 99   : 		// default word detection
; 100  : 		cr.cpMin = m_re.SendMessage(EM_FINDWORDBREAK, WB_RIGHT, m_crCurrentWord.cpMax);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	push	ecx
	push	1
	push	1100					; 0000044cH
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+4]
	call	?SendMessageW@CWnd@@QAEJIIJ@Z		; CWnd::SendMessageW
	mov	DWORD PTR _cr$[ebp], eax

; 101  : 		cr.cpMax = m_re.SendMessage(EM_FINDWORDBREAK, WB_RIGHT, cr.cpMin + 1);

	mov	eax, DWORD PTR _cr$[ebp]
	add	eax, 1
	push	eax
	push	1
	push	1100					; 0000044cH
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+4]
	call	?SendMessageW@CWnd@@QAEJIIJ@Z		; CWnd::SendMessageW
	mov	DWORD PTR _cr$[ebp+4], eax

; 102  : 		
; 103  : 		nLength = GetWord(cr, m_sCurrentWord);

	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 16					; 00000010H
	push	edx
	lea	eax, DWORD PTR _cr$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetWord@CRichEditSpellCheck@@IBEHABU_charrange@@AAVCString@@@Z ; CRichEditSpellCheck::GetWord
	mov	DWORD PTR _nLength$[ebp], eax
$L73579:

; 105  : 
; 106  : 	if (nLength)

	cmp	DWORD PTR _nLength$[ebp], 0
	je	$L73585

; 108  : 		m_crCurrentWord = cr;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _cr$[ebp]
	mov	DWORD PTR [ecx+8], edx
	mov	eax, DWORD PTR _cr$[ebp+4]
	mov	DWORD PTR [ecx+12], eax

; 109  : 
; 110  : 		// if there's any leading delimiters then trim them off
; 111  : 		m_sCurrentWord.TrimLeft(DELIMS);

	mov	ecx, OFFSET FLAT:_DELIMS
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	?TrimLeft@CString@@QAEXPBG@Z		; CString::TrimLeft

; 112  : 
; 113  : 		// and update char range
; 114  : 		m_crCurrentWord.cpMin += (nLength - m_sCurrentWord.GetLength());

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	ecx, DWORD PTR _nLength$[ebp]
	sub	ecx, eax
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	add	eax, ecx
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+8], eax

; 115  : 		nLength = m_sCurrentWord.GetLength();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR _nLength$[ebp], eax

; 116  : 
; 117  : 		// richedit sometimes mis-handles trailing line endings
; 118  : 		// leaving them in and including the next word, so we
; 119  : 		// try to detect it here
; 120  : 		int nFind = m_sCurrentWord.FindOneOf(DELIMS);

	mov	ecx, OFFSET FLAT:_DELIMS
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	?FindOneOf@CString@@QBEHPBG@Z		; CString::FindOneOf
	mov	DWORD PTR _nFind$73581[ebp], eax

; 121  : 		
; 122  : 		if (nFind != -1)

	cmp	DWORD PTR _nFind$73581[ebp], -1
	je	$L73582

; 124  : 			// store what's left for further processing next time round
; 125  : 			m_sLeftover = m_sCurrentWord.Mid(nFind);

	mov	edx, DWORD PTR _nFind$73581[ebp]
	push	edx
	lea	eax, DWORD PTR $T73798[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	?Mid@CString@@QBE?AV1@H@Z		; CString::Mid
	mov	DWORD PTR -84+[ebp], eax
	mov	ecx, DWORD PTR -84+[ebp]
	mov	DWORD PTR -88+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 2
	mov	edx, DWORD PTR -88+[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 24					; 00000018H
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T73798[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 126  : 
; 127  : 			m_crCurrentWord.cpMax = (m_crCurrentWord.cpMin + nFind);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	add	ecx, DWORD PTR _nFind$73581[ebp]
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12], ecx

; 128  : 			m_sCurrentWord = m_sCurrentWord.Left(nFind);

	mov	eax, DWORD PTR _nFind$73581[ebp]
	push	eax
	lea	ecx, DWORD PTR $T73799[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	?Left@CString@@QBE?AV1@H@Z		; CString::Left
	mov	DWORD PTR -92+[ebp], eax
	mov	edx, DWORD PTR -92+[ebp]
	mov	DWORD PTR -96+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 3
	mov	eax, DWORD PTR -96+[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T73799[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 129  : 			nLength = m_sCurrentWord.GetLength();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR _nLength$[ebp], eax
$L73582:

; 131  : 
; 132  : 		// if there's any trailing delimiters then trim them off
; 133  : 		m_sCurrentWord.TrimRight(DELIMS);

	mov	ecx, OFFSET FLAT:_DELIMS
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	?TrimRight@CString@@QAEXPBG@Z		; CString::TrimRight

; 134  : 
; 135  : 		// and update char range
; 136  : 		m_crCurrentWord.cpMax -= (nLength - m_sCurrentWord.GetLength());

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	ecx, DWORD PTR _nLength$[ebp]
	sub	ecx, eax
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	sub	eax, ecx
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+12], eax

; 137  : 		nLength = m_sCurrentWord.GetLength();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR _nLength$[ebp], eax

; 138  : 
; 139  : 		// if there was some text but it was all whitespace, return
; 140  : 		// a non-empty string so that searching is not terminated
; 141  : 		// and move the selection to the end of the whitespace
; 142  : 		if (m_sCurrentWord.IsEmpty())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L73585

; 144  : 			m_crCurrentWord.cpMin = m_crCurrentWord.cpMax = cr.cpMax;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _cr$[ebp+4]
	mov	DWORD PTR [edx+12], eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _cr$[ebp+4]
	mov	DWORD PTR [ecx+8], edx

; 145  : 			m_sCurrentWord = " ";

	push	OFFSET FLAT:$SG73586
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	??4CString@@QAEABV0@PBD@Z		; CString::operator=
$L73585:

; 152  : 
; 153  : 	return m_sCurrentWord;

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *

; 154  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 96					; 00000060H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73800:
	lea	ecx, DWORD PTR $T73796[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73801:
	lea	ecx, DWORD PTR $T73797[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73802:
	lea	ecx, DWORD PTR $T73798[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73803:
	lea	ecx, DWORD PTR $T73799[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73806:
	mov	eax, OFFSET FLAT:$T73805
	jmp	___CxxFrameHandler
text$x	ENDS
?GetNextWord@CRichEditSpellCheck@@UBEPBGXZ ENDP		; CRichEditSpellCheck::GetNextWord
EXTRN	?GetWindowTextW@CWnd@@QBEXAAVCString@@@Z:NEAR	; CWnd::GetWindowTextW
;	COMDAT ?GetReferenceTextBeingChecked@CRichEditSpellCheck@@UBEPBGXZ
_TEXT	SEGMENT
_this$ = -4
?GetReferenceTextBeingChecked@CRichEditSpellCheck@@UBEPBGXZ PROC NEAR ; CRichEditSpellCheck::GetReferenceTextBeingChecked, COMDAT

; 157  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 158  : 	if (m_sText.IsEmpty())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 20					; 00000014H
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L73591

; 159  : 		m_re.GetWindowText(m_sText);

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 20					; 00000014H
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+4]
	call	?GetWindowTextW@CWnd@@QBEXAAVCString@@@Z ; CWnd::GetWindowTextW
$L73591:

; 160  : 
; 161  : 	return m_sText;

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 20					; 00000014H
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *

; 162  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetReferenceTextBeingChecked@CRichEditSpellCheck@@UBEPBGXZ ENDP ; CRichEditSpellCheck::GetReferenceTextBeingChecked
_TEXT	ENDS
;	COMDAT ?GetCurrentWord@CRichEditSpellCheck@@UBEPBGXZ
_TEXT	SEGMENT
_this$ = -4
?GetCurrentWord@CRichEditSpellCheck@@UBEPBGXZ PROC NEAR	; CRichEditSpellCheck::GetCurrentWord, COMDAT

; 165  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 166  : 	return m_sCurrentWord;

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *

; 167  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetCurrentWord@CRichEditSpellCheck@@UBEPBGXZ ENDP	; CRichEditSpellCheck::GetCurrentWord
_TEXT	ENDS
EXTRN	?GetTextRange@CRichEditBaseCtrl@@QAE?AVCString@@ABU_charrange@@@Z:NEAR ; CRichEditBaseCtrl::GetTextRange
;	COMDAT xdata$x
; File D:\_CODE\Shared\RichEditSpellCheck.cpp
xdata$x	SEGMENT
$T73820	DD	019930520H
	DD	01H
	DD	FLAT:$T73822
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73822	DD	0ffffffffH
	DD	FLAT:$L73818
xdata$x	ENDS
;	COMDAT ?GetWord@CRichEditSpellCheck@@IBEHABU_charrange@@AAVCString@@@Z
_TEXT	SEGMENT
$T73817 = -16
__$EHRec$ = -12
_cr$ = 8
_sWord$ = 12
_this$ = -20
?GetWord@CRichEditSpellCheck@@IBEHABU_charrange@@AAVCString@@@Z PROC NEAR ; CRichEditSpellCheck::GetWord, COMDAT

; 170  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73821
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

; 171  : 	if (cr.cpMax > cr.cpMin)

	mov	eax, DWORD PTR _cr$[ebp]
	mov	ecx, DWORD PTR _cr$[ebp]
	mov	edx, DWORD PTR [eax+4]
	cmp	edx, DWORD PTR [ecx]
	jle	SHORT $L73600

; 172  : 		sWord = m_re.GetTextRange(cr);

	mov	eax, DWORD PTR _cr$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T73817[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+4]
	call	?GetTextRange@CRichEditBaseCtrl@@QAE?AVCString@@ABU_charrange@@@Z ; CRichEditBaseCtrl::GetTextRange
	mov	DWORD PTR -24+[ebp], eax
	mov	eax, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -28+[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sWord$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T73817[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 173  : 	else

	jmp	SHORT $L73602
$L73600:

; 174  : 		sWord.Empty();

	mov	ecx, DWORD PTR _sWord$[ebp]
	call	?Empty@CString@@QAEXXZ			; CString::Empty
$L73602:

; 175  : 
; 176  : 	// else
; 177  : 	return sWord.GetLength();

	mov	ecx, DWORD PTR _sWord$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength

; 178  : }

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
$L73818:
	lea	ecx, DWORD PTR $T73817[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73821:
	mov	eax, OFFSET FLAT:$T73820
	jmp	___CxxFrameHandler
text$x	ENDS
?GetWord@CRichEditSpellCheck@@IBEHABU_charrange@@AAVCString@@@Z ENDP ; CRichEditSpellCheck::GetWord
EXTRN	?GetCharPos@CRichEditCtrl@@QBE?AVCPoint@@J@Z:NEAR ; CRichEditCtrl::GetCharPos
EXTRN	?GetClientRect@CWnd@@QBEXPAUtagRECT@@@Z:NEAR	; CWnd::GetClientRect
EXTRN	?SetSel@CRichEditCtrl@@QAEXAAU_charrange@@@Z:NEAR ; CRichEditCtrl::SetSel
EXTRN	?LineScroll@CRichEditCtrl@@QAEXHH@Z:NEAR	; CRichEditCtrl::LineScroll
EXTRN	??0CRect@@QAE@XZ:NEAR				; CRect::CRect
EXTRN	??BCRect@@QAEPAUtagRECT@@XZ:NEAR		; CRect::operator tagRECT *
;	COMDAT ?SelectCurrentWord@CRichEditSpellCheck@@UAEXXZ
_TEXT	SEGMENT
$T73827 = -32
$T73828 = -40
_this$ = -44
_ptSel$ = -8
_rClient$ = -24
?SelectCurrentWord@CRichEditSpellCheck@@UAEXXZ PROC NEAR ; CRichEditSpellCheck::SelectCurrentWord, COMDAT

; 181  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 44					; 0000002cH
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-44]
	mov	ecx, 11					; 0000000bH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 182  : 	m_re.SetSel(m_crCurrentWord);

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 8
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+4]
	call	?SetSel@CRichEditCtrl@@QAEXAAU_charrange@@@Z ; CRichEditCtrl::SetSel

; 183  : 	
; 184  : 	// need to make sure line is visible
; 185  : 	CPoint ptSel = m_re.GetCharPos(m_crCurrentWord.cpMax);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	lea	ecx, DWORD PTR _ptSel$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+4]
	call	?GetCharPos@CRichEditCtrl@@QBE?AVCPoint@@J@Z ; CRichEditCtrl::GetCharPos

; 186  : 	
; 187  : 	CRect rClient;

	lea	ecx, DWORD PTR _rClient$[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 188  : 	m_re.GetClientRect(rClient);

	lea	ecx, DWORD PTR _rClient$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	call	?GetClientRect@CWnd@@QBEXPAUtagRECT@@@Z	; CWnd::GetClientRect

; 189  : 	
; 190  : 	if (ptSel.y >= rClient.bottom)

	mov	ecx, DWORD PTR _ptSel$[ebp+4]
	cmp	ecx, DWORD PTR _rClient$[ebp+12]
	jl	SHORT $L73609
$L73611:

; 192  : 		while (ptSel.y >= rClient.bottom)

	mov	edx, DWORD PTR _ptSel$[ebp+4]
	cmp	edx, DWORD PTR _rClient$[ebp+12]
	jl	SHORT $L73612

; 194  : 			m_re.LineScroll(1);

	push	0
	push	1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	call	?LineScroll@CRichEditCtrl@@QAEXHH@Z	; CRichEditCtrl::LineScroll

; 195  : 			ptSel = m_re.GetCharPos(m_crCurrentWord.cpMax);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	push	edx
	lea	eax, DWORD PTR $T73827[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+4]
	call	?GetCharPos@CRichEditCtrl@@QBE?AVCPoint@@J@Z ; CRichEditCtrl::GetCharPos
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR [eax+4]
	mov	DWORD PTR _ptSel$[ebp], edx
	mov	DWORD PTR _ptSel$[ebp+4], eax

; 196  : 		}

	jmp	SHORT $L73611
$L73612:

; 197  : 		
; 198  : 		// one more for good measure
; 199  : 		m_re.LineScroll(1);

	push	0
	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+4]
	call	?LineScroll@CRichEditCtrl@@QAEXHH@Z	; CRichEditCtrl::LineScroll

; 201  : 	else if (ptSel.y < rClient.top)

	jmp	SHORT $L73615
$L73609:
	mov	edx, DWORD PTR _ptSel$[ebp+4]
	cmp	edx, DWORD PTR _rClient$[ebp+4]
	jge	SHORT $L73615
$L73617:

; 203  : 		while (ptSel.y < rClient.top)

	mov	eax, DWORD PTR _ptSel$[ebp+4]
	cmp	eax, DWORD PTR _rClient$[ebp+4]
	jge	SHORT $L73618

; 205  : 			m_re.LineScroll(-1);

	push	0
	push	-1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+4]
	call	?LineScroll@CRichEditCtrl@@QAEXHH@Z	; CRichEditCtrl::LineScroll

; 206  : 			ptSel = m_re.GetCharPos(m_crCurrentWord.cpMax);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	lea	ecx, DWORD PTR $T73828[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+4]
	call	?GetCharPos@CRichEditCtrl@@QBE?AVCPoint@@J@Z ; CRichEditCtrl::GetCharPos
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR _ptSel$[ebp], ecx
	mov	DWORD PTR _ptSel$[ebp+4], edx

; 207  : 		}

	jmp	SHORT $L73617
$L73618:

; 208  : 		
; 209  : 		// one more for good measure
; 210  : 		m_re.LineScroll(-1);

	push	0
	push	-1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	call	?LineScroll@CRichEditCtrl@@QAEXHH@Z	; CRichEditCtrl::LineScroll
$L73615:

; 212  : }

	pop	edi
	add	esp, 44					; 0000002cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?SelectCurrentWord@CRichEditSpellCheck@@UAEXXZ ENDP	; CRichEditSpellCheck::SelectCurrentWord
_TEXT	ENDS
EXTRN	?ReplaceSel@CRichEditCtrl@@QAEXPBGH@Z:NEAR	; CRichEditCtrl::ReplaceSel
EXTRN	__imp__lstrlenW@4:NEAR
EXTRN	??9@YG_NABVCString@@PBG@Z:NEAR			; operator!=
;	COMDAT ?ReplaceCurrentWord@CRichEditSpellCheck@@UAEXPBG@Z
_TEXT	SEGMENT
_szWord$ = 8
_this$ = -4
?ReplaceCurrentWord@CRichEditSpellCheck@@UAEXPBG@Z PROC NEAR ; CRichEditSpellCheck::ReplaceCurrentWord, COMDAT

; 215  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 216  : 	if (m_sCurrentWord != szWord)

	mov	eax, DWORD PTR _szWord$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	push	ecx
	call	??9@YG_NABVCString@@PBG@Z		; operator!=
	and	eax, 255				; 000000ffH
	test	eax, eax
	je	SHORT $L73624

; 218  : 		m_re.SetSel(m_crCurrentWord);

	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 8
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	call	?SetSel@CRichEditCtrl@@QAEXAAU_charrange@@@Z ; CRichEditCtrl::SetSel

; 219  : 		m_re.ReplaceSel(szWord, TRUE);

	push	1
	mov	ecx, DWORD PTR _szWord$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+4]
	call	?ReplaceSel@CRichEditCtrl@@QAEXPBGH@Z	; CRichEditCtrl::ReplaceSel

; 220  : 
; 221  : 		// update cached text
; 222  : 		m_re.GetWindowText(m_sText);

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 20					; 00000014H
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+4]
	call	?GetWindowTextW@CWnd@@QBEXAAVCString@@@Z ; CWnd::GetWindowTextW

; 223  : 
; 224  : 		// update cached selection
; 225  : 		m_crCurrentWord.cpMax += (lstrlen(szWord) - m_sCurrentWord.GetLength());

	mov	esi, esp
	mov	edx, DWORD PTR _szWord$[ebp]
	push	edx
	call	DWORD PTR __imp__lstrlenW@4
	cmp	esi, esp
	call	__chkesp
	mov	esi, eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 16					; 00000010H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	sub	esi, eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	add	ecx, esi
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12], ecx
$L73624:

; 227  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ReplaceCurrentWord@CRichEditSpellCheck@@UAEXPBG@Z ENDP	; CRichEditSpellCheck::ReplaceCurrentWord
_TEXT	ENDS
;	COMDAT ?ClearSelection@CRichEditSpellCheck@@UAEXXZ
_TEXT	SEGMENT
_this$ = -4
?ClearSelection@CRichEditSpellCheck@@UAEXXZ PROC NEAR	; CRichEditSpellCheck::ClearSelection, COMDAT

; 230  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 231  : 	m_crCurrentWord.cpMin = m_crCurrentWord.cpMax;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	mov	DWORD PTR [eax+8], edx

; 232  : 	m_re.SetSel(m_crCurrentWord);

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 8
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+4]
	call	?SetSel@CRichEditCtrl@@QAEXAAU_charrange@@@Z ; CRichEditCtrl::SetSel

; 233  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?ClearSelection@CRichEditSpellCheck@@UAEXXZ ENDP	; CRichEditSpellCheck::ClearSelection
_TEXT	ENDS
END

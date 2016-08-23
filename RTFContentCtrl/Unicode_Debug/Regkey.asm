	TITLE	D:\_CODE\Shared\Regkey.cpp
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
text$AFX_COL1	SEGMENT PARA USE32 PUBLIC ''
text$AFX_COL1	ENDS
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
;	COMDAT ??0CRegKey@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCRegKey@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CRegKey@@QAE@AAV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CRegKey@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4CRegKey@@QAEAAV0@V0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?KeyExists@CRegKey@@SAHPAUHKEY__@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RecurseDeleteKey@CRegKey@@KAJPAUHKEY__@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DeleteKey@CRegKey@@SAJPAUHKEY__@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Close@CRegKey@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Write@CRegKey@@QAEJPBGK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetValueType@CRegKey@@QBEKPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Write@CRegKey@@QAEJPBG0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Write@CRegKey@@QAEJPBGPBEK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Read@CRegKey@@QBEJPBGAAK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Read@CRegKey@@QBEJPBGAAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Read@CRegKey@@QBEJPBGPAEAAK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSubkeyNames@CRegKey@@QBEHAAVCStringArray@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetValueNames@CRegKey@@QBEHAAVCStringArray@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HasValues@CRegKey@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAppRegPath@CRegKey@@SA?AVCString@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ExportToIni@CRegKey@@QBEHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CStdioFileEx@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ExportKeyToIni@CRegKey@@IBEHPBGAAVCStdioFile@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ExportValueToIni@CRegKey@@IBEHKAAVCStdioFile@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ImportFromIni@CRegKey@@QAEHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ImportSectionFromIni@CRegKey@@IAEHABVCString@@AAVCStdioFile@@AAV2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CopyKey@CRegKey@@SAHPAUHKEY__@@ABVCString@@01@Z
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
;	COMDAT ??_7CRegKey@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
_DATA	SEGMENT
__szAfxTempl DB	'afxtempl.h', 00H
_DATA	ENDS
PUBLIC	??0CRegKey@@QAE@XZ				; CRegKey::CRegKey
PUBLIC	??_7CRegKey@@6B@				; CRegKey::`vftable'
PUBLIC	??_GCRegKey@@UAEPAXI@Z				; CRegKey::`scalar deleting destructor'
PUBLIC	??_ECRegKey@@UAEPAXI@Z				; CRegKey::`vector deleting destructor'
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	?Empty@CString@@QAEXXZ:NEAR			; CString::Empty
EXTRN	__except_list:DWORD
EXTRN	__chkesp:NEAR
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT ??_7CRegKey@@6B@
; File D:\_CODE\Shared\Regkey.cpp
CONST	SEGMENT
??_7CRegKey@@6B@ DD FLAT:??_ECRegKey@@UAEPAXI@Z		; CRegKey::`vftable'
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74642	DD	019930520H
	DD	01H
	DD	FLAT:$T74646
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74646	DD	0ffffffffH
	DD	FLAT:$L74638
xdata$x	ENDS
;	COMDAT ??0CRegKey@@QAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??0CRegKey@@QAE@XZ PROC NEAR				; CRegKey::CRegKey, COMDAT

; 9    : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74643
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CRegKey@@6B@ ; CRegKey::`vftable'

; 10   : 	m_hKeyRoot = NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+8], 0

; 11   : 	m_hKey = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0

; 12   : 	m_sPath.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 13   : }

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
$L74638:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74643:
	mov	eax, OFFSET FLAT:$T74642
	jmp	___CxxFrameHandler
text$x	ENDS
??0CRegKey@@QAE@XZ ENDP					; CRegKey::CRegKey
PUBLIC	??1CRegKey@@UAE@XZ				; CRegKey::~CRegKey
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCRegKey@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCRegKey@@UAEPAXI@Z PROC NEAR			; CRegKey::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L74038
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L74038:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCRegKey@@UAEPAXI@Z ENDP				; CRegKey::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??0CRegKey@@QAE@AAV0@@Z				; CRegKey::CRegKey
PUBLIC	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z		; CRegKey::Open
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
;	COMDAT xdata$x
; File D:\_CODE\Shared\Regkey.cpp
xdata$x	SEGMENT
$T74658	DD	019930520H
	DD	03H
	DD	FLAT:$T74660
	DD	01H
	DD	FLAT:$T74661
	DD	2 DUP(00H)
	ORG $+4
$T74660	DD	0ffffffffH
	DD	FLAT:$L74656
	DD	00H
	DD	00H
	DD	00H
	DD	00H
$T74661	DD	01H
	DD	01H
	DD	02H
	DD	01H
	DD	FLAT:$T74662
	ORG $+4
$T74662	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L74655
xdata$x	ENDS
;	COMDAT ??0CRegKey@@QAE@AAV0@@Z
_TEXT	SEGMENT
_regKey$ = 8
_this$ = -20
__$EHRec$ = -16
??0CRegKey@@QAE@AAV0@@Z PROC NEAR			; CRegKey::CRegKey, COMDAT

; 16   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74659
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
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+12], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CRegKey@@6B@ ; CRegKey::`vftable'

; 17   : 	m_hKeyRoot = NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+8], 0

; 18   : 	m_hKey = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0

; 19   : 	m_sPath.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 20   : 
; 21   : 	try
; 22   : 	{

	mov	BYTE PTR __$EHRec$[ebp+12], 1

; 23   : 		Open(regKey.m_hKeyRoot, regKey.m_sPath);

	mov	ecx, DWORD PTR _regKey$[ebp]
	add	ecx, 12					; 0000000cH
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -24+[ebp], eax
	mov	eax, DWORD PTR -24+[ebp]
	push	eax
	mov	ecx, DWORD PTR _regKey$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open

; 25   : 	catch (...)

	jmp	SHORT $L74043
$L74655:

; 26   : 	{
; 27   : 	}

	mov	eax, $L74043
	ret	0
$L74043:

; 28   : }

	mov	DWORD PTR __$EHRec$[ebp+12], 0
	mov	DWORD PTR __$EHRec$[ebp+12], -1
	mov	eax, DWORD PTR _this$[ebp]
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
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74656:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74659:
	mov	eax, OFFSET FLAT:$T74658
	jmp	___CxxFrameHandler
text$x	ENDS
??0CRegKey@@QAE@AAV0@@Z ENDP				; CRegKey::CRegKey
PUBLIC	?Close@CRegKey@@QAEXXZ				; CRegKey::Close
;	COMDAT xdata$x
; File D:\_CODE\Shared\Regkey.cpp
xdata$x	SEGMENT
$T74669	DD	019930520H
	DD	01H
	DD	FLAT:$T74671
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74671	DD	0ffffffffH
	DD	FLAT:$L74667
xdata$x	ENDS
;	COMDAT ??1CRegKey@@UAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1CRegKey@@UAE@XZ PROC NEAR				; CRegKey::~CRegKey, COMDAT

; 31   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74670
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CRegKey@@6B@ ; CRegKey::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 32   : 	Close();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Close@CRegKey@@QAEXXZ			; CRegKey::Close

; 33   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
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
$L74667:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74670:
	mov	eax, OFFSET FLAT:$T74669
	jmp	___CxxFrameHandler
text$x	ENDS
??1CRegKey@@UAE@XZ ENDP					; CRegKey::~CRegKey
PUBLIC	??4CRegKey@@QAEAAV0@V0@@Z			; CRegKey::operator=
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
;	COMDAT xdata$x
; File D:\_CODE\Shared\Regkey.cpp
xdata$x	SEGMENT
$T74679	DD	019930520H
	DD	01H
	DD	FLAT:$T74681
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74681	DD	0ffffffffH
	DD	FLAT:$L74677
xdata$x	ENDS
;	COMDAT ??4CRegKey@@QAEAAV0@V0@@Z
_TEXT	SEGMENT
_regKey$ = 8
_this$ = -20
$T74676 = -16
__$EHRec$ = -12
??4CRegKey@@QAEAAV0@V0@@Z PROC NEAR			; CRegKey::operator=, COMDAT

; 36   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74680
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 37   : 	m_hKey = regKey.m_hKey;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _regKey$[ebp+4]
	mov	DWORD PTR [eax+4], ecx

; 38   : 	m_sPath = regKey.m_sPath;

	lea	edx, DWORD PTR _regKey$[ebp+12]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=

; 39   : 
; 40   :     return *this; 

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR $T74676[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _regKey$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74676[ebp]

; 41   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74677:
	lea	ecx, DWORD PTR _regKey$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	ret	0
$L74680:
	mov	eax, OFFSET FLAT:$T74679
	jmp	___CxxFrameHandler
text$x	ENDS
??4CRegKey@@QAEAAV0@V0@@Z ENDP				; CRegKey::operator=
EXTRN	__imp__RegCreateKeyExW@36:NEAR
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	??4CString@@QAEABV0@PBG@Z:NEAR			; CString::operator=
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74064 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
data	ENDS
;	COMDAT ?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z
_TEXT	SEGMENT
_hKeyRoot$ = 8
_pszPath$ = 12
_this$ = -12
_dwDisp$ = -8
_lResult$ = -4
?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z PROC NEAR		; CRegKey::Open, COMDAT

; 44   : {

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

; 45   : 	DWORD dwDisp;
; 46   : 	LONG lResult;
; 47   : 
; 48   : 	// if the key has aleady been opened then close it first
; 49   : 	if (m_hKey)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74059
$L74060:

; 51   : 		ASSERT(0);

	mov	ecx, 1
	test	ecx, ecx
	je	SHORT $L74063
	push	51					; 00000033H
	push	OFFSET FLAT:$SG74064
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74063
	int	3
$L74063:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74060

; 52   : 		return ERROR_ACCESS_DENIED;

	mov	eax, 5
	jmp	SHORT $L74056
$L74059:

; 54   : 
; 55   : 	m_sPath = pszPath;

	mov	eax, DWORD PTR _pszPath$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 56   : 	m_hKeyRoot = hKeyRoot;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _hKeyRoot$[ebp]
	mov	DWORD PTR [ecx+8], edx

; 57   : 
; 58   : 	lResult = RegCreateKeyEx(hKeyRoot, pszPath, 0L, NULL,
; 59   : 				REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, 
; 60   : 				&m_hKey, &dwDisp);

	mov	esi, esp
	lea	eax, DWORD PTR _dwDisp$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	push	ecx
	push	0
	push	983103					; 000f003fH
	push	0
	push	0
	push	0
	mov	edx, DWORD PTR _pszPath$[ebp]
	push	edx
	mov	eax, DWORD PTR _hKeyRoot$[ebp]
	push	eax
	call	DWORD PTR __imp__RegCreateKeyExW@36
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lResult$[ebp], eax

; 61   : 
; 62   : 	return lResult;

	mov	eax, DWORD PTR _lResult$[ebp]
$L74056:

; 63   : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z ENDP		; CRegKey::Open
_TEXT	ENDS
PUBLIC	?DeleteKey@CRegKey@@SAJPAUHKEY__@@PBG@Z		; CRegKey::DeleteKey
PUBLIC	?KeyExists@CRegKey@@SAHPAUHKEY__@@PBG@Z		; CRegKey::KeyExists
EXTRN	__imp__RegCloseKey@4:NEAR
;	COMDAT ?KeyExists@CRegKey@@SAHPAUHKEY__@@PBG@Z
_TEXT	SEGMENT
_hKeyRoot$ = 8
_pszPath$ = 12
_bExistingKey$ = -8
_dwDisp$ = -12
_hTemp$ = -4
?KeyExists@CRegKey@@SAHPAUHKEY__@@PBG@Z PROC NEAR	; CRegKey::KeyExists, COMDAT

; 66   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 67   : 	BOOL bExistingKey;
; 68   : 	DWORD dwDisp;
; 69   : 	HKEY hTemp;
; 70   : 
; 71   : 	// open a temporary key
; 72   : 	RegCreateKeyEx(hKeyRoot, pszPath, 0L, NULL,
; 73   : 				REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, 
; 74   : 				&hTemp, &dwDisp);

	mov	esi, esp
	lea	eax, DWORD PTR _dwDisp$[ebp]
	push	eax
	lea	ecx, DWORD PTR _hTemp$[ebp]
	push	ecx
	push	0
	push	983103					; 000f003fH
	push	0
	push	0
	push	0
	mov	edx, DWORD PTR _pszPath$[ebp]
	push	edx
	mov	eax, DWORD PTR _hKeyRoot$[ebp]
	push	eax
	call	DWORD PTR __imp__RegCreateKeyExW@36
	cmp	esi, esp
	call	__chkesp

; 75   : 	
; 76   : 	// make sure we close it
; 77   : 	RegCloseKey(hTemp);

	mov	esi, esp
	mov	ecx, DWORD PTR _hTemp$[ebp]
	push	ecx
	call	DWORD PTR __imp__RegCloseKey@4
	cmp	esi, esp
	call	__chkesp

; 78   : 
; 79   : 	// test for existing
; 80   : 	bExistingKey = (dwDisp == REG_OPENED_EXISTING_KEY);

	xor	edx, edx
	cmp	DWORD PTR _dwDisp$[ebp], 2
	sete	dl
	mov	DWORD PTR _bExistingKey$[ebp], edx

; 81   : 
; 82   : 	// and then delete the physical entry from the registry if new
; 83   : 	if (!bExistingKey)

	cmp	DWORD PTR _bExistingKey$[ebp], 0
	jne	SHORT $L74072

; 84   : 		DeleteKey(hKeyRoot, pszPath);

	mov	eax, DWORD PTR _pszPath$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hKeyRoot$[ebp]
	push	ecx
	call	?DeleteKey@CRegKey@@SAJPAUHKEY__@@PBG@Z	; CRegKey::DeleteKey
	add	esp, 8
$L74072:

; 85   : 
; 86   : 	return bExistingKey;

	mov	eax, DWORD PTR _bExistingKey$[ebp]

; 87   : }

	pop	esi
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?KeyExists@CRegKey@@SAHPAUHKEY__@@PBG@Z ENDP		; CRegKey::KeyExists
_TEXT	ENDS
PUBLIC	?RecurseDeleteKey@CRegKey@@KAJPAUHKEY__@@PBG@Z	; CRegKey::RecurseDeleteKey
EXTRN	__imp__RegDeleteKeyW@8:NEAR
EXTRN	__imp__RegEnumKeyExW@32:NEAR
EXTRN	__imp__RegOpenKeyExW@20:NEAR
;	COMDAT ?RecurseDeleteKey@CRegKey@@KAJPAUHKEY__@@PBG@Z
_TEXT	SEGMENT
_key$ = 8
_lpszKey$ = 12
_rslt$ = -532
_lRes$ = -516
_time$ = -524
_szBuffer$ = -512
_dwSize$ = -528
?RecurseDeleteKey@CRegKey@@KAJPAUHKEY__@@PBG@Z PROC NEAR ; CRegKey::RecurseDeleteKey, COMDAT

; 90   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 532				; 00000214H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-532]
	mov	ecx, 133				; 00000085H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 91   : 	HKEY rslt;
; 92   : 	LONG lRes;
; 93   : 	FILETIME time;
; 94   : 	TCHAR szBuffer[256];
; 95   : 	DWORD dwSize = 256;

	mov	DWORD PTR _dwSize$[ebp], 256		; 00000100H

; 96   : 
; 97   : 	lRes = RegOpenKeyEx(key, lpszKey, 0, KEY_ALL_ACCESS, &rslt);

	mov	esi, esp
	lea	eax, DWORD PTR _rslt$[ebp]
	push	eax
	push	983103					; 000f003fH
	push	0
	mov	ecx, DWORD PTR _lpszKey$[ebp]
	push	ecx
	mov	edx, DWORD PTR _key$[ebp]
	push	edx
	call	DWORD PTR __imp__RegOpenKeyExW@20
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lRes$[ebp], eax

; 98   : 
; 99   : 	if (lRes != ERROR_SUCCESS)

	cmp	DWORD PTR _lRes$[ebp], 0
	je	SHORT $L74082

; 100  : 		return lRes;

	mov	eax, DWORD PTR _lRes$[ebp]
	jmp	$L74076
$L74082:

; 101  : 
; 102  : 	while (RegEnumKeyEx(rslt, 0, szBuffer, &dwSize, NULL, NULL, NULL, &time) == ERROR_SUCCESS)

	mov	esi, esp
	lea	eax, DWORD PTR _time$[ebp]
	push	eax
	push	0
	push	0
	push	0
	lea	ecx, DWORD PTR _dwSize$[ebp]
	push	ecx
	lea	edx, DWORD PTR _szBuffer$[ebp]
	push	edx
	push	0
	mov	eax, DWORD PTR _rslt$[ebp]
	push	eax
	call	DWORD PTR __imp__RegEnumKeyExW@32
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L74085

; 104  : 		lRes = RecurseDeleteKey(rslt, szBuffer);

	lea	ecx, DWORD PTR _szBuffer$[ebp]
	push	ecx
	mov	edx, DWORD PTR _rslt$[ebp]
	push	edx
	call	?RecurseDeleteKey@CRegKey@@KAJPAUHKEY__@@PBG@Z ; CRegKey::RecurseDeleteKey
	add	esp, 8
	mov	DWORD PTR _lRes$[ebp], eax

; 105  : 
; 106  : 		if (lRes != ERROR_SUCCESS)

	cmp	DWORD PTR _lRes$[ebp], 0
	je	SHORT $L74086

; 107  : 			return lRes;

	mov	eax, DWORD PTR _lRes$[ebp]
	jmp	SHORT $L74076
$L74086:

; 108  : 
; 109  : 		dwSize = 256;

	mov	DWORD PTR _dwSize$[ebp], 256		; 00000100H

; 110  : 	}

	jmp	SHORT $L74082
$L74085:

; 111  : 
; 112  : 	RegCloseKey(rslt);

	mov	esi, esp
	mov	eax, DWORD PTR _rslt$[ebp]
	push	eax
	call	DWORD PTR __imp__RegCloseKey@4
	cmp	esi, esp
	call	__chkesp

; 113  : 
; 114  : 	return RegDeleteKey(key, lpszKey);

	mov	esi, esp
	mov	ecx, DWORD PTR _lpszKey$[ebp]
	push	ecx
	mov	edx, DWORD PTR _key$[ebp]
	push	edx
	call	DWORD PTR __imp__RegDeleteKeyW@8
	cmp	esi, esp
	call	__chkesp
$L74076:

; 115  : }

	pop	edi
	pop	esi
	add	esp, 532				; 00000214H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?RecurseDeleteKey@CRegKey@@KAJPAUHKEY__@@PBG@Z ENDP	; CRegKey::RecurseDeleteKey
_TEXT	ENDS
;	COMDAT ?DeleteKey@CRegKey@@SAJPAUHKEY__@@PBG@Z
_TEXT	SEGMENT
_hKeyRoot$ = 8
_pszPath$ = 12
?DeleteKey@CRegKey@@SAJPAUHKEY__@@PBG@Z PROC NEAR	; CRegKey::DeleteKey, COMDAT

; 118  : {

	push	ebp
	mov	ebp, esp

; 119  : 	return RecurseDeleteKey(hKeyRoot, pszPath);

	mov	eax, DWORD PTR _pszPath$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hKeyRoot$[ebp]
	push	ecx
	call	?RecurseDeleteKey@CRegKey@@KAJPAUHKEY__@@PBG@Z ; CRegKey::RecurseDeleteKey
	add	esp, 8

; 120  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?DeleteKey@CRegKey@@SAJPAUHKEY__@@PBG@Z ENDP		; CRegKey::DeleteKey
_TEXT	ENDS
;	COMDAT ?Close@CRegKey@@QAEXXZ
_TEXT	SEGMENT
_this$ = -4
?Close@CRegKey@@QAEXXZ PROC NEAR			; CRegKey::Close, COMDAT

; 123  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 124  : 	if (m_hKey)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L74094

; 126  : 		RegCloseKey(m_hKey);

	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR __imp__RegCloseKey@4
	cmp	esi, esp
	call	__chkesp
$L74094:

; 128  : 		
; 129  : 	m_hKey = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+4], 0

; 130  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Close@CRegKey@@QAEXXZ ENDP				; CRegKey::Close
_TEXT	ENDS
PUBLIC	?Write@CRegKey@@QAEJPBGK@Z			; CRegKey::Write
EXTRN	__imp__RegSetValueExW@24:NEAR
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74104 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
	ORG $+1
$SG74109 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
data	ENDS
;	COMDAT ?Write@CRegKey@@QAEJPBGK@Z
_TEXT	SEGMENT
_pszItem$ = 8
_dwVal$ = 12
_this$ = -4
?Write@CRegKey@@QAEJPBGK@Z PROC NEAR			; CRegKey::Write, COMDAT

; 133  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74100:

; 134  : 	ASSERT(m_hKey);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74103
	push	134					; 00000086H
	push	OFFSET FLAT:$SG74104
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74103
	int	3
$L74103:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74100
$L74102:

; 135  : 	ASSERT(pszItem);

	cmp	DWORD PTR _pszItem$[ebp], 0
	jne	SHORT $L74108
	push	135					; 00000087H
	push	OFFSET FLAT:$SG74109
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74108
	int	3
$L74108:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74102

; 136  : 	return RegSetValueEx(m_hKey, pszItem, 0L, REG_DWORD,
; 137  : 		(CONST BYTE*) &dwVal, sizeof(DWORD));

	mov	esi, esp
	push	4
	lea	eax, DWORD PTR _dwVal$[ebp]
	push	eax
	push	4
	push	0
	mov	ecx, DWORD PTR _pszItem$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	DWORD PTR __imp__RegSetValueExW@24
	cmp	esi, esp
	call	__chkesp

; 138  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Write@CRegKey@@QAEJPBGK@Z ENDP				; CRegKey::Write
_TEXT	ENDS
PUBLIC	?GetValueType@CRegKey@@QBEKPBG@Z		; CRegKey::GetValueType
EXTRN	__imp__RegQueryValueExW@24:NEAR
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74120 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
	ORG $+1
$SG74125 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
data	ENDS
;	COMDAT ?GetValueType@CRegKey@@QBEKPBG@Z
_TEXT	SEGMENT
_pszItem$ = 8
_this$ = -12
_dwType$ = -8
_lRet$ = -4
?GetValueType@CRegKey@@QBEKPBG@Z PROC NEAR		; CRegKey::GetValueType, COMDAT

; 141  : {

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
$L74116:

; 142  : 	ASSERT(m_hKey);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74119
	push	142					; 0000008eH
	push	OFFSET FLAT:$SG74120
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74119
	int	3
$L74119:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74116
$L74118:

; 143  : 	ASSERT(pszItem);

	cmp	DWORD PTR _pszItem$[ebp], 0
	jne	SHORT $L74124
	push	143					; 0000008fH
	push	OFFSET FLAT:$SG74125
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74124
	int	3
$L74124:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74118

; 144  : 
; 145  : 	DWORD dwType;
; 146  : 
; 147  : 	LONG lRet = RegQueryValueEx (m_hKey, pszItem, NULL, &dwType, NULL, NULL);

	mov	esi, esp
	push	0
	push	0
	lea	eax, DWORD PTR _dwType$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _pszItem$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	DWORD PTR __imp__RegQueryValueExW@24
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lRet$[ebp], eax

; 148  : 
; 149  : 	if (lRet == ERROR_SUCCESS)

	cmp	DWORD PTR _lRet$[ebp], 0
	jne	SHORT $L74128

; 150  : 		return dwType;

	mov	eax, DWORD PTR _dwType$[ebp]
	jmp	SHORT $L74115
$L74128:

; 151  : 
; 152  : 	return REG_NONE;

	xor	eax, eax
$L74115:

; 153  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetValueType@CRegKey@@QBEKPBG@Z ENDP			; CRegKey::GetValueType
_TEXT	ENDS
PUBLIC	?Write@CRegKey@@QAEJPBG0@Z			; CRegKey::Write
EXTRN	?AfxIsValidAddress@@YGHPBXIH@Z:NEAR		; AfxIsValidAddress
EXTRN	__imp__lstrlenW@4:NEAR
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74138 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
	ORG $+1
$SG74143 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
	ORG $+1
$SG74148 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
	ORG $+1
$SG74154 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
data	ENDS
;	COMDAT ?Write@CRegKey@@QAEJPBG0@Z
_TEXT	SEGMENT
_pszItem$ = 8
_pszVal$ = 12
_this$ = -8
_nBytes$ = -4
?Write@CRegKey@@QAEJPBG0@Z PROC NEAR			; CRegKey::Write, COMDAT

; 156  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74134:

; 157  : 	ASSERT(m_hKey);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74137
	push	157					; 0000009dH
	push	OFFSET FLAT:$SG74138
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74137
	int	3
$L74137:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74134
$L74136:

; 158  : 	ASSERT(pszItem);

	cmp	DWORD PTR _pszItem$[ebp], 0
	jne	SHORT $L74142
	push	158					; 0000009eH
	push	OFFSET FLAT:$SG74143
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74142
	int	3
$L74142:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74136
$L74141:

; 159  : 	ASSERT(pszVal);

	cmp	DWORD PTR _pszVal$[ebp], 0
	jne	SHORT $L74147
	push	159					; 0000009fH
	push	OFFSET FLAT:$SG74148
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74147
	int	3
$L74147:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74141
$L74146:

; 160  : 
; 161  : #ifndef _NOT_USING_MFC_
; 162  : 	ASSERT(AfxIsValidAddress(pszVal, lstrlen(pszVal) * sizeof(TCHAR), FALSE));

	push	0
	mov	esi, esp
	mov	ecx, DWORD PTR _pszVal$[ebp]
	push	ecx
	call	DWORD PTR __imp__lstrlenW@4
	cmp	esi, esp
	call	__chkesp
	shl	eax, 1
	push	eax
	mov	edx, DWORD PTR _pszVal$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74153
	push	162					; 000000a2H
	push	OFFSET FLAT:$SG74154
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74153
	int	3
$L74153:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74146

; 163  : #endif
; 164  : 
; 165  : 	int nBytes = (lstrlen(pszVal) + 1) * sizeof(TCHAR);

	mov	esi, esp
	mov	ecx, DWORD PTR _pszVal$[ebp]
	push	ecx
	call	DWORD PTR __imp__lstrlenW@4
	cmp	esi, esp
	call	__chkesp
	lea	edx, DWORD PTR [eax+eax+2]
	mov	DWORD PTR _nBytes$[ebp], edx

; 166  : 
; 167  : 	return RegSetValueEx(m_hKey, pszItem, 0L, REG_SZ, (CONST BYTE*) pszVal, nBytes);

	mov	esi, esp
	mov	eax, DWORD PTR _nBytes$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pszVal$[ebp]
	push	ecx
	push	1
	push	0
	mov	edx, DWORD PTR _pszItem$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__RegSetValueExW@24
	cmp	esi, esp
	call	__chkesp

; 168  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Write@CRegKey@@QAEJPBG0@Z ENDP				; CRegKey::Write
_TEXT	ENDS
PUBLIC	?Write@CRegKey@@QAEJPBGPBEK@Z			; CRegKey::Write
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74168 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
	ORG $+1
$SG74173 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
	ORG $+1
$SG74179 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
	ORG $+1
$SG74184 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
data	ENDS
;	COMDAT ?Write@CRegKey@@QAEJPBGPBEK@Z
_TEXT	SEGMENT
_pszItem$ = 8
_pData$ = 12
_dwLength$ = 16
_this$ = -4
?Write@CRegKey@@QAEJPBGPBEK@Z PROC NEAR			; CRegKey::Write, COMDAT

; 171  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74164:

; 172  : 	ASSERT(m_hKey);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74167
	push	172					; 000000acH
	push	OFFSET FLAT:$SG74168
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74167
	int	3
$L74167:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74164
$L74166:

; 173  : 	ASSERT(pszItem);

	cmp	DWORD PTR _pszItem$[ebp], 0
	jne	SHORT $L74172
	push	173					; 000000adH
	push	OFFSET FLAT:$SG74173
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74172
	int	3
$L74172:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74166
$L74171:

; 174  : 	ASSERT(pData && dwLength > 0);

	cmp	DWORD PTR _pData$[ebp], 0
	je	SHORT $L74178
	cmp	DWORD PTR _dwLength$[ebp], 0
	ja	SHORT $L74177
$L74178:
	push	174					; 000000aeH
	push	OFFSET FLAT:$SG74179
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74177
	int	3
$L74177:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74171
$L74176:

; 175  : 
; 176  : #ifndef _NOT_USING_MFC_
; 177  : 	ASSERT(AfxIsValidAddress(pData, dwLength, FALSE));

	push	0
	mov	ecx, DWORD PTR _dwLength$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pData$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74183
	push	177					; 000000b1H
	push	OFFSET FLAT:$SG74184
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74183
	int	3
$L74183:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74176

; 178  : #endif
; 179  : 
; 180  : 	return RegSetValueEx (m_hKey, pszItem, 0L, REG_BINARY, pData, dwLength);

	mov	esi, esp
	mov	ecx, DWORD PTR _dwLength$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pData$[ebp]
	push	edx
	push	3
	push	0
	mov	eax, DWORD PTR _pszItem$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR __imp__RegSetValueExW@24
	cmp	esi, esp
	call	__chkesp

; 181  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?Write@CRegKey@@QAEJPBGPBEK@Z ENDP			; CRegKey::Write
_TEXT	ENDS
PUBLIC	?Read@CRegKey@@QBEJPBGAAK@Z			; CRegKey::Read
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74194 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
	ORG $+1
$SG74199 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
data	ENDS
;	COMDAT ?Read@CRegKey@@QBEJPBGAAK@Z
_TEXT	SEGMENT
_pszItem$ = 8
_dwVal$ = 12
_this$ = -20
_dwType$ = -8
_dwSize$ = -12
_dwDest$ = -16
_lRet$ = -4
?Read@CRegKey@@QBEJPBGAAK@Z PROC NEAR			; CRegKey::Read, COMDAT

; 184  : {

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
$L74190:

; 185  : 	ASSERT(m_hKey);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74193
	push	185					; 000000b9H
	push	OFFSET FLAT:$SG74194
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74193
	int	3
$L74193:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74190
$L74192:

; 186  : 	ASSERT(pszItem);

	cmp	DWORD PTR _pszItem$[ebp], 0
	jne	SHORT $L74198
	push	186					; 000000baH
	push	OFFSET FLAT:$SG74199
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74198
	int	3
$L74198:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74192

; 187  : 
; 188  : 	DWORD dwType;
; 189  : 	DWORD dwSize = sizeof (DWORD);

	mov	DWORD PTR _dwSize$[ebp], 4

; 190  : 	DWORD dwDest;
; 191  : 
; 192  : 	LONG lRet = RegQueryValueEx (m_hKey, pszItem, NULL, 
; 193  : 		&dwType, (BYTE *) &dwDest, &dwSize);

	mov	esi, esp
	lea	eax, DWORD PTR _dwSize$[ebp]
	push	eax
	lea	ecx, DWORD PTR _dwDest$[ebp]
	push	ecx
	lea	edx, DWORD PTR _dwType$[ebp]
	push	edx
	push	0
	mov	eax, DWORD PTR _pszItem$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR __imp__RegQueryValueExW@24
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lRet$[ebp], eax

; 194  : 
; 195  : 	if (lRet == ERROR_SUCCESS)

	cmp	DWORD PTR _lRet$[ebp], 0
	jne	SHORT $L74206

; 196  : 		dwVal = dwDest;

	mov	eax, DWORD PTR _dwVal$[ebp]
	mov	ecx, DWORD PTR _dwDest$[ebp]
	mov	DWORD PTR [eax], ecx
$L74206:

; 197  : 
; 198  : 	return lRet;

	mov	eax, DWORD PTR _lRet$[ebp]

; 199  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Read@CRegKey@@QBEJPBGAAK@Z ENDP			; CRegKey::Read
_TEXT	ENDS
PUBLIC	?Read@CRegKey@@QBEJPBGAAVCString@@@Z		; CRegKey::Read
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74216 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
data	ENDS
;	COMDAT ?Read@CRegKey@@QBEJPBGAAVCString@@@Z
_TEXT	SEGMENT
_pszItem$ = 8
_sVal$ = 12
_this$ = -216
_dwType$ = -204
_dwSize$ = -208
_string$ = -200
_lReturn$ = -212
?Read@CRegKey@@QBEJPBGAAVCString@@@Z PROC NEAR		; CRegKey::Read, COMDAT

; 202  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 216				; 000000d8H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-216]
	mov	ecx, 54					; 00000036H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L74212:

; 203  : 	ASSERT(m_hKey);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74215
	push	203					; 000000cbH
	push	OFFSET FLAT:$SG74216
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74215
	int	3
$L74215:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74212

; 204  : //	ASSERT(pszItem);
; 205  : 
; 206  : 	DWORD dwType;
; 207  : 	DWORD dwSize = 200;

	mov	DWORD PTR _dwSize$[ebp], 200		; 000000c8H

; 208  : 	char  string[200];
; 209  : 
; 210  : 	LONG lReturn = RegQueryValueEx (m_hKey, pszItem, NULL,
; 211  : 		&dwType, (BYTE *) string, &dwSize);

	mov	esi, esp
	lea	edx, DWORD PTR _dwSize$[ebp]
	push	edx
	lea	eax, DWORD PTR _string$[ebp]
	push	eax
	lea	ecx, DWORD PTR _dwType$[ebp]
	push	ecx
	push	0
	mov	edx, DWORD PTR _pszItem$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__RegQueryValueExW@24
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lReturn$[ebp], eax

; 212  : 
; 213  : 	if (lReturn == ERROR_SUCCESS)

	cmp	DWORD PTR _lReturn$[ebp], 0
	jne	SHORT $L74222

; 214  : 		sVal = (TCHAR*)string;

	lea	edx, DWORD PTR _string$[ebp]
	push	edx
	mov	ecx, DWORD PTR _sVal$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L74222:

; 215  : 
; 216  : 	return lReturn;

	mov	eax, DWORD PTR _lReturn$[ebp]

; 217  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 216				; 000000d8H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Read@CRegKey@@QBEJPBGAAVCString@@@Z ENDP		; CRegKey::Read
_TEXT	ENDS
PUBLIC	?Read@CRegKey@@QBEJPBGPAEAAK@Z			; CRegKey::Read
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74234 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
	ORG $+1
$SG74239 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
data	ENDS
;	COMDAT ?Read@CRegKey@@QBEJPBGPAEAAK@Z
_TEXT	SEGMENT
_pszItem$ = 8
_pData$ = 12
_dwLen$ = 16
_this$ = -8
_dwType$ = -4
?Read@CRegKey@@QBEJPBGPAEAAK@Z PROC NEAR		; CRegKey::Read, COMDAT

; 220  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L74230:

; 221  : 	ASSERT(m_hKey);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74233
	push	221					; 000000ddH
	push	OFFSET FLAT:$SG74234
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74233
	int	3
$L74233:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74230
$L74232:

; 222  : 	ASSERT(pszItem);

	cmp	DWORD PTR _pszItem$[ebp], 0
	jne	SHORT $L74238
	push	222					; 000000deH
	push	OFFSET FLAT:$SG74239
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74238
	int	3
$L74238:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74232

; 223  : 
; 224  : 	DWORD dwType;
; 225  : 
; 226  : 	return RegQueryValueEx(m_hKey, pszItem, NULL,
; 227  : 		&dwType, pData, &dwLen);

	mov	esi, esp
	mov	eax, DWORD PTR _dwLen$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pData$[ebp]
	push	ecx
	lea	edx, DWORD PTR _dwType$[ebp]
	push	edx
	push	0
	mov	eax, DWORD PTR _pszItem$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR __imp__RegQueryValueExW@24
	cmp	esi, esp
	call	__chkesp

; 228  : 
; 229  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?Read@CRegKey@@QBEJPBGPAEAAK@Z ENDP			; CRegKey::Read
_TEXT	ENDS
PUBLIC	?GetSubkeyNames@CRegKey@@QBEHAAVCStringArray@@@Z ; CRegKey::GetSubkeyNames
EXTRN	__imp__RegEnumKeyW@16:NEAR
EXTRN	?GetSize@CStringArray@@QBEHXZ:NEAR		; CStringArray::GetSize
EXTRN	?RemoveAll@CStringArray@@QAEXXZ:NEAR		; CStringArray::RemoveAll
EXTRN	?Add@CStringArray@@QAEHPBG@Z:NEAR		; CStringArray::Add
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74249 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
data	ENDS
;	COMDAT ?GetSubkeyNames@CRegKey@@QBEHAAVCStringArray@@@Z
_TEXT	SEGMENT
_aNames$ = 8
_this$ = -1040
_nKey$ = -1028
_szName$ = -1024
_lResult$ = -1032
_nNameLen$ = -1036
?GetSubkeyNames@CRegKey@@QBEHAAVCStringArray@@@Z PROC NEAR ; CRegKey::GetSubkeyNames, COMDAT

; 232  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 1040				; 00000410H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-1040]
	mov	ecx, 260				; 00000104H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L74245:

; 233  : 	ASSERT(m_hKey);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74248
	push	233					; 000000e9H
	push	OFFSET FLAT:$SG74249
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74248
	int	3
$L74248:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74245

; 234  : 
; 235  :    aNames.RemoveAll();

	mov	ecx, DWORD PTR _aNames$[ebp]
	call	?RemoveAll@CStringArray@@QAEXXZ		; CStringArray::RemoveAll

; 236  : 
; 237  :    DWORD nKey = 0;

	mov	DWORD PTR _nKey$[ebp], 0

; 238  :    TCHAR szName[512];
; 239  :    LONG lResult = ERROR_SUCCESS;

	mov	DWORD PTR _lResult$[ebp], 0

; 240  :    DWORD nNameLen = sizeof(szName)/sizeof(TCHAR);

	mov	DWORD PTR _nNameLen$[ebp], 512		; 00000200H
$L74256:

; 241  : 
; 242  :    while (lResult == ERROR_SUCCESS)

	cmp	DWORD PTR _lResult$[ebp], 0
	jne	SHORT $L74257

; 244  :       lResult = ::RegEnumKey(m_hKey, nKey, szName, nNameLen);

	mov	esi, esp
	mov	edx, DWORD PTR _nNameLen$[ebp]
	push	edx
	lea	eax, DWORD PTR _szName$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nKey$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	DWORD PTR __imp__RegEnumKeyW@16
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lResult$[ebp], eax

; 245  :       
; 246  :       // we have a valid key name
; 247  :       if (lResult == ERROR_SUCCESS)

	cmp	DWORD PTR _lResult$[ebp], 0
	jne	SHORT $L74258

; 248  :          aNames.Add(szName);

	lea	ecx, DWORD PTR _szName$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _aNames$[ebp]
	call	?Add@CStringArray@@QAEHPBG@Z		; CStringArray::Add
$L74258:

; 249  : 
; 250  :       nKey++; // next 

	mov	edx, DWORD PTR _nKey$[ebp]
	add	edx, 1
	mov	DWORD PTR _nKey$[ebp], edx

; 251  :    }

	jmp	SHORT $L74256
$L74257:

; 252  : 
; 253  :    return aNames.GetSize();

	mov	ecx, DWORD PTR _aNames$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize

; 254  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 1040				; 00000410H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetSubkeyNames@CRegKey@@QBEHAAVCStringArray@@@Z ENDP	; CRegKey::GetSubkeyNames
_TEXT	ENDS
PUBLIC	?GetValueNames@CRegKey@@QBEHAAVCStringArray@@@Z	; CRegKey::GetValueNames
EXTRN	__imp__RegEnumValueW@32:NEAR
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74267 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
data	ENDS
;	COMDAT ?GetValueNames@CRegKey@@QBEHAAVCStringArray@@@Z
_TEXT	SEGMENT
_aNames$ = 8
_this$ = -1040
_nVal$ = -4
_szName$ = -1028
_lResult$ = -1032
_nNameLen$74274 = -1036
?GetValueNames@CRegKey@@QBEHAAVCStringArray@@@Z PROC NEAR ; CRegKey::GetValueNames, COMDAT

; 257  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 1040				; 00000410H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-1040]
	mov	ecx, 260				; 00000104H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L74263:

; 258  : 	ASSERT(m_hKey);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74266
	push	258					; 00000102H
	push	OFFSET FLAT:$SG74267
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74266
	int	3
$L74266:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74263

; 259  : 
; 260  :    DWORD nVal = 0;

	mov	DWORD PTR _nVal$[ebp], 0

; 261  :    TCHAR szName[512];
; 262  :    LONG lResult = ERROR_SUCCESS;

	mov	DWORD PTR _lResult$[ebp], 0
$L74272:

; 263  : 
; 264  :    while (lResult == ERROR_SUCCESS)

	cmp	DWORD PTR _lResult$[ebp], 0
	jne	SHORT $L74273

; 266  :       DWORD nNameLen = sizeof(szName)/sizeof(TCHAR);

	mov	DWORD PTR _nNameLen$74274[ebp], 512	; 00000200H

; 267  : 
; 268  :       lResult = ::RegEnumValue(m_hKey, nVal, szName, &nNameLen,
; 269  :                                NULL, NULL, NULL, NULL);

	mov	esi, esp
	push	0
	push	0
	push	0
	push	0
	lea	edx, DWORD PTR _nNameLen$74274[ebp]
	push	edx
	lea	eax, DWORD PTR _szName$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nVal$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	DWORD PTR __imp__RegEnumValueW@32
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lResult$[ebp], eax

; 270  :       
; 271  :       // we have a valid key name
; 272  :       if (lResult == ERROR_SUCCESS)

	cmp	DWORD PTR _lResult$[ebp], 0
	jne	SHORT $L74276

; 273  :          aNames.Add(szName);

	lea	ecx, DWORD PTR _szName$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _aNames$[ebp]
	call	?Add@CStringArray@@QAEHPBG@Z		; CStringArray::Add
$L74276:

; 274  : 
; 275  :       nVal++; // next 

	mov	edx, DWORD PTR _nVal$[ebp]
	add	edx, 1
	mov	DWORD PTR _nVal$[ebp], edx

; 276  :    } 

	jmp	SHORT $L74272
$L74273:

; 277  : 
; 278  :    return aNames.GetSize();

	mov	ecx, DWORD PTR _aNames$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize

; 279  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 1040				; 00000410H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetValueNames@CRegKey@@QBEHAAVCStringArray@@@Z ENDP	; CRegKey::GetValueNames
_TEXT	ENDS
PUBLIC	?HasValues@CRegKey@@QBEHXZ			; CRegKey::HasValues
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74284 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
data	ENDS
;	COMDAT ?HasValues@CRegKey@@QBEHXZ
_TEXT	SEGMENT
_this$ = -1032
_szName$ = -1024
_nNameLen$ = -1028
?HasValues@CRegKey@@QBEHXZ PROC NEAR			; CRegKey::HasValues, COMDAT

; 282  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 1032				; 00000408H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-1032]
	mov	ecx, 258				; 00000102H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L74280:

; 283  : 	ASSERT(m_hKey);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74283
	push	283					; 0000011bH
	push	OFFSET FLAT:$SG74284
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74283
	int	3
$L74283:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74280

; 284  : 
; 285  :    TCHAR szName[512];
; 286  :    DWORD nNameLen = sizeof(szName)/sizeof(TCHAR);

	mov	DWORD PTR _nNameLen$[ebp], 512		; 00000200H

; 287  : 
; 288  :    return (::RegEnumValue(m_hKey, 0, szName, &nNameLen,
; 289  :                           NULL, NULL, NULL, NULL) == ERROR_SUCCESS);

	mov	esi, esp
	push	0
	push	0
	push	0
	push	0
	lea	edx, DWORD PTR _nNameLen$[ebp]
	push	edx
	lea	eax, DWORD PTR _szName$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR __imp__RegEnumValueW@32
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax

; 290  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 1032				; 00000408H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?HasValues@CRegKey@@QBEHXZ ENDP				; CRegKey::HasValues
_TEXT	ENDS
PUBLIC	?GetAppRegPath@CRegKey@@SA?AVCString@@PBG@Z	; CRegKey::GetAppRegPath
EXTRN	?AfxGetApp@@YGPAVCWinApp@@XZ:NEAR		; AfxGetApp
EXTRN	?AfxGetAppName@@YGPBGXZ:NEAR			; AfxGetAppName
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	??4CString@@QAEABV0@PBD@Z:NEAR			; CString::operator=
EXTRN	??YCString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator+=
EXTRN	??YCString@@QAEABV0@D@Z:NEAR			; CString::operator+=
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74296 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
	ORG $+1
$SG74301 DB	'Software\', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74735	DD	019930520H
	DD	04H
	DD	FLAT:$T74737
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74737	DD	0ffffffffH
	DD	FLAT:$L74731
	DD	00H
	DD	FLAT:$L74728
	DD	01H
	DD	FLAT:$L74729
	DD	02H
	DD	FLAT:$L74730
xdata$x	ENDS
;	COMDAT ?GetAppRegPath@CRegKey@@SA?AVCString@@PBG@Z
_TEXT	SEGMENT
_szAppName$ = 12
___$ReturnUdt$ = 8
_sRegPath$ = -16
_sAppName$ = -20
$T74727 = -24
$T74732 = -28
__$EHRec$ = -12
?GetAppRegPath@CRegKey@@SA?AVCString@@PBG@Z PROC NEAR	; CRegKey::GetAppRegPath, COMDAT

; 295  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74736
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
	mov	DWORD PTR $T74732[ebp], 0
$L74292:

; 296  : 	ASSERT(AfxGetApp()->m_pszRegistryKey);

	call	?AfxGetApp@@YGPAVCWinApp@@XZ		; AfxGetApp
	cmp	DWORD PTR [eax+132], 0
	jne	SHORT $L74295
	push	296					; 00000128H
	push	OFFSET FLAT:$SG74296
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74295
	int	3
$L74295:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74292

; 297  : 	CString sRegPath, sAppName;

	lea	ecx, DWORD PTR _sRegPath$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sAppName$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 298  : 
; 299  : 	if (szAppName && lstrlen(szAppName))

	cmp	DWORD PTR _szAppName$[ebp], 0
	je	SHORT $L74299
	mov	esi, esp
	mov	ecx, DWORD PTR _szAppName$[ebp]
	push	ecx
	call	DWORD PTR __imp__lstrlenW@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L74299

; 300  : 		sAppName = szAppName;

	mov	edx, DWORD PTR _szAppName$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sAppName$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 301  : 	else

	jmp	SHORT $L74300
$L74299:

; 302  : 		sAppName = AfxGetAppName();

	call	?AfxGetAppName@@YGPBGXZ			; AfxGetAppName
	push	eax
	lea	ecx, DWORD PTR _sAppName$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L74300:

; 303  : 
; 304  : 	// construct reg path
; 305  : 	sRegPath = "Software\\";

	push	OFFSET FLAT:$SG74301
	lea	ecx, DWORD PTR _sRegPath$[ebp]
	call	??4CString@@QAEABV0@PBD@Z		; CString::operator=

; 306  : 	sRegPath += CString(AfxGetApp()->m_pszRegistryKey);

	call	?AfxGetApp@@YGPAVCWinApp@@XZ		; AfxGetApp
	mov	eax, DWORD PTR [eax+132]
	push	eax
	lea	ecx, DWORD PTR $T74727[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR -32+[ebp], eax
	mov	ecx, DWORD PTR -32+[ebp]
	mov	DWORD PTR -36+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	edx, DWORD PTR -36+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sRegPath$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T74727[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 307  : 	sRegPath += '\\';

	push	92					; 0000005cH
	lea	ecx, DWORD PTR _sRegPath$[ebp]
	call	??YCString@@QAEABV0@D@Z			; CString::operator+=

; 308  : 	sRegPath += sAppName;

	lea	eax, DWORD PTR _sAppName$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sRegPath$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=

; 309  : 	sRegPath += '\\';

	push	92					; 0000005cH
	lea	ecx, DWORD PTR _sRegPath$[ebp]
	call	??YCString@@QAEABV0@D@Z			; CString::operator+=

; 310  : 
; 311  : 	return sRegPath;

	lea	ecx, DWORD PTR _sRegPath$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T74732[ebp]
	or	edx, 1
	mov	DWORD PTR $T74732[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sAppName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sRegPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 312  : }

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
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74728:
	lea	ecx, DWORD PTR _sRegPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74729:
	lea	ecx, DWORD PTR _sAppName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74730:
	lea	ecx, DWORD PTR $T74727[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74731:
	mov	eax, DWORD PTR $T74732[ebp]
	and	eax, 1
	test	eax, eax
	je	$L74733
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74733:
	ret	0
$L74736:
	mov	eax, OFFSET FLAT:$T74735
	jmp	___CxxFrameHandler
text$x	ENDS
?GetAppRegPath@CRegKey@@SA?AVCString@@PBG@Z ENDP	; CRegKey::GetAppRegPath
PUBLIC	?ExportToIni@CRegKey@@QBEHPBG@Z			; CRegKey::ExportToIni
PUBLIC	?ExportKeyToIni@CRegKey@@IBEHPBGAAVCStdioFile@@@Z ; CRegKey::ExportKeyToIni
PUBLIC	??1CStdioFileEx@@UAE@XZ				; CStdioFileEx::~CStdioFileEx
EXTRN	__chkstk:NEAR
EXTRN	??0CStdioFileEx@@QAE@XZ:NEAR			; CStdioFileEx::CStdioFileEx
EXTRN	?Open@CStdioFileEx@@UAEHPBGIW4SFE_SAVEAS@@PAVCFileException@@@Z:NEAR ; CStdioFileEx::Open
EXTRN	??0CStringArray@@QAE@XZ:NEAR			; CStringArray::CStringArray
EXTRN	??ACStringArray@@QAEAAVCString@@H@Z:NEAR	; CStringArray::operator[]
EXTRN	??1CStringArray@@UAE@XZ:NEAR			; CStringArray::~CStringArray
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74311 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74750	DD	019930520H
	DD	03H
	DD	FLAT:$T74753
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74753	DD	0ffffffffH
	DD	FLAT:$L74746
	DD	00H
	DD	FLAT:$L74747
	DD	01H
	DD	FLAT:$L74748
xdata$x	ENDS
;	COMDAT ?ExportToIni@CRegKey@@QBEHPBG@Z
_TEXT	SEGMENT
_szIniPath$ = 8
_this$ = -12388
_file$ = -12364
_aSubKeys$ = -32
_nKey$74318 = -12368
_sName$74322 = -12372
$T74743 = -12376
$T74744 = -12380
$T74745 = -12384
__$EHRec$ = -12
?ExportToIni@CRegKey@@QBEHPBG@Z PROC NEAR		; CRegKey::ExportToIni, COMDAT

; 317  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74751
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	mov	eax, 12376				; 00003058H
	call	__chkstk
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-12388]
	mov	ecx, 3094				; 00000c16H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L74307:

; 318  : 	ASSERT (m_hKey);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74310
	push	318					; 0000013eH
	push	OFFSET FLAT:$SG74311
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74310
	int	3
$L74310:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74307

; 319  : 	
; 320  : 	if (!m_hKey)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	jne	SHORT $L74312

; 321  : 		return FALSE;

	xor	eax, eax
	jmp	$L74306
$L74312:

; 322  : 	
; 323  : 	CStdioFileEx file;

	lea	ecx, DWORD PTR _file$[ebp]
	call	??0CStdioFileEx@@QAE@XZ			; CStdioFileEx::CStdioFileEx
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 324  : 	
; 325  : 	if (!file.Open(szIniPath, CFile::modeCreate | CFile::modeWrite))

	push	0
	push	2
	push	4097					; 00001001H
	mov	eax, DWORD PTR _szIniPath$[ebp]
	push	eax
	lea	ecx, DWORD PTR _file$[ebp]
	call	?Open@CStdioFileEx@@UAEHPBGIW4SFE_SAVEAS@@PAVCFileException@@@Z ; CStdioFileEx::Open
	test	eax, eax
	jne	SHORT $L74314

; 326  : 		return FALSE;

	mov	DWORD PTR $T74743[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	mov	eax, DWORD PTR $T74743[ebp]
	jmp	$L74306
$L74314:

; 327  : 	
; 328  : 	// process top level keys. ie we ignore any values in the root
; 329  : 	CStringArray aSubKeys;

	lea	ecx, DWORD PTR _aSubKeys$[ebp]
	call	??0CStringArray@@QAE@XZ			; CStringArray::CStringArray
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 330  : 	
; 331  : 	if (GetSubkeyNames(aSubKeys))

	lea	ecx, DWORD PTR _aSubKeys$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSubkeyNames@CRegKey@@QBEHAAVCStringArray@@@Z ; CRegKey::GetSubkeyNames
	test	eax, eax
	je	$L74321

; 333  : 		for (int nKey = 0; nKey < aSubKeys.GetSize(); nKey++)

	mov	DWORD PTR _nKey$74318[ebp], 0
	jmp	SHORT $L74319
$L74320:
	mov	edx, DWORD PTR _nKey$74318[ebp]
	add	edx, 1
	mov	DWORD PTR _nKey$74318[ebp], edx
$L74319:
	lea	ecx, DWORD PTR _aSubKeys$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	cmp	DWORD PTR _nKey$74318[ebp], eax
	jge	$L74321

; 335  : 			CString sName = aSubKeys[nKey];

	mov	eax, DWORD PTR _nKey$74318[ebp]
	push	eax
	lea	ecx, DWORD PTR _aSubKeys$[ebp]
	call	??ACStringArray@@QAEAAVCString@@H@Z	; CStringArray::operator[]
	push	eax
	lea	ecx, DWORD PTR _sName$74322[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 336  : 			
; 337  : 			if (!ExportKeyToIni(sName, file))

	lea	ecx, DWORD PTR _file$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sName$74322[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ExportKeyToIni@CRegKey@@IBEHPBGAAVCStdioFile@@@Z ; CRegKey::ExportKeyToIni
	test	eax, eax
	jne	SHORT $L74323

; 338  : 				return FALSE;

	mov	DWORD PTR $T74744[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sName$74322[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _aSubKeys$[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	mov	eax, DWORD PTR $T74744[ebp]
	jmp	SHORT $L74306
$L74323:

; 339  : 		}

	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sName$74322[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	$L74320
$L74321:

; 341  : 	
; 342  : 	return TRUE;

	mov	DWORD PTR $T74745[ebp], 1
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _aSubKeys$[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	mov	eax, DWORD PTR $T74745[ebp]
$L74306:

; 343  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12388				; 00003064H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74746:
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	ret	0
$L74747:
	lea	ecx, DWORD PTR _aSubKeys$[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	ret	0
$L74748:
	lea	ecx, DWORD PTR _sName$74322[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74751:
	mov	eax, OFFSET FLAT:$T74750
	jmp	___CxxFrameHandler
text$x	ENDS
?ExportToIni@CRegKey@@QBEHPBG@Z ENDP			; CRegKey::ExportToIni
EXTRN	??1CStdioFile@@UAE@XZ:NEAR			; CStdioFile::~CStdioFile
;	COMDAT ??1CStdioFileEx@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CStdioFileEx@@UAE@XZ PROC NEAR			; CStdioFileEx::~CStdioFileEx, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CStdioFile@@UAE@XZ			; CStdioFile::~CStdioFile
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CStdioFileEx@@UAE@XZ ENDP				; CStdioFileEx::~CStdioFileEx
_TEXT	ENDS
PUBLIC	?ExportValueToIni@CRegKey@@IBEHKAAVCStdioFile@@@Z ; CRegKey::ExportValueToIni
EXTRN	?Format@CString@@QAAXPBGZZ:NEAR			; CString::Format
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74338 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
	ORG $+1
$SG74344 DB	'[', 00H, '%', 00H, 's', 00H, ']', 00H, 0aH, 00H, 00H, 00H
$SG74357 DB	'%', 00H, 's', 00H, '\', 00H, '%', 00H, 's', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74771	DD	019930520H
	DD	05H
	DD	FLAT:$T74773
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74773	DD	0ffffffffH
	DD	FLAT:$L74765
	DD	00H
	DD	FLAT:$L74766
	DD	00H
	DD	FLAT:$L74767
	DD	02H
	DD	FLAT:$L74768
	DD	03H
	DD	FLAT:$L74769
xdata$x	ENDS
;	COMDAT ?ExportKeyToIni@CRegKey@@IBEHPBGAAVCStdioFile@@@Z
_TEXT	SEGMENT
$T74762 = -76
$T74763 = -80
$T74764 = -84
__$EHRec$ = -12
_pszKey$ = 8
_file$ = 12
_this$ = -88
_reg$ = -28
_bSectionHasValues$74341 = -52
_sSection$74343 = -60
_nIndex$74345 = -56
_aSubKeys$74349 = -48
_nKey$74351 = -64
_sName$74355 = -72
_sKeyName$74356 = -68
?ExportKeyToIni@CRegKey@@IBEHPBGAAVCStdioFile@@@Z PROC NEAR ; CRegKey::ExportKeyToIni, COMDAT

; 346  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74772
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 76					; 0000004cH
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-88]
	mov	ecx, 19					; 00000013H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L74333:

; 347  : 	ASSERT (pszKey && *pszKey);

	cmp	DWORD PTR _pszKey$[ebp], 0
	je	SHORT $L74337
	mov	eax, DWORD PTR _pszKey$[ebp]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax]
	test	ecx, ecx
	jne	SHORT $L74336
$L74337:
	push	347					; 0000015bH
	push	OFFSET FLAT:$SG74338
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74336
	int	3
$L74336:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74333

; 348  : 	CRegKey reg;

	lea	ecx, DWORD PTR _reg$[ebp]
	call	??0CRegKey@@QAE@XZ			; CRegKey::CRegKey
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 349  : 	
; 350  : 	if (reg.Open(m_hKey, pszKey) == ERROR_SUCCESS)

	mov	eax, DWORD PTR _pszKey$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open
	test	eax, eax
	jne	$L74340

; 352  : 		BOOL bSectionHasValues = reg.HasValues();

	lea	ecx, DWORD PTR _reg$[ebp]
	call	?HasValues@CRegKey@@QBEHXZ		; CRegKey::HasValues
	mov	DWORD PTR _bSectionHasValues$74341[ebp], eax

; 353  : 		
; 354  : 		if (bSectionHasValues)

	cmp	DWORD PTR _bSectionHasValues$74341[ebp], 0
	je	SHORT $L74342

; 356  : 			// write out section heading
; 357  : 			CString sSection;

	lea	ecx, DWORD PTR _sSection$74343[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 358  : 			sSection.Format(_T("[%s]\n"), pszKey);

	mov	eax, DWORD PTR _pszKey$[ebp]
	push	eax
	push	OFFSET FLAT:$SG74344
	lea	ecx, DWORD PTR _sSection$74343[ebp]
	push	ecx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 12					; 0000000cH

; 359  : 			file.WriteString(sSection);

	lea	ecx, DWORD PTR _sSection$74343[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	mov	edx, DWORD PTR _file$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _file$[ebp]
	call	DWORD PTR [eax+92]
	cmp	esi, esp
	call	__chkesp

; 360  : 
; 361  : 			// write out values
; 362  : 			int nIndex = 0;

	mov	DWORD PTR _nIndex$74345[ebp], 0
$L74347:

; 363  : 			
; 364  : 			while (reg.ExportValueToIni(nIndex, file))

	mov	ecx, DWORD PTR _file$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nIndex$74345[ebp]
	push	edx
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?ExportValueToIni@CRegKey@@IBEHKAAVCStdioFile@@@Z ; CRegKey::ExportValueToIni
	test	eax, eax
	je	SHORT $L74348

; 365  : 				nIndex++;

	mov	eax, DWORD PTR _nIndex$74345[ebp]
	add	eax, 1
	mov	DWORD PTR _nIndex$74345[ebp], eax
	jmp	SHORT $L74347
$L74348:

; 366  : 		}

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sSection$74343[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74342:

; 367  : 		
; 368  : 		// write out subkeys
; 369  : 		CStringArray aSubKeys;

	lea	ecx, DWORD PTR _aSubKeys$74349[ebp]
	call	??0CStringArray@@QAE@XZ			; CStringArray::CStringArray
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 370  : 		
; 371  : 		if (reg.GetSubkeyNames(aSubKeys))

	lea	ecx, DWORD PTR _aSubKeys$74349[ebp]
	push	ecx
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?GetSubkeyNames@CRegKey@@QBEHAAVCStringArray@@@Z ; CRegKey::GetSubkeyNames
	test	eax, eax
	je	$L74354

; 373  : 			for (int nKey = 0; nKey < aSubKeys.GetSize(); nKey++)

	mov	DWORD PTR _nKey$74351[ebp], 0
	jmp	SHORT $L74352
$L74353:
	mov	edx, DWORD PTR _nKey$74351[ebp]
	add	edx, 1
	mov	DWORD PTR _nKey$74351[ebp], edx
$L74352:
	lea	ecx, DWORD PTR _aSubKeys$74349[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	cmp	DWORD PTR _nKey$74351[ebp], eax
	jge	$L74354

; 375  : 				CString sName = aSubKeys[nKey];

	mov	eax, DWORD PTR _nKey$74351[ebp]
	push	eax
	lea	ecx, DWORD PTR _aSubKeys$74349[ebp]
	call	??ACStringArray@@QAEAAVCString@@H@Z	; CStringArray::operator[]
	push	eax
	lea	ecx, DWORD PTR _sName$74355[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3

; 376  : 				
; 377  : 				// process subkey
; 378  : 				CString sKeyName;

	lea	ecx, DWORD PTR _sKeyName$74356[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 4

; 379  : 				sKeyName.Format(_T("%s\\%s"), pszKey, sName);

	mov	ecx, DWORD PTR _sName$74355[ebp]
	push	ecx
	mov	edx, DWORD PTR _pszKey$[ebp]
	push	edx
	push	OFFSET FLAT:$SG74357
	lea	eax, DWORD PTR _sKeyName$74356[ebp]
	push	eax
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 16					; 00000010H

; 380  : 				
; 381  : 				if (!ExportKeyToIni(sKeyName, file))

	mov	ecx, DWORD PTR _file$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sKeyName$74356[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ExportKeyToIni@CRegKey@@IBEHPBGAAVCStdioFile@@@Z ; CRegKey::ExportKeyToIni
	test	eax, eax
	jne	SHORT $L74358

; 382  : 					return FALSE;

	mov	DWORD PTR $T74762[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR _sKeyName$74356[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sName$74355[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _aSubKeys$74349[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74762[ebp]
	jmp	SHORT $L74332
$L74358:

; 383  : 			}

	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR _sKeyName$74356[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sName$74355[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	$L74353
$L74354:

; 385  : 		
; 386  : 		return TRUE;

	mov	DWORD PTR $T74763[ebp], 1
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _aSubKeys$74349[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74763[ebp]
	jmp	SHORT $L74332
$L74340:

; 388  : 	
; 389  : 	// else
; 390  : 	return FALSE;

	mov	DWORD PTR $T74764[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74764[ebp]
$L74332:

; 391  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 88					; 00000058H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74765:
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	ret	0
$L74766:
	lea	ecx, DWORD PTR _sSection$74343[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74767:
	lea	ecx, DWORD PTR _aSubKeys$74349[ebp]
	call	??1CStringArray@@UAE@XZ			; CStringArray::~CStringArray
	ret	0
$L74768:
	lea	ecx, DWORD PTR _sName$74355[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74769:
	lea	ecx, DWORD PTR _sKeyName$74356[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74772:
	mov	eax, OFFSET FLAT:$T74771
	jmp	___CxxFrameHandler
text$x	ENDS
?ExportKeyToIni@CRegKey@@IBEHPBGAAVCStdioFile@@@Z ENDP	; CRegKey::ExportKeyToIni
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74380 DB	'%', 00H, 's', 00H, ' ', 00H, '=', 00H, ' ', 00H, '%', 00H
	DB	'l', 00H, 'u', 00H, 0aH, 00H, 00H, 00H
$SG74383 DB	'%', 00H, 's', 00H, ' ', 00H, '=', 00H, ' ', 00H, '"', 00H
	DB	'%', 00H, 's', 00H, '"', 00H, 0aH, 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74783	DD	019930520H
	DD	02H
	DD	FLAT:$T74785
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74785	DD	0ffffffffH
	DD	FLAT:$L74780
	DD	00H
	DD	FLAT:$L74781
xdata$x	ENDS
;	COMDAT ?ExportValueToIni@CRegKey@@IBEHKAAVCStdioFile@@@Z
_TEXT	SEGMENT
$T74779 = -1064
__$EHRec$ = -12
_nIndex$ = 8
_file$ = 12
_this$ = -1068
_szName$ = -1040
_nNameLen$ = -1048
_dwType$ = -16
_lResult$ = -1044
_sValueLine$74373 = -1052
_dwValue$74379 = -1056
_sValue$74382 = -1060
?ExportValueToIni@CRegKey@@IBEHKAAVCStdioFile@@@Z PROC NEAR ; CRegKey::ExportValueToIni, COMDAT

; 394  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74784
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 1060				; 00000424H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-1072]
	mov	ecx, 265				; 00000109H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 395  : 	TCHAR szName[512];
; 396  : 	DWORD nNameLen = sizeof(szName)/sizeof(TCHAR);

	mov	DWORD PTR _nNameLen$[ebp], 512		; 00000200H

; 397  : 	DWORD dwType;
; 398  : 	
; 399  : 	LONG lResult = ::RegEnumValue(m_hKey, nIndex, szName, &nNameLen,
; 400  : 		NULL, &dwType, NULL, NULL);

	mov	esi, esp
	push	0
	push	0
	lea	eax, DWORD PTR _dwType$[ebp]
	push	eax
	push	0
	lea	ecx, DWORD PTR _nNameLen$[ebp]
	push	ecx
	lea	edx, DWORD PTR _szName$[ebp]
	push	edx
	mov	eax, DWORD PTR _nIndex$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR __imp__RegEnumValueW@32
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lResult$[ebp], eax

; 401  : 	
; 402  : 	// we have a valid key name
; 403  : 	if (lResult == ERROR_SUCCESS)

	cmp	DWORD PTR _lResult$[ebp], 0
	jne	$L74372

; 405  : 		CString sValueLine;

	lea	ecx, DWORD PTR _sValueLine$74373[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 408  : 		{

	mov	eax, DWORD PTR _dwType$[ebp]
	mov	DWORD PTR -1072+[ebp], eax
	cmp	DWORD PTR -1072+[ebp], 1
	je	SHORT $L74381
	cmp	DWORD PTR -1072+[ebp], 4
	je	SHORT $L74378
	jmp	$L74375
$L74378:

; 411  : 				DWORD dwValue;
; 412  : 				Read(szName, dwValue);

	lea	ecx, DWORD PTR _dwValue$74379[ebp]
	push	ecx
	lea	edx, DWORD PTR _szName$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Read@CRegKey@@QBEJPBGAAK@Z		; CRegKey::Read

; 413  : 				sValueLine.Format(_T("%s = %lu\n"), szName, dwValue);

	mov	eax, DWORD PTR _dwValue$74379[ebp]
	push	eax
	lea	ecx, DWORD PTR _szName$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG74380
	lea	edx, DWORD PTR _sValueLine$74373[ebp]
	push	edx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 16					; 00000010H

; 415  : 			break;

	jmp	SHORT $L74375
$L74381:

; 419  : 				CString sValue;

	lea	ecx, DWORD PTR _sValue$74382[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 420  : 				Read(szName, sValue);

	lea	eax, DWORD PTR _sValue$74382[ebp]
	push	eax
	lea	ecx, DWORD PTR _szName$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Read@CRegKey@@QBEJPBGAAVCString@@@Z	; CRegKey::Read

; 421  : 				sValueLine.Format(_T("%s = \"%s\"\n"), szName, sValue);

	mov	edx, DWORD PTR _sValue$74382[ebp]
	push	edx
	lea	eax, DWORD PTR _szName$[ebp]
	push	eax
	push	OFFSET FLAT:$SG74383
	lea	ecx, DWORD PTR _sValueLine$74373[ebp]
	push	ecx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 16					; 00000010H

; 422  : 			}

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sValue$74382[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74375:

; 424  : 		
; 425  : 		if (!sValueLine.IsEmpty())

	lea	ecx, DWORD PTR _sValueLine$74373[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L74384

; 426  : 			file.WriteString(sValueLine);

	lea	ecx, DWORD PTR _sValueLine$74373[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	mov	edx, DWORD PTR _file$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _file$[ebp]
	call	DWORD PTR [eax+92]
	cmp	esi, esp
	call	__chkesp
$L74384:

; 427  : 		
; 428  : 		return TRUE;

	mov	DWORD PTR $T74779[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sValueLine$74373[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74779[ebp]
	jmp	SHORT $L74366
$L74372:

; 430  : 	
; 431  : 	return FALSE;

	xor	eax, eax
$L74366:

; 432  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 1072				; 00000430H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74780:
	lea	ecx, DWORD PTR _sValueLine$74373[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74781:
	lea	ecx, DWORD PTR _sValue$74382[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74784:
	mov	eax, OFFSET FLAT:$T74783
	jmp	___CxxFrameHandler
text$x	ENDS
?ExportValueToIni@CRegKey@@IBEHKAAVCStdioFile@@@Z ENDP	; CRegKey::ExportValueToIni
PUBLIC	?ImportFromIni@CRegKey@@QAEHPBG@Z		; CRegKey::ImportFromIni
PUBLIC	?ImportSectionFromIni@CRegKey@@IAEHABVCString@@AAVCStdioFile@@AAV2@@Z ; CRegKey::ImportSectionFromIni
;	COMDAT data
; File D:\_CODE\Shared\Regkey.cpp
data	SEGMENT
$SG74394 DB	'D:\_CODE\Shared\Regkey.cpp', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74797	DD	019930520H
	DD	03H
	DD	FLAT:$T74799
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74799	DD	0ffffffffH
	DD	FLAT:$L74793
	DD	00H
	DD	FLAT:$L74794
	DD	01H
	DD	FLAT:$L74795
xdata$x	ENDS
;	COMDAT ?ImportFromIni@CRegKey@@QAEHPBG@Z
_TEXT	SEGMENT
$T74791 = -12360
$T74792 = -12364
__$EHRec$ = -12
_szIniPath$ = 8
_this$ = -12368
_file$ = -12356
_sSection$ = -24
_sNextSection$ = -16
_bRes$ = -20
?ImportFromIni@CRegKey@@QAEHPBG@Z PROC NEAR		; CRegKey::ImportFromIni, COMDAT

; 435  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74798
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	mov	eax, 12356				; 00003044H
	call	__chkstk
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-12368]
	mov	ecx, 3089				; 00000c11H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L74390:

; 436  : 	ASSERT (m_hKey);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74393
	push	436					; 000001b4H
	push	OFFSET FLAT:$SG74394
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74393
	int	3
$L74393:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74390

; 437  : 	
; 438  : 	if (!m_hKey)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	jne	SHORT $L74395

; 439  : 		return FALSE;

	xor	eax, eax
	jmp	$L74389
$L74395:

; 440  : 	
; 441  : 	CStdioFileEx file;

	lea	ecx, DWORD PTR _file$[ebp]
	call	??0CStdioFileEx@@QAE@XZ			; CStdioFileEx::CStdioFileEx
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 442  : 	
; 443  : 	if (!file.Open(szIniPath, CFile::modeRead))

	push	0
	push	2
	push	0
	mov	eax, DWORD PTR _szIniPath$[ebp]
	push	eax
	lea	ecx, DWORD PTR _file$[ebp]
	call	?Open@CStdioFileEx@@UAEHPBGIW4SFE_SAVEAS@@PAVCFileException@@@Z ; CStdioFileEx::Open
	test	eax, eax
	jne	SHORT $L74397

; 444  : 		return FALSE;

	mov	DWORD PTR $T74791[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	mov	eax, DWORD PTR $T74791[ebp]
	jmp	$L74389
$L74397:

; 445  : 
; 446  : 	CString sSection, sNextSection;

	lea	ecx, DWORD PTR _sSection$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sNextSection$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 447  : 	BOOL bRes = TRUE;

	mov	DWORD PTR _bRes$[ebp], 1
$L74402:

; 451  : 		// detect invalid ini files
; 452  : 		bRes &= ImportSectionFromIni(sSection, file, sNextSection);

	lea	ecx, DWORD PTR _sNextSection$[ebp]
	push	ecx
	lea	edx, DWORD PTR _file$[ebp]
	push	edx
	lea	eax, DWORD PTR _sSection$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ImportSectionFromIni@CRegKey@@IAEHABVCString@@AAVCStdioFile@@AAV2@@Z ; CRegKey::ImportSectionFromIni
	mov	ecx, DWORD PTR _bRes$[ebp]
	and	ecx, eax
	mov	DWORD PTR _bRes$[ebp], ecx

; 453  : 		sSection = sNextSection;

	lea	edx, DWORD PTR _sNextSection$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sSection$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=

; 455  : 	while(bRes && !sSection.IsEmpty());

	cmp	DWORD PTR _bRes$[ebp], 0
	je	SHORT $L74405
	lea	ecx, DWORD PTR _sSection$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L74402
$L74405:

; 456  : 	
; 457  : 	return bRes;

	mov	eax, DWORD PTR _bRes$[ebp]
	mov	DWORD PTR $T74792[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sNextSection$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sSection$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	mov	eax, DWORD PTR $T74792[ebp]
$L74389:

; 458  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12368				; 00003050H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74793:
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	ret	0
$L74794:
	lea	ecx, DWORD PTR _sSection$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74795:
	lea	ecx, DWORD PTR _sNextSection$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74798:
	mov	eax, OFFSET FLAT:$T74797
	jmp	___CxxFrameHandler
text$x	ENDS
?ImportFromIni@CRegKey@@QAEHPBG@Z ENDP			; CRegKey::ImportFromIni
EXTRN	__imp___wtoi:NEAR
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	??ACString@@QBEGH@Z:NEAR			; CString::operator[]
EXTRN	?Mid@CString@@QBE?AV1@HH@Z:NEAR			; CString::Mid
EXTRN	?Mid@CString@@QBE?AV1@H@Z:NEAR			; CString::Mid
EXTRN	?Left@CString@@QBE?AV1@H@Z:NEAR			; CString::Left
EXTRN	?TrimRight@CString@@QAEXXZ:NEAR			; CString::TrimRight
EXTRN	?TrimLeft@CString@@QAEXXZ:NEAR			; CString::TrimLeft
EXTRN	?Find@CString@@QBEHG@Z:NEAR			; CString::Find
;	COMDAT xdata$x
; File D:\_CODE\Shared\Regkey.cpp
xdata$x	SEGMENT
$T74823	DD	019930520H
	DD	08H
	DD	FLAT:$T74825
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74825	DD	0ffffffffH
	DD	FLAT:$L74814
	DD	00H
	DD	FLAT:$L74815
	DD	01H
	DD	FLAT:$L74816
	DD	01H
	DD	FLAT:$L74817
	DD	03H
	DD	FLAT:$L74818
	DD	04H
	DD	FLAT:$L74819
	DD	04H
	DD	FLAT:$L74820
	DD	04H
	DD	FLAT:$L74821
xdata$x	ENDS
;	COMDAT ?ImportSectionFromIni@CRegKey@@IAEHABVCString@@AAVCStdioFile@@AAV2@@Z
_TEXT	SEGMENT
$T74805 = -60
$T74806 = -64
$T74807 = -68
$T74808 = -72
$T74809 = -76
$T74810 = -80
$T74811 = -84
$T74812 = -88
$T74813 = -92
__$EHRec$ = -12
_sSection$ = 8
_file$ = 12
_sNextSection$ = 16
_this$ = -96
_bRoot$ = -16
_reg$ = -32
_sLine$ = -36
_sName$74431 = -48
_sValue$74432 = -40
_nEquals$74433 = -44
_bString$74438 = -52
_nChar$74441 = -56
?ImportSectionFromIni@CRegKey@@IAEHABVCString@@AAVCStdioFile@@AAV2@@Z PROC NEAR ; CRegKey::ImportSectionFromIni, COMDAT

; 461  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74824
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 120				; 00000078H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-132]
	mov	ecx, 30					; 0000001eH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 462  : 	sNextSection.Empty();

	mov	ecx, DWORD PTR _sNextSection$[ebp]
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 463  : 	BOOL bRoot = (sSection.IsEmpty());

	mov	ecx, DWORD PTR _sSection$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	mov	DWORD PTR _bRoot$[ebp], eax

; 464  : 	CRegKey reg;

	lea	ecx, DWORD PTR _reg$[ebp]
	call	??0CRegKey@@QAE@XZ			; CRegKey::CRegKey
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 465  : 	
; 466  : 	// open the reg key if not root
; 467  : 	if (!bRoot)

	cmp	DWORD PTR _bRoot$[ebp], 0
	jne	SHORT $L74416

; 469  : 		//TRACE (" CRegKey::ImportSectionFromIni(%s)\n", sSection);
; 470  : 		if (reg.Open(m_hKey, sSection) != ERROR_SUCCESS)

	mov	ecx, DWORD PTR _sSection$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open
	test	eax, eax
	je	SHORT $L74416

; 471  : 			return FALSE;

	mov	DWORD PTR $T74805[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74805[ebp]
	jmp	$L74412
$L74416:

; 473  : 	
; 474  : 	CString sLine;

	lea	ecx, DWORD PTR _sLine$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
$L74420:

; 475  : 	
; 476  : 	while (file.ReadString(sLine))

	mov	esi, esp
	lea	edx, DWORD PTR _sLine$[ebp]
	push	edx
	mov	eax, DWORD PTR _file$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _file$[ebp]
	call	DWORD PTR [edx+96]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	$L74421

; 478  : 		sLine.TrimLeft();

	lea	ecx, DWORD PTR _sLine$[ebp]
	call	?TrimLeft@CString@@QAEXXZ		; CString::TrimLeft

; 479  : 		sLine.TrimRight();

	lea	ecx, DWORD PTR _sLine$[ebp]
	call	?TrimRight@CString@@QAEXXZ		; CString::TrimRight

; 480  : 		
; 481  : 		if (sLine.IsEmpty())

	lea	ecx, DWORD PTR _sLine$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L74422

; 482  : 			continue;

	jmp	SHORT $L74420
$L74422:

; 483  : 
; 484  : 		else if (sLine[0] == '[')

	push	0
	lea	ecx, DWORD PTR _sLine$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	and	eax, 65535				; 0000ffffH
	cmp	eax, 91					; 0000005bH
	jne	$L74424

; 486  : 			// check for end tag
; 487  : 			if (sLine[sLine.GetLength() - 1] == ']')

	lea	ecx, DWORD PTR _sLine$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	sub	eax, 1
	push	eax
	lea	ecx, DWORD PTR _sLine$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	and	eax, 65535				; 0000ffffH
	cmp	eax, 93					; 0000005dH
	jne	SHORT $L74425

; 489  : 				sNextSection = sLine.Mid(1, sLine.GetLength() - 2);

	lea	ecx, DWORD PTR _sLine$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	sub	eax, 2
	push	eax
	push	1
	lea	eax, DWORD PTR $T74806[ebp]
	push	eax
	lea	ecx, DWORD PTR _sLine$[ebp]
	call	?Mid@CString@@QBE?AV1@HH@Z		; CString::Mid
	mov	DWORD PTR -100+[ebp], eax
	mov	ecx, DWORD PTR -100+[ebp]
	mov	DWORD PTR -104+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	edx, DWORD PTR -104+[ebp]
	push	edx
	mov	ecx, DWORD PTR _sNextSection$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T74806[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 490  : 				return TRUE;

	mov	DWORD PTR $T74807[ebp], 1
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sLine$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74807[ebp]
	jmp	$L74412
$L74425:

; 492  : 			
; 493  : 			// else
; 494  : 			return FALSE;

	mov	DWORD PTR $T74808[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sLine$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74808[ebp]
	jmp	$L74412
$L74424:

; 496  : 		else if (!bRoot) // can't have values in the root

	cmp	DWORD PTR _bRoot$[ebp], 0
	jne	$L74430

; 498  : 			CString sName, sValue;

	lea	ecx, DWORD PTR _sName$74431[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 4

; 499  : 			int nEquals = sLine.Find('=');

	push	61					; 0000003dH
	lea	ecx, DWORD PTR _sLine$[ebp]
	call	?Find@CString@@QBEHG@Z			; CString::Find
	mov	DWORD PTR _nEquals$74433[ebp], eax

; 500  : 			
; 501  : 			if (nEquals > 0)

	cmp	DWORD PTR _nEquals$74433[ebp], 0
	jle	$L74456

; 503  : 				sName = sLine.Left(nEquals);

	mov	eax, DWORD PTR _nEquals$74433[ebp]
	push	eax
	lea	ecx, DWORD PTR $T74809[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sLine$[ebp]
	call	?Left@CString@@QBE?AV1@H@Z		; CString::Left
	mov	DWORD PTR -108+[ebp], eax
	mov	edx, DWORD PTR -108+[ebp]
	mov	DWORD PTR -112+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	mov	eax, DWORD PTR -112+[ebp]
	push	eax
	lea	ecx, DWORD PTR _sName$74431[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR $T74809[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 504  : 				sValue = sLine.Mid(nEquals + 1);

	mov	ecx, DWORD PTR _nEquals$74433[ebp]
	add	ecx, 1
	push	ecx
	lea	edx, DWORD PTR $T74810[ebp]
	push	edx
	lea	ecx, DWORD PTR _sLine$[ebp]
	call	?Mid@CString@@QBE?AV1@H@Z		; CString::Mid
	mov	DWORD PTR -116+[ebp], eax
	mov	eax, DWORD PTR -116+[ebp]
	mov	DWORD PTR -120+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	mov	ecx, DWORD PTR -120+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR $T74810[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 505  : 				
; 506  : 				sName.TrimLeft();

	lea	ecx, DWORD PTR _sName$74431[ebp]
	call	?TrimLeft@CString@@QAEXXZ		; CString::TrimLeft

; 507  : 				sName.TrimRight();

	lea	ecx, DWORD PTR _sName$74431[ebp]
	call	?TrimRight@CString@@QAEXXZ		; CString::TrimRight

; 508  : 				sValue.TrimLeft();

	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	?TrimLeft@CString@@QAEXXZ		; CString::TrimLeft

; 509  : 				sValue.TrimRight();

	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	?TrimRight@CString@@QAEXXZ		; CString::TrimRight

; 510  : 				
; 511  : 				// name must not be empty
; 512  : 				if (!sName.IsEmpty())

	lea	ecx, DWORD PTR _sName$74431[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	$L74456

; 514  : 					// if value contains only digits optionally beginning 
; 515  : 					// with a minus sign then its a DWORD else a string
; 516  : 					BOOL bString = FALSE;

	mov	DWORD PTR _bString$74438[ebp], 0

; 517  : 
; 518  : 					if (sValue.IsEmpty())

	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L74439

; 519  : 						bString = TRUE;

	mov	DWORD PTR _bString$74438[ebp], 1

; 520  : 					else

	jmp	SHORT $L74444
$L74439:

; 522  : 						// traverse the chars
; 523  : 						for (int nChar = 0; nChar < sValue.GetLength() && !bString; nChar++)

	mov	DWORD PTR _nChar$74441[ebp], 0
	jmp	SHORT $L74442
$L74443:
	mov	edx, DWORD PTR _nChar$74441[ebp]
	add	edx, 1
	mov	DWORD PTR _nChar$74441[ebp], edx
$L74442:
	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	cmp	DWORD PTR _nChar$74441[ebp], eax
	jge	SHORT $L74444
	cmp	DWORD PTR _bString$74438[ebp], 0
	jne	SHORT $L74444

; 526  : 							{

	mov	eax, DWORD PTR _nChar$74441[ebp]
	push	eax
	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	and	eax, 65535				; 0000ffffH
	mov	DWORD PTR -124+[ebp], eax
	cmp	DWORD PTR -124+[ebp], 45		; 0000002dH
	je	SHORT $L74449
	cmp	DWORD PTR -124+[ebp], 47		; 0000002fH
	jle	SHORT $L74451
	cmp	DWORD PTR -124+[ebp], 57		; 00000039H
	jle	SHORT $L74450
	jmp	SHORT $L74451
$L74449:

; 527  : 							case '-':
; 528  : 								bString = (nChar > 0);

	xor	ecx, ecx
	cmp	DWORD PTR _nChar$74441[ebp], 0
	setg	cl
	mov	DWORD PTR _bString$74438[ebp], ecx

; 529  : 								break;

	jmp	SHORT $L74446
$L74450:

; 530  : 
; 531  : 							case '0':
; 532  : 							case '1':
; 533  : 							case '2':
; 534  : 							case '3':
; 535  : 							case '4':
; 536  : 							case '5':
; 537  : 							case '6':
; 538  : 							case '7':
; 539  : 							case '8':
; 540  : 							case '9':
; 541  : 								break; // okay

	jmp	SHORT $L74446
$L74451:

; 542  : 
; 543  : 							default:
; 544  : 								bString = TRUE; // everything else

	mov	DWORD PTR _bString$74438[ebp], 1
$L74446:

; 548  : 						}

	jmp	SHORT $L74443
$L74444:

; 550  : 
; 551  : 					if (bString)

	cmp	DWORD PTR _bString$74438[ebp], 0
	je	$L74452

; 553  : 						// remove possible leading and trailing quotes
; 554  : 						if (sValue.GetLength() && sValue[0] == '\"' && sValue[sValue.GetLength() - 1] == '\"')

	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	$L74453
	push	0
	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	and	eax, 65535				; 0000ffffH
	cmp	eax, 34					; 00000022H
	jne	SHORT $L74453
	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	sub	eax, 1
	push	eax
	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	and	eax, 65535				; 0000ffffH
	cmp	eax, 34					; 00000022H
	jne	SHORT $L74453

; 555  : 							reg.Write(sName, sValue.Mid(1, sValue.GetLength() - 2));

	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	sub	eax, 2
	push	eax
	push	1
	lea	edx, DWORD PTR $T74811[ebp]
	push	edx
	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	?Mid@CString@@QBE?AV1@HH@Z		; CString::Mid
	mov	DWORD PTR -128+[ebp], eax
	mov	eax, DWORD PTR -128+[ebp]
	mov	DWORD PTR -132+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	mov	ecx, DWORD PTR -132+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sName$74431[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Write@CRegKey@@QAEJPBG0@Z		; CRegKey::Write
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR $T74811[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 556  : 						else

	jmp	SHORT $L74455
$L74453:

; 557  : 							reg.Write(sName, sValue);

	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sName$74431[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Write@CRegKey@@QAEJPBG0@Z		; CRegKey::Write
$L74455:

; 559  : 					else // DWORD

	jmp	SHORT $L74456
$L74452:

; 560  : 						reg.Write(sName, (DWORD)_ttoi(sValue));

	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp___wtoi
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	push	eax
	lea	ecx, DWORD PTR _sName$74431[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Write@CRegKey@@QAEJPBGK@Z		; CRegKey::Write
$L74456:

; 563  : 		}

	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sName$74431[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 564  : 		else // invalid file

	jmp	SHORT $L74458
$L74430:

; 565  : 			return FALSE;

	mov	DWORD PTR $T74812[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sLine$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74812[ebp]
	jmp	SHORT $L74412
$L74458:

; 566  : 	}

	jmp	$L74420
$L74421:

; 567  : 	
; 568  : 	return TRUE;

	mov	DWORD PTR $T74813[ebp], 1
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sLine$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74813[ebp]
$L74412:

; 569  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 132				; 00000084H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74814:
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	ret	0
$L74815:
	lea	ecx, DWORD PTR _sLine$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74816:
	lea	ecx, DWORD PTR $T74806[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74817:
	lea	ecx, DWORD PTR _sName$74431[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74818:
	lea	ecx, DWORD PTR _sValue$74432[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74819:
	lea	ecx, DWORD PTR $T74809[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74820:
	lea	ecx, DWORD PTR $T74810[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74821:
	lea	ecx, DWORD PTR $T74811[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74824:
	mov	eax, OFFSET FLAT:$T74823
	jmp	___CxxFrameHandler
text$x	ENDS
?ImportSectionFromIni@CRegKey@@IAEHABVCString@@AAVCStdioFile@@AAV2@@Z ENDP ; CRegKey::ImportSectionFromIni
PUBLIC	?CopyKey@CRegKey@@SAHPAUHKEY__@@ABVCString@@01@Z ; CRegKey::CopyKey
EXTRN	?CopyRegistryKey@@YAHPAUHKEY__@@ABVCString@@01@Z:NEAR ; CopyRegistryKey
;	COMDAT ?CopyKey@CRegKey@@SAHPAUHKEY__@@ABVCString@@01@Z
_TEXT	SEGMENT
_hkRootFrom$ = 8
_sFromPath$ = 12
_hkRootTo$ = 16
_sToPath$ = 20
?CopyKey@CRegKey@@SAHPAUHKEY__@@ABVCString@@01@Z PROC NEAR ; CRegKey::CopyKey, COMDAT

; 572  : {

	push	ebp
	mov	ebp, esp

; 573  : 	return CopyRegistryKey(hkRootFrom, sFromPath, hkRootTo, sToPath);

	mov	eax, DWORD PTR _sToPath$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hkRootTo$[ebp]
	push	ecx
	mov	edx, DWORD PTR _sFromPath$[ebp]
	push	edx
	mov	eax, DWORD PTR _hkRootFrom$[ebp]
	push	eax
	call	?CopyRegistryKey@@YAHPAUHKEY__@@ABVCString@@01@Z ; CopyRegistryKey
	add	esp, 16					; 00000010H

; 574  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?CopyKey@CRegKey@@SAHPAUHKEY__@@ABVCString@@01@Z ENDP	; CRegKey::CopyKey
_TEXT	ENDS
END

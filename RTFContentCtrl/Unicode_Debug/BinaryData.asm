	TITLE	D:\_CODE\Shared\BinaryData.cpp
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
;	COMDAT ??0CBinaryData@@QAE@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CBinaryData@@QAE@PBEH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CBinaryData@@QAE@ABVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CBinaryData@@QAE@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetBuffer@CBinaryData@@QAEPAEH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReleaseBuffer@CBinaryData@@QAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Get@CBinaryData@@QBEPBEAAH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Get@CBinaryData@@QBEPBEXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Empty@CBinaryData@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsEmpty@CBinaryData@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Get@CBinaryData@@QBEHAAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??8CBinaryData@@QBE_NABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??9CBinaryData@@QBE_NABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetLength@CBinaryData@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetByteLength@CBinaryData@@IBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Set@CBinaryData@@IAEXPBEH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??BCBinaryData@@IBEPBEXZ
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
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	??0CBinaryData@@QAE@PBG@Z			; CBinaryData::CBinaryData
PUBLIC	?Set@CBinaryData@@IAEXPBEH@Z			; CBinaryData::Set
EXTRN	__chkesp:NEAR
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
EXTRN	__imp__lstrlenW@4:NEAR
;	COMDAT xdata$x
; File D:\_CODE\Shared\BinaryData.cpp
xdata$x	SEGMENT
$T73309	DD	019930520H
	DD	01H
	DD	FLAT:$T73313
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73313	DD	0ffffffffH
	DD	FLAT:$L73305
xdata$x	ENDS
;	COMDAT ??0CBinaryData@@QAE@PBG@Z
_TEXT	SEGMENT
_szData$ = 8
_this$ = -16
__$EHRec$ = -12
??0CBinaryData@@QAE@PBG@Z PROC NEAR			; CBinaryData::CBinaryData, COMDAT

; 13   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73311
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 14   :     if (szData)

	cmp	DWORD PTR _szData$[ebp], 0
	je	SHORT $L73103

; 15   : 		Set((const unsigned char*)szData, lstrlen(szData) * sizeof(TCHAR));

	mov	esi, esp
	mov	eax, DWORD PTR _szData$[ebp]
	push	eax
	call	DWORD PTR __imp__lstrlenW@4
	cmp	esi, esp
	call	__chkesp
	shl	eax, 1
	push	eax
	mov	ecx, DWORD PTR _szData$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Set@CBinaryData@@IAEXPBEH@Z		; CBinaryData::Set
$L73103:

; 16   : }

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
$L73305:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73311:
	mov	eax, OFFSET FLAT:$T73309
	jmp	___CxxFrameHandler
text$x	ENDS
??0CBinaryData@@QAE@PBG@Z ENDP				; CBinaryData::CBinaryData
PUBLIC	??0CBinaryData@@QAE@PBEH@Z			; CBinaryData::CBinaryData
;	COMDAT xdata$x
; File D:\_CODE\Shared\BinaryData.cpp
xdata$x	SEGMENT
$T73321	DD	019930520H
	DD	01H
	DD	FLAT:$T73323
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73323	DD	0ffffffffH
	DD	FLAT:$L73319
xdata$x	ENDS
;	COMDAT ??0CBinaryData@@QAE@PBEH@Z
_TEXT	SEGMENT
_pData$ = 8
_nByteLength$ = 12
_this$ = -16
__$EHRec$ = -12
??0CBinaryData@@QAE@PBEH@Z PROC NEAR			; CBinaryData::CBinaryData, COMDAT

; 19   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73322
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 20   :     Set(pData, nByteLength);

	mov	eax, DWORD PTR _nByteLength$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pData$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Set@CBinaryData@@IAEXPBEH@Z		; CBinaryData::Set

; 21   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73319:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73322:
	mov	eax, OFFSET FLAT:$T73321
	jmp	___CxxFrameHandler
text$x	ENDS
??0CBinaryData@@QAE@PBEH@Z ENDP				; CBinaryData::CBinaryData
PUBLIC	??0CBinaryData@@QAE@ABVCString@@@Z		; CBinaryData::CBinaryData
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
;	COMDAT xdata$x
; File D:\_CODE\Shared\BinaryData.cpp
xdata$x	SEGMENT
$T73330	DD	019930520H
	DD	01H
	DD	FLAT:$T73332
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73332	DD	0ffffffffH
	DD	FLAT:$L73328
xdata$x	ENDS
;	COMDAT ??0CBinaryData@@QAE@ABVCString@@@Z
_TEXT	SEGMENT
_sContent$ = 8
_this$ = -16
__$EHRec$ = -12
??0CBinaryData@@QAE@ABVCString@@@Z PROC NEAR		; CBinaryData::CBinaryData, COMDAT

; 24   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73331
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 25   :     Set((const unsigned char*)(LPCTSTR)sContent, sContent.GetLength() * sizeof(TCHAR));

	mov	ecx, DWORD PTR _sContent$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	shl	eax, 1
	push	eax
	mov	ecx, DWORD PTR _sContent$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Set@CBinaryData@@IAEXPBEH@Z		; CBinaryData::Set

; 26   : }

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
$L73328:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73331:
	mov	eax, OFFSET FLAT:$T73330
	jmp	___CxxFrameHandler
text$x	ENDS
??0CBinaryData@@QAE@ABVCString@@@Z ENDP			; CBinaryData::CBinaryData
PUBLIC	??0CBinaryData@@QAE@ABV0@@Z			; CBinaryData::CBinaryData
PUBLIC	?GetLength@CBinaryData@@QBEHXZ			; CBinaryData::GetLength
PUBLIC	??BCBinaryData@@IBEPBEXZ			; CBinaryData::operator unsigned char const *
;	COMDAT xdata$x
; File D:\_CODE\Shared\BinaryData.cpp
xdata$x	SEGMENT
$T73339	DD	019930520H
	DD	01H
	DD	FLAT:$T73341
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73341	DD	0ffffffffH
	DD	FLAT:$L73337
xdata$x	ENDS
;	COMDAT ??0CBinaryData@@QAE@ABV0@@Z
_TEXT	SEGMENT
_data$ = 8
_this$ = -16
__$EHRec$ = -12
??0CBinaryData@@QAE@ABV0@@Z PROC NEAR			; CBinaryData::CBinaryData, COMDAT

; 29   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73340
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 30   :     Set(data, data.GetLength());

	mov	ecx, DWORD PTR _data$[ebp]
	call	?GetLength@CBinaryData@@QBEHXZ		; CBinaryData::GetLength
	push	eax
	mov	ecx, DWORD PTR _data$[ebp]
	call	??BCBinaryData@@IBEPBEXZ		; CBinaryData::operator unsigned char const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Set@CBinaryData@@IAEXPBEH@Z		; CBinaryData::Set

; 31   : }

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
$L73337:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73340:
	mov	eax, OFFSET FLAT:$T73339
	jmp	___CxxFrameHandler
text$x	ENDS
??0CBinaryData@@QAE@ABV0@@Z ENDP			; CBinaryData::CBinaryData
PUBLIC	?GetBuffer@CBinaryData@@QAEPAEH@Z		; CBinaryData::GetBuffer
EXTRN	_memset:NEAR
EXTRN	?GetBuffer@CString@@QAEPAGH@Z:NEAR		; CString::GetBuffer
;	COMDAT ?GetBuffer@CBinaryData@@QAEPAEH@Z
_TEXT	SEGMENT
_nByteLength$ = 8
_this$ = -12
_nFixedLen$ = -8
_pBuf$ = -4
?GetBuffer@CBinaryData@@QAEPAEH@Z PROC NEAR		; CBinaryData::GetBuffer, COMDAT

; 34   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 35   : #ifdef _UNICODE
; 36   : 	
; 37   :     // need to copy into wide buffer
; 38   :     // must make the length multiple of 2
; 39   :     int nFixedLen = nByteLength + (nByteLength % 2);

	mov	eax, DWORD PTR _nByteLength$[ebp]
	and	eax, -2147483647			; 80000001H
	jns	SHORT $L73346
	dec	eax
	or	eax, -2					; fffffffeH
	inc	eax
$L73346:
	mov	ecx, DWORD PTR _nByteLength$[ebp]
	add	ecx, eax
	mov	DWORD PTR _nFixedLen$[ebp], ecx

; 40   :     LPWSTR pBuf = CString::GetBuffer(nFixedLen / 2);

	mov	eax, DWORD PTR _nFixedLen$[ebp]
	cdq
	sub	eax, edx
	sar	eax, 1
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	mov	DWORD PTR _pBuf$[ebp], eax

; 41   : 	
; 42   :     ZeroMemory(pBuf, nFixedLen);

	mov	edx, DWORD PTR _nFixedLen$[ebp]
	push	edx
	push	0
	mov	eax, DWORD PTR _pBuf$[ebp]
	push	eax
	call	_memset
	add	esp, 12					; 0000000cH

; 43   : 	
; 44   : #else
; 45   : 	
; 46   :     // simple copy
; 47   :     LPSTR pBuf = CString::GetBuffer(nByteLength);
; 48   :     ZeroMemory(pBuf, nByteLength);
; 49   : 	
; 50   : #endif
; 51   : 	
; 52   :     return (unsigned char*)pBuf;

	mov	eax, DWORD PTR _pBuf$[ebp]

; 53   : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetBuffer@CBinaryData@@QAEPAEH@Z ENDP			; CBinaryData::GetBuffer
_TEXT	ENDS
PUBLIC	?ReleaseBuffer@CBinaryData@@QAEXH@Z		; CBinaryData::ReleaseBuffer
EXTRN	?ReleaseBuffer@CString@@QAEXH@Z:NEAR		; CString::ReleaseBuffer
;	COMDAT ?ReleaseBuffer@CBinaryData@@QAEXH@Z
_TEXT	SEGMENT
_nByteLength$ = 8
_this$ = -8
_nFixedLen$ = -4
?ReleaseBuffer@CBinaryData@@QAEXH@Z PROC NEAR		; CBinaryData::ReleaseBuffer, COMDAT

; 56   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 57   : #ifdef _UNICODE
; 58   : 
; 59   :     // must make the length multiple of 2
; 60   :     int nFixedLen = nByteLength + (nByteLength % 2);

	mov	eax, DWORD PTR _nByteLength$[ebp]
	and	eax, -2147483647			; 80000001H
	jns	SHORT $L73349
	dec	eax
	or	eax, -2					; fffffffeH
	inc	eax
$L73349:
	mov	ecx, DWORD PTR _nByteLength$[ebp]
	add	ecx, eax
	mov	DWORD PTR _nFixedLen$[ebp], ecx

; 61   : 	CString::ReleaseBuffer(nFixedLen / 2);

	mov	eax, DWORD PTR _nFixedLen$[ebp]
	cdq
	sub	eax, edx
	sar	eax, 1
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 62   : 
; 63   : #else
; 64   : 
; 65   :     // simple release
; 66   : 	CString::ReleaseBuffer(nByteLength);
; 67   : 
; 68   : #endif
; 69   : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ReleaseBuffer@CBinaryData@@QAEXH@Z ENDP		; CBinaryData::ReleaseBuffer
_TEXT	ENDS
PUBLIC	?Get@CBinaryData@@QBEPBEAAH@Z			; CBinaryData::Get
PUBLIC	?Get@CBinaryData@@QBEPBEXZ			; CBinaryData::Get
;	COMDAT ?Get@CBinaryData@@QBEPBEAAH@Z
_TEXT	SEGMENT
_nByteLength$ = 8
_this$ = -4
?Get@CBinaryData@@QBEPBEAAH@Z PROC NEAR			; CBinaryData::Get, COMDAT

; 73   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 74   :     nByteLength = GetLength();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetLength@CBinaryData@@QBEHXZ		; CBinaryData::GetLength
	mov	ecx, DWORD PTR _nByteLength$[ebp]
	mov	DWORD PTR [ecx], eax

; 75   :     return Get();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Get@CBinaryData@@QBEPBEXZ		; CBinaryData::Get

; 76   : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Get@CBinaryData@@QBEPBEAAH@Z ENDP			; CBinaryData::Get
_TEXT	ENDS
;	COMDAT ?Get@CBinaryData@@QBEPBEXZ
_TEXT	SEGMENT
_this$ = -4
?Get@CBinaryData@@QBEPBEXZ PROC NEAR			; CBinaryData::Get, COMDAT

; 79   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 80   :     return *this;

	mov	ecx, DWORD PTR _this$[ebp]
	call	??BCBinaryData@@IBEPBEXZ		; CBinaryData::operator unsigned char const *

; 81   : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Get@CBinaryData@@QBEPBEXZ ENDP				; CBinaryData::Get
_TEXT	ENDS
PUBLIC	?Empty@CBinaryData@@QAEXXZ			; CBinaryData::Empty
EXTRN	?Empty@CString@@QAEXXZ:NEAR			; CString::Empty
;	COMDAT ?Empty@CBinaryData@@QAEXXZ
_TEXT	SEGMENT
_this$ = -4
?Empty@CBinaryData@@QAEXXZ PROC NEAR			; CBinaryData::Empty, COMDAT

; 84   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 85   :     CString::Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 86   : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Empty@CBinaryData@@QAEXXZ ENDP				; CBinaryData::Empty
_TEXT	ENDS
PUBLIC	?IsEmpty@CBinaryData@@QBEHXZ			; CBinaryData::IsEmpty
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
;	COMDAT ?IsEmpty@CBinaryData@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?IsEmpty@CBinaryData@@QBEHXZ PROC NEAR			; CBinaryData::IsEmpty, COMDAT

; 89   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 90   :     return CString::IsEmpty();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty

; 91   : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsEmpty@CBinaryData@@QBEHXZ ENDP			; CBinaryData::IsEmpty
_TEXT	ENDS
PUBLIC	?Get@CBinaryData@@QBEHAAVCString@@@Z		; CBinaryData::Get
PUBLIC	?GetByteLength@CBinaryData@@IBEHXZ		; CBinaryData::GetByteLength
EXTRN	??0CString@@QAE@PBGH@Z:NEAR			; CString::CString
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
;	COMDAT xdata$x
; File D:\_CODE\Shared\BinaryData.cpp
xdata$x	SEGMENT
$T73363	DD	019930520H
	DD	01H
	DD	FLAT:$T73365
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73365	DD	0ffffffffH
	DD	FLAT:$L73361
xdata$x	ENDS
;	COMDAT ?Get@CBinaryData@@QBEHAAVCString@@@Z
_TEXT	SEGMENT
_sContent$ = 8
_this$ = -24
_nStrLength$ = -16
$T73360 = -20
__$EHRec$ = -12
?Get@CBinaryData@@QBEHAAVCString@@@Z PROC NEAR		; CBinaryData::Get, COMDAT

; 94   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73364
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
	mov	DWORD PTR _this$[ebp], ecx

; 95   :     int nStrLength = GetByteLength() / sizeof(TCHAR);

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetByteLength@CBinaryData@@IBEHXZ	; CBinaryData::GetByteLength
	shr	eax, 1
	mov	DWORD PTR _nStrLength$[ebp], eax

; 96   :     sContent = CString((LPCTSTR)Get(), nStrLength);

	mov	eax, DWORD PTR _nStrLength$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Get@CBinaryData@@QBEPBEXZ		; CBinaryData::Get
	push	eax
	lea	ecx, DWORD PTR $T73360[ebp]
	call	??0CString@@QAE@PBGH@Z			; CString::CString
	mov	DWORD PTR -28+[ebp], eax
	mov	ecx, DWORD PTR -28+[ebp]
	mov	DWORD PTR -32+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	edx, DWORD PTR -32+[ebp]
	push	edx
	mov	ecx, DWORD PTR _sContent$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T73360[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 97   : 
; 98   : 	return GetLength();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetLength@CBinaryData@@QBEHXZ		; CBinaryData::GetLength

; 99   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73361:
	lea	ecx, DWORD PTR $T73360[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73364:
	mov	eax, OFFSET FLAT:$T73363
	jmp	___CxxFrameHandler
text$x	ENDS
?Get@CBinaryData@@QBEHAAVCString@@@Z ENDP		; CBinaryData::Get
PUBLIC	??8CBinaryData@@QBE_NABV0@@Z			; CBinaryData::operator==
EXTRN	_memcmp:NEAR
;	COMDAT ??8CBinaryData@@QBE_NABV0@@Z
_TEXT	SEGMENT
_data$ = 8
_this$ = -4
??8CBinaryData@@QBE_NABV0@@Z PROC NEAR			; CBinaryData::operator==, COMDAT

; 102  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 103  :     return (this == &data || (GetLength() == data.GetLength() && memcmp(Get(), data.Get(), GetLength()) == 0));

	mov	eax, DWORD PTR _this$[ebp]
	cmp	eax, DWORD PTR _data$[ebp]
	je	SHORT $L73371
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetLength@CBinaryData@@QBEHXZ		; CBinaryData::GetLength
	mov	esi, eax
	mov	ecx, DWORD PTR _data$[ebp]
	call	?GetLength@CBinaryData@@QBEHXZ		; CBinaryData::GetLength
	cmp	esi, eax
	jne	SHORT $L73370
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetLength@CBinaryData@@QBEHXZ		; CBinaryData::GetLength
	push	eax
	mov	ecx, DWORD PTR _data$[ebp]
	call	?Get@CBinaryData@@QBEPBEXZ		; CBinaryData::Get
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Get@CBinaryData@@QBEPBEXZ		; CBinaryData::Get
	push	eax
	call	_memcmp
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	SHORT $L73371
$L73370:
	mov	DWORD PTR -8+[ebp], 0
	jmp	SHORT $L73372
$L73371:
	mov	DWORD PTR -8+[ebp], 1
$L73372:
	mov	al, BYTE PTR -8+[ebp]

; 104  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??8CBinaryData@@QBE_NABV0@@Z ENDP			; CBinaryData::operator==
_TEXT	ENDS
PUBLIC	??9CBinaryData@@QBE_NABV0@@Z			; CBinaryData::operator!=
;	COMDAT ??9CBinaryData@@QBE_NABV0@@Z
_TEXT	SEGMENT
_data$ = 8
_this$ = -4
??9CBinaryData@@QBE_NABV0@@Z PROC NEAR			; CBinaryData::operator!=, COMDAT

; 107  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 108  :     return !(*this == data);

	mov	eax, DWORD PTR _data$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??8CBinaryData@@QBE_NABV0@@Z		; CBinaryData::operator==
	and	eax, 255				; 000000ffH
	neg	eax
	sbb	eax, eax
	inc	eax

; 109  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??9CBinaryData@@QBE_NABV0@@Z ENDP			; CBinaryData::operator!=
_TEXT	ENDS
;	COMDAT ?GetLength@CBinaryData@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?GetLength@CBinaryData@@QBEHXZ PROC NEAR		; CBinaryData::GetLength, COMDAT

; 112  : { 

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 113  : 	return GetByteLength(); 

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetByteLength@CBinaryData@@IBEHXZ	; CBinaryData::GetByteLength

; 114  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetLength@CBinaryData@@QBEHXZ ENDP			; CBinaryData::GetLength
_TEXT	ENDS
;	COMDAT ?GetByteLength@CBinaryData@@IBEHXZ
_TEXT	SEGMENT
_this$ = -8
_nByteLength$ = -4
?GetByteLength@CBinaryData@@IBEHXZ PROC NEAR		; CBinaryData::GetByteLength, COMDAT

; 117  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 118  : 	int nByteLength = CString::GetLength() * sizeof(TCHAR);

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	shl	eax, 1
	mov	DWORD PTR _nByteLength$[ebp], eax

; 119  : 	return nByteLength;

	mov	eax, DWORD PTR _nByteLength$[ebp]

; 120  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetByteLength@CBinaryData@@IBEHXZ ENDP			; CBinaryData::GetByteLength
_TEXT	ENDS
EXTRN	_memcpy:NEAR
;	COMDAT ?Set@CBinaryData@@IAEXPBEH@Z
_TEXT	SEGMENT
_pData$ = 8
_nByteLength$ = 12
_this$ = -8
_pBuf$ = -4
?Set@CBinaryData@@IAEXPBEH@Z PROC NEAR			; CBinaryData::Set, COMDAT

; 123  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 124  : 	unsigned char* pBuf = CBinaryData::GetBuffer(nByteLength);

	mov	eax, DWORD PTR _nByteLength$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetBuffer@CBinaryData@@QAEPAEH@Z	; CBinaryData::GetBuffer
	mov	DWORD PTR _pBuf$[ebp], eax

; 125  : 	CopyMemory(pBuf, pData, nByteLength);

	mov	ecx, DWORD PTR _nByteLength$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pData$[ebp]
	push	edx
	mov	eax, DWORD PTR _pBuf$[ebp]
	push	eax
	call	_memcpy
	add	esp, 12					; 0000000cH

; 126  : 	CBinaryData::ReleaseBuffer(nByteLength);

	mov	ecx, DWORD PTR _nByteLength$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ReleaseBuffer@CBinaryData@@QAEXH@Z	; CBinaryData::ReleaseBuffer

; 127  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Set@CBinaryData@@IAEXPBEH@Z ENDP			; CBinaryData::Set
_TEXT	ENDS
;	COMDAT ??BCBinaryData@@IBEPBEXZ
_TEXT	SEGMENT
_this$ = -4
??BCBinaryData@@IBEPBEXZ PROC NEAR			; CBinaryData::operator unsigned char const *, COMDAT

; 130  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 131  : 	return (const unsigned char*)(LPCTSTR)(*this);

	mov	ecx, DWORD PTR _this$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *

; 132  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??BCBinaryData@@IBEPBEXZ ENDP				; CBinaryData::operator unsigned char const *
_TEXT	ENDS
END

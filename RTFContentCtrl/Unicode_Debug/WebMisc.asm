	TITLE	D:\_code\Shared\WebMisc.cpp
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
;	COMDAT ?IsOnline@WebMisc@@YAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DeleteCacheEntry@WebMisc@@YAHPBG@Z
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
PUBLIC	?IsOnline@WebMisc@@YAHXZ			; WebMisc::IsOnline
EXTRN	__chkesp:NEAR
EXTRN	__imp__InternetQueryOptionW@16:NEAR
;	COMDAT ?IsOnline@WebMisc@@YAHXZ
_TEXT	SEGMENT
_dwState$ = -4
_dwSize$ = -8
?IsOnline@WebMisc@@YAHXZ PROC NEAR			; WebMisc::IsOnline, COMDAT

; 10   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 11   :     DWORD dwState = 0; 

	mov	DWORD PTR _dwState$[ebp], 0

; 12   :     DWORD dwSize = sizeof(DWORD);

	mov	DWORD PTR _dwSize$[ebp], 4

; 13   : 	
; 14   :     return InternetQueryOption(NULL, INTERNET_OPTION_CONNECTED_STATE, &dwState, &dwSize) && 
; 15   : 		(dwState & INTERNET_STATE_CONNECTED);

	mov	esi, esp
	lea	eax, DWORD PTR _dwSize$[ebp]
	push	eax
	lea	ecx, DWORD PTR _dwState$[ebp]
	push	ecx
	push	50					; 00000032H
	push	0
	call	DWORD PTR __imp__InternetQueryOptionW@16
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L74660
	mov	edx, DWORD PTR _dwState$[ebp]
	and	edx, 1
	test	edx, edx
	je	SHORT $L74660
	mov	DWORD PTR -12+[ebp], 1
	jmp	SHORT $L74661
$L74660:
	mov	DWORD PTR -12+[ebp], 0
$L74661:
	mov	eax, DWORD PTR -12+[ebp]

; 16   : }

	pop	esi
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsOnline@WebMisc@@YAHXZ ENDP				; WebMisc::IsOnline
_TEXT	ENDS
PUBLIC	?DeleteCacheEntry@WebMisc@@YAHPBG@Z		; WebMisc::DeleteCacheEntry
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
EXTRN	?WideToMultiByte@Misc@@YAPADPBGI@Z:NEAR		; Misc::WideToMultiByte
EXTRN	__imp__DeleteUrlCacheEntry@4:NEAR
;	COMDAT ?DeleteCacheEntry@WebMisc@@YAHPBG@Z
_TEXT	SEGMENT
_szURI$ = 8
_bSuccess$ = -4
_szAnsiPath$ = -8
$T74665 = -12
?DeleteCacheEntry@WebMisc@@YAHPBG@Z PROC NEAR		; WebMisc::DeleteCacheEntry, COMDAT

; 19   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 20   : 	BOOL bSuccess = FALSE;

	mov	DWORD PTR _bSuccess$[ebp], 0

; 21   : 
; 22   : #if _MSC_VER >= 1400
; 23   : 	bSuccess = DeleteUrlCacheEntry(szURI);
; 24   : #elif _UNICODE
; 25   : 	LPSTR szAnsiPath = Misc::WideToMultiByte(szURI);

	push	0
	mov	eax, DWORD PTR _szURI$[ebp]
	push	eax
	call	?WideToMultiByte@Misc@@YAPADPBGI@Z	; Misc::WideToMultiByte
	add	esp, 8
	mov	DWORD PTR _szAnsiPath$[ebp], eax

; 26   : 	bSuccess = DeleteUrlCacheEntry(szAnsiPath);

	mov	esi, esp
	mov	ecx, DWORD PTR _szAnsiPath$[ebp]
	push	ecx
	call	DWORD PTR __imp__DeleteUrlCacheEntry@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bSuccess$[ebp], eax

; 27   : 	delete [] szAnsiPath;

	mov	edx, DWORD PTR _szAnsiPath$[ebp]
	mov	DWORD PTR $T74665[ebp], edx
	mov	eax, DWORD PTR $T74665[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 28   : #else
; 29   : 	bSuccess = DeleteUrlCacheEntry(szURI);
; 30   : #endif
; 31   : 
; 32   : 	return bSuccess;

	mov	eax, DWORD PTR _bSuccess$[ebp]

; 33   : }

	pop	esi
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?DeleteCacheEntry@WebMisc@@YAHPBG@Z ENDP		; WebMisc::DeleteCacheEntry
_TEXT	ENDS
END

	TITLE	D:\_CODE\3rdParty\Compression.cpp
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
;	COMDAT ?Compress@Compression@@YAHPBEHAAPAEAAH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Decompress@Compression@@YAHPBEHAAPAEAAH@Z
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
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\3rdParty\Compression.cpp', 00H
_DATA	ENDS
PUBLIC	?Compress@Compression@@YAHPBEHAAPAEAAH@Z	; Compression::Compress
PUBLIC	__real@8@3fff8020c49ba5e35000
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
EXTRN	_compress@16:NEAR
EXTRN	__ftol:NEAR
EXTRN	__chkesp:NEAR
EXTRN	__fltused:NEAR
EXTRN	??2@YAPAXIPBDH@Z:NEAR				; operator new
;	COMDAT __real@8@3fff8020c49ba5e35000
; File D:\_CODE\3rdParty\Compression.cpp
CONST	SEGMENT
__real@8@3fff8020c49ba5e35000 DQ 03ff004189374bc6ar ; 1.001
CONST	ENDS
;	COMDAT ?Compress@Compression@@YAHPBEHAAPAEAAH@Z
_TEXT	SEGMENT
_pContentSrc$ = 8
_nLenSrc$ = 12
_pContentDest$ = 16
_nLenDest$ = 20
_lDest$ = -4
_nRet$ = -8
$T73395 = -12
$T73396 = -16
?Compress@Compression@@YAHPBEHAAPAEAAH@Z PROC NEAR	; Compression::Compress, COMDAT

; 22   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 23   : 	ULONG lDest = (int)(nLenSrc * 1.001) + 12; // zlib requirements

	fild	DWORD PTR _nLenSrc$[ebp]
	fmul	QWORD PTR __real@8@3fff8020c49ba5e35000
	call	__ftol
	add	eax, 12					; 0000000cH
	mov	DWORD PTR _lDest$[ebp], eax

; 24   : 	pContentDest = new unsigned char[lDest + 4]; // extra DWORD for storing the source length

	push	24					; 00000018H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _lDest$[ebp]
	add	eax, 4
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T73395[ebp], eax
	mov	ecx, DWORD PTR _pContentDest$[ebp]
	mov	edx, DWORD PTR $T73395[ebp]
	mov	DWORD PTR [ecx], edx

; 25   : 
; 26   : 	// leave the first DWORD free
; 27   : 	int nRet = compress(pContentDest + 4, &lDest, pContentSrc, nLenSrc);

	mov	eax, DWORD PTR _nLenSrc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pContentSrc$[ebp]
	push	ecx
	lea	edx, DWORD PTR _lDest$[ebp]
	push	edx
	mov	eax, DWORD PTR _pContentDest$[ebp]
	mov	ecx, DWORD PTR [eax]
	add	ecx, 4
	push	ecx
	call	_compress@16
	mov	DWORD PTR _nRet$[ebp], eax

; 28   : 
; 29   : 	if (nRet != Z_OK)

	cmp	DWORD PTR _nRet$[ebp], 0
	je	SHORT $L73266

; 31   : 		// cleanup
; 32   : 		delete [] pContentDest;

	mov	edx, DWORD PTR _pContentDest$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR $T73396[ebp], eax
	mov	ecx, DWORD PTR $T73396[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 33   : 		nLenDest = 0;

	mov	edx, DWORD PTR _nLenDest$[ebp]
	mov	DWORD PTR [edx], 0

; 35   : 	else

	jmp	SHORT $L73268
$L73266:

; 37   : 		nLenDest = lDest + 4;

	mov	eax, DWORD PTR _lDest$[ebp]
	add	eax, 4
	mov	ecx, DWORD PTR _nLenDest$[ebp]
	mov	DWORD PTR [ecx], eax

; 38   : 
; 39   : 		// encode the source length into the first 4 bytes
; 40   : 		((DWORD*)pContentDest)[0] = nLenSrc;

	mov	edx, DWORD PTR _pContentDest$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _nLenSrc$[ebp]
	mov	DWORD PTR [eax], ecx
$L73268:

; 42   : 
; 43   : 	return (nRet == Z_OK);

	xor	eax, eax
	cmp	DWORD PTR _nRet$[ebp], 0
	sete	al

; 44   : }

	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Compress@Compression@@YAHPBEHAAPAEAAH@Z ENDP		; Compression::Compress
_TEXT	ENDS
PUBLIC	?Decompress@Compression@@YAHPBEHAAPAEAAH@Z	; Compression::Decompress
EXTRN	_uncompress@16:NEAR
;	COMDAT ?Decompress@Compression@@YAHPBEHAAPAEAAH@Z
_TEXT	SEGMENT
_pContentSrc$ = 8
_nLenSrc$ = 12
_pContentDest$ = 16
_nLenDest$ = 20
_lDest$ = -4
_nRet$ = -8
$T73404 = -12
$T73405 = -16
?Decompress@Compression@@YAHPBEHAAPAEAAH@Z PROC NEAR	; Compression::Decompress, COMDAT

; 48   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 49   : 	// get the source length from the first 4 bytes
; 50   : 	ULONG lDest = ((DWORD*)pContentSrc)[0];

	mov	eax, DWORD PTR _pContentSrc$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _lDest$[ebp], ecx

; 51   : 
; 52   : 	// sanity check
; 53   : 	if (lDest < (ULONG)nLenSrc || lDest > (ULONG)(1000 * nLenSrc))

	mov	edx, DWORD PTR _lDest$[ebp]
	cmp	edx, DWORD PTR _nLenSrc$[ebp]
	jb	SHORT $L73281
	mov	eax, DWORD PTR _nLenSrc$[ebp]
	imul	eax, 1000				; 000003e8H
	cmp	DWORD PTR _lDest$[ebp], eax
	jbe	SHORT $L73280
$L73281:

; 54   : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L73275
$L73280:

; 55   : 
; 56   : 	pContentDest = new unsigned char[lDest];

	push	56					; 00000038H
	push	OFFSET FLAT:_THIS_FILE
	mov	ecx, DWORD PTR _lDest$[ebp]
	push	ecx
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T73404[ebp], eax
	mov	edx, DWORD PTR _pContentDest$[ebp]
	mov	eax, DWORD PTR $T73404[ebp]
	mov	DWORD PTR [edx], eax

; 57   : 
; 58   : 	int nRet = uncompress(pContentDest, &lDest, pContentSrc + 4, nLenSrc - 4);

	mov	ecx, DWORD PTR _nLenSrc$[ebp]
	sub	ecx, 4
	push	ecx
	mov	edx, DWORD PTR _pContentSrc$[ebp]
	add	edx, 4
	push	edx
	lea	eax, DWORD PTR _lDest$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pContentDest$[ebp]
	mov	edx, DWORD PTR [ecx]
	push	edx
	call	_uncompress@16
	mov	DWORD PTR _nRet$[ebp], eax

; 59   : 
; 60   : 	if (nRet != Z_OK)

	cmp	DWORD PTR _nRet$[ebp], 0
	je	SHORT $L73285

; 62   : 		delete [] pContentDest;

	mov	eax, DWORD PTR _pContentDest$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR $T73405[ebp], ecx
	mov	edx, DWORD PTR $T73405[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 63   : 		nLenDest = 0;

	mov	eax, DWORD PTR _nLenDest$[ebp]
	mov	DWORD PTR [eax], 0

; 65   : 	else

	jmp	SHORT $L73287
$L73285:

; 66   : 		nLenDest = lDest;

	mov	ecx, DWORD PTR _nLenDest$[ebp]
	mov	edx, DWORD PTR _lDest$[ebp]
	mov	DWORD PTR [ecx], edx
$L73287:

; 67   : 
; 68   : 	return (nRet == Z_OK);

	xor	eax, eax
	cmp	DWORD PTR _nRet$[ebp], 0
	sete	al
$L73275:

; 69   : }

	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Decompress@Compression@@YAHPBEHAAPAEAAH@Z ENDP		; Compression::Decompress
_TEXT	ENDS
END

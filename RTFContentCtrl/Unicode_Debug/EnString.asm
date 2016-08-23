	TITLE	D:\_CODE\Shared\EnString.cpp
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
;	COMDAT ??0CEnString@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CEnString@@QAA@PBGZZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CEnString@@QAE@IPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4CEnString@@QAEAAV0@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CEnString@@QAE@IH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CEnString@@QAE@IPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CEnString@@QAE@ABVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CEnString@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CompareIgnoreString@CEnString@@IAEXPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Format@CEnString@@QAAXPBGZZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Format@CEnString@@QAAXIZZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadStringW@CEnString@@KAHIPAUHWND__@@AAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadStringW@CEnString@@QAEHIPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Translate@CEnString@@QAEHPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DoNotTranslate@CEnString@@QBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DoNotTranslateString@CEnString@@SAXABVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?TranslateStrings@CEnString@@SAHAAVCStringArray@@PAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?TranslateString@CEnString@@SAHAAVCString@@PAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetCharacterCount@CEnString@@QBEHGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CEnString@@QAE@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FormatDC@CEnString@@QAE?AVCSize@@PAVCDC@@HH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Draw@CEnString@@QAE?AVCSize@@PAVCDC@@PAUtagRECT@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FormatDCEx@CEnString@@IAE?AVCSize@@PAVCDC@@HH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetLocalizer@CEnString@@SAXPAVITransText@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Allocate@CEnString@@QAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Allocate@CEnString@@SAHAAVCString@@H@Z
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
PUBLIC	?s_pTT@CEnString@@1PAVITransText@@A		; CEnString::s_pTT
_BSS	SEGMENT
?s_pTT@CEnString@@1PAVITransText@@A DD 01H DUP (?)	; CEnString::s_pTT
_BSS	ENDS
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\Shared\EnString.cpp', 00H
_DATA	ENDS
PUBLIC	??0CEnString@@QAE@XZ				; CEnString::CEnString
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	__chkesp:NEAR
;	COMDAT ??0CEnString@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0CEnString@@QAE@XZ PROC NEAR				; CEnString::CEnString, COMDAT

; 24   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString

; 25   : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CEnString@@QAE@XZ ENDP				; CEnString::CEnString
_TEXT	ENDS
PUBLIC	??0CEnString@@QAA@PBGZZ				; CEnString::CEnString
PUBLIC	?TranslateString@CEnString@@SAHAAVCString@@PAUHWND__@@@Z ; CEnString::TranslateString
PUBLIC	?CompareIgnoreString@CEnString@@IAEXPBG@Z	; CEnString::CompareIgnoreString
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	?FormatV@CString@@QAEXPBGPAD@Z:NEAR		; CString::FormatV
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnString.cpp
xdata$x	SEGMENT
$T73595	DD	019930520H
	DD	02H
	DD	FLAT:$T73598
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73598	DD	0ffffffffH
	DD	FLAT:$L73590
	DD	00H
	DD	FLAT:$L73591
xdata$x	ENDS
;	COMDAT ??0CEnString@@QAA@PBGZZ
_TEXT	SEGMENT
_lpszFormat$ = 12
_this$ = 8
_strFormat$ = -16
_argList$73241 = -20
__$EHRec$ = -12
??0CEnString@@QAA@PBGZZ PROC NEAR			; CEnString::CEnString, COMDAT

; 28   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73596
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 29   : 	CString strFormat(lpszFormat);

	mov	eax, DWORD PTR _lpszFormat$[ebp]
	push	eax
	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 30   : 		
; 31   : 	if (!strFormat.IsEmpty())

	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L73240

; 33   : 		TranslateString(strFormat, NULL);

	push	0
	lea	ecx, DWORD PTR _strFormat$[ebp]
	push	ecx
	call	?TranslateString@CEnString@@SAHAAVCString@@PAUHWND__@@@Z ; CEnString::TranslateString
	add	esp, 8

; 34   : 
; 35   : 		va_list argList;
; 36   : 		va_start(argList, lpszFormat);

	lea	edx, DWORD PTR _lpszFormat$[ebp+4]
	mov	DWORD PTR _argList$73241[ebp], edx

; 37   : 		CString::FormatV(strFormat, argList);

	mov	eax, DWORD PTR _argList$73241[ebp]
	push	eax
	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FormatV@CString@@QAEXPBGPAD@Z		; CString::FormatV

; 38   : 		va_end(argList);

	mov	DWORD PTR _argList$73241[ebp], 0

; 39   : 
; 40   : 		CompareIgnoreString(strFormat);

	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?CompareIgnoreString@CEnString@@IAEXPBG@Z ; CEnString::CompareIgnoreString
$L73240:

; 42   : }

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73590:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73591:
	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73596:
	mov	eax, OFFSET FLAT:$T73595
	jmp	___CxxFrameHandler
text$x	ENDS
??0CEnString@@QAA@PBGZZ ENDP				; CEnString::CEnString
PUBLIC	??0CEnString@@QAE@IPBG@Z			; CEnString::CEnString
PUBLIC	??0CEnString@@QAE@ABVCString@@@Z		; CEnString::CEnString
PUBLIC	??1CEnString@@QAE@XZ				; CEnString::~CEnString
PUBLIC	?LoadStringW@CEnString@@KAHIPAUHWND__@@AAVCString@@@Z ; CEnString::LoadStringW
PUBLIC	??4CEnString@@QAEAAV0@ABV0@@Z			; CEnString::operator=
EXTRN	?Format@CString@@QAAXPBGZZ:NEAR			; CString::Format
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnString.cpp
xdata$x	SEGMENT
$T73609	DD	019930520H
	DD	03H
	DD	FLAT:$T73611
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73611	DD	0ffffffffH
	DD	FLAT:$L73605
	DD	00H
	DD	FLAT:$L73606
	DD	01H
	DD	FLAT:$L73607
xdata$x	ENDS
;	COMDAT ??0CEnString@@QAE@IPBG@Z
_TEXT	SEGMENT
_nFormatID$ = 8
_szText$ = 12
_this$ = -24
_strFormat$73252 = -16
$T73604 = -20
__$EHRec$ = -12
??0CEnString@@QAE@IPBG@Z PROC NEAR			; CEnString::CEnString, COMDAT

; 45   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73610
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 46   : 	if (nFormatID)

	cmp	DWORD PTR _nFormatID$[ebp], 0
	je	$L73251

; 48   : 		CString strFormat;

	lea	ecx, DWORD PTR _strFormat$73252[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 49   : 		
; 50   : 		if (LoadString(nFormatID, NULL, strFormat))

	lea	eax, DWORD PTR _strFormat$73252[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _nFormatID$[ebp]
	push	ecx
	call	?LoadStringW@CEnString@@KAHIPAUHWND__@@AAVCString@@@Z ; CEnString::LoadStringW
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	SHORT $L73253

; 52   : 			if (szText && *szText)

	cmp	DWORD PTR _szText$[ebp], 0
	je	SHORT $L73254
	mov	edx, DWORD PTR _szText$[ebp]
	xor	eax, eax
	mov	ax, WORD PTR [edx]
	test	eax, eax
	je	SHORT $L73254

; 53   : 				CString::Format(strFormat, szText);

	mov	ecx, DWORD PTR _szText$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _strFormat$73252[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	edx, DWORD PTR _this$[ebp]
	push	edx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 12					; 0000000cH

; 54   : 			else

	jmp	SHORT $L73255
$L73254:

; 55   : 				*this = strFormat;

	lea	eax, DWORD PTR _strFormat$73252[ebp]
	push	eax
	lea	ecx, DWORD PTR $T73604[ebp]
	call	??0CEnString@@QAE@ABVCString@@@Z	; CEnString::CEnString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T73604[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??4CEnString@@QAEAAV0@ABV0@@Z		; CEnString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T73604[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
$L73255:

; 56   : 			
; 57   : 			CompareIgnoreString(strFormat);

	lea	ecx, DWORD PTR _strFormat$73252[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?CompareIgnoreString@CEnString@@IAEXPBG@Z ; CEnString::CompareIgnoreString
$L73253:

; 59   : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _strFormat$73252[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L73251:

; 60   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73605:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73606:
	lea	ecx, DWORD PTR _strFormat$73252[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73607:
	lea	ecx, DWORD PTR $T73604[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	ret	0
$L73610:
	mov	eax, OFFSET FLAT:$T73609
	jmp	___CxxFrameHandler
text$x	ENDS
??0CEnString@@QAE@IPBG@Z ENDP				; CEnString::CEnString
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
;	COMDAT ??4CEnString@@QAEAAV0@ABV0@@Z
_TEXT	SEGMENT
___that$ = 8
_this$ = -4
??4CEnString@@QAEAAV0@ABV0@@Z PROC NEAR			; CEnString::operator=, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR ___that$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??4CEnString@@QAEAAV0@ABV0@@Z ENDP			; CEnString::operator=
_TEXT	ENDS
PUBLIC	??0CEnString@@QAE@IH@Z				; CEnString::CEnString
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnString.cpp
xdata$x	SEGMENT
$T73621	DD	019930520H
	DD	02H
	DD	FLAT:$T73623
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73623	DD	0ffffffffH
	DD	FLAT:$L73618
	DD	00H
	DD	FLAT:$L73619
xdata$x	ENDS
;	COMDAT ??0CEnString@@QAE@IH@Z
_TEXT	SEGMENT
_nFormatID$ = 8
_nData$ = 12
_this$ = -20
_strFormat$73266 = -16
__$EHRec$ = -12
??0CEnString@@QAE@IH@Z PROC NEAR			; CEnString::CEnString, COMDAT

; 63   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73622
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 64   : 	if (nFormatID)

	cmp	DWORD PTR _nFormatID$[ebp], 0
	je	SHORT $L73265

; 66   : 		CString strFormat;

	lea	ecx, DWORD PTR _strFormat$73266[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 67   : 		
; 68   : 		if (LoadString(nFormatID, NULL, strFormat))

	lea	eax, DWORD PTR _strFormat$73266[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _nFormatID$[ebp]
	push	ecx
	call	?LoadStringW@CEnString@@KAHIPAUHWND__@@AAVCString@@@Z ; CEnString::LoadStringW
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	SHORT $L73267

; 70   : 			CString::Format(strFormat, nData);

	mov	edx, DWORD PTR _nData$[ebp]
	push	edx
	lea	ecx, DWORD PTR _strFormat$73266[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 12					; 0000000cH

; 71   : 
; 72   : 			CompareIgnoreString(strFormat);

	lea	ecx, DWORD PTR _strFormat$73266[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?CompareIgnoreString@CEnString@@IAEXPBG@Z ; CEnString::CompareIgnoreString
$L73267:

; 74   : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _strFormat$73266[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L73265:

; 75   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73618:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73619:
	lea	ecx, DWORD PTR _strFormat$73266[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73622:
	mov	eax, OFFSET FLAT:$T73621
	jmp	___CxxFrameHandler
text$x	ENDS
??0CEnString@@QAE@IH@Z ENDP				; CEnString::CEnString
PUBLIC	??0CEnString@@QAE@IPAUHWND__@@@Z		; CEnString::CEnString
PUBLIC	?LoadStringW@CEnString@@QAEHIPAUHWND__@@@Z	; CEnString::LoadStringW
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnString.cpp
xdata$x	SEGMENT
$T73630	DD	019930520H
	DD	01H
	DD	FLAT:$T73632
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73632	DD	0ffffffffH
	DD	FLAT:$L73628
xdata$x	ENDS
;	COMDAT ??0CEnString@@QAE@IPAUHWND__@@@Z
_TEXT	SEGMENT
_nID$ = 8
_hwndRef$ = 12
_this$ = -16
__$EHRec$ = -12
??0CEnString@@QAE@IPAUHWND__@@@Z PROC NEAR		; CEnString::CEnString, COMDAT

; 78   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73631
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 79   : 	LoadString(nID, hwndRef);

	mov	eax, DWORD PTR _hwndRef$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nID$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadStringW@CEnString@@QAEHIPAUHWND__@@@Z ; CEnString::LoadStringW

; 80   : }

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
$L73628:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73631:
	mov	eax, OFFSET FLAT:$T73630
	jmp	___CxxFrameHandler
text$x	ENDS
??0CEnString@@QAE@IPAUHWND__@@@Z ENDP			; CEnString::CEnString
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
;	COMDAT ??0CEnString@@QAE@ABVCString@@@Z
_TEXT	SEGMENT
_str$ = 8
_this$ = -4
??0CEnString@@QAE@ABVCString@@@Z PROC NEAR		; CEnString::CEnString, COMDAT

; 83   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _str$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString

; 84   : 
; 85   : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0CEnString@@QAE@ABVCString@@@Z ENDP			; CEnString::CEnString
_TEXT	ENDS
;	COMDAT ??1CEnString@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CEnString@@QAE@XZ PROC NEAR				; CEnString::~CEnString, COMDAT

; 88   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 89   : 
; 90   : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CEnString@@QAE@XZ ENDP				; CEnString::~CEnString
_TEXT	ENDS
EXTRN	?Compare@CString@@QBEHPBG@Z:NEAR		; CString::Compare
;	COMDAT ?CompareIgnoreString@CEnString@@IAEXPBG@Z
_TEXT	SEGMENT
_szText$ = 8
_this$ = -4
?CompareIgnoreString@CEnString@@IAEXPBG@Z PROC NEAR	; CEnString::CompareIgnoreString, COMDAT

; 93   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 94   : 	// if there was a change, mark the formatted string as non-translatable
; 95   : 	if (s_pTT && Compare(szText) != 0)

	cmp	DWORD PTR ?s_pTT@CEnString@@1PAVITransText@@A, 0 ; CEnString::s_pTT
	je	SHORT $L73284
	mov	eax, DWORD PTR _szText$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Compare@CString@@QBEHPBG@Z		; CString::Compare
	test	eax, eax
	je	SHORT $L73284

; 96   : 		s_pTT->IgnoreString(*this);

	mov	ecx, DWORD PTR _this$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	mov	ecx, DWORD PTR ?s_pTT@CEnString@@1PAVITransText@@A ; CEnString::s_pTT
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR ?s_pTT@CEnString@@1PAVITransText@@A ; CEnString::s_pTT
	call	DWORD PTR [edx+56]
	cmp	esi, esp
	call	__chkesp
$L73284:

; 97   : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?CompareIgnoreString@CEnString@@IAEXPBG@Z ENDP		; CEnString::CompareIgnoreString
_TEXT	ENDS
PUBLIC	?Format@CEnString@@QAAXPBGZZ			; CEnString::Format
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnString.cpp
xdata$x	SEGMENT
$T73647	DD	019930520H
	DD	01H
	DD	FLAT:$T73649
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73649	DD	0ffffffffH
	DD	FLAT:$L73645
xdata$x	ENDS
;	COMDAT ?Format@CEnString@@QAAXPBGZZ
_TEXT	SEGMENT
_lpszFormat$ = 12
_this$ = 8
_strFormat$ = -16
_argList$73291 = -20
__$EHRec$ = -12
?Format@CEnString@@QAAXPBGZZ PROC NEAR			; CEnString::Format, COMDAT

; 100  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73648
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH

; 101  : 	CString strFormat(lpszFormat);

	mov	eax, DWORD PTR _lpszFormat$[ebp]
	push	eax
	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 102  : 		
; 103  : 	if (!strFormat.IsEmpty())

	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L73290

; 105  : 		TranslateString(strFormat, NULL);

	push	0
	lea	ecx, DWORD PTR _strFormat$[ebp]
	push	ecx
	call	?TranslateString@CEnString@@SAHAAVCString@@PAUHWND__@@@Z ; CEnString::TranslateString
	add	esp, 8

; 106  : 
; 107  : 		va_list argList;
; 108  : 		va_start(argList, lpszFormat);

	lea	edx, DWORD PTR _lpszFormat$[ebp+4]
	mov	DWORD PTR _argList$73291[ebp], edx

; 109  : 		CString::FormatV(strFormat, argList);

	mov	eax, DWORD PTR _argList$73291[ebp]
	push	eax
	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FormatV@CString@@QAEXPBGPAD@Z		; CString::FormatV

; 110  : 		va_end(argList);

	mov	DWORD PTR _argList$73291[ebp], 0

; 111  : 
; 112  : 		CompareIgnoreString(strFormat);

	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?CompareIgnoreString@CEnString@@IAEXPBG@Z ; CEnString::CompareIgnoreString
$L73290:

; 114  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73645:
	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73648:
	mov	eax, OFFSET FLAT:$T73647
	jmp	___CxxFrameHandler
text$x	ENDS
?Format@CEnString@@QAAXPBGZZ ENDP			; CEnString::Format
PUBLIC	?Format@CEnString@@QAAXIZZ			; CEnString::Format
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnString.cpp
xdata$x	SEGMENT
$T73656	DD	019930520H
	DD	01H
	DD	FLAT:$T73658
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73658	DD	0ffffffffH
	DD	FLAT:$L73654
xdata$x	ENDS
;	COMDAT ?Format@CEnString@@QAAXIZZ
_TEXT	SEGMENT
_nFormatID$ = 12
_this$ = 8
_strFormat$ = -16
_argList$73302 = -20
__$EHRec$ = -12
?Format@CEnString@@QAAXIZZ PROC NEAR			; CEnString::Format, COMDAT

; 117  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73657
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH

; 118  : 	CString strFormat;

	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 119  : 		
; 120  : 	if (LoadString(nFormatID, NULL, strFormat))

	lea	eax, DWORD PTR _strFormat$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _nFormatID$[ebp]
	push	ecx
	call	?LoadStringW@CEnString@@KAHIPAUHWND__@@AAVCString@@@Z ; CEnString::LoadStringW
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	SHORT $L73301

; 122  : 		va_list argList;
; 123  : 		va_start(argList, nFormatID);

	lea	edx, DWORD PTR _nFormatID$[ebp+4]
	mov	DWORD PTR _argList$73302[ebp], edx

; 124  : 		CString::FormatV(strFormat, argList);

	mov	eax, DWORD PTR _argList$73302[ebp]
	push	eax
	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FormatV@CString@@QAEXPBGPAD@Z		; CString::FormatV

; 125  : 		va_end(argList);

	mov	DWORD PTR _argList$73302[ebp], 0

; 126  : 
; 127  : 		CompareIgnoreString(strFormat);

	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?CompareIgnoreString@CEnString@@IAEXPBG@Z ; CEnString::CompareIgnoreString
$L73301:

; 129  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73654:
	lea	ecx, DWORD PTR _strFormat$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73657:
	mov	eax, OFFSET FLAT:$T73656
	jmp	___CxxFrameHandler
text$x	ENDS
?Format@CEnString@@QAAXIZZ ENDP				; CEnString::Format
EXTRN	?LoadStringW@CString@@QAEHI@Z:NEAR		; CString::LoadStringW
;	COMDAT ?LoadStringW@CEnString@@KAHIPAUHWND__@@AAVCString@@@Z
_TEXT	SEGMENT
_nID$ = 8
_hwndRef$ = 12
_sText$ = 16
?LoadStringW@CEnString@@KAHIPAUHWND__@@AAVCString@@@Z PROC NEAR ; CEnString::LoadStringW, COMDAT

; 133  : {

	push	ebp
	mov	ebp, esp

; 134  : 	if (sText.LoadString(nID))

	mov	eax, DWORD PTR _nID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sText$[ebp]
	call	?LoadStringW@CString@@QAEHI@Z		; CString::LoadStringW
	test	eax, eax
	je	SHORT $L73312

; 136  : 		TranslateString(sText, hwndRef);

	mov	ecx, DWORD PTR _hwndRef$[ebp]
	push	ecx
	mov	edx, DWORD PTR _sText$[ebp]
	push	edx
	call	?TranslateString@CEnString@@SAHAAVCString@@PAUHWND__@@@Z ; CEnString::TranslateString
	add	esp, 8

; 137  : 		return (!sText.IsEmpty());

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	neg	eax
	sbb	eax, eax
	inc	eax
	jmp	SHORT $L73311
$L73312:

; 139  : 
; 140  : 	return FALSE;

	xor	eax, eax
$L73311:

; 141  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?LoadStringW@CEnString@@KAHIPAUHWND__@@AAVCString@@@Z ENDP ; CEnString::LoadStringW
_TEXT	ENDS
;	COMDAT ?LoadStringW@CEnString@@QAEHIPAUHWND__@@@Z
_TEXT	SEGMENT
_nFormatID$ = 8
_hwndRef$ = 12
_this$ = -4
?LoadStringW@CEnString@@QAEHIPAUHWND__@@@Z PROC NEAR	; CEnString::LoadStringW, COMDAT

; 144  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 145  : 	return LoadString(nFormatID, hwndRef, *this);

	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hwndRef$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nFormatID$[ebp]
	push	edx
	call	?LoadStringW@CEnString@@KAHIPAUHWND__@@AAVCString@@@Z ; CEnString::LoadStringW
	add	esp, 12					; 0000000cH

; 146  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?LoadStringW@CEnString@@QAEHIPAUHWND__@@@Z ENDP		; CEnString::LoadStringW
_TEXT	ENDS
PUBLIC	?Translate@CEnString@@QAEHPAUHWND__@@@Z		; CEnString::Translate
;	COMDAT ?Translate@CEnString@@QAEHPAUHWND__@@@Z
_TEXT	SEGMENT
_hwndRef$ = 8
_this$ = -4
?Translate@CEnString@@QAEHPAUHWND__@@@Z PROC NEAR	; CEnString::Translate, COMDAT

; 149  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 150  : 	return TranslateString(*this, hwndRef);

	mov	eax, DWORD PTR _hwndRef$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	?TranslateString@CEnString@@SAHAAVCString@@PAUHWND__@@@Z ; CEnString::TranslateString
	add	esp, 8

; 151  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Translate@CEnString@@QAEHPAUHWND__@@@Z ENDP		; CEnString::Translate
_TEXT	ENDS
PUBLIC	?DoNotTranslate@CEnString@@QBEXXZ		; CEnString::DoNotTranslate
PUBLIC	?DoNotTranslateString@CEnString@@SAXABVCString@@@Z ; CEnString::DoNotTranslateString
;	COMDAT ?DoNotTranslate@CEnString@@QBEXXZ
_TEXT	SEGMENT
_this$ = -4
?DoNotTranslate@CEnString@@QBEXXZ PROC NEAR		; CEnString::DoNotTranslate, COMDAT

; 154  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 155  : 	DoNotTranslateString(*this);

	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?DoNotTranslateString@CEnString@@SAXABVCString@@@Z ; CEnString::DoNotTranslateString
	add	esp, 4

; 156  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?DoNotTranslate@CEnString@@QBEXXZ ENDP			; CEnString::DoNotTranslate
_TEXT	ENDS
;	COMDAT ?DoNotTranslateString@CEnString@@SAXABVCString@@@Z
_TEXT	SEGMENT
_sText$ = 8
?DoNotTranslateString@CEnString@@SAXABVCString@@@Z PROC NEAR ; CEnString::DoNotTranslateString, COMDAT

; 159  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 160  : 	if (s_pTT && !sText.IsEmpty())

	cmp	DWORD PTR ?s_pTT@CEnString@@1PAVITransText@@A, 0 ; CEnString::s_pTT
	je	SHORT $L73328
	mov	ecx, DWORD PTR _sText$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L73328

; 161  : 		s_pTT->IgnoreString(sText);

	mov	ecx, DWORD PTR _sText$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	mov	eax, DWORD PTR ?s_pTT@CEnString@@1PAVITransText@@A ; CEnString::s_pTT
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR ?s_pTT@CEnString@@1PAVITransText@@A ; CEnString::s_pTT
	call	DWORD PTR [edx+56]
	cmp	esi, esp
	call	__chkesp
$L73328:

; 162  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?DoNotTranslateString@CEnString@@SAXABVCString@@@Z ENDP	; CEnString::DoNotTranslateString
_TEXT	ENDS
PUBLIC	?TranslateStrings@CEnString@@SAHAAVCStringArray@@PAUHWND__@@@Z ; CEnString::TranslateStrings
EXTRN	?GetSize@CStringArray@@QBEHXZ:NEAR		; CStringArray::GetSize
EXTRN	??ACStringArray@@QAEAAVCString@@H@Z:NEAR	; CStringArray::operator[]
;	COMDAT ?TranslateStrings@CEnString@@SAHAAVCStringArray@@PAUHWND__@@@Z
_TEXT	SEGMENT
_aText$ = 8
_hwndRef$ = 12
_bResult$ = -8
_nItem$ = -4
?TranslateStrings@CEnString@@SAHAAVCStringArray@@PAUHWND__@@@Z PROC NEAR ; CEnString::TranslateStrings, COMDAT

; 165  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 166  : 	BOOL bResult = FALSE;

	mov	DWORD PTR _bResult$[ebp], 0

; 167  : 
; 168  : 	int nItem = aText.GetSize();

	mov	ecx, DWORD PTR _aText$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	mov	DWORD PTR _nItem$[ebp], eax
$L73336:

; 169  : 
; 170  : 	while (nItem--)

	mov	eax, DWORD PTR _nItem$[ebp]
	mov	ecx, DWORD PTR _nItem$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nItem$[ebp], ecx
	test	eax, eax
	je	SHORT $L73337

; 171  : 		bResult |= TranslateString(aText[nItem], hwndRef);

	mov	edx, DWORD PTR _hwndRef$[ebp]
	push	edx
	mov	eax, DWORD PTR _nItem$[ebp]
	push	eax
	mov	ecx, DWORD PTR _aText$[ebp]
	call	??ACStringArray@@QAEAAVCString@@H@Z	; CStringArray::operator[]
	push	eax
	call	?TranslateString@CEnString@@SAHAAVCString@@PAUHWND__@@@Z ; CEnString::TranslateString
	add	esp, 8
	mov	ecx, DWORD PTR _bResult$[ebp]
	or	ecx, eax
	mov	DWORD PTR _bResult$[ebp], ecx
	jmp	SHORT $L73336
$L73337:

; 172  : 
; 173  : 	return bResult;

	mov	eax, DWORD PTR _bResult$[ebp]

; 174  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?TranslateStrings@CEnString@@SAHAAVCStringArray@@PAUHWND__@@@Z ENDP ; CEnString::TranslateStrings
_TEXT	ENDS
EXTRN	??4CString@@QAEABV0@PBG@Z:NEAR			; CString::operator=
;	COMDAT ?TranslateString@CEnString@@SAHAAVCString@@PAUHWND__@@@Z
_TEXT	SEGMENT
_sText$ = 8
_hwndRef$ = 12
_szTranslated$73343 = -4
?TranslateString@CEnString@@SAHAAVCString@@PAUHWND__@@@Z PROC NEAR ; CEnString::TranslateString, COMDAT

; 177  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 178  : 	if (s_pTT && !sText.IsEmpty())

	cmp	DWORD PTR ?s_pTT@CEnString@@1PAVITransText@@A, 0 ; CEnString::s_pTT
	je	SHORT $L73344
	mov	ecx, DWORD PTR _sText$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L73344

; 180  : 		LPTSTR szTranslated = NULL;

	mov	DWORD PTR _szTranslated$73343[ebp], 0

; 181  : 
; 182  : 		if (s_pTT->TranslateText(sText, hwndRef, szTranslated))

	mov	esi, esp
	lea	eax, DWORD PTR _szTranslated$73343[ebp]
	push	eax
	mov	ecx, DWORD PTR _hwndRef$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sText$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	edx, DWORD PTR ?s_pTT@CEnString@@1PAVITransText@@A ; CEnString::s_pTT
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR ?s_pTT@CEnString@@1PAVITransText@@A ; CEnString::s_pTT
	call	DWORD PTR [eax+28]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L73344

; 184  : 			sText = szTranslated;

	mov	ecx, DWORD PTR _szTranslated$73343[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sText$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 185  : 
; 186  : 			// cleanup
; 187  : 			s_pTT->FreeTextBuffer(szTranslated);

	mov	esi, esp
	lea	edx, DWORD PTR _szTranslated$73343[ebp]
	push	edx
	mov	eax, DWORD PTR ?s_pTT@CEnString@@1PAVITransText@@A ; CEnString::s_pTT
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR ?s_pTT@CEnString@@1PAVITransText@@A ; CEnString::s_pTT
	call	DWORD PTR [edx+60]
	cmp	esi, esp
	call	__chkesp

; 188  : 
; 189  : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L73341
$L73344:

; 192  : 
; 193  : 	// all else
; 194  : 	return FALSE;

	xor	eax, eax
$L73341:

; 195  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?TranslateString@CEnString@@SAHAAVCString@@PAUHWND__@@@Z ENDP ; CEnString::TranslateString
_TEXT	ENDS
PUBLIC	?GetCharacterCount@CEnString@@QBEHGH@Z		; CEnString::GetCharacterCount
PUBLIC	??0CEnString@@QAE@ABV0@@Z			; CEnString::CEnString
EXTRN	__imp__towupper:NEAR
EXTRN	?MakeUpper@CString@@QAEXXZ:NEAR			; CString::MakeUpper
EXTRN	?Find@CString@@QBEHGH@Z:NEAR			; CString::Find
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnString.cpp
xdata$x	SEGMENT
$T73680	DD	019930520H
	DD	01H
	DD	FLAT:$T73682
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73682	DD	0ffffffffH
	DD	FLAT:$L73678
xdata$x	ENDS
;	COMDAT ?GetCharacterCount@CEnString@@QBEHGH@Z
_TEXT	SEGMENT
_nChar$ = 8
_bCaseSensitive$ = 12
_this$ = -32
_sThis$73351 = -24
_nCount$ = -20
_nFind$ = -16
$T73677 = -28
__$EHRec$ = -12
?GetCharacterCount@CEnString@@QBEHGH@Z PROC NEAR	; CEnString::GetCharacterCount, COMDAT

; 198  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73681
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

; 199  :     if (!bCaseSensitive)

	cmp	DWORD PTR _bCaseSensitive$[ebp], 0
	jne	SHORT $L73350

; 201  : 		CEnString sThis(*this);

	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sThis$73351[ebp]
	call	??0CEnString@@QAE@ABV0@@Z		; CEnString::CEnString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 202  : 		sThis.MakeUpper();

	lea	ecx, DWORD PTR _sThis$73351[ebp]
	call	?MakeUpper@CString@@QAEXXZ		; CString::MakeUpper

; 203  : 		
; 204  : 		return sThis.GetCharacterCount((TCHAR)_totupper(nChar));

	push	1
	mov	esi, esp
	mov	cx, WORD PTR _nChar$[ebp]
	push	ecx
	call	DWORD PTR __imp__towupper
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	push	eax
	lea	ecx, DWORD PTR _sThis$73351[ebp]
	call	?GetCharacterCount@CEnString@@QBEHGH@Z	; CEnString::GetCharacterCount
	mov	DWORD PTR $T73677[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sThis$73351[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	mov	eax, DWORD PTR $T73677[ebp]
	jmp	SHORT $L73349
$L73350:

; 206  : 	
; 207  :     int nCount = 0;

	mov	DWORD PTR _nCount$[ebp], 0

; 208  :     int nFind = Find(nChar, 0);

	push	0
	mov	dx, WORD PTR _nChar$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Find@CString@@QBEHGH@Z			; CString::Find
	mov	DWORD PTR _nFind$[ebp], eax
$L73357:

; 209  : 	
; 210  :     while (nFind != -1)

	cmp	DWORD PTR _nFind$[ebp], -1
	je	SHORT $L73358

; 212  : 		nCount++;

	mov	eax, DWORD PTR _nCount$[ebp]
	add	eax, 1
	mov	DWORD PTR _nCount$[ebp], eax

; 213  : 		nFind = Find(nChar, nFind + 1);

	mov	ecx, DWORD PTR _nFind$[ebp]
	add	ecx, 1
	push	ecx
	mov	dx, WORD PTR _nChar$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Find@CString@@QBEHGH@Z			; CString::Find
	mov	DWORD PTR _nFind$[ebp], eax

; 214  :     }

	jmp	SHORT $L73357
$L73358:

; 215  : 	
; 216  :     return nCount;

	mov	eax, DWORD PTR _nCount$[ebp]
$L73349:

; 217  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73678:
	lea	ecx, DWORD PTR _sThis$73351[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	ret	0
$L73681:
	mov	eax, OFFSET FLAT:$T73680
	jmp	___CxxFrameHandler
text$x	ENDS
?GetCharacterCount@CEnString@@QBEHGH@Z ENDP		; CEnString::GetCharacterCount
;	COMDAT ??0CEnString@@QAE@ABV0@@Z
_TEXT	SEGMENT
___that$ = 8
_this$ = -4
??0CEnString@@QAE@ABV0@@Z PROC NEAR			; CEnString::CEnString, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR ___that$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0CEnString@@QAE@ABV0@@Z ENDP				; CEnString::CEnString
_TEXT	ENDS
PUBLIC	?FormatDC@CEnString@@QAE?AVCSize@@PAVCDC@@HH@Z	; CEnString::FormatDC
PUBLIC	?FormatDCEx@CEnString@@IAE?AVCSize@@PAVCDC@@HH@Z ; CEnString::FormatDCEx
EXTRN	?GetSafeHdc@CDC@@QBEPAUHDC__@@XZ:NEAR		; CDC::GetSafeHdc
EXTRN	__imp__DrawTextW@20:NEAR
EXTRN	??0CRect@@QAE@HHHH@Z:NEAR			; CRect::CRect
EXTRN	?Width@CRect@@QBEHXZ:NEAR			; CRect::Width
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	??BCRect@@QAEPAUtagRECT@@XZ:NEAR		; CRect::operator tagRECT *
EXTRN	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z:NEAR ; CDC::GetTextExtent
EXTRN	?GetBuffer@CString@@QAEPAGH@Z:NEAR		; CString::GetBuffer
EXTRN	?ReleaseBuffer@CString@@QAEXH@Z:NEAR		; CString::ReleaseBuffer
;	COMDAT ?FormatDC@CEnString@@QAE?AVCSize@@PAVCDC@@HH@Z
_TEXT	SEGMENT
_pDC$ = 12
_nWidth$ = 16
_nStyle$ = 20
___$ReturnUdt$ = 8
_this$ = -40
_rect$ = -16
_uFlags$ = -20
$T73690 = -28
$T73691 = -36
?FormatDC@CEnString@@QAE?AVCSize@@PAVCDC@@HH@Z PROC NEAR ; CEnString::FormatDC, COMDAT

; 220  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 44					; 0000002cH
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-44]
	mov	ecx, 11					; 0000000bH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 221  : 	CRect rect(0, 0, nWidth, 20);

	push	20					; 00000014H
	mov	eax, DWORD PTR _nWidth$[ebp]
	push	eax
	push	0
	push	0
	lea	ecx, DWORD PTR _rect$[ebp]
	call	??0CRect@@QAE@HHHH@Z			; CRect::CRect

; 222  : 	UINT uFlags = (DT_CALCRECT | DT_SINGLELINE | DT_MODIFYSTRING);

	mov	DWORD PTR _uFlags$[ebp], 66592		; 00010420H

; 223  : 
; 224  : 	// special case: ES_START
; 225  : 	if (nStyle == ES_START)

	cmp	DWORD PTR _nStyle$[ebp], 3
	jne	SHORT $L73371

; 227  : 		FormatDCEx(pDC, nWidth, nStyle);

	mov	ecx, DWORD PTR _nStyle$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nWidth$[ebp]
	push	edx
	mov	eax, DWORD PTR _pDC$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T73690[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FormatDCEx@CEnString@@IAE?AVCSize@@PAVCDC@@HH@Z ; CEnString::FormatDCEx

; 229  : 	else

	jmp	$L73381
$L73371:

; 232  : 		{

	mov	edx, DWORD PTR _nStyle$[ebp]
	mov	DWORD PTR -44+[ebp], edx
	cmp	DWORD PTR -44+[ebp], 0
	je	SHORT $L73378
	cmp	DWORD PTR -44+[ebp], 1
	je	SHORT $L73379
	cmp	DWORD PTR -44+[ebp], 2
	je	SHORT $L73380
	jmp	SHORT $L73375
$L73378:

; 233  : 			case ES_END:
; 234  : 				uFlags |= DT_END_ELLIPSIS;

	mov	eax, DWORD PTR _uFlags$[ebp]
	or	ah, -128				; ffffff80H
	mov	DWORD PTR _uFlags$[ebp], eax

; 235  : 				break;

	jmp	SHORT $L73375
$L73379:

; 236  : 
; 237  : 			case ES_WORD:
; 238  : 				uFlags |= DT_WORD_ELLIPSIS;

	mov	ecx, DWORD PTR _uFlags$[ebp]
	or	ecx, 262144				; 00040000H
	mov	DWORD PTR _uFlags$[ebp], ecx

; 239  : 				break;

	jmp	SHORT $L73375
$L73380:

; 240  : 
; 241  : 			case ES_PATH:
; 242  : 				uFlags |= DT_PATH_ELLIPSIS;

	mov	edx, DWORD PTR _uFlags$[ebp]
	or	dh, 64					; 00000040H
	mov	DWORD PTR _uFlags$[ebp], edx
$L73375:

; 245  : 
; 246  : 		::DrawText(pDC->GetSafeHdc(), GetBuffer(GetLength() + 4), -1, rect, uFlags);

	mov	esi, esp
	mov	eax, DWORD PTR _uFlags$[ebp]
	push	eax
	lea	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	push	-1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	add	eax, 4
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetSafeHdc@CDC@@QBEPAUHDC__@@XZ	; CDC::GetSafeHdc
	push	eax
	call	DWORD PTR __imp__DrawTextW@20
	cmp	esi, esp
	call	__chkesp

; 247  : 		ReleaseBuffer();

	push	-1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 248  : 
; 249  : 		// if its still too big then do our internal version
; 250  : 		if (rect.Width() > nWidth)

	lea	ecx, DWORD PTR _rect$[ebp]
	call	?Width@CRect@@QBEHXZ			; CRect::Width
	cmp	eax, DWORD PTR _nWidth$[ebp]
	jle	SHORT $L73381

; 251  : 			FormatDCEx(pDC, nWidth, nStyle);

	mov	ecx, DWORD PTR _nStyle$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nWidth$[ebp]
	push	edx
	mov	eax, DWORD PTR _pDC$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T73691[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FormatDCEx@CEnString@@IAE?AVCSize@@PAVCDC@@HH@Z ; CEnString::FormatDCEx
$L73381:

; 253  : 
; 254  : 	return pDC->GetTextExtent(*this);

	mov	edx, DWORD PTR _this$[ebp]
	push	edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z ; CDC::GetTextExtent
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 255  : }

	pop	edi
	pop	esi
	add	esp, 44					; 0000002cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?FormatDC@CEnString@@QAE?AVCSize@@PAVCDC@@HH@Z ENDP	; CEnString::FormatDC
_TEXT	ENDS
PUBLIC	?Draw@CEnString@@QAE?AVCSize@@PAVCDC@@PAUtagRECT@@H@Z ; CEnString::Draw
EXTRN	?DrawTextW@CDC@@QAEHABVCString@@PAUtagRECT@@I@Z:NEAR ; CDC::DrawTextW
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnString.cpp
xdata$x	SEGMENT
$T73697	DD	019930520H
	DD	01H
	DD	FLAT:$T73699
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73699	DD	0ffffffffH
	DD	FLAT:$L73695
xdata$x	ENDS
;	COMDAT ?Draw@CEnString@@QAE?AVCSize@@PAVCDC@@PAUtagRECT@@H@Z
_TEXT	SEGMENT
_pDC$ = 12
_lpRect$ = 16
_nStyle$ = 20
___$ReturnUdt$ = 8
_this$ = -28
_sTemp$ = -16
$T73694 = -24
__$EHRec$ = -12
?Draw@CEnString@@QAE?AVCSize@@PAVCDC@@PAUtagRECT@@H@Z PROC NEAR ; CEnString::Draw, COMDAT

; 258  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73698
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

; 259  : 	// we must do the format independently of DrawText because we override it
; 260  : 	CEnString sTemp(*this);

	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sTemp$[ebp]
	call	??0CEnString@@QAE@ABV0@@Z		; CEnString::CEnString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 261  : 	sTemp.FormatDC(pDC, lpRect->right - lpRect->left, nStyle);

	mov	ecx, DWORD PTR _nStyle$[ebp]
	push	ecx
	mov	edx, DWORD PTR _lpRect$[ebp]
	mov	eax, DWORD PTR _lpRect$[ebp]
	mov	ecx, DWORD PTR [edx+8]
	sub	ecx, DWORD PTR [eax]
	push	ecx
	mov	edx, DWORD PTR _pDC$[ebp]
	push	edx
	lea	eax, DWORD PTR $T73694[ebp]
	push	eax
	lea	ecx, DWORD PTR _sTemp$[ebp]
	call	?FormatDC@CEnString@@QAE?AVCSize@@PAVCDC@@HH@Z ; CEnString::FormatDC

; 262  : 
; 263  : 	pDC->DrawText(sTemp, lpRect, DT_SINGLELINE);

	push	32					; 00000020H
	mov	ecx, DWORD PTR _lpRect$[ebp]
	push	ecx
	lea	edx, DWORD PTR _sTemp$[ebp]
	push	edx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?DrawTextW@CDC@@QAEHABVCString@@PAUtagRECT@@I@Z ; CDC::DrawTextW

; 264  : 
; 265  : 	return pDC->GetTextExtent(sTemp);

	lea	eax, DWORD PTR _sTemp$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z ; CDC::GetTextExtent
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sTemp$[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 266  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73695:
	lea	ecx, DWORD PTR _sTemp$[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	ret	0
$L73698:
	mov	eax, OFFSET FLAT:$T73697
	jmp	___CxxFrameHandler
text$x	ENDS
?Draw@CEnString@@QAE?AVCSize@@PAVCDC@@PAUtagRECT@@H@Z ENDP ; CEnString::Draw
EXTRN	??0CString@@QAE@PBD@Z:NEAR			; CString::CString
EXTRN	??YCString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator+=
EXTRN	??H@YG?AVCString@@ABV0@0@Z:NEAR			; operator+
EXTRN	?Mid@CString@@QBE?AV1@H@Z:NEAR			; CString::Mid
EXTRN	?Left@CString@@QBE?AV1@H@Z:NEAR			; CString::Left
;	COMDAT data
; File D:\_CODE\Shared\EnString.cpp
data	SEGMENT
$SG73407 DB	'...', 00H
$SG73419 DB	'...', 00H
$SG73426 DB	'...', 00H
$SG73430 DB	'...', 00H
$SG73434 DB	'...', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73732	DD	019930520H
	DD	0cH
	DD	FLAT:$T73734
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73734	DD	0ffffffffH
	DD	FLAT:$L73719
	DD	00H
	DD	FLAT:$L73720
	DD	00H
	DD	FLAT:$L73721
	DD	00H
	DD	FLAT:$L73722
	DD	03H
	DD	FLAT:$L73723
	DD	00H
	DD	FLAT:$L73724
	DD	00H
	DD	FLAT:$L73725
	DD	06H
	DD	FLAT:$L73726
	DD	00H
	DD	FLAT:$L73727
	DD	00H
	DD	FLAT:$L73728
	DD	09H
	DD	FLAT:$L73729
	DD	00H
	DD	FLAT:$L73730
xdata$x	ENDS
;	COMDAT ?FormatDCEx@CEnString@@IAE?AVCSize@@PAVCDC@@HH@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_pDC$ = 12
_nWidth$ = 16
_nStyle$ = 20
___$ReturnUdt$ = 8
_this$ = -100
_sFinalText$ = -24
_sizeText$ = -36
_sizeEllipsis$ = -20
_bEndEllipsis$ = -28
$T73704 = -40
$T73707 = -44
$T73708 = -48
$T73709 = -52
$T73710 = -60
$T73711 = -64
$T73712 = -68
$T73713 = -72
$T73714 = -80
$T73715 = -84
$T73716 = -88
$T73717 = -92
$T73718 = -96
?FormatDCEx@CEnString@@IAE?AVCSize@@PAVCDC@@HH@Z PROC NEAR ; CEnString::FormatDCEx, COMDAT

; 269  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73733
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 140				; 0000008cH
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-152]
	mov	ecx, 35					; 00000023H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 270  : 	CString sFinalText = *this;

	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFinalText$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 271  : 
; 272  : 	CSize sizeText = pDC->GetTextExtent(sFinalText);

	lea	ecx, DWORD PTR _sFinalText$[ebp]
	push	ecx
	lea	edx, DWORD PTR _sizeText$[ebp]
	push	edx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z ; CDC::GetTextExtent

; 273  : 	CSize sizeEllipsis = pDC->GetTextExtent("...");

	push	OFFSET FLAT:$SG73407
	lea	ecx, DWORD PTR $T73704[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	eax, DWORD PTR $T73704[ebp]
	push	eax
	lea	ecx, DWORD PTR _sizeEllipsis$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z ; CDC::GetTextExtent
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T73704[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 274  : 
; 275  : 	bool bEndEllipsis = (nStyle == ES_END || nStyle == ES_WORD);

	cmp	DWORD PTR _nStyle$[ebp], 0
	je	SHORT $L73705
	cmp	DWORD PTR _nStyle$[ebp], 1
	je	SHORT $L73705
	mov	DWORD PTR -104+[ebp], 0
	jmp	SHORT $L73706
$L73705:
	mov	DWORD PTR -104+[ebp], 1
$L73706:
	mov	dl, BYTE PTR -104+[ebp]
	mov	BYTE PTR _bEndEllipsis$[ebp], dl

; 276  : 	
; 277  : 	// truncate string if too long adding ellipsis (...)
; 278  : 	if ((sizeText.cx + sizeEllipsis.cx) > nWidth)

	mov	eax, DWORD PTR _sizeText$[ebp]
	add	eax, DWORD PTR _sizeEllipsis$[ebp]
	cmp	eax, DWORD PTR _nWidth$[ebp]
	jle	$L73431
$L73411:

; 280  : 	    while ((sizeText.cx + sizeEllipsis.cx) > nWidth)

	mov	ecx, DWORD PTR _sizeText$[ebp]
	add	ecx, DWORD PTR _sizeEllipsis$[ebp]
	cmp	ecx, DWORD PTR _nWidth$[ebp]
	jle	$L73412

; 282  : 			// truncate another char
; 283  : 			if (bEndEllipsis)

	mov	edx, DWORD PTR _bEndEllipsis$[ebp]
	and	edx, 255				; 000000ffH
	test	edx, edx
	je	$L73413

; 285  : 				sFinalText = sFinalText.Left(sFinalText.GetLength() - 1);

	lea	ecx, DWORD PTR _sFinalText$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	sub	eax, 1
	push	eax
	lea	eax, DWORD PTR $T73707[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFinalText$[ebp]
	call	?Left@CString@@QBE?AV1@H@Z		; CString::Left
	mov	DWORD PTR -108+[ebp], eax
	mov	ecx, DWORD PTR -108+[ebp]
	mov	DWORD PTR -112+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	edx, DWORD PTR -112+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sFinalText$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T73707[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 286  : 				sizeText = pDC->GetTextExtent(CString("..." + sFinalText));

	push	OFFSET FLAT:$SG73419
	lea	ecx, DWORD PTR $T73708[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	eax, DWORD PTR _sFinalText$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T73708[ebp]
	push	ecx
	lea	edx, DWORD PTR $T73709[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -116+[ebp], eax
	mov	eax, DWORD PTR -116+[ebp]
	mov	DWORD PTR -120+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	ecx, DWORD PTR -120+[ebp]
	push	ecx
	lea	edx, DWORD PTR $T73710[ebp]
	push	edx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z ; CDC::GetTextExtent
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR _sizeText$[ebp], ecx
	mov	DWORD PTR _sizeText$[ebp+4], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR $T73709[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T73708[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 288  : 			else

	jmp	$L73420
$L73413:

; 290  : 				sFinalText = sFinalText.Mid(1);

	push	1
	lea	eax, DWORD PTR $T73711[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFinalText$[ebp]
	call	?Mid@CString@@QBE?AV1@H@Z		; CString::Mid
	mov	DWORD PTR -124+[ebp], eax
	mov	ecx, DWORD PTR -124+[ebp]
	mov	DWORD PTR -128+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	mov	edx, DWORD PTR -128+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sFinalText$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T73711[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 291  : 		        sizeText = pDC->GetTextExtent(CString(sFinalText + "..."));

	push	OFFSET FLAT:$SG73426
	lea	ecx, DWORD PTR $T73712[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	eax, DWORD PTR $T73712[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFinalText$[ebp]
	push	ecx
	lea	edx, DWORD PTR $T73713[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -132+[ebp], eax
	mov	eax, DWORD PTR -132+[ebp]
	mov	DWORD PTR -136+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	mov	ecx, DWORD PTR -136+[ebp]
	push	ecx
	lea	edx, DWORD PTR $T73714[ebp]
	push	edx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z ; CDC::GetTextExtent
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR _sizeText$[ebp], ecx
	mov	DWORD PTR _sizeText$[ebp+4], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR $T73713[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T73712[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L73420:

; 293  : 
; 294  : 			if (sFinalText.IsEmpty())

	lea	ecx, DWORD PTR _sFinalText$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L73427

; 295  : 				break;

	jmp	SHORT $L73412
$L73427:

; 296  : 		}

	jmp	$L73411
$L73412:

; 297  : 			
; 298  : 		if (bEndEllipsis)

	mov	eax, DWORD PTR _bEndEllipsis$[ebp]
	and	eax, 255				; 000000ffH
	test	eax, eax
	je	SHORT $L73428

; 299  : 			sFinalText += "...";

	push	OFFSET FLAT:$SG73430
	lea	ecx, DWORD PTR $T73715[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 8
	lea	ecx, DWORD PTR $T73715[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sFinalText$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T73715[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 300  : 		else

	jmp	SHORT $L73431
$L73428:

; 301  : 			sFinalText = "..." + sFinalText;

	push	OFFSET FLAT:$SG73434
	lea	ecx, DWORD PTR $T73716[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 9
	lea	edx, DWORD PTR _sFinalText$[ebp]
	push	edx
	lea	eax, DWORD PTR $T73716[ebp]
	push	eax
	lea	ecx, DWORD PTR $T73717[ebp]
	push	ecx
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -140+[ebp], eax
	mov	edx, DWORD PTR -140+[ebp]
	mov	DWORD PTR -144+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 10		; 0000000aH
	mov	eax, DWORD PTR -144+[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFinalText$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 9
	lea	ecx, DWORD PTR $T73717[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T73716[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L73431:

; 303  : 
; 304  : 	*this = CEnString(sFinalText);

	lea	ecx, DWORD PTR _sFinalText$[ebp]
	push	ecx
	lea	ecx, DWORD PTR $T73718[ebp]
	call	??0CEnString@@QAE@ABVCString@@@Z	; CEnString::CEnString
	mov	DWORD PTR -148+[ebp], eax
	mov	edx, DWORD PTR -148+[ebp]
	mov	DWORD PTR -152+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 11		; 0000000bH
	mov	eax, DWORD PTR -152+[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??4CEnString@@QAEAAV0@ABV0@@Z		; CEnString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T73718[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString

; 305  : 
; 306  : 	return pDC->GetTextExtent(*this);

	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	mov	edx, DWORD PTR ___$ReturnUdt$[ebp]
	push	edx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z ; CDC::GetTextExtent
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sFinalText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 307  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 152				; 00000098H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73719:
	lea	ecx, DWORD PTR _sFinalText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73720:
	lea	ecx, DWORD PTR $T73704[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73721:
	lea	ecx, DWORD PTR $T73707[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73722:
	lea	ecx, DWORD PTR $T73708[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73723:
	lea	ecx, DWORD PTR $T73709[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73724:
	lea	ecx, DWORD PTR $T73711[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73725:
	lea	ecx, DWORD PTR $T73712[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73726:
	lea	ecx, DWORD PTR $T73713[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73727:
	lea	ecx, DWORD PTR $T73715[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73728:
	lea	ecx, DWORD PTR $T73716[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73729:
	lea	ecx, DWORD PTR $T73717[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73730:
	lea	ecx, DWORD PTR $T73718[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	ret	0
$L73733:
	mov	eax, OFFSET FLAT:$T73732
	jmp	___CxxFrameHandler
text$x	ENDS
?FormatDCEx@CEnString@@IAE?AVCSize@@PAVCDC@@HH@Z ENDP	; CEnString::FormatDCEx
PUBLIC	?SetLocalizer@CEnString@@SAXPAVITransText@@@Z	; CEnString::SetLocalizer
;	COMDAT ?SetLocalizer@CEnString@@SAXPAVITransText@@@Z
_TEXT	SEGMENT
_pTT$ = 8
?SetLocalizer@CEnString@@SAXPAVITransText@@@Z PROC NEAR	; CEnString::SetLocalizer, COMDAT

; 310  : {

	push	ebp
	mov	ebp, esp

; 311  : 	s_pTT = pTT;

	mov	eax, DWORD PTR _pTT$[ebp]
	mov	DWORD PTR ?s_pTT@CEnString@@1PAVITransText@@A, eax ; CEnString::s_pTT

; 312  : }

	pop	ebp
	ret	0
?SetLocalizer@CEnString@@SAXPAVITransText@@@Z ENDP	; CEnString::SetLocalizer
_TEXT	ENDS
PUBLIC	?Allocate@CEnString@@QAEHH@Z			; CEnString::Allocate
PUBLIC	?Allocate@CEnString@@SAHAAVCString@@H@Z		; CEnString::Allocate
;	COMDAT ?Allocate@CEnString@@QAEHH@Z
_TEXT	SEGMENT
_nMinLen$ = 8
_this$ = -4
?Allocate@CEnString@@QAEHH@Z PROC NEAR			; CEnString::Allocate, COMDAT

; 315  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 316  : 	return Allocate(*this, nMinLen);

	mov	eax, DWORD PTR _nMinLen$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	?Allocate@CEnString@@SAHAAVCString@@H@Z	; CEnString::Allocate
	add	esp, 8

; 317  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Allocate@CEnString@@QAEHH@Z ENDP			; CEnString::Allocate
_TEXT	ENDS
;	COMDAT ?Allocate@CEnString@@SAHAAVCString@@H@Z
_TEXT	SEGMENT
_str$ = 8
_nMinLen$ = 12
_nCurLen$ = -4
?Allocate@CEnString@@SAHAAVCString@@H@Z PROC NEAR	; CEnString::Allocate, COMDAT

; 321  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 322  : 	int nCurLen = str.GetLength();

	mov	ecx, DWORD PTR _str$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR _nCurLen$[ebp], eax

; 323  : 
; 324  : 	if (nCurLen < nMinLen)

	mov	eax, DWORD PTR _nCurLen$[ebp]
	cmp	eax, DWORD PTR _nMinLen$[ebp]
	jge	SHORT $L73449

; 326  : 		str.GetBuffer(nMinLen);

	mov	ecx, DWORD PTR _nMinLen$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _str$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer

; 327  : 		str.ReleaseBuffer(nCurLen);

	mov	edx, DWORD PTR _nCurLen$[ebp]
	push	edx
	mov	ecx, DWORD PTR _str$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 328  : 
; 329  : 		return nMinLen;

	mov	eax, DWORD PTR _nMinLen$[ebp]
	jmp	SHORT $L73447
$L73449:

; 331  : 
; 332  : 	return nCurLen;

	mov	eax, DWORD PTR _nCurLen$[ebp]
$L73447:

; 333  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Allocate@CEnString@@SAHAAVCString@@H@Z ENDP		; CEnString::Allocate
_TEXT	ENDS
END

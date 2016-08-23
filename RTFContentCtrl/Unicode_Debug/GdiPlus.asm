	TITLE	D:\_CODE\Shared\GdiPlus.cpp
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
;	COMDAT ?Initialize@CGdiPlus@@KAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Free@CGdiPlus@@KAXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateBitmapFromStream@CGdiPlus@@SAHPAUIStream@@PAPAPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateBitmapFromFile@CGdiPlus@@SAHPBGPAPAPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateHBITMAPFromBitmap@CGdiPlus@@SAHPAPAXPAPAUHBITMAP__@@K@Z
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
PUBLIC	?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A	; CGdiPlus::s_hGdiPlus
PUBLIC	?s_GdiToken@CGdiPlus@@1_KA			; CGdiPlus::s_GdiToken
_BSS	SEGMENT
?s_GdiToken@CGdiPlus@@1_KA DQ 01H DUP (?)		; CGdiPlus::s_GdiToken
_BSS	ENDS
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\Shared\GdiPlus.cpp', 00H
?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A DD 0ffffffffH	; CGdiPlus::s_hGdiPlus
_DATA	ENDS
PUBLIC	?Initialize@CGdiPlus@@KAHXZ			; CGdiPlus::Initialize
EXTRN	__imp__FreeLibrary@4:NEAR
EXTRN	__imp__GetProcAddress@8:NEAR
EXTRN	__imp__LoadLibraryW@4:NEAR
EXTRN	__chkesp:NEAR
;	COMDAT data
; File D:\_CODE\Shared\GdiPlus.cpp
data	SEGMENT
$SG73315 DB	'G', 00H, 'd', 00H, 'i', 00H, 'P', 00H, 'l', 00H, 'u', 00H
	DB	's', 00H, '.', 00H, 'd', 00H, 'l', 00H, 'l', 00H, 00H, 00H
$SG73319 DB	'GdiplusStartup', 00H
data	ENDS
;	COMDAT ?Initialize@CGdiPlus@@KAHXZ
_TEXT	SEGMENT
_pFn$73317 = -4
_input$73321 = -20
?Initialize@CGdiPlus@@KAHXZ PROC NEAR			; CGdiPlus::Initialize, COMDAT

; 102  : {

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

; 103  : 	if (s_hGdiPlus == HMODULE(-1)) // one time only

	cmp	DWORD PTR ?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A, -1 ; CGdiPlus::s_hGdiPlus
	jne	$L73316

; 105  : 		s_hGdiPlus = LoadLibrary(_T("GdiPlus.dll"));

	mov	esi, esp
	push	OFFSET FLAT:$SG73315
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR ?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A, eax ; CGdiPlus::s_hGdiPlus

; 106  : 
; 107  : 		if (s_hGdiPlus)

	cmp	DWORD PTR ?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A, 0 ; CGdiPlus::s_hGdiPlus
	je	$L73316

; 109  : 		    PFNSTARTUP pFn = (PFNSTARTUP)GetProcAddress(s_hGdiPlus, "GdiplusStartup");

	mov	esi, esp
	push	OFFSET FLAT:$SG73319
	mov	eax, DWORD PTR ?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A ; CGdiPlus::s_hGdiPlus
	push	eax
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pFn$73317[ebp], eax

; 110  : 
; 111  : 			if (pFn)

	cmp	DWORD PTR _pFn$73317[ebp], 0
	je	SHORT $L73322

; 113  : 			    gdix_StartupInput input = { 0 };

	mov	DWORD PTR _input$73321[ebp], 0
	xor	ecx, ecx
	mov	DWORD PTR _input$73321[ebp+4], ecx
	mov	DWORD PTR _input$73321[ebp+8], ecx
	mov	DWORD PTR _input$73321[ebp+12], ecx

; 114  : 
; 115  : 				input.GdiplusVersion = 1;

	mov	DWORD PTR _input$73321[ebp], 1

; 116  : 				input.SuppressExternalCodecs = TRUE;

	mov	DWORD PTR _input$73321[ebp+12], 1

; 117  : 
; 118  : 				if (pFn(&s_GdiToken, &input, NULL) == gdix_Ok) 

	mov	esi, esp
	push	0
	lea	edx, DWORD PTR _input$73321[ebp]
	push	edx
	push	OFFSET FLAT:?s_GdiToken@CGdiPlus@@1_KA	; CGdiPlus::s_GdiToken
	call	DWORD PTR _pFn$73317[ebp]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L73322

; 120  : 					return TRUE;

	mov	eax, 1
	jmp	SHORT $L73313
$L73322:

; 123  : 
; 124  : 			// failed
; 125  : 			FreeLibrary(s_hGdiPlus);

	mov	esi, esp
	mov	eax, DWORD PTR ?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A ; CGdiPlus::s_hGdiPlus
	push	eax
	call	DWORD PTR __imp__FreeLibrary@4
	cmp	esi, esp
	call	__chkesp

; 126  : 
; 127  : 			s_hGdiPlus = NULL;

	mov	DWORD PTR ?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A, 0 ; CGdiPlus::s_hGdiPlus

; 128  : 			s_GdiToken = NULL;

	mov	DWORD PTR ?s_GdiToken@CGdiPlus@@1_KA, 0
	mov	DWORD PTR ?s_GdiToken@CGdiPlus@@1_KA+4, 0
$L73316:

; 131  : 	
; 132  : 	return (NULL != s_hGdiPlus);

	xor	eax, eax
	cmp	DWORD PTR ?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A, 0 ; CGdiPlus::s_hGdiPlus
	setne	al
$L73313:

; 133  : }

	pop	esi
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Initialize@CGdiPlus@@KAHXZ ENDP			; CGdiPlus::Initialize
_TEXT	ENDS
PUBLIC	?Free@CGdiPlus@@KAXXZ				; CGdiPlus::Free
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
;	COMDAT data
; File D:\_CODE\Shared\GdiPlus.cpp
data	SEGMENT
$SG73332 DB	'GdiplusShutdown', 00H
data	ENDS
;	COMDAT ?Free@CGdiPlus@@KAXXZ
_TEXT	SEGMENT
_pFn$73330 = -4
?Free@CGdiPlus@@KAXXZ PROC NEAR				; CGdiPlus::Free, COMDAT

; 136  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 137  : 	if (s_hGdiPlus)

	cmp	DWORD PTR ?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A, 0 ; CGdiPlus::s_hGdiPlus
	je	$L73325
$L73326:

; 139  : 		ASSERT(s_GdiToken);

	mov	eax, DWORD PTR ?s_GdiToken@CGdiPlus@@1_KA
	or	eax, DWORD PTR ?s_GdiToken@CGdiPlus@@1_KA+4
	test	eax, eax
	jne	SHORT $L73329
	push	139					; 0000008bH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73329
	int	3
$L73329:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73326

; 140  : 
; 141  : 		PFNSHUTDOWN pFn = (PFNSHUTDOWN)GetProcAddress(s_hGdiPlus, "GdiplusShutdown");

	mov	esi, esp
	push	OFFSET FLAT:$SG73332
	mov	edx, DWORD PTR ?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A ; CGdiPlus::s_hGdiPlus
	push	edx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pFn$73330[ebp], eax

; 142  : 		
; 143  : 		if (pFn)

	cmp	DWORD PTR _pFn$73330[ebp], 0
	je	SHORT $L73333

; 144  : 			pFn(s_GdiToken);

	mov	esi, esp
	mov	eax, DWORD PTR ?s_GdiToken@CGdiPlus@@1_KA+4
	push	eax
	mov	ecx, DWORD PTR ?s_GdiToken@CGdiPlus@@1_KA
	push	ecx
	call	DWORD PTR _pFn$73330[ebp]
	cmp	esi, esp
	call	__chkesp
$L73333:

; 145  : 
; 146  : 		FreeLibrary(s_hGdiPlus);

	mov	esi, esp
	mov	edx, DWORD PTR ?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A ; CGdiPlus::s_hGdiPlus
	push	edx
	call	DWORD PTR __imp__FreeLibrary@4
	cmp	esi, esp
	call	__chkesp

; 147  : 
; 148  : 		s_hGdiPlus = HMODULE(-1);

	mov	DWORD PTR ?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A, -1 ; CGdiPlus::s_hGdiPlus

; 149  : 		s_GdiToken = NULL;

	mov	DWORD PTR ?s_GdiToken@CGdiPlus@@1_KA, 0
	mov	DWORD PTR ?s_GdiToken@CGdiPlus@@1_KA+4, 0
$L73325:

; 151  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Free@CGdiPlus@@KAXXZ ENDP				; CGdiPlus::Free
_TEXT	ENDS
PUBLIC	?CreateBitmapFromStream@CGdiPlus@@SAHPAUIStream@@PAPAPAX@Z ; CGdiPlus::CreateBitmapFromStream
_BSS	SEGMENT
_?pFN@?3??CreateBitmapFromStream@CGdiPlus@@SAHPAUIStream@@PAPAPAX@Z@4P6G?AW4gdix_Status@@01@ZA DD 01H DUP (?)
_?$S269@?3??CreateBitmapFromStream@CGdiPlus@@SAHPAUIStream@@PAPAPAX@Z@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\GdiPlus.cpp
data	SEGMENT
$SG73345 DB	'GdipCreateBitmapFromStream', 00H
data	ENDS
;	COMDAT ?CreateBitmapFromStream@CGdiPlus@@SAHPAUIStream@@PAPAPAX@Z
_TEXT	SEGMENT
_stream$ = 8
_bitmap$ = 12
?CreateBitmapFromStream@CGdiPlus@@SAHPAUIStream@@PAPAPAX@Z PROC NEAR ; CGdiPlus::CreateBitmapFromStream, COMDAT

; 154  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 155  : 	if (!Initialize())

	call	?Initialize@CGdiPlus@@KAHXZ		; CGdiPlus::Initialize
	test	eax, eax
	jne	SHORT $L73338

; 156  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L73337
$L73338:

; 157  : 	
; 158  : 	static PFNCREATEBITMAPFROMSTREAM pFN = (PFNCREATEBITMAPFROMSTREAM)GetProcAddress(s_hGdiPlus, "GdipCreateBitmapFromStream");

	xor	eax, eax
	mov	al, BYTE PTR _?$S269@?3??CreateBitmapFromStream@CGdiPlus@@SAHPAUIStream@@PAPAPAX@Z@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L73344
	mov	cl, BYTE PTR _?$S269@?3??CreateBitmapFromStream@CGdiPlus@@SAHPAUIStream@@PAPAPAX@Z@4EA
	or	cl, 1
	mov	BYTE PTR _?$S269@?3??CreateBitmapFromStream@CGdiPlus@@SAHPAUIStream@@PAPAPAX@Z@4EA, cl
	mov	esi, esp
	push	OFFSET FLAT:$SG73345
	mov	edx, DWORD PTR ?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A ; CGdiPlus::s_hGdiPlus
	push	edx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?pFN@?3??CreateBitmapFromStream@CGdiPlus@@SAHPAUIStream@@PAPAPAX@Z@4P6G?AW4gdix_Status@@01@ZA, eax
$L73344:

; 159  : 
; 160  : 	if (pFN)

	cmp	DWORD PTR _?pFN@?3??CreateBitmapFromStream@CGdiPlus@@SAHPAUIStream@@PAPAPAX@Z@4P6G?AW4gdix_Status@@01@ZA, 0
	je	SHORT $L73346

; 161  : 		return (pFN(stream, bitmap) == gdix_Ok);

	mov	esi, esp
	mov	eax, DWORD PTR _bitmap$[ebp]
	push	eax
	mov	ecx, DWORD PTR _stream$[ebp]
	push	ecx
	call	DWORD PTR _?pFN@?3??CreateBitmapFromStream@CGdiPlus@@SAHPAUIStream@@PAPAPAX@Z@4P6G?AW4gdix_Status@@01@ZA
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax
	jmp	SHORT $L73337
$L73346:

; 162  : 
; 163  : 	// else
; 164  : 	return FALSE;

	xor	eax, eax
$L73337:

; 165  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?CreateBitmapFromStream@CGdiPlus@@SAHPAUIStream@@PAPAPAX@Z ENDP ; CGdiPlus::CreateBitmapFromStream
_TEXT	ENDS
PUBLIC	?CreateBitmapFromFile@CGdiPlus@@SAHPBGPAPAPAX@Z	; CGdiPlus::CreateBitmapFromFile
_BSS	SEGMENT
	ALIGN	4

_?pFN@?3??CreateBitmapFromFile@CGdiPlus@@SAHPBGPAPAPAX@Z@4P6G?AW4gdix_Status@@01@ZA DD 01H DUP (?)
_?$S270@?3??CreateBitmapFromFile@CGdiPlus@@SAHPBGPAPAPAX@Z@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\GdiPlus.cpp
data	SEGMENT
$SG73358 DB	'GdipCreateBitmapFromFile', 00H
data	ENDS
;	COMDAT ?CreateBitmapFromFile@CGdiPlus@@SAHPBGPAPAPAX@Z
_TEXT	SEGMENT
_filename$ = 8
_bitmap$ = 12
?CreateBitmapFromFile@CGdiPlus@@SAHPBGPAPAPAX@Z PROC NEAR ; CGdiPlus::CreateBitmapFromFile, COMDAT

; 168  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 169  : 	if (!Initialize())

	call	?Initialize@CGdiPlus@@KAHXZ		; CGdiPlus::Initialize
	test	eax, eax
	jne	SHORT $L73351

; 170  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L73350
$L73351:

; 171  : 
; 172  : 	static PFNCREATEBITMAPFROMFILE pFN = (PFNCREATEBITMAPFROMFILE)GetProcAddress(s_hGdiPlus, "GdipCreateBitmapFromFile");

	xor	eax, eax
	mov	al, BYTE PTR _?$S270@?3??CreateBitmapFromFile@CGdiPlus@@SAHPBGPAPAPAX@Z@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L73357
	mov	cl, BYTE PTR _?$S270@?3??CreateBitmapFromFile@CGdiPlus@@SAHPBGPAPAPAX@Z@4EA
	or	cl, 1
	mov	BYTE PTR _?$S270@?3??CreateBitmapFromFile@CGdiPlus@@SAHPBGPAPAPAX@Z@4EA, cl
	mov	esi, esp
	push	OFFSET FLAT:$SG73358
	mov	edx, DWORD PTR ?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A ; CGdiPlus::s_hGdiPlus
	push	edx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?pFN@?3??CreateBitmapFromFile@CGdiPlus@@SAHPBGPAPAPAX@Z@4P6G?AW4gdix_Status@@01@ZA, eax
$L73357:

; 173  : 
; 174  : 	if (pFN)

	cmp	DWORD PTR _?pFN@?3??CreateBitmapFromFile@CGdiPlus@@SAHPBGPAPAPAX@Z@4P6G?AW4gdix_Status@@01@ZA, 0
	je	SHORT $L73359

; 175  : 		return (pFN(filename, bitmap) == gdix_Ok);

	mov	esi, esp
	mov	eax, DWORD PTR _bitmap$[ebp]
	push	eax
	mov	ecx, DWORD PTR _filename$[ebp]
	push	ecx
	call	DWORD PTR _?pFN@?3??CreateBitmapFromFile@CGdiPlus@@SAHPBGPAPAPAX@Z@4P6G?AW4gdix_Status@@01@ZA
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax
	jmp	SHORT $L73350
$L73359:

; 176  : 
; 177  : 	// else
; 178  : 	return FALSE;

	xor	eax, eax
$L73350:

; 179  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?CreateBitmapFromFile@CGdiPlus@@SAHPBGPAPAPAX@Z ENDP	; CGdiPlus::CreateBitmapFromFile
_TEXT	ENDS
PUBLIC	?CreateHBITMAPFromBitmap@CGdiPlus@@SAHPAPAXPAPAUHBITMAP__@@K@Z ; CGdiPlus::CreateHBITMAPFromBitmap
_BSS	SEGMENT
	ALIGN	4

_?pFN@?3??CreateHBITMAPFromBitmap@CGdiPlus@@SAHPAPAXPAPAUHBITMAP__@@K@Z@4P6G?AW4gdix_Status@@01K@ZA DD 01H DUP (?)
_?$S271@?3??CreateHBITMAPFromBitmap@CGdiPlus@@SAHPAPAXPAPAUHBITMAP__@@K@Z@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\GdiPlus.cpp
data	SEGMENT
$SG73372 DB	'GdipCreateHBITMAPFromBitmap', 00H
data	ENDS
;	COMDAT ?CreateHBITMAPFromBitmap@CGdiPlus@@SAHPAPAXPAPAUHBITMAP__@@K@Z
_TEXT	SEGMENT
_bitmap$ = 8
_hbmReturn$ = 12
_background$ = 16
?CreateHBITMAPFromBitmap@CGdiPlus@@SAHPAPAXPAPAUHBITMAP__@@K@Z PROC NEAR ; CGdiPlus::CreateHBITMAPFromBitmap, COMDAT

; 182  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 183  : 	if (!Initialize())

	call	?Initialize@CGdiPlus@@KAHXZ		; CGdiPlus::Initialize
	test	eax, eax
	jne	SHORT $L73365

; 184  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L73364
$L73365:

; 185  : 
; 186  : 	static PFNCREATEHBITMAPFROMBITMAP pFN = (PFNCREATEHBITMAPFROMBITMAP)GetProcAddress(s_hGdiPlus, "GdipCreateHBITMAPFromBitmap");

	xor	eax, eax
	mov	al, BYTE PTR _?$S271@?3??CreateHBITMAPFromBitmap@CGdiPlus@@SAHPAPAXPAPAUHBITMAP__@@K@Z@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L73371
	mov	cl, BYTE PTR _?$S271@?3??CreateHBITMAPFromBitmap@CGdiPlus@@SAHPAPAXPAPAUHBITMAP__@@K@Z@4EA
	or	cl, 1
	mov	BYTE PTR _?$S271@?3??CreateHBITMAPFromBitmap@CGdiPlus@@SAHPAPAXPAPAUHBITMAP__@@K@Z@4EA, cl
	mov	esi, esp
	push	OFFSET FLAT:$SG73372
	mov	edx, DWORD PTR ?s_hGdiPlus@CGdiPlus@@1PAUHINSTANCE__@@A ; CGdiPlus::s_hGdiPlus
	push	edx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?pFN@?3??CreateHBITMAPFromBitmap@CGdiPlus@@SAHPAPAXPAPAUHBITMAP__@@K@Z@4P6G?AW4gdix_Status@@01K@ZA, eax
$L73371:

; 187  : 
; 188  : 	if (pFN)

	cmp	DWORD PTR _?pFN@?3??CreateHBITMAPFromBitmap@CGdiPlus@@SAHPAPAXPAPAUHBITMAP__@@K@Z@4P6G?AW4gdix_Status@@01K@ZA, 0
	je	SHORT $L73373

; 189  : 		return (pFN(bitmap, hbmReturn, background) == gdix_Ok);

	mov	esi, esp
	mov	eax, DWORD PTR _background$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hbmReturn$[ebp]
	push	ecx
	mov	edx, DWORD PTR _bitmap$[ebp]
	push	edx
	call	DWORD PTR _?pFN@?3??CreateHBITMAPFromBitmap@CGdiPlus@@SAHPAPAXPAPAUHBITMAP__@@K@Z@4P6G?AW4gdix_Status@@01K@ZA
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax
	jmp	SHORT $L73364
$L73373:

; 190  : 	
; 191  : 	// else
; 192  : 	return FALSE;

	xor	eax, eax
$L73364:

; 193  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?CreateHBITMAPFromBitmap@CGdiPlus@@SAHPAPAXPAPAUHBITMAP__@@K@Z ENDP ; CGdiPlus::CreateHBITMAPFromBitmap
_TEXT	ENDS
END

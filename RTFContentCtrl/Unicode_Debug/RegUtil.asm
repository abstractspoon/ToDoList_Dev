	TITLE	D:\_CODE\3rdParty\RegUtil.cpp
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
;	COMDAT ?CopyKeys@@YAHPAUHKEY__@@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CopyValues@@YAHPAUHKEY__@@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CopyRegistryKey@@YAHPAUHKEY__@@ABVCString@@01@Z
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
PUBLIC	?CopyRegistryKey@@YAHPAUHKEY__@@ABVCString@@01@Z ; CopyRegistryKey
EXTRN	__imp__RegCloseKey@4:NEAR
EXTRN	__imp__RegCreateKeyExW@36:NEAR
EXTRN	__imp__RegOpenKeyExW@20:NEAR
EXTRN	__chkesp:NEAR
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
;	COMDAT ?CopyRegistryKey@@YAHPAUHKEY__@@ABVCString@@01@Z
_TEXT	SEGMENT
_hkRootFrom$ = 8
_strFromPath$ = 12
_hkRootTo$ = 16
_strToPath$ = 20
_hkFrom$ = -8
_res$ = -4
_hkTo$ = -12
_bRes$ = -16
?CopyRegistryKey@@YAHPAUHKEY__@@ABVCString@@01@Z PROC NEAR ; CopyRegistryKey, COMDAT

; 59   : {

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

; 60   : 	HKEY hkFrom;
; 61   : 	LONG res = ::RegOpenKeyEx(hkRootFrom, strFromPath, 0, KEY_READ, &hkFrom);

	mov	esi, esp
	lea	eax, DWORD PTR _hkFrom$[ebp]
	push	eax
	push	131097					; 00020019H
	push	0
	mov	ecx, DWORD PTR _strFromPath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _hkRootFrom$[ebp]
	push	ecx
	call	DWORD PTR __imp__RegOpenKeyExW@20
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _res$[ebp], eax

; 62   : 	if (ERROR_SUCCESS != res) {

	cmp	DWORD PTR _res$[ebp], 0
	je	SHORT $L73106

; 63   : 		return FALSE;	

	xor	eax, eax
	jmp	$L73103
$L73106:

; 66   : 	res = ::RegCreateKeyEx(hkRootTo, strToPath, 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hkTo, 0);

	mov	esi, esp
	push	0
	lea	edx, DWORD PTR _hkTo$[ebp]
	push	edx
	push	0
	push	131078					; 00020006H
	push	0
	push	0
	push	0
	mov	ecx, DWORD PTR _strToPath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	eax, DWORD PTR _hkRootTo$[ebp]
	push	eax
	call	DWORD PTR __imp__RegCreateKeyExW@36
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _res$[ebp], eax

; 67   : 	if (ERROR_SUCCESS != res) {

	cmp	DWORD PTR _res$[ebp], 0
	je	SHORT $L73108

; 68   : 		::RegCloseKey(hkFrom);

	mov	esi, esp
	mov	ecx, DWORD PTR _hkFrom$[ebp]
	push	ecx
	call	DWORD PTR __imp__RegCloseKey@4
	cmp	esi, esp
	call	__chkesp

; 69   : 		return FALSE;	

	xor	eax, eax
	jmp	SHORT $L73103
$L73108:

; 71   : 	BOOL bRes = CopyKeys(hkFrom, hkTo) && CopyValues(hkFrom, hkTo);

	mov	edx, DWORD PTR _hkTo$[ebp]
	push	edx
	mov	eax, DWORD PTR _hkFrom$[ebp]
	push	eax
	call	?CopyKeys@@YAHPAUHKEY__@@0@Z		; CopyKeys
	add	esp, 8
	test	eax, eax
	je	SHORT $L73224
	mov	ecx, DWORD PTR _hkTo$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hkFrom$[ebp]
	push	edx
	call	?CopyValues@@YAHPAUHKEY__@@0@Z		; CopyValues
	add	esp, 8
	test	eax, eax
	je	SHORT $L73224
	mov	DWORD PTR -20+[ebp], 1
	jmp	SHORT $L73225
$L73224:
	mov	DWORD PTR -20+[ebp], 0
$L73225:
	mov	eax, DWORD PTR -20+[ebp]
	mov	DWORD PTR _bRes$[ebp], eax

; 72   : 
; 73   : 	::RegCloseKey(hkFrom);

	mov	esi, esp
	mov	ecx, DWORD PTR _hkFrom$[ebp]
	push	ecx
	call	DWORD PTR __imp__RegCloseKey@4
	cmp	esi, esp
	call	__chkesp

; 74   : 	::RegCloseKey(hkTo);

	mov	esi, esp
	mov	edx, DWORD PTR _hkTo$[ebp]
	push	edx
	call	DWORD PTR __imp__RegCloseKey@4
	cmp	esi, esp
	call	__chkesp

; 75   : 
; 76   : 	return bRes;

	mov	eax, DWORD PTR _bRes$[ebp]
$L73103:

; 77   : }

	pop	esi
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?CopyRegistryKey@@YAHPAUHKEY__@@ABVCString@@01@Z ENDP	; CopyRegistryKey
_TEXT	ENDS
EXTRN	__imp__RegEnumKeyExW@32:NEAR
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\RegUtil.cpp
xdata$x	SEGMENT
$T73236	DD	019930520H
	DD	02H
	DD	FLAT:$T73239
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73239	DD	0ffffffffH
	DD	FLAT:$L73231
	DD	00H
	DD	FLAT:$L73232
xdata$x	ENDS
;	COMDAT ?CopyKeys@@YAHPAUHKEY__@@0@Z
_TEXT	SEGMENT
_hkFrom$ = 8
_hkTo$ = 12
_lpstrName$ = -1056
_lpstrClass$ = -532
_i$ = -536
_nNameSize$73072 = -1068
_nClassSize$73073 = -1072
_res$73074 = -1064
_hkNew$73076 = -1060
_bRes$73078 = -1076
$T73229 = -1080
$T73230 = -1084
__$EHRec$ = -12
?CopyKeys@@YAHPAUHKEY__@@0@Z PROC NEAR			; CopyKeys, COMDAT

; 10   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73237
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 1072				; 00000430H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-1084]
	mov	ecx, 268				; 0000010cH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 11   : 	TCHAR lpstrName[nMaxKeyNameSize];
; 12   : 	TCHAR lpstrClass[nMaxClassSize];
; 13   : 
; 14   : 	for (int i = 0;;i++) {

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L73069
$L73070:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L73069:

; 15   : 		DWORD nNameSize = nMaxKeyNameSize;

	mov	DWORD PTR _nNameSize$73072[ebp], 260	; 00000104H

; 16   : 		DWORD nClassSize = nMaxClassSize;

	mov	DWORD PTR _nClassSize$73073[ebp], 260	; 00000104H

; 17   : 		LONG res = ::RegEnumKeyEx(hkFrom, i, lpstrName, &nNameSize, 0, lpstrClass, &nClassSize, 0);

	mov	esi, esp
	push	0
	lea	ecx, DWORD PTR _nClassSize$73073[ebp]
	push	ecx
	lea	edx, DWORD PTR _lpstrClass$[ebp]
	push	edx
	push	0
	lea	eax, DWORD PTR _nNameSize$73072[ebp]
	push	eax
	lea	ecx, DWORD PTR _lpstrName$[ebp]
	push	ecx
	mov	edx, DWORD PTR _i$[ebp]
	push	edx
	mov	eax, DWORD PTR _hkFrom$[ebp]
	push	eax
	call	DWORD PTR __imp__RegEnumKeyExW@32
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _res$73074[ebp], eax

; 18   : 		if (ERROR_NO_MORE_ITEMS == res) {

	cmp	DWORD PTR _res$73074[ebp], 259		; 00000103H
	jne	SHORT $L73075

; 19   : 			break;

	jmp	$L73071
$L73075:

; 22   : 		res = ::RegCreateKeyEx(hkTo, lpstrName, 0, lpstrClass, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &hkNew, 0); ;

	mov	esi, esp
	push	0
	lea	ecx, DWORD PTR _hkNew$73076[ebp]
	push	ecx
	push	0
	push	983103					; 000f003fH
	push	0
	lea	edx, DWORD PTR _lpstrClass$[ebp]
	push	edx
	push	0
	lea	eax, DWORD PTR _lpstrName$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hkTo$[ebp]
	push	ecx
	call	DWORD PTR __imp__RegCreateKeyExW@36
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _res$73074[ebp], eax

; 23   : 		if (ERROR_SUCCESS != res) {

	cmp	DWORD PTR _res$73074[ebp], 0
	je	SHORT $L73077

; 24   : 			return FALSE;		

	xor	eax, eax
	jmp	$L73065
$L73077:

; 26   : 		::RegCloseKey(hkNew);

	mov	esi, esp
	mov	edx, DWORD PTR _hkNew$73076[ebp]
	push	edx
	call	DWORD PTR __imp__RegCloseKey@4
	cmp	esi, esp
	call	__chkesp

; 27   : 		BOOL bRes = CopyRegistryKey(hkFrom, lpstrName, hkTo, lpstrName);

	lea	eax, DWORD PTR _lpstrName$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T73229[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _lpstrName$[ebp]
	push	ecx
	lea	ecx, DWORD PTR $T73230[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	edx, DWORD PTR $T73229[ebp]
	push	edx
	mov	eax, DWORD PTR _hkTo$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T73230[ebp]
	push	ecx
	mov	edx, DWORD PTR _hkFrom$[ebp]
	push	edx
	call	?CopyRegistryKey@@YAHPAUHKEY__@@ABVCString@@01@Z ; CopyRegistryKey
	add	esp, 16					; 00000010H
	mov	DWORD PTR _bRes$73078[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T73230[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T73229[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 28   : 		if (! bRes) {

	cmp	DWORD PTR _bRes$73078[ebp], 0
	jne	SHORT $L73081

; 29   : 			return FALSE;		

	xor	eax, eax
	jmp	SHORT $L73065
$L73081:

; 31   : 	}

	jmp	$L73070
$L73071:

; 32   : 
; 33   : 	return TRUE;

	mov	eax, 1
$L73065:

; 34   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 1084				; 0000043cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73231:
	lea	ecx, DWORD PTR $T73229[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73232:
	lea	ecx, DWORD PTR $T73230[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73237:
	mov	eax, OFFSET FLAT:$T73236
	jmp	___CxxFrameHandler
text$x	ENDS
?CopyKeys@@YAHPAUHKEY__@@0@Z ENDP			; CopyKeys
EXTRN	__imp__RegEnumValueW@32:NEAR
EXTRN	__imp__RegSetValueExW@24:NEAR
EXTRN	__chkstk:NEAR
;	COMDAT ?CopyValues@@YAHPAUHKEY__@@0@Z
_TEXT	SEGMENT
_hkFrom$ = 8
_hkTo$ = 12
_lpstrName$ = -524
_pValue$ = -4620
_i$ = -4
_nType$73092 = -4624
_nNameSize$73093 = -4636
_nValueSize$73094 = -4632
_res$73095 = -4628
?CopyValues@@YAHPAUHKEY__@@0@Z PROC NEAR		; CopyValues, COMDAT

; 37   : {

	push	ebp
	mov	ebp, esp
	mov	eax, 4636				; 0000121cH
	call	__chkstk
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-4636]
	mov	ecx, 1159				; 00000487H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 38   : 	TCHAR lpstrName[nMaxValueNameSize];
; 39   : 	BYTE pValue[nMaxValueValueSize];
; 40   : 
; 41   : 	for (int i = 0;;i++) {

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L73089
$L73090:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L73089:

; 42   : 		DWORD nType;
; 43   : 		DWORD nNameSize = nMaxValueNameSize;

	mov	DWORD PTR _nNameSize$73093[ebp], 260	; 00000104H

; 44   : 		DWORD nValueSize = nMaxValueValueSize;

	mov	DWORD PTR _nValueSize$73094[ebp], 4096	; 00001000H

; 45   : 		LONG res = ::RegEnumValue(hkFrom, i, lpstrName, &nNameSize, 0, &nType, pValue, &nValueSize);

	mov	esi, esp
	lea	ecx, DWORD PTR _nValueSize$73094[ebp]
	push	ecx
	lea	edx, DWORD PTR _pValue$[ebp]
	push	edx
	lea	eax, DWORD PTR _nType$73092[ebp]
	push	eax
	push	0
	lea	ecx, DWORD PTR _nNameSize$73093[ebp]
	push	ecx
	lea	edx, DWORD PTR _lpstrName$[ebp]
	push	edx
	mov	eax, DWORD PTR _i$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hkFrom$[ebp]
	push	ecx
	call	DWORD PTR __imp__RegEnumValueW@32
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _res$73095[ebp], eax

; 46   : 		if (ERROR_NO_MORE_ITEMS == res) {

	cmp	DWORD PTR _res$73095[ebp], 259		; 00000103H
	jne	SHORT $L73096

; 47   : 			break;

	jmp	SHORT $L73091
$L73096:

; 49   : 		res = ::RegSetValueEx(hkTo, lpstrName, 0, nType, pValue, nValueSize);

	mov	esi, esp
	mov	edx, DWORD PTR _nValueSize$73094[ebp]
	push	edx
	lea	eax, DWORD PTR _pValue$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nType$73092[ebp]
	push	ecx
	push	0
	lea	edx, DWORD PTR _lpstrName$[ebp]
	push	edx
	mov	eax, DWORD PTR _hkTo$[ebp]
	push	eax
	call	DWORD PTR __imp__RegSetValueExW@24
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _res$73095[ebp], eax

; 50   : 		if (ERROR_SUCCESS != res) {

	cmp	DWORD PTR _res$73095[ebp], 0
	je	SHORT $L73097

; 51   : 			return FALSE;		

	xor	eax, eax
	jmp	SHORT $L73085
$L73097:

; 53   : 	}

	jmp	$L73090
$L73091:

; 54   : 
; 55   : 	return TRUE;

	mov	eax, 1
$L73085:

; 56   : }

	pop	edi
	pop	esi
	add	esp, 4636				; 0000121cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?CopyValues@@YAHPAUHKEY__@@0@Z ENDP			; CopyValues
_TEXT	ENDS
END

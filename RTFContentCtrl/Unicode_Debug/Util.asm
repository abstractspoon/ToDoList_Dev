	TITLE	D:\_CODE\3rdParty\rtf2html\Util.cpp
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
;	COMDAT ?LongToString@@YA?AVCString@@J@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?StringToLong@@YAJVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?StringToArray@@YAXABVCString@@AAVCStringArray@@@Z
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
_THIS_FILE DB	'D:\_CODE\3rdParty\rtf2html\Util.cpp', 00H
_DATA	ENDS
PUBLIC	?LongToString@@YA?AVCString@@J@Z		; LongToString
EXTRN	__except_list:DWORD
EXTRN	__chkesp:NEAR
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
EXTRN	?Format@CString@@QAAXPBGZZ:NEAR			; CString::Format
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT data
; File D:\_CODE\3rdParty\rtf2html\Util.cpp
data	SEGMENT
$SG73067 DB	'%', 00H, 'd', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73207	DD	019930520H
	DD	02H
	DD	FLAT:$T73211
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73211	DD	0ffffffffH
	DD	FLAT:$L73201
	DD	00H
	DD	FLAT:$L73200
xdata$x	ENDS
;	COMDAT ?LongToString@@YA?AVCString@@J@Z
_TEXT	SEGMENT
_newValue$ = 12
___$ReturnUdt$ = 8
_lvalue$ = -16
_str_rtc$ = -20
$T73202 = -24
__$EHRec$ = -12
?LongToString@@YA?AVCString@@J@Z PROC NEAR		; LongToString, COMDAT

; 14   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73208
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T73202[ebp], 0

; 15   : 
; 16   :    long lvalue;
; 17   :    CString str_rtc;

	lea	ecx, DWORD PTR _str_rtc$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 18   :    lvalue = newValue;

	mov	eax, DWORD PTR _newValue$[ebp]
	mov	DWORD PTR _lvalue$[ebp], eax

; 19   :    str_rtc.Format(_T("%d"),lvalue);

	mov	ecx, DWORD PTR _lvalue$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG73067
	lea	edx, DWORD PTR _str_rtc$[ebp]
	push	edx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 12					; 0000000cH

; 20   :    return str_rtc;

	lea	eax, DWORD PTR _str_rtc$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T73202[ebp]
	or	ecx, 1
	mov	DWORD PTR $T73202[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _str_rtc$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 21   : 
; 22   : } //LongToString()

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73200:
	lea	ecx, DWORD PTR _str_rtc$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73201:
	mov	eax, DWORD PTR $T73202[ebp]
	and	eax, 1
	test	eax, eax
	je	$L73203
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L73203:
	ret	0
$L73208:
	mov	eax, OFFSET FLAT:$T73207
	jmp	___CxxFrameHandler
text$x	ENDS
?LongToString@@YA?AVCString@@J@Z ENDP			; LongToString
PUBLIC	?StringToLong@@YAJVCString@@@Z			; StringToLong
EXTRN	__imp__strtol:NEAR
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\rtf2html\Util.cpp
xdata$x	SEGMENT
$T73222	DD	019930520H
	DD	02H
	DD	FLAT:$T73224
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73224	DD	0ffffffffH
	DD	FLAT:$L73219
	DD	00H
	DD	FLAT:$L73220
xdata$x	ENDS
;	COMDAT ?StringToLong@@YAJVCString@@@Z
_TEXT	SEGMENT
_newValue$ = 8
_lvalue$ = -28
_p_str$ = -20
_p_endstr$ = -24
_str$ = -16
$T73218 = -32
__$EHRec$ = -12
?StringToLong@@YAJVCString@@@Z PROC NEAR		; StringToLong, COMDAT

; 25   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73223
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
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 26   :    long lvalue;
; 27   :    char *p_str;
; 28   :    char *p_endstr;
; 29   :    CString str;

	lea	ecx, DWORD PTR _str$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 30   :    str = newValue;

	lea	eax, DWORD PTR _newValue$[ebp]
	push	eax
	lea	ecx, DWORD PTR _str$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=

; 31   :    p_str=(LPSTR) LPCTSTR(str);

	lea	ecx, DWORD PTR _str$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR _p_str$[ebp], eax

; 32   :    lvalue = strtol(p_str, &p_endstr, 10);

	mov	esi, esp
	push	10					; 0000000aH
	lea	ecx, DWORD PTR _p_endstr$[ebp]
	push	ecx
	mov	edx, DWORD PTR _p_str$[ebp]
	push	edx
	call	DWORD PTR __imp__strtol
	add	esp, 12					; 0000000cH
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lvalue$[ebp], eax

; 33   : 
; 34   :    return lvalue;

	mov	eax, DWORD PTR _lvalue$[ebp]
	mov	DWORD PTR $T73218[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _str$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _newValue$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T73218[ebp]

; 35   : 
; 36   : } //StringToLong()

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73219:
	lea	ecx, DWORD PTR _newValue$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73220:
	lea	ecx, DWORD PTR _str$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73223:
	mov	eax, OFFSET FLAT:$T73222
	jmp	___CxxFrameHandler
text$x	ENDS
?StringToLong@@YAJVCString@@@Z ENDP			; StringToLong
PUBLIC	?StringToArray@@YAXABVCString@@AAVCStringArray@@@Z ; StringToArray
EXTRN	?Add@CStringArray@@QAEHABVCString@@@Z:NEAR	; CStringArray::Add
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	?GetAt@CString@@QBEGH@Z:NEAR			; CString::GetAt
EXTRN	??4CString@@QAEABV0@PBD@Z:NEAR			; CString::operator=
EXTRN	??YCString@@QAEABV0@G@Z:NEAR			; CString::operator+=
EXTRN	?Find@CString@@QBEHG@Z:NEAR			; CString::Find
_BSS	SEGMENT
$SG73089 DB	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\3rdParty\rtf2html\Util.cpp
data	SEGMENT
$SG73082 DB	';', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73232	DD	019930520H
	DD	02H
	DD	FLAT:$T73234
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73234	DD	0ffffffffH
	DD	FLAT:$L73229
	DD	00H
	DD	FLAT:$L73230
xdata$x	ENDS
;	COMDAT ?StringToArray@@YAXABVCString@@AAVCStringArray@@@Z
_TEXT	SEGMENT
_strSrc$ = 8
_trgt$ = 12
_strDelimiter$ = -20
_strElement$ = -16
_lPosition$ = -24
__$EHRec$ = -12
?StringToArray@@YAXABVCString@@AAVCStringArray@@@Z PROC NEAR ; StringToArray, COMDAT

; 38   : void StringToArray(const CString& strSrc, CStringArray& trgt) {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73233
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH

; 39   :    CString strDelimiter=_T(";");

	push	OFFSET FLAT:$SG73082
	lea	ecx, DWORD PTR _strDelimiter$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 40   :    CString strElement;

	lea	ecx, DWORD PTR _strElement$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 41   :    long lPosition=0;

	mov	DWORD PTR _lPosition$[ebp], 0
$L73086:

; 42   : 
; 43   :    while (lPosition<strSrc.GetLength()) {

	mov	ecx, DWORD PTR _strSrc$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	cmp	DWORD PTR _lPosition$[ebp], eax
	jge	SHORT $L73087

; 44   :       if ((strDelimiter.Find(strSrc.GetAt(lPosition))>=0)) {

	mov	eax, DWORD PTR _lPosition$[ebp]
	push	eax
	mov	ecx, DWORD PTR _strSrc$[ebp]
	call	?GetAt@CString@@QBEGH@Z			; CString::GetAt
	push	eax
	lea	ecx, DWORD PTR _strDelimiter$[ebp]
	call	?Find@CString@@QBEHG@Z			; CString::Find
	test	eax, eax
	jl	SHORT $L73088

; 45   :          trgt.Add(strElement);

	lea	ecx, DWORD PTR _strElement$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _trgt$[ebp]
	call	?Add@CStringArray@@QAEHABVCString@@@Z	; CStringArray::Add

; 46   :          strElement = "";

	push	OFFSET FLAT:$SG73089
	lea	ecx, DWORD PTR _strElement$[ebp]
	call	??4CString@@QAEABV0@PBD@Z		; CString::operator=

; 47   :       } else {

	jmp	SHORT $L73090
$L73088:

; 48   :          strElement += strSrc.GetAt(lPosition);

	mov	edx, DWORD PTR _lPosition$[ebp]
	push	edx
	mov	ecx, DWORD PTR _strSrc$[ebp]
	call	?GetAt@CString@@QBEGH@Z			; CString::GetAt
	push	eax
	lea	ecx, DWORD PTR _strElement$[ebp]
	call	??YCString@@QAEABV0@G@Z			; CString::operator+=
$L73090:

; 50   :       lPosition++;

	mov	eax, DWORD PTR _lPosition$[ebp]
	add	eax, 1
	mov	DWORD PTR _lPosition$[ebp], eax

; 51   :    }

	jmp	SHORT $L73086
$L73087:

; 52   :    trgt.Add(strElement); // add last

	lea	ecx, DWORD PTR _strElement$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _trgt$[ebp]
	call	?Add@CStringArray@@QAEHABVCString@@@Z	; CStringArray::Add

; 53   : }

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _strElement$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _strDelimiter$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73229:
	lea	ecx, DWORD PTR _strDelimiter$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73230:
	lea	ecx, DWORD PTR _strElement$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73233:
	mov	eax, OFFSET FLAT:$T73232
	jmp	___CxxFrameHandler
text$x	ENDS
?StringToArray@@YAXABVCString@@AAVCStringArray@@@Z ENDP	; StringToArray
END

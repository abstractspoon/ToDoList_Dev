	TITLE	D:\_CODE\Shared\OSVersion.cpp
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
;	COMDAT ??0COSVersion@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCOSVersion@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1COSVersion@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FormatOSVersion@COSVersion@@QBE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ
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
;	COMDAT ??_7COSVersion@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\Shared\OSVersion.cpp', 00H
	ORG $+2
_?nVersion@?1??GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ@4W43@A DD 0ffffffffH
_DATA	ENDS
PUBLIC	??0COSVersion@@QAE@XZ				; COSVersion::COSVersion
PUBLIC	??_7COSVersion@@6B@				; COSVersion::`vftable'
PUBLIC	??_GCOSVersion@@UAEPAXI@Z			; COSVersion::`scalar deleting destructor'
PUBLIC	??_ECOSVersion@@UAEPAXI@Z			; COSVersion::`vector deleting destructor'
;	COMDAT ??_7COSVersion@@6B@
; File D:\_CODE\Shared\OSVersion.cpp
CONST	SEGMENT
??_7COSVersion@@6B@ DD FLAT:??_ECOSVersion@@UAEPAXI@Z	; COSVersion::`vftable'
CONST	ENDS
;	COMDAT ??0COSVersion@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0COSVersion@@QAE@XZ PROC NEAR				; COSVersion::COSVersion, COMDAT

; 19   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7COSVersion@@6B@ ; COSVersion::`vftable'

; 20   : 
; 21   : }

	mov	eax, DWORD PTR _this$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	0
??0COSVersion@@QAE@XZ ENDP				; COSVersion::COSVersion
_TEXT	ENDS
PUBLIC	??1COSVersion@@UAE@XZ				; COSVersion::~COSVersion
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
EXTRN	__chkesp:NEAR
;	COMDAT ??_GCOSVersion@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCOSVersion@@UAEPAXI@Z PROC NEAR			; COSVersion::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1COSVersion@@UAE@XZ			; COSVersion::~COSVersion
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73093
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73093:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCOSVersion@@UAEPAXI@Z ENDP				; COSVersion::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1COSVersion@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1COSVersion@@UAE@XZ PROC NEAR				; COSVersion::~COSVersion, COMDAT

; 24   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7COSVersion@@6B@ ; COSVersion::`vftable'

; 25   : 
; 26   : }

	mov	esp, ebp
	pop	ebp
	ret	0
??1COSVersion@@UAE@XZ ENDP				; COSVersion::~COSVersion
_TEXT	ENDS
PUBLIC	?FormatOSVersion@COSVersion@@QBE?AVCString@@XZ	; COSVersion::FormatOSVersion
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
EXTRN	?Format@CString@@QAAXPBGZZ:NEAR			; CString::Format
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
EXTRN	__imp__GetVersionExW@4:NEAR
;	COMDAT data
; File D:\_CODE\Shared\OSVersion.cpp
data	SEGMENT
$SG73106 DB	'%', 00H, 'l', 00H, 'u', 00H, '.', 00H, '%', 00H, 'l', 00H
	DB	'u', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73297	DD	019930520H
	DD	02H
	DD	FLAT:$T73300
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73300	DD	0ffffffffH
	DD	FLAT:$L73291
	DD	00H
	DD	FLAT:$L73290
xdata$x	ENDS
;	COMDAT ?FormatOSVersion@COSVersion@@QBE?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_this$ = -304
_sVersion$ = -16
_vinfo$ = -296
_rslt$ = -20
$T73292 = -300
__$EHRec$ = -12
?FormatOSVersion@COSVersion@@QBE?AVCString@@XZ PROC NEAR ; COSVersion::FormatOSVersion, COMDAT

; 29   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73298
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 292				; 00000124H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-304]
	mov	ecx, 73					; 00000049H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T73292[ebp], 0

; 30   : 	CString sVersion;

	lea	ecx, DWORD PTR _sVersion$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 31   : 	OSVERSIONINFO vinfo = { sizeof(OSVERSIONINFO) };

	mov	DWORD PTR _vinfo$[ebp], 276		; 00000114H
	mov	ecx, 68					; 00000044H
	xor	eax, eax
	lea	edi, DWORD PTR _vinfo$[ebp+4]
	rep stosd

; 32   : 
; 33   : 	BOOL rslt = GetVersionEx(&vinfo);

	mov	esi, esp
	lea	eax, DWORD PTR _vinfo$[ebp]
	push	eax
	call	DWORD PTR __imp__GetVersionExW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _rslt$[ebp], eax

; 34   : 
; 35   : 	if (rslt)

	cmp	DWORD PTR _rslt$[ebp], 0
	je	SHORT $L73105

; 36   : 		sVersion.Format(_T("%lu.%lu"), vinfo.dwMajorVersion, vinfo.dwMinorVersion);

	mov	ecx, DWORD PTR _vinfo$[ebp+8]
	push	ecx
	mov	edx, DWORD PTR _vinfo$[ebp+4]
	push	edx
	push	OFFSET FLAT:$SG73106
	lea	eax, DWORD PTR _sVersion$[ebp]
	push	eax
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 16					; 00000010H
$L73105:

; 37   : 	
; 38   : 	return sVersion;

	lea	ecx, DWORD PTR _sVersion$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T73292[ebp]
	or	edx, 1
	mov	DWORD PTR $T73292[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sVersion$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 39   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 304				; 00000130H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73290:
	lea	ecx, DWORD PTR _sVersion$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73291:
	mov	eax, DWORD PTR $T73292[ebp]
	and	eax, 1
	test	eax, eax
	je	$L73293
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L73293:
	ret	0
$L73298:
	mov	eax, OFFSET FLAT:$T73297
	jmp	___CxxFrameHandler
text$x	ENDS
?FormatOSVersion@COSVersion@@QBE?AVCString@@XZ ENDP	; COSVersion::FormatOSVersion
PUBLIC	?GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ	; COSVersion::GetOSVersion
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
;	COMDAT ?GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ
_TEXT	SEGMENT
_this$ = -284
_vinfo$73113 = -280
_rslt$73115 = -4
?GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ PROC NEAR ; COSVersion::GetOSVersion, COMDAT

; 42   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 304				; 00000130H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-304]
	mov	ecx, 76					; 0000004cH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 43   : 	static OSVERSION nVersion = OSV_UNKNOWN;
; 44   : 
; 45   : 	if (nVersion == OSV_UNKNOWN) // first time only

	cmp	DWORD PTR _?nVersion@?1??GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ@4W43@A, -1
	jne	$L73118

; 47   : 		OSVERSIONINFO vinfo;
; 48   : 		vinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	mov	DWORD PTR _vinfo$73113[ebp], 276	; 00000114H

; 49   : 		
; 50   : 		BOOL rslt = GetVersionEx(&vinfo);

	mov	esi, esp
	lea	eax, DWORD PTR _vinfo$73113[ebp]
	push	eax
	call	DWORD PTR __imp__GetVersionExW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _rslt$73115[ebp], eax

; 51   : 		
; 52   : 		if (rslt)

	cmp	DWORD PTR _rslt$73115[ebp], 0
	je	$L73118

; 55   : 			{

	mov	ecx, DWORD PTR _vinfo$73113[ebp+16]
	mov	DWORD PTR -288+[ebp], ecx
	cmp	DWORD PTR -288+[ebp], 1
	je	$L73149
	cmp	DWORD PTR -288+[ebp], 2
	je	SHORT $L73121
	jmp	$L73166
$L73121:

; 59   : 					{

	mov	edx, DWORD PTR _vinfo$73113[ebp+4]
	mov	DWORD PTR -292+[ebp], edx
	mov	eax, DWORD PTR -292+[ebp]
	sub	eax, 3
	mov	DWORD PTR -292+[ebp], eax
	cmp	DWORD PTR -292+[ebp], 3
	ja	$L73148
	mov	ecx, DWORD PTR -292+[ebp]
	jmp	DWORD PTR $L73307[ecx*4]
$L73126:

; 60   : 					case 3: // nt351
; 61   : 						ASSERT (0); // not supported

	mov	edx, 1
	test	edx, edx
	je	SHORT $L73130
	push	61					; 0000003dH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73130
	int	3
$L73130:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73126

; 62   : 						break;

	jmp	$L73123
$L73131:

; 63   : 						
; 64   : 					case 4: // nt4
; 65   : 						nVersion = OSV_NT4;

	mov	DWORD PTR _?nVersion@?1??GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ@4W43@A, 3

; 66   : 						break;

	jmp	$L73123
$L73132:

; 71   : 							{

	mov	ecx, DWORD PTR _vinfo$73113[ebp+8]
	mov	DWORD PTR -296+[ebp], ecx
	cmp	DWORD PTR -296+[ebp], 0
	je	SHORT $L73137
	cmp	DWORD PTR -296+[ebp], 1
	je	SHORT $L73138
	jmp	SHORT $L73139
$L73137:

; 72   : 							case 0: // w2k
; 73   : 								nVersion = OSV_2K;

	mov	DWORD PTR _?nVersion@?1??GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ@4W43@A, 4

; 74   : 								break;

	jmp	SHORT $L73134
$L73138:

; 75   : 								
; 76   : 							case 1: // xp
; 77   : 								nVersion = OSV_XP;

	mov	DWORD PTR _?nVersion@?1??GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ@4W43@A, 5

; 78   : 								break;

	jmp	SHORT $L73134
$L73139:

; 79   : 								
; 80   : 							default: // > xp
; 81   : 								nVersion = OSV_XPP;

	mov	DWORD PTR _?nVersion@?1??GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ@4W43@A, 6
$L73134:

; 85   : 						break;

	jmp	SHORT $L73123
$L73140:

; 90   : 							{

	mov	edx, DWORD PTR _vinfo$73113[ebp+8]
	mov	DWORD PTR -300+[ebp], edx
	cmp	DWORD PTR -300+[ebp], 0
	je	SHORT $L73145
	cmp	DWORD PTR -300+[ebp], 1
	je	SHORT $L73146
	jmp	SHORT $L73147
$L73145:

; 91   : 							case 0: // vista
; 92   : 								nVersion = OSV_VISTA;

	mov	DWORD PTR _?nVersion@?1??GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ@4W43@A, 7

; 93   : 								break;

	jmp	SHORT $L73142
$L73146:

; 94   : 								
; 95   : 							case 1: // w7
; 96   : 								nVersion = OSV_WIN7;

	mov	DWORD PTR _?nVersion@?1??GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ@4W43@A, 8

; 97   : 								break;

	jmp	SHORT $L73142
$L73147:

; 98   : 								
; 99   : 							default: // > w7
; 100  : 								nVersion = OSV_WIN7P;

	mov	DWORD PTR _?nVersion@?1??GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ@4W43@A, 9
$L73142:

; 104  : 						break;

	jmp	SHORT $L73123
$L73148:

; 105  : 						
; 106  : 					default: // > w7
; 107  : 						nVersion = OSV_WIN7P;

	mov	DWORD PTR _?nVersion@?1??GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ@4W43@A, 9
$L73123:

; 111  : 				break;

	jmp	$L73118
$L73149:

; 115  : 					ASSERT (vinfo.dwMajorVersion == 4);

	cmp	DWORD PTR _vinfo$73113[ebp+4], 4
	je	SHORT $L73153
	push	115					; 00000073H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73153
	int	3
$L73153:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73149

; 118  : 					{

	mov	ecx, DWORD PTR _vinfo$73113[ebp+8]
	mov	DWORD PTR -304+[ebp], ecx
	cmp	DWORD PTR -304+[ebp], 0
	je	SHORT $L73158
	cmp	DWORD PTR -304+[ebp], 10		; 0000000aH
	je	SHORT $L73159
	cmp	DWORD PTR -304+[ebp], 90		; 0000005aH
	je	SHORT $L73160
	jmp	SHORT $L73161
$L73158:

; 119  : 					case 0: 
; 120  : 						nVersion = OSV_95;

	mov	DWORD PTR _?nVersion@?1??GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ@4W43@A, 0

; 121  : 						break;

	jmp	SHORT $L73155
$L73159:

; 122  : 						
; 123  : 					case 10:
; 124  : 						nVersion = OSV_98;

	mov	DWORD PTR _?nVersion@?1??GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ@4W43@A, 1

; 125  : 						break;

	jmp	SHORT $L73155
$L73160:

; 126  : 						
; 127  : 					case 90:
; 128  : 						nVersion = OSV_ME;

	mov	DWORD PTR _?nVersion@?1??GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ@4W43@A, 2

; 129  : 						break;

	jmp	SHORT $L73155
$L73161:

; 130  : 						
; 131  : 					default:
; 132  : 						ASSERT (0);

	mov	edx, 1
	test	edx, edx
	je	SHORT $L73165
	push	132					; 00000084H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73165
	int	3
$L73165:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73161
$L73155:

; 137  : 				break;

	jmp	SHORT $L73118
$L73166:

; 138  : 				
; 139  : 			default:
; 140  : 				ASSERT (0);

	mov	ecx, 1
	test	ecx, ecx
	je	SHORT $L73170
	push	140					; 0000008cH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73170
	int	3
$L73170:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L73166
$L73118:

; 145  : 
; 146  : 	return nVersion;

	mov	eax, DWORD PTR _?nVersion@?1??GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ@4W43@A

; 147  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 304				; 00000130H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
$L73307:
	DD	$L73126
	DD	$L73131
	DD	$L73132
	DD	$L73140
?GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ ENDP	; COSVersion::GetOSVersion
_TEXT	ENDS
END

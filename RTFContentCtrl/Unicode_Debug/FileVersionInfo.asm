	TITLE	D:\_CODE\3rdParty\FileVersionInfo.cpp
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
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??0CFileVersionInfo@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCFileVersionInfo@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CFileVersionInfo@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTranslationId@CFileVersionInfo@@IAEHPAXIGAAKH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Create@CFileVersionInfo@@QAEHPAUHINSTANCE__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Create@CFileVersionInfo@@QAEHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileVersion@CFileVersionInfo@@QBEGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetProductVersion@CFileVersionInfo@@QBEGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileFlagsMask@CFileVersionInfo@@QBEKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileFlags@CFileVersionInfo@@QBEKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileOs@CFileVersionInfo@@QBEKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileType@CFileVersionInfo@@QBEKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileSubtype@CFileVersionInfo@@QBEKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileDate@CFileVersionInfo@@QBE?AVCTime@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetCompanyName@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileDescription@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileVersion@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetInternalName@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetLegalCopyright@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetOriginalFileName@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetProductName@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetProductVersion@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetComments@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetLegalTrademarks@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetPrivateBuild@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSpecialBuild@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Reset@CFileVersionInfo@@MAEXXZ
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
;	COMDAT __CTA1H
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0H@84
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0H@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __TI1H
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_7CFileVersionInfo@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS, xdata$x
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\3rdParty\FileVersionInfo.cpp', 00H
_DATA	ENDS
PUBLIC	??0CFileVersionInfo@@QAE@XZ			; CFileVersionInfo::CFileVersionInfo
PUBLIC	?Reset@CFileVersionInfo@@MAEXXZ			; CFileVersionInfo::Reset
PUBLIC	??_7CFileVersionInfo@@6B@			; CFileVersionInfo::`vftable'
PUBLIC	??_GCFileVersionInfo@@UAEPAXI@Z			; CFileVersionInfo::`scalar deleting destructor'
PUBLIC	??_ECFileVersionInfo@@UAEPAXI@Z			; CFileVersionInfo::`vector deleting destructor'
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	__except_list:DWORD
EXTRN	__chkesp:NEAR
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT ??_7CFileVersionInfo@@6B@
; File D:\_CODE\3rdParty\FileVersionInfo.cpp
CONST	SEGMENT
??_7CFileVersionInfo@@6B@ DD FLAT:??_ECFileVersionInfo@@UAEPAXI@Z ; CFileVersionInfo::`vftable'
	DD	FLAT:?Reset@CFileVersionInfo@@MAEXXZ
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73560	DD	019930520H
	DD	0cH
	DD	FLAT:$T73564
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73564	DD	0ffffffffH
	DD	FLAT:$L73545
	DD	00H
	DD	FLAT:$L73546
	DD	01H
	DD	FLAT:$L73547
	DD	02H
	DD	FLAT:$L73548
	DD	03H
	DD	FLAT:$L73549
	DD	04H
	DD	FLAT:$L73550
	DD	05H
	DD	FLAT:$L73551
	DD	06H
	DD	FLAT:$L73552
	DD	07H
	DD	FLAT:$L73553
	DD	08H
	DD	FLAT:$L73554
	DD	09H
	DD	FLAT:$L73555
	DD	0aH
	DD	FLAT:$L73556
xdata$x	ENDS
;	COMDAT ??0CFileVersionInfo@@QAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??0CFileVersionInfo@@QAE@XZ PROC NEAR			; CFileVersionInfo::CFileVersionInfo, COMDAT

; 34   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73561
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 56					; 00000038H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 68					; 00000044H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 76					; 0000004cH
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 80					; 00000050H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 84					; 00000054H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 88					; 00000058H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 8
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 92					; 0000005cH
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 9
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 96					; 00000060H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 10		; 0000000aH
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 100				; 00000064H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 11		; 0000000bH
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CFileVersionInfo@@6B@ ; CFileVersionInfo::`vftable'

; 35   : 	Reset();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Reset@CFileVersionInfo@@MAEXXZ		; CFileVersionInfo::Reset

; 36   : }

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
$L73545:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 56					; 00000038H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73546:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73547:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73548:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 68					; 00000044H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73549:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73550:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 76					; 0000004cH
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73551:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 80					; 00000050H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73552:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 84					; 00000054H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73553:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 88					; 00000058H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73554:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 92					; 0000005cH
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73555:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 96					; 00000060H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73556:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 100				; 00000064H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73561:
	mov	eax, OFFSET FLAT:$T73560
	jmp	___CxxFrameHandler
text$x	ENDS
??0CFileVersionInfo@@QAE@XZ ENDP			; CFileVersionInfo::CFileVersionInfo
PUBLIC	??1CFileVersionInfo@@UAE@XZ			; CFileVersionInfo::~CFileVersionInfo
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCFileVersionInfo@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCFileVersionInfo@@UAEPAXI@Z PROC NEAR		; CFileVersionInfo::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CFileVersionInfo@@UAE@XZ		; CFileVersionInfo::~CFileVersionInfo
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73125
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73125:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCFileVersionInfo@@UAEPAXI@Z ENDP			; CFileVersionInfo::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\FileVersionInfo.cpp
xdata$x	SEGMENT
$T73585	DD	019930520H
	DD	0bH
	DD	FLAT:$T73587
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73587	DD	0ffffffffH
	DD	FLAT:$L73572
	DD	00H
	DD	FLAT:$L73573
	DD	01H
	DD	FLAT:$L73574
	DD	02H
	DD	FLAT:$L73575
	DD	03H
	DD	FLAT:$L73576
	DD	04H
	DD	FLAT:$L73577
	DD	05H
	DD	FLAT:$L73578
	DD	06H
	DD	FLAT:$L73579
	DD	07H
	DD	FLAT:$L73580
	DD	08H
	DD	FLAT:$L73581
	DD	09H
	DD	FLAT:$L73582
xdata$x	ENDS
;	COMDAT ??1CFileVersionInfo@@UAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1CFileVersionInfo@@UAE@XZ PROC NEAR			; CFileVersionInfo::~CFileVersionInfo, COMDAT

; 40   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73586
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CFileVersionInfo@@6B@ ; CFileVersionInfo::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 10		; 0000000aH

; 41   : }

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 100				; 00000064H
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 9
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 96					; 00000060H
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 8
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 92					; 0000005cH
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 88					; 00000058H
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 84					; 00000054H
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 80					; 00000050H
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 76					; 0000004cH
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 68					; 00000044H
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 56					; 00000038H
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
$L73572:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 56					; 00000038H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73573:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73574:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73575:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 68					; 00000044H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73576:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73577:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 76					; 0000004cH
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73578:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 80					; 00000050H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73579:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 84					; 00000054H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73580:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 88					; 00000058H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73581:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 92					; 0000005cH
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73582:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 96					; 00000060H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73586:
	mov	eax, OFFSET FLAT:$T73585
	jmp	___CxxFrameHandler
text$x	ENDS
??1CFileVersionInfo@@UAE@XZ ENDP			; CFileVersionInfo::~CFileVersionInfo
PUBLIC	?GetTranslationId@CFileVersionInfo@@IAEHPAXIGAAKH@Z ; CFileVersionInfo::GetTranslationId
;	COMDAT ?GetTranslationId@CFileVersionInfo@@IAEHPAXIGAAKH@Z
_TEXT	SEGMENT
_lpData$ = 8
_unBlockSize$ = 12
_wLangId$ = 16
_dwId$ = 20
_bPrimaryEnough$ = 24
_this$ = -8
_lpwData$ = -4
?GetTranslationId@CFileVersionInfo@@IAEHPAXIGAAKH@Z PROC NEAR ; CFileVersionInfo::GetTranslationId, COMDAT

; 45   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 46   : 	LPWORD lpwData;
; 47   : 
; 48   : 	for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData)+unBlockSize; lpwData+=2)

	mov	eax, DWORD PTR _lpData$[ebp]
	mov	DWORD PTR _lpwData$[ebp], eax
	jmp	SHORT $L73141
$L73142:
	mov	ecx, DWORD PTR _lpwData$[ebp]
	add	ecx, 4
	mov	DWORD PTR _lpwData$[ebp], ecx
$L73141:
	mov	edx, DWORD PTR _lpData$[ebp]
	add	edx, DWORD PTR _unBlockSize$[ebp]
	cmp	DWORD PTR _lpwData$[ebp], edx
	jae	SHORT $L73143

; 50   : 		if (*lpwData == wLangId)

	mov	eax, DWORD PTR _lpwData$[ebp]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax]
	mov	edx, DWORD PTR _wLangId$[ebp]
	and	edx, 65535				; 0000ffffH
	cmp	ecx, edx
	jne	SHORT $L73144

; 52   : 			dwId = *((DWORD*)lpwData);

	mov	eax, DWORD PTR _dwId$[ebp]
	mov	ecx, DWORD PTR _lpwData$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR [eax], edx

; 53   : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L73136
$L73144:

; 55   : 	}

	jmp	SHORT $L73142
$L73143:

; 56   : 
; 57   : 	if (!bPrimaryEnough)

	cmp	DWORD PTR _bPrimaryEnough$[ebp], 0
	jne	SHORT $L73146

; 58   : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L73136
$L73146:

; 59   : 
; 60   : 	for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData)+unBlockSize; lpwData+=2)

	mov	eax, DWORD PTR _lpData$[ebp]
	mov	DWORD PTR _lpwData$[ebp], eax
	jmp	SHORT $L73150
$L73151:
	mov	ecx, DWORD PTR _lpwData$[ebp]
	add	ecx, 4
	mov	DWORD PTR _lpwData$[ebp], ecx
$L73150:
	mov	edx, DWORD PTR _lpData$[ebp]
	add	edx, DWORD PTR _unBlockSize$[ebp]
	cmp	DWORD PTR _lpwData$[ebp], edx
	jae	SHORT $L73152

; 62   : 		if (((*lpwData)&0x00FF) == (wLangId&0x00FF))

	mov	eax, DWORD PTR _lpwData$[ebp]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax]
	and	ecx, 255				; 000000ffH
	mov	edx, DWORD PTR _wLangId$[ebp]
	and	edx, 65535				; 0000ffffH
	and	edx, 255				; 000000ffH
	cmp	ecx, edx
	jne	SHORT $L73153

; 64   : 			dwId = *((DWORD*)lpwData);

	mov	eax, DWORD PTR _dwId$[ebp]
	mov	ecx, DWORD PTR _lpwData$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR [eax], edx

; 65   : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L73136
$L73153:

; 67   : 	}

	jmp	SHORT $L73151
$L73152:

; 68   : 
; 69   : 	return FALSE;

	xor	eax, eax
$L73136:

; 70   : }

	mov	esp, ebp
	pop	ebp
	ret	20					; 00000014H
?GetTranslationId@CFileVersionInfo@@IAEHPAXIGAAKH@Z ENDP ; CFileVersionInfo::GetTranslationId
_TEXT	ENDS
PUBLIC	?Create@CFileVersionInfo@@QAEHPAUHINSTANCE__@@@Z ; CFileVersionInfo::Create
PUBLIC	?Create@CFileVersionInfo@@QAEHPBG@Z		; CFileVersionInfo::Create
EXTRN	__imp__GetModuleFileNameW@12:NEAR
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	?GetBuffer@CString@@QAEPAGH@Z:NEAR		; CString::GetBuffer
EXTRN	?ReleaseBuffer@CString@@QAEXH@Z:NEAR		; CString::ReleaseBuffer
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\FileVersionInfo.cpp
xdata$x	SEGMENT
$T73597	DD	019930520H
	DD	01H
	DD	FLAT:$T73599
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73599	DD	0ffffffffH
	DD	FLAT:$L73595
xdata$x	ENDS
;	COMDAT ?Create@CFileVersionInfo@@QAEHPAUHINSTANCE__@@@Z
_TEXT	SEGMENT
_hModule$ = 8
_this$ = -24
_strPath$ = -16
$T73594 = -20
__$EHRec$ = -12
?Create@CFileVersionInfo@@QAEHPAUHINSTANCE__@@@Z PROC NEAR ; CFileVersionInfo::Create, COMDAT

; 74   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73598
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 75   : 	CString	strPath;

	lea	ecx, DWORD PTR _strPath$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 76   : 	GetModuleFileName(hModule, strPath.GetBuffer(_MAX_PATH+1), _MAX_PATH);

	mov	esi, esp
	push	260					; 00000104H
	push	261					; 00000105H
	lea	ecx, DWORD PTR _strPath$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	mov	eax, DWORD PTR _hModule$[ebp]
	push	eax
	call	DWORD PTR __imp__GetModuleFileNameW@12
	cmp	esi, esp
	call	__chkesp

; 77   : 	strPath.ReleaseBuffer();

	push	-1
	lea	ecx, DWORD PTR _strPath$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 78   : 
; 79   : 	return Create(strPath);

	lea	ecx, DWORD PTR _strPath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Create@CFileVersionInfo@@QAEHPBG@Z	; CFileVersionInfo::Create
	mov	DWORD PTR $T73594[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _strPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T73594[ebp]

; 80   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73595:
	lea	ecx, DWORD PTR _strPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73598:
	mov	eax, OFFSET FLAT:$T73597
	jmp	___CxxFrameHandler
text$x	ENDS
?Create@CFileVersionInfo@@QAEHPAUHINSTANCE__@@@Z ENDP	; CFileVersionInfo::Create
PUBLIC	__TI1H
PUBLIC	__CTA1H
PUBLIC	??_R0H@8					; int `RTTI Type Descriptor'
PUBLIC	__CT??_R0H@84
EXTRN	_GetFileVersionInfoSizeW@8:NEAR
EXTRN	_GetFileVersionInfoW@16:NEAR
EXTRN	_VerQueryValueW@16:NEAR
EXTRN	__imp__GetUserDefaultLangID@0:NEAR
EXTRN	??_7type_info@@6B@:BYTE				; type_info::`vftable'
EXTRN	_memcpy:NEAR
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	??4CString@@QAEABV0@PBG@Z:NEAR			; CString::operator=
EXTRN	??H@YG?AVCString@@ABV0@PBG@Z:NEAR		; operator+
EXTRN	??2@YAPAXIPBDH@Z:NEAR				; operator new
EXTRN	?Replace@CString@@QAEHPBG0@Z:NEAR		; CString::Replace
EXTRN	__CxxThrowException@8:NEAR
EXTRN	?Format@CString@@QAAXPBGZZ:NEAR			; CString::Format
_BSS	SEGMENT
$SG73226 DW	01H DUP (?)
	ALIGN	4

$SG73265 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT ??_R0H@8
; File D:\_CODE\3rdParty\FileVersionInfo.cpp
_DATA	SEGMENT
??_R0H@8 DD	FLAT:??_7type_info@@6B@			; int `RTTI Type Descriptor'
	DD	00H
	DB	'.H', 00H
_DATA	ENDS
;	COMDAT __CT??_R0H@84
xdata$x	SEGMENT
__CT??_R0H@84 DD 01H
	DD	FLAT:??_R0H@8
	DD	00H
	DD	0ffffffffH
	ORG $+4
	DD	04H
	DD	00H
xdata$x	ENDS
;	COMDAT __CTA1H
xdata$x	SEGMENT
__CTA1H	DD	01H
	DD	FLAT:__CT??_R0H@84
xdata$x	ENDS
;	COMDAT __TI1H
xdata$x	SEGMENT
__TI1H	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:__CTA1H
xdata$x	ENDS
;	COMDAT data
data	SEGMENT
$SG73186 DB	'\', 00H, 00H, 00H
$SG73192 DB	'\', 00H, 'V', 00H, 'a', 00H, 'r', 00H, 'F', 00H, 'i', 00H
	DB	'l', 00H, 'e', 00H, 'I', 00H, 'n', 00H, 'f', 00H, 'o', 00H, '\'
	DB	00H, 'T', 00H, 'r', 00H, 'a', 00H, 'n', 00H, 's', 00H, 'l', 00H
	DB	'a', 00H, 't', 00H, 'i', 00H, 'o', 00H, 'n', 00H, 00H, 00H
	ORG $+2
$SG73204 DB	'\', 00H, 'S', 00H, 't', 00H, 'r', 00H, 'i', 00H, 'n', 00H
	DB	'g', 00H, 'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'I', 00H, 'n'
	DB	00H, 'f', 00H, 'o', 00H, '\', 00H, '%', 00H, '0', 00H, '4', 00H
	DB	'X', 00H, '%', 00H, '0', 00H, '4', 00H, 'X', 00H, '\', 00H, 00H
	DB	00H
$SG73210 DB	'C', 00H, 'o', 00H, 'm', 00H, 'p', 00H, 'a', 00H, 'n', 00H
	DB	'y', 00H, 'N', 00H, 'a', 00H, 'm', 00H, 'e', 00H, 00H, 00H
$SG73217 DB	'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'D', 00H, 'e', 00H
	DB	's', 00H, 'c', 00H, 'r', 00H, 'i', 00H, 'p', 00H, 't', 00H, 'i'
	DB	00H, 'o', 00H, 'n', 00H, 00H, 00H
$SG73224 DB	'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'V', 00H, 'e', 00H
	DB	'r', 00H, 's', 00H, 'i', 00H, 'o', 00H, 'n', 00H, 00H, 00H
$SG73227 DB	' ', 00H, 00H, 00H
$SG73228 DB	'.', 00H, 00H, 00H
$SG73229 DB	',', 00H, 00H, 00H
$SG73235 DB	'I', 00H, 'n', 00H, 't', 00H, 'e', 00H, 'r', 00H, 'n', 00H
	DB	'a', 00H, 'l', 00H, 'N', 00H, 'a', 00H, 'm', 00H, 'e', 00H, 00H
	DB	00H
	ORG $+2
$SG73242 DB	'L', 00H, 'e', 00H, 'g', 00H, 'a', 00H, 'l', 00H, 'C', 00H
	DB	'o', 00H, 'p', 00H, 'y', 00H, 'r', 00H, 'i', 00H, 'g', 00H, 'h'
	DB	00H, 't', 00H, 00H, 00H
	ORG $+2
$SG73249 DB	'O', 00H, 'r', 00H, 'i', 00H, 'g', 00H, 'i', 00H, 'n', 00H
	DB	'a', 00H, 'l', 00H, 'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'N'
	DB	00H, 'a', 00H, 'm', 00H, 'e', 00H, 00H, 00H
	ORG $+2
$SG73256 DB	'P', 00H, 'r', 00H, 'o', 00H, 'd', 00H, 'u', 00H, 'c', 00H
	DB	't', 00H, 'N', 00H, 'a', 00H, 'm', 00H, 'e', 00H, 00H, 00H
$SG73263 DB	'P', 00H, 'r', 00H, 'o', 00H, 'd', 00H, 'u', 00H, 'c', 00H
	DB	't', 00H, 'V', 00H, 'e', 00H, 'r', 00H, 's', 00H, 'i', 00H, 'o'
	DB	00H, 'n', 00H, 00H, 00H
	ORG $+2
$SG73266 DB	' ', 00H, 00H, 00H
$SG73267 DB	'.', 00H, 00H, 00H
$SG73268 DB	',', 00H, 00H, 00H
$SG73274 DB	'C', 00H, 'o', 00H, 'm', 00H, 'm', 00H, 'e', 00H, 'n', 00H
	DB	't', 00H, 's', 00H, 00H, 00H
	ORG $+2
$SG73281 DB	'L', 00H, 'e', 00H, 'g', 00H, 'a', 00H, 'l', 00H, 'T', 00H
	DB	'r', 00H, 'a', 00H, 'd', 00H, 'e', 00H, 'm', 00H, 'a', 00H, 'r'
	DB	00H, 'k', 00H, 's', 00H, 00H, 00H
$SG73288 DB	'P', 00H, 'r', 00H, 'i', 00H, 'v', 00H, 'a', 00H, 't', 00H
	DB	'e', 00H, 'B', 00H, 'u', 00H, 'i', 00H, 'l', 00H, 'd', 00H, 00H
	DB	00H
	ORG $+2
$SG73295 DB	'S', 00H, 'p', 00H, 'e', 00H, 'c', 00H, 'i', 00H, 'a', 00H
	DB	'l', 00H, 'B', 00H, 'u', 00H, 'i', 00H, 'l', 00H, 'd', 00H, 00H
	DB	00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73651	DD	019930520H
	DD	0fH
	DD	FLAT:$T73653
	DD	01H
	DD	FLAT:$T73654
	DD	2 DUP(00H)
	ORG $+4
$T73653	DD	0ffffffffH
	DD	00H
	DD	00H
	DD	FLAT:$L73636
	DD	01H
	DD	FLAT:$L73637
	DD	01H
	DD	FLAT:$L73638
	DD	01H
	DD	FLAT:$L73639
	DD	01H
	DD	FLAT:$L73640
	DD	01H
	DD	FLAT:$L73641
	DD	01H
	DD	FLAT:$L73642
	DD	01H
	DD	FLAT:$L73643
	DD	01H
	DD	FLAT:$L73644
	DD	01H
	DD	FLAT:$L73645
	DD	01H
	DD	FLAT:$L73646
	DD	01H
	DD	FLAT:$L73647
	DD	01H
	DD	FLAT:$L73648
	DD	0ffffffffH
	DD	00H
$T73654	DD	00H
	DD	0dH
	DD	0eH
	DD	01H
	DD	FLAT:$T73655
	ORG $+4
$T73655	DD	00H
	DD	FLAT:??_R0H@8
	DD	00H
	DD	FLAT:$L73635
xdata$x	ENDS
;	COMDAT ?Create@CFileVersionInfo@@QAEHPBG@Z
_TEXT	SEGMENT
_lpszFileName$ = 8
_this$ = -164
_dwHandle$ = -20
_dwFileVersionInfoSize$ = -24
_lpData$ = -28
_lpInfo$73183 = -32
_unInfoLen$73184 = -40
_dwLangCode$73193 = -44
_strSubBlock$73203 = -36
$T73604 = -48
$T73605 = -52
$T73607 = -56
$T73608 = -60
$T73609 = -64
$T73610 = -68
$T73611 = -72
$T73612 = -76
$T73613 = -80
$T73614 = -84
$T73615 = -88
$T73616 = -92
$T73617 = -96
$T73618 = -100
$T73619 = -104
$T73620 = -108
$T73621 = -112
$T73622 = -116
$T73623 = -120
$T73624 = -124
$T73625 = -128
$T73626 = -132
$T73627 = -136
$T73628 = -140
$T73629 = -144
$T73630 = -148
$T73631 = -152
$T73632 = -156
$T73633 = -160
__$EHRec$ = -16
?Create@CFileVersionInfo@@QAEHPBG@Z PROC NEAR		; CFileVersionInfo::Create, COMDAT

; 84   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73652
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	sub	esp, 316				; 0000013cH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR __$EHRec$[ebp], esp
	push	ecx
	lea	edi, DWORD PTR [ebp-332]
	mov	ecx, 79					; 0000004fH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 85   : 	Reset();

	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp

; 86   : 
; 87   : 	DWORD	dwHandle;
; 88   : 	DWORD	dwFileVersionInfoSize = GetFileVersionInfoSize((LPTSTR)lpszFileName, &dwHandle);

	lea	eax, DWORD PTR _dwHandle$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpszFileName$[ebp]
	push	ecx
	call	_GetFileVersionInfoSizeW@8
	mov	DWORD PTR _dwFileVersionInfoSize$[ebp], eax

; 89   : 	if (!dwFileVersionInfoSize)

	cmp	DWORD PTR _dwFileVersionInfoSize$[ebp], 0
	jne	SHORT $L73168

; 90   : 		return FALSE;

	xor	eax, eax
	jmp	$L73164
$L73168:

; 91   : 
; 92   : 	LPVOID	lpData = (LPVOID)new BYTE[dwFileVersionInfoSize];

	push	92					; 0000005cH
	push	OFFSET FLAT:_THIS_FILE
	mov	edx, DWORD PTR _dwFileVersionInfoSize$[ebp]
	push	edx
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T73604[ebp], eax
	mov	eax, DWORD PTR $T73604[ebp]
	mov	DWORD PTR _lpData$[ebp], eax

; 93   : 	if (!lpData)

	cmp	DWORD PTR _lpData$[ebp], 0
	jne	SHORT $L73173

; 94   : 		return FALSE;

	xor	eax, eax
	jmp	$L73164
$L73173:

; 97   : 	{

	mov	DWORD PTR __$EHRec$[ebp+12], 0

; 98   : 		if (!GetFileVersionInfo((LPTSTR)lpszFileName, dwHandle, dwFileVersionInfoSize, lpData))

	mov	ecx, DWORD PTR _lpData$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwFileVersionInfoSize$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwHandle$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpszFileName$[ebp]
	push	ecx
	call	_GetFileVersionInfoW@16
	test	eax, eax
	jne	SHORT $L73176

; 99   : 			throw FALSE;

	mov	DWORD PTR $T73605[ebp], 0
	push	OFFSET FLAT:__TI1H
	lea	edx, DWORD PTR $T73605[ebp]
	push	edx
	call	__CxxThrowException@8
$L73176:

; 103  : 		UINT		unInfoLen;
; 104  : 		if (VerQueryValue(lpData, _T("\\"), &lpInfo, &unInfoLen))

	lea	eax, DWORD PTR _unInfoLen$73184[ebp]
	push	eax
	lea	ecx, DWORD PTR _lpInfo$73183[ebp]
	push	ecx
	push	OFFSET FLAT:$SG73186
	mov	edx, DWORD PTR _lpData$[ebp]
	push	edx
	call	_VerQueryValueW@16
	test	eax, eax
	je	SHORT $L73191
$L73187:

; 106  : 			ASSERT(unInfoLen == sizeof(m_FileInfo));

	cmp	DWORD PTR _unInfoLen$73184[ebp], 52	; 00000034H
	je	SHORT $L73190
	push	106					; 0000006aH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	mov	DWORD PTR -168+[ebp], eax
	cmp	DWORD PTR -168+[ebp], 0
	je	SHORT $L73190
	int	3
$L73190:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73187

; 107  : 			if (unInfoLen == sizeof(m_FileInfo))

	cmp	DWORD PTR _unInfoLen$73184[ebp], 52	; 00000034H
	jne	SHORT $L73191

; 108  : 				memcpy(&m_FileInfo, lpInfo, unInfoLen);

	mov	ecx, DWORD PTR _unInfoLen$73184[ebp]
	push	ecx
	mov	edx, DWORD PTR _lpInfo$73183[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 4
	push	eax
	call	_memcpy
	add	esp, 12					; 0000000cH
$L73191:

; 110  : 
; 111  : 		// find best matching language and codepage
; 112  : 		VerQueryValue(lpData, _T("\\VarFileInfo\\Translation"), &lpInfo, &unInfoLen);

	lea	ecx, DWORD PTR _unInfoLen$73184[ebp]
	push	ecx
	lea	edx, DWORD PTR _lpInfo$73183[ebp]
	push	edx
	push	OFFSET FLAT:$SG73192
	mov	eax, DWORD PTR _lpData$[ebp]
	push	eax
	call	_VerQueryValueW@16

; 113  : 		
; 114  : 		DWORD	dwLangCode = 0;

	mov	DWORD PTR _dwLangCode$73193[ebp], 0

; 115  : 		if (!GetTranslationId(lpInfo, unInfoLen, GetUserDefaultLangID(), dwLangCode, FALSE))

	push	0
	lea	ecx, DWORD PTR _dwLangCode$73193[ebp]
	push	ecx
	mov	esi, esp
	call	DWORD PTR __imp__GetUserDefaultLangID@0
	cmp	esi, esp
	call	__chkesp
	push	eax
	mov	edx, DWORD PTR _unInfoLen$73184[ebp]
	push	edx
	mov	eax, DWORD PTR _lpInfo$73183[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetTranslationId@CFileVersionInfo@@IAEHPAXIGAAKH@Z ; CFileVersionInfo::GetTranslationId
	mov	DWORD PTR -172+[ebp], eax
	cmp	DWORD PTR -172+[ebp], 0
	jne	$L73201

; 117  : 			if (!GetTranslationId(lpInfo, unInfoLen, GetUserDefaultLangID(), dwLangCode, TRUE))

	push	1
	lea	ecx, DWORD PTR _dwLangCode$73193[ebp]
	push	ecx
	mov	esi, esp
	call	DWORD PTR __imp__GetUserDefaultLangID@0
	cmp	esi, esp
	call	__chkesp
	push	eax
	mov	edx, DWORD PTR _unInfoLen$73184[ebp]
	push	edx
	mov	eax, DWORD PTR _lpInfo$73183[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetTranslationId@CFileVersionInfo@@IAEHPAXIGAAKH@Z ; CFileVersionInfo::GetTranslationId
	mov	DWORD PTR -176+[ebp], eax
	cmp	DWORD PTR -176+[ebp], 0
	jne	SHORT $L73201

; 119  : 				if (!GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), dwLangCode, TRUE))

	push	1
	lea	ecx, DWORD PTR _dwLangCode$73193[ebp]
	push	ecx
	push	0
	mov	edx, DWORD PTR _unInfoLen$73184[ebp]
	push	edx
	mov	eax, DWORD PTR _lpInfo$73183[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetTranslationId@CFileVersionInfo@@IAEHPAXIGAAKH@Z ; CFileVersionInfo::GetTranslationId
	mov	DWORD PTR -180+[ebp], eax
	cmp	DWORD PTR -180+[ebp], 0
	jne	SHORT $L73201

; 121  : 					if (!GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_ENGLISH, SUBLANG_NEUTRAL), dwLangCode, TRUE))

	push	1
	lea	ecx, DWORD PTR _dwLangCode$73193[ebp]
	push	ecx
	push	9
	mov	edx, DWORD PTR _unInfoLen$73184[ebp]
	push	edx
	mov	eax, DWORD PTR _lpInfo$73183[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetTranslationId@CFileVersionInfo@@IAEHPAXIGAAKH@Z ; CFileVersionInfo::GetTranslationId
	mov	DWORD PTR -184+[ebp], eax
	cmp	DWORD PTR -184+[ebp], 0
	jne	SHORT $L73201

; 122  : 						// use the first one we can get
; 123  : 						dwLangCode = *((DWORD*)lpInfo);

	mov	ecx, DWORD PTR _lpInfo$73183[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _dwLangCode$73193[ebp], edx
$L73201:

; 127  : 		
; 128  : 
; 129  : 		CString	strSubBlock;

	lea	ecx, DWORD PTR _strSubBlock$73203[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR -188+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 1

; 130  : 		strSubBlock.Format(_T("\\StringFileInfo\\%04X%04X\\"), dwLangCode&0x0000FFFF, (dwLangCode&0xFFFF0000)>>16);

	mov	eax, DWORD PTR _dwLangCode$73193[ebp]
	and	eax, -65536				; ffff0000H
	shr	eax, 16					; 00000010H
	push	eax
	mov	ecx, DWORD PTR _dwLangCode$73193[ebp]
	and	ecx, 65535				; 0000ffffH
	push	ecx
	push	OFFSET FLAT:$SG73204
	lea	edx, DWORD PTR _strSubBlock$73203[ebp]
	push	edx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 16					; 00000010H

; 131  : 		
; 132  : 		// catch string table
; 133  : 		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("CompanyName")), &lpInfo, &unInfoLen))

	push	OFFSET FLAT:$SG73210
	lea	eax, DWORD PTR _strSubBlock$73203[ebp]
	push	eax
	lea	ecx, DWORD PTR $T73608[ebp]
	push	ecx
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -192+[ebp], eax
	mov	edx, DWORD PTR -192+[ebp]
	mov	DWORD PTR -196+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+12], 2
	mov	ecx, DWORD PTR -196+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -200+[ebp], eax
	lea	eax, DWORD PTR _unInfoLen$73184[ebp]
	push	eax
	lea	ecx, DWORD PTR _lpInfo$73183[ebp]
	push	ecx
	mov	edx, DWORD PTR -200+[ebp]
	push	edx
	mov	eax, DWORD PTR _lpData$[ebp]
	push	eax
	call	_VerQueryValueW@16
	mov	DWORD PTR $T73607[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 1
	lea	ecx, DWORD PTR $T73608[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	DWORD PTR $T73607[ebp], 0
	je	SHORT $L73208

; 134  : 			m_strCompanyName = (LPCTSTR)lpInfo;

	mov	ecx, DWORD PTR _lpInfo$73183[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 56					; 00000038H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L73208:

; 135  : 
; 136  : 		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("FileDescription")), &lpInfo, &unInfoLen))

	push	OFFSET FLAT:$SG73217
	lea	edx, DWORD PTR _strSubBlock$73203[ebp]
	push	edx
	lea	eax, DWORD PTR $T73610[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -204+[ebp], eax
	mov	ecx, DWORD PTR -204+[ebp]
	mov	DWORD PTR -208+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+12], 3
	mov	ecx, DWORD PTR -208+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -212+[ebp], eax
	lea	edx, DWORD PTR _unInfoLen$73184[ebp]
	push	edx
	lea	eax, DWORD PTR _lpInfo$73183[ebp]
	push	eax
	mov	ecx, DWORD PTR -212+[ebp]
	push	ecx
	mov	edx, DWORD PTR _lpData$[ebp]
	push	edx
	call	_VerQueryValueW@16
	mov	DWORD PTR $T73609[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 1
	lea	ecx, DWORD PTR $T73610[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	DWORD PTR $T73609[ebp], 0
	je	SHORT $L73215

; 137  : 			m_strFileDescription = (LPCTSTR)lpInfo;

	mov	eax, DWORD PTR _lpInfo$73183[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L73215:

; 138  : 
; 139  : 		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("FileVersion")), &lpInfo, &unInfoLen))

	push	OFFSET FLAT:$SG73224
	lea	ecx, DWORD PTR _strSubBlock$73203[ebp]
	push	ecx
	lea	edx, DWORD PTR $T73612[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -216+[ebp], eax
	mov	eax, DWORD PTR -216+[ebp]
	mov	DWORD PTR -220+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 4
	mov	ecx, DWORD PTR -220+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -224+[ebp], eax
	lea	ecx, DWORD PTR _unInfoLen$73184[ebp]
	push	ecx
	lea	edx, DWORD PTR _lpInfo$73183[ebp]
	push	edx
	mov	eax, DWORD PTR -224+[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpData$[ebp]
	push	ecx
	call	_VerQueryValueW@16
	mov	DWORD PTR $T73611[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 1
	lea	ecx, DWORD PTR $T73612[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	DWORD PTR $T73611[ebp], 0
	je	SHORT $L73222

; 141  : 			m_strFileVersion = (LPCTSTR)lpInfo;

	mov	edx, DWORD PTR _lpInfo$73183[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 142  : 			m_strFileVersion.Replace(_T(" "), _T("")); // remove spaces

	push	OFFSET FLAT:$SG73226
	push	OFFSET FLAT:$SG73227
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 143  : 			m_strFileVersion.Replace(_T(","), _T(".")); // replace commas with periods

	push	OFFSET FLAT:$SG73228
	push	OFFSET FLAT:$SG73229
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace
$L73222:

; 145  : 
; 146  : 		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("InternalName")), &lpInfo, &unInfoLen))

	push	OFFSET FLAT:$SG73235
	lea	eax, DWORD PTR _strSubBlock$73203[ebp]
	push	eax
	lea	ecx, DWORD PTR $T73614[ebp]
	push	ecx
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -228+[ebp], eax
	mov	edx, DWORD PTR -228+[ebp]
	mov	DWORD PTR -232+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+12], 5
	mov	ecx, DWORD PTR -232+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -236+[ebp], eax
	lea	eax, DWORD PTR _unInfoLen$73184[ebp]
	push	eax
	lea	ecx, DWORD PTR _lpInfo$73183[ebp]
	push	ecx
	mov	edx, DWORD PTR -236+[ebp]
	push	edx
	mov	eax, DWORD PTR _lpData$[ebp]
	push	eax
	call	_VerQueryValueW@16
	mov	DWORD PTR $T73613[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 1
	lea	ecx, DWORD PTR $T73614[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	DWORD PTR $T73613[ebp], 0
	je	SHORT $L73233

; 147  : 			m_strInternalName = (LPCTSTR)lpInfo;

	mov	ecx, DWORD PTR _lpInfo$73183[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 68					; 00000044H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L73233:

; 148  : 
; 149  : 		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("LegalCopyright")), &lpInfo, &unInfoLen))

	push	OFFSET FLAT:$SG73242
	lea	edx, DWORD PTR _strSubBlock$73203[ebp]
	push	edx
	lea	eax, DWORD PTR $T73616[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -240+[ebp], eax
	mov	ecx, DWORD PTR -240+[ebp]
	mov	DWORD PTR -244+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+12], 6
	mov	ecx, DWORD PTR -244+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -248+[ebp], eax
	lea	edx, DWORD PTR _unInfoLen$73184[ebp]
	push	edx
	lea	eax, DWORD PTR _lpInfo$73183[ebp]
	push	eax
	mov	ecx, DWORD PTR -248+[ebp]
	push	ecx
	mov	edx, DWORD PTR _lpData$[ebp]
	push	edx
	call	_VerQueryValueW@16
	mov	DWORD PTR $T73615[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 1
	lea	ecx, DWORD PTR $T73616[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	DWORD PTR $T73615[ebp], 0
	je	SHORT $L73240

; 150  : 			m_strLegalCopyright = (LPCTSTR)lpInfo;

	mov	eax, DWORD PTR _lpInfo$73183[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L73240:

; 151  : 
; 152  : 		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("OriginalFileName")), &lpInfo, &unInfoLen))

	push	OFFSET FLAT:$SG73249
	lea	ecx, DWORD PTR _strSubBlock$73203[ebp]
	push	ecx
	lea	edx, DWORD PTR $T73618[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -252+[ebp], eax
	mov	eax, DWORD PTR -252+[ebp]
	mov	DWORD PTR -256+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 7
	mov	ecx, DWORD PTR -256+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -260+[ebp], eax
	lea	ecx, DWORD PTR _unInfoLen$73184[ebp]
	push	ecx
	lea	edx, DWORD PTR _lpInfo$73183[ebp]
	push	edx
	mov	eax, DWORD PTR -260+[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpData$[ebp]
	push	ecx
	call	_VerQueryValueW@16
	mov	DWORD PTR $T73617[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 1
	lea	ecx, DWORD PTR $T73618[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	DWORD PTR $T73617[ebp], 0
	je	SHORT $L73247

; 153  : 			m_strOriginalFileName = (LPCTSTR)lpInfo;

	mov	edx, DWORD PTR _lpInfo$73183[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 76					; 0000004cH
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L73247:

; 154  : 
; 155  : 		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("ProductName")), &lpInfo, &unInfoLen))

	push	OFFSET FLAT:$SG73256
	lea	eax, DWORD PTR _strSubBlock$73203[ebp]
	push	eax
	lea	ecx, DWORD PTR $T73620[ebp]
	push	ecx
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -264+[ebp], eax
	mov	edx, DWORD PTR -264+[ebp]
	mov	DWORD PTR -268+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+12], 8
	mov	ecx, DWORD PTR -268+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -272+[ebp], eax
	lea	eax, DWORD PTR _unInfoLen$73184[ebp]
	push	eax
	lea	ecx, DWORD PTR _lpInfo$73183[ebp]
	push	ecx
	mov	edx, DWORD PTR -272+[ebp]
	push	edx
	mov	eax, DWORD PTR _lpData$[ebp]
	push	eax
	call	_VerQueryValueW@16
	mov	DWORD PTR $T73619[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 1
	lea	ecx, DWORD PTR $T73620[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	DWORD PTR $T73619[ebp], 0
	je	SHORT $L73254

; 156  : 			m_strProductName = (LPCTSTR)lpInfo;

	mov	ecx, DWORD PTR _lpInfo$73183[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 80					; 00000050H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L73254:

; 157  : 
; 158  : 		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("ProductVersion")), &lpInfo, &unInfoLen))

	push	OFFSET FLAT:$SG73263
	lea	edx, DWORD PTR _strSubBlock$73203[ebp]
	push	edx
	lea	eax, DWORD PTR $T73622[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -276+[ebp], eax
	mov	ecx, DWORD PTR -276+[ebp]
	mov	DWORD PTR -280+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+12], 9
	mov	ecx, DWORD PTR -280+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -284+[ebp], eax
	lea	edx, DWORD PTR _unInfoLen$73184[ebp]
	push	edx
	lea	eax, DWORD PTR _lpInfo$73183[ebp]
	push	eax
	mov	ecx, DWORD PTR -284+[ebp]
	push	ecx
	mov	edx, DWORD PTR _lpData$[ebp]
	push	edx
	call	_VerQueryValueW@16
	mov	DWORD PTR $T73621[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 1
	lea	ecx, DWORD PTR $T73622[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	DWORD PTR $T73621[ebp], 0
	je	SHORT $L73261

; 160  : 			m_strProductVersion = (LPCTSTR)lpInfo;

	mov	eax, DWORD PTR _lpInfo$73183[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 84					; 00000054H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 161  : 			m_strProductVersion.Replace(_T(" "), _T("")); // remove spaces

	push	OFFSET FLAT:$SG73265
	push	OFFSET FLAT:$SG73266
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 84					; 00000054H
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace

; 162  : 			m_strProductVersion.Replace(_T(","), _T(".")); // replace commas with periods

	push	OFFSET FLAT:$SG73267
	push	OFFSET FLAT:$SG73268
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 84					; 00000054H
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace
$L73261:

; 164  : 
; 165  : 		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("Comments")), &lpInfo, &unInfoLen))

	push	OFFSET FLAT:$SG73274
	lea	ecx, DWORD PTR _strSubBlock$73203[ebp]
	push	ecx
	lea	edx, DWORD PTR $T73624[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -288+[ebp], eax
	mov	eax, DWORD PTR -288+[ebp]
	mov	DWORD PTR -292+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 10		; 0000000aH
	mov	ecx, DWORD PTR -292+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -296+[ebp], eax
	lea	ecx, DWORD PTR _unInfoLen$73184[ebp]
	push	ecx
	lea	edx, DWORD PTR _lpInfo$73183[ebp]
	push	edx
	mov	eax, DWORD PTR -296+[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpData$[ebp]
	push	ecx
	call	_VerQueryValueW@16
	mov	DWORD PTR $T73623[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 1
	lea	ecx, DWORD PTR $T73624[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	DWORD PTR $T73623[ebp], 0
	je	SHORT $L73272

; 166  : 			m_strComments = (LPCTSTR)lpInfo;

	mov	edx, DWORD PTR _lpInfo$73183[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 88					; 00000058H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L73272:

; 167  : 
; 168  : 		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("LegalTrademarks")), &lpInfo, &unInfoLen))

	push	OFFSET FLAT:$SG73281
	lea	eax, DWORD PTR _strSubBlock$73203[ebp]
	push	eax
	lea	ecx, DWORD PTR $T73626[ebp]
	push	ecx
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -300+[ebp], eax
	mov	edx, DWORD PTR -300+[ebp]
	mov	DWORD PTR -304+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+12], 11		; 0000000bH
	mov	ecx, DWORD PTR -304+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -308+[ebp], eax
	lea	eax, DWORD PTR _unInfoLen$73184[ebp]
	push	eax
	lea	ecx, DWORD PTR _lpInfo$73183[ebp]
	push	ecx
	mov	edx, DWORD PTR -308+[ebp]
	push	edx
	mov	eax, DWORD PTR _lpData$[ebp]
	push	eax
	call	_VerQueryValueW@16
	mov	DWORD PTR $T73625[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 1
	lea	ecx, DWORD PTR $T73626[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	DWORD PTR $T73625[ebp], 0
	je	SHORT $L73279

; 169  : 			m_strLegalTrademarks = (LPCTSTR)lpInfo;

	mov	ecx, DWORD PTR _lpInfo$73183[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 92					; 0000005cH
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L73279:

; 170  : 
; 171  : 		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("PrivateBuild")), &lpInfo, &unInfoLen))

	push	OFFSET FLAT:$SG73288
	lea	edx, DWORD PTR _strSubBlock$73203[ebp]
	push	edx
	lea	eax, DWORD PTR $T73628[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -312+[ebp], eax
	mov	ecx, DWORD PTR -312+[ebp]
	mov	DWORD PTR -316+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+12], 12		; 0000000cH
	mov	ecx, DWORD PTR -316+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -320+[ebp], eax
	lea	edx, DWORD PTR _unInfoLen$73184[ebp]
	push	edx
	lea	eax, DWORD PTR _lpInfo$73183[ebp]
	push	eax
	mov	ecx, DWORD PTR -320+[ebp]
	push	ecx
	mov	edx, DWORD PTR _lpData$[ebp]
	push	edx
	call	_VerQueryValueW@16
	mov	DWORD PTR $T73627[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 1
	lea	ecx, DWORD PTR $T73628[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	DWORD PTR $T73627[ebp], 0
	je	SHORT $L73286

; 172  : 			m_strPrivateBuild = (LPCTSTR)lpInfo;

	mov	eax, DWORD PTR _lpInfo$73183[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 96					; 00000060H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L73286:

; 173  : 
; 174  : 		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("SpecialBuild")), &lpInfo, &unInfoLen))

	push	OFFSET FLAT:$SG73295
	lea	ecx, DWORD PTR _strSubBlock$73203[ebp]
	push	ecx
	lea	edx, DWORD PTR $T73630[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -324+[ebp], eax
	mov	eax, DWORD PTR -324+[ebp]
	mov	DWORD PTR -328+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 13		; 0000000dH
	mov	ecx, DWORD PTR -328+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR -332+[ebp], eax
	lea	ecx, DWORD PTR _unInfoLen$73184[ebp]
	push	ecx
	lea	edx, DWORD PTR _lpInfo$73183[ebp]
	push	edx
	mov	eax, DWORD PTR -332+[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpData$[ebp]
	push	ecx
	call	_VerQueryValueW@16
	mov	DWORD PTR $T73629[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 1
	lea	ecx, DWORD PTR $T73630[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	DWORD PTR $T73629[ebp], 0
	je	SHORT $L73293

; 175  : 			m_strSpecialBuild = (LPCTSTR)lpInfo;

	mov	edx, DWORD PTR _lpInfo$73183[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 100				; 00000064H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L73293:

; 176  : 
; 177  : 		delete[] lpData;

	mov	eax, DWORD PTR _lpData$[ebp]
	mov	DWORD PTR $T73631[ebp], eax
	mov	ecx, DWORD PTR $T73631[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 178  : 	}

	mov	BYTE PTR __$EHRec$[ebp+12], 0
	lea	ecx, DWORD PTR _strSubBlock$73203[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 179  : 	catch (BOOL)

	jmp	SHORT $L73174
$L73635:

; 180  : 	{
; 181  : 		delete[] lpData;

	mov	edx, DWORD PTR _lpData$[ebp]
	mov	DWORD PTR $T73632[ebp], edx
	mov	eax, DWORD PTR $T73632[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 182  : 		return FALSE;

	mov	DWORD PTR $T73633[ebp], 0
	mov	eax, $L73649
	ret	0
$L73649:
	mov	eax, DWORD PTR $T73633[ebp]
	jmp	SHORT $L73164
$L73174:

; 184  : 
; 185  : 	return TRUE;

	mov	DWORD PTR __$EHRec$[ebp+12], -1
	mov	eax, 1
$L73164:

; 186  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp+4]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 332				; 0000014cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73636:
	lea	ecx, DWORD PTR _strSubBlock$73203[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73637:
	lea	ecx, DWORD PTR $T73608[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73638:
	lea	ecx, DWORD PTR $T73610[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73639:
	lea	ecx, DWORD PTR $T73612[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73640:
	lea	ecx, DWORD PTR $T73614[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73641:
	lea	ecx, DWORD PTR $T73616[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73642:
	lea	ecx, DWORD PTR $T73618[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73643:
	lea	ecx, DWORD PTR $T73620[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73644:
	lea	ecx, DWORD PTR $T73622[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73645:
	lea	ecx, DWORD PTR $T73624[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73646:
	lea	ecx, DWORD PTR $T73626[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73647:
	lea	ecx, DWORD PTR $T73628[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73648:
	lea	ecx, DWORD PTR $T73630[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73652:
	mov	eax, OFFSET FLAT:$T73651
	jmp	___CxxFrameHandler
text$x	ENDS
?Create@CFileVersionInfo@@QAEHPBG@Z ENDP		; CFileVersionInfo::Create
PUBLIC	?GetFileVersion@CFileVersionInfo@@QBEGH@Z	; CFileVersionInfo::GetFileVersion
;	COMDAT ?GetFileVersion@CFileVersionInfo@@QBEGH@Z
_TEXT	SEGMENT
_nIndex$ = 8
_this$ = -4
?GetFileVersion@CFileVersionInfo@@QBEGH@Z PROC NEAR	; CFileVersionInfo::GetFileVersion, COMDAT

; 190  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 191  : 	if (nIndex == 0)

	cmp	DWORD PTR _nIndex$[ebp], 0
	jne	SHORT $L73306

; 192  : 		return (WORD)(m_FileInfo.dwFileVersionLS & 0x0000FFFF);

	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+16]
	and	eax, 65535				; 0000ffffH
	jmp	SHORT $L73317
$L73306:

; 193  : 	else if (nIndex == 1)

	cmp	DWORD PTR _nIndex$[ebp], 1
	jne	SHORT $L73309

; 194  : 		return (WORD)((m_FileInfo.dwFileVersionLS & 0xFFFF0000) >> 16);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [ecx+16]
	and	eax, -65536				; ffff0000H
	shr	eax, 16					; 00000010H
	jmp	SHORT $L73317
$L73309:

; 195  : 	else if (nIndex == 2)

	cmp	DWORD PTR _nIndex$[ebp], 2
	jne	SHORT $L73312

; 196  : 		return (WORD)(m_FileInfo.dwFileVersionMS & 0x0000FFFF);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	and	eax, 65535				; 0000ffffH
	jmp	SHORT $L73317
$L73312:

; 197  : 	else if (nIndex == 3)

	cmp	DWORD PTR _nIndex$[ebp], 3
	jne	SHORT $L73315

; 198  : 		return (WORD)((m_FileInfo.dwFileVersionMS & 0xFFFF0000) >> 16);

	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+12]
	and	eax, -65536				; ffff0000H
	shr	eax, 16					; 00000010H
	jmp	SHORT $L73317
$L73315:

; 200  : 		return 0;

	xor	ax, ax
$L73317:

; 201  : }

	mov	esp, ebp
	pop	ebp
	ret	4
?GetFileVersion@CFileVersionInfo@@QBEGH@Z ENDP		; CFileVersionInfo::GetFileVersion
_TEXT	ENDS
PUBLIC	?GetProductVersion@CFileVersionInfo@@QBEGH@Z	; CFileVersionInfo::GetProductVersion
;	COMDAT ?GetProductVersion@CFileVersionInfo@@QBEGH@Z
_TEXT	SEGMENT
_nIndex$ = 8
_this$ = -4
?GetProductVersion@CFileVersionInfo@@QBEGH@Z PROC NEAR	; CFileVersionInfo::GetProductVersion, COMDAT

; 205  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 206  : 	if (nIndex == 0)

	cmp	DWORD PTR _nIndex$[ebp], 0
	jne	SHORT $L73322

; 207  : 		return (WORD)(m_FileInfo.dwProductVersionLS & 0x0000FFFF);

	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+24]
	and	eax, 65535				; 0000ffffH
	jmp	SHORT $L73333
$L73322:

; 208  : 	else if (nIndex == 1)

	cmp	DWORD PTR _nIndex$[ebp], 1
	jne	SHORT $L73325

; 209  : 		return (WORD)((m_FileInfo.dwProductVersionLS & 0xFFFF0000) >> 16);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [ecx+24]
	and	eax, -65536				; ffff0000H
	shr	eax, 16					; 00000010H
	jmp	SHORT $L73333
$L73325:

; 210  : 	else if (nIndex == 2)

	cmp	DWORD PTR _nIndex$[ebp], 2
	jne	SHORT $L73328

; 211  : 		return (WORD)(m_FileInfo.dwProductVersionMS & 0x0000FFFF);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+20]
	and	eax, 65535				; 0000ffffH
	jmp	SHORT $L73333
$L73328:

; 212  : 	else if (nIndex == 3)

	cmp	DWORD PTR _nIndex$[ebp], 3
	jne	SHORT $L73331

; 213  : 		return (WORD)((m_FileInfo.dwProductVersionMS & 0xFFFF0000) >> 16);

	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+20]
	and	eax, -65536				; ffff0000H
	shr	eax, 16					; 00000010H
	jmp	SHORT $L73333
$L73331:

; 215  : 		return 0;

	xor	ax, ax
$L73333:

; 216  : }

	mov	esp, ebp
	pop	ebp
	ret	4
?GetProductVersion@CFileVersionInfo@@QBEGH@Z ENDP	; CFileVersionInfo::GetProductVersion
_TEXT	ENDS
PUBLIC	?GetFileFlagsMask@CFileVersionInfo@@QBEKXZ	; CFileVersionInfo::GetFileFlagsMask
;	COMDAT ?GetFileFlagsMask@CFileVersionInfo@@QBEKXZ
_TEXT	SEGMENT
_this$ = -4
?GetFileFlagsMask@CFileVersionInfo@@QBEKXZ PROC NEAR	; CFileVersionInfo::GetFileFlagsMask, COMDAT

; 220  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 221  : 	return m_FileInfo.dwFileFlagsMask;

	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+28]

; 222  : }

	mov	esp, ebp
	pop	ebp
	ret	0
?GetFileFlagsMask@CFileVersionInfo@@QBEKXZ ENDP		; CFileVersionInfo::GetFileFlagsMask
_TEXT	ENDS
PUBLIC	?GetFileFlags@CFileVersionInfo@@QBEKXZ		; CFileVersionInfo::GetFileFlags
;	COMDAT ?GetFileFlags@CFileVersionInfo@@QBEKXZ
_TEXT	SEGMENT
_this$ = -4
?GetFileFlags@CFileVersionInfo@@QBEKXZ PROC NEAR	; CFileVersionInfo::GetFileFlags, COMDAT

; 226  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 227  : 	return m_FileInfo.dwFileFlags;

	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+32]

; 228  : }

	mov	esp, ebp
	pop	ebp
	ret	0
?GetFileFlags@CFileVersionInfo@@QBEKXZ ENDP		; CFileVersionInfo::GetFileFlags
_TEXT	ENDS
PUBLIC	?GetFileOs@CFileVersionInfo@@QBEKXZ		; CFileVersionInfo::GetFileOs
;	COMDAT ?GetFileOs@CFileVersionInfo@@QBEKXZ
_TEXT	SEGMENT
_this$ = -4
?GetFileOs@CFileVersionInfo@@QBEKXZ PROC NEAR		; CFileVersionInfo::GetFileOs, COMDAT

; 232  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 233  : 	return m_FileInfo.dwFileOS;

	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+36]

; 234  : }

	mov	esp, ebp
	pop	ebp
	ret	0
?GetFileOs@CFileVersionInfo@@QBEKXZ ENDP		; CFileVersionInfo::GetFileOs
_TEXT	ENDS
PUBLIC	?GetFileType@CFileVersionInfo@@QBEKXZ		; CFileVersionInfo::GetFileType
;	COMDAT ?GetFileType@CFileVersionInfo@@QBEKXZ
_TEXT	SEGMENT
_this$ = -4
?GetFileType@CFileVersionInfo@@QBEKXZ PROC NEAR		; CFileVersionInfo::GetFileType, COMDAT

; 238  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 239  : 	return m_FileInfo.dwFileType;

	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+40]

; 240  : }

	mov	esp, ebp
	pop	ebp
	ret	0
?GetFileType@CFileVersionInfo@@QBEKXZ ENDP		; CFileVersionInfo::GetFileType
_TEXT	ENDS
PUBLIC	?GetFileSubtype@CFileVersionInfo@@QBEKXZ	; CFileVersionInfo::GetFileSubtype
;	COMDAT ?GetFileSubtype@CFileVersionInfo@@QBEKXZ
_TEXT	SEGMENT
_this$ = -4
?GetFileSubtype@CFileVersionInfo@@QBEKXZ PROC NEAR	; CFileVersionInfo::GetFileSubtype, COMDAT

; 244  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 245  : 	return m_FileInfo.dwFileSubtype;

	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+44]

; 246  : }

	mov	esp, ebp
	pop	ebp
	ret	0
?GetFileSubtype@CFileVersionInfo@@QBEKXZ ENDP		; CFileVersionInfo::GetFileSubtype
_TEXT	ENDS
PUBLIC	?GetFileDate@CFileVersionInfo@@QBE?AVCTime@@XZ	; CFileVersionInfo::GetFileDate
EXTRN	??0CTime@@QAE@ABU_FILETIME@@H@Z:NEAR		; CTime::CTime
;	COMDAT ?GetFileDate@CFileVersionInfo@@QBE?AVCTime@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_this$ = -12
_ft$ = -8
?GetFileDate@CFileVersionInfo@@QBE?AVCTime@@XZ PROC NEAR ; CFileVersionInfo::GetFileDate, COMDAT

; 250  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 251  : 	FILETIME	ft;
; 252  : 	ft.dwLowDateTime = m_FileInfo.dwFileDateLS;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+52]
	mov	DWORD PTR _ft$[ebp], ecx

; 253  : 	ft.dwHighDateTime = m_FileInfo.dwFileDateMS;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+48]
	mov	DWORD PTR _ft$[ebp+4], eax

; 254  : 	return CTime(ft);

	push	-1
	lea	ecx, DWORD PTR _ft$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CTime@@QAE@ABU_FILETIME@@H@Z		; CTime::CTime
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 255  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetFileDate@CFileVersionInfo@@QBE?AVCTime@@XZ ENDP	; CFileVersionInfo::GetFileDate
_TEXT	ENDS
PUBLIC	?GetCompanyName@CFileVersionInfo@@QBE?AVCString@@XZ ; CFileVersionInfo::GetCompanyName
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
;	COMDAT ?GetCompanyName@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_this$ = -8
$T73678 = -4
?GetCompanyName@CFileVersionInfo@@QBE?AVCString@@XZ PROC NEAR ; CFileVersionInfo::GetCompanyName, COMDAT

; 259  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T73678[ebp], 0

; 260  : 	return m_strCompanyName;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 56					; 00000038H
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T73678[ebp]
	or	ecx, 1
	mov	DWORD PTR $T73678[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 261  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetCompanyName@CFileVersionInfo@@QBE?AVCString@@XZ ENDP ; CFileVersionInfo::GetCompanyName
_TEXT	ENDS
PUBLIC	?GetFileDescription@CFileVersionInfo@@QBE?AVCString@@XZ ; CFileVersionInfo::GetFileDescription
;	COMDAT ?GetFileDescription@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_this$ = -8
$T73683 = -4
?GetFileDescription@CFileVersionInfo@@QBE?AVCString@@XZ PROC NEAR ; CFileVersionInfo::GetFileDescription, COMDAT

; 265  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T73683[ebp], 0

; 266  : 	return m_strFileDescription;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 60					; 0000003cH
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T73683[ebp]
	or	ecx, 1
	mov	DWORD PTR $T73683[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 267  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetFileDescription@CFileVersionInfo@@QBE?AVCString@@XZ ENDP ; CFileVersionInfo::GetFileDescription
_TEXT	ENDS
PUBLIC	?GetFileVersion@CFileVersionInfo@@QBE?AVCString@@XZ ; CFileVersionInfo::GetFileVersion
;	COMDAT ?GetFileVersion@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_this$ = -8
$T73688 = -4
?GetFileVersion@CFileVersionInfo@@QBE?AVCString@@XZ PROC NEAR ; CFileVersionInfo::GetFileVersion, COMDAT

; 271  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T73688[ebp], 0

; 272  : 	return m_strFileVersion;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 64					; 00000040H
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T73688[ebp]
	or	ecx, 1
	mov	DWORD PTR $T73688[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 273  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetFileVersion@CFileVersionInfo@@QBE?AVCString@@XZ ENDP ; CFileVersionInfo::GetFileVersion
_TEXT	ENDS
PUBLIC	?GetInternalName@CFileVersionInfo@@QBE?AVCString@@XZ ; CFileVersionInfo::GetInternalName
;	COMDAT ?GetInternalName@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_this$ = -8
$T73693 = -4
?GetInternalName@CFileVersionInfo@@QBE?AVCString@@XZ PROC NEAR ; CFileVersionInfo::GetInternalName, COMDAT

; 277  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T73693[ebp], 0

; 278  : 	return m_strInternalName;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 68					; 00000044H
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T73693[ebp]
	or	ecx, 1
	mov	DWORD PTR $T73693[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 279  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetInternalName@CFileVersionInfo@@QBE?AVCString@@XZ ENDP ; CFileVersionInfo::GetInternalName
_TEXT	ENDS
PUBLIC	?GetLegalCopyright@CFileVersionInfo@@QBE?AVCString@@XZ ; CFileVersionInfo::GetLegalCopyright
;	COMDAT ?GetLegalCopyright@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_this$ = -8
$T73698 = -4
?GetLegalCopyright@CFileVersionInfo@@QBE?AVCString@@XZ PROC NEAR ; CFileVersionInfo::GetLegalCopyright, COMDAT

; 283  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T73698[ebp], 0

; 284  : 	return m_strLegalCopyright;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 72					; 00000048H
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T73698[ebp]
	or	ecx, 1
	mov	DWORD PTR $T73698[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 285  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetLegalCopyright@CFileVersionInfo@@QBE?AVCString@@XZ ENDP ; CFileVersionInfo::GetLegalCopyright
_TEXT	ENDS
PUBLIC	?GetOriginalFileName@CFileVersionInfo@@QBE?AVCString@@XZ ; CFileVersionInfo::GetOriginalFileName
;	COMDAT ?GetOriginalFileName@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_this$ = -8
$T73703 = -4
?GetOriginalFileName@CFileVersionInfo@@QBE?AVCString@@XZ PROC NEAR ; CFileVersionInfo::GetOriginalFileName, COMDAT

; 289  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T73703[ebp], 0

; 290  : 	return m_strOriginalFileName;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 76					; 0000004cH
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T73703[ebp]
	or	ecx, 1
	mov	DWORD PTR $T73703[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 291  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetOriginalFileName@CFileVersionInfo@@QBE?AVCString@@XZ ENDP ; CFileVersionInfo::GetOriginalFileName
_TEXT	ENDS
PUBLIC	?GetProductName@CFileVersionInfo@@QBE?AVCString@@XZ ; CFileVersionInfo::GetProductName
;	COMDAT ?GetProductName@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_this$ = -8
$T73708 = -4
?GetProductName@CFileVersionInfo@@QBE?AVCString@@XZ PROC NEAR ; CFileVersionInfo::GetProductName, COMDAT

; 295  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T73708[ebp], 0

; 296  : 	return m_strProductName;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 80					; 00000050H
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T73708[ebp]
	or	ecx, 1
	mov	DWORD PTR $T73708[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 297  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetProductName@CFileVersionInfo@@QBE?AVCString@@XZ ENDP ; CFileVersionInfo::GetProductName
_TEXT	ENDS
PUBLIC	?GetProductVersion@CFileVersionInfo@@QBE?AVCString@@XZ ; CFileVersionInfo::GetProductVersion
;	COMDAT ?GetProductVersion@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_this$ = -8
$T73713 = -4
?GetProductVersion@CFileVersionInfo@@QBE?AVCString@@XZ PROC NEAR ; CFileVersionInfo::GetProductVersion, COMDAT

; 301  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T73713[ebp], 0

; 302  : 	return m_strProductVersion;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 84					; 00000054H
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T73713[ebp]
	or	ecx, 1
	mov	DWORD PTR $T73713[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 303  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetProductVersion@CFileVersionInfo@@QBE?AVCString@@XZ ENDP ; CFileVersionInfo::GetProductVersion
_TEXT	ENDS
PUBLIC	?GetComments@CFileVersionInfo@@QBE?AVCString@@XZ ; CFileVersionInfo::GetComments
;	COMDAT ?GetComments@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_this$ = -8
$T73718 = -4
?GetComments@CFileVersionInfo@@QBE?AVCString@@XZ PROC NEAR ; CFileVersionInfo::GetComments, COMDAT

; 307  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T73718[ebp], 0

; 308  : 	return m_strComments;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 88					; 00000058H
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T73718[ebp]
	or	ecx, 1
	mov	DWORD PTR $T73718[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 309  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetComments@CFileVersionInfo@@QBE?AVCString@@XZ ENDP	; CFileVersionInfo::GetComments
_TEXT	ENDS
PUBLIC	?GetLegalTrademarks@CFileVersionInfo@@QBE?AVCString@@XZ ; CFileVersionInfo::GetLegalTrademarks
;	COMDAT ?GetLegalTrademarks@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_this$ = -8
$T73723 = -4
?GetLegalTrademarks@CFileVersionInfo@@QBE?AVCString@@XZ PROC NEAR ; CFileVersionInfo::GetLegalTrademarks, COMDAT

; 313  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T73723[ebp], 0

; 314  : 	return m_strLegalTrademarks;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 92					; 0000005cH
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T73723[ebp]
	or	ecx, 1
	mov	DWORD PTR $T73723[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 315  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetLegalTrademarks@CFileVersionInfo@@QBE?AVCString@@XZ ENDP ; CFileVersionInfo::GetLegalTrademarks
_TEXT	ENDS
PUBLIC	?GetPrivateBuild@CFileVersionInfo@@QBE?AVCString@@XZ ; CFileVersionInfo::GetPrivateBuild
;	COMDAT ?GetPrivateBuild@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT
$T73728 = -4
___$ReturnUdt$ = 8
_this$ = -8
?GetPrivateBuild@CFileVersionInfo@@QBE?AVCString@@XZ PROC NEAR ; CFileVersionInfo::GetPrivateBuild, COMDAT

; 319  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T73728[ebp], 0

; 320  : 	return m_strPrivateBuild;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 96					; 00000060H
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T73728[ebp]
	or	ecx, 1
	mov	DWORD PTR $T73728[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 321  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetPrivateBuild@CFileVersionInfo@@QBE?AVCString@@XZ ENDP ; CFileVersionInfo::GetPrivateBuild
_TEXT	ENDS
PUBLIC	?GetSpecialBuild@CFileVersionInfo@@QBE?AVCString@@XZ ; CFileVersionInfo::GetSpecialBuild
;	COMDAT ?GetSpecialBuild@CFileVersionInfo@@QBE?AVCString@@XZ
_TEXT	SEGMENT
$T73733 = -4
___$ReturnUdt$ = 8
_this$ = -8
?GetSpecialBuild@CFileVersionInfo@@QBE?AVCString@@XZ PROC NEAR ; CFileVersionInfo::GetSpecialBuild, COMDAT

; 325  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T73733[ebp], 0

; 326  : 	return m_strSpecialBuild;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 100				; 00000064H
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T73733[ebp]
	or	ecx, 1
	mov	DWORD PTR $T73733[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 327  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetSpecialBuild@CFileVersionInfo@@QBE?AVCString@@XZ ENDP ; CFileVersionInfo::GetSpecialBuild
_TEXT	ENDS
EXTRN	_memset:NEAR
EXTRN	?Empty@CString@@QAEXXZ:NEAR			; CString::Empty
;	COMDAT ?Reset@CFileVersionInfo@@MAEXXZ
_TEXT	SEGMENT
_this$ = -4
?Reset@CFileVersionInfo@@MAEXXZ PROC NEAR		; CFileVersionInfo::Reset, COMDAT

; 331  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 332  : 	ZeroMemory(&m_FileInfo, sizeof(m_FileInfo));

	push	52					; 00000034H
	push	0
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 4
	push	eax
	call	_memset
	add	esp, 12					; 0000000cH

; 333  : 	m_strCompanyName.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 56					; 00000038H
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 334  : 	m_strFileDescription.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 60					; 0000003cH
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 335  : 	m_strFileVersion.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 336  : 	m_strInternalName.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 68					; 00000044H
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 337  : 	m_strLegalCopyright.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 338  : 	m_strOriginalFileName.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 76					; 0000004cH
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 339  : 	m_strProductName.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 80					; 00000050H
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 340  : 	m_strProductVersion.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 84					; 00000054H
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 341  : 	m_strComments.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 88					; 00000058H
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 342  : 	m_strLegalTrademarks.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 92					; 0000005cH
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 343  : 	m_strPrivateBuild.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 96					; 00000060H
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 344  : 	m_strSpecialBuild.Empty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 100				; 00000064H
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 345  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Reset@CFileVersionInfo@@MAEXXZ ENDP			; CFileVersionInfo::Reset
_TEXT	ENDS
END

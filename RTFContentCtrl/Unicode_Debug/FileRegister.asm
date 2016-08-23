	TITLE	D:\_CODE\Shared\FileRegister.cpp
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
;	COMDAT ??0CFileRegister@@QAE@PBG0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CFileRegister@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RegisterFileType@CFileRegister@@QAEHPBGHH0H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?UnRegisterFileType@CFileRegister@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsRegisteredApp@CFileRegister@@SAHPBG0H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsRegisteredAppInstalled@CFileRegister@@SAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetRegisteredAppPath@CFileRegister@@SA?AVCString@@PBGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetExtension@CFileRegister@@KA?AVCString@@PBG@Z
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
PUBLIC	??0CFileRegister@@QAE@PBG0@Z			; CFileRegister::CFileRegister
PUBLIC	?GetExtension@CFileRegister@@KA?AVCString@@PBG@Z ; CFileRegister::GetExtension
EXTRN	__except_list:DWORD
EXTRN	__chkesp:NEAR
EXTRN	___CxxFrameHandler:NEAR
EXTRN	?GetAppFileName@FileMisc@@YA?AVCString@@XZ:NEAR	; FileMisc::GetAppFileName
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
EXTRN	??4CString@@QAEABV0@PBG@Z:NEAR			; CString::operator=
EXTRN	?TrimRight@CString@@QAEXXZ:NEAR			; CString::TrimRight
EXTRN	?TrimLeft@CString@@QAEXXZ:NEAR			; CString::TrimLeft
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT data
; File D:\_CODE\Shared\FileRegister.cpp
data	SEGMENT
$SG74484 DB	'D:\_CODE\Shared\FileRegister.cpp', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74818	DD	019930520H
	DD	05H
	DD	FLAT:$T74822
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74822	DD	0ffffffffH
	DD	FLAT:$L74810
	DD	00H
	DD	FLAT:$L74811
	DD	01H
	DD	FLAT:$L74812
	DD	02H
	DD	FLAT:$L74813
	DD	02H
	DD	FLAT:$L74814
xdata$x	ENDS
;	COMDAT ??0CFileRegister@@QAE@PBG0@Z
_TEXT	SEGMENT
$T74808 = -16
$T74809 = -20
__$EHRec$ = -12
_szExt$ = 8
_szFileType$ = 12
_this$ = -24
??0CFileRegister@@QAE@PBG0@Z PROC NEAR			; CFileRegister::CFileRegister, COMDAT

; 15   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74819
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 28					; 0000001cH
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 16   : 	m_sExt = GetExtension(szExt);

	mov	eax, DWORD PTR _szExt$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T74808[ebp]
	push	ecx
	call	?GetExtension@CFileRegister@@KA?AVCString@@PBG@Z ; CFileRegister::GetExtension
	add	esp, 8
	mov	DWORD PTR -28+[ebp], eax
	mov	edx, DWORD PTR -28+[ebp]
	mov	DWORD PTR -32+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	eax, DWORD PTR -32+[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T74808[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 17   : 
; 18   : 	m_sFileType = szFileType;

	mov	ecx, DWORD PTR _szFileType$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 19   : 	m_sFileType.TrimRight();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	?TrimRight@CString@@QAEXXZ		; CString::TrimRight

; 20   : 	m_sFileType.TrimLeft();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	?TrimLeft@CString@@QAEXXZ		; CString::TrimLeft

; 21   : 
; 22   : 	// get the app path
; 23   : 	m_sAppPath = FileMisc::GetAppFileName();

	lea	edx, DWORD PTR $T74809[ebp]
	push	edx
	call	?GetAppFileName@FileMisc@@YA?AVCString@@XZ ; FileMisc::GetAppFileName
	add	esp, 4
	mov	DWORD PTR -36+[ebp], eax
	mov	eax, DWORD PTR -36+[ebp]
	mov	DWORD PTR -40+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	ecx, DWORD PTR -40+[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T74809[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74480:

; 24   : 
; 25   : 	ASSERT (!m_sAppPath.IsEmpty());

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L74483
	push	25					; 00000019H
	push	OFFSET FLAT:$SG74484
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74483
	int	3
$L74483:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74480

; 26   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74810:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74811:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74812:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74813:
	lea	ecx, DWORD PTR $T74808[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74814:
	lea	ecx, DWORD PTR $T74809[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74819:
	mov	eax, OFFSET FLAT:$T74818
	jmp	___CxxFrameHandler
text$x	ENDS
??0CFileRegister@@QAE@PBG0@Z ENDP			; CFileRegister::CFileRegister
PUBLIC	??1CFileRegister@@QAE@XZ			; CFileRegister::~CFileRegister
;	COMDAT xdata$x
; File D:\_CODE\Shared\FileRegister.cpp
xdata$x	SEGMENT
$T74833	DD	019930520H
	DD	02H
	DD	FLAT:$T74835
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74835	DD	0ffffffffH
	DD	FLAT:$L74829
	DD	00H
	DD	FLAT:$L74830
xdata$x	ENDS
;	COMDAT ??1CFileRegister@@QAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??1CFileRegister@@QAE@XZ PROC NEAR			; CFileRegister::~CFileRegister, COMDAT

; 29   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74834
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 30   : }

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
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
$L74829:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74830:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74834:
	mov	eax, OFFSET FLAT:$T74833
	jmp	___CxxFrameHandler
text$x	ENDS
??1CFileRegister@@QAE@XZ ENDP				; CFileRegister::~CFileRegister
PUBLIC	?RegisterFileType@CFileRegister@@QAEHPBGHH0H@Z	; CFileRegister::RegisterFileType
EXTRN	??0CRegKey@@QAE@XZ:NEAR				; CRegKey::CRegKey
EXTRN	??1CRegKey@@UAE@XZ:NEAR				; CRegKey::~CRegKey
EXTRN	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z:NEAR	; CRegKey::Open
EXTRN	?Close@CRegKey@@QAEXXZ:NEAR			; CRegKey::Close
EXTRN	?Write@CRegKey@@QAEJPBG0@Z:NEAR			; CRegKey::Write
EXTRN	?Read@CRegKey@@QBEJPBGAAVCString@@@Z:NEAR	; CRegKey::Read
EXTRN	?AfxMessageBox@@YGHPBGII@Z:NEAR			; AfxMessageBox
EXTRN	?AfxGetAppName@@YGPBGXZ:NEAR			; AfxGetAppName
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	??H@YG?AVCString@@ABV0@0@Z:NEAR			; operator+
EXTRN	??H@YG?AVCString@@ABV0@PBG@Z:NEAR		; operator+
EXTRN	??H@YG?AVCString@@PBGABV0@@Z:NEAR		; operator+
EXTRN	?CompareNoCase@CString@@QBEHPBG@Z:NEAR		; CString::CompareNoCase
EXTRN	?Format@CString@@QAAXPBGZZ:NEAR			; CString::Format
EXTRN	__imp__SHChangeNotify@16:NEAR
_BSS	SEGMENT
$SG74518 DW	01H DUP (?)
	ALIGN	4

$SG74524 DW	01H DUP (?)
	ALIGN	4

$SG74527 DW	01H DUP (?)
	ALIGN	4

$SG74547 DW	01H DUP (?)
	ALIGN	4

$SG74556 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\FileRegister.cpp
data	SEGMENT
$SG74507 DB	'D:\_CODE\Shared\FileRegister.cpp', 00H
	ORG $+3
$SG74512 DB	'D:\_CODE\Shared\FileRegister.cpp', 00H
	ORG $+3
$SG74521 DB	'T', 00H, 'h', 00H, 'e', 00H, ' ', 00H, 'f', 00H, 'i', 00H
	DB	'l', 00H, 'e', 00H, ' ', 00H, 'e', 00H, 'x', 00H, 't', 00H, 'e'
	DB	00H, 'n', 00H, 's', 00H, 'i', 00H, 'o', 00H, 'n', 00H, ' ', 00H
	DB	'%', 00H, 's', 00H, ' ', 00H, 'i', 00H, 's', 00H, ' ', 00H, 'u'
	DB	00H, 's', 00H, 'e', 00H, 'd', 00H, ' ', 00H, 'b', 00H, 'y', 00H
	DB	' ', 00H, '%', 00H, 's', 00H, ' ', 00H, 'f', 00H, 'o', 00H, 'r'
	DB	00H, ' ', 00H, 'i', 00H, 't', 00H, 's', 00H, ' ', 00H, '%', 00H
	DB	's', 00H, '.', 00H, 0aH, 00H, 0aH, 00H, 'W', 00H, 'o', 00H, 'u'
	DB	00H, 'l', 00H, 'd', 00H, ' ', 00H, 'y', 00H, 'o', 00H, 'u', 00H
	DB	' ', 00H, 'l', 00H, 'i', 00H, 'k', 00H, 'e', 00H, ' ', 00H, '%'
	DB	00H, 's', 00H, ' ', 00H, 't', 00H, 'o', 00H, ' ', 00H, 'b', 00H
	DB	'e', 00H, ' ', 00H, 't', 00H, 'h', 00H, 'e', 00H, ' ', 00H, 'd'
	DB	00H, 'e', 00H, 'f', 00H, 'a', 00H, 'u', 00H, 'l', 00H, 't', 00H
	DB	' ', 00H, 'a', 00H, 'p', 00H, 'p', 00H, 'l', 00H, 'i', 00H, 'c'
	DB	00H, 'a', 00H, 't', 00H, 'i', 00H, 'o', 00H, 'n', 00H, ' ', 00H
	DB	'f', 00H, 'o', 00H, 'r', 00H, ' ', 00H, 't', 00H, 'h', 00H, 'i'
	DB	00H, 's', 00H, ' ', 00H, 'f', 00H, 'i', 00H, 'l', 00H, 'e', 00H
	DB	' ', 00H, 't', 00H, 'y', 00H, 'p', 00H, 'e', 00H, '.', 00H, 00H
	DB	00H
	ORG $+2
$SG74532 DB	'\', 00H, 's', 00H, 'h', 00H, 'e', 00H, 'l', 00H, 'l', 00H
	DB	'\', 00H, 'o', 00H, 'p', 00H, 'e', 00H, 'n', 00H, '\', 00H, 'c'
	DB	00H, 'o', 00H, 'm', 00H, 'm', 00H, 'a', 00H, 'n', 00H, 'd', 00H
	DB	00H, 00H
$SG74540 DB	'"', 00H, ' ', 00H, '"', 00H, '%', 00H, '1', 00H, '"', 00H
	DB	' ', 00H, 00H, 00H
$SG74541 DB	'"', 00H, 00H, 00H
$SG74545 DB	'"', 00H, ' ', 00H, '"', 00H, '%', 00H, '1', 00H, '"', 00H
	DB	00H, 00H
	ORG $+2
$SG74546 DB	'"', 00H, 00H, 00H
$SG74553 DB	'\', 00H, 'D', 00H, 'e', 00H, 'f', 00H, 'a', 00H, 'u', 00H
	DB	'l', 00H, 't', 00H, 'I', 00H, 'c', 00H, 'o', 00H, 'n', 00H, 00H
	DB	00H
	ORG $+2
$SG74555 DB	'%', 00H, 's', 00H, ',', 00H, '%', 00H, 'd', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74867	DD	019930520H
	DD	0eH
	DD	FLAT:$T74869
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74869	DD	0ffffffffH
	DD	FLAT:$L74852
	DD	00H
	DD	FLAT:$L74853
	DD	01H
	DD	FLAT:$L74854
	DD	02H
	DD	FLAT:$L74855
	DD	03H
	DD	FLAT:$L74856
	DD	03H
	DD	FLAT:$L74857
	DD	05H
	DD	FLAT:$L74858
	DD	06H
	DD	FLAT:$L74859
	DD	07H
	DD	FLAT:$L74860
	DD	08H
	DD	FLAT:$L74861
	DD	05H
	DD	FLAT:$L74862
	DD	0aH
	DD	FLAT:$L74863
	DD	03H
	DD	FLAT:$L74864
	DD	03H
	DD	FLAT:$L74865
xdata$x	ENDS
;	COMDAT ?RegisterFileType@CFileRegister@@QAEHPBGHH0H@Z
_TEXT	SEGMENT
$T74840 = -64
$T74841 = -68
$T74842 = -72
$T74843 = -76
$T74844 = -80
$T74845 = -84
$T74846 = -88
$T74847 = -92
$T74848 = -96
$T74849 = -100
$T74850 = -104
$T74851 = -108
__$EHRec$ = -12
_szFileDesc$ = 8
_nIcon$ = 12
_bAllowShellOpen$ = 16
_szParams$ = 20
_bAskUser$ = 24
_this$ = -112
_reg$ = -52
_sKey$ = -20
_sEntry$ = -36
_nRet$ = -28
_sMessage$ = -16
_bSuccess$ = -24
_bChange$ = -32
_sShellOpen$74533 = -56
_sIconPath$74554 = -60
?RegisterFileType@CFileRegister@@QAEHPBGHH0H@Z PROC NEAR ; CFileRegister::RegisterFileType, COMDAT

; 33   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74868
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 164				; 000000a4H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-176]
	mov	ecx, 41					; 00000029H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 34   : 	CRegKey reg;

	lea	ecx, DWORD PTR _reg$[ebp]
	call	??0CRegKey@@QAE@XZ			; CRegKey::CRegKey
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 35   : 	CString sKey;

	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 36   : 	CString sEntry;

	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 37   : 	int nRet = IDYES;

	mov	DWORD PTR _nRet$[ebp], 6

; 38   : 	CString sMessage;

	lea	ecx, DWORD PTR _sMessage$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3

; 39   : 	BOOL bSuccess = TRUE, bChange = FALSE;

	mov	DWORD PTR _bSuccess$[ebp], 1
	mov	DWORD PTR _bChange$[ebp], 0
$L74503:

; 40   : 
; 41   : 	ASSERT (!m_sExt.IsEmpty());

	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L74506
	push	41					; 00000029H
	push	OFFSET FLAT:$SG74507
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74506
	int	3
$L74506:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74503
$L74505:

; 42   : 	ASSERT (!m_sFileType.IsEmpty());

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L74511
	push	42					; 0000002aH
	push	OFFSET FLAT:$SG74512
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74511
	int	3
$L74511:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74505

; 43   : 
; 44   : 	if (m_sExt.IsEmpty() || m_sFileType.IsEmpty())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L74514
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L74513
$L74514:

; 45   : 		return FALSE;

	mov	DWORD PTR $T74840[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sMessage$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74840[ebp]
	jmp	$L74495
$L74513:

; 46   : 
; 47   : 	if (reg.Open(HKEY_CLASSES_ROOT, m_sExt) == ERROR_SUCCESS)

	mov	ecx, DWORD PTR _this$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	-2147483648				; 80000000H
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open
	test	eax, eax
	jne	$L74517

; 49   : 		reg.Read(_T(""), sEntry);

	lea	edx, DWORD PTR _sEntry$[ebp]
	push	edx
	push	OFFSET FLAT:$SG74518
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Read@CRegKey@@QBEJPBGAAVCString@@@Z	; CRegKey::Read

; 50   : 
; 51   : 		// if the current filetype is not correct query the use to reset it
; 52   : 		if (!sEntry.IsEmpty() && sEntry.CompareNoCase(m_sFileType) != 0)

	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L74519
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	?CompareNoCase@CString@@QBEHPBG@Z	; CString::CompareNoCase
	test	eax, eax
	je	SHORT $L74519

; 54   : 			if (bAskUser)

	cmp	DWORD PTR _bAskUser$[ebp], 0
	je	SHORT $L74520

; 56   : 				sMessage.Format(_T("The file extension %s is used by %s for its %s.\n\nWould you like %s to be the default application for this file type."),
; 57   : 								m_sExt, AfxGetAppName(), szFileDesc, AfxGetAppName());

	call	?AfxGetAppName@@YGPBGXZ			; AfxGetAppName
	push	eax
	mov	eax, DWORD PTR _szFileDesc$[ebp]
	push	eax
	call	?AfxGetAppName@@YGPBGXZ			; AfxGetAppName
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx]
	push	edx
	push	OFFSET FLAT:$SG74521
	lea	eax, DWORD PTR _sMessage$[ebp]
	push	eax
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 24					; 00000018H

; 58   : 				
; 59   : 				nRet = AfxMessageBox(sMessage, MB_YESNO | MB_ICONQUESTION);

	push	0
	push	36					; 00000024H
	lea	ecx, DWORD PTR _sMessage$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?AfxMessageBox@@YGHPBGII@Z		; AfxMessageBox
	mov	DWORD PTR _nRet$[ebp], eax
$L74520:

; 61   : 
; 62   : 			bChange = TRUE;

	mov	DWORD PTR _bChange$[ebp], 1

; 64   : 		else

	jmp	SHORT $L74522
$L74519:

; 65   : 			bChange = sEntry.IsEmpty();

	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	mov	DWORD PTR _bChange$[ebp], eax
$L74522:

; 66   : 
; 67   : 		// if not no then set
; 68   : 		if (nRet != IDNO)

	cmp	DWORD PTR _nRet$[ebp], 7
	je	$L74523

; 70   : 			bSuccess = (reg.Write(_T(""), m_sFileType) == ERROR_SUCCESS);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	OFFSET FLAT:$SG74524
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Write@CRegKey@@QAEJPBG0@Z		; CRegKey::Write
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR _bSuccess$[ebp], eax

; 71   : 
; 72   : 			reg.Close();

	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Close@CRegKey@@QAEXXZ			; CRegKey::Close

; 73   : 
; 74   : 			if (reg.Open(HKEY_CLASSES_ROOT, m_sFileType) == ERROR_SUCCESS)

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	-2147483648				; 80000000H
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open
	test	eax, eax
	jne	$L74526

; 76   : 				bSuccess &= (reg.Write(_T(""), szFileDesc) == ERROR_SUCCESS);

	mov	ecx, DWORD PTR _szFileDesc$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG74527
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Write@CRegKey@@QAEJPBG0@Z		; CRegKey::Write
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	edx, DWORD PTR _bSuccess$[ebp]
	and	edx, eax
	mov	DWORD PTR _bSuccess$[ebp], edx

; 77   : 				reg.Close();

	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Close@CRegKey@@QAEXXZ			; CRegKey::Close

; 78   : 
; 79   : 				// app to open file
; 80   : 				if (reg.Open(HKEY_CLASSES_ROOT, m_sFileType + _T("\\shell\\open\\command")) == ERROR_SUCCESS)

	push	OFFSET FLAT:$SG74532
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 4
	push	eax
	lea	ecx, DWORD PTR $T74842[ebp]
	push	ecx
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -116+[ebp], eax
	mov	edx, DWORD PTR -116+[ebp]
	mov	DWORD PTR -120+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	ecx, DWORD PTR -120+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	-2147483648				; 80000000H
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	BYTE PTR $T74841[ebp], al
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR $T74842[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74841[ebp]
	and	eax, 255				; 000000ffH
	test	eax, eax
	je	$L74530

; 82   : 					CString sShellOpen;

	lea	ecx, DWORD PTR _sShellOpen$74533[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 5

; 83   : 
; 84   : 					if (bAllowShellOpen)

	cmp	DWORD PTR _bAllowShellOpen$[ebp], 0
	je	$L74542

; 86   : 						if (szParams)

	cmp	DWORD PTR _szParams$[ebp], 0
	je	$L74535

; 87   : 							sShellOpen = _T("\"") + m_sAppPath + _T("\" \"%1\" ") + CString(szParams);

	mov	ecx, DWORD PTR _szParams$[ebp]
	push	ecx
	lea	ecx, DWORD PTR $T74843[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR -124+[ebp], eax
	mov	edx, DWORD PTR -124+[ebp]
	mov	DWORD PTR -128+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	mov	eax, DWORD PTR -128+[ebp]
	push	eax
	push	OFFSET FLAT:$SG74540
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	push	ecx
	push	OFFSET FLAT:$SG74541
	lea	edx, DWORD PTR $T74844[ebp]
	push	edx
	call	??H@YG?AVCString@@PBGABV0@@Z		; operator+
	mov	DWORD PTR -132+[ebp], eax
	mov	eax, DWORD PTR -132+[ebp]
	mov	DWORD PTR -136+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	mov	ecx, DWORD PTR -136+[ebp]
	push	ecx
	lea	edx, DWORD PTR $T74845[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -140+[ebp], eax
	mov	eax, DWORD PTR -140+[ebp]
	mov	DWORD PTR -144+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 8
	mov	ecx, DWORD PTR -144+[ebp]
	push	ecx
	lea	edx, DWORD PTR $T74846[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -148+[ebp], eax
	mov	eax, DWORD PTR -148+[ebp]
	mov	DWORD PTR -152+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 9
	mov	ecx, DWORD PTR -152+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sShellOpen$74533[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 8
	lea	ecx, DWORD PTR $T74846[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	lea	ecx, DWORD PTR $T74845[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR $T74844[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	lea	ecx, DWORD PTR $T74843[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 88   : 						else

	jmp	SHORT $L74542
$L74535:

; 89   : 							sShellOpen = _T("\"") + m_sAppPath + _T("\" \"%1\"");

	push	OFFSET FLAT:$SG74545
	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 8
	push	edx
	push	OFFSET FLAT:$SG74546
	lea	eax, DWORD PTR $T74847[ebp]
	push	eax
	call	??H@YG?AVCString@@PBGABV0@@Z		; operator+
	mov	DWORD PTR -156+[ebp], eax
	mov	ecx, DWORD PTR -156+[ebp]
	mov	DWORD PTR -160+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 10		; 0000000aH
	mov	edx, DWORD PTR -160+[ebp]
	push	edx
	lea	eax, DWORD PTR $T74848[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -164+[ebp], eax
	mov	ecx, DWORD PTR -164+[ebp]
	mov	DWORD PTR -168+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 11		; 0000000bH
	mov	edx, DWORD PTR -168+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sShellOpen$74533[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 10		; 0000000aH
	lea	ecx, DWORD PTR $T74848[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	lea	ecx, DWORD PTR $T74847[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74542:

; 91   : 
; 92   : 					bSuccess &= (reg.Write(_T(""), sShellOpen) == ERROR_SUCCESS);

	lea	ecx, DWORD PTR _sShellOpen$74533[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	OFFSET FLAT:$SG74547
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Write@CRegKey@@QAEJPBG0@Z		; CRegKey::Write
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	ecx, DWORD PTR _bSuccess$[ebp]
	and	ecx, eax
	mov	DWORD PTR _bSuccess$[ebp], ecx

; 93   : 				}

	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR _sShellOpen$74533[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 94   : 				else

	jmp	SHORT $L74548
$L74530:

; 95   : 					bSuccess = FALSE;

	mov	DWORD PTR _bSuccess$[ebp], 0
$L74548:

; 96   : 
; 97   : 				// icons
; 98   : 				reg.Close();

	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Close@CRegKey@@QAEXXZ			; CRegKey::Close

; 99   : 
; 100  : 				if (reg.Open(HKEY_CLASSES_ROOT, m_sFileType + _T("\\DefaultIcon")) == ERROR_SUCCESS)

	push	OFFSET FLAT:$SG74553
	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 4
	push	edx
	lea	eax, DWORD PTR $T74850[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	DWORD PTR -172+[ebp], eax
	mov	ecx, DWORD PTR -172+[ebp]
	mov	DWORD PTR -176+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 12		; 0000000cH
	mov	ecx, DWORD PTR -176+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	-2147483648				; 80000000H
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	BYTE PTR $T74849[ebp], al
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR $T74850[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	edx, DWORD PTR $T74849[ebp]
	and	edx, 255				; 000000ffH
	test	edx, edx
	je	SHORT $L74551

; 102  : 					CString sIconPath;

	lea	ecx, DWORD PTR _sIconPath$74554[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 13		; 0000000dH

; 103  : 					sIconPath.Format(_T("%s,%d"), m_sAppPath, nIcon);

	mov	eax, DWORD PTR _nIcon$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	push	OFFSET FLAT:$SG74555
	lea	eax, DWORD PTR _sIconPath$74554[ebp]
	push	eax
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 16					; 00000010H

; 104  : 					bSuccess &= (reg.Write(_T(""), sIconPath) == ERROR_SUCCESS);

	lea	ecx, DWORD PTR _sIconPath$74554[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	OFFSET FLAT:$SG74556
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Write@CRegKey@@QAEJPBG0@Z		; CRegKey::Write
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	ecx, DWORD PTR _bSuccess$[ebp]
	and	ecx, eax
	mov	DWORD PTR _bSuccess$[ebp], ecx

; 105  : 				}

	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR _sIconPath$74554[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 106  : 				else

	jmp	SHORT $L74557
$L74551:

; 107  : 					bSuccess = FALSE;

	mov	DWORD PTR _bSuccess$[ebp], 0
$L74557:

; 109  : 			else

	jmp	SHORT $L74558
$L74526:

; 110  : 				bSuccess = FALSE; 

	mov	DWORD PTR _bSuccess$[ebp], 0
$L74558:

; 112  : 		else

	jmp	SHORT $L74559
$L74523:

; 113  : 			bSuccess = FALSE;

	mov	DWORD PTR _bSuccess$[ebp], 0
$L74559:

; 115  : 	else

	jmp	SHORT $L74560
$L74517:

; 116  : 		bSuccess = FALSE;

	mov	DWORD PTR _bSuccess$[ebp], 0
$L74560:

; 117  : 
; 118  : 	if (bSuccess && bChange)

	cmp	DWORD PTR _bSuccess$[ebp], 0
	je	SHORT $L74561
	cmp	DWORD PTR _bChange$[ebp], 0
	je	SHORT $L74561

; 119  : 		SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);

	mov	esi, esp
	push	0
	push	0
	push	0
	push	134217728				; 08000000H
	call	DWORD PTR __imp__SHChangeNotify@16
	cmp	esi, esp
	call	__chkesp
$L74561:

; 120  : 
; 121  : 	return bSuccess;

	mov	edx, DWORD PTR _bSuccess$[ebp]
	mov	DWORD PTR $T74851[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sMessage$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74851[ebp]
$L74495:

; 122  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 176				; 000000b0H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	20					; 00000014H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74852:
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	ret	0
$L74853:
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74854:
	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74855:
	lea	ecx, DWORD PTR _sMessage$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74856:
	lea	ecx, DWORD PTR $T74842[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74857:
	lea	ecx, DWORD PTR _sShellOpen$74533[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74858:
	lea	ecx, DWORD PTR $T74843[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74859:
	lea	ecx, DWORD PTR $T74844[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74860:
	lea	ecx, DWORD PTR $T74845[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74861:
	lea	ecx, DWORD PTR $T74846[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74862:
	lea	ecx, DWORD PTR $T74847[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74863:
	lea	ecx, DWORD PTR $T74848[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74864:
	lea	ecx, DWORD PTR $T74850[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74865:
	lea	ecx, DWORD PTR _sIconPath$74554[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74868:
	mov	eax, OFFSET FLAT:$T74867
	jmp	___CxxFrameHandler
text$x	ENDS
?RegisterFileType@CFileRegister@@QAEHPBGHH0H@Z ENDP	; CFileRegister::RegisterFileType
PUBLIC	?UnRegisterFileType@CFileRegister@@QAEHXZ	; CFileRegister::UnRegisterFileType
EXTRN	?DeleteKey@CRegKey@@SAJPAUHKEY__@@PBG@Z:NEAR	; CRegKey::DeleteKey
_BSS	SEGMENT
	ALIGN	4

$SG74579 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\FileRegister.cpp
data	SEGMENT
$SG74574 DB	'D:\_CODE\Shared\FileRegister.cpp', 00H
	ORG $+3
$SG74586 DB	'D:\_CODE\Shared\FileRegister.cpp', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74884	DD	019930520H
	DD	03H
	DD	FLAT:$T74886
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74886	DD	0ffffffffH
	DD	FLAT:$L74880
	DD	00H
	DD	FLAT:$L74881
	DD	01H
	DD	FLAT:$L74882
xdata$x	ENDS
;	COMDAT ?UnRegisterFileType@CFileRegister@@QAEHXZ
_TEXT	SEGMENT
$T74875 = -44
$T74876 = -48
$T74877 = -52
$T74878 = -56
$T74879 = -60
__$EHRec$ = -12
_this$ = -64
_reg$ = -40
_sKey$ = -16
_sEntry$ = -24
_bSuccess$ = -20
?UnRegisterFileType@CFileRegister@@QAEHXZ PROC NEAR	; CFileRegister::UnRegisterFileType, COMDAT

; 125  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74885
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 52					; 00000034H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-64]
	mov	ecx, 13					; 0000000dH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 126  : 	CRegKey reg;

	lea	ecx, DWORD PTR _reg$[ebp]
	call	??0CRegKey@@QAE@XZ			; CRegKey::CRegKey
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 127  : 	CString sKey;

	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 128  : 	CString sEntry;

	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 129  : 	BOOL bSuccess = FALSE;

	mov	DWORD PTR _bSuccess$[ebp], 0
$L74570:

; 130  : 
; 131  : 	ASSERT (!m_sExt.IsEmpty());

	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L74573
	push	131					; 00000083H
	push	OFFSET FLAT:$SG74574
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74573
	int	3
$L74573:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74570

; 132  : 
; 133  : 	if (m_sExt.IsEmpty())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L74575

; 134  : 		return FALSE;

	mov	DWORD PTR $T74875[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74875[ebp]
	jmp	$L74565
$L74575:

; 135  : 
; 136  : 	if (reg.Open(HKEY_CLASSES_ROOT, m_sExt) == ERROR_SUCCESS)

	mov	ecx, DWORD PTR _this$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	-2147483648				; 80000000H
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open
	test	eax, eax
	jne	$L74578

; 138  : 		reg.Read(_T(""), sEntry);

	lea	ecx, DWORD PTR _sEntry$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG74579
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Read@CRegKey@@QBEJPBGAAVCString@@@Z	; CRegKey::Read

; 139  : 
; 140  : 		if (sEntry.IsEmpty())

	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L74580

; 141  : 			return TRUE; // we werent the register app so all's well

	mov	DWORD PTR $T74876[ebp], 1
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74876[ebp]
	jmp	$L74565
$L74580:

; 142  : 
; 143  : 		ASSERT (!m_sFileType.IsEmpty());

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L74585
	push	143					; 0000008fH
	push	OFFSET FLAT:$SG74586
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74585
	int	3
$L74585:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74580

; 144  : 
; 145  : 		if (m_sFileType.IsEmpty())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L74587

; 146  : 			return FALSE;

	mov	DWORD PTR $T74877[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74877[ebp]
	jmp	$L74565
$L74587:

; 147  : 
; 148  : 		if (sEntry.CompareNoCase(m_sFileType) != 0)

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	?CompareNoCase@CString@@QBEHPBG@Z	; CString::CompareNoCase
	test	eax, eax
	je	SHORT $L74589

; 149  : 			return TRUE; // we werent the register app so all's well

	mov	DWORD PTR $T74878[ebp], 1
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74878[ebp]
	jmp	$L74565
$L74589:

; 150  : 
; 151  : 		// else delete the keys
; 152  : 		reg.Close();

	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Close@CRegKey@@QAEXXZ			; CRegKey::Close

; 153  : 		CRegKey::DeleteKey(HKEY_CLASSES_ROOT, m_sExt);

	mov	ecx, DWORD PTR _this$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	-2147483648				; 80000000H
	call	?DeleteKey@CRegKey@@SAJPAUHKEY__@@PBG@Z	; CRegKey::DeleteKey
	add	esp, 8

; 154  : 		CRegKey::DeleteKey(HKEY_CLASSES_ROOT, m_sFileType);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	-2147483648				; 80000000H
	call	?DeleteKey@CRegKey@@SAJPAUHKEY__@@PBG@Z	; CRegKey::DeleteKey
	add	esp, 8

; 155  : 		SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);

	mov	esi, esp
	push	0
	push	0
	push	0
	push	134217728				; 08000000H
	call	DWORD PTR __imp__SHChangeNotify@16
	cmp	esi, esp
	call	__chkesp

; 156  : 
; 157  : 		bSuccess = TRUE;

	mov	DWORD PTR _bSuccess$[ebp], 1
$L74578:

; 159  : 
; 160  : 	return bSuccess;

	mov	eax, DWORD PTR _bSuccess$[ebp]
	mov	DWORD PTR $T74879[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T74879[ebp]
$L74565:

; 161  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 64					; 00000040H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74880:
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	ret	0
$L74881:
	lea	ecx, DWORD PTR _sKey$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74882:
	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74885:
	mov	eax, OFFSET FLAT:$T74884
	jmp	___CxxFrameHandler
text$x	ENDS
?UnRegisterFileType@CFileRegister@@QAEHXZ ENDP		; CFileRegister::UnRegisterFileType
PUBLIC	?GetRegisteredAppPath@CFileRegister@@SA?AVCString@@PBGH@Z ; CFileRegister::GetRegisteredAppPath
PUBLIC	?IsRegisteredApp@CFileRegister@@SAHPBG0H@Z	; CFileRegister::IsRegisteredApp
EXTRN	?GetFileNameFromPath@FileMisc@@YA?AVCString@@PBGH@Z:NEAR ; FileMisc::GetFileNameFromPath
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
;	COMDAT xdata$x
; File D:\_CODE\Shared\FileRegister.cpp
xdata$x	SEGMENT
$T74908	DD	019930520H
	DD	06H
	DD	FLAT:$T74910
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74910	DD	0ffffffffH
	DD	FLAT:$L74898
	DD	00H
	DD	FLAT:$L74900
	DD	01H
	DD	FLAT:$L74902
	DD	02H
	DD	FLAT:$L74904
	DD	00H
	DD	FLAT:$L74904
	DD	04H
	DD	FLAT:$L74902
xdata$x	ENDS
;	COMDAT ?IsRegisteredApp@CFileRegister@@SAHPBG0H@Z
_TEXT	SEGMENT
$T74892 = -24
$T74893 = -28
$T74894 = -32
$T74897 = -36
$T74899 = -40
__$EHRec$ = -12
_szExt$ = 8
_szAppPath$ = 12
_bFilenameOnly$ = 16
_sRegAppPath$ = -16
_sAppPath$ = -20
?IsRegisteredApp@CFileRegister@@SAHPBG0H@Z PROC NEAR	; CFileRegister::IsRegisteredApp, COMDAT

; 165  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74909
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 48					; 00000030H
	push	edi
	lea	edi, DWORD PTR [ebp-60]
	mov	ecx, 12					; 0000000cH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T74899[ebp], 0

; 166  : 	CString sRegAppPath = GetRegisteredAppPath(szExt, bFilenameOnly);

	mov	eax, DWORD PTR _bFilenameOnly$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szExt$[ebp]
	push	ecx
	lea	edx, DWORD PTR _sRegAppPath$[ebp]
	push	edx
	call	?GetRegisteredAppPath@CFileRegister@@SA?AVCString@@PBGH@Z ; CFileRegister::GetRegisteredAppPath
	add	esp, 12					; 0000000cH
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 167  : 	CString sAppPath = (bFilenameOnly ? FileMisc::GetFileNameFromPath(szAppPath) : szAppPath);

	cmp	DWORD PTR _bFilenameOnly$[ebp], 0
	je	SHORT $L74895
	push	1
	mov	eax, DWORD PTR _szAppPath$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T74893[ebp]
	push	ecx
	call	?GetFileNameFromPath@FileMisc@@YA?AVCString@@PBGH@Z ; FileMisc::GetFileNameFromPath
	add	esp, 12					; 0000000cH
	mov	DWORD PTR -44+[ebp], eax
	mov	edx, DWORD PTR $T74899[ebp]
	or	edx, 1
	mov	DWORD PTR $T74899[ebp], edx
	mov	eax, DWORD PTR -44+[ebp]
	mov	DWORD PTR -48+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -48+[ebp]
	mov	DWORD PTR -52+[ebp], ecx
	jmp	SHORT $L74896
$L74895:
	mov	edx, DWORD PTR _szAppPath$[ebp]
	push	edx
	lea	ecx, DWORD PTR $T74894[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR -56+[ebp], eax
	mov	eax, DWORD PTR $T74899[ebp]
	or	al, 2
	mov	DWORD PTR $T74899[ebp], eax
	mov	ecx, DWORD PTR -56+[ebp]
	mov	DWORD PTR -60+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 2
	mov	edx, DWORD PTR -60+[ebp]
	mov	DWORD PTR -52+[ebp], edx
$L74896:
	mov	eax, DWORD PTR -52+[ebp]
	mov	DWORD PTR $T74892[ebp], eax
	mov	ecx, DWORD PTR $T74892[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sAppPath$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 5
	mov	edx, DWORD PTR $T74899[ebp]
	and	edx, 2
	test	edx, edx
	je	SHORT $L74905
	and	DWORD PTR $T74899[ebp], -3		; fffffffdH
	lea	ecx, DWORD PTR $T74894[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74905:
	mov	DWORD PTR __$EHRec$[ebp+8], 4
	mov	eax, DWORD PTR $T74899[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L74906
	and	DWORD PTR $T74899[ebp], -2		; fffffffeH
	lea	ecx, DWORD PTR $T74893[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74906:

; 168  : 
; 169  : 	return (sAppPath.CompareNoCase(sRegAppPath) == 0);

	lea	ecx, DWORD PTR _sRegAppPath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sAppPath$[ebp]
	call	?CompareNoCase@CString@@QBEHPBG@Z	; CString::CompareNoCase
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR $T74897[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sAppPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sRegAppPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74897[ebp]

; 170  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 60					; 0000003cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74898:
	lea	ecx, DWORD PTR _sRegAppPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74900:
	mov	eax, DWORD PTR $T74899[ebp]
	and	eax, 1
	test	eax, eax
	je	$L74901
	lea	ecx, DWORD PTR $T74893[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74901:
	ret	0
$L74902:
	mov	eax, DWORD PTR $T74899[ebp]
	and	eax, 2
	test	eax, eax
	je	$L74903
	lea	ecx, DWORD PTR $T74894[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74903:
	ret	0
$L74904:
	lea	ecx, DWORD PTR _sAppPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74909:
	mov	eax, OFFSET FLAT:$T74908
	jmp	___CxxFrameHandler
text$x	ENDS
?IsRegisteredApp@CFileRegister@@SAHPBG0H@Z ENDP		; CFileRegister::IsRegisteredApp
PUBLIC	?IsRegisteredAppInstalled@CFileRegister@@SAHPBG@Z ; CFileRegister::IsRegisteredAppInstalled
EXTRN	?FileExists@FileMisc@@YAHPBG@Z:NEAR		; FileMisc::FileExists
;	COMDAT xdata$x
; File D:\_CODE\Shared\FileRegister.cpp
xdata$x	SEGMENT
$T74918	DD	019930520H
	DD	01H
	DD	FLAT:$T74920
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74920	DD	0ffffffffH
	DD	FLAT:$L74916
xdata$x	ENDS
;	COMDAT ?IsRegisteredAppInstalled@CFileRegister@@SAHPBG@Z
_TEXT	SEGMENT
$T74915 = -20
__$EHRec$ = -12
_szExt$ = 8
_sRegAppPath$ = -16
?IsRegisteredAppInstalled@CFileRegister@@SAHPBG@Z PROC NEAR ; CFileRegister::IsRegisteredAppInstalled, COMDAT

; 173  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74919
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH

; 174  : 	CString sRegAppPath = GetRegisteredAppPath(szExt);

	push	0
	mov	eax, DWORD PTR _szExt$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sRegAppPath$[ebp]
	push	ecx
	call	?GetRegisteredAppPath@CFileRegister@@SA?AVCString@@PBGH@Z ; CFileRegister::GetRegisteredAppPath
	add	esp, 12					; 0000000cH
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 175  : 	return FileMisc::FileExists(sRegAppPath);

	lea	ecx, DWORD PTR _sRegAppPath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?FileExists@FileMisc@@YAHPBG@Z		; FileMisc::FileExists
	add	esp, 4
	mov	DWORD PTR $T74915[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sRegAppPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74915[ebp]

; 176  : }

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
$L74916:
	lea	ecx, DWORD PTR _sRegAppPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74919:
	mov	eax, OFFSET FLAT:$T74918
	jmp	___CxxFrameHandler
text$x	ENDS
?IsRegisteredAppInstalled@CFileRegister@@SAHPBG@Z ENDP	; CFileRegister::IsRegisteredAppInstalled
EXTRN	?IsPath@FileMisc@@YAHPBG@Z:NEAR			; FileMisc::IsPath
EXTRN	?GetExtension@FileMisc@@YA?AVCString@@PBG@Z:NEAR ; FileMisc::GetExtension
EXTRN	?SplitPath@FileMisc@@YAXPBGPAVCString@@111@Z:NEAR ; FileMisc::SplitPath
EXTRN	?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z:NEAR ; FileMisc::MakePath
EXTRN	??ACString@@QBEGH@Z:NEAR			; CString::operator[]
EXTRN	??YCString@@QAEABV0@PBG@Z:NEAR			; CString::operator+=
EXTRN	?Mid@CString@@QBE?AV1@HH@Z:NEAR			; CString::Mid
EXTRN	?Find@CString@@QBEHGH@Z:NEAR			; CString::Find
_BSS	SEGMENT
	ALIGN	4

$SG74630 DW	01H DUP (?)
	ALIGN	4

$SG74636 DW	01H DUP (?)
	ALIGN	4

$SG74643 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\FileRegister.cpp
data	SEGMENT
$SG74627 DB	'D:\_CODE\Shared\FileRegister.cpp', 00H
	ORG $+3
$SG74640 DB	'\', 00H, 's', 00H, 'h', 00H, 'e', 00H, 'l', 00H, 'l', 00H
	DB	'\', 00H, 'o', 00H, 'p', 00H, 'e', 00H, 'n', 00H, '\', 00H, 'c'
	DB	00H, 'o', 00H, 'm', 00H, 'm', 00H, 'a', 00H, 'n', 00H, 'd', 00H
	DB	00H, 00H
$SG74651 DB	'D:\_CODE\Shared\FileRegister.cpp', 00H
	ORG $+3
$SG74656 DB	'.', 00H, 'e', 00H, 'x', 00H, 'e', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74942	DD	019930520H
	DD	0bH
	DD	FLAT:$T74944
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74944	DD	0ffffffffH
	DD	FLAT:$L74931
	DD	00H
	DD	FLAT:$L74928
	DD	01H
	DD	FLAT:$L74929
	DD	01H
	DD	FLAT:$L74930
	DD	01H
	DD	FLAT:$L74934
	DD	04H
	DD	FLAT:$L74935
	DD	05H
	DD	FLAT:$L74936
	DD	06H
	DD	FLAT:$L74937
	DD	06H
	DD	FLAT:$L74938
	DD	08H
	DD	FLAT:$L74939
	DD	09H
	DD	FLAT:$L74940
xdata$x	ENDS
;	COMDAT ?GetRegisteredAppPath@CFileRegister@@SA?AVCString@@PBGH@Z
_TEXT	SEGMENT
$T74925 = -60
$T74926 = -64
$T74927 = -68
$T74932 = -72
__$EHRec$ = -12
_szExt$ = 12
_bFilenameOnly$ = 16
___$ReturnUdt$ = 8
_sExt$ = -16
_reg$ = -40
_sEntry$ = -24
_sAppPath$ = -20
_nEnd$74646 = -56
_sDrive$74653 = -44
_sDir$74654 = -48
_sFName$74655 = -52
?GetRegisteredAppPath@CFileRegister@@SA?AVCString@@PBGH@Z PROC NEAR ; CFileRegister::GetRegisteredAppPath, COMDAT

; 179  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74943
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 84					; 00000054H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-96]
	mov	ecx, 21					; 00000015H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T74932[ebp], 0

; 180  : 	CString sExt;

	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 181  : 	
; 182  : 	if (FileMisc::IsPath(szExt))

	mov	eax, DWORD PTR _szExt$[ebp]
	push	eax
	call	?IsPath@FileMisc@@YAHPBG@Z		; FileMisc::IsPath
	add	esp, 4
	test	eax, eax
	je	SHORT $L74618

; 183  : 		sExt = FileMisc::GetExtension(szExt);

	mov	ecx, DWORD PTR _szExt$[ebp]
	push	ecx
	lea	edx, DWORD PTR $T74925[ebp]
	push	edx
	call	?GetExtension@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::GetExtension
	add	esp, 8
	mov	DWORD PTR -76+[ebp], eax
	mov	eax, DWORD PTR -76+[ebp]
	mov	DWORD PTR -80+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR -80+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T74925[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 184  : 	else

	jmp	SHORT $L74620
$L74618:

; 185  : 		sExt = GetExtension(szExt);

	mov	edx, DWORD PTR _szExt$[ebp]
	push	edx
	lea	eax, DWORD PTR $T74926[ebp]
	push	eax
	call	?GetExtension@CFileRegister@@KA?AVCString@@PBG@Z ; CFileRegister::GetExtension
	add	esp, 8
	mov	DWORD PTR -84+[ebp], eax
	mov	ecx, DWORD PTR -84+[ebp]
	mov	DWORD PTR -88+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	edx, DWORD PTR -88+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T74926[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74620:

; 186  : 
; 187  : 	ASSERT (!sExt.IsEmpty() && (sExt[0] == '.'));

	lea	ecx, DWORD PTR _sExt$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L74626
	push	0
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	and	eax, 65535				; 0000ffffH
	cmp	eax, 46					; 0000002eH
	je	SHORT $L74625
$L74626:
	push	187					; 000000bbH
	push	OFFSET FLAT:$SG74627
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74625
	int	3
$L74625:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74620

; 188  : 
; 189  : 	if (sExt.IsEmpty() || (sExt[0] != '.'))

	lea	ecx, DWORD PTR _sExt$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L74629
	push	0
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	and	eax, 65535				; 0000ffffH
	cmp	eax, 46					; 0000002eH
	je	SHORT $L74628
$L74629:

; 190  : 		return _T("");

	push	OFFSET FLAT:$SG74630
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	ecx, DWORD PTR $T74932[ebp]
	or	ecx, 1
	mov	DWORD PTR $T74932[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L74616
$L74628:

; 191  : 
; 192  : 	CRegKey reg;

	lea	ecx, DWORD PTR _reg$[ebp]
	call	??0CRegKey@@QAE@XZ			; CRegKey::CRegKey
	mov	BYTE PTR __$EHRec$[ebp+8], 4

; 193  : 	CString sEntry, sAppPath;

	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	lea	ecx, DWORD PTR _sAppPath$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 6

; 194  : 
; 195  : 	if (reg.Open(HKEY_CLASSES_ROOT, sExt) == ERROR_SUCCESS)

	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	-2147483648				; 80000000H
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open
	test	eax, eax
	jne	$L74642

; 197  : 		reg.Read(_T(""), sEntry);

	lea	edx, DWORD PTR _sEntry$[ebp]
	push	edx
	push	OFFSET FLAT:$SG74636
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Read@CRegKey@@QBEJPBGAAVCString@@@Z	; CRegKey::Read

; 198  : 
; 199  : 		if (!sEntry.IsEmpty())

	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L74642

; 201  : 			reg.Close();

	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Close@CRegKey@@QAEXXZ			; CRegKey::Close

; 202  : 
; 203  : 			if (reg.Open(HKEY_CLASSES_ROOT, sEntry) == ERROR_SUCCESS)

	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	-2147483648				; 80000000H
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open
	test	eax, eax
	jne	SHORT $L74642

; 205  : 				reg.Close();

	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Close@CRegKey@@QAEXXZ			; CRegKey::Close

; 206  : 
; 207  : 				// app to open file
; 208  : 				sEntry += _T("\\shell\\open\\command");

	push	OFFSET FLAT:$SG74640
	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=

; 209  : 
; 210  : 				if (reg.Open(HKEY_CLASSES_ROOT, sEntry) == ERROR_SUCCESS)

	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	-2147483648				; 80000000H
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open
	test	eax, eax
	jne	SHORT $L74642

; 212  : 					reg.Read(_T(""), sAppPath);

	lea	eax, DWORD PTR _sAppPath$[ebp]
	push	eax
	push	OFFSET FLAT:$SG74643
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Read@CRegKey@@QBEJPBGAAVCString@@@Z	; CRegKey::Read

; 213  : 					sAppPath.TrimLeft();

	lea	ecx, DWORD PTR _sAppPath$[ebp]
	call	?TrimLeft@CString@@QAEXXZ		; CString::TrimLeft
$L74642:

; 218  : 
; 219  : 	if (!sAppPath.IsEmpty())

	lea	ecx, DWORD PTR _sAppPath$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	$L74644

; 221  : 		// extract the file path if quoted
; 222  : 		if (sAppPath[0] == '"')

	push	0
	lea	ecx, DWORD PTR _sAppPath$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	and	eax, 65535				; 0000ffffH
	cmp	eax, 34					; 00000022H
	jne	SHORT $L74645

; 224  : 			int nEnd = sAppPath.Find('"', 1);

	push	1
	push	34					; 00000022H
	lea	ecx, DWORD PTR _sAppPath$[ebp]
	call	?Find@CString@@QBEHGH@Z			; CString::Find
	mov	DWORD PTR _nEnd$74646[ebp], eax
$L74647:

; 225  : 			ASSERT(nEnd != -1);

	cmp	DWORD PTR _nEnd$74646[ebp], -1
	jne	SHORT $L74650
	push	225					; 000000e1H
	push	OFFSET FLAT:$SG74651
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74650
	int	3
$L74650:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74647

; 226  : 
; 227  : 			sAppPath = sAppPath.Mid(1, nEnd - 1);

	mov	edx, DWORD PTR _nEnd$74646[ebp]
	sub	edx, 1
	push	edx
	push	1
	lea	eax, DWORD PTR $T74927[ebp]
	push	eax
	lea	ecx, DWORD PTR _sAppPath$[ebp]
	call	?Mid@CString@@QBE?AV1@HH@Z		; CString::Mid
	mov	DWORD PTR -92+[ebp], eax
	mov	ecx, DWORD PTR -92+[ebp]
	mov	DWORD PTR -96+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	mov	edx, DWORD PTR -96+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sAppPath$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR $T74927[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74645:

; 229  : 		
; 230  : 		// note: apps often have parameters after so we do this
; 231  : 		CString sDrive, sDir, sFName;

	lea	ecx, DWORD PTR _sDrive$74653[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 8
	lea	ecx, DWORD PTR _sDir$74654[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 9
	lea	ecx, DWORD PTR _sFName$74655[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 10		; 0000000aH

; 232  : 		FileMisc::SplitPath(sAppPath, &sDrive, &sDir, &sFName);

	push	0
	lea	eax, DWORD PTR _sFName$74655[ebp]
	push	eax
	lea	ecx, DWORD PTR _sDir$74654[ebp]
	push	ecx
	lea	edx, DWORD PTR _sDrive$74653[ebp]
	push	edx
	lea	ecx, DWORD PTR _sAppPath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?SplitPath@FileMisc@@YAXPBGPAVCString@@111@Z ; FileMisc::SplitPath
	add	esp, 20					; 00000014H

; 233  : 		
; 234  : 		sFName += _T(".exe");

	push	OFFSET FLAT:$SG74656
	lea	ecx, DWORD PTR _sFName$74655[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=

; 235  : 		
; 236  : 		if (bFilenameOnly)

	cmp	DWORD PTR _bFilenameOnly$[ebp], 0
	je	SHORT $L74657

; 237  : 			sAppPath = sFName;

	lea	eax, DWORD PTR _sFName$74655[ebp]
	push	eax
	lea	ecx, DWORD PTR _sAppPath$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=

; 238  : 		else

	jmp	SHORT $L74658
$L74657:

; 239  : 			FileMisc::MakePath(sAppPath, sDrive, sDir, sFName);

	push	0
	lea	ecx, DWORD PTR _sFName$74655[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sDir$74654[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sDrive$74653[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _sAppPath$[ebp]
	push	ecx
	call	?MakePath@FileMisc@@YAAAVCString@@AAV2@PBG111@Z ; FileMisc::MakePath
	add	esp, 20					; 00000014H
$L74658:

; 240  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 9
	lea	ecx, DWORD PTR _sFName$74655[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 8
	lea	ecx, DWORD PTR _sDir$74654[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR _sDrive$74653[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74644:

; 241  : 	
; 242  : 	return sAppPath;

	lea	edx, DWORD PTR _sAppPath$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T74932[ebp]
	or	al, 1
	mov	DWORD PTR $T74932[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	lea	ecx, DWORD PTR _sAppPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L74616:

; 243  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 96					; 00000060H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74928:
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74929:
	lea	ecx, DWORD PTR $T74925[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74930:
	lea	ecx, DWORD PTR $T74926[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74931:
	mov	eax, DWORD PTR $T74932[ebp]
	and	eax, 1
	test	eax, eax
	je	$L74933
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74933:
	ret	0
$L74934:
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	ret	0
$L74935:
	lea	ecx, DWORD PTR _sEntry$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74936:
	lea	ecx, DWORD PTR _sAppPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74937:
	lea	ecx, DWORD PTR $T74927[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74938:
	lea	ecx, DWORD PTR _sDrive$74653[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74939:
	lea	ecx, DWORD PTR _sDir$74654[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74940:
	lea	ecx, DWORD PTR _sFName$74655[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74943:
	mov	eax, OFFSET FLAT:$T74942
	jmp	___CxxFrameHandler
text$x	ENDS
?GetRegisteredAppPath@CFileRegister@@SA?AVCString@@PBGH@Z ENDP ; CFileRegister::GetRegisteredAppPath
EXTRN	??0CString@@QAE@PBD@Z:NEAR			; CString::CString
;	COMDAT data
; File D:\_CODE\Shared\FileRegister.cpp
data	SEGMENT
$SG74667 DB	'.', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74959	DD	019930520H
	DD	04H
	DD	FLAT:$T74961
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74961	DD	0ffffffffH
	DD	FLAT:$L74955
	DD	00H
	DD	FLAT:$L74952
	DD	01H
	DD	FLAT:$L74953
	DD	02H
	DD	FLAT:$L74954
xdata$x	ENDS
;	COMDAT ?GetExtension@CFileRegister@@KA?AVCString@@PBG@Z
_TEXT	SEGMENT
$T74950 = -20
$T74951 = -24
$T74956 = -28
__$EHRec$ = -12
_szExt$ = 12
___$ReturnUdt$ = 8
_sExt$ = -16
?GetExtension@CFileRegister@@KA?AVCString@@PBG@Z PROC NEAR ; CFileRegister::GetExtension, COMDAT

; 246  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74960
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR $T74956[ebp], 0

; 247  : 	// ensure leading period
; 248  : 	CString sExt(szExt);

	mov	eax, DWORD PTR _szExt$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 249  : 
; 250  : 	sExt.TrimRight();

	lea	ecx, DWORD PTR _sExt$[ebp]
	call	?TrimRight@CString@@QAEXXZ		; CString::TrimRight

; 251  : 	sExt.TrimLeft();

	lea	ecx, DWORD PTR _sExt$[ebp]
	call	?TrimLeft@CString@@QAEXXZ		; CString::TrimLeft

; 252  : 
; 253  : 	if (!sExt.IsEmpty() && sExt[0] != '.')

	lea	ecx, DWORD PTR _sExt$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L74664
	push	0
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	and	eax, 65535				; 0000ffffH
	cmp	eax, 46					; 0000002eH
	je	SHORT $L74664

; 254  : 		sExt = "." + sExt;

	push	OFFSET FLAT:$SG74667
	lea	ecx, DWORD PTR $T74950[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sExt$[ebp]
	push	ecx
	lea	edx, DWORD PTR $T74950[ebp]
	push	edx
	lea	eax, DWORD PTR $T74951[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -32+[ebp], eax
	mov	ecx, DWORD PTR -32+[ebp]
	mov	DWORD PTR -36+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	edx, DWORD PTR -36+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T74951[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T74950[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74664:

; 255  : 
; 256  : 	return sExt;

	lea	eax, DWORD PTR _sExt$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T74956[ebp]
	or	ecx, 1
	mov	DWORD PTR $T74956[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 257  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74952:
	lea	ecx, DWORD PTR _sExt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74953:
	lea	ecx, DWORD PTR $T74950[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74954:
	lea	ecx, DWORD PTR $T74951[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74955:
	mov	eax, DWORD PTR $T74956[ebp]
	and	eax, 1
	test	eax, eax
	je	$L74957
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74957:
	ret	0
$L74960:
	mov	eax, OFFSET FLAT:$T74959
	jmp	___CxxFrameHandler
text$x	ENDS
?GetExtension@CFileRegister@@KA?AVCString@@PBG@Z ENDP	; CFileRegister::GetExtension
END

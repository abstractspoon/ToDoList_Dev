	TITLE	D:\_CODE\Shared\driveinfo.cpp
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
;	COMDAT ?GetFreeBytes@CDriveInfo@@SAMH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetVolume@CDriveInfo@@SA?AVCString@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetRoot@CDriveInfo@@SA?AVCString@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFullName@CDriveInfo@@SA?AVCString@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTotalBytes@CDriveInfo@@SAMH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetLetter@CDriveInfo@@SADH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetType@CDriveInfo@@SAHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetPathType@CDriveInfo@@SAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FormatName@CDriveInfo@@SAXAAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsDriveAvailable@CDriveInfo@@SAHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsUNCPath@CDriveInfo@@SAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetDrive@CDriveInfo@@SAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsMappedPath@CDriveInfo@@SAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsRemotePath@CDriveInfo@@SAHPBGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsFixedPath@CDriveInfo@@SAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsRemovablePath@CDriveInfo@@SAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsReadonlyPath@CDriveInfo@@SAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSerialNumber@CDriveInfo@@SAKH@Z
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
PUBLIC	?GetFreeBytes@CDriveInfo@@SAMH@Z		; CDriveInfo::GetFreeBytes
PUBLIC	?GetRoot@CDriveInfo@@SA?AVCString@@H@Z		; CDriveInfo::GetRoot
PUBLIC	?IsDriveAvailable@CDriveInfo@@SAHH@Z		; CDriveInfo::IsDriveAvailable
PUBLIC	__real@4@40138000000000000000
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	__imp__GetDiskFreeSpaceW@20:NEAR
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	__chkesp:NEAR
EXTRN	__except_list:DWORD
EXTRN	__fltused:NEAR
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT data
; File D:\_CODE\Shared\driveinfo.cpp
data	SEGMENT
$SG73157 DB	'D:\_CODE\Shared\driveinfo.cpp', 00H
data	ENDS
;	COMDAT __real@4@40138000000000000000
CONST	SEGMENT
__real@4@40138000000000000000 DD 049800000r	; 1.04858e+006
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73469	DD	019930520H
	DD	02H
	DD	FLAT:$T73476
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73476	DD	0ffffffffH
	DD	FLAT:$L73464
	DD	00H
	DD	FLAT:$L73465
xdata$x	ENDS
;	COMDAT ?GetFreeBytes@CDriveInfo@@SAMH@Z
_TEXT	SEGMENT
_nDrive$ = 8
_sRoot$ = -16
_totalClusters$ = -28
_freeClusters$ = -20
_sectors$ = -24
_bytes$ = -32
_fSpace$73160 = -36
$T73461 = -40
$T73462 = -44
$T73463 = -48
__$EHRec$ = -12
?GetFreeBytes@CDriveInfo@@SAMH@Z PROC NEAR		; CDriveInfo::GetFreeBytes, COMDAT

; 9    : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73473
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 68					; 00000044H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-80]
	mov	ecx, 17					; 00000011H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 10   : 	CString sRoot;

	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
$L73152:

; 11   : 	unsigned long totalClusters, freeClusters, sectors, bytes;
; 12   : 
; 13   : 	ASSERT (nDrive > 0 && nDrive <= 26);

	cmp	DWORD PTR _nDrive$[ebp], 0
	jle	SHORT $L73156
	cmp	DWORD PTR _nDrive$[ebp], 26		; 0000001aH
	jle	SHORT $L73155
$L73156:
	push	13					; 0000000dH
	push	OFFSET FLAT:$SG73157
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73155
	int	3
$L73155:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73152

; 14   : 
; 15   : 	if (IsDriveAvailable(nDrive))

	mov	ecx, DWORD PTR _nDrive$[ebp]
	push	ecx
	call	?IsDriveAvailable@CDriveInfo@@SAHH@Z	; CDriveInfo::IsDriveAvailable
	add	esp, 4
	test	eax, eax
	je	$L73158

; 17   : 		sRoot = GetRoot(nDrive);

	mov	edx, DWORD PTR _nDrive$[ebp]
	push	edx
	lea	eax, DWORD PTR $T73461[ebp]
	push	eax
	call	?GetRoot@CDriveInfo@@SA?AVCString@@H@Z	; CDriveInfo::GetRoot
	add	esp, 8
	mov	DWORD PTR -52+[ebp], eax
	mov	ecx, DWORD PTR -52+[ebp]
	mov	DWORD PTR -56+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	edx, DWORD PTR -56+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T73461[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 18   : 	
; 19   : 		::GetDiskFreeSpace(sRoot, &sectors, &bytes, &freeClusters, &totalClusters);

	mov	esi, esp
	lea	eax, DWORD PTR _totalClusters$[ebp]
	push	eax
	lea	ecx, DWORD PTR _freeClusters$[ebp]
	push	ecx
	lea	edx, DWORD PTR _bytes$[ebp]
	push	edx
	lea	eax, DWORD PTR _sectors$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	DWORD PTR __imp__GetDiskFreeSpaceW@20
	cmp	esi, esp
	call	__chkesp

; 20   : 	
; 21   : 		// do maths like this to avoid truncation
; 22   : 		// errors
; 23   : 		float fSpace = (float)sectors;

	mov	ecx, DWORD PTR _sectors$[ebp]
	mov	DWORD PTR -64+[ebp], ecx
	mov	DWORD PTR -64+[ebp+4], 0
	fild	QWORD PTR -64+[ebp]
	fstp	DWORD PTR _fSpace$73160[ebp]

; 24   : 		fSpace *= bytes;

	mov	edx, DWORD PTR _bytes$[ebp]
	mov	DWORD PTR -72+[ebp], edx
	mov	DWORD PTR -72+[ebp+4], 0
	fild	QWORD PTR -72+[ebp]
	fmul	DWORD PTR _fSpace$73160[ebp]
	fstp	DWORD PTR _fSpace$73160[ebp]

; 25   : 		fSpace *= freeClusters;

	mov	eax, DWORD PTR _freeClusters$[ebp]
	mov	DWORD PTR -80+[ebp], eax
	mov	DWORD PTR -80+[ebp+4], 0
	fild	QWORD PTR -80+[ebp]
	fmul	DWORD PTR _fSpace$73160[ebp]
	fst	DWORD PTR _fSpace$73160[ebp]

; 26   : 		fSpace /= 1048576;

	fdiv	DWORD PTR __real@4@40138000000000000000
	fstp	DWORD PTR _fSpace$73160[ebp]

; 27   : 
; 28   : 		return fSpace;

	mov	ecx, DWORD PTR _fSpace$73160[ebp]
	mov	DWORD PTR $T73462[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	fld	DWORD PTR $T73462[ebp]
	jmp	SHORT $L73146
$L73158:

; 30   : 	
; 31   : 	return 0.0f;

	mov	DWORD PTR $T73463[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	fld	DWORD PTR $T73463[ebp]
$L73146:

; 32   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 80					; 00000050H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73464:
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73465:
	lea	ecx, DWORD PTR $T73461[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73473:
	mov	eax, OFFSET FLAT:$T73469
	jmp	___CxxFrameHandler
text$x	ENDS
?GetFreeBytes@CDriveInfo@@SAMH@Z ENDP			; CDriveInfo::GetFreeBytes
PUBLIC	?GetVolume@CDriveInfo@@SA?AVCString@@H@Z	; CDriveInfo::GetVolume
PUBLIC	?FormatName@CDriveInfo@@SAXAAVCString@@@Z	; CDriveInfo::FormatName
EXTRN	__imp__GetVolumeInformationW@32:NEAR
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
EXTRN	??0CString@@QAE@PBD@Z:NEAR			; CString::CString
EXTRN	?GetBuffer@CString@@QAEPAGH@Z:NEAR		; CString::GetBuffer
EXTRN	?ReleaseBuffer@CString@@QAEXH@Z:NEAR		; CString::ReleaseBuffer
_BSS	SEGMENT
$SG73178 DB	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\driveinfo.cpp
data	SEGMENT
$SG73175 DB	'D:\_CODE\Shared\driveinfo.cpp', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73491	DD	019930520H
	DD	04H
	DD	FLAT:$T73493
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73493	DD	0ffffffffH
	DD	FLAT:$L73487
	DD	00H
	DD	FLAT:$L73484
	DD	01H
	DD	FLAT:$L73485
	DD	02H
	DD	FLAT:$L73486
xdata$x	ENDS
;	COMDAT ?GetVolume@CDriveInfo@@SA?AVCString@@H@Z
_TEXT	SEGMENT
_nDrive$ = 12
___$ReturnUdt$ = 8
_sVolume$ = -20
_sRoot$ = -16
$T73483 = -24
$T73488 = -28
__$EHRec$ = -12
?GetVolume@CDriveInfo@@SA?AVCString@@H@Z PROC NEAR	; CDriveInfo::GetVolume, COMDAT

; 35   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73492
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR $T73488[ebp], 0

; 36   : 	CString sVolume, sRoot;

	lea	ecx, DWORD PTR _sVolume$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
$L73170:

; 37   : 
; 38   : 	ASSERT (nDrive > 0 && nDrive <= 26);

	cmp	DWORD PTR _nDrive$[ebp], 0
	jle	SHORT $L73174
	cmp	DWORD PTR _nDrive$[ebp], 26		; 0000001aH
	jle	SHORT $L73173
$L73174:
	push	38					; 00000026H
	push	OFFSET FLAT:$SG73175
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73173
	int	3
$L73173:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73170

; 39   : 
; 40   : 	if (IsDriveAvailable(nDrive))

	mov	ecx, DWORD PTR _nDrive$[ebp]
	push	ecx
	call	?IsDriveAvailable@CDriveInfo@@SAHH@Z	; CDriveInfo::IsDriveAvailable
	add	esp, 4
	test	eax, eax
	je	$L73176

; 42   : 		sRoot = GetRoot(nDrive);

	mov	edx, DWORD PTR _nDrive$[ebp]
	push	edx
	lea	eax, DWORD PTR $T73483[ebp]
	push	eax
	call	?GetRoot@CDriveInfo@@SA?AVCString@@H@Z	; CDriveInfo::GetRoot
	add	esp, 8
	mov	DWORD PTR -32+[ebp], eax
	mov	ecx, DWORD PTR -32+[ebp]
	mov	DWORD PTR -36+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	edx, DWORD PTR -36+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T73483[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 43   : 
; 44   : 		GetVolumeInformation(sRoot, sVolume.GetBuffer(20), 20, NULL, NULL, NULL, NULL, 0);

	mov	esi, esp
	push	0
	push	0
	push	0
	push	0
	push	0
	push	20					; 00000014H
	push	20					; 00000014H
	lea	ecx, DWORD PTR _sVolume$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	DWORD PTR __imp__GetVolumeInformationW@32
	cmp	esi, esp
	call	__chkesp

; 45   : 		sVolume.ReleaseBuffer();

	push	-1
	lea	ecx, DWORD PTR _sVolume$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 46   : 		FormatName(sVolume);

	lea	eax, DWORD PTR _sVolume$[ebp]
	push	eax
	call	?FormatName@CDriveInfo@@SAXAAVCString@@@Z ; CDriveInfo::FormatName
	add	esp, 4

; 47   : 
; 48   : 		return sVolume;

	lea	ecx, DWORD PTR _sVolume$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T73488[ebp]
	or	edx, 1
	mov	DWORD PTR $T73488[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sVolume$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L73167
$L73176:

; 50   : 
; 51   : 	return "";

	push	OFFSET FLAT:$SG73178
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	eax, DWORD PTR $T73488[ebp]
	or	al, 1
	mov	DWORD PTR $T73488[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sVolume$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L73167:

; 52   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73484:
	lea	ecx, DWORD PTR _sVolume$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73485:
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73486:
	lea	ecx, DWORD PTR $T73483[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73487:
	mov	eax, DWORD PTR $T73488[ebp]
	and	eax, 1
	test	eax, eax
	je	$L73489
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L73489:
	ret	0
$L73492:
	mov	eax, OFFSET FLAT:$T73491
	jmp	___CxxFrameHandler
text$x	ENDS
?GetVolume@CDriveInfo@@SA?AVCString@@H@Z ENDP		; CDriveInfo::GetVolume
PUBLIC	?GetLetter@CDriveInfo@@SADH@Z			; CDriveInfo::GetLetter
EXTRN	??4CString@@QAEABV0@D@Z:NEAR			; CString::operator=
EXTRN	??YCString@@QAEABV0@PBG@Z:NEAR			; CString::operator+=
;	COMDAT data
; File D:\_CODE\Shared\driveinfo.cpp
data	SEGMENT
$SG73184 DB	':', 00H, '\', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73504	DD	019930520H
	DD	02H
	DD	FLAT:$T73506
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73506	DD	0ffffffffH
	DD	FLAT:$L73500
	DD	00H
	DD	FLAT:$L73499
xdata$x	ENDS
;	COMDAT ?GetRoot@CDriveInfo@@SA?AVCString@@H@Z
_TEXT	SEGMENT
_nDrive$ = 12
___$ReturnUdt$ = 8
_sRoot$ = -16
$T73501 = -20
__$EHRec$ = -12
?GetRoot@CDriveInfo@@SA?AVCString@@H@Z PROC NEAR	; CDriveInfo::GetRoot, COMDAT

; 55   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73505
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR $T73501[ebp], 0

; 56   : 	CString sRoot;

	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 57   : 
; 58   : 	sRoot = GetLetter(nDrive);

	mov	eax, DWORD PTR _nDrive$[ebp]
	push	eax
	call	?GetLetter@CDriveInfo@@SADH@Z		; CDriveInfo::GetLetter
	add	esp, 4
	push	eax
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??4CString@@QAEABV0@D@Z			; CString::operator=

; 59   : 	sRoot += _T(":\\");

	push	OFFSET FLAT:$SG73184
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=

; 60   : 
; 61   : 	return sRoot;

	lea	ecx, DWORD PTR _sRoot$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T73501[ebp]
	or	edx, 1
	mov	DWORD PTR $T73501[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 62   : }

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
$L73499:
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73500:
	mov	eax, DWORD PTR $T73501[ebp]
	and	eax, 1
	test	eax, eax
	je	$L73502
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L73502:
	ret	0
$L73505:
	mov	eax, OFFSET FLAT:$T73504
	jmp	___CxxFrameHandler
text$x	ENDS
?GetRoot@CDriveInfo@@SA?AVCString@@H@Z ENDP		; CDriveInfo::GetRoot
PUBLIC	?GetFullName@CDriveInfo@@SA?AVCString@@H@Z	; CDriveInfo::GetFullName
EXTRN	??H@YG?AVCString@@ABV0@0@Z:NEAR			; operator+
_BSS	SEGMENT
	ALIGN	4

$SG73207 DB	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\driveinfo.cpp
data	SEGMENT
$SG73197 DB	'D:\_CODE\Shared\driveinfo.cpp', 00H
	ORG $+2
$SG73205 DB	':)', 00H
	ORG $+1
$SG73206 DB	' (', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73531	DD	019930520H
	DD	0aH
	DD	FLAT:$T73533
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73533	DD	0ffffffffH
	DD	FLAT:$L73527
	DD	00H
	DD	FLAT:$L73518
	DD	01H
	DD	FLAT:$L73519
	DD	02H
	DD	FLAT:$L73520
	DD	03H
	DD	FLAT:$L73521
	DD	03H
	DD	FLAT:$L73522
	DD	05H
	DD	FLAT:$L73523
	DD	06H
	DD	FLAT:$L73524
	DD	07H
	DD	FLAT:$L73525
	DD	08H
	DD	FLAT:$L73526
xdata$x	ENDS
;	COMDAT ?GetFullName@CDriveInfo@@SA?AVCString@@H@Z
_TEXT	SEGMENT
_nDrive$ = 12
___$ReturnUdt$ = 8
_sFullName$ = -20
_sLetter$ = -16
_sVolume$ = -24
$T73512 = -28
$T73513 = -32
$T73514 = -36
$T73515 = -40
$T73516 = -44
$T73517 = -48
$T73528 = -52
__$EHRec$ = -12
?GetFullName@CDriveInfo@@SA?AVCString@@H@Z PROC NEAR	; CDriveInfo::GetFullName, COMDAT

; 65   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73532
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 72					; 00000048H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-84]
	mov	ecx, 18					; 00000012H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T73528[ebp], 0

; 66   : 	CString sFullName, sLetter, sVolume;

	lea	ecx, DWORD PTR _sFullName$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sLetter$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sVolume$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3
$L73192:

; 67   : 
; 68   : 	ASSERT (nDrive > 0 && nDrive <= 26);

	cmp	DWORD PTR _nDrive$[ebp], 0
	jle	SHORT $L73196
	cmp	DWORD PTR _nDrive$[ebp], 26		; 0000001aH
	jle	SHORT $L73195
$L73196:
	push	68					; 00000044H
	push	OFFSET FLAT:$SG73197
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73195
	int	3
$L73195:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73192

; 69   : 
; 70   : 	if (IsDriveAvailable(nDrive))

	mov	ecx, DWORD PTR _nDrive$[ebp]
	push	ecx
	call	?IsDriveAvailable@CDriveInfo@@SAHH@Z	; CDriveInfo::IsDriveAvailable
	add	esp, 4
	test	eax, eax
	je	$L73198

; 72   : 		sLetter = GetLetter(nDrive);

	mov	edx, DWORD PTR _nDrive$[ebp]
	push	edx
	call	?GetLetter@CDriveInfo@@SADH@Z		; CDriveInfo::GetLetter
	add	esp, 4
	push	eax
	lea	ecx, DWORD PTR _sLetter$[ebp]
	call	??4CString@@QAEABV0@D@Z			; CString::operator=

; 73   : 		sVolume = GetVolume(nDrive);

	mov	eax, DWORD PTR _nDrive$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T73512[ebp]
	push	ecx
	call	?GetVolume@CDriveInfo@@SA?AVCString@@H@Z ; CDriveInfo::GetVolume
	add	esp, 8
	mov	DWORD PTR -56+[ebp], eax
	mov	edx, DWORD PTR -56+[ebp]
	mov	DWORD PTR -60+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	eax, DWORD PTR -60+[ebp]
	push	eax
	lea	ecx, DWORD PTR _sVolume$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR $T73512[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 74   : 
; 75   : 		sFullName = sVolume + " (" + sLetter + ":)";

	push	OFFSET FLAT:$SG73205
	lea	ecx, DWORD PTR $T73513[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	push	OFFSET FLAT:$SG73206
	lea	ecx, DWORD PTR $T73514[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR $T73513[ebp]
	push	ecx
	lea	edx, DWORD PTR _sLetter$[ebp]
	push	edx
	lea	eax, DWORD PTR $T73514[ebp]
	push	eax
	lea	ecx, DWORD PTR _sVolume$[ebp]
	push	ecx
	lea	edx, DWORD PTR $T73515[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -64+[ebp], eax
	mov	eax, DWORD PTR -64+[ebp]
	mov	DWORD PTR -68+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	mov	ecx, DWORD PTR -68+[ebp]
	push	ecx
	lea	edx, DWORD PTR $T73516[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -72+[ebp], eax
	mov	eax, DWORD PTR -72+[ebp]
	mov	DWORD PTR -76+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 8
	mov	ecx, DWORD PTR -76+[ebp]
	push	ecx
	lea	edx, DWORD PTR $T73517[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -80+[ebp], eax
	mov	eax, DWORD PTR -80+[ebp]
	mov	DWORD PTR -84+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 9
	mov	ecx, DWORD PTR -84+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sFullName$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 8
	lea	ecx, DWORD PTR $T73517[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	lea	ecx, DWORD PTR $T73516[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR $T73515[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	lea	ecx, DWORD PTR $T73514[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR $T73513[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 76   : 
; 77   : 		return sFullName;

	lea	edx, DWORD PTR _sFullName$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T73528[ebp]
	or	al, 1
	mov	DWORD PTR $T73528[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sVolume$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sLetter$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sFullName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L73188
$L73198:

; 79   : 
; 80   : 	return "";

	push	OFFSET FLAT:$SG73207
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	ecx, DWORD PTR $T73528[ebp]
	or	ecx, 1
	mov	DWORD PTR $T73528[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sVolume$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sLetter$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sFullName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L73188:

; 81   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 84					; 00000054H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73518:
	lea	ecx, DWORD PTR _sFullName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73519:
	lea	ecx, DWORD PTR _sLetter$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73520:
	lea	ecx, DWORD PTR _sVolume$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73521:
	lea	ecx, DWORD PTR $T73512[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73522:
	lea	ecx, DWORD PTR $T73513[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73523:
	lea	ecx, DWORD PTR $T73514[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73524:
	lea	ecx, DWORD PTR $T73515[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73525:
	lea	ecx, DWORD PTR $T73516[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73526:
	lea	ecx, DWORD PTR $T73517[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73527:
	mov	eax, DWORD PTR $T73528[ebp]
	and	eax, 1
	test	eax, eax
	je	$L73529
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L73529:
	ret	0
$L73532:
	mov	eax, OFFSET FLAT:$T73531
	jmp	___CxxFrameHandler
text$x	ENDS
?GetFullName@CDriveInfo@@SA?AVCString@@H@Z ENDP		; CDriveInfo::GetFullName
PUBLIC	?GetTotalBytes@CDriveInfo@@SAMH@Z		; CDriveInfo::GetTotalBytes
;	COMDAT data
; File D:\_CODE\Shared\driveinfo.cpp
data	SEGMENT
$SG73221 DB	'D:\_CODE\Shared\driveinfo.cpp', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73545	DD	019930520H
	DD	02H
	DD	FLAT:$T73547
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73547	DD	0ffffffffH
	DD	FLAT:$L73542
	DD	00H
	DD	FLAT:$L73543
xdata$x	ENDS
;	COMDAT ?GetTotalBytes@CDriveInfo@@SAMH@Z
_TEXT	SEGMENT
_nDrive$ = 8
_sRoot$ = -16
_totalClusters$ = -28
_freeClusters$ = -20
_sectors$ = -24
_bytes$ = -32
_fSpace$73224 = -36
$T73539 = -40
$T73540 = -44
$T73541 = -48
__$EHRec$ = -12
?GetTotalBytes@CDriveInfo@@SAMH@Z PROC NEAR		; CDriveInfo::GetTotalBytes, COMDAT

; 84   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73546
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 68					; 00000044H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-80]
	mov	ecx, 17					; 00000011H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 85   : 	CString sRoot;

	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
$L73216:

; 86   : 	unsigned long totalClusters, freeClusters, sectors, bytes;
; 87   : 
; 88   : 	ASSERT (nDrive > 0 && nDrive <= 26);

	cmp	DWORD PTR _nDrive$[ebp], 0
	jle	SHORT $L73220
	cmp	DWORD PTR _nDrive$[ebp], 26		; 0000001aH
	jle	SHORT $L73219
$L73220:
	push	88					; 00000058H
	push	OFFSET FLAT:$SG73221
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73219
	int	3
$L73219:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73216

; 89   : 
; 90   : 	if (IsDriveAvailable(nDrive))

	mov	ecx, DWORD PTR _nDrive$[ebp]
	push	ecx
	call	?IsDriveAvailable@CDriveInfo@@SAHH@Z	; CDriveInfo::IsDriveAvailable
	add	esp, 4
	test	eax, eax
	je	$L73222

; 92   : 		sRoot = GetRoot(nDrive);

	mov	edx, DWORD PTR _nDrive$[ebp]
	push	edx
	lea	eax, DWORD PTR $T73539[ebp]
	push	eax
	call	?GetRoot@CDriveInfo@@SA?AVCString@@H@Z	; CDriveInfo::GetRoot
	add	esp, 8
	mov	DWORD PTR -52+[ebp], eax
	mov	ecx, DWORD PTR -52+[ebp]
	mov	DWORD PTR -56+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	edx, DWORD PTR -56+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T73539[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 93   : 	
; 94   : 		::GetDiskFreeSpace(sRoot, &sectors, &bytes, &freeClusters, &totalClusters);

	mov	esi, esp
	lea	eax, DWORD PTR _totalClusters$[ebp]
	push	eax
	lea	ecx, DWORD PTR _freeClusters$[ebp]
	push	ecx
	lea	edx, DWORD PTR _bytes$[ebp]
	push	edx
	lea	eax, DWORD PTR _sectors$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	DWORD PTR __imp__GetDiskFreeSpaceW@20
	cmp	esi, esp
	call	__chkesp

; 95   : 	
; 96   : 		// do maths like this to avoid truncation
; 97   : 		// errors
; 98   : 		float fSpace = (float)sectors;

	mov	ecx, DWORD PTR _sectors$[ebp]
	mov	DWORD PTR -64+[ebp], ecx
	mov	DWORD PTR -64+[ebp+4], 0
	fild	QWORD PTR -64+[ebp]
	fstp	DWORD PTR _fSpace$73224[ebp]

; 99   : 		fSpace *= bytes;

	mov	edx, DWORD PTR _bytes$[ebp]
	mov	DWORD PTR -72+[ebp], edx
	mov	DWORD PTR -72+[ebp+4], 0
	fild	QWORD PTR -72+[ebp]
	fmul	DWORD PTR _fSpace$73224[ebp]
	fstp	DWORD PTR _fSpace$73224[ebp]

; 100  : 		fSpace *= totalClusters;

	mov	eax, DWORD PTR _totalClusters$[ebp]
	mov	DWORD PTR -80+[ebp], eax
	mov	DWORD PTR -80+[ebp+4], 0
	fild	QWORD PTR -80+[ebp]
	fmul	DWORD PTR _fSpace$73224[ebp]
	fst	DWORD PTR _fSpace$73224[ebp]

; 101  : 		fSpace /= 1048576;

	fdiv	DWORD PTR __real@4@40138000000000000000
	fstp	DWORD PTR _fSpace$73224[ebp]

; 102  : 
; 103  : 		return fSpace;

	mov	ecx, DWORD PTR _fSpace$73224[ebp]
	mov	DWORD PTR $T73540[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	fld	DWORD PTR $T73540[ebp]
	jmp	SHORT $L73210
$L73222:

; 105  : 	
; 106  : 	return 0.0f;

	mov	DWORD PTR $T73541[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	fld	DWORD PTR $T73541[ebp]
$L73210:

; 107  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 80					; 00000050H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73542:
	lea	ecx, DWORD PTR _sRoot$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73543:
	lea	ecx, DWORD PTR $T73539[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73546:
	mov	eax, OFFSET FLAT:$T73545
	jmp	___CxxFrameHandler
text$x	ENDS
?GetTotalBytes@CDriveInfo@@SAMH@Z ENDP			; CDriveInfo::GetTotalBytes
;	COMDAT data
; File D:\_CODE\Shared\driveinfo.cpp
data	SEGMENT
$SG73236 DB	'D:\_CODE\Shared\driveinfo.cpp', 00H
data	ENDS
;	COMDAT ?GetLetter@CDriveInfo@@SADH@Z
_TEXT	SEGMENT
_nDrive$ = 8
?GetLetter@CDriveInfo@@SADH@Z PROC NEAR			; CDriveInfo::GetLetter, COMDAT

; 110  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L73231:

; 111  : 	ASSERT (nDrive > 0 && nDrive <= 26);

	cmp	DWORD PTR _nDrive$[ebp], 0
	jle	SHORT $L73235
	cmp	DWORD PTR _nDrive$[ebp], 26		; 0000001aH
	jle	SHORT $L73234
$L73235:
	push	111					; 0000006fH
	push	OFFSET FLAT:$SG73236
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73234
	int	3
$L73234:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73231

; 112  : 
; 113  : 	return (char)(nDrive + 'A' - 1);

	mov	eax, DWORD PTR _nDrive$[ebp]
	add	eax, 64					; 00000040H

; 114  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?GetLetter@CDriveInfo@@SADH@Z ENDP			; CDriveInfo::GetLetter
_TEXT	ENDS
PUBLIC	?GetType@CDriveInfo@@SAHH@Z			; CDriveInfo::GetType
EXTRN	__imp__GetDriveTypeW@4:NEAR
;	COMDAT data
; File D:\_CODE\Shared\driveinfo.cpp
data	SEGMENT
$SG73247 DB	'D:\_CODE\Shared\driveinfo.cpp', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73562	DD	019930520H
	DD	02H
	DD	FLAT:$T73564
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73564	DD	0ffffffffH
	DD	FLAT:$L73559
	DD	00H
	DD	FLAT:$L73560
xdata$x	ENDS
;	COMDAT ?GetType@CDriveInfo@@SAHH@Z
_TEXT	SEGMENT
_nDrive$ = 8
_sVolume$ = -16
$T73556 = -20
$T73557 = -24
$T73558 = -28
__$EHRec$ = -12
?GetType@CDriveInfo@@SAHH@Z PROC NEAR			; CDriveInfo::GetType, COMDAT

; 117  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73563
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 118  : 	CString sVolume;

	lea	ecx, DWORD PTR _sVolume$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
$L73242:

; 119  : 
; 120  : 	ASSERT (nDrive > 0 && nDrive <= 26);

	cmp	DWORD PTR _nDrive$[ebp], 0
	jle	SHORT $L73246
	cmp	DWORD PTR _nDrive$[ebp], 26		; 0000001aH
	jle	SHORT $L73245
$L73246:
	push	120					; 00000078H
	push	OFFSET FLAT:$SG73247
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73245
	int	3
$L73245:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73242

; 121  : 
; 122  : 	// shortcut to avoid floppy access
; 123  : 	if (nDrive ==1 || nDrive == 2)

	cmp	DWORD PTR _nDrive$[ebp], 1
	je	SHORT $L73249
	cmp	DWORD PTR _nDrive$[ebp], 2
	jne	SHORT $L73248
$L73249:

; 124  : 		return DRIVE_REMOVABLE;

	mov	DWORD PTR $T73556[ebp], 2
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sVolume$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T73556[ebp]
	jmp	SHORT $L73240
$L73248:

; 125  : 
; 126  : 	return ::GetDriveType(GetRoot(nDrive));

	mov	ecx, DWORD PTR _nDrive$[ebp]
	push	ecx
	lea	edx, DWORD PTR $T73558[ebp]
	push	edx
	call	?GetRoot@CDriveInfo@@SA?AVCString@@H@Z	; CDriveInfo::GetRoot
	add	esp, 8
	mov	DWORD PTR -32+[ebp], eax
	mov	eax, DWORD PTR -32+[ebp]
	mov	DWORD PTR -36+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -36+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__GetDriveTypeW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR $T73557[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T73558[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sVolume$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T73557[ebp]
$L73240:

; 127  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73559:
	lea	ecx, DWORD PTR _sVolume$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73560:
	lea	ecx, DWORD PTR $T73558[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73563:
	mov	eax, OFFSET FLAT:$T73562
	jmp	___CxxFrameHandler
text$x	ENDS
?GetType@CDriveInfo@@SAHH@Z ENDP			; CDriveInfo::GetType
PUBLIC	?GetPathType@CDriveInfo@@SAHPBG@Z		; CDriveInfo::GetPathType
PUBLIC	?GetDrive@CDriveInfo@@SAHPBG@Z			; CDriveInfo::GetDrive
PUBLIC	?IsRemotePath@CDriveInfo@@SAHPBGH@Z		; CDriveInfo::IsRemotePath
;	COMDAT ?GetPathType@CDriveInfo@@SAHPBG@Z
_TEXT	SEGMENT
_szPathName$ = 8
_nDrive$ = -4
?GetPathType@CDriveInfo@@SAHPBG@Z PROC NEAR		; CDriveInfo::GetPathType, COMDAT

; 130  : { 

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 131  :     int nDrive = GetDrive(szPathName);

	mov	eax, DWORD PTR _szPathName$[ebp]
	push	eax
	call	?GetDrive@CDriveInfo@@SAHPBG@Z		; CDriveInfo::GetDrive
	add	esp, 4
	mov	DWORD PTR _nDrive$[ebp], eax

; 132  : 
; 133  : 	if (nDrive >= 0)

	cmp	DWORD PTR _nDrive$[ebp], 0
	jl	SHORT $L73257

; 134  : 		return GetType(nDrive);

	mov	ecx, DWORD PTR _nDrive$[ebp]
	push	ecx
	call	?GetType@CDriveInfo@@SAHH@Z		; CDriveInfo::GetType
	add	esp, 4
	jmp	SHORT $L73260
$L73257:

; 135  : 
; 136  : 	else if (IsRemotePath(szPathName) > 0)

	push	1
	mov	edx, DWORD PTR _szPathName$[ebp]
	push	edx
	call	?IsRemotePath@CDriveInfo@@SAHPBGH@Z	; CDriveInfo::IsRemotePath
	add	esp, 8
	test	eax, eax
	jle	SHORT $L73259

; 137  : 		return DRIVE_REMOTE;

	mov	eax, 4
	jmp	SHORT $L73260
$L73259:

; 140  : 	    return DRIVE_UNKNOWN; 

	xor	eax, eax
$L73260:

; 141  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetPathType@CDriveInfo@@SAHPBG@Z ENDP			; CDriveInfo::GetPathType
_TEXT	ENDS
EXTRN	??0CString@@QAE@GH@Z:NEAR			; CString::CString
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	??ACString@@QBEGH@Z:NEAR			; CString::operator[]
EXTRN	??YCString@@QAEABV0@G@Z:NEAR			; CString::operator+=
EXTRN	?MakeUpper@CString@@QAEXXZ:NEAR			; CString::MakeUpper
EXTRN	?MakeLower@CString@@QAEXXZ:NEAR			; CString::MakeLower
;	COMDAT xdata$x
; File D:\_CODE\Shared\driveinfo.cpp
xdata$x	SEGMENT
$T73577	DD	019930520H
	DD	03H
	DD	FLAT:$T73579
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73579	DD	0ffffffffH
	DD	FLAT:$L73573
	DD	00H
	DD	FLAT:$L73574
	DD	01H
	DD	FLAT:$L73575
xdata$x	ENDS
;	COMDAT ?FormatName@CDriveInfo@@SAXAAVCString@@@Z
_TEXT	SEGMENT
_sFilename$ = 8
_sTemp$ = -20
_sChar$ = -24
_nLen$ = -32
_nChar$ = -28
_cChar$ = -36
_cLastChar$ = -16
$T73572 = -40
__$EHRec$ = -12
?FormatName@CDriveInfo@@SAXAAVCString@@@Z PROC NEAR	; CDriveInfo::FormatName, COMDAT

; 144  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73578
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 36					; 00000024H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-48], eax
	mov	DWORD PTR [ebp-44], eax
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 145  : 	CString sTemp, sChar;

	lea	ecx, DWORD PTR _sTemp$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sChar$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 146  : 	int nLen, nChar;
; 147  : 	TCHAR cChar, cLastChar = ' '; // space 

	mov	WORD PTR _cLastChar$[ebp], 32		; 00000020H

; 148  : 
; 149  : 	// this function accepts pathnames and names with spaces
; 150  : 	sFilename.MakeLower();

	mov	ecx, DWORD PTR _sFilename$[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower

; 151  : 	nLen = sFilename.GetLength();

	mov	ecx, DWORD PTR _sFilename$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR _nLen$[ebp], eax

; 152  : 
; 153  : 	// for each word make the first letter upper case
; 154  : 	for (nChar = 0; nChar < nLen; nChar++)

	mov	DWORD PTR _nChar$[ebp], 0
	jmp	SHORT $L73270
$L73271:
	mov	eax, DWORD PTR _nChar$[ebp]
	add	eax, 1
	mov	DWORD PTR _nChar$[ebp], eax
$L73270:
	mov	ecx, DWORD PTR _nChar$[ebp]
	cmp	ecx, DWORD PTR _nLen$[ebp]
	jge	$L73272

; 156  : 		cChar = sFilename[nChar];

	mov	edx, DWORD PTR _nChar$[ebp]
	push	edx
	mov	ecx, DWORD PTR _sFilename$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	mov	WORD PTR _cChar$[ebp], ax

; 157  : 
; 158  : 		if (cLastChar == ' ' || cLastChar == '\\')

	mov	eax, DWORD PTR _cLastChar$[ebp]
	and	eax, 65535				; 0000ffffH
	cmp	eax, 32					; 00000020H
	je	SHORT $L73274
	mov	ecx, DWORD PTR _cLastChar$[ebp]
	and	ecx, 65535				; 0000ffffH
	cmp	ecx, 92					; 0000005cH
	jne	SHORT $L73273
$L73274:

; 160  : 			sChar = CString(cChar);

	push	1
	mov	dx, WORD PTR _cChar$[ebp]
	push	edx
	lea	ecx, DWORD PTR $T73572[ebp]
	call	??0CString@@QAE@GH@Z			; CString::CString
	mov	DWORD PTR -44+[ebp], eax
	mov	eax, DWORD PTR -44+[ebp]
	mov	DWORD PTR -48+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR -48+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sChar$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T73572[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 161  : 			sChar.MakeUpper();

	lea	ecx, DWORD PTR _sChar$[ebp]
	call	?MakeUpper@CString@@QAEXXZ		; CString::MakeUpper

; 162  : 			cChar = sChar[0];

	push	0
	lea	ecx, DWORD PTR _sChar$[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	mov	WORD PTR _cChar$[ebp], ax
$L73273:

; 164  : 
; 165  : 		sTemp += cChar;

	mov	dx, WORD PTR _cChar$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sTemp$[ebp]
	call	??YCString@@QAEABV0@G@Z			; CString::operator+=

; 166  : 		cLastChar = cChar;

	mov	ax, WORD PTR _cChar$[ebp]
	mov	WORD PTR _cLastChar$[ebp], ax

; 167  : 	}

	jmp	$L73271
$L73272:

; 168  : 
; 169  : 	sFilename = sTemp;

	lea	ecx, DWORD PTR _sTemp$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sFilename$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=

; 170  : }

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sChar$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sTemp$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 48					; 00000030H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73573:
	lea	ecx, DWORD PTR _sTemp$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73574:
	lea	ecx, DWORD PTR _sChar$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73575:
	lea	ecx, DWORD PTR $T73572[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73578:
	mov	eax, OFFSET FLAT:$T73577
	jmp	___CxxFrameHandler
text$x	ENDS
?FormatName@CDriveInfo@@SAXAAVCString@@@Z ENDP		; CDriveInfo::FormatName
EXTRN	__imp___chdrive:NEAR
EXTRN	__imp___getdrive:NEAR
;	COMDAT ?IsDriveAvailable@CDriveInfo@@SAHH@Z
_TEXT	SEGMENT
_nDrive$ = 8
_nCurDrive$ = -8
_nRes$ = -4
?IsDriveAvailable@CDriveInfo@@SAHH@Z PROC NEAR		; CDriveInfo::IsDriveAvailable, COMDAT

; 173  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 174  : 	int nCurDrive;
; 175  : 	int nRes;
; 176  : 
; 177  : 	// save cur drive and try to change to drive
; 178  : 	nCurDrive = _getdrive();

	mov	esi, esp
	call	DWORD PTR __imp___getdrive
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nCurDrive$[ebp], eax

; 179  : 	nRes = _chdrive(nDrive);

	mov	esi, esp
	mov	eax, DWORD PTR _nDrive$[ebp]
	push	eax
	call	DWORD PTR __imp___chdrive
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nRes$[ebp], eax

; 180  : 
; 181  : 	// if change successful change back before return
; 182  : 	if (nRes == 0)

	cmp	DWORD PTR _nRes$[ebp], 0
	jne	SHORT $L73281

; 183  : 		_chdrive(nCurDrive);

	mov	esi, esp
	mov	ecx, DWORD PTR _nCurDrive$[ebp]
	push	ecx
	call	DWORD PTR __imp___chdrive
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
$L73281:

; 184  : 
; 185  : 	return (nRes == 0) ? TRUE : FALSE;

	xor	eax, eax
	cmp	DWORD PTR _nRes$[ebp], 0
	sete	al

; 186  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsDriveAvailable@CDriveInfo@@SAHH@Z ENDP		; CDriveInfo::IsDriveAvailable
_TEXT	ENDS
PUBLIC	?IsUNCPath@CDriveInfo@@SAHPBG@Z			; CDriveInfo::IsUNCPath
EXTRN	__imp__wcsstr:NEAR
;	COMDAT data
; File D:\_CODE\Shared\driveinfo.cpp
data	SEGMENT
$SG73285 DB	'\', 00H, '\', 00H, 00H, 00H
data	ENDS
;	COMDAT ?IsUNCPath@CDriveInfo@@SAHPBG@Z
_TEXT	SEGMENT
_szPathName$ = 8
?IsUNCPath@CDriveInfo@@SAHPBG@Z PROC NEAR		; CDriveInfo::IsUNCPath, COMDAT

; 189  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 190  : 	return (_tcsstr(szPathName, _T("\\\\")) == szPathName);

	mov	esi, esp
	push	OFFSET FLAT:$SG73285
	mov	eax, DWORD PTR _szPathName$[ebp]
	push	eax
	call	DWORD PTR __imp__wcsstr
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	xor	ecx, ecx
	cmp	eax, DWORD PTR _szPathName$[ebp]
	sete	cl
	mov	eax, ecx

; 191  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?IsUNCPath@CDriveInfo@@SAHPBG@Z ENDP			; CDriveInfo::IsUNCPath
_TEXT	ENDS
EXTRN	__imp__toupper:NEAR
;	COMDAT data
; File D:\_CODE\Shared\driveinfo.cpp
data	SEGMENT
$SG73291 DB	':', 00H, 00H, 00H
data	ENDS
;	COMDAT ?GetDrive@CDriveInfo@@SAHPBG@Z
_TEXT	SEGMENT
_szPathName$ = 8
_nDrive$ = -4
_cDrive$73292 = -8
?GetDrive@CDriveInfo@@SAHPBG@Z PROC NEAR		; CDriveInfo::GetDrive, COMDAT

; 194  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 195  : 	int nDrive = 0;

	mov	DWORD PTR _nDrive$[ebp], 0

; 196  : 	
; 197  : 	if (_tcsstr(szPathName, _T(":")) == szPathName + 1)

	mov	esi, esp
	push	OFFSET FLAT:$SG73291
	mov	eax, DWORD PTR _szPathName$[ebp]
	push	eax
	call	DWORD PTR __imp__wcsstr
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	mov	ecx, DWORD PTR _szPathName$[ebp]
	add	ecx, 2
	cmp	eax, ecx
	jne	SHORT $L73290

; 199  : 		TCHAR cDrive = szPathName[0];

	mov	edx, DWORD PTR _szPathName$[ebp]
	mov	ax, WORD PTR [edx]
	mov	WORD PTR _cDrive$73292[ebp], ax

; 200  : 		cDrive = (TCHAR)toupper(cDrive);

	mov	ecx, DWORD PTR _cDrive$73292[ebp]
	and	ecx, 65535				; 0000ffffH
	mov	esi, esp
	push	ecx
	call	DWORD PTR __imp__toupper
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	mov	WORD PTR _cDrive$73292[ebp], ax

; 201  : 		nDrive = cDrive - 64;

	mov	edx, DWORD PTR _cDrive$73292[ebp]
	and	edx, 65535				; 0000ffffH
	sub	edx, 64					; 00000040H
	mov	DWORD PTR _nDrive$[ebp], edx
$L73290:

; 204  : 	
; 205  : 	return nDrive ? nDrive : -1;

	cmp	DWORD PTR _nDrive$[ebp], 0
	je	SHORT $L73589
	mov	eax, DWORD PTR _nDrive$[ebp]
	mov	DWORD PTR -12+[ebp], eax
	jmp	SHORT $L73590
$L73589:
	mov	DWORD PTR -12+[ebp], -1
$L73590:
	mov	eax, DWORD PTR -12+[ebp]

; 206  : }

	pop	esi
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetDrive@CDriveInfo@@SAHPBG@Z ENDP			; CDriveInfo::GetDrive
_TEXT	ENDS
PUBLIC	?IsMappedPath@CDriveInfo@@SAHPBG@Z		; CDriveInfo::IsMappedPath
;	COMDAT ?IsMappedPath@CDriveInfo@@SAHPBG@Z
_TEXT	SEGMENT
_szPathName$ = 8
_nDrive$ = -4
?IsMappedPath@CDriveInfo@@SAHPBG@Z PROC NEAR		; CDriveInfo::IsMappedPath, COMDAT

; 209  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 210  : 	int nDrive;
; 211  : 
; 212  : 	nDrive = GetDrive(szPathName);

	mov	eax, DWORD PTR _szPathName$[ebp]
	push	eax
	call	?GetDrive@CDriveInfo@@SAHPBG@Z		; CDriveInfo::GetDrive
	add	esp, 4
	mov	DWORD PTR _nDrive$[ebp], eax

; 213  : 
; 214  : 	if (nDrive <= 0)

	cmp	DWORD PTR _nDrive$[ebp], 0
	jg	SHORT $L73298

; 215  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L73296
$L73298:

; 216  : 
; 217  : 	return (GetType(nDrive) == DRIVE_REMOTE);

	mov	ecx, DWORD PTR _nDrive$[ebp]
	push	ecx
	call	?GetType@CDriveInfo@@SAHH@Z		; CDriveInfo::GetType
	add	esp, 4
	xor	edx, edx
	cmp	eax, 4
	sete	dl
	mov	eax, edx
$L73296:

; 218  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsMappedPath@CDriveInfo@@SAHPBG@Z ENDP			; CDriveInfo::IsMappedPath
_TEXT	ENDS
EXTRN	__imp__GetFileAttributesW@4:NEAR
;	COMDAT ?IsRemotePath@CDriveInfo@@SAHPBGH@Z
_TEXT	SEGMENT
_szPathName$ = 8
_bAllowFileCheck$ = 12
_dwAttr$73304 = -4
?IsRemotePath@CDriveInfo@@SAHPBGH@Z PROC NEAR		; CDriveInfo::IsRemotePath, COMDAT

; 221  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 222  : 	if (bAllowFileCheck)

	cmp	DWORD PTR _bAllowFileCheck$[ebp], 0
	je	SHORT $L73305

; 224  : 		DWORD dwAttr = ::GetFileAttributes(szPathName);

	mov	esi, esp
	mov	eax, DWORD PTR _szPathName$[ebp]
	push	eax
	call	DWORD PTR __imp__GetFileAttributesW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _dwAttr$73304[ebp], eax

; 225  : 		
; 226  : 		if (dwAttr == 0xffffffff)

	cmp	DWORD PTR _dwAttr$73304[ebp], -1
	jne	SHORT $L73305

; 227  : 			return -1;

	or	eax, -1
	jmp	SHORT $L73302
$L73305:

; 229  : 	
; 230  : 	return (IsUNCPath(szPathName) || IsMappedPath(szPathName));

	mov	ecx, DWORD PTR _szPathName$[ebp]
	push	ecx
	call	?IsUNCPath@CDriveInfo@@SAHPBG@Z		; CDriveInfo::IsUNCPath
	add	esp, 4
	test	eax, eax
	jne	SHORT $L73596
	mov	edx, DWORD PTR _szPathName$[ebp]
	push	edx
	call	?IsMappedPath@CDriveInfo@@SAHPBG@Z	; CDriveInfo::IsMappedPath
	add	esp, 4
	test	eax, eax
	jne	SHORT $L73596
	mov	DWORD PTR -8+[ebp], 0
	jmp	SHORT $L73597
$L73596:
	mov	DWORD PTR -8+[ebp], 1
$L73597:
	mov	eax, DWORD PTR -8+[ebp]
$L73302:

; 231  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsRemotePath@CDriveInfo@@SAHPBGH@Z ENDP		; CDriveInfo::IsRemotePath
_TEXT	ENDS
PUBLIC	?IsFixedPath@CDriveInfo@@SAHPBG@Z		; CDriveInfo::IsFixedPath
;	COMDAT ?IsFixedPath@CDriveInfo@@SAHPBG@Z
_TEXT	SEGMENT
_szPathName$ = 8
_nDrive$ = -4
?IsFixedPath@CDriveInfo@@SAHPBG@Z PROC NEAR		; CDriveInfo::IsFixedPath, COMDAT

; 234  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 235  : 	int nDrive = GetDrive(szPathName);

	mov	eax, DWORD PTR _szPathName$[ebp]
	push	eax
	call	?GetDrive@CDriveInfo@@SAHPBG@Z		; CDriveInfo::GetDrive
	add	esp, 4
	mov	DWORD PTR _nDrive$[ebp], eax

; 236  : 
; 237  : 	if (nDrive == -1) // unknown

	cmp	DWORD PTR _nDrive$[ebp], -1
	jne	SHORT $L73310

; 238  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L73308
$L73310:

; 239  : 
; 240  : 	return (GetType(nDrive) == DRIVE_FIXED);

	mov	ecx, DWORD PTR _nDrive$[ebp]
	push	ecx
	call	?GetType@CDriveInfo@@SAHH@Z		; CDriveInfo::GetType
	add	esp, 4
	xor	edx, edx
	cmp	eax, 3
	sete	dl
	mov	eax, edx
$L73308:

; 241  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsFixedPath@CDriveInfo@@SAHPBG@Z ENDP			; CDriveInfo::IsFixedPath
_TEXT	ENDS
PUBLIC	?IsRemovablePath@CDriveInfo@@SAHPBG@Z		; CDriveInfo::IsRemovablePath
;	COMDAT ?IsRemovablePath@CDriveInfo@@SAHPBG@Z
_TEXT	SEGMENT
_szPathName$ = 8
_nDrive$ = -4
?IsRemovablePath@CDriveInfo@@SAHPBG@Z PROC NEAR		; CDriveInfo::IsRemovablePath, COMDAT

; 244  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 245  : 	int nDrive = GetDrive(szPathName);

	mov	eax, DWORD PTR _szPathName$[ebp]
	push	eax
	call	?GetDrive@CDriveInfo@@SAHPBG@Z		; CDriveInfo::GetDrive
	add	esp, 4
	mov	DWORD PTR _nDrive$[ebp], eax

; 246  : 
; 247  : 	if (nDrive == -1) // unknown

	cmp	DWORD PTR _nDrive$[ebp], -1
	jne	SHORT $L73315

; 248  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L73313
$L73315:

; 249  : 
; 250  : 	return (GetType(nDrive) == DRIVE_REMOVABLE);

	mov	ecx, DWORD PTR _nDrive$[ebp]
	push	ecx
	call	?GetType@CDriveInfo@@SAHH@Z		; CDriveInfo::GetType
	add	esp, 4
	xor	edx, edx
	cmp	eax, 2
	sete	dl
	mov	eax, edx
$L73313:

; 251  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsRemovablePath@CDriveInfo@@SAHPBG@Z ENDP		; CDriveInfo::IsRemovablePath
_TEXT	ENDS
PUBLIC	?IsReadonlyPath@CDriveInfo@@SAHPBG@Z		; CDriveInfo::IsReadonlyPath
;	COMDAT ?IsReadonlyPath@CDriveInfo@@SAHPBG@Z
_TEXT	SEGMENT
_szPathName$ = 8
_dwAttr$ = -4
?IsReadonlyPath@CDriveInfo@@SAHPBG@Z PROC NEAR		; CDriveInfo::IsReadonlyPath, COMDAT

; 254  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 255  : 	DWORD dwAttr = ::GetFileAttributes(szPathName);

	mov	esi, esp
	mov	eax, DWORD PTR _szPathName$[ebp]
	push	eax
	call	DWORD PTR __imp__GetFileAttributesW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _dwAttr$[ebp], eax

; 256  : 
; 257  : 	if (dwAttr == 0xffffffff)

	cmp	DWORD PTR _dwAttr$[ebp], -1
	jne	SHORT $L73320

; 258  : 		return -1;

	or	eax, -1
	jmp	SHORT $L73318
$L73320:

; 259  : 
; 260  : 	// else
; 261  : 	return (dwAttr & FILE_ATTRIBUTE_READONLY);

	mov	eax, DWORD PTR _dwAttr$[ebp]
	and	eax, 1
$L73318:

; 262  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsReadonlyPath@CDriveInfo@@SAHPBG@Z ENDP		; CDriveInfo::IsReadonlyPath
_TEXT	ENDS
PUBLIC	?GetSerialNumber@CDriveInfo@@SAKH@Z		; CDriveInfo::GetSerialNumber
;	COMDAT xdata$x
; File D:\_CODE\Shared\driveinfo.cpp
xdata$x	SEGMENT
$T73610	DD	019930520H
	DD	01H
	DD	FLAT:$T73612
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73612	DD	0ffffffffH
	DD	FLAT:$L73608
xdata$x	ENDS
;	COMDAT ?GetSerialNumber@CDriveInfo@@SAKH@Z
_TEXT	SEGMENT
_nDrive$ = 8
_dwHDSerialNum$ = -16
$T73606 = -20
$T73607 = -24
__$EHRec$ = -12
?GetSerialNumber@CDriveInfo@@SAKH@Z PROC NEAR		; CDriveInfo::GetSerialNumber, COMDAT

; 265  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73611
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

; 266  : 	if (GetType(nDrive) != DRIVE_FIXED)

	mov	eax, DWORD PTR _nDrive$[ebp]
	push	eax
	call	?GetType@CDriveInfo@@SAHH@Z		; CDriveInfo::GetType
	add	esp, 4
	cmp	eax, 3
	je	SHORT $L73324

; 267  : 		return 0;

	xor	eax, eax
	jmp	SHORT $L73323
$L73324:

; 268  : 
; 269  : 	DWORD dwHDSerialNum = 0;

	mov	DWORD PTR _dwHDSerialNum$[ebp], 0

; 270  : 
; 271  : 	if (!GetVolumeInformation(GetRoot(nDrive), NULL, 0, &dwHDSerialNum, NULL, NULL, NULL, 0))

	mov	esi, esp
	push	0
	push	0
	push	0
	push	0
	lea	ecx, DWORD PTR _dwHDSerialNum$[ebp]
	push	ecx
	push	0
	push	0
	mov	edx, DWORD PTR _nDrive$[ebp]
	push	edx
	lea	eax, DWORD PTR $T73607[ebp]
	push	eax
	call	?GetRoot@CDriveInfo@@SA?AVCString@@H@Z	; CDriveInfo::GetRoot
	add	esp, 8
	mov	DWORD PTR -28+[ebp], eax
	mov	ecx, DWORD PTR -28+[ebp]
	mov	DWORD PTR -32+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -32+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	DWORD PTR __imp__GetVolumeInformationW@32
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	BYTE PTR $T73606[ebp], al
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T73607[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	edx, DWORD PTR $T73606[ebp]
	and	edx, 255				; 000000ffH
	test	edx, edx
	je	SHORT $L73327

; 272  : 		return (DWORD)-1;

	or	eax, -1
	jmp	SHORT $L73323
$L73327:

; 273  : 
; 274  : 	return dwHDSerialNum;

	mov	eax, DWORD PTR _dwHDSerialNum$[ebp]
$L73323:

; 275  : }

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
$L73608:
	lea	ecx, DWORD PTR $T73607[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73611:
	mov	eax, OFFSET FLAT:$T73610
	jmp	___CxxFrameHandler
text$x	ENDS
?GetSerialNumber@CDriveInfo@@SAKH@Z ENDP		; CDriveInfo::GetSerialNumber
END

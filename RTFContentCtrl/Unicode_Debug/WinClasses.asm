	TITLE	D:\_CODE\Shared\WinClasses.cpp
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
_BSS	SEGMENT PARA USE32 PUBLIC 'BSS'
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
;	COMDAT ??_C@_0BP@FCNA@D?3?2_CODE?2Shared?2folderdialog?4h?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0EJ@KGBI@C?3?2Program?5Files?5?$CIx86?$CJ?2Microsoft@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_08PEKM@lpa?5?$CB?$DN?50?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0EK@BMNG@C?3?2Program?5Files?5?$CIx86?$CJ?2Microsoft@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_08NDGO@lpw?5?$CB?$DN?50?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0BA@DNCN@lpDevModeW?5?$CB?$DN?50?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0L@BLIP@lptmW?5?$CB?$DN?50?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0L@DMEN@lptmA?5?$CB?$DN?50?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0BA@BKOP@lpDevModeA?5?$CB?$DN?50?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0EK@MDCF@C?3?2Program?5Files?5?$CIx86?$CJ?2Microsoft@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
CRT$XCA	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCA	ENDS
CRT$XCU	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCU	ENDS
CRT$XCL	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCL	ENDS
CRT$XCC	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCC	ENDS
CRT$XCZ	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCZ	ENDS
;	COMDAT ?IsClass@CWinClasses@@SAHPBG0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AtlA2WHelper@@YGPAGPAGPBDHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AtlW2AHelper@@YGPADPADPBGHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?A2WBSTR@@YAPAGPBDH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?TRACEWND@@YAXPBGPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E283
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E284
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E286
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E287
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E289
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsClass@CWinClasses@@SAHPAUHWND__@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsClassEx@CWinClasses@@SAHPAUHWND__@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsClassEx@CWinClasses@@SAHPBG0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetClassEx@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsControlClass@CWinClasses@@SAHPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsControlClass@CWinClasses@@SAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsEditControl@CWinClasses@@SAHPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsEditControl@CWinClasses@@SAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsRichEditControl@CWinClasses@@SAHPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsRichEditControl@CWinClasses@@SAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsComboBox@CWinClasses@@SAHPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsComboBox@CWinClasses@@SAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsDialog@CWinClasses@@SAHPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsChild@CWinClasses@@SAHPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsCommonDialog@CWinClasses@@SAHPAUHWND__@@W4WCLS_COMMONDIALOG@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetButtonType@CWinClasses@@SAHPAUHWND__@@@Z
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
FLAT	GROUP _DATA, CONST, _BSS, CRT$XCA, CRT$XCU, CRT$XCL, CRT$XCC, CRT$XCZ
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A ; CWinClasses::s_mapCtrlClasses
_BSS	SEGMENT
?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A DB 01cH DUP (?) ; CWinClasses::s_mapCtrlClasses
_BSS	ENDS
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\Shared\WinClasses.cpp', 00H
_DATA	ENDS
CRT$XCU	SEGMENT
_$S288	DD	FLAT:_$E287
CRT$XCU	ENDS
PUBLIC	?TRACEWND@@YAXPBGPAUHWND__@@@Z			; TRACEWND
PUBLIC	?GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z ; CWinClasses::GetClass
EXTRN	?FromHandle@CWnd@@SGPAV1@PAUHWND__@@@Z:NEAR	; CWnd::FromHandle
EXTRN	?GetWindowTextW@CWnd@@QBEXAAVCString@@@Z:NEAR	; CWnd::GetWindowTextW
EXTRN	__chkesp:NEAR
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	?AfxTrace@@YAXPBGZZ:NEAR			; AfxTrace
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	__imp__GetDlgCtrlID@4:NEAR
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT data
; File D:\_CODE\Shared\WinClasses.cpp
data	SEGMENT
$SG74451 DB	'%', 00H, 's', 00H, '(', 00H, '%', 00H, 's', 00H, ',', 00H
	DB	' ', 00H, '%', 00H, 's', 00H, ',', 00H, ' ', 00H, 'i', 00H, 'd'
	DB	00H, ' ', 00H, '=', 00H, ' ', 00H, '%', 00H, 'd', 00H, ')', 00H
	DB	0aH, 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74928	DD	019930520H
	DD	02H
	DD	FLAT:$T74932
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74932	DD	0ffffffffH
	DD	FLAT:$L74923
	DD	00H
	DD	FLAT:$L74924
xdata$x	ENDS
;	COMDAT ?TRACEWND@@YAXPBGPAUHWND__@@@Z
_TEXT	SEGMENT
$T74922 = -20
__$EHRec$ = -12
_szFunctionName$ = 8
_hWnd$ = 12
_sText$74449 = -16
?TRACEWND@@YAXPBGPAUHWND__@@@Z PROC NEAR		; TRACEWND, COMDAT

; 21   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74930
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 22   : 	if (hWnd)

	cmp	DWORD PTR _hWnd$[ebp], 0
	je	$L74448

; 24   : 		CString sText; 

	lea	ecx, DWORD PTR _sText$74449[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 25   : 		CWnd::FromHandle(hWnd)->GetWindowText(sText); 

	lea	eax, DWORD PTR _sText$74449[ebp]
	push	eax
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	call	?FromHandle@CWnd@@SGPAV1@PAUHWND__@@@Z	; CWnd::FromHandle
	mov	ecx, eax
	call	?GetWindowTextW@CWnd@@QBEXAAVCString@@@Z ; CWnd::GetWindowTextW

; 26   : 		TRACE (_T("%s(%s, %s, id = %d)\n"), szFunctionName, CWinClasses::GetClass(hWnd), sText, GetDlgCtrlID(hWnd)); 

	mov	esi, esp
	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	call	DWORD PTR __imp__GetDlgCtrlID@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	mov	eax, DWORD PTR _sText$74449[ebp]
	push	eax
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	lea	edx, DWORD PTR $T74922[ebp]
	push	edx
	call	?GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z ; CWinClasses::GetClass
	add	esp, 8
	mov	DWORD PTR -24+[ebp], eax
	mov	eax, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -28+[ebp]
	mov	edx, DWORD PTR [ecx]
	push	edx
	mov	eax, DWORD PTR _szFunctionName$[ebp]
	push	eax
	push	OFFSET FLAT:$SG74451
	call	?AfxTrace@@YAXPBGZZ			; AfxTrace
	add	esp, 20					; 00000014H
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T74922[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 27   : 	}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sText$74449[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74448:

; 28   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74923:
	lea	ecx, DWORD PTR _sText$74449[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74924:
	lea	ecx, DWORD PTR $T74922[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74930:
	mov	eax, OFFSET FLAT:$T74928
	jmp	___CxxFrameHandler
text$x	ENDS
?TRACEWND@@YAXPBGPAUHWND__@@@Z ENDP			; TRACEWND
;	COMDAT _$E287
_TEXT	SEGMENT
_$E287	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E283
	call	_$E286
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E287	ENDP
_TEXT	ENDS
EXTRN	??0CMapStringToPtr@@QAE@H@Z:NEAR		; CMapStringToPtr::CMapStringToPtr
;	COMDAT _$E283
_TEXT	SEGMENT
_$E283	PROC NEAR					; COMDAT

; 39   : CMapStringToPtr CWinClasses::s_mapCtrlClasses;

	push	ebp
	mov	ebp, esp
	push	10					; 0000000aH
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??0CMapStringToPtr@@QAE@H@Z		; CMapStringToPtr::CMapStringToPtr
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E283	ENDP
_TEXT	ENDS
EXTRN	_atexit:NEAR
;	COMDAT _$E286
_TEXT	SEGMENT
_$E286	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	push	OFFSET FLAT:_$E284
	call	_atexit
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E286	ENDP
_TEXT	ENDS
EXTRN	??1CMapStringToPtr@@UAE@XZ:NEAR			; CMapStringToPtr::~CMapStringToPtr
_BSS	SEGMENT
_$S285	DB	01H DUP (?)
_BSS	ENDS
;	COMDAT _$E284
_TEXT	SEGMENT
_$E284	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	xor	eax, eax
	mov	al, BYTE PTR _$S285
	and	eax, 1
	test	eax, eax
	jne	SHORT $L74457
	mov	cl, BYTE PTR _$S285
	or	cl, 1
	mov	BYTE PTR _$S285, cl
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??1CMapStringToPtr@@UAE@XZ		; CMapStringToPtr::~CMapStringToPtr
$L74457:
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E284	ENDP
_TEXT	ENDS
EXTRN	__imp__GetClassNameW@12:NEAR
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
EXTRN	?Empty@CString@@QAEXXZ:NEAR			; CString::Empty
EXTRN	?MakeLower@CString@@QAEXXZ:NEAR			; CString::MakeLower
EXTRN	?GetBuffer@CString@@QAEPAGH@Z:NEAR		; CString::GetBuffer
EXTRN	?ReleaseBuffer@CString@@QAEXH@Z:NEAR		; CString::ReleaseBuffer
_BSS	SEGMENT
	ALIGN	4

_?sWndClass@?1??GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z@4V3@A DD 01H DUP (?)
_BSS	ENDS
;	COMDAT ?GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z
_TEXT	SEGMENT
$T74948 = -8
_hWnd$ = 12
___$ReturnUdt$ = 8
_nLen$74477 = -4
?GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z PROC NEAR ; CWinClasses::GetClass, COMDAT

; 42   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T74948[ebp], 0

; 43   : 	static CString sWndClass;

	xor	eax, eax
	mov	al, BYTE PTR _$S285
	and	eax, 2
	test	eax, eax
	jne	SHORT $L74474
	mov	cl, BYTE PTR _$S285
	or	cl, 2
	mov	BYTE PTR _$S285, cl
	mov	ecx, OFFSET FLAT:_?sWndClass@?1??GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z@4V3@A
	call	??0CString@@QAE@XZ			; CString::CString
	push	OFFSET FLAT:_$E289
	call	_atexit
	add	esp, 4
$L74474:

; 44   : 	sWndClass.Empty(); // reset each time

	mov	ecx, OFFSET FLAT:_?sWndClass@?1??GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z@4V3@A
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 45   : 	
; 46   : 	if (hWnd)

	cmp	DWORD PTR _hWnd$[ebp], 0
	je	SHORT $L74476

; 48   : 		int nLen = ::GetClassName(hWnd, sWndClass.GetBuffer(128), 128);

	mov	esi, esp
	push	128					; 00000080H
	push	128					; 00000080H
	mov	ecx, OFFSET FLAT:_?sWndClass@?1??GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z@4V3@A
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	call	DWORD PTR __imp__GetClassNameW@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nLen$74477[ebp], eax

; 49   : 
; 50   : 		sWndClass.ReleaseBuffer(nLen);

	mov	eax, DWORD PTR _nLen$74477[ebp]
	push	eax
	mov	ecx, OFFSET FLAT:_?sWndClass@?1??GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z@4V3@A
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 51   : 		sWndClass.MakeLower();

	mov	ecx, OFFSET FLAT:_?sWndClass@?1??GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z@4V3@A
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
$L74476:

; 53   : 	
; 54   : 	return sWndClass;

	push	OFFSET FLAT:_?sWndClass@?1??GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z@4V3@A
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T74948[ebp]
	or	ecx, 1
	mov	DWORD PTR $T74948[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 55   : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z ENDP	; CWinClasses::GetClass
_TEXT	ENDS
;	COMDAT _$E289
_TEXT	SEGMENT
_$E289	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?sWndClass@?1??GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z@4V3@A
	call	??1CString@@QAE@XZ			; CString::~CString
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E289	ENDP
_TEXT	ENDS
PUBLIC	?IsClass@CWinClasses@@SAHPAUHWND__@@PBG@Z	; CWinClasses::IsClass
PUBLIC	?IsClass@CWinClasses@@SAHPBG0@Z			; CWinClasses::IsClass
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
;	COMDAT xdata$x
; File D:\_CODE\Shared\WinClasses.cpp
xdata$x	SEGMENT
$T74958	DD	019930520H
	DD	01H
	DD	FLAT:$T74960
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74960	DD	0ffffffffH
	DD	FLAT:$L74956
xdata$x	ENDS
;	COMDAT ?IsClass@CWinClasses@@SAHPAUHWND__@@PBG@Z
_TEXT	SEGMENT
$T74954 = -16
$T74955 = -20
__$EHRec$ = -12
_hWnd$ = 8
_szClass$ = 12
?IsClass@CWinClasses@@SAHPAUHWND__@@PBG@Z PROC NEAR	; CWinClasses::IsClass, COMDAT

; 58   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74959
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 59   : 	if (hWnd)

	cmp	DWORD PTR _hWnd$[ebp], 0
	je	SHORT $L74483

; 60   : 		return IsClass(szClass, GetClass(hWnd));

	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T74955[ebp]
	push	ecx
	call	?GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z ; CWinClasses::GetClass
	add	esp, 8
	mov	DWORD PTR -24+[ebp], eax
	mov	edx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -28+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	eax, DWORD PTR _szClass$[ebp]
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	mov	DWORD PTR $T74954[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T74955[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74954[ebp]
	jmp	SHORT $L74482
$L74483:

; 61   : 	
; 62   : 	return FALSE;

	xor	eax, eax
$L74482:

; 63   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74956:
	lea	ecx, DWORD PTR $T74955[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74959:
	mov	eax, OFFSET FLAT:$T74958
	jmp	___CxxFrameHandler
text$x	ENDS
?IsClass@CWinClasses@@SAHPAUHWND__@@PBG@Z ENDP		; CWinClasses::IsClass
EXTRN	__imp___wcsicmp:NEAR
;	COMDAT ?IsClass@CWinClasses@@SAHPBG0@Z
_TEXT	SEGMENT
_szClass$ = 8
_szWndClass$ = 12
?IsClass@CWinClasses@@SAHPBG0@Z PROC NEAR		; CWinClasses::IsClass, COMDAT

; 36   : 	static BOOL IsClass(LPCTSTR szClass, LPCTSTR szWndClass) { return (_tcsicmp(szClass, szWndClass) == 0); }

	push	ebp
	mov	ebp, esp
	push	esi
	mov	esi, esp
	mov	eax, DWORD PTR _szWndClass$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szClass$[ebp]
	push	ecx
	call	DWORD PTR __imp___wcsicmp
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	neg	eax
	sbb	eax, eax
	inc	eax
	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?IsClass@CWinClasses@@SAHPBG0@Z ENDP			; CWinClasses::IsClass
_TEXT	ENDS
PUBLIC	?GetClassEx@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z ; CWinClasses::GetClassEx
PUBLIC	?IsClassEx@CWinClasses@@SAHPAUHWND__@@PBG@Z	; CWinClasses::IsClassEx
EXTRN	?FromHandlePermanent@CWnd@@SGPAV1@PAUHWND__@@@Z:NEAR ; CWnd::FromHandlePermanent
EXTRN	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z:NEAR ; CObject::IsKindOf
EXTRN	__imp_?classCMDIChildWnd@CMDIChildWnd@@2UCRuntimeClass@@B:BYTE
EXTRN	__imp_?classCMiniFrameWnd@CMiniFrameWnd@@2UCRuntimeClass@@B:BYTE
EXTRN	__imp_?classCMiniDockFrameWnd@CMiniDockFrameWnd@@2UCRuntimeClass@@B:BYTE
EXTRN	__imp_?classCView@CView@@2UCRuntimeClass@@B:BYTE
EXTRN	__imp_?classCDialogBar@CDialogBar@@2UCRuntimeClass@@B:BYTE
EXTRN	__imp_?classCSplitterWnd@CSplitterWnd@@2UCRuntimeClass@@B:BYTE
EXTRN	__imp_?classCFrameWnd@CFrameWnd@@2UCRuntimeClass@@B:BYTE
EXTRN	__imp_?classCControlBar@CControlBar@@2UCRuntimeClass@@B:BYTE
EXTRN	??8@YG_NABVCString@@PBG@Z:NEAR			; operator==
EXTRN	__imp_?classCMDIFrameWnd@CMDIFrameWnd@@2UCRuntimeClass@@B:BYTE
;	COMDAT data
; File D:\_CODE\Shared\WinClasses.cpp
data	SEGMENT
$SG74499 DB	'M', 00H, 'f', 00H, 'c', 00H, 'W', 00H, 'n', 00H, 'd', 00H
	DB	00H, 00H
	ORG $+2
$SG74504 DB	'M', 00H, 'f', 00H, 'c', 00H, 'F', 00H, 'r', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 00H, 00H
	ORG $+2
$SG74509 DB	'M', 00H, 'f', 00H, 'c', 00H, 'V', 00H, 'i', 00H, 'e', 00H
	DB	'w', 00H, 00H, 00H
$SG74514 DB	'M', 00H, 'f', 00H, 'c', 00H, 'F', 00H, 'r', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 'M', 00H, 'D', 00H, 'I', 00H, 00H, 00H
$SG74519 DB	'M', 00H, 'f', 00H, 'c', 00H, 'F', 00H, 'r', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 'M', 00H, 'D', 00H, 'I', 00H, 'C', 00H, 'h'
	DB	00H, 'i', 00H, 'l', 00H, 'd', 00H, 00H, 00H
	ORG $+2
$SG74524 DB	'M', 00H, 'f', 00H, 'c', 00H, 'F', 00H, 'r', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 'M', 00H, 'i', 00H, 'n', 00H, 'i', 00H, 'D'
	DB	00H, 'o', 00H, 'c', 00H, 'k', 00H, 00H, 00H
	ORG $+2
$SG74529 DB	'M', 00H, 'f', 00H, 'c', 00H, 'F', 00H, 'r', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 'M', 00H, 'i', 00H, 'n', 00H, 'i', 00H, 00H
	DB	00H
	ORG $+2
$SG74534 DB	'M', 00H, 'f', 00H, 'c', 00H, 'S', 00H, 'p', 00H, 'l', 00H
	DB	'i', 00H, 't', 00H, 't', 00H, 'e', 00H, 'r', 00H, 00H, 00H
$SG74539 DB	'M', 00H, 'f', 00H, 'c', 00H, 'D', 00H, 'i', 00H, 'a', 00H
	DB	'l', 00H, 'o', 00H, 'g', 00H, 'B', 00H, 'a', 00H, 'r', 00H, 00H
	DB	00H
	ORG $+2
$SG74544 DB	'A', 00H, 'f', 00H, 'x', 00H, 'C', 00H, 'o', 00H, 'n', 00H
	DB	't', 00H, 'r', 00H, 'o', 00H, 'l', 00H, 'B', 00H, 'a', 00H, 'r'
	DB	00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74980	DD	019930520H
	DD	01H
	DD	FLAT:$T74982
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74982	DD	0ffffffffH
	DD	FLAT:$L74978
xdata$x	ENDS
;	COMDAT ?IsClassEx@CWinClasses@@SAHPAUHWND__@@PBG@Z
_TEXT	SEGMENT
$T74967 = -24
$T74968 = -28
$T74969 = -32
$T74970 = -36
$T74971 = -40
$T74972 = -44
$T74973 = -48
$T74974 = -52
$T74975 = -56
$T74976 = -60
$T74977 = -64
__$EHRec$ = -12
_hWnd$ = 8
_szClass$ = 12
_sClass$74491 = -16
_pWnd$74496 = -20
?IsClassEx@CWinClasses@@SAHPAUHWND__@@PBG@Z PROC NEAR	; CWinClasses::IsClassEx, COMDAT

; 66   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74981
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 52					; 00000034H
	push	edi
	lea	edi, DWORD PTR [ebp-64]
	mov	ecx, 13					; 0000000dH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 67   : 	if (hWnd)

	cmp	DWORD PTR _hWnd$[ebp], 0
	je	$L74490

; 69   : 		CString sClass = GetClassEx(hWnd);

	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	push	ecx
	call	?GetClassEx@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z ; CWinClasses::GetClassEx
	add	esp, 8
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 70   : 		
; 71   : 		if (IsClass(sClass, szClass))

	mov	edx, DWORD PTR _szClass$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L74493

; 72   : 			return TRUE;

	mov	DWORD PTR $T74967[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74967[ebp]
	jmp	$L74489
$L74493:

; 75   : 			CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);

	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	call	?FromHandlePermanent@CWnd@@SGPAV1@PAUHWND__@@@Z ; CWnd::FromHandlePermanent
	mov	DWORD PTR _pWnd$74496[ebp], eax

; 76   : 			
; 77   : 			if (pWnd)

	cmp	DWORD PTR _pWnd$74496[ebp], 0
	je	$L74543

; 79   : 				if (sClass == WC_MFCWND)

	push	OFFSET FLAT:$SG74499
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	push	ecx
	call	??8@YG_NABVCString@@PBG@Z		; operator==
	and	eax, 255				; 000000ffH
	test	eax, eax
	je	SHORT $L74498

; 80   : 					return TRUE;

	mov	DWORD PTR $T74968[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74968[ebp]
	jmp	$L74489
$L74498:

; 81   : 				
; 82   : 				else if (IsClass(sClass, WC_MFCFRAME) && pWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))

	push	OFFSET FLAT:$SG74504
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L74503
	mov	edx, DWORD PTR __imp_?classCFrameWnd@CFrameWnd@@2UCRuntimeClass@@B
	push	edx
	mov	ecx, DWORD PTR _pWnd$74496[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74503

; 83   : 					return TRUE;

	mov	DWORD PTR $T74969[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74969[ebp]
	jmp	$L74489
$L74503:

; 84   : 				
; 85   : 				else if (IsClass(sClass, WC_MFCVIEW) && pWnd->IsKindOf(RUNTIME_CLASS(CView)))

	push	OFFSET FLAT:$SG74509
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L74508
	mov	eax, DWORD PTR __imp_?classCView@CView@@2UCRuntimeClass@@B
	push	eax
	mov	ecx, DWORD PTR _pWnd$74496[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74508

; 86   : 					return TRUE;

	mov	DWORD PTR $T74970[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74970[ebp]
	jmp	$L74489
$L74508:

; 87   : 				
; 88   : 				else if (IsClass(sClass, WC_MFCMDIFRAME) && pWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))

	push	OFFSET FLAT:$SG74514
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L74513
	mov	ecx, DWORD PTR __imp_?classCMDIFrameWnd@CMDIFrameWnd@@2UCRuntimeClass@@B
	push	ecx
	mov	ecx, DWORD PTR _pWnd$74496[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74513

; 89   : 					return TRUE;

	mov	DWORD PTR $T74971[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74971[ebp]
	jmp	$L74489
$L74513:

; 90   : 				
; 91   : 				else if (IsClass(sClass, WC_MFCMDICHILD) && pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))

	push	OFFSET FLAT:$SG74519
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L74518
	mov	edx, DWORD PTR __imp_?classCMDIChildWnd@CMDIChildWnd@@2UCRuntimeClass@@B
	push	edx
	mov	ecx, DWORD PTR _pWnd$74496[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74518

; 92   : 					return TRUE;

	mov	DWORD PTR $T74972[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74972[ebp]
	jmp	$L74489
$L74518:

; 93   : 				
; 94   : 				else if (IsClass(sClass, WC_MFCMINIDOCKFRAME) && pWnd->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))

	push	OFFSET FLAT:$SG74524
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L74523
	mov	eax, DWORD PTR __imp_?classCMiniDockFrameWnd@CMiniDockFrameWnd@@2UCRuntimeClass@@B
	push	eax
	mov	ecx, DWORD PTR _pWnd$74496[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74523

; 95   : 					return TRUE;

	mov	DWORD PTR $T74973[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74973[ebp]
	jmp	$L74489
$L74523:

; 96   : 				
; 97   : 				else if (IsClass(sClass, WC_MFCMINIFRAME) && pWnd->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))

	push	OFFSET FLAT:$SG74529
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L74528
	mov	ecx, DWORD PTR __imp_?classCMiniFrameWnd@CMiniFrameWnd@@2UCRuntimeClass@@B
	push	ecx
	mov	ecx, DWORD PTR _pWnd$74496[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74528

; 98   : 					return TRUE;

	mov	DWORD PTR $T74974[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74974[ebp]
	jmp	$L74489
$L74528:

; 99   : 				
; 100  : 				else if (IsClass(sClass, WC_MFCSPLITTER) && pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))

	push	OFFSET FLAT:$SG74534
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L74533
	mov	edx, DWORD PTR __imp_?classCSplitterWnd@CSplitterWnd@@2UCRuntimeClass@@B
	push	edx
	mov	ecx, DWORD PTR _pWnd$74496[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74533

; 101  : 					return TRUE;

	mov	DWORD PTR $T74975[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74975[ebp]
	jmp	$L74489
$L74533:

; 102  : 				
; 103  : 				else if (IsClass(sClass, WC_MFCDIALOGBAR) && pWnd->IsKindOf(RUNTIME_CLASS(CDialogBar)))

	push	OFFSET FLAT:$SG74539
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L74538
	mov	eax, DWORD PTR __imp_?classCDialogBar@CDialogBar@@2UCRuntimeClass@@B
	push	eax
	mov	ecx, DWORD PTR _pWnd$74496[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74538

; 104  : 					return TRUE;

	mov	DWORD PTR $T74976[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74976[ebp]
	jmp	SHORT $L74489
$L74538:

; 105  : 				
; 106  : 				else if (IsClass(sClass, WC_CONTROLBAR) && pWnd->IsKindOf(RUNTIME_CLASS(CControlBar)))

	push	OFFSET FLAT:$SG74544
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L74543
	mov	ecx, DWORD PTR __imp_?classCControlBar@CControlBar@@2UCRuntimeClass@@B
	push	ecx
	mov	ecx, DWORD PTR _pWnd$74496[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74543

; 107  : 					return TRUE;

	mov	DWORD PTR $T74977[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74977[ebp]
	jmp	SHORT $L74489
$L74543:

; 110  : 	}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74490:

; 111  : 	
; 112  : 	return FALSE;

	xor	eax, eax
$L74489:

; 113  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 64					; 00000040H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74978:
	lea	ecx, DWORD PTR _sClass$74491[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74981:
	mov	eax, OFFSET FLAT:$T74980
	jmp	___CxxFrameHandler
text$x	ENDS
?IsClassEx@CWinClasses@@SAHPAUHWND__@@PBG@Z ENDP	; CWinClasses::IsClassEx
PUBLIC	?IsClassEx@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClassEx
;	COMDAT data
; File D:\_CODE\Shared\WinClasses.cpp
data	SEGMENT
$SG74553 DB	'M', 00H, 'f', 00H, 'c', 00H, 'F', 00H, 'r', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 00H, 00H
	ORG $+2
$SG74556 DB	'M', 00H, 'f', 00H, 'c', 00H, 'F', 00H, 'r', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 'M', 00H, 'D', 00H, 'I', 00H, 00H, 00H
$SG74557 DB	'M', 00H, 'f', 00H, 'c', 00H, 'F', 00H, 'r', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 'M', 00H, 'D', 00H, 'I', 00H, 'C', 00H, 'h'
	DB	00H, 'i', 00H, 'l', 00H, 'd', 00H, 00H, 00H
	ORG $+2
$SG74558 DB	'M', 00H, 'f', 00H, 'c', 00H, 'F', 00H, 'r', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 'M', 00H, 'i', 00H, 'n', 00H, 'i', 00H, 'D'
	DB	00H, 'o', 00H, 'c', 00H, 'k', 00H, 00H, 00H
	ORG $+2
$SG74559 DB	'M', 00H, 'f', 00H, 'c', 00H, 'F', 00H, 'r', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 'M', 00H, 'i', 00H, 'n', 00H, 'i', 00H, 00H
	DB	00H
data	ENDS
;	COMDAT ?IsClassEx@CWinClasses@@SAHPBG0@Z
_TEXT	SEGMENT
_szClass$ = 8
_szWndClass$ = 12
?IsClassEx@CWinClasses@@SAHPBG0@Z PROC NEAR		; CWinClasses::IsClassEx, COMDAT

; 116  : {

	push	ebp
	mov	ebp, esp

; 117  : 	if (IsClass(szClass, szWndClass)) // string comparison

	mov	eax, DWORD PTR _szWndClass$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szClass$[ebp]
	push	ecx
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L74550

; 118  : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L74549
$L74550:

; 119  : 	
; 120  : 	else if (IsClass(szWndClass, WC_MFCFRAME)) // handle frame derivatives

	push	OFFSET FLAT:$SG74553
	mov	edx, DWORD PTR _szWndClass$[ebp]
	push	edx
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L74554

; 123  : 			IsClass(szClass, WC_MFCMINIDOCKFRAME) || IsClass(szClass, WC_MFCMINIFRAME))

	push	OFFSET FLAT:$SG74556
	mov	eax, DWORD PTR _szClass$[ebp]
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	jne	SHORT $L74555
	push	OFFSET FLAT:$SG74557
	mov	ecx, DWORD PTR _szClass$[ebp]
	push	ecx
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	jne	SHORT $L74555
	push	OFFSET FLAT:$SG74558
	mov	edx, DWORD PTR _szClass$[ebp]
	push	edx
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	jne	SHORT $L74555
	push	OFFSET FLAT:$SG74559
	mov	eax, DWORD PTR _szClass$[ebp]
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L74554
$L74555:

; 124  : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L74549
$L74554:

; 126  : 	
; 127  : 	return FALSE;

	xor	eax, eax
$L74549:

; 128  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?IsClassEx@CWinClasses@@SAHPBG0@Z ENDP			; CWinClasses::IsClassEx
_TEXT	ENDS
PUBLIC	?IsControlClass@CWinClasses@@SAHPBG@Z		; CWinClasses::IsControlClass
EXTRN	__imp_?classCCheckListBox@CCheckListBox@@2UCRuntimeClass@@B:BYTE
EXTRN	??4CString@@QAEABV0@PBG@Z:NEAR			; CString::operator=
;	COMDAT data
; File D:\_CODE\Shared\WinClasses.cpp
data	SEGMENT
$SG74567 DB	'L', 00H, 'i', 00H, 's', 00H, 't', 00H, 'B', 00H, 'o', 00H
	DB	'x', 00H, 00H, 00H
$SG74571 DB	'C', 00H, 'h', 00H, 'e', 00H, 'c', 00H, 'k', 00H, 'L', 00H
	DB	'i', 00H, 's', 00H, 't', 00H, 'B', 00H, 'o', 00H, 'x', 00H, 00H
	DB	00H
	ORG $+2
$SG74578 DB	'M', 00H, 'f', 00H, 'c', 00H, 'V', 00H, 'i', 00H, 'e', 00H
	DB	'w', 00H, 00H, 00H
$SG74582 DB	'M', 00H, 'f', 00H, 'c', 00H, 'F', 00H, 'r', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 'M', 00H, 'D', 00H, 'I', 00H, 00H, 00H
$SG74586 DB	'M', 00H, 'f', 00H, 'c', 00H, 'F', 00H, 'r', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 'M', 00H, 'D', 00H, 'I', 00H, 'C', 00H, 'h'
	DB	00H, 'i', 00H, 'l', 00H, 'd', 00H, 00H, 00H
	ORG $+2
$SG74590 DB	'M', 00H, 'f', 00H, 'c', 00H, 'F', 00H, 'r', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 'M', 00H, 'i', 00H, 'n', 00H, 'i', 00H, 'D'
	DB	00H, 'o', 00H, 'c', 00H, 'k', 00H, 00H, 00H
	ORG $+2
$SG74594 DB	'M', 00H, 'f', 00H, 'c', 00H, 'F', 00H, 'r', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 'M', 00H, 'i', 00H, 'n', 00H, 'i', 00H, 00H
	DB	00H
	ORG $+2
$SG74598 DB	'M', 00H, 'f', 00H, 'c', 00H, 'F', 00H, 'r', 00H, 'a', 00H
	DB	'm', 00H, 'e', 00H, 00H, 00H
	ORG $+2
$SG74602 DB	'M', 00H, 'f', 00H, 'c', 00H, 'S', 00H, 'p', 00H, 'l', 00H
	DB	'i', 00H, 't', 00H, 't', 00H, 'e', 00H, 'r', 00H, 00H, 00H
$SG74606 DB	'M', 00H, 'f', 00H, 'c', 00H, 'D', 00H, 'i', 00H, 'a', 00H
	DB	'l', 00H, 'o', 00H, 'g', 00H, 'B', 00H, 'a', 00H, 'r', 00H, 00H
	DB	00H
	ORG $+2
$SG74610 DB	'A', 00H, 'f', 00H, 'x', 00H, 'C', 00H, 'o', 00H, 'n', 00H
	DB	't', 00H, 'r', 00H, 'o', 00H, 'l', 00H, 'B', 00H, 'a', 00H, 'r'
	DB	00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74996	DD	019930520H
	DD	02H
	DD	FLAT:$T74998
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74998	DD	0ffffffffH
	DD	FLAT:$L74992
	DD	00H
	DD	FLAT:$L74991
xdata$x	ENDS
;	COMDAT ?GetClassEx@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z
_TEXT	SEGMENT
$T74993 = -28
__$EHRec$ = -12
_hWnd$ = 12
___$ReturnUdt$ = 8
_sClass$ = -16
_pWnd$74568 = -20
_pWnd$74574 = -24
?GetClassEx@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z PROC NEAR ; CWinClasses::GetClassEx, COMDAT

; 131  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74997
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR $T74993[ebp], 0

; 132  : 	CString sClass = GetClass(hWnd);

	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sClass$[ebp]
	push	ecx
	call	?GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z ; CWinClasses::GetClass
	add	esp, 8
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 133  : 	
; 134  : 	// handle CCheckListBox
; 135  : 	if (IsClass(sClass, WC_LISTBOX))

	push	OFFSET FLAT:$SG74567
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L74566

; 137  : 		CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);

	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	call	?FromHandlePermanent@CWnd@@SGPAV1@PAUHWND__@@@Z ; CWnd::FromHandlePermanent
	mov	DWORD PTR _pWnd$74568[ebp], eax

; 138  : 		
; 139  : 		if (pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CCheckListBox)))

	cmp	DWORD PTR _pWnd$74568[ebp], 0
	je	SHORT $L74570
	mov	eax, DWORD PTR __imp_?classCCheckListBox@CCheckListBox@@2UCRuntimeClass@@B
	push	eax
	mov	ecx, DWORD PTR _pWnd$74568[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74570

; 141  : 			sClass = WC_CHECKLISTBOX;

	push	OFFSET FLAT:$SG74571
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 142  : 			sClass.MakeLower();

	lea	ecx, DWORD PTR _sClass$[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
$L74570:

; 145  : 	else if (!IsControlClass(sClass))

	jmp	$L74575
$L74566:
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsControlClass@CWinClasses@@SAHPBG@Z	; CWinClasses::IsControlClass
	add	esp, 4
	test	eax, eax
	jne	$L74575

; 147  : 		// can do the check if pWnd is permanent else mfc will not yet
; 148  : 		// have hooked up
; 149  : 		CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);

	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	call	?FromHandlePermanent@CWnd@@SGPAV1@PAUHWND__@@@Z ; CWnd::FromHandlePermanent
	mov	DWORD PTR _pWnd$74574[ebp], eax

; 150  : 		
; 151  : 		if (pWnd)

	cmp	DWORD PTR _pWnd$74574[ebp], 0
	je	$L74575

; 153  : 			// must do the check in order of most derived class first
; 154  : 			if (pWnd->IsKindOf(RUNTIME_CLASS(CView)))

	mov	edx, DWORD PTR __imp_?classCView@CView@@2UCRuntimeClass@@B
	push	edx
	mov	ecx, DWORD PTR _pWnd$74574[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74577

; 155  : 				sClass = WC_MFCVIEW;

	push	OFFSET FLAT:$SG74578
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 156  : 			
; 157  : 			else if (pWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))

	jmp	$L74609
$L74577:
	mov	eax, DWORD PTR __imp_?classCMDIFrameWnd@CMDIFrameWnd@@2UCRuntimeClass@@B
	push	eax
	mov	ecx, DWORD PTR _pWnd$74574[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74581

; 158  : 				sClass = WC_MFCMDIFRAME;

	push	OFFSET FLAT:$SG74582
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 159  : 			
; 160  : 			else if (pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))

	jmp	$L74609
$L74581:
	mov	ecx, DWORD PTR __imp_?classCMDIChildWnd@CMDIChildWnd@@2UCRuntimeClass@@B
	push	ecx
	mov	ecx, DWORD PTR _pWnd$74574[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74585

; 161  : 				sClass = WC_MFCMDICHILD;

	push	OFFSET FLAT:$SG74586
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 162  : 			
; 163  : 			else if (pWnd->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))

	jmp	$L74609
$L74585:
	mov	edx, DWORD PTR __imp_?classCMiniDockFrameWnd@CMiniDockFrameWnd@@2UCRuntimeClass@@B
	push	edx
	mov	ecx, DWORD PTR _pWnd$74574[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74589

; 164  : 				sClass = WC_MFCMINIDOCKFRAME;

	push	OFFSET FLAT:$SG74590
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 165  : 			
; 166  : 			else if (pWnd->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))

	jmp	$L74609
$L74589:
	mov	eax, DWORD PTR __imp_?classCMiniFrameWnd@CMiniFrameWnd@@2UCRuntimeClass@@B
	push	eax
	mov	ecx, DWORD PTR _pWnd$74574[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74593

; 167  : 				sClass = WC_MFCMINIFRAME;

	push	OFFSET FLAT:$SG74594
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 168  : 			
; 169  : 			else if (pWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd))) // this is the catch all for frame wnds

	jmp	$L74609
$L74593:
	mov	ecx, DWORD PTR __imp_?classCFrameWnd@CFrameWnd@@2UCRuntimeClass@@B
	push	ecx
	mov	ecx, DWORD PTR _pWnd$74574[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74597

; 170  : 				sClass = WC_MFCFRAME;

	push	OFFSET FLAT:$SG74598
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 171  : 			
; 172  : 			else if (pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))

	jmp	SHORT $L74609
$L74597:
	mov	edx, DWORD PTR __imp_?classCSplitterWnd@CSplitterWnd@@2UCRuntimeClass@@B
	push	edx
	mov	ecx, DWORD PTR _pWnd$74574[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74601

; 173  : 				sClass = WC_MFCSPLITTER;

	push	OFFSET FLAT:$SG74602
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 174  : 			
; 175  : 			else if (pWnd->IsKindOf(RUNTIME_CLASS(CDialogBar)))

	jmp	SHORT $L74609
$L74601:
	mov	eax, DWORD PTR __imp_?classCDialogBar@CDialogBar@@2UCRuntimeClass@@B
	push	eax
	mov	ecx, DWORD PTR _pWnd$74574[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74605

; 176  : 				sClass = WC_MFCDIALOGBAR;

	push	OFFSET FLAT:$SG74606
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 177  : 			
; 178  : 			else if (pWnd->IsKindOf(RUNTIME_CLASS(CControlBar)))

	jmp	SHORT $L74609
$L74605:
	mov	ecx, DWORD PTR __imp_?classCControlBar@CControlBar@@2UCRuntimeClass@@B
	push	ecx
	mov	ecx, DWORD PTR _pWnd$74574[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L74609

; 179  : 				sClass = WC_CONTROLBAR;

	push	OFFSET FLAT:$SG74610
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L74609:

; 180  : 
; 181  : 			sClass.MakeLower();

	lea	ecx, DWORD PTR _sClass$[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
$L74575:

; 184  : 
; 185  : 	return sClass;

	lea	edx, DWORD PTR _sClass$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T74993[ebp]
	or	al, 1
	mov	DWORD PTR $T74993[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 186  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74991:
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74992:
	mov	eax, DWORD PTR $T74993[ebp]
	and	eax, 1
	test	eax, eax
	je	$L74994
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74994:
	ret	0
$L74997:
	mov	eax, OFFSET FLAT:$T74996
	jmp	___CxxFrameHandler
text$x	ENDS
?GetClassEx@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z ENDP ; CWinClasses::GetClassEx
PUBLIC	?IsControlClass@CWinClasses@@SAHPAUHWND__@@@Z	; CWinClasses::IsControlClass
;	COMDAT xdata$x
; File D:\_CODE\Shared\WinClasses.cpp
xdata$x	SEGMENT
$T75008	DD	019930520H
	DD	01H
	DD	FLAT:$T75010
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75010	DD	0ffffffffH
	DD	FLAT:$L75006
xdata$x	ENDS
;	COMDAT ?IsControlClass@CWinClasses@@SAHPAUHWND__@@@Z
_TEXT	SEGMENT
$T75004 = -16
$T75005 = -20
__$EHRec$ = -12
_hWnd$ = 8
?IsControlClass@CWinClasses@@SAHPAUHWND__@@@Z PROC NEAR	; CWinClasses::IsControlClass, COMDAT

; 189  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75009
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 190  : 	return IsControlClass(GetClass(hWnd));

	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T75005[ebp]
	push	ecx
	call	?GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z ; CWinClasses::GetClass
	add	esp, 8
	mov	DWORD PTR -24+[ebp], eax
	mov	edx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -28+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsControlClass@CWinClasses@@SAHPBG@Z	; CWinClasses::IsControlClass
	add	esp, 4
	mov	DWORD PTR $T75004[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T75005[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T75004[ebp]

; 191  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75006:
	lea	ecx, DWORD PTR $T75005[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75009:
	mov	eax, OFFSET FLAT:$T75008
	jmp	___CxxFrameHandler
text$x	ENDS
?IsControlClass@CWinClasses@@SAHPAUHWND__@@@Z ENDP	; CWinClasses::IsControlClass
EXTRN	?GetCount@CMapStringToPtr@@QBEHXZ:NEAR		; CMapStringToPtr::GetCount
EXTRN	?Lookup@CMapStringToPtr@@QBEHPBGAAPAX@Z:NEAR	; CMapStringToPtr::Lookup
EXTRN	??ACMapStringToPtr@@QAEAAPAXPBG@Z:NEAR		; CMapStringToPtr::operator[]
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
;	COMDAT data
; File D:\_CODE\Shared\WinClasses.cpp
data	SEGMENT
$SG74621 DB	'B', 00H, 'u', 00H, 't', 00H, 't', 00H, 'o', 00H, 'n', 00H
	DB	00H, 00H
	ORG $+2
$SG74623 DB	'S', 00H, 't', 00H, 'a', 00H, 't', 00H, 'i', 00H, 'c', 00H
	DB	00H, 00H
	ORG $+2
$SG74625 DB	'E', 00H, 'd', 00H, 'i', 00H, 't', 00H, 00H, 00H
	ORG $+2
$SG74627 DB	'C', 00H, 'o', 00H, 'm', 00H, 'b', 00H, 'o', 00H, 'B', 00H
	DB	'o', 00H, 'x', 00H, 00H, 00H
	ORG $+2
$SG74629 DB	'C', 00H, 'o', 00H, 'm', 00H, 'b', 00H, 'o', 00H, 'L', 00H
	DB	'B', 00H, 'o', 00H, 'x', 00H, 00H, 00H
$SG74631 DB	'L', 00H, 'i', 00H, 's', 00H, 't', 00H, 'B', 00H, 'o', 00H
	DB	'x', 00H, 00H, 00H
$SG74633 DB	'C', 00H, 'o', 00H, 'm', 00H, 'b', 00H, 'o', 00H, 'B', 00H
	DB	'o', 00H, 'x', 00H, 'E', 00H, 'x', 00H, '3', 00H, '2', 00H, 00H
	DB	00H
	ORG $+2
$SG74635 DB	'S', 00H, 'c', 00H, 'r', 00H, 'o', 00H, 'l', 00H, 'l', 00H
	DB	'b', 00H, 'a', 00H, 'r', 00H, 00H, 00H
$SG74637 DB	't', 00H, 'o', 00H, 'o', 00H, 'l', 00H, 'b', 00H, 'a', 00H
	DB	'r', 00H, 'w', 00H, 'i', 00H, 'n', 00H, 'd', 00H, 'o', 00H, 'w'
	DB	00H, '3', 00H, '2', 00H, 00H, 00H
$SG74639 DB	'm', 00H, 's', 00H, 'c', 00H, 't', 00H, 'l', 00H, 's', 00H
	DB	'_', 00H, 'u', 00H, 'p', 00H, 'd', 00H, 'o', 00H, 'w', 00H, 'n'
	DB	00H, '3', 00H, '2', 00H, 00H, 00H
$SG74641 DB	'm', 00H, 's', 00H, 'c', 00H, 't', 00H, 'l', 00H, 's', 00H
	DB	'_', 00H, 'p', 00H, 'r', 00H, 'o', 00H, 'g', 00H, 'r', 00H, 'e'
	DB	00H, 's', 00H, 's', 00H, '3', 00H, '2', 00H, 00H, 00H
$SG74643 DB	'm', 00H, 's', 00H, 'c', 00H, 't', 00H, 'l', 00H, 's', 00H
	DB	'_', 00H, 't', 00H, 'r', 00H, 'a', 00H, 'c', 00H, 'k', 00H, 'b'
	DB	00H, 'a', 00H, 'r', 00H, '3', 00H, '2', 00H, 00H, 00H
$SG74645 DB	'm', 00H, 's', 00H, 'c', 00H, 't', 00H, 'l', 00H, 's', 00H
	DB	'_', 00H, 'h', 00H, 'o', 00H, 't', 00H, 'k', 00H, 'e', 00H, 'y'
	DB	00H, '3', 00H, '2', 00H, 00H, 00H
$SG74647 DB	'S', 00H, 'H', 00H, 'E', 00H, 'L', 00H, 'L', 00H, 'D', 00H
	DB	'L', 00H, 'L', 00H, '_', 00H, 'D', 00H, 'e', 00H, 'f', 00H, 'V'
	DB	00H, 'i', 00H, 'e', 00H, 'w', 00H, 00H, 00H
	ORG $+2
$SG74649 DB	'm', 00H, 's', 00H, 'c', 00H, 't', 00H, 'l', 00H, 's', 00H
	DB	'_', 00H, 's', 00H, 't', 00H, 'a', 00H, 't', 00H, 'u', 00H, 's'
	DB	00H, 'b', 00H, 'a', 00H, 'r', 00H, '3', 00H, '2', 00H, 00H, 00H
	ORG $+2
$SG74651 DB	'S', 00H, 'y', 00H, 's', 00H, 'A', 00H, 'n', 00H, 'i', 00H
	DB	'm', 00H, 'a', 00H, 't', 00H, 'e', 00H, '3', 00H, '2', 00H, 00H
	DB	00H
	ORG $+2
$SG74653 DB	'R', 00H, 'i', 00H, 'c', 00H, 'h', 00H, 'e', 00H, 'd', 00H
	DB	'i', 00H, 't', 00H, 00H, 00H
	ORG $+2
$SG74655 DB	'R', 00H, 'i', 00H, 'c', 00H, 'h', 00H, 'E', 00H, 'd', 00H
	DB	'i', 00H, 't', 00H, '2', 00H, '0', 00H, 'W', 00H, 00H, 00H
$SG74657 DB	'R', 00H, 'i', 00H, 'c', 00H, 'h', 00H, 'E', 00H, 'd', 00H
	DB	'i', 00H, 't', 00H, '5', 00H, '0', 00H, 'W', 00H, 00H, 00H
$SG74659 DB	'S', 00H, 'y', 00H, 's', 00H, 'D', 00H, 'a', 00H, 't', 00H
	DB	'e', 00H, 'T', 00H, 'i', 00H, 'm', 00H, 'e', 00H, 'P', 00H, 'i'
	DB	00H, 'c', 00H, 'k', 00H, '3', 00H, '2', 00H, 00H, 00H
$SG74661 DB	'S', 00H, 'y', 00H, 's', 00H, 'M', 00H, 'o', 00H, 'n', 00H
	DB	't', 00H, 'h', 00H, 'C', 00H, 'a', 00H, 'l', 00H, '3', 00H, '2'
	DB	00H, 00H, 00H
$SG74663 DB	'R', 00H, 'e', 00H, 'B', 00H, 'a', 00H, 'r', 00H, 'W', 00H
	DB	'i', 00H, 'n', 00H, 'd', 00H, 'o', 00H, 'w', 00H, '3', 00H, '2'
	DB	00H, 00H, 00H
$SG74665 DB	't', 00H, 'o', 00H, 'o', 00H, 'l', 00H, 't', 00H, 'i', 00H
	DB	'p', 00H, 's', 00H, '_', 00H, 'c', 00H, 'l', 00H, 'a', 00H, 's'
	DB	00H, 's', 00H, '3', 00H, '2', 00H, 00H, 00H
	ORG $+2
$SG74667 DB	'T', 00H, 'h', 00H, 'u', 00H, 'm', 00H, 'b', 00H, 'n', 00H
	DB	'a', 00H, 'i', 00H, 'l', 00H, 'V', 00H, 'w', 00H, 'E', 00H, 'x'
	DB	00H, 't', 00H, 'W', 00H, 'n', 00H, 'd', 00H, '3', 00H, '2', 00H
	DB	00H, 00H
$SG74669 DB	'I', 00H, 'n', 00H, 't', 00H, 'e', 00H, 'r', 00H, 'n', 00H
	DB	'e', 00H, 't', 00H, ' ', 00H, 'E', 00H, 'x', 00H, 'p', 00H, 'l'
	DB	00H, 'o', 00H, 'r', 00H, 'e', 00H, 'r', 00H, '_', 00H, 'S', 00H
	DB	'e', 00H, 'r', 00H, 'v', 00H, 'e', 00H, 'r', 00H, 00H, 00H
	ORG $+2
$SG74671 DB	'S', 00H, 'h', 00H, 'e', 00H, 'l', 00H, 'l', 00H, ' ', 00H
	DB	'D', 00H, 'o', 00H, 'c', 00H, 'O', 00H, 'b', 00H, 'j', 00H, 'e'
	DB	00H, 'c', 00H, 't', 00H, ' ', 00H, 'V', 00H, 'i', 00H, 'e', 00H
	DB	'w', 00H, 00H, 00H
	ORG $+2
$SG74673 DB	'S', 00H, 'h', 00H, 'e', 00H, 'l', 00H, 'l', 00H, ' ', 00H
	DB	'E', 00H, 'm', 00H, 'b', 00H, 'e', 00H, 'd', 00H, 'd', 00H, 'i'
	DB	00H, 'n', 00H, 'g', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T75045	DD	019930520H
	DD	01cH
	DD	FLAT:$T75047
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75047	DD	0ffffffffH
	DD	FLAT:$L75016
	DD	0ffffffffH
	DD	FLAT:$L75017
	DD	0ffffffffH
	DD	FLAT:$L75018
	DD	0ffffffffH
	DD	FLAT:$L75019
	DD	0ffffffffH
	DD	FLAT:$L75020
	DD	0ffffffffH
	DD	FLAT:$L75021
	DD	0ffffffffH
	DD	FLAT:$L75022
	DD	0ffffffffH
	DD	FLAT:$L75023
	DD	0ffffffffH
	DD	FLAT:$L75024
	DD	0ffffffffH
	DD	FLAT:$L75025
	DD	0ffffffffH
	DD	FLAT:$L75026
	DD	0ffffffffH
	DD	FLAT:$L75027
	DD	0ffffffffH
	DD	FLAT:$L75028
	DD	0ffffffffH
	DD	FLAT:$L75029
	DD	0ffffffffH
	DD	FLAT:$L75030
	DD	0ffffffffH
	DD	FLAT:$L75031
	DD	0ffffffffH
	DD	FLAT:$L75032
	DD	0ffffffffH
	DD	FLAT:$L75033
	DD	0ffffffffH
	DD	FLAT:$L75034
	DD	0ffffffffH
	DD	FLAT:$L75035
	DD	0ffffffffH
	DD	FLAT:$L75036
	DD	0ffffffffH
	DD	FLAT:$L75037
	DD	0ffffffffH
	DD	FLAT:$L75038
	DD	0ffffffffH
	DD	FLAT:$L75039
	DD	0ffffffffH
	DD	FLAT:$L75040
	DD	0ffffffffH
	DD	FLAT:$L75041
	DD	0ffffffffH
	DD	FLAT:$L75042
	DD	0ffffffffH
	DD	FLAT:$L75043
xdata$x	ENDS
;	COMDAT ?IsControlClass@CWinClasses@@SAHPBG@Z
_TEXT	SEGMENT
$T75015 = -132
__$EHRec$ = -12
_szClass$ = 8
_sCtrl$74620 = -24
_sCtrl$74622 = -28
_sCtrl$74624 = -32
_sCtrl$74626 = -36
_sCtrl$74628 = -40
_sCtrl$74630 = -44
_sCtrl$74632 = -48
_sCtrl$74634 = -52
_sCtrl$74636 = -56
_sCtrl$74638 = -60
_sCtrl$74640 = -64
_sCtrl$74642 = -68
_sCtrl$74644 = -72
_sCtrl$74646 = -76
_sCtrl$74648 = -80
_sCtrl$74650 = -84
_sCtrl$74652 = -88
_sCtrl$74654 = -92
_sCtrl$74656 = -96
_sCtrl$74658 = -100
_sCtrl$74660 = -104
_sCtrl$74662 = -108
_sCtrl$74664 = -112
_sCtrl$74666 = -116
_sCtrl$74668 = -120
_sCtrl$74670 = -124
_sCtrl$74672 = -128
_pTemp$ = -20
_sClass$ = -16
?IsControlClass@CWinClasses@@SAHPBG@Z PROC NEAR		; CWinClasses::IsControlClass, COMDAT

; 194  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75046
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 120				; 00000078H
	push	edi
	lea	edi, DWORD PTR [ebp-132]
	mov	ecx, 30					; 0000001eH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 195  : 	// init
; 196  : 	if (!s_mapCtrlClasses.GetCount())

	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	?GetCount@CMapStringToPtr@@QBEHXZ	; CMapStringToPtr::GetCount
	test	eax, eax
	jne	$L74619

; 198  : 		ADDCONTROL(WC_BUTTON);

	push	OFFSET FLAT:$SG74621
	lea	ecx, DWORD PTR _sCtrl$74620[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sCtrl$74620[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74620[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74620[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 199  : 		ADDCONTROL(WC_STATIC);

	push	OFFSET FLAT:$SG74623
	lea	ecx, DWORD PTR _sCtrl$74622[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sCtrl$74622[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74622[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74622[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 200  : 		ADDCONTROL(WC_EDIT);  

	push	OFFSET FLAT:$SG74625
	lea	ecx, DWORD PTR _sCtrl$74624[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sCtrl$74624[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74624[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74624[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 201  : 		ADDCONTROL(WC_COMBOBOX);

	push	OFFSET FLAT:$SG74627
	lea	ecx, DWORD PTR _sCtrl$74626[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR _sCtrl$74626[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74626[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74626[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 202  : 		ADDCONTROL(WC_COMBOLBOX);

	push	OFFSET FLAT:$SG74629
	lea	ecx, DWORD PTR _sCtrl$74628[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR _sCtrl$74628[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74628[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74628[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 203  : 		ADDCONTROL(WC_LISTBOX);  

	push	OFFSET FLAT:$SG74631
	lea	ecx, DWORD PTR _sCtrl$74630[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 5
	lea	ecx, DWORD PTR _sCtrl$74630[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74630[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74630[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 204  : 		ADDCONTROL(WC_COMBOBOXEX);     

	push	OFFSET FLAT:$SG74633
	lea	ecx, DWORD PTR _sCtrl$74632[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR _sCtrl$74632[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74632[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74632[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 205  : 		ADDCONTROL(WC_SCROLLBAR);

	push	OFFSET FLAT:$SG74635
	lea	ecx, DWORD PTR _sCtrl$74634[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 7
	lea	ecx, DWORD PTR _sCtrl$74634[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74634[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74634[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 206  : 		ADDCONTROL(WC_TOOLBAR);  

	push	OFFSET FLAT:$SG74637
	lea	ecx, DWORD PTR _sCtrl$74636[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 8
	lea	ecx, DWORD PTR _sCtrl$74636[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74636[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74636[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 207  : 		ADDCONTROL(WC_SPIN);     

	push	OFFSET FLAT:$SG74639
	lea	ecx, DWORD PTR _sCtrl$74638[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 9
	lea	ecx, DWORD PTR _sCtrl$74638[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74638[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74638[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 208  : 		ADDCONTROL(WC_PROGRESS); 

	push	OFFSET FLAT:$SG74641
	lea	ecx, DWORD PTR _sCtrl$74640[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 10		; 0000000aH
	lea	ecx, DWORD PTR _sCtrl$74640[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74640[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74640[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 209  : 		ADDCONTROL(WC_SLIDER);   

	push	OFFSET FLAT:$SG74643
	lea	ecx, DWORD PTR _sCtrl$74642[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 11		; 0000000bH
	lea	ecx, DWORD PTR _sCtrl$74642[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74642[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74642[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 210  : 		ADDCONTROL(WC_HOTKEY);   

	push	OFFSET FLAT:$SG74645
	lea	ecx, DWORD PTR _sCtrl$74644[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 12		; 0000000cH
	lea	ecx, DWORD PTR _sCtrl$74644[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74644[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74644[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 211  : 		ADDCONTROL(WC_SHELLDLLDEFVIEW);  

	push	OFFSET FLAT:$SG74647
	lea	ecx, DWORD PTR _sCtrl$74646[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 13		; 0000000dH
	lea	ecx, DWORD PTR _sCtrl$74646[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74646[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74646[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 212  : 		ADDCONTROL(WC_STATUSBAR);        

	push	OFFSET FLAT:$SG74649
	lea	ecx, DWORD PTR _sCtrl$74648[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 14		; 0000000eH
	lea	ecx, DWORD PTR _sCtrl$74648[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74648[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74648[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 213  : 		ADDCONTROL(WC_ANIMATE);          

	push	OFFSET FLAT:$SG74651
	lea	ecx, DWORD PTR _sCtrl$74650[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 15		; 0000000fH
	lea	ecx, DWORD PTR _sCtrl$74650[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74650[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74650[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 214  : 		ADDCONTROL(WC_RICHEDIT);         

	push	OFFSET FLAT:$SG74653
	lea	ecx, DWORD PTR _sCtrl$74652[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 16		; 00000010H
	lea	ecx, DWORD PTR _sCtrl$74652[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74652[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74652[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 215  : 		ADDCONTROL(WC_RICHEDIT20);       

	push	OFFSET FLAT:$SG74655
	lea	ecx, DWORD PTR _sCtrl$74654[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 17		; 00000011H
	lea	ecx, DWORD PTR _sCtrl$74654[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74654[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74654[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 216  : 		ADDCONTROL(WC_RICHEDIT50);  

	push	OFFSET FLAT:$SG74657
	lea	ecx, DWORD PTR _sCtrl$74656[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 18		; 00000012H
	lea	ecx, DWORD PTR _sCtrl$74656[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74656[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74656[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 217  : 		ADDCONTROL(WC_DATETIMEPICK);     

	push	OFFSET FLAT:$SG74659
	lea	ecx, DWORD PTR _sCtrl$74658[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 19		; 00000013H
	lea	ecx, DWORD PTR _sCtrl$74658[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74658[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74658[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 218  : 		ADDCONTROL(WC_MONTHCAL);         

	push	OFFSET FLAT:$SG74661
	lea	ecx, DWORD PTR _sCtrl$74660[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 20		; 00000014H
	lea	ecx, DWORD PTR _sCtrl$74660[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74660[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74660[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 219  : 		ADDCONTROL(WC_REBAR);            

	push	OFFSET FLAT:$SG74663
	lea	ecx, DWORD PTR _sCtrl$74662[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 21		; 00000015H
	lea	ecx, DWORD PTR _sCtrl$74662[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74662[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74662[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 220  : 		ADDCONTROL(WC_TOOLTIPS); 

	push	OFFSET FLAT:$SG74665
	lea	ecx, DWORD PTR _sCtrl$74664[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 22		; 00000016H
	lea	ecx, DWORD PTR _sCtrl$74664[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74664[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74664[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 221  : 		ADDCONTROL(WC_THUMBNAILVIEW); 

	push	OFFSET FLAT:$SG74667
	lea	ecx, DWORD PTR _sCtrl$74666[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 23		; 00000017H
	lea	ecx, DWORD PTR _sCtrl$74666[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74666[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74666[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 222  : 		ADDCONTROL(WC_IE); 

	push	OFFSET FLAT:$SG74669
	lea	ecx, DWORD PTR _sCtrl$74668[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 24		; 00000018H
	lea	ecx, DWORD PTR _sCtrl$74668[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74668[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74668[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 223  : 		ADDCONTROL(WC_SHDOCOBJVW); 

	push	OFFSET FLAT:$SG74671
	lea	ecx, DWORD PTR _sCtrl$74670[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 25		; 00000019H
	lea	ecx, DWORD PTR _sCtrl$74670[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74670[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74670[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 224  : 		ADDCONTROL(WC_SHELLEMBEDDING); 

	push	OFFSET FLAT:$SG74673
	lea	ecx, DWORD PTR _sCtrl$74672[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 26		; 0000001aH
	lea	ecx, DWORD PTR _sCtrl$74672[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower
	lea	ecx, DWORD PTR _sCtrl$74672[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	??ACMapStringToPtr@@QAEAAPAXPBG@Z	; CMapStringToPtr::operator[]
	mov	DWORD PTR [eax], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sCtrl$74672[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L74619:

; 228  : 	CString sClass(szClass);

	mov	eax, DWORD PTR _szClass$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 27		; 0000001bH

; 229  : 	sClass.MakeLower();

	lea	ecx, DWORD PTR _sClass$[ebp]
	call	?MakeLower@CString@@QAEXXZ		; CString::MakeLower

; 230  : 	
; 231  : 	return s_mapCtrlClasses.Lookup(sClass, pTemp);

	lea	ecx, DWORD PTR _pTemp$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, OFFSET FLAT:?s_mapCtrlClasses@CWinClasses@@1VCMapStringToPtr@@A
	call	?Lookup@CMapStringToPtr@@QBEHPBGAAPAX@Z	; CMapStringToPtr::Lookup
	mov	DWORD PTR $T75015[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T75015[ebp]

; 232  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 132				; 00000084H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75016:
	lea	ecx, DWORD PTR _sCtrl$74620[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75017:
	lea	ecx, DWORD PTR _sCtrl$74622[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75018:
	lea	ecx, DWORD PTR _sCtrl$74624[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75019:
	lea	ecx, DWORD PTR _sCtrl$74626[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75020:
	lea	ecx, DWORD PTR _sCtrl$74628[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75021:
	lea	ecx, DWORD PTR _sCtrl$74630[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75022:
	lea	ecx, DWORD PTR _sCtrl$74632[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75023:
	lea	ecx, DWORD PTR _sCtrl$74634[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75024:
	lea	ecx, DWORD PTR _sCtrl$74636[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75025:
	lea	ecx, DWORD PTR _sCtrl$74638[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75026:
	lea	ecx, DWORD PTR _sCtrl$74640[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75027:
	lea	ecx, DWORD PTR _sCtrl$74642[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75028:
	lea	ecx, DWORD PTR _sCtrl$74644[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75029:
	lea	ecx, DWORD PTR _sCtrl$74646[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75030:
	lea	ecx, DWORD PTR _sCtrl$74648[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75031:
	lea	ecx, DWORD PTR _sCtrl$74650[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75032:
	lea	ecx, DWORD PTR _sCtrl$74652[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75033:
	lea	ecx, DWORD PTR _sCtrl$74654[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75034:
	lea	ecx, DWORD PTR _sCtrl$74656[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75035:
	lea	ecx, DWORD PTR _sCtrl$74658[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75036:
	lea	ecx, DWORD PTR _sCtrl$74660[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75037:
	lea	ecx, DWORD PTR _sCtrl$74662[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75038:
	lea	ecx, DWORD PTR _sCtrl$74664[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75039:
	lea	ecx, DWORD PTR _sCtrl$74666[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75040:
	lea	ecx, DWORD PTR _sCtrl$74668[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75041:
	lea	ecx, DWORD PTR _sCtrl$74670[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75042:
	lea	ecx, DWORD PTR _sCtrl$74672[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75043:
	lea	ecx, DWORD PTR _sClass$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75046:
	mov	eax, OFFSET FLAT:$T75045
	jmp	___CxxFrameHandler
text$x	ENDS
?IsControlClass@CWinClasses@@SAHPBG@Z ENDP		; CWinClasses::IsControlClass
PUBLIC	?IsEditControl@CWinClasses@@SAHPBG@Z		; CWinClasses::IsEditControl
PUBLIC	?IsEditControl@CWinClasses@@SAHPAUHWND__@@@Z	; CWinClasses::IsEditControl
;	COMDAT xdata$x
; File D:\_CODE\Shared\WinClasses.cpp
xdata$x	SEGMENT
$T75057	DD	019930520H
	DD	01H
	DD	FLAT:$T75059
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75059	DD	0ffffffffH
	DD	FLAT:$L75055
xdata$x	ENDS
;	COMDAT ?IsEditControl@CWinClasses@@SAHPAUHWND__@@@Z
_TEXT	SEGMENT
$T75053 = -16
$T75054 = -20
__$EHRec$ = -12
_hWnd$ = 8
?IsEditControl@CWinClasses@@SAHPAUHWND__@@@Z PROC NEAR	; CWinClasses::IsEditControl, COMDAT

; 235  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75058
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 236  : 	return IsEditControl(GetClass(hWnd));

	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T75054[ebp]
	push	ecx
	call	?GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z ; CWinClasses::GetClass
	add	esp, 8
	mov	DWORD PTR -24+[ebp], eax
	mov	edx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -28+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsEditControl@CWinClasses@@SAHPBG@Z	; CWinClasses::IsEditControl
	add	esp, 4
	mov	DWORD PTR $T75053[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T75054[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T75053[ebp]

; 237  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75055:
	lea	ecx, DWORD PTR $T75054[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75058:
	mov	eax, OFFSET FLAT:$T75057
	jmp	___CxxFrameHandler
text$x	ENDS
?IsEditControl@CWinClasses@@SAHPAUHWND__@@@Z ENDP	; CWinClasses::IsEditControl
PUBLIC	?IsRichEditControl@CWinClasses@@SAHPBG@Z	; CWinClasses::IsRichEditControl
;	COMDAT data
; File D:\_CODE\Shared\WinClasses.cpp
data	SEGMENT
$SG74685 DB	'E', 00H, 'd', 00H, 'i', 00H, 't', 00H, 00H, 00H
data	ENDS
;	COMDAT ?IsEditControl@CWinClasses@@SAHPBG@Z
_TEXT	SEGMENT
_szClass$ = 8
?IsEditControl@CWinClasses@@SAHPBG@Z PROC NEAR		; CWinClasses::IsEditControl, COMDAT

; 240  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 241  : 	return (IsRichEditControl(szClass) || IsClass(szClass, WC_EDIT));

	mov	eax, DWORD PTR _szClass$[ebp]
	push	eax
	call	?IsRichEditControl@CWinClasses@@SAHPBG@Z ; CWinClasses::IsRichEditControl
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75064
	push	OFFSET FLAT:$SG74685
	mov	ecx, DWORD PTR _szClass$[ebp]
	push	ecx
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75064
	mov	DWORD PTR -4+[ebp], 0
	jmp	SHORT $L75065
$L75064:
	mov	DWORD PTR -4+[ebp], 1
$L75065:
	mov	eax, DWORD PTR -4+[ebp]

; 242  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsEditControl@CWinClasses@@SAHPBG@Z ENDP		; CWinClasses::IsEditControl
_TEXT	ENDS
PUBLIC	?IsRichEditControl@CWinClasses@@SAHPAUHWND__@@@Z ; CWinClasses::IsRichEditControl
;	COMDAT xdata$x
; File D:\_CODE\Shared\WinClasses.cpp
xdata$x	SEGMENT
$T75073	DD	019930520H
	DD	01H
	DD	FLAT:$T75075
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75075	DD	0ffffffffH
	DD	FLAT:$L75071
xdata$x	ENDS
;	COMDAT ?IsRichEditControl@CWinClasses@@SAHPAUHWND__@@@Z
_TEXT	SEGMENT
$T75069 = -16
$T75070 = -20
__$EHRec$ = -12
_hWnd$ = 8
?IsRichEditControl@CWinClasses@@SAHPAUHWND__@@@Z PROC NEAR ; CWinClasses::IsRichEditControl, COMDAT

; 245  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75074
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 246  : 	return IsRichEditControl(GetClass(hWnd));

	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T75070[ebp]
	push	ecx
	call	?GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z ; CWinClasses::GetClass
	add	esp, 8
	mov	DWORD PTR -24+[ebp], eax
	mov	edx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -28+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsRichEditControl@CWinClasses@@SAHPBG@Z ; CWinClasses::IsRichEditControl
	add	esp, 4
	mov	DWORD PTR $T75069[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T75070[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T75069[ebp]

; 247  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75071:
	lea	ecx, DWORD PTR $T75070[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75074:
	mov	eax, OFFSET FLAT:$T75073
	jmp	___CxxFrameHandler
text$x	ENDS
?IsRichEditControl@CWinClasses@@SAHPAUHWND__@@@Z ENDP	; CWinClasses::IsRichEditControl
;	COMDAT data
; File D:\_CODE\Shared\WinClasses.cpp
data	SEGMENT
$SG74694 DB	'R', 00H, 'i', 00H, 'c', 00H, 'h', 00H, 'e', 00H, 'd', 00H
	DB	'i', 00H, 't', 00H, 00H, 00H
	ORG $+2
$SG74695 DB	'R', 00H, 'i', 00H, 'c', 00H, 'h', 00H, 'E', 00H, 'd', 00H
	DB	'i', 00H, 't', 00H, '2', 00H, '0', 00H, 'W', 00H, 00H, 00H
$SG74696 DB	'R', 00H, 'i', 00H, 'c', 00H, 'h', 00H, 'E', 00H, 'd', 00H
	DB	'i', 00H, 't', 00H, '5', 00H, '0', 00H, 'W', 00H, 00H, 00H
data	ENDS
;	COMDAT ?IsRichEditControl@CWinClasses@@SAHPBG@Z
_TEXT	SEGMENT
_szClass$ = 8
?IsRichEditControl@CWinClasses@@SAHPBG@Z PROC NEAR	; CWinClasses::IsRichEditControl, COMDAT

; 250  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 251  : 	return (IsClass(szClass, WC_RICHEDIT) ||
; 252  : 			IsClass(szClass, WC_RICHEDIT20) ||
; 253  : 			IsClass(szClass, WC_RICHEDIT50));

	push	OFFSET FLAT:$SG74694
	mov	eax, DWORD PTR _szClass$[ebp]
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75080
	push	OFFSET FLAT:$SG74695
	mov	ecx, DWORD PTR _szClass$[ebp]
	push	ecx
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75080
	push	OFFSET FLAT:$SG74696
	mov	edx, DWORD PTR _szClass$[ebp]
	push	edx
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75080
	mov	DWORD PTR -4+[ebp], 0
	jmp	SHORT $L75081
$L75080:
	mov	DWORD PTR -4+[ebp], 1
$L75081:
	mov	eax, DWORD PTR -4+[ebp]

; 254  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsRichEditControl@CWinClasses@@SAHPBG@Z ENDP		; CWinClasses::IsRichEditControl
_TEXT	ENDS
PUBLIC	?IsComboBox@CWinClasses@@SAHPBG@Z		; CWinClasses::IsComboBox
PUBLIC	?IsComboBox@CWinClasses@@SAHPAUHWND__@@@Z	; CWinClasses::IsComboBox
;	COMDAT xdata$x
; File D:\_CODE\Shared\WinClasses.cpp
xdata$x	SEGMENT
$T75089	DD	019930520H
	DD	01H
	DD	FLAT:$T75091
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75091	DD	0ffffffffH
	DD	FLAT:$L75087
xdata$x	ENDS
;	COMDAT ?IsComboBox@CWinClasses@@SAHPAUHWND__@@@Z
_TEXT	SEGMENT
$T75085 = -16
$T75086 = -20
__$EHRec$ = -12
_hWnd$ = 8
?IsComboBox@CWinClasses@@SAHPAUHWND__@@@Z PROC NEAR	; CWinClasses::IsComboBox, COMDAT

; 257  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75090
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 258  : 	return IsComboBox(GetClass(hWnd));

	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T75086[ebp]
	push	ecx
	call	?GetClass@CWinClasses@@SA?AVCString@@PAUHWND__@@@Z ; CWinClasses::GetClass
	add	esp, 8
	mov	DWORD PTR -24+[ebp], eax
	mov	edx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -28+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsComboBox@CWinClasses@@SAHPBG@Z	; CWinClasses::IsComboBox
	add	esp, 4
	mov	DWORD PTR $T75085[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T75086[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T75085[ebp]

; 259  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75087:
	lea	ecx, DWORD PTR $T75086[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75090:
	mov	eax, OFFSET FLAT:$T75089
	jmp	___CxxFrameHandler
text$x	ENDS
?IsComboBox@CWinClasses@@SAHPAUHWND__@@@Z ENDP		; CWinClasses::IsComboBox
;	COMDAT data
; File D:\_CODE\Shared\WinClasses.cpp
data	SEGMENT
$SG74705 DB	'C', 00H, 'o', 00H, 'm', 00H, 'b', 00H, 'o', 00H, 'B', 00H
	DB	'o', 00H, 'x', 00H, 00H, 00H
	ORG $+2
$SG74706 DB	'C', 00H, 'o', 00H, 'm', 00H, 'b', 00H, 'o', 00H, 'B', 00H
	DB	'o', 00H, 'x', 00H, 'E', 00H, 'x', 00H, '3', 00H, '2', 00H, 00H
	DB	00H
data	ENDS
;	COMDAT ?IsComboBox@CWinClasses@@SAHPBG@Z
_TEXT	SEGMENT
_szClass$ = 8
?IsComboBox@CWinClasses@@SAHPBG@Z PROC NEAR		; CWinClasses::IsComboBox, COMDAT

; 262  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 263  : 	return (IsClass(szClass, WC_COMBOBOX) || IsClass(szClass, WC_COMBOBOXEX));

	push	OFFSET FLAT:$SG74705
	mov	eax, DWORD PTR _szClass$[ebp]
	push	eax
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75096
	push	OFFSET FLAT:$SG74706
	mov	ecx, DWORD PTR _szClass$[ebp]
	push	ecx
	call	?IsClass@CWinClasses@@SAHPBG0@Z		; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75096
	mov	DWORD PTR -4+[ebp], 0
	jmp	SHORT $L75097
$L75096:
	mov	DWORD PTR -4+[ebp], 1
$L75097:
	mov	eax, DWORD PTR -4+[ebp]

; 264  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsComboBox@CWinClasses@@SAHPBG@Z ENDP			; CWinClasses::IsComboBox
_TEXT	ENDS
PUBLIC	?IsDialog@CWinClasses@@SAHPAUHWND__@@@Z		; CWinClasses::IsDialog
;	COMDAT data
; File D:\_CODE\Shared\WinClasses.cpp
data	SEGMENT
$SG74710 DB	'#', 00H, '3', 00H, '2', 00H, '7', 00H, '7', 00H, '0', 00H
	DB	00H, 00H
data	ENDS
;	COMDAT ?IsDialog@CWinClasses@@SAHPAUHWND__@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
?IsDialog@CWinClasses@@SAHPAUHWND__@@@Z PROC NEAR	; CWinClasses::IsDialog, COMDAT

; 267  : {

	push	ebp
	mov	ebp, esp

; 268  : 	return IsClass(hWnd, WC_DIALOGBOX);

	push	OFFSET FLAT:$SG74710
	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	call	?IsClass@CWinClasses@@SAHPAUHWND__@@PBG@Z ; CWinClasses::IsClass
	add	esp, 8

; 269  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?IsDialog@CWinClasses@@SAHPAUHWND__@@@Z ENDP		; CWinClasses::IsDialog
_TEXT	ENDS
PUBLIC	?IsChild@CWinClasses@@SAHPAUHWND__@@@Z		; CWinClasses::IsChild
EXTRN	__imp__GetWindowLongW@8:NEAR
;	COMDAT ?IsChild@CWinClasses@@SAHPAUHWND__@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
?IsChild@CWinClasses@@SAHPAUHWND__@@@Z PROC NEAR	; CWinClasses::IsChild, COMDAT

; 272  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 273  : 	return (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD);

	mov	esi, esp
	push	-16					; fffffff0H
	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	call	DWORD PTR __imp__GetWindowLongW@8
	cmp	esi, esp
	call	__chkesp
	and	eax, 1073741824				; 40000000H

; 274  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?IsChild@CWinClasses@@SAHPAUHWND__@@@Z ENDP		; CWinClasses::IsChild
_TEXT	ENDS
PUBLIC	?IsCommonDialog@CWinClasses@@SAHPAUHWND__@@W4WCLS_COMMONDIALOG@@@Z ; CWinClasses::IsCommonDialog
EXTRN	?GetDlgItem@CWnd@@QBEPAV1@H@Z:NEAR		; CWnd::GetDlgItem
EXTRN	__imp_?classCFileDialog@CFileDialog@@2UCRuntimeClass@@B:BYTE
EXTRN	__imp_?classCFontDialog@CFontDialog@@2UCRuntimeClass@@B:BYTE
EXTRN	__imp_?classCColorDialog@CColorDialog@@2UCRuntimeClass@@B:BYTE
EXTRN	__imp_?classCPageSetupDialog@CPageSetupDialog@@2UCRuntimeClass@@B:BYTE
EXTRN	?classCFolderDialog@CFolderDialog@@2UCRuntimeClass@@B:BYTE ; CFolderDialog::classCFolderDialog
EXTRN	__imp_?classCPrintDialog@CPrintDialog@@2UCRuntimeClass@@B:BYTE
EXTRN	__imp_?classCFindReplaceDialog@CFindReplaceDialog@@2UCRuntimeClass@@B:BYTE
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
;	COMDAT ?IsCommonDialog@CWinClasses@@SAHPAUHWND__@@W4WCLS_COMMONDIALOG@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
_nType$ = 12
_pChild$74721 = -8
_pDlg$ = -4
?IsCommonDialog@CWinClasses@@SAHPAUHWND__@@W4WCLS_COMMONDIALOG@@@Z PROC NEAR ; CWinClasses::IsCommonDialog, COMDAT

; 277  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 20					; 00000014H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 278  : 	// special case
; 279  : 	if (nType == WCD_ANY)

	cmp	DWORD PTR _nType$[ebp], 0
	jne	$L74718

; 281  : 		return IsCommonDialog(hWnd, WCD_PRINT) ||
; 282  : 				IsCommonDialog(hWnd, WCD_PAGESETUP) ||
; 283  : 				IsCommonDialog(hWnd, WCD_FONT) ||
; 284  : 				IsCommonDialog(hWnd, WCD_COLOR) ||
; 285  : 				IsCommonDialog(hWnd, WCD_FINDREPLACE) ||
; 286  : 				IsCommonDialog(hWnd, WCD_OPENSAVE) ||
; 287  : 				IsCommonDialog(hWnd, WCD_BROWSEFOLDER);

	push	1
	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	call	?IsCommonDialog@CWinClasses@@SAHPAUHWND__@@W4WCLS_COMMONDIALOG@@@Z ; CWinClasses::IsCommonDialog
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75106
	push	2
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	call	?IsCommonDialog@CWinClasses@@SAHPAUHWND__@@W4WCLS_COMMONDIALOG@@@Z ; CWinClasses::IsCommonDialog
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75106
	push	3
	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	call	?IsCommonDialog@CWinClasses@@SAHPAUHWND__@@W4WCLS_COMMONDIALOG@@@Z ; CWinClasses::IsCommonDialog
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75106
	push	4
	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	call	?IsCommonDialog@CWinClasses@@SAHPAUHWND__@@W4WCLS_COMMONDIALOG@@@Z ; CWinClasses::IsCommonDialog
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75106
	push	5
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	call	?IsCommonDialog@CWinClasses@@SAHPAUHWND__@@W4WCLS_COMMONDIALOG@@@Z ; CWinClasses::IsCommonDialog
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75106
	push	6
	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	call	?IsCommonDialog@CWinClasses@@SAHPAUHWND__@@W4WCLS_COMMONDIALOG@@@Z ; CWinClasses::IsCommonDialog
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75106
	push	7
	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	call	?IsCommonDialog@CWinClasses@@SAHPAUHWND__@@W4WCLS_COMMONDIALOG@@@Z ; CWinClasses::IsCommonDialog
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75106
	mov	DWORD PTR -12+[ebp], 0
	jmp	SHORT $L75107
$L75106:
	mov	DWORD PTR -12+[ebp], 1
$L75107:
	mov	eax, DWORD PTR -12+[ebp]
	jmp	$L74717
$L74718:

; 289  : 
; 290  : 	// all else
; 291  : 	if (!IsDialog(hWnd))

	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	call	?IsDialog@CWinClasses@@SAHPAUHWND__@@@Z	; CWinClasses::IsDialog
	add	esp, 4
	test	eax, eax
	jne	SHORT $L74719

; 292  : 		return FALSE;

	xor	eax, eax
	jmp	$L74717
$L74719:

; 293  : 
; 294  : 	// special case
; 295  : 	if (nType == WCD_OPENSAVE)

	cmp	DWORD PTR _nType$[ebp], 6
	jne	SHORT $L74720

; 297  : 		// The open save dialog we are interested in is actually
; 298  : 		// a child of the dialog that users see
; 299  : 		CWnd* pChild = CWnd::FromHandle(hWnd)->GetDlgItem(0);

	push	0
	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	call	?FromHandle@CWnd@@SGPAV1@PAUHWND__@@@Z	; CWnd::FromHandle
	mov	ecx, eax
	call	?GetDlgItem@CWnd@@QBEPAV1@H@Z		; CWnd::GetDlgItem
	mov	DWORD PTR _pChild$74721[ebp], eax

; 300  : 		return (pChild && pChild->IsKindOf(RUNTIME_CLASS(CFileDialog)));

	cmp	DWORD PTR _pChild$74721[ebp], 0
	je	SHORT $L75108
	mov	eax, DWORD PTR __imp_?classCFileDialog@CFileDialog@@2UCRuntimeClass@@B
	push	eax
	mov	ecx, DWORD PTR _pChild$74721[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	test	eax, eax
	je	SHORT $L75108
	mov	DWORD PTR -16+[ebp], 1
	jmp	SHORT $L75109
$L75108:
	mov	DWORD PTR -16+[ebp], 0
$L75109:
	mov	eax, DWORD PTR -16+[ebp]
	jmp	$L74717
$L74720:

; 302  : 
; 303  : 	// get permanent window mapping so we can
; 304  : 	// lookup runtime class type. 
; 305  : 	CWnd* pDlg = CWnd::FromHandlePermanent(hWnd);

	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	call	?FromHandlePermanent@CWnd@@SGPAV1@PAUHWND__@@@Z ; CWnd::FromHandlePermanent
	mov	DWORD PTR _pDlg$[ebp], eax

; 306  : 
; 307  : 	if (pDlg == NULL)

	cmp	DWORD PTR _pDlg$[ebp], 0
	jne	SHORT $L74724

; 308  : 		return FALSE;

	xor	eax, eax
	jmp	$L74717
$L74724:

; 311  : 	{

	mov	edx, DWORD PTR _nType$[ebp]
	mov	DWORD PTR -20+[ebp], edx
	mov	eax, DWORD PTR -20+[ebp]
	sub	eax, 1
	mov	DWORD PTR -20+[ebp], eax
	cmp	DWORD PTR -20+[ebp], 6
	ja	SHORT $L74726
	mov	ecx, DWORD PTR -20+[ebp]
	jmp	DWORD PTR $L75110[ecx*4]
$L74729:

; 312  : 	case WCD_PRINT:
; 313  : 		return pDlg->IsKindOf(RUNTIME_CLASS(CPrintDialog));

	mov	edx, DWORD PTR __imp_?classCPrintDialog@CPrintDialog@@2UCRuntimeClass@@B
	push	edx
	mov	ecx, DWORD PTR _pDlg$[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	jmp	SHORT $L74717
$L74731:

; 314  : 
; 315  : 	case WCD_PAGESETUP:
; 316  : 		return pDlg->IsKindOf(RUNTIME_CLASS(CPageSetupDialog));

	mov	eax, DWORD PTR __imp_?classCPageSetupDialog@CPageSetupDialog@@2UCRuntimeClass@@B
	push	eax
	mov	ecx, DWORD PTR _pDlg$[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	jmp	SHORT $L74717
$L74733:

; 317  : 
; 318  : 	case WCD_FONT:
; 319  : 		return pDlg->IsKindOf(RUNTIME_CLASS(CFontDialog));

	mov	ecx, DWORD PTR __imp_?classCFontDialog@CFontDialog@@2UCRuntimeClass@@B
	push	ecx
	mov	ecx, DWORD PTR _pDlg$[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	jmp	SHORT $L74717
$L74735:

; 320  : 
; 321  : 	case WCD_COLOR:
; 322  : 		return pDlg->IsKindOf(RUNTIME_CLASS(CColorDialog));

	mov	edx, DWORD PTR __imp_?classCColorDialog@CColorDialog@@2UCRuntimeClass@@B
	push	edx
	mov	ecx, DWORD PTR _pDlg$[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	jmp	SHORT $L74717
$L74737:

; 323  : 
; 324  : 	case WCD_FINDREPLACE:
; 325  : 		return pDlg->IsKindOf(RUNTIME_CLASS(CFindReplaceDialog));

	mov	eax, DWORD PTR __imp_?classCFindReplaceDialog@CFindReplaceDialog@@2UCRuntimeClass@@B
	push	eax
	mov	ecx, DWORD PTR _pDlg$[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	jmp	SHORT $L74717
$L74739:

; 326  : 
; 327  : 	case WCD_BROWSEFOLDER:
; 328  : 		return pDlg->IsKindOf(RUNTIME_CLASS(CFolderDialog));

	push	OFFSET FLAT:?classCFolderDialog@CFolderDialog@@2UCRuntimeClass@@B ; CFolderDialog::classCFolderDialog
	mov	ecx, DWORD PTR _pDlg$[ebp]
	call	?IsKindOf@CObject@@QBEHPBUCRuntimeClass@@@Z ; CObject::IsKindOf
	jmp	SHORT $L74717
$L74726:

; 330  : 
; 331  : 	// how do we get here?
; 332  : 	ASSERT(0);

	mov	ecx, 1
	test	ecx, ecx
	je	SHORT $L74744
	push	332					; 0000014cH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74744
	int	3
$L74744:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74726

; 333  : 	return FALSE;

	xor	eax, eax
$L74717:

; 334  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
$L75110:
	DD	$L74729
	DD	$L74731
	DD	$L74733
	DD	$L74735
	DD	$L74737
	DD	$L74726
	DD	$L74739
?IsCommonDialog@CWinClasses@@SAHPAUHWND__@@W4WCLS_COMMONDIALOG@@@Z ENDP ; CWinClasses::IsCommonDialog
_TEXT	ENDS
PUBLIC	?GetButtonType@CWinClasses@@SAHPAUHWND__@@@Z	; CWinClasses::GetButtonType
;	COMDAT data
; File D:\_CODE\Shared\WinClasses.cpp
data	SEGMENT
$SG74749 DB	'B', 00H, 'u', 00H, 't', 00H, 't', 00H, 'o', 00H, 'n', 00H
	DB	00H, 00H
data	ENDS
;	COMDAT ?GetButtonType@CWinClasses@@SAHPAUHWND__@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
_BTN_TYPEMASK$ = -4
?GetButtonType@CWinClasses@@SAHPAUHWND__@@@Z PROC NEAR	; CWinClasses::GetButtonType, COMDAT

; 337  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 338  : 	if (!IsClass(hWnd, WC_BUTTON))

	push	OFFSET FLAT:$SG74749
	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	call	?IsClass@CWinClasses@@SAHPAUHWND__@@PBG@Z ; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	jne	SHORT $L74748

; 339  : 		return -1;

	or	eax, -1
	jmp	SHORT $L74747
$L74748:

; 340  : 
; 341  : 	UINT BTN_TYPEMASK = 0xf;

	mov	DWORD PTR _BTN_TYPEMASK$[ebp], 15	; 0000000fH

; 342  : 	return (::GetWindowLong(hWnd, GWL_STYLE) & BTN_TYPEMASK);

	mov	esi, esp
	push	-16					; fffffff0H
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetWindowLongW@8
	cmp	esi, esp
	call	__chkesp
	and	eax, DWORD PTR _BTN_TYPEMASK$[ebp]
$L74747:

; 343  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetButtonType@CWinClasses@@SAHPAUHWND__@@@Z ENDP	; CWinClasses::GetButtonType
_TEXT	ENDS
END

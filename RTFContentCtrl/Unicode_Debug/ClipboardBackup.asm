	TITLE	D:\_CODE\3rdParty\ClipboardBackup.cpp
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
;	COMDAT ??_C@_05KIFI@with?5?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_09LOPF@?5elements?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_01BJG@?6?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
text$AFX_COL1	SEGMENT PARA USE32 PUBLIC ''
text$AFX_COL1	ENDS
;	COMDAT ?data@CPlex@@QAEPAXXZ
text$AFX_COL1	SEGMENT PARA USE32 PUBLIC ''
text$AFX_COL1	ENDS
;	COMDAT ??2@YAPAXIPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
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
;	COMDAT ??0CClipboardBackup@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CClipboardBackup@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddData@CClipboardBackup@@QAEHPBGI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Backup@CClipboardBackup@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Restore@CClipboardBackup@@QAEHXZ
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
;	COMDAT ??0?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetCount@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddTail@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEPAU__POSITION@@AAUClipboardData@CClipboardBackup@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetHeadPosition@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNext@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEAAUClipboardData@CClipboardBackup@@AAPAU__POSITION@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewNode@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@IAEPAUCNode@1@PAU21@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAUClipboardData@CClipboardBackup@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAUClipboardData@CClipboardBackup@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBUClipboardData@CClipboardBackup@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAUClipboardData@CClipboardBackup@@H@Z
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
;	COMDAT ??_7?$CList@UClipboardData@CClipboardBackup@@AAU12@@@6B@
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
	ORG $+1
_THIS_FILE DB	'D:\_CODE\3rdParty\ClipboardBackup.cpp', 00H
_DATA	ENDS
PUBLIC	??0CClipboardBackup@@QAE@PAUHWND__@@@Z		; CClipboardBackup::CClipboardBackup
PUBLIC	??0?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAE@H@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>
PUBLIC	??1?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAE@XZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::~CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>
EXTRN	__chkesp:NEAR
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	__imp__IsWindow@4:NEAR
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\ClipboardBackup.cpp
xdata$x	SEGMENT
$T74367	DD	019930520H
	DD	01H
	DD	FLAT:$T74371
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74371	DD	0ffffffffH
	DD	FLAT:$L74363
xdata$x	ENDS
;	COMDAT ??0CClipboardBackup@@QAE@PAUHWND__@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
_this$ = -16
__$EHRec$ = -12
??0CClipboardBackup@@QAE@PAUHWND__@@@Z PROC NEAR	; CClipboardBackup::CClipboardBackup, COMDAT

; 20   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74369
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _hWnd$[ebp]
	mov	DWORD PTR [eax+4], ecx
	push	10					; 0000000aH
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??0?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAE@H@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>
	mov	DWORD PTR __$EHRec$[ebp+8], 0
$L73909:

; 21   : 	ASSERT(hWnd && IsWindow(hWnd));

	cmp	DWORD PTR _hWnd$[ebp], 0
	je	SHORT $L73913
	mov	esi, esp
	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L73912
$L73913:
	push	21					; 00000015H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73912
	int	3
$L73912:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73909

; 22   : 
; 23   : 	m_bFilled = FALSE;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], 0

; 24   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74363:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??1?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAE@XZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::~CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>
	ret	0
$L74369:
	mov	eax, OFFSET FLAT:$T74367
	jmp	___CxxFrameHandler
text$x	ENDS
??0CClipboardBackup@@QAE@PAUHWND__@@@Z ENDP		; CClipboardBackup::CClipboardBackup
PUBLIC	??1CClipboardBackup@@QAE@XZ			; CClipboardBackup::~CClipboardBackup
PUBLIC	?RemoveAll@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEXXZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::RemoveAll
PUBLIC	?GetHeadPosition@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QBEPAU__POSITION@@XZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetHeadPosition
PUBLIC	?GetNext@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEAAUClipboardData@CClipboardBackup@@AAPAU__POSITION@@@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetNext
EXTRN	__imp__GlobalFree@4:NEAR
EXTRN	__imp__DeleteMetaFile@4:NEAR
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\ClipboardBackup.cpp
xdata$x	SEGMENT
$T74379	DD	019930520H
	DD	01H
	DD	FLAT:$T74381
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74381	DD	0ffffffffH
	DD	FLAT:$L74377
xdata$x	ENDS
;	COMDAT ??1CClipboardBackup@@QAE@XZ
_TEXT	SEGMENT
_this$ = -24
_pos$73918 = -16
_data$73922 = -20
__$EHRec$ = -12
??1CClipboardBackup@@QAE@XZ PROC NEAR			; CClipboardBackup::~CClipboardBackup, COMDAT

; 27   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74380
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
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 28   : 	if (m_bFilled)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax], 0
	je	$L73917

; 30   : 		POSITION pos = m_lstData.GetHeadPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	?GetHeadPosition@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QBEPAU__POSITION@@XZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetHeadPosition
	mov	DWORD PTR _pos$73918[ebp], eax
$L73920:

; 31   : 		while( pos != NULL)

	cmp	DWORD PTR _pos$73918[ebp], 0
	je	SHORT $L73921

; 33   : 			ClipboardData & data = m_lstData.GetNext( pos);

	lea	ecx, DWORD PTR _pos$73918[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	?GetNext@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEAAUClipboardData@CClipboardBackup@@AAPAU__POSITION@@@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetNext
	mov	DWORD PTR _data$73922[ebp], eax

; 36   : 			{

	mov	edx, DWORD PTR _data$73922[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR -28+[ebp], eax
	cmp	DWORD PTR -28+[ebp], 14			; 0000000eH
	je	SHORT $L73927
	cmp	DWORD PTR -28+[ebp], 142		; 0000008eH
	je	SHORT $L73927
	jmp	SHORT $L73929
$L73927:

; 37   : 			case CF_ENHMETAFILE:
; 38   : 			case CF_DSPENHMETAFILE:
; 39   : 				::DeleteMetaFile((HMETAFILE)data.m_hData);

	mov	esi, esp
	mov	ecx, DWORD PTR _data$73922[ebp]
	mov	edx, DWORD PTR [ecx+204]
	push	edx
	call	DWORD PTR __imp__DeleteMetaFile@4
	cmp	esi, esp
	call	__chkesp

; 40   : 				break;

	jmp	SHORT $L73924
$L73929:

; 41   : 
; 42   : 			default:
; 43   : 				::GlobalFree( data.m_hData);

	mov	esi, esp
	mov	eax, DWORD PTR _data$73922[ebp]
	mov	ecx, DWORD PTR [eax+204]
	push	ecx
	call	DWORD PTR __imp__GlobalFree@4
	cmp	esi, esp
	call	__chkesp
$L73924:

; 45   : 		}

	jmp	SHORT $L73920
$L73921:

; 46   : 
; 47   : 		m_lstData.RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	?RemoveAll@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEXXZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::RemoveAll
$L73917:

; 49   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??1?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAE@XZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::~CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>
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
$L74377:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??1?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAE@XZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::~CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>
	ret	0
$L74380:
	mov	eax, OFFSET FLAT:$T74379
	jmp	___CxxFrameHandler
text$x	ENDS
??1CClipboardBackup@@QAE@XZ ENDP			; CClipboardBackup::~CClipboardBackup
PUBLIC	?AddData@CClipboardBackup@@QAEHPBGI@Z		; CClipboardBackup::AddData
PUBLIC	?AddTail@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEPAU__POSITION@@AAUClipboardData@CClipboardBackup@@@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::AddTail
EXTRN	__imp__GlobalAlloc@8:NEAR
EXTRN	__imp__GlobalLock@4:NEAR
EXTRN	__imp__GlobalUnlock@4:NEAR
EXTRN	_memcpy:NEAR
EXTRN	__imp__lstrlenW@4:NEAR
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\ClipboardBackup.cpp
xdata$x	SEGMENT
$T74389	DD	019930520H
	DD	02H
	DD	FLAT:$T74391
	DD	01H
	DD	FLAT:$T74392
	DD	2 DUP(00H)
	ORG $+4
$T74391	DD	0ffffffffH
	DD	00H
	DD	0ffffffffH
	DD	00H
$T74392	DD	00H
	DD	00H
	DD	01H
	DD	01H
	DD	FLAT:$T74393
	ORG $+4
$T74393	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L74387
xdata$x	ENDS
;	COMDAT ?AddData@CClipboardBackup@@QAEHPBGI@Z
_TEXT	SEGMENT
_szData$ = 8
_nFormat$ = 12
_this$ = -240
_data$ = -224
_bResult$ = -228
_nBytes$73940 = -232
_pCopy$73943 = -236
__$EHRec$ = -16
?AddData@CClipboardBackup@@QAEHPBGI@Z PROC NEAR		; CClipboardBackup::AddData, COMDAT

; 52   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74390
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	sub	esp, 224				; 000000e0H
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR __$EHRec$[ebp], esp
	push	ecx
	lea	edi, DWORD PTR [ebp-240]
	mov	ecx, 56					; 00000038H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 53   : 	if (!szData || !*szData || !nFormat)

	cmp	DWORD PTR _szData$[ebp], 0
	je	SHORT $L73936
	mov	eax, DWORD PTR _szData$[ebp]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax]
	test	ecx, ecx
	je	SHORT $L73936
	cmp	DWORD PTR _nFormat$[ebp], 0
	jne	SHORT $L73935
$L73936:

; 54   : 		return FALSE;

	xor	eax, eax
	jmp	$L73934
$L73935:

; 55   : 
; 56   : 	ClipboardData data = { nFormat, 0, NULL };

	mov	edx, DWORD PTR _nFormat$[ebp]
	mov	DWORD PTR _data$[ebp], edx
	mov	WORD PTR _data$[ebp+4], 0
	mov	WORD PTR _data$[ebp+6], 0
	mov	ecx, 50					; 00000032H
	xor	eax, eax
	lea	edi, DWORD PTR _data$[ebp+8]
	rep stosd

; 57   : 	BOOL bResult = FALSE;

	mov	DWORD PTR _bResult$[ebp], 0

; 58   : 	
; 59   : 	try
; 60   : 	{

	mov	DWORD PTR __$EHRec$[ebp+12], 0

; 61   : 		// Allocate a global memory object for the text. 
; 62   : 		size_t nBytes = (lstrlen(szData) * sizeof(TCHAR));

	mov	esi, esp
	mov	eax, DWORD PTR _szData$[ebp]
	push	eax
	call	DWORD PTR __imp__lstrlenW@4
	cmp	esi, esp
	call	__chkesp
	shl	eax, 1
	mov	DWORD PTR _nBytes$73940[ebp], eax

; 63   : 		data.m_hData = ::GlobalAlloc(GMEM_MOVEABLE, nBytes); 

	mov	esi, esp
	mov	ecx, DWORD PTR _nBytes$73940[ebp]
	push	ecx
	push	2
	call	DWORD PTR __imp__GlobalAlloc@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _data$[ebp+204], eax

; 64   : 		
; 65   : 		if (data.m_hData) 

	cmp	DWORD PTR _data$[ebp+204], 0
	je	SHORT $L73942

; 67   : 			// Lock the handle and copy the text to the buffer.
; 68   : 			LPVOID pCopy = ::GlobalLock(data.m_hData);

	mov	esi, esp
	mov	edx, DWORD PTR _data$[ebp+204]
	push	edx
	call	DWORD PTR __imp__GlobalLock@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pCopy$73943[ebp], eax

; 69   : 			CopyMemory(pCopy, szData, nBytes); 

	mov	eax, DWORD PTR _nBytes$73940[ebp]
	push	eax
	mov	ecx, DWORD PTR _szData$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pCopy$73943[ebp]
	push	edx
	call	_memcpy
	add	esp, 12					; 0000000cH

; 70   : 			::GlobalUnlock(data.m_hData); 

	mov	esi, esp
	mov	eax, DWORD PTR _data$[ebp+204]
	push	eax
	call	DWORD PTR __imp__GlobalUnlock@4
	cmp	esi, esp
	call	__chkesp

; 71   : 			
; 72   : 			// Place the handle on the clipboard.
; 73   : 			m_lstData.AddTail(data);

	lea	ecx, DWORD PTR _data$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	?AddTail@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEPAU__POSITION@@AAUClipboardData@CClipboardBackup@@@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::AddTail

; 74   : 			bResult = m_bFilled = TRUE;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], 1
	mov	DWORD PTR _bResult$[ebp], 1
$L73942:

; 77   : 	catch(...)

	jmp	SHORT $L73939
$L74387:

; 78   : 	{
; 79   : 	}

	mov	eax, $L73939
	ret	0
$L73939:

; 80   : 	
; 81   : 	// cleanup
; 82   : 	if (!bResult && data.m_hData)

	mov	DWORD PTR __$EHRec$[ebp+12], -1
	cmp	DWORD PTR _bResult$[ebp], 0
	jne	SHORT $L73944
	cmp	DWORD PTR _data$[ebp+204], 0
	je	SHORT $L73944

; 83   : 		::GlobalFree(data.m_hData);

	mov	esi, esp
	mov	eax, DWORD PTR _data$[ebp+204]
	push	eax
	call	DWORD PTR __imp__GlobalFree@4
	cmp	esi, esp
	call	__chkesp
$L73944:

; 84   : 
; 85   : 	return bResult;

	mov	eax, DWORD PTR _bResult$[ebp]
$L73934:

; 86   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp+4]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 240				; 000000f0H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74390:
	mov	eax, OFFSET FLAT:$T74389
	jmp	___CxxFrameHandler
text$x	ENDS
?AddData@CClipboardBackup@@QAEHPBGI@Z ENDP		; CClipboardBackup::AddData
PUBLIC	?Backup@CClipboardBackup@@QAEHXZ		; CClipboardBackup::Backup
PUBLIC	?GetCount@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QBEHXZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetCount
EXTRN	__imp__GlobalSize@4:NEAR
EXTRN	__imp__CopyMetaFileW@8:NEAR
EXTRN	__imp__OpenClipboard@4:NEAR
EXTRN	__imp__CloseClipboard@0:NEAR
EXTRN	__imp__GetClipboardData@4:NEAR
EXTRN	__imp__EnumClipboardFormats@4:NEAR
EXTRN	__imp__GetClipboardFormatNameW@12:NEAR
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\ClipboardBackup.cpp
xdata$x	SEGMENT
$T74401	DD	019930520H
	DD	02H
	DD	FLAT:$T74403
	DD	01H
	DD	FLAT:$T74404
	DD	2 DUP(00H)
	ORG $+4
$T74403	DD	0ffffffffH
	DD	00H
	DD	0ffffffffH
	DD	00H
$T74404	DD	00H
	DD	00H
	DD	01H
	DD	01H
	DD	FLAT:$T74405
	ORG $+4
$T74405	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L74399
xdata$x	ENDS
;	COMDAT ?Backup@CClipboardBackup@@QAEHXZ
_TEXT	SEGMENT
_this$ = -248
_format$73951 = -20
_data$73955 = -232
_hMem$73961 = -24
_size$73970 = -236
_pMem$73971 = -240
_pNewMem$73972 = -244
__$EHRec$ = -16
?Backup@CClipboardBackup@@QAEHXZ PROC NEAR		; CClipboardBackup::Backup, COMDAT

; 89   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74402
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	sub	esp, 236				; 000000ecH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR __$EHRec$[ebp], esp
	push	ecx
	lea	edi, DWORD PTR [ebp-252]
	mov	ecx, 59					; 0000003bH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 90   : 	if (m_bFilled)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax], 0
	je	SHORT $L73948

; 91   : 		return FALSE;

	xor	eax, eax
	jmp	$L73947
$L73948:

; 92   : 
; 93   : 	if (!::OpenClipboard(NULL))

	mov	esi, esp
	push	0
	call	DWORD PTR __imp__OpenClipboard@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L73949

; 94   : 		return FALSE;

	xor	eax, eax
	jmp	$L73947
$L73949:

; 97   : 	{

	mov	DWORD PTR __$EHRec$[ebp+12], 0

; 98   : 		UINT format = 0;

	mov	DWORD PTR _format$73951[ebp], 0
$L73953:

; 99   : 		while( (format = ::EnumClipboardFormats(format)) != 0)

	mov	esi, esp
	mov	ecx, DWORD PTR _format$73951[ebp]
	push	ecx
	call	DWORD PTR __imp__EnumClipboardFormats@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _format$73951[ebp], eax
	cmp	DWORD PTR _format$73951[ebp], 0
	je	$L73954

; 101  : 			ClipboardData data;
; 102  : 			data.m_nFormat = format;

	mov	edx, DWORD PTR _format$73951[ebp]
	mov	DWORD PTR _data$73955[ebp], edx

; 106  : 				format == CF_DSPMETAFILEPICT || format == CF_DSPBITMAP ||
; 107  : 				( format >= CF_PRIVATEFIRST && format <= CF_PRIVATELAST))

	cmp	DWORD PTR _format$73951[ebp], 2
	je	SHORT $L73957
	cmp	DWORD PTR _format$73951[ebp], 3
	je	SHORT $L73957
	cmp	DWORD PTR _format$73951[ebp], 9
	je	SHORT $L73957
	cmp	DWORD PTR _format$73951[ebp], 128	; 00000080H
	je	SHORT $L73957
	cmp	DWORD PTR _format$73951[ebp], 131	; 00000083H
	je	SHORT $L73957
	cmp	DWORD PTR _format$73951[ebp], 130	; 00000082H
	je	SHORT $L73957
	cmp	DWORD PTR _format$73951[ebp], 512	; 00000200H
	jb	SHORT $L73956
	cmp	DWORD PTR _format$73951[ebp], 767	; 000002ffH
	ja	SHORT $L73956
$L73957:

; 109  : 				continue;

	jmp	SHORT $L73953
$L73956:

; 111  : 
; 112  : 			// get format name
; 113  : 			if (format <= 14)

	cmp	DWORD PTR _format$73951[ebp], 14	; 0000000eH
	ja	SHORT $L73958

; 115  : 				data.m_szFormatName[0] = 0;

	mov	WORD PTR _data$73955[ebp+4], 0

; 117  : 			else if (GetClipboardFormatName(format, data.m_szFormatName, 100) == 0)

	jmp	SHORT $L73960
$L73958:
	mov	esi, esp
	push	100					; 00000064H
	lea	eax, DWORD PTR _data$73955[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _format$73951[ebp]
	push	ecx
	call	DWORD PTR __imp__GetClipboardFormatNameW@12
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L73960

; 119  : 				data.m_szFormatName[0] = 0;

	mov	WORD PTR _data$73955[ebp+4], 0
$L73960:

; 121  : 
; 122  : 			// get handle
; 123  : 			HANDLE hMem = ::GetClipboardData( format);

	mov	esi, esp
	mov	edx, DWORD PTR _format$73951[ebp]
	push	edx
	call	DWORD PTR __imp__GetClipboardData@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hMem$73961[ebp], eax

; 124  : 			if (hMem == NULL)

	cmp	DWORD PTR _hMem$73961[ebp], 0
	jne	SHORT $L73962

; 125  : 				continue;

	jmp	$L73953
$L73962:

; 129  : 			{

	mov	eax, DWORD PTR _format$73951[ebp]
	mov	DWORD PTR -252+[ebp], eax
	cmp	DWORD PTR -252+[ebp], 14		; 0000000eH
	je	SHORT $L73967
	cmp	DWORD PTR -252+[ebp], 142		; 0000008eH
	je	SHORT $L73967
	jmp	SHORT $L73969
$L73967:

; 130  : 			case CF_ENHMETAFILE:
; 131  : 			case CF_DSPENHMETAFILE:
; 132  : 				data.m_hData = ::CopyMetaFile((HMETAFILE)hMem, NULL);

	mov	esi, esp
	push	0
	mov	ecx, DWORD PTR _hMem$73961[ebp]
	push	ecx
	call	DWORD PTR __imp__CopyMetaFileW@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _data$73955[ebp+204], eax

; 133  : 				break;

	jmp	$L73964
$L73969:

; 137  : 					int    size = ::GlobalSize(hMem);

	mov	esi, esp
	mov	edx, DWORD PTR _hMem$73961[ebp]
	push	edx
	call	DWORD PTR __imp__GlobalSize@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _size$73970[ebp], eax

; 138  : 					LPVOID pMem = ::GlobalLock( hMem);

	mov	esi, esp
	mov	eax, DWORD PTR _hMem$73961[ebp]
	push	eax
	call	DWORD PTR __imp__GlobalLock@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pMem$73971[ebp], eax

; 139  : 
; 140  : 					data.m_hData   = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, size);

	mov	esi, esp
	mov	ecx, DWORD PTR _size$73970[ebp]
	push	ecx
	push	8194					; 00002002H
	call	DWORD PTR __imp__GlobalAlloc@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _data$73955[ebp+204], eax

; 141  : 					LPVOID pNewMem = ::GlobalLock( data.m_hData);

	mov	esi, esp
	mov	edx, DWORD PTR _data$73955[ebp+204]
	push	edx
	call	DWORD PTR __imp__GlobalLock@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pNewMem$73972[ebp], eax

; 142  : 
; 143  : 					memcpy(pNewMem, pMem, size);

	mov	eax, DWORD PTR _size$73970[ebp]
	push	eax
	mov	ecx, DWORD PTR _pMem$73971[ebp]
	push	ecx
	mov	edx, DWORD PTR _pNewMem$73972[ebp]
	push	edx
	call	_memcpy
	add	esp, 12					; 0000000cH

; 144  : 
; 145  : 					::GlobalUnlock(hMem);

	mov	esi, esp
	mov	eax, DWORD PTR _hMem$73961[ebp]
	push	eax
	call	DWORD PTR __imp__GlobalUnlock@4
	cmp	esi, esp
	call	__chkesp

; 146  : 					::GlobalUnlock(data.m_hData);

	mov	esi, esp
	mov	ecx, DWORD PTR _data$73955[ebp+204]
	push	ecx
	call	DWORD PTR __imp__GlobalUnlock@4
	cmp	esi, esp
	call	__chkesp
$L73964:

; 149  : 
; 150  : 			m_lstData.AddTail(data);

	lea	edx, DWORD PTR _data$73955[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	?AddTail@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEPAU__POSITION@@AAUClipboardData@CClipboardBackup@@@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::AddTail

; 151  : 		}

	jmp	$L73953
$L73954:

; 153  : 	catch (...)

	jmp	SHORT $L73950
$L74399:

; 154  : 	{
; 155  : 		// do nothing
; 156  : 	}

	mov	eax, $L73950
	ret	0
$L73950:

; 157  : 
; 158  : 	::CloseClipboard(); // always

	mov	DWORD PTR __$EHRec$[ebp+12], -1
	mov	esi, esp
	call	DWORD PTR __imp__CloseClipboard@0
	cmp	esi, esp
	call	__chkesp

; 159  : 
; 160  : 	m_bFilled = (m_lstData.GetCount() > 0);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	?GetCount@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QBEHXZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetCount
	xor	ecx, ecx
	test	eax, eax
	setg	cl
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], ecx

; 161  : 
; 162  : 	return TRUE;

	mov	eax, 1
$L73947:

; 163  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp+4]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 252				; 000000fcH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74402:
	mov	eax, OFFSET FLAT:$T74401
	jmp	___CxxFrameHandler
text$x	ENDS
?Backup@CClipboardBackup@@QAEHXZ ENDP			; CClipboardBackup::Backup
PUBLIC	?Restore@CClipboardBackup@@QAEHXZ		; CClipboardBackup::Restore
EXTRN	__imp__SetClipboardData@8:NEAR
EXTRN	__imp__RegisterClipboardFormatW@4:NEAR
EXTRN	__imp__EmptyClipboard@0:NEAR
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\ClipboardBackup.cpp
xdata$x	SEGMENT
$T74413	DD	019930520H
	DD	02H
	DD	FLAT:$T74415
	DD	01H
	DD	FLAT:$T74416
	DD	2 DUP(00H)
	ORG $+4
$T74415	DD	0ffffffffH
	DD	00H
	DD	0ffffffffH
	DD	00H
$T74416	DD	00H
	DD	00H
	DD	01H
	DD	01H
	DD	FLAT:$T74417
	ORG $+4
$T74417	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L74411
xdata$x	ENDS
;	COMDAT ?Restore@CClipboardBackup@@QAEHXZ
_TEXT	SEGMENT
_this$ = -36
_pos$73979 = -20
_data$73983 = -24
_format$73984 = -28
_u$73986 = -32
__$EHRec$ = -16
?Restore@CClipboardBackup@@QAEHXZ PROC NEAR		; CClipboardBackup::Restore, COMDAT

; 166  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74414
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	sub	esp, 28					; 0000001cH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR __$EHRec$[ebp], esp
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-44], eax
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR _this$[ebp], ecx

; 167  : 	if (!m_bFilled)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax], 0
	jne	SHORT $L73976

; 168  : 		return FALSE;

	xor	eax, eax
	jmp	$L73975
$L73976:

; 169  : 
; 170  : 	if (!::OpenClipboard(m_hWnd))

	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR __imp__OpenClipboard@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L73977

; 171  : 		return FALSE;

	xor	eax, eax
	jmp	$L73975
$L73977:

; 174  : 	{

	mov	DWORD PTR __$EHRec$[ebp+12], 0

; 175  : 		::EmptyClipboard();

	mov	esi, esp
	call	DWORD PTR __imp__EmptyClipboard@0
	cmp	esi, esp
	call	__chkesp

; 176  : 
; 177  : 		POSITION pos = m_lstData.GetHeadPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	?GetHeadPosition@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QBEPAU__POSITION@@XZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetHeadPosition
	mov	DWORD PTR -40+[ebp], eax
	mov	eax, DWORD PTR -40+[ebp]
	mov	DWORD PTR _pos$73979[ebp], eax
$L73981:

; 178  : 		while( pos != NULL)

	cmp	DWORD PTR _pos$73979[ebp], 0
	je	SHORT $L73982

; 180  : 			ClipboardData & data = m_lstData.GetNext( pos);

	lea	ecx, DWORD PTR _pos$73979[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	?GetNext@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEAAUClipboardData@CClipboardBackup@@AAPAU__POSITION@@@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetNext
	mov	DWORD PTR -44+[ebp], eax
	mov	edx, DWORD PTR -44+[ebp]
	mov	DWORD PTR _data$73983[ebp], edx

; 181  : 
; 182  : 			UINT format = data.m_nFormat;

	mov	eax, DWORD PTR _data$73983[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _format$73984[ebp], ecx

; 183  : 
; 184  : 			if (data.m_szFormatName[0] != 0)

	mov	edx, DWORD PTR _data$73983[ebp]
	xor	eax, eax
	mov	ax, WORD PTR [edx+4]
	test	eax, eax
	je	SHORT $L73987

; 186  : 				UINT u = RegisterClipboardFormat( data.m_szFormatName);

	mov	ecx, DWORD PTR _data$73983[ebp]
	add	ecx, 4
	mov	esi, esp
	push	ecx
	call	DWORD PTR __imp__RegisterClipboardFormatW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _u$73986[ebp], eax

; 187  : 				if (u > 0) format = u;

	cmp	DWORD PTR _u$73986[ebp], 0
	jbe	SHORT $L73987
	mov	edx, DWORD PTR _u$73986[ebp]
	mov	DWORD PTR _format$73984[ebp], edx
$L73987:

; 189  : 
; 190  : 			::SetClipboardData( format, data.m_hData);

	mov	esi, esp
	mov	eax, DWORD PTR _data$73983[ebp]
	mov	ecx, DWORD PTR [eax+204]
	push	ecx
	mov	edx, DWORD PTR _format$73984[ebp]
	push	edx
	call	DWORD PTR __imp__SetClipboardData@8
	cmp	esi, esp
	call	__chkesp

; 191  : 		}

	jmp	SHORT $L73981
$L73982:

; 193  : 	catch (...)

	jmp	SHORT $L73978
$L74411:

; 194  : 	{
; 195  : 		// do nothing
; 196  : 	}

	mov	eax, $L73978
	ret	0
$L73978:

; 197  : 
; 198  : 	::CloseClipboard(); // always

	mov	DWORD PTR __$EHRec$[ebp+12], -1
	mov	esi, esp
	call	DWORD PTR __imp__CloseClipboard@0
	cmp	esi, esp
	call	__chkesp

; 199  : 
; 200  : 	m_lstData.RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	?RemoveAll@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEXXZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::RemoveAll

; 201  : 	m_bFilled = FALSE;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], 0

; 202  : 
; 203  : 	return TRUE;

	mov	eax, 1
$L73975:

; 204  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp+4]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 44					; 0000002cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74414:
	mov	eax, OFFSET FLAT:$T74413
	jmp	___CxxFrameHandler
text$x	ENDS
?Restore@CClipboardBackup@@QAEHXZ ENDP			; CClipboardBackup::Restore
PUBLIC	?Serialize@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAEXAAVCArchive@@@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::Serialize
PUBLIC	?Dump@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UBEXAAVCDumpContext@@@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::Dump
PUBLIC	?AssertValid@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UBEXXZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::AssertValid
PUBLIC	??_7?$CList@UClipboardData@CClipboardBackup@@AAU12@@@6B@ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::`vftable'
PUBLIC	??_G?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAEPAXI@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::`scalar deleting destructor'
PUBLIC	??_E?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAEPAXI@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::`vector deleting destructor'
EXTRN	?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ:NEAR ; CObject::GetRuntimeClass
EXTRN	??1CObject@@UAE@XZ:NEAR				; CObject::~CObject
EXTRN	??0CObject@@IAE@XZ:NEAR				; CObject::CObject
;	COMDAT ??_7?$CList@UClipboardData@CClipboardBackup@@AAU12@@@6B@
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxTempl.h
CONST	SEGMENT
??_7?$CList@UClipboardData@CClipboardBackup@@AAU12@@@6B@ DD FLAT:?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::`vftable'
	DD	FLAT:??_E?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAEPAXI@Z
	DD	FLAT:?Serialize@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UBEXXZ
	DD	FLAT:?Dump@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UBEXAAVCDumpContext@@@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74424	DD	019930520H
	DD	01H
	DD	FLAT:$T74426
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74426	DD	0ffffffffH
	DD	FLAT:$L74422
xdata$x	ENDS
;	COMDAT ??0?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAE@H@Z
_TEXT	SEGMENT
_nBlockSize$ = 8
_this$ = -16
__$EHRec$ = -12
??0?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAE@H@Z PROC NEAR ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>, COMDAT

; 728  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74425
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CObject@@IAE@XZ			; CObject::CObject
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CList@UClipboardData@CClipboardBackup@@AAU12@@@6B@ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::`vftable'
$L74076:

; 729  : 	ASSERT(nBlockSize > 0);

	cmp	DWORD PTR _nBlockSize$[ebp], 0
	jg	SHORT $L74079
	push	729					; 000002d9H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74079
	int	3
$L74079:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74076

; 730  : 
; 731  : 	m_nCount = 0;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12], 0

; 732  : 	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+16], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+8], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0

; 733  : 	m_pBlocks = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+20], 0

; 734  : 	m_nBlockSize = nBlockSize;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nBlockSize$[ebp]
	mov	DWORD PTR [ecx+24], edx

; 735  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74422:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L74425:
	mov	eax, OFFSET FLAT:$T74424
	jmp	___CxxFrameHandler
text$x	ENDS
??0?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAE@H@Z ENDP ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>
;	COMDAT ?GetCount@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?GetCount@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QBEHXZ PROC NEAR ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetCount, COMDAT

; 660  : 	{ return m_nCount; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+12]
	mov	esp, ebp
	pop	ebp
	ret	0
?GetCount@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QBEHXZ ENDP ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetCount
_TEXT	ENDS
PUBLIC	?NewNode@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@IAEPAUCNode@1@PAU21@0@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::NewNode
EXTRN	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z:NEAR ; AfxAssertValidObject
;	COMDAT ?AddTail@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEPAU__POSITION@@AAUClipboardData@CClipboardBackup@@@Z
_TEXT	SEGMENT
_newElement$ = 8
_this$ = -8
_pNewNode$ = -4
?AddTail@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEPAU__POSITION@@AAUClipboardData@CClipboardBackup@@@Z PROC NEAR ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::AddTail, COMDAT

; 839  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 840  : 	ASSERT_VALID(this);

	push	840					; 00000348H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 841  : 
; 842  : 	CNode* pNewNode = NewNode(m_pNodeTail, NULL);

	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?NewNode@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@IAEPAUCNode@1@PAU21@0@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::NewNode
	mov	DWORD PTR _pNewNode$[ebp], eax

; 843  : 	pNewNode->data = newElement;

	mov	esi, DWORD PTR _newElement$[ebp]
	mov	edi, DWORD PTR _pNewNode$[ebp]
	add	edi, 8
	mov	ecx, 52					; 00000034H
	rep movsd

; 844  : 	if (m_pNodeTail != NULL)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	je	SHORT $L74088

; 845  : 		m_pNodeTail->pNext = pNewNode;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	eax, DWORD PTR _pNewNode$[ebp]
	mov	DWORD PTR [edx], eax

; 846  : 	else

	jmp	SHORT $L74089
$L74088:

; 847  : 		m_pNodeHead = pNewNode;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _pNewNode$[ebp]
	mov	DWORD PTR [ecx+4], edx
$L74089:

; 848  : 	m_pNodeTail = pNewNode;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _pNewNode$[ebp]
	mov	DWORD PTR [eax+8], ecx

; 849  : 	return (POSITION) pNewNode;

	mov	eax, DWORD PTR _pNewNode$[ebp]

; 850  : }

	pop	edi
	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?AddTail@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEPAU__POSITION@@AAUClipboardData@CClipboardBackup@@@Z ENDP ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::AddTail
_TEXT	ENDS
PUBLIC	?DestructElements@@YGXPAUClipboardData@CClipboardBackup@@H@Z ; DestructElements
EXTRN	?FreeDataChain@CPlex@@QAEXXZ:NEAR		; CPlex::FreeDataChain
;	COMDAT ?RemoveAll@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEXXZ
_TEXT	SEGMENT
_this$ = -8
_pNode$ = -4
?RemoveAll@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEXXZ PROC NEAR ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::RemoveAll, COMDAT

; 739  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 740  : 	ASSERT_VALID(this);

	push	740					; 000002e4H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 741  : 
; 742  : 	// destroy elements
; 743  : 	CNode* pNode;
; 744  : 	for (pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR _pNode$[ebp], edx
	jmp	SHORT $L74095
$L74096:
	mov	eax, DWORD PTR _pNode$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pNode$[ebp], ecx
$L74095:
	cmp	DWORD PTR _pNode$[ebp], 0
	je	SHORT $L74097

; 745  : 		DestructElements<TYPE>(&pNode->data, 1);

	push	1
	mov	edx, DWORD PTR _pNode$[ebp]
	add	edx, 8
	push	edx
	call	?DestructElements@@YGXPAUClipboardData@CClipboardBackup@@H@Z ; DestructElements
	jmp	SHORT $L74096
$L74097:

; 746  : 
; 747  : 	m_nCount = 0;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+12], 0

; 748  : 	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+16], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+8], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+4], 0

; 749  : 	m_pBlocks->FreeDataChain();

	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+20]
	call	?FreeDataChain@CPlex@@QAEXXZ		; CPlex::FreeDataChain

; 750  : 	m_pBlocks = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+20], 0

; 751  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?RemoveAll@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEXXZ ENDP ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::RemoveAll
_TEXT	ENDS
;	COMDAT ?GetHeadPosition@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetHeadPosition@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QBEPAU__POSITION@@XZ PROC NEAR ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetHeadPosition, COMDAT

; 682  : 	{ return (POSITION) m_pNodeHead; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+4]
	mov	esp, ebp
	pop	ebp
	ret	0
?GetHeadPosition@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QBEPAU__POSITION@@XZ ENDP ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetHeadPosition
_TEXT	ENDS
EXTRN	?AfxIsValidAddress@@YGHPBXIH@Z:NEAR		; AfxIsValidAddress
;	COMDAT ?GetNext@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEAAUClipboardData@CClipboardBackup@@AAPAU__POSITION@@@Z
_TEXT	SEGMENT
_rPosition$ = 8
_this$ = -8
_pNode$ = -4
?GetNext@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEAAUClipboardData@CClipboardBackup@@AAPAU__POSITION@@@Z PROC NEAR ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetNext, COMDAT

; 688  : 	{ CNode* pNode = (CNode*) rPosition;

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _rPosition$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pNode$[ebp], ecx
$L74113:

; 689  : 		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));

	push	1
	push	216					; 000000d8H
	mov	edx, DWORD PTR _pNode$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74117
	push	689					; 000002b1H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74117
	int	3
$L74117:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74113

; 690  : 		rPosition = (POSITION) pNode->pNext;

	mov	ecx, DWORD PTR _rPosition$[ebp]
	mov	edx, DWORD PTR _pNode$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR [ecx], eax

; 691  : 		return pNode->data; }

	mov	eax, DWORD PTR _pNode$[ebp]
	add	eax, 8
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetNext@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEAAUClipboardData@CClipboardBackup@@AAPAU__POSITION@@@Z ENDP ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetNext
_TEXT	ENDS
;	COMDAT xdata$x
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxTempl.h
xdata$x	SEGMENT
$T74443	DD	019930520H
	DD	01H
	DD	FLAT:$T74445
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74445	DD	0ffffffffH
	DD	FLAT:$L74441
xdata$x	ENDS
;	COMDAT ??1?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAE@XZ PROC NEAR ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::~CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>, COMDAT

; 755  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74444
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CList@UClipboardData@CClipboardBackup@@AAU12@@@6B@ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 756  : 	RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEXXZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::RemoveAll
$L74122:

; 757  : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L74125
	push	757					; 000002f5H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74125
	int	3
$L74125:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74122

; 758  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74441:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L74444:
	mov	eax, OFFSET FLAT:$T74443
	jmp	___CxxFrameHandler
text$x	ENDS
??1?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAE@XZ ENDP ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::~CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>
PUBLIC	?SerializeElements@@YGXAAVCArchive@@PAUClipboardData@CClipboardBackup@@H@Z ; SerializeElements
EXTRN	?ReadCount@CArchive@@QAEKXZ:NEAR		; CArchive::ReadCount
EXTRN	?WriteCount@CArchive@@QAEXK@Z:NEAR		; CArchive::WriteCount
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	?IsStoring@CArchive@@QBEHXZ:NEAR		; CArchive::IsStoring
;	COMDAT ?Serialize@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT
_ar$ = 8
_this$ = -220
_pNode$74131 = -4
_nNewCount$74147 = -8
_newData$74151 = -216
?Serialize@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAEXAAVCArchive@@@Z PROC NEAR ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::Serialize, COMDAT

; 1043 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 220				; 000000dcH
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-220]
	mov	ecx, 55					; 00000037H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 1044 : 	ASSERT_VALID(this);

	push	1044					; 00000414H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1045 : 
; 1046 : 	CObject::Serialize(ar);

	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Serialize@CObject@@UAEXAAVCArchive@@@Z	; CObject::Serialize

; 1047 : 
; 1048 : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L74130

; 1050 : 		ar.WriteCount(m_nCount);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?WriteCount@CArchive@@QAEXK@Z		; CArchive::WriteCount

; 1051 : 		for (CNode* pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR _pNode$74131[ebp], edx
	jmp	SHORT $L74132
$L74133:
	mov	eax, DWORD PTR _pNode$74131[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pNode$74131[ebp], ecx
$L74132:
	cmp	DWORD PTR _pNode$74131[ebp], 0
	je	SHORT $L74134
$L74135:

; 1053 : 			ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));

	push	1
	push	216					; 000000d8H
	mov	edx, DWORD PTR _pNode$74131[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74139
	push	1053					; 0000041dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74139
	int	3
$L74139:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74135

; 1054 : 			SerializeElements<TYPE>(ar, &pNode->data, 1);

	push	1
	mov	ecx, DWORD PTR _pNode$74131[ebp]
	add	ecx, 8
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAUClipboardData@CClipboardBackup@@H@Z ; SerializeElements

; 1055 : 		}

	jmp	SHORT $L74133
$L74134:

; 1057 : 	else

	jmp	SHORT $L74150
$L74130:

; 1059 : 		DWORD nNewCount = ar.ReadCount();

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?ReadCount@CArchive@@QAEKXZ		; CArchive::ReadCount
	mov	DWORD PTR _nNewCount$74147[ebp], eax
$L74149:

; 1060 : 		while (nNewCount--)

	mov	eax, DWORD PTR _nNewCount$74147[ebp]
	mov	ecx, DWORD PTR _nNewCount$74147[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nNewCount$74147[ebp], ecx
	test	eax, eax
	je	SHORT $L74150

; 1062 : 			TYPE newData;
; 1063 : 			SerializeElements<TYPE>(ar, &newData, 1);

	push	1
	lea	edx, DWORD PTR _newData$74151[ebp]
	push	edx
	mov	eax, DWORD PTR _ar$[ebp]
	push	eax
	call	?SerializeElements@@YGXAAVCArchive@@PAUClipboardData@CClipboardBackup@@H@Z ; SerializeElements

; 1064 : 			AddTail(newData);

	lea	ecx, DWORD PTR _newData$74151[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddTail@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEPAU__POSITION@@AAUClipboardData@CClipboardBackup@@@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::AddTail

; 1065 : 		}

	jmp	SHORT $L74149
$L74150:

; 1067 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 220				; 000000dcH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Serialize@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAEXAAVCArchive@@@Z ENDP ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::Serialize
_TEXT	ENDS
PUBLIC	??_C@_05KIFI@with?5?$AA@			; `string'
PUBLIC	??_C@_09LOPF@?5elements?$AA@			; `string'
PUBLIC	??_C@_01BJG@?6?$AA@				; `string'
PUBLIC	?DumpElements@@YGXAAVCDumpContext@@PBUClipboardData@CClipboardBackup@@H@Z ; DumpElements
EXTRN	?Dump@CObject@@UBEXAAVCDumpContext@@@Z:NEAR	; CObject::Dump
EXTRN	?GetDepth@CDumpContext@@QBEHXZ:NEAR		; CDumpContext::GetDepth
EXTRN	??6CDumpContext@@QAEAAV0@PBD@Z:NEAR		; CDumpContext::operator<<
EXTRN	??6CDumpContext@@QAEAAV0@H@Z:NEAR		; CDumpContext::operator<<
;	COMDAT ??_C@_05KIFI@with?5?$AA@
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxTempl.h
_DATA	SEGMENT
??_C@_05KIFI@with?5?$AA@ DB 'with ', 00H		; `string'
_DATA	ENDS
;	COMDAT ??_C@_09LOPF@?5elements?$AA@
_DATA	SEGMENT
??_C@_09LOPF@?5elements?$AA@ DB ' elements', 00H	; `string'
_DATA	ENDS
;	COMDAT ??_C@_01BJG@?6?$AA@
_DATA	SEGMENT
??_C@_01BJG@?6?$AA@ DB 0aH, 00H				; `string'
_DATA	ENDS
;	COMDAT ?Dump@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT
_dc$ = 8
_this$ = -8
_pos$74164 = -4
?Dump@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UBEXAAVCDumpContext@@@Z PROC NEAR ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::Dump, COMDAT

; 1072 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1073 : 	CObject::Dump(dc);

	mov	eax, DWORD PTR _dc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Dump@CObject@@UBEXAAVCDumpContext@@@Z	; CObject::Dump

; 1074 : 
; 1075 : 	dc << "with " << m_nCount << " elements";

	push	OFFSET FLAT:??_C@_09LOPF@?5elements?$AA@ ; `string'
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	push	edx
	push	OFFSET FLAT:??_C@_05KIFI@with?5?$AA@	; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<
	mov	ecx, eax
	call	??6CDumpContext@@QAEAAV0@H@Z		; CDumpContext::operator<<
	mov	ecx, eax
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1076 : 	if (dc.GetDepth() > 0)

	mov	ecx, DWORD PTR _dc$[ebp]
	call	?GetDepth@CDumpContext@@QBEHXZ		; CDumpContext::GetDepth
	test	eax, eax
	jle	SHORT $L74167

; 1078 : 		POSITION pos = GetHeadPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetHeadPosition@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QBEPAU__POSITION@@XZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetHeadPosition
	mov	DWORD PTR _pos$74164[ebp], eax
$L74166:

; 1079 : 		while (pos != NULL)

	cmp	DWORD PTR _pos$74164[ebp], 0
	je	SHORT $L74167

; 1081 : 			dc << "\n";

	push	OFFSET FLAT:??_C@_01BJG@?6?$AA@		; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1082 : 			DumpElements<TYPE>(dc, &((CList*)this)->GetNext(pos), 1);

	push	1
	lea	eax, DWORD PTR _pos$74164[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetNext@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@QAEAAUClipboardData@CClipboardBackup@@AAPAU__POSITION@@@Z ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::GetNext
	push	eax
	mov	ecx, DWORD PTR _dc$[ebp]
	push	ecx
	call	?DumpElements@@YGXAAVCDumpContext@@PBUClipboardData@CClipboardBackup@@H@Z ; DumpElements

; 1083 : 		}

	jmp	SHORT $L74166
$L74167:

; 1085 : 
; 1086 : 	dc << "\n";

	push	OFFSET FLAT:??_C@_01BJG@?6?$AA@		; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1087 : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Dump@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UBEXAAVCDumpContext@@@Z ENDP ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::Dump
_TEXT	ENDS
EXTRN	?AssertValid@CObject@@UBEXXZ:NEAR		; CObject::AssertValid
;	COMDAT ?AssertValid@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UBEXXZ
_TEXT	SEGMENT
_this$ = -4
?AssertValid@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UBEXXZ PROC NEAR ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::AssertValid, COMDAT

; 1091 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1092 : 	CObject::AssertValid();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?AssertValid@CObject@@UBEXXZ		; CObject::AssertValid

; 1093 : 
; 1094 : 	if (m_nCount == 0)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jne	SHORT $L74179
$L74180:

; 1096 : 		// empty list
; 1097 : 		ASSERT(m_pNodeHead == NULL);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L74183
	push	1097					; 00000449H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74183
	int	3
$L74183:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74180
$L74182:

; 1098 : 		ASSERT(m_pNodeTail == NULL);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	je	SHORT $L74187
	push	1098					; 0000044aH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74187
	int	3
$L74187:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74182

; 1100 : 	else

	jmp	SHORT $L74196
$L74179:

; 1102 : 		// non-empty list
; 1103 : 		ASSERT(AfxIsValidAddress(m_pNodeHead, sizeof(CNode)));

	push	1
	push	216					; 000000d8H
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74193
	push	1103					; 0000044fH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74193
	int	3
$L74193:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74179
$L74191:

; 1104 : 		ASSERT(AfxIsValidAddress(m_pNodeTail, sizeof(CNode)));

	push	1
	push	216					; 000000d8H
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	push	eax
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74198
	push	1104					; 00000450H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74198
	int	3
$L74198:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74191
$L74196:

; 1106 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?AssertValid@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UBEXXZ ENDP ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::AssertValid
_TEXT	ENDS
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_G?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_G?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAEPAXI@Z PROC NEAR ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAE@XZ ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::~CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L74202
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L74202:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_G?$CList@UClipboardData@CClipboardBackup@@AAU12@@@UAEPAXI@Z ENDP ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	?data@CPlex@@QAEPAXXZ				; CPlex::data
PUBLIC	?ConstructElements@@YGXPAUClipboardData@CClipboardBackup@@H@Z ; ConstructElements
EXTRN	?Create@CPlex@@SGPAU1@AAPAU1@II@Z:NEAR		; CPlex::Create
;	COMDAT ?NewNode@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@IAEPAUCNode@1@PAU21@0@Z
_TEXT	SEGMENT
_pPrev$ = 8
_pNext$ = 12
_this$ = -20
_pNewBlock$74209 = -16
_pNode$74211 = -8
_i$74213 = -12
_pNode$ = -4
?NewNode@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@IAEPAUCNode@1@PAU21@0@Z PROC NEAR ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::NewNode, COMDAT

; 778  : {

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
	mov	DWORD PTR _this$[ebp], ecx

; 779  : 	if (m_pNodeFree == NULL)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	jne	SHORT $L74216

; 781  : 		// add another block
; 782  : 		CPlex* pNewBlock = CPlex::Create(m_pBlocks, m_nBlockSize,
; 783  : 				 sizeof(CNode));

	push	216					; 000000d8H
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 20					; 00000014H
	push	eax
	call	?Create@CPlex@@SGPAU1@AAPAU1@II@Z	; CPlex::Create
	mov	DWORD PTR _pNewBlock$74209[ebp], eax

; 784  : 
; 785  : 		// chain them into free list
; 786  : 		CNode* pNode = (CNode*) pNewBlock->data();

	mov	ecx, DWORD PTR _pNewBlock$74209[ebp]
	call	?data@CPlex@@QAEPAXXZ			; CPlex::data
	mov	DWORD PTR _pNode$74211[ebp], eax

; 787  : 		// free in reverse order to make it easier to debug
; 788  : 		pNode += m_nBlockSize - 1;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	imul	edx, 216				; 000000d8H
	mov	eax, DWORD PTR _pNode$74211[ebp]
	add	eax, edx
	mov	DWORD PTR _pNode$74211[ebp], eax

; 789  : 		for (int i = m_nBlockSize-1; i >= 0; i--, pNode--)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	mov	DWORD PTR _i$74213[ebp], edx
	jmp	SHORT $L74214
$L74215:
	mov	eax, DWORD PTR _i$74213[ebp]
	sub	eax, 1
	mov	DWORD PTR _i$74213[ebp], eax
	mov	ecx, DWORD PTR _pNode$74211[ebp]
	sub	ecx, 216				; 000000d8H
	mov	DWORD PTR _pNode$74211[ebp], ecx
$L74214:
	cmp	DWORD PTR _i$74213[ebp], 0
	jl	SHORT $L74216

; 791  : 			pNode->pNext = m_pNodeFree;

	mov	edx, DWORD PTR _pNode$74211[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR [edx], ecx

; 792  : 			m_pNodeFree = pNode;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _pNode$74211[ebp]
	mov	DWORD PTR [edx+16], eax

; 793  : 		}

	jmp	SHORT $L74215
$L74216:

; 795  : 	ASSERT(m_pNodeFree != NULL);  // we must have something

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+16], 0
	jne	SHORT $L74220
	push	795					; 0000031bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74220
	int	3
$L74220:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74216

; 796  : 
; 797  : 	CList::CNode* pNode = m_pNodeFree;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR _pNode$[ebp], ecx

; 798  : 	m_pNodeFree = m_pNodeFree->pNext;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	DWORD PTR [ecx+16], edx

; 799  : 	pNode->pPrev = pPrev;

	mov	eax, DWORD PTR _pNode$[ebp]
	mov	ecx, DWORD PTR _pPrev$[ebp]
	mov	DWORD PTR [eax+4], ecx

; 800  : 	pNode->pNext = pNext;

	mov	edx, DWORD PTR _pNode$[ebp]
	mov	eax, DWORD PTR _pNext$[ebp]
	mov	DWORD PTR [edx], eax

; 801  : 	m_nCount++;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	add	edx, 1
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+12], edx
$L74222:

; 802  : 	ASSERT(m_nCount > 0);  // make sure we don't overflow

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jg	SHORT $L74225
	push	802					; 00000322H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74225
	int	3
$L74225:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74222

; 803  : 
; 804  : 	ConstructElements<TYPE>(&pNode->data, 1);

	push	1
	mov	eax, DWORD PTR _pNode$[ebp]
	add	eax, 8
	push	eax
	call	?ConstructElements@@YGXPAUClipboardData@CClipboardBackup@@H@Z ; ConstructElements

; 805  : 	return pNode;

	mov	eax, DWORD PTR _pNode$[ebp]

; 806  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?NewNode@?$CList@UClipboardData@CClipboardBackup@@AAU12@@@IAEPAUCNode@1@PAU21@0@Z ENDP ; CList<CClipboardBackup::ClipboardData,CClipboardBackup::ClipboardData &>::NewNode
_TEXT	ENDS
;	COMDAT ?data@CPlex@@QAEPAXXZ
text$AFX_COL1	SEGMENT
_this$ = -4
?data@CPlex@@QAEPAXXZ PROC NEAR				; CPlex::data, COMDAT

; 34   : 	void* data() { return this+1; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 4
	mov	esp, ebp
	pop	ebp
	ret	0
?data@CPlex@@QAEPAXXZ ENDP				; CPlex::data
text$AFX_COL1	ENDS
;	COMDAT ?DestructElements@@YGXPAUClipboardData@CClipboardBackup@@H@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
?DestructElements@@YGXPAUClipboardData@CClipboardBackup@@H@Z PROC NEAR ; DestructElements, COMDAT

; 67   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L74232:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L74236
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	imul	eax, 208				; 000000d0H
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74236
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74236
	int	3
$L74236:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74232

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L74237
$L74238:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 208				; 000000d0H
	mov	DWORD PTR _pElements$[ebp], eax
$L74237:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L74239

; 73   : 		pElements->~TYPE();

	jmp	SHORT $L74238
$L74239:

; 74   : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?DestructElements@@YGXPAUClipboardData@CClipboardBackup@@H@Z ENDP ; DestructElements
_TEXT	ENDS
EXTRN	?Read@CArchive@@QAEIPAXI@Z:NEAR			; CArchive::Read
EXTRN	?Write@CArchive@@QAEXPBXI@Z:NEAR		; CArchive::Write
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAUClipboardData@CClipboardBackup@@H@Z
_TEXT	SEGMENT
_ar$ = 8
_pElements$ = 12
_nCount$ = 16
?SerializeElements@@YGXAAVCArchive@@PAUClipboardData@CClipboardBackup@@H@Z PROC NEAR ; SerializeElements, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L74241:

; 92   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L74245
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	imul	eax, 208				; 000000d0H
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74245
	push	93					; 0000005dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74245
	int	3
$L74245:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74241

; 93   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 94   : 
; 95   : 	// default is bit-wise read/write
; 96   : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L74246

; 97   : 		ar.Write((void*)pElements, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	imul	eax, 208				; 000000d0H
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Write@CArchive@@QAEXPBXI@Z		; CArchive::Write

; 98   : 	else

	jmp	SHORT $L74249
$L74246:

; 99   : 		ar.Read((void*)pElements, nCount * sizeof(TYPE));

	mov	edx, DWORD PTR _nCount$[ebp]
	imul	edx, 208				; 000000d0H
	push	edx
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Read@CArchive@@QAEIPAXI@Z		; CArchive::Read
$L74249:

; 100  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	12					; 0000000cH
?SerializeElements@@YGXAAVCArchive@@PAUClipboardData@CClipboardBackup@@H@Z ENDP ; SerializeElements
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBUClipboardData@CClipboardBackup@@H@Z
_TEXT	SEGMENT
_pElements$ = 12
_nCount$ = 16
?DumpElements@@YGXAAVCDumpContext@@PBUClipboardData@CClipboardBackup@@H@Z PROC NEAR ; DumpElements, COMDAT

; 105  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L74253:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L74257
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	imul	eax, 208				; 000000d0H
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74257
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74257
	int	3
$L74257:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74253

; 107  : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE), FALSE));
; 108  : 	&dc; // not used
; 109  : 	pElements;  // not used
; 110  : 	nCount; // not used
; 111  : 
; 112  : 	// default does nothing
; 113  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	12					; 0000000cH
?DumpElements@@YGXAAVCDumpContext@@PBUClipboardData@CClipboardBackup@@H@Z ENDP ; DumpElements
_TEXT	ENDS
PUBLIC	??2@YAPAXIPAX@Z					; operator new
EXTRN	_memset:NEAR
;	COMDAT ?ConstructElements@@YGXPAUClipboardData@CClipboardBackup@@H@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
$T74468 = -4
?ConstructElements@@YGXPAUClipboardData@CClipboardBackup@@H@Z PROC NEAR ; ConstructElements, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L74259:

; 54   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L74263
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	imul	eax, 208				; 000000d0H
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74263
	push	55					; 00000037H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74263
	int	3
$L74263:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74259

; 55   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 56   : 
; 57   : 	// first do bit-wise zero initialization
; 58   : 	memset((void*)pElements, 0, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	imul	eax, 208				; 000000d0H
	push	eax
	push	0
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	_memset
	add	esp, 12					; 0000000cH

; 59   : 
; 60   : 	// then call the constructor(s)
; 61   : 	for (; nCount--; pElements++)

	jmp	SHORT $L74266
$L74267:
	mov	edx, DWORD PTR _pElements$[ebp]
	add	edx, 208				; 000000d0H
	mov	DWORD PTR _pElements$[ebp], edx
$L74266:
	mov	eax, DWORD PTR _nCount$[ebp]
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
	test	eax, eax
	je	SHORT $L74268

; 62   : 		::new((void*)pElements) TYPE;

	mov	edx, DWORD PTR _pElements$[ebp]
	push	edx
	push	208					; 000000d0H
	call	??2@YAPAXIPAX@Z				; operator new
	add	esp, 8
	mov	DWORD PTR $T74468[ebp], eax
	jmp	SHORT $L74267
$L74268:

; 63   : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?ConstructElements@@YGXPAUClipboardData@CClipboardBackup@@H@Z ENDP ; ConstructElements
_TEXT	ENDS
;	COMDAT ??2@YAPAXIPAX@Z
_TEXT	SEGMENT
__P$ = 12
??2@YAPAXIPAX@Z PROC NEAR				; operator new, COMDAT

; 76   :         {return (_P); }

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR __P$[ebp]
	pop	ebp
	ret	0
??2@YAPAXIPAX@Z ENDP					; operator new
_TEXT	ENDS
END

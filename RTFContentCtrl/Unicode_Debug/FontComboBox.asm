	TITLE	D:\_CODE\3rdParty\FontComboBox.cpp
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
;	COMDAT ?PreSubclassWindow@CFontComboBox@@UAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EnumFontProc@@YGHPAUtagLOGFONTW@@PAUtagTEXTMETRICW@@KJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CFontComboBox@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCFontComboBox@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CFontComboBox@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_GetBaseMessageMap@CFontComboBox@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMessageMap@CFontComboBox@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnDropdown@CFontComboBox@@IAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FillCombo@CFontComboBox@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetMaxWidth@CFontComboBox@@QAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMaxWidth@CFontComboBox@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SelectFontName@CFontComboBox@@QAEHABVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DDX@CFontComboBox@@QAEXPAVCDataExchange@@AAVCString@@@Z
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
;	COMDAT ??_7CFontComboBox@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?OnDropdown@CFontComboBox@@IAEXXZ		; CFontComboBox::OnDropdown
PUBLIC	?_messageEntries@CFontComboBox@@0QBUAFX_MSGMAP_ENTRY@@B ; CFontComboBox::_messageEntries
PUBLIC	?messageMap@CFontComboBox@@1UAFX_MSGMAP@@B	; CFontComboBox::messageMap
PUBLIC	?_GetBaseMessageMap@CFontComboBox@@KGPBUAFX_MSGMAP@@XZ ; CFontComboBox::_GetBaseMessageMap
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\3rdParty\FontComboBox.cpp', 00H
_DATA	ENDS
CONST	SEGMENT
?messageMap@CFontComboBox@@1UAFX_MSGMAP@@B DD FLAT:?_GetBaseMessageMap@CFontComboBox@@KGPBUAFX_MSGMAP@@XZ ; CFontComboBox::messageMap
	DD	FLAT:?_messageEntries@CFontComboBox@@0QBUAFX_MSGMAP_ENTRY@@B
?_messageEntries@CFontComboBox@@0QBUAFX_MSGMAP_ENTRY@@B DD 0bd11H ; CFontComboBox::_messageEntries
	DD	07H
	DD	00H
	DD	00H
	DD	0cH
	DD	FLAT:?OnDropdown@CFontComboBox@@IAEXXZ
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
CONST	ENDS
PUBLIC	?SetMaxWidth@CFontComboBox@@QAEXH@Z		; CFontComboBox::SetMaxWidth
PUBLIC	?GetMaxWidth@CFontComboBox@@QBEHXZ		; CFontComboBox::GetMaxWidth
PUBLIC	?EnumFontProc@@YGHPAUtagLOGFONTW@@PAUtagTEXTMETRICW@@KJ@Z ; EnumFontProc
EXTRN	??0CClientDC@@QAE@PAVCWnd@@@Z:NEAR		; CClientDC::CClientDC
EXTRN	??1CClientDC@@UAE@XZ:NEAR			; CClientDC::~CClientDC
EXTRN	?SelectStockObject@CDC@@UAEPAVCGdiObject@@H@Z:NEAR ; CDC::SelectStockObject
EXTRN	?AddString@CComboBox@@QAEHPBG@Z:NEAR		; CComboBox::AddString
EXTRN	__except_list:DWORD
EXTRN	__chkesp:NEAR
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CString@@QAE@PBD@Z:NEAR			; CString::CString
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	??H@YG?AVCString@@ABV0@0@Z:NEAR			; operator+
EXTRN	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z:NEAR ; CDC::GetTextExtent
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT data
; File D:\_CODE\3rdParty\FontComboBox.cpp
data	SEGMENT
$SG73265 DB	'0', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73456	DD	019930520H
	DD	04H
	DD	FLAT:$T73460
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73460	DD	0ffffffffH
	DD	FLAT:$L73449
	DD	00H
	DD	FLAT:$L73450
	DD	01H
	DD	FLAT:$L73451
	DD	02H
	DD	FLAT:$L73452
xdata$x	ENDS
;	COMDAT ?EnumFontProc@@YGHPAUtagLOGFONTW@@PAUtagTEXTMETRICW@@KJ@Z
_TEXT	SEGMENT
_lplf$ = 8
_lpData$ = 20
_caller$ = -16
_dc$ = -36
_sz$ = -44
$T73443 = -48
$T73444 = -52
$T73445 = -56
$T73448 = -60
__$EHRec$ = -12
?EnumFontProc@@YGHPAUtagLOGFONTW@@PAUtagTEXTMETRICW@@KJ@Z PROC NEAR ; EnumFontProc, COMDAT

; 39   : {	

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73457
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 76					; 0000004cH
	push	edi
	lea	edi, DWORD PTR [ebp-88]
	mov	ecx, 19					; 00000013H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 40   : 	CFontComboBox *caller = reinterpret_cast< CFontComboBox* > ( lpData );		

	mov	eax, DWORD PTR _lpData$[ebp]
	mov	DWORD PTR _caller$[ebp], eax

; 41   : 	caller->AddString( lplf->lfFaceName );

	mov	ecx, DWORD PTR _lplf$[ebp]
	add	ecx, 28					; 0000001cH
	push	ecx
	mov	ecx, DWORD PTR _caller$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 42   : 
; 43   : 	CClientDC dc( caller );

	mov	edx, DWORD PTR _caller$[ebp]
	push	edx
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??0CClientDC@@QAE@PAVCWnd@@@Z		; CClientDC::CClientDC
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 44   : 	dc.SelectStockObject( ANSI_VAR_FONT );

	push	12					; 0000000cH
	lea	ecx, DWORD PTR _dc$[ebp]
	call	?SelectStockObject@CDC@@UAEPAVCGdiObject@@H@Z ; CDC::SelectStockObject

; 45   : 
; 46   : 	// Add a "0" for the margin.
; 47   : 	CSize sz = dc.GetTextExtent( CString( lplf->lfFaceName ) + CString( "0" ) );

	push	OFFSET FLAT:$SG73265
	lea	ecx, DWORD PTR $T73443[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	DWORD PTR -64+[ebp], eax
	mov	eax, DWORD PTR -64+[ebp]
	mov	DWORD PTR -68+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -68+[ebp]
	push	ecx
	mov	edx, DWORD PTR _lplf$[ebp]
	add	edx, 28					; 0000001cH
	push	edx
	lea	ecx, DWORD PTR $T73444[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR -72+[ebp], eax
	mov	eax, DWORD PTR -72+[ebp]
	mov	DWORD PTR -76+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR -76+[ebp]
	push	ecx
	lea	edx, DWORD PTR $T73445[ebp]
	push	edx
	call	??H@YG?AVCString@@ABV0@0@Z		; operator+
	mov	DWORD PTR -80+[ebp], eax
	mov	eax, DWORD PTR -80+[ebp]
	mov	DWORD PTR -84+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR -84+[ebp]
	push	ecx
	lea	edx, DWORD PTR _sz$[ebp]
	push	edx
	lea	ecx, DWORD PTR _dc$[ebp]
	call	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z ; CDC::GetTextExtent
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T73445[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T73444[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR $T73443[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 48   : 
; 49   : 	caller->SetMaxWidth( max( caller->GetMaxWidth(), sz.cx ) );

	mov	ecx, DWORD PTR _caller$[ebp]
	call	?GetMaxWidth@CFontComboBox@@QBEHXZ	; CFontComboBox::GetMaxWidth
	cmp	eax, DWORD PTR _sz$[ebp]
	jle	SHORT $L73446
	mov	ecx, DWORD PTR _caller$[ebp]
	call	?GetMaxWidth@CFontComboBox@@QBEHXZ	; CFontComboBox::GetMaxWidth
	mov	DWORD PTR -88+[ebp], eax
	jmp	SHORT $L73447
$L73446:
	mov	eax, DWORD PTR _sz$[ebp]
	mov	DWORD PTR -88+[ebp], eax
$L73447:
	mov	ecx, DWORD PTR -88+[ebp]
	push	ecx
	mov	ecx, DWORD PTR _caller$[ebp]
	call	?SetMaxWidth@CFontComboBox@@QAEXH@Z	; CFontComboBox::SetMaxWidth

; 50   : 
; 51   : 	return TRUE;

	mov	DWORD PTR $T73448[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??1CClientDC@@UAE@XZ			; CClientDC::~CClientDC
	mov	eax, DWORD PTR $T73448[ebp]

; 52   : 
; 53   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 88					; 00000058H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73449:
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??1CClientDC@@UAE@XZ			; CClientDC::~CClientDC
	ret	0
$L73450:
	lea	ecx, DWORD PTR $T73443[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73451:
	lea	ecx, DWORD PTR $T73444[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73452:
	lea	ecx, DWORD PTR $T73445[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73457:
	mov	eax, OFFSET FLAT:$T73456
	jmp	___CxxFrameHandler
text$x	ENDS
?EnumFontProc@@YGHPAUtagLOGFONTW@@PAUtagTEXTMETRICW@@KJ@Z ENDP ; EnumFontProc
PUBLIC	??0CFontComboBox@@QAE@XZ			; CFontComboBox::CFontComboBox
PUBLIC	?PreSubclassWindow@CFontComboBox@@UAEXXZ	; CFontComboBox::PreSubclassWindow
PUBLIC	?GetMessageMap@CFontComboBox@@MBEPBUAFX_MSGMAP@@XZ ; CFontComboBox::GetMessageMap
PUBLIC	??_7CFontComboBox@@6B@				; CFontComboBox::`vftable'
PUBLIC	??_GCFontComboBox@@UAEPAXI@Z			; CFontComboBox::`scalar deleting destructor'
PUBLIC	??_ECFontComboBox@@UAEPAXI@Z			; CFontComboBox::`vector deleting destructor'
EXTRN	?Create@CWnd@@UAEHPBG0KABUtagRECT@@PAV1@IPAUCCreateContext@@@Z:NEAR ; CWnd::Create
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	?DestroyWindow@CWnd@@UAEHXZ:NEAR		; CWnd::DestroyWindow
EXTRN	?PreCreateWindow@CWnd@@UAEHAAUtagCREATESTRUCTW@@@Z:NEAR ; CWnd::PreCreateWindow
EXTRN	?CalcWindowRect@CWnd@@UAEXPAUtagRECT@@I@Z:NEAR	; CWnd::CalcWindowRect
EXTRN	?OnToolHitTest@CWnd@@UBEHVCPoint@@PAUtagTOOLINFOW@@@Z:NEAR ; CWnd::OnToolHitTest
EXTRN	?GetScrollBarCtrl@CWnd@@UBEPAVCScrollBar@@H@Z:NEAR ; CWnd::GetScrollBarCtrl
EXTRN	?GetRuntimeClass@CComboBox@@UBEPAUCRuntimeClass@@XZ:NEAR ; CComboBox::GetRuntimeClass
EXTRN	??0CComboBox@@QAE@XZ:NEAR			; CComboBox::CComboBox
EXTRN	?WinHelpW@CWnd@@UAEXKI@Z:NEAR			; CWnd::WinHelpW
EXTRN	?ContinueModal@CWnd@@UAEHXZ:NEAR		; CWnd::ContinueModal
EXTRN	?EndModalLoop@CWnd@@UAEXH@Z:NEAR		; CWnd::EndModalLoop
EXTRN	?OnCommand@CWnd@@MAEHIJ@Z:NEAR			; CWnd::OnCommand
EXTRN	?OnNotify@CWnd@@MAEHIJPAJ@Z:NEAR		; CWnd::OnNotify
EXTRN	?DrawItem@CComboBox@@UAEXPAUtagDRAWITEMSTRUCT@@@Z:NEAR ; CComboBox::DrawItem
EXTRN	?MeasureItem@CComboBox@@UAEXPAUtagMEASUREITEMSTRUCT@@@Z:NEAR ; CComboBox::MeasureItem
EXTRN	?CompareItem@CComboBox@@UAEHPAUtagCOMPAREITEMSTRUCT@@@Z:NEAR ; CComboBox::CompareItem
EXTRN	?DeleteItem@CComboBox@@UAEXPAUtagDELETEITEMSTRUCT@@@Z:NEAR ; CComboBox::DeleteItem
EXTRN	?OnChildNotify@CComboBox@@MAEHIIJPAJ@Z:NEAR	; CComboBox::OnChildNotify
EXTRN	?OnCmdMsg@CCmdTarget@@UAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z:NEAR ; CCmdTarget::OnCmdMsg
EXTRN	?IsInvokeAllowed@CCmdTarget@@UAEHJ@Z:NEAR	; CCmdTarget::IsInvokeAllowed
EXTRN	?GetDispatchIID@CCmdTarget@@UAEHPAU_GUID@@@Z:NEAR ; CCmdTarget::GetDispatchIID
EXTRN	?GetTypeInfoCount@CCmdTarget@@UAEIXZ:NEAR	; CCmdTarget::GetTypeInfoCount
EXTRN	?GetTypeLibCache@CCmdTarget@@UAEPAVCTypeLibCache@@XZ:NEAR ; CCmdTarget::GetTypeLibCache
EXTRN	?GetTypeLib@CCmdTarget@@UAEJKPAPAUITypeLib@@@Z:NEAR ; CCmdTarget::GetTypeLib
EXTRN	?GetCommandMap@CCmdTarget@@MBEPBUAFX_OLECMDMAP@@XZ:NEAR ; CCmdTarget::GetCommandMap
EXTRN	?GetDispatchMap@CCmdTarget@@MBEPBUAFX_DISPMAP@@XZ:NEAR ; CCmdTarget::GetDispatchMap
EXTRN	?GetConnectionMap@CCmdTarget@@MBEPBUAFX_CONNECTIONMAP@@XZ:NEAR ; CCmdTarget::GetConnectionMap
EXTRN	?GetInterfaceMap@CCmdTarget@@MBEPBUAFX_INTERFACEMAP@@XZ:NEAR ; CCmdTarget::GetInterfaceMap
EXTRN	?GetEventSinkMap@CCmdTarget@@MBEPBUAFX_EVENTSINKMAP@@XZ:NEAR ; CCmdTarget::GetEventSinkMap
EXTRN	?OnCreateAggregates@CCmdTarget@@UAEHXZ:NEAR	; CCmdTarget::OnCreateAggregates
EXTRN	?GetInterfaceHook@CCmdTarget@@UAEPAUIUnknown@@PBX@Z:NEAR ; CCmdTarget::GetInterfaceHook
EXTRN	?GetSuperWndProcAddr@CWnd@@MAEPAP6GJPAUHWND__@@IIJ@ZXZ:NEAR ; CWnd::GetSuperWndProcAddr
EXTRN	?DoDataExchange@CWnd@@MAEXPAVCDataExchange@@@Z:NEAR ; CWnd::DoDataExchange
EXTRN	?BeginModalState@CWnd@@UAEXXZ:NEAR		; CWnd::BeginModalState
EXTRN	?EndModalState@CWnd@@UAEXXZ:NEAR		; CWnd::EndModalState
EXTRN	?PreTranslateMessage@CWnd@@UAEHPAUtagMSG@@@Z:NEAR ; CWnd::PreTranslateMessage
EXTRN	?OnAmbientProperty@CWnd@@UAEHPAVCOleControlSite@@JPAUtagVARIANT@@@Z:NEAR ; CWnd::OnAmbientProperty
EXTRN	?WindowProc@CWnd@@MAEJIIJ@Z:NEAR		; CWnd::WindowProc
EXTRN	?OnWndMsg@CWnd@@MAEHIIJPAJ@Z:NEAR		; CWnd::OnWndMsg
EXTRN	?DefWindowProcW@CWnd@@MAEJIIJ@Z:NEAR		; CWnd::DefWindowProcW
EXTRN	?PostNcDestroy@CWnd@@MAEXXZ:NEAR		; CWnd::PostNcDestroy
EXTRN	?CheckAutoCenter@CWnd@@UAEHXZ:NEAR		; CWnd::CheckAutoCenter
EXTRN	?AssertValid@CWnd@@UBEXXZ:NEAR			; CWnd::AssertValid
EXTRN	?Dump@CWnd@@UBEXAAVCDumpContext@@@Z:NEAR	; CWnd::Dump
EXTRN	?GetExtraConnectionPoints@CCmdTarget@@MAEHPAVCPtrArray@@@Z:NEAR ; CCmdTarget::GetExtraConnectionPoints
EXTRN	?GetConnectionHook@CCmdTarget@@MAEPAUIConnectionPoint@@ABU_GUID@@@Z:NEAR ; CCmdTarget::GetConnectionHook
EXTRN	?IsFrameWnd@CWnd@@UBEHXZ:NEAR			; CWnd::IsFrameWnd
EXTRN	?OnFinalRelease@CWnd@@UAEXXZ:NEAR		; CWnd::OnFinalRelease
EXTRN	?SetOccDialogInfo@CWnd@@MAEHPAU_AFX_OCC_DIALOG_INFO@@@Z:NEAR ; CWnd::SetOccDialogInfo
;	COMDAT ??_7CFontComboBox@@6B@
; File D:\_CODE\3rdParty\FontComboBox.cpp
CONST	SEGMENT
??_7CFontComboBox@@6B@ DD FLAT:?GetRuntimeClass@CComboBox@@UBEPAUCRuntimeClass@@XZ ; CFontComboBox::`vftable'
	DD	FLAT:??_ECFontComboBox@@UAEPAXI@Z
	DD	FLAT:?Serialize@CObject@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@CWnd@@UBEXXZ
	DD	FLAT:?Dump@CWnd@@UBEXAAVCDumpContext@@@Z
	DD	FLAT:?OnCmdMsg@CCmdTarget@@UAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z
	DD	FLAT:?OnFinalRelease@CWnd@@UAEXXZ
	DD	FLAT:?IsInvokeAllowed@CCmdTarget@@UAEHJ@Z
	DD	FLAT:?GetDispatchIID@CCmdTarget@@UAEHPAU_GUID@@@Z
	DD	FLAT:?GetTypeInfoCount@CCmdTarget@@UAEIXZ
	DD	FLAT:?GetTypeLibCache@CCmdTarget@@UAEPAVCTypeLibCache@@XZ
	DD	FLAT:?GetTypeLib@CCmdTarget@@UAEJKPAPAUITypeLib@@@Z
	DD	FLAT:?GetMessageMap@CFontComboBox@@MBEPBUAFX_MSGMAP@@XZ
	DD	FLAT:?GetCommandMap@CCmdTarget@@MBEPBUAFX_OLECMDMAP@@XZ
	DD	FLAT:?GetDispatchMap@CCmdTarget@@MBEPBUAFX_DISPMAP@@XZ
	DD	FLAT:?GetConnectionMap@CCmdTarget@@MBEPBUAFX_CONNECTIONMAP@@XZ
	DD	FLAT:?GetInterfaceMap@CCmdTarget@@MBEPBUAFX_INTERFACEMAP@@XZ
	DD	FLAT:?GetEventSinkMap@CCmdTarget@@MBEPBUAFX_EVENTSINKMAP@@XZ
	DD	FLAT:?OnCreateAggregates@CCmdTarget@@UAEHXZ
	DD	FLAT:?GetInterfaceHook@CCmdTarget@@UAEPAUIUnknown@@PBX@Z
	DD	FLAT:?GetExtraConnectionPoints@CCmdTarget@@MAEHPAVCPtrArray@@@Z
	DD	FLAT:?GetConnectionHook@CCmdTarget@@MAEPAUIConnectionPoint@@ABU_GUID@@@Z
	DD	FLAT:?PreSubclassWindow@CFontComboBox@@UAEXXZ
	DD	FLAT:?Create@CWnd@@UAEHPBG0KABUtagRECT@@PAV1@IPAUCCreateContext@@@Z
	DD	FLAT:?DestroyWindow@CWnd@@UAEHXZ
	DD	FLAT:?PreCreateWindow@CWnd@@UAEHAAUtagCREATESTRUCTW@@@Z
	DD	FLAT:?CalcWindowRect@CWnd@@UAEXPAUtagRECT@@I@Z
	DD	FLAT:?OnToolHitTest@CWnd@@UBEHVCPoint@@PAUtagTOOLINFOW@@@Z
	DD	FLAT:?GetScrollBarCtrl@CWnd@@UBEPAVCScrollBar@@H@Z
	DD	FLAT:?WinHelpW@CWnd@@UAEXKI@Z
	DD	FLAT:?ContinueModal@CWnd@@UAEHXZ
	DD	FLAT:?EndModalLoop@CWnd@@UAEXH@Z
	DD	FLAT:?OnCommand@CWnd@@MAEHIJ@Z
	DD	FLAT:?OnNotify@CWnd@@MAEHIJPAJ@Z
	DD	FLAT:?GetSuperWndProcAddr@CWnd@@MAEPAP6GJPAUHWND__@@IIJ@ZXZ
	DD	FLAT:?DoDataExchange@CWnd@@MAEXPAVCDataExchange@@@Z
	DD	FLAT:?BeginModalState@CWnd@@UAEXXZ
	DD	FLAT:?EndModalState@CWnd@@UAEXXZ
	DD	FLAT:?PreTranslateMessage@CWnd@@UAEHPAUtagMSG@@@Z
	DD	FLAT:?OnAmbientProperty@CWnd@@UAEHPAVCOleControlSite@@JPAUtagVARIANT@@@Z
	DD	FLAT:?WindowProc@CWnd@@MAEJIIJ@Z
	DD	FLAT:?OnWndMsg@CWnd@@MAEHIIJPAJ@Z
	DD	FLAT:?DefWindowProcW@CWnd@@MAEJIIJ@Z
	DD	FLAT:?PostNcDestroy@CWnd@@MAEXXZ
	DD	FLAT:?OnChildNotify@CComboBox@@MAEHIIJPAJ@Z
	DD	FLAT:?CheckAutoCenter@CWnd@@UAEHXZ
	DD	FLAT:?IsFrameWnd@CWnd@@UBEHXZ
	DD	FLAT:?SetOccDialogInfo@CWnd@@MAEHPAU_AFX_OCC_DIALOG_INFO@@@Z
	DD	FLAT:?DrawItem@CComboBox@@UAEXPAUtagDRAWITEMSTRUCT@@@Z
	DD	FLAT:?MeasureItem@CComboBox@@UAEXPAUtagMEASUREITEMSTRUCT@@@Z
	DD	FLAT:?CompareItem@CComboBox@@UAEHPAUtagCOMPAREITEMSTRUCT@@@Z
	DD	FLAT:?DeleteItem@CComboBox@@UAEXPAUtagDELETEITEMSTRUCT@@@Z
CONST	ENDS
;	COMDAT ??0CFontComboBox@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0CFontComboBox@@QAE@XZ PROC NEAR			; CFontComboBox::CFontComboBox, COMDAT

; 70   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CComboBox@@QAE@XZ			; CComboBox::CComboBox
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CFontComboBox@@6B@ ; CFontComboBox::`vftable'

; 71   : 	m_maxWidth = 0;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+64], 0

; 72   : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CFontComboBox@@QAE@XZ ENDP				; CFontComboBox::CFontComboBox
_TEXT	ENDS
PUBLIC	?FillCombo@CFontComboBox@@QAEXXZ		; CFontComboBox::FillCombo
;	COMDAT ?PreSubclassWindow@CFontComboBox@@UAEXXZ
_TEXT	SEGMENT
_this$ = -4
?PreSubclassWindow@CFontComboBox@@UAEXXZ PROC NEAR	; CFontComboBox::PreSubclassWindow, COMDAT

; 34   : 		virtual void PreSubclassWindow() { FillCombo(); }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FillCombo@CFontComboBox@@QAEXXZ	; CFontComboBox::FillCombo
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?PreSubclassWindow@CFontComboBox@@UAEXXZ ENDP		; CFontComboBox::PreSubclassWindow
_TEXT	ENDS
PUBLIC	??1CFontComboBox@@UAE@XZ			; CFontComboBox::~CFontComboBox
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_GCFontComboBox@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCFontComboBox@@UAEPAXI@Z PROC NEAR			; CFontComboBox::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CFontComboBox@@UAE@XZ		; CFontComboBox::~CFontComboBox
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73273
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L73273:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCFontComboBox@@UAEPAXI@Z ENDP			; CFontComboBox::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	??1CComboBox@@UAE@XZ:NEAR			; CComboBox::~CComboBox
;	COMDAT ??1CFontComboBox@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CFontComboBox@@UAE@XZ PROC NEAR			; CFontComboBox::~CFontComboBox, COMDAT

; 86   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CFontComboBox@@6B@ ; CFontComboBox::`vftable'

; 87   : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CComboBox@@UAE@XZ			; CComboBox::~CComboBox
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CFontComboBox@@UAE@XZ ENDP				; CFontComboBox::~CFontComboBox
_TEXT	ENDS
EXTRN	__imp_?messageMap@CWnd@@1UAFX_MSGMAP@@B:QWORD
;	COMDAT ?_GetBaseMessageMap@CFontComboBox@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
?_GetBaseMessageMap@CFontComboBox@@KGPBUAFX_MSGMAP@@XZ PROC NEAR ; CFontComboBox::_GetBaseMessageMap, COMDAT

; 89   : BEGIN_MESSAGE_MAP(CFontComboBox, CComboBox)

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR __imp_?messageMap@CWnd@@1UAFX_MSGMAP@@B
	pop	ebp
	ret	0
?_GetBaseMessageMap@CFontComboBox@@KGPBUAFX_MSGMAP@@XZ ENDP ; CFontComboBox::_GetBaseMessageMap
_TEXT	ENDS
;	COMDAT ?GetMessageMap@CFontComboBox@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetMessageMap@CFontComboBox@@MBEPBUAFX_MSGMAP@@XZ PROC NEAR ; CFontComboBox::GetMessageMap, COMDAT

; 89   : BEGIN_MESSAGE_MAP(CFontComboBox, CComboBox)

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, OFFSET FLAT:?messageMap@CFontComboBox@@1UAFX_MSGMAP@@B ; CFontComboBox::messageMap
	mov	esp, ebp
	pop	ebp
	ret	0
?GetMessageMap@CFontComboBox@@MBEPBUAFX_MSGMAP@@XZ ENDP	; CFontComboBox::GetMessageMap
_TEXT	ENDS
EXTRN	__imp__GetSystemMetrics@4:NEAR
EXTRN	?SetDroppedWidth@CComboBox@@QAEHI@Z:NEAR	; CComboBox::SetDroppedWidth
;	COMDAT ?OnDropdown@CFontComboBox@@IAEXXZ
_TEXT	SEGMENT
_this$ = -8
_scroll$ = -4
?OnDropdown@CFontComboBox@@IAEXXZ PROC NEAR		; CFontComboBox::OnDropdown, COMDAT

; 112  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 113  : 
; 114  :    int scroll = ::GetSystemMetrics(SM_CXVSCROLL);

	mov	esi, esp
	push	2
	call	DWORD PTR __imp__GetSystemMetrics@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _scroll$[ebp], eax

; 115  :    SetDroppedWidth( m_maxWidth + scroll );

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+64]
	add	ecx, DWORD PTR _scroll$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetDroppedWidth@CComboBox@@QAEHI@Z	; CComboBox::SetDroppedWidth

; 116  : 
; 117  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?OnDropdown@CFontComboBox@@IAEXXZ ENDP			; CFontComboBox::OnDropdown
_TEXT	ENDS
EXTRN	??BCDC@@QBEPAUHDC__@@XZ:NEAR			; CDC::operator HDC__ *
EXTRN	?EnableTranslation@CLocalizer@@SAXABVCWnd@@H@Z:NEAR ; CLocalizer::EnableTranslation
EXTRN	__imp__EnumFontsW@16:NEAR
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\FontComboBox.cpp
xdata$x	SEGMENT
$T73485	DD	019930520H
	DD	01H
	DD	FLAT:$T73487
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73487	DD	0ffffffffH
	DD	FLAT:$L73483
xdata$x	ENDS
;	COMDAT ?FillCombo@CFontComboBox@@QAEXXZ
_TEXT	SEGMENT
_this$ = -36
_dc$ = -32
__$EHRec$ = -12
?FillCombo@CFontComboBox@@QAEXXZ PROC NEAR		; CFontComboBox::FillCombo, COMDAT

; 136  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73486
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 137  : 	CLocalizer::EnableTranslation(*this, FALSE);

	push	0
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?EnableTranslation@CLocalizer@@SAXABVCWnd@@H@Z ; CLocalizer::EnableTranslation
	add	esp, 8

; 138  : 	
; 139  : 	CClientDC dc( this );		

	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??0CClientDC@@QAE@PAVCWnd@@@Z		; CClientDC::CClientDC
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 140  : 	::EnumFonts( dc, 0, ( FONTENUMPROC ) EnumFontProc, ( LPARAM ) this );

	mov	esi, esp
	mov	edx, DWORD PTR _this$[ebp]
	push	edx
	push	OFFSET FLAT:?EnumFontProc@@YGHPAUtagLOGFONTW@@PAUtagTEXTMETRICW@@KJ@Z ; EnumFontProc
	push	0
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??BCDC@@QBEPAUHDC__@@XZ			; CDC::operator HDC__ *
	push	eax
	call	DWORD PTR __imp__EnumFontsW@16
	cmp	esi, esp
	call	__chkesp

; 141  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??1CClientDC@@UAE@XZ			; CClientDC::~CClientDC
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73483:
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??1CClientDC@@UAE@XZ			; CClientDC::~CClientDC
	ret	0
$L73486:
	mov	eax, OFFSET FLAT:$T73485
	jmp	___CxxFrameHandler
text$x	ENDS
?FillCombo@CFontComboBox@@QAEXXZ ENDP			; CFontComboBox::FillCombo
;	COMDAT ?SetMaxWidth@CFontComboBox@@QAEXH@Z
_TEXT	SEGMENT
_maxWidth$ = 8
_this$ = -4
?SetMaxWidth@CFontComboBox@@QAEXH@Z PROC NEAR		; CFontComboBox::SetMaxWidth, COMDAT

; 159  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 160  : 
; 161  : 	m_maxWidth = maxWidth;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _maxWidth$[ebp]
	mov	DWORD PTR [eax+64], ecx

; 162  : 
; 163  : }

	mov	esp, ebp
	pop	ebp
	ret	4
?SetMaxWidth@CFontComboBox@@QAEXH@Z ENDP		; CFontComboBox::SetMaxWidth
_TEXT	ENDS
;	COMDAT ?GetMaxWidth@CFontComboBox@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?GetMaxWidth@CFontComboBox@@QBEHXZ PROC NEAR		; CFontComboBox::GetMaxWidth, COMDAT

; 180  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 181  : 
; 182  : 	return m_maxWidth;

	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+64]

; 183  : 
; 184  : }

	mov	esp, ebp
	pop	ebp
	ret	0
?GetMaxWidth@CFontComboBox@@QBEHXZ ENDP			; CFontComboBox::GetMaxWidth
_TEXT	ENDS
PUBLIC	?SelectFontName@CFontComboBox@@QAEHABVCString@@@Z ; CFontComboBox::SelectFontName
EXTRN	?GetCurSel@CComboBox@@QBEHXZ:NEAR		; CComboBox::GetCurSel
EXTRN	?SetCurSel@CComboBox@@QAEHH@Z:NEAR		; CComboBox::SetCurSel
EXTRN	?FindStringExact@CComboBox@@QBEHHPBG@Z:NEAR	; CComboBox::FindStringExact
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
;	COMDAT ?SelectFontName@CFontComboBox@@QAEHABVCString@@@Z
_TEXT	SEGMENT
_font$ = 8
_this$ = -8
_nFind$ = -4
?SelectFontName@CFontComboBox@@QAEHABVCString@@@Z PROC NEAR ; CFontComboBox::SelectFontName, COMDAT

; 201  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 202  : 	int nFind = FindStringExact(-1, font);

	mov	ecx, DWORD PTR _font$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	-1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FindStringExact@CComboBox@@QBEHHPBG@Z	; CComboBox::FindStringExact
	mov	DWORD PTR _nFind$[ebp], eax

; 203  : 
; 204  : 	if (nFind != GetCurSel())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCurSel@CComboBox@@QBEHXZ		; CComboBox::GetCurSel
	cmp	DWORD PTR _nFind$[ebp], eax
	je	SHORT $L73307

; 205  : 		SetCurSel(nFind);

	mov	eax, DWORD PTR _nFind$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetCurSel@CComboBox@@QAEHH@Z		; CComboBox::SetCurSel
$L73307:

; 206  : 
; 207  : 	return (nFind != -1);

	xor	eax, eax
	cmp	DWORD PTR _nFind$[ebp], -1
	setne	al

; 208  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?SelectFontName@CFontComboBox@@QAEHABVCString@@@Z ENDP	; CFontComboBox::SelectFontName
_TEXT	ENDS
PUBLIC	?DDX@CFontComboBox@@QAEXPAVCDataExchange@@AAVCString@@@Z ; CFontComboBox::DDX
EXTRN	?GetLBText@CComboBox@@QBEXHAAVCString@@@Z:NEAR	; CComboBox::GetLBText
EXTRN	?SelectString@CComboBox@@QAEHHPBG@Z:NEAR	; CComboBox::SelectString
EXTRN	?Empty@CString@@QAEXXZ:NEAR			; CString::Empty
;	COMDAT ?DDX@CFontComboBox@@QAEXPAVCDataExchange@@AAVCString@@@Z
_TEXT	SEGMENT
_pDX$ = 8
_font$ = 12
_this$ = -8
_nSel$73314 = -4
?DDX@CFontComboBox@@QAEXPAVCDataExchange@@AAVCString@@@Z PROC NEAR ; CFontComboBox::DDX, COMDAT

; 211  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 212  : 	if (pDX->m_bSaveAndValidate)

	mov	eax, DWORD PTR _pDX$[ebp]
	cmp	DWORD PTR [eax], 0
	je	SHORT $L73313

; 214  : 		int nSel = GetCurSel();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCurSel@CComboBox@@QBEHXZ		; CComboBox::GetCurSel
	mov	DWORD PTR _nSel$73314[ebp], eax

; 215  : 
; 216  : 		if (nSel == -1)

	cmp	DWORD PTR _nSel$73314[ebp], -1
	jne	SHORT $L73315

; 217  : 			font.Empty();

	mov	ecx, DWORD PTR _font$[ebp]
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 218  : 		else

	jmp	SHORT $L73316
$L73315:

; 219  : 			GetLBText(nSel, font);

	mov	ecx, DWORD PTR _font$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nSel$73314[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetLBText@CComboBox@@QBEXHAAVCString@@@Z ; CComboBox::GetLBText
$L73316:

; 221  : 	else

	jmp	SHORT $L73317
$L73313:

; 222  : 		SelectString(-1, font);

	mov	ecx, DWORD PTR _font$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	-1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SelectString@CComboBox@@QAEHHPBG@Z	; CComboBox::SelectString
$L73317:

; 223  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?DDX@CFontComboBox@@QAEXPAVCDataExchange@@AAVCString@@@Z ENDP ; CFontComboBox::DDX
_TEXT	ENDS
END

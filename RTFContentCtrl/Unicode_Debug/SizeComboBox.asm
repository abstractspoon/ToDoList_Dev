	TITLE	D:\_CODE\3rdParty\SizeComboBox.cpp
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
;	COMDAT ??0CSizeComboBox@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCSizeComboBox@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CSizeComboBox@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_GetBaseMessageMap@CSizeComboBox@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMessageMap@CSizeComboBox@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SelectSize@CSizeComboBox@@QAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IncrementSize@CSizeComboBox@@QAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FillCombo@CSizeComboBox@@QAEXXZ
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
;	COMDAT ??_7CSizeComboBox@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?_messageEntries@CSizeComboBox@@0QBUAFX_MSGMAP_ENTRY@@B ; CSizeComboBox::_messageEntries
PUBLIC	?messageMap@CSizeComboBox@@1UAFX_MSGMAP@@B	; CSizeComboBox::messageMap
PUBLIC	?_GetBaseMessageMap@CSizeComboBox@@KGPBUAFX_MSGMAP@@XZ ; CSizeComboBox::_GetBaseMessageMap
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\3rdParty\SizeComboBox.cpp', 00H
_DATA	ENDS
CONST	SEGMENT
?messageMap@CSizeComboBox@@1UAFX_MSGMAP@@B DD FLAT:?_GetBaseMessageMap@CSizeComboBox@@KGPBUAFX_MSGMAP@@XZ ; CSizeComboBox::messageMap
	DD	FLAT:?_messageEntries@CSizeComboBox@@0QBUAFX_MSGMAP_ENTRY@@B
?_messageEntries@CSizeComboBox@@0QBUAFX_MSGMAP_ENTRY@@B DD 00H ; CSizeComboBox::_messageEntries
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
CONST	ENDS
PUBLIC	??0CSizeComboBox@@QAE@XZ			; CSizeComboBox::CSizeComboBox
PUBLIC	?GetMessageMap@CSizeComboBox@@MBEPBUAFX_MSGMAP@@XZ ; CSizeComboBox::GetMessageMap
PUBLIC	??_7CSizeComboBox@@6B@				; CSizeComboBox::`vftable'
PUBLIC	??_GCSizeComboBox@@UAEPAXI@Z			; CSizeComboBox::`scalar deleting destructor'
PUBLIC	??_ECSizeComboBox@@UAEPAXI@Z			; CSizeComboBox::`vector deleting destructor'
EXTRN	?PreSubclassWindow@CWnd@@UAEXXZ:NEAR		; CWnd::PreSubclassWindow
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
EXTRN	__chkesp:NEAR
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
;	COMDAT ??_7CSizeComboBox@@6B@
; File D:\_CODE\3rdParty\SizeComboBox.cpp
CONST	SEGMENT
??_7CSizeComboBox@@6B@ DD FLAT:?GetRuntimeClass@CComboBox@@UBEPAUCRuntimeClass@@XZ ; CSizeComboBox::`vftable'
	DD	FLAT:??_ECSizeComboBox@@UAEPAXI@Z
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
	DD	FLAT:?GetMessageMap@CSizeComboBox@@MBEPBUAFX_MSGMAP@@XZ
	DD	FLAT:?GetCommandMap@CCmdTarget@@MBEPBUAFX_OLECMDMAP@@XZ
	DD	FLAT:?GetDispatchMap@CCmdTarget@@MBEPBUAFX_DISPMAP@@XZ
	DD	FLAT:?GetConnectionMap@CCmdTarget@@MBEPBUAFX_CONNECTIONMAP@@XZ
	DD	FLAT:?GetInterfaceMap@CCmdTarget@@MBEPBUAFX_INTERFACEMAP@@XZ
	DD	FLAT:?GetEventSinkMap@CCmdTarget@@MBEPBUAFX_EVENTSINKMAP@@XZ
	DD	FLAT:?OnCreateAggregates@CCmdTarget@@UAEHXZ
	DD	FLAT:?GetInterfaceHook@CCmdTarget@@UAEPAUIUnknown@@PBX@Z
	DD	FLAT:?GetExtraConnectionPoints@CCmdTarget@@MAEHPAVCPtrArray@@@Z
	DD	FLAT:?GetConnectionHook@CCmdTarget@@MAEPAUIConnectionPoint@@ABU_GUID@@@Z
	DD	FLAT:?PreSubclassWindow@CWnd@@UAEXXZ
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
;	COMDAT ??0CSizeComboBox@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0CSizeComboBox@@QAE@XZ PROC NEAR			; CSizeComboBox::CSizeComboBox, COMDAT

; 46   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CComboBox@@QAE@XZ			; CComboBox::CComboBox
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CSizeComboBox@@6B@ ; CSizeComboBox::`vftable'

; 47   : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CSizeComboBox@@QAE@XZ ENDP				; CSizeComboBox::CSizeComboBox
_TEXT	ENDS
PUBLIC	??1CSizeComboBox@@UAE@XZ			; CSizeComboBox::~CSizeComboBox
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_GCSizeComboBox@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCSizeComboBox@@UAEPAXI@Z PROC NEAR			; CSizeComboBox::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CSizeComboBox@@UAE@XZ		; CSizeComboBox::~CSizeComboBox
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73081
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L73081:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCSizeComboBox@@UAEPAXI@Z ENDP			; CSizeComboBox::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	??1CComboBox@@UAE@XZ:NEAR			; CComboBox::~CComboBox
;	COMDAT ??1CSizeComboBox@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CSizeComboBox@@UAE@XZ PROC NEAR			; CSizeComboBox::~CSizeComboBox, COMDAT

; 61   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CSizeComboBox@@6B@ ; CSizeComboBox::`vftable'

; 62   : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CComboBox@@UAE@XZ			; CComboBox::~CComboBox
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CSizeComboBox@@UAE@XZ ENDP				; CSizeComboBox::~CSizeComboBox
_TEXT	ENDS
EXTRN	__imp_?messageMap@CWnd@@1UAFX_MSGMAP@@B:QWORD
;	COMDAT ?_GetBaseMessageMap@CSizeComboBox@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
?_GetBaseMessageMap@CSizeComboBox@@KGPBUAFX_MSGMAP@@XZ PROC NEAR ; CSizeComboBox::_GetBaseMessageMap, COMDAT

; 65   : BEGIN_MESSAGE_MAP(CSizeComboBox, CComboBox)

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR __imp_?messageMap@CWnd@@1UAFX_MSGMAP@@B
	pop	ebp
	ret	0
?_GetBaseMessageMap@CSizeComboBox@@KGPBUAFX_MSGMAP@@XZ ENDP ; CSizeComboBox::_GetBaseMessageMap
_TEXT	ENDS
;	COMDAT ?GetMessageMap@CSizeComboBox@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetMessageMap@CSizeComboBox@@MBEPBUAFX_MSGMAP@@XZ PROC NEAR ; CSizeComboBox::GetMessageMap, COMDAT

; 65   : BEGIN_MESSAGE_MAP(CSizeComboBox, CComboBox)

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, OFFSET FLAT:?messageMap@CSizeComboBox@@1UAFX_MSGMAP@@B ; CSizeComboBox::messageMap
	mov	esp, ebp
	pop	ebp
	ret	0
?GetMessageMap@CSizeComboBox@@MBEPBUAFX_MSGMAP@@XZ ENDP	; CSizeComboBox::GetMessageMap
_TEXT	ENDS
PUBLIC	?SelectSize@CSizeComboBox@@QAEHH@Z		; CSizeComboBox::SelectSize
EXTRN	?GetCurSel@CComboBox@@QBEHXZ:NEAR		; CComboBox::GetCurSel
EXTRN	?SetCurSel@CComboBox@@QAEHH@Z:NEAR		; CComboBox::SetCurSel
EXTRN	?FindStringExact@CComboBox@@QBEHHPBG@Z:NEAR	; CComboBox::FindStringExact
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	?Format@CString@@QAAXPBGZZ:NEAR			; CString::Format
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT data
; File D:\_CODE\3rdParty\SizeComboBox.cpp
data	SEGMENT
$SG73096 DB	'%', 00H, 'd', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73258	DD	019930520H
	DD	01H
	DD	FLAT:$T73261
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73261	DD	0ffffffffH
	DD	FLAT:$L73254
xdata$x	ENDS
;	COMDAT ?SelectSize@CSizeComboBox@@QAEHH@Z
_TEXT	SEGMENT
_size$ = 8
_this$ = -28
_sz$ = -20
_nFind$ = -16
$T73253 = -24
__$EHRec$ = -12
?SelectSize@CSizeComboBox@@QAEHH@Z PROC NEAR		; CSizeComboBox::SelectSize, COMDAT

; 88   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73259
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 89   : 	CString sz;

	lea	ecx, DWORD PTR _sz$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 90   : 	sz.Format( _T( "%d" ), size );

	mov	eax, DWORD PTR _size$[ebp]
	push	eax
	push	OFFSET FLAT:$SG73096
	lea	ecx, DWORD PTR _sz$[ebp]
	push	ecx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 12					; 0000000cH

; 91   : 
; 92   : 	int nFind = FindStringExact(-1, sz);

	lea	ecx, DWORD PTR _sz$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	-1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FindStringExact@CComboBox@@QBEHHPBG@Z	; CComboBox::FindStringExact
	mov	DWORD PTR _nFind$[ebp], eax

; 93   : 
; 94   : 	if (nFind != GetCurSel())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCurSel@CComboBox@@QBEHXZ		; CComboBox::GetCurSel
	cmp	DWORD PTR _nFind$[ebp], eax
	je	SHORT $L73098

; 95   : 		SetCurSel(nFind);

	mov	edx, DWORD PTR _nFind$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetCurSel@CComboBox@@QAEHH@Z		; CComboBox::SetCurSel
$L73098:

; 96   : 
; 97   : 	return (nFind != -1);

	xor	eax, eax
	cmp	DWORD PTR _nFind$[ebp], -1
	setne	al
	mov	DWORD PTR $T73253[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sz$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T73253[ebp]

; 98   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73254:
	lea	ecx, DWORD PTR _sz$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73259:
	mov	eax, OFFSET FLAT:$T73258
	jmp	___CxxFrameHandler
text$x	ENDS
?SelectSize@CSizeComboBox@@QAEHH@Z ENDP			; CSizeComboBox::SelectSize
PUBLIC	?IncrementSize@CSizeComboBox@@QAEHH@Z		; CSizeComboBox::IncrementSize
EXTRN	?GetCount@CComboBox@@QBEHXZ:NEAR		; CComboBox::GetCount
;	COMDAT ?IncrementSize@CSizeComboBox@@QAEHH@Z
_TEXT	SEGMENT
_nBy$ = 8
_this$ = -8
_nSel$ = -4
?IncrementSize@CSizeComboBox@@QAEHH@Z PROC NEAR		; CSizeComboBox::IncrementSize, COMDAT

; 101  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 102  : 	int nSel = GetCurSel() + nBy;

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCurSel@CComboBox@@QBEHXZ		; CComboBox::GetCurSel
	add	eax, DWORD PTR _nBy$[ebp]
	mov	DWORD PTR _nSel$[ebp], eax

; 103  : 
; 104  : 	nSel = max(nSel, 0);

	xor	eax, eax
	cmp	DWORD PTR _nSel$[ebp], 0
	setle	al
	dec	eax
	and	eax, DWORD PTR _nSel$[ebp]
	mov	DWORD PTR _nSel$[ebp], eax

; 105  : 	nSel = min(nSel, GetCount() - 1);

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCount@CComboBox@@QBEHXZ		; CComboBox::GetCount
	sub	eax, 1
	cmp	DWORD PTR _nSel$[ebp], eax
	jge	SHORT $L73268
	mov	ecx, DWORD PTR _nSel$[ebp]
	mov	DWORD PTR -12+[ebp], ecx
	jmp	SHORT $L73269
$L73268:
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCount@CComboBox@@QBEHXZ		; CComboBox::GetCount
	sub	eax, 1
	mov	DWORD PTR -12+[ebp], eax
$L73269:
	mov	edx, DWORD PTR -12+[ebp]
	mov	DWORD PTR _nSel$[ebp], edx

; 106  : 
; 107  : 	if (nSel != GetCurSel())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCurSel@CComboBox@@QBEHXZ		; CComboBox::GetCurSel
	cmp	DWORD PTR _nSel$[ebp], eax
	je	SHORT $L73105

; 109  : 		SetCurSel(nSel);

	mov	eax, DWORD PTR _nSel$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetCurSel@CComboBox@@QAEHH@Z		; CComboBox::SetCurSel

; 110  : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L73103
$L73105:

; 112  : 
; 113  : 	return FALSE;

	xor	eax, eax
$L73103:

; 114  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?IncrementSize@CSizeComboBox@@QAEHH@Z ENDP		; CSizeComboBox::IncrementSize
_TEXT	ENDS
PUBLIC	?FillCombo@CSizeComboBox@@QAEXXZ		; CSizeComboBox::FillCombo
EXTRN	?AddString@CComboBox@@QAEHPBG@Z:NEAR		; CComboBox::AddString
;	COMDAT data
; File D:\_CODE\3rdParty\SizeComboBox.cpp
data	SEGMENT
$SG73109 DB	'8', 00H, 00H, 00H
$SG73110 DB	'9', 00H, 00H, 00H
$SG73111 DB	'1', 00H, '0', 00H, 00H, 00H
	ORG $+2
$SG73112 DB	'1', 00H, '1', 00H, 00H, 00H
	ORG $+2
$SG73113 DB	'1', 00H, '2', 00H, 00H, 00H
	ORG $+2
$SG73114 DB	'1', 00H, '4', 00H, 00H, 00H
	ORG $+2
$SG73115 DB	'1', 00H, '6', 00H, 00H, 00H
	ORG $+2
$SG73116 DB	'1', 00H, '8', 00H, 00H, 00H
	ORG $+2
$SG73117 DB	'2', 00H, '0', 00H, 00H, 00H
	ORG $+2
$SG73118 DB	'2', 00H, '2', 00H, 00H, 00H
	ORG $+2
$SG73119 DB	'2', 00H, '4', 00H, 00H, 00H
	ORG $+2
$SG73120 DB	'2', 00H, '6', 00H, 00H, 00H
	ORG $+2
$SG73121 DB	'2', 00H, '8', 00H, 00H, 00H
	ORG $+2
$SG73122 DB	'3', 00H, '6', 00H, 00H, 00H
	ORG $+2
$SG73123 DB	'4', 00H, '8', 00H, 00H, 00H
	ORG $+2
$SG73124 DB	'7', 00H, '2', 00H, 00H, 00H
data	ENDS
;	COMDAT ?FillCombo@CSizeComboBox@@QAEXXZ
_TEXT	SEGMENT
_this$ = -4
?FillCombo@CSizeComboBox@@QAEXXZ PROC NEAR		; CSizeComboBox::FillCombo, COMDAT

; 130  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 131  : 
; 132  : 	AddString( _T( "8" ) );

	push	OFFSET FLAT:$SG73109
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 133  : 	AddString( _T( "9" ) );

	push	OFFSET FLAT:$SG73110
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 134  : 	AddString( _T( "10" ) );

	push	OFFSET FLAT:$SG73111
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 135  : 	AddString( _T( "11" ) );

	push	OFFSET FLAT:$SG73112
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 136  : 	AddString( _T( "12" ) );

	push	OFFSET FLAT:$SG73113
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 137  : 	AddString( _T( "14" ) );

	push	OFFSET FLAT:$SG73114
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 138  : 	AddString( _T( "16" ) );

	push	OFFSET FLAT:$SG73115
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 139  : 	AddString( _T( "18" ) );

	push	OFFSET FLAT:$SG73116
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 140  : 	AddString( _T( "20" ) );

	push	OFFSET FLAT:$SG73117
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 141  : 	AddString( _T( "22" ) );

	push	OFFSET FLAT:$SG73118
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 142  : 	AddString( _T( "24" ) );

	push	OFFSET FLAT:$SG73119
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 143  : 	AddString( _T( "26" ) );

	push	OFFSET FLAT:$SG73120
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 144  : 	AddString( _T( "28" ) );

	push	OFFSET FLAT:$SG73121
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 145  : 	AddString( _T( "36" ) );

	push	OFFSET FLAT:$SG73122
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 146  : 	AddString( _T( "48" ) );

	push	OFFSET FLAT:$SG73123
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 147  : 	AddString( _T( "72" ) );

	push	OFFSET FLAT:$SG73124
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddString@CComboBox@@QAEHPBG@Z		; CComboBox::AddString

; 148  : 
; 149  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?FillCombo@CSizeComboBox@@QAEXXZ ENDP			; CSizeComboBox::FillCombo
_TEXT	ENDS
END

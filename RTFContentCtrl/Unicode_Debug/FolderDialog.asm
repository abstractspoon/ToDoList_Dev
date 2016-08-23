	TITLE	D:\_CODE\Shared\FolderDialog.cpp
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
;	COMDAT ??_C@_0BP@FNCP@D?3?2_CODE?2Shared?2FolderDialog?4h?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0EK@MDCF@C?3?2Program?5Files?5?$CIx86?$CJ?2Microsoft@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ?EnableOK@CFolderDialog@@IAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetSelection@CFolderDialog@@IAEXPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetStatusText@CFolderDialog@@IAEXPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_GetBaseClass@CFolderDialog@@KGPAUCRuntimeClass@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetRuntimeClass@CFolderDialog@@UBEPAUCRuntimeClass@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CFolderDialog@@QAE@PBG0PAVCWnd@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCFolderDialog@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CCommonDialog@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CFolderDialog@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_GetBaseMessageMap@CFolderDialog@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMessageMap@CFolderDialog@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DoModal@CFolderDialog@@UAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnInitialized@CFolderDialog@@MAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnValidateFailed@CFolderDialog@@MAEHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnSelChanged@CFolderDialog@@MAEXPAU_ITEMIDLIST@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?BrowseCallbackProc@CFolderDialog@@CGHPAUHWND__@@IJJ@Z
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
;	COMDAT ??_7CFolderDialog@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?_GetBaseClass@CFolderDialog@@KGPAUCRuntimeClass@@XZ ; CFolderDialog::_GetBaseClass
PUBLIC	?classCFolderDialog@CFolderDialog@@2UCRuntimeClass@@B ; CFolderDialog::classCFolderDialog
PUBLIC	?_messageEntries@CFolderDialog@@0QBUAFX_MSGMAP_ENTRY@@B ; CFolderDialog::_messageEntries
PUBLIC	?messageMap@CFolderDialog@@1UAFX_MSGMAP@@B	; CFolderDialog::messageMap
PUBLIC	?_GetBaseMessageMap@CFolderDialog@@KGPBUAFX_MSGMAP@@XZ ; CFolderDialog::_GetBaseMessageMap
CONST	SEGMENT
?classCFolderDialog@CFolderDialog@@2UCRuntimeClass@@B DD FLAT:$SG73233 ; CFolderDialog::classCFolderDialog
	DD	06a4H
	DD	0ffffH
	DD	00H
	DD	FLAT:?_GetBaseClass@CFolderDialog@@KGPAUCRuntimeClass@@XZ
	DD	00H
?messageMap@CFolderDialog@@1UAFX_MSGMAP@@B DD FLAT:?_GetBaseMessageMap@CFolderDialog@@KGPBUAFX_MSGMAP@@XZ ; CFolderDialog::messageMap
	DD	FLAT:?_messageEntries@CFolderDialog@@0QBUAFX_MSGMAP_ENTRY@@B
?_messageEntries@CFolderDialog@@0QBUAFX_MSGMAP_ENTRY@@B DD 00H ; CFolderDialog::_messageEntries
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
CONST	ENDS
_DATA	SEGMENT
$SG73233 DB	'CFolderDialog', 00H
_DATA	ENDS
EXTRN	__imp_?classCDialog@CDialog@@2UCRuntimeClass@@B:BYTE
;	COMDAT ?_GetBaseClass@CFolderDialog@@KGPAUCRuntimeClass@@XZ
_TEXT	SEGMENT
?_GetBaseClass@CFolderDialog@@KGPAUCRuntimeClass@@XZ PROC NEAR ; CFolderDialog::_GetBaseClass, COMDAT

; 28   : IMPLEMENT_DYNAMIC(CFolderDialog, CDialog)

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR __imp_?classCDialog@CDialog@@2UCRuntimeClass@@B
	pop	ebp
	ret	0
?_GetBaseClass@CFolderDialog@@KGPAUCRuntimeClass@@XZ ENDP ; CFolderDialog::_GetBaseClass
_TEXT	ENDS
PUBLIC	?GetRuntimeClass@CFolderDialog@@UBEPAUCRuntimeClass@@XZ ; CFolderDialog::GetRuntimeClass
;	COMDAT ?GetRuntimeClass@CFolderDialog@@UBEPAUCRuntimeClass@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetRuntimeClass@CFolderDialog@@UBEPAUCRuntimeClass@@XZ PROC NEAR ; CFolderDialog::GetRuntimeClass, COMDAT

; 28   : IMPLEMENT_DYNAMIC(CFolderDialog, CDialog)

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, OFFSET FLAT:?classCFolderDialog@CFolderDialog@@2UCRuntimeClass@@B ; CFolderDialog::classCFolderDialog
	mov	esp, ebp
	pop	ebp
	ret	0
?GetRuntimeClass@CFolderDialog@@UBEPAUCRuntimeClass@@XZ ENDP ; CFolderDialog::GetRuntimeClass
_TEXT	ENDS
PUBLIC	??1CCommonDialog@@UAE@XZ			; CCommonDialog::~CCommonDialog
PUBLIC	??0CFolderDialog@@QAE@PBG0PAVCWnd@@I@Z		; CFolderDialog::CFolderDialog
PUBLIC	?DoModal@CFolderDialog@@UAEHXZ			; CFolderDialog::DoModal
PUBLIC	?GetMessageMap@CFolderDialog@@MBEPBUAFX_MSGMAP@@XZ ; CFolderDialog::GetMessageMap
PUBLIC	?OnInitialized@CFolderDialog@@MAEXXZ		; CFolderDialog::OnInitialized
PUBLIC	?OnValidateFailed@CFolderDialog@@MAEHPBG@Z	; CFolderDialog::OnValidateFailed
PUBLIC	?OnSelChanged@CFolderDialog@@MAEXPAU_ITEMIDLIST@@@Z ; CFolderDialog::OnSelChanged
PUBLIC	?BrowseCallbackProc@CFolderDialog@@CGHPAUHWND__@@IJJ@Z ; CFolderDialog::BrowseCallbackProc
PUBLIC	??_7CFolderDialog@@6B@				; CFolderDialog::`vftable'
PUBLIC	??_GCFolderDialog@@UAEPAXI@Z			; CFolderDialog::`scalar deleting destructor'
PUBLIC	??_ECFolderDialog@@UAEPAXI@Z			; CFolderDialog::`vector deleting destructor'
EXTRN	?PreSubclassWindow@CWnd@@UAEXXZ:NEAR		; CWnd::PreSubclassWindow
EXTRN	?Create@CWnd@@UAEHPBG0KABUtagRECT@@PAV1@IPAUCCreateContext@@@Z:NEAR ; CWnd::Create
EXTRN	?OnInitDialog@CDialog@@UAEHXZ:NEAR		; CDialog::OnInitDialog
EXTRN	?OnSetFont@CDialog@@UAEXPAVCFont@@@Z:NEAR	; CDialog::OnSetFont
EXTRN	?AssertValid@CDialog@@UBEXXZ:NEAR		; CDialog::AssertValid
EXTRN	?Dump@CDialog@@UBEXAAVCDumpContext@@@Z:NEAR	; CDialog::Dump
EXTRN	?PreTranslateMessage@CDialog@@UAEHPAUtagMSG@@@Z:NEAR ; CDialog::PreTranslateMessage
EXTRN	?OnCmdMsg@CDialog@@UAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z:NEAR ; CDialog::OnCmdMsg
EXTRN	?CheckAutoCenter@CDialog@@UAEHXZ:NEAR		; CDialog::CheckAutoCenter
EXTRN	?SetOccDialogInfo@CDialog@@MAEHPAU_AFX_OCC_DIALOG_INFO@@@Z:NEAR ; CDialog::SetOccDialogInfo
EXTRN	?PreInitDialog@CDialog@@MAEXXZ:NEAR		; CDialog::PreInitDialog
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	?DestroyWindow@CWnd@@UAEHXZ:NEAR		; CWnd::DestroyWindow
EXTRN	?PreCreateWindow@CWnd@@UAEHAAUtagCREATESTRUCTW@@@Z:NEAR ; CWnd::PreCreateWindow
EXTRN	?CalcWindowRect@CWnd@@UAEXPAUtagRECT@@I@Z:NEAR	; CWnd::CalcWindowRect
EXTRN	??0CCommonDialog@@QAE@PAVCWnd@@@Z:NEAR		; CCommonDialog::CCommonDialog
EXTRN	?OnOK@CCommonDialog@@MAEXXZ:NEAR		; CCommonDialog::OnOK
EXTRN	?OnCancel@CCommonDialog@@MAEXXZ:NEAR		; CCommonDialog::OnCancel
EXTRN	?OnToolHitTest@CWnd@@UBEHVCPoint@@PAUtagTOOLINFOW@@@Z:NEAR ; CWnd::OnToolHitTest
EXTRN	?GetScrollBarCtrl@CWnd@@UBEPAVCScrollBar@@H@Z:NEAR ; CWnd::GetScrollBarCtrl
EXTRN	?WinHelpW@CWnd@@UAEXKI@Z:NEAR			; CWnd::WinHelpW
EXTRN	?ContinueModal@CWnd@@UAEHXZ:NEAR		; CWnd::ContinueModal
EXTRN	?EndModalLoop@CWnd@@UAEXH@Z:NEAR		; CWnd::EndModalLoop
EXTRN	?OnCommand@CWnd@@MAEHIJ@Z:NEAR			; CWnd::OnCommand
EXTRN	?OnNotify@CWnd@@MAEHIJPAJ@Z:NEAR		; CWnd::OnNotify
EXTRN	_memset:NEAR
EXTRN	?IsInvokeAllowed@CCmdTarget@@UAEHJ@Z:NEAR	; CCmdTarget::IsInvokeAllowed
EXTRN	?GetDispatchIID@CCmdTarget@@UAEHPAU_GUID@@@Z:NEAR ; CCmdTarget::GetDispatchIID
EXTRN	?GetTypeInfoCount@CCmdTarget@@UAEIXZ:NEAR	; CCmdTarget::GetTypeInfoCount
EXTRN	?GetTypeLibCache@CCmdTarget@@UAEPAVCTypeLibCache@@XZ:NEAR ; CCmdTarget::GetTypeLibCache
EXTRN	?GetTypeLib@CCmdTarget@@UAEJKPAPAUITypeLib@@@Z:NEAR ; CCmdTarget::GetTypeLib
EXTRN	__chkesp:NEAR
EXTRN	__except_list:DWORD
EXTRN	?GetCommandMap@CCmdTarget@@MBEPBUAFX_OLECMDMAP@@XZ:NEAR ; CCmdTarget::GetCommandMap
EXTRN	___CxxFrameHandler:NEAR
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
EXTRN	?OnAmbientProperty@CWnd@@UAEHPAVCOleControlSite@@JPAUtagVARIANT@@@Z:NEAR ; CWnd::OnAmbientProperty
EXTRN	?WindowProc@CWnd@@MAEJIIJ@Z:NEAR		; CWnd::WindowProc
EXTRN	?OnWndMsg@CWnd@@MAEHIIJPAJ@Z:NEAR		; CWnd::OnWndMsg
EXTRN	?DefWindowProcW@CWnd@@MAEJIIJ@Z:NEAR		; CWnd::DefWindowProcW
EXTRN	?PostNcDestroy@CWnd@@MAEXXZ:NEAR		; CWnd::PostNcDestroy
EXTRN	?OnChildNotify@CWnd@@MAEHIIJPAJ@Z:NEAR		; CWnd::OnChildNotify
EXTRN	?GetExtraConnectionPoints@CCmdTarget@@MAEHPAVCPtrArray@@@Z:NEAR ; CCmdTarget::GetExtraConnectionPoints
EXTRN	?GetConnectionHook@CCmdTarget@@MAEPAUIConnectionPoint@@ABU_GUID@@@Z:NEAR ; CCmdTarget::GetConnectionHook
EXTRN	?IsFrameWnd@CWnd@@UBEHXZ:NEAR			; CWnd::IsFrameWnd
EXTRN	?OnFinalRelease@CWnd@@UAEXXZ:NEAR		; CWnd::OnFinalRelease
EXTRN	__imp__lstrcpyW@8:NEAR
EXTRN	__imp__lstrlenW@4:NEAR
EXTRN	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ:NEAR	; CWnd::GetSafeHwnd
;	COMDAT ??_7CFolderDialog@@6B@
; File D:\_CODE\Shared\FolderDialog.cpp
CONST	SEGMENT
??_7CFolderDialog@@6B@ DD FLAT:?GetRuntimeClass@CFolderDialog@@UBEPAUCRuntimeClass@@XZ ; CFolderDialog::`vftable'
	DD	FLAT:??_ECFolderDialog@@UAEPAXI@Z
	DD	FLAT:?Serialize@CObject@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@CDialog@@UBEXXZ
	DD	FLAT:?Dump@CDialog@@UBEXAAVCDumpContext@@@Z
	DD	FLAT:?OnCmdMsg@CDialog@@UAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z
	DD	FLAT:?OnFinalRelease@CWnd@@UAEXXZ
	DD	FLAT:?IsInvokeAllowed@CCmdTarget@@UAEHJ@Z
	DD	FLAT:?GetDispatchIID@CCmdTarget@@UAEHPAU_GUID@@@Z
	DD	FLAT:?GetTypeInfoCount@CCmdTarget@@UAEIXZ
	DD	FLAT:?GetTypeLibCache@CCmdTarget@@UAEPAVCTypeLibCache@@XZ
	DD	FLAT:?GetTypeLib@CCmdTarget@@UAEJKPAPAUITypeLib@@@Z
	DD	FLAT:?GetMessageMap@CFolderDialog@@MBEPBUAFX_MSGMAP@@XZ
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
	DD	FLAT:?PreTranslateMessage@CDialog@@UAEHPAUtagMSG@@@Z
	DD	FLAT:?OnAmbientProperty@CWnd@@UAEHPAVCOleControlSite@@JPAUtagVARIANT@@@Z
	DD	FLAT:?WindowProc@CWnd@@MAEJIIJ@Z
	DD	FLAT:?OnWndMsg@CWnd@@MAEHIIJPAJ@Z
	DD	FLAT:?DefWindowProcW@CWnd@@MAEJIIJ@Z
	DD	FLAT:?PostNcDestroy@CWnd@@MAEXXZ
	DD	FLAT:?OnChildNotify@CWnd@@MAEHIIJPAJ@Z
	DD	FLAT:?CheckAutoCenter@CDialog@@UAEHXZ
	DD	FLAT:?IsFrameWnd@CWnd@@UBEHXZ
	DD	FLAT:?SetOccDialogInfo@CDialog@@MAEHPAU_AFX_OCC_DIALOG_INFO@@@Z
	DD	FLAT:?DoModal@CFolderDialog@@UAEHXZ
	DD	FLAT:?OnInitDialog@CDialog@@UAEHXZ
	DD	FLAT:?OnSetFont@CDialog@@UAEXPAVCFont@@@Z
	DD	FLAT:?OnOK@CCommonDialog@@MAEXXZ
	DD	FLAT:?OnCancel@CCommonDialog@@MAEXXZ
	DD	FLAT:?PreInitDialog@CDialog@@MAEXXZ
	DD	FLAT:?OnInitialized@CFolderDialog@@MAEXXZ
	DD	FLAT:?OnValidateFailed@CFolderDialog@@MAEHPBG@Z
	DD	FLAT:?OnSelChanged@CFolderDialog@@MAEXPAU_ITEMIDLIST@@@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73475	DD	019930520H
	DD	01H
	DD	FLAT:$T73479
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73479	DD	0ffffffffH
	DD	FLAT:$L73471
xdata$x	ENDS
;	COMDAT ??0CFolderDialog@@QAE@PBG0PAVCWnd@@I@Z
_TEXT	SEGMENT
_lpszTitle$ = 8
_lpszFolderName$ = 12
_pParentWnd$ = 16
_uFlags$ = 20
_this$ = -16
__$EHRec$ = -12
??0CFolderDialog@@QAE@PBG0PAVCWnd@@I@Z PROC NEAR	; CFolderDialog::CFolderDialog, COMDAT

; 35   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73477
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _pParentWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CCommonDialog@@QAE@PAVCWnd@@@Z	; CCommonDialog::CCommonDialog
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+1696], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7CFolderDialog@@6B@ ; CFolderDialog::`vftable'

; 36   : 	m_szSelectedPath[0]	= '\0';

	mov	eax, DWORD PTR _this$[ebp]
	mov	WORD PTR [eax+650], 0

; 37   : 	m_szFolderPath[0]	= '\0';

	mov	ecx, DWORD PTR _this$[ebp]
	mov	WORD PTR [ecx+128], 0

; 38   : 	m_szFolderDisplayName[0]= '\0';

	mov	edx, DWORD PTR _this$[ebp]
	mov	WORD PTR [edx+1172], 0

; 39   : 	
; 40   : 	if(lpszFolderName != NULL && lstrlen(lpszFolderName))

	cmp	DWORD PTR _lpszFolderName$[ebp], 0
	je	SHORT $L73246
	mov	esi, esp
	mov	eax, DWORD PTR _lpszFolderName$[ebp]
	push	eax
	call	DWORD PTR __imp__lstrlenW@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L73246

; 41   : 		lstrcpy(m_szSelectedPath, lpszFolderName);

	mov	esi, esp
	mov	ecx, DWORD PTR _lpszFolderName$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 650				; 0000028aH
	push	edx
	call	DWORD PTR __imp__lstrcpyW@8
	cmp	esi, esp
	call	__chkesp
$L73246:

; 42   : 	
; 43   : 	// Fill
; 44   : 	::ZeroMemory(&m_bi, sizeof(BROWSEINFO)); 

	push	32					; 00000020H
	push	0
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 96					; 00000060H
	push	eax
	call	_memset
	add	esp, 12					; 0000000cH

; 45   : 	m_bi.hwndOwner = pParentWnd->GetSafeHwnd();

	mov	ecx, DWORD PTR _pParentWnd$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+96], eax

; 46   : 	m_bi.pidlRoot = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+100], 0

; 47   : 	m_bi.pszDisplayName = m_szFolderDisplayName;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 1172				; 00000494H
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+104], eax

; 48   : 	m_bi.lpszTitle = lpszTitle;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _lpszTitle$[ebp]
	mov	DWORD PTR [edx+108], eax

; 49   : 	m_bi.ulFlags = uFlags;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _uFlags$[ebp]
	mov	DWORD PTR [ecx+112], edx

; 50   : 	m_bi.lpfn = BrowseCallbackProc;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+116], OFFSET FLAT:?BrowseCallbackProc@CFolderDialog@@CGHPAUHWND__@@IJJ@Z ; CFolderDialog::BrowseCallbackProc

; 51   : 	m_bi.lParam = (LPARAM)this;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+120], edx

; 52   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73471:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CCommonDialog@@UAE@XZ		; CCommonDialog::~CCommonDialog
	ret	0
$L73477:
	mov	eax, OFFSET FLAT:$T73475
	jmp	___CxxFrameHandler
text$x	ENDS
??0CFolderDialog@@QAE@PBG0PAVCWnd@@I@Z ENDP		; CFolderDialog::CFolderDialog
PUBLIC	??1CFolderDialog@@UAE@XZ			; CFolderDialog::~CFolderDialog
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_GCFolderDialog@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCFolderDialog@@UAEPAXI@Z PROC NEAR			; CFolderDialog::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CFolderDialog@@UAE@XZ		; CFolderDialog::~CFolderDialog
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73252
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L73252:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCFolderDialog@@UAEPAXI@Z ENDP			; CFolderDialog::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	??1CDialog@@UAE@XZ:NEAR				; CDialog::~CDialog
;	COMDAT ??1CCommonDialog@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CCommonDialog@@UAE@XZ PROC NEAR			; CCommonDialog::~CCommonDialog, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CDialog@@UAE@XZ			; CDialog::~CDialog
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CCommonDialog@@UAE@XZ ENDP				; CCommonDialog::~CCommonDialog
_TEXT	ENDS
;	COMDAT ??1CFolderDialog@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CFolderDialog@@UAE@XZ PROC NEAR			; CFolderDialog::~CFolderDialog, COMDAT

; 55   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CFolderDialog@@6B@ ; CFolderDialog::`vftable'

; 56   : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CCommonDialog@@UAE@XZ		; CCommonDialog::~CCommonDialog
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CFolderDialog@@UAE@XZ ENDP				; CFolderDialog::~CFolderDialog
_TEXT	ENDS
EXTRN	__imp_?messageMap@CCommonDialog@@1UAFX_MSGMAP@@B:QWORD
;	COMDAT ?_GetBaseMessageMap@CFolderDialog@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
?_GetBaseMessageMap@CFolderDialog@@KGPBUAFX_MSGMAP@@XZ PROC NEAR ; CFolderDialog::_GetBaseMessageMap, COMDAT

; 58   : BEGIN_MESSAGE_MAP(CFolderDialog, CCommonDialog)

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR __imp_?messageMap@CCommonDialog@@1UAFX_MSGMAP@@B
	pop	ebp
	ret	0
?_GetBaseMessageMap@CFolderDialog@@KGPBUAFX_MSGMAP@@XZ ENDP ; CFolderDialog::_GetBaseMessageMap
_TEXT	ENDS
;	COMDAT ?GetMessageMap@CFolderDialog@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetMessageMap@CFolderDialog@@MBEPBUAFX_MSGMAP@@XZ PROC NEAR ; CFolderDialog::GetMessageMap, COMDAT

; 58   : BEGIN_MESSAGE_MAP(CFolderDialog, CCommonDialog)

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, OFFSET FLAT:?messageMap@CFolderDialog@@1UAFX_MSGMAP@@B ; CFolderDialog::messageMap
	mov	esp, ebp
	pop	ebp
	ret	0
?GetMessageMap@CFolderDialog@@MBEPBUAFX_MSGMAP@@XZ ENDP	; CFolderDialog::GetMessageMap
_TEXT	ENDS
EXTRN	?PreModal@CDialog@@IAEPAUHWND__@@XZ:NEAR	; CDialog::PreModal
EXTRN	?PostModal@CDialog@@IAEXXZ:NEAR			; CDialog::PostModal
EXTRN	__imp__CoInitialize@4:NEAR
EXTRN	__imp__CoUninitialize@0:NEAR
EXTRN	__imp__SHGetMalloc@4:NEAR
EXTRN	__imp__SHGetPathFromIDListW@8:NEAR
EXTRN	__imp__SHBrowseForFolderW@4:NEAR
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z:NEAR ; AfxAssertValidObject
;	COMDAT data
; File D:\_CODE\Shared\FolderDialog.cpp
data	SEGMENT
$SG73269 DB	'D:\_CODE\Shared\FolderDialog.cpp', 00H
	ORG $+3
$SG73274 DB	'D:\_CODE\Shared\FolderDialog.cpp', 00H
data	ENDS
;	COMDAT ?DoModal@CFolderDialog@@UAEHXZ
_TEXT	SEGMENT
_this$ = -16
_nRet$ = -4
_pItemIDList$ = -8
_pMalloc$73279 = -12
?DoModal@CFolderDialog@@UAEHXZ PROC NEAR		; CFolderDialog::DoModal, COMDAT

; 64   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 65   : 	ASSERT_VALID(this);	

	push	65					; 00000041H
	push	OFFSET FLAT:$SG73269
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject
$L73270:

; 66   : 	ASSERT(m_bi.lpfn != NULL);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+116], 0
	jne	SHORT $L73273
	push	66					; 00000042H
	push	OFFSET FLAT:$SG73274
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73273
	int	3
$L73273:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L73270

; 67   : 
; 68   : 	CoInitialize(NULL);

	mov	esi, esp
	push	0
	call	DWORD PTR __imp__CoInitialize@4
	cmp	esi, esp
	call	__chkesp

; 69   : 		
; 70   : 	m_bi.hwndOwner = PreModal();	

	mov	ecx, DWORD PTR _this$[ebp]
	call	?PreModal@CDialog@@IAEPAUHWND__@@XZ	; CDialog::PreModal
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+96], eax

; 71   : 	INT_PTR nRet   = -1;

	mov	DWORD PTR _nRet$[ebp], -1

; 72   : 	LPITEMIDLIST pItemIDList = ::SHBrowseForFolder(&m_bi);

	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 96					; 00000060H
	mov	esi, esp
	push	edx
	call	DWORD PTR __imp__SHBrowseForFolderW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pItemIDList$[ebp], eax

; 73   : 
; 74   : 	if(pItemIDList != NULL)

	cmp	DWORD PTR _pItemIDList$[ebp], 0
	je	SHORT $L73282

; 76   : 		if(::SHGetPathFromIDList(pItemIDList, m_szFolderPath))

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 128				; 00000080H
	mov	esi, esp
	push	eax
	mov	ecx, DWORD PTR _pItemIDList$[ebp]
	push	ecx
	call	DWORD PTR __imp__SHGetPathFromIDListW@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L73278

; 78   : 			IMalloc* pMalloc = NULL;

	mov	DWORD PTR _pMalloc$73279[ebp], 0

; 79   : 			if(SUCCEEDED(::SHGetMalloc(&pMalloc)))

	mov	esi, esp
	lea	edx, DWORD PTR _pMalloc$73279[ebp]
	push	edx
	call	DWORD PTR __imp__SHGetMalloc@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jl	SHORT $L73281

; 81   : 				pMalloc->Free(pItemIDList);

	mov	esi, esp
	mov	eax, DWORD PTR _pItemIDList$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pMalloc$73279[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	eax, DWORD PTR _pMalloc$73279[ebp]
	push	eax
	call	DWORD PTR [edx+20]
	cmp	esi, esp
	call	__chkesp

; 82   : 				pMalloc->Release();

	mov	ecx, DWORD PTR _pMalloc$73279[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	esi, esp
	mov	eax, DWORD PTR _pMalloc$73279[ebp]
	push	eax
	call	DWORD PTR [edx+8]
	cmp	esi, esp
	call	__chkesp
$L73281:

; 84   : 			nRet = IDOK;

	mov	DWORD PTR _nRet$[ebp], 1

; 86   : 		else

	jmp	SHORT $L73282
$L73278:

; 88   : 			nRet = IDCANCEL;

	mov	DWORD PTR _nRet$[ebp], 2
$L73282:

; 91   : 	PostModal();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?PostModal@CDialog@@IAEXXZ		; CDialog::PostModal

; 92   : 
; 93   : 	CoUninitialize();

	mov	esi, esp
	call	DWORD PTR __imp__CoUninitialize@0
	cmp	esi, esp
	call	__chkesp

; 94   : 
; 95   : 	return nRet;	

	mov	eax, DWORD PTR _nRet$[ebp]

; 96   : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?DoModal@CFolderDialog@@UAEHXZ ENDP			; CFolderDialog::DoModal
_TEXT	ENDS
PUBLIC	?SetSelection@CFolderDialog@@IAEXPBG@Z		; CFolderDialog::SetSelection
;	COMDAT ?OnInitialized@CFolderDialog@@MAEXXZ
_TEXT	SEGMENT
_this$ = -4
?OnInitialized@CFolderDialog@@MAEXXZ PROC NEAR		; CFolderDialog::OnInitialized, COMDAT

; 101  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 102  : 	if(lstrlen(m_szSelectedPath))

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 650				; 0000028aH
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__lstrlenW@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L73287

; 103  : 		SetSelection(m_szSelectedPath);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 650				; 0000028aH
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetSelection@CFolderDialog@@IAEXPBG@Z	; CFolderDialog::SetSelection
$L73287:

; 104  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?OnInitialized@CFolderDialog@@MAEXXZ ENDP		; CFolderDialog::OnInitialized
_TEXT	ENDS
PUBLIC	??_C@_0BP@FNCP@D?3?2_CODE?2Shared?2FolderDialog?4h?$AA@ ; `string'
EXTRN	__imp__SendMessageW@16:NEAR
;	COMDAT ??_C@_0BP@FNCP@D?3?2_CODE?2Shared?2FolderDialog?4h?$AA@
; File D:\_CODE\Shared\FolderDialog.h
_DATA	SEGMENT
??_C@_0BP@FNCP@D?3?2_CODE?2Shared?2FolderDialog?4h?$AA@ DB 'D:\_CODE\Shar'
	DB	'ed\FolderDialog.h', 00H			; `string'
_DATA	ENDS
;	COMDAT ?SetSelection@CFolderDialog@@IAEXPBG@Z
_TEXT	SEGMENT
_lpszFolderPath$ = 8
_this$ = -4
?SetSelection@CFolderDialog@@IAEXPBG@Z PROC NEAR	; CFolderDialog::SetSelection, COMDAT

; 106  : 	{ ASSERT(m_hWnd != NULL);

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L73167:
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+1696], 0
	jne	SHORT $L73170
	push	106					; 0000006aH
	push	OFFSET FLAT:??_C@_0BP@FNCP@D?3?2_CODE?2Shared?2FolderDialog?4h?$AA@ ; `string'
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73170
	int	3
$L73170:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73167

; 107  : 		::SendMessage(m_hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)lpszFolderPath); }

	mov	esi, esp
	mov	edx, DWORD PTR _lpszFolderPath$[ebp]
	push	edx
	push	1
	push	1127					; 00000467H
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+1696]
	push	ecx
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?SetSelection@CFolderDialog@@IAEXPBG@Z ENDP		; CFolderDialog::SetSelection
_TEXT	ENDS
;	COMDAT ?OnValidateFailed@CFolderDialog@@MAEHPBG@Z
_TEXT	SEGMENT
_this$ = -8
_nRet$ = -4
?OnValidateFailed@CFolderDialog@@MAEHPBG@Z PROC NEAR	; CFolderDialog::OnValidateFailed, COMDAT

; 107  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 108  : 	int nRet = IDYES;//AfxMessageBox(_T("The path entered is not valid! Continue ?"), 

	mov	DWORD PTR _nRet$[ebp], 6

; 109  : 					//MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
; 110  : 	
; 111  : 	// Return 1 = continue, 0 = EndDialog
; 112  : 	return((nRet == IDYES) ? 0 : 1);

	xor	eax, eax
	cmp	DWORD PTR _nRet$[ebp], 6
	setne	al

; 113  : }

	mov	esp, ebp
	pop	ebp
	ret	4
?OnValidateFailed@CFolderDialog@@MAEHPBG@Z ENDP		; CFolderDialog::OnValidateFailed
_TEXT	ENDS
PUBLIC	?EnableOK@CFolderDialog@@IAEXH@Z		; CFolderDialog::EnableOK
PUBLIC	?SetStatusText@CFolderDialog@@IAEXPBG@Z		; CFolderDialog::SetStatusText
EXTRN	?IsRemotePath@CDriveInfo@@SAHPBGH@Z:NEAR	; CDriveInfo::IsRemotePath
EXTRN	?IsFixedPath@CDriveInfo@@SAHPBG@Z:NEAR		; CDriveInfo::IsFixedPath
EXTRN	?IsReadonlyPath@CDriveInfo@@SAHPBG@Z:NEAR	; CDriveInfo::IsReadonlyPath
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	__imp__GetFileAttributesW@4:NEAR
EXTRN	??4CString@@QAEABV0@PBD@Z:NEAR			; CString::operator=
EXTRN	?MakeUpper@CString@@QAEXXZ:NEAR			; CString::MakeUpper
EXTRN	?GetBuffer@CString@@QAEPAGH@Z:NEAR		; CString::GetBuffer
EXTRN	?ReleaseBuffer@CString@@QAEXH@Z:NEAR		; CString::ReleaseBuffer
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
_BSS	SEGMENT
$SG73304 DB	01H DUP (?)
	ALIGN	4

$SG73307 DB	01H DUP (?)
	ALIGN	4

$SG73310 DB	01H DUP (?)
	ALIGN	4

$SG73313 DB	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\FolderDialog.cpp
data	SEGMENT
$SG73302 DB	'< Please select a valid folder >', 00H
	ORG $+3
$SG73305 DB	'< Please select a folder on a local drive >', 00H
$SG73308 DB	'< Please select a folder on a non-removeable drive >', 00H
	ORG $+3
$SG73311 DB	'< Please select a non-readonly folder >', 00H
$SG73314 DB	'< Please select a non-root folder >', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T73517	DD	019930520H
	DD	02H
	DD	FLAT:$T73519
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T73519	DD	0ffffffffH
	DD	FLAT:$L73514
	DD	00H
	DD	FLAT:$L73515
xdata$x	ENDS
;	COMDAT ?OnSelChanged@CFolderDialog@@MAEXPAU_ITEMIDLIST@@@Z
_TEXT	SEGMENT
_pItemIDList$ = 8
_this$ = -32
_sStatusText$ = -20
_sPath$ = -28
_dwAttrib$ = -16
_bEnable$ = -24
__$EHRec$ = -12
?OnSelChanged@CFolderDialog@@MAEXPAU_ITEMIDLIST@@@Z PROC NEAR ; CFolderDialog::OnSelChanged, COMDAT

; 116  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L73518
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 36					; 00000024H
	push	esi
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
	mov	DWORD PTR _this$[ebp], ecx

; 117  : 	CString sStatusText, sPath;

	lea	ecx, DWORD PTR _sStatusText$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 118  : 
; 119  : 	SHGetPathFromIDList(pItemIDList, sPath.GetBuffer(MAX_PATH+1));

	push	261					; 00000105H
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	mov	esi, esp
	push	eax
	mov	eax, DWORD PTR _pItemIDList$[ebp]
	push	eax
	call	DWORD PTR __imp__SHGetPathFromIDListW@8
	cmp	esi, esp
	call	__chkesp

; 120  : 	sPath.ReleaseBuffer();

	push	-1
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 121  : 	sPath.MakeUpper();

	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?MakeUpper@CString@@QAEXXZ		; CString::MakeUpper

; 122  : 
; 123  : 	DWORD dwAttrib = GetFileAttributes(sPath);

	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__GetFileAttributesW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _dwAttrib$[ebp], eax

; 124  : 	BOOL bEnable = (dwAttrib != 0xffffffff);

	xor	ecx, ecx
	cmp	DWORD PTR _dwAttrib$[ebp], -1
	setne	cl
	mov	DWORD PTR _bEnable$[ebp], ecx

; 125  : 
; 126  : 	if (!bEnable)

	cmp	DWORD PTR _bEnable$[ebp], 0
	jne	SHORT $L73301

; 127  : 		sStatusText = "< Please select a valid folder >";

	push	OFFSET FLAT:$SG73302
	lea	ecx, DWORD PTR _sStatusText$[ebp]
	call	??4CString@@QAEABV0@PBD@Z		; CString::operator=
$L73301:

; 128  : 
; 129  : 	if (bEnable && (m_bi.ulFlags & BIF_BROWSENONETWORK))

	cmp	DWORD PTR _bEnable$[ebp], 0
	je	SHORT $L73303
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+112]
	and	eax, 32768				; 00008000H
	test	eax, eax
	je	SHORT $L73303

; 131  : 		bEnable = !CDriveInfo::IsRemotePath(sPath);

	push	1
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsRemotePath@CDriveInfo@@SAHPBGH@Z	; CDriveInfo::IsRemotePath
	add	esp, 8
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR _bEnable$[ebp], eax

; 132  : 		sStatusText = bEnable ? "" : "< Please select a folder on a local drive >";

	cmp	DWORD PTR _bEnable$[ebp], 0
	je	SHORT $L73506
	mov	DWORD PTR -36+[ebp], OFFSET FLAT:$SG73304
	jmp	SHORT $L73507
$L73506:
	mov	DWORD PTR -36+[ebp], OFFSET FLAT:$SG73305
$L73507:
	mov	ecx, DWORD PTR -36+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sStatusText$[ebp]
	call	??4CString@@QAEABV0@PBD@Z		; CString::operator=
$L73303:

; 134  : 
; 135  : 	if (bEnable && (m_bi.ulFlags & BIF_BROWSEFIXEDONLY))

	cmp	DWORD PTR _bEnable$[ebp], 0
	je	SHORT $L73306
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+112]
	and	eax, 65536				; 00010000H
	test	eax, eax
	je	SHORT $L73306

; 137  : 		bEnable = CDriveInfo::IsFixedPath(sPath);

	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsFixedPath@CDriveInfo@@SAHPBG@Z	; CDriveInfo::IsFixedPath
	add	esp, 4
	mov	DWORD PTR _bEnable$[ebp], eax

; 138  : 		sStatusText = bEnable ? "" : "< Please select a folder on a non-removeable drive >";

	cmp	DWORD PTR _bEnable$[ebp], 0
	je	SHORT $L73508
	mov	DWORD PTR -40+[ebp], OFFSET FLAT:$SG73307
	jmp	SHORT $L73509
$L73508:
	mov	DWORD PTR -40+[ebp], OFFSET FLAT:$SG73308
$L73509:
	mov	ecx, DWORD PTR -40+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sStatusText$[ebp]
	call	??4CString@@QAEABV0@PBD@Z		; CString::operator=
$L73306:

; 140  : 
; 141  : 	if (bEnable && (m_bi.ulFlags & BIF_BROWSENOREADONLY))

	cmp	DWORD PTR _bEnable$[ebp], 0
	je	SHORT $L73309
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+112]
	and	eax, 131072				; 00020000H
	test	eax, eax
	je	SHORT $L73309

; 143  : 		bEnable = !CDriveInfo::IsReadonlyPath(sPath);

	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsReadonlyPath@CDriveInfo@@SAHPBG@Z	; CDriveInfo::IsReadonlyPath
	add	esp, 4
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR _bEnable$[ebp], eax

; 144  : 		sStatusText = bEnable ? "" : "< Please select a non-readonly folder >";

	cmp	DWORD PTR _bEnable$[ebp], 0
	je	SHORT $L73510
	mov	DWORD PTR -44+[ebp], OFFSET FLAT:$SG73310
	jmp	SHORT $L73511
$L73510:
	mov	DWORD PTR -44+[ebp], OFFSET FLAT:$SG73311
$L73511:
	mov	ecx, DWORD PTR -44+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sStatusText$[ebp]
	call	??4CString@@QAEABV0@PBD@Z		; CString::operator=
$L73309:

; 146  : 
; 147  : 	if (bEnable && (m_bi.ulFlags & BIF_BROWSENOROOTDIR))

	cmp	DWORD PTR _bEnable$[ebp], 0
	je	SHORT $L73312
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+112]
	and	eax, 262144				; 00040000H
	test	eax, eax
	je	SHORT $L73312

; 149  : 		bEnable = sPath.GetLength() > 3;

	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	xor	ecx, ecx
	cmp	eax, 3
	setg	cl
	mov	DWORD PTR _bEnable$[ebp], ecx

; 150  : 		sStatusText = bEnable ? "" : "< Please select a non-root folder >";

	cmp	DWORD PTR _bEnable$[ebp], 0
	je	SHORT $L73512
	mov	DWORD PTR -48+[ebp], OFFSET FLAT:$SG73313
	jmp	SHORT $L73513
$L73512:
	mov	DWORD PTR -48+[ebp], OFFSET FLAT:$SG73314
$L73513:
	mov	edx, DWORD PTR -48+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sStatusText$[ebp]
	call	??4CString@@QAEABV0@PBD@Z		; CString::operator=
$L73312:

; 152  : 
; 153  : 	EnableOK(bEnable);

	mov	eax, DWORD PTR _bEnable$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?EnableOK@CFolderDialog@@IAEXH@Z	; CFolderDialog::EnableOK

; 154  : 	SetStatusText(sStatusText);

	lea	ecx, DWORD PTR _sStatusText$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetStatusText@CFolderDialog@@IAEXPBG@Z	; CFolderDialog::SetStatusText

; 155  : }

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sStatusText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 48					; 00000030H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L73514:
	lea	ecx, DWORD PTR _sStatusText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73515:
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L73518:
	mov	eax, OFFSET FLAT:$T73517
	jmp	___CxxFrameHandler
text$x	ENDS
?OnSelChanged@CFolderDialog@@MAEXPAU_ITEMIDLIST@@@Z ENDP ; CFolderDialog::OnSelChanged
;	COMDAT ?EnableOK@CFolderDialog@@IAEXH@Z
_TEXT	SEGMENT
_bEnable$ = 8
_this$ = -4
?EnableOK@CFolderDialog@@IAEXH@Z PROC NEAR		; CFolderDialog::EnableOK, COMDAT

; 98   : 	{ ASSERT(m_hWnd != NULL); 

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L73150:
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+1696], 0
	jne	SHORT $L73153
	push	98					; 00000062H
	push	OFFSET FLAT:??_C@_0BP@FNCP@D?3?2_CODE?2Shared?2FolderDialog?4h?$AA@ ; `string'
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73153
	int	3
$L73153:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73150

; 99   : 		::SendMessage(m_hWnd, BFFM_ENABLEOK, 0L, bEnable);}

	mov	esi, esp
	mov	edx, DWORD PTR _bEnable$[ebp]
	push	edx
	push	0
	push	1125					; 00000465H
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+1696]
	push	ecx
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?EnableOK@CFolderDialog@@IAEXH@Z ENDP			; CFolderDialog::EnableOK
_TEXT	ENDS
;	COMDAT ?SetStatusText@CFolderDialog@@IAEXPBG@Z
_TEXT	SEGMENT
_lpszText$ = 8
_this$ = -4
?SetStatusText@CFolderDialog@@IAEXPBG@Z PROC NEAR	; CFolderDialog::SetStatusText, COMDAT

; 110  : 	{ ASSERT(m_hWnd != NULL);

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L73176:
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+1696], 0
	jne	SHORT $L73179
	push	110					; 0000006eH
	push	OFFSET FLAT:??_C@_0BP@FNCP@D?3?2_CODE?2Shared?2FolderDialog?4h?$AA@ ; `string'
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73179
	int	3
$L73179:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73176

; 111  : 		::SendMessage(m_hWnd, BFFM_SETSTATUSTEXT, 0L, (LPARAM)lpszText);}

	mov	esi, esp
	mov	edx, DWORD PTR _lpszText$[ebp]
	push	edx
	push	0
	push	1128					; 00000468H
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+1696]
	push	ecx
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?SetStatusText@CFolderDialog@@IAEXPBG@Z ENDP		; CFolderDialog::SetStatusText
_TEXT	ENDS
;	COMDAT ?BrowseCallbackProc@CFolderDialog@@CGHPAUHWND__@@IJJ@Z
_TEXT	SEGMENT
_hWnd$ = 8
_uMsg$ = 12
_lParam$ = 16
_lpData$ = 20
_pThis$ = -8
_nRet$ = -4
?BrowseCallbackProc@CFolderDialog@@CGHPAUHWND__@@IJJ@Z PROC NEAR ; CFolderDialog::BrowseCallbackProc, COMDAT

; 161  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 162  : 	CFolderDialog* pThis = (CFolderDialog*)(lpData);

	mov	eax, DWORD PTR _lpData$[ebp]
	mov	DWORD PTR _pThis$[ebp], eax

; 163  : 	pThis->m_hWnd = hWnd;

	mov	ecx, DWORD PTR _pThis$[ebp]
	mov	edx, DWORD PTR _hWnd$[ebp]
	mov	DWORD PTR [ecx+1696], edx

; 164  : 	int nRet = 0;

	mov	DWORD PTR _nRet$[ebp], 0

; 167  : 	{

	mov	eax, DWORD PTR _uMsg$[ebp]
	mov	DWORD PTR -12+[ebp], eax
	cmp	DWORD PTR -12+[ebp], 1
	je	SHORT $L73328
	cmp	DWORD PTR -12+[ebp], 2
	je	SHORT $L73329
	cmp	DWORD PTR -12+[ebp], 4
	je	SHORT $L73331
	jmp	SHORT $L73325
$L73328:

; 168  : 	case BFFM_INITIALIZED:
; 169  : 		pThis->OnInitialized();

	mov	ecx, DWORD PTR _pThis$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	esi, esp
	mov	ecx, DWORD PTR _pThis$[ebp]
	call	DWORD PTR [edx+216]
	cmp	esi, esp
	call	__chkesp

; 170  : 		break;

	jmp	SHORT $L73325
$L73329:

; 171  : 	case BFFM_SELCHANGED:
; 172  : 		pThis->OnSelChanged((LPITEMIDLIST)lParam);

	mov	esi, esp
	mov	eax, DWORD PTR _lParam$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pThis$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _pThis$[ebp]
	call	DWORD PTR [edx+224]
	cmp	esi, esp
	call	__chkesp

; 173  : 		break;

	jmp	SHORT $L73325
$L73331:

; 174  : 	case BFFM_VALIDATEFAILED:
; 175  : 		nRet = pThis->OnValidateFailed((LPCTSTR)lParam);

	mov	esi, esp
	mov	eax, DWORD PTR _lParam$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pThis$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _pThis$[ebp]
	call	DWORD PTR [edx+220]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nRet$[ebp], eax
$L73325:

; 178  : 	pThis->m_hWnd = NULL;

	mov	eax, DWORD PTR _pThis$[ebp]
	mov	DWORD PTR [eax+1696], 0

; 179  : 	return nRet;	

	mov	eax, DWORD PTR _nRet$[ebp]

; 180  : }

	pop	esi
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?BrowseCallbackProc@CFolderDialog@@CGHPAUHWND__@@IJJ@Z ENDP ; CFolderDialog::BrowseCallbackProc
_TEXT	ENDS
END

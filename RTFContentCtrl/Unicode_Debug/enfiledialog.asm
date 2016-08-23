	TITLE	D:\_CODE\Shared\enfiledialog.cpp
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
;	COMDAT ??BCOSVersion@@QAE?AW4OSVERSION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_GetBaseClass@CEnFileDialog@@KGPAUCRuntimeClass@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetRuntimeClass@CEnFileDialog@@UBEPAUCRuntimeClass@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CEnFileDialog@@QAE@HPBG00K0PAVCWnd@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCEnFileDialog@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CFileDialog@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CEnFileDialog@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CCommonDialog@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_GetBaseMessageMap@CEnFileDialog@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMessageMap@CEnFileDialog@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DoModal@CEnFileDialog@@QAEHPAVIPreferences@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetPreferenceKey@CEnFileDialog@@IBE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetTitle@CEnFileDialog@@QAEXPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HasInitialFileName@CEnFileDialog@@IBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HasInitialFolder@CEnFileDialog@@IBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_GetBaseClass@CFileOpenDialog@@KGPAUCRuntimeClass@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetRuntimeClass@CFileOpenDialog@@UBEPAUCRuntimeClass@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CFileOpenDialog@@QAE@PBG00K0PAVCWnd@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCFileOpenDialog@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CFileOpenDialog@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CFileOpenDialog@@QAE@IPBG0K0PAVCWnd@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_GetBaseClass@CFileSaveDialog@@KGPAUCRuntimeClass@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetRuntimeClass@CFileSaveDialog@@UBEPAUCRuntimeClass@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CFileSaveDialog@@QAE@PBG00K0PAVCWnd@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCFileSaveDialog@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CFileSaveDialog@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CFileSaveDialog@@QAE@IPBG0K0PAVCWnd@@@Z
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
;	COMDAT ??_7CFileSaveDialog@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CFileOpenDialog@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CEnFileDialog@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?_GetBaseClass@CEnFileDialog@@KGPAUCRuntimeClass@@XZ ; CEnFileDialog::_GetBaseClass
PUBLIC	?classCEnFileDialog@CEnFileDialog@@2UCRuntimeClass@@B ; CEnFileDialog::classCEnFileDialog
PUBLIC	?_messageEntries@CEnFileDialog@@0QBUAFX_MSGMAP_ENTRY@@B ; CEnFileDialog::_messageEntries
PUBLIC	?messageMap@CEnFileDialog@@1UAFX_MSGMAP@@B	; CEnFileDialog::messageMap
PUBLIC	?_GetBaseMessageMap@CEnFileDialog@@KGPBUAFX_MSGMAP@@XZ ; CEnFileDialog::_GetBaseMessageMap
PUBLIC	?_GetBaseClass@CFileOpenDialog@@KGPAUCRuntimeClass@@XZ ; CFileOpenDialog::_GetBaseClass
PUBLIC	?classCFileOpenDialog@CFileOpenDialog@@2UCRuntimeClass@@B ; CFileOpenDialog::classCFileOpenDialog
PUBLIC	?_GetBaseClass@CFileSaveDialog@@KGPAUCRuntimeClass@@XZ ; CFileSaveDialog::_GetBaseClass
PUBLIC	?classCFileSaveDialog@CFileSaveDialog@@2UCRuntimeClass@@B ; CFileSaveDialog::classCFileSaveDialog
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\Shared\enfiledialog.cpp', 00H
_DATA	ENDS
CONST	SEGMENT
?classCEnFileDialog@CEnFileDialog@@2UCRuntimeClass@@B DD FLAT:$SG73871 ; CEnFileDialog::classCEnFileDialog
	DD	0348H
	DD	0ffffH
	DD	00H
	DD	FLAT:?_GetBaseClass@CEnFileDialog@@KGPAUCRuntimeClass@@XZ
	DD	00H
?messageMap@CEnFileDialog@@1UAFX_MSGMAP@@B DD FLAT:?_GetBaseMessageMap@CEnFileDialog@@KGPBUAFX_MSGMAP@@XZ ; CEnFileDialog::messageMap
	DD	FLAT:?_messageEntries@CEnFileDialog@@0QBUAFX_MSGMAP_ENTRY@@B
?_messageEntries@CEnFileDialog@@0QBUAFX_MSGMAP_ENTRY@@B DD 00H ; CEnFileDialog::_messageEntries
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
?classCFileOpenDialog@CFileOpenDialog@@2UCRuntimeClass@@B DD FLAT:$SG73977 ; CFileOpenDialog::classCFileOpenDialog
	DD	0348H
	DD	0ffffH
	DD	00H
	DD	FLAT:?_GetBaseClass@CFileOpenDialog@@KGPAUCRuntimeClass@@XZ
	DD	00H
?classCFileSaveDialog@CFileSaveDialog@@2UCRuntimeClass@@B DD FLAT:$SG74011 ; CFileSaveDialog::classCFileSaveDialog
	DD	0348H
	DD	0ffffH
	DD	00H
	DD	FLAT:?_GetBaseClass@CFileSaveDialog@@KGPAUCRuntimeClass@@XZ
	DD	00H
CONST	ENDS
_DATA	SEGMENT
	ORG $+3
$SG73871 DB	'CEnFileDialog', 00H
	ORG $+2
$SG73977 DB	'CFileOpenDialog', 00H
$SG74011 DB	'CFileSaveDialog', 00H
_DATA	ENDS
EXTRN	__imp_?classCFileDialog@CFileDialog@@2UCRuntimeClass@@B:BYTE
;	COMDAT ?_GetBaseClass@CEnFileDialog@@KGPAUCRuntimeClass@@XZ
_TEXT	SEGMENT
?_GetBaseClass@CEnFileDialog@@KGPAUCRuntimeClass@@XZ PROC NEAR ; CEnFileDialog::_GetBaseClass, COMDAT

; 20   : IMPLEMENT_DYNAMIC(CEnFileDialog, CFileDialog)

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR __imp_?classCFileDialog@CFileDialog@@2UCRuntimeClass@@B
	pop	ebp
	ret	0
?_GetBaseClass@CEnFileDialog@@KGPAUCRuntimeClass@@XZ ENDP ; CEnFileDialog::_GetBaseClass
_TEXT	ENDS
PUBLIC	?GetRuntimeClass@CEnFileDialog@@UBEPAUCRuntimeClass@@XZ ; CEnFileDialog::GetRuntimeClass
;	COMDAT ?GetRuntimeClass@CEnFileDialog@@UBEPAUCRuntimeClass@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetRuntimeClass@CEnFileDialog@@UBEPAUCRuntimeClass@@XZ PROC NEAR ; CEnFileDialog::GetRuntimeClass, COMDAT

; 20   : IMPLEMENT_DYNAMIC(CEnFileDialog, CFileDialog)

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, OFFSET FLAT:?classCEnFileDialog@CEnFileDialog@@2UCRuntimeClass@@B ; CEnFileDialog::classCEnFileDialog
	mov	esp, ebp
	pop	ebp
	ret	0
?GetRuntimeClass@CEnFileDialog@@UBEPAUCRuntimeClass@@XZ ENDP ; CEnFileDialog::GetRuntimeClass
_TEXT	ENDS
PUBLIC	??1CFileDialog@@UAE@XZ				; CFileDialog::~CFileDialog
PUBLIC	??0CEnFileDialog@@QAE@HPBG00K0PAVCWnd@@@Z	; CEnFileDialog::CEnFileDialog
PUBLIC	?GetMessageMap@CEnFileDialog@@MBEPBUAFX_MSGMAP@@XZ ; CEnFileDialog::GetMessageMap
PUBLIC	??_7CEnFileDialog@@6B@				; CEnFileDialog::`vftable'
PUBLIC	??_GCEnFileDialog@@UAEPAXI@Z			; CEnFileDialog::`scalar deleting destructor'
PUBLIC	??_ECEnFileDialog@@UAEPAXI@Z			; CEnFileDialog::`vector deleting destructor'
EXTRN	?PreSubclassWindow@CWnd@@UAEXXZ:NEAR		; CWnd::PreSubclassWindow
EXTRN	__imp__GetVersion@0:NEAR
EXTRN	?Create@CWnd@@UAEHPBG0KABUtagRECT@@PAV1@IPAUCCreateContext@@@Z:NEAR ; CWnd::Create
EXTRN	?OnInitDialog@CDialog@@UAEHXZ:NEAR		; CDialog::OnInitDialog
EXTRN	?OnSetFont@CDialog@@UAEXPAVCFont@@@Z:NEAR	; CDialog::OnSetFont
EXTRN	?AssertValid@CDialog@@UBEXXZ:NEAR		; CDialog::AssertValid
EXTRN	?PreTranslateMessage@CDialog@@UAEHPAUtagMSG@@@Z:NEAR ; CDialog::PreTranslateMessage
EXTRN	?OnCmdMsg@CDialog@@UAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z:NEAR ; CDialog::OnCmdMsg
EXTRN	?CheckAutoCenter@CDialog@@UAEHXZ:NEAR		; CDialog::CheckAutoCenter
EXTRN	?SetOccDialogInfo@CDialog@@MAEHPAU_AFX_OCC_DIALOG_INFO@@@Z:NEAR ; CDialog::SetOccDialogInfo
EXTRN	?PreInitDialog@CDialog@@MAEXXZ:NEAR		; CDialog::PreInitDialog
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	?DestroyWindow@CWnd@@UAEHXZ:NEAR		; CWnd::DestroyWindow
EXTRN	?PreCreateWindow@CWnd@@UAEHAAUtagCREATESTRUCTW@@@Z:NEAR ; CWnd::PreCreateWindow
EXTRN	?CalcWindowRect@CWnd@@UAEXPAUtagRECT@@I@Z:NEAR	; CWnd::CalcWindowRect
EXTRN	?OnOK@CCommonDialog@@MAEXXZ:NEAR		; CCommonDialog::OnOK
EXTRN	?OnCancel@CCommonDialog@@MAEXXZ:NEAR		; CCommonDialog::OnCancel
EXTRN	??0CFileDialog@@QAE@HPBG0K0PAVCWnd@@@Z:NEAR	; CFileDialog::CFileDialog
EXTRN	?DoModal@CFileDialog@@UAEHXZ:NEAR		; CFileDialog::DoModal
EXTRN	?OnToolHitTest@CWnd@@UBEHVCPoint@@PAUtagTOOLINFOW@@@Z:NEAR ; CWnd::OnToolHitTest
EXTRN	?OnShareViolation@CFileDialog@@MAEIPBG@Z:NEAR	; CFileDialog::OnShareViolation
EXTRN	?OnFileNameOK@CFileDialog@@MAEHXZ:NEAR		; CFileDialog::OnFileNameOK
EXTRN	?OnLBSelChangedNotify@CFileDialog@@MAEXIII@Z:NEAR ; CFileDialog::OnLBSelChangedNotify
EXTRN	?OnInitDone@CFileDialog@@MAEXXZ:NEAR		; CFileDialog::OnInitDone
EXTRN	?OnFileNameChange@CFileDialog@@MAEXXZ:NEAR	; CFileDialog::OnFileNameChange
EXTRN	?OnFolderChange@CFileDialog@@MAEXXZ:NEAR	; CFileDialog::OnFolderChange
EXTRN	?OnTypeChange@CFileDialog@@MAEXXZ:NEAR		; CFileDialog::OnTypeChange
EXTRN	?Dump@CFileDialog@@UBEXAAVCDumpContext@@@Z:NEAR	; CFileDialog::Dump
EXTRN	?OnNotify@CFileDialog@@MAEHIJPAJ@Z:NEAR		; CFileDialog::OnNotify
EXTRN	?GetScrollBarCtrl@CWnd@@UBEPAVCScrollBar@@H@Z:NEAR ; CWnd::GetScrollBarCtrl
EXTRN	__chkesp:NEAR
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	?WinHelpW@CWnd@@UAEXKI@Z:NEAR			; CWnd::WinHelpW
EXTRN	?ContinueModal@CWnd@@UAEHXZ:NEAR		; CWnd::ContinueModal
EXTRN	?EndModalLoop@CWnd@@UAEXH@Z:NEAR		; CWnd::EndModalLoop
EXTRN	?OnCommand@CWnd@@MAEHIJ@Z:NEAR			; CWnd::OnCommand
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	?IsInvokeAllowed@CCmdTarget@@UAEHJ@Z:NEAR	; CCmdTarget::IsInvokeAllowed
EXTRN	?GetDispatchIID@CCmdTarget@@UAEHPAU_GUID@@@Z:NEAR ; CCmdTarget::GetDispatchIID
EXTRN	?GetTypeInfoCount@CCmdTarget@@UAEIXZ:NEAR	; CCmdTarget::GetTypeInfoCount
EXTRN	?GetTypeLibCache@CCmdTarget@@UAEPAVCTypeLibCache@@XZ:NEAR ; CCmdTarget::GetTypeLibCache
EXTRN	?GetTypeLib@CCmdTarget@@UAEJKPAPAUITypeLib@@@Z:NEAR ; CCmdTarget::GetTypeLib
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	?GetCommandMap@CCmdTarget@@MBEPBUAFX_OLECMDMAP@@XZ:NEAR ; CCmdTarget::GetCommandMap
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
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
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT ??_7CEnFileDialog@@6B@
; File D:\_CODE\Shared\enfiledialog.cpp
CONST	SEGMENT
??_7CEnFileDialog@@6B@ DD FLAT:?GetRuntimeClass@CEnFileDialog@@UBEPAUCRuntimeClass@@XZ ; CEnFileDialog::`vftable'
	DD	FLAT:??_ECEnFileDialog@@UAEPAXI@Z
	DD	FLAT:?Serialize@CObject@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@CDialog@@UBEXXZ
	DD	FLAT:?Dump@CFileDialog@@UBEXAAVCDumpContext@@@Z
	DD	FLAT:?OnCmdMsg@CDialog@@UAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z
	DD	FLAT:?OnFinalRelease@CWnd@@UAEXXZ
	DD	FLAT:?IsInvokeAllowed@CCmdTarget@@UAEHJ@Z
	DD	FLAT:?GetDispatchIID@CCmdTarget@@UAEHPAU_GUID@@@Z
	DD	FLAT:?GetTypeInfoCount@CCmdTarget@@UAEIXZ
	DD	FLAT:?GetTypeLibCache@CCmdTarget@@UAEPAVCTypeLibCache@@XZ
	DD	FLAT:?GetTypeLib@CCmdTarget@@UAEJKPAPAUITypeLib@@@Z
	DD	FLAT:?GetMessageMap@CEnFileDialog@@MBEPBUAFX_MSGMAP@@XZ
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
	DD	FLAT:?OnNotify@CFileDialog@@MAEHIJPAJ@Z
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
	DD	FLAT:?DoModal@CFileDialog@@UAEHXZ
	DD	FLAT:?OnInitDialog@CDialog@@UAEHXZ
	DD	FLAT:?OnSetFont@CDialog@@UAEXPAVCFont@@@Z
	DD	FLAT:?OnOK@CCommonDialog@@MAEXXZ
	DD	FLAT:?OnCancel@CCommonDialog@@MAEXXZ
	DD	FLAT:?PreInitDialog@CDialog@@MAEXXZ
	DD	FLAT:?OnShareViolation@CFileDialog@@MAEIPBG@Z
	DD	FLAT:?OnFileNameOK@CFileDialog@@MAEHXZ
	DD	FLAT:?OnLBSelChangedNotify@CFileDialog@@MAEXIII@Z
	DD	FLAT:?OnInitDone@CFileDialog@@MAEXXZ
	DD	FLAT:?OnFileNameChange@CFileDialog@@MAEXXZ
	DD	FLAT:?OnFolderChange@CFileDialog@@MAEXXZ
	DD	FLAT:?OnTypeChange@CFileDialog@@MAEXXZ
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74192	DD	019930520H
	DD	03H
	DD	FLAT:$T74196
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74196	DD	0ffffffffH
	DD	FLAT:$L74186
	DD	00H
	DD	FLAT:$L74187
	DD	01H
	DD	FLAT:$L74188
xdata$x	ENDS
;	COMDAT ??0CEnFileDialog@@QAE@HPBG00K0PAVCWnd@@@Z
_TEXT	SEGMENT
_bOpenFileDialog$ = 8
_lpszTitle$ = 12
_lpszDefExt$ = 16
_lpszFileName$ = 20
_dwFlags$ = 24
_lpszFilter$ = 28
_pParentWnd$ = 32
_this$ = -24
_dwVersion$ = -16
_dwWinMajor$ = -20
__$EHRec$ = -12
??0CEnFileDialog@@QAE@HPBG00K0PAVCWnd@@@Z PROC NEAR	; CEnFileDialog::CEnFileDialog, COMDAT

; 46   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74194
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _pParentWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpszFilter$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwFlags$[ebp]
	push	edx
	mov	eax, DWORD PTR _lpszFileName$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpszDefExt$[ebp]
	push	ecx
	mov	edx, DWORD PTR _bOpenFileDialog$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CFileDialog@@QAE@HPBG0K0PAVCWnd@@@Z	; CFileDialog::CFileDialog
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR _lpszTitle$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 832				; 00000340H
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 836				; 00000344H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CEnFileDialog@@6B@ ; CEnFileDialog::`vftable'

; 47   : #if _MSC_VER < 1300
; 48   : 	DWORD dwVersion = ::GetVersion();

	mov	esi, esp
	call	DWORD PTR __imp__GetVersion@0
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _dwVersion$[ebp], eax

; 49   : 	DWORD dwWinMajor = (DWORD)(LOBYTE(LOWORD(dwVersion)));

	mov	edx, DWORD PTR _dwVersion$[ebp]
	and	edx, 255				; 000000ffH
	mov	DWORD PTR _dwWinMajor$[ebp], edx

; 50   : 
; 51   : 	if ((BYTE)dwWinMajor >= 5)

	mov	eax, DWORD PTR _dwWinMajor$[ebp]
	and	eax, 255				; 000000ffH
	cmp	eax, 5
	jl	SHORT $L73904

; 52   : 		m_ofn.lStructSize = sizeof(OPENFILENAMEEX);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+96], 88			; 00000058H
$L73904:

; 53   : #endif		
; 54   : 
; 55   : 	ASSERT(dwFlags); // make sure caller sets this

	cmp	DWORD PTR _dwFlags$[ebp], 0
	jne	SHORT $L73909
	push	55					; 00000037H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73909
	int	3
$L73909:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L73904

; 56   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74186:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CFileDialog@@UAE@XZ			; CFileDialog::~CFileDialog
	ret	0
$L74187:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 832				; 00000340H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74188:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 836				; 00000344H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74194:
	mov	eax, OFFSET FLAT:$T74192
	jmp	___CxxFrameHandler
text$x	ENDS
??0CEnFileDialog@@QAE@HPBG00K0PAVCWnd@@@Z ENDP		; CEnFileDialog::CEnFileDialog
PUBLIC	??1CEnFileDialog@@UAE@XZ			; CEnFileDialog::~CEnFileDialog
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_GCEnFileDialog@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCEnFileDialog@@UAEPAXI@Z PROC NEAR			; CEnFileDialog::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CEnFileDialog@@UAE@XZ		; CEnFileDialog::~CEnFileDialog
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73913
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L73913:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCEnFileDialog@@UAEPAXI@Z ENDP			; CEnFileDialog::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??1CCommonDialog@@UAE@XZ			; CCommonDialog::~CCommonDialog
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74207	DD	019930520H
	DD	01H
	DD	FLAT:$T74209
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74209	DD	0ffffffffH
	DD	FLAT:$L74204
xdata$x	ENDS
;	COMDAT ??1CFileDialog@@UAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1CFileDialog@@UAE@XZ PROC NEAR			; CFileDialog::~CFileDialog, COMDAT
	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74208
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 176				; 000000b0H
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CCommonDialog@@UAE@XZ		; CCommonDialog::~CCommonDialog
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
$L74204:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CCommonDialog@@UAE@XZ		; CCommonDialog::~CCommonDialog
	ret	0
$L74208:
	mov	eax, OFFSET FLAT:$T74207
	jmp	___CxxFrameHandler
text$x	ENDS
??1CFileDialog@@UAE@XZ ENDP				; CFileDialog::~CFileDialog
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74218	DD	019930520H
	DD	02H
	DD	FLAT:$T74220
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74220	DD	0ffffffffH
	DD	FLAT:$L74214
	DD	00H
	DD	FLAT:$L74215
xdata$x	ENDS
;	COMDAT ??1CEnFileDialog@@UAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1CEnFileDialog@@UAE@XZ PROC NEAR			; CEnFileDialog::~CEnFileDialog, COMDAT
	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74219
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 836				; 00000344H
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 832				; 00000340H
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CFileDialog@@UAE@XZ			; CFileDialog::~CFileDialog
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
$L74214:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CFileDialog@@UAE@XZ			; CFileDialog::~CFileDialog
	ret	0
$L74215:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 832				; 00000340H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74219:
	mov	eax, OFFSET FLAT:$T74218
	jmp	___CxxFrameHandler
text$x	ENDS
??1CEnFileDialog@@UAE@XZ ENDP				; CEnFileDialog::~CEnFileDialog
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
EXTRN	__imp_?messageMap@CCommonDialog@@1UAFX_MSGMAP@@B:QWORD
;	COMDAT ?_GetBaseMessageMap@CEnFileDialog@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
?_GetBaseMessageMap@CEnFileDialog@@KGPBUAFX_MSGMAP@@XZ PROC NEAR ; CEnFileDialog::_GetBaseMessageMap, COMDAT

; 59   : BEGIN_MESSAGE_MAP(CEnFileDialog, CFileDialog)

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR __imp_?messageMap@CCommonDialog@@1UAFX_MSGMAP@@B
	pop	ebp
	ret	0
?_GetBaseMessageMap@CEnFileDialog@@KGPBUAFX_MSGMAP@@XZ ENDP ; CEnFileDialog::_GetBaseMessageMap
_TEXT	ENDS
;	COMDAT ?GetMessageMap@CEnFileDialog@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetMessageMap@CEnFileDialog@@MBEPBUAFX_MSGMAP@@XZ PROC NEAR ; CEnFileDialog::GetMessageMap, COMDAT

; 59   : BEGIN_MESSAGE_MAP(CEnFileDialog, CFileDialog)

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, OFFSET FLAT:?messageMap@CEnFileDialog@@1UAFX_MSGMAP@@B ; CEnFileDialog::messageMap
	mov	esp, ebp
	pop	ebp
	ret	0
?GetMessageMap@CEnFileDialog@@MBEPBUAFX_MSGMAP@@XZ ENDP	; CEnFileDialog::GetMessageMap
_TEXT	ENDS
PUBLIC	?DoModal@CEnFileDialog@@QAEHPAVIPreferences@@@Z	; CEnFileDialog::DoModal
PUBLIC	?HasInitialFileName@CEnFileDialog@@IBEHXZ	; CEnFileDialog::HasInitialFileName
PUBLIC	?GetPreferenceKey@CEnFileDialog@@IBE?AVCString@@XZ ; CEnFileDialog::GetPreferenceKey
PUBLIC	??BCOSVersion@@QAE?AW4OSVERSION@@XZ		; COSVersion::operator enum OSVERSION
EXTRN	?GetFolderFromFilePath@FileMisc@@YA?AVCString@@PBG@Z:NEAR ; FileMisc::GetFolderFromFilePath
EXTRN	?GetFileNameFromPath@FileMisc@@YA?AVCString@@PBGH@Z:NEAR ; FileMisc::GetFileNameFromPath
EXTRN	??0COSVersion@@QAE@XZ:NEAR			; COSVersion::COSVersion
EXTRN	??1COSVersion@@UAE@XZ:NEAR			; COSVersion::~COSVersion
EXTRN	__imp__GetCurrentDirectoryW@8:NEAR
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
EXTRN	?GetBuffer@CString@@QAEPAGH@Z:NEAR		; CString::GetBuffer
EXTRN	?ReleaseBuffer@CString@@QAEXH@Z:NEAR		; CString::ReleaseBuffer
EXTRN	__imp__lstrcpynW@12:NEAR
_BSS	SEGMENT
$SG73943 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT xdata$x
; File D:\_CODE\Shared\enfiledialog.cpp
xdata$x	SEGMENT
$T74248	DD	019930520H
	DD	09H
	DD	FLAT:$T74250
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74250	DD	0ffffffffH
	DD	FLAT:$L74239
	DD	0ffffffffH
	DD	FLAT:$L74240
	DD	01H
	DD	FLAT:$L74241
	DD	02H
	DD	FLAT:$L74242
	DD	02H
	DD	FLAT:$L74243
	DD	02H
	DD	FLAT:$L74244
	DD	05H
	DD	FLAT:$L74245
	DD	02H
	DD	FLAT:$L74245
	DD	02H
	DD	FLAT:$L74246
xdata$x	ENDS
;	COMDAT ?DoModal@CEnFileDialog@@QAEHPAVIPreferences@@@Z
_TEXT	SEGMENT
_pPrefs$ = 8
_this$ = -64
_sInitialFolder$ = -20
_sFilename$ = -24
_sLastFolder$73940 = -32
_szOrgFolder$ = -16
_nRet$ = -28
$T74232 = -36
$T74233 = -40
$T74234 = -44
$T74235 = -48
$T74236 = -52
$T74237 = -56
$T74238 = -60
__$EHRec$ = -12
?DoModal@CEnFileDialog@@QAEHPAVIPreferences@@@Z PROC NEAR ; CEnFileDialog::DoModal, COMDAT

; 66   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74249
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 92					; 0000005cH
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-104]
	mov	ecx, 23					; 00000017H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 67   : 	if (COSVersion() >= OSV_XPP)

	lea	ecx, DWORD PTR $T74233[ebp]
	call	??0COSVersion@@QAE@XZ			; COSVersion::COSVersion
	mov	DWORD PTR -68+[ebp], eax
	mov	eax, DWORD PTR -68+[ebp]
	mov	DWORD PTR -72+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -72+[ebp]
	call	??BCOSVersion@@QAE?AW4OSVERSION@@XZ	; COSVersion::operator enum OSVERSION
	xor	ecx, ecx
	cmp	eax, 6
	setge	cl
	mov	BYTE PTR $T74232[ebp], cl
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T74233[ebp]
	call	??1COSVersion@@UAE@XZ			; COSVersion::~COSVersion
	mov	edx, DWORD PTR $T74232[ebp]
	and	edx, 255				; 000000ffH
	test	edx, edx
	je	SHORT $L73931

; 69   : 		m_ofn.lpfnHook = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+164], 0

; 70   : 		m_ofn.Flags &= ~OFN_ENABLEHOOK;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+148]
	and	edx, -33				; ffffffdfH
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+148], edx
$L73931:

; 72   : 
; 73   : 	if (!m_sTitle.IsEmpty())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 832				; 00000340H
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L73933

; 74   : 		m_ofn.lpstrTitle = m_sTitle;

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 832				; 00000340H
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+144], eax
$L73933:

; 75   : 
; 76   : 	// if a filename has been specified, split off the folder 
; 77   : 	CString sInitialFolder, sFilename;

	lea	ecx, DWORD PTR _sInitialFolder$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sFilename$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 78   : 	
; 79   : 	if (HasInitialFileName())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?HasInitialFileName@CEnFileDialog@@IBEHXZ ; CEnFileDialog::HasInitialFileName
	test	eax, eax
	je	SHORT $L73936

; 81   : 		sFilename = FileMisc::GetFileNameFromPath(m_ofn.lpstrFile);

	push	1
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+124]
	push	eax
	lea	ecx, DWORD PTR $T74234[ebp]
	push	ecx
	call	?GetFileNameFromPath@FileMisc@@YA?AVCString@@PBGH@Z ; FileMisc::GetFileNameFromPath
	add	esp, 12					; 0000000cH
	mov	DWORD PTR -76+[ebp], eax
	mov	edx, DWORD PTR -76+[ebp]
	mov	DWORD PTR -80+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	eax, DWORD PTR -80+[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFilename$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T74234[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 82   : 		sInitialFolder = FileMisc::GetFolderFromFilePath(m_ofn.lpstrFile);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+124]
	push	edx
	lea	eax, DWORD PTR $T74235[ebp]
	push	eax
	call	?GetFolderFromFilePath@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::GetFolderFromFilePath
	add	esp, 8
	mov	DWORD PTR -84+[ebp], eax
	mov	ecx, DWORD PTR -84+[ebp]
	mov	DWORD PTR -88+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	edx, DWORD PTR -88+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sInitialFolder$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T74235[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L73936:

; 84   : 
; 85   : 	// restore last saved folder in preference to initial dir
; 86   : 	if (pPrefs && !m_sTitle.IsEmpty())

	cmp	DWORD PTR _pPrefs$[ebp], 0
	je	$L73939
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 832				; 00000340H
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L73939

; 88   : 		CString sLastFolder = pPrefs->GetProfileString(GetPreferenceKey(), m_sTitle);

	mov	esi, esp
	push	OFFSET FLAT:$SG73943
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 832				; 00000340H
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	eax, DWORD PTR $T74236[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetPreferenceKey@CEnFileDialog@@IBE?AVCString@@XZ ; CEnFileDialog::GetPreferenceKey
	mov	DWORD PTR -92+[ebp], eax
	mov	ecx, DWORD PTR -92+[ebp]
	mov	DWORD PTR -96+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	mov	ecx, DWORD PTR -96+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	edx, DWORD PTR _sLastFolder$73940[ebp]
	push	edx
	mov	eax, DWORD PTR _pPrefs$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _pPrefs$[ebp]
	call	DWORD PTR [edx+8]
	cmp	esi, esp
	call	__chkesp
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	lea	ecx, DWORD PTR $T74236[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 89   : 
; 90   : 		if (!sLastFolder.IsEmpty())

	lea	ecx, DWORD PTR _sLastFolder$73940[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L73944

; 91   : 			sInitialFolder = sLastFolder;

	lea	eax, DWORD PTR _sLastFolder$73940[ebp]
	push	eax
	lea	ecx, DWORD PTR _sInitialFolder$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
$L73944:

; 92   : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sLastFolder$73940[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L73939:

; 93   : 
; 94   : 	// save off original values and apply new ones
; 95   : 	LPCTSTR szOrgFolder = NULL;

	mov	DWORD PTR _szOrgFolder$[ebp], 0

; 96   : 
; 97   : 	if (!sInitialFolder.IsEmpty())

	lea	ecx, DWORD PTR _sInitialFolder$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L73946

; 99   : 		szOrgFolder = m_ofn.lpstrInitialDir;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+140]
	mov	DWORD PTR _szOrgFolder$[ebp], edx

; 100  : 		m_ofn.lpstrInitialDir = sInitialFolder;

	lea	ecx, DWORD PTR _sInitialFolder$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+140], eax
$L73946:

; 102  : 
; 103  : 	if (!sFilename.IsEmpty())

	lea	ecx, DWORD PTR _sFilename$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L73947

; 105  : 		// overwrite original filename
; 106  : 		lstrcpyn(m_szFileName, sFilename, (sizeof(m_szFileName) / sizeof(TCHAR)));

	mov	esi, esp
	push	260					; 00000104H
	lea	ecx, DWORD PTR _sFilename$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 308				; 00000134H
	push	edx
	call	DWORD PTR __imp__lstrcpynW@12
	cmp	esi, esp
	call	__chkesp
$L73947:

; 108  : 	
; 109  : 	int nRet = CFileDialog::DoModal();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?DoModal@CFileDialog@@UAEHXZ		; CFileDialog::DoModal
	mov	DWORD PTR _nRet$[ebp], eax

; 110  : 
; 111  : 	// restore original folder
; 112  : 	if (szOrgFolder)

	cmp	DWORD PTR _szOrgFolder$[ebp], 0
	je	SHORT $L73950

; 113  : 		m_ofn.lpstrInitialDir = szOrgFolder;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _szOrgFolder$[ebp]
	mov	DWORD PTR [eax+140], ecx
$L73950:

; 114  : 		
; 115  : 	// get and save off current working directory
; 116  : 	::GetCurrentDirectory(MAX_PATH, m_sLastFolder.GetBuffer(MAX_PATH + 1));

	push	261					; 00000105H
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 836				; 00000344H
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	mov	esi, esp
	push	eax
	push	260					; 00000104H
	call	DWORD PTR __imp__GetCurrentDirectoryW@8
	cmp	esi, esp
	call	__chkesp

; 117  : 	m_sLastFolder.ReleaseBuffer();

	push	-1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 836				; 00000344H
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 118  : 
; 119  : 	if ((nRet == IDOK) && pPrefs && !m_sTitle.IsEmpty())

	cmp	DWORD PTR _nRet$[ebp], 1
	jne	SHORT $L73951
	cmp	DWORD PTR _pPrefs$[ebp], 0
	je	SHORT $L73951
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 832				; 00000340H
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L73951

; 121  : 		pPrefs->WriteProfileString(GetPreferenceKey(), m_sTitle, m_sLastFolder);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 836				; 00000344H
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 832				; 00000340H
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	edx, DWORD PTR $T74237[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetPreferenceKey@CEnFileDialog@@IBE?AVCString@@XZ ; CEnFileDialog::GetPreferenceKey
	mov	DWORD PTR -100+[ebp], eax
	mov	eax, DWORD PTR -100+[ebp]
	mov	DWORD PTR -104+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 8
	mov	ecx, DWORD PTR -104+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _pPrefs$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _pPrefs$[ebp]
	call	DWORD PTR [edx+12]
	cmp	esi, esp
	call	__chkesp
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T74237[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L73951:

; 123  : 
; 124  : 	return nRet;

	mov	eax, DWORD PTR _nRet$[ebp]
	mov	DWORD PTR $T74238[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sFilename$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sInitialFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74238[ebp]

; 125  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 104				; 00000068H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74239:
	lea	ecx, DWORD PTR $T74233[ebp]
	call	??1COSVersion@@UAE@XZ			; COSVersion::~COSVersion
	ret	0
$L74240:
	lea	ecx, DWORD PTR _sInitialFolder$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74241:
	lea	ecx, DWORD PTR _sFilename$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74242:
	lea	ecx, DWORD PTR $T74234[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74243:
	lea	ecx, DWORD PTR $T74235[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74244:
	lea	ecx, DWORD PTR $T74236[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74245:
	lea	ecx, DWORD PTR _sLastFolder$73940[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74246:
	lea	ecx, DWORD PTR $T74237[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74249:
	mov	eax, OFFSET FLAT:$T74248
	jmp	___CxxFrameHandler
text$x	ENDS
?DoModal@CEnFileDialog@@QAEHPAVIPreferences@@@Z ENDP	; CEnFileDialog::DoModal
EXTRN	?GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ:NEAR ; COSVersion::GetOSVersion
;	COMDAT ??BCOSVersion@@QAE?AW4OSVERSION@@XZ
_TEXT	SEGMENT
_this$ = -4
??BCOSVersion@@QAE?AW4OSVERSION@@XZ PROC NEAR		; COSVersion::operator enum OSVERSION, COMDAT

; 35   : 	operator OSVERSION() { return GetOSVersion(); }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetOSVersion@COSVersion@@IAE?AW4OSVERSION@@XZ ; COSVersion::GetOSVersion
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??BCOSVersion@@QAE?AW4OSVERSION@@XZ ENDP		; COSVersion::operator enum OSVERSION
_TEXT	ENDS
;	COMDAT data
; File D:\_CODE\Shared\enfiledialog.cpp
data	SEGMENT
$SG73958 DB	'O', 00H, 'p', 00H, 'e', 00H, 'n', 00H, 'F', 00H, 'i', 00H
	DB	'l', 00H, 'e', 00H, 'D', 00H, 'i', 00H, 'a', 00H, 'l', 00H, 'o'
	DB	00H, 'g', 00H, 00H, 00H
	ORG $+2
$SG73959 DB	'S', 00H, 'a', 00H, 'v', 00H, 'e', 00H, 'F', 00H, 'i', 00H
	DB	'l', 00H, 'e', 00H, 'D', 00H, 'i', 00H, 'a', 00H, 'l', 00H, 'o'
	DB	00H, 'g', 00H, 00H, 00H
data	ENDS
;	COMDAT ?GetPreferenceKey@CEnFileDialog@@IBE?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_this$ = -8
$T74260 = -4
?GetPreferenceKey@CEnFileDialog@@IBE?AVCString@@XZ PROC NEAR ; CEnFileDialog::GetPreferenceKey, COMDAT

; 128  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T74260[ebp], 0

; 129  : 	return (m_bOpenFileDialog ? _T("OpenFileDialog") : _T("SaveFileDialog"));

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+172], 0
	je	SHORT $L74257
	mov	DWORD PTR -12+[ebp], OFFSET FLAT:$SG73958
	jmp	SHORT $L74258
$L74257:
	mov	DWORD PTR -12+[ebp], OFFSET FLAT:$SG73959
$L74258:
	mov	ecx, DWORD PTR -12+[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	edx, DWORD PTR $T74260[ebp]
	or	edx, 1
	mov	DWORD PTR $T74260[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 130  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetPreferenceKey@CEnFileDialog@@IBE?AVCString@@XZ ENDP	; CEnFileDialog::GetPreferenceKey
_TEXT	ENDS
PUBLIC	?SetTitle@CEnFileDialog@@QAEXPBG@Z		; CEnFileDialog::SetTitle
EXTRN	??4CString@@QAEABV0@PBG@Z:NEAR			; CString::operator=
;	COMDAT ?SetTitle@CEnFileDialog@@QAEXPBG@Z
_TEXT	SEGMENT
_szTitle$ = 8
_this$ = -4
?SetTitle@CEnFileDialog@@QAEXPBG@Z PROC NEAR		; CEnFileDialog::SetTitle, COMDAT

; 133  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 134  : 	m_sTitle = szTitle;

	mov	eax, DWORD PTR _szTitle$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 832				; 00000340H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 135  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?SetTitle@CEnFileDialog@@QAEXPBG@Z ENDP			; CEnFileDialog::SetTitle
_TEXT	ENDS
;	COMDAT xdata$x
; File D:\_CODE\Shared\enfiledialog.cpp
xdata$x	SEGMENT
$T74271	DD	019930520H
	DD	01H
	DD	FLAT:$T74273
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74273	DD	0ffffffffH
	DD	FLAT:$L74269
xdata$x	ENDS
;	COMDAT ?HasInitialFileName@CEnFileDialog@@IBEHXZ
_TEXT	SEGMENT
_this$ = -24
$T74267 = -16
$T74268 = -20
__$EHRec$ = -12
?HasInitialFileName@CEnFileDialog@@IBEHXZ PROC NEAR	; CEnFileDialog::HasInitialFileName, COMDAT

; 138  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74272
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 20					; 00000014H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 139  : 	return !CString(m_ofn.lpstrFile).IsEmpty();

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+124]
	push	ecx
	lea	ecx, DWORD PTR $T74268[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR -28+[ebp], eax
	mov	edx, DWORD PTR -28+[ebp]
	mov	DWORD PTR -32+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -32+[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR $T74267[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T74268[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74267[ebp]

; 140  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74269:
	lea	ecx, DWORD PTR $T74268[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74272:
	mov	eax, OFFSET FLAT:$T74271
	jmp	___CxxFrameHandler
text$x	ENDS
?HasInitialFileName@CEnFileDialog@@IBEHXZ ENDP		; CEnFileDialog::HasInitialFileName
PUBLIC	?HasInitialFolder@CEnFileDialog@@IBEHXZ		; CEnFileDialog::HasInitialFolder
;	COMDAT xdata$x
; File D:\_CODE\Shared\enfiledialog.cpp
xdata$x	SEGMENT
$T74282	DD	019930520H
	DD	01H
	DD	FLAT:$T74284
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74284	DD	0ffffffffH
	DD	FLAT:$L74280
xdata$x	ENDS
;	COMDAT ?HasInitialFolder@CEnFileDialog@@IBEHXZ
_TEXT	SEGMENT
_this$ = -24
$T74278 = -16
$T74279 = -20
__$EHRec$ = -12
?HasInitialFolder@CEnFileDialog@@IBEHXZ PROC NEAR	; CEnFileDialog::HasInitialFolder, COMDAT

; 143  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74283
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 20					; 00000014H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 144  : 	return !CString(m_ofn.lpstrInitialDir).IsEmpty();

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+140]
	push	ecx
	lea	ecx, DWORD PTR $T74279[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR -28+[ebp], eax
	mov	edx, DWORD PTR -28+[ebp]
	mov	DWORD PTR -32+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -32+[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR $T74278[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T74279[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74278[ebp]

; 145  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74280:
	lea	ecx, DWORD PTR $T74279[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74283:
	mov	eax, OFFSET FLAT:$T74282
	jmp	___CxxFrameHandler
text$x	ENDS
?HasInitialFolder@CEnFileDialog@@IBEHXZ ENDP		; CEnFileDialog::HasInitialFolder
;	COMDAT ?_GetBaseClass@CFileOpenDialog@@KGPAUCRuntimeClass@@XZ
_TEXT	SEGMENT
?_GetBaseClass@CFileOpenDialog@@KGPAUCRuntimeClass@@XZ PROC NEAR ; CFileOpenDialog::_GetBaseClass, COMDAT

; 150  : IMPLEMENT_DYNAMIC(CFileOpenDialog, CEnFileDialog)

	push	ebp
	mov	ebp, esp
	mov	eax, OFFSET FLAT:?classCEnFileDialog@CEnFileDialog@@2UCRuntimeClass@@B ; CEnFileDialog::classCEnFileDialog
	pop	ebp
	ret	0
?_GetBaseClass@CFileOpenDialog@@KGPAUCRuntimeClass@@XZ ENDP ; CFileOpenDialog::_GetBaseClass
_TEXT	ENDS
PUBLIC	?GetRuntimeClass@CFileOpenDialog@@UBEPAUCRuntimeClass@@XZ ; CFileOpenDialog::GetRuntimeClass
;	COMDAT ?GetRuntimeClass@CFileOpenDialog@@UBEPAUCRuntimeClass@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetRuntimeClass@CFileOpenDialog@@UBEPAUCRuntimeClass@@XZ PROC NEAR ; CFileOpenDialog::GetRuntimeClass, COMDAT

; 150  : IMPLEMENT_DYNAMIC(CFileOpenDialog, CEnFileDialog)

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, OFFSET FLAT:?classCFileOpenDialog@CFileOpenDialog@@2UCRuntimeClass@@B ; CFileOpenDialog::classCFileOpenDialog
	mov	esp, ebp
	pop	ebp
	ret	0
?GetRuntimeClass@CFileOpenDialog@@UBEPAUCRuntimeClass@@XZ ENDP ; CFileOpenDialog::GetRuntimeClass
_TEXT	ENDS
PUBLIC	??0CFileOpenDialog@@QAE@PBG00K0PAVCWnd@@@Z	; CFileOpenDialog::CFileOpenDialog
PUBLIC	??_7CFileOpenDialog@@6B@			; CFileOpenDialog::`vftable'
PUBLIC	??_GCFileOpenDialog@@UAEPAXI@Z			; CFileOpenDialog::`scalar deleting destructor'
PUBLIC	??_ECFileOpenDialog@@UAEPAXI@Z			; CFileOpenDialog::`vector deleting destructor'
;	COMDAT ??_7CFileOpenDialog@@6B@
; File D:\_CODE\Shared\enfiledialog.cpp
CONST	SEGMENT
??_7CFileOpenDialog@@6B@ DD FLAT:?GetRuntimeClass@CFileOpenDialog@@UBEPAUCRuntimeClass@@XZ ; CFileOpenDialog::`vftable'
	DD	FLAT:??_ECFileOpenDialog@@UAEPAXI@Z
	DD	FLAT:?Serialize@CObject@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@CDialog@@UBEXXZ
	DD	FLAT:?Dump@CFileDialog@@UBEXAAVCDumpContext@@@Z
	DD	FLAT:?OnCmdMsg@CDialog@@UAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z
	DD	FLAT:?OnFinalRelease@CWnd@@UAEXXZ
	DD	FLAT:?IsInvokeAllowed@CCmdTarget@@UAEHJ@Z
	DD	FLAT:?GetDispatchIID@CCmdTarget@@UAEHPAU_GUID@@@Z
	DD	FLAT:?GetTypeInfoCount@CCmdTarget@@UAEIXZ
	DD	FLAT:?GetTypeLibCache@CCmdTarget@@UAEPAVCTypeLibCache@@XZ
	DD	FLAT:?GetTypeLib@CCmdTarget@@UAEJKPAPAUITypeLib@@@Z
	DD	FLAT:?GetMessageMap@CEnFileDialog@@MBEPBUAFX_MSGMAP@@XZ
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
	DD	FLAT:?OnNotify@CFileDialog@@MAEHIJPAJ@Z
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
	DD	FLAT:?DoModal@CFileDialog@@UAEHXZ
	DD	FLAT:?OnInitDialog@CDialog@@UAEHXZ
	DD	FLAT:?OnSetFont@CDialog@@UAEXPAVCFont@@@Z
	DD	FLAT:?OnOK@CCommonDialog@@MAEXXZ
	DD	FLAT:?OnCancel@CCommonDialog@@MAEXXZ
	DD	FLAT:?PreInitDialog@CDialog@@MAEXXZ
	DD	FLAT:?OnShareViolation@CFileDialog@@MAEIPBG@Z
	DD	FLAT:?OnFileNameOK@CFileDialog@@MAEHXZ
	DD	FLAT:?OnLBSelChangedNotify@CFileDialog@@MAEXIII@Z
	DD	FLAT:?OnInitDone@CFileDialog@@MAEXXZ
	DD	FLAT:?OnFileNameChange@CFileDialog@@MAEXXZ
	DD	FLAT:?OnFolderChange@CFileDialog@@MAEXXZ
	DD	FLAT:?OnTypeChange@CFileDialog@@MAEXXZ
CONST	ENDS
;	COMDAT ??0CFileOpenDialog@@QAE@PBG00K0PAVCWnd@@@Z
_TEXT	SEGMENT
_lpszTitle$ = 8
_lpszDefExt$ = 12
_lpszFileName$ = 16
_dwFlags$ = 20
_lpszFilter$ = 24
_pParentWnd$ = 28
_this$ = -4
??0CFileOpenDialog@@QAE@PBG00K0PAVCWnd@@@Z PROC NEAR	; CFileOpenDialog::CFileOpenDialog, COMDAT

; 160  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _pParentWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpszFilter$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwFlags$[ebp]
	push	edx
	mov	eax, DWORD PTR _lpszFileName$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpszDefExt$[ebp]
	push	ecx
	mov	edx, DWORD PTR _lpszTitle$[ebp]
	push	edx
	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CEnFileDialog@@QAE@HPBG00K0PAVCWnd@@@Z ; CEnFileDialog::CEnFileDialog
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CFileOpenDialog@@6B@ ; CFileOpenDialog::`vftable'

; 161  : 	// nothing else to do
; 162  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	24					; 00000018H
??0CFileOpenDialog@@QAE@PBG00K0PAVCWnd@@@Z ENDP		; CFileOpenDialog::CFileOpenDialog
_TEXT	ENDS
PUBLIC	??1CFileOpenDialog@@UAE@XZ			; CFileOpenDialog::~CFileOpenDialog
;	COMDAT ??_GCFileOpenDialog@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCFileOpenDialog@@UAEPAXI@Z PROC NEAR		; CFileOpenDialog::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CFileOpenDialog@@UAE@XZ		; CFileOpenDialog::~CFileOpenDialog
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73995
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L73995:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCFileOpenDialog@@UAEPAXI@Z ENDP			; CFileOpenDialog::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CFileOpenDialog@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CFileOpenDialog@@UAE@XZ PROC NEAR			; CFileOpenDialog::~CFileOpenDialog, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CEnFileDialog@@UAE@XZ		; CEnFileDialog::~CEnFileDialog
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CFileOpenDialog@@UAE@XZ ENDP				; CFileOpenDialog::~CFileOpenDialog
_TEXT	ENDS
PUBLIC	??0CFileOpenDialog@@QAE@IPBG0K0PAVCWnd@@@Z	; CFileOpenDialog::CFileOpenDialog
EXTRN	??0CEnString@@QAE@IPBG@Z:NEAR			; CEnString::CEnString
EXTRN	??1CEnString@@QAE@XZ:NEAR			; CEnString::~CEnString
;	COMDAT xdata$x
; File D:\_CODE\Shared\enfiledialog.cpp
xdata$x	SEGMENT
$T74305	DD	019930520H
	DD	03H
	DD	FLAT:$T74307
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74307	DD	0ffffffffH
	DD	FLAT:$L74302
	DD	00H
	DD	FLAT:$L74303
	DD	0ffffffffH
	DD	FLAT:$L74303
xdata$x	ENDS
;	COMDAT ??0CFileOpenDialog@@QAE@IPBG0K0PAVCWnd@@@Z
_TEXT	SEGMENT
_nIDTitle$ = 8
_lpszDefExt$ = 12
_lpszFileName$ = 16
_dwFlags$ = 20
_lpszFilter$ = 24
_pParentWnd$ = 28
_this$ = -20
$T74301 = -16
__$EHRec$ = -12
??0CFileOpenDialog@@QAE@IPBG0K0PAVCWnd@@@Z PROC NEAR	; CFileOpenDialog::CFileOpenDialog, COMDAT

; 172  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74306
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
	mov	eax, DWORD PTR _pParentWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpszFilter$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwFlags$[ebp]
	push	edx
	mov	eax, DWORD PTR _lpszFileName$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpszDefExt$[ebp]
	push	ecx
	push	0
	mov	edx, DWORD PTR _nIDTitle$[ebp]
	push	edx
	lea	ecx, DWORD PTR $T74301[ebp]
	call	??0CEnString@@QAE@IPBG@Z		; CEnString::CEnString
	mov	DWORD PTR -24+[ebp], eax
	mov	eax, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -28+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CEnFileDialog@@QAE@HPBG00K0PAVCWnd@@@Z ; CEnFileDialog::CEnFileDialog
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T74301[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CFileOpenDialog@@6B@ ; CFileOpenDialog::`vftable'

; 173  : 	// nothing else to do
; 174  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	24					; 00000018H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74302:
	lea	ecx, DWORD PTR $T74301[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	ret	0
$L74303:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CEnFileDialog@@UAE@XZ		; CEnFileDialog::~CEnFileDialog
	ret	0
$L74306:
	mov	eax, OFFSET FLAT:$T74305
	jmp	___CxxFrameHandler
text$x	ENDS
??0CFileOpenDialog@@QAE@IPBG0K0PAVCWnd@@@Z ENDP		; CFileOpenDialog::CFileOpenDialog
;	COMDAT ?_GetBaseClass@CFileSaveDialog@@KGPAUCRuntimeClass@@XZ
_TEXT	SEGMENT
?_GetBaseClass@CFileSaveDialog@@KGPAUCRuntimeClass@@XZ PROC NEAR ; CFileSaveDialog::_GetBaseClass, COMDAT

; 180  : IMPLEMENT_DYNAMIC(CFileSaveDialog, CEnFileDialog)

	push	ebp
	mov	ebp, esp
	mov	eax, OFFSET FLAT:?classCEnFileDialog@CEnFileDialog@@2UCRuntimeClass@@B ; CEnFileDialog::classCEnFileDialog
	pop	ebp
	ret	0
?_GetBaseClass@CFileSaveDialog@@KGPAUCRuntimeClass@@XZ ENDP ; CFileSaveDialog::_GetBaseClass
_TEXT	ENDS
PUBLIC	?GetRuntimeClass@CFileSaveDialog@@UBEPAUCRuntimeClass@@XZ ; CFileSaveDialog::GetRuntimeClass
;	COMDAT ?GetRuntimeClass@CFileSaveDialog@@UBEPAUCRuntimeClass@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetRuntimeClass@CFileSaveDialog@@UBEPAUCRuntimeClass@@XZ PROC NEAR ; CFileSaveDialog::GetRuntimeClass, COMDAT

; 180  : IMPLEMENT_DYNAMIC(CFileSaveDialog, CEnFileDialog)

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, OFFSET FLAT:?classCFileSaveDialog@CFileSaveDialog@@2UCRuntimeClass@@B ; CFileSaveDialog::classCFileSaveDialog
	mov	esp, ebp
	pop	ebp
	ret	0
?GetRuntimeClass@CFileSaveDialog@@UBEPAUCRuntimeClass@@XZ ENDP ; CFileSaveDialog::GetRuntimeClass
_TEXT	ENDS
PUBLIC	??0CFileSaveDialog@@QAE@PBG00K0PAVCWnd@@@Z	; CFileSaveDialog::CFileSaveDialog
PUBLIC	??_7CFileSaveDialog@@6B@			; CFileSaveDialog::`vftable'
PUBLIC	??_GCFileSaveDialog@@UAEPAXI@Z			; CFileSaveDialog::`scalar deleting destructor'
PUBLIC	??_ECFileSaveDialog@@UAEPAXI@Z			; CFileSaveDialog::`vector deleting destructor'
;	COMDAT ??_7CFileSaveDialog@@6B@
; File D:\_CODE\Shared\enfiledialog.cpp
CONST	SEGMENT
??_7CFileSaveDialog@@6B@ DD FLAT:?GetRuntimeClass@CFileSaveDialog@@UBEPAUCRuntimeClass@@XZ ; CFileSaveDialog::`vftable'
	DD	FLAT:??_ECFileSaveDialog@@UAEPAXI@Z
	DD	FLAT:?Serialize@CObject@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@CDialog@@UBEXXZ
	DD	FLAT:?Dump@CFileDialog@@UBEXAAVCDumpContext@@@Z
	DD	FLAT:?OnCmdMsg@CDialog@@UAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z
	DD	FLAT:?OnFinalRelease@CWnd@@UAEXXZ
	DD	FLAT:?IsInvokeAllowed@CCmdTarget@@UAEHJ@Z
	DD	FLAT:?GetDispatchIID@CCmdTarget@@UAEHPAU_GUID@@@Z
	DD	FLAT:?GetTypeInfoCount@CCmdTarget@@UAEIXZ
	DD	FLAT:?GetTypeLibCache@CCmdTarget@@UAEPAVCTypeLibCache@@XZ
	DD	FLAT:?GetTypeLib@CCmdTarget@@UAEJKPAPAUITypeLib@@@Z
	DD	FLAT:?GetMessageMap@CEnFileDialog@@MBEPBUAFX_MSGMAP@@XZ
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
	DD	FLAT:?OnNotify@CFileDialog@@MAEHIJPAJ@Z
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
	DD	FLAT:?DoModal@CFileDialog@@UAEHXZ
	DD	FLAT:?OnInitDialog@CDialog@@UAEHXZ
	DD	FLAT:?OnSetFont@CDialog@@UAEXPAVCFont@@@Z
	DD	FLAT:?OnOK@CCommonDialog@@MAEXXZ
	DD	FLAT:?OnCancel@CCommonDialog@@MAEXXZ
	DD	FLAT:?PreInitDialog@CDialog@@MAEXXZ
	DD	FLAT:?OnShareViolation@CFileDialog@@MAEIPBG@Z
	DD	FLAT:?OnFileNameOK@CFileDialog@@MAEHXZ
	DD	FLAT:?OnLBSelChangedNotify@CFileDialog@@MAEXIII@Z
	DD	FLAT:?OnInitDone@CFileDialog@@MAEXXZ
	DD	FLAT:?OnFileNameChange@CFileDialog@@MAEXXZ
	DD	FLAT:?OnFolderChange@CFileDialog@@MAEXXZ
	DD	FLAT:?OnTypeChange@CFileDialog@@MAEXXZ
CONST	ENDS
;	COMDAT ??0CFileSaveDialog@@QAE@PBG00K0PAVCWnd@@@Z
_TEXT	SEGMENT
_lpszTitle$ = 8
_lpszDefExt$ = 12
_lpszFileName$ = 16
_dwFlags$ = 20
_lpszFilter$ = 24
_pParentWnd$ = 28
_this$ = -4
??0CFileSaveDialog@@QAE@PBG00K0PAVCWnd@@@Z PROC NEAR	; CFileSaveDialog::CFileSaveDialog, COMDAT

; 190  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _pParentWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpszFilter$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwFlags$[ebp]
	push	edx
	mov	eax, DWORD PTR _lpszFileName$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpszDefExt$[ebp]
	push	ecx
	mov	edx, DWORD PTR _lpszTitle$[ebp]
	push	edx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CEnFileDialog@@QAE@HPBG00K0PAVCWnd@@@Z ; CEnFileDialog::CEnFileDialog
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CFileSaveDialog@@6B@ ; CFileSaveDialog::`vftable'

; 191  : 	// nothing else to do
; 192  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	24					; 00000018H
??0CFileSaveDialog@@QAE@PBG00K0PAVCWnd@@@Z ENDP		; CFileSaveDialog::CFileSaveDialog
_TEXT	ENDS
PUBLIC	??1CFileSaveDialog@@UAE@XZ			; CFileSaveDialog::~CFileSaveDialog
;	COMDAT ??_GCFileSaveDialog@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCFileSaveDialog@@UAEPAXI@Z PROC NEAR		; CFileSaveDialog::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CFileSaveDialog@@UAE@XZ		; CFileSaveDialog::~CFileSaveDialog
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L74029
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L74029:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCFileSaveDialog@@UAEPAXI@Z ENDP			; CFileSaveDialog::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CFileSaveDialog@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CFileSaveDialog@@UAE@XZ PROC NEAR			; CFileSaveDialog::~CFileSaveDialog, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CEnFileDialog@@UAE@XZ		; CEnFileDialog::~CEnFileDialog
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CFileSaveDialog@@UAE@XZ ENDP				; CFileSaveDialog::~CFileSaveDialog
_TEXT	ENDS
PUBLIC	??0CFileSaveDialog@@QAE@IPBG0K0PAVCWnd@@@Z	; CFileSaveDialog::CFileSaveDialog
;	COMDAT xdata$x
; File D:\_CODE\Shared\enfiledialog.cpp
xdata$x	SEGMENT
$T74328	DD	019930520H
	DD	03H
	DD	FLAT:$T74330
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74330	DD	0ffffffffH
	DD	FLAT:$L74325
	DD	00H
	DD	FLAT:$L74326
	DD	0ffffffffH
	DD	FLAT:$L74326
xdata$x	ENDS
;	COMDAT ??0CFileSaveDialog@@QAE@IPBG0K0PAVCWnd@@@Z
_TEXT	SEGMENT
_nIDTitle$ = 8
_lpszDefExt$ = 12
_lpszFileName$ = 16
_dwFlags$ = 20
_lpszFilter$ = 24
_pParentWnd$ = 28
_this$ = -20
$T74324 = -16
__$EHRec$ = -12
??0CFileSaveDialog@@QAE@IPBG0K0PAVCWnd@@@Z PROC NEAR	; CFileSaveDialog::CFileSaveDialog, COMDAT

; 202  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74329
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
	mov	eax, DWORD PTR _pParentWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpszFilter$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwFlags$[ebp]
	push	edx
	mov	eax, DWORD PTR _lpszFileName$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpszDefExt$[ebp]
	push	ecx
	push	0
	mov	edx, DWORD PTR _nIDTitle$[ebp]
	push	edx
	lea	ecx, DWORD PTR $T74324[ebp]
	call	??0CEnString@@QAE@IPBG@Z		; CEnString::CEnString
	mov	DWORD PTR -24+[ebp], eax
	mov	eax, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -28+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CEnFileDialog@@QAE@HPBG00K0PAVCWnd@@@Z ; CEnFileDialog::CEnFileDialog
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T74324[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CFileSaveDialog@@6B@ ; CFileSaveDialog::`vftable'

; 203  : 	// nothing else to do
; 204  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	24					; 00000018H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74325:
	lea	ecx, DWORD PTR $T74324[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	ret	0
$L74326:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CEnFileDialog@@UAE@XZ		; CEnFileDialog::~CEnFileDialog
	ret	0
$L74329:
	mov	eax, OFFSET FLAT:$T74328
	jmp	___CxxFrameHandler
text$x	ENDS
??0CFileSaveDialog@@QAE@IPBG0K0PAVCWnd@@@Z ENDP		; CFileSaveDialog::CFileSaveDialog
END

	TITLE	D:\_CODE\Shared\entoolbar.cpp
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
;	COMDAT ??_C@_0BP@DDHI@D?3?2_CODE?2Shared?2graphicsmisc?4h?$AA@
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
text$AFX_COL1	SEGMENT PARA USE32 PUBLIC ''
text$AFX_COL1	ENDS
;	COMDAT ?OnSizeParent@CEnToolBar@@IAEJIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnItemPrePaint@CEnToolBar@@MAEJPAU_NMTBCUSTOMDRAW@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnItemPostPaint@CEnToolBar@@MAEJPAU_NMTBCUSTOMDRAW@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HasBkgndColor@CEnToolBar@@IBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0RGBX@@QAE@EEE@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4RGBX@@QAEABV0@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Luminance@RGBX@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MakeGray@RGBX@@QAEXNNN@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Hue2Triplet@RGBX@@KAEMMM@Z
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
;	COMDAT ??1CImageSysColorMapper@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCImageSysColorMapper@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CImageEmbosser@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCImageEmbosser@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??BCOSVersion@@QAE?AW4OSVERSION@@XZ
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
;	COMDAT ??0CEnToolBar@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCEnToolBar@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CEnToolBar@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_GetBaseMessageMap@CEnToolBar@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMessageMap@CEnToolBar@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadToolBar@CEnToolBar@@QAEHPBG0K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadToolBar@CEnToolBar@@QAEHIPBGK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadToolBar@CEnToolBar@@QAEHIIK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetImage@CEnToolBar@@QAEHIK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetImage@CEnToolBar@@QAEHABVCString@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetImage@CEnToolBar@@IAEHPAVCEnBitmapEx@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetButtonCount@CEnToolBar@@QBEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetBackgroundColors@CEnToolBar@@QAEXKKHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RefreshButtonStates@CEnToolBar@@QAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnRefreshButtonStates@CEnToolBar@@IAEJIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RefreshDisabledImageList@CEnToolBar@@IAEXPAVCEnBitmapEx@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnCustomDraw@CEnToolBar@@IAEXPAUtagNMHDR@@PAJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnNcPaint@CEnToolBar@@IAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawBkgnd@CEnToolBar@@IAEXPAVCDC@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnSettingChange@CEnToolBar@@IAEXIPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EstimateHeightRequired@CEnToolBar@@IBEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMinReqLength@CEnToolBar@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EstimateRowsRequired@CEnToolBar@@IBEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RefreshRowHeights@CEnToolBar@@IAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Resize@CEnToolBar@@QAEHHVCPoint@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetHeight@CEnToolBar@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnDestroy@CEnToolBar@@IAEXXZ
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
;	COMDAT ??_7CEnToolBar@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CImageEmbosser@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CImageSysColorMapper@@6B@
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
PUBLIC	?OnCustomDraw@CEnToolBar@@IAEXPAUtagNMHDR@@PAJ@Z ; CEnToolBar::OnCustomDraw
PUBLIC	?OnSettingChange@CEnToolBar@@IAEXIPBG@Z		; CEnToolBar::OnSettingChange
PUBLIC	?OnRefreshButtonStates@CEnToolBar@@IAEJIJ@Z	; CEnToolBar::OnRefreshButtonStates
PUBLIC	?OnSizeParent@CEnToolBar@@IAEJIJ@Z		; CEnToolBar::OnSizeParent
PUBLIC	?OnDestroy@CEnToolBar@@IAEXXZ			; CEnToolBar::OnDestroy
PUBLIC	?OnNcPaint@CEnToolBar@@IAEXXZ			; CEnToolBar::OnNcPaint
PUBLIC	?_messageEntries@CEnToolBar@@0QBUAFX_MSGMAP_ENTRY@@B ; CEnToolBar::_messageEntries
PUBLIC	?messageMap@CEnToolBar@@1UAFX_MSGMAP@@B		; CEnToolBar::messageMap
PUBLIC	?_GetBaseMessageMap@CEnToolBar@@KGPBUAFX_MSGMAP@@XZ ; CEnToolBar::_GetBaseMessageMap
_DATA	SEGMENT
__szAfxTempl DB	'afxtempl.h', 00H
	ORG $+1
_THIS_FILE DB	'D:\_CODE\Shared\entoolbar.cpp', 00H
_DATA	ENDS
CONST	SEGMENT
?messageMap@CEnToolBar@@1UAFX_MSGMAP@@B DD FLAT:?_GetBaseMessageMap@CEnToolBar@@KGPBUAFX_MSGMAP@@XZ ; CEnToolBar::messageMap
	DD	FLAT:?_messageEntries@CEnToolBar@@0QBUAFX_MSGMAP_ENTRY@@B
?_messageEntries@CEnToolBar@@0QBUAFX_MSGMAP_ENTRY@@B DD 0bc4eH ; CEnToolBar::_messageEntries
	DD	0fff4H
	DD	00H
	DD	00H
	DD	026H
	DD	FLAT:?OnCustomDraw@CEnToolBar@@IAEXPAUtagNMHDR@@PAJ@Z
	DD	01aH
	DD	00H
	DD	00H
	DD	00H
	DD	02fH
	DD	FLAT:?OnSettingChange@CEnToolBar@@IAEXIPBG@Z
	DD	08001H
	DD	00H
	DD	00H
	DD	00H
	DD	0aH
	DD	FLAT:?OnRefreshButtonStates@CEnToolBar@@IAEJIJ@Z
	DD	0361H
	DD	00H
	DD	00H
	DD	00H
	DD	0aH
	DD	FLAT:?OnSizeParent@CEnToolBar@@IAEJIJ@Z
	DD	02H
	DD	00H
	DD	00H
	DD	00H
	DD	0cH
	DD	FLAT:?OnDestroy@CEnToolBar@@IAEXXZ
	DD	085H
	DD	00H
	DD	00H
	DD	00H
	DD	0cH
	DD	FLAT:?OnNcPaint@CEnToolBar@@IAEXXZ
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
CONST	ENDS
;	COMDAT ?OnSizeParent@CEnToolBar@@IAEJIJ@Z
_TEXT	SEGMENT
_this$ = -4
?OnSizeParent@CEnToolBar@@IAEJIJ@Z PROC NEAR		; CEnToolBar::OnSizeParent, COMDAT

; 59   : 	afx_msg LRESULT OnSizeParent(WPARAM /*wParam*/, LPARAM /*lParam*/) { return 0L; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	xor	eax, eax
	mov	esp, ebp
	pop	ebp
	ret	8
?OnSizeParent@CEnToolBar@@IAEJIJ@Z ENDP			; CEnToolBar::OnSizeParent
_TEXT	ENDS
PUBLIC	??0CEnToolBar@@QAE@XZ				; CEnToolBar::CEnToolBar
PUBLIC	?GetMessageMap@CEnToolBar@@MBEPBUAFX_MSGMAP@@XZ	; CEnToolBar::GetMessageMap
PUBLIC	?OnItemPrePaint@CEnToolBar@@MAEJPAU_NMTBCUSTOMDRAW@@@Z ; CEnToolBar::OnItemPrePaint
PUBLIC	?OnItemPostPaint@CEnToolBar@@MAEJPAU_NMTBCUSTOMDRAW@@@Z ; CEnToolBar::OnItemPostPaint
PUBLIC	??_7CEnToolBar@@6B@				; CEnToolBar::`vftable'
PUBLIC	??_GCEnToolBar@@UAEPAXI@Z			; CEnToolBar::`scalar deleting destructor'
PUBLIC	??_ECEnToolBar@@UAEPAXI@Z			; CEnToolBar::`vector deleting destructor'
EXTRN	?PreSubclassWindow@CWnd@@UAEXXZ:NEAR		; CWnd::PreSubclassWindow
EXTRN	?PreTranslateMessage@CControlBar@@UAEHPAUtagMSG@@@Z:NEAR ; CControlBar::PreTranslateMessage
EXTRN	?PreCreateWindow@CControlBar@@UAEHAAUtagCREATESTRUCTW@@@Z:NEAR ; CControlBar::PreCreateWindow
EXTRN	?PostNcDestroy@CControlBar@@UAEXXZ:NEAR		; CControlBar::PostNcDestroy
EXTRN	?DoPaint@CControlBar@@UAEXPAVCDC@@@Z:NEAR	; CControlBar::DoPaint
EXTRN	?Create@CWnd@@UAEHPBG0KABUtagRECT@@PAV1@IPAUCCreateContext@@@Z:NEAR ; CWnd::Create
EXTRN	?WindowProc@CControlBar@@UAEJIIJ@Z:NEAR		; CControlBar::WindowProc
EXTRN	?SetStatusText@CControlBar@@UAEHH@Z:NEAR	; CControlBar::SetStatusText
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	?CalcWindowRect@CWnd@@UAEXPAUtagRECT@@I@Z:NEAR	; CWnd::CalcWindowRect
EXTRN	?GetRuntimeClass@CToolBar@@UBEPAUCRuntimeClass@@XZ:NEAR ; CToolBar::GetRuntimeClass
EXTRN	??0CToolBar@@QAE@XZ:NEAR			; CToolBar::CToolBar
EXTRN	?GetItemRect@CToolBar@@UBEXHPAUtagRECT@@@Z:NEAR	; CToolBar::GetItemRect
EXTRN	??1CToolBar@@UAE@XZ:NEAR			; CToolBar::~CToolBar
EXTRN	?CalcFixedLayout@CToolBar@@UAE?AVCSize@@HH@Z:NEAR ; CToolBar::CalcFixedLayout
EXTRN	?CalcDynamicLayout@CToolBar@@UAE?AVCSize@@HK@Z:NEAR ; CToolBar::CalcDynamicLayout
EXTRN	?OnToolHitTest@CToolBar@@UBEHVCPoint@@PAUtagTOOLINFOW@@@Z:NEAR ; CToolBar::OnToolHitTest
EXTRN	?OnUpdateCmdUI@CToolBar@@UAEXPAVCFrameWnd@@H@Z:NEAR ; CToolBar::OnUpdateCmdUI
EXTRN	?OnBarStyleChange@CToolBar@@UAEXKK@Z:NEAR	; CToolBar::OnBarStyleChange
EXTRN	?AssertValid@CToolBar@@UBEXXZ:NEAR		; CToolBar::AssertValid
EXTRN	?Dump@CToolBar@@UBEXAAVCDumpContext@@@Z:NEAR	; CToolBar::Dump
EXTRN	??0CUIntArray@@QAE@XZ:NEAR			; CUIntArray::CUIntArray
EXTRN	?GetScrollBarCtrl@CWnd@@UBEPAVCScrollBar@@H@Z:NEAR ; CWnd::GetScrollBarCtrl
EXTRN	?WinHelpW@CWnd@@UAEXKI@Z:NEAR			; CWnd::WinHelpW
EXTRN	?ContinueModal@CWnd@@UAEHXZ:NEAR		; CWnd::ContinueModal
EXTRN	?EndModalLoop@CWnd@@UAEXH@Z:NEAR		; CWnd::EndModalLoop
EXTRN	?OnCommand@CWnd@@MAEHIJ@Z:NEAR			; CWnd::OnCommand
EXTRN	?OnNotify@CWnd@@MAEHIJPAJ@Z:NEAR		; CWnd::OnNotify
EXTRN	?OnCmdMsg@CCmdTarget@@UAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z:NEAR ; CCmdTarget::OnCmdMsg
EXTRN	?IsInvokeAllowed@CCmdTarget@@UAEHJ@Z:NEAR	; CCmdTarget::IsInvokeAllowed
EXTRN	?GetDispatchIID@CCmdTarget@@UAEHPAU_GUID@@@Z:NEAR ; CCmdTarget::GetDispatchIID
EXTRN	?GetTypeInfoCount@CCmdTarget@@UAEIXZ:NEAR	; CCmdTarget::GetTypeInfoCount
EXTRN	?GetTypeLibCache@CCmdTarget@@UAEPAVCTypeLibCache@@XZ:NEAR ; CCmdTarget::GetTypeLibCache
EXTRN	?GetTypeLib@CCmdTarget@@UAEJKPAPAUITypeLib@@@Z:NEAR ; CCmdTarget::GetTypeLib
EXTRN	?GetCommandMap@CCmdTarget@@MBEPBUAFX_OLECMDMAP@@XZ:NEAR ; CCmdTarget::GetCommandMap
EXTRN	??0CImageList@@QAE@XZ:NEAR			; CImageList::CImageList
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
EXTRN	?OnWndMsg@CWnd@@MAEHIIJPAJ@Z:NEAR		; CWnd::OnWndMsg
EXTRN	?DefWindowProcW@CWnd@@MAEJIIJ@Z:NEAR		; CWnd::DefWindowProcW
EXTRN	__except_list:DWORD
EXTRN	__chkesp:NEAR
EXTRN	___CxxFrameHandler:NEAR
EXTRN	?OnChildNotify@CWnd@@MAEHIIJPAJ@Z:NEAR		; CWnd::OnChildNotify
EXTRN	?CheckAutoCenter@CWnd@@UAEHXZ:NEAR		; CWnd::CheckAutoCenter
EXTRN	?GetExtraConnectionPoints@CCmdTarget@@MAEHPAVCPtrArray@@@Z:NEAR ; CCmdTarget::GetExtraConnectionPoints
EXTRN	?GetConnectionHook@CCmdTarget@@MAEPAUIConnectionPoint@@ABU_GUID@@@Z:NEAR ; CCmdTarget::GetConnectionHook
EXTRN	??1CImageList@@UAE@XZ:NEAR			; CImageList::~CImageList
EXTRN	?IsFrameWnd@CWnd@@UBEHXZ:NEAR			; CWnd::IsFrameWnd
EXTRN	?OnFinalRelease@CWnd@@UAEXXZ:NEAR		; CWnd::OnFinalRelease
EXTRN	?SetOccDialogInfo@CWnd@@MAEHPAU_AFX_OCC_DIALOG_INFO@@@Z:NEAR ; CWnd::SetOccDialogInfo
EXTRN	?DelayShow@CControlBar@@UAEXH@Z:NEAR		; CControlBar::DelayShow
EXTRN	?IsVisible@CControlBar@@UBEHXZ:NEAR		; CControlBar::IsVisible
EXTRN	?RecalcDelayShow@CControlBar@@UAEKPAUAFX_SIZEPARENTPARAMS@@@Z:NEAR ; CControlBar::RecalcDelayShow
EXTRN	?IsDockBar@CControlBar@@UBEHXZ:NEAR		; CControlBar::IsDockBar
EXTRN	?DestroyWindow@CControlBar@@UAEHXZ:NEAR		; CControlBar::DestroyWindow
;	COMDAT ??_7CEnToolBar@@6B@
; File D:\_CODE\Shared\entoolbar.cpp
CONST	SEGMENT
??_7CEnToolBar@@6B@ DD FLAT:?GetRuntimeClass@CToolBar@@UBEPAUCRuntimeClass@@XZ ; CEnToolBar::`vftable'
	DD	FLAT:??_ECEnToolBar@@UAEPAXI@Z
	DD	FLAT:?Serialize@CObject@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@CToolBar@@UBEXXZ
	DD	FLAT:?Dump@CToolBar@@UBEXAAVCDumpContext@@@Z
	DD	FLAT:?OnCmdMsg@CCmdTarget@@UAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z
	DD	FLAT:?OnFinalRelease@CWnd@@UAEXXZ
	DD	FLAT:?IsInvokeAllowed@CCmdTarget@@UAEHJ@Z
	DD	FLAT:?GetDispatchIID@CCmdTarget@@UAEHPAU_GUID@@@Z
	DD	FLAT:?GetTypeInfoCount@CCmdTarget@@UAEIXZ
	DD	FLAT:?GetTypeLibCache@CCmdTarget@@UAEPAVCTypeLibCache@@XZ
	DD	FLAT:?GetTypeLib@CCmdTarget@@UAEJKPAPAUITypeLib@@@Z
	DD	FLAT:?GetMessageMap@CEnToolBar@@MBEPBUAFX_MSGMAP@@XZ
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
	DD	FLAT:?DestroyWindow@CControlBar@@UAEHXZ
	DD	FLAT:?PreCreateWindow@CControlBar@@UAEHAAUtagCREATESTRUCTW@@@Z
	DD	FLAT:?CalcWindowRect@CWnd@@UAEXPAUtagRECT@@I@Z
	DD	FLAT:?OnToolHitTest@CToolBar@@UBEHVCPoint@@PAUtagTOOLINFOW@@@Z
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
	DD	FLAT:?PreTranslateMessage@CControlBar@@UAEHPAUtagMSG@@@Z
	DD	FLAT:?OnAmbientProperty@CWnd@@UAEHPAVCOleControlSite@@JPAUtagVARIANT@@@Z
	DD	FLAT:?WindowProc@CControlBar@@UAEJIIJ@Z
	DD	FLAT:?OnWndMsg@CWnd@@MAEHIIJPAJ@Z
	DD	FLAT:?DefWindowProcW@CWnd@@MAEJIIJ@Z
	DD	FLAT:?PostNcDestroy@CControlBar@@UAEXXZ
	DD	FLAT:?OnChildNotify@CWnd@@MAEHIIJPAJ@Z
	DD	FLAT:?CheckAutoCenter@CWnd@@UAEHXZ
	DD	FLAT:?IsFrameWnd@CWnd@@UBEHXZ
	DD	FLAT:?SetOccDialogInfo@CWnd@@MAEHPAU_AFX_OCC_DIALOG_INFO@@@Z
	DD	FLAT:?CalcFixedLayout@CToolBar@@UAE?AVCSize@@HH@Z
	DD	FLAT:?CalcDynamicLayout@CToolBar@@UAE?AVCSize@@HK@Z
	DD	FLAT:?OnUpdateCmdUI@CToolBar@@UAEXPAVCFrameWnd@@H@Z
	DD	FLAT:?DelayShow@CControlBar@@UAEXH@Z
	DD	FLAT:?IsVisible@CControlBar@@UBEHXZ
	DD	FLAT:?RecalcDelayShow@CControlBar@@UAEKPAUAFX_SIZEPARENTPARAMS@@@Z
	DD	FLAT:?IsDockBar@CControlBar@@UBEHXZ
	DD	FLAT:?OnBarStyleChange@CToolBar@@UAEXKK@Z
	DD	FLAT:?DoPaint@CControlBar@@UAEXPAVCDC@@@Z
	DD	FLAT:?SetStatusText@CControlBar@@UAEHH@Z
	DD	FLAT:?GetItemRect@CToolBar@@UBEXHPAUtagRECT@@@Z
	DD	FLAT:?OnItemPrePaint@CEnToolBar@@MAEJPAU_NMTBCUSTOMDRAW@@@Z
	DD	FLAT:?OnItemPostPaint@CEnToolBar@@MAEJPAU_NMTBCUSTOMDRAW@@@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76639	DD	019930520H
	DD	03H
	DD	FLAT:$T76643
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76643	DD	0ffffffffH
	DD	FLAT:$L76632
	DD	00H
	DD	FLAT:$L76633
	DD	01H
	DD	FLAT:$L76634
xdata$x	ENDS
;	COMDAT ??0CEnToolBar@@QAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??0CEnToolBar@@QAE@XZ PROC NEAR				; CEnToolBar::CEnToolBar, COMDAT

; 27   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76640
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CToolBar@@QAE@XZ			; CToolBar::CToolBar
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 160				; 000000a0H
	call	??0CImageList@@QAE@XZ			; CImageList::CImageList
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	??0CImageList@@QAE@XZ			; CImageList::CImageList
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+176], -1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+180], -1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	??0CUIntArray@@QAE@XZ			; CUIntArray::CUIntArray
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7CEnToolBar@@6B@ ; CEnToolBar::`vftable'

; 28   : }

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
$L76632:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CToolBar@@UAE@XZ			; CToolBar::~CToolBar
	ret	0
$L76633:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 160				; 000000a0H
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	ret	0
$L76634:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	ret	0
$L76640:
	mov	eax, OFFSET FLAT:$T76639
	jmp	___CxxFrameHandler
text$x	ENDS
??0CEnToolBar@@QAE@XZ ENDP				; CEnToolBar::CEnToolBar
;	COMDAT ?OnItemPrePaint@CEnToolBar@@MAEJPAU_NMTBCUSTOMDRAW@@@Z
_TEXT	SEGMENT
_this$ = -4
?OnItemPrePaint@CEnToolBar@@MAEJPAU_NMTBCUSTOMDRAW@@@Z PROC NEAR ; CEnToolBar::OnItemPrePaint, COMDAT

; 64   : 	virtual LRESULT OnItemPrePaint(LPNMTBCUSTOMDRAW /*lpNMCustomDraw*/) { return CDRF_DODEFAULT; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	xor	eax, eax
	mov	esp, ebp
	pop	ebp
	ret	4
?OnItemPrePaint@CEnToolBar@@MAEJPAU_NMTBCUSTOMDRAW@@@Z ENDP ; CEnToolBar::OnItemPrePaint
_TEXT	ENDS
;	COMDAT ?OnItemPostPaint@CEnToolBar@@MAEJPAU_NMTBCUSTOMDRAW@@@Z
_TEXT	SEGMENT
_this$ = -4
?OnItemPostPaint@CEnToolBar@@MAEJPAU_NMTBCUSTOMDRAW@@@Z PROC NEAR ; CEnToolBar::OnItemPostPaint, COMDAT

; 65   : 	virtual LRESULT OnItemPostPaint(LPNMTBCUSTOMDRAW /*lpNMCustomDraw*/) { return CDRF_DODEFAULT; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	xor	eax, eax
	mov	esp, ebp
	pop	ebp
	ret	4
?OnItemPostPaint@CEnToolBar@@MAEJPAU_NMTBCUSTOMDRAW@@@Z ENDP ; CEnToolBar::OnItemPostPaint
_TEXT	ENDS
PUBLIC	??1CEnToolBar@@UAE@XZ				; CEnToolBar::~CEnToolBar
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_GCEnToolBar@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCEnToolBar@@UAEPAXI@Z PROC NEAR			; CEnToolBar::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CEnToolBar@@UAE@XZ			; CEnToolBar::~CEnToolBar
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L76132
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L76132:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCEnToolBar@@UAEPAXI@Z ENDP				; CEnToolBar::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	??1CUIntArray@@UAE@XZ:NEAR			; CUIntArray::~CUIntArray
EXTRN	?DeleteImageList@CImageList@@QAEHXZ:NEAR	; CImageList::DeleteImageList
;	COMDAT xdata$x
; File D:\_CODE\Shared\entoolbar.cpp
xdata$x	SEGMENT
$T76660	DD	019930520H
	DD	04H
	DD	FLAT:$T76662
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76662	DD	0ffffffffH
	DD	FLAT:$L76655
	DD	00H
	DD	FLAT:$L76656
	DD	01H
	DD	FLAT:$L76657
	DD	02H
	DD	FLAT:$L76658
xdata$x	ENDS
;	COMDAT ??1CEnToolBar@@UAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1CEnToolBar@@UAE@XZ PROC NEAR				; CEnToolBar::~CEnToolBar, COMDAT

; 31   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76661
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CEnToolBar@@6B@ ; CEnToolBar::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 3

; 32   : 	m_ilNormal.DeleteImageList();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?DeleteImageList@CImageList@@QAEHXZ	; CImageList::DeleteImageList

; 33   : 	m_ilDisabled.DeleteImageList();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 160				; 000000a0H
	call	?DeleteImageList@CImageList@@QAEHXZ	; CImageList::DeleteImageList

; 34   : }

	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	??1CUIntArray@@UAE@XZ			; CUIntArray::~CUIntArray
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 160				; 000000a0H
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CToolBar@@UAE@XZ			; CToolBar::~CToolBar
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
$L76655:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CToolBar@@UAE@XZ			; CToolBar::~CToolBar
	ret	0
$L76656:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 160				; 000000a0H
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	ret	0
$L76657:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	ret	0
$L76658:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	??1CUIntArray@@UAE@XZ			; CUIntArray::~CUIntArray
	ret	0
$L76661:
	mov	eax, OFFSET FLAT:$T76660
	jmp	___CxxFrameHandler
text$x	ENDS
??1CEnToolBar@@UAE@XZ ENDP				; CEnToolBar::~CEnToolBar
EXTRN	__imp_?messageMap@CToolBar@@1UAFX_MSGMAP@@B:QWORD
;	COMDAT ?_GetBaseMessageMap@CEnToolBar@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
?_GetBaseMessageMap@CEnToolBar@@KGPBUAFX_MSGMAP@@XZ PROC NEAR ; CEnToolBar::_GetBaseMessageMap, COMDAT

; 37   : BEGIN_MESSAGE_MAP(CEnToolBar, CToolBar)

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR __imp_?messageMap@CToolBar@@1UAFX_MSGMAP@@B
	pop	ebp
	ret	0
?_GetBaseMessageMap@CEnToolBar@@KGPBUAFX_MSGMAP@@XZ ENDP ; CEnToolBar::_GetBaseMessageMap
_TEXT	ENDS
;	COMDAT ?GetMessageMap@CEnToolBar@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetMessageMap@CEnToolBar@@MBEPBUAFX_MSGMAP@@XZ PROC NEAR ; CEnToolBar::GetMessageMap, COMDAT

; 37   : BEGIN_MESSAGE_MAP(CEnToolBar, CToolBar)

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, OFFSET FLAT:?messageMap@CEnToolBar@@1UAFX_MSGMAP@@B ; CEnToolBar::messageMap
	mov	esp, ebp
	pop	ebp
	ret	0
?GetMessageMap@CEnToolBar@@MBEPBUAFX_MSGMAP@@XZ ENDP	; CEnToolBar::GetMessageMap
_TEXT	ENDS
PUBLIC	?LoadToolBar@CEnToolBar@@QAEHPBG0K@Z		; CEnToolBar::LoadToolBar
PUBLIC	?SetImage@CEnToolBar@@QAEHABVCString@@K@Z	; CEnToolBar::SetImage
EXTRN	?LoadToolBar@CToolBar@@QAEHPBG@Z:NEAR		; CToolBar::LoadToolBar
EXTRN	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ:NEAR ; CToolBar::GetToolBarCtrl
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	__imp__GetFileAttributesW@4:NEAR
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
EXTRN	?SetDisabledImageList@CToolBarCtrl@@QAEPAVCImageList@@PAV2@@Z:NEAR ; CToolBarCtrl::SetDisabledImageList
;	COMDAT xdata$x
; File D:\_CODE\Shared\entoolbar.cpp
xdata$x	SEGMENT
$T76674	DD	019930520H
	DD	01H
	DD	FLAT:$T76676
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76676	DD	0ffffffffH
	DD	FLAT:$L76672
xdata$x	ENDS
;	COMDAT ?LoadToolBar@CEnToolBar@@QAEHPBG0K@Z
_TEXT	SEGMENT
_lpszResourceName$ = 8
_szImagePath$ = 12
_crMask$ = 16
_this$ = -20
$T76671 = -16
__$EHRec$ = -12
?LoadToolBar@CEnToolBar@@QAEHPBG0K@Z PROC NEAR		; CEnToolBar::LoadToolBar, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76675
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 54   : 	if (CToolBar::LoadToolBar(lpszResourceName))

	mov	eax, DWORD PTR _lpszResourceName$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadToolBar@CToolBar@@QAEHPBG@Z	; CToolBar::LoadToolBar
	test	eax, eax
	je	SHORT $L76177

; 56   : 		if (szImagePath && ::GetFileAttributes(szImagePath) != 0xffffffff)

	cmp	DWORD PTR _szImagePath$[ebp], 0
	je	SHORT $L76178
	mov	esi, esp
	mov	ecx, DWORD PTR _szImagePath$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetFileAttributesW@4
	cmp	esi, esp
	call	__chkesp
	cmp	eax, -1
	je	SHORT $L76178

; 58   : 			SetImage(szImagePath, crMask);

	mov	edx, DWORD PTR _szImagePath$[ebp]
	push	edx
	lea	ecx, DWORD PTR $T76671[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR _crMask$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T76671[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetImage@CEnToolBar@@QAEHABVCString@@K@Z ; CEnToolBar::SetImage
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76671[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 60   : 		else

	jmp	SHORT $L76180
$L76178:

; 62   : 			GetToolBarCtrl().SetDisabledImageList(NULL);

	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?SetDisabledImageList@CToolBarCtrl@@QAEPAVCImageList@@PAV2@@Z ; CToolBarCtrl::SetDisabledImageList

; 63   : 			m_ilDisabled.DeleteImageList();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 160				; 000000a0H
	call	?DeleteImageList@CImageList@@QAEHXZ	; CImageList::DeleteImageList
$L76180:

; 65   : 
; 66   : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76176
$L76177:

; 68   : 
; 69   : 	return FALSE;

	xor	eax, eax
$L76176:

; 70   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76672:
	lea	ecx, DWORD PTR $T76671[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76675:
	mov	eax, OFFSET FLAT:$T76674
	jmp	___CxxFrameHandler
text$x	ENDS
?LoadToolBar@CEnToolBar@@QAEHPBG0K@Z ENDP		; CEnToolBar::LoadToolBar
PUBLIC	?LoadToolBar@CEnToolBar@@QAEHIPBGK@Z		; CEnToolBar::LoadToolBar
;	COMDAT ?LoadToolBar@CEnToolBar@@QAEHIPBGK@Z
_TEXT	SEGMENT
_nIDResource$ = 8
_szImagePath$ = 12
_crMask$ = 16
_this$ = -4
?LoadToolBar@CEnToolBar@@QAEHIPBGK@Z PROC NEAR		; CEnToolBar::LoadToolBar, COMDAT

; 73   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 74   : 	return LoadToolBar(MAKEINTRESOURCE(nIDResource), szImagePath, crMask);

	mov	eax, DWORD PTR _crMask$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szImagePath$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nIDResource$[ebp]
	and	edx, 65535				; 0000ffffH
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadToolBar@CEnToolBar@@QAEHPBG0K@Z	; CEnToolBar::LoadToolBar

; 75   : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?LoadToolBar@CEnToolBar@@QAEHIPBGK@Z ENDP		; CEnToolBar::LoadToolBar
_TEXT	ENDS
PUBLIC	?LoadToolBar@CEnToolBar@@QAEHIIK@Z		; CEnToolBar::LoadToolBar
PUBLIC	?SetImage@CEnToolBar@@QAEHIK@Z			; CEnToolBar::SetImage
;	COMDAT ?LoadToolBar@CEnToolBar@@QAEHIIK@Z
_TEXT	SEGMENT
_nIDResource$ = 8
_nIDImage$ = 12
_crMask$ = 16
_this$ = -4
?LoadToolBar@CEnToolBar@@QAEHIIK@Z PROC NEAR		; CEnToolBar::LoadToolBar, COMDAT

; 78   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 79   : 	if (CToolBar::LoadToolBar(MAKEINTRESOURCE(nIDResource)))

	mov	eax, DWORD PTR _nIDResource$[ebp]
	and	eax, 65535				; 0000ffffH
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadToolBar@CToolBar@@QAEHPBG@Z	; CToolBar::LoadToolBar
	test	eax, eax
	je	SHORT $L76199

; 81   : 		if (nIDImage)

	cmp	DWORD PTR _nIDImage$[ebp], 0
	je	SHORT $L76200

; 83   : 			SetImage(nIDImage, crMask);

	mov	ecx, DWORD PTR _crMask$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nIDImage$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetImage@CEnToolBar@@QAEHIK@Z		; CEnToolBar::SetImage

; 85   : 		else

	jmp	SHORT $L76201
$L76200:

; 87   : 			GetToolBarCtrl().SetDisabledImageList(NULL);

	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?SetDisabledImageList@CToolBarCtrl@@QAEPAVCImageList@@PAV2@@Z ; CToolBarCtrl::SetDisabledImageList

; 88   : 			m_ilDisabled.DeleteImageList();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 160				; 000000a0H
	call	?DeleteImageList@CImageList@@QAEHXZ	; CImageList::DeleteImageList
$L76201:

; 90   : 
; 91   : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76195
$L76199:

; 93   : 
; 94   : 	return FALSE;

	xor	eax, eax
$L76195:

; 95   : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?LoadToolBar@CEnToolBar@@QAEHIIK@Z ENDP			; CEnToolBar::LoadToolBar
_TEXT	ENDS
PUBLIC	?SetImage@CEnToolBar@@IAEHPAVCEnBitmapEx@@K@Z	; CEnToolBar::SetImage
EXTRN	?LoadBitmapW@CBitmap@@QAEHI@Z:NEAR		; CBitmap::LoadBitmapW
EXTRN	??0CEnBitmapEx@@QAE@K@Z:NEAR			; CEnBitmapEx::CEnBitmapEx
EXTRN	??1CEnBitmapEx@@UAE@XZ:NEAR			; CEnBitmapEx::~CEnBitmapEx
;	COMDAT xdata$x
; File D:\_CODE\Shared\entoolbar.cpp
xdata$x	SEGMENT
$T76689	DD	019930520H
	DD	01H
	DD	FLAT:$T76691
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76691	DD	0ffffffffH
	DD	FLAT:$L76687
xdata$x	ENDS
;	COMDAT ?SetImage@CEnToolBar@@QAEHIK@Z
_TEXT	SEGMENT
_nIDImage$ = 8
_crMask$ = 12
_this$ = -44
_bitmap$ = -32
$T76685 = -36
$T76686 = -40
__$EHRec$ = -12
?SetImage@CEnToolBar@@QAEHIK@Z PROC NEAR		; CEnToolBar::SetImage, COMDAT

; 98   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76690
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 32					; 00000020H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-44], eax
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 99   : 	CEnBitmapEx bitmap;

	push	16777215				; 00ffffffH
	lea	ecx, DWORD PTR _bitmap$[ebp]
	call	??0CEnBitmapEx@@QAE@K@Z			; CEnBitmapEx::CEnBitmapEx
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 100  : 	
; 101  : 	if (!bitmap.LoadBitmap(nIDImage))

	mov	eax, DWORD PTR _nIDImage$[ebp]
	push	eax
	lea	ecx, DWORD PTR _bitmap$[ebp]
	call	?LoadBitmapW@CBitmap@@QAEHI@Z		; CBitmap::LoadBitmapW
	test	eax, eax
	jne	SHORT $L76208

; 102  : 		return FALSE;

	mov	DWORD PTR $T76685[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _bitmap$[ebp]
	call	??1CEnBitmapEx@@UAE@XZ			; CEnBitmapEx::~CEnBitmapEx
	mov	eax, DWORD PTR $T76685[ebp]
	jmp	SHORT $L76206
$L76208:

; 103  : 	
; 104  : 	return SetImage(&bitmap, crMask);

	mov	ecx, DWORD PTR _crMask$[ebp]
	push	ecx
	lea	edx, DWORD PTR _bitmap$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetImage@CEnToolBar@@IAEHPAVCEnBitmapEx@@K@Z ; CEnToolBar::SetImage
	mov	DWORD PTR $T76686[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _bitmap$[ebp]
	call	??1CEnBitmapEx@@UAE@XZ			; CEnBitmapEx::~CEnBitmapEx
	mov	eax, DWORD PTR $T76686[ebp]
$L76206:

; 105  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 44					; 0000002cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76687:
	lea	ecx, DWORD PTR _bitmap$[ebp]
	call	??1CEnBitmapEx@@UAE@XZ			; CEnBitmapEx::~CEnBitmapEx
	ret	0
$L76690:
	mov	eax, OFFSET FLAT:$T76689
	jmp	___CxxFrameHandler
text$x	ENDS
?SetImage@CEnToolBar@@QAEHIK@Z ENDP			; CEnToolBar::SetImage
EXTRN	?LoadImageW@CEnBitmap@@QAEHPBGK@Z:NEAR		; CEnBitmap::LoadImageW
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
;	COMDAT xdata$x
; File D:\_CODE\Shared\entoolbar.cpp
xdata$x	SEGMENT
$T76700	DD	019930520H
	DD	01H
	DD	FLAT:$T76702
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76702	DD	0ffffffffH
	DD	FLAT:$L76698
xdata$x	ENDS
;	COMDAT ?SetImage@CEnToolBar@@QAEHABVCString@@K@Z
_TEXT	SEGMENT
_sImagePath$ = 8
_crMask$ = 12
_this$ = -44
_bitmap$ = -32
$T76696 = -36
$T76697 = -40
__$EHRec$ = -12
?SetImage@CEnToolBar@@QAEHABVCString@@K@Z PROC NEAR	; CEnToolBar::SetImage, COMDAT

; 108  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76701
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 32					; 00000020H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-44], eax
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 109  : 	CEnBitmapEx bitmap;

	push	16777215				; 00ffffffH
	lea	ecx, DWORD PTR _bitmap$[ebp]
	call	??0CEnBitmapEx@@QAE@K@Z			; CEnBitmapEx::CEnBitmapEx
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 110  : 	
; 111  : 	if (!bitmap.LoadImage(sImagePath))

	push	-1
	mov	ecx, DWORD PTR _sImagePath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _bitmap$[ebp]
	call	?LoadImageW@CEnBitmap@@QAEHPBGK@Z	; CEnBitmap::LoadImageW
	test	eax, eax
	jne	SHORT $L76217

; 112  : 		return FALSE;

	mov	DWORD PTR $T76696[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _bitmap$[ebp]
	call	??1CEnBitmapEx@@UAE@XZ			; CEnBitmapEx::~CEnBitmapEx
	mov	eax, DWORD PTR $T76696[ebp]
	jmp	SHORT $L76215
$L76217:

; 113  : 	
; 114  : 	return SetImage(&bitmap, crMask);

	mov	eax, DWORD PTR _crMask$[ebp]
	push	eax
	lea	ecx, DWORD PTR _bitmap$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetImage@CEnToolBar@@IAEHPAVCEnBitmapEx@@K@Z ; CEnToolBar::SetImage
	mov	DWORD PTR $T76697[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _bitmap$[ebp]
	call	??1CEnBitmapEx@@UAE@XZ			; CEnBitmapEx::~CEnBitmapEx
	mov	eax, DWORD PTR $T76697[ebp]
$L76215:

; 115  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 44					; 0000002cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76698:
	lea	ecx, DWORD PTR _bitmap$[ebp]
	call	??1CEnBitmapEx@@UAE@XZ			; CEnBitmapEx::~CEnBitmapEx
	ret	0
$L76701:
	mov	eax, OFFSET FLAT:$T76700
	jmp	___CxxFrameHandler
text$x	ENDS
?SetImage@CEnToolBar@@QAEHABVCString@@K@Z ENDP		; CEnToolBar::SetImage
PUBLIC	?GetButtonCount@CEnToolBar@@QBEHH@Z		; CEnToolBar::GetButtonCount
PUBLIC	?RefreshDisabledImageList@CEnToolBar@@IAEXPAVCEnBitmapEx@@K@Z ; CEnToolBar::RefreshDisabledImageList
EXTRN	?GetBitmap@CBitmap@@QAEHPAUtagBITMAP@@@Z:NEAR	; CBitmap::GetBitmap
EXTRN	?SetSizes@CToolBar@@QAEXUtagSIZE@@0@Z:NEAR	; CToolBar::SetSizes
EXTRN	?CopyImage@CEnBitmap@@QAEHPAVCBitmap@@@Z:NEAR	; CEnBitmap::CopyImage
EXTRN	?RemapSysColors@CEnBitmapEx@@QAEHXZ:NEAR	; CEnBitmapEx::RemapSysColors
EXTRN	??0CSize@@QAE@HH@Z:NEAR				; CSize::CSize
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	?Create@CImageList@@QAEHHHIHH@Z:NEAR		; CImageList::Create
EXTRN	?Add@CImageList@@QAEHPAVCBitmap@@K@Z:NEAR	; CImageList::Add
EXTRN	?SetHotImageList@CToolBarCtrl@@QAEPAVCImageList@@PAV2@@Z:NEAR ; CToolBarCtrl::SetHotImageList
EXTRN	?SetImageList@CToolBarCtrl@@QAEPAVCImageList@@PAV2@@Z:NEAR ; CToolBarCtrl::SetImageList
;	COMDAT xdata$x
; File D:\_CODE\Shared\entoolbar.cpp
xdata$x	SEGMENT
$T76714	DD	019930520H
	DD	01H
	DD	FLAT:$T76716
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76716	DD	0ffffffffH
	DD	FLAT:$L76712
xdata$x	ENDS
;	COMDAT ?SetImage@CEnToolBar@@IAEHPAVCEnBitmapEx@@K@Z
_TEXT	SEGMENT
_pBitmap$ = 8
_crMask$ = 12
_this$ = -112
_bmDis$ = -64
_BM$ = -44
_nCount$ = -68
_sizeBmp$ = -76
_sizeBtn$ = -20
_pILPrev$76240 = -80
$T76707 = -84
$T76708 = -92
$T76709 = -100
$T76710 = -104
$T76711 = -108
__$EHRec$ = -12
?SetImage@CEnToolBar@@IAEHPAVCEnBitmapEx@@K@Z PROC NEAR	; CEnToolBar::SetImage, COMDAT

; 118  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76715
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 100				; 00000064H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-112]
	mov	ecx, 25					; 00000019H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 119  : 	CEnBitmapEx bmDis;

	push	16777215				; 00ffffffH
	lea	ecx, DWORD PTR _bmDis$[ebp]
	call	??0CEnBitmapEx@@QAE@K@Z			; CEnBitmapEx::CEnBitmapEx
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 120  : 	bmDis.CopyImage(pBitmap); // for later

	mov	eax, DWORD PTR _pBitmap$[ebp]
	push	eax
	lea	ecx, DWORD PTR _bmDis$[ebp]
	call	?CopyImage@CEnBitmap@@QAEHPAVCBitmap@@@Z ; CEnBitmap::CopyImage

; 121  : 	
; 122  :     if (crMask == CLR_NONE) // map 3d colors

	cmp	DWORD PTR _crMask$[ebp], -1
	jne	SHORT $L76226

; 123  : 	   pBitmap->RemapSysColors();

	mov	ecx, DWORD PTR _pBitmap$[ebp]
	call	?RemapSysColors@CEnBitmapEx@@QAEHXZ	; CEnBitmapEx::RemapSysColors
$L76226:

; 127  : 	pBitmap->GetBitmap(&BM);

	lea	ecx, DWORD PTR _BM$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pBitmap$[ebp]
	call	?GetBitmap@CBitmap@@QAEHPAUtagBITMAP@@@Z ; CBitmap::GetBitmap

; 128  : 	
; 129  : 	int nCount = GetButtonCount(TRUE);

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetButtonCount@CEnToolBar@@QBEHH@Z	; CEnToolBar::GetButtonCount
	mov	DWORD PTR _nCount$[ebp], eax
$L76229:

; 130  : 	ASSERT (nCount);

	cmp	DWORD PTR _nCount$[ebp], 0
	jne	SHORT $L76232
	push	130					; 00000082H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76232
	int	3
$L76232:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76229

; 131  : 	
; 132  : 	if (!nCount)

	cmp	DWORD PTR _nCount$[ebp], 0
	jne	SHORT $L76233

; 133  : 		return FALSE;

	mov	DWORD PTR $T76707[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _bmDis$[ebp]
	call	??1CEnBitmapEx@@UAE@XZ			; CEnBitmapEx::~CEnBitmapEx
	mov	eax, DWORD PTR $T76707[ebp]
	jmp	$L76224
$L76233:

; 134  : 	
; 135  : 	CSize sizeBmp((BM.bmWidth / nCount), BM.bmHeight);

	mov	eax, DWORD PTR _BM$[ebp+8]
	push	eax
	mov	eax, DWORD PTR _BM$[ebp+4]
	cdq
	idiv	DWORD PTR _nCount$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sizeBmp$[ebp]
	call	??0CSize@@QAE@HH@Z			; CSize::CSize

; 136  : 	CSize sizeBtn(sizeBmp.cx + 7, sizeBmp.cy + 7);

	mov	ecx, DWORD PTR _sizeBmp$[ebp+4]
	add	ecx, 7
	push	ecx
	mov	edx, DWORD PTR _sizeBmp$[ebp]
	add	edx, 7
	push	edx
	lea	ecx, DWORD PTR _sizeBtn$[ebp]
	call	??0CSize@@QAE@HH@Z			; CSize::CSize

; 137  : 	
; 138  : 	SetSizes(sizeBtn, sizeBmp);

	mov	eax, DWORD PTR _sizeBmp$[ebp]
	mov	DWORD PTR $T76708[ebp], eax
	mov	ecx, DWORD PTR _sizeBmp$[ebp+4]
	mov	DWORD PTR $T76708[ebp+4], ecx
	mov	edx, DWORD PTR $T76708[ebp+4]
	push	edx
	mov	eax, DWORD PTR $T76708[ebp]
	push	eax
	mov	ecx, DWORD PTR _sizeBtn$[ebp]
	mov	DWORD PTR $T76709[ebp], ecx
	mov	edx, DWORD PTR _sizeBtn$[ebp+4]
	mov	DWORD PTR $T76709[ebp+4], edx
	mov	eax, DWORD PTR $T76709[ebp+4]
	push	eax
	mov	ecx, DWORD PTR $T76709[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetSizes@CToolBar@@QAEXUtagSIZE@@0@Z	; CToolBar::SetSizes

; 139  : 
; 140  : 	m_ilNormal.DeleteImageList();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?DeleteImageList@CImageList@@QAEHXZ	; CImageList::DeleteImageList

; 141  : 	
; 142  : 	if (m_ilNormal.Create(sizeBmp.cx, sizeBmp.cy, ILC_COLOR32 | ILC_MASK, 0, 1)) 

	push	1
	push	0
	push	33					; 00000021H
	mov	edx, DWORD PTR _sizeBmp$[ebp+4]
	push	edx
	mov	eax, DWORD PTR _sizeBmp$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?Create@CImageList@@QAEHHHIHH@Z		; CImageList::Create
	test	eax, eax
	je	$L76239

; 144  : 		m_ilNormal.Add(pBitmap, crMask);

	mov	ecx, DWORD PTR _crMask$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pBitmap$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?Add@CImageList@@QAEHPAVCBitmap@@K@Z	; CImageList::Add

; 145  : 				
; 146  : 		CImageList* pILPrev = GetToolBarCtrl().SetImageList(&m_ilNormal);

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 168				; 000000a8H
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?SetImageList@CToolBarCtrl@@QAEPAVCImageList@@PAV2@@Z ; CToolBarCtrl::SetImageList
	mov	DWORD PTR _pILPrev$76240[ebp], eax

; 147  : 
; 148  : 		if (pILPrev)

	cmp	DWORD PTR _pILPrev$76240[ebp], 0
	je	SHORT $L76241

; 149  : 			pILPrev->DeleteImageList(); // cleanup

	mov	ecx, DWORD PTR _pILPrev$76240[ebp]
	call	?DeleteImageList@CImageList@@QAEHXZ	; CImageList::DeleteImageList
$L76241:

; 150  : 
; 151  : 		pILPrev = GetToolBarCtrl().SetHotImageList(&m_ilNormal);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?SetHotImageList@CToolBarCtrl@@QAEPAVCImageList@@PAV2@@Z ; CToolBarCtrl::SetHotImageList
	mov	DWORD PTR _pILPrev$76240[ebp], eax

; 152  : 
; 153  : 		if (pILPrev)

	cmp	DWORD PTR _pILPrev$76240[ebp], 0
	je	SHORT $L76242

; 154  : 			pILPrev->DeleteImageList(); // cleanup

	mov	ecx, DWORD PTR _pILPrev$76240[ebp]
	call	?DeleteImageList@CImageList@@QAEHXZ	; CImageList::DeleteImageList
$L76242:

; 155  : 		
; 156  : 		RefreshDisabledImageList(&bmDis, crMask);

	mov	edx, DWORD PTR _crMask$[ebp]
	push	edx
	lea	eax, DWORD PTR _bmDis$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?RefreshDisabledImageList@CEnToolBar@@IAEXPAVCEnBitmapEx@@K@Z ; CEnToolBar::RefreshDisabledImageList

; 157  : 		return TRUE;

	mov	DWORD PTR $T76710[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _bmDis$[ebp]
	call	??1CEnBitmapEx@@UAE@XZ			; CEnBitmapEx::~CEnBitmapEx
	mov	eax, DWORD PTR $T76710[ebp]
	jmp	SHORT $L76224
$L76239:

; 159  : 	
; 160  : 	// else 
; 161  : 	return FALSE;

	mov	DWORD PTR $T76711[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _bmDis$[ebp]
	call	??1CEnBitmapEx@@UAE@XZ			; CEnBitmapEx::~CEnBitmapEx
	mov	eax, DWORD PTR $T76711[ebp]
$L76224:

; 162  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 112				; 00000070H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76712:
	lea	ecx, DWORD PTR _bmDis$[ebp]
	call	??1CEnBitmapEx@@UAE@XZ			; CEnBitmapEx::~CEnBitmapEx
	ret	0
$L76715:
	mov	eax, OFFSET FLAT:$T76714
	jmp	___CxxFrameHandler
text$x	ENDS
?SetImage@CEnToolBar@@IAEHPAVCEnBitmapEx@@K@Z ENDP	; CEnToolBar::SetImage
EXTRN	?GetItemID@CToolBar@@QBEIH@Z:NEAR		; CToolBar::GetItemID
EXTRN	?GetButtonCount@CToolBarCtrl@@QBEHXZ:NEAR	; CToolBarCtrl::GetButtonCount
;	COMDAT ?GetButtonCount@CEnToolBar@@QBEHH@Z
_TEXT	SEGMENT
_bIgnoreSeparators$ = 8
_this$ = -12
_nCount$ = -4
_nBtn$76251 = -8
?GetButtonCount@CEnToolBar@@QBEHH@Z PROC NEAR		; CEnToolBar::GetButtonCount, COMDAT

; 165  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 166  : 	int nCount = GetToolBarCtrl().GetButtonCount();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?GetButtonCount@CToolBarCtrl@@QBEHXZ	; CToolBarCtrl::GetButtonCount
	mov	DWORD PTR _nCount$[ebp], eax

; 167  : 
; 168  : 	if (bIgnoreSeparators)

	cmp	DWORD PTR _bIgnoreSeparators$[ebp], 0
	je	SHORT $L76254

; 170  : 		int nBtn = nCount;

	mov	eax, DWORD PTR _nCount$[ebp]
	mov	DWORD PTR _nBtn$76251[ebp], eax
$L76253:

; 171  : 
; 172  : 		while (nBtn--)

	mov	ecx, DWORD PTR _nBtn$76251[ebp]
	mov	edx, DWORD PTR _nBtn$76251[ebp]
	sub	edx, 1
	mov	DWORD PTR _nBtn$76251[ebp], edx
	test	ecx, ecx
	je	SHORT $L76254

; 174  : 			if (GetItemID(nBtn) == ID_SEPARATOR)

	mov	eax, DWORD PTR _nBtn$76251[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetItemID@CToolBar@@QBEIH@Z		; CToolBar::GetItemID
	test	eax, eax
	jne	SHORT $L76255

; 175  : 				nCount--;

	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
$L76255:

; 176  : 		}

	jmp	SHORT $L76253
$L76254:

; 178  : 
; 179  : 	return nCount;

	mov	eax, DWORD PTR _nCount$[ebp]

; 180  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetButtonCount@CEnToolBar@@QBEHH@Z ENDP		; CEnToolBar::GetButtonCount
_TEXT	ENDS
PUBLIC	?SetBackgroundColors@CEnToolBar@@QAEXKKHH@Z	; CEnToolBar::SetBackgroundColors
EXTRN	?Invalidate@CWnd@@QAEXH@Z:NEAR			; CWnd::Invalidate
EXTRN	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ:NEAR	; CWnd::GetSafeHwnd
;	COMDAT ?SetBackgroundColors@CEnToolBar@@QAEXKKHH@Z
_TEXT	SEGMENT
_crFrom$ = 8
_crTo$ = 12
_bGradient$ = 16
_bGlass$ = 20
_this$ = -4
?SetBackgroundColors@CEnToolBar@@QAEXKKHH@Z PROC NEAR	; CEnToolBar::SetBackgroundColors, COMDAT

; 183  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 184  : 	m_crFrom = crFrom;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _crFrom$[ebp]
	mov	DWORD PTR [eax+176], ecx

; 185  : 
; 186  : 	if (crTo == CLR_NONE)

	cmp	DWORD PTR _crTo$[ebp], -1
	jne	SHORT $L76263

; 187  : 		m_crTo = m_crFrom;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+176]
	mov	DWORD PTR [edx+180], ecx

; 188  : 	else

	jmp	SHORT $L76264
$L76263:

; 189  : 		m_crTo = crTo;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _crTo$[ebp]
	mov	DWORD PTR [edx+180], eax
$L76264:

; 190  : 
; 191  : 	m_bGradient = bGradient;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _bGradient$[ebp]
	mov	DWORD PTR [ecx+184], edx

; 192  : 	m_bGlass = bGlass;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _bGlass$[ebp]
	mov	DWORD PTR [eax+188], ecx

; 193  : 
; 194  : 	if (GetSafeHwnd())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	test	eax, eax
	je	SHORT $L76265

; 195  : 		Invalidate();

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Invalidate@CWnd@@QAEXH@Z		; CWnd::Invalidate
$L76265:

; 196  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?SetBackgroundColors@CEnToolBar@@QAEXKKHH@Z ENDP	; CEnToolBar::SetBackgroundColors
_TEXT	ENDS
PUBLIC	?RefreshButtonStates@CEnToolBar@@QAEXH@Z	; CEnToolBar::RefreshButtonStates
EXTRN	?PostMessageW@CWnd@@QAEHIIJ@Z:NEAR		; CWnd::PostMessageW
EXTRN	?GetParent@CWnd@@QBEPAV1@XZ:NEAR		; CWnd::GetParent
;	COMDAT ?RefreshButtonStates@CEnToolBar@@QAEXH@Z
_TEXT	SEGMENT
_bImmediate$ = 8
_this$ = -4
?RefreshButtonStates@CEnToolBar@@QAEXH@Z PROC NEAR	; CEnToolBar::RefreshButtonStates, COMDAT

; 199  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 200  : 	if (bImmediate)

	cmp	DWORD PTR _bImmediate$[ebp], 0
	je	SHORT $L76270

; 201  : 		OnUpdateCmdUI((CFrameWnd*)GetParent(), FALSE);

	mov	esi, esp
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetParent@CWnd@@QBEPAV1@XZ		; CWnd::GetParent
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+200]
	cmp	esi, esp
	call	__chkesp

; 202  : 	else

	jmp	SHORT $L76272
$L76270:

; 203  : 		PostMessage(WM_REFRESHBUTTONSTATES);

	push	0
	push	0
	push	32769					; 00008001H
	mov	ecx, DWORD PTR _this$[ebp]
	call	?PostMessageW@CWnd@@QAEHIIJ@Z		; CWnd::PostMessageW
$L76272:

; 204  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?RefreshButtonStates@CEnToolBar@@QAEXH@Z ENDP		; CEnToolBar::RefreshButtonStates
_TEXT	ENDS
;	COMDAT ?OnRefreshButtonStates@CEnToolBar@@IAEJIJ@Z
_TEXT	SEGMENT
_this$ = -4
?OnRefreshButtonStates@CEnToolBar@@IAEJIJ@Z PROC NEAR	; CEnToolBar::OnRefreshButtonStates, COMDAT

; 207  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 208  : 	RefreshButtonStates(TRUE);

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?RefreshButtonStates@CEnToolBar@@QAEXH@Z ; CEnToolBar::RefreshButtonStates

; 209  : 	return 0L;

	xor	eax, eax

; 210  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?OnRefreshButtonStates@CEnToolBar@@IAEJIJ@Z ENDP	; CEnToolBar::OnRefreshButtonStates
_TEXT	ENDS
PUBLIC	??BCOSVersion@@QAE?AW4OSVERSION@@XZ		; COSVersion::operator enum OSVERSION
EXTRN	?GrayScale@CEnBitmapEx@@QAEHK@Z:NEAR		; CEnBitmapEx::GrayScale
EXTRN	??0COSVersion@@QAE@XZ:NEAR			; COSVersion::COSVersion
EXTRN	??1COSVersion@@UAE@XZ:NEAR			; COSVersion::~COSVersion
;	COMDAT xdata$x
; File D:\_CODE\Shared\entoolbar.cpp
xdata$x	SEGMENT
$T76733	DD	019930520H
	DD	01H
	DD	FLAT:$T76735
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76735	DD	0ffffffffH
	DD	FLAT:$L76731
xdata$x	ENDS
;	COMDAT ?RefreshDisabledImageList@CEnToolBar@@IAEXPAVCEnBitmapEx@@K@Z
_TEXT	SEGMENT
_pBitmap$ = 8
_crMask$ = 12
_this$ = -36
_nCx$76288 = -20
_nCy$76289 = -24
_pILPrev$76290 = -16
$T76729 = -28
$T76730 = -32
__$EHRec$ = -12
?RefreshDisabledImageList@CEnToolBar@@IAEXPAVCEnBitmapEx@@K@Z PROC NEAR ; CEnToolBar::RefreshDisabledImageList, COMDAT

; 213  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76734
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 32					; 00000020H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-44], eax
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 214  : 	// not under win9x
; 215  : 	if (COSVersion() >= OSV_NT4)

	lea	ecx, DWORD PTR $T76730[ebp]
	call	??0COSVersion@@QAE@XZ			; COSVersion::COSVersion
	mov	DWORD PTR -40+[ebp], eax
	mov	eax, DWORD PTR -40+[ebp]
	mov	DWORD PTR -44+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -44+[ebp]
	call	??BCOSVersion@@QAE?AW4OSVERSION@@XZ	; COSVersion::operator enum OSVERSION
	xor	ecx, ecx
	cmp	eax, 3
	setge	cl
	mov	BYTE PTR $T76729[ebp], cl
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76730[ebp]
	call	??1COSVersion@@UAE@XZ			; COSVersion::~COSVersion
	mov	edx, DWORD PTR $T76729[ebp]
	and	edx, 255				; 000000ffH
	test	edx, edx
	je	$L76286

; 217  : 		// create 'nice' disabled imagelist 
; 218  : 		if (pBitmap->GrayScale(crMask))

	mov	eax, DWORD PTR _crMask$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pBitmap$[ebp]
	call	?GrayScale@CEnBitmapEx@@QAEHK@Z		; CEnBitmapEx::GrayScale
	test	eax, eax
	je	$L76286

; 220  : 			if (crMask == CLR_NONE) // map 3d colors

	cmp	DWORD PTR _crMask$[ebp], -1
	jne	SHORT $L76287

; 221  : 				pBitmap->RemapSysColors();

	mov	ecx, DWORD PTR _pBitmap$[ebp]
	call	?RemapSysColors@CEnBitmapEx@@QAEHXZ	; CEnBitmapEx::RemapSysColors
$L76287:

; 222  : 
; 223  : 			// button size
; 224  : 			int nCx = m_sizeImage.cx, nCy = m_sizeImage.cy;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+140]
	mov	DWORD PTR _nCx$76288[ebp], edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+144]
	mov	DWORD PTR _nCy$76289[ebp], ecx

; 225  : 			
; 226  : 			m_ilDisabled.DeleteImageList();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 160				; 000000a0H
	call	?DeleteImageList@CImageList@@QAEHXZ	; CImageList::DeleteImageList

; 227  : 			m_ilDisabled.Create(nCx, nCy, ILC_COLOR24 | ILC_MASK, 0, 1);

	push	1
	push	0
	push	25					; 00000019H
	mov	edx, DWORD PTR _nCy$76289[ebp]
	push	edx
	mov	eax, DWORD PTR _nCx$76288[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 160				; 000000a0H
	call	?Create@CImageList@@QAEHHHIHH@Z		; CImageList::Create

; 228  : 			m_ilDisabled.Add(pBitmap, crMask);

	mov	ecx, DWORD PTR _crMask$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pBitmap$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 160				; 000000a0H
	call	?Add@CImageList@@QAEHPAVCBitmap@@K@Z	; CImageList::Add

; 229  : 			
; 230  : 			CImageList* pILPrev = GetToolBarCtrl().SetDisabledImageList(&m_ilDisabled);

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 160				; 000000a0H
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?SetDisabledImageList@CToolBarCtrl@@QAEPAVCImageList@@PAV2@@Z ; CToolBarCtrl::SetDisabledImageList
	mov	DWORD PTR _pILPrev$76290[ebp], eax

; 231  : 
; 232  : 			if (pILPrev)

	cmp	DWORD PTR _pILPrev$76290[ebp], 0
	je	SHORT $L76291

; 233  : 				pILPrev->DeleteImageList(); // cleanup

	mov	ecx, DWORD PTR _pILPrev$76290[ebp]
	call	?DeleteImageList@CImageList@@QAEHXZ	; CImageList::DeleteImageList
$L76291:

; 234  : 		
; 235  : 			Invalidate();

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Invalidate@CWnd@@QAEXH@Z		; CWnd::Invalidate
$L76286:

; 238  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 44					; 0000002cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76731:
	lea	ecx, DWORD PTR $T76730[ebp]
	call	??1COSVersion@@UAE@XZ			; COSVersion::~COSVersion
	ret	0
$L76734:
	mov	eax, OFFSET FLAT:$T76733
	jmp	___CxxFrameHandler
text$x	ENDS
?RefreshDisabledImageList@CEnToolBar@@IAEXPAVCEnBitmapEx@@K@Z ENDP ; CEnToolBar::RefreshDisabledImageList
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
PUBLIC	?DrawBkgnd@CEnToolBar@@IAEXPAVCDC@@H@Z		; CEnToolBar::DrawBkgnd
EXTRN	?FromHandle@CDC@@SGPAV1@PAUHDC__@@@Z:NEAR	; CDC::FromHandle
;	COMDAT ?OnCustomDraw@CEnToolBar@@IAEXPAUtagNMHDR@@PAJ@Z
_TEXT	SEGMENT
_pNMHDR$ = 8
_pResult$ = 12
_this$ = -8
_lpNMCustomDraw$ = -4
?OnCustomDraw@CEnToolBar@@IAEXPAUtagNMHDR@@PAJ@Z PROC NEAR ; CEnToolBar::OnCustomDraw, COMDAT

; 241  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 242  :     *pResult = CDRF_DODEFAULT;

	mov	eax, DWORD PTR _pResult$[ebp]
	mov	DWORD PTR [eax], 0

; 243  :     LPNMTBCUSTOMDRAW lpNMCustomDraw = ( LPNMTBCUSTOMDRAW )pNMHDR;

	mov	ecx, DWORD PTR _pNMHDR$[ebp]
	mov	DWORD PTR _lpNMCustomDraw$[ebp], ecx

; 246  :     {

	mov	edx, DWORD PTR _lpNMCustomDraw$[ebp]
	mov	eax, DWORD PTR [edx+12]
	mov	DWORD PTR -12+[ebp], eax
	cmp	DWORD PTR -12+[ebp], 3
	ja	SHORT $L76742
	cmp	DWORD PTR -12+[ebp], 3
	je	SHORT $L76305
	cmp	DWORD PTR -12+[ebp], 1
	je	SHORT $L76303
	cmp	DWORD PTR -12+[ebp], 2
	je	SHORT $L76304
	jmp	$L76308
$L76742:
	cmp	DWORD PTR -12+[ebp], 65537		; 00010001H
	je	SHORT $L76306
	cmp	DWORD PTR -12+[ebp], 65538		; 00010002H
	je	SHORT $L76307
	jmp	SHORT $L76308
$L76303:

; 247  :     case CDDS_PREPAINT:
; 248  :  		DrawBkgnd(CDC::FromHandle(lpNMCustomDraw->nmcd.hdc), TRUE);

	push	1
	mov	ecx, DWORD PTR _lpNMCustomDraw$[ebp]
	mov	edx, DWORD PTR [ecx+16]
	push	edx
	call	?FromHandle@CDC@@SGPAV1@PAUHDC__@@@Z	; CDC::FromHandle
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DrawBkgnd@CEnToolBar@@IAEXPAVCDC@@H@Z	; CEnToolBar::DrawBkgnd

; 249  : 
; 250  : 		*pResult = CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;

	mov	eax, DWORD PTR _pResult$[ebp]
	mov	DWORD PTR [eax], 48			; 00000030H

; 251  : 		break;

	jmp	SHORT $L76300
$L76304:

; 252  : 		
; 253  :     case CDDS_POSTPAINT:
; 254  :        *pResult = CDRF_DODEFAULT | CDRF_NOTIFYITEMDRAW;

	mov	ecx, DWORD PTR _pResult$[ebp]
	mov	DWORD PTR [ecx], 32			; 00000020H

; 255  :        break;

	jmp	SHORT $L76300
$L76305:

; 256  :        
; 257  :     case CDDS_PREERASE:
; 258  :        *pResult = CDRF_SKIPDEFAULT | CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;

	mov	edx, DWORD PTR _pResult$[ebp]
	mov	DWORD PTR [edx], 52			; 00000034H

; 259  :        break;

	jmp	SHORT $L76300
$L76306:

; 260  :        
; 261  :     case CDDS_ITEMPREPAINT:
; 262  :        *pResult = OnItemPrePaint(lpNMCustomDraw) | CDRF_NOTIFYPOSTPAINT;

	mov	esi, esp
	mov	eax, DWORD PTR _lpNMCustomDraw$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+236]
	cmp	esi, esp
	call	__chkesp
	or	al, 16					; 00000010H
	mov	ecx, DWORD PTR _pResult$[ebp]
	mov	DWORD PTR [ecx], eax

; 263  :        break;

	jmp	SHORT $L76300
$L76307:

; 264  : 
; 265  :     case CDDS_ITEMPOSTPAINT:
; 266  :        *pResult = OnItemPostPaint(lpNMCustomDraw);

	mov	esi, esp
	mov	edx, DWORD PTR _lpNMCustomDraw$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+240]
	cmp	esi, esp
	call	__chkesp
	mov	ecx, DWORD PTR _pResult$[ebp]
	mov	DWORD PTR [ecx], eax
$L76308:
$L76300:

; 272  : }

	pop	esi
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?OnCustomDraw@CEnToolBar@@IAEXPAUtagNMHDR@@PAJ@Z ENDP	; CEnToolBar::OnCustomDraw
_TEXT	ENDS
EXTRN	?GetWindowDC@CWnd@@QAEPAVCDC@@XZ:NEAR		; CWnd::GetWindowDC
EXTRN	?ReleaseDC@CWnd@@QAEHPAVCDC@@@Z:NEAR		; CWnd::ReleaseDC
EXTRN	?Default@CWnd@@IAEJXZ:NEAR			; CWnd::Default
;	COMDAT ?OnNcPaint@CEnToolBar@@IAEXXZ
_TEXT	SEGMENT
_this$ = -8
_pDC$ = -4
?OnNcPaint@CEnToolBar@@IAEXXZ PROC NEAR			; CEnToolBar::OnNcPaint, COMDAT

; 275  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 276  : 	CDC* pDC = GetWindowDC();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetWindowDC@CWnd@@QAEPAVCDC@@XZ	; CWnd::GetWindowDC
	mov	DWORD PTR _pDC$[ebp], eax

; 277  : 
; 278  : 	DrawBkgnd(pDC, FALSE);

	push	0
	mov	eax, DWORD PTR _pDC$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DrawBkgnd@CEnToolBar@@IAEXPAVCDC@@H@Z	; CEnToolBar::DrawBkgnd

; 279  : 	ReleaseDC(pDC);

	mov	ecx, DWORD PTR _pDC$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ReleaseDC@CWnd@@QAEHPAVCDC@@@Z		; CWnd::ReleaseDC

; 280  : 
; 281  : 	Default();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Default@CWnd@@IAEJXZ			; CWnd::Default

; 282  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?OnNcPaint@CEnToolBar@@IAEXXZ ENDP			; CEnToolBar::OnNcPaint
_TEXT	ENDS
PUBLIC	?HasBkgndColor@CEnToolBar@@IBEHXZ		; CEnToolBar::HasBkgndColor
EXTRN	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z:NEAR	; CDC::FillSolidRect
EXTRN	?GetWindowRect@CWnd@@QBEXPAUtagRECT@@@Z:NEAR	; CWnd::GetWindowRect
EXTRN	?GetClientRect@CWnd@@QBEXPAUtagRECT@@@Z:NEAR	; CWnd::GetClientRect
EXTRN	?ClientToScreen@CWnd@@QBEXPAUtagRECT@@@Z:NEAR	; CWnd::ClientToScreen
EXTRN	?GetSize@CUIntArray@@QBEHXZ:NEAR		; CUIntArray::GetSize
EXTRN	??ACUIntArray@@QAEAAIH@Z:NEAR			; CUIntArray::operator[]
EXTRN	?ExcludeClipRect@CDC@@QAEHPBUtagRECT@@@Z:NEAR	; CDC::ExcludeClipRect
EXTRN	??GCPoint@@QBE?AV0@XZ:NEAR			; CPoint::operator-
EXTRN	??0CRect@@QAE@XZ:NEAR				; CRect::CRect
EXTRN	?TopLeft@CRect@@QAEAAVCPoint@@XZ:NEAR		; CRect::TopLeft
EXTRN	??BCRect@@QAEPAUtagRECT@@XZ:NEAR		; CRect::operator tagRECT *
EXTRN	?OffsetRect@CRect@@QAEXUtagPOINT@@@Z:NEAR	; CRect::OffsetRect
EXTRN	?DrawGradient@GraphicsMisc@@YAHW4GM_GRADIENT@@PAVCDC@@PBUtagRECT@@KKHH@Z:NEAR ; GraphicsMisc::DrawGradient
EXTRN	?GetGradientEdgeColor@GraphicsMisc@@YAKW4GM_GRADIENT@@KH@Z:NEAR ; GraphicsMisc::GetGradientEdgeColor
EXTRN	?GetGradientType@GraphicsMisc@@YA?AW4GM_GRADIENT@@HH@Z:NEAR ; GraphicsMisc::GetGradientType
EXTRN	__imp__GetSysColor@4:NEAR
;	COMDAT ?DrawBkgnd@CEnToolBar@@IAEXPAVCDC@@H@Z
_TEXT	SEGMENT
_pDC$ = 8
_bClient$ = 12
_this$ = -128
_rect$ = -16
_rWindow$76320 = -32
_nType$76328 = -36
_rBorder$76331 = -52
_rBorder$76333 = -68
_nNumRows$76336 = -76
_rRow$76337 = -92
_nRow$76338 = -72
$T76747 = -100
$T76748 = -108
$T76749 = -116
$T76750 = -124
?DrawBkgnd@CEnToolBar@@IAEXPAVCDC@@H@Z PROC NEAR	; CEnToolBar::DrawBkgnd, COMDAT

; 285  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 128				; 00000080H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-128]
	mov	ecx, 32					; 00000020H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 286  : 	// get client rect always
; 287  : 	CRect rect;

	lea	ecx, DWORD PTR _rect$[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 288  : 	GetClientRect(rect);

	lea	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetClientRect@CWnd@@QBEXPAUtagRECT@@@Z	; CWnd::GetClientRect

; 289  : 
; 290  : 	// if rendering the non-client background
; 291  : 	// we first clip out the client rect and
; 292  : 	// translate the window rect to (0, 0)
; 293  : 	if (!bClient)

	cmp	DWORD PTR _bClient$[ebp], 0
	jne	$L76319

; 295  : 		CRect rWindow;

	lea	ecx, DWORD PTR _rWindow$76320[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 296  : 		GetWindowRect(rWindow);

	lea	ecx, DWORD PTR _rWindow$76320[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetWindowRect@CWnd@@QBEXPAUtagRECT@@@Z	; CWnd::GetWindowRect

; 297  : 
; 298  : 		ClientToScreen(rect);

	lea	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ClientToScreen@CWnd@@QBEXPAUtagRECT@@@Z ; CWnd::ClientToScreen

; 299  : 		rect.OffsetRect(-rWindow.TopLeft());

	lea	eax, DWORD PTR $T76748[ebp]
	push	eax
	lea	ecx, DWORD PTR _rWindow$76320[ebp]
	call	?TopLeft@CRect@@QAEAAVCPoint@@XZ	; CRect::TopLeft
	mov	ecx, eax
	call	??GCPoint@@QBE?AV0@XZ			; CPoint::operator-
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR $T76747[ebp], ecx
	mov	DWORD PTR $T76747[ebp+4], edx
	mov	eax, DWORD PTR $T76747[ebp+4]
	push	eax
	mov	ecx, DWORD PTR $T76747[ebp]
	push	ecx
	lea	ecx, DWORD PTR _rect$[ebp]
	call	?OffsetRect@CRect@@QAEXUtagPOINT@@@Z	; CRect::OffsetRect

; 300  : 		pDC->ExcludeClipRect(rect);

	lea	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?ExcludeClipRect@CDC@@QAEHPBUtagRECT@@@Z ; CDC::ExcludeClipRect

; 301  : 
; 302  : 		rect = rWindow;

	mov	edx, DWORD PTR _rWindow$76320[ebp]
	mov	DWORD PTR _rect$[ebp], edx
	mov	eax, DWORD PTR _rWindow$76320[ebp+4]
	mov	DWORD PTR _rect$[ebp+4], eax
	mov	ecx, DWORD PTR _rWindow$76320[ebp+8]
	mov	DWORD PTR _rect$[ebp+8], ecx
	mov	edx, DWORD PTR _rWindow$76320[ebp+12]
	mov	DWORD PTR _rect$[ebp+12], edx

; 303  : 		rect.OffsetRect(-rect.TopLeft());

	lea	eax, DWORD PTR $T76750[ebp]
	push	eax
	lea	ecx, DWORD PTR _rect$[ebp]
	call	?TopLeft@CRect@@QAEAAVCPoint@@XZ	; CRect::TopLeft
	mov	ecx, eax
	call	??GCPoint@@QBE?AV0@XZ			; CPoint::operator-
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR $T76749[ebp], ecx
	mov	DWORD PTR $T76749[ebp+4], edx
	mov	eax, DWORD PTR $T76749[ebp+4]
	push	eax
	mov	ecx, DWORD PTR $T76749[ebp]
	push	ecx
	lea	ecx, DWORD PTR _rect$[ebp]
	call	?OffsetRect@CRect@@QAEXUtagPOINT@@@Z	; CRect::OffsetRect
$L76319:

; 305  : 
; 306  : 	if (HasBkgndColor())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?HasBkgndColor@CEnToolBar@@IBEHXZ	; CEnToolBar::HasBkgndColor
	test	eax, eax
	je	$L76325

; 308  : 		if (m_crTo == m_crFrom)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+180]
	cmp	ecx, DWORD PTR [eax+176]
	jne	SHORT $L76326

; 310  : 			pDC->FillSolidRect(rect, m_crFrom);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+176]
	push	eax
	lea	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect

; 312  : 		else

	jmp	$L76341
$L76326:

; 314  : 			GM_GRADIENT nType = GraphicsMisc::GetGradientType(m_bGlass, m_bGradient);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+184]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+188]
	push	ecx
	call	?GetGradientType@GraphicsMisc@@YA?AW4GM_GRADIENT@@HH@Z ; GraphicsMisc::GetGradientType
	add	esp, 8
	mov	DWORD PTR _nType$76328[ebp], eax

; 315  : 
; 316  : 			// if rendering the non-client background
; 317  : 			// first fill the top and bottom borders
; 318  : 			if (!bClient)

	cmp	DWORD PTR _bClient$[ebp], 0
	jne	$L76332

; 320  : 				if (m_cyTopBorder > 0)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+76], 0
	jle	SHORT $L76330

; 322  : 					CRect rBorder(rect);

	mov	eax, DWORD PTR _rect$[ebp]
	mov	DWORD PTR _rBorder$76331[ebp], eax
	mov	ecx, DWORD PTR _rect$[ebp+4]
	mov	DWORD PTR _rBorder$76331[ebp+4], ecx
	mov	edx, DWORD PTR _rect$[ebp+8]
	mov	DWORD PTR _rBorder$76331[ebp+8], edx
	mov	eax, DWORD PTR _rect$[ebp+12]
	mov	DWORD PTR _rBorder$76331[ebp+12], eax

; 323  : 					rBorder.bottom = m_cyTopBorder;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+76]
	mov	DWORD PTR _rBorder$76331[ebp+12], edx

; 324  : 					pDC->FillSolidRect(rBorder, GraphicsMisc::GetGradientEdgeColor(nType, m_crFrom, TRUE));

	push	1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+176]
	push	ecx
	mov	edx, DWORD PTR _nType$76328[ebp]
	push	edx
	call	?GetGradientEdgeColor@GraphicsMisc@@YAKW4GM_GRADIENT@@KH@Z ; GraphicsMisc::GetGradientEdgeColor
	add	esp, 12					; 0000000cH
	push	eax
	lea	ecx, DWORD PTR _rBorder$76331[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect

; 325  : 
; 326  : 					// update rect
; 327  : 					rect.top = rBorder.bottom;

	mov	eax, DWORD PTR _rBorder$76331[ebp+12]
	mov	DWORD PTR _rect$[ebp+4], eax
$L76330:

; 329  : 
; 330  : 				if (m_cyBottomBorder > 0)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+80], 0
	jle	SHORT $L76332

; 332  : 					CRect rBorder(rect);

	mov	edx, DWORD PTR _rect$[ebp]
	mov	DWORD PTR _rBorder$76333[ebp], edx
	mov	eax, DWORD PTR _rect$[ebp+4]
	mov	DWORD PTR _rBorder$76333[ebp+4], eax
	mov	ecx, DWORD PTR _rect$[ebp+8]
	mov	DWORD PTR _rBorder$76333[ebp+8], ecx
	mov	edx, DWORD PTR _rect$[ebp+12]
	mov	DWORD PTR _rBorder$76333[ebp+12], edx

; 333  : 					rBorder.top = (rect.bottom - m_cyBottomBorder);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _rect$[ebp+12]
	sub	ecx, DWORD PTR [eax+80]
	mov	DWORD PTR _rBorder$76333[ebp+4], ecx

; 334  : 					pDC->FillSolidRect(rBorder, GraphicsMisc::GetGradientEdgeColor(nType, m_crTo, FALSE));

	push	0
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+180]
	push	eax
	mov	ecx, DWORD PTR _nType$76328[ebp]
	push	ecx
	call	?GetGradientEdgeColor@GraphicsMisc@@YAKW4GM_GRADIENT@@KH@Z ; GraphicsMisc::GetGradientEdgeColor
	add	esp, 12					; 0000000cH
	push	eax
	lea	ecx, DWORD PTR _rBorder$76333[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect

; 335  : 
; 336  : 					// update rect
; 337  : 					rect.bottom = rBorder.top;

	mov	edx, DWORD PTR _rBorder$76333[ebp+4]
	mov	DWORD PTR _rect$[ebp+12], edx
$L76332:

; 340  : 
; 341  : 			// draw the non-client rows only if we have 
; 342  : 			// a left or right border
; 343  : 			if (bClient || (m_cxLeftBorder > 0) || (m_cxRightBorder > 0))

	cmp	DWORD PTR _bClient$[ebp], 0
	jne	SHORT $L76335
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+68], 0
	jg	SHORT $L76335
	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+72], 0
	jle	$L76341
$L76335:

; 345  : 				int nNumRows = m_aRowHeights.GetSize();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	?GetSize@CUIntArray@@QBEHXZ		; CUIntArray::GetSize
	mov	DWORD PTR _nNumRows$76336[ebp], eax

; 346  : 				CRect rRow(rect);

	mov	edx, DWORD PTR _rect$[ebp]
	mov	DWORD PTR _rRow$76337[ebp], edx
	mov	eax, DWORD PTR _rect$[ebp+4]
	mov	DWORD PTR _rRow$76337[ebp+4], eax
	mov	ecx, DWORD PTR _rect$[ebp+8]
	mov	DWORD PTR _rRow$76337[ebp+8], ecx
	mov	edx, DWORD PTR _rect$[ebp+12]
	mov	DWORD PTR _rRow$76337[ebp+12], edx

; 347  : 
; 348  : 				for (int nRow = 0; nRow < nNumRows; nRow++)

	mov	DWORD PTR _nRow$76338[ebp], 0
	jmp	SHORT $L76339
$L76340:
	mov	eax, DWORD PTR _nRow$76338[ebp]
	add	eax, 1
	mov	DWORD PTR _nRow$76338[ebp], eax
$L76339:
	mov	ecx, DWORD PTR _nRow$76338[ebp]
	cmp	ecx, DWORD PTR _nNumRows$76336[ebp]
	jge	SHORT $L76341

; 350  : 					rRow.bottom = rRow.top + m_aRowHeights[nRow];

	mov	edx, DWORD PTR _nRow$76338[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	??ACUIntArray@@QAEAAIH@Z		; CUIntArray::operator[]
	mov	ecx, DWORD PTR _rRow$76337[ebp+4]
	add	ecx, DWORD PTR [eax]
	mov	DWORD PTR _rRow$76337[ebp+12], ecx

; 351  : 					
; 352  : 					GraphicsMisc::DrawGradient(nType, pDC, rRow, m_crFrom, m_crTo, FALSE);

	push	-1
	push	0
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+180]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+176]
	push	edx
	lea	ecx, DWORD PTR _rRow$76337[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	eax, DWORD PTR _pDC$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nType$76328[ebp]
	push	ecx
	call	?DrawGradient@GraphicsMisc@@YAHW4GM_GRADIENT@@PAVCDC@@PBUtagRECT@@KKHH@Z ; GraphicsMisc::DrawGradient
	add	esp, 28					; 0000001cH

; 353  : 
; 354  : 					// next row
; 355  : 					rRow.top = rRow.bottom;

	mov	edx, DWORD PTR _rRow$76337[ebp+12]
	mov	DWORD PTR _rRow$76337[ebp+4], edx

; 356  : 				}

	jmp	SHORT $L76340
$L76341:

; 360  : 	else

	jmp	SHORT $L76342
$L76325:

; 362  : 		pDC->FillSolidRect(rect, GetSysColor(COLOR_3DFACE));

	mov	esi, esp
	push	15					; 0000000fH
	call	DWORD PTR __imp__GetSysColor@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	lea	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect
$L76342:

; 364  : }

	pop	edi
	pop	esi
	add	esp, 128				; 00000080H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?DrawBkgnd@CEnToolBar@@IAEXPAVCDC@@H@Z ENDP		; CEnToolBar::DrawBkgnd
_TEXT	ENDS
;	COMDAT ?HasBkgndColor@CEnToolBar@@IBEHXZ
_TEXT	SEGMENT
_this$ = -4
?HasBkgndColor@CEnToolBar@@IBEHXZ PROC NEAR		; CEnToolBar::HasBkgndColor, COMDAT

; 73   : 	BOOL HasBkgndColor() const { return m_crFrom != CLR_NONE; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	cmp	DWORD PTR [eax+176], -1
	setne	cl
	mov	eax, ecx
	mov	esp, ebp
	pop	ebp
	ret	0
?HasBkgndColor@CEnToolBar@@IBEHXZ ENDP			; CEnToolBar::HasBkgndColor
_TEXT	ENDS
EXTRN	?CommandToIndex@CToolBar@@QBEHI@Z:NEAR		; CToolBar::CommandToIndex
EXTRN	?GetButtonStyle@CToolBar@@QBEIH@Z:NEAR		; CToolBar::GetButtonStyle
EXTRN	?SetButtonStyle@CToolBar@@QAEXHI@Z:NEAR		; CToolBar::SetButtonStyle
EXTRN	?Add@CUIntArray@@QAEHI@Z:NEAR			; CUIntArray::Add
EXTRN	?OnSettingChange@CWnd@@IAEXIPBG@Z:NEAR		; CWnd::OnSettingChange
;	COMDAT xdata$x
; File D:\_CODE\Shared\entoolbar.cpp
xdata$x	SEGMENT
$T76757	DD	019930520H
	DD	01H
	DD	FLAT:$T76759
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76759	DD	0ffffffffH
	DD	FLAT:$L76755
xdata$x	ENDS
;	COMDAT ?OnSettingChange@CEnToolBar@@IAEXIPBG@Z
_TEXT	SEGMENT
_uFlags$ = 8
_lpszSection$ = 12
_this$ = -60
_aDropBtns$ = -32
_nBtn$ = -36
_nBtnID$76353 = -44
_dwStyle$76354 = -48
_nItem$ = -40
_nBtn$76360 = -52
_dwStyle$76361 = -56
__$EHRec$ = -12
?OnSettingChange@CEnToolBar@@IAEXIPBG@Z PROC NEAR	; CEnToolBar::OnSettingChange, COMDAT

; 367  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76758
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 48					; 00000030H
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-60]
	mov	ecx, 12					; 0000000cH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 368  : 	// toolbar button sizes get screwed if any button has the drop style applied
; 369  : 	// at the time this is handled. to get round it we remove any such button styles
; 370  : 	// and then readd them after the default processing
; 371  : 	CUIntArray aDropBtns;

	lea	ecx, DWORD PTR _aDropBtns$[ebp]
	call	??0CUIntArray@@QAE@XZ			; CUIntArray::CUIntArray
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 372  : 	int nBtn = GetToolBarCtrl().GetButtonCount();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?GetButtonCount@CToolBarCtrl@@QBEHXZ	; CToolBarCtrl::GetButtonCount
	mov	DWORD PTR _nBtn$[ebp], eax
$L76351:

; 373  : 
; 374  : 	while (nBtn--)

	mov	eax, DWORD PTR _nBtn$[ebp]
	mov	ecx, DWORD PTR _nBtn$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nBtn$[ebp], ecx
	test	eax, eax
	je	SHORT $L76352

; 376  : 		UINT nBtnID = GetItemID(nBtn);

	mov	edx, DWORD PTR _nBtn$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetItemID@CToolBar@@QBEIH@Z		; CToolBar::GetItemID
	mov	DWORD PTR _nBtnID$76353[ebp], eax

; 377  : 		DWORD dwStyle = GetButtonStyle(nBtn);

	mov	eax, DWORD PTR _nBtn$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetButtonStyle@CToolBar@@QBEIH@Z	; CToolBar::GetButtonStyle
	mov	DWORD PTR _dwStyle$76354[ebp], eax

; 378  : 		
; 379  : 		if (dwStyle & TBSTYLE_DROPDOWN)

	mov	ecx, DWORD PTR _dwStyle$76354[ebp]
	and	ecx, 8
	test	ecx, ecx
	je	SHORT $L76355

; 381  : 			SetButtonStyle(nBtn, dwStyle & ~TBSTYLE_DROPDOWN);

	mov	edx, DWORD PTR _dwStyle$76354[ebp]
	and	edx, -9					; fffffff7H
	push	edx
	mov	eax, DWORD PTR _nBtn$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetButtonStyle@CToolBar@@QAEXHI@Z	; CToolBar::SetButtonStyle

; 382  : 			aDropBtns.Add(nBtnID);

	mov	ecx, DWORD PTR _nBtnID$76353[ebp]
	push	ecx
	lea	ecx, DWORD PTR _aDropBtns$[ebp]
	call	?Add@CUIntArray@@QAEHI@Z		; CUIntArray::Add
$L76355:

; 384  : 	}

	jmp	SHORT $L76351
$L76352:

; 385  : 
; 386  : 	// default processing
; 387  : 	CToolBar::OnSettingChange(uFlags, lpszSection);

	mov	edx, DWORD PTR _lpszSection$[ebp]
	push	edx
	mov	eax, DWORD PTR _uFlags$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnSettingChange@CWnd@@IAEXIPBG@Z	; CWnd::OnSettingChange

; 388  : 
; 389  : 	// restore styles
; 390  : 	int nItem = aDropBtns.GetSize();

	lea	ecx, DWORD PTR _aDropBtns$[ebp]
	call	?GetSize@CUIntArray@@QBEHXZ		; CUIntArray::GetSize
	mov	DWORD PTR _nItem$[ebp], eax
$L76358:

; 391  : 
; 392  : 	while (nItem--)

	mov	ecx, DWORD PTR _nItem$[ebp]
	mov	edx, DWORD PTR _nItem$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nItem$[ebp], edx
	test	ecx, ecx
	je	SHORT $L76359

; 394  : 		int nBtn = CommandToIndex(aDropBtns[nItem]);

	mov	eax, DWORD PTR _nItem$[ebp]
	push	eax
	lea	ecx, DWORD PTR _aDropBtns$[ebp]
	call	??ACUIntArray@@QAEAAIH@Z		; CUIntArray::operator[]
	mov	ecx, DWORD PTR [eax]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?CommandToIndex@CToolBar@@QBEHI@Z	; CToolBar::CommandToIndex
	mov	DWORD PTR _nBtn$76360[ebp], eax

; 395  : 		DWORD dwStyle = GetButtonStyle(nBtn);

	mov	edx, DWORD PTR _nBtn$76360[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetButtonStyle@CToolBar@@QBEIH@Z	; CToolBar::GetButtonStyle
	mov	DWORD PTR _dwStyle$76361[ebp], eax

; 396  : 	
; 397  : 		SetButtonStyle(nBtn, dwStyle | TBSTYLE_DROPDOWN);

	mov	eax, DWORD PTR _dwStyle$76361[ebp]
	or	al, 8
	push	eax
	mov	ecx, DWORD PTR _nBtn$76360[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetButtonStyle@CToolBar@@QAEXHI@Z	; CToolBar::SetButtonStyle

; 398  : 	}

	jmp	SHORT $L76358
$L76359:

; 399  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _aDropBtns$[ebp]
	call	??1CUIntArray@@UAE@XZ			; CUIntArray::~CUIntArray
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 60					; 0000003cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76755:
	lea	ecx, DWORD PTR _aDropBtns$[ebp]
	call	??1CUIntArray@@UAE@XZ			; CUIntArray::~CUIntArray
	ret	0
$L76758:
	mov	eax, OFFSET FLAT:$T76757
	jmp	___CxxFrameHandler
text$x	ENDS
?OnSettingChange@CEnToolBar@@IAEXIPBG@Z ENDP		; CEnToolBar::OnSettingChange
PUBLIC	?EstimateHeightRequired@CEnToolBar@@IBEHH@Z	; CEnToolBar::EstimateHeightRequired
PUBLIC	?EstimateRowsRequired@CEnToolBar@@IBEHH@Z	; CEnToolBar::EstimateRowsRequired
;	COMDAT ?EstimateHeightRequired@CEnToolBar@@IBEHH@Z
_TEXT	SEGMENT
_cx$ = 8
_this$ = -12
_nRows$ = -8
_nRowHeight$ = -4
?EstimateHeightRequired@CEnToolBar@@IBEHH@Z PROC NEAR	; CEnToolBar::EstimateHeightRequired, COMDAT

; 402  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 20					; 00000014H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 403  : 	int nRows = EstimateRowsRequired(cx);

	mov	eax, DWORD PTR _cx$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?EstimateRowsRequired@CEnToolBar@@IBEHH@Z ; CEnToolBar::EstimateRowsRequired
	mov	DWORD PTR _nRows$[ebp], eax

; 404  : 	int nRowHeight = (nRows == 1) ? (m_sizeButton.cy + 2) : 
; 405  : 					(nRows < 4) ? (m_sizeButton.cy + 4) : (m_sizeButton.cy + 6);

	cmp	DWORD PTR _nRows$[ebp], 1
	jne	SHORT $L76766
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+152]
	add	edx, 2
	mov	DWORD PTR -16+[ebp], edx
	jmp	SHORT $L76767
$L76766:
	cmp	DWORD PTR _nRows$[ebp], 4
	jge	SHORT $L76764
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+152]
	add	ecx, 4
	mov	DWORD PTR -20+[ebp], ecx
	jmp	SHORT $L76765
$L76764:
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+152]
	add	eax, 6
	mov	DWORD PTR -20+[ebp], eax
$L76765:
	mov	ecx, DWORD PTR -20+[ebp]
	mov	DWORD PTR -16+[ebp], ecx
$L76767:
	mov	edx, DWORD PTR -16+[ebp]
	mov	DWORD PTR _nRowHeight$[ebp], edx

; 406  : 
; 407  : 	return ((nRows * nRowHeight) + m_cyTopBorder + m_cyBottomBorder);

	mov	eax, DWORD PTR _nRows$[ebp]
	imul	eax, DWORD PTR _nRowHeight$[ebp]
	mov	ecx, DWORD PTR _this$[ebp]
	add	eax, DWORD PTR [ecx+76]
	mov	edx, DWORD PTR _this$[ebp]
	add	eax, DWORD PTR [edx+80]

; 408  : }

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?EstimateHeightRequired@CEnToolBar@@IBEHH@Z ENDP	; CEnToolBar::EstimateHeightRequired
_TEXT	ENDS
PUBLIC	?GetMinReqLength@CEnToolBar@@QBEHXZ		; CEnToolBar::GetMinReqLength
EXTRN	?Width@CRect@@QBEHXZ:NEAR			; CRect::Width
;	COMDAT ?GetMinReqLength@CEnToolBar@@QBEHXZ
_TEXT	SEGMENT
_this$ = -28
_nLen$ = -8
_nBtn$ = -4
_rBtn$76376 = -24
?GetMinReqLength@CEnToolBar@@QBEHXZ PROC NEAR		; CEnToolBar::GetMinReqLength, COMDAT

; 411  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 28					; 0000001cH
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 412  : 	int nLen = (m_cxLeftBorder + m_cxLeftBorder);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+68]
	mov	edx, DWORD PTR _this$[ebp]
	add	ecx, DWORD PTR [edx+68]
	mov	DWORD PTR _nLen$[ebp], ecx

; 413  : 	int nBtn = GetButtonCount();

	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetButtonCount@CEnToolBar@@QBEHH@Z	; CEnToolBar::GetButtonCount
	mov	DWORD PTR _nBtn$[ebp], eax
$L76374:

; 414  : 
; 415  : 	while (nBtn--)

	mov	eax, DWORD PTR _nBtn$[ebp]
	mov	ecx, DWORD PTR _nBtn$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nBtn$[ebp], ecx
	test	eax, eax
	je	SHORT $L76375

; 417  : 		CRect rBtn;

	lea	ecx, DWORD PTR _rBtn$76376[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 418  : 		GetItemRect(nBtn, rBtn);

	lea	ecx, DWORD PTR _rBtn$76376[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	mov	esi, esp
	push	eax
	mov	edx, DWORD PTR _nBtn$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+232]
	cmp	esi, esp
	call	__chkesp

; 419  : 
; 420  : 		nLen += rBtn.Width();

	lea	ecx, DWORD PTR _rBtn$76376[ebp]
	call	?Width@CRect@@QBEHXZ			; CRect::Width
	mov	ecx, DWORD PTR _nLen$[ebp]
	add	ecx, eax
	mov	DWORD PTR _nLen$[ebp], ecx

; 421  : 	}

	jmp	SHORT $L76374
$L76375:

; 422  : 
; 423  : 	return nLen;

	mov	eax, DWORD PTR _nLen$[ebp]

; 424  : }

	pop	esi
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetMinReqLength@CEnToolBar@@QBEHXZ ENDP		; CEnToolBar::GetMinReqLength
_TEXT	ENDS
;	COMDAT ?EstimateRowsRequired@CEnToolBar@@IBEHH@Z
_TEXT	SEGMENT
_cx$ = 8
_this$ = -36
_nRows$ = -12
_nXPos$ = -4
_nBtnCount$ = -16
_nBtn$ = -8
_rBtn$76388 = -32
?EstimateRowsRequired@CEnToolBar@@IBEHH@Z PROC NEAR	; CEnToolBar::EstimateRowsRequired, COMDAT

; 427  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 36					; 00000024H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 428  : 	int nRows = 1;

	mov	DWORD PTR _nRows$[ebp], 1

; 429  : 	int nXPos = 0;

	mov	DWORD PTR _nXPos$[ebp], 0

; 430  : 	int nBtnCount = GetButtonCount();

	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetButtonCount@CEnToolBar@@QBEHH@Z	; CEnToolBar::GetButtonCount
	mov	DWORD PTR _nBtnCount$[ebp], eax

; 431  : 
; 432  : 	for (int nBtn = 0; nBtn < nBtnCount; nBtn++)

	mov	DWORD PTR _nBtn$[ebp], 0
	jmp	SHORT $L76385
$L76386:
	mov	eax, DWORD PTR _nBtn$[ebp]
	add	eax, 1
	mov	DWORD PTR _nBtn$[ebp], eax
$L76385:
	mov	ecx, DWORD PTR _nBtn$[ebp]
	cmp	ecx, DWORD PTR _nBtnCount$[ebp]
	jge	SHORT $L76387

; 434  : 		CRect rBtn;

	lea	ecx, DWORD PTR _rBtn$76388[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 435  : 		GetItemRect(nBtn, rBtn);

	lea	ecx, DWORD PTR _rBtn$76388[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	mov	esi, esp
	push	eax
	mov	edx, DWORD PTR _nBtn$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+232]
	cmp	esi, esp
	call	__chkesp

; 436  : 
; 437  : 		if (nXPos) // a button has been added

	cmp	DWORD PTR _nXPos$[ebp], 0
	je	SHORT $L76390

; 439  : 			if (nXPos + rBtn.Width() > cx)

	lea	ecx, DWORD PTR _rBtn$76388[ebp]
	call	?Width@CRect@@QBEHXZ			; CRect::Width
	mov	ecx, DWORD PTR _nXPos$[ebp]
	add	ecx, eax
	cmp	ecx, DWORD PTR _cx$[ebp]
	jle	SHORT $L76390

; 441  : 				nRows++;

	mov	edx, DWORD PTR _nRows$[ebp]
	add	edx, 1
	mov	DWORD PTR _nRows$[ebp], edx

; 442  : 				nXPos = 0;

	mov	DWORD PTR _nXPos$[ebp], 0
$L76390:

; 445  : 		
; 446  : 		// else
; 447  : 		nXPos += rBtn.Width();

	lea	ecx, DWORD PTR _rBtn$76388[ebp]
	call	?Width@CRect@@QBEHXZ			; CRect::Width
	mov	ecx, DWORD PTR _nXPos$[ebp]
	add	ecx, eax
	mov	DWORD PTR _nXPos$[ebp], ecx

; 448  : 	}

	jmp	SHORT $L76386
$L76387:

; 449  : 
; 450  : 	return nRows;

	mov	eax, DWORD PTR _nRows$[ebp]

; 451  : }

	pop	esi
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?EstimateRowsRequired@CEnToolBar@@IBEHH@Z ENDP		; CEnToolBar::EstimateRowsRequired
_TEXT	ENDS
PUBLIC	?RefreshRowHeights@CEnToolBar@@IAEHXZ		; CEnToolBar::RefreshRowHeights
EXTRN	?RemoveAll@CUIntArray@@QAEXXZ:NEAR		; CUIntArray::RemoveAll
;	COMDAT ?RefreshRowHeights@CEnToolBar@@IAEHXZ
_TEXT	SEGMENT
_this$ = -68
_nBtnCount$ = -16
_nRowHead$ = -40
_nMaxRowHeight$ = -36
_nTotalHeight$ = -4
_rBtn$ = -32
_nBtn$ = -8
_rNext$76404 = -56
_nRowBase$76406 = -64
_nRowHeight$76407 = -60
_nLastRow$ = -12
?RefreshRowHeights@CEnToolBar@@IAEHXZ PROC NEAR		; CEnToolBar::RefreshRowHeights, COMDAT

; 454  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 72					; 00000048H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-72]
	mov	ecx, 18					; 00000012H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 455  : 	m_aRowHeights.RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	?RemoveAll@CUIntArray@@QAEXXZ		; CUIntArray::RemoveAll

; 456  : 
; 457  : 	int nBtnCount = GetToolBarCtrl().GetButtonCount();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?GetButtonCount@CToolBarCtrl@@QBEHXZ	; CToolBarCtrl::GetButtonCount
	mov	DWORD PTR _nBtnCount$[ebp], eax

; 458  : 
; 459  : 	if (nBtnCount == 0)

	cmp	DWORD PTR _nBtnCount$[ebp], 0
	jne	SHORT $L76395

; 460  : 		return 0;

	xor	eax, eax
	jmp	$L76393
$L76395:

; 461  : 
; 462  : 	// work thru the buttons noticing the changes in vertical position
; 463  : 	int nRowHead = 0;

	mov	DWORD PTR _nRowHead$[ebp], 0

; 464  : 	int nMaxRowHeight = 0, nTotalHeight = 0;

	mov	DWORD PTR _nMaxRowHeight$[ebp], 0
	mov	DWORD PTR _nTotalHeight$[ebp], 0

; 465  : 
; 466  : 	// get the first button rect as our start point
; 467  : 	CRect rBtn;

	lea	ecx, DWORD PTR _rBtn$[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 468  : 	GetItemRect(0, rBtn);

	lea	ecx, DWORD PTR _rBtn$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	mov	esi, esp
	push	eax
	push	0
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+232]
	cmp	esi, esp
	call	__chkesp

; 469  : 
; 470  : 	for (int nBtn = 1; nBtn < nBtnCount; nBtn++)

	mov	DWORD PTR _nBtn$[ebp], 1
	jmp	SHORT $L76401
$L76402:
	mov	eax, DWORD PTR _nBtn$[ebp]
	add	eax, 1
	mov	DWORD PTR _nBtn$[ebp], eax
$L76401:
	mov	ecx, DWORD PTR _nBtn$[ebp]
	cmp	ecx, DWORD PTR _nBtnCount$[ebp]
	jge	$L76403

; 472  : 		CRect rNext;

	lea	ecx, DWORD PTR _rNext$76404[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 473  : 		GetItemRect(nBtn, rNext);

	lea	ecx, DWORD PTR _rNext$76404[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	mov	esi, esp
	push	eax
	mov	edx, DWORD PTR _nBtn$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+232]
	cmp	esi, esp
	call	__chkesp

; 474  : 
; 475  : 		if (rNext.top > rBtn.top) // new row

	mov	eax, DWORD PTR _rNext$76404[ebp+4]
	cmp	eax, DWORD PTR _rBtn$[ebp+4]
	jle	SHORT $L76405

; 477  : 			// add current row
; 478  : 			int nRowBase = (rBtn.bottom + rNext.top) / 2;

	mov	eax, DWORD PTR _rBtn$[ebp+12]
	add	eax, DWORD PTR _rNext$76404[ebp+4]
	cdq
	sub	eax, edx
	sar	eax, 1
	mov	DWORD PTR _nRowBase$76406[ebp], eax

; 479  : 			int nRowHeight = nRowBase - nRowHead;

	mov	ecx, DWORD PTR _nRowBase$76406[ebp]
	sub	ecx, DWORD PTR _nRowHead$[ebp]
	mov	DWORD PTR _nRowHeight$76407[ebp], ecx

; 480  : 
; 481  : 			m_aRowHeights.Add(nRowHeight);

	mov	edx, DWORD PTR _nRowHeight$76407[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	?Add@CUIntArray@@QAEHI@Z		; CUIntArray::Add

; 482  : 
; 483  : 			// keep track of max row height
; 484  : 			if (nRowHeight > nMaxRowHeight)

	mov	eax, DWORD PTR _nRowHeight$76407[ebp]
	cmp	eax, DWORD PTR _nMaxRowHeight$[ebp]
	jle	SHORT $L76408

; 485  : 				nMaxRowHeight = nRowHeight;

	mov	ecx, DWORD PTR _nRowHeight$76407[ebp]
	mov	DWORD PTR _nMaxRowHeight$[ebp], ecx
$L76408:

; 486  : 
; 487  : 			// and total row height
; 488  : 			nTotalHeight += nRowHeight;

	mov	edx, DWORD PTR _nTotalHeight$[ebp]
	add	edx, DWORD PTR _nRowHeight$76407[ebp]
	mov	DWORD PTR _nTotalHeight$[ebp], edx

; 489  : 
; 490  : 			// next row
; 491  : 			rBtn = rNext;

	mov	eax, DWORD PTR _rNext$76404[ebp]
	mov	DWORD PTR _rBtn$[ebp], eax
	mov	ecx, DWORD PTR _rNext$76404[ebp+4]
	mov	DWORD PTR _rBtn$[ebp+4], ecx
	mov	edx, DWORD PTR _rNext$76404[ebp+8]
	mov	DWORD PTR _rBtn$[ebp+8], edx
	mov	eax, DWORD PTR _rNext$76404[ebp+12]
	mov	DWORD PTR _rBtn$[ebp+12], eax

; 492  : 			nRowHead = nRowBase;

	mov	ecx, DWORD PTR _nRowBase$76406[ebp]
	mov	DWORD PTR _nRowHead$[ebp], ecx
$L76405:

; 494  : 	}

	jmp	$L76402
$L76403:

; 495  : 
; 496  : 	// last row is the maximum of the button base + 2,
; 497  : 	int nLastRow = (rBtn.bottom + 2 - nRowHead);

	mov	edx, DWORD PTR _rBtn$[ebp+12]
	add	edx, 2
	sub	edx, DWORD PTR _nRowHead$[ebp]
	mov	DWORD PTR _nLastRow$[ebp], edx

; 498  : 	
; 499  : 	// or the maximum row height calculated
; 500  : 	nLastRow = max(nMaxRowHeight, nLastRow);

	mov	eax, DWORD PTR _nMaxRowHeight$[ebp]
	cmp	eax, DWORD PTR _nLastRow$[ebp]
	jle	SHORT $L76774
	mov	ecx, DWORD PTR _nMaxRowHeight$[ebp]
	mov	DWORD PTR -72+[ebp], ecx
	jmp	SHORT $L76775
$L76774:
	mov	edx, DWORD PTR _nLastRow$[ebp]
	mov	DWORD PTR -72+[ebp], edx
$L76775:
	mov	eax, DWORD PTR -72+[ebp]
	mov	DWORD PTR _nLastRow$[ebp], eax

; 501  : 	
; 502  : 	m_aRowHeights.Add(nLastRow);

	mov	ecx, DWORD PTR _nLastRow$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	?Add@CUIntArray@@QAEHI@Z		; CUIntArray::Add

; 503  : 	nTotalHeight += (nLastRow + m_cyBottomBorder + m_cyTopBorder);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nLastRow$[ebp]
	add	eax, DWORD PTR [edx+80]
	mov	ecx, DWORD PTR _this$[ebp]
	add	eax, DWORD PTR [ecx+76]
	mov	edx, DWORD PTR _nTotalHeight$[ebp]
	add	edx, eax
	mov	DWORD PTR _nTotalHeight$[ebp], edx

; 504  : 
; 505  : 	return nTotalHeight;

	mov	eax, DWORD PTR _nTotalHeight$[ebp]
$L76393:

; 506  : }

	pop	edi
	pop	esi
	add	esp, 72					; 00000048H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?RefreshRowHeights@CEnToolBar@@IAEHXZ ENDP		; CEnToolBar::RefreshRowHeights
_TEXT	ENDS
PUBLIC	?Resize@CEnToolBar@@QAEHHVCPoint@@@Z		; CEnToolBar::Resize
EXTRN	?MoveWindow@CWnd@@QAEXPBUtagRECT@@H@Z:NEAR	; CWnd::MoveWindow
EXTRN	??0CRect@@QAE@UtagPOINT@@UtagSIZE@@@Z:NEAR	; CRect::CRect
;	COMDAT ?Resize@CEnToolBar@@QAEHHVCPoint@@@Z
_TEXT	SEGMENT
_cx$ = 8
_ptTopLeft$ = 12
_this$ = -52
_nEstHeight$ = -20
_rToolbar$ = -16
_nRealHeight$ = -24
$T76778 = -32
$T76779 = -40
$T76780 = -48
?Resize@CEnToolBar@@QAEHHVCPoint@@@Z PROC NEAR		; CEnToolBar::Resize, COMDAT

; 509  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 52					; 00000034H
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-52]
	mov	ecx, 13					; 0000000dH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 510  : 	int nEstHeight = EstimateHeightRequired(cx);

	mov	eax, DWORD PTR _cx$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?EstimateHeightRequired@CEnToolBar@@IBEHH@Z ; CEnToolBar::EstimateHeightRequired
	mov	DWORD PTR _nEstHeight$[ebp], eax

; 511  : 	CRect rToolbar(ptTopLeft, CSize(cx, nEstHeight));

	mov	ecx, DWORD PTR _nEstHeight$[ebp]
	push	ecx
	mov	edx, DWORD PTR _cx$[ebp]
	push	edx
	lea	ecx, DWORD PTR $T76779[ebp]
	call	??0CSize@@QAE@HH@Z			; CSize::CSize
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR $T76778[ebp], ecx
	mov	DWORD PTR $T76778[ebp+4], edx
	mov	eax, DWORD PTR $T76778[ebp+4]
	push	eax
	mov	ecx, DWORD PTR $T76778[ebp]
	push	ecx
	mov	edx, DWORD PTR _ptTopLeft$[ebp]
	mov	DWORD PTR $T76780[ebp], edx
	mov	eax, DWORD PTR _ptTopLeft$[ebp+4]
	mov	DWORD PTR $T76780[ebp+4], eax
	mov	ecx, DWORD PTR $T76780[ebp+4]
	push	ecx
	mov	edx, DWORD PTR $T76780[ebp]
	push	edx
	lea	ecx, DWORD PTR _rToolbar$[ebp]
	call	??0CRect@@QAE@UtagPOINT@@UtagSIZE@@@Z	; CRect::CRect

; 512  : 
; 513  : 	MoveWindow(rToolbar);

	push	1
	lea	ecx, DWORD PTR _rToolbar$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?MoveWindow@CWnd@@QAEXPBUtagRECT@@H@Z	; CWnd::MoveWindow

; 514  : 	
; 515  : 	int nRealHeight = RefreshRowHeights();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RefreshRowHeights@CEnToolBar@@IAEHXZ	; CEnToolBar::RefreshRowHeights
	mov	DWORD PTR _nRealHeight$[ebp], eax

; 516  : 
; 517  : 	if (nRealHeight != nEstHeight)

	mov	eax, DWORD PTR _nRealHeight$[ebp]
	cmp	eax, DWORD PTR _nEstHeight$[ebp]
	je	SHORT $L76421

; 519  : 		rToolbar.bottom = rToolbar.top + nRealHeight;

	mov	ecx, DWORD PTR _rToolbar$[ebp+4]
	add	ecx, DWORD PTR _nRealHeight$[ebp]
	mov	DWORD PTR _rToolbar$[ebp+12], ecx

; 520  : 		MoveWindow(rToolbar);

	push	1
	lea	ecx, DWORD PTR _rToolbar$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?MoveWindow@CWnd@@QAEXPBUtagRECT@@H@Z	; CWnd::MoveWindow
$L76421:

; 522  : 
; 523  : 	return nRealHeight;

	mov	eax, DWORD PTR _nRealHeight$[ebp]

; 524  : }

	pop	edi
	add	esp, 52					; 00000034H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?Resize@CEnToolBar@@QAEHHVCPoint@@@Z ENDP		; CEnToolBar::Resize
_TEXT	ENDS
PUBLIC	?GetHeight@CEnToolBar@@QBEHXZ			; CEnToolBar::GetHeight
EXTRN	?Height@CRect@@QBEHXZ:NEAR			; CRect::Height
;	COMDAT ?GetHeight@CEnToolBar@@QBEHXZ
_TEXT	SEGMENT
_this$ = -20
_rToolbar$ = -16
?GetHeight@CEnToolBar@@QBEHXZ PROC NEAR			; CEnToolBar::GetHeight, COMDAT

; 527  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 20					; 00000014H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 528  : 	CRect rToolbar;

	lea	ecx, DWORD PTR _rToolbar$[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 529  : 	GetWindowRect(rToolbar);

	lea	ecx, DWORD PTR _rToolbar$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetWindowRect@CWnd@@QBEXPAUtagRECT@@@Z	; CWnd::GetWindowRect

; 530  : 	return rToolbar.Height();

	lea	ecx, DWORD PTR _rToolbar$[ebp]
	call	?Height@CRect@@QBEHXZ			; CRect::Height

; 531  : }

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetHeight@CEnToolBar@@QBEHXZ ENDP			; CEnToolBar::GetHeight
_TEXT	ENDS
EXTRN	?OnDestroy@CControlBar@@QAEXXZ:NEAR		; CControlBar::OnDestroy
EXTRN	?AfxDeleteObject@@YGXPAPAX@Z:NEAR		; AfxDeleteObject
;	COMDAT ?OnDestroy@CEnToolBar@@IAEXXZ
_TEXT	SEGMENT
_this$ = -4
?OnDestroy@CEnToolBar@@IAEXXZ PROC NEAR			; CEnToolBar::OnDestroy, COMDAT

; 534  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 535  : 	CToolBar::OnDestroy();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnDestroy@CControlBar@@QAEXXZ		; CControlBar::OnDestroy

; 536  : 
; 537  : 	AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 132				; 00000084H
	push	eax
	call	?AfxDeleteObject@@YGXPAPAX@Z		; AfxDeleteObject

; 538  : 
; 539  : 	m_ilNormal.DeleteImageList();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?DeleteImageList@CImageList@@QAEHXZ	; CImageList::DeleteImageList

; 540  : 	m_ilDisabled.DeleteImageList();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 160				; 000000a0H
	call	?DeleteImageList@CImageList@@QAEHXZ	; CImageList::DeleteImageList

; 541  : 
; 542  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?OnDestroy@CEnToolBar@@IAEXXZ ENDP			; CEnToolBar::OnDestroy
_TEXT	ENDS
END

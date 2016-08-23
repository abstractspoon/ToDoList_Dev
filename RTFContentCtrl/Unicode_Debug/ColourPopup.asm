	TITLE	D:\_CODE\3rdParty\ColourPopup.cpp
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
;	COMDAT ??_C@_0CH@EADC@D?3?2_CODE?23rdParty?2?4?4?2shared?2Subc@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0EK@MDCF@C?3?2Program?5Files?5?$CIx86?$CJ?2Microsoft@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetCWnd@CSubclassWnd@@QBEPAVCWnd@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetExStyle@CSubclassWnd@@QBEKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetStyle@CSubclassWnd@@QBEKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Invalidate@CSubclassWnd@@QBEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CSubclasser@@IAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScWindowProc@CSubclasser@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTopSubclasser@CSubclasser@@MAEPAV1@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScPreDetachWindow@CSubclasser@@MAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScPostDetachWindow@CSubclasser@@MAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScHookWindow@CSubclasser@@IAEHPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetColour@CColourPopup@@IAEKH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetColourName@CColourPopup@@IAEPBGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CColourPopup@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCColourPopup@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CSubclasser@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CColourPopup@@QAE@VCPoint@@KPAVCWnd@@IPBG2H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Initialise@CColourPopup@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CColourPopup@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Create@CColourPopup@@QAEHVCPoint@@KPAVCWnd@@IPBG2H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_GetBaseMessageMap@CColourPopup@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMessageMap@CColourPopup@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PreTranslateMessage@CColourPopup@@UAEHPAUtagMSG@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ScWindowProc@CColourPopup@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnNcDestroy@CColourPopup@@IAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnPaint@CColourPopup@@IAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnMouseMove@CColourPopup@@IAEXIVCPoint@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnLButtonUp@CColourPopup@@IAEXIVCPoint@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetIndex@CColourPopup@@IBEHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetRow@CColourPopup@@IBEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetColumn@CColourPopup@@IBEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FindCellFromColour@CColourPopup@@IAEXK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetCellRect@CColourPopup@@IAEHHABQAUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetWindowSize@CColourPopup@@IAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateToolTips@CColourPopup@@IAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ChangeSelection@CColourPopup@@IAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EndSelection@CColourPopup@@IAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CColorDialog@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CCommonDialog@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawCell@CColourPopup@@IAEXPAVCDC@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnQueryNewPalette@CColourPopup@@IAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnPaletteChanged@CColourPopup@@IAEXPAVCWnd@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnKillFocus@CColourPopup@@IAEXPAVCWnd@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnActivateApp@CColourPopup@@IAEXHPAUHTASK__@@@Z
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
;	COMDAT ??_7CColourPopup@@6BCSubclasser@@@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CColourPopup@@6BCWnd@@@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CSubclasser@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?OnPaletteChanged@CColourPopup@@IAEXPAVCWnd@@@Z	; CColourPopup::OnPaletteChanged
PUBLIC	?OnKillFocus@CColourPopup@@IAEXPAVCWnd@@@Z	; CColourPopup::OnKillFocus
PUBLIC	?OnActivateApp@CColourPopup@@IAEXHPAUHTASK__@@@Z ; CColourPopup::OnActivateApp
PUBLIC	?_messageEntries@CColourPopup@@0QBUAFX_MSGMAP_ENTRY@@B ; CColourPopup::_messageEntries
PUBLIC	?messageMap@CColourPopup@@1UAFX_MSGMAP@@B	; CColourPopup::messageMap
PUBLIC	?_GetBaseMessageMap@CColourPopup@@KGPBUAFX_MSGMAP@@XZ ; CColourPopup::_GetBaseMessageMap
PUBLIC	?m_crColours@CColourPopup@@1PAUColourTableEntry@@A ; CColourPopup::m_crColours
PUBLIC	?OnNcDestroy@CColourPopup@@IAEXXZ		; CColourPopup::OnNcDestroy
PUBLIC	?OnLButtonUp@CColourPopup@@IAEXIVCPoint@@@Z	; CColourPopup::OnLButtonUp
PUBLIC	?OnPaint@CColourPopup@@IAEXXZ			; CColourPopup::OnPaint
PUBLIC	?OnMouseMove@CColourPopup@@IAEXIVCPoint@@@Z	; CColourPopup::OnMouseMove
PUBLIC	?OnQueryNewPalette@CColourPopup@@IAEHXZ		; CColourPopup::OnQueryNewPalette
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\3rdParty\ColourPopup.cpp', 00H
	ORG $+6
?m_crColours@CColourPopup@@1PAUColourTableEntry@@A DD 00H ; CColourPopup::m_crColours
	DD	FLAT:$SG73787
	DD	02aa5H
	DD	FLAT:$SG73794
	DD	0404000H
	DD	FLAT:$SG73801
	DD	05500H
	DD	FLAT:$SG73808
	DD	05e0000H
	DD	FLAT:$SG73815
	DD	08b0000H
	DD	FLAT:$SG73822
	DD	082004bH
	DD	FLAT:$SG73829
	DD	0282828H
	DD	FLAT:$SG73836
	DD	08bH
	DD	FLAT:$SG73843
	DD	02068ffH
	DD	FLAT:$SG73850
	DD	08b8bH
	DD	FLAT:$SG73857
	DD	09300H
	DD	FLAT:$SG73864
	DD	08e8e38H
	DD	FLAT:$SG73871
	DD	0ff0000H
	DD	FLAT:$SG73878
	DD	0c07b7bH
	DD	FLAT:$SG73885
	DD	0666666H
	DD	FLAT:$SG73892
	DD	0ffH
	DD	FLAT:$SG73899
	DD	05badffH
	DD	FLAT:$SG73906
	DD	032cd32H
	DD	FLAT:$SG73913
	DD	071b33cH
	DD	FLAT:$SG73920
	DD	0d4ff7fH
	DD	FLAT:$SG73927
	DD	0c09e7dH
	DD	FLAT:$SG73934
	DD	0800080H
	DD	FLAT:$SG73941
	DD	07f7f7fH
	DD	FLAT:$SG73948
	DD	0cbc0ffH
	DD	FLAT:$SG73955
	DD	0d7ffH
	DD	FLAT:$SG73962
	DD	0ffffH
	DD	FLAT:$SG73969
	DD	0ff00H
	DD	FLAT:$SG73976
	DD	0d0e040H
	DD	FLAT:$SG73983
	DD	0ffffc0H
	DD	FLAT:$SG73990
	DD	0480048H
	DD	FLAT:$SG73997
	DD	0c0c0c0H
	DD	FLAT:$SG74004
	DD	0e1e4ffH
	DD	FLAT:$SG74011
	DD	08cb4d2H
	DD	FLAT:$SG74018
	DD	0e0ffffH
	DD	FLAT:$SG74025
	DD	098fb98H
	DD	FLAT:$SG74032
	DD	0eeeeafH
	DD	FLAT:$SG74039
	DD	08b8368H
	DD	FLAT:$SG74046
	DD	0fae6e6H
	DD	FLAT:$SG74053
	DD	0ffffffH
	DD	FLAT:$SG74060
_DATA	ENDS
CONST	SEGMENT
?messageMap@CColourPopup@@1UAFX_MSGMAP@@B DD FLAT:?_GetBaseMessageMap@CColourPopup@@KGPBUAFX_MSGMAP@@XZ ; CColourPopup::messageMap
	DD	FLAT:?_messageEntries@CColourPopup@@0QBUAFX_MSGMAP_ENTRY@@B
?_messageEntries@CColourPopup@@0QBUAFX_MSGMAP_ENTRY@@B DD 082H ; CColourPopup::_messageEntries
	DD	00H
	DD	00H
	DD	00H
	DD	0cH
	DD	FLAT:?OnNcDestroy@CColourPopup@@IAEXXZ
	DD	0202H
	DD	00H
	DD	00H
	DD	00H
	DD	031H
	DD	FLAT:?OnLButtonUp@CColourPopup@@IAEXIVCPoint@@@Z
	DD	0fH
	DD	00H
	DD	00H
	DD	00H
	DD	0cH
	DD	FLAT:?OnPaint@CColourPopup@@IAEXXZ
	DD	0200H
	DD	00H
	DD	00H
	DD	00H
	DD	031H
	DD	FLAT:?OnMouseMove@CColourPopup@@IAEXIVCPoint@@@Z
	DD	030fH
	DD	00H
	DD	00H
	DD	00H
	DD	023H
	DD	FLAT:?OnQueryNewPalette@CColourPopup@@IAEHXZ
	DD	0311H
	DD	00H
	DD	00H
	DD	00H
	DD	017H
	DD	FLAT:?OnPaletteChanged@CColourPopup@@IAEXPAVCWnd@@@Z
	DD	08H
	DD	00H
	DD	00H
	DD	00H
	DD	017H
	DD	FLAT:?OnKillFocus@CColourPopup@@IAEXPAVCWnd@@@Z
	DD	01cH
	DD	00H
	DD	00H
	DD	00H
	DD	0eH
	DD	FLAT:?OnActivateApp@CColourPopup@@IAEXHPAUHTASK__@@@Z
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
CONST	ENDS
_DATA	SEGMENT
$SG73787 DB	'B', 00H, 'l', 00H, 'a', 00H, 'c', 00H, 'k', 00H, 00H, 00H
$SG73794 DB	'B', 00H, 'r', 00H, 'o', 00H, 'w', 00H, 'n', 00H, 00H, 00H
$SG73801 DB	'D', 00H, 'a', 00H, 'r', 00H, 'k', 00H, ' ', 00H, 'O', 00H
	DB	'l', 00H, 'i', 00H, 'v', 00H, 'e', 00H, ' ', 00H, 'G', 00H, 'r'
	DB	00H, 'e', 00H, 'e', 00H, 'n', 00H, 00H, 00H
	ORG $+2
$SG73808 DB	'D', 00H, 'a', 00H, 'r', 00H, 'k', 00H, ' ', 00H, 'G', 00H
	DB	'r', 00H, 'e', 00H, 'e', 00H, 'n', 00H, 00H, 00H
	ORG $+2
$SG73815 DB	'D', 00H, 'a', 00H, 'r', 00H, 'k', 00H, ' ', 00H, 'T', 00H
	DB	'e', 00H, 'a', 00H, 'l', 00H, 00H, 00H
$SG73822 DB	'D', 00H, 'a', 00H, 'r', 00H, 'k', 00H, ' ', 00H, 'b', 00H
	DB	'l', 00H, 'u', 00H, 'e', 00H, 00H, 00H
$SG73829 DB	'I', 00H, 'n', 00H, 'd', 00H, 'i', 00H, 'g', 00H, 'o', 00H
	DB	00H, 00H
	ORG $+2
$SG73836 DB	'D', 00H, 'a', 00H, 'r', 00H, 'k', 00H, ' ', 00H, 'g', 00H
	DB	'r', 00H, 'e', 00H, 'y', 00H, 00H, 00H
$SG73843 DB	'D', 00H, 'a', 00H, 'r', 00H, 'k', 00H, ' ', 00H, 'r', 00H
	DB	'e', 00H, 'd', 00H, 00H, 00H
	ORG $+2
$SG73850 DB	'O', 00H, 'r', 00H, 'a', 00H, 'n', 00H, 'g', 00H, 'e', 00H
	DB	00H, 00H
	ORG $+2
$SG73857 DB	'D', 00H, 'a', 00H, 'r', 00H, 'k', 00H, ' ', 00H, 'y', 00H
	DB	'e', 00H, 'l', 00H, 'l', 00H, 'o', 00H, 'w', 00H, 00H, 00H
$SG73864 DB	'G', 00H, 'r', 00H, 'e', 00H, 'e', 00H, 'n', 00H, 00H, 00H
$SG73871 DB	'T', 00H, 'e', 00H, 'a', 00H, 'l', 00H, 00H, 00H
	ORG $+2
$SG73878 DB	'B', 00H, 'l', 00H, 'u', 00H, 'e', 00H, 00H, 00H
	ORG $+2
$SG73885 DB	'B', 00H, 'l', 00H, 'u', 00H, 'e', 00H, '-', 00H, 'g', 00H
	DB	'r', 00H, 'e', 00H, 'y', 00H, 00H, 00H
$SG73892 DB	'G', 00H, 'r', 00H, 'e', 00H, 'y', 00H, ' ', 00H, '-', 00H
	DB	' ', 00H, '4', 00H, '0', 00H, 00H, 00H
$SG73899 DB	'R', 00H, 'e', 00H, 'd', 00H, 00H, 00H
$SG73906 DB	'L', 00H, 'i', 00H, 'g', 00H, 'h', 00H, 't', 00H, ' ', 00H
	DB	'o', 00H, 'r', 00H, 'a', 00H, 'n', 00H, 'g', 00H, 'e', 00H, 00H
	DB	00H
	ORG $+2
$SG73913 DB	'L', 00H, 'i', 00H, 'm', 00H, 'e', 00H, 00H, 00H
	ORG $+2
$SG73920 DB	'S', 00H, 'e', 00H, 'a', 00H, ' ', 00H, 'g', 00H, 'r', 00H
	DB	'e', 00H, 'e', 00H, 'n', 00H, 00H, 00H
$SG73927 DB	'A', 00H, 'q', 00H, 'u', 00H, 'a', 00H, 00H, 00H
	ORG $+2
$SG73934 DB	'L', 00H, 'i', 00H, 'g', 00H, 'h', 00H, 't', 00H, ' ', 00H
	DB	'b', 00H, 'l', 00H, 'u', 00H, 'e', 00H, 00H, 00H
	ORG $+2
$SG73941 DB	'V', 00H, 'i', 00H, 'o', 00H, 'l', 00H, 'e', 00H, 't', 00H
	DB	00H, 00H
	ORG $+2
$SG73948 DB	'G', 00H, 'r', 00H, 'e', 00H, 'y', 00H, ' ', 00H, '-', 00H
	DB	' ', 00H, '5', 00H, '0', 00H, 00H, 00H
$SG73955 DB	'P', 00H, 'i', 00H, 'n', 00H, 'k', 00H, 00H, 00H
	ORG $+2
$SG73962 DB	'G', 00H, 'o', 00H, 'l', 00H, 'd', 00H, 00H, 00H
	ORG $+2
$SG73969 DB	'Y', 00H, 'e', 00H, 'l', 00H, 'l', 00H, 'o', 00H, 'w', 00H
	DB	00H, 00H
	ORG $+2
$SG73976 DB	'B', 00H, 'r', 00H, 'i', 00H, 'g', 00H, 'h', 00H, 't', 00H
	DB	' ', 00H, 'g', 00H, 'r', 00H, 'e', 00H, 'e', 00H, 'n', 00H, 00H
	DB	00H
	ORG $+2
$SG73983 DB	'T', 00H, 'u', 00H, 'r', 00H, 'q', 00H, 'u', 00H, 'o', 00H
	DB	'i', 00H, 's', 00H, 'e', 00H, 00H, 00H
$SG73990 DB	'S', 00H, 'k', 00H, 'y', 00H, 'b', 00H, 'l', 00H, 'u', 00H
	DB	'e', 00H, 00H, 00H
$SG73997 DB	'P', 00H, 'l', 00H, 'u', 00H, 'm', 00H, 00H, 00H
	ORG $+2
$SG74004 DB	'L', 00H, 'i', 00H, 'g', 00H, 'h', 00H, 't', 00H, ' ', 00H
	DB	'g', 00H, 'r', 00H, 'e', 00H, 'y', 00H, 00H, 00H
	ORG $+2
$SG74011 DB	'R', 00H, 'o', 00H, 's', 00H, 'e', 00H, 00H, 00H
	ORG $+2
$SG74018 DB	'T', 00H, 'a', 00H, 'n', 00H, 00H, 00H
$SG74025 DB	'L', 00H, 'i', 00H, 'g', 00H, 'h', 00H, 't', 00H, ' ', 00H
	DB	'y', 00H, 'e', 00H, 'l', 00H, 'l', 00H, 'o', 00H, 'w', 00H, 00H
	DB	00H
	ORG $+2
$SG74032 DB	'P', 00H, 'a', 00H, 'l', 00H, 'e', 00H, ' ', 00H, 'g', 00H
	DB	'r', 00H, 'e', 00H, 'e', 00H, 'n', 00H, ' ', 00H, 00H, 00H
$SG74039 DB	'P', 00H, 'a', 00H, 'l', 00H, 'e', 00H, ' ', 00H, 't', 00H
	DB	'u', 00H, 'r', 00H, 'q', 00H, 'u', 00H, 'o', 00H, 'i', 00H, 's'
	DB	00H, 'e', 00H, 00H, 00H
	ORG $+2
$SG74046 DB	'P', 00H, 'a', 00H, 'l', 00H, 'e', 00H, ' ', 00H, 'b', 00H
	DB	'l', 00H, 'u', 00H, 'e', 00H, 00H, 00H
$SG74053 DB	'L', 00H, 'a', 00H, 'v', 00H, 'e', 00H, 'n', 00H, 'd', 00H
	DB	'e', 00H, 'r', 00H, 00H, 00H
	ORG $+2
$SG74060 DB	'W', 00H, 'h', 00H, 'i', 00H, 't', 00H, 'e', 00H, 00H, 00H
_DATA	ENDS
PUBLIC	?GetMessageMap@CColourPopup@@MBEPBUAFX_MSGMAP@@XZ ; CColourPopup::GetMessageMap
PUBLIC	?ScWindowProc@CColourPopup@@MAEJPAUHWND__@@IIJ@Z ; CColourPopup::ScWindowProc
PUBLIC	??_7CColourPopup@@6BCWnd@@@			; CColourPopup::`vftable'
PUBLIC	??_7CColourPopup@@6BCSubclasser@@@		; CColourPopup::`vftable'
PUBLIC	??_GCColourPopup@@UAEPAXI@Z			; CColourPopup::`scalar deleting destructor'
PUBLIC	??_ECColourPopup@@UAEPAXI@Z			; CColourPopup::`vector deleting destructor'
PUBLIC	??0CSubclasser@@IAE@XZ				; CSubclasser::CSubclasser
PUBLIC	?GetTopSubclasser@CSubclasser@@MAEPAV1@XZ	; CSubclasser::GetTopSubclasser
PUBLIC	?ScPreDetachWindow@CSubclasser@@MAEXXZ		; CSubclasser::ScPreDetachWindow
PUBLIC	?ScPostDetachWindow@CSubclasser@@MAEXXZ		; CSubclasser::ScPostDetachWindow
PUBLIC	??1CSubclasser@@QAE@XZ				; CSubclasser::~CSubclasser
PUBLIC	??0CColourPopup@@QAE@XZ				; CColourPopup::CColourPopup
PUBLIC	?Initialise@CColourPopup@@QAEXXZ		; CColourPopup::Initialise
PUBLIC	?PreTranslateMessage@CColourPopup@@UAEHPAUtagMSG@@@Z ; CColourPopup::PreTranslateMessage
EXTRN	??0CWnd@@QAE@XZ:NEAR				; CWnd::CWnd
EXTRN	?PreSubclassWindow@CWnd@@UAEXXZ:NEAR		; CWnd::PreSubclassWindow
EXTRN	?Create@CWnd@@UAEHPBG0KABUtagRECT@@PAV1@IPAUCCreateContext@@@Z:NEAR ; CWnd::Create
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	?DestroyWindow@CWnd@@UAEHXZ:NEAR		; CWnd::DestroyWindow
EXTRN	?PreCreateWindow@CWnd@@UAEHAAUtagCREATESTRUCTW@@@Z:NEAR ; CWnd::PreCreateWindow
EXTRN	?CalcWindowRect@CWnd@@UAEXPAUtagRECT@@I@Z:NEAR	; CWnd::CalcWindowRect
EXTRN	??0CFont@@QAE@XZ:NEAR				; CFont::CFont
EXTRN	??1CFont@@UAE@XZ:NEAR				; CFont::~CFont
EXTRN	??0CPalette@@QAE@XZ:NEAR			; CPalette::CPalette
EXTRN	??1CPalette@@UAE@XZ:NEAR			; CPalette::~CPalette
EXTRN	?OnToolHitTest@CWnd@@UBEHVCPoint@@PAUtagTOOLINFOW@@@Z:NEAR ; CWnd::OnToolHitTest
EXTRN	?GetScrollBarCtrl@CWnd@@UBEPAVCScrollBar@@H@Z:NEAR ; CWnd::GetScrollBarCtrl
EXTRN	?WinHelpW@CWnd@@UAEXKI@Z:NEAR			; CWnd::WinHelpW
EXTRN	?ContinueModal@CWnd@@UAEHXZ:NEAR		; CWnd::ContinueModal
EXTRN	?EndModalLoop@CWnd@@UAEXH@Z:NEAR		; CWnd::EndModalLoop
EXTRN	??0CToolTipCtrl@@QAE@XZ:NEAR			; CToolTipCtrl::CToolTipCtrl
EXTRN	?OnCommand@CWnd@@MAEHIJ@Z:NEAR			; CWnd::OnCommand
EXTRN	?OnNotify@CWnd@@MAEHIJPAJ@Z:NEAR		; CWnd::OnNotify
EXTRN	??1CToolTipCtrl@@UAE@XZ:NEAR			; CToolTipCtrl::~CToolTipCtrl
EXTRN	?OnCmdMsg@CCmdTarget@@UAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z:NEAR ; CCmdTarget::OnCmdMsg
EXTRN	?IsInvokeAllowed@CCmdTarget@@UAEHJ@Z:NEAR	; CCmdTarget::IsInvokeAllowed
EXTRN	?GetDispatchIID@CCmdTarget@@UAEHPAU_GUID@@@Z:NEAR ; CCmdTarget::GetDispatchIID
EXTRN	?GetTypeInfoCount@CCmdTarget@@UAEIXZ:NEAR	; CCmdTarget::GetTypeInfoCount
EXTRN	?GetTypeLibCache@CCmdTarget@@UAEPAVCTypeLibCache@@XZ:NEAR ; CCmdTarget::GetTypeLibCache
EXTRN	?GetTypeLib@CCmdTarget@@UAEJKPAPAUITypeLib@@@Z:NEAR ; CCmdTarget::GetTypeLib
EXTRN	??0CRect@@QAE@XZ:NEAR				; CRect::CRect
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
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
EXTRN	?OnAmbientProperty@CWnd@@UAEHPAVCOleControlSite@@JPAUtagVARIANT@@@Z:NEAR ; CWnd::OnAmbientProperty
EXTRN	?WindowProc@CWnd@@MAEJIIJ@Z:NEAR		; CWnd::WindowProc
EXTRN	?OnWndMsg@CWnd@@MAEHIIJPAJ@Z:NEAR		; CWnd::OnWndMsg
EXTRN	?DefWindowProcW@CWnd@@MAEJIIJ@Z:NEAR		; CWnd::DefWindowProcW
EXTRN	?PostNcDestroy@CWnd@@MAEXXZ:NEAR		; CWnd::PostNcDestroy
EXTRN	?OnChildNotify@CWnd@@MAEHIIJPAJ@Z:NEAR		; CWnd::OnChildNotify
EXTRN	??1CWnd@@UAE@XZ:NEAR				; CWnd::~CWnd
EXTRN	?CheckAutoCenter@CWnd@@UAEHXZ:NEAR		; CWnd::CheckAutoCenter
EXTRN	?AssertValid@CWnd@@UBEXXZ:NEAR			; CWnd::AssertValid
EXTRN	?Dump@CWnd@@UBEXAAVCDumpContext@@@Z:NEAR	; CWnd::Dump
EXTRN	?GetExtraConnectionPoints@CCmdTarget@@MAEHPAVCPtrArray@@@Z:NEAR ; CCmdTarget::GetExtraConnectionPoints
EXTRN	?GetConnectionHook@CCmdTarget@@MAEPAUIConnectionPoint@@ABU_GUID@@@Z:NEAR ; CCmdTarget::GetConnectionHook
EXTRN	?IsFrameWnd@CWnd@@UBEHXZ:NEAR			; CWnd::IsFrameWnd
EXTRN	?OnFinalRelease@CWnd@@UAEXXZ:NEAR		; CWnd::OnFinalRelease
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
EXTRN	?SetOccDialogInfo@CWnd@@MAEHPAU_AFX_OCC_DIALOG_INFO@@@Z:NEAR ; CWnd::SetOccDialogInfo
EXTRN	__except_list:DWORD
EXTRN	__chkesp:NEAR
EXTRN	___CxxFrameHandler:NEAR
EXTRN	?GetRuntimeClass@CWnd@@UBEPAUCRuntimeClass@@XZ:NEAR ; CWnd::GetRuntimeClass
;	COMDAT ??_7CColourPopup@@6BCWnd@@@
; File D:\_CODE\3rdParty\ColourPopup.cpp
CONST	SEGMENT
??_7CColourPopup@@6BCWnd@@@ DD FLAT:?GetRuntimeClass@CWnd@@UBEPAUCRuntimeClass@@XZ ; CColourPopup::`vftable'
	DD	FLAT:??_ECColourPopup@@UAEPAXI@Z
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
	DD	FLAT:?GetMessageMap@CColourPopup@@MBEPBUAFX_MSGMAP@@XZ
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
	DD	FLAT:?PreTranslateMessage@CColourPopup@@UAEHPAUtagMSG@@@Z
	DD	FLAT:?OnAmbientProperty@CWnd@@UAEHPAVCOleControlSite@@JPAUtagVARIANT@@@Z
	DD	FLAT:?WindowProc@CWnd@@MAEJIIJ@Z
	DD	FLAT:?OnWndMsg@CWnd@@MAEHIIJPAJ@Z
	DD	FLAT:?DefWindowProcW@CWnd@@MAEJIIJ@Z
	DD	FLAT:?PostNcDestroy@CWnd@@MAEXXZ
	DD	FLAT:?OnChildNotify@CWnd@@MAEHIIJPAJ@Z
	DD	FLAT:?CheckAutoCenter@CWnd@@UAEHXZ
	DD	FLAT:?IsFrameWnd@CWnd@@UBEHXZ
	DD	FLAT:?SetOccDialogInfo@CWnd@@MAEHPAU_AFX_OCC_DIALOG_INFO@@@Z
CONST	ENDS
;	COMDAT ??_7CColourPopup@@6BCSubclasser@@@
CONST	SEGMENT
??_7CColourPopup@@6BCSubclasser@@@ DD FLAT:?ScWindowProc@CColourPopup@@MAEJPAUHWND__@@IIJ@Z ; CColourPopup::`vftable'
	DD	FLAT:?GetTopSubclasser@CSubclasser@@MAEPAV1@XZ
	DD	FLAT:?ScPreDetachWindow@CSubclasser@@MAEXXZ
	DD	FLAT:?ScPostDetachWindow@CSubclasser@@MAEXXZ
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74723	DD	019930520H
	DD	07H
	DD	FLAT:$T74727
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74727	DD	0ffffffffH
	DD	FLAT:$L74713
	DD	00H
	DD	FLAT:$L74714
	DD	01H
	DD	FLAT:$L74715
	DD	02H
	DD	FLAT:$L74716
	DD	03H
	DD	FLAT:$L74717
	DD	04H
	DD	FLAT:$L74718
	DD	05H
	DD	FLAT:$L74719
xdata$x	ENDS
;	COMDAT ??0CColourPopup@@QAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??0CColourPopup@@QAE@XZ PROC NEAR			; CColourPopup::CColourPopup, COMDAT

; 112  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74724
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CWnd@@QAE@XZ				; CWnd::CWnd
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	??0CSubclasser@@IAE@XZ			; CSubclasser::CSubclasser
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 136				; 00000088H
	call	??0CRect@@QAE@XZ			; CRect::CRect
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 152				; 00000098H
	call	??0CRect@@QAE@XZ			; CRect::CRect
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	??0CRect@@QAE@XZ			; CRect::CRect
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 184				; 000000b8H
	call	??0CFont@@QAE@XZ			; CFont::CFont
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	??0CPalette@@QAE@XZ			; CPalette::CPalette
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 208				; 000000d0H
	call	??0CToolTipCtrl@@QAE@XZ			; CToolTipCtrl::CToolTipCtrl
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CColourPopup@@6BCWnd@@@ ; CColourPopup::`vftable'
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+64], OFFSET FLAT:??_7CColourPopup@@6BCSubclasser@@@ ; CColourPopup::`vftable'

; 113  :     Initialise();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Initialise@CColourPopup@@QAEXXZ	; CColourPopup::Initialise

; 114  : }

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
$L74713:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CWnd@@UAE@XZ				; CWnd::~CWnd
	ret	0
$L74714:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	??1CSubclasser@@QAE@XZ			; CSubclasser::~CSubclasser
	ret	0
$L74715:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74716:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74717:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 184				; 000000b8H
	call	??1CFont@@UAE@XZ			; CFont::~CFont
	ret	0
$L74718:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	??1CPalette@@UAE@XZ			; CPalette::~CPalette
	ret	0
$L74719:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 208				; 000000d0H
	call	??1CToolTipCtrl@@UAE@XZ			; CToolTipCtrl::~CToolTipCtrl
	ret	0
$L74724:
	mov	eax, OFFSET FLAT:$T74723
	jmp	___CxxFrameHandler
text$x	ENDS
??0CColourPopup@@QAE@XZ ENDP				; CColourPopup::CColourPopup
PUBLIC	?ScWindowProc@CSubclasser@@MAEJPAUHWND__@@IIJ@Z	; CSubclasser::ScWindowProc
PUBLIC	??_7CSubclasser@@6B@				; CSubclasser::`vftable'
EXTRN	??0CSubclassWnd@@QAE@XZ:NEAR			; CSubclassWnd::CSubclassWnd
;	COMDAT ??_7CSubclasser@@6B@
; File D:\_CODE\3rdParty\..\shared\Subclass.h
CONST	SEGMENT
??_7CSubclasser@@6B@ DD FLAT:?ScWindowProc@CSubclasser@@MAEJPAUHWND__@@IIJ@Z ; CSubclasser::`vftable'
	DD	FLAT:?GetTopSubclasser@CSubclasser@@MAEPAV1@XZ
	DD	FLAT:?ScPreDetachWindow@CSubclasser@@MAEXXZ
	DD	FLAT:?ScPostDetachWindow@CSubclasser@@MAEXXZ
CONST	ENDS
;	COMDAT ??0CSubclasser@@IAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0CSubclasser@@IAE@XZ PROC NEAR			; CSubclasser::CSubclasser, COMDAT

; 134  : 	CSubclasser() {}

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??0CSubclassWnd@@QAE@XZ			; CSubclassWnd::CSubclassWnd
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CSubclasser@@6B@ ; CSubclasser::`vftable'
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CSubclasser@@IAE@XZ ENDP				; CSubclasser::CSubclasser
_TEXT	ENDS
PUBLIC	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ		; CSubclassWnd::GetHwnd
PUBLIC	??_C@_0CH@EADC@D?3?2_CODE?23rdParty?2?4?4?2shared?2Subc@ ; `string'
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
;	COMDAT ??_C@_0CH@EADC@D?3?2_CODE?23rdParty?2?4?4?2shared?2Subc@
; File D:\_CODE\3rdParty\..\shared\Subclass.h
_DATA	SEGMENT
??_C@_0CH@EADC@D?3?2_CODE?23rdParty?2?4?4?2shared?2Subc@ DB 'D:\_CODE\3rd'
	DB	'Party\..\shared\Subclass.h', 00H		; `string'
_DATA	ENDS
;	COMDAT ?ScWindowProc@CSubclasser@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT
_hRealWnd$ = 8
_msg$ = 12
_wp$ = 16
_lp$ = 20
_this$ = -4
?ScWindowProc@CSubclasser@@MAEJPAUHWND__@@IIJ@Z PROC NEAR ; CSubclasser::ScWindowProc, COMDAT

; 137  : 	{

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L73607:

; 138  : 		ASSERT(hRealWnd == m_subclass.GetHwnd()); 

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ	; CSubclassWnd::GetHwnd
	cmp	DWORD PTR _hRealWnd$[ebp], eax
	je	SHORT $L73610
	push	138					; 0000008aH
	push	OFFSET FLAT:??_C@_0CH@EADC@D?3?2_CODE?23rdParty?2?4?4?2shared?2Subc@ ; `string'
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73610
	int	3
$L73610:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73607

; 139  : 		return m_subclass.WindowProc(hRealWnd, msg, wp, lp); 

	mov	esi, esp
	mov	ecx, DWORD PTR _lp$[ebp]
	push	ecx
	mov	edx, DWORD PTR _wp$[ebp]
	push	edx
	mov	eax, DWORD PTR _msg$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hRealWnd$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	call	DWORD PTR [eax+40]
	cmp	esi, esp
	call	__chkesp

; 140  : 	}

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?ScWindowProc@CSubclasser@@MAEJPAUHWND__@@IIJ@Z ENDP	; CSubclasser::ScWindowProc
_TEXT	ENDS
;	COMDAT ?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ PROC NEAR	; CSubclassWnd::GetHwnd, COMDAT

; 58   : 	inline HWND GetHwnd() const { return m_hWndHooked; }

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
?GetHwnd@CSubclassWnd@@QBEPAUHWND__@@XZ ENDP		; CSubclassWnd::GetHwnd
_TEXT	ENDS
;	COMDAT ?GetTopSubclasser@CSubclasser@@MAEPAV1@XZ
_TEXT	SEGMENT
_this$ = -4
?GetTopSubclasser@CSubclasser@@MAEPAV1@XZ PROC NEAR	; CSubclasser::GetTopSubclasser, COMDAT

; 142  : 	virtual CSubclasser* GetTopSubclasser() { return this; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	0
?GetTopSubclasser@CSubclasser@@MAEPAV1@XZ ENDP		; CSubclasser::GetTopSubclasser
_TEXT	ENDS
;	COMDAT ?ScPreDetachWindow@CSubclasser@@MAEXXZ
_TEXT	SEGMENT
_this$ = -4
?ScPreDetachWindow@CSubclasser@@MAEXXZ PROC NEAR	; CSubclasser::ScPreDetachWindow, COMDAT

; 146  : 	virtual void ScPreDetachWindow() { }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esp, ebp
	pop	ebp
	ret	0
?ScPreDetachWindow@CSubclasser@@MAEXXZ ENDP		; CSubclasser::ScPreDetachWindow
_TEXT	ENDS
;	COMDAT ?ScPostDetachWindow@CSubclasser@@MAEXXZ
_TEXT	SEGMENT
_this$ = -4
?ScPostDetachWindow@CSubclasser@@MAEXXZ PROC NEAR	; CSubclasser::ScPostDetachWindow, COMDAT

; 147  : 	virtual void ScPostDetachWindow() { }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	esp, ebp
	pop	ebp
	ret	0
?ScPostDetachWindow@CSubclasser@@MAEXXZ ENDP		; CSubclasser::ScPostDetachWindow
_TEXT	ENDS
PUBLIC	??1CColourPopup@@UAE@XZ				; CColourPopup::~CColourPopup
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_GCColourPopup@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCColourPopup@@UAEPAXI@Z PROC NEAR			; CColourPopup::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CColourPopup@@UAE@XZ			; CColourPopup::~CColourPopup
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L74067
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L74067:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCColourPopup@@UAEPAXI@Z ENDP			; CColourPopup::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	??1CSubclassWnd@@UAE@XZ:NEAR			; CSubclassWnd::~CSubclassWnd
;	COMDAT ??1CSubclasser@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CSubclasser@@QAE@XZ PROC NEAR			; CSubclasser::~CSubclasser, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	call	??1CSubclassWnd@@UAE@XZ			; CSubclassWnd::~CSubclassWnd
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CSubclasser@@QAE@XZ ENDP				; CSubclasser::~CSubclasser
_TEXT	ENDS
PUBLIC	??0CColourPopup@@QAE@VCPoint@@KPAVCWnd@@IPBG2H@Z ; CColourPopup::CColourPopup
PUBLIC	?Create@CColourPopup@@QAEHVCPoint@@KPAVCWnd@@IPBG2H@Z ; CColourPopup::Create
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\ColourPopup.cpp
xdata$x	SEGMENT
$T74759	DD	019930520H
	DD	07H
	DD	FLAT:$T74761
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74761	DD	0ffffffffH
	DD	FLAT:$L74751
	DD	00H
	DD	FLAT:$L74752
	DD	01H
	DD	FLAT:$L74753
	DD	02H
	DD	FLAT:$L74754
	DD	03H
	DD	FLAT:$L74755
	DD	04H
	DD	FLAT:$L74756
	DD	05H
	DD	FLAT:$L74757
xdata$x	ENDS
;	COMDAT ??0CColourPopup@@QAE@VCPoint@@KPAVCWnd@@IPBG2H@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_p$ = 8
_crColour$ = 16
_pParentWnd$ = 20
_nID$ = 24
_szDefaultText$ = 28
_szCustomText$ = 32
_bIgnoreFirstLBtnUp$ = 36
_this$ = -16
??0CColourPopup@@QAE@VCPoint@@KPAVCWnd@@IPBG2H@Z PROC NEAR ; CColourPopup::CColourPopup, COMDAT

; 121  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74760
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CWnd@@QAE@XZ				; CWnd::CWnd
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	??0CSubclasser@@IAE@XZ			; CSubclasser::CSubclasser
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 136				; 00000088H
	call	??0CRect@@QAE@XZ			; CRect::CRect
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 152				; 00000098H
	call	??0CRect@@QAE@XZ			; CRect::CRect
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	??0CRect@@QAE@XZ			; CRect::CRect
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 184				; 000000b8H
	call	??0CFont@@QAE@XZ			; CFont::CFont
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	??0CPalette@@QAE@XZ			; CPalette::CPalette
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 208				; 000000d0H
	call	??0CToolTipCtrl@@QAE@XZ			; CToolTipCtrl::CToolTipCtrl
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CColourPopup@@6BCWnd@@@ ; CColourPopup::`vftable'
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+64], OFFSET FLAT:??_7CColourPopup@@6BCSubclasser@@@ ; CColourPopup::`vftable'

; 122  :     Initialise();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Initialise@CColourPopup@@QAEXXZ	; CColourPopup::Initialise

; 123  : 
; 124  :     CColourPopup::Create(p, crColour, pParentWnd, nID, szDefaultText, szCustomText, bIgnoreFirstLBtnUp);

	mov	edx, DWORD PTR _bIgnoreFirstLBtnUp$[ebp]
	push	edx
	mov	eax, DWORD PTR _szCustomText$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szDefaultText$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nID$[ebp]
	push	edx
	mov	eax, DWORD PTR _pParentWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _crColour$[ebp]
	push	ecx
	mov	edx, DWORD PTR _p$[ebp+4]
	push	edx
	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Create@CColourPopup@@QAEHVCPoint@@KPAVCWnd@@IPBG2H@Z ; CColourPopup::Create

; 125  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	32					; 00000020H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74751:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CWnd@@UAE@XZ				; CWnd::~CWnd
	ret	0
$L74752:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	??1CSubclasser@@QAE@XZ			; CSubclasser::~CSubclasser
	ret	0
$L74753:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74754:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74755:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 184				; 000000b8H
	call	??1CFont@@UAE@XZ			; CFont::~CFont
	ret	0
$L74756:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	??1CPalette@@UAE@XZ			; CPalette::~CPalette
	ret	0
$L74757:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 208				; 000000d0H
	call	??1CToolTipCtrl@@UAE@XZ			; CToolTipCtrl::~CToolTipCtrl
	ret	0
$L74760:
	mov	eax, OFFSET FLAT:$T74759
	jmp	___CxxFrameHandler
text$x	ENDS
??0CColourPopup@@QAE@VCPoint@@KPAVCWnd@@IPBG2H@Z ENDP	; CColourPopup::CColourPopup
EXTRN	_xGetSystemMetrics@4:NEAR
EXTRN	?CreateFontIndirectW@CFont@@QAEHPBUtagLOGFONTW@@@Z:NEAR ; CFont::CreateFontIndirectW
EXTRN	?CreatePalette@CPalette@@QAEHPAUtagLOGPALETTE@@@Z:NEAR ; CPalette::CreatePalette
EXTRN	__imp__SystemParametersInfoW@16:NEAR
;	COMDAT ?Initialise@CColourPopup@@QAEXXZ
_TEXT	SEGMENT
_this$ = -920
_ncm$ = -500
_pal$ = -916
_pLogPalette$ = -508
_i$ = -504
?Initialise@CColourPopup@@QAEXXZ PROC NEAR		; CColourPopup::Initialise, COMDAT

; 128  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 920				; 00000398H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-920]
	mov	ecx, 230				; 000000e6H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 129  :     m_nNumColours       = sizeof(m_crColours)/sizeof(ColourTableEntry);

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+100], 40			; 00000028H
$L74084:

; 130  :     ASSERT(m_nNumColours <= MAX_COLOURS);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+100], 100		; 00000064H
	jle	SHORT $L74087
	push	130					; 00000082H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74087
	int	3
$L74087:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74084

; 131  :     if (m_nNumColours > MAX_COLOURS)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+100], 100		; 00000064H
	jle	SHORT $L74088

; 132  :         m_nNumColours = MAX_COLOURS;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+100], 100		; 00000064H
$L74088:

; 133  : 
; 134  :     m_nNumColumns       = 0;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+104], 0

; 135  :     m_nNumRows          = 0;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+108], 0

; 136  :     m_nBoxSize          = 18;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+112], 18			; 00000012H

; 137  :     m_nMargin           = ::GetSystemMetrics(SM_CXEDGE);

	push	45					; 0000002dH
	call	_xGetSystemMetrics@4
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+116], eax

; 138  :     m_nCurrentSel       = INVALID_COLOUR;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+120], -1

; 139  :     m_nChosenColourSel  = INVALID_COLOUR;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+124], -1

; 140  :     m_pParent           = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+300], 0

; 141  :     m_crColour          = m_crInitialColour = RGB(0,0,0);

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+200], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+204], 0

; 142  : 
; 143  : 	m_bIgnoreFirstLBtnUp = FALSE;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+308], 0

; 144  :     m_bChildWindowVisible = FALSE;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+312], 0

; 145  : 
; 146  :     // Idiot check: Make sure the colour square is at least 5 x 5;
; 147  :     if (m_nBoxSize - 2*m_nMargin - 2 < 5) m_nBoxSize = 5 + 2*m_nMargin + 2;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+116]
	shl	edx, 1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+112]
	sub	ecx, edx
	sub	ecx, 2
	cmp	ecx, 5
	jge	SHORT $L74095
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+116]
	lea	ecx, DWORD PTR [eax+eax+7]
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+112], ecx
$L74095:

; 151  :     ncm.cbSize = sizeof(NONCLIENTMETRICS);

	mov	DWORD PTR _ncm$[ebp], 500		; 000001f4H
$L74098:

; 152  :     VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));

	mov	esi, esp
	push	0
	lea	eax, DWORD PTR _ncm$[ebp]
	push	eax
	push	500					; 000001f4H
	push	41					; 00000029H
	call	DWORD PTR __imp__SystemParametersInfoW@16
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L74102
	push	152					; 00000098H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74102
	int	3
$L74102:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74098

; 153  :     m_Font.CreateFontIndirect(&(ncm.lfMessageFont));

	lea	edx, DWORD PTR _ncm$[ebp+408]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 184				; 000000b8H
	call	?CreateFontIndirectW@CFont@@QAEHPBUtagLOGFONTW@@@Z ; CFont::CreateFontIndirectW

; 154  : 
; 155  :     // Create the palette
; 156  :     struct {
; 157  :         LOGPALETTE    LogPalette;
; 158  :         PALETTEENTRY  PalEntry[MAX_COLOURS];
; 159  :     } pal;
; 160  : 
; 161  :     LOGPALETTE* pLogPalette = (LOGPALETTE*) &pal;

	lea	eax, DWORD PTR _pal$[ebp]
	mov	DWORD PTR _pLogPalette$[ebp], eax

; 162  :     pLogPalette->palVersion    = 0x300;

	mov	ecx, DWORD PTR _pLogPalette$[ebp]
	mov	WORD PTR [ecx], 768			; 00000300H

; 163  :     pLogPalette->palNumEntries = (WORD) m_nNumColours; 

	mov	edx, DWORD PTR _pLogPalette$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	cx, WORD PTR [eax+100]
	mov	WORD PTR [edx+2], cx

; 164  : 
; 165  :     for (int i = 0; i < m_nNumColours; i++)

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L74118
$L74119:
	mov	edx, DWORD PTR _i$[ebp]
	add	edx, 1
	mov	DWORD PTR _i$[ebp], edx
$L74118:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _i$[ebp]
	cmp	ecx, DWORD PTR [eax+100]
	jge	SHORT $L74120

; 167  :         pLogPalette->palPalEntry[i].peRed   = GetRValue(m_crColours[i].crColour);

	mov	edx, DWORD PTR _i$[ebp]
	mov	eax, DWORD PTR _pLogPalette$[ebp]
	mov	ecx, DWORD PTR _i$[ebp]
	mov	cl, BYTE PTR ?m_crColours@CColourPopup@@1PAUColourTableEntry@@A[ecx*8]
	mov	BYTE PTR [eax+edx*4+4], cl

; 168  :         pLogPalette->palPalEntry[i].peGreen = GetGValue(m_crColours[i].crColour);

	mov	edx, DWORD PTR _i$[ebp]
	xor	eax, eax
	mov	ax, WORD PTR ?m_crColours@CColourPopup@@1PAUColourTableEntry@@A[edx*8]
	sar	eax, 8
	mov	ecx, DWORD PTR _i$[ebp]
	mov	edx, DWORD PTR _pLogPalette$[ebp]
	mov	BYTE PTR [edx+ecx*4+5], al

; 169  :         pLogPalette->palPalEntry[i].peBlue  = GetBValue(m_crColours[i].crColour);

	mov	eax, DWORD PTR _i$[ebp]
	mov	ecx, DWORD PTR ?m_crColours@CColourPopup@@1PAUColourTableEntry@@A[eax*8]
	shr	ecx, 16					; 00000010H
	mov	edx, DWORD PTR _i$[ebp]
	mov	eax, DWORD PTR _pLogPalette$[ebp]
	mov	BYTE PTR [eax+edx*4+6], cl

; 170  :         pLogPalette->palPalEntry[i].peFlags = 0;

	mov	ecx, DWORD PTR _i$[ebp]
	mov	edx, DWORD PTR _pLogPalette$[ebp]
	mov	BYTE PTR [edx+ecx*4+7], 0

; 171  :     }

	jmp	$L74119
$L74120:

; 172  : 
; 173  :     m_Palette.CreatePalette(pLogPalette);

	mov	eax, DWORD PTR _pLogPalette$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	?CreatePalette@CPalette@@QAEHPAUtagLOGPALETTE@@@Z ; CPalette::CreatePalette

; 174  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 920				; 00000398H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Initialise@CColourPopup@@QAEXXZ ENDP			; CColourPopup::Initialise
_TEXT	ENDS
EXTRN	?DeleteObject@CGdiObject@@QAEHXZ:NEAR		; CGdiObject::DeleteObject
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\ColourPopup.cpp
xdata$x	SEGMENT
$T74780	DD	019930520H
	DD	07H
	DD	FLAT:$T74782
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74782	DD	0ffffffffH
	DD	FLAT:$L74770
	DD	00H
	DD	FLAT:$L74773
	DD	01H
	DD	FLAT:$L74774
	DD	02H
	DD	FLAT:$L74775
	DD	03H
	DD	FLAT:$L74776
	DD	04H
	DD	FLAT:$L74777
	DD	05H
	DD	FLAT:$L74778
xdata$x	ENDS
;	COMDAT ??1CColourPopup@@UAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??1CColourPopup@@UAE@XZ PROC NEAR			; CColourPopup::~CColourPopup, COMDAT

; 177  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74781
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CColourPopup@@6BCWnd@@@ ; CColourPopup::`vftable'
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+64], OFFSET FLAT:??_7CColourPopup@@6BCSubclasser@@@ ; CColourPopup::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 6

; 178  :     m_Font.DeleteObject();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 184				; 000000b8H
	call	?DeleteObject@CGdiObject@@QAEHXZ	; CGdiObject::DeleteObject

; 179  :     m_Palette.DeleteObject();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	?DeleteObject@CGdiObject@@QAEHXZ	; CGdiObject::DeleteObject

; 180  : }

	mov	BYTE PTR __$EHRec$[ebp+8], 5
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 208				; 000000d0H
	call	??1CToolTipCtrl@@UAE@XZ			; CToolTipCtrl::~CToolTipCtrl
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	??1CPalette@@UAE@XZ			; CPalette::~CPalette
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 184				; 000000b8H
	call	??1CFont@@UAE@XZ			; CFont::~CFont
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	cmp	DWORD PTR _this$[ebp], 0
	je	SHORT $L74768
	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 64					; 00000040H
	mov	DWORD PTR -20+[ebp], edx
	jmp	SHORT $L74769
$L74768:
	mov	DWORD PTR -20+[ebp], 0
$L74769:
	mov	ecx, DWORD PTR -20+[ebp]
	call	??1CSubclasser@@QAE@XZ			; CSubclasser::~CSubclasser
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CWnd@@UAE@XZ				; CWnd::~CWnd
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
$L74770:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CWnd@@UAE@XZ				; CWnd::~CWnd
	ret	0
$L74773:
	cmp	DWORD PTR _this$[ebp], 0
	je	$L74771
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 64					; 00000040H
	mov	DWORD PTR -20+[ebp], eax
	jmp	$L74772
$L74771:
	mov	DWORD PTR -20+[ebp], 0
$L74772:
	mov	ecx, DWORD PTR -20+[ebp]
	call	??1CSubclasser@@QAE@XZ			; CSubclasser::~CSubclasser
	ret	0
$L74774:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74775:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74776:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 184				; 000000b8H
	call	??1CFont@@UAE@XZ			; CFont::~CFont
	ret	0
$L74777:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 192				; 000000c0H
	call	??1CPalette@@UAE@XZ			; CPalette::~CPalette
	ret	0
$L74778:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 208				; 000000d0H
	call	??1CToolTipCtrl@@UAE@XZ			; CToolTipCtrl::~CToolTipCtrl
	ret	0
$L74781:
	mov	eax, OFFSET FLAT:$T74780
	jmp	___CxxFrameHandler
text$x	ENDS
??1CColourPopup@@UAE@XZ ENDP				; CColourPopup::~CColourPopup
PUBLIC	?ScHookWindow@CSubclasser@@IAEHPAUHWND__@@@Z	; CSubclasser::ScHookWindow
PUBLIC	?FindCellFromColour@CColourPopup@@IAEXK@Z	; CColourPopup::FindCellFromColour
PUBLIC	?SetWindowSize@CColourPopup@@IAEXXZ		; CColourPopup::SetWindowSize
PUBLIC	?CreateToolTips@CColourPopup@@IAEXXZ		; CColourPopup::CreateToolTips
EXTRN	?CreateEx@CWnd@@QAEHKPBG0KHHHHPAUHWND__@@PAUHMENU__@@PAX@Z:NEAR ; CWnd::CreateEx
EXTRN	?GetDlgCtrlID@CWnd@@QBEHXZ:NEAR			; CWnd::GetDlgCtrlID
EXTRN	?ShowWindow@CWnd@@QAEHH@Z:NEAR			; CWnd::ShowWindow
EXTRN	?SetCapture@CWnd@@QAEPAV1@XZ:NEAR		; CWnd::SetCapture
EXTRN	?SetFocus@CWnd@@QAEPAV1@XZ:NEAR			; CWnd::SetFocus
EXTRN	__imp__IsWindow@4:NEAR
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	??4CString@@QAEABV0@PBG@Z:NEAR			; CString::operator=
EXTRN	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ:NEAR	; CWnd::GetSafeHwnd
EXTRN	?AfxRegisterWndClass@@YGPBGIPAUHICON__@@PAUHBRUSH__@@0@Z:NEAR ; AfxRegisterWndClass
_BSS	SEGMENT
$SG74146 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\ColourPopup.cpp
xdata$x	SEGMENT
$T74793	DD	019930520H
	DD	01H
	DD	FLAT:$T74795
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74795	DD	0ffffffffH
	DD	FLAT:$L74791
xdata$x	ENDS
;	COMDAT ?Create@CColourPopup@@QAEHVCPoint@@KPAVCWnd@@IPBG2H@Z
_TEXT	SEGMENT
$T74789 = -20
$T74790 = -24
__$EHRec$ = -12
_p$ = 8
_crColour$ = 16
_pParentWnd$ = 20
_nID$ = 24
_szDefaultText$ = 28
_szCustomText$ = 32
_bIgnoreFirstLBtnUp$ = 36
_this$ = -28
_szClassName$ = -16
?Create@CColourPopup@@QAEHVCPoint@@KPAVCWnd@@IPBG2H@Z PROC NEAR ; CColourPopup::Create, COMDAT

; 187  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74794
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 20					; 00000014H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx
$L74138:

; 188  :     ASSERT(pParentWnd && ::IsWindow(pParentWnd->GetSafeHwnd()));

	cmp	DWORD PTR _pParentWnd$[ebp], 0
	je	SHORT $L74142
	mov	ecx, DWORD PTR _pParentWnd$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L74141
$L74142:
	push	188					; 000000bcH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74141
	int	3
$L74141:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74138

; 189  : //    ASSERT(pParentWnd->IsKindOf(RUNTIME_CLASS(CColourPicker)));
; 190  : 
; 191  :     m_pParent  = pParentWnd;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _pParentWnd$[ebp]
	mov	DWORD PTR [ecx+300], edx

; 192  :     m_crColour = m_crInitialColour = crColour;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _crColour$[ebp]
	mov	DWORD PTR [eax+200], ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _crColour$[ebp]
	mov	DWORD PTR [edx+204], eax

; 193  : 	m_bIgnoreFirstLBtnUp = bIgnoreFirstLBtnUp;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _bIgnoreFirstLBtnUp$[ebp]
	mov	DWORD PTR [ecx+308], edx

; 194  : 	m_nID = (nID != 0) ? nID : pParentWnd->GetDlgCtrlID();

	cmp	DWORD PTR _nID$[ebp], 0
	je	SHORT $L74787
	mov	eax, DWORD PTR _nID$[ebp]
	mov	DWORD PTR -32+[ebp], eax
	jmp	SHORT $L74788
$L74787:
	mov	ecx, DWORD PTR _pParentWnd$[ebp]
	call	?GetDlgCtrlID@CWnd@@QBEHXZ		; CWnd::GetDlgCtrlID
	mov	DWORD PTR -32+[ebp], eax
$L74788:
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR -32+[ebp]
	mov	DWORD PTR [ecx+304], edx

; 195  : 
; 196  :     // Get the class name and create the window
; 197  :     CString szClassName = AfxRegisterWndClass(CS_CLASSDC|CS_SAVEBITS|CS_HREDRAW|CS_VREDRAW,
; 198  :                                               0,
; 199  :                                               (HBRUSH) (COLOR_BTNFACE+1), 
; 200  :                                               0);

	push	0
	push	16					; 00000010H
	push	0
	push	2115					; 00000843H
	call	?AfxRegisterWndClass@@YGPBGIPAUHICON__@@PAUHBRUSH__@@0@Z ; AfxRegisterWndClass
	push	eax
	lea	ecx, DWORD PTR _szClassName$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 203  :                         p.x, p.y, 100, 100, // size updated soon
; 204  :                         pParentWnd->GetSafeHwnd(), 0, NULL))

	push	0
	push	0
	mov	ecx, DWORD PTR _pParentWnd$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	push	eax
	push	100					; 00000064H
	push	100					; 00000064H
	mov	eax, DWORD PTR _p$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	push	ecx
	push	-2147483648				; 80000000H
	push	OFFSET FLAT:$SG74146
	lea	ecx, DWORD PTR _szClassName$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?CreateEx@CWnd@@QAEHKPBG0KHHHHPAUHWND__@@PAUHMENU__@@PAX@Z ; CWnd::CreateEx
	test	eax, eax
	jne	SHORT $L74145

; 205  :         return FALSE;

	mov	DWORD PTR $T74789[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _szClassName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74789[ebp]
	jmp	$L74137
$L74145:

; 206  : 
; 207  :     // Store the Custom text
; 208  :     if (szCustomText != NULL) 

	cmp	DWORD PTR _szCustomText$[ebp], 0
	je	SHORT $L74148

; 209  :         m_strCustomText = szCustomText;

	mov	edx, DWORD PTR _szCustomText$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L74148:

; 210  : 
; 211  :     // Store the Default Area text
; 212  :     if (szDefaultText != NULL) 

	cmp	DWORD PTR _szDefaultText$[ebp], 0
	je	SHORT $L74149

; 213  :         m_strDefaultText = szDefaultText;

	mov	eax, DWORD PTR _szDefaultText$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=
$L74149:

; 214  :         
; 215  :     // Set the window size
; 216  :     SetWindowSize();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetWindowSize@CColourPopup@@IAEXXZ	; CColourPopup::SetWindowSize

; 217  : 
; 218  :     // Create the tooltips
; 219  :     CreateToolTips();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?CreateToolTips@CColourPopup@@IAEXXZ	; CColourPopup::CreateToolTips

; 220  : 
; 221  :     // Find which cell (if any) corresponds to the initial colour
; 222  :     FindCellFromColour(crColour);

	mov	ecx, DWORD PTR _crColour$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FindCellFromColour@CColourPopup@@IAEXK@Z ; CColourPopup::FindCellFromColour

; 223  : 
; 224  : 	pParentWnd->SetFocus();

	mov	ecx, DWORD PTR _pParentWnd$[ebp]
	call	?SetFocus@CWnd@@QAEPAV1@XZ		; CWnd::SetFocus

; 225  : 	ShowWindow(SW_SHOWNOACTIVATE);

	push	4
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ShowWindow@CWnd@@QAEHH@Z		; CWnd::ShowWindow

; 226  : 
; 227  :     // Capture all mouse events for the life of this window
; 228  :     SetCapture();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetCapture@CWnd@@QAEPAV1@XZ		; CWnd::SetCapture

; 229  : 
; 230  : 	// Hook the parent wnd
; 231  : 	return ScHookWindow(pParentWnd->GetSafeHwnd());

	mov	ecx, DWORD PTR _pParentWnd$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	?ScHookWindow@CSubclasser@@IAEHPAUHWND__@@@Z ; CSubclasser::ScHookWindow
	mov	DWORD PTR $T74790[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _szClassName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T74790[ebp]
$L74137:

; 232  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	32					; 00000020H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74791:
	lea	ecx, DWORD PTR _szClassName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74794:
	mov	eax, OFFSET FLAT:$T74793
	jmp	___CxxFrameHandler
text$x	ENDS
?Create@CColourPopup@@QAEHVCPoint@@KPAVCWnd@@IPBG2H@Z ENDP ; CColourPopup::Create
;	COMDAT ?ScHookWindow@CSubclasser@@IAEHPAUHWND__@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
_this$ = -4
?ScHookWindow@CSubclasser@@IAEHPAUHWND__@@@Z PROC NEAR	; CSubclasser::ScHookWindow, COMDAT

; 151  : 	inline BOOL ScHookWindow(HWND hWnd) { return m_subclass.HookWindow(hWnd, GetTopSubclasser()); }

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp
	mov	esi, esp
	push	eax
	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	call	DWORD PTR [eax+4]
	cmp	esi, esp
	call	__chkesp
	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ScHookWindow@CSubclasser@@IAEHPAUHWND__@@@Z ENDP	; CSubclasser::ScHookWindow
_TEXT	ENDS
EXTRN	__imp_?messageMap@CWnd@@1UAFX_MSGMAP@@B:QWORD
;	COMDAT ?_GetBaseMessageMap@CColourPopup@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
?_GetBaseMessageMap@CColourPopup@@KGPBUAFX_MSGMAP@@XZ PROC NEAR ; CColourPopup::_GetBaseMessageMap, COMDAT

; 234  : BEGIN_MESSAGE_MAP(CColourPopup, CWnd)

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR __imp_?messageMap@CWnd@@1UAFX_MSGMAP@@B
	pop	ebp
	ret	0
?_GetBaseMessageMap@CColourPopup@@KGPBUAFX_MSGMAP@@XZ ENDP ; CColourPopup::_GetBaseMessageMap
_TEXT	ENDS
;	COMDAT ?GetMessageMap@CColourPopup@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetMessageMap@CColourPopup@@MBEPBUAFX_MSGMAP@@XZ PROC NEAR ; CColourPopup::GetMessageMap, COMDAT

; 234  : BEGIN_MESSAGE_MAP(CColourPopup, CWnd)

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, OFFSET FLAT:?messageMap@CColourPopup@@1UAFX_MSGMAP@@B ; CColourPopup::messageMap
	mov	esp, ebp
	pop	ebp
	ret	0
?GetMessageMap@CColourPopup@@MBEPBUAFX_MSGMAP@@XZ ENDP	; CColourPopup::GetMessageMap
_TEXT	ENDS
EXTRN	?GetCapture@CWnd@@SGPAV1@XZ:NEAR		; CWnd::GetCapture
EXTRN	?RelayEvent@CToolTipCtrl@@QAEXPAUtagMSG@@@Z:NEAR ; CToolTipCtrl::RelayEvent
EXTRN	?PreTranslateMessage@CWnd@@UAEHPAUtagMSG@@@Z:NEAR ; CWnd::PreTranslateMessage
;	COMDAT ?PreTranslateMessage@CColourPopup@@UAEHPAUtagMSG@@@Z
_TEXT	SEGMENT
_pMsg$ = 8
_this$ = -4
?PreTranslateMessage@CColourPopup@@UAEHPAUtagMSG@@@Z PROC NEAR ; CColourPopup::PreTranslateMessage, COMDAT

; 252  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 253  :     m_ToolTip.RelayEvent(pMsg);

	mov	eax, DWORD PTR _pMsg$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 208				; 000000d0H
	call	?RelayEvent@CToolTipCtrl@@QAEXPAUtagMSG@@@Z ; CToolTipCtrl::RelayEvent

; 254  : 
; 255  :     // Fix (Adrian Roman): Sometimes if the picker loses focus it is never destroyed
; 256  :     if (GetCapture()->GetSafeHwnd() != m_hWnd)

	call	?GetCapture@CWnd@@SGPAV1@XZ		; CWnd::GetCapture
	mov	ecx, eax
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	mov	ecx, DWORD PTR _this$[ebp]
	cmp	eax, DWORD PTR [ecx+32]
	je	SHORT $L74204

; 257  :         SetCapture(); 

	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetCapture@CWnd@@QAEPAV1@XZ		; CWnd::SetCapture
$L74204:

; 258  : 
; 259  :     return CWnd::PreTranslateMessage(pMsg);

	mov	edx, DWORD PTR _pMsg$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?PreTranslateMessage@CWnd@@UAEHPAUtagMSG@@@Z ; CWnd::PreTranslateMessage

; 260  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?PreTranslateMessage@CColourPopup@@UAEHPAUtagMSG@@@Z ENDP ; CColourPopup::PreTranslateMessage
_TEXT	ENDS
PUBLIC	?ChangeSelection@CColourPopup@@IAEXH@Z		; CColourPopup::ChangeSelection
PUBLIC	?EndSelection@CColourPopup@@IAEXH@Z		; CColourPopup::EndSelection
PUBLIC	?GetIndex@CColourPopup@@IBEHHH@Z		; CColourPopup::GetIndex
PUBLIC	?GetRow@CColourPopup@@IBEHH@Z			; CColourPopup::GetRow
PUBLIC	?GetColumn@CColourPopup@@IBEHH@Z		; CColourPopup::GetColumn
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	?ScDefault@CSubclasser@@IAEJPAUHWND__@@@Z:NEAR	; CSubclasser::ScDefault
;	COMDAT ?ScWindowProc@CColourPopup@@MAEJPAUHWND__@@IIJ@Z
_TEXT	SEGMENT
_hRealWnd$ = 8
_msg$ = 12
_wp$ = 16
_this$ = -12
_row$74217 = -8
_col$74218 = -4
?ScWindowProc@CColourPopup@@MAEJPAUHWND__@@IIJ@Z PROC NEAR ; CColourPopup::ScWindowProc, COMDAT

; 263  : {

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

; 265  : 	{

	mov	eax, DWORD PTR _msg$[ebp]
	mov	DWORD PTR -16+[ebp], eax
	cmp	DWORD PTR -16+[ebp], 256		; 00000100H
	je	SHORT $L74216
	jmp	$L74213
$L74216:

; 268  : 			int row = GetRow(m_nCurrentSel),

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+56]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetRow@CColourPopup@@IBEHH@Z		; CColourPopup::GetRow
	mov	DWORD PTR _row$74217[ebp], eax

; 269  : 				col = GetColumn(m_nCurrentSel);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+56]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetColumn@CColourPopup@@IBEHH@Z	; CColourPopup::GetColumn
	mov	DWORD PTR _col$74218[ebp], eax

; 272  : 			{

	mov	edx, DWORD PTR _wp$[ebp]
	mov	DWORD PTR -20+[ebp], edx
	mov	eax, DWORD PTR -20+[ebp]
	sub	eax, 13					; 0000000dH
	mov	DWORD PTR -20+[ebp], eax
	cmp	DWORD PTR -20+[ebp], 27			; 0000001bH
	ja	$L74220
	mov	edx, DWORD PTR -20+[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR $L74808[edx]
	jmp	DWORD PTR $L74809[ecx*4]
$L74223:

; 275  : 					if (row == DEFAULT_BOX_VALUE) 

	cmp	DWORD PTR _row$74217[ebp], -3		; fffffffdH
	jne	SHORT $L74224

; 276  : 						row = col = 0; 

	mov	DWORD PTR _col$74218[ebp], 0
	mov	eax, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], eax

; 277  : 
; 278  : 					else if (row == CUSTOM_BOX_VALUE)

	jmp	$L74234
$L74224:
	cmp	DWORD PTR _row$74217[ebp], -2		; fffffffeH
	jne	SHORT $L74226

; 280  : 						if (m_strDefaultText.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74227

; 281  : 							row = col = DEFAULT_BOX_VALUE;

	mov	DWORD PTR _col$74218[ebp], -3		; fffffffdH
	mov	ecx, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], ecx

; 282  : 						else

	jmp	SHORT $L74228
$L74227:

; 283  : 							row = col = 0;

	mov	DWORD PTR _col$74218[ebp], 0
	mov	edx, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], edx
$L74228:

; 285  : 					else

	jmp	SHORT $L74234
$L74226:

; 287  : 						row++;

	mov	eax, DWORD PTR _row$74217[ebp]
	add	eax, 1
	mov	DWORD PTR _row$74217[ebp], eax

; 288  : 						if (GetIndex(row,col) < 0)

	mov	ecx, DWORD PTR _col$74218[ebp]
	push	ecx
	mov	edx, DWORD PTR _row$74217[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetIndex@CColourPopup@@IBEHHH@Z	; CColourPopup::GetIndex
	test	eax, eax
	jge	SHORT $L74234

; 290  : 							if (m_strCustomText.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 68					; 00000044H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74231

; 291  : 								row = col = CUSTOM_BOX_VALUE;

	mov	DWORD PTR _col$74218[ebp], -2		; fffffffeH
	mov	eax, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], eax

; 292  : 							else if (m_strDefaultText.GetLength())

	jmp	SHORT $L74234
$L74231:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74233

; 293  : 								row = col = DEFAULT_BOX_VALUE;

	mov	DWORD PTR _col$74218[ebp], -3		; fffffffdH
	mov	ecx, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], ecx

; 294  : 							else

	jmp	SHORT $L74234
$L74233:

; 295  : 								row = col = 0;

	mov	DWORD PTR _col$74218[ebp], 0
	mov	edx, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], edx
$L74234:

; 298  : 					ChangeSelection(GetIndex(row, col));

	mov	eax, DWORD PTR _col$74218[ebp]
	push	eax
	mov	ecx, DWORD PTR _row$74217[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetIndex@CColourPopup@@IBEHHH@Z	; CColourPopup::GetIndex
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?ChangeSelection@CColourPopup@@IAEXH@Z	; CColourPopup::ChangeSelection

; 300  : 				return 0L; // eat

	xor	eax, eax
	jmp	$L74211
$L74235:

; 304  : 					if (row == DEFAULT_BOX_VALUE)

	cmp	DWORD PTR _row$74217[ebp], -3		; fffffffdH
	jne	SHORT $L74236

; 306  : 						if (m_strCustomText.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 68					; 00000044H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74237

; 307  : 							row = col = CUSTOM_BOX_VALUE;

	mov	DWORD PTR _col$74218[ebp], -2		; fffffffeH
	mov	edx, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], edx

; 308  : 						else

	jmp	SHORT $L74238
$L74237:

; 310  : 							row = GetRow(m_nNumColours-1); 

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+36]
	sub	ecx, 1
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetRow@CColourPopup@@IBEHH@Z		; CColourPopup::GetRow
	mov	DWORD PTR _row$74217[ebp], eax

; 311  : 							col = GetColumn(m_nNumColours-1); 

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+36]
	sub	eax, 1
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetColumn@CColourPopup@@IBEHH@Z	; CColourPopup::GetColumn
	mov	DWORD PTR _col$74218[ebp], eax
$L74238:

; 314  : 					else if (row == CUSTOM_BOX_VALUE)

	jmp	$L74247
$L74236:
	cmp	DWORD PTR _row$74217[ebp], -2		; fffffffeH
	jne	SHORT $L74240

; 316  : 						row = GetRow(m_nNumColours-1); 

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+36]
	sub	edx, 1
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetRow@CColourPopup@@IBEHH@Z		; CColourPopup::GetRow
	mov	DWORD PTR _row$74217[ebp], eax

; 317  : 						col = GetColumn(m_nNumColours-1); 

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+36]
	sub	ecx, 1
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetColumn@CColourPopup@@IBEHH@Z	; CColourPopup::GetColumn
	mov	DWORD PTR _col$74218[ebp], eax

; 319  : 					else if (row > 0) 

	jmp	SHORT $L74247
$L74240:
	cmp	DWORD PTR _row$74217[ebp], 0
	jle	SHORT $L74242

; 320  : 						row--;

	mov	edx, DWORD PTR _row$74217[ebp]
	sub	edx, 1
	mov	DWORD PTR _row$74217[ebp], edx

; 321  : 					else /* row == 0 */

	jmp	SHORT $L74247
$L74242:

; 323  : 						if (m_strDefaultText.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74244

; 324  : 							row = col = DEFAULT_BOX_VALUE;

	mov	DWORD PTR _col$74218[ebp], -3		; fffffffdH
	mov	eax, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], eax

; 325  : 						else if (m_strCustomText.GetLength())

	jmp	SHORT $L74247
$L74244:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 68					; 00000044H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74246

; 326  : 							row = col = CUSTOM_BOX_VALUE;

	mov	DWORD PTR _col$74218[ebp], -2		; fffffffeH
	mov	ecx, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], ecx

; 327  : 						else

	jmp	SHORT $L74247
$L74246:

; 329  : 							row = GetRow(m_nNumColours-1); 

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+36]
	sub	eax, 1
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetRow@CColourPopup@@IBEHH@Z		; CColourPopup::GetRow
	mov	DWORD PTR _row$74217[ebp], eax

; 330  : 							col = GetColumn(m_nNumColours-1); 

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+36]
	sub	edx, 1
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetColumn@CColourPopup@@IBEHH@Z	; CColourPopup::GetColumn
	mov	DWORD PTR _col$74218[ebp], eax
$L74247:

; 333  : 					ChangeSelection(GetIndex(row, col));

	mov	eax, DWORD PTR _col$74218[ebp]
	push	eax
	mov	ecx, DWORD PTR _row$74217[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetIndex@CColourPopup@@IBEHHH@Z	; CColourPopup::GetIndex
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?ChangeSelection@CColourPopup@@IAEXH@Z	; CColourPopup::ChangeSelection

; 335  : 				return 0L; // eat

	xor	eax, eax
	jmp	$L74211
$L74248:

; 339  : 					if (row == DEFAULT_BOX_VALUE) 

	cmp	DWORD PTR _row$74217[ebp], -3		; fffffffdH
	jne	SHORT $L74249

; 340  : 						row = col = 0; 

	mov	DWORD PTR _col$74218[ebp], 0
	mov	edx, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], edx

; 341  : 					
; 342  : 					else if (row == CUSTOM_BOX_VALUE)

	jmp	SHORT $L74256
$L74249:
	cmp	DWORD PTR _row$74217[ebp], -2		; fffffffeH
	jne	SHORT $L74251

; 344  : 						if (m_strDefaultText.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74252

; 345  : 							row = col = DEFAULT_BOX_VALUE;

	mov	DWORD PTR _col$74218[ebp], -3		; fffffffdH
	mov	eax, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], eax

; 346  : 						else

	jmp	SHORT $L74253
$L74252:

; 347  : 							row = col = 0;

	mov	DWORD PTR _col$74218[ebp], 0
	mov	ecx, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], ecx
$L74253:

; 349  : 					else if (col < m_nNumColumns-1) 

	jmp	SHORT $L74256
$L74251:
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+40]
	sub	eax, 1
	cmp	DWORD PTR _col$74218[ebp], eax
	jge	SHORT $L74255

; 350  : 						col++;

	mov	ecx, DWORD PTR _col$74218[ebp]
	add	ecx, 1
	mov	DWORD PTR _col$74218[ebp], ecx

; 351  : 					else 

	jmp	SHORT $L74256
$L74255:

; 353  : 						col = 0; row++;

	mov	DWORD PTR _col$74218[ebp], 0
	mov	edx, DWORD PTR _row$74217[ebp]
	add	edx, 1
	mov	DWORD PTR _row$74217[ebp], edx
$L74256:

; 355  : 					
; 356  : 					if (GetIndex(row,col) == INVALID_COLOUR)

	mov	eax, DWORD PTR _col$74218[ebp]
	push	eax
	mov	ecx, DWORD PTR _row$74217[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetIndex@CColourPopup@@IBEHHH@Z	; CColourPopup::GetIndex
	cmp	eax, -1
	jne	SHORT $L74261

; 358  : 						if (m_strCustomText.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 68					; 00000044H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74258

; 359  : 							row = col = CUSTOM_BOX_VALUE;

	mov	DWORD PTR _col$74218[ebp], -2		; fffffffeH
	mov	edx, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], edx

; 360  : 						else if (m_strDefaultText.GetLength())

	jmp	SHORT $L74261
$L74258:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74260

; 361  : 							row = col = DEFAULT_BOX_VALUE;

	mov	DWORD PTR _col$74218[ebp], -3		; fffffffdH
	mov	eax, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], eax

; 362  : 						else

	jmp	SHORT $L74261
$L74260:

; 363  : 							row = col = 0;

	mov	DWORD PTR _col$74218[ebp], 0
	mov	ecx, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], ecx
$L74261:

; 365  : 					
; 366  : 					ChangeSelection(GetIndex(row, col));

	mov	edx, DWORD PTR _col$74218[ebp]
	push	edx
	mov	eax, DWORD PTR _row$74217[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetIndex@CColourPopup@@IBEHHH@Z	; CColourPopup::GetIndex
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?ChangeSelection@CColourPopup@@IAEXH@Z	; CColourPopup::ChangeSelection

; 368  : 				return 0L; // eat

	xor	eax, eax
	jmp	$L74211
$L74262:

; 372  : 					if (row == DEFAULT_BOX_VALUE)

	cmp	DWORD PTR _row$74217[ebp], -3		; fffffffdH
	jne	SHORT $L74263

; 374  : 						if (m_strCustomText.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 68					; 00000044H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74264

; 375  : 							row = col = CUSTOM_BOX_VALUE;

	mov	DWORD PTR _col$74218[ebp], -2		; fffffffeH
	mov	ecx, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], ecx

; 376  : 						else

	jmp	SHORT $L74265
$L74264:

; 378  : 							row = GetRow(m_nNumColours-1); 

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+36]
	sub	eax, 1
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetRow@CColourPopup@@IBEHH@Z		; CColourPopup::GetRow
	mov	DWORD PTR _row$74217[ebp], eax

; 379  : 							col = GetColumn(m_nNumColours-1); 

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+36]
	sub	edx, 1
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetColumn@CColourPopup@@IBEHH@Z	; CColourPopup::GetColumn
	mov	DWORD PTR _col$74218[ebp], eax
$L74265:

; 382  : 					else if (row == CUSTOM_BOX_VALUE)

	jmp	$L74276
$L74263:
	cmp	DWORD PTR _row$74217[ebp], -2		; fffffffeH
	jne	SHORT $L74267

; 384  : 						row = GetRow(m_nNumColours-1); 

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+36]
	sub	ecx, 1
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetRow@CColourPopup@@IBEHH@Z		; CColourPopup::GetRow
	mov	DWORD PTR _row$74217[ebp], eax

; 385  : 						col = GetColumn(m_nNumColours-1); 

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+36]
	sub	eax, 1
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetColumn@CColourPopup@@IBEHH@Z	; CColourPopup::GetColumn
	mov	DWORD PTR _col$74218[ebp], eax

; 387  : 					else if (col > 0) 

	jmp	$L74276
$L74267:
	cmp	DWORD PTR _col$74218[ebp], 0
	jle	SHORT $L74269

; 388  : 						col--;

	mov	ecx, DWORD PTR _col$74218[ebp]
	sub	ecx, 1
	mov	DWORD PTR _col$74218[ebp], ecx

; 389  : 					else /* col == 0 */

	jmp	$L74276
$L74269:

; 391  : 						if (row > 0) { row--; col = m_nNumColumns-1; }

	cmp	DWORD PTR _row$74217[ebp], 0
	jle	SHORT $L74271
	mov	edx, DWORD PTR _row$74217[ebp]
	sub	edx, 1
	mov	DWORD PTR _row$74217[ebp], edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+40]
	sub	ecx, 1
	mov	DWORD PTR _col$74218[ebp], ecx

; 392  : 						else 

	jmp	SHORT $L74276
$L74271:

; 394  : 							if (m_strDefaultText.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 64					; 00000040H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74273

; 395  : 								row = col = DEFAULT_BOX_VALUE;

	mov	DWORD PTR _col$74218[ebp], -3		; fffffffdH
	mov	edx, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], edx

; 396  : 							else if (m_strCustomText.GetLength())

	jmp	SHORT $L74276
$L74273:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 68					; 00000044H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74275

; 397  : 								row = col = CUSTOM_BOX_VALUE;

	mov	DWORD PTR _col$74218[ebp], -2		; fffffffeH
	mov	eax, DWORD PTR _col$74218[ebp]
	mov	DWORD PTR _row$74217[ebp], eax

; 398  : 							else

	jmp	SHORT $L74276
$L74275:

; 400  : 								row = GetRow(m_nNumColours-1); 

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+36]
	sub	edx, 1
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetRow@CColourPopup@@IBEHH@Z		; CColourPopup::GetRow
	mov	DWORD PTR _row$74217[ebp], eax

; 401  : 								col = GetColumn(m_nNumColours-1); 

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+36]
	sub	ecx, 1
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetColumn@CColourPopup@@IBEHH@Z	; CColourPopup::GetColumn
	mov	DWORD PTR _col$74218[ebp], eax
$L74276:

; 405  : 					ChangeSelection(GetIndex(row, col));

	mov	edx, DWORD PTR _col$74218[ebp]
	push	edx
	mov	eax, DWORD PTR _row$74217[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?GetIndex@CColourPopup@@IBEHHH@Z	; CColourPopup::GetIndex
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?ChangeSelection@CColourPopup@@IAEXH@Z	; CColourPopup::ChangeSelection

; 407  : 				return 0L; // eat

	xor	eax, eax
	jmp	SHORT $L74211
$L74277:

; 408  : 				
; 409  : 			case VK_ESCAPE:
; 410  : 				m_crColour = m_crInitialColour;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+136]
	mov	DWORD PTR [ecx+140], eax

; 411  : 				EndSelection(CPN_SELENDCANCEL);

	push	2029					; 000007edH
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?EndSelection@CColourPopup@@IAEXH@Z	; CColourPopup::EndSelection

; 412  : 				return 0L; // eat

	xor	eax, eax
	jmp	SHORT $L74211
$L74278:

; 413  : 				
; 414  : 			case VK_RETURN:
; 415  : 			case VK_SPACE:
; 416  : 				EndSelection(CPN_SELENDOK);

	push	2028					; 000007ecH
	mov	ecx, DWORD PTR _this$[ebp]
	sub	ecx, 64					; 00000040H
	call	?EndSelection@CColourPopup@@IAEXH@Z	; CColourPopup::EndSelection

; 417  : 				return 0L; // eat

	xor	eax, eax
	jmp	SHORT $L74211
$L74220:
$L74213:

; 422  : 
; 423  : 	return CSubclasser::ScDefault(hRealWnd);

	mov	ecx, DWORD PTR _hRealWnd$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ScDefault@CSubclasser@@IAEJPAUHWND__@@@Z ; CSubclasser::ScDefault
$L74211:

; 424  : }

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
$L74809:
	DD	$L74278
	DD	$L74277
	DD	$L74262
	DD	$L74235
	DD	$L74248
	DD	$L74223
	DD	$L74220
$L74808:
	DB	0
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	1
	DB	6
	DB	6
	DB	6
	DB	6
	DB	0
	DB	6
	DB	6
	DB	6
	DB	6
	DB	2
	DB	3
	DB	4
	DB	5
?ScWindowProc@CColourPopup@@MAEJPAUHWND__@@IIJ@Z ENDP	; CColourPopup::ScWindowProc
_TEXT	ENDS
EXTRN	?OnNcDestroy@CWnd@@IAEXXZ:NEAR			; CWnd::OnNcDestroy
;	COMDAT ?OnNcDestroy@CColourPopup@@IAEXXZ
_TEXT	SEGMENT
$T74812 = -4
$T74813 = -8
_this$ = -12
?OnNcDestroy@CColourPopup@@IAEXXZ PROC NEAR		; CColourPopup::OnNcDestroy, COMDAT

; 428  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 429  :     CWnd::OnNcDestroy();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnNcDestroy@CWnd@@IAEXXZ		; CWnd::OnNcDestroy

; 430  :     delete this;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR $T74813[ebp], eax
	mov	ecx, DWORD PTR $T74813[ebp]
	mov	DWORD PTR $T74812[ebp], ecx
	cmp	DWORD PTR $T74812[ebp], 0
	je	SHORT $L74814
	mov	esi, esp
	push	1
	mov	edx, DWORD PTR $T74812[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR $T74812[ebp]
	call	DWORD PTR [eax+4]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR -16+[ebp], eax
	jmp	SHORT $L74815
$L74814:
	mov	DWORD PTR -16+[ebp], 0
$L74815:

; 431  : }

	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?OnNcDestroy@CColourPopup@@IAEXXZ ENDP			; CColourPopup::OnNcDestroy
_TEXT	ENDS
PUBLIC	?DrawCell@CColourPopup@@IAEXPAVCDC@@H@Z		; CColourPopup::DrawCell
EXTRN	??0CPaintDC@@QAE@PAVCWnd@@@Z:NEAR		; CPaintDC::CPaintDC
EXTRN	??1CPaintDC@@UAE@XZ:NEAR			; CPaintDC::~CPaintDC
EXTRN	?GetClientRect@CWnd@@QBEXPAUtagRECT@@@Z:NEAR	; CWnd::GetClientRect
EXTRN	??BCRect@@QAEPAUtagRECT@@XZ:NEAR		; CRect::operator tagRECT *
EXTRN	?DrawEdge@CDC@@QAEHPAUtagRECT@@II@Z:NEAR	; CDC::DrawEdge
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\ColourPopup.cpp
xdata$x	SEGMENT
$T74820	DD	019930520H
	DD	01H
	DD	FLAT:$T74822
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74822	DD	0ffffffffH
	DD	FLAT:$L74818
xdata$x	ENDS
;	COMDAT ?OnPaint@CColourPopup@@IAEXXZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -120
_dc$ = -116
_i$ = -32
_rect$ = -28
?OnPaint@CColourPopup@@IAEXXZ PROC NEAR			; CColourPopup::OnPaint, COMDAT

; 434  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74821
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 108				; 0000006cH
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-120]
	mov	ecx, 27					; 0000001bH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 435  :     CPaintDC dc(this); // device context for painting

	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??0CPaintDC@@QAE@PAVCWnd@@@Z		; CPaintDC::CPaintDC
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 436  : 
; 437  :     // Draw the Default Area text
; 438  :     if (m_strDefaultText.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74288

; 439  :         DrawCell(&dc, DEFAULT_BOX_VALUE);

	push	-3					; fffffffdH
	lea	ecx, DWORD PTR _dc$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DrawCell@CColourPopup@@IAEXPAVCDC@@H@Z	; CColourPopup::DrawCell
$L74288:

; 440  :  
; 441  :     // Draw colour cells
; 442  :     for (int i = 0; i < m_nNumColours; i++)

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L74290
$L74291:
	mov	edx, DWORD PTR _i$[ebp]
	add	edx, 1
	mov	DWORD PTR _i$[ebp], edx
$L74290:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _i$[ebp]
	cmp	ecx, DWORD PTR [eax+100]
	jge	SHORT $L74292

; 443  :         DrawCell(&dc, i);

	mov	edx, DWORD PTR _i$[ebp]
	push	edx
	lea	eax, DWORD PTR _dc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DrawCell@CColourPopup@@IAEXPAVCDC@@H@Z	; CColourPopup::DrawCell
	jmp	SHORT $L74291
$L74292:

; 444  :     
; 445  :     // Draw custom text
; 446  :     if (m_strCustomText.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74293

; 447  :         DrawCell(&dc, CUSTOM_BOX_VALUE);

	push	-2					; fffffffeH
	lea	ecx, DWORD PTR _dc$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DrawCell@CColourPopup@@IAEXPAVCDC@@H@Z	; CColourPopup::DrawCell
$L74293:

; 448  : 
; 449  :     // Draw raised window edge (ex-window style WS_EX_WINDOWEDGE is sposed to do this,
; 450  :     // but for some reason isn't
; 451  :     CRect rect;

	lea	ecx, DWORD PTR _rect$[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 452  :     GetClientRect(rect);

	lea	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetClientRect@CWnd@@QBEXPAUtagRECT@@@Z	; CWnd::GetClientRect

; 453  :     dc.DrawEdge(rect, EDGE_RAISED, BF_RECT);

	push	15					; 0000000fH
	push	5
	lea	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	lea	ecx, DWORD PTR _dc$[ebp]
	call	?DrawEdge@CDC@@QAEHPAUtagRECT@@II@Z	; CDC::DrawEdge

; 454  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??1CPaintDC@@UAE@XZ			; CPaintDC::~CPaintDC
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 120				; 00000078H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74818:
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??1CPaintDC@@UAE@XZ			; CPaintDC::~CPaintDC
	ret	0
$L74821:
	mov	eax, OFFSET FLAT:$T74820
	jmp	___CxxFrameHandler
text$x	ENDS
?OnPaint@CColourPopup@@IAEXXZ ENDP			; CColourPopup::OnPaint
EXTRN	?OnMouseMove@CWnd@@IAEXIVCPoint@@@Z:NEAR	; CWnd::OnMouseMove
EXTRN	?Height@CRect@@QBEHXZ:NEAR			; CRect::Height
EXTRN	?PtInRect@CRect@@QBEHUtagPOINT@@@Z:NEAR		; CRect::PtInRect
;	COMDAT ?OnMouseMove@CColourPopup@@IAEXIVCPoint@@@Z
_TEXT	SEGMENT
$T74827 = -12
$T74828 = -20
_nFlags$ = 8
_point$ = 12
_this$ = -24
_nNewSelection$ = -4
?OnMouseMove@CColourPopup@@IAEXIVCPoint@@@Z PROC NEAR	; CColourPopup::OnMouseMove, COMDAT

; 457  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 458  :     int nNewSelection = INVALID_COLOUR;

	mov	DWORD PTR _nNewSelection$[ebp], -1

; 459  : 
; 460  :     // Translate points to be relative raised window edge
; 461  :     point.x -= m_nMargin;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _point$[ebp]
	sub	ecx, DWORD PTR [eax+116]
	mov	DWORD PTR _point$[ebp], ecx

; 462  :     point.y -= m_nMargin;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _point$[ebp+4]
	sub	eax, DWORD PTR [edx+116]
	mov	DWORD PTR _point$[ebp+4], eax

; 463  : 
; 464  :     // First check we aren't in text box
; 465  :     if (m_strCustomText.GetLength() && m_CustomTextRect.PtInRect(point))

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74302
	mov	ecx, DWORD PTR _point$[ebp]
	mov	DWORD PTR $T74827[ebp], ecx
	mov	edx, DWORD PTR _point$[ebp+4]
	mov	DWORD PTR $T74827[ebp+4], edx
	mov	eax, DWORD PTR $T74827[ebp+4]
	push	eax
	mov	ecx, DWORD PTR $T74827[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 136				; 00000088H
	call	?PtInRect@CRect@@QBEHUtagPOINT@@@Z	; CRect::PtInRect
	test	eax, eax
	je	SHORT $L74302

; 466  :         nNewSelection = CUSTOM_BOX_VALUE;

	mov	DWORD PTR _nNewSelection$[ebp], -2	; fffffffeH

; 467  :     else if (m_strDefaultText.GetLength() && m_DefaultTextRect.PtInRect(point))

	jmp	$L74308
$L74302:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74305
	mov	edx, DWORD PTR _point$[ebp]
	mov	DWORD PTR $T74828[ebp], edx
	mov	eax, DWORD PTR _point$[ebp+4]
	mov	DWORD PTR $T74828[ebp+4], eax
	mov	ecx, DWORD PTR $T74828[ebp+4]
	push	ecx
	mov	edx, DWORD PTR $T74828[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 152				; 00000098H
	call	?PtInRect@CRect@@QBEHUtagPOINT@@@Z	; CRect::PtInRect
	test	eax, eax
	je	SHORT $L74305

; 468  :         nNewSelection = DEFAULT_BOX_VALUE;

	mov	DWORD PTR _nNewSelection$[ebp], -3	; fffffffdH

; 469  :     else

	jmp	SHORT $L74308
$L74305:

; 471  :         // Take into account text box
; 472  :         if (m_strDefaultText.GetLength()) 

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74307

; 473  :             point.y -= m_DefaultTextRect.Height();  

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 152				; 00000098H
	call	?Height@CRect@@QBEHXZ			; CRect::Height
	mov	ecx, DWORD PTR _point$[ebp+4]
	sub	ecx, eax
	mov	DWORD PTR _point$[ebp+4], ecx
$L74307:

; 474  : 
; 475  :         // Get the row and column
; 476  :         nNewSelection = GetIndex(point.y / m_nBoxSize, point.x / m_nBoxSize);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _point$[ebp]
	cdq
	idiv	DWORD PTR [ecx+112]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _point$[ebp+4]
	cdq
	idiv	DWORD PTR [ecx+112]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetIndex@CColourPopup@@IBEHHH@Z	; CColourPopup::GetIndex
	mov	DWORD PTR _nNewSelection$[ebp], eax

; 477  : 
; 478  :         // In range? If not, default and exit
; 479  :         if (nNewSelection < 0 || nNewSelection >= m_nNumColours)

	cmp	DWORD PTR _nNewSelection$[ebp], 0
	jl	SHORT $L74309
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nNewSelection$[ebp]
	cmp	eax, DWORD PTR [edx+100]
	jl	SHORT $L74308
$L74309:

; 481  :             CWnd::OnMouseMove(nFlags, point);

	mov	ecx, DWORD PTR _point$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _point$[ebp]
	push	edx
	mov	eax, DWORD PTR _nFlags$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnMouseMove@CWnd@@IAEXIVCPoint@@@Z	; CWnd::OnMouseMove

; 482  :             return;

	jmp	SHORT $L74299
$L74308:

; 485  : 
; 486  :     // OK - we have the row and column of the current selection (may be CUSTOM_BOX_VALUE)
; 487  :     // Has the row/col selection changed? If yes, then redraw old and new cells.
; 488  :     if (nNewSelection != m_nCurrentSel)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nNewSelection$[ebp]
	cmp	edx, DWORD PTR [ecx+120]
	je	SHORT $L74310

; 489  :         ChangeSelection(nNewSelection);

	mov	eax, DWORD PTR _nNewSelection$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ChangeSelection@CColourPopup@@IAEXH@Z	; CColourPopup::ChangeSelection
$L74310:

; 490  : 
; 491  :     CWnd::OnMouseMove(nFlags, point);

	mov	ecx, DWORD PTR _point$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _point$[ebp]
	push	edx
	mov	eax, DWORD PTR _nFlags$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnMouseMove@CWnd@@IAEXIVCPoint@@@Z	; CWnd::OnMouseMove
$L74299:

; 492  : }

	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?OnMouseMove@CColourPopup@@IAEXIVCPoint@@@Z ENDP	; CColourPopup::OnMouseMove
_TEXT	ENDS
EXTRN	__imp__GetMessagePos@0:NEAR
EXTRN	??0CPoint@@QAE@HH@Z:NEAR			; CPoint::CPoint
EXTRN	?OnLButtonUp@CWnd@@IAEXIVCPoint@@@Z:NEAR	; CWnd::OnLButtonUp
;	COMDAT ?OnLButtonUp@CColourPopup@@IAEXIVCPoint@@@Z
_TEXT	SEGMENT
$T74831 = -12
$T74832 = -20
_nFlags$ = 8
_point$ = 12
_this$ = -24
_pos$ = -4
?OnLButtonUp@CColourPopup@@IAEXIVCPoint@@@Z PROC NEAR	; CColourPopup::OnLButtonUp, COMDAT

; 496  : {    

	push	ebp
	mov	ebp, esp
	sub	esp, 24					; 00000018H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 497  :     CWnd::OnLButtonUp(nFlags, point);

	mov	eax, DWORD PTR _point$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _point$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nFlags$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnLButtonUp@CWnd@@IAEXIVCPoint@@@Z	; CWnd::OnLButtonUp

; 498  : 
; 499  : 	if (m_bIgnoreFirstLBtnUp)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+308], 0
	je	SHORT $L74316

; 501  : 		m_bIgnoreFirstLBtnUp = FALSE;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+308], 0

; 502  : 		return;

	jmp	SHORT $L74315
$L74316:

; 504  : 
; 505  :     DWORD pos = GetMessagePos();

	mov	esi, esp
	call	DWORD PTR __imp__GetMessagePos@0
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pos$[ebp], eax

; 506  :     point = CPoint((short)LOWORD(pos), (short)HIWORD(pos));

	mov	edx, DWORD PTR _pos$[ebp]
	shr	edx, 16					; 00000010H
	and	edx, 65535				; 0000ffffH
	movsx	eax, dx
	push	eax
	movsx	ecx, WORD PTR _pos$[ebp]
	push	ecx
	lea	ecx, DWORD PTR $T74831[ebp]
	call	??0CPoint@@QAE@HH@Z			; CPoint::CPoint
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR [eax+4]
	mov	DWORD PTR _point$[ebp], edx
	mov	DWORD PTR _point$[ebp+4], eax

; 507  : 
; 508  :     if (m_WindowRect.PtInRect(point))

	mov	ecx, DWORD PTR _point$[ebp]
	mov	DWORD PTR $T74832[ebp], ecx
	mov	edx, DWORD PTR _point$[ebp+4]
	mov	DWORD PTR $T74832[ebp+4], edx
	mov	eax, DWORD PTR $T74832[ebp+4]
	push	eax
	mov	ecx, DWORD PTR $T74832[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?PtInRect@CRect@@QBEHUtagPOINT@@@Z	; CRect::PtInRect
	test	eax, eax
	je	SHORT $L74325

; 509  :         EndSelection(CPN_SELENDOK);

	push	2028					; 000007ecH
	mov	ecx, DWORD PTR _this$[ebp]
	call	?EndSelection@CColourPopup@@IAEXH@Z	; CColourPopup::EndSelection

; 510  :     else

	jmp	SHORT $L74326
$L74325:

; 511  :         EndSelection(CPN_SELENDCANCEL);

	push	2029					; 000007edH
	mov	ecx, DWORD PTR _this$[ebp]
	call	?EndSelection@CColourPopup@@IAEXH@Z	; CColourPopup::EndSelection
$L74326:
$L74315:

; 512  : }

	pop	esi
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?OnLButtonUp@CColourPopup@@IAEXIVCPoint@@@Z ENDP	; CColourPopup::OnLButtonUp
_TEXT	ENDS
;	COMDAT ?GetIndex@CColourPopup@@IBEHHH@Z
_TEXT	SEGMENT
_row$ = 8
_col$ = 12
_this$ = -4
?GetIndex@CColourPopup@@IBEHHH@Z PROC NEAR		; CColourPopup::GetIndex, COMDAT

; 518  : { 

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 519  :     if ((row == CUSTOM_BOX_VALUE || col == CUSTOM_BOX_VALUE) && m_strCustomText.GetLength())

	cmp	DWORD PTR _row$[ebp], -2		; fffffffeH
	je	SHORT $L74333
	cmp	DWORD PTR _col$[ebp], -2		; fffffffeH
	jne	SHORT $L74332
$L74333:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74332

; 520  :         return CUSTOM_BOX_VALUE;

	mov	eax, -2					; fffffffeH
	jmp	SHORT $L74342
$L74332:

; 521  :     else if ((row == DEFAULT_BOX_VALUE || col == DEFAULT_BOX_VALUE) && m_strDefaultText.GetLength())

	cmp	DWORD PTR _row$[ebp], -3		; fffffffdH
	je	SHORT $L74336
	cmp	DWORD PTR _col$[ebp], -3		; fffffffdH
	jne	SHORT $L74335
$L74336:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74335

; 522  :         return DEFAULT_BOX_VALUE;

	mov	eax, -3					; fffffffdH
	jmp	SHORT $L74342
$L74335:

; 523  :     else if (row < 0 || col < 0 || row >= m_nNumRows || col >= m_nNumColumns)

	cmp	DWORD PTR _row$[ebp], 0
	jl	SHORT $L74339
	cmp	DWORD PTR _col$[ebp], 0
	jl	SHORT $L74339
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _row$[ebp]
	cmp	ecx, DWORD PTR [eax+108]
	jge	SHORT $L74339
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _col$[ebp]
	cmp	eax, DWORD PTR [edx+104]
	jl	SHORT $L74338
$L74339:

; 524  :         return INVALID_COLOUR;

	or	eax, -1
	jmp	SHORT $L74342
$L74338:

; 527  :         if (row*m_nNumColumns + col >= m_nNumColours)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _row$[ebp]
	imul	edx, DWORD PTR [ecx+104]
	add	edx, DWORD PTR _col$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	cmp	edx, DWORD PTR [eax+100]
	jl	SHORT $L74341

; 528  :             return INVALID_COLOUR;

	or	eax, -1
	jmp	SHORT $L74342
$L74341:

; 530  :             return row*m_nNumColumns + col;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _row$[ebp]
	imul	eax, DWORD PTR [ecx+104]
	add	eax, DWORD PTR _col$[ebp]
$L74342:

; 532  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?GetIndex@CColourPopup@@IBEHHH@Z ENDP			; CColourPopup::GetIndex
_TEXT	ENDS
;	COMDAT ?GetRow@CColourPopup@@IBEHH@Z
_TEXT	SEGMENT
_nIndex$ = 8
_this$ = -4
?GetRow@CColourPopup@@IBEHH@Z PROC NEAR			; CColourPopup::GetRow, COMDAT

; 535  : { 

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 536  :     if (nIndex == CUSTOM_BOX_VALUE && m_strCustomText.GetLength())

	cmp	DWORD PTR _nIndex$[ebp], -2		; fffffffeH
	jne	SHORT $L74347
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74347

; 537  :         return CUSTOM_BOX_VALUE;

	mov	eax, -2					; fffffffeH
	jmp	SHORT $L74353
$L74347:

; 538  :     else if (nIndex == DEFAULT_BOX_VALUE && m_strDefaultText.GetLength())

	cmp	DWORD PTR _nIndex$[ebp], -3		; fffffffdH
	jne	SHORT $L74349
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74349

; 539  :         return DEFAULT_BOX_VALUE;

	mov	eax, -3					; fffffffdH
	jmp	SHORT $L74353
$L74349:

; 540  :     else if (nIndex < 0 || nIndex >= m_nNumColours)

	cmp	DWORD PTR _nIndex$[ebp], 0
	jl	SHORT $L74352
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nIndex$[ebp]
	cmp	ecx, DWORD PTR [eax+100]
	jl	SHORT $L74351
$L74352:

; 541  :         return INVALID_COLOUR;

	or	eax, -1
	jmp	SHORT $L74353
$L74351:

; 543  :         return nIndex / m_nNumColumns; 

	mov	ecx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nIndex$[ebp]
	cdq
	idiv	DWORD PTR [ecx+104]
$L74353:

; 544  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetRow@CColourPopup@@IBEHH@Z ENDP			; CColourPopup::GetRow
_TEXT	ENDS
;	COMDAT ?GetColumn@CColourPopup@@IBEHH@Z
_TEXT	SEGMENT
_nIndex$ = 8
_this$ = -4
?GetColumn@CColourPopup@@IBEHH@Z PROC NEAR		; CColourPopup::GetColumn, COMDAT

; 547  : { 

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 548  :     if (nIndex == CUSTOM_BOX_VALUE && m_strCustomText.GetLength())

	cmp	DWORD PTR _nIndex$[ebp], -2		; fffffffeH
	jne	SHORT $L74358
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74358

; 549  :         return CUSTOM_BOX_VALUE;

	mov	eax, -2					; fffffffeH
	jmp	SHORT $L74364
$L74358:

; 550  :     else if (nIndex == DEFAULT_BOX_VALUE && m_strDefaultText.GetLength())

	cmp	DWORD PTR _nIndex$[ebp], -3		; fffffffdH
	jne	SHORT $L74360
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74360

; 551  :         return DEFAULT_BOX_VALUE;

	mov	eax, -3					; fffffffdH
	jmp	SHORT $L74364
$L74360:

; 552  :     else if (nIndex < 0 || nIndex >= m_nNumColours)

	cmp	DWORD PTR _nIndex$[ebp], 0
	jl	SHORT $L74363
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nIndex$[ebp]
	cmp	ecx, DWORD PTR [eax+100]
	jl	SHORT $L74362
$L74363:

; 553  :         return INVALID_COLOUR;

	or	eax, -1
	jmp	SHORT $L74364
$L74362:

; 555  :         return nIndex % m_nNumColumns; 

	mov	ecx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nIndex$[ebp]
	cdq
	idiv	DWORD PTR [ecx+104]
	mov	eax, edx
$L74364:

; 556  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetColumn@CColourPopup@@IBEHH@Z ENDP			; CColourPopup::GetColumn
_TEXT	ENDS
PUBLIC	?GetColour@CColourPopup@@IAEKH@Z		; CColourPopup::GetColour
;	COMDAT ?FindCellFromColour@CColourPopup@@IAEXK@Z
_TEXT	SEGMENT
_crColour$ = 8
_this$ = -8
_i$ = -4
?FindCellFromColour@CColourPopup@@IAEXK@Z PROC NEAR	; CColourPopup::FindCellFromColour, COMDAT

; 559  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 560  :     if (crColour == CLR_DEFAULT && m_strDefaultText.GetLength())

	cmp	DWORD PTR _crColour$[ebp], -16777216	; ff000000H
	jne	SHORT $L74369
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74369

; 562  :         m_nChosenColourSel = DEFAULT_BOX_VALUE;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+124], -3			; fffffffdH

; 563  :         return;

	jmp	SHORT $L74368
$L74369:

; 565  : 
; 566  :     for (int i = 0; i < m_nNumColours; i++)

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L74371
$L74372:
	mov	ecx, DWORD PTR _i$[ebp]
	add	ecx, 1
	mov	DWORD PTR _i$[ebp], ecx
$L74371:
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _i$[ebp]
	cmp	eax, DWORD PTR [edx+100]
	jge	SHORT $L74373

; 568  :         if (GetColour(i) == crColour)

	mov	ecx, DWORD PTR _i$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetColour@CColourPopup@@IAEKH@Z	; CColourPopup::GetColour
	cmp	eax, DWORD PTR _crColour$[ebp]
	jne	SHORT $L74374

; 570  :             m_nChosenColourSel = i;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _i$[ebp]
	mov	DWORD PTR [edx+124], eax

; 571  :             return;

	jmp	SHORT $L74368
$L74374:

; 573  :     }

	jmp	SHORT $L74372
$L74373:

; 574  : 
; 575  :     if (m_strCustomText.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74375

; 576  :         m_nChosenColourSel = CUSTOM_BOX_VALUE;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+124], -2			; fffffffeH

; 577  :     else

	jmp	SHORT $L74376
$L74375:

; 578  :         m_nChosenColourSel = INVALID_COLOUR;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+124], -1
$L74376:
$L74368:

; 579  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?FindCellFromColour@CColourPopup@@IAEXK@Z ENDP		; CColourPopup::FindCellFromColour
_TEXT	ENDS
;	COMDAT ?GetColour@CColourPopup@@IAEKH@Z
_TEXT	SEGMENT
_this$ = -4
_nIndex$ = 8
?GetColour@CColourPopup@@IAEKH@Z PROC NEAR		; CColourPopup::GetColour, COMDAT

; 87   :     COLORREF GetColour(int nIndex)              { return m_crColours[nIndex].crColour; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _nIndex$[ebp]
	mov	eax, DWORD PTR ?m_crColours@CColourPopup@@1PAUColourTableEntry@@A[eax*8]
	mov	esp, ebp
	pop	ebp
	ret	4
?GetColour@CColourPopup@@IAEKH@Z ENDP			; CColourPopup::GetColour
_TEXT	ENDS
PUBLIC	?GetCellRect@CColourPopup@@IAEHHABQAUtagRECT@@@Z ; CColourPopup::GetCellRect
EXTRN	__imp__SetRect@20:NEAR
;	COMDAT ?GetCellRect@CColourPopup@@IAEHHABQAUtagRECT@@@Z
_TEXT	SEGMENT
_nIndex$ = 8
_rect$ = 12
_this$ = -4
?GetCellRect@CColourPopup@@IAEHHABQAUtagRECT@@@Z PROC NEAR ; CColourPopup::GetCellRect, COMDAT

; 583  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 584  :     if (nIndex == CUSTOM_BOX_VALUE)

	cmp	DWORD PTR _nIndex$[ebp], -2		; fffffffeH
	jne	SHORT $L74382

; 586  :         ::SetRect(rect, 
; 587  :                   m_CustomTextRect.left,  m_CustomTextRect.top,
; 588  :                   m_CustomTextRect.right, m_CustomTextRect.bottom);

	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+148]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+144]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+140]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+136]
	push	ecx
	mov	edx, DWORD PTR _rect$[ebp]
	mov	eax, DWORD PTR [edx]
	push	eax
	call	DWORD PTR __imp__SetRect@20
	cmp	esi, esp
	call	__chkesp

; 589  :         return TRUE;

	mov	eax, 1
	jmp	$L74381
$L74382:

; 591  :     else if (nIndex == DEFAULT_BOX_VALUE)

	cmp	DWORD PTR _nIndex$[ebp], -3		; fffffffdH
	jne	SHORT $L74384

; 593  :         ::SetRect(rect, 
; 594  :                   m_DefaultTextRect.left,  m_DefaultTextRect.top,
; 595  :                   m_DefaultTextRect.right, m_DefaultTextRect.bottom);

	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+164]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+160]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+156]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+152]
	push	edx
	mov	eax, DWORD PTR _rect$[ebp]
	mov	ecx, DWORD PTR [eax]
	push	ecx
	call	DWORD PTR __imp__SetRect@20
	cmp	esi, esp
	call	__chkesp

; 596  :         return TRUE;

	mov	eax, 1
	jmp	$L74381
$L74384:

; 598  : 
; 599  :     if (nIndex < 0 || nIndex >= m_nNumColours)

	cmp	DWORD PTR _nIndex$[ebp], 0
	jl	SHORT $L74386
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nIndex$[ebp]
	cmp	eax, DWORD PTR [edx+100]
	jl	SHORT $L74385
$L74386:

; 600  :         return FALSE;

	xor	eax, eax
	jmp	$L74381
$L74385:

; 601  : 
; 602  :     rect->left = GetColumn(nIndex) * m_nBoxSize + m_nMargin;

	mov	ecx, DWORD PTR _nIndex$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetColumn@CColourPopup@@IBEHH@Z	; CColourPopup::GetColumn
	mov	edx, DWORD PTR _this$[ebp]
	imul	eax, DWORD PTR [edx+112]
	mov	ecx, DWORD PTR _this$[ebp]
	add	eax, DWORD PTR [ecx+116]
	mov	edx, DWORD PTR _rect$[ebp]
	mov	ecx, DWORD PTR [edx]
	mov	DWORD PTR [ecx], eax

; 603  :     rect->top  = GetRow(nIndex) * m_nBoxSize + m_nMargin;

	mov	edx, DWORD PTR _nIndex$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetRow@CColourPopup@@IBEHH@Z		; CColourPopup::GetRow
	mov	ecx, DWORD PTR _this$[ebp]
	imul	eax, DWORD PTR [ecx+112]
	mov	edx, DWORD PTR _this$[ebp]
	add	eax, DWORD PTR [edx+116]
	mov	ecx, DWORD PTR _rect$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR [edx+4], eax

; 604  : 
; 605  :     // Move everything down if we are displaying a default text area
; 606  :     if (m_strDefaultText.GetLength()) 

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74387

; 607  :         rect->top += (m_nMargin + m_DefaultTextRect.Height());

	mov	eax, DWORD PTR _rect$[ebp]
	mov	esi, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 152				; 00000098H
	call	?Height@CRect@@QBEHXZ			; CRect::Height
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+116]
	add	edx, eax
	mov	eax, DWORD PTR [esi+4]
	add	eax, edx
	mov	ecx, DWORD PTR _rect$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR [edx+4], eax
$L74387:

; 608  : 
; 609  :     rect->right = rect->left + m_nBoxSize;

	mov	eax, DWORD PTR _rect$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [ecx]
	mov	eax, DWORD PTR _this$[ebp]
	add	edx, DWORD PTR [eax+112]
	mov	ecx, DWORD PTR _rect$[ebp]
	mov	eax, DWORD PTR [ecx]
	mov	DWORD PTR [eax+8], edx

; 610  :     rect->bottom = rect->top + m_nBoxSize;

	mov	ecx, DWORD PTR _rect$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _this$[ebp]
	add	eax, DWORD PTR [ecx+112]
	mov	edx, DWORD PTR _rect$[ebp]
	mov	ecx, DWORD PTR [edx]
	mov	DWORD PTR [ecx+12], eax

; 611  : 
; 612  :     return TRUE;

	mov	eax, 1
$L74381:

; 613  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?GetCellRect@CColourPopup@@IAEHHABQAUtagRECT@@@Z ENDP	; CColourPopup::GetCellRect
_TEXT	ENDS
EXTRN	_xMonitorFromRect@8:NEAR
EXTRN	_xGetMonitorInfo@8:NEAR
EXTRN	??0CClientDC@@QAE@PAVCWnd@@@Z:NEAR		; CClientDC::CClientDC
EXTRN	??1CClientDC@@UAE@XZ:NEAR			; CClientDC::~CClientDC
EXTRN	?MoveWindow@CWnd@@QAEXPBUtagRECT@@H@Z:NEAR	; CWnd::MoveWindow
EXTRN	?GetWindowRect@CWnd@@QBEXPAUtagRECT@@@Z:NEAR	; CWnd::GetWindowRect
EXTRN	?SelectObject@CDC@@UAEPAVCFont@@PAV2@@Z:NEAR	; CDC::SelectObject
EXTRN	??0CSize@@QAE@XZ:NEAR				; CSize::CSize
EXTRN	??0CSize@@QAE@HH@Z:NEAR				; CSize::CSize
EXTRN	??YCSize@@QAEXUtagSIZE@@@Z:NEAR			; CSize::operator+=
EXTRN	?Width@CRect@@QBEHXZ:NEAR			; CRect::Width
EXTRN	?SetRect@CRect@@QAEXHHHH@Z:NEAR			; CRect::SetRect
EXTRN	?OffsetRect@CRect@@QAEXHH@Z:NEAR		; CRect::OffsetRect
EXTRN	??4CRect@@QAEXABUtagRECT@@@Z:NEAR		; CRect::operator=
EXTRN	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z:NEAR ; CDC::GetTextExtent
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\ColourPopup.cpp
xdata$x	SEGMENT
$T74853	DD	019930520H
	DD	01H
	DD	FLAT:$T74855
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74855	DD	0ffffffffH
	DD	FLAT:$L74851
xdata$x	ENDS
;	COMDAT ?SetWindowSize@CColourPopup@@IAEXXZ
_TEXT	SEGMENT
$T74847 = -152
$T74848 = -160
$T74849 = -168
$T74850 = -176
__$EHRec$ = -12
_this$ = -180
_TextSize$ = -96
_dc$74394 = -120
_pOldFont$74395 = -100
_DefaultSize$74401 = -128
_rect$ = -28
_rWorkArea$ = -84
_hMon$ = -88
_mi$ = -68
_ParentRect$74423 = -144
?SetWindowSize@CColourPopup@@IAEXXZ PROC NEAR		; CColourPopup::SetWindowSize, COMDAT

; 617  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74854
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 168				; 000000a8H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-180]
	mov	ecx, 42					; 0000002aH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 618  :     CSize TextSize;

	lea	ecx, DWORD PTR _TextSize$[ebp]
	call	??0CSize@@QAE@XZ			; CSize::CSize

; 619  : 
; 620  :     // If we are showing a custom or default text area, get the font and text size.
; 621  :     if (m_strCustomText.GetLength() || m_strDefaultText.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	jne	SHORT $L74393
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	$L74392
$L74393:

; 623  :         CClientDC dc(this);

	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	lea	ecx, DWORD PTR _dc$74394[ebp]
	call	??0CClientDC@@QAE@PAVCWnd@@@Z		; CClientDC::CClientDC
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 624  :         CFont* pOldFont = (CFont*) dc.SelectObject(&m_Font);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 184				; 000000b8H
	push	ecx
	lea	ecx, DWORD PTR _dc$74394[ebp]
	call	?SelectObject@CDC@@UAEPAVCFont@@PAV2@@Z	; CDC::SelectObject
	mov	DWORD PTR _pOldFont$74395[ebp], eax

; 625  : 
; 626  :         // Get the size of the custom text (if there IS custom text)
; 627  :         TextSize = CSize(0,0);

	push	0
	push	0
	lea	ecx, DWORD PTR $T74847[ebp]
	call	??0CSize@@QAE@HH@Z			; CSize::CSize
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR [eax+4]
	mov	DWORD PTR _TextSize$[ebp], edx
	mov	DWORD PTR _TextSize$[ebp+4], eax

; 628  :         if (m_strCustomText.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74398

; 629  :             TextSize = dc.GetTextExtent(m_strCustomText);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	push	ecx
	lea	edx, DWORD PTR $T74848[ebp]
	push	edx
	lea	ecx, DWORD PTR _dc$74394[ebp]
	call	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z ; CDC::GetTextExtent
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR _TextSize$[ebp], ecx
	mov	DWORD PTR _TextSize$[ebp+4], edx
$L74398:

; 630  : 
; 631  :         // Get the size of the default text (if there IS default text)
; 632  :         if (m_strDefaultText.GetLength())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	SHORT $L74404

; 634  :             CSize DefaultSize = dc.GetTextExtent(m_strDefaultText);

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 128				; 00000080H
	push	eax
	lea	ecx, DWORD PTR _DefaultSize$74401[ebp]
	push	ecx
	lea	ecx, DWORD PTR _dc$74394[ebp]
	call	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z ; CDC::GetTextExtent

; 635  :             if (DefaultSize.cx > TextSize.cx) TextSize.cx = DefaultSize.cx;

	mov	edx, DWORD PTR _DefaultSize$74401[ebp]
	cmp	edx, DWORD PTR _TextSize$[ebp]
	jle	SHORT $L74403
	mov	eax, DWORD PTR _DefaultSize$74401[ebp]
	mov	DWORD PTR _TextSize$[ebp], eax
$L74403:

; 636  :             if (DefaultSize.cy > TextSize.cy) TextSize.cy = DefaultSize.cy;

	mov	ecx, DWORD PTR _DefaultSize$74401[ebp+4]
	cmp	ecx, DWORD PTR _TextSize$[ebp+4]
	jle	SHORT $L74404
	mov	edx, DWORD PTR _DefaultSize$74401[ebp+4]
	mov	DWORD PTR _TextSize$[ebp+4], edx
$L74404:

; 638  : 
; 639  :         dc.SelectObject(pOldFont);

	mov	eax, DWORD PTR _pOldFont$74395[ebp]
	push	eax
	lea	ecx, DWORD PTR _dc$74394[ebp]
	call	?SelectObject@CDC@@UAEPAVCFont@@PAV2@@Z	; CDC::SelectObject

; 640  :         TextSize += CSize(2*m_nMargin,2*m_nMargin);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+116]
	shl	edx, 1
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+116]
	shl	ecx, 1
	push	ecx
	lea	ecx, DWORD PTR $T74850[ebp]
	call	??0CSize@@QAE@HH@Z			; CSize::CSize
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR [eax+4]
	mov	DWORD PTR $T74849[ebp], edx
	mov	DWORD PTR $T74849[ebp+4], eax
	mov	ecx, DWORD PTR $T74849[ebp+4]
	push	ecx
	mov	edx, DWORD PTR $T74849[ebp]
	push	edx
	lea	ecx, DWORD PTR _TextSize$[ebp]
	call	??YCSize@@QAEXUtagSIZE@@@Z		; CSize::operator+=

; 641  : 
; 642  :         // Add even more space to draw the horizontal line
; 643  :         TextSize.cy += 2*m_nMargin + 2;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+116]
	mov	edx, DWORD PTR _TextSize$[ebp+4]
	lea	eax, DWORD PTR [edx+ecx*2+2]
	mov	DWORD PTR _TextSize$[ebp+4], eax

; 644  :     }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _dc$74394[ebp]
	call	??1CClientDC@@UAE@XZ			; CClientDC::~CClientDC
$L74392:

; 645  : 
; 646  :     // Get the number of columns and rows
; 647  :     //m_nNumColumns = (int) sqrt((double)m_nNumColours);    // for a square window (yuk)
; 648  :     m_nNumColumns = 8;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+104], 8

; 649  :     m_nNumRows = m_nNumColours / m_nNumColumns;

	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+100]
	cdq
	idiv	DWORD PTR [ecx+104]
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+108], eax

; 650  :     if (m_nNumColours % m_nNumColumns) m_nNumRows++;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+100]
	cdq
	idiv	DWORD PTR [ecx+104]
	test	edx, edx
	je	SHORT $L74407
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+108]
	add	eax, 1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+108], eax
$L74407:

; 651  : 
; 652  :     // Get the current window position, and set the new size
; 653  :     CRect rect;

	lea	ecx, DWORD PTR _rect$[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 654  :     GetWindowRect(rect);

	lea	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetWindowRect@CWnd@@QBEXPAUtagRECT@@@Z	; CWnd::GetWindowRect

; 655  : 
; 656  :     m_WindowRect.SetRect(rect.left, rect.top, 
; 657  :                          rect.left + m_nNumColumns*m_nBoxSize + 2*m_nMargin,
; 658  :                          rect.top  + m_nNumRows*m_nBoxSize + 2*m_nMargin);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+108]
	imul	ecx, DWORD PTR [eax+112]
	mov	edx, DWORD PTR _rect$[ebp+4]
	add	edx, ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+116]
	lea	edx, DWORD PTR [edx+ecx*2]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax+104]
	imul	edx, DWORD PTR [ecx+112]
	mov	eax, DWORD PTR _rect$[ebp]
	add	eax, edx
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+116]
	lea	eax, DWORD PTR [eax+edx*2]
	push	eax
	mov	ecx, DWORD PTR _rect$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _rect$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?SetRect@CRect@@QAEXHHHH@Z		; CRect::SetRect

; 659  : 
; 660  :     // if custom text, then expand window if necessary, and set text width as
; 661  :     // window width
; 662  :     if (m_strDefaultText.GetLength()) 

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	$L74409

; 664  :         if (TextSize.cx > m_WindowRect.Width())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?Width@CRect@@QBEHXZ			; CRect::Width
	cmp	DWORD PTR _TextSize$[ebp], eax
	jle	SHORT $L74410

; 665  :             m_WindowRect.right = m_WindowRect.left + TextSize.cx;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+168]
	add	ecx, DWORD PTR _TextSize$[ebp]
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+176], ecx
$L74410:

; 666  :         TextSize.cx = m_WindowRect.Width()-2*m_nMargin;

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?Width@CRect@@QBEHXZ			; CRect::Width
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+116]
	shl	edx, 1
	sub	eax, edx
	mov	DWORD PTR _TextSize$[ebp], eax

; 667  : 
; 668  :         // Work out the text area
; 669  :         m_DefaultTextRect.SetRect(m_nMargin, m_nMargin, 
; 670  :                                   m_nMargin+TextSize.cx, 2*m_nMargin+TextSize.cy);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+116]
	mov	edx, DWORD PTR _TextSize$[ebp+4]
	lea	eax, DWORD PTR [edx+ecx*2]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+116]
	add	edx, DWORD PTR _TextSize$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+116]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+116]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 152				; 00000098H
	call	?SetRect@CRect@@QAEXHHHH@Z		; CRect::SetRect

; 671  :         m_WindowRect.bottom += m_DefaultTextRect.Height() + 2*m_nMargin;

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 152				; 00000098H
	call	?Height@CRect@@QBEHXZ			; CRect::Height
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+116]
	lea	eax, DWORD PTR [eax+edx*2]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+180]
	add	edx, eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+180], edx
$L74409:

; 673  : 
; 674  :     // if custom text, then expand window if necessary, and set text width as
; 675  :     // window width
; 676  :     if (m_strCustomText.GetLength()) 

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	$L74411

; 678  :        if (TextSize.cx > m_WindowRect.Width())

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?Width@CRect@@QBEHXZ			; CRect::Width
	cmp	DWORD PTR _TextSize$[ebp], eax
	jle	SHORT $L74412

; 679  :           m_WindowRect.right = m_WindowRect.left + TextSize.cx;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+168]
	add	edx, DWORD PTR _TextSize$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+176], edx
$L74412:

; 680  :        TextSize.cx = m_WindowRect.Width()-2*m_nMargin;

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?Width@CRect@@QBEHXZ			; CRect::Width
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+116]
	shl	edx, 1
	sub	eax, edx
	mov	DWORD PTR _TextSize$[ebp], eax

; 681  :        
; 682  :        // Work out the text area
; 683  :        m_CustomTextRect.SetRect(m_nMargin, m_WindowRect.Height(), 
; 684  :           m_nMargin+TextSize.cx, 
; 685  :           m_WindowRect.Height()+m_nMargin+TextSize.cy);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?Height@CRect@@QBEHXZ			; CRect::Height
	mov	ecx, DWORD PTR _this$[ebp]
	add	eax, DWORD PTR [ecx+116]
	add	eax, DWORD PTR _TextSize$[ebp+4]
	push	eax
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+116]
	add	eax, DWORD PTR _TextSize$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?Height@CRect@@QBEHXZ			; CRect::Height
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+116]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 136				; 00000088H
	call	?SetRect@CRect@@QAEXHHHH@Z		; CRect::SetRect

; 686  :        m_WindowRect.bottom += m_CustomTextRect.Height() + 2*m_nMargin;

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 136				; 00000088H
	call	?Height@CRect@@QBEHXZ			; CRect::Height
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+116]
	lea	eax, DWORD PTR [eax+edx*2]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+180]
	add	edx, eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+180], edx
$L74411:

; 688  :     
; 689  :     // Need to check it'll fit on screen: Too far right?
; 690  :     CRect rWorkArea;

	lea	ecx, DWORD PTR _rWorkArea$[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 691  :     HMONITOR hMon = MonitorFromRect(m_WindowRect, MONITOR_DEFAULTTONEAREST);

	push	2
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	call	_xMonitorFromRect@8
	mov	DWORD PTR _hMon$[ebp], eax

; 692  :     MONITORINFO mi = { sizeof(MONITORINFO) };

	mov	DWORD PTR _mi$[ebp], 40			; 00000028H
	mov	ecx, 9
	xor	eax, eax
	lea	edi, DWORD PTR _mi$[ebp+4]
	rep stosd

; 693  : 
; 694  :     if (hMon && GetMonitorInfo(hMon, &mi))

	cmp	DWORD PTR _hMon$[ebp], 0
	je	SHORT $L74417
	lea	ecx, DWORD PTR _mi$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hMon$[ebp]
	push	edx
	call	_xGetMonitorInfo@8
	test	eax, eax
	je	SHORT $L74417

; 695  :        rWorkArea = mi.rcWork;

	lea	eax, DWORD PTR _mi$[ebp+20]
	push	eax
	lea	ecx, DWORD PTR _rWorkArea$[ebp]
	call	??4CRect@@QAEXABUtagRECT@@@Z		; CRect::operator=

; 696  :     else

	jmp	SHORT $L74418
$L74417:

; 697  :        SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID)&rWorkArea, 0);

	mov	esi, esp
	push	0
	lea	ecx, DWORD PTR _rWorkArea$[ebp]
	push	ecx
	push	0
	push	48					; 00000030H
	call	DWORD PTR __imp__SystemParametersInfoW@16
	cmp	esi, esp
	call	__chkesp
$L74418:

; 698  :     
; 699  :     if (m_WindowRect.right > rWorkArea.right)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+176]
	cmp	eax, DWORD PTR _rWorkArea$[ebp+8]
	jle	SHORT $L74420

; 700  :        m_WindowRect.OffsetRect(-(m_WindowRect.right - rWorkArea.right), 0);

	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+176]
	sub	edx, DWORD PTR _rWorkArea$[ebp+8]
	neg	edx
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?OffsetRect@CRect@@QAEXHH@Z		; CRect::OffsetRect
$L74420:

; 701  :     
; 702  :     // Too far left?
; 703  :     if (m_WindowRect.left < rWorkArea.left)

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+168]
	cmp	ecx, DWORD PTR _rWorkArea$[ebp]
	jge	SHORT $L74421

; 704  :        m_WindowRect.OffsetRect( -(m_WindowRect.left - rWorkArea.left), 0);

	push	0
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+168]
	sub	eax, DWORD PTR _rWorkArea$[ebp]
	neg	eax
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?OffsetRect@CRect@@QAEXHH@Z		; CRect::OffsetRect
$L74421:

; 705  : 
; 706  :     // Bottom falling out of screen?
; 707  :     if (m_WindowRect.bottom > rWorkArea.bottom)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+180]
	cmp	edx, DWORD PTR _rWorkArea$[ebp+12]
	jle	SHORT $L74422

; 709  :         CRect ParentRect;

	lea	ecx, DWORD PTR _ParentRect$74423[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 710  :         m_pParent->GetWindowRect(ParentRect);

	lea	ecx, DWORD PTR _ParentRect$74423[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+300]
	call	?GetWindowRect@CWnd@@QBEXPAUtagRECT@@@Z	; CWnd::GetWindowRect

; 711  :         m_WindowRect.OffsetRect(0, -(ParentRect.Height() + m_WindowRect.Height()));

	lea	ecx, DWORD PTR _ParentRect$74423[ebp]
	call	?Height@CRect@@QBEHXZ			; CRect::Height
	mov	esi, eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?Height@CRect@@QBEHXZ			; CRect::Height
	add	esi, eax
	neg	esi
	push	esi
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	?OffsetRect@CRect@@QAEXHH@Z		; CRect::OffsetRect
$L74422:

; 713  : 
; 714  :     // Set the window size and position
; 715  :     MoveWindow(m_WindowRect, TRUE);

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 168				; 000000a8H
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?MoveWindow@CWnd@@QAEXPBUtagRECT@@H@Z	; CWnd::MoveWindow

; 716  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 180				; 000000b4H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74851:
	lea	ecx, DWORD PTR _dc$74394[ebp]
	call	??1CClientDC@@UAE@XZ			; CClientDC::~CClientDC
	ret	0
$L74854:
	mov	eax, OFFSET FLAT:$T74853
	jmp	___CxxFrameHandler
text$x	ENDS
?SetWindowSize@CColourPopup@@IAEXXZ ENDP		; CColourPopup::SetWindowSize
PUBLIC	?GetColourName@CColourPopup@@IAEPBGH@Z		; CColourPopup::GetColourName
EXTRN	?Create@CToolTipCtrl@@QAEHPAVCWnd@@K@Z:NEAR	; CToolTipCtrl::Create
EXTRN	?AddTool@CToolTipCtrl@@QAEHPAVCWnd@@PBGPBUtagRECT@@I@Z:NEAR ; CToolTipCtrl::AddTool
;	COMDAT ?CreateToolTips@CColourPopup@@IAEXXZ
_TEXT	SEGMENT
$T74860 = -24
_this$ = -28
_i$ = -4
_rect$74432 = -20
?CreateToolTips@CColourPopup@@IAEXXZ PROC NEAR		; CColourPopup::CreateToolTips, COMDAT

; 719  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 28					; 0000001cH
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 720  :     // Create the tool tip
; 721  :     if (!m_ToolTip.Create(this)) return;

	push	0
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 208				; 000000d0H
	call	?Create@CToolTipCtrl@@QAEHPAVCWnd@@K@Z	; CToolTipCtrl::Create
	test	eax, eax
	jne	SHORT $L74427
	jmp	SHORT $L74426
$L74427:

; 722  : 
; 723  :     // Add a tool for each cell
; 724  :     for (int i = 0; i < m_nNumColours; i++)

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L74429
$L74430:
	mov	ecx, DWORD PTR _i$[ebp]
	add	ecx, 1
	mov	DWORD PTR _i$[ebp], ecx
$L74429:
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _i$[ebp]
	cmp	eax, DWORD PTR [edx+100]
	jge	SHORT $L74431

; 726  :         CRect rect;

	lea	ecx, DWORD PTR _rect$74432[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 727  :         if (!GetCellRect(i, rect)) continue;

	lea	ecx, DWORD PTR _rect$74432[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	mov	DWORD PTR $T74860[ebp], eax
	lea	ecx, DWORD PTR $T74860[ebp]
	push	ecx
	mov	edx, DWORD PTR _i$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCellRect@CColourPopup@@IAEHHABQAUtagRECT@@@Z ; CColourPopup::GetCellRect
	test	eax, eax
	jne	SHORT $L74434
	jmp	SHORT $L74430
$L74434:

; 728  :             m_ToolTip.AddTool(this, GetColourName(i), rect, 1);

	push	1
	lea	ecx, DWORD PTR _rect$74432[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	eax, DWORD PTR _i$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetColourName@CColourPopup@@IAEPBGH@Z	; CColourPopup::GetColourName
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 208				; 000000d0H
	call	?AddTool@CToolTipCtrl@@QAEHPAVCWnd@@PBGPBUtagRECT@@I@Z ; CToolTipCtrl::AddTool

; 729  :     }

	jmp	SHORT $L74430
$L74431:
$L74426:

; 730  : }

	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?CreateToolTips@CColourPopup@@IAEXXZ ENDP		; CColourPopup::CreateToolTips
_TEXT	ENDS
;	COMDAT ?GetColourName@CColourPopup@@IAEPBGH@Z
_TEXT	SEGMENT
_this$ = -4
_nIndex$ = 8
?GetColourName@CColourPopup@@IAEPBGH@Z PROC NEAR	; CColourPopup::GetColourName, COMDAT

; 88   :     LPCTSTR GetColourName(int nIndex)           { return m_crColours[nIndex].szName; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _nIndex$[ebp]
	mov	eax, DWORD PTR ?m_crColours@CColourPopup@@1PAUColourTableEntry@@A[eax*8+4]
	mov	esp, ebp
	pop	ebp
	ret	4
?GetColourName@CColourPopup@@IAEPBGH@Z ENDP		; CColourPopup::GetColourName
_TEXT	ENDS
EXTRN	?SendMessageW@CWnd@@QAEJIIJ@Z:NEAR		; CWnd::SendMessageW
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\ColourPopup.cpp
xdata$x	SEGMENT
$T74867	DD	019930520H
	DD	01H
	DD	FLAT:$T74869
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74869	DD	0ffffffffH
	DD	FLAT:$L74865
xdata$x	ENDS
;	COMDAT ?ChangeSelection@CColourPopup@@IAEXH@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_nIndex$ = 8
_this$ = -40
_dc$ = -32
_OldSel$74444 = -36
?ChangeSelection@CColourPopup@@IAEXH@Z PROC NEAR	; CColourPopup::ChangeSelection, COMDAT

; 733  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74868
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 28					; 0000001cH
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 734  :     CClientDC dc(this);        // device context for drawing

	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??0CClientDC@@QAE@PAVCWnd@@@Z		; CClientDC::CClientDC
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 735  : 
; 736  :     if (nIndex > m_nNumColours)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nIndex$[ebp]
	cmp	edx, DWORD PTR [ecx+100]
	jle	SHORT $L74440

; 737  :         nIndex = CUSTOM_BOX_VALUE; 

	mov	DWORD PTR _nIndex$[ebp], -2		; fffffffeH
$L74440:

; 740  :         m_nCurrentSel == CUSTOM_BOX_VALUE || m_nCurrentSel == DEFAULT_BOX_VALUE)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+120], 0
	jl	SHORT $L74443
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [ecx+120]
	cmp	eax, DWORD PTR [edx+100]
	jl	SHORT $L74442
$L74443:
	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+120], -2			; fffffffeH
	je	SHORT $L74442
	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+120], -3			; fffffffdH
	jne	SHORT $L74441
$L74442:

; 742  :         // Set Current selection as invalid and redraw old selection (this way
; 743  :         // the old selection will be drawn unselected)
; 744  :         int OldSel = m_nCurrentSel;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+120]
	mov	DWORD PTR _OldSel$74444[ebp], ecx

; 745  :         m_nCurrentSel = INVALID_COLOUR;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+120], -1

; 746  :         DrawCell(&dc, OldSel);

	mov	eax, DWORD PTR _OldSel$74444[ebp]
	push	eax
	lea	ecx, DWORD PTR _dc$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DrawCell@CColourPopup@@IAEXPAVCDC@@H@Z	; CColourPopup::DrawCell
$L74441:

; 748  : 
; 749  :     // Set the current selection as row/col and draw (it will be drawn selected)
; 750  :     m_nCurrentSel = nIndex;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nIndex$[ebp]
	mov	DWORD PTR [edx+120], eax

; 751  :     DrawCell(&dc, m_nCurrentSel);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+120]
	push	edx
	lea	eax, DWORD PTR _dc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DrawCell@CColourPopup@@IAEXPAVCDC@@H@Z	; CColourPopup::DrawCell

; 752  : 
; 753  :     // Store the current colour
; 754  :     if (m_nCurrentSel == CUSTOM_BOX_VALUE)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+120], -2			; fffffffeH
	jne	SHORT $L74445

; 755  :         m_pParent->SendMessage(CPN_SELCHANGE, (WPARAM) m_crInitialColour, m_nID);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+304]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+200]
	push	edx
	push	2025					; 000007e9H
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+300]
	call	?SendMessageW@CWnd@@QAEJIIJ@Z		; CWnd::SendMessageW

; 756  :     else if (m_nCurrentSel == DEFAULT_BOX_VALUE)

	jmp	SHORT $L74450
$L74445:
	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+120], -3			; fffffffdH
	jne	SHORT $L74448

; 758  :         m_crColour = CLR_DEFAULT;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+204], -16777216		; ff000000H

; 759  :         m_pParent->SendMessage(CPN_SELCHANGE, (WPARAM) CLR_DEFAULT, m_nID);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+304]
	push	ecx
	push	-16777216				; ff000000H
	push	2025					; 000007e9H
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+300]
	call	?SendMessageW@CWnd@@QAEJIIJ@Z		; CWnd::SendMessageW

; 761  :     else

	jmp	SHORT $L74450
$L74448:

; 763  :         m_crColour = GetColour(m_nCurrentSel);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+120]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetColour@CColourPopup@@IAEKH@Z	; CColourPopup::GetColour
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+204], eax

; 764  :         m_pParent->SendMessage(CPN_SELCHANGE, (WPARAM) m_crColour, m_nID);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+304]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+204]
	push	eax
	push	2025					; 000007e9H
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+300]
	call	?SendMessageW@CWnd@@QAEJIIJ@Z		; CWnd::SendMessageW
$L74450:

; 766  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??1CClientDC@@UAE@XZ			; CClientDC::~CClientDC
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74865:
	lea	ecx, DWORD PTR _dc$[ebp]
	call	??1CClientDC@@UAE@XZ			; CClientDC::~CClientDC
	ret	0
$L74868:
	mov	eax, OFFSET FLAT:$T74867
	jmp	___CxxFrameHandler
text$x	ENDS
?ChangeSelection@CColourPopup@@IAEXH@Z ENDP		; CColourPopup::ChangeSelection
PUBLIC	??1CColorDialog@@UAE@XZ				; CColorDialog::~CColorDialog
EXTRN	__imp__ReleaseCapture@0:NEAR
EXTRN	??0CColorDialog@@QAE@KKPAVCWnd@@@Z:NEAR		; CColorDialog::CColorDialog
EXTRN	?DoModal@CColorDialog@@UAEHXZ:NEAR		; CColorDialog::DoModal
EXTRN	?GetColor@CColorDialog@@QBEKXZ:NEAR		; CColorDialog::GetColor
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\ColourPopup.cpp
xdata$x	SEGMENT
$T74876	DD	019930520H
	DD	01H
	DD	FLAT:$T74878
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74878	DD	0ffffffffH
	DD	FLAT:$L74874
xdata$x	ENDS
;	COMDAT ?EndSelection@CColourPopup@@IAEXH@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_nMessage$ = 8
_this$ = -148
_dlg$74457 = -144
?EndSelection@CColourPopup@@IAEXH@Z PROC NEAR		; CColourPopup::EndSelection, COMDAT

; 769  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74877
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 136				; 00000088H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-148]
	mov	ecx, 34					; 00000022H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 770  :     ReleaseCapture();

	mov	esi, esp
	call	DWORD PTR __imp__ReleaseCapture@0
	cmp	esi, esp
	call	__chkesp

; 771  : 
; 772  :     // If custom text selected, perform a custom colour selection
; 773  :     if (nMessage != CPN_SELENDCANCEL && m_nCurrentSel == CUSTOM_BOX_VALUE)

	cmp	DWORD PTR _nMessage$[ebp], 2029		; 000007edH
	je	$L74456
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+120], -2			; fffffffeH
	jne	$L74456

; 775  :         m_bChildWindowVisible = TRUE;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+312], 1

; 776  : 
; 777  :         CColorDialog dlg(m_crInitialColour, CC_FULLOPEN | CC_ANYCOLOR, this);

	mov	edx, DWORD PTR _this$[ebp]
	push	edx
	push	258					; 00000102H
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+200]
	push	ecx
	lea	ecx, DWORD PTR _dlg$74457[ebp]
	call	??0CColorDialog@@QAE@KKPAVCWnd@@@Z	; CColorDialog::CColorDialog
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 778  : 
; 779  :         if (dlg.DoModal() == IDOK)

	lea	ecx, DWORD PTR _dlg$74457[ebp]
	call	?DoModal@CColorDialog@@UAEHXZ		; CColorDialog::DoModal
	cmp	eax, 1
	jne	SHORT $L74458

; 780  :             m_crColour = dlg.GetColor();

	lea	ecx, DWORD PTR _dlg$74457[ebp]
	call	?GetColor@CColorDialog@@QBEKXZ		; CColorDialog::GetColor
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+204], eax

; 781  :         else

	jmp	SHORT $L74459
$L74458:

; 782  :             nMessage = CPN_SELENDCANCEL;

	mov	DWORD PTR _nMessage$[ebp], 2029		; 000007edH
$L74459:

; 783  : 
; 784  :         m_bChildWindowVisible = FALSE;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+312], 0

; 785  :     } 

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _dlg$74457[ebp]
	call	??1CColorDialog@@UAE@XZ			; CColorDialog::~CColorDialog
$L74456:

; 786  : 
; 787  :     if (nMessage == CPN_SELENDCANCEL)

	cmp	DWORD PTR _nMessage$[ebp], 2029		; 000007edH
	jne	SHORT $L74460

; 788  :         m_crColour = m_crInitialColour;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+200]
	mov	DWORD PTR [ecx+204], eax
$L74460:

; 789  : 
; 790  : 	if (IsWindow(m_pParent->GetSafeHwnd()))

	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+300]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L74461

; 791  : 		m_pParent->SendMessage(nMessage, (WPARAM) m_crColour, m_nID);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+304]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+204]
	push	edx
	mov	eax, DWORD PTR _nMessage$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+300]
	call	?SendMessageW@CWnd@@QAEJIIJ@Z		; CWnd::SendMessageW
$L74461:

; 792  :     
; 793  :     // Kill focus bug fixed by Martin Wawrusch
; 794  :     if (!m_bChildWindowVisible)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+312], 0
	jne	SHORT $L74463

; 795  :         DestroyWindow();

	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+96]
	cmp	esi, esp
	call	__chkesp
$L74463:

; 796  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 148				; 00000094H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74874:
	lea	ecx, DWORD PTR _dlg$74457[ebp]
	call	??1CColorDialog@@UAE@XZ			; CColorDialog::~CColorDialog
	ret	0
$L74877:
	mov	eax, OFFSET FLAT:$T74876
	jmp	___CxxFrameHandler
text$x	ENDS
?EndSelection@CColourPopup@@IAEXH@Z ENDP		; CColourPopup::EndSelection
PUBLIC	??1CCommonDialog@@UAE@XZ			; CCommonDialog::~CCommonDialog
;	COMDAT ??1CColorDialog@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CColorDialog@@UAE@XZ PROC NEAR			; CColorDialog::~CColorDialog, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CCommonDialog@@UAE@XZ		; CCommonDialog::~CCommonDialog
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CColorDialog@@UAE@XZ ENDP				; CColorDialog::~CColorDialog
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
EXTRN	??0CPen@@QAE@XZ:NEAR				; CPen::CPen
EXTRN	??0CPen@@QAE@HHK@Z:NEAR				; CPen::CPen
EXTRN	?CreatePen@CPen@@QAEHHHK@Z:NEAR			; CPen::CreatePen
EXTRN	??1CPen@@UAE@XZ:NEAR				; CPen::~CPen
EXTRN	??0CBrush@@QAE@K@Z:NEAR				; CBrush::CBrush
EXTRN	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z:NEAR	; CDC::FillSolidRect
EXTRN	?FillSolidRect@CDC@@QAEXHHHHK@Z:NEAR		; CDC::FillSolidRect
EXTRN	??1CBrush@@UAE@XZ:NEAR				; CBrush::~CBrush
EXTRN	?GetDeviceCaps@CDC@@QBEHH@Z:NEAR		; CDC::GetDeviceCaps
EXTRN	?SelectObject@CDC@@QAEPAVCPen@@PAV2@@Z:NEAR	; CDC::SelectObject
EXTRN	?SelectObject@CDC@@QAEPAVCBrush@@PAV2@@Z:NEAR	; CDC::SelectObject
EXTRN	?SelectPalette@CDC@@QAEPAVCPalette@@PAV2@H@Z:NEAR ; CDC::SelectPalette
EXTRN	?RealizePalette@CDC@@QAEIXZ:NEAR		; CDC::RealizePalette
EXTRN	?SetBkMode@CDC@@QAEHH@Z:NEAR			; CDC::SetBkMode
EXTRN	?Rectangle@CDC@@QAEHPBUtagRECT@@@Z:NEAR		; CDC::Rectangle
EXTRN	?DeflateRect@CRect@@QAEXHH@Z:NEAR		; CRect::DeflateRect
EXTRN	?DrawTextW@CDC@@QAEHABVCString@@PAUtagRECT@@I@Z:NEAR ; CDC::DrawTextW
EXTRN	__imp__GetSysColor@4:NEAR
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\ColourPopup.cpp
xdata$x	SEGMENT
$T74894	DD	019930520H
	DD	03H
	DD	FLAT:$T74896
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74896	DD	0ffffffffH
	DD	FLAT:$L74890
	DD	0ffffffffH
	DD	FLAT:$L74891
	DD	01H
	DD	FLAT:$L74892
xdata$x	ENDS
;	COMDAT ?DrawCell@CColourPopup@@IAEXPAVCDC@@H@Z
_TEXT	SEGMENT
$T74889 = -128
__$EHRec$ = -12
_pDC$ = 8
_nIndex$ = 12
_this$ = -132
_TextButtonRect$74474 = -76
_pOldFont$74480 = -60
_TextButtonRect$74483 = -104
_LineRect$74486 = -120
_pen$74487 = -84
_pOldPen$74488 = -124
_pOldFont$74492 = -88
_rect$ = -36
_pOldPalette$ = -56
_brush$ = -52
_pen$ = -20
_pOldBrush$ = -40
_pOldPen$ = -44
?DrawCell@CColourPopup@@IAEXPAVCDC@@H@Z PROC NEAR	; CColourPopup::DrawCell, COMDAT

; 799  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74895
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 120				; 00000078H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-132]
	mov	ecx, 30					; 0000001eH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 800  :     // For the Custom Text area
; 801  :     if (m_strCustomText.GetLength() && nIndex == CUSTOM_BOX_VALUE)

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 132				; 00000084H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	$L74473
	cmp	DWORD PTR _nIndex$[ebp], -2		; fffffffeH
	jne	$L74473

; 803  :         // The extent of the actual text button
; 804  :         CRect TextButtonRect = m_CustomTextRect;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 136				; 00000088H
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _TextButtonRect$74474[ebp], ecx
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR _TextButtonRect$74474[ebp+4], edx
	mov	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR _TextButtonRect$74474[ebp+8], ecx
	mov	edx, DWORD PTR [eax+12]
	mov	DWORD PTR _TextButtonRect$74474[ebp+12], edx

; 805  :         TextButtonRect.top += 2*m_nMargin;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+116]
	mov	edx, DWORD PTR _TextButtonRect$74474[ebp+4]
	lea	eax, DWORD PTR [edx+ecx*2]
	mov	DWORD PTR _TextButtonRect$74474[ebp+4], eax

; 806  : 
; 807  :         // Fill background
; 808  :         pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DFACE));

	mov	esi, esp
	push	15					; 0000000fH
	call	DWORD PTR __imp__GetSysColor@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	lea	ecx, DWORD PTR _TextButtonRect$74474[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect

; 809  : 
; 810  :         // Draw horizontal line
; 811  :         pDC->FillSolidRect(m_CustomTextRect.left+2*m_nMargin, m_CustomTextRect.top,
; 812  :                            m_CustomTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DSHADOW));

	mov	esi, esp
	push	16					; 00000010H
	call	DWORD PTR __imp__GetSysColor@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 136				; 00000088H
	call	?Width@CRect@@QBEHXZ			; CRect::Width
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+116]
	shl	edx, 2
	sub	eax, edx
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+140]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+116]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+136]
	lea	eax, DWORD PTR [edx+eax*2]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXHHHHK@Z		; CDC::FillSolidRect

; 813  :         pDC->FillSolidRect(m_CustomTextRect.left+2*m_nMargin, m_CustomTextRect.top+1,
; 814  :                            m_CustomTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DHILIGHT));

	mov	esi, esp
	push	20					; 00000014H
	call	DWORD PTR __imp__GetSysColor@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 136				; 00000088H
	call	?Width@CRect@@QBEHXZ			; CRect::Width
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+116]
	shl	edx, 2
	sub	eax, edx
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+140]
	add	ecx, 1
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+116]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+136]
	lea	eax, DWORD PTR [edx+eax*2]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXHHHHK@Z		; CDC::FillSolidRect

; 815  : 
; 816  :         TextButtonRect.DeflateRect(1,1);

	push	1
	push	1
	lea	ecx, DWORD PTR _TextButtonRect$74474[ebp]
	call	?DeflateRect@CRect@@QAEXHH@Z		; CRect::DeflateRect

; 817  : 
; 818  :         // fill background
; 819  :         if (m_nChosenColourSel == nIndex && m_nCurrentSel != nIndex)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+124]
	cmp	edx, DWORD PTR _nIndex$[ebp]
	jne	SHORT $L74475
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+120]
	cmp	ecx, DWORD PTR _nIndex$[ebp]
	je	SHORT $L74475

; 820  :             pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DLIGHT));

	mov	esi, esp
	push	22					; 00000016H
	call	DWORD PTR __imp__GetSysColor@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	lea	ecx, DWORD PTR _TextButtonRect$74474[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect

; 821  :         else

	jmp	SHORT $L74476
$L74475:

; 822  :             pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DFACE));

	mov	esi, esp
	push	15					; 0000000fH
	call	DWORD PTR __imp__GetSysColor@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	lea	ecx, DWORD PTR _TextButtonRect$74474[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect
$L74476:

; 823  : 
; 824  :         // Draw button
; 825  :         if (m_nCurrentSel == nIndex) 

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+120]
	cmp	eax, DWORD PTR _nIndex$[ebp]
	jne	SHORT $L74477

; 826  :             pDC->DrawEdge(TextButtonRect, BDR_RAISEDINNER, BF_RECT);

	push	15					; 0000000fH
	push	4
	lea	ecx, DWORD PTR _TextButtonRect$74474[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?DrawEdge@CDC@@QAEHPAUtagRECT@@II@Z	; CDC::DrawEdge

; 827  :         else if (m_nChosenColourSel == nIndex)

	jmp	SHORT $L74479
$L74477:
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+124]
	cmp	edx, DWORD PTR _nIndex$[ebp]
	jne	SHORT $L74479

; 828  :             pDC->DrawEdge(TextButtonRect, BDR_SUNKENOUTER, BF_RECT);

	push	15					; 0000000fH
	push	2
	lea	ecx, DWORD PTR _TextButtonRect$74474[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?DrawEdge@CDC@@QAEHPAUtagRECT@@II@Z	; CDC::DrawEdge
$L74479:

; 829  : 
; 830  :         // Draw custom text
; 831  :         CFont *pOldFont = (CFont*) pDC->SelectObject(&m_Font);

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 184				; 000000b8H
	mov	esi, esp
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	DWORD PTR [edx+48]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pOldFont$74480[ebp], eax

; 832  :         pDC->SetBkMode(TRANSPARENT);

	push	1
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?SetBkMode@CDC@@QAEHH@Z			; CDC::SetBkMode

; 833  :         pDC->DrawText(m_strCustomText, TextButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	push	37					; 00000025H
	lea	ecx, DWORD PTR _TextButtonRect$74474[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 132				; 00000084H
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?DrawTextW@CDC@@QAEHABVCString@@PAUtagRECT@@I@Z ; CDC::DrawTextW

; 834  :         pDC->SelectObject(pOldFont);

	mov	esi, esp
	mov	ecx, DWORD PTR _pOldFont$74480[ebp]
	push	ecx
	mov	edx, DWORD PTR _pDC$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	DWORD PTR [eax+48]
	cmp	esi, esp
	call	__chkesp

; 835  : 
; 836  :         return;

	jmp	$L74472
$L74473:

; 838  : 
; 839  :     // For the Default Text area
; 840  :     if (m_strDefaultText.GetLength() && nIndex == DEFAULT_BOX_VALUE)

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 128				; 00000080H
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	test	eax, eax
	je	$L74482
	cmp	DWORD PTR _nIndex$[ebp], -3		; fffffffdH
	jne	$L74482

; 842  :         // Fill background
; 843  :         pDC->FillSolidRect(m_DefaultTextRect, ::GetSysColor(COLOR_3DFACE));

	mov	esi, esp
	push	15					; 0000000fH
	call	DWORD PTR __imp__GetSysColor@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 152				; 00000098H
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect

; 844  : 
; 845  :         // The extent of the actual text button
; 846  :         CRect TextButtonRect = m_DefaultTextRect;

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 152				; 00000098H
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _TextButtonRect$74483[ebp], edx
	mov	eax, DWORD PTR [ecx+4]
	mov	DWORD PTR _TextButtonRect$74483[ebp+4], eax
	mov	edx, DWORD PTR [ecx+8]
	mov	DWORD PTR _TextButtonRect$74483[ebp+8], edx
	mov	eax, DWORD PTR [ecx+12]
	mov	DWORD PTR _TextButtonRect$74483[ebp+12], eax

; 847  :         TextButtonRect.DeflateRect(1,1);

	push	1
	push	1
	lea	ecx, DWORD PTR _TextButtonRect$74483[ebp]
	call	?DeflateRect@CRect@@QAEXHH@Z		; CRect::DeflateRect

; 848  : 
; 849  :         // fill background
; 850  :         if (m_nChosenColourSel == nIndex && m_nCurrentSel != nIndex)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+124]
	cmp	edx, DWORD PTR _nIndex$[ebp]
	jne	SHORT $L74484
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+120]
	cmp	ecx, DWORD PTR _nIndex$[ebp]
	je	SHORT $L74484

; 851  :             pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DLIGHT));

	mov	esi, esp
	push	22					; 00000016H
	call	DWORD PTR __imp__GetSysColor@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	lea	ecx, DWORD PTR _TextButtonRect$74483[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect

; 852  :         else

	jmp	SHORT $L74485
$L74484:

; 853  :             pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DFACE));

	mov	esi, esp
	push	15					; 0000000fH
	call	DWORD PTR __imp__GetSysColor@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	lea	ecx, DWORD PTR _TextButtonRect$74483[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect
$L74485:

; 854  : 
; 855  :         // Draw thin line around text
; 856  :         CRect LineRect = TextButtonRect;

	mov	edx, DWORD PTR _TextButtonRect$74483[ebp]
	mov	DWORD PTR _LineRect$74486[ebp], edx
	mov	eax, DWORD PTR _TextButtonRect$74483[ebp+4]
	mov	DWORD PTR _LineRect$74486[ebp+4], eax
	mov	ecx, DWORD PTR _TextButtonRect$74483[ebp+8]
	mov	DWORD PTR _LineRect$74486[ebp+8], ecx
	mov	edx, DWORD PTR _TextButtonRect$74483[ebp+12]
	mov	DWORD PTR _LineRect$74486[ebp+12], edx

; 857  :         LineRect.DeflateRect(2*m_nMargin,2*m_nMargin);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+116]
	shl	ecx, 1
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+116]
	shl	eax, 1
	push	eax
	lea	ecx, DWORD PTR _LineRect$74486[ebp]
	call	?DeflateRect@CRect@@QAEXHH@Z		; CRect::DeflateRect

; 858  :         CPen pen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));

	mov	esi, esp
	push	16					; 00000010H
	call	DWORD PTR __imp__GetSysColor@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	push	1
	push	0
	lea	ecx, DWORD PTR _pen$74487[ebp]
	call	??0CPen@@QAE@HHK@Z			; CPen::CPen
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 859  :         CPen* pOldPen = pDC->SelectObject(&pen);

	lea	ecx, DWORD PTR _pen$74487[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?SelectObject@CDC@@QAEPAVCPen@@PAV2@@Z	; CDC::SelectObject
	mov	DWORD PTR _pOldPen$74488[ebp], eax

; 860  :         pDC->SelectStockObject(NULL_BRUSH);

	mov	esi, esp
	push	5
	mov	edx, DWORD PTR _pDC$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	DWORD PTR [eax+44]
	cmp	esi, esp
	call	__chkesp

; 861  :         pDC->Rectangle(LineRect);

	lea	ecx, DWORD PTR _LineRect$74486[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?Rectangle@CDC@@QAEHPBUtagRECT@@@Z	; CDC::Rectangle

; 862  :         pDC->SelectObject(pOldPen);

	mov	ecx, DWORD PTR _pOldPen$74488[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?SelectObject@CDC@@QAEPAVCPen@@PAV2@@Z	; CDC::SelectObject

; 863  : 
; 864  :         // Draw button
; 865  :         if (m_nCurrentSel == nIndex) 

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+120]
	cmp	eax, DWORD PTR _nIndex$[ebp]
	jne	SHORT $L74489

; 866  :             pDC->DrawEdge(TextButtonRect, BDR_RAISEDINNER, BF_RECT);

	push	15					; 0000000fH
	push	4
	lea	ecx, DWORD PTR _TextButtonRect$74483[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?DrawEdge@CDC@@QAEHPAUtagRECT@@II@Z	; CDC::DrawEdge

; 867  :         else if (m_nChosenColourSel == nIndex)

	jmp	SHORT $L74491
$L74489:
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+124]
	cmp	edx, DWORD PTR _nIndex$[ebp]
	jne	SHORT $L74491

; 868  :             pDC->DrawEdge(TextButtonRect, BDR_SUNKENOUTER, BF_RECT);

	push	15					; 0000000fH
	push	2
	lea	ecx, DWORD PTR _TextButtonRect$74483[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?DrawEdge@CDC@@QAEHPAUtagRECT@@II@Z	; CDC::DrawEdge
$L74491:

; 869  : 
; 870  :         // Draw custom text
; 871  :         CFont *pOldFont = (CFont*) pDC->SelectObject(&m_Font);

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 184				; 000000b8H
	mov	esi, esp
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	DWORD PTR [edx+48]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pOldFont$74492[ebp], eax

; 872  :         pDC->SetBkMode(TRANSPARENT);

	push	1
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?SetBkMode@CDC@@QAEHH@Z			; CDC::SetBkMode

; 873  :         pDC->DrawText(m_strDefaultText, TextButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	push	37					; 00000025H
	lea	ecx, DWORD PTR _TextButtonRect$74483[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 128				; 00000080H
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?DrawTextW@CDC@@QAEHABVCString@@PAUtagRECT@@I@Z ; CDC::DrawTextW

; 874  :         pDC->SelectObject(pOldFont);

	mov	esi, esp
	mov	ecx, DWORD PTR _pOldFont$74492[ebp]
	push	ecx
	mov	edx, DWORD PTR _pDC$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	DWORD PTR [eax+48]
	cmp	esi, esp
	call	__chkesp

; 875  : 
; 876  :         return;

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _pen$74487[ebp]
	call	??1CPen@@UAE@XZ				; CPen::~CPen
	jmp	$L74472
$L74482:

; 878  : 
; 879  :     CRect rect;

	lea	ecx, DWORD PTR _rect$[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 880  :     if (!GetCellRect(nIndex, rect)) return;

	lea	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	mov	DWORD PTR $T74889[ebp], eax
	lea	ecx, DWORD PTR $T74889[ebp]
	push	ecx
	mov	edx, DWORD PTR _nIndex$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCellRect@CColourPopup@@IAEHHABQAUtagRECT@@@Z ; CColourPopup::GetCellRect
	test	eax, eax
	jne	SHORT $L74496
	jmp	$L74472
$L74496:

; 881  : 
; 882  :     // Select and realize the palette
; 883  :     CPalette* pOldPalette = NULL;

	mov	DWORD PTR _pOldPalette$[ebp], 0

; 884  :     if (pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)

	push	38					; 00000026H
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetDeviceCaps@CDC@@QBEHH@Z		; CDC::GetDeviceCaps
	and	eax, 256				; 00000100H
	test	eax, eax
	je	SHORT $L74498

; 886  :         pOldPalette = pDC->SelectPalette(&m_Palette, FALSE);

	push	0
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 192				; 000000c0H
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?SelectPalette@CDC@@QAEPAVCPalette@@PAV2@H@Z ; CDC::SelectPalette
	mov	DWORD PTR _pOldPalette$[ebp], eax

; 887  :         pDC->RealizePalette();

	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?RealizePalette@CDC@@QAEIXZ		; CDC::RealizePalette
$L74498:

; 889  : 
; 890  :     // fill background
; 891  :     if (m_nChosenColourSel == nIndex && m_nCurrentSel != nIndex)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+124]
	cmp	edx, DWORD PTR _nIndex$[ebp]
	jne	SHORT $L74499
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+120]
	cmp	ecx, DWORD PTR _nIndex$[ebp]
	je	SHORT $L74499

; 892  :         pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DHILIGHT));

	mov	esi, esp
	push	20					; 00000014H
	call	DWORD PTR __imp__GetSysColor@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	lea	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect

; 893  :     else

	jmp	SHORT $L74500
$L74499:

; 894  :         pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));

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
$L74500:

; 895  : 
; 896  :     // Draw button
; 897  :     if (m_nCurrentSel == nIndex) 

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+120]
	cmp	eax, DWORD PTR _nIndex$[ebp]
	jne	SHORT $L74501

; 898  :         pDC->DrawEdge(rect, BDR_RAISEDINNER, BF_RECT);

	push	15					; 0000000fH
	push	4
	lea	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?DrawEdge@CDC@@QAEHPAUtagRECT@@II@Z	; CDC::DrawEdge

; 899  :     else if (m_nChosenColourSel == nIndex)

	jmp	SHORT $L74503
$L74501:
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+124]
	cmp	edx, DWORD PTR _nIndex$[ebp]
	jne	SHORT $L74503

; 900  :         pDC->DrawEdge(rect, BDR_SUNKENOUTER, BF_RECT);

	push	15					; 0000000fH
	push	2
	lea	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?DrawEdge@CDC@@QAEHPAUtagRECT@@II@Z	; CDC::DrawEdge
$L74503:

; 903  :                             GetGValue(GetColour(nIndex)), 
; 904  :                             GetBValue(GetColour(nIndex)) ));

	mov	eax, DWORD PTR _nIndex$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetColour@CColourPopup@@IAEKH@Z	; CColourPopup::GetColour
	mov	esi, eax
	and	esi, 255				; 000000ffH
	mov	ecx, DWORD PTR _nIndex$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetColour@CColourPopup@@IAEKH@Z	; CColourPopup::GetColour
	and	eax, 65535				; 0000ffffH
	sar	eax, 8
	and	eax, 255				; 000000ffH
	shl	eax, 8
	or	esi, eax
	mov	edx, DWORD PTR _nIndex$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetColour@CColourPopup@@IAEKH@Z	; CColourPopup::GetColour
	shr	eax, 16					; 00000010H
	and	eax, 255				; 000000ffH
	shl	eax, 16					; 00000010H
	or	esi, eax
	or	esi, 33554432				; 02000000H
	push	esi
	lea	ecx, DWORD PTR _brush$[ebp]
	call	??0CBrush@@QAE@K@Z			; CBrush::CBrush
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 905  :     CPen   pen;

	lea	ecx, DWORD PTR _pen$[ebp]
	call	??0CPen@@QAE@XZ				; CPen::CPen
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 906  :     pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));

	mov	esi, esp
	push	16					; 00000010H
	call	DWORD PTR __imp__GetSysColor@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	push	1
	push	0
	lea	ecx, DWORD PTR _pen$[ebp]
	call	?CreatePen@CPen@@QAEHHHK@Z		; CPen::CreatePen

; 907  : 
; 908  :     CBrush* pOldBrush = (CBrush*) pDC->SelectObject(&brush);

	lea	eax, DWORD PTR _brush$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?SelectObject@CDC@@QAEPAVCBrush@@PAV2@@Z ; CDC::SelectObject
	mov	DWORD PTR _pOldBrush$[ebp], eax

; 909  :     CPen*   pOldPen   = (CPen*)   pDC->SelectObject(&pen);

	lea	ecx, DWORD PTR _pen$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?SelectObject@CDC@@QAEPAVCPen@@PAV2@@Z	; CDC::SelectObject
	mov	DWORD PTR _pOldPen$[ebp], eax

; 910  : 
; 911  :     // Draw the cell colour
; 912  :     rect.DeflateRect(m_nMargin+1, m_nMargin+1);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+116]
	add	eax, 1
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+116]
	add	edx, 1
	push	edx
	lea	ecx, DWORD PTR _rect$[ebp]
	call	?DeflateRect@CRect@@QAEXHH@Z		; CRect::DeflateRect

; 913  :     pDC->Rectangle(rect);

	lea	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?Rectangle@CDC@@QAEHPBUtagRECT@@@Z	; CDC::Rectangle

; 914  : 
; 915  :     // restore DC and cleanup
; 916  :     pDC->SelectObject(pOldBrush);

	mov	eax, DWORD PTR _pOldBrush$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?SelectObject@CDC@@QAEPAVCBrush@@PAV2@@Z ; CDC::SelectObject

; 917  :     pDC->SelectObject(pOldPen);

	mov	ecx, DWORD PTR _pOldPen$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?SelectObject@CDC@@QAEPAVCPen@@PAV2@@Z	; CDC::SelectObject

; 918  :     brush.DeleteObject();

	lea	ecx, DWORD PTR _brush$[ebp]
	call	?DeleteObject@CGdiObject@@QAEHXZ	; CGdiObject::DeleteObject

; 919  :     pen.DeleteObject();

	lea	ecx, DWORD PTR _pen$[ebp]
	call	?DeleteObject@CGdiObject@@QAEHXZ	; CGdiObject::DeleteObject

; 920  : 
; 921  :     if (pOldPalette && pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)

	cmp	DWORD PTR _pOldPalette$[ebp], 0
	je	SHORT $L74520
	push	38					; 00000026H
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetDeviceCaps@CDC@@QBEHH@Z		; CDC::GetDeviceCaps
	and	eax, 256				; 00000100H
	test	eax, eax
	je	SHORT $L74520

; 922  :         pDC->SelectPalette(pOldPalette, FALSE);

	push	0
	mov	edx, DWORD PTR _pOldPalette$[ebp]
	push	edx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?SelectPalette@CDC@@QAEPAVCPalette@@PAV2@H@Z ; CDC::SelectPalette
$L74520:

; 923  : }

	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _pen$[ebp]
	call	??1CPen@@UAE@XZ				; CPen::~CPen
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _brush$[ebp]
	call	??1CBrush@@UAE@XZ			; CBrush::~CBrush
$L74472:
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 132				; 00000084H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74890:
	lea	ecx, DWORD PTR _pen$74487[ebp]
	call	??1CPen@@UAE@XZ				; CPen::~CPen
	ret	0
$L74891:
	lea	ecx, DWORD PTR _brush$[ebp]
	call	??1CBrush@@UAE@XZ			; CBrush::~CBrush
	ret	0
$L74892:
	lea	ecx, DWORD PTR _pen$[ebp]
	call	??1CPen@@UAE@XZ				; CPen::~CPen
	ret	0
$L74895:
	mov	eax, OFFSET FLAT:$T74894
	jmp	___CxxFrameHandler
text$x	ENDS
?DrawCell@CColourPopup@@IAEXPAVCDC@@H@Z ENDP		; CColourPopup::DrawCell
EXTRN	?Invalidate@CWnd@@QAEXH@Z:NEAR			; CWnd::Invalidate
EXTRN	?OnQueryNewPalette@CWnd@@IAEHXZ:NEAR		; CWnd::OnQueryNewPalette
;	COMDAT ?OnQueryNewPalette@CColourPopup@@IAEHXZ
_TEXT	SEGMENT
_this$ = -4
?OnQueryNewPalette@CColourPopup@@IAEHXZ PROC NEAR	; CColourPopup::OnQueryNewPalette, COMDAT

; 926  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 927  :     Invalidate();    

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Invalidate@CWnd@@QAEXH@Z		; CWnd::Invalidate

; 928  :     return CWnd::OnQueryNewPalette();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnQueryNewPalette@CWnd@@IAEHXZ		; CWnd::OnQueryNewPalette

; 929  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?OnQueryNewPalette@CColourPopup@@IAEHXZ ENDP		; CColourPopup::OnQueryNewPalette
_TEXT	ENDS
EXTRN	?OnPaletteChanged@CWnd@@IAEXPAV1@@Z:NEAR	; CWnd::OnPaletteChanged
;	COMDAT ?OnPaletteChanged@CColourPopup@@IAEXPAVCWnd@@@Z
_TEXT	SEGMENT
_pFocusWnd$ = 8
_this$ = -4
?OnPaletteChanged@CColourPopup@@IAEXPAVCWnd@@@Z PROC NEAR ; CColourPopup::OnPaletteChanged, COMDAT

; 932  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 933  :     CWnd::OnPaletteChanged(pFocusWnd);

	mov	eax, DWORD PTR _pFocusWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnPaletteChanged@CWnd@@IAEXPAV1@@Z	; CWnd::OnPaletteChanged

; 934  : 
; 935  :     if (pFocusWnd->GetSafeHwnd() != GetSafeHwnd())

	mov	ecx, DWORD PTR _pFocusWnd$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	mov	esi, eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	cmp	esi, eax
	je	SHORT $L74528

; 936  :         Invalidate();

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Invalidate@CWnd@@QAEXH@Z		; CWnd::Invalidate
$L74528:

; 937  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?OnPaletteChanged@CColourPopup@@IAEXPAVCWnd@@@Z ENDP	; CColourPopup::OnPaletteChanged
_TEXT	ENDS
EXTRN	?OnKillFocus@CWnd@@IAEXPAV1@@Z:NEAR		; CWnd::OnKillFocus
;	COMDAT ?OnKillFocus@CColourPopup@@IAEXPAVCWnd@@@Z
_TEXT	SEGMENT
_pNewWnd$ = 8
_this$ = -4
?OnKillFocus@CColourPopup@@IAEXPAVCWnd@@@Z PROC NEAR	; CColourPopup::OnKillFocus, COMDAT

; 940  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 941  : 	CWnd::OnKillFocus(pNewWnd);

	mov	eax, DWORD PTR _pNewWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnKillFocus@CWnd@@IAEXPAV1@@Z		; CWnd::OnKillFocus

; 942  : 
; 943  :     ReleaseCapture();

	mov	esi, esp
	call	DWORD PTR __imp__ReleaseCapture@0
	cmp	esi, esp
	call	__chkesp

; 944  :     //DestroyWindow(); - causes crash when Custom colour dialog appears.
; 945  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?OnKillFocus@CColourPopup@@IAEXPAVCWnd@@@Z ENDP		; CColourPopup::OnKillFocus
_TEXT	ENDS
EXTRN	?OnActivateApp@CWnd@@IAEXHPAUHTASK__@@@Z:NEAR	; CWnd::OnActivateApp
;	COMDAT ?OnActivateApp@CColourPopup@@IAEXHPAUHTASK__@@@Z
_TEXT	SEGMENT
_bActive$ = 8
_hTask$ = 12
_this$ = -4
?OnActivateApp@CColourPopup@@IAEXHPAUHTASK__@@@Z PROC NEAR ; CColourPopup::OnActivateApp, COMDAT

; 953  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 954  : 	CWnd::OnActivateApp(bActive, hTask);

	mov	eax, DWORD PTR _hTask$[ebp]
	push	eax
	mov	ecx, DWORD PTR _bActive$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnActivateApp@CWnd@@IAEXHPAUHTASK__@@@Z ; CWnd::OnActivateApp

; 955  : 
; 956  : 	// If Deactivating App, cancel this selection
; 957  : 	if (!bActive)

	cmp	DWORD PTR _bActive$[ebp], 0
	jne	SHORT $L74538

; 958  : 		 EndSelection(CPN_SELENDCANCEL);

	push	2029					; 000007edH
	mov	ecx, DWORD PTR _this$[ebp]
	call	?EndSelection@CColourPopup@@IAEXH@Z	; CColourPopup::EndSelection
$L74538:

; 959  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?OnActivateApp@CColourPopup@@IAEXHPAUHTASK__@@@Z ENDP	; CColourPopup::OnActivateApp
_TEXT	ENDS
END

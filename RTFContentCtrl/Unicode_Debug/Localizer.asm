	TITLE	D:\_CODE\Shared\Localizer.cpp
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
;	COMDAT ??_C@_03DANH@?6?7?$FL?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_04MNLE@?$FN?5?$DN?5?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_01BJG@?6?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
text$AFX_COL1	SEGMENT PARA USE32 PUBLIC ''
text$AFX_COL1	ENDS
;	COMDAT ?CreateTransTextInterface@@YAPAVITransText@@PBG0W4ITT_TRANSLATEOPTION@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsTransTextDll@@YAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
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
;	COMDAT ??0CLocDialog@@QAE@PBGPAVCWnd@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnInitDialog@CLocDialog@@UAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnShowWindow@CLocDialog@@IAEXHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCLocDialog@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CLocDialog@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_GetBaseMessageMap@CLocDialog@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMessageMap@CLocDialog@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Initialize@CLocalizer@@SAHPBGW4ITT_TRANSLATEOPTION@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Initialize@CLocalizer@@SAHPAVITransText@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ValidLocalizer@CLocalizer@@KAHPAVITransText@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ValidLocalizer@CLocalizer@@KAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Release@CLocalizer@@SAXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetTranslationOption@CLocalizer@@SAXW4ITT_TRANSLATEOPTION@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTranslationOption@CLocalizer@@SA?AW4ITT_TRANSLATEOPTION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetDictionaryPath@CLocalizer@@SA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetDictionaryVersion@CLocalizer@@SA?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetDictionaryVersion@CLocalizer@@SAHAAVCUIntArray@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CleanupDictionary@CLocalizer@@SAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ForceTranslateAllUIElements@CLocalizer@@SAXII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?TranslateText@CLocalizer@@SAHAAVCString@@PAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?TranslateMenu@CLocalizer@@SAHPAUHMENU__@@PAUHWND__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?UpdateMenu@CLocalizer@@SAXPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EnableTranslation@CLocalizer@@SAXPAUHWND__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EnableTranslation@CLocalizer@@SAXABVCWnd@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EnableTranslation@CLocalizer@@SAXPAUHMENU__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EnableTranslation@CLocalizer@@SAXIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EnableTranslation@CLocalizer@@SAXIIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IgnoreString@CLocalizer@@SAXABVCString@@@Z
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
;	COMDAT ??1?$CMap@IIHH@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CMap@IIHH@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CMap@IIHH@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CMap@IIHH@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$CList@II@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddTail@?$CList@II@@QAEPAU__POSITION@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Find@?$CList@II@@QBEPAU__POSITION@@IPAU2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CList@II@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CList@II@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CList@II@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CList@II@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@IIHH@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CList@II@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@IIHH@@QAEXIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@IIHH@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetStartPosition@?$CMap@IIHH@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNextAssoc@?$CMap@IIHH@@QBEXAAPAU__POSITION@@AAIAAH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CList@II@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetHeadPosition@?$CList@II@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNext@?$CList@II@@QAEAAIAAPAU__POSITION@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewNode@?$CList@II@@IAEPAUCNode@1@PAU21@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CompareElements@@YGHPBI0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CMap@IIHH@@QAEAAHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InitHashTable@?$CMap@IIHH@@QAEXIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewAssoc@?$CMap@IIHH@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAssocAt@?$CMap@IIHH@@IBEPAUCAssoc@1@IAAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HashKey@@YGII@Z
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
;	COMDAT ??_7?$CList@II@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CMap@IIHH@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CLocDialog@@6B@
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
PUBLIC	?OnShowWindow@CLocDialog@@IAEXHI@Z		; CLocDialog::OnShowWindow
PUBLIC	?_messageEntries@CLocDialog@@0QBUAFX_MSGMAP_ENTRY@@B ; CLocDialog::_messageEntries
PUBLIC	?messageMap@CLocDialog@@1UAFX_MSGMAP@@B		; CLocDialog::messageMap
PUBLIC	?_GetBaseMessageMap@CLocDialog@@KGPBUAFX_MSGMAP@@XZ ; CLocDialog::_GetBaseMessageMap
PUBLIC	?s_pTransText@CLocalizer@@1PAVITransText@@A	; CLocalizer::s_pTransText
PUBLIC	?s_bOwner@CLocalizer@@1HA			; CLocalizer::s_bOwner
_BSS	SEGMENT
?s_bOwner@CLocalizer@@1HA DD 01H DUP (?)		; CLocalizer::s_bOwner
_BSS	ENDS
_DATA	SEGMENT
__szAfxTempl DB	'afxtempl.h', 00H
	ORG $+1
_THIS_FILE DB	'D:\_CODE\Shared\Localizer.cpp', 00H
_DATA	ENDS
CONST	SEGMENT
?messageMap@CLocDialog@@1UAFX_MSGMAP@@B DD FLAT:?_GetBaseMessageMap@CLocDialog@@KGPBUAFX_MSGMAP@@XZ ; CLocDialog::messageMap
	DD	FLAT:?_messageEntries@CLocDialog@@0QBUAFX_MSGMAP_ENTRY@@B
?_messageEntries@CLocDialog@@0QBUAFX_MSGMAP_ENTRY@@B DD 018H ; CLocDialog::_messageEntries
	DD	00H
	DD	00H
	DD	00H
	DD	0eH
	DD	FLAT:?OnShowWindow@CLocDialog@@IAEXHI@Z
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
CONST	ENDS
_DATA	SEGMENT
	ORG $+2
?s_pTransText@CLocalizer@@1PAVITransText@@A DD 0ffffffffH ; CLocalizer::s_pTransText
_DATA	ENDS
EXTRN	?PostMessageW@CWnd@@QAEHIIJ@Z:NEAR		; CWnd::PostMessageW
EXTRN	__chkesp:NEAR
EXTRN	?OnShowWindow@CWnd@@IAEXHI@Z:NEAR		; CWnd::OnShowWindow
;	COMDAT ?OnShowWindow@CLocDialog@@IAEXHI@Z
_TEXT	SEGMENT
_bShow$ = 8
_nStatus$ = 12
_this$ = -4
?OnShowWindow@CLocDialog@@IAEXHI@Z PROC NEAR		; CLocDialog::OnShowWindow, COMDAT

; 42   : 	{

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 43   : 		CDialog::OnShowWindow(bShow, nStatus);

	mov	eax, DWORD PTR _nStatus$[ebp]
	push	eax
	mov	ecx, DWORD PTR _bShow$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnShowWindow@CWnd@@IAEXHI@Z		; CWnd::OnShowWindow

; 44   : 		PostMessage(WM_CLOSE);

	push	0
	push	0
	push	16					; 00000010H
	mov	ecx, DWORD PTR _this$[ebp]
	call	?PostMessageW@CWnd@@QAEHIIJ@Z		; CWnd::PostMessageW

; 45   : 	}

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?OnShowWindow@CLocDialog@@IAEXHI@Z ENDP			; CLocDialog::OnShowWindow
_TEXT	ENDS
EXTRN	__imp_?messageMap@CDialog@@1UAFX_MSGMAP@@B:QWORD
;	COMDAT ?_GetBaseMessageMap@CLocDialog@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
?_GetBaseMessageMap@CLocDialog@@KGPBUAFX_MSGMAP@@XZ PROC NEAR ; CLocDialog::_GetBaseMessageMap, COMDAT

; 50   : BEGIN_MESSAGE_MAP(CLocDialog, CDialog)

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR __imp_?messageMap@CDialog@@1UAFX_MSGMAP@@B
	pop	ebp
	ret	0
?_GetBaseMessageMap@CLocDialog@@KGPBUAFX_MSGMAP@@XZ ENDP ; CLocDialog::_GetBaseMessageMap
_TEXT	ENDS
PUBLIC	?GetMessageMap@CLocDialog@@MBEPBUAFX_MSGMAP@@XZ	; CLocDialog::GetMessageMap
;	COMDAT ?GetMessageMap@CLocDialog@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetMessageMap@CLocDialog@@MBEPBUAFX_MSGMAP@@XZ PROC NEAR ; CLocDialog::GetMessageMap, COMDAT

; 50   : BEGIN_MESSAGE_MAP(CLocDialog, CDialog)

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, OFFSET FLAT:?messageMap@CLocDialog@@1UAFX_MSGMAP@@B ; CLocDialog::messageMap
	mov	esp, ebp
	pop	ebp
	ret	0
?GetMessageMap@CLocDialog@@MBEPBUAFX_MSGMAP@@XZ ENDP	; CLocDialog::GetMessageMap
_TEXT	ENDS
PUBLIC	?Initialize@CLocalizer@@SAHPBGW4ITT_TRANSLATEOPTION@@@Z ; CLocalizer::Initialize
EXTRN	?SetLocalizer@CEnString@@SAXPAVITransText@@@Z:NEAR ; CEnString::SetLocalizer
EXTRN	?SetLocalizer@CEnMenu@@SAXPAVITransText@@@Z:NEAR ; CEnMenu::SetLocalizer
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	?FileExists@FileMisc@@YAHPBG@Z:NEAR		; FileMisc::FileExists
EXTRN	??H@YG?AVCString@@ABV0@PBG@Z:NEAR		; operator+
EXTRN	?GetAppFolder@FileMisc@@YA?AVCString@@PBG@Z:NEAR ; FileMisc::GetAppFolder
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT data
; File D:\_CODE\Shared\Localizer.cpp
data	SEGMENT
$SG75207 DB	'\', 00H, 'T', 00H, 'r', 00H, 'a', 00H, 'n', 00H, 's', 00H
	DB	'T', 00H, 'e', 00H, 'x', 00H, 't', 00H, '.', 00H, 'd', 00H, 'l'
	DB	00H, 'l', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76286	DD	019930520H
	DD	03H
	DD	FLAT:$T76289
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76289	DD	0ffffffffH
	DD	FLAT:$L76281
	DD	00H
	DD	FLAT:$L76282
	DD	0ffffffffH
	DD	FLAT:$L76282
xdata$x	ENDS
;	COMDAT ?Initialize@CLocalizer@@SAHPBGW4ITT_TRANSLATEOPTION@@@Z
_TEXT	SEGMENT
_szDictFile$ = 8
_nOption$ = 12
_sTTDll$75204 = -16
$T76280 = -20
__$EHRec$ = -12
?Initialize@CLocalizer@@SAHPBGW4ITT_TRANSLATEOPTION@@@Z PROC NEAR ; CLocalizer::Initialize, COMDAT

; 64   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76287
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 65   : 	if (s_pTransText == NEEDSINIT)

	cmp	DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A, -1 ; CLocalizer::s_pTransText
	jne	$L75203

; 67   : 		// Load TransText module
; 68   : 		CString sTTDll = FileMisc::GetAppFolder() + _T("\\TransText.dll");

	push	OFFSET FLAT:$SG75207
	push	0
	lea	eax, DWORD PTR $T76280[ebp]
	push	eax
	call	?GetAppFolder@FileMisc@@YA?AVCString@@PBG@Z ; FileMisc::GetAppFolder
	add	esp, 8
	mov	DWORD PTR -24+[ebp], eax
	mov	ecx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	edx, DWORD PTR -28+[ebp]
	push	edx
	lea	eax, DWORD PTR _sTTDll$75204[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR $T76280[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 69   : 		
; 70   : 		if (IsTransTextDll(sTTDll) && (nOption != ITTTO_TRANSLATEONLY || FileMisc::FileExists(szDictFile)))

	lea	ecx, DWORD PTR _sTTDll$75204[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?IsTransTextDll@@YAHPBG@Z		; IsTransTextDll
	add	esp, 4
	test	eax, eax
	je	SHORT $L75208
	cmp	DWORD PTR _nOption$[ebp], 0
	jne	SHORT $L75209
	mov	ecx, DWORD PTR _szDictFile$[ebp]
	push	ecx
	call	?FileExists@FileMisc@@YAHPBG@Z		; FileMisc::FileExists
	add	esp, 4
	test	eax, eax
	je	SHORT $L75208
$L75209:

; 72   : 			s_pTransText = CreateTransTextInterface(sTTDll, szDictFile, nOption);

	mov	edx, DWORD PTR _nOption$[ebp]
	push	edx
	mov	eax, DWORD PTR _szDictFile$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sTTDll$75204[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?CreateTransTextInterface@@YAPAVITransText@@PBG0W4ITT_TRANSLATEOPTION@@@Z ; CreateTransTextInterface
	add	esp, 12					; 0000000cH
	mov	DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A, eax ; CLocalizer::s_pTransText
$L75210:

; 73   : 			ASSERT(s_pTransText);

	cmp	DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A, 0 ; CLocalizer::s_pTransText
	jne	SHORT $L75213
	push	73					; 00000049H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75213
	int	3
$L75213:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75210

; 74   : 
; 75   : 			CEnString::SetLocalizer(s_pTransText);

	mov	edx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	push	edx
	call	?SetLocalizer@CEnString@@SAXPAVITransText@@@Z ; CEnString::SetLocalizer
	add	esp, 4

; 76   : 			CEnMenu::SetLocalizer(s_pTransText);

	mov	eax, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	push	eax
	call	?SetLocalizer@CEnMenu@@SAXPAVITransText@@@Z ; CEnMenu::SetLocalizer
	add	esp, 4

; 77   : 
; 78   : 			s_bOwner = TRUE;

	mov	DWORD PTR ?s_bOwner@CLocalizer@@1HA, 1	; CLocalizer::s_bOwner

; 80   : 		else

	jmp	SHORT $L75214
$L75208:

; 82   : 			s_pTransText = NULL;

	mov	DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A, 0 ; CLocalizer::s_pTransText

; 83   : 			s_bOwner = FALSE;

	mov	DWORD PTR ?s_bOwner@CLocalizer@@1HA, 0	; CLocalizer::s_bOwner
$L75214:

; 85   : 	}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sTTDll$75204[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75203:

; 86   : 
; 87   : 	return (s_pTransText != NEEDSINIT);

	xor	eax, eax
	cmp	DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A, -1 ; CLocalizer::s_pTransText
	setne	al

; 88   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76281:
	lea	ecx, DWORD PTR $T76280[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76282:
	lea	ecx, DWORD PTR _sTTDll$75204[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76287:
	mov	eax, OFFSET FLAT:$T76286
	jmp	___CxxFrameHandler
text$x	ENDS
?Initialize@CLocalizer@@SAHPBGW4ITT_TRANSLATEOPTION@@@Z ENDP ; CLocalizer::Initialize
EXTRN	__imp__GetProcAddress@8:NEAR
EXTRN	__imp__LoadLibraryW@4:NEAR
;	COMDAT data
; File D:\_CODE\Shared\ITransText.h
data	SEGMENT
$SG73078 DB	'CreateTransTextInterface', 00H
	ORG $+3
$SG73084 DB	'GetInterfaceVersion', 00H
data	ENDS
;	COMDAT ?CreateTransTextInterface@@YAPAVITransText@@PBG0W4ITT_TRANSLATEOPTION@@@Z
_TEXT	SEGMENT
_szDllPath$ = 8
_szTransFile$ = 12
_nOption$ = 16
_pInterface$ = -4
_hDll$ = -8
_pCreate$73076 = -12
_pVersion$73082 = -16
?CreateTransTextInterface@@YAPAVITransText@@PBG0W4ITT_TRANSLATEOPTION@@@Z PROC NEAR ; CreateTransTextInterface, COMDAT

; 39   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 40   :     ITransText* pInterface = NULL;

	mov	DWORD PTR _pInterface$[ebp], 0

; 41   :     HMODULE hDll = LoadLibrary(szDllPath);

	mov	esi, esp
	mov	eax, DWORD PTR _szDllPath$[ebp]
	push	eax
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hDll$[ebp], eax

; 42   : 	
; 43   :     if (hDll)

	cmp	DWORD PTR _hDll$[ebp], 0
	je	$L73085

; 45   :         PFNCREATETTI pCreate = (PFNCREATETTI)GetProcAddress(hDll, "CreateTransTextInterface");

	mov	esi, esp
	push	OFFSET FLAT:$SG73078
	mov	ecx, DWORD PTR _hDll$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pCreate$73076[ebp], eax

; 46   : 		
; 47   :         if (pCreate)

	cmp	DWORD PTR _pCreate$73076[ebp], 0
	je	SHORT $L73085

; 49   : 			if (ITRANSTEXT_VERSION == 0)

	mov	edx, 1
	test	edx, edx
	je	SHORT $L73080

; 50   : 				pInterface = pCreate(szTransFile, nOption);

	mov	esi, esp
	mov	eax, DWORD PTR _nOption$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szTransFile$[ebp]
	push	ecx
	call	DWORD PTR _pCreate$73076[ebp]
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pInterface$[ebp], eax

; 51   : 			else

	jmp	SHORT $L73085
$L73080:

; 53   : 				// check version
; 54   : 				PFNGETVERSION pVersion = (PFNGETVERSION)GetProcAddress(hDll, "GetInterfaceVersion");

	mov	esi, esp
	push	OFFSET FLAT:$SG73084
	mov	edx, DWORD PTR _hDll$[ebp]
	push	edx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pVersion$73082[ebp], eax

; 55   : 
; 56   : 				if ((pVersion && pVersion() >= ITRANSTEXT_VERSION))

	cmp	DWORD PTR _pVersion$73082[ebp], 0
	je	SHORT $L73085
	mov	esi, esp
	call	DWORD PTR _pVersion$73082[ebp]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jb	SHORT $L73085

; 57   : 					pInterface = pCreate(szTransFile, nOption);

	mov	esi, esp
	mov	eax, DWORD PTR _nOption$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szTransFile$[ebp]
	push	ecx
	call	DWORD PTR _pCreate$73076[ebp]
	add	esp, 8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pInterface$[ebp], eax
$L73085:

; 61   : 	
; 62   :     return pInterface;

	mov	eax, DWORD PTR _pInterface$[ebp]

; 63   : }

	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?CreateTransTextInterface@@YAPAVITransText@@PBG0W4ITT_TRANSLATEOPTION@@@Z ENDP ; CreateTransTextInterface
_TEXT	ENDS
EXTRN	__imp__FreeLibrary@4:NEAR
;	COMDAT data
; File D:\_CODE\Shared\ITransText.h
data	SEGMENT
$SG73093 DB	'CreateTransTextInterface', 00H
data	ENDS
;	COMDAT ?IsTransTextDll@@YAHPBG@Z
_TEXT	SEGMENT
_szDllPath$ = 8
_hDll$ = -4
_pCreate$73091 = -8
?IsTransTextDll@@YAHPBG@Z PROC NEAR			; IsTransTextDll, COMDAT

; 66   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 67   :     HMODULE hDll = LoadLibrary(szDllPath);

	mov	esi, esp
	mov	eax, DWORD PTR _szDllPath$[ebp]
	push	eax
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hDll$[ebp], eax

; 68   : 	
; 69   :     if (hDll)

	cmp	DWORD PTR _hDll$[ebp], 0
	je	SHORT $L73090

; 71   :         PFNCREATETTI pCreate = (PFNCREATETTI)GetProcAddress(hDll, "CreateTransTextInterface");

	mov	esi, esp
	push	OFFSET FLAT:$SG73093
	mov	ecx, DWORD PTR _hDll$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pCreate$73091[ebp], eax

; 72   : 		FreeLibrary(hDll);

	mov	esi, esp
	mov	edx, DWORD PTR _hDll$[ebp]
	push	edx
	call	DWORD PTR __imp__FreeLibrary@4
	cmp	esi, esp
	call	__chkesp

; 73   : 
; 74   : 		return (NULL != pCreate);

	xor	eax, eax
	cmp	DWORD PTR _pCreate$73091[ebp], 0
	setne	al
	jmp	SHORT $L73088
$L73090:

; 76   : 
; 77   : 	return FALSE;

	xor	eax, eax
$L73088:

; 78   : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsTransTextDll@@YAHPBG@Z ENDP				; IsTransTextDll
_TEXT	ENDS
PUBLIC	?Initialize@CLocalizer@@SAHPAVITransText@@@Z	; CLocalizer::Initialize
PUBLIC	?Release@CLocalizer@@SAXXZ			; CLocalizer::Release
PUBLIC	?ValidLocalizer@CLocalizer@@KAHPAVITransText@@@Z ; CLocalizer::ValidLocalizer
;	COMDAT ?Initialize@CLocalizer@@SAHPAVITransText@@@Z
_TEXT	SEGMENT
_pTT$ = 8
?Initialize@CLocalizer@@SAHPAVITransText@@@Z PROC NEAR	; CLocalizer::Initialize, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp

; 92   : 	// check it's valid
; 93   : 	if (!ValidLocalizer(pTT))

	mov	eax, DWORD PTR _pTT$[ebp]
	push	eax
	call	?ValidLocalizer@CLocalizer@@KAHPAVITransText@@@Z ; CLocalizer::ValidLocalizer
	add	esp, 4
	test	eax, eax
	jne	SHORT $L75219

; 94   : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75218
$L75219:

; 95   : 
; 96   : 	// check it's a different interface
; 97   : 	if (pTT == s_pTransText)

	mov	ecx, DWORD PTR _pTT$[ebp]
	cmp	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	jne	SHORT $L75220

; 98   : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L75218
$L75220:

; 99   : 
; 100  : 	// release existing interface
; 101  : 	Release();

	call	?Release@CLocalizer@@SAXXZ		; CLocalizer::Release

; 102  : 
; 103  : 	// assign as non-owner
; 104  : 	s_pTransText = pTT;

	mov	edx, DWORD PTR _pTT$[ebp]
	mov	DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A, edx ; CLocalizer::s_pTransText

; 105  : 	s_bOwner = FALSE;

	mov	DWORD PTR ?s_bOwner@CLocalizer@@1HA, 0	; CLocalizer::s_bOwner

; 106  : 
; 107  : 	CEnString::SetLocalizer(s_pTransText);

	mov	eax, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	push	eax
	call	?SetLocalizer@CEnString@@SAXPAVITransText@@@Z ; CEnString::SetLocalizer
	add	esp, 4

; 108  : 	CEnMenu::SetLocalizer(s_pTransText);

	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	push	ecx
	call	?SetLocalizer@CEnMenu@@SAXPAVITransText@@@Z ; CEnMenu::SetLocalizer
	add	esp, 4

; 109  : 
; 110  : 	return TRUE;

	mov	eax, 1
$L75218:

; 111  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?Initialize@CLocalizer@@SAHPAVITransText@@@Z ENDP	; CLocalizer::Initialize
_TEXT	ENDS
;	COMDAT ?ValidLocalizer@CLocalizer@@KAHPAVITransText@@@Z
_TEXT	SEGMENT
_pTT$ = 8
?ValidLocalizer@CLocalizer@@KAHPAVITransText@@@Z PROC NEAR ; CLocalizer::ValidLocalizer, COMDAT

; 114  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 115  : 	return (pTT && (pTT != NEEDSINIT));

	cmp	DWORD PTR _pTT$[ebp], 0
	je	SHORT $L76304
	cmp	DWORD PTR _pTT$[ebp], -1
	je	SHORT $L76304
	mov	DWORD PTR -4+[ebp], 1
	jmp	SHORT $L76305
$L76304:
	mov	DWORD PTR -4+[ebp], 0
$L76305:
	mov	eax, DWORD PTR -4+[ebp]

; 116  : }

	mov	esp, ebp
	pop	ebp
	ret	0
?ValidLocalizer@CLocalizer@@KAHPAVITransText@@@Z ENDP	; CLocalizer::ValidLocalizer
_TEXT	ENDS
PUBLIC	?ValidLocalizer@CLocalizer@@KAHXZ		; CLocalizer::ValidLocalizer
;	COMDAT ?ValidLocalizer@CLocalizer@@KAHXZ
_TEXT	SEGMENT
?ValidLocalizer@CLocalizer@@KAHXZ PROC NEAR		; CLocalizer::ValidLocalizer, COMDAT

; 119  : {

	push	ebp
	mov	ebp, esp

; 120  : 	return ValidLocalizer(s_pTransText);

	mov	eax, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	push	eax
	call	?ValidLocalizer@CLocalizer@@KAHPAVITransText@@@Z ; CLocalizer::ValidLocalizer
	add	esp, 4

; 121  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?ValidLocalizer@CLocalizer@@KAHXZ ENDP			; CLocalizer::ValidLocalizer
_TEXT	ENDS
;	COMDAT ?Release@CLocalizer@@SAXXZ
_TEXT	SEGMENT
?Release@CLocalizer@@SAXXZ PROC NEAR			; CLocalizer::Release, COMDAT

; 124  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 125  : 	if (ValidLocalizer() && s_bOwner)

	call	?ValidLocalizer@CLocalizer@@KAHXZ	; CLocalizer::ValidLocalizer
	test	eax, eax
	je	SHORT $L75229
	cmp	DWORD PTR ?s_bOwner@CLocalizer@@1HA, 0	; CLocalizer::s_bOwner
	je	SHORT $L75229

; 126  : 		s_pTransText->Release();

	mov	eax, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp
$L75229:

; 127  : 
; 128  : 	s_pTransText = NEEDSINIT;

	mov	DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A, -1 ; CLocalizer::s_pTransText

; 129  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?Release@CLocalizer@@SAXXZ ENDP				; CLocalizer::Release
_TEXT	ENDS
PUBLIC	?SetTranslationOption@CLocalizer@@SAXW4ITT_TRANSLATEOPTION@@@Z ; CLocalizer::SetTranslationOption
;	COMDAT ?SetTranslationOption@CLocalizer@@SAXW4ITT_TRANSLATEOPTION@@@Z
_TEXT	SEGMENT
_nOption$ = 8
?SetTranslationOption@CLocalizer@@SAXW4ITT_TRANSLATEOPTION@@@Z PROC NEAR ; CLocalizer::SetTranslationOption, COMDAT

; 132  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 133  : 	if (ValidLocalizer())

	call	?ValidLocalizer@CLocalizer@@KAHXZ	; CLocalizer::ValidLocalizer
	test	eax, eax
	je	SHORT $L75234

; 134  : 		s_pTransText->SetTranslationOption(nOption);

	mov	esi, esp
	mov	eax, DWORD PTR _nOption$[ebp]
	push	eax
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	call	DWORD PTR [edx+16]
	cmp	esi, esp
	call	__chkesp
$L75234:

; 135  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?SetTranslationOption@CLocalizer@@SAXW4ITT_TRANSLATEOPTION@@@Z ENDP ; CLocalizer::SetTranslationOption
_TEXT	ENDS
PUBLIC	?GetTranslationOption@CLocalizer@@SA?AW4ITT_TRANSLATEOPTION@@XZ ; CLocalizer::GetTranslationOption
;	COMDAT ?GetTranslationOption@CLocalizer@@SA?AW4ITT_TRANSLATEOPTION@@XZ
_TEXT	SEGMENT
?GetTranslationOption@CLocalizer@@SA?AW4ITT_TRANSLATEOPTION@@XZ PROC NEAR ; CLocalizer::GetTranslationOption, COMDAT

; 138  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 139  : 	if (ValidLocalizer())

	call	?ValidLocalizer@CLocalizer@@KAHXZ	; CLocalizer::ValidLocalizer
	test	eax, eax
	je	SHORT $L75237

; 140  : 		return s_pTransText->GetTranslationOption();

	mov	eax, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	call	DWORD PTR [edx+20]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75236
$L75237:

; 141  : 
; 142  : 	// else
; 143  : 	return ITTTO_TRANSLATEONLY;

	xor	eax, eax
$L75236:

; 144  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?GetTranslationOption@CLocalizer@@SA?AW4ITT_TRANSLATEOPTION@@XZ ENDP ; CLocalizer::GetTranslationOption
_TEXT	ENDS
PUBLIC	?GetDictionaryPath@CLocalizer@@SA?AVCString@@XZ	; CLocalizer::GetDictionaryPath
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
_BSS	SEGMENT
$SG75242 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT ?GetDictionaryPath@CLocalizer@@SA?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
$T76317 = -4
?GetDictionaryPath@CLocalizer@@SA?AVCString@@XZ PROC NEAR ; CLocalizer::GetDictionaryPath, COMDAT

; 147  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T76317[ebp], 0

; 148  : 	if (ValidLocalizer())

	call	?ValidLocalizer@CLocalizer@@KAHXZ	; CLocalizer::ValidLocalizer
	test	eax, eax
	je	SHORT $L75241

; 149  : 		return s_pTransText->GetDictionaryFile();

	mov	eax, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	call	DWORD PTR [edx+8]
	cmp	esi, esp
	call	__chkesp
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	eax, DWORD PTR $T76317[ebp]
	or	al, 1
	mov	DWORD PTR $T76317[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L75240
$L75241:

; 150  : 
; 151  : 	return _T("");

	push	OFFSET FLAT:$SG75242
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	ecx, DWORD PTR $T76317[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76317[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L75240:

; 152  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetDictionaryPath@CLocalizer@@SA?AVCString@@XZ ENDP	; CLocalizer::GetDictionaryPath
_TEXT	ENDS
PUBLIC	?GetDictionaryVersion@CLocalizer@@SA?AVCString@@XZ ; CLocalizer::GetDictionaryVersion
_BSS	SEGMENT
	ALIGN	4

$SG75247 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT ?GetDictionaryVersion@CLocalizer@@SA?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
$T76322 = -4
?GetDictionaryVersion@CLocalizer@@SA?AVCString@@XZ PROC NEAR ; CLocalizer::GetDictionaryVersion, COMDAT

; 155  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T76322[ebp], 0

; 156  : 	if (ValidLocalizer())

	call	?ValidLocalizer@CLocalizer@@KAHXZ	; CLocalizer::ValidLocalizer
	test	eax, eax
	je	SHORT $L75246

; 157  : 		return s_pTransText->GetDictionaryVersion();

	mov	eax, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	call	DWORD PTR [edx+12]
	cmp	esi, esp
	call	__chkesp
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	eax, DWORD PTR $T76322[ebp]
	or	al, 1
	mov	DWORD PTR $T76322[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L75245
$L75246:

; 158  : 
; 159  : 	return _T("");

	push	OFFSET FLAT:$SG75247
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	ecx, DWORD PTR $T76322[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76322[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L75245:

; 160  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetDictionaryVersion@CLocalizer@@SA?AVCString@@XZ ENDP	; CLocalizer::GetDictionaryVersion
_TEXT	ENDS
PUBLIC	?GetDictionaryVersion@CLocalizer@@SAHAAVCUIntArray@@@Z ; CLocalizer::GetDictionaryVersion
EXTRN	?SplitVersionNumber@FileMisc@@YAHPBGAAVCUIntArray@@@Z:NEAR ; FileMisc::SplitVersionNumber
;	COMDAT xdata$x
; File D:\_CODE\Shared\Localizer.cpp
xdata$x	SEGMENT
$T76329	DD	019930520H
	DD	01H
	DD	FLAT:$T76331
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76331	DD	0ffffffffH
	DD	FLAT:$L76327
xdata$x	ENDS
;	COMDAT ?GetDictionaryVersion@CLocalizer@@SAHAAVCUIntArray@@@Z
_TEXT	SEGMENT
_aVersionParts$ = 8
_sVersion$ = -16
$T76326 = -20
__$EHRec$ = -12
?GetDictionaryVersion@CLocalizer@@SAHAAVCUIntArray@@@Z PROC NEAR ; CLocalizer::GetDictionaryVersion, COMDAT

; 163  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76330
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH

; 164  : 	CString sVersion = GetDictionaryVersion();

	lea	eax, DWORD PTR _sVersion$[ebp]
	push	eax
	call	?GetDictionaryVersion@CLocalizer@@SA?AVCString@@XZ ; CLocalizer::GetDictionaryVersion
	add	esp, 4
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 165  : 
; 166  : 	return FileMisc::SplitVersionNumber(sVersion, aVersionParts);

	mov	ecx, DWORD PTR _aVersionParts$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sVersion$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?SplitVersionNumber@FileMisc@@YAHPBGAAVCUIntArray@@@Z ; FileMisc::SplitVersionNumber
	add	esp, 8
	mov	DWORD PTR $T76326[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sVersion$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76326[ebp]

; 167  : }

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
$L76327:
	lea	ecx, DWORD PTR _sVersion$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76330:
	mov	eax, OFFSET FLAT:$T76329
	jmp	___CxxFrameHandler
text$x	ENDS
?GetDictionaryVersion@CLocalizer@@SAHAAVCUIntArray@@@Z ENDP ; CLocalizer::GetDictionaryVersion
PUBLIC	?CleanupDictionary@CLocalizer@@SAHPBG@Z		; CLocalizer::CleanupDictionary
;	COMDAT ?CleanupDictionary@CLocalizer@@SAHPBG@Z
_TEXT	SEGMENT
_szMasterDictPath$ = 8
?CleanupDictionary@CLocalizer@@SAHPBG@Z PROC NEAR	; CLocalizer::CleanupDictionary, COMDAT

; 170  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 171  : 	if (ValidLocalizer())

	call	?ValidLocalizer@CLocalizer@@KAHXZ	; CLocalizer::ValidLocalizer
	test	eax, eax
	je	SHORT $L75257

; 172  : 		return s_pTransText->CleanupDictionary(szMasterDictPath);

	mov	esi, esp
	mov	eax, DWORD PTR _szMasterDictPath$[ebp]
	push	eax
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	call	DWORD PTR [edx+24]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75256
$L75257:

; 173  : 
; 174  : 	return FALSE;

	xor	eax, eax
$L75256:

; 175  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?CleanupDictionary@CLocalizer@@SAHPBG@Z ENDP		; CLocalizer::CleanupDictionary
_TEXT	ENDS
PUBLIC	??0CLocDialog@@QAE@PBGPAVCWnd@@@Z		; CLocDialog::CLocDialog
PUBLIC	??1CLocDialog@@UAE@XZ				; CLocDialog::~CLocDialog
PUBLIC	?ForceTranslateAllUIElements@CLocalizer@@SAXII@Z ; CLocalizer::ForceTranslateAllUIElements
PUBLIC	?TranslateText@CLocalizer@@SAHAAVCString@@PAUHWND__@@@Z ; CLocalizer::TranslateText
PUBLIC	?TranslateMenu@CLocalizer@@SAHPAUHMENU__@@PAUHWND__@@H@Z ; CLocalizer::TranslateMenu
PUBLIC	??0?$CList@II@@QAE@H@Z				; CList<unsigned int,unsigned int>::CList<unsigned int,unsigned int>
PUBLIC	?AddTail@?$CList@II@@QAEPAU__POSITION@@I@Z	; CList<unsigned int,unsigned int>::AddTail
PUBLIC	?Find@?$CList@II@@QBEPAU__POSITION@@IPAU2@@Z	; CList<unsigned int,unsigned int>::Find
PUBLIC	??1?$CList@II@@UAE@XZ				; CList<unsigned int,unsigned int>::~CList<unsigned int,unsigned int>
EXTRN	?DoModal@CDialog@@UAEHXZ:NEAR			; CDialog::DoModal
EXTRN	??0CMenu@@QAE@XZ:NEAR				; CMenu::CMenu
EXTRN	?LoadMenuW@CMenu@@QAEHI@Z:NEAR			; CMenu::LoadMenuW
EXTRN	??BCMenu@@QBEPAUHMENU__@@XZ:NEAR		; CMenu::operator HMENU__ *
EXTRN	??0CToolBar@@QAE@XZ:NEAR			; CToolBar::CToolBar
EXTRN	?CreateEx@CToolBar@@QAEHPAVCWnd@@KKVCRect@@I@Z:NEAR ; CToolBar::CreateEx
EXTRN	?LoadToolBar@CToolBar@@QAEHI@Z:NEAR		; CToolBar::LoadToolBar
EXTRN	?GetItemID@CToolBar@@QBEIH@Z:NEAR		; CToolBar::GetItemID
EXTRN	??1CMenu@@UAE@XZ:NEAR				; CMenu::~CMenu
EXTRN	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ:NEAR ; CToolBar::GetToolBarCtrl
EXTRN	??1CToolBar@@UAE@XZ:NEAR			; CToolBar::~CToolBar
EXTRN	__imp__FindResourceW@12:NEAR
EXTRN	??0CToolTipCtrl@@QAE@XZ:NEAR			; CToolTipCtrl::CToolTipCtrl
EXTRN	?Create@CToolTipCtrl@@QAEHPAVCWnd@@K@Z:NEAR	; CToolTipCtrl::Create
EXTRN	??1CToolTipCtrl@@UAE@XZ:NEAR			; CToolTipCtrl::~CToolTipCtrl
EXTRN	?AfxGetMainWnd@@YGPAVCWnd@@XZ:NEAR		; AfxGetMainWnd
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	??0CRect@@QAE@HHHH@Z:NEAR			; CRect::CRect
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
EXTRN	?Right@CString@@QBE?AV1@H@Z:NEAR		; CString::Right
EXTRN	?TrimRight@CString@@QAEXXZ:NEAR			; CString::TrimRight
EXTRN	?TrimLeft@CString@@QAEXXZ:NEAR			; CString::TrimLeft
EXTRN	?Replace@CString@@QAEHPBG0@Z:NEAR		; CString::Replace
EXTRN	?Find@CString@@QBEHG@Z:NEAR			; CString::Find
EXTRN	?GetButtonCount@CToolBarCtrl@@QBEHXZ:NEAR	; CToolBarCtrl::GetButtonCount
EXTRN	?LoadStringW@CString@@QAEHI@Z:NEAR		; CString::LoadStringW
EXTRN	??0CWaitCursor@@QAE@XZ:NEAR			; CWaitCursor::CWaitCursor
EXTRN	??1CWaitCursor@@QAE@XZ:NEAR			; CWaitCursor::~CWaitCursor
EXTRN	??BCWnd@@QBEPAUHWND__@@XZ:NEAR			; CWnd::operator HWND__ *
EXTRN	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ:NEAR	; CWnd::GetSafeHwnd
_BSS	SEGMENT
	ALIGN	4

$SG75431 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\Localizer.cpp
data	SEGMENT
$SG75432 DB	'.', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76351	DD	019930520H
	DD	0aH
	DD	FLAT:$T76353
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76353	DD	0ffffffffH
	DD	FLAT:$L76340
	DD	00H
	DD	FLAT:$L76341
	DD	0ffffffffH
	DD	FLAT:$L76342
	DD	02H
	DD	FLAT:$L76343
	DD	02H
	DD	FLAT:$L76344
	DD	04H
	DD	FLAT:$L76345
	DD	05H
	DD	FLAT:$L76346
	DD	06H
	DD	FLAT:$L76347
	DD	07H
	DD	FLAT:$L76348
	DD	05H
	DD	FLAT:$L76349
xdata$x	ENDS
;	COMDAT ?ForceTranslateAllUIElements@CLocalizer@@SAXII@Z
_TEXT	SEGMENT
_nIDFirstStr$ = 8
_nIDLastStr$ = 12
_pAppWnd$ = -60
_nDlg$ = -16
_szDlgTemplate$75272 = -172
_hResource$75276 = -168
_wait$75281 = -164
_dialog$75282 = -268
_wait$ = -20
_nMenu$ = -56
_szMenuDef$75288 = -280
_hResource$75292 = -284
_menu$75296 = -276
$T76338 = -488
$T76339 = -492
__$EHRec$ = -12
_lstTooltipIDs$ = -52
_tt$ = -156
_nTBar$ = -64
_szTBDef$75403 = -464
_hResource$75407 = -452
_toolbar$75411 = -444
_nBtnCount$75419 = -460
_sTip$75420 = -456
_nBtn$75421 = -448
_nID$75425 = -468
_nStartTip$75427 = -472
_sStr$ = -160
_nStr$ = -24
?ForceTranslateAllUIElements@CLocalizer@@SAXII@Z PROC NEAR ; CLocalizer::ForceTranslateAllUIElements, COMDAT

; 178  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76352
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 488				; 000001e8H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-500]
	mov	ecx, 122				; 0000007aH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 179  : 	CWnd* pAppWnd = AfxGetMainWnd();

	call	?AfxGetMainWnd@@YGPAVCWnd@@XZ		; AfxGetMainWnd
	mov	DWORD PTR _pAppWnd$[ebp], eax
$L75263:

; 180  : 	ASSERT(pAppWnd);

	cmp	DWORD PTR _pAppWnd$[ebp], 0
	jne	SHORT $L75266
	push	180					; 000000b4H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75266
	int	3
$L75266:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75263

; 181  : 
; 182  : 	if (pAppWnd == NULL)

	cmp	DWORD PTR _pAppWnd$[ebp], 0
	jne	SHORT $L75267

; 183  : 		return;

	jmp	$L75261
$L75267:

; 184  : 
; 185  : 	// we translate dialogs by displaying them
; 186  : 	for (int nDlg = 1; nDlg <= 0x6FFF; nDlg++)

	mov	DWORD PTR _nDlg$[ebp], 1
	jmp	SHORT $L75269
$L75270:
	mov	ecx, DWORD PTR _nDlg$[ebp]
	add	ecx, 1
	mov	DWORD PTR _nDlg$[ebp], ecx
$L75269:
	cmp	DWORD PTR _nDlg$[ebp], 28671		; 00006fffH
	jg	$L75271

; 188  : 		LPCTSTR szDlgTemplate = MAKEINTRESOURCE(nDlg);

	mov	edx, DWORD PTR _nDlg$[ebp]
	and	edx, 65535				; 0000ffffH
	mov	DWORD PTR _szDlgTemplate$75272[ebp], edx

; 189  : 		HRSRC hResource = ::FindResource(NULL, szDlgTemplate, RT_DIALOG);

	mov	esi, esp
	push	5
	mov	eax, DWORD PTR _szDlgTemplate$75272[ebp]
	push	eax
	push	0
	call	DWORD PTR __imp__FindResourceW@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hResource$75276[ebp], eax

; 190  : 		
; 191  : 		if (hResource == NULL)

	cmp	DWORD PTR _hResource$75276[ebp], 0
	jne	SHORT $L75280

; 192  : 			continue;

	jmp	SHORT $L75270
$L75280:

; 193  : 		
; 194  : 		CWaitCursor wait;

	lea	ecx, DWORD PTR _wait$75281[ebp]
	call	??0CWaitCursor@@QAE@XZ			; CWaitCursor::CWaitCursor
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 195  : 		CLocDialog dialog(szDlgTemplate, pAppWnd);

	mov	ecx, DWORD PTR _pAppWnd$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szDlgTemplate$75272[ebp]
	push	edx
	lea	ecx, DWORD PTR _dialog$75282[ebp]
	call	??0CLocDialog@@QAE@PBGPAVCWnd@@@Z	; CLocDialog::CLocDialog
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 196  : 		
; 197  : 		dialog.DoModal();

	lea	ecx, DWORD PTR _dialog$75282[ebp]
	call	?DoModal@CDialog@@UAEHXZ		; CDialog::DoModal

; 198  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _dialog$75282[ebp]
	call	??1CLocDialog@@UAE@XZ			; CLocDialog::~CLocDialog
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _wait$75281[ebp]
	call	??1CWaitCursor@@QAE@XZ			; CWaitCursor::~CWaitCursor
	jmp	$L75270
$L75271:

; 199  : 
; 200  : 	CWaitCursor wait;

	lea	ecx, DWORD PTR _wait$[ebp]
	call	??0CWaitCursor@@QAE@XZ			; CWaitCursor::CWaitCursor
	mov	DWORD PTR __$EHRec$[ebp+8], 2

; 201  : 
; 202  : 	// we translate menus directly
; 203  : 	for (int nMenu = 1; nMenu <= 0x6FFF; nMenu++)

	mov	DWORD PTR _nMenu$[ebp], 1
	jmp	SHORT $L75285
$L75286:
	mov	eax, DWORD PTR _nMenu$[ebp]
	add	eax, 1
	mov	DWORD PTR _nMenu$[ebp], eax
$L75285:
	cmp	DWORD PTR _nMenu$[ebp], 28671		; 00006fffH
	jg	$L75287

; 205  : 		LPCTSTR szMenuDef = MAKEINTRESOURCE(nMenu);

	mov	ecx, DWORD PTR _nMenu$[ebp]
	and	ecx, 65535				; 0000ffffH
	mov	DWORD PTR _szMenuDef$75288[ebp], ecx

; 206  : 		HRSRC hResource = ::FindResource(NULL, szMenuDef, RT_MENU);

	mov	esi, esp
	push	4
	mov	edx, DWORD PTR _szMenuDef$75288[ebp]
	push	edx
	push	0
	call	DWORD PTR __imp__FindResourceW@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hResource$75292[ebp], eax

; 207  : 		CMenu menu;

	lea	ecx, DWORD PTR _menu$75296[ebp]
	call	??0CMenu@@QAE@XZ			; CMenu::CMenu
	mov	BYTE PTR __$EHRec$[ebp+8], 3

; 208  : 		
; 209  : 		if (hResource == NULL)

	cmp	DWORD PTR _hResource$75292[ebp], 0
	jne	SHORT $L75297

; 210  : 			continue;

	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _menu$75296[ebp]
	call	??1CMenu@@UAE@XZ			; CMenu::~CMenu
	jmp	SHORT $L75286
$L75297:

; 211  : 		
; 212  : 		VERIFY(menu.LoadMenu(nMenu));

	mov	eax, DWORD PTR _nMenu$[ebp]
	push	eax
	lea	ecx, DWORD PTR _menu$75296[ebp]
	call	?LoadMenuW@CMenu@@QAEHI@Z		; CMenu::LoadMenuW
	test	eax, eax
	jne	SHORT $L75301
	push	212					; 000000d4H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75301
	int	3
$L75301:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75297

; 213  : 
; 214  : 		CLocalizer::TranslateMenu(menu);

	push	1
	push	0
	lea	ecx, DWORD PTR _menu$75296[ebp]
	call	??BCMenu@@QBEPAUHMENU__@@XZ		; CMenu::operator HMENU__ *
	push	eax
	call	?TranslateMenu@CLocalizer@@SAHPAUHMENU__@@PAUHWND__@@H@Z ; CLocalizer::TranslateMenu
	add	esp, 12					; 0000000cH

; 215  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _menu$75296[ebp]
	call	??1CMenu@@UAE@XZ			; CMenu::~CMenu
	jmp	$L75286
$L75287:

; 216  : 
; 217  : 	// we translate tooltips by loading the toolbar and
; 218  : 	// iterating its buttons directly
; 219  : 	CList<UINT, UINT> lstTooltipIDs;

	push	10					; 0000000aH
	lea	ecx, DWORD PTR _lstTooltipIDs$[ebp]
	call	??0?$CList@II@@QAE@H@Z			; CList<unsigned int,unsigned int>::CList<unsigned int,unsigned int>
	mov	BYTE PTR __$EHRec$[ebp+8], 4

; 220  : 
; 221  : 	// dummy tooltipctrl to pass to CLocalizer
; 222  : 	CToolTipCtrl tt;

	lea	ecx, DWORD PTR _tt$[ebp]
	call	??0CToolTipCtrl@@QAE@XZ			; CToolTipCtrl::CToolTipCtrl
	mov	BYTE PTR __$EHRec$[ebp+8], 5

; 223  : 
; 224  : 	for (int nTBar = 1; nTBar <= 0x6FFF; nTBar++)

	mov	DWORD PTR _nTBar$[ebp], 1
	jmp	SHORT $L75400
$L75401:
	mov	edx, DWORD PTR _nTBar$[ebp]
	add	edx, 1
	mov	DWORD PTR _nTBar$[ebp], edx
$L75400:
	cmp	DWORD PTR _nTBar$[ebp], 28671		; 00006fffH
	jg	$L75402

; 226  : 		LPCTSTR szTBDef = MAKEINTRESOURCE(nTBar);

	mov	eax, DWORD PTR _nTBar$[ebp]
	and	eax, 65535				; 0000ffffH
	mov	DWORD PTR _szTBDef$75403[ebp], eax

; 227  : 		HRSRC hResource = ::FindResource(NULL, szTBDef, RT_TOOLBAR);

	mov	esi, esp
	push	241					; 000000f1H
	mov	ecx, DWORD PTR _szTBDef$75403[ebp]
	push	ecx
	push	0
	call	DWORD PTR __imp__FindResourceW@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hResource$75407[ebp], eax

; 228  : 		CToolBar toolbar;

	lea	ecx, DWORD PTR _toolbar$75411[ebp]
	call	??0CToolBar@@QAE@XZ			; CToolBar::CToolBar
	mov	BYTE PTR __$EHRec$[ebp+8], 6

; 229  : 		
; 230  : 		if (hResource == NULL)

	cmp	DWORD PTR _hResource$75407[ebp], 0
	jne	SHORT $L75412

; 231  : 			continue;

	mov	BYTE PTR __$EHRec$[ebp+8], 5
	lea	ecx, DWORD PTR _toolbar$75411[ebp]
	call	??1CToolBar@@UAE@XZ			; CToolBar::~CToolBar
	jmp	SHORT $L75401
$L75412:

; 232  : 		
; 233  : 		VERIFY (toolbar.CreateEx(pAppWnd, 0, WS_CHILD | CBRS_TOOLTIPS) && toolbar.LoadToolBar(nTBar));

	push	59392					; 0000e800H
	push	0
	push	0
	push	0
	push	0
	lea	ecx, DWORD PTR $T76338[ebp]
	call	??0CRect@@QAE@HHHH@Z			; CRect::CRect
	sub	esp, 16					; 00000010H
	mov	edx, esp
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR [edx], ecx
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR [edx+4], ecx
	mov	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR [edx+8], ecx
	mov	eax, DWORD PTR [eax+12]
	mov	DWORD PTR [edx+12], eax
	push	1073741840				; 40000010H
	push	0
	mov	ecx, DWORD PTR _pAppWnd$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _toolbar$75411[ebp]
	call	?CreateEx@CToolBar@@QAEHPAVCWnd@@KKVCRect@@I@Z ; CToolBar::CreateEx
	test	eax, eax
	je	SHORT $L75418
	mov	edx, DWORD PTR _nTBar$[ebp]
	push	edx
	lea	ecx, DWORD PTR _toolbar$75411[ebp]
	call	?LoadToolBar@CToolBar@@QAEHI@Z		; CToolBar::LoadToolBar
	test	eax, eax
	jne	SHORT $L75417
$L75418:
	push	233					; 000000e9H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75417
	int	3
$L75417:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75412

; 234  : 
; 235  : 		int nBtnCount = toolbar.GetToolBarCtrl().GetButtonCount();

	lea	ecx, DWORD PTR _toolbar$75411[ebp]
	call	?GetToolBarCtrl@CToolBar@@QBEAAVCToolBarCtrl@@XZ ; CToolBar::GetToolBarCtrl
	mov	ecx, eax
	call	?GetButtonCount@CToolBarCtrl@@QBEHXZ	; CToolBarCtrl::GetButtonCount
	mov	DWORD PTR _nBtnCount$75419[ebp], eax

; 236  : 		CString sTip;

	lea	ecx, DWORD PTR _sTip$75420[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 7

; 237  : 		
; 238  : 		for (int nBtn = 0; nBtn < nBtnCount; nBtn++)

	mov	DWORD PTR _nBtn$75421[ebp], 0
	jmp	SHORT $L75422
$L75423:
	mov	ecx, DWORD PTR _nBtn$75421[ebp]
	add	ecx, 1
	mov	DWORD PTR _nBtn$75421[ebp], ecx
$L75422:
	mov	edx, DWORD PTR _nBtn$75421[ebp]
	cmp	edx, DWORD PTR _nBtnCount$75419[ebp]
	jge	$L75424

; 240  : 			UINT nID = toolbar.GetItemID(nBtn);

	mov	eax, DWORD PTR _nBtn$75421[ebp]
	push	eax
	lea	ecx, DWORD PTR _toolbar$75411[ebp]
	call	?GetItemID@CToolBar@@QBEIH@Z		; CToolBar::GetItemID
	mov	DWORD PTR _nID$75425[ebp], eax

; 241  : 
; 242  : 			if (sTip.LoadString(nID) && !sTip.IsEmpty())

	mov	ecx, DWORD PTR _nID$75425[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sTip$75420[ebp]
	call	?LoadStringW@CString@@QAEHI@Z		; CString::LoadStringW
	test	eax, eax
	je	$L75433
	lea	ecx, DWORD PTR _sTip$75420[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	$L75433

; 244  : 				// tip text starts at '\n' 
; 245  : 				int nStartTip = sTip.Find('\n');

	push	10					; 0000000aH
	lea	ecx, DWORD PTR _sTip$75420[ebp]
	call	?Find@CString@@QBEHG@Z			; CString::Find
	mov	DWORD PTR _nStartTip$75427[ebp], eax

; 246  : 				
; 247  : 				if (nStartTip != -1) 

	cmp	DWORD PTR _nStartTip$75427[ebp], -1
	je	SHORT $L75428

; 248  : 					sTip = sTip.Right(sTip.GetLength() - nStartTip - 1);

	lea	ecx, DWORD PTR _sTip$75420[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	sub	eax, DWORD PTR _nStartTip$75427[ebp]
	sub	eax, 1
	push	eax
	lea	edx, DWORD PTR $T76339[ebp]
	push	edx
	lea	ecx, DWORD PTR _sTip$75420[ebp]
	call	?Right@CString@@QBE?AV1@H@Z		; CString::Right
	mov	DWORD PTR -496+[ebp], eax
	mov	eax, DWORD PTR -496+[ebp]
	mov	DWORD PTR -500+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 8
	mov	ecx, DWORD PTR -500+[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sTip$75420[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	lea	ecx, DWORD PTR $T76339[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 249  : 				
; 250  : 				else // strip '...' if present

	jmp	SHORT $L75430
$L75428:

; 251  : 					sTip.Replace(_T("."), _T(""));

	push	OFFSET FLAT:$SG75431
	push	OFFSET FLAT:$SG75432
	lea	ecx, DWORD PTR _sTip$75420[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace
$L75430:

; 252  : 				
; 253  : 				sTip.TrimLeft();

	lea	ecx, DWORD PTR _sTip$75420[ebp]
	call	?TrimLeft@CString@@QAEXXZ		; CString::TrimLeft

; 254  : 				sTip.TrimRight();

	lea	ecx, DWORD PTR _sTip$75420[ebp]
	call	?TrimRight@CString@@QAEXXZ		; CString::TrimRight

; 255  : 
; 256  : 				if (!sTip.IsEmpty())

	lea	ecx, DWORD PTR _sTip$75420[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75433

; 258  : 					// create tooltip ctrl first time we need it
; 259  : 					if (tt.GetSafeHwnd() == NULL)

	lea	ecx, DWORD PTR _tt$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	test	eax, eax
	jne	SHORT $L75434

; 260  : 						tt.Create(pAppWnd, TTS_ALWAYSTIP);

	push	1
	mov	edx, DWORD PTR _pAppWnd$[ebp]
	push	edx
	lea	ecx, DWORD PTR _tt$[ebp]
	call	?Create@CToolTipCtrl@@QAEHPAVCWnd@@K@Z	; CToolTipCtrl::Create
$L75434:

; 261  : 					
; 262  : 					CLocalizer::TranslateText(sTip, tt);

	lea	ecx, DWORD PTR _tt$[ebp]
	call	??BCWnd@@QBEPAUHWND__@@XZ		; CWnd::operator HWND__ *
	push	eax
	lea	eax, DWORD PTR _sTip$75420[ebp]
	push	eax
	call	?TranslateText@CLocalizer@@SAHAAVCString@@PAUHWND__@@@Z ; CLocalizer::TranslateText
	add	esp, 8

; 263  : 
; 264  : 					// keep track of tooltips we've translated
; 265  : 					// so we don't re-handle them when we translate
; 266  : 					// the rest of the strings below
; 267  : 					lstTooltipIDs.AddTail(nID);

	mov	ecx, DWORD PTR _nID$75425[ebp]
	push	ecx
	lea	ecx, DWORD PTR _lstTooltipIDs$[ebp]
	call	?AddTail@?$CList@II@@QAEPAU__POSITION@@I@Z ; CList<unsigned int,unsigned int>::AddTail
$L75433:

; 270  : 		}

	jmp	$L75423
$L75424:

; 271  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR _sTip$75420[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 5
	lea	ecx, DWORD PTR _toolbar$75411[ebp]
	call	??1CToolBar@@UAE@XZ			; CToolBar::~CToolBar
	jmp	$L75401
$L75402:

; 272  : 
; 273  : 	// any left over strings but not AFX_IDS_
; 274  : 	CString sStr;

	lea	ecx, DWORD PTR _sStr$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 9

; 275  : 
; 276  : 	for (UINT nStr = nIDFirstStr; nStr <= nIDLastStr; nStr++)

	mov	edx, DWORD PTR _nIDFirstStr$[ebp]
	mov	DWORD PTR _nStr$[ebp], edx
	jmp	SHORT $L75437
$L75438:
	mov	eax, DWORD PTR _nStr$[ebp]
	add	eax, 1
	mov	DWORD PTR _nStr$[ebp], eax
$L75437:
	mov	ecx, DWORD PTR _nStr$[ebp]
	cmp	ecx, DWORD PTR _nIDLastStr$[ebp]
	ja	SHORT $L75439

; 280  : 			(nStr >= 0xE700 && nStr <= 0xE706))

	cmp	DWORD PTR _nStr$[ebp], 57344		; 0000e000H
	jb	SHORT $L75442
	cmp	DWORD PTR _nStr$[ebp], 57349		; 0000e005H
	jbe	SHORT $L75441
$L75442:
	cmp	DWORD PTR _nStr$[ebp], 59136		; 0000e700H
	jb	SHORT $L75440
	cmp	DWORD PTR _nStr$[ebp], 59142		; 0000e706H
	ja	SHORT $L75440
$L75441:

; 281  : 			continue;

	jmp	SHORT $L75438
$L75440:

; 282  : 
; 283  : 		// ignore previously translated tooltips
; 284  : 		if (lstTooltipIDs.Find(nStr))

	push	0
	mov	edx, DWORD PTR _nStr$[ebp]
	push	edx
	lea	ecx, DWORD PTR _lstTooltipIDs$[ebp]
	call	?Find@?$CList@II@@QBEPAU__POSITION@@IPAU2@@Z ; CList<unsigned int,unsigned int>::Find
	test	eax, eax
	je	SHORT $L75443

; 285  : 			continue;

	jmp	SHORT $L75438
$L75443:

; 286  : 
; 287  : 		if (sStr.LoadString(nStr) && !sStr.IsEmpty())

	mov	eax, DWORD PTR _nStr$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sStr$[ebp]
	call	?LoadStringW@CString@@QAEHI@Z		; CString::LoadStringW
	test	eax, eax
	je	SHORT $L75444
	lea	ecx, DWORD PTR _sStr$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75444

; 288  : 			CLocalizer::TranslateText(sStr);

	push	0
	lea	ecx, DWORD PTR _sStr$[ebp]
	push	ecx
	call	?TranslateText@CLocalizer@@SAHAAVCString@@PAUHWND__@@@Z ; CLocalizer::TranslateText
	add	esp, 8
$L75444:

; 289  : 	}

	jmp	SHORT $L75438
$L75439:

; 290  : }

	mov	BYTE PTR __$EHRec$[ebp+8], 5
	lea	ecx, DWORD PTR _sStr$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR _tt$[ebp]
	call	??1CToolTipCtrl@@UAE@XZ			; CToolTipCtrl::~CToolTipCtrl
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _lstTooltipIDs$[ebp]
	call	??1?$CList@II@@UAE@XZ			; CList<unsigned int,unsigned int>::~CList<unsigned int,unsigned int>
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _wait$[ebp]
	call	??1CWaitCursor@@QAE@XZ			; CWaitCursor::~CWaitCursor
$L75261:
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 500				; 000001f4H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76340:
	lea	ecx, DWORD PTR _wait$75281[ebp]
	call	??1CWaitCursor@@QAE@XZ			; CWaitCursor::~CWaitCursor
	ret	0
$L76341:
	lea	ecx, DWORD PTR _dialog$75282[ebp]
	call	??1CLocDialog@@UAE@XZ			; CLocDialog::~CLocDialog
	ret	0
$L76342:
	lea	ecx, DWORD PTR _wait$[ebp]
	call	??1CWaitCursor@@QAE@XZ			; CWaitCursor::~CWaitCursor
	ret	0
$L76343:
	lea	ecx, DWORD PTR _menu$75296[ebp]
	call	??1CMenu@@UAE@XZ			; CMenu::~CMenu
	ret	0
$L76344:
	lea	ecx, DWORD PTR _lstTooltipIDs$[ebp]
	call	??1?$CList@II@@UAE@XZ			; CList<unsigned int,unsigned int>::~CList<unsigned int,unsigned int>
	ret	0
$L76345:
	lea	ecx, DWORD PTR _tt$[ebp]
	call	??1CToolTipCtrl@@UAE@XZ			; CToolTipCtrl::~CToolTipCtrl
	ret	0
$L76346:
	lea	ecx, DWORD PTR _toolbar$75411[ebp]
	call	??1CToolBar@@UAE@XZ			; CToolBar::~CToolBar
	ret	0
$L76347:
	lea	ecx, DWORD PTR _sTip$75420[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76348:
	lea	ecx, DWORD PTR $T76339[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76349:
	lea	ecx, DWORD PTR _sStr$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76352:
	mov	eax, OFFSET FLAT:$T76351
	jmp	___CxxFrameHandler
text$x	ENDS
?ForceTranslateAllUIElements@CLocalizer@@SAXII@Z ENDP	; CLocalizer::ForceTranslateAllUIElements
PUBLIC	?OnInitDialog@CLocDialog@@UAEHXZ		; CLocDialog::OnInitDialog
PUBLIC	??_7CLocDialog@@6B@				; CLocDialog::`vftable'
PUBLIC	??_GCLocDialog@@UAEPAXI@Z			; CLocDialog::`scalar deleting destructor'
PUBLIC	??_ECLocDialog@@UAEPAXI@Z			; CLocDialog::`vector deleting destructor'
EXTRN	?GetRuntimeClass@CDialog@@UBEPAUCRuntimeClass@@XZ:NEAR ; CDialog::GetRuntimeClass
EXTRN	?PreSubclassWindow@CWnd@@UAEXXZ:NEAR		; CWnd::PreSubclassWindow
EXTRN	?Create@CWnd@@UAEHPBG0KABUtagRECT@@PAV1@IPAUCCreateContext@@@Z:NEAR ; CWnd::Create
EXTRN	??0CDialog@@QAE@PBGPAVCWnd@@@Z:NEAR		; CDialog::CDialog
EXTRN	?OnSetFont@CDialog@@UAEXPAVCFont@@@Z:NEAR	; CDialog::OnSetFont
EXTRN	?OnOK@CDialog@@MAEXXZ:NEAR			; CDialog::OnOK
EXTRN	?OnCancel@CDialog@@MAEXXZ:NEAR			; CDialog::OnCancel
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
EXTRN	?OnToolHitTest@CWnd@@UBEHVCPoint@@PAUtagTOOLINFOW@@@Z:NEAR ; CWnd::OnToolHitTest
EXTRN	?GetScrollBarCtrl@CWnd@@UBEPAVCScrollBar@@H@Z:NEAR ; CWnd::GetScrollBarCtrl
EXTRN	?WinHelpW@CWnd@@UAEXKI@Z:NEAR			; CWnd::WinHelpW
EXTRN	?ContinueModal@CWnd@@UAEHXZ:NEAR		; CWnd::ContinueModal
EXTRN	?EndModalLoop@CWnd@@UAEXH@Z:NEAR		; CWnd::EndModalLoop
EXTRN	?OnCommand@CWnd@@MAEHIJ@Z:NEAR			; CWnd::OnCommand
EXTRN	?OnNotify@CWnd@@MAEHIJPAJ@Z:NEAR		; CWnd::OnNotify
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
;	COMDAT ??_7CLocDialog@@6B@
; File D:\_CODE\Shared\Localizer.cpp
CONST	SEGMENT
??_7CLocDialog@@6B@ DD FLAT:?GetRuntimeClass@CDialog@@UBEPAUCRuntimeClass@@XZ ; CLocDialog::`vftable'
	DD	FLAT:??_ECLocDialog@@UAEPAXI@Z
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
	DD	FLAT:?GetMessageMap@CLocDialog@@MBEPBUAFX_MSGMAP@@XZ
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
	DD	FLAT:?DoModal@CDialog@@UAEHXZ
	DD	FLAT:?OnInitDialog@CLocDialog@@UAEHXZ
	DD	FLAT:?OnSetFont@CDialog@@UAEXPAVCFont@@@Z
	DD	FLAT:?OnOK@CDialog@@MAEXXZ
	DD	FLAT:?OnCancel@CDialog@@MAEXXZ
	DD	FLAT:?PreInitDialog@CDialog@@MAEXXZ
CONST	ENDS
;	COMDAT ??0CLocDialog@@QAE@PBGPAVCWnd@@@Z
_TEXT	SEGMENT
_szIDTemplate$ = 8
_pParent$ = 12
_this$ = -4
??0CLocDialog@@QAE@PBGPAVCWnd@@@Z PROC NEAR		; CLocDialog::CLocDialog, COMDAT

; 24   : 	CLocDialog(LPCTSTR szIDTemplate, CWnd* pParent = NULL) : CDialog(szIDTemplate, pParent) {}

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _pParent$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szIDTemplate$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CDialog@@QAE@PBGPAVCWnd@@@Z		; CDialog::CDialog
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7CLocDialog@@6B@ ; CLocDialog::`vftable'
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
??0CLocDialog@@QAE@PBGPAVCWnd@@@Z ENDP			; CLocDialog::CLocDialog
_TEXT	ENDS
EXTRN	?OnInitDialog@CDialog@@UAEHXZ:NEAR		; CDialog::OnInitDialog
EXTRN	?MoveWindow@CWnd@@QAEXPBUtagRECT@@H@Z:NEAR	; CWnd::MoveWindow
EXTRN	?GetWindowRect@CWnd@@QBEXPAUtagRECT@@@Z:NEAR	; CWnd::GetWindowRect
EXTRN	??0CRect@@QAE@XZ:NEAR				; CRect::CRect
EXTRN	??BCRect@@QAEPAUtagRECT@@XZ:NEAR		; CRect::operator tagRECT *
EXTRN	?OffsetRect@CRect@@QAEXHH@Z:NEAR		; CRect::OffsetRect
;	COMDAT ?OnInitDialog@CLocDialog@@UAEHXZ
_TEXT	SEGMENT
_this$ = -20
_rPos$ = -16
?OnInitDialog@CLocDialog@@UAEHXZ PROC NEAR		; CLocDialog::OnInitDialog, COMDAT

; 27   : 	{

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

; 28   : 		CDialog::OnInitDialog();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnInitDialog@CDialog@@UAEHXZ		; CDialog::OnInitDialog

; 29   : 
; 30   : 		// move off screen before showing
; 31   : 		CRect rPos;

	lea	ecx, DWORD PTR _rPos$[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 32   : 		GetWindowRect(rPos);

	lea	ecx, DWORD PTR _rPos$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetWindowRect@CWnd@@QBEXPAUtagRECT@@@Z	; CWnd::GetWindowRect

; 33   : 		rPos.OffsetRect(-30000, -30000);

	push	-30000					; ffff8ad0H
	push	-30000					; ffff8ad0H
	lea	ecx, DWORD PTR _rPos$[ebp]
	call	?OffsetRect@CRect@@QAEXHH@Z		; CRect::OffsetRect

; 34   : 		MoveWindow(rPos, FALSE);

	push	0
	lea	ecx, DWORD PTR _rPos$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?MoveWindow@CWnd@@QAEXPBUtagRECT@@H@Z	; CWnd::MoveWindow

; 35   : 
; 36   : 		return TRUE;

	mov	eax, 1

; 37   : 	}

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?OnInitDialog@CLocDialog@@UAEHXZ ENDP			; CLocDialog::OnInitDialog
_TEXT	ENDS
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_GCLocDialog@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCLocDialog@@UAEPAXI@Z PROC NEAR			; CLocDialog::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CLocDialog@@UAE@XZ			; CLocDialog::~CLocDialog
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75183
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L75183:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCLocDialog@@UAEPAXI@Z ENDP				; CLocDialog::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	??1CDialog@@UAE@XZ:NEAR				; CDialog::~CDialog
;	COMDAT ??1CLocDialog@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CLocDialog@@UAE@XZ PROC NEAR				; CLocDialog::~CLocDialog, COMDAT
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
??1CLocDialog@@UAE@XZ ENDP				; CLocDialog::~CLocDialog
_TEXT	ENDS
EXTRN	?TranslateString@CEnString@@SAHAAVCString@@PAUHWND__@@@Z:NEAR ; CEnString::TranslateString
_BSS	SEGMENT
	ALIGN	4

$SG75453 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT ?TranslateText@CLocalizer@@SAHAAVCString@@PAUHWND__@@@Z
_TEXT	SEGMENT
_sText$ = 8
_hWndRef$ = 12
?TranslateText@CLocalizer@@SAHAAVCString@@PAUHWND__@@@Z PROC NEAR ; CLocalizer::TranslateText, COMDAT

; 293  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L75449:

; 294  : 	VERIFY (Initialize());

	push	0
	push	OFFSET FLAT:$SG75453
	call	?Initialize@CLocalizer@@SAHPBGW4ITT_TRANSLATEOPTION@@@Z ; CLocalizer::Initialize
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75452
	push	294					; 00000126H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75452
	int	3
$L75452:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75449

; 295  : 
; 296  : 	return CEnString::TranslateString(sText, hWndRef);

	mov	ecx, DWORD PTR _hWndRef$[ebp]
	push	ecx
	mov	edx, DWORD PTR _sText$[ebp]
	push	edx
	call	?TranslateString@CEnString@@SAHAAVCString@@PAUHWND__@@@Z ; CEnString::TranslateString
	add	esp, 8

; 297  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?TranslateText@CLocalizer@@SAHAAVCString@@PAUHWND__@@@Z ENDP ; CLocalizer::TranslateText
_TEXT	ENDS
_BSS	SEGMENT
	ALIGN	4

$SG75463 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT ?TranslateMenu@CLocalizer@@SAHPAUHMENU__@@PAUHWND__@@H@Z
_TEXT	SEGMENT
_hMenu$ = 8
_hWndRef$ = 12
_bRecursive$ = 16
?TranslateMenu@CLocalizer@@SAHPAUHMENU__@@PAUHWND__@@H@Z PROC NEAR ; CLocalizer::TranslateMenu, COMDAT

; 300  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L75459:

; 301  : 	VERIFY (Initialize());

	push	0
	push	OFFSET FLAT:$SG75463
	call	?Initialize@CLocalizer@@SAHPBGW4ITT_TRANSLATEOPTION@@@Z ; CLocalizer::Initialize
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75462
	push	301					; 0000012dH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75462
	int	3
$L75462:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75459

; 302  : 
; 303  : 	if (s_pTransText)

	cmp	DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A, 0 ; CLocalizer::s_pTransText
	je	SHORT $L75464

; 304  : 		return s_pTransText->TranslateMenu(hMenu, hWndRef, bRecursive);

	mov	esi, esp
	mov	ecx, DWORD PTR _bRecursive$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hWndRef$[ebp]
	push	edx
	mov	eax, DWORD PTR _hMenu$[ebp]
	push	eax
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	call	DWORD PTR [edx+36]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75458
$L75464:

; 305  : 
; 306  : 	return FALSE;

	xor	eax, eax
$L75458:

; 307  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?TranslateMenu@CLocalizer@@SAHPAUHMENU__@@PAUHWND__@@H@Z ENDP ; CLocalizer::TranslateMenu
_TEXT	ENDS
PUBLIC	?UpdateMenu@CLocalizer@@SAXPAUHWND__@@@Z	; CLocalizer::UpdateMenu
_BSS	SEGMENT
	ALIGN	4

$SG75472 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT ?UpdateMenu@CLocalizer@@SAXPAUHWND__@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
?UpdateMenu@CLocalizer@@SAXPAUHWND__@@@Z PROC NEAR	; CLocalizer::UpdateMenu, COMDAT

; 310  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L75468:

; 311  : 	VERIFY (Initialize());

	push	0
	push	OFFSET FLAT:$SG75472
	call	?Initialize@CLocalizer@@SAHPBGW4ITT_TRANSLATEOPTION@@@Z ; CLocalizer::Initialize
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75471
	push	311					; 00000137H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75471
	int	3
$L75471:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75468

; 312  : 
; 313  : 	if (s_pTransText)

	cmp	DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A, 0 ; CLocalizer::s_pTransText
	je	SHORT $L75473

; 314  : 		s_pTransText->UpdateMenu(hWnd);

	mov	esi, esp
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	mov	edx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	call	DWORD PTR [eax+40]
	cmp	esi, esp
	call	__chkesp
$L75473:

; 315  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?UpdateMenu@CLocalizer@@SAXPAUHWND__@@@Z ENDP		; CLocalizer::UpdateMenu
_TEXT	ENDS
PUBLIC	?EnableTranslation@CLocalizer@@SAXPAUHWND__@@H@Z ; CLocalizer::EnableTranslation
_BSS	SEGMENT
	ALIGN	4

$SG75482 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT ?EnableTranslation@CLocalizer@@SAXPAUHWND__@@H@Z
_TEXT	SEGMENT
_hWnd$ = 8
_bEnable$ = 12
?EnableTranslation@CLocalizer@@SAXPAUHWND__@@H@Z PROC NEAR ; CLocalizer::EnableTranslation, COMDAT

; 318  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L75478:

; 319  : 	VERIFY (Initialize());

	push	0
	push	OFFSET FLAT:$SG75482
	call	?Initialize@CLocalizer@@SAHPBGW4ITT_TRANSLATEOPTION@@@Z ; CLocalizer::Initialize
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75481
	push	319					; 0000013fH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75481
	int	3
$L75481:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75478

; 320  : 
; 321  : 	if (s_pTransText)

	cmp	DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A, 0 ; CLocalizer::s_pTransText
	je	SHORT $L75483

; 322  : 		s_pTransText->EnableTranslation(hWnd, bEnable);

	mov	esi, esp
	mov	ecx, DWORD PTR _bEnable$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	mov	eax, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	call	DWORD PTR [edx+52]
	cmp	esi, esp
	call	__chkesp
$L75483:

; 323  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?EnableTranslation@CLocalizer@@SAXPAUHWND__@@H@Z ENDP	; CLocalizer::EnableTranslation
_TEXT	ENDS
PUBLIC	?EnableTranslation@CLocalizer@@SAXABVCWnd@@H@Z	; CLocalizer::EnableTranslation
_BSS	SEGMENT
	ALIGN	4

$SG75492 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT ?EnableTranslation@CLocalizer@@SAXABVCWnd@@H@Z
_TEXT	SEGMENT
_hWnd$ = 8
_bEnable$ = 12
?EnableTranslation@CLocalizer@@SAXABVCWnd@@H@Z PROC NEAR ; CLocalizer::EnableTranslation, COMDAT

; 326  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L75488:

; 327  : 	VERIFY (Initialize());

	push	0
	push	OFFSET FLAT:$SG75492
	call	?Initialize@CLocalizer@@SAHPBGW4ITT_TRANSLATEOPTION@@@Z ; CLocalizer::Initialize
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75491
	push	327					; 00000147H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75491
	int	3
$L75491:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75488

; 328  : 
; 329  : 	if (s_pTransText)

	cmp	DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A, 0 ; CLocalizer::s_pTransText
	je	SHORT $L75493

; 330  : 		s_pTransText->EnableTranslation(hWnd.GetSafeHwnd(), bEnable);

	mov	esi, esp
	mov	ecx, DWORD PTR _bEnable$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _hWnd$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	push	eax
	mov	edx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	call	DWORD PTR [eax+52]
	cmp	esi, esp
	call	__chkesp
$L75493:

; 331  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?EnableTranslation@CLocalizer@@SAXABVCWnd@@H@Z ENDP	; CLocalizer::EnableTranslation
_TEXT	ENDS
PUBLIC	?EnableTranslation@CLocalizer@@SAXPAUHMENU__@@H@Z ; CLocalizer::EnableTranslation
_BSS	SEGMENT
	ALIGN	4

$SG75502 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT ?EnableTranslation@CLocalizer@@SAXPAUHMENU__@@H@Z
_TEXT	SEGMENT
_hMenu$ = 8
_bEnable$ = 12
?EnableTranslation@CLocalizer@@SAXPAUHMENU__@@H@Z PROC NEAR ; CLocalizer::EnableTranslation, COMDAT

; 334  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L75498:

; 335  : 	VERIFY (Initialize());

	push	0
	push	OFFSET FLAT:$SG75502
	call	?Initialize@CLocalizer@@SAHPBGW4ITT_TRANSLATEOPTION@@@Z ; CLocalizer::Initialize
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75501
	push	335					; 0000014fH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75501
	int	3
$L75501:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75498

; 336  : 
; 337  : 	if (s_pTransText)

	cmp	DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A, 0 ; CLocalizer::s_pTransText
	je	SHORT $L75503

; 338  : 		s_pTransText->EnableTranslation(hMenu, bEnable);

	mov	esi, esp
	mov	ecx, DWORD PTR _bEnable$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hMenu$[ebp]
	push	edx
	mov	eax, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	call	DWORD PTR [edx+48]
	cmp	esi, esp
	call	__chkesp
$L75503:

; 339  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?EnableTranslation@CLocalizer@@SAXPAUHMENU__@@H@Z ENDP	; CLocalizer::EnableTranslation
_TEXT	ENDS
PUBLIC	?EnableTranslation@CLocalizer@@SAXIH@Z		; CLocalizer::EnableTranslation
_BSS	SEGMENT
	ALIGN	4

$SG75512 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT ?EnableTranslation@CLocalizer@@SAXIH@Z
_TEXT	SEGMENT
_nMenuID$ = 8
_bEnable$ = 12
?EnableTranslation@CLocalizer@@SAXIH@Z PROC NEAR	; CLocalizer::EnableTranslation, COMDAT

; 342  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L75508:

; 343  : 	VERIFY (Initialize());

	push	0
	push	OFFSET FLAT:$SG75512
	call	?Initialize@CLocalizer@@SAHPBGW4ITT_TRANSLATEOPTION@@@Z ; CLocalizer::Initialize
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75511
	push	343					; 00000157H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75511
	int	3
$L75511:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75508

; 344  : 
; 345  : 	if (s_pTransText)

	cmp	DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A, 0 ; CLocalizer::s_pTransText
	je	SHORT $L75513

; 346  : 		s_pTransText->EnableTranslation(nMenuID, bEnable);

	mov	esi, esp
	mov	ecx, DWORD PTR _bEnable$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nMenuID$[ebp]
	push	edx
	mov	eax, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	call	DWORD PTR [edx+44]
	cmp	esi, esp
	call	__chkesp
$L75513:

; 347  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?EnableTranslation@CLocalizer@@SAXIH@Z ENDP		; CLocalizer::EnableTranslation
_TEXT	ENDS
PUBLIC	?EnableTranslation@CLocalizer@@SAXIIH@Z		; CLocalizer::EnableTranslation
_BSS	SEGMENT
	ALIGN	4

$SG75523 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT ?EnableTranslation@CLocalizer@@SAXIIH@Z
_TEXT	SEGMENT
_nMenuIDFirst$ = 8
_nMenuIDLast$ = 12
_bEnable$ = 16
_nMenuID$75525 = -4
?EnableTranslation@CLocalizer@@SAXIIH@Z PROC NEAR	; CLocalizer::EnableTranslation, COMDAT

; 350  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L75519:

; 351  : 	VERIFY (Initialize());

	push	0
	push	OFFSET FLAT:$SG75523
	call	?Initialize@CLocalizer@@SAHPBGW4ITT_TRANSLATEOPTION@@@Z ; CLocalizer::Initialize
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75522
	push	351					; 0000015fH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75522
	int	3
$L75522:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75519

; 352  : 
; 353  : 	if (s_pTransText)

	cmp	DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A, 0 ; CLocalizer::s_pTransText
	je	SHORT $L75528

; 355  : 		for (UINT nMenuID = nMenuIDFirst; nMenuID <= nMenuIDLast; nMenuID++)

	mov	ecx, DWORD PTR _nMenuIDFirst$[ebp]
	mov	DWORD PTR _nMenuID$75525[ebp], ecx
	jmp	SHORT $L75526
$L75527:
	mov	edx, DWORD PTR _nMenuID$75525[ebp]
	add	edx, 1
	mov	DWORD PTR _nMenuID$75525[ebp], edx
$L75526:
	mov	eax, DWORD PTR _nMenuID$75525[ebp]
	cmp	eax, DWORD PTR _nMenuIDLast$[ebp]
	ja	SHORT $L75528

; 356  : 			s_pTransText->EnableTranslation(nMenuID, bEnable);

	mov	esi, esp
	mov	ecx, DWORD PTR _bEnable$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nMenuID$75525[ebp]
	push	edx
	mov	eax, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	call	DWORD PTR [edx+44]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75527
$L75528:

; 358  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?EnableTranslation@CLocalizer@@SAXIIH@Z ENDP		; CLocalizer::EnableTranslation
_TEXT	ENDS
PUBLIC	?IgnoreString@CLocalizer@@SAXABVCString@@@Z	; CLocalizer::IgnoreString
_BSS	SEGMENT
	ALIGN	4

$SG75536 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT ?IgnoreString@CLocalizer@@SAXABVCString@@@Z
_TEXT	SEGMENT
_sText$ = 8
?IgnoreString@CLocalizer@@SAXABVCString@@@Z PROC NEAR	; CLocalizer::IgnoreString, COMDAT

; 362  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L75532:

; 363  : 	VERIFY (Initialize());

	push	0
	push	OFFSET FLAT:$SG75536
	call	?Initialize@CLocalizer@@SAHPBGW4ITT_TRANSLATEOPTION@@@Z ; CLocalizer::Initialize
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75535
	push	363					; 0000016bH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75535
	int	3
$L75535:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75532

; 364  : 
; 365  : 	if (s_pTransText)

	cmp	DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A, 0 ; CLocalizer::s_pTransText
	je	SHORT $L75537

; 366  : 		s_pTransText->IgnoreString(sText);

	mov	ecx, DWORD PTR _sText$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR ?s_pTransText@CLocalizer@@1PAVITransText@@A ; CLocalizer::s_pTransText
	call	DWORD PTR [edx+56]
	cmp	esi, esp
	call	__chkesp
$L75537:

; 367  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?IgnoreString@CLocalizer@@SAXABVCString@@@Z ENDP	; CLocalizer::IgnoreString
_TEXT	ENDS
PUBLIC	?Serialize@?$CList@II@@UAEXAAVCArchive@@@Z	; CList<unsigned int,unsigned int>::Serialize
PUBLIC	?Dump@?$CList@II@@UBEXAAVCDumpContext@@@Z	; CList<unsigned int,unsigned int>::Dump
PUBLIC	?AssertValid@?$CList@II@@UBEXXZ			; CList<unsigned int,unsigned int>::AssertValid
PUBLIC	??_7?$CList@II@@6B@				; CList<unsigned int,unsigned int>::`vftable'
PUBLIC	??_G?$CList@II@@UAEPAXI@Z			; CList<unsigned int,unsigned int>::`scalar deleting destructor'
PUBLIC	??_E?$CList@II@@UAEPAXI@Z			; CList<unsigned int,unsigned int>::`vector deleting destructor'
EXTRN	?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ:NEAR ; CObject::GetRuntimeClass
EXTRN	??1CObject@@UAE@XZ:NEAR				; CObject::~CObject
EXTRN	??0CObject@@IAE@XZ:NEAR				; CObject::CObject
;	COMDAT ??_7?$CList@II@@6B@
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
CONST	SEGMENT
??_7?$CList@II@@6B@ DD FLAT:?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ ; CList<unsigned int,unsigned int>::`vftable'
	DD	FLAT:??_E?$CList@II@@UAEPAXI@Z
	DD	FLAT:?Serialize@?$CList@II@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@?$CList@II@@UBEXXZ
	DD	FLAT:?Dump@?$CList@II@@UBEXAAVCDumpContext@@@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76389	DD	019930520H
	DD	01H
	DD	FLAT:$T76391
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76391	DD	0ffffffffH
	DD	FLAT:$L76387
xdata$x	ENDS
;	COMDAT ??0?$CList@II@@QAE@H@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_nBlockSize$ = 8
_this$ = -16
??0?$CList@II@@QAE@H@Z PROC NEAR			; CList<unsigned int,unsigned int>::CList<unsigned int,unsigned int>, COMDAT

; 728  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76390
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
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CList@II@@6B@ ; CList<unsigned int,unsigned int>::`vftable'
$L75719:

; 729  : 	ASSERT(nBlockSize > 0);

	cmp	DWORD PTR _nBlockSize$[ebp], 0
	jg	SHORT $L75722
	push	729					; 000002d9H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75722
	int	3
$L75722:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75719

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
$L76387:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L76390:
	mov	eax, OFFSET FLAT:$T76389
	jmp	___CxxFrameHandler
text$x	ENDS
??0?$CList@II@@QAE@H@Z ENDP				; CList<unsigned int,unsigned int>::CList<unsigned int,unsigned int>
PUBLIC	?NewNode@?$CList@II@@IAEPAUCNode@1@PAU21@0@Z	; CList<unsigned int,unsigned int>::NewNode
EXTRN	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z:NEAR ; AfxAssertValidObject
;	COMDAT ?AddTail@?$CList@II@@QAEPAU__POSITION@@I@Z
_TEXT	SEGMENT
_newElement$ = 8
_this$ = -8
_pNewNode$ = -4
?AddTail@?$CList@II@@QAEPAU__POSITION@@I@Z PROC NEAR	; CList<unsigned int,unsigned int>::AddTail, COMDAT

; 839  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
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
	call	?NewNode@?$CList@II@@IAEPAUCNode@1@PAU21@0@Z ; CList<unsigned int,unsigned int>::NewNode
	mov	DWORD PTR _pNewNode$[ebp], eax

; 843  : 	pNewNode->data = newElement;

	mov	eax, DWORD PTR _pNewNode$[ebp]
	mov	ecx, DWORD PTR _newElement$[ebp]
	mov	DWORD PTR [eax+8], ecx

; 844  : 	if (m_pNodeTail != NULL)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+8], 0
	je	SHORT $L75728

; 845  : 		m_pNodeTail->pNext = pNewNode;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	edx, DWORD PTR _pNewNode$[ebp]
	mov	DWORD PTR [ecx], edx

; 846  : 	else

	jmp	SHORT $L75729
$L75728:

; 847  : 		m_pNodeHead = pNewNode;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _pNewNode$[ebp]
	mov	DWORD PTR [eax+4], ecx
$L75729:

; 848  : 	m_pNodeTail = pNewNode;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _pNewNode$[ebp]
	mov	DWORD PTR [edx+8], eax

; 849  : 	return (POSITION) pNewNode;

	mov	eax, DWORD PTR _pNewNode$[ebp]

; 850  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?AddTail@?$CList@II@@QAEPAU__POSITION@@I@Z ENDP		; CList<unsigned int,unsigned int>::AddTail
_TEXT	ENDS
PUBLIC	?CompareElements@@YGHPBI0@Z			; CompareElements
EXTRN	?AfxIsValidAddress@@YGHPBXIH@Z:NEAR		; AfxIsValidAddress
;	COMDAT ?Find@?$CList@II@@QBEPAU__POSITION@@IPAU2@@Z
_TEXT	SEGMENT
_searchValue$ = 8
_startAfter$ = 12
_this$ = -8
_pNode$ = -4
?Find@?$CList@II@@QBEPAU__POSITION@@IPAU2@@Z PROC NEAR	; CList<unsigned int,unsigned int>::Find, COMDAT

; 1021 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1022 : 	ASSERT_VALID(this);

	push	1022					; 000003feH
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1023 : 
; 1024 : 	CNode* pNode = (CNode*) startAfter;

	mov	ecx, DWORD PTR _startAfter$[ebp]
	mov	DWORD PTR _pNode$[ebp], ecx

; 1025 : 	if (pNode == NULL)

	cmp	DWORD PTR _pNode$[ebp], 0
	jne	SHORT $L75738

; 1027 : 		pNode = m_pNodeHead;  // start at head

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR _pNode$[ebp], eax

; 1029 : 	else

	jmp	SHORT $L75739
$L75738:

; 1031 : 		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));

	push	1
	push	12					; 0000000cH
	mov	ecx, DWORD PTR _pNode$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L75744
	push	1031					; 00000407H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75744
	int	3
$L75744:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75738

; 1032 : 		pNode = pNode->pNext;  // start after the one specified

	mov	eax, DWORD PTR _pNode$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pNode$[ebp], ecx
$L75739:

; 1034 : 
; 1035 : 	for (; pNode != NULL; pNode = pNode->pNext)

	jmp	SHORT $L75745
$L75746:
	mov	edx, DWORD PTR _pNode$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _pNode$[ebp], eax
$L75745:
	cmp	DWORD PTR _pNode$[ebp], 0
	je	SHORT $L75747

; 1036 : 		if (CompareElements<TYPE>(&pNode->data, &searchValue))

	lea	ecx, DWORD PTR _searchValue$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pNode$[ebp]
	add	edx, 8
	push	edx
	call	?CompareElements@@YGHPBI0@Z		; CompareElements
	test	eax, eax
	je	SHORT $L75754

; 1037 : 			return (POSITION)pNode;

	mov	eax, DWORD PTR _pNode$[ebp]
	jmp	SHORT $L75735
$L75754:

; 1038 : 	return NULL;

	jmp	SHORT $L75746
$L75747:
	xor	eax, eax
$L75735:

; 1039 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Find@?$CList@II@@QBEPAU__POSITION@@IPAU2@@Z ENDP	; CList<unsigned int,unsigned int>::Find
_TEXT	ENDS
PUBLIC	?RemoveAll@?$CList@II@@QAEXXZ			; CList<unsigned int,unsigned int>::RemoveAll
;	COMDAT xdata$x
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
xdata$x	SEGMENT
$T76402	DD	019930520H
	DD	01H
	DD	FLAT:$T76404
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76404	DD	0ffffffffH
	DD	FLAT:$L76400
xdata$x	ENDS
;	COMDAT ??1?$CList@II@@UAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??1?$CList@II@@UAE@XZ PROC NEAR				; CList<unsigned int,unsigned int>::~CList<unsigned int,unsigned int>, COMDAT

; 755  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76403
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
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CList@II@@6B@ ; CList<unsigned int,unsigned int>::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 756  : 	RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CList@II@@QAEXXZ		; CList<unsigned int,unsigned int>::RemoveAll
$L75759:

; 757  : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L75762
	push	757					; 000002f5H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75762
	int	3
$L75762:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75759

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
$L76400:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L76403:
	mov	eax, OFFSET FLAT:$T76402
	jmp	___CxxFrameHandler
text$x	ENDS
??1?$CList@II@@UAE@XZ ENDP				; CList<unsigned int,unsigned int>::~CList<unsigned int,unsigned int>
PUBLIC	?SerializeElements@@YGXAAVCArchive@@PAIH@Z	; SerializeElements
EXTRN	?ReadCount@CArchive@@QAEKXZ:NEAR		; CArchive::ReadCount
EXTRN	?WriteCount@CArchive@@QAEXK@Z:NEAR		; CArchive::WriteCount
EXTRN	?IsStoring@CArchive@@QBEHXZ:NEAR		; CArchive::IsStoring
;	COMDAT ?Serialize@?$CList@II@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT
_nNewCount$75783 = -8
_newData$75787 = -12
_ar$ = 8
_this$ = -16
_pNode$75768 = -4
?Serialize@?$CList@II@@UAEXAAVCArchive@@@Z PROC NEAR	; CList<unsigned int,unsigned int>::Serialize, COMDAT

; 1043 : {

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
	je	SHORT $L75767

; 1050 : 		ar.WriteCount(m_nCount);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?WriteCount@CArchive@@QAEXK@Z		; CArchive::WriteCount

; 1051 : 		for (CNode* pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR _pNode$75768[ebp], edx
	jmp	SHORT $L75769
$L75770:
	mov	eax, DWORD PTR _pNode$75768[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pNode$75768[ebp], ecx
$L75769:
	cmp	DWORD PTR _pNode$75768[ebp], 0
	je	SHORT $L75771
$L75772:

; 1053 : 			ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));

	push	1
	push	12					; 0000000cH
	mov	edx, DWORD PTR _pNode$75768[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L75776
	push	1053					; 0000041dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75776
	int	3
$L75776:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75772

; 1054 : 			SerializeElements<TYPE>(ar, &pNode->data, 1);

	push	1
	mov	ecx, DWORD PTR _pNode$75768[ebp]
	add	ecx, 8
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAIH@Z ; SerializeElements

; 1055 : 		}

	jmp	SHORT $L75770
$L75771:

; 1057 : 	else

	jmp	SHORT $L75786
$L75767:

; 1059 : 		DWORD nNewCount = ar.ReadCount();

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?ReadCount@CArchive@@QAEKXZ		; CArchive::ReadCount
	mov	DWORD PTR _nNewCount$75783[ebp], eax
$L75785:

; 1060 : 		while (nNewCount--)

	mov	eax, DWORD PTR _nNewCount$75783[ebp]
	mov	ecx, DWORD PTR _nNewCount$75783[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nNewCount$75783[ebp], ecx
	test	eax, eax
	je	SHORT $L75786

; 1062 : 			TYPE newData;
; 1063 : 			SerializeElements<TYPE>(ar, &newData, 1);

	push	1
	lea	edx, DWORD PTR _newData$75787[ebp]
	push	edx
	mov	eax, DWORD PTR _ar$[ebp]
	push	eax
	call	?SerializeElements@@YGXAAVCArchive@@PAIH@Z ; SerializeElements

; 1064 : 			AddTail(newData);

	mov	ecx, DWORD PTR _newData$75787[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AddTail@?$CList@II@@QAEPAU__POSITION@@I@Z ; CList<unsigned int,unsigned int>::AddTail

; 1065 : 		}

	jmp	SHORT $L75785
$L75786:

; 1067 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Serialize@?$CList@II@@UAEXAAVCArchive@@@Z ENDP		; CList<unsigned int,unsigned int>::Serialize
_TEXT	ENDS
PUBLIC	?GetHeadPosition@?$CList@II@@QBEPAU__POSITION@@XZ ; CList<unsigned int,unsigned int>::GetHeadPosition
PUBLIC	?GetNext@?$CList@II@@QAEAAIAAPAU__POSITION@@@Z	; CList<unsigned int,unsigned int>::GetNext
PUBLIC	??_C@_05KIFI@with?5?$AA@			; `string'
PUBLIC	??_C@_09LOPF@?5elements?$AA@			; `string'
PUBLIC	?DumpElements@@YGXAAVCDumpContext@@PBIH@Z	; DumpElements
PUBLIC	??_C@_01BJG@?6?$AA@				; `string'
EXTRN	?Dump@CObject@@UBEXAAVCDumpContext@@@Z:NEAR	; CObject::Dump
EXTRN	?GetDepth@CDumpContext@@QBEHXZ:NEAR		; CDumpContext::GetDepth
EXTRN	??6CDumpContext@@QAEAAV0@PBD@Z:NEAR		; CDumpContext::operator<<
EXTRN	??6CDumpContext@@QAEAAV0@H@Z:NEAR		; CDumpContext::operator<<
;	COMDAT ??_C@_05KIFI@with?5?$AA@
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
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
;	COMDAT ?Dump@?$CList@II@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT
_dc$ = 8
_this$ = -8
_pos$75798 = -4
?Dump@?$CList@II@@UBEXAAVCDumpContext@@@Z PROC NEAR	; CList<unsigned int,unsigned int>::Dump, COMDAT

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
	jle	SHORT $L75801

; 1078 : 		POSITION pos = GetHeadPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetHeadPosition@?$CList@II@@QBEPAU__POSITION@@XZ ; CList<unsigned int,unsigned int>::GetHeadPosition
	mov	DWORD PTR _pos$75798[ebp], eax
$L75800:

; 1079 : 		while (pos != NULL)

	cmp	DWORD PTR _pos$75798[ebp], 0
	je	SHORT $L75801

; 1081 : 			dc << "\n";

	push	OFFSET FLAT:??_C@_01BJG@?6?$AA@		; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1082 : 			DumpElements<TYPE>(dc, &((CList*)this)->GetNext(pos), 1);

	push	1
	lea	eax, DWORD PTR _pos$75798[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetNext@?$CList@II@@QAEAAIAAPAU__POSITION@@@Z ; CList<unsigned int,unsigned int>::GetNext
	push	eax
	mov	ecx, DWORD PTR _dc$[ebp]
	push	ecx
	call	?DumpElements@@YGXAAVCDumpContext@@PBIH@Z ; DumpElements

; 1083 : 		}

	jmp	SHORT $L75800
$L75801:

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
?Dump@?$CList@II@@UBEXAAVCDumpContext@@@Z ENDP		; CList<unsigned int,unsigned int>::Dump
_TEXT	ENDS
EXTRN	?AssertValid@CObject@@UBEXXZ:NEAR		; CObject::AssertValid
;	COMDAT ?AssertValid@?$CList@II@@UBEXXZ
_TEXT	SEGMENT
_this$ = -4
?AssertValid@?$CList@II@@UBEXXZ PROC NEAR		; CList<unsigned int,unsigned int>::AssertValid, COMDAT

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
	jne	SHORT $L75811
$L75812:

; 1096 : 		// empty list
; 1097 : 		ASSERT(m_pNodeHead == NULL);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L75815
	push	1097					; 00000449H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75815
	int	3
$L75815:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75812
$L75814:

; 1098 : 		ASSERT(m_pNodeTail == NULL);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	je	SHORT $L75819
	push	1098					; 0000044aH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75819
	int	3
$L75819:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75814

; 1100 : 	else

	jmp	SHORT $L75828
$L75811:

; 1102 : 		// non-empty list
; 1103 : 		ASSERT(AfxIsValidAddress(m_pNodeHead, sizeof(CNode)));

	push	1
	push	12					; 0000000cH
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L75825
	push	1103					; 0000044fH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75825
	int	3
$L75825:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75811
$L75823:

; 1104 : 		ASSERT(AfxIsValidAddress(m_pNodeTail, sizeof(CNode)));

	push	1
	push	12					; 0000000cH
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	push	eax
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L75830
	push	1104					; 00000450H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75830
	int	3
$L75830:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75823
$L75828:

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
?AssertValid@?$CList@II@@UBEXXZ ENDP			; CList<unsigned int,unsigned int>::AssertValid
_TEXT	ENDS
;	COMDAT ??_G?$CList@II@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_G?$CList@II@@UAEPAXI@Z PROC NEAR			; CList<unsigned int,unsigned int>::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1?$CList@II@@UAE@XZ			; CList<unsigned int,unsigned int>::~CList<unsigned int,unsigned int>
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75838
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L75838:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_G?$CList@II@@UAEPAXI@Z ENDP				; CList<unsigned int,unsigned int>::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	?Read@CArchive@@QAEIPAXI@Z:NEAR			; CArchive::Read
EXTRN	?Write@CArchive@@QAEXPBXI@Z:NEAR		; CArchive::Write
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAIH@Z
_TEXT	SEGMENT
_ar$ = 8
_pElements$ = 12
_nCount$ = 16
?SerializeElements@@YGXAAVCArchive@@PAIH@Z PROC NEAR	; SerializeElements, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L75913:

; 92   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L75917
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L75917
	push	93					; 0000005dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75917
	int	3
$L75917:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75913

; 93   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 94   : 
; 95   : 	// default is bit-wise read/write
; 96   : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L75918

; 97   : 		ar.Write((void*)pElements, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Write@CArchive@@QAEXPBXI@Z		; CArchive::Write

; 98   : 	else

	jmp	SHORT $L75921
$L75918:

; 99   : 		ar.Read((void*)pElements, nCount * sizeof(TYPE));

	mov	edx, DWORD PTR _nCount$[ebp]
	shl	edx, 2
	push	edx
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Read@CArchive@@QAEIPAXI@Z		; CArchive::Read
$L75921:

; 100  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	12					; 0000000cH
?SerializeElements@@YGXAAVCArchive@@PAIH@Z ENDP		; SerializeElements
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBIH@Z
_TEXT	SEGMENT
_pElements$ = 12
_nCount$ = 16
?DumpElements@@YGXAAVCDumpContext@@PBIH@Z PROC NEAR	; DumpElements, COMDAT

; 105  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L75937:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L75941
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L75941
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75941
	int	3
$L75941:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75937

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
?DumpElements@@YGXAAVCDumpContext@@PBIH@Z ENDP		; DumpElements
_TEXT	ENDS
PUBLIC	?DestructElements@@YGXPAIH@Z			; DestructElements
EXTRN	?FreeDataChain@CPlex@@QAEXXZ:NEAR		; CPlex::FreeDataChain
;	COMDAT ?RemoveAll@?$CList@II@@QAEXXZ
_TEXT	SEGMENT
_this$ = -8
_pNode$ = -4
?RemoveAll@?$CList@II@@QAEXXZ PROC NEAR			; CList<unsigned int,unsigned int>::RemoveAll, COMDAT

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
	jmp	SHORT $L75952
$L75953:
	mov	eax, DWORD PTR _pNode$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pNode$[ebp], ecx
$L75952:
	cmp	DWORD PTR _pNode$[ebp], 0
	je	SHORT $L75954

; 745  : 		DestructElements<TYPE>(&pNode->data, 1);

	push	1
	mov	edx, DWORD PTR _pNode$[ebp]
	add	edx, 8
	push	edx
	call	?DestructElements@@YGXPAIH@Z		; DestructElements
	jmp	SHORT $L75953
$L75954:

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
?RemoveAll@?$CList@II@@QAEXXZ ENDP			; CList<unsigned int,unsigned int>::RemoveAll
_TEXT	ENDS
;	COMDAT ?GetHeadPosition@?$CList@II@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetHeadPosition@?$CList@II@@QBEPAU__POSITION@@XZ PROC NEAR ; CList<unsigned int,unsigned int>::GetHeadPosition, COMDAT

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
?GetHeadPosition@?$CList@II@@QBEPAU__POSITION@@XZ ENDP	; CList<unsigned int,unsigned int>::GetHeadPosition
_TEXT	ENDS
;	COMDAT ?GetNext@?$CList@II@@QAEAAIAAPAU__POSITION@@@Z
_TEXT	SEGMENT
_rPosition$ = 8
_this$ = -8
_pNode$ = -4
?GetNext@?$CList@II@@QAEAAIAAPAU__POSITION@@@Z PROC NEAR ; CList<unsigned int,unsigned int>::GetNext, COMDAT

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
$L75969:

; 689  : 		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));

	push	1
	push	12					; 0000000cH
	mov	edx, DWORD PTR _pNode$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L75973
	push	689					; 000002b1H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75973
	int	3
$L75973:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75969

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
?GetNext@?$CList@II@@QAEAAIAAPAU__POSITION@@@Z ENDP	; CList<unsigned int,unsigned int>::GetNext
_TEXT	ENDS
PUBLIC	?data@CPlex@@QAEPAXXZ				; CPlex::data
PUBLIC	?ConstructElements@@YGXPAIH@Z			; ConstructElements
EXTRN	?Create@CPlex@@SGPAU1@AAPAU1@II@Z:NEAR		; CPlex::Create
;	COMDAT ?NewNode@?$CList@II@@IAEPAUCNode@1@PAU21@0@Z
_TEXT	SEGMENT
_pPrev$ = 8
_pNext$ = 12
_this$ = -20
_pNewBlock$75981 = -16
_pNode$75983 = -8
_i$75985 = -12
_pNode$ = -4
?NewNode@?$CList@II@@IAEPAUCNode@1@PAU21@0@Z PROC NEAR	; CList<unsigned int,unsigned int>::NewNode, COMDAT

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
	jne	SHORT $L75988

; 781  : 		// add another block
; 782  : 		CPlex* pNewBlock = CPlex::Create(m_pBlocks, m_nBlockSize,
; 783  : 				 sizeof(CNode));

	push	12					; 0000000cH
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 20					; 00000014H
	push	eax
	call	?Create@CPlex@@SGPAU1@AAPAU1@II@Z	; CPlex::Create
	mov	DWORD PTR _pNewBlock$75981[ebp], eax

; 784  : 
; 785  : 		// chain them into free list
; 786  : 		CNode* pNode = (CNode*) pNewBlock->data();

	mov	ecx, DWORD PTR _pNewBlock$75981[ebp]
	call	?data@CPlex@@QAEPAXXZ			; CPlex::data
	mov	DWORD PTR _pNode$75983[ebp], eax

; 787  : 		// free in reverse order to make it easier to debug
; 788  : 		pNode += m_nBlockSize - 1;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	imul	edx, 12					; 0000000cH
	mov	eax, DWORD PTR _pNode$75983[ebp]
	add	eax, edx
	mov	DWORD PTR _pNode$75983[ebp], eax

; 789  : 		for (int i = m_nBlockSize-1; i >= 0; i--, pNode--)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	mov	DWORD PTR _i$75985[ebp], edx
	jmp	SHORT $L75986
$L75987:
	mov	eax, DWORD PTR _i$75985[ebp]
	sub	eax, 1
	mov	DWORD PTR _i$75985[ebp], eax
	mov	ecx, DWORD PTR _pNode$75983[ebp]
	sub	ecx, 12					; 0000000cH
	mov	DWORD PTR _pNode$75983[ebp], ecx
$L75986:
	cmp	DWORD PTR _i$75985[ebp], 0
	jl	SHORT $L75988

; 791  : 			pNode->pNext = m_pNodeFree;

	mov	edx, DWORD PTR _pNode$75983[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR [edx], ecx

; 792  : 			m_pNodeFree = pNode;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _pNode$75983[ebp]
	mov	DWORD PTR [edx+16], eax

; 793  : 		}

	jmp	SHORT $L75987
$L75988:

; 795  : 	ASSERT(m_pNodeFree != NULL);  // we must have something

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+16], 0
	jne	SHORT $L75992
	push	795					; 0000031bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75992
	int	3
$L75992:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75988

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
$L75994:

; 802  : 	ASSERT(m_nCount > 0);  // make sure we don't overflow

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jg	SHORT $L75997
	push	802					; 00000322H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75997
	int	3
$L75997:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75994

; 803  : 
; 804  : 	ConstructElements<TYPE>(&pNode->data, 1);

	push	1
	mov	eax, DWORD PTR _pNode$[ebp]
	add	eax, 8
	push	eax
	call	?ConstructElements@@YGXPAIH@Z		; ConstructElements

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
?NewNode@?$CList@II@@IAEPAUCNode@1@PAU21@0@Z ENDP	; CList<unsigned int,unsigned int>::NewNode
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
;	COMDAT ?CompareElements@@YGHPBI0@Z
_TEXT	SEGMENT
_pElement1$ = 8
_pElement2$ = 12
?CompareElements@@YGHPBI0@Z PROC NEAR			; CompareElements, COMDAT

; 118  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L76004:

; 119  : 	ASSERT(AfxIsValidAddress(pElement1, sizeof(TYPE), FALSE));

	push	0
	push	4
	mov	eax, DWORD PTR _pElement1$[ebp]
	push	eax
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76008
	push	119					; 00000077H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76008
	int	3
$L76008:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76004
$L76006:

; 120  : 	ASSERT(AfxIsValidAddress(pElement2, sizeof(ARG_TYPE), FALSE));

	push	0
	push	4
	mov	edx, DWORD PTR _pElement2$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76013
	push	120					; 00000078H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76013
	int	3
$L76013:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76006

; 121  : 
; 122  : 	return *pElement1 == *pElement2;

	mov	ecx, DWORD PTR _pElement1$[ebp]
	mov	edx, DWORD PTR _pElement2$[ebp]
	mov	eax, DWORD PTR [ecx]
	xor	ecx, ecx
	cmp	eax, DWORD PTR [edx]
	sete	cl
	mov	eax, ecx

; 123  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?CompareElements@@YGHPBI0@Z ENDP			; CompareElements
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAIH@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
?DestructElements@@YGXPAIH@Z PROC NEAR			; DestructElements, COMDAT

; 67   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L76032:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76036
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76036
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76036
	int	3
$L76036:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76032

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L76037
$L76038:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 4
	mov	DWORD PTR _pElements$[ebp], eax
$L76037:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L76039

; 73   : 		pElements->~TYPE();

	jmp	SHORT $L76038
$L76039:

; 74   : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?DestructElements@@YGXPAIH@Z ENDP			; DestructElements
_TEXT	ENDS
PUBLIC	??2@YAPAXIPAX@Z					; operator new
EXTRN	_memset:NEAR
;	COMDAT ?ConstructElements@@YGXPAIH@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
$T76435 = -4
?ConstructElements@@YGXPAIH@Z PROC NEAR			; ConstructElements, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L76041:

; 54   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76045
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76045
	push	55					; 00000037H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76045
	int	3
$L76045:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76041

; 55   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 56   : 
; 57   : 	// first do bit-wise zero initialization
; 58   : 	memset((void*)pElements, 0, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	push	0
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	_memset
	add	esp, 12					; 0000000cH

; 59   : 
; 60   : 	// then call the constructor(s)
; 61   : 	for (; nCount--; pElements++)

	jmp	SHORT $L76048
$L76049:
	mov	edx, DWORD PTR _pElements$[ebp]
	add	edx, 4
	mov	DWORD PTR _pElements$[ebp], edx
$L76048:
	mov	eax, DWORD PTR _nCount$[ebp]
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
	test	eax, eax
	je	SHORT $L76050

; 62   : 		::new((void*)pElements) TYPE;

	mov	edx, DWORD PTR _pElements$[ebp]
	push	edx
	push	4
	call	??2@YAPAXIPAX@Z				; operator new
	add	esp, 8
	mov	DWORD PTR $T76435[ebp], eax
	jmp	SHORT $L76049
$L76050:

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
?ConstructElements@@YGXPAIH@Z ENDP			; ConstructElements
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

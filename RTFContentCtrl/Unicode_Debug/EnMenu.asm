	TITLE	D:\_CODE\Shared\EnMenu.cpp
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
;	COMDAT ??BCOSVersion@@QAE?AW4OSVERSION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?VerifyDeleteObject@GraphicsMisc@@YAHAAVCGdiObject@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CEnMenu@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCEnMenu@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CEnMenu@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddMDIButton@CEnMenu@@QAEHW4MENUEX_BTN@@IH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DeleteMDIMenu@CEnMenu@@QAEHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsThemed@CEnMenu@@KAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawMDIButton@CEnMenu@@QAEHPAUtagDRAWITEMSTRUCT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MeasureMDIButton@CEnMenu@@QAEHPAUtagMEASUREITEMSTRUCT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetBackgroundColor@CEnMenu@@QAEXK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadMenuW@CEnMenu@@QAEHIPAUHWND__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMenuItemPos@CEnMenu@@QBEHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMenuItemPos@CEnMenu@@QBEHPAUHMENU__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DeleteSubMenu@CEnMenu@@QAEHPAUHMENU__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DeleteMenu@CEnMenu@@QAEHIIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetLocalizer@CEnMenu@@SAXPAVITransText@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DeleteSubMenu@CEnMenu@@SAHPAUHMENU__@@0H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@IAAPAU2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@0AAPAU2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMenuItemPos@CEnMenu@@KAHPAUHMENU__@@KAAPAU2@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DeleteMenu@CEnMenu@@SAHPAUHMENU__@@IIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DoCleanUp@CEnMenu@@KAXPAUHMENU__@@0H@Z
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
;	COMDAT ??0?$CMap@IIHH@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lookup@?$CMap@IIHH@@QBEHIAAH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CMap@IIHH@@QAEAAHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveKey@?$CMap@IIHH@@QAEHI@Z
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
;	COMDAT ??_G?$CMap@IIHH@@UAEPAXI@Z
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
;	COMDAT ?InitHashTable@?$CMap@IIHH@@QAEXIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewAssoc@?$CMap@IIHH@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FreeAssoc@?$CMap@IIHH@@IAEXPAUCAssoc@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAssocAt@?$CMap@IIHH@@IBEPAUCAssoc@1@IAAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HashKey@@YGII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CompareElements@@YGHPBI0@Z
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
;	COMDAT ?DestructElements@@YGXPAHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAHH@Z
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
;	COMDAT ??_7?$CMap@IIHH@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CEnMenu@@6B@
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
PUBLIC	?s_pTT@CEnMenu@@1PAVITransText@@A		; CEnMenu::s_pTT
_BSS	SEGMENT
?s_pTT@CEnMenu@@1PAVITransText@@A DD 01H DUP (?)	; CEnMenu::s_pTT
_BSS	ENDS
_DATA	SEGMENT
__szAfxTempl DB	'afxtempl.h', 00H
	ORG $+1
_THIS_FILE DB	'D:\_CODE\Shared\EnMenu.cpp', 00H
_DATA	ENDS
PUBLIC	??0CEnMenu@@QAE@XZ				; CEnMenu::CEnMenu
PUBLIC	??0?$CMap@IIHH@@QAE@H@Z				; CMap<unsigned int,unsigned int,int,int>::CMap<unsigned int,unsigned int,int,int>
PUBLIC	??1?$CMap@IIHH@@UAE@XZ				; CMap<unsigned int,unsigned int,int,int>::~CMap<unsigned int,unsigned int,int,int>
PUBLIC	??_7CEnMenu@@6B@				; CEnMenu::`vftable'
PUBLIC	??_GCEnMenu@@UAEPAXI@Z				; CEnMenu::`scalar deleting destructor'
PUBLIC	??_ECEnMenu@@UAEPAXI@Z				; CEnMenu::`vector deleting destructor'
EXTRN	??0CBrush@@QAE@XZ:NEAR				; CBrush::CBrush
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	__except_list:DWORD
EXTRN	__chkesp:NEAR
EXTRN	?GetRuntimeClass@CMenu@@UBEPAUCRuntimeClass@@XZ:NEAR ; CMenu::GetRuntimeClass
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CMenu@@QAE@XZ:NEAR				; CMenu::CMenu
EXTRN	?DrawItem@CMenu@@UAEXPAUtagDRAWITEMSTRUCT@@@Z:NEAR ; CMenu::DrawItem
EXTRN	?MeasureItem@CMenu@@UAEXPAUtagMEASUREITEMSTRUCT@@@Z:NEAR ; CMenu::MeasureItem
EXTRN	??1CMenu@@UAE@XZ:NEAR				; CMenu::~CMenu
EXTRN	?AssertValid@CMenu@@UBEXXZ:NEAR			; CMenu::AssertValid
EXTRN	?Dump@CMenu@@UBEXAAVCDumpContext@@@Z:NEAR	; CMenu::Dump
;	COMDAT ??_7CEnMenu@@6B@
; File D:\_CODE\Shared\EnMenu.cpp
CONST	SEGMENT
??_7CEnMenu@@6B@ DD FLAT:?GetRuntimeClass@CMenu@@UBEPAUCRuntimeClass@@XZ ; CEnMenu::`vftable'
	DD	FLAT:??_ECEnMenu@@UAEPAXI@Z
	DD	FLAT:?Serialize@CObject@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@CMenu@@UBEXXZ
	DD	FLAT:?Dump@CMenu@@UBEXAAVCDumpContext@@@Z
	DD	FLAT:?DrawItem@CMenu@@UAEXPAUtagDRAWITEMSTRUCT@@@Z
	DD	FLAT:?MeasureItem@CMenu@@UAEXPAUtagMEASUREITEMSTRUCT@@@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76970	DD	019930520H
	DD	02H
	DD	FLAT:$T76974
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76974	DD	0ffffffffH
	DD	FLAT:$L76964
	DD	00H
	DD	FLAT:$L76965
xdata$x	ENDS
;	COMDAT ??0CEnMenu@@QAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??0CEnMenu@@QAE@XZ PROC NEAR				; CEnMenu::CEnMenu, COMDAT

; 49   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76971
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CMenu@@QAE@XZ			; CMenu::CMenu
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	push	10					; 0000000aH
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??0?$CMap@IIHH@@QAE@H@Z			; CMap<unsigned int,unsigned int,int,int>::CMap<unsigned int,unsigned int,int,int>
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 36					; 00000024H
	call	??0CBrush@@QAE@XZ			; CBrush::CBrush
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CEnMenu@@6B@ ; CEnMenu::`vftable'

; 50   : 
; 51   : }

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
$L76964:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CMenu@@UAE@XZ			; CMenu::~CMenu
	ret	0
$L76965:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??1?$CMap@IIHH@@UAE@XZ			; CMap<unsigned int,unsigned int,int,int>::~CMap<unsigned int,unsigned int,int,int>
	ret	0
$L76971:
	mov	eax, OFFSET FLAT:$T76970
	jmp	___CxxFrameHandler
text$x	ENDS
??0CEnMenu@@QAE@XZ ENDP					; CEnMenu::CEnMenu
PUBLIC	??1CEnMenu@@UAE@XZ				; CEnMenu::~CEnMenu
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_GCEnMenu@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCEnMenu@@UAEPAXI@Z PROC NEAR			; CEnMenu::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CEnMenu@@UAE@XZ			; CEnMenu::~CEnMenu
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L76144
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L76144:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCEnMenu@@UAEPAXI@Z ENDP				; CEnMenu::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	??1CBrush@@UAE@XZ:NEAR				; CBrush::~CBrush
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnMenu.cpp
xdata$x	SEGMENT
$T76986	DD	019930520H
	DD	02H
	DD	FLAT:$T76988
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76988	DD	0ffffffffH
	DD	FLAT:$L76982
	DD	00H
	DD	FLAT:$L76983
xdata$x	ENDS
;	COMDAT ??1CEnMenu@@UAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??1CEnMenu@@UAE@XZ PROC NEAR				; CEnMenu::~CEnMenu, COMDAT

; 54   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76987
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CEnMenu@@6B@ ; CEnMenu::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 55   : }

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 36					; 00000024H
	call	??1CBrush@@UAE@XZ			; CBrush::~CBrush
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??1?$CMap@IIHH@@UAE@XZ			; CMap<unsigned int,unsigned int,int,int>::~CMap<unsigned int,unsigned int,int,int>
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CMenu@@UAE@XZ			; CMenu::~CMenu
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
$L76982:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CMenu@@UAE@XZ			; CMenu::~CMenu
	ret	0
$L76983:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??1?$CMap@IIHH@@UAE@XZ			; CMap<unsigned int,unsigned int,int,int>::~CMap<unsigned int,unsigned int,int,int>
	ret	0
$L76987:
	mov	eax, OFFSET FLAT:$T76986
	jmp	___CxxFrameHandler
text$x	ENDS
??1CEnMenu@@UAE@XZ ENDP					; CEnMenu::~CEnMenu
PUBLIC	??BCOSVersion@@QAE?AW4OSVERSION@@XZ		; COSVersion::operator enum OSVERSION
PUBLIC	?AddMDIButton@CEnMenu@@QAEHW4MENUEX_BTN@@IH@Z	; CEnMenu::AddMDIButton
PUBLIC	??A?$CMap@IIHH@@QAEAAHI@Z			; CMap<unsigned int,unsigned int,int,int>::operator[]
PUBLIC	?IsThemed@CEnMenu@@KAHXZ			; CEnMenu::IsThemed
EXTRN	??0COSVersion@@QAE@XZ:NEAR			; COSVersion::COSVersion
EXTRN	??1COSVersion@@UAE@XZ:NEAR			; COSVersion::~COSVersion
EXTRN	?FromHandle@CBitmap@@SGPAV1@PAUHBITMAP__@@@Z:NEAR ; CBitmap::FromHandle
EXTRN	?GetSafeHmenu@CMenu@@QBEPAUHMENU__@@XZ:NEAR	; CMenu::GetSafeHmenu
EXTRN	?InsertMenuW@CMenu@@QAEHIIIPBVCBitmap@@@Z:NEAR	; CMenu::InsertMenuW
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnMenu.cpp
xdata$x	SEGMENT
$T77000	DD	019930520H
	DD	02H
	DD	FLAT:$T77002
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77002	DD	0ffffffffH
	DD	FLAT:$L76997
	DD	0ffffffffH
	DD	FLAT:$L76998
xdata$x	ENDS
;	COMDAT ?AddMDIButton@CEnMenu@@QAEHW4MENUEX_BTN@@IH@Z
_TEXT	SEGMENT
$T76993 = -24
$T76994 = -28
$T76995 = -32
$T76996 = -36
__$EHRec$ = -12
_nBtn$ = 8
_nCmdID$ = 12
_bRightJustify$ = 16
_this$ = -40
_hbm$ = -20
_nFlags$ = -16
?AddMDIButton@CEnMenu@@QAEHW4MENUEX_BTN@@IH@Z PROC NEAR	; CEnMenu::AddMDIButton, COMDAT

; 58   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77001
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 48					; 00000030H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-60]
	mov	ecx, 12					; 0000000cH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L76154:

; 59   : 	ASSERT (GetSafeHmenu());

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHmenu@CMenu@@QBEPAUHMENU__@@XZ	; CMenu::GetSafeHmenu
	test	eax, eax
	jne	SHORT $L76157
	push	59					; 0000003bH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76157
	int	3
$L76157:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76154

; 60   : 
; 61   : 	if (!GetSafeHmenu())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHmenu@CMenu@@QBEPAUHMENU__@@XZ	; CMenu::GetSafeHmenu
	test	eax, eax
	jne	SHORT $L76158

; 62   : 		return FALSE;

	xor	eax, eax
	jmp	$L76153
$L76158:

; 63   : 
; 64   : 	HBITMAP hbm = NULL;

	mov	DWORD PTR _hbm$[ebp], 0

; 65   : 
; 66   : 	if (!IsThemed() || COSVersion() > OSV_XP)

	call	?IsThemed@CEnMenu@@KAHXZ		; CEnMenu::IsThemed
	test	eax, eax
	je	SHORT $L76163
	lea	ecx, DWORD PTR $T76994[ebp]
	call	??0COSVersion@@QAE@XZ			; COSVersion::COSVersion
	mov	DWORD PTR -44+[ebp], eax
	mov	ecx, DWORD PTR -44+[ebp]
	mov	DWORD PTR -48+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -48+[ebp]
	call	??BCOSVersion@@QAE?AW4OSVERSION@@XZ	; COSVersion::operator enum OSVERSION
	xor	edx, edx
	cmp	eax, 5
	setg	dl
	mov	BYTE PTR $T76993[ebp], dl
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76994[ebp]
	call	??1COSVersion@@UAE@XZ			; COSVersion::~COSVersion
	mov	eax, DWORD PTR $T76993[ebp]
	and	eax, 255				; 000000ffH
	test	eax, eax
	je	SHORT $L76165
$L76163:

; 69   : 		{

	mov	ecx, DWORD PTR _nBtn$[ebp]
	mov	DWORD PTR -52+[ebp], ecx
	cmp	DWORD PTR -52+[ebp], 0
	je	SHORT $L76168
	cmp	DWORD PTR -52+[ebp], 1
	je	SHORT $L76170
	cmp	DWORD PTR -52+[ebp], 2
	je	SHORT $L76172
	jmp	SHORT $L76174
$L76168:

; 70   : 		case MEB_MINIMIZE:
; 71   : 			hbm = HBMMENU_MBAR_MINIMIZE;

	mov	DWORD PTR _hbm$[ebp], 3

; 72   : 			break;

	jmp	SHORT $L76165
$L76170:

; 73   : 			
; 74   : 		case MEB_RESTORE:
; 75   : 			hbm = HBMMENU_MBAR_RESTORE;

	mov	DWORD PTR _hbm$[ebp], 2

; 76   : 			break;

	jmp	SHORT $L76165
$L76172:

; 77   : 			
; 78   : 		case MEB_CLOSE:
; 79   : 			hbm = HBMMENU_MBAR_CLOSE;

	mov	DWORD PTR _hbm$[ebp], 5

; 80   : 			break;

	jmp	SHORT $L76165
$L76174:

; 81   : 			
; 82   : 		default:
; 83   : 			return FALSE;

	xor	eax, eax
	jmp	$L76153
$L76165:

; 86   : 	
; 87   : 	UINT nFlags = (bRightJustify ? MFT_RIGHTJUSTIFY : 0);

	mov	edx, DWORD PTR _bRightJustify$[ebp]
	neg	edx
	sbb	edx, edx
	and	edx, 16384				; 00004000H
	mov	DWORD PTR _nFlags$[ebp], edx

; 88   : 	
; 89   : 	if (!IsThemed() || COSVersion() > OSV_XP)

	call	?IsThemed@CEnMenu@@KAHXZ		; CEnMenu::IsThemed
	test	eax, eax
	je	SHORT $L76179
	lea	ecx, DWORD PTR $T76996[ebp]
	call	??0COSVersion@@QAE@XZ			; COSVersion::COSVersion
	mov	DWORD PTR -56+[ebp], eax
	mov	eax, DWORD PTR -56+[ebp]
	mov	DWORD PTR -60+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -60+[ebp]
	call	??BCOSVersion@@QAE?AW4OSVERSION@@XZ	; COSVersion::operator enum OSVERSION
	xor	ecx, ecx
	cmp	eax, 5
	setg	cl
	mov	BYTE PTR $T76995[ebp], cl
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76996[ebp]
	call	??1COSVersion@@UAE@XZ			; COSVersion::~COSVersion
	mov	edx, DWORD PTR $T76995[ebp]
	and	edx, 255				; 000000ffH
	test	edx, edx
	je	SHORT $L76178
$L76179:

; 90   : 		nFlags |= MFT_BITMAP;

	mov	eax, DWORD PTR _nFlags$[ebp]
	or	al, 4
	mov	DWORD PTR _nFlags$[ebp], eax

; 91   : 	else

	jmp	SHORT $L76180
$L76178:

; 92   : 		nFlags |= MFT_OWNERDRAW;

	mov	ecx, DWORD PTR _nFlags$[ebp]
	or	ch, 1
	mov	DWORD PTR _nFlags$[ebp], ecx
$L76180:

; 93   : 		
; 94   : 	if (InsertMenu((UINT)-1, nFlags, nCmdID, CBitmap::FromHandle(hbm)))

	mov	edx, DWORD PTR _hbm$[ebp]
	push	edx
	call	?FromHandle@CBitmap@@SGPAV1@PAUHBITMAP__@@@Z ; CBitmap::FromHandle
	push	eax
	mov	eax, DWORD PTR _nCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nFlags$[ebp]
	push	ecx
	push	-1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?InsertMenuW@CMenu@@QAEHIIIPBVCBitmap@@@Z ; CMenu::InsertMenuW
	test	eax, eax
	je	SHORT $L76182

; 96   : 		m_mapCmd2ID[nCmdID] = nBtn;

	mov	edx, DWORD PTR _nCmdID$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??A?$CMap@IIHH@@QAEAAHI@Z		; CMap<unsigned int,unsigned int,int,int>::operator[]
	mov	ecx, DWORD PTR _nBtn$[ebp]
	mov	DWORD PTR [eax], ecx

; 97   : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76153
$L76182:

; 99   : 
; 100  : 	// else
; 101  : 	return FALSE;

	xor	eax, eax
$L76153:

; 102  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 60					; 0000003cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76997:
	lea	ecx, DWORD PTR $T76994[ebp]
	call	??1COSVersion@@UAE@XZ			; COSVersion::~COSVersion
	ret	0
$L76998:
	lea	ecx, DWORD PTR $T76996[ebp]
	call	??1COSVersion@@UAE@XZ			; COSVersion::~COSVersion
	ret	0
$L77001:
	mov	eax, OFFSET FLAT:$T77000
	jmp	___CxxFrameHandler
text$x	ENDS
?AddMDIButton@CEnMenu@@QAEHW4MENUEX_BTN@@IH@Z ENDP	; CEnMenu::AddMDIButton
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
PUBLIC	?DeleteMDIMenu@CEnMenu@@QAEHI@Z			; CEnMenu::DeleteMDIMenu
PUBLIC	?DeleteMenu@CEnMenu@@QAEHIIH@Z			; CEnMenu::DeleteMenu
PUBLIC	?RemoveKey@?$CMap@IIHH@@QAEHI@Z			; CMap<unsigned int,unsigned int,int,int>::RemoveKey
EXTRN	?GetMenuItemCount@CMenu@@QBEIXZ:NEAR		; CMenu::GetMenuItemCount
EXTRN	?GetMenuItemID@CMenu@@QBEIH@Z:NEAR		; CMenu::GetMenuItemID
;	COMDAT ?DeleteMDIMenu@CEnMenu@@QAEHI@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_this$ = -12
_nItem$ = -4
_nMenuCmdID$76191 = -8
?DeleteMDIMenu@CEnMenu@@QAEHI@Z PROC NEAR		; CEnMenu::DeleteMDIMenu, COMDAT

; 105  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 106  : 	// CMenu::DeleteMenu won't work on bitmap buttons directly
; 107  : 	// so we must traverse all menu items looking for nCmdID
; 108  : 	int nItem = GetMenuItemCount();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetMenuItemCount@CMenu@@QBEIXZ		; CMenu::GetMenuItemCount
	mov	DWORD PTR _nItem$[ebp], eax
$L76189:

; 109  : 
; 110  : 	while (nItem--)

	mov	eax, DWORD PTR _nItem$[ebp]
	mov	ecx, DWORD PTR _nItem$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nItem$[ebp], ecx
	test	eax, eax
	je	SHORT $L76190

; 112  : 		UINT nMenuCmdID = GetMenuItemID(nItem);

	mov	edx, DWORD PTR _nItem$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetMenuItemID@CMenu@@QBEIH@Z		; CMenu::GetMenuItemID
	mov	DWORD PTR _nMenuCmdID$76191[ebp], eax

; 113  : 
; 114  : 		if (nCmdID == nMenuCmdID)

	mov	eax, DWORD PTR _nCmdID$[ebp]
	cmp	eax, DWORD PTR _nMenuCmdID$76191[ebp]
	jne	SHORT $L76192

; 116  : 			DeleteMenu(nItem, MF_BYPOSITION);

	push	0
	push	1024					; 00000400H
	mov	ecx, DWORD PTR _nItem$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DeleteMenu@CEnMenu@@QAEHIIH@Z		; CEnMenu::DeleteMenu

; 117  : 			m_mapCmd2ID.RemoveKey(nCmdID);

	mov	edx, DWORD PTR _nCmdID$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	?RemoveKey@?$CMap@IIHH@@QAEHI@Z		; CMap<unsigned int,unsigned int,int,int>::RemoveKey

; 118  : 
; 119  : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L76186
$L76192:

; 121  : 	}

	jmp	SHORT $L76189
$L76190:

; 122  : 
; 123  : 	return FALSE;

	xor	eax, eax
$L76186:

; 124  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?DeleteMDIMenu@CEnMenu@@QAEHI@Z ENDP			; CEnMenu::DeleteMDIMenu
_TEXT	ENDS
EXTRN	??0CThemed@@QAE@PAVCWnd@@PBG@Z:NEAR		; CThemed::CThemed
EXTRN	??1CThemed@@UAE@XZ:NEAR				; CThemed::~CThemed
EXTRN	?IsNonClientThemed@CThemed@@SAHXZ:NEAR		; CThemed::IsNonClientThemed
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnMenu.cpp
xdata$x	SEGMENT
$T77015	DD	019930520H
	DD	01H
	DD	FLAT:$T77017
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77017	DD	0ffffffffH
	DD	FLAT:$L77013
xdata$x	ENDS
;	COMDAT ?IsThemed@CEnMenu@@KAHXZ
_TEXT	SEGMENT
$T77011 = -16
$T77012 = -28
__$EHRec$ = -12
?IsThemed@CEnMenu@@KAHXZ PROC NEAR			; CEnMenu::IsThemed, COMDAT

; 127  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77016
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 128  : 	return CThemed().IsNonClientThemed();

	push	0
	push	0
	lea	ecx, DWORD PTR $T77012[ebp]
	call	??0CThemed@@QAE@PAVCWnd@@PBG@Z		; CThemed::CThemed
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	call	?IsNonClientThemed@CThemed@@SAHXZ	; CThemed::IsNonClientThemed
	mov	DWORD PTR $T77011[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T77012[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	mov	eax, DWORD PTR $T77011[ebp]

; 129  : }

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
$L77013:
	lea	ecx, DWORD PTR $T77012[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	ret	0
$L77016:
	mov	eax, OFFSET FLAT:$T77015
	jmp	___CxxFrameHandler
text$x	ENDS
?IsThemed@CEnMenu@@KAHXZ ENDP				; CEnMenu::IsThemed
PUBLIC	?DrawMDIButton@CEnMenu@@QAEHPAUtagDRAWITEMSTRUCT@@@Z ; CEnMenu::DrawMDIButton
PUBLIC	?Lookup@?$CMap@IIHH@@QBEHIAAH@Z			; CMap<unsigned int,unsigned int,int,int>::Lookup
EXTRN	?FromHandle@CDC@@SGPAV1@PAUHDC__@@@Z:NEAR	; CDC::FromHandle
EXTRN	?AfxGetMainWnd@@YGPAVCWnd@@XZ:NEAR		; AfxGetMainWnd
EXTRN	??0CRect@@QAE@ABUtagRECT@@@Z:NEAR		; CRect::CRect
EXTRN	?Open@CThemed@@QAEHPBVCWnd@@PBG@Z:NEAR		; CThemed::Open
EXTRN	?DrawBackground@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBUtagRECT@@@Z:NEAR ; CThemed::DrawBackground
;	COMDAT data
; File D:\_CODE\Shared\EnMenu.cpp
data	SEGMENT
$SG76207 DB	'W', 00H, 'I', 00H, 'N', 00H, 'D', 00H, 'O', 00H, 'W', 00H
	DB	00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77026	DD	019930520H
	DD	01H
	DD	FLAT:$T77028
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77028	DD	0ffffffffH
	DD	FLAT:$L77024
xdata$x	ENDS
;	COMDAT ?DrawMDIButton@CEnMenu@@QAEHPAUtagDRAWITEMSTRUCT@@@Z
_TEXT	SEGMENT
$T77022 = -60
$T77023 = -64
__$EHRec$ = -12
_lpDrawItemStruct$ = 8
_this$ = -68
_rect$ = -36
_pDC$ = -56
_nBtn$ = -40
_th$ = -52
_nThPart$ = -20
_nThState$ = -16
?DrawMDIButton@CEnMenu@@QAEHPAUtagDRAWITEMSTRUCT@@@Z PROC NEAR ; CEnMenu::DrawMDIButton, COMDAT

; 132  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77027
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 60					; 0000003cH
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-72]
	mov	ecx, 15					; 0000000fH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 133  : 	if (!IsThemed())

	call	?IsThemed@CEnMenu@@KAHXZ		; CEnMenu::IsThemed
	test	eax, eax
	jne	SHORT $L76201

; 134  : 		return FALSE;

	xor	eax, eax
	jmp	$L76200
$L76201:

; 135  : 
; 136  : 	// draw the button
; 137  : 	CRect rect(lpDrawItemStruct->rcItem);

	mov	eax, DWORD PTR _lpDrawItemStruct$[ebp]
	add	eax, 28					; 0000001cH
	push	eax
	lea	ecx, DWORD PTR _rect$[ebp]
	call	??0CRect@@QAE@ABUtagRECT@@@Z		; CRect::CRect

; 138  : 	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	mov	ecx, DWORD PTR _lpDrawItemStruct$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	push	edx
	call	?FromHandle@CDC@@SGPAV1@PAUHDC__@@@Z	; CDC::FromHandle
	mov	DWORD PTR _pDC$[ebp], eax

; 139  : 
; 140  : 	int nBtn = -1;

	mov	DWORD PTR _nBtn$[ebp], -1

; 141  : 	m_mapCmd2ID.Lookup(lpDrawItemStruct->itemID, nBtn);

	lea	eax, DWORD PTR _nBtn$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpDrawItemStruct$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	?Lookup@?$CMap@IIHH@@QBEHIAAH@Z		; CMap<unsigned int,unsigned int,int,int>::Lookup

; 142  : 
; 143  : 	CThemed th;

	push	0
	push	0
	lea	ecx, DWORD PTR _th$[ebp]
	call	??0CThemed@@QAE@PAVCWnd@@PBG@Z		; CThemed::CThemed
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 144  : 		
; 145  : 	if (!th.Open(AfxGetMainWnd(), _T("WINDOW")))

	push	OFFSET FLAT:$SG76207
	call	?AfxGetMainWnd@@YGPAVCWnd@@XZ		; AfxGetMainWnd
	push	eax
	lea	ecx, DWORD PTR _th$[ebp]
	call	?Open@CThemed@@QAEHPBVCWnd@@PBG@Z	; CThemed::Open
	test	eax, eax
	jne	SHORT $L76206

; 146  : 		return FALSE;

	mov	DWORD PTR $T77022[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _th$[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	mov	eax, DWORD PTR $T77022[ebp]
	jmp	SHORT $L76200
$L76206:

; 147  : 
; 148  : 	int nThPart = 0, nThState = 0;

	mov	DWORD PTR _nThPart$[ebp], 0
	mov	DWORD PTR _nThState$[ebp], 0

; 151  : 	{

	mov	eax, DWORD PTR _nBtn$[ebp]
	mov	DWORD PTR -72+[ebp], eax
	cmp	DWORD PTR -72+[ebp], 0
	je	SHORT $L76215
	cmp	DWORD PTR -72+[ebp], 1
	je	SHORT $L76216
	cmp	DWORD PTR -72+[ebp], 2
	je	SHORT $L76217
	jmp	SHORT $L76212
$L76215:

; 152  : 	case MEB_MINIMIZE:
; 153  : 		nThPart = WP_MDIMINBUTTON;

	mov	DWORD PTR _nThPart$[ebp], 16		; 00000010H

; 154  : 		break;

	jmp	SHORT $L76212
$L76216:

; 155  : 
; 156  : 	case MEB_RESTORE:
; 157  : 		nThPart = WP_MDIRESTOREBUTTON;

	mov	DWORD PTR _nThPart$[ebp], 22		; 00000016H

; 158  : 		break;

	jmp	SHORT $L76212
$L76217:

; 159  : 
; 160  : 	case MEB_CLOSE:
; 161  : 		nThPart = WP_MDICLOSEBUTTON;

	mov	DWORD PTR _nThPart$[ebp], 20		; 00000014H
$L76212:

; 164  : 		
; 165  : 	th.DrawBackground(pDC, nThPart, nThState, rect, NULL);

	push	0
	lea	ecx, DWORD PTR _rect$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nThState$[ebp]
	push	edx
	mov	eax, DWORD PTR _nThPart$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _th$[ebp]
	call	?DrawBackground@CThemed@@QAEHPAVCDC@@HHABVCRect@@PBUtagRECT@@@Z ; CThemed::DrawBackground

; 166  : 
; 167  : 	return TRUE;

	mov	DWORD PTR $T77023[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _th$[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	mov	eax, DWORD PTR $T77023[ebp]
$L76200:

; 168  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 72					; 00000048H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77024:
	lea	ecx, DWORD PTR _th$[ebp]
	call	??1CThemed@@UAE@XZ			; CThemed::~CThemed
	ret	0
$L77027:
	mov	eax, OFFSET FLAT:$T77026
	jmp	___CxxFrameHandler
text$x	ENDS
?DrawMDIButton@CEnMenu@@QAEHPAUtagDRAWITEMSTRUCT@@@Z ENDP ; CEnMenu::DrawMDIButton
PUBLIC	?MeasureMDIButton@CEnMenu@@QAEHPAUtagMEASUREITEMSTRUCT@@@Z ; CEnMenu::MeasureMDIButton
;	COMDAT ?MeasureMDIButton@CEnMenu@@QAEHPAUtagMEASUREITEMSTRUCT@@@Z
_TEXT	SEGMENT
_lpMeasureItemStruct$ = 8
_this$ = -4
?MeasureMDIButton@CEnMenu@@QAEHPAUtagMEASUREITEMSTRUCT@@@Z PROC NEAR ; CEnMenu::MeasureMDIButton, COMDAT

; 171  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 172  : 	lpMeasureItemStruct->itemHeight = 4;

	mov	eax, DWORD PTR _lpMeasureItemStruct$[ebp]
	mov	DWORD PTR [eax+16], 4

; 173  : 	lpMeasureItemStruct->itemWidth = 4;

	mov	ecx, DWORD PTR _lpMeasureItemStruct$[ebp]
	mov	DWORD PTR [ecx+12], 4

; 174  : 
; 175  : 	return TRUE;

	mov	eax, 1

; 176  : }

	mov	esp, ebp
	pop	ebp
	ret	4
?MeasureMDIButton@CEnMenu@@QAEHPAUtagMEASUREITEMSTRUCT@@@Z ENDP ; CEnMenu::MeasureMDIButton
_TEXT	ENDS
PUBLIC	?VerifyDeleteObject@GraphicsMisc@@YAHAAVCGdiObject@@@Z ; GraphicsMisc::VerifyDeleteObject
PUBLIC	?SetBackgroundColor@CEnMenu@@QAEXK@Z		; CEnMenu::SetBackgroundColor
EXTRN	?CreateSolidBrush@CBrush@@QAEHK@Z:NEAR		; CBrush::CreateSolidBrush
EXTRN	??BCBrush@@QBEPAUHBRUSH__@@XZ:NEAR		; CBrush::operator HBRUSH__ *
EXTRN	__imp__SetMenuInfo@8:NEAR
;	COMDAT ?SetBackgroundColor@CEnMenu@@QAEXK@Z
_TEXT	SEGMENT
_color$ = 8
_this$ = -32
_MenuInfo$ = -28
?SetBackgroundColor@CEnMenu@@QAEXK@Z PROC NEAR		; CEnMenu::SetBackgroundColor, COMDAT

; 179  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 32					; 00000020H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx
$L76227:

; 180  : 	ASSERT(GetSafeHmenu());

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHmenu@CMenu@@QBEPAUHMENU__@@XZ	; CMenu::GetSafeHmenu
	test	eax, eax
	jne	SHORT $L76230
	push	180					; 000000b4H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76230
	int	3
$L76230:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76227

; 181  : 
; 182  : 	// menu background color
; 183  : 	GraphicsMisc::VerifyDeleteObject(m_brBkgnd);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 36					; 00000024H
	push	ecx
	call	?VerifyDeleteObject@GraphicsMisc@@YAHAAVCGdiObject@@@Z ; GraphicsMisc::VerifyDeleteObject
	add	esp, 4

; 184  : 	m_brBkgnd.CreateSolidBrush(color);

	mov	edx, DWORD PTR _color$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 36					; 00000024H
	call	?CreateSolidBrush@CBrush@@QAEHK@Z	; CBrush::CreateSolidBrush

; 185  : 	
; 186  : 	MENUINFO MenuInfo = {0};

	mov	DWORD PTR _MenuInfo$[ebp], 0
	mov	ecx, 6
	xor	eax, eax
	lea	edi, DWORD PTR _MenuInfo$[ebp+4]
	rep stosd

; 187  : 	MenuInfo.cbSize = sizeof(MenuInfo);

	mov	DWORD PTR _MenuInfo$[ebp], 28		; 0000001cH

; 188  : 	MenuInfo.hbrBack = m_brBkgnd; 

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 36					; 00000024H
	call	??BCBrush@@QBEPAUHBRUSH__@@XZ		; CBrush::operator HBRUSH__ *
	mov	DWORD PTR _MenuInfo$[ebp+16], eax

; 189  : 	MenuInfo.fMask = MIM_BACKGROUND;

	mov	DWORD PTR _MenuInfo$[ebp+4], 2

; 190  : 	
; 191  : 	::SetMenuInfo(GetSafeHmenu(), &MenuInfo);

	mov	esi, esp
	lea	eax, DWORD PTR _MenuInfo$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHmenu@CMenu@@QBEPAUHMENU__@@XZ	; CMenu::GetSafeHmenu
	push	eax
	call	DWORD PTR __imp__SetMenuInfo@8
	cmp	esi, esp
	call	__chkesp

; 192  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?SetBackgroundColor@CEnMenu@@QAEXK@Z ENDP		; CEnMenu::SetBackgroundColor
_TEXT	ENDS
PUBLIC	??_C@_0BP@DDHI@D?3?2_CODE?2Shared?2graphicsmisc?4h?$AA@ ; `string'
EXTRN	?DeleteObject@CGdiObject@@QAEHXZ:NEAR		; CGdiObject::DeleteObject
;	COMDAT ??_C@_0BP@DDHI@D?3?2_CODE?2Shared?2graphicsmisc?4h?$AA@
; File D:\_CODE\Shared\graphicsmisc.h
_DATA	SEGMENT
??_C@_0BP@DDHI@D?3?2_CODE?2Shared?2graphicsmisc?4h?$AA@ DB 'D:\_CODE\Shar'
	DB	'ed\graphicsmisc.h', 00H			; `string'
_DATA	ENDS
;	COMDAT ?VerifyDeleteObject@GraphicsMisc@@YAHAAVCGdiObject@@@Z
_TEXT	SEGMENT
_obj$ = 8
?VerifyDeleteObject@GraphicsMisc@@YAHAAVCGdiObject@@@Z PROC NEAR ; GraphicsMisc::VerifyDeleteObject, COMDAT

; 101  : 	{

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi

; 102  : 		if (obj.m_hObject == NULL)

	mov	eax, DWORD PTR _obj$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L76060

; 103  : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L76059
$L76060:

; 104  : 
; 105  : 		// else
; 106  : 		if (obj.DeleteObject())

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?DeleteObject@CGdiObject@@QAEHXZ	; CGdiObject::DeleteObject
	test	eax, eax
	je	SHORT $L76061

; 107  : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L76059
$L76061:

; 108  : 
; 109  : 		// else
; 110  : 		ASSERT(0);

	mov	ecx, 1
	test	ecx, ecx
	je	SHORT $L76065
	push	110					; 0000006eH
	push	OFFSET FLAT:??_C@_0BP@DDHI@D?3?2_CODE?2Shared?2graphicsmisc?4h?$AA@ ; `string'
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76065
	int	3
$L76065:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76061

; 111  : 		return FALSE;

	xor	eax, eax
$L76059:

; 112  : 	}

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?VerifyDeleteObject@GraphicsMisc@@YAHAAVCGdiObject@@@Z ENDP ; GraphicsMisc::VerifyDeleteObject
_TEXT	ENDS
PUBLIC	?LoadMenuW@CEnMenu@@QAEHIPAUHWND__@@H@Z		; CEnMenu::LoadMenuW
EXTRN	?LoadMenuW@CMenu@@QAEHI@Z:NEAR			; CMenu::LoadMenuW
EXTRN	??BCMenu@@QBEPAUHMENU__@@XZ:NEAR		; CMenu::operator HMENU__ *
;	COMDAT ?LoadMenuW@CEnMenu@@QAEHIPAUHWND__@@H@Z
_TEXT	SEGMENT
_nMenuResID$ = 8
_hWndRef$ = 12
_bTranslateAll$ = 16
_this$ = -4
?LoadMenuW@CEnMenu@@QAEHIPAUHWND__@@H@Z PROC NEAR	; CEnMenu::LoadMenuW, COMDAT

; 195  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 196  : 	if (CMenu::LoadMenu(nMenuResID))

	mov	eax, DWORD PTR _nMenuResID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadMenuW@CMenu@@QAEHI@Z		; CMenu::LoadMenuW
	test	eax, eax
	je	SHORT $L76238

; 198  : 		if (s_pTT)

	cmp	DWORD PTR ?s_pTT@CEnMenu@@1PAVITransText@@A, 0 ; CEnMenu::s_pTT
	je	SHORT $L76239

; 199  : 			s_pTT->TranslateMenu(*this, hWndRef, bTranslateAll);

	mov	esi, esp
	mov	ecx, DWORD PTR _bTranslateAll$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hWndRef$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??BCMenu@@QBEPAUHMENU__@@XZ		; CMenu::operator HMENU__ *
	push	eax
	mov	eax, DWORD PTR ?s_pTT@CEnMenu@@1PAVITransText@@A ; CEnMenu::s_pTT
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR ?s_pTT@CEnMenu@@1PAVITransText@@A ; CEnMenu::s_pTT
	call	DWORD PTR [edx+36]
	cmp	esi, esp
	call	__chkesp
$L76239:

; 200  : 
; 201  : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76237
$L76238:

; 203  : 
; 204  : 	// else
; 205  : 	return FALSE;

	xor	eax, eax
$L76237:

; 206  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?LoadMenuW@CEnMenu@@QAEHIPAUHWND__@@H@Z ENDP		; CEnMenu::LoadMenuW
_TEXT	ENDS
PUBLIC	?GetMenuItemPos@CEnMenu@@QBEHI@Z		; CEnMenu::GetMenuItemPos
PUBLIC	?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@I@Z	; CEnMenu::GetMenuItemPos
;	COMDAT ?GetMenuItemPos@CEnMenu@@QBEHI@Z
_TEXT	SEGMENT
_nCmdID$ = 8
_this$ = -4
?GetMenuItemPos@CEnMenu@@QBEHI@Z PROC NEAR		; CEnMenu::GetMenuItemPos, COMDAT

; 209  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 210  : 	return GetMenuItemPos(m_hMenu, nCmdID);

	mov	eax, DWORD PTR _nCmdID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@I@Z ; CEnMenu::GetMenuItemPos
	add	esp, 8

; 211  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetMenuItemPos@CEnMenu@@QBEHI@Z ENDP			; CEnMenu::GetMenuItemPos
_TEXT	ENDS
PUBLIC	?GetMenuItemPos@CEnMenu@@QBEHPAUHMENU__@@@Z	; CEnMenu::GetMenuItemPos
PUBLIC	?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@0@Z	; CEnMenu::GetMenuItemPos
;	COMDAT ?GetMenuItemPos@CEnMenu@@QBEHPAUHMENU__@@@Z
_TEXT	SEGMENT
_hSubMenu$ = 8
_this$ = -4
?GetMenuItemPos@CEnMenu@@QBEHPAUHMENU__@@@Z PROC NEAR	; CEnMenu::GetMenuItemPos, COMDAT

; 214  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 215  : 	return GetMenuItemPos(m_hMenu, hSubMenu);

	mov	eax, DWORD PTR _hSubMenu$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@0@Z ; CEnMenu::GetMenuItemPos
	add	esp, 8

; 216  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetMenuItemPos@CEnMenu@@QBEHPAUHMENU__@@@Z ENDP	; CEnMenu::GetMenuItemPos
_TEXT	ENDS
PUBLIC	?DeleteSubMenu@CEnMenu@@QAEHPAUHMENU__@@H@Z	; CEnMenu::DeleteSubMenu
PUBLIC	?DeleteSubMenu@CEnMenu@@SAHPAUHMENU__@@0H@Z	; CEnMenu::DeleteSubMenu
;	COMDAT ?DeleteSubMenu@CEnMenu@@QAEHPAUHMENU__@@H@Z
_TEXT	SEGMENT
_hSubMenu$ = 8
_bAutoCleanUp$ = 12
_this$ = -4
?DeleteSubMenu@CEnMenu@@QAEHPAUHMENU__@@H@Z PROC NEAR	; CEnMenu::DeleteSubMenu, COMDAT

; 219  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 220  : 	return DeleteSubMenu(m_hMenu, hSubMenu, bAutoCleanUp);

	mov	eax, DWORD PTR _bAutoCleanUp$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hSubMenu$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	?DeleteSubMenu@CEnMenu@@SAHPAUHMENU__@@0H@Z ; CEnMenu::DeleteSubMenu
	add	esp, 12					; 0000000cH

; 221  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?DeleteSubMenu@CEnMenu@@QAEHPAUHMENU__@@H@Z ENDP	; CEnMenu::DeleteSubMenu
_TEXT	ENDS
PUBLIC	?DeleteMenu@CEnMenu@@SAHPAUHMENU__@@IIH@Z	; CEnMenu::DeleteMenu
;	COMDAT ?DeleteMenu@CEnMenu@@QAEHIIH@Z
_TEXT	SEGMENT
_nPosition$ = 8
_nFlags$ = 12
_bAutoCleanUp$ = 16
_this$ = -4
?DeleteMenu@CEnMenu@@QAEHIIH@Z PROC NEAR		; CEnMenu::DeleteMenu, COMDAT

; 224  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 225  : 	return DeleteMenu(m_hMenu, nPosition, nFlags, bAutoCleanUp);

	mov	eax, DWORD PTR _bAutoCleanUp$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nFlags$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nPosition$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	?DeleteMenu@CEnMenu@@SAHPAUHMENU__@@IIH@Z ; CEnMenu::DeleteMenu
	add	esp, 16					; 00000010H

; 226  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?DeleteMenu@CEnMenu@@QAEHIIH@Z ENDP			; CEnMenu::DeleteMenu
_TEXT	ENDS
PUBLIC	?SetLocalizer@CEnMenu@@SAXPAVITransText@@@Z	; CEnMenu::SetLocalizer
;	COMDAT ?SetLocalizer@CEnMenu@@SAXPAVITransText@@@Z
_TEXT	SEGMENT
_pTT$ = 8
?SetLocalizer@CEnMenu@@SAXPAVITransText@@@Z PROC NEAR	; CEnMenu::SetLocalizer, COMDAT

; 231  : {

	push	ebp
	mov	ebp, esp

; 232  : 	s_pTT = pTT;

	mov	eax, DWORD PTR _pTT$[ebp]
	mov	DWORD PTR ?s_pTT@CEnMenu@@1PAVITransText@@A, eax ; CEnMenu::s_pTT

; 233  : }

	pop	ebp
	ret	0
?SetLocalizer@CEnMenu@@SAXPAVITransText@@@Z ENDP	; CEnMenu::SetLocalizer
_TEXT	ENDS
PUBLIC	?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@0AAPAU2@@Z ; CEnMenu::GetMenuItemPos
PUBLIC	?DoCleanUp@CEnMenu@@KAXPAUHMENU__@@0H@Z		; CEnMenu::DoCleanUp
EXTRN	__imp__GetSubMenu@8:NEAR
EXTRN	__imp__DeleteMenu@12:NEAR
;	COMDAT ?DeleteSubMenu@CEnMenu@@SAHPAUHMENU__@@0H@Z
_TEXT	SEGMENT
_hMenu$ = 8
_hSubMenu$ = 12
_bAutoCleanUp$ = 16
_hItemMenu$ = -4
_nPos$ = -8
_bRes$76274 = -12
?DeleteSubMenu@CEnMenu@@SAHPAUHMENU__@@0H@Z PROC NEAR	; CEnMenu::DeleteSubMenu, COMDAT

; 236  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 237  : 	HMENU hItemMenu = NULL;

	mov	DWORD PTR _hItemMenu$[ebp], 0

; 238  : 	int nPos = GetMenuItemPos(hMenu, hSubMenu, hItemMenu);

	lea	eax, DWORD PTR _hItemMenu$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hSubMenu$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hMenu$[ebp]
	push	edx
	call	?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@0AAPAU2@@Z ; CEnMenu::GetMenuItemPos
	add	esp, 12					; 0000000cH
	mov	DWORD PTR _nPos$[ebp], eax

; 239  : 
; 240  : 	if (nPos != -1)

	cmp	DWORD PTR _nPos$[ebp], -1
	je	$L76269
$L76270:

; 242  : 		ASSERT(::GetSubMenu(hItemMenu, nPos) == hSubMenu);

	mov	esi, esp
	mov	eax, DWORD PTR _nPos$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hItemMenu$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetSubMenu@8
	cmp	esi, esp
	call	__chkesp
	cmp	eax, DWORD PTR _hSubMenu$[ebp]
	je	SHORT $L76273
	push	242					; 000000f2H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76273
	int	3
$L76273:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76270

; 243  : 
; 244  : 		BOOL bRes = ::DeleteMenu(hItemMenu, nPos, MF_BYPOSITION);

	mov	esi, esp
	push	1024					; 00000400H
	mov	eax, DWORD PTR _nPos$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hItemMenu$[ebp]
	push	ecx
	call	DWORD PTR __imp__DeleteMenu@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bRes$76274[ebp], eax

; 245  : 
; 246  : 		if (!bRes || !bAutoCleanUp)

	cmp	DWORD PTR _bRes$76274[ebp], 0
	je	SHORT $L76276
	cmp	DWORD PTR _bAutoCleanUp$[ebp], 0
	jne	SHORT $L76275
$L76276:

; 247  : 			return bRes;

	mov	eax, DWORD PTR _bRes$76274[ebp]
	jmp	SHORT $L76266
$L76275:

; 248  : 
; 249  : 		DoCleanUp(hMenu, hItemMenu, nPos);

	mov	edx, DWORD PTR _nPos$[ebp]
	push	edx
	mov	eax, DWORD PTR _hItemMenu$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hMenu$[ebp]
	push	ecx
	call	?DoCleanUp@CEnMenu@@KAXPAUHMENU__@@0H@Z	; CEnMenu::DoCleanUp
	add	esp, 12					; 0000000cH

; 250  : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76266
$L76269:

; 252  : 
; 253  : 	return FALSE;

	xor	eax, eax
$L76266:

; 254  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?DeleteSubMenu@CEnMenu@@SAHPAUHMENU__@@0H@Z ENDP	; CEnMenu::DeleteSubMenu
_TEXT	ENDS
PUBLIC	?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@IAAPAU2@@Z ; CEnMenu::GetMenuItemPos
;	COMDAT ?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@I@Z
_TEXT	SEGMENT
_hMenu$ = 8
_nCmdID$ = 12
_hItemMenu$ = -4
_nPos$ = -8
?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@I@Z PROC NEAR	; CEnMenu::GetMenuItemPos, COMDAT

; 257  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 258  : 	HMENU hItemMenu = NULL;

	mov	DWORD PTR _hItemMenu$[ebp], 0

; 259  : 	int nPos = GetMenuItemPos(hMenu, nCmdID, hItemMenu);

	lea	eax, DWORD PTR _hItemMenu$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nCmdID$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hMenu$[ebp]
	push	edx
	call	?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@IAAPAU2@@Z ; CEnMenu::GetMenuItemPos
	add	esp, 12					; 0000000cH
	mov	DWORD PTR _nPos$[ebp], eax

; 260  : 
; 261  : 	if (hItemMenu == hMenu)

	mov	eax, DWORD PTR _hItemMenu$[ebp]
	cmp	eax, DWORD PTR _hMenu$[ebp]
	jne	SHORT $L76283

; 262  : 		return nPos;

	mov	eax, DWORD PTR _nPos$[ebp]
	jmp	SHORT $L76280
$L76283:

; 263  : 
; 264  : 	// else
; 265  : 	return -1;

	or	eax, -1
$L76280:

; 266  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@I@Z ENDP	; CEnMenu::GetMenuItemPos
_TEXT	ENDS
PUBLIC	?GetMenuItemPos@CEnMenu@@KAHPAUHMENU__@@KAAPAU2@H@Z ; CEnMenu::GetMenuItemPos
;	COMDAT ?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@IAAPAU2@@Z
_TEXT	SEGMENT
_hMenu$ = 8
_nCmdID$ = 12
_hItemMenu$ = 16
?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@IAAPAU2@@Z PROC NEAR ; CEnMenu::GetMenuItemPos, COMDAT

; 269  : {

	push	ebp
	mov	ebp, esp

; 270  : 	return GetMenuItemPos(hMenu, (DWORD)nCmdID, hItemMenu, FALSE);

	push	0
	mov	eax, DWORD PTR _hItemMenu$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nCmdID$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hMenu$[ebp]
	push	edx
	call	?GetMenuItemPos@CEnMenu@@KAHPAUHMENU__@@KAAPAU2@H@Z ; CEnMenu::GetMenuItemPos
	add	esp, 16					; 00000010H

; 271  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@IAAPAU2@@Z ENDP	; CEnMenu::GetMenuItemPos
_TEXT	ENDS
;	COMDAT ?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@0@Z
_TEXT	SEGMENT
_hMenu$ = 8
_hSubMenu$ = 12
_hItemMenu$ = -4
_nPos$ = -8
?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@0@Z PROC NEAR	; CEnMenu::GetMenuItemPos, COMDAT

; 274  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 275  : 	HMENU hItemMenu = NULL;

	mov	DWORD PTR _hItemMenu$[ebp], 0

; 276  : 	int nPos = GetMenuItemPos(hMenu, hSubMenu, hItemMenu);

	lea	eax, DWORD PTR _hItemMenu$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hSubMenu$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hMenu$[ebp]
	push	edx
	call	?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@0AAPAU2@@Z ; CEnMenu::GetMenuItemPos
	add	esp, 12					; 0000000cH
	mov	DWORD PTR _nPos$[ebp], eax

; 277  : 
; 278  : 	if (hItemMenu == hMenu)

	mov	eax, DWORD PTR _hItemMenu$[ebp]
	cmp	eax, DWORD PTR _hMenu$[ebp]
	jne	SHORT $L76296

; 279  : 		return nPos;

	mov	eax, DWORD PTR _nPos$[ebp]
	jmp	SHORT $L76293
$L76296:

; 280  : 
; 281  : 	// else
; 282  : 	return -1;

	or	eax, -1
$L76293:

; 283  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@0@Z ENDP	; CEnMenu::GetMenuItemPos
_TEXT	ENDS
;	COMDAT ?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@0AAPAU2@@Z
_TEXT	SEGMENT
_hMenu$ = 8
_hSubMenu$ = 12
_hItemMenu$ = 16
?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@0AAPAU2@@Z PROC NEAR ; CEnMenu::GetMenuItemPos, COMDAT

; 286  : {

	push	ebp
	mov	ebp, esp

; 287  : 	return GetMenuItemPos(hMenu, (DWORD)hSubMenu, hItemMenu, TRUE);

	push	1
	mov	eax, DWORD PTR _hItemMenu$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hSubMenu$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hMenu$[ebp]
	push	edx
	call	?GetMenuItemPos@CEnMenu@@KAHPAUHMENU__@@KAAPAU2@H@Z ; CEnMenu::GetMenuItemPos
	add	esp, 16					; 00000010H

; 288  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@0AAPAU2@@Z ENDP	; CEnMenu::GetMenuItemPos
_TEXT	ENDS
EXTRN	__imp__GetMenuItemID@8:NEAR
EXTRN	__imp__GetMenuItemCount@4:NEAR
;	COMDAT ?GetMenuItemPos@CEnMenu@@KAHPAUHMENU__@@KAAPAU2@H@Z
_TEXT	SEGMENT
_hMenu$ = 8
_dwItem$ = 12
_hItemMenu$ = 16
_bItemIsMenu$ = 20
_nPos$ = -4
_nMenuID$76318 = -8
_hSubMenu$76319 = -12
_nSubPos$76327 = -16
?GetMenuItemPos@CEnMenu@@KAHPAUHMENU__@@KAAPAU2@H@Z PROC NEAR ; CEnMenu::GetMenuItemPos, COMDAT

; 291  : {

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

; 292  : 	hItemMenu = NULL;

	mov	eax, DWORD PTR _hItemMenu$[ebp]
	mov	DWORD PTR [eax], 0
$L76309:

; 293  : 
; 294  : 	ASSERT(dwItem > 0);

	cmp	DWORD PTR _dwItem$[ebp], 0
	ja	SHORT $L76312
	push	294					; 00000126H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76312
	int	3
$L76312:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76309

; 295  : 
; 296  : 	if (dwItem <= 0)

	cmp	DWORD PTR _dwItem$[ebp], 0
	ja	SHORT $L76313

; 297  : 		return -1;

	or	eax, -1
	jmp	$L76308
$L76313:

; 298  : 
; 299  : 	// search recursively
; 300  : 	int nPos = ::GetMenuItemCount(hMenu);

	mov	esi, esp
	mov	edx, DWORD PTR _hMenu$[ebp]
	push	edx
	call	DWORD PTR __imp__GetMenuItemCount@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nPos$[ebp], eax
$L76316:

; 301  : 
; 302  : 	while (nPos--)

	mov	eax, DWORD PTR _nPos$[ebp]
	mov	ecx, DWORD PTR _nPos$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nPos$[ebp], ecx
	test	eax, eax
	je	$L76317

; 304  : 		UINT nMenuID = ::GetMenuItemID(hMenu, nPos);

	mov	esi, esp
	mov	edx, DWORD PTR _nPos$[ebp]
	push	edx
	mov	eax, DWORD PTR _hMenu$[ebp]
	push	eax
	call	DWORD PTR __imp__GetMenuItemID@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nMenuID$76318[ebp], eax

; 305  : 		HMENU hSubMenu = ::GetSubMenu(hMenu, nPos);

	mov	esi, esp
	mov	ecx, DWORD PTR _nPos$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hMenu$[ebp]
	push	edx
	call	DWORD PTR __imp__GetSubMenu@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hSubMenu$76319[ebp], eax

; 308  : 			(!bItemIsMenu && nMenuID == (UINT)dwItem))

	cmp	DWORD PTR _bItemIsMenu$[ebp], 0
	je	SHORT $L76324
	mov	eax, DWORD PTR _hSubMenu$76319[ebp]
	cmp	eax, DWORD PTR _dwItem$[ebp]
	je	SHORT $L76323
$L76324:
	cmp	DWORD PTR _bItemIsMenu$[ebp], 0
	jne	SHORT $L76322
	mov	ecx, DWORD PTR _nMenuID$76318[ebp]
	cmp	ecx, DWORD PTR _dwItem$[ebp]
	jne	SHORT $L76322
$L76323:

; 310  : 			hItemMenu = hMenu;

	mov	edx, DWORD PTR _hItemMenu$[ebp]
	mov	eax, DWORD PTR _hMenu$[ebp]
	mov	DWORD PTR [edx], eax

; 311  : 			return nPos;

	mov	eax, DWORD PTR _nPos$[ebp]
	jmp	SHORT $L76308
$L76322:

; 313  : 		else if (hSubMenu) // search recursively

	cmp	DWORD PTR _hSubMenu$76319[ebp], 0
	je	SHORT $L76328

; 315  : 			int nSubPos = GetMenuItemPos(hSubMenu, dwItem, hItemMenu, bItemIsMenu);

	mov	ecx, DWORD PTR _bItemIsMenu$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hItemMenu$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwItem$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hSubMenu$76319[ebp]
	push	ecx
	call	?GetMenuItemPos@CEnMenu@@KAHPAUHMENU__@@KAAPAU2@H@Z ; CEnMenu::GetMenuItemPos
	add	esp, 16					; 00000010H
	mov	DWORD PTR _nSubPos$76327[ebp], eax

; 316  : 
; 317  : 			if (nSubPos != -1)

	cmp	DWORD PTR _nSubPos$76327[ebp], -1
	je	SHORT $L76328

; 318  : 				return nSubPos;

	mov	eax, DWORD PTR _nSubPos$76327[ebp]
	jmp	SHORT $L76308
$L76328:

; 320  : 	}

	jmp	$L76316
$L76317:

; 321  : 
; 322  : 	// else
; 323  : 	return -1;

	or	eax, -1
$L76308:

; 324  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetMenuItemPos@CEnMenu@@KAHPAUHMENU__@@KAAPAU2@H@Z ENDP ; CEnMenu::GetMenuItemPos
_TEXT	ENDS
;	COMDAT ?DeleteMenu@CEnMenu@@SAHPAUHMENU__@@IIH@Z
_TEXT	SEGMENT
_hMenu$ = 8
_nPosition$ = 12
_nFlags$ = 16
_bAutoCleanUp$ = 20
_hCmdMenu$ = -4
_nCmdPos$ = -16
_bByCommand$ = -12
_bRes$ = -8
?DeleteMenu@CEnMenu@@SAHPAUHMENU__@@IIH@Z PROC NEAR	; CEnMenu::DeleteMenu, COMDAT

; 327  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 328  : 	// ::DeleteMenu will do a deep delete if passed a commandID
; 329  : 	// and MF_BYCOMMAND so we first need to get a handle (pun!)
; 330  : 	// on the item's _real_ parent menu so we can cleanup after
; 331  : 	HMENU hCmdMenu = hMenu;

	mov	eax, DWORD PTR _hMenu$[ebp]
	mov	DWORD PTR _hCmdMenu$[ebp], eax

; 332  : 	int nCmdPos = nPosition;

	mov	ecx, DWORD PTR _nPosition$[ebp]
	mov	DWORD PTR _nCmdPos$[ebp], ecx

; 333  : 
; 334  : 	BOOL bByCommand = ((nFlags & MF_BYPOSITION) == 0);

	mov	edx, DWORD PTR _nFlags$[ebp]
	and	edx, 1024				; 00000400H
	neg	edx
	sbb	edx, edx
	inc	edx
	mov	DWORD PTR _bByCommand$[ebp], edx

; 335  : 
; 336  : 	if (bByCommand)

	cmp	DWORD PTR _bByCommand$[ebp], 0
	je	SHORT $L76338

; 337  : 		nCmdPos = GetMenuItemPos(hMenu, nPosition, hCmdMenu);

	lea	eax, DWORD PTR _hCmdMenu$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nPosition$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hMenu$[ebp]
	push	edx
	call	?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@IAAPAU2@@Z ; CEnMenu::GetMenuItemPos
	add	esp, 12					; 0000000cH
	mov	DWORD PTR _nCmdPos$[ebp], eax
$L76338:

; 338  : 	
; 339  : 	// then do a standard delete
; 340  : 	BOOL bRes = ::DeleteMenu(hCmdMenu, nCmdPos, MF_BYPOSITION);

	mov	esi, esp
	push	1024					; 00000400H
	mov	eax, DWORD PTR _nCmdPos$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hCmdMenu$[ebp]
	push	ecx
	call	DWORD PTR __imp__DeleteMenu@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bRes$[ebp], eax

; 341  : 
; 342  : 	// if it failed or no cleanup is required then we can exit
; 343  : 	if (!bRes || !bAutoCleanUp)

	cmp	DWORD PTR _bRes$[ebp], 0
	je	SHORT $L76341
	cmp	DWORD PTR _bAutoCleanUp$[ebp], 0
	jne	SHORT $L76340
$L76341:

; 344  : 		return bRes;

	mov	eax, DWORD PTR _bRes$[ebp]
	jmp	SHORT $L76334
$L76340:

; 345  : 
; 346  : 	DoCleanUp(hMenu, hCmdMenu, nCmdPos);

	mov	edx, DWORD PTR _nCmdPos$[ebp]
	push	edx
	mov	eax, DWORD PTR _hCmdMenu$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hMenu$[ebp]
	push	ecx
	call	?DoCleanUp@CEnMenu@@KAXPAUHMENU__@@0H@Z	; CEnMenu::DoCleanUp
	add	esp, 12					; 0000000cH

; 347  : 	return TRUE;

	mov	eax, 1
$L76334:

; 348  : }

	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?DeleteMenu@CEnMenu@@SAHPAUHMENU__@@IIH@Z ENDP		; CEnMenu::DeleteMenu
_TEXT	ENDS
;	COMDAT ?DoCleanUp@CEnMenu@@KAXPAUHMENU__@@0H@Z
_TEXT	SEGMENT
_hMenu$ = 8
_hCmdMenu$ = 12
_nCmdPos$ = 16
_nCount$ = -4
_nMenuPos$76349 = -8
_nDelPos$76352 = -12
?DoCleanUp@CEnMenu@@KAXPAUHMENU__@@0H@Z PROC NEAR	; CEnMenu::DoCleanUp, COMDAT

; 351  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 352  : 	// else if that menu is now empty we delete it
; 353  : 	int nCount = ::GetMenuItemCount(hCmdMenu);

	mov	esi, esp
	mov	eax, DWORD PTR _hCmdMenu$[ebp]
	push	eax
	call	DWORD PTR __imp__GetMenuItemCount@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nCount$[ebp], eax

; 354  : 
; 355  : 	if (nCount == 0)

	cmp	DWORD PTR _nCount$[ebp], 0
	jne	SHORT $L76348

; 357  : 		int nMenuPos = CEnMenu::GetMenuItemPos(hMenu, hCmdMenu, hCmdMenu);

	lea	ecx, DWORD PTR _hCmdMenu$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hCmdMenu$[ebp]
	push	edx
	mov	eax, DWORD PTR _hMenu$[ebp]
	push	eax
	call	?GetMenuItemPos@CEnMenu@@SAHPAUHMENU__@@0AAPAU2@@Z ; CEnMenu::GetMenuItemPos
	add	esp, 12					; 0000000cH
	mov	DWORD PTR _nMenuPos$76349[ebp], eax

; 358  : 		
; 359  : 		if (::DeleteMenu(hCmdMenu, nMenuPos, MF_BYPOSITION))

	mov	esi, esp
	push	1024					; 00000400H
	mov	ecx, DWORD PTR _nMenuPos$76349[ebp]
	push	ecx
	mov	edx, DWORD PTR _hCmdMenu$[ebp]
	push	edx
	call	DWORD PTR __imp__DeleteMenu@12
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L76350

; 361  : 			DoCleanUp(hMenu, hCmdMenu, nMenuPos); // RECURSIVE CALL

	mov	eax, DWORD PTR _nMenuPos$76349[ebp]
	push	eax
	mov	ecx, DWORD PTR _hCmdMenu$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hMenu$[ebp]
	push	edx
	call	?DoCleanUp@CEnMenu@@KAXPAUHMENU__@@0H@Z	; CEnMenu::DoCleanUp
	add	esp, 12					; 0000000cH
$L76350:

; 364  : 	else // check for stray separators

	jmp	$L76358
$L76348:

; 366  : 		int nDelPos = -1;

	mov	DWORD PTR _nDelPos$76352[ebp], -1

; 367  : 
; 368  : 		// at start
; 369  : 		if (::GetMenuItemID(hCmdMenu, 0) == 0)

	mov	esi, esp
	push	0
	mov	eax, DWORD PTR _hCmdMenu$[ebp]
	push	eax
	call	DWORD PTR __imp__GetMenuItemID@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76353

; 370  : 			nDelPos = 0;

	mov	DWORD PTR _nDelPos$76352[ebp], 0

; 371  : 
; 372  : 		// at end
; 373  : 		else if (::GetMenuItemID(hCmdMenu, nCount - 1) == 0)

	jmp	SHORT $L76357
$L76353:
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	esi, esp
	push	ecx
	mov	edx, DWORD PTR _hCmdMenu$[ebp]
	push	edx
	call	DWORD PTR __imp__GetMenuItemID@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76355

; 374  : 			nDelPos = nCount - 1;

	mov	eax, DWORD PTR _nCount$[ebp]
	sub	eax, 1
	mov	DWORD PTR _nDelPos$76352[ebp], eax

; 375  : 
; 376  : 		// doubles
; 377  : 		else if (::GetMenuItemID(hCmdMenu, nCmdPos) == 0 &&	nCmdPos > 0 && 

	jmp	SHORT $L76357
$L76355:

; 378  : 				 ::GetMenuItemID(hCmdMenu, nCmdPos - 1) == 0)

	mov	esi, esp
	mov	ecx, DWORD PTR _nCmdPos$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hCmdMenu$[ebp]
	push	edx
	call	DWORD PTR __imp__GetMenuItemID@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76357
	cmp	DWORD PTR _nCmdPos$[ebp], 0
	jle	SHORT $L76357
	mov	eax, DWORD PTR _nCmdPos$[ebp]
	sub	eax, 1
	mov	esi, esp
	push	eax
	mov	ecx, DWORD PTR _hCmdMenu$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetMenuItemID@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76357

; 379  : 			nDelPos = nCmdPos;

	mov	edx, DWORD PTR _nCmdPos$[ebp]
	mov	DWORD PTR _nDelPos$76352[ebp], edx
$L76357:

; 380  : 
; 381  : 		if (nDelPos != -1)

	cmp	DWORD PTR _nDelPos$76352[ebp], -1
	je	SHORT $L76358

; 382  : 			::DeleteMenu(hCmdMenu, nDelPos, MF_BYPOSITION);

	mov	esi, esp
	push	1024					; 00000400H
	mov	eax, DWORD PTR _nDelPos$76352[ebp]
	push	eax
	mov	ecx, DWORD PTR _hCmdMenu$[ebp]
	push	ecx
	call	DWORD PTR __imp__DeleteMenu@12
	cmp	esi, esp
	call	__chkesp
$L76358:

; 384  : }

	pop	esi
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?DoCleanUp@CEnMenu@@KAXPAUHMENU__@@0H@Z ENDP		; CEnMenu::DoCleanUp
_TEXT	ENDS
PUBLIC	?Serialize@?$CMap@IIHH@@UAEXAAVCArchive@@@Z	; CMap<unsigned int,unsigned int,int,int>::Serialize
PUBLIC	?Dump@?$CMap@IIHH@@UBEXAAVCDumpContext@@@Z	; CMap<unsigned int,unsigned int,int,int>::Dump
PUBLIC	?AssertValid@?$CMap@IIHH@@UBEXXZ		; CMap<unsigned int,unsigned int,int,int>::AssertValid
PUBLIC	??_7?$CMap@IIHH@@6B@				; CMap<unsigned int,unsigned int,int,int>::`vftable'
PUBLIC	??_G?$CMap@IIHH@@UAEPAXI@Z			; CMap<unsigned int,unsigned int,int,int>::`scalar deleting destructor'
PUBLIC	??_E?$CMap@IIHH@@UAEPAXI@Z			; CMap<unsigned int,unsigned int,int,int>::`vector deleting destructor'
EXTRN	?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ:NEAR ; CObject::GetRuntimeClass
EXTRN	??1CObject@@UAE@XZ:NEAR				; CObject::~CObject
EXTRN	??0CObject@@IAE@XZ:NEAR				; CObject::CObject
;	COMDAT ??_7?$CMap@IIHH@@6B@
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
CONST	SEGMENT
??_7?$CMap@IIHH@@6B@ DD FLAT:?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ ; CMap<unsigned int,unsigned int,int,int>::`vftable'
	DD	FLAT:??_E?$CMap@IIHH@@UAEPAXI@Z
	DD	FLAT:?Serialize@?$CMap@IIHH@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@?$CMap@IIHH@@UBEXXZ
	DD	FLAT:?Dump@?$CMap@IIHH@@UBEXAAVCDumpContext@@@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77070	DD	019930520H
	DD	01H
	DD	FLAT:$T77072
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77072	DD	0ffffffffH
	DD	FLAT:$L77068
xdata$x	ENDS
;	COMDAT ??0?$CMap@IIHH@@QAE@H@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_nBlockSize$ = 8
_this$ = -16
??0?$CMap@IIHH@@QAE@H@Z PROC NEAR			; CMap<unsigned int,unsigned int,int,int>::CMap<unsigned int,unsigned int,int,int>, COMDAT

; 1201 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77071
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
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CMap@IIHH@@6B@ ; CMap<unsigned int,unsigned int,int,int>::`vftable'
$L76447:

; 1202 : 	ASSERT(nBlockSize > 0);

	cmp	DWORD PTR _nBlockSize$[ebp], 0
	jg	SHORT $L76450
	push	1202					; 000004b2H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76450
	int	3
$L76450:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76447

; 1203 : 
; 1204 : 	m_pHashTable = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0

; 1205 : 	m_nHashTableSize = 17;  // default size

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+8], 17			; 00000011H

; 1206 : 	m_nCount = 0;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+12], 0

; 1207 : 	m_pFreeList = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+16], 0

; 1208 : 	m_pBlocks = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+20], 0

; 1209 : 	m_nBlockSize = nBlockSize;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nBlockSize$[ebp]
	mov	DWORD PTR [ecx+24], edx

; 1210 : }

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
$L77068:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L77071:
	mov	eax, OFFSET FLAT:$T77070
	jmp	___CxxFrameHandler
text$x	ENDS
??0?$CMap@IIHH@@QAE@H@Z ENDP				; CMap<unsigned int,unsigned int,int,int>::CMap<unsigned int,unsigned int,int,int>
PUBLIC	?GetAssocAt@?$CMap@IIHH@@IBEPAUCAssoc@1@IAAI@Z	; CMap<unsigned int,unsigned int,int,int>::GetAssocAt
EXTRN	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z:NEAR ; AfxAssertValidObject
;	COMDAT ?Lookup@?$CMap@IIHH@@QBEHIAAH@Z
_TEXT	SEGMENT
_key$ = 8
_rValue$ = 12
_this$ = -12
_nHash$ = -8
_pAssoc$ = -4
?Lookup@?$CMap@IIHH@@QBEHIAAH@Z PROC NEAR		; CMap<unsigned int,unsigned int,int,int>::Lookup, COMDAT

; 1341 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1342 : 	ASSERT_VALID(this);

	push	1342					; 0000053eH
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1343 : 
; 1344 : 	UINT nHash;
; 1345 : 	CAssoc* pAssoc = GetAssocAt(key, nHash);

	lea	ecx, DWORD PTR _nHash$[ebp]
	push	ecx
	mov	edx, DWORD PTR _key$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetAssocAt@?$CMap@IIHH@@IBEPAUCAssoc@1@IAAI@Z ; CMap<unsigned int,unsigned int,int,int>::GetAssocAt
	mov	DWORD PTR _pAssoc$[ebp], eax

; 1346 : 	if (pAssoc == NULL)

	cmp	DWORD PTR _pAssoc$[ebp], 0
	jne	SHORT $L76458

; 1347 : 		return FALSE;  // not in map

	xor	eax, eax
	jmp	SHORT $L76455
$L76458:

; 1348 : 
; 1349 : 	rValue = pAssoc->value;

	mov	eax, DWORD PTR _rValue$[ebp]
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	mov	DWORD PTR [eax], edx

; 1350 : 	return TRUE;

	mov	eax, 1
$L76455:

; 1351 : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Lookup@?$CMap@IIHH@@QBEHIAAH@Z ENDP			; CMap<unsigned int,unsigned int,int,int>::Lookup
_TEXT	ENDS
PUBLIC	?InitHashTable@?$CMap@IIHH@@QAEXIH@Z		; CMap<unsigned int,unsigned int,int,int>::InitHashTable
PUBLIC	?NewAssoc@?$CMap@IIHH@@IAEPAUCAssoc@1@XZ	; CMap<unsigned int,unsigned int,int,int>::NewAssoc
;	COMDAT ??A?$CMap@IIHH@@QAEAAHI@Z
_TEXT	SEGMENT
_key$ = 8
_this$ = -12
_nHash$ = -8
_pAssoc$ = -4
??A?$CMap@IIHH@@QAEAAHI@Z PROC NEAR			; CMap<unsigned int,unsigned int,int,int>::operator[], COMDAT

; 1355 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1356 : 	ASSERT_VALID(this);

	push	1356					; 0000054cH
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1357 : 
; 1358 : 	UINT nHash;
; 1359 : 	CAssoc* pAssoc;
; 1360 : 	if ((pAssoc = GetAssocAt(key, nHash)) == NULL)

	lea	ecx, DWORD PTR _nHash$[ebp]
	push	ecx
	mov	edx, DWORD PTR _key$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetAssocAt@?$CMap@IIHH@@IBEPAUCAssoc@1@IAAI@Z ; CMap<unsigned int,unsigned int,int,int>::GetAssocAt
	mov	DWORD PTR _pAssoc$[ebp], eax
	cmp	DWORD PTR _pAssoc$[ebp], 0
	jne	SHORT $L76465

; 1362 : 		if (m_pHashTable == NULL)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L76466

; 1363 : 			InitHashTable(m_nHashTableSize);

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?InitHashTable@?$CMap@IIHH@@QAEXIH@Z	; CMap<unsigned int,unsigned int,int,int>::InitHashTable
$L76466:

; 1364 : 
; 1365 : 		// it doesn't exist, add a new Association
; 1366 : 		pAssoc = NewAssoc();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?NewAssoc@?$CMap@IIHH@@IAEPAUCAssoc@1@XZ ; CMap<unsigned int,unsigned int,int,int>::NewAssoc
	mov	DWORD PTR _pAssoc$[ebp], eax

; 1367 : 		pAssoc->nHashValue = nHash;

	mov	eax, DWORD PTR _pAssoc$[ebp]
	mov	ecx, DWORD PTR _nHash$[ebp]
	mov	DWORD PTR [eax+4], ecx

; 1368 : 		pAssoc->key = key;

	mov	edx, DWORD PTR _pAssoc$[ebp]
	mov	eax, DWORD PTR _key$[ebp]
	mov	DWORD PTR [edx+8], eax

; 1369 : 		// 'pAssoc->value' is a constructed object, nothing more
; 1370 : 
; 1371 : 		// put into hash table
; 1372 : 		pAssoc->pNext = m_pHashTable[nHash];

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR _pAssoc$[ebp]
	mov	ecx, DWORD PTR _nHash$[ebp]
	mov	edx, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR [eax], edx

; 1373 : 		m_pHashTable[nHash] = pAssoc;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _nHash$[ebp]
	mov	eax, DWORD PTR _pAssoc$[ebp]
	mov	DWORD PTR [ecx+edx*4], eax
$L76465:

; 1375 : 	return pAssoc->value;  // return new reference

	mov	eax, DWORD PTR _pAssoc$[ebp]
	add	eax, 12					; 0000000cH

; 1376 : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??A?$CMap@IIHH@@QAEAAHI@Z ENDP				; CMap<unsigned int,unsigned int,int,int>::operator[]
_TEXT	ENDS
PUBLIC	?FreeAssoc@?$CMap@IIHH@@IAEXPAUCAssoc@1@@Z	; CMap<unsigned int,unsigned int,int,int>::FreeAssoc
PUBLIC	?HashKey@@YGII@Z				; HashKey
PUBLIC	?CompareElements@@YGHPBI0@Z			; CompareElements
;	COMDAT ?RemoveKey@?$CMap@IIHH@@QAEHI@Z
_TEXT	SEGMENT
_key$ = 8
_this$ = -12
_ppAssocPrev$ = -4
_pAssoc$ = -8
?RemoveKey@?$CMap@IIHH@@QAEHI@Z PROC NEAR		; CMap<unsigned int,unsigned int,int,int>::RemoveKey, COMDAT

; 1381 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1382 : 	ASSERT_VALID(this);

	push	1382					; 00000566H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1383 : 
; 1384 : 	if (m_pHashTable == NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	jne	SHORT $L76471

; 1385 : 		return FALSE;  // nothing in the table

	xor	eax, eax
	jmp	SHORT $L76470
$L76471:

; 1388 : 	ppAssocPrev = &m_pHashTable[HashKey<ARG_KEY>(key) % m_nHashTableSize];

	mov	edx, DWORD PTR _key$[ebp]
	push	edx
	call	?HashKey@@YGII@Z			; HashKey
	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	div	DWORD PTR [ecx+8]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	lea	edx, DWORD PTR [ecx+edx*4]
	mov	DWORD PTR _ppAssocPrev$[ebp], edx

; 1389 : 
; 1390 : 	CAssoc* pAssoc;
; 1391 : 	for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)

	mov	eax, DWORD PTR _ppAssocPrev$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssoc$[ebp], ecx
	jmp	SHORT $L76478
$L76479:
	mov	edx, DWORD PTR _pAssoc$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _pAssoc$[ebp], eax
$L76478:
	cmp	DWORD PTR _pAssoc$[ebp], 0
	je	SHORT $L76480

; 1393 : 		if (CompareElements(&pAssoc->key, &key))

	lea	ecx, DWORD PTR _key$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pAssoc$[ebp]
	add	edx, 8
	push	edx
	call	?CompareElements@@YGHPBI0@Z		; CompareElements
	test	eax, eax
	je	SHORT $L76487

; 1395 : 			// remove it
; 1396 : 			*ppAssocPrev = pAssoc->pNext;  // remove from list

	mov	eax, DWORD PTR _ppAssocPrev$[ebp]
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR [eax], edx

; 1397 : 			FreeAssoc(pAssoc);

	mov	eax, DWORD PTR _pAssoc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FreeAssoc@?$CMap@IIHH@@IAEXPAUCAssoc@1@@Z ; CMap<unsigned int,unsigned int,int,int>::FreeAssoc

; 1398 : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L76470
$L76487:

; 1400 : 		ppAssocPrev = &pAssoc->pNext;

	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	DWORD PTR _ppAssocPrev$[ebp], ecx

; 1401 : 	}

	jmp	SHORT $L76479
$L76480:

; 1402 : 	return FALSE;  // not found

	xor	eax, eax
$L76470:

; 1403 : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?RemoveKey@?$CMap@IIHH@@QAEHI@Z ENDP			; CMap<unsigned int,unsigned int,int,int>::RemoveKey
_TEXT	ENDS
PUBLIC	?RemoveAll@?$CMap@IIHH@@QAEXXZ			; CMap<unsigned int,unsigned int,int,int>::RemoveAll
;	COMDAT xdata$x
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
xdata$x	SEGMENT
$T77085	DD	019930520H
	DD	01H
	DD	FLAT:$T77087
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77087	DD	0ffffffffH
	DD	FLAT:$L77083
xdata$x	ENDS
;	COMDAT ??1?$CMap@IIHH@@UAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??1?$CMap@IIHH@@UAE@XZ PROC NEAR			; CMap<unsigned int,unsigned int,int,int>::~CMap<unsigned int,unsigned int,int,int>, COMDAT

; 1270 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77086
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
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CMap@IIHH@@6B@ ; CMap<unsigned int,unsigned int,int,int>::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1271 : 	RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CMap@IIHH@@QAEXXZ		; CMap<unsigned int,unsigned int,int,int>::RemoveAll
$L76491:

; 1272 : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L76494
	push	1272					; 000004f8H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76494
	int	3
$L76494:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76491

; 1273 : }

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
$L77083:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L77086:
	mov	eax, OFFSET FLAT:$T77085
	jmp	___CxxFrameHandler
text$x	ENDS
??1?$CMap@IIHH@@UAE@XZ ENDP				; CMap<unsigned int,unsigned int,int,int>::~CMap<unsigned int,unsigned int,int,int>
PUBLIC	?SetAt@?$CMap@IIHH@@QAEXIH@Z			; CMap<unsigned int,unsigned int,int,int>::SetAt
PUBLIC	?SerializeElements@@YGXAAVCArchive@@PAIH@Z	; SerializeElements
PUBLIC	?SerializeElements@@YGXAAVCArchive@@PAHH@Z	; SerializeElements
EXTRN	?ReadCount@CArchive@@QAEKXZ:NEAR		; CArchive::ReadCount
EXTRN	?WriteCount@CArchive@@QAEXK@Z:NEAR		; CArchive::WriteCount
EXTRN	?IsStoring@CArchive@@QBEHXZ:NEAR		; CArchive::IsStoring
;	COMDAT ?Serialize@?$CMap@IIHH@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT
_ar$ = 8
_this$ = -24
_nHash$76505 = -4
_pAssoc$76509 = -8
_nNewCount$76526 = -12
_newKey$76530 = -16
_newValue$76531 = -20
?Serialize@?$CMap@IIHH@@UAEXAAVCArchive@@@Z PROC NEAR	; CMap<unsigned int,unsigned int,int,int>::Serialize, COMDAT

; 1445 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 24					; 00000018H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 1446 : 	ASSERT_VALID(this);

	push	1446					; 000005a6H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1447 : 
; 1448 : 	CObject::Serialize(ar);

	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Serialize@CObject@@UAEXAAVCArchive@@@Z	; CObject::Serialize

; 1449 : 
; 1450 : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	$L76499

; 1452 : 		ar.WriteCount(m_nCount);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?WriteCount@CArchive@@QAEXK@Z		; CArchive::WriteCount

; 1453 : 		if (m_nCount == 0)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jne	SHORT $L76500

; 1454 : 			return;  // nothing more to do

	jmp	$L76498
$L76500:

; 1455 : 
; 1456 : 		ASSERT(m_pHashTable != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	jne	SHORT $L76504
	push	1456					; 000005b0H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76504
	int	3
$L76504:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76500

; 1457 : 		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)

	mov	DWORD PTR _nHash$76505[ebp], 0
	jmp	SHORT $L76506
$L76507:
	mov	ecx, DWORD PTR _nHash$76505[ebp]
	add	ecx, 1
	mov	DWORD PTR _nHash$76505[ebp], ecx
$L76506:
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nHash$76505[ebp]
	cmp	eax, DWORD PTR [edx+8]
	jae	SHORT $L76508

; 1459 : 			CAssoc* pAssoc;
; 1460 : 			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR _nHash$76505[ebp]
	mov	ecx, DWORD PTR [edx+eax*4]
	mov	DWORD PTR _pAssoc$76509[ebp], ecx

; 1461 : 			  pAssoc = pAssoc->pNext)

	jmp	SHORT $L76510
$L76511:
	mov	edx, DWORD PTR _pAssoc$76509[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _pAssoc$76509[ebp], eax
$L76510:
	cmp	DWORD PTR _pAssoc$76509[ebp], 0
	je	SHORT $L76512

; 1463 : 				SerializeElements<KEY>(ar, &pAssoc->key, 1);

	push	1
	mov	ecx, DWORD PTR _pAssoc$76509[ebp]
	add	ecx, 8
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAIH@Z ; SerializeElements

; 1464 : 				SerializeElements<VALUE>(ar, &pAssoc->value, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$76509[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	call	?SerializeElements@@YGXAAVCArchive@@PAHH@Z ; SerializeElements

; 1465 : 			}

	jmp	SHORT $L76511
$L76512:

; 1466 : 		}

	jmp	SHORT $L76507
$L76508:

; 1468 : 	else

	jmp	SHORT $L76529
$L76499:

; 1470 : 		DWORD nNewCount = ar.ReadCount();

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?ReadCount@CArchive@@QAEKXZ		; CArchive::ReadCount
	mov	DWORD PTR _nNewCount$76526[ebp], eax
$L76528:

; 1471 : 		while (nNewCount--)

	mov	edx, DWORD PTR _nNewCount$76526[ebp]
	mov	eax, DWORD PTR _nNewCount$76526[ebp]
	sub	eax, 1
	mov	DWORD PTR _nNewCount$76526[ebp], eax
	test	edx, edx
	je	SHORT $L76529

; 1473 : 			KEY newKey;
; 1474 : 			VALUE newValue;
; 1475 : 			SerializeElements<KEY>(ar, &newKey, 1);

	push	1
	lea	ecx, DWORD PTR _newKey$76530[ebp]
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAIH@Z ; SerializeElements

; 1476 : 			SerializeElements<VALUE>(ar, &newValue, 1);

	push	1
	lea	eax, DWORD PTR _newValue$76531[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	call	?SerializeElements@@YGXAAVCArchive@@PAHH@Z ; SerializeElements

; 1477 : 			SetAt(newKey, newValue);

	mov	edx, DWORD PTR _newValue$76531[ebp]
	push	edx
	mov	eax, DWORD PTR _newKey$76530[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetAt@?$CMap@IIHH@@QAEXIH@Z		; CMap<unsigned int,unsigned int,int,int>::SetAt

; 1478 : 		}

	jmp	SHORT $L76528
$L76529:
$L76498:

; 1480 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Serialize@?$CMap@IIHH@@UAEXAAVCArchive@@@Z ENDP	; CMap<unsigned int,unsigned int,int,int>::Serialize
_TEXT	ENDS
PUBLIC	?GetStartPosition@?$CMap@IIHH@@QBEPAU__POSITION@@XZ ; CMap<unsigned int,unsigned int,int,int>::GetStartPosition
PUBLIC	?GetNextAssoc@?$CMap@IIHH@@QBEXAAPAU__POSITION@@AAIAAH@Z ; CMap<unsigned int,unsigned int,int,int>::GetNextAssoc
PUBLIC	??_C@_05KIFI@with?5?$AA@			; `string'
PUBLIC	??_C@_09LOPF@?5elements?$AA@			; `string'
PUBLIC	??_C@_03DANH@?6?7?$FL?$AA@			; `string'
PUBLIC	?DumpElements@@YGXAAVCDumpContext@@PBIH@Z	; DumpElements
PUBLIC	??_C@_04MNLE@?$FN?5?$DN?5?$AA@			; `string'
PUBLIC	?DumpElements@@YGXAAVCDumpContext@@PBHH@Z	; DumpElements
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
;	COMDAT ??_C@_03DANH@?6?7?$FL?$AA@
_DATA	SEGMENT
??_C@_03DANH@?6?7?$FL?$AA@ DB 0aH, 09H, '[', 00H	; `string'
_DATA	ENDS
;	COMDAT ??_C@_04MNLE@?$FN?5?$DN?5?$AA@
_DATA	SEGMENT
??_C@_04MNLE@?$FN?5?$DN?5?$AA@ DB '] = ', 00H		; `string'
_DATA	ENDS
;	COMDAT ??_C@_01BJG@?6?$AA@
_DATA	SEGMENT
??_C@_01BJG@?6?$AA@ DB 0aH, 00H				; `string'
_DATA	ENDS
;	COMDAT ?Dump@?$CMap@IIHH@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT
_dc$ = 8
_this$ = -16
_key$76549 = -8
_val$76550 = -4
_pos$76551 = -12
?Dump@?$CMap@IIHH@@UBEXAAVCDumpContext@@@Z PROC NEAR	; CMap<unsigned int,unsigned int,int,int>::Dump, COMDAT

; 1485 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 1486 : 	CObject::Dump(dc);

	mov	eax, DWORD PTR _dc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Dump@CObject@@UBEXAAVCDumpContext@@@Z	; CObject::Dump

; 1487 : 
; 1488 : 	dc << "with " << m_nCount << " elements";

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

; 1489 : 	if (dc.GetDepth() > 0)

	mov	ecx, DWORD PTR _dc$[ebp]
	call	?GetDepth@CDumpContext@@QBEHXZ		; CDumpContext::GetDepth
	test	eax, eax
	jle	SHORT $L76554

; 1491 : 		// Dump in format "[key] -> value"
; 1492 : 		KEY key;
; 1493 : 		VALUE val;
; 1494 : 
; 1495 : 		POSITION pos = GetStartPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetStartPosition@?$CMap@IIHH@@QBEPAU__POSITION@@XZ ; CMap<unsigned int,unsigned int,int,int>::GetStartPosition
	mov	DWORD PTR _pos$76551[ebp], eax
$L76553:

; 1496 : 		while (pos != NULL)

	cmp	DWORD PTR _pos$76551[ebp], 0
	je	SHORT $L76554

; 1498 : 			GetNextAssoc(pos, key, val);

	lea	eax, DWORD PTR _val$76550[ebp]
	push	eax
	lea	ecx, DWORD PTR _key$76549[ebp]
	push	ecx
	lea	edx, DWORD PTR _pos$76551[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetNextAssoc@?$CMap@IIHH@@QBEXAAPAU__POSITION@@AAIAAH@Z ; CMap<unsigned int,unsigned int,int,int>::GetNextAssoc

; 1499 : 			dc << "\n\t[";

	push	OFFSET FLAT:??_C@_03DANH@?6?7?$FL?$AA@	; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1500 : 			DumpElements<KEY>(dc, &key, 1);

	push	1
	lea	eax, DWORD PTR _key$76549[ebp]
	push	eax
	mov	ecx, DWORD PTR _dc$[ebp]
	push	ecx
	call	?DumpElements@@YGXAAVCDumpContext@@PBIH@Z ; DumpElements

; 1501 : 			dc << "] = ";

	push	OFFSET FLAT:??_C@_04MNLE@?$FN?5?$DN?5?$AA@ ; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1502 : 			DumpElements<VALUE>(dc, &val, 1);

	push	1
	lea	edx, DWORD PTR _val$76550[ebp]
	push	edx
	mov	eax, DWORD PTR _dc$[ebp]
	push	eax
	call	?DumpElements@@YGXAAVCDumpContext@@PBHH@Z ; DumpElements

; 1503 : 		}

	jmp	SHORT $L76553
$L76554:

; 1505 : 
; 1506 : 	dc << "\n";

	push	OFFSET FLAT:??_C@_01BJG@?6?$AA@		; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1507 : }

	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Dump@?$CMap@IIHH@@UBEXAAVCDumpContext@@@Z ENDP		; CMap<unsigned int,unsigned int,int,int>::Dump
_TEXT	ENDS
EXTRN	?AssertValid@CObject@@UBEXXZ:NEAR		; CObject::AssertValid
;	COMDAT ?AssertValid@?$CMap@IIHH@@UBEXXZ
_TEXT	SEGMENT
_this$ = -4
?AssertValid@?$CMap@IIHH@@UBEXXZ PROC NEAR		; CMap<unsigned int,unsigned int,int,int>::AssertValid, COMDAT

; 1511 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1512 : 	CObject::AssertValid();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?AssertValid@CObject@@UBEXXZ		; CObject::AssertValid
$L76573:

; 1513 : 
; 1514 : 	ASSERT(m_nHashTableSize > 0);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	ja	SHORT $L76576
	push	1514					; 000005eaH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76576
	int	3
$L76576:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76573
$L76575:

; 1515 : 	ASSERT(m_nCount == 0 || m_pHashTable != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+12], 0
	je	SHORT $L76580
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L76580
	push	1515					; 000005ebH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76580
	int	3
$L76580:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76575

; 1516 : 		// non-empty map should have hash table
; 1517 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?AssertValid@?$CMap@IIHH@@UBEXXZ ENDP			; CMap<unsigned int,unsigned int,int,int>::AssertValid
_TEXT	ENDS
;	COMDAT ??_G?$CMap@IIHH@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_G?$CMap@IIHH@@UAEPAXI@Z PROC NEAR			; CMap<unsigned int,unsigned int,int,int>::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1?$CMap@IIHH@@UAE@XZ			; CMap<unsigned int,unsigned int,int,int>::~CMap<unsigned int,unsigned int,int,int>
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L76584
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L76584:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_G?$CMap@IIHH@@UAEPAXI@Z ENDP				; CMap<unsigned int,unsigned int,int,int>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@IIHH@@QAEXIH@Z
_TEXT	SEGMENT
_key$ = 8
_newValue$ = 12
_this$ = -4
?SetAt@?$CMap@IIHH@@QAEXIH@Z PROC NEAR			; CMap<unsigned int,unsigned int,int,int>::SetAt, COMDAT

; 1188 : 	{ (*this)[key] = newValue; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _key$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??A?$CMap@IIHH@@QAEAAHI@Z		; CMap<unsigned int,unsigned int,int,int>::operator[]
	mov	ecx, DWORD PTR _newValue$[ebp]
	mov	DWORD PTR [eax], ecx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SetAt@?$CMap@IIHH@@QAEXIH@Z ENDP			; CMap<unsigned int,unsigned int,int,int>::SetAt
_TEXT	ENDS
PUBLIC	?DestructElements@@YGXPAHH@Z			; DestructElements
PUBLIC	?DestructElements@@YGXPAIH@Z			; DestructElements
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
EXTRN	?FreeDataChain@CPlex@@QAEXXZ:NEAR		; CPlex::FreeDataChain
;	COMDAT ?RemoveAll@?$CMap@IIHH@@QAEXXZ
_TEXT	SEGMENT
$T77102 = -12
_this$ = -16
_nHash$76594 = -4
_pAssoc$76598 = -8
?RemoveAll@?$CMap@IIHH@@QAEXXZ PROC NEAR		; CMap<unsigned int,unsigned int,int,int>::RemoveAll, COMDAT

; 1240 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 1241 : 	ASSERT_VALID(this);

	push	1241					; 000004d9H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 1242 : 
; 1243 : 	if (m_pHashTable != NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L76597

; 1245 : 		// destroy elements (values and keys)
; 1246 : 		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)

	mov	DWORD PTR _nHash$76594[ebp], 0
	jmp	SHORT $L76595
$L76596:
	mov	edx, DWORD PTR _nHash$76594[ebp]
	add	edx, 1
	mov	DWORD PTR _nHash$76594[ebp], edx
$L76595:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nHash$76594[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jae	SHORT $L76597

; 1248 : 			CAssoc* pAssoc;
; 1249 : 			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _nHash$76594[ebp]
	mov	edx, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR _pAssoc$76598[ebp], edx

; 1250 : 			  pAssoc = pAssoc->pNext)

	jmp	SHORT $L76599
$L76600:
	mov	eax, DWORD PTR _pAssoc$76598[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssoc$76598[ebp], ecx
$L76599:
	cmp	DWORD PTR _pAssoc$76598[ebp], 0
	je	SHORT $L76601

; 1252 : 				DestructElements<VALUE>(&pAssoc->value, 1);

	push	1
	mov	edx, DWORD PTR _pAssoc$76598[ebp]
	add	edx, 12					; 0000000cH
	push	edx
	call	?DestructElements@@YGXPAHH@Z		; DestructElements

; 1253 : 				DestructElements<KEY>(&pAssoc->key, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$76598[ebp]
	add	eax, 8
	push	eax
	call	?DestructElements@@YGXPAIH@Z		; DestructElements

; 1254 : 			}

	jmp	SHORT $L76600
$L76601:

; 1255 : 		}

	jmp	SHORT $L76596
$L76597:

; 1257 : 
; 1258 : 	// free hash table
; 1259 : 	delete[] m_pHashTable;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR $T77102[ebp], edx
	mov	eax, DWORD PTR $T77102[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 1260 : 	m_pHashTable = NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+4], 0

; 1261 : 
; 1262 : 	m_nCount = 0;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12], 0

; 1263 : 	m_pFreeList = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+16], 0

; 1264 : 	m_pBlocks->FreeDataChain();

	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+20]
	call	?FreeDataChain@CPlex@@QAEXXZ		; CPlex::FreeDataChain

; 1265 : 	m_pBlocks = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+20], 0

; 1266 : }

	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?RemoveAll@?$CMap@IIHH@@QAEXXZ ENDP			; CMap<unsigned int,unsigned int,int,int>::RemoveAll
_TEXT	ENDS
;	COMDAT ?GetStartPosition@?$CMap@IIHH@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetStartPosition@?$CMap@IIHH@@QBEPAU__POSITION@@XZ PROC NEAR ; CMap<unsigned int,unsigned int,int,int>::GetStartPosition, COMDAT

; 1191 : 	{ return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+12]
	neg	eax
	sbb	eax, eax
	mov	esp, ebp
	pop	ebp
	ret	0
?GetStartPosition@?$CMap@IIHH@@QBEPAU__POSITION@@XZ ENDP ; CMap<unsigned int,unsigned int,int,int>::GetStartPosition
_TEXT	ENDS
EXTRN	?AfxIsValidAddress@@YGHPBXIH@Z:NEAR		; AfxIsValidAddress
;	COMDAT ?GetNextAssoc@?$CMap@IIHH@@QBEXAAPAU__POSITION@@AAIAAH@Z
_TEXT	SEGMENT
_rNextPosition$ = 8
_rKey$ = 12
_rValue$ = 16
_this$ = -20
_pAssocRet$ = -4
_nBucket$76636 = -12
_pAssocNext$ = -8
_nBucket$76652 = -16
?GetNextAssoc@?$CMap@IIHH@@QBEXAAPAU__POSITION@@AAIAAH@Z PROC NEAR ; CMap<unsigned int,unsigned int,int,int>::GetNextAssoc, COMDAT

; 1408 : {

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

; 1409 : 	ASSERT_VALID(this);

	push	1409					; 00000581H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject
$L76623:

; 1410 : 	ASSERT(m_pHashTable != NULL);  // never call on empty map

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	jne	SHORT $L76626
	push	1410					; 00000582H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76626
	int	3
$L76626:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76623

; 1411 : 
; 1412 : 	CAssoc* pAssocRet = (CAssoc*)rNextPosition;

	mov	eax, DWORD PTR _rNextPosition$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssocRet$[ebp], ecx
$L76629:

; 1413 : 	ASSERT(pAssocRet != NULL);

	cmp	DWORD PTR _pAssocRet$[ebp], 0
	jne	SHORT $L76632
	push	1413					; 00000585H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76632
	int	3
$L76632:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76629

; 1414 : 
; 1415 : 	if (pAssocRet == (CAssoc*) BEFORE_START_POSITION)

	cmp	DWORD PTR _pAssocRet$[ebp], -1
	jne	SHORT $L76643

; 1417 : 		// find the first association
; 1418 : 		for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)

	mov	DWORD PTR _nBucket$76636[ebp], 0
	jmp	SHORT $L76637
$L76638:
	mov	eax, DWORD PTR _nBucket$76636[ebp]
	add	eax, 1
	mov	DWORD PTR _nBucket$76636[ebp], eax
$L76637:
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nBucket$76636[ebp]
	cmp	edx, DWORD PTR [ecx+8]
	jae	SHORT $L76639

; 1419 : 			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _nBucket$76636[ebp]
	mov	eax, DWORD PTR [ecx+edx*4]
	mov	DWORD PTR _pAssocRet$[ebp], eax
	cmp	DWORD PTR _pAssocRet$[ebp], 0
	je	SHORT $L76640

; 1420 : 				break;

	jmp	SHORT $L76639
$L76640:

; 1421 : 		ASSERT(pAssocRet != NULL);  // must find something

	jmp	SHORT $L76638
$L76639:
	cmp	DWORD PTR _pAssocRet$[ebp], 0
	jne	SHORT $L76644
	push	1421					; 0000058dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76644
	int	3
$L76644:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76639
$L76643:

; 1423 : 
; 1424 : 	// find next association
; 1425 : 	ASSERT(AfxIsValidAddress(pAssocRet, sizeof(CAssoc)));

	push	1
	push	16					; 00000010H
	mov	edx, DWORD PTR _pAssocRet$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76649
	push	1425					; 00000591H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76649
	int	3
$L76649:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76643

; 1426 : 	CAssoc* pAssocNext;
; 1427 : 	if ((pAssocNext = pAssocRet->pNext) == NULL)

	mov	ecx, DWORD PTR _pAssocRet$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _pAssocNext$[ebp], edx
	cmp	DWORD PTR _pAssocNext$[ebp], 0
	jne	SHORT $L76655

; 1429 : 		// go to next bucket
; 1430 : 		for (UINT nBucket = pAssocRet->nHashValue + 1;

	mov	eax, DWORD PTR _pAssocRet$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	add	ecx, 1
	mov	DWORD PTR _nBucket$76652[ebp], ecx

; 1431 : 		  nBucket < m_nHashTableSize; nBucket++)

	jmp	SHORT $L76653
$L76654:
	mov	edx, DWORD PTR _nBucket$76652[ebp]
	add	edx, 1
	mov	DWORD PTR _nBucket$76652[ebp], edx
$L76653:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nBucket$76652[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jae	SHORT $L76655

; 1432 : 			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _nBucket$76652[ebp]
	mov	edx, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR _pAssocNext$[ebp], edx
	cmp	DWORD PTR _pAssocNext$[ebp], 0
	je	SHORT $L76656

; 1433 : 				break;

	jmp	SHORT $L76655
$L76656:

; 1434 : 	}

	jmp	SHORT $L76654
$L76655:

; 1435 : 
; 1436 : 	rNextPosition = (POSITION) pAssocNext;

	mov	eax, DWORD PTR _rNextPosition$[ebp]
	mov	ecx, DWORD PTR _pAssocNext$[ebp]
	mov	DWORD PTR [eax], ecx

; 1437 : 
; 1438 : 	// fill in return data
; 1439 : 	rKey = pAssocRet->key;

	mov	edx, DWORD PTR _rKey$[ebp]
	mov	eax, DWORD PTR _pAssocRet$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR [edx], ecx

; 1440 : 	rValue = pAssocRet->value;

	mov	edx, DWORD PTR _rValue$[ebp]
	mov	eax, DWORD PTR _pAssocRet$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR [edx], ecx

; 1441 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?GetNextAssoc@?$CMap@IIHH@@QBEXAAPAU__POSITION@@AAIAAH@Z ENDP ; CMap<unsigned int,unsigned int,int,int>::GetNextAssoc
_TEXT	ENDS
EXTRN	_memset:NEAR
EXTRN	??2@YAPAXIPBDH@Z:NEAR				; operator new
;	COMDAT ?InitHashTable@?$CMap@IIHH@@QAEXIH@Z
_TEXT	SEGMENT
$T77109 = -4
$T77110 = -8
_nHashSize$ = 8
_bAllocNow$ = 12
_this$ = -12
?InitHashTable@?$CMap@IIHH@@QAEXIH@Z PROC NEAR		; CMap<unsigned int,unsigned int,int,int>::InitHashTable, COMDAT

; 1218 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1219 : 	ASSERT_VALID(this);

	push	1219					; 000004c3H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject
$L76663:

; 1220 : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L76666
	push	1220					; 000004c4H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76666
	int	3
$L76666:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76663
$L76665:

; 1221 : 	ASSERT(nHashSize > 0);

	cmp	DWORD PTR _nHashSize$[ebp], 0
	ja	SHORT $L76670
	push	1221					; 000004c5H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76670
	int	3
$L76670:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76665

; 1222 : 
; 1223 : 	if (m_pHashTable != NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L76671

; 1225 : 		// free hash table
; 1226 : 		delete[] m_pHashTable;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR $T77109[ebp], eax
	mov	ecx, DWORD PTR $T77109[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 1227 : 		m_pHashTable = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0
$L76671:

; 1229 : 
; 1230 : 	if (bAllocNow)

	cmp	DWORD PTR _bAllocNow$[ebp], 0
	je	SHORT $L76673

; 1232 : 		m_pHashTable = new CAssoc* [nHashSize];

	push	1232					; 000004d0H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _nHashSize$[ebp]
	shl	eax, 2
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77110[ebp], eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR $T77110[ebp]
	mov	DWORD PTR [ecx+4], edx

; 1233 : 		memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);

	mov	eax, DWORD PTR _nHashSize$[ebp]
	shl	eax, 2
	push	eax
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	_memset
	add	esp, 12					; 0000000cH
$L76673:

; 1235 : 	m_nHashTableSize = nHashSize;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nHashSize$[ebp]
	mov	DWORD PTR [eax+8], ecx

; 1236 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?InitHashTable@?$CMap@IIHH@@QAEXIH@Z ENDP		; CMap<unsigned int,unsigned int,int,int>::InitHashTable
_TEXT	ENDS
PUBLIC	?data@CPlex@@QAEPAXXZ				; CPlex::data
PUBLIC	?ConstructElements@@YGXPAIH@Z			; ConstructElements
PUBLIC	?ConstructElements@@YGXPAHH@Z			; ConstructElements
EXTRN	?Create@CPlex@@SGPAU1@AAPAU1@II@Z:NEAR		; CPlex::Create
;	COMDAT ?NewAssoc@?$CMap@IIHH@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT
_this$ = -20
_newBlock$76681 = -8
_pAssoc$76683 = -16
_i$76685 = -12
_pAssoc$ = -4
?NewAssoc@?$CMap@IIHH@@IAEPAUCAssoc@1@XZ PROC NEAR	; CMap<unsigned int,unsigned int,int,int>::NewAssoc, COMDAT

; 1278 : {

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

; 1279 : 	if (m_pFreeList == NULL)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	jne	SHORT $L76688

; 1281 : 		// add another block
; 1282 : 		CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CMap::CAssoc));

	push	16					; 00000010H
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 20					; 00000014H
	push	eax
	call	?Create@CPlex@@SGPAU1@AAPAU1@II@Z	; CPlex::Create
	mov	DWORD PTR _newBlock$76681[ebp], eax

; 1283 : 		// chain them into free list
; 1284 : 		CMap::CAssoc* pAssoc = (CMap::CAssoc*) newBlock->data();

	mov	ecx, DWORD PTR _newBlock$76681[ebp]
	call	?data@CPlex@@QAEPAXXZ			; CPlex::data
	mov	DWORD PTR _pAssoc$76683[ebp], eax

; 1285 : 		// free in reverse order to make it easier to debug
; 1286 : 		pAssoc += m_nBlockSize - 1;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	shl	edx, 4
	mov	eax, DWORD PTR _pAssoc$76683[ebp]
	add	eax, edx
	mov	DWORD PTR _pAssoc$76683[ebp], eax

; 1287 : 		for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	mov	DWORD PTR _i$76685[ebp], edx
	jmp	SHORT $L76686
$L76687:
	mov	eax, DWORD PTR _i$76685[ebp]
	sub	eax, 1
	mov	DWORD PTR _i$76685[ebp], eax
	mov	ecx, DWORD PTR _pAssoc$76683[ebp]
	sub	ecx, 16					; 00000010H
	mov	DWORD PTR _pAssoc$76683[ebp], ecx
$L76686:
	cmp	DWORD PTR _i$76685[ebp], 0
	jl	SHORT $L76688

; 1289 : 			pAssoc->pNext = m_pFreeList;

	mov	edx, DWORD PTR _pAssoc$76683[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR [edx], ecx

; 1290 : 			m_pFreeList = pAssoc;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _pAssoc$76683[ebp]
	mov	DWORD PTR [edx+16], eax

; 1291 : 		}

	jmp	SHORT $L76687
$L76688:

; 1293 : 	ASSERT(m_pFreeList != NULL);  // we must have something

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+16], 0
	jne	SHORT $L76692
	push	1293					; 0000050dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76692
	int	3
$L76692:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76688

; 1294 : 
; 1295 : 	CMap::CAssoc* pAssoc = m_pFreeList;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR _pAssoc$[ebp], ecx

; 1296 : 	m_pFreeList = m_pFreeList->pNext;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	DWORD PTR [ecx+16], edx

; 1297 : 	m_nCount++;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	add	ecx, 1
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12], ecx
$L76694:

; 1298 : 	ASSERT(m_nCount > 0);  // make sure we don't overflow

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jg	SHORT $L76697
	push	1298					; 00000512H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76697
	int	3
$L76697:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76694

; 1299 : 	ConstructElements<KEY>(&pAssoc->key, 1);

	push	1
	mov	edx, DWORD PTR _pAssoc$[ebp]
	add	edx, 8
	push	edx
	call	?ConstructElements@@YGXPAIH@Z		; ConstructElements

; 1300 : 	ConstructElements<VALUE>(&pAssoc->value, 1);   // special construct values

	push	1
	mov	eax, DWORD PTR _pAssoc$[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	call	?ConstructElements@@YGXPAHH@Z		; ConstructElements

; 1301 : 	return pAssoc;

	mov	eax, DWORD PTR _pAssoc$[ebp]

; 1302 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?NewAssoc@?$CMap@IIHH@@IAEPAUCAssoc@1@XZ ENDP		; CMap<unsigned int,unsigned int,int,int>::NewAssoc
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
;	COMDAT ?FreeAssoc@?$CMap@IIHH@@IAEXPAUCAssoc@1@@Z
_TEXT	SEGMENT
_pAssoc$ = 8
_this$ = -4
?FreeAssoc@?$CMap@IIHH@@IAEXPAUCAssoc@1@@Z PROC NEAR	; CMap<unsigned int,unsigned int,int,int>::FreeAssoc, COMDAT

; 1306 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1307 : 	DestructElements<VALUE>(&pAssoc->value, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	call	?DestructElements@@YGXPAHH@Z		; DestructElements

; 1308 : 	DestructElements<KEY>(&pAssoc->key, 1);

	push	1
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	add	ecx, 8
	push	ecx
	call	?DestructElements@@YGXPAIH@Z		; DestructElements

; 1309 : 	pAssoc->pNext = m_pFreeList;

	mov	edx, DWORD PTR _pAssoc$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR [edx], ecx

; 1310 : 	m_pFreeList = pAssoc;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _pAssoc$[ebp]
	mov	DWORD PTR [edx+16], eax

; 1311 : 	m_nCount--;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	sub	edx, 1
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+12], edx
$L76720:

; 1312 : 	ASSERT(m_nCount >= 0);  // make sure we don't underflow

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jge	SHORT $L76723
	push	1312					; 00000520H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76723
	int	3
$L76723:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76720

; 1313 : 
; 1314 : 	// if no more elements, cleanup completely
; 1315 : 	if (m_nCount == 0)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jne	SHORT $L76724

; 1316 : 		RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CMap@IIHH@@QAEXXZ		; CMap<unsigned int,unsigned int,int,int>::RemoveAll
$L76724:

; 1317 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?FreeAssoc@?$CMap@IIHH@@IAEXPAUCAssoc@1@@Z ENDP		; CMap<unsigned int,unsigned int,int,int>::FreeAssoc
_TEXT	ENDS
;	COMDAT ?GetAssocAt@?$CMap@IIHH@@IBEPAUCAssoc@1@IAAI@Z
_TEXT	SEGMENT
_key$ = 8
_nHash$ = 12
_this$ = -8
_pAssoc$ = -4
?GetAssocAt@?$CMap@IIHH@@IBEPAUCAssoc@1@IAAI@Z PROC NEAR ; CMap<unsigned int,unsigned int,int,int>::GetAssocAt, COMDAT

; 1323 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1324 : 	nHash = HashKey<ARG_KEY>(key) % m_nHashTableSize;

	mov	eax, DWORD PTR _key$[ebp]
	push	eax
	call	?HashKey@@YGII@Z			; HashKey
	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	div	DWORD PTR [ecx+8]
	mov	eax, DWORD PTR _nHash$[ebp]
	mov	DWORD PTR [eax], edx

; 1325 : 
; 1326 : 	if (m_pHashTable == NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	jne	SHORT $L76733

; 1327 : 		return NULL;

	xor	eax, eax
	jmp	SHORT $L76729
$L76733:

; 1331 : 	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)

	mov	edx, DWORD PTR _nHash$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR [edx+eax*4]
	mov	DWORD PTR _pAssoc$[ebp], eax
	jmp	SHORT $L76735
$L76736:
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _pAssoc$[ebp], edx
$L76735:
	cmp	DWORD PTR _pAssoc$[ebp], 0
	je	SHORT $L76737

; 1333 : 		if (CompareElements(&pAssoc->key, &key))

	lea	eax, DWORD PTR _key$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	add	ecx, 8
	push	ecx
	call	?CompareElements@@YGHPBI0@Z		; CompareElements
	test	eax, eax
	je	SHORT $L76743

; 1334 : 			return pAssoc;

	mov	eax, DWORD PTR _pAssoc$[ebp]
	jmp	SHORT $L76729
$L76743:

; 1335 : 	}

	jmp	SHORT $L76736
$L76737:

; 1336 : 	return NULL;

	xor	eax, eax
$L76729:

; 1337 : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?GetAssocAt@?$CMap@IIHH@@IBEPAUCAssoc@1@IAAI@Z ENDP	; CMap<unsigned int,unsigned int,int,int>::GetAssocAt
_TEXT	ENDS
;	COMDAT ?HashKey@@YGII@Z
_TEXT	SEGMENT
_key$ = 8
?HashKey@@YGII@Z PROC NEAR				; HashKey, COMDAT

; 127  : {

	push	ebp
	mov	ebp, esp

; 128  : 	// default identity hash - works for most primitive values
; 129  : 	return ((UINT)(void*)(DWORD)key) >> 4;

	mov	eax, DWORD PTR _key$[ebp]
	shr	eax, 4

; 130  : }

	pop	ebp
	ret	4
?HashKey@@YGII@Z ENDP					; HashKey
_TEXT	ENDS
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
$L76749:

; 119  : 	ASSERT(AfxIsValidAddress(pElement1, sizeof(TYPE), FALSE));

	push	0
	push	4
	mov	eax, DWORD PTR _pElement1$[ebp]
	push	eax
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76753
	push	119					; 00000077H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76753
	int	3
$L76753:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76749
$L76751:

; 120  : 	ASSERT(AfxIsValidAddress(pElement2, sizeof(ARG_TYPE), FALSE));

	push	0
	push	4
	mov	edx, DWORD PTR _pElement2$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76758
	push	120					; 00000078H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76758
	int	3
$L76758:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76751

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
$L76760:

; 92   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76764
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76764
	push	93					; 0000005dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76764
	int	3
$L76764:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76760

; 93   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 94   : 
; 95   : 	// default is bit-wise read/write
; 96   : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L76765

; 97   : 		ar.Write((void*)pElements, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Write@CArchive@@QAEXPBXI@Z		; CArchive::Write

; 98   : 	else

	jmp	SHORT $L76768
$L76765:

; 99   : 		ar.Read((void*)pElements, nCount * sizeof(TYPE));

	mov	edx, DWORD PTR _nCount$[ebp]
	shl	edx, 2
	push	edx
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Read@CArchive@@QAEIPAXI@Z		; CArchive::Read
$L76768:

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
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAHH@Z
_TEXT	SEGMENT
_ar$ = 8
_pElements$ = 12
_nCount$ = 16
?SerializeElements@@YGXAAVCArchive@@PAHH@Z PROC NEAR	; SerializeElements, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L76772:

; 92   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76776
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76776
	push	93					; 0000005dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76776
	int	3
$L76776:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76772

; 93   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 94   : 
; 95   : 	// default is bit-wise read/write
; 96   : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L76777

; 97   : 		ar.Write((void*)pElements, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Write@CArchive@@QAEXPBXI@Z		; CArchive::Write

; 98   : 	else

	jmp	SHORT $L76780
$L76777:

; 99   : 		ar.Read((void*)pElements, nCount * sizeof(TYPE));

	mov	edx, DWORD PTR _nCount$[ebp]
	shl	edx, 2
	push	edx
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Read@CArchive@@QAEIPAXI@Z		; CArchive::Read
$L76780:

; 100  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	12					; 0000000cH
?SerializeElements@@YGXAAVCArchive@@PAHH@Z ENDP		; SerializeElements
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
$L76784:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76788
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76788
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76788
	int	3
$L76788:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76784

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
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBHH@Z
_TEXT	SEGMENT
_pElements$ = 12
_nCount$ = 16
?DumpElements@@YGXAAVCDumpContext@@PBHH@Z PROC NEAR	; DumpElements, COMDAT

; 105  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L76790:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76794
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76794
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76794
	int	3
$L76794:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76790

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
?DumpElements@@YGXAAVCDumpContext@@PBHH@Z ENDP		; DumpElements
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAHH@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
?DestructElements@@YGXPAHH@Z PROC NEAR			; DestructElements, COMDAT

; 67   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L76796:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76800
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76800
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76800
	int	3
$L76800:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76796

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L76801
$L76802:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 4
	mov	DWORD PTR _pElements$[ebp], eax
$L76801:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L76803

; 73   : 		pElements->~TYPE();

	jmp	SHORT $L76802
$L76803:

; 74   : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?DestructElements@@YGXPAHH@Z ENDP			; DestructElements
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
$L76805:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76809
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76809
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76809
	int	3
$L76809:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76805

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L76810
$L76811:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 4
	mov	DWORD PTR _pElements$[ebp], eax
$L76810:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L76812

; 73   : 		pElements->~TYPE();

	jmp	SHORT $L76811
$L76812:

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
;	COMDAT ?ConstructElements@@YGXPAIH@Z
_TEXT	SEGMENT
$T77137 = -4
_pElements$ = 8
_nCount$ = 12
?ConstructElements@@YGXPAIH@Z PROC NEAR			; ConstructElements, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L76814:

; 54   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76818
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76818
	push	55					; 00000037H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76818
	int	3
$L76818:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76814

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

	jmp	SHORT $L76821
$L76822:
	mov	edx, DWORD PTR _pElements$[ebp]
	add	edx, 4
	mov	DWORD PTR _pElements$[ebp], edx
$L76821:
	mov	eax, DWORD PTR _nCount$[ebp]
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
	test	eax, eax
	je	SHORT $L76823

; 62   : 		::new((void*)pElements) TYPE;

	mov	edx, DWORD PTR _pElements$[ebp]
	push	edx
	push	4
	call	??2@YAPAXIPAX@Z				; operator new
	add	esp, 8
	mov	DWORD PTR $T77137[ebp], eax
	jmp	SHORT $L76822
$L76823:

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
;	COMDAT ?ConstructElements@@YGXPAHH@Z
_TEXT	SEGMENT
$T77142 = -4
_pElements$ = 8
_nCount$ = 12
?ConstructElements@@YGXPAHH@Z PROC NEAR			; ConstructElements, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L76828:

; 54   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76832
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76832
	push	55					; 00000037H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76832
	int	3
$L76832:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76828

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

	jmp	SHORT $L76835
$L76836:
	mov	edx, DWORD PTR _pElements$[ebp]
	add	edx, 4
	mov	DWORD PTR _pElements$[ebp], edx
$L76835:
	mov	eax, DWORD PTR _nCount$[ebp]
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
	test	eax, eax
	je	SHORT $L76837

; 62   : 		::new((void*)pElements) TYPE;

	mov	edx, DWORD PTR _pElements$[ebp]
	push	edx
	push	4
	call	??2@YAPAXIPAX@Z				; operator new
	add	esp, 8
	mov	DWORD PTR $T77142[ebp], eax
	jmp	SHORT $L76836
$L76837:

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
?ConstructElements@@YGXPAHH@Z ENDP			; ConstructElements
_TEXT	ENDS
END

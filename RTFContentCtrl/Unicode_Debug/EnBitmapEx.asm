	TITLE	D:\_CODE\Shared\EnBitmapEx.cpp
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
;	COMDAT ??0CImageSysColorMapper@@QAE@XZ
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
;	COMDAT ??0CEnBitmapEx@@QAE@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCEnBitmapEx@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CEnBitmapEx@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadSysBitmap@CEnBitmapEx@@QAEHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RotateImage@CEnBitmapEx@@QAEHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ShearImage@CEnBitmapEx@@QAEHHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GrayImage@CEnBitmapEx@@QAEHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?BlurImage@CEnBitmapEx@@QAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SharpenImage@CEnBitmapEx@@QAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ResizeImage@CEnBitmapEx@@QAEHN@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LightenImage@CEnBitmapEx@@QAEHNK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FlipImage@CEnBitmapEx@@QAEHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NegateImage@CEnBitmapEx@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReplaceColor@CEnBitmapEx@@QAEHKK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ColorizeImage@CEnBitmapEx@@QAEHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ContrastImage@CEnBitmapEx@@QAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?TintImage@CEnBitmapEx@@QAEHKHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemapSysColors@CEnBitmapEx@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GrayScale@CEnBitmapEx@@QAEHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GrayScale@CEnBitmapEx@@SAHAAVCBitmap@@K@Z
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
;	COMDAT ??0?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Add@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEHPAVC32BitImageProcessor@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetAtGrow@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHPAVC32BitImageProcessor@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAPAVC32BitImageProcessor@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAPAVC32BitImageProcessor@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBQAVC32BitImageProcessor@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAPAVC32BitImageProcessor@@H@Z
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
;	COMDAT ??_7?$CArray@PAVC32BitImageProcessor@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CEnBitmapEx@@6B@
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
_DATA	SEGMENT
__szAfxTempl DB	'afxtempl.h', 00H
	ORG $+1
_THIS_FILE DB	'D:\_CODE\Shared\EnBitmapEx.cpp', 00H
_DATA	ENDS
PUBLIC	??0CEnBitmapEx@@QAE@K@Z				; CEnBitmapEx::CEnBitmapEx
PUBLIC	??_7CEnBitmapEx@@6B@				; CEnBitmapEx::`vftable'
PUBLIC	??_GCEnBitmapEx@@UAEPAXI@Z			; CEnBitmapEx::`scalar deleting destructor'
PUBLIC	??_ECEnBitmapEx@@UAEPAXI@Z			; CEnBitmapEx::`vector deleting destructor'
EXTRN	?AssertValid@CGdiObject@@UBEXXZ:NEAR		; CGdiObject::AssertValid
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	?GetRuntimeClass@CBitmap@@UBEPAUCRuntimeClass@@XZ:NEAR ; CBitmap::GetRuntimeClass
EXTRN	?Dump@CBitmap@@UBEXAAVCDumpContext@@@Z:NEAR	; CBitmap::Dump
EXTRN	??0CEnBitmap@@QAE@K@Z:NEAR			; CEnBitmap::CEnBitmap
EXTRN	__chkesp:NEAR
;	COMDAT ??_7CEnBitmapEx@@6B@
; File D:\_CODE\Shared\EnBitmapEx.cpp
CONST	SEGMENT
??_7CEnBitmapEx@@6B@ DD FLAT:?GetRuntimeClass@CBitmap@@UBEPAUCRuntimeClass@@XZ ; CEnBitmapEx::`vftable'
	DD	FLAT:??_ECEnBitmapEx@@UAEPAXI@Z
	DD	FLAT:?Serialize@CObject@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@CGdiObject@@UBEXXZ
	DD	FLAT:?Dump@CBitmap@@UBEXAAVCDumpContext@@@Z
CONST	ENDS
;	COMDAT ??0CEnBitmapEx@@QAE@K@Z
_TEXT	SEGMENT
_crBkgnd$ = 8
_this$ = -4
??0CEnBitmapEx@@QAE@K@Z PROC NEAR			; CEnBitmapEx::CEnBitmapEx, COMDAT

; 24   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _crBkgnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CEnBitmap@@QAE@K@Z			; CEnBitmap::CEnBitmap
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CEnBitmapEx@@6B@ ; CEnBitmapEx::`vftable'

; 25   : 
; 26   : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0CEnBitmapEx@@QAE@K@Z ENDP				; CEnBitmapEx::CEnBitmapEx
_TEXT	ENDS
PUBLIC	??1CEnBitmapEx@@UAE@XZ				; CEnBitmapEx::~CEnBitmapEx
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_GCEnBitmapEx@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCEnBitmapEx@@UAEPAXI@Z PROC NEAR			; CEnBitmapEx::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CEnBitmapEx@@UAE@XZ			; CEnBitmapEx::~CEnBitmapEx
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L74575
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L74575:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCEnBitmapEx@@UAEPAXI@Z ENDP				; CEnBitmapEx::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	??1CEnBitmap@@UAE@XZ:NEAR			; CEnBitmap::~CEnBitmap
;	COMDAT ??1CEnBitmapEx@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CEnBitmapEx@@UAE@XZ PROC NEAR			; CEnBitmapEx::~CEnBitmapEx, COMDAT

; 29   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CEnBitmapEx@@6B@ ; CEnBitmapEx::`vftable'

; 30   : 
; 31   : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CEnBitmap@@UAE@XZ			; CEnBitmap::~CEnBitmap
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CEnBitmapEx@@UAE@XZ ENDP				; CEnBitmapEx::~CEnBitmapEx
_TEXT	ENDS
PUBLIC	?LoadSysBitmap@CEnBitmapEx@@QAEHI@Z		; CEnBitmapEx::LoadSysBitmap
PUBLIC	?RemapSysColors@CEnBitmapEx@@QAEHXZ		; CEnBitmapEx::RemapSysColors
EXTRN	?LoadBitmapW@CBitmap@@QAEHI@Z:NEAR		; CBitmap::LoadBitmapW
;	COMDAT ?LoadSysBitmap@CEnBitmapEx@@QAEHI@Z
_TEXT	SEGMENT
_nID$ = 8
_this$ = -4
?LoadSysBitmap@CEnBitmapEx@@QAEHI@Z PROC NEAR		; CEnBitmapEx::LoadSysBitmap, COMDAT

; 34   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 35   : 	if (LoadBitmap(nID))

	mov	eax, DWORD PTR _nID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LoadBitmapW@CBitmap@@QAEHI@Z		; CBitmap::LoadBitmapW
	test	eax, eax
	je	SHORT $L74583

; 36   : 		return RemapSysColors();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemapSysColors@CEnBitmapEx@@QAEHXZ	; CEnBitmapEx::RemapSysColors
	jmp	SHORT $L74582
$L74583:

; 37   : 
; 38   : 	return FALSE;

	xor	eax, eax
$L74582:

; 39   : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?LoadSysBitmap@CEnBitmapEx@@QAEHI@Z ENDP		; CEnBitmapEx::LoadSysBitmap
_TEXT	ENDS
PUBLIC	?RotateImage@CEnBitmapEx@@QAEHHH@Z		; CEnBitmapEx::RotateImage
EXTRN	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z:NEAR ; CEnBitmap::ProcessImage
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CImageRotator@@QAE@HH@Z:NEAR			; CImageRotator::CImageRotator
EXTRN	??1CImageRotator@@UAE@XZ:NEAR			; CImageRotator::~CImageRotator
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75166	DD	019930520H
	DD	01H
	DD	FLAT:$T75169
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75169	DD	0ffffffffH
	DD	FLAT:$L75162
xdata$x	ENDS
;	COMDAT ?RotateImage@CEnBitmapEx@@QAEHHH@Z
_TEXT	SEGMENT
$T75161 = -32
__$EHRec$ = -12
_nDegrees$ = 8
_bEnableWeighting$ = 12
_this$ = -36
_proc$ = -28
?RotateImage@CEnBitmapEx@@QAEHHH@Z PROC NEAR		; CEnBitmapEx::RotateImage, COMDAT

; 42   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75167
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 43   : 	CImageRotator proc(nDegrees, bEnableWeighting);

	mov	eax, DWORD PTR _bEnableWeighting$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nDegrees$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??0CImageRotator@@QAE@HH@Z		; CImageRotator::CImageRotator
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 44   : 	return ProcessImage(&proc);

	push	-1
	lea	edx, DWORD PTR _proc$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T75161[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageRotator@@UAE@XZ		; CImageRotator::~CImageRotator
	mov	eax, DWORD PTR $T75161[ebp]

; 45   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75162:
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageRotator@@UAE@XZ		; CImageRotator::~CImageRotator
	ret	0
$L75167:
	mov	eax, OFFSET FLAT:$T75166
	jmp	___CxxFrameHandler
text$x	ENDS
?RotateImage@CEnBitmapEx@@QAEHHH@Z ENDP			; CEnBitmapEx::RotateImage
PUBLIC	?ShearImage@CEnBitmapEx@@QAEHHHH@Z		; CEnBitmapEx::ShearImage
EXTRN	??0CImageShearer@@QAE@HHH@Z:NEAR		; CImageShearer::CImageShearer
EXTRN	??1CImageShearer@@UAE@XZ:NEAR			; CImageShearer::~CImageShearer
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75178	DD	019930520H
	DD	01H
	DD	FLAT:$T75180
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75180	DD	0ffffffffH
	DD	FLAT:$L75176
xdata$x	ENDS
;	COMDAT ?ShearImage@CEnBitmapEx@@QAEHHHH@Z
_TEXT	SEGMENT
$T75175 = -32
__$EHRec$ = -12
_nHorz$ = 8
_nVert$ = 12
_bEnableWeighting$ = 16
_this$ = -36
_proc$ = -28
?ShearImage@CEnBitmapEx@@QAEHHHH@Z PROC NEAR		; CEnBitmapEx::ShearImage, COMDAT

; 48   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75179
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 49   : 	CImageShearer proc(nHorz, nVert, bEnableWeighting);

	mov	eax, DWORD PTR _bEnableWeighting$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nVert$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nHorz$[ebp]
	push	edx
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??0CImageShearer@@QAE@HHH@Z		; CImageShearer::CImageShearer
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 50   : 	return ProcessImage(&proc);

	push	-1
	lea	eax, DWORD PTR _proc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T75175[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageShearer@@UAE@XZ		; CImageShearer::~CImageShearer
	mov	eax, DWORD PTR $T75175[ebp]

; 51   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75176:
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageShearer@@UAE@XZ		; CImageShearer::~CImageShearer
	ret	0
$L75179:
	mov	eax, OFFSET FLAT:$T75178
	jmp	___CxxFrameHandler
text$x	ENDS
?ShearImage@CEnBitmapEx@@QAEHHHH@Z ENDP			; CEnBitmapEx::ShearImage
PUBLIC	?GrayImage@CEnBitmapEx@@QAEHK@Z			; CEnBitmapEx::GrayImage
EXTRN	??0CImageGrayer@@QAE@XZ:NEAR			; CImageGrayer::CImageGrayer
EXTRN	??1CImageGrayer@@UAE@XZ:NEAR			; CImageGrayer::~CImageGrayer
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75188	DD	019930520H
	DD	01H
	DD	FLAT:$T75190
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75190	DD	0ffffffffH
	DD	FLAT:$L75186
xdata$x	ENDS
;	COMDAT ?GrayImage@CEnBitmapEx@@QAEHK@Z
_TEXT	SEGMENT
$T75185 = -56
__$EHRec$ = -12
_crMask$ = 8
_this$ = -60
_proc$ = -52
?GrayImage@CEnBitmapEx@@QAEHK@Z PROC NEAR		; CEnBitmapEx::GrayImage, COMDAT

; 54   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75189
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

; 55   : 	CImageGrayer proc;

	lea	ecx, DWORD PTR _proc$[ebp]
	call	??0CImageGrayer@@QAE@XZ			; CImageGrayer::CImageGrayer
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 56   : 	return ProcessImage(&proc, crMask);

	mov	eax, DWORD PTR _crMask$[ebp]
	push	eax
	lea	ecx, DWORD PTR _proc$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T75185[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageGrayer@@UAE@XZ			; CImageGrayer::~CImageGrayer
	mov	eax, DWORD PTR $T75185[ebp]

; 57   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 60					; 0000003cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75186:
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageGrayer@@UAE@XZ			; CImageGrayer::~CImageGrayer
	ret	0
$L75189:
	mov	eax, OFFSET FLAT:$T75188
	jmp	___CxxFrameHandler
text$x	ENDS
?GrayImage@CEnBitmapEx@@QAEHK@Z ENDP			; CEnBitmapEx::GrayImage
PUBLIC	?BlurImage@CEnBitmapEx@@QAEHH@Z			; CEnBitmapEx::BlurImage
EXTRN	??0CImageBlurrer@@QAE@H@Z:NEAR			; CImageBlurrer::CImageBlurrer
EXTRN	??1CImageBlurrer@@UAE@XZ:NEAR			; CImageBlurrer::~CImageBlurrer
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75198	DD	019930520H
	DD	01H
	DD	FLAT:$T75200
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75200	DD	0ffffffffH
	DD	FLAT:$L75196
xdata$x	ENDS
;	COMDAT ?BlurImage@CEnBitmapEx@@QAEHH@Z
_TEXT	SEGMENT
$T75195 = -28
__$EHRec$ = -12
_nAmount$ = 8
_this$ = -32
_proc$ = -24
?BlurImage@CEnBitmapEx@@QAEHH@Z PROC NEAR		; CEnBitmapEx::BlurImage, COMDAT

; 60   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75199
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

; 61   : 	CImageBlurrer proc(nAmount);

	mov	eax, DWORD PTR _nAmount$[ebp]
	push	eax
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??0CImageBlurrer@@QAE@H@Z		; CImageBlurrer::CImageBlurrer
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 62   : 	return ProcessImage(&proc);

	push	-1
	lea	ecx, DWORD PTR _proc$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T75195[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageBlurrer@@UAE@XZ		; CImageBlurrer::~CImageBlurrer
	mov	eax, DWORD PTR $T75195[ebp]

; 63   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75196:
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageBlurrer@@UAE@XZ		; CImageBlurrer::~CImageBlurrer
	ret	0
$L75199:
	mov	eax, OFFSET FLAT:$T75198
	jmp	___CxxFrameHandler
text$x	ENDS
?BlurImage@CEnBitmapEx@@QAEHH@Z ENDP			; CEnBitmapEx::BlurImage
PUBLIC	?SharpenImage@CEnBitmapEx@@QAEHH@Z		; CEnBitmapEx::SharpenImage
EXTRN	??0CImageSharpener@@QAE@H@Z:NEAR		; CImageSharpener::CImageSharpener
EXTRN	??1CImageSharpener@@UAE@XZ:NEAR			; CImageSharpener::~CImageSharpener
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75208	DD	019930520H
	DD	01H
	DD	FLAT:$T75210
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75210	DD	0ffffffffH
	DD	FLAT:$L75206
xdata$x	ENDS
;	COMDAT ?SharpenImage@CEnBitmapEx@@QAEHH@Z
_TEXT	SEGMENT
$T75205 = -28
__$EHRec$ = -12
_nAmount$ = 8
_this$ = -32
_proc$ = -24
?SharpenImage@CEnBitmapEx@@QAEHH@Z PROC NEAR		; CEnBitmapEx::SharpenImage, COMDAT

; 66   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75209
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

; 67   : 	CImageSharpener proc(nAmount);

	mov	eax, DWORD PTR _nAmount$[ebp]
	push	eax
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??0CImageSharpener@@QAE@H@Z		; CImageSharpener::CImageSharpener
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 68   : 	return ProcessImage(&proc);

	push	-1
	lea	ecx, DWORD PTR _proc$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T75205[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageSharpener@@UAE@XZ		; CImageSharpener::~CImageSharpener
	mov	eax, DWORD PTR $T75205[ebp]

; 69   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75206:
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageSharpener@@UAE@XZ		; CImageSharpener::~CImageSharpener
	ret	0
$L75209:
	mov	eax, OFFSET FLAT:$T75208
	jmp	___CxxFrameHandler
text$x	ENDS
?SharpenImage@CEnBitmapEx@@QAEHH@Z ENDP			; CEnBitmapEx::SharpenImage
PUBLIC	?ResizeImage@CEnBitmapEx@@QAEHN@Z		; CEnBitmapEx::ResizeImage
EXTRN	__fltused:NEAR
EXTRN	??0CImageResizer@@QAE@N@Z:NEAR			; CImageResizer::CImageResizer
EXTRN	??1CImageResizer@@UAE@XZ:NEAR			; CImageResizer::~CImageResizer
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75218	DD	019930520H
	DD	01H
	DD	FLAT:$T75221
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75221	DD	0ffffffffH
	DD	FLAT:$L75216
xdata$x	ENDS
;	COMDAT ?ResizeImage@CEnBitmapEx@@QAEHN@Z
_TEXT	SEGMENT
$T75215 = -32
__$EHRec$ = -12
_dFactor$ = 8
_this$ = -36
_proc$ = -28
?ResizeImage@CEnBitmapEx@@QAEHN@Z PROC NEAR		; CEnBitmapEx::ResizeImage, COMDAT

; 72   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75219
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 73   : 	CImageResizer proc(dFactor);

	mov	eax, DWORD PTR _dFactor$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _dFactor$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??0CImageResizer@@QAE@N@Z		; CImageResizer::CImageResizer
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 74   : 	return ProcessImage(&proc);

	push	-1
	lea	edx, DWORD PTR _proc$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T75215[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageResizer@@UAE@XZ		; CImageResizer::~CImageResizer
	mov	eax, DWORD PTR $T75215[ebp]

; 75   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75216:
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageResizer@@UAE@XZ		; CImageResizer::~CImageResizer
	ret	0
$L75219:
	mov	eax, OFFSET FLAT:$T75218
	jmp	___CxxFrameHandler
text$x	ENDS
?ResizeImage@CEnBitmapEx@@QAEHN@Z ENDP			; CEnBitmapEx::ResizeImage
PUBLIC	?LightenImage@CEnBitmapEx@@QAEHNK@Z		; CEnBitmapEx::LightenImage
EXTRN	??0CImageLightener@@QAE@N@Z:NEAR		; CImageLightener::CImageLightener
EXTRN	??1CImageLightener@@UAE@XZ:NEAR			; CImageLightener::~CImageLightener
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75229	DD	019930520H
	DD	01H
	DD	FLAT:$T75231
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75231	DD	0ffffffffH
	DD	FLAT:$L75227
xdata$x	ENDS
;	COMDAT ?LightenImage@CEnBitmapEx@@QAEHNK@Z
_TEXT	SEGMENT
$T75226 = -32
__$EHRec$ = -12
_dAmount$ = 8
_crMask$ = 16
_this$ = -36
_proc$ = -28
?LightenImage@CEnBitmapEx@@QAEHNK@Z PROC NEAR		; CEnBitmapEx::LightenImage, COMDAT

; 78   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75230
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 79   : 	CImageLightener proc(dAmount);

	mov	eax, DWORD PTR _dAmount$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _dAmount$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??0CImageLightener@@QAE@N@Z		; CImageLightener::CImageLightener
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 80   : 	return ProcessImage(&proc, crMask);

	mov	edx, DWORD PTR _crMask$[ebp]
	push	edx
	lea	eax, DWORD PTR _proc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T75226[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageLightener@@UAE@XZ		; CImageLightener::~CImageLightener
	mov	eax, DWORD PTR $T75226[ebp]

; 81   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75227:
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageLightener@@UAE@XZ		; CImageLightener::~CImageLightener
	ret	0
$L75230:
	mov	eax, OFFSET FLAT:$T75229
	jmp	___CxxFrameHandler
text$x	ENDS
?LightenImage@CEnBitmapEx@@QAEHNK@Z ENDP		; CEnBitmapEx::LightenImage
PUBLIC	?FlipImage@CEnBitmapEx@@QAEHHH@Z		; CEnBitmapEx::FlipImage
EXTRN	??0CImageFlipper@@QAE@HH@Z:NEAR			; CImageFlipper::CImageFlipper
EXTRN	??1CImageFlipper@@UAE@XZ:NEAR			; CImageFlipper::~CImageFlipper
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75239	DD	019930520H
	DD	01H
	DD	FLAT:$T75241
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75241	DD	0ffffffffH
	DD	FLAT:$L75237
xdata$x	ENDS
;	COMDAT ?FlipImage@CEnBitmapEx@@QAEHHH@Z
_TEXT	SEGMENT
$T75236 = -32
__$EHRec$ = -12
_bHorz$ = 8
_bVert$ = 12
_this$ = -36
_proc$ = -28
?FlipImage@CEnBitmapEx@@QAEHHH@Z PROC NEAR		; CEnBitmapEx::FlipImage, COMDAT

; 84   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75240
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 85   : 	CImageFlipper proc(bHorz, bVert);

	mov	eax, DWORD PTR _bVert$[ebp]
	push	eax
	mov	ecx, DWORD PTR _bHorz$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??0CImageFlipper@@QAE@HH@Z		; CImageFlipper::CImageFlipper
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 86   : 	return ProcessImage(&proc);

	push	-1
	lea	edx, DWORD PTR _proc$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T75236[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageFlipper@@UAE@XZ		; CImageFlipper::~CImageFlipper
	mov	eax, DWORD PTR $T75236[ebp]

; 87   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75237:
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageFlipper@@UAE@XZ		; CImageFlipper::~CImageFlipper
	ret	0
$L75240:
	mov	eax, OFFSET FLAT:$T75239
	jmp	___CxxFrameHandler
text$x	ENDS
?FlipImage@CEnBitmapEx@@QAEHHH@Z ENDP			; CEnBitmapEx::FlipImage
PUBLIC	?NegateImage@CEnBitmapEx@@QAEHXZ		; CEnBitmapEx::NegateImage
EXTRN	??0CImageNegator@@QAE@XZ:NEAR			; CImageNegator::CImageNegator
EXTRN	??1CImageNegator@@UAE@XZ:NEAR			; CImageNegator::~CImageNegator
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75249	DD	019930520H
	DD	01H
	DD	FLAT:$T75251
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75251	DD	0ffffffffH
	DD	FLAT:$L75247
xdata$x	ENDS
;	COMDAT ?NegateImage@CEnBitmapEx@@QAEHXZ
_TEXT	SEGMENT
$T75246 = -24
__$EHRec$ = -12
_this$ = -28
_proc$ = -20
?NegateImage@CEnBitmapEx@@QAEHXZ PROC NEAR		; CEnBitmapEx::NegateImage, COMDAT

; 90   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75250
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

; 91   : 	CImageNegator proc;

	lea	ecx, DWORD PTR _proc$[ebp]
	call	??0CImageNegator@@QAE@XZ		; CImageNegator::CImageNegator
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 92   : 	return ProcessImage(&proc);

	push	-1
	lea	eax, DWORD PTR _proc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T75246[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageNegator@@UAE@XZ		; CImageNegator::~CImageNegator
	mov	eax, DWORD PTR $T75246[ebp]

; 93   : }

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
$L75247:
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageNegator@@UAE@XZ		; CImageNegator::~CImageNegator
	ret	0
$L75250:
	mov	eax, OFFSET FLAT:$T75249
	jmp	___CxxFrameHandler
text$x	ENDS
?NegateImage@CEnBitmapEx@@QAEHXZ ENDP			; CEnBitmapEx::NegateImage
PUBLIC	?ReplaceColor@CEnBitmapEx@@QAEHKK@Z		; CEnBitmapEx::ReplaceColor
EXTRN	??0CColorReplacer@@QAE@KK@Z:NEAR		; CColorReplacer::CColorReplacer
EXTRN	??1CColorReplacer@@UAE@XZ:NEAR			; CColorReplacer::~CColorReplacer
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75259	DD	019930520H
	DD	01H
	DD	FLAT:$T75261
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75261	DD	0ffffffffH
	DD	FLAT:$L75257
xdata$x	ENDS
;	COMDAT ?ReplaceColor@CEnBitmapEx@@QAEHKK@Z
_TEXT	SEGMENT
$T75256 = -32
__$EHRec$ = -12
_crFrom$ = 8
_crTo$ = 12
_this$ = -36
_proc$ = -28
?ReplaceColor@CEnBitmapEx@@QAEHKK@Z PROC NEAR		; CEnBitmapEx::ReplaceColor, COMDAT

; 96   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75260
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 97   : 	CColorReplacer proc(crFrom, crTo);

	mov	eax, DWORD PTR _crTo$[ebp]
	push	eax
	mov	ecx, DWORD PTR _crFrom$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??0CColorReplacer@@QAE@KK@Z		; CColorReplacer::CColorReplacer
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 98   : 	return ProcessImage(&proc);

	push	-1
	lea	edx, DWORD PTR _proc$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T75256[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CColorReplacer@@UAE@XZ		; CColorReplacer::~CColorReplacer
	mov	eax, DWORD PTR $T75256[ebp]

; 99   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75257:
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CColorReplacer@@UAE@XZ		; CColorReplacer::~CColorReplacer
	ret	0
$L75260:
	mov	eax, OFFSET FLAT:$T75259
	jmp	___CxxFrameHandler
text$x	ENDS
?ReplaceColor@CEnBitmapEx@@QAEHKK@Z ENDP		; CEnBitmapEx::ReplaceColor
PUBLIC	?ColorizeImage@CEnBitmapEx@@QAEHK@Z		; CEnBitmapEx::ColorizeImage
EXTRN	??0CImageColorizer@@QAE@K@Z:NEAR		; CImageColorizer::CImageColorizer
EXTRN	??1CImageColorizer@@UAE@XZ:NEAR			; CImageColorizer::~CImageColorizer
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75269	DD	019930520H
	DD	01H
	DD	FLAT:$T75271
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75271	DD	0ffffffffH
	DD	FLAT:$L75267
xdata$x	ENDS
;	COMDAT ?ColorizeImage@CEnBitmapEx@@QAEHK@Z
_TEXT	SEGMENT
$T75266 = -64
__$EHRec$ = -12
_color$ = 8
_this$ = -68
_proc$ = -60
?ColorizeImage@CEnBitmapEx@@QAEHK@Z PROC NEAR		; CEnBitmapEx::ColorizeImage, COMDAT

; 102  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75270
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 56					; 00000038H
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-68]
	mov	ecx, 14					; 0000000eH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 103  : 	CImageColorizer proc(color);

	mov	eax, DWORD PTR _color$[ebp]
	push	eax
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??0CImageColorizer@@QAE@K@Z		; CImageColorizer::CImageColorizer
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 104  : 	return ProcessImage(&proc);

	push	-1
	lea	ecx, DWORD PTR _proc$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T75266[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageColorizer@@UAE@XZ		; CImageColorizer::~CImageColorizer
	mov	eax, DWORD PTR $T75266[ebp]

; 105  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 68					; 00000044H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75267:
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageColorizer@@UAE@XZ		; CImageColorizer::~CImageColorizer
	ret	0
$L75270:
	mov	eax, OFFSET FLAT:$T75269
	jmp	___CxxFrameHandler
text$x	ENDS
?ColorizeImage@CEnBitmapEx@@QAEHK@Z ENDP		; CEnBitmapEx::ColorizeImage
PUBLIC	?ContrastImage@CEnBitmapEx@@QAEHH@Z		; CEnBitmapEx::ContrastImage
EXTRN	??0CImageContraster@@QAE@H@Z:NEAR		; CImageContraster::CImageContraster
EXTRN	??1CImageContraster@@UAE@XZ:NEAR		; CImageContraster::~CImageContraster
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75279	DD	019930520H
	DD	01H
	DD	FLAT:$T75281
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75281	DD	0ffffffffH
	DD	FLAT:$L75277
xdata$x	ENDS
;	COMDAT ?ContrastImage@CEnBitmapEx@@QAEHH@Z
_TEXT	SEGMENT
$T75276 = -28
__$EHRec$ = -12
_nAmount$ = 8
_this$ = -32
_proc$ = -24
?ContrastImage@CEnBitmapEx@@QAEHH@Z PROC NEAR		; CEnBitmapEx::ContrastImage, COMDAT

; 108  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75280
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

; 109  : 	CImageContraster proc(nAmount);

	mov	eax, DWORD PTR _nAmount$[ebp]
	push	eax
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??0CImageContraster@@QAE@H@Z		; CImageContraster::CImageContraster
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 110  : 	return ProcessImage(&proc);

	push	-1
	lea	ecx, DWORD PTR _proc$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T75276[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageContraster@@UAE@XZ		; CImageContraster::~CImageContraster
	mov	eax, DWORD PTR $T75276[ebp]

; 111  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75277:
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageContraster@@UAE@XZ		; CImageContraster::~CImageContraster
	ret	0
$L75280:
	mov	eax, OFFSET FLAT:$T75279
	jmp	___CxxFrameHandler
text$x	ENDS
?ContrastImage@CEnBitmapEx@@QAEHH@Z ENDP		; CEnBitmapEx::ContrastImage
PUBLIC	?TintImage@CEnBitmapEx@@QAEHKHK@Z		; CEnBitmapEx::TintImage
EXTRN	??0CImageTinter@@QAE@KH@Z:NEAR			; CImageTinter::CImageTinter
EXTRN	??1CImageTinter@@UAE@XZ:NEAR			; CImageTinter::~CImageTinter
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75289	DD	019930520H
	DD	01H
	DD	FLAT:$T75291
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75291	DD	0ffffffffH
	DD	FLAT:$L75287
xdata$x	ENDS
;	COMDAT ?TintImage@CEnBitmapEx@@QAEHKHK@Z
_TEXT	SEGMENT
$T75286 = -32
__$EHRec$ = -12
_color$ = 8
_nAmount$ = 12
_crMask$ = 16
_this$ = -36
_proc$ = -28
?TintImage@CEnBitmapEx@@QAEHKHK@Z PROC NEAR		; CEnBitmapEx::TintImage, COMDAT

; 114  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75290
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 115  : 	CImageTinter proc(color, nAmount);

	mov	eax, DWORD PTR _nAmount$[ebp]
	push	eax
	mov	ecx, DWORD PTR _color$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??0CImageTinter@@QAE@KH@Z		; CImageTinter::CImageTinter
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 116  : 	return ProcessImage(&proc, crMask);

	mov	edx, DWORD PTR _crMask$[ebp]
	push	edx
	lea	eax, DWORD PTR _proc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T75286[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageTinter@@UAE@XZ			; CImageTinter::~CImageTinter
	mov	eax, DWORD PTR $T75286[ebp]

; 117  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75287:
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageTinter@@UAE@XZ			; CImageTinter::~CImageTinter
	ret	0
$L75290:
	mov	eax, OFFSET FLAT:$T75289
	jmp	___CxxFrameHandler
text$x	ENDS
?TintImage@CEnBitmapEx@@QAEHKHK@Z ENDP			; CEnBitmapEx::TintImage
PUBLIC	??0CImageSysColorMapper@@QAE@XZ			; CImageSysColorMapper::CImageSysColorMapper
PUBLIC	??1CImageSysColorMapper@@UAE@XZ			; CImageSysColorMapper::~CImageSysColorMapper
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75299	DD	019930520H
	DD	01H
	DD	FLAT:$T75301
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75301	DD	0ffffffffH
	DD	FLAT:$L75297
xdata$x	ENDS
;	COMDAT ?RemapSysColors@CEnBitmapEx@@QAEHXZ
_TEXT	SEGMENT
$T75296 = -24
__$EHRec$ = -12
_this$ = -28
_proc$ = -20
?RemapSysColors@CEnBitmapEx@@QAEHXZ PROC NEAR		; CEnBitmapEx::RemapSysColors, COMDAT

; 120  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75300
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

; 121  : 	CImageSysColorMapper proc;

	lea	ecx, DWORD PTR _proc$[ebp]
	call	??0CImageSysColorMapper@@QAE@XZ		; CImageSysColorMapper::CImageSysColorMapper
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 122  : 	return ProcessImage(&proc);

	push	-1
	lea	eax, DWORD PTR _proc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T75296[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageSysColorMapper@@UAE@XZ		; CImageSysColorMapper::~CImageSysColorMapper
	mov	eax, DWORD PTR $T75296[ebp]

; 123  : }

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
$L75297:
	lea	ecx, DWORD PTR _proc$[ebp]
	call	??1CImageSysColorMapper@@UAE@XZ		; CImageSysColorMapper::~CImageSysColorMapper
	ret	0
$L75300:
	mov	eax, OFFSET FLAT:$T75299
	jmp	___CxxFrameHandler
text$x	ENDS
?RemapSysColors@CEnBitmapEx@@QAEHXZ ENDP		; CEnBitmapEx::RemapSysColors
PUBLIC	??_7CImageSysColorMapper@@6B@			; CImageSysColorMapper::`vftable'
PUBLIC	??_GCImageSysColorMapper@@UAEPAXI@Z		; CImageSysColorMapper::`scalar deleting destructor'
PUBLIC	??_ECImageSysColorMapper@@UAEPAXI@Z		; CImageSysColorMapper::`vector deleting destructor'
EXTRN	??0C32BitImageProcessor@@QAE@H@Z:NEAR		; C32BitImageProcessor::C32BitImageProcessor
EXTRN	?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z:NEAR ; C32BitImageProcessor::CalcDestSize
EXTRN	?ProcessPixels@CImageSysColorMapper@@UAEHPAVRGBX@@VCSize@@01K@Z:NEAR ; CImageSysColorMapper::ProcessPixels
;	COMDAT ??_7CImageSysColorMapper@@6B@
; File D:\_CODE\Shared\imageprocessors.h
CONST	SEGMENT
??_7CImageSysColorMapper@@6B@ DD FLAT:??_ECImageSysColorMapper@@UAEPAXI@Z ; CImageSysColorMapper::`vftable'
	DD	FLAT:?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z
	DD	FLAT:?ProcessPixels@CImageSysColorMapper@@UAEHPAVRGBX@@VCSize@@01K@Z
CONST	ENDS
;	COMDAT ??0CImageSysColorMapper@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0CImageSysColorMapper@@QAE@XZ PROC NEAR		; CImageSysColorMapper::CImageSysColorMapper, COMDAT

; 194  : 	CImageSysColorMapper() {}

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0C32BitImageProcessor@@QAE@H@Z	; C32BitImageProcessor::C32BitImageProcessor
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageSysColorMapper@@6B@ ; CImageSysColorMapper::`vftable'
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CImageSysColorMapper@@QAE@XZ ENDP			; CImageSysColorMapper::CImageSysColorMapper
_TEXT	ENDS
EXTRN	??1C32BitImageProcessor@@UAE@XZ:NEAR		; C32BitImageProcessor::~C32BitImageProcessor
;	COMDAT ??1CImageSysColorMapper@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CImageSysColorMapper@@UAE@XZ PROC NEAR		; CImageSysColorMapper::~CImageSysColorMapper, COMDAT

; 195  : 	virtual ~CImageSysColorMapper() {}

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageSysColorMapper@@6B@ ; CImageSysColorMapper::`vftable'
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CImageSysColorMapper@@UAE@XZ ENDP			; CImageSysColorMapper::~CImageSysColorMapper
_TEXT	ENDS
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCImageSysColorMapper@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCImageSysColorMapper@@UAEPAXI@Z PROC NEAR		; CImageSysColorMapper::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CImageSysColorMapper@@UAE@XZ		; CImageSysColorMapper::~CImageSysColorMapper
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L74537
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L74537:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCImageSysColorMapper@@UAEPAXI@Z ENDP		; CImageSysColorMapper::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	?GrayScale@CEnBitmapEx@@QAEHK@Z			; CEnBitmapEx::GrayScale
PUBLIC	??0?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAE@XZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::CArray<C32BitImageProcessor *,C32BitImageProcessor *>
PUBLIC	?Add@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEHPAVC32BitImageProcessor@@@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::Add
PUBLIC	??1?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAE@XZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::~CArray<C32BitImageProcessor *,C32BitImageProcessor *>
EXTRN	?ProcessImage@CEnBitmap@@QAEHAAV?$CArray@PAVC32BitImageProcessor@@PAV1@@@K@Z:NEAR ; CEnBitmap::ProcessImage
EXTRN	??0CImageGrayer@@QAE@NNN@Z:NEAR			; CImageGrayer::CImageGrayer
EXTRN	__imp__GetSysColor@4:NEAR
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75321	DD	019930520H
	DD	05H
	DD	FLAT:$T75323
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75323	DD	0ffffffffH
	DD	FLAT:$L75315
	DD	00H
	DD	FLAT:$L75316
	DD	01H
	DD	FLAT:$L75317
	DD	02H
	DD	FLAT:$L75318
	DD	03H
	DD	FLAT:$L75319
xdata$x	ENDS
;	COMDAT ?GrayScale@CEnBitmapEx@@QAEHK@Z
_TEXT	SEGMENT
$T75314 = -120
__$EHRec$ = -12
_crMask$ = 8
_this$ = -124
_aProcessors$ = -32
_ip2$ = -72
_ip3$ = -88
_ip4$ = -104
_ip5$ = -116
?GrayScale@CEnBitmapEx@@QAEHK@Z PROC NEAR		; CEnBitmapEx::GrayScale, COMDAT

; 126  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75322
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 112				; 00000070H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-124]
	mov	ecx, 28					; 0000001cH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 127  : 	// create 'nice' disabled image
; 128  : 	C32BIPArray aProcessors;

	lea	ecx, DWORD PTR _aProcessors$[ebp]
	call	??0?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAE@XZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::CArray<C32BitImageProcessor *,C32BitImageProcessor *>
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 129  : 	CImageGrayer ip2(0.33, 0.33, 0.33);

	push	1070931640				; 3fd51eb8H
	push	1374389535				; 51eb851fH
	push	1070931640				; 3fd51eb8H
	push	1374389535				; 51eb851fH
	push	1070931640				; 3fd51eb8H
	push	1374389535				; 51eb851fH
	lea	ecx, DWORD PTR _ip2$[ebp]
	call	??0CImageGrayer@@QAE@NNN@Z		; CImageGrayer::CImageGrayer
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 130  : 	CImageLightener ip3(0.2);

	push	1070176665				; 3fc99999H
	push	-1717986918				; 9999999aH
	lea	ecx, DWORD PTR _ip3$[ebp]
	call	??0CImageLightener@@QAE@N@Z		; CImageLightener::CImageLightener
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 131  : 	CImageTinter ip4(GetSysColor(COLOR_3DSHADOW), 10);

	push	10					; 0000000aH
	mov	esi, esp
	push	16					; 00000010H
	call	DWORD PTR __imp__GetSysColor@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	lea	ecx, DWORD PTR _ip4$[ebp]
	call	??0CImageTinter@@QAE@KH@Z		; CImageTinter::CImageTinter
	mov	BYTE PTR __$EHRec$[ebp+8], 3

; 132  : 	CImageContraster ip5(-30);

	push	-30					; ffffffe2H
	lea	ecx, DWORD PTR _ip5$[ebp]
	call	??0CImageContraster@@QAE@H@Z		; CImageContraster::CImageContraster
	mov	BYTE PTR __$EHRec$[ebp+8], 4

; 133  : 	
; 134  : 	aProcessors.Add(&ip2);

	lea	eax, DWORD PTR _ip2$[ebp]
	push	eax
	lea	ecx, DWORD PTR _aProcessors$[ebp]
	call	?Add@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEHPAVC32BitImageProcessor@@@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::Add

; 135  : 	aProcessors.Add(&ip3);

	lea	ecx, DWORD PTR _ip3$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _aProcessors$[ebp]
	call	?Add@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEHPAVC32BitImageProcessor@@@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::Add

; 136  : 	aProcessors.Add(&ip4);

	lea	edx, DWORD PTR _ip4$[ebp]
	push	edx
	lea	ecx, DWORD PTR _aProcessors$[ebp]
	call	?Add@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEHPAVC32BitImageProcessor@@@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::Add

; 137  : 	aProcessors.Add(&ip5);

	lea	eax, DWORD PTR _ip5$[ebp]
	push	eax
	lea	ecx, DWORD PTR _aProcessors$[ebp]
	call	?Add@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEHPAVC32BitImageProcessor@@@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::Add

; 138  : 	
; 139  : 	return ProcessImage(aProcessors, crMask);

	mov	ecx, DWORD PTR _crMask$[ebp]
	push	ecx
	lea	edx, DWORD PTR _aProcessors$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHAAV?$CArray@PAVC32BitImageProcessor@@PAV1@@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T75314[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR _ip5$[ebp]
	call	??1CImageContraster@@UAE@XZ		; CImageContraster::~CImageContraster
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _ip4$[ebp]
	call	??1CImageTinter@@UAE@XZ			; CImageTinter::~CImageTinter
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _ip3$[ebp]
	call	??1CImageLightener@@UAE@XZ		; CImageLightener::~CImageLightener
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _ip2$[ebp]
	call	??1CImageGrayer@@UAE@XZ			; CImageGrayer::~CImageGrayer
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _aProcessors$[ebp]
	call	??1?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAE@XZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::~CArray<C32BitImageProcessor *,C32BitImageProcessor *>
	mov	eax, DWORD PTR $T75314[ebp]

; 140  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 124				; 0000007cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75315:
	lea	ecx, DWORD PTR _aProcessors$[ebp]
	call	??1?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAE@XZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::~CArray<C32BitImageProcessor *,C32BitImageProcessor *>
	ret	0
$L75316:
	lea	ecx, DWORD PTR _ip2$[ebp]
	call	??1CImageGrayer@@UAE@XZ			; CImageGrayer::~CImageGrayer
	ret	0
$L75317:
	lea	ecx, DWORD PTR _ip3$[ebp]
	call	??1CImageLightener@@UAE@XZ		; CImageLightener::~CImageLightener
	ret	0
$L75318:
	lea	ecx, DWORD PTR _ip4$[ebp]
	call	??1CImageTinter@@UAE@XZ			; CImageTinter::~CImageTinter
	ret	0
$L75319:
	lea	ecx, DWORD PTR _ip5$[ebp]
	call	??1CImageContraster@@UAE@XZ		; CImageContraster::~CImageContraster
	ret	0
$L75322:
	mov	eax, OFFSET FLAT:$T75321
	jmp	___CxxFrameHandler
text$x	ENDS
?GrayScale@CEnBitmapEx@@QAEHK@Z ENDP			; CEnBitmapEx::GrayScale
PUBLIC	?GrayScale@CEnBitmapEx@@SAHAAVCBitmap@@K@Z	; CEnBitmapEx::GrayScale
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	?GetSafeHandle@CGdiObject@@QBEPAXXZ:NEAR	; CGdiObject::GetSafeHandle
EXTRN	?Attach@CGdiObject@@QAEHPAX@Z:NEAR		; CGdiObject::Attach
EXTRN	?Detach@CGdiObject@@QAEPAXXZ:NEAR		; CGdiObject::Detach
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmapEx.cpp
xdata$x	SEGMENT
$T75331	DD	019930520H
	DD	01H
	DD	FLAT:$T75333
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75333	DD	0ffffffffH
	DD	FLAT:$L75329
xdata$x	ENDS
;	COMDAT ?GrayScale@CEnBitmapEx@@SAHAAVCBitmap@@K@Z
_TEXT	SEGMENT
_bmpEnhanced$ = -32
$T75328 = -36
__$EHRec$ = -12
_bitmap$ = 8
_crMask$ = 12
?GrayScale@CEnBitmapEx@@SAHAAVCBitmap@@K@Z PROC NEAR	; CEnBitmapEx::GrayScale, COMDAT

; 143  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75332
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 24					; 00000018H
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
$L74750:

; 144  : 	ASSERT(bitmap.GetSafeHandle());

	mov	ecx, DWORD PTR _bitmap$[ebp]
	call	?GetSafeHandle@CGdiObject@@QBEPAXXZ	; CGdiObject::GetSafeHandle
	test	eax, eax
	jne	SHORT $L74753
	push	144					; 00000090H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74753
	int	3
$L74753:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74750

; 145  : 
; 146  : 	if (!bitmap.GetSafeHandle())

	mov	ecx, DWORD PTR _bitmap$[ebp]
	call	?GetSafeHandle@CGdiObject@@QBEPAXXZ	; CGdiObject::GetSafeHandle
	test	eax, eax
	jne	SHORT $L74754

; 147  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L74749
$L74754:

; 148  : 
; 149  : 	CEnBitmapEx bmpEnhanced(crMask);

	mov	ecx, DWORD PTR _crMask$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _bmpEnhanced$[ebp]
	call	??0CEnBitmapEx@@QAE@K@Z			; CEnBitmapEx::CEnBitmapEx
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 150  : 	bmpEnhanced.Attach(bitmap.Detach());

	mov	ecx, DWORD PTR _bitmap$[ebp]
	call	?Detach@CGdiObject@@QAEPAXXZ		; CGdiObject::Detach
	push	eax
	lea	ecx, DWORD PTR _bmpEnhanced$[ebp]
	call	?Attach@CGdiObject@@QAEHPAX@Z		; CGdiObject::Attach

; 151  : 	
; 152  : 	if (bmpEnhanced.GrayScale(crMask))

	mov	edx, DWORD PTR _crMask$[ebp]
	push	edx
	lea	ecx, DWORD PTR _bmpEnhanced$[ebp]
	call	?GrayScale@CEnBitmapEx@@QAEHK@Z		; CEnBitmapEx::GrayScale
	test	eax, eax
	je	SHORT $L74756

; 153  : 		bitmap.Attach(bmpEnhanced.Detach());

	lea	ecx, DWORD PTR _bmpEnhanced$[ebp]
	call	?Detach@CGdiObject@@QAEPAXXZ		; CGdiObject::Detach
	push	eax
	mov	ecx, DWORD PTR _bitmap$[ebp]
	call	?Attach@CGdiObject@@QAEHPAX@Z		; CGdiObject::Attach
$L74756:

; 154  : 
; 155  : 	// else
; 156  : 	return FALSE;

	mov	DWORD PTR $T75328[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _bmpEnhanced$[ebp]
	call	??1CEnBitmapEx@@UAE@XZ			; CEnBitmapEx::~CEnBitmapEx
	mov	eax, DWORD PTR $T75328[ebp]
$L74749:

; 157  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75329:
	lea	ecx, DWORD PTR _bmpEnhanced$[ebp]
	call	??1CEnBitmapEx@@UAE@XZ			; CEnBitmapEx::~CEnBitmapEx
	ret	0
$L75332:
	mov	eax, OFFSET FLAT:$T75331
	jmp	___CxxFrameHandler
text$x	ENDS
?GrayScale@CEnBitmapEx@@SAHAAVCBitmap@@K@Z ENDP		; CEnBitmapEx::GrayScale
PUBLIC	?Serialize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAEXAAVCArchive@@@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::Serialize
PUBLIC	?Dump@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UBEXAAVCDumpContext@@@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::Dump
PUBLIC	?AssertValid@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UBEXXZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::AssertValid
PUBLIC	??_7?$CArray@PAVC32BitImageProcessor@@PAV1@@@6B@ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::`vftable'
PUBLIC	??_G?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAEPAXI@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::`scalar deleting destructor'
PUBLIC	??_E?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAEPAXI@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::`vector deleting destructor'
EXTRN	?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ:NEAR ; CObject::GetRuntimeClass
EXTRN	??0CObject@@IAE@XZ:NEAR				; CObject::CObject
;	COMDAT ??_7?$CArray@PAVC32BitImageProcessor@@PAV1@@@6B@
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
CONST	SEGMENT
??_7?$CArray@PAVC32BitImageProcessor@@PAV1@@@6B@ DD FLAT:?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::`vftable'
	DD	FLAT:??_E?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAEPAXI@Z
	DD	FLAT:?Serialize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UBEXXZ
	DD	FLAT:?Dump@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UBEXAAVCDumpContext@@@Z
CONST	ENDS
;	COMDAT ??0?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAE@XZ PROC NEAR ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::CArray<C32BitImageProcessor *,C32BitImageProcessor *>, COMDAT

; 287  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CObject@@IAE@XZ			; CObject::CObject
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CArray@PAVC32BitImageProcessor@@PAV1@@@6B@ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::`vftable'

; 288  : 	m_pData = NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+4], 0

; 289  : 	m_nSize = m_nMaxSize = m_nGrowBy = 0;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+16], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+12], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+8], 0

; 290  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAE@XZ ENDP	; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::CArray<C32BitImageProcessor *,C32BitImageProcessor *>
_TEXT	ENDS
PUBLIC	?SetAtGrow@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHPAVC32BitImageProcessor@@@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::SetAtGrow
;	COMDAT ?Add@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEHPAVC32BitImageProcessor@@@Z
_TEXT	SEGMENT
_newElement$ = 8
_this$ = -8
_nIndex$ = -4
?Add@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEHPAVC32BitImageProcessor@@@Z PROC NEAR ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::Add, COMDAT

; 272  : 	{ int nIndex = m_nSize;

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR _nIndex$[ebp], ecx

; 273  : 		SetAtGrow(nIndex, newElement);

	mov	edx, DWORD PTR _newElement$[ebp]
	push	edx
	mov	eax, DWORD PTR _nIndex$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetAtGrow@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHPAVC32BitImageProcessor@@@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::SetAtGrow

; 274  : 		return nIndex; }

	mov	eax, DWORD PTR _nIndex$[ebp]
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Add@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEHPAVC32BitImageProcessor@@@Z ENDP ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::Add
_TEXT	ENDS
PUBLIC	?DestructElements@@YGXPAPAVC32BitImageProcessor@@H@Z ; DestructElements
EXTRN	??1CObject@@UAE@XZ:NEAR				; CObject::~CObject
EXTRN	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z:NEAR ; AfxAssertValidObject
;	COMDAT xdata$x
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
xdata$x	SEGMENT
$T75346	DD	019930520H
	DD	01H
	DD	FLAT:$T75348
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75348	DD	0ffffffffH
	DD	FLAT:$L75344
xdata$x	ENDS
;	COMDAT ??1?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAE@XZ
_TEXT	SEGMENT
_this$ = -20
$T75343 = -16
__$EHRec$ = -12
??1?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAE@XZ PROC NEAR ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::~CArray<C32BitImageProcessor *,C32BitImageProcessor *>, COMDAT

; 294  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75347
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CArray@PAVC32BitImageProcessor@@PAV1@@@6B@ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 295  : 	ASSERT_VALID(this);

	push	295					; 00000127H
	push	OFFSET FLAT:__szAfxTempl
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 296  : 
; 297  : 	if (m_pData != NULL)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	je	SHORT $L74853

; 299  : 		DestructElements<TYPE>(m_pData, m_nSize);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	call	?DestructElements@@YGXPAPAVC32BitImageProcessor@@H@Z ; DestructElements

; 300  : 		delete[] (BYTE*)m_pData;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR $T75343[ebp], edx
	mov	eax, DWORD PTR $T75343[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L74853:

; 302  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
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
$L75344:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L75347:
	mov	eax, OFFSET FLAT:$T75346
	jmp	___CxxFrameHandler
text$x	ENDS
??1?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAE@XZ ENDP	; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::~CArray<C32BitImageProcessor *,C32BitImageProcessor *>
PUBLIC	?SerializeElements@@YGXAAVCArchive@@PAPAVC32BitImageProcessor@@H@Z ; SerializeElements
PUBLIC	?SetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHH@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::SetSize
EXTRN	?ReadCount@CArchive@@QAEKXZ:NEAR		; CArchive::ReadCount
EXTRN	?WriteCount@CArchive@@QAEXK@Z:NEAR		; CArchive::WriteCount
EXTRN	?IsStoring@CArchive@@QBEHXZ:NEAR		; CArchive::IsStoring
;	COMDAT ?Serialize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT
_ar$ = 8
_this$ = -8
_nOldSize$74867 = -4
?Serialize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAEXAAVCArchive@@@Z PROC NEAR ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::Serialize, COMDAT

; 514  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 515  : 	ASSERT_VALID(this);

	push	515					; 00000203H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 516  : 
; 517  : 	CObject::Serialize(ar);

	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Serialize@CObject@@UAEXAAVCArchive@@@Z	; CObject::Serialize

; 518  : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L74865

; 520  : 		ar.WriteCount(m_nSize);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?WriteCount@CArchive@@QAEXK@Z		; CArchive::WriteCount

; 522  : 	else

	jmp	SHORT $L74866
$L74865:

; 524  : 		DWORD nOldSize = ar.ReadCount();

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?ReadCount@CArchive@@QAEKXZ		; CArchive::ReadCount
	mov	DWORD PTR _nOldSize$74867[ebp], eax

; 525  : 		SetSize(nOldSize, -1);

	push	-1
	mov	ecx, DWORD PTR _nOldSize$74867[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHH@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::SetSize
$L74866:

; 527  : 	SerializeElements<TYPE>(ar, m_pData, m_nSize);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	mov	eax, DWORD PTR _ar$[ebp]
	push	eax
	call	?SerializeElements@@YGXAAVCArchive@@PAPAVC32BitImageProcessor@@H@Z ; SerializeElements

; 528  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Serialize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAEXAAVCArchive@@@Z ENDP ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::Serialize
_TEXT	ENDS
PUBLIC	??_C@_05KIFI@with?5?$AA@			; `string'
PUBLIC	??_C@_09LOPF@?5elements?$AA@			; `string'
PUBLIC	??_C@_01BJG@?6?$AA@				; `string'
PUBLIC	?DumpElements@@YGXAAVCDumpContext@@PBQAVC32BitImageProcessor@@H@Z ; DumpElements
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
;	COMDAT ?Dump@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT
_dc$ = 8
_this$ = -4
?Dump@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UBEXAAVCDumpContext@@@Z PROC NEAR ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::Dump, COMDAT

; 533  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 534  : 	CObject::Dump(dc);

	mov	eax, DWORD PTR _dc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Dump@CObject@@UBEXAAVCDumpContext@@@Z	; CObject::Dump

; 535  : 
; 536  : 	dc << "with " << m_nSize << " elements";

	push	OFFSET FLAT:??_C@_09LOPF@?5elements?$AA@ ; `string'
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	push	OFFSET FLAT:??_C@_05KIFI@with?5?$AA@	; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<
	mov	ecx, eax
	call	??6CDumpContext@@QAEAAV0@H@Z		; CDumpContext::operator<<
	mov	ecx, eax
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 537  : 	if (dc.GetDepth() > 0)

	mov	ecx, DWORD PTR _dc$[ebp]
	call	?GetDepth@CDumpContext@@QBEHXZ		; CDumpContext::GetDepth
	test	eax, eax
	jle	SHORT $L74880

; 539  : 		dc << "\n";

	push	OFFSET FLAT:??_C@_01BJG@?6?$AA@		; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 540  : 		DumpElements<TYPE>(dc, m_pData, m_nSize);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	push	eax
	mov	ecx, DWORD PTR _dc$[ebp]
	push	ecx
	call	?DumpElements@@YGXAAVCDumpContext@@PBQAVC32BitImageProcessor@@H@Z ; DumpElements
$L74880:

; 542  : 
; 543  : 	dc << "\n";

	push	OFFSET FLAT:??_C@_01BJG@?6?$AA@		; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 544  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Dump@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UBEXAAVCDumpContext@@@Z ENDP ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::Dump
_TEXT	ENDS
EXTRN	?AssertValid@CObject@@UBEXXZ:NEAR		; CObject::AssertValid
EXTRN	?AfxIsValidAddress@@YGHPBXIH@Z:NEAR		; AfxIsValidAddress
;	COMDAT ?AssertValid@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UBEXXZ
_TEXT	SEGMENT
_this$ = -4
?AssertValid@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UBEXXZ PROC NEAR ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::AssertValid, COMDAT

; 548  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 549  : 	CObject::AssertValid();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?AssertValid@CObject@@UBEXXZ		; CObject::AssertValid

; 550  : 
; 551  : 	if (m_pData == NULL)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74891
$L74892:

; 553  : 		ASSERT(m_nSize == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+8], 0
	je	SHORT $L74895
	push	553					; 00000229H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74895
	int	3
$L74895:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74892
$L74894:

; 554  : 		ASSERT(m_nMaxSize == 0);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	je	SHORT $L74899
	push	554					; 0000022aH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74899
	int	3
$L74899:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74894

; 556  : 	else

	jmp	$L74915
$L74891:

; 558  : 		ASSERT(m_nSize >= 0);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+8], 0
	jge	SHORT $L74904
	push	558					; 0000022eH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74904
	int	3
$L74904:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74891
$L74903:

; 559  : 		ASSERT(m_nMaxSize >= 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jge	SHORT $L74908
	push	559					; 0000022fH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74908
	int	3
$L74908:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74903
$L74907:

; 560  : 		ASSERT(m_nSize <= m_nMaxSize);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax+8]
	cmp	edx, DWORD PTR [ecx+12]
	jle	SHORT $L74912
	push	560					; 00000230H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74912
	int	3
$L74912:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74907
$L74911:

; 561  : 		ASSERT(AfxIsValidAddress(m_pData, m_nMaxSize * sizeof(TYPE)));

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	shl	edx, 2
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L74917
	push	561					; 00000231H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74917
	int	3
$L74917:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74911
$L74915:

; 563  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?AssertValid@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UBEXXZ ENDP ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::AssertValid
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_G?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAEPAXI@Z PROC NEAR ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAE@XZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::~CArray<C32BitImageProcessor *,C32BitImageProcessor *>
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L74921
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L74921:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_G?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAEPAXI@Z ENDP ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	?ConstructElements@@YGXPAPAVC32BitImageProcessor@@H@Z ; ConstructElements
EXTRN	_memcpy:NEAR
EXTRN	??2@YAPAXIPBDH@Z:NEAR				; operator new
;	COMDAT ?SetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHH@Z
_TEXT	SEGMENT
_nNewSize$ = 8
_nGrowBy$ = 12
_this$ = -32
_nGrowBy$74970 = -8
_nNewMax$74972 = -12
_pNewData$74984 = -4
$T75361 = -16
$T75362 = -20
$T75367 = -24
$T75368 = -28
?SetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHH@Z PROC NEAR ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::SetSize, COMDAT

; 306  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 40					; 00000028H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-40]
	mov	ecx, 10					; 0000000aH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 307  : 	ASSERT_VALID(this);

	push	307					; 00000133H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject
$L74927:

; 308  : 	ASSERT(nNewSize >= 0);

	cmp	DWORD PTR _nNewSize$[ebp], 0
	jge	SHORT $L74930
	push	308					; 00000134H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74930
	int	3
$L74930:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74927

; 309  : 
; 310  : 	if (nGrowBy != -1)

	cmp	DWORD PTR _nGrowBy$[ebp], -1
	je	SHORT $L74931

; 311  : 		m_nGrowBy = nGrowBy;  // set new size

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nGrowBy$[ebp]
	mov	DWORD PTR [edx+16], eax
$L74931:

; 312  : 
; 313  : 	if (nNewSize == 0)

	cmp	DWORD PTR _nNewSize$[ebp], 0
	jne	SHORT $L74932

; 315  : 		// shrink to nothing
; 316  : 		if (m_pData != NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L74933

; 318  : 			DestructElements<TYPE>(m_pData, m_nSize);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	?DestructElements@@YGXPAPAVC32BitImageProcessor@@H@Z ; DestructElements

; 319  : 			delete[] (BYTE*)m_pData;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR $T75361[ebp], ecx
	mov	edx, DWORD PTR $T75361[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 320  : 			m_pData = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+4], 0
$L74933:

; 322  : 		m_nSize = m_nMaxSize = 0;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+12], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+8], 0

; 324  : 	else if (m_pData == NULL)

	jmp	$L74969
$L74932:
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74941
$L74942:

; 326  : 		// create one with exact size
; 327  : #ifdef SIZE_T_MAX
; 328  : 		ASSERT(nNewSize <= SIZE_T_MAX/sizeof(TYPE));    // no overflow

	cmp	DWORD PTR _nNewSize$[ebp], 1073741823	; 3fffffffH
	jbe	SHORT $L74946
	push	328					; 00000148H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74946
	int	3
$L74946:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74942

; 329  : #endif
; 330  : 		m_pData = (TYPE*) new BYTE[nNewSize * sizeof(TYPE)];

	push	330					; 0000014aH
	push	OFFSET FLAT:__szAfxTempl
	mov	edx, DWORD PTR _nNewSize$[ebp]
	shl	edx, 2
	push	edx
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T75362[ebp], eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR $T75362[ebp]
	mov	DWORD PTR [eax+4], ecx

; 331  : 		ConstructElements<TYPE>(m_pData, nNewSize);

	mov	edx, DWORD PTR _nNewSize$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	?ConstructElements@@YGXPAPAVC32BitImageProcessor@@H@Z ; ConstructElements

; 332  : 		m_nSize = m_nMaxSize = nNewSize;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nNewSize$[ebp]
	mov	DWORD PTR [edx+12], eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nNewSize$[ebp]
	mov	DWORD PTR [ecx+8], edx

; 334  : 	else if (nNewSize <= m_nMaxSize)

	jmp	$L74969
$L74941:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nNewSize$[ebp]
	cmp	ecx, DWORD PTR [eax+12]
	jg	SHORT $L74957

; 336  : 		// it fits
; 337  : 		if (nNewSize > m_nSize)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nNewSize$[ebp]
	cmp	eax, DWORD PTR [edx+8]
	jle	SHORT $L74958

; 339  : 			// initialize the new elements
; 340  : 			ConstructElements<TYPE>(&m_pData[m_nSize], nNewSize-m_nSize);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nNewSize$[ebp]
	sub	edx, DWORD PTR [ecx+8]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	lea	ecx, DWORD PTR [eax+ecx*4]
	push	ecx
	call	?ConstructElements@@YGXPAPAVC32BitImageProcessor@@H@Z ; ConstructElements

; 342  : 		else if (m_nSize > nNewSize)

	jmp	SHORT $L74964
$L74958:
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	cmp	eax, DWORD PTR _nNewSize$[ebp]
	jle	SHORT $L74964

; 344  : 			// destroy the old elements
; 345  : 			DestructElements<TYPE>(&m_pData[nNewSize], m_nSize-nNewSize);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	sub	edx, DWORD PTR _nNewSize$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _nNewSize$[ebp]
	lea	eax, DWORD PTR [ecx+edx*4]
	push	eax
	call	?DestructElements@@YGXPAPAVC32BitImageProcessor@@H@Z ; DestructElements
$L74964:

; 347  : 		m_nSize = nNewSize;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nNewSize$[ebp]
	mov	DWORD PTR [ecx+8], edx

; 349  : 	else

	jmp	$L74969
$L74957:

; 351  : 		// otherwise, grow array
; 352  : 		int nGrowBy = m_nGrowBy;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR _nGrowBy$74970[ebp], ecx

; 353  : 		if (nGrowBy == 0)

	cmp	DWORD PTR _nGrowBy$74970[ebp], 0
	jne	SHORT $L74971

; 355  : 			// heuristically determine growth when nGrowBy == 0
; 356  : 			//  (this avoids heap fragmentation in many situations)
; 357  : 			nGrowBy = m_nSize / 8;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	cdq
	and	edx, 7
	add	eax, edx
	sar	eax, 3
	mov	DWORD PTR _nGrowBy$74970[ebp], eax

; 358  : 			nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);

	cmp	DWORD PTR _nGrowBy$74970[ebp], 4
	jge	SHORT $L75365
	mov	DWORD PTR -36+[ebp], 4
	jmp	SHORT $L75366
$L75365:
	cmp	DWORD PTR _nGrowBy$74970[ebp], 1024	; 00000400H
	jle	SHORT $L75363
	mov	DWORD PTR -40+[ebp], 1024		; 00000400H
	jmp	SHORT $L75364
$L75363:
	mov	eax, DWORD PTR _nGrowBy$74970[ebp]
	mov	DWORD PTR -40+[ebp], eax
$L75364:
	mov	ecx, DWORD PTR -40+[ebp]
	mov	DWORD PTR -36+[ebp], ecx
$L75366:
	mov	edx, DWORD PTR -36+[ebp]
	mov	DWORD PTR _nGrowBy$74970[ebp], edx
$L74971:

; 361  : 		if (nNewSize < m_nMaxSize + nGrowBy)

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	add	ecx, DWORD PTR _nGrowBy$74970[ebp]
	cmp	DWORD PTR _nNewSize$[ebp], ecx
	jge	SHORT $L74973

; 362  : 			nNewMax = m_nMaxSize + nGrowBy;  // granularity

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	add	eax, DWORD PTR _nGrowBy$74970[ebp]
	mov	DWORD PTR _nNewMax$74972[ebp], eax

; 363  : 		else

	jmp	SHORT $L74974
$L74973:

; 364  : 			nNewMax = nNewSize;  // no slush

	mov	ecx, DWORD PTR _nNewSize$[ebp]
	mov	DWORD PTR _nNewMax$74972[ebp], ecx
$L74974:

; 365  : 
; 366  : 		ASSERT(nNewMax >= m_nMaxSize);  // no wrap around

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nNewMax$74972[ebp]
	cmp	eax, DWORD PTR [edx+12]
	jge	SHORT $L74978
	push	366					; 0000016eH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74978
	int	3
$L74978:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74974
$L74977:

; 367  : #ifdef SIZE_T_MAX
; 368  : 		ASSERT(nNewMax <= SIZE_T_MAX/sizeof(TYPE)); // no overflow

	cmp	DWORD PTR _nNewMax$74972[ebp], 1073741823 ; 3fffffffH
	jbe	SHORT $L74983
	push	368					; 00000170H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74983
	int	3
$L74983:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74977

; 369  : #endif
; 370  : 		TYPE* pNewData = (TYPE*) new BYTE[nNewMax * sizeof(TYPE)];

	push	370					; 00000172H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _nNewMax$74972[ebp]
	shl	eax, 2
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T75367[ebp], eax
	mov	ecx, DWORD PTR $T75367[ebp]
	mov	DWORD PTR _pNewData$74984[ebp], ecx

; 371  : 
; 372  : 		// copy new data from old
; 373  : 		memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	mov	eax, DWORD PTR _pNewData$74984[ebp]
	push	eax
	call	_memcpy
	add	esp, 12					; 0000000cH
$L74990:

; 374  : 
; 375  : 		// construct remaining elements
; 376  : 		ASSERT(nNewSize > m_nSize);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nNewSize$[ebp]
	cmp	edx, DWORD PTR [ecx+8]
	jg	SHORT $L74993
	push	376					; 00000178H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74993
	int	3
$L74993:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L74990

; 377  : 		ConstructElements<TYPE>(&pNewData[m_nSize], nNewSize-m_nSize);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nNewSize$[ebp]
	sub	edx, DWORD PTR [ecx+8]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	edx, DWORD PTR _pNewData$74984[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	push	eax
	call	?ConstructElements@@YGXPAPAVC32BitImageProcessor@@H@Z ; ConstructElements

; 378  : 
; 379  : 		// get rid of old stuff (note: no destructors called)
; 380  : 		delete[] (BYTE*)m_pData;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR $T75368[ebp], edx
	mov	eax, DWORD PTR $T75368[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 381  : 		m_pData = pNewData;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _pNewData$74984[ebp]
	mov	DWORD PTR [ecx+4], edx

; 382  : 		m_nSize = nNewSize;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nNewSize$[ebp]
	mov	DWORD PTR [eax+8], ecx

; 383  : 		m_nMaxSize = nNewMax;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nNewMax$74972[ebp]
	mov	DWORD PTR [edx+12], eax
$L74969:

; 385  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHH@Z ENDP ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::SetSize
_TEXT	ENDS
;	COMDAT ?SetAtGrow@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHPAVC32BitImageProcessor@@@Z
_TEXT	SEGMENT
_nIndex$ = 8
_newElement$ = 12
_this$ = -4
?SetAtGrow@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHPAVC32BitImageProcessor@@@Z PROC NEAR ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::SetAtGrow, COMDAT

; 437  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 438  : 	ASSERT_VALID(this);

	push	438					; 000001b6H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject
$L75005:

; 439  : 	ASSERT(nIndex >= 0);

	cmp	DWORD PTR _nIndex$[ebp], 0
	jge	SHORT $L75008
	push	439					; 000001b7H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75008
	int	3
$L75008:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75005

; 440  : 
; 441  : 	if (nIndex >= m_nSize)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nIndex$[ebp]
	cmp	eax, DWORD PTR [edx+8]
	jl	SHORT $L75009

; 442  : 		SetSize(nIndex+1, -1);

	push	-1
	mov	ecx, DWORD PTR _nIndex$[ebp]
	add	ecx, 1
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHH@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::SetSize
$L75009:

; 443  : 	m_pData[nIndex] = newElement;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _nIndex$[ebp]
	mov	edx, DWORD PTR _newElement$[ebp]
	mov	DWORD PTR [eax+ecx*4], edx

; 444  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SetAtGrow@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHPAVC32BitImageProcessor@@@Z ENDP ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::SetAtGrow
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAPAVC32BitImageProcessor@@H@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
?DestructElements@@YGXPAPAVC32BitImageProcessor@@H@Z PROC NEAR ; DestructElements, COMDAT

; 67   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L75011:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L75015
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L75015
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75015
	int	3
$L75015:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75011

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L75016
$L75017:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 4
	mov	DWORD PTR _pElements$[ebp], eax
$L75016:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L75018

; 73   : 		pElements->~TYPE();

	jmp	SHORT $L75017
$L75018:

; 74   : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?DestructElements@@YGXPAPAVC32BitImageProcessor@@H@Z ENDP ; DestructElements
_TEXT	ENDS
EXTRN	?Read@CArchive@@QAEIPAXI@Z:NEAR			; CArchive::Read
EXTRN	?Write@CArchive@@QAEXPBXI@Z:NEAR		; CArchive::Write
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAPAVC32BitImageProcessor@@H@Z
_TEXT	SEGMENT
_ar$ = 8
_pElements$ = 12
_nCount$ = 16
?SerializeElements@@YGXAAVCArchive@@PAPAVC32BitImageProcessor@@H@Z PROC NEAR ; SerializeElements, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L75020:

; 92   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L75024
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L75024
	push	93					; 0000005dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75024
	int	3
$L75024:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75020

; 93   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 94   : 
; 95   : 	// default is bit-wise read/write
; 96   : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L75025

; 97   : 		ar.Write((void*)pElements, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Write@CArchive@@QAEXPBXI@Z		; CArchive::Write

; 98   : 	else

	jmp	SHORT $L75028
$L75025:

; 99   : 		ar.Read((void*)pElements, nCount * sizeof(TYPE));

	mov	edx, DWORD PTR _nCount$[ebp]
	shl	edx, 2
	push	edx
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Read@CArchive@@QAEIPAXI@Z		; CArchive::Read
$L75028:

; 100  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	12					; 0000000cH
?SerializeElements@@YGXAAVCArchive@@PAPAVC32BitImageProcessor@@H@Z ENDP ; SerializeElements
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBQAVC32BitImageProcessor@@H@Z
_TEXT	SEGMENT
_pElements$ = 12
_nCount$ = 16
?DumpElements@@YGXAAVCDumpContext@@PBQAVC32BitImageProcessor@@H@Z PROC NEAR ; DumpElements, COMDAT

; 105  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L75032:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L75036
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L75036
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75036
	int	3
$L75036:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75032

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
?DumpElements@@YGXAAVCDumpContext@@PBQAVC32BitImageProcessor@@H@Z ENDP ; DumpElements
_TEXT	ENDS
PUBLIC	??2@YAPAXIPAX@Z					; operator new
EXTRN	_memset:NEAR
;	COMDAT ?ConstructElements@@YGXPAPAVC32BitImageProcessor@@H@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
$T75379 = -4
?ConstructElements@@YGXPAPAVC32BitImageProcessor@@H@Z PROC NEAR ; ConstructElements, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L75038:

; 54   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L75042
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L75042
	push	55					; 00000037H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75042
	int	3
$L75042:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75038

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

	jmp	SHORT $L75045
$L75046:
	mov	edx, DWORD PTR _pElements$[ebp]
	add	edx, 4
	mov	DWORD PTR _pElements$[ebp], edx
$L75045:
	mov	eax, DWORD PTR _nCount$[ebp]
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
	test	eax, eax
	je	SHORT $L75047

; 62   : 		::new((void*)pElements) TYPE;

	mov	edx, DWORD PTR _pElements$[ebp]
	push	edx
	push	4
	call	??2@YAPAXIPAX@Z				; operator new
	add	esp, 8
	mov	DWORD PTR $T75379[ebp], eax
	jmp	SHORT $L75046
$L75047:

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
?ConstructElements@@YGXPAPAVC32BitImageProcessor@@H@Z ENDP ; ConstructElements
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

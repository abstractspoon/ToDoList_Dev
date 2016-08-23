	TITLE	D:\_CODE\Shared\ImageProcessors.cpp
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
text$AFX_COL1	SEGMENT PARA USE32 PUBLIC ''
text$AFX_COL1	ENDS
;	COMDAT ??0HLSX@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0RGBX@@QAE@EEE@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0RGBX@@QAE@ABK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4RGBX@@QAEABV0@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??8RGBX@@QBEHABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??BRGBX@@QBEKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Luminance@RGBX@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MakeGray@RGBX@@QAEXABV1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MakeGray@RGBX@@QAEXABV1@NNN@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MakeGray@RGBX@@QAEXNNN@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HLS2RGB@RGBX@@SAXABUHLSX@@AAV1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RGB2HLS@RGBX@@SAXABV1@AAUHLSX@@@Z
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
;	COMDAT ??0CImageRotator@@QAE@HH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCImageRotator@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CImageRotator@@QAE@N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CImageRotator@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CalcDestSize@CImageRotator@@UAE?AVCSize@@V2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageRotator@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CImageShearer@@QAE@HHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCImageShearer@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CImageShearer@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CalcDestSize@CImageShearer@@UAE?AVCSize@@V2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageShearer@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CImageGrayer@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCImageGrayer@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CImageGrayer@@QAE@NNN@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CImageGrayer@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageGrayer@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CImageLightener@@QAE@N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCImageLightener@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CImageLightener@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageLightener@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CImageBlurrer@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCImageBlurrer@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CImageBlurrer@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageBlurrer@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CImageSharpener@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCImageSharpener@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CImageSharpener@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageSharpener@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageEmbosser@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CImageResizer@@QAE@N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCImageResizer@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CImageResizer@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CalcDestSize@CImageResizer@@UAE?AVCSize@@V2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageResizer@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Enlarge@CImageResizer@@IAEHPAVRGBX@@VCSize@@01@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Shrink@CImageResizer@@IAEHPAVRGBX@@VCSize@@01@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CImageNegator@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCImageNegator@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CImageNegator@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageNegator@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CImageFlipper@@QAE@HH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCImageFlipper@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CImageFlipper@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageFlipper@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CColorReplacer@@QAE@KK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCColorReplacer@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CColorReplacer@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CColorReplacer@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CImageColorizer@@QAE@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCImageColorizer@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CImageColorizer@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageColorizer@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CImageTinter@@QAE@KH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCImageTinter@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CImageTinter@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageTinter@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CImageContraster@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCImageContraster@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CImageContraster@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageContraster@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageSysColorMapper@@UAEHPAVRGBX@@VCSize@@01K@Z
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
;	COMDAT ??_7CImageContraster@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CImageTinter@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CImageColorizer@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CColorReplacer@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CImageFlipper@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CImageNegator@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CImageResizer@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CImageSharpener@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CImageBlurrer@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CImageLightener@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CImageGrayer@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CImageShearer@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CImageRotator@@6B@
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
_THIS_FILE DB	'D:\_CODE\Shared\ImageProcessors.cpp', 00H
_DATA	ENDS
CONST	SEGMENT
_PI	DQ	0400921fb54442d18r		; 3.14159
CONST	ENDS
_DATA	SEGMENT
_COLORMAPPINGS DD 00H
	DD	012H
	DD	0808080H
	DD	010H
	DD	0c0c0c0H
	DD	0fH
	DD	0ffffffH
	DD	014H
_?NUMCOLORMAPS@?1??ProcessPixels@CImageSysColorMapper@@UAEHPAVRGBX@@VCSize@@01K@Z@4HA DD 04H
_DATA	ENDS
PUBLIC	__real@8@4006b400000000000000
PUBLIC	??0CImageRotator@@QAE@HH@Z			; CImageRotator::CImageRotator
PUBLIC	?CalcDestSize@CImageRotator@@UAE?AVCSize@@V2@@Z	; CImageRotator::CalcDestSize
PUBLIC	?ProcessPixels@CImageRotator@@UAEHPAVRGBX@@VCSize@@01K@Z ; CImageRotator::ProcessPixels
PUBLIC	??_7CImageRotator@@6B@				; CImageRotator::`vftable'
PUBLIC	??_GCImageRotator@@UAEPAXI@Z			; CImageRotator::`scalar deleting destructor'
PUBLIC	??_ECImageRotator@@UAEPAXI@Z			; CImageRotator::`vector deleting destructor'
EXTRN	__except_list:DWORD
EXTRN	__chkesp:NEAR
EXTRN	__fltused:NEAR
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0C32BitImageProcessor@@QAE@H@Z:NEAR		; C32BitImageProcessor::C32BitImageProcessor
EXTRN	??1C32BitImageProcessor@@UAE@XZ:NEAR		; C32BitImageProcessor::~C32BitImageProcessor
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
;	COMDAT ??_7CImageRotator@@6B@
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
??_7CImageRotator@@6B@ DD FLAT:??_ECImageRotator@@UAEPAXI@Z ; CImageRotator::`vftable'
	DD	FLAT:?CalcDestSize@CImageRotator@@UAE?AVCSize@@V2@@Z
	DD	FLAT:?ProcessPixels@CImageRotator@@UAEHPAVRGBX@@VCSize@@01K@Z
CONST	ENDS
;	COMDAT __real@8@4006b400000000000000
CONST	SEGMENT
__real@8@4006b400000000000000 DQ 04066800000000000r ; 180
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T75913	DD	019930520H
	DD	01H
	DD	FLAT:$T75920
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75920	DD	0ffffffffH
	DD	FLAT:$L75909
xdata$x	ENDS
;	COMDAT ??0CImageRotator@@QAE@HH@Z
_TEXT	SEGMENT
_nDegrees$ = 8
_bEnableWeighting$ = 12
_this$ = -16
__$EHRec$ = -12
??0CImageRotator@@QAE@HH@Z PROC NEAR			; CImageRotator::CImageRotator, COMDAT

; 25   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75916
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _bEnableWeighting$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0C32BitImageProcessor@@QAE@H@Z	; C32BitImageProcessor::C32BitImageProcessor
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CImageRotator@@6B@ ; CImageRotator::`vftable'
$L74852:

; 26   : 	// normalize the angle
; 27   : 	while (nDegrees >= 360)

	cmp	DWORD PTR _nDegrees$[ebp], 360		; 00000168H
	jl	SHORT $L74853

; 28   : 		nDegrees -= 360;

	mov	edx, DWORD PTR _nDegrees$[ebp]
	sub	edx, 360				; 00000168H
	mov	DWORD PTR _nDegrees$[ebp], edx
	jmp	SHORT $L74852
$L74853:

; 29   : 
; 30   : 	while (nDegrees < 0)

	cmp	DWORD PTR _nDegrees$[ebp], 0
	jge	SHORT $L74856

; 31   : 		nDegrees += 360;

	mov	eax, DWORD PTR _nDegrees$[ebp]
	add	eax, 360				; 00000168H
	mov	DWORD PTR _nDegrees$[ebp], eax
	jmp	SHORT $L74853
$L74856:

; 32   : 
; 33   : 	ASSERT (nDegrees >= 0 && nDegrees < 360);

	cmp	DWORD PTR _nDegrees$[ebp], 0
	jl	SHORT $L74861
	cmp	DWORD PTR _nDegrees$[ebp], 360		; 00000168H
	jl	SHORT $L74860
$L74861:
	push	33					; 00000021H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74860
	int	3
$L74860:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74856

; 34   : 
; 35   : 	m_dRadians = nDegrees * PI / 180;

	fild	DWORD PTR _nDegrees$[ebp]
	fmul	QWORD PTR _PI
	fdiv	QWORD PTR __real@8@4006b400000000000000
	mov	edx, DWORD PTR _this$[ebp]
	fstp	QWORD PTR [edx+8]

; 36   : }

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
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75909:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	ret	0
$L75916:
	mov	eax, OFFSET FLAT:$T75913
	jmp	___CxxFrameHandler
text$x	ENDS
??0CImageRotator@@QAE@HH@Z ENDP				; CImageRotator::CImageRotator
PUBLIC	??1CImageRotator@@UAE@XZ			; CImageRotator::~CImageRotator
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCImageRotator@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCImageRotator@@UAEPAXI@Z PROC NEAR			; CImageRotator::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CImageRotator@@UAE@XZ		; CImageRotator::~CImageRotator
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L74865
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L74865:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCImageRotator@@UAEPAXI@Z ENDP			; CImageRotator::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	__real@8@40008000000000000000
PUBLIC	__real@8@00000000000000000000
PUBLIC	??0CImageRotator@@QAE@N@Z			; CImageRotator::CImageRotator
;	COMDAT __real@8@40008000000000000000
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
__real@8@40008000000000000000 DQ 04000000000000000r ; 2
CONST	ENDS
;	COMDAT __real@8@00000000000000000000
CONST	SEGMENT
__real@8@00000000000000000000 DQ 00000000000000000r ; 0
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T75930	DD	019930520H
	DD	01H
	DD	FLAT:$T75936
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75936	DD	0ffffffffH
	DD	FLAT:$L75928
xdata$x	ENDS
;	COMDAT ??0CImageRotator@@QAE@N@Z
_TEXT	SEGMENT
_dRadians$ = 8
_this$ = -16
__$EHRec$ = -12
??0CImageRotator@@QAE@N@Z PROC NEAR			; CImageRotator::CImageRotator, COMDAT

; 39   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75935
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0C32BitImageProcessor@@QAE@H@Z	; C32BitImageProcessor::C32BitImageProcessor
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageRotator@@6B@ ; CImageRotator::`vftable'
$L74871:

; 40   : 	// normalize the angle
; 41   : 	while (dRadians >= 2 * PI)

	fld	QWORD PTR __real@8@40008000000000000000
	fmul	QWORD PTR _PI
	fcomp	QWORD PTR _dRadians$[ebp]
	fnstsw	ax
	test	ah, 65					; 00000041H
	je	SHORT $L74872

; 42   : 		dRadians -= 2 * PI;

	fld	QWORD PTR __real@8@40008000000000000000
	fmul	QWORD PTR _PI
	fsubr	QWORD PTR _dRadians$[ebp]
	fstp	QWORD PTR _dRadians$[ebp]
	jmp	SHORT $L74871
$L74872:

; 43   : 
; 44   : 	while (dRadians <= 0)

	fld	QWORD PTR _dRadians$[ebp]
	fcomp	QWORD PTR __real@8@00000000000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	je	SHORT $L74875

; 45   : 		dRadians += 2 * PI;

	fld	QWORD PTR __real@8@40008000000000000000
	fmul	QWORD PTR _PI
	fadd	QWORD PTR _dRadians$[ebp]
	fstp	QWORD PTR _dRadians$[ebp]
	jmp	SHORT $L74872
$L74875:

; 46   : 
; 47   : 	ASSERT (dRadians >= 0 && dRadians < 2 * PI);

	fld	QWORD PTR _dRadians$[ebp]
	fcomp	QWORD PTR __real@8@00000000000000000000
	fnstsw	ax
	test	ah, 1
	jne	SHORT $L74880
	fld	QWORD PTR __real@8@40008000000000000000
	fmul	QWORD PTR _PI
	fcomp	QWORD PTR _dRadians$[ebp]
	fnstsw	ax
	test	ah, 65					; 00000041H
	je	SHORT $L74879
$L74880:
	push	47					; 0000002fH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74879
	int	3
$L74879:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L74875

; 48   : 
; 49   : 	m_dRadians = dRadians;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _dRadians$[ebp]
	mov	DWORD PTR [edx+8], eax
	mov	ecx, DWORD PTR _dRadians$[ebp+4]
	mov	DWORD PTR [edx+12], ecx

; 50   : }

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
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L75928:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	ret	0
$L75935:
	mov	eax, OFFSET FLAT:$T75930
	jmp	___CxxFrameHandler
text$x	ENDS
??0CImageRotator@@QAE@N@Z ENDP				; CImageRotator::CImageRotator
;	COMDAT ??1CImageRotator@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CImageRotator@@UAE@XZ PROC NEAR			; CImageRotator::~CImageRotator, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageRotator@@6B@ ; CImageRotator::`vftable'

; 54   : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CImageRotator@@UAE@XZ ENDP				; CImageRotator::~CImageRotator
_TEXT	ENDS
EXTRN	__ftol:NEAR
EXTRN	_abs:NEAR
EXTRN	??0CSize@@QAE@HH@Z:NEAR				; CSize::CSize
EXTRN	??0CPoint@@QAE@XZ:NEAR				; CPoint::CPoint
EXTRN	_cos:NEAR
EXTRN	_sin:NEAR
;	COMDAT ?CalcDestSize@CImageRotator@@UAE?AVCSize@@V2@@Z
_TEXT	SEGMENT
_sizeSrc$ = 12
___$ReturnUdt$ = 8
_this$ = -60
_dCosA$ = -8
_dSinA$ = -32
_ptTopLeft$ = -24
_ptTopRight$ = -48
_ptBottomLeft$ = -56
_ptBottomRight$ = -16
_nMaxY$ = -40
_nMaxX$ = -36
?CalcDestSize@CImageRotator@@UAE?AVCSize@@V2@@Z PROC NEAR ; CImageRotator::CalcDestSize, COMDAT

; 57   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 148				; 00000094H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-148]
	mov	ecx, 37					; 00000025H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 58   : 	if (!m_dRadians || !sizeSrc.cx || !sizeSrc.cy)

	mov	eax, DWORD PTR _this$[ebp]
	fld	QWORD PTR [eax+8]
	fcomp	QWORD PTR __real@8@00000000000000000000
	fnstsw	ax
	test	ah, 64					; 00000040H
	jne	SHORT $L74890
	cmp	DWORD PTR _sizeSrc$[ebp], 0
	je	SHORT $L74890
	cmp	DWORD PTR _sizeSrc$[ebp+4], 0
	jne	SHORT $L74889
$L74890:

; 59   : 		return sizeSrc;

	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	mov	edx, DWORD PTR _sizeSrc$[ebp]
	mov	DWORD PTR [ecx], edx
	mov	eax, DWORD PTR _sizeSrc$[ebp+4]
	mov	DWORD PTR [ecx+4], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L74888
$L74889:

; 60   : 
; 61   : 	// calculate the four rotated corners
; 62   : 	double dCosA = cos(m_dRadians);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	push	edx
	mov	eax, DWORD PTR [ecx+8]
	push	eax
	call	_cos
	add	esp, 8
	fstp	QWORD PTR _dCosA$[ebp]

; 63   : 	double dSinA = sin(m_dRadians);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	push	edx
	mov	eax, DWORD PTR [ecx+8]
	push	eax
	call	_sin
	add	esp, 8
	fstp	QWORD PTR _dSinA$[ebp]

; 64   : 
; 65   : 	CPoint ptTopLeft, ptTopRight, ptBottomLeft, ptBottomRight;

	lea	ecx, DWORD PTR _ptTopLeft$[ebp]
	call	??0CPoint@@QAE@XZ			; CPoint::CPoint
	lea	ecx, DWORD PTR _ptTopRight$[ebp]
	call	??0CPoint@@QAE@XZ			; CPoint::CPoint
	lea	ecx, DWORD PTR _ptBottomLeft$[ebp]
	call	??0CPoint@@QAE@XZ			; CPoint::CPoint
	lea	ecx, DWORD PTR _ptBottomRight$[ebp]
	call	??0CPoint@@QAE@XZ			; CPoint::CPoint

; 66   : 
; 67   : 	ptTopLeft.x = (int)(-sizeSrc.cx * dCosA / 2 + sizeSrc.cy * dSinA / 2);

	mov	ecx, DWORD PTR _sizeSrc$[ebp]
	neg	ecx
	mov	DWORD PTR -64+[ebp], ecx
	fild	DWORD PTR -64+[ebp]
	fmul	QWORD PTR _dCosA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	fild	DWORD PTR _sizeSrc$[ebp+4]
	fmul	QWORD PTR _dSinA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	faddp	ST(1), ST(0)
	call	__ftol
	mov	DWORD PTR _ptTopLeft$[ebp], eax

; 68   : 	ptTopLeft.y = (int)(sizeSrc.cy * dCosA / 2 - (-sizeSrc.cx) * dSinA / 2);

	fild	DWORD PTR _sizeSrc$[ebp+4]
	fmul	QWORD PTR _dCosA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	mov	edx, DWORD PTR _sizeSrc$[ebp]
	neg	edx
	mov	DWORD PTR -68+[ebp], edx
	fild	DWORD PTR -68+[ebp]
	fmul	QWORD PTR _dSinA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	fsubp	ST(1), ST(0)
	call	__ftol
	mov	DWORD PTR _ptTopLeft$[ebp+4], eax

; 69   : 
; 70   : 	ptTopRight.x = (int)(sizeSrc.cx * dCosA / 2 + sizeSrc.cy * dSinA / 2);

	fild	DWORD PTR _sizeSrc$[ebp]
	fmul	QWORD PTR _dCosA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	fild	DWORD PTR _sizeSrc$[ebp+4]
	fmul	QWORD PTR _dSinA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	faddp	ST(1), ST(0)
	call	__ftol
	mov	DWORD PTR _ptTopRight$[ebp], eax

; 71   : 	ptTopRight.y = (int)(sizeSrc.cy * dCosA / 2 - sizeSrc.cx * dSinA / 2);

	fild	DWORD PTR _sizeSrc$[ebp+4]
	fmul	QWORD PTR _dCosA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	fild	DWORD PTR _sizeSrc$[ebp]
	fmul	QWORD PTR _dSinA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	fsubp	ST(1), ST(0)
	call	__ftol
	mov	DWORD PTR _ptTopRight$[ebp+4], eax

; 72   : 
; 73   : 	ptBottomLeft.x = (int)(-sizeSrc.cx * dCosA / 2 + (-sizeSrc.cy) * dSinA / 2);

	mov	eax, DWORD PTR _sizeSrc$[ebp]
	neg	eax
	mov	DWORD PTR -72+[ebp], eax
	fild	DWORD PTR -72+[ebp]
	fmul	QWORD PTR _dCosA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	mov	ecx, DWORD PTR _sizeSrc$[ebp+4]
	neg	ecx
	mov	DWORD PTR -76+[ebp], ecx
	fild	DWORD PTR -76+[ebp]
	fmul	QWORD PTR _dSinA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	faddp	ST(1), ST(0)
	call	__ftol
	mov	DWORD PTR _ptBottomLeft$[ebp], eax

; 74   : 	ptBottomLeft.y = (int)(-sizeSrc.cy * dCosA / 2 - (-sizeSrc.cx) * dSinA / 2);

	mov	edx, DWORD PTR _sizeSrc$[ebp+4]
	neg	edx
	mov	DWORD PTR -80+[ebp], edx
	fild	DWORD PTR -80+[ebp]
	fmul	QWORD PTR _dCosA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	mov	eax, DWORD PTR _sizeSrc$[ebp]
	neg	eax
	mov	DWORD PTR -84+[ebp], eax
	fild	DWORD PTR -84+[ebp]
	fmul	QWORD PTR _dSinA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	fsubp	ST(1), ST(0)
	call	__ftol
	mov	DWORD PTR _ptBottomLeft$[ebp+4], eax

; 75   : 
; 76   : 	ptBottomRight.x = (int)(sizeSrc.cx * dCosA / 2 + (-sizeSrc.cy) * dSinA / 2);

	fild	DWORD PTR _sizeSrc$[ebp]
	fmul	QWORD PTR _dCosA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	mov	ecx, DWORD PTR _sizeSrc$[ebp+4]
	neg	ecx
	mov	DWORD PTR -88+[ebp], ecx
	fild	DWORD PTR -88+[ebp]
	fmul	QWORD PTR _dSinA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	faddp	ST(1), ST(0)
	call	__ftol
	mov	DWORD PTR _ptBottomRight$[ebp], eax

; 77   : 	ptBottomRight.y = (int)(-sizeSrc.cy * dCosA / 2 - sizeSrc.cx * dSinA / 2);

	mov	edx, DWORD PTR _sizeSrc$[ebp+4]
	neg	edx
	mov	DWORD PTR -92+[ebp], edx
	fild	DWORD PTR -92+[ebp]
	fmul	QWORD PTR _dCosA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	fild	DWORD PTR _sizeSrc$[ebp]
	fmul	QWORD PTR _dSinA$[ebp]
	fdiv	QWORD PTR __real@8@40008000000000000000
	fsubp	ST(1), ST(0)
	call	__ftol
	mov	DWORD PTR _ptBottomRight$[ebp+4], eax

; 78   : 
; 79   : 	// find the max absolute values in each direction
; 80   : 	int nMaxY = max(abs(ptTopLeft.y), max(abs(ptTopRight.y), max(abs(ptBottomLeft.y), abs(ptBottomRight.y))));

	mov	eax, DWORD PTR _ptBottomLeft$[ebp+4]
	push	eax
	call	_abs
	add	esp, 4
	mov	esi, eax
	mov	ecx, DWORD PTR _ptBottomRight$[ebp+4]
	push	ecx
	call	_abs
	add	esp, 4
	cmp	esi, eax
	jle	SHORT $L75944
	mov	edx, DWORD PTR _ptBottomLeft$[ebp+4]
	push	edx
	call	_abs
	add	esp, 4
	mov	DWORD PTR -96+[ebp], eax
	jmp	SHORT $L75945
$L75944:
	mov	eax, DWORD PTR _ptBottomRight$[ebp+4]
	push	eax
	call	_abs
	add	esp, 4
	mov	DWORD PTR -96+[ebp], eax
$L75945:
	mov	ecx, DWORD PTR _ptTopRight$[ebp+4]
	push	ecx
	call	_abs
	add	esp, 4
	cmp	eax, DWORD PTR -96+[ebp]
	jle	SHORT $L75948
	mov	edx, DWORD PTR _ptTopRight$[ebp+4]
	push	edx
	call	_abs
	add	esp, 4
	mov	DWORD PTR -100+[ebp], eax
	jmp	SHORT $L75949
$L75948:
	mov	eax, DWORD PTR _ptBottomLeft$[ebp+4]
	push	eax
	call	_abs
	add	esp, 4
	mov	esi, eax
	mov	ecx, DWORD PTR _ptBottomRight$[ebp+4]
	push	ecx
	call	_abs
	add	esp, 4
	cmp	esi, eax
	jle	SHORT $L75946
	mov	edx, DWORD PTR _ptBottomLeft$[ebp+4]
	push	edx
	call	_abs
	add	esp, 4
	mov	DWORD PTR -104+[ebp], eax
	jmp	SHORT $L75947
$L75946:
	mov	eax, DWORD PTR _ptBottomRight$[ebp+4]
	push	eax
	call	_abs
	add	esp, 4
	mov	DWORD PTR -104+[ebp], eax
$L75947:
	mov	ecx, DWORD PTR -104+[ebp]
	mov	DWORD PTR -100+[ebp], ecx
$L75949:
	mov	edx, DWORD PTR _ptTopLeft$[ebp+4]
	push	edx
	call	_abs
	add	esp, 4
	cmp	eax, DWORD PTR -100+[ebp]
	jle	SHORT $L75956
	mov	eax, DWORD PTR _ptTopLeft$[ebp+4]
	push	eax
	call	_abs
	add	esp, 4
	mov	DWORD PTR -108+[ebp], eax
	jmp	$L75957
$L75956:
	mov	ecx, DWORD PTR _ptBottomLeft$[ebp+4]
	push	ecx
	call	_abs
	add	esp, 4
	mov	esi, eax
	mov	edx, DWORD PTR _ptBottomRight$[ebp+4]
	push	edx
	call	_abs
	add	esp, 4
	cmp	esi, eax
	jle	SHORT $L75950
	mov	eax, DWORD PTR _ptBottomLeft$[ebp+4]
	push	eax
	call	_abs
	add	esp, 4
	mov	DWORD PTR -112+[ebp], eax
	jmp	SHORT $L75951
$L75950:
	mov	ecx, DWORD PTR _ptBottomRight$[ebp+4]
	push	ecx
	call	_abs
	add	esp, 4
	mov	DWORD PTR -112+[ebp], eax
$L75951:
	mov	edx, DWORD PTR _ptTopRight$[ebp+4]
	push	edx
	call	_abs
	add	esp, 4
	cmp	eax, DWORD PTR -112+[ebp]
	jle	SHORT $L75954
	mov	eax, DWORD PTR _ptTopRight$[ebp+4]
	push	eax
	call	_abs
	add	esp, 4
	mov	DWORD PTR -116+[ebp], eax
	jmp	SHORT $L75955
$L75954:
	mov	ecx, DWORD PTR _ptBottomLeft$[ebp+4]
	push	ecx
	call	_abs
	add	esp, 4
	mov	esi, eax
	mov	edx, DWORD PTR _ptBottomRight$[ebp+4]
	push	edx
	call	_abs
	add	esp, 4
	cmp	esi, eax
	jle	SHORT $L75952
	mov	eax, DWORD PTR _ptBottomLeft$[ebp+4]
	push	eax
	call	_abs
	add	esp, 4
	mov	DWORD PTR -120+[ebp], eax
	jmp	SHORT $L75953
$L75952:
	mov	ecx, DWORD PTR _ptBottomRight$[ebp+4]
	push	ecx
	call	_abs
	add	esp, 4
	mov	DWORD PTR -120+[ebp], eax
$L75953:
	mov	edx, DWORD PTR -120+[ebp]
	mov	DWORD PTR -116+[ebp], edx
$L75955:
	mov	eax, DWORD PTR -116+[ebp]
	mov	DWORD PTR -108+[ebp], eax
$L75957:
	mov	ecx, DWORD PTR -108+[ebp]
	mov	DWORD PTR _nMaxY$[ebp], ecx

; 81   : 	int nMaxX = max(abs(ptTopLeft.x), max(abs(ptTopRight.x), max(abs(ptBottomLeft.x), abs(ptBottomRight.x))));

	mov	edx, DWORD PTR _ptBottomLeft$[ebp]
	push	edx
	call	_abs
	add	esp, 4
	mov	esi, eax
	mov	eax, DWORD PTR _ptBottomRight$[ebp]
	push	eax
	call	_abs
	add	esp, 4
	cmp	esi, eax
	jle	SHORT $L75958
	mov	ecx, DWORD PTR _ptBottomLeft$[ebp]
	push	ecx
	call	_abs
	add	esp, 4
	mov	DWORD PTR -124+[ebp], eax
	jmp	SHORT $L75959
$L75958:
	mov	edx, DWORD PTR _ptBottomRight$[ebp]
	push	edx
	call	_abs
	add	esp, 4
	mov	DWORD PTR -124+[ebp], eax
$L75959:
	mov	eax, DWORD PTR _ptTopRight$[ebp]
	push	eax
	call	_abs
	add	esp, 4
	cmp	eax, DWORD PTR -124+[ebp]
	jle	SHORT $L75962
	mov	ecx, DWORD PTR _ptTopRight$[ebp]
	push	ecx
	call	_abs
	add	esp, 4
	mov	DWORD PTR -128+[ebp], eax
	jmp	SHORT $L75963
$L75962:
	mov	edx, DWORD PTR _ptBottomLeft$[ebp]
	push	edx
	call	_abs
	add	esp, 4
	mov	esi, eax
	mov	eax, DWORD PTR _ptBottomRight$[ebp]
	push	eax
	call	_abs
	add	esp, 4
	cmp	esi, eax
	jle	SHORT $L75960
	mov	ecx, DWORD PTR _ptBottomLeft$[ebp]
	push	ecx
	call	_abs
	add	esp, 4
	mov	DWORD PTR -132+[ebp], eax
	jmp	SHORT $L75961
$L75960:
	mov	edx, DWORD PTR _ptBottomRight$[ebp]
	push	edx
	call	_abs
	add	esp, 4
	mov	DWORD PTR -132+[ebp], eax
$L75961:
	mov	eax, DWORD PTR -132+[ebp]
	mov	DWORD PTR -128+[ebp], eax
$L75963:
	mov	ecx, DWORD PTR _ptTopLeft$[ebp]
	push	ecx
	call	_abs
	add	esp, 4
	cmp	eax, DWORD PTR -128+[ebp]
	jle	SHORT $L75970
	mov	edx, DWORD PTR _ptTopLeft$[ebp]
	push	edx
	call	_abs
	add	esp, 4
	mov	DWORD PTR -136+[ebp], eax
	jmp	$L75971
$L75970:
	mov	eax, DWORD PTR _ptBottomLeft$[ebp]
	push	eax
	call	_abs
	add	esp, 4
	mov	esi, eax
	mov	ecx, DWORD PTR _ptBottomRight$[ebp]
	push	ecx
	call	_abs
	add	esp, 4
	cmp	esi, eax
	jle	SHORT $L75964
	mov	edx, DWORD PTR _ptBottomLeft$[ebp]
	push	edx
	call	_abs
	add	esp, 4
	mov	DWORD PTR -140+[ebp], eax
	jmp	SHORT $L75965
$L75964:
	mov	eax, DWORD PTR _ptBottomRight$[ebp]
	push	eax
	call	_abs
	add	esp, 4
	mov	DWORD PTR -140+[ebp], eax
$L75965:
	mov	ecx, DWORD PTR _ptTopRight$[ebp]
	push	ecx
	call	_abs
	add	esp, 4
	cmp	eax, DWORD PTR -140+[ebp]
	jle	SHORT $L75968
	mov	edx, DWORD PTR _ptTopRight$[ebp]
	push	edx
	call	_abs
	add	esp, 4
	mov	DWORD PTR -144+[ebp], eax
	jmp	SHORT $L75969
$L75968:
	mov	eax, DWORD PTR _ptBottomLeft$[ebp]
	push	eax
	call	_abs
	add	esp, 4
	mov	esi, eax
	mov	ecx, DWORD PTR _ptBottomRight$[ebp]
	push	ecx
	call	_abs
	add	esp, 4
	cmp	esi, eax
	jle	SHORT $L75966
	mov	edx, DWORD PTR _ptBottomLeft$[ebp]
	push	edx
	call	_abs
	add	esp, 4
	mov	DWORD PTR -148+[ebp], eax
	jmp	SHORT $L75967
$L75966:
	mov	eax, DWORD PTR _ptBottomRight$[ebp]
	push	eax
	call	_abs
	add	esp, 4
	mov	DWORD PTR -148+[ebp], eax
$L75967:
	mov	ecx, DWORD PTR -148+[ebp]
	mov	DWORD PTR -144+[ebp], ecx
$L75969:
	mov	edx, DWORD PTR -144+[ebp]
	mov	DWORD PTR -136+[ebp], edx
$L75971:
	mov	eax, DWORD PTR -136+[ebp]
	mov	DWORD PTR _nMaxX$[ebp], eax

; 82   : 	
; 83   : 	return CSize((nMaxX + 1) * 2, (nMaxY + 1) * 2);

	mov	ecx, DWORD PTR _nMaxY$[ebp]
	lea	edx, DWORD PTR [ecx+ecx+2]
	push	edx
	mov	eax, DWORD PTR _nMaxX$[ebp]
	lea	ecx, DWORD PTR [eax+eax+2]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CSize@@QAE@HH@Z			; CSize::CSize
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L74888:

; 84   : }

	pop	edi
	pop	esi
	add	esp, 148				; 00000094H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?CalcDestSize@CImageRotator@@UAE?AVCSize@@V2@@Z ENDP	; CImageRotator::CalcDestSize
_TEXT	ENDS
PUBLIC	??4RGBX@@QAEABV0@ABV0@@Z			; RGBX::operator=
EXTRN	?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z:NEAR ; C32BitImageProcessor::ProcessPixels
EXTRN	?CalcWeightedColor@C32BitImageProcessor@@QAEXPAVRGBX@@VCSize@@NNAAV2@@Z:NEAR ; C32BitImageProcessor::CalcWeightedColor
EXTRN	??GCSize@@QBE?AV0@XZ:NEAR			; CSize::operator-
EXTRN	??0CPoint@@QAE@HH@Z:NEAR			; CPoint::CPoint
EXTRN	?Offset@CPoint@@QAEXUtagSIZE@@@Z:NEAR		; CPoint::Offset
EXTRN	??0CRect@@QAE@HHHH@Z:NEAR			; CRect::CRect
EXTRN	?PtInRect@CRect@@QBEHUtagPOINT@@@Z:NEAR		; CRect::PtInRect
EXTRN	?OffsetRect@CRect@@QAEXUtagSIZE@@@Z:NEAR	; CRect::OffsetRect
;	COMDAT ?ProcessPixels@CImageRotator@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_this$ = -144
_bRes$ = -4
_sizeDestOffset$74919 = -56
_sizeSrcOffset$74920 = -40
_rSrc$74921 = -28
_dCosA$74924 = -12
_dSinA$74925 = -48
_nY$74926 = -32
_dCosYComponent$74930 = -76
_dSinYComponent$74931 = -64
_dSrcX$74932 = -84
_dSrcY$74933 = -92
_nX$74934 = -68
_ptSrc$74940 = -100
_nPixel$74941 = -104
_pRGBSrc$74946 = -108
$T75975 = -116
$T75976 = -124
$T75977 = -132
$T75978 = -140
?ProcessPixels@CImageRotator@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; CImageRotator::ProcessPixels, COMDAT

; 88   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 160				; 000000a0H
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-160]
	mov	ecx, 40					; 00000028H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 89   : 	BOOL bRes = TRUE;

	mov	DWORD PTR _bRes$[ebp], 1

; 90   : 
; 91   : 	if (!m_dRadians)

	mov	eax, DWORD PTR _this$[ebp]
	fld	QWORD PTR [eax+8]
	fcomp	QWORD PTR __real@8@00000000000000000000
	fnstsw	ax
	test	ah, 64					; 00000040H
	je	SHORT $L74917

; 92   : 		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	push	-1
	mov	ecx, DWORD PTR _sizeDest$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _sizeDest$[ebp]
	push	edx
	mov	eax, DWORD PTR _pDestPixels$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sizeSrc$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _sizeSrc$[ebp]
	push	edx
	mov	eax, DWORD PTR _pSrcPixels$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z ; C32BitImageProcessor::ProcessPixels
	mov	DWORD PTR _bRes$[ebp], eax

; 93   : 	else

	jmp	$L74929
$L74917:

; 95   : 		// note: we also need to translate the coords after rotating
; 96   : 		CSize sizeDestOffset(sizeDest.cx / 2 + sizeDest.cx % 2, sizeDest.cy / 2 + sizeDest.cy % 2);

	mov	eax, DWORD PTR _sizeDest$[ebp+4]
	cdq
	sub	eax, edx
	sar	eax, 1
	mov	ecx, DWORD PTR _sizeDest$[ebp+4]
	and	ecx, -2147483647			; 80000001H
	jns	SHORT $L75979
	dec	ecx
	or	ecx, -2					; fffffffeH
	inc	ecx
$L75979:
	add	eax, ecx
	push	eax
	mov	eax, DWORD PTR _sizeDest$[ebp]
	cdq
	sub	eax, edx
	sar	eax, 1
	mov	edx, DWORD PTR _sizeDest$[ebp]
	and	edx, -2147483647			; 80000001H
	jns	SHORT $L75980
	dec	edx
	or	edx, -2					; fffffffeH
	inc	edx
$L75980:
	add	eax, edx
	push	eax
	lea	ecx, DWORD PTR _sizeDestOffset$74919[ebp]
	call	??0CSize@@QAE@HH@Z			; CSize::CSize

; 97   : 		CSize sizeSrcOffset(sizeSrc.cx / 2 + sizeSrc.cx % 2, sizeSrc.cy / 2 + sizeSrc.cy % 2);

	mov	eax, DWORD PTR _sizeSrc$[ebp+4]
	cdq
	sub	eax, edx
	sar	eax, 1
	mov	ecx, DWORD PTR _sizeSrc$[ebp+4]
	and	ecx, -2147483647			; 80000001H
	jns	SHORT $L75981
	dec	ecx
	or	ecx, -2					; fffffffeH
	inc	ecx
$L75981:
	add	eax, ecx
	push	eax
	mov	eax, DWORD PTR _sizeSrc$[ebp]
	cdq
	sub	eax, edx
	sar	eax, 1
	mov	edx, DWORD PTR _sizeSrc$[ebp]
	and	edx, -2147483647			; 80000001H
	jns	SHORT $L75982
	dec	edx
	or	edx, -2					; fffffffeH
	inc	edx
$L75982:
	add	eax, edx
	push	eax
	lea	ecx, DWORD PTR _sizeSrcOffset$74920[ebp]
	call	??0CSize@@QAE@HH@Z			; CSize::CSize

; 98   : 
; 99   : 		CRect rSrc(0, 0, sizeSrc.cx - 1, sizeSrc.cy - 1);

	mov	eax, DWORD PTR _sizeSrc$[ebp+4]
	sub	eax, 1
	push	eax
	mov	ecx, DWORD PTR _sizeSrc$[ebp]
	sub	ecx, 1
	push	ecx
	push	0
	push	0
	lea	ecx, DWORD PTR _rSrc$74921[ebp]
	call	??0CRect@@QAE@HHHH@Z			; CRect::CRect

; 100  : 		rSrc.OffsetRect(-sizeSrcOffset);

	lea	edx, DWORD PTR $T75976[ebp]
	push	edx
	lea	ecx, DWORD PTR _sizeSrcOffset$74920[ebp]
	call	??GCSize@@QBE?AV0@XZ			; CSize::operator-
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR $T75975[ebp], ecx
	mov	DWORD PTR $T75975[ebp+4], edx
	mov	eax, DWORD PTR $T75975[ebp+4]
	push	eax
	mov	ecx, DWORD PTR $T75975[ebp]
	push	ecx
	lea	ecx, DWORD PTR _rSrc$74921[ebp]
	call	?OffsetRect@CRect@@QAEXUtagSIZE@@@Z	; CRect::OffsetRect

; 101  : 
; 102  : 		// note: traversing the src bitmap leads to artifacts in the destination image
; 103  : 		// what we do is to traverse the destination bitmaps and compute the equivalent 
; 104  : 		// source color - credit for this observation goes to Yves Maurer (GDIRotate) 2002
; 105  : 		double dCosA = cos(m_dRadians);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR [edx+8]
	push	ecx
	call	_cos
	add	esp, 8
	fstp	QWORD PTR _dCosA$74924[ebp]

; 106  : 		double dSinA = sin(m_dRadians);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR [edx+8]
	push	ecx
	call	_sin
	add	esp, 8
	fstp	QWORD PTR _dSinA$74925[ebp]

; 107  : 
; 108  : 		for (int nY = 0; nY < sizeDest.cy; nY++)

	mov	DWORD PTR _nY$74926[ebp], 0
	jmp	SHORT $L74927
$L74928:
	mov	edx, DWORD PTR _nY$74926[ebp]
	add	edx, 1
	mov	DWORD PTR _nY$74926[ebp], edx
$L74927:
	mov	eax, DWORD PTR _nY$74926[ebp]
	cmp	eax, DWORD PTR _sizeDest$[ebp+4]
	jge	$L74929

; 110  : 			// calc y components of rotation
; 111  : 			double dCosYComponent = (nY - sizeDestOffset.cy) * dCosA;

	mov	ecx, DWORD PTR _nY$74926[ebp]
	sub	ecx, DWORD PTR _sizeDestOffset$74919[ebp+4]
	mov	DWORD PTR -148+[ebp], ecx
	fild	DWORD PTR -148+[ebp]
	fmul	QWORD PTR _dCosA$74924[ebp]
	fstp	QWORD PTR _dCosYComponent$74930[ebp]

; 112  : 			double dSinYComponent = (nY - sizeDestOffset.cy) * dSinA;

	mov	edx, DWORD PTR _nY$74926[ebp]
	sub	edx, DWORD PTR _sizeDestOffset$74919[ebp+4]
	mov	DWORD PTR -152+[ebp], edx
	fild	DWORD PTR -152+[ebp]
	fmul	QWORD PTR _dSinA$74925[ebp]
	fstp	QWORD PTR _dSinYComponent$74931[ebp]

; 113  : 
; 114  : 			double dSrcX = -sizeDestOffset.cx * dCosA + dSinYComponent;

	mov	eax, DWORD PTR _sizeDestOffset$74919[ebp]
	neg	eax
	mov	DWORD PTR -156+[ebp], eax
	fild	DWORD PTR -156+[ebp]
	fmul	QWORD PTR _dCosA$74924[ebp]
	fadd	QWORD PTR _dSinYComponent$74931[ebp]
	fstp	QWORD PTR _dSrcX$74932[ebp]

; 115  : 			double dSrcY = dCosYComponent - (-sizeDestOffset.cx * dSinA);

	mov	ecx, DWORD PTR _sizeDestOffset$74919[ebp]
	neg	ecx
	mov	DWORD PTR -160+[ebp], ecx
	fild	DWORD PTR -160+[ebp]
	fmul	QWORD PTR _dSinA$74925[ebp]
	fsubr	QWORD PTR _dCosYComponent$74930[ebp]
	fstp	QWORD PTR _dSrcY$74933[ebp]

; 116  : 
; 117  : 			for (int nX = 0; nX < sizeDest.cx; nX++)

	mov	DWORD PTR _nX$74934[ebp], 0
	jmp	SHORT $L74935
$L74936:
	mov	edx, DWORD PTR _nX$74934[ebp]
	add	edx, 1
	mov	DWORD PTR _nX$74934[ebp], edx
$L74935:
	mov	eax, DWORD PTR _nX$74934[ebp]
	cmp	eax, DWORD PTR _sizeDest$[ebp]
	jge	$L74937

; 119  : 				dSrcX += dCosA;

	fld	QWORD PTR _dSrcX$74932[ebp]
	fadd	QWORD PTR _dCosA$74924[ebp]
	fstp	QWORD PTR _dSrcX$74932[ebp]

; 120  : 				dSrcY -= dSinA;

	fld	QWORD PTR _dSrcY$74933[ebp]
	fsub	QWORD PTR _dSinA$74925[ebp]
	fst	QWORD PTR _dSrcY$74933[ebp]

; 121  : 
; 122  : 				CPoint ptSrc((int)dSrcX, (int)dSrcY);

	call	__ftol
	push	eax
	fld	QWORD PTR _dSrcX$74932[ebp]
	call	__ftol
	push	eax
	lea	ecx, DWORD PTR _ptSrc$74940[ebp]
	call	??0CPoint@@QAE@HH@Z			; CPoint::CPoint

; 123  : 				int nPixel = (nY * sizeDest.cx + nX);

	mov	ecx, DWORD PTR _nY$74926[ebp]
	imul	ecx, DWORD PTR _sizeDest$[ebp]
	add	ecx, DWORD PTR _nX$74934[ebp]
	mov	DWORD PTR _nPixel$74941[ebp], ecx

; 124  : 
; 125  : 				if (rSrc.PtInRect(ptSrc))

	mov	edx, DWORD PTR _ptSrc$74940[ebp]
	mov	DWORD PTR $T75977[ebp], edx
	mov	eax, DWORD PTR _ptSrc$74940[ebp+4]
	mov	DWORD PTR $T75977[ebp+4], eax
	mov	ecx, DWORD PTR $T75977[ebp+4]
	push	ecx
	mov	edx, DWORD PTR $T75977[ebp]
	push	edx
	lea	ecx, DWORD PTR _rSrc$74921[ebp]
	call	?PtInRect@CRect@@QBEHUtagPOINT@@@Z	; CRect::PtInRect
	test	eax, eax
	je	$L74947

; 127  : 					if (!m_bWeightingEnabled)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L74944

; 129  : 						ptSrc.Offset(sizeSrcOffset);

	mov	ecx, DWORD PTR _sizeSrcOffset$74920[ebp]
	mov	DWORD PTR $T75978[ebp], ecx
	mov	edx, DWORD PTR _sizeSrcOffset$74920[ebp+4]
	mov	DWORD PTR $T75978[ebp+4], edx
	mov	eax, DWORD PTR $T75978[ebp+4]
	push	eax
	mov	ecx, DWORD PTR $T75978[ebp]
	push	ecx
	lea	ecx, DWORD PTR _ptSrc$74940[ebp]
	call	?Offset@CPoint@@QAEXUtagSIZE@@@Z	; CPoint::Offset

; 130  : 						RGBX* pRGBSrc = &pSrcPixels[ptSrc.y * sizeSrc.cx + ptSrc.x];

	mov	edx, DWORD PTR _ptSrc$74940[ebp+4]
	imul	edx, DWORD PTR _sizeSrc$[ebp]
	add	edx, DWORD PTR _ptSrc$74940[ebp]
	mov	eax, DWORD PTR _pSrcPixels$[ebp]
	lea	ecx, DWORD PTR [eax+edx*4]
	mov	DWORD PTR _pRGBSrc$74946[ebp], ecx

; 131  : 						
; 132  : 						pDestPixels[nPixel] = *pRGBSrc;

	mov	edx, DWORD PTR _pRGBSrc$74946[ebp]
	push	edx
	mov	eax, DWORD PTR _nPixel$74941[ebp]
	mov	ecx, DWORD PTR _pDestPixels$[ebp]
	lea	ecx, DWORD PTR [ecx+eax*4]
	call	??4RGBX@@QAEABV0@ABV0@@Z		; RGBX::operator=

; 134  : 					else

	jmp	SHORT $L74947
$L74944:

; 135  : 						CalcWeightedColor(pSrcPixels, sizeSrc, 
; 136  : 										dSrcX + sizeSrcOffset.cx, dSrcY + sizeSrcOffset.cy,
; 137  : 										pDestPixels[nPixel]);

	mov	edx, DWORD PTR _nPixel$74941[ebp]
	mov	eax, DWORD PTR _pDestPixels$[ebp]
	lea	ecx, DWORD PTR [eax+edx*4]
	push	ecx
	fild	DWORD PTR _sizeSrcOffset$74920[ebp+4]
	fadd	QWORD PTR _dSrcY$74933[ebp]
	sub	esp, 8
	fstp	QWORD PTR [esp]
	fild	DWORD PTR _sizeSrcOffset$74920[ebp]
	fadd	QWORD PTR _dSrcX$74932[ebp]
	sub	esp, 8
	fstp	QWORD PTR [esp]
	mov	edx, DWORD PTR _sizeSrc$[ebp+4]
	push	edx
	mov	eax, DWORD PTR _sizeSrc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pSrcPixels$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?CalcWeightedColor@C32BitImageProcessor@@QAEXPAVRGBX@@VCSize@@NNAAV2@@Z ; C32BitImageProcessor::CalcWeightedColor
$L74947:

; 139  : 			}

	jmp	$L74936
$L74937:

; 140  : 		}

	jmp	$L74928
$L74929:

; 142  : 
; 143  : 	return bRes;

	mov	eax, DWORD PTR _bRes$[ebp]

; 144  : }

	pop	edi
	add	esp, 160				; 000000a0H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?ProcessPixels@CImageRotator@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; CImageRotator::ProcessPixels
_TEXT	ENDS
;	COMDAT ??4RGBX@@QAEABV0@ABV0@@Z
_TEXT	SEGMENT
_rgb$ = 8
_this$ = -4
??4RGBX@@QAEABV0@ABV0@@Z PROC NEAR			; RGBX::operator=, COMDAT

; 43   : 	inline const RGBX& operator=(const RGBX& rgb) { btRed = rgb.btRed; btBlue = rgb.btBlue; btGreen = rgb.btGreen; btUnused = 0; return *this; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _rgb$[ebp]
	mov	dl, BYTE PTR [ecx+2]
	mov	BYTE PTR [eax+2], dl
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _rgb$[ebp]
	mov	dl, BYTE PTR [ecx]
	mov	BYTE PTR [eax], dl
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _rgb$[ebp]
	mov	dl, BYTE PTR [ecx+1]
	mov	BYTE PTR [eax+1], dl
	mov	eax, DWORD PTR _this$[ebp]
	mov	BYTE PTR [eax+3], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	4
??4RGBX@@QAEABV0@ABV0@@Z ENDP				; RGBX::operator=
_TEXT	ENDS
PUBLIC	??0CImageShearer@@QAE@HHH@Z			; CImageShearer::CImageShearer
PUBLIC	?CalcDestSize@CImageShearer@@UAE?AVCSize@@V2@@Z	; CImageShearer::CalcDestSize
PUBLIC	?ProcessPixels@CImageShearer@@UAEHPAVRGBX@@VCSize@@01K@Z ; CImageShearer::ProcessPixels
PUBLIC	??_7CImageShearer@@6B@				; CImageShearer::`vftable'
PUBLIC	??_GCImageShearer@@UAEPAXI@Z			; CImageShearer::`scalar deleting destructor'
PUBLIC	??_ECImageShearer@@UAEPAXI@Z			; CImageShearer::`vector deleting destructor'
;	COMDAT ??_7CImageShearer@@6B@
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
??_7CImageShearer@@6B@ DD FLAT:??_ECImageShearer@@UAEPAXI@Z ; CImageShearer::`vftable'
	DD	FLAT:?CalcDestSize@CImageShearer@@UAE?AVCSize@@V2@@Z
	DD	FLAT:?ProcessPixels@CImageShearer@@UAEHPAVRGBX@@VCSize@@01K@Z
CONST	ENDS
;	COMDAT ??0CImageShearer@@QAE@HHH@Z
_TEXT	SEGMENT
_nHorz$ = 8
_nVert$ = 12
_bEnableWeighting$ = 16
_this$ = -4
??0CImageShearer@@QAE@HHH@Z PROC NEAR			; CImageShearer::CImageShearer, COMDAT

; 150  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _bEnableWeighting$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0C32BitImageProcessor@@QAE@H@Z	; C32BitImageProcessor::C32BitImageProcessor
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nHorz$[ebp]
	mov	DWORD PTR [ecx+8], edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nVert$[ebp]
	mov	DWORD PTR [eax+12], ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7CImageShearer@@6B@ ; CImageShearer::`vftable'

; 151  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
??0CImageShearer@@QAE@HHH@Z ENDP			; CImageShearer::CImageShearer
_TEXT	ENDS
PUBLIC	??1CImageShearer@@UAE@XZ			; CImageShearer::~CImageShearer
;	COMDAT ??_GCImageShearer@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCImageShearer@@UAEPAXI@Z PROC NEAR			; CImageShearer::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CImageShearer@@UAE@XZ		; CImageShearer::~CImageShearer
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L74957
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L74957:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCImageShearer@@UAEPAXI@Z ENDP			; CImageShearer::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CImageShearer@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CImageShearer@@UAE@XZ PROC NEAR			; CImageShearer::~CImageShearer, COMDAT

; 154  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageShearer@@6B@ ; CImageShearer::`vftable'

; 155  : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CImageShearer@@UAE@XZ ENDP				; CImageShearer::~CImageShearer
_TEXT	ENDS
;	COMDAT ?CalcDestSize@CImageShearer@@UAE?AVCSize@@V2@@Z
_TEXT	SEGMENT
_sizeSrc$ = 12
___$ReturnUdt$ = 8
_this$ = -4
?CalcDestSize@CImageShearer@@UAE?AVCSize@@V2@@Z PROC NEAR ; CImageShearer::CalcDestSize, COMDAT

; 158  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 159  : 	return CSize(sizeSrc.cx + abs(m_nHorz), sizeSrc.cy + abs(m_nVert));

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	push	ecx
	call	_abs
	add	esp, 4
	mov	edx, DWORD PTR _sizeSrc$[ebp+4]
	add	edx, eax
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	push	ecx
	call	_abs
	add	esp, 4
	mov	edx, DWORD PTR _sizeSrc$[ebp]
	add	edx, eax
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CSize@@QAE@HH@Z			; CSize::CSize
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 160  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?CalcDestSize@CImageShearer@@UAE?AVCSize@@V2@@Z ENDP	; CImageShearer::CalcDestSize
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageShearer@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_this$ = -52
_bRes$ = -4
_nX$74978 = -8
_dYOffset$74982 = -16
_nY$74988 = -20
_dXOffset$74992 = -44
_dSrcX$74998 = -28
_dSrcY$74999 = -36
_pRGBSrc$75006 = -48
?ProcessPixels@CImageShearer@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; CImageShearer::ProcessPixels, COMDAT

; 164  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 68					; 00000044H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-68]
	mov	ecx, 17					; 00000011H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 165  : 	BOOL bRes = TRUE;

	mov	DWORD PTR _bRes$[ebp], 1

; 166  : 
; 167  : 	if (!m_nHorz && !m_nVert)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	jne	SHORT $L74976
	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jne	SHORT $L74976

; 168  : 		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	push	-1
	mov	edx, DWORD PTR _sizeDest$[ebp+4]
	push	edx
	mov	eax, DWORD PTR _sizeDest$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDestPixels$[ebp]
	push	ecx
	mov	edx, DWORD PTR _sizeSrc$[ebp+4]
	push	edx
	mov	eax, DWORD PTR _sizeSrc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pSrcPixels$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z ; C32BitImageProcessor::ProcessPixels
	mov	DWORD PTR _bRes$[ebp], eax

; 169  : 	else

	jmp	$L74981
$L74976:

; 171  : 		// shears +ve (down) or -ve (up)
; 172  : 		for (int nX = 0; nX < sizeDest.cx; nX++)

	mov	DWORD PTR _nX$74978[ebp], 0
	jmp	SHORT $L74979
$L74980:
	mov	edx, DWORD PTR _nX$74978[ebp]
	add	edx, 1
	mov	DWORD PTR _nX$74978[ebp], edx
$L74979:
	mov	eax, DWORD PTR _nX$74978[ebp]
	cmp	eax, DWORD PTR _sizeDest$[ebp]
	jge	$L74981

; 174  : 			double dYOffset = 0;

	mov	DWORD PTR _dYOffset$74982[ebp], 0
	mov	DWORD PTR _dYOffset$74982[ebp+4], 0

; 175  : 
; 176  : 			// calc the offset to src Y coord
; 177  : 			if (m_nVert > 0)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jle	SHORT $L74983

; 178  : 				dYOffset = (double)m_nVert * nX / sizeDest.cx;

	mov	edx, DWORD PTR _this$[ebp]
	fild	DWORD PTR [edx+12]
	fimul	DWORD PTR _nX$74978[ebp]
	fidiv	DWORD PTR _sizeDest$[ebp]
	fstp	QWORD PTR _dYOffset$74982[ebp]

; 179  : 				
; 180  : 			else if (m_nVert < 0)

	jmp	SHORT $L74986
$L74983:
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jge	SHORT $L74986

; 181  : 				dYOffset = (double)-m_nVert * (sizeDest.cx - nX) / sizeDest.cx;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	neg	edx
	mov	DWORD PTR -56+[ebp], edx
	fild	DWORD PTR -56+[ebp]
	mov	eax, DWORD PTR _sizeDest$[ebp]
	sub	eax, DWORD PTR _nX$74978[ebp]
	mov	DWORD PTR -60+[ebp], eax
	fild	DWORD PTR -60+[ebp]
	fmulp	ST(1), ST(0)
	fidiv	DWORD PTR _sizeDest$[ebp]
	fstp	QWORD PTR _dYOffset$74982[ebp]
$L74986:

; 182  : 
; 183  : 			// shears +ve (right) or -ve (left)
; 184  : 			for (int nY = 0; nY < sizeDest.cy; nY++)

	mov	DWORD PTR _nY$74988[ebp], 0
	jmp	SHORT $L74989
$L74990:
	mov	ecx, DWORD PTR _nY$74988[ebp]
	add	ecx, 1
	mov	DWORD PTR _nY$74988[ebp], ecx
$L74989:
	mov	edx, DWORD PTR _nY$74988[ebp]
	cmp	edx, DWORD PTR _sizeDest$[ebp+4]
	jge	$L74991

; 186  : 				double dXOffset = 0;

	mov	DWORD PTR _dXOffset$74992[ebp], 0
	mov	DWORD PTR _dXOffset$74992[ebp+4], 0

; 187  : 
; 188  : 				// calc the offset to src X coord
; 189  : 				if (m_nHorz < 0)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	jge	SHORT $L74993

; 190  : 					dXOffset = (double)-m_nHorz * nY / sizeDest.cy;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	neg	edx
	mov	DWORD PTR -64+[ebp], edx
	fild	DWORD PTR -64+[ebp]
	fimul	DWORD PTR _nY$74988[ebp]
	fidiv	DWORD PTR _sizeDest$[ebp+4]
	fstp	QWORD PTR _dXOffset$74992[ebp]

; 191  : 				
; 192  : 				else if (m_nHorz > 0)

	jmp	SHORT $L74996
$L74993:
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	jle	SHORT $L74996

; 193  : 					dXOffset = (double)m_nHorz * (sizeDest.cy - nY) / sizeDest.cy;

	mov	ecx, DWORD PTR _this$[ebp]
	fild	DWORD PTR [ecx+8]
	mov	edx, DWORD PTR _sizeDest$[ebp+4]
	sub	edx, DWORD PTR _nY$74988[ebp]
	mov	DWORD PTR -68+[ebp], edx
	fild	DWORD PTR -68+[ebp]
	fmulp	ST(1), ST(0)
	fidiv	DWORD PTR _sizeDest$[ebp+4]
	fstp	QWORD PTR _dXOffset$74992[ebp]
$L74996:

; 194  : 
; 195  : 				double dSrcX = nX - dXOffset;

	fild	DWORD PTR _nX$74978[ebp]
	fsub	QWORD PTR _dXOffset$74992[ebp]
	fstp	QWORD PTR _dSrcX$74998[ebp]

; 196  : 				double dSrcY = nY - dYOffset;

	fild	DWORD PTR _nY$74988[ebp]
	fsub	QWORD PTR _dYOffset$74982[ebp]
	fstp	QWORD PTR _dSrcY$74999[ebp]

; 197  : 
; 198  : 				if ((int)dSrcX >= 0 && (int)dSrcX < sizeSrc.cx && (int)dSrcY >= 0 && (int)dSrcY < sizeSrc.cy)

	fld	QWORD PTR _dSrcX$74998[ebp]
	call	__ftol
	test	eax, eax
	jl	$L75009
	fld	QWORD PTR _dSrcX$74998[ebp]
	call	__ftol
	cmp	eax, DWORD PTR _sizeSrc$[ebp]
	jge	$L75009
	fld	QWORD PTR _dSrcY$74999[ebp]
	call	__ftol
	test	eax, eax
	jl	$L75009
	fld	QWORD PTR _dSrcY$74999[ebp]
	call	__ftol
	cmp	eax, DWORD PTR _sizeSrc$[ebp+4]
	jge	SHORT $L75009

; 200  : 					if (!m_bWeightingEnabled)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L75005

; 202  : 						RGBX* pRGBSrc = &pSrcPixels[(int)dSrcY * sizeSrc.cx + (int)dSrcX];

	fld	QWORD PTR _dSrcY$74999[ebp]
	call	__ftol
	mov	esi, eax
	imul	esi, DWORD PTR _sizeSrc$[ebp]
	fld	QWORD PTR _dSrcX$74998[ebp]
	call	__ftol
	add	esi, eax
	mov	ecx, DWORD PTR _pSrcPixels$[ebp]
	lea	edx, DWORD PTR [ecx+esi*4]
	mov	DWORD PTR _pRGBSrc$75006[ebp], edx

; 203  : 						pDestPixels[nY * sizeDest.cx + nX] = *pRGBSrc;

	mov	eax, DWORD PTR _pRGBSrc$75006[ebp]
	push	eax
	mov	ecx, DWORD PTR _nY$74988[ebp]
	imul	ecx, DWORD PTR _sizeDest$[ebp]
	add	ecx, DWORD PTR _nX$74978[ebp]
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	lea	ecx, DWORD PTR [edx+ecx*4]
	call	??4RGBX@@QAEABV0@ABV0@@Z		; RGBX::operator=

; 205  : 					else

	jmp	SHORT $L75009
$L75005:

; 206  : 						CalcWeightedColor(pSrcPixels, sizeSrc, dSrcX, dSrcY, 
; 207  : 											pDestPixels[nY * sizeDest.cx + nX]);

	mov	eax, DWORD PTR _nY$74988[ebp]
	imul	eax, DWORD PTR _sizeDest$[ebp]
	add	eax, DWORD PTR _nX$74978[ebp]
	mov	ecx, DWORD PTR _pDestPixels$[ebp]
	lea	edx, DWORD PTR [ecx+eax*4]
	push	edx
	mov	eax, DWORD PTR _dSrcY$74999[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _dSrcY$74999[ebp]
	push	ecx
	mov	edx, DWORD PTR _dSrcX$74998[ebp+4]
	push	edx
	mov	eax, DWORD PTR _dSrcX$74998[ebp]
	push	eax
	mov	ecx, DWORD PTR _sizeSrc$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _sizeSrc$[ebp]
	push	edx
	mov	eax, DWORD PTR _pSrcPixels$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?CalcWeightedColor@C32BitImageProcessor@@QAEXPAVRGBX@@VCSize@@NNAAV2@@Z ; C32BitImageProcessor::CalcWeightedColor
$L75009:

; 209  : 			}

	jmp	$L74990
$L74991:

; 210  : 		}

	jmp	$L74980
$L74981:

; 212  : 
; 213  : 	return bRes;

	mov	eax, DWORD PTR _bRes$[ebp]

; 214  : }

	pop	edi
	pop	esi
	add	esp, 68					; 00000044H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?ProcessPixels@CImageShearer@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; CImageShearer::ProcessPixels
_TEXT	ENDS
PUBLIC	??0CImageGrayer@@QAE@XZ				; CImageGrayer::CImageGrayer
PUBLIC	?ProcessPixels@CImageGrayer@@UAEHPAVRGBX@@VCSize@@01K@Z ; CImageGrayer::ProcessPixels
PUBLIC	??_7CImageGrayer@@6B@				; CImageGrayer::`vftable'
PUBLIC	??_GCImageGrayer@@UAEPAXI@Z			; CImageGrayer::`scalar deleting destructor'
PUBLIC	??_ECImageGrayer@@UAEPAXI@Z			; CImageGrayer::`vector deleting destructor'
EXTRN	?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z:NEAR ; C32BitImageProcessor::CalcDestSize
;	COMDAT ??_7CImageGrayer@@6B@
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
??_7CImageGrayer@@6B@ DD FLAT:??_ECImageGrayer@@UAEPAXI@Z ; CImageGrayer::`vftable'
	DD	FLAT:?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z
	DD	FLAT:?ProcessPixels@CImageGrayer@@UAEHPAVRGBX@@VCSize@@01K@Z
CONST	ENDS
;	COMDAT ??0CImageGrayer@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0CImageGrayer@@QAE@XZ PROC NEAR			; CImageGrayer::CImageGrayer, COMDAT

; 220  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0C32BitImageProcessor@@QAE@H@Z	; C32BitImageProcessor::C32BitImageProcessor
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+8], 0
	mov	DWORD PTR [eax+12], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+16], 0
	mov	DWORD PTR [ecx+20], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+24], 0
	mov	DWORD PTR [edx+28], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+32], 1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CImageGrayer@@6B@ ; CImageGrayer::`vftable'

; 221  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CImageGrayer@@QAE@XZ ENDP				; CImageGrayer::CImageGrayer
_TEXT	ENDS
PUBLIC	??1CImageGrayer@@UAE@XZ				; CImageGrayer::~CImageGrayer
;	COMDAT ??_GCImageGrayer@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCImageGrayer@@UAEPAXI@Z PROC NEAR			; CImageGrayer::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CImageGrayer@@UAE@XZ			; CImageGrayer::~CImageGrayer
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75016
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75016:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCImageGrayer@@UAEPAXI@Z ENDP			; CImageGrayer::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??0CImageGrayer@@QAE@NNN@Z			; CImageGrayer::CImageGrayer
;	COMDAT ??0CImageGrayer@@QAE@NNN@Z
_TEXT	SEGMENT
_dRedFactor$ = 8
_dGreenFactor$ = 16
_dBlueFactor$ = 24
_this$ = -4
??0CImageGrayer@@QAE@NNN@Z PROC NEAR			; CImageGrayer::CImageGrayer, COMDAT

; 225  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0C32BitImageProcessor@@QAE@H@Z	; C32BitImageProcessor::C32BitImageProcessor
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _dRedFactor$[ebp]
	mov	DWORD PTR [eax+8], ecx
	mov	edx, DWORD PTR _dRedFactor$[ebp+4]
	mov	DWORD PTR [eax+12], edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _dGreenFactor$[ebp]
	mov	DWORD PTR [eax+16], ecx
	mov	edx, DWORD PTR _dGreenFactor$[ebp+4]
	mov	DWORD PTR [eax+20], edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _dBlueFactor$[ebp]
	mov	DWORD PTR [eax+24], ecx
	mov	edx, DWORD PTR _dBlueFactor$[ebp+4]
	mov	DWORD PTR [eax+28], edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+32], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CImageGrayer@@6B@ ; CImageGrayer::`vftable'

; 226  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	24					; 00000018H
??0CImageGrayer@@QAE@NNN@Z ENDP				; CImageGrayer::CImageGrayer
_TEXT	ENDS
;	COMDAT ??1CImageGrayer@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CImageGrayer@@UAE@XZ PROC NEAR			; CImageGrayer::~CImageGrayer, COMDAT

; 229  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageGrayer@@6B@ ; CImageGrayer::`vftable'

; 230  : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CImageGrayer@@UAE@XZ ENDP				; CImageGrayer::~CImageGrayer
_TEXT	ENDS
PUBLIC	??BRGBX@@QBEKXZ					; RGBX::operator unsigned long
PUBLIC	?MakeGray@RGBX@@QAEXABV1@@Z			; RGBX::MakeGray
PUBLIC	?MakeGray@RGBX@@QAEXABV1@NNN@Z			; RGBX::MakeGray
EXTRN	??8CSize@@QBEHUtagSIZE@@@Z:NEAR			; CSize::operator==
;	COMDAT ?ProcessPixels@CImageGrayer@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
$T76010 = -24
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_crMask$ = 32
_this$ = -28
_nX$ = -4
_nY$75043 = -8
_pRGBSrc$75047 = -16
_pRGBDest$75048 = -12
?ProcessPixels@CImageGrayer@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; CImageGrayer::ProcessPixels, COMDAT

; 234  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 28					; 0000001cH
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx
$L75034:

; 235  : 	UNREFERENCED_PARAMETER(sizeDest);
; 236  : 	ASSERT (sizeSrc == sizeDest);

	mov	eax, DWORD PTR _sizeDest$[ebp]
	mov	DWORD PTR $T76010[ebp], eax
	mov	ecx, DWORD PTR _sizeDest$[ebp+4]
	mov	DWORD PTR $T76010[ebp+4], ecx
	mov	edx, DWORD PTR $T76010[ebp+4]
	push	edx
	mov	eax, DWORD PTR $T76010[ebp]
	push	eax
	lea	ecx, DWORD PTR _sizeSrc$[ebp]
	call	??8CSize@@QBEHUtagSIZE@@@Z		; CSize::operator==
	test	eax, eax
	jne	SHORT $L75038
	push	236					; 000000ecH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75038
	int	3
$L75038:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75034

; 237  : 
; 238  : 	for (int nX = 0; nX < sizeSrc.cx; nX++)

	mov	DWORD PTR _nX$[ebp], 0
	jmp	SHORT $L75040
$L75041:
	mov	edx, DWORD PTR _nX$[ebp]
	add	edx, 1
	mov	DWORD PTR _nX$[ebp], edx
$L75040:
	mov	eax, DWORD PTR _nX$[ebp]
	cmp	eax, DWORD PTR _sizeSrc$[ebp]
	jge	$L75042

; 240  : 		for (int nY = 0; nY < sizeSrc.cy; nY++)

	mov	DWORD PTR _nY$75043[ebp], 0
	jmp	SHORT $L75044
$L75045:
	mov	ecx, DWORD PTR _nY$75043[ebp]
	add	ecx, 1
	mov	DWORD PTR _nY$75043[ebp], ecx
$L75044:
	mov	edx, DWORD PTR _nY$75043[ebp]
	cmp	edx, DWORD PTR _sizeSrc$[ebp+4]
	jge	$L75046

; 242  : 			RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];

	mov	eax, DWORD PTR _nY$75043[ebp]
	imul	eax, DWORD PTR _sizeSrc$[ebp]
	add	eax, DWORD PTR _nX$[ebp]
	mov	ecx, DWORD PTR _pSrcPixels$[ebp]
	lea	edx, DWORD PTR [ecx+eax*4]
	mov	DWORD PTR _pRGBSrc$75047[ebp], edx

; 243  : 			RGBX* pRGBDest = &pDestPixels[nY * sizeSrc.cx + nX];

	mov	eax, DWORD PTR _nY$75043[ebp]
	imul	eax, DWORD PTR _sizeSrc$[ebp]
	add	eax, DWORD PTR _nX$[ebp]
	mov	ecx, DWORD PTR _pDestPixels$[ebp]
	lea	edx, DWORD PTR [ecx+eax*4]
	mov	DWORD PTR _pRGBDest$75048[ebp], edx

; 244  : 
; 245  : 			if (crMask == -1 || !(crMask == *pRGBSrc))

	cmp	DWORD PTR _crMask$[ebp], -1
	je	SHORT $L75050
	mov	ecx, DWORD PTR _pRGBSrc$75047[ebp]
	call	??BRGBX@@QBEKXZ				; RGBX::operator unsigned long
	cmp	DWORD PTR _crMask$[ebp], eax
	je	SHORT $L75049
$L75050:

; 247  : 				if (m_bDefault)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+32], 0
	je	SHORT $L75051

; 248  : 					pRGBDest->MakeGray(*pRGBSrc);

	mov	ecx, DWORD PTR _pRGBSrc$75047[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pRGBDest$75048[ebp]
	call	?MakeGray@RGBX@@QAEXABV1@@Z		; RGBX::MakeGray

; 249  : 				else

	jmp	SHORT $L75052
$L75051:

; 250  : 					pRGBDest->MakeGray(*pRGBSrc, m_dRedFactor, m_dGreenFactor, m_dBlueFactor);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+28]
	push	eax
	mov	ecx, DWORD PTR [edx+24]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+20]
	push	eax
	mov	ecx, DWORD PTR [edx+16]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR [edx+8]
	push	ecx
	mov	edx, DWORD PTR _pRGBSrc$75047[ebp]
	push	edx
	mov	ecx, DWORD PTR _pRGBDest$75048[ebp]
	call	?MakeGray@RGBX@@QAEXABV1@NNN@Z		; RGBX::MakeGray
$L75052:

; 252  : 			else

	jmp	SHORT $L75053
$L75049:

; 253  : 				*pRGBDest = *pRGBSrc;

	mov	eax, DWORD PTR _pRGBSrc$75047[ebp]
	push	eax
	mov	ecx, DWORD PTR _pRGBDest$75048[ebp]
	call	??4RGBX@@QAEABV0@ABV0@@Z		; RGBX::operator=
$L75053:

; 254  : 		}

	jmp	$L75045
$L75046:

; 255  : 	}

	jmp	$L75041
$L75042:

; 256  : 
; 257  : 	return TRUE;

	mov	eax, 1

; 258  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?ProcessPixels@CImageGrayer@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; CImageGrayer::ProcessPixels
_TEXT	ENDS
;	COMDAT ??BRGBX@@QBEKXZ
_TEXT	SEGMENT
_this$ = -4
??BRGBX@@QBEKXZ PROC NEAR				; RGBX::operator unsigned long, COMDAT

; 45   : 	inline operator COLORREF() const { return RGB(btRed, btGreen, btBlue); }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+2]
	mov	eax, ecx
	mov	edx, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [edx+1]
	shl	ecx, 8
	or	eax, ecx
	mov	edx, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [edx]
	shl	ecx, 16					; 00000010H
	or	eax, ecx
	mov	esp, ebp
	pop	ebp
	ret	0
??BRGBX@@QBEKXZ ENDP					; RGBX::operator unsigned long
_TEXT	ENDS
PUBLIC	?Luminance@RGBX@@QBEHXZ				; RGBX::Luminance
;	COMDAT ?MakeGray@RGBX@@QAEXABV1@@Z
_TEXT	SEGMENT
_rgbSrc$ = 8
_this$ = -4
?MakeGray@RGBX@@QAEXABV1@@Z PROC NEAR			; RGBX::MakeGray, COMDAT

; 57   : 	inline void MakeGray(const RGBX& rgbSrc) { btRed = btGreen = btBlue = (BYTE)rgbSrc.Luminance(); }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _rgbSrc$[ebp]
	call	?Luminance@RGBX@@QBEHXZ			; RGBX::Luminance
	mov	ecx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [ecx], al
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	cl, BYTE PTR [eax]
	mov	BYTE PTR [edx+1], cl
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	cl, BYTE PTR [eax+1]
	mov	BYTE PTR [edx+2], cl
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?MakeGray@RGBX@@QAEXABV1@@Z ENDP			; RGBX::MakeGray
_TEXT	ENDS
;	COMDAT ?Luminance@RGBX@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?Luminance@RGBX@@QBEHXZ PROC NEAR			; RGBX::Luminance, COMDAT

; 47   : 	inline int Luminance() const { return (((int)btBlue + (int)btGreen * 6 + (int)btRed * 3) / 10); }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax]
	mov	eax, ecx
	mov	edx, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [edx+1]
	imul	ecx, 6
	add	eax, ecx
	mov	edx, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [edx+2]
	imul	ecx, 3
	add	eax, ecx
	cdq
	mov	ecx, 10					; 0000000aH
	idiv	ecx
	mov	esp, ebp
	pop	ebp
	ret	0
?Luminance@RGBX@@QBEHXZ ENDP				; RGBX::Luminance
_TEXT	ENDS
PUBLIC	?MakeGray@RGBX@@QAEXNNN@Z			; RGBX::MakeGray
;	COMDAT ?MakeGray@RGBX@@QAEXABV1@NNN@Z
_TEXT	SEGMENT
_rgbSrc$ = 8
_dRedFactor$ = 12
_dGreenFactor$ = 20
_dBlueFactor$ = 28
_this$ = -4
?MakeGray@RGBX@@QAEXABV1@NNN@Z PROC NEAR		; RGBX::MakeGray, COMDAT

; 60   : 	{ 

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 61   : 		*this = rgbSrc;

	mov	eax, DWORD PTR _rgbSrc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??4RGBX@@QAEABV0@ABV0@@Z		; RGBX::operator=

; 62   : 		MakeGray(dRedFactor, dGreenFactor, dBlueFactor);

	mov	ecx, DWORD PTR _dBlueFactor$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _dBlueFactor$[ebp]
	push	edx
	mov	eax, DWORD PTR _dGreenFactor$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _dGreenFactor$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dRedFactor$[ebp+4]
	push	edx
	mov	eax, DWORD PTR _dRedFactor$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?MakeGray@RGBX@@QAEXNNN@Z		; RGBX::MakeGray

; 63   : 	}

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?MakeGray@RGBX@@QAEXABV1@NNN@Z ENDP			; RGBX::MakeGray
_TEXT	ENDS
;	COMDAT ?MakeGray@RGBX@@QAEXNNN@Z
_TEXT	SEGMENT
_dRedFactor$ = 8
_dGreenFactor$ = 16
_dBlueFactor$ = 24
_this$ = -4
?MakeGray@RGBX@@QAEXNNN@Z PROC NEAR			; RGBX::MakeGray, COMDAT

; 66   : 	{ 

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 67   : 		btRed = btGreen = btBlue = (BYTE)((btBlue * dBlueFactor) +
; 68   : 									(btGreen * dGreenFactor) + 
; 69   : 									(btRed * dRedFactor)); 

	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax]
	mov	DWORD PTR -8+[ebp], ecx
	fild	DWORD PTR -8+[ebp]
	fmul	QWORD PTR _dBlueFactor$[ebp]
	mov	edx, DWORD PTR _this$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+1]
	mov	DWORD PTR -12+[ebp], eax
	fild	DWORD PTR -12+[ebp]
	fmul	QWORD PTR _dGreenFactor$[ebp]
	faddp	ST(1), ST(0)
	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+2]
	mov	DWORD PTR -16+[ebp], edx
	fild	DWORD PTR -16+[ebp]
	fmul	QWORD PTR _dRedFactor$[ebp]
	faddp	ST(1), ST(0)
	call	__ftol
	mov	ecx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [ecx], al
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	cl, BYTE PTR [eax]
	mov	BYTE PTR [edx+1], cl
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	cl, BYTE PTR [eax+1]
	mov	BYTE PTR [edx+2], cl

; 70   : 	}

	mov	esp, ebp
	pop	ebp
	ret	24					; 00000018H
?MakeGray@RGBX@@QAEXNNN@Z ENDP				; RGBX::MakeGray
_TEXT	ENDS
PUBLIC	??0CImageLightener@@QAE@N@Z			; CImageLightener::CImageLightener
PUBLIC	?ProcessPixels@CImageLightener@@UAEHPAVRGBX@@VCSize@@01K@Z ; CImageLightener::ProcessPixels
PUBLIC	??_7CImageLightener@@6B@			; CImageLightener::`vftable'
PUBLIC	??_GCImageLightener@@UAEPAXI@Z			; CImageLightener::`scalar deleting destructor'
PUBLIC	??_ECImageLightener@@UAEPAXI@Z			; CImageLightener::`vector deleting destructor'
;	COMDAT ??_7CImageLightener@@6B@
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
??_7CImageLightener@@6B@ DD FLAT:??_ECImageLightener@@UAEPAXI@Z ; CImageLightener::`vftable'
	DD	FLAT:?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z
	DD	FLAT:?ProcessPixels@CImageLightener@@UAEHPAVRGBX@@VCSize@@01K@Z
CONST	ENDS
;	COMDAT ??0CImageLightener@@QAE@N@Z
_TEXT	SEGMENT
_dAmount$ = 8
_this$ = -4
??0CImageLightener@@QAE@N@Z PROC NEAR			; CImageLightener::CImageLightener, COMDAT

; 263  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0C32BitImageProcessor@@QAE@H@Z	; C32BitImageProcessor::C32BitImageProcessor
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _dAmount$[ebp]
	mov	DWORD PTR [eax+8], ecx
	mov	edx, DWORD PTR _dAmount$[ebp+4]
	mov	DWORD PTR [eax+12], edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageLightener@@6B@ ; CImageLightener::`vftable'

; 264  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
??0CImageLightener@@QAE@N@Z ENDP			; CImageLightener::CImageLightener
_TEXT	ENDS
PUBLIC	??1CImageLightener@@UAE@XZ			; CImageLightener::~CImageLightener
;	COMDAT ??_GCImageLightener@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCImageLightener@@UAEPAXI@Z PROC NEAR		; CImageLightener::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CImageLightener@@UAE@XZ		; CImageLightener::~CImageLightener
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75061
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75061:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCImageLightener@@UAEPAXI@Z ENDP			; CImageLightener::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CImageLightener@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CImageLightener@@UAE@XZ PROC NEAR			; CImageLightener::~CImageLightener, COMDAT

; 267  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageLightener@@6B@ ; CImageLightener::`vftable'

; 268  : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CImageLightener@@UAE@XZ ENDP				; CImageLightener::~CImageLightener
_TEXT	ENDS
PUBLIC	__real@4@00000000000000000000
PUBLIC	__real@4@3fff8000000000000000
PUBLIC	??0HLSX@@QAE@XZ					; HLSX::HLSX
PUBLIC	?HLS2RGB@RGBX@@SAXABUHLSX@@AAV1@@Z		; RGBX::HLS2RGB
PUBLIC	?RGB2HLS@RGBX@@SAXABV1@AAUHLSX@@@Z		; RGBX::RGB2HLS
;	COMDAT __real@4@00000000000000000000
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
__real@4@00000000000000000000 DD 000000000r	; 0
CONST	ENDS
;	COMDAT __real@4@3fff8000000000000000
CONST	SEGMENT
__real@4@3fff8000000000000000 DD 03f800000r	; 1
CONST	ENDS
;	COMDAT ?ProcessPixels@CImageLightener@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
$T76031 = -40
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_crMask$ = 32
_this$ = -44
_bRes$ = -4
_nX$75081 = -8
_nY$75085 = -12
_pRGBSrc$75089 = -20
_pRGBDest$75090 = -16
_hls$75093 = -32
?ProcessPixels@CImageLightener@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; CImageLightener::ProcessPixels, COMDAT

; 272  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 72					; 00000048H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-72]
	mov	ecx, 18					; 00000012H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L75073:

; 273  : 	ASSERT (sizeSrc == sizeDest);

	mov	eax, DWORD PTR _sizeDest$[ebp]
	mov	DWORD PTR $T76031[ebp], eax
	mov	ecx, DWORD PTR _sizeDest$[ebp+4]
	mov	DWORD PTR $T76031[ebp+4], ecx
	mov	edx, DWORD PTR $T76031[ebp+4]
	push	edx
	mov	eax, DWORD PTR $T76031[ebp]
	push	eax
	lea	ecx, DWORD PTR _sizeSrc$[ebp]
	call	??8CSize@@QBEHUtagSIZE@@@Z		; CSize::operator==
	test	eax, eax
	jne	SHORT $L75077
	push	273					; 00000111H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75077
	int	3
$L75077:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75073

; 274  : 	BOOL bRes = TRUE;

	mov	DWORD PTR _bRes$[ebp], 1

; 275  : 
; 276  : 	if (m_dAmount == 0)

	mov	edx, DWORD PTR _this$[ebp]
	fld	QWORD PTR [edx+8]
	fcomp	QWORD PTR __real@8@00000000000000000000
	fnstsw	ax
	test	ah, 64					; 00000040H
	je	SHORT $L75079

; 277  : 		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	push	-1
	mov	eax, DWORD PTR _sizeDest$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sizeDest$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	push	edx
	mov	eax, DWORD PTR _sizeSrc$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sizeSrc$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pSrcPixels$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z ; C32BitImageProcessor::ProcessPixels
	mov	DWORD PTR _bRes$[ebp], eax

; 278  : 	else

	jmp	$L75084
$L75079:

; 280  : 		for (int nX = 0; nX < sizeSrc.cx; nX++)

	mov	DWORD PTR _nX$75081[ebp], 0
	jmp	SHORT $L75082
$L75083:
	mov	eax, DWORD PTR _nX$75081[ebp]
	add	eax, 1
	mov	DWORD PTR _nX$75081[ebp], eax
$L75082:
	mov	ecx, DWORD PTR _nX$75081[ebp]
	cmp	ecx, DWORD PTR _sizeSrc$[ebp]
	jge	$L75084

; 282  : 			for (int nY = 0; nY < sizeSrc.cy; nY++)

	mov	DWORD PTR _nY$75085[ebp], 0
	jmp	SHORT $L75086
$L75087:
	mov	edx, DWORD PTR _nY$75085[ebp]
	add	edx, 1
	mov	DWORD PTR _nY$75085[ebp], edx
$L75086:
	mov	eax, DWORD PTR _nY$75085[ebp]
	cmp	eax, DWORD PTR _sizeSrc$[ebp+4]
	jge	$L75088

; 284  : 				RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];

	mov	ecx, DWORD PTR _nY$75085[ebp]
	imul	ecx, DWORD PTR _sizeSrc$[ebp]
	add	ecx, DWORD PTR _nX$75081[ebp]
	mov	edx, DWORD PTR _pSrcPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGBSrc$75089[ebp], eax

; 285  : 				RGBX* pRGBDest = &pDestPixels[nY * sizeSrc.cx + nX];

	mov	ecx, DWORD PTR _nY$75085[ebp]
	imul	ecx, DWORD PTR _sizeSrc$[ebp]
	add	ecx, DWORD PTR _nX$75081[ebp]
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGBDest$75090[ebp], eax

; 286  : 
; 287  : 				if (crMask == -1 || !(crMask == *pRGBSrc))

	cmp	DWORD PTR _crMask$[ebp], -1
	je	SHORT $L75092
	mov	ecx, DWORD PTR _pRGBSrc$75089[ebp]
	call	??BRGBX@@QBEKXZ				; RGBX::operator unsigned long
	cmp	DWORD PTR _crMask$[ebp], eax
	je	$L75091
$L75092:

; 289  : 					// convert to HLS
; 290  : 					HLSX hls;

	lea	ecx, DWORD PTR _hls$75093[ebp]
	call	??0HLSX@@QAE@XZ				; HLSX::HLSX

; 291  : 					RGBX::RGB2HLS(*pRGBSrc, hls);

	lea	ecx, DWORD PTR _hls$75093[ebp]
	push	ecx
	mov	edx, DWORD PTR _pRGBSrc$75089[ebp]
	push	edx
	call	?RGB2HLS@RGBX@@SAXABV1@AAUHLSX@@@Z	; RGBX::RGB2HLS
	add	esp, 8

; 292  : 
; 293  : 					// then tweak the luminosity
; 294  : 					hls.fLuminosity = min(max(0.0f, hls.fLuminosity + (float)m_dAmount), 1.0f);

	mov	eax, DWORD PTR _this$[ebp]
	fld	QWORD PTR [eax+8]
	fst	DWORD PTR -48+[ebp]
	fadd	DWORD PTR _hls$75093[ebp+4]
	fcomp	DWORD PTR __real@4@00000000000000000000
	fnstsw	ax
	test	ah, 1
	je	SHORT $L76032
	mov	DWORD PTR -52+[ebp], 0
	jmp	SHORT $L76033
$L76032:
	mov	ecx, DWORD PTR _this$[ebp]
	fld	QWORD PTR [ecx+8]
	fst	DWORD PTR -56+[ebp]
	fadd	DWORD PTR _hls$75093[ebp+4]
	fstp	DWORD PTR -52+[ebp]
$L76033:
	fld	DWORD PTR -52+[ebp]
	fcomp	DWORD PTR __real@4@3fff8000000000000000
	fnstsw	ax
	test	ah, 1
	je	SHORT $L76036
	mov	edx, DWORD PTR _this$[ebp]
	fld	QWORD PTR [edx+8]
	fst	DWORD PTR -60+[ebp]
	fadd	DWORD PTR _hls$75093[ebp+4]
	fcomp	DWORD PTR __real@4@00000000000000000000
	fnstsw	ax
	test	ah, 1
	je	SHORT $L76034
	mov	DWORD PTR -64+[ebp], 0
	jmp	SHORT $L76035
$L76034:
	mov	eax, DWORD PTR _this$[ebp]
	fld	QWORD PTR [eax+8]
	fst	DWORD PTR -68+[ebp]
	fadd	DWORD PTR _hls$75093[ebp+4]
	fstp	DWORD PTR -64+[ebp]
$L76035:
	mov	ecx, DWORD PTR -64+[ebp]
	mov	DWORD PTR -72+[ebp], ecx
	jmp	SHORT $L76037
$L76036:
	mov	DWORD PTR -72+[ebp], 1065353216		; 3f800000H
$L76037:
	mov	edx, DWORD PTR -72+[ebp]
	mov	DWORD PTR _hls$75093[ebp+4], edx

; 295  : 					RGBX::HLS2RGB(hls, *pRGBDest);

	mov	eax, DWORD PTR _pRGBDest$75090[ebp]
	push	eax
	lea	ecx, DWORD PTR _hls$75093[ebp]
	push	ecx
	call	?HLS2RGB@RGBX@@SAXABUHLSX@@AAV1@@Z	; RGBX::HLS2RGB
	add	esp, 8

; 297  : 				else

	jmp	SHORT $L75098
$L75091:

; 298  : 					*pRGBDest = *pRGBSrc;

	mov	edx, DWORD PTR _pRGBSrc$75089[ebp]
	push	edx
	mov	ecx, DWORD PTR _pRGBDest$75090[ebp]
	call	??4RGBX@@QAEABV0@ABV0@@Z		; RGBX::operator=
$L75098:

; 299  : 			}

	jmp	$L75087
$L75088:

; 300  : 		}

	jmp	$L75083
$L75084:

; 302  : 
; 303  : 	return bRes;

	mov	eax, DWORD PTR _bRes$[ebp]

; 304  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 72					; 00000048H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?ProcessPixels@CImageLightener@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; CImageLightener::ProcessPixels
_TEXT	ENDS
;	COMDAT ??0HLSX@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0HLSX@@QAE@XZ PROC NEAR				; HLSX::HLSX, COMDAT

; 20   : 	HLSX() { fHue = fLuminosity = fSaturation = 0.0f; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+4], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	0
??0HLSX@@QAE@XZ ENDP					; HLSX::HLSX
_TEXT	ENDS
PUBLIC	__real@4@4006ff00000000000000
PUBLIC	__real@4@3ffe8000000000000000
PUBLIC	__real@4@40008000000000000000
PUBLIC	__real@4@4005f000000000000000
PUBLIC	?Hue2Triplet@RGBX@@KAEMMM@Z			; RGBX::Hue2Triplet
;	COMDAT __real@4@4006ff00000000000000
; File D:\_CODE\Shared\colordef.h
CONST	SEGMENT
__real@4@4006ff00000000000000 DD 0437f0000r	; 255
CONST	ENDS
;	COMDAT __real@4@3ffe8000000000000000
CONST	SEGMENT
__real@4@3ffe8000000000000000 DD 03f000000r	; 0.5
CONST	ENDS
;	COMDAT __real@4@40008000000000000000
CONST	SEGMENT
__real@4@40008000000000000000 DD 040000000r	; 2
CONST	ENDS
;	COMDAT __real@4@4005f000000000000000
CONST	SEGMENT
__real@4@4005f000000000000000 DD 042f00000r	; 120
CONST	ENDS
;	COMDAT ?HLS2RGB@RGBX@@SAXABUHLSX@@AAV1@@Z
_TEXT	SEGMENT
_hls$ = 8
_rgb$ = 12
_fT1$73190 = -4
_fT2$73191 = -8
?HLS2RGB@RGBX@@SAXABUHLSX@@AAV1@@Z PROC NEAR		; RGBX::HLS2RGB, COMDAT

; 93   : 	{

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 94   : 		if (!hls.fSaturation)

	mov	eax, DWORD PTR _hls$[ebp]
	fld	DWORD PTR [eax+8]
	fcomp	DWORD PTR __real@4@00000000000000000000
	fnstsw	ax
	test	ah, 64					; 00000040H
	je	SHORT $L73187

; 95   : 			rgb.btRed = rgb.btBlue = rgb.btGreen = (BYTE)(min(255.0f, hls.fLuminosity * 255));

	mov	ecx, DWORD PTR _hls$[ebp]
	fld	DWORD PTR [ecx+4]
	fmul	DWORD PTR __real@4@4006ff00000000000000
	fcomp	DWORD PTR __real@4@4006ff00000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L76046
	mov	DWORD PTR -12+[ebp], 1132396544		; 437f0000H
	jmp	SHORT $L76047
$L76046:
	mov	edx, DWORD PTR _hls$[ebp]
	fld	DWORD PTR [edx+4]
	fmul	DWORD PTR __real@4@4006ff00000000000000
	fstp	DWORD PTR -12+[ebp]
$L76047:
	fld	DWORD PTR -12+[ebp]
	call	__ftol
	mov	ecx, DWORD PTR _rgb$[ebp]
	mov	BYTE PTR [ecx+1], al
	mov	edx, DWORD PTR _rgb$[ebp]
	mov	eax, DWORD PTR _rgb$[ebp]
	mov	cl, BYTE PTR [eax+1]
	mov	BYTE PTR [edx], cl
	mov	edx, DWORD PTR _rgb$[ebp]
	mov	eax, DWORD PTR _rgb$[ebp]
	mov	cl, BYTE PTR [eax]
	mov	BYTE PTR [edx+2], cl

; 96   : 
; 97   : 		else

	jmp	$L73189
$L73187:

; 99   : 			float fT1, fT2;
; 100  : 
; 101  : 			if (hls.fLuminosity < 0.5f)

	mov	edx, DWORD PTR _hls$[ebp]
	fld	DWORD PTR [edx+4]
	fcomp	DWORD PTR __real@4@3ffe8000000000000000
	fnstsw	ax
	test	ah, 1
	je	SHORT $L73192

; 102  : 				fT2 = hls.fLuminosity * (1.0f + hls.fSaturation);

	mov	eax, DWORD PTR _hls$[ebp]
	fld	DWORD PTR __real@4@3fff8000000000000000
	fadd	DWORD PTR [eax+8]
	mov	ecx, DWORD PTR _hls$[ebp]
	fmul	DWORD PTR [ecx+4]
	fstp	DWORD PTR _fT2$73191[ebp]

; 103  : 			else

	jmp	SHORT $L73193
$L73192:

; 104  : 				fT2= (hls.fLuminosity + hls.fSaturation) - (hls.fLuminosity * hls.fSaturation);

	mov	edx, DWORD PTR _hls$[ebp]
	mov	eax, DWORD PTR _hls$[ebp]
	fld	DWORD PTR [edx+4]
	fadd	DWORD PTR [eax+8]
	mov	ecx, DWORD PTR _hls$[ebp]
	mov	edx, DWORD PTR _hls$[ebp]
	fld	DWORD PTR [ecx+4]
	fmul	DWORD PTR [edx+8]
	fsubp	ST(1), ST(0)
	fstp	DWORD PTR _fT2$73191[ebp]
$L73193:

; 105  : 
; 106  : 			fT1 = 2 * hls.fLuminosity - fT2;

	mov	eax, DWORD PTR _hls$[ebp]
	fld	DWORD PTR __real@4@40008000000000000000
	fmul	DWORD PTR [eax+4]
	fsub	DWORD PTR _fT2$73191[ebp]
	fstp	DWORD PTR _fT1$73190[ebp]

; 107  : 
; 108  : 			rgb.btRed = Hue2Triplet(fT1, fT2, hls.fHue + 120.0f);

	mov	ecx, DWORD PTR _hls$[ebp]
	fld	DWORD PTR [ecx]
	fadd	DWORD PTR __real@4@4005f000000000000000
	push	ecx
	fstp	DWORD PTR [esp]
	mov	edx, DWORD PTR _fT2$73191[ebp]
	push	edx
	mov	eax, DWORD PTR _fT1$73190[ebp]
	push	eax
	call	?Hue2Triplet@RGBX@@KAEMMM@Z		; RGBX::Hue2Triplet
	add	esp, 12					; 0000000cH
	mov	ecx, DWORD PTR _rgb$[ebp]
	mov	BYTE PTR [ecx+2], al

; 109  : 			rgb.btGreen = Hue2Triplet(fT1, fT2, hls.fHue);

	mov	edx, DWORD PTR _hls$[ebp]
	mov	eax, DWORD PTR [edx]
	push	eax
	mov	ecx, DWORD PTR _fT2$73191[ebp]
	push	ecx
	mov	edx, DWORD PTR _fT1$73190[ebp]
	push	edx
	call	?Hue2Triplet@RGBX@@KAEMMM@Z		; RGBX::Hue2Triplet
	add	esp, 12					; 0000000cH
	mov	ecx, DWORD PTR _rgb$[ebp]
	mov	BYTE PTR [ecx+1], al

; 110  : 			rgb.btBlue = Hue2Triplet(fT1, fT2, hls.fHue - 120.0f);

	mov	edx, DWORD PTR _hls$[ebp]
	fld	DWORD PTR [edx]
	fsub	DWORD PTR __real@4@4005f000000000000000
	push	ecx
	fstp	DWORD PTR [esp]
	mov	eax, DWORD PTR _fT2$73191[ebp]
	push	eax
	mov	ecx, DWORD PTR _fT1$73190[ebp]
	push	ecx
	call	?Hue2Triplet@RGBX@@KAEMMM@Z		; RGBX::Hue2Triplet
	add	esp, 12					; 0000000cH
	mov	edx, DWORD PTR _rgb$[ebp]
	mov	BYTE PTR [edx], al
$L73189:

; 112  : 	}

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?HLS2RGB@RGBX@@SAXABUHLSX@@AAV1@@Z ENDP			; RGBX::HLS2RGB
_TEXT	ENDS
PUBLIC	__real@4@4007ff00000000000000
PUBLIC	__real@4@4001c000000000000000
PUBLIC	__real@4@4004f000000000000000
PUBLIC	__real@4@40018000000000000000
PUBLIC	__real@4@4007b400000000000000
;	COMDAT __real@4@4007ff00000000000000
; File D:\_CODE\Shared\colordef.h
CONST	SEGMENT
__real@4@4007ff00000000000000 DD 043ff0000r	; 510
CONST	ENDS
;	COMDAT __real@4@4001c000000000000000
CONST	SEGMENT
__real@4@4001c000000000000000 DD 040c00000r	; 6
CONST	ENDS
;	COMDAT __real@4@4004f000000000000000
CONST	SEGMENT
__real@4@4004f000000000000000 DD 042700000r	; 60
CONST	ENDS
;	COMDAT __real@4@40018000000000000000
CONST	SEGMENT
__real@4@40018000000000000000 DD 040800000r	; 4
CONST	ENDS
;	COMDAT __real@4@4007b400000000000000
CONST	SEGMENT
__real@4@4007b400000000000000 DD 043b40000r	; 360
CONST	ENDS
;	COMDAT ?RGB2HLS@RGBX@@SAXABV1@AAUHLSX@@@Z
_TEXT	SEGMENT
_rgb$ = 8
_hls$ = 12
_nMax$ = -12
_nMin$ = -16
_fDiff$ = -4
_fSum$ = -8
_fRNorm$73205 = -28
_fGNorm$73206 = -20
_fBNorm$73207 = -24
?RGB2HLS@RGBX@@SAXABV1@AAUHLSX@@@Z PROC NEAR		; RGBX::RGB2HLS, COMDAT

; 115  : 	{

	push	ebp
	mov	ebp, esp
	sub	esp, 72					; 00000048H
	push	edi
	lea	edi, DWORD PTR [ebp-72]
	mov	ecx, 18					; 00000012H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 116  : 		int nMax = max(rgb.btRed, max(rgb.btGreen, rgb.btBlue));

	mov	eax, DWORD PTR _rgb$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+1]
	mov	edx, DWORD PTR _rgb$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx]
	cmp	ecx, eax
	jle	SHORT $L76058
	mov	ecx, DWORD PTR _rgb$[ebp]
	mov	dl, BYTE PTR [ecx+1]
	mov	BYTE PTR -32+[ebp], dl
	jmp	SHORT $L76059
$L76058:
	mov	eax, DWORD PTR _rgb$[ebp]
	mov	cl, BYTE PTR [eax]
	mov	BYTE PTR -32+[ebp], cl
$L76059:
	mov	edx, DWORD PTR _rgb$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+2]
	mov	ecx, DWORD PTR -32+[ebp]
	and	ecx, 255				; 000000ffH
	cmp	eax, ecx
	jle	SHORT $L76062
	mov	edx, DWORD PTR _rgb$[ebp]
	mov	al, BYTE PTR [edx+2]
	mov	BYTE PTR -36+[ebp], al
	jmp	SHORT $L76063
$L76062:
	mov	ecx, DWORD PTR _rgb$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+1]
	mov	eax, DWORD PTR _rgb$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax]
	cmp	edx, ecx
	jle	SHORT $L76060
	mov	edx, DWORD PTR _rgb$[ebp]
	mov	al, BYTE PTR [edx+1]
	mov	BYTE PTR -37+[ebp], al
	jmp	SHORT $L76061
$L76060:
	mov	ecx, DWORD PTR _rgb$[ebp]
	mov	dl, BYTE PTR [ecx]
	mov	BYTE PTR -37+[ebp], dl
$L76061:
	mov	al, BYTE PTR -37+[ebp]
	mov	BYTE PTR -36+[ebp], al
$L76063:
	mov	ecx, DWORD PTR -36+[ebp]
	and	ecx, 255				; 000000ffH
	mov	DWORD PTR _nMax$[ebp], ecx

; 117  : 		int nMin = min(rgb.btRed, min(rgb.btGreen, rgb.btBlue));

	mov	edx, DWORD PTR _rgb$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+1]
	mov	ecx, DWORD PTR _rgb$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx]
	cmp	eax, edx
	jge	SHORT $L76064
	mov	eax, DWORD PTR _rgb$[ebp]
	mov	cl, BYTE PTR [eax+1]
	mov	BYTE PTR -44+[ebp], cl
	jmp	SHORT $L76065
$L76064:
	mov	edx, DWORD PTR _rgb$[ebp]
	mov	al, BYTE PTR [edx]
	mov	BYTE PTR -44+[ebp], al
$L76065:
	mov	ecx, DWORD PTR _rgb$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+2]
	mov	eax, DWORD PTR -44+[ebp]
	and	eax, 255				; 000000ffH
	cmp	edx, eax
	jge	SHORT $L76068
	mov	ecx, DWORD PTR _rgb$[ebp]
	mov	dl, BYTE PTR [ecx+2]
	mov	BYTE PTR -48+[ebp], dl
	jmp	SHORT $L76069
$L76068:
	mov	eax, DWORD PTR _rgb$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+1]
	mov	edx, DWORD PTR _rgb$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx]
	cmp	ecx, eax
	jge	SHORT $L76066
	mov	ecx, DWORD PTR _rgb$[ebp]
	mov	dl, BYTE PTR [ecx+1]
	mov	BYTE PTR -49+[ebp], dl
	jmp	SHORT $L76067
$L76066:
	mov	eax, DWORD PTR _rgb$[ebp]
	mov	cl, BYTE PTR [eax]
	mov	BYTE PTR -49+[ebp], cl
$L76067:
	mov	dl, BYTE PTR -49+[ebp]
	mov	BYTE PTR -48+[ebp], dl
$L76069:
	mov	eax, DWORD PTR -48+[ebp]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR _nMin$[ebp], eax

; 118  : 
; 119  : 		float fDiff = (float)(nMax - nMin);

	mov	ecx, DWORD PTR _nMax$[ebp]
	sub	ecx, DWORD PTR _nMin$[ebp]
	mov	DWORD PTR -56+[ebp], ecx
	fild	DWORD PTR -56+[ebp]
	fstp	DWORD PTR _fDiff$[ebp]

; 120  : 		float fSum = (float)(nMax + nMin);

	mov	edx, DWORD PTR _nMax$[ebp]
	add	edx, DWORD PTR _nMin$[ebp]
	mov	DWORD PTR -60+[ebp], edx
	fild	DWORD PTR -60+[ebp]
	fst	DWORD PTR _fSum$[ebp]

; 121  : 			  
; 122  : 		// calculate luminosity
; 123  : 		hls.fLuminosity = fSum / 510.0f;

	fdiv	DWORD PTR __real@4@4007ff00000000000000
	mov	eax, DWORD PTR _hls$[ebp]
	fstp	DWORD PTR [eax+4]

; 124  : 
; 125  : 		if (nMax == nMin) // gray

	mov	ecx, DWORD PTR _nMax$[ebp]
	cmp	ecx, DWORD PTR _nMin$[ebp]
	jne	SHORT $L73201

; 127  : 			hls.fHue = hls.fSaturation = 0.0f;

	mov	edx, DWORD PTR _hls$[ebp]
	mov	DWORD PTR [edx+8], 0
	mov	eax, DWORD PTR _hls$[ebp]
	mov	DWORD PTR [eax], 0

; 129  : 		else

	jmp	$L73212
$L73201:

; 131  : 			// calculate Saturation
; 132  : 			if (hls.fLuminosity < 0.5f)

	mov	ecx, DWORD PTR _hls$[ebp]
	fld	DWORD PTR [ecx+4]
	fcomp	DWORD PTR __real@4@3ffe8000000000000000
	fnstsw	ax
	test	ah, 1
	je	SHORT $L73203

; 133  : 				hls.fSaturation = fDiff / fSum;

	fld	DWORD PTR _fDiff$[ebp]
	fdiv	DWORD PTR _fSum$[ebp]
	mov	edx, DWORD PTR _hls$[ebp]
	fstp	DWORD PTR [edx+8]

; 134  : 			else

	jmp	SHORT $L73204
$L73203:

; 135  : 				hls.fSaturation = fDiff / (510.0f - fSum);

	fld	DWORD PTR __real@4@4007ff00000000000000
	fsub	DWORD PTR _fSum$[ebp]
	fdivr	DWORD PTR _fDiff$[ebp]
	mov	eax, DWORD PTR _hls$[ebp]
	fstp	DWORD PTR [eax+8]
$L73204:

; 136  : 			
; 137  : 			// calculate Hue
; 138  : 			float fRNorm = (nMax - rgb.btRed) / fDiff;	  

	mov	ecx, DWORD PTR _rgb$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+2]
	mov	eax, DWORD PTR _nMax$[ebp]
	sub	eax, edx
	mov	DWORD PTR -64+[ebp], eax
	fild	DWORD PTR -64+[ebp]
	fdiv	DWORD PTR _fDiff$[ebp]
	fstp	DWORD PTR _fRNorm$73205[ebp]

; 139  : 			float fGNorm = (nMax - rgb.btGreen) / fDiff;

	mov	ecx, DWORD PTR _rgb$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+1]
	mov	eax, DWORD PTR _nMax$[ebp]
	sub	eax, edx
	mov	DWORD PTR -68+[ebp], eax
	fild	DWORD PTR -68+[ebp]
	fdiv	DWORD PTR _fDiff$[ebp]
	fstp	DWORD PTR _fGNorm$73206[ebp]

; 140  : 			float fBNorm = (nMax - rgb.btBlue) / fDiff;   

	mov	ecx, DWORD PTR _rgb$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx]
	mov	eax, DWORD PTR _nMax$[ebp]
	sub	eax, edx
	mov	DWORD PTR -72+[ebp], eax
	fild	DWORD PTR -72+[ebp]
	fdiv	DWORD PTR _fDiff$[ebp]
	fstp	DWORD PTR _fBNorm$73207[ebp]

; 141  : 			
; 142  : 			if (rgb.btRed == nMax)

	mov	ecx, DWORD PTR _rgb$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+2]
	cmp	edx, DWORD PTR _nMax$[ebp]
	jne	SHORT $L73208

; 143  : 				hls.fHue = 60.0f * (6.0f + fBNorm - fGNorm);

	fld	DWORD PTR __real@4@4001c000000000000000
	fadd	DWORD PTR _fBNorm$73207[ebp]
	fsub	DWORD PTR _fGNorm$73206[ebp]
	fmul	DWORD PTR __real@4@4004f000000000000000
	mov	eax, DWORD PTR _hls$[ebp]
	fstp	DWORD PTR [eax]

; 144  : 			
; 145  : 			else if (rgb.btGreen == nMax)

	jmp	SHORT $L73211
$L73208:
	mov	ecx, DWORD PTR _rgb$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+1]
	cmp	edx, DWORD PTR _nMax$[ebp]
	jne	SHORT $L73210

; 146  : 				hls.fHue = 60.0f * (2.0f + fRNorm - fBNorm);

	fld	DWORD PTR __real@4@40008000000000000000
	fadd	DWORD PTR _fRNorm$73205[ebp]
	fsub	DWORD PTR _fBNorm$73207[ebp]
	fmul	DWORD PTR __real@4@4004f000000000000000
	mov	eax, DWORD PTR _hls$[ebp]
	fstp	DWORD PTR [eax]

; 147  : 			
; 148  : 			else

	jmp	SHORT $L73211
$L73210:

; 149  : 				hls.fHue = 60.0f * (4.0f + fGNorm - fRNorm);

	fld	DWORD PTR __real@4@40018000000000000000
	fadd	DWORD PTR _fGNorm$73206[ebp]
	fsub	DWORD PTR _fRNorm$73205[ebp]
	fmul	DWORD PTR __real@4@4004f000000000000000
	mov	ecx, DWORD PTR _hls$[ebp]
	fstp	DWORD PTR [ecx]
$L73211:

; 150  : 			
; 151  : 			if (hls.fHue > 360.0f)

	mov	edx, DWORD PTR _hls$[ebp]
	fld	DWORD PTR [edx]
	fcomp	DWORD PTR __real@4@4007b400000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L73212

; 152  : 				hls.fHue -= 360.0f;

	mov	eax, DWORD PTR _hls$[ebp]
	fld	DWORD PTR [eax]
	fsub	DWORD PTR __real@4@4007b400000000000000
	mov	ecx, DWORD PTR _hls$[ebp]
	fstp	DWORD PTR [ecx]
$L73212:

; 154  : 	}

	pop	edi
	mov	esp, ebp
	pop	ebp
	ret	0
?RGB2HLS@RGBX@@SAXABV1@AAUHLSX@@@Z ENDP			; RGBX::RGB2HLS
_TEXT	ENDS
PUBLIC	__real@4@4006b400000000000000
PUBLIC	__real@4@4006f000000000000000
;	COMDAT __real@4@4006b400000000000000
; File D:\_CODE\Shared\colordef.h
CONST	SEGMENT
__real@4@4006b400000000000000 DD 043340000r	; 180
CONST	ENDS
;	COMDAT __real@4@4006f000000000000000
CONST	SEGMENT
__real@4@4006f000000000000000 DD 043700000r	; 240
CONST	ENDS
;	COMDAT ?Hue2Triplet@RGBX@@KAEMMM@Z
_TEXT	SEGMENT
_fT1$ = 8
_fT2$ = 12
_fHue$ = 16
?Hue2Triplet@RGBX@@KAEMMM@Z PROC NEAR			; RGBX::Hue2Triplet, COMDAT

; 158  : 	{

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 159  : 		if (fHue < 0)

	fld	DWORD PTR _fHue$[ebp]
	fcomp	DWORD PTR __real@4@00000000000000000000
	fnstsw	ax
	test	ah, 1
	je	SHORT $L73214

; 160  : 			fHue += 360.0f;

	fld	DWORD PTR _fHue$[ebp]
	fadd	DWORD PTR __real@4@4007b400000000000000
	fstp	DWORD PTR _fHue$[ebp]

; 161  : 
; 162  : 		else if (fHue > 360.0f)

	jmp	SHORT $L73216
$L73214:
	fld	DWORD PTR _fHue$[ebp]
	fcomp	DWORD PTR __real@4@4007b400000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L73216

; 163  : 			fHue -= 360.0f;

	fld	DWORD PTR _fHue$[ebp]
	fsub	DWORD PTR __real@4@4007b400000000000000
	fstp	DWORD PTR _fHue$[ebp]
$L73216:

; 164  : 
; 165  : 		if (fHue < 60.0f) 

	fld	DWORD PTR _fHue$[ebp]
	fcomp	DWORD PTR __real@4@4004f000000000000000
	fnstsw	ax
	test	ah, 1
	je	SHORT $L73217

; 166  : 			fT1 = fT1 + (fT2 - fT1) * fHue / 60.0f; 

	fld	DWORD PTR _fT2$[ebp]
	fsub	DWORD PTR _fT1$[ebp]
	fmul	DWORD PTR _fHue$[ebp]
	fdiv	DWORD PTR __real@4@4004f000000000000000
	fadd	DWORD PTR _fT1$[ebp]
	fstp	DWORD PTR _fT1$[ebp]

; 167  : 		
; 168  : 		else if (fHue < 180.0f) 

	jmp	SHORT $L73221
$L73217:
	fld	DWORD PTR _fHue$[ebp]
	fcomp	DWORD PTR __real@4@4006b400000000000000
	fnstsw	ax
	test	ah, 1
	je	SHORT $L73219

; 169  : 			fT1 = fT2;

	mov	eax, DWORD PTR _fT2$[ebp]
	mov	DWORD PTR _fT1$[ebp], eax

; 170  : 
; 171  : 		else if (fHue < 240.0f) 

	jmp	SHORT $L73221
$L73219:
	fld	DWORD PTR _fHue$[ebp]
	fcomp	DWORD PTR __real@4@4006f000000000000000
	fnstsw	ax
	test	ah, 1
	je	SHORT $L73221

; 172  : 			fT1 = fT1 + (fT2 - fT1) * (240.0f - fHue) / 60.0f;  

	fld	DWORD PTR _fT2$[ebp]
	fsub	DWORD PTR _fT1$[ebp]
	fld	DWORD PTR __real@4@4006f000000000000000
	fsub	DWORD PTR _fHue$[ebp]
	fmulp	ST(1), ST(0)
	fdiv	DWORD PTR __real@4@4004f000000000000000
	fadd	DWORD PTR _fT1$[ebp]
	fstp	DWORD PTR _fT1$[ebp]
$L73221:

; 173  : 		
; 174  : 		return (BYTE)min(fT1 * 255.0f, 255.0f);

	fld	DWORD PTR _fT1$[ebp]
	fmul	DWORD PTR __real@4@4006ff00000000000000
	fcomp	DWORD PTR __real@4@4006ff00000000000000
	fnstsw	ax
	test	ah, 1
	je	SHORT $L76082
	fld	DWORD PTR _fT1$[ebp]
	fmul	DWORD PTR __real@4@4006ff00000000000000
	fstp	DWORD PTR -4+[ebp]
	jmp	SHORT $L76083
$L76082:
	mov	DWORD PTR -4+[ebp], 1132396544		; 437f0000H
$L76083:
	fld	DWORD PTR -4+[ebp]
	call	__ftol

; 175  : 	}

	mov	esp, ebp
	pop	ebp
	ret	0
?Hue2Triplet@RGBX@@KAEMMM@Z ENDP			; RGBX::Hue2Triplet
_TEXT	ENDS
PUBLIC	??0CImageBlurrer@@QAE@H@Z			; CImageBlurrer::CImageBlurrer
PUBLIC	?ProcessPixels@CImageBlurrer@@UAEHPAVRGBX@@VCSize@@01K@Z ; CImageBlurrer::ProcessPixels
PUBLIC	??_7CImageBlurrer@@6B@				; CImageBlurrer::`vftable'
PUBLIC	??_GCImageBlurrer@@UAEPAXI@Z			; CImageBlurrer::`scalar deleting destructor'
PUBLIC	??_ECImageBlurrer@@UAEPAXI@Z			; CImageBlurrer::`vector deleting destructor'
;	COMDAT ??_7CImageBlurrer@@6B@
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
??_7CImageBlurrer@@6B@ DD FLAT:??_ECImageBlurrer@@UAEPAXI@Z ; CImageBlurrer::`vftable'
	DD	FLAT:?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z
	DD	FLAT:?ProcessPixels@CImageBlurrer@@UAEHPAVRGBX@@VCSize@@01K@Z
CONST	ENDS
;	COMDAT ??0CImageBlurrer@@QAE@H@Z
_TEXT	SEGMENT
_nAmount$ = 8
_this$ = -4
??0CImageBlurrer@@QAE@H@Z PROC NEAR			; CImageBlurrer::CImageBlurrer, COMDAT

; 309  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0C32BitImageProcessor@@QAE@H@Z	; C32BitImageProcessor::C32BitImageProcessor
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageBlurrer@@6B@ ; CImageBlurrer::`vftable'

; 310  : 	m_nAmount = max(0, min(100, nAmount));

	cmp	DWORD PTR _nAmount$[ebp], 100		; 00000064H
	jle	SHORT $L76090
	mov	DWORD PTR -8+[ebp], 100			; 00000064H
	jmp	SHORT $L76091
$L76090:
	mov	ecx, DWORD PTR _nAmount$[ebp]
	mov	DWORD PTR -8+[ebp], ecx
$L76091:
	cmp	DWORD PTR -8+[ebp], 0
	jge	SHORT $L76094
	mov	DWORD PTR -12+[ebp], 0
	jmp	SHORT $L76095
$L76094:
	cmp	DWORD PTR _nAmount$[ebp], 100		; 00000064H
	jle	SHORT $L76092
	mov	DWORD PTR -16+[ebp], 100		; 00000064H
	jmp	SHORT $L76093
$L76092:
	mov	edx, DWORD PTR _nAmount$[ebp]
	mov	DWORD PTR -16+[ebp], edx
$L76093:
	mov	eax, DWORD PTR -16+[ebp]
	mov	DWORD PTR -12+[ebp], eax
$L76095:
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR -12+[ebp]
	mov	DWORD PTR [ecx+8], edx

; 311  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0CImageBlurrer@@QAE@H@Z ENDP				; CImageBlurrer::CImageBlurrer
_TEXT	ENDS
PUBLIC	??1CImageBlurrer@@UAE@XZ			; CImageBlurrer::~CImageBlurrer
;	COMDAT ??_GCImageBlurrer@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCImageBlurrer@@UAEPAXI@Z PROC NEAR			; CImageBlurrer::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CImageBlurrer@@UAE@XZ		; CImageBlurrer::~CImageBlurrer
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75106
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75106:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCImageBlurrer@@UAEPAXI@Z ENDP			; CImageBlurrer::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CImageBlurrer@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CImageBlurrer@@UAE@XZ PROC NEAR			; CImageBlurrer::~CImageBlurrer, COMDAT

; 314  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageBlurrer@@6B@ ; CImageBlurrer::`vftable'

; 315  : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CImageBlurrer@@UAE@XZ ENDP				; CImageBlurrer::~CImageBlurrer
_TEXT	ENDS
PUBLIC	__real@8@3ffe8000000000000000
;	COMDAT __real@8@3ffe8000000000000000
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
__real@8@3ffe8000000000000000 DQ 03fe0000000000000r ; 0.5
CONST	ENDS
;	COMDAT ?ProcessPixels@CImageBlurrer@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
$T76104 = -76
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_this$ = -80
_bRes$ = -4
_nPeak$75126 = -24
_cMask$75128 = -16
_nX$75129 = -20
_nY$75133 = -28
_nRed$75137 = -44
_nGreen$75138 = -56
_nBlue$75139 = -52
_nSubCount$75140 = -40
_nDivisor$75141 = -32
_nSubX$75142 = -48
_nSubY$75146 = -60
_pRGBSub$75151 = -68
_nMask$75152 = -64
_pRGBDest$75153 = -36
?ProcessPixels@CImageBlurrer@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; CImageBlurrer::ProcessPixels, COMDAT

; 319  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 96					; 00000060H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-96]
	mov	ecx, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 320  : 	BOOL bRes = TRUE;

	mov	DWORD PTR _bRes$[ebp], 1

; 321  : 
; 322  : 	if (m_nAmount == 0)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	jne	SHORT $L75119

; 323  : 		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	push	-1
	mov	ecx, DWORD PTR _sizeDest$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _sizeDest$[ebp]
	push	edx
	mov	eax, DWORD PTR _pDestPixels$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sizeSrc$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _sizeSrc$[ebp]
	push	edx
	mov	eax, DWORD PTR _pSrcPixels$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z ; C32BitImageProcessor::ProcessPixels
	mov	DWORD PTR _bRes$[ebp], eax

; 324  : 	else

	jmp	$L75132
$L75119:

; 326  : 		ASSERT (sizeSrc == sizeDest);

	mov	ecx, DWORD PTR _sizeDest$[ebp]
	mov	DWORD PTR $T76104[ebp], ecx
	mov	edx, DWORD PTR _sizeDest$[ebp+4]
	mov	DWORD PTR $T76104[ebp+4], edx
	mov	eax, DWORD PTR $T76104[ebp+4]
	push	eax
	mov	ecx, DWORD PTR $T76104[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sizeSrc$[ebp]
	call	??8CSize@@QBEHUtagSIZE@@@Z		; CSize::operator==
	test	eax, eax
	jne	SHORT $L75125
	push	326					; 00000146H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75125
	int	3
$L75125:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75119

; 327  : 
; 328  : 		char nPeak = (char)(0.5 * (110 - m_nAmount));

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, 110				; 0000006eH
	sub	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR -84+[ebp], ecx
	fild	DWORD PTR -84+[ebp]
	fmul	QWORD PTR __real@8@3ffe8000000000000000
	call	__ftol
	mov	BYTE PTR _nPeak$75126[ebp], al

; 329  : 		char cMask[9] = { 1, 1, 1, 

	mov	BYTE PTR _cMask$75128[ebp], 1
	mov	BYTE PTR _cMask$75128[ebp+1], 1
	mov	BYTE PTR _cMask$75128[ebp+2], 1

; 330  : 						  1, nPeak, 1, 

	mov	BYTE PTR _cMask$75128[ebp+3], 1
	mov	dl, BYTE PTR _nPeak$75126[ebp]
	mov	BYTE PTR _cMask$75128[ebp+4], dl
	mov	BYTE PTR _cMask$75128[ebp+5], 1

; 331  : 						  1, 1, 1 };

	mov	BYTE PTR _cMask$75128[ebp+6], 1
	mov	BYTE PTR _cMask$75128[ebp+7], 1
	mov	BYTE PTR _cMask$75128[ebp+8], 1

; 332  : 
; 333  : 		for (int nX = 0; nX < sizeSrc.cx; nX++)

	mov	DWORD PTR _nX$75129[ebp], 0
	jmp	SHORT $L75130
$L75131:
	mov	eax, DWORD PTR _nX$75129[ebp]
	add	eax, 1
	mov	DWORD PTR _nX$75129[ebp], eax
$L75130:
	mov	ecx, DWORD PTR _nX$75129[ebp]
	cmp	ecx, DWORD PTR _sizeSrc$[ebp]
	jge	$L75132

; 335  : 			for (int nY = 0; nY < sizeSrc.cy; nY++)

	mov	DWORD PTR _nY$75133[ebp], 0
	jmp	SHORT $L75134
$L75135:
	mov	edx, DWORD PTR _nY$75133[ebp]
	add	edx, 1
	mov	DWORD PTR _nY$75133[ebp], edx
$L75134:
	mov	eax, DWORD PTR _nY$75133[ebp]
	cmp	eax, DWORD PTR _sizeSrc$[ebp+4]
	jge	$L75136

; 337  : 				int nRed = 0, nGreen = 0, nBlue = 0, nSubCount = 0, nDivisor = 0;

	mov	DWORD PTR _nRed$75137[ebp], 0
	mov	DWORD PTR _nGreen$75138[ebp], 0
	mov	DWORD PTR _nBlue$75139[ebp], 0
	mov	DWORD PTR _nSubCount$75140[ebp], 0
	mov	DWORD PTR _nDivisor$75141[ebp], 0

; 338  : 
; 339  : 				for (int nSubX = nX - 1; nSubX <= nX + 1; nSubX++)

	mov	ecx, DWORD PTR _nX$75129[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nSubX$75142[ebp], ecx
	jmp	SHORT $L75143
$L75144:
	mov	edx, DWORD PTR _nSubX$75142[ebp]
	add	edx, 1
	mov	DWORD PTR _nSubX$75142[ebp], edx
$L75143:
	mov	eax, DWORD PTR _nX$75129[ebp]
	add	eax, 1
	cmp	DWORD PTR _nSubX$75142[ebp], eax
	jg	$L75145

; 341  : 					for (int nSubY = nY - 1; nSubY <= nY + 1; nSubY++)

	mov	ecx, DWORD PTR _nY$75133[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nSubY$75146[ebp], ecx
	jmp	SHORT $L75147
$L75148:
	mov	edx, DWORD PTR _nSubY$75146[ebp]
	add	edx, 1
	mov	DWORD PTR _nSubY$75146[ebp], edx
$L75147:
	mov	eax, DWORD PTR _nY$75133[ebp]
	add	eax, 1
	cmp	DWORD PTR _nSubY$75146[ebp], eax
	jg	$L75149

; 343  : 						if (nSubX >= 0 && nSubX < sizeSrc.cx && nSubY >= 0 && nSubY < sizeSrc.cy)

	cmp	DWORD PTR _nSubX$75142[ebp], 0
	jl	SHORT $L75150
	mov	ecx, DWORD PTR _nSubX$75142[ebp]
	cmp	ecx, DWORD PTR _sizeSrc$[ebp]
	jge	SHORT $L75150
	cmp	DWORD PTR _nSubY$75146[ebp], 0
	jl	SHORT $L75150
	mov	edx, DWORD PTR _nSubY$75146[ebp]
	cmp	edx, DWORD PTR _sizeSrc$[ebp+4]
	jge	SHORT $L75150

; 345  : 							RGBX* pRGBSub = &pSrcPixels[nSubY * sizeSrc.cx + nSubX];

	mov	eax, DWORD PTR _nSubY$75146[ebp]
	imul	eax, DWORD PTR _sizeSrc$[ebp]
	add	eax, DWORD PTR _nSubX$75142[ebp]
	mov	ecx, DWORD PTR _pSrcPixels$[ebp]
	lea	edx, DWORD PTR [ecx+eax*4]
	mov	DWORD PTR _pRGBSub$75151[ebp], edx

; 346  : 
; 347  : 							int nMask = cMask[nSubCount];

	mov	eax, DWORD PTR _nSubCount$75140[ebp]
	movsx	ecx, BYTE PTR _cMask$75128[ebp+eax]
	mov	DWORD PTR _nMask$75152[ebp], ecx

; 348  : 
; 349  : 							nRed += pRGBSub->btRed * nMask;

	mov	edx, DWORD PTR _pRGBSub$75151[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+2]
	imul	eax, DWORD PTR _nMask$75152[ebp]
	mov	ecx, DWORD PTR _nRed$75137[ebp]
	add	ecx, eax
	mov	DWORD PTR _nRed$75137[ebp], ecx

; 350  : 							nGreen += pRGBSub->btGreen * nMask;

	mov	edx, DWORD PTR _pRGBSub$75151[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+1]
	imul	eax, DWORD PTR _nMask$75152[ebp]
	mov	ecx, DWORD PTR _nGreen$75138[ebp]
	add	ecx, eax
	mov	DWORD PTR _nGreen$75138[ebp], ecx

; 351  : 							nBlue += pRGBSub->btBlue * nMask;

	mov	edx, DWORD PTR _pRGBSub$75151[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx]
	imul	eax, DWORD PTR _nMask$75152[ebp]
	mov	ecx, DWORD PTR _nBlue$75139[ebp]
	add	ecx, eax
	mov	DWORD PTR _nBlue$75139[ebp], ecx

; 352  : 
; 353  : 							nDivisor += nMask;

	mov	edx, DWORD PTR _nDivisor$75141[ebp]
	add	edx, DWORD PTR _nMask$75152[ebp]
	mov	DWORD PTR _nDivisor$75141[ebp], edx
$L75150:

; 355  : 
; 356  : 						nSubCount++;

	mov	eax, DWORD PTR _nSubCount$75140[ebp]
	add	eax, 1
	mov	DWORD PTR _nSubCount$75140[ebp], eax

; 357  : 					}

	jmp	$L75148
$L75149:

; 358  : 				}

	jmp	$L75144
$L75145:

; 359  : 
; 360  : 				RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];

	mov	ecx, DWORD PTR _nY$75133[ebp]
	imul	ecx, DWORD PTR _sizeDest$[ebp]
	add	ecx, DWORD PTR _nX$75129[ebp]
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGBDest$75153[ebp], eax

; 361  : 
; 362  : 				pRGBDest->btRed = (BYTE)min(255, nRed / nDivisor);

	mov	eax, DWORD PTR _nRed$75137[ebp]
	cdq
	idiv	DWORD PTR _nDivisor$75141[ebp]
	cmp	eax, 255				; 000000ffH
	jle	SHORT $L76105
	mov	DWORD PTR -88+[ebp], 255		; 000000ffH
	jmp	SHORT $L76106
$L76105:
	mov	eax, DWORD PTR _nRed$75137[ebp]
	cdq
	idiv	DWORD PTR _nDivisor$75141[ebp]
	mov	DWORD PTR -88+[ebp], eax
$L76106:
	mov	ecx, DWORD PTR _pRGBDest$75153[ebp]
	mov	dl, BYTE PTR -88+[ebp]
	mov	BYTE PTR [ecx+2], dl

; 363  : 				pRGBDest->btGreen = (BYTE)min(255, nGreen / nDivisor);

	mov	eax, DWORD PTR _nGreen$75138[ebp]
	cdq
	idiv	DWORD PTR _nDivisor$75141[ebp]
	cmp	eax, 255				; 000000ffH
	jle	SHORT $L76107
	mov	DWORD PTR -92+[ebp], 255		; 000000ffH
	jmp	SHORT $L76108
$L76107:
	mov	eax, DWORD PTR _nGreen$75138[ebp]
	cdq
	idiv	DWORD PTR _nDivisor$75141[ebp]
	mov	DWORD PTR -92+[ebp], eax
$L76108:
	mov	eax, DWORD PTR _pRGBDest$75153[ebp]
	mov	cl, BYTE PTR -92+[ebp]
	mov	BYTE PTR [eax+1], cl

; 364  : 				pRGBDest->btBlue = (BYTE)min(255, nBlue / nDivisor);

	mov	eax, DWORD PTR _nBlue$75139[ebp]
	cdq
	idiv	DWORD PTR _nDivisor$75141[ebp]
	cmp	eax, 255				; 000000ffH
	jle	SHORT $L76109
	mov	DWORD PTR -96+[ebp], 255		; 000000ffH
	jmp	SHORT $L76110
$L76109:
	mov	eax, DWORD PTR _nBlue$75139[ebp]
	cdq
	idiv	DWORD PTR _nDivisor$75141[ebp]
	mov	DWORD PTR -96+[ebp], eax
$L76110:
	mov	edx, DWORD PTR _pRGBDest$75153[ebp]
	mov	al, BYTE PTR -96+[ebp]
	mov	BYTE PTR [edx], al

; 365  : 			}

	jmp	$L75135
$L75136:

; 366  : 		}

	jmp	$L75131
$L75132:

; 368  : 
; 369  : 	return TRUE;

	mov	eax, 1

; 370  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 96					; 00000060H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?ProcessPixels@CImageBlurrer@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; CImageBlurrer::ProcessPixels
_TEXT	ENDS
PUBLIC	??0CImageSharpener@@QAE@H@Z			; CImageSharpener::CImageSharpener
PUBLIC	?ProcessPixels@CImageSharpener@@UAEHPAVRGBX@@VCSize@@01K@Z ; CImageSharpener::ProcessPixels
PUBLIC	??_7CImageSharpener@@6B@			; CImageSharpener::`vftable'
PUBLIC	??_GCImageSharpener@@UAEPAXI@Z			; CImageSharpener::`scalar deleting destructor'
PUBLIC	??_ECImageSharpener@@UAEPAXI@Z			; CImageSharpener::`vector deleting destructor'
;	COMDAT ??_7CImageSharpener@@6B@
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
??_7CImageSharpener@@6B@ DD FLAT:??_ECImageSharpener@@UAEPAXI@Z ; CImageSharpener::`vftable'
	DD	FLAT:?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z
	DD	FLAT:?ProcessPixels@CImageSharpener@@UAEHPAVRGBX@@VCSize@@01K@Z
CONST	ENDS
;	COMDAT ??0CImageSharpener@@QAE@H@Z
_TEXT	SEGMENT
_nAmount$ = 8
_this$ = -4
??0CImageSharpener@@QAE@H@Z PROC NEAR			; CImageSharpener::CImageSharpener, COMDAT

; 375  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0C32BitImageProcessor@@QAE@H@Z	; C32BitImageProcessor::C32BitImageProcessor
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageSharpener@@6B@ ; CImageSharpener::`vftable'

; 376  : 	m_nAmount = max(0, min(100, nAmount));

	cmp	DWORD PTR _nAmount$[ebp], 100		; 00000064H
	jle	SHORT $L76115
	mov	DWORD PTR -8+[ebp], 100			; 00000064H
	jmp	SHORT $L76116
$L76115:
	mov	ecx, DWORD PTR _nAmount$[ebp]
	mov	DWORD PTR -8+[ebp], ecx
$L76116:
	cmp	DWORD PTR -8+[ebp], 0
	jge	SHORT $L76119
	mov	DWORD PTR -12+[ebp], 0
	jmp	SHORT $L76120
$L76119:
	cmp	DWORD PTR _nAmount$[ebp], 100		; 00000064H
	jle	SHORT $L76117
	mov	DWORD PTR -16+[ebp], 100		; 00000064H
	jmp	SHORT $L76118
$L76117:
	mov	edx, DWORD PTR _nAmount$[ebp]
	mov	DWORD PTR -16+[ebp], edx
$L76118:
	mov	eax, DWORD PTR -16+[ebp]
	mov	DWORD PTR -12+[ebp], eax
$L76120:
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR -12+[ebp]
	mov	DWORD PTR [ecx+8], edx

; 377  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0CImageSharpener@@QAE@H@Z ENDP			; CImageSharpener::CImageSharpener
_TEXT	ENDS
PUBLIC	??1CImageSharpener@@UAE@XZ			; CImageSharpener::~CImageSharpener
;	COMDAT ??_GCImageSharpener@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCImageSharpener@@UAEPAXI@Z PROC NEAR		; CImageSharpener::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CImageSharpener@@UAE@XZ		; CImageSharpener::~CImageSharpener
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75164
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75164:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCImageSharpener@@UAEPAXI@Z ENDP			; CImageSharpener::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CImageSharpener@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CImageSharpener@@UAE@XZ PROC NEAR			; CImageSharpener::~CImageSharpener, COMDAT

; 380  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageSharpener@@6B@ ; CImageSharpener::`vftable'

; 381  : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CImageSharpener@@UAE@XZ ENDP				; CImageSharpener::~CImageSharpener
_TEXT	ENDS
PUBLIC	__real@8@3fff8000000000000000
PUBLIC	__real@8@40018000000000000000
PUBLIC	__real@8@4006ff00000000000000
;	COMDAT __real@8@3fff8000000000000000
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
__real@8@3fff8000000000000000 DQ 03ff0000000000000r ; 1
CONST	ENDS
;	COMDAT __real@8@40018000000000000000
CONST	SEGMENT
__real@8@40018000000000000000 DQ 04010000000000000r ; 4
CONST	ENDS
;	COMDAT __real@8@4006ff00000000000000
CONST	SEGMENT
__real@8@4006ff00000000000000 DQ 0406fe00000000000r ; 255
CONST	ENDS
;	COMDAT ?ProcessPixels@CImageSharpener@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
$T76129 = -168
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_this$ = -172
_bRes$ = -4
_dMinMaxRatio$75184 = -20
_dMaxFactor$75186 = -12
_dMinFactor$75187 = -32
_dMask$75188 = -104
_nX$75189 = -24
_nY$75193 = -108
_dRed$75200 = -120
_dGreen$75201 = -128
_dBlue$75202 = -144
_nSubCount$75203 = -132
_nSubX$75204 = -136
_nSubY$75208 = -148
_pRGBSub$75212 = -160
_dVMask$75213 = -156
_pRGBDest$75214 = -112
?ProcessPixels@CImageSharpener@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; CImageSharpener::ProcessPixels, COMDAT

; 385  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 236				; 000000ecH
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-236]
	mov	ecx, 59					; 0000003bH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 386  : 	BOOL bRes = TRUE;

	mov	DWORD PTR _bRes$[ebp], 1

; 387  : 
; 388  : 	if (m_nAmount == 0)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	jne	SHORT $L75177

; 389  : 		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	push	-1
	mov	ecx, DWORD PTR _sizeDest$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _sizeDest$[ebp]
	push	edx
	mov	eax, DWORD PTR _pDestPixels$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sizeSrc$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _sizeSrc$[ebp]
	push	edx
	mov	eax, DWORD PTR _pSrcPixels$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z ; C32BitImageProcessor::ProcessPixels
	mov	DWORD PTR _bRes$[ebp], eax

; 390  : 	else

	jmp	$L75192
$L75177:

; 392  : 		ASSERT (sizeSrc == sizeDest);

	mov	ecx, DWORD PTR _sizeDest$[ebp]
	mov	DWORD PTR $T76129[ebp], ecx
	mov	edx, DWORD PTR _sizeDest$[ebp+4]
	mov	DWORD PTR $T76129[ebp+4], edx
	mov	eax, DWORD PTR $T76129[ebp+4]
	push	eax
	mov	ecx, DWORD PTR $T76129[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sizeSrc$[ebp]
	call	??8CSize@@QBEHUtagSIZE@@@Z		; CSize::operator==
	test	eax, eax
	jne	SHORT $L75183
	push	392					; 00000188H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75183
	int	3
$L75183:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75177

; 393  : 
; 394  : 		double dMinMaxRatio = (double)1 / (1 + (100 - m_nAmount) * 0.5);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, 100				; 00000064H
	sub	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR -176+[ebp], ecx
	fild	DWORD PTR -176+[ebp]
	fmul	QWORD PTR __real@8@3ffe8000000000000000
	fadd	QWORD PTR __real@8@3fff8000000000000000
	fdivr	QWORD PTR __real@8@3fff8000000000000000
	fstp	QWORD PTR _dMinMaxRatio$75184[ebp]

; 395  : 
; 396  : 		double dMaxFactor = 1 / (4 * (1 + dMinMaxRatio));

	fld	QWORD PTR __real@8@3fff8000000000000000
	fadd	QWORD PTR _dMinMaxRatio$75184[ebp]
	fmul	QWORD PTR __real@8@40018000000000000000
	fdivr	QWORD PTR __real@8@3fff8000000000000000
	fst	QWORD PTR _dMaxFactor$75186[ebp]

; 397  : 		double dMinFactor = dMaxFactor * dMinMaxRatio;

	fmul	QWORD PTR _dMinMaxRatio$75184[ebp]
	fst	QWORD PTR _dMinFactor$75187[ebp]

; 398  : 
; 399  : 		double dMask[9] = { -dMinFactor, -dMaxFactor, -dMinFactor, 

	fchs
	fstp	QWORD PTR _dMask$75188[ebp]
	fld	QWORD PTR _dMaxFactor$75186[ebp]
	fchs
	fstp	QWORD PTR _dMask$75188[ebp+8]
	fld	QWORD PTR _dMinFactor$75187[ebp]
	fchs
	fstp	QWORD PTR _dMask$75188[ebp+16]

; 400  : 							-dMaxFactor, 2, -dMaxFactor, 

	fld	QWORD PTR _dMaxFactor$75186[ebp]
	fchs
	fstp	QWORD PTR _dMask$75188[ebp+24]
	mov	DWORD PTR _dMask$75188[ebp+32], 0
	mov	DWORD PTR _dMask$75188[ebp+36], 1073741824 ; 40000000H
	fld	QWORD PTR _dMaxFactor$75186[ebp]
	fchs
	fstp	QWORD PTR _dMask$75188[ebp+40]

; 401  : 							-dMinFactor, -dMaxFactor, -dMinFactor };

	fld	QWORD PTR _dMinFactor$75187[ebp]
	fchs
	fstp	QWORD PTR _dMask$75188[ebp+48]
	fld	QWORD PTR _dMaxFactor$75186[ebp]
	fchs
	fstp	QWORD PTR _dMask$75188[ebp+56]
	fld	QWORD PTR _dMinFactor$75187[ebp]
	fchs
	fstp	QWORD PTR _dMask$75188[ebp+64]

; 402  : 
; 403  : 		for (int nX = 0; nX < sizeSrc.cx; nX++)

	mov	DWORD PTR _nX$75189[ebp], 0
	jmp	SHORT $L75190
$L75191:
	mov	edx, DWORD PTR _nX$75189[ebp]
	add	edx, 1
	mov	DWORD PTR _nX$75189[ebp], edx
$L75190:
	mov	eax, DWORD PTR _nX$75189[ebp]
	cmp	eax, DWORD PTR _sizeSrc$[ebp]
	jge	$L75192

; 405  : 			for (int nY = 0; nY < sizeSrc.cy; nY++)

	mov	DWORD PTR _nY$75193[ebp], 0
	jmp	SHORT $L75194
$L75195:
	mov	ecx, DWORD PTR _nY$75193[ebp]
	add	ecx, 1
	mov	DWORD PTR _nY$75193[ebp], ecx
$L75194:
	mov	edx, DWORD PTR _nY$75193[ebp]
	cmp	edx, DWORD PTR _sizeSrc$[ebp+4]
	jge	$L75196

; 407  : 				if (nX == 0 || nX == sizeSrc.cx - 1 || nY == 0 || nY == sizeSrc.cy - 1)

	cmp	DWORD PTR _nX$75189[ebp], 0
	je	SHORT $L75198
	mov	eax, DWORD PTR _sizeSrc$[ebp]
	sub	eax, 1
	cmp	DWORD PTR _nX$75189[ebp], eax
	je	SHORT $L75198
	cmp	DWORD PTR _nY$75193[ebp], 0
	je	SHORT $L75198
	mov	ecx, DWORD PTR _sizeSrc$[ebp+4]
	sub	ecx, 1
	cmp	DWORD PTR _nY$75193[ebp], ecx
	jne	SHORT $L75197
$L75198:

; 409  : 					pDestPixels[nY * sizeDest.cx + nX] = pSrcPixels[nY * sizeSrc.cx + nX];

	mov	edx, DWORD PTR _nY$75193[ebp]
	imul	edx, DWORD PTR _sizeSrc$[ebp]
	add	edx, DWORD PTR _nX$75189[ebp]
	mov	eax, DWORD PTR _pSrcPixels$[ebp]
	lea	ecx, DWORD PTR [eax+edx*4]
	push	ecx
	mov	edx, DWORD PTR _nY$75193[ebp]
	imul	edx, DWORD PTR _sizeDest$[ebp]
	add	edx, DWORD PTR _nX$75189[ebp]
	mov	eax, DWORD PTR _pDestPixels$[ebp]
	lea	ecx, DWORD PTR [eax+edx*4]
	call	??4RGBX@@QAEABV0@ABV0@@Z		; RGBX::operator=

; 411  : 				else

	jmp	$L75199
$L75197:

; 413  : 					double dRed = 0, dGreen = 0, dBlue = 0;

	mov	DWORD PTR _dRed$75200[ebp], 0
	mov	DWORD PTR _dRed$75200[ebp+4], 0
	mov	DWORD PTR _dGreen$75201[ebp], 0
	mov	DWORD PTR _dGreen$75201[ebp+4], 0
	mov	DWORD PTR _dBlue$75202[ebp], 0
	mov	DWORD PTR _dBlue$75202[ebp+4], 0

; 414  : 					int nSubCount = 0;

	mov	DWORD PTR _nSubCount$75203[ebp], 0

; 415  : 					
; 416  : 					for (int nSubX = nX - 1; nSubX <= nX + 1; nSubX++)

	mov	ecx, DWORD PTR _nX$75189[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nSubX$75204[ebp], ecx
	jmp	SHORT $L75205
$L75206:
	mov	edx, DWORD PTR _nSubX$75204[ebp]
	add	edx, 1
	mov	DWORD PTR _nSubX$75204[ebp], edx
$L75205:
	mov	eax, DWORD PTR _nX$75189[ebp]
	add	eax, 1
	cmp	DWORD PTR _nSubX$75204[ebp], eax
	jg	$L75207

; 418  : 						for (int nSubY = nY - 1; nSubY <= nY + 1; nSubY++)

	mov	ecx, DWORD PTR _nY$75193[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nSubY$75208[ebp], ecx
	jmp	SHORT $L75209
$L75210:
	mov	edx, DWORD PTR _nSubY$75208[ebp]
	add	edx, 1
	mov	DWORD PTR _nSubY$75208[ebp], edx
$L75209:
	mov	eax, DWORD PTR _nY$75193[ebp]
	add	eax, 1
	cmp	DWORD PTR _nSubY$75208[ebp], eax
	jg	$L75211

; 420  : 							RGBX* pRGBSub = &pSrcPixels[nSubY * sizeSrc.cx + nSubX];

	mov	ecx, DWORD PTR _nSubY$75208[ebp]
	imul	ecx, DWORD PTR _sizeSrc$[ebp]
	add	ecx, DWORD PTR _nSubX$75204[ebp]
	mov	edx, DWORD PTR _pSrcPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGBSub$75212[ebp], eax

; 421  : 
; 422  : 							double dVMask = dMask[nSubCount];

	mov	ecx, DWORD PTR _nSubCount$75203[ebp]
	mov	edx, DWORD PTR _dMask$75188[ebp+ecx*8]
	mov	DWORD PTR _dVMask$75213[ebp], edx
	mov	eax, DWORD PTR _dMask$75188[ebp+ecx*8+4]
	mov	DWORD PTR _dVMask$75213[ebp+4], eax

; 423  : 							
; 424  : 							dRed += pRGBSub->btRed * dVMask;

	mov	ecx, DWORD PTR _pRGBSub$75212[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+2]
	mov	DWORD PTR -180+[ebp], edx
	fild	DWORD PTR -180+[ebp]
	fmul	QWORD PTR _dVMask$75213[ebp]
	fadd	QWORD PTR _dRed$75200[ebp]
	fstp	QWORD PTR _dRed$75200[ebp]

; 425  : 							dGreen += pRGBSub->btGreen * dVMask;

	mov	eax, DWORD PTR _pRGBSub$75212[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+1]
	mov	DWORD PTR -184+[ebp], ecx
	fild	DWORD PTR -184+[ebp]
	fmul	QWORD PTR _dVMask$75213[ebp]
	fadd	QWORD PTR _dGreen$75201[ebp]
	fstp	QWORD PTR _dGreen$75201[ebp]

; 426  : 							dBlue += pRGBSub->btBlue * dVMask;

	mov	edx, DWORD PTR _pRGBSub$75212[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx]
	mov	DWORD PTR -188+[ebp], eax
	fild	DWORD PTR -188+[ebp]
	fmul	QWORD PTR _dVMask$75213[ebp]
	fadd	QWORD PTR _dBlue$75202[ebp]
	fstp	QWORD PTR _dBlue$75202[ebp]

; 427  : 							
; 428  : 							nSubCount++;

	mov	ecx, DWORD PTR _nSubCount$75203[ebp]
	add	ecx, 1
	mov	DWORD PTR _nSubCount$75203[ebp], ecx

; 429  : 						}

	jmp	$L75210
$L75211:

; 430  : 					}

	jmp	$L75206
$L75207:

; 431  : 
; 432  : 					RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];

	mov	edx, DWORD PTR _nY$75193[ebp]
	imul	edx, DWORD PTR _sizeDest$[ebp]
	add	edx, DWORD PTR _nX$75189[ebp]
	mov	eax, DWORD PTR _pDestPixels$[ebp]
	lea	ecx, DWORD PTR [eax+edx*4]
	mov	DWORD PTR _pRGBDest$75214[ebp], ecx

; 433  : 					
; 434  : 					dRed = min(255, dRed);

	fld	QWORD PTR __real@8@4006ff00000000000000
	fcomp	QWORD PTR _dRed$75200[ebp]
	fnstsw	ax
	test	ah, 1
	je	SHORT $L76130
	mov	DWORD PTR -196+[ebp], 0
	mov	DWORD PTR -196+[ebp+4], 1081073664	; 406fe000H
	jmp	SHORT $L76131
$L76130:
	mov	edx, DWORD PTR _dRed$75200[ebp]
	mov	DWORD PTR -196+[ebp], edx
	mov	eax, DWORD PTR _dRed$75200[ebp+4]
	mov	DWORD PTR -196+[ebp+4], eax
$L76131:
	mov	ecx, DWORD PTR -196+[ebp]
	mov	DWORD PTR _dRed$75200[ebp], ecx
	mov	edx, DWORD PTR -196+[ebp+4]
	mov	DWORD PTR _dRed$75200[ebp+4], edx

; 435  : 					dGreen = min(255, dGreen);

	fld	QWORD PTR __real@8@4006ff00000000000000
	fcomp	QWORD PTR _dGreen$75201[ebp]
	fnstsw	ax
	test	ah, 1
	je	SHORT $L76132
	mov	DWORD PTR -204+[ebp], 0
	mov	DWORD PTR -204+[ebp+4], 1081073664	; 406fe000H
	jmp	SHORT $L76133
$L76132:
	mov	eax, DWORD PTR _dGreen$75201[ebp]
	mov	DWORD PTR -204+[ebp], eax
	mov	ecx, DWORD PTR _dGreen$75201[ebp+4]
	mov	DWORD PTR -204+[ebp+4], ecx
$L76133:
	mov	edx, DWORD PTR -204+[ebp]
	mov	DWORD PTR _dGreen$75201[ebp], edx
	mov	eax, DWORD PTR -204+[ebp+4]
	mov	DWORD PTR _dGreen$75201[ebp+4], eax

; 436  : 					dBlue = min(255, dBlue);

	fld	QWORD PTR __real@8@4006ff00000000000000
	fcomp	QWORD PTR _dBlue$75202[ebp]
	fnstsw	ax
	test	ah, 1
	je	SHORT $L76134
	mov	DWORD PTR -212+[ebp], 0
	mov	DWORD PTR -212+[ebp+4], 1081073664	; 406fe000H
	jmp	SHORT $L76135
$L76134:
	mov	ecx, DWORD PTR _dBlue$75202[ebp]
	mov	DWORD PTR -212+[ebp], ecx
	mov	edx, DWORD PTR _dBlue$75202[ebp+4]
	mov	DWORD PTR -212+[ebp+4], edx
$L76135:
	mov	eax, DWORD PTR -212+[ebp]
	mov	DWORD PTR _dBlue$75202[ebp], eax
	mov	ecx, DWORD PTR -212+[ebp+4]
	mov	DWORD PTR _dBlue$75202[ebp+4], ecx

; 437  : 					dRed = max(0, dRed);

	fld	QWORD PTR __real@8@00000000000000000000
	fcomp	QWORD PTR _dRed$75200[ebp]
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L76136
	mov	DWORD PTR -220+[ebp], 0
	mov	DWORD PTR -220+[ebp+4], 0
	jmp	SHORT $L76137
$L76136:
	mov	edx, DWORD PTR _dRed$75200[ebp]
	mov	DWORD PTR -220+[ebp], edx
	mov	eax, DWORD PTR _dRed$75200[ebp+4]
	mov	DWORD PTR -220+[ebp+4], eax
$L76137:
	mov	ecx, DWORD PTR -220+[ebp]
	mov	DWORD PTR _dRed$75200[ebp], ecx
	mov	edx, DWORD PTR -220+[ebp+4]
	mov	DWORD PTR _dRed$75200[ebp+4], edx

; 438  : 					dGreen = max(0, dGreen);

	fld	QWORD PTR __real@8@00000000000000000000
	fcomp	QWORD PTR _dGreen$75201[ebp]
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L76138
	mov	DWORD PTR -228+[ebp], 0
	mov	DWORD PTR -228+[ebp+4], 0
	jmp	SHORT $L76139
$L76138:
	mov	eax, DWORD PTR _dGreen$75201[ebp]
	mov	DWORD PTR -228+[ebp], eax
	mov	ecx, DWORD PTR _dGreen$75201[ebp+4]
	mov	DWORD PTR -228+[ebp+4], ecx
$L76139:
	mov	edx, DWORD PTR -228+[ebp]
	mov	DWORD PTR _dGreen$75201[ebp], edx
	mov	eax, DWORD PTR -228+[ebp+4]
	mov	DWORD PTR _dGreen$75201[ebp+4], eax

; 439  : 					dBlue = max(0, dBlue);

	fld	QWORD PTR __real@8@00000000000000000000
	fcomp	QWORD PTR _dBlue$75202[ebp]
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L76140
	mov	DWORD PTR -236+[ebp], 0
	mov	DWORD PTR -236+[ebp+4], 0
	jmp	SHORT $L76141
$L76140:
	mov	ecx, DWORD PTR _dBlue$75202[ebp]
	mov	DWORD PTR -236+[ebp], ecx
	mov	edx, DWORD PTR _dBlue$75202[ebp+4]
	mov	DWORD PTR -236+[ebp+4], edx
$L76141:
	mov	eax, DWORD PTR -236+[ebp]
	mov	DWORD PTR _dBlue$75202[ebp], eax
	mov	ecx, DWORD PTR -236+[ebp+4]
	mov	DWORD PTR _dBlue$75202[ebp+4], ecx

; 440  : 
; 441  : 					pRGBDest->btRed = (BYTE)dRed;

	fld	QWORD PTR _dRed$75200[ebp]
	call	__ftol
	mov	edx, DWORD PTR _pRGBDest$75214[ebp]
	mov	BYTE PTR [edx+2], al

; 442  : 					pRGBDest->btGreen = (BYTE)dGreen;

	fld	QWORD PTR _dGreen$75201[ebp]
	call	__ftol
	mov	ecx, DWORD PTR _pRGBDest$75214[ebp]
	mov	BYTE PTR [ecx+1], al

; 443  : 					pRGBDest->btBlue = (BYTE)dBlue;

	fld	QWORD PTR _dBlue$75202[ebp]
	call	__ftol
	mov	edx, DWORD PTR _pRGBDest$75214[ebp]
	mov	BYTE PTR [edx], al
$L75199:

; 445  : 			}

	jmp	$L75195
$L75196:

; 446  : 		}

	jmp	$L75191
$L75192:

; 448  : 
; 449  : 	return TRUE;

	mov	eax, 1

; 450  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 236				; 000000ecH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?ProcessPixels@CImageSharpener@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; CImageSharpener::ProcessPixels
_TEXT	ENDS
PUBLIC	__real@8@40068000000000000000
PUBLIC	?ProcessPixels@CImageEmbosser@@UAEHPAVRGBX@@VCSize@@01K@Z ; CImageEmbosser::ProcessPixels
;	COMDAT __real@8@40068000000000000000
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
__real@8@40068000000000000000 DQ 04060000000000000r ; 128
CONST	ENDS
;	COMDAT ?ProcessPixels@CImageEmbosser@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
$T76150 = -144
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_crMask$ = 32
_this$ = -148
_dMask$ = -76
_nX$ = -4
_nY$75236 = -80
_pRGBDest$75243 = -84
_pRGBSrc$75244 = -88
_dRed$75247 = -96
_dGreen$75248 = -104
_dBlue$75249 = -120
_nSubCount$75250 = -108
_nSubX$75251 = -112
_nSubY$75255 = -124
_pRGBSub$75259 = -136
_dVMask$75260 = -132
?ProcessPixels@CImageEmbosser@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; CImageEmbosser::ProcessPixels, COMDAT

; 456  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 208				; 000000d0H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-208]
	mov	ecx, 52					; 00000034H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L75226:

; 457  : 	ASSERT (sizeSrc == sizeDest);

	mov	eax, DWORD PTR _sizeDest$[ebp]
	mov	DWORD PTR $T76150[ebp], eax
	mov	ecx, DWORD PTR _sizeDest$[ebp+4]
	mov	DWORD PTR $T76150[ebp+4], ecx
	mov	edx, DWORD PTR $T76150[ebp+4]
	push	edx
	mov	eax, DWORD PTR $T76150[ebp]
	push	eax
	lea	ecx, DWORD PTR _sizeSrc$[ebp]
	call	??8CSize@@QBEHUtagSIZE@@@Z		; CSize::operator==
	test	eax, eax
	jne	SHORT $L75230
	push	457					; 000001c9H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75230
	int	3
$L75230:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75226

; 458  : 	
; 459  : 	double dMask[9] = { -0.5, 0, 0, 

	mov	DWORD PTR _dMask$[ebp], 0
	mov	DWORD PTR _dMask$[ebp+4], -1075838976	; bfe00000H
	mov	DWORD PTR _dMask$[ebp+8], 0
	mov	DWORD PTR _dMask$[ebp+12], 0
	mov	DWORD PTR _dMask$[ebp+16], 0
	mov	DWORD PTR _dMask$[ebp+20], 0

; 460  : 						0, 1, 0, 

	mov	DWORD PTR _dMask$[ebp+24], 0
	mov	DWORD PTR _dMask$[ebp+28], 0
	mov	DWORD PTR _dMask$[ebp+32], 0
	mov	DWORD PTR _dMask$[ebp+36], 1072693248	; 3ff00000H
	mov	DWORD PTR _dMask$[ebp+40], 0
	mov	DWORD PTR _dMask$[ebp+44], 0

; 461  : 						0, 0, 0 };

	mov	DWORD PTR _dMask$[ebp+48], 0
	mov	DWORD PTR _dMask$[ebp+52], 0
	mov	DWORD PTR _dMask$[ebp+56], 0
	mov	DWORD PTR _dMask$[ebp+60], 0
	mov	DWORD PTR _dMask$[ebp+64], 0
	mov	DWORD PTR _dMask$[ebp+68], 0

; 462  : 	
; 463  : 	for (int nX = 0; nX < sizeSrc.cx; nX++)

	mov	DWORD PTR _nX$[ebp], 0
	jmp	SHORT $L75233
$L75234:
	mov	edx, DWORD PTR _nX$[ebp]
	add	edx, 1
	mov	DWORD PTR _nX$[ebp], edx
$L75233:
	mov	eax, DWORD PTR _nX$[ebp]
	cmp	eax, DWORD PTR _sizeSrc$[ebp]
	jge	$L75235

; 465  : 		for (int nY = 0; nY < sizeSrc.cy; nY++)

	mov	DWORD PTR _nY$75236[ebp], 0
	jmp	SHORT $L75237
$L75238:
	mov	ecx, DWORD PTR _nY$75236[ebp]
	add	ecx, 1
	mov	DWORD PTR _nY$75236[ebp], ecx
$L75237:
	mov	edx, DWORD PTR _nY$75236[ebp]
	cmp	edx, DWORD PTR _sizeSrc$[ebp+4]
	jge	$L75239

; 467  : 			if (nX == 0 || nX == sizeSrc.cx - 1 || nY == 0 || nY == sizeSrc.cy - 1)

	cmp	DWORD PTR _nX$[ebp], 0
	je	SHORT $L75241
	mov	eax, DWORD PTR _sizeSrc$[ebp]
	sub	eax, 1
	cmp	DWORD PTR _nX$[ebp], eax
	je	SHORT $L75241
	cmp	DWORD PTR _nY$75236[ebp], 0
	je	SHORT $L75241
	mov	ecx, DWORD PTR _sizeSrc$[ebp+4]
	sub	ecx, 1
	cmp	DWORD PTR _nY$75236[ebp], ecx
	jne	SHORT $L75240
$L75241:

; 469  : 				pDestPixels[nY * sizeDest.cx + nX] = pSrcPixels[nY * sizeSrc.cx + nX];

	mov	edx, DWORD PTR _nY$75236[ebp]
	imul	edx, DWORD PTR _sizeSrc$[ebp]
	add	edx, DWORD PTR _nX$[ebp]
	mov	eax, DWORD PTR _pSrcPixels$[ebp]
	lea	ecx, DWORD PTR [eax+edx*4]
	push	ecx
	mov	edx, DWORD PTR _nY$75236[ebp]
	imul	edx, DWORD PTR _sizeDest$[ebp]
	add	edx, DWORD PTR _nX$[ebp]
	mov	eax, DWORD PTR _pDestPixels$[ebp]
	lea	ecx, DWORD PTR [eax+edx*4]
	call	??4RGBX@@QAEABV0@ABV0@@Z		; RGBX::operator=

; 471  : 			else

	jmp	$L75265
$L75240:

; 473  : 				RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];

	mov	ecx, DWORD PTR _nY$75236[ebp]
	imul	ecx, DWORD PTR _sizeDest$[ebp]
	add	ecx, DWORD PTR _nX$[ebp]
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGBDest$75243[ebp], eax

; 474  : 				RGBX* pRGBSrc = &pSrcPixels[nY * sizeDest.cx + nX];

	mov	ecx, DWORD PTR _nY$75236[ebp]
	imul	ecx, DWORD PTR _sizeDest$[ebp]
	add	ecx, DWORD PTR _nX$[ebp]
	mov	edx, DWORD PTR _pSrcPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGBSrc$75244[ebp], eax

; 475  : 
; 476  : 				if (crMask == -1 || !(crMask == *pRGBSrc))

	cmp	DWORD PTR _crMask$[ebp], -1
	je	SHORT $L75246
	mov	ecx, DWORD PTR _pRGBSrc$75244[ebp]
	call	??BRGBX@@QBEKXZ				; RGBX::operator unsigned long
	cmp	DWORD PTR _crMask$[ebp], eax
	je	$L75245
$L75246:

; 478  : 					double dRed = 0, dGreen = 0, dBlue = 0;

	mov	DWORD PTR _dRed$75247[ebp], 0
	mov	DWORD PTR _dRed$75247[ebp+4], 0
	mov	DWORD PTR _dGreen$75248[ebp], 0
	mov	DWORD PTR _dGreen$75248[ebp+4], 0
	mov	DWORD PTR _dBlue$75249[ebp], 0
	mov	DWORD PTR _dBlue$75249[ebp+4], 0

; 479  : 					int nSubCount = 0;

	mov	DWORD PTR _nSubCount$75250[ebp], 0

; 480  : 					
; 481  : 					for (int nSubX = nX - 1; nSubX <= nX + 1; nSubX++)

	mov	ecx, DWORD PTR _nX$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nSubX$75251[ebp], ecx
	jmp	SHORT $L75252
$L75253:
	mov	edx, DWORD PTR _nSubX$75251[ebp]
	add	edx, 1
	mov	DWORD PTR _nSubX$75251[ebp], edx
$L75252:
	mov	eax, DWORD PTR _nX$[ebp]
	add	eax, 1
	cmp	DWORD PTR _nSubX$75251[ebp], eax
	jg	$L75254

; 483  : 						for (int nSubY = nY - 1; nSubY <= nY + 1; nSubY++)

	mov	ecx, DWORD PTR _nY$75236[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nSubY$75255[ebp], ecx
	jmp	SHORT $L75256
$L75257:
	mov	edx, DWORD PTR _nSubY$75255[ebp]
	add	edx, 1
	mov	DWORD PTR _nSubY$75255[ebp], edx
$L75256:
	mov	eax, DWORD PTR _nY$75236[ebp]
	add	eax, 1
	cmp	DWORD PTR _nSubY$75255[ebp], eax
	jg	$L75258

; 485  : 							RGBX* pRGBSub = &pSrcPixels[nSubY * sizeSrc.cx + nSubX];

	mov	ecx, DWORD PTR _nSubY$75255[ebp]
	imul	ecx, DWORD PTR _sizeSrc$[ebp]
	add	ecx, DWORD PTR _nSubX$75251[ebp]
	mov	edx, DWORD PTR _pSrcPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGBSub$75259[ebp], eax

; 486  : 						
; 487  : 							double dVMask = dMask[nSubCount];

	mov	ecx, DWORD PTR _nSubCount$75250[ebp]
	mov	edx, DWORD PTR _dMask$[ebp+ecx*8]
	mov	DWORD PTR _dVMask$75260[ebp], edx
	mov	eax, DWORD PTR _dMask$[ebp+ecx*8+4]
	mov	DWORD PTR _dVMask$75260[ebp+4], eax

; 488  : 
; 489  : 							if (dVMask != 0)

	fld	QWORD PTR _dVMask$75260[ebp]
	fcomp	QWORD PTR __real@8@00000000000000000000
	fnstsw	ax
	test	ah, 64					; 00000040H
	jne	SHORT $L75261

; 491  : 								dRed += pRGBSub->btRed * dVMask;

	mov	ecx, DWORD PTR _pRGBSub$75259[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+2]
	mov	DWORD PTR -152+[ebp], edx
	fild	DWORD PTR -152+[ebp]
	fmul	QWORD PTR _dVMask$75260[ebp]
	fadd	QWORD PTR _dRed$75247[ebp]
	fstp	QWORD PTR _dRed$75247[ebp]

; 492  : 								dGreen += pRGBSub->btGreen * dVMask;

	mov	eax, DWORD PTR _pRGBSub$75259[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+1]
	mov	DWORD PTR -156+[ebp], ecx
	fild	DWORD PTR -156+[ebp]
	fmul	QWORD PTR _dVMask$75260[ebp]
	fadd	QWORD PTR _dGreen$75248[ebp]
	fstp	QWORD PTR _dGreen$75248[ebp]

; 493  : 								dBlue += pRGBSub->btBlue * dVMask;

	mov	edx, DWORD PTR _pRGBSub$75259[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx]
	mov	DWORD PTR -160+[ebp], eax
	fild	DWORD PTR -160+[ebp]
	fmul	QWORD PTR _dVMask$75260[ebp]
	fadd	QWORD PTR _dBlue$75249[ebp]
	fstp	QWORD PTR _dBlue$75249[ebp]
$L75261:

; 495  : 							
; 496  : 							nSubCount++;

	mov	ecx, DWORD PTR _nSubCount$75250[ebp]
	add	ecx, 1
	mov	DWORD PTR _nSubCount$75250[ebp], ecx

; 497  : 						}

	jmp	$L75257
$L75258:

; 498  : 					}

	jmp	$L75253
$L75254:

; 499  : 				
; 500  : 					dRed = min(255, dRed + 128);

	fld	QWORD PTR _dRed$75247[ebp]
	fadd	QWORD PTR __real@8@40068000000000000000
	fcomp	QWORD PTR __real@8@4006ff00000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L76151
	mov	DWORD PTR -168+[ebp], 0
	mov	DWORD PTR -168+[ebp+4], 1081073664	; 406fe000H
	jmp	SHORT $L76152
$L76151:
	fld	QWORD PTR _dRed$75247[ebp]
	fadd	QWORD PTR __real@8@40068000000000000000
	fstp	QWORD PTR -168+[ebp]
$L76152:
	mov	edx, DWORD PTR -168+[ebp]
	mov	DWORD PTR _dRed$75247[ebp], edx
	mov	eax, DWORD PTR -168+[ebp+4]
	mov	DWORD PTR _dRed$75247[ebp+4], eax

; 501  : 					dGreen = min(255, dGreen + 128);

	fld	QWORD PTR _dGreen$75248[ebp]
	fadd	QWORD PTR __real@8@40068000000000000000
	fcomp	QWORD PTR __real@8@4006ff00000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L76153
	mov	DWORD PTR -176+[ebp], 0
	mov	DWORD PTR -176+[ebp+4], 1081073664	; 406fe000H
	jmp	SHORT $L76154
$L76153:
	fld	QWORD PTR _dGreen$75248[ebp]
	fadd	QWORD PTR __real@8@40068000000000000000
	fstp	QWORD PTR -176+[ebp]
$L76154:
	mov	ecx, DWORD PTR -176+[ebp]
	mov	DWORD PTR _dGreen$75248[ebp], ecx
	mov	edx, DWORD PTR -176+[ebp+4]
	mov	DWORD PTR _dGreen$75248[ebp+4], edx

; 502  : 					dBlue = min(255, dBlue + 128);

	fld	QWORD PTR _dBlue$75249[ebp]
	fadd	QWORD PTR __real@8@40068000000000000000
	fcomp	QWORD PTR __real@8@4006ff00000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L76155
	mov	DWORD PTR -184+[ebp], 0
	mov	DWORD PTR -184+[ebp+4], 1081073664	; 406fe000H
	jmp	SHORT $L76156
$L76155:
	fld	QWORD PTR _dBlue$75249[ebp]
	fadd	QWORD PTR __real@8@40068000000000000000
	fstp	QWORD PTR -184+[ebp]
$L76156:
	mov	eax, DWORD PTR -184+[ebp]
	mov	DWORD PTR _dBlue$75249[ebp], eax
	mov	ecx, DWORD PTR -184+[ebp+4]
	mov	DWORD PTR _dBlue$75249[ebp+4], ecx

; 503  : 					dRed = max(0, dRed);

	fld	QWORD PTR __real@8@00000000000000000000
	fcomp	QWORD PTR _dRed$75247[ebp]
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L76157
	mov	DWORD PTR -192+[ebp], 0
	mov	DWORD PTR -192+[ebp+4], 0
	jmp	SHORT $L76158
$L76157:
	mov	edx, DWORD PTR _dRed$75247[ebp]
	mov	DWORD PTR -192+[ebp], edx
	mov	eax, DWORD PTR _dRed$75247[ebp+4]
	mov	DWORD PTR -192+[ebp+4], eax
$L76158:
	mov	ecx, DWORD PTR -192+[ebp]
	mov	DWORD PTR _dRed$75247[ebp], ecx
	mov	edx, DWORD PTR -192+[ebp+4]
	mov	DWORD PTR _dRed$75247[ebp+4], edx

; 504  : 					dGreen = max(0, dGreen);

	fld	QWORD PTR __real@8@00000000000000000000
	fcomp	QWORD PTR _dGreen$75248[ebp]
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L76159
	mov	DWORD PTR -200+[ebp], 0
	mov	DWORD PTR -200+[ebp+4], 0
	jmp	SHORT $L76160
$L76159:
	mov	eax, DWORD PTR _dGreen$75248[ebp]
	mov	DWORD PTR -200+[ebp], eax
	mov	ecx, DWORD PTR _dGreen$75248[ebp+4]
	mov	DWORD PTR -200+[ebp+4], ecx
$L76160:
	mov	edx, DWORD PTR -200+[ebp]
	mov	DWORD PTR _dGreen$75248[ebp], edx
	mov	eax, DWORD PTR -200+[ebp+4]
	mov	DWORD PTR _dGreen$75248[ebp+4], eax

; 505  : 					dBlue = max(0, dBlue);

	fld	QWORD PTR __real@8@00000000000000000000
	fcomp	QWORD PTR _dBlue$75249[ebp]
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L76161
	mov	DWORD PTR -208+[ebp], 0
	mov	DWORD PTR -208+[ebp+4], 0
	jmp	SHORT $L76162
$L76161:
	mov	ecx, DWORD PTR _dBlue$75249[ebp]
	mov	DWORD PTR -208+[ebp], ecx
	mov	edx, DWORD PTR _dBlue$75249[ebp+4]
	mov	DWORD PTR -208+[ebp+4], edx
$L76162:
	mov	eax, DWORD PTR -208+[ebp]
	mov	DWORD PTR _dBlue$75249[ebp], eax
	mov	ecx, DWORD PTR -208+[ebp+4]
	mov	DWORD PTR _dBlue$75249[ebp+4], ecx

; 506  : 					
; 507  : 					pRGBDest->btRed = (BYTE)dRed;

	fld	QWORD PTR _dRed$75247[ebp]
	call	__ftol
	mov	edx, DWORD PTR _pRGBDest$75243[ebp]
	mov	BYTE PTR [edx+2], al

; 508  : 					pRGBDest->btGreen = (BYTE)dGreen;

	fld	QWORD PTR _dGreen$75248[ebp]
	call	__ftol
	mov	ecx, DWORD PTR _pRGBDest$75243[ebp]
	mov	BYTE PTR [ecx+1], al

; 509  : 					pRGBDest->btBlue = (BYTE)dBlue;

	fld	QWORD PTR _dBlue$75249[ebp]
	call	__ftol
	mov	edx, DWORD PTR _pRGBDest$75243[ebp]
	mov	BYTE PTR [edx], al

; 511  : 				else

	jmp	SHORT $L75265
$L75245:

; 513  : 					*pRGBDest = *pRGBSrc;

	mov	eax, DWORD PTR _pRGBSrc$75244[ebp]
	push	eax
	mov	ecx, DWORD PTR _pRGBDest$75243[ebp]
	call	??4RGBX@@QAEABV0@ABV0@@Z		; RGBX::operator=
$L75265:

; 516  : 		}

	jmp	$L75238
$L75239:

; 517  : 	}

	jmp	$L75234
$L75235:

; 518  : 
; 519  : 	return TRUE;

	mov	eax, 1

; 520  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 208				; 000000d0H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?ProcessPixels@CImageEmbosser@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; CImageEmbosser::ProcessPixels
_TEXT	ENDS
PUBLIC	??0CImageResizer@@QAE@N@Z			; CImageResizer::CImageResizer
PUBLIC	?CalcDestSize@CImageResizer@@UAE?AVCSize@@V2@@Z	; CImageResizer::CalcDestSize
PUBLIC	?ProcessPixels@CImageResizer@@UAEHPAVRGBX@@VCSize@@01K@Z ; CImageResizer::ProcessPixels
PUBLIC	??_7CImageResizer@@6B@				; CImageResizer::`vftable'
PUBLIC	??_GCImageResizer@@UAEPAXI@Z			; CImageResizer::`scalar deleting destructor'
PUBLIC	??_ECImageResizer@@UAEPAXI@Z			; CImageResizer::`vector deleting destructor'
;	COMDAT ??_7CImageResizer@@6B@
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
??_7CImageResizer@@6B@ DD FLAT:??_ECImageResizer@@UAEPAXI@Z ; CImageResizer::`vftable'
	DD	FLAT:?CalcDestSize@CImageResizer@@UAE?AVCSize@@V2@@Z
	DD	FLAT:?ProcessPixels@CImageResizer@@UAEHPAVRGBX@@VCSize@@01K@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76169	DD	019930520H
	DD	01H
	DD	FLAT:$T76171
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76171	DD	0ffffffffH
	DD	FLAT:$L76167
xdata$x	ENDS
;	COMDAT ??0CImageResizer@@QAE@N@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_dFactor$ = 8
_this$ = -16
??0CImageResizer@@QAE@N@Z PROC NEAR			; CImageResizer::CImageResizer, COMDAT

; 525  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76170
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0C32BitImageProcessor@@QAE@H@Z	; C32BitImageProcessor::C32BitImageProcessor
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _dFactor$[ebp]
	mov	DWORD PTR [eax+8], ecx
	mov	edx, DWORD PTR _dFactor$[ebp+4]
	mov	DWORD PTR [eax+12], edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageResizer@@6B@ ; CImageResizer::`vftable'
$L75270:

; 526  : 	ASSERT (m_dFactor > 0);

	mov	ecx, DWORD PTR _this$[ebp]
	fld	QWORD PTR [ecx+8]
	fcomp	QWORD PTR __real@8@00000000000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	je	SHORT $L75273
	push	526					; 0000020eH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75273
	int	3
$L75273:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75270

; 527  : 
; 528  : 	if (m_dFactor > 1)

	mov	eax, DWORD PTR _this$[ebp]
	fld	QWORD PTR [eax+8]
	fcomp	QWORD PTR __real@8@3fff8000000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L75274

; 529  : 		m_bWeightingEnabled = TRUE;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+4], 1
$L75274:

; 530  : }

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
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76167:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	ret	0
$L76170:
	mov	eax, OFFSET FLAT:$T76169
	jmp	___CxxFrameHandler
text$x	ENDS
??0CImageResizer@@QAE@N@Z ENDP				; CImageResizer::CImageResizer
PUBLIC	??1CImageResizer@@UAE@XZ			; CImageResizer::~CImageResizer
;	COMDAT ??_GCImageResizer@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCImageResizer@@UAEPAXI@Z PROC NEAR			; CImageResizer::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CImageResizer@@UAE@XZ		; CImageResizer::~CImageResizer
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75278
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75278:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCImageResizer@@UAEPAXI@Z ENDP			; CImageResizer::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CImageResizer@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CImageResizer@@UAE@XZ PROC NEAR			; CImageResizer::~CImageResizer, COMDAT

; 533  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageResizer@@6B@ ; CImageResizer::`vftable'

; 534  : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CImageResizer@@UAE@XZ ENDP				; CImageResizer::~CImageResizer
_TEXT	ENDS
;	COMDAT ?CalcDestSize@CImageResizer@@UAE?AVCSize@@V2@@Z
_TEXT	SEGMENT
_sizeSrc$ = 12
___$ReturnUdt$ = 8
_this$ = -4
?CalcDestSize@CImageResizer@@UAE?AVCSize@@V2@@Z PROC NEAR ; CImageResizer::CalcDestSize, COMDAT

; 537  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 538  : 	return CSize((int)(sizeSrc.cx * m_dFactor), (int)(sizeSrc.cy * m_dFactor));

	fild	DWORD PTR _sizeSrc$[ebp+4]
	mov	eax, DWORD PTR _this$[ebp]
	fmul	QWORD PTR [eax+8]
	call	__ftol
	push	eax
	fild	DWORD PTR _sizeSrc$[ebp]
	mov	ecx, DWORD PTR _this$[ebp]
	fmul	QWORD PTR [ecx+8]
	call	__ftol
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CSize@@QAE@HH@Z			; CSize::CSize
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 539  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?CalcDestSize@CImageResizer@@UAE?AVCSize@@V2@@Z ENDP	; CImageResizer::CalcDestSize
_TEXT	ENDS
PUBLIC	?Enlarge@CImageResizer@@IAEHPAVRGBX@@VCSize@@01@Z ; CImageResizer::Enlarge
PUBLIC	?Shrink@CImageResizer@@IAEHPAVRGBX@@VCSize@@01@Z ; CImageResizer::Shrink
;	COMDAT ?ProcessPixels@CImageResizer@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_this$ = -8
_bRes$ = -4
?ProcessPixels@CImageResizer@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; CImageResizer::ProcessPixels, COMDAT

; 543  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 544  : 	BOOL bRes = TRUE;

	mov	DWORD PTR _bRes$[ebp], 1

; 545  : 
; 546  : 	if (m_dFactor <= 0)

	mov	eax, DWORD PTR _this$[ebp]
	fld	QWORD PTR [eax+8]
	fcomp	QWORD PTR __real@8@00000000000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	je	SHORT $L75299

; 547  : 		return FALSE;

	xor	eax, eax
	jmp	$L75297
$L75299:

; 548  : 
; 549  : 	if (m_dFactor == 1)

	mov	ecx, DWORD PTR _this$[ebp]
	fld	QWORD PTR [ecx+8]
	fcomp	QWORD PTR __real@8@3fff8000000000000000
	fnstsw	ax
	test	ah, 64					; 00000040H
	je	SHORT $L75300

; 550  : 		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	push	-1
	mov	edx, DWORD PTR _sizeDest$[ebp+4]
	push	edx
	mov	eax, DWORD PTR _sizeDest$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDestPixels$[ebp]
	push	ecx
	mov	edx, DWORD PTR _sizeSrc$[ebp+4]
	push	edx
	mov	eax, DWORD PTR _sizeSrc$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pSrcPixels$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z ; C32BitImageProcessor::ProcessPixels
	mov	DWORD PTR _bRes$[ebp], eax

; 551  : 
; 552  : 	else if (m_dFactor > 1)

	jmp	SHORT $L75303
$L75300:
	mov	edx, DWORD PTR _this$[ebp]
	fld	QWORD PTR [edx+8]
	fcomp	QWORD PTR __real@8@3fff8000000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L75302

; 553  : 		bRes = Enlarge(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	mov	eax, DWORD PTR _sizeDest$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sizeDest$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	push	edx
	mov	eax, DWORD PTR _sizeSrc$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sizeSrc$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pSrcPixels$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Enlarge@CImageResizer@@IAEHPAVRGBX@@VCSize@@01@Z ; CImageResizer::Enlarge
	mov	DWORD PTR _bRes$[ebp], eax

; 554  : 
; 555  : 	else

	jmp	SHORT $L75303
$L75302:

; 556  : 		bRes = Shrink(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	mov	eax, DWORD PTR _sizeDest$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sizeDest$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	push	edx
	mov	eax, DWORD PTR _sizeSrc$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sizeSrc$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pSrcPixels$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Shrink@CImageResizer@@IAEHPAVRGBX@@VCSize@@01@Z ; CImageResizer::Shrink
	mov	DWORD PTR _bRes$[ebp], eax
$L75303:

; 557  : 
; 558  : 	return TRUE;

	mov	eax, 1
$L75297:

; 559  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?ProcessPixels@CImageResizer@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; CImageResizer::ProcessPixels
_TEXT	ENDS
;	COMDAT ?Enlarge@CImageResizer@@IAEHPAVRGBX@@VCSize@@01@Z
_TEXT	SEGMENT
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_this$ = -36
_dFactor$ = -8
_dXSrc$ = -20
_nX$ = -12
_dYSrc$75322 = -32
_nY$75323 = -24
?Enlarge@CImageResizer@@IAEHPAVRGBX@@VCSize@@01@Z PROC NEAR ; CImageResizer::Enlarge, COMDAT

; 562  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 36					; 00000024H
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
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx
$L75311:

; 563  : 	ASSERT (m_dFactor > 1);

	mov	eax, DWORD PTR _this$[ebp]
	fld	QWORD PTR [eax+8]
	fcomp	QWORD PTR __real@8@3fff8000000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	je	SHORT $L75314
	push	563					; 00000233H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75314
	int	3
$L75314:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75311

; 564  : 
; 565  : 	if (m_dFactor <= 1)

	mov	edx, DWORD PTR _this$[ebp]
	fld	QWORD PTR [edx+8]
	fcomp	QWORD PTR __real@8@3fff8000000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	je	SHORT $L75315

; 566  : 		return FALSE;

	xor	eax, eax
	jmp	$L75310
$L75315:

; 567  : 
; 568  : 	double dFactor = 1 / m_dFactor;

	mov	eax, DWORD PTR _this$[ebp]
	fld	QWORD PTR __real@8@3fff8000000000000000
	fdiv	QWORD PTR [eax+8]
	fstp	QWORD PTR _dFactor$[ebp]

; 569  : 	double dXSrc = 0;

	mov	DWORD PTR _dXSrc$[ebp], 0
	mov	DWORD PTR _dXSrc$[ebp+4], 0

; 570  : 
; 571  : 	for (int nX = 0; nX < sizeDest.cx; nX++)

	mov	DWORD PTR _nX$[ebp], 0
	jmp	SHORT $L75319
$L75320:
	mov	ecx, DWORD PTR _nX$[ebp]
	add	ecx, 1
	mov	DWORD PTR _nX$[ebp], ecx
$L75319:
	mov	edx, DWORD PTR _nX$[ebp]
	cmp	edx, DWORD PTR _sizeDest$[ebp]
	jge	SHORT $L75321

; 573  : 		double dYSrc = 0;

	mov	DWORD PTR _dYSrc$75322[ebp], 0
	mov	DWORD PTR _dYSrc$75322[ebp+4], 0

; 574  : 
; 575  : 		for (int nY = 0; nY < sizeDest.cy; nY++)

	mov	DWORD PTR _nY$75323[ebp], 0
	jmp	SHORT $L75324
$L75325:
	mov	eax, DWORD PTR _nY$75323[ebp]
	add	eax, 1
	mov	DWORD PTR _nY$75323[ebp], eax
$L75324:
	mov	ecx, DWORD PTR _nY$75323[ebp]
	cmp	ecx, DWORD PTR _sizeDest$[ebp+4]
	jge	SHORT $L75326

; 577  : 			CalcWeightedColor(pSrcPixels, sizeSrc, dXSrc, dYSrc, pDestPixels[nY * sizeDest.cx + nX]);

	mov	edx, DWORD PTR _nY$75323[ebp]
	imul	edx, DWORD PTR _sizeDest$[ebp]
	add	edx, DWORD PTR _nX$[ebp]
	mov	eax, DWORD PTR _pDestPixels$[ebp]
	lea	ecx, DWORD PTR [eax+edx*4]
	push	ecx
	mov	edx, DWORD PTR _dYSrc$75322[ebp+4]
	push	edx
	mov	eax, DWORD PTR _dYSrc$75322[ebp]
	push	eax
	mov	ecx, DWORD PTR _dXSrc$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _dXSrc$[ebp]
	push	edx
	mov	eax, DWORD PTR _sizeSrc$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sizeSrc$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pSrcPixels$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?CalcWeightedColor@C32BitImageProcessor@@QAEXPAVRGBX@@VCSize@@NNAAV2@@Z ; C32BitImageProcessor::CalcWeightedColor

; 578  : 
; 579  : 			dYSrc += dFactor; // next dest pixel in source coords

	fld	QWORD PTR _dYSrc$75322[ebp]
	fadd	QWORD PTR _dFactor$[ebp]
	fstp	QWORD PTR _dYSrc$75322[ebp]

; 580  : 		}

	jmp	SHORT $L75325
$L75326:

; 581  : 
; 582  : 		dXSrc += dFactor; // next dest pixel in source coords

	fld	QWORD PTR _dXSrc$[ebp]
	fadd	QWORD PTR _dFactor$[ebp]
	fstp	QWORD PTR _dXSrc$[ebp]

; 583  : 	}

	jmp	$L75320
$L75321:

; 584  : 
; 585  : 	return TRUE;

	mov	eax, 1
$L75310:

; 586  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	24					; 00000018H
?Enlarge@CImageResizer@@IAEHPAVRGBX@@VCSize@@01@Z ENDP	; CImageResizer::Enlarge
_TEXT	ENDS
;	COMDAT ?Shrink@CImageResizer@@IAEHPAVRGBX@@VCSize@@01@Z
_TEXT	SEGMENT
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_this$ = -84
_dFactor$ = -8
_dXEnd$ = -16
_nXStart$ = -28
_nXEnd$ = -24
_nX$ = -20
_nYStart$75349 = -32
_nYEnd$75350 = -48
_dYEnd$75351 = -40
_nY$75355 = -44
_nCount$75362 = -64
_nRed$75363 = -56
_nGreen$75364 = -72
_nBlue$75365 = -68
_nXSub$75366 = -60
_nYSub$75370 = -76
_pRGBSrc$75374 = -80
_pRGBDest$75375 = -52
?Shrink@CImageResizer@@IAEHPAVRGBX@@VCSize@@01@Z PROC NEAR ; CImageResizer::Shrink, COMDAT

; 589  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 92					; 0000005cH
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-92]
	mov	ecx, 23					; 00000017H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L75334:

; 590  : 	ASSERT (m_dFactor < 1 && m_dFactor > 0);

	mov	eax, DWORD PTR _this$[ebp]
	fld	QWORD PTR [eax+8]
	fcomp	QWORD PTR __real@8@3fff8000000000000000
	fnstsw	ax
	test	ah, 1
	je	SHORT $L75338
	mov	ecx, DWORD PTR _this$[ebp]
	fld	QWORD PTR [ecx+8]
	fcomp	QWORD PTR __real@8@00000000000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	je	SHORT $L75337
$L75338:
	push	590					; 0000024eH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75337
	int	3
$L75337:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75334

; 591  : 
; 592  : 	if (m_dFactor >= 1 || m_dFactor <= 0)

	mov	eax, DWORD PTR _this$[ebp]
	fld	QWORD PTR [eax+8]
	fcomp	QWORD PTR __real@8@3fff8000000000000000
	fnstsw	ax
	test	ah, 1
	je	SHORT $L75340
	mov	ecx, DWORD PTR _this$[ebp]
	fld	QWORD PTR [ecx+8]
	fcomp	QWORD PTR __real@8@00000000000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	je	SHORT $L75339
$L75340:

; 593  : 		return FALSE;

	xor	eax, eax
	jmp	$L75333
$L75339:

; 594  : 
; 595  : 	double dFactor = 1 / m_dFactor;

	mov	edx, DWORD PTR _this$[ebp]
	fld	QWORD PTR __real@8@3fff8000000000000000
	fdiv	QWORD PTR [edx+8]
	fst	QWORD PTR _dFactor$[ebp]

; 596  : 	double dXEnd = -dFactor / 2;

	fchs
	fdiv	QWORD PTR __real@8@40008000000000000000
	fstp	QWORD PTR _dXEnd$[ebp]

; 597  : 	int nXStart, nXEnd = -1;

	mov	DWORD PTR _nXEnd$[ebp], -1

; 598  : 
; 599  : 	for (int nX = 0; nX < sizeDest.cx; nX++)

	mov	DWORD PTR _nX$[ebp], 0
	jmp	SHORT $L75346
$L75347:
	mov	eax, DWORD PTR _nX$[ebp]
	add	eax, 1
	mov	DWORD PTR _nX$[ebp], eax
$L75346:
	mov	ecx, DWORD PTR _nX$[ebp]
	cmp	ecx, DWORD PTR _sizeDest$[ebp]
	jge	$L75348

; 601  : 		int nYStart, nYEnd = -1;

	mov	DWORD PTR _nYEnd$75350[ebp], -1

; 602  : 		double dYEnd = -dFactor / 2;

	fld	QWORD PTR _dFactor$[ebp]
	fchs
	fdiv	QWORD PTR __real@8@40008000000000000000
	fstp	QWORD PTR _dYEnd$75351[ebp]

; 603  : 
; 604  : 		nXStart = nXEnd + 1;

	mov	edx, DWORD PTR _nXEnd$[ebp]
	add	edx, 1
	mov	DWORD PTR _nXStart$[ebp], edx

; 605  : 		dXEnd += dFactor;

	fld	QWORD PTR _dXEnd$[ebp]
	fadd	QWORD PTR _dFactor$[ebp]
	fstp	QWORD PTR _dXEnd$[ebp]

; 606  : 		nXEnd = min(sizeSrc.cx - 1, (int)dXEnd + 1);

	mov	esi, DWORD PTR _sizeSrc$[ebp]
	sub	esi, 1
	fld	QWORD PTR _dXEnd$[ebp]
	call	__ftol
	add	eax, 1
	cmp	esi, eax
	jge	SHORT $L76187
	mov	eax, DWORD PTR _sizeSrc$[ebp]
	sub	eax, 1
	mov	DWORD PTR -88+[ebp], eax
	jmp	SHORT $L76188
$L76187:
	fld	QWORD PTR _dXEnd$[ebp]
	call	__ftol
	add	eax, 1
	mov	DWORD PTR -88+[ebp], eax
$L76188:
	mov	ecx, DWORD PTR -88+[ebp]
	mov	DWORD PTR _nXEnd$[ebp], ecx

; 607  : 
; 608  : 		if (nXStart > nXEnd)

	mov	edx, DWORD PTR _nXStart$[ebp]
	cmp	edx, DWORD PTR _nXEnd$[ebp]
	jle	SHORT $L75354

; 609  : 			continue;

	jmp	SHORT $L75347
$L75354:

; 610  : 
; 611  : 		for (int nY = 0; nY < sizeDest.cy; nY++)

	mov	DWORD PTR _nY$75355[ebp], 0
	jmp	SHORT $L75356
$L75357:
	mov	eax, DWORD PTR _nY$75355[ebp]
	add	eax, 1
	mov	DWORD PTR _nY$75355[ebp], eax
$L75356:
	mov	ecx, DWORD PTR _nY$75355[ebp]
	cmp	ecx, DWORD PTR _sizeDest$[ebp+4]
	jge	$L75358

; 613  : 			nYStart = nYEnd + 1;

	mov	edx, DWORD PTR _nYEnd$75350[ebp]
	add	edx, 1
	mov	DWORD PTR _nYStart$75349[ebp], edx

; 614  : 			dYEnd += dFactor;

	fld	QWORD PTR _dYEnd$75351[ebp]
	fadd	QWORD PTR _dFactor$[ebp]
	fstp	QWORD PTR _dYEnd$75351[ebp]

; 615  : 			nYEnd = min(sizeSrc.cy - 1, (int)dYEnd + 1);

	mov	esi, DWORD PTR _sizeSrc$[ebp+4]
	sub	esi, 1
	fld	QWORD PTR _dYEnd$75351[ebp]
	call	__ftol
	add	eax, 1
	cmp	esi, eax
	jge	SHORT $L76189
	mov	eax, DWORD PTR _sizeSrc$[ebp+4]
	sub	eax, 1
	mov	DWORD PTR -92+[ebp], eax
	jmp	SHORT $L76190
$L76189:
	fld	QWORD PTR _dYEnd$75351[ebp]
	call	__ftol
	add	eax, 1
	mov	DWORD PTR -92+[ebp], eax
$L76190:
	mov	ecx, DWORD PTR -92+[ebp]
	mov	DWORD PTR _nYEnd$75350[ebp], ecx

; 616  : 
; 617  : 			if (nYStart > nYEnd)

	mov	edx, DWORD PTR _nYStart$75349[ebp]
	cmp	edx, DWORD PTR _nYEnd$75350[ebp]
	jle	SHORT $L75361

; 618  : 				continue;

	jmp	SHORT $L75357
$L75361:

; 619  : 
; 620  : 			int nCount = 0, nRed = 0, nGreen = 0, nBlue = 0;

	mov	DWORD PTR _nCount$75362[ebp], 0
	mov	DWORD PTR _nRed$75363[ebp], 0
	mov	DWORD PTR _nGreen$75364[ebp], 0
	mov	DWORD PTR _nBlue$75365[ebp], 0

; 621  : 
; 622  : 			// average the pixels over the range
; 623  : 			for (int nXSub = nXStart; nXSub <= nXEnd; nXSub++)

	mov	eax, DWORD PTR _nXStart$[ebp]
	mov	DWORD PTR _nXSub$75366[ebp], eax
	jmp	SHORT $L75367
$L75368:
	mov	ecx, DWORD PTR _nXSub$75366[ebp]
	add	ecx, 1
	mov	DWORD PTR _nXSub$75366[ebp], ecx
$L75367:
	mov	edx, DWORD PTR _nXSub$75366[ebp]
	cmp	edx, DWORD PTR _nXEnd$[ebp]
	jg	SHORT $L75369

; 625  : 				for (int nYSub = nYStart; nYSub <= nYEnd; nYSub++)

	mov	eax, DWORD PTR _nYStart$75349[ebp]
	mov	DWORD PTR _nYSub$75370[ebp], eax
	jmp	SHORT $L75371
$L75372:
	mov	ecx, DWORD PTR _nYSub$75370[ebp]
	add	ecx, 1
	mov	DWORD PTR _nYSub$75370[ebp], ecx
$L75371:
	mov	edx, DWORD PTR _nYSub$75370[ebp]
	cmp	edx, DWORD PTR _nYEnd$75350[ebp]
	jg	SHORT $L75373

; 627  : 					RGBX* pRGBSrc = &pSrcPixels[nYSub * sizeSrc.cx + nXSub];

	mov	eax, DWORD PTR _nYSub$75370[ebp]
	imul	eax, DWORD PTR _sizeSrc$[ebp]
	add	eax, DWORD PTR _nXSub$75366[ebp]
	mov	ecx, DWORD PTR _pSrcPixels$[ebp]
	lea	edx, DWORD PTR [ecx+eax*4]
	mov	DWORD PTR _pRGBSrc$75374[ebp], edx

; 628  : 
; 629  : 					nRed += pRGBSrc->btRed;

	mov	eax, DWORD PTR _pRGBSrc$75374[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+2]
	mov	edx, DWORD PTR _nRed$75363[ebp]
	add	edx, ecx
	mov	DWORD PTR _nRed$75363[ebp], edx

; 630  : 					nGreen += pRGBSrc->btGreen;

	mov	eax, DWORD PTR _pRGBSrc$75374[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+1]
	mov	edx, DWORD PTR _nGreen$75364[ebp]
	add	edx, ecx
	mov	DWORD PTR _nGreen$75364[ebp], edx

; 631  : 					nBlue += pRGBSrc->btBlue;

	mov	eax, DWORD PTR _pRGBSrc$75374[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax]
	mov	edx, DWORD PTR _nBlue$75365[ebp]
	add	edx, ecx
	mov	DWORD PTR _nBlue$75365[ebp], edx

; 632  : 					nCount++;

	mov	eax, DWORD PTR _nCount$75362[ebp]
	add	eax, 1
	mov	DWORD PTR _nCount$75362[ebp], eax

; 633  : 				}

	jmp	SHORT $L75372
$L75373:

; 634  : 			}

	jmp	SHORT $L75368
$L75369:

; 635  : 
; 636  : 			RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];

	mov	ecx, DWORD PTR _nY$75355[ebp]
	imul	ecx, DWORD PTR _sizeDest$[ebp]
	add	ecx, DWORD PTR _nX$[ebp]
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGBDest$75375[ebp], eax

; 637  : 
; 638  : 			pRGBDest->btRed = (BYTE)(nRed / nCount);

	mov	eax, DWORD PTR _nRed$75363[ebp]
	cdq
	idiv	DWORD PTR _nCount$75362[ebp]
	mov	ecx, DWORD PTR _pRGBDest$75375[ebp]
	mov	BYTE PTR [ecx+2], al

; 639  : 			pRGBDest->btGreen = (BYTE)(nGreen / nCount);

	mov	eax, DWORD PTR _nGreen$75364[ebp]
	cdq
	idiv	DWORD PTR _nCount$75362[ebp]
	mov	edx, DWORD PTR _pRGBDest$75375[ebp]
	mov	BYTE PTR [edx+1], al

; 640  : 			pRGBDest->btBlue = (BYTE)(nBlue / nCount);

	mov	eax, DWORD PTR _nBlue$75365[ebp]
	cdq
	idiv	DWORD PTR _nCount$75362[ebp]
	mov	ecx, DWORD PTR _pRGBDest$75375[ebp]
	mov	BYTE PTR [ecx], al

; 641  : 		}

	jmp	$L75357
$L75358:

; 642  : 	}

	jmp	$L75347
$L75348:

; 643  : 
; 644  : 	return TRUE;

	mov	eax, 1
$L75333:

; 645  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 92					; 0000005cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	24					; 00000018H
?Shrink@CImageResizer@@IAEHPAVRGBX@@VCSize@@01@Z ENDP	; CImageResizer::Shrink
_TEXT	ENDS
PUBLIC	??0CImageNegator@@QAE@XZ			; CImageNegator::CImageNegator
PUBLIC	?ProcessPixels@CImageNegator@@UAEHPAVRGBX@@VCSize@@01K@Z ; CImageNegator::ProcessPixels
PUBLIC	??_7CImageNegator@@6B@				; CImageNegator::`vftable'
PUBLIC	??_GCImageNegator@@UAEPAXI@Z			; CImageNegator::`scalar deleting destructor'
PUBLIC	??_ECImageNegator@@UAEPAXI@Z			; CImageNegator::`vector deleting destructor'
;	COMDAT ??_7CImageNegator@@6B@
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
??_7CImageNegator@@6B@ DD FLAT:??_ECImageNegator@@UAEPAXI@Z ; CImageNegator::`vftable'
	DD	FLAT:?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z
	DD	FLAT:?ProcessPixels@CImageNegator@@UAEHPAVRGBX@@VCSize@@01K@Z
CONST	ENDS
;	COMDAT ??0CImageNegator@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0CImageNegator@@QAE@XZ PROC NEAR			; CImageNegator::CImageNegator, COMDAT

; 650  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0C32BitImageProcessor@@QAE@H@Z	; C32BitImageProcessor::C32BitImageProcessor
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageNegator@@6B@ ; CImageNegator::`vftable'

; 651  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CImageNegator@@QAE@XZ ENDP				; CImageNegator::CImageNegator
_TEXT	ENDS
PUBLIC	??1CImageNegator@@UAE@XZ			; CImageNegator::~CImageNegator
;	COMDAT ??_GCImageNegator@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCImageNegator@@UAEPAXI@Z PROC NEAR			; CImageNegator::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CImageNegator@@UAE@XZ		; CImageNegator::~CImageNegator
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75385
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75385:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCImageNegator@@UAEPAXI@Z ENDP			; CImageNegator::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CImageNegator@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CImageNegator@@UAE@XZ PROC NEAR			; CImageNegator::~CImageNegator, COMDAT

; 654  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageNegator@@6B@ ; CImageNegator::`vftable'

; 655  : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CImageNegator@@UAE@XZ ENDP				; CImageNegator::~CImageNegator
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageNegator@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_this$ = -20
_nX$ = -4
_nY$75401 = -8
_pRGBSrc$75405 = -16
_pRGBDest$75406 = -12
?ProcessPixels@CImageNegator@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; CImageNegator::ProcessPixels, COMDAT

; 659  : {

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

; 660  : 	for (int nX = 0; nX < sizeSrc.cx; nX++)

	mov	DWORD PTR _nX$[ebp], 0
	jmp	SHORT $L75398
$L75399:
	mov	eax, DWORD PTR _nX$[ebp]
	add	eax, 1
	mov	DWORD PTR _nX$[ebp], eax
$L75398:
	mov	ecx, DWORD PTR _nX$[ebp]
	cmp	ecx, DWORD PTR _sizeSrc$[ebp]
	jge	$L75400

; 662  : 		for (int nY = 0; nY < sizeSrc.cy; nY++)

	mov	DWORD PTR _nY$75401[ebp], 0
	jmp	SHORT $L75402
$L75403:
	mov	edx, DWORD PTR _nY$75401[ebp]
	add	edx, 1
	mov	DWORD PTR _nY$75401[ebp], edx
$L75402:
	mov	eax, DWORD PTR _nY$75401[ebp]
	cmp	eax, DWORD PTR _sizeSrc$[ebp+4]
	jge	SHORT $L75404

; 664  : 			RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];

	mov	ecx, DWORD PTR _nY$75401[ebp]
	imul	ecx, DWORD PTR _sizeSrc$[ebp]
	add	ecx, DWORD PTR _nX$[ebp]
	mov	edx, DWORD PTR _pSrcPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGBSrc$75405[ebp], eax

; 665  : 			RGBX* pRGBDest = &pDestPixels[nY * sizeSrc.cx + nX];

	mov	ecx, DWORD PTR _nY$75401[ebp]
	imul	ecx, DWORD PTR _sizeSrc$[ebp]
	add	ecx, DWORD PTR _nX$[ebp]
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGBDest$75406[ebp], eax

; 666  : 
; 667  : 			pRGBDest->btRed = (BYTE)(255 - pRGBSrc->btRed);

	mov	ecx, DWORD PTR _pRGBSrc$75405[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+2]
	mov	eax, 255				; 000000ffH
	sub	eax, edx
	mov	ecx, DWORD PTR _pRGBDest$75406[ebp]
	mov	BYTE PTR [ecx+2], al

; 668  : 			pRGBDest->btGreen = (BYTE)(255 - pRGBSrc->btGreen);

	mov	edx, DWORD PTR _pRGBSrc$75405[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+1]
	mov	ecx, 255				; 000000ffH
	sub	ecx, eax
	mov	edx, DWORD PTR _pRGBDest$75406[ebp]
	mov	BYTE PTR [edx+1], cl

; 669  : 			pRGBDest->btBlue = (BYTE)(255 - pRGBSrc->btBlue);

	mov	eax, DWORD PTR _pRGBSrc$75405[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax]
	mov	edx, 255				; 000000ffH
	sub	edx, ecx
	mov	eax, DWORD PTR _pRGBDest$75406[ebp]
	mov	BYTE PTR [eax], dl

; 670  : 		}

	jmp	SHORT $L75403
$L75404:

; 671  : 	}

	jmp	$L75399
$L75400:

; 672  : 
; 673  : 	return TRUE;

	mov	eax, 1

; 674  : }

	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?ProcessPixels@CImageNegator@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; CImageNegator::ProcessPixels
_TEXT	ENDS
PUBLIC	??0CImageFlipper@@QAE@HH@Z			; CImageFlipper::CImageFlipper
PUBLIC	?ProcessPixels@CImageFlipper@@UAEHPAVRGBX@@VCSize@@01K@Z ; CImageFlipper::ProcessPixels
PUBLIC	??_7CImageFlipper@@6B@				; CImageFlipper::`vftable'
PUBLIC	??_GCImageFlipper@@UAEPAXI@Z			; CImageFlipper::`scalar deleting destructor'
PUBLIC	??_ECImageFlipper@@UAEPAXI@Z			; CImageFlipper::`vector deleting destructor'
;	COMDAT ??_7CImageFlipper@@6B@
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
??_7CImageFlipper@@6B@ DD FLAT:??_ECImageFlipper@@UAEPAXI@Z ; CImageFlipper::`vftable'
	DD	FLAT:?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z
	DD	FLAT:?ProcessPixels@CImageFlipper@@UAEHPAVRGBX@@VCSize@@01K@Z
CONST	ENDS
;	COMDAT ??0CImageFlipper@@QAE@HH@Z
_TEXT	SEGMENT
_bHorz$ = 8
_bVert$ = 12
_this$ = -4
??0CImageFlipper@@QAE@HH@Z PROC NEAR			; CImageFlipper::CImageFlipper, COMDAT

; 679  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0C32BitImageProcessor@@QAE@H@Z	; C32BitImageProcessor::C32BitImageProcessor
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _bHorz$[ebp]
	mov	DWORD PTR [eax+8], ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _bVert$[ebp]
	mov	DWORD PTR [edx+12], eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CImageFlipper@@6B@ ; CImageFlipper::`vftable'

; 680  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
??0CImageFlipper@@QAE@HH@Z ENDP				; CImageFlipper::CImageFlipper
_TEXT	ENDS
PUBLIC	??1CImageFlipper@@UAE@XZ			; CImageFlipper::~CImageFlipper
;	COMDAT ??_GCImageFlipper@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCImageFlipper@@UAEPAXI@Z PROC NEAR			; CImageFlipper::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CImageFlipper@@UAE@XZ		; CImageFlipper::~CImageFlipper
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75418
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75418:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCImageFlipper@@UAEPAXI@Z ENDP			; CImageFlipper::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CImageFlipper@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CImageFlipper@@UAE@XZ PROC NEAR			; CImageFlipper::~CImageFlipper, COMDAT

; 683  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageFlipper@@6B@ ; CImageFlipper::`vftable'

; 684  : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CImageFlipper@@UAE@XZ ENDP				; CImageFlipper::~CImageFlipper
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageFlipper@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_this$ = -28
_nX$ = -4
_nDestX$75434 = -8
_nY$75435 = -12
_pRGBSrc$75439 = -24
_nDestY$75440 = -20
_pRGBDest$75441 = -16
?ProcessPixels@CImageFlipper@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; CImageFlipper::ProcessPixels, COMDAT

; 688  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 36					; 00000024H
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

; 689  : 	for (int nX = 0; nX < sizeSrc.cx; nX++)

	mov	DWORD PTR _nX$[ebp], 0
	jmp	SHORT $L75431
$L75432:
	mov	eax, DWORD PTR _nX$[ebp]
	add	eax, 1
	mov	DWORD PTR _nX$[ebp], eax
$L75431:
	mov	ecx, DWORD PTR _nX$[ebp]
	cmp	ecx, DWORD PTR _sizeSrc$[ebp]
	jge	$L75433

; 691  : 		int nDestX = m_bHorz ? sizeDest.cx - nX - 1 : nX;

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+8], 0
	je	SHORT $L76211
	mov	eax, DWORD PTR _sizeDest$[ebp]
	sub	eax, DWORD PTR _nX$[ebp]
	sub	eax, 1
	mov	DWORD PTR -32+[ebp], eax
	jmp	SHORT $L76212
$L76211:
	mov	ecx, DWORD PTR _nX$[ebp]
	mov	DWORD PTR -32+[ebp], ecx
$L76212:
	mov	edx, DWORD PTR -32+[ebp]
	mov	DWORD PTR _nDestX$75434[ebp], edx

; 692  : 
; 693  : 		for (int nY = 0; nY < sizeSrc.cy; nY++)

	mov	DWORD PTR _nY$75435[ebp], 0
	jmp	SHORT $L75436
$L75437:
	mov	eax, DWORD PTR _nY$75435[ebp]
	add	eax, 1
	mov	DWORD PTR _nY$75435[ebp], eax
$L75436:
	mov	ecx, DWORD PTR _nY$75435[ebp]
	cmp	ecx, DWORD PTR _sizeSrc$[ebp+4]
	jge	SHORT $L75438

; 695  : 			RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];

	mov	edx, DWORD PTR _nY$75435[ebp]
	imul	edx, DWORD PTR _sizeSrc$[ebp]
	add	edx, DWORD PTR _nX$[ebp]
	mov	eax, DWORD PTR _pSrcPixels$[ebp]
	lea	ecx, DWORD PTR [eax+edx*4]
	mov	DWORD PTR _pRGBSrc$75439[ebp], ecx

; 696  : 
; 697  : 			int nDestY = m_bVert ? sizeDest.cy - nY - 1 : nY;

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+12], 0
	je	SHORT $L76213
	mov	eax, DWORD PTR _sizeDest$[ebp+4]
	sub	eax, DWORD PTR _nY$75435[ebp]
	sub	eax, 1
	mov	DWORD PTR -36+[ebp], eax
	jmp	SHORT $L76214
$L76213:
	mov	ecx, DWORD PTR _nY$75435[ebp]
	mov	DWORD PTR -36+[ebp], ecx
$L76214:
	mov	edx, DWORD PTR -36+[ebp]
	mov	DWORD PTR _nDestY$75440[ebp], edx

; 698  : 			RGBX* pRGBDest = &pDestPixels[nDestY * sizeDest.cx + nDestX];

	mov	eax, DWORD PTR _nDestY$75440[ebp]
	imul	eax, DWORD PTR _sizeDest$[ebp]
	add	eax, DWORD PTR _nDestX$75434[ebp]
	mov	ecx, DWORD PTR _pDestPixels$[ebp]
	lea	edx, DWORD PTR [ecx+eax*4]
	mov	DWORD PTR _pRGBDest$75441[ebp], edx

; 699  : 
; 700  : 			*pRGBDest = *pRGBSrc;

	mov	eax, DWORD PTR _pRGBSrc$75439[ebp]
	push	eax
	mov	ecx, DWORD PTR _pRGBDest$75441[ebp]
	call	??4RGBX@@QAEABV0@ABV0@@Z		; RGBX::operator=

; 701  : 		}

	jmp	SHORT $L75437
$L75438:

; 702  : 	}

	jmp	$L75432
$L75433:

; 703  : 
; 704  : 	return TRUE;

	mov	eax, 1

; 705  : }

	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?ProcessPixels@CImageFlipper@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; CImageFlipper::ProcessPixels
_TEXT	ENDS
PUBLIC	??0CColorReplacer@@QAE@KK@Z			; CColorReplacer::CColorReplacer
PUBLIC	?ProcessPixels@CColorReplacer@@UAEHPAVRGBX@@VCSize@@01K@Z ; CColorReplacer::ProcessPixels
PUBLIC	??_7CColorReplacer@@6B@				; CColorReplacer::`vftable'
PUBLIC	??_GCColorReplacer@@UAEPAXI@Z			; CColorReplacer::`scalar deleting destructor'
PUBLIC	??_ECColorReplacer@@UAEPAXI@Z			; CColorReplacer::`vector deleting destructor'
;	COMDAT ??_7CColorReplacer@@6B@
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
??_7CColorReplacer@@6B@ DD FLAT:??_ECColorReplacer@@UAEPAXI@Z ; CColorReplacer::`vftable'
	DD	FLAT:?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z
	DD	FLAT:?ProcessPixels@CColorReplacer@@UAEHPAVRGBX@@VCSize@@01K@Z
CONST	ENDS
;	COMDAT ??0CColorReplacer@@QAE@KK@Z
_TEXT	SEGMENT
_crFrom$ = 8
_crTo$ = 12
_this$ = -4
??0CColorReplacer@@QAE@KK@Z PROC NEAR			; CColorReplacer::CColorReplacer, COMDAT

; 710  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0C32BitImageProcessor@@QAE@H@Z	; C32BitImageProcessor::C32BitImageProcessor
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _crFrom$[ebp]
	mov	DWORD PTR [eax+8], ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _crTo$[ebp]
	mov	DWORD PTR [edx+12], eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CColorReplacer@@6B@ ; CColorReplacer::`vftable'

; 711  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
??0CColorReplacer@@QAE@KK@Z ENDP			; CColorReplacer::CColorReplacer
_TEXT	ENDS
PUBLIC	??1CColorReplacer@@UAE@XZ			; CColorReplacer::~CColorReplacer
;	COMDAT ??_GCColorReplacer@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCColorReplacer@@UAEPAXI@Z PROC NEAR			; CColorReplacer::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CColorReplacer@@UAE@XZ		; CColorReplacer::~CColorReplacer
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75450
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75450:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCColorReplacer@@UAEPAXI@Z ENDP			; CColorReplacer::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CColorReplacer@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CColorReplacer@@UAE@XZ PROC NEAR			; CColorReplacer::~CColorReplacer, COMDAT

; 714  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CColorReplacer@@6B@ ; CColorReplacer::`vftable'

; 715  : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CColorReplacer@@UAE@XZ ENDP				; CColorReplacer::~CColorReplacer
_TEXT	ENDS
PUBLIC	??0RGBX@@QAE@ABK@Z				; RGBX::RGBX
PUBLIC	??8RGBX@@QBEHABV0@@Z				; RGBX::operator==
;	COMDAT ?ProcessPixels@CColorReplacer@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_this$ = -32
_bRes$ = -4
_rgbFrom$75465 = -16
_rgbTo$75466 = -12
_nX$75467 = -8
_nY$75471 = -20
_pRGBSrc$75475 = -28
_pRGBDest$75476 = -24
?ProcessPixels@CColorReplacer@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; CColorReplacer::ProcessPixels, COMDAT

; 719  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 32					; 00000020H
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

; 720  : 	BOOL bRes = TRUE;

	mov	DWORD PTR _bRes$[ebp], 1

; 721  : 
; 722  : 	if (m_crFrom == m_crTo)

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax+8]
	cmp	edx, DWORD PTR [ecx+12]
	jne	SHORT $L75463

; 723  : 		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	push	-1
	mov	eax, DWORD PTR _sizeDest$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sizeDest$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	push	edx
	mov	eax, DWORD PTR _sizeSrc$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sizeSrc$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pSrcPixels$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z ; C32BitImageProcessor::ProcessPixels
	mov	DWORD PTR _bRes$[ebp], eax

; 724  : 	else

	jmp	$L75470
$L75463:

; 726  : 		RGBX rgbFrom(m_crFrom), rgbTo(m_crTo);

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 8
	push	eax
	lea	ecx, DWORD PTR _rgbFrom$75465[ebp]
	call	??0RGBX@@QAE@ABK@Z			; RGBX::RGBX
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	push	ecx
	lea	ecx, DWORD PTR _rgbTo$75466[ebp]
	call	??0RGBX@@QAE@ABK@Z			; RGBX::RGBX

; 727  : 
; 728  : 		for (int nX = 0; nX < sizeSrc.cx; nX++)

	mov	DWORD PTR _nX$75467[ebp], 0
	jmp	SHORT $L75468
$L75469:
	mov	edx, DWORD PTR _nX$75467[ebp]
	add	edx, 1
	mov	DWORD PTR _nX$75467[ebp], edx
$L75468:
	mov	eax, DWORD PTR _nX$75467[ebp]
	cmp	eax, DWORD PTR _sizeSrc$[ebp]
	jge	SHORT $L75470

; 730  : 			for (int nY = 0; nY < sizeSrc.cy; nY++)

	mov	DWORD PTR _nY$75471[ebp], 0
	jmp	SHORT $L75472
$L75473:
	mov	ecx, DWORD PTR _nY$75471[ebp]
	add	ecx, 1
	mov	DWORD PTR _nY$75471[ebp], ecx
$L75472:
	mov	edx, DWORD PTR _nY$75471[ebp]
	cmp	edx, DWORD PTR _sizeSrc$[ebp+4]
	jge	SHORT $L75474

; 732  : 				RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];

	mov	eax, DWORD PTR _nY$75471[ebp]
	imul	eax, DWORD PTR _sizeSrc$[ebp]
	add	eax, DWORD PTR _nX$75467[ebp]
	mov	ecx, DWORD PTR _pSrcPixels$[ebp]
	lea	edx, DWORD PTR [ecx+eax*4]
	mov	DWORD PTR _pRGBSrc$75475[ebp], edx

; 733  : 				RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];

	mov	eax, DWORD PTR _nY$75471[ebp]
	imul	eax, DWORD PTR _sizeDest$[ebp]
	add	eax, DWORD PTR _nX$75467[ebp]
	mov	ecx, DWORD PTR _pDestPixels$[ebp]
	lea	edx, DWORD PTR [ecx+eax*4]
	mov	DWORD PTR _pRGBDest$75476[ebp], edx

; 734  : 
; 735  : 				if (*pRGBSrc == rgbFrom)

	lea	eax, DWORD PTR _rgbFrom$75465[ebp]
	push	eax
	mov	ecx, DWORD PTR _pRGBSrc$75475[ebp]
	call	??8RGBX@@QBEHABV0@@Z			; RGBX::operator==
	test	eax, eax
	je	SHORT $L75477

; 736  : 					*pRGBDest = rgbTo;

	lea	ecx, DWORD PTR _rgbTo$75466[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pRGBDest$75476[ebp]
	call	??4RGBX@@QAEABV0@ABV0@@Z		; RGBX::operator=

; 737  : 				else

	jmp	SHORT $L75478
$L75477:

; 738  : 					*pRGBDest = *pRGBSrc;

	mov	edx, DWORD PTR _pRGBSrc$75475[ebp]
	push	edx
	mov	ecx, DWORD PTR _pRGBDest$75476[ebp]
	call	??4RGBX@@QAEABV0@ABV0@@Z		; RGBX::operator=
$L75478:

; 739  : 			}

	jmp	SHORT $L75473
$L75474:

; 740  : 		}

	jmp	SHORT $L75469
$L75470:

; 742  : 
; 743  : 	return bRes;

	mov	eax, DWORD PTR _bRes$[ebp]

; 744  : }

	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?ProcessPixels@CColorReplacer@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; CColorReplacer::ProcessPixels
_TEXT	ENDS
;	COMDAT ??0RGBX@@QAE@ABK@Z
_TEXT	SEGMENT
_color$ = 8
_this$ = -4
??0RGBX@@QAE@ABK@Z PROC NEAR				; RGBX::RGBX, COMDAT

; 41   : 	RGBX(const COLORREF& color) { btRed = GetRValue(color); btBlue = GetBValue(color); btGreen = GetGValue(color); btUnused = 0; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _color$[ebp]
	mov	dl, BYTE PTR [ecx]
	mov	BYTE PTR [eax+2], dl
	mov	eax, DWORD PTR _color$[ebp]
	mov	ecx, DWORD PTR [eax]
	shr	ecx, 16					; 00000010H
	mov	edx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [edx], cl
	mov	eax, DWORD PTR _color$[ebp]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax]
	sar	ecx, 8
	mov	edx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [edx+1], cl
	mov	eax, DWORD PTR _this$[ebp]
	mov	BYTE PTR [eax+3], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	4
??0RGBX@@QAE@ABK@Z ENDP					; RGBX::RGBX
_TEXT	ENDS
;	COMDAT ??8RGBX@@QBEHABV0@@Z
_TEXT	SEGMENT
_rgb$ = 8
_this$ = -4
??8RGBX@@QBEHABV0@@Z PROC NEAR				; RGBX::operator==, COMDAT

; 44   : 	inline BOOL operator==(const RGBX& rgb) const { return (btRed == rgb.btRed && btGreen == rgb.btGreen && btBlue == rgb.btBlue); }

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+2]
	mov	edx, DWORD PTR _rgb$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+2]
	cmp	ecx, eax
	jne	SHORT $L76229
	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+1]
	mov	eax, DWORD PTR _rgb$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+1]
	cmp	edx, ecx
	jne	SHORT $L76229
	mov	edx, DWORD PTR _this$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx]
	mov	ecx, DWORD PTR _rgb$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx]
	cmp	eax, edx
	jne	SHORT $L76229
	mov	DWORD PTR -8+[ebp], 1
	jmp	SHORT $L76230
$L76229:
	mov	DWORD PTR -8+[ebp], 0
$L76230:
	mov	eax, DWORD PTR -8+[ebp]
	mov	esp, ebp
	pop	ebp
	ret	4
??8RGBX@@QBEHABV0@@Z ENDP				; RGBX::operator==
_TEXT	ENDS
PUBLIC	??0CImageColorizer@@QAE@K@Z			; CImageColorizer::CImageColorizer
PUBLIC	?ProcessPixels@CImageColorizer@@UAEHPAVRGBX@@VCSize@@01K@Z ; CImageColorizer::ProcessPixels
PUBLIC	??_7CImageColorizer@@6B@			; CImageColorizer::`vftable'
PUBLIC	??_GCImageColorizer@@UAEPAXI@Z			; CImageColorizer::`scalar deleting destructor'
PUBLIC	??_ECImageColorizer@@UAEPAXI@Z			; CImageColorizer::`vector deleting destructor'
;	COMDAT ??_7CImageColorizer@@6B@
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
??_7CImageColorizer@@6B@ DD FLAT:??_ECImageColorizer@@UAEPAXI@Z ; CImageColorizer::`vftable'
	DD	FLAT:?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z
	DD	FLAT:?ProcessPixels@CImageColorizer@@UAEHPAVRGBX@@VCSize@@01K@Z
CONST	ENDS
;	COMDAT ??0CImageColorizer@@QAE@K@Z
_TEXT	SEGMENT
_color$ = 8
_this$ = -4
??0CImageColorizer@@QAE@K@Z PROC NEAR			; CImageColorizer::CImageColorizer, COMDAT

; 749  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CImageGrayer@@QAE@XZ			; CImageGrayer::CImageGrayer
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _color$[ebp]
	mov	DWORD PTR [eax+40], ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7CImageColorizer@@6B@ ; CImageColorizer::`vftable'

; 750  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0CImageColorizer@@QAE@K@Z ENDP			; CImageColorizer::CImageColorizer
_TEXT	ENDS
PUBLIC	??1CImageColorizer@@UAE@XZ			; CImageColorizer::~CImageColorizer
;	COMDAT ??_GCImageColorizer@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCImageColorizer@@UAEPAXI@Z PROC NEAR		; CImageColorizer::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CImageColorizer@@UAE@XZ		; CImageColorizer::~CImageColorizer
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75486
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75486:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCImageColorizer@@UAEPAXI@Z ENDP			; CImageColorizer::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CImageColorizer@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CImageColorizer@@UAE@XZ PROC NEAR			; CImageColorizer::~CImageColorizer, COMDAT

; 753  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageColorizer@@6B@ ; CImageColorizer::`vftable'

; 754  : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CImageGrayer@@UAE@XZ			; CImageGrayer::~CImageGrayer
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CImageColorizer@@UAE@XZ ENDP				; CImageColorizer::~CImageColorizer
_TEXT	ENDS
EXTRN	_memset:NEAR
EXTRN	__imp__MulDiv@12:NEAR
;	COMDAT ?ProcessPixels@CImageColorizer@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_this$ = -36
_bRes$ = -4
_rgb$75509 = -12
_nColorLum$75510 = -8
_nX$75513 = -16
_nY$75517 = -20
_pRGBSrc$75521 = -32
_pRGBDest$75522 = -24
_nPixelLum$75523 = -28
?ProcessPixels@CImageColorizer@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; CImageColorizer::ProcessPixels, COMDAT

; 758  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 48					; 00000030H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-48]
	mov	ecx, 12					; 0000000cH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 759  : 	BOOL bRes = TRUE;

	mov	DWORD PTR _bRes$[ebp], 1

; 760  : 
; 761  : 	if (m_color == 0)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+40], 0
	jne	SHORT $L75499

; 762  : 		FillMemory(pDestPixels, sizeDest.cx * sizeDest.cy * 4, 0);

	mov	ecx, DWORD PTR _sizeDest$[ebp]
	imul	ecx, DWORD PTR _sizeDest$[ebp+4]
	shl	ecx, 2
	push	ecx
	push	0
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	push	edx
	call	_memset
	add	esp, 12					; 0000000cH

; 763  : 
; 764  : 	else if (m_color == RGB(255, 255, 255))

	jmp	$L75516
$L75499:
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+40], 16777215		; 00ffffffH
	jne	SHORT $L75507

; 765  : 		return CImageGrayer::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	push	-1
	mov	ecx, DWORD PTR _sizeDest$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _sizeDest$[ebp]
	push	edx
	mov	eax, DWORD PTR _pDestPixels$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sizeSrc$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _sizeSrc$[ebp]
	push	edx
	mov	eax, DWORD PTR _pSrcPixels$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessPixels@CImageGrayer@@UAEHPAVRGBX@@VCSize@@01K@Z ; CImageGrayer::ProcessPixels
	jmp	$L75497
$L75507:

; 769  : 		RGBX rgb(m_color);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	lea	ecx, DWORD PTR _rgb$75509[ebp]
	call	??0RGBX@@QAE@ABK@Z			; RGBX::RGBX

; 770  : 		int nColorLum = rgb.Luminance();

	lea	ecx, DWORD PTR _rgb$75509[ebp]
	call	?Luminance@RGBX@@QBEHXZ			; RGBX::Luminance
	mov	DWORD PTR _nColorLum$75510[ebp], eax

; 771  : 
; 772  : 		if (!nColorLum)

	cmp	DWORD PTR _nColorLum$75510[ebp], 0
	jne	SHORT $L75511

; 773  : 			FillMemory(pDestPixels, sizeDest.cx * sizeDest.cy * 4, 0);

	mov	edx, DWORD PTR _sizeDest$[ebp]
	imul	edx, DWORD PTR _sizeDest$[ebp+4]
	shl	edx, 2
	push	edx
	push	0
	mov	eax, DWORD PTR _pDestPixels$[ebp]
	push	eax
	call	_memset
	add	esp, 12					; 0000000cH

; 774  : 		else

	jmp	$L75516
$L75511:

; 776  : 			for (int nX = 0; nX < sizeSrc.cx; nX++)

	mov	DWORD PTR _nX$75513[ebp], 0
	jmp	SHORT $L75514
$L75515:
	mov	ecx, DWORD PTR _nX$75513[ebp]
	add	ecx, 1
	mov	DWORD PTR _nX$75513[ebp], ecx
$L75514:
	mov	edx, DWORD PTR _nX$75513[ebp]
	cmp	edx, DWORD PTR _sizeSrc$[ebp]
	jge	$L75516

; 778  : 				for (int nY = 0; nY < sizeSrc.cy; nY++)

	mov	DWORD PTR _nY$75517[ebp], 0
	jmp	SHORT $L75518
$L75519:
	mov	eax, DWORD PTR _nY$75517[ebp]
	add	eax, 1
	mov	DWORD PTR _nY$75517[ebp], eax
$L75518:
	mov	ecx, DWORD PTR _nY$75517[ebp]
	cmp	ecx, DWORD PTR _sizeSrc$[ebp+4]
	jge	$L75520

; 780  : 					RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];

	mov	edx, DWORD PTR _nY$75517[ebp]
	imul	edx, DWORD PTR _sizeSrc$[ebp]
	add	edx, DWORD PTR _nX$75513[ebp]
	mov	eax, DWORD PTR _pSrcPixels$[ebp]
	lea	ecx, DWORD PTR [eax+edx*4]
	mov	DWORD PTR _pRGBSrc$75521[ebp], ecx

; 781  : 					RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];

	mov	edx, DWORD PTR _nY$75517[ebp]
	imul	edx, DWORD PTR _sizeDest$[ebp]
	add	edx, DWORD PTR _nX$75513[ebp]
	mov	eax, DWORD PTR _pDestPixels$[ebp]
	lea	ecx, DWORD PTR [eax+edx*4]
	mov	DWORD PTR _pRGBDest$75522[ebp], ecx

; 782  : 
; 783  : 					pRGBDest->MakeGray(*pRGBSrc);

	mov	edx, DWORD PTR _pRGBSrc$75521[ebp]
	push	edx
	mov	ecx, DWORD PTR _pRGBDest$75522[ebp]
	call	?MakeGray@RGBX@@QAEXABV1@@Z		; RGBX::MakeGray

; 784  : 
; 785  : 					int nPixelLum = pRGBDest->Luminance();

	mov	ecx, DWORD PTR _pRGBDest$75522[ebp]
	call	?Luminance@RGBX@@QBEHXZ			; RGBX::Luminance
	mov	DWORD PTR _nPixelLum$75523[ebp], eax

; 786  : 					
; 787  : 					pRGBDest->btRed = (BYTE)min(255, MulDiv((int)rgb.btRed, nPixelLum, nColorLum));

	mov	esi, esp
	mov	eax, DWORD PTR _nColorLum$75510[ebp]
	push	eax
	mov	ecx, DWORD PTR _nPixelLum$75523[ebp]
	push	ecx
	mov	edx, DWORD PTR _rgb$75509[ebp+2]
	and	edx, 255				; 000000ffH
	push	edx
	call	DWORD PTR __imp__MulDiv@12
	cmp	esi, esp
	call	__chkesp
	cmp	eax, 255				; 000000ffH
	jle	SHORT $L76241
	mov	DWORD PTR -40+[ebp], 255		; 000000ffH
	jmp	SHORT $L76242
$L76241:
	mov	esi, esp
	mov	eax, DWORD PTR _nColorLum$75510[ebp]
	push	eax
	mov	ecx, DWORD PTR _nPixelLum$75523[ebp]
	push	ecx
	mov	edx, DWORD PTR _rgb$75509[ebp+2]
	and	edx, 255				; 000000ffH
	push	edx
	call	DWORD PTR __imp__MulDiv@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR -40+[ebp], eax
$L76242:
	mov	eax, DWORD PTR _pRGBDest$75522[ebp]
	mov	cl, BYTE PTR -40+[ebp]
	mov	BYTE PTR [eax+2], cl

; 788  : 					pRGBDest->btGreen = (BYTE)min(255, MulDiv((int)rgb.btGreen, nPixelLum, nColorLum));

	mov	esi, esp
	mov	edx, DWORD PTR _nColorLum$75510[ebp]
	push	edx
	mov	eax, DWORD PTR _nPixelLum$75523[ebp]
	push	eax
	mov	ecx, DWORD PTR _rgb$75509[ebp+1]
	and	ecx, 255				; 000000ffH
	push	ecx
	call	DWORD PTR __imp__MulDiv@12
	cmp	esi, esp
	call	__chkesp
	cmp	eax, 255				; 000000ffH
	jle	SHORT $L76243
	mov	DWORD PTR -44+[ebp], 255		; 000000ffH
	jmp	SHORT $L76244
$L76243:
	mov	esi, esp
	mov	edx, DWORD PTR _nColorLum$75510[ebp]
	push	edx
	mov	eax, DWORD PTR _nPixelLum$75523[ebp]
	push	eax
	mov	ecx, DWORD PTR _rgb$75509[ebp+1]
	and	ecx, 255				; 000000ffH
	push	ecx
	call	DWORD PTR __imp__MulDiv@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR -44+[ebp], eax
$L76244:
	mov	edx, DWORD PTR _pRGBDest$75522[ebp]
	mov	al, BYTE PTR -44+[ebp]
	mov	BYTE PTR [edx+1], al

; 789  : 					pRGBDest->btBlue = (BYTE)min(255, MulDiv((int)rgb.btBlue, nPixelLum, nColorLum));

	mov	esi, esp
	mov	ecx, DWORD PTR _nColorLum$75510[ebp]
	push	ecx
	mov	edx, DWORD PTR _nPixelLum$75523[ebp]
	push	edx
	mov	eax, DWORD PTR _rgb$75509[ebp]
	and	eax, 255				; 000000ffH
	push	eax
	call	DWORD PTR __imp__MulDiv@12
	cmp	esi, esp
	call	__chkesp
	cmp	eax, 255				; 000000ffH
	jle	SHORT $L76245
	mov	DWORD PTR -48+[ebp], 255		; 000000ffH
	jmp	SHORT $L76246
$L76245:
	mov	esi, esp
	mov	ecx, DWORD PTR _nColorLum$75510[ebp]
	push	ecx
	mov	edx, DWORD PTR _nPixelLum$75523[ebp]
	push	edx
	mov	eax, DWORD PTR _rgb$75509[ebp]
	and	eax, 255				; 000000ffH
	push	eax
	call	DWORD PTR __imp__MulDiv@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR -48+[ebp], eax
$L76246:
	mov	ecx, DWORD PTR _pRGBDest$75522[ebp]
	mov	dl, BYTE PTR -48+[ebp]
	mov	BYTE PTR [ecx], dl

; 790  : 				}

	jmp	$L75519
$L75520:

; 791  : 			}

	jmp	$L75515
$L75516:

; 794  : 
; 795  : 	return bRes;

	mov	eax, DWORD PTR _bRes$[ebp]
$L75497:

; 796  : }

	pop	edi
	pop	esi
	add	esp, 48					; 00000030H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?ProcessPixels@CImageColorizer@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; CImageColorizer::ProcessPixels
_TEXT	ENDS
PUBLIC	??0CImageTinter@@QAE@KH@Z			; CImageTinter::CImageTinter
PUBLIC	?ProcessPixels@CImageTinter@@UAEHPAVRGBX@@VCSize@@01K@Z ; CImageTinter::ProcessPixels
PUBLIC	??_7CImageTinter@@6B@				; CImageTinter::`vftable'
PUBLIC	??_GCImageTinter@@UAEPAXI@Z			; CImageTinter::`scalar deleting destructor'
PUBLIC	??_ECImageTinter@@UAEPAXI@Z			; CImageTinter::`vector deleting destructor'
;	COMDAT ??_7CImageTinter@@6B@
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
??_7CImageTinter@@6B@ DD FLAT:??_ECImageTinter@@UAEPAXI@Z ; CImageTinter::`vftable'
	DD	FLAT:?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z
	DD	FLAT:?ProcessPixels@CImageTinter@@UAEHPAVRGBX@@VCSize@@01K@Z
CONST	ENDS
;	COMDAT ??0CImageTinter@@QAE@KH@Z
_TEXT	SEGMENT
_color$ = 8
_nAmount$ = 12
_this$ = -4
??0CImageTinter@@QAE@KH@Z PROC NEAR			; CImageTinter::CImageTinter, COMDAT

; 801  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0C32BitImageProcessor@@QAE@H@Z	; C32BitImageProcessor::C32BitImageProcessor
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _color$[ebp]
	mov	DWORD PTR [eax+8], ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7CImageTinter@@6B@ ; CImageTinter::`vftable'

; 802  : 	m_nAmount = max(-100, min(100, nAmount));

	cmp	DWORD PTR _nAmount$[ebp], 100		; 00000064H
	jle	SHORT $L76249
	mov	DWORD PTR -8+[ebp], 100			; 00000064H
	jmp	SHORT $L76250
$L76249:
	mov	eax, DWORD PTR _nAmount$[ebp]
	mov	DWORD PTR -8+[ebp], eax
$L76250:
	cmp	DWORD PTR -8+[ebp], -100		; ffffff9cH
	jge	SHORT $L76253
	mov	DWORD PTR -12+[ebp], -100		; ffffff9cH
	jmp	SHORT $L76254
$L76253:
	cmp	DWORD PTR _nAmount$[ebp], 100		; 00000064H
	jle	SHORT $L76251
	mov	DWORD PTR -16+[ebp], 100		; 00000064H
	jmp	SHORT $L76252
$L76251:
	mov	ecx, DWORD PTR _nAmount$[ebp]
	mov	DWORD PTR -16+[ebp], ecx
$L76252:
	mov	edx, DWORD PTR -16+[ebp]
	mov	DWORD PTR -12+[ebp], edx
$L76254:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR -12+[ebp]
	mov	DWORD PTR [eax+12], ecx

; 803  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
??0CImageTinter@@QAE@KH@Z ENDP				; CImageTinter::CImageTinter
_TEXT	ENDS
PUBLIC	??1CImageTinter@@UAE@XZ				; CImageTinter::~CImageTinter
;	COMDAT ??_GCImageTinter@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCImageTinter@@UAEPAXI@Z PROC NEAR			; CImageTinter::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CImageTinter@@UAE@XZ			; CImageTinter::~CImageTinter
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75541
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75541:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCImageTinter@@UAEPAXI@Z ENDP			; CImageTinter::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CImageTinter@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CImageTinter@@UAE@XZ PROC NEAR			; CImageTinter::~CImageTinter, COMDAT

; 806  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageTinter@@6B@ ; CImageTinter::`vftable'

; 807  : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CImageTinter@@UAE@XZ ENDP				; CImageTinter::~CImageTinter
_TEXT	ENDS
PUBLIC	__real@4@4005c800000000000000
;	COMDAT __real@4@4005c800000000000000
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
__real@4@4005c800000000000000 DD 042c80000r	; 100
CONST	ENDS
;	COMDAT ?ProcessPixels@CImageTinter@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_crMask$ = 32
_this$ = -32
_bRes$ = -4
_rgb$75556 = -16
_fFactor$75557 = -8
_nX$75558 = -12
_nY$75562 = -20
_pRGBSrc$75566 = -28
_pRGBDest$75567 = -24
?ProcessPixels@CImageTinter@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; CImageTinter::ProcessPixels, COMDAT

; 811  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 116				; 00000074H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-116]
	mov	ecx, 29					; 0000001dH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 812  : 	BOOL bRes = TRUE;

	mov	DWORD PTR _bRes$[ebp], 1

; 813  : 
; 814  : 	if (!m_nAmount)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jne	SHORT $L75554

; 815  : 		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	push	-1
	mov	ecx, DWORD PTR _sizeDest$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _sizeDest$[ebp]
	push	edx
	mov	eax, DWORD PTR _pDestPixels$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sizeSrc$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _sizeSrc$[ebp]
	push	edx
	mov	eax, DWORD PTR _pSrcPixels$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z ; C32BitImageProcessor::ProcessPixels
	mov	DWORD PTR _bRes$[ebp], eax

; 816  : 	else

	jmp	$L75561
$L75554:

; 818  : 		RGBX rgb(m_color);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	push	ecx
	lea	ecx, DWORD PTR _rgb$75556[ebp]
	call	??0RGBX@@QAE@ABK@Z			; RGBX::RGBX

; 819  : 		float fFactor = m_nAmount / 100.0f;

	mov	edx, DWORD PTR _this$[ebp]
	fild	DWORD PTR [edx+12]
	fdiv	DWORD PTR __real@4@4005c800000000000000
	fstp	DWORD PTR _fFactor$75557[ebp]

; 820  : 		
; 821  : 		for (int nX = 0; nX < sizeSrc.cx; nX++)

	mov	DWORD PTR _nX$75558[ebp], 0
	jmp	SHORT $L75559
$L75560:
	mov	eax, DWORD PTR _nX$75558[ebp]
	add	eax, 1
	mov	DWORD PTR _nX$75558[ebp], eax
$L75559:
	mov	ecx, DWORD PTR _nX$75558[ebp]
	cmp	ecx, DWORD PTR _sizeSrc$[ebp]
	jge	$L75561

; 823  : 			for (int nY = 0; nY < sizeSrc.cy; nY++)

	mov	DWORD PTR _nY$75562[ebp], 0
	jmp	SHORT $L75563
$L75564:
	mov	edx, DWORD PTR _nY$75562[ebp]
	add	edx, 1
	mov	DWORD PTR _nY$75562[ebp], edx
$L75563:
	mov	eax, DWORD PTR _nY$75562[ebp]
	cmp	eax, DWORD PTR _sizeSrc$[ebp+4]
	jge	$L75565

; 825  : 				RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];

	mov	ecx, DWORD PTR _nY$75562[ebp]
	imul	ecx, DWORD PTR _sizeSrc$[ebp]
	add	ecx, DWORD PTR _nX$75558[ebp]
	mov	edx, DWORD PTR _pSrcPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGBSrc$75566[ebp], eax

; 826  : 				RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];

	mov	ecx, DWORD PTR _nY$75562[ebp]
	imul	ecx, DWORD PTR _sizeDest$[ebp]
	add	ecx, DWORD PTR _nX$75558[ebp]
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGBDest$75567[ebp], eax

; 827  : 				
; 828  : 				if (crMask == -1 || !(crMask == *pRGBSrc))

	cmp	DWORD PTR _crMask$[ebp], -1
	je	SHORT $L75569
	mov	ecx, DWORD PTR _pRGBSrc$75566[ebp]
	call	??BRGBX@@QBEKXZ				; RGBX::operator unsigned long
	cmp	DWORD PTR _crMask$[ebp], eax
	je	$L75568
$L75569:

; 830  : 					pRGBDest->btRed = (BYTE)min(255, max(0, (int)(pRGBSrc->btRed + (int)(rgb.btRed * fFactor))));

	mov	ecx, DWORD PTR _pRGBSrc$75566[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+2]
	mov	esi, edx
	mov	eax, DWORD PTR _rgb$75556[ebp+2]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR -36+[ebp], eax
	fild	DWORD PTR -36+[ebp]
	fmul	DWORD PTR _fFactor$75557[ebp]
	call	__ftol
	add	esi, eax
	test	esi, esi
	jge	SHORT $L76263
	mov	DWORD PTR -40+[ebp], 0
	jmp	SHORT $L76264
$L76263:
	mov	ecx, DWORD PTR _pRGBSrc$75566[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+2]
	mov	esi, edx
	mov	eax, DWORD PTR _rgb$75556[ebp+2]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR -44+[ebp], eax
	fild	DWORD PTR -44+[ebp]
	fmul	DWORD PTR _fFactor$75557[ebp]
	call	__ftol
	add	esi, eax
	mov	DWORD PTR -40+[ebp], esi
$L76264:
	cmp	DWORD PTR -40+[ebp], 255		; 000000ffH
	jle	SHORT $L76267
	mov	DWORD PTR -48+[ebp], 255		; 000000ffH
	jmp	SHORT $L76268
$L76267:
	mov	ecx, DWORD PTR _pRGBSrc$75566[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+2]
	mov	esi, edx
	mov	eax, DWORD PTR _rgb$75556[ebp+2]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR -52+[ebp], eax
	fild	DWORD PTR -52+[ebp]
	fmul	DWORD PTR _fFactor$75557[ebp]
	call	__ftol
	add	esi, eax
	test	esi, esi
	jge	SHORT $L76265
	mov	DWORD PTR -56+[ebp], 0
	jmp	SHORT $L76266
$L76265:
	mov	ecx, DWORD PTR _pRGBSrc$75566[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+2]
	mov	esi, edx
	mov	eax, DWORD PTR _rgb$75556[ebp+2]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR -60+[ebp], eax
	fild	DWORD PTR -60+[ebp]
	fmul	DWORD PTR _fFactor$75557[ebp]
	call	__ftol
	add	esi, eax
	mov	DWORD PTR -56+[ebp], esi
$L76266:
	mov	ecx, DWORD PTR -56+[ebp]
	mov	DWORD PTR -48+[ebp], ecx
$L76268:
	mov	edx, DWORD PTR _pRGBDest$75567[ebp]
	mov	al, BYTE PTR -48+[ebp]
	mov	BYTE PTR [edx+2], al

; 831  : 					pRGBDest->btGreen = (BYTE)min(255, max(0, (int)(pRGBSrc->btGreen + (int)(rgb.btGreen * fFactor))));

	mov	ecx, DWORD PTR _pRGBSrc$75566[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+1]
	mov	esi, edx
	mov	eax, DWORD PTR _rgb$75556[ebp+1]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR -64+[ebp], eax
	fild	DWORD PTR -64+[ebp]
	fmul	DWORD PTR _fFactor$75557[ebp]
	call	__ftol
	add	esi, eax
	test	esi, esi
	jge	SHORT $L76269
	mov	DWORD PTR -68+[ebp], 0
	jmp	SHORT $L76270
$L76269:
	mov	ecx, DWORD PTR _pRGBSrc$75566[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+1]
	mov	esi, edx
	mov	eax, DWORD PTR _rgb$75556[ebp+1]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR -72+[ebp], eax
	fild	DWORD PTR -72+[ebp]
	fmul	DWORD PTR _fFactor$75557[ebp]
	call	__ftol
	add	esi, eax
	mov	DWORD PTR -68+[ebp], esi
$L76270:
	cmp	DWORD PTR -68+[ebp], 255		; 000000ffH
	jle	SHORT $L76273
	mov	DWORD PTR -76+[ebp], 255		; 000000ffH
	jmp	SHORT $L76274
$L76273:
	mov	ecx, DWORD PTR _pRGBSrc$75566[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+1]
	mov	esi, edx
	mov	eax, DWORD PTR _rgb$75556[ebp+1]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR -80+[ebp], eax
	fild	DWORD PTR -80+[ebp]
	fmul	DWORD PTR _fFactor$75557[ebp]
	call	__ftol
	add	esi, eax
	test	esi, esi
	jge	SHORT $L76271
	mov	DWORD PTR -84+[ebp], 0
	jmp	SHORT $L76272
$L76271:
	mov	ecx, DWORD PTR _pRGBSrc$75566[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+1]
	mov	esi, edx
	mov	eax, DWORD PTR _rgb$75556[ebp+1]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR -88+[ebp], eax
	fild	DWORD PTR -88+[ebp]
	fmul	DWORD PTR _fFactor$75557[ebp]
	call	__ftol
	add	esi, eax
	mov	DWORD PTR -84+[ebp], esi
$L76272:
	mov	ecx, DWORD PTR -84+[ebp]
	mov	DWORD PTR -76+[ebp], ecx
$L76274:
	mov	edx, DWORD PTR _pRGBDest$75567[ebp]
	mov	al, BYTE PTR -76+[ebp]
	mov	BYTE PTR [edx+1], al

; 832  : 					pRGBDest->btBlue = (BYTE)min(255, max(0, (int)(pRGBSrc->btBlue + (int)(rgb.btBlue * fFactor))));

	mov	ecx, DWORD PTR _pRGBSrc$75566[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx]
	mov	esi, edx
	mov	eax, DWORD PTR _rgb$75556[ebp]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR -92+[ebp], eax
	fild	DWORD PTR -92+[ebp]
	fmul	DWORD PTR _fFactor$75557[ebp]
	call	__ftol
	add	esi, eax
	test	esi, esi
	jge	SHORT $L76275
	mov	DWORD PTR -96+[ebp], 0
	jmp	SHORT $L76276
$L76275:
	mov	ecx, DWORD PTR _pRGBSrc$75566[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx]
	mov	esi, edx
	mov	eax, DWORD PTR _rgb$75556[ebp]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR -100+[ebp], eax
	fild	DWORD PTR -100+[ebp]
	fmul	DWORD PTR _fFactor$75557[ebp]
	call	__ftol
	add	esi, eax
	mov	DWORD PTR -96+[ebp], esi
$L76276:
	cmp	DWORD PTR -96+[ebp], 255		; 000000ffH
	jle	SHORT $L76279
	mov	DWORD PTR -104+[ebp], 255		; 000000ffH
	jmp	SHORT $L76280
$L76279:
	mov	ecx, DWORD PTR _pRGBSrc$75566[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx]
	mov	esi, edx
	mov	eax, DWORD PTR _rgb$75556[ebp]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR -108+[ebp], eax
	fild	DWORD PTR -108+[ebp]
	fmul	DWORD PTR _fFactor$75557[ebp]
	call	__ftol
	add	esi, eax
	test	esi, esi
	jge	SHORT $L76277
	mov	DWORD PTR -112+[ebp], 0
	jmp	SHORT $L76278
$L76277:
	mov	ecx, DWORD PTR _pRGBSrc$75566[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx]
	mov	esi, edx
	mov	eax, DWORD PTR _rgb$75556[ebp]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR -116+[ebp], eax
	fild	DWORD PTR -116+[ebp]
	fmul	DWORD PTR _fFactor$75557[ebp]
	call	__ftol
	add	esi, eax
	mov	DWORD PTR -112+[ebp], esi
$L76278:
	mov	ecx, DWORD PTR -112+[ebp]
	mov	DWORD PTR -104+[ebp], ecx
$L76280:
	mov	edx, DWORD PTR _pRGBDest$75567[ebp]
	mov	al, BYTE PTR -104+[ebp]
	mov	BYTE PTR [edx], al

; 834  : 				else

	jmp	SHORT $L75597
$L75568:

; 835  : 					*pRGBDest = *pRGBSrc;

	mov	ecx, DWORD PTR _pRGBSrc$75566[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pRGBDest$75567[ebp]
	call	??4RGBX@@QAEABV0@ABV0@@Z		; RGBX::operator=
$L75597:

; 836  : 			}

	jmp	$L75564
$L75565:

; 837  : 		}

	jmp	$L75560
$L75561:

; 839  : 
; 840  : 	return bRes;

	mov	eax, DWORD PTR _bRes$[ebp]

; 841  : }

	pop	edi
	pop	esi
	add	esp, 116				; 00000074H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?ProcessPixels@CImageTinter@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; CImageTinter::ProcessPixels
_TEXT	ENDS
PUBLIC	??0CImageContraster@@QAE@H@Z			; CImageContraster::CImageContraster
PUBLIC	?ProcessPixels@CImageContraster@@UAEHPAVRGBX@@VCSize@@01K@Z ; CImageContraster::ProcessPixels
PUBLIC	??_7CImageContraster@@6B@			; CImageContraster::`vftable'
PUBLIC	??_GCImageContraster@@UAEPAXI@Z			; CImageContraster::`scalar deleting destructor'
PUBLIC	??_ECImageContraster@@UAEPAXI@Z			; CImageContraster::`vector deleting destructor'
;	COMDAT ??_7CImageContraster@@6B@
; File D:\_CODE\Shared\ImageProcessors.cpp
CONST	SEGMENT
??_7CImageContraster@@6B@ DD FLAT:??_ECImageContraster@@UAEPAXI@Z ; CImageContraster::`vftable'
	DD	FLAT:?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z
	DD	FLAT:?ProcessPixels@CImageContraster@@UAEHPAVRGBX@@VCSize@@01K@Z
CONST	ENDS
;	COMDAT ??0CImageContraster@@QAE@H@Z
_TEXT	SEGMENT
_nAmount$ = 8
_this$ = -4
??0CImageContraster@@QAE@H@Z PROC NEAR			; CImageContraster::CImageContraster, COMDAT

; 846  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0C32BitImageProcessor@@QAE@H@Z	; C32BitImageProcessor::C32BitImageProcessor
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageContraster@@6B@ ; CImageContraster::`vftable'

; 847  : 	m_nAmount = max(-100, min(200, nAmount));

	cmp	DWORD PTR _nAmount$[ebp], 200		; 000000c8H
	jle	SHORT $L76285
	mov	DWORD PTR -8+[ebp], 200			; 000000c8H
	jmp	SHORT $L76286
$L76285:
	mov	ecx, DWORD PTR _nAmount$[ebp]
	mov	DWORD PTR -8+[ebp], ecx
$L76286:
	cmp	DWORD PTR -8+[ebp], -100		; ffffff9cH
	jge	SHORT $L76289
	mov	DWORD PTR -12+[ebp], -100		; ffffff9cH
	jmp	SHORT $L76290
$L76289:
	cmp	DWORD PTR _nAmount$[ebp], 200		; 000000c8H
	jle	SHORT $L76287
	mov	DWORD PTR -16+[ebp], 200		; 000000c8H
	jmp	SHORT $L76288
$L76287:
	mov	edx, DWORD PTR _nAmount$[ebp]
	mov	DWORD PTR -16+[ebp], edx
$L76288:
	mov	eax, DWORD PTR -16+[ebp]
	mov	DWORD PTR -12+[ebp], eax
$L76290:
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR -12+[ebp]
	mov	DWORD PTR [ecx+8], edx

; 848  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0CImageContraster@@QAE@H@Z ENDP			; CImageContraster::CImageContraster
_TEXT	ENDS
PUBLIC	??1CImageContraster@@UAE@XZ			; CImageContraster::~CImageContraster
;	COMDAT ??_GCImageContraster@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCImageContraster@@UAEPAXI@Z PROC NEAR		; CImageContraster::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CImageContraster@@UAE@XZ		; CImageContraster::~CImageContraster
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75605
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75605:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCImageContraster@@UAEPAXI@Z ENDP			; CImageContraster::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1CImageContraster@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CImageContraster@@UAE@XZ PROC NEAR			; CImageContraster::~CImageContraster, COMDAT

; 851  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CImageContraster@@6B@ ; CImageContraster::`vftable'

; 852  : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CImageContraster@@UAE@XZ ENDP			; CImageContraster::~CImageContraster
_TEXT	ENDS
;	COMDAT ?ProcessPixels@CImageContraster@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_crMask$ = 32
_this$ = -28
_bRes$ = -4
_fFactor$75620 = -8
_nX$75621 = -12
_nY$75625 = -16
_pRGBSrc$75629 = -24
_pRGBDest$75630 = -20
?ProcessPixels@CImageContraster@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; CImageContraster::ProcessPixels, COMDAT

; 856  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 112				; 00000070H
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-112]
	mov	ecx, 28					; 0000001cH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 857  : 	BOOL bRes = TRUE;

	mov	DWORD PTR _bRes$[ebp], 1

; 858  : 
; 859  : 	if (!m_nAmount)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	jne	SHORT $L75618

; 860  : 		bRes = C32BitImageProcessor::ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	push	-1
	mov	ecx, DWORD PTR _sizeDest$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _sizeDest$[ebp]
	push	edx
	mov	eax, DWORD PTR _pDestPixels$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sizeSrc$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _sizeSrc$[ebp]
	push	edx
	mov	eax, DWORD PTR _pSrcPixels$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z ; C32BitImageProcessor::ProcessPixels
	mov	DWORD PTR _bRes$[ebp], eax

; 861  : 	else

	jmp	$L75624
$L75618:

; 863  : 		float fFactor = 1.0f + m_nAmount / 100.0f;

	mov	ecx, DWORD PTR _this$[ebp]
	fild	DWORD PTR [ecx+8]
	fdiv	DWORD PTR __real@4@4005c800000000000000
	fadd	DWORD PTR __real@4@3fff8000000000000000
	fstp	DWORD PTR _fFactor$75620[ebp]

; 864  : 
; 865  : 		for (int nX = 0; nX < sizeSrc.cx; nX++)

	mov	DWORD PTR _nX$75621[ebp], 0
	jmp	SHORT $L75622
$L75623:
	mov	edx, DWORD PTR _nX$75621[ebp]
	add	edx, 1
	mov	DWORD PTR _nX$75621[ebp], edx
$L75622:
	mov	eax, DWORD PTR _nX$75621[ebp]
	cmp	eax, DWORD PTR _sizeSrc$[ebp]
	jge	$L75624

; 867  : 			for (int nY = 0; nY < sizeSrc.cy; nY++)

	mov	DWORD PTR _nY$75625[ebp], 0
	jmp	SHORT $L75626
$L75627:
	mov	ecx, DWORD PTR _nY$75625[ebp]
	add	ecx, 1
	mov	DWORD PTR _nY$75625[ebp], ecx
$L75626:
	mov	edx, DWORD PTR _nY$75625[ebp]
	cmp	edx, DWORD PTR _sizeSrc$[ebp+4]
	jge	$L75628

; 869  : 				RGBX* pRGBSrc = &pSrcPixels[nY * sizeSrc.cx + nX];

	mov	eax, DWORD PTR _nY$75625[ebp]
	imul	eax, DWORD PTR _sizeSrc$[ebp]
	add	eax, DWORD PTR _nX$75621[ebp]
	mov	ecx, DWORD PTR _pSrcPixels$[ebp]
	lea	edx, DWORD PTR [ecx+eax*4]
	mov	DWORD PTR _pRGBSrc$75629[ebp], edx

; 870  : 				RGBX* pRGBDest = &pDestPixels[nY * sizeDest.cx + nX];

	mov	eax, DWORD PTR _nY$75625[ebp]
	imul	eax, DWORD PTR _sizeDest$[ebp]
	add	eax, DWORD PTR _nX$75621[ebp]
	mov	ecx, DWORD PTR _pDestPixels$[ebp]
	lea	edx, DWORD PTR [ecx+eax*4]
	mov	DWORD PTR _pRGBDest$75630[ebp], edx

; 871  : 
; 872  : 				if (crMask == -1 || !(crMask == *pRGBSrc))

	cmp	DWORD PTR _crMask$[ebp], -1
	je	SHORT $L75632
	mov	ecx, DWORD PTR _pRGBSrc$75629[ebp]
	call	??BRGBX@@QBEKXZ				; RGBX::operator unsigned long
	cmp	DWORD PTR _crMask$[ebp], eax
	je	$L75631
$L75632:

; 874  : 					pRGBDest->btRed = (BYTE)max(0, min(255, (int)((pRGBSrc->btRed - 128) * fFactor) + 128));

	mov	eax, DWORD PTR _pRGBSrc$75629[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+2]
	sub	ecx, 128				; 00000080H
	mov	DWORD PTR -32+[ebp], ecx
	fild	DWORD PTR -32+[ebp]
	fmul	DWORD PTR _fFactor$75620[ebp]
	call	__ftol
	add	eax, 128				; 00000080H
	cmp	eax, 255				; 000000ffH
	jle	SHORT $L76299
	mov	DWORD PTR -36+[ebp], 255		; 000000ffH
	jmp	SHORT $L76300
$L76299:
	mov	edx, DWORD PTR _pRGBSrc$75629[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+2]
	sub	eax, 128				; 00000080H
	mov	DWORD PTR -40+[ebp], eax
	fild	DWORD PTR -40+[ebp]
	fmul	DWORD PTR _fFactor$75620[ebp]
	call	__ftol
	add	eax, 128				; 00000080H
	mov	DWORD PTR -36+[ebp], eax
$L76300:
	cmp	DWORD PTR -36+[ebp], 0
	jge	SHORT $L76303
	mov	DWORD PTR -44+[ebp], 0
	jmp	SHORT $L76304
$L76303:
	mov	ecx, DWORD PTR _pRGBSrc$75629[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+2]
	sub	edx, 128				; 00000080H
	mov	DWORD PTR -48+[ebp], edx
	fild	DWORD PTR -48+[ebp]
	fmul	DWORD PTR _fFactor$75620[ebp]
	call	__ftol
	add	eax, 128				; 00000080H
	cmp	eax, 255				; 000000ffH
	jle	SHORT $L76301
	mov	DWORD PTR -52+[ebp], 255		; 000000ffH
	jmp	SHORT $L76302
$L76301:
	mov	eax, DWORD PTR _pRGBSrc$75629[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+2]
	sub	ecx, 128				; 00000080H
	mov	DWORD PTR -56+[ebp], ecx
	fild	DWORD PTR -56+[ebp]
	fmul	DWORD PTR _fFactor$75620[ebp]
	call	__ftol
	add	eax, 128				; 00000080H
	mov	DWORD PTR -52+[ebp], eax
$L76302:
	mov	edx, DWORD PTR -52+[ebp]
	mov	DWORD PTR -44+[ebp], edx
$L76304:
	mov	eax, DWORD PTR _pRGBDest$75630[ebp]
	mov	cl, BYTE PTR -44+[ebp]
	mov	BYTE PTR [eax+2], cl

; 875  : 					pRGBDest->btGreen = (BYTE)max(0, min(255, (int)((pRGBSrc->btGreen - 128) * fFactor) + 128));

	mov	edx, DWORD PTR _pRGBSrc$75629[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+1]
	sub	eax, 128				; 00000080H
	mov	DWORD PTR -60+[ebp], eax
	fild	DWORD PTR -60+[ebp]
	fmul	DWORD PTR _fFactor$75620[ebp]
	call	__ftol
	add	eax, 128				; 00000080H
	cmp	eax, 255				; 000000ffH
	jle	SHORT $L76305
	mov	DWORD PTR -64+[ebp], 255		; 000000ffH
	jmp	SHORT $L76306
$L76305:
	mov	ecx, DWORD PTR _pRGBSrc$75629[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+1]
	sub	edx, 128				; 00000080H
	mov	DWORD PTR -68+[ebp], edx
	fild	DWORD PTR -68+[ebp]
	fmul	DWORD PTR _fFactor$75620[ebp]
	call	__ftol
	add	eax, 128				; 00000080H
	mov	DWORD PTR -64+[ebp], eax
$L76306:
	cmp	DWORD PTR -64+[ebp], 0
	jge	SHORT $L76309
	mov	DWORD PTR -72+[ebp], 0
	jmp	SHORT $L76310
$L76309:
	mov	eax, DWORD PTR _pRGBSrc$75629[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+1]
	sub	ecx, 128				; 00000080H
	mov	DWORD PTR -76+[ebp], ecx
	fild	DWORD PTR -76+[ebp]
	fmul	DWORD PTR _fFactor$75620[ebp]
	call	__ftol
	add	eax, 128				; 00000080H
	cmp	eax, 255				; 000000ffH
	jle	SHORT $L76307
	mov	DWORD PTR -80+[ebp], 255		; 000000ffH
	jmp	SHORT $L76308
$L76307:
	mov	edx, DWORD PTR _pRGBSrc$75629[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+1]
	sub	eax, 128				; 00000080H
	mov	DWORD PTR -84+[ebp], eax
	fild	DWORD PTR -84+[ebp]
	fmul	DWORD PTR _fFactor$75620[ebp]
	call	__ftol
	add	eax, 128				; 00000080H
	mov	DWORD PTR -80+[ebp], eax
$L76308:
	mov	ecx, DWORD PTR -80+[ebp]
	mov	DWORD PTR -72+[ebp], ecx
$L76310:
	mov	edx, DWORD PTR _pRGBDest$75630[ebp]
	mov	al, BYTE PTR -72+[ebp]
	mov	BYTE PTR [edx+1], al

; 876  : 					pRGBDest->btBlue = (BYTE)max(0, min(255, (int)((pRGBSrc->btBlue - 128) * fFactor) + 128));

	mov	ecx, DWORD PTR _pRGBSrc$75629[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx]
	sub	edx, 128				; 00000080H
	mov	DWORD PTR -88+[ebp], edx
	fild	DWORD PTR -88+[ebp]
	fmul	DWORD PTR _fFactor$75620[ebp]
	call	__ftol
	add	eax, 128				; 00000080H
	cmp	eax, 255				; 000000ffH
	jle	SHORT $L76311
	mov	DWORD PTR -92+[ebp], 255		; 000000ffH
	jmp	SHORT $L76312
$L76311:
	mov	eax, DWORD PTR _pRGBSrc$75629[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax]
	sub	ecx, 128				; 00000080H
	mov	DWORD PTR -96+[ebp], ecx
	fild	DWORD PTR -96+[ebp]
	fmul	DWORD PTR _fFactor$75620[ebp]
	call	__ftol
	add	eax, 128				; 00000080H
	mov	DWORD PTR -92+[ebp], eax
$L76312:
	cmp	DWORD PTR -92+[ebp], 0
	jge	SHORT $L76315
	mov	DWORD PTR -100+[ebp], 0
	jmp	SHORT $L76316
$L76315:
	mov	edx, DWORD PTR _pRGBSrc$75629[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx]
	sub	eax, 128				; 00000080H
	mov	DWORD PTR -104+[ebp], eax
	fild	DWORD PTR -104+[ebp]
	fmul	DWORD PTR _fFactor$75620[ebp]
	call	__ftol
	add	eax, 128				; 00000080H
	cmp	eax, 255				; 000000ffH
	jle	SHORT $L76313
	mov	DWORD PTR -108+[ebp], 255		; 000000ffH
	jmp	SHORT $L76314
$L76313:
	mov	ecx, DWORD PTR _pRGBSrc$75629[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx]
	sub	edx, 128				; 00000080H
	mov	DWORD PTR -112+[ebp], edx
	fild	DWORD PTR -112+[ebp]
	fmul	DWORD PTR _fFactor$75620[ebp]
	call	__ftol
	add	eax, 128				; 00000080H
	mov	DWORD PTR -108+[ebp], eax
$L76314:
	mov	eax, DWORD PTR -108+[ebp]
	mov	DWORD PTR -100+[ebp], eax
$L76316:
	mov	ecx, DWORD PTR _pRGBDest$75630[ebp]
	mov	dl, BYTE PTR -100+[ebp]
	mov	BYTE PTR [ecx], dl

; 878  : 				else

	jmp	SHORT $L75648
$L75631:

; 879  : 					*pRGBDest = *pRGBSrc;

	mov	eax, DWORD PTR _pRGBSrc$75629[ebp]
	push	eax
	mov	ecx, DWORD PTR _pRGBDest$75630[ebp]
	call	??4RGBX@@QAEABV0@ABV0@@Z		; RGBX::operator=
$L75648:

; 880  : 			}

	jmp	$L75627
$L75628:

; 881  : 		}

	jmp	$L75623
$L75624:

; 883  : 
; 884  : 	return bRes;

	mov	eax, DWORD PTR _bRes$[ebp]

; 885  : }

	pop	edi
	add	esp, 112				; 00000070H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?ProcessPixels@CImageContraster@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; CImageContraster::ProcessPixels
_TEXT	ENDS
PUBLIC	?ProcessPixels@CImageSysColorMapper@@UAEHPAVRGBX@@VCSize@@01K@Z ; CImageSysColorMapper::ProcessPixels
EXTRN	__imp__GetSysColor@4:NEAR
;	COMDAT xdata$x
; File D:\_CODE\Shared\ImageProcessors.cpp
xdata$x	SEGMENT
$T76321	DD	019930520H
	DD	01H
	DD	FLAT:$T76323
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76323	DD	0ffffffffH
	DD	FLAT:$L76319
xdata$x	ENDS
;	COMDAT ?ProcessPixels@CImageSysColorMapper@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_pSrcPixels$ = 8
_sizeSrc$ = 12
_pDestPixels$ = 20
_sizeDest$ = 24
_this$ = -40
_nMap$ = -16
_cr$75675 = -32
_pTemp$75677 = -36
?ProcessPixels@CImageSysColorMapper@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; CImageSysColorMapper::ProcessPixels, COMDAT

; 906  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76322
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 28					; 0000001cH
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx

; 907  : 	static int NUMCOLORMAPS = sizeof(COLORMAPPINGS) / sizeof(COLORMAPPING);
; 908  : 
; 909  : 	for (int nMap = 0; nMap < NUMCOLORMAPS; nMap++)

	mov	DWORD PTR _nMap$[ebp], 0
	jmp	SHORT $L75672
$L75673:
	mov	eax, DWORD PTR _nMap$[ebp]
	add	eax, 1
	mov	DWORD PTR _nMap$[ebp], eax
$L75672:
	mov	ecx, DWORD PTR _nMap$[ebp]
	cmp	ecx, DWORD PTR _?NUMCOLORMAPS@?1??ProcessPixels@CImageSysColorMapper@@UAEHPAVRGBX@@VCSize@@01K@Z@4HA
	jge	$L75674

; 911  : 		CColorReplacer cr(COLORMAPPINGS[nMap].color, GetSysColor(COLORMAPPINGS[nMap].nSysColor));

	mov	esi, esp
	mov	edx, DWORD PTR _nMap$[ebp]
	mov	eax, DWORD PTR _COLORMAPPINGS[edx*8+4]
	push	eax
	call	DWORD PTR __imp__GetSysColor@4
	cmp	esi, esp
	call	__chkesp
	push	eax
	mov	ecx, DWORD PTR _nMap$[ebp]
	mov	edx, DWORD PTR _COLORMAPPINGS[ecx*8]
	push	edx
	lea	ecx, DWORD PTR _cr$75675[ebp]
	call	??0CColorReplacer@@QAE@KK@Z		; CColorReplacer::CColorReplacer
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 912  : 
; 913  : 		cr.ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);

	push	-1
	mov	eax, DWORD PTR _sizeDest$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sizeDest$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	push	edx
	mov	eax, DWORD PTR _sizeSrc$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sizeSrc$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pSrcPixels$[ebp]
	push	edx
	lea	ecx, DWORD PTR _cr$75675[ebp]
	call	?ProcessPixels@CColorReplacer@@UAEHPAVRGBX@@VCSize@@01K@Z ; CColorReplacer::ProcessPixels

; 914  : 
; 915  : 		// switch source and dest for next iteration provided its not the last
; 916  : 		if (nMap < NUMCOLORMAPS - 1)

	mov	eax, DWORD PTR _?NUMCOLORMAPS@?1??ProcessPixels@CImageSysColorMapper@@UAEHPAVRGBX@@VCSize@@01K@Z@4HA
	sub	eax, 1
	cmp	DWORD PTR _nMap$[ebp], eax
	jge	SHORT $L75676

; 918  : 			RGBX* pTemp = pSrcPixels;

	mov	ecx, DWORD PTR _pSrcPixels$[ebp]
	mov	DWORD PTR _pTemp$75677[ebp], ecx

; 919  : 			pSrcPixels = pDestPixels;

	mov	edx, DWORD PTR _pDestPixels$[ebp]
	mov	DWORD PTR _pSrcPixels$[ebp], edx

; 920  : 			pDestPixels = pTemp;

	mov	eax, DWORD PTR _pTemp$75677[ebp]
	mov	DWORD PTR _pDestPixels$[ebp], eax
$L75676:

; 922  : 	}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _cr$75675[ebp]
	call	??1CColorReplacer@@UAE@XZ		; CColorReplacer::~CColorReplacer
	jmp	$L75673
$L75674:

; 923  : 	
; 924  : 	return TRUE;

	mov	eax, 1

; 925  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76319:
	lea	ecx, DWORD PTR _cr$75675[ebp]
	call	??1CColorReplacer@@UAE@XZ		; CColorReplacer::~CColorReplacer
	ret	0
$L76322:
	mov	eax, OFFSET FLAT:$T76321
	jmp	___CxxFrameHandler
text$x	ENDS
?ProcessPixels@CImageSysColorMapper@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; CImageSysColorMapper::ProcessPixels
END

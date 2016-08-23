	TITLE	D:\_CODE\Shared\GraphicsMisc.cpp
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
_BSS	SEGMENT PARA USE32 PUBLIC 'BSS'
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
;	COMDAT ??_C@_0BP@INFB@D?3?2_CODE?2Shared?2GraphicsMisc?4h?$AA@
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
;	COMDAT ?VerifyDeleteObject@GraphicsMisc@@YAHPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?VerifyDeleteObject@GraphicsMisc@@YAHAAVCGdiObject@@@Z
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
;	COMDAT ?Luminance@RGBX@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MakeGray@RGBX@@QAEXNNN@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Hue2Triplet@RGBX@@KAEMMM@Z
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
;	COMDAT ?PointsPerInch@@YAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawGradient@GraphicsMisc@@YAHW4GM_GRADIENT@@PAVCDC@@PBUtagRECT@@KKHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetGradientType@GraphicsMisc@@YA?AW4GM_GRADIENT@@HH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetGradientEdgeColor@GraphicsMisc@@YAKW4GM_GRADIENT@@KH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawGlass@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawGlassWithGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateFontW@GraphicsMisc@@YAPAUHFONT__@@PAU2@KK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateFontW@GraphicsMisc@@YAHAAVCFont@@PAUHFONT__@@KK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateFontW@GraphicsMisc@@YAPAUHFONT__@@PBGHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CreateFontW@GraphicsMisc@@YAHAAVCFont@@PBGHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFontFlags@GraphicsMisc@@YAKPAUHFONT__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFontNameAndPointSize@GraphicsMisc@@YAHPAUHFONT__@@AAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PointToPixel@GraphicsMisc@@YAHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PixelToPoint@GraphicsMisc@@YAHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PixelsPerInch@GraphicsMisc@@YAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFontPointSize@GraphicsMisc@@YAHPAUHFONT__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFontPixelSize@GraphicsMisc@@YAHPAUHFONT__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SameFont@GraphicsMisc@@YAHPAUHFONT__@@PBGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SameFontNameSize@GraphicsMisc@@YAHPAUHFONT__@@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadIconW@GraphicsMisc@@YAPAUHICON__@@IH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HandCursor@GraphicsMisc@@YAPAUHICON__@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?WingDings@GraphicsMisc@@YAAAVCFont@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E282
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Marlett@GraphicsMisc@@YAAAVCFont@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E284
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTextWidth@GraphicsMisc@@YAHIAAVCWnd@@PAVCFont@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTextWidth@GraphicsMisc@@YAHABVCString@@AAVCWnd@@PAVCFont@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTextWidth@GraphicsMisc@@YAHPAVCDC@@PBGZZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAverageCharWidth@GraphicsMisc@@YAMPAVCDC@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAverageStringWidth@GraphicsMisc@@YAHABVCString@@PAVCDC@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAverageMaxStringWidth@GraphicsMisc@@YAHABVCString@@PAVCDC@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PrepareDCFont@GraphicsMisc@@YAPAVCFont@@PAVCDC@@PAVCWnd@@PAV2@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetBestTextColor@GraphicsMisc@@YAKK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lighter@GraphicsMisc@@YAKKN@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Darker@GraphicsMisc@@YAKKN@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Blend@GraphicsMisc@@YAKKKN@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetScreenWorkArea@GraphicsMisc@@YAHPAUHWND__@@AAVCRect@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ForceIconicRepresentation@GraphicsMisc@@YAHPAUHWND__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EnableAeroPeek@GraphicsMisc@@YAHPAUHWND__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EnableFlip3D@GraphicsMisc@@YAHPAUHWND__@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DwmSetWindowAttribute@GraphicsMisc@@YAHPAUHWND__@@KPBXK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawSymbol@GraphicsMisc@@YAHPAVCDC@@DABVCRect@@IPAVCFont@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawRect@GraphicsMisc@@YAXPAVCDC@@ABVCRect@@KKHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAvailableScreenSpace@GraphicsMisc@@YAHABVCRect@@AAV2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAvailableScreenSpace@GraphicsMisc@@YAHPAUHWND__@@AAVCRect@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAvailableScreenSpace@GraphicsMisc@@YAHAAVCRect@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawHorzLine@GraphicsMisc@@YAXPAVCDC@@HHHKK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DrawVertLine@GraphicsMisc@@YAXPAVCDC@@HHHKK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetRTLDrawTextFlags@GraphicsMisc@@YAIPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetRTLDrawTextFlags@GraphicsMisc@@YAIPAVCDC@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetWebColor@GraphicsMisc@@YA?AVCString@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetIconSize@GraphicsMisc@@YA?AVCSize@@PAUHICON__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetBitmapSize@GraphicsMisc@@YA?AVCSize@@PAUHBITMAP__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InitBitmapInfo@@YAXPAUtagBITMAPINFO@@KJJG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Create32BitHBITMAP@@YAHPAUHDC__@@PBUtagSIZE@@PAPAXPAPAUHBITMAP__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HasAlpha@@YAHPAKAAUtagSIZE@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConvertToPARGB32@@YAHPAUHDC__@@PAKPAUHBITMAP__@@AAUtagSIZE@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConvertBufferToPARGB32@@YAHPAXPAUHDC__@@PAUHICON__@@AAUtagSIZE@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IconToPARGB32Bitmap@GraphicsMisc@@YAPAUHBITMAP__@@PAUHICON__@@@Z
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
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
_BSS	SEGMENT
_?cursor@?1??HandCursor@GraphicsMisc@@YAPAUHICON__@@XZ@4PAU3@A DD 01H DUP (?)
_BSS	ENDS
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\Shared\GraphicsMisc.cpp', 00H
_DATA	ENDS
PUBLIC	?DrawGradient@GraphicsMisc@@YAHW4GM_GRADIENT@@PAVCDC@@PBUtagRECT@@KKHH@Z ; GraphicsMisc::DrawGradient
PUBLIC	?DrawGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z ; GraphicsMisc::DrawGradient
PUBLIC	?DrawGlass@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z ; GraphicsMisc::DrawGlass
PUBLIC	?DrawGlassWithGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z ; GraphicsMisc::DrawGlassWithGradient
EXTRN	__chkesp:NEAR
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
;	COMDAT ?DrawGradient@GraphicsMisc@@YAHW4GM_GRADIENT@@PAVCDC@@PBUtagRECT@@KKHH@Z
_TEXT	SEGMENT
_nType$ = 8
_pDC$ = 12
_pRect$ = 16
_crFrom$ = 20
_crTo$ = 24
_bHorz$ = 28
_nBorder$ = 32
?DrawGradient@GraphicsMisc@@YAHW4GM_GRADIENT@@PAVCDC@@PBUtagRECT@@KKHH@Z PROC NEAR ; GraphicsMisc::DrawGradient, COMDAT

; 50   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 52   : 	{

	mov	eax, DWORD PTR _nType$[ebp]
	mov	DWORD PTR -4+[ebp], eax
	cmp	DWORD PTR -4+[ebp], 1
	je	SHORT $L76217
	cmp	DWORD PTR -4+[ebp], 2
	je	SHORT $L76218
	cmp	DWORD PTR -4+[ebp], 3
	je	SHORT $L76219
	jmp	SHORT $L76214
$L76217:

; 53   : 	case GMG_GRADIENT:
; 54   : 		DrawGradient(pDC, pRect, crFrom, crTo, bHorz, nBorder);

	mov	ecx, DWORD PTR _nBorder$[ebp]
	push	ecx
	mov	edx, DWORD PTR _bHorz$[ebp]
	push	edx
	mov	eax, DWORD PTR _crTo$[ebp]
	push	eax
	mov	ecx, DWORD PTR _crFrom$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pRect$[ebp]
	push	edx
	mov	eax, DWORD PTR _pDC$[ebp]
	push	eax
	call	?DrawGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z ; GraphicsMisc::DrawGradient
	add	esp, 24					; 00000018H

; 55   : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76212
$L76218:

; 56   : 
; 57   : 	case GMG_GLASS:
; 58   : 		DrawGlass(pDC, pRect, crFrom, crTo, bHorz, nBorder);

	mov	ecx, DWORD PTR _nBorder$[ebp]
	push	ecx
	mov	edx, DWORD PTR _bHorz$[ebp]
	push	edx
	mov	eax, DWORD PTR _crTo$[ebp]
	push	eax
	mov	ecx, DWORD PTR _crFrom$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pRect$[ebp]
	push	edx
	mov	eax, DWORD PTR _pDC$[ebp]
	push	eax
	call	?DrawGlass@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z ; GraphicsMisc::DrawGlass
	add	esp, 24					; 00000018H

; 59   : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76212
$L76219:

; 60   : 
; 61   : 	case GMG_GLASSWITHGRADIENT:
; 62   : 		DrawGlassWithGradient(pDC, pRect, crFrom, crTo, bHorz, nBorder);

	mov	ecx, DWORD PTR _nBorder$[ebp]
	push	ecx
	mov	edx, DWORD PTR _bHorz$[ebp]
	push	edx
	mov	eax, DWORD PTR _crTo$[ebp]
	push	eax
	mov	ecx, DWORD PTR _crFrom$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pRect$[ebp]
	push	edx
	mov	eax, DWORD PTR _pDC$[ebp]
	push	eax
	call	?DrawGlassWithGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z ; GraphicsMisc::DrawGlassWithGradient
	add	esp, 24					; 00000018H

; 63   : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76212
$L76214:

; 65   : 	
; 66   : 	// all the rest
; 67   : 	ASSERT(0);

	mov	ecx, 1
	test	ecx, ecx
	je	SHORT $L76223
	push	67					; 00000043H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76223
	int	3
$L76223:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76214

; 68   : 	return FALSE;

	xor	eax, eax
$L76212:

; 69   : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?DrawGradient@GraphicsMisc@@YAHW4GM_GRADIENT@@PAVCDC@@PBUtagRECT@@KKHH@Z ENDP ; GraphicsMisc::DrawGradient
_TEXT	ENDS
PUBLIC	?GetGradientType@GraphicsMisc@@YA?AW4GM_GRADIENT@@HH@Z ; GraphicsMisc::GetGradientType
;	COMDAT ?GetGradientType@GraphicsMisc@@YA?AW4GM_GRADIENT@@HH@Z
_TEXT	SEGMENT
_bGlass$ = 8
_bGradient$ = 12
?GetGradientType@GraphicsMisc@@YA?AW4GM_GRADIENT@@HH@Z PROC NEAR ; GraphicsMisc::GetGradientType, COMDAT

; 72   : {

	push	ebp
	mov	ebp, esp

; 73   : 	if (bGradient && bGlass) 

	cmp	DWORD PTR _bGradient$[ebp], 0
	je	SHORT $L76228
	cmp	DWORD PTR _bGlass$[ebp], 0
	je	SHORT $L76228

; 75   : 		return GMG_GLASSWITHGRADIENT;

	mov	eax, 3
	jmp	SHORT $L76227
$L76228:

; 77   : 	else if (bGradient) 

	cmp	DWORD PTR _bGradient$[ebp], 0
	je	SHORT $L76230

; 79   : 		return GMG_GRADIENT;

	mov	eax, 1
	jmp	SHORT $L76227
$L76230:

; 81   : 	else if (bGlass)

	cmp	DWORD PTR _bGlass$[ebp], 0
	je	SHORT $L76232

; 83   : 		return GMG_GLASS;

	mov	eax, 2
	jmp	SHORT $L76227
$L76232:

; 85   : 
; 86   : 	// else
; 87   : 	return GMG_NONE;

	xor	eax, eax
$L76227:

; 88   : }

	pop	ebp
	ret	0
?GetGradientType@GraphicsMisc@@YA?AW4GM_GRADIENT@@HH@Z ENDP ; GraphicsMisc::GetGradientType
_TEXT	ENDS
PUBLIC	?GetGradientEdgeColor@GraphicsMisc@@YAKW4GM_GRADIENT@@KH@Z ; GraphicsMisc::GetGradientEdgeColor
PUBLIC	?Lighter@GraphicsMisc@@YAKKN@Z			; GraphicsMisc::Lighter
EXTRN	__fltused:NEAR
;	COMDAT ?GetGradientEdgeColor@GraphicsMisc@@YAKW4GM_GRADIENT@@KH@Z
_TEXT	SEGMENT
_nType$ = 8
_color$ = 12
?GetGradientEdgeColor@GraphicsMisc@@YAKW4GM_GRADIENT@@KH@Z PROC NEAR ; GraphicsMisc::GetGradientEdgeColor, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 93   : 	{

	mov	eax, DWORD PTR _nType$[ebp]
	mov	DWORD PTR -4+[ebp], eax
	cmp	DWORD PTR -4+[ebp], 1
	je	SHORT $L76242
	cmp	DWORD PTR -4+[ebp], 2
	je	SHORT $L76243
	cmp	DWORD PTR -4+[ebp], 3
	je	SHORT $L76242
	jmp	SHORT $L76239
$L76242:

; 94   : 	case GMG_GRADIENT:
; 95   : 	case GMG_GLASSWITHGRADIENT:
; 96   : 		return color;

	mov	eax, DWORD PTR _color$[ebp]
	jmp	SHORT $L76237
$L76243:

; 97   : 		
; 98   : 	case GMG_GLASS:
; 99   : 		return Lighter(color, 0.2);

	push	1070176665				; 3fc99999H
	push	-1717986918				; 9999999aH
	mov	ecx, DWORD PTR _color$[ebp]
	push	ecx
	call	?Lighter@GraphicsMisc@@YAKKN@Z		; GraphicsMisc::Lighter
	add	esp, 12					; 0000000cH
	jmp	SHORT $L76237
$L76239:

; 101  : 
; 102  : 	// all the rest
; 103  : 	ASSERT(0);

	mov	edx, 1
	test	edx, edx
	je	SHORT $L76247
	push	103					; 00000067H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76247
	int	3
$L76247:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76239

; 104  : 	return color;

	mov	eax, DWORD PTR _color$[ebp]
$L76237:

; 105  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetGradientEdgeColor@GraphicsMisc@@YAKW4GM_GRADIENT@@KH@Z ENDP ; GraphicsMisc::GetGradientEdgeColor
_TEXT	ENDS
PUBLIC	?DrawRect@GraphicsMisc@@YAXPAVCDC@@ABVCRect@@KKHK@Z ; GraphicsMisc::DrawRect
EXTRN	?GetSafeHdc@CDC@@QBEPAUHDC__@@XZ:NEAR		; CDC::GetSafeHdc
EXTRN	??0CRect@@QAE@PBUtagRECT@@@Z:NEAR		; CRect::CRect
EXTRN	__imp__GradientFill@24:NEAR
EXTRN	?DeflateRect@CRect@@QAEXHH@Z:NEAR		; CRect::DeflateRect
;	COMDAT ?DrawGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z
_TEXT	SEGMENT
_pDC$ = 8
_pRect$ = 12
_crFrom$ = 16
_crTo$ = 20
_bHorz$ = 24
_nBorder$ = 28
_vert$ = -32
_gRect$ = -40
_rBorder$76291 = -56
?DrawGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z PROC NEAR ; GraphicsMisc::DrawGradient, COMDAT

; 108  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 56					; 00000038H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-56]
	mov	ecx, 14					; 0000000eH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 109  : 	TRIVERTEX vert[2];
; 110  : 	GRADIENT_RECT gRect;
; 111  : 
; 112  : 	vert[0] .x      = pRect->left;

	mov	eax, DWORD PTR _pRect$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _vert$[ebp], ecx

; 113  : 	vert[0] .y      = pRect->top;

	mov	edx, DWORD PTR _pRect$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR _vert$[ebp+4], eax

; 114  : 	vert[0] .Red    = MAKEWORD(0, GetRValue(crFrom));

	mov	ecx, DWORD PTR _crFrom$[ebp]
	and	ecx, 255				; 000000ffH
	shl	ecx, 8
	mov	WORD PTR _vert$[ebp+8], cx

; 115  : 	vert[0] .Green  = MAKEWORD(0, GetGValue(crFrom));

	mov	edx, DWORD PTR _crFrom$[ebp]
	and	edx, 65535				; 0000ffffH
	sar	edx, 8
	and	edx, 255				; 000000ffH
	shl	edx, 8
	mov	WORD PTR _vert$[ebp+10], dx

; 116  : 	vert[0] .Blue   = MAKEWORD(0, GetBValue(crFrom));

	mov	eax, DWORD PTR _crFrom$[ebp]
	shr	eax, 16					; 00000010H
	and	eax, 255				; 000000ffH
	shl	eax, 8
	mov	WORD PTR _vert$[ebp+12], ax

; 117  : 	vert[0] .Alpha  = 0x0000;

	mov	WORD PTR _vert$[ebp+14], 0

; 118  : 	vert[1] .x      = pRect->right;

	mov	ecx, DWORD PTR _pRect$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	DWORD PTR _vert$[ebp+16], edx

; 119  : 	vert[1] .y      = pRect->bottom; 

	mov	eax, DWORD PTR _pRect$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR _vert$[ebp+20], ecx

; 120  : 	vert[1] .Red    = MAKEWORD(0, GetRValue(crTo));

	mov	edx, DWORD PTR _crTo$[ebp]
	and	edx, 255				; 000000ffH
	shl	edx, 8
	mov	WORD PTR _vert$[ebp+24], dx

; 121  : 	vert[1] .Green  = MAKEWORD(0, GetGValue(crTo));

	mov	eax, DWORD PTR _crTo$[ebp]
	and	eax, 65535				; 0000ffffH
	sar	eax, 8
	and	eax, 255				; 000000ffH
	shl	eax, 8
	mov	WORD PTR _vert$[ebp+26], ax

; 122  : 	vert[1] .Blue   = MAKEWORD(0, GetBValue(crTo));

	mov	ecx, DWORD PTR _crTo$[ebp]
	shr	ecx, 16					; 00000010H
	and	ecx, 255				; 000000ffH
	shl	ecx, 8
	mov	WORD PTR _vert$[ebp+28], cx

; 123  : 	vert[1] .Alpha  = 0x0000;

	mov	WORD PTR _vert$[ebp+30], 0

; 124  : 	gRect.UpperLeft  = 0;

	mov	DWORD PTR _gRect$[ebp], 0

; 125  : 	gRect.LowerRight = 1;

	mov	DWORD PTR _gRect$[ebp+4], 1

; 126  : 
; 127  : 	GradientFill(pDC->GetSafeHdc(), vert, 2, &gRect, 1, bHorz ? GRADIENT_FILL_RECT_H : GRADIENT_FILL_RECT_V);

	xor	edx, edx
	cmp	DWORD PTR _bHorz$[ebp], 0
	sete	dl
	mov	esi, esp
	push	edx
	push	1
	lea	eax, DWORD PTR _gRect$[ebp]
	push	eax
	push	2
	lea	ecx, DWORD PTR _vert$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetSafeHdc@CDC@@QBEPAUHDC__@@XZ	; CDC::GetSafeHdc
	push	eax
	call	DWORD PTR __imp__GradientFill@24
	cmp	esi, esp
	call	__chkesp

; 128  : 
; 129  : 	if (nBorder >= 0)

	cmp	DWORD PTR _nBorder$[ebp], 0
	jl	SHORT $L76290

; 131  : 		// draw a border in from the edge
; 132  : 		CRect rBorder(pRect);

	mov	edx, DWORD PTR _pRect$[ebp]
	push	edx
	lea	ecx, DWORD PTR _rBorder$76291[ebp]
	call	??0CRect@@QAE@PBUtagRECT@@@Z		; CRect::CRect

; 133  : 		rBorder.DeflateRect(nBorder, nBorder);

	mov	eax, DWORD PTR _nBorder$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nBorder$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _rBorder$76291[ebp]
	call	?DeflateRect@CRect@@QAEXHH@Z		; CRect::DeflateRect

; 134  : 		DrawRect(pDC, rBorder, CLR_NONE, crFrom);

	push	15					; 0000000fH
	push	0
	mov	edx, DWORD PTR _crFrom$[ebp]
	push	edx
	push	-1
	lea	eax, DWORD PTR _rBorder$76291[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	push	ecx
	call	?DrawRect@GraphicsMisc@@YAXPAVCDC@@ABVCRect@@KKHK@Z ; GraphicsMisc::DrawRect
	add	esp, 24					; 00000018H
$L76290:

; 136  : }

	pop	edi
	pop	esi
	add	esp, 56					; 00000038H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?DrawGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z ENDP ; GraphicsMisc::DrawGradient
_TEXT	ENDS
EXTRN	?Width@CRect@@QBEHXZ:NEAR			; CRect::Width
EXTRN	?Height@CRect@@QBEHXZ:NEAR			; CRect::Height
EXTRN	??BCRect@@QAEPAUtagRECT@@XZ:NEAR		; CRect::operator tagRECT *
;	COMDAT ?DrawGlass@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z
_TEXT	SEGMENT
_pDC$ = 8
_pRect$ = 12
_crFrom$ = 16
_crTo$ = 20
_bHorz$ = 24
_nBorder$ = 28
_rBarFrom$ = -32
_rBarTo$ = -16
_rBorder$76305 = -48
?DrawGlass@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z PROC NEAR ; GraphicsMisc::DrawGlass, COMDAT

; 139  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 48					; 00000030H
	push	edi
	lea	edi, DWORD PTR [ebp-48]
	mov	ecx, 12					; 0000000cH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 140  : 	CRect rBarFrom(pRect), rBarTo(pRect);

	mov	eax, DWORD PTR _pRect$[ebp]
	push	eax
	lea	ecx, DWORD PTR _rBarFrom$[ebp]
	call	??0CRect@@QAE@PBUtagRECT@@@Z		; CRect::CRect
	mov	ecx, DWORD PTR _pRect$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _rBarTo$[ebp]
	call	??0CRect@@QAE@PBUtagRECT@@@Z		; CRect::CRect

; 141  : 
; 142  : 	if (bHorz)

	cmp	DWORD PTR _bHorz$[ebp], 0
	je	SHORT $L76302

; 144  : 		rBarFrom.right = rBarFrom.left + (rBarFrom.Width() * 2 / 5);

	lea	ecx, DWORD PTR _rBarFrom$[ebp]
	call	?Width@CRect@@QBEHXZ			; CRect::Width
	shl	eax, 1
	cdq
	mov	ecx, 5
	idiv	ecx
	mov	edx, DWORD PTR _rBarFrom$[ebp]
	add	edx, eax
	mov	DWORD PTR _rBarFrom$[ebp+8], edx

; 145  : 		rBarTo.left = rBarFrom.right;

	mov	eax, DWORD PTR _rBarFrom$[ebp+8]
	mov	DWORD PTR _rBarTo$[ebp], eax

; 147  : 	else // vert

	jmp	SHORT $L76303
$L76302:

; 149  : 		rBarFrom.bottom = rBarFrom.top + (rBarFrom.Height() * 2 / 5);

	lea	ecx, DWORD PTR _rBarFrom$[ebp]
	call	?Height@CRect@@QBEHXZ			; CRect::Height
	shl	eax, 1
	cdq
	mov	ecx, 5
	idiv	ecx
	mov	edx, DWORD PTR _rBarFrom$[ebp+4]
	add	edx, eax
	mov	DWORD PTR _rBarFrom$[ebp+12], edx

; 150  : 		rBarTo.top = rBarFrom.bottom;

	mov	eax, DWORD PTR _rBarFrom$[ebp+12]
	mov	DWORD PTR _rBarTo$[ebp+4], eax
$L76303:

; 152  : 
; 153  : 	DrawGradient(pDC, rBarFrom, Lighter(crFrom, 0.2), crFrom, bHorz, -1);

	push	-1
	mov	ecx, DWORD PTR _bHorz$[ebp]
	push	ecx
	mov	edx, DWORD PTR _crFrom$[ebp]
	push	edx
	push	1070176665				; 3fc99999H
	push	-1717986918				; 9999999aH
	mov	eax, DWORD PTR _crFrom$[ebp]
	push	eax
	call	?Lighter@GraphicsMisc@@YAKKN@Z		; GraphicsMisc::Lighter
	add	esp, 12					; 0000000cH
	push	eax
	lea	ecx, DWORD PTR _rBarFrom$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	push	ecx
	call	?DrawGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z ; GraphicsMisc::DrawGradient
	add	esp, 24					; 00000018H

; 154  : 	DrawGradient(pDC, rBarTo, crTo, Lighter(crTo, 0.2), bHorz, -1);

	push	-1
	mov	edx, DWORD PTR _bHorz$[ebp]
	push	edx
	push	1070176665				; 3fc99999H
	push	-1717986918				; 9999999aH
	mov	eax, DWORD PTR _crTo$[ebp]
	push	eax
	call	?Lighter@GraphicsMisc@@YAKKN@Z		; GraphicsMisc::Lighter
	add	esp, 12					; 0000000cH
	push	eax
	mov	ecx, DWORD PTR _crTo$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _rBarTo$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	edx, DWORD PTR _pDC$[ebp]
	push	edx
	call	?DrawGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z ; GraphicsMisc::DrawGradient
	add	esp, 24					; 00000018H

; 155  : 
; 156  : 	if (nBorder >= 0)

	cmp	DWORD PTR _nBorder$[ebp], 0
	jl	SHORT $L76304

; 158  : 		// draw a border in from the edge
; 159  : 		CRect rBorder(pRect);

	mov	eax, DWORD PTR _pRect$[ebp]
	push	eax
	lea	ecx, DWORD PTR _rBorder$76305[ebp]
	call	??0CRect@@QAE@PBUtagRECT@@@Z		; CRect::CRect

; 160  : 		rBorder.DeflateRect(nBorder, nBorder);

	mov	ecx, DWORD PTR _nBorder$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nBorder$[ebp]
	push	edx
	lea	ecx, DWORD PTR _rBorder$76305[ebp]
	call	?DeflateRect@CRect@@QAEXHH@Z		; CRect::DeflateRect

; 161  : 		DrawRect(pDC, rBorder, CLR_NONE, crFrom);

	push	15					; 0000000fH
	push	0
	mov	eax, DWORD PTR _crFrom$[ebp]
	push	eax
	push	-1
	lea	ecx, DWORD PTR _rBorder$76305[ebp]
	push	ecx
	mov	edx, DWORD PTR _pDC$[ebp]
	push	edx
	call	?DrawRect@GraphicsMisc@@YAXPAVCDC@@ABVCRect@@KKHK@Z ; GraphicsMisc::DrawRect
	add	esp, 24					; 00000018H
$L76304:

; 163  : }

	pop	edi
	add	esp, 48					; 00000030H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?DrawGlass@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z ENDP ; GraphicsMisc::DrawGlass
_TEXT	ENDS
EXTRN	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z:NEAR	; CDC::FillSolidRect
;	COMDAT ?DrawGlassWithGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z
_TEXT	SEGMENT
_pDC$ = 8
_pRect$ = 12
_crFrom$ = 16
_crTo$ = 20
_bHorz$ = 24
_nBorder$ = 28
_rBarFrom$ = -48
_rBarTo$ = -16
_rGrad$ = -32
_rBorder$76322 = -64
?DrawGlassWithGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z PROC NEAR ; GraphicsMisc::DrawGlassWithGradient, COMDAT

; 166  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 64					; 00000040H
	push	edi
	lea	edi, DWORD PTR [ebp-64]
	mov	ecx, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 167  : 	// draw the glass first
; 168  : 	CRect rBarFrom(pRect), rBarTo(pRect);

	mov	eax, DWORD PTR _pRect$[ebp]
	push	eax
	lea	ecx, DWORD PTR _rBarFrom$[ebp]
	call	??0CRect@@QAE@PBUtagRECT@@@Z		; CRect::CRect
	mov	ecx, DWORD PTR _pRect$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _rBarTo$[ebp]
	call	??0CRect@@QAE@PBUtagRECT@@@Z		; CRect::CRect

; 169  : 
; 170  : 	if (bHorz)

	cmp	DWORD PTR _bHorz$[ebp], 0
	je	SHORT $L76316

; 172  : 		rBarFrom.right = rBarFrom.left + (rBarFrom.Width() * 2 / 10);

	lea	ecx, DWORD PTR _rBarFrom$[ebp]
	call	?Width@CRect@@QBEHXZ			; CRect::Width
	shl	eax, 1
	cdq
	mov	ecx, 10					; 0000000aH
	idiv	ecx
	mov	edx, DWORD PTR _rBarFrom$[ebp]
	add	edx, eax
	mov	DWORD PTR _rBarFrom$[ebp+8], edx

; 173  : 		rBarTo.left = rBarTo.right - (rBarTo.Width() * 4 / 10);

	lea	ecx, DWORD PTR _rBarTo$[ebp]
	call	?Width@CRect@@QBEHXZ			; CRect::Width
	shl	eax, 2
	cdq
	mov	ecx, 10					; 0000000aH
	idiv	ecx
	mov	edx, DWORD PTR _rBarTo$[ebp+8]
	sub	edx, eax
	mov	DWORD PTR _rBarTo$[ebp], edx

; 175  : 	else // vert

	jmp	SHORT $L76317
$L76316:

; 177  : 		rBarFrom.bottom = rBarFrom.top + (rBarFrom.Height() * 2 / 10);

	lea	ecx, DWORD PTR _rBarFrom$[ebp]
	call	?Height@CRect@@QBEHXZ			; CRect::Height
	shl	eax, 1
	cdq
	mov	ecx, 10					; 0000000aH
	idiv	ecx
	mov	edx, DWORD PTR _rBarFrom$[ebp+4]
	add	edx, eax
	mov	DWORD PTR _rBarFrom$[ebp+12], edx

; 178  : 		rBarTo.top = rBarTo.bottom - (rBarTo.Height() * 4 / 10);

	lea	ecx, DWORD PTR _rBarTo$[ebp]
	call	?Height@CRect@@QBEHXZ			; CRect::Height
	shl	eax, 2
	cdq
	mov	ecx, 10					; 0000000aH
	idiv	ecx
	mov	edx, DWORD PTR _rBarTo$[ebp+12]
	sub	edx, eax
	mov	DWORD PTR _rBarTo$[ebp+4], edx
$L76317:

; 180  : 
; 181  : 	pDC->FillSolidRect(rBarFrom, crFrom);

	mov	eax, DWORD PTR _crFrom$[ebp]
	push	eax
	lea	ecx, DWORD PTR _rBarFrom$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect

; 182  : 	pDC->FillSolidRect(rBarTo, crTo);

	mov	ecx, DWORD PTR _crTo$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _rBarTo$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect

; 183  : 
; 184  : 	// then the gradient
; 185  : 	CRect rGrad(pRect);

	mov	edx, DWORD PTR _pRect$[ebp]
	push	edx
	lea	ecx, DWORD PTR _rGrad$[ebp]
	call	??0CRect@@QAE@PBUtagRECT@@@Z		; CRect::CRect

; 186  : 
; 187  : 	if (bHorz)

	cmp	DWORD PTR _bHorz$[ebp], 0
	je	SHORT $L76319

; 189  : 		rGrad.left = rBarFrom.right;

	mov	eax, DWORD PTR _rBarFrom$[ebp+8]
	mov	DWORD PTR _rGrad$[ebp], eax

; 190  : 		rGrad.right = rBarTo.left;

	mov	ecx, DWORD PTR _rBarTo$[ebp]
	mov	DWORD PTR _rGrad$[ebp+8], ecx

; 192  : 	else

	jmp	SHORT $L76320
$L76319:

; 194  : 		rGrad.top = rBarFrom.bottom;

	mov	edx, DWORD PTR _rBarFrom$[ebp+12]
	mov	DWORD PTR _rGrad$[ebp+4], edx

; 195  : 		rGrad.bottom = rBarTo.top;

	mov	eax, DWORD PTR _rBarTo$[ebp+4]
	mov	DWORD PTR _rGrad$[ebp+12], eax
$L76320:

; 197  : 
; 198  : 	DrawGradient(pDC, rGrad, crFrom, crTo, bHorz, -1); // no border

	push	-1
	mov	ecx, DWORD PTR _bHorz$[ebp]
	push	ecx
	mov	edx, DWORD PTR _crTo$[ebp]
	push	edx
	mov	eax, DWORD PTR _crFrom$[ebp]
	push	eax
	lea	ecx, DWORD PTR _rGrad$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	push	ecx
	call	?DrawGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z ; GraphicsMisc::DrawGradient
	add	esp, 24					; 00000018H

; 199  : 
; 200  : 	if (nBorder >= 0)

	cmp	DWORD PTR _nBorder$[ebp], 0
	jl	SHORT $L76321

; 202  : 		// draw a border in from the edge
; 203  : 		CRect rBorder(pRect);

	mov	edx, DWORD PTR _pRect$[ebp]
	push	edx
	lea	ecx, DWORD PTR _rBorder$76322[ebp]
	call	??0CRect@@QAE@PBUtagRECT@@@Z		; CRect::CRect

; 204  : 		rBorder.DeflateRect(nBorder, nBorder);

	mov	eax, DWORD PTR _nBorder$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nBorder$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _rBorder$76322[ebp]
	call	?DeflateRect@CRect@@QAEXHH@Z		; CRect::DeflateRect

; 205  : 		DrawRect(pDC, rBorder, CLR_NONE, crFrom);

	push	15					; 0000000fH
	push	0
	mov	edx, DWORD PTR _crFrom$[ebp]
	push	edx
	push	-1
	lea	eax, DWORD PTR _rBorder$76322[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	push	ecx
	call	?DrawRect@GraphicsMisc@@YAXPAVCDC@@ABVCRect@@KKHK@Z ; GraphicsMisc::DrawRect
	add	esp, 24					; 00000018H
$L76321:

; 207  : }

	pop	edi
	add	esp, 64					; 00000040H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?DrawGlassWithGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z ENDP ; GraphicsMisc::DrawGlassWithGradient
_TEXT	ENDS
PUBLIC	?CreateFontW@GraphicsMisc@@YAPAUHFONT__@@PAU2@KK@Z ; GraphicsMisc::CreateFontW
PUBLIC	?SameFontNameSize@GraphicsMisc@@YAHPAUHFONT__@@0@Z ; GraphicsMisc::SameFontNameSize
PUBLIC	?VerifyDeleteObject@GraphicsMisc@@YAHPAX@Z	; GraphicsMisc::VerifyDeleteObject
EXTRN	__imp__GetStockObject@4:NEAR
EXTRN	__imp__GetObjectW@12:NEAR
EXTRN	__imp__CreateFontIndirectW@4:NEAR
;	COMDAT ?CreateFontW@GraphicsMisc@@YAPAUHFONT__@@PAU2@KK@Z
_TEXT	SEGMENT
_hFont$ = 8
_dwFlags$ = 12
_dwMask$ = 16
_lf$ = -96
_hFontOut$ = -4
?CreateFontW@GraphicsMisc@@YAPAUHFONT__@@PAU2@KK@Z PROC NEAR ; GraphicsMisc::CreateFontW, COMDAT

; 210  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 96					; 00000060H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-96]
	mov	ecx, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 211  : 	if (hFont == NULL)

	cmp	DWORD PTR _hFont$[ebp], 0
	jne	SHORT $L76328

; 212  : 		hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	mov	esi, esp
	push	17					; 00000011H
	call	DWORD PTR __imp__GetStockObject@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hFont$[ebp], eax
$L76328:

; 215  : 	::GetObject(hFont, sizeof(lf), &lf);

	mov	esi, esp
	lea	eax, DWORD PTR _lf$[ebp]
	push	eax
	push	92					; 0000005cH
	mov	ecx, DWORD PTR _hFont$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetObjectW@12
	cmp	esi, esp
	call	__chkesp

; 216  : 	
; 217  : 	if (dwMask & GMFS_UNDERLINED)

	mov	edx, DWORD PTR _dwMask$[ebp]
	and	edx, 4
	test	edx, edx
	je	SHORT $L76331

; 218  : 		lf.lfUnderline = (BYTE)(dwFlags & GMFS_UNDERLINED);

	mov	eax, DWORD PTR _dwFlags$[ebp]
	and	eax, 4
	mov	BYTE PTR _lf$[ebp+21], al
$L76331:

; 219  : 	
; 220  : 	if (dwMask & GMFS_ITALIC)

	mov	ecx, DWORD PTR _dwMask$[ebp]
	and	ecx, 2
	test	ecx, ecx
	je	SHORT $L76333

; 221  : 		lf.lfItalic = (BYTE)(dwFlags & GMFS_ITALIC);

	mov	edx, DWORD PTR _dwFlags$[ebp]
	and	edx, 2
	mov	BYTE PTR _lf$[ebp+20], dl
$L76333:

; 222  : 	
; 223  : 	if (dwMask & GMFS_STRIKETHRU)

	mov	eax, DWORD PTR _dwMask$[ebp]
	and	eax, 8
	test	eax, eax
	je	SHORT $L76335

; 224  : 		lf.lfStrikeOut = (BYTE)(dwFlags & GMFS_STRIKETHRU);

	mov	ecx, DWORD PTR _dwFlags$[ebp]
	and	ecx, 8
	mov	BYTE PTR _lf$[ebp+22], cl
$L76335:

; 225  : 	
; 226  : 	if (dwMask & GMFS_BOLD)

	mov	edx, DWORD PTR _dwMask$[ebp]
	and	edx, 1
	test	edx, edx
	je	SHORT $L76337

; 227  : 		lf.lfWeight = (dwFlags & GMFS_BOLD) ? FW_BOLD : FW_NORMAL;

	mov	eax, DWORD PTR _dwFlags$[ebp]
	and	eax, 1
	neg	eax
	sbb	eax, eax
	and	eax, 300				; 0000012cH
	add	eax, 400				; 00000190H
	mov	DWORD PTR _lf$[ebp+16], eax
$L76337:

; 228  : 	
; 229  : 	HFONT hFontOut = CreateFontIndirect(&lf);

	mov	esi, esp
	lea	ecx, DWORD PTR _lf$[ebp]
	push	ecx
	call	DWORD PTR __imp__CreateFontIndirectW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hFontOut$[ebp], eax

; 230  : 	
; 231  : 	// verify the font creation
; 232  : 	if (!SameFontNameSize(hFont, hFontOut))

	mov	edx, DWORD PTR _hFontOut$[ebp]
	push	edx
	mov	eax, DWORD PTR _hFont$[ebp]
	push	eax
	call	?SameFontNameSize@GraphicsMisc@@YAHPAUHFONT__@@0@Z ; GraphicsMisc::SameFontNameSize
	add	esp, 8
	test	eax, eax
	jne	SHORT $L76339

; 234  : 		VerifyDeleteObject(hFontOut);

	mov	ecx, DWORD PTR _hFontOut$[ebp]
	push	ecx
	call	?VerifyDeleteObject@GraphicsMisc@@YAHPAX@Z ; GraphicsMisc::VerifyDeleteObject
	add	esp, 4

; 235  : 		hFontOut = NULL;

	mov	DWORD PTR _hFontOut$[ebp], 0
$L76339:

; 237  : 	
; 238  : 	return hFontOut;

	mov	eax, DWORD PTR _hFontOut$[ebp]

; 239  : }

	pop	edi
	pop	esi
	add	esp, 96					; 00000060H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?CreateFontW@GraphicsMisc@@YAPAUHFONT__@@PAU2@KK@Z ENDP	; GraphicsMisc::CreateFontW
_TEXT	ENDS
PUBLIC	??_C@_0BP@INFB@D?3?2_CODE?2Shared?2GraphicsMisc?4h?$AA@ ; `string'
EXTRN	__imp__DeleteObject@4:NEAR
;	COMDAT ??_C@_0BP@INFB@D?3?2_CODE?2Shared?2GraphicsMisc?4h?$AA@
; File D:\_CODE\Shared\GraphicsMisc.h
_DATA	SEGMENT
??_C@_0BP@INFB@D?3?2_CODE?2Shared?2GraphicsMisc?4h?$AA@ DB 'D:\_CODE\Shar'
	DB	'ed\GraphicsMisc.h', 00H			; `string'
_DATA	ENDS
;	COMDAT ?VerifyDeleteObject@GraphicsMisc@@YAHPAX@Z
_TEXT	SEGMENT
_hObj$ = 8
?VerifyDeleteObject@GraphicsMisc@@YAHPAX@Z PROC NEAR	; GraphicsMisc::VerifyDeleteObject, COMDAT

; 87   : 	{

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi

; 88   : 		if (hObj == NULL)

	cmp	DWORD PTR _hObj$[ebp], 0
	jne	SHORT $L73193

; 89   : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L73192
$L73193:

; 90   : 
; 91   : 		// else
; 92   : 		if (::DeleteObject(hObj))

	mov	esi, esp
	mov	eax, DWORD PTR _hObj$[ebp]
	push	eax
	call	DWORD PTR __imp__DeleteObject@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L73194

; 93   : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L73192
$L73194:

; 94   : 
; 95   : 		// else
; 96   : 		ASSERT(0);

	mov	ecx, 1
	test	ecx, ecx
	je	SHORT $L73198
	push	96					; 00000060H
	push	OFFSET FLAT:??_C@_0BP@INFB@D?3?2_CODE?2Shared?2GraphicsMisc?4h?$AA@ ; `string'
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73198
	int	3
$L73198:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L73194

; 97   : 		return FALSE;

	xor	eax, eax
$L73192:

; 98   : 	}

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?VerifyDeleteObject@GraphicsMisc@@YAHPAX@Z ENDP		; GraphicsMisc::VerifyDeleteObject
_TEXT	ENDS
PUBLIC	?CreateFontW@GraphicsMisc@@YAHAAVCFont@@PAUHFONT__@@KK@Z ; GraphicsMisc::CreateFontW
PUBLIC	?VerifyDeleteObject@GraphicsMisc@@YAHAAVCGdiObject@@@Z ; GraphicsMisc::VerifyDeleteObject
EXTRN	?Attach@CGdiObject@@QAEHPAX@Z:NEAR		; CGdiObject::Attach
;	COMDAT ?CreateFontW@GraphicsMisc@@YAHAAVCFont@@PAUHFONT__@@KK@Z
_TEXT	SEGMENT
_fontOut$ = 8
_fontIn$ = 12
_dwFlags$ = 16
_dwMask$ = 20
?CreateFontW@GraphicsMisc@@YAHAAVCFont@@PAUHFONT__@@KK@Z PROC NEAR ; GraphicsMisc::CreateFontW, COMDAT

; 242  : {

	push	ebp
	mov	ebp, esp

; 243  : 	VerifyDeleteObject(fontOut);

	mov	eax, DWORD PTR _fontOut$[ebp]
	push	eax
	call	?VerifyDeleteObject@GraphicsMisc@@YAHAAVCGdiObject@@@Z ; GraphicsMisc::VerifyDeleteObject
	add	esp, 4

; 244  : 
; 245  : 	return fontOut.Attach(CreateFont(fontIn, dwFlags, dwMask));

	mov	ecx, DWORD PTR _dwMask$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwFlags$[ebp]
	push	edx
	mov	eax, DWORD PTR _fontIn$[ebp]
	push	eax
	call	?CreateFontW@GraphicsMisc@@YAPAUHFONT__@@PAU2@KK@Z ; GraphicsMisc::CreateFontW
	add	esp, 12					; 0000000cH
	push	eax
	mov	ecx, DWORD PTR _fontOut$[ebp]
	call	?Attach@CGdiObject@@QAEHPAX@Z		; CGdiObject::Attach

; 246  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?CreateFontW@GraphicsMisc@@YAHAAVCFont@@PAUHFONT__@@KK@Z ENDP ; GraphicsMisc::CreateFontW
_TEXT	ENDS
EXTRN	?DeleteObject@CGdiObject@@QAEHXZ:NEAR		; CGdiObject::DeleteObject
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
	jne	SHORT $L73204

; 103  : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L73203
$L73204:

; 104  : 
; 105  : 		// else
; 106  : 		if (obj.DeleteObject())

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?DeleteObject@CGdiObject@@QAEHXZ	; CGdiObject::DeleteObject
	test	eax, eax
	je	SHORT $L73205

; 107  : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L73203
$L73205:

; 108  : 
; 109  : 		// else
; 110  : 		ASSERT(0);

	mov	ecx, 1
	test	ecx, ecx
	je	SHORT $L73209
	push	110					; 0000006eH
	push	OFFSET FLAT:??_C@_0BP@INFB@D?3?2_CODE?2Shared?2GraphicsMisc?4h?$AA@ ; `string'
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73209
	int	3
$L73209:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L73205

; 111  : 		return FALSE;

	xor	eax, eax
$L73203:

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
PUBLIC	?CreateFontW@GraphicsMisc@@YAPAUHFONT__@@PBGHK@Z ; GraphicsMisc::CreateFontW
PUBLIC	?PointToPixel@GraphicsMisc@@YAHH@Z		; GraphicsMisc::PointToPixel
PUBLIC	?SameFont@GraphicsMisc@@YAHPAUHFONT__@@PBGH@Z	; GraphicsMisc::SameFont
EXTRN	__imp__MulDiv@12:NEAR
EXTRN	__imp__lstrcpyW@8:NEAR
;	COMDAT ?CreateFontW@GraphicsMisc@@YAPAUHFONT__@@PBGHK@Z
_TEXT	SEGMENT
_szFaceName$ = 8
_nPoint$ = 12
_dwFlags$ = 16
_hDefFont$ = -4
_lf$ = -100
_hFont$ = -8
?CreateFontW@GraphicsMisc@@YAPAUHFONT__@@PBGHK@Z PROC NEAR ; GraphicsMisc::CreateFontW, COMDAT

; 249  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 100				; 00000064H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-100]
	mov	ecx, 25					; 00000019H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 250  : 	HFONT hDefFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	mov	esi, esp
	push	17					; 00000011H
	call	DWORD PTR __imp__GetStockObject@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hDefFont$[ebp], eax
$L76353:

; 251  : 	ASSERT (hDefFont);

	cmp	DWORD PTR _hDefFont$[ebp], 0
	jne	SHORT $L76356
	push	251					; 000000fbH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76356
	int	3
$L76356:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76353

; 252  : 	
; 253  : 	LOGFONT lf;
; 254  : 	::GetObject(hDefFont, sizeof(lf), &lf);

	mov	esi, esp
	lea	ecx, DWORD PTR _lf$[ebp]
	push	ecx
	push	92					; 0000005cH
	mov	edx, DWORD PTR _hDefFont$[ebp]
	push	edx
	call	DWORD PTR __imp__GetObjectW@12
	cmp	esi, esp
	call	__chkesp

; 255  : 	
; 256  : 	// set the charset
; 257  : 	if (dwFlags & GMFS_SYMBOL)

	mov	eax, DWORD PTR _dwFlags$[ebp]
	and	eax, 16					; 00000010H
	test	eax, eax
	je	SHORT $L76358

; 259  : 		lf.lfCharSet = SYMBOL_CHARSET;

	mov	BYTE PTR _lf$[ebp+23], 2

; 261  : 	else if (!lf.lfCharSet)

	jmp	SHORT $L76360
$L76358:
	mov	ecx, DWORD PTR _lf$[ebp+23]
	and	ecx, 255				; 000000ffH
	test	ecx, ecx
	jne	SHORT $L76360

; 263  : 		lf.lfCharSet = DEFAULT_CHARSET;

	mov	BYTE PTR _lf$[ebp+23], 1
$L76360:

; 265  : 	
; 266  : 	if (szFaceName && *szFaceName)

	cmp	DWORD PTR _szFaceName$[ebp], 0
	je	SHORT $L76361
	mov	edx, DWORD PTR _szFaceName$[ebp]
	xor	eax, eax
	mov	ax, WORD PTR [edx]
	test	eax, eax
	je	SHORT $L76361

; 268  : 		lstrcpy(lf.lfFaceName, szFaceName);

	mov	esi, esp
	mov	ecx, DWORD PTR _szFaceName$[ebp]
	push	ecx
	lea	edx, DWORD PTR _lf$[ebp+28]
	push	edx
	call	DWORD PTR __imp__lstrcpyW@8
	cmp	esi, esp
	call	__chkesp

; 269  : 		
; 270  : 		// reset character-set 
; 271  : 		lf.lfCharSet = DEFAULT_CHARSET;

	mov	BYTE PTR _lf$[ebp+23], 1
$L76361:

; 273  : 	
; 274  : 	if (nPoint > 0)

	cmp	DWORD PTR _nPoint$[ebp], 0
	jle	SHORT $L76362

; 276  : 		lf.lfHeight = -PointToPixel(nPoint);

	mov	eax, DWORD PTR _nPoint$[ebp]
	push	eax
	call	?PointToPixel@GraphicsMisc@@YAHH@Z	; GraphicsMisc::PointToPixel
	add	esp, 4
	neg	eax
	mov	DWORD PTR _lf$[ebp], eax

; 278  : 	else if (dwFlags & GMFS_SYMBOL)

	jmp	SHORT $L76364
$L76362:
	mov	ecx, DWORD PTR _dwFlags$[ebp]
	and	ecx, 16					; 00000010H
	test	ecx, ecx
	je	SHORT $L76364

; 280  : 		lf.lfHeight = MulDiv(lf.lfHeight, 12, 10);

	mov	esi, esp
	push	10					; 0000000aH
	push	12					; 0000000cH
	mov	edx, DWORD PTR _lf$[ebp]
	push	edx
	call	DWORD PTR __imp__MulDiv@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lf$[ebp], eax
$L76364:

; 282  : 	
; 283  : 	lf.lfWidth = 0;

	mov	DWORD PTR _lf$[ebp+4], 0

; 284  : 	lf.lfUnderline = (BYTE)(dwFlags & GMFS_UNDERLINED);

	mov	eax, DWORD PTR _dwFlags$[ebp]
	and	eax, 4
	mov	BYTE PTR _lf$[ebp+21], al

; 285  : 	lf.lfItalic = (BYTE)(dwFlags & GMFS_ITALIC);

	mov	ecx, DWORD PTR _dwFlags$[ebp]
	and	ecx, 2
	mov	BYTE PTR _lf$[ebp+20], cl

; 286  : 	lf.lfStrikeOut = (BYTE)(dwFlags & GMFS_STRIKETHRU);

	mov	edx, DWORD PTR _dwFlags$[ebp]
	and	edx, 8
	mov	BYTE PTR _lf$[ebp+22], dl

; 287  : 	lf.lfWeight = (dwFlags & GMFS_BOLD) ? FW_BOLD : FW_NORMAL;

	mov	eax, DWORD PTR _dwFlags$[ebp]
	and	eax, 1
	neg	eax
	sbb	eax, eax
	and	eax, 300				; 0000012cH
	add	eax, 400				; 00000190H
	mov	DWORD PTR _lf$[ebp+16], eax

; 288  : 	
; 289  : 	HFONT hFont = CreateFontIndirect(&lf);

	mov	esi, esp
	lea	ecx, DWORD PTR _lf$[ebp]
	push	ecx
	call	DWORD PTR __imp__CreateFontIndirectW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hFont$[ebp], eax

; 290  : 
; 291  : 	// verify the font creation
; 292  : 	if (!SameFont(hFont, szFaceName, nPoint))

	mov	edx, DWORD PTR _nPoint$[ebp]
	push	edx
	mov	eax, DWORD PTR _szFaceName$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hFont$[ebp]
	push	ecx
	call	?SameFont@GraphicsMisc@@YAHPAUHFONT__@@PBGH@Z ; GraphicsMisc::SameFont
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	SHORT $L76369

; 294  : 		VerifyDeleteObject(hFont);

	mov	edx, DWORD PTR _hFont$[ebp]
	push	edx
	call	?VerifyDeleteObject@GraphicsMisc@@YAHPAX@Z ; GraphicsMisc::VerifyDeleteObject
	add	esp, 4

; 295  : 		hFont = NULL;

	mov	DWORD PTR _hFont$[ebp], 0
$L76369:

; 297  : 	
; 298  : 	return hFont;

	mov	eax, DWORD PTR _hFont$[ebp]

; 299  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 100				; 00000064H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?CreateFontW@GraphicsMisc@@YAPAUHFONT__@@PBGHK@Z ENDP	; GraphicsMisc::CreateFontW
_TEXT	ENDS
PUBLIC	?CreateFontW@GraphicsMisc@@YAHAAVCFont@@PBGHK@Z	; GraphicsMisc::CreateFontW
;	COMDAT ?CreateFontW@GraphicsMisc@@YAHAAVCFont@@PBGHK@Z
_TEXT	SEGMENT
_font$ = 8
_szFaceName$ = 12
_nPoint$ = 16
_dwFlags$ = 20
?CreateFontW@GraphicsMisc@@YAHAAVCFont@@PBGHK@Z PROC NEAR ; GraphicsMisc::CreateFontW, COMDAT

; 302  : {

	push	ebp
	mov	ebp, esp

; 303  : 	VerifyDeleteObject(font);

	mov	eax, DWORD PTR _font$[ebp]
	push	eax
	call	?VerifyDeleteObject@GraphicsMisc@@YAHAAVCGdiObject@@@Z ; GraphicsMisc::VerifyDeleteObject
	add	esp, 4

; 304  : 
; 305  : 	return font.Attach(CreateFont(szFaceName, nPoint, dwFlags));

	mov	ecx, DWORD PTR _dwFlags$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nPoint$[ebp]
	push	edx
	mov	eax, DWORD PTR _szFaceName$[ebp]
	push	eax
	call	?CreateFontW@GraphicsMisc@@YAPAUHFONT__@@PBGHK@Z ; GraphicsMisc::CreateFontW
	add	esp, 12					; 0000000cH
	push	eax
	mov	ecx, DWORD PTR _font$[ebp]
	call	?Attach@CGdiObject@@QAEHPAX@Z		; CGdiObject::Attach

; 306  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?CreateFontW@GraphicsMisc@@YAHAAVCFont@@PBGHK@Z ENDP	; GraphicsMisc::CreateFontW
_TEXT	ENDS
PUBLIC	?GetFontFlags@GraphicsMisc@@YAKPAUHFONT__@@@Z	; GraphicsMisc::GetFontFlags
;	COMDAT ?GetFontFlags@GraphicsMisc@@YAKPAUHFONT__@@@Z
_TEXT	SEGMENT
_hFont$ = 8
_lf$ = -92
_dwFlags$ = -96
?GetFontFlags@GraphicsMisc@@YAKPAUHFONT__@@@Z PROC NEAR	; GraphicsMisc::GetFontFlags, COMDAT

; 309  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 96					; 00000060H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-96]
	mov	ecx, 24					; 00000018H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 310  : 	if (!hFont)

	cmp	DWORD PTR _hFont$[ebp], 0
	jne	SHORT $L76379

; 311  : 		return 0;

	xor	eax, eax
	jmp	SHORT $L76378
$L76379:

; 314  : 	::GetObject(hFont, sizeof(lf), &lf);

	mov	esi, esp
	lea	eax, DWORD PTR _lf$[ebp]
	push	eax
	push	92					; 0000005cH
	mov	ecx, DWORD PTR _hFont$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetObjectW@12
	cmp	esi, esp
	call	__chkesp

; 315  : 
; 316  : 	DWORD dwFlags = 0;

	mov	DWORD PTR _dwFlags$[ebp], 0

; 317  : 	
; 318  : 	dwFlags |= (lf.lfItalic ? GMFS_ITALIC : 0);

	mov	edx, DWORD PTR _lf$[ebp+20]
	and	edx, 255				; 000000ffH
	neg	edx
	sbb	edx, edx
	and	edx, 2
	mov	eax, DWORD PTR _dwFlags$[ebp]
	or	eax, edx
	mov	DWORD PTR _dwFlags$[ebp], eax

; 319  : 	dwFlags |= (lf.lfUnderline ? GMFS_UNDERLINED : 0);

	mov	ecx, DWORD PTR _lf$[ebp+21]
	and	ecx, 255				; 000000ffH
	neg	ecx
	sbb	ecx, ecx
	and	ecx, 4
	mov	edx, DWORD PTR _dwFlags$[ebp]
	or	edx, ecx
	mov	DWORD PTR _dwFlags$[ebp], edx

; 320  : 	dwFlags |= (lf.lfStrikeOut ? GMFS_STRIKETHRU : 0);

	mov	eax, DWORD PTR _lf$[ebp+22]
	and	eax, 255				; 000000ffH
	neg	eax
	sbb	eax, eax
	and	eax, 8
	mov	ecx, DWORD PTR _dwFlags$[ebp]
	or	ecx, eax
	mov	DWORD PTR _dwFlags$[ebp], ecx

; 321  : 	dwFlags |= (lf.lfWeight >= FW_BOLD ? GMFS_BOLD : 0);

	xor	edx, edx
	cmp	DWORD PTR _lf$[ebp+16], 700		; 000002bcH
	setge	dl
	mov	eax, DWORD PTR _dwFlags$[ebp]
	or	eax, edx
	mov	DWORD PTR _dwFlags$[ebp], eax

; 322  : 	
; 323  : 	return dwFlags;

	mov	eax, DWORD PTR _dwFlags$[ebp]
$L76378:

; 324  : }

	pop	edi
	pop	esi
	add	esp, 96					; 00000060H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetFontFlags@GraphicsMisc@@YAKPAUHFONT__@@@Z ENDP	; GraphicsMisc::GetFontFlags
_TEXT	ENDS
PUBLIC	?PixelToPoint@GraphicsMisc@@YAHH@Z		; GraphicsMisc::PixelToPoint
PUBLIC	?GetFontNameAndPointSize@GraphicsMisc@@YAHPAUHFONT__@@AAVCString@@@Z ; GraphicsMisc::GetFontNameAndPointSize
EXTRN	?Empty@CString@@QAEXXZ:NEAR			; CString::Empty
EXTRN	??4CString@@QAEABV0@PBG@Z:NEAR			; CString::operator=
;	COMDAT ?GetFontNameAndPointSize@GraphicsMisc@@YAHPAUHFONT__@@AAVCString@@@Z
_TEXT	SEGMENT
_hFont$ = 8
_sFaceName$ = 12
_lf$ = -92
?GetFontNameAndPointSize@GraphicsMisc@@YAHPAUHFONT__@@AAVCString@@@Z PROC NEAR ; GraphicsMisc::GetFontNameAndPointSize, COMDAT

; 327  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 92					; 0000005cH
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-92]
	mov	ecx, 23					; 00000017H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 328  : 	if (!hFont)

	cmp	DWORD PTR _hFont$[ebp], 0
	jne	SHORT $L76386

; 330  : 		sFaceName.Empty();

	mov	ecx, DWORD PTR _sFaceName$[ebp]
	call	?Empty@CString@@QAEXXZ			; CString::Empty

; 331  : 		return 0;

	xor	eax, eax
	jmp	SHORT $L76385
$L76386:

; 333  : 	
; 334  : 	LOGFONT lf = { 0 };

	mov	DWORD PTR _lf$[ebp], 0
	mov	ecx, 22					; 00000016H
	xor	eax, eax
	lea	edi, DWORD PTR _lf$[ebp+4]
	rep stosd

; 335  : 	::GetObject(hFont, sizeof(lf), &lf);

	mov	esi, esp
	lea	eax, DWORD PTR _lf$[ebp]
	push	eax
	push	92					; 0000005cH
	mov	ecx, DWORD PTR _hFont$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetObjectW@12
	cmp	esi, esp
	call	__chkesp

; 336  : 	
; 337  : 	sFaceName = lf.lfFaceName;

	lea	edx, DWORD PTR _lf$[ebp+28]
	push	edx
	mov	ecx, DWORD PTR _sFaceName$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 338  : 
; 339  : 	return PixelToPoint(lf.lfHeight);

	mov	eax, DWORD PTR _lf$[ebp]
	push	eax
	call	?PixelToPoint@GraphicsMisc@@YAHH@Z	; GraphicsMisc::PixelToPoint
	add	esp, 4
$L76385:

; 340  : }

	pop	edi
	pop	esi
	add	esp, 92					; 0000005cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetFontNameAndPointSize@GraphicsMisc@@YAHPAUHFONT__@@AAVCString@@@Z ENDP ; GraphicsMisc::GetFontNameAndPointSize
_TEXT	ENDS
PUBLIC	?PixelsPerInch@GraphicsMisc@@YAHXZ		; GraphicsMisc::PixelsPerInch
EXTRN	_abs:NEAR
;	COMDAT ?PointToPixel@GraphicsMisc@@YAHH@Z
_TEXT	SEGMENT
_nPoints$ = 8
?PointToPixel@GraphicsMisc@@YAHH@Z PROC NEAR		; GraphicsMisc::PointToPixel, COMDAT

; 343  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 344  : 	return MulDiv(abs(nPoints), PixelsPerInch(), PointsPerInch());

	call	?PointsPerInch@@YAHXZ			; PointsPerInch
	mov	esi, esp
	push	eax
	call	?PixelsPerInch@GraphicsMisc@@YAHXZ	; GraphicsMisc::PixelsPerInch
	push	eax
	mov	eax, DWORD PTR _nPoints$[ebp]
	push	eax
	call	_abs
	add	esp, 4
	push	eax
	call	DWORD PTR __imp__MulDiv@12
	cmp	esi, esp
	call	__chkesp

; 345  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?PointToPixel@GraphicsMisc@@YAHH@Z ENDP			; GraphicsMisc::PointToPixel
_TEXT	ENDS
;	COMDAT ?PointsPerInch@@YAHXZ
_TEXT	SEGMENT
?PointsPerInch@@YAHXZ PROC NEAR				; PointsPerInch, COMDAT

; 35   : static int PointsPerInch() { return 72; }

	push	ebp
	mov	ebp, esp
	mov	eax, 72					; 00000048H
	pop	ebp
	ret	0
?PointsPerInch@@YAHXZ ENDP				; PointsPerInch
_TEXT	ENDS
;	COMDAT ?PixelToPoint@GraphicsMisc@@YAHH@Z
_TEXT	SEGMENT
_nPixels$ = 8
?PixelToPoint@GraphicsMisc@@YAHH@Z PROC NEAR		; GraphicsMisc::PixelToPoint, COMDAT

; 348  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 349  : 	return MulDiv(abs(nPixels), PointsPerInch(), PixelsPerInch());

	call	?PixelsPerInch@GraphicsMisc@@YAHXZ	; GraphicsMisc::PixelsPerInch
	mov	esi, esp
	push	eax
	call	?PointsPerInch@@YAHXZ			; PointsPerInch
	push	eax
	mov	eax, DWORD PTR _nPixels$[ebp]
	push	eax
	call	_abs
	add	esp, 4
	push	eax
	call	DWORD PTR __imp__MulDiv@12
	cmp	esi, esp
	call	__chkesp

; 350  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?PixelToPoint@GraphicsMisc@@YAHH@Z ENDP			; GraphicsMisc::PixelToPoint
_TEXT	ENDS
EXTRN	__imp__GetDC@4:NEAR
EXTRN	__imp__ReleaseDC@8:NEAR
EXTRN	__imp__GetDeviceCaps@8:NEAR
;	COMDAT ?PixelsPerInch@GraphicsMisc@@YAHXZ
_TEXT	SEGMENT
_hDC$ = -8
_nPPI$ = -4
?PixelsPerInch@GraphicsMisc@@YAHXZ PROC NEAR		; GraphicsMisc::PixelsPerInch, COMDAT

; 353  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 354  : 	HDC hDC = ::GetDC(NULL);

	mov	esi, esp
	push	0
	call	DWORD PTR __imp__GetDC@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hDC$[ebp], eax

; 355  : 	int nPPI = GetDeviceCaps(hDC, LOGPIXELSY);

	mov	esi, esp
	push	90					; 0000005aH
	mov	eax, DWORD PTR _hDC$[ebp]
	push	eax
	call	DWORD PTR __imp__GetDeviceCaps@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nPPI$[ebp], eax

; 356  : 	::ReleaseDC(NULL, hDC);

	mov	esi, esp
	mov	ecx, DWORD PTR _hDC$[ebp]
	push	ecx
	push	0
	call	DWORD PTR __imp__ReleaseDC@8
	cmp	esi, esp
	call	__chkesp

; 357  : 
; 358  : 	return nPPI;

	mov	eax, DWORD PTR _nPPI$[ebp]

; 359  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?PixelsPerInch@GraphicsMisc@@YAHXZ ENDP			; GraphicsMisc::PixelsPerInch
_TEXT	ENDS
PUBLIC	?GetFontPointSize@GraphicsMisc@@YAHPAUHFONT__@@@Z ; GraphicsMisc::GetFontPointSize
PUBLIC	?GetFontPixelSize@GraphicsMisc@@YAHPAUHFONT__@@@Z ; GraphicsMisc::GetFontPixelSize
;	COMDAT ?GetFontPointSize@GraphicsMisc@@YAHPAUHFONT__@@@Z
_TEXT	SEGMENT
_hFont$ = 8
?GetFontPointSize@GraphicsMisc@@YAHPAUHFONT__@@@Z PROC NEAR ; GraphicsMisc::GetFontPointSize, COMDAT

; 362  : {

	push	ebp
	mov	ebp, esp

; 363  : 	return PixelToPoint(GetFontPixelSize(hFont));

	mov	eax, DWORD PTR _hFont$[ebp]
	push	eax
	call	?GetFontPixelSize@GraphicsMisc@@YAHPAUHFONT__@@@Z ; GraphicsMisc::GetFontPixelSize
	add	esp, 4
	push	eax
	call	?PixelToPoint@GraphicsMisc@@YAHH@Z	; GraphicsMisc::PixelToPoint
	add	esp, 4

; 364  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?GetFontPointSize@GraphicsMisc@@YAHPAUHFONT__@@@Z ENDP	; GraphicsMisc::GetFontPointSize
_TEXT	ENDS
;	COMDAT ?GetFontPixelSize@GraphicsMisc@@YAHPAUHFONT__@@@Z
_TEXT	SEGMENT
_hFont$ = 8
_lf$ = -92
?GetFontPixelSize@GraphicsMisc@@YAHPAUHFONT__@@@Z PROC NEAR ; GraphicsMisc::GetFontPixelSize, COMDAT

; 367  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 92					; 0000005cH
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-92]
	mov	ecx, 23					; 00000017H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 368  : 	if (!hFont)

	cmp	DWORD PTR _hFont$[ebp], 0
	jne	SHORT $L76404

; 369  : 		return 0;

	xor	eax, eax
	jmp	SHORT $L76403
$L76404:

; 370  : 	
; 371  : 	LOGFONT lf = { 0 };

	mov	DWORD PTR _lf$[ebp], 0
	mov	ecx, 22					; 00000016H
	xor	eax, eax
	lea	edi, DWORD PTR _lf$[ebp+4]
	rep stosd

; 372  : 	::GetObject(hFont, sizeof(lf), &lf);

	mov	esi, esp
	lea	eax, DWORD PTR _lf$[ebp]
	push	eax
	push	92					; 0000005cH
	mov	ecx, DWORD PTR _hFont$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetObjectW@12
	cmp	esi, esp
	call	__chkesp

; 373  : 	
; 374  : 	return abs(lf.lfHeight);

	mov	edx, DWORD PTR _lf$[ebp]
	push	edx
	call	_abs
	add	esp, 4
$L76403:

; 375  : }

	pop	edi
	pop	esi
	add	esp, 92					; 0000005cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetFontPixelSize@GraphicsMisc@@YAHPAUHFONT__@@@Z ENDP	; GraphicsMisc::GetFontPixelSize
_TEXT	ENDS
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	?CompareNoCase@CString@@QBEHPBG@Z:NEAR		; CString::CompareNoCase
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT xdata$x
; File D:\_CODE\Shared\GraphicsMisc.cpp
xdata$x	SEGMENT
$T77213	DD	019930520H
	DD	01H
	DD	FLAT:$T77216
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77216	DD	0ffffffffH
	DD	FLAT:$L77209
xdata$x	ENDS
;	COMDAT ?SameFont@GraphicsMisc@@YAHPAUHFONT__@@PBGH@Z
_TEXT	SEGMENT
$T77204 = -24
__$EHRec$ = -12
_hFont$ = 8
_szFaceName$ = 12
_nPoint$ = 16
_sFontName$ = -16
_nFontSize$ = -20
?SameFont@GraphicsMisc@@YAHPAUHFONT__@@PBGH@Z PROC NEAR	; GraphicsMisc::SameFont, COMDAT

; 378  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77214
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 379  : 	CString sFontName;

	lea	ecx, DWORD PTR _sFontName$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 380  : 	int nFontSize = GetFontNameAndPointSize(hFont, sFontName);

	lea	eax, DWORD PTR _sFontName$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hFont$[ebp]
	push	ecx
	call	?GetFontNameAndPointSize@GraphicsMisc@@YAHPAUHFONT__@@AAVCString@@@Z ; GraphicsMisc::GetFontNameAndPointSize
	add	esp, 8
	mov	DWORD PTR _nFontSize$[ebp], eax

; 381  : 
; 382  : 	return ((nPoint <= 0 || nPoint == nFontSize) && 
; 383  : 			(!szFaceName || sFontName.CompareNoCase(szFaceName) == 0));

	cmp	DWORD PTR _nPoint$[ebp], 0
	jle	SHORT $L77205
	mov	edx, DWORD PTR _nPoint$[ebp]
	cmp	edx, DWORD PTR _nFontSize$[ebp]
	jne	SHORT $L77207
$L77205:
	cmp	DWORD PTR _szFaceName$[ebp], 0
	je	SHORT $L77206
	mov	eax, DWORD PTR _szFaceName$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sFontName$[ebp]
	call	?CompareNoCase@CString@@QBEHPBG@Z	; CString::CompareNoCase
	test	eax, eax
	jne	SHORT $L77207
$L77206:
	mov	DWORD PTR -28+[ebp], 1
	jmp	SHORT $L77208
$L77207:
	mov	DWORD PTR -28+[ebp], 0
$L77208:
	mov	ecx, DWORD PTR -28+[ebp]
	mov	DWORD PTR $T77204[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sFontName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77204[ebp]

; 384  : }

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
$L77209:
	lea	ecx, DWORD PTR _sFontName$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77214:
	mov	eax, OFFSET FLAT:$T77213
	jmp	___CxxFrameHandler
text$x	ENDS
?SameFont@GraphicsMisc@@YAHPAUHFONT__@@PBGH@Z ENDP	; GraphicsMisc::SameFont
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
;	COMDAT xdata$x
; File D:\_CODE\Shared\GraphicsMisc.cpp
xdata$x	SEGMENT
$T77225	DD	019930520H
	DD	01H
	DD	FLAT:$T77227
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77227	DD	0ffffffffH
	DD	FLAT:$L77223
xdata$x	ENDS
;	COMDAT ?SameFontNameSize@GraphicsMisc@@YAHPAUHFONT__@@0@Z
_TEXT	SEGMENT
$T77222 = -24
__$EHRec$ = -12
_hFont1$ = 8
_hFont2$ = 12
_sName1$ = -16
_nSize1$ = -20
?SameFontNameSize@GraphicsMisc@@YAHPAUHFONT__@@0@Z PROC NEAR ; GraphicsMisc::SameFontNameSize, COMDAT

; 387  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77226
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH

; 388  : 	CString sName1;

	lea	ecx, DWORD PTR _sName1$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 389  : 	int nSize1 = GetFontNameAndPointSize(hFont1, sName1);

	lea	eax, DWORD PTR _sName1$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hFont1$[ebp]
	push	ecx
	call	?GetFontNameAndPointSize@GraphicsMisc@@YAHPAUHFONT__@@AAVCString@@@Z ; GraphicsMisc::GetFontNameAndPointSize
	add	esp, 8
	mov	DWORD PTR _nSize1$[ebp], eax

; 390  : 
; 391  : 	return SameFont(hFont2, sName1, nSize1);

	mov	edx, DWORD PTR _nSize1$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sName1$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	eax, DWORD PTR _hFont2$[ebp]
	push	eax
	call	?SameFont@GraphicsMisc@@YAHPAUHFONT__@@PBGH@Z ; GraphicsMisc::SameFont
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77222[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sName1$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77222[ebp]

; 392  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77223:
	lea	ecx, DWORD PTR _sName1$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77226:
	mov	eax, OFFSET FLAT:$T77225
	jmp	___CxxFrameHandler
text$x	ENDS
?SameFontNameSize@GraphicsMisc@@YAHPAUHFONT__@@0@Z ENDP	; GraphicsMisc::SameFontNameSize
PUBLIC	?LoadIconW@GraphicsMisc@@YAPAUHICON__@@IH@Z	; GraphicsMisc::LoadIconW
EXTRN	__imp__LoadImageW@24:NEAR
EXTRN	?AfxGetResourceHandle@@YGPAUHINSTANCE__@@XZ:NEAR ; AfxGetResourceHandle
;	COMDAT ?LoadIconW@GraphicsMisc@@YAPAUHICON__@@IH@Z
_TEXT	SEGMENT
_nIDIcon$ = 8
_nSize$ = 12
_hIcon$ = -4
?LoadIconW@GraphicsMisc@@YAPAUHICON__@@IH@Z PROC NEAR	; GraphicsMisc::LoadIconW, COMDAT

; 395  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 396  : 	HICON hIcon = (HICON)::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(nIDIcon), 
; 397  : 									IMAGE_ICON, nSize, nSize, LR_LOADMAP3DCOLORS);

	mov	esi, esp
	push	4096					; 00001000H
	mov	eax, DWORD PTR _nSize$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nSize$[ebp]
	push	ecx
	push	1
	mov	edx, DWORD PTR _nIDIcon$[ebp]
	and	edx, 65535				; 0000ffffH
	push	edx
	call	?AfxGetResourceHandle@@YGPAUHINSTANCE__@@XZ ; AfxGetResourceHandle
	push	eax
	call	DWORD PTR __imp__LoadImageW@24
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hIcon$[ebp], eax

; 398  : 
; 399  : 	return hIcon;

	mov	eax, DWORD PTR _hIcon$[ebp]

; 400  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?LoadIconW@GraphicsMisc@@YAPAUHICON__@@IH@Z ENDP	; GraphicsMisc::LoadIconW
_TEXT	ENDS
PUBLIC	?HandCursor@GraphicsMisc@@YAPAUHICON__@@XZ	; GraphicsMisc::HandCursor
EXTRN	__imp__LoadCursorW@8:NEAR
EXTRN	__imp__LoadLibraryW@4:NEAR
EXTRN	__imp__GetWindowsDirectoryW@8:NEAR
EXTRN	??YCString@@QAEABV0@PBG@Z:NEAR			; CString::operator+=
EXTRN	?GetBuffer@CString@@QAEPAGH@Z:NEAR		; CString::GetBuffer
EXTRN	?ReleaseBuffer@CString@@QAEXH@Z:NEAR		; CString::ReleaseBuffer
;	COMDAT data
; File D:\_CODE\Shared\GraphicsMisc.cpp
data	SEGMENT
$SG76440 DB	'\', 00H, 'w', 00H, 'i', 00H, 'n', 00H, 'h', 00H, 'l', 00H
	DB	'p', 00H, '3', 00H, '2', 00H, '.', 00H, 'e', 00H, 'x', 00H, 'e'
	DB	00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77236	DD	019930520H
	DD	01H
	DD	FLAT:$T77238
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77238	DD	0ffffffffH
	DD	FLAT:$L77234
xdata$x	ENDS
;	COMDAT ?HandCursor@GraphicsMisc@@YAPAUHICON__@@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_sWinHlp32$76439 = -20
_hMod$76441 = -16
?HandCursor@GraphicsMisc@@YAPAUHICON__@@XZ PROC NEAR	; GraphicsMisc::HandCursor, COMDAT

; 403  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77237
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH

; 404  : #ifndef IDC_HAND
; 405  : #	define IDC_HAND  MAKEINTRESOURCE(32649) // from winuser.h
; 406  : #endif
; 407  : 	static HCURSOR cursor = NULL;
; 408  : 	
; 409  : 	if (!cursor)

	cmp	DWORD PTR _?cursor@?1??HandCursor@GraphicsMisc@@YAPAUHICON__@@XZ@4PAU3@A, 0
	jne	$L76438

; 411  : 		cursor = ::LoadCursor(NULL, IDC_HAND);

	mov	esi, esp
	push	32649					; 00007f89H
	push	0
	call	DWORD PTR __imp__LoadCursorW@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?cursor@?1??HandCursor@GraphicsMisc@@YAPAUHICON__@@XZ@4PAU3@A, eax

; 412  : 		
; 413  : 		// fallback hack for win9x
; 414  : 		if (!cursor)

	cmp	DWORD PTR _?cursor@?1??HandCursor@GraphicsMisc@@YAPAUHICON__@@XZ@4PAU3@A, 0
	jne	$L76438

; 416  : 			CString sWinHlp32;

	lea	ecx, DWORD PTR _sWinHlp32$76439[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 417  : 			
; 418  : 			GetWindowsDirectory(sWinHlp32.GetBuffer(MAX_PATH+1), MAX_PATH);

	mov	esi, esp
	push	260					; 00000104H
	push	261					; 00000105H
	lea	ecx, DWORD PTR _sWinHlp32$76439[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	push	eax
	call	DWORD PTR __imp__GetWindowsDirectoryW@8
	cmp	esi, esp
	call	__chkesp

; 419  : 			sWinHlp32.ReleaseBuffer();

	push	-1
	lea	ecx, DWORD PTR _sWinHlp32$76439[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 420  : 			sWinHlp32 += _T("\\winhlp32.exe");

	push	OFFSET FLAT:$SG76440
	lea	ecx, DWORD PTR _sWinHlp32$76439[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=

; 421  : 			
; 422  : 			HMODULE hMod = LoadLibrary(sWinHlp32);

	lea	ecx, DWORD PTR _sWinHlp32$76439[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hMod$76441[ebp], eax

; 423  : 			
; 424  : 			if (hMod)

	cmp	DWORD PTR _hMod$76441[ebp], 0
	je	SHORT $L76442

; 425  : 				cursor = ::LoadCursor(hMod, MAKEINTRESOURCE(106));

	mov	esi, esp
	push	106					; 0000006aH
	mov	eax, DWORD PTR _hMod$76441[ebp]
	push	eax
	call	DWORD PTR __imp__LoadCursorW@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _?cursor@?1??HandCursor@GraphicsMisc@@YAPAUHICON__@@XZ@4PAU3@A, eax
$L76442:

; 426  : 		}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sWinHlp32$76439[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76438:

; 428  : 
; 429  : 	return cursor;

	mov	eax, DWORD PTR _?cursor@?1??HandCursor@GraphicsMisc@@YAPAUHICON__@@XZ@4PAU3@A

; 430  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77234:
	lea	ecx, DWORD PTR _sWinHlp32$76439[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77237:
	mov	eax, OFFSET FLAT:$T77236
	jmp	___CxxFrameHandler
text$x	ENDS
?HandCursor@GraphicsMisc@@YAPAUHICON__@@XZ ENDP		; GraphicsMisc::HandCursor
PUBLIC	?WingDings@GraphicsMisc@@YAAAVCFont@@XZ		; GraphicsMisc::WingDings
EXTRN	??0CFont@@QAE@XZ:NEAR				; CFont::CFont
EXTRN	_atexit:NEAR
EXTRN	?GetSafeHandle@CGdiObject@@QBEPAXXZ:NEAR	; CGdiObject::GetSafeHandle
_BSS	SEGMENT
	ALIGN	8

_?font@?1??WingDings@GraphicsMisc@@YAAAVCFont@@XZ@4V3@A DQ 01H DUP (?)
_?$S281@?1??WingDings@GraphicsMisc@@YAAAVCFont@@XZ@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\GraphicsMisc.cpp
data	SEGMENT
$SG76455 DB	'W', 00H, 'i', 00H, 'n', 00H, 'g', 00H, 'd', 00H, 'i', 00H
	DB	'n', 00H, 'g', 00H, 's', 00H, 00H, 00H
data	ENDS
;	COMDAT ?WingDings@GraphicsMisc@@YAAAVCFont@@XZ
_TEXT	SEGMENT
?WingDings@GraphicsMisc@@YAAAVCFont@@XZ PROC NEAR	; GraphicsMisc::WingDings, COMDAT

; 433  : {

	push	ebp
	mov	ebp, esp

; 434  : 	static CFont font;

	xor	eax, eax
	mov	al, BYTE PTR _?$S281@?1??WingDings@GraphicsMisc@@YAAAVCFont@@XZ@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L76452
	mov	cl, BYTE PTR _?$S281@?1??WingDings@GraphicsMisc@@YAAAVCFont@@XZ@4EA
	or	cl, 1
	mov	BYTE PTR _?$S281@?1??WingDings@GraphicsMisc@@YAAAVCFont@@XZ@4EA, cl
	mov	ecx, OFFSET FLAT:_?font@?1??WingDings@GraphicsMisc@@YAAAVCFont@@XZ@4V3@A
	call	??0CFont@@QAE@XZ			; CFont::CFont
	push	OFFSET FLAT:_$E282
	call	_atexit
	add	esp, 4
$L76452:

; 435  : 				
; 436  : 	if (!font.GetSafeHandle())

	mov	ecx, OFFSET FLAT:_?font@?1??WingDings@GraphicsMisc@@YAAAVCFont@@XZ@4V3@A
	call	?GetSafeHandle@CGdiObject@@QBEPAXXZ	; CGdiObject::GetSafeHandle
	test	eax, eax
	jne	SHORT $L76454

; 437  : 		font.Attach(CreateFont(_T("Wingdings"), -1, GMFS_SYMBOL));

	push	16					; 00000010H
	push	-1
	push	OFFSET FLAT:$SG76455
	call	?CreateFontW@GraphicsMisc@@YAPAUHFONT__@@PBGHK@Z ; GraphicsMisc::CreateFontW
	add	esp, 12					; 0000000cH
	push	eax
	mov	ecx, OFFSET FLAT:_?font@?1??WingDings@GraphicsMisc@@YAAAVCFont@@XZ@4V3@A
	call	?Attach@CGdiObject@@QAEHPAX@Z		; CGdiObject::Attach
$L76454:

; 438  : 
; 439  : 	return font;

	mov	eax, OFFSET FLAT:_?font@?1??WingDings@GraphicsMisc@@YAAAVCFont@@XZ@4V3@A

; 440  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?WingDings@GraphicsMisc@@YAAAVCFont@@XZ ENDP		; GraphicsMisc::WingDings
_TEXT	ENDS
EXTRN	??1CFont@@UAE@XZ:NEAR				; CFont::~CFont
;	COMDAT _$E282
_TEXT	SEGMENT
_$E282	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?font@?1??WingDings@GraphicsMisc@@YAAAVCFont@@XZ@4V3@A
	call	??1CFont@@UAE@XZ			; CFont::~CFont
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E282	ENDP
_TEXT	ENDS
PUBLIC	?Marlett@GraphicsMisc@@YAAAVCFont@@XZ		; GraphicsMisc::Marlett
_BSS	SEGMENT
	ALIGN	8

_?font@?1??Marlett@GraphicsMisc@@YAAAVCFont@@XZ@4V3@A DQ 01H DUP (?)
_?$S283@?1??Marlett@GraphicsMisc@@YAAAVCFont@@XZ@4EA DB 01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\GraphicsMisc.cpp
data	SEGMENT
$SG76466 DB	'M', 00H, 'a', 00H, 'r', 00H, 'l', 00H, 'e', 00H, 't', 00H
	DB	't', 00H, 00H, 00H
data	ENDS
;	COMDAT ?Marlett@GraphicsMisc@@YAAAVCFont@@XZ
_TEXT	SEGMENT
?Marlett@GraphicsMisc@@YAAAVCFont@@XZ PROC NEAR		; GraphicsMisc::Marlett, COMDAT

; 443  : {

	push	ebp
	mov	ebp, esp

; 444  : 	static CFont font;

	xor	eax, eax
	mov	al, BYTE PTR _?$S283@?1??Marlett@GraphicsMisc@@YAAAVCFont@@XZ@4EA
	and	eax, 1
	test	eax, eax
	jne	SHORT $L76463
	mov	cl, BYTE PTR _?$S283@?1??Marlett@GraphicsMisc@@YAAAVCFont@@XZ@4EA
	or	cl, 1
	mov	BYTE PTR _?$S283@?1??Marlett@GraphicsMisc@@YAAAVCFont@@XZ@4EA, cl
	mov	ecx, OFFSET FLAT:_?font@?1??Marlett@GraphicsMisc@@YAAAVCFont@@XZ@4V3@A
	call	??0CFont@@QAE@XZ			; CFont::CFont
	push	OFFSET FLAT:_$E284
	call	_atexit
	add	esp, 4
$L76463:

; 445  : 				
; 446  : 	if (!font.GetSafeHandle())

	mov	ecx, OFFSET FLAT:_?font@?1??Marlett@GraphicsMisc@@YAAAVCFont@@XZ@4V3@A
	call	?GetSafeHandle@CGdiObject@@QBEPAXXZ	; CGdiObject::GetSafeHandle
	test	eax, eax
	jne	SHORT $L76465

; 447  : 		font.Attach(CreateFont(_T("Marlett"), -1, GMFS_SYMBOL));

	push	16					; 00000010H
	push	-1
	push	OFFSET FLAT:$SG76466
	call	?CreateFontW@GraphicsMisc@@YAPAUHFONT__@@PBGHK@Z ; GraphicsMisc::CreateFontW
	add	esp, 12					; 0000000cH
	push	eax
	mov	ecx, OFFSET FLAT:_?font@?1??Marlett@GraphicsMisc@@YAAAVCFont@@XZ@4V3@A
	call	?Attach@CGdiObject@@QAEHPAX@Z		; CGdiObject::Attach
$L76465:

; 448  : 
; 449  : 	return font;

	mov	eax, OFFSET FLAT:_?font@?1??Marlett@GraphicsMisc@@YAAAVCFont@@XZ@4V3@A

; 450  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?Marlett@GraphicsMisc@@YAAAVCFont@@XZ ENDP		; GraphicsMisc::Marlett
_TEXT	ENDS
;	COMDAT _$E284
_TEXT	SEGMENT
_$E284	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	mov	ecx, OFFSET FLAT:_?font@?1??Marlett@GraphicsMisc@@YAAAVCFont@@XZ@4V3@A
	call	??1CFont@@UAE@XZ			; CFont::~CFont
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E284	ENDP
_TEXT	ENDS
PUBLIC	?GetTextWidth@GraphicsMisc@@YAHIAAVCWnd@@PAVCFont@@@Z ; GraphicsMisc::GetTextWidth
PUBLIC	?GetTextWidth@GraphicsMisc@@YAHABVCString@@AAVCWnd@@PAVCFont@@@Z ; GraphicsMisc::GetTextWidth
EXTRN	??0CEnString@@QAE@IPBG@Z:NEAR			; CEnString::CEnString
EXTRN	??1CEnString@@QAE@XZ:NEAR			; CEnString::~CEnString
;	COMDAT xdata$x
; File D:\_CODE\Shared\GraphicsMisc.cpp
xdata$x	SEGMENT
$T77257	DD	019930520H
	DD	01H
	DD	FLAT:$T77259
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77259	DD	0ffffffffH
	DD	FLAT:$L77255
xdata$x	ENDS
;	COMDAT ?GetTextWidth@GraphicsMisc@@YAHIAAVCWnd@@PAVCFont@@@Z
_TEXT	SEGMENT
$T77254 = -20
__$EHRec$ = -12
_nIDString$ = 8
_wndRef$ = 12
_pRefFont$ = 16
_sText$ = -16
?GetTextWidth@GraphicsMisc@@YAHIAAVCWnd@@PAVCFont@@@Z PROC NEAR ; GraphicsMisc::GetTextWidth, COMDAT

; 453  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77258
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH

; 454  : 	CEnString sText(nIDString);

	push	0
	mov	eax, DWORD PTR _nIDString$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??0CEnString@@QAE@IPBG@Z		; CEnString::CEnString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 455  : 	return GetTextWidth(sText, wndRef, pRefFont);

	mov	ecx, DWORD PTR _pRefFont$[ebp]
	push	ecx
	mov	edx, DWORD PTR _wndRef$[ebp]
	push	edx
	lea	eax, DWORD PTR _sText$[ebp]
	push	eax
	call	?GetTextWidth@GraphicsMisc@@YAHABVCString@@AAVCWnd@@PAVCFont@@@Z ; GraphicsMisc::GetTextWidth
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77254[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	mov	eax, DWORD PTR $T77254[ebp]

; 456  : }

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
$L77255:
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	ret	0
$L77258:
	mov	eax, OFFSET FLAT:$T77257
	jmp	___CxxFrameHandler
text$x	ENDS
?GetTextWidth@GraphicsMisc@@YAHIAAVCWnd@@PAVCFont@@@Z ENDP ; GraphicsMisc::GetTextWidth
EXTRN	?GetFont@CWnd@@QBEPAVCFont@@XZ:NEAR		; CWnd::GetFont
EXTRN	?GetDC@CWnd@@QAEPAVCDC@@XZ:NEAR			; CWnd::GetDC
EXTRN	?ReleaseDC@CWnd@@QAEHPAVCDC@@@Z:NEAR		; CWnd::ReleaseDC
EXTRN	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z:NEAR ; CDC::GetTextExtent
;	COMDAT ?GetTextWidth@GraphicsMisc@@YAHABVCString@@AAVCWnd@@PAVCFont@@@Z
_TEXT	SEGMENT
$T77264 = -20
_sText$ = 8
_wndRef$ = 12
_pRefFont$ = 16
_pDC$ = -12
_pOldFont$ = -8
_nLength$ = -4
?GetTextWidth@GraphicsMisc@@YAHABVCString@@AAVCWnd@@PAVCFont@@@Z PROC NEAR ; GraphicsMisc::GetTextWidth, COMDAT

; 459  : {

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

; 460  : 	CDC* pDC = wndRef.GetDC();

	mov	ecx, DWORD PTR _wndRef$[ebp]
	call	?GetDC@CWnd@@QAEPAVCDC@@XZ		; CWnd::GetDC
	mov	DWORD PTR _pDC$[ebp], eax
$L76481:

; 461  : 	ASSERT(pDC);

	cmp	DWORD PTR _pDC$[ebp], 0
	jne	SHORT $L76484
	push	461					; 000001cdH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76484
	int	3
$L76484:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76481

; 462  : 
; 463  : 	if (!pDC)

	cmp	DWORD PTR _pDC$[ebp], 0
	jne	SHORT $L76485

; 464  : 		return -1;

	or	eax, -1
	jmp	SHORT $L76479
$L76485:

; 465  : 	
; 466  : 	if (pRefFont == NULL)

	cmp	DWORD PTR _pRefFont$[ebp], 0
	jne	SHORT $L76486

; 467  : 		pRefFont = wndRef.GetFont();

	mov	ecx, DWORD PTR _wndRef$[ebp]
	call	?GetFont@CWnd@@QBEPAVCFont@@XZ		; CWnd::GetFont
	mov	DWORD PTR _pRefFont$[ebp], eax
$L76486:

; 468  : 
; 469  : 	CFont* pOldFont = pDC->SelectObject(pRefFont);

	mov	esi, esp
	mov	ecx, DWORD PTR _pRefFont$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pDC$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	DWORD PTR [eax+48]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pOldFont$[ebp], eax

; 470  : 	int nLength = pDC->GetTextExtent(sText).cx;

	mov	ecx, DWORD PTR _sText$[ebp]
	push	ecx
	lea	edx, DWORD PTR $T77264[ebp]
	push	edx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z ; CDC::GetTextExtent
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR _nLength$[ebp], eax

; 471  : 
; 472  : 	pDC->SelectObject(pOldFont);

	mov	esi, esp
	mov	ecx, DWORD PTR _pOldFont$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pDC$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	DWORD PTR [eax+48]
	cmp	esi, esp
	call	__chkesp

; 473  : 	wndRef.ReleaseDC(pDC);

	mov	ecx, DWORD PTR _pDC$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _wndRef$[ebp]
	call	?ReleaseDC@CWnd@@QAEHPAVCDC@@@Z		; CWnd::ReleaseDC

; 474  : 
; 475  : 	return nLength;

	mov	eax, DWORD PTR _nLength$[ebp]
$L76479:

; 476  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetTextWidth@GraphicsMisc@@YAHABVCString@@AAVCWnd@@PAVCFont@@@Z ENDP ; GraphicsMisc::GetTextWidth
_TEXT	ENDS
PUBLIC	?GetTextWidth@GraphicsMisc@@YAHPAVCDC@@PBGZZ	; GraphicsMisc::GetTextWidth
EXTRN	__imp__vswprintf:NEAR
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	?AfxIsValidString@@YGHPBGH@Z:NEAR		; AfxIsValidString
_BSS	SEGMENT
	ALIGN	4

_?BUFFER@?1??GetTextWidth@GraphicsMisc@@YAHPAVCDC@@PBGZZ@4PAGA DW 0800H DUP (?)
_BSS	ENDS
;	COMDAT xdata$x
; File D:\_CODE\Shared\GraphicsMisc.cpp
xdata$x	SEGMENT
$T77272	DD	019930520H
	DD	01H
	DD	FLAT:$T77274
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77274	DD	0ffffffffH
	DD	FLAT:$L77270
xdata$x	ENDS
;	COMDAT ?GetTextWidth@GraphicsMisc@@YAHPAVCDC@@PBGZZ
_TEXT	SEGMENT
$T77267 = -20
$T77268 = -24
$T77269 = -32
__$EHRec$ = -12
_pDC$ = 8
_lpszFormat$ = 12
_argList$ = -16
?GetTextWidth@GraphicsMisc@@YAHPAVCDC@@PBGZZ PROC NEAR	; GraphicsMisc::GetTextWidth, COMDAT

; 479  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77273
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
$L76496:

; 480  : 	static TCHAR BUFFER[2048];
; 481  : 
; 482  : 	ASSERT(AfxIsValidString(lpszFormat));

	push	-1
	mov	eax, DWORD PTR _lpszFormat$[ebp]
	push	eax
	call	?AfxIsValidString@@YGHPBGH@Z		; AfxIsValidString
	test	eax, eax
	jne	SHORT $L76499
	push	482					; 000001e2H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76499
	int	3
$L76499:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76496

; 483  : 
; 484  : 	va_list argList;
; 485  : 	va_start(argList, lpszFormat);

	lea	edx, DWORD PTR _lpszFormat$[ebp+4]
	mov	DWORD PTR _argList$[ebp], edx

; 486  : //fabio_2005
; 487  : #if _MSC_VER >= 1400
; 488  : 	_vstprintf_s(BUFFER, lpszFormat, argList);
; 489  : #else
; 490  : 	_vstprintf(BUFFER, lpszFormat, argList);

	mov	esi, esp
	mov	eax, DWORD PTR _argList$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpszFormat$[ebp]
	push	ecx
	push	OFFSET FLAT:_?BUFFER@?1??GetTextWidth@GraphicsMisc@@YAHPAVCDC@@PBGZZ@4PAGA
	call	DWORD PTR __imp__vswprintf
	add	esp, 12					; 0000000cH
	cmp	esi, esp
	call	__chkesp

; 491  : #endif
; 492  : 	va_end(argList);

	mov	DWORD PTR _argList$[ebp], 0

; 493  : 
; 494  : 	return pDC->GetTextExtent(BUFFER).cx;

	push	OFFSET FLAT:_?BUFFER@?1??GetTextWidth@GraphicsMisc@@YAHPAVCDC@@PBGZZ@4PAGA
	lea	ecx, DWORD PTR $T77268[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	lea	edx, DWORD PTR $T77268[ebp]
	push	edx
	lea	eax, DWORD PTR $T77269[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z ; CDC::GetTextExtent
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR $T77267[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T77268[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77267[ebp]

; 495  : }

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
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77270:
	lea	ecx, DWORD PTR $T77268[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77273:
	mov	eax, OFFSET FLAT:$T77272
	jmp	___CxxFrameHandler
text$x	ENDS
?GetTextWidth@GraphicsMisc@@YAHPAVCDC@@PBGZZ ENDP	; GraphicsMisc::GetTextWidth
PUBLIC	?GetAverageCharWidth@GraphicsMisc@@YAMPAVCDC@@@Z ; GraphicsMisc::GetAverageCharWidth
EXTRN	?GetTextMetricsW@CDC@@QBEHPAUtagTEXTMETRICW@@@Z:NEAR ; CDC::GetTextMetricsW
;	COMDAT ?GetAverageCharWidth@GraphicsMisc@@YAMPAVCDC@@@Z
_TEXT	SEGMENT
_pDC$ = 8
_tm$ = -60
?GetAverageCharWidth@GraphicsMisc@@YAMPAVCDC@@@Z PROC NEAR ; GraphicsMisc::GetAverageCharWidth, COMDAT

; 498  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 60					; 0000003cH
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-60]
	mov	ecx, 15					; 0000000fH
	mov	eax, -858993460				; ccccccccH
	rep stosd
$L76511:

; 499  : 	ASSERT(pDC);

	cmp	DWORD PTR _pDC$[ebp], 0
	jne	SHORT $L76514
	push	499					; 000001f3H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76514
	int	3
$L76514:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76511

; 500  : 	
; 501  : 	TEXTMETRIC tm = { 0 };

	mov	DWORD PTR _tm$[ebp], 0
	mov	ecx, 14					; 0000000eH
	xor	eax, eax
	lea	edi, DWORD PTR _tm$[ebp+4]
	rep stosd

; 502  : 	pDC->GetTextMetrics(&tm);

	lea	ecx, DWORD PTR _tm$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetTextMetricsW@CDC@@QBEHPAUtagTEXTMETRICW@@@Z ; CDC::GetTextMetricsW

; 503  : 	
; 504  : 	return (float)tm.tmAveCharWidth;

	fild	DWORD PTR _tm$[ebp+20]

; 505  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 60					; 0000003cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetAverageCharWidth@GraphicsMisc@@YAMPAVCDC@@@Z ENDP	; GraphicsMisc::GetAverageCharWidth
_TEXT	ENDS
PUBLIC	?GetAverageStringWidth@GraphicsMisc@@YAHABVCString@@PAVCDC@@@Z ; GraphicsMisc::GetAverageStringWidth
EXTRN	__ftol:NEAR
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
;	COMDAT ?GetAverageStringWidth@GraphicsMisc@@YAHABVCString@@PAVCDC@@@Z
_TEXT	SEGMENT
_sText$ = 8
_pDC$ = 12
?GetAverageStringWidth@GraphicsMisc@@YAHABVCString@@PAVCDC@@@Z PROC NEAR ; GraphicsMisc::GetAverageStringWidth, COMDAT

; 508  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 509  : 	if (sText.IsEmpty())

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L76521

; 510  : 		return 0;

	xor	eax, eax
	jmp	SHORT $L76520
$L76521:

; 511  : 
; 512  : 	return (int)(GetAverageCharWidth(pDC) * sText.GetLength());

	mov	eax, DWORD PTR _pDC$[ebp]
	push	eax
	call	?GetAverageCharWidth@GraphicsMisc@@YAMPAVCDC@@@Z ; GraphicsMisc::GetAverageCharWidth
	fstp	DWORD PTR -4+[ebp]
	add	esp, 4
	mov	ecx, DWORD PTR _sText$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR -8+[ebp], eax
	fild	DWORD PTR -8+[ebp]
	fmul	DWORD PTR -4+[ebp]
	call	__ftol
$L76520:

; 513  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetAverageStringWidth@GraphicsMisc@@YAHABVCString@@PAVCDC@@@Z ENDP ; GraphicsMisc::GetAverageStringWidth
_TEXT	ENDS
PUBLIC	?GetAverageMaxStringWidth@GraphicsMisc@@YAHABVCString@@PAVCDC@@@Z ; GraphicsMisc::GetAverageMaxStringWidth
;	COMDAT ?GetAverageMaxStringWidth@GraphicsMisc@@YAHABVCString@@PAVCDC@@@Z
_TEXT	SEGMENT
$T77284 = -16
_sText$ = 8
_pDC$ = 12
_nAveWidth$ = -8
_nActualWidth$ = -4
?GetAverageMaxStringWidth@GraphicsMisc@@YAHABVCString@@PAVCDC@@@Z PROC NEAR ; GraphicsMisc::GetAverageMaxStringWidth, COMDAT

; 516  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 20					; 00000014H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 517  : 	if (sText.IsEmpty())

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L76527

; 518  : 		return 0;

	xor	eax, eax
	jmp	SHORT $L76526
$L76527:

; 519  : 
; 520  : 	int nAveWidth = GetAverageStringWidth(sText, pDC);

	mov	eax, DWORD PTR _pDC$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sText$[ebp]
	push	ecx
	call	?GetAverageStringWidth@GraphicsMisc@@YAHABVCString@@PAVCDC@@@Z ; GraphicsMisc::GetAverageStringWidth
	add	esp, 8
	mov	DWORD PTR _nAveWidth$[ebp], eax

; 521  : 	int nActualWidth = pDC->GetTextExtent(sText).cx;

	mov	edx, DWORD PTR _sText$[ebp]
	push	edx
	lea	eax, DWORD PTR $T77284[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetTextExtent@CDC@@QBE?AVCSize@@ABVCString@@@Z ; CDC::GetTextExtent
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _nActualWidth$[ebp], ecx

; 522  : 
; 523  : 	return max(nAveWidth, nActualWidth);

	mov	edx, DWORD PTR _nAveWidth$[ebp]
	cmp	edx, DWORD PTR _nActualWidth$[ebp]
	jle	SHORT $L77285
	mov	eax, DWORD PTR _nAveWidth$[ebp]
	mov	DWORD PTR -20+[ebp], eax
	jmp	SHORT $L77286
$L77285:
	mov	ecx, DWORD PTR _nActualWidth$[ebp]
	mov	DWORD PTR -20+[ebp], ecx
$L77286:
	mov	eax, DWORD PTR -20+[ebp]
$L76526:

; 524  : }

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetAverageMaxStringWidth@GraphicsMisc@@YAHABVCString@@PAVCDC@@@Z ENDP ; GraphicsMisc::GetAverageMaxStringWidth
_TEXT	ENDS
PUBLIC	?PrepareDCFont@GraphicsMisc@@YAPAVCFont@@PAVCDC@@PAVCWnd@@PAV2@H@Z ; GraphicsMisc::PrepareDCFont
EXTRN	?FromHandle@CFont@@SGPAV1@PAUHFONT__@@@Z:NEAR	; CFont::FromHandle
EXTRN	?SendMessageW@CWnd@@QAEJIIJ@Z:NEAR		; CWnd::SendMessageW
;	COMDAT ?PrepareDCFont@GraphicsMisc@@YAPAVCFont@@PAVCDC@@PAVCWnd@@PAV2@H@Z
_TEXT	SEGMENT
_pDC$ = 8
_pWndRef$ = 12
_pFont$ = 16
_nStockFont$ = 20
?PrepareDCFont@GraphicsMisc@@YAPAVCFont@@PAVCDC@@PAVCWnd@@PAV2@H@Z PROC NEAR ; GraphicsMisc::PrepareDCFont, COMDAT

; 527  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 528  : 	if (pFont == NULL && pWndRef != NULL)

	cmp	DWORD PTR _pFont$[ebp], 0
	jne	SHORT $L76537
	cmp	DWORD PTR _pWndRef$[ebp], 0
	je	SHORT $L76537

; 529  : 		pFont = CFont::FromHandle((HFONT)pWndRef->SendMessage(WM_GETFONT, 0, 0));

	push	0
	push	0
	push	49					; 00000031H
	mov	ecx, DWORD PTR _pWndRef$[ebp]
	call	?SendMessageW@CWnd@@QAEJIIJ@Z		; CWnd::SendMessageW
	push	eax
	call	?FromHandle@CFont@@SGPAV1@PAUHFONT__@@@Z ; CFont::FromHandle
	mov	DWORD PTR _pFont$[ebp], eax
$L76537:

; 530  : 
; 531  : 	if (pFont)

	cmp	DWORD PTR _pFont$[ebp], 0
	je	SHORT $L76539

; 532  : 		return pDC->SelectObject(pFont);

	mov	esi, esp
	mov	eax, DWORD PTR _pFont$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	DWORD PTR [edx+48]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L76536
$L76539:

; 533  : 	
; 534  : 	// else
; 535  : 	return (CFont*)pDC->SelectStockObject(nStockFont);

	mov	esi, esp
	mov	eax, DWORD PTR _nStockFont$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	DWORD PTR [edx+44]
	cmp	esi, esp
	call	__chkesp
$L76536:

; 536  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?PrepareDCFont@GraphicsMisc@@YAPAVCFont@@PAVCDC@@PAVCWnd@@PAV2@H@Z ENDP ; GraphicsMisc::PrepareDCFont
_TEXT	ENDS
PUBLIC	??0RGBX@@QAE@ABK@Z				; RGBX::RGBX
PUBLIC	?Luminance@RGBX@@QBEHXZ				; RGBX::Luminance
PUBLIC	?GetBestTextColor@GraphicsMisc@@YAKK@Z		; GraphicsMisc::GetBestTextColor
;	COMDAT ?GetBestTextColor@GraphicsMisc@@YAKK@Z
_TEXT	SEGMENT
$T77291 = -4
_crBack$ = 8
?GetBestTextColor@GraphicsMisc@@YAKK@Z PROC NEAR	; GraphicsMisc::GetBestTextColor, COMDAT

; 539  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 540  : 	// base text color on luminance
; 541  : 	return (RGBX(crBack).Luminance() < 128) ? RGB(255, 255, 255) : 0;

	lea	eax, DWORD PTR _crBack$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77291[ebp]
	call	??0RGBX@@QAE@ABK@Z			; RGBX::RGBX
	mov	ecx, eax
	call	?Luminance@RGBX@@QBEHXZ			; RGBX::Luminance
	xor	ecx, ecx
	cmp	eax, 128				; 00000080H
	setge	cl
	dec	ecx
	and	ecx, 16777215				; 00ffffffH
	mov	eax, ecx

; 542  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetBestTextColor@GraphicsMisc@@YAKK@Z ENDP		; GraphicsMisc::GetBestTextColor
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
;	COMDAT ?Lighter@GraphicsMisc@@YAKKN@Z
_TEXT	SEGMENT
_color$ = 8
_dAmount$ = 12
_red$ = -4
_green$ = -8
_blue$ = -12
?Lighter@GraphicsMisc@@YAKKN@Z PROC NEAR		; GraphicsMisc::Lighter, COMDAT

; 545  : {

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

; 546  : 	if (color == CLR_NONE)

	cmp	DWORD PTR _color$[ebp], -1
	jne	SHORT $L76555

; 547  : 		return CLR_NONE;

	or	eax, -1
	jmp	$L76554
$L76555:

; 548  : 
; 549  : 	int red = GetRValue(color);

	mov	eax, DWORD PTR _color$[ebp]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR _red$[ebp], eax

; 550  : 	int green = GetGValue(color);

	mov	ecx, DWORD PTR _color$[ebp]
	and	ecx, 65535				; 0000ffffH
	sar	ecx, 8
	and	ecx, 255				; 000000ffH
	mov	DWORD PTR _green$[ebp], ecx

; 551  : 	int blue = GetBValue(color);

	mov	edx, DWORD PTR _color$[ebp]
	shr	edx, 16					; 00000010H
	and	edx, 255				; 000000ffH
	mov	DWORD PTR _blue$[ebp], edx

; 552  : 	
; 553  : 	red += (int)((255 - red) * dAmount);

	mov	eax, 255				; 000000ffH
	sub	eax, DWORD PTR _red$[ebp]
	mov	DWORD PTR -16+[ebp], eax
	fild	DWORD PTR -16+[ebp]
	fmul	QWORD PTR _dAmount$[ebp]
	call	__ftol
	mov	ecx, DWORD PTR _red$[ebp]
	add	ecx, eax
	mov	DWORD PTR _red$[ebp], ecx

; 554  : 	green += (int)((255 - green) * dAmount);

	mov	edx, 255				; 000000ffH
	sub	edx, DWORD PTR _green$[ebp]
	mov	DWORD PTR -20+[ebp], edx
	fild	DWORD PTR -20+[ebp]
	fmul	QWORD PTR _dAmount$[ebp]
	call	__ftol
	mov	ecx, DWORD PTR _green$[ebp]
	add	ecx, eax
	mov	DWORD PTR _green$[ebp], ecx

; 555  : 	blue += (int)((255 - blue) * dAmount);

	mov	edx, 255				; 000000ffH
	sub	edx, DWORD PTR _blue$[ebp]
	mov	DWORD PTR -24+[ebp], edx
	fild	DWORD PTR -24+[ebp]
	fmul	QWORD PTR _dAmount$[ebp]
	call	__ftol
	mov	ecx, DWORD PTR _blue$[ebp]
	add	ecx, eax
	mov	DWORD PTR _blue$[ebp], ecx

; 556  : 
; 557  : 	red = min(255, red);

	cmp	DWORD PTR _red$[ebp], 255		; 000000ffH
	jle	SHORT $L77298
	mov	DWORD PTR -28+[ebp], 255		; 000000ffH
	jmp	SHORT $L77299
$L77298:
	mov	edx, DWORD PTR _red$[ebp]
	mov	DWORD PTR -28+[ebp], edx
$L77299:
	mov	eax, DWORD PTR -28+[ebp]
	mov	DWORD PTR _red$[ebp], eax

; 558  : 	green = min(255, green);

	cmp	DWORD PTR _green$[ebp], 255		; 000000ffH
	jle	SHORT $L77300
	mov	DWORD PTR -32+[ebp], 255		; 000000ffH
	jmp	SHORT $L77301
$L77300:
	mov	ecx, DWORD PTR _green$[ebp]
	mov	DWORD PTR -32+[ebp], ecx
$L77301:
	mov	edx, DWORD PTR -32+[ebp]
	mov	DWORD PTR _green$[ebp], edx

; 559  : 	blue = min(255, blue);

	cmp	DWORD PTR _blue$[ebp], 255		; 000000ffH
	jle	SHORT $L77302
	mov	DWORD PTR -36+[ebp], 255		; 000000ffH
	jmp	SHORT $L77303
$L77302:
	mov	eax, DWORD PTR _blue$[ebp]
	mov	DWORD PTR -36+[ebp], eax
$L77303:
	mov	ecx, DWORD PTR -36+[ebp]
	mov	DWORD PTR _blue$[ebp], ecx

; 560  : 	
; 561  : 	return RGB(red, green, blue);

	mov	eax, DWORD PTR _red$[ebp]
	and	eax, 255				; 000000ffH
	mov	edx, DWORD PTR _green$[ebp]
	and	edx, 255				; 000000ffH
	shl	edx, 8
	or	eax, edx
	mov	ecx, DWORD PTR _blue$[ebp]
	and	ecx, 255				; 000000ffH
	shl	ecx, 16					; 00000010H
	or	eax, ecx
$L76554:

; 562  : }

	mov	esp, ebp
	pop	ebp
	ret	0
?Lighter@GraphicsMisc@@YAKKN@Z ENDP			; GraphicsMisc::Lighter
_TEXT	ENDS
PUBLIC	?Darker@GraphicsMisc@@YAKKN@Z			; GraphicsMisc::Darker
;	COMDAT ?Darker@GraphicsMisc@@YAKKN@Z
_TEXT	SEGMENT
_color$ = 8
_dAmount$ = 12
_red$ = -4
_green$ = -8
_blue$ = -12
?Darker@GraphicsMisc@@YAKKN@Z PROC NEAR			; GraphicsMisc::Darker, COMDAT

; 565  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 566  : 	if (color == CLR_NONE)

	cmp	DWORD PTR _color$[ebp], -1
	jne	SHORT $L76576

; 567  : 		return CLR_NONE;

	or	eax, -1
	jmp	$L76575
$L76576:

; 568  : 
; 569  : 	int red = GetRValue(color);

	mov	eax, DWORD PTR _color$[ebp]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR _red$[ebp], eax

; 570  : 	int green = GetGValue(color);

	mov	ecx, DWORD PTR _color$[ebp]
	and	ecx, 65535				; 0000ffffH
	sar	ecx, 8
	and	ecx, 255				; 000000ffH
	mov	DWORD PTR _green$[ebp], ecx

; 571  : 	int blue = GetBValue(color);

	mov	edx, DWORD PTR _color$[ebp]
	shr	edx, 16					; 00000010H
	and	edx, 255				; 000000ffH
	mov	DWORD PTR _blue$[ebp], edx

; 572  : 	
; 573  : 	red -= (int)(red * dAmount);

	fild	DWORD PTR _red$[ebp]
	fmul	QWORD PTR _dAmount$[ebp]
	call	__ftol
	mov	ecx, DWORD PTR _red$[ebp]
	sub	ecx, eax
	mov	DWORD PTR _red$[ebp], ecx

; 574  : 	green -= (int)(green * dAmount);

	fild	DWORD PTR _green$[ebp]
	fmul	QWORD PTR _dAmount$[ebp]
	call	__ftol
	mov	edx, DWORD PTR _green$[ebp]
	sub	edx, eax
	mov	DWORD PTR _green$[ebp], edx

; 575  : 	blue -= (int)(blue * dAmount);

	fild	DWORD PTR _blue$[ebp]
	fmul	QWORD PTR _dAmount$[ebp]
	call	__ftol
	mov	ecx, DWORD PTR _blue$[ebp]
	sub	ecx, eax
	mov	DWORD PTR _blue$[ebp], ecx

; 576  : 
; 577  : 	red = max(0, red);

	xor	edx, edx
	cmp	DWORD PTR _red$[ebp], 0
	setl	dl
	dec	edx
	and	edx, DWORD PTR _red$[ebp]
	mov	DWORD PTR _red$[ebp], edx

; 578  : 	green = max(0, green);

	xor	eax, eax
	cmp	DWORD PTR _green$[ebp], 0
	setl	al
	dec	eax
	and	eax, DWORD PTR _green$[ebp]
	mov	DWORD PTR _green$[ebp], eax

; 579  : 	blue = max(0, blue);

	xor	ecx, ecx
	cmp	DWORD PTR _blue$[ebp], 0
	setl	cl
	dec	ecx
	and	ecx, DWORD PTR _blue$[ebp]
	mov	DWORD PTR _blue$[ebp], ecx

; 580  : 	
; 581  : 	return RGB(red, green, blue);

	mov	eax, DWORD PTR _red$[ebp]
	and	eax, 255				; 000000ffH
	mov	edx, DWORD PTR _green$[ebp]
	and	edx, 255				; 000000ffH
	shl	edx, 8
	or	eax, edx
	mov	ecx, DWORD PTR _blue$[ebp]
	and	ecx, 255				; 000000ffH
	shl	ecx, 16					; 00000010H
	or	eax, ecx
$L76575:

; 582  : }

	mov	esp, ebp
	pop	ebp
	ret	0
?Darker@GraphicsMisc@@YAKKN@Z ENDP			; GraphicsMisc::Darker
_TEXT	ENDS
PUBLIC	?Blend@GraphicsMisc@@YAKKKN@Z			; GraphicsMisc::Blend
;	COMDAT ?Blend@GraphicsMisc@@YAKKKN@Z
_TEXT	SEGMENT
_color1$ = 8
_color2$ = 12
_dAmount$ = 16
_red1$ = -4
_green1$ = -16
_blue1$ = -32
_red2$ = -8
_green2$ = -24
_blue2$ = -36
_redBlend$ = -20
_greenBlend$ = -12
_blueBlend$ = -28
?Blend@GraphicsMisc@@YAKKKN@Z PROC NEAR			; GraphicsMisc::Blend, COMDAT

; 585  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 48					; 00000030H
	push	edi
	lea	edi, DWORD PTR [ebp-48]
	mov	ecx, 12					; 0000000cH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 586  : 	if (color1 == CLR_NONE || color2 == CLR_NONE)

	cmp	DWORD PTR _color1$[ebp], -1
	je	SHORT $L76599
	cmp	DWORD PTR _color2$[ebp], -1
	jne	SHORT $L76598
$L76599:

; 587  : 		return CLR_NONE;

	or	eax, -1
	jmp	$L76597
$L76598:

; 588  : 
; 589  : 	int red1 = GetRValue(color1);

	mov	eax, DWORD PTR _color1$[ebp]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR _red1$[ebp], eax

; 590  : 	int green1 = GetGValue(color1);

	mov	ecx, DWORD PTR _color1$[ebp]
	and	ecx, 65535				; 0000ffffH
	sar	ecx, 8
	and	ecx, 255				; 000000ffH
	mov	DWORD PTR _green1$[ebp], ecx

; 591  : 	int blue1 = GetBValue(color1);

	mov	edx, DWORD PTR _color1$[ebp]
	shr	edx, 16					; 00000010H
	and	edx, 255				; 000000ffH
	mov	DWORD PTR _blue1$[ebp], edx

; 592  : 	
; 593  : 	int red2 = GetRValue(color2);

	mov	eax, DWORD PTR _color2$[ebp]
	and	eax, 255				; 000000ffH
	mov	DWORD PTR _red2$[ebp], eax

; 594  : 	int green2 = GetGValue(color2);

	mov	ecx, DWORD PTR _color2$[ebp]
	and	ecx, 65535				; 0000ffffH
	sar	ecx, 8
	and	ecx, 255				; 000000ffH
	mov	DWORD PTR _green2$[ebp], ecx

; 595  : 	int blue2 = GetBValue(color2);

	mov	edx, DWORD PTR _color2$[ebp]
	shr	edx, 16					; 00000010H
	and	edx, 255				; 000000ffH
	mov	DWORD PTR _blue2$[ebp], edx

; 596  : 	
; 597  : 	int redBlend = (int)((red1 + red2) * dAmount);

	mov	eax, DWORD PTR _red1$[ebp]
	add	eax, DWORD PTR _red2$[ebp]
	mov	DWORD PTR -40+[ebp], eax
	fild	DWORD PTR -40+[ebp]
	fmul	QWORD PTR _dAmount$[ebp]
	call	__ftol
	mov	DWORD PTR _redBlend$[ebp], eax

; 598  : 	int greenBlend = (int)((green1 + green2) * dAmount);

	mov	ecx, DWORD PTR _green1$[ebp]
	add	ecx, DWORD PTR _green2$[ebp]
	mov	DWORD PTR -44+[ebp], ecx
	fild	DWORD PTR -44+[ebp]
	fmul	QWORD PTR _dAmount$[ebp]
	call	__ftol
	mov	DWORD PTR _greenBlend$[ebp], eax

; 599  : 	int blueBlend = (int)((blue1 + blue2) * dAmount);

	mov	edx, DWORD PTR _blue1$[ebp]
	add	edx, DWORD PTR _blue2$[ebp]
	mov	DWORD PTR -48+[ebp], edx
	fild	DWORD PTR -48+[ebp]
	fmul	QWORD PTR _dAmount$[ebp]
	call	__ftol
	mov	DWORD PTR _blueBlend$[ebp], eax

; 600  : 
; 601  : 	return RGB(redBlend, greenBlend, blueBlend);

	mov	eax, DWORD PTR _redBlend$[ebp]
	and	eax, 255				; 000000ffH
	mov	ecx, DWORD PTR _greenBlend$[ebp]
	and	ecx, 255				; 000000ffH
	shl	ecx, 8
	or	eax, ecx
	mov	edx, DWORD PTR _blueBlend$[ebp]
	and	edx, 255				; 000000ffH
	shl	edx, 16					; 00000010H
	or	eax, edx
$L76597:

; 602  : }

	pop	edi
	mov	esp, ebp
	pop	ebp
	ret	0
?Blend@GraphicsMisc@@YAKKKN@Z ENDP			; GraphicsMisc::Blend
_TEXT	ENDS
PUBLIC	?GetScreenWorkArea@GraphicsMisc@@YAHPAUHWND__@@AAVCRect@@I@Z ; GraphicsMisc::GetScreenWorkArea
EXTRN	__imp__GetMonitorInfoW@8:NEAR
EXTRN	??4CRect@@QAEXABUtagRECT@@@Z:NEAR		; CRect::operator=
EXTRN	__imp__MonitorFromWindow@8:NEAR
;	COMDAT ?GetScreenWorkArea@GraphicsMisc@@YAHPAUHWND__@@AAVCRect@@I@Z
_TEXT	SEGMENT
_hWnd$ = 8
_rWorkArea$ = 12
_nMonitor$ = 16
_hMonitor$ = -4
_mi$76637 = -44
?GetScreenWorkArea@GraphicsMisc@@YAHPAUHWND__@@AAVCRect@@I@Z PROC NEAR ; GraphicsMisc::GetScreenWorkArea, COMDAT

; 605  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 44					; 0000002cH
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-44]
	mov	ecx, 11					; 0000000bH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 606  : 	HMONITOR hMonitor = MonitorFromWindow(hWnd, nMonitor);

	mov	esi, esp
	mov	eax, DWORD PTR _nMonitor$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	call	DWORD PTR __imp__MonitorFromWindow@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hMonitor$[ebp], eax
$L76632:

; 607  : 	ASSERT (hMonitor);

	cmp	DWORD PTR _hMonitor$[ebp], 0
	jne	SHORT $L76635
	push	607					; 0000025fH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76635
	int	3
$L76635:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76632

; 608  : 
; 609  : 	if (hMonitor)

	cmp	DWORD PTR _hMonitor$[ebp], 0
	je	SHORT $L76638

; 611  : 		MONITORINFO mi; 
; 612  : 		mi.cbSize = sizeof(mi);

	mov	DWORD PTR _mi$76637[ebp], 40		; 00000028H

; 613  : 		
; 614  : 		if (GetMonitorInfo(hMonitor, &mi))

	mov	esi, esp
	lea	eax, DWORD PTR _mi$76637[ebp]
	push	eax
	mov	ecx, DWORD PTR _hMonitor$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetMonitorInfoW@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L76638

; 616  : 			rWorkArea = mi.rcWork;

	lea	edx, DWORD PTR _mi$76637[ebp+20]
	push	edx
	mov	ecx, DWORD PTR _rWorkArea$[ebp]
	call	??4CRect@@QAEXABUtagRECT@@@Z		; CRect::operator=

; 617  : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L76630
$L76638:

; 620  : 
; 621  : 	// all else
; 622  : 	return FALSE;

	xor	eax, eax
$L76630:

; 623  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 44					; 0000002cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetScreenWorkArea@GraphicsMisc@@YAHPAUHWND__@@AAVCRect@@I@Z ENDP ; GraphicsMisc::GetScreenWorkArea
_TEXT	ENDS
PUBLIC	?ForceIconicRepresentation@GraphicsMisc@@YAHPAUHWND__@@H@Z ; GraphicsMisc::ForceIconicRepresentation
PUBLIC	?DwmSetWindowAttribute@GraphicsMisc@@YAHPAUHWND__@@KPBXK@Z ; GraphicsMisc::DwmSetWindowAttribute
;	COMDAT ?ForceIconicRepresentation@GraphicsMisc@@YAHPAUHWND__@@H@Z
_TEXT	SEGMENT
_hWnd$ = 8
_bForce$ = 12
?ForceIconicRepresentation@GraphicsMisc@@YAHPAUHWND__@@H@Z PROC NEAR ; GraphicsMisc::ForceIconicRepresentation, COMDAT

; 626  : {

	push	ebp
	mov	ebp, esp

; 627  : #ifndef DWMWA_FORCE_ICONIC_REPRESENTATION
; 628  : # define DWMWA_FORCE_ICONIC_REPRESENTATION 7
; 629  : #endif
; 630  : 	
; 631  : 	return DwmSetWindowAttribute(hWnd, DWMWA_FORCE_ICONIC_REPRESENTATION, &bForce, sizeof(bForce));

	push	4
	lea	eax, DWORD PTR _bForce$[ebp]
	push	eax
	push	7
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	call	?DwmSetWindowAttribute@GraphicsMisc@@YAHPAUHWND__@@KPBXK@Z ; GraphicsMisc::DwmSetWindowAttribute
	add	esp, 16					; 00000010H

; 632  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?ForceIconicRepresentation@GraphicsMisc@@YAHPAUHWND__@@H@Z ENDP ; GraphicsMisc::ForceIconicRepresentation
_TEXT	ENDS
PUBLIC	?EnableAeroPeek@GraphicsMisc@@YAHPAUHWND__@@H@Z	; GraphicsMisc::EnableAeroPeek
;	COMDAT ?EnableAeroPeek@GraphicsMisc@@YAHPAUHWND__@@H@Z
_TEXT	SEGMENT
_hWnd$ = 8
_bEnable$ = 12
_bDisallow$ = -4
?EnableAeroPeek@GraphicsMisc@@YAHPAUHWND__@@H@Z PROC NEAR ; GraphicsMisc::EnableAeroPeek, COMDAT

; 635  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 636  : #ifndef DWMWA_DISALLOW_PEEK
; 637  : # define DWMWA_DISALLOW_PEEK 11
; 638  : #endif
; 639  : 	
; 640  : 	BOOL bDisallow = !bEnable;

	xor	eax, eax
	cmp	DWORD PTR _bEnable$[ebp], 0
	sete	al
	mov	DWORD PTR _bDisallow$[ebp], eax

; 641  : 	
; 642  : 	return DwmSetWindowAttribute(hWnd, DWMWA_DISALLOW_PEEK, &bDisallow, sizeof(bDisallow));

	push	4
	lea	ecx, DWORD PTR _bDisallow$[ebp]
	push	ecx
	push	11					; 0000000bH
	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	call	?DwmSetWindowAttribute@GraphicsMisc@@YAHPAUHWND__@@KPBXK@Z ; GraphicsMisc::DwmSetWindowAttribute
	add	esp, 16					; 00000010H

; 643  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?EnableAeroPeek@GraphicsMisc@@YAHPAUHWND__@@H@Z ENDP	; GraphicsMisc::EnableAeroPeek
_TEXT	ENDS
PUBLIC	?EnableFlip3D@GraphicsMisc@@YAHPAUHWND__@@H@Z	; GraphicsMisc::EnableFlip3D
;	COMDAT ?EnableFlip3D@GraphicsMisc@@YAHPAUHWND__@@H@Z
_TEXT	SEGMENT
_hWnd$ = 8
_bEnable$ = 12
_nPolicy$ = -4
?EnableFlip3D@GraphicsMisc@@YAHPAUHWND__@@H@Z PROC NEAR	; GraphicsMisc::EnableFlip3D, COMDAT

; 646  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 647  : #ifndef DWMWA_FLIP3D_POLICY
; 648  : # define DWMWA_FLIP3D_POLICY 8
; 649  : # define DWMFLIP3D_DEFAULT      0
; 650  : # define DWMFLIP3D_EXCLUDEBELOW 1
; 651  : # define DWMFLIP3D_EXCLUDEABOVE 2
; 652  : #endif
; 653  : 	
; 654  : 	int nPolicy = bEnable ? DWMFLIP3D_DEFAULT : DWMFLIP3D_EXCLUDEBELOW;

	xor	eax, eax
	cmp	DWORD PTR _bEnable$[ebp], 0
	sete	al
	mov	DWORD PTR _nPolicy$[ebp], eax

; 655  : 	
; 656  : 	return DwmSetWindowAttribute(hWnd, DWMWA_FLIP3D_POLICY, &nPolicy, sizeof(nPolicy));

	push	4
	lea	ecx, DWORD PTR _nPolicy$[ebp]
	push	ecx
	push	8
	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	call	?DwmSetWindowAttribute@GraphicsMisc@@YAHPAUHWND__@@KPBXK@Z ; GraphicsMisc::DwmSetWindowAttribute
	add	esp, 16					; 00000010H

; 657  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?EnableFlip3D@GraphicsMisc@@YAHPAUHWND__@@H@Z ENDP	; GraphicsMisc::EnableFlip3D
_TEXT	ENDS
EXTRN	__imp__GetProcAddress@8:NEAR
;	COMDAT data
; File D:\_CODE\Shared\GraphicsMisc.cpp
data	SEGMENT
$SG76660 DB	'D', 00H, 'w', 00H, 'm', 00H, 'a', 00H, 'p', 00H, 'i', 00H
	DB	'.', 00H, 'd', 00H, 'l', 00H, 'l', 00H, 00H, 00H
	ORG $+2
$SG76669 DB	'DwmSetWindowAttribute', 00H
data	ENDS
;	COMDAT ?DwmSetWindowAttribute@GraphicsMisc@@YAHPAUHWND__@@KPBXK@Z
_TEXT	SEGMENT
_hWnd$ = 8
_dwAttrib$ = 12
_pData$ = 16
_dwDataSize$ = 20
_hMod$ = -4
_pFn$76667 = -8
_hr$76671 = -12
?DwmSetWindowAttribute@GraphicsMisc@@YAHPAUHWND__@@KPBXK@Z PROC NEAR ; GraphicsMisc::DwmSetWindowAttribute, COMDAT

; 660  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 661  : 	HMODULE hMod = ::LoadLibrary(_T("Dwmapi.dll"));

	mov	esi, esp
	push	OFFSET FLAT:$SG76660
	call	DWORD PTR __imp__LoadLibraryW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hMod$[ebp], eax

; 662  : 	
; 663  : 	if (hMod)

	cmp	DWORD PTR _hMod$[ebp], 0
	je	SHORT $L76670

; 665  : 		typedef HRESULT (WINAPI *PFNDWMSETWINDOWATTRIBUTE)(HWND, DWORD, LPCVOID, DWORD);
; 666  : 		PFNDWMSETWINDOWATTRIBUTE pFn = (PFNDWMSETWINDOWATTRIBUTE)::GetProcAddress(hMod, "DwmSetWindowAttribute");

	mov	esi, esp
	push	OFFSET FLAT:$SG76669
	mov	eax, DWORD PTR _hMod$[ebp]
	push	eax
	call	DWORD PTR __imp__GetProcAddress@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pFn$76667[ebp], eax

; 667  : 		
; 668  : 		if (pFn)

	cmp	DWORD PTR _pFn$76667[ebp], 0
	je	SHORT $L76670

; 670  : 			HRESULT hr = pFn(hWnd, dwAttrib, pData, dwDataSize);

	mov	esi, esp
	mov	ecx, DWORD PTR _dwDataSize$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pData$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwAttrib$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	call	DWORD PTR _pFn$76667[ebp]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hr$76671[ebp], eax

; 671  : 			return SUCCEEDED(hr);

	xor	eax, eax
	cmp	DWORD PTR _hr$76671[ebp], 0
	setge	al
	jmp	SHORT $L76658
$L76670:

; 674  : 	
; 675  : 	return FALSE;

	xor	eax, eax
$L76658:

; 676  : }

	pop	esi
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?DwmSetWindowAttribute@GraphicsMisc@@YAHPAUHWND__@@KPBXK@Z ENDP ; GraphicsMisc::DwmSetWindowAttribute
_TEXT	ENDS
PUBLIC	?DrawSymbol@GraphicsMisc@@YAHPAVCDC@@DABVCRect@@IPAVCFont@@@Z ; GraphicsMisc::DrawSymbol
EXTRN	??BCDC@@QBEPAUHDC__@@XZ:NEAR			; CDC::operator HDC__ *
EXTRN	?SetBkMode@CDC@@QAEHH@Z:NEAR			; CDC::SetBkMode
EXTRN	__imp__DrawTextA@20:NEAR
EXTRN	??BCRect@@QBEPBUtagRECT@@XZ:NEAR		; CRect::operator tagRECT const *
;	COMDAT ?DrawSymbol@GraphicsMisc@@YAHPAVCDC@@DABVCRect@@IPAVCFont@@@Z
_TEXT	SEGMENT
_pDC$ = 8
_cSymbol$ = 12
_rText$ = 16
_nFlags$ = 20
_pFont$ = 24
_pOldFont$ = -4
_nResult$ = -12
_szAnsi$ = -8
?DrawSymbol@GraphicsMisc@@YAHPAVCDC@@DABVCRect@@IPAVCFont@@@Z PROC NEAR ; GraphicsMisc::DrawSymbol, COMDAT

; 679  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 680  : 	if (cSymbol == 0)

	movsx	eax, BYTE PTR _cSymbol$[ebp]
	test	eax, eax
	jne	SHORT $L76680

; 681  : 		return 0;

	xor	eax, eax
	jmp	$L76679
$L76680:

; 682  : 
; 683  : 	CFont* pOldFont = pFont ? pDC->SelectObject(pFont) : NULL;

	cmp	DWORD PTR _pFont$[ebp], 0
	je	SHORT $L77321
	mov	esi, esp
	mov	ecx, DWORD PTR _pFont$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pDC$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	DWORD PTR [eax+48]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR -16+[ebp], eax
	jmp	SHORT $L77322
$L77321:
	mov	DWORD PTR -16+[ebp], 0
$L77322:
	mov	ecx, DWORD PTR -16+[ebp]
	mov	DWORD PTR _pOldFont$[ebp], ecx

; 684  : 	pDC->SetBkMode(TRANSPARENT);

	push	1
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?SetBkMode@CDC@@QAEHH@Z			; CDC::SetBkMode

; 685  : 	int nResult = 0;

	mov	DWORD PTR _nResult$[ebp], 0

; 686  : 
; 687  : 	// draw as ANSI string
; 688  : 	char szAnsi[2] = { cSymbol, 0 };

	mov	dl, BYTE PTR _cSymbol$[ebp]
	mov	BYTE PTR _szAnsi$[ebp], dl
	mov	BYTE PTR _szAnsi$[ebp+1], 0

; 689  : 	nResult = ::DrawTextA(*pDC, szAnsi, 1, (LPRECT)(LPCRECT)rText, nFlags);

	mov	esi, esp
	mov	eax, DWORD PTR _nFlags$[ebp]
	push	eax
	mov	ecx, DWORD PTR _rText$[ebp]
	call	??BCRect@@QBEPBUtagRECT@@XZ		; CRect::operator tagRECT const *
	push	eax
	push	1
	lea	ecx, DWORD PTR _szAnsi$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	??BCDC@@QBEPAUHDC__@@XZ			; CDC::operator HDC__ *
	push	eax
	call	DWORD PTR __imp__DrawTextA@20
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nResult$[ebp], eax

; 690  : 	
; 691  : 	if (pFont)

	cmp	DWORD PTR _pFont$[ebp], 0
	je	SHORT $L76686

; 692  : 		pDC->SelectObject(pOldFont);

	mov	esi, esp
	mov	edx, DWORD PTR _pOldFont$[ebp]
	push	edx
	mov	eax, DWORD PTR _pDC$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	DWORD PTR [edx+48]
	cmp	esi, esp
	call	__chkesp
$L76686:

; 693  : 
; 694  : 	return nResult;

	mov	eax, DWORD PTR _nResult$[ebp]
$L76679:

; 695  : }

	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?DrawSymbol@GraphicsMisc@@YAHPAVCDC@@DABVCRect@@IPAVCFont@@@Z ENDP ; GraphicsMisc::DrawSymbol
_TEXT	ENDS
EXTRN	??0CPen@@QAE@XZ:NEAR				; CPen::CPen
EXTRN	?CreatePen@CPen@@QAEHHHK@Z:NEAR			; CPen::CreatePen
EXTRN	??1CPen@@UAE@XZ:NEAR				; CPen::~CPen
EXTRN	??0CBrush@@QAE@XZ:NEAR				; CBrush::CBrush
EXTRN	?CreateSolidBrush@CBrush@@QAEHK@Z:NEAR		; CBrush::CreateSolidBrush
EXTRN	?FillSolidRect@CDC@@QAEXHHHHK@Z:NEAR		; CDC::FillSolidRect
EXTRN	??1CBrush@@UAE@XZ:NEAR				; CBrush::~CBrush
EXTRN	?SelectObject@CDC@@QAEPAVCPen@@PAV2@@Z:NEAR	; CDC::SelectObject
EXTRN	?SelectObject@CDC@@QAEPAVCBrush@@PAV2@@Z:NEAR	; CDC::SelectObject
EXTRN	??0CPoint@@QAE@HH@Z:NEAR			; CPoint::CPoint
EXTRN	?RoundRect@CDC@@QAEHPBUtagRECT@@UtagPOINT@@@Z:NEAR ; CDC::RoundRect
EXTRN	?IsRectEmpty@CRect@@QBEHXZ:NEAR			; CRect::IsRectEmpty
;	COMDAT xdata$x
; File D:\_CODE\Shared\GraphicsMisc.cpp
xdata$x	SEGMENT
$T77330	DD	019930520H
	DD	02H
	DD	FLAT:$T77332
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77332	DD	0ffffffffH
	DD	FLAT:$L77327
	DD	00H
	DD	FLAT:$L77328
xdata$x	ENDS
;	COMDAT ?DrawRect@GraphicsMisc@@YAXPAVCDC@@ABVCRect@@KKHK@Z
_TEXT	SEGMENT
$T77325 = -60
$T77326 = -68
__$EHRec$ = -12
_pDC$ = 8
_rect$ = 12
_crFill$ = 16
_crBorder$ = 20
_nCornerRadius$ = 24
_dwEdges$ = 28
_rFill$76700 = -28
_pOldPen$76716 = -44
_penBorder$76717 = -52
_pOldBrush$76718 = -40
_brFill$76719 = -36
?DrawRect@GraphicsMisc@@YAXPAVCDC@@ABVCRect@@KKHK@Z PROC NEAR ; GraphicsMisc::DrawRect, COMDAT

; 698  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77331
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 56					; 00000038H
	push	edi
	lea	edi, DWORD PTR [ebp-68]
	mov	ecx, 14					; 0000000eH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 699  : 	if (rect.IsRectEmpty())

	mov	ecx, DWORD PTR _rect$[ebp]
	call	?IsRectEmpty@CRect@@QBEHXZ		; CRect::IsRectEmpty
	test	eax, eax
	je	SHORT $L76695

; 700  : 		return;

	jmp	$L76694
$L76695:

; 701  : 
; 702  : 	if (nCornerRadius == 0)

	cmp	DWORD PTR _nCornerRadius$[ebp], 0
	jne	$L76696

; 704  : 		// can't have border color and no edges
; 705  : 		if (dwEdges == 0)

	cmp	DWORD PTR _dwEdges$[ebp], 0
	jne	SHORT $L76697

; 706  : 			crBorder = CLR_NONE;

	mov	DWORD PTR _crBorder$[ebp], -1
$L76697:

; 707  : 
; 708  : 		// if both colours are set there's an optimisation we can do
; 709  : 		if ((crFill != CLR_NONE) && (crBorder != CLR_NONE))

	cmp	DWORD PTR _crFill$[ebp], -1
	je	$L76698
	cmp	DWORD PTR _crBorder$[ebp], -1
	je	$L76698

; 711  : 			pDC->FillSolidRect(rect, crBorder);

	mov	eax, DWORD PTR _crBorder$[ebp]
	push	eax
	mov	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QBEPBUtagRECT@@XZ		; CRect::operator tagRECT const *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect

; 712  : 
; 713  : 			if (crFill != crBorder)

	mov	ecx, DWORD PTR _crFill$[ebp]
	cmp	ecx, DWORD PTR _crBorder$[ebp]
	je	$L76699

; 715  : 				CRect rFill(rect);

	mov	edx, DWORD PTR _rect$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _rFill$76700[ebp], eax
	mov	ecx, DWORD PTR [edx+4]
	mov	DWORD PTR _rFill$76700[ebp+4], ecx
	mov	eax, DWORD PTR [edx+8]
	mov	DWORD PTR _rFill$76700[ebp+8], eax
	mov	ecx, DWORD PTR [edx+12]
	mov	DWORD PTR _rFill$76700[ebp+12], ecx

; 716  : 
; 717  : 				if ((dwEdges & GMDR_ALL) == GMDR_ALL)

	mov	edx, DWORD PTR _dwEdges$[ebp]
	and	edx, 15					; 0000000fH
	cmp	edx, 15					; 0000000fH
	jne	SHORT $L76701

; 719  : 					rFill.DeflateRect(1, 1);

	push	1
	push	1
	lea	ecx, DWORD PTR _rFill$76700[ebp]
	call	?DeflateRect@CRect@@QAEXHH@Z		; CRect::DeflateRect

; 721  : 				else

	jmp	SHORT $L76706
$L76701:

; 723  : 					if (dwEdges & GMDR_LEFT)

	mov	eax, DWORD PTR _dwEdges$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L76703

; 724  : 						rFill.left++;

	mov	ecx, DWORD PTR _rFill$76700[ebp]
	add	ecx, 1
	mov	DWORD PTR _rFill$76700[ebp], ecx
$L76703:

; 725  : 
; 726  : 					if (dwEdges & GMDR_TOP)

	mov	edx, DWORD PTR _dwEdges$[ebp]
	and	edx, 2
	test	edx, edx
	je	SHORT $L76704

; 727  : 						rFill.top++;

	mov	eax, DWORD PTR _rFill$76700[ebp+4]
	add	eax, 1
	mov	DWORD PTR _rFill$76700[ebp+4], eax
$L76704:

; 728  : 
; 729  : 					if (dwEdges & GMDR_RIGHT)

	mov	ecx, DWORD PTR _dwEdges$[ebp]
	and	ecx, 4
	test	ecx, ecx
	je	SHORT $L76705

; 730  : 						rFill.right--;

	mov	edx, DWORD PTR _rFill$76700[ebp+8]
	sub	edx, 1
	mov	DWORD PTR _rFill$76700[ebp+8], edx
$L76705:

; 731  : 
; 732  : 					if (dwEdges & GMDR_BOTTOM)

	mov	eax, DWORD PTR _dwEdges$[ebp]
	and	eax, 8
	test	eax, eax
	je	SHORT $L76706

; 733  : 						rFill.bottom--;

	mov	ecx, DWORD PTR _rFill$76700[ebp+12]
	sub	ecx, 1
	mov	DWORD PTR _rFill$76700[ebp+12], ecx
$L76706:

; 735  : 
; 736  : 				pDC->FillSolidRect(rFill, crFill);

	mov	edx, DWORD PTR _crFill$[ebp]
	push	edx
	lea	ecx, DWORD PTR _rFill$76700[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect
$L76699:

; 739  : 		else if (crFill != CLR_NONE) // inside of rect

	jmp	$L76714
$L76698:
	cmp	DWORD PTR _crFill$[ebp], -1
	je	SHORT $L76708

; 741  : 			pDC->FillSolidRect(rect, crFill);

	mov	eax, DWORD PTR _crFill$[ebp]
	push	eax
	mov	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QBEPBUtagRECT@@XZ		; CRect::operator tagRECT const *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect

; 743  : 		else if (crBorder != CLR_NONE) // border

	jmp	$L76714
$L76708:
	cmp	DWORD PTR _crBorder$[ebp], -1
	je	$L76714

; 745  : 			if (dwEdges & GMDR_TOP)

	mov	ecx, DWORD PTR _dwEdges$[ebp]
	and	ecx, 2
	test	ecx, ecx
	je	SHORT $L76711

; 746  : 				pDC->FillSolidRect(rect.left, rect.top, rect.Width(), 1, crBorder);

	mov	edx, DWORD PTR _crBorder$[ebp]
	push	edx
	push	1
	mov	ecx, DWORD PTR _rect$[ebp]
	call	?Width@CRect@@QBEHXZ			; CRect::Width
	push	eax
	mov	eax, DWORD PTR _rect$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	mov	edx, DWORD PTR _rect$[ebp]
	mov	eax, DWORD PTR [edx]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXHHHHK@Z		; CDC::FillSolidRect
$L76711:

; 747  : 
; 748  : 			if (dwEdges & GMDR_BOTTOM)

	mov	ecx, DWORD PTR _dwEdges$[ebp]
	and	ecx, 8
	test	ecx, ecx
	je	SHORT $L76712

; 749  : 				pDC->FillSolidRect(rect.left, rect.bottom - 1, rect.Width(), 1, crBorder);

	mov	edx, DWORD PTR _crBorder$[ebp]
	push	edx
	push	1
	mov	ecx, DWORD PTR _rect$[ebp]
	call	?Width@CRect@@QBEHXZ			; CRect::Width
	push	eax
	mov	eax, DWORD PTR _rect$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	sub	ecx, 1
	push	ecx
	mov	edx, DWORD PTR _rect$[ebp]
	mov	eax, DWORD PTR [edx]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXHHHHK@Z		; CDC::FillSolidRect
$L76712:

; 750  : 
; 751  : 			if (dwEdges & GMDR_LEFT)

	mov	ecx, DWORD PTR _dwEdges$[ebp]
	and	ecx, 1
	test	ecx, ecx
	je	SHORT $L76713

; 752  : 				pDC->FillSolidRect(rect.left, rect.top, 1, rect.Height(), crBorder);

	mov	edx, DWORD PTR _crBorder$[ebp]
	push	edx
	mov	ecx, DWORD PTR _rect$[ebp]
	call	?Height@CRect@@QBEHXZ			; CRect::Height
	push	eax
	push	1
	mov	eax, DWORD PTR _rect$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	mov	edx, DWORD PTR _rect$[ebp]
	mov	eax, DWORD PTR [edx]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXHHHHK@Z		; CDC::FillSolidRect
$L76713:

; 753  : 
; 754  : 			if (dwEdges & GMDR_RIGHT)

	mov	ecx, DWORD PTR _dwEdges$[ebp]
	and	ecx, 4
	test	ecx, ecx
	je	SHORT $L76714

; 755  : 				pDC->FillSolidRect(rect.right - 1, rect.top, 1, rect.Height(), crBorder);

	mov	edx, DWORD PTR _crBorder$[ebp]
	push	edx
	mov	ecx, DWORD PTR _rect$[ebp]
	call	?Height@CRect@@QBEHXZ			; CRect::Height
	push	eax
	push	1
	mov	eax, DWORD PTR _rect$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	mov	edx, DWORD PTR _rect$[ebp]
	mov	eax, DWORD PTR [edx+8]
	sub	eax, 1
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXHHHHK@Z		; CDC::FillSolidRect
$L76714:

; 758  : 	else // round-rect

	jmp	$L76715
$L76696:

; 760  : 		CPen* pOldPen = NULL, penBorder;

	mov	DWORD PTR _pOldPen$76716[ebp], 0
	lea	ecx, DWORD PTR _penBorder$76717[ebp]
	call	??0CPen@@QAE@XZ				; CPen::CPen
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 761  : 		CBrush* pOldBrush = NULL, brFill;

	mov	DWORD PTR _pOldBrush$76718[ebp], 0
	lea	ecx, DWORD PTR _brFill$76719[ebp]
	call	??0CBrush@@QAE@XZ			; CBrush::CBrush
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 762  : 
; 763  : 		// inside of rect
; 764  : 		if (crFill != CLR_NONE)

	cmp	DWORD PTR _crFill$[ebp], -1
	je	SHORT $L76720

; 766  : 			brFill.CreateSolidBrush(crFill);

	mov	ecx, DWORD PTR _crFill$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _brFill$76719[ebp]
	call	?CreateSolidBrush@CBrush@@QAEHK@Z	; CBrush::CreateSolidBrush

; 767  : 			pOldBrush = pDC->SelectObject(&brFill);

	lea	edx, DWORD PTR _brFill$76719[ebp]
	push	edx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?SelectObject@CDC@@QAEPAVCBrush@@PAV2@@Z ; CDC::SelectObject
	mov	DWORD PTR _pOldBrush$76718[ebp], eax
$L76720:

; 769  : 
; 770  : 		// border
; 771  : 		if (crBorder != CLR_NONE)

	cmp	DWORD PTR _crBorder$[ebp], -1
	je	SHORT $L76721

; 773  : 			penBorder.CreatePen(PS_SOLID, 1, crBorder);

	mov	eax, DWORD PTR _crBorder$[ebp]
	push	eax
	push	1
	push	0
	lea	ecx, DWORD PTR _penBorder$76717[ebp]
	call	?CreatePen@CPen@@QAEHHHK@Z		; CPen::CreatePen

; 774  : 			pOldPen = pDC->SelectObject(&penBorder);

	lea	ecx, DWORD PTR _penBorder$76717[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?SelectObject@CDC@@QAEPAVCPen@@PAV2@@Z	; CDC::SelectObject
	mov	DWORD PTR _pOldPen$76716[ebp], eax
$L76721:

; 776  : 
; 777  : 		pDC->RoundRect(rect, CPoint(nCornerRadius, nCornerRadius));

	mov	edx, DWORD PTR _nCornerRadius$[ebp]
	push	edx
	mov	eax, DWORD PTR _nCornerRadius$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77326[ebp]
	call	??0CPoint@@QAE@HH@Z			; CPoint::CPoint
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR $T77325[ebp], ecx
	mov	DWORD PTR $T77325[ebp+4], edx
	mov	eax, DWORD PTR $T77325[ebp+4]
	push	eax
	mov	ecx, DWORD PTR $T77325[ebp]
	push	ecx
	mov	ecx, DWORD PTR _rect$[ebp]
	call	??BCRect@@QBEPBUtagRECT@@XZ		; CRect::operator tagRECT const *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?RoundRect@CDC@@QAEHPBUtagRECT@@UtagPOINT@@@Z ; CDC::RoundRect

; 778  : 
; 779  : 		// cleanup
; 780  : 		pDC->SelectObject(pOldBrush);

	mov	edx, DWORD PTR _pOldBrush$76718[ebp]
	push	edx
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?SelectObject@CDC@@QAEPAVCBrush@@PAV2@@Z ; CDC::SelectObject

; 781  : 		pDC->SelectObject(pOldPen);

	mov	eax, DWORD PTR _pOldPen$76716[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?SelectObject@CDC@@QAEPAVCPen@@PAV2@@Z	; CDC::SelectObject

; 782  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _brFill$76719[ebp]
	call	??1CBrush@@UAE@XZ			; CBrush::~CBrush
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _penBorder$76717[ebp]
	call	??1CPen@@UAE@XZ				; CPen::~CPen
$L76715:
$L76694:

; 783  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 68					; 00000044H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77327:
	lea	ecx, DWORD PTR _penBorder$76717[ebp]
	call	??1CPen@@UAE@XZ				; CPen::~CPen
	ret	0
$L77328:
	lea	ecx, DWORD PTR _brFill$76719[ebp]
	call	??1CBrush@@UAE@XZ			; CBrush::~CBrush
	ret	0
$L77331:
	mov	eax, OFFSET FLAT:$T77330
	jmp	___CxxFrameHandler
text$x	ENDS
?DrawRect@GraphicsMisc@@YAXPAVCDC@@ABVCRect@@KKHK@Z ENDP ; GraphicsMisc::DrawRect
PUBLIC	?GetAvailableScreenSpace@GraphicsMisc@@YAHABVCRect@@AAV2@@Z ; GraphicsMisc::GetAvailableScreenSpace
EXTRN	?CenterPoint@CRect@@QBE?AVCPoint@@XZ:NEAR	; CRect::CenterPoint
EXTRN	__imp__MonitorFromPoint@12:NEAR
;	COMDAT ?GetAvailableScreenSpace@GraphicsMisc@@YAHABVCRect@@AAV2@@Z
_TEXT	SEGMENT
$T77337 = -52
$T77338 = -60
_rWnd$ = 8
_rScreen$ = 12
_hMon$ = -44
_mi$ = -40
?GetAvailableScreenSpace@GraphicsMisc@@YAHABVCRect@@AAV2@@Z PROC NEAR ; GraphicsMisc::GetAvailableScreenSpace, COMDAT

; 786  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 60					; 0000003cH
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-60]
	mov	ecx, 15					; 0000000fH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 787  : 	if (rWnd.IsRectEmpty())

	mov	ecx, DWORD PTR _rWnd$[ebp]
	call	?IsRectEmpty@CRect@@QBEHXZ		; CRect::IsRectEmpty
	test	eax, eax
	je	SHORT $L76728

; 788  : 		return FALSE;

	xor	eax, eax
	jmp	$L76727
$L76728:

; 789  : 
; 790  : 	HMONITOR hMon = MonitorFromPoint(rWnd.CenterPoint(), MONITOR_DEFAULTTONULL);

	mov	esi, esp
	push	0
	lea	eax, DWORD PTR $T77338[ebp]
	push	eax
	mov	ecx, DWORD PTR _rWnd$[ebp]
	call	?CenterPoint@CRect@@QBE?AVCPoint@@XZ	; CRect::CenterPoint
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR $T77337[ebp], ecx
	mov	DWORD PTR $T77337[ebp+4], edx
	mov	eax, DWORD PTR $T77337[ebp+4]
	push	eax
	mov	ecx, DWORD PTR $T77337[ebp]
	push	ecx
	call	DWORD PTR __imp__MonitorFromPoint@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hMon$[ebp], eax

; 791  : 
; 792  : 	if (hMon == NULL)

	cmp	DWORD PTR _hMon$[ebp], 0
	jne	SHORT $L76732

; 793  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L76727
$L76732:

; 794  : 
; 795  : 	MONITORINFO mi = { sizeof(MONITORINFO), 0 };

	mov	DWORD PTR _mi$[ebp], 40			; 00000028H
	mov	DWORD PTR _mi$[ebp+4], 0
	mov	ecx, 8
	xor	eax, eax
	lea	edi, DWORD PTR _mi$[ebp+8]
	rep stosd

; 796  : 	
; 797  : 	if (GetMonitorInfo(hMon, &mi))

	mov	esi, esp
	lea	edx, DWORD PTR _mi$[ebp]
	push	edx
	mov	eax, DWORD PTR _hMon$[ebp]
	push	eax
	call	DWORD PTR __imp__GetMonitorInfoW@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L76735

; 799  : 		rScreen = mi.rcWork;

	lea	ecx, DWORD PTR _mi$[ebp+20]
	push	ecx
	mov	ecx, DWORD PTR _rScreen$[ebp]
	call	??4CRect@@QAEXABUtagRECT@@@Z		; CRect::operator=

; 800  : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76727
$L76735:

; 802  : 
; 803  : 	// else
; 804  : 	ASSERT(0);

	mov	edx, 1
	test	edx, edx
	je	SHORT $L76739
	push	804					; 00000324H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76739
	int	3
$L76739:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76735

; 805  : 	return FALSE;

	xor	eax, eax
$L76727:

; 806  : 
; 807  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 60					; 0000003cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetAvailableScreenSpace@GraphicsMisc@@YAHABVCRect@@AAV2@@Z ENDP ; GraphicsMisc::GetAvailableScreenSpace
_TEXT	ENDS
PUBLIC	?GetAvailableScreenSpace@GraphicsMisc@@YAHPAUHWND__@@AAVCRect@@@Z ; GraphicsMisc::GetAvailableScreenSpace
EXTRN	??0CRect@@QAE@XZ:NEAR				; CRect::CRect
EXTRN	__imp__GetWindowRect@8:NEAR
;	COMDAT ?GetAvailableScreenSpace@GraphicsMisc@@YAHPAUHWND__@@AAVCRect@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
_rScreen$ = 12
_rWnd$ = -16
?GetAvailableScreenSpace@GraphicsMisc@@YAHPAUHWND__@@AAVCRect@@@Z PROC NEAR ; GraphicsMisc::GetAvailableScreenSpace, COMDAT

; 810  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 811  : 	if (hWnd == NULL)

	cmp	DWORD PTR _hWnd$[ebp], 0
	jne	SHORT $L76744

; 812  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L76743
$L76744:

; 813  : 
; 814  : 	// else
; 815  : 	CRect rWnd;

	lea	ecx, DWORD PTR _rWnd$[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 816  : 	::GetWindowRect(hWnd, rWnd);

	lea	ecx, DWORD PTR _rWnd$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	mov	esi, esp
	push	eax
	mov	eax, DWORD PTR _hWnd$[ebp]
	push	eax
	call	DWORD PTR __imp__GetWindowRect@8
	cmp	esi, esp
	call	__chkesp

; 817  : 
; 818  : 	return GetAvailableScreenSpace(rWnd, rScreen);

	mov	ecx, DWORD PTR _rScreen$[ebp]
	push	ecx
	lea	edx, DWORD PTR _rWnd$[ebp]
	push	edx
	call	?GetAvailableScreenSpace@GraphicsMisc@@YAHABVCRect@@AAV2@@Z ; GraphicsMisc::GetAvailableScreenSpace
	add	esp, 8
$L76743:

; 819  : }

	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetAvailableScreenSpace@GraphicsMisc@@YAHPAUHWND__@@AAVCRect@@@Z ENDP ; GraphicsMisc::GetAvailableScreenSpace
_TEXT	ENDS
PUBLIC	?GetAvailableScreenSpace@GraphicsMisc@@YAHAAVCRect@@@Z ; GraphicsMisc::GetAvailableScreenSpace
EXTRN	__imp__SystemParametersInfoW@16:NEAR
;	COMDAT ?GetAvailableScreenSpace@GraphicsMisc@@YAHAAVCRect@@@Z
_TEXT	SEGMENT
_rScreen$ = 8
?GetAvailableScreenSpace@GraphicsMisc@@YAHAAVCRect@@@Z PROC NEAR ; GraphicsMisc::GetAvailableScreenSpace, COMDAT

; 822  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 823  : 	return SystemParametersInfo(SPI_GETWORKAREA, 0, &rScreen, 0);

	mov	esi, esp
	push	0
	mov	eax, DWORD PTR _rScreen$[ebp]
	push	eax
	push	0
	push	48					; 00000030H
	call	DWORD PTR __imp__SystemParametersInfoW@16
	cmp	esi, esp
	call	__chkesp

; 824  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?GetAvailableScreenSpace@GraphicsMisc@@YAHAAVCRect@@@Z ENDP ; GraphicsMisc::GetAvailableScreenSpace
_TEXT	ENDS
PUBLIC	?DrawHorzLine@GraphicsMisc@@YAXPAVCDC@@HHHKK@Z	; GraphicsMisc::DrawHorzLine
EXTRN	??0CRect@@QAE@HHHH@Z:NEAR			; CRect::CRect
;	COMDAT ?DrawHorzLine@GraphicsMisc@@YAXPAVCDC@@HHHKK@Z
_TEXT	SEGMENT
_pDC$ = 8
_nXFrom$ = 12
_nXTo$ = 16
_nYPos$ = 20
_crFrom$ = 24
_crTo$ = 28
_rLine$ = -16
?DrawHorzLine@GraphicsMisc@@YAXPAVCDC@@HHHKK@Z PROC NEAR ; GraphicsMisc::DrawHorzLine, COMDAT

; 827  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 828  : 	CRect rLine(nXFrom, nYPos, nXTo, nYPos + 1);

	mov	eax, DWORD PTR _nYPos$[ebp]
	add	eax, 1
	push	eax
	mov	ecx, DWORD PTR _nXTo$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nYPos$[ebp]
	push	edx
	mov	eax, DWORD PTR _nXFrom$[ebp]
	push	eax
	lea	ecx, DWORD PTR _rLine$[ebp]
	call	??0CRect@@QAE@HHHH@Z			; CRect::CRect

; 829  : 
; 830  : 	if ((crFrom == crTo) || (crTo == CLR_NONE))

	mov	ecx, DWORD PTR _crFrom$[ebp]
	cmp	ecx, DWORD PTR _crTo$[ebp]
	je	SHORT $L76759
	cmp	DWORD PTR _crTo$[ebp], -1
	jne	SHORT $L76758
$L76759:

; 831  : 		pDC->FillSolidRect(rLine, crFrom);

	mov	edx, DWORD PTR _crFrom$[ebp]
	push	edx
	lea	ecx, DWORD PTR _rLine$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect

; 832  : 	else

	jmp	SHORT $L76760
$L76758:

; 833  : 		DrawGradient(pDC, rLine, crFrom, crTo, TRUE, -1);

	push	-1
	push	1
	mov	eax, DWORD PTR _crTo$[ebp]
	push	eax
	mov	ecx, DWORD PTR _crFrom$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _rLine$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	edx, DWORD PTR _pDC$[ebp]
	push	edx
	call	?DrawGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z ; GraphicsMisc::DrawGradient
	add	esp, 24					; 00000018H
$L76760:

; 834  : }

	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?DrawHorzLine@GraphicsMisc@@YAXPAVCDC@@HHHKK@Z ENDP	; GraphicsMisc::DrawHorzLine
_TEXT	ENDS
PUBLIC	?DrawVertLine@GraphicsMisc@@YAXPAVCDC@@HHHKK@Z	; GraphicsMisc::DrawVertLine
;	COMDAT ?DrawVertLine@GraphicsMisc@@YAXPAVCDC@@HHHKK@Z
_TEXT	SEGMENT
_pDC$ = 8
_nYFrom$ = 12
_nYTo$ = 16
_nXPos$ = 20
_crFrom$ = 24
_crTo$ = 28
_rLine$ = -16
?DrawVertLine@GraphicsMisc@@YAXPAVCDC@@HHHKK@Z PROC NEAR ; GraphicsMisc::DrawVertLine, COMDAT

; 837  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 838  : 	CRect rLine(nXPos, nYFrom, nXPos + 1, nYTo);

	mov	eax, DWORD PTR _nYTo$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nXPos$[ebp]
	add	ecx, 1
	push	ecx
	mov	edx, DWORD PTR _nYFrom$[ebp]
	push	edx
	mov	eax, DWORD PTR _nXPos$[ebp]
	push	eax
	lea	ecx, DWORD PTR _rLine$[ebp]
	call	??0CRect@@QAE@HHHH@Z			; CRect::CRect

; 839  : 	
; 840  : 	if ((crFrom == crTo) || (crTo == CLR_NONE))

	mov	ecx, DWORD PTR _crFrom$[ebp]
	cmp	ecx, DWORD PTR _crTo$[ebp]
	je	SHORT $L76771
	cmp	DWORD PTR _crTo$[ebp], -1
	jne	SHORT $L76770
$L76771:

; 841  : 		pDC->FillSolidRect(rLine, crFrom);

	mov	edx, DWORD PTR _crFrom$[ebp]
	push	edx
	lea	ecx, DWORD PTR _rLine$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?FillSolidRect@CDC@@QAEXPBUtagRECT@@K@Z	; CDC::FillSolidRect

; 842  : 	else

	jmp	SHORT $L76772
$L76770:

; 843  : 		DrawGradient(pDC, rLine, crFrom, crTo, FALSE, -1);

	push	-1
	push	0
	mov	eax, DWORD PTR _crTo$[ebp]
	push	eax
	mov	ecx, DWORD PTR _crFrom$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _rLine$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	edx, DWORD PTR _pDC$[ebp]
	push	edx
	call	?DrawGradient@GraphicsMisc@@YAXPAVCDC@@PBUtagRECT@@KKHH@Z ; GraphicsMisc::DrawGradient
	add	esp, 24					; 00000018H
$L76772:

; 844  : }

	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?DrawVertLine@GraphicsMisc@@YAXPAVCDC@@HHHKK@Z ENDP	; GraphicsMisc::DrawVertLine
_TEXT	ENDS
PUBLIC	?GetRTLDrawTextFlags@GraphicsMisc@@YAIPAUHWND__@@@Z ; GraphicsMisc::GetRTLDrawTextFlags
EXTRN	__imp__GetWindowLongW@8:NEAR
;	COMDAT ?GetRTLDrawTextFlags@GraphicsMisc@@YAIPAUHWND__@@@Z
_TEXT	SEGMENT
_hwnd$ = 8
_dwStyle$76781 = -12
_bRTLLayout$76783 = -4
_bRTLReading$76784 = -8
?GetRTLDrawTextFlags@GraphicsMisc@@YAIPAUHWND__@@@Z PROC NEAR ; GraphicsMisc::GetRTLDrawTextFlags, COMDAT

; 847  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L76776:

; 848  : 	ASSERT(hwnd);

	cmp	DWORD PTR _hwnd$[ebp], 0
	jne	SHORT $L76779
	push	848					; 00000350H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76779
	int	3
$L76779:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76776

; 849  : 
; 850  : 	if (hwnd)

	cmp	DWORD PTR _hwnd$[ebp], 0
	je	SHORT $L76780

; 852  : 		DWORD dwStyle = (DWORD)GetWindowLong(hwnd, GWL_EXSTYLE);

	mov	esi, esp
	push	-20					; ffffffecH
	mov	ecx, DWORD PTR _hwnd$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetWindowLongW@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _dwStyle$76781[ebp], eax

; 853  : 		BOOL bRTLLayout = ((dwStyle & WS_EX_LAYOUTRTL) ? TRUE : FALSE);

	mov	edx, DWORD PTR _dwStyle$76781[ebp]
	and	edx, 4194304				; 00400000H
	neg	edx
	sbb	edx, edx
	neg	edx
	mov	DWORD PTR _bRTLLayout$76783[ebp], edx

; 854  : 		BOOL bRTLReading = ((dwStyle & WS_EX_RTLREADING) ? TRUE : FALSE);

	mov	eax, DWORD PTR _dwStyle$76781[ebp]
	and	eax, 8192				; 00002000H
	neg	eax
	sbb	eax, eax
	neg	eax
	mov	DWORD PTR _bRTLReading$76784[ebp], eax

; 855  : 		
; 856  : 		return ((bRTLReading != bRTLLayout) ? DT_RTLREADING : 0);

	mov	eax, DWORD PTR _bRTLReading$76784[ebp]
	sub	eax, DWORD PTR _bRTLLayout$76783[ebp]
	neg	eax
	sbb	eax, eax
	and	eax, 131072				; 00020000H
	jmp	SHORT $L76775
$L76780:

; 858  : 
; 859  : 	// else
; 860  : 	return 0;

	xor	eax, eax
$L76775:

; 861  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetRTLDrawTextFlags@GraphicsMisc@@YAIPAUHWND__@@@Z ENDP ; GraphicsMisc::GetRTLDrawTextFlags
_TEXT	ENDS
PUBLIC	?GetRTLDrawTextFlags@GraphicsMisc@@YAIPAVCDC@@@Z ; GraphicsMisc::GetRTLDrawTextFlags
EXTRN	?GetWindow@CDC@@QBEPAVCWnd@@XZ:NEAR		; CDC::GetWindow
EXTRN	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ:NEAR	; CWnd::GetSafeHwnd
;	COMDAT ?GetRTLDrawTextFlags@GraphicsMisc@@YAIPAVCDC@@@Z
_TEXT	SEGMENT
_pDC$ = 8
?GetRTLDrawTextFlags@GraphicsMisc@@YAIPAVCDC@@@Z PROC NEAR ; GraphicsMisc::GetRTLDrawTextFlags, COMDAT

; 864  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L76788:

; 865  : 	ASSERT(pDC && pDC->GetWindow());

	cmp	DWORD PTR _pDC$[ebp], 0
	je	SHORT $L76792
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetWindow@CDC@@QBEPAVCWnd@@XZ		; CDC::GetWindow
	test	eax, eax
	jne	SHORT $L76791
$L76792:
	push	865					; 00000361H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76791
	int	3
$L76791:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76788

; 866  : 
; 867  : 	if (pDC && pDC->GetWindow())

	cmp	DWORD PTR _pDC$[ebp], 0
	je	SHORT $L76793
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetWindow@CDC@@QBEPAVCWnd@@XZ		; CDC::GetWindow
	test	eax, eax
	je	SHORT $L76793

; 868  : 		return GetRTLDrawTextFlags(pDC->GetWindow()->GetSafeHwnd());

	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetWindow@CDC@@QBEPAVCWnd@@XZ		; CDC::GetWindow
	mov	ecx, eax
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	push	eax
	call	?GetRTLDrawTextFlags@GraphicsMisc@@YAIPAUHWND__@@@Z ; GraphicsMisc::GetRTLDrawTextFlags
	add	esp, 4
	jmp	SHORT $L76787
$L76793:

; 869  : 
; 870  : 	// else
; 871  : 	return 0;

	xor	eax, eax
$L76787:

; 872  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?GetRTLDrawTextFlags@GraphicsMisc@@YAIPAVCDC@@@Z ENDP	; GraphicsMisc::GetRTLDrawTextFlags
_TEXT	ENDS
PUBLIC	?GetWebColor@GraphicsMisc@@YA?AVCString@@K@Z	; GraphicsMisc::GetWebColor
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
EXTRN	?Format@CString@@QAAXPBGZZ:NEAR			; CString::Format
;	COMDAT data
; File D:\_CODE\Shared\GraphicsMisc.cpp
data	SEGMENT
$SG76806 DB	'#', 00H, '%', 00H, '0', 00H, '2', 00H, 'X', 00H, '%', 00H
	DB	'0', 00H, '2', 00H, 'X', 00H, '%', 00H, '0', 00H, '2', 00H, 'X'
	DB	00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77358	DD	019930520H
	DD	02H
	DD	FLAT:$T77360
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77360	DD	0ffffffffH
	DD	FLAT:$L77354
	DD	00H
	DD	FLAT:$L77353
xdata$x	ENDS
;	COMDAT ?GetWebColor@GraphicsMisc@@YA?AVCString@@K@Z
_TEXT	SEGMENT
_cGreen$ = -24
_cBlue$ = -20
_sColor$ = -28
$T77355 = -32
__$EHRec$ = -12
_color$ = 12
___$ReturnUdt$ = 8
_cRed$ = -16
?GetWebColor@GraphicsMisc@@YA?AVCString@@K@Z PROC NEAR	; GraphicsMisc::GetWebColor, COMDAT

; 875  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77359
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
	mov	DWORD PTR $T77355[ebp], 0

; 876  : 	unsigned char cRed = GetRValue(color);

	mov	al, BYTE PTR _color$[ebp]
	mov	BYTE PTR _cRed$[ebp], al

; 877  : 	unsigned char cGreen = GetGValue(color);

	mov	ecx, DWORD PTR _color$[ebp]
	and	ecx, 65535				; 0000ffffH
	sar	ecx, 8
	mov	BYTE PTR _cGreen$[ebp], cl

; 878  : 	unsigned char cBlue = GetBValue(color);

	mov	edx, DWORD PTR _color$[ebp]
	shr	edx, 16					; 00000010H
	mov	BYTE PTR _cBlue$[ebp], dl

; 879  : 	
; 880  : 	CString sColor;

	lea	ecx, DWORD PTR _sColor$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 881  : 	sColor.Format(_T("#%02X%02X%02X"), cRed, cGreen, cBlue);

	mov	eax, DWORD PTR _cBlue$[ebp]
	and	eax, 255				; 000000ffH
	push	eax
	mov	ecx, DWORD PTR _cGreen$[ebp]
	and	ecx, 255				; 000000ffH
	push	ecx
	mov	edx, DWORD PTR _cRed$[ebp]
	and	edx, 255				; 000000ffH
	push	edx
	push	OFFSET FLAT:$SG76806
	lea	eax, DWORD PTR _sColor$[ebp]
	push	eax
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 20					; 00000014H

; 882  : 	
; 883  : 	return sColor;

	lea	ecx, DWORD PTR _sColor$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T77355[ebp]
	or	edx, 1
	mov	DWORD PTR $T77355[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sColor$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 884  : }

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
$L77353:
	lea	ecx, DWORD PTR _sColor$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77354:
	mov	eax, DWORD PTR $T77355[ebp]
	and	eax, 1
	test	eax, eax
	je	$L77356
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L77356:
	ret	0
$L77359:
	mov	eax, OFFSET FLAT:$T77358
	jmp	___CxxFrameHandler
text$x	ENDS
?GetWebColor@GraphicsMisc@@YA?AVCString@@K@Z ENDP	; GraphicsMisc::GetWebColor
PUBLIC	?GetIconSize@GraphicsMisc@@YA?AVCSize@@PAUHICON__@@@Z ; GraphicsMisc::GetIconSize
PUBLIC	?GetBitmapSize@GraphicsMisc@@YA?AVCSize@@PAUHBITMAP__@@@Z ; GraphicsMisc::GetBitmapSize
EXTRN	__imp__GetIconInfo@8:NEAR
;	COMDAT ?GetIconSize@GraphicsMisc@@YA?AVCSize@@PAUHICON__@@@Z
_TEXT	SEGMENT
_hIcon$ = 12
___$ReturnUdt$ = 8
_iconinfo$ = -28
_sIcon$ = -8
?GetIconSize@GraphicsMisc@@YA?AVCSize@@PAUHICON__@@@Z PROC NEAR ; GraphicsMisc::GetIconSize, COMDAT

; 887  : {

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

; 888  : 	ICONINFO iconinfo = { 0 };

	mov	DWORD PTR _iconinfo$[ebp], 0
	xor	eax, eax
	mov	DWORD PTR _iconinfo$[ebp+4], eax
	mov	DWORD PTR _iconinfo$[ebp+8], eax
	mov	DWORD PTR _iconinfo$[ebp+12], eax
	mov	DWORD PTR _iconinfo$[ebp+16], eax

; 889  : 	::GetIconInfo(hIcon, &iconinfo);

	mov	esi, esp
	lea	ecx, DWORD PTR _iconinfo$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hIcon$[ebp]
	push	edx
	call	DWORD PTR __imp__GetIconInfo@8
	cmp	esi, esp
	call	__chkesp

; 890  : 
; 891  : 	CSize sIcon = GetBitmapSize(iconinfo.hbmColor);

	mov	eax, DWORD PTR _iconinfo$[ebp+16]
	push	eax
	lea	ecx, DWORD PTR _sIcon$[ebp]
	push	ecx
	call	?GetBitmapSize@GraphicsMisc@@YA?AVCSize@@PAUHBITMAP__@@@Z ; GraphicsMisc::GetBitmapSize
	add	esp, 8

; 892  : 		
; 893  : 	// cleanup
; 894  : 	GraphicsMisc::VerifyDeleteObject(iconinfo.hbmColor);

	mov	edx, DWORD PTR _iconinfo$[ebp+16]
	push	edx
	call	?VerifyDeleteObject@GraphicsMisc@@YAHPAX@Z ; GraphicsMisc::VerifyDeleteObject
	add	esp, 4

; 895  : 	GraphicsMisc::VerifyDeleteObject(iconinfo.hbmMask);

	mov	eax, DWORD PTR _iconinfo$[ebp+12]
	push	eax
	call	?VerifyDeleteObject@GraphicsMisc@@YAHPAX@Z ; GraphicsMisc::VerifyDeleteObject
	add	esp, 4

; 896  : 
; 897  : 	return sIcon;

	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	mov	edx, DWORD PTR _sIcon$[ebp]
	mov	DWORD PTR [ecx], edx
	mov	eax, DWORD PTR _sIcon$[ebp+4]
	mov	DWORD PTR [ecx+4], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 898  : }

	pop	esi
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetIconSize@GraphicsMisc@@YA?AVCSize@@PAUHICON__@@@Z ENDP ; GraphicsMisc::GetIconSize
_TEXT	ENDS
EXTRN	??0CSize@@QAE@HH@Z:NEAR				; CSize::CSize
;	COMDAT ?GetBitmapSize@GraphicsMisc@@YA?AVCSize@@PAUHBITMAP__@@@Z
_TEXT	SEGMENT
_hBmp$ = 12
___$ReturnUdt$ = 8
_bitmap$ = -24
?GetBitmapSize@GraphicsMisc@@YA?AVCSize@@PAUHBITMAP__@@@Z PROC NEAR ; GraphicsMisc::GetBitmapSize, COMDAT

; 901  : {

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

; 902  : 	BITMAP bitmap = { 0 };

	mov	DWORD PTR _bitmap$[ebp], 0
	xor	eax, eax
	mov	DWORD PTR _bitmap$[ebp+4], eax
	mov	DWORD PTR _bitmap$[ebp+8], eax
	mov	DWORD PTR _bitmap$[ebp+12], eax
	mov	DWORD PTR _bitmap$[ebp+16], eax
	mov	DWORD PTR _bitmap$[ebp+20], eax

; 903  : 	::GetObject(hBmp, sizeof(bitmap), &bitmap);

	mov	esi, esp
	lea	ecx, DWORD PTR _bitmap$[ebp]
	push	ecx
	push	24					; 00000018H
	mov	edx, DWORD PTR _hBmp$[ebp]
	push	edx
	call	DWORD PTR __imp__GetObjectW@12
	cmp	esi, esp
	call	__chkesp

; 904  : 
; 905  : 	return CSize(bitmap.bmWidth, bitmap.bmHeight);

	mov	eax, DWORD PTR _bitmap$[ebp+8]
	push	eax
	mov	ecx, DWORD PTR _bitmap$[ebp+4]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CSize@@QAE@HH@Z			; CSize::CSize
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 906  : }

	pop	esi
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetBitmapSize@GraphicsMisc@@YA?AVCSize@@PAUHBITMAP__@@@Z ENDP ; GraphicsMisc::GetBitmapSize
_TEXT	ENDS
PUBLIC	?InitBitmapInfo@@YAXPAUtagBITMAPINFO@@KJJG@Z	; InitBitmapInfo
EXTRN	_memset:NEAR
;	COMDAT ?InitBitmapInfo@@YAXPAUtagBITMAPINFO@@KJJG@Z
_TEXT	SEGMENT
_pbmi$ = 8
_cbInfo$ = 12
_cx$ = 16
_cy$ = 20
_bpp$ = 24
?InitBitmapInfo@@YAXPAUtagBITMAPINFO@@KJJG@Z PROC NEAR	; InitBitmapInfo, COMDAT

; 911  : {

	push	ebp
	mov	ebp, esp

; 912  :     ZeroMemory(pbmi, cbInfo);

	mov	eax, DWORD PTR _cbInfo$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _pbmi$[ebp]
	push	ecx
	call	_memset
	add	esp, 12					; 0000000cH

; 913  : 
; 914  :     pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	mov	edx, DWORD PTR _pbmi$[ebp]
	mov	DWORD PTR [edx], 40			; 00000028H

; 915  :     pbmi->bmiHeader.biPlanes = 1;

	mov	eax, DWORD PTR _pbmi$[ebp]
	mov	WORD PTR [eax+12], 1

; 916  :     pbmi->bmiHeader.biCompression = BI_RGB;

	mov	ecx, DWORD PTR _pbmi$[ebp]
	mov	DWORD PTR [ecx+16], 0

; 917  : 	
; 918  :     pbmi->bmiHeader.biWidth = cx;

	mov	edx, DWORD PTR _pbmi$[ebp]
	mov	eax, DWORD PTR _cx$[ebp]
	mov	DWORD PTR [edx+4], eax

; 919  :     pbmi->bmiHeader.biHeight = cy;

	mov	ecx, DWORD PTR _pbmi$[ebp]
	mov	edx, DWORD PTR _cy$[ebp]
	mov	DWORD PTR [ecx+8], edx

; 920  :     pbmi->bmiHeader.biBitCount = bpp;

	mov	eax, DWORD PTR _pbmi$[ebp]
	mov	cx, WORD PTR _bpp$[ebp]
	mov	WORD PTR [eax+14], cx

; 921  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?InitBitmapInfo@@YAXPAUtagBITMAPINFO@@KJJG@Z ENDP	; InitBitmapInfo
_TEXT	ENDS
PUBLIC	?Create32BitHBITMAP@@YAHPAUHDC__@@PBUtagSIZE@@PAPAXPAPAUHBITMAP__@@@Z ; Create32BitHBITMAP
EXTRN	__imp__CreateDIBSection@24:NEAR
;	COMDAT ?Create32BitHBITMAP@@YAHPAUHDC__@@PBUtagSIZE@@PAPAXPAPAUHBITMAP__@@@Z
_TEXT	SEGMENT
_hdc$ = 8
_psize$ = 12
_ppvBits$ = 16
_phBmp$ = 20
_bmi$ = -48
_hdcUsed$ = -4
?Create32BitHBITMAP@@YAHPAUHDC__@@PBUtagSIZE@@PAPAXPAPAUHBITMAP__@@@Z PROC NEAR ; Create32BitHBITMAP, COMDAT

; 926  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 52					; 00000034H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-52]
	mov	ecx, 13					; 0000000dH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 927  :     *phBmp = NULL;

	mov	eax, DWORD PTR _phBmp$[ebp]
	mov	DWORD PTR [eax], 0

; 928  : 	
; 929  :     BITMAPINFO bmi = { 0 };

	mov	DWORD PTR _bmi$[ebp], 0
	mov	ecx, 10					; 0000000aH
	xor	eax, eax
	lea	edi, DWORD PTR _bmi$[ebp+4]
	rep stosd

; 930  :     InitBitmapInfo(&bmi, sizeof(bmi), psize->cx, psize->cy, 32);

	push	32					; 00000020H
	mov	ecx, DWORD PTR _psize$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	mov	eax, DWORD PTR _psize$[ebp]
	mov	ecx, DWORD PTR [eax]
	push	ecx
	push	44					; 0000002cH
	lea	edx, DWORD PTR _bmi$[ebp]
	push	edx
	call	?InitBitmapInfo@@YAXPAUtagBITMAPINFO@@KJJG@Z ; InitBitmapInfo
	add	esp, 20					; 00000014H

; 931  : 	
; 932  : 	HDC hdcUsed = (hdc ? hdc : ::GetDC(NULL));

	cmp	DWORD PTR _hdc$[ebp], 0
	je	SHORT $L77372
	mov	eax, DWORD PTR _hdc$[ebp]
	mov	DWORD PTR -52+[ebp], eax
	jmp	SHORT $L77373
$L77372:
	mov	esi, esp
	push	0
	call	DWORD PTR __imp__GetDC@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR -52+[ebp], eax
$L77373:
	mov	ecx, DWORD PTR -52+[ebp]
	mov	DWORD PTR _hdcUsed$[ebp], ecx

; 933  : 
; 934  :     if (hdcUsed)

	cmp	DWORD PTR _hdcUsed$[ebp], 0
	je	SHORT $L76837

; 936  :         *phBmp = CreateDIBSection(hdcUsed, &bmi, DIB_RGB_COLORS, ppvBits, NULL, 0);

	mov	esi, esp
	push	0
	push	0
	mov	edx, DWORD PTR _ppvBits$[ebp]
	push	edx
	push	0
	lea	eax, DWORD PTR _bmi$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hdcUsed$[ebp]
	push	ecx
	call	DWORD PTR __imp__CreateDIBSection@24
	cmp	esi, esp
	call	__chkesp
	mov	edx, DWORD PTR _phBmp$[ebp]
	mov	DWORD PTR [edx], eax

; 937  : 
; 938  :         if (hdc != hdcUsed)

	mov	eax, DWORD PTR _hdc$[ebp]
	cmp	eax, DWORD PTR _hdcUsed$[ebp]
	je	SHORT $L76837

; 940  : 			::ReleaseDC(NULL, hdcUsed);

	mov	esi, esp
	mov	ecx, DWORD PTR _hdcUsed$[ebp]
	push	ecx
	push	0
	call	DWORD PTR __imp__ReleaseDC@8
	cmp	esi, esp
	call	__chkesp
$L76837:

; 943  : 	
; 944  :     return (NULL != *phBmp);

	mov	edx, DWORD PTR _phBmp$[ebp]
	xor	eax, eax
	cmp	DWORD PTR [edx], 0
	setne	al

; 945  : }

	pop	edi
	pop	esi
	add	esp, 52					; 00000034H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Create32BitHBITMAP@@YAHPAUHDC__@@PBUtagSIZE@@PAPAXPAPAUHBITMAP__@@@Z ENDP ; Create32BitHBITMAP
_TEXT	ENDS
PUBLIC	?HasAlpha@@YAHPAKAAUtagSIZE@@H@Z		; HasAlpha
;	COMDAT ?HasAlpha@@YAHPAKAAUtagSIZE@@H@Z
_TEXT	SEGMENT
_pargb$ = 8
_sizImage$ = 12
_cxRow$ = 16
_cxDelta$ = -4
_y$ = -8
_x$76848 = -12
?HasAlpha@@YAHPAKAAUtagSIZE@@H@Z PROC NEAR		; HasAlpha, COMDAT

; 950  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 951  :     ULONG cxDelta = (cxRow - sizImage.cx);

	mov	eax, DWORD PTR _sizImage$[ebp]
	mov	ecx, DWORD PTR _cxRow$[ebp]
	sub	ecx, DWORD PTR [eax]
	mov	DWORD PTR _cxDelta$[ebp], ecx

; 952  : 
; 953  :     for (ULONG y = sizImage.cy; y; --y)

	mov	edx, DWORD PTR _sizImage$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR _y$[ebp], eax
	jmp	SHORT $L76845
$L76846:
	mov	ecx, DWORD PTR _y$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _y$[ebp], ecx
$L76845:
	cmp	DWORD PTR _y$[ebp], 0
	je	SHORT $L76847

; 955  :         for (ULONG x = sizImage.cx; x; --x)

	mov	edx, DWORD PTR _sizImage$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _x$76848[ebp], eax
	jmp	SHORT $L76849
$L76850:
	mov	ecx, DWORD PTR _x$76848[ebp]
	sub	ecx, 1
	mov	DWORD PTR _x$76848[ebp], ecx
$L76849:
	cmp	DWORD PTR _x$76848[ebp], 0
	je	SHORT $L76851

; 957  :             if (*pargb++ & 0xFF000000)

	mov	edx, DWORD PTR _pargb$[ebp]
	mov	eax, DWORD PTR [edx]
	and	eax, -16777216				; ff000000H
	mov	ecx, DWORD PTR _pargb$[ebp]
	add	ecx, 4
	mov	DWORD PTR _pargb$[ebp], ecx
	test	eax, eax
	je	SHORT $L76852

; 959  :                 return TRUE;

	mov	eax, 1
	jmp	SHORT $L76842
$L76852:

; 961  :         }

	jmp	SHORT $L76850
$L76851:

; 962  : 		
; 963  :         pargb += cxDelta;

	mov	edx, DWORD PTR _cxDelta$[ebp]
	mov	eax, DWORD PTR _pargb$[ebp]
	lea	ecx, DWORD PTR [eax+edx*4]
	mov	DWORD PTR _pargb$[ebp], ecx

; 964  :     }

	jmp	SHORT $L76846
$L76847:

; 965  : 	
; 966  :     return FALSE;

	xor	eax, eax
$L76842:

; 967  : }

	mov	esp, ebp
	pop	ebp
	ret	0
?HasAlpha@@YAHPAKAAUtagSIZE@@H@Z ENDP			; HasAlpha
_TEXT	ENDS
PUBLIC	?ConvertToPARGB32@@YAHPAUHDC__@@PAKPAUHBITMAP__@@AAUtagSIZE@@H@Z ; ConvertToPARGB32
EXTRN	__imp__HeapAlloc@12:NEAR
EXTRN	__imp__HeapFree@12:NEAR
EXTRN	__imp__GetProcessHeap@0:NEAR
EXTRN	__imp__GetDIBits@28:NEAR
;	COMDAT ?ConvertToPARGB32@@YAHPAUHDC__@@PAKPAUHBITMAP__@@AAUtagSIZE@@H@Z
_TEXT	SEGMENT
_hdc$ = 8
_pargb$ = 12
_hbmp$ = 16
_sizImage$ = 20
_cxRow$ = 24
_bSuccess$ = -52
_bmi$ = -48
_hHeap$ = -56
_pvBits$ = -4
_cxDelta$76866 = -60
_pargbMask$76867 = -68
_y$76869 = -64
_x$76873 = -72
?ConvertToPARGB32@@YAHPAUHDC__@@PAKPAUHBITMAP__@@AAUtagSIZE@@H@Z PROC NEAR ; ConvertToPARGB32, COMDAT

; 972  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 72					; 00000048H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-72]
	mov	ecx, 18					; 00000012H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 973  :     BOOL bSuccess = FALSE;

	mov	DWORD PTR _bSuccess$[ebp], 0

; 974  : 
; 975  :     BITMAPINFO bmi =  { 0 };

	mov	DWORD PTR _bmi$[ebp], 0
	mov	ecx, 10					; 0000000aH
	xor	eax, eax
	lea	edi, DWORD PTR _bmi$[ebp+4]
	rep stosd

; 976  :     InitBitmapInfo(&bmi, sizeof(bmi), sizImage.cx, sizImage.cy, 32);

	push	32					; 00000020H
	mov	eax, DWORD PTR _sizImage$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	mov	edx, DWORD PTR _sizImage$[ebp]
	mov	eax, DWORD PTR [edx]
	push	eax
	push	44					; 0000002cH
	lea	ecx, DWORD PTR _bmi$[ebp]
	push	ecx
	call	?InitBitmapInfo@@YAXPAUtagBITMAPINFO@@KJJG@Z ; InitBitmapInfo
	add	esp, 20					; 00000014H

; 977  : 	
; 978  :     HANDLE hHeap = GetProcessHeap();

	mov	esi, esp
	call	DWORD PTR __imp__GetProcessHeap@0
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hHeap$[ebp], eax

; 979  :     void *pvBits = HeapAlloc(hHeap, 0, bmi.bmiHeader.biWidth * 4 * bmi.bmiHeader.biHeight);

	mov	edx, DWORD PTR _bmi$[ebp+4]
	shl	edx, 2
	imul	edx, DWORD PTR _bmi$[ebp+8]
	mov	esi, esp
	push	edx
	push	0
	mov	eax, DWORD PTR _hHeap$[ebp]
	push	eax
	call	DWORD PTR __imp__HeapAlloc@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pvBits$[ebp], eax

; 980  : 
; 981  :     if (pvBits)

	cmp	DWORD PTR _pvBits$[ebp], 0
	je	$L76864

; 983  :         if (GetDIBits(hdc, hbmp, 0, bmi.bmiHeader.biHeight, pvBits, &bmi, DIB_RGB_COLORS) == bmi.bmiHeader.biHeight)

	mov	esi, esp
	push	0
	lea	ecx, DWORD PTR _bmi$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pvBits$[ebp]
	push	edx
	mov	eax, DWORD PTR _bmi$[ebp+8]
	push	eax
	push	0
	mov	ecx, DWORD PTR _hbmp$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hdc$[ebp]
	push	edx
	call	DWORD PTR __imp__GetDIBits@28
	cmp	esi, esp
	call	__chkesp
	cmp	eax, DWORD PTR _bmi$[ebp+8]
	jne	$L76865

; 985  :             ULONG cxDelta = (cxRow - bmi.bmiHeader.biWidth);

	mov	eax, DWORD PTR _cxRow$[ebp]
	sub	eax, DWORD PTR _bmi$[ebp+4]
	mov	DWORD PTR _cxDelta$76866[ebp], eax

; 986  :             ARGB *pargbMask = static_cast<ARGB *>(pvBits);

	mov	ecx, DWORD PTR _pvBits$[ebp]
	mov	DWORD PTR _pargbMask$76867[ebp], ecx

; 987  : 			
; 988  :             for (ULONG y = bmi.bmiHeader.biHeight; y; --y)

	mov	edx, DWORD PTR _bmi$[ebp+8]
	mov	DWORD PTR _y$76869[ebp], edx
	jmp	SHORT $L76870
$L76871:
	mov	eax, DWORD PTR _y$76869[ebp]
	sub	eax, 1
	mov	DWORD PTR _y$76869[ebp], eax
$L76870:
	cmp	DWORD PTR _y$76869[ebp], 0
	je	SHORT $L76872

; 990  :                 for (ULONG x = bmi.bmiHeader.biWidth; x; --x)

	mov	ecx, DWORD PTR _bmi$[ebp+4]
	mov	DWORD PTR _x$76873[ebp], ecx
	jmp	SHORT $L76874
$L76875:
	mov	edx, DWORD PTR _x$76873[ebp]
	sub	edx, 1
	mov	DWORD PTR _x$76873[ebp], edx
$L76874:
	cmp	DWORD PTR _x$76873[ebp], 0
	je	SHORT $L76876

; 992  :                     if (*pargbMask++)

	mov	eax, DWORD PTR _pargbMask$76867[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR _pargbMask$76867[ebp]
	add	edx, 4
	mov	DWORD PTR _pargbMask$76867[ebp], edx
	test	ecx, ecx
	je	SHORT $L76877

; 994  :                         // transparent pixel
; 995  :                         *pargb++ = 0;

	mov	eax, DWORD PTR _pargb$[ebp]
	mov	DWORD PTR [eax], 0
	mov	ecx, DWORD PTR _pargb$[ebp]
	add	ecx, 4
	mov	DWORD PTR _pargb$[ebp], ecx

; 997  :                     else

	jmp	SHORT $L76878
$L76877:

; 999  :                         // opaque pixel
; 1000 :                         *pargb++ |= 0xFF000000;

	mov	edx, DWORD PTR _pargb$[ebp]
	mov	eax, DWORD PTR [edx]
	or	eax, -16777216				; ff000000H
	mov	ecx, DWORD PTR _pargb$[ebp]
	mov	DWORD PTR [ecx], eax
	mov	edx, DWORD PTR _pargb$[ebp]
	add	edx, 4
	mov	DWORD PTR _pargb$[ebp], edx
$L76878:

; 1002 :                 }

	jmp	SHORT $L76875
$L76876:

; 1003 : 				
; 1004 :                 pargb += cxDelta;

	mov	eax, DWORD PTR _cxDelta$76866[ebp]
	mov	ecx, DWORD PTR _pargb$[ebp]
	lea	edx, DWORD PTR [ecx+eax*4]
	mov	DWORD PTR _pargb$[ebp], edx

; 1005 :             }

	jmp	SHORT $L76871
$L76872:

; 1006 : 			
; 1007 :             bSuccess = TRUE;

	mov	DWORD PTR _bSuccess$[ebp], 1
$L76865:

; 1009 : 		
; 1010 :         HeapFree(hHeap, 0, pvBits);

	mov	esi, esp
	mov	eax, DWORD PTR _pvBits$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _hHeap$[ebp]
	push	ecx
	call	DWORD PTR __imp__HeapFree@12
	cmp	esi, esp
	call	__chkesp
$L76864:

; 1012 : 	
; 1013 :     return bSuccess;

	mov	eax, DWORD PTR _bSuccess$[ebp]

; 1014 : }

	pop	edi
	pop	esi
	add	esp, 72					; 00000048H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?ConvertToPARGB32@@YAHPAUHDC__@@PAKPAUHBITMAP__@@AAUtagSIZE@@H@Z ENDP ; ConvertToPARGB32
_TEXT	ENDS
PUBLIC	?ConvertBufferToPARGB32@@YAHPAXPAUHDC__@@PAUHICON__@@AAUtagSIZE@@@Z ; ConvertBufferToPARGB32
EXTRN	?GetBufferedPaintBits@CThemed@@SAHPAXPAPAUtagRGBQUAD@@PAH@Z:NEAR ; CThemed::GetBufferedPaintBits
;	COMDAT ?ConvertBufferToPARGB32@@YAHPAXPAUHDC__@@PAUHICON__@@AAUtagSIZE@@@Z
_TEXT	SEGMENT
_hPaintBuffer$ = 8
_hdc$ = 12
_hicon$ = 16
_sizIcon$ = 20
_prgbQuad$ = -4
_cxRow$ = -8
_pargb$76888 = -12
_info$76891 = -32
?ConvertBufferToPARGB32@@YAHPAXPAUHDC__@@PAUHICON__@@AAUtagSIZE@@@Z PROC NEAR ; ConvertBufferToPARGB32, COMDAT

; 1019 : {

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

; 1020 :     RGBQUAD *prgbQuad = NULL;

	mov	DWORD PTR _prgbQuad$[ebp], 0

; 1021 :     int cxRow = 0;

	mov	DWORD PTR _cxRow$[ebp], 0

; 1022 : 
; 1023 : 	if (CThemed::GetBufferedPaintBits(hPaintBuffer, &prgbQuad, &cxRow))

	lea	eax, DWORD PTR _cxRow$[ebp]
	push	eax
	lea	ecx, DWORD PTR _prgbQuad$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hPaintBuffer$[ebp]
	push	edx
	call	?GetBufferedPaintBits@CThemed@@SAHPAXPAPAUtagRGBQUAD@@PAH@Z ; CThemed::GetBufferedPaintBits
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	$L76887

; 1025 : 		ARGB *pargb = reinterpret_cast<ARGB *>(prgbQuad);

	mov	eax, DWORD PTR _prgbQuad$[ebp]
	mov	DWORD PTR _pargb$76888[ebp], eax

; 1026 : 
; 1027 : 		if (!HasAlpha(pargb, sizIcon, cxRow))

	mov	ecx, DWORD PTR _cxRow$[ebp]
	push	ecx
	mov	edx, DWORD PTR _sizIcon$[ebp]
	push	edx
	mov	eax, DWORD PTR _pargb$76888[ebp]
	push	eax
	call	?HasAlpha@@YAHPAKAAUtagSIZE@@H@Z	; HasAlpha
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	$L76900

; 1029 : 			ICONINFO info = { 0 };

	mov	DWORD PTR _info$76891[ebp], 0
	xor	ecx, ecx
	mov	DWORD PTR _info$76891[ebp+4], ecx
	mov	DWORD PTR _info$76891[ebp+8], ecx
	mov	DWORD PTR _info$76891[ebp+12], ecx
	mov	DWORD PTR _info$76891[ebp+16], ecx

; 1030 : 
; 1031 : 			if (GetIconInfo(hicon, &info))

	mov	esi, esp
	lea	edx, DWORD PTR _info$76891[ebp]
	push	edx
	mov	eax, DWORD PTR _hicon$[ebp]
	push	eax
	call	DWORD PTR __imp__GetIconInfo@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	$L76900

; 1033 : 				if (info.hbmMask)

	cmp	DWORD PTR _info$76891[ebp+12], 0
	je	SHORT $L76893

; 1035 : 					ConvertToPARGB32(hdc, pargb, info.hbmMask, sizIcon, cxRow);

	mov	ecx, DWORD PTR _cxRow$[ebp]
	push	ecx
	mov	edx, DWORD PTR _sizIcon$[ebp]
	push	edx
	mov	eax, DWORD PTR _info$76891[ebp+12]
	push	eax
	mov	ecx, DWORD PTR _pargb$76888[ebp]
	push	ecx
	mov	edx, DWORD PTR _hdc$[ebp]
	push	edx
	call	?ConvertToPARGB32@@YAHPAUHDC__@@PAKPAUHBITMAP__@@AAUtagSIZE@@H@Z ; ConvertToPARGB32
	add	esp, 20					; 00000014H
$L76893:

; 1037 : 
; 1038 : 				// cleanup
; 1039 : 				VERIFY(DeleteObject(info.hbmColor));

	mov	esi, esp
	mov	eax, DWORD PTR _info$76891[ebp+16]
	push	eax
	call	DWORD PTR __imp__DeleteObject@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76897
	push	1039					; 0000040fH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76897
	int	3
$L76897:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76893
$L76896:

; 1040 : 				VERIFY(DeleteObject(info.hbmMask));

	mov	esi, esp
	mov	edx, DWORD PTR _info$76891[ebp+12]
	push	edx
	call	DWORD PTR __imp__DeleteObject@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76901
	push	1040					; 00000410H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76901
	int	3
$L76901:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76896
$L76900:

; 1043 : 		
; 1044 : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L76884
$L76887:

; 1046 : 	
; 1047 :     return FALSE;

	xor	eax, eax
$L76884:

; 1048 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?ConvertBufferToPARGB32@@YAHPAXPAUHDC__@@PAUHICON__@@AAUtagSIZE@@@Z ENDP ; ConvertBufferToPARGB32
_TEXT	ENDS
PUBLIC	?IconToPARGB32Bitmap@GraphicsMisc@@YAPAUHBITMAP__@@PAUHICON__@@@Z ; GraphicsMisc::IconToPARGB32Bitmap
EXTRN	__imp__DrawIconEx@36:NEAR
EXTRN	__imp__SelectObject@8:NEAR
EXTRN	__imp__CreateCompatibleDC@4:NEAR
EXTRN	__imp__DeleteDC@4:NEAR
EXTRN	?BeginBufferedPaint@CThemed@@SAPAXPAUHDC__@@PBUtagRECT@@W4TH_BUFFERFORMAT@@PAUTH_PAINTPARAMS@@PAPAU2@@Z:NEAR ; CThemed::BeginBufferedPaint
EXTRN	?EndBufferedPaint@CThemed@@SAHPAXH@Z:NEAR	; CThemed::EndBufferedPaint
;	COMDAT ?IconToPARGB32Bitmap@GraphicsMisc@@YAPAUHBITMAP__@@PAUHICON__@@@Z
_TEXT	SEGMENT
_hIcon$ = 8
_hBmp$ = -4
_hdcDest$76907 = -8
_bSuccess$76908 = -12
_sIcon$76910 = -20
_hbmpOld$76913 = -24
_bfAlpha$76916 = -68
_paintParams$76917 = -44
_hdcBuffer$76918 = -28
_rIcon$76919 = -64
_hPaintBuffer$76920 = -48
?IconToPARGB32Bitmap@GraphicsMisc@@YAPAUHBITMAP__@@PAUHICON__@@@Z PROC NEAR ; GraphicsMisc::IconToPARGB32Bitmap, COMDAT

; 1051 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 68					; 00000044H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-68]
	mov	ecx, 17					; 00000011H
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 1052 :     HBITMAP hBmp = NULL;

	mov	DWORD PTR _hBmp$[ebp], 0

; 1053 : 
; 1054 : 	if (hIcon)

	cmp	DWORD PTR _hIcon$[ebp], 0
	je	$L76931

; 1056 : 		HDC hdcDest = CreateCompatibleDC(NULL);

	mov	esi, esp
	push	0
	call	DWORD PTR __imp__CreateCompatibleDC@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hdcDest$76907[ebp], eax

; 1057 : 		BOOL bSuccess = FALSE;

	mov	DWORD PTR _bSuccess$76908[ebp], 0

; 1058 : 
; 1059 : 		if (hdcDest) 

	cmp	DWORD PTR _hdcDest$76907[ebp], 0
	je	$L76929

; 1061 : 			CSize sIcon = GetIconSize(hIcon);

	mov	eax, DWORD PTR _hIcon$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sIcon$76910[ebp]
	push	ecx
	call	?GetIconSize@GraphicsMisc@@YA?AVCSize@@PAUHICON__@@@Z ; GraphicsMisc::GetIconSize
	add	esp, 8

; 1062 : 			
; 1063 : 			if (Create32BitHBITMAP(hdcDest, &sIcon, NULL, &hBmp))

	lea	edx, DWORD PTR _hBmp$[ebp]
	push	edx
	push	0
	lea	eax, DWORD PTR _sIcon$76910[ebp]
	push	eax
	mov	ecx, DWORD PTR _hdcDest$76907[ebp]
	push	ecx
	call	?Create32BitHBITMAP@@YAHPAUHDC__@@PBUtagSIZE@@PAPAXPAPAUHBITMAP__@@@Z ; Create32BitHBITMAP
	add	esp, 16					; 00000010H
	test	eax, eax
	je	$L76915

; 1065 : 				HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcDest, hBmp);

	mov	esi, esp
	mov	edx, DWORD PTR _hBmp$[ebp]
	push	edx
	mov	eax, DWORD PTR _hdcDest$76907[ebp]
	push	eax
	call	DWORD PTR __imp__SelectObject@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hbmpOld$76913[ebp], eax

; 1066 : 
; 1067 : 				if (hbmpOld) 

	cmp	DWORD PTR _hbmpOld$76913[ebp], 0
	je	$L76915

; 1069 : 					BLENDFUNCTION bfAlpha = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	mov	BYTE PTR _bfAlpha$76916[ebp], 0
	mov	BYTE PTR _bfAlpha$76916[ebp+1], 0
	mov	BYTE PTR _bfAlpha$76916[ebp+2], 255	; 000000ffH
	mov	BYTE PTR _bfAlpha$76916[ebp+3], 1

; 1070 : 
; 1071 : 					TH_PAINTPARAMS paintParams = { 0 };

	mov	DWORD PTR _paintParams$76917[ebp], 0
	xor	ecx, ecx
	mov	DWORD PTR _paintParams$76917[ebp+4], ecx
	mov	DWORD PTR _paintParams$76917[ebp+8], ecx
	mov	DWORD PTR _paintParams$76917[ebp+12], ecx

; 1072 : 					paintParams.cbSize = sizeof(paintParams);

	mov	DWORD PTR _paintParams$76917[ebp], 16	; 00000010H

; 1073 : 					paintParams.dwFlags = BPPF_ERASE;

	mov	DWORD PTR _paintParams$76917[ebp+4], 1

; 1074 : 					paintParams.pBlendFunction = &bfAlpha;

	lea	edx, DWORD PTR _bfAlpha$76916[ebp]
	mov	DWORD PTR _paintParams$76917[ebp+12], edx

; 1075 : 					
; 1076 : 					HDC hdcBuffer = NULL;

	mov	DWORD PTR _hdcBuffer$76918[ebp], 0

; 1077 : 					CRect rIcon(0, 0, sIcon.cx, sIcon.cy);

	mov	eax, DWORD PTR _sIcon$76910[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sIcon$76910[ebp]
	push	ecx
	push	0
	push	0
	lea	ecx, DWORD PTR _rIcon$76919[ebp]
	call	??0CRect@@QAE@HHHH@Z			; CRect::CRect

; 1078 : 					HPAINTBUFFER hPaintBuffer = CThemed::BeginBufferedPaint(hdcDest, rIcon, BPBF_DIB, &paintParams, &hdcBuffer);

	lea	edx, DWORD PTR _hdcBuffer$76918[ebp]
	push	edx
	lea	eax, DWORD PTR _paintParams$76917[ebp]
	push	eax
	push	1
	lea	ecx, DWORD PTR _rIcon$76919[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _hdcDest$76907[ebp]
	push	ecx
	call	?BeginBufferedPaint@CThemed@@SAPAXPAUHDC__@@PBUtagRECT@@W4TH_BUFFERFORMAT@@PAUTH_PAINTPARAMS@@PAPAU2@@Z ; CThemed::BeginBufferedPaint
	add	esp, 20					; 00000014H
	mov	DWORD PTR _hPaintBuffer$76920[ebp], eax

; 1079 : 
; 1080 : 					if (hPaintBuffer) 

	cmp	DWORD PTR _hPaintBuffer$76920[ebp], 0
	je	SHORT $L76921
$L76922:

; 1082 : 						ASSERT(hdcBuffer);

	cmp	DWORD PTR _hdcBuffer$76918[ebp], 0
	jne	SHORT $L76925
	push	1082					; 0000043aH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76925
	int	3
$L76925:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76922

; 1083 : 
; 1084 : 						if (DrawIconEx(hdcBuffer, 0, 0, hIcon, sIcon.cx, sIcon.cy, 0, NULL, DI_NORMAL)) 

	mov	esi, esp
	push	3
	push	0
	push	0
	mov	eax, DWORD PTR _sIcon$76910[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sIcon$76910[ebp]
	push	ecx
	mov	edx, DWORD PTR _hIcon$[ebp]
	push	edx
	push	0
	push	0
	mov	eax, DWORD PTR _hdcBuffer$76918[ebp]
	push	eax
	call	DWORD PTR __imp__DrawIconEx@36
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L76926

; 1086 : 							// If icon did not have an alpha channel, we need to convert buffer to PARGB.
; 1087 : 							bSuccess = ConvertBufferToPARGB32(hPaintBuffer, hdcDest, hIcon, sIcon);

	lea	ecx, DWORD PTR _sIcon$76910[ebp]
	push	ecx
	mov	edx, DWORD PTR _hIcon$[ebp]
	push	edx
	mov	eax, DWORD PTR _hdcDest$76907[ebp]
	push	eax
	mov	ecx, DWORD PTR _hPaintBuffer$76920[ebp]
	push	ecx
	call	?ConvertBufferToPARGB32@@YAHPAXPAUHDC__@@PAUHICON__@@AAUtagSIZE@@@Z ; ConvertBufferToPARGB32
	add	esp, 16					; 00000010H
	mov	DWORD PTR _bSuccess$76908[ebp], eax
$L76926:

; 1089 : 						
; 1090 : 						// This will write the buffer contents to the destination bitmap.
; 1091 : 						CThemed::EndBufferedPaint(hPaintBuffer, TRUE);

	push	1
	mov	edx, DWORD PTR _hPaintBuffer$76920[ebp]
	push	edx
	call	?EndBufferedPaint@CThemed@@SAHPAXH@Z	; CThemed::EndBufferedPaint
	add	esp, 8
$L76921:

; 1093 : 					
; 1094 : 					SelectObject(hdcDest, hbmpOld);

	mov	esi, esp
	mov	eax, DWORD PTR _hbmpOld$76913[ebp]
	push	eax
	mov	ecx, DWORD PTR _hdcDest$76907[ebp]
	push	ecx
	call	DWORD PTR __imp__SelectObject@8
	cmp	esi, esp
	call	__chkesp
$L76915:

; 1097 : 			
; 1098 : 			VERIFY(DeleteDC(hdcDest));

	mov	esi, esp
	mov	edx, DWORD PTR _hdcDest$76907[ebp]
	push	edx
	call	DWORD PTR __imp__DeleteDC@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76930
	push	1098					; 0000044aH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76930
	int	3
$L76930:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76915
$L76929:

; 1100 : 		
; 1101 : 		if (!bSuccess && hBmp)

	cmp	DWORD PTR _bSuccess$76908[ebp], 0
	jne	SHORT $L76931
	cmp	DWORD PTR _hBmp$[ebp], 0
	je	SHORT $L76931
$L76932:

; 1103 : 			VERIFY(DeleteObject(hBmp));

	mov	esi, esp
	mov	ecx, DWORD PTR _hBmp$[ebp]
	push	ecx
	call	DWORD PTR __imp__DeleteObject@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76935
	push	1103					; 0000044fH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76935
	int	3
$L76935:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76932

; 1104 : 			hBmp = NULL;

	mov	DWORD PTR _hBmp$[ebp], 0
$L76931:

; 1107 : 	
; 1108 :     return hBmp;

	mov	eax, DWORD PTR _hBmp$[ebp]

; 1109 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 68					; 00000044H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IconToPARGB32Bitmap@GraphicsMisc@@YAPAUHBITMAP__@@PAUHICON__@@@Z ENDP ; GraphicsMisc::IconToPARGB32Bitmap
_TEXT	ENDS
END

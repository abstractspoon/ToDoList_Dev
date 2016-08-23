	TITLE	D:\_CODE\Shared\EnBitmap.cpp
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
;	COMDAT ??_C@_0BP@IHHM@D?3?2_CODE?2Shared?2graphicsMisc?4h?$AA@
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
;	COMDAT ??_C@_05KIFI@with?5?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_09LOPF@?5elements?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_01BJG@?6?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_11A@?$AA?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
CRT$XCA	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCA	ENDS
CRT$XCU	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCU	ENDS
CRT$XCL	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCL	ENDS
CRT$XCC	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCC	ENDS
CRT$XCZ	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCZ	ENDS
text$AFX_COL1	SEGMENT PARA USE32 PUBLIC ''
text$AFX_COL1	ENDS
;	COMDAT ??0RGBX@@QAE@XZ
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
;	COMDAT ??2@YAPAXIPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??3@YAXPAX0@Z
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
;	COMDAT ?VerifyDeleteObject@GraphicsMisc@@YAHPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?VerifyDeleteObject@GraphicsMisc@@YAHAAVCGdiObject@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E267
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E268
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CLIPBOARDDATA@@QAE@ABVCString@@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AtlA2WHelper@@YGPAGPAGPBDHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AtlW2AHelper@@YGPADPADPBGHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?T2OLE@@YAPAGPAG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?A2WBSTR@@YAPAGPBDH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0C32BitImageProcessor@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GC32BitImageProcessor@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1C32BitImageProcessor@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CalcWeightedColor@C32BitImageProcessor@@QAEXPAVRGBX@@VCSize@@NNAAV2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CEnBitmap@@QAE@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCEnBitmap@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CEnBitmap@@QAE@HHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CEnBitmap@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadImageW@CEnBitmap@@QAEHIPBGPAUHINSTANCE__@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadImageW@CEnBitmap@@QAEHPBGK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadImageFile@CEnBitmap@@SAPAUHBITMAP__@@PBGKHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadImageResource@CEnBitmap@@SAPAUHBITMAP__@@IPBGPAUHINSTANCE__@@KHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?LoadFromBuffer@CEnBitmap@@SAPAUIPicture@@PAEH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetResource@CEnBitmap@@SAHPBG0PAUHINSTANCE__@@PAXAAH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAUIPicture@@KHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAUHICON__@@KHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAU2@KHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@ABVCImageList@@KHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CopyToClipboard@CEnBitmap@@QBEHPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CopyImageFileToClipboard@CEnBitmap@@SAHPAUHWND__@@PBGK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileType@CEnBitmap@@SA?AW4EB_IMAGETYPE@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessImage@CEnBitmap@@QAEHAAV?$CArray@PAVC32BitImageProcessor@@PAV1@@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_H@YGXPAXIHP6EX0@Z@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetDIBits32@CEnBitmap@@IAEPAVRGBX@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PrepareBitmapInfo32@CEnBitmap@@IAEHAAUtagBITMAPINFO@@PAUHBITMAP__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CopyImage@CEnBitmap@@QAEHPAUHBITMAP__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CopyImage@CEnBitmap@@QAEHPAVCBitmap@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Fill@CEnBitmap@@KAHPAVRGBX@@VCSize@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Copy@CEnBitmap@@QAEHPAU_IMAGELIST@@@Z
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
;	COMDAT ??1?$CArray@UCLIPBOARDDATA@@AAU1@@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CArray@UCLIPBOARDDATA@@AAU1@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CArray@UCLIPBOARDDATA@@AAU1@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CArray@UCLIPBOARDDATA@@AAU1@@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Add@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEHPAVC32BitImageProcessor@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEAAPAVC32BitImageProcessor@@H@Z
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
;	COMDAT ??_G?$CArray@UCLIPBOARDDATA@@AAU1@@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetSize@?$CArray@UCLIPBOARDDATA@@AAU1@@@QAEXHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAUCLIPBOARDDATA@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAUCLIPBOARDDATA@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBUCLIPBOARDDATA@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ElementAt@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEAAPAVC32BitImageProcessor@@H@Z
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
;	COMDAT ??_GCLIPBOARDDATA@@QAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CLIPBOARDDATA@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAUCLIPBOARDDATA@@H@Z
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
;	COMDAT ??_7?$CArray@UCLIPBOARDDATA@@AAU1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT __imp__FreeResource@4
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?UnlockResource@@YGHPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7CEnBitmap@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7C32BitImageProcessor@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS, CRT$XCA, CRT$XCU, CRT$XCL, CRT$XCC, CRT$XCZ
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
_DATA	SEGMENT
__szAfxTempl DB	'afxtempl.h', 00H
_DATA	ENDS
CRT$XCU	SEGMENT
_$S269	DD	FLAT:_$E268
CRT$XCU	ENDS
_DATA	SEGMENT
	ORG $+1
_THIS_FILE DB	'D:\_CODE\Shared\EnBitmap.cpp', 00H
	ORG $+3
$SG74342 DB	'H', 00H, 'T', 00H, 'M', 00H, 'L', 00H, ' ', 00H, 'F', 00H
	DB	'o', 00H, 'r', 00H, 'm', 00H, 'a', 00H, 't', 00H, 00H, 00H
_DATA	ENDS
EXTRN	__chkesp:NEAR
;	COMDAT _$E268
_TEXT	SEGMENT
_$E268	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E267
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E268	ENDP
_TEXT	ENDS
EXTRN	__imp__RegisterClipboardFormatW@4:NEAR
_BSS	SEGMENT
_CF_HTML DD	01H DUP (?)
_BSS	ENDS
;	COMDAT _$E267
_TEXT	SEGMENT
_$E267	PROC NEAR					; COMDAT

; 42   : static UINT CF_HTML = ::RegisterClipboardFormat(_T("HTML Format"));

	push	ebp
	mov	ebp, esp
	push	esi
	mov	esi, esp
	push	OFFSET FLAT:$SG74342
	call	DWORD PTR __imp__RegisterClipboardFormatW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _CF_HTML, eax
	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E267	ENDP
_TEXT	ENDS
PUBLIC	??0C32BitImageProcessor@@QAE@H@Z		; C32BitImageProcessor::C32BitImageProcessor
PUBLIC	?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z ; C32BitImageProcessor::CalcDestSize
PUBLIC	?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z ; C32BitImageProcessor::ProcessPixels
PUBLIC	??_7C32BitImageProcessor@@6B@			; C32BitImageProcessor::`vftable'
PUBLIC	??_GC32BitImageProcessor@@UAEPAXI@Z		; C32BitImageProcessor::`scalar deleting destructor'
PUBLIC	??_EC32BitImageProcessor@@UAEPAXI@Z		; C32BitImageProcessor::`vector deleting destructor'
;	COMDAT ??_7C32BitImageProcessor@@6B@
; File D:\_CODE\Shared\EnBitmap.cpp
CONST	SEGMENT
??_7C32BitImageProcessor@@6B@ DD FLAT:??_EC32BitImageProcessor@@UAEPAXI@Z ; C32BitImageProcessor::`vftable'
	DD	FLAT:?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z
	DD	FLAT:?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z
CONST	ENDS
;	COMDAT ??0C32BitImageProcessor@@QAE@H@Z
_TEXT	SEGMENT
_bEnableWeighting$ = 8
_this$ = -4
??0C32BitImageProcessor@@QAE@H@Z PROC NEAR		; C32BitImageProcessor::C32BitImageProcessor, COMDAT

; 31   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _bEnableWeighting$[ebp]
	mov	DWORD PTR [eax+4], ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7C32BitImageProcessor@@6B@ ; C32BitImageProcessor::`vftable'

; 32   : }

	mov	eax, DWORD PTR _this$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	4
??0C32BitImageProcessor@@QAE@H@Z ENDP			; C32BitImageProcessor::C32BitImageProcessor
_TEXT	ENDS
PUBLIC	??1C32BitImageProcessor@@UAE@XZ			; C32BitImageProcessor::~C32BitImageProcessor
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GC32BitImageProcessor@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GC32BitImageProcessor@@UAEPAXI@Z PROC NEAR		; C32BitImageProcessor::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1C32BitImageProcessor@@UAE@XZ		; C32BitImageProcessor::~C32BitImageProcessor
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75708
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75708:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GC32BitImageProcessor@@UAEPAXI@Z ENDP		; C32BitImageProcessor::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??1C32BitImageProcessor@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1C32BitImageProcessor@@UAE@XZ PROC NEAR		; C32BitImageProcessor::~C32BitImageProcessor, COMDAT

; 35   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7C32BitImageProcessor@@6B@ ; C32BitImageProcessor::`vftable'

; 36   : }

	mov	esp, ebp
	pop	ebp
	ret	0
??1C32BitImageProcessor@@UAE@XZ ENDP			; C32BitImageProcessor::~C32BitImageProcessor
_TEXT	ENDS
;	COMDAT ?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z
_TEXT	SEGMENT
_sizeSrc$ = 12
___$ReturnUdt$ = 8
_this$ = -4
?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z PROC NEAR ; C32BitImageProcessor::CalcDestSize, COMDAT

; 39   : { 

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 40   : 	return sizeSrc; // default

	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	mov	ecx, DWORD PTR _sizeSrc$[ebp]
	mov	DWORD PTR [eax], ecx
	mov	edx, DWORD PTR _sizeSrc$[ebp+4]
	mov	DWORD PTR [eax+4], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 41   : }

	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?CalcDestSize@C32BitImageProcessor@@UAE?AVCSize@@V2@@Z ENDP ; C32BitImageProcessor::CalcDestSize
_TEXT	ENDS
EXTRN	_memcpy:NEAR
;	COMDAT ?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z
_TEXT	SEGMENT
_pSrcPixels$ = 8
_pDestPixels$ = 20
_sizeDest$ = 24
_this$ = -4
?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z PROC NEAR ; C32BitImageProcessor::ProcessPixels, COMDAT

; 45   : { 

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 46   : 	CopyMemory(pDestPixels, pSrcPixels, sizeDest.cx * 4 * sizeDest.cy); // default

	mov	eax, DWORD PTR _sizeDest$[ebp]
	shl	eax, 2
	imul	eax, DWORD PTR _sizeDest$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _pSrcPixels$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	push	edx
	call	_memcpy
	add	esp, 12					; 0000000cH

; 47   : 	return TRUE;

	mov	eax, 1

; 48   : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	28					; 0000001cH
?ProcessPixels@C32BitImageProcessor@@UAEHPAVRGBX@@VCSize@@01K@Z ENDP ; C32BitImageProcessor::ProcessPixels
_TEXT	ENDS
PUBLIC	?CalcWeightedColor@C32BitImageProcessor@@QAEXPAVRGBX@@VCSize@@NNAAV2@@Z ; C32BitImageProcessor::CalcWeightedColor
PUBLIC	__real@8@00000000000000000000
PUBLIC	__real@8@3fff8000000000000000
PUBLIC	??4RGBX@@QAEABV0@ABV0@@Z			; RGBX::operator=
EXTRN	__ftol:NEAR
EXTRN	__fltused:NEAR
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
;	COMDAT __real@8@00000000000000000000
; File D:\_CODE\Shared\EnBitmap.cpp
CONST	SEGMENT
__real@8@00000000000000000000 DQ 00000000000000000r ; 0
CONST	ENDS
;	COMDAT __real@8@3fff8000000000000000
CONST	SEGMENT
__real@8@3fff8000000000000000 DQ 03ff0000000000000r ; 1
CONST	ENDS
;	COMDAT ?CalcWeightedColor@C32BitImageProcessor@@QAEXPAVRGBX@@VCSize@@NNAAV2@@Z
_TEXT	SEGMENT
_pPixels$ = 8
_size$ = 12
_dX$ = 20
_dY$ = 28
_rgbResult$ = 36
_this$ = -80
_nX$ = -36
_nY$ = -48
_dXFraction$ = -44
_dX1MinusFraction$ = -16
_dYFraction$ = -60
_dY1MinusFraction$ = -24
_nXP1$ = -8
_nYP1$ = -4
_pRGB$ = -72
_pRGBXP$ = -32
_pRGBYP$ = -68
_pRGBXYP$ = -28
_nRed$ = -52
_nGreen$ = -76
_nBlue$ = -64
?CalcWeightedColor@C32BitImageProcessor@@QAEXPAVRGBX@@VCSize@@NNAAV2@@Z PROC NEAR ; C32BitImageProcessor::CalcWeightedColor, COMDAT

; 51   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 172				; 000000acH
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-172]
	mov	ecx, 43					; 0000002bH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L75733:

; 52   : 	ASSERT (m_bWeightingEnabled);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L75736
	push	52					; 00000034H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75736
	int	3
$L75736:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75733

; 53   : 
; 54   : 	// interpolate between the current pixel and its adjacent pixels to the right and down
; 55   : 	int nX = (int)dX;

	fld	QWORD PTR _dX$[ebp]
	call	__ftol
	mov	DWORD PTR _nX$[ebp], eax

; 56   : 	int nY = (int)dY;

	fld	QWORD PTR _dY$[ebp]
	call	__ftol
	mov	DWORD PTR _nY$[ebp], eax

; 57   : 
; 58   : 	if (dX < 0 || dY < 0)

	fld	QWORD PTR _dX$[ebp]
	fcomp	QWORD PTR __real@8@00000000000000000000
	fnstsw	ax
	test	ah, 1
	jne	SHORT $L75742
	fld	QWORD PTR _dY$[ebp]
	fcomp	QWORD PTR __real@8@00000000000000000000
	fnstsw	ax
	test	ah, 1
	je	SHORT $L75741
$L75742:

; 60   : 		rgbResult = pPixels[max(0, nY) * size.cx + max(0, nX)]; // closest

	xor	edx, edx
	cmp	DWORD PTR _nY$[ebp], 0
	setl	dl
	dec	edx
	and	edx, DWORD PTR _nY$[ebp]
	imul	edx, DWORD PTR _size$[ebp]
	xor	eax, eax
	cmp	DWORD PTR _nX$[ebp], 0
	setl	al
	dec	eax
	and	eax, DWORD PTR _nX$[ebp]
	add	edx, eax
	mov	ecx, DWORD PTR _pPixels$[ebp]
	lea	edx, DWORD PTR [ecx+edx*4]
	push	edx
	mov	ecx, DWORD PTR _rgbResult$[ebp]
	call	??4RGBX@@QAEABV0@ABV0@@Z		; RGBX::operator=

; 61   : 		return;

	jmp	$L75732
$L75741:

; 63   : 
; 64   : 	double dXFraction = dX - nX;

	fild	DWORD PTR _nX$[ebp]
	fsubr	QWORD PTR _dX$[ebp]
	fstp	QWORD PTR _dXFraction$[ebp]

; 65   : 	double dX1MinusFraction = 1 - dXFraction;

	fld	QWORD PTR __real@8@3fff8000000000000000
	fsub	QWORD PTR _dXFraction$[ebp]
	fstp	QWORD PTR _dX1MinusFraction$[ebp]

; 66   : 	
; 67   : 	double dYFraction = dY - nY;

	fild	DWORD PTR _nY$[ebp]
	fsubr	QWORD PTR _dY$[ebp]
	fstp	QWORD PTR _dYFraction$[ebp]

; 68   : 	double dY1MinusFraction = 1 - dYFraction;

	fld	QWORD PTR __real@8@3fff8000000000000000
	fsub	QWORD PTR _dYFraction$[ebp]
	fstp	QWORD PTR _dY1MinusFraction$[ebp]

; 69   : 
; 70   : 	int nXP1 = min(nX + 1, size.cx - 1);

	mov	eax, DWORD PTR _nX$[ebp]
	add	eax, 1
	mov	ecx, DWORD PTR _size$[ebp]
	sub	ecx, 1
	cmp	eax, ecx
	jge	SHORT $L77040
	mov	edx, DWORD PTR _nX$[ebp]
	add	edx, 1
	mov	DWORD PTR -84+[ebp], edx
	jmp	SHORT $L77041
$L77040:
	mov	eax, DWORD PTR _size$[ebp]
	sub	eax, 1
	mov	DWORD PTR -84+[ebp], eax
$L77041:
	mov	ecx, DWORD PTR -84+[ebp]
	mov	DWORD PTR _nXP1$[ebp], ecx

; 71   : 	int nYP1 = min(nY + 1, size.cy - 1);

	mov	edx, DWORD PTR _nY$[ebp]
	add	edx, 1
	mov	eax, DWORD PTR _size$[ebp+4]
	sub	eax, 1
	cmp	edx, eax
	jge	SHORT $L77042
	mov	ecx, DWORD PTR _nY$[ebp]
	add	ecx, 1
	mov	DWORD PTR -88+[ebp], ecx
	jmp	SHORT $L77043
$L77042:
	mov	edx, DWORD PTR _size$[ebp+4]
	sub	edx, 1
	mov	DWORD PTR -88+[ebp], edx
$L77043:
	mov	eax, DWORD PTR -88+[ebp]
	mov	DWORD PTR _nYP1$[ebp], eax

; 72   : 	
; 73   : 	RGBX* pRGB = &pPixels[nY * size.cx + nX]; // x, y

	mov	ecx, DWORD PTR _nY$[ebp]
	imul	ecx, DWORD PTR _size$[ebp]
	add	ecx, DWORD PTR _nX$[ebp]
	mov	edx, DWORD PTR _pPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGB$[ebp], eax

; 74   : 	RGBX* pRGBXP = &pPixels[nY * size.cx + nXP1]; // x + 1, y

	mov	ecx, DWORD PTR _nY$[ebp]
	imul	ecx, DWORD PTR _size$[ebp]
	add	ecx, DWORD PTR _nXP1$[ebp]
	mov	edx, DWORD PTR _pPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGBXP$[ebp], eax

; 75   : 	RGBX* pRGBYP = &pPixels[nYP1 * size.cx + nX]; // x, y + 1

	mov	ecx, DWORD PTR _nYP1$[ebp]
	imul	ecx, DWORD PTR _size$[ebp]
	add	ecx, DWORD PTR _nX$[ebp]
	mov	edx, DWORD PTR _pPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGBYP$[ebp], eax

; 76   : 	RGBX* pRGBXYP = &pPixels[nYP1 * size.cx + nXP1]; // x + 1, y + 1

	mov	ecx, DWORD PTR _nYP1$[ebp]
	imul	ecx, DWORD PTR _size$[ebp]
	add	ecx, DWORD PTR _nXP1$[ebp]
	mov	edx, DWORD PTR _pPixels$[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	mov	DWORD PTR _pRGBXYP$[ebp], eax

; 77   : 	
; 78   : 	int nRed = (int)(dX1MinusFraction * dY1MinusFraction * pRGB->btRed +
; 79   : 					dXFraction * dY1MinusFraction * pRGBXP->btRed +
; 80   : 					dX1MinusFraction * dYFraction * pRGBYP->btRed +
; 81   : 					dXFraction * dYFraction * pRGBXYP->btRed);

	fld	QWORD PTR _dX1MinusFraction$[ebp]
	fmul	QWORD PTR _dY1MinusFraction$[ebp]
	mov	ecx, DWORD PTR _pRGB$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+2]
	mov	DWORD PTR -92+[ebp], edx
	fild	DWORD PTR -92+[ebp]
	fmulp	ST(1), ST(0)
	fld	QWORD PTR _dXFraction$[ebp]
	fmul	QWORD PTR _dY1MinusFraction$[ebp]
	mov	eax, DWORD PTR _pRGBXP$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+2]
	mov	DWORD PTR -96+[ebp], ecx
	fild	DWORD PTR -96+[ebp]
	fmulp	ST(1), ST(0)
	faddp	ST(1), ST(0)
	fld	QWORD PTR _dX1MinusFraction$[ebp]
	fmul	QWORD PTR _dYFraction$[ebp]
	mov	edx, DWORD PTR _pRGBYP$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+2]
	mov	DWORD PTR -100+[ebp], eax
	fild	DWORD PTR -100+[ebp]
	fmulp	ST(1), ST(0)
	faddp	ST(1), ST(0)
	fld	QWORD PTR _dXFraction$[ebp]
	fmul	QWORD PTR _dYFraction$[ebp]
	mov	ecx, DWORD PTR _pRGBXYP$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+2]
	mov	DWORD PTR -104+[ebp], edx
	fild	DWORD PTR -104+[ebp]
	fmulp	ST(1), ST(0)
	faddp	ST(1), ST(0)
	call	__ftol
	mov	DWORD PTR _nRed$[ebp], eax

; 82   : 	
; 83   : 	int nGreen = (int)(dX1MinusFraction * dY1MinusFraction * pRGB->btGreen +
; 84   : 					dXFraction * dY1MinusFraction * pRGBXP->btGreen +
; 85   : 					dX1MinusFraction * dYFraction * pRGBYP->btGreen +
; 86   : 					dXFraction * dYFraction * pRGBXYP->btGreen);

	fld	QWORD PTR _dX1MinusFraction$[ebp]
	fmul	QWORD PTR _dY1MinusFraction$[ebp]
	mov	eax, DWORD PTR _pRGB$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+1]
	mov	DWORD PTR -108+[ebp], ecx
	fild	DWORD PTR -108+[ebp]
	fmulp	ST(1), ST(0)
	fld	QWORD PTR _dXFraction$[ebp]
	fmul	QWORD PTR _dY1MinusFraction$[ebp]
	mov	edx, DWORD PTR _pRGBXP$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+1]
	mov	DWORD PTR -112+[ebp], eax
	fild	DWORD PTR -112+[ebp]
	fmulp	ST(1), ST(0)
	faddp	ST(1), ST(0)
	fld	QWORD PTR _dX1MinusFraction$[ebp]
	fmul	QWORD PTR _dYFraction$[ebp]
	mov	ecx, DWORD PTR _pRGBYP$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+1]
	mov	DWORD PTR -116+[ebp], edx
	fild	DWORD PTR -116+[ebp]
	fmulp	ST(1), ST(0)
	faddp	ST(1), ST(0)
	fld	QWORD PTR _dXFraction$[ebp]
	fmul	QWORD PTR _dYFraction$[ebp]
	mov	eax, DWORD PTR _pRGBXYP$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+1]
	mov	DWORD PTR -120+[ebp], ecx
	fild	DWORD PTR -120+[ebp]
	fmulp	ST(1), ST(0)
	faddp	ST(1), ST(0)
	call	__ftol
	mov	DWORD PTR _nGreen$[ebp], eax

; 87   : 	
; 88   : 	int nBlue = (int)(dX1MinusFraction * dY1MinusFraction * pRGB->btBlue +
; 89   : 					dXFraction * dY1MinusFraction * pRGBXP->btBlue +
; 90   : 					dX1MinusFraction * dYFraction * pRGBYP->btBlue +
; 91   : 					dXFraction * dYFraction * pRGBXYP->btBlue);

	fld	QWORD PTR _dX1MinusFraction$[ebp]
	fmul	QWORD PTR _dY1MinusFraction$[ebp]
	mov	edx, DWORD PTR _pRGB$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx]
	mov	DWORD PTR -124+[ebp], eax
	fild	DWORD PTR -124+[ebp]
	fmulp	ST(1), ST(0)
	fld	QWORD PTR _dXFraction$[ebp]
	fmul	QWORD PTR _dY1MinusFraction$[ebp]
	mov	ecx, DWORD PTR _pRGBXP$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx]
	mov	DWORD PTR -128+[ebp], edx
	fild	DWORD PTR -128+[ebp]
	fmulp	ST(1), ST(0)
	faddp	ST(1), ST(0)
	fld	QWORD PTR _dX1MinusFraction$[ebp]
	fmul	QWORD PTR _dYFraction$[ebp]
	mov	eax, DWORD PTR _pRGBYP$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax]
	mov	DWORD PTR -132+[ebp], ecx
	fild	DWORD PTR -132+[ebp]
	fmulp	ST(1), ST(0)
	faddp	ST(1), ST(0)
	fld	QWORD PTR _dXFraction$[ebp]
	fmul	QWORD PTR _dYFraction$[ebp]
	mov	edx, DWORD PTR _pRGBXYP$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx]
	mov	DWORD PTR -136+[ebp], eax
	fild	DWORD PTR -136+[ebp]
	fmulp	ST(1), ST(0)
	faddp	ST(1), ST(0)
	call	__ftol
	mov	DWORD PTR _nBlue$[ebp], eax

; 92   : 
; 93   : 	rgbResult.btRed = (BYTE)max(0, min(255, nRed));

	cmp	DWORD PTR _nRed$[ebp], 255		; 000000ffH
	jle	SHORT $L77044
	mov	DWORD PTR -140+[ebp], 255		; 000000ffH
	jmp	SHORT $L77045
$L77044:
	mov	ecx, DWORD PTR _nRed$[ebp]
	mov	DWORD PTR -140+[ebp], ecx
$L77045:
	cmp	DWORD PTR -140+[ebp], 0
	jge	SHORT $L77048
	mov	DWORD PTR -144+[ebp], 0
	jmp	SHORT $L77049
$L77048:
	cmp	DWORD PTR _nRed$[ebp], 255		; 000000ffH
	jle	SHORT $L77046
	mov	DWORD PTR -148+[ebp], 255		; 000000ffH
	jmp	SHORT $L77047
$L77046:
	mov	edx, DWORD PTR _nRed$[ebp]
	mov	DWORD PTR -148+[ebp], edx
$L77047:
	mov	eax, DWORD PTR -148+[ebp]
	mov	DWORD PTR -144+[ebp], eax
$L77049:
	mov	ecx, DWORD PTR _rgbResult$[ebp]
	mov	dl, BYTE PTR -144+[ebp]
	mov	BYTE PTR [ecx+2], dl

; 94   : 	rgbResult.btGreen = (BYTE)max(0, min(255, nGreen));

	cmp	DWORD PTR _nGreen$[ebp], 255		; 000000ffH
	jle	SHORT $L77050
	mov	DWORD PTR -152+[ebp], 255		; 000000ffH
	jmp	SHORT $L77051
$L77050:
	mov	eax, DWORD PTR _nGreen$[ebp]
	mov	DWORD PTR -152+[ebp], eax
$L77051:
	cmp	DWORD PTR -152+[ebp], 0
	jge	SHORT $L77054
	mov	DWORD PTR -156+[ebp], 0
	jmp	SHORT $L77055
$L77054:
	cmp	DWORD PTR _nGreen$[ebp], 255		; 000000ffH
	jle	SHORT $L77052
	mov	DWORD PTR -160+[ebp], 255		; 000000ffH
	jmp	SHORT $L77053
$L77052:
	mov	ecx, DWORD PTR _nGreen$[ebp]
	mov	DWORD PTR -160+[ebp], ecx
$L77053:
	mov	edx, DWORD PTR -160+[ebp]
	mov	DWORD PTR -156+[ebp], edx
$L77055:
	mov	eax, DWORD PTR _rgbResult$[ebp]
	mov	cl, BYTE PTR -156+[ebp]
	mov	BYTE PTR [eax+1], cl

; 95   : 	rgbResult.btBlue =(BYTE) max(0, min(255, nBlue));

	cmp	DWORD PTR _nBlue$[ebp], 255		; 000000ffH
	jle	SHORT $L77056
	mov	DWORD PTR -164+[ebp], 255		; 000000ffH
	jmp	SHORT $L77057
$L77056:
	mov	edx, DWORD PTR _nBlue$[ebp]
	mov	DWORD PTR -164+[ebp], edx
$L77057:
	cmp	DWORD PTR -164+[ebp], 0
	jge	SHORT $L77060
	mov	DWORD PTR -168+[ebp], 0
	jmp	SHORT $L77061
$L77060:
	cmp	DWORD PTR _nBlue$[ebp], 255		; 000000ffH
	jle	SHORT $L77058
	mov	DWORD PTR -172+[ebp], 255		; 000000ffH
	jmp	SHORT $L77059
$L77058:
	mov	eax, DWORD PTR _nBlue$[ebp]
	mov	DWORD PTR -172+[ebp], eax
$L77059:
	mov	ecx, DWORD PTR -172+[ebp]
	mov	DWORD PTR -168+[ebp], ecx
$L77061:
	mov	edx, DWORD PTR _rgbResult$[ebp]
	mov	al, BYTE PTR -168+[ebp]
	mov	BYTE PTR [edx], al
$L75732:

; 96   : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 172				; 000000acH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	32					; 00000020H
?CalcWeightedColor@C32BitImageProcessor@@QAEXPAVRGBX@@VCSize@@NNAAV2@@Z ENDP ; C32BitImageProcessor::CalcWeightedColor
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
PUBLIC	??0CEnBitmap@@QAE@K@Z				; CEnBitmap::CEnBitmap
PUBLIC	??_7CEnBitmap@@6B@				; CEnBitmap::`vftable'
PUBLIC	??_GCEnBitmap@@UAEPAXI@Z			; CEnBitmap::`scalar deleting destructor'
PUBLIC	??_ECEnBitmap@@UAEPAXI@Z			; CEnBitmap::`vector deleting destructor'
EXTRN	?AssertValid@CGdiObject@@UBEXXZ:NEAR		; CGdiObject::AssertValid
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	?GetRuntimeClass@CBitmap@@UBEPAUCRuntimeClass@@XZ:NEAR ; CBitmap::GetRuntimeClass
EXTRN	??0CBitmap@@QAE@XZ:NEAR				; CBitmap::CBitmap
EXTRN	??1CBitmap@@UAE@XZ:NEAR				; CBitmap::~CBitmap
EXTRN	?Dump@CBitmap@@UBEXAAVCDumpContext@@@Z:NEAR	; CBitmap::Dump
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CSize@@QAE@HH@Z:NEAR				; CSize::CSize
;	COMDAT ??_7CEnBitmap@@6B@
; File D:\_CODE\Shared\EnBitmap.cpp
CONST	SEGMENT
??_7CEnBitmap@@6B@ DD FLAT:?GetRuntimeClass@CBitmap@@UBEPAUCRuntimeClass@@XZ ; CEnBitmap::`vftable'
	DD	FLAT:??_ECEnBitmap@@UAEPAXI@Z
	DD	FLAT:?Serialize@CObject@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@CGdiObject@@UBEXXZ
	DD	FLAT:?Dump@CBitmap@@UBEXAAVCDumpContext@@@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77076	DD	019930520H
	DD	01H
	DD	FLAT:$T77079
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77079	DD	0ffffffffH
	DD	FLAT:$L77072
xdata$x	ENDS
;	COMDAT ??0CEnBitmap@@QAE@K@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_crBkgnd$ = 8
_this$ = -16
??0CEnBitmap@@QAE@K@Z PROC NEAR				; CEnBitmap::CEnBitmap, COMDAT

; 104  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77077
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CBitmap@@QAE@XZ			; CBitmap::CBitmap
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _crBkgnd$[ebp]
	mov	DWORD PTR [eax+8], ecx
	push	0
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	call	??0CSize@@QAE@HH@Z			; CSize::CSize
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7CEnBitmap@@6B@ ; CEnBitmap::`vftable'

; 105  : 
; 106  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77072:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CBitmap@@UAE@XZ			; CBitmap::~CBitmap
	ret	0
$L77077:
	mov	eax, OFFSET FLAT:$T77076
	jmp	___CxxFrameHandler
text$x	ENDS
??0CEnBitmap@@QAE@K@Z ENDP				; CEnBitmap::CEnBitmap
PUBLIC	??1CEnBitmap@@UAE@XZ				; CEnBitmap::~CEnBitmap
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_GCEnBitmap@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCEnBitmap@@UAEPAXI@Z PROC NEAR			; CEnBitmap::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CEnBitmap@@UAE@XZ			; CEnBitmap::~CEnBitmap
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75769
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L75769:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCEnBitmap@@UAEPAXI@Z ENDP				; CEnBitmap::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??0CEnBitmap@@QAE@HHK@Z				; CEnBitmap::CEnBitmap
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmap.cpp
xdata$x	SEGMENT
$T77089	DD	019930520H
	DD	01H
	DD	FLAT:$T77091
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77091	DD	0ffffffffH
	DD	FLAT:$L77087
xdata$x	ENDS
;	COMDAT ??0CEnBitmap@@QAE@HHK@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_cx$ = 8
_cy$ = 12
_crBkgnd$ = 16
_this$ = -16
??0CEnBitmap@@QAE@HHK@Z PROC NEAR			; CEnBitmap::CEnBitmap, COMDAT

; 109  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77090
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CBitmap@@QAE@XZ			; CBitmap::CBitmap
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _crBkgnd$[ebp]
	mov	DWORD PTR [eax+8], ecx
	mov	edx, DWORD PTR _cy$[ebp]
	push	edx
	mov	eax, DWORD PTR _cx$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	call	??0CSize@@QAE@HH@Z			; CSize::CSize
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CEnBitmap@@6B@ ; CEnBitmap::`vftable'

; 110  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77087:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CBitmap@@UAE@XZ			; CBitmap::~CBitmap
	ret	0
$L77090:
	mov	eax, OFFSET FLAT:$T77089
	jmp	___CxxFrameHandler
text$x	ENDS
??0CEnBitmap@@QAE@HHK@Z ENDP				; CEnBitmap::CEnBitmap
;	COMDAT ??1CEnBitmap@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CEnBitmap@@UAE@XZ PROC NEAR				; CEnBitmap::~CEnBitmap, COMDAT

; 113  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CEnBitmap@@6B@ ; CEnBitmap::`vftable'

; 114  : 
; 115  : }

	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CBitmap@@UAE@XZ			; CBitmap::~CBitmap
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CEnBitmap@@UAE@XZ ENDP				; CEnBitmap::~CEnBitmap
_TEXT	ENDS
PUBLIC	?LoadImageW@CEnBitmap@@QAEHIPBGPAUHINSTANCE__@@K@Z ; CEnBitmap::LoadImageW
PUBLIC	?LoadImageResource@CEnBitmap@@SAPAUHBITMAP__@@IPBGPAUHINSTANCE__@@KHH@Z ; CEnBitmap::LoadImageResource
EXTRN	?Attach@CGdiObject@@QAEHPAX@Z:NEAR		; CGdiObject::Attach
;	COMDAT ?LoadImageW@CEnBitmap@@QAEHIPBGPAUHINSTANCE__@@K@Z
_TEXT	SEGMENT
_uIDRes$ = 8
_szResourceType$ = 12
_hInst$ = 16
_crBack$ = 20
_this$ = -8
_hbm$ = -4
?LoadImageW@CEnBitmap@@QAEHIPBGPAUHINSTANCE__@@K@Z PROC NEAR ; CEnBitmap::LoadImageW, COMDAT

; 118  : {

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
$L75786:

; 119  : 	ASSERT(m_hObject == NULL);      // only attach once, detach on destroy

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L75789
	push	119					; 00000077H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75789
	int	3
$L75789:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75786

; 120  : 
; 121  : 	if (m_hObject != NULL)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	je	SHORT $L75790

; 122  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75785
$L75790:

; 125  : 									(crBack == -1 ? m_crBkgnd : crBack), 
; 126  : 									m_sizeDest.cx, m_sizeDest.cy);

	cmp	DWORD PTR _crBack$[ebp], -1
	jne	SHORT $L77099
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR -12+[ebp], ecx
	jmp	SHORT $L77100
$L77099:
	mov	edx, DWORD PTR _crBack$[ebp]
	mov	DWORD PTR -12+[ebp], edx
$L77100:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR -12+[ebp]
	push	ecx
	mov	edx, DWORD PTR _hInst$[ebp]
	push	edx
	mov	eax, DWORD PTR _szResourceType$[ebp]
	push	eax
	mov	ecx, DWORD PTR _uIDRes$[ebp]
	push	ecx
	call	?LoadImageResource@CEnBitmap@@SAPAUHBITMAP__@@IPBGPAUHINSTANCE__@@KHH@Z ; CEnBitmap::LoadImageResource
	add	esp, 24					; 00000018H
	mov	DWORD PTR _hbm$[ebp], eax

; 127  : 
; 128  : 	return Attach(hbm);

	mov	edx, DWORD PTR _hbm$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Attach@CGdiObject@@QAEHPAX@Z		; CGdiObject::Attach
$L75785:

; 129  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?LoadImageW@CEnBitmap@@QAEHIPBGPAUHINSTANCE__@@K@Z ENDP	; CEnBitmap::LoadImageW
_TEXT	ENDS
PUBLIC	?LoadImageW@CEnBitmap@@QAEHPBGK@Z		; CEnBitmap::LoadImageW
PUBLIC	?LoadImageFile@CEnBitmap@@SAPAUHBITMAP__@@PBGKHH@Z ; CEnBitmap::LoadImageFile
;	COMDAT ?LoadImageW@CEnBitmap@@QAEHPBGK@Z
_TEXT	SEGMENT
_szImagePath$ = 8
_crBack$ = 12
_this$ = -8
_hbm$ = -4
?LoadImageW@CEnBitmap@@QAEHPBGK@Z PROC NEAR		; CEnBitmap::LoadImageW, COMDAT

; 132  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L75797:

; 133  : 	ASSERT(m_hObject == NULL);      // only attach once, detach on destroy

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L75800
	push	133					; 00000085H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75800
	int	3
$L75800:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75797

; 134  : 
; 135  : 	if (m_hObject != NULL)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	je	SHORT $L75801

; 136  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75796
$L75801:

; 137  : 
; 138  : 	if (crBack == -1)

	cmp	DWORD PTR _crBack$[ebp], -1
	jne	SHORT $L75802

; 139  : 		crBack = m_crBkgnd;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR _crBack$[ebp], ecx
$L75802:

; 140  : 
; 141  : 	HBITMAP hbm = LoadImageFile(szImagePath, crBack, m_sizeDest.cx, m_sizeDest.cy);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+16]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	push	edx
	mov	eax, DWORD PTR _crBack$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szImagePath$[ebp]
	push	ecx
	call	?LoadImageFile@CEnBitmap@@SAPAUHBITMAP__@@PBGKHH@Z ; CEnBitmap::LoadImageFile
	add	esp, 16					; 00000010H
	mov	DWORD PTR _hbm$[ebp], eax

; 142  : 
; 143  : 	return Attach(hbm);

	mov	edx, DWORD PTR _hbm$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Attach@CGdiObject@@QAEHPAX@Z		; CGdiObject::Attach
$L75796:

; 144  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?LoadImageW@CEnBitmap@@QAEHPBGK@Z ENDP			; CEnBitmap::LoadImageW
_TEXT	ENDS
PUBLIC	?GetFileType@CEnBitmap@@SA?AW4EB_IMAGETYPE@@PBG@Z ; CEnBitmap::GetFileType
PUBLIC	?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAUIPicture@@KHH@Z ; CEnBitmap::ExtractBitmap
PUBLIC	?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAUHICON__@@KHH@Z ; CEnBitmap::ExtractBitmap
PUBLIC	?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAU2@KHH@Z ; CEnBitmap::ExtractBitmap
PUBLIC	?T2OLE@@YAPAGPAG@Z				; T2OLE
EXTRN	__imp__DestroyIcon@4:NEAR
EXTRN	__imp__LoadImageW@24:NEAR
EXTRN	__imp__OleLoadPicturePath@24:NEAR
EXTRN	?CreateBitmapFromFile@CGdiPlus@@SAHPBGPAPAPAX@Z:NEAR ; CGdiPlus::CreateBitmapFromFile
EXTRN	?CreateHBITMAPFromBitmap@CGdiPlus@@SAHPAPAXPAPAUHBITMAP__@@K@Z:NEAR ; CGdiPlus::CreateHBITMAPFromBitmap
EXTRN	__imp__DeleteObject@4:NEAR
EXTRN	_IID_IPicture:BYTE
;	COMDAT ?LoadImageFile@CEnBitmap@@SAPAUHBITMAP__@@PBGKHH@Z
_TEXT	SEGMENT
_szImagePath$ = 8
_crBack$ = 12
_cx$ = 16
_cy$ = 20
_nType$ = -4
_hbm$ = -8
_hIcon$75820 = -12
_gdBitmap$75824 = -16
_hbmGdiP$75825 = -20
__convert$75828 = -44
__acp$75829 = -28
__lpw$75830 = -24
__lpa$75831 = -40
_pPicture$75832 = -32
_hr$75833 = -36
?LoadImageFile@CEnBitmap@@SAPAUHBITMAP__@@PBGKHH@Z PROC NEAR ; CEnBitmap::LoadImageFile, COMDAT

; 147  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 48					; 00000030H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-48]
	mov	ecx, 12					; 0000000cH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 148  : 	int nType = GetFileType(szImagePath);

	mov	eax, DWORD PTR _szImagePath$[ebp]
	push	eax
	call	?GetFileType@CEnBitmap@@SA?AW4EB_IMAGETYPE@@PBG@Z ; CEnBitmap::GetFileType
	add	esp, 4
	mov	DWORD PTR _nType$[ebp], eax

; 149  : 	HBITMAP hbm = NULL;

	mov	DWORD PTR _hbm$[ebp], 0

; 152  : 	{

	mov	ecx, DWORD PTR _nType$[ebp]
	mov	DWORD PTR -48+[ebp], ecx
	cmp	DWORD PTR -48+[ebp], 5
	ja	$L75827
	mov	edx, DWORD PTR -48+[ebp]
	jmp	DWORD PTR $L77105[edx*4]
$L75816:

; 153  : 	case FT_BMP:
; 154  : 		// the reason for this is that i suspect it is more efficient to load
; 155  : 		// bmps this way since it avoids creating device contexts etc that the 
; 156  : 		// IPicture methods requires. that method however is still valuable
; 157  : 		// since it handles other image types and transparency
; 158  : 		hbm = (HBITMAP)::LoadImage(NULL, szImagePath, IMAGE_BITMAP, cx, cy, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS);

	mov	esi, esp
	push	4112					; 00001010H
	mov	eax, DWORD PTR _cy$[ebp]
	push	eax
	mov	ecx, DWORD PTR _cx$[ebp]
	push	ecx
	push	0
	mov	edx, DWORD PTR _szImagePath$[ebp]
	push	edx
	push	0
	call	DWORD PTR __imp__LoadImageW@24
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hbm$[ebp], eax

; 159  : 		break;

	jmp	$L75813
$L75818:

; 160  : 		
; 161  : 	case FT_UNKNOWN:
; 162  : 		break;

	jmp	$L75813
$L75819:

; 166  : 			HICON hIcon = (HICON)::LoadImage(NULL, szImagePath, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

	mov	esi, esp
	push	16					; 00000010H
	push	0
	push	0
	push	1
	mov	eax, DWORD PTR _szImagePath$[ebp]
	push	eax
	push	0
	call	DWORD PTR __imp__LoadImageW@24
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hIcon$75820[ebp], eax

; 167  : 			
; 168  : 			if (hIcon)

	cmp	DWORD PTR _hIcon$75820[ebp], 0
	je	SHORT $L75822

; 170  : 				hbm = ExtractBitmap(hIcon, crBack, cx, cy);

	mov	ecx, DWORD PTR _cy$[ebp]
	push	ecx
	mov	edx, DWORD PTR _cx$[ebp]
	push	edx
	mov	eax, DWORD PTR _crBack$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hIcon$75820[ebp]
	push	ecx
	call	?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAUHICON__@@KHH@Z ; CEnBitmap::ExtractBitmap
	add	esp, 16					; 00000010H
	mov	DWORD PTR _hbm$[ebp], eax

; 171  : 			
; 172  : 				// cleanup
; 173  : 				::DestroyIcon(hIcon);

	mov	esi, esp
	mov	edx, DWORD PTR _hIcon$75820[ebp]
	push	edx
	call	DWORD PTR __imp__DestroyIcon@4
	cmp	esi, esp
	call	__chkesp
$L75822:

; 176  : 		break;

	jmp	$L75813
$L75823:

; 181  : 			gdix_Bitmap* gdBitmap = NULL;

	mov	DWORD PTR _gdBitmap$75824[ebp], 0

; 182  : 			HBITMAP hbmGdiP = NULL;

	mov	DWORD PTR _hbmGdiP$75825[ebp], 0

; 185  : 				CGdiPlus::CreateHBITMAPFromBitmap(gdBitmap, &hbmGdiP, crBack))

	lea	eax, DWORD PTR _gdBitmap$75824[ebp]
	push	eax
	mov	ecx, DWORD PTR _szImagePath$[ebp]
	push	ecx
	call	?CreateBitmapFromFile@CGdiPlus@@SAHPBGPAPAPAX@Z ; CGdiPlus::CreateBitmapFromFile
	add	esp, 8
	test	eax, eax
	je	SHORT $L75826
	mov	edx, DWORD PTR _crBack$[ebp]
	push	edx
	lea	eax, DWORD PTR _hbmGdiP$75825[ebp]
	push	eax
	mov	ecx, DWORD PTR _gdBitmap$75824[ebp]
	push	ecx
	call	?CreateHBITMAPFromBitmap@CGdiPlus@@SAHPAPAXPAPAUHBITMAP__@@K@Z ; CGdiPlus::CreateHBITMAPFromBitmap
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	SHORT $L75826

; 187  : 				hbm = ExtractBitmap(hbmGdiP, crBack, cx, cy);

	mov	edx, DWORD PTR _cy$[ebp]
	push	edx
	mov	eax, DWORD PTR _cx$[ebp]
	push	eax
	mov	ecx, DWORD PTR _crBack$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hbmGdiP$75825[ebp]
	push	edx
	call	?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAU2@KHH@Z ; CEnBitmap::ExtractBitmap
	add	esp, 16					; 00000010H
	mov	DWORD PTR _hbm$[ebp], eax

; 188  : 
; 189  : 				// cleanup
; 190  : 				::DeleteObject(hbmGdiP);

	mov	esi, esp
	mov	eax, DWORD PTR _hbmGdiP$75825[ebp]
	push	eax
	call	DWORD PTR __imp__DeleteObject@4
	cmp	esi, esp
	call	__chkesp
$L75826:

; 193  : 		break;

	jmp	$L75813
$L75827:

; 198  : 			USES_CONVERSION;

	mov	DWORD PTR __convert$75828[ebp], 0
	mov	DWORD PTR __acp$75829[ebp], 0
	mov	DWORD PTR __lpw$75830[ebp], 0
	mov	DWORD PTR __lpa$75831[ebp], 0

; 199  : 			IPicture* pPicture = NULL;

	mov	DWORD PTR _pPicture$75832[ebp], 0

; 200  : 			
; 201  : 			HRESULT hr = OleLoadPicturePath(T2OLE((LPTSTR)szImagePath), NULL, 0, crBack, IID_IPicture, (LPVOID*)&pPicture);

	mov	esi, esp
	lea	ecx, DWORD PTR _pPicture$75832[ebp]
	push	ecx
	push	OFFSET FLAT:_IID_IPicture
	mov	edx, DWORD PTR _crBack$[ebp]
	push	edx
	push	0
	push	0
	mov	eax, DWORD PTR _szImagePath$[ebp]
	push	eax
	call	?T2OLE@@YAPAGPAG@Z			; T2OLE
	add	esp, 4
	push	eax
	call	DWORD PTR __imp__OleLoadPicturePath@24
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hr$75833[ebp], eax

; 202  : 			
; 203  : 			if (SUCCEEDED(hr) && pPicture)

	cmp	DWORD PTR _hr$75833[ebp], 0
	jl	SHORT $L75837
	cmp	DWORD PTR _pPicture$75832[ebp], 0
	je	SHORT $L75837

; 205  : 				hbm = ExtractBitmap(pPicture, crBack, cx, cy);

	mov	ecx, DWORD PTR _cy$[ebp]
	push	ecx
	mov	edx, DWORD PTR _cx$[ebp]
	push	edx
	mov	eax, DWORD PTR _crBack$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pPicture$75832[ebp]
	push	ecx
	call	?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAUIPicture@@KHH@Z ; CEnBitmap::ExtractBitmap
	add	esp, 16					; 00000010H
	mov	DWORD PTR _hbm$[ebp], eax

; 206  : 				pPicture->Release();

	mov	edx, DWORD PTR _pPicture$75832[ebp]
	mov	eax, DWORD PTR [edx]
	mov	esi, esp
	mov	ecx, DWORD PTR _pPicture$75832[ebp]
	push	ecx
	call	DWORD PTR [eax+8]
	cmp	esi, esp
	call	__chkesp
$L75837:
$L75813:

; 211  : 	
; 212  : 	return hbm;

	mov	eax, DWORD PTR _hbm$[ebp]

; 213  : }

	pop	edi
	pop	esi
	add	esp, 48					; 00000030H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
$L77105:
	DD	$L75816
	DD	$L75819
	DD	$L75827
	DD	$L75827
	DD	$L75823
	DD	$L75818
?LoadImageFile@CEnBitmap@@SAPAUHBITMAP__@@PBGKHH@Z ENDP	; CEnBitmap::LoadImageFile
_TEXT	ENDS
;	COMDAT ?T2OLE@@YAPAGPAG@Z
_TEXT	SEGMENT
_lp$ = 8
?T2OLE@@YAPAGPAG@Z PROC NEAR				; T2OLE, COMDAT

; 127  : 	inline LPOLESTR T2OLE(LPTSTR lp) { return lp; }

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR _lp$[ebp]
	pop	ebp
	ret	0
?T2OLE@@YAPAGPAG@Z ENDP					; T2OLE
_TEXT	ENDS
PUBLIC	?GetResource@CEnBitmap@@SAHPBG0PAUHINSTANCE__@@PAXAAH@Z ; CEnBitmap::GetResource
PUBLIC	?LoadFromBuffer@CEnBitmap@@SAPAUIPicture@@PAEH@Z ; CEnBitmap::LoadFromBuffer
EXTRN	??2@YAPAXIPBDH@Z:NEAR				; operator new
;	COMDAT ?LoadImageResource@CEnBitmap@@SAPAUHBITMAP__@@IPBGPAUHINSTANCE__@@KHH@Z
_TEXT	SEGMENT
_uIDRes$ = 8
_szResourceType$ = 12
_hInst$ = 16
_crBack$ = 20
_cx$ = 24
_cy$ = 28
_pBuff$ = -8
_nSize$ = -4
_hbm$ = -12
_pPicture$75860 = -16
$T77110 = -20
$T77111 = -24
?LoadImageResource@CEnBitmap@@SAPAUHBITMAP__@@IPBGPAUHINSTANCE__@@KHH@Z PROC NEAR ; CEnBitmap::LoadImageResource, COMDAT

; 216  : {

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

; 217  : 	BYTE* pBuff = NULL;

	mov	DWORD PTR _pBuff$[ebp], 0

; 218  : 	int nSize = 0;

	mov	DWORD PTR _nSize$[ebp], 0

; 219  : 	HBITMAP hbm = NULL;

	mov	DWORD PTR _hbm$[ebp], 0

; 220  : 
; 221  : 	// first call is to get buffer size
; 222  : 	if (GetResource(MAKEINTRESOURCE(uIDRes), szResourceType, hInst, 0, nSize))

	lea	eax, DWORD PTR _nSize$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _hInst$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szResourceType$[ebp]
	push	edx
	mov	eax, DWORD PTR _uIDRes$[ebp]
	and	eax, 65535				; 0000ffffH
	push	eax
	call	?GetResource@CEnBitmap@@SAHPBG0PAUHINSTANCE__@@PAXAAH@Z ; CEnBitmap::GetResource
	add	esp, 20					; 00000014H
	test	eax, eax
	je	$L75853

; 224  : 		if (nSize > 0)

	cmp	DWORD PTR _nSize$[ebp], 0
	jle	$L75853

; 226  : 			pBuff = new BYTE[nSize];

	push	226					; 000000e2H
	push	OFFSET FLAT:_THIS_FILE
	mov	ecx, DWORD PTR _nSize$[ebp]
	push	ecx
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77110[ebp], eax
	mov	edx, DWORD PTR $T77110[ebp]
	mov	DWORD PTR _pBuff$[ebp], edx

; 227  : 			
; 228  : 			// this loads it
; 229  : 			if (GetResource(MAKEINTRESOURCE(uIDRes), szResourceType, hInst, pBuff, nSize))

	lea	eax, DWORD PTR _nSize$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pBuff$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hInst$[ebp]
	push	edx
	mov	eax, DWORD PTR _szResourceType$[ebp]
	push	eax
	mov	ecx, DWORD PTR _uIDRes$[ebp]
	and	ecx, 65535				; 0000ffffH
	push	ecx
	call	?GetResource@CEnBitmap@@SAHPBG0PAUHINSTANCE__@@PAXAAH@Z ; CEnBitmap::GetResource
	add	esp, 20					; 00000014H
	test	eax, eax
	je	SHORT $L75861

; 231  : 				IPicture* pPicture = LoadFromBuffer(pBuff, nSize);

	mov	edx, DWORD PTR _nSize$[ebp]
	push	edx
	mov	eax, DWORD PTR _pBuff$[ebp]
	push	eax
	call	?LoadFromBuffer@CEnBitmap@@SAPAUIPicture@@PAEH@Z ; CEnBitmap::LoadFromBuffer
	add	esp, 8
	mov	DWORD PTR _pPicture$75860[ebp], eax

; 232  : 
; 233  : 				if (pPicture)

	cmp	DWORD PTR _pPicture$75860[ebp], 0
	je	SHORT $L75861

; 235  : 					hbm = ExtractBitmap(pPicture, crBack, cx, cy);

	mov	ecx, DWORD PTR _cy$[ebp]
	push	ecx
	mov	edx, DWORD PTR _cx$[ebp]
	push	edx
	mov	eax, DWORD PTR _crBack$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pPicture$75860[ebp]
	push	ecx
	call	?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAUIPicture@@KHH@Z ; CEnBitmap::ExtractBitmap
	add	esp, 16					; 00000010H
	mov	DWORD PTR _hbm$[ebp], eax

; 236  : 					pPicture->Release();

	mov	edx, DWORD PTR _pPicture$75860[ebp]
	mov	eax, DWORD PTR [edx]
	mov	esi, esp
	mov	ecx, DWORD PTR _pPicture$75860[ebp]
	push	ecx
	call	DWORD PTR [eax+8]
	cmp	esi, esp
	call	__chkesp
$L75861:

; 239  : 			
; 240  : 			delete [] pBuff;

	mov	edx, DWORD PTR _pBuff$[ebp]
	mov	DWORD PTR $T77111[ebp], edx
	mov	eax, DWORD PTR $T77111[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75853:

; 243  : 
; 244  : 	return hbm;

	mov	eax, DWORD PTR _hbm$[ebp]

; 245  : }

	pop	esi
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?LoadImageResource@CEnBitmap@@SAPAUHBITMAP__@@IPBGPAUHINSTANCE__@@KHH@Z ENDP ; CEnBitmap::LoadImageResource
_TEXT	ENDS
EXTRN	__imp__GlobalAlloc@8:NEAR
EXTRN	__imp__GlobalLock@4:NEAR
EXTRN	__imp__GlobalUnlock@4:NEAR
EXTRN	__imp__CreateStreamOnHGlobal@12:NEAR
EXTRN	__imp__OleLoadPicture@20:NEAR
;	COMDAT ?LoadFromBuffer@CEnBitmap@@SAPAUIPicture@@PAEH@Z
_TEXT	SEGMENT
_pBuff$ = 8
_nSize$ = 12
_hGlobal$ = -16
_pData$ = -4
_pStream$ = -12
_pPicture$ = -8
?LoadFromBuffer@CEnBitmap@@SAPAUIPicture@@PAEH@Z PROC NEAR ; CEnBitmap::LoadFromBuffer, COMDAT

; 248  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 249  : 	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);

	mov	esi, esp
	mov	eax, DWORD PTR _nSize$[ebp]
	push	eax
	push	2
	call	DWORD PTR __imp__GlobalAlloc@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hGlobal$[ebp], eax

; 250  : 	void* pData = GlobalLock(hGlobal);

	mov	esi, esp
	mov	ecx, DWORD PTR _hGlobal$[ebp]
	push	ecx
	call	DWORD PTR __imp__GlobalLock@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _pData$[ebp], eax

; 251  : 	memcpy(pData, pBuff, nSize);

	mov	edx, DWORD PTR _nSize$[ebp]
	push	edx
	mov	eax, DWORD PTR _pBuff$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pData$[ebp]
	push	ecx
	call	_memcpy
	add	esp, 12					; 0000000cH

; 252  : 	GlobalUnlock(hGlobal);

	mov	esi, esp
	mov	edx, DWORD PTR _hGlobal$[ebp]
	push	edx
	call	DWORD PTR __imp__GlobalUnlock@4
	cmp	esi, esp
	call	__chkesp

; 253  : 
; 254  : 	IStream* pStream = NULL;

	mov	DWORD PTR _pStream$[ebp], 0

; 255  : 	IPicture* pPicture = NULL;

	mov	DWORD PTR _pPicture$[ebp], 0

; 256  : 
; 257  : 	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)

	mov	esi, esp
	lea	eax, DWORD PTR _pStream$[ebp]
	push	eax
	push	1
	mov	ecx, DWORD PTR _hGlobal$[ebp]
	push	ecx
	call	DWORD PTR __imp__CreateStreamOnHGlobal@12
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75872

; 259  : 		OleLoadPicture(pStream, nSize, FALSE, IID_IPicture, (LPVOID *)&pPicture);

	mov	esi, esp
	lea	edx, DWORD PTR _pPicture$[ebp]
	push	edx
	push	OFFSET FLAT:_IID_IPicture
	push	0
	mov	eax, DWORD PTR _nSize$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pStream$[ebp]
	push	ecx
	call	DWORD PTR __imp__OleLoadPicture@20
	cmp	esi, esp
	call	__chkesp

; 260  : 		pStream->Release();

	mov	edx, DWORD PTR _pStream$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	esi, esp
	mov	ecx, DWORD PTR _pStream$[ebp]
	push	ecx
	call	DWORD PTR [eax+8]
	cmp	esi, esp
	call	__chkesp
$L75872:

; 262  : 
; 263  : 	return pPicture; // caller releases

	mov	eax, DWORD PTR _pPicture$[ebp]

; 264  : }

	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?LoadFromBuffer@CEnBitmap@@SAPAUIPicture@@PAEH@Z ENDP	; CEnBitmap::LoadFromBuffer
_TEXT	ENDS
PUBLIC	?UnlockResource@@YGHPAX@Z			; UnlockResource
EXTRN	__imp__FreeResource@4:NEAR
EXTRN	__imp__LockResource@4:NEAR
EXTRN	__imp__FindResourceW@12:NEAR
EXTRN	__imp__LoadResource@8:NEAR
EXTRN	__imp__SizeofResource@8:NEAR
;	COMDAT ?GetResource@CEnBitmap@@SAHPBG0PAUHINSTANCE__@@PAXAAH@Z
_TEXT	SEGMENT
_lpName$ = 8
_lpType$ = 12
_hInst$ = 16
_pResource$ = 20
_nBufSize$ = 24
_hResInfo$ = -16
_hRes$ = -12
_lpRes$ = -8
_bResult$ = -4
?GetResource@CEnBitmap@@SAHPBG0PAUHINSTANCE__@@PAXAAH@Z PROC NEAR ; CEnBitmap::GetResource, COMDAT

; 267  : { 

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	push	esi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 268  : 	HRSRC		hResInfo;
; 269  : 	HANDLE		hRes;
; 270  : 	LPSTR		lpRes	= NULL; 

	mov	DWORD PTR _lpRes$[ebp], 0

; 271  : 	bool		bResult	= FALSE;

	mov	BYTE PTR _bResult$[ebp], 0

; 272  : 
; 273  : 	// Find the resource
; 274  : 	hResInfo = FindResource(hInst, lpName, lpType);

	mov	esi, esp
	mov	eax, DWORD PTR _lpType$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpName$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hInst$[ebp]
	push	edx
	call	DWORD PTR __imp__FindResourceW@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hResInfo$[ebp], eax

; 275  : 
; 276  : 	if (hResInfo == NULL) 

	cmp	DWORD PTR _hResInfo$[ebp], 0
	jne	SHORT $L75885

; 277  : 		return false;

	xor	eax, eax
	jmp	$L75880
$L75885:

; 278  : 
; 279  : 	// Load the resource
; 280  : 	hRes = LoadResource(hInst, hResInfo);

	mov	esi, esp
	mov	eax, DWORD PTR _hResInfo$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hInst$[ebp]
	push	ecx
	call	DWORD PTR __imp__LoadResource@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hRes$[ebp], eax

; 281  : 
; 282  : 	if (hRes == NULL) 

	cmp	DWORD PTR _hRes$[ebp], 0
	jne	SHORT $L75886

; 283  : 		return false;

	xor	eax, eax
	jmp	$L75880
$L75886:

; 284  : 
; 285  : 	// Lock the resource
; 286  : 	lpRes = (LPSTR)(LPCTSTR)LockResource(hRes);

	mov	esi, esp
	mov	edx, DWORD PTR _hRes$[ebp]
	push	edx
	call	DWORD PTR __imp__LockResource@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _lpRes$[ebp], eax

; 287  : 
; 288  : 	if (lpRes != NULL)

	cmp	DWORD PTR _lpRes$[ebp], 0
	je	SHORT $L75889

; 290  : 		if (pResource == NULL)

	cmp	DWORD PTR _pResource$[ebp], 0
	jne	SHORT $L75890

; 292  : 			nBufSize = SizeofResource(hInst, hResInfo);

	mov	esi, esp
	mov	eax, DWORD PTR _hResInfo$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hInst$[ebp]
	push	ecx
	call	DWORD PTR __imp__SizeofResource@8
	cmp	esi, esp
	call	__chkesp
	mov	edx, DWORD PTR _nBufSize$[ebp]
	mov	DWORD PTR [edx], eax

; 293  : 			bResult = true;

	mov	BYTE PTR _bResult$[ebp], 1

; 295  : 		else

	jmp	SHORT $L75893
$L75890:

; 297  : 			if (nBufSize >= (int)SizeofResource(hInst, hResInfo))

	mov	esi, esp
	mov	eax, DWORD PTR _hResInfo$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hInst$[ebp]
	push	ecx
	call	DWORD PTR __imp__SizeofResource@8
	cmp	esi, esp
	call	__chkesp
	mov	edx, DWORD PTR _nBufSize$[ebp]
	cmp	DWORD PTR [edx], eax
	jl	SHORT $L75893

; 299  : 				memcpy(pResource, lpRes, nBufSize);

	mov	eax, DWORD PTR _nBufSize$[ebp]
	mov	ecx, DWORD PTR [eax]
	push	ecx
	mov	edx, DWORD PTR _lpRes$[ebp]
	push	edx
	mov	eax, DWORD PTR _pResource$[ebp]
	push	eax
	call	_memcpy
	add	esp, 12					; 0000000cH

; 300  : 				bResult = true;

	mov	BYTE PTR _bResult$[ebp], 1
$L75893:

; 303  : 
; 304  : 		UnlockResource(hRes);  

	mov	ecx, DWORD PTR _hRes$[ebp]
	push	ecx
	call	?UnlockResource@@YGHPAX@Z		; UnlockResource
$L75889:

; 306  : 
; 307  : 	// Free the resource
; 308  : 	FreeResource(hRes);

	mov	esi, esp
	mov	edx, DWORD PTR _hRes$[ebp]
	push	edx
	call	DWORD PTR __imp__FreeResource@4
	cmp	esi, esp
	call	__chkesp

; 309  : 
; 310  : 	return bResult;

	mov	eax, DWORD PTR _bResult$[ebp]
	and	eax, 255				; 000000ffH
$L75880:

; 311  : }

	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetResource@CEnBitmap@@SAHPBG0PAUHINSTANCE__@@PAXAAH@Z ENDP ; CEnBitmap::GetResource
_TEXT	ENDS
;	COMDAT ?UnlockResource@@YGHPAX@Z
_TEXT	SEGMENT
?UnlockResource@@YGHPAX@Z PROC NEAR			; UnlockResource, COMDAT

; 214  : AFX_INLINE int WINAPI UnlockResource(HGLOBAL) { return 0; }

	push	ebp
	mov	ebp, esp
	xor	eax, eax
	pop	ebp
	ret	4
?UnlockResource@@YGHPAX@Z ENDP				; UnlockResource
_TEXT	ENDS
EXTRN	?FillSolidRect@CDC@@QAEXHHHHK@Z:NEAR		; CDC::FillSolidRect
EXTRN	??1CDC@@UAE@XZ:NEAR				; CDC::~CDC
EXTRN	?CreateCompatibleBitmap@CBitmap@@QAEHPAVCDC@@HH@Z:NEAR ; CBitmap::CreateCompatibleBitmap
EXTRN	?GetDC@CWnd@@QAEPAVCDC@@XZ:NEAR			; CWnd::GetDC
EXTRN	?ReleaseDC@CWnd@@QAEHPAVCDC@@@Z:NEAR		; CWnd::ReleaseDC
EXTRN	?GetDesktopWindow@CWnd@@SGPAV1@XZ:NEAR		; CWnd::GetDesktopWindow
EXTRN	??BCDC@@QBEPAUHDC__@@XZ:NEAR			; CDC::operator HDC__ *
EXTRN	??0CDC@@QAE@XZ:NEAR				; CDC::CDC
EXTRN	?CreateCompatibleDC@CDC@@QAEHPAV1@@Z:NEAR	; CDC::CreateCompatibleDC
EXTRN	?GetDeviceCaps@CDC@@QBEHH@Z:NEAR		; CDC::GetDeviceCaps
EXTRN	?SelectObject@CDC@@QAEPAVCBitmap@@PAV2@@Z:NEAR	; CDC::SelectObject
EXTRN	__imp__MulDiv@12:NEAR
EXTRN	?Detach@CGdiObject@@QAEPAXXZ:NEAR		; CGdiObject::Detach
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmap.cpp
xdata$x	SEGMENT
$T77124	DD	019930520H
	DD	02H
	DD	FLAT:$T77126
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77126	DD	0ffffffffH
	DD	FLAT:$L77121
	DD	00H
	DD	FLAT:$L77122
xdata$x	ENDS
;	COMDAT ?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAUIPicture@@KHH@Z
_TEXT	SEGMENT
_pPicture$ = 8
_crBack$ = 12
_cx$ = 16
_cy$ = 20
_bmMem$ = -36
_dcMem$ = -28
_pDC$ = -40
_hmWidth$75909 = -48
_hmHeight$75910 = -44
_nWidth$75911 = -52
_nHeight$75913 = -56
_pOldBM$75916 = -64
_hr$75918 = -60
$T77120 = -68
__$EHRec$ = -12
?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAUIPicture@@KHH@Z PROC NEAR ; CEnBitmap::ExtractBitmap, COMDAT

; 314  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77125
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 56					; 00000038H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-68]
	mov	ecx, 14					; 0000000eH
	mov	eax, -858993460				; ccccccccH
	rep stosd
$L75900:

; 315  : 	ASSERT(pPicture);

	cmp	DWORD PTR _pPicture$[ebp], 0
	jne	SHORT $L75903
	push	315					; 0000013bH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75903
	int	3
$L75903:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75900

; 316  : 
; 317  : 	if (!pPicture)

	cmp	DWORD PTR _pPicture$[ebp], 0
	jne	SHORT $L75904

; 318  : 		return NULL;

	xor	eax, eax
	jmp	$L75899
$L75904:

; 319  : 
; 320  : 	CBitmap bmMem;

	lea	ecx, DWORD PTR _bmMem$[ebp]
	call	??0CBitmap@@QAE@XZ			; CBitmap::CBitmap
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 321  : 	CDC dcMem;

	lea	ecx, DWORD PTR _dcMem$[ebp]
	call	??0CDC@@QAE@XZ				; CDC::CDC
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 322  : 	CDC* pDC = CWnd::GetDesktopWindow()->GetDC();

	call	?GetDesktopWindow@CWnd@@SGPAV1@XZ	; CWnd::GetDesktopWindow
	mov	ecx, eax
	call	?GetDC@CWnd@@QAEPAVCDC@@XZ		; CWnd::GetDC
	mov	DWORD PTR _pDC$[ebp], eax

; 323  : 
; 324  : 	if (dcMem.CreateCompatibleDC(pDC))

	mov	ecx, DWORD PTR _pDC$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _dcMem$[ebp]
	call	?CreateCompatibleDC@CDC@@QAEHPAV1@@Z	; CDC::CreateCompatibleDC
	test	eax, eax
	je	$L75915

; 326  : 		long hmWidth;
; 327  : 		long hmHeight;
; 328  : 
; 329  : 		pPicture->get_Width(&hmWidth);

	mov	esi, esp
	lea	edx, DWORD PTR _hmWidth$75909[ebp]
	push	edx
	mov	eax, DWORD PTR _pPicture$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR _pPicture$[ebp]
	push	edx
	call	DWORD PTR [ecx+24]
	cmp	esi, esp
	call	__chkesp

; 330  : 		pPicture->get_Height(&hmHeight);

	mov	esi, esp
	lea	eax, DWORD PTR _hmHeight$75910[ebp]
	push	eax
	mov	ecx, DWORD PTR _pPicture$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	eax, DWORD PTR _pPicture$[ebp]
	push	eax
	call	DWORD PTR [edx+28]
	cmp	esi, esp
	call	__chkesp

; 331  : 		
; 332  : 		int nWidth	= cx;

	mov	ecx, DWORD PTR _cx$[ebp]
	mov	DWORD PTR _nWidth$75911[ebp], ecx

; 333  : 
; 334  : 		if (nWidth == 0)

	cmp	DWORD PTR _nWidth$75911[ebp], 0
	jne	SHORT $L75912

; 335  : 			nWidth = MulDiv(hmWidth, pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);

	mov	esi, esp
	push	2540					; 000009ecH
	push	88					; 00000058H
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetDeviceCaps@CDC@@QBEHH@Z		; CDC::GetDeviceCaps
	push	eax
	mov	edx, DWORD PTR _hmWidth$75909[ebp]
	push	edx
	call	DWORD PTR __imp__MulDiv@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nWidth$75911[ebp], eax
$L75912:

; 336  : 
; 337  : 		int nHeight	= cy;

	mov	eax, DWORD PTR _cy$[ebp]
	mov	DWORD PTR _nHeight$75913[ebp], eax

; 338  : 
; 339  : 		if (nHeight == 0)

	cmp	DWORD PTR _nHeight$75913[ebp], 0
	jne	SHORT $L75914

; 340  : 			nHeight = MulDiv(hmHeight, pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);

	mov	esi, esp
	push	2540					; 000009ecH
	push	90					; 0000005aH
	mov	ecx, DWORD PTR _pDC$[ebp]
	call	?GetDeviceCaps@CDC@@QBEHH@Z		; CDC::GetDeviceCaps
	push	eax
	mov	ecx, DWORD PTR _hmHeight$75910[ebp]
	push	ecx
	call	DWORD PTR __imp__MulDiv@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nHeight$75913[ebp], eax
$L75914:

; 341  : 
; 342  : 		if (bmMem.CreateCompatibleBitmap(pDC, nWidth, nHeight))

	mov	edx, DWORD PTR _nHeight$75913[ebp]
	push	edx
	mov	eax, DWORD PTR _nWidth$75911[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDC$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _bmMem$[ebp]
	call	?CreateCompatibleBitmap@CBitmap@@QAEHPAVCDC@@HH@Z ; CBitmap::CreateCompatibleBitmap
	test	eax, eax
	je	$L75915

; 344  : 			CBitmap* pOldBM = dcMem.SelectObject(&bmMem);

	lea	edx, DWORD PTR _bmMem$[ebp]
	push	edx
	lea	ecx, DWORD PTR _dcMem$[ebp]
	call	?SelectObject@CDC@@QAEPAVCBitmap@@PAV2@@Z ; CDC::SelectObject
	mov	DWORD PTR _pOldBM$75916[ebp], eax

; 345  : 
; 346  : 			if (crBack != -1)

	cmp	DWORD PTR _crBack$[ebp], -1
	je	SHORT $L75917

; 347  : 				dcMem.FillSolidRect(0, 0, nWidth, nHeight, crBack);

	mov	eax, DWORD PTR _crBack$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nHeight$75913[ebp]
	push	ecx
	mov	edx, DWORD PTR _nWidth$75911[ebp]
	push	edx
	push	0
	push	0
	lea	ecx, DWORD PTR _dcMem$[ebp]
	call	?FillSolidRect@CDC@@QAEXHHHHK@Z		; CDC::FillSolidRect
$L75917:

; 348  : 			
; 349  : 			HRESULT hr = pPicture->Render(dcMem, 0, 0, nWidth, nHeight, 0, hmHeight, hmWidth, -hmHeight, NULL);

	mov	esi, esp
	push	0
	mov	eax, DWORD PTR _hmHeight$75910[ebp]
	neg	eax
	push	eax
	mov	ecx, DWORD PTR _hmWidth$75909[ebp]
	push	ecx
	mov	edx, DWORD PTR _hmHeight$75910[ebp]
	push	edx
	push	0
	mov	eax, DWORD PTR _nHeight$75913[ebp]
	push	eax
	mov	ecx, DWORD PTR _nWidth$75911[ebp]
	push	ecx
	push	0
	push	0
	lea	ecx, DWORD PTR _dcMem$[ebp]
	call	??BCDC@@QBEPAUHDC__@@XZ			; CDC::operator HDC__ *
	push	eax
	mov	edx, DWORD PTR _pPicture$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _pPicture$[ebp]
	push	ecx
	call	DWORD PTR [eax+32]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hr$75918[ebp], eax
$L75919:

; 350  : 			ASSERT (SUCCEEDED(hr));

	cmp	DWORD PTR _hr$75918[ebp], 0
	jge	SHORT $L75923
	push	350					; 0000015eH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75923
	int	3
$L75923:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75919

; 351  : 
; 352  : 			// cleanup
; 353  : 			dcMem.SelectObject(pOldBM);

	mov	eax, DWORD PTR _pOldBM$75916[ebp]
	push	eax
	lea	ecx, DWORD PTR _dcMem$[ebp]
	call	?SelectObject@CDC@@QAEPAVCBitmap@@PAV2@@Z ; CDC::SelectObject
$L75915:

; 356  : 
; 357  : 	CWnd::GetDesktopWindow()->ReleaseDC(pDC);

	mov	ecx, DWORD PTR _pDC$[ebp]
	push	ecx
	call	?GetDesktopWindow@CWnd@@SGPAV1@XZ	; CWnd::GetDesktopWindow
	mov	ecx, eax
	call	?ReleaseDC@CWnd@@QAEHPAVCDC@@@Z		; CWnd::ReleaseDC

; 358  : 
; 359  : 	return (HBITMAP)bmMem.Detach();

	lea	ecx, DWORD PTR _bmMem$[ebp]
	call	?Detach@CGdiObject@@QAEPAXXZ		; CGdiObject::Detach
	mov	DWORD PTR $T77120[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _dcMem$[ebp]
	call	??1CDC@@UAE@XZ				; CDC::~CDC
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _bmMem$[ebp]
	call	??1CBitmap@@UAE@XZ			; CBitmap::~CBitmap
	mov	eax, DWORD PTR $T77120[ebp]
$L75899:

; 360  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 68					; 00000044H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77121:
	lea	ecx, DWORD PTR _bmMem$[ebp]
	call	??1CBitmap@@UAE@XZ			; CBitmap::~CBitmap
	ret	0
$L77122:
	lea	ecx, DWORD PTR _dcMem$[ebp]
	call	??1CDC@@UAE@XZ				; CDC::~CDC
	ret	0
$L77125:
	mov	eax, OFFSET FLAT:$T77124
	jmp	___CxxFrameHandler
text$x	ENDS
?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAUIPicture@@KHH@Z ENDP ; CEnBitmap::ExtractBitmap
PUBLIC	?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@ABVCImageList@@KHH@Z ; CEnBitmap::ExtractBitmap
EXTRN	??0CImageList@@QAE@XZ:NEAR			; CImageList::CImageList
EXTRN	?Create@CImageList@@QAEHHHIHH@Z:NEAR		; CImageList::Create
EXTRN	?Add@CImageList@@QAEHPAUHICON__@@@Z:NEAR	; CImageList::Add
EXTRN	??1CImageList@@UAE@XZ:NEAR			; CImageList::~CImageList
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmap.cpp
xdata$x	SEGMENT
$T77134	DD	019930520H
	DD	01H
	DD	FLAT:$T77136
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77136	DD	0ffffffffH
	DD	FLAT:$L77132
xdata$x	ENDS
;	COMDAT ?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAUHICON__@@KHH@Z
_TEXT	SEGMENT
_hIcon$ = 8
_crBack$ = 12
_cx$ = 16
_cy$ = 20
_hbm$ = -24
_il$ = -20
$T77131 = -28
__$EHRec$ = -12
?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAUHICON__@@KHH@Z PROC NEAR ; CEnBitmap::ExtractBitmap, COMDAT

; 363  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77135
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
$L75932:

; 364  : 	// sanity checks
; 365  : 	ASSERT(hIcon);

	cmp	DWORD PTR _hIcon$[ebp], 0
	jne	SHORT $L75935
	push	365					; 0000016dH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75935
	int	3
$L75935:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75932

; 366  : 
; 367  : 	if (hIcon == NULL)

	cmp	DWORD PTR _hIcon$[ebp], 0
	jne	SHORT $L75936

; 368  : 		return NULL;

	xor	eax, eax
	jmp	$L75931
$L75936:

; 369  : 
; 370  : 	if (cx == 0 || cy == 0)

	cmp	DWORD PTR _cx$[ebp], 0
	je	SHORT $L75938
	cmp	DWORD PTR _cy$[ebp], 0
	jne	SHORT $L75937
$L75938:

; 372  : 		cx = cy = 32;

	mov	DWORD PTR _cy$[ebp], 32			; 00000020H
	mov	ecx, DWORD PTR _cy$[ebp]
	mov	DWORD PTR _cx$[ebp], ecx
$L75937:

; 374  : 
; 375  : 	// when we load the icon we have no idea whether it 
; 376  : 	// contains an image of the required size, so we use an
; 377  : 	// imagelist to do the heavy lifting
; 378  : 	HBITMAP hbm = NULL;

	mov	DWORD PTR _hbm$[ebp], 0

; 379  : 	CImageList il;

	lea	ecx, DWORD PTR _il$[ebp]
	call	??0CImageList@@QAE@XZ			; CImageList::CImageList
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 380  : 			
; 381  : 	if (il.Create(cx, cy, ILC_COLOR32 | ILC_MASK, 1, 1))

	push	1
	push	1
	push	33					; 00000021H
	mov	edx, DWORD PTR _cy$[ebp]
	push	edx
	mov	eax, DWORD PTR _cx$[ebp]
	push	eax
	lea	ecx, DWORD PTR _il$[ebp]
	call	?Create@CImageList@@QAEHHHIHH@Z		; CImageList::Create
	test	eax, eax
	je	SHORT $L75942

; 383  : 		if (il.Add(hIcon) == 0)

	mov	ecx, DWORD PTR _hIcon$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _il$[ebp]
	call	?Add@CImageList@@QAEHPAUHICON__@@@Z	; CImageList::Add
	test	eax, eax
	jne	SHORT $L75942

; 384  : 			hbm = ExtractBitmap(il, crBack, cx, cy);

	mov	edx, DWORD PTR _cy$[ebp]
	push	edx
	mov	eax, DWORD PTR _cx$[ebp]
	push	eax
	mov	ecx, DWORD PTR _crBack$[ebp]
	push	ecx
	lea	edx, DWORD PTR _il$[ebp]
	push	edx
	call	?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@ABVCImageList@@KHH@Z ; CEnBitmap::ExtractBitmap
	add	esp, 16					; 00000010H
	mov	DWORD PTR _hbm$[ebp], eax
$L75942:

; 386  : 
; 387  : 	return hbm;

	mov	eax, DWORD PTR _hbm$[ebp]
	mov	DWORD PTR $T77131[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _il$[ebp]
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	mov	eax, DWORD PTR $T77131[ebp]
$L75931:

; 388  : }

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
$L77132:
	lea	ecx, DWORD PTR _il$[ebp]
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	ret	0
$L77135:
	mov	eax, OFFSET FLAT:$T77134
	jmp	___CxxFrameHandler
text$x	ENDS
?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAUHICON__@@KHH@Z ENDP ; CEnBitmap::ExtractBitmap
EXTRN	__imp__GetObjectW@12:NEAR
EXTRN	?FromHandle@CBitmap@@SGPAV1@PAUHBITMAP__@@@Z:NEAR ; CBitmap::FromHandle
EXTRN	?Add@CImageList@@QAEHPAVCBitmap@@K@Z:NEAR	; CImageList::Add
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmap.cpp
xdata$x	SEGMENT
$T77144	DD	019930520H
	DD	01H
	DD	FLAT:$T77146
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77146	DD	0ffffffffH
	DD	FLAT:$L77142
xdata$x	ENDS
;	COMDAT ?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAU2@KHH@Z
_TEXT	SEGMENT
_hbmGdip$ = 8
_crBack$ = 12
_cx$ = 16
_cy$ = 20
_hbm$ = -24
_bm$75958 = -48
_il$ = -20
$T77141 = -52
__$EHRec$ = -12
?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAU2@KHH@Z PROC NEAR ; CEnBitmap::ExtractBitmap, COMDAT

; 392  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77145
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 40					; 00000028H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-52]
	mov	ecx, 10					; 0000000aH
	mov	eax, -858993460				; ccccccccH
	rep stosd
$L75950:

; 393  : 	// sanity checks
; 394  : 	ASSERT(hbmGdip);

	cmp	DWORD PTR _hbmGdip$[ebp], 0
	jne	SHORT $L75953
	push	394					; 0000018aH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75953
	int	3
$L75953:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75950

; 395  : 
; 396  : 	if (hbmGdip == NULL)

	cmp	DWORD PTR _hbmGdip$[ebp], 0
	jne	SHORT $L75954

; 397  : 		return NULL;

	xor	eax, eax
	jmp	$L75949
$L75954:

; 398  : 
; 399  : 	HBITMAP hbm = NULL;

	mov	DWORD PTR _hbm$[ebp], 0

; 400  : 
; 401  : 	if (cx == 0 || cy == 0)

	cmp	DWORD PTR _cx$[ebp], 0
	je	SHORT $L75957
	cmp	DWORD PTR _cy$[ebp], 0
	jne	SHORT $L75956
$L75957:

; 403  : 		BITMAP bm = { 0 };

	mov	DWORD PTR _bm$75958[ebp], 0
	xor	ecx, ecx
	mov	DWORD PTR _bm$75958[ebp+4], ecx
	mov	DWORD PTR _bm$75958[ebp+8], ecx
	mov	DWORD PTR _bm$75958[ebp+12], ecx
	mov	DWORD PTR _bm$75958[ebp+16], ecx
	mov	DWORD PTR _bm$75958[ebp+20], ecx
$L75959:

; 404  : 		VERIFY (::GetObject(hbmGdip, sizeof(bm), &bm));

	mov	esi, esp
	lea	edx, DWORD PTR _bm$75958[ebp]
	push	edx
	push	24					; 00000018H
	mov	eax, DWORD PTR _hbmGdip$[ebp]
	push	eax
	call	DWORD PTR __imp__GetObjectW@12
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75962
	push	404					; 00000194H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75962
	int	3
$L75962:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75959

; 405  : 
; 406  : 		cx = bm.bmWidth;

	mov	edx, DWORD PTR _bm$75958[ebp+4]
	mov	DWORD PTR _cx$[ebp], edx

; 407  : 		cy = bm.bmHeight;

	mov	eax, DWORD PTR _bm$75958[ebp+8]
	mov	DWORD PTR _cy$[ebp], eax
$L75956:

; 409  : 
; 410  : 	CImageList il;

	lea	ecx, DWORD PTR _il$[ebp]
	call	??0CImageList@@QAE@XZ			; CImageList::CImageList
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 411  : 	
; 412  : 	if (il.Create(cx, cy, ILC_COLOR32 | ILC_MASK, 1, 1))

	push	1
	push	1
	push	33					; 00000021H
	mov	ecx, DWORD PTR _cy$[ebp]
	push	ecx
	mov	edx, DWORD PTR _cx$[ebp]
	push	edx
	lea	ecx, DWORD PTR _il$[ebp]
	call	?Create@CImageList@@QAEHHHIHH@Z		; CImageList::Create
	test	eax, eax
	je	SHORT $L75965

; 414  : 		if (il.Add(CBitmap::FromHandle(hbmGdip), crBack) == 0)

	mov	eax, DWORD PTR _crBack$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hbmGdip$[ebp]
	push	ecx
	call	?FromHandle@CBitmap@@SGPAV1@PAUHBITMAP__@@@Z ; CBitmap::FromHandle
	push	eax
	lea	ecx, DWORD PTR _il$[ebp]
	call	?Add@CImageList@@QAEHPAVCBitmap@@K@Z	; CImageList::Add
	test	eax, eax
	jne	SHORT $L75965

; 416  : 			hbm = ExtractBitmap(il, crBack, cx, cy);

	mov	edx, DWORD PTR _cy$[ebp]
	push	edx
	mov	eax, DWORD PTR _cx$[ebp]
	push	eax
	mov	ecx, DWORD PTR _crBack$[ebp]
	push	ecx
	lea	edx, DWORD PTR _il$[ebp]
	push	edx
	call	?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@ABVCImageList@@KHH@Z ; CEnBitmap::ExtractBitmap
	add	esp, 16					; 00000010H
	mov	DWORD PTR _hbm$[ebp], eax
$L75965:

; 419  : 
; 420  : 	return hbm;

	mov	eax, DWORD PTR _hbm$[ebp]
	mov	DWORD PTR $T77141[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _il$[ebp]
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	mov	eax, DWORD PTR $T77141[ebp]
$L75949:

; 421  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 52					; 00000034H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77142:
	lea	ecx, DWORD PTR _il$[ebp]
	call	??1CImageList@@UAE@XZ			; CImageList::~CImageList
	ret	0
$L77145:
	mov	eax, OFFSET FLAT:$T77144
	jmp	___CxxFrameHandler
text$x	ENDS
?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@PAU2@KHH@Z ENDP ; CEnBitmap::ExtractBitmap
EXTRN	__imp__GetDesktopWindow@0:NEAR
EXTRN	__imp__ExtTextOutW@32:NEAR
EXTRN	__imp__ImageList_Draw@24:NEAR
EXTRN	__imp__SelectObject@8:NEAR
EXTRN	__imp__SetBkColor@8:NEAR
EXTRN	__imp__CreateCompatibleBitmap@12:NEAR
EXTRN	__imp__CreateCompatibleDC@4:NEAR
EXTRN	__imp__GetDC@4:NEAR
EXTRN	__imp__ReleaseDC@8:NEAR
EXTRN	??0CRect@@QAE@HHHH@Z:NEAR			; CRect::CRect
EXTRN	__imp__DeleteDC@4:NEAR
EXTRN	??BCRect@@QAEPAUtagRECT@@XZ:NEAR		; CRect::operator tagRECT *
EXTRN	??BCImageList@@QBEPAU_IMAGELIST@@XZ:NEAR	; CImageList::operator _IMAGELIST *
;	COMDAT ?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@ABVCImageList@@KHH@Z
_TEXT	SEGMENT
_il$ = 8
_crBack$ = 12
_cx$ = 16
_cy$ = 20
_hbm$ = -12
_hDC$ = -8
_hMemDC$ = -4
_hOldBitmap$75978 = -16
$T77151 = -32
?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@ABVCImageList@@KHH@Z PROC NEAR ; CEnBitmap::ExtractBitmap, COMDAT

; 425  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 32					; 00000020H
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

; 426  : 	HBITMAP hbm = NULL;

	mov	DWORD PTR _hbm$[ebp], 0

; 427  : // 	IMAGEINFO ii = { 0 };
; 428  : // 
; 429  : // 	VERIFY (il.GetImageInfo(0, &ii));
; 430  : // 
; 431  : // 	int cx = (ii.rcImage.right - ii.rcImage.left);
; 432  : // 	int cy = (ii.rcImage.bottom - ii.rcImage.top);
; 433  : 		
; 434  : 	HDC hDC = ::GetDC(::GetDesktopWindow());

	mov	esi, esp
	call	DWORD PTR __imp__GetDesktopWindow@0
	cmp	esi, esp
	call	__chkesp
	mov	esi, esp
	push	eax
	call	DWORD PTR __imp__GetDC@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hDC$[ebp], eax

; 435  : 	HDC hMemDC = ::CreateCompatibleDC(NULL);

	mov	esi, esp
	push	0
	call	DWORD PTR __imp__CreateCompatibleDC@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hMemDC$[ebp], eax

; 436  : 
; 437  : 	if (hMemDC)

	cmp	DWORD PTR _hMemDC$[ebp], 0
	je	$L75976

; 439  : 		hbm = ::CreateCompatibleBitmap(hDC, cx, cy);

	mov	esi, esp
	mov	eax, DWORD PTR _cy$[ebp]
	push	eax
	mov	ecx, DWORD PTR _cx$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hDC$[ebp]
	push	edx
	call	DWORD PTR __imp__CreateCompatibleBitmap@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hbm$[ebp], eax

; 440  : 		
; 441  : 		if (hbm)

	cmp	DWORD PTR _hbm$[ebp], 0
	je	$L75977

; 443  : 			HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbm);

	mov	esi, esp
	mov	eax, DWORD PTR _hbm$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hMemDC$[ebp]
	push	ecx
	call	DWORD PTR __imp__SelectObject@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hOldBitmap$75978[ebp], eax

; 444  : 			
; 445  : 			::SetBkColor(hMemDC, crBack);

	mov	esi, esp
	mov	edx, DWORD PTR _crBack$[ebp]
	push	edx
	mov	eax, DWORD PTR _hMemDC$[ebp]
	push	eax
	call	DWORD PTR __imp__SetBkColor@8
	cmp	esi, esp
	call	__chkesp

; 446  : 			::ExtTextOut(hMemDC, 0, 0, ETO_OPAQUE, CRect(0, 0, cx, cy), NULL, 0, NULL);

	mov	esi, esp
	push	0
	push	0
	push	0
	mov	ecx, DWORD PTR _cy$[ebp]
	push	ecx
	mov	edx, DWORD PTR _cx$[ebp]
	push	edx
	push	0
	push	0
	lea	ecx, DWORD PTR $T77151[ebp]
	call	??0CRect@@QAE@HHHH@Z			; CRect::CRect
	mov	ecx, eax
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	push	2
	push	0
	push	0
	mov	eax, DWORD PTR _hMemDC$[ebp]
	push	eax
	call	DWORD PTR __imp__ExtTextOutW@32
	cmp	esi, esp
	call	__chkesp

; 447  : 			
; 448  : 			ImageList_Draw(il, 0, hMemDC, 0, 0, ILD_TRANSPARENT);

	mov	esi, esp
	push	1
	push	0
	push	0
	mov	ecx, DWORD PTR _hMemDC$[ebp]
	push	ecx
	push	0
	mov	ecx, DWORD PTR _il$[ebp]
	call	??BCImageList@@QBEPAU_IMAGELIST@@XZ	; CImageList::operator _IMAGELIST *
	push	eax
	call	DWORD PTR __imp__ImageList_Draw@24
	cmp	esi, esp
	call	__chkesp

; 449  : 			
; 450  : 			::SelectObject(hMemDC, hOldBitmap);

	mov	esi, esp
	mov	edx, DWORD PTR _hOldBitmap$75978[ebp]
	push	edx
	mov	eax, DWORD PTR _hMemDC$[ebp]
	push	eax
	call	DWORD PTR __imp__SelectObject@8
	cmp	esi, esp
	call	__chkesp
$L75977:

; 452  : 		
; 453  : 		::DeleteDC(hMemDC);

	mov	esi, esp
	mov	ecx, DWORD PTR _hMemDC$[ebp]
	push	ecx
	call	DWORD PTR __imp__DeleteDC@4
	cmp	esi, esp
	call	__chkesp
$L75976:

; 455  : 	
; 456  : 	// cleanup
; 457  : 	::ReleaseDC(::GetDesktopWindow(), hDC);

	mov	esi, esp
	mov	edx, DWORD PTR _hDC$[ebp]
	push	edx
	mov	edi, esp
	call	DWORD PTR __imp__GetDesktopWindow@0
	cmp	edi, esp
	call	__chkesp
	push	eax
	call	DWORD PTR __imp__ReleaseDC@8
	cmp	esi, esp
	call	__chkesp

; 458  : 
; 459  : 	return hbm;

	mov	eax, DWORD PTR _hbm$[ebp]

; 460  : }

	pop	edi
	pop	esi
	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?ExtractBitmap@CEnBitmap@@KAPAUHBITMAP__@@ABVCImageList@@KHH@Z ENDP ; CEnBitmap::ExtractBitmap
_TEXT	ENDS
PUBLIC	?CopyToClipboard@CEnBitmap@@QBEHPAUHWND__@@@Z	; CEnBitmap::CopyToClipboard
EXTRN	??0CClipboard@@QAE@PAUHWND__@@@Z:NEAR		; CClipboard::CClipboard
EXTRN	??1CClipboard@@QAE@XZ:NEAR			; CClipboard::~CClipboard
EXTRN	?SetData@CClipboard@@QBEHABVCBitmap@@@Z:NEAR	; CClipboard::SetData
EXTRN	?GetSafeHandle@CGdiObject@@QBEPAXXZ:NEAR	; CGdiObject::GetSafeHandle
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmap.cpp
xdata$x	SEGMENT
$T77158	DD	019930520H
	DD	01H
	DD	FLAT:$T77160
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77160	DD	0ffffffffH
	DD	FLAT:$L77156
xdata$x	ENDS
;	COMDAT ?CopyToClipboard@CEnBitmap@@QBEHPAUHWND__@@@Z
_TEXT	SEGMENT
_hWnd$ = 8
_this$ = -28
$T77154 = -16
$T77155 = -24
__$EHRec$ = -12
?CopyToClipboard@CEnBitmap@@QBEHPAUHWND__@@@Z PROC NEAR	; CEnBitmap::CopyToClipboard, COMDAT

; 463  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77159
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

; 464  : 	if (!hWnd || !GetSafeHandle())

	cmp	DWORD PTR _hWnd$[ebp], 0
	je	SHORT $L75986
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHandle@CGdiObject@@QBEPAXXZ	; CGdiObject::GetSafeHandle
	test	eax, eax
	jne	SHORT $L75985
$L75986:

; 465  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75984
$L75985:

; 466  : 
; 467  : 	return CClipboard(hWnd).SetData(*this);

	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hWnd$[ebp]
	push	ecx
	lea	ecx, DWORD PTR $T77155[ebp]
	call	??0CClipboard@@QAE@PAUHWND__@@@Z	; CClipboard::CClipboard
	mov	DWORD PTR -32+[ebp], eax
	mov	edx, DWORD PTR -32+[ebp]
	mov	DWORD PTR -36+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -36+[ebp]
	call	?SetData@CClipboard@@QBEHABVCBitmap@@@Z	; CClipboard::SetData
	mov	DWORD PTR $T77154[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T77155[ebp]
	call	??1CClipboard@@QAE@XZ			; CClipboard::~CClipboard
	mov	eax, DWORD PTR $T77154[ebp]
$L75984:

; 468  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77156:
	lea	ecx, DWORD PTR $T77155[ebp]
	call	??1CClipboard@@QAE@XZ			; CClipboard::~CClipboard
	ret	0
$L77159:
	mov	eax, OFFSET FLAT:$T77158
	jmp	___CxxFrameHandler
text$x	ENDS
?CopyToClipboard@CEnBitmap@@QBEHPAUHWND__@@@Z ENDP	; CEnBitmap::CopyToClipboard
PUBLIC	?CopyImageFileToClipboard@CEnBitmap@@SAHPAUHWND__@@PBGK@Z ; CEnBitmap::CopyImageFileToClipboard
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmap.cpp
xdata$x	SEGMENT
$T77169	DD	019930520H
	DD	01H
	DD	FLAT:$T77171
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77171	DD	0ffffffffH
	DD	FLAT:$L77167
xdata$x	ENDS
;	COMDAT ?CopyImageFileToClipboard@CEnBitmap@@SAHPAUHWND__@@PBGK@Z
_TEXT	SEGMENT
_hWnd$ = 8
_szImagePath$ = 12
_crBack$ = 16
_bm$ = -32
$T77165 = -36
$T77166 = -40
__$EHRec$ = -12
?CopyImageFileToClipboard@CEnBitmap@@SAHPAUHWND__@@PBGK@Z PROC NEAR ; CEnBitmap::CopyImageFileToClipboard, COMDAT

; 471  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77170
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

; 472  : 	CEnBitmap bm;

	push	16777215				; 00ffffffH
	lea	ecx, DWORD PTR _bm$[ebp]
	call	??0CEnBitmap@@QAE@K@Z			; CEnBitmap::CEnBitmap
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 473  : 
; 474  : 	if (bm.LoadImage(szImagePath, crBack))

	mov	eax, DWORD PTR _crBack$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szImagePath$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _bm$[ebp]
	call	?LoadImageW@CEnBitmap@@QAEHPBGK@Z	; CEnBitmap::LoadImageW
	test	eax, eax
	je	SHORT $L76057

; 475  : 		return bm.CopyToClipboard(hWnd);

	mov	edx, DWORD PTR _hWnd$[ebp]
	push	edx
	lea	ecx, DWORD PTR _bm$[ebp]
	call	?CopyToClipboard@CEnBitmap@@QBEHPAUHWND__@@@Z ; CEnBitmap::CopyToClipboard
	mov	DWORD PTR $T77165[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _bm$[ebp]
	call	??1CEnBitmap@@UAE@XZ			; CEnBitmap::~CEnBitmap
	mov	eax, DWORD PTR $T77165[ebp]
	jmp	SHORT $L76055
$L76057:

; 476  : 
; 477  : 	return FALSE;

	mov	DWORD PTR $T77166[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _bm$[ebp]
	call	??1CEnBitmap@@UAE@XZ			; CEnBitmap::~CEnBitmap
	mov	eax, DWORD PTR $T77166[ebp]
$L76055:

; 478  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77167:
	lea	ecx, DWORD PTR _bm$[ebp]
	call	??1CEnBitmap@@UAE@XZ			; CEnBitmap::~CEnBitmap
	ret	0
$L77170:
	mov	eax, OFFSET FLAT:$T77169
	jmp	___CxxFrameHandler
text$x	ENDS
?CopyImageFileToClipboard@CEnBitmap@@SAHPAUHWND__@@PBGK@Z ENDP ; CEnBitmap::CopyImageFileToClipboard
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	?MakeUpper@CString@@QAEXXZ:NEAR			; CString::MakeUpper
EXTRN	?Find@CString@@QBEHPBG@Z:NEAR			; CString::Find
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT data
; File D:\_CODE\Shared\EnBitmap.cpp
data	SEGMENT
$SG76065 DB	'.', 00H, 'B', 00H, 'M', 00H, 'P', 00H, 00H, 00H
	ORG $+2
$SG76069 DB	'.', 00H, 'I', 00H, 'C', 00H, 'O', 00H, 00H, 00H
	ORG $+2
$SG76074 DB	'.', 00H, 'J', 00H, 'P', 00H, 'G', 00H, 00H, 00H
	ORG $+2
$SG76075 DB	'.', 00H, 'J', 00H, 'P', 00H, 'E', 00H, 'G', 00H, 00H, 00H
$SG76079 DB	'.', 00H, 'G', 00H, 'I', 00H, 'F', 00H, 00H, 00H
	ORG $+2
$SG76083 DB	'.', 00H, 'P', 00H, 'N', 00H, 'G', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T77184	DD	019930520H
	DD	01H
	DD	FLAT:$T77186
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77186	DD	0ffffffffH
	DD	FLAT:$L77182
xdata$x	ENDS
;	COMDAT ?GetFileType@CEnBitmap@@SA?AW4EB_IMAGETYPE@@PBG@Z
_TEXT	SEGMENT
_szImagePath$ = 8
_sPath$ = -16
$T77176 = -20
$T77177 = -24
$T77178 = -28
$T77179 = -32
$T77180 = -36
$T77181 = -40
__$EHRec$ = -12
?GetFileType@CEnBitmap@@SA?AW4EB_IMAGETYPE@@PBG@Z PROC NEAR ; CEnBitmap::GetFileType, COMDAT

; 481  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77185
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

; 482  : 	CString sPath(szImagePath);

	mov	eax, DWORD PTR _szImagePath$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 483  : 	sPath.MakeUpper();

	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?MakeUpper@CString@@QAEXXZ		; CString::MakeUpper

; 484  : 
; 485  : 	if (sPath.Find(_T(".BMP")) > 0)

	push	OFFSET FLAT:$SG76065
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	test	eax, eax
	jle	SHORT $L76064

; 486  : 		return FT_BMP;

	mov	DWORD PTR $T77176[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77176[ebp]
	jmp	$L76062
$L76064:

; 487  : 
; 488  : 	else if (sPath.Find(_T(".ICO")) > 0)

	push	OFFSET FLAT:$SG76069
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	test	eax, eax
	jle	SHORT $L76068

; 489  : 		return FT_ICO;

	mov	DWORD PTR $T77177[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77177[ebp]
	jmp	$L76062
$L76068:

; 490  : 
; 491  : 	else if (sPath.Find(_T(".JPG")) > 0 || sPath.Find(_T(".JPEG")) > 0)

	push	OFFSET FLAT:$SG76074
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	test	eax, eax
	jg	SHORT $L76073
	push	OFFSET FLAT:$SG76075
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	test	eax, eax
	jle	SHORT $L76072
$L76073:

; 492  : 		return FT_JPG;

	mov	DWORD PTR $T77178[ebp], 2
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77178[ebp]
	jmp	SHORT $L76062
$L76072:

; 493  : 
; 494  : 	else if (sPath.Find(_T(".GIF")) > 0)

	push	OFFSET FLAT:$SG76079
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	test	eax, eax
	jle	SHORT $L76078

; 495  : 		return FT_GIF;

	mov	DWORD PTR $T77179[ebp], 3
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77179[ebp]
	jmp	SHORT $L76062
$L76078:

; 496  : 
; 497  : #ifdef PNG_SUPPORT
; 498  : 	else if (sPath.Find(_T(".PNG")) > 0)

	push	OFFSET FLAT:$SG76083
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	test	eax, eax
	jle	SHORT $L76082

; 499  : 		return FT_PNG;

	mov	DWORD PTR $T77180[ebp], 4
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77180[ebp]
	jmp	SHORT $L76062
$L76082:

; 500  : #endif
; 501  : 
; 502  : 	// else
; 503  : 	return FT_UNKNOWN;

	mov	DWORD PTR $T77181[ebp], 5
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T77181[ebp]
$L76062:

; 504  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77182:
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77185:
	mov	eax, OFFSET FLAT:$T77184
	jmp	___CxxFrameHandler
text$x	ENDS
?GetFileType@CEnBitmap@@SA?AW4EB_IMAGETYPE@@PBG@Z ENDP	; CEnBitmap::GetFileType
PUBLIC	?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ; CEnBitmap::ProcessImage
PUBLIC	?ProcessImage@CEnBitmap@@QAEHAAV?$CArray@PAVC32BitImageProcessor@@PAV1@@@K@Z ; CEnBitmap::ProcessImage
PUBLIC	??0?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAE@XZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::CArray<C32BitImageProcessor *,C32BitImageProcessor *>
PUBLIC	?Add@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEHPAVC32BitImageProcessor@@@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::Add
PUBLIC	??1?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAE@XZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::~CArray<C32BitImageProcessor *,C32BitImageProcessor *>
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmap.cpp
xdata$x	SEGMENT
$T77195	DD	019930520H
	DD	01H
	DD	FLAT:$T77197
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77197	DD	0ffffffffH
	DD	FLAT:$L77193
xdata$x	ENDS
;	COMDAT ?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z
_TEXT	SEGMENT
_pProcessor$ = 8
_crMask$ = 12
_this$ = -40
_aProcessors$ = -32
$T77192 = -36
__$EHRec$ = -12
?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z PROC NEAR ; CEnBitmap::ProcessImage, COMDAT

; 507  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77196
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

; 508  : 	C32BIPArray aProcessors;

	lea	ecx, DWORD PTR _aProcessors$[ebp]
	call	??0?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAE@XZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::CArray<C32BitImageProcessor *,C32BitImageProcessor *>
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 509  : 
; 510  : 	aProcessors.Add(pProcessor);

	mov	eax, DWORD PTR _pProcessor$[ebp]
	push	eax
	lea	ecx, DWORD PTR _aProcessors$[ebp]
	call	?Add@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEHPAVC32BitImageProcessor@@@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::Add

; 511  : 
; 512  : 	return ProcessImage(aProcessors, crMask);

	mov	ecx, DWORD PTR _crMask$[ebp]
	push	ecx
	lea	edx, DWORD PTR _aProcessors$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessImage@CEnBitmap@@QAEHAAV?$CArray@PAVC32BitImageProcessor@@PAV1@@@K@Z ; CEnBitmap::ProcessImage
	mov	DWORD PTR $T77192[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _aProcessors$[ebp]
	call	??1?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAE@XZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::~CArray<C32BitImageProcessor *,C32BitImageProcessor *>
	mov	eax, DWORD PTR $T77192[ebp]

; 513  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77193:
	lea	ecx, DWORD PTR _aProcessors$[ebp]
	call	??1?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAE@XZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::~CArray<C32BitImageProcessor *,C32BitImageProcessor *>
	ret	0
$L77196:
	mov	eax, OFFSET FLAT:$T77195
	jmp	___CxxFrameHandler
text$x	ENDS
?ProcessImage@CEnBitmap@@QAEHPAVC32BitImageProcessor@@K@Z ENDP ; CEnBitmap::ProcessImage
PUBLIC	??_H@YGXPAXIHP6EX0@Z@Z				; `vector constructor iterator'
PUBLIC	?GetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QBEHXZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::GetSize
PUBLIC	?GetDIBits32@CEnBitmap@@IAEPAVRGBX@@XZ		; CEnBitmap::GetDIBits32
PUBLIC	?PrepareBitmapInfo32@CEnBitmap@@IAEHAAUtagBITMAPINFO@@PAUHBITMAP__@@@Z ; CEnBitmap::PrepareBitmapInfo32
PUBLIC	?Fill@CEnBitmap@@KAHPAVRGBX@@VCSize@@K@Z	; CEnBitmap::Fill
PUBLIC	??A?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEAAPAVC32BitImageProcessor@@H@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::operator[]
PUBLIC	??0RGBX@@QAE@XZ					; RGBX::RGBX
PUBLIC	?VerifyDeleteObject@GraphicsMisc@@YAHPAX@Z	; GraphicsMisc::VerifyDeleteObject
PUBLIC	?VerifyDeleteObject@GraphicsMisc@@YAHAAVCGdiObject@@@Z ; GraphicsMisc::VerifyDeleteObject
EXTRN	?GetBitmap@CBitmap@@QAEHPAUtagBITMAP@@@Z:NEAR	; CBitmap::GetBitmap
EXTRN	__imp__SetDIBits@28:NEAR
EXTRN	??3@YAXPAXPBDH@Z:NEAR				; operator delete
;	COMDAT xdata$x
; File D:\_CODE\Shared\EnBitmap.cpp
xdata$x	SEGMENT
$T77223	DD	019930520H
	DD	02H
	DD	FLAT:$T77225
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77225	DD	0ffffffffH
	DD	FLAT:$L77220
	DD	0ffffffffH
	DD	FLAT:$L77221
xdata$x	ENDS
;	COMDAT ?ProcessImage@CEnBitmap@@QAEHAAV?$CArray@PAVC32BitImageProcessor@@PAV1@@@K@Z
_TEXT	SEGMENT
_aProcessors$ = 8
_crMask$ = 12
_this$ = -196
_nProcessor$ = -16
_nCount$ = -72
_BM$ = -68
_sizeSrc$ = -80
_sizeDest$ = -44
_sizeMax$ = -36
$T77202 = -148
_pSrcPixels$ = -20
$T77203 = -156
_pDestPixels$ = -28
_$S284$ = -24
$T77208 = -160
$T77209 = -164
$T77212 = -168
$T77213 = -172
$T77214 = -176
_bRes$ = -84
$T77217 = -184
$T77218 = -188
$T77219 = -192
__$EHRec$ = -12
_$S285$76199 = -88
_hdc$76208 = -92
_hbmSrc$76209 = -96
_bi$76211 = -140
?ProcessImage@CEnBitmap@@QAEHAAV?$CArray@PAVC32BitImageProcessor@@PAV1@@@K@Z PROC NEAR ; CEnBitmap::ProcessImage, COMDAT

; 516  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77224
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 212				; 000000d4H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-224]
	mov	ecx, 53					; 00000035H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L76160:

; 517  : 	ASSERT (GetSafeHandle());

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHandle@CGdiObject@@QBEPAXXZ	; CGdiObject::GetSafeHandle
	test	eax, eax
	jne	SHORT $L76163
	push	517					; 00000205H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76163
	int	3
$L76163:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76160

; 518  : 
; 519  : 	if (!GetSafeHandle())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHandle@CGdiObject@@QBEPAXXZ	; CGdiObject::GetSafeHandle
	test	eax, eax
	jne	SHORT $L76164

; 520  : 		return FALSE;

	xor	eax, eax
	jmp	$L76159
$L76164:

; 521  : 
; 522  : 	if (!aProcessors.GetSize())

	mov	ecx, DWORD PTR _aProcessors$[ebp]
	call	?GetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QBEHXZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::GetSize
	test	eax, eax
	jne	SHORT $L76165

; 523  : 		return TRUE;

	mov	eax, 1
	jmp	$L76159
$L76165:

; 524  : 
; 525  : 	int nProcessor, nCount = aProcessors.GetSize();

	mov	ecx, DWORD PTR _aProcessors$[ebp]
	call	?GetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QBEHXZ ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::GetSize
	mov	DWORD PTR _nCount$[ebp], eax

; 526  : 
; 527  : 	// retrieve src and final dest sizes
; 528  : 	BITMAP BM;
; 529  : 
; 530  : 	if (!GetBitmap(&BM))

	lea	ecx, DWORD PTR _BM$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetBitmap@CBitmap@@QAEHPAUtagBITMAP@@@Z ; CBitmap::GetBitmap
	test	eax, eax
	jne	SHORT $L76169

; 531  : 		return FALSE;

	xor	eax, eax
	jmp	$L76159
$L76169:

; 532  : 
; 533  : 	CSize sizeSrc(BM.bmWidth, BM.bmHeight);

	mov	edx, DWORD PTR _BM$[ebp+8]
	push	edx
	mov	eax, DWORD PTR _BM$[ebp+4]
	push	eax
	lea	ecx, DWORD PTR _sizeSrc$[ebp]
	call	??0CSize@@QAE@HH@Z			; CSize::CSize

; 534  : 	CSize sizeDest(sizeSrc), sizeMax(sizeSrc);

	mov	ecx, DWORD PTR _sizeSrc$[ebp]
	mov	DWORD PTR _sizeDest$[ebp], ecx
	mov	edx, DWORD PTR _sizeSrc$[ebp+4]
	mov	DWORD PTR _sizeDest$[ebp+4], edx
	mov	eax, DWORD PTR _sizeSrc$[ebp]
	mov	DWORD PTR _sizeMax$[ebp], eax
	mov	ecx, DWORD PTR _sizeSrc$[ebp+4]
	mov	DWORD PTR _sizeMax$[ebp+4], ecx

; 535  : 
; 536  : 	for (nProcessor = 0; nProcessor < nCount; nProcessor++)

	mov	DWORD PTR _nProcessor$[ebp], 0
	jmp	SHORT $L76173
$L76174:
	mov	edx, DWORD PTR _nProcessor$[ebp]
	add	edx, 1
	mov	DWORD PTR _nProcessor$[ebp], edx
$L76173:
	mov	eax, DWORD PTR _nProcessor$[ebp]
	cmp	eax, DWORD PTR _nCount$[ebp]
	jge	$L76175

; 538  : 		sizeDest = aProcessors[nProcessor]->CalcDestSize(sizeDest);

	mov	esi, esp
	mov	ecx, DWORD PTR _sizeDest$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _sizeDest$[ebp]
	push	edx
	lea	eax, DWORD PTR $T77202[ebp]
	push	eax
	mov	ecx, DWORD PTR _nProcessor$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _aProcessors$[ebp]
	call	??A?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEAAPAVC32BitImageProcessor@@H@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::operator[]
	mov	edx, DWORD PTR [eax]
	mov	DWORD PTR -200+[ebp], edx
	mov	eax, DWORD PTR -200+[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR -200+[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR _sizeDest$[ebp], ecx
	mov	DWORD PTR _sizeDest$[ebp+4], edx

; 539  : 		sizeMax = CSize(max(sizeMax.cx, sizeDest.cx), max(sizeMax.cy, sizeDest.cy));

	mov	eax, DWORD PTR _sizeMax$[ebp+4]
	cmp	eax, DWORD PTR _sizeDest$[ebp+4]
	jle	SHORT $L77204
	mov	ecx, DWORD PTR _sizeMax$[ebp+4]
	mov	DWORD PTR -204+[ebp], ecx
	jmp	SHORT $L77205
$L77204:
	mov	edx, DWORD PTR _sizeDest$[ebp+4]
	mov	DWORD PTR -204+[ebp], edx
$L77205:
	mov	eax, DWORD PTR _sizeMax$[ebp]
	cmp	eax, DWORD PTR _sizeDest$[ebp]
	jle	SHORT $L77206
	mov	ecx, DWORD PTR _sizeMax$[ebp]
	mov	DWORD PTR -208+[ebp], ecx
	jmp	SHORT $L77207
$L77206:
	mov	edx, DWORD PTR _sizeDest$[ebp]
	mov	DWORD PTR -208+[ebp], edx
$L77207:
	mov	eax, DWORD PTR -204+[ebp]
	push	eax
	mov	ecx, DWORD PTR -208+[ebp]
	push	ecx
	lea	ecx, DWORD PTR $T77203[ebp]
	call	??0CSize@@QAE@HH@Z			; CSize::CSize
	mov	edx, DWORD PTR [eax]
	mov	eax, DWORD PTR [eax+4]
	mov	DWORD PTR _sizeMax$[ebp], edx
	mov	DWORD PTR _sizeMax$[ebp+4], eax

; 540  : 	}

	jmp	$L76174
$L76175:

; 541  : 
; 542  : 	// prepare src and dest bits
; 543  : 	RGBX* pSrcPixels = GetDIBits32();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetDIBits32@CEnBitmap@@IAEPAVRGBX@@XZ	; CEnBitmap::GetDIBits32
	mov	DWORD PTR _pSrcPixels$[ebp], eax

; 544  : 
; 545  : 	if (!pSrcPixels)

	cmp	DWORD PTR _pSrcPixels$[ebp], 0
	jne	SHORT $L76179

; 546  : 		return FALSE;

	xor	eax, eax
	jmp	$L76159
$L76179:

; 547  : 
; 548  : 	RGBX* pDestPixels = new RGBX[sizeMax.cx * sizeMax.cy];

	mov	ecx, DWORD PTR _sizeMax$[ebp]
	imul	ecx, DWORD PTR _sizeMax$[ebp+4]
	mov	DWORD PTR _$S284$[ebp], ecx
	push	548					; 00000224H
	push	OFFSET FLAT:_THIS_FILE
	mov	edx, DWORD PTR _$S284$[ebp]
	shl	edx, 2
	push	edx
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77209[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	cmp	DWORD PTR $T77209[ebp], 0
	je	SHORT $L77210
	push	OFFSET FLAT:??0RGBX@@QAE@XZ		; RGBX::RGBX
	mov	eax, DWORD PTR _$S284$[ebp]
	push	eax
	push	4
	mov	ecx, DWORD PTR $T77209[ebp]
	push	ecx
	call	??_H@YGXPAXIHP6EX0@Z@Z			; `vector constructor iterator'
	mov	edx, DWORD PTR $T77209[ebp]
	mov	DWORD PTR -212+[ebp], edx
	jmp	SHORT $L77211
$L77210:
	mov	DWORD PTR -212+[ebp], 0
$L77211:
	mov	eax, DWORD PTR -212+[ebp]
	mov	DWORD PTR $T77208[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR $T77208[ebp]
	mov	DWORD PTR _pDestPixels$[ebp], ecx

; 549  : 
; 550  : 	if (!pDestPixels)

	cmp	DWORD PTR _pDestPixels$[ebp], 0
	jne	SHORT $L76189

; 551  : 		return FALSE;

	xor	eax, eax
	jmp	$L76159
$L76189:

; 552  : 
; 553  : 	Fill(pDestPixels, sizeMax, m_crBkgnd);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	push	eax
	mov	ecx, DWORD PTR _sizeMax$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _sizeMax$[ebp]
	push	edx
	mov	eax, DWORD PTR _pDestPixels$[ebp]
	push	eax
	call	?Fill@CEnBitmap@@KAHPAVRGBX@@VCSize@@K@Z ; CEnBitmap::Fill
	add	esp, 16					; 00000010H

; 554  : 
; 555  : 	BOOL bRes = TRUE;

	mov	DWORD PTR _bRes$[ebp], 1

; 556  : 	sizeDest = sizeSrc;

	mov	ecx, DWORD PTR _sizeSrc$[ebp]
	mov	DWORD PTR _sizeDest$[ebp], ecx
	mov	edx, DWORD PTR _sizeSrc$[ebp+4]
	mov	DWORD PTR _sizeDest$[ebp+4], edx

; 557  : 
; 558  : 	// do the processing
; 559  : 	for (nProcessor = 0; bRes && nProcessor < nCount; nProcessor++)

	mov	DWORD PTR _nProcessor$[ebp], 0
	jmp	SHORT $L76191
$L76192:
	mov	eax, DWORD PTR _nProcessor$[ebp]
	add	eax, 1
	mov	DWORD PTR _nProcessor$[ebp], eax
$L76191:
	cmp	DWORD PTR _bRes$[ebp], 0
	je	$L76193
	mov	ecx, DWORD PTR _nProcessor$[ebp]
	cmp	ecx, DWORD PTR _nCount$[ebp]
	jge	$L76193

; 561  : 		// if its the second processor or later then we need to copy
; 562  : 		// the previous dest bits back into source.
; 563  : 		// we also need to check that sizeSrc is big enough
; 564  : 		if (nProcessor > 0)

	cmp	DWORD PTR _nProcessor$[ebp], 0
	jle	$L76194

; 566  : 			if (sizeSrc.cx < sizeDest.cx || sizeSrc.cy < sizeDest.cy)

	mov	edx, DWORD PTR _sizeSrc$[ebp]
	cmp	edx, DWORD PTR _sizeDest$[ebp]
	jl	SHORT $L76196
	mov	eax, DWORD PTR _sizeSrc$[ebp+4]
	cmp	eax, DWORD PTR _sizeDest$[ebp+4]
	jge	$L76195
$L76196:

; 568  : 				delete [] pSrcPixels;

	mov	ecx, DWORD PTR _pSrcPixels$[ebp]
	mov	DWORD PTR $T77212[ebp], ecx
	mov	edx, DWORD PTR $T77212[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 569  : 				pSrcPixels = new RGBX[sizeDest.cx * sizeDest.cy];

	mov	eax, DWORD PTR _sizeDest$[ebp]
	imul	eax, DWORD PTR _sizeDest$[ebp+4]
	mov	DWORD PTR _$S285$76199[ebp], eax
	push	569					; 00000239H
	push	OFFSET FLAT:_THIS_FILE
	mov	ecx, DWORD PTR _$S285$76199[ebp]
	shl	ecx, 2
	push	ecx
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77214[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	cmp	DWORD PTR $T77214[ebp], 0
	je	SHORT $L77215
	push	OFFSET FLAT:??0RGBX@@QAE@XZ		; RGBX::RGBX
	mov	edx, DWORD PTR _$S285$76199[ebp]
	push	edx
	push	4
	mov	eax, DWORD PTR $T77214[ebp]
	push	eax
	call	??_H@YGXPAXIHP6EX0@Z@Z			; `vector constructor iterator'
	mov	ecx, DWORD PTR $T77214[ebp]
	mov	DWORD PTR -216+[ebp], ecx
	jmp	SHORT $L77216
$L77215:
	mov	DWORD PTR -216+[ebp], 0
$L77216:
	mov	edx, DWORD PTR -216+[ebp]
	mov	DWORD PTR $T77213[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR $T77213[ebp]
	mov	DWORD PTR _pSrcPixels$[ebp], eax
$L76195:

; 571  : 
; 572  : 			CopyMemory(pSrcPixels, pDestPixels, sizeDest.cx * 4 * sizeDest.cy); // default

	mov	ecx, DWORD PTR _sizeDest$[ebp]
	shl	ecx, 2
	imul	ecx, DWORD PTR _sizeDest$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	push	edx
	mov	eax, DWORD PTR _pSrcPixels$[ebp]
	push	eax
	call	_memcpy
	add	esp, 12					; 0000000cH

; 573  : 			Fill(pDestPixels, sizeDest, m_crBkgnd);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	mov	eax, DWORD PTR _sizeDest$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sizeDest$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pDestPixels$[ebp]
	push	edx
	call	?Fill@CEnBitmap@@KAHPAVRGBX@@VCSize@@K@Z ; CEnBitmap::Fill
	add	esp, 16					; 00000010H
$L76194:

; 575  : 
; 576  : 		sizeSrc = sizeDest;

	mov	eax, DWORD PTR _sizeDest$[ebp]
	mov	DWORD PTR _sizeSrc$[ebp], eax
	mov	ecx, DWORD PTR _sizeDest$[ebp+4]
	mov	DWORD PTR _sizeSrc$[ebp+4], ecx

; 577  : 		sizeDest = aProcessors[nProcessor]->CalcDestSize(sizeSrc);

	mov	esi, esp
	mov	edx, DWORD PTR _sizeSrc$[ebp+4]
	push	edx
	mov	eax, DWORD PTR _sizeSrc$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T77217[ebp]
	push	ecx
	mov	edx, DWORD PTR _nProcessor$[ebp]
	push	edx
	mov	ecx, DWORD PTR _aProcessors$[ebp]
	call	??A?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEAAPAVC32BitImageProcessor@@H@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::operator[]
	mov	eax, DWORD PTR [eax]
	mov	DWORD PTR -220+[ebp], eax
	mov	ecx, DWORD PTR -220+[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR -220+[ebp]
	call	DWORD PTR [edx+4]
	cmp	esi, esp
	call	__chkesp
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR _sizeDest$[ebp], ecx
	mov	DWORD PTR _sizeDest$[ebp+4], edx

; 578  : 		
; 579  : 		bRes = aProcessors[nProcessor]->ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest, crMask);

	mov	esi, esp
	mov	eax, DWORD PTR _crMask$[ebp]
	push	eax
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
	mov	ecx, DWORD PTR _nProcessor$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _aProcessors$[ebp]
	call	??A?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEAAPAVC32BitImageProcessor@@H@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::operator[]
	mov	edx, DWORD PTR [eax]
	mov	DWORD PTR -224+[ebp], edx
	mov	eax, DWORD PTR -224+[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR -224+[ebp]
	call	DWORD PTR [edx+8]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bRes$[ebp], eax

; 580  : 	}

	jmp	$L76192
$L76193:

; 581  : 
; 582  : 	// update the bitmap
; 583  : 	if (bRes)

	cmp	DWORD PTR _bRes$[ebp], 0
	je	$L76214

; 585  : 		// set the bits
; 586  : 		HDC hdc = GetDC(NULL);

	mov	esi, esp
	push	0
	call	DWORD PTR __imp__GetDC@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hdc$76208[ebp], eax

; 587  : 		HBITMAP hbmSrc = ::CreateCompatibleBitmap(hdc, sizeDest.cx, sizeDest.cy);

	mov	esi, esp
	mov	eax, DWORD PTR _sizeDest$[ebp+4]
	push	eax
	mov	ecx, DWORD PTR _sizeDest$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hdc$76208[ebp]
	push	edx
	call	DWORD PTR __imp__CreateCompatibleBitmap@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hbmSrc$76209[ebp], eax

; 588  : 
; 589  : 		if (hbmSrc)

	cmp	DWORD PTR _hbmSrc$76209[ebp], 0
	je	$L76214

; 591  : 			BITMAPINFO bi;
; 592  : 
; 593  : 			if (PrepareBitmapInfo32(bi, hbmSrc))

	mov	eax, DWORD PTR _hbmSrc$76209[ebp]
	push	eax
	lea	ecx, DWORD PTR _bi$76211[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?PrepareBitmapInfo32@CEnBitmap@@IAEHAAUtagBITMAPINFO@@PAUHBITMAP__@@@Z ; CEnBitmap::PrepareBitmapInfo32
	test	eax, eax
	je	SHORT $L76213

; 595  : 				if (SetDIBits(hdc, hbmSrc, 0, sizeDest.cy, pDestPixels, &bi, DIB_RGB_COLORS))

	mov	esi, esp
	push	0
	lea	edx, DWORD PTR _bi$76211[ebp]
	push	edx
	mov	eax, DWORD PTR _pDestPixels$[ebp]
	push	eax
	mov	ecx, DWORD PTR _sizeDest$[ebp+4]
	push	ecx
	push	0
	mov	edx, DWORD PTR _hbmSrc$76209[ebp]
	push	edx
	mov	eax, DWORD PTR _hdc$76208[ebp]
	push	eax
	call	DWORD PTR __imp__SetDIBits@28
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L76213

; 597  : 					// delete the bitmap and attach new
; 598  : 					GraphicsMisc::VerifyDeleteObject(*this);

	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	?VerifyDeleteObject@GraphicsMisc@@YAHAAVCGdiObject@@@Z ; GraphicsMisc::VerifyDeleteObject
	add	esp, 4

; 599  : 					bRes = Attach(hbmSrc);

	mov	edx, DWORD PTR _hbmSrc$76209[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Attach@CGdiObject@@QAEHPAX@Z		; CGdiObject::Attach
	mov	DWORD PTR _bRes$[ebp], eax
$L76213:

; 602  : 
; 603  : 			::ReleaseDC(NULL, hdc);

	mov	esi, esp
	mov	eax, DWORD PTR _hdc$76208[ebp]
	push	eax
	push	0
	call	DWORD PTR __imp__ReleaseDC@8
	cmp	esi, esp
	call	__chkesp

; 604  : 
; 605  : 			if (!bRes)

	cmp	DWORD PTR _bRes$[ebp], 0
	jne	SHORT $L76214

; 606  : 				GraphicsMisc::VerifyDeleteObject(hbmSrc);

	mov	ecx, DWORD PTR _hbmSrc$76209[ebp]
	push	ecx
	call	?VerifyDeleteObject@GraphicsMisc@@YAHPAX@Z ; GraphicsMisc::VerifyDeleteObject
	add	esp, 4
$L76214:

; 609  : 
; 610  : 	delete [] pSrcPixels;

	mov	edx, DWORD PTR _pSrcPixels$[ebp]
	mov	DWORD PTR $T77218[ebp], edx
	mov	eax, DWORD PTR $T77218[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 611  : 	delete [] pDestPixels;

	mov	ecx, DWORD PTR _pDestPixels$[ebp]
	mov	DWORD PTR $T77219[ebp], ecx
	mov	edx, DWORD PTR $T77219[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 612  : 
; 613  : 	return bRes;

	mov	eax, DWORD PTR _bRes$[ebp]
$L76159:

; 614  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 224				; 000000e0H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77220:
	push	548					; 00000224H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR $T77209[ebp]
	push	eax
	call	??3@YAXPAXPBDH@Z			; operator delete
	add	esp, 12					; 0000000cH
	ret	0
$L77221:
	push	569					; 00000239H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR $T77214[ebp]
	push	eax
	call	??3@YAXPAXPBDH@Z			; operator delete
	add	esp, 12					; 0000000cH
	ret	0
$L77224:
	mov	eax, OFFSET FLAT:$T77223
	jmp	___CxxFrameHandler
text$x	ENDS
?ProcessImage@CEnBitmap@@QAEHAAV?$CArray@PAVC32BitImageProcessor@@PAV1@@@K@Z ENDP ; CEnBitmap::ProcessImage
;	COMDAT ??0RGBX@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0RGBX@@QAE@XZ PROC NEAR				; RGBX::RGBX, COMDAT

; 38   : 	RGBX() { btRed = btBlue = btGreen = btUnused = 0; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	BYTE PTR [eax+3], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [ecx+1], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [edx], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	BYTE PTR [eax+2], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	0
??0RGBX@@QAE@XZ ENDP					; RGBX::RGBX
_TEXT	ENDS
PUBLIC	??_C@_0BP@IHHM@D?3?2_CODE?2Shared?2graphicsMisc?4h?$AA@ ; `string'
;	COMDAT ??_C@_0BP@IHHM@D?3?2_CODE?2Shared?2graphicsMisc?4h?$AA@
; File D:\_CODE\Shared\graphicsMisc.h
_DATA	SEGMENT
??_C@_0BP@IHHM@D?3?2_CODE?2Shared?2graphicsMisc?4h?$AA@ DB 'D:\_CODE\Shar'
	DB	'ed\graphicsMisc.h', 00H			; `string'
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
	jne	SHORT $L74250

; 89   : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L74249
$L74250:

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
	je	SHORT $L74251

; 93   : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L74249
$L74251:

; 94   : 
; 95   : 		// else
; 96   : 		ASSERT(0);

	mov	ecx, 1
	test	ecx, ecx
	je	SHORT $L74255
	push	96					; 00000060H
	push	OFFSET FLAT:??_C@_0BP@IHHM@D?3?2_CODE?2Shared?2graphicsMisc?4h?$AA@ ; `string'
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74255
	int	3
$L74255:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74251

; 97   : 		return FALSE;

	xor	eax, eax
$L74249:

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
	jne	SHORT $L74261

; 103  : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L74260
$L74261:

; 104  : 
; 105  : 		// else
; 106  : 		if (obj.DeleteObject())

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?DeleteObject@CGdiObject@@QAEHXZ	; CGdiObject::DeleteObject
	test	eax, eax
	je	SHORT $L74262

; 107  : 			return TRUE;

	mov	eax, 1
	jmp	SHORT $L74260
$L74262:

; 108  : 
; 109  : 		// else
; 110  : 		ASSERT(0);

	mov	ecx, 1
	test	ecx, ecx
	je	SHORT $L74266
	push	110					; 0000006eH
	push	OFFSET FLAT:??_C@_0BP@IHHM@D?3?2_CODE?2Shared?2graphicsMisc?4h?$AA@ ; `string'
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L74266
	int	3
$L74266:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L74262

; 111  : 		return FALSE;

	xor	eax, eax
$L74260:

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
;	COMDAT ??_H@YGXPAXIHP6EX0@Z@Z
_TEXT	SEGMENT
___t$ = 8
___s$ = 12
___n$ = 16
___f$ = 20
??_H@YGXPAXIHP6EX0@Z@Z PROC NEAR			; `vector constructor iterator', COMDAT
	push	ebp
	mov	ebp, esp
	push	esi
$L76225:
	mov	eax, DWORD PTR ___n$[ebp]
	sub	eax, 1
	mov	DWORD PTR ___n$[ebp], eax
	cmp	DWORD PTR ___n$[ebp], 0
	jl	SHORT $L76226
	mov	esi, esp
	mov	ecx, DWORD PTR ___t$[ebp]
	call	DWORD PTR ___f$[ebp]
	cmp	esi, esp
	call	__chkesp
	mov	ecx, DWORD PTR ___t$[ebp]
	add	ecx, DWORD PTR ___s$[ebp]
	mov	DWORD PTR ___t$[ebp], ecx
	jmp	SHORT $L76225
$L76226:
	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	16					; 00000010H
??_H@YGXPAXIHP6EX0@Z@Z ENDP				; `vector constructor iterator'
_TEXT	ENDS
EXTRN	__imp__GetDIBits@28:NEAR
;	COMDAT ?GetDIBits32@CEnBitmap@@IAEPAVRGBX@@XZ
_TEXT	SEGMENT
$T77238 = -60
$T77239 = -64
_this$ = -68
_bi$ = -48
_nHeight$ = -52
_pBits$ = -4
_hdc$ = -56
?GetDIBits32@CEnBitmap@@IAEPAVRGBX@@XZ PROC NEAR	; CEnBitmap::GetDIBits32, COMDAT

; 617  : {

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

; 618  : 	BITMAPINFO bi;
; 619  : 
; 620  : 	int nHeight = PrepareBitmapInfo32(bi);

	push	0
	lea	eax, DWORD PTR _bi$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?PrepareBitmapInfo32@CEnBitmap@@IAEHAAUtagBITMAPINFO@@PAUHBITMAP__@@@Z ; CEnBitmap::PrepareBitmapInfo32
	mov	DWORD PTR _nHeight$[ebp], eax

; 621  : 	
; 622  : 	if (!nHeight)

	cmp	DWORD PTR _nHeight$[ebp], 0
	jne	SHORT $L76233

; 623  : 		return FALSE;

	xor	eax, eax
	jmp	$L76230
$L76233:

; 624  : 
; 625  : 	BYTE* pBits = (BYTE*)new BYTE[bi.bmiHeader.biSizeImage];

	push	625					; 00000271H
	push	OFFSET FLAT:_THIS_FILE
	mov	ecx, DWORD PTR _bi$[ebp+20]
	push	ecx
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77238[ebp], eax
	mov	edx, DWORD PTR $T77238[ebp]
	mov	DWORD PTR _pBits$[ebp], edx

; 626  : 	HDC hdc = GetDC(NULL);

	mov	esi, esp
	push	0
	call	DWORD PTR __imp__GetDC@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hdc$[ebp], eax

; 627  : 
; 628  : 	if (!GetDIBits(hdc, (HBITMAP)GetSafeHandle(), 0, nHeight, pBits, &bi, DIB_RGB_COLORS))

	mov	esi, esp
	push	0
	lea	eax, DWORD PTR _bi$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pBits$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nHeight$[ebp]
	push	edx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHandle@CGdiObject@@QBEPAXXZ	; CGdiObject::GetSafeHandle
	push	eax
	mov	eax, DWORD PTR _hdc$[ebp]
	push	eax
	call	DWORD PTR __imp__GetDIBits@28
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76240

; 630  : 		delete [] pBits;

	mov	ecx, DWORD PTR _pBits$[ebp]
	mov	DWORD PTR $T77239[ebp], ecx
	mov	edx, DWORD PTR $T77239[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 631  : 		pBits = NULL;

	mov	DWORD PTR _pBits$[ebp], 0
$L76240:

; 633  : 
; 634  : 	::ReleaseDC(NULL, hdc);

	mov	esi, esp
	mov	eax, DWORD PTR _hdc$[ebp]
	push	eax
	push	0
	call	DWORD PTR __imp__ReleaseDC@8
	cmp	esi, esp
	call	__chkesp

; 635  : 
; 636  : 	return (RGBX*)pBits;

	mov	eax, DWORD PTR _pBits$[ebp]
$L76230:

; 637  : }

	pop	edi
	pop	esi
	add	esp, 68					; 00000044H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetDIBits32@CEnBitmap@@IAEPAVRGBX@@XZ ENDP		; CEnBitmap::GetDIBits32
_TEXT	ENDS
;	COMDAT ?PrepareBitmapInfo32@CEnBitmap@@IAEHAAUtagBITMAPINFO@@PAUHBITMAP__@@@Z
_TEXT	SEGMENT
_bi$ = 8
_hBitmap$ = 12
_this$ = -28
_BM$ = -24
?PrepareBitmapInfo32@CEnBitmap@@IAEHAAUtagBITMAPINFO@@PAUHBITMAP__@@@Z PROC NEAR ; CEnBitmap::PrepareBitmapInfo32, COMDAT

; 640  : {

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

; 641  : 	if (!hBitmap)

	cmp	DWORD PTR _hBitmap$[ebp], 0
	jne	SHORT $L76248

; 642  : 		hBitmap = (HBITMAP)GetSafeHandle();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHandle@CGdiObject@@QBEPAXXZ	; CGdiObject::GetSafeHandle
	mov	DWORD PTR _hBitmap$[ebp], eax
$L76248:

; 645  : 
; 646  : 	if (!::GetObject(hBitmap, sizeof(BM), &BM))

	mov	esi, esp
	lea	eax, DWORD PTR _BM$[ebp]
	push	eax
	push	24					; 00000018H
	mov	ecx, DWORD PTR _hBitmap$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetObjectW@12
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76251

; 647  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L76247
$L76251:

; 648  : 
; 649  : 	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);

	mov	edx, DWORD PTR _bi$[ebp]
	mov	DWORD PTR [edx], 40			; 00000028H

; 650  : 	bi.bmiHeader.biWidth = BM.bmWidth;

	mov	eax, DWORD PTR _bi$[ebp]
	mov	ecx, DWORD PTR _BM$[ebp+4]
	mov	DWORD PTR [eax+4], ecx

; 651  : 	bi.bmiHeader.biHeight = -BM.bmHeight;

	mov	edx, DWORD PTR _BM$[ebp+8]
	neg	edx
	mov	eax, DWORD PTR _bi$[ebp]
	mov	DWORD PTR [eax+8], edx

; 652  : 	bi.bmiHeader.biPlanes = 1;

	mov	ecx, DWORD PTR _bi$[ebp]
	mov	WORD PTR [ecx+12], 1

; 653  : 	bi.bmiHeader.biBitCount = 32; // 32 bit

	mov	edx, DWORD PTR _bi$[ebp]
	mov	WORD PTR [edx+14], 32			; 00000020H

; 654  : 	bi.bmiHeader.biCompression = BI_RGB; // 32 bit

	mov	eax, DWORD PTR _bi$[ebp]
	mov	DWORD PTR [eax+16], 0

; 655  : 	bi.bmiHeader.biSizeImage = BM.bmWidth * 4 * BM.bmHeight; // 32 bit

	mov	ecx, DWORD PTR _BM$[ebp+4]
	shl	ecx, 2
	imul	ecx, DWORD PTR _BM$[ebp+8]
	mov	edx, DWORD PTR _bi$[ebp]
	mov	DWORD PTR [edx+20], ecx

; 656  : 	bi.bmiHeader.biClrUsed = 0;

	mov	eax, DWORD PTR _bi$[ebp]
	mov	DWORD PTR [eax+32], 0

; 657  : 	bi.bmiHeader.biClrImportant = 0;

	mov	ecx, DWORD PTR _bi$[ebp]
	mov	DWORD PTR [ecx+36], 0

; 658  : 
; 659  : 	return BM.bmHeight;

	mov	eax, DWORD PTR _BM$[ebp+8]
$L76247:

; 660  : }

	pop	esi
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?PrepareBitmapInfo32@CEnBitmap@@IAEHAAUtagBITMAPINFO@@PAUHBITMAP__@@@Z ENDP ; CEnBitmap::PrepareBitmapInfo32
_TEXT	ENDS
PUBLIC	?CopyImage@CEnBitmap@@QAEHPAUHBITMAP__@@@Z	; CEnBitmap::CopyImage
;	COMDAT ?CopyImage@CEnBitmap@@QAEHPAUHBITMAP__@@@Z
_TEXT	SEGMENT
$T77244 = -72
$T77245 = -76
_hBitmap$ = 8
_this$ = -80
_bi$ = -48
_nHeight$ = -52
_pBits$ = -4
_hdc$ = -56
_bRes$ = -60
_nWidth$76271 = -68
_hbmDest$76272 = -64
?CopyImage@CEnBitmap@@QAEHPAUHBITMAP__@@@Z PROC NEAR	; CEnBitmap::CopyImage, COMDAT

; 663  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 80					; 00000050H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-80]
	mov	ecx, 20					; 00000014H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L76256:

; 664  : 	ASSERT (hBitmap);

	cmp	DWORD PTR _hBitmap$[ebp], 0
	jne	SHORT $L76259
	push	664					; 00000298H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76259
	int	3
$L76259:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76256

; 665  : 	
; 666  : 	if (!hBitmap)

	cmp	DWORD PTR _hBitmap$[ebp], 0
	jne	SHORT $L76260

; 667  : 		return FALSE;

	xor	eax, eax
	jmp	$L76255
$L76260:

; 670  : 	int nHeight = PrepareBitmapInfo32(bi, hBitmap);

	mov	ecx, DWORD PTR _hBitmap$[ebp]
	push	ecx
	lea	edx, DWORD PTR _bi$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?PrepareBitmapInfo32@CEnBitmap@@IAEHAAUtagBITMAPINFO@@PAUHBITMAP__@@@Z ; CEnBitmap::PrepareBitmapInfo32
	mov	DWORD PTR _nHeight$[ebp], eax

; 671  : 
; 672  : 	if (!nHeight)

	cmp	DWORD PTR _nHeight$[ebp], 0
	jne	SHORT $L76263

; 673  : 		return FALSE;

	xor	eax, eax
	jmp	$L76255
$L76263:

; 674  : 
; 675  : 	BYTE* pBits = (BYTE*)new BYTE[bi.bmiHeader.biSizeImage];

	push	675					; 000002a3H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _bi$[ebp+20]
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77244[ebp], eax
	mov	ecx, DWORD PTR $T77244[ebp]
	mov	DWORD PTR _pBits$[ebp], ecx

; 676  : 	HDC hdc = GetDC(NULL);

	mov	esi, esp
	push	0
	call	DWORD PTR __imp__GetDC@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hdc$[ebp], eax

; 677  : 	BOOL bRes = FALSE;

	mov	DWORD PTR _bRes$[ebp], 0

; 678  : 
; 679  : 	if (GetDIBits(hdc, hBitmap, 0, nHeight, pBits, &bi, DIB_RGB_COLORS))

	mov	esi, esp
	push	0
	lea	edx, DWORD PTR _bi$[ebp]
	push	edx
	mov	eax, DWORD PTR _pBits$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nHeight$[ebp]
	push	ecx
	push	0
	mov	edx, DWORD PTR _hBitmap$[ebp]
	push	edx
	mov	eax, DWORD PTR _hdc$[ebp]
	push	eax
	call	DWORD PTR __imp__GetDIBits@28
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L76274

; 681  : 		int nWidth = bi.bmiHeader.biSizeImage / (nHeight * 4);

	mov	ecx, DWORD PTR _nHeight$[ebp]
	shl	ecx, 2
	mov	eax, DWORD PTR _bi$[ebp+20]
	xor	edx, edx
	div	ecx
	mov	DWORD PTR _nWidth$76271[ebp], eax

; 682  : 
; 683  : 		HBITMAP hbmDest = ::CreateCompatibleBitmap(hdc, nWidth, nHeight);

	mov	esi, esp
	mov	edx, DWORD PTR _nHeight$[ebp]
	push	edx
	mov	eax, DWORD PTR _nWidth$76271[ebp]
	push	eax
	mov	ecx, DWORD PTR _hdc$[ebp]
	push	ecx
	call	DWORD PTR __imp__CreateCompatibleBitmap@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hbmDest$76272[ebp], eax

; 684  : 
; 685  : 		if (hbmDest)

	cmp	DWORD PTR _hbmDest$76272[ebp], 0
	je	SHORT $L76274

; 687  : 			if (SetDIBits(hdc, hbmDest, 0, nHeight, pBits, &bi, DIB_RGB_COLORS))

	mov	esi, esp
	push	0
	lea	edx, DWORD PTR _bi$[ebp]
	push	edx
	mov	eax, DWORD PTR _pBits$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nHeight$[ebp]
	push	ecx
	push	0
	mov	edx, DWORD PTR _hbmDest$76272[ebp]
	push	edx
	mov	eax, DWORD PTR _hdc$[ebp]
	push	eax
	call	DWORD PTR __imp__SetDIBits@28
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L76274

; 689  : 				GraphicsMisc::VerifyDeleteObject(*this);

	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	?VerifyDeleteObject@GraphicsMisc@@YAHAAVCGdiObject@@@Z ; GraphicsMisc::VerifyDeleteObject
	add	esp, 4

; 690  : 				bRes = Attach(hbmDest);

	mov	edx, DWORD PTR _hbmDest$76272[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Attach@CGdiObject@@QAEHPAX@Z		; CGdiObject::Attach
	mov	DWORD PTR _bRes$[ebp], eax
$L76274:

; 694  : 
; 695  : 	::ReleaseDC(NULL, hdc);

	mov	esi, esp
	mov	eax, DWORD PTR _hdc$[ebp]
	push	eax
	push	0
	call	DWORD PTR __imp__ReleaseDC@8
	cmp	esi, esp
	call	__chkesp

; 696  : 	delete [] pBits;

	mov	ecx, DWORD PTR _pBits$[ebp]
	mov	DWORD PTR $T77245[ebp], ecx
	mov	edx, DWORD PTR $T77245[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 697  : 
; 698  : 	return bRes;

	mov	eax, DWORD PTR _bRes$[ebp]
$L76255:

; 699  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 80					; 00000050H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?CopyImage@CEnBitmap@@QAEHPAUHBITMAP__@@@Z ENDP		; CEnBitmap::CopyImage
_TEXT	ENDS
PUBLIC	?CopyImage@CEnBitmap@@QAEHPAVCBitmap@@@Z	; CEnBitmap::CopyImage
;	COMDAT ?CopyImage@CEnBitmap@@QAEHPAVCBitmap@@@Z
_TEXT	SEGMENT
_pBitmap$ = 8
_this$ = -4
?CopyImage@CEnBitmap@@QAEHPAVCBitmap@@@Z PROC NEAR	; CEnBitmap::CopyImage, COMDAT

; 702  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 703  : 	if (!pBitmap)

	cmp	DWORD PTR _pBitmap$[ebp], 0
	jne	SHORT $L76280

; 704  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L76279
$L76280:

; 705  : 
; 706  : 	return CopyImage((HBITMAP)pBitmap->GetSafeHandle());

	mov	ecx, DWORD PTR _pBitmap$[ebp]
	call	?GetSafeHandle@CGdiObject@@QBEPAXXZ	; CGdiObject::GetSafeHandle
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?CopyImage@CEnBitmap@@QAEHPAUHBITMAP__@@@Z ; CEnBitmap::CopyImage
$L76279:

; 707  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?CopyImage@CEnBitmap@@QAEHPAVCBitmap@@@Z ENDP		; CEnBitmap::CopyImage
_TEXT	ENDS
PUBLIC	??0RGBX@@QAE@ABK@Z				; RGBX::RGBX
EXTRN	_memset:NEAR
;	COMDAT ?Fill@CEnBitmap@@KAHPAVRGBX@@VCSize@@K@Z
_TEXT	SEGMENT
$T77250 = -24
_pPixels$ = 8
_size$ = 12
_color$ = 20
_pLine$76299 = -12
_nSize$76300 = -4
_nAmount$76306 = -16
_nRow$76307 = -8
_nAmount$76312 = -20
?Fill@CEnBitmap@@KAHPAVRGBX@@VCSize@@K@Z PROC NEAR	; CEnBitmap::Fill, COMDAT

; 710  : {

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

; 711  : 	if (!pPixels)

	cmp	DWORD PTR _pPixels$[ebp], 0
	jne	SHORT $L76287

; 712  : 		return FALSE;

	xor	eax, eax
	jmp	$L76286
$L76287:

; 713  : 
; 714  : 	if (color == -1 || color == RGB(255, 255, 255))

	cmp	DWORD PTR _color$[ebp], -1
	je	SHORT $L76295
	cmp	DWORD PTR _color$[ebp], 16777215	; 00ffffffH
	jne	SHORT $L76294
$L76295:

; 715  : 		FillMemory(pPixels, size.cx * 4 * size.cy, 255); // white

	mov	eax, DWORD PTR _size$[ebp]
	shl	eax, 2
	imul	eax, DWORD PTR _size$[ebp+4]
	push	eax
	push	255					; 000000ffH
	mov	ecx, DWORD PTR _pPixels$[ebp]
	push	ecx
	call	_memset
	add	esp, 12					; 0000000cH

; 716  : 
; 717  : 	else if (color == 0)

	jmp	$L76310
$L76294:
	cmp	DWORD PTR _color$[ebp], 0
	jne	SHORT $L76297

; 718  : 		FillMemory(pPixels, size.cx * 4 * size.cy, 0); // black

	mov	edx, DWORD PTR _size$[ebp]
	shl	edx, 2
	imul	edx, DWORD PTR _size$[ebp+4]
	push	edx
	push	0
	mov	eax, DWORD PTR _pPixels$[ebp]
	push	eax
	call	_memset
	add	esp, 12					; 0000000cH

; 719  : 
; 720  : 	else

	jmp	$L76310
$L76297:

; 722  : 		// fill the first line with the color
; 723  : 		RGBX* pLine = &pPixels[0];

	mov	ecx, DWORD PTR _pPixels$[ebp]
	mov	DWORD PTR _pLine$76299[ebp], ecx

; 724  : 		int nSize = 1;

	mov	DWORD PTR _nSize$76300[ebp], 1

; 725  : 
; 726  : 		pLine[0] = RGBX(color);

	lea	edx, DWORD PTR _color$[ebp]
	push	edx
	lea	ecx, DWORD PTR $T77250[ebp]
	call	??0RGBX@@QAE@ABK@Z			; RGBX::RGBX
	push	eax
	mov	ecx, DWORD PTR _pLine$76299[ebp]
	call	??4RGBX@@QAEABV0@ABV0@@Z		; RGBX::operator=
$L76303:

; 727  : 
; 728  : 		while (1)

	mov	eax, 1
	test	eax, eax
	je	SHORT $L76304

; 730  : 			if (nSize > size.cx)

	mov	ecx, DWORD PTR _nSize$76300[ebp]
	cmp	ecx, DWORD PTR _size$[ebp]
	jle	SHORT $L76305

; 731  : 				break;

	jmp	SHORT $L76304
$L76305:

; 732  : 
; 733  : 			// else
; 734  : 			int nAmount = min(size.cx - nSize, nSize) * 4;

	mov	edx, DWORD PTR _size$[ebp]
	sub	edx, DWORD PTR _nSize$76300[ebp]
	cmp	edx, DWORD PTR _nSize$76300[ebp]
	jge	SHORT $L77251
	mov	eax, DWORD PTR _size$[ebp]
	sub	eax, DWORD PTR _nSize$76300[ebp]
	mov	DWORD PTR -28+[ebp], eax
	jmp	SHORT $L77252
$L77251:
	mov	ecx, DWORD PTR _nSize$76300[ebp]
	mov	DWORD PTR -28+[ebp], ecx
$L77252:
	mov	edx, DWORD PTR -28+[ebp]
	shl	edx, 2
	mov	DWORD PTR _nAmount$76306[ebp], edx

; 735  : 
; 736  : 			CopyMemory(&pLine[nSize], pLine, nAmount);

	mov	eax, DWORD PTR _nAmount$76306[ebp]
	push	eax
	mov	ecx, DWORD PTR _pLine$76299[ebp]
	push	ecx
	mov	edx, DWORD PTR _nSize$76300[ebp]
	mov	eax, DWORD PTR _pLine$76299[ebp]
	lea	ecx, DWORD PTR [eax+edx*4]
	push	ecx
	call	_memcpy
	add	esp, 12					; 0000000cH

; 737  : 			nSize *= 2;

	mov	edx, DWORD PTR _nSize$76300[ebp]
	shl	edx, 1
	mov	DWORD PTR _nSize$76300[ebp], edx

; 738  : 		}

	jmp	SHORT $L76303
$L76304:

; 739  : 
; 740  : 		// use that line to fill the rest of the block
; 741  : 		int nRow = 1;

	mov	DWORD PTR _nRow$76307[ebp], 1
$L76309:

; 742  : 
; 743  : 		while (1)

	mov	eax, 1
	test	eax, eax
	je	SHORT $L76310

; 745  : 			if (nRow > size.cy)

	mov	ecx, DWORD PTR _nRow$76307[ebp]
	cmp	ecx, DWORD PTR _size$[ebp+4]
	jle	SHORT $L76311

; 746  : 				break;

	jmp	SHORT $L76310
$L76311:

; 747  : 
; 748  : 			// else
; 749  : 			int nAmount = min(size.cy - nRow, nRow) * size.cx * 4;

	mov	edx, DWORD PTR _size$[ebp+4]
	sub	edx, DWORD PTR _nRow$76307[ebp]
	cmp	edx, DWORD PTR _nRow$76307[ebp]
	jge	SHORT $L77253
	mov	eax, DWORD PTR _size$[ebp+4]
	sub	eax, DWORD PTR _nRow$76307[ebp]
	mov	DWORD PTR -32+[ebp], eax
	jmp	SHORT $L77254
$L77253:
	mov	ecx, DWORD PTR _nRow$76307[ebp]
	mov	DWORD PTR -32+[ebp], ecx
$L77254:
	mov	edx, DWORD PTR -32+[ebp]
	imul	edx, DWORD PTR _size$[ebp]
	shl	edx, 2
	mov	DWORD PTR _nAmount$76312[ebp], edx

; 750  : 
; 751  : 			CopyMemory(&pPixels[nRow * size.cx], pPixels, nAmount);

	mov	eax, DWORD PTR _nAmount$76312[ebp]
	push	eax
	mov	ecx, DWORD PTR _pPixels$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nRow$76307[ebp]
	imul	edx, DWORD PTR _size$[ebp]
	mov	eax, DWORD PTR _pPixels$[ebp]
	lea	ecx, DWORD PTR [eax+edx*4]
	push	ecx
	call	_memcpy
	add	esp, 12					; 0000000cH

; 752  : 			nRow *= 2;

	mov	edx, DWORD PTR _nRow$76307[ebp]
	shl	edx, 1
	mov	DWORD PTR _nRow$76307[ebp], edx

; 753  : 		}

	jmp	SHORT $L76309
$L76310:

; 755  : 
; 756  : 	return TRUE;

	mov	eax, 1
$L76286:

; 757  : }

	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?Fill@CEnBitmap@@KAHPAVRGBX@@VCSize@@K@Z ENDP		; CEnBitmap::Fill
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
PUBLIC	?Copy@CEnBitmap@@QAEHPAU_IMAGELIST@@@Z		; CEnBitmap::Copy
EXTRN	__imp__ImageList_GetImageCount@4:NEAR
EXTRN	__imp__ImageList_GetIconSize@12:NEAR
;	COMDAT ?Copy@CEnBitmap@@QAEHPAU_IMAGELIST@@@Z
_TEXT	SEGMENT
_hImageList$ = 8
_this$ = -36
_nWidth$ = -16
_nHeight$ = -28
_nCount$ = -24
_hdc$ = -32
_hbmDest$ = -4
_hdcMem$ = -20
_hBMOld$ = -8
_nIcon$ = -12
?Copy@CEnBitmap@@QAEHPAU_IMAGELIST@@@Z PROC NEAR	; CEnBitmap::Copy, COMDAT

; 760  : {

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

; 761  : 	GraphicsMisc::VerifyDeleteObject(*this);

	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?VerifyDeleteObject@GraphicsMisc@@YAHAAVCGdiObject@@@Z ; GraphicsMisc::VerifyDeleteObject
	add	esp, 4

; 762  : 	
; 763  : 	int nWidth = 0, nHeight = 0;

	mov	DWORD PTR _nWidth$[ebp], 0
	mov	DWORD PTR _nHeight$[ebp], 0

; 764  : 	int nCount = ImageList_GetImageCount(hImageList);

	mov	esi, esp
	mov	ecx, DWORD PTR _hImageList$[ebp]
	push	ecx
	call	DWORD PTR __imp__ImageList_GetImageCount@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nCount$[ebp], eax

; 765  : 	
; 766  : 	ImageList_GetIconSize(hImageList, &nWidth, &nHeight);

	mov	esi, esp
	lea	edx, DWORD PTR _nHeight$[ebp]
	push	edx
	lea	eax, DWORD PTR _nWidth$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hImageList$[ebp]
	push	ecx
	call	DWORD PTR __imp__ImageList_GetIconSize@12
	cmp	esi, esp
	call	__chkesp

; 767  : 	
; 768  : 	HDC hdc = GetDC(NULL);

	mov	esi, esp
	push	0
	call	DWORD PTR __imp__GetDC@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hdc$[ebp], eax

; 769  : 	HBITMAP hbmDest = ::CreateCompatibleBitmap(hdc, nWidth * nCount, nHeight);

	mov	esi, esp
	mov	edx, DWORD PTR _nHeight$[ebp]
	push	edx
	mov	eax, DWORD PTR _nWidth$[ebp]
	imul	eax, DWORD PTR _nCount$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hdc$[ebp]
	push	ecx
	call	DWORD PTR __imp__CreateCompatibleBitmap@12
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hbmDest$[ebp], eax

; 770  : 	HDC hdcMem = CreateCompatibleDC(hdc);

	mov	esi, esp
	mov	edx, DWORD PTR _hdc$[ebp]
	push	edx
	call	DWORD PTR __imp__CreateCompatibleDC@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hdcMem$[ebp], eax

; 771  : 	
; 772  : 	HBITMAP hBMOld = (HBITMAP)::SelectObject(hdcMem, hbmDest);

	mov	esi, esp
	mov	eax, DWORD PTR _hbmDest$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hdcMem$[ebp]
	push	ecx
	call	DWORD PTR __imp__SelectObject@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hBMOld$[ebp], eax

; 773  : 	
; 774  : 	for (int nIcon = 0; nIcon < nCount; nIcon++)

	mov	DWORD PTR _nIcon$[ebp], 0
	jmp	SHORT $L76326
$L76327:
	mov	edx, DWORD PTR _nIcon$[ebp]
	add	edx, 1
	mov	DWORD PTR _nIcon$[ebp], edx
$L76326:
	mov	eax, DWORD PTR _nIcon$[ebp]
	cmp	eax, DWORD PTR _nCount$[ebp]
	jge	SHORT $L76328
$L76329:

; 776  : 		VERIFY (ImageList_Draw(hImageList, nIcon, hdcMem, nIcon * nWidth, 0, ILD_NORMAL));

	mov	esi, esp
	push	0
	push	0
	mov	ecx, DWORD PTR _nIcon$[ebp]
	imul	ecx, DWORD PTR _nWidth$[ebp]
	push	ecx
	mov	edx, DWORD PTR _hdcMem$[ebp]
	push	edx
	mov	eax, DWORD PTR _nIcon$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hImageList$[ebp]
	push	ecx
	call	DWORD PTR __imp__ImageList_Draw@24
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L76332
	push	776					; 00000308H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76332
	int	3
$L76332:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76329

; 777  : 	}

	jmp	SHORT $L76327
$L76328:

; 778  : 	
; 779  : 	// cleanup
; 780  : 	::SelectObject(hdcMem, hBMOld);

	mov	esi, esp
	mov	eax, DWORD PTR _hBMOld$[ebp]
	push	eax
	mov	ecx, DWORD PTR _hdcMem$[ebp]
	push	ecx
	call	DWORD PTR __imp__SelectObject@8
	cmp	esi, esp
	call	__chkesp

; 781  : 	DeleteDC(hdcMem);

	mov	esi, esp
	mov	edx, DWORD PTR _hdcMem$[ebp]
	push	edx
	call	DWORD PTR __imp__DeleteDC@4
	cmp	esi, esp
	call	__chkesp

; 782  : 	
; 783  : 	return Attach(hbmDest);

	mov	eax, DWORD PTR _hbmDest$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Attach@CGdiObject@@QAEHPAX@Z		; CGdiObject::Attach

; 784  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?Copy@CEnBitmap@@QAEHPAU_IMAGELIST@@@Z ENDP		; CEnBitmap::Copy
_TEXT	ENDS
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
;	COMDAT ?GetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?GetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QBEHXZ PROC NEAR ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::GetSize, COMDAT

; 245  : 	{ return m_nSize; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+8]
	mov	esp, ebp
	pop	ebp
	ret	0
?GetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QBEHXZ ENDP ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::GetSize
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
PUBLIC	?ElementAt@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEAAPAVC32BitImageProcessor@@H@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::ElementAt
;	COMDAT ??A?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEAAPAVC32BitImageProcessor@@H@Z
_TEXT	SEGMENT
_nIndex$ = 8
_this$ = -4
??A?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEAAPAVC32BitImageProcessor@@H@Z PROC NEAR ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::operator[], COMDAT

; 280  : 	{ return ElementAt(nIndex); }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _nIndex$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ElementAt@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEAAPAVC32BitImageProcessor@@H@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::ElementAt
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??A?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEAAPAVC32BitImageProcessor@@H@Z ENDP ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::operator[]
_TEXT	ENDS
PUBLIC	?DestructElements@@YGXPAPAVC32BitImageProcessor@@H@Z ; DestructElements
EXTRN	??1CObject@@UAE@XZ:NEAR				; CObject::~CObject
EXTRN	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z:NEAR ; AfxAssertValidObject
;	COMDAT xdata$x
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
xdata$x	SEGMENT
$T77273	DD	019930520H
	DD	01H
	DD	FLAT:$T77275
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77275	DD	0ffffffffH
	DD	FLAT:$L77271
xdata$x	ENDS
;	COMDAT ??1?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAE@XZ
_TEXT	SEGMENT
$T77270 = -16
__$EHRec$ = -12
_this$ = -20
??1?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAE@XZ PROC NEAR ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::~CArray<C32BitImageProcessor *,C32BitImageProcessor *>, COMDAT

; 294  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77274
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
	je	SHORT $L76503

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
	mov	DWORD PTR $T77270[ebp], edx
	mov	eax, DWORD PTR $T77270[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L76503:

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
$L77271:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L77274:
	mov	eax, OFFSET FLAT:$T77273
	jmp	___CxxFrameHandler
text$x	ENDS
??1?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAE@XZ ENDP	; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::~CArray<C32BitImageProcessor *,C32BitImageProcessor *>
PUBLIC	?SetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHH@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::SetSize
PUBLIC	?SerializeElements@@YGXAAVCArchive@@PAPAVC32BitImageProcessor@@H@Z ; SerializeElements
EXTRN	?ReadCount@CArchive@@QAEKXZ:NEAR		; CArchive::ReadCount
EXTRN	?WriteCount@CArchive@@QAEXK@Z:NEAR		; CArchive::WriteCount
EXTRN	?IsStoring@CArchive@@QBEHXZ:NEAR		; CArchive::IsStoring
;	COMDAT ?Serialize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT
_ar$ = 8
_this$ = -8
_nOldSize$76517 = -4
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
	je	SHORT $L76515

; 520  : 		ar.WriteCount(m_nSize);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?WriteCount@CArchive@@QAEXK@Z		; CArchive::WriteCount

; 522  : 	else

	jmp	SHORT $L76516
$L76515:

; 524  : 		DWORD nOldSize = ar.ReadCount();

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?ReadCount@CArchive@@QAEKXZ		; CArchive::ReadCount
	mov	DWORD PTR _nOldSize$76517[ebp], eax

; 525  : 		SetSize(nOldSize, -1);

	push	-1
	mov	ecx, DWORD PTR _nOldSize$76517[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHH@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::SetSize
$L76516:

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
	jle	SHORT $L76528

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
$L76528:

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
	jne	SHORT $L76538
$L76539:

; 553  : 		ASSERT(m_nSize == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+8], 0
	je	SHORT $L76542
	push	553					; 00000229H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76542
	int	3
$L76542:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76539
$L76541:

; 554  : 		ASSERT(m_nMaxSize == 0);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	je	SHORT $L76546
	push	554					; 0000022aH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76546
	int	3
$L76546:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76541

; 556  : 	else

	jmp	$L76562
$L76538:

; 558  : 		ASSERT(m_nSize >= 0);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+8], 0
	jge	SHORT $L76551
	push	558					; 0000022eH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76551
	int	3
$L76551:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76538
$L76550:

; 559  : 		ASSERT(m_nMaxSize >= 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jge	SHORT $L76555
	push	559					; 0000022fH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76555
	int	3
$L76555:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76550
$L76554:

; 560  : 		ASSERT(m_nSize <= m_nMaxSize);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax+8]
	cmp	edx, DWORD PTR [ecx+12]
	jle	SHORT $L76559
	push	560					; 00000230H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76559
	int	3
$L76559:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76554
$L76558:

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
	jne	SHORT $L76564
	push	561					; 00000231H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76564
	int	3
$L76564:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76558
$L76562:

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
	je	SHORT $L76572
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L76572:
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
;	COMDAT ?SetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHH@Z
_TEXT	SEGMENT
$T77288 = -16
$T77289 = -20
$T77294 = -24
$T77295 = -28
_nNewSize$ = 8
_nGrowBy$ = 12
_this$ = -32
_nGrowBy$76726 = -8
_nNewMax$76728 = -12
_pNewData$76740 = -4
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
$L76683:

; 308  : 	ASSERT(nNewSize >= 0);

	cmp	DWORD PTR _nNewSize$[ebp], 0
	jge	SHORT $L76686
	push	308					; 00000134H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76686
	int	3
$L76686:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76683

; 309  : 
; 310  : 	if (nGrowBy != -1)

	cmp	DWORD PTR _nGrowBy$[ebp], -1
	je	SHORT $L76687

; 311  : 		m_nGrowBy = nGrowBy;  // set new size

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nGrowBy$[ebp]
	mov	DWORD PTR [edx+16], eax
$L76687:

; 312  : 
; 313  : 	if (nNewSize == 0)

	cmp	DWORD PTR _nNewSize$[ebp], 0
	jne	SHORT $L76688

; 315  : 		// shrink to nothing
; 316  : 		if (m_pData != NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L76689

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
	mov	DWORD PTR $T77288[ebp], ecx
	mov	edx, DWORD PTR $T77288[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 320  : 			m_pData = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+4], 0
$L76689:

; 322  : 		m_nSize = m_nMaxSize = 0;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+12], 0
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+8], 0

; 324  : 	else if (m_pData == NULL)

	jmp	$L76725
$L76688:
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L76697
$L76698:

; 326  : 		// create one with exact size
; 327  : #ifdef SIZE_T_MAX
; 328  : 		ASSERT(nNewSize <= SIZE_T_MAX/sizeof(TYPE));    // no overflow

	cmp	DWORD PTR _nNewSize$[ebp], 1073741823	; 3fffffffH
	jbe	SHORT $L76702
	push	328					; 00000148H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76702
	int	3
$L76702:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76698

; 329  : #endif
; 330  : 		m_pData = (TYPE*) new BYTE[nNewSize * sizeof(TYPE)];

	push	330					; 0000014aH
	push	OFFSET FLAT:__szAfxTempl
	mov	edx, DWORD PTR _nNewSize$[ebp]
	shl	edx, 2
	push	edx
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77289[ebp], eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR $T77289[ebp]
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

	jmp	$L76725
$L76697:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nNewSize$[ebp]
	cmp	ecx, DWORD PTR [eax+12]
	jg	SHORT $L76713

; 336  : 		// it fits
; 337  : 		if (nNewSize > m_nSize)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nNewSize$[ebp]
	cmp	eax, DWORD PTR [edx+8]
	jle	SHORT $L76714

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

	jmp	SHORT $L76720
$L76714:
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	cmp	eax, DWORD PTR _nNewSize$[ebp]
	jle	SHORT $L76720

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
$L76720:

; 347  : 		m_nSize = nNewSize;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nNewSize$[ebp]
	mov	DWORD PTR [ecx+8], edx

; 349  : 	else

	jmp	$L76725
$L76713:

; 351  : 		// otherwise, grow array
; 352  : 		int nGrowBy = m_nGrowBy;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR _nGrowBy$76726[ebp], ecx

; 353  : 		if (nGrowBy == 0)

	cmp	DWORD PTR _nGrowBy$76726[ebp], 0
	jne	SHORT $L76727

; 355  : 			// heuristically determine growth when nGrowBy == 0
; 356  : 			//  (this avoids heap fragmentation in many situations)
; 357  : 			nGrowBy = m_nSize / 8;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	cdq
	and	edx, 7
	add	eax, edx
	sar	eax, 3
	mov	DWORD PTR _nGrowBy$76726[ebp], eax

; 358  : 			nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);

	cmp	DWORD PTR _nGrowBy$76726[ebp], 4
	jge	SHORT $L77292
	mov	DWORD PTR -36+[ebp], 4
	jmp	SHORT $L77293
$L77292:
	cmp	DWORD PTR _nGrowBy$76726[ebp], 1024	; 00000400H
	jle	SHORT $L77290
	mov	DWORD PTR -40+[ebp], 1024		; 00000400H
	jmp	SHORT $L77291
$L77290:
	mov	eax, DWORD PTR _nGrowBy$76726[ebp]
	mov	DWORD PTR -40+[ebp], eax
$L77291:
	mov	ecx, DWORD PTR -40+[ebp]
	mov	DWORD PTR -36+[ebp], ecx
$L77293:
	mov	edx, DWORD PTR -36+[ebp]
	mov	DWORD PTR _nGrowBy$76726[ebp], edx
$L76727:

; 361  : 		if (nNewSize < m_nMaxSize + nGrowBy)

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	add	ecx, DWORD PTR _nGrowBy$76726[ebp]
	cmp	DWORD PTR _nNewSize$[ebp], ecx
	jge	SHORT $L76729

; 362  : 			nNewMax = m_nMaxSize + nGrowBy;  // granularity

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	add	eax, DWORD PTR _nGrowBy$76726[ebp]
	mov	DWORD PTR _nNewMax$76728[ebp], eax

; 363  : 		else

	jmp	SHORT $L76730
$L76729:

; 364  : 			nNewMax = nNewSize;  // no slush

	mov	ecx, DWORD PTR _nNewSize$[ebp]
	mov	DWORD PTR _nNewMax$76728[ebp], ecx
$L76730:

; 365  : 
; 366  : 		ASSERT(nNewMax >= m_nMaxSize);  // no wrap around

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nNewMax$76728[ebp]
	cmp	eax, DWORD PTR [edx+12]
	jge	SHORT $L76734
	push	366					; 0000016eH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76734
	int	3
$L76734:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76730
$L76733:

; 367  : #ifdef SIZE_T_MAX
; 368  : 		ASSERT(nNewMax <= SIZE_T_MAX/sizeof(TYPE)); // no overflow

	cmp	DWORD PTR _nNewMax$76728[ebp], 1073741823 ; 3fffffffH
	jbe	SHORT $L76739
	push	368					; 00000170H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76739
	int	3
$L76739:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76733

; 369  : #endif
; 370  : 		TYPE* pNewData = (TYPE*) new BYTE[nNewMax * sizeof(TYPE)];

	push	370					; 00000172H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _nNewMax$76728[ebp]
	shl	eax, 2
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77294[ebp], eax
	mov	ecx, DWORD PTR $T77294[ebp]
	mov	DWORD PTR _pNewData$76740[ebp], ecx

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
	mov	eax, DWORD PTR _pNewData$76740[ebp]
	push	eax
	call	_memcpy
	add	esp, 12					; 0000000cH
$L76746:

; 374  : 
; 375  : 		// construct remaining elements
; 376  : 		ASSERT(nNewSize > m_nSize);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nNewSize$[ebp]
	cmp	edx, DWORD PTR [ecx+8]
	jg	SHORT $L76749
	push	376					; 00000178H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76749
	int	3
$L76749:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76746

; 377  : 		ConstructElements<TYPE>(&pNewData[m_nSize], nNewSize-m_nSize);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nNewSize$[ebp]
	sub	edx, DWORD PTR [ecx+8]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	edx, DWORD PTR _pNewData$76740[ebp]
	lea	eax, DWORD PTR [edx+ecx*4]
	push	eax
	call	?ConstructElements@@YGXPAPAVC32BitImageProcessor@@H@Z ; ConstructElements

; 378  : 
; 379  : 		// get rid of old stuff (note: no destructors called)
; 380  : 		delete[] (BYTE*)m_pData;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR $T77295[ebp], edx
	mov	eax, DWORD PTR $T77295[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 381  : 		m_pData = pNewData;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _pNewData$76740[ebp]
	mov	DWORD PTR [ecx+4], edx

; 382  : 		m_nSize = nNewSize;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nNewSize$[ebp]
	mov	DWORD PTR [eax+8], ecx

; 383  : 		m_nMaxSize = nNewMax;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nNewMax$76728[ebp]
	mov	DWORD PTR [edx+12], eax
$L76725:

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
;	COMDAT ?ElementAt@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEAAPAVC32BitImageProcessor@@H@Z
_TEXT	SEGMENT
_nIndex$ = 8
_this$ = -4
?ElementAt@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEAAPAVC32BitImageProcessor@@H@Z PROC NEAR ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::ElementAt, COMDAT

; 262  : 	{ ASSERT(nIndex >= 0 && nIndex < m_nSize);

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L76760:
	cmp	DWORD PTR _nIndex$[ebp], 0
	jl	SHORT $L76764
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nIndex$[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jl	SHORT $L76763
$L76764:
	push	262					; 00000106H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76763
	int	3
$L76763:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76760

; 263  : 		return m_pData[nIndex]; }

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _nIndex$[ebp]
	lea	eax, DWORD PTR [ecx+edx*4]
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ElementAt@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEAAPAVC32BitImageProcessor@@H@Z ENDP ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::ElementAt
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
$L76770:

; 439  : 	ASSERT(nIndex >= 0);

	cmp	DWORD PTR _nIndex$[ebp], 0
	jge	SHORT $L76773
	push	439					; 000001b7H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76773
	int	3
$L76773:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76770

; 440  : 
; 441  : 	if (nIndex >= m_nSize)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nIndex$[ebp]
	cmp	eax, DWORD PTR [edx+8]
	jl	SHORT $L76774

; 442  : 		SetSize(nIndex+1, -1);

	push	-1
	mov	ecx, DWORD PTR _nIndex$[ebp]
	add	ecx, 1
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetSize@?$CArray@PAVC32BitImageProcessor@@PAV1@@@QAEXHH@Z ; CArray<C32BitImageProcessor *,C32BitImageProcessor *>::SetSize
$L76774:

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
$L76776:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76780
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76780
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76780
	int	3
$L76780:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76776

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L76781
$L76782:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 4
	mov	DWORD PTR _pElements$[ebp], eax
$L76781:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L76783

; 73   : 		pElements->~TYPE();

	jmp	SHORT $L76782
$L76783:

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
$L76785:

; 92   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76789
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76789
	push	93					; 0000005dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76789
	int	3
$L76789:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76785

; 93   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 94   : 
; 95   : 	// default is bit-wise read/write
; 96   : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L76790

; 97   : 		ar.Write((void*)pElements, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Write@CArchive@@QAEXPBXI@Z		; CArchive::Write

; 98   : 	else

	jmp	SHORT $L76793
$L76790:

; 99   : 		ar.Read((void*)pElements, nCount * sizeof(TYPE));

	mov	edx, DWORD PTR _nCount$[ebp]
	shl	edx, 2
	push	edx
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Read@CArchive@@QAEIPAXI@Z		; CArchive::Read
$L76793:

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
$L76797:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76801
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76801
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76801
	int	3
$L76801:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76797

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
;	COMDAT ?ConstructElements@@YGXPAPAVC32BitImageProcessor@@H@Z
_TEXT	SEGMENT
$T77308 = -4
_pElements$ = 8
_nCount$ = 12
?ConstructElements@@YGXPAPAVC32BitImageProcessor@@H@Z PROC NEAR ; ConstructElements, COMDAT

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
	mov	DWORD PTR $T77308[ebp], eax
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

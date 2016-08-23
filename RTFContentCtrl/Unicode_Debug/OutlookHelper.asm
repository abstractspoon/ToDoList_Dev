	TITLE	D:\_CODE\Shared\OutlookHelper.cpp
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
;	COMDAT ??0_Application@OutlookAPI@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0_NameSpace@OutlookAPI@@QAE@PAUIDispatch@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0MAPIFolder@OutlookAPI@@QAE@PAUIDispatch@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0_Explorer@OutlookAPI@@QAE@PAUIDispatch@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0Selection@OutlookAPI@@QAE@PAUIDispatch@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0_MailItem@OutlookAPI@@QAE@PAUIDispatch@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0COutlookItemDataMap@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCOutlookItemDataMap@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1COutlookItemDataMap@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0COutlookItemDataMap@@QAE@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFieldData@COutlookItemDataMap@@QBE?AVCString@@W4OUTLOOK_FIELDTYPE@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HasFieldData@COutlookItemDataMap@@QBEHW4OUTLOOK_FIELDTYPE@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E273
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E274
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E276
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E277
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E279
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E280
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E282
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E283
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E285
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E286
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0COutlookHelper@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCOutlookHelper@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G_Application@OutlookAPI@@QAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1_Application@OutlookAPI@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1COutlookHelper@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsOutlookInstalled@COutlookHelper@@SAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsUrlHandlerInstalled@COutlookHelper@@SAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InstallUrlHandler@COutlookHelper@@SAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?QueryInstallUrlHandler@COutlookHelper@@SAHIIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FormatItemAsUrl@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@K@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1MAPIFolder@OutlookAPI@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetItemData@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@W4OUTLOOK_FIELDTYPE@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetItemID@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetItemClass@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetItemData@COutlookHelper@@SAHAAV_MailItem@OutlookAPI@@AAVCOutlookItemDataMap@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?MapDate@COutlookHelper@@KA?AVCString@@N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsOutlookObject@COutlookHelper@@SAHPAUIDataObject@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsOutlookObject@COutlookHelper@@SAHPAVCOleDataObject@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsOutlookObject@COutlookHelper@@SAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsOutlookUrl@COutlookHelper@@SAHPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFirstFileObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@ABVCStringArray@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@PBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1_NameSpace@OutlookAPI@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSelection@COutlookHelper@@QAEPAVSelection@OutlookAPI@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1_Explorer@OutlookAPI@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSelectionCount@COutlookHelper@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1Selection@OutlookAPI@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFirstSelectedObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GSelection@OutlookAPI@@QAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFirstObject@COutlookHelper@@SAPAV_MailItem@OutlookAPI@@PAVSelection@3@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsConfidential@COutlookHelper@@SAHW4OUTLOOK_FIELDTYPE@@@Z
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
;	COMDAT ??0?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetCount@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Lookup@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEHW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEAAVCString@@W4OUTLOOK_FIELDTYPE@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveAll@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetStartPosition@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNextAssoc@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEXAAPAU__POSITION@@AAW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Serialize@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dump@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AssertValid@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InitHashTable@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXIH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewAssoc@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetAssocAt@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@IBEPAUCAssoc@1@W4OUTLOOK_FIELDTYPE@@AAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAW4OUTLOOK_FIELDTYPE@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAW4OUTLOOK_FIELDTYPE@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBW4OUTLOOK_FIELDTYPE@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBVCString@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ConstructElements@@YGXPAW4OUTLOOK_FIELDTYPE@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HashKey@@YGIW4OUTLOOK_FIELDTYPE@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CompareElements@@YGHPBW4OUTLOOK_FIELDTYPE@@0@Z
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
;	COMDAT ??_7?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCPtrList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$_CTypedPtrList@VCObList@@PAV1@@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_R0PAVCOleException@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_7COutlookHelper@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7COutlookItemDataMap@@6B@
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
PUBLIC	?CF_OUTLOOK@COutlookHelper@@2GB			; COutlookHelper::CF_OUTLOOK
PUBLIC	?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A ; COutlookHelper::s_pOutlook
PUBLIC	?s_nRefCount@COutlookHelper@@1HA		; COutlookHelper::s_nRefCount
PUBLIC	?s_bDenyConfidential@COutlookHelper@@1HA	; COutlookHelper::s_bDenyConfidential
PUBLIC	?OUTLOOKDATA@@3PAUOUTLOOKDATAITEM@@A		; OUTLOOKDATA
_BSS	SEGMENT
?CF_OUTLOOK@COutlookHelper@@2GB DW 01H DUP (?)		; COutlookHelper::CF_OUTLOOK
	ALIGN	4

?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A DD 01H DUP (?) ; COutlookHelper::s_pOutlook
?s_nRefCount@COutlookHelper@@1HA DD 01H DUP (?)		; COutlookHelper::s_nRefCount
?s_bDenyConfidential@COutlookHelper@@1HA DD 01H DUP (?)	; COutlookHelper::s_bDenyConfidential
_BSS	ENDS
_DATA	SEGMENT
__szAfxTempl DB	'afxtempl.h', 00H
	ORG $+1
_THIS_FILE DB	'D:\_CODE\Shared\OutlookHelper.cpp', 00H
_DATA	ENDS
CRT$XCU	SEGMENT
_$S275	DD	FLAT:_$E274
_$S278	DD	FLAT:_$E277
_$S281	DD	FLAT:_$E280
_$S284	DD	FLAT:_$E283
_$S287	DD	FLAT:_$E286
CRT$XCU	ENDS
_DATA	SEGMENT
	ORG $+2
?OUTLOOKDATA@@3PAUOUTLOOKDATAITEM@@A DD 00H		; OUTLOOKDATA
	DD	01H
	DD	01H
	DD	00H
	DD	02H
	DD	01H
	DD	03H
	DD	00H
	DD	04H
	DD	01H
	DD	05H
	DD	00H
	DD	06H
	DD	00H
	DD	07H
	DD	00H
	DD	08H
	DD	00H
	DD	09H
	DD	00H
	DD	0aH
	DD	00H
	DD	0bH
	DD	00H
	DD	0cH
	DD	00H
	DD	0dH
	DD	00H
	DD	0eH
	DD	00H
	DD	0fH
	DD	00H
	DD	010H
	DD	01H
	DD	011H
	DD	00H
	DD	012H
	DD	00H
	DD	013H
	DD	01H
	DD	014H
	DD	01H
	DD	015H
	DD	01H
	DD	016H
	DD	00H
	DD	017H
	DD	00H
	DD	018H
	DD	01H
	DD	019H
	DD	00H
	DD	01aH
	DD	00H
	DD	01bH
	DD	00H
	DD	01cH
	DD	00H
	DD	01dH
	DD	01H
$SG75669 DB	'R', 00H, 'e', 00H, 'n', 00H, 'P', 00H, 'r', 00H, 'i', 00H
	DB	'v', 00H, 'a', 00H, 't', 00H, 'e', 00H, 'M', 00H, 'e', 00H, 's'
	DB	00H, 's', 00H, 'a', 00H, 'g', 00H, 'e', 00H, 's', 00H, 00H, 00H
	ORG $+2
$SG75682 DB	'R', 00H, 'e', 00H, 'n', 00H, 'P', 00H, 'r', 00H, 'i', 00H
	DB	'v', 00H, 'a', 00H, 't', 00H, 'e', 00H, 'S', 00H, 'o', 00H, 'u'
	DB	00H, 'r', 00H, 'c', 00H, 'e', 00H, 'F', 00H, 'o', 00H, 'l', 00H
	DB	'd', 00H, 'e', 00H, 'r', 00H, 00H, 00H
	ORG $+2
$SG75690 DB	'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'C', 00H, 'o', 00H
	DB	'n', 00H, 't', 00H, 'e', 00H, 'n', 00H, 't', 00H, 's', 00H, 00H
	DB	00H
	ORG $+2
$SG75698 DB	'F', 00H, 'i', 00H, 'l', 00H, 'e', 00H, 'G', 00H, 'r', 00H
	DB	'o', 00H, 'u', 00H, 'p', 00H, 'D', 00H, 'e', 00H, 's', 00H, 'c'
	DB	00H, 'r', 00H, 'i', 00H, 'p', 00H, 't', 00H, 'o', 00H, 'r', 00H
	DB	'W', 00H, 00H, 00H
_DATA	ENDS
PUBLIC	??0?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAE@H@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>
PUBLIC	?Serialize@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAEXAAVCArchive@@@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::Serialize
PUBLIC	?Dump@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UBEXAAVCDumpContext@@@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::Dump
PUBLIC	?AssertValid@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UBEXXZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::AssertValid
PUBLIC	??0COutlookItemDataMap@@QAE@XZ			; COutlookItemDataMap::COutlookItemDataMap
PUBLIC	??_7COutlookItemDataMap@@6B@			; COutlookItemDataMap::`vftable'
PUBLIC	??_GCOutlookItemDataMap@@UAEPAXI@Z		; COutlookItemDataMap::`scalar deleting destructor'
PUBLIC	??_ECOutlookItemDataMap@@UAEPAXI@Z		; COutlookItemDataMap::`vector deleting destructor'
EXTRN	?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ:NEAR ; CObject::GetRuntimeClass
EXTRN	__chkesp:NEAR
;	COMDAT ??_7COutlookItemDataMap@@6B@
; File D:\_CODE\Shared\OutlookHelper.cpp
CONST	SEGMENT
??_7COutlookItemDataMap@@6B@ DD FLAT:?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ ; COutlookItemDataMap::`vftable'
	DD	FLAT:??_ECOutlookItemDataMap@@UAEPAXI@Z
	DD	FLAT:?Serialize@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UBEXXZ
	DD	FLAT:?Dump@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UBEXAAVCDumpContext@@@Z
CONST	ENDS
;	COMDAT ??0COutlookItemDataMap@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0COutlookItemDataMap@@QAE@XZ PROC NEAR		; COutlookItemDataMap::COutlookItemDataMap, COMDAT

; 27   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	10					; 0000000aH
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAE@H@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7COutlookItemDataMap@@6B@ ; COutlookItemDataMap::`vftable'

; 28   : 
; 29   : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0COutlookItemDataMap@@QAE@XZ ENDP			; COutlookItemDataMap::COutlookItemDataMap
_TEXT	ENDS
PUBLIC	??1COutlookItemDataMap@@UAE@XZ			; COutlookItemDataMap::~COutlookItemDataMap
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_GCOutlookItemDataMap@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCOutlookItemDataMap@@UAEPAXI@Z PROC NEAR		; COutlookItemDataMap::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1COutlookItemDataMap@@UAE@XZ		; COutlookItemDataMap::~COutlookItemDataMap
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75636
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L75636:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCOutlookItemDataMap@@UAEPAXI@Z ENDP			; COutlookItemDataMap::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??1?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAE@XZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::~CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>
;	COMDAT ??1COutlookItemDataMap@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1COutlookItemDataMap@@UAE@XZ PROC NEAR		; COutlookItemDataMap::~COutlookItemDataMap, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAE@XZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::~CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1COutlookItemDataMap@@UAE@XZ ENDP			; COutlookItemDataMap::~COutlookItemDataMap
_TEXT	ENDS
PUBLIC	?GetCount@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEHXZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetCount
PUBLIC	?SetAt@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::SetAt
PUBLIC	?RemoveAll@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXXZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::RemoveAll
PUBLIC	?GetStartPosition@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEPAU__POSITION@@XZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetStartPosition
PUBLIC	?GetNextAssoc@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEXAAPAU__POSITION@@AAW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetNextAssoc
PUBLIC	??0COutlookItemDataMap@@QAE@ABV0@@Z		; COutlookItemDataMap::COutlookItemDataMap
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT xdata$x
; File D:\_CODE\Shared\OutlookHelper.cpp
xdata$x	SEGMENT
$T76640	DD	019930520H
	DD	02H
	DD	FLAT:$T76643
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76643	DD	0ffffffffH
	DD	FLAT:$L76635
	DD	00H
	DD	FLAT:$L76636
xdata$x	ENDS
;	COMDAT ??0COutlookItemDataMap@@QAE@ABV0@@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_map$ = 8
_this$ = -28
_pos$ = -16
_nField$75647 = -20
_sData$75648 = -24
??0COutlookItemDataMap@@QAE@ABV0@@Z PROC NEAR		; COutlookItemDataMap::COutlookItemDataMap, COMDAT

; 32   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76641
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
	mov	DWORD PTR _this$[ebp], ecx
	push	10					; 0000000aH
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAE@H@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7COutlookItemDataMap@@6B@ ; COutlookItemDataMap::`vftable'

; 33   : 	RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXXZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::RemoveAll

; 34   : 
; 35   : 	POSITION pos = map.GetStartPosition();

	mov	ecx, DWORD PTR _map$[ebp]
	call	?GetStartPosition@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEPAU__POSITION@@XZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetStartPosition
	mov	DWORD PTR _pos$[ebp], eax
$L75645:

; 36   : 
; 37   : 	while (pos)

	cmp	DWORD PTR _pos$[ebp], 0
	je	SHORT $L75646

; 39   : 		OUTLOOK_FIELDTYPE nField;
; 40   : 		CString sData;

	lea	ecx, DWORD PTR _sData$75648[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 41   : 
; 42   : 		map.GetNextAssoc(pos, nField, sData);

	lea	ecx, DWORD PTR _sData$75648[ebp]
	push	ecx
	lea	edx, DWORD PTR _nField$75647[ebp]
	push	edx
	lea	eax, DWORD PTR _pos$[ebp]
	push	eax
	mov	ecx, DWORD PTR _map$[ebp]
	call	?GetNextAssoc@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEXAAPAU__POSITION@@AAW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetNextAssoc

; 43   : 
; 44   : 		SetAt(nField, sData);

	lea	ecx, DWORD PTR _sData$75648[ebp]
	push	ecx
	mov	edx, DWORD PTR _nField$75647[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetAt@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::SetAt

; 45   : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sData$75648[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	SHORT $L75645
$L75646:

; 46   : 
; 47   : 	ASSERT(map.GetCount() == GetCount());

	mov	ecx, DWORD PTR _map$[ebp]
	call	?GetCount@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEHXZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetCount
	mov	esi, eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCount@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEHXZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetCount
	cmp	esi, eax
	je	SHORT $L75652
	push	47					; 0000002fH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75652
	int	3
$L75652:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75646

; 48   : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR _this$[ebp]
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
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76635:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAE@XZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::~CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>
	ret	0
$L76636:
	lea	ecx, DWORD PTR _sData$75648[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76641:
	mov	eax, OFFSET FLAT:$T76640
	jmp	___CxxFrameHandler
text$x	ENDS
??0COutlookItemDataMap@@QAE@ABV0@@Z ENDP		; COutlookItemDataMap::COutlookItemDataMap
PUBLIC	?Lookup@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEHW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::Lookup
PUBLIC	?GetFieldData@COutlookItemDataMap@@QBE?AVCString@@W4OUTLOOK_FIELDTYPE@@@Z ; COutlookItemDataMap::GetFieldData
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
;	COMDAT xdata$x
; File D:\_CODE\Shared\OutlookHelper.cpp
xdata$x	SEGMENT
$T76654	DD	019930520H
	DD	02H
	DD	FLAT:$T76656
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76656	DD	0ffffffffH
	DD	FLAT:$L76650
	DD	00H
	DD	FLAT:$L76649
xdata$x	ENDS
;	COMDAT ?GetFieldData@COutlookItemDataMap@@QBE?AVCString@@W4OUTLOOK_FIELDTYPE@@@Z
_TEXT	SEGMENT
$T76651 = -20
__$EHRec$ = -12
_nField$ = 12
___$ReturnUdt$ = 8
_this$ = -24
_sData$ = -16
?GetFieldData@COutlookItemDataMap@@QBE?AVCString@@W4OUTLOOK_FIELDTYPE@@@Z PROC NEAR ; COutlookItemDataMap::GetFieldData, COMDAT

; 51   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76655
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T76651[ebp], 0

; 52   : 	CString sData;

	lea	ecx, DWORD PTR _sData$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 53   : 	Lookup(nField, sData);

	lea	eax, DWORD PTR _sData$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nField$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Lookup@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEHW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::Lookup

; 54   : 	return sData;

	lea	edx, DWORD PTR _sData$[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T76651[ebp]
	or	al, 1
	mov	DWORD PTR $T76651[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sData$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 55   : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76649:
	lea	ecx, DWORD PTR _sData$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76650:
	mov	eax, DWORD PTR $T76651[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76652
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76652:
	ret	0
$L76655:
	mov	eax, OFFSET FLAT:$T76654
	jmp	___CxxFrameHandler
text$x	ENDS
?GetFieldData@COutlookItemDataMap@@QBE?AVCString@@W4OUTLOOK_FIELDTYPE@@@Z ENDP ; COutlookItemDataMap::GetFieldData
PUBLIC	?HasFieldData@COutlookItemDataMap@@QBEHW4OUTLOOK_FIELDTYPE@@@Z ; COutlookItemDataMap::HasFieldData
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
;	COMDAT xdata$x
; File D:\_CODE\Shared\OutlookHelper.cpp
xdata$x	SEGMENT
$T76665	DD	019930520H
	DD	01H
	DD	FLAT:$T76667
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76667	DD	0ffffffffH
	DD	FLAT:$L76663
xdata$x	ENDS
;	COMDAT ?HasFieldData@COutlookItemDataMap@@QBEHW4OUTLOOK_FIELDTYPE@@@Z
_TEXT	SEGMENT
$T76661 = -16
$T76662 = -20
__$EHRec$ = -12
_nField$ = 8
_this$ = -24
?HasFieldData@COutlookItemDataMap@@QBEHW4OUTLOOK_FIELDTYPE@@@Z PROC NEAR ; COutlookItemDataMap::HasFieldData, COMDAT

; 58   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76666
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

; 59   : 	return !(GetFieldData(nField).IsEmpty());

	mov	eax, DWORD PTR _nField$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T76662[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetFieldData@COutlookItemDataMap@@QBE?AVCString@@W4OUTLOOK_FIELDTYPE@@@Z ; COutlookItemDataMap::GetFieldData
	mov	DWORD PTR -28+[ebp], eax
	mov	edx, DWORD PTR -28+[ebp]
	mov	DWORD PTR -32+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -32+[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR $T76661[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76662[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76661[ebp]

; 60   : }

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
$L76663:
	lea	ecx, DWORD PTR $T76662[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76666:
	mov	eax, OFFSET FLAT:$T76665
	jmp	___CxxFrameHandler
text$x	ENDS
?HasFieldData@COutlookItemDataMap@@QBEHW4OUTLOOK_FIELDTYPE@@@Z ENDP ; COutlookItemDataMap::HasFieldData
;	COMDAT _$E274
_TEXT	SEGMENT
_$E274	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E273
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E274	ENDP
_TEXT	ENDS
EXTRN	__imp__RegisterClipboardFormatW@4:NEAR
_BSS	SEGMENT
_CF_RENPRIVATEMESSAGES DW 01H DUP (?)
_BSS	ENDS
;	COMDAT _$E273
_TEXT	SEGMENT
_$E273	PROC NEAR					; COMDAT

; 73   : static const CLIPFORMAT CF_RENPRIVATEMESSAGES = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_RENPRIVATEMESSAGES);

	push	ebp
	mov	ebp, esp
	push	esi
	mov	esi, esp
	push	OFFSET FLAT:$SG75669
	call	DWORD PTR __imp__RegisterClipboardFormatW@4
	cmp	esi, esp
	call	__chkesp
	mov	WORD PTR _CF_RENPRIVATEMESSAGES, ax
	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E273	ENDP
_TEXT	ENDS
;	COMDAT _$E277
_TEXT	SEGMENT
_$E277	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E276
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E277	ENDP
_TEXT	ENDS
_BSS	SEGMENT
	ALIGN	4

_CF_RENPRIVATESRCFOLDER DW 01H DUP (?)
_BSS	ENDS
;	COMDAT _$E276
_TEXT	SEGMENT
_$E276	PROC NEAR					; COMDAT

; 74   : static const CLIPFORMAT CF_RENPRIVATESRCFOLDER = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_RENPRIVATESRCFOLDER);	

	push	ebp
	mov	ebp, esp
	push	esi
	mov	esi, esp
	push	OFFSET FLAT:$SG75682
	call	DWORD PTR __imp__RegisterClipboardFormatW@4
	cmp	esi, esp
	call	__chkesp
	mov	WORD PTR _CF_RENPRIVATESRCFOLDER, ax
	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E276	ENDP
_TEXT	ENDS
;	COMDAT _$E280
_TEXT	SEGMENT
_$E280	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E279
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E280	ENDP
_TEXT	ENDS
_BSS	SEGMENT
	ALIGN	4

_CF_FILECONTENTS DW 01H DUP (?)
_BSS	ENDS
;	COMDAT _$E279
_TEXT	SEGMENT
_$E279	PROC NEAR					; COMDAT

; 75   : static const CLIPFORMAT CF_FILECONTENTS = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_FILECONTENTS);

	push	ebp
	mov	ebp, esp
	push	esi
	mov	esi, esp
	push	OFFSET FLAT:$SG75690
	call	DWORD PTR __imp__RegisterClipboardFormatW@4
	cmp	esi, esp
	call	__chkesp
	mov	WORD PTR _CF_FILECONTENTS, ax
	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E279	ENDP
_TEXT	ENDS
;	COMDAT _$E283
_TEXT	SEGMENT
_$E283	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E282
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E283	ENDP
_TEXT	ENDS
_BSS	SEGMENT
	ALIGN	4

_CF_FILEDESCRIPTOR DW 01H DUP (?)
_BSS	ENDS
;	COMDAT _$E282
_TEXT	SEGMENT
_$E282	PROC NEAR					; COMDAT

; 76   : static const CLIPFORMAT CF_FILEDESCRIPTOR = (CLIPFORMAT)::RegisterClipboardFormat(CFSTR_FILEDESCRIPTORW);

	push	ebp
	mov	ebp, esp
	push	esi
	mov	esi, esp
	push	OFFSET FLAT:$SG75698
	call	DWORD PTR __imp__RegisterClipboardFormatW@4
	cmp	esi, esp
	call	__chkesp
	mov	WORD PTR _CF_FILEDESCRIPTOR, ax
	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E282	ENDP
_TEXT	ENDS
;	COMDAT _$E286
_TEXT	SEGMENT
_$E286	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E285
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E286	ENDP
_TEXT	ENDS
;	COMDAT _$E285
_TEXT	SEGMENT
_$E285	PROC NEAR					; COMDAT

; 78   : const CLIPFORMAT COutlookHelper::CF_OUTLOOK = CF_RENPRIVATEMESSAGES;

	push	ebp
	mov	ebp, esp
	mov	ax, WORD PTR _CF_RENPRIVATEMESSAGES
	mov	WORD PTR ?CF_OUTLOOK@COutlookHelper@@2GB, ax ; COutlookHelper::CF_OUTLOOK
	pop	ebp
	ret	0
_$E285	ENDP
_TEXT	ENDS
PUBLIC	??0_Application@OutlookAPI@@QAE@XZ		; OutlookAPI::_Application::_Application
PUBLIC	??_G_Application@OutlookAPI@@QAEPAXI@Z		; OutlookAPI::_Application::`scalar deleting destructor'
PUBLIC	??0COutlookHelper@@QAE@XZ			; COutlookHelper::COutlookHelper
PUBLIC	?IsOutlookInstalled@COutlookHelper@@SAHXZ	; COutlookHelper::IsOutlookInstalled
PUBLIC	??_7COutlookHelper@@6B@				; COutlookHelper::`vftable'
PUBLIC	??_GCOutlookHelper@@UAEPAXI@Z			; COutlookHelper::`scalar deleting destructor'
PUBLIC	??_ECOutlookHelper@@UAEPAXI@Z			; COutlookHelper::`vector deleting destructor'
EXTRN	?CreateDispatch@COleDispatchDriver@@QAEHPBGPAVCOleException@@@Z:NEAR ; COleDispatchDriver::CreateDispatch
EXTRN	??2@YAPAXIPBDH@Z:NEAR				; operator new
EXTRN	??3@YAXPAXPBDH@Z:NEAR				; operator delete
;	COMDAT ??_7COutlookHelper@@6B@
; File D:\_CODE\Shared\OutlookHelper.cpp
CONST	SEGMENT
??_7COutlookHelper@@6B@ DD FLAT:??_ECOutlookHelper@@UAEPAXI@Z ; COutlookHelper::`vftable'
CONST	ENDS
;	COMDAT data
data	SEGMENT
$SG75735 DB	'O', 00H, 'u', 00H, 't', 00H, 'l', 00H, 'o', 00H, 'o', 00H
	DB	'k', 00H, '.', 00H, 'A', 00H, 'p', 00H, 'p', 00H, 'l', 00H, 'i'
	DB	00H, 'c', 00H, 'a', 00H, 't', 00H, 'i', 00H, 'o', 00H, 'n', 00H
	DB	00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76702	DD	019930520H
	DD	01H
	DD	FLAT:$T76704
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76704	DD	0ffffffffH
	DD	FLAT:$L76700
xdata$x	ENDS
;	COMDAT ??0COutlookHelper@@QAE@XZ
_TEXT	SEGMENT
$T76692 = -16
$T76693 = -20
$T76696 = -24
$T76697 = -28
__$EHRec$ = -12
_this$ = -32
??0COutlookHelper@@QAE@XZ PROC NEAR			; COutlookHelper::COutlookHelper, COMDAT

; 124  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76703
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 28					; 0000001cH
	push	ebx
	push	esi
	push	edi
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7COutlookHelper@@6B@ ; COutlookHelper::`vftable'

; 125  : 	if (s_pOutlook == NULL && IsOutlookInstalled())

	cmp	DWORD PTR ?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A, 0 ; COutlookHelper::s_pOutlook
	jne	$L75734
	call	?IsOutlookInstalled@COutlookHelper@@SAHXZ ; COutlookHelper::IsOutlookInstalled
	test	eax, eax
	je	$L75734
$L75724:

; 127  : 		ASSERT(s_nRefCount == 0);

	cmp	DWORD PTR ?s_nRefCount@COutlookHelper@@1HA, 0 ; COutlookHelper::s_nRefCount
	je	SHORT $L75727
	push	127					; 0000007fH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75727
	int	3
$L75727:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75724

; 128  : 
; 129  : 		s_pOutlook = new _Application;

	push	129					; 00000081H
	push	OFFSET FLAT:_THIS_FILE
	push	8
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T76693[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	cmp	DWORD PTR $T76693[ebp], 0
	je	SHORT $L76694
	mov	ecx, DWORD PTR $T76693[ebp]
	call	??0_Application@OutlookAPI@@QAE@XZ	; OutlookAPI::_Application::_Application
	mov	DWORD PTR -36+[ebp], eax
	jmp	SHORT $L76695
$L76694:
	mov	DWORD PTR -36+[ebp], 0
$L76695:
	mov	edx, DWORD PTR -36+[ebp]
	mov	DWORD PTR $T76692[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR $T76692[ebp]
	mov	DWORD PTR ?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A, eax ; COutlookHelper::s_pOutlook

; 130  : 
; 131  : 		if (!s_pOutlook->CreateDispatch(_T("Outlook.Application")))

	push	0
	push	OFFSET FLAT:$SG75735
	mov	ecx, DWORD PTR ?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A ; COutlookHelper::s_pOutlook
	call	?CreateDispatch@COleDispatchDriver@@QAEHPBGPAVCOleException@@@Z ; COleDispatchDriver::CreateDispatch
	test	eax, eax
	jne	SHORT $L75734

; 133  : 			delete s_pOutlook;

	mov	ecx, DWORD PTR ?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A ; COutlookHelper::s_pOutlook
	mov	DWORD PTR $T76697[ebp], ecx
	mov	edx, DWORD PTR $T76697[ebp]
	mov	DWORD PTR $T76696[ebp], edx
	cmp	DWORD PTR $T76696[ebp], 0
	je	SHORT $L76698
	push	1
	mov	ecx, DWORD PTR $T76696[ebp]
	call	??_G_Application@OutlookAPI@@QAEPAXI@Z	; OutlookAPI::_Application::`scalar deleting destructor'
	mov	DWORD PTR -40+[ebp], eax
	jmp	SHORT $L76699
$L76698:
	mov	DWORD PTR -40+[ebp], 0
$L76699:

; 134  : 			s_pOutlook = NULL;

	mov	DWORD PTR ?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A, 0 ; COutlookHelper::s_pOutlook
$L75734:

; 137  : 
; 138  : 	s_nRefCount++;

	mov	eax, DWORD PTR ?s_nRefCount@COutlookHelper@@1HA ; COutlookHelper::s_nRefCount
	add	eax, 1
	mov	DWORD PTR ?s_nRefCount@COutlookHelper@@1HA, eax ; COutlookHelper::s_nRefCount

; 139  : }

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76700:
	push	129					; 00000081H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR $T76693[ebp]
	push	eax
	call	??3@YAXPAXPBDH@Z			; operator delete
	add	esp, 12					; 0000000cH
	ret	0
$L76703:
	mov	eax, OFFSET FLAT:$T76702
	jmp	___CxxFrameHandler
text$x	ENDS
??0COutlookHelper@@QAE@XZ ENDP				; COutlookHelper::COutlookHelper
EXTRN	??0COleDispatchDriver@@QAE@XZ:NEAR		; COleDispatchDriver::COleDispatchDriver
;	COMDAT ??0_Application@OutlookAPI@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0_Application@OutlookAPI@@QAE@XZ PROC NEAR		; OutlookAPI::_Application::_Application, COMDAT

; 1195 : 	_Application() {}		// Calls COleDispatchDriver default constructor

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0COleDispatchDriver@@QAE@XZ		; COleDispatchDriver::COleDispatchDriver
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0_Application@OutlookAPI@@QAE@XZ ENDP			; OutlookAPI::_Application::_Application
_TEXT	ENDS
PUBLIC	??1COutlookHelper@@UAE@XZ			; COutlookHelper::~COutlookHelper
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT ??_GCOutlookHelper@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCOutlookHelper@@UAEPAXI@Z PROC NEAR			; COutlookHelper::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1COutlookHelper@@UAE@XZ		; COutlookHelper::~COutlookHelper
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75742
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75742:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCOutlookHelper@@UAEPAXI@Z ENDP			; COutlookHelper::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??1_Application@OutlookAPI@@QAE@XZ		; OutlookAPI::_Application::~_Application
;	COMDAT ??_G_Application@OutlookAPI@@QAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_G_Application@OutlookAPI@@QAEPAXI@Z PROC NEAR	; OutlookAPI::_Application::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1_Application@OutlookAPI@@QAE@XZ	; OutlookAPI::_Application::~_Application
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75745
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75745:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_G_Application@OutlookAPI@@QAEPAXI@Z ENDP		; OutlookAPI::_Application::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	??1COleDispatchDriver@@QAE@XZ:NEAR		; COleDispatchDriver::~COleDispatchDriver
;	COMDAT ??1_Application@OutlookAPI@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1_Application@OutlookAPI@@QAE@XZ PROC NEAR		; OutlookAPI::_Application::~_Application, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1COleDispatchDriver@@QAE@XZ		; COleDispatchDriver::~COleDispatchDriver
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1_Application@OutlookAPI@@QAE@XZ ENDP			; OutlookAPI::_Application::~_Application
_TEXT	ENDS
;	COMDAT ??1COutlookHelper@@UAE@XZ
_TEXT	SEGMENT
$T76720 = -4
$T76721 = -8
_this$ = -12
??1COutlookHelper@@UAE@XZ PROC NEAR			; COutlookHelper::~COutlookHelper, COMDAT

; 142  : {

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
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7COutlookHelper@@6B@ ; COutlookHelper::`vftable'

; 143  : 	s_nRefCount--;

	mov	ecx, DWORD PTR ?s_nRefCount@COutlookHelper@@1HA ; COutlookHelper::s_nRefCount
	sub	ecx, 1
	mov	DWORD PTR ?s_nRefCount@COutlookHelper@@1HA, ecx ; COutlookHelper::s_nRefCount

; 144  : 
; 145  : 	if (s_nRefCount == 0)

	cmp	DWORD PTR ?s_nRefCount@COutlookHelper@@1HA, 0 ; COutlookHelper::s_nRefCount
	jne	SHORT $L75751
$L75752:

; 147  : 		ASSERT(s_pOutlook);

	cmp	DWORD PTR ?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A, 0 ; COutlookHelper::s_pOutlook
	jne	SHORT $L75755
	push	147					; 00000093H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75755
	int	3
$L75755:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75752

; 148  : 
; 149  : 		delete s_pOutlook;

	mov	eax, DWORD PTR ?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A ; COutlookHelper::s_pOutlook
	mov	DWORD PTR $T76721[ebp], eax
	mov	ecx, DWORD PTR $T76721[ebp]
	mov	DWORD PTR $T76720[ebp], ecx
	cmp	DWORD PTR $T76720[ebp], 0
	je	SHORT $L76722
	push	1
	mov	ecx, DWORD PTR $T76720[ebp]
	call	??_G_Application@OutlookAPI@@QAEPAXI@Z	; OutlookAPI::_Application::`scalar deleting destructor'
	mov	DWORD PTR -16+[ebp], eax
	jmp	SHORT $L76723
$L76722:
	mov	DWORD PTR -16+[ebp], 0
$L76723:

; 150  : 		s_pOutlook = NULL;

	mov	DWORD PTR ?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A, 0 ; COutlookHelper::s_pOutlook
$L75751:

; 152  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1COutlookHelper@@UAE@XZ ENDP				; COutlookHelper::~COutlookHelper
_TEXT	ENDS
EXTRN	?IsRegisteredApp@CFileRegister@@SAHPBG0H@Z:NEAR	; CFileRegister::IsRegisteredApp
;	COMDAT data
; File D:\_CODE\Shared\OutlookHelper.cpp
data	SEGMENT
$SG75760 DB	'O', 00H, 'U', 00H, 'T', 00H, 'L', 00H, 'O', 00H, 'O', 00H
	DB	'K', 00H, '.', 00H, 'E', 00H, 'X', 00H, 'E', 00H, 00H, 00H
$SG75761 DB	'm', 00H, 's', 00H, 'g', 00H, 00H, 00H
data	ENDS
;	COMDAT ?IsOutlookInstalled@COutlookHelper@@SAHXZ
_TEXT	SEGMENT
?IsOutlookInstalled@COutlookHelper@@SAHXZ PROC NEAR	; COutlookHelper::IsOutlookInstalled, COMDAT

; 156  : {

	push	ebp
	mov	ebp, esp

; 157  : 	return CFileRegister::IsRegisteredApp(_T("msg"), _T("OUTLOOK.EXE"), TRUE);

	push	1
	push	OFFSET FLAT:$SG75760
	push	OFFSET FLAT:$SG75761
	call	?IsRegisteredApp@CFileRegister@@SAHPBG0H@Z ; CFileRegister::IsRegisteredApp
	add	esp, 12					; 0000000cH

; 158  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?IsOutlookInstalled@COutlookHelper@@SAHXZ ENDP		; COutlookHelper::IsOutlookInstalled
_TEXT	ENDS
PUBLIC	?IsUrlHandlerInstalled@COutlookHelper@@SAHXZ	; COutlookHelper::IsUrlHandlerInstalled
EXTRN	??0CRegKey@@QAE@XZ:NEAR				; CRegKey::CRegKey
EXTRN	??1CRegKey@@UAE@XZ:NEAR				; CRegKey::~CRegKey
EXTRN	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z:NEAR	; CRegKey::Open
EXTRN	?Read@CRegKey@@QBEJPBGAAVCString@@@Z:NEAR	; CRegKey::Read
_BSS	SEGMENT
	ALIGN	4

$SG75772 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\OutlookHelper.cpp
data	SEGMENT
$SG75769 DB	'o', 00H, 'u', 00H, 't', 00H, 'l', 00H, 'o', 00H, 'o', 00H
	DB	'k', 00H, '\', 00H, 's', 00H, 'h', 00H, 'e', 00H, 'l', 00H, 'l'
	DB	00H, '\', 00H, 'o', 00H, 'p', 00H, 'e', 00H, 'n', 00H, '\', 00H
	DB	'c', 00H, 'o', 00H, 'm', 00H, 'm', 00H, 'a', 00H, 'n', 00H, 'd'
	DB	00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76736	DD	019930520H
	DD	02H
	DD	FLAT:$T76738
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76738	DD	0ffffffffH
	DD	FLAT:$L76733
	DD	00H
	DD	FLAT:$L76734
xdata$x	ENDS
;	COMDAT ?IsUrlHandlerInstalled@COutlookHelper@@SAHXZ
_TEXT	SEGMENT
$T76729 = -36
$T76730 = -40
__$EHRec$ = -12
_reg$75765 = -32
_sEntry$75766 = -16
?IsUrlHandlerInstalled@COutlookHelper@@SAHXZ PROC NEAR	; COutlookHelper::IsUrlHandlerInstalled, COMDAT

; 161  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76737
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

; 162  : 	if (IsOutlookInstalled())

	call	?IsOutlookInstalled@COutlookHelper@@SAHXZ ; COutlookHelper::IsOutlookInstalled
	test	eax, eax
	je	$L75764

; 164  : 		CRegKey reg;

	lea	ecx, DWORD PTR _reg$75765[ebp]
	call	??0CRegKey@@QAE@XZ			; CRegKey::CRegKey
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 165  : 		CString sEntry;

	lea	ecx, DWORD PTR _sEntry$75766[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 166  : 
; 167  : 		if (reg.Open(HKEY_CLASSES_ROOT, _T("outlook\\shell\\open\\command")) != ERROR_SUCCESS)

	push	OFFSET FLAT:$SG75769
	push	-2147483648				; 80000000H
	lea	ecx, DWORD PTR _reg$75765[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open
	test	eax, eax
	je	SHORT $L75768

; 168  : 			return FALSE;

	mov	DWORD PTR $T76729[ebp], 0
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sEntry$75766[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$75765[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T76729[ebp]
	jmp	SHORT $L75763
$L75768:

; 169  : 
; 170  : 		return (reg.Read(_T(""), sEntry) == ERROR_SUCCESS && !sEntry.IsEmpty());

	lea	eax, DWORD PTR _sEntry$75766[ebp]
	push	eax
	push	OFFSET FLAT:$SG75772
	lea	ecx, DWORD PTR _reg$75765[ebp]
	call	?Read@CRegKey@@QBEJPBGAAVCString@@@Z	; CRegKey::Read
	test	eax, eax
	jne	SHORT $L76731
	lea	ecx, DWORD PTR _sEntry$75766[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L76731
	mov	DWORD PTR -44+[ebp], 1
	jmp	SHORT $L76732
$L76731:
	mov	DWORD PTR -44+[ebp], 0
$L76732:
	mov	ecx, DWORD PTR -44+[ebp]
	mov	DWORD PTR $T76730[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sEntry$75766[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$75765[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T76730[ebp]
	jmp	SHORT $L75763
$L75764:

; 172  : 
; 173  : 	// all else
; 174  : 	return FALSE;

	xor	eax, eax
$L75763:

; 175  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 44					; 0000002cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76733:
	lea	ecx, DWORD PTR _reg$75765[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	ret	0
$L76734:
	lea	ecx, DWORD PTR _sEntry$75766[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76737:
	mov	eax, OFFSET FLAT:$T76736
	jmp	___CxxFrameHandler
text$x	ENDS
?IsUrlHandlerInstalled@COutlookHelper@@SAHXZ ENDP	; COutlookHelper::IsUrlHandlerInstalled
PUBLIC	?InstallUrlHandler@COutlookHelper@@SAHXZ	; COutlookHelper::InstallUrlHandler
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	??H@YG?AVCString@@ABV0@PBG@Z:NEAR		; operator+
EXTRN	?GetRegisteredAppPath@CFileRegister@@SA?AVCString@@PBGH@Z:NEAR ; CFileRegister::GetRegisteredAppPath
EXTRN	?Close@CRegKey@@QAEXXZ:NEAR			; CRegKey::Close
EXTRN	?Write@CRegKey@@QAEJPBG0@Z:NEAR			; CRegKey::Write
EXTRN	?DeleteKey@CRegKey@@SAJPAUHKEY__@@PBG@Z:NEAR	; CRegKey::DeleteKey
EXTRN	?KeyExists@CRegKey@@SAHPAUHKEY__@@PBG@Z:NEAR	; CRegKey::KeyExists
_BSS	SEGMENT
	ALIGN	4

$SG75786 DW	01H DUP (?)
	ALIGN	4

$SG75789 DW	01H DUP (?)
	ALIGN	4

$SG75795 DW	01H DUP (?)
	ALIGN	4

$SG75806 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\OutlookHelper.cpp
data	SEGMENT
$SG75779 DB	'o', 00H, 'u', 00H, 't', 00H, 'l', 00H, 'o', 00H, 'o', 00H
	DB	'k', 00H, 00H, 00H
$SG75783 DB	'o', 00H, 'u', 00H, 't', 00H, 'l', 00H, 'o', 00H, 'o', 00H
	DB	'k', 00H, 00H, 00H
$SG75787 DB	'U', 00H, 'R', 00H, 'L', 00H, ' ', 00H, 'P', 00H, 'r', 00H
	DB	'o', 00H, 't', 00H, 'o', 00H, 'c', 00H, 'o', 00H, 'l', 00H, 00H
	DB	00H
	ORG $+2
$SG75788 DB	'U', 00H, 'R', 00H, 'L', 00H, ':', 00H, 'O', 00H, 'u', 00H
	DB	't', 00H, 'l', 00H, 'o', 00H, 'o', 00H, 'k', 00H, ' ', 00H, 'F'
	DB	00H, 'o', 00H, 'l', 00H, 'd', 00H, 'e', 00H, 'r', 00H, 's', 00H
	DB	00H, 00H
$SG75793 DB	'o', 00H, 'u', 00H, 't', 00H, 'l', 00H, 'o', 00H, 'o', 00H
	DB	'k', 00H, '\', 00H, 's', 00H, 'h', 00H, 'e', 00H, 'l', 00H, 'l'
	DB	00H, 00H, 00H
$SG75794 DB	'o', 00H, 'p', 00H, 'e', 00H, 'n', 00H, 00H, 00H
	ORG $+2
$SG75799 DB	'o', 00H, 'u', 00H, 't', 00H, 'l', 00H, 'o', 00H, 'o', 00H
	DB	'k', 00H, '\', 00H, 's', 00H, 'h', 00H, 'e', 00H, 'l', 00H, 'l'
	DB	00H, '\', 00H, 'o', 00H, 'p', 00H, 'e', 00H, 'n', 00H, '\', 00H
	DB	'c', 00H, 'o', 00H, 'm', 00H, 'm', 00H, 'a', 00H, 'n', 00H, 'd'
	DB	00H, 00H, 00H
	ORG $+2
$SG75802 DB	'm', 00H, 's', 00H, 'g', 00H, 00H, 00H
$SG75805 DB	' ', 00H, '/', 00H, 's', 00H, 'e', 00H, 'l', 00H, 'e', 00H
	DB	'c', 00H, 't', 00H, ' ', 00H, '"', 00H, '%', 00H, '1', 00H, '"'
	DB	00H, 00H, 00H
$SG75809 DB	'o', 00H, 'u', 00H, 't', 00H, 'l', 00H, 'o', 00H, 'o', 00H
	DB	'k', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76752	DD	019930520H
	DD	03H
	DD	FLAT:$T76754
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76754	DD	0ffffffffH
	DD	FLAT:$L76748
	DD	00H
	DD	FLAT:$L76749
	DD	01H
	DD	FLAT:$L76750
xdata$x	ENDS
;	COMDAT ?InstallUrlHandler@COutlookHelper@@SAHXZ
_TEXT	SEGMENT
_bKeyExists$ = -16
_reg$ = -36
_bSuccess$ = -20
_sOutlookPath$75800 = -40
_sCommand$75803 = -44
$T76744 = -48
$T76747 = -52
__$EHRec$ = -12
?InstallUrlHandler@COutlookHelper@@SAHXZ PROC NEAR	; COutlookHelper::InstallUrlHandler, COMDAT

; 178  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76753
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 44					; 0000002cH
	push	edi
	lea	edi, DWORD PTR [ebp-56]
	mov	ecx, 11					; 0000000bH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 179  : 	if (!IsOutlookInstalled())

	call	?IsOutlookInstalled@COutlookHelper@@SAHXZ ; COutlookHelper::IsOutlookInstalled
	test	eax, eax
	jne	SHORT $L75775

; 180  : 		return FALSE;

	xor	eax, eax
	jmp	$L75774
$L75775:

; 181  : 
; 182  : 	if (IsUrlHandlerInstalled())

	call	?IsUrlHandlerInstalled@COutlookHelper@@SAHXZ ; COutlookHelper::IsUrlHandlerInstalled
	test	eax, eax
	je	SHORT $L75776

; 183  : 		return TRUE;

	mov	eax, 1
	jmp	$L75774
$L75776:

; 184  : 
; 185  : 	// cache whether the key already existed so we don't delete it on failure
; 186  : 	BOOL bKeyExists = CRegKey::KeyExists(HKEY_CLASSES_ROOT, _T("outlook"));

	push	OFFSET FLAT:$SG75779
	push	-2147483648				; 80000000H
	call	?KeyExists@CRegKey@@SAHPAUHKEY__@@PBG@Z	; CRegKey::KeyExists
	add	esp, 8
	mov	DWORD PTR _bKeyExists$[ebp], eax

; 187  : 
; 188  : 	CRegKey reg;

	lea	ecx, DWORD PTR _reg$[ebp]
	call	??0CRegKey@@QAE@XZ			; CRegKey::CRegKey
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 189  : 	
; 190  : 	if (reg.Open(HKEY_CLASSES_ROOT, _T("outlook")) != ERROR_SUCCESS)

	push	OFFSET FLAT:$SG75783
	push	-2147483648				; 80000000H
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open
	test	eax, eax
	je	SHORT $L75782

; 191  : 		return FALSE;

	mov	DWORD PTR $T76744[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T76744[ebp]
	jmp	$L75774
$L75782:

; 194  : 					 reg.Write(_T(""), _T("URL:Outlook Folders")) == ERROR_SUCCESS);

	push	OFFSET FLAT:$SG75786
	push	OFFSET FLAT:$SG75787
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Write@CRegKey@@QAEJPBG0@Z		; CRegKey::Write
	test	eax, eax
	jne	SHORT $L76745
	push	OFFSET FLAT:$SG75788
	push	OFFSET FLAT:$SG75789
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Write@CRegKey@@QAEJPBG0@Z		; CRegKey::Write
	test	eax, eax
	jne	SHORT $L76745
	mov	DWORD PTR -56+[ebp], 1
	jmp	SHORT $L76746
$L76745:
	mov	DWORD PTR -56+[ebp], 0
$L76746:
	mov	eax, DWORD PTR -56+[ebp]
	mov	DWORD PTR _bSuccess$[ebp], eax

; 195  : 
; 196  : 	if (bSuccess)

	cmp	DWORD PTR _bSuccess$[ebp], 0
	je	$L75798

; 198  : 		reg.Close();

	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Close@CRegKey@@QAEXXZ			; CRegKey::Close

; 199  : 
; 200  : 		if (reg.Open(HKEY_CLASSES_ROOT, _T("outlook\\shell")) == ERROR_SUCCESS)

	push	OFFSET FLAT:$SG75793
	push	-2147483648				; 80000000H
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open
	test	eax, eax
	jne	$L75798

; 202  : 			bSuccess = (reg.Write(_T(""), _T("open")) == ERROR_SUCCESS);

	push	OFFSET FLAT:$SG75794
	push	OFFSET FLAT:$SG75795
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Write@CRegKey@@QAEJPBG0@Z		; CRegKey::Write
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR _bSuccess$[ebp], eax

; 203  : 
; 204  : 			if (bSuccess)

	cmp	DWORD PTR _bSuccess$[ebp], 0
	je	$L75798

; 206  : 				reg.Close();

	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Close@CRegKey@@QAEXXZ			; CRegKey::Close

; 207  : 
; 208  : 				if (reg.Open(HKEY_CLASSES_ROOT, _T("outlook\\shell\\open\\command")) == ERROR_SUCCESS)

	push	OFFSET FLAT:$SG75799
	push	-2147483648				; 80000000H
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Open@CRegKey@@QAEJPAUHKEY__@@PBG@Z	; CRegKey::Open
	test	eax, eax
	jne	SHORT $L75798

; 210  : 					CString sOutlookPath = CFileRegister::GetRegisteredAppPath(_T("msg")); // full path

	push	0
	push	OFFSET FLAT:$SG75802
	lea	ecx, DWORD PTR _sOutlookPath$75800[ebp]
	push	ecx
	call	?GetRegisteredAppPath@CFileRegister@@SA?AVCString@@PBGH@Z ; CFileRegister::GetRegisteredAppPath
	add	esp, 12					; 0000000cH
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 211  : 					CString sCommand = sOutlookPath + _T(" /select \"%1\"");

	push	OFFSET FLAT:$SG75805
	lea	edx, DWORD PTR _sOutlookPath$75800[ebp]
	push	edx
	lea	eax, DWORD PTR _sCommand$75803[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@PBG@Z		; operator+
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 212  : 
; 213  : 					bSuccess = (reg.Write(_T(""), sCommand) == ERROR_SUCCESS);

	lea	ecx, DWORD PTR _sCommand$75803[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	push	OFFSET FLAT:$SG75806
	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Write@CRegKey@@QAEJPBG0@Z		; CRegKey::Write
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR _bSuccess$[ebp], eax

; 214  : 				}

	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sCommand$75803[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sOutlookPath$75800[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75798:

; 218  : 
; 219  : 	//[HKEY_CLASSES_ROOT\outlook\DefaultIcon]
; 220  : 	//@="C:\\PROGRA~1\\MICROS~3\\OFFICE12\\OUTLLIB.DLL,-9403"
; 221  : 	
; 222  : 	// delete key on failure
; 223  : 	reg.Close();

	lea	ecx, DWORD PTR _reg$[ebp]
	call	?Close@CRegKey@@QAEXXZ			; CRegKey::Close

; 224  : 
; 225  : 	if (!bSuccess && !bKeyExists)

	cmp	DWORD PTR _bSuccess$[ebp], 0
	jne	SHORT $L75807
	cmp	DWORD PTR _bKeyExists$[ebp], 0
	jne	SHORT $L75807

; 226  : 		CRegKey::DeleteKey(HKEY_CLASSES_ROOT, _T("outlook"));

	push	OFFSET FLAT:$SG75809
	push	-2147483648				; 80000000H
	call	?DeleteKey@CRegKey@@SAJPAUHKEY__@@PBG@Z	; CRegKey::DeleteKey
	add	esp, 8
$L75807:

; 227  : 
; 228  : 	return bSuccess;

	mov	ecx, DWORD PTR _bSuccess$[ebp]
	mov	DWORD PTR $T76747[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	mov	eax, DWORD PTR $T76747[ebp]
$L75774:

; 229  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 56					; 00000038H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76748:
	lea	ecx, DWORD PTR _reg$[ebp]
	call	??1CRegKey@@UAE@XZ			; CRegKey::~CRegKey
	ret	0
$L76749:
	lea	ecx, DWORD PTR _sOutlookPath$75800[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76750:
	lea	ecx, DWORD PTR _sCommand$75803[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76753:
	mov	eax, OFFSET FLAT:$T76752
	jmp	___CxxFrameHandler
text$x	ENDS
?InstallUrlHandler@COutlookHelper@@SAHXZ ENDP		; COutlookHelper::InstallUrlHandler
PUBLIC	?QueryInstallUrlHandler@COutlookHelper@@SAHIIH@Z ; COutlookHelper::QueryInstallUrlHandler
EXTRN	?AfxMessageBox@@YGHPBGII@Z:NEAR			; AfxMessageBox
EXTRN	??0CEnString@@QAE@IPBG@Z:NEAR			; CEnString::CEnString
EXTRN	??1CEnString@@QAE@XZ:NEAR			; CEnString::~CEnString
;	COMDAT xdata$x
; File D:\_CODE\Shared\OutlookHelper.cpp
xdata$x	SEGMENT
$T76764	DD	019930520H
	DD	01H
	DD	FLAT:$T76766
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76766	DD	0ffffffffH
	DD	FLAT:$L76762
xdata$x	ENDS
;	COMDAT ?QueryInstallUrlHandler@COutlookHelper@@SAHIIH@Z
_TEXT	SEGMENT
_nIDQuery$ = 8
_nMBOptions$ = 12
_nMBSuccess$ = 16
$T76760 = -16
$T76761 = -20
__$EHRec$ = -12
?QueryInstallUrlHandler@COutlookHelper@@SAHIIH@Z PROC NEAR ; COutlookHelper::QueryInstallUrlHandler, COMDAT

; 232  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76765
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 233  : 	if (!IsOutlookInstalled())

	call	?IsOutlookInstalled@COutlookHelper@@SAHXZ ; COutlookHelper::IsOutlookInstalled
	test	eax, eax
	jne	SHORT $L75816

; 234  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75815
$L75816:

; 235  : 
; 236  : 	if (IsUrlHandlerInstalled())

	call	?IsUrlHandlerInstalled@COutlookHelper@@SAHXZ ; COutlookHelper::IsUrlHandlerInstalled
	test	eax, eax
	je	SHORT $L75817

; 237  : 		return TRUE;

	mov	eax, 1
	jmp	SHORT $L75815
$L75817:

; 238  : 
; 239  : 	if (nIDQuery)

	cmp	DWORD PTR _nIDQuery$[ebp], 0
	je	SHORT $L75820

; 241  : 		if (AfxMessageBox(CEnString(nIDQuery), nMBOptions) != nMBSuccess)

	push	0
	mov	eax, DWORD PTR _nMBOptions$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _nIDQuery$[ebp]
	push	ecx
	lea	ecx, DWORD PTR $T76761[ebp]
	call	??0CEnString@@QAE@IPBG@Z		; CEnString::CEnString
	mov	DWORD PTR -24+[ebp], eax
	mov	edx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -28+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?AfxMessageBox@@YGHPBGII@Z		; AfxMessageBox
	xor	ecx, ecx
	cmp	eax, DWORD PTR _nMBSuccess$[ebp]
	setne	cl
	mov	BYTE PTR $T76760[ebp], cl
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76761[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	mov	edx, DWORD PTR $T76760[ebp]
	and	edx, 255				; 000000ffH
	test	edx, edx
	je	SHORT $L75820

; 242  : 			return FALSE;

	xor	eax, eax
	jmp	SHORT $L75815
$L75820:

; 244  : 
; 245  : 	return InstallUrlHandler();

	call	?InstallUrlHandler@COutlookHelper@@SAHXZ ; COutlookHelper::InstallUrlHandler
$L75815:

; 246  : }

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
$L76762:
	lea	ecx, DWORD PTR $T76761[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	ret	0
$L76765:
	mov	eax, OFFSET FLAT:$T76764
	jmp	___CxxFrameHandler
text$x	ENDS
?QueryInstallUrlHandler@COutlookHelper@@SAHIIH@Z ENDP	; COutlookHelper::QueryInstallUrlHandler
PUBLIC	?GetItemData@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@W4OUTLOOK_FIELDTYPE@@@Z ; COutlookHelper::GetItemData
PUBLIC	?GetItemID@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@@Z ; COutlookHelper::GetItemID
PUBLIC	?FormatItemAsUrl@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@K@Z ; COutlookHelper::FormatItemAsUrl
PUBLIC	??0MAPIFolder@OutlookAPI@@QAE@PAUIDispatch@@@Z	; OutlookAPI::MAPIFolder::MAPIFolder
PUBLIC	??1MAPIFolder@OutlookAPI@@QAE@XZ		; OutlookAPI::MAPIFolder::~MAPIFolder
EXTRN	?GetFolderPath@MAPIFolder@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::MAPIFolder::GetFolderPath
EXTRN	?GetParent@_MailItem@OutlookAPI@@QAEPAUIDispatch@@XZ:NEAR ; OutlookAPI::_MailItem::GetParent
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	??ACString@@QBEGH@Z:NEAR			; CString::operator[]
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
EXTRN	??H@YG?AVCString@@ABV0@D@Z:NEAR			; operator+
EXTRN	??H@YG?AVCString@@DABV0@@Z:NEAR			; operator+
EXTRN	?Replace@CString@@QAEHPBG0@Z:NEAR		; CString::Replace
EXTRN	?Find@CString@@QBEHGH@Z:NEAR			; CString::Find
EXTRN	?Format@CString@@QAAXPBGZZ:NEAR			; CString::Format
;	COMDAT data
; File D:\_CODE\Shared\OutlookHelper.cpp
data	SEGMENT
$SG75839 DB	'%', 00H, 's', 00H, '\', 00H, '~', 00H, '%', 00H, 's', 00H
	DB	00H, 00H
	ORG $+2
$SG75841 DB	'o', 00H, 'u', 00H, 't', 00H, 'l', 00H, 'o', 00H, 'o', 00H
	DB	'k', 00H, ':', 00H, '%', 00H, 's', 00H, 00H, 00H
	ORG $+2
$SG75847 DB	'%', 00H, '2', 00H, '0', 00H, 00H, 00H
$SG75848 DB	' ', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76785	DD	019930520H
	DD	09H
	DD	FLAT:$T76787
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76787	DD	0ffffffffH
	DD	FLAT:$L76781
	DD	00H
	DD	FLAT:$L76774
	DD	01H
	DD	FLAT:$L76775
	DD	02H
	DD	FLAT:$L76776
	DD	01H
	DD	FLAT:$L76776
	DD	04H
	DD	FLAT:$L76777
	DD	01H
	DD	FLAT:$L76778
	DD	06H
	DD	FLAT:$L76779
	DD	07H
	DD	FLAT:$L76780
xdata$x	ENDS
;	COMDAT ?FormatItemAsUrl@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@K@Z
_TEXT	SEGMENT
_obj$ = 12
_dwFlags$ = 16
___$ReturnUdt$ = 8
_sPath$ = -16
_lpParent$75830 = -24
_sFolder$75832 = -28
_sSubject$75835 = -32
_sUrl$ = -20
$T76771 = -40
$T76772 = -44
$T76773 = -48
$T76782 = -52
__$EHRec$ = -12
?FormatItemAsUrl@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@K@Z PROC NEAR ; COutlookHelper::FormatItemAsUrl, COMDAT

; 249  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76786
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 64					; 00000040H
	push	edi
	lea	edi, DWORD PTR [ebp-76]
	mov	ecx, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T76782[ebp], 0

; 250  : 	CString sPath(GetItemID(obj)); // default

	mov	eax, DWORD PTR _obj$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sPath$[ebp]
	push	ecx
	call	?GetItemID@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@@Z ; COutlookHelper::GetItemID
	add	esp, 8
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 251  : 
; 252  : 	if (dwFlags & OAFMT_NICE)

	mov	edx, DWORD PTR _dwFlags$[ebp]
	and	edx, 1
	test	edx, edx
	je	$L75831

; 254  : 		// get the parent path
; 255  : 		LPDISPATCH lpParent = obj.GetParent();

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetParent@_MailItem@OutlookAPI@@QAEPAUIDispatch@@XZ ; OutlookAPI::_MailItem::GetParent
	mov	DWORD PTR _lpParent$75830[ebp], eax

; 256  : 		
; 257  : 		if (lpParent)

	cmp	DWORD PTR _lpParent$75830[ebp], 0
	je	$L75831

; 259  : 			CString sFolder = MAPIFolder(lpParent).GetFolderPath();

	lea	eax, DWORD PTR _sFolder$75832[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpParent$75830[ebp]
	push	ecx
	lea	ecx, DWORD PTR $T76771[ebp]
	call	??0MAPIFolder@OutlookAPI@@QAE@PAUIDispatch@@@Z ; OutlookAPI::MAPIFolder::MAPIFolder
	mov	DWORD PTR -56+[ebp], eax
	mov	edx, DWORD PTR -56+[ebp]
	mov	DWORD PTR -60+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR -60+[ebp]
	call	?GetFolderPath@MAPIFolder@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::MAPIFolder::GetFolderPath
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR $T76771[ebp]
	call	??1MAPIFolder@OutlookAPI@@QAE@XZ	; OutlookAPI::MAPIFolder::~MAPIFolder

; 260  : 			CString sSubject = GetItemData(obj, OA_SUBJECT);

	push	25					; 00000019H
	mov	eax, DWORD PTR _obj$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sSubject$75835[ebp]
	push	ecx
	call	?GetItemData@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@W4OUTLOOK_FIELDTYPE@@@Z ; COutlookHelper::GetItemData
	add	esp, 12					; 0000000cH
	mov	BYTE PTR __$EHRec$[ebp+8], 5

; 261  : 
; 262  : 			// very odd bug if subject starts with two chars 
; 263  : 			// and then a colon -> Outlook fails to open the link
; 264  : 			// so we handle this specific issue by falling back on the itemID
; 265  : 			if ((sSubject.GetLength() <= 2) || (sSubject[2] != ':'))

	lea	ecx, DWORD PTR _sSubject$75835[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	cmp	eax, 2
	jle	SHORT $L75838
	push	2
	lea	ecx, DWORD PTR _sSubject$75835[ebp]
	call	??ACString@@QBEGH@Z			; CString::operator[]
	and	eax, 65535				; 0000ffffH
	cmp	eax, 58					; 0000003aH
	je	SHORT $L75837
$L75838:

; 267  : 				sPath.Format(_T("%s\\~%s"), sFolder, sSubject);

	mov	edx, DWORD PTR _sSubject$75835[ebp]
	push	edx
	mov	eax, DWORD PTR _sFolder$75832[ebp]
	push	eax
	push	OFFSET FLAT:$SG75839
	lea	ecx, DWORD PTR _sPath$[ebp]
	push	ecx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 16					; 00000010H
$L75837:

; 269  : 			// else use itemID from above
; 270  : 		}

	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR _sSubject$75835[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sFolder$75832[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75831:

; 272  : 
; 273  : 	CString sUrl;

	lea	ecx, DWORD PTR _sUrl$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 6

; 274  : 	sUrl.Format(_T("outlook:%s"), sPath);

	mov	edx, DWORD PTR _sPath$[ebp]
	push	edx
	push	OFFSET FLAT:$SG75841
	lea	eax, DWORD PTR _sUrl$[ebp]
	push	eax
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 12					; 0000000cH

; 275  : 
; 276  : 	if ((dwFlags & OAFMT_BRACED) && (sUrl.Find(' ', 0) > 0))

	mov	ecx, DWORD PTR _dwFlags$[ebp]
	and	ecx, 2
	test	ecx, ecx
	je	SHORT $L75842
	push	0
	push	32					; 00000020H
	lea	ecx, DWORD PTR _sUrl$[ebp]
	call	?Find@CString@@QBEHGH@Z			; CString::Find
	test	eax, eax
	jle	SHORT $L75842

; 278  : 		(sUrl = '<' + sUrl + '>');

	push	62					; 0000003eH
	lea	edx, DWORD PTR _sUrl$[ebp]
	push	edx
	push	60					; 0000003cH
	lea	eax, DWORD PTR $T76772[ebp]
	push	eax
	call	??H@YG?AVCString@@DABV0@@Z		; operator+
	mov	DWORD PTR -64+[ebp], eax
	mov	ecx, DWORD PTR -64+[ebp]
	mov	DWORD PTR -68+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	mov	edx, DWORD PTR -68+[ebp]
	push	edx
	lea	eax, DWORD PTR $T76773[ebp]
	push	eax
	call	??H@YG?AVCString@@ABV0@D@Z		; operator+
	mov	DWORD PTR -72+[ebp], eax
	mov	ecx, DWORD PTR -72+[ebp]
	mov	DWORD PTR -76+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 8
	mov	edx, DWORD PTR -76+[ebp]
	push	edx
	lea	ecx, DWORD PTR _sUrl$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+8], 7
	lea	ecx, DWORD PTR $T76773[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 6
	lea	ecx, DWORD PTR $T76772[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 280  : 	else if (dwFlags & OAFMT_REPLACESPACES)

	jmp	SHORT $L75846
$L75842:
	mov	eax, DWORD PTR _dwFlags$[ebp]
	and	eax, 4
	test	eax, eax
	je	SHORT $L75846

; 282  : 		sUrl.Replace(_T(" "), _T("%20"));

	push	OFFSET FLAT:$SG75847
	push	OFFSET FLAT:$SG75848
	lea	ecx, DWORD PTR _sUrl$[ebp]
	call	?Replace@CString@@QAEHPBG0@Z		; CString::Replace
$L75846:

; 284  : 
; 285  : 	return sUrl;

	lea	ecx, DWORD PTR _sUrl$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76782[ebp]
	or	edx, 1
	mov	DWORD PTR $T76782[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sUrl$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 286  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 76					; 0000004cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76774:
	lea	ecx, DWORD PTR _sPath$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76775:
	lea	ecx, DWORD PTR $T76771[ebp]
	call	??1MAPIFolder@OutlookAPI@@QAE@XZ	; OutlookAPI::MAPIFolder::~MAPIFolder
	ret	0
$L76776:
	lea	ecx, DWORD PTR _sFolder$75832[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76777:
	lea	ecx, DWORD PTR _sSubject$75835[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76778:
	lea	ecx, DWORD PTR _sUrl$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76779:
	lea	ecx, DWORD PTR $T76772[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76780:
	lea	ecx, DWORD PTR $T76773[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76781:
	mov	eax, DWORD PTR $T76782[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76783
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76783:
	ret	0
$L76786:
	mov	eax, OFFSET FLAT:$T76785
	jmp	___CxxFrameHandler
text$x	ENDS
?FormatItemAsUrl@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@K@Z ENDP ; COutlookHelper::FormatItemAsUrl
EXTRN	??0COleDispatchDriver@@QAE@PAUIDispatch@@H@Z:NEAR ; COleDispatchDriver::COleDispatchDriver
;	COMDAT ??0MAPIFolder@OutlookAPI@@QAE@PAUIDispatch@@@Z
_TEXT	SEGMENT
_pDispatch$ = 8
_this$ = -4
??0MAPIFolder@OutlookAPI@@QAE@PAUIDispatch@@@Z PROC NEAR ; OutlookAPI::MAPIFolder::MAPIFolder, COMDAT

; 2040 : 	MAPIFolder(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	1
	mov	eax, DWORD PTR _pDispatch$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0COleDispatchDriver@@QAE@PAUIDispatch@@H@Z ; COleDispatchDriver::COleDispatchDriver
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0MAPIFolder@OutlookAPI@@QAE@PAUIDispatch@@@Z ENDP	; OutlookAPI::MAPIFolder::MAPIFolder
_TEXT	ENDS
;	COMDAT ??1MAPIFolder@OutlookAPI@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1MAPIFolder@OutlookAPI@@QAE@XZ PROC NEAR		; OutlookAPI::MAPIFolder::~MAPIFolder, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1COleDispatchDriver@@QAE@XZ		; COleDispatchDriver::~COleDispatchDriver
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1MAPIFolder@OutlookAPI@@QAE@XZ ENDP			; OutlookAPI::MAPIFolder::~MAPIFolder
_TEXT	ENDS
PUBLIC	?IsConfidential@COutlookHelper@@SAHW4OUTLOOK_FIELDTYPE@@@Z ; COutlookHelper::IsConfidential
PUBLIC	?MapDate@COutlookHelper@@KA?AVCString@@N@Z	; COutlookHelper::MapDate
EXTRN	__fltused:NEAR
EXTRN	??0CEnString@@QAA@PBGZZ:NEAR			; CEnString::CEnString
EXTRN	?GetBillingInformation@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetBillingInformation
EXTRN	?GetBody@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetBody
EXTRN	?GetCategories@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetCategories
EXTRN	?GetCompanies@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetCompanies
EXTRN	?GetConversationTopic@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetConversationTopic
EXTRN	?GetCreationTime@_MailItem@OutlookAPI@@QAENXZ:NEAR ; OutlookAPI::_MailItem::GetCreationTime
EXTRN	?GetEntryID@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetEntryID
EXTRN	?GetImportance@_MailItem@OutlookAPI@@QAEJXZ:NEAR ; OutlookAPI::_MailItem::GetImportance
EXTRN	?GetLastModificationTime@_MailItem@OutlookAPI@@QAENXZ:NEAR ; OutlookAPI::_MailItem::GetLastModificationTime
EXTRN	?GetMessageClass@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetMessageClass
EXTRN	?GetMileage@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetMileage
EXTRN	?GetSensitivity@_MailItem@OutlookAPI@@QAEJXZ:NEAR ; OutlookAPI::_MailItem::GetSensitivity
EXTRN	?GetSubject@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetSubject
EXTRN	?GetBcc@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetBcc
EXTRN	?GetCc@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetCc
EXTRN	?GetExpiryTime@_MailItem@OutlookAPI@@QAENXZ:NEAR ; OutlookAPI::_MailItem::GetExpiryTime
EXTRN	?GetFlagRequest@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetFlagRequest
EXTRN	?GetReceivedByName@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetReceivedByName
EXTRN	?GetReceivedTime@_MailItem@OutlookAPI@@QAENXZ:NEAR ; OutlookAPI::_MailItem::GetReceivedTime
EXTRN	?GetReminderTime@_MailItem@OutlookAPI@@QAENXZ:NEAR ; OutlookAPI::_MailItem::GetReminderTime
EXTRN	?GetReplyRecipientNames@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetReplyRecipientNames
EXTRN	?GetSenderName@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetSenderName
EXTRN	?GetSentOn@_MailItem@OutlookAPI@@QAENXZ:NEAR	; OutlookAPI::_MailItem::GetSentOn
EXTRN	?GetSentOnBehalfOfName@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetSentOnBehalfOfName
EXTRN	?GetTo@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetTo
EXTRN	?GetSenderEmailAddress@_MailItem@OutlookAPI@@QAE?AVCString@@XZ:NEAR ; OutlookAPI::_MailItem::GetSenderEmailAddress
EXTRN	?GetPermission@_MailItem@OutlookAPI@@QAEJXZ:NEAR ; OutlookAPI::_MailItem::GetPermission
EXTRN	?GetTaskDueDate@_MailItem@OutlookAPI@@QAENXZ:NEAR ; OutlookAPI::_MailItem::GetTaskDueDate
EXTRN	?GetTaskStartDate@_MailItem@OutlookAPI@@QAENXZ:NEAR ; OutlookAPI::_MailItem::GetTaskStartDate
EXTRN	?GetTaskCompletedDate@_MailItem@OutlookAPI@@QAENXZ:NEAR ; OutlookAPI::_MailItem::GetTaskCompletedDate
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
_BSS	SEGMENT
	ALIGN	4

$SG75857 DW	01H DUP (?)
	ALIGN	4

$SG75929 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT data
; File D:\_CODE\Shared\OutlookHelper.cpp
data	SEGMENT
$SG75886 DB	'%', 00H, 'd', 00H, 00H, 00H
	ORG $+2
$SG75895 DB	'%', 00H, 'd', 00H, 00H, 00H
	ORG $+2
$SG75910 DB	'%', 00H, 'd', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76809	DD	019930520H
	DD	03H
	DD	FLAT:$T76813
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76813	DD	0ffffffffH
	DD	FLAT:$L76805
	DD	0ffffffffH
	DD	FLAT:$L76806
	DD	0ffffffffH
	DD	FLAT:$L76807
xdata$x	ENDS
;	COMDAT ?GetItemData@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@W4OUTLOOK_FIELDTYPE@@@Z
_TEXT	SEGMENT
$T76800 = -20
$T76801 = -24
$T76803 = -28
__$EHRec$ = -12
_obj$ = 12
_nField$ = 16
___$ReturnUdt$ = 8
$T76799 = -16
?GetItemData@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@W4OUTLOOK_FIELDTYPE@@@Z PROC NEAR ; COutlookHelper::GetItemData, COMDAT

; 289  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76810
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 44					; 0000002cH
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-56]
	mov	ecx, 11					; 0000000bH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T76803[ebp], 0

; 290  : 	if (IsConfidential(nField) && s_bDenyConfidential)

	mov	eax, DWORD PTR _nField$[ebp]
	push	eax
	call	?IsConfidential@COutlookHelper@@SAHW4OUTLOOK_FIELDTYPE@@@Z ; COutlookHelper::IsConfidential
	add	esp, 4
	test	eax, eax
	je	SHORT $L75856
	cmp	DWORD PTR ?s_bDenyConfidential@COutlookHelper@@1HA, 0 ; COutlookHelper::s_bDenyConfidential
	je	SHORT $L75856

; 291  : 		return _T("");

	push	OFFSET FLAT:$SG75857
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	ecx, DWORD PTR $T76803[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76803[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75856:

; 294  : 	{

	mov	edx, DWORD PTR _nField$[ebp]
	mov	DWORD PTR -32+[ebp], edx
	cmp	DWORD PTR -32+[ebp], 29			; 0000001dH
	ja	$L75859
	mov	eax, DWORD PTR -32+[ebp]
	jmp	DWORD PTR $L76811[eax*4]
$L75862:

; 295  : 		case OA_BCC:					return obj.GetBcc();

	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetBcc@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetBcc
	mov	edx, DWORD PTR $T76803[ebp]
	or	edx, 1
	mov	DWORD PTR $T76803[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75864:

; 296  : 		case OA_BILLINGINFORMATION:		return obj.GetBillingInformation();

	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetBillingInformation@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetBillingInformation
	mov	ecx, DWORD PTR $T76803[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76803[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75866:

; 297  : 		case OA_BODY:					return obj.GetBody();

	mov	edx, DWORD PTR ___$ReturnUdt$[ebp]
	push	edx
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetBody@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetBody
	mov	eax, DWORD PTR $T76803[ebp]
	or	al, 1
	mov	DWORD PTR $T76803[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75868:

; 298  : 		case OA_CATEGORIES:				return obj.GetCategories();

	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetCategories@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetCategories
	mov	edx, DWORD PTR $T76803[ebp]
	or	edx, 1
	mov	DWORD PTR $T76803[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75870:

; 299  : 		case OA_CC:						return obj.GetCc();

	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetCc@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetCc
	mov	ecx, DWORD PTR $T76803[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76803[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75872:

; 300  : 		case OA_COMPANIES:				return obj.GetCompanies();

	mov	edx, DWORD PTR ___$ReturnUdt$[ebp]
	push	edx
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetCompanies@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetCompanies
	mov	eax, DWORD PTR $T76803[ebp]
	or	al, 1
	mov	DWORD PTR $T76803[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75874:

; 301  : 		case OA_CONVERSATIONTOPIC:		return obj.GetConversationTopic();

	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetConversationTopic@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetConversationTopic
	mov	edx, DWORD PTR $T76803[ebp]
	or	edx, 1
	mov	DWORD PTR $T76803[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75876:

; 302  : 		case OA_CREATIONTIME:			return MapDate(obj.GetCreationTime());

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetCreationTime@_MailItem@OutlookAPI@@QAENXZ ; OutlookAPI::_MailItem::GetCreationTime
	sub	esp, 8
	fstp	QWORD PTR [esp]
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	call	?MapDate@COutlookHelper@@KA?AVCString@@N@Z ; COutlookHelper::MapDate
	add	esp, 12					; 0000000cH
	mov	ecx, DWORD PTR $T76803[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76803[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75878:

; 303  : 		case OA_ENTRYID:				return obj.GetEntryID();

	mov	edx, DWORD PTR ___$ReturnUdt$[ebp]
	push	edx
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetEntryID@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetEntryID
	mov	eax, DWORD PTR $T76803[ebp]
	or	al, 1
	mov	DWORD PTR $T76803[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75880:

; 304  : 		case OA_EXPIRYTIME:				return MapDate(obj.GetExpiryTime());

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetExpiryTime@_MailItem@OutlookAPI@@QAENXZ ; OutlookAPI::_MailItem::GetExpiryTime
	sub	esp, 8
	fstp	QWORD PTR [esp]
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	call	?MapDate@COutlookHelper@@KA?AVCString@@N@Z ; COutlookHelper::MapDate
	add	esp, 12					; 0000000cH
	mov	edx, DWORD PTR $T76803[ebp]
	or	edx, 1
	mov	DWORD PTR $T76803[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75882:

; 305  : 		case OA_FLAGREQUEST:			return obj.GetFlagRequest();

	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetFlagRequest@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetFlagRequest
	mov	ecx, DWORD PTR $T76803[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76803[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75884:

; 306  : 		case OA_IMPORTANCE:				return CEnString(_T("%d"), obj.GetImportance());

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetImportance@_MailItem@OutlookAPI@@QAEJXZ ; OutlookAPI::_MailItem::GetImportance
	push	eax
	push	OFFSET FLAT:$SG75886
	lea	edx, DWORD PTR $T76799[ebp]
	push	edx
	call	??0CEnString@@QAA@PBGZZ			; CEnString::CEnString
	add	esp, 12					; 0000000cH
	mov	DWORD PTR -36+[ebp], eax
	mov	eax, DWORD PTR -36+[ebp]
	mov	DWORD PTR -40+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -40+[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76803[ebp]
	or	edx, 1
	mov	DWORD PTR $T76803[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76799[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75887:

; 307  : 		case OA_LASTMODIFICATIONTIME:	return MapDate(obj.GetLastModificationTime());

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetLastModificationTime@_MailItem@OutlookAPI@@QAENXZ ; OutlookAPI::_MailItem::GetLastModificationTime
	sub	esp, 8
	fstp	QWORD PTR [esp]
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	call	?MapDate@COutlookHelper@@KA?AVCString@@N@Z ; COutlookHelper::MapDate
	add	esp, 12					; 0000000cH
	mov	ecx, DWORD PTR $T76803[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76803[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75889:

; 308  : 		case OA_MESSAGECLASS:			return obj.GetMessageClass();

	mov	edx, DWORD PTR ___$ReturnUdt$[ebp]
	push	edx
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetMessageClass@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetMessageClass
	mov	eax, DWORD PTR $T76803[ebp]
	or	al, 1
	mov	DWORD PTR $T76803[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75891:

; 309  : 		case OA_MILEAGE:				return obj.GetMileage();

	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetMileage@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetMileage
	mov	edx, DWORD PTR $T76803[ebp]
	or	edx, 1
	mov	DWORD PTR $T76803[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75893:

; 310  : 		case OA_PERMISSION:				return CEnString(_T("%d"), obj.GetPermission());

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetPermission@_MailItem@OutlookAPI@@QAEJXZ ; OutlookAPI::_MailItem::GetPermission
	push	eax
	push	OFFSET FLAT:$SG75895
	lea	eax, DWORD PTR $T76800[ebp]
	push	eax
	call	??0CEnString@@QAA@PBGZZ			; CEnString::CEnString
	add	esp, 12					; 0000000cH
	mov	DWORD PTR -44+[ebp], eax
	mov	ecx, DWORD PTR -44+[ebp]
	mov	DWORD PTR -48+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	mov	edx, DWORD PTR -48+[ebp]
	push	edx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	eax, DWORD PTR $T76803[ebp]
	or	al, 1
	mov	DWORD PTR $T76803[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76800[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75896:

; 311  : 		case OA_RECEIVEDBYNAME:			return obj.GetReceivedByName();

	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetReceivedByName@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetReceivedByName
	mov	edx, DWORD PTR $T76803[ebp]
	or	edx, 1
	mov	DWORD PTR $T76803[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75898:

; 312  : 		case OA_RECEIVEDTIME:			return MapDate(obj.GetReceivedTime());

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetReceivedTime@_MailItem@OutlookAPI@@QAENXZ ; OutlookAPI::_MailItem::GetReceivedTime
	sub	esp, 8
	fstp	QWORD PTR [esp]
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	call	?MapDate@COutlookHelper@@KA?AVCString@@N@Z ; COutlookHelper::MapDate
	add	esp, 12					; 0000000cH
	mov	ecx, DWORD PTR $T76803[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76803[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75900:

; 313  : 		case OA_REMINDERTIME:			return MapDate(obj.GetReminderTime());

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetReminderTime@_MailItem@OutlookAPI@@QAENXZ ; OutlookAPI::_MailItem::GetReminderTime
	sub	esp, 8
	fstp	QWORD PTR [esp]
	mov	edx, DWORD PTR ___$ReturnUdt$[ebp]
	push	edx
	call	?MapDate@COutlookHelper@@KA?AVCString@@N@Z ; COutlookHelper::MapDate
	add	esp, 12					; 0000000cH
	mov	eax, DWORD PTR $T76803[ebp]
	or	al, 1
	mov	DWORD PTR $T76803[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75902:

; 314  : 		case OA_REPLYRECIPIENTNAMES:	return obj.GetReplyRecipientNames();

	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetReplyRecipientNames@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetReplyRecipientNames
	mov	edx, DWORD PTR $T76803[ebp]
	or	edx, 1
	mov	DWORD PTR $T76803[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75904:

; 315  : 		case OA_SENDEREMAILADDRESS:		return obj.GetSenderEmailAddress();

	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetSenderEmailAddress@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetSenderEmailAddress
	mov	ecx, DWORD PTR $T76803[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76803[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75906:

; 316  : 		case OA_SENDERNAME:				return obj.GetSenderName();

	mov	edx, DWORD PTR ___$ReturnUdt$[ebp]
	push	edx
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetSenderName@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetSenderName
	mov	eax, DWORD PTR $T76803[ebp]
	or	al, 1
	mov	DWORD PTR $T76803[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75908:

; 317  : 		case OA_SENSITIVITY:			return CEnString(_T("%d"), obj.GetSensitivity());

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetSensitivity@_MailItem@OutlookAPI@@QAEJXZ ; OutlookAPI::_MailItem::GetSensitivity
	push	eax
	push	OFFSET FLAT:$SG75910
	lea	ecx, DWORD PTR $T76801[ebp]
	push	ecx
	call	??0CEnString@@QAA@PBGZZ			; CEnString::CEnString
	add	esp, 12					; 0000000cH
	mov	DWORD PTR -52+[ebp], eax
	mov	edx, DWORD PTR -52+[ebp]
	mov	DWORD PTR -56+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 2
	mov	eax, DWORD PTR -56+[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T76803[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76803[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76801[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75911:

; 318  : 		case OA_SENTONBEHALFOFNAME:		return obj.GetSentOnBehalfOfName();

	mov	edx, DWORD PTR ___$ReturnUdt$[ebp]
	push	edx
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetSentOnBehalfOfName@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetSentOnBehalfOfName
	mov	eax, DWORD PTR $T76803[ebp]
	or	al, 1
	mov	DWORD PTR $T76803[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75913:

; 319  : 		case OA_SENTON:					return MapDate(obj.GetSentOn());

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetSentOn@_MailItem@OutlookAPI@@QAENXZ	; OutlookAPI::_MailItem::GetSentOn
	sub	esp, 8
	fstp	QWORD PTR [esp]
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	call	?MapDate@COutlookHelper@@KA?AVCString@@N@Z ; COutlookHelper::MapDate
	add	esp, 12					; 0000000cH
	mov	edx, DWORD PTR $T76803[ebp]
	or	edx, 1
	mov	DWORD PTR $T76803[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75915:

; 320  : 		case OA_SUBJECT:				return obj.GetSubject();

	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetSubject@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetSubject
	mov	ecx, DWORD PTR $T76803[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76803[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75917:

; 321  : 		case OA_TASKCOMPLETEDDATE:		return MapDate(obj.GetTaskCompletedDate());

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetTaskCompletedDate@_MailItem@OutlookAPI@@QAENXZ ; OutlookAPI::_MailItem::GetTaskCompletedDate
	sub	esp, 8
	fstp	QWORD PTR [esp]
	mov	edx, DWORD PTR ___$ReturnUdt$[ebp]
	push	edx
	call	?MapDate@COutlookHelper@@KA?AVCString@@N@Z ; COutlookHelper::MapDate
	add	esp, 12					; 0000000cH
	mov	eax, DWORD PTR $T76803[ebp]
	or	al, 1
	mov	DWORD PTR $T76803[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	$L75855
$L75919:

; 322  : 		case OA_TASKDUEDATE:			return MapDate(obj.GetTaskDueDate());

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetTaskDueDate@_MailItem@OutlookAPI@@QAENXZ ; OutlookAPI::_MailItem::GetTaskDueDate
	sub	esp, 8
	fstp	QWORD PTR [esp]
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	call	?MapDate@COutlookHelper@@KA?AVCString@@N@Z ; COutlookHelper::MapDate
	add	esp, 12					; 0000000cH
	mov	edx, DWORD PTR $T76803[ebp]
	or	edx, 1
	mov	DWORD PTR $T76803[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L75855
$L75921:

; 323  : 		case OA_TASKSTARTDATE:			return MapDate(obj.GetTaskStartDate());

	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetTaskStartDate@_MailItem@OutlookAPI@@QAENXZ ; OutlookAPI::_MailItem::GetTaskStartDate
	sub	esp, 8
	fstp	QWORD PTR [esp]
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	call	?MapDate@COutlookHelper@@KA?AVCString@@N@Z ; COutlookHelper::MapDate
	add	esp, 12					; 0000000cH
	mov	ecx, DWORD PTR $T76803[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76803[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L75855
$L75923:

; 324  : //		case OA_TASKSUBJECT:			return obj.GetTaskSubject();
; 325  : 		case OA_TO:						return obj.GetTo();

	mov	edx, DWORD PTR ___$ReturnUdt$[ebp]
	push	edx
	mov	ecx, DWORD PTR _obj$[ebp]
	call	?GetTo@_MailItem@OutlookAPI@@QAE?AVCString@@XZ ; OutlookAPI::_MailItem::GetTo
	mov	eax, DWORD PTR $T76803[ebp]
	or	al, 1
	mov	DWORD PTR $T76803[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L75855
$L75859:

; 327  : 
; 328  : 	ASSERT(0);

	mov	ecx, 1
	test	ecx, ecx
	je	SHORT $L75928
	push	328					; 00000148H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75928
	int	3
$L75928:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75859

; 329  : 	return _T("");

	push	OFFSET FLAT:$SG75929
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	eax, DWORD PTR $T76803[ebp]
	or	al, 1
	mov	DWORD PTR $T76803[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L75855:

; 330  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 56					; 00000038H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
$L76811:
	DD	$L75862
	DD	$L75864
	DD	$L75866
	DD	$L75868
	DD	$L75870
	DD	$L75872
	DD	$L75874
	DD	$L75876
	DD	$L75878
	DD	$L75880
	DD	$L75882
	DD	$L75884
	DD	$L75887
	DD	$L75889
	DD	$L75891
	DD	$L75893
	DD	$L75896
	DD	$L75898
	DD	$L75900
	DD	$L75902
	DD	$L75904
	DD	$L75906
	DD	$L75908
	DD	$L75913
	DD	$L75911
	DD	$L75915
	DD	$L75917
	DD	$L75919
	DD	$L75921
	DD	$L75923
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76805:
	lea	ecx, DWORD PTR $T76799[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	ret	0
$L76806:
	lea	ecx, DWORD PTR $T76800[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	ret	0
$L76807:
	lea	ecx, DWORD PTR $T76801[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	ret	0
$L76810:
	mov	eax, OFFSET FLAT:$T76809
	jmp	___CxxFrameHandler
text$x	ENDS
?GetItemData@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@W4OUTLOOK_FIELDTYPE@@@Z ENDP ; COutlookHelper::GetItemData
;	COMDAT ?GetItemID@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@@Z
_TEXT	SEGMENT
$T76820 = -4
_obj$ = 12
___$ReturnUdt$ = 8
?GetItemID@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@@Z PROC NEAR ; COutlookHelper::GetItemID, COMDAT

; 333  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T76820[ebp], 0

; 334  : 	return GetItemData(obj, OA_ENTRYID);

	push	8
	mov	eax, DWORD PTR _obj$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	call	?GetItemData@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@W4OUTLOOK_FIELDTYPE@@@Z ; COutlookHelper::GetItemData
	add	esp, 12					; 0000000cH
	mov	edx, DWORD PTR $T76820[ebp]
	or	edx, 1
	mov	DWORD PTR $T76820[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 335  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetItemID@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@@Z ENDP ; COutlookHelper::GetItemID
_TEXT	ENDS
PUBLIC	?GetItemClass@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@@Z ; COutlookHelper::GetItemClass
;	COMDAT ?GetItemClass@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@@Z
_TEXT	SEGMENT
$T76825 = -4
_obj$ = 12
___$ReturnUdt$ = 8
?GetItemClass@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@@Z PROC NEAR ; COutlookHelper::GetItemClass, COMDAT

; 338  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR $T76825[ebp], 0

; 339  : 	return GetItemData(obj, OA_MESSAGECLASS);

	push	13					; 0000000dH
	mov	eax, DWORD PTR _obj$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	push	ecx
	call	?GetItemData@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@W4OUTLOOK_FIELDTYPE@@@Z ; COutlookHelper::GetItemData
	add	esp, 12					; 0000000cH
	mov	edx, DWORD PTR $T76825[ebp]
	or	edx, 1
	mov	DWORD PTR $T76825[ebp], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 340  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetItemClass@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@@Z ENDP ; COutlookHelper::GetItemClass
_TEXT	ENDS
PUBLIC	??A?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEAAVCString@@W4OUTLOOK_FIELDTYPE@@@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::operator[]
PUBLIC	??_R0PAVCOleException@@@8			; COleException * `RTTI Type Descriptor'
PUBLIC	?GetItemData@COutlookHelper@@SAHAAV_MailItem@OutlookAPI@@AAVCOutlookItemDataMap@@H@Z ; COutlookHelper::GetItemData
EXTRN	?Delete@CException@@QAEXXZ:NEAR			; CException::Delete
EXTRN	??_7type_info@@6B@:BYTE				; type_info::`vftable'
;	COMDAT ??_R0PAVCOleException@@@8
; File D:\_CODE\Shared\OutlookHelper.cpp
_DATA	SEGMENT
??_R0PAVCOleException@@@8 DD FLAT:??_7type_info@@6B@	; COleException * `RTTI Type Descriptor'
	DD	00H
	DB	'.PAVCOleException@@', 00H
_DATA	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76835	DD	019930520H
	DD	03H
	DD	FLAT:$T76837
	DD	01H
	DD	FLAT:$T76838
	DD	2 DUP(00H)
	ORG $+4
$T76837	DD	0ffffffffH
	DD	00H
	DD	00H
	DD	FLAT:$L76833
	DD	0ffffffffH
	DD	00H
$T76838	DD	00H
	DD	01H
	DD	02H
	DD	02H
	DD	FLAT:$T76839
	ORG $+4
$T76839	DD	00H
	DD	FLAT:??_R0PAVCOleException@@@8
	DD	0ffffffe4H
	DD	FLAT:$L76831
	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L76832
xdata$x	ENDS
;	COMDAT ?GetItemData@COutlookHelper@@SAHAAV_MailItem@OutlookAPI@@AAVCOutlookItemDataMap@@H@Z
_TEXT	SEGMENT
$T76829 = -32
__$EHRec$ = -16
_obj$ = 8
_mapData$ = 12
_bIncludeConfidential$ = 16
_bDenyConfidential$ = -20
_nField$75948 = -24
_e$75956 = -28
?GetItemData@COutlookHelper@@SAHAAV_MailItem@OutlookAPI@@AAVCOutlookItemDataMap@@H@Z PROC NEAR ; COutlookHelper::GetItemData, COMDAT

; 344  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76836
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	sub	esp, 28					; 0000001cH
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR __$EHRec$[ebp], esp
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-44], eax
	mov	DWORD PTR [ebp-40], eax
	mov	DWORD PTR [ebp-36], eax
	mov	DWORD PTR [ebp-32], eax
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax

; 345  : 	mapData.RemoveAll();

	mov	ecx, DWORD PTR _mapData$[ebp]
	call	?RemoveAll@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXXZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::RemoveAll

; 346  : 
; 347  : 	// temporarily override confidentiality only if bIncludeConfidential is FALSE
; 348  : 	BOOL bDenyConfidential = s_bDenyConfidential;

	mov	eax, DWORD PTR ?s_bDenyConfidential@COutlookHelper@@1HA ; COutlookHelper::s_bDenyConfidential
	mov	DWORD PTR _bDenyConfidential$[ebp], eax

; 349  : 
; 350  : 	if (!s_bDenyConfidential)

	cmp	DWORD PTR ?s_bDenyConfidential@COutlookHelper@@1HA, 0 ; COutlookHelper::s_bDenyConfidential
	jne	SHORT $L75946

; 351  : 		s_bDenyConfidential = (bIncludeConfidential == FALSE);

	xor	ecx, ecx
	cmp	DWORD PTR _bIncludeConfidential$[ebp], 0
	sete	cl
	mov	DWORD PTR ?s_bDenyConfidential@COutlookHelper@@1HA, ecx ; COutlookHelper::s_bDenyConfidential
$L75946:

; 352  : 
; 353  : 	if (obj.m_lpDispatch != NULL)

	mov	edx, DWORD PTR _obj$[ebp]
	cmp	DWORD PTR [edx], 0
	je	$L75951

; 355  : 		for (int nField = OA_FIRST; nField < OA_LAST; nField++)

	mov	DWORD PTR _nField$75948[ebp], 0
	jmp	SHORT $L75949
$L75950:
	mov	eax, DWORD PTR _nField$75948[ebp]
	add	eax, 1
	mov	DWORD PTR _nField$75948[ebp], eax
$L75949:
	cmp	DWORD PTR _nField$75948[ebp], 30	; 0000001eH
	jge	$L75951

; 357  : 			try
; 358  : 			{

	mov	DWORD PTR __$EHRec$[ebp+12], 0

; 359  : 				mapData[(OUTLOOK_FIELDTYPE)nField] = GetItemData(obj, (OUTLOOK_FIELDTYPE)nField);

	mov	ecx, DWORD PTR _nField$75948[ebp]
	push	ecx
	mov	edx, DWORD PTR _obj$[ebp]
	push	edx
	lea	eax, DWORD PTR $T76829[ebp]
	push	eax
	call	?GetItemData@COutlookHelper@@SA?AVCString@@AAV_MailItem@OutlookAPI@@W4OUTLOOK_FIELDTYPE@@@Z ; COutlookHelper::GetItemData
	add	esp, 12					; 0000000cH
	mov	DWORD PTR -36+[ebp], eax
	mov	ecx, DWORD PTR -36+[ebp]
	mov	DWORD PTR -40+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+12], 1
	mov	edx, DWORD PTR _nField$75948[ebp]
	push	edx
	mov	ecx, DWORD PTR _mapData$[ebp]
	call	??A?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEAAVCString@@W4OUTLOOK_FIELDTYPE@@@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::operator[]
	mov	DWORD PTR -44+[ebp], eax
	mov	eax, DWORD PTR -40+[ebp]
	push	eax
	mov	ecx, DWORD PTR -44+[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+12], 0
	lea	ecx, DWORD PTR $T76829[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 361  : 			catch (COleException* e)

	jmp	SHORT $L75952
$L76831:

; 362  : 			{
; 363  : 				e->Delete();

	mov	ecx, DWORD PTR _e$75956[ebp]
	call	?Delete@CException@@QAEXXZ		; CException::Delete

; 364  : 				s_bDenyConfidential = TRUE;

	mov	DWORD PTR ?s_bDenyConfidential@COutlookHelper@@1HA, 1 ; COutlookHelper::s_bDenyConfidential

; 365  : 			}

	mov	eax, $L75952
	ret	0
$L76832:
$L75958:

; 367  : 			{
; 368  : 				ASSERT(0);

	mov	ecx, 1
	test	ecx, ecx
	je	SHORT $L75961
	push	368					; 00000170H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75961
	int	3
$L75961:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75958

; 369  : 				s_bDenyConfidential = TRUE;

	mov	DWORD PTR ?s_bDenyConfidential@COutlookHelper@@1HA, 1 ; COutlookHelper::s_bDenyConfidential

; 370  : 			}

	mov	eax, $L75952
	ret	0
$L75952:

; 371  : 		}

	mov	DWORD PTR __$EHRec$[ebp+12], -1
	jmp	$L75950
$L75951:

; 373  : 
; 374  : 	// restore confidentiality
; 375  : 	s_bDenyConfidential |= bDenyConfidential;

	mov	eax, DWORD PTR ?s_bDenyConfidential@COutlookHelper@@1HA ; COutlookHelper::s_bDenyConfidential
	or	eax, DWORD PTR _bDenyConfidential$[ebp]
	mov	DWORD PTR ?s_bDenyConfidential@COutlookHelper@@1HA, eax ; COutlookHelper::s_bDenyConfidential

; 376  : 
; 377  : 	return mapData.GetCount();

	mov	ecx, DWORD PTR _mapData$[ebp]
	call	?GetCount@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEHXZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetCount

; 378  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp+4]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 44					; 0000002cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76833:
	lea	ecx, DWORD PTR $T76829[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76836:
	mov	eax, OFFSET FLAT:$T76835
	jmp	___CxxFrameHandler
text$x	ENDS
?GetItemData@COutlookHelper@@SAHAAV_MailItem@OutlookAPI@@AAVCOutlookItemDataMap@@H@Z ENDP ; COutlookHelper::GetItemData
EXTRN	??0COleDateTime@@QAE@N@Z:NEAR			; COleDateTime::COleDateTime
EXTRN	?Format@COleDateTime@@QBE?AVCString@@KK@Z:NEAR	; COleDateTime::Format
_BSS	SEGMENT
	ALIGN	4

$SG75971 DW	01H DUP (?)
_BSS	ENDS
;	COMDAT ?MapDate@COutlookHelper@@KA?AVCString@@N@Z
_TEXT	SEGMENT
$T76844 = -20
$T76846 = -24
_date$ = 12
___$ReturnUdt$ = 8
_NODATE$ = -8
?MapDate@COutlookHelper@@KA?AVCString@@N@Z PROC NEAR	; COutlookHelper::MapDate, COMDAT

; 382  : {

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
	mov	DWORD PTR $T76846[ebp], 0

; 383  : 	const DATE NODATE = 949998;

	mov	DWORD PTR _NODATE$[ebp], 0
	mov	DWORD PTR _NODATE$[ebp+4], 1093467612	; 412cfddcH

; 384  : 
; 385  : 	if (date != NODATE)

	fld	QWORD PTR _date$[ebp]
	fcomp	QWORD PTR _NODATE$[ebp]
	fnstsw	ax
	test	ah, 64					; 00000040H
	jne	SHORT $L75968

; 386  : 		return COleDateTime(date).Format();

	push	1024					; 00000400H
	push	0
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	push	eax
	mov	ecx, DWORD PTR _date$[ebp+4]
	push	ecx
	mov	edx, DWORD PTR _date$[ebp]
	push	edx
	lea	ecx, DWORD PTR $T76844[ebp]
	call	??0COleDateTime@@QAE@N@Z		; COleDateTime::COleDateTime
	mov	ecx, eax
	call	?Format@COleDateTime@@QBE?AVCString@@KK@Z ; COleDateTime::Format
	mov	eax, DWORD PTR $T76846[ebp]
	or	al, 1
	mov	DWORD PTR $T76846[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L75966
$L75968:

; 387  : 
; 388  : 	// else
; 389  : 	return _T("");

	push	OFFSET FLAT:$SG75971
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	ecx, DWORD PTR $T76846[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76846[ebp], ecx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L75966:

; 390  : }

	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?MapDate@COutlookHelper@@KA?AVCString@@N@Z ENDP		; COutlookHelper::MapDate
_TEXT	ENDS
PUBLIC	?IsOutlookObject@COutlookHelper@@SAHPAUIDataObject@@@Z ; COutlookHelper::IsOutlookObject
PUBLIC	?IsOutlookObject@COutlookHelper@@SAHPAVCOleDataObject@@@Z ; COutlookHelper::IsOutlookObject
EXTRN	??0COleDataObject@@QAE@XZ:NEAR			; COleDataObject::COleDataObject
EXTRN	?Attach@COleDataObject@@QAEXPAUIDataObject@@H@Z:NEAR ; COleDataObject::Attach
EXTRN	??1COleDataObject@@QAE@XZ:NEAR			; COleDataObject::~COleDataObject
;	COMDAT xdata$x
; File D:\_CODE\Shared\OutlookHelper.cpp
xdata$x	SEGMENT
$T76853	DD	019930520H
	DD	01H
	DD	FLAT:$T76855
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76855	DD	0ffffffffH
	DD	FLAT:$L76851
xdata$x	ENDS
;	COMDAT ?IsOutlookObject@COutlookHelper@@SAHPAUIDataObject@@@Z
_TEXT	SEGMENT
$T76850 = -32
__$EHRec$ = -12
_lpObject$ = 8
_dataobj$75976 = -28
?IsOutlookObject@COutlookHelper@@SAHPAUIDataObject@@@Z PROC NEAR ; COutlookHelper::IsOutlookObject, COMDAT

; 394  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76854
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

; 395  : 	if (lpObject)

	cmp	DWORD PTR _lpObject$[ebp], 0
	je	SHORT $L75975

; 397  : 		COleDataObject dataobj;

	lea	ecx, DWORD PTR _dataobj$75976[ebp]
	call	??0COleDataObject@@QAE@XZ		; COleDataObject::COleDataObject
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 398  : 		dataobj.Attach(lpObject, FALSE);

	push	0
	mov	eax, DWORD PTR _lpObject$[ebp]
	push	eax
	lea	ecx, DWORD PTR _dataobj$75976[ebp]
	call	?Attach@COleDataObject@@QAEXPAUIDataObject@@H@Z ; COleDataObject::Attach

; 399  : 
; 400  : 		return IsOutlookObject(&dataobj);

	lea	ecx, DWORD PTR _dataobj$75976[ebp]
	push	ecx
	call	?IsOutlookObject@COutlookHelper@@SAHPAVCOleDataObject@@@Z ; COutlookHelper::IsOutlookObject
	add	esp, 4
	mov	DWORD PTR $T76850[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _dataobj$75976[ebp]
	call	??1COleDataObject@@QAE@XZ		; COleDataObject::~COleDataObject
	mov	eax, DWORD PTR $T76850[ebp]
	jmp	SHORT $L75974
$L75975:

; 402  : 
; 403  : 	// else
; 404  : 	return FALSE;

	xor	eax, eax
$L75974:

; 405  : }

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
$L76851:
	lea	ecx, DWORD PTR _dataobj$75976[ebp]
	call	??1COleDataObject@@QAE@XZ		; COleDataObject::~COleDataObject
	ret	0
$L76854:
	mov	eax, OFFSET FLAT:$T76853
	jmp	___CxxFrameHandler
text$x	ENDS
?IsOutlookObject@COutlookHelper@@SAHPAUIDataObject@@@Z ENDP ; COutlookHelper::IsOutlookObject
EXTRN	?IsDataAvailable@COleDataObject@@QAEHGPAUtagFORMATETC@@@Z:NEAR ; COleDataObject::IsDataAvailable
;	COMDAT ?IsOutlookObject@COutlookHelper@@SAHPAVCOleDataObject@@@Z
_TEXT	SEGMENT
_pObject$ = 8
?IsOutlookObject@COutlookHelper@@SAHPAVCOleDataObject@@@Z PROC NEAR ; COutlookHelper::IsOutlookObject, COMDAT

; 409  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 410  : 	return (pObject &&
; 411  : 			pObject->IsDataAvailable(CF_RENPRIVATEMESSAGES) && 
; 412  : 			pObject->IsDataAvailable(CF_RENPRIVATESRCFOLDER) &&
; 413  : 			pObject->IsDataAvailable(CF_FILECONTENTS) && 
; 414  : 			pObject->IsDataAvailable(CF_FILEDESCRIPTOR));

	cmp	DWORD PTR _pObject$[ebp], 0
	je	SHORT $L76860
	push	0
	mov	ax, WORD PTR _CF_RENPRIVATEMESSAGES
	push	eax
	mov	ecx, DWORD PTR _pObject$[ebp]
	call	?IsDataAvailable@COleDataObject@@QAEHGPAUtagFORMATETC@@@Z ; COleDataObject::IsDataAvailable
	test	eax, eax
	je	SHORT $L76860
	push	0
	mov	cx, WORD PTR _CF_RENPRIVATESRCFOLDER
	push	ecx
	mov	ecx, DWORD PTR _pObject$[ebp]
	call	?IsDataAvailable@COleDataObject@@QAEHGPAUtagFORMATETC@@@Z ; COleDataObject::IsDataAvailable
	test	eax, eax
	je	SHORT $L76860
	push	0
	mov	dx, WORD PTR _CF_FILECONTENTS
	push	edx
	mov	ecx, DWORD PTR _pObject$[ebp]
	call	?IsDataAvailable@COleDataObject@@QAEHGPAUtagFORMATETC@@@Z ; COleDataObject::IsDataAvailable
	test	eax, eax
	je	SHORT $L76860
	push	0
	mov	ax, WORD PTR _CF_FILEDESCRIPTOR
	push	eax
	mov	ecx, DWORD PTR _pObject$[ebp]
	call	?IsDataAvailable@COleDataObject@@QAEHGPAUtagFORMATETC@@@Z ; COleDataObject::IsDataAvailable
	test	eax, eax
	je	SHORT $L76860
	mov	DWORD PTR -4+[ebp], 1
	jmp	SHORT $L76861
$L76860:
	mov	DWORD PTR -4+[ebp], 0
$L76861:
	mov	eax, DWORD PTR -4+[ebp]

; 415  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsOutlookObject@COutlookHelper@@SAHPAVCOleDataObject@@@Z ENDP ; COutlookHelper::IsOutlookObject
_TEXT	ENDS
PUBLIC	?IsOutlookObject@COutlookHelper@@SAHPBG@Z	; COutlookHelper::IsOutlookObject
EXTRN	?HasExtension@FileMisc@@YAHPBG0@Z:NEAR		; FileMisc::HasExtension
;	COMDAT data
; File D:\_CODE\Shared\OutlookHelper.cpp
data	SEGMENT
$SG75984 DB	'.', 00H, 'm', 00H, 's', 00H, 'g', 00H, 00H, 00H
data	ENDS
;	COMDAT ?IsOutlookObject@COutlookHelper@@SAHPBG@Z
_TEXT	SEGMENT
_szFilePath$ = 8
?IsOutlookObject@COutlookHelper@@SAHPBG@Z PROC NEAR	; COutlookHelper::IsOutlookObject, COMDAT

; 419  : {

	push	ebp
	mov	ebp, esp

; 420  : 	return FileMisc::HasExtension(szFilePath, _T(".msg"));

	push	OFFSET FLAT:$SG75984
	mov	eax, DWORD PTR _szFilePath$[ebp]
	push	eax
	call	?HasExtension@FileMisc@@YAHPBG0@Z	; FileMisc::HasExtension
	add	esp, 8

; 421  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?IsOutlookObject@COutlookHelper@@SAHPBG@Z ENDP		; COutlookHelper::IsOutlookObject
_TEXT	ENDS
PUBLIC	?IsOutlookUrl@COutlookHelper@@SAHPBG@Z		; COutlookHelper::IsOutlookUrl
EXTRN	?Find@CString@@QBEHPBG@Z:NEAR			; CString::Find
;	COMDAT data
; File D:\_CODE\Shared\OutlookHelper.cpp
data	SEGMENT
$SG75990 DB	'o', 00H, 'u', 00H, 't', 00H, 'l', 00H, 'o', 00H, 'o', 00H
	DB	'k', 00H, ':', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76871	DD	019930520H
	DD	01H
	DD	FLAT:$T76873
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76873	DD	0ffffffffH
	DD	FLAT:$L76869
xdata$x	ENDS
;	COMDAT ?IsOutlookUrl@COutlookHelper@@SAHPBG@Z
_TEXT	SEGMENT
$T76867 = -16
$T76868 = -20
__$EHRec$ = -12
_szURL$ = 8
?IsOutlookUrl@COutlookHelper@@SAHPBG@Z PROC NEAR	; COutlookHelper::IsOutlookUrl, COMDAT

; 425  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76872
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-28], eax
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax

; 426  : 	return (CString(szURL).Find(_T("outlook:")) == 0);

	push	OFFSET FLAT:$SG75990
	mov	eax, DWORD PTR _szURL$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T76868[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR -24+[ebp], eax
	mov	ecx, DWORD PTR -24+[ebp]
	mov	DWORD PTR -28+[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -28+[ebp]
	call	?Find@CString@@QBEHPBG@Z		; CString::Find
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR $T76867[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76868[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76867[ebp]

; 427  : }

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
$L76869:
	lea	ecx, DWORD PTR $T76868[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76872:
	mov	eax, OFFSET FLAT:$T76871
	jmp	___CxxFrameHandler
text$x	ENDS
?IsOutlookUrl@COutlookHelper@@SAHPBG@Z ENDP		; COutlookHelper::IsOutlookUrl
PUBLIC	?GetFirstFileObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@ABVCStringArray@@@Z ; COutlookHelper::GetFirstFileObject
PUBLIC	?GetFileObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@PBG@Z ; COutlookHelper::GetFileObject
EXTRN	?GetSize@CStringArray@@QBEHXZ:NEAR		; CStringArray::GetSize
EXTRN	??ACStringArray@@QBE?AVCString@@H@Z:NEAR	; CStringArray::operator[]
;	COMDAT xdata$x
; File D:\_CODE\Shared\OutlookHelper.cpp
xdata$x	SEGMENT
$T76882	DD	019930520H
	DD	01H
	DD	FLAT:$T76884
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76884	DD	0ffffffffH
	DD	FLAT:$L76880
xdata$x	ENDS
;	COMDAT ?GetFirstFileObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@ABVCStringArray@@@Z
_TEXT	SEGMENT
$T76879 = -24
__$EHRec$ = -12
_aFiles$ = 8
_this$ = -28
_nFile$ = -16
_pItem$75999 = -20
?GetFirstFileObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@ABVCStringArray@@@Z PROC NEAR ; COutlookHelper::GetFirstFileObject, COMDAT

; 430  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76883
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

; 431  : 	for (int nFile = 0; nFile < aFiles.GetSize(); nFile++)

	mov	DWORD PTR _nFile$[ebp], 0
	jmp	SHORT $L75996
$L75997:
	mov	eax, DWORD PTR _nFile$[ebp]
	add	eax, 1
	mov	DWORD PTR _nFile$[ebp], eax
$L75996:
	mov	ecx, DWORD PTR _aFiles$[ebp]
	call	?GetSize@CStringArray@@QBEHXZ		; CStringArray::GetSize
	cmp	DWORD PTR _nFile$[ebp], eax
	jge	SHORT $L75998

; 433  : 		OutlookAPI::_MailItem* pItem = GetFileObject(aFiles[nFile]);

	mov	ecx, DWORD PTR _nFile$[ebp]
	push	ecx
	lea	edx, DWORD PTR $T76879[ebp]
	push	edx
	mov	ecx, DWORD PTR _aFiles$[ebp]
	call	??ACStringArray@@QBE?AVCString@@H@Z	; CStringArray::operator[]
	mov	DWORD PTR -32+[ebp], eax
	mov	eax, DWORD PTR -32+[ebp]
	mov	DWORD PTR -36+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -36+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetFileObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@PBG@Z ; COutlookHelper::GetFileObject
	mov	DWORD PTR _pItem$75999[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76879[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 434  : 		
; 435  : 		if (pItem)

	cmp	DWORD PTR _pItem$75999[ebp], 0
	je	SHORT $L76001

; 436  : 			return pItem;

	mov	eax, DWORD PTR _pItem$75999[ebp]
	jmp	SHORT $L75994
$L76001:

; 437  : 	}

	jmp	SHORT $L75997
$L75998:

; 438  : 
; 439  : 	return NULL; // no outlook object found

	xor	eax, eax
$L75994:

; 440  : }

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
$L76880:
	lea	ecx, DWORD PTR $T76879[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76883:
	mov	eax, OFFSET FLAT:$T76882
	jmp	___CxxFrameHandler
text$x	ENDS
?GetFirstFileObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@ABVCStringArray@@@Z ENDP ; COutlookHelper::GetFirstFileObject
PUBLIC	??0_NameSpace@OutlookAPI@@QAE@PAUIDispatch@@@Z	; OutlookAPI::_NameSpace::_NameSpace
PUBLIC	??1_NameSpace@OutlookAPI@@QAE@XZ		; OutlookAPI::_NameSpace::~_NameSpace
PUBLIC	??0_MailItem@OutlookAPI@@QAE@PAUIDispatch@@@Z	; OutlookAPI::_MailItem::_MailItem
EXTRN	?GetNamespace@_Application@OutlookAPI@@QAEPAUIDispatch@@PBG@Z:NEAR ; OutlookAPI::_Application::GetNamespace
EXTRN	?OpenSharedItem@_NameSpace@OutlookAPI@@QAEPAUIDispatch@@PBG@Z:NEAR ; OutlookAPI::_NameSpace::OpenSharedItem
;	COMDAT data
; File D:\_CODE\Shared\OutlookHelper.cpp
data	SEGMENT
$SG76009 DB	'M', 00H, 'A', 00H, 'P', 00H, 'I', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76897	DD	019930520H
	DD	02H
	DD	FLAT:$T76899
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76899	DD	0ffffffffH
	DD	FLAT:$L76894
	DD	00H
	DD	FLAT:$L76895
xdata$x	ENDS
;	COMDAT ?GetFileObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@PBG@Z
_TEXT	SEGMENT
$T76889 = -28
$T76890 = -32
$T76893 = -36
__$EHRec$ = -12
_szFilePath$ = 8
_this$ = -40
_nmspc$ = -20
_pItem$ = -24
?GetFileObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@PBG@Z PROC NEAR ; COutlookHelper::GetFileObject, COMDAT

; 443  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76898
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

; 444  : 	if (!IsOutlookObject(szFilePath) || s_pOutlook == NULL)

	mov	eax, DWORD PTR _szFilePath$[ebp]
	push	eax
	call	?IsOutlookObject@COutlookHelper@@SAHPBG@Z ; COutlookHelper::IsOutlookObject
	add	esp, 4
	test	eax, eax
	je	SHORT $L76007
	cmp	DWORD PTR ?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A, 0 ; COutlookHelper::s_pOutlook
	jne	SHORT $L76006
$L76007:

; 445  : 		return NULL;

	xor	eax, eax
	jmp	$L76005
$L76006:

; 446  : 
; 447  : 	_NameSpace nmspc(s_pOutlook->GetNamespace(_T("MAPI")));

	push	OFFSET FLAT:$SG76009
	mov	ecx, DWORD PTR ?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A ; COutlookHelper::s_pOutlook
	call	?GetNamespace@_Application@OutlookAPI@@QAEPAUIDispatch@@PBG@Z ; OutlookAPI::_Application::GetNamespace
	push	eax
	lea	ecx, DWORD PTR _nmspc$[ebp]
	call	??0_NameSpace@OutlookAPI@@QAE@PAUIDispatch@@@Z ; OutlookAPI::_NameSpace::_NameSpace
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 448  : 	_MailItem* pItem = new _MailItem(nmspc.OpenSharedItem(szFilePath));

	push	448					; 000001c0H
	push	OFFSET FLAT:_THIS_FILE
	push	8
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T76890[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	cmp	DWORD PTR $T76890[ebp], 0
	je	SHORT $L76891
	mov	ecx, DWORD PTR _szFilePath$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _nmspc$[ebp]
	call	?OpenSharedItem@_NameSpace@OutlookAPI@@QAEPAUIDispatch@@PBG@Z ; OutlookAPI::_NameSpace::OpenSharedItem
	push	eax
	mov	ecx, DWORD PTR $T76890[ebp]
	call	??0_MailItem@OutlookAPI@@QAE@PAUIDispatch@@@Z ; OutlookAPI::_MailItem::_MailItem
	mov	DWORD PTR -44+[ebp], eax
	jmp	SHORT $L76892
$L76891:
	mov	DWORD PTR -44+[ebp], 0
$L76892:
	mov	edx, DWORD PTR -44+[ebp]
	mov	DWORD PTR $T76889[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR $T76889[ebp]
	mov	DWORD PTR _pItem$[ebp], eax

; 449  : 
; 450  : 	return pItem;

	mov	ecx, DWORD PTR _pItem$[ebp]
	mov	DWORD PTR $T76893[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _nmspc$[ebp]
	call	??1_NameSpace@OutlookAPI@@QAE@XZ	; OutlookAPI::_NameSpace::~_NameSpace
	mov	eax, DWORD PTR $T76893[ebp]
$L76005:

; 451  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 44					; 0000002cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76894:
	lea	ecx, DWORD PTR _nmspc$[ebp]
	call	??1_NameSpace@OutlookAPI@@QAE@XZ	; OutlookAPI::_NameSpace::~_NameSpace
	ret	0
$L76895:
	push	448					; 000001c0H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR $T76890[ebp]
	push	eax
	call	??3@YAXPAXPBDH@Z			; operator delete
	add	esp, 12					; 0000000cH
	ret	0
$L76898:
	mov	eax, OFFSET FLAT:$T76897
	jmp	___CxxFrameHandler
text$x	ENDS
?GetFileObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@PBG@Z ENDP ; COutlookHelper::GetFileObject
;	COMDAT ??0_NameSpace@OutlookAPI@@QAE@PAUIDispatch@@@Z
_TEXT	SEGMENT
_pDispatch$ = 8
_this$ = -4
??0_NameSpace@OutlookAPI@@QAE@PAUIDispatch@@@Z PROC NEAR ; OutlookAPI::_NameSpace::_NameSpace, COMDAT

; 1240 : 	_NameSpace(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	1
	mov	eax, DWORD PTR _pDispatch$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0COleDispatchDriver@@QAE@PAUIDispatch@@H@Z ; COleDispatchDriver::COleDispatchDriver
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0_NameSpace@OutlookAPI@@QAE@PAUIDispatch@@@Z ENDP	; OutlookAPI::_NameSpace::_NameSpace
_TEXT	ENDS
;	COMDAT ??0_MailItem@OutlookAPI@@QAE@PAUIDispatch@@@Z
_TEXT	SEGMENT
_pDispatch$ = 8
_this$ = -4
??0_MailItem@OutlookAPI@@QAE@PAUIDispatch@@@Z PROC NEAR	; OutlookAPI::_MailItem::_MailItem, COMDAT

; 3250 : 	_MailItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	1
	mov	eax, DWORD PTR _pDispatch$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0COleDispatchDriver@@QAE@PAUIDispatch@@H@Z ; COleDispatchDriver::COleDispatchDriver
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0_MailItem@OutlookAPI@@QAE@PAUIDispatch@@@Z ENDP	; OutlookAPI::_MailItem::_MailItem
_TEXT	ENDS
;	COMDAT ??1_NameSpace@OutlookAPI@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1_NameSpace@OutlookAPI@@QAE@XZ PROC NEAR		; OutlookAPI::_NameSpace::~_NameSpace, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1COleDispatchDriver@@QAE@XZ		; COleDispatchDriver::~COleDispatchDriver
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1_NameSpace@OutlookAPI@@QAE@XZ ENDP			; OutlookAPI::_NameSpace::~_NameSpace
_TEXT	ENDS
PUBLIC	?GetSelection@COutlookHelper@@QAEPAVSelection@OutlookAPI@@XZ ; COutlookHelper::GetSelection
PUBLIC	??0_Explorer@OutlookAPI@@QAE@PAUIDispatch@@@Z	; OutlookAPI::_Explorer::_Explorer
PUBLIC	??1_Explorer@OutlookAPI@@QAE@XZ			; OutlookAPI::_Explorer::~_Explorer
PUBLIC	??0Selection@OutlookAPI@@QAE@PAUIDispatch@@@Z	; OutlookAPI::Selection::Selection
EXTRN	?ActiveExplorer@_Application@OutlookAPI@@QAEPAUIDispatch@@XZ:NEAR ; OutlookAPI::_Application::ActiveExplorer
EXTRN	?GetSelection@_Explorer@OutlookAPI@@QAEPAUIDispatch@@XZ:NEAR ; OutlookAPI::_Explorer::GetSelection
;	COMDAT xdata$x
; File D:\_CODE\Shared\OutlookHelper.cpp
xdata$x	SEGMENT
$T76922	DD	019930520H
	DD	02H
	DD	FLAT:$T76924
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76924	DD	0ffffffffH
	DD	FLAT:$L76919
	DD	00H
	DD	FLAT:$L76920
xdata$x	ENDS
;	COMDAT ?GetSelection@COutlookHelper@@QAEPAVSelection@OutlookAPI@@XZ
_TEXT	SEGMENT
$T76914 = -28
$T76915 = -32
$T76918 = -36
__$EHRec$ = -12
_this$ = -40
_explorer$ = -24
_pSelection$ = -16
?GetSelection@COutlookHelper@@QAEPAVSelection@OutlookAPI@@XZ PROC NEAR ; COutlookHelper::GetSelection, COMDAT

; 454  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76923
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

; 455  : 	if (s_pOutlook == NULL)

	cmp	DWORD PTR ?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A, 0 ; COutlookHelper::s_pOutlook
	jne	SHORT $L76023

; 456  : 		return NULL;

	xor	eax, eax
	jmp	$L76022
$L76023:

; 457  : 	
; 458  : 	_Explorer explorer(s_pOutlook->ActiveExplorer());

	mov	ecx, DWORD PTR ?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A ; COutlookHelper::s_pOutlook
	call	?ActiveExplorer@_Application@OutlookAPI@@QAEPAUIDispatch@@XZ ; OutlookAPI::_Application::ActiveExplorer
	push	eax
	lea	ecx, DWORD PTR _explorer$[ebp]
	call	??0_Explorer@OutlookAPI@@QAE@PAUIDispatch@@@Z ; OutlookAPI::_Explorer::_Explorer
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 459  : 	Selection* pSelection = new Selection(explorer.GetSelection());

	push	459					; 000001cbH
	push	OFFSET FLAT:_THIS_FILE
	push	8
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T76915[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	cmp	DWORD PTR $T76915[ebp], 0
	je	SHORT $L76916
	lea	ecx, DWORD PTR _explorer$[ebp]
	call	?GetSelection@_Explorer@OutlookAPI@@QAEPAUIDispatch@@XZ ; OutlookAPI::_Explorer::GetSelection
	push	eax
	mov	ecx, DWORD PTR $T76915[ebp]
	call	??0Selection@OutlookAPI@@QAE@PAUIDispatch@@@Z ; OutlookAPI::Selection::Selection
	mov	DWORD PTR -44+[ebp], eax
	jmp	SHORT $L76917
$L76916:
	mov	DWORD PTR -44+[ebp], 0
$L76917:
	mov	eax, DWORD PTR -44+[ebp]
	mov	DWORD PTR $T76914[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR $T76914[ebp]
	mov	DWORD PTR _pSelection$[ebp], ecx

; 460  : 
; 461  : 	return pSelection;

	mov	edx, DWORD PTR _pSelection$[ebp]
	mov	DWORD PTR $T76918[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _explorer$[ebp]
	call	??1_Explorer@OutlookAPI@@QAE@XZ		; OutlookAPI::_Explorer::~_Explorer
	mov	eax, DWORD PTR $T76918[ebp]
$L76022:

; 462  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 44					; 0000002cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76919:
	lea	ecx, DWORD PTR _explorer$[ebp]
	call	??1_Explorer@OutlookAPI@@QAE@XZ		; OutlookAPI::_Explorer::~_Explorer
	ret	0
$L76920:
	push	459					; 000001cbH
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR $T76915[ebp]
	push	eax
	call	??3@YAXPAXPBDH@Z			; operator delete
	add	esp, 12					; 0000000cH
	ret	0
$L76923:
	mov	eax, OFFSET FLAT:$T76922
	jmp	___CxxFrameHandler
text$x	ENDS
?GetSelection@COutlookHelper@@QAEPAVSelection@OutlookAPI@@XZ ENDP ; COutlookHelper::GetSelection
;	COMDAT ??0_Explorer@OutlookAPI@@QAE@PAUIDispatch@@@Z
_TEXT	SEGMENT
_pDispatch$ = 8
_this$ = -4
??0_Explorer@OutlookAPI@@QAE@PAUIDispatch@@@Z PROC NEAR	; OutlookAPI::_Explorer::_Explorer, COMDAT

; 2167 : 	_Explorer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	1
	mov	eax, DWORD PTR _pDispatch$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0COleDispatchDriver@@QAE@PAUIDispatch@@H@Z ; COleDispatchDriver::COleDispatchDriver
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0_Explorer@OutlookAPI@@QAE@PAUIDispatch@@@Z ENDP	; OutlookAPI::_Explorer::_Explorer
_TEXT	ENDS
;	COMDAT ??0Selection@OutlookAPI@@QAE@PAUIDispatch@@@Z
_TEXT	SEGMENT
_pDispatch$ = 8
_this$ = -4
??0Selection@OutlookAPI@@QAE@PAUIDispatch@@@Z PROC NEAR	; OutlookAPI::Selection::Selection, COMDAT

; 2237 : 	Selection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	push	1
	mov	eax, DWORD PTR _pDispatch$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0COleDispatchDriver@@QAE@PAUIDispatch@@H@Z ; COleDispatchDriver::COleDispatchDriver
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??0Selection@OutlookAPI@@QAE@PAUIDispatch@@@Z ENDP	; OutlookAPI::Selection::Selection
_TEXT	ENDS
;	COMDAT ??1_Explorer@OutlookAPI@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1_Explorer@OutlookAPI@@QAE@XZ PROC NEAR		; OutlookAPI::_Explorer::~_Explorer, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1COleDispatchDriver@@QAE@XZ		; COleDispatchDriver::~COleDispatchDriver
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1_Explorer@OutlookAPI@@QAE@XZ ENDP			; OutlookAPI::_Explorer::~_Explorer
_TEXT	ENDS
PUBLIC	?GetSelectionCount@COutlookHelper@@QAEHXZ	; COutlookHelper::GetSelectionCount
PUBLIC	??1Selection@OutlookAPI@@QAE@XZ			; OutlookAPI::Selection::~Selection
EXTRN	?GetCount@Selection@OutlookAPI@@QAEJXZ:NEAR	; OutlookAPI::Selection::GetCount
;	COMDAT xdata$x
; File D:\_CODE\Shared\OutlookHelper.cpp
xdata$x	SEGMENT
$T76942	DD	019930520H
	DD	02H
	DD	FLAT:$T76944
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76944	DD	0ffffffffH
	DD	FLAT:$L76939
	DD	00H
	DD	FLAT:$L76940
xdata$x	ENDS
;	COMDAT ?GetSelectionCount@COutlookHelper@@QAEHXZ
_TEXT	SEGMENT
$T76938 = -32
__$EHRec$ = -12
_this$ = -36
_explorer$ = -28
_selection$ = -20
?GetSelectionCount@COutlookHelper@@QAEHXZ PROC NEAR	; COutlookHelper::GetSelectionCount, COMDAT

; 465  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76943
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

; 466  : 	if (s_pOutlook == NULL)

	cmp	DWORD PTR ?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A, 0 ; COutlookHelper::s_pOutlook
	jne	SHORT $L76038

; 467  : 		return NULL;

	xor	eax, eax
	jmp	SHORT $L76037
$L76038:

; 468  : 	
; 469  : 	_Explorer explorer(s_pOutlook->ActiveExplorer());

	mov	ecx, DWORD PTR ?s_pOutlook@COutlookHelper@@1PAV_Application@OutlookAPI@@A ; COutlookHelper::s_pOutlook
	call	?ActiveExplorer@_Application@OutlookAPI@@QAEPAUIDispatch@@XZ ; OutlookAPI::_Application::ActiveExplorer
	push	eax
	lea	ecx, DWORD PTR _explorer$[ebp]
	call	??0_Explorer@OutlookAPI@@QAE@PAUIDispatch@@@Z ; OutlookAPI::_Explorer::_Explorer
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 470  : 	Selection selection(explorer.GetSelection());

	lea	ecx, DWORD PTR _explorer$[ebp]
	call	?GetSelection@_Explorer@OutlookAPI@@QAEPAUIDispatch@@XZ ; OutlookAPI::_Explorer::GetSelection
	push	eax
	lea	ecx, DWORD PTR _selection$[ebp]
	call	??0Selection@OutlookAPI@@QAE@PAUIDispatch@@@Z ; OutlookAPI::Selection::Selection
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 471  : 
; 472  : 	return selection.GetCount();

	lea	ecx, DWORD PTR _selection$[ebp]
	call	?GetCount@Selection@OutlookAPI@@QAEJXZ	; OutlookAPI::Selection::GetCount
	mov	DWORD PTR $T76938[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _selection$[ebp]
	call	??1Selection@OutlookAPI@@QAE@XZ		; OutlookAPI::Selection::~Selection
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _explorer$[ebp]
	call	??1_Explorer@OutlookAPI@@QAE@XZ		; OutlookAPI::_Explorer::~_Explorer
	mov	eax, DWORD PTR $T76938[ebp]
$L76037:

; 473  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 36					; 00000024H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76939:
	lea	ecx, DWORD PTR _explorer$[ebp]
	call	??1_Explorer@OutlookAPI@@QAE@XZ		; OutlookAPI::_Explorer::~_Explorer
	ret	0
$L76940:
	lea	ecx, DWORD PTR _selection$[ebp]
	call	??1Selection@OutlookAPI@@QAE@XZ		; OutlookAPI::Selection::~Selection
	ret	0
$L76943:
	mov	eax, OFFSET FLAT:$T76942
	jmp	___CxxFrameHandler
text$x	ENDS
?GetSelectionCount@COutlookHelper@@QAEHXZ ENDP		; COutlookHelper::GetSelectionCount
;	COMDAT ??1Selection@OutlookAPI@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1Selection@OutlookAPI@@QAE@XZ PROC NEAR		; OutlookAPI::Selection::~Selection, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1COleDispatchDriver@@QAE@XZ		; COleDispatchDriver::~COleDispatchDriver
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1Selection@OutlookAPI@@QAE@XZ ENDP			; OutlookAPI::Selection::~Selection
_TEXT	ENDS
PUBLIC	?GetFirstSelectedObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@XZ ; COutlookHelper::GetFirstSelectedObject
PUBLIC	?GetFirstObject@COutlookHelper@@SAPAV_MailItem@OutlookAPI@@PAVSelection@3@@Z ; COutlookHelper::GetFirstObject
PUBLIC	??_GSelection@OutlookAPI@@QAEPAXI@Z		; OutlookAPI::Selection::`scalar deleting destructor'
;	COMDAT ?GetFirstSelectedObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@XZ
_TEXT	SEGMENT
$T76952 = -12
$T76953 = -16
_this$ = -20
_pSelection$ = -4
_pItem$ = -8
?GetFirstSelectedObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@XZ PROC NEAR ; COutlookHelper::GetFirstSelectedObject, COMDAT

; 476  : {

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

; 477  : 	Selection* pSelection = GetSelection();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSelection@COutlookHelper@@QAEPAVSelection@OutlookAPI@@XZ ; COutlookHelper::GetSelection
	mov	DWORD PTR _pSelection$[ebp], eax

; 478  : 	_MailItem* pItem = GetFirstObject(pSelection);

	mov	eax, DWORD PTR _pSelection$[ebp]
	push	eax
	call	?GetFirstObject@COutlookHelper@@SAPAV_MailItem@OutlookAPI@@PAVSelection@3@@Z ; COutlookHelper::GetFirstObject
	add	esp, 4
	mov	DWORD PTR _pItem$[ebp], eax

; 479  : 
; 480  : 	// cleanup
; 481  : 	delete pSelection;

	mov	ecx, DWORD PTR _pSelection$[ebp]
	mov	DWORD PTR $T76953[ebp], ecx
	mov	edx, DWORD PTR $T76953[ebp]
	mov	DWORD PTR $T76952[ebp], edx
	cmp	DWORD PTR $T76952[ebp], 0
	je	SHORT $L76954
	push	1
	mov	ecx, DWORD PTR $T76952[ebp]
	call	??_GSelection@OutlookAPI@@QAEPAXI@Z	; OutlookAPI::Selection::`scalar deleting destructor'
	mov	DWORD PTR -24+[ebp], eax
	jmp	SHORT $L76955
$L76954:
	mov	DWORD PTR -24+[ebp], 0
$L76955:

; 482  : 
; 483  : 	return pItem;

	mov	eax, DWORD PTR _pItem$[ebp]

; 484  : }

	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetFirstSelectedObject@COutlookHelper@@QAEPAV_MailItem@OutlookAPI@@XZ ENDP ; COutlookHelper::GetFirstSelectedObject
_TEXT	ENDS
;	COMDAT ??_GSelection@OutlookAPI@@QAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GSelection@OutlookAPI@@QAEPAXI@Z PROC NEAR		; OutlookAPI::Selection::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1Selection@OutlookAPI@@QAE@XZ		; OutlookAPI::Selection::~Selection
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L76054
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L76054:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GSelection@OutlookAPI@@QAEPAXI@Z ENDP		; OutlookAPI::Selection::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	?Item@Selection@OutlookAPI@@QAEPAUIDispatch@@ABUtagVARIANT@@@Z:NEAR ; OutlookAPI::Selection::Item
EXTRN	??0COleVariant@@QAE@FG@Z:NEAR			; COleVariant::COleVariant
EXTRN	??1COleVariant@@QAE@XZ:NEAR			; COleVariant::~COleVariant
;	COMDAT xdata$x
; File D:\_CODE\Shared\OutlookHelper.cpp
xdata$x	SEGMENT
$T76973	DD	019930520H
	DD	03H
	DD	FLAT:$T76975
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76975	DD	0ffffffffH
	DD	FLAT:$L76967
	DD	00H
	DD	FLAT:$L76969
	DD	0ffffffffH
	DD	FLAT:$L76969
xdata$x	ENDS
;	COMDAT ?GetFirstObject@COutlookHelper@@SAPAV_MailItem@OutlookAPI@@PAVSelection@3@@Z
_TEXT	SEGMENT
$T76960 = -20
$T76961 = -24
$T76962 = -40
$T76968 = -44
__$EHRec$ = -12
_pSelection$ = 8
_pItem$ = -16
?GetFirstObject@COutlookHelper@@SAPAV_MailItem@OutlookAPI@@PAVSelection@3@@Z PROC NEAR ; COutlookHelper::GetFirstObject, COMDAT

; 487  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76974
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 48					; 00000030H
	push	edi
	lea	edi, DWORD PTR [ebp-60]
	mov	ecx, 12					; 0000000cH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	DWORD PTR $T76968[ebp], 0

; 488  : 	if (pSelection == NULL)

	cmp	DWORD PTR _pSelection$[ebp], 0
	jne	SHORT $L76058

; 489  : 		return NULL;

	xor	eax, eax
	jmp	$L76057
$L76058:

; 490  : 
; 491  : 	_MailItem* pItem = (pSelection->GetCount() ? new _MailItem(pSelection->Item(COleVariant((short)1))) : NULL);

	mov	ecx, DWORD PTR _pSelection$[ebp]
	call	?GetCount@Selection@OutlookAPI@@QAEJXZ	; OutlookAPI::Selection::GetCount
	test	eax, eax
	je	SHORT $L76965
	push	491					; 000001ebH
	push	OFFSET FLAT:_THIS_FILE
	push	8
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T76961[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	cmp	DWORD PTR $T76961[ebp], 0
	je	SHORT $L76963
	push	2
	push	1
	lea	ecx, DWORD PTR $T76962[ebp]
	call	??0COleVariant@@QAE@FG@Z		; COleVariant::COleVariant
	mov	DWORD PTR -48+[ebp], eax
	mov	eax, DWORD PTR $T76968[ebp]
	or	al, 1
	mov	DWORD PTR $T76968[ebp], eax
	mov	ecx, DWORD PTR -48+[ebp]
	mov	DWORD PTR -52+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	edx, DWORD PTR -52+[ebp]
	push	edx
	mov	ecx, DWORD PTR _pSelection$[ebp]
	call	?Item@Selection@OutlookAPI@@QAEPAUIDispatch@@ABUtagVARIANT@@@Z ; OutlookAPI::Selection::Item
	push	eax
	mov	ecx, DWORD PTR $T76961[ebp]
	call	??0_MailItem@OutlookAPI@@QAE@PAUIDispatch@@@Z ; OutlookAPI::_MailItem::_MailItem
	mov	DWORD PTR -56+[ebp], eax
	jmp	SHORT $L76964
$L76963:
	mov	DWORD PTR -56+[ebp], 0
$L76964:
	mov	eax, DWORD PTR -56+[ebp]
	mov	DWORD PTR $T76960[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR $T76960[ebp]
	mov	DWORD PTR -60+[ebp], ecx
	jmp	SHORT $L76966
$L76965:
	mov	DWORD PTR -60+[ebp], 0
$L76966:
	mov	edx, DWORD PTR -60+[ebp]
	mov	DWORD PTR _pItem$[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR $T76968[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L76971
	and	DWORD PTR $T76968[ebp], -2		; fffffffeH
	lea	ecx, DWORD PTR $T76962[ebp]
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
$L76971:

; 492  : 	
; 493  : 	return pItem;

	mov	eax, DWORD PTR _pItem$[ebp]
$L76057:

; 494  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 60					; 0000003cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76967:
	push	491					; 000001ebH
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR $T76961[ebp]
	push	eax
	call	??3@YAXPAXPBDH@Z			; operator delete
	add	esp, 12					; 0000000cH
	ret	0
$L76969:
	mov	eax, DWORD PTR $T76968[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76970
	lea	ecx, DWORD PTR $T76962[ebp]
	call	??1COleVariant@@QAE@XZ			; COleVariant::~COleVariant
$L76970:
	ret	0
$L76974:
	mov	eax, OFFSET FLAT:$T76973
	jmp	___CxxFrameHandler
text$x	ENDS
?GetFirstObject@COutlookHelper@@SAPAV_MailItem@OutlookAPI@@PAVSelection@3@@Z ENDP ; COutlookHelper::GetFirstObject
;	COMDAT ?IsConfidential@COutlookHelper@@SAHW4OUTLOOK_FIELDTYPE@@@Z
_TEXT	SEGMENT
_nField$ = 8
_i$ = -4
?IsConfidential@COutlookHelper@@SAHW4OUTLOOK_FIELDTYPE@@@Z PROC NEAR ; COutlookHelper::IsConfidential, COMDAT

; 498  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L76071:

; 499  : 	// sanity check
; 500  : 	ASSERT(NUMOUTLOOKDATA == OA_LAST);

	xor	eax, eax
	test	eax, eax
	je	SHORT $L76074
	int	3
$L76074:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76071

; 501  : 
; 502  : 	for (int i = 0; i < NUMOUTLOOKDATA; i++)

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L76076
$L76077:
	mov	edx, DWORD PTR _i$[ebp]
	add	edx, 1
	mov	DWORD PTR _i$[ebp], edx
$L76076:
	cmp	DWORD PTR _i$[ebp], 30			; 0000001eH
	jge	SHORT $L76078

; 504  : 		if (OUTLOOKDATA[i].nField == nField)

	mov	eax, DWORD PTR _i$[ebp]
	mov	ecx, DWORD PTR ?OUTLOOKDATA@@3PAUOUTLOOKDATAITEM@@A[eax*8]
	cmp	ecx, DWORD PTR _nField$[ebp]
	jne	SHORT $L76079

; 505  : 			return OUTLOOKDATA[i].bConfidential;

	mov	edx, DWORD PTR _i$[ebp]
	mov	eax, DWORD PTR ?OUTLOOKDATA@@3PAUOUTLOOKDATAITEM@@A[edx*8+4]
	jmp	SHORT $L76070
$L76079:

; 506  : 	}

	jmp	SHORT $L76077
$L76078:

; 507  : 
; 508  : 	// fallback
; 509  : 	return TRUE;

	mov	eax, 1
$L76070:

; 510  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?IsConfidential@COutlookHelper@@SAHW4OUTLOOK_FIELDTYPE@@@Z ENDP ; COutlookHelper::IsConfidential
_TEXT	ENDS
PUBLIC	??_7?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@6B@ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::`vftable'
PUBLIC	??_G?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAEPAXI@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::`scalar deleting destructor'
PUBLIC	??_E?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAEPAXI@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::`vector deleting destructor'
EXTRN	??1CObject@@UAE@XZ:NEAR				; CObject::~CObject
EXTRN	??0CObject@@IAE@XZ:NEAR				; CObject::CObject
;	COMDAT ??_7?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@6B@
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
CONST	SEGMENT
??_7?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@6B@ DD FLAT:?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::`vftable'
	DD	FLAT:??_E?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAEPAXI@Z
	DD	FLAT:?Serialize@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UBEXXZ
	DD	FLAT:?Dump@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UBEXAAVCDumpContext@@@Z
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76984	DD	019930520H
	DD	01H
	DD	FLAT:$T76986
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76986	DD	0ffffffffH
	DD	FLAT:$L76982
xdata$x	ENDS
;	COMDAT ??0?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAE@H@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_nBlockSize$ = 8
_this$ = -16
??0?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAE@H@Z PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>, COMDAT

; 1201 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76985
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
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@6B@ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::`vftable'
$L76168:

; 1202 : 	ASSERT(nBlockSize > 0);

	cmp	DWORD PTR _nBlockSize$[ebp], 0
	jg	SHORT $L76171
	push	1202					; 000004b2H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76171
	int	3
$L76171:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76168

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
$L76982:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L76985:
	mov	eax, OFFSET FLAT:$T76984
	jmp	___CxxFrameHandler
text$x	ENDS
??0?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAE@H@Z ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>
;	COMDAT ?GetCount@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?GetCount@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEHXZ PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetCount, COMDAT

; 1182 : 	{ return m_nCount; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+12]
	mov	esp, ebp
	pop	ebp
	ret	0
?GetCount@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEHXZ ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetCount
_TEXT	ENDS
PUBLIC	?GetAssocAt@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@IBEPAUCAssoc@1@W4OUTLOOK_FIELDTYPE@@AAI@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetAssocAt
EXTRN	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z:NEAR ; AfxAssertValidObject
;	COMDAT ?Lookup@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEHW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z
_TEXT	SEGMENT
_key$ = 8
_rValue$ = 12
_this$ = -12
_nHash$ = -8
_pAssoc$ = -4
?Lookup@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEHW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::Lookup, COMDAT

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
	call	?GetAssocAt@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@IBEPAUCAssoc@1@W4OUTLOOK_FIELDTYPE@@AAI@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetAssocAt
	mov	DWORD PTR _pAssoc$[ebp], eax

; 1346 : 	if (pAssoc == NULL)

	cmp	DWORD PTR _pAssoc$[ebp], 0
	jne	SHORT $L76182

; 1347 : 		return FALSE;  // not in map

	xor	eax, eax
	jmp	SHORT $L76179
$L76182:

; 1348 : 
; 1349 : 	rValue = pAssoc->value;

	mov	eax, DWORD PTR _pAssoc$[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	mov	ecx, DWORD PTR _rValue$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=

; 1350 : 	return TRUE;

	mov	eax, 1
$L76179:

; 1351 : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Lookup@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEHW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::Lookup
_TEXT	ENDS
PUBLIC	?InitHashTable@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXIH@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::InitHashTable
PUBLIC	?NewAssoc@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@IAEPAUCAssoc@1@XZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::NewAssoc
;	COMDAT ??A?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEAAVCString@@W4OUTLOOK_FIELDTYPE@@@Z
_TEXT	SEGMENT
_key$ = 8
_this$ = -12
_nHash$ = -8
_pAssoc$ = -4
??A?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEAAVCString@@W4OUTLOOK_FIELDTYPE@@@Z PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::operator[], COMDAT

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
	call	?GetAssocAt@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@IBEPAUCAssoc@1@W4OUTLOOK_FIELDTYPE@@AAI@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetAssocAt
	mov	DWORD PTR _pAssoc$[ebp], eax
	cmp	DWORD PTR _pAssoc$[ebp], 0
	jne	SHORT $L76189

; 1362 : 		if (m_pHashTable == NULL)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L76190

; 1363 : 			InitHashTable(m_nHashTableSize);

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?InitHashTable@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXIH@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::InitHashTable
$L76190:

; 1364 : 
; 1365 : 		// it doesn't exist, add a new Association
; 1366 : 		pAssoc = NewAssoc();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?NewAssoc@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@IAEPAUCAssoc@1@XZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::NewAssoc
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
$L76189:

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
??A?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEAAVCString@@W4OUTLOOK_FIELDTYPE@@@Z ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::operator[]
_TEXT	ENDS
;	COMDAT ?SetAt@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z
_TEXT	SEGMENT
_key$ = 8
_newValue$ = 12
_this$ = -4
?SetAt@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::SetAt, COMDAT

; 1188 : 	{ (*this)[key] = newValue; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _newValue$[ebp]
	push	eax
	mov	ecx, DWORD PTR _key$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??A?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEAAVCString@@W4OUTLOOK_FIELDTYPE@@@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::operator[]
	mov	ecx, eax
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SetAt@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::SetAt
_TEXT	ENDS
PUBLIC	?DestructElements@@YGXPAW4OUTLOOK_FIELDTYPE@@H@Z ; DestructElements
EXTRN	?FreeDataChain@CPlex@@QAEXXZ:NEAR		; CPlex::FreeDataChain
EXTRN	?DestructElements@@YGXPAVCString@@H@Z:NEAR	; DestructElements
;	COMDAT ?RemoveAll@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXXZ
_TEXT	SEGMENT
$T76999 = -12
_this$ = -16
_nHash$76200 = -4
_pAssoc$76204 = -8
?RemoveAll@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXXZ PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::RemoveAll, COMDAT

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
	je	SHORT $L76203

; 1245 : 		// destroy elements (values and keys)
; 1246 : 		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)

	mov	DWORD PTR _nHash$76200[ebp], 0
	jmp	SHORT $L76201
$L76202:
	mov	edx, DWORD PTR _nHash$76200[ebp]
	add	edx, 1
	mov	DWORD PTR _nHash$76200[ebp], edx
$L76201:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nHash$76200[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jae	SHORT $L76203

; 1248 : 			CAssoc* pAssoc;
; 1249 : 			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _nHash$76200[ebp]
	mov	edx, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR _pAssoc$76204[ebp], edx

; 1250 : 			  pAssoc = pAssoc->pNext)

	jmp	SHORT $L76205
$L76206:
	mov	eax, DWORD PTR _pAssoc$76204[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssoc$76204[ebp], ecx
$L76205:
	cmp	DWORD PTR _pAssoc$76204[ebp], 0
	je	SHORT $L76207

; 1252 : 				DestructElements<VALUE>(&pAssoc->value, 1);

	push	1
	mov	edx, DWORD PTR _pAssoc$76204[ebp]
	add	edx, 12					; 0000000cH
	push	edx
	call	?DestructElements@@YGXPAVCString@@H@Z	; DestructElements

; 1253 : 				DestructElements<KEY>(&pAssoc->key, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$76204[ebp]
	add	eax, 8
	push	eax
	call	?DestructElements@@YGXPAW4OUTLOOK_FIELDTYPE@@H@Z ; DestructElements

; 1254 : 			}

	jmp	SHORT $L76206
$L76207:

; 1255 : 		}

	jmp	SHORT $L76202
$L76203:

; 1257 : 
; 1258 : 	// free hash table
; 1259 : 	delete[] m_pHashTable;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR $T76999[ebp], edx
	mov	eax, DWORD PTR $T76999[ebp]
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
?RemoveAll@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXXZ ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::RemoveAll
_TEXT	ENDS
;	COMDAT ?GetStartPosition@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEPAU__POSITION@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetStartPosition@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEPAU__POSITION@@XZ PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetStartPosition, COMDAT

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
?GetStartPosition@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEPAU__POSITION@@XZ ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetStartPosition
_TEXT	ENDS
EXTRN	?AfxIsValidAddress@@YGHPBXIH@Z:NEAR		; AfxIsValidAddress
;	COMDAT ?GetNextAssoc@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEXAAPAU__POSITION@@AAW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z
_TEXT	SEGMENT
_rNextPosition$ = 8
_rKey$ = 12
_rValue$ = 16
_this$ = -20
_pAssocRet$ = -4
_nBucket$76241 = -12
_pAssocNext$ = -8
_nBucket$76257 = -16
?GetNextAssoc@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEXAAPAU__POSITION@@AAW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetNextAssoc, COMDAT

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
$L76228:

; 1410 : 	ASSERT(m_pHashTable != NULL);  // never call on empty map

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	jne	SHORT $L76231
	push	1410					; 00000582H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76231
	int	3
$L76231:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76228

; 1411 : 
; 1412 : 	CAssoc* pAssocRet = (CAssoc*)rNextPosition;

	mov	eax, DWORD PTR _rNextPosition$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _pAssocRet$[ebp], ecx
$L76234:

; 1413 : 	ASSERT(pAssocRet != NULL);

	cmp	DWORD PTR _pAssocRet$[ebp], 0
	jne	SHORT $L76237
	push	1413					; 00000585H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76237
	int	3
$L76237:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76234

; 1414 : 
; 1415 : 	if (pAssocRet == (CAssoc*) BEFORE_START_POSITION)

	cmp	DWORD PTR _pAssocRet$[ebp], -1
	jne	SHORT $L76248

; 1417 : 		// find the first association
; 1418 : 		for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)

	mov	DWORD PTR _nBucket$76241[ebp], 0
	jmp	SHORT $L76242
$L76243:
	mov	eax, DWORD PTR _nBucket$76241[ebp]
	add	eax, 1
	mov	DWORD PTR _nBucket$76241[ebp], eax
$L76242:
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nBucket$76241[ebp]
	cmp	edx, DWORD PTR [ecx+8]
	jae	SHORT $L76244

; 1419 : 			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR _nBucket$76241[ebp]
	mov	eax, DWORD PTR [ecx+edx*4]
	mov	DWORD PTR _pAssocRet$[ebp], eax
	cmp	DWORD PTR _pAssocRet$[ebp], 0
	je	SHORT $L76245

; 1420 : 				break;

	jmp	SHORT $L76244
$L76245:

; 1421 : 		ASSERT(pAssocRet != NULL);  // must find something

	jmp	SHORT $L76243
$L76244:
	cmp	DWORD PTR _pAssocRet$[ebp], 0
	jne	SHORT $L76249
	push	1421					; 0000058dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76249
	int	3
$L76249:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76244
$L76248:

; 1423 : 
; 1424 : 	// find next association
; 1425 : 	ASSERT(AfxIsValidAddress(pAssocRet, sizeof(CAssoc)));

	push	1
	push	16					; 00000010H
	mov	edx, DWORD PTR _pAssocRet$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76254
	push	1425					; 00000591H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76254
	int	3
$L76254:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76248

; 1426 : 	CAssoc* pAssocNext;
; 1427 : 	if ((pAssocNext = pAssocRet->pNext) == NULL)

	mov	ecx, DWORD PTR _pAssocRet$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _pAssocNext$[ebp], edx
	cmp	DWORD PTR _pAssocNext$[ebp], 0
	jne	SHORT $L76260

; 1429 : 		// go to next bucket
; 1430 : 		for (UINT nBucket = pAssocRet->nHashValue + 1;

	mov	eax, DWORD PTR _pAssocRet$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	add	ecx, 1
	mov	DWORD PTR _nBucket$76257[ebp], ecx

; 1431 : 		  nBucket < m_nHashTableSize; nBucket++)

	jmp	SHORT $L76258
$L76259:
	mov	edx, DWORD PTR _nBucket$76257[ebp]
	add	edx, 1
	mov	DWORD PTR _nBucket$76257[ebp], edx
$L76258:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nBucket$76257[ebp]
	cmp	ecx, DWORD PTR [eax+8]
	jae	SHORT $L76260

; 1432 : 			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	ecx, DWORD PTR _nBucket$76257[ebp]
	mov	edx, DWORD PTR [eax+ecx*4]
	mov	DWORD PTR _pAssocNext$[ebp], edx
	cmp	DWORD PTR _pAssocNext$[ebp], 0
	je	SHORT $L76261

; 1433 : 				break;

	jmp	SHORT $L76260
$L76261:

; 1434 : 	}

	jmp	SHORT $L76259
$L76260:

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

	mov	edx, DWORD PTR _pAssocRet$[ebp]
	add	edx, 12					; 0000000cH
	push	edx
	mov	ecx, DWORD PTR _rValue$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=

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
?GetNextAssoc@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEXAAPAU__POSITION@@AAW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetNextAssoc
_TEXT	ENDS
;	COMDAT xdata$x
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
xdata$x	SEGMENT
$T77008	DD	019930520H
	DD	01H
	DD	FLAT:$T77010
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77010	DD	0ffffffffH
	DD	FLAT:$L77006
xdata$x	ENDS
;	COMDAT ??1?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??1?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAE@XZ PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::~CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>, COMDAT

; 1270 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77009
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
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@6B@ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1271 : 	RemoveAll();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?RemoveAll@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXXZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::RemoveAll
$L76266:

; 1272 : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L76269
	push	1272					; 000004f8H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76269
	int	3
$L76269:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76266

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
$L77006:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CObject@@UAE@XZ			; CObject::~CObject
	ret	0
$L77009:
	mov	eax, OFFSET FLAT:$T77008
	jmp	___CxxFrameHandler
text$x	ENDS
??1?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAE@XZ ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::~CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>
PUBLIC	?SerializeElements@@YGXAAVCArchive@@PAW4OUTLOOK_FIELDTYPE@@H@Z ; SerializeElements
EXTRN	?ReadCount@CArchive@@QAEKXZ:NEAR		; CArchive::ReadCount
EXTRN	?WriteCount@CArchive@@QAEXK@Z:NEAR		; CArchive::WriteCount
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	?SerializeElements@@YGXAAVCArchive@@PAVCString@@H@Z:NEAR ; SerializeElements
EXTRN	?IsStoring@CArchive@@QBEHXZ:NEAR		; CArchive::IsStoring
;	COMDAT xdata$x
; File C:\Program Files (x86)\Microsoft Visual Studio\VC98\MFC\INCLUDE\afxtempl.h
xdata$x	SEGMENT
$T77017	DD	019930520H
	DD	01H
	DD	FLAT:$T77019
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T77019	DD	0ffffffffH
	DD	FLAT:$L77015
xdata$x	ENDS
;	COMDAT ?Serialize@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAEXAAVCArchive@@@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_ar$ = 8
_this$ = -36
_nHash$76280 = -16
_pAssoc$76284 = -20
_nNewCount$76300 = -24
_newKey$76304 = -28
_newValue$76305 = -32
?Serialize@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAEXAAVCArchive@@@Z PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::Serialize, COMDAT

; 1445 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77018
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
	je	$L76274

; 1452 : 		ar.WriteCount(m_nCount);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?WriteCount@CArchive@@QAEXK@Z		; CArchive::WriteCount

; 1453 : 		if (m_nCount == 0)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jne	SHORT $L76275

; 1454 : 			return;  // nothing more to do

	jmp	$L76273
$L76275:

; 1455 : 
; 1456 : 		ASSERT(m_pHashTable != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+4], 0
	jne	SHORT $L76279
	push	1456					; 000005b0H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76279
	int	3
$L76279:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76275

; 1457 : 		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)

	mov	DWORD PTR _nHash$76280[ebp], 0
	jmp	SHORT $L76281
$L76282:
	mov	ecx, DWORD PTR _nHash$76280[ebp]
	add	ecx, 1
	mov	DWORD PTR _nHash$76280[ebp], ecx
$L76281:
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _nHash$76280[ebp]
	cmp	eax, DWORD PTR [edx+8]
	jae	SHORT $L76283

; 1459 : 			CAssoc* pAssoc;
; 1460 : 			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR _nHash$76280[ebp]
	mov	ecx, DWORD PTR [edx+eax*4]
	mov	DWORD PTR _pAssoc$76284[ebp], ecx

; 1461 : 			  pAssoc = pAssoc->pNext)

	jmp	SHORT $L76285
$L76286:
	mov	edx, DWORD PTR _pAssoc$76284[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _pAssoc$76284[ebp], eax
$L76285:
	cmp	DWORD PTR _pAssoc$76284[ebp], 0
	je	SHORT $L76287

; 1463 : 				SerializeElements<KEY>(ar, &pAssoc->key, 1);

	push	1
	mov	ecx, DWORD PTR _pAssoc$76284[ebp]
	add	ecx, 8
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAW4OUTLOOK_FIELDTYPE@@H@Z ; SerializeElements

; 1464 : 				SerializeElements<VALUE>(ar, &pAssoc->value, 1);

	push	1
	mov	eax, DWORD PTR _pAssoc$76284[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	call	?SerializeElements@@YGXAAVCArchive@@PAVCString@@H@Z ; SerializeElements

; 1465 : 			}

	jmp	SHORT $L76286
$L76287:

; 1466 : 		}

	jmp	SHORT $L76282
$L76283:

; 1468 : 	else

	jmp	SHORT $L76303
$L76274:

; 1470 : 		DWORD nNewCount = ar.ReadCount();

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?ReadCount@CArchive@@QAEKXZ		; CArchive::ReadCount
	mov	DWORD PTR _nNewCount$76300[ebp], eax
$L76302:

; 1471 : 		while (nNewCount--)

	mov	edx, DWORD PTR _nNewCount$76300[ebp]
	mov	eax, DWORD PTR _nNewCount$76300[ebp]
	sub	eax, 1
	mov	DWORD PTR _nNewCount$76300[ebp], eax
	test	edx, edx
	je	SHORT $L76303

; 1473 : 			KEY newKey;
; 1474 : 			VALUE newValue;

	lea	ecx, DWORD PTR _newValue$76305[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1475 : 			SerializeElements<KEY>(ar, &newKey, 1);

	push	1
	lea	ecx, DWORD PTR _newKey$76304[ebp]
	push	ecx
	mov	edx, DWORD PTR _ar$[ebp]
	push	edx
	call	?SerializeElements@@YGXAAVCArchive@@PAW4OUTLOOK_FIELDTYPE@@H@Z ; SerializeElements

; 1476 : 			SerializeElements<VALUE>(ar, &newValue, 1);

	push	1
	lea	eax, DWORD PTR _newValue$76305[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	push	ecx
	call	?SerializeElements@@YGXAAVCArchive@@PAVCString@@H@Z ; SerializeElements

; 1477 : 			SetAt(newKey, newValue);

	lea	edx, DWORD PTR _newValue$76305[ebp]
	push	edx
	mov	eax, DWORD PTR _newKey$76304[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetAt@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::SetAt

; 1478 : 		}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _newValue$76305[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	SHORT $L76302
$L76303:
$L76273:

; 1480 : }

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
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77015:
	lea	ecx, DWORD PTR _newValue$76305[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77018:
	mov	eax, OFFSET FLAT:$T77017
	jmp	___CxxFrameHandler
text$x	ENDS
?Serialize@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAEXAAVCArchive@@@Z ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::Serialize
PUBLIC	??_C@_05KIFI@with?5?$AA@			; `string'
PUBLIC	??_C@_09LOPF@?5elements?$AA@			; `string'
PUBLIC	??_C@_03DANH@?6?7?$FL?$AA@			; `string'
PUBLIC	?DumpElements@@YGXAAVCDumpContext@@PBW4OUTLOOK_FIELDTYPE@@H@Z ; DumpElements
PUBLIC	??_C@_04MNLE@?$FN?5?$DN?5?$AA@			; `string'
PUBLIC	?DumpElements@@YGXAAVCDumpContext@@PBVCString@@H@Z ; DumpElements
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
;	COMDAT ?Dump@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UBEXAAVCDumpContext@@@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_dc$ = 8
_this$ = -28
_key$76323 = -20
_val$76324 = -16
_pos$76325 = -24
?Dump@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UBEXAAVCDumpContext@@@Z PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::Dump, COMDAT

; 1485 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L77027
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
	jle	SHORT $L76322

; 1491 : 		// Dump in format "[key] -> value"
; 1492 : 		KEY key;
; 1493 : 		VALUE val;

	lea	ecx, DWORD PTR _val$76324[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1494 : 
; 1495 : 		POSITION pos = GetStartPosition();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetStartPosition@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEPAU__POSITION@@XZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetStartPosition
	mov	DWORD PTR _pos$76325[ebp], eax
$L76327:

; 1496 : 		while (pos != NULL)

	cmp	DWORD PTR _pos$76325[ebp], 0
	je	SHORT $L76328

; 1498 : 			GetNextAssoc(pos, key, val);

	lea	eax, DWORD PTR _val$76324[ebp]
	push	eax
	lea	ecx, DWORD PTR _key$76323[ebp]
	push	ecx
	lea	edx, DWORD PTR _pos$76325[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetNextAssoc@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QBEXAAPAU__POSITION@@AAW4OUTLOOK_FIELDTYPE@@AAVCString@@@Z ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetNextAssoc

; 1499 : 			dc << "\n\t[";

	push	OFFSET FLAT:??_C@_03DANH@?6?7?$FL?$AA@	; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1500 : 			DumpElements<KEY>(dc, &key, 1);

	push	1
	lea	eax, DWORD PTR _key$76323[ebp]
	push	eax
	mov	ecx, DWORD PTR _dc$[ebp]
	push	ecx
	call	?DumpElements@@YGXAAVCDumpContext@@PBW4OUTLOOK_FIELDTYPE@@H@Z ; DumpElements

; 1501 : 			dc << "] = ";

	push	OFFSET FLAT:??_C@_04MNLE@?$FN?5?$DN?5?$AA@ ; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1502 : 			DumpElements<VALUE>(dc, &val, 1);

	push	1
	lea	edx, DWORD PTR _val$76324[ebp]
	push	edx
	mov	eax, DWORD PTR _dc$[ebp]
	push	eax
	call	?DumpElements@@YGXAAVCDumpContext@@PBVCString@@H@Z ; DumpElements

; 1503 : 		}

	jmp	SHORT $L76327
$L76328:

; 1504 : 	}

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _val$76324[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76322:

; 1505 : 
; 1506 : 	dc << "\n";

	push	OFFSET FLAT:??_C@_01BJG@?6?$AA@		; `string'
	mov	ecx, DWORD PTR _dc$[ebp]
	call	??6CDumpContext@@QAEAAV0@PBD@Z		; CDumpContext::operator<<

; 1507 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L77024:
	lea	ecx, DWORD PTR _val$76324[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L77027:
	mov	eax, OFFSET FLAT:$T77026
	jmp	___CxxFrameHandler
text$x	ENDS
?Dump@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UBEXAAVCDumpContext@@@Z ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::Dump
EXTRN	?AssertValid@CObject@@UBEXXZ:NEAR		; CObject::AssertValid
;	COMDAT ?AssertValid@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UBEXXZ
_TEXT	SEGMENT
_this$ = -4
?AssertValid@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UBEXXZ PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::AssertValid, COMDAT

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
$L76347:

; 1513 : 
; 1514 : 	ASSERT(m_nHashTableSize > 0);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	ja	SHORT $L76350
	push	1514					; 000005eaH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76350
	int	3
$L76350:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76347
$L76349:

; 1515 : 	ASSERT(m_nCount == 0 || m_pHashTable != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+12], 0
	je	SHORT $L76354
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L76354
	push	1515					; 000005ebH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76354
	int	3
$L76354:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76349

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
?AssertValid@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UBEXXZ ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::AssertValid
_TEXT	ENDS
;	COMDAT ??_G?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_G?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAEPAXI@Z PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAE@XZ ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::~CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L76358
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L76358:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_G?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@UAEPAXI@Z ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	_memset:NEAR
;	COMDAT ?InitHashTable@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXIH@Z
_TEXT	SEGMENT
$T77037 = -4
$T77038 = -8
_nHashSize$ = 8
_bAllocNow$ = 12
_this$ = -12
?InitHashTable@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXIH@Z PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::InitHashTable, COMDAT

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
$L76364:

; 1220 : 	ASSERT(m_nCount == 0);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	je	SHORT $L76367
	push	1220					; 000004c4H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76367
	int	3
$L76367:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76364
$L76366:

; 1221 : 	ASSERT(nHashSize > 0);

	cmp	DWORD PTR _nHashSize$[ebp], 0
	ja	SHORT $L76371
	push	1221					; 000004c5H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76371
	int	3
$L76371:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76366

; 1222 : 
; 1223 : 	if (m_pHashTable != NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L76372

; 1225 : 		// free hash table
; 1226 : 		delete[] m_pHashTable;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR $T77037[ebp], eax
	mov	ecx, DWORD PTR $T77037[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 1227 : 		m_pHashTable = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0
$L76372:

; 1229 : 
; 1230 : 	if (bAllocNow)

	cmp	DWORD PTR _bAllocNow$[ebp], 0
	je	SHORT $L76374

; 1232 : 		m_pHashTable = new CAssoc* [nHashSize];

	push	1232					; 000004d0H
	push	OFFSET FLAT:__szAfxTempl
	mov	eax, DWORD PTR _nHashSize$[ebp]
	shl	eax, 2
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T77038[ebp], eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR $T77038[ebp]
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
$L76374:

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
?InitHashTable@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@QAEXIH@Z ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::InitHashTable
_TEXT	ENDS
PUBLIC	?data@CPlex@@QAEPAXXZ				; CPlex::data
PUBLIC	?ConstructElements@@YGXPAW4OUTLOOK_FIELDTYPE@@H@Z ; ConstructElements
EXTRN	?Create@CPlex@@SGPAU1@AAPAU1@II@Z:NEAR		; CPlex::Create
EXTRN	?ConstructElements@@YGXPAVCString@@H@Z:NEAR	; ConstructElements
;	COMDAT ?NewAssoc@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@IAEPAUCAssoc@1@XZ
_TEXT	SEGMENT
_this$ = -20
_newBlock$76382 = -8
_pAssoc$76384 = -16
_i$76386 = -12
_pAssoc$ = -4
?NewAssoc@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@IAEPAUCAssoc@1@XZ PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::NewAssoc, COMDAT

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
	jne	SHORT $L76389

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
	mov	DWORD PTR _newBlock$76382[ebp], eax

; 1283 : 		// chain them into free list
; 1284 : 		CMap::CAssoc* pAssoc = (CMap::CAssoc*) newBlock->data();

	mov	ecx, DWORD PTR _newBlock$76382[ebp]
	call	?data@CPlex@@QAEPAXXZ			; CPlex::data
	mov	DWORD PTR _pAssoc$76384[ebp], eax

; 1285 : 		// free in reverse order to make it easier to debug
; 1286 : 		pAssoc += m_nBlockSize - 1;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	shl	edx, 4
	mov	eax, DWORD PTR _pAssoc$76384[ebp]
	add	eax, edx
	mov	DWORD PTR _pAssoc$76384[ebp], eax

; 1287 : 		for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	sub	edx, 1
	mov	DWORD PTR _i$76386[ebp], edx
	jmp	SHORT $L76387
$L76388:
	mov	eax, DWORD PTR _i$76386[ebp]
	sub	eax, 1
	mov	DWORD PTR _i$76386[ebp], eax
	mov	ecx, DWORD PTR _pAssoc$76384[ebp]
	sub	ecx, 16					; 00000010H
	mov	DWORD PTR _pAssoc$76384[ebp], ecx
$L76387:
	cmp	DWORD PTR _i$76386[ebp], 0
	jl	SHORT $L76389

; 1289 : 			pAssoc->pNext = m_pFreeList;

	mov	edx, DWORD PTR _pAssoc$76384[ebp]
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR [edx], ecx

; 1290 : 			m_pFreeList = pAssoc;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _pAssoc$76384[ebp]
	mov	DWORD PTR [edx+16], eax

; 1291 : 		}

	jmp	SHORT $L76388
$L76389:

; 1293 : 	ASSERT(m_pFreeList != NULL);  // we must have something

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+16], 0
	jne	SHORT $L76393
	push	1293					; 0000050dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76393
	int	3
$L76393:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76389

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
$L76395:

; 1298 : 	ASSERT(m_nCount > 0);  // make sure we don't overflow

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jg	SHORT $L76398
	push	1298					; 00000512H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76398
	int	3
$L76398:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76395

; 1299 : 	ConstructElements<KEY>(&pAssoc->key, 1);

	push	1
	mov	edx, DWORD PTR _pAssoc$[ebp]
	add	edx, 8
	push	edx
	call	?ConstructElements@@YGXPAW4OUTLOOK_FIELDTYPE@@H@Z ; ConstructElements

; 1300 : 	ConstructElements<VALUE>(&pAssoc->value, 1);   // special construct values

	push	1
	mov	eax, DWORD PTR _pAssoc$[ebp]
	add	eax, 12					; 0000000cH
	push	eax
	call	?ConstructElements@@YGXPAVCString@@H@Z	; ConstructElements

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
?NewAssoc@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@IAEPAUCAssoc@1@XZ ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::NewAssoc
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
PUBLIC	?HashKey@@YGIW4OUTLOOK_FIELDTYPE@@@Z		; HashKey
PUBLIC	?CompareElements@@YGHPBW4OUTLOOK_FIELDTYPE@@0@Z	; CompareElements
;	COMDAT ?GetAssocAt@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@IBEPAUCAssoc@1@W4OUTLOOK_FIELDTYPE@@AAI@Z
_TEXT	SEGMENT
_key$ = 8
_nHash$ = 12
_this$ = -8
_pAssoc$ = -4
?GetAssocAt@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@IBEPAUCAssoc@1@W4OUTLOOK_FIELDTYPE@@AAI@Z PROC NEAR ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetAssocAt, COMDAT

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
	call	?HashKey@@YGIW4OUTLOOK_FIELDTYPE@@@Z	; HashKey
	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	div	DWORD PTR [ecx+8]
	mov	eax, DWORD PTR _nHash$[ebp]
	mov	DWORD PTR [eax], edx

; 1325 : 
; 1326 : 	if (m_pHashTable == NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	jne	SHORT $L76417

; 1327 : 		return NULL;

	xor	eax, eax
	jmp	SHORT $L76412
$L76417:

; 1331 : 	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)

	mov	edx, DWORD PTR _nHash$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	eax, DWORD PTR [edx+eax*4]
	mov	DWORD PTR _pAssoc$[ebp], eax
	jmp	SHORT $L76419
$L76420:
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR _pAssoc$[ebp], edx
$L76419:
	cmp	DWORD PTR _pAssoc$[ebp], 0
	je	SHORT $L76421

; 1333 : 		if (CompareElements(&pAssoc->key, &key))

	lea	eax, DWORD PTR _key$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pAssoc$[ebp]
	add	ecx, 8
	push	ecx
	call	?CompareElements@@YGHPBW4OUTLOOK_FIELDTYPE@@0@Z ; CompareElements
	test	eax, eax
	je	SHORT $L76428

; 1334 : 			return pAssoc;

	mov	eax, DWORD PTR _pAssoc$[ebp]
	jmp	SHORT $L76412
$L76428:

; 1335 : 	}

	jmp	SHORT $L76420
$L76421:

; 1336 : 	return NULL;

	xor	eax, eax
$L76412:

; 1337 : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?GetAssocAt@?$CMap@W4OUTLOOK_FIELDTYPE@@W41@VCString@@AAV2@@@IBEPAUCAssoc@1@W4OUTLOOK_FIELDTYPE@@AAI@Z ENDP ; CMap<enum OUTLOOK_FIELDTYPE,enum OUTLOOK_FIELDTYPE,CString,CString &>::GetAssocAt
_TEXT	ENDS
;	COMDAT ?DestructElements@@YGXPAW4OUTLOOK_FIELDTYPE@@H@Z
_TEXT	SEGMENT
_pElements$ = 8
_nCount$ = 12
?DestructElements@@YGXPAW4OUTLOOK_FIELDTYPE@@H@Z PROC NEAR ; DestructElements, COMDAT

; 67   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L76430:

; 68   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76434
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76434
	push	69					; 00000045H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76434
	int	3
$L76434:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76430

; 69   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 70   : 
; 71   : 	// call the destructor(s)
; 72   : 	for (; nCount--; pElements++)

	jmp	SHORT $L76435
$L76436:
	mov	eax, DWORD PTR _pElements$[ebp]
	add	eax, 4
	mov	DWORD PTR _pElements$[ebp], eax
$L76435:
	mov	ecx, DWORD PTR _nCount$[ebp]
	mov	edx, DWORD PTR _nCount$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nCount$[ebp], edx
	test	ecx, ecx
	je	SHORT $L76437

; 73   : 		pElements->~TYPE();

	jmp	SHORT $L76436
$L76437:

; 74   : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?DestructElements@@YGXPAW4OUTLOOK_FIELDTYPE@@H@Z ENDP	; DestructElements
_TEXT	ENDS
EXTRN	?Read@CArchive@@QAEIPAXI@Z:NEAR			; CArchive::Read
EXTRN	?Write@CArchive@@QAEXPBXI@Z:NEAR		; CArchive::Write
;	COMDAT ?SerializeElements@@YGXAAVCArchive@@PAW4OUTLOOK_FIELDTYPE@@H@Z
_TEXT	SEGMENT
_ar$ = 8
_pElements$ = 12
_nCount$ = 16
?SerializeElements@@YGXAAVCArchive@@PAW4OUTLOOK_FIELDTYPE@@H@Z PROC NEAR ; SerializeElements, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L76439:

; 92   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76443
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76443
	push	93					; 0000005dH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76443
	int	3
$L76443:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76439

; 93   : 		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));
; 94   : 
; 95   : 	// default is bit-wise read/write
; 96   : 	if (ar.IsStoring())

	mov	ecx, DWORD PTR _ar$[ebp]
	call	?IsStoring@CArchive@@QBEHXZ		; CArchive::IsStoring
	test	eax, eax
	je	SHORT $L76444

; 97   : 		ar.Write((void*)pElements, nCount * sizeof(TYPE));

	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Write@CArchive@@QAEXPBXI@Z		; CArchive::Write

; 98   : 	else

	jmp	SHORT $L76447
$L76444:

; 99   : 		ar.Read((void*)pElements, nCount * sizeof(TYPE));

	mov	edx, DWORD PTR _nCount$[ebp]
	shl	edx, 2
	push	edx
	mov	eax, DWORD PTR _pElements$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ar$[ebp]
	call	?Read@CArchive@@QAEIPAXI@Z		; CArchive::Read
$L76447:

; 100  : }

	pop	edi
	pop	esi
	pop	ebx
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	12					; 0000000cH
?SerializeElements@@YGXAAVCArchive@@PAW4OUTLOOK_FIELDTYPE@@H@Z ENDP ; SerializeElements
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBW4OUTLOOK_FIELDTYPE@@H@Z
_TEXT	SEGMENT
_pElements$ = 12
_nCount$ = 16
?DumpElements@@YGXAAVCDumpContext@@PBW4OUTLOOK_FIELDTYPE@@H@Z PROC NEAR ; DumpElements, COMDAT

; 105  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L76451:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76455
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76455
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76455
	int	3
$L76455:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76451

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
?DumpElements@@YGXAAVCDumpContext@@PBW4OUTLOOK_FIELDTYPE@@H@Z ENDP ; DumpElements
_TEXT	ENDS
;	COMDAT ?DumpElements@@YGXAAVCDumpContext@@PBVCString@@H@Z
_TEXT	SEGMENT
_pElements$ = 12
_nCount$ = 16
?DumpElements@@YGXAAVCDumpContext@@PBVCString@@H@Z PROC NEAR ; DumpElements, COMDAT

; 105  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L76457:

; 106  : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76461
	push	0
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76461
	push	107					; 0000006bH
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76461
	int	3
$L76461:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76457

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
?DumpElements@@YGXAAVCDumpContext@@PBVCString@@H@Z ENDP	; DumpElements
_TEXT	ENDS
PUBLIC	??2@YAPAXIPAX@Z					; operator new
;	COMDAT ?ConstructElements@@YGXPAW4OUTLOOK_FIELDTYPE@@H@Z
_TEXT	SEGMENT
$T77055 = -4
_pElements$ = 8
_nCount$ = 12
?ConstructElements@@YGXPAW4OUTLOOK_FIELDTYPE@@H@Z PROC NEAR ; ConstructElements, COMDAT

; 53   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
$L76463:

; 54   : 	ASSERT(nCount == 0 ||

	cmp	DWORD PTR _nCount$[ebp], 0
	je	SHORT $L76467
	push	1
	mov	eax, DWORD PTR _nCount$[ebp]
	shl	eax, 2
	push	eax
	mov	ecx, DWORD PTR _pElements$[ebp]
	push	ecx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76467
	push	55					; 00000037H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76467
	int	3
$L76467:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L76463

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

	jmp	SHORT $L76470
$L76471:
	mov	edx, DWORD PTR _pElements$[ebp]
	add	edx, 4
	mov	DWORD PTR _pElements$[ebp], edx
$L76470:
	mov	eax, DWORD PTR _nCount$[ebp]
	mov	ecx, DWORD PTR _nCount$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCount$[ebp], ecx
	test	eax, eax
	je	SHORT $L76472

; 62   : 		::new((void*)pElements) TYPE;

	mov	edx, DWORD PTR _pElements$[ebp]
	push	edx
	push	4
	call	??2@YAPAXIPAX@Z				; operator new
	add	esp, 8
	mov	DWORD PTR $T77055[ebp], eax
	jmp	SHORT $L76471
$L76472:

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
?ConstructElements@@YGXPAW4OUTLOOK_FIELDTYPE@@H@Z ENDP	; ConstructElements
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
;	COMDAT ?HashKey@@YGIW4OUTLOOK_FIELDTYPE@@@Z
_TEXT	SEGMENT
_key$ = 8
?HashKey@@YGIW4OUTLOOK_FIELDTYPE@@@Z PROC NEAR		; HashKey, COMDAT

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
?HashKey@@YGIW4OUTLOOK_FIELDTYPE@@@Z ENDP		; HashKey
_TEXT	ENDS
;	COMDAT ?CompareElements@@YGHPBW4OUTLOOK_FIELDTYPE@@0@Z
_TEXT	SEGMENT
_pElement1$ = 8
_pElement2$ = 12
?CompareElements@@YGHPBW4OUTLOOK_FIELDTYPE@@0@Z PROC NEAR ; CompareElements, COMDAT

; 118  : {

	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi
$L76481:

; 119  : 	ASSERT(AfxIsValidAddress(pElement1, sizeof(TYPE), FALSE));

	push	0
	push	4
	mov	eax, DWORD PTR _pElement1$[ebp]
	push	eax
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76485
	push	119					; 00000077H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76485
	int	3
$L76485:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L76481
$L76483:

; 120  : 	ASSERT(AfxIsValidAddress(pElement2, sizeof(ARG_TYPE), FALSE));

	push	0
	push	4
	mov	edx, DWORD PTR _pElement2$[ebp]
	push	edx
	call	?AfxIsValidAddress@@YGHPBXIH@Z		; AfxIsValidAddress
	test	eax, eax
	jne	SHORT $L76490
	push	120					; 00000078H
	push	OFFSET FLAT:__szAfxTempl
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L76490
	int	3
$L76490:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L76483

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
?CompareElements@@YGHPBW4OUTLOOK_FIELDTYPE@@0@Z ENDP	; CompareElements
_TEXT	ENDS
END

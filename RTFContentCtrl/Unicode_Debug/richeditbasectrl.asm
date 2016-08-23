	TITLE	D:\_CODE\Shared\richeditbasectrl.cpp
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
;	COMDAT ??0FIND_STATE@CRichEditBaseCtrl@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetOwner@CRichEditOleCallback@CRichEditBaseCtrl@@QAEXPAV2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNewStorage@CRichEditBaseCtrl@@MAEJPAPAUIStorage@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetInPlaceContext@CRichEditBaseCtrl@@MAEJPAPAUIOleInPlaceFrame@@PAPAUIOleInPlaceUIWindow@@PAUtagOIFI@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ShowContainerUI@CRichEditBaseCtrl@@MAEJH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?QueryInsertObject@CRichEditBaseCtrl@@MAEJPAU_GUID@@PAUIStorage@@J@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DeleteObject@CRichEditBaseCtrl@@MAEJPAUIOleObject@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?QueryAcceptData@CRichEditBaseCtrl@@MAEJPAUIDataObject@@PAGKHPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ContextSensitiveHelp@CRichEditBaseCtrl@@MAEJH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetClipboardData@CRichEditBaseCtrl@@MAEJPAU_charrange@@KPAPAUIDataObject@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetDragDropEffect@CRichEditBaseCtrl@@MAEJHKPAK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetContextMenu@CRichEditBaseCtrl@@MAEJGPAUIOleObject@@PAU_charrange@@PAPAUHMENU__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?NewFindReplaceDlg@CRichEditBaseCtrl@@MAEPAVCFindReplaceDialog@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HasFlag@Misc@@YAHKK@Z
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
;	COMDAT _$E272
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E273
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E275
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _$E276
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0STREAMINCOOKIE@@QAE@ABVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetLength@STREAMINCOOKIE@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CopyFrom@STREAMINCOOKIE@@QBEPBDXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CRichEditBaseCtrl@@QAE@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCRichEditBaseCtrl@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1FIND_STATE@CRichEditBaseCtrl@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CRichEditBaseCtrl@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_GetBaseMessageMap@CRichEditBaseCtrl@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMessageMap@CRichEditBaseCtrl@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Create@CRichEditBaseCtrl@@QAEHKABUtagRECT@@PAVCWnd@@IK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnCreate@CRichEditBaseCtrl@@IAEHPAUtagCREATESTRUCTW@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnSetFocus@CRichEditBaseCtrl@@IAEXPAVCWnd@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnEditSetSelection@CRichEditBaseCtrl@@IAEJIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnDestroy@CRichEditBaseCtrl@@IAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PreSubclassWindow@CRichEditBaseCtrl@@MAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Undo@CRichEditBaseCtrl@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Redo@CRichEditBaseCtrl@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetTextEx@CRichEditBaseCtrl@@QAEHABVCString@@KI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSelText@CRichEditBaseCtrl@@QAE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetTextRange@CRichEditBaseCtrl@@QAE?AVCString@@ABU_charrange@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SelectCurrentWord@CRichEditBaseCtrl@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InsertSoftReturn@CRichEditBaseCtrl@@IAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InsertHorizontalLine@CRichEditBaseCtrl@@QAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InsertTable@CRichEditBaseCtrl@@QAEHHHHHK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CRichEditOleCallback@CRichEditBaseCtrl@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCRichEditOleCallback@CRichEditBaseCtrl@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0IRichEditOleCallback@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0IUnknown@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CRichEditOleCallback@CRichEditBaseCtrl@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNewStorage@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAPAUIStorage@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?QueryInterface@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJABU_GUID@@PAPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddRef@CRichEditOleCallback@CRichEditBaseCtrl@@UAGKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Release@CRichEditOleCallback@CRichEditBaseCtrl@@UAGKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetInPlaceContext@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAPAUIOleInPlaceFrame@@PAPAUIOleInPlaceUIWindow@@PAUtagOIFI@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ShowContainerUI@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?QueryInsertObject@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAU_GUID@@PAUIStorage@@J@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DeleteObject@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAUIOleObject@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?QueryAcceptData@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAUIDataObject@@PAGKHPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ContextSensitiveHelp@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetClipboardData@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAU_charrange@@KPAPAUIDataObject@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetDragDropEffect@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJHKPAK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetContextMenu@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJGPAUIOleObject@@PAU_charrange@@PAPAUHMENU__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DoEditFind@CRichEditBaseCtrl@@QAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DoEditReplace@CRichEditBaseCtrl@@QAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AdjustDialogPosition@CRichEditBaseCtrl@@IAEXPAVCDialog@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DoEditFindReplace@CRichEditBaseCtrl@@IAEXHI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnFindNext@CRichEditBaseCtrl@@MAEXPBGHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnReplaceSel@CRichEditBaseCtrl@@MAEXPBGHHH0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnReplaceAll@CRichEditBaseCtrl@@MAEXPBG0HH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnFindReplaceCmd@CRichEditBaseCtrl@@IAEJIJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SameAsSelected@CRichEditBaseCtrl@@IAEHPBGHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FindTextW@CRichEditBaseCtrl@@IAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FindTextW@CRichEditBaseCtrl@@IAEHPBGHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FindAndSelect@CRichEditBaseCtrl@@IAEJKAAU_findtextexw@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?TextNotFound@CRichEditBaseCtrl@@IAEXPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsFindDialog@CRichEditBaseCtrl@@IBEHPAUHWND__@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetMargins@CRichEditBaseCtrl@@QAEXHHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetMargins@CRichEditBaseCtrl@@QAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetMargins@CRichEditBaseCtrl@@QAEXPBUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?OnSize@CRichEditBaseCtrl@@IAEXIHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSelectionCharFormat@CRichEditBaseCtrl@@QBEKAAUCHARFORMAT2W@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSelectionCharFormat@CRichEditBaseCtrl@@QBEKAAU_charformat@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetSelectionCharFormat@CRichEditBaseCtrl@@QAEHAAUCHARFORMAT2W@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetSelectionCharFormat@CRichEditBaseCtrl@@QAEHAAU_charformat@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetParaFormat@CRichEditBaseCtrl@@QAEHAAUPARAFORMAT2@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetParaFormat@CRichEditBaseCtrl@@QAEHAAU_paraformat@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetParaFormat@CRichEditBaseCtrl@@QBEKAAUPARAFORMAT2@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetParaFormat@CRichEditBaseCtrl@@QBEKAAU_paraformat@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SelectionHasEffect@CRichEditBaseCtrl@@QBEHKK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CanEdit@CRichEditBaseCtrl@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HasSelection@CRichEditBaseCtrl@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetFirstVisibleLine@CRichEditBaseCtrl@@QAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetLineHeight@CRichEditBaseCtrl@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetCaretPos@CRichEditBaseCtrl@@QBE?AVCPoint@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CharFromPoint@CRichEditBaseCtrl@@QBEHABVCPoint@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?StreamOutCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?StreamOutLenCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?StreamInCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Save@CRichEditBaseCtrl@@QAEHAAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Load@CRichEditBaseCtrl@@QAEHAAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetRTF@CRichEditBaseCtrl@@QBE?AVCString@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetRTFLength@CRichEditBaseCtrl@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetRTF@CRichEditBaseCtrl@@QAEXABVCString@@@Z
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
;	COMDAT ??8@YAHABU_GUID@@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7IRichEditOleCallback@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CRichEditOleCallback@CRichEditBaseCtrl@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CRichEditBaseCtrl@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS, CRT$XCA, CRT$XCU, CRT$XCL, CRT$XCC, CRT$XCZ
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?OnCreate@CRichEditBaseCtrl@@IAEHPAUtagCREATESTRUCTW@@@Z ; CRichEditBaseCtrl::OnCreate
PUBLIC	?OnFindReplaceCmd@CRichEditBaseCtrl@@IAEJIJ@Z	; CRichEditBaseCtrl::OnFindReplaceCmd
PUBLIC	?OnDestroy@CRichEditBaseCtrl@@IAEXXZ		; CRichEditBaseCtrl::OnDestroy
PUBLIC	?OnSetFocus@CRichEditBaseCtrl@@IAEXPAVCWnd@@@Z	; CRichEditBaseCtrl::OnSetFocus
PUBLIC	?OnEditSetSelection@CRichEditBaseCtrl@@IAEJIJ@Z	; CRichEditBaseCtrl::OnEditSetSelection
PUBLIC	?OnSize@CRichEditBaseCtrl@@IAEXIHH@Z		; CRichEditBaseCtrl::OnSize
PUBLIC	?_messageEntries@CRichEditBaseCtrl@@0QBUAFX_MSGMAP_ENTRY@@B ; CRichEditBaseCtrl::_messageEntries
PUBLIC	?messageMap@CRichEditBaseCtrl@@1UAFX_MSGMAP@@B	; CRichEditBaseCtrl::messageMap
PUBLIC	?_GetBaseMessageMap@CRichEditBaseCtrl@@KGPBUAFX_MSGMAP@@XZ ; CRichEditBaseCtrl::_GetBaseMessageMap
_BSS	SEGMENT
_WM_FINDREPLACE DD 01H DUP (?)
_BSS	ENDS
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\Shared\richeditbasectrl.cpp', 00H
_DATA	ENDS
CRT$XCU	SEGMENT
_$S274	DD	FLAT:_$E273
_$S277	DD	FLAT:_$E276
CRT$XCU	ENDS
CONST	SEGMENT
_DEFAULTRTF DD	FLAT:$SG74879
_RTF_TABLE_HEADER DD FLAT:$SG74881
_RTF_TEXT_INDENT DD FLAT:$SG74883
_RTF_COLUMN_WIDTH DD FLAT:$SG74885
_RTF_CELL_BORDER_TOP DD FLAT:$SG74887
_RTF_CELL_BORDER_LEFT DD FLAT:$SG74889
_RTF_CELL_BORDERS_BOTTOM DD FLAT:$SG74891
_RTF_CELL_BORDERS_RIGHT DD FLAT:$SG74893
_RTF_TABLE_START DD FLAT:$SG74895
_RTF_COLUMN_CELL DD FLAT:$SG74897
_RTF_ROW DD	FLAT:$SG74899
_RTF_TABLE_FOOTER DD FLAT:$SG74901
_RTF_SOFT_RETURN DD FLAT:$SG74903
	ORG $+4
?messageMap@CRichEditBaseCtrl@@1UAFX_MSGMAP@@B DD FLAT:?_GetBaseMessageMap@CRichEditBaseCtrl@@KGPBUAFX_MSGMAP@@XZ ; CRichEditBaseCtrl::messageMap
	DD	FLAT:?_messageEntries@CRichEditBaseCtrl@@0QBUAFX_MSGMAP_ENTRY@@B
?_messageEntries@CRichEditBaseCtrl@@0QBUAFX_MSGMAP_ENTRY@@B DD 01H ; CRichEditBaseCtrl::_messageEntries
	DD	00H
	DD	00H
	DD	00H
	DD	09H
	DD	FLAT:?OnCreate@CRichEditBaseCtrl@@IAEHPAUtagCREATESTRUCTW@@@Z
	DD	02H
	DD	00H
	DD	00H
	DD	00H
	DD	0cH
	DD	FLAT:?OnDestroy@CRichEditBaseCtrl@@IAEXXZ
	DD	0c000H
	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:_WM_FINDREPLACE
	DD	FLAT:?OnFindReplaceCmd@CRichEditBaseCtrl@@IAEJIJ@Z
	DD	07H
	DD	00H
	DD	00H
	DD	00H
	DD	017H
	DD	FLAT:?OnSetFocus@CRichEditBaseCtrl@@IAEXPAVCWnd@@@Z
	DD	0b1H
	DD	00H
	DD	00H
	DD	00H
	DD	0aH
	DD	FLAT:?OnEditSetSelection@CRichEditBaseCtrl@@IAEJIJ@Z
	DD	05H
	DD	00H
	DD	00H
	DD	00H
	DD	011H
	DD	FLAT:?OnSize@CRichEditBaseCtrl@@IAEXIHH@Z
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
	DD	00H
CONST	ENDS
_DATA	SEGMENT
	ORG $+3
$SG74862 DB	'c', 00H, 'o', 00H, 'm', 00H, 'm', 00H, 'd', 00H, 'l', 00H
	DB	'g', 00H, '_', 00H, 'F', 00H, 'i', 00H, 'n', 00H, 'd', 00H, 'R'
	DB	00H, 'e', 00H, 'p', 00H, 'l', 00H, 'a', 00H, 'c', 00H, 'e', 00H
	DB	00H, 00H
$SG74879 DB	'{\rtf1\ansi\deff0\f0\fs60}', 00H
	ORG $+1
$SG74881 DB	'{', 00H, '\', 00H, 'r', 00H, 't', 00H, 'f', 00H, '1', 00H
	DB	'{', 00H, '\', 00H, 'p', 00H, 'a', 00H, 'r', 00H, 'd', 00H, '{'
	DB	00H, '{', 00H, '\', 00H, 't', 00H, 'r', 00H, 'o', 00H, 'w', 00H
	DB	'd', 00H, 00H, 00H
	ORG $+2
$SG74883 DB	'\', 00H, 't', 00H, 'r', 00H, 'g', 00H, 'a', 00H, 'p', 00H
	DB	'h', 00H, '%', 00H, 'd', 00H, 00H, 00H
$SG74885 DB	'\', 00H, 'c', 00H, 'e', 00H, 'l', 00H, 'l', 00H, 'x', 00H
	DB	'%', 00H, 'd', 00H, 00H, 00H
	ORG $+2
$SG74887 DB	'\', 00H, 'c', 00H, 'l', 00H, 'b', 00H, 'r', 00H, 'd', 00H
	DB	'r', 00H, 't', 00H, '\', 00H, 'b', 00H, 'r', 00H, 'd', 00H, 'r'
	DB	00H, 'd', 00H, 'b', 00H, '\', 00H, 'b', 00H, 'r', 00H, 'd', 00H
	DB	'r', 00H, 'w', 00H, '1', 00H, 00H, 00H
	ORG $+2
$SG74889 DB	'\', 00H, 'c', 00H, 'l', 00H, 'b', 00H, 'r', 00H, 'd', 00H
	DB	'r', 00H, 'l', 00H, '\', 00H, 'b', 00H, 'r', 00H, 'd', 00H, 'r'
	DB	00H, 'd', 00H, 'b', 00H, '\', 00H, 'b', 00H, 'r', 00H, 'd', 00H
	DB	'r', 00H, 'w', 00H, '1', 00H, 00H, 00H
	ORG $+2
$SG74891 DB	'\', 00H, 'c', 00H, 'l', 00H, 'b', 00H, 'r', 00H, 'd', 00H
	DB	'r', 00H, 'b', 00H, '\', 00H, 'b', 00H, 'r', 00H, 'd', 00H, 'r'
	DB	00H, 'd', 00H, 'b', 00H, '\', 00H, 'b', 00H, 'r', 00H, 'd', 00H
	DB	'r', 00H, 'w', 00H, '1', 00H, 00H, 00H
	ORG $+2
$SG74893 DB	'\', 00H, 'c', 00H, 'l', 00H, 'b', 00H, 'r', 00H, 'd', 00H
	DB	'r', 00H, 'r', 00H, '\', 00H, 'b', 00H, 'r', 00H, 'd', 00H, 'r'
	DB	00H, 'd', 00H, 'b', 00H, '\', 00H, 'b', 00H, 'r', 00H, 'd', 00H
	DB	'r', 00H, 'w', 00H, '1', 00H, 00H, 00H
	ORG $+2
$SG74895 DB	'\', 00H, 'p', 00H, 'a', 00H, 'r', 00H, 'd', 00H, '\', 00H
	DB	'i', 00H, 'n', 00H, 't', 00H, 'b', 00H, 'l', 00H, 00H, 00H
$SG74897 DB	'\', 00H, 'c', 00H, 'e', 00H, 'l', 00H, 'l', 00H, 00H, 00H
$SG74899 DB	'{', 00H, '\', 00H, 'r', 00H, 'o', 00H, 'w', 00H, '}', 00H
	DB	00H, 00H
	ORG $+2
$SG74901 DB	'}', 00H, '\', 00H, 'p', 00H, 'a', 00H, 'r', 00H, '}', 00H
	DB	'}', 00H, '}', 00H, 00H, 00H
	ORG $+2
$SG74903 DB	'{', 00H, '\', 00H, 'r', 00H, 't', 00H, 'f', 00H, '1', 00H
	DB	'{', 00H, '\', 00H, 'l', 00H, 'i', 00H, 'n', 00H, 'e', 00H, '}'
	DB	00H, 00H, 00H
_DATA	ENDS
EXTRN	__chkesp:NEAR
;	COMDAT _$E273
_TEXT	SEGMENT
_$E273	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E272
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E273	ENDP
_TEXT	ENDS
EXTRN	__imp__RegisterWindowMessageW@4:NEAR
;	COMDAT _$E272
_TEXT	SEGMENT
_$E272	PROC NEAR					; COMDAT

; 24   : const UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

	push	ebp
	mov	ebp, esp
	push	esi
	mov	esi, esp
	push	OFFSET FLAT:$SG74862
	call	DWORD PTR __imp__RegisterWindowMessageW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _WM_FINDREPLACE, eax
	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E272	ENDP
_TEXT	ENDS
;	COMDAT _$E276
_TEXT	SEGMENT
_$E276	PROC NEAR					; COMDAT
	push	ebp
	mov	ebp, esp
	call	_$E275
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E276	ENDP
_TEXT	ENDS
EXTRN	??0CRect@@QAE@HHHH@Z:NEAR			; CRect::CRect
_BSS	SEGMENT
_DEFMARGINS DB	010H DUP (?)
_BSS	ENDS
;	COMDAT _$E275
_TEXT	SEGMENT
_$E275	PROC NEAR					; COMDAT

; 25   : const CRect DEFMARGINS = CRect(8, 4, 8, 0);

	push	ebp
	mov	ebp, esp
	push	0
	push	8
	push	4
	push	8
	mov	ecx, OFFSET FLAT:_DEFMARGINS
	call	??0CRect@@QAE@HHHH@Z			; CRect::CRect
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
_$E275	ENDP
_TEXT	ENDS
PUBLIC	??0CRichEditBaseCtrl@@QAE@H@Z			; CRichEditBaseCtrl::CRichEditBaseCtrl
PUBLIC	??0FIND_STATE@CRichEditBaseCtrl@@QAE@XZ		; CRichEditBaseCtrl::FIND_STATE::FIND_STATE
PUBLIC	??1FIND_STATE@CRichEditBaseCtrl@@QAE@XZ		; CRichEditBaseCtrl::FIND_STATE::~FIND_STATE
PUBLIC	?PreSubclassWindow@CRichEditBaseCtrl@@MAEXXZ	; CRichEditBaseCtrl::PreSubclassWindow
PUBLIC	??0CRichEditOleCallback@CRichEditBaseCtrl@@QAE@XZ ; CRichEditBaseCtrl::CRichEditOleCallback::CRichEditOleCallback
PUBLIC	??1CRichEditOleCallback@CRichEditBaseCtrl@@UAE@XZ ; CRichEditBaseCtrl::CRichEditOleCallback::~CRichEditOleCallback
PUBLIC	?SetOwner@CRichEditOleCallback@CRichEditBaseCtrl@@QAEXPAV2@@Z ; CRichEditBaseCtrl::CRichEditOleCallback::SetOwner
PUBLIC	?GetNewStorage@CRichEditBaseCtrl@@MAEJPAPAUIStorage@@@Z ; CRichEditBaseCtrl::GetNewStorage
PUBLIC	?GetInPlaceContext@CRichEditBaseCtrl@@MAEJPAPAUIOleInPlaceFrame@@PAPAUIOleInPlaceUIWindow@@PAUtagOIFI@@@Z ; CRichEditBaseCtrl::GetInPlaceContext
PUBLIC	?ShowContainerUI@CRichEditBaseCtrl@@MAEJH@Z	; CRichEditBaseCtrl::ShowContainerUI
PUBLIC	?QueryInsertObject@CRichEditBaseCtrl@@MAEJPAU_GUID@@PAUIStorage@@J@Z ; CRichEditBaseCtrl::QueryInsertObject
PUBLIC	?DeleteObject@CRichEditBaseCtrl@@MAEJPAUIOleObject@@@Z ; CRichEditBaseCtrl::DeleteObject
PUBLIC	?QueryAcceptData@CRichEditBaseCtrl@@MAEJPAUIDataObject@@PAGKHPAX@Z ; CRichEditBaseCtrl::QueryAcceptData
PUBLIC	?ContextSensitiveHelp@CRichEditBaseCtrl@@MAEJH@Z ; CRichEditBaseCtrl::ContextSensitiveHelp
PUBLIC	?GetClipboardData@CRichEditBaseCtrl@@MAEJPAU_charrange@@KPAPAUIDataObject@@@Z ; CRichEditBaseCtrl::GetClipboardData
PUBLIC	?GetDragDropEffect@CRichEditBaseCtrl@@MAEJHKPAK@Z ; CRichEditBaseCtrl::GetDragDropEffect
PUBLIC	?GetContextMenu@CRichEditBaseCtrl@@MAEJGPAUIOleObject@@PAU_charrange@@PAPAUHMENU__@@@Z ; CRichEditBaseCtrl::GetContextMenu
PUBLIC	?OnFindNext@CRichEditBaseCtrl@@MAEXPBGHHH@Z	; CRichEditBaseCtrl::OnFindNext
PUBLIC	?OnReplaceSel@CRichEditBaseCtrl@@MAEXPBGHHH0@Z	; CRichEditBaseCtrl::OnReplaceSel
PUBLIC	?OnReplaceAll@CRichEditBaseCtrl@@MAEXPBG0HH@Z	; CRichEditBaseCtrl::OnReplaceAll
PUBLIC	?NewFindReplaceDlg@CRichEditBaseCtrl@@MAEPAVCFindReplaceDialog@@XZ ; CRichEditBaseCtrl::NewFindReplaceDlg
PUBLIC	?GetMessageMap@CRichEditBaseCtrl@@MBEPBUAFX_MSGMAP@@XZ ; CRichEditBaseCtrl::GetMessageMap
PUBLIC	??_7CRichEditBaseCtrl@@6B@			; CRichEditBaseCtrl::`vftable'
PUBLIC	??_GCRichEditBaseCtrl@@UAEPAXI@Z		; CRichEditBaseCtrl::`scalar deleting destructor'
PUBLIC	??_ECRichEditBaseCtrl@@UAEPAXI@Z		; CRichEditBaseCtrl::`vector deleting destructor'
EXTRN	?Create@CWnd@@UAEHPBG0KABUtagRECT@@PAV1@IPAUCCreateContext@@@Z:NEAR ; CWnd::Create
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	?DestroyWindow@CWnd@@UAEHXZ:NEAR		; CWnd::DestroyWindow
EXTRN	?PreCreateWindow@CWnd@@UAEHAAUtagCREATESTRUCTW@@@Z:NEAR ; CWnd::PreCreateWindow
EXTRN	?CalcWindowRect@CWnd@@UAEXPAUtagRECT@@I@Z:NEAR	; CWnd::CalcWindowRect
EXTRN	?GetRuntimeClass@CRichEditCtrl@@UBEPAUCRuntimeClass@@XZ:NEAR ; CRichEditCtrl::GetRuntimeClass
EXTRN	??0CRichEditCtrl@@QAE@XZ:NEAR			; CRichEditCtrl::CRichEditCtrl
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	?OnToolHitTest@CWnd@@UBEHVCPoint@@PAUtagTOOLINFOW@@@Z:NEAR ; CWnd::OnToolHitTest
EXTRN	??1CRichEditCtrl@@UAE@XZ:NEAR			; CRichEditCtrl::~CRichEditCtrl
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
EXTRN	?PreTranslateMessage@CWnd@@UAEHPAUtagMSG@@@Z:NEAR ; CWnd::PreTranslateMessage
EXTRN	?OnAmbientProperty@CWnd@@UAEHPAVCOleControlSite@@JPAUtagVARIANT@@@Z:NEAR ; CWnd::OnAmbientProperty
EXTRN	?WindowProc@CWnd@@MAEJIIJ@Z:NEAR		; CWnd::WindowProc
EXTRN	?OnWndMsg@CWnd@@MAEHIIJPAJ@Z:NEAR		; CWnd::OnWndMsg
EXTRN	?DefWindowProcW@CWnd@@MAEJIIJ@Z:NEAR		; CWnd::DefWindowProcW
EXTRN	?PostNcDestroy@CWnd@@MAEXXZ:NEAR		; CWnd::PostNcDestroy
EXTRN	?OnChildNotify@CWnd@@MAEHIIJPAJ@Z:NEAR		; CWnd::OnChildNotify
EXTRN	?CheckAutoCenter@CWnd@@UAEHXZ:NEAR		; CWnd::CheckAutoCenter
EXTRN	?AssertValid@CWnd@@UBEXXZ:NEAR			; CWnd::AssertValid
EXTRN	?Dump@CWnd@@UBEXAAVCDumpContext@@@Z:NEAR	; CWnd::Dump
EXTRN	?GetExtraConnectionPoints@CCmdTarget@@MAEHPAVCPtrArray@@@Z:NEAR ; CCmdTarget::GetExtraConnectionPoints
EXTRN	?GetConnectionHook@CCmdTarget@@MAEPAUIConnectionPoint@@ABU_GUID@@@Z:NEAR ; CCmdTarget::GetConnectionHook
EXTRN	?IsFrameWnd@CWnd@@UBEHXZ:NEAR			; CWnd::IsFrameWnd
EXTRN	?OnFinalRelease@CWnd@@UAEXXZ:NEAR		; CWnd::OnFinalRelease
EXTRN	?SetOccDialogInfo@CWnd@@MAEHPAU_AFX_OCC_DIALOG_INFO@@@Z:NEAR ; CWnd::SetOccDialogInfo
;	COMDAT ??_7CRichEditBaseCtrl@@6B@
; File D:\_CODE\Shared\richeditbasectrl.cpp
CONST	SEGMENT
??_7CRichEditBaseCtrl@@6B@ DD FLAT:?GetRuntimeClass@CRichEditCtrl@@UBEPAUCRuntimeClass@@XZ ; CRichEditBaseCtrl::`vftable'
	DD	FLAT:??_ECRichEditBaseCtrl@@UAEPAXI@Z
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
	DD	FLAT:?GetMessageMap@CRichEditBaseCtrl@@MBEPBUAFX_MSGMAP@@XZ
	DD	FLAT:?GetCommandMap@CCmdTarget@@MBEPBUAFX_OLECMDMAP@@XZ
	DD	FLAT:?GetDispatchMap@CCmdTarget@@MBEPBUAFX_DISPMAP@@XZ
	DD	FLAT:?GetConnectionMap@CCmdTarget@@MBEPBUAFX_CONNECTIONMAP@@XZ
	DD	FLAT:?GetInterfaceMap@CCmdTarget@@MBEPBUAFX_INTERFACEMAP@@XZ
	DD	FLAT:?GetEventSinkMap@CCmdTarget@@MBEPBUAFX_EVENTSINKMAP@@XZ
	DD	FLAT:?OnCreateAggregates@CCmdTarget@@UAEHXZ
	DD	FLAT:?GetInterfaceHook@CCmdTarget@@UAEPAUIUnknown@@PBX@Z
	DD	FLAT:?GetExtraConnectionPoints@CCmdTarget@@MAEHPAVCPtrArray@@@Z
	DD	FLAT:?GetConnectionHook@CCmdTarget@@MAEPAUIConnectionPoint@@ABU_GUID@@@Z
	DD	FLAT:?PreSubclassWindow@CRichEditBaseCtrl@@MAEXXZ
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
	DD	FLAT:?PreTranslateMessage@CWnd@@UAEHPAUtagMSG@@@Z
	DD	FLAT:?OnAmbientProperty@CWnd@@UAEHPAVCOleControlSite@@JPAUtagVARIANT@@@Z
	DD	FLAT:?WindowProc@CWnd@@MAEJIIJ@Z
	DD	FLAT:?OnWndMsg@CWnd@@MAEHIIJPAJ@Z
	DD	FLAT:?DefWindowProcW@CWnd@@MAEJIIJ@Z
	DD	FLAT:?PostNcDestroy@CWnd@@MAEXXZ
	DD	FLAT:?OnChildNotify@CWnd@@MAEHIIJPAJ@Z
	DD	FLAT:?CheckAutoCenter@CWnd@@UAEHXZ
	DD	FLAT:?IsFrameWnd@CWnd@@UBEHXZ
	DD	FLAT:?SetOccDialogInfo@CWnd@@MAEHPAU_AFX_OCC_DIALOG_INFO@@@Z
	DD	FLAT:?GetNewStorage@CRichEditBaseCtrl@@MAEJPAPAUIStorage@@@Z
	DD	FLAT:?GetInPlaceContext@CRichEditBaseCtrl@@MAEJPAPAUIOleInPlaceFrame@@PAPAUIOleInPlaceUIWindow@@PAUtagOIFI@@@Z
	DD	FLAT:?ShowContainerUI@CRichEditBaseCtrl@@MAEJH@Z
	DD	FLAT:?QueryInsertObject@CRichEditBaseCtrl@@MAEJPAU_GUID@@PAUIStorage@@J@Z
	DD	FLAT:?DeleteObject@CRichEditBaseCtrl@@MAEJPAUIOleObject@@@Z
	DD	FLAT:?QueryAcceptData@CRichEditBaseCtrl@@MAEJPAUIDataObject@@PAGKHPAX@Z
	DD	FLAT:?ContextSensitiveHelp@CRichEditBaseCtrl@@MAEJH@Z
	DD	FLAT:?GetClipboardData@CRichEditBaseCtrl@@MAEJPAU_charrange@@KPAPAUIDataObject@@@Z
	DD	FLAT:?GetDragDropEffect@CRichEditBaseCtrl@@MAEJHKPAK@Z
	DD	FLAT:?GetContextMenu@CRichEditBaseCtrl@@MAEJGPAUIOleObject@@PAU_charrange@@PAPAUHMENU__@@@Z
	DD	FLAT:?OnFindNext@CRichEditBaseCtrl@@MAEXPBGHHH@Z
	DD	FLAT:?OnReplaceSel@CRichEditBaseCtrl@@MAEXPBGHHH0@Z
	DD	FLAT:?OnReplaceAll@CRichEditBaseCtrl@@MAEXPBG0HH@Z
	DD	FLAT:?NewFindReplaceDlg@CRichEditBaseCtrl@@MAEPAVCFindReplaceDialog@@XZ
CONST	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T75956	DD	019930520H
	DD	03H
	DD	FLAT:$T75959
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75959	DD	0ffffffffH
	DD	FLAT:$L75950
	DD	00H
	DD	FLAT:$L75951
	DD	01H
	DD	FLAT:$L75952
xdata$x	ENDS
;	COMDAT ??0CRichEditBaseCtrl@@QAE@H@Z
_TEXT	SEGMENT
_bAutoRTL$ = 8
_this$ = -16
__$EHRec$ = -12
??0CRichEditBaseCtrl@@QAE@H@Z PROC NEAR			; CRichEditBaseCtrl::CRichEditBaseCtrl, COMDAT

; 72   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75957
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CRichEditCtrl@@QAE@XZ		; CRichEditCtrl::CRichEditCtrl
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+64], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+68], 0
	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 72					; 00000048H
	mov	eax, DWORD PTR _DEFMARGINS
	mov	DWORD PTR [edx], eax
	mov	ecx, DWORD PTR _DEFMARGINS+4
	mov	DWORD PTR [edx+4], ecx
	mov	eax, DWORD PTR _DEFMARGINS+8
	mov	DWORD PTR [edx+8], eax
	mov	ecx, DWORD PTR _DEFMARGINS+12
	mov	DWORD PTR [edx+12], ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _bAutoRTL$[ebp]
	mov	DWORD PTR [edx+88], eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 92					; 0000005cH
	call	??0FIND_STATE@CRichEditBaseCtrl@@QAE@XZ	; CRichEditBaseCtrl::FIND_STATE::FIND_STATE
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 120				; 00000078H
	call	??0CRichEditOleCallback@CRichEditBaseCtrl@@QAE@XZ ; CRichEditBaseCtrl::CRichEditOleCallback::CRichEditOleCallback
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CRichEditBaseCtrl@@6B@ ; CRichEditBaseCtrl::`vftable'

; 73   :    m_callback.SetOwner(this);

	mov	edx, DWORD PTR _this$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 120				; 00000078H
	call	?SetOwner@CRichEditOleCallback@CRichEditBaseCtrl@@QAEXPAV2@@Z ; CRichEditBaseCtrl::CRichEditOleCallback::SetOwner

; 74   : }

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
$L75950:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CRichEditCtrl@@UAE@XZ		; CRichEditCtrl::~CRichEditCtrl
	ret	0
$L75951:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 92					; 0000005cH
	call	??1FIND_STATE@CRichEditBaseCtrl@@QAE@XZ	; CRichEditBaseCtrl::FIND_STATE::~FIND_STATE
	ret	0
$L75952:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 120				; 00000078H
	call	??1CRichEditOleCallback@CRichEditBaseCtrl@@UAE@XZ ; CRichEditBaseCtrl::CRichEditOleCallback::~CRichEditOleCallback
	ret	0
$L75957:
	mov	eax, OFFSET FLAT:$T75956
	jmp	___CxxFrameHandler
text$x	ENDS
??0CRichEditBaseCtrl@@QAE@H@Z ENDP			; CRichEditBaseCtrl::CRichEditBaseCtrl
EXTRN	??0CString@@QAE@XZ:NEAR				; CString::CString
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT xdata$x
; File D:\_CODE\Shared\richeditbasectrl.h
xdata$x	SEGMENT
$T75968	DD	019930520H
	DD	01H
	DD	FLAT:$T75970
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T75970	DD	0ffffffffH
	DD	FLAT:$L75965
xdata$x	ENDS
;	COMDAT ??0FIND_STATE@CRichEditBaseCtrl@@QAE@XZ
_TEXT	SEGMENT
__$EHRec$ = -12
_this$ = -16
??0FIND_STATE@CRichEditBaseCtrl@@QAE@XZ PROC NEAR	; CRichEditBaseCtrl::FIND_STATE::FIND_STATE, COMDAT

; 117  : 		FIND_STATE() : pFindReplaceDlg(NULL), bFindOnly(FALSE), bCase(FALSE), bNext(TRUE), bWord(FALSE) { }

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L75969
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+4], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	call	??0CString@@QAE@XZ			; CString::CString
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+16], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+20], 1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+24], 0
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
$L75965:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L75969:
	mov	eax, OFFSET FLAT:$T75968
	jmp	___CxxFrameHandler
text$x	ENDS
??0FIND_STATE@CRichEditBaseCtrl@@QAE@XZ ENDP		; CRichEditBaseCtrl::FIND_STATE::FIND_STATE
;	COMDAT ?SetOwner@CRichEditOleCallback@CRichEditBaseCtrl@@QAEXPAV2@@Z
_TEXT	SEGMENT
_pOwner$ = 8
_this$ = -4
?SetOwner@CRichEditOleCallback@CRichEditBaseCtrl@@QAEXPAV2@@Z PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::SetOwner, COMDAT

; 147  : 		void SetOwner(CRichEditBaseCtrl* pOwner) { m_pOwner = pOwner; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _pOwner$[ebp]
	mov	DWORD PTR [eax+16], ecx
	mov	esp, ebp
	pop	ebp
	ret	4
?SetOwner@CRichEditOleCallback@CRichEditBaseCtrl@@QAEXPAV2@@Z ENDP ; CRichEditBaseCtrl::CRichEditOleCallback::SetOwner
_TEXT	ENDS
;	COMDAT ?GetNewStorage@CRichEditBaseCtrl@@MAEJPAPAUIStorage@@@Z
_TEXT	SEGMENT
_this$ = -4
?GetNewStorage@CRichEditBaseCtrl@@MAEJPAPAUIStorage@@@Z PROC NEAR ; CRichEditBaseCtrl::GetNewStorage, COMDAT

; 181  : 	virtual HRESULT GetNewStorage(LPSTORAGE* /*lplpstg*/) { return S_OK; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	xor	eax, eax
	mov	esp, ebp
	pop	ebp
	ret	4
?GetNewStorage@CRichEditBaseCtrl@@MAEJPAPAUIStorage@@@Z ENDP ; CRichEditBaseCtrl::GetNewStorage
_TEXT	ENDS
;	COMDAT ?GetInPlaceContext@CRichEditBaseCtrl@@MAEJPAPAUIOleInPlaceFrame@@PAPAUIOleInPlaceUIWindow@@PAUtagOIFI@@@Z
_TEXT	SEGMENT
_this$ = -4
?GetInPlaceContext@CRichEditBaseCtrl@@MAEJPAPAUIOleInPlaceFrame@@PAPAUIOleInPlaceUIWindow@@PAUtagOIFI@@@Z PROC NEAR ; CRichEditBaseCtrl::GetInPlaceContext, COMDAT

; 183  : 		LPOLEINPLACEUIWINDOW FAR* /*lplpDoc*/, LPOLEINPLACEFRAMEINFO /*lpFrameInfo*/) { return S_FALSE; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, 1
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?GetInPlaceContext@CRichEditBaseCtrl@@MAEJPAPAUIOleInPlaceFrame@@PAPAUIOleInPlaceUIWindow@@PAUtagOIFI@@@Z ENDP ; CRichEditBaseCtrl::GetInPlaceContext
_TEXT	ENDS
;	COMDAT ?ShowContainerUI@CRichEditBaseCtrl@@MAEJH@Z
_TEXT	SEGMENT
_this$ = -4
?ShowContainerUI@CRichEditBaseCtrl@@MAEJH@Z PROC NEAR	; CRichEditBaseCtrl::ShowContainerUI, COMDAT

; 184  : 	virtual HRESULT ShowContainerUI(BOOL /*fShow*/) { return S_FALSE; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, 1
	mov	esp, ebp
	pop	ebp
	ret	4
?ShowContainerUI@CRichEditBaseCtrl@@MAEJH@Z ENDP	; CRichEditBaseCtrl::ShowContainerUI
_TEXT	ENDS
;	COMDAT ?QueryInsertObject@CRichEditBaseCtrl@@MAEJPAU_GUID@@PAUIStorage@@J@Z
_TEXT	SEGMENT
_this$ = -4
?QueryInsertObject@CRichEditBaseCtrl@@MAEJPAU_GUID@@PAUIStorage@@J@Z PROC NEAR ; CRichEditBaseCtrl::QueryInsertObject, COMDAT

; 185  : 	virtual HRESULT QueryInsertObject(LPCLSID /*lpclsid*/, LPSTORAGE /*lpstg*/, LONG /*cp*/) { return S_OK; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	xor	eax, eax
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?QueryInsertObject@CRichEditBaseCtrl@@MAEJPAU_GUID@@PAUIStorage@@J@Z ENDP ; CRichEditBaseCtrl::QueryInsertObject
_TEXT	ENDS
;	COMDAT ?DeleteObject@CRichEditBaseCtrl@@MAEJPAUIOleObject@@@Z
_TEXT	SEGMENT
_this$ = -4
?DeleteObject@CRichEditBaseCtrl@@MAEJPAUIOleObject@@@Z PROC NEAR ; CRichEditBaseCtrl::DeleteObject, COMDAT

; 186  : 	virtual HRESULT DeleteObject(LPOLEOBJECT /*lpoleobj*/) { return S_OK; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	xor	eax, eax
	mov	esp, ebp
	pop	ebp
	ret	4
?DeleteObject@CRichEditBaseCtrl@@MAEJPAUIOleObject@@@Z ENDP ; CRichEditBaseCtrl::DeleteObject
_TEXT	ENDS
;	COMDAT ?QueryAcceptData@CRichEditBaseCtrl@@MAEJPAUIDataObject@@PAGKHPAX@Z
_TEXT	SEGMENT
_this$ = -4
?QueryAcceptData@CRichEditBaseCtrl@@MAEJPAUIDataObject@@PAGKHPAX@Z PROC NEAR ; CRichEditBaseCtrl::QueryAcceptData, COMDAT

; 188  : 		DWORD /*reco*/, BOOL /*fReally*/, HGLOBAL /*hMetaPict*/) { return S_OK; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	xor	eax, eax
	mov	esp, ebp
	pop	ebp
	ret	20					; 00000014H
?QueryAcceptData@CRichEditBaseCtrl@@MAEJPAUIDataObject@@PAGKHPAX@Z ENDP ; CRichEditBaseCtrl::QueryAcceptData
_TEXT	ENDS
;	COMDAT ?ContextSensitiveHelp@CRichEditBaseCtrl@@MAEJH@Z
_TEXT	SEGMENT
_this$ = -4
?ContextSensitiveHelp@CRichEditBaseCtrl@@MAEJH@Z PROC NEAR ; CRichEditBaseCtrl::ContextSensitiveHelp, COMDAT

; 189  : 	virtual HRESULT ContextSensitiveHelp(BOOL /*fEnterMode*/) { return S_OK; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	xor	eax, eax
	mov	esp, ebp
	pop	ebp
	ret	4
?ContextSensitiveHelp@CRichEditBaseCtrl@@MAEJH@Z ENDP	; CRichEditBaseCtrl::ContextSensitiveHelp
_TEXT	ENDS
;	COMDAT ?GetClipboardData@CRichEditBaseCtrl@@MAEJPAU_charrange@@KPAPAUIDataObject@@@Z
_TEXT	SEGMENT
_this$ = -4
?GetClipboardData@CRichEditBaseCtrl@@MAEJPAU_charrange@@KPAPAUIDataObject@@@Z PROC NEAR ; CRichEditBaseCtrl::GetClipboardData, COMDAT

; 190  : 	virtual HRESULT GetClipboardData(CHARRANGE FAR* /*lpchrg*/, DWORD /*reco*/, LPDATAOBJECT FAR* /*lplpdataobj*/) { return E_NOTIMPL; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, -2147467263			; 80004001H
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?GetClipboardData@CRichEditBaseCtrl@@MAEJPAU_charrange@@KPAPAUIDataObject@@@Z ENDP ; CRichEditBaseCtrl::GetClipboardData
_TEXT	ENDS
;	COMDAT ?GetDragDropEffect@CRichEditBaseCtrl@@MAEJHKPAK@Z
_TEXT	SEGMENT
_this$ = -4
?GetDragDropEffect@CRichEditBaseCtrl@@MAEJHKPAK@Z PROC NEAR ; CRichEditBaseCtrl::GetDragDropEffect, COMDAT

; 191  : 	virtual HRESULT GetDragDropEffect(BOOL /*fDrag*/, DWORD /*grfKeyState*/, LPDWORD /*pdwEffect*/) { return S_OK; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	xor	eax, eax
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?GetDragDropEffect@CRichEditBaseCtrl@@MAEJHKPAK@Z ENDP	; CRichEditBaseCtrl::GetDragDropEffect
_TEXT	ENDS
;	COMDAT ?GetContextMenu@CRichEditBaseCtrl@@MAEJGPAUIOleObject@@PAU_charrange@@PAPAUHMENU__@@@Z
_TEXT	SEGMENT
_this$ = -4
?GetContextMenu@CRichEditBaseCtrl@@MAEJGPAUIOleObject@@PAU_charrange@@PAPAUHMENU__@@@Z PROC NEAR ; CRichEditBaseCtrl::GetContextMenu, COMDAT

; 193  : 		HMENU FAR* /*lphmenu*/) { return S_OK; }

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	xor	eax, eax
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?GetContextMenu@CRichEditBaseCtrl@@MAEJGPAUIOleObject@@PAU_charrange@@PAPAUHMENU__@@@Z ENDP ; CRichEditBaseCtrl::GetContextMenu
_TEXT	ENDS
EXTRN	??2CObject@@SGPAXI@Z:NEAR			; CObject::operator new
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
EXTRN	??0CFindReplaceDialog@@QAE@XZ:NEAR		; CFindReplaceDialog::CFindReplaceDialog
;	COMDAT xdata$x
; File D:\_CODE\Shared\richeditbasectrl.h
xdata$x	SEGMENT
$T76003	DD	019930520H
	DD	01H
	DD	FLAT:$T76005
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76005	DD	0ffffffffH
	DD	FLAT:$L76001
xdata$x	ENDS
;	COMDAT ?NewFindReplaceDlg@CRichEditBaseCtrl@@MAEPAVCFindReplaceDialog@@XZ
_TEXT	SEGMENT
$T75997 = -16
$T75998 = -20
__$EHRec$ = -12
_this$ = -24
?NewFindReplaceDlg@CRichEditBaseCtrl@@MAEPAVCFindReplaceDialog@@XZ PROC NEAR ; CRichEditBaseCtrl::NewFindReplaceDlg, COMDAT

; 200  : 	virtual CFindReplaceDialog* NewFindReplaceDlg() { return new CFindReplaceDialog; }

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76004
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
	push	648					; 00000288H
	call	??2CObject@@SGPAXI@Z			; CObject::operator new
	mov	DWORD PTR $T75998[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	cmp	DWORD PTR $T75998[ebp], 0
	je	SHORT $L75999
	mov	ecx, DWORD PTR $T75998[ebp]
	call	??0CFindReplaceDialog@@QAE@XZ		; CFindReplaceDialog::CFindReplaceDialog
	mov	DWORD PTR -28+[ebp], eax
	jmp	SHORT $L76000
$L75999:
	mov	DWORD PTR -28+[ebp], 0
$L76000:
	mov	eax, DWORD PTR -28+[ebp]
	mov	DWORD PTR $T75997[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	eax, DWORD PTR $T75997[ebp]
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
$L76001:
	mov	eax, DWORD PTR $T75998[ebp]
	push	eax
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
	ret	0
$L76004:
	mov	eax, OFFSET FLAT:$T76003
	jmp	___CxxFrameHandler
text$x	ENDS
?NewFindReplaceDlg@CRichEditBaseCtrl@@MAEPAVCFindReplaceDialog@@XZ ENDP ; CRichEditBaseCtrl::NewFindReplaceDlg
PUBLIC	??1CRichEditBaseCtrl@@UAE@XZ			; CRichEditBaseCtrl::~CRichEditBaseCtrl
;	COMDAT ??_GCRichEditBaseCtrl@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCRichEditBaseCtrl@@UAEPAXI@Z PROC NEAR		; CRichEditBaseCtrl::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CRichEditBaseCtrl@@UAE@XZ		; CRichEditBaseCtrl::~CRichEditBaseCtrl
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L74931
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L74931:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCRichEditBaseCtrl@@UAEPAXI@Z ENDP			; CRichEditBaseCtrl::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76015	DD	019930520H
	DD	01H
	DD	FLAT:$T76017
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76017	DD	0ffffffffH
	DD	FLAT:$L76012
xdata$x	ENDS
;	COMDAT ??1FIND_STATE@CRichEditBaseCtrl@@QAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1FIND_STATE@CRichEditBaseCtrl@@QAE@XZ PROC NEAR	; CRichEditBaseCtrl::FIND_STATE::~FIND_STATE, COMDAT
	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76016
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 12					; 0000000cH
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??1CString@@QAE@XZ			; CString::~CString
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
$L76012:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 8
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76016:
	mov	eax, OFFSET FLAT:$T76015
	jmp	___CxxFrameHandler
text$x	ENDS
??1FIND_STATE@CRichEditBaseCtrl@@QAE@XZ ENDP		; CRichEditBaseCtrl::FIND_STATE::~FIND_STATE
;	COMDAT xdata$x
; File D:\_CODE\Shared\richeditbasectrl.cpp
xdata$x	SEGMENT
$T76026	DD	019930520H
	DD	03H
	DD	FLAT:$T76028
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76028	DD	0ffffffffH
	DD	FLAT:$L76022
	DD	00H
	DD	FLAT:$L76023
	DD	01H
	DD	FLAT:$L76024
xdata$x	ENDS
;	COMDAT ??1CRichEditBaseCtrl@@UAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1CRichEditBaseCtrl@@UAE@XZ PROC NEAR			; CRichEditBaseCtrl::~CRichEditBaseCtrl, COMDAT

; 77   : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76027
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CRichEditBaseCtrl@@6B@ ; CRichEditBaseCtrl::`vftable'
	mov	DWORD PTR __$EHRec$[ebp+8], 2

; 78   :    m_callback.Release();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 120				; 00000078H
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+120]
	mov	esi, esp
	push	ecx
	call	DWORD PTR [eax+8]
	cmp	esi, esp
	call	__chkesp

; 79   : }

	mov	BYTE PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 120				; 00000078H
	call	??1CRichEditOleCallback@CRichEditBaseCtrl@@UAE@XZ ; CRichEditBaseCtrl::CRichEditOleCallback::~CRichEditOleCallback
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 92					; 0000005cH
	call	??1FIND_STATE@CRichEditBaseCtrl@@QAE@XZ	; CRichEditBaseCtrl::FIND_STATE::~FIND_STATE
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CRichEditCtrl@@UAE@XZ		; CRichEditCtrl::~CRichEditCtrl
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76022:
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CRichEditCtrl@@UAE@XZ		; CRichEditCtrl::~CRichEditCtrl
	ret	0
$L76023:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 92					; 0000005cH
	call	??1FIND_STATE@CRichEditBaseCtrl@@QAE@XZ	; CRichEditBaseCtrl::FIND_STATE::~FIND_STATE
	ret	0
$L76024:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 120				; 00000078H
	call	??1CRichEditOleCallback@CRichEditBaseCtrl@@UAE@XZ ; CRichEditBaseCtrl::CRichEditOleCallback::~CRichEditOleCallback
	ret	0
$L76027:
	mov	eax, OFFSET FLAT:$T76026
	jmp	___CxxFrameHandler
text$x	ENDS
??1CRichEditBaseCtrl@@UAE@XZ ENDP			; CRichEditBaseCtrl::~CRichEditBaseCtrl
EXTRN	__imp_?messageMap@CWnd@@1UAFX_MSGMAP@@B:QWORD
;	COMDAT ?_GetBaseMessageMap@CRichEditBaseCtrl@@KGPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
?_GetBaseMessageMap@CRichEditBaseCtrl@@KGPBUAFX_MSGMAP@@XZ PROC NEAR ; CRichEditBaseCtrl::_GetBaseMessageMap, COMDAT

; 82   : BEGIN_MESSAGE_MAP(CRichEditBaseCtrl, CRichEditCtrl)

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR __imp_?messageMap@CWnd@@1UAFX_MSGMAP@@B
	pop	ebp
	ret	0
?_GetBaseMessageMap@CRichEditBaseCtrl@@KGPBUAFX_MSGMAP@@XZ ENDP ; CRichEditBaseCtrl::_GetBaseMessageMap
_TEXT	ENDS
;	COMDAT ?GetMessageMap@CRichEditBaseCtrl@@MBEPBUAFX_MSGMAP@@XZ
_TEXT	SEGMENT
_this$ = -4
?GetMessageMap@CRichEditBaseCtrl@@MBEPBUAFX_MSGMAP@@XZ PROC NEAR ; CRichEditBaseCtrl::GetMessageMap, COMDAT

; 82   : BEGIN_MESSAGE_MAP(CRichEditBaseCtrl, CRichEditCtrl)

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, OFFSET FLAT:?messageMap@CRichEditBaseCtrl@@1UAFX_MSGMAP@@B ; CRichEditBaseCtrl::messageMap
	mov	esp, ebp
	pop	ebp
	ret	0
?GetMessageMap@CRichEditBaseCtrl@@MBEPBUAFX_MSGMAP@@XZ ENDP ; CRichEditBaseCtrl::GetMessageMap
_TEXT	ENDS
PUBLIC	?Create@CRichEditBaseCtrl@@QAEHKABUtagRECT@@PAVCWnd@@IK@Z ; CRichEditBaseCtrl::Create
EXTRN	?CreateRichEdit20@CRichEditHelper@@SAHAAVCWnd@@KABUtagRECT@@PAV2@IKH@Z:NEAR ; CRichEditHelper::CreateRichEdit20
EXTRN	?CreateRichEdit50@CRichEditHelper@@SAHAAVCWnd@@KABUtagRECT@@PAV2@IKH@Z:NEAR ; CRichEditHelper::CreateRichEdit50
;	COMDAT ?Create@CRichEditBaseCtrl@@QAEHKABUtagRECT@@PAVCWnd@@IK@Z
_TEXT	SEGMENT
_dwStyle$ = 8
_rect$ = 12
_pParentWnd$ = 16
_nID$ = 20
_dwExStyle$ = 24
_this$ = -4
?Create@CRichEditBaseCtrl@@QAEHKABUtagRECT@@PAVCWnd@@IK@Z PROC NEAR ; CRichEditBaseCtrl::Create, COMDAT

; 97   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 98   : 	return (CRichEditHelper::CreateRichEdit50(*this, dwStyle, rect, pParentWnd, nID, dwExStyle, m_bAutoRTL) ||
; 99   : 			CRichEditHelper::CreateRichEdit20(*this, dwStyle, rect, pParentWnd, nID, dwExStyle, m_bAutoRTL));

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+88]
	push	ecx
	mov	edx, DWORD PTR _dwExStyle$[ebp]
	push	edx
	mov	eax, DWORD PTR _nID$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pParentWnd$[ebp]
	push	ecx
	mov	edx, DWORD PTR _rect$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwStyle$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	?CreateRichEdit50@CRichEditHelper@@SAHAAVCWnd@@KABUtagRECT@@PAV2@IKH@Z ; CRichEditHelper::CreateRichEdit50
	add	esp, 28					; 0000001cH
	test	eax, eax
	jne	SHORT $L76037
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+88]
	push	eax
	mov	ecx, DWORD PTR _dwExStyle$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nID$[ebp]
	push	edx
	mov	eax, DWORD PTR _pParentWnd$[ebp]
	push	eax
	mov	ecx, DWORD PTR _rect$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwStyle$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?CreateRichEdit20@CRichEditHelper@@SAHAAVCWnd@@KABUtagRECT@@PAV2@IKH@Z ; CRichEditHelper::CreateRichEdit20
	add	esp, 28					; 0000001cH
	test	eax, eax
	jne	SHORT $L76037
	mov	DWORD PTR -8+[ebp], 0
	jmp	SHORT $L76038
$L76037:
	mov	DWORD PTR -8+[ebp], 1
$L76038:
	mov	eax, DWORD PTR -8+[ebp]

; 100  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	20					; 00000014H
?Create@CRichEditBaseCtrl@@QAEHKABUtagRECT@@PAVCWnd@@IK@Z ENDP ; CRichEditBaseCtrl::Create
_TEXT	ENDS
EXTRN	?SetOLECallback@CRichEditCtrl@@QAEHPAUIRichEditOleCallback@@@Z:NEAR ; CRichEditCtrl::SetOLECallback
EXTRN	?OnCreate@CWnd@@IAEHPAUtagCREATESTRUCTW@@@Z:NEAR ; CWnd::OnCreate
EXTRN	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z:NEAR ; AfxAssertValidObject
;	COMDAT ?OnCreate@CRichEditBaseCtrl@@IAEHPAUtagCREATESTRUCTW@@@Z
_TEXT	SEGMENT
_lpCreateStruct$ = 8
_this$ = -4
?OnCreate@CRichEditBaseCtrl@@IAEHPAUtagCREATESTRUCTW@@@Z PROC NEAR ; CRichEditBaseCtrl::OnCreate, COMDAT

; 103  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 104  : 	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)

	mov	eax, DWORD PTR _lpCreateStruct$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnCreate@CWnd@@IAEHPAUtagCREATESTRUCTW@@@Z ; CWnd::OnCreate
	cmp	eax, -1
	jne	SHORT $L74985

; 105  : 		return -1;

	or	eax, -1
	jmp	SHORT $L74984
$L74985:

; 106  : 
; 107  : 	ASSERT_VALID(this);

	push	107					; 0000006bH
	push	OFFSET FLAT:_THIS_FILE
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 108  : 	
; 109  : 	SetOLECallback(&m_callback);

	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 120				; 00000078H
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetOLECallback@CRichEditCtrl@@QAEHPAUIRichEditOleCallback@@@Z ; CRichEditCtrl::SetOLECallback

; 110  : 	
; 111  : 	return 0;

	xor	eax, eax
$L74984:

; 112  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?OnCreate@CRichEditBaseCtrl@@IAEHPAUtagCREATESTRUCTW@@@Z ENDP ; CRichEditBaseCtrl::OnCreate
_TEXT	ENDS
EXTRN	?OnSetFocus@CWnd@@IAEXPAV1@@Z:NEAR		; CWnd::OnSetFocus
EXTRN	??0CAutoFlag@@QAE@AAHH@Z:NEAR			; CAutoFlag::CAutoFlag
EXTRN	??1CAutoFlag@@UAE@XZ:NEAR			; CAutoFlag::~CAutoFlag
;	COMDAT xdata$x
; File D:\_CODE\Shared\richeditbasectrl.cpp
xdata$x	SEGMENT
$T76045	DD	019930520H
	DD	01H
	DD	FLAT:$T76047
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76047	DD	0ffffffffH
	DD	FLAT:$L76043
xdata$x	ENDS
;	COMDAT ?OnSetFocus@CRichEditBaseCtrl@@IAEXPAVCWnd@@@Z
_TEXT	SEGMENT
_pOldWnd$ = 8
_this$ = -28
_af$ = -24
__$EHRec$ = -12
?OnSetFocus@CRichEditBaseCtrl@@IAEXPAVCWnd@@@Z PROC NEAR ; CRichEditBaseCtrl::OnSetFocus, COMDAT

; 115  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76046
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

; 116  : 	CAutoFlag af(m_bInOnFocus, TRUE);

	push	1
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 68					; 00000044H
	push	eax
	lea	ecx, DWORD PTR _af$[ebp]
	call	??0CAutoFlag@@QAE@AAHH@Z		; CAutoFlag::CAutoFlag
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 117  : 
; 118  : 	CRichEditCtrl::OnSetFocus(pOldWnd);

	mov	ecx, DWORD PTR _pOldWnd$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnSetFocus@CWnd@@IAEXPAV1@@Z		; CWnd::OnSetFocus

; 119  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _af$[ebp]
	call	??1CAutoFlag@@UAE@XZ			; CAutoFlag::~CAutoFlag
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
$L76043:
	lea	ecx, DWORD PTR _af$[ebp]
	call	??1CAutoFlag@@UAE@XZ			; CAutoFlag::~CAutoFlag
	ret	0
$L76046:
	mov	eax, OFFSET FLAT:$T76045
	jmp	___CxxFrameHandler
text$x	ENDS
?OnSetFocus@CRichEditBaseCtrl@@IAEXPAVCWnd@@@Z ENDP	; CRichEditBaseCtrl::OnSetFocus
EXTRN	?Default@CWnd@@IAEJXZ:NEAR			; CWnd::Default
;	COMDAT ?OnEditSetSelection@CRichEditBaseCtrl@@IAEJIJ@Z
_TEXT	SEGMENT
_this$ = -4
?OnEditSetSelection@CRichEditBaseCtrl@@IAEJIJ@Z PROC NEAR ; CRichEditBaseCtrl::OnEditSetSelection, COMDAT

; 122  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 123  : 	if (m_bEnableSelectOnFocus || !m_bInOnFocus)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+64], 0
	jne	SHORT $L74997
	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+68], 0
	jne	SHORT $L74996
$L74997:

; 124  : 		return Default();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?Default@CWnd@@IAEJXZ			; CWnd::Default
	jmp	SHORT $L74995
$L74996:

; 125  : 
; 126  : 	// else
; 127  : 	return 0L;

	xor	eax, eax
$L74995:

; 128  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?OnEditSetSelection@CRichEditBaseCtrl@@IAEJIJ@Z ENDP	; CRichEditBaseCtrl::OnEditSetSelection
_TEXT	ENDS
EXTRN	?OnDestroy@CWnd@@IAEXXZ:NEAR			; CWnd::OnDestroy
;	COMDAT ?OnDestroy@CRichEditBaseCtrl@@IAEXXZ
_TEXT	SEGMENT
_this$ = -4
?OnDestroy@CRichEditBaseCtrl@@IAEXXZ PROC NEAR		; CRichEditBaseCtrl::OnDestroy, COMDAT

; 131  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 132  : 	// destroy the find dialog. it will delete itself
; 133  : 	if (m_findState.pFindReplaceDlg)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+92], 0
	je	SHORT $L75001

; 135  : 		m_findState.pFindReplaceDlg->DestroyWindow();

	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+92]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+92]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	call	DWORD PTR [edx+96]
	cmp	esi, esp
	call	__chkesp

; 136  : 		m_findState.pFindReplaceDlg = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+92], 0
$L75001:

; 138  : 
; 139  : 	CRichEditCtrl::OnDestroy();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnDestroy@CWnd@@IAEXXZ			; CWnd::OnDestroy

; 140  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?OnDestroy@CRichEditBaseCtrl@@IAEXXZ ENDP		; CRichEditBaseCtrl::OnDestroy
_TEXT	ENDS
EXTRN	?PreSubclassWindow@CWnd@@UAEXXZ:NEAR		; CWnd::PreSubclassWindow
;	COMDAT ?PreSubclassWindow@CRichEditBaseCtrl@@MAEXXZ
_TEXT	SEGMENT
_this$ = -4
?PreSubclassWindow@CRichEditBaseCtrl@@MAEXXZ PROC NEAR	; CRichEditBaseCtrl::PreSubclassWindow, COMDAT

; 143  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 144  : 	SetOLECallback(&m_callback);

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetOLECallback@CRichEditCtrl@@QAEHPAUIRichEditOleCallback@@@Z ; CRichEditCtrl::SetOLECallback

; 145  : 	
; 146  : 	CRichEditCtrl::PreSubclassWindow();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?PreSubclassWindow@CWnd@@UAEXXZ		; CWnd::PreSubclassWindow

; 147  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?PreSubclassWindow@CRichEditBaseCtrl@@MAEXXZ ENDP	; CRichEditBaseCtrl::PreSubclassWindow
_TEXT	ENDS
PUBLIC	?Undo@CRichEditBaseCtrl@@QAEHXZ			; CRichEditBaseCtrl::Undo
EXTRN	??0CTextDocument@@QAE@PAUHWND__@@@Z:NEAR	; CTextDocument::CTextDocument
EXTRN	??1CTextDocument@@UAE@XZ:NEAR			; CTextDocument::~CTextDocument
EXTRN	?Undo@CTextDocument@@QAEHXZ:NEAR		; CTextDocument::Undo
EXTRN	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ:NEAR	; CWnd::GetSafeHwnd
;	COMDAT xdata$x
; File D:\_CODE\Shared\richeditbasectrl.cpp
xdata$x	SEGMENT
$T76062	DD	019930520H
	DD	01H
	DD	FLAT:$T76064
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76064	DD	0ffffffffH
	DD	FLAT:$L76060
xdata$x	ENDS
;	COMDAT ?Undo@CRichEditBaseCtrl@@QAEHXZ
_TEXT	SEGMENT
_this$ = -36
$T76058 = -16
$T76059 = -32
__$EHRec$ = -12
?Undo@CRichEditBaseCtrl@@QAEHXZ PROC NEAR		; CRichEditBaseCtrl::Undo, COMDAT

; 150  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76063
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

; 151  : 	return CTextDocument(GetSafeHwnd()).Undo();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	push	eax
	lea	ecx, DWORD PTR $T76059[ebp]
	call	??0CTextDocument@@QAE@PAUHWND__@@@Z	; CTextDocument::CTextDocument
	mov	DWORD PTR -40+[ebp], eax
	mov	eax, DWORD PTR -40+[ebp]
	mov	DWORD PTR -44+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -44+[ebp]
	call	?Undo@CTextDocument@@QAEHXZ		; CTextDocument::Undo
	mov	DWORD PTR $T76058[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76059[ebp]
	call	??1CTextDocument@@UAE@XZ		; CTextDocument::~CTextDocument
	mov	eax, DWORD PTR $T76058[ebp]

; 152  : }

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
$L76060:
	lea	ecx, DWORD PTR $T76059[ebp]
	call	??1CTextDocument@@UAE@XZ		; CTextDocument::~CTextDocument
	ret	0
$L76063:
	mov	eax, OFFSET FLAT:$T76062
	jmp	___CxxFrameHandler
text$x	ENDS
?Undo@CRichEditBaseCtrl@@QAEHXZ ENDP			; CRichEditBaseCtrl::Undo
PUBLIC	?Redo@CRichEditBaseCtrl@@QAEHXZ			; CRichEditBaseCtrl::Redo
EXTRN	?Redo@CTextDocument@@QAEHXZ:NEAR		; CTextDocument::Redo
;	COMDAT xdata$x
; File D:\_CODE\Shared\richeditbasectrl.cpp
xdata$x	SEGMENT
$T76073	DD	019930520H
	DD	01H
	DD	FLAT:$T76075
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76075	DD	0ffffffffH
	DD	FLAT:$L76071
xdata$x	ENDS
;	COMDAT ?Redo@CRichEditBaseCtrl@@QAEHXZ
_TEXT	SEGMENT
_this$ = -36
$T76069 = -16
$T76070 = -32
__$EHRec$ = -12
?Redo@CRichEditBaseCtrl@@QAEHXZ PROC NEAR		; CRichEditBaseCtrl::Redo, COMDAT

; 155  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76074
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

; 156  : 	return CTextDocument(GetSafeHwnd()).Redo();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	push	eax
	lea	ecx, DWORD PTR $T76070[ebp]
	call	??0CTextDocument@@QAE@PAUHWND__@@@Z	; CTextDocument::CTextDocument
	mov	DWORD PTR -40+[ebp], eax
	mov	eax, DWORD PTR -40+[ebp]
	mov	DWORD PTR -44+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -44+[ebp]
	call	?Redo@CTextDocument@@QAEHXZ		; CTextDocument::Redo
	mov	DWORD PTR $T76069[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76070[ebp]
	call	??1CTextDocument@@UAE@XZ		; CTextDocument::~CTextDocument
	mov	eax, DWORD PTR $T76069[ebp]

; 157  : }

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
$L76071:
	lea	ecx, DWORD PTR $T76070[ebp]
	call	??1CTextDocument@@UAE@XZ		; CTextDocument::~CTextDocument
	ret	0
$L76074:
	mov	eax, OFFSET FLAT:$T76073
	jmp	___CxxFrameHandler
text$x	ENDS
?Redo@CRichEditBaseCtrl@@QAEHXZ ENDP			; CRichEditBaseCtrl::Redo
PUBLIC	?SetTextEx@CRichEditBaseCtrl@@QAEHABVCString@@KI@Z ; CRichEditBaseCtrl::SetTextEx
EXTRN	?SendMessageW@CWnd@@QAEJIIJ@Z:NEAR		; CWnd::SendMessageW
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
EXTRN	?WideToMultiByte@Misc@@YAPADPBGAAHI@Z:NEAR	; Misc::WideToMultiByte
;	COMDAT ?SetTextEx@CRichEditBaseCtrl@@QAEHABVCString@@KI@Z
_TEXT	SEGMENT
_sText$ = 8
_dwFlags$ = 12
_nCodePage$ = 16
_this$ = -32
_szText$ = -8
_nTextLen$ = -20
_bWantUnicode$ = -4
_stex$ = -16
_bResult$ = -24
$T76080 = -28
?SetTextEx@CRichEditBaseCtrl@@QAEHABVCString@@KI@Z PROC NEAR ; CRichEditBaseCtrl::SetTextEx, COMDAT

; 160  : {

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

; 161  : 	LPTSTR szText = NULL;

	mov	DWORD PTR _szText$[ebp], 0

; 162  : 	int nTextLen = sText.GetLength();

	mov	ecx, DWORD PTR _sText$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR _nTextLen$[ebp], eax

; 163  : 	BOOL bWantUnicode = (nCodePage == CP_UNICODE);

	xor	eax, eax
	cmp	DWORD PTR _nCodePage$[ebp], 1200	; 000004b0H
	sete	al
	mov	DWORD PTR _bWantUnicode$[ebp], eax

; 164  : 
; 165  : #ifdef _UNICODE
; 166  : 	if (bWantUnicode)

	cmp	DWORD PTR _bWantUnicode$[ebp], 0
	je	SHORT $L75024

; 167  : 		szText = (LPWSTR)(LPCTSTR)sText;

	mov	ecx, DWORD PTR _sText$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	DWORD PTR _szText$[ebp], eax

; 168  : 
; 169  : 	else // ansi

	jmp	SHORT $L75027
$L75024:

; 170  : 		szText = (LPWSTR)Misc::WideToMultiByte(sText, nTextLen, nCodePage);

	mov	ecx, DWORD PTR _nCodePage$[ebp]
	push	ecx
	lea	edx, DWORD PTR _nTextLen$[ebp]
	push	edx
	mov	ecx, DWORD PTR _sText$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?WideToMultiByte@Misc@@YAPADPBGAAHI@Z	; Misc::WideToMultiByte
	add	esp, 12					; 0000000cH
	mov	DWORD PTR _szText$[ebp], eax
$L75027:

; 171  : #else
; 172  : 	if (bWantUnicode)
; 173  : 		szText = (LPSTR)Misc::MultiByteToWide(sText, nTextLen, nCodePage);
; 174  : 
; 175  : 	else // ansi
; 176  : 		szText = (LPSTR)(LPCSTR)sText;
; 177  : #endif
; 178  : 
; 179  : 	SETTEXTEX stex = { dwFlags, nCodePage };

	mov	eax, DWORD PTR _dwFlags$[ebp]
	mov	DWORD PTR _stex$[ebp], eax
	mov	ecx, DWORD PTR _nCodePage$[ebp]
	mov	DWORD PTR _stex$[ebp+4], ecx

; 180  : 	BOOL bResult = SendMessage(EM_SETTEXTEX, (WPARAM)&stex, (LPARAM)szText);

	mov	edx, DWORD PTR _szText$[ebp]
	push	edx
	lea	eax, DWORD PTR _stex$[ebp]
	push	eax
	push	1121					; 00000461H
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SendMessageW@CWnd@@QAEJIIJ@Z		; CWnd::SendMessageW
	mov	DWORD PTR _bResult$[ebp], eax

; 181  : 
; 182  : 	// cleanup
; 183  : 	if (szText != (LPCTSTR)sText)

	mov	ecx, DWORD PTR _sText$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	cmp	DWORD PTR _szText$[ebp], eax
	je	SHORT $L75034

; 184  : 		delete [] szText;

	mov	ecx, DWORD PTR _szText$[ebp]
	mov	DWORD PTR $T76080[ebp], ecx
	mov	edx, DWORD PTR $T76080[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75034:

; 185  : 
; 186  : 	return bResult;

	mov	eax, DWORD PTR _bResult$[ebp]

; 187  : }

	add	esp, 32					; 00000020H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?SetTextEx@CRichEditBaseCtrl@@QAEHABVCString@@KI@Z ENDP	; CRichEditBaseCtrl::SetTextEx
_TEXT	ENDS
PUBLIC	?GetTextRange@CRichEditBaseCtrl@@QAE?AVCString@@ABU_charrange@@@Z ; CRichEditBaseCtrl::GetTextRange
PUBLIC	?GetSelText@CRichEditBaseCtrl@@QAE?AVCString@@XZ ; CRichEditBaseCtrl::GetSelText
EXTRN	?GetSel@CRichEditCtrl@@QBEXAAU_charrange@@@Z:NEAR ; CRichEditCtrl::GetSel
;	COMDAT ?GetSelText@CRichEditBaseCtrl@@QAE?AVCString@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_this$ = -16
_cr$ = -8
$T76084 = -12
?GetSelText@CRichEditBaseCtrl@@QAE?AVCString@@XZ PROC NEAR ; CRichEditBaseCtrl::GetSelText, COMDAT

; 190  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T76084[ebp], 0

; 191  : 	CHARRANGE cr;
; 192  : 	GetSel(cr);

	lea	eax, DWORD PTR _cr$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSel@CRichEditCtrl@@QBEXAAU_charrange@@@Z ; CRichEditCtrl::GetSel

; 193  : 
; 194  : 	return GetTextRange(cr);

	lea	ecx, DWORD PTR _cr$[ebp]
	push	ecx
	mov	edx, DWORD PTR ___$ReturnUdt$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetTextRange@CRichEditBaseCtrl@@QAE?AVCString@@ABU_charrange@@@Z ; CRichEditBaseCtrl::GetTextRange
	mov	eax, DWORD PTR $T76084[ebp]
	or	al, 1
	mov	DWORD PTR $T76084[ebp], eax
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 195  : }

	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetSelText@CRichEditBaseCtrl@@QAE?AVCString@@XZ ENDP	; CRichEditBaseCtrl::GetSelText
_TEXT	ENDS
EXTRN	__imp__MultiByteToWideChar@24:NEAR
EXTRN	_memset:NEAR
EXTRN	??0CString@@QAE@ABV0@@Z:NEAR			; CString::CString
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	??2@YAPAXIPBDH@Z:NEAR				; operator new
EXTRN	?IsClass@CWinClasses@@SAHPAUHWND__@@PBG@Z:NEAR	; CWinClasses::IsClass
EXTRN	??BCWnd@@QBEPAUHWND__@@XZ:NEAR			; CWnd::operator HWND__ *
;	COMDAT data
; File D:\_CODE\Shared\richeditbasectrl.cpp
data	SEGMENT
$SG75053 DB	'R', 00H, 'i', 00H, 'c', 00H, 'h', 00H, 'E', 00H, 'd', 00H
	DB	'i', 00H, 't', 00H, '5', 00H, '0', 00H, 'W', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76097	DD	019930520H
	DD	02H
	DD	FLAT:$T76099
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76099	DD	0ffffffffH
	DD	FLAT:$L76093
	DD	00H
	DD	FLAT:$L76092
xdata$x	ENDS
;	COMDAT ?GetTextRange@CRichEditBaseCtrl@@QAE?AVCString@@ABU_charrange@@@Z
_TEXT	SEGMENT
_cr$ = 12
___$ReturnUdt$ = 8
_this$ = -76
_nLength$ = -16
_szChar$ = -20
_tr$75054 = -36
_lpszChar$75057 = -52
_tr$75060 = -48
_sText$ = -24
$T76088 = -56
$T76089 = -60
$T76090 = -64
$T76091 = -68
$T76094 = -72
__$EHRec$ = -12
?GetTextRange@CRichEditBaseCtrl@@QAE?AVCString@@ABU_charrange@@@Z PROC NEAR ; CRichEditBaseCtrl::GetTextRange, COMDAT

; 198  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76098
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 64					; 00000040H
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-76]
	mov	ecx, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T76094[ebp], 0

; 199  : 	int nLength = int(cr.cpMax - cr.cpMin + 1);

	mov	eax, DWORD PTR _cr$[ebp]
	mov	ecx, DWORD PTR _cr$[ebp]
	mov	edx, DWORD PTR [eax+4]
	sub	edx, DWORD PTR [ecx]
	add	edx, 1
	mov	DWORD PTR _nLength$[ebp], edx

; 200  : 
; 201  :     // create an ANSI buffer 
; 202  :     LPTSTR szChar = new TCHAR[nLength];

	push	202					; 000000caH
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _nLength$[ebp]
	shl	eax, 1
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T76088[ebp], eax
	mov	ecx, DWORD PTR $T76088[ebp]
	mov	DWORD PTR _szChar$[ebp], ecx

; 203  : 	ZeroMemory(szChar, nLength * sizeof(TCHAR));

	mov	edx, DWORD PTR _nLength$[ebp]
	shl	edx, 1
	push	edx
	push	0
	mov	eax, DWORD PTR _szChar$[ebp]
	push	eax
	call	_memset
	add	esp, 12					; 0000000cH

; 204  : 
; 205  : #ifndef _UNICODE
; 206  : 	if (CWinClasses::IsClass(*this, WC_RICHEDIT50)) // must handle unicode 
; 207  : 	{
; 208  : 		// create a Unicode (Wide Character) buffer of the same length
; 209  : 		LPWSTR lpszWChar = new WCHAR[nLength];
; 210  : 
; 211  : 		TEXTRANGEW tr;
; 212  : 		tr.chrg = cr;
; 213  : 		tr.lpstrText = lpszWChar;
; 214  : 		SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr);
; 215  : 
; 216  : 		// Convert the Unicode text to ANSI.
; 217  : 		WideCharToMultiByte(CP_ACP, 0, lpszWChar, -1, szChar, nLength, NULL, NULL);
; 218  : 
; 219  : 		delete lpszWChar;
; 220  : 	}
; 221  : 	else 
; 222  : 	{
; 223  : 		TEXTRANGE tr;
; 224  : 		tr.chrg = cr;
; 225  : 		tr.lpstrText = szChar;
; 226  : 		SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr);
; 227  : 	}
; 228  : #else
; 229  : 	if (CWinClasses::IsClass(*this, WC_RICHEDIT50)) 

	push	OFFSET FLAT:$SG75053
	mov	ecx, DWORD PTR _this$[ebp]
	call	??BCWnd@@QBEPAUHWND__@@XZ		; CWnd::operator HWND__ *
	push	eax
	call	?IsClass@CWinClasses@@SAHPAUHWND__@@PBG@Z ; CWinClasses::IsClass
	add	esp, 8
	test	eax, eax
	je	SHORT $L75052

; 231  : 		TEXTRANGE tr;
; 232  : 		tr.chrg = cr;

	mov	ecx, DWORD PTR _cr$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	eax, DWORD PTR [ecx+4]
	mov	DWORD PTR _tr$75054[ebp], edx
	mov	DWORD PTR _tr$75054[ebp+4], eax

; 233  : 		tr.lpstrText = szChar;

	mov	ecx, DWORD PTR _szChar$[ebp]
	mov	DWORD PTR _tr$75054[ebp+8], ecx

; 234  : 		SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr);

	lea	edx, DWORD PTR _tr$75054[ebp]
	push	edx
	push	0
	push	1099					; 0000044bH
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SendMessageW@CWnd@@QAEJIIJ@Z		; CWnd::SendMessageW

; 236  : 	else // must handle ansi

	jmp	SHORT $L75056
$L75052:

; 238  : 		// create a Ansi buffer of the same length
; 239  : 		LPSTR lpszChar = new char[nLength];

	push	239					; 000000efH
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _nLength$[ebp]
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T76089[ebp], eax
	mov	ecx, DWORD PTR $T76089[ebp]
	mov	DWORD PTR _lpszChar$75057[ebp], ecx

; 240  : 
; 241  : 		TEXTRANGEA tr;
; 242  : 		tr.chrg = cr;

	mov	edx, DWORD PTR _cr$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR [edx+4]
	mov	DWORD PTR _tr$75060[ebp], eax
	mov	DWORD PTR _tr$75060[ebp+4], ecx

; 243  : 		tr.lpstrText = lpszChar;

	mov	edx, DWORD PTR _lpszChar$75057[ebp]
	mov	DWORD PTR _tr$75060[ebp+8], edx

; 244  : 		SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr);

	lea	eax, DWORD PTR _tr$75060[ebp]
	push	eax
	push	0
	push	1099					; 0000044bH
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SendMessageW@CWnd@@QAEJIIJ@Z		; CWnd::SendMessageW

; 245  : 
; 246  : 		// Convert the Ansi text to Unicode.
; 247  : 		MultiByteToWideChar(CP_ACP, 0, lpszChar, -1, szChar, nLength);

	mov	esi, esp
	mov	ecx, DWORD PTR _nLength$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szChar$[ebp]
	push	edx
	push	-1
	mov	eax, DWORD PTR _lpszChar$75057[ebp]
	push	eax
	push	0
	push	0
	call	DWORD PTR __imp__MultiByteToWideChar@24
	cmp	esi, esp
	call	__chkesp

; 248  : 
; 249  : 		delete lpszChar;

	mov	ecx, DWORD PTR _lpszChar$75057[ebp]
	mov	DWORD PTR $T76090[ebp], ecx
	mov	edx, DWORD PTR $T76090[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75056:

; 251  : #endif
; 252  : 
; 253  : 	CString sText(szChar);

	mov	eax, DWORD PTR _szChar$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 254  : 	delete [] szChar;

	mov	ecx, DWORD PTR _szChar$[ebp]
	mov	DWORD PTR $T76091[ebp], ecx
	mov	edx, DWORD PTR $T76091[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 255  : 
; 256  : 	return sText;

	lea	eax, DWORD PTR _sText$[ebp]
	push	eax
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	ecx, DWORD PTR $T76094[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76094[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 257  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 76					; 0000004cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76092:
	lea	ecx, DWORD PTR _sText$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76093:
	mov	eax, DWORD PTR $T76094[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76095
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76095:
	ret	0
$L76098:
	mov	eax, OFFSET FLAT:$T76097
	jmp	___CxxFrameHandler
text$x	ENDS
?GetTextRange@CRichEditBaseCtrl@@QAE?AVCString@@ABU_charrange@@@Z ENDP ; CRichEditBaseCtrl::GetTextRange
PUBLIC	?SelectCurrentWord@CRichEditBaseCtrl@@QAEXXZ	; CRichEditBaseCtrl::SelectCurrentWord
EXTRN	?SetSel@CRichEditCtrl@@QAEXAAU_charrange@@@Z:NEAR ; CRichEditCtrl::SetSel
;	COMDAT ?SelectCurrentWord@CRichEditBaseCtrl@@QAEXXZ
_TEXT	SEGMENT
_this$ = -12
_cr$ = -8
?SelectCurrentWord@CRichEditBaseCtrl@@QAEXXZ PROC NEAR	; CRichEditBaseCtrl::SelectCurrentWord, COMDAT

; 260  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 261  : 	CHARRANGE cr;
; 262  : 	GetSel(cr);

	lea	eax, DWORD PTR _cr$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSel@CRichEditCtrl@@QBEXAAU_charrange@@@Z ; CRichEditCtrl::GetSel

; 263  : 
; 264  : 	if (cr.cpMin == cr.cpMax) // nothing already selected

	mov	ecx, DWORD PTR _cr$[ebp]
	cmp	ecx, DWORD PTR _cr$[ebp+4]
	jne	SHORT $L75069

; 266  : 		cr.cpMin = SendMessage(EM_FINDWORDBREAK, WB_LEFT, cr.cpMin);

	mov	edx, DWORD PTR _cr$[ebp]
	push	edx
	push	0
	push	1100					; 0000044cH
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SendMessageW@CWnd@@QAEJIIJ@Z		; CWnd::SendMessageW
	mov	DWORD PTR _cr$[ebp], eax

; 267  : 		cr.cpMax = SendMessage(EM_FINDWORDBREAK, WB_RIGHTBREAK, cr.cpMax + 1);

	mov	eax, DWORD PTR _cr$[ebp+4]
	add	eax, 1
	push	eax
	push	7
	push	1100					; 0000044cH
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SendMessageW@CWnd@@QAEJIIJ@Z		; CWnd::SendMessageW
	mov	DWORD PTR _cr$[ebp+4], eax

; 268  : 
; 269  : 		SetSel(cr);

	lea	ecx, DWORD PTR _cr$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetSel@CRichEditCtrl@@QAEXAAU_charrange@@@Z ; CRichEditCtrl::SetSel
$L75069:

; 271  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?SelectCurrentWord@CRichEditBaseCtrl@@QAEXXZ ENDP	; CRichEditBaseCtrl::SelectCurrentWord
_TEXT	ENDS
PUBLIC	?InsertSoftReturn@CRichEditBaseCtrl@@IAEHXZ	; CRichEditBaseCtrl::InsertSoftReturn
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
;	COMDAT xdata$x
; File D:\_CODE\Shared\richeditbasectrl.cpp
xdata$x	SEGMENT
$T76111	DD	019930520H
	DD	01H
	DD	FLAT:$T76113
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76113	DD	0ffffffffH
	DD	FLAT:$L76109
xdata$x	ENDS
;	COMDAT ?InsertSoftReturn@CRichEditBaseCtrl@@IAEHXZ
_TEXT	SEGMENT
_this$ = -24
$T76107 = -16
$T76108 = -20
__$EHRec$ = -12
?InsertSoftReturn@CRichEditBaseCtrl@@IAEHXZ PROC NEAR	; CRichEditBaseCtrl::InsertSoftReturn, COMDAT

; 274  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76112
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
$L75073:

; 275  : 	ASSERT(GetSafeHwnd());

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	test	eax, eax
	jne	SHORT $L75076
	push	275					; 00000113H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75076
	int	3
$L75076:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75073

; 276  : 
; 277  : 	if (!GetSafeHwnd())

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	test	eax, eax
	jne	SHORT $L75077

; 278  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75072
$L75077:

; 279  : 
; 280  : 	return SetTextEx(RTF_SOFT_RETURN);

	mov	ecx, DWORD PTR _RTF_SOFT_RETURN
	push	ecx
	lea	ecx, DWORD PTR $T76108[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	push	0
	push	3
	lea	edx, DWORD PTR $T76108[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetTextEx@CRichEditBaseCtrl@@QAEHABVCString@@KI@Z ; CRichEditBaseCtrl::SetTextEx
	mov	DWORD PTR $T76107[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76108[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76107[ebp]
$L75072:

; 281  : }

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
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76109:
	lea	ecx, DWORD PTR $T76108[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76112:
	mov	eax, OFFSET FLAT:$T76111
	jmp	___CxxFrameHandler
text$x	ENDS
?InsertSoftReturn@CRichEditBaseCtrl@@IAEHXZ ENDP	; CRichEditBaseCtrl::InsertSoftReturn
PUBLIC	?InsertTable@CRichEditBaseCtrl@@QAEHHHHHK@Z	; CRichEditBaseCtrl::InsertTable
PUBLIC	?InsertHorizontalLine@CRichEditBaseCtrl@@QAEHH@Z ; CRichEditBaseCtrl::InsertHorizontalLine
;	COMDAT ?InsertHorizontalLine@CRichEditBaseCtrl@@QAEHH@Z
_TEXT	SEGMENT
_nWidth$ = 8
_this$ = -4
?InsertHorizontalLine@CRichEditBaseCtrl@@QAEHH@Z PROC NEAR ; CRichEditBaseCtrl::InsertHorizontalLine, COMDAT

; 284  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L75084:

; 285  : 	ASSERT(GetSafeHwnd());

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	test	eax, eax
	jne	SHORT $L75087
	push	285					; 0000011dH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75087
	int	3
$L75087:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75084

; 286  : 
; 287  : 	if (!GetSafeHwnd() || nWidth == 0 || nWidth < -1)

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	test	eax, eax
	je	SHORT $L75089
	cmp	DWORD PTR _nWidth$[ebp], 0
	je	SHORT $L75089
	cmp	DWORD PTR _nWidth$[ebp], -1
	jge	SHORT $L75088
$L75089:

; 288  : 		return FALSE;

	xor	eax, eax
	jmp	SHORT $L75083
$L75088:

; 289  : 
; 290  : 	if (nWidth == -1) // full page width

	cmp	DWORD PTR _nWidth$[ebp], -1
	jne	SHORT $L75090

; 291  : 		nWidth = (int)(8.3 * 1440); // in twips

	mov	DWORD PTR _nWidth$[ebp], 11952		; 00002eb0H
$L75090:

; 292  : 
; 293  : 	// paste 
; 294  : 	return InsertTable(1, 1, nWidth, 0, REBCB_BOTTOM);

	push	4
	push	0
	mov	ecx, DWORD PTR _nWidth$[ebp]
	push	ecx
	push	1
	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?InsertTable@CRichEditBaseCtrl@@QAEHHHHHK@Z ; CRichEditBaseCtrl::InsertTable
$L75083:

; 295  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?InsertHorizontalLine@CRichEditBaseCtrl@@QAEHH@Z ENDP	; CRichEditBaseCtrl::InsertHorizontalLine
_TEXT	ENDS
PUBLIC	?HasFlag@Misc@@YAHKK@Z				; Misc::HasFlag
EXTRN	??0CString@@QAE@PBD@Z:NEAR			; CString::CString
EXTRN	??YCString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator+=
EXTRN	??YCString@@QAEABV0@PBG@Z:NEAR			; CString::operator+=
EXTRN	?Format@CString@@QAAXPBGZZ:NEAR			; CString::Format
;	COMDAT data
; File D:\_CODE\Shared\richeditbasectrl.cpp
data	SEGMENT
$SG75124 DB	'{', 00H
	ORG $+2
$SG75130 DB	'}', 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76128	DD	019930520H
	DD	05H
	DD	FLAT:$T76130
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76130	DD	0ffffffffH
	DD	FLAT:$L76122
	DD	00H
	DD	FLAT:$L76123
	DD	00H
	DD	FLAT:$L76124
	DD	02H
	DD	FLAT:$L76125
	DD	03H
	DD	FLAT:$L76126
xdata$x	ENDS
;	COMDAT ?InsertTable@CRichEditBaseCtrl@@QAEHHHHHK@Z
_TEXT	SEGMENT
_nRows$ = 8
_nCols$ = 12
_nColWidth$ = 16
_nTextIndent$ = 20
$T76120 = -48
_dwBorders$ = 24
$T76121 = -52
_this$ = -56
__$EHRec$ = -12
_sTable$ = -24
_sIndent$75108 = -36
_sWidth$ = -32
_nWidth$ = -28
_nCol$ = -16
_nRow$ = -20
_sRow$75123 = -44
_nCol$75125 = -40
?InsertTable@CRichEditBaseCtrl@@QAEHHHHHK@Z PROC NEAR	; CRichEditBaseCtrl::InsertTable, COMDAT

; 298  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76129
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 44					; 0000002cH
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-56]
	mov	ecx, 11					; 0000000bH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L75100:

; 299  : 	ASSERT(GetSafeHwnd());

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	test	eax, eax
	jne	SHORT $L75103
	push	299					; 0000012bH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75103
	int	3
$L75103:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75100

; 300  : 
; 301  : 	if (!GetSafeHwnd() || nRows <= 0 || nCols <= 0 || nColWidth == 0)

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	test	eax, eax
	je	SHORT $L75105
	cmp	DWORD PTR _nRows$[ebp], 0
	jle	SHORT $L75105
	cmp	DWORD PTR _nCols$[ebp], 0
	jle	SHORT $L75105
	cmp	DWORD PTR _nColWidth$[ebp], 0
	jne	SHORT $L75104
$L75105:

; 302  : 		return FALSE;

	xor	eax, eax
	jmp	$L75099
$L75104:

; 303  : 
; 304  : 	// table initialization
; 305  : 	CString sTable(RTF_TABLE_HEADER);

	mov	ecx, DWORD PTR _RTF_TABLE_HEADER
	push	ecx
	lea	ecx, DWORD PTR _sTable$[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 306  : 
; 307  : 	// text indent
; 308  : 	if (nTextIndent > 0)

	cmp	DWORD PTR _nTextIndent$[ebp], 0
	jle	SHORT $L75107

; 310  : 		CString sIndent;

	lea	ecx, DWORD PTR _sIndent$75108[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 311  : 		sIndent.Format(RTF_TEXT_INDENT, nTextIndent);

	mov	edx, DWORD PTR _nTextIndent$[ebp]
	push	edx
	mov	eax, DWORD PTR _RTF_TEXT_INDENT
	push	eax
	lea	ecx, DWORD PTR _sIndent$75108[ebp]
	push	ecx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 12					; 0000000cH

; 312  : 		sTable += sIndent;

	lea	edx, DWORD PTR _sIndent$75108[ebp]
	push	edx
	lea	ecx, DWORD PTR _sTable$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=

; 313  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sIndent$75108[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75107:

; 314  : 
; 315  : 	// column width setup
; 316  : 	CString sWidth;

	lea	ecx, DWORD PTR _sWidth$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 317  : 	int nWidth = nColWidth;

	mov	eax, DWORD PTR _nColWidth$[ebp]
	mov	DWORD PTR _nWidth$[ebp], eax

; 318  : 
; 319  : 	for (int nCol = 0; nCol < nCols; nCol++)

	mov	DWORD PTR _nCol$[ebp], 0
	jmp	SHORT $L75112
$L75113:
	mov	ecx, DWORD PTR _nCol$[ebp]
	add	ecx, 1
	mov	DWORD PTR _nCol$[ebp], ecx
$L75112:
	mov	edx, DWORD PTR _nCol$[ebp]
	cmp	edx, DWORD PTR _nCols$[ebp]
	jge	$L75114

; 321  : 		// borders
; 322  : 		if (Misc::HasFlag(dwBorders, REBCB_TOP))

	push	1
	mov	eax, DWORD PTR _dwBorders$[ebp]
	push	eax
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	test	eax, eax
	je	SHORT $L75115

; 323  : 			sTable += RTF_CELL_BORDER_TOP;

	mov	ecx, DWORD PTR _RTF_CELL_BORDER_TOP
	push	ecx
	lea	ecx, DWORD PTR _sTable$[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=
$L75115:

; 324  : 
; 325  : 		if (Misc::HasFlag(dwBorders, REBCB_LEFT))

	push	2
	mov	edx, DWORD PTR _dwBorders$[ebp]
	push	edx
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	test	eax, eax
	je	SHORT $L75116

; 326  : 			sTable += RTF_CELL_BORDER_LEFT;

	mov	eax, DWORD PTR _RTF_CELL_BORDER_LEFT
	push	eax
	lea	ecx, DWORD PTR _sTable$[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=
$L75116:

; 327  : 
; 328  : 		if (Misc::HasFlag(dwBorders, REBCB_BOTTOM))

	push	4
	mov	ecx, DWORD PTR _dwBorders$[ebp]
	push	ecx
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	test	eax, eax
	je	SHORT $L75117

; 329  : 			sTable += RTF_CELL_BORDERS_BOTTOM;

	mov	edx, DWORD PTR _RTF_CELL_BORDERS_BOTTOM
	push	edx
	lea	ecx, DWORD PTR _sTable$[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=
$L75117:

; 330  : 
; 331  : 		if (Misc::HasFlag(dwBorders, REBCB_RIGHT))

	push	8
	mov	eax, DWORD PTR _dwBorders$[ebp]
	push	eax
	call	?HasFlag@Misc@@YAHKK@Z			; Misc::HasFlag
	add	esp, 8
	test	eax, eax
	je	SHORT $L75118

; 332  : 			sTable += RTF_CELL_BORDERS_RIGHT;

	mov	ecx, DWORD PTR _RTF_CELL_BORDERS_RIGHT
	push	ecx
	lea	ecx, DWORD PTR _sTable$[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=
$L75118:

; 333  : 
; 334  : 		// column widths
; 335  : 		sWidth.Format(RTF_COLUMN_WIDTH, nWidth);

	mov	edx, DWORD PTR _nWidth$[ebp]
	push	edx
	mov	eax, DWORD PTR _RTF_COLUMN_WIDTH
	push	eax
	lea	ecx, DWORD PTR _sWidth$[ebp]
	push	ecx
	call	?Format@CString@@QAAXPBGZZ		; CString::Format
	add	esp, 12					; 0000000cH

; 336  : 		sTable += sWidth;

	lea	edx, DWORD PTR _sWidth$[ebp]
	push	edx
	lea	ecx, DWORD PTR _sTable$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=

; 337  : 		nWidth += nColWidth;

	mov	eax, DWORD PTR _nWidth$[ebp]
	add	eax, DWORD PTR _nColWidth$[ebp]
	mov	DWORD PTR _nWidth$[ebp], eax

; 338  : 	}

	jmp	$L75113
$L75114:

; 339  : 
; 340  : 	sTable += RTF_TABLE_START;

	mov	ecx, DWORD PTR _RTF_TABLE_START
	push	ecx
	lea	ecx, DWORD PTR _sTable$[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=

; 341  : 
; 342  : 	// rows
; 343  : 	for (int nRow = 0; nRow < nRows; nRow++)

	mov	DWORD PTR _nRow$[ebp], 0
	jmp	SHORT $L75120
$L75121:
	mov	edx, DWORD PTR _nRow$[ebp]
	add	edx, 1
	mov	DWORD PTR _nRow$[ebp], edx
$L75120:
	mov	eax, DWORD PTR _nRow$[ebp]
	cmp	eax, DWORD PTR _nRows$[ebp]
	jge	$L75122

; 345  : 		CString sRow("{");

	push	OFFSET FLAT:$SG75124
	lea	ecx, DWORD PTR _sRow$75123[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 3

; 346  : 
; 347  : 		for (int nCol = 0; nCol < nCols; nCol++)

	mov	DWORD PTR _nCol$75125[ebp], 0
	jmp	SHORT $L75126
$L75127:
	mov	ecx, DWORD PTR _nCol$75125[ebp]
	add	ecx, 1
	mov	DWORD PTR _nCol$75125[ebp], ecx
$L75126:
	mov	edx, DWORD PTR _nCol$75125[ebp]
	cmp	edx, DWORD PTR _nCols$[ebp]
	jge	SHORT $L75128

; 348  : 			sRow += RTF_COLUMN_CELL;

	mov	eax, DWORD PTR _RTF_COLUMN_CELL
	push	eax
	lea	ecx, DWORD PTR _sRow$75123[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=
	jmp	SHORT $L75127
$L75128:

; 349  : 
; 350  : 		sRow += "}";

	push	OFFSET FLAT:$SG75130
	lea	ecx, DWORD PTR $T76120[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	lea	ecx, DWORD PTR $T76120[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sRow$75123[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR $T76120[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 351  : 		sRow += RTF_ROW;

	mov	edx, DWORD PTR _RTF_ROW
	push	edx
	lea	ecx, DWORD PTR _sRow$75123[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=

; 352  : 
; 353  : 		sTable += sRow;

	lea	eax, DWORD PTR _sRow$75123[ebp]
	push	eax
	lea	ecx, DWORD PTR _sTable$[ebp]
	call	??YCString@@QAEABV0@ABV0@@Z		; CString::operator+=

; 354  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 2
	lea	ecx, DWORD PTR _sRow$75123[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	$L75121
$L75122:

; 355  : 
; 356  : 	// footer
; 357  : 	sTable += RTF_TABLE_FOOTER;

	mov	ecx, DWORD PTR _RTF_TABLE_FOOTER
	push	ecx
	lea	ecx, DWORD PTR _sTable$[ebp]
	call	??YCString@@QAEABV0@PBG@Z		; CString::operator+=

; 358  : 
; 359  : 	// paste table
; 360  : 	return SetTextEx(sTable);

	push	0
	push	3
	lea	edx, DWORD PTR _sTable$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetTextEx@CRichEditBaseCtrl@@QAEHABVCString@@KI@Z ; CRichEditBaseCtrl::SetTextEx
	mov	DWORD PTR $T76121[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _sWidth$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sTable$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76121[ebp]
$L75099:

; 361  : }

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
	ret	20					; 00000014H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76122:
	lea	ecx, DWORD PTR _sTable$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76123:
	lea	ecx, DWORD PTR _sIndent$75108[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76124:
	lea	ecx, DWORD PTR _sWidth$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76125:
	lea	ecx, DWORD PTR _sRow$75123[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76126:
	lea	ecx, DWORD PTR $T76120[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76129:
	mov	eax, OFFSET FLAT:$T76128
	jmp	___CxxFrameHandler
text$x	ENDS
?InsertTable@CRichEditBaseCtrl@@QAEHHHHHK@Z ENDP	; CRichEditBaseCtrl::InsertTable
;	COMDAT ?HasFlag@Misc@@YAHKK@Z
_TEXT	SEGMENT
_dwFlags$ = 8
_dwFlag$ = 12
?HasFlag@Misc@@YAHKK@Z PROC NEAR			; Misc::HasFlag, COMDAT

; 215  : 	inline BOOL HasFlag(DWORD dwFlags, DWORD dwFlag) { return (((dwFlags & dwFlag) == dwFlag) ? TRUE : FALSE); }

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR _dwFlags$[ebp]
	and	eax, DWORD PTR _dwFlag$[ebp]
	xor	ecx, ecx
	cmp	eax, DWORD PTR _dwFlag$[ebp]
	sete	cl
	mov	eax, ecx
	pop	ebp
	ret	0
?HasFlag@Misc@@YAHKK@Z ENDP				; Misc::HasFlag
_TEXT	ENDS
PUBLIC	?GetNewStorage@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAPAUIStorage@@@Z ; CRichEditBaseCtrl::CRichEditOleCallback::GetNewStorage
PUBLIC	?QueryInterface@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJABU_GUID@@PAPAX@Z ; CRichEditBaseCtrl::CRichEditOleCallback::QueryInterface
PUBLIC	?AddRef@CRichEditOleCallback@CRichEditBaseCtrl@@UAGKXZ ; CRichEditBaseCtrl::CRichEditOleCallback::AddRef
PUBLIC	?Release@CRichEditOleCallback@CRichEditBaseCtrl@@UAGKXZ ; CRichEditBaseCtrl::CRichEditOleCallback::Release
PUBLIC	?GetInPlaceContext@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAPAUIOleInPlaceFrame@@PAPAUIOleInPlaceUIWindow@@PAUtagOIFI@@@Z ; CRichEditBaseCtrl::CRichEditOleCallback::GetInPlaceContext
PUBLIC	?ShowContainerUI@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJH@Z ; CRichEditBaseCtrl::CRichEditOleCallback::ShowContainerUI
PUBLIC	?QueryInsertObject@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAU_GUID@@PAUIStorage@@J@Z ; CRichEditBaseCtrl::CRichEditOleCallback::QueryInsertObject
PUBLIC	?DeleteObject@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAUIOleObject@@@Z ; CRichEditBaseCtrl::CRichEditOleCallback::DeleteObject
PUBLIC	?QueryAcceptData@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAUIDataObject@@PAGKHPAX@Z ; CRichEditBaseCtrl::CRichEditOleCallback::QueryAcceptData
PUBLIC	?ContextSensitiveHelp@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJH@Z ; CRichEditBaseCtrl::CRichEditOleCallback::ContextSensitiveHelp
PUBLIC	?GetClipboardData@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAU_charrange@@KPAPAUIDataObject@@@Z ; CRichEditBaseCtrl::CRichEditOleCallback::GetClipboardData
PUBLIC	?GetDragDropEffect@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJHKPAK@Z ; CRichEditBaseCtrl::CRichEditOleCallback::GetDragDropEffect
PUBLIC	?GetContextMenu@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJGPAUIOleObject@@PAU_charrange@@PAPAUHMENU__@@@Z ; CRichEditBaseCtrl::CRichEditOleCallback::GetContextMenu
PUBLIC	??_7CRichEditOleCallback@CRichEditBaseCtrl@@6B@	; CRichEditBaseCtrl::CRichEditOleCallback::`vftable'
PUBLIC	??_GCRichEditOleCallback@CRichEditBaseCtrl@@UAEPAXI@Z ; CRichEditBaseCtrl::CRichEditOleCallback::`scalar deleting destructor'
PUBLIC	??_ECRichEditOleCallback@CRichEditBaseCtrl@@UAEPAXI@Z ; CRichEditBaseCtrl::CRichEditOleCallback::`vector deleting destructor'
PUBLIC	??0IRichEditOleCallback@@QAE@XZ			; IRichEditOleCallback::IRichEditOleCallback
EXTRN	?AfxThrowOleException@@YGXJ@Z:NEAR		; AfxThrowOleException
EXTRN	__imp__StgCreateDocfile@16:NEAR
;	COMDAT ??_7CRichEditOleCallback@CRichEditBaseCtrl@@6B@
; File D:\_CODE\Shared\richeditbasectrl.cpp
CONST	SEGMENT
??_7CRichEditOleCallback@CRichEditBaseCtrl@@6B@ DD FLAT:?QueryInterface@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJABU_GUID@@PAPAX@Z ; CRichEditBaseCtrl::CRichEditOleCallback::`vftable'
	DD	FLAT:?AddRef@CRichEditOleCallback@CRichEditBaseCtrl@@UAGKXZ
	DD	FLAT:?Release@CRichEditOleCallback@CRichEditBaseCtrl@@UAGKXZ
	DD	FLAT:?GetNewStorage@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAPAUIStorage@@@Z
	DD	FLAT:?GetInPlaceContext@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAPAUIOleInPlaceFrame@@PAPAUIOleInPlaceUIWindow@@PAUtagOIFI@@@Z
	DD	FLAT:?ShowContainerUI@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJH@Z
	DD	FLAT:?QueryInsertObject@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAU_GUID@@PAUIStorage@@J@Z
	DD	FLAT:?DeleteObject@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAUIOleObject@@@Z
	DD	FLAT:?QueryAcceptData@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAUIDataObject@@PAGKHPAX@Z
	DD	FLAT:?ContextSensitiveHelp@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJH@Z
	DD	FLAT:?GetClipboardData@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAU_charrange@@KPAPAUIDataObject@@@Z
	DD	FLAT:?GetDragDropEffect@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJHKPAK@Z
	DD	FLAT:?GetContextMenu@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJGPAUIOleObject@@PAU_charrange@@PAPAUHMENU__@@@Z
	DD	FLAT:??_ECRichEditOleCallback@CRichEditBaseCtrl@@UAEPAXI@Z
CONST	ENDS
;	COMDAT ??0CRichEditOleCallback@CRichEditBaseCtrl@@QAE@XZ
_TEXT	SEGMENT
_this$ = -8
_hResult$ = -4
??0CRichEditOleCallback@CRichEditBaseCtrl@@QAE@XZ PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::CRichEditOleCallback, COMDAT

; 364  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0IRichEditOleCallback@@QAE@XZ		; IRichEditOleCallback::IRichEditOleCallback
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+16], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx], OFFSET FLAT:??_7CRichEditOleCallback@CRichEditBaseCtrl@@6B@ ; CRichEditBaseCtrl::CRichEditOleCallback::`vftable'

; 365  : 	m_pStorage = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+8], 0

; 366  : 	m_iNumStorages = 0;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+4], 0

; 367  : 	m_dwRef = 0;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+12], 0

; 368  : 
; 369  : 	// set up OLE storage
; 370  : 	HRESULT hResult = ::StgCreateDocfile(NULL, STGM_TRANSACTED | 
; 371  : 		STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_DELETEONRELEASE,
; 372  : 		0, &m_pStorage );

	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 8
	mov	esi, esp
	push	edx
	push	0
	push	67178514				; 04011012H
	push	0
	call	DWORD PTR __imp__StgCreateDocfile@16
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hResult$[ebp], eax

; 375  : 		hResult != S_OK )

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+8], 0
	je	SHORT $L75138
	cmp	DWORD PTR _hResult$[ebp], 0
	je	SHORT $L75137
$L75138:

; 377  : 		AfxThrowOleException( hResult );

	mov	ecx, DWORD PTR _hResult$[ebp]
	push	ecx
	call	?AfxThrowOleException@@YGXJ@Z		; AfxThrowOleException
$L75137:

; 379  : }

	mov	eax, DWORD PTR _this$[ebp]
	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CRichEditOleCallback@CRichEditBaseCtrl@@QAE@XZ ENDP	; CRichEditBaseCtrl::CRichEditOleCallback::CRichEditOleCallback
_TEXT	ENDS
;	COMDAT ??_GCRichEditOleCallback@CRichEditBaseCtrl@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCRichEditOleCallback@CRichEditBaseCtrl@@UAEPAXI@Z PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CRichEditOleCallback@CRichEditBaseCtrl@@UAE@XZ ; CRichEditBaseCtrl::CRichEditOleCallback::~CRichEditOleCallback
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L75142
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L75142:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCRichEditOleCallback@CRichEditBaseCtrl@@UAEPAXI@Z ENDP ; CRichEditBaseCtrl::CRichEditOleCallback::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??0IUnknown@@QAE@XZ				; IUnknown::IUnknown
PUBLIC	??_7IRichEditOleCallback@@6B@			; IRichEditOleCallback::`vftable'
EXTRN	__purecall:NEAR
;	COMDAT ??_7IRichEditOleCallback@@6B@
CONST	SEGMENT
??_7IRichEditOleCallback@@6B@ DD FLAT:__purecall	; IRichEditOleCallback::`vftable'
	DD	FLAT:__purecall
	DD	FLAT:__purecall
	DD	FLAT:__purecall
	DD	FLAT:__purecall
	DD	FLAT:__purecall
	DD	FLAT:__purecall
	DD	FLAT:__purecall
	DD	FLAT:__purecall
	DD	FLAT:__purecall
	DD	FLAT:__purecall
	DD	FLAT:__purecall
	DD	FLAT:__purecall
CONST	ENDS
;	COMDAT ??0IRichEditOleCallback@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0IRichEditOleCallback@@QAE@XZ PROC NEAR		; IRichEditOleCallback::IRichEditOleCallback, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0IUnknown@@QAE@XZ			; IUnknown::IUnknown
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7IRichEditOleCallback@@6B@ ; IRichEditOleCallback::`vftable'
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0IRichEditOleCallback@@QAE@XZ ENDP			; IRichEditOleCallback::IRichEditOleCallback
_TEXT	ENDS
;	COMDAT ??0IUnknown@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0IUnknown@@QAE@XZ PROC NEAR				; IUnknown::IUnknown, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	0
??0IUnknown@@QAE@XZ ENDP				; IUnknown::IUnknown
_TEXT	ENDS
;	COMDAT ??1CRichEditOleCallback@CRichEditBaseCtrl@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CRichEditOleCallback@CRichEditBaseCtrl@@UAE@XZ PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::~CRichEditOleCallback, COMDAT

; 382  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CRichEditOleCallback@CRichEditBaseCtrl@@6B@ ; CRichEditBaseCtrl::CRichEditOleCallback::`vftable'

; 383  : 	if (m_pStorage)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+8], 0
	je	SHORT $L75150

; 385  :       m_pStorage->Release();

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	ecx, DWORD PTR [edx]
	mov	esi, esp
	push	eax
	call	DWORD PTR [ecx+8]
	cmp	esi, esp
	call	__chkesp

; 386  :       m_pStorage = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+8], 0
$L75150:

; 388  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CRichEditOleCallback@CRichEditBaseCtrl@@UAE@XZ ENDP	; CRichEditBaseCtrl::CRichEditOleCallback::~CRichEditOleCallback
_TEXT	ENDS
EXTRN	__imp__swprintf:NEAR
;	COMDAT data
; File D:\_CODE\Shared\richeditbasectrl.cpp
data	SEGMENT
$SG75156 DB	'R', 00H, 'E', 00H, 'O', 00H, 'L', 00H, 'E', 00H, 'S', 00H
	DB	't', 00H, 'o', 00H, 'r', 00H, 'a', 00H, 'g', 00H, 'e', 00H, '%'
	DB	00H, 'd', 00H, 00H, 00H
data	ENDS
;	COMDAT ?GetNewStorage@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAPAUIStorage@@@Z
_TEXT	SEGMENT
_lplpstg$ = 12
_this$ = 8
_tName$ = -100
_hResult$ = -104
?GetNewStorage@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAPAUIStorage@@@Z PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::GetNewStorage, COMDAT

; 393  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 104				; 00000068H
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-104]
	mov	ecx, 26					; 0000001aH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 394  : 	m_iNumStorages++;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	add	ecx, 1
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], ecx

; 395  : 	WCHAR tName[50];
; 396  : 
; 397  : #if _MSC_VER >= 1400
; 398  : 	swprintf_s(tName, 50, L"REOLEStorage%d", m_iNumStorages);
; 399  : #else
; 400  : 	swprintf(tName, L"REOLEStorage%d", m_iNumStorages);

	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	push	OFFSET FLAT:$SG75156
	lea	edx, DWORD PTR _tName$[ebp]
	push	edx
	call	DWORD PTR __imp__swprintf
	add	esp, 12					; 0000000cH
	cmp	esi, esp
	call	__chkesp

; 401  : #endif
; 402  : 
; 403  : 	HRESULT hResult = m_pStorage->CreateStorage(tName, 
; 404  : 		STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE ,
; 405  : 		0, 0, lplpstg );

	mov	esi, esp
	mov	eax, DWORD PTR _lplpstg$[ebp]
	push	eax
	push	0
	push	0
	push	69650					; 00011012H
	lea	ecx, DWORD PTR _tName$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	ecx, DWORD PTR [edx]
	push	eax
	call	DWORD PTR [ecx+20]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hResult$[ebp], eax

; 406  : 
; 407  : 	if (hResult != S_OK )

	cmp	DWORD PTR _hResult$[ebp], 0
	je	SHORT $L75159

; 409  : 		::AfxThrowOleException( hResult );

	mov	edx, DWORD PTR _hResult$[ebp]
	push	edx
	call	?AfxThrowOleException@@YGXJ@Z		; AfxThrowOleException
$L75159:

; 411  : 
; 412  : 	return hResult;

	mov	eax, DWORD PTR _hResult$[ebp]

; 413  : }

	pop	edi
	pop	esi
	add	esp, 104				; 00000068H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?GetNewStorage@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAPAUIStorage@@@Z ENDP ; CRichEditBaseCtrl::CRichEditOleCallback::GetNewStorage
_TEXT	ENDS
PUBLIC	??8@YAHABU_GUID@@0@Z				; operator==
EXTRN	_IID_IUnknown:BYTE
EXTRN	_IID_IRichEditOleCallback:BYTE
;	COMDAT ?QueryInterface@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJABU_GUID@@PAPAX@Z
_TEXT	SEGMENT
_iid$ = 12
_ppvObject$ = 16
_this$ = 8
_hr$ = -4
?QueryInterface@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJABU_GUID@@PAPAX@Z PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::QueryInterface, COMDAT

; 417  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 418  : 
; 419  : 	HRESULT hr = S_OK;

	mov	DWORD PTR _hr$[ebp], 0

; 420  : 	*ppvObject = NULL;

	mov	eax, DWORD PTR _ppvObject$[ebp]
	mov	DWORD PTR [eax], 0

; 423  : 		iid == IID_IRichEditOleCallback )

	push	OFFSET FLAT:_IID_IUnknown
	mov	ecx, DWORD PTR _iid$[ebp]
	push	ecx
	call	??8@YAHABU_GUID@@0@Z			; operator==
	add	esp, 8
	test	eax, eax
	jne	SHORT $L75168
	push	OFFSET FLAT:_IID_IRichEditOleCallback
	mov	edx, DWORD PTR _iid$[ebp]
	push	edx
	call	??8@YAHABU_GUID@@0@Z			; operator==
	add	esp, 8
	test	eax, eax
	je	SHORT $L75167
$L75168:

; 425  : 		*ppvObject = this;

	mov	eax, DWORD PTR _ppvObject$[ebp]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], ecx

; 426  : 		AddRef();

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	DWORD PTR [eax+4]
	cmp	esi, esp
	call	__chkesp

; 427  : 		hr = NOERROR;

	mov	DWORD PTR _hr$[ebp], 0

; 429  : 	else

	jmp	SHORT $L75169
$L75167:

; 431  : 		hr = E_NOINTERFACE;

	mov	DWORD PTR _hr$[ebp], -2147467262	; 80004002H
$L75169:

; 433  : 
; 434  : 	return hr;

	mov	eax, DWORD PTR _hr$[ebp]

; 435  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?QueryInterface@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJABU_GUID@@PAPAX@Z ENDP ; CRichEditBaseCtrl::CRichEditOleCallback::QueryInterface
_TEXT	ENDS
EXTRN	_memcmp:NEAR
;	COMDAT ??8@YAHABU_GUID@@0@Z
_TEXT	SEGMENT
_guidOne$ = 8
_guidOther$ = 12
??8@YAHABU_GUID@@0@Z PROC NEAR				; operator==, COMDAT

; 457  : {

	push	ebp
	mov	ebp, esp

; 458  : #ifdef _WIN32
; 459  :     return !memcmp(&guidOne,&guidOther,sizeof(GUID));

	push	16					; 00000010H
	mov	eax, DWORD PTR _guidOther$[ebp]
	push	eax
	mov	ecx, DWORD PTR _guidOne$[ebp]
	push	ecx
	call	_memcmp
	add	esp, 12					; 0000000cH
	neg	eax
	sbb	eax, eax
	inc	eax

; 460  : #else
; 461  :     return !_fmemcmp(&guidOne,&guidOther,sizeof(GUID)); }
; 462  : #endif
; 463  : }

	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
??8@YAHABU_GUID@@0@Z ENDP				; operator==
_TEXT	ENDS
;	COMDAT ?AddRef@CRichEditOleCallback@CRichEditBaseCtrl@@UAGKXZ
_TEXT	SEGMENT
_this$ = 8
?AddRef@CRichEditOleCallback@CRichEditBaseCtrl@@UAGKXZ PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::AddRef, COMDAT

; 439  : {

	push	ebp
	mov	ebp, esp

; 440  : 	return ++m_dwRef;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	add	ecx, 1
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+12]

; 441  : }

	pop	ebp
	ret	4
?AddRef@CRichEditOleCallback@CRichEditBaseCtrl@@UAGKXZ ENDP ; CRichEditBaseCtrl::CRichEditOleCallback::AddRef
_TEXT	ENDS
;	COMDAT ?Release@CRichEditOleCallback@CRichEditBaseCtrl@@UAGKXZ
_TEXT	SEGMENT
_this$ = 8
?Release@CRichEditOleCallback@CRichEditBaseCtrl@@UAGKXZ PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::Release, COMDAT

; 447  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 448  : 	if ( --m_dwRef == 0 )

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	sub	ecx, 1
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12], ecx
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+12], 0
	jne	SHORT $L75177

; 450  : 		if (m_pStorage)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+8], 0
	je	SHORT $L75178

; 451  :          m_pStorage->Release();

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+8]
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	ecx, DWORD PTR [edx]
	mov	esi, esp
	push	eax
	call	DWORD PTR [ecx+8]
	cmp	esi, esp
	call	__chkesp
$L75178:

; 452  : 
; 453  :       m_pStorage = NULL;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+8], 0

; 454  : 
; 455  : 		return 0;

	xor	eax, eax
	jmp	SHORT $L75176
$L75177:

; 457  : 
; 458  : 	return m_dwRef;

	mov	eax, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [eax+12]
$L75176:

; 459  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	4
?Release@CRichEditOleCallback@CRichEditBaseCtrl@@UAGKXZ ENDP ; CRichEditBaseCtrl::CRichEditOleCallback::Release
_TEXT	ENDS
;	COMDAT ?GetInPlaceContext@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAPAUIOleInPlaceFrame@@PAPAUIOleInPlaceUIWindow@@PAUtagOIFI@@@Z
_TEXT	SEGMENT
_lplpFrame$ = 12
_lplpDoc$ = 16
_lpFrameInfo$ = 20
_this$ = 8
?GetInPlaceContext@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAPAUIOleInPlaceFrame@@PAPAUIOleInPlaceUIWindow@@PAUtagOIFI@@@Z PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::GetInPlaceContext, COMDAT

; 465  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 466  :    if (m_pOwner)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	je	SHORT $L75185

; 467  :       return m_pOwner->GetInPlaceContext(lplpFrame, lplpDoc, lpFrameInfo);

	mov	esi, esp
	mov	ecx, DWORD PTR _lpFrameInfo$[ebp]
	push	ecx
	mov	edx, DWORD PTR _lplpDoc$[ebp]
	push	edx
	mov	eax, DWORD PTR _lplpFrame$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+16]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	edx, DWORD PTR [eax]
	call	DWORD PTR [edx+196]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75184
$L75185:

; 468  : 
; 469  : 	return S_OK;

	xor	eax, eax
$L75184:

; 470  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	16					; 00000010H
?GetInPlaceContext@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAPAUIOleInPlaceFrame@@PAPAUIOleInPlaceUIWindow@@PAUtagOIFI@@@Z ENDP ; CRichEditBaseCtrl::CRichEditOleCallback::GetInPlaceContext
_TEXT	ENDS
;	COMDAT ?ShowContainerUI@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJH@Z
_TEXT	SEGMENT
_fShow$ = 12
_this$ = 8
?ShowContainerUI@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJH@Z PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::ShowContainerUI, COMDAT

; 475  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 476  :    if (m_pOwner)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	je	SHORT $L75191

; 477  :       return m_pOwner->ShowContainerUI(fShow);

	mov	esi, esp
	mov	ecx, DWORD PTR _fShow$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+16]
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax+16]
	mov	eax, DWORD PTR [edx]
	call	DWORD PTR [eax+200]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75190
$L75191:

; 478  : 
; 479  : 	return S_OK;

	xor	eax, eax
$L75190:

; 480  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?ShowContainerUI@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJH@Z ENDP ; CRichEditBaseCtrl::CRichEditOleCallback::ShowContainerUI
_TEXT	ENDS
;	COMDAT ?QueryInsertObject@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAU_GUID@@PAUIStorage@@J@Z
_TEXT	SEGMENT
_lpclsid$ = 12
_lpstg$ = 16
_cp$ = 20
_this$ = 8
?QueryInsertObject@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAU_GUID@@PAUIStorage@@J@Z PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::QueryInsertObject, COMDAT

; 486  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 487  :    if (m_pOwner)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	je	SHORT $L75199

; 488  :       return m_pOwner->QueryInsertObject(lpclsid, lpstg, cp);

	mov	esi, esp
	mov	ecx, DWORD PTR _cp$[ebp]
	push	ecx
	mov	edx, DWORD PTR _lpstg$[ebp]
	push	edx
	mov	eax, DWORD PTR _lpclsid$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+16]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	edx, DWORD PTR [eax]
	call	DWORD PTR [edx+204]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75198
$L75199:

; 489  : 
; 490  : 	return S_OK;

	xor	eax, eax
$L75198:

; 491  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	16					; 00000010H
?QueryInsertObject@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAU_GUID@@PAUIStorage@@J@Z ENDP ; CRichEditBaseCtrl::CRichEditOleCallback::QueryInsertObject
_TEXT	ENDS
;	COMDAT ?DeleteObject@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAUIOleObject@@@Z
_TEXT	SEGMENT
_lpoleobj$ = 12
_this$ = 8
?DeleteObject@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAUIOleObject@@@Z PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::DeleteObject, COMDAT

; 496  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 497  :    if (m_pOwner)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	je	SHORT $L75205

; 498  :       return m_pOwner->DeleteObject(lpoleobj);

	mov	esi, esp
	mov	ecx, DWORD PTR _lpoleobj$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+16]
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax+16]
	mov	eax, DWORD PTR [edx]
	call	DWORD PTR [eax+208]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75204
$L75205:

; 499  : 
; 500  : 	return S_OK;

	xor	eax, eax
$L75204:

; 501  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?DeleteObject@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAUIOleObject@@@Z ENDP ; CRichEditBaseCtrl::CRichEditOleCallback::DeleteObject
_TEXT	ENDS
;	COMDAT ?QueryAcceptData@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAUIDataObject@@PAGKHPAX@Z
_TEXT	SEGMENT
_lpdataobj$ = 12
_lpcfFormat$ = 16
_reco$ = 20
_fReally$ = 24
_hMetaPict$ = 28
_this$ = 8
?QueryAcceptData@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAUIDataObject@@PAGKHPAX@Z PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::QueryAcceptData, COMDAT

; 508  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 509  :    if (m_pOwner)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	je	SHORT $L75215

; 510  :       return m_pOwner->QueryAcceptData(lpdataobj, lpcfFormat, reco, fReally, hMetaPict);

	mov	esi, esp
	mov	ecx, DWORD PTR _hMetaPict$[ebp]
	push	ecx
	mov	edx, DWORD PTR _fReally$[ebp]
	push	edx
	mov	eax, DWORD PTR _reco$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpcfFormat$[ebp]
	push	ecx
	mov	edx, DWORD PTR _lpdataobj$[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+16]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	edx, DWORD PTR [eax]
	call	DWORD PTR [edx+212]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75214
$L75215:

; 511  : 
; 512  : 	return S_OK;

	xor	eax, eax
$L75214:

; 513  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	24					; 00000018H
?QueryAcceptData@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAUIDataObject@@PAGKHPAX@Z ENDP ; CRichEditBaseCtrl::CRichEditOleCallback::QueryAcceptData
_TEXT	ENDS
;	COMDAT ?ContextSensitiveHelp@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJH@Z
_TEXT	SEGMENT
_fEnterMode$ = 12
_this$ = 8
?ContextSensitiveHelp@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJH@Z PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::ContextSensitiveHelp, COMDAT

; 518  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 519  :    if (m_pOwner)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	je	SHORT $L75221

; 520  :       return m_pOwner->ContextSensitiveHelp(fEnterMode);

	mov	esi, esp
	mov	ecx, DWORD PTR _fEnterMode$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+16]
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax+16]
	mov	eax, DWORD PTR [edx]
	call	DWORD PTR [eax+216]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75220
$L75221:

; 521  : 
; 522  : 	return S_OK;

	xor	eax, eax
$L75220:

; 523  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	8
?ContextSensitiveHelp@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJH@Z ENDP ; CRichEditBaseCtrl::CRichEditOleCallback::ContextSensitiveHelp
_TEXT	ENDS
;	COMDAT ?GetClipboardData@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAU_charrange@@KPAPAUIDataObject@@@Z
_TEXT	SEGMENT
_lpchrg$ = 12
_reco$ = 16
_lplpdataobj$ = 20
_this$ = 8
?GetClipboardData@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAU_charrange@@KPAPAUIDataObject@@@Z PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::GetClipboardData, COMDAT

; 529  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 530  :    if (m_pOwner)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	je	SHORT $L75229

; 531  :       return m_pOwner->GetClipboardData(lpchrg, reco, lplpdataobj);

	mov	esi, esp
	mov	ecx, DWORD PTR _lplpdataobj$[ebp]
	push	ecx
	mov	edx, DWORD PTR _reco$[ebp]
	push	edx
	mov	eax, DWORD PTR _lpchrg$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+16]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	edx, DWORD PTR [eax]
	call	DWORD PTR [edx+220]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75228
$L75229:

; 532  : 
; 533  : 	return E_NOTIMPL;

	mov	eax, -2147467263			; 80004001H
$L75228:

; 534  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	16					; 00000010H
?GetClipboardData@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJPAU_charrange@@KPAPAUIDataObject@@@Z ENDP ; CRichEditBaseCtrl::CRichEditOleCallback::GetClipboardData
_TEXT	ENDS
;	COMDAT ?GetDragDropEffect@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJHKPAK@Z
_TEXT	SEGMENT
_fDrag$ = 12
_grfKeyState$ = 16
_pdwEffect$ = 20
_this$ = 8
?GetDragDropEffect@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJHKPAK@Z PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::GetDragDropEffect, COMDAT

; 539  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 540  :    if (m_pOwner)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	je	SHORT $L75237

; 541  :       return m_pOwner->GetDragDropEffect(fDrag, grfKeyState, pdwEffect);

	mov	esi, esp
	mov	ecx, DWORD PTR _pdwEffect$[ebp]
	push	ecx
	mov	edx, DWORD PTR _grfKeyState$[ebp]
	push	edx
	mov	eax, DWORD PTR _fDrag$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+16]
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	edx, DWORD PTR [eax]
	call	DWORD PTR [edx+224]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75236
$L75237:

; 542  : 
; 543  : 	return S_OK;

	xor	eax, eax
$L75236:

; 544  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	16					; 00000010H
?GetDragDropEffect@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJHKPAK@Z ENDP ; CRichEditBaseCtrl::CRichEditOleCallback::GetDragDropEffect
_TEXT	ENDS
;	COMDAT ?GetContextMenu@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJGPAUIOleObject@@PAU_charrange@@PAPAUHMENU__@@@Z
_TEXT	SEGMENT
_seltyp$ = 12
_lpoleobj$ = 16
_lpchrg$ = 20
_lphmenu$ = 24
_this$ = 8
?GetContextMenu@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJGPAUIOleObject@@PAU_charrange@@PAPAUHMENU__@@@Z PROC NEAR ; CRichEditBaseCtrl::CRichEditOleCallback::GetContextMenu, COMDAT

; 549  : {

	push	ebp
	mov	ebp, esp
	push	esi

; 550  :    if (m_pOwner)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	je	SHORT $L75246

; 551  :       return m_pOwner->GetContextMenu(seltyp, lpoleobj, lpchrg, lphmenu);

	mov	esi, esp
	mov	ecx, DWORD PTR _lphmenu$[ebp]
	push	ecx
	mov	edx, DWORD PTR _lpchrg$[ebp]
	push	edx
	mov	eax, DWORD PTR _lpoleobj$[ebp]
	push	eax
	mov	cx, WORD PTR _seltyp$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+16]
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax+16]
	mov	eax, DWORD PTR [edx]
	call	DWORD PTR [eax+228]
	cmp	esi, esp
	call	__chkesp
	jmp	SHORT $L75245
$L75246:

; 552  : 
; 553  : 	return S_OK;

	xor	eax, eax
$L75245:

; 554  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	20					; 00000014H
?GetContextMenu@CRichEditOleCallback@CRichEditBaseCtrl@@UAGJGPAUIOleObject@@PAU_charrange@@PAPAUHMENU__@@@Z ENDP ; CRichEditBaseCtrl::CRichEditOleCallback::GetContextMenu
_TEXT	ENDS
PUBLIC	?DoEditFind@CRichEditBaseCtrl@@QAEXI@Z		; CRichEditBaseCtrl::DoEditFind
PUBLIC	?DoEditFindReplace@CRichEditBaseCtrl@@IAEXHI@Z	; CRichEditBaseCtrl::DoEditFindReplace
;	COMDAT ?DoEditFind@CRichEditBaseCtrl@@QAEXI@Z
_TEXT	SEGMENT
_nIDTitle$ = 8
_this$ = -4
?DoEditFind@CRichEditBaseCtrl@@QAEXI@Z PROC NEAR	; CRichEditBaseCtrl::DoEditFind, COMDAT

; 560  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 561  : 	ASSERT_VALID(this);

	push	561					; 00000231H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 562  : 	DoEditFindReplace(TRUE, nIDTitle);

	mov	ecx, DWORD PTR _nIDTitle$[ebp]
	push	ecx
	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DoEditFindReplace@CRichEditBaseCtrl@@IAEXHI@Z ; CRichEditBaseCtrl::DoEditFindReplace

; 563  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?DoEditFind@CRichEditBaseCtrl@@QAEXI@Z ENDP		; CRichEditBaseCtrl::DoEditFind
_TEXT	ENDS
PUBLIC	?DoEditReplace@CRichEditBaseCtrl@@QAEXI@Z	; CRichEditBaseCtrl::DoEditReplace
;	COMDAT ?DoEditReplace@CRichEditBaseCtrl@@QAEXI@Z
_TEXT	SEGMENT
_nIDTitle$ = 8
_this$ = -4
?DoEditReplace@CRichEditBaseCtrl@@QAEXI@Z PROC NEAR	; CRichEditBaseCtrl::DoEditReplace, COMDAT

; 566  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 567  : 	ASSERT_VALID(this);

	push	567					; 00000237H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 568  : 	DoEditFindReplace(FALSE, nIDTitle);

	mov	ecx, DWORD PTR _nIDTitle$[ebp]
	push	ecx
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?DoEditFindReplace@CRichEditBaseCtrl@@IAEXHI@Z ; CRichEditBaseCtrl::DoEditFindReplace

; 569  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?DoEditReplace@CRichEditBaseCtrl@@QAEXI@Z ENDP		; CRichEditBaseCtrl::DoEditReplace
_TEXT	ENDS
PUBLIC	?AdjustDialogPosition@CRichEditBaseCtrl@@IAEXPAVCDialog@@@Z ; CRichEditBaseCtrl::AdjustDialogPosition
EXTRN	?MoveWindow@CWnd@@QAEXPBUtagRECT@@H@Z:NEAR	; CWnd::MoveWindow
EXTRN	?GetCharPos@CRichEditCtrl@@QBE?AVCPoint@@J@Z:NEAR ; CRichEditCtrl::GetCharPos
EXTRN	?GetWindowRect@CWnd@@QBEXPAUtagRECT@@@Z:NEAR	; CWnd::GetWindowRect
EXTRN	?GetSel@CRichEditCtrl@@QBEXAAJ0@Z:NEAR		; CRichEditCtrl::GetSel
EXTRN	?ClientToScreen@CWnd@@QBEXPAUtagPOINT@@@Z:NEAR	; CWnd::ClientToScreen
EXTRN	__imp__GetSystemMetrics@4:NEAR
EXTRN	??0CRect@@QAE@XZ:NEAR				; CRect::CRect
EXTRN	?Height@CRect@@QBEHXZ:NEAR			; CRect::Height
EXTRN	?PtInRect@CRect@@QBEHUtagPOINT@@@Z:NEAR		; CRect::PtInRect
EXTRN	?OffsetRect@CRect@@QAEXHH@Z:NEAR		; CRect::OffsetRect
;	COMDAT ?AdjustDialogPosition@CRichEditBaseCtrl@@IAEXPAVCDialog@@@Z
_TEXT	SEGMENT
$T76181 = -44
_pDlg$ = 8
_this$ = -48
_lStart$ = -20
_lEnd$ = -32
_point$ = -28
_rectDlg$ = -16
_nVertExt$75273 = -36
?AdjustDialogPosition@CRichEditBaseCtrl@@IAEXPAVCDialog@@@Z PROC NEAR ; CRichEditBaseCtrl::AdjustDialogPosition, COMDAT

; 572  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 48					; 00000030H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-48]
	mov	ecx, 12					; 0000000cH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
$L75260:

; 573  : 	ASSERT(pDlg != NULL);

	cmp	DWORD PTR _pDlg$[ebp], 0
	jne	SHORT $L75263
	push	573					; 0000023dH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75263
	int	3
$L75263:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75260

; 574  : 	long lStart, lEnd;
; 575  : 	GetSel(lStart, lEnd);

	lea	ecx, DWORD PTR _lEnd$[ebp]
	push	ecx
	lea	edx, DWORD PTR _lStart$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSel@CRichEditCtrl@@QBEXAAJ0@Z	; CRichEditCtrl::GetSel

; 576  : 	CPoint point = GetCharPos(lStart);

	mov	eax, DWORD PTR _lStart$[ebp]
	push	eax
	lea	ecx, DWORD PTR _point$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetCharPos@CRichEditCtrl@@QBE?AVCPoint@@J@Z ; CRichEditCtrl::GetCharPos

; 577  : 	ClientToScreen(&point);

	lea	edx, DWORD PTR _point$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ClientToScreen@CWnd@@QBEXPAUtagPOINT@@@Z ; CWnd::ClientToScreen

; 578  : 	CRect rectDlg;

	lea	ecx, DWORD PTR _rectDlg$[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 579  : 	pDlg->GetWindowRect(&rectDlg);

	lea	eax, DWORD PTR _rectDlg$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDlg$[ebp]
	call	?GetWindowRect@CWnd@@QBEXPAUtagRECT@@@Z	; CWnd::GetWindowRect

; 580  : 	if (rectDlg.PtInRect(point))

	mov	ecx, DWORD PTR _point$[ebp]
	mov	DWORD PTR $T76181[ebp], ecx
	mov	edx, DWORD PTR _point$[ebp+4]
	mov	DWORD PTR $T76181[ebp+4], edx
	mov	eax, DWORD PTR $T76181[ebp+4]
	push	eax
	mov	ecx, DWORD PTR $T76181[ebp]
	push	ecx
	lea	ecx, DWORD PTR _rectDlg$[ebp]
	call	?PtInRect@CRect@@QBEHUtagPOINT@@@Z	; CRect::PtInRect
	test	eax, eax
	je	SHORT $L75270

; 582  : 		if (point.y > rectDlg.Height())

	lea	ecx, DWORD PTR _rectDlg$[ebp]
	call	?Height@CRect@@QBEHXZ			; CRect::Height
	cmp	DWORD PTR _point$[ebp+4], eax
	jle	SHORT $L75271

; 583  : 			rectDlg.OffsetRect(0, point.y - rectDlg.bottom - 20);

	mov	edx, DWORD PTR _point$[ebp+4]
	sub	edx, DWORD PTR _rectDlg$[ebp+12]
	sub	edx, 20					; 00000014H
	push	edx
	push	0
	lea	ecx, DWORD PTR _rectDlg$[ebp]
	call	?OffsetRect@CRect@@QAEXHH@Z		; CRect::OffsetRect

; 584  : 		else

	jmp	SHORT $L75274
$L75271:

; 586  : 			int nVertExt = GetSystemMetrics(SM_CYSCREEN);

	mov	esi, esp
	push	1
	call	DWORD PTR __imp__GetSystemMetrics@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nVertExt$75273[ebp], eax

; 587  : 			if (point.y + rectDlg.Height() < nVertExt)

	lea	ecx, DWORD PTR _rectDlg$[ebp]
	call	?Height@CRect@@QBEHXZ			; CRect::Height
	mov	ecx, DWORD PTR _point$[ebp+4]
	add	ecx, eax
	cmp	ecx, DWORD PTR _nVertExt$75273[ebp]
	jge	SHORT $L75274

; 588  : 				rectDlg.OffsetRect(0, 40 + point.y - rectDlg.top);

	mov	edx, DWORD PTR _point$[ebp+4]
	add	edx, 40					; 00000028H
	sub	edx, DWORD PTR _rectDlg$[ebp+4]
	push	edx
	push	0
	lea	ecx, DWORD PTR _rectDlg$[ebp]
	call	?OffsetRect@CRect@@QAEXHH@Z		; CRect::OffsetRect
$L75274:

; 590  : 		pDlg->MoveWindow(&rectDlg);

	push	1
	lea	eax, DWORD PTR _rectDlg$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pDlg$[ebp]
	call	?MoveWindow@CWnd@@QAEXPBUtagRECT@@H@Z	; CWnd::MoveWindow
$L75270:

; 592  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 48					; 00000030H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?AdjustDialogPosition@CRichEditBaseCtrl@@IAEXPAVCDialog@@@Z ENDP ; CRichEditBaseCtrl::AdjustDialogPosition
_TEXT	ENDS
EXTRN	?SetWindowTextW@CWnd@@QAEXPBG@Z:NEAR		; CWnd::SetWindowTextW
EXTRN	?ShowWindow@CWnd@@QAEHH@Z:NEAR			; CWnd::ShowWindow
EXTRN	?SetActiveWindow@CWnd@@QAEPAV1@XZ:NEAR		; CWnd::SetActiveWindow
EXTRN	?Create@CFindReplaceDialog@@QAEHHPBG0KPAVCWnd@@@Z:NEAR ; CFindReplaceDialog::Create
EXTRN	??0CEnString@@QAE@IPBG@Z:NEAR			; CEnString::CEnString
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
EXTRN	??1CEnString@@QAE@XZ:NEAR			; CEnString::~CEnString
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
EXTRN	?FindOneOf@CString@@QBEHPBG@Z:NEAR		; CString::FindOneOf
;	COMDAT data
; File D:\_CODE\Shared\richeditbasectrl.cpp
data	SEGMENT
$SG75295 DB	0aH, 00H, 0dH, 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T76188	DD	019930520H
	DD	03H
	DD	FLAT:$T76190
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76190	DD	0ffffffffH
	DD	FLAT:$L76184
	DD	00H
	DD	FLAT:$L76185
	DD	01H
	DD	FLAT:$L76186
xdata$x	ENDS
;	COMDAT ?DoEditFindReplace@CRichEditBaseCtrl@@IAEXHI@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_bFindOnly$ = 8
_nIDTitle$ = 12
_this$ = -32
_strFind$ = -20
_strReplace$ = -16
_dwFlags$ = -24
_sTitle$75311 = -28
?DoEditFindReplace@CRichEditBaseCtrl@@IAEXHI@Z PROC NEAR ; CRichEditBaseCtrl::DoEditFindReplace, COMDAT

; 595  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76189
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

; 596  : 	ASSERT_VALID(this);

	push	596					; 00000254H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 597  : 	
; 598  : 	if (m_findState.pFindReplaceDlg != NULL)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+92], 0
	je	$L75282

; 600  : 		if (m_findState.bFindOnly == bFindOnly)

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+96]
	cmp	eax, DWORD PTR _bFindOnly$[ebp]
	jne	SHORT $L75281

; 602  : 			m_findState.pFindReplaceDlg->SetActiveWindow();

	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+92]
	call	?SetActiveWindow@CWnd@@QAEPAV1@XZ	; CWnd::SetActiveWindow

; 603  : 			m_findState.pFindReplaceDlg->ShowWindow(SW_SHOW);

	push	5
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+92]
	call	?ShowWindow@CWnd@@QAEHH@Z		; CWnd::ShowWindow

; 604  : 			return;

	jmp	$L75279
$L75281:

; 608  : 			ASSERT(m_findState.bFindOnly != bFindOnly);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+96]
	cmp	ecx, DWORD PTR _bFindOnly$[ebp]
	jne	SHORT $L75286
	push	608					; 00000260H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75286
	int	3
$L75286:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75281

; 609  : 			m_findState.pFindReplaceDlg->SendMessage(WM_CLOSE);

	push	0
	push	0
	push	16					; 00000010H
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+92]
	call	?SendMessageW@CWnd@@QAEJIIJ@Z		; CWnd::SendMessageW
$L75287:

; 610  : 			ASSERT(m_findState.pFindReplaceDlg == NULL);

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+92], 0
	je	SHORT $L75290
	push	610					; 00000262H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75290
	int	3
$L75290:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75287

; 611  : 			ASSERT_VALID(this);

	push	611					; 00000263H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject
$L75282:

; 614  : 	CString strFind = GetSelText();

	lea	ecx, DWORD PTR _strFind$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSelText@CRichEditBaseCtrl@@QAE?AVCString@@XZ ; CRichEditBaseCtrl::GetSelText
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 615  : 
; 616  : 	// if selection is empty or spans multiple lines use old find text
; 617  : 	if (strFind.IsEmpty() || (strFind.FindOneOf(_T("\n\r")) != -1))

	lea	ecx, DWORD PTR _strFind$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75294
	push	OFFSET FLAT:$SG75295
	lea	ecx, DWORD PTR _strFind$[ebp]
	call	?FindOneOf@CString@@QBEHPBG@Z		; CString::FindOneOf
	cmp	eax, -1
	je	SHORT $L75293
$L75294:

; 618  : 		strFind = m_findState.strFind;

	mov	edx, DWORD PTR _this$[ebp]
	add	edx, 100				; 00000064H
	push	edx
	lea	ecx, DWORD PTR _strFind$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
$L75293:

; 619  : 
; 620  : 	CString strReplace = m_findState.strReplace;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 104				; 00000068H
	push	eax
	lea	ecx, DWORD PTR _strReplace$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 621  : 	m_findState.pFindReplaceDlg = NewFindReplaceDlg();

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+244]
	cmp	esi, esp
	call	__chkesp
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+92], eax
$L75297:

; 622  : 	ASSERT(m_findState.pFindReplaceDlg != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+92], 0
	jne	SHORT $L75300
	push	622					; 0000026eH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75300
	int	3
$L75300:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75297

; 623  : 
; 624  : 	DWORD dwFlags = NULL;

	mov	DWORD PTR _dwFlags$[ebp], 0

; 625  : 	if (m_findState.bNext)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+112], 0
	je	SHORT $L75302

; 626  : 		dwFlags |= FR_DOWN;

	mov	edx, DWORD PTR _dwFlags$[ebp]
	or	edx, 1
	mov	DWORD PTR _dwFlags$[ebp], edx
$L75302:

; 627  : 	if (m_findState.bCase)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+108], 0
	je	SHORT $L75303

; 628  : 		dwFlags |= FR_MATCHCASE;

	mov	ecx, DWORD PTR _dwFlags$[ebp]
	or	ecx, 4
	mov	DWORD PTR _dwFlags$[ebp], ecx
$L75303:

; 629  : 	if (m_findState.bWord)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+116], 0
	je	SHORT $L75304

; 630  : 		dwFlags |= FR_WHOLEWORD;

	mov	eax, DWORD PTR _dwFlags$[ebp]
	or	al, 2
	mov	DWORD PTR _dwFlags$[ebp], eax
$L75304:

; 631  : 
; 632  : 	// hide stuff that RichEdit doesn't support
; 633  : 	dwFlags |= FR_HIDEUPDOWN;

	mov	ecx, DWORD PTR _dwFlags$[ebp]
	or	ch, 64					; 00000040H
	mov	DWORD PTR _dwFlags$[ebp], ecx

; 634  : 	
; 635  : 	if (!m_findState.pFindReplaceDlg->Create(bFindOnly, strFind, strReplace, dwFlags, this))

	mov	edx, DWORD PTR _this$[ebp]
	push	edx
	mov	eax, DWORD PTR _dwFlags$[ebp]
	push	eax
	lea	ecx, DWORD PTR _strReplace$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _strFind$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _bFindOnly$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+92]
	call	?Create@CFindReplaceDialog@@QAEHHPBG0KPAVCWnd@@@Z ; CFindReplaceDialog::Create
	test	eax, eax
	jne	SHORT $L75305

; 637  : 		m_findState.pFindReplaceDlg = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+92], 0

; 638  : 		ASSERT_VALID(this);

	push	638					; 0000027eH
	push	OFFSET FLAT:_THIS_FILE
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 639  : 		return;

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _strReplace$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _strFind$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	jmp	$L75279
$L75305:

; 641  : 	ASSERT(m_findState.pFindReplaceDlg != NULL);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+92], 0
	jne	SHORT $L75309
	push	641					; 00000281H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75309
	int	3
$L75309:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75305

; 642  : 
; 643  : 	// set the title
; 644  : 	if (nIDTitle)

	cmp	DWORD PTR _nIDTitle$[ebp], 0
	je	SHORT $L75310

; 646  : 		CEnString sTitle(nIDTitle);

	push	0
	mov	ecx, DWORD PTR _nIDTitle$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _sTitle$75311[ebp]
	call	??0CEnString@@QAE@IPBG@Z		; CEnString::CEnString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 647  : 		m_findState.pFindReplaceDlg->SetWindowText(sTitle);

	lea	ecx, DWORD PTR _sTitle$75311[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+92]
	call	?SetWindowTextW@CWnd@@QAEXPBG@Z		; CWnd::SetWindowTextW

; 648  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sTitle$75311[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
$L75310:

; 649  : 
; 650  : 	m_findState.bFindOnly = bFindOnly;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _bFindOnly$[ebp]
	mov	DWORD PTR [eax+96], ecx

; 651  : 	m_findState.pFindReplaceDlg->SetActiveWindow();

	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+92]
	call	?SetActiveWindow@CWnd@@QAEPAV1@XZ	; CWnd::SetActiveWindow

; 652  : 	m_findState.pFindReplaceDlg->ShowWindow(SW_SHOW);

	push	5
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+92]
	call	?ShowWindow@CWnd@@QAEHH@Z		; CWnd::ShowWindow

; 653  : 	ASSERT_VALID(this);

	push	653					; 0000028dH
	push	OFFSET FLAT:_THIS_FILE
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 654  : }

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _strReplace$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _strFind$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75279:
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
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76184:
	lea	ecx, DWORD PTR _strFind$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76185:
	lea	ecx, DWORD PTR _strReplace$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76186:
	lea	ecx, DWORD PTR _sTitle$75311[ebp]
	call	??1CEnString@@QAE@XZ			; CEnString::~CEnString
	ret	0
$L76189:
	mov	eax, OFFSET FLAT:$T76188
	jmp	___CxxFrameHandler
text$x	ENDS
?DoEditFindReplace@CRichEditBaseCtrl@@IAEXHI@Z ENDP	; CRichEditBaseCtrl::DoEditFindReplace
PUBLIC	?TextNotFound@CRichEditBaseCtrl@@IAEXPBG@Z	; CRichEditBaseCtrl::TextNotFound
PUBLIC	?FindTextW@CRichEditBaseCtrl@@IAEHH@Z		; CRichEditBaseCtrl::FindTextW
EXTRN	??4CString@@QAEABV0@PBG@Z:NEAR			; CString::operator=
;	COMDAT ?OnFindNext@CRichEditBaseCtrl@@MAEXPBGHHH@Z
_TEXT	SEGMENT
_lpszFind$ = 8
_bNext$ = 12
_bCase$ = 16
_bWord$ = 20
_this$ = -4
?OnFindNext@CRichEditBaseCtrl@@MAEXPBGHHH@Z PROC NEAR	; CRichEditBaseCtrl::OnFindNext, COMDAT

; 657  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 658  : 	ASSERT_VALID(this);

	push	658					; 00000292H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 659  : 	
; 660  : 	m_findState.strFind = lpszFind;

	mov	ecx, DWORD PTR _lpszFind$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 100				; 00000064H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 661  : 	m_findState.bCase = bCase;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _bCase$[ebp]
	mov	DWORD PTR [edx+108], eax

; 662  : 	m_findState.bWord = bWord;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _bWord$[ebp]
	mov	DWORD PTR [ecx+116], edx

; 663  : 	m_findState.bNext = bNext;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _bNext$[ebp]
	mov	DWORD PTR [eax+112], ecx

; 664  : 
; 665  : 	if (!FindText())

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FindTextW@CRichEditBaseCtrl@@IAEHH@Z	; CRichEditBaseCtrl::FindTextW
	test	eax, eax
	jne	SHORT $L75319

; 666  : 		TextNotFound(m_findState.strFind);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 100				; 00000064H
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?TextNotFound@CRichEditBaseCtrl@@IAEXPBG@Z ; CRichEditBaseCtrl::TextNotFound

; 667  : 	else

	jmp	SHORT $L75320
$L75319:

; 668  : 		AdjustDialogPosition(m_findState.pFindReplaceDlg);

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+92]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AdjustDialogPosition@CRichEditBaseCtrl@@IAEXPAVCDialog@@@Z ; CRichEditBaseCtrl::AdjustDialogPosition
$L75320:

; 669  : 
; 670  : 	ASSERT_VALID(this);

	push	670					; 0000029eH
	push	OFFSET FLAT:_THIS_FILE
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 671  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?OnFindNext@CRichEditBaseCtrl@@MAEXPBGHHH@Z ENDP	; CRichEditBaseCtrl::OnFindNext
_TEXT	ENDS
PUBLIC	?SameAsSelected@CRichEditBaseCtrl@@IAEHPBGHH@Z	; CRichEditBaseCtrl::SameAsSelected
EXTRN	?ReplaceSel@CRichEditCtrl@@QAEXPBGH@Z:NEAR	; CRichEditCtrl::ReplaceSel
;	COMDAT ?OnReplaceSel@CRichEditBaseCtrl@@MAEXPBGHHH0@Z
_TEXT	SEGMENT
_lpszFind$ = 8
_bNext$ = 12
_bCase$ = 16
_bWord$ = 20
_lpszReplace$ = 24
_this$ = -4
?OnReplaceSel@CRichEditBaseCtrl@@MAEXPBGHHH0@Z PROC NEAR ; CRichEditBaseCtrl::OnReplaceSel, COMDAT

; 675  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 676  : 	ASSERT_VALID(this);

	push	676					; 000002a4H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 677  : 	
; 678  : 	m_findState.strFind = lpszFind;

	mov	ecx, DWORD PTR _lpszFind$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 100				; 00000064H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 679  : 	m_findState.strReplace = lpszReplace;

	mov	edx, DWORD PTR _lpszReplace$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 104				; 00000068H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 680  : 	m_findState.bCase = bCase;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _bCase$[ebp]
	mov	DWORD PTR [eax+108], ecx

; 681  : 	m_findState.bWord = bWord;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _bWord$[ebp]
	mov	DWORD PTR [edx+116], eax

; 682  : 	m_findState.bNext = bNext;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _bNext$[ebp]
	mov	DWORD PTR [ecx+112], edx

; 683  : 
; 684  : 	if (!SameAsSelected(m_findState.strFind, m_findState.bCase, m_findState.bWord))

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+116]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+108]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 100				; 00000064H
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SameAsSelected@CRichEditBaseCtrl@@IAEHPBGHH@Z ; CRichEditBaseCtrl::SameAsSelected
	test	eax, eax
	jne	SHORT $L75331

; 686  : 		if (!FindText())

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FindTextW@CRichEditBaseCtrl@@IAEHH@Z	; CRichEditBaseCtrl::FindTextW
	test	eax, eax
	jne	SHORT $L75330

; 688  : 			TextNotFound(m_findState.strFind);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 100				; 00000064H
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?TextNotFound@CRichEditBaseCtrl@@IAEXPBG@Z ; CRichEditBaseCtrl::TextNotFound

; 689  : 			return;

	jmp	SHORT $L75328
$L75330:

; 692  : 			AdjustDialogPosition(m_findState.pFindReplaceDlg);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+92]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AdjustDialogPosition@CRichEditBaseCtrl@@IAEXPAVCDialog@@@Z ; CRichEditBaseCtrl::AdjustDialogPosition
$L75331:

; 694  : 
; 695  : 	ReplaceSel(m_findState.strReplace, TRUE);

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 104				; 00000068H
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ReplaceSel@CRichEditCtrl@@QAEXPBGH@Z	; CRichEditCtrl::ReplaceSel

; 696  : 
; 697  : 	if (!FindText())

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FindTextW@CRichEditBaseCtrl@@IAEHH@Z	; CRichEditBaseCtrl::FindTextW
	test	eax, eax
	jne	SHORT $L75332

; 698  : 		TextNotFound(m_findState.strFind);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 100				; 00000064H
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?TextNotFound@CRichEditBaseCtrl@@IAEXPBG@Z ; CRichEditBaseCtrl::TextNotFound

; 699  : 	else

	jmp	SHORT $L75333
$L75332:

; 700  : 		AdjustDialogPosition(m_findState.pFindReplaceDlg);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+92]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?AdjustDialogPosition@CRichEditBaseCtrl@@IAEXPAVCDialog@@@Z ; CRichEditBaseCtrl::AdjustDialogPosition
$L75333:

; 701  : 
; 702  : 	ASSERT_VALID(this);

	push	702					; 000002beH
	push	OFFSET FLAT:_THIS_FILE
	mov	edx, DWORD PTR _this$[ebp]
	push	edx
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject
$L75328:

; 703  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	20					; 00000014H
?OnReplaceSel@CRichEditBaseCtrl@@MAEXPBGHHH0@Z ENDP	; CRichEditBaseCtrl::OnReplaceSel
_TEXT	ENDS
EXTRN	?SetSel@CRichEditCtrl@@QAEXJJ@Z:NEAR		; CRichEditCtrl::SetSel
EXTRN	?HideSelection@CRichEditCtrl@@QAEXHH@Z:NEAR	; CRichEditCtrl::HideSelection
EXTRN	??0CWaitCursor@@QAE@XZ:NEAR			; CWaitCursor::CWaitCursor
EXTRN	??1CWaitCursor@@QAE@XZ:NEAR			; CWaitCursor::~CWaitCursor
;	COMDAT xdata$x
; File D:\_CODE\Shared\richeditbasectrl.cpp
xdata$x	SEGMENT
$T76202	DD	019930520H
	DD	01H
	DD	FLAT:$T76204
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76204	DD	0ffffffffH
	DD	FLAT:$L76200
xdata$x	ENDS
;	COMDAT ?OnReplaceAll@CRichEditBaseCtrl@@MAEXPBG0HH@Z
_TEXT	SEGMENT
__$EHRec$ = -12
_lpszFind$ = 8
_lpszReplace$ = 12
_bCase$ = 16
_bWord$ = 20
_this$ = -20
_wait$ = -16
?OnReplaceAll@CRichEditBaseCtrl@@MAEXPBG0HH@Z PROC NEAR	; CRichEditBaseCtrl::OnReplaceAll, COMDAT

; 706  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76203
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 707  : 	ASSERT_VALID(this);

	push	707					; 000002c3H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 708  : 	
; 709  : 	// start searching at the beginning of the text so that we know to stop at the end
; 710  : 	SetSel(0, 0);

	push	0
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetSel@CRichEditCtrl@@QAEXJJ@Z		; CRichEditCtrl::SetSel

; 711  : 
; 712  : 	m_findState.strFind = lpszFind;

	mov	ecx, DWORD PTR _lpszFind$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 100				; 00000064H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 713  : 	m_findState.strReplace = lpszReplace;

	mov	edx, DWORD PTR _lpszReplace$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 104				; 00000068H
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 714  : 	m_findState.bCase = bCase;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _bCase$[ebp]
	mov	DWORD PTR [eax+108], ecx

; 715  : 	m_findState.bWord = bWord;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR _bWord$[ebp]
	mov	DWORD PTR [edx+116], eax

; 716  : 	m_findState.bNext = TRUE;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+112], 1

; 717  : 
; 718  : 	CWaitCursor wait;

	lea	ecx, DWORD PTR _wait$[ebp]
	call	??0CWaitCursor@@QAE@XZ			; CWaitCursor::CWaitCursor
	mov	DWORD PTR __$EHRec$[ebp+8], 0
$L75343:

; 719  : 
; 720  : 	while (FindText(FALSE))

	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FindTextW@CRichEditBaseCtrl@@IAEHH@Z	; CRichEditBaseCtrl::FindTextW
	test	eax, eax
	je	SHORT $L75344

; 721  : 		ReplaceSel(m_findState.strReplace, TRUE);

	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 104				; 00000068H
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ReplaceSel@CRichEditCtrl@@QAEXPBGH@Z	; CRichEditCtrl::ReplaceSel
	jmp	SHORT $L75343
$L75344:

; 722  : 
; 723  : 	TextNotFound(m_findState.strFind);

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 100				; 00000064H
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?TextNotFound@CRichEditBaseCtrl@@IAEXPBG@Z ; CRichEditBaseCtrl::TextNotFound

; 724  : 	HideSelection(FALSE, FALSE);

	push	0
	push	0
	mov	ecx, DWORD PTR _this$[ebp]
	call	?HideSelection@CRichEditCtrl@@QAEXHH@Z	; CRichEditCtrl::HideSelection

; 725  : 
; 726  : 	ASSERT_VALID(this);

	push	726					; 000002d6H
	push	OFFSET FLAT:_THIS_FILE
	mov	edx, DWORD PTR _this$[ebp]
	push	edx
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 727  : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _wait$[ebp]
	call	??1CWaitCursor@@QAE@XZ			; CWaitCursor::~CWaitCursor
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76200:
	lea	ecx, DWORD PTR _wait$[ebp]
	call	??1CWaitCursor@@QAE@XZ			; CWaitCursor::~CWaitCursor
	ret	0
$L76203:
	mov	eax, OFFSET FLAT:$T76202
	jmp	___CxxFrameHandler
text$x	ENDS
?OnReplaceAll@CRichEditBaseCtrl@@MAEXPBG0HH@Z ENDP	; CRichEditBaseCtrl::OnReplaceAll
EXTRN	?SetFocus@CWnd@@QAEPAV1@XZ:NEAR			; CWnd::SetFocus
EXTRN	?GetNotifier@CFindReplaceDialog@@SGPAV1@J@Z:NEAR ; CFindReplaceDialog::GetNotifier
EXTRN	?GetReplaceString@CFindReplaceDialog@@QBE?AVCString@@XZ:NEAR ; CFindReplaceDialog::GetReplaceString
EXTRN	?GetFindString@CFindReplaceDialog@@QBE?AVCString@@XZ:NEAR ; CFindReplaceDialog::GetFindString
EXTRN	?SearchDown@CFindReplaceDialog@@QBEHXZ:NEAR	; CFindReplaceDialog::SearchDown
EXTRN	?FindNext@CFindReplaceDialog@@QBEHXZ:NEAR	; CFindReplaceDialog::FindNext
EXTRN	?MatchCase@CFindReplaceDialog@@QBEHXZ:NEAR	; CFindReplaceDialog::MatchCase
EXTRN	?MatchWholeWord@CFindReplaceDialog@@QBEHXZ:NEAR	; CFindReplaceDialog::MatchWholeWord
EXTRN	?ReplaceCurrent@CFindReplaceDialog@@QBEHXZ:NEAR	; CFindReplaceDialog::ReplaceCurrent
EXTRN	?ReplaceAll@CFindReplaceDialog@@QBEHXZ:NEAR	; CFindReplaceDialog::ReplaceAll
EXTRN	?IsTerminating@CFindReplaceDialog@@QBEHXZ:NEAR	; CFindReplaceDialog::IsTerminating
;	COMDAT xdata$x
; File D:\_CODE\Shared\richeditbasectrl.cpp
xdata$x	SEGMENT
$T76220	DD	019930520H
	DD	05H
	DD	FLAT:$T76222
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76222	DD	0ffffffffH
	DD	FLAT:$L76214
	DD	0ffffffffH
	DD	FLAT:$L76215
	DD	01H
	DD	FLAT:$L76216
	DD	0ffffffffH
	DD	FLAT:$L76217
	DD	03H
	DD	FLAT:$L76218
xdata$x	ENDS
;	COMDAT ?OnFindReplaceCmd@CRichEditBaseCtrl@@IAEJIJ@Z
_TEXT	SEGMENT
$T76209 = -20
$T76210 = -24
$T76211 = -28
$T76212 = -32
$T76213 = -36
__$EHRec$ = -12
_lParam$ = 12
_this$ = -40
_pDialog$ = -16
?OnFindReplaceCmd@CRichEditBaseCtrl@@IAEJIJ@Z PROC NEAR	; CRichEditBaseCtrl::OnFindReplaceCmd, COMDAT

; 730  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76221
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 68					; 00000044H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-80]
	mov	ecx, 17					; 00000011H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 731  : 	ASSERT_VALID(this);

	push	731					; 000002dbH
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 732  : 	CFindReplaceDialog* pDialog = CFindReplaceDialog::GetNotifier(lParam);

	mov	ecx, DWORD PTR _lParam$[ebp]
	push	ecx
	call	?GetNotifier@CFindReplaceDialog@@SGPAV1@J@Z ; CFindReplaceDialog::GetNotifier
	mov	DWORD PTR _pDialog$[ebp], eax
$L75351:

; 733  : 	ASSERT(pDialog != NULL);

	cmp	DWORD PTR _pDialog$[ebp], 0
	jne	SHORT $L75354
	push	733					; 000002ddH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75354
	int	3
$L75354:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75351
$L75353:

; 734  : 	
; 735  : 	ASSERT(pDialog == m_findState.pFindReplaceDlg);

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _pDialog$[ebp]
	cmp	ecx, DWORD PTR [eax+92]
	je	SHORT $L75358
	push	735					; 000002dfH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75358
	int	3
$L75358:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75353

; 736  : 	if (pDialog->IsTerminating())

	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?IsTerminating@CFindReplaceDialog@@QBEHXZ ; CFindReplaceDialog::IsTerminating
	test	eax, eax
	je	SHORT $L75359

; 738  : 		m_findState.pFindReplaceDlg = NULL;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+92], 0

; 739  : 		SetFocus();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetFocus@CWnd@@QAEPAV1@XZ		; CWnd::SetFocus

; 741  : 	else if (pDialog->FindNext())

	jmp	$L75372
$L75359:
	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?FindNext@CFindReplaceDialog@@QBEHXZ	; CFindReplaceDialog::FindNext
	test	eax, eax
	je	SHORT $L75361

; 743  : 		OnFindNext(pDialog->GetFindString(), pDialog->SearchDown(),
; 744  : 			pDialog->MatchCase(), pDialog->MatchWholeWord());

	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?MatchWholeWord@CFindReplaceDialog@@QBEHXZ ; CFindReplaceDialog::MatchWholeWord
	mov	esi, esp
	push	eax
	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?MatchCase@CFindReplaceDialog@@QBEHXZ	; CFindReplaceDialog::MatchCase
	push	eax
	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?SearchDown@CFindReplaceDialog@@QBEHXZ	; CFindReplaceDialog::SearchDown
	push	eax
	lea	ecx, DWORD PTR $T76209[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?GetFindString@CFindReplaceDialog@@QBE?AVCString@@XZ ; CFindReplaceDialog::GetFindString
	mov	DWORD PTR -44+[ebp], eax
	mov	edx, DWORD PTR -44+[ebp]
	mov	DWORD PTR -48+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	ecx, DWORD PTR -48+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+232]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76209[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 746  : 	else if (pDialog->ReplaceCurrent())

	jmp	$L75372
$L75361:
	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?ReplaceCurrent@CFindReplaceDialog@@QBEHXZ ; CFindReplaceDialog::ReplaceCurrent
	test	eax, eax
	je	$L75364
$L75365:

; 748  : 		ASSERT(!m_findState.bFindOnly);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+96], 0
	je	SHORT $L75368
	push	748					; 000002ecH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75368
	int	3
$L75368:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75365

; 749  : 		OnReplaceSel(pDialog->GetFindString(),
; 750  : 			pDialog->SearchDown(), pDialog->MatchCase(), pDialog->MatchWholeWord(),
; 751  : 			pDialog->GetReplaceString());

	lea	edx, DWORD PTR $T76210[ebp]
	push	edx
	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?GetReplaceString@CFindReplaceDialog@@QBE?AVCString@@XZ ; CFindReplaceDialog::GetReplaceString
	mov	DWORD PTR -52+[ebp], eax
	mov	eax, DWORD PTR -52+[ebp]
	mov	DWORD PTR -56+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 1
	mov	ecx, DWORD PTR -56+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?MatchWholeWord@CFindReplaceDialog@@QBEHXZ ; CFindReplaceDialog::MatchWholeWord
	push	eax
	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?MatchCase@CFindReplaceDialog@@QBEHXZ	; CFindReplaceDialog::MatchCase
	push	eax
	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?SearchDown@CFindReplaceDialog@@QBEHXZ	; CFindReplaceDialog::SearchDown
	push	eax
	lea	ecx, DWORD PTR $T76211[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?GetFindString@CFindReplaceDialog@@QBE?AVCString@@XZ ; CFindReplaceDialog::GetFindString
	mov	DWORD PTR -60+[ebp], eax
	mov	edx, DWORD PTR -60+[ebp]
	mov	DWORD PTR -64+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR -64+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+236]
	cmp	esi, esp
	call	__chkesp
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR $T76211[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76210[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 753  : 	else if (pDialog->ReplaceAll())

	jmp	$L75372
$L75364:
	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?ReplaceAll@CFindReplaceDialog@@QBEHXZ	; CFindReplaceDialog::ReplaceAll
	test	eax, eax
	je	$L75372
$L75373:

; 755  : 		ASSERT(!m_findState.bFindOnly);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+96], 0
	je	SHORT $L75376
	push	755					; 000002f3H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75376
	int	3
$L75376:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75373

; 756  : 		OnReplaceAll(pDialog->GetFindString(), pDialog->GetReplaceString(),
; 757  : 			pDialog->MatchCase(), pDialog->MatchWholeWord());

	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?MatchWholeWord@CFindReplaceDialog@@QBEHXZ ; CFindReplaceDialog::MatchWholeWord
	mov	esi, esp
	push	eax
	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?MatchCase@CFindReplaceDialog@@QBEHXZ	; CFindReplaceDialog::MatchCase
	push	eax
	lea	edx, DWORD PTR $T76212[ebp]
	push	edx
	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?GetReplaceString@CFindReplaceDialog@@QBE?AVCString@@XZ ; CFindReplaceDialog::GetReplaceString
	mov	DWORD PTR -68+[ebp], eax
	mov	eax, DWORD PTR -68+[ebp]
	mov	DWORD PTR -72+[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], 3
	mov	ecx, DWORD PTR -72+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR $T76213[ebp]
	push	ecx
	mov	ecx, DWORD PTR _pDialog$[ebp]
	call	?GetFindString@CFindReplaceDialog@@QBE?AVCString@@XZ ; CFindReplaceDialog::GetFindString
	mov	DWORD PTR -76+[ebp], eax
	mov	edx, DWORD PTR -76+[ebp]
	mov	DWORD PTR -80+[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 4
	mov	ecx, DWORD PTR -80+[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+240]
	cmp	esi, esp
	call	__chkesp
	mov	BYTE PTR __$EHRec$[ebp+8], 3
	lea	ecx, DWORD PTR $T76213[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR $T76212[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75372:

; 759  : 	ASSERT_VALID(this);

	push	759					; 000002f7H
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 760  : 	return 0;

	xor	eax, eax

; 761  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 80					; 00000050H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76214:
	lea	ecx, DWORD PTR $T76209[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76215:
	lea	ecx, DWORD PTR $T76210[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76216:
	lea	ecx, DWORD PTR $T76211[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76217:
	lea	ecx, DWORD PTR $T76212[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76218:
	lea	ecx, DWORD PTR $T76213[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76221:
	mov	eax, OFFSET FLAT:$T76220
	jmp	___CxxFrameHandler
text$x	ENDS
?OnFindReplaceCmd@CRichEditBaseCtrl@@IAEJIJ@Z ENDP	; CRichEditBaseCtrl::OnFindReplaceCmd
EXTRN	__imp__lstrcmpW@8:NEAR
EXTRN	__imp__lstrcmpiW@8:NEAR
EXTRN	__imp__lstrlenW@4:NEAR
;	COMDAT xdata$x
; File D:\_CODE\Shared\richeditbasectrl.cpp
xdata$x	SEGMENT
$T76234	DD	019930520H
	DD	01H
	DD	FLAT:$T76236
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76236	DD	0ffffffffH
	DD	FLAT:$L76232
xdata$x	ENDS
;	COMDAT ?SameAsSelected@CRichEditBaseCtrl@@IAEHPBGHH@Z
_TEXT	SEGMENT
$T76227 = -32
__$EHRec$ = -12
_lpszCompare$ = 8
_bCase$ = 12
_this$ = -36
_nLen$ = -28
_lStartChar$ = -20
_lEndChar$ = -24
_strSelect$ = -16
?SameAsSelected@CRichEditBaseCtrl@@IAEHPBGHH@Z PROC NEAR ; CRichEditBaseCtrl::SameAsSelected, COMDAT

; 764  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76235
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

; 765  : 	// check length first
; 766  : 	size_t nLen = lstrlen(lpszCompare);

	mov	esi, esp
	mov	eax, DWORD PTR _lpszCompare$[ebp]
	push	eax
	call	DWORD PTR __imp__lstrlenW@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nLen$[ebp], eax

; 767  : 	long lStartChar, lEndChar;
; 768  : 	GetSel(lStartChar, lEndChar);

	lea	ecx, DWORD PTR _lEndChar$[ebp]
	push	ecx
	lea	edx, DWORD PTR _lStartChar$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSel@CRichEditCtrl@@QBEXAAJ0@Z	; CRichEditCtrl::GetSel

; 769  : 	if (nLen != (size_t)(lEndChar - lStartChar))

	mov	eax, DWORD PTR _lEndChar$[ebp]
	sub	eax, DWORD PTR _lStartChar$[ebp]
	cmp	DWORD PTR _nLen$[ebp], eax
	je	SHORT $L75389

; 770  : 		return FALSE;

	xor	eax, eax
	jmp	$L75384
$L75389:

; 771  : 
; 772  : 	// length is the same, check contents
; 773  : 	CString strSelect = GetSelText();

	lea	ecx, DWORD PTR _strSelect$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSelText@CRichEditBaseCtrl@@QAE?AVCString@@XZ ; CRichEditBaseCtrl::GetSelText
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 774  : 	return (bCase && lstrcmp(lpszCompare, strSelect) == 0) ||
; 775  : 		(!bCase && lstrcmpi(lpszCompare, strSelect) == 0);

	cmp	DWORD PTR _bCase$[ebp], 0
	je	SHORT $L76228
	lea	ecx, DWORD PTR _strSelect$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	mov	edx, DWORD PTR _lpszCompare$[ebp]
	push	edx
	call	DWORD PTR __imp__lstrcmpW@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L76230
$L76228:
	cmp	DWORD PTR _bCase$[ebp], 0
	jne	SHORT $L76229
	lea	ecx, DWORD PTR _strSelect$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	esi, esp
	push	eax
	mov	eax, DWORD PTR _lpszCompare$[ebp]
	push	eax
	call	DWORD PTR __imp__lstrcmpiW@8
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	je	SHORT $L76230
$L76229:
	mov	DWORD PTR -40+[ebp], 0
	jmp	SHORT $L76231
$L76230:
	mov	DWORD PTR -40+[ebp], 1
$L76231:
	mov	ecx, DWORD PTR -40+[ebp]
	mov	DWORD PTR $T76227[ebp], ecx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _strSelect$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76227[ebp]
$L75384:

; 776  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76232:
	lea	ecx, DWORD PTR _strSelect$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76235:
	mov	eax, OFFSET FLAT:$T76234
	jmp	___CxxFrameHandler
text$x	ENDS
?SameAsSelected@CRichEditBaseCtrl@@IAEHPBGHH@Z ENDP	; CRichEditBaseCtrl::SameAsSelected
PUBLIC	?FindTextW@CRichEditBaseCtrl@@IAEHPBGHHH@Z	; CRichEditBaseCtrl::FindTextW
;	COMDAT ?FindTextW@CRichEditBaseCtrl@@IAEHH@Z
_TEXT	SEGMENT
_bWrap$ = 8
_this$ = -4
?FindTextW@CRichEditBaseCtrl@@IAEHH@Z PROC NEAR		; CRichEditBaseCtrl::FindTextW, COMDAT

; 779  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 780  : 	return FindText(m_findState.strFind, m_findState.bCase, m_findState.bWord, bWrap);

	mov	eax, DWORD PTR _bWrap$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+116]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+108]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 100				; 00000064H
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FindTextW@CRichEditBaseCtrl@@IAEHPBGHHH@Z ; CRichEditBaseCtrl::FindTextW

; 781  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?FindTextW@CRichEditBaseCtrl@@IAEHH@Z ENDP		; CRichEditBaseCtrl::FindTextW
_TEXT	ENDS
PUBLIC	?FindAndSelect@CRichEditBaseCtrl@@IAEJKAAU_findtextexw@@@Z ; CRichEditBaseCtrl::FindAndSelect
EXTRN	?GetTextLength@CRichEditCtrl@@QBEJXZ:NEAR	; CRichEditCtrl::GetTextLength
;	COMDAT xdata$x
; File D:\_CODE\Shared\richeditbasectrl.cpp
xdata$x	SEGMENT
$T76250	DD	019930520H
	DD	02H
	DD	FLAT:$T76252
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76252	DD	0ffffffffH
	DD	FLAT:$L76247
	DD	00H
	DD	FLAT:$L76248
xdata$x	ENDS
;	COMDAT ?FindTextW@CRichEditBaseCtrl@@IAEHPBGHHH@Z
_TEXT	SEGMENT
$T76243 = -56
$T76244 = -60
$T76245 = -64
$T76246 = -68
__$EHRec$ = -12
_lpszFind$ = 8
_bCase$ = 12
_bWord$ = 16
_bWrap$ = 20
_this$ = -72
_wait$ = -16
_ft$ = -36
_cr$75415 = -48
_ch$75416 = -52
_dwFlags$ = -40
?FindTextW@CRichEditBaseCtrl@@IAEHPBGHHH@Z PROC NEAR	; CRichEditBaseCtrl::FindTextW, COMDAT

; 784  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76251
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 60					; 0000003cH
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-72]
	mov	ecx, 15					; 0000000fH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 785  : 	CWaitCursor wait;

	lea	ecx, DWORD PTR _wait$[ebp]
	call	??0CWaitCursor@@QAE@XZ			; CWaitCursor::CWaitCursor
	mov	DWORD PTR __$EHRec$[ebp+8], 0
$L75405:

; 786  : 
; 787  : 	ASSERT(lpszFind != NULL);

	cmp	DWORD PTR _lpszFind$[ebp], 0
	jne	SHORT $L75408
	push	787					; 00000313H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75408
	int	3
$L75408:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75405

; 788  : 
; 789  : 	if (!lpszFind || !*lpszFind)

	cmp	DWORD PTR _lpszFind$[ebp], 0
	je	SHORT $L75410
	mov	ecx, DWORD PTR _lpszFind$[ebp]
	xor	edx, edx
	mov	dx, WORD PTR [ecx]
	test	edx, edx
	jne	SHORT $L75409
$L75410:

; 790  : 		return FALSE;

	mov	DWORD PTR $T76243[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _wait$[ebp]
	call	??1CWaitCursor@@QAE@XZ			; CWaitCursor::~CWaitCursor
	mov	eax, DWORD PTR $T76243[ebp]
	jmp	$L75403
$L75409:

; 795  : 	GetSel(ft.chrg);

	lea	eax, DWORD PTR _ft$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSel@CRichEditCtrl@@QBEXAAU_charrange@@@Z ; CRichEditCtrl::GetSel

; 796  : 
; 797  : 	// convert text to multibyte string for RichEdit50W
; 798  : #ifdef _UNICODE
; 799  : 	ft.lpstrText = (LPTSTR)lpszFind;

	mov	ecx, DWORD PTR _lpszFind$[ebp]
	mov	DWORD PTR _ft$[ebp+8], ecx

; 800  : #else
; 801  : 	ft.lpstrText = (LPTSTR)A2BSTR((LPTSTR)lpszFind);
; 802  : #endif
; 803  : 
; 804  : 	// is there is a selection? for instance, previously found text
; 805  : 	if (ft.chrg.cpMin < ft.chrg.cpMax) 

	mov	edx, DWORD PTR _ft$[ebp]
	cmp	edx, DWORD PTR _ft$[ebp+4]
	jge	SHORT $L75414

; 807  : 		// If byte at beginning of selection is a DBCS lead byte,
; 808  : 		// increment by one extra byte.
; 809  : 		CHARRANGE cr = { ft.chrg.cpMin, ft.chrg.cpMin + 1 };

	mov	eax, DWORD PTR _ft$[ebp]
	mov	DWORD PTR _cr$75415[ebp], eax
	mov	ecx, DWORD PTR _ft$[ebp]
	add	ecx, 1
	mov	DWORD PTR _cr$75415[ebp+4], ecx

; 810  : 		CString ch = GetTextRange(cr);

	lea	edx, DWORD PTR _cr$75415[ebp]
	push	edx
	lea	eax, DWORD PTR _ch$75416[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetTextRange@CRichEditBaseCtrl@@QAE?AVCString@@ABU_charrange@@@Z ; CRichEditBaseCtrl::GetTextRange
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 811  : 
; 812  : 		if (!ch.IsEmpty() && _istlead(ch[0]))

	lea	ecx, DWORD PTR _ch$75416[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75418
	xor	ecx, ecx
	test	ecx, ecx
	je	SHORT $L75418
$L75419:

; 814  : 			ASSERT(ft.chrg.cpMax - ft.chrg.cpMin >= 2);

	mov	edx, DWORD PTR _ft$[ebp+4]
	sub	edx, DWORD PTR _ft$[ebp]
	cmp	edx, 2
	jge	SHORT $L75422
	push	814					; 0000032eH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75422
	int	3
$L75422:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L75419

; 815  : 			ft.chrg.cpMin++;

	mov	ecx, DWORD PTR _ft$[ebp]
	add	ecx, 1
	mov	DWORD PTR _ft$[ebp], ecx
$L75418:

; 817  : 
; 818  : 		// then shift the selection start forward a char so that we don't simply
; 819  : 		// find the already selected text.
; 820  : 		ft.chrg.cpMin++;

	mov	edx, DWORD PTR _ft$[ebp]
	add	edx, 1
	mov	DWORD PTR _ft$[ebp], edx

; 821  : 	}

	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _ch$75416[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L75414:

; 822  : 
; 823  : 	// always search to the end of the text
; 824  : 	ft.chrg.cpMax = GetTextLength();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetTextLength@CRichEditCtrl@@QBEJXZ	; CRichEditCtrl::GetTextLength
	mov	DWORD PTR _ft$[ebp+4], eax

; 825  : 
; 826  : 	DWORD dwFlags = FR_DOWN;

	mov	DWORD PTR _dwFlags$[ebp], 1

; 827  : 	dwFlags |= bCase ? FR_MATCHCASE : 0;

	mov	eax, DWORD PTR _bCase$[ebp]
	neg	eax
	sbb	eax, eax
	and	eax, 4
	mov	ecx, DWORD PTR _dwFlags$[ebp]
	or	ecx, eax
	mov	DWORD PTR _dwFlags$[ebp], ecx

; 828  : 	dwFlags |= bWord ? FR_WHOLEWORD : 0;

	mov	edx, DWORD PTR _bWord$[ebp]
	neg	edx
	sbb	edx, edx
	and	edx, 2
	mov	eax, DWORD PTR _dwFlags$[ebp]
	or	eax, edx
	mov	DWORD PTR _dwFlags$[ebp], eax

; 829  : 
; 830  : 	// if we find the text return TRUE
; 831  : 	if (FindAndSelect(dwFlags, ft) != -1)

	lea	ecx, DWORD PTR _ft$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwFlags$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FindAndSelect@CRichEditBaseCtrl@@IAEJKAAU_findtextexw@@@Z ; CRichEditBaseCtrl::FindAndSelect
	cmp	eax, -1
	je	SHORT $L75424

; 832  : 		return TRUE;

	mov	DWORD PTR $T76244[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _wait$[ebp]
	call	??1CWaitCursor@@QAE@XZ			; CWaitCursor::~CWaitCursor
	mov	eax, DWORD PTR $T76244[ebp]
	jmp	SHORT $L75403
$L75424:

; 833  : 
; 834  : 	// else we need to restart the search from the beginning
; 835  : 	if (bWrap)

	cmp	DWORD PTR _bWrap$[ebp], 0
	je	SHORT $L75426

; 837  : 		ft.chrg.cpMin = 0;

	mov	DWORD PTR _ft$[ebp], 0

; 838  : 		ft.chrg.cpMax = GetTextLength();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetTextLength@CRichEditCtrl@@QBEJXZ	; CRichEditCtrl::GetTextLength
	mov	DWORD PTR _ft$[ebp+4], eax

; 839  : 
; 840  : 		return (FindAndSelect(dwFlags, ft) != -1);

	lea	eax, DWORD PTR _ft$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwFlags$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?FindAndSelect@CRichEditBaseCtrl@@IAEJKAAU_findtextexw@@@Z ; CRichEditBaseCtrl::FindAndSelect
	xor	edx, edx
	cmp	eax, -1
	setne	dl
	mov	DWORD PTR $T76245[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _wait$[ebp]
	call	??1CWaitCursor@@QAE@XZ			; CWaitCursor::~CWaitCursor
	mov	eax, DWORD PTR $T76245[ebp]
	jmp	SHORT $L75403
$L75426:

; 842  : 
; 843  : 	// else
; 844  : 	return FALSE;

	mov	DWORD PTR $T76246[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _wait$[ebp]
	call	??1CWaitCursor@@QAE@XZ			; CWaitCursor::~CWaitCursor
	mov	eax, DWORD PTR $T76246[ebp]
$L75403:

; 845  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 72					; 00000048H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76247:
	lea	ecx, DWORD PTR _wait$[ebp]
	call	??1CWaitCursor@@QAE@XZ			; CWaitCursor::~CWaitCursor
	ret	0
$L76248:
	lea	ecx, DWORD PTR _ch$75416[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76251:
	mov	eax, OFFSET FLAT:$T76250
	jmp	___CxxFrameHandler
text$x	ENDS
?FindTextW@CRichEditBaseCtrl@@IAEHPBGHHH@Z ENDP		; CRichEditBaseCtrl::FindTextW
EXTRN	__imp__SendMessageW@16:NEAR
;	COMDAT ?FindAndSelect@CRichEditBaseCtrl@@IAEJKAAU_findtextexw@@@Z
_TEXT	SEGMENT
_dwFlags$ = 8
_ft$ = 12
_this$ = -8
_index$ = -4
?FindAndSelect@CRichEditBaseCtrl@@IAEJKAAU_findtextexw@@@Z PROC NEAR ; CRichEditBaseCtrl::FindAndSelect, COMDAT

; 848  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 849  : 	long index = (long)::SendMessage(m_hWnd, EM_FINDTEXTEX, dwFlags, (LPARAM)&ft);

	mov	esi, esp
	mov	eax, DWORD PTR _ft$[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwFlags$[ebp]
	push	ecx
	push	1103					; 0000044fH
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+32]
	push	eax
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _index$[ebp], eax

; 850  : 
; 851  : 	if (index == -1)

	cmp	DWORD PTR _index$[ebp], -1
	jne	SHORT $L75437

; 852  : 		index = (long)::SendMessage(m_hWnd, EM_FINDTEXTEXW, dwFlags, (LPARAM)&ft);

	mov	esi, esp
	mov	ecx, DWORD PTR _ft$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dwFlags$[ebp]
	push	edx
	push	1148					; 0000047cH
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+32]
	push	ecx
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _index$[ebp], eax
$L75437:

; 853  : 
; 854  : 	if (index != -1) // i.e. we found something

	cmp	DWORD PTR _index$[ebp], -1
	je	SHORT $L75440

; 855  : 		SetSel(ft.chrgText);

	mov	edx, DWORD PTR _ft$[ebp]
	add	edx, 12					; 0000000cH
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetSel@CRichEditCtrl@@QAEXAAU_charrange@@@Z ; CRichEditCtrl::SetSel
$L75440:

; 856  : 	
; 857  : 	return index;

	mov	eax, DWORD PTR _index$[ebp]

; 858  : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?FindAndSelect@CRichEditBaseCtrl@@IAEJKAAU_findtextexw@@@Z ENDP ; CRichEditBaseCtrl::FindAndSelect
_TEXT	ENDS
EXTRN	__imp__MessageBeep@4:NEAR
;	COMDAT ?TextNotFound@CRichEditBaseCtrl@@IAEXPBG@Z
_TEXT	SEGMENT
_this$ = -4
?TextNotFound@CRichEditBaseCtrl@@IAEXPBG@Z PROC NEAR	; CRichEditBaseCtrl::TextNotFound, COMDAT

; 861  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 862  : 	ASSERT_VALID(this);

	push	862					; 0000035eH
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _this$[ebp]
	push	eax
	call	?AfxAssertValidObject@@YGXPBVCObject@@PBDH@Z ; AfxAssertValidObject

; 863  : 	MessageBeep(MB_ICONHAND);

	mov	esi, esp
	push	16					; 00000010H
	call	DWORD PTR __imp__MessageBeep@4
	cmp	esi, esp
	call	__chkesp

; 864  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?TextNotFound@CRichEditBaseCtrl@@IAEXPBG@Z ENDP		; CRichEditBaseCtrl::TextNotFound
_TEXT	ENDS
PUBLIC	?IsFindDialog@CRichEditBaseCtrl@@IBEHPAUHWND__@@@Z ; CRichEditBaseCtrl::IsFindDialog
;	COMDAT ?IsFindDialog@CRichEditBaseCtrl@@IBEHPAUHWND__@@@Z
_TEXT	SEGMENT
_hwnd$ = 8
_this$ = -4
?IsFindDialog@CRichEditBaseCtrl@@IBEHPAUHWND__@@@Z PROC NEAR ; CRichEditBaseCtrl::IsFindDialog, COMDAT

; 867  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 868  : 	if (m_findState.pFindReplaceDlg)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+92], 0
	je	SHORT $L75449

; 869  : 		return (m_findState.pFindReplaceDlg->GetSafeHwnd() == hwnd);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [ecx+92]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	xor	edx, edx
	cmp	eax, DWORD PTR _hwnd$[ebp]
	sete	dl
	mov	eax, edx
	jmp	SHORT $L75448
$L75449:

; 870  : 
; 871  : 	return FALSE;

	xor	eax, eax
$L75448:

; 872  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?IsFindDialog@CRichEditBaseCtrl@@IBEHPAUHWND__@@@Z ENDP	; CRichEditBaseCtrl::IsFindDialog
_TEXT	ENDS
PUBLIC	?SetMargins@CRichEditBaseCtrl@@QAEXPBUtagRECT@@@Z ; CRichEditBaseCtrl::SetMargins
PUBLIC	?SetMargins@CRichEditBaseCtrl@@QAEXHHHH@Z	; CRichEditBaseCtrl::SetMargins
EXTRN	??BCRect@@QAEPAUtagRECT@@XZ:NEAR		; CRect::operator tagRECT *
;	COMDAT ?SetMargins@CRichEditBaseCtrl@@QAEXHHHH@Z
_TEXT	SEGMENT
$T76263 = -16
_nLeft$ = 8
_nTop$ = 12
_nRight$ = 16
_nBottom$ = 20
_this$ = -20
?SetMargins@CRichEditBaseCtrl@@QAEXHHHH@Z PROC NEAR	; CRichEditBaseCtrl::SetMargins, COMDAT

; 875  : {

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

; 876  : 	SetMargins(CRect(nLeft, nTop, nRight, nBottom));

	mov	eax, DWORD PTR _nBottom$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nRight$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nTop$[ebp]
	push	edx
	mov	eax, DWORD PTR _nLeft$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T76263[ebp]
	call	??0CRect@@QAE@HHHH@Z			; CRect::CRect
	mov	ecx, eax
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetMargins@CRichEditBaseCtrl@@QAEXPBUtagRECT@@@Z ; CRichEditBaseCtrl::SetMargins

; 877  : }

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?SetMargins@CRichEditBaseCtrl@@QAEXHHHH@Z ENDP		; CRichEditBaseCtrl::SetMargins
_TEXT	ENDS
PUBLIC	?SetMargins@CRichEditBaseCtrl@@QAEXH@Z		; CRichEditBaseCtrl::SetMargins
;	COMDAT ?SetMargins@CRichEditBaseCtrl@@QAEXH@Z
_TEXT	SEGMENT
$T76266 = -16
_nMargin$ = 8
_this$ = -20
?SetMargins@CRichEditBaseCtrl@@QAEXH@Z PROC NEAR	; CRichEditBaseCtrl::SetMargins, COMDAT

; 880  : {

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

; 881  : 	SetMargins(CRect(nMargin, nMargin, nMargin, nMargin));

	mov	eax, DWORD PTR _nMargin$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nMargin$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nMargin$[ebp]
	push	edx
	mov	eax, DWORD PTR _nMargin$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T76266[ebp]
	call	??0CRect@@QAE@HHHH@Z			; CRect::CRect
	mov	ecx, eax
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetMargins@CRichEditBaseCtrl@@QAEXPBUtagRECT@@@Z ; CRichEditBaseCtrl::SetMargins

; 882  : }

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?SetMargins@CRichEditBaseCtrl@@QAEXH@Z ENDP		; CRichEditBaseCtrl::SetMargins
_TEXT	ENDS
EXTRN	?GetClientRect@CWnd@@QBEXPAUtagRECT@@@Z:NEAR	; CWnd::GetClientRect
EXTRN	?SetRect@CRichEditCtrl@@QAEXPBUtagRECT@@@Z:NEAR	; CRichEditCtrl::SetRect
EXTRN	?SetRectEmpty@CRect@@QAEXXZ:NEAR		; CRect::SetRectEmpty
EXTRN	??4CRect@@QAEXABUtagRECT@@@Z:NEAR		; CRect::operator=
EXTRN	??ZCRect@@QAEXPBUtagRECT@@@Z:NEAR		; CRect::operator-=
;	COMDAT ?SetMargins@CRichEditBaseCtrl@@QAEXPBUtagRECT@@@Z
_TEXT	SEGMENT
_pMargins$ = 8
_this$ = -20
_rClient$ = -16
?SetMargins@CRichEditBaseCtrl@@QAEXPBUtagRECT@@@Z PROC NEAR ; CRichEditBaseCtrl::SetMargins, COMDAT

; 885  : {

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

; 886  : 	if (pMargins == NULL)

	cmp	DWORD PTR _pMargins$[ebp], 0
	jne	SHORT $L75467

; 887  : 		m_rMargins.SetRectEmpty();

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	?SetRectEmpty@CRect@@QAEXXZ		; CRect::SetRectEmpty

; 888  : 	else

	jmp	SHORT $L75468
$L75467:

; 889  : 		m_rMargins = *pMargins;

	mov	eax, DWORD PTR _pMargins$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	??4CRect@@QAEXABUtagRECT@@@Z		; CRect::operator=
$L75468:

; 890  : 
; 891  : 	CRect rClient;

	lea	ecx, DWORD PTR _rClient$[ebp]
	call	??0CRect@@QAE@XZ			; CRect::CRect

; 892  : 	GetClientRect(rClient);

	lea	ecx, DWORD PTR _rClient$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetClientRect@CWnd@@QBEXPAUtagRECT@@@Z	; CWnd::GetClientRect

; 893  : 
; 894  : 	rClient -= m_rMargins;

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	lea	ecx, DWORD PTR _rClient$[ebp]
	call	??ZCRect@@QAEXPBUtagRECT@@@Z		; CRect::operator-=

; 895  : 
; 896  : 	SetRect(rClient);

	lea	ecx, DWORD PTR _rClient$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetRect@CRichEditCtrl@@QAEXPBUtagRECT@@@Z ; CRichEditCtrl::SetRect

; 897  : }

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?SetMargins@CRichEditBaseCtrl@@QAEXPBUtagRECT@@@Z ENDP	; CRichEditBaseCtrl::SetMargins
_TEXT	ENDS
EXTRN	?OnSize@CWnd@@IAEXIHH@Z:NEAR			; CWnd::OnSize
;	COMDAT ?OnSize@CRichEditBaseCtrl@@IAEXIHH@Z
_TEXT	SEGMENT
_nType$ = 8
_cx$ = 12
_cy$ = 16
_this$ = -20
_rClient$ = -16
?OnSize@CRichEditBaseCtrl@@IAEXIHH@Z PROC NEAR		; CRichEditBaseCtrl::OnSize, COMDAT

; 900  : {

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

; 901  : 	CRichEditCtrl::OnSize(nType, cx, cy);

	mov	eax, DWORD PTR _cy$[ebp]
	push	eax
	mov	ecx, DWORD PTR _cx$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nType$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?OnSize@CWnd@@IAEXIHH@Z			; CWnd::OnSize

; 902  : 
; 903  : 	CRect rClient(0, 0, cx, cy);

	mov	eax, DWORD PTR _cy$[ebp]
	push	eax
	mov	ecx, DWORD PTR _cx$[ebp]
	push	ecx
	push	0
	push	0
	lea	ecx, DWORD PTR _rClient$[ebp]
	call	??0CRect@@QAE@HHHH@Z			; CRect::CRect

; 904  : 	rClient -= m_rMargins;

	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 72					; 00000048H
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	lea	ecx, DWORD PTR _rClient$[ebp]
	call	??ZCRect@@QAEXPBUtagRECT@@@Z		; CRect::operator-=

; 905  : 
; 906  : 	SetRect(rClient);

	lea	ecx, DWORD PTR _rClient$[ebp]
	call	??BCRect@@QAEPAUtagRECT@@XZ		; CRect::operator tagRECT *
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetRect@CRichEditCtrl@@QAEXPBUtagRECT@@@Z ; CRichEditCtrl::SetRect

; 907  : }

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?OnSize@CRichEditBaseCtrl@@IAEXIHH@Z ENDP		; CRichEditBaseCtrl::OnSize
_TEXT	ENDS
PUBLIC	?GetSelectionCharFormat@CRichEditBaseCtrl@@QBEKAAUCHARFORMAT2W@@@Z ; CRichEditBaseCtrl::GetSelectionCharFormat
EXTRN	__imp__IsWindow@4:NEAR
;	COMDAT ?GetSelectionCharFormat@CRichEditBaseCtrl@@QBEKAAUCHARFORMAT2W@@@Z
_TEXT	SEGMENT
_cf$ = 8
_this$ = -4
?GetSelectionCharFormat@CRichEditBaseCtrl@@QBEKAAUCHARFORMAT2W@@@Z PROC NEAR ; CRichEditBaseCtrl::GetSelectionCharFormat, COMDAT

; 910  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L75481:

; 911  : 	ASSERT(::IsWindow(m_hWnd));

	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+32]
	push	ecx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75484
	push	911					; 0000038fH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75484
	int	3
$L75484:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75481

; 912  : 	cf.cbSize = sizeof(cf);

	mov	eax, DWORD PTR _cf$[ebp]
	mov	DWORD PTR [eax], 116			; 00000074H

; 913  : 	return (DWORD)::SendMessage(m_hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

	mov	esi, esp
	mov	ecx, DWORD PTR _cf$[ebp]
	push	ecx
	push	1
	push	1082					; 0000043aH
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+32]
	push	eax
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 914  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetSelectionCharFormat@CRichEditBaseCtrl@@QBEKAAUCHARFORMAT2W@@@Z ENDP ; CRichEditBaseCtrl::GetSelectionCharFormat
_TEXT	ENDS
PUBLIC	?GetSelectionCharFormat@CRichEditBaseCtrl@@QBEKAAU_charformat@@@Z ; CRichEditBaseCtrl::GetSelectionCharFormat
;	COMDAT ?GetSelectionCharFormat@CRichEditBaseCtrl@@QBEKAAU_charformat@@@Z
_TEXT	SEGMENT
_cf$ = 8
_this$ = -4
?GetSelectionCharFormat@CRichEditBaseCtrl@@QBEKAAU_charformat@@@Z PROC NEAR ; CRichEditBaseCtrl::GetSelectionCharFormat, COMDAT

; 917  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L75491:

; 918  : 	ASSERT(::IsWindow(m_hWnd));

	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+32]
	push	ecx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75494
	push	918					; 00000396H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75494
	int	3
$L75494:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75491

; 919  : 	cf.cbSize = sizeof(cf);

	mov	eax, DWORD PTR _cf$[ebp]
	mov	DWORD PTR [eax], 60			; 0000003cH

; 920  : 	return (DWORD)::SendMessage(m_hWnd, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

	mov	esi, esp
	mov	ecx, DWORD PTR _cf$[ebp]
	push	ecx
	push	1
	push	1082					; 0000043aH
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+32]
	push	eax
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 921  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetSelectionCharFormat@CRichEditBaseCtrl@@QBEKAAU_charformat@@@Z ENDP ; CRichEditBaseCtrl::GetSelectionCharFormat
_TEXT	ENDS
PUBLIC	?SetSelectionCharFormat@CRichEditBaseCtrl@@QAEHAAUCHARFORMAT2W@@@Z ; CRichEditBaseCtrl::SetSelectionCharFormat
;	COMDAT ?SetSelectionCharFormat@CRichEditBaseCtrl@@QAEHAAUCHARFORMAT2W@@@Z
_TEXT	SEGMENT
_cf$ = 8
_this$ = -4
?SetSelectionCharFormat@CRichEditBaseCtrl@@QAEHAAUCHARFORMAT2W@@@Z PROC NEAR ; CRichEditBaseCtrl::SetSelectionCharFormat, COMDAT

; 924  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L75501:

; 925  : 	ASSERT(::IsWindow(m_hWnd));

	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+32]
	push	ecx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75504
	push	925					; 0000039dH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75504
	int	3
$L75504:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75501

; 926  : 	cf.cbSize = sizeof(cf);

	mov	eax, DWORD PTR _cf$[ebp]
	mov	DWORD PTR [eax], 116			; 00000074H

; 927  : 	return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

	mov	esi, esp
	mov	ecx, DWORD PTR _cf$[ebp]
	push	ecx
	push	1
	push	1092					; 00000444H
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+32]
	push	eax
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 928  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?SetSelectionCharFormat@CRichEditBaseCtrl@@QAEHAAUCHARFORMAT2W@@@Z ENDP ; CRichEditBaseCtrl::SetSelectionCharFormat
_TEXT	ENDS
PUBLIC	?SetSelectionCharFormat@CRichEditBaseCtrl@@QAEHAAU_charformat@@@Z ; CRichEditBaseCtrl::SetSelectionCharFormat
;	COMDAT ?SetSelectionCharFormat@CRichEditBaseCtrl@@QAEHAAU_charformat@@@Z
_TEXT	SEGMENT
_cf$ = 8
_this$ = -4
?SetSelectionCharFormat@CRichEditBaseCtrl@@QAEHAAU_charformat@@@Z PROC NEAR ; CRichEditBaseCtrl::SetSelectionCharFormat, COMDAT

; 931  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L75511:

; 932  : 	ASSERT(::IsWindow(m_hWnd));

	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+32]
	push	ecx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75514
	push	932					; 000003a4H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75514
	int	3
$L75514:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75511

; 933  : 	cf.cbSize = sizeof(cf);

	mov	eax, DWORD PTR _cf$[ebp]
	mov	DWORD PTR [eax], 60			; 0000003cH

; 934  : 	return (BOOL)::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

	mov	esi, esp
	mov	ecx, DWORD PTR _cf$[ebp]
	push	ecx
	push	1
	push	1092					; 00000444H
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+32]
	push	eax
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 935  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?SetSelectionCharFormat@CRichEditBaseCtrl@@QAEHAAU_charformat@@@Z ENDP ; CRichEditBaseCtrl::SetSelectionCharFormat
_TEXT	ENDS
PUBLIC	?SetParaFormat@CRichEditBaseCtrl@@QAEHAAUPARAFORMAT2@@@Z ; CRichEditBaseCtrl::SetParaFormat
EXTRN	?GetDlgCtrlID@CWnd@@QBEHXZ:NEAR			; CWnd::GetDlgCtrlID
EXTRN	?GetParent@CWnd@@QBEPAV1@XZ:NEAR		; CWnd::GetParent
;	COMDAT ?SetParaFormat@CRichEditBaseCtrl@@QAEHAAUPARAFORMAT2@@@Z
_TEXT	SEGMENT
_pf$ = 8
_this$ = -8
_bResult$ = -4
?SetParaFormat@CRichEditBaseCtrl@@QAEHAAUPARAFORMAT2@@@Z PROC NEAR ; CRichEditBaseCtrl::SetParaFormat, COMDAT

; 938  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L75521:

; 939  : 	ASSERT(::IsWindow(m_hWnd));

	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+32]
	push	ecx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75524
	push	939					; 000003abH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75524
	int	3
$L75524:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75521

; 940  : 	pf.cbSize = sizeof(pf);

	mov	eax, DWORD PTR _pf$[ebp]
	mov	DWORD PTR [eax], 188			; 000000bcH

; 941  : 	BOOL bResult = (BOOL)::SendMessage(m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);

	mov	esi, esp
	mov	ecx, DWORD PTR _pf$[ebp]
	push	ecx
	push	0
	push	1095					; 00000447H
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+32]
	push	eax
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _bResult$[ebp], eax

; 942  : 
; 943  : 	// Windows seems not to sent EN_CHANGE when using PARAFORMAT2
; 944  : 	// so we must send it ourselves
; 945  : 	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), EN_CHANGE), (LPARAM)GetSafeHwnd());

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetDlgCtrlID@CWnd@@QBEHXZ		; CWnd::GetDlgCtrlID
	and	eax, 65535				; 0000ffffH
	or	eax, 50331648				; 03000000H
	push	eax
	push	273					; 00000111H
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetParent@CWnd@@QBEPAV1@XZ		; CWnd::GetParent
	mov	ecx, eax
	call	?SendMessageW@CWnd@@QAEJIIJ@Z		; CWnd::SendMessageW

; 946  : 
; 947  : 	return bResult;

	mov	eax, DWORD PTR _bResult$[ebp]

; 948  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?SetParaFormat@CRichEditBaseCtrl@@QAEHAAUPARAFORMAT2@@@Z ENDP ; CRichEditBaseCtrl::SetParaFormat
_TEXT	ENDS
PUBLIC	?SetParaFormat@CRichEditBaseCtrl@@QAEHAAU_paraformat@@@Z ; CRichEditBaseCtrl::SetParaFormat
;	COMDAT ?SetParaFormat@CRichEditBaseCtrl@@QAEHAAU_paraformat@@@Z
_TEXT	SEGMENT
_pf$ = 8
_this$ = -4
?SetParaFormat@CRichEditBaseCtrl@@QAEHAAU_paraformat@@@Z PROC NEAR ; CRichEditBaseCtrl::SetParaFormat, COMDAT

; 951  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L75538:

; 952  : 	ASSERT(::IsWindow(m_hWnd));

	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+32]
	push	ecx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75541
	push	952					; 000003b8H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75541
	int	3
$L75541:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75538

; 953  : 	pf.cbSize = sizeof(pf);

	mov	eax, DWORD PTR _pf$[ebp]
	mov	DWORD PTR [eax], 156			; 0000009cH

; 954  : 	return (BOOL)::SendMessage(m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);

	mov	esi, esp
	mov	ecx, DWORD PTR _pf$[ebp]
	push	ecx
	push	0
	push	1095					; 00000447H
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+32]
	push	eax
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 955  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?SetParaFormat@CRichEditBaseCtrl@@QAEHAAU_paraformat@@@Z ENDP ; CRichEditBaseCtrl::SetParaFormat
_TEXT	ENDS
PUBLIC	?GetParaFormat@CRichEditBaseCtrl@@QBEKAAUPARAFORMAT2@@@Z ; CRichEditBaseCtrl::GetParaFormat
;	COMDAT ?GetParaFormat@CRichEditBaseCtrl@@QBEKAAUPARAFORMAT2@@@Z
_TEXT	SEGMENT
_pf$ = 8
_this$ = -4
?GetParaFormat@CRichEditBaseCtrl@@QBEKAAUPARAFORMAT2@@@Z PROC NEAR ; CRichEditBaseCtrl::GetParaFormat, COMDAT

; 958  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L75548:

; 959  : 	ASSERT(::IsWindow(m_hWnd));

	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+32]
	push	ecx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75551
	push	959					; 000003bfH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75551
	int	3
$L75551:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75548

; 960  : 	pf.cbSize = sizeof(pf);

	mov	eax, DWORD PTR _pf$[ebp]
	mov	DWORD PTR [eax], 188			; 000000bcH

; 961  : 	return (DWORD)::SendMessage(m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf);

	mov	esi, esp
	mov	ecx, DWORD PTR _pf$[ebp]
	push	ecx
	push	0
	push	1085					; 0000043dH
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+32]
	push	eax
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 962  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetParaFormat@CRichEditBaseCtrl@@QBEKAAUPARAFORMAT2@@@Z ENDP ; CRichEditBaseCtrl::GetParaFormat
_TEXT	ENDS
PUBLIC	?GetParaFormat@CRichEditBaseCtrl@@QBEKAAU_paraformat@@@Z ; CRichEditBaseCtrl::GetParaFormat
;	COMDAT ?GetParaFormat@CRichEditBaseCtrl@@QBEKAAU_paraformat@@@Z
_TEXT	SEGMENT
_pf$ = 8
_this$ = -4
?GetParaFormat@CRichEditBaseCtrl@@QBEKAAU_paraformat@@@Z PROC NEAR ; CRichEditBaseCtrl::GetParaFormat, COMDAT

; 965  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L75558:

; 966  : 	ASSERT(::IsWindow(m_hWnd));

	mov	esi, esp
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+32]
	push	ecx
	call	DWORD PTR __imp__IsWindow@4
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L75561
	push	966					; 000003c6H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75561
	int	3
$L75561:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L75558

; 967  : 	pf.cbSize = sizeof(pf);

	mov	eax, DWORD PTR _pf$[ebp]
	mov	DWORD PTR [eax], 156			; 0000009cH

; 968  : 	return (DWORD)::SendMessage(m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf);

	mov	esi, esp
	mov	ecx, DWORD PTR _pf$[ebp]
	push	ecx
	push	0
	push	1085					; 0000043dH
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+32]
	push	eax
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 969  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetParaFormat@CRichEditBaseCtrl@@QBEKAAU_paraformat@@@Z ENDP ; CRichEditBaseCtrl::GetParaFormat
_TEXT	ENDS
PUBLIC	?SelectionHasEffect@CRichEditBaseCtrl@@QBEHKK@Z	; CRichEditBaseCtrl::SelectionHasEffect
;	COMDAT ?SelectionHasEffect@CRichEditBaseCtrl@@QBEHKK@Z
_TEXT	SEGMENT
_dwMask$ = 8
_dwEffect$ = 12
_this$ = -120
_cf$ = -116
?SelectionHasEffect@CRichEditBaseCtrl@@QBEHKK@Z PROC NEAR ; CRichEditBaseCtrl::SelectionHasEffect, COMDAT

; 972  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 120				; 00000078H
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-120]
	mov	ecx, 30					; 0000001eH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 973  : 	CHARFORMAT2 cf;
; 974  : 	ZeroMemory(&cf, sizeof(cf));

	push	116					; 00000074H
	push	0
	lea	eax, DWORD PTR _cf$[ebp]
	push	eax
	call	_memset
	add	esp, 12					; 0000000cH

; 975  : 	
; 976  : 	cf.cbSize = sizeof(CHARFORMAT);

	mov	DWORD PTR _cf$[ebp], 60			; 0000003cH

; 977  : 	cf.dwMask = dwMask;

	mov	ecx, DWORD PTR _dwMask$[ebp]
	mov	DWORD PTR _cf$[ebp+4], ecx

; 978  : 
; 979  : 	if (GetSelectionCharFormat(cf))

	lea	edx, DWORD PTR _cf$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSelectionCharFormat@CRichEditBaseCtrl@@QBEKAAUCHARFORMAT2W@@@Z ; CRichEditBaseCtrl::GetSelectionCharFormat
	test	eax, eax
	je	SHORT $L75571

; 980  : 		return (cf.dwEffects & dwEffect);

	mov	eax, DWORD PTR _cf$[ebp+8]
	and	eax, DWORD PTR _dwEffect$[ebp]
	jmp	SHORT $L75568
$L75571:

; 981  : 
; 982  : 	// else
; 983  : 	return FALSE;

	xor	eax, eax
$L75568:

; 984  : }

	pop	edi
	add	esp, 120				; 00000078H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?SelectionHasEffect@CRichEditBaseCtrl@@QBEHKK@Z ENDP	; CRichEditBaseCtrl::SelectionHasEffect
_TEXT	ENDS
PUBLIC	?CanEdit@CRichEditBaseCtrl@@QBEHXZ		; CRichEditBaseCtrl::CanEdit
EXTRN	?IsWindowEnabled@CWnd@@QBEHXZ:NEAR		; CWnd::IsWindowEnabled
EXTRN	?GetStyle@CWnd@@QBEKXZ:NEAR			; CWnd::GetStyle
;	COMDAT ?CanEdit@CRichEditBaseCtrl@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?CanEdit@CRichEditBaseCtrl@@QBEHXZ PROC NEAR		; CRichEditBaseCtrl::CanEdit, COMDAT

; 987  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 988  : 	return (IsWindowEnabled() && !(GetStyle() & ES_READONLY));

	mov	ecx, DWORD PTR _this$[ebp]
	call	?IsWindowEnabled@CWnd@@QBEHXZ		; CWnd::IsWindowEnabled
	test	eax, eax
	je	SHORT $L76291
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetStyle@CWnd@@QBEKXZ			; CWnd::GetStyle
	and	eax, 2048				; 00000800H
	test	eax, eax
	jne	SHORT $L76291
	mov	DWORD PTR -8+[ebp], 1
	jmp	SHORT $L76292
$L76291:
	mov	DWORD PTR -8+[ebp], 0
$L76292:
	mov	eax, DWORD PTR -8+[ebp]

; 989  : }

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?CanEdit@CRichEditBaseCtrl@@QBEHXZ ENDP			; CRichEditBaseCtrl::CanEdit
_TEXT	ENDS
PUBLIC	?HasSelection@CRichEditBaseCtrl@@QBEHXZ		; CRichEditBaseCtrl::HasSelection
;	COMDAT ?HasSelection@CRichEditBaseCtrl@@QBEHXZ
_TEXT	SEGMENT
_this$ = -12
_cr$ = -8
?HasSelection@CRichEditBaseCtrl@@QBEHXZ PROC NEAR	; CRichEditBaseCtrl::HasSelection, COMDAT

; 992  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 993  : 	CHARRANGE cr = { 0 };

	mov	DWORD PTR _cr$[ebp], 0
	xor	eax, eax
	mov	DWORD PTR _cr$[ebp+4], eax

; 994  : 	GetSel(cr);

	lea	ecx, DWORD PTR _cr$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSel@CRichEditCtrl@@QBEXAAU_charrange@@@Z ; CRichEditCtrl::GetSel

; 995  : 				
; 996  : 	return ((cr.cpMax - cr.cpMin) != 0);

	mov	eax, DWORD PTR _cr$[ebp+4]
	sub	eax, DWORD PTR _cr$[ebp]
	neg	eax
	sbb	eax, eax
	neg	eax

; 997  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?HasSelection@CRichEditBaseCtrl@@QBEHXZ ENDP		; CRichEditBaseCtrl::HasSelection
_TEXT	ENDS
PUBLIC	?SetFirstVisibleLine@CRichEditBaseCtrl@@QAEXH@Z	; CRichEditBaseCtrl::SetFirstVisibleLine
EXTRN	?GetFirstVisibleLine@CRichEditCtrl@@QBEHXZ:NEAR	; CRichEditCtrl::GetFirstVisibleLine
EXTRN	?LineScroll@CRichEditCtrl@@QAEXHH@Z:NEAR	; CRichEditCtrl::LineScroll
;	COMDAT ?SetFirstVisibleLine@CRichEditBaseCtrl@@QAEXH@Z
_TEXT	SEGMENT
_nLine$ = 8
_this$ = -16
_nFirst$ = -4
_nPrevFirst$75585 = -8
_nPrevFirst$75592 = -12
?SetFirstVisibleLine@CRichEditBaseCtrl@@QAEXH@Z PROC NEAR ; CRichEditBaseCtrl::SetFirstVisibleLine, COMDAT

; 1000 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax
	mov	DWORD PTR _this$[ebp], ecx

; 1001 : 	int nFirst = GetFirstVisibleLine();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetFirstVisibleLine@CRichEditCtrl@@QBEHXZ ; CRichEditCtrl::GetFirstVisibleLine
	mov	DWORD PTR _nFirst$[ebp], eax

; 1002 : 	
; 1003 : 	if (nLine < nFirst)

	mov	eax, DWORD PTR _nLine$[ebp]
	cmp	eax, DWORD PTR _nFirst$[ebp]
	jge	SHORT $L75584
$L75586:

; 1009 : 			LineScroll(-1);

	push	0
	push	-1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LineScroll@CRichEditCtrl@@QAEXHH@Z	; CRichEditCtrl::LineScroll

; 1010 : 			
; 1011 : 			nPrevFirst = nFirst;

	mov	ecx, DWORD PTR _nFirst$[ebp]
	mov	DWORD PTR _nPrevFirst$75585[ebp], ecx

; 1012 : 			nFirst = GetFirstVisibleLine();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetFirstVisibleLine@CRichEditCtrl@@QBEHXZ ; CRichEditCtrl::GetFirstVisibleLine
	mov	DWORD PTR _nFirst$[ebp], eax

; 1014 : 		while  (nLine < nFirst && (nFirst != nPrevFirst));

	mov	edx, DWORD PTR _nLine$[ebp]
	cmp	edx, DWORD PTR _nFirst$[ebp]
	jge	SHORT $L75589
	mov	eax, DWORD PTR _nFirst$[ebp]
	cmp	eax, DWORD PTR _nPrevFirst$75585[ebp]
	jne	SHORT $L75586
$L75589:

; 1016 : 	else if (nLine > nFirst)

	jmp	SHORT $L75596
$L75584:
	mov	ecx, DWORD PTR _nLine$[ebp]
	cmp	ecx, DWORD PTR _nFirst$[ebp]
	jle	SHORT $L75596
$L75593:

; 1022 : 			LineScroll(1);

	push	0
	push	1
	mov	ecx, DWORD PTR _this$[ebp]
	call	?LineScroll@CRichEditCtrl@@QAEXHH@Z	; CRichEditCtrl::LineScroll

; 1023 : 			
; 1024 : 			nPrevFirst = nFirst;

	mov	edx, DWORD PTR _nFirst$[ebp]
	mov	DWORD PTR _nPrevFirst$75592[ebp], edx

; 1025 : 			nFirst = GetFirstVisibleLine();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetFirstVisibleLine@CRichEditCtrl@@QBEHXZ ; CRichEditCtrl::GetFirstVisibleLine
	mov	DWORD PTR _nFirst$[ebp], eax

; 1027 : 		while  (nLine > nFirst && (nFirst != nPrevFirst));

	mov	eax, DWORD PTR _nLine$[ebp]
	cmp	eax, DWORD PTR _nFirst$[ebp]
	jle	SHORT $L75596
	mov	ecx, DWORD PTR _nFirst$[ebp]
	cmp	ecx, DWORD PTR _nPrevFirst$75592[ebp]
	jne	SHORT $L75593
$L75596:

; 1029 : }

	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?SetFirstVisibleLine@CRichEditBaseCtrl@@QAEXH@Z ENDP	; CRichEditBaseCtrl::SetFirstVisibleLine
_TEXT	ENDS
PUBLIC	?GetLineHeight@CRichEditBaseCtrl@@QBEHXZ	; CRichEditBaseCtrl::GetLineHeight
EXTRN	?GetDefaultCharFormat@CRichEditCtrl@@QBEKAAU_charformat@@@Z:NEAR ; CRichEditCtrl::GetDefaultCharFormat
EXTRN	__imp__GetDC@4:NEAR
EXTRN	__imp__ReleaseDC@8:NEAR
EXTRN	__imp__GetDeviceCaps@8:NEAR
;	COMDAT ?GetLineHeight@CRichEditBaseCtrl@@QBEHXZ
_TEXT	SEGMENT
_this$ = -76
_cf$ = -60
_nTwipsPerInch$ = -72
_hdc$ = -68
_nPixelsPerInch$ = -64
?GetLineHeight@CRichEditBaseCtrl@@QBEHXZ PROC NEAR	; CRichEditBaseCtrl::GetLineHeight, COMDAT

; 1032 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 76					; 0000004cH
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-76]
	mov	ecx, 19					; 00000013H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 1033 : 	CHARFORMAT cf;
; 1034 : 	GetDefaultCharFormat(cf);

	lea	eax, DWORD PTR _cf$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetDefaultCharFormat@CRichEditCtrl@@QBEKAAU_charformat@@@Z ; CRichEditCtrl::GetDefaultCharFormat

; 1035 : 	
; 1036 : 	// convert height in Twips to pixels
; 1037 : 	int nTwipsPerInch = 1440;

	mov	DWORD PTR _nTwipsPerInch$[ebp], 1440	; 000005a0H

; 1038 : 	HDC hdc = ::GetDC(NULL);

	mov	esi, esp
	push	0
	call	DWORD PTR __imp__GetDC@4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _hdc$[ebp], eax

; 1039 :     int nPixelsPerInch = GetDeviceCaps(hdc, LOGPIXELSY);

	mov	esi, esp
	push	90					; 0000005aH
	mov	ecx, DWORD PTR _hdc$[ebp]
	push	ecx
	call	DWORD PTR __imp__GetDeviceCaps@8
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nPixelsPerInch$[ebp], eax

; 1040 : 	::ReleaseDC(NULL, hdc);

	mov	esi, esp
	mov	edx, DWORD PTR _hdc$[ebp]
	push	edx
	push	0
	call	DWORD PTR __imp__ReleaseDC@8
	cmp	esi, esp
	call	__chkesp

; 1041 : 	
; 1042 : 	return (cf.yHeight * nPixelsPerInch) / nTwipsPerInch + 2;

	mov	eax, DWORD PTR _cf$[ebp+12]
	imul	eax, DWORD PTR _nPixelsPerInch$[ebp]
	cdq
	idiv	DWORD PTR _nTwipsPerInch$[ebp]
	add	eax, 2

; 1043 : }

	pop	edi
	pop	esi
	add	esp, 76					; 0000004cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetLineHeight@CRichEditBaseCtrl@@QBEHXZ ENDP		; CRichEditBaseCtrl::GetLineHeight
_TEXT	ENDS
PUBLIC	?GetCaretPos@CRichEditBaseCtrl@@QBE?AVCPoint@@XZ ; CRichEditBaseCtrl::GetCaretPos
EXTRN	__imp__GetFocus@0:NEAR
EXTRN	?GetCaretPos@CWnd@@SG?AVCPoint@@XZ:NEAR		; CWnd::GetCaretPos
EXTRN	??0CPoint@@QAE@HH@Z:NEAR			; CPoint::CPoint
;	COMDAT ?GetCaretPos@CRichEditBaseCtrl@@QBE?AVCPoint@@XZ
_TEXT	SEGMENT
___$ReturnUdt$ = 8
_this$ = -12
_ptCaret$ = -8
?GetCaretPos@CRichEditBaseCtrl@@QBE?AVCPoint@@XZ PROC NEAR ; CRichEditBaseCtrl::GetCaretPos, COMDAT

; 1046 : {

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

; 1047 : 	if (::GetFocus() != GetSafeHwnd())

	mov	esi, esp
	call	DWORD PTR __imp__GetFocus@0
	cmp	esi, esp
	call	__chkesp
	mov	esi, eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	cmp	esi, eax
	je	SHORT $L75608
$L75609:

; 1049 : 		ASSERT (0);

	mov	eax, 1
	test	eax, eax
	je	SHORT $L75612
	push	1049					; 00000419H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L75612
	int	3
$L75612:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L75609

; 1050 : 		return CPoint(0, 0);

	push	0
	push	0
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CPoint@@QAE@HH@Z			; CPoint::CPoint
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	jmp	SHORT $L75607
$L75608:

; 1052 : 	
; 1053 : 	CPoint ptCaret = CWnd::GetCaretPos();

	lea	edx, DWORD PTR _ptCaret$[ebp]
	push	edx
	call	?GetCaretPos@CWnd@@SG?AVCPoint@@XZ	; CWnd::GetCaretPos

; 1054 : 	ptCaret.y += GetLineHeight() / 2; // half line height

	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetLineHeight@CRichEditBaseCtrl@@QBEHXZ ; CRichEditBaseCtrl::GetLineHeight
	cdq
	sub	eax, edx
	sar	eax, 1
	mov	ecx, DWORD PTR _ptCaret$[ebp+4]
	add	ecx, eax
	mov	DWORD PTR _ptCaret$[ebp+4], ecx

; 1055 : 	ptCaret.x += 4; // estaimate 1/2 char width

	mov	edx, DWORD PTR _ptCaret$[ebp]
	add	edx, 4
	mov	DWORD PTR _ptCaret$[ebp], edx

; 1056 : 	
; 1057 : 	return ptCaret;

	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
	mov	ecx, DWORD PTR _ptCaret$[ebp]
	mov	DWORD PTR [eax], ecx
	mov	edx, DWORD PTR _ptCaret$[ebp+4]
	mov	DWORD PTR [eax+4], edx
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]
$L75607:

; 1058 : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?GetCaretPos@CRichEditBaseCtrl@@QBE?AVCPoint@@XZ ENDP	; CRichEditBaseCtrl::GetCaretPos
_TEXT	ENDS
PUBLIC	?CharFromPoint@CRichEditBaseCtrl@@QBEHABVCPoint@@@Z ; CRichEditBaseCtrl::CharFromPoint
;	COMDAT ?CharFromPoint@CRichEditBaseCtrl@@QBEHABVCPoint@@@Z
_TEXT	SEGMENT
_point$ = 8
_this$ = -12
_ptl$ = -8
?CharFromPoint@CRichEditBaseCtrl@@QBEHABVCPoint@@@Z PROC NEAR ; CRichEditBaseCtrl::CharFromPoint, COMDAT

; 1061 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1062 : 	POINTL ptl = { point.x, point.y };

	mov	eax, DWORD PTR _point$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _ptl$[ebp], ecx
	mov	edx, DWORD PTR _point$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR _ptl$[ebp+4], eax

; 1063 : 	
; 1064 : 	return ::SendMessage(GetSafeHwnd(), EM_CHARFROMPOS, 0, (LPARAM)&ptl);

	mov	esi, esp
	lea	ecx, DWORD PTR _ptl$[ebp]
	push	ecx
	push	0
	push	215					; 000000d7H
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetSafeHwnd@CWnd@@QBEPAUHWND__@@XZ	; CWnd::GetSafeHwnd
	push	eax
	call	DWORD PTR __imp__SendMessageW@16
	cmp	esi, esp
	call	__chkesp

; 1065 : }

	pop	esi
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?CharFromPoint@CRichEditBaseCtrl@@QBEHABVCPoint@@@Z ENDP ; CRichEditBaseCtrl::CharFromPoint
_TEXT	ENDS
PUBLIC	?StreamOutCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z	; CRichEditBaseCtrl::StreamOutCB
;	COMDAT ?StreamOutCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z
_TEXT	SEGMENT
_dwCookie$ = 8
_pbBuff$ = 12
_cb$ = 16
_pcb$ = 20
_pFile$ = -4
?StreamOutCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z PROC NEAR	; CRichEditBaseCtrl::StreamOutCB, COMDAT

; 1072 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1073 : 	CMemFile* pFile = (CMemFile*)dwCookie;

	mov	eax, DWORD PTR _dwCookie$[ebp]
	mov	DWORD PTR _pFile$[ebp], eax

; 1074 : 	
; 1075 : 	pFile->Write(pbBuff, cb);

	mov	esi, esp
	mov	ecx, DWORD PTR _cb$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pbBuff$[ebp]
	push	edx
	mov	eax, DWORD PTR _pFile$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _pFile$[ebp]
	call	DWORD PTR [edx+64]
	cmp	esi, esp
	call	__chkesp

; 1076 : 	*pcb = cb;

	mov	eax, DWORD PTR _pcb$[ebp]
	mov	ecx, DWORD PTR _cb$[ebp]
	mov	DWORD PTR [eax], ecx

; 1077 : 	
; 1078 : 	return 0;

	xor	eax, eax

; 1079 : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?StreamOutCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z ENDP	; CRichEditBaseCtrl::StreamOutCB
_TEXT	ENDS
PUBLIC	?StreamOutLenCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z ; CRichEditBaseCtrl::StreamOutLenCB
;	COMDAT ?StreamOutLenCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z
_TEXT	SEGMENT
_dwCookie$ = 8
_cb$ = 16
_pcb$ = 20
_pByteLen$ = -4
?StreamOutLenCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z PROC NEAR ; CRichEditBaseCtrl::StreamOutLenCB, COMDAT

; 1082 : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1083 : 	int* pByteLen = (int*)dwCookie;

	mov	eax, DWORD PTR _dwCookie$[ebp]
	mov	DWORD PTR _pByteLen$[ebp], eax

; 1084 : 	
; 1085 : 	*pByteLen += cb;

	mov	ecx, DWORD PTR _pByteLen$[ebp]
	mov	edx, DWORD PTR [ecx]
	add	edx, DWORD PTR _cb$[ebp]
	mov	eax, DWORD PTR _pByteLen$[ebp]
	mov	DWORD PTR [eax], edx

; 1086 : 	*pcb = cb;

	mov	ecx, DWORD PTR _pcb$[ebp]
	mov	edx, DWORD PTR _cb$[ebp]
	mov	DWORD PTR [ecx], edx

; 1087 : 	
; 1088 : 	return 0;

	xor	eax, eax

; 1089 : }

	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?StreamOutLenCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z ENDP	; CRichEditBaseCtrl::StreamOutLenCB
_TEXT	ENDS
PUBLIC	?GetLength@STREAMINCOOKIE@@QBEHXZ		; STREAMINCOOKIE::GetLength
PUBLIC	?CopyFrom@STREAMINCOOKIE@@QBEPBDXZ		; STREAMINCOOKIE::CopyFrom
PUBLIC	?StreamInCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z	; CRichEditBaseCtrl::StreamInCB
EXTRN	__imp__strncpy:NEAR
;	COMDAT ?StreamInCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z
_TEXT	SEGMENT
_dwCookie$ = 8
_pbBuff$ = 12
_cb$ = 16
_pcb$ = 20
_pCookie$ = -8
_nBytes$ = -4
?StreamInCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z PROC NEAR	; CRichEditBaseCtrl::StreamInCB, COMDAT

; 1092 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 1093 : 	STREAMINCOOKIE* pCookie = (STREAMINCOOKIE*)dwCookie;

	mov	eax, DWORD PTR _dwCookie$[ebp]
	mov	DWORD PTR _pCookie$[ebp], eax

; 1094 : 	int nBytes = min(cb, pCookie->GetLength());

	mov	ecx, DWORD PTR _pCookie$[ebp]
	call	?GetLength@STREAMINCOOKIE@@QBEHXZ	; STREAMINCOOKIE::GetLength
	cmp	DWORD PTR _cb$[ebp], eax
	jge	SHORT $L76309
	mov	ecx, DWORD PTR _cb$[ebp]
	mov	DWORD PTR -12+[ebp], ecx
	jmp	SHORT $L76310
$L76309:
	mov	ecx, DWORD PTR _pCookie$[ebp]
	call	?GetLength@STREAMINCOOKIE@@QBEHXZ	; STREAMINCOOKIE::GetLength
	mov	DWORD PTR -12+[ebp], eax
$L76310:
	mov	edx, DWORD PTR -12+[ebp]
	mov	DWORD PTR _nBytes$[ebp], edx

; 1095 : 	
; 1096 : #if _MSC_VER >= 1400
; 1097 : 	strncpy_s((char*)pbBuff, cb, pCookie->CopyFrom(), nBytes);
; 1098 : #else
; 1099 : 	strncpy((char*)pbBuff, pCookie->CopyFrom(), nBytes);

	mov	esi, esp
	mov	eax, DWORD PTR _nBytes$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pCookie$[ebp]
	call	?CopyFrom@STREAMINCOOKIE@@QBEPBDXZ	; STREAMINCOOKIE::CopyFrom
	push	eax
	mov	ecx, DWORD PTR _pbBuff$[ebp]
	push	ecx
	call	DWORD PTR __imp__strncpy
	add	esp, 12					; 0000000cH
	cmp	esi, esp
	call	__chkesp

; 1100 : #endif
; 1101 : 	
; 1102 : 	*pcb = nBytes;

	mov	edx, DWORD PTR _pcb$[ebp]
	mov	eax, DWORD PTR _nBytes$[ebp]
	mov	DWORD PTR [edx], eax

; 1103 : 	pCookie->nStreamPos += nBytes;

	mov	ecx, DWORD PTR _pCookie$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	add	edx, DWORD PTR _nBytes$[ebp]
	mov	eax, DWORD PTR _pCookie$[ebp]
	mov	DWORD PTR [eax+4], edx

; 1104 : 	
; 1105 : 	return 0;

	xor	eax, eax

; 1106 : }

	pop	esi
	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?StreamInCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z ENDP	; CRichEditBaseCtrl::StreamInCB
_TEXT	ENDS
;	COMDAT ?GetLength@STREAMINCOOKIE@@QBEHXZ
_TEXT	SEGMENT
_this$ = -4
?GetLength@STREAMINCOOKIE@@QBEHXZ PROC NEAR		; STREAMINCOOKIE::GetLength, COMDAT

; 50   : 	{ 

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 51   : 		return (sRTF.GetLength() * sizeof(TCHAR)); 

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	shl	eax, 1

; 52   : 	}

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetLength@STREAMINCOOKIE@@QBEHXZ ENDP			; STREAMINCOOKIE::GetLength
_TEXT	ENDS
;	COMDAT ?CopyFrom@STREAMINCOOKIE@@QBEPBDXZ
_TEXT	SEGMENT
_this$ = -4
?CopyFrom@STREAMINCOOKIE@@QBEPBDXZ PROC NEAR		; STREAMINCOOKIE::CopyFrom, COMDAT

; 55   : 	{ 

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 56   : 		return (((LPCSTR)(LPCTSTR)sRTF) + nStreamPos); 

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	mov	ecx, DWORD PTR _this$[ebp]
	add	eax, DWORD PTR [ecx+4]

; 57   : 	}

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?CopyFrom@STREAMINCOOKIE@@QBEPBDXZ ENDP			; STREAMINCOOKIE::CopyFrom
_TEXT	ENDS
PUBLIC	?Save@CRichEditBaseCtrl@@QAEHAAVCString@@@Z	; CRichEditBaseCtrl::Save
PUBLIC	?GetRTF@CRichEditBaseCtrl@@QBE?AVCString@@XZ	; CRichEditBaseCtrl::GetRTF
EXTRN	?SaveFile@FileMisc@@YAHPBGPBD@Z:NEAR		; FileMisc::SaveFile
;	COMDAT xdata$x
; File D:\_CODE\Shared\richeditbasectrl.cpp
xdata$x	SEGMENT
$T76320	DD	019930520H
	DD	01H
	DD	FLAT:$T76322
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76322	DD	0ffffffffH
	DD	FLAT:$L76318
xdata$x	ENDS
;	COMDAT ?Save@CRichEditBaseCtrl@@QAEHAAVCString@@@Z
_TEXT	SEGMENT
$T76317 = -20
__$EHRec$ = -12
_filename$ = 8
_this$ = -24
_str$ = -16
?Save@CRichEditBaseCtrl@@QAEHAAVCString@@@Z PROC NEAR	; CRichEditBaseCtrl::Save, COMDAT

; 1109 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76321
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-16], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1110 : 	CString str = GetRTF(); // returns multibyte encoded string

	lea	eax, DWORD PTR _str$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?GetRTF@CRichEditBaseCtrl@@QBE?AVCString@@XZ ; CRichEditBaseCtrl::GetRTF
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1111 : 	
; 1112 : 	// save as multibyte
; 1113 : 	return FileMisc::SaveFile(filename, (LPCSTR)(LPCTSTR)str);

	lea	ecx, DWORD PTR _str$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	mov	ecx, DWORD PTR _filename$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?SaveFile@FileMisc@@YAHPBGPBD@Z		; FileMisc::SaveFile
	add	esp, 8
	mov	DWORD PTR $T76317[ebp], eax
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _str$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76317[ebp]

; 1114 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76318:
	lea	ecx, DWORD PTR _str$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76321:
	mov	eax, OFFSET FLAT:$T76320
	jmp	___CxxFrameHandler
text$x	ENDS
?Save@CRichEditBaseCtrl@@QAEHAAVCString@@@Z ENDP	; CRichEditBaseCtrl::Save
PUBLIC	?Load@CRichEditBaseCtrl@@QAEHAAVCString@@@Z	; CRichEditBaseCtrl::Load
PUBLIC	?SetRTF@CRichEditBaseCtrl@@QAEXABVCString@@@Z	; CRichEditBaseCtrl::SetRTF
EXTRN	?LoadFile@FileMisc@@YAHPBGAAVCString@@H@Z:NEAR	; FileMisc::LoadFile
EXTRN	?EncodeAsMultiByte@Misc@@YAAAVCString@@AAV2@I@Z:NEAR ; Misc::EncodeAsMultiByte
;	COMDAT xdata$x
; File D:\_CODE\Shared\richeditbasectrl.cpp
xdata$x	SEGMENT
$T76331	DD	019930520H
	DD	01H
	DD	FLAT:$T76333
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76333	DD	0ffffffffH
	DD	FLAT:$L76329
xdata$x	ENDS
;	COMDAT ?Load@CRichEditBaseCtrl@@QAEHAAVCString@@@Z
_TEXT	SEGMENT
$T76327 = -20
$T76328 = -24
__$EHRec$ = -12
_filename$ = 8
_this$ = -28
_str$ = -16
?Load@CRichEditBaseCtrl@@QAEHAAVCString@@@Z PROC NEAR	; CRichEditBaseCtrl::Load, COMDAT

; 1117 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76332
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

; 1118 : 	CString str;

	lea	ecx, DWORD PTR _str$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1119 : 	
; 1120 : 	if (!FileMisc::LoadFile(filename, str))

	push	0
	lea	eax, DWORD PTR _str$[ebp]
	push	eax
	mov	ecx, DWORD PTR _filename$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	call	?LoadFile@FileMisc@@YAHPBGAAVCString@@H@Z ; FileMisc::LoadFile
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	SHORT $L75662

; 1121 : 		return FALSE;

	mov	DWORD PTR $T76327[ebp], 0
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _str$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76327[ebp]
	jmp	SHORT $L75660
$L75662:

; 1122 : 	
; 1123 : 	if (!str.IsEmpty())

	lea	ecx, DWORD PTR _str$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	jne	SHORT $L75664

; 1125 : #ifdef _UNICODE
; 1126 : 		// str is unicode but RTF must be multibyte
; 1127 : 		Misc::EncodeAsMultiByte(str);

	push	0
	lea	ecx, DWORD PTR _str$[ebp]
	push	ecx
	call	?EncodeAsMultiByte@Misc@@YAAAVCString@@AAV2@I@Z ; Misc::EncodeAsMultiByte
	add	esp, 8

; 1128 : #endif
; 1129 : 		
; 1130 : 		SetRTF(str);

	lea	edx, DWORD PTR _str$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?SetRTF@CRichEditBaseCtrl@@QAEXABVCString@@@Z ; CRichEditBaseCtrl::SetRTF
$L75664:

; 1132 : 	
; 1133 : 	return TRUE;

	mov	DWORD PTR $T76328[ebp], 1
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _str$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	eax, DWORD PTR $T76328[ebp]
$L75660:

; 1134 : }

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
$L76329:
	lea	ecx, DWORD PTR _str$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76332:
	mov	eax, OFFSET FLAT:$T76331
	jmp	___CxxFrameHandler
text$x	ENDS
?Load@CRichEditBaseCtrl@@QAEHAAVCString@@@Z ENDP	; CRichEditBaseCtrl::Load
EXTRN	?StreamOut@CRichEditCtrl@@QAEJHAAU_editstream@@@Z:NEAR ; CRichEditCtrl::StreamOut
EXTRN	?SeekToBegin@CFile@@QAEXXZ:NEAR			; CFile::SeekToBegin
EXTRN	?GetLength@CFile@@UBEKXZ:NEAR			; CFile::GetLength
EXTRN	??0CMemFile@@QAE@I@Z:NEAR			; CMemFile::CMemFile
EXTRN	??1CMemFile@@UAE@XZ:NEAR			; CMemFile::~CMemFile
EXTRN	?Read@CMemFile@@UAEIPAXI@Z:NEAR			; CMemFile::Read
EXTRN	?GetBuffer@CString@@QAEPAGH@Z:NEAR		; CString::GetBuffer
EXTRN	?ReleaseBuffer@CString@@QAEXH@Z:NEAR		; CString::ReleaseBuffer
;	COMDAT xdata$x
; File D:\_CODE\Shared\richeditbasectrl.cpp
xdata$x	SEGMENT
$T76344	DD	019930520H
	DD	03H
	DD	FLAT:$T76346
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76346	DD	0ffffffffH
	DD	FLAT:$L76340
	DD	00H
	DD	FLAT:$L76338
	DD	01H
	DD	FLAT:$L76339
xdata$x	ENDS
;	COMDAT ?GetRTF@CRichEditBaseCtrl@@QBE?AVCString@@XZ
_TEXT	SEGMENT
$T76341 = -80
__$EHRec$ = -12
___$ReturnUdt$ = 8
_this$ = -84
_file$ = -76
_es$ = -32
_sRTF$ = -36
_nLen$ = -20
_szRTF$ = -16
?GetRTF@CRichEditBaseCtrl@@QBE?AVCString@@XZ PROC NEAR	; CRichEditBaseCtrl::GetRTF, COMDAT

; 1137 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76345
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 72					; 00000048H
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-84]
	mov	ecx, 18					; 00000012H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T76341[ebp], 0

; 1138 : 	// stream to mem file in big chunks
; 1139 : 	// note: we use a BIG file because it gives us excellent
; 1140 : 	// performance on content containing images
; 1141 : 	CMemFile file(2024 * 1024);

	push	2072576					; 001fa000H
	lea	ecx, DWORD PTR _file$[ebp]
	call	??0CMemFile@@QAE@I@Z			; CMemFile::CMemFile
	mov	DWORD PTR __$EHRec$[ebp+8], 1

; 1142 : 	EDITSTREAM es = { (DWORD)&file, 0, StreamOutCB };

	lea	eax, DWORD PTR _file$[ebp]
	mov	DWORD PTR _es$[ebp], eax
	mov	DWORD PTR _es$[ebp+4], 0
	mov	DWORD PTR _es$[ebp+8], OFFSET FLAT:?StreamOutCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z ; CRichEditBaseCtrl::StreamOutCB
	mov	ecx, DWORD PTR _es$[ebp+8]
	mov	DWORD PTR _es$[ebp+8], ecx

; 1143 : 
; 1144 : 	const_cast<CRichEditBaseCtrl*>(this)->StreamOut(SF_RTF, es);

	lea	edx, DWORD PTR _es$[ebp]
	push	edx
	push	2
	mov	ecx, DWORD PTR _this$[ebp]
	call	?StreamOut@CRichEditCtrl@@QAEJHAAU_editstream@@@Z ; CRichEditCtrl::StreamOut

; 1145 : 	
; 1146 : 	// then copy to string
; 1147 : 	CString sRTF;

	lea	ecx, DWORD PTR _sRTF$[ebp]
	call	??0CString@@QAE@XZ			; CString::CString
	mov	BYTE PTR __$EHRec$[ebp+8], 2

; 1148 : 	int nLen = (int)file.GetLength();

	lea	ecx, DWORD PTR _file$[ebp]
	call	?GetLength@CFile@@UBEKXZ		; CFile::GetLength
	mov	DWORD PTR _nLen$[ebp], eax

; 1149 : 	
; 1150 : 	LPTSTR szRTF = sRTF.GetBuffer(nLen);

	mov	eax, DWORD PTR _nLen$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sRTF$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	mov	DWORD PTR _szRTF$[ebp], eax

; 1151 : 	
; 1152 : 	file.SeekToBegin();

	lea	ecx, DWORD PTR _file$[ebp]
	call	?SeekToBegin@CFile@@QAEXXZ		; CFile::SeekToBegin

; 1153 : 	file.Read((void*)szRTF, nLen);

	mov	ecx, DWORD PTR _nLen$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szRTF$[ebp]
	push	edx
	lea	ecx, DWORD PTR _file$[ebp]
	call	?Read@CMemFile@@UAEIPAXI@Z		; CMemFile::Read

; 1154 : 	
; 1155 : 	sRTF.ReleaseBuffer(nLen);

	mov	eax, DWORD PTR _nLen$[ebp]
	push	eax
	lea	ecx, DWORD PTR _sRTF$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 1156 : 	return sRTF;

	lea	ecx, DWORD PTR _sRTF$[ebp]
	push	ecx
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	edx, DWORD PTR $T76341[ebp]
	or	edx, 1
	mov	DWORD PTR $T76341[ebp], edx
	mov	BYTE PTR __$EHRec$[ebp+8], 1
	lea	ecx, DWORD PTR _sRTF$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CMemFile@@UAE@XZ			; CMemFile::~CMemFile
	mov	eax, DWORD PTR ___$ReturnUdt$[ebp]

; 1157 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 84					; 00000054H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76338:
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CMemFile@@UAE@XZ			; CMemFile::~CMemFile
	ret	0
$L76339:
	lea	ecx, DWORD PTR _sRTF$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76340:
	mov	eax, DWORD PTR $T76341[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76342
	mov	ecx, DWORD PTR ___$ReturnUdt$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76342:
	ret	0
$L76345:
	mov	eax, OFFSET FLAT:$T76344
	jmp	___CxxFrameHandler
text$x	ENDS
?GetRTF@CRichEditBaseCtrl@@QBE?AVCString@@XZ ENDP	; CRichEditBaseCtrl::GetRTF
PUBLIC	?GetRTFLength@CRichEditBaseCtrl@@QBEHXZ		; CRichEditBaseCtrl::GetRTFLength
;	COMDAT ?GetRTFLength@CRichEditBaseCtrl@@QBEHXZ
_TEXT	SEGMENT
_this$ = -20
_nByteLen$ = -4
_es$ = -16
?GetRTFLength@CRichEditBaseCtrl@@QBEHXZ PROC NEAR	; CRichEditBaseCtrl::GetRTFLength, COMDAT

; 1160 : {

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

; 1161 : 	int nByteLen = 0;

	mov	DWORD PTR _nByteLen$[ebp], 0

; 1162 : 	EDITSTREAM es = { (DWORD)&nByteLen, 0, StreamOutLenCB };

	lea	eax, DWORD PTR _nByteLen$[ebp]
	mov	DWORD PTR _es$[ebp], eax
	mov	DWORD PTR _es$[ebp+4], 0
	mov	DWORD PTR _es$[ebp+8], OFFSET FLAT:?StreamOutLenCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z ; CRichEditBaseCtrl::StreamOutLenCB
	mov	ecx, DWORD PTR _es$[ebp+8]
	mov	DWORD PTR _es$[ebp+8], ecx

; 1163 : 	
; 1164 : 	const_cast<CRichEditBaseCtrl*>(this)->StreamOut(SF_RTF, es);

	lea	edx, DWORD PTR _es$[ebp]
	push	edx
	push	2
	mov	ecx, DWORD PTR _this$[ebp]
	call	?StreamOut@CRichEditCtrl@@QAEJHAAU_editstream@@@Z ; CRichEditCtrl::StreamOut

; 1165 : 	
; 1166 : 	return nByteLen;

	mov	eax, DWORD PTR _nByteLen$[ebp]

; 1167 : }

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetRTFLength@CRichEditBaseCtrl@@QBEHXZ ENDP		; CRichEditBaseCtrl::GetRTFLength
_TEXT	ENDS
PUBLIC	??0STREAMINCOOKIE@@QAE@ABVCString@@@Z		; STREAMINCOOKIE::STREAMINCOOKIE
EXTRN	?StreamIn@CRichEditCtrl@@QAEJHAAU_editstream@@@Z:NEAR ; CRichEditCtrl::StreamIn
;	COMDAT xdata$x
; File D:\_CODE\Shared\richeditbasectrl.cpp
xdata$x	SEGMENT
$T76363	DD	019930520H
	DD	03H
	DD	FLAT:$T76365
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T76365	DD	0ffffffffH
	DD	FLAT:$L76358
	DD	00H
	DD	FLAT:$L76360
	DD	0ffffffffH
	DD	FLAT:$L76360
xdata$x	ENDS
;	COMDAT ?SetRTF@CRichEditBaseCtrl@@QAEXABVCString@@@Z
_TEXT	SEGMENT
$T76353 = -40
$T76354 = -44
$T76357 = -48
__$EHRec$ = -12
_rtf$ = 8
_this$ = -52
_sRTF$ = -36
_cookie$ = -20
_es$ = -32
?SetRTF@CRichEditBaseCtrl@@QAEXABVCString@@@Z PROC NEAR	; CRichEditBaseCtrl::SetRTF, COMDAT

; 1170 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L76364
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 52					; 00000034H
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-64]
	mov	ecx, 13					; 0000000dH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx
	mov	DWORD PTR $T76357[ebp], 0

; 1171 : 	CString sRTF = (rtf.IsEmpty() ? DEFAULTRTF : rtf);

	mov	ecx, DWORD PTR _rtf$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	test	eax, eax
	je	SHORT $L76355
	mov	eax, DWORD PTR _DEFAULTRTF
	push	eax
	lea	ecx, DWORD PTR $T76354[ebp]
	call	??0CString@@QAE@PBD@Z			; CString::CString
	mov	DWORD PTR -56+[ebp], eax
	mov	ecx, DWORD PTR $T76357[ebp]
	or	ecx, 1
	mov	DWORD PTR $T76357[ebp], ecx
	mov	edx, DWORD PTR -56+[ebp]
	mov	DWORD PTR -60+[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], 0
	mov	eax, DWORD PTR -60+[ebp]
	mov	DWORD PTR -64+[ebp], eax
	jmp	SHORT $L76356
$L76355:
	mov	ecx, DWORD PTR _rtf$[ebp]
	mov	DWORD PTR -64+[ebp], ecx
$L76356:
	mov	edx, DWORD PTR -64+[ebp]
	mov	DWORD PTR $T76353[ebp], edx
	mov	eax, DWORD PTR $T76353[ebp]
	push	eax
	lea	ecx, DWORD PTR _sRTF$[ebp]
	call	??0CString@@QAE@ABV0@@Z			; CString::CString
	mov	DWORD PTR __$EHRec$[ebp+8], 2
	mov	ecx, DWORD PTR $T76357[ebp]
	and	ecx, 1
	test	ecx, ecx
	je	SHORT $L76361
	and	DWORD PTR $T76357[ebp], -2		; fffffffeH
	lea	ecx, DWORD PTR $T76354[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76361:

; 1172 : 	
; 1173 : 	STREAMINCOOKIE cookie(sRTF);

	lea	edx, DWORD PTR _sRTF$[ebp]
	push	edx
	lea	ecx, DWORD PTR _cookie$[ebp]
	call	??0STREAMINCOOKIE@@QAE@ABVCString@@@Z	; STREAMINCOOKIE::STREAMINCOOKIE

; 1174 : 	EDITSTREAM es = { (DWORD)&cookie, 0, StreamInCB };

	lea	eax, DWORD PTR _cookie$[ebp]
	mov	DWORD PTR _es$[ebp], eax
	mov	DWORD PTR _es$[ebp+4], 0
	mov	DWORD PTR _es$[ebp+8], OFFSET FLAT:?StreamInCB@CRichEditBaseCtrl@@KGKKPAEJPAJ@Z ; CRichEditBaseCtrl::StreamInCB
	mov	ecx, DWORD PTR _es$[ebp+8]
	mov	DWORD PTR _es$[ebp+8], ecx

; 1175 : 	
; 1176 : 	StreamIn(SF_RTF, es);

	lea	edx, DWORD PTR _es$[ebp]
	push	edx
	push	2
	mov	ecx, DWORD PTR _this$[ebp]
	call	?StreamIn@CRichEditCtrl@@QAEJHAAU_editstream@@@Z ; CRichEditCtrl::StreamIn

; 1177 : }

	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _sRTF$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 64					; 00000040H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L76358:
	mov	eax, DWORD PTR $T76357[ebp]
	and	eax, 1
	test	eax, eax
	je	$L76359
	lea	ecx, DWORD PTR $T76354[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L76359:
	ret	0
$L76360:
	lea	ecx, DWORD PTR _sRTF$[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L76364:
	mov	eax, OFFSET FLAT:$T76363
	jmp	___CxxFrameHandler
text$x	ENDS
?SetRTF@CRichEditBaseCtrl@@QAEXABVCString@@@Z ENDP	; CRichEditBaseCtrl::SetRTF
;	COMDAT ??0STREAMINCOOKIE@@QAE@ABVCString@@@Z
_TEXT	SEGMENT
_sContent$ = 8
_this$ = -4
??0STREAMINCOOKIE@@QAE@ABVCString@@@Z PROC NEAR		; STREAMINCOOKIE::STREAMINCOOKIE, COMDAT

; 46   : 	{

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _sContent$[ebp]
	mov	DWORD PTR [eax], ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+4], 0

; 47   : 	}

	mov	eax, DWORD PTR _this$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	4
??0STREAMINCOOKIE@@QAE@ABVCString@@@Z ENDP		; STREAMINCOOKIE::STREAMINCOOKIE
_TEXT	ENDS
END

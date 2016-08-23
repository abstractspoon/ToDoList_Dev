	TITLE	D:\_CODE\3rdParty\StdioFileEx.cpp
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
;	COMDAT ??_C@_0CF@POHN@D?3?2_CODE?23rdParty?2TemplateSmartP@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0BM@IBK@D?3?2_CODE?23rdParty?2ggets?4cpp?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ?GetFileString@@YAPADPADHPAU_iobuf@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetFileString@@YAPAGPAGHPAU_iobuf@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FindCharInString@@YAPADPBDH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FindCharInString@@YAPAGPBGG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CStdioFileEx@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCStdioFileEx@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1CStdioFileEx@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0CStdioFileEx@@QAE@PBGI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetCodePage@CStdioFileEx@@QAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetSaveAs@CStdioFileEx@@QAEXW4SFE_SAVEAS@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Open@CStdioFileEx@@UAEHPBGIW4SFE_SAVEAS@@PAVCFileException@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReadString@CStdioFileEx@@UAEHAAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReadString@CStdioFileEx@@UAEPAGPAGI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?WantUnicode@CStdioFileEx@@KA_NW4SFE_SAVEAS@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReadFile@CStdioFileEx@@QAEHAAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?WriteString@CStdioFileEx@@UAEXPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ProcessSaveFlags@CStdioFileEx@@IAEIAAIW4SFE_SAVEAS@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsFileUnicode@CStdioFileEx@@SA_NABVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetApproxCharCount@CStdioFileEx@@QAEKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Seek@CStdioFileEx@@UAEKKI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetCurrentLocaleCodePage@CStdioFileEx@@SAIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetUnicodeStringFromMultiByteString@CStdioFileEx@@SAHPBDPAGII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetRequiredUnicodeLengthFromMultiByteString@CStdioFileEx@@SAHPBDI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNewUnicodeStringFromMultiByteString@CStdioFileEx@@SAHPBDAAV?$CTemplateSmartPtrArray@G@@HI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetMultiByteStringFromUnicodeString@CStdioFileEx@@SAHPBGPADHID@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetRequiredMultiByteLengthForUnicodeString@CStdioFileEx@@SAHPBGI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNewMultiByteStringFromUnicodeString@CStdioFileEx@@SAHPBGAAV?$CTemplateSmartPtrArray@D@@HID@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReadUnicodeLine@CStdioFileEx@@IAEHAAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReadMultiByteLine@CStdioFileEx@@IAEHAAVCString@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?WriteUnicodeLine@CStdioFileEx@@IAEIPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?WriteMultiByteLine@CStdioFileEx@@IAEIPBG@Z
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
;	COMDAT ??0?$CTemplateSmartPtrArray@G@@QAE@PAG_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CTemplateSmartPtrArray@G@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Assign@?$CTemplateSmartPtrArray@G@@QAEXPAG_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetBuffer@?$CTemplateSmartPtrArray@G@@QAEPAGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$CTemplateSmartPtrArray@D@@QAE@PAD_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$CTemplateSmartPtrArray@D@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Assign@?$CTemplateSmartPtrArray@D@@QAEXPAD_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetBuffer@?$CTemplateSmartPtrArray@D@@QAEPADXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CTemplateSmartPtrArray@G@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$CTemplateSmartPtrArray@D@@UAEPAXI@Z
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
;	COMDAT ?fggets@@YAHPAPAGPAU_iobuf@@AA_NH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?fggets@@YAHPAPADPAU_iobuf@@AA_NH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReallocString@@YAPAGPAGIIAA_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ReallocString@@YAPADPADIIAA_N@Z
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
;	COMDAT ??_7?$CTemplateSmartPtrArray@D@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$CTemplateSmartPtrArray@G@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7CStdioFileEx@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	?modeWriteUnicode@CStdioFileEx@@2IB		; CStdioFileEx::modeWriteUnicode
_DATA	SEGMENT
_THIS_FILE DB	'D:\_CODE\3rdParty\StdioFileEx.cpp', 00H
_DATA	ENDS
CONST	SEGMENT
_UTF8_BOM DB	0efH
	DB	0bbH
	DB	0bfH
	ORG $+1
_UNICODE_BOM DW	0feffH
	ORG $+2
?modeWriteUnicode@CStdioFileEx@@2IB DD 0200000H		; CStdioFileEx::modeWriteUnicode
CONST	ENDS
PUBLIC	??0CStdioFileEx@@QAE@XZ				; CStdioFileEx::CStdioFileEx
PUBLIC	?Open@CStdioFileEx@@UAEHPBGIW4SFE_SAVEAS@@PAVCFileException@@@Z ; CStdioFileEx::Open
PUBLIC	?ReadString@CStdioFileEx@@UAEHAAVCString@@@Z	; CStdioFileEx::ReadString
PUBLIC	?ReadString@CStdioFileEx@@UAEPAGPAGI@Z		; CStdioFileEx::ReadString
PUBLIC	?WriteString@CStdioFileEx@@UAEXPBG@Z		; CStdioFileEx::WriteString
PUBLIC	?Seek@CStdioFileEx@@UAEKKI@Z			; CStdioFileEx::Seek
PUBLIC	??_7CStdioFileEx@@6B@				; CStdioFileEx::`vftable'
PUBLIC	??_GCStdioFileEx@@UAEPAXI@Z			; CStdioFileEx::`scalar deleting destructor'
PUBLIC	??_ECStdioFileEx@@UAEPAXI@Z			; CStdioFileEx::`vector deleting destructor'
EXTRN	?Serialize@CObject@@UAEXAAVCArchive@@@Z:NEAR	; CObject::Serialize
EXTRN	__chkesp:NEAR
EXTRN	?GetFileName@CFile@@UBE?AVCString@@XZ:NEAR	; CFile::GetFileName
EXTRN	?GetFileTitle@CFile@@UBE?AVCString@@XZ:NEAR	; CFile::GetFileTitle
EXTRN	?GetFilePath@CFile@@UBE?AVCString@@XZ:NEAR	; CFile::GetFilePath
EXTRN	?SetFilePath@CFile@@UAEXPBG@Z:NEAR		; CFile::SetFilePath
EXTRN	?SetLength@CFile@@UAEXK@Z:NEAR			; CFile::SetLength
EXTRN	?GetLength@CFile@@UBEKXZ:NEAR			; CFile::GetLength
EXTRN	?AssertValid@CFile@@UBEXXZ:NEAR			; CFile::AssertValid
EXTRN	?GetBufferPtr@CFile@@UAEIIIPAPAX0@Z:NEAR	; CFile::GetBufferPtr
EXTRN	?GetRuntimeClass@CStdioFile@@UBEPAUCRuntimeClass@@XZ:NEAR ; CStdioFile::GetRuntimeClass
EXTRN	??0CStdioFile@@QAE@XZ:NEAR			; CStdioFile::CStdioFile
EXTRN	?Dump@CStdioFile@@UBEXAAVCDumpContext@@@Z:NEAR	; CStdioFile::Dump
EXTRN	?GetPosition@CStdioFile@@UBEKXZ:NEAR		; CStdioFile::GetPosition
EXTRN	?Open@CStdioFile@@UAEHPBGIPAVCFileException@@@Z:NEAR ; CStdioFile::Open
EXTRN	?Read@CStdioFile@@UAEIPAXI@Z:NEAR		; CStdioFile::Read
EXTRN	?Write@CStdioFile@@UAEXPBXI@Z:NEAR		; CStdioFile::Write
EXTRN	?Seek@CStdioFile@@UAEJJI@Z:NEAR			; CStdioFile::Seek
EXTRN	?Abort@CStdioFile@@UAEXXZ:NEAR			; CStdioFile::Abort
EXTRN	?Flush@CStdioFile@@UAEXXZ:NEAR			; CStdioFile::Flush
EXTRN	?Close@CStdioFile@@UAEXXZ:NEAR			; CStdioFile::Close
EXTRN	?Duplicate@CStdioFile@@UBEPAVCFile@@XZ:NEAR	; CStdioFile::Duplicate
EXTRN	?LockRange@CStdioFile@@UAEXKK@Z:NEAR		; CStdioFile::LockRange
EXTRN	?UnlockRange@CStdioFile@@UAEXKK@Z:NEAR		; CStdioFile::UnlockRange
;	COMDAT ??_7CStdioFileEx@@6B@
; File D:\_CODE\3rdParty\StdioFileEx.cpp
CONST	SEGMENT
??_7CStdioFileEx@@6B@ DD FLAT:?GetRuntimeClass@CStdioFile@@UBEPAUCRuntimeClass@@XZ ; CStdioFileEx::`vftable'
	DD	FLAT:??_ECStdioFileEx@@UAEPAXI@Z
	DD	FLAT:?Serialize@CObject@@UAEXAAVCArchive@@@Z
	DD	FLAT:?AssertValid@CFile@@UBEXXZ
	DD	FLAT:?Dump@CStdioFile@@UBEXAAVCDumpContext@@@Z
	DD	FLAT:?GetPosition@CStdioFile@@UBEKXZ
	DD	FLAT:?GetFileName@CFile@@UBE?AVCString@@XZ
	DD	FLAT:?GetFileTitle@CFile@@UBE?AVCString@@XZ
	DD	FLAT:?GetFilePath@CFile@@UBE?AVCString@@XZ
	DD	FLAT:?SetFilePath@CFile@@UAEXPBG@Z
	DD	FLAT:?Open@CStdioFile@@UAEHPBGIPAVCFileException@@@Z
	DD	FLAT:?Duplicate@CStdioFile@@UBEPAVCFile@@XZ
	DD	FLAT:?Seek@CStdioFile@@UAEJJI@Z
	DD	FLAT:?SetLength@CFile@@UAEXK@Z
	DD	FLAT:?GetLength@CFile@@UBEKXZ
	DD	FLAT:?Read@CStdioFile@@UAEIPAXI@Z
	DD	FLAT:?Write@CStdioFile@@UAEXPBXI@Z
	DD	FLAT:?LockRange@CStdioFile@@UAEXKK@Z
	DD	FLAT:?UnlockRange@CStdioFile@@UAEXKK@Z
	DD	FLAT:?Abort@CStdioFile@@UAEXXZ
	DD	FLAT:?Flush@CStdioFile@@UAEXXZ
	DD	FLAT:?Close@CStdioFile@@UAEXXZ
	DD	FLAT:?GetBufferPtr@CFile@@UAEIIIPAPAX0@Z
	DD	FLAT:?WriteString@CStdioFileEx@@UAEXPBG@Z
	DD	FLAT:?ReadString@CStdioFileEx@@UAEHAAVCString@@@Z
	DD	FLAT:?ReadString@CStdioFileEx@@UAEPAGPAGI@Z
	DD	FLAT:?Open@CStdioFileEx@@UAEHPBGIW4SFE_SAVEAS@@PAVCFileException@@@Z
	DD	FLAT:?Seek@CStdioFileEx@@UAEKKI@Z
CONST	ENDS
;	COMDAT ??0CStdioFileEx@@QAE@XZ
_TEXT	SEGMENT
_this$ = -4
??0CStdioFileEx@@QAE@XZ PROC NEAR			; CStdioFileEx::CStdioFileEx, COMDAT

; 89   : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CStdioFile@@QAE@XZ			; CStdioFile::CStdioFile
	mov	eax, DWORD PTR _this$[ebp]
	mov	BYTE PTR [eax+20], 0
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+24], -1
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+32], -1
	mov	eax, DWORD PTR _this$[ebp]
	mov	BYTE PTR [eax+36], 35			; 00000023H
	mov	ecx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [ecx+37], 1
	mov	edx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [edx+12326], 1
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7CStdioFileEx@@6B@ ; CStdioFileEx::`vftable'

; 90   : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??0CStdioFileEx@@QAE@XZ ENDP				; CStdioFileEx::CStdioFileEx
_TEXT	ENDS
PUBLIC	??1CStdioFileEx@@UAE@XZ				; CStdioFileEx::~CStdioFileEx
EXTRN	??3CObject@@SGXPAX@Z:NEAR			; CObject::operator delete
;	COMDAT ??_GCStdioFileEx@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_GCStdioFileEx@@UAEPAXI@Z PROC NEAR			; CStdioFileEx::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CStdioFileEx@@UAE@XZ			; CStdioFileEx::~CStdioFileEx
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73258
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3CObject@@SGXPAX@Z			; CObject::operator delete
$L73258:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_GCStdioFileEx@@UAEPAXI@Z ENDP			; CStdioFileEx::`scalar deleting destructor'
_TEXT	ENDS
EXTRN	??1CStdioFile@@UAE@XZ:NEAR			; CStdioFile::~CStdioFile
;	COMDAT ??1CStdioFileEx@@UAE@XZ
_TEXT	SEGMENT
_this$ = -4
??1CStdioFileEx@@UAE@XZ PROC NEAR			; CStdioFileEx::~CStdioFileEx, COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1CStdioFile@@UAE@XZ			; CStdioFile::~CStdioFile
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1CStdioFileEx@@UAE@XZ ENDP				; CStdioFileEx::~CStdioFileEx
_TEXT	ENDS
PUBLIC	??0CStdioFileEx@@QAE@PBGI@Z			; CStdioFileEx::CStdioFileEx
EXTRN	??0CStdioFile@@QAE@PBGI@Z:NEAR			; CStdioFile::CStdioFile
;	COMDAT ??0CStdioFileEx@@QAE@PBGI@Z
_TEXT	SEGMENT
_lpszFileName$ = 8
_nOpenFlags$ = 12
_this$ = -4
??0CStdioFileEx@@QAE@PBGI@Z PROC NEAR			; CStdioFileEx::CStdioFileEx, COMDAT

; 100  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _nOpenFlags$[ebp]
	push	eax
	mov	ecx, DWORD PTR _lpszFileName$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??0CStdioFile@@QAE@PBGI@Z		; CStdioFile::CStdioFile
	mov	edx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [edx+20], 0
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+24], -1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+32], -1
	mov	edx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [edx+36], 35			; 00000023H
	mov	eax, DWORD PTR _this$[ebp]
	mov	BYTE PTR [eax+37], 1
	mov	ecx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [ecx+12326], 1
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx], OFFSET FLAT:??_7CStdioFileEx@@6B@ ; CStdioFileEx::`vftable'

; 101  : }

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
??0CStdioFileEx@@QAE@PBGI@Z ENDP			; CStdioFileEx::CStdioFileEx
_TEXT	ENDS
PUBLIC	?SetCodePage@CStdioFileEx@@QAEXI@Z		; CStdioFileEx::SetCodePage
;	COMDAT ?SetCodePage@CStdioFileEx@@QAEXI@Z
_TEXT	SEGMENT
_nCodePage$ = 8
_this$ = -4
?SetCodePage@CStdioFileEx@@QAEXI@Z PROC NEAR		; CStdioFileEx::SetCodePage, COMDAT

; 105  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 106  : 	m_nFileCodePage = (int)nCodePage; 

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nCodePage$[ebp]
	mov	DWORD PTR [eax+32], ecx

; 107  : }

	mov	esp, ebp
	pop	ebp
	ret	4
?SetCodePage@CStdioFileEx@@QAEXI@Z ENDP			; CStdioFileEx::SetCodePage
_TEXT	ENDS
PUBLIC	?SetSaveAs@CStdioFileEx@@QAEXW4SFE_SAVEAS@@@Z	; CStdioFileEx::SetSaveAs
PUBLIC	?WantUnicode@CStdioFileEx@@KA_NW4SFE_SAVEAS@@@Z	; CStdioFileEx::WantUnicode
;	COMDAT ?SetSaveAs@CStdioFileEx@@QAEXW4SFE_SAVEAS@@@Z
_TEXT	SEGMENT
_nSaveAs$ = 8
_this$ = -4
?SetSaveAs@CStdioFileEx@@QAEXW4SFE_SAVEAS@@@Z PROC NEAR	; CStdioFileEx::SetSaveAs, COMDAT

; 110  : { 

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 111  : 	m_nSaveAs = nSaveAs; 

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _nSaveAs$[ebp]
	mov	DWORD PTR [eax+24], ecx

; 112  : 	m_bIsUnicodeText = WantUnicode(nSaveAs);

	mov	edx, DWORD PTR _nSaveAs$[ebp]
	push	edx
	call	?WantUnicode@CStdioFileEx@@KA_NW4SFE_SAVEAS@@@Z ; CStdioFileEx::WantUnicode
	add	esp, 4
	mov	ecx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [ecx+20], al

; 113  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?SetSaveAs@CStdioFileEx@@QAEXW4SFE_SAVEAS@@@Z ENDP	; CStdioFileEx::SetSaveAs
_TEXT	ENDS
PUBLIC	?ProcessSaveFlags@CStdioFileEx@@IAEIAAIW4SFE_SAVEAS@@@Z ; CStdioFileEx::ProcessSaveFlags
EXTRN	?SeekToBegin@CFile@@QAEXXZ:NEAR			; CFile::SeekToBegin
EXTRN	_memcmp:NEAR
;	COMDAT ?Open@CStdioFileEx@@UAEHPBGIW4SFE_SAVEAS@@PAVCFileException@@@Z
_TEXT	SEGMENT
_lpszFileName$ = 8
_nOpenFlags$ = 12
_nSaveAs$ = 16
_pError$ = 20
_this$ = -16
_bOK$ = -4
_cBOMTest$73288 = -8
_arrUTF8BOMTest$73292 = -12
?Open@CStdioFileEx@@UAEHPBGIW4SFE_SAVEAS@@PAVCFileException@@@Z PROC NEAR ; CStdioFileEx::Open, COMDAT

; 116  : {

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

; 117  : 	// Process any Unicode stuff. This no longer checks for the Unicode BOM. We do this on
; 118  : 	// opening for efficiency.
; 119  : 	// if m_nSaveAs has been set then it overrides unless nSaveAs has been explicitly set
; 120  : 	if (m_nSaveAs != (SFE_SAVEAS)-1 && nSaveAs == SFE_ASCOMPILED)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+24], -1
	je	SHORT $L73284
	cmp	DWORD PTR _nSaveAs$[ebp], 2
	jne	SHORT $L73284

; 121  : 		ProcessSaveFlags(nOpenFlags, m_nSaveAs);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	push	edx
	lea	eax, DWORD PTR _nOpenFlags$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessSaveFlags@CStdioFileEx@@IAEIAAIW4SFE_SAVEAS@@@Z ; CStdioFileEx::ProcessSaveFlags

; 122  : 	else

	jmp	SHORT $L73285
$L73284:

; 123  : 		ProcessSaveFlags(nOpenFlags, nSaveAs);

	mov	ecx, DWORD PTR _nSaveAs$[ebp]
	push	ecx
	lea	edx, DWORD PTR _nOpenFlags$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ProcessSaveFlags@CStdioFileEx@@IAEIAAIW4SFE_SAVEAS@@@Z ; CStdioFileEx::ProcessSaveFlags
$L73285:

; 124  : 
; 125  : 	BOOL bOK = CStdioFile::Open(lpszFileName, nOpenFlags, pError);

	mov	eax, DWORD PTR _pError$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nOpenFlags$[ebp]
	push	ecx
	mov	edx, DWORD PTR _lpszFileName$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Open@CStdioFile@@UAEHPBGIPAVCFileException@@@Z ; CStdioFile::Open
	mov	DWORD PTR _bOK$[ebp], eax

; 126  : 
; 127  : 	// If we are reading or writing an existing file, see if it has a BOM. 
; 128  : 	// and use the BOM's existence (or not) to override the supplied format
; 129  : 	if (bOK && GetLength() >= max(UTF8_BOM_SIZE, UNICODE_BOM_SIZE))

	cmp	DWORD PTR _bOK$[ebp], 0
	je	$L73287
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+56]
	cmp	esi, esp
	call	__chkesp
	cmp	eax, 3
	jb	$L73287

; 131  : 		// check for unicode BOM
; 132  : 		wchar_t	cBOMTest;
; 133  : 		Read(&cBOMTest, UNICODE_BOM_SIZE);

	mov	esi, esp
	push	2
	lea	eax, DWORD PTR _cBOMTest$73288[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+60]
	cmp	esi, esp
	call	__chkesp

; 134  : 
; 135  : 		m_bIsUnicodeText = (memcmp(&cBOMTest, UNICODE_BOM, UNICODE_BOM_SIZE) == 0);

	push	2
	push	OFFSET FLAT:_UNICODE_BOM
	lea	eax, DWORD PTR _cBOMTest$73288[ebp]
	push	eax
	call	_memcmp
	add	esp, 12					; 0000000cH
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [ecx+20], al

; 136  : 
; 137  : 		if (m_bIsUnicodeText)

	mov	edx, DWORD PTR _this$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+20]
	test	eax, eax
	je	SHORT $L73289

; 139  : 			m_bWriteBOM = TRUE;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [ecx+37], 1

; 140  : 			m_nSaveAs = SFE_UNICODE;

	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+24], 1

; 141  : 			m_nFlags |= CStdioFileEx::modeWriteUnicode;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+28]
	or	ecx, 2097152				; 00200000H
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+28], ecx

; 143  : 		else // not unicode

	jmp	SHORT $L73291
$L73289:

; 145  : 			m_bWriteBOM = FALSE;

	mov	eax, DWORD PTR _this$[ebp]
	mov	BYTE PTR [eax+37], 0

; 146  : 			m_nSaveAs = SFE_MULTIBYTE;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+24], 0

; 147  : 			m_nFlags &= ~CStdioFileEx::modeWriteUnicode;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+28]
	and	eax, -2097153				; ffdfffffH
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+28], eax

; 148  : 
; 149  : 			// check for UTF8 BOM
; 150  : 			if (CP_UTF8 == m_nFileCodePage)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+32], 65001		; 0000fde9H
	jne	SHORT $L73291

; 152  : 				BYTE	arrUTF8BOMTest[UTF8_BOM_SIZE];
; 153  : 				Read(arrUTF8BOMTest, UTF8_BOM_SIZE);

	mov	esi, esp
	push	3
	lea	eax, DWORD PTR _arrUTF8BOMTest$73292[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+60]
	cmp	esi, esp
	call	__chkesp

; 154  : 				
; 155  : 				m_bWriteBOM = (memcmp(&arrUTF8BOMTest, UTF8_BOM, UTF8_BOM_SIZE) == 0);

	push	3
	push	OFFSET FLAT:_UTF8_BOM
	lea	eax, DWORD PTR _arrUTF8BOMTest$73292[ebp]
	push	eax
	call	_memcmp
	add	esp, 12					; 0000000cH
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [ecx+37], al
$L73291:

; 158  : 		
; 159  : 		// Reset to start of file
; 160  : 		SeekToBegin();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?SeekToBegin@CFile@@QAEXXZ		; CFile::SeekToBegin

; 161  : 		
; 162  : 		m_bCheckFilePos = true;

	mov	edx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [edx+12326], 1

; 163  : 		m_nBytesWritten = 0;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+12328], 0
$L73287:

; 165  : 
; 166  : 	return bOK;

	mov	eax, DWORD PTR _bOK$[ebp]

; 167  : }

	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?Open@CStdioFileEx@@UAEHPBGIW4SFE_SAVEAS@@PAVCFileException@@@Z ENDP ; CStdioFileEx::Open
_TEXT	ENDS
PUBLIC	?ReadUnicodeLine@CStdioFileEx@@IAEHAAVCString@@@Z ; CStdioFileEx::ReadUnicodeLine
PUBLIC	?ReadMultiByteLine@CStdioFileEx@@IAEHAAVCString@@@Z ; CStdioFileEx::ReadMultiByteLine
EXTRN	?AfxAssertFailedLine@@YGHPBDH@Z:NEAR		; AfxAssertFailedLine
EXTRN	?GetLength@CString@@QBEHXZ:NEAR			; CString::GetLength
EXTRN	?GetBuffer@CString@@QAEPAGH@Z:NEAR		; CString::GetBuffer
EXTRN	?ReleaseBuffer@CString@@QAEXH@Z:NEAR		; CString::ReleaseBuffer
EXTRN	?GetBufferSetLength@CString@@QAEPAGH@Z:NEAR	; CString::GetBufferSetLength
;	COMDAT ?ReadString@CStdioFileEx@@UAEHAAVCString@@@Z
_TEXT	SEGMENT
_rString$ = 8
_this$ = -24
_bReadData$ = -12
_lpsz$ = -4
_nLen$ = -8
_cBOMDummy$73306 = -16
_arrUTF8BOMTest$73310 = -20
?ReadString@CStdioFileEx@@UAEHAAVCString@@@Z PROC NEAR	; CStdioFileEx::ReadString, COMDAT

; 170  : {

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
$L73297:

; 171  : 	ASSERT(m_pStream != NULL);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	jne	SHORT $L73300
	push	171					; 000000abH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73300
	int	3
$L73300:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73297

; 172  : 
; 173  : 	BOOL			bReadData = FALSE;

	mov	DWORD PTR _bReadData$[ebp], 0

; 174  : 	LPTSTR		lpsz;
; 175  : 	int			nLen = 0;

	mov	DWORD PTR _nLen$[ebp], 0

; 176  : 
; 177  : 	// If at position 0, discard byte-order mark before reading. To optimise reading, we only
; 178  : 	// check this when the m_bCheckFilePos is set (this avoids a call to ftell every time we
; 179  : 	// read a line)
; 180  : 	if (m_bCheckFilePos && GetPosition() == 0)

	mov	edx, DWORD PTR _this$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+12326]
	test	eax, eax
	je	SHORT $L73309
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+20]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L73309

; 182  : 		// Look for Unicode BOM
; 183  : 		if (m_bIsUnicodeText)

	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+20]
	test	ecx, ecx
	je	SHORT $L73305

; 185  : 			wchar_t	cBOMDummy;
; 186  : 			Read(&cBOMDummy, sizeof(wchar_t));

	mov	esi, esp
	push	2
	lea	edx, DWORD PTR _cBOMDummy$73306[ebp]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+60]
	cmp	esi, esp
	call	__chkesp

; 189  : 		// Look for UTF8 BOM
; 190  : 		else if (CP_UTF8 == m_nFileCodePage)

	jmp	SHORT $L73309
$L73305:
	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+32], 65001		; 0000fde9H
	jne	SHORT $L73309

; 192  : 			BYTE	arrUTF8BOMTest[UTF8_BOM_SIZE];
; 193  : 			Read(arrUTF8BOMTest, UTF8_BOM_SIZE);

	mov	esi, esp
	push	3
	lea	ecx, DWORD PTR _arrUTF8BOMTest$73310[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [eax+60]
	cmp	esi, esp
	call	__chkesp
$L73309:

; 196  : 
; 197  : 	// Read Unicode line or multibyte line (implementations 
; 198  : 	// differ depending on the compilation)
; 199  : 	if (m_bIsUnicodeText)

	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+20]
	test	edx, edx
	je	SHORT $L73311

; 201  : 		bReadData = ReadUnicodeLine(rString);

	mov	eax, DWORD PTR _rString$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ReadUnicodeLine@CStdioFileEx@@IAEHAAVCString@@@Z ; CStdioFileEx::ReadUnicodeLine
	mov	DWORD PTR _bReadData$[ebp], eax

; 203  : 	else

	jmp	SHORT $L73312
$L73311:

; 205  : 		bReadData = ReadMultiByteLine(rString);

	mov	ecx, DWORD PTR _rString$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ReadMultiByteLine@CStdioFileEx@@IAEHAAVCString@@@Z ; CStdioFileEx::ReadMultiByteLine
	mov	DWORD PTR _bReadData$[ebp], eax
$L73312:

; 207  : 
; 208  : 	// Then remove end-of-line character as necessary.  
; 209  : 	// fggets keeps the end-of-line confusion level at maximum by stripping the \n
; 210  : 	// from the end of lines, but leaving the \r. Grrrr.
; 211  : 	// Remember that you could quite legitimately have a \r or \n at the end of
; 212  : 	// your line before the actual \r\n line break. 
; 213  : 	if (bReadData)

	cmp	DWORD PTR _bReadData$[ebp], 0
	je	SHORT $L73313

; 215  : 		// Copied from FileTxt.cpp but adapted to use of fgets
; 216  : 		nLen = rString.GetLength();

	mov	ecx, DWORD PTR _rString$[ebp]
	call	?GetLength@CString@@QBEHXZ		; CString::GetLength
	mov	DWORD PTR _nLen$[ebp], eax

; 217  : 		
; 218  : 		lpsz = rString.GetBuffer(0);

	push	0
	mov	ecx, DWORD PTR _rString$[ebp]
	call	?GetBuffer@CString@@QAEPAGH@Z		; CString::GetBuffer
	mov	DWORD PTR _lpsz$[ebp], eax

; 219  : 
; 220  : 		// Strip \r from the end
; 221  : 		if (nLen != 0 && (lpsz[nLen-1] == _T('\r') ))

	cmp	DWORD PTR _nLen$[ebp], 0
	je	SHORT $L73314
	mov	edx, DWORD PTR _nLen$[ebp]
	mov	eax, DWORD PTR _lpsz$[ebp]
	xor	ecx, ecx
	mov	cx, WORD PTR [eax+edx*2-2]
	cmp	ecx, 13					; 0000000dH
	jne	SHORT $L73314

; 223  : 			rString.GetBufferSetLength(nLen-1);

	mov	edx, DWORD PTR _nLen$[ebp]
	sub	edx, 1
	push	edx
	mov	ecx, DWORD PTR _rString$[ebp]
	call	?GetBufferSetLength@CString@@QAEPAGH@Z	; CString::GetBufferSetLength
$L73314:

; 225  : 
; 226  : 		rString.ReleaseBuffer();

	push	-1
	mov	ecx, DWORD PTR _rString$[ebp]
	call	?ReleaseBuffer@CString@@QAEXH@Z		; CString::ReleaseBuffer

; 227  : 
; 228  : 		// Now we've moved on in the file, don't bother to check any more unless the
; 229  : 		// file pointer is moved
; 230  : 		m_bCheckFilePos = false;

	mov	eax, DWORD PTR _this$[ebp]
	mov	BYTE PTR [eax+12326], 0
$L73313:

; 232  : 
; 233  : 	return bReadData;

	mov	eax, DWORD PTR _bReadData$[ebp]

; 234  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 24					; 00000018H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ReadString@CStdioFileEx@@UAEHAAVCString@@@Z ENDP	; CStdioFileEx::ReadString
_TEXT	ENDS
EXTRN	?ReadString@CStdioFile@@UAEPAGPAGI@Z:NEAR	; CStdioFile::ReadString
;	COMDAT ?ReadString@CStdioFileEx@@UAEPAGPAGI@Z
_TEXT	SEGMENT
_lpsz$ = 8
_nMax$ = 12
_this$ = -4
?ReadString@CStdioFileEx@@UAEPAGPAGI@Z PROC NEAR	; CStdioFileEx::ReadString, COMDAT

; 237  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L73320:

; 238  : 	// Can't really provide compatible behaviour at the moment, so no Unicode handling.
; 239  : 	ASSERT(false);

	mov	eax, 1
	test	eax, eax
	je	SHORT $L73323
	push	239					; 000000efH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73323
	int	3
$L73323:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73320

; 240  : 
; 241  : 	return CStdioFile::ReadString(lpsz, nMax);

	mov	edx, DWORD PTR _nMax$[ebp]
	push	edx
	mov	eax, DWORD PTR _lpsz$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?ReadString@CStdioFile@@UAEPAGPAGI@Z	; CStdioFile::ReadString

; 242  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?ReadString@CStdioFileEx@@UAEPAGPAGI@Z ENDP		; CStdioFileEx::ReadString
_TEXT	ENDS
;	COMDAT ?WantUnicode@CStdioFileEx@@KA_NW4SFE_SAVEAS@@@Z
_TEXT	SEGMENT
_nSaveAs$ = 8
?WantUnicode@CStdioFileEx@@KA_NW4SFE_SAVEAS@@@Z PROC NEAR ; CStdioFileEx::WantUnicode, COMDAT

; 245  : {

	push	ebp
	mov	ebp, esp

; 246  : #ifdef _UNICODE
; 247  : 
; 248  : 		return (nSaveAs != SFE_MULTIBYTE);

	xor	eax, eax
	cmp	DWORD PTR _nSaveAs$[ebp], 0
	setne	al

; 249  : 
; 250  : #else // ANSI
; 251  : 		
; 252  : 		return (nSaveAs == SFE_UNICODE);
; 253  : 
; 254  : #endif
; 255  : }

	pop	ebp
	ret	0
?WantUnicode@CStdioFileEx@@KA_NW4SFE_SAVEAS@@@Z ENDP	; CStdioFileEx::WantUnicode
_TEXT	ENDS
PUBLIC	?ReadFile@CStdioFileEx@@QAEHAAVCString@@@Z	; CStdioFileEx::ReadFile
PUBLIC	?GetCurrentLocaleCodePage@CStdioFileEx@@SAIXZ	; CStdioFileEx::GetCurrentLocaleCodePage
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
EXTRN	__imp__MultiByteToWideChar@24:NEAR
EXTRN	_memset:NEAR
EXTRN	?IsEmpty@CString@@QBEHXZ:NEAR			; CString::IsEmpty
EXTRN	??4CString@@QAEABV0@PBG@Z:NEAR			; CString::operator=
EXTRN	??2@YAPAXIPBDH@Z:NEAR				; operator new
;	COMDAT ?ReadFile@CStdioFileEx@@QAEHAAVCString@@@Z
_TEXT	SEGMENT
$T74060 = -48
$T74065 = -52
_sFile$ = 8
_this$ = -56
_nByteOffset$ = -20
_cBOMDummy$73334 = -24
_arrUTF8BOMTest$73338 = -28
_bUTF8$73339 = -32
_dwBytesLeft$ = -12
_dwTotalBytesRead$ = -16
_pFileContents$ = -4
_pFilePtr$ = -8
_nRead$73354 = -36
_nLen$73358 = -40
_pTo$73365 = -44
?ReadFile@CStdioFileEx@@QAEHAAVCString@@@Z PROC NEAR	; CStdioFileEx::ReadFile, COMDAT

; 258  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 64					; 00000040H
	push	ebx
	push	esi
	push	edi
	push	ecx
	lea	edi, DWORD PTR [ebp-64]
	mov	ecx, 16					; 00000010H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 259  : 	SeekToBegin();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?SeekToBegin@CFile@@QAEXXZ		; CFile::SeekToBegin

; 260  : 	int nByteOffset = 0;

	mov	DWORD PTR _nByteOffset$[ebp], 0

; 261  : 
; 262  : 	if (m_bCheckFilePos && GetPosition() == 0)

	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+12326]
	test	ecx, ecx
	je	$L73342
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [eax+20]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	$L73342

; 264  : 		// Look for Unicode BOM
; 265  : 		if (m_bIsUnicodeText)

	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+20]
	test	edx, edx
	je	SHORT $L73333

; 267  : 			wchar_t	cBOMDummy;
; 268  : 			Read(&cBOMDummy, sizeof(wchar_t));

	mov	esi, esp
	push	2
	lea	eax, DWORD PTR _cBOMDummy$73334[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+60]
	cmp	esi, esp
	call	__chkesp

; 269  : 
; 270  : 			nByteOffset = sizeof(wchar_t);

	mov	DWORD PTR _nByteOffset$[ebp], 2

; 272  : 		else // Look for UTF8 BOM

	jmp	SHORT $L73342
$L73333:

; 274  : 			BYTE	arrUTF8BOMTest[UTF8_BOM_SIZE];
; 275  : 			Read(arrUTF8BOMTest, UTF8_BOM_SIZE);

	mov	esi, esp
	push	3
	lea	eax, DWORD PTR _arrUTF8BOMTest$73338[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+60]
	cmp	esi, esp
	call	__chkesp

; 276  : 
; 277  : 			BOOL bUTF8 = (memcmp(arrUTF8BOMTest, UTF8_BOM, UTF8_BOM_SIZE) == 0);

	push	3
	push	OFFSET FLAT:_UTF8_BOM
	lea	eax, DWORD PTR _arrUTF8BOMTest$73338[ebp]
	push	eax
	call	_memcmp
	add	esp, 12					; 0000000cH
	neg	eax
	sbb	eax, eax
	inc	eax
	mov	DWORD PTR _bUTF8$73339[ebp], eax

; 278  : 
; 279  : 			if (bUTF8)

	cmp	DWORD PTR _bUTF8$73339[ebp], 0
	je	SHORT $L73340

; 281  : 				m_nFileCodePage = CP_UTF8;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+32], 65001		; 0000fde9H

; 282  : 				nByteOffset = UTF8_BOM_SIZE;

	mov	DWORD PTR _nByteOffset$[ebp], 3

; 284  : 			else

	jmp	SHORT $L73342
$L73340:

; 286  : 				SeekToBegin();

	mov	ecx, DWORD PTR _this$[ebp]
	call	?SeekToBegin@CFile@@QAEXXZ		; CFile::SeekToBegin

; 287  : 
; 288  : 				if (m_nFileCodePage == -1)

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+32], -1
	jne	SHORT $L73342

; 289  : 					m_nFileCodePage = CP_ACP;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+32], 0
$L73342:

; 293  : 
; 294  : 	// read entire file in as a byte stream
; 295  : 	DWORD dwBytesLeft = ((DWORD)GetLength() - nByteOffset), dwTotalBytesRead = 0;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+56]
	cmp	esi, esp
	call	__chkesp
	sub	eax, DWORD PTR _nByteOffset$[ebp]
	mov	DWORD PTR _dwBytesLeft$[ebp], eax
	mov	DWORD PTR _dwTotalBytesRead$[ebp], 0

; 296  : 
; 297  : 	if (dwBytesLeft == 0) // zero length file

	cmp	DWORD PTR _dwBytesLeft$[ebp], 0
	jne	SHORT $L73346

; 298  : 		return TRUE;

	mov	eax, 1
	jmp	$L73330
$L73346:

; 299  : 
; 300  : 	LPSTR pFileContents = new CHAR[dwBytesLeft + 2]; // +2 for unicode zero terminator

	push	300					; 0000012cH
	push	OFFSET FLAT:_THIS_FILE
	mov	eax, DWORD PTR _dwBytesLeft$[ebp]
	add	eax, 2
	push	eax
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T74060[ebp], eax
	mov	ecx, DWORD PTR $T74060[ebp]
	mov	DWORD PTR _pFileContents$[ebp], ecx

; 301  : 	ZeroMemory(pFileContents, dwBytesLeft + 2);

	mov	edx, DWORD PTR _dwBytesLeft$[ebp]
	add	edx, 2
	push	edx
	push	0
	mov	eax, DWORD PTR _pFileContents$[ebp]
	push	eax
	call	_memset
	add	esp, 12					; 0000000cH

; 302  : 	LPSTR pFilePtr = pFileContents;

	mov	ecx, DWORD PTR _pFileContents$[ebp]
	mov	DWORD PTR _pFilePtr$[ebp], ecx
$L73352:

; 303  : 		
; 304  : 	while (dwBytesLeft > 0)

	cmp	DWORD PTR _dwBytesLeft$[ebp], 0
	jbe	SHORT $L73353

; 306  : 		DWORD nRead = Read(pFilePtr, dwBytesLeft);

	mov	esi, esp
	mov	edx, DWORD PTR _dwBytesLeft$[ebp]
	push	edx
	mov	eax, DWORD PTR _pFilePtr$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+60]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nRead$73354[ebp], eax

; 307  : 
; 308  : 		pFilePtr += nRead;

	mov	eax, DWORD PTR _pFilePtr$[ebp]
	add	eax, DWORD PTR _nRead$73354[ebp]
	mov	DWORD PTR _pFilePtr$[ebp], eax

; 309  : 		dwTotalBytesRead += nRead;

	mov	ecx, DWORD PTR _dwTotalBytesRead$[ebp]
	add	ecx, DWORD PTR _nRead$73354[ebp]
	mov	DWORD PTR _dwTotalBytesRead$[ebp], ecx

; 310  : 		dwBytesLeft -= nRead;

	mov	edx, DWORD PTR _dwBytesLeft$[ebp]
	sub	edx, DWORD PTR _nRead$73354[ebp]
	mov	DWORD PTR _dwBytesLeft$[ebp], edx

; 311  : 	}

	jmp	SHORT $L73352
$L73353:

; 312  : 
; 313  : #ifdef _UNICODE
; 314  : 	if (m_bIsUnicodeText) // byte stream is already unicode format

	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+20]
	test	ecx, ecx
	je	SHORT $L73355

; 316  : 		sFile = (LPCTSTR)pFileContents;

	mov	edx, DWORD PTR _pFileContents$[ebp]
	push	edx
	mov	ecx, DWORD PTR _sFile$[ebp]
	call	??4CString@@QAEABV0@PBG@Z		; CString::operator=

; 318  : 	else // convert _from_ multibyte

	jmp	$L73364
$L73355:

; 320  : 		// Use api convert routine
; 321  : 		int nLen = MultiByteToWideChar(m_nFileCodePage == -1 ? GetCurrentLocaleCodePage() : m_nFileCodePage, 0, (LPCSTR)pFileContents, dwTotalBytesRead, NULL, 0);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+32], -1
	jne	SHORT $L74061
	call	?GetCurrentLocaleCodePage@CStdioFileEx@@SAIXZ ; CStdioFileEx::GetCurrentLocaleCodePage
	mov	DWORD PTR -60+[ebp], eax
	jmp	SHORT $L74062
$L74061:
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+32]
	mov	DWORD PTR -60+[ebp], edx
$L74062:
	mov	esi, esp
	push	0
	push	0
	mov	eax, DWORD PTR _dwTotalBytesRead$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pFileContents$[ebp]
	push	ecx
	push	0
	mov	edx, DWORD PTR -60+[ebp]
	push	edx
	call	DWORD PTR __imp__MultiByteToWideChar@24
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nLen$73358[ebp], eax
$L73360:

; 322  : 
; 323  : 		// if wlen == 0, some unknown codepage was probably used.
; 324  : 		ASSERT(nLen);

	cmp	DWORD PTR _nLen$73358[ebp], 0
	jne	SHORT $L73363
	push	324					; 00000144H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73363
	int	3
$L73363:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73360

; 325  : 
; 326  : 		if (nLen > 0) 

	cmp	DWORD PTR _nLen$73358[ebp], 0
	jle	SHORT $L73364

; 328  : 			LPWSTR pTo = sFile.GetBufferSetLength(nLen); 

	mov	ecx, DWORD PTR _nLen$73358[ebp]
	push	ecx
	mov	ecx, DWORD PTR _sFile$[ebp]
	call	?GetBufferSetLength@CString@@QAEPAGH@Z	; CString::GetBufferSetLength
	mov	DWORD PTR _pTo$73365[ebp], eax

; 329  : 			nLen = MultiByteToWideChar(m_nFileCodePage == -1 ? GetCurrentLocaleCodePage() : m_nFileCodePage, 0, (LPCSTR)pFileContents, dwTotalBytesRead, pTo, nLen);

	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+32], -1
	jne	SHORT $L74063
	call	?GetCurrentLocaleCodePage@CStdioFileEx@@SAIXZ ; CStdioFileEx::GetCurrentLocaleCodePage
	mov	DWORD PTR -64+[ebp], eax
	jmp	SHORT $L74064
$L74063:
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+32]
	mov	DWORD PTR -64+[ebp], ecx
$L74064:
	mov	esi, esp
	mov	edx, DWORD PTR _nLen$73358[ebp]
	push	edx
	mov	eax, DWORD PTR _pTo$73365[ebp]
	push	eax
	mov	ecx, DWORD PTR _dwTotalBytesRead$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pFileContents$[ebp]
	push	edx
	push	0
	mov	eax, DWORD PTR -64+[ebp]
	push	eax
	call	DWORD PTR __imp__MultiByteToWideChar@24
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nLen$73358[ebp], eax
$L73364:

; 332  : 
; 333  : #else // multibyte
; 334  : 
; 335  : 	if (m_bIsUnicodeText) // convert _to_ multibyte
; 336  : 	{
; 337  : 		ASSERT((dwTotalBytesRead % 2) == 0);
; 338  : 
; 339  : 		int nBytes = WideCharToMultiByte(GetCurrentLocaleCodePage(), 0, (LPCWSTR)pFileContents, dwTotalBytesRead / 2, NULL, 0, NULL, NULL);
; 340  : 		ASSERT(nBytes);
; 341  : 
; 342  : 		LPSTR pTo = new char[nBytes + 1];
; 343  : 		nBytes = WideCharToMultiByte(GetCurrentLocaleCodePage(), 0, (LPCWSTR)pFileContents, dwTotalBytesRead / 2, pTo, nBytes, NULL, NULL);
; 344  : 		pTo[nBytes] = 0; // null terminator
; 345  : 
; 346  : 		sFile = pTo;
; 347  : 		delete [] pTo;
; 348  : 	}
; 349  : 	else // byte stream is already multibyte format
; 350  : 	{
; 351  : 		LPSTR pTo = sFile.GetBufferSetLength(dwTotalBytesRead);
; 352  : 		CopyMemory(pTo, pFileContents, dwTotalBytesRead);
; 353  : 	}
; 354  : 
; 355  : #endif
; 356  : 
; 357  : 	delete [] pFileContents;

	mov	ecx, DWORD PTR _pFileContents$[ebp]
	mov	DWORD PTR $T74065[ebp], ecx
	mov	edx, DWORD PTR $T74065[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 358  : 	return !sFile.IsEmpty();

	mov	ecx, DWORD PTR _sFile$[ebp]
	call	?IsEmpty@CString@@QBEHXZ		; CString::IsEmpty
	neg	eax
	sbb	eax, eax
	inc	eax
$L73330:

; 359  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 64					; 00000040H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?ReadFile@CStdioFileEx@@QAEHAAVCString@@@Z ENDP		; CStdioFileEx::ReadFile
_TEXT	ENDS
PUBLIC	?WriteUnicodeLine@CStdioFileEx@@IAEIPBG@Z	; CStdioFileEx::WriteUnicodeLine
PUBLIC	?WriteMultiByteLine@CStdioFileEx@@IAEIPBG@Z	; CStdioFileEx::WriteMultiByteLine
EXTRN	?Write@CFile@@UAEXPBXI@Z:NEAR			; CFile::Write
;	COMDAT ?WriteString@CStdioFileEx@@UAEXPBG@Z
_TEXT	SEGMENT
_lpsz$ = 8
_this$ = -4
?WriteString@CStdioFileEx@@UAEXPBG@Z PROC NEAR		; CStdioFileEx::WriteString, COMDAT

; 382  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 383  : 	// Write Byte Order Mark if needed
; 384  : 	if (!m_pStream || GetPosition() == 0)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	je	SHORT $L73373
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+20]
	cmp	esi, esp
	call	__chkesp
	test	eax, eax
	jne	SHORT $L73378
$L73373:

; 386  : 		m_nBytesWritten = 0;

	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+12328], 0

; 387  : 
; 388  : 		if (m_bWriteBOM)

	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+37]
	test	edx, edx
	je	SHORT $L73378

; 390  : 			// If writing Unicode and at the start of the file, need to write byte mark
; 391  : 			if (m_bIsUnicodeText)

	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+20]
	test	ecx, ecx
	je	SHORT $L73375

; 393  : 				// If at position 0, write byte-order mark before writing anything else
; 394  : 				CFile::Write(&UNICODE_BOM, sizeof(wchar_t));

	push	2
	push	OFFSET FLAT:_UNICODE_BOM
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Write@CFile@@UAEXPBXI@Z		; CFile::Write

; 395  : 
; 396  : 				m_nBytesWritten += UNICODE_BOM_SIZE;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+12328]
	add	eax, 2
	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+12328], eax

; 398  : 			// Fix by philfar 
; 399  : 			// http://www.codeproject.com/file/stdiofileex.asp?forumid=15472&select=1805024&df=100#xx1805024xx
; 400  : 			// otherwise, if we are writing UTF-8 and at the start of the file, need to write UTF-8 byte mark
; 401  : 			else if (m_nFileCodePage == CP_UTF8)

	jmp	SHORT $L73378
$L73375:
	mov	edx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [edx+32], 65001		; 0000fde9H
	jne	SHORT $L73378

; 403  : 				// If at position 0, write byte-order mark before writing anything else
; 404  : 				CFile::Write(UTF8_BOM, UTF8_BOM_SIZE);

	push	3
	push	OFFSET FLAT:_UTF8_BOM
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Write@CFile@@UAEXPBXI@Z		; CFile::Write

; 405  : 
; 406  : 				m_nBytesWritten += UTF8_BOM_SIZE;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+12328]
	add	ecx, 3
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12328], ecx
$L73378:

; 410  : 
; 411  : 	// If writing Unicode...
; 412  : 	if (m_nFlags & CStdioFileEx::modeWriteUnicode)

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+28]
	and	ecx, 2097152				; 00200000H
	test	ecx, ecx
	je	SHORT $L73379

; 414  : 		m_nBytesWritten += WriteUnicodeLine(lpsz);

	mov	edx, DWORD PTR _lpsz$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?WriteUnicodeLine@CStdioFileEx@@IAEIPBG@Z ; CStdioFileEx::WriteUnicodeLine
	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+12328]
	add	edx, eax
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax+12328], edx

; 416  : 	// Else write multibyte/ANSI
; 417  : 	else

	jmp	SHORT $L73380
$L73379:

; 419  : 		m_nBytesWritten += WriteMultiByteLine(lpsz);

	mov	ecx, DWORD PTR _lpsz$[ebp]
	push	ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?WriteMultiByteLine@CStdioFileEx@@IAEIPBG@Z ; CStdioFileEx::WriteMultiByteLine
	mov	edx, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [edx+12328]
	add	ecx, eax
	mov	edx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [edx+12328], ecx
$L73380:

; 421  : }

	pop	esi
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?WriteString@CStdioFileEx@@UAEXPBG@Z ENDP		; CStdioFileEx::WriteString
_TEXT	ENDS
;	COMDAT ?ProcessSaveFlags@CStdioFileEx@@IAEIAAIW4SFE_SAVEAS@@@Z
_TEXT	SEGMENT
_nOpenFlags$ = 8
_nSaveAs$ = 12
_this$ = -4
?ProcessSaveFlags@CStdioFileEx@@IAEIAAIW4SFE_SAVEAS@@@Z PROC NEAR ; CStdioFileEx::ProcessSaveFlags, COMDAT

; 424  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 425  : 	m_bIsUnicodeText = false;

	mov	eax, DWORD PTR _this$[ebp]
	mov	BYTE PTR [eax+20], 0

; 426  : 
; 427  : 	// add unicode as required if saving
; 428  : 	if ((nOpenFlags & CFile::modeWrite) || (nOpenFlags & CFile::modeReadWrite))

	mov	ecx, DWORD PTR _nOpenFlags$[ebp]
	mov	edx, DWORD PTR [ecx]
	and	edx, 1
	test	edx, edx
	jne	SHORT $L73387
	mov	eax, DWORD PTR _nOpenFlags$[ebp]
	mov	ecx, DWORD PTR [eax]
	and	ecx, 2
	test	ecx, ecx
	je	SHORT $L73386
$L73387:

; 430  : 		if (WantUnicode(nSaveAs))

	mov	edx, DWORD PTR _nSaveAs$[ebp]
	push	edx
	call	?WantUnicode@CStdioFileEx@@KA_NW4SFE_SAVEAS@@@Z ; CStdioFileEx::WantUnicode
	add	esp, 4
	and	eax, 255				; 000000ffH
	test	eax, eax
	je	SHORT $L73388

; 432  : 			m_bWriteBOM = true;

	mov	eax, DWORD PTR _this$[ebp]
	mov	BYTE PTR [eax+37], 1

; 433  : 			nOpenFlags |= CStdioFileEx::modeWriteUnicode;

	mov	ecx, DWORD PTR _nOpenFlags$[ebp]
	mov	edx, DWORD PTR [ecx]
	or	edx, 2097152				; 00200000H
	mov	eax, DWORD PTR _nOpenFlags$[ebp]
	mov	DWORD PTR [eax], edx

; 434  : 			m_bIsUnicodeText = true;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [ecx+20], 1

; 436  : 		else 

	jmp	SHORT $L73389
$L73388:

; 438  : 			m_bWriteBOM = false;

	mov	edx, DWORD PTR _this$[ebp]
	mov	BYTE PTR [edx+37], 0

; 439  : 			nOpenFlags &= ~CStdioFileEx::modeWriteUnicode;

	mov	eax, DWORD PTR _nOpenFlags$[ebp]
	mov	ecx, DWORD PTR [eax]
	and	ecx, -2097153				; ffdfffffH
	mov	edx, DWORD PTR _nOpenFlags$[ebp]
	mov	DWORD PTR [edx], ecx

; 440  : 			m_bIsUnicodeText = false; // for completeness

	mov	eax, DWORD PTR _this$[ebp]
	mov	BYTE PTR [eax+20], 0
$L73389:

; 442  : 
; 443  : 		// must have readwrite access
; 444  : 		nOpenFlags &= ~CFile::modeWrite;

	mov	ecx, DWORD PTR _nOpenFlags$[ebp]
	mov	edx, DWORD PTR [ecx]
	and	edx, -2					; fffffffeH
	mov	eax, DWORD PTR _nOpenFlags$[ebp]
	mov	DWORD PTR [eax], edx

; 445  : 		nOpenFlags |= CFile::modeReadWrite;

	mov	ecx, DWORD PTR _nOpenFlags$[ebp]
	mov	edx, DWORD PTR [ecx]
	or	edx, 2
	mov	eax, DWORD PTR _nOpenFlags$[ebp]
	mov	DWORD PTR [eax], edx
$L73386:

; 447  : 
; 448  : 	// Always use binary mode, for any type of writing
; 449  : 	nOpenFlags &= ~CFile::typeText;

	mov	ecx, DWORD PTR _nOpenFlags$[ebp]
	mov	edx, DWORD PTR [ecx]
	and	dh, -65					; ffffffbfH
	mov	eax, DWORD PTR _nOpenFlags$[ebp]
	mov	DWORD PTR [eax], edx

; 450  : 	nOpenFlags |= CFile::typeBinary;

	mov	ecx, DWORD PTR _nOpenFlags$[ebp]
	mov	edx, DWORD PTR [ecx]
	or	dh, -128				; ffffff80H
	mov	eax, DWORD PTR _nOpenFlags$[ebp]
	mov	DWORD PTR [eax], edx

; 451  : 
; 452  : 	m_nFlags = nOpenFlags;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR _nOpenFlags$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR [ecx+28], eax

; 453  : 
; 454  : 	return nOpenFlags;

	mov	ecx, DWORD PTR _nOpenFlags$[ebp]
	mov	eax, DWORD PTR [ecx]

; 455  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?ProcessSaveFlags@CStdioFileEx@@IAEIAAIW4SFE_SAVEAS@@@Z ENDP ; CStdioFileEx::ProcessSaveFlags
_TEXT	ENDS
PUBLIC	?IsFileUnicode@CStdioFileEx@@SA_NABVCString@@@Z	; CStdioFileEx::IsFileUnicode
EXTRN	??0CFileException@@QAE@HJPBG@Z:NEAR		; CFileException::CFileException
EXTRN	??1CFileException@@UAE@XZ:NEAR			; CFileException::~CFileException
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0CFile@@QAE@XZ:NEAR				; CFile::CFile
EXTRN	?Open@CFile@@UAEHPBGIPAVCFileException@@@Z:NEAR	; CFile::Open
EXTRN	?Read@CFile@@UAEIPAXI@Z:NEAR			; CFile::Read
EXTRN	?Close@CFile@@UAEXXZ:NEAR			; CFile::Close
EXTRN	??1CFile@@UAE@XZ:NEAR				; CFile::~CFile
EXTRN	??BCString@@QBEPBGXZ:NEAR			; CString::operator unsigned short const *
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\StdioFileEx.cpp
xdata$x	SEGMENT
$T74078	DD	019930520H
	DD	02H
	DD	FLAT:$T74081
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74081	DD	0ffffffffH
	DD	FLAT:$L74073
	DD	00H
	DD	FLAT:$L74074
xdata$x	ENDS
;	COMDAT ?IsFileUnicode@CStdioFileEx@@SA_NABVCString@@@Z
_TEXT	SEGMENT
$T74072 = -64
__$EHRec$ = -12
_sFilePath$ = 8
_file$ = -60
_bIsUnicode$ = -44
_cFirstChar$ = -40
_exFile$ = -36
?IsFileUnicode@CStdioFileEx@@SA_NABVCString@@@Z PROC NEAR ; CStdioFileEx::IsFileUnicode, COMDAT

; 471  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74079
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 52					; 00000034H
	push	edi
	lea	edi, DWORD PTR [ebp-64]
	mov	ecx, 13					; 0000000dH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 472  : 	CFile				file;

	lea	ecx, DWORD PTR _file$[ebp]
	call	??0CFile@@QAE@XZ			; CFile::CFile
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 473  : 	bool				bIsUnicode = false;

	mov	BYTE PTR _bIsUnicode$[ebp], 0

; 474  : 	wchar_t			cFirstChar;
; 475  : 	CFileException	exFile;

	push	0
	push	-1
	push	0
	lea	ecx, DWORD PTR _exFile$[ebp]
	call	??0CFileException@@QAE@HJPBG@Z		; CFileException::CFileException
	mov	BYTE PTR __$EHRec$[ebp+8], 1

; 476  : 
; 477  : 	// Open file in binary mode and read first character
; 478  : 	if (file.Open(sFilePath, CFile::typeBinary | CFile::modeRead, &exFile))

	lea	eax, DWORD PTR _exFile$[ebp]
	push	eax
	push	32768					; 00008000H
	mov	ecx, DWORD PTR _sFilePath$[ebp]
	call	??BCString@@QBEPBGXZ			; CString::operator unsigned short const *
	push	eax
	lea	ecx, DWORD PTR _file$[ebp]
	call	?Open@CFile@@UAEHPBGIPAVCFileException@@@Z ; CFile::Open
	test	eax, eax
	je	SHORT $L73397

; 480  : 		// If byte is Unicode byte-order marker, let's say it's Unicode
; 481  : 		if ((file.Read(&cFirstChar, sizeof(wchar_t)) > 0) && (cFirstChar == (wchar_t)UNICODE_BOM))

	push	2
	lea	ecx, DWORD PTR _cFirstChar$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _file$[ebp]
	call	?Read@CFile@@UAEIPAXI@Z			; CFile::Read
	test	eax, eax
	jbe	SHORT $L73400
	mov	edx, DWORD PTR _cFirstChar$[ebp]
	and	edx, 65535				; 0000ffffH
	xor	eax, eax
	mov	ecx, OFFSET FLAT:_UNICODE_BOM
	mov	ax, cx
	cmp	edx, eax
	jne	SHORT $L73400

; 483  : 			bIsUnicode = true;

	mov	BYTE PTR _bIsUnicode$[ebp], 1
$L73400:

; 485  : 
; 486  : 		file.Close();

	lea	ecx, DWORD PTR _file$[ebp]
	call	?Close@CFile@@UAEXXZ			; CFile::Close
$L73397:

; 492  : 
; 493  : 	return bIsUnicode;

	mov	dl, BYTE PTR _bIsUnicode$[ebp]
	mov	BYTE PTR $T74072[ebp], dl
	mov	BYTE PTR __$EHRec$[ebp+8], 0
	lea	ecx, DWORD PTR _exFile$[ebp]
	call	??1CFileException@@UAE@XZ		; CFileException::~CFileException
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CFile@@UAE@XZ			; CFile::~CFile
	mov	al, BYTE PTR $T74072[ebp]

; 494  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	add	esp, 64					; 00000040H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74073:
	lea	ecx, DWORD PTR _file$[ebp]
	call	??1CFile@@UAE@XZ			; CFile::~CFile
	ret	0
$L74074:
	lea	ecx, DWORD PTR _exFile$[ebp]
	call	??1CFileException@@UAE@XZ		; CFileException::~CFileException
	ret	0
$L74079:
	mov	eax, OFFSET FLAT:$T74078
	jmp	___CxxFrameHandler
text$x	ENDS
?IsFileUnicode@CStdioFileEx@@SA_NABVCString@@@Z ENDP	; CStdioFileEx::IsFileUnicode
PUBLIC	?GetApproxCharCount@CStdioFileEx@@QAEKXZ	; CStdioFileEx::GetApproxCharCount
;	COMDAT ?GetApproxCharCount@CStdioFileEx@@QAEKXZ
_TEXT	SEGMENT
_this$ = -16
_nCharSize$ = -4
_nByteCount$ = -8
_nCharCount$ = -12
?GetApproxCharCount@CStdioFileEx@@QAEKXZ PROC NEAR	; CStdioFileEx::GetApproxCharCount, COMDAT

; 499  : {

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

; 500  : 	int			nCharSize;
; 501  : 	STDIOEXLONG	nByteCount, nCharCount = 0;

	mov	DWORD PTR _nCharCount$[ebp], 0

; 502  : 
; 503  : 	if (m_pStream)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+16], 0
	je	SHORT $L73409

; 505  : 		// Get size of chars in file
; 506  : 		nCharSize = m_bIsUnicodeText ? sizeof(wchar_t): sizeof(char);

	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+20]
	neg	edx
	sbb	edx, edx
	neg	edx
	add	edx, 1
	mov	DWORD PTR _nCharSize$[ebp], edx

; 507  : 
; 508  : 		// If Unicode, remove byte order mark from count
; 509  : 		nByteCount = GetLength();

	mov	eax, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [eax]
	mov	esi, esp
	mov	ecx, DWORD PTR _this$[ebp]
	call	DWORD PTR [edx+56]
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nByteCount$[ebp], eax

; 510  : 		
; 511  : 		if (m_bIsUnicodeText)

	mov	eax, DWORD PTR _this$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+20]
	test	ecx, ecx
	je	SHORT $L73412

; 513  : 			nByteCount = nByteCount - sizeof(wchar_t);

	mov	edx, DWORD PTR _nByteCount$[ebp]
	sub	edx, 2
	mov	DWORD PTR _nByteCount$[ebp], edx
$L73412:

; 515  : 
; 516  : 		// Calc chars
; 517  : 		nCharCount = (nByteCount / nCharSize);

	mov	eax, DWORD PTR _nByteCount$[ebp]
	xor	edx, edx
	div	DWORD PTR _nCharSize$[ebp]
	mov	DWORD PTR _nCharCount$[ebp], eax
$L73409:

; 519  : 
; 520  : 	return nCharCount;

	mov	eax, DWORD PTR _nCharCount$[ebp]

; 521  : }

	pop	esi
	add	esp, 16					; 00000010H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetApproxCharCount@CStdioFileEx@@QAEKXZ ENDP		; CStdioFileEx::GetApproxCharCount
_TEXT	ENDS
;	COMDAT ?Seek@CStdioFileEx@@UAEKKI@Z
_TEXT	SEGMENT
_lOff$ = 8
_nFrom$ = 12
_this$ = -4
?Seek@CStdioFileEx@@UAEKKI@Z PROC NEAR			; CStdioFileEx::Seek, COMDAT

; 524  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 525  : 	// Note that we should check the file position on the next read
; 526  : 	m_bCheckFilePos = true;

	mov	eax, DWORD PTR _this$[ebp]
	mov	BYTE PTR [eax+12326], 1

; 527  : 
; 528  : 	return CStdioFile::Seek((LONG)lOff, nFrom);

	mov	ecx, DWORD PTR _nFrom$[ebp]
	push	ecx
	mov	edx, DWORD PTR _lOff$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Seek@CStdioFile@@UAEJJI@Z		; CStdioFile::Seek

; 529  : }

	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Seek@CStdioFileEx@@UAEKKI@Z ENDP			; CStdioFileEx::Seek
_TEXT	ENDS
EXTRN	__imp__GetLocaleInfoW@16:NEAR
EXTRN	__imp___wtoi:NEAR
;	COMDAT ?GetCurrentLocaleCodePage@CStdioFileEx@@SAIXZ
_TEXT	SEGMENT
_szLocalCodePage$ = -24
_nLocaleCodePage$ = -4
_nLocaleChars$ = -28
?GetCurrentLocaleCodePage@CStdioFileEx@@SAIXZ PROC NEAR	; CStdioFileEx::GetCurrentLocaleCodePage, COMDAT

; 533  : {

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

; 534  : 	_TCHAR	szLocalCodePage[10];
; 535  : 	UINT		nLocaleCodePage = 0;

	mov	DWORD PTR _nLocaleCodePage$[ebp], 0

; 536  : 	int		nLocaleChars = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, szLocalCodePage, 10);

	mov	esi, esp
	push	10					; 0000000aH
	lea	eax, DWORD PTR _szLocalCodePage$[ebp]
	push	eax
	push	4100					; 00001004H
	push	1024					; 00000400H
	call	DWORD PTR __imp__GetLocaleInfoW@16
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nLocaleChars$[ebp], eax

; 537  : 
; 538  : 	// If OK
; 539  : 	if (nLocaleChars > 0)

	cmp	DWORD PTR _nLocaleChars$[ebp], 0
	jle	SHORT $L73432

; 541  : 		nLocaleCodePage = (UINT)_ttoi(szLocalCodePage);

	mov	esi, esp
	lea	ecx, DWORD PTR _szLocalCodePage$[ebp]
	push	ecx
	call	DWORD PTR __imp___wtoi
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nLocaleCodePage$[ebp], eax
$L73434:

; 542  : 		ASSERT(nLocaleCodePage > 0);

	cmp	DWORD PTR _nLocaleCodePage$[ebp], 0
	ja	SHORT $L73437
	push	542					; 0000021eH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73437
	int	3
$L73437:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L73434

; 544  : 	else

	jmp	SHORT $L73441
$L73432:

; 546  : 		ASSERT(false);

	mov	eax, 1
	test	eax, eax
	je	SHORT $L73442
	push	546					; 00000222H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73442
	int	3
$L73442:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73432
$L73441:

; 548  : 
; 549  : 	// O means either: no ANSI code page (Unicode-only locale?) or failed to get locale
; 550  : 	// In the case of Unicode-only locales, what do multibyte apps do? Answers on a postcard.
; 551  : 	return nLocaleCodePage;

	mov	eax, DWORD PTR _nLocaleCodePage$[ebp]

; 552  : }

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 28					; 0000001cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetCurrentLocaleCodePage@CStdioFileEx@@SAIXZ ENDP	; CStdioFileEx::GetCurrentLocaleCodePage
_TEXT	ENDS
PUBLIC	?GetUnicodeStringFromMultiByteString@CStdioFileEx@@SAHPBDPAGII@Z ; CStdioFileEx::GetUnicodeStringFromMultiByteString
EXTRN	__imp__GetACP@0:NEAR
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\StdioFileEx.cpp
xdata$x	SEGMENT
$T74096	DD	019930520H
	DD	02H
	DD	FLAT:$T74098
	DD	01H
	DD	FLAT:$T74099
	DD	2 DUP(00H)
	ORG $+4
$T74098	DD	0ffffffffH
	DD	00H
	DD	0ffffffffH
	DD	00H
$T74099	DD	00H
	DD	00H
	DD	01H
	DD	01H
	DD	FLAT:$T74100
	ORG $+4
$T74100	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L74094
xdata$x	ENDS
;	COMDAT ?GetUnicodeStringFromMultiByteString@CStdioFileEx@@SAHPBDPAGII@Z
_TEXT	SEGMENT
__$EHRec$ = -16
_szMultiByteString$ = 8
_szUnicodeString$ = 12
_nUnicodeBufferSize$ = 16
_nCodePage$ = 20
_nCharsWritten$ = -20
?GetUnicodeStringFromMultiByteString@CStdioFileEx@@SAHPBDPAGII@Z PROC NEAR ; CStdioFileEx::GetUnicodeStringFromMultiByteString, COMDAT

; 571  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74097
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR __$EHRec$[ebp], esp
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH

; 572  : 	int		nCharsWritten = 0;

	mov	DWORD PTR _nCharsWritten$[ebp], 0

; 573  : 		
; 574  : 	if (szUnicodeString && szMultiByteString)

	cmp	DWORD PTR _szUnicodeString$[ebp], 0
	je	SHORT $L73450
	cmp	DWORD PTR _szMultiByteString$[ebp], 0
	je	SHORT $L73450

; 576  : 		// If no code page specified, take default for system
; 577  : 		if (nCodePage == (UINT)-1)

	cmp	DWORD PTR _nCodePage$[ebp], -1
	jne	SHORT $L73452

; 579  : 			nCodePage = GetACP();

	mov	esi, esp
	call	DWORD PTR __imp__GetACP@0
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nCodePage$[ebp], eax
$L73452:

; 583  : 		{

	mov	DWORD PTR __$EHRec$[ebp+12], 0

; 584  : 			// Zero out buffer first
; 585  : 			memset((void*)szUnicodeString, '\0', sizeof(wchar_t) * nUnicodeBufferSize);

	mov	eax, DWORD PTR _nUnicodeBufferSize$[ebp]
	shl	eax, 1
	push	eax
	push	0
	mov	ecx, DWORD PTR _szUnicodeString$[ebp]
	push	ecx
	call	_memset
	add	esp, 12					; 0000000cH

; 586  : 
; 587  : 			// When converting to UTF8, don't set any flags (see Q175392).
; 588  : 			nCharsWritten = MultiByteToWideChar((UINT)nCodePage,(nCodePage==CP_UTF8 ?
; 589  : 				0:MB_PRECOMPOSED),szMultiByteString,-1,szUnicodeString, (int)nUnicodeBufferSize);

	mov	esi, esp
	mov	edx, DWORD PTR _nUnicodeBufferSize$[ebp]
	push	edx
	mov	eax, DWORD PTR _szUnicodeString$[ebp]
	push	eax
	push	-1
	mov	ecx, DWORD PTR _szMultiByteString$[ebp]
	push	ecx
	xor	edx, edx
	cmp	DWORD PTR _nCodePage$[ebp], 65001	; 0000fde9H
	setne	dl
	push	edx
	mov	eax, DWORD PTR _nCodePage$[ebp]
	push	eax
	call	DWORD PTR __imp__MultiByteToWideChar@24
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nCharsWritten$[ebp], eax

; 591  : 		catch(...)

	jmp	SHORT $L73453
$L74094:

; 592  : 		{
; 593  : 			// Level 4 compile says this is unreachable code in VS2005. I guess MultiByteToWideChar doesn't throw exceptions.
; 594  : //			TRACE(_T("Controlled exception in MultiByteToWideChar!\n"));
; 595  : 		}

	mov	eax, $L73453
	ret	0
$L73453:

; 596  : 	}

	mov	DWORD PTR __$EHRec$[ebp+12], -1

; 597  : 	else

	jmp	SHORT $L73461
$L73450:

; 599  : 		ASSERT(false);

	mov	ecx, 1
	test	ecx, ecx
	je	SHORT $L73462
	push	599					; 00000257H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73462
	int	3
$L73462:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L73450
$L73461:

; 601  : 
; 602  : 	// Konrad Windszus 29/3/2006: ASSERT in wrong place!
; 603  : 	// Should have at least the terminator (right?)
; 604  : 	ASSERT(nCharsWritten > 0);

	cmp	DWORD PTR _nCharsWritten$[ebp], 0
	jg	SHORT $L73466
	push	604					; 0000025cH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73466
	int	3
$L73466:
	xor	eax, eax
	test	eax, eax
	jne	SHORT $L73461

; 605  : 
; 606  : 	// Now fix nCharsWritten to exclude \0 terminator 
; 607  : 	if (nCharsWritten > 0)

	cmp	DWORD PTR _nCharsWritten$[ebp], 0
	jle	SHORT $L73467

; 609  : 		nCharsWritten--;

	mov	ecx, DWORD PTR _nCharsWritten$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _nCharsWritten$[ebp], ecx
$L73467:

; 611  : 
; 612  : 	return nCharsWritten;

	mov	eax, DWORD PTR _nCharsWritten$[ebp]

; 613  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp+4]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74097:
	mov	eax, OFFSET FLAT:$T74096
	jmp	___CxxFrameHandler
text$x	ENDS
?GetUnicodeStringFromMultiByteString@CStdioFileEx@@SAHPBDPAGII@Z ENDP ; CStdioFileEx::GetUnicodeStringFromMultiByteString
PUBLIC	?GetRequiredUnicodeLengthFromMultiByteString@CStdioFileEx@@SAHPBDI@Z ; CStdioFileEx::GetRequiredUnicodeLengthFromMultiByteString
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\StdioFileEx.cpp
xdata$x	SEGMENT
$T74108	DD	019930520H
	DD	02H
	DD	FLAT:$T74110
	DD	01H
	DD	FLAT:$T74111
	DD	2 DUP(00H)
	ORG $+4
$T74110	DD	0ffffffffH
	DD	00H
	DD	0ffffffffH
	DD	00H
$T74111	DD	00H
	DD	00H
	DD	01H
	DD	01H
	DD	FLAT:$T74112
	ORG $+4
$T74112	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L74106
xdata$x	ENDS
;	COMDAT ?GetRequiredUnicodeLengthFromMultiByteString@CStdioFileEx@@SAHPBDI@Z
_TEXT	SEGMENT
__$EHRec$ = -16
_szMultiByteString$ = 8
_nCodePage$ = 12
_nCharsNeeded$ = -20
?GetRequiredUnicodeLengthFromMultiByteString@CStdioFileEx@@SAHPBDI@Z PROC NEAR ; CStdioFileEx::GetRequiredUnicodeLengthFromMultiByteString, COMDAT

; 630  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74109
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR __$EHRec$[ebp], esp
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH

; 631  : 	int	nCharsNeeded = 0;

	mov	DWORD PTR _nCharsNeeded$[ebp], 0

; 632  : 		
; 633  : 	if (szMultiByteString)

	cmp	DWORD PTR _szMultiByteString$[ebp], 0
	je	SHORT $L73473

; 635  : 		// If no code page specified, take default for system
; 636  : 		if (nCodePage == (UINT)-1)

	cmp	DWORD PTR _nCodePage$[ebp], -1
	jne	SHORT $L73475

; 638  : 			nCodePage = GetACP();

	mov	esi, esp
	call	DWORD PTR __imp__GetACP@0
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nCodePage$[ebp], eax
$L73475:

; 642  : 		{

	mov	DWORD PTR __$EHRec$[ebp+12], 0

; 643  : 			// When converting to UTF8, don't set any flags (see Q175392).
; 644  : 			nCharsNeeded = MultiByteToWideChar((UINT)nCodePage,(nCodePage==CP_UTF8 ?
; 645  : 				0:MB_PRECOMPOSED),
; 646  : 				szMultiByteString,
; 647  : 				-1,	// Get the function to work out the length
; 648  : 				NULL, // No output buffer needed
; 649  : 				0		// Ask to be told how many chars we need (includes space for terminator)
; 650  : 				);

	mov	esi, esp
	push	0
	push	0
	push	-1
	mov	eax, DWORD PTR _szMultiByteString$[ebp]
	push	eax
	xor	ecx, ecx
	cmp	DWORD PTR _nCodePage$[ebp], 65001	; 0000fde9H
	setne	cl
	push	ecx
	mov	edx, DWORD PTR _nCodePage$[ebp]
	push	edx
	call	DWORD PTR __imp__MultiByteToWideChar@24
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nCharsNeeded$[ebp], eax

; 652  : 		catch(...)

	jmp	SHORT $L73476
$L74106:

; 653  : 		{
; 654  : 			// Level 4 compile says this is unreachable code in VS2005. I guess MultiByteToWideChar doesn't throw exceptions.
; 655  : //			TRACE(_T("Controlled exception in MultiByteToWideChar!\n"));
; 656  : 		}

	mov	eax, $L73476
	ret	0
$L73476:

; 657  : 	}

	mov	DWORD PTR __$EHRec$[ebp+12], -1

; 658  : 	else

	jmp	SHORT $L73481
$L73473:

; 660  : 		ASSERT(false);

	mov	eax, 1
	test	eax, eax
	je	SHORT $L73482
	push	660					; 00000294H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73482
	int	3
$L73482:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73473
$L73481:

; 662  : 
; 663  : 	// Konrad Windszus 29/3/2006: ASSERT in wrong place!
; 664  : 	// Should have at least the terminator (right?). The value returned by MultiByteToWideChar includes the terminator.
; 665  : 	ASSERT(nCharsNeeded > 0);

	cmp	DWORD PTR _nCharsNeeded$[ebp], 0
	jg	SHORT $L73486
	push	665					; 00000299H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73486
	int	3
$L73486:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L73481

; 666  : 
; 667  : 	return nCharsNeeded;

	mov	eax, DWORD PTR _nCharsNeeded$[ebp]

; 668  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp+4]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74109:
	mov	eax, OFFSET FLAT:$T74108
	jmp	___CxxFrameHandler
text$x	ENDS
?GetRequiredUnicodeLengthFromMultiByteString@CStdioFileEx@@SAHPBDI@Z ENDP ; CStdioFileEx::GetRequiredUnicodeLengthFromMultiByteString
PUBLIC	?GetNewUnicodeStringFromMultiByteString@CStdioFileEx@@SAHPBDAAV?$CTemplateSmartPtrArray@G@@HI@Z ; CStdioFileEx::GetNewUnicodeStringFromMultiByteString
PUBLIC	?Assign@?$CTemplateSmartPtrArray@G@@QAEXPAG_N@Z	; CTemplateSmartPtrArray<unsigned short>::Assign
PUBLIC	?GetBuffer@?$CTemplateSmartPtrArray@G@@QAEPAGXZ	; CTemplateSmartPtrArray<unsigned short>::GetBuffer
;	COMDAT ?GetNewUnicodeStringFromMultiByteString@CStdioFileEx@@SAHPBDAAV?$CTemplateSmartPtrArray@G@@HI@Z
_TEXT	SEGMENT
$T74117 = -12
_szMultiByteString$ = 8
_spUnicodeString$ = 12
_nDefaultBufferSize$ = 16
_nCodePage$ = 20
_nActualBufferSize$ = -8
_nUnicodeBufferSizeChars$ = -4
?GetNewUnicodeStringFromMultiByteString@CStdioFileEx@@SAHPBDAAV?$CTemplateSmartPtrArray@G@@HI@Z PROC NEAR ; CStdioFileEx::GetNewUnicodeStringFromMultiByteString, COMDAT

; 702  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 703  : 	int nActualBufferSize = nDefaultBufferSize;

	mov	eax, DWORD PTR _nDefaultBufferSize$[ebp]
	mov	DWORD PTR _nActualBufferSize$[ebp], eax

; 704  : 
; 705  : 	// Calculate the required buffer size and allocate
; 706  : 	int nUnicodeBufferSizeChars = GetRequiredUnicodeLengthFromMultiByteString(szMultiByteString, nCodePage);

	mov	ecx, DWORD PTR _nCodePage$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szMultiByteString$[ebp]
	push	edx
	call	?GetRequiredUnicodeLengthFromMultiByteString@CStdioFileEx@@SAHPBDI@Z ; CStdioFileEx::GetRequiredUnicodeLengthFromMultiByteString
	add	esp, 8
	mov	DWORD PTR _nUnicodeBufferSizeChars$[ebp], eax

; 707  : 
; 708  : 	// If we have enough in the default buffer, don't bother to allocate
; 709  : 	if (nUnicodeBufferSizeChars > nDefaultBufferSize)

	mov	eax, DWORD PTR _nUnicodeBufferSizeChars$[ebp]
	cmp	eax, DWORD PTR _nDefaultBufferSize$[ebp]
	jle	SHORT $L73495

; 711  : 		//pszUnicodeString = new wchar_t[nUnicodeBufferSizeChars];
; 712  : 
; 713  : 		// The new buffer will be marked as "owned" by the smart ptr by default, and therefore automatically deleted by the smart ptr
; 714  : 		spUnicodeString.Assign(new wchar_t[nUnicodeBufferSizeChars]);

	push	714					; 000002caH
	push	OFFSET FLAT:_THIS_FILE
	mov	ecx, DWORD PTR _nUnicodeBufferSizeChars$[ebp]
	shl	ecx, 1
	push	ecx
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T74117[ebp], eax
	push	1
	mov	edx, DWORD PTR $T74117[ebp]
	push	edx
	mov	ecx, DWORD PTR _spUnicodeString$[ebp]
	call	?Assign@?$CTemplateSmartPtrArray@G@@QAEXPAG_N@Z ; CTemplateSmartPtrArray<unsigned short>::Assign

; 715  : 
; 716  : 		nActualBufferSize = nUnicodeBufferSizeChars;

	mov	eax, DWORD PTR _nUnicodeBufferSizeChars$[ebp]
	mov	DWORD PTR _nActualBufferSize$[ebp], eax
$L73495:

; 718  : 
; 719  : 	// Call standard
; 720  : 	return GetUnicodeStringFromMultiByteString(szMultiByteString, spUnicodeString.GetBuffer(), nActualBufferSize, nCodePage);

	mov	ecx, DWORD PTR _nCodePage$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nActualBufferSize$[ebp]
	push	edx
	mov	ecx, DWORD PTR _spUnicodeString$[ebp]
	call	?GetBuffer@?$CTemplateSmartPtrArray@G@@QAEPAGXZ ; CTemplateSmartPtrArray<unsigned short>::GetBuffer
	push	eax
	mov	eax, DWORD PTR _szMultiByteString$[ebp]
	push	eax
	call	?GetUnicodeStringFromMultiByteString@CStdioFileEx@@SAHPBDPAGII@Z ; CStdioFileEx::GetUnicodeStringFromMultiByteString
	add	esp, 16					; 00000010H

; 721  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetNewUnicodeStringFromMultiByteString@CStdioFileEx@@SAHPBDAAV?$CTemplateSmartPtrArray@G@@HI@Z ENDP ; CStdioFileEx::GetNewUnicodeStringFromMultiByteString
_TEXT	ENDS
PUBLIC	?GetMultiByteStringFromUnicodeString@CStdioFileEx@@SAHPBGPADHID@Z ; CStdioFileEx::GetMultiByteStringFromUnicodeString
EXTRN	__imp__GetLastError@0:NEAR
EXTRN	__imp__WideCharToMultiByte@32:NEAR
EXTRN	?AfxTrace@@YAXPBGZZ:NEAR			; AfxTrace
;	COMDAT data
; File D:\_CODE\3rdParty\StdioFileEx.cpp
data	SEGMENT
$SG73551 DB	'E', 00H, 'r', 00H, 'r', 00H, 'o', 00H, 'r', 00H, ' ', 00H
	DB	'i', 00H, 'n', 00H, ' ', 00H, 'W', 00H, 'i', 00H, 'd', 00H, 'e'
	DB	00H, 'C', 00H, 'h', 00H, 'a', 00H, 'r', 00H, 'T', 00H, 'o', 00H
	DB	'M', 00H, 'u', 00H, 'l', 00H, 't', 00H, 'i', 00H, 'B', 00H, 'y'
	DB	00H, 't', 00H, 'e', 00H, ':', 00H, ' ', 00H, '%', 00H, 'd', 00H
	DB	0aH, 00H, 00H, 00H
$SG73552 DB	'C', 00H, 'o', 00H, 'n', 00H, 't', 00H, 'r', 00H, 'o', 00H
	DB	'l', 00H, 'l', 00H, 'e', 00H, 'd', 00H, ' ', 00H, 'e', 00H, 'x'
	DB	00H, 'c', 00H, 'e', 00H, 'p', 00H, 't', 00H, 'i', 00H, 'o', 00H
	DB	'n', 00H, ' ', 00H, 'i', 00H, 'n', 00H, ' ', 00H, 'W', 00H, 'i'
	DB	00H, 'd', 00H, 'e', 00H, 'C', 00H, 'h', 00H, 'a', 00H, 'r', 00H
	DB	'T', 00H, 'o', 00H, 'M', 00H, 'u', 00H, 'l', 00H, 't', 00H, 'i'
	DB	00H, 'B', 00H, 'y', 00H, 't', 00H, 'e', 00H, '!', 00H, 0aH, 00H
	DB	00H, 00H
$SG73553 DB	'%', 00H, 's', 00H, 00H, 00H
data	ENDS
;	COMDAT xdata$x
xdata$x	SEGMENT
$T74123	DD	019930520H
	DD	02H
	DD	FLAT:$T74125
	DD	01H
	DD	FLAT:$T74126
	DD	2 DUP(00H)
	ORG $+4
$T74125	DD	0ffffffffH
	DD	00H
	DD	0ffffffffH
	DD	00H
$T74126	DD	00H
	DD	00H
	DD	01H
	DD	01H
	DD	FLAT:$T74127
	ORG $+4
$T74127	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L74121
xdata$x	ENDS
;	COMDAT ?GetMultiByteStringFromUnicodeString@CStdioFileEx@@SAHPBGPADHID@Z
_TEXT	SEGMENT
__$EHRec$ = -16
_szUnicodeString$ = 8
_szMultiByteString$ = 12
_nMultiByteBufferSize$ = 16
_nCodePage$ = 20
_cFillerChar$ = 24
_bUsedDefChar$ = -24
_nBytesWritten$ = -20
?GetMultiByteStringFromUnicodeString@CStdioFileEx@@SAHPBGPADHID@Z PROC NEAR ; CStdioFileEx::GetMultiByteStringFromUnicodeString, COMDAT

; 746  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74124
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR __$EHRec$[ebp], esp
	mov	DWORD PTR [ebp-24], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH

; 747  : 	BOOL			bUsedDefChar	= FALSE;

	mov	DWORD PTR _bUsedDefChar$[ebp], 0

; 748  : 	int			nBytesWritten = 0;

	mov	DWORD PTR _nBytesWritten$[ebp], 0

; 749  : 
; 750  : 	// Fix by Andy Goodwin: don't do anything if buffer is 0
; 751  : 	if ( nMultiByteBufferSize > 0 )

	cmp	DWORD PTR _nMultiByteBufferSize$[ebp], 0
	jle	$L73542

; 753  : 		if (szUnicodeString && szMultiByteString) 

	cmp	DWORD PTR _szUnicodeString$[ebp], 0
	je	$L73542
	cmp	DWORD PTR _szMultiByteString$[ebp], 0
	je	$L73542

; 755  : 			// Zero out buffer first
; 756  : 			memset((void*)szMultiByteString, '\0', sizeof(char) * nMultiByteBufferSize);

	mov	eax, DWORD PTR _nMultiByteBufferSize$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _szMultiByteString$[ebp]
	push	ecx
	call	_memset
	add	esp, 12					; 0000000cH

; 757  : 
; 758  : 			// If no code page specified, take default for system
; 759  : 			if (nCodePage == (UINT)-1)

	cmp	DWORD PTR _nCodePage$[ebp], -1
	jne	SHORT $L73546

; 761  : 				nCodePage = (UINT)GetACP();

	mov	esi, esp
	call	DWORD PTR __imp__GetACP@0
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nCodePage$[ebp], eax
$L73546:

; 765  : 			{

	mov	DWORD PTR __$EHRec$[ebp+12], 0

; 766  : 				// If writing to UTF8, flags, default char and boolean flag must be NULL
; 767  : 				nBytesWritten = WideCharToMultiByte((UINT)nCodePage, 
; 768  : 					(nCodePage==CP_UTF8 ? 0 : WC_COMPOSITECHECK | WC_SEPCHARS), // Flags
; 769  : 					szUnicodeString,-1, 
; 770  : 					szMultiByteString, 
; 771  : 					nMultiByteBufferSize, 
; 772  : 					(nCodePage==CP_UTF8 ? NULL: &cFillerChar),		// Filler char
; 773  : 					(nCodePage==CP_UTF8 ? NULL: &bUsedDefChar));		// Did we use filler char?

	mov	edx, DWORD PTR _nCodePage$[ebp]
	sub	edx, 65001				; 0000fde9H
	neg	edx
	sbb	edx, edx
	lea	eax, DWORD PTR _bUsedDefChar$[ebp]
	and	edx, eax
	mov	esi, esp
	push	edx
	mov	ecx, DWORD PTR _nCodePage$[ebp]
	sub	ecx, 65001				; 0000fde9H
	neg	ecx
	sbb	ecx, ecx
	lea	edx, DWORD PTR _cFillerChar$[ebp]
	and	ecx, edx
	push	ecx
	mov	eax, DWORD PTR _nMultiByteBufferSize$[ebp]
	push	eax
	mov	ecx, DWORD PTR _szMultiByteString$[ebp]
	push	ecx
	push	-1
	mov	edx, DWORD PTR _szUnicodeString$[ebp]
	push	edx
	mov	eax, DWORD PTR _nCodePage$[ebp]
	sub	eax, 65001				; 0000fde9H
	neg	eax
	sbb	eax, eax
	and	eax, 544				; 00000220H
	push	eax
	mov	ecx, DWORD PTR _nCodePage$[ebp]
	push	ecx
	call	DWORD PTR __imp__WideCharToMultiByte@32
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nBytesWritten$[ebp], eax

; 774  : 
; 775  : 				// If no chars were written and the buffer is not 0, error!
; 776  : 				if (nBytesWritten == 0 && nMultiByteBufferSize > 0)

	cmp	DWORD PTR _nBytesWritten$[ebp], 0
	jne	SHORT $L73550
	cmp	DWORD PTR _nMultiByteBufferSize$[ebp], 0
	jle	SHORT $L73550

; 778  : 					TRACE1("Error in WideCharToMultiByte: %d\n", ::GetLastError());

	mov	esi, esp
	call	DWORD PTR __imp__GetLastError@0
	cmp	esi, esp
	call	__chkesp
	push	eax
	push	OFFSET FLAT:$SG73551
	call	?AfxTrace@@YAXPBGZZ			; AfxTrace
	add	esp, 8
$L73550:

; 781  : 			catch(...) 

	jmp	SHORT $L73548
$L74121:

; 782  : 			{
; 783  : 				TRACE0("Controlled exception in WideCharToMultiByte!\n");

	push	OFFSET FLAT:$SG73552
	push	OFFSET FLAT:$SG73553
	call	?AfxTrace@@YAXPBGZZ			; AfxTrace
	add	esp, 8

; 784  : 			}

	mov	eax, $L73548
	ret	0
$L73548:

; 785  : 		} 

	mov	DWORD PTR __$EHRec$[ebp+12], -1
$L73542:

; 787  : 
; 788  : 	// Now fix nCharsWritten to exclude \0 terminator 
; 789  : 	if (nBytesWritten > 0)

	cmp	DWORD PTR _nBytesWritten$[ebp], 0
	jle	SHORT $L73554

; 791  : 		nBytesWritten = nBytesWritten - sizeof('\0');

	mov	edx, DWORD PTR _nBytesWritten$[ebp]
	sub	edx, 1
	mov	DWORD PTR _nBytesWritten$[ebp], edx
$L73554:

; 793  : 
; 794  : 	return nBytesWritten;

	mov	eax, DWORD PTR _nBytesWritten$[ebp]

; 795  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp+4]
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
$L74124:
	mov	eax, OFFSET FLAT:$T74123
	jmp	___CxxFrameHandler
text$x	ENDS
?GetMultiByteStringFromUnicodeString@CStdioFileEx@@SAHPBGPADHID@Z ENDP ; CStdioFileEx::GetMultiByteStringFromUnicodeString
PUBLIC	?GetRequiredMultiByteLengthForUnicodeString@CStdioFileEx@@SAHPBGI@Z ; CStdioFileEx::GetRequiredMultiByteLengthForUnicodeString
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\StdioFileEx.cpp
xdata$x	SEGMENT
$T74136	DD	019930520H
	DD	02H
	DD	FLAT:$T74138
	DD	01H
	DD	FLAT:$T74139
	DD	2 DUP(00H)
	ORG $+4
$T74138	DD	0ffffffffH
	DD	00H
	DD	0ffffffffH
	DD	00H
$T74139	DD	00H
	DD	00H
	DD	01H
	DD	01H
	DD	FLAT:$T74140
	ORG $+4
$T74140	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L74134
xdata$x	ENDS
;	COMDAT ?GetRequiredMultiByteLengthForUnicodeString@CStdioFileEx@@SAHPBGI@Z
_TEXT	SEGMENT
__$EHRec$ = -16
_szUnicodeString$ = 8
_nCodePage$ = 12
_nBytesNeeded$ = -20
?GetRequiredMultiByteLengthForUnicodeString@CStdioFileEx@@SAHPBGI@Z PROC NEAR ; CStdioFileEx::GetRequiredMultiByteLengthForUnicodeString, COMDAT

; 813  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74137
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR __$EHRec$[ebp], esp
	mov	DWORD PTR [ebp-20], -858993460		; ccccccccH

; 814  : 	int nBytesNeeded = 0;

	mov	DWORD PTR _nBytesNeeded$[ebp], 0

; 815  : 
; 816  : 	try 
; 817  : 	{

	mov	DWORD PTR __$EHRec$[ebp+12], 0

; 818  : 		// If no code page specified, take default for system
; 819  : 		if (nCodePage == -1)

	cmp	DWORD PTR _nCodePage$[ebp], -1
	jne	SHORT $L73561

; 821  : 			nCodePage = GetACP();

	mov	esi, esp
	call	DWORD PTR __imp__GetACP@0
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nCodePage$[ebp], eax
$L73561:

; 826  : 			(nCodePage==CP_UTF8 ? 0 : WC_COMPOSITECHECK | WC_SEPCHARS), // Flags
; 827  : 			szUnicodeString,-1, 
; 828  : 			NULL, 
; 829  : 			0,	// Calculate required buffer, please! (Includes space for terminator)
; 830  : 			NULL,	// Filler char doesn't matter here
; 831  : 			NULL);

	mov	esi, esp
	push	0
	push	0
	push	0
	push	0
	push	-1
	mov	eax, DWORD PTR _szUnicodeString$[ebp]
	push	eax
	mov	ecx, DWORD PTR _nCodePage$[ebp]
	sub	ecx, 65001				; 0000fde9H
	neg	ecx
	sbb	ecx, ecx
	and	ecx, 544				; 00000220H
	push	ecx
	mov	edx, DWORD PTR _nCodePage$[ebp]
	push	edx
	call	DWORD PTR __imp__WideCharToMultiByte@32
	cmp	esi, esp
	call	__chkesp
	mov	DWORD PTR _nBytesNeeded$[ebp], eax

; 833  : 	catch(...) 

	jmp	SHORT $L73560
$L74134:

; 834  : 	{
; 835  : 		// TRACE(_T("Controlled exception in WideCharToMultiByte!\n"));
; 836  : 		// Gives us "unreachable code" error compiling on level 4
; 837  : 	}

	mov	eax, $L73560
	ret	0
$L73560:

; 838  : 
; 839  : 	return nBytesNeeded;

	mov	DWORD PTR __$EHRec$[ebp+12], -1
	mov	eax, DWORD PTR _nBytesNeeded$[ebp]

; 840  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp+4]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74137:
	mov	eax, OFFSET FLAT:$T74136
	jmp	___CxxFrameHandler
text$x	ENDS
?GetRequiredMultiByteLengthForUnicodeString@CStdioFileEx@@SAHPBGI@Z ENDP ; CStdioFileEx::GetRequiredMultiByteLengthForUnicodeString
PUBLIC	?GetNewMultiByteStringFromUnicodeString@CStdioFileEx@@SAHPBGAAV?$CTemplateSmartPtrArray@D@@HID@Z ; CStdioFileEx::GetNewMultiByteStringFromUnicodeString
PUBLIC	?Assign@?$CTemplateSmartPtrArray@D@@QAEXPAD_N@Z	; CTemplateSmartPtrArray<char>::Assign
PUBLIC	?GetBuffer@?$CTemplateSmartPtrArray@D@@QAEPADXZ	; CTemplateSmartPtrArray<char>::GetBuffer
;	COMDAT ?GetNewMultiByteStringFromUnicodeString@CStdioFileEx@@SAHPBGAAV?$CTemplateSmartPtrArray@D@@HID@Z
_TEXT	SEGMENT
$T74145 = -12
_szUnicodeString$ = 8
_spMultiByteString$ = 12
_nDefaultBufferSize$ = 16
_nCodePage$ = 20
_cFillerChar$ = 24
_nActualBufferSize$ = -8
_nMultibyteBufferSizeBytes$ = -4
?GetNewMultiByteStringFromUnicodeString@CStdioFileEx@@SAHPBGAAV?$CTemplateSmartPtrArray@D@@HID@Z PROC NEAR ; CStdioFileEx::GetNewMultiByteStringFromUnicodeString, COMDAT

; 877  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	DWORD PTR [ebp-12], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH

; 878  : 	int nActualBufferSize = nDefaultBufferSize;

	mov	eax, DWORD PTR _nDefaultBufferSize$[ebp]
	mov	DWORD PTR _nActualBufferSize$[ebp], eax

; 879  : 
; 880  : 	// Calculate the required buffer size and allocate
; 881  : 	int nMultibyteBufferSizeBytes = GetRequiredMultiByteLengthForUnicodeString(szUnicodeString, nCodePage);

	mov	ecx, DWORD PTR _nCodePage$[ebp]
	push	ecx
	mov	edx, DWORD PTR _szUnicodeString$[ebp]
	push	edx
	call	?GetRequiredMultiByteLengthForUnicodeString@CStdioFileEx@@SAHPBGI@Z ; CStdioFileEx::GetRequiredMultiByteLengthForUnicodeString
	add	esp, 8
	mov	DWORD PTR _nMultibyteBufferSizeBytes$[ebp], eax

; 882  : 
; 883  : 	// If we have enough in the default buffer, don't bother to allocate
; 884  : 	if (nMultibyteBufferSizeBytes > nDefaultBufferSize)

	mov	eax, DWORD PTR _nMultibyteBufferSizeBytes$[ebp]
	cmp	eax, DWORD PTR _nDefaultBufferSize$[ebp]
	jle	SHORT $L73572

; 886  : 		// The new buffer will be marked as "owned" by the smart ptr by default, and therefore automatically deleted by the smart ptr
; 887  : 		spMultiByteString.Assign(new char[nMultibyteBufferSizeBytes]);

	push	887					; 00000377H
	push	OFFSET FLAT:_THIS_FILE
	mov	ecx, DWORD PTR _nMultibyteBufferSizeBytes$[ebp]
	push	ecx
	call	??2@YAPAXIPBDH@Z			; operator new
	add	esp, 12					; 0000000cH
	mov	DWORD PTR $T74145[ebp], eax
	push	1
	mov	edx, DWORD PTR $T74145[ebp]
	push	edx
	mov	ecx, DWORD PTR _spMultiByteString$[ebp]
	call	?Assign@?$CTemplateSmartPtrArray@D@@QAEXPAD_N@Z ; CTemplateSmartPtrArray<char>::Assign

; 888  : 
; 889  : 		nActualBufferSize = nMultibyteBufferSizeBytes;

	mov	eax, DWORD PTR _nMultibyteBufferSizeBytes$[ebp]
	mov	DWORD PTR _nActualBufferSize$[ebp], eax
$L73572:

; 891  : 
; 892  : 	// Call standard
; 893  : 	return GetMultiByteStringFromUnicodeString(szUnicodeString, spMultiByteString.GetBuffer(), nActualBufferSize, nCodePage, cFillerChar);

	mov	cl, BYTE PTR _cFillerChar$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nCodePage$[ebp]
	push	edx
	mov	eax, DWORD PTR _nActualBufferSize$[ebp]
	push	eax
	mov	ecx, DWORD PTR _spMultiByteString$[ebp]
	call	?GetBuffer@?$CTemplateSmartPtrArray@D@@QAEPADXZ ; CTemplateSmartPtrArray<char>::GetBuffer
	push	eax
	mov	ecx, DWORD PTR _szUnicodeString$[ebp]
	push	ecx
	call	?GetMultiByteStringFromUnicodeString@CStdioFileEx@@SAHPBGPADHID@Z ; CStdioFileEx::GetMultiByteStringFromUnicodeString
	add	esp, 20					; 00000014H

; 894  : }

	add	esp, 12					; 0000000cH
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetNewMultiByteStringFromUnicodeString@CStdioFileEx@@SAHPBGAAV?$CTemplateSmartPtrArray@D@@HID@Z ENDP ; CStdioFileEx::GetNewMultiByteStringFromUnicodeString
_TEXT	ENDS
PUBLIC	?fggets@@YAHPAPAGPAU_iobuf@@AA_NH@Z		; fggets
EXTRN	__CxxThrowException@8:NEAR
EXTRN	??0CString@@QAE@PBG@Z:NEAR			; CString::CString
EXTRN	?Empty@CString@@QAEXXZ:NEAR			; CString::Empty
EXTRN	??4CString@@QAEABV0@ABV0@@Z:NEAR		; CString::operator=
EXTRN	??1CString@@QAE@XZ:NEAR				; CString::~CString
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\StdioFileEx.cpp
xdata$x	SEGMENT
$T74156	DD	019930520H
	DD	03H
	DD	FLAT:$T74158
	DD	01H
	DD	FLAT:$T74159
	DD	2 DUP(00H)
	ORG $+4
$T74158	DD	0ffffffffH
	DD	00H
	DD	00H
	DD	FLAT:$L74154
	DD	0ffffffffH
	DD	00H
$T74159	DD	00H
	DD	01H
	DD	02H
	DD	01H
	DD	FLAT:$T74160
	ORG $+4
$T74160	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L74153
xdata$x	ENDS
;	COMDAT ?ReadUnicodeLine@CStdioFileEx@@IAEHAAVCString@@@Z
_TEXT	SEGMENT
$T74148 = -32
$T74149 = -36
$T74151 = -40
__$EHRec$ = -16
_sOutputLine$ = 8
_this$ = -44
_bReadData$ = -24
_pszUnicodeString$ = -28
_bNeedToDelete$ = -20
?ReadUnicodeLine@CStdioFileEx@@IAEHAAVCString@@@Z PROC NEAR ; CStdioFileEx::ReadUnicodeLine, COMDAT

; 971  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74157
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	sub	esp, 40					; 00000028H
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR __$EHRec$[ebp], esp
	push	ecx
	lea	edi, DWORD PTR [ebp-56]
	mov	ecx, 10					; 0000000aH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 972  : 	BOOL			bReadData = FALSE;

	mov	DWORD PTR _bReadData$[ebp], 0

; 973  : 	wchar_t*		pszUnicodeString = (wchar_t*)&m_arrUnicodeDefaultBuffer;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 38					; 00000026H
	mov	DWORD PTR _pszUnicodeString$[ebp], eax

; 974  : 	bool			bNeedToDelete = false;

	mov	BYTE PTR _bNeedToDelete$[ebp], 0

; 975  : 
; 976  : 	try
; 977  : 	{

	mov	DWORD PTR __$EHRec$[ebp+12], 0

; 978  : 		bReadData = (FGGETS_OK == fggets<wchar_t>(&pszUnicodeString, m_pStream, bNeedToDelete, nSTDIOFILEEX_DEFAULT_BUFFER_SIZE));

	push	4096					; 00001000H
	lea	ecx, DWORD PTR _bNeedToDelete$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+16]
	push	eax
	lea	ecx, DWORD PTR _pszUnicodeString$[ebp]
	push	ecx
	call	?fggets@@YAHPAPAGPAU_iobuf@@AA_NH@Z	; fggets
	add	esp, 16					; 00000010H
	mov	DWORD PTR -48+[ebp], eax
	xor	edx, edx
	cmp	DWORD PTR -48+[ebp], 0
	sete	dl
	mov	DWORD PTR _bReadData$[ebp], edx

; 979  : 
; 980  : 		if (bReadData)

	cmp	DWORD PTR _bReadData$[ebp], 0
	je	SHORT $L73632

; 982  : 			sOutputLine = (CString)pszUnicodeString;

	mov	eax, DWORD PTR _pszUnicodeString$[ebp]
	push	eax
	lea	ecx, DWORD PTR $T74148[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR -52+[ebp], eax
	mov	ecx, DWORD PTR -52+[ebp]
	mov	DWORD PTR -56+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+12], 1
	mov	edx, DWORD PTR -56+[ebp]
	push	edx
	mov	ecx, DWORD PTR _sOutputLine$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+12], 0
	lea	ecx, DWORD PTR $T74148[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString

; 984  : 		else

	jmp	SHORT $L73635
$L73632:

; 986  : 			sOutputLine.Empty();

	mov	ecx, DWORD PTR _sOutputLine$[ebp]
	call	?Empty@CString@@QAEXXZ			; CString::Empty
$L73635:

; 989  : 	// Ensure we always clean up, no matter what
; 990  : 	FINALLY( if (bNeedToDelete) DELETE_SAFE_ARRAY(pszUnicodeString); )

	jmp	SHORT $L73617
$L74153:
$L73636:
	mov	eax, 1
	test	eax, eax
	je	SHORT $L73639
	push	990					; 000003deH
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73639
	int	3
$L73639:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73636
	mov	edx, DWORD PTR _bNeedToDelete$[ebp]
	and	edx, 255				; 000000ffH
	test	edx, edx
	je	SHORT $L73641
	cmp	DWORD PTR _pszUnicodeString$[ebp], 0
	je	SHORT $L73641
	mov	eax, DWORD PTR _pszUnicodeString$[ebp]
	mov	DWORD PTR $T74149[ebp], eax
	mov	ecx, DWORD PTR $T74149[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
	mov	DWORD PTR _pszUnicodeString$[ebp], 0
$L73641:
	push	0
	push	0
	call	__CxxThrowException@8
$L73617:
	mov	DWORD PTR __$EHRec$[ebp+12], -1
	mov	edx, DWORD PTR _bNeedToDelete$[ebp]
	and	edx, 255				; 000000ffH
	test	edx, edx
	je	SHORT $L73644
	cmp	DWORD PTR _pszUnicodeString$[ebp], 0
	je	SHORT $L73644
	mov	eax, DWORD PTR _pszUnicodeString$[ebp]
	mov	DWORD PTR $T74151[ebp], eax
	mov	ecx, DWORD PTR $T74151[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
	mov	DWORD PTR _pszUnicodeString$[ebp], 0
$L73644:

; 991  : 
; 992  : 	return bReadData;

	mov	eax, DWORD PTR _bReadData$[ebp]
$L73612:

; 993  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp+4]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 56					; 00000038H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74154:
	lea	ecx, DWORD PTR $T74148[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74157:
	mov	eax, OFFSET FLAT:$T74156
	jmp	___CxxFrameHandler
text$x	ENDS
?ReadUnicodeLine@CStdioFileEx@@IAEHAAVCString@@@Z ENDP	; CStdioFileEx::ReadUnicodeLine
PUBLIC	??0?$CTemplateSmartPtrArray@G@@QAE@PAG_N@Z	; CTemplateSmartPtrArray<unsigned short>::CTemplateSmartPtrArray<unsigned short>
PUBLIC	??1?$CTemplateSmartPtrArray@G@@UAE@XZ		; CTemplateSmartPtrArray<unsigned short>::~CTemplateSmartPtrArray<unsigned short>
PUBLIC	?fggets@@YAHPAPADPAU_iobuf@@AA_NH@Z		; fggets
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\StdioFileEx.cpp
xdata$x	SEGMENT
$T74173	DD	019930520H
	DD	04H
	DD	FLAT:$T74175
	DD	01H
	DD	FLAT:$T74176
	DD	2 DUP(00H)
	ORG $+4
$T74175	DD	0ffffffffH
	DD	00H
	DD	00H
	DD	FLAT:$L74170
	DD	01H
	DD	FLAT:$L74171
	DD	0ffffffffH
	DD	00H
$T74176	DD	00H
	DD	02H
	DD	03H
	DD	01H
	DD	FLAT:$T74177
	ORG $+4
$T74177	DD	00H
	DD	00H
	DD	00H
	DD	FLAT:$L74169
xdata$x	ENDS
;	COMDAT ?ReadMultiByteLine@CStdioFileEx@@IAEHAAVCString@@@Z
_TEXT	SEGMENT
$T74165 = -48
$T74166 = -52
$T74167 = -56
__$EHRec$ = -16
_sOutputLine$ = 8
_this$ = -60
_bReadData$ = -28
_pszMultiByteString$ = -20
_nChars$ = -32
_bNeedToDelete$ = -24
_spUnicodeString$73672 = -44
?ReadMultiByteLine@CStdioFileEx@@IAEHAAVCString@@@Z PROC NEAR ; CStdioFileEx::ReadMultiByteLine, COMDAT

; 997  : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74174
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	sub	esp, 68					; 00000044H
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR __$EHRec$[ebp], esp
	push	ecx
	lea	edi, DWORD PTR [ebp-84]
	mov	ecx, 17					; 00000011H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	pop	ecx
	mov	DWORD PTR _this$[ebp], ecx

; 998  : 	BOOL			bReadData = FALSE;

	mov	DWORD PTR _bReadData$[ebp], 0

; 999  : 	char	*		pszMultiByteString = (char*)&m_arrMultibyteDefaultBuffer;;

	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 8230				; 00002026H
	mov	DWORD PTR _pszMultiByteString$[ebp], eax

; 1000 : 	int			nChars = 0;

	mov	DWORD PTR _nChars$[ebp], 0

; 1001 : 	bool			bNeedToDelete = false;

	mov	BYTE PTR _bNeedToDelete$[ebp], 0

; 1002 : 
; 1003 : 	try
; 1004 : 	{

	mov	DWORD PTR __$EHRec$[ebp+12], 0

; 1005 : 		// Read the string -- the function dynamically allocates the necessary memory according
; 1006 : 		// to the line length
; 1007 : 		bReadData = (FGGETS_OK == fggets<char>(&pszMultiByteString, m_pStream, bNeedToDelete, nSTDIOFILEEX_DEFAULT_BUFFER_SIZE));

	push	4096					; 00001000H
	lea	ecx, DWORD PTR _bNeedToDelete$[ebp]
	push	ecx
	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+16]
	push	eax
	lea	ecx, DWORD PTR _pszMultiByteString$[ebp]
	push	ecx
	call	?fggets@@YAHPAPADPAU_iobuf@@AA_NH@Z	; fggets
	add	esp, 16					; 00000010H
	mov	DWORD PTR -64+[ebp], eax
	xor	edx, edx
	cmp	DWORD PTR -64+[ebp], 0
	sete	dl
	mov	DWORD PTR _bReadData$[ebp], edx

; 1008 : 
; 1009 : 		if (bReadData)

	cmp	DWORD PTR _bReadData$[ebp], 0
	je	$L73670

; 1011 : 			// Assign default buffer to take care of 99% of cases. Not owned by smart ptr array, so won't be deleted
; 1012 : 			CTemplateSmartPtrArray<wchar_t>	spUnicodeString((wchar_t*)&m_arrUnicodeDefaultBuffer, bTEMPLATESMARTPTR_NOTOWNED);

	push	0
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 38					; 00000026H
	push	eax
	lea	ecx, DWORD PTR _spUnicodeString$73672[ebp]
	call	??0?$CTemplateSmartPtrArray@G@@QAE@PAG_N@Z ; CTemplateSmartPtrArray<unsigned short>::CTemplateSmartPtrArray<unsigned short>
	mov	DWORD PTR -68+[ebp], eax
	mov	BYTE PTR __$EHRec$[ebp+12], 1

; 1013 : 
; 1014 : 			// Use all-in-one allocation and conversion function. Avoid _mbslen and such like since they're unreliable
; 1015 : 			// with UTF8
; 1016 : 			nChars = GetNewUnicodeStringFromMultiByteString(pszMultiByteString, spUnicodeString, nSTDIOFILEEX_DEFAULT_BUFFER_SIZE, m_nFileCodePage);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+32]
	push	edx
	push	4096					; 00001000H
	lea	eax, DWORD PTR _spUnicodeString$73672[ebp]
	push	eax
	mov	ecx, DWORD PTR _pszMultiByteString$[ebp]
	push	ecx
	call	?GetNewUnicodeStringFromMultiByteString@CStdioFileEx@@SAHPBDAAV?$CTemplateSmartPtrArray@G@@HI@Z ; CStdioFileEx::GetNewUnicodeStringFromMultiByteString
	add	esp, 16					; 00000010H
	mov	DWORD PTR -72+[ebp], eax
	mov	edx, DWORD PTR -72+[ebp]
	mov	DWORD PTR _nChars$[ebp], edx

; 1017 : 
; 1018 : 			if (nChars > 0)

	cmp	DWORD PTR _nChars$[ebp], 0
	jle	SHORT $L73673

; 1020 : 				sOutputLine = (CString)spUnicodeString.GetBuffer();

	lea	ecx, DWORD PTR _spUnicodeString$73672[ebp]
	call	?GetBuffer@?$CTemplateSmartPtrArray@G@@QAEPAGXZ ; CTemplateSmartPtrArray<unsigned short>::GetBuffer
	mov	DWORD PTR -76+[ebp], eax
	mov	eax, DWORD PTR -76+[ebp]
	push	eax
	lea	ecx, DWORD PTR $T74165[ebp]
	call	??0CString@@QAE@PBG@Z			; CString::CString
	mov	DWORD PTR -80+[ebp], eax
	mov	ecx, DWORD PTR -80+[ebp]
	mov	DWORD PTR -84+[ebp], ecx
	mov	BYTE PTR __$EHRec$[ebp+12], 2
	mov	edx, DWORD PTR -84+[ebp]
	push	edx
	mov	ecx, DWORD PTR _sOutputLine$[ebp]
	call	??4CString@@QAEABV0@ABV0@@Z		; CString::operator=
	mov	BYTE PTR __$EHRec$[ebp+12], 1
	lea	ecx, DWORD PTR $T74165[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
$L73673:

; 1022 : 		}

	mov	BYTE PTR __$EHRec$[ebp+12], 0
	lea	ecx, DWORD PTR _spUnicodeString$73672[ebp]
	call	??1?$CTemplateSmartPtrArray@G@@UAE@XZ	; CTemplateSmartPtrArray<unsigned short>::~CTemplateSmartPtrArray<unsigned short>
$L73670:

; 1023 : 		
; 1024 : 		// Empty the string if we failed to read anything
; 1025 : 		if (nChars == 0)

	cmp	DWORD PTR _nChars$[ebp], 0
	jne	SHORT $L73676

; 1027 : 			sOutputLine.Empty();

	mov	ecx, DWORD PTR _sOutputLine$[ebp]
	call	?Empty@CString@@QAEXXZ			; CString::Empty
$L73676:

; 1030 : 	// Ensure we always clean up, no matter what
; 1031 : 	FINALLY( if (bNeedToDelete) DELETE_SAFE_ARRAY(pszMultiByteString); )

	jmp	SHORT $L73655
$L74169:
$L73677:
	mov	eax, 1
	test	eax, eax
	je	SHORT $L73680
	push	1031					; 00000407H
	push	OFFSET FLAT:_THIS_FILE
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73680
	int	3
$L73680:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73677
	mov	edx, DWORD PTR _bNeedToDelete$[ebp]
	and	edx, 255				; 000000ffH
	test	edx, edx
	je	SHORT $L73682
	cmp	DWORD PTR _pszMultiByteString$[ebp], 0
	je	SHORT $L73682
	mov	eax, DWORD PTR _pszMultiByteString$[ebp]
	mov	DWORD PTR $T74166[ebp], eax
	mov	ecx, DWORD PTR $T74166[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
	mov	DWORD PTR _pszMultiByteString$[ebp], 0
$L73682:
	push	0
	push	0
	call	__CxxThrowException@8
$L73655:
	mov	DWORD PTR __$EHRec$[ebp+12], -1
	mov	edx, DWORD PTR _bNeedToDelete$[ebp]
	and	edx, 255				; 000000ffH
	test	edx, edx
	je	SHORT $L73685
	cmp	DWORD PTR _pszMultiByteString$[ebp], 0
	je	SHORT $L73685
	mov	eax, DWORD PTR _pszMultiByteString$[ebp]
	mov	DWORD PTR $T74167[ebp], eax
	mov	ecx, DWORD PTR $T74167[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
	mov	DWORD PTR _pszMultiByteString$[ebp], 0
$L73685:

; 1032 : 
; 1033 : 	return bReadData;

	mov	eax, DWORD PTR _bReadData$[ebp]
$L73649:

; 1034 : }

	mov	ecx, DWORD PTR __$EHRec$[ebp+4]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	pop	ebx
	add	esp, 84					; 00000054H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L74170:
	lea	ecx, DWORD PTR _spUnicodeString$73672[ebp]
	call	??1?$CTemplateSmartPtrArray@G@@UAE@XZ	; CTemplateSmartPtrArray<unsigned short>::~CTemplateSmartPtrArray<unsigned short>
	ret	0
$L74171:
	lea	ecx, DWORD PTR $T74165[ebp]
	call	??1CString@@QAE@XZ			; CString::~CString
	ret	0
$L74174:
	mov	eax, OFFSET FLAT:$T74173
	jmp	___CxxFrameHandler
text$x	ENDS
?ReadMultiByteLine@CStdioFileEx@@IAEHAAVCString@@@Z ENDP ; CStdioFileEx::ReadMultiByteLine
EXTRN	__imp__wcslen:NEAR
;	COMDAT ?WriteUnicodeLine@CStdioFileEx@@IAEIPBG@Z
_TEXT	SEGMENT
_sInputLine$ = 8
_this$ = -8
_nLenBytes$ = -4
?WriteUnicodeLine@CStdioFileEx@@IAEIPBG@Z PROC NEAR	; CStdioFileEx::WriteUnicodeLine, COMDAT

; 1154 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	push	esi
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 1155 : 	// Write in byte mode
; 1156 : 	UINT nLenBytes = (UINT)(wcslen(sInputLine) * sizeof(wchar_t));

	mov	esi, esp
	mov	eax, DWORD PTR _sInputLine$[ebp]
	push	eax
	call	DWORD PTR __imp__wcslen
	add	esp, 4
	cmp	esi, esp
	call	__chkesp
	shl	eax, 1
	mov	DWORD PTR _nLenBytes$[ebp], eax

; 1157 : 	CFile::Write(sInputLine, nLenBytes);

	mov	ecx, DWORD PTR _nLenBytes$[ebp]
	push	ecx
	mov	edx, DWORD PTR _sInputLine$[ebp]
	push	edx
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Write@CFile@@UAEXPBXI@Z		; CFile::Write

; 1158 : 
; 1159 : 	return nLenBytes;

	mov	eax, DWORD PTR _nLenBytes$[ebp]

; 1160 : }

	pop	esi
	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
?WriteUnicodeLine@CStdioFileEx@@IAEIPBG@Z ENDP		; CStdioFileEx::WriteUnicodeLine
_TEXT	ENDS
PUBLIC	??0?$CTemplateSmartPtrArray@D@@QAE@PAD_N@Z	; CTemplateSmartPtrArray<char>::CTemplateSmartPtrArray<char>
PUBLIC	??1?$CTemplateSmartPtrArray@D@@UAE@XZ		; CTemplateSmartPtrArray<char>::~CTemplateSmartPtrArray<char>
;	COMDAT xdata$x
; File D:\_CODE\3rdParty\StdioFileEx.cpp
xdata$x	SEGMENT
$T74187	DD	019930520H
	DD	01H
	DD	FLAT:$T74189
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T74189	DD	0ffffffffH
	DD	FLAT:$L74185
xdata$x	ENDS
;	COMDAT ?WriteMultiByteLine@CStdioFileEx@@IAEIPBG@Z
_TEXT	SEGMENT
$T74184 = -32
__$EHRec$ = -12
_sInputLine$ = 8
_this$ = -36
_nCharsWritten$ = -28
_spMultiByteString$ = -24
?WriteMultiByteLine@CStdioFileEx@@IAEIPBG@Z PROC NEAR	; CStdioFileEx::WriteMultiByteLine, COMDAT

; 1164 : {

	push	ebp
	mov	ebp, esp
	push	-1
	push	$L74188
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

; 1165 : 	int			nCharsWritten = 0;

	mov	DWORD PTR _nCharsWritten$[ebp], 0

; 1166 : 
; 1167 : 	// Assign default buffer to take care of 99% of cases. Not owned by smart ptr array, so won't be deleted
; 1168 : 	CTemplateSmartPtrArray<char>	spMultiByteString((char*)&m_arrMultibyteDefaultBuffer, bTEMPLATESMARTPTR_NOTOWNED);

	push	0
	mov	eax, DWORD PTR _this$[ebp]
	add	eax, 8230				; 00002026H
	push	eax
	lea	ecx, DWORD PTR _spMultiByteString$[ebp]
	call	??0?$CTemplateSmartPtrArray@D@@QAE@PAD_N@Z ; CTemplateSmartPtrArray<char>::CTemplateSmartPtrArray<char>
	mov	DWORD PTR __$EHRec$[ebp+8], 0

; 1169 : 
; 1170 : 	// Call all-in-one function to calculate required buffer size and allocate
; 1171 : 	nCharsWritten = GetNewMultiByteStringFromUnicodeString(sInputLine, spMultiByteString, nSTDIOFILEEX_DEFAULT_BUFFER_SIZE, m_nFileCodePage, m_cUnicodeFillerChar);

	mov	ecx, DWORD PTR _this$[ebp]
	mov	dl, BYTE PTR [ecx+36]
	push	edx
	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+32]
	push	ecx
	push	4096					; 00001000H
	lea	edx, DWORD PTR _spMultiByteString$[ebp]
	push	edx
	mov	eax, DWORD PTR _sInputLine$[ebp]
	push	eax
	call	?GetNewMultiByteStringFromUnicodeString@CStdioFileEx@@SAHPBGAAV?$CTemplateSmartPtrArray@D@@HID@Z ; CStdioFileEx::GetNewMultiByteStringFromUnicodeString
	add	esp, 20					; 00000014H
	mov	DWORD PTR _nCharsWritten$[ebp], eax

; 1172 : 
; 1173 : 	if (nCharsWritten > 0)

	cmp	DWORD PTR _nCharsWritten$[ebp], 0
	jle	SHORT $L73701

; 1175 : 		// Do byte-mode write using actual chars written (fix by Howard J Oh)
; 1176 : 		CFile::Write((const void*)spMultiByteString.GetBuffer(), nCharsWritten*sizeof(char));

	mov	ecx, DWORD PTR _nCharsWritten$[ebp]
	push	ecx
	lea	ecx, DWORD PTR _spMultiByteString$[ebp]
	call	?GetBuffer@?$CTemplateSmartPtrArray@D@@QAEPADXZ ; CTemplateSmartPtrArray<char>::GetBuffer
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Write@CFile@@UAEXPBXI@Z		; CFile::Write
$L73701:

; 1178 : 
; 1179 : 	return nCharsWritten*sizeof(char);

	mov	edx, DWORD PTR _nCharsWritten$[ebp]
	mov	DWORD PTR $T74184[ebp], edx
	mov	DWORD PTR __$EHRec$[ebp+8], -1
	lea	ecx, DWORD PTR _spMultiByteString$[ebp]
	call	??1?$CTemplateSmartPtrArray@D@@UAE@XZ	; CTemplateSmartPtrArray<char>::~CTemplateSmartPtrArray<char>
	mov	eax, DWORD PTR $T74184[ebp]

; 1180 : }

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
$L74185:
	lea	ecx, DWORD PTR _spMultiByteString$[ebp]
	call	??1?$CTemplateSmartPtrArray@D@@UAE@XZ	; CTemplateSmartPtrArray<char>::~CTemplateSmartPtrArray<char>
	ret	0
$L74188:
	mov	eax, OFFSET FLAT:$T74187
	jmp	___CxxFrameHandler
text$x	ENDS
?WriteMultiByteLine@CStdioFileEx@@IAEIPBG@Z ENDP	; CStdioFileEx::WriteMultiByteLine
PUBLIC	??_7?$CTemplateSmartPtrArray@G@@6B@		; CTemplateSmartPtrArray<unsigned short>::`vftable'
PUBLIC	??_G?$CTemplateSmartPtrArray@G@@UAEPAXI@Z	; CTemplateSmartPtrArray<unsigned short>::`scalar deleting destructor'
PUBLIC	??_E?$CTemplateSmartPtrArray@G@@UAEPAXI@Z	; CTemplateSmartPtrArray<unsigned short>::`vector deleting destructor'
;	COMDAT ??_7?$CTemplateSmartPtrArray@G@@6B@
; File D:\_CODE\3rdParty\TemplateSmartPtr.h
CONST	SEGMENT
??_7?$CTemplateSmartPtrArray@G@@6B@ DD FLAT:??_E?$CTemplateSmartPtrArray@G@@UAEPAXI@Z ; CTemplateSmartPtrArray<unsigned short>::`vftable'
CONST	ENDS
;	COMDAT ??0?$CTemplateSmartPtrArray@G@@QAE@PAG_N@Z
_TEXT	SEGMENT
_this$ = -4
_pBuffer$ = 8
_bOwned$ = 12
??0?$CTemplateSmartPtrArray@G@@QAE@PAG_N@Z PROC NEAR	; CTemplateSmartPtrArray<unsigned short>::CTemplateSmartPtrArray<unsigned short>, COMDAT

; 26   : 	CTemplateSmartPtrArray(TYPE* pBuffer, IN const bool bOwned=true)

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CTemplateSmartPtrArray@G@@6B@ ; CTemplateSmartPtrArray<unsigned short>::`vftable'

; 27   : 		{
; 28   : 			m_pBuffer = NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+4], 0

; 29   : 			Assign(pBuffer, bOwned);

	mov	dl, BYTE PTR _bOwned$[ebp]
	push	edx
	mov	eax, DWORD PTR _pBuffer$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Assign@?$CTemplateSmartPtrArray@G@@QAEXPAG_N@Z ; CTemplateSmartPtrArray<unsigned short>::Assign

; 30   : 		}	

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
??0?$CTemplateSmartPtrArray@G@@QAE@PAG_N@Z ENDP		; CTemplateSmartPtrArray<unsigned short>::CTemplateSmartPtrArray<unsigned short>
_TEXT	ENDS
;	COMDAT ??1?$CTemplateSmartPtrArray@G@@UAE@XZ
_TEXT	SEGMENT
_this$ = -8
$T74196 = -4
??1?$CTemplateSmartPtrArray@G@@UAE@XZ PROC NEAR		; CTemplateSmartPtrArray<unsigned short>::~CTemplateSmartPtrArray<unsigned short>, COMDAT

; 32   : 		{

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CTemplateSmartPtrArray@G@@6B@ ; CTemplateSmartPtrArray<unsigned short>::`vftable'

; 33   : 			// Auto-delete, but only if we own it!
; 34   : 			if (m_pBuffer && m_bOwnObject)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L73739
	mov	edx, DWORD PTR _this$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+8]
	test	eax, eax
	je	SHORT $L73739

; 36   : 				delete [] m_pBuffer;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR $T74196[ebp], edx
	mov	eax, DWORD PTR $T74196[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73739:

; 38   : 		}

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1?$CTemplateSmartPtrArray@G@@UAE@XZ ENDP		; CTemplateSmartPtrArray<unsigned short>::~CTemplateSmartPtrArray<unsigned short>
_TEXT	ENDS
;	COMDAT ?Assign@?$CTemplateSmartPtrArray@G@@QAEXPAG_N@Z
_TEXT	SEGMENT
_this$ = -8
$T74199 = -4
_pBuffer$ = 8
_bOwned$ = 12
?Assign@?$CTemplateSmartPtrArray@G@@QAEXPAG_N@Z PROC NEAR ; CTemplateSmartPtrArray<unsigned short>::Assign, COMDAT

; 44   : 		{

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 45   : 			if (m_pBuffer && m_bOwnObject)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L73743
	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+8]
	test	edx, edx
	je	SHORT $L73743

; 47   : 				delete []m_pBuffer;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR $T74199[ebp], ecx
	mov	edx, DWORD PTR $T74199[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73743:

; 49   : 			m_pBuffer = pBuffer;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _pBuffer$[ebp]
	mov	DWORD PTR [eax+4], ecx

; 50   : 			m_bOwnObject = bOwned;

	mov	edx, DWORD PTR _this$[ebp]
	mov	al, BYTE PTR _bOwned$[ebp]
	mov	BYTE PTR [edx+8], al

; 51   : 		}

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Assign@?$CTemplateSmartPtrArray@G@@QAEXPAG_N@Z ENDP	; CTemplateSmartPtrArray<unsigned short>::Assign
_TEXT	ENDS
PUBLIC	??_C@_0CF@POHN@D?3?2_CODE?23rdParty?2TemplateSmartP@ ; `string'
;	COMDAT ??_C@_0CF@POHN@D?3?2_CODE?23rdParty?2TemplateSmartP@
; File D:\_CODE\3rdParty\TemplateSmartPtr.h
_DATA	SEGMENT
??_C@_0CF@POHN@D?3?2_CODE?23rdParty?2TemplateSmartP@ DB 'D:\_CODE\3rdPart'
	DB	'y\TemplateSmartPtr.h', 00H			; `string'
_DATA	ENDS
;	COMDAT ?GetBuffer@?$CTemplateSmartPtrArray@G@@QAEPAGXZ
_TEXT	SEGMENT
_this$ = -4
?GetBuffer@?$CTemplateSmartPtrArray@G@@QAEPAGXZ PROC NEAR ; CTemplateSmartPtrArray<unsigned short>::GetBuffer, COMDAT

; 53   : 		{

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L73747:

; 54   : 			ASSERT(m_pBuffer);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L73750
	push	54					; 00000036H
	push	OFFSET FLAT:??_C@_0CF@POHN@D?3?2_CODE?23rdParty?2TemplateSmartP@ ; `string'
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73750
	int	3
$L73750:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73747

; 55   : 			return m_pBuffer;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]

; 56   : 		}

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetBuffer@?$CTemplateSmartPtrArray@G@@QAEPAGXZ ENDP	; CTemplateSmartPtrArray<unsigned short>::GetBuffer
_TEXT	ENDS
PUBLIC	??_7?$CTemplateSmartPtrArray@D@@6B@		; CTemplateSmartPtrArray<char>::`vftable'
PUBLIC	??_G?$CTemplateSmartPtrArray@D@@UAEPAXI@Z	; CTemplateSmartPtrArray<char>::`scalar deleting destructor'
PUBLIC	??_E?$CTemplateSmartPtrArray@D@@UAEPAXI@Z	; CTemplateSmartPtrArray<char>::`vector deleting destructor'
;	COMDAT ??_7?$CTemplateSmartPtrArray@D@@6B@
; File D:\_CODE\3rdParty\TemplateSmartPtr.h
CONST	SEGMENT
??_7?$CTemplateSmartPtrArray@D@@6B@ DD FLAT:??_E?$CTemplateSmartPtrArray@D@@UAEPAXI@Z ; CTemplateSmartPtrArray<char>::`vftable'
CONST	ENDS
;	COMDAT ??0?$CTemplateSmartPtrArray@D@@QAE@PAD_N@Z
_TEXT	SEGMENT
_this$ = -4
_pBuffer$ = 8
_bOwned$ = 12
??0?$CTemplateSmartPtrArray@D@@QAE@PAD_N@Z PROC NEAR	; CTemplateSmartPtrArray<char>::CTemplateSmartPtrArray<char>, COMDAT

; 26   : 	CTemplateSmartPtrArray(TYPE* pBuffer, IN const bool bOwned=true)

	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CTemplateSmartPtrArray@D@@6B@ ; CTemplateSmartPtrArray<char>::`vftable'

; 27   : 		{
; 28   : 			m_pBuffer = NULL;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	DWORD PTR [ecx+4], 0

; 29   : 			Assign(pBuffer, bOwned);

	mov	dl, BYTE PTR _bOwned$[ebp]
	push	edx
	mov	eax, DWORD PTR _pBuffer$[ebp]
	push	eax
	mov	ecx, DWORD PTR _this$[ebp]
	call	?Assign@?$CTemplateSmartPtrArray@D@@QAEXPAD_N@Z ; CTemplateSmartPtrArray<char>::Assign

; 30   : 		}	

	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
??0?$CTemplateSmartPtrArray@D@@QAE@PAD_N@Z ENDP		; CTemplateSmartPtrArray<char>::CTemplateSmartPtrArray<char>
_TEXT	ENDS
;	COMDAT ??1?$CTemplateSmartPtrArray@D@@UAE@XZ
_TEXT	SEGMENT
_this$ = -8
$T74206 = -4
??1?$CTemplateSmartPtrArray@D@@UAE@XZ PROC NEAR		; CTemplateSmartPtrArray<char>::~CTemplateSmartPtrArray<char>, COMDAT

; 32   : 		{

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	eax, DWORD PTR _this$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:??_7?$CTemplateSmartPtrArray@D@@6B@ ; CTemplateSmartPtrArray<char>::`vftable'

; 33   : 			// Auto-delete, but only if we own it!
; 34   : 			if (m_pBuffer && m_bOwnObject)

	mov	ecx, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [ecx+4], 0
	je	SHORT $L73756
	mov	edx, DWORD PTR _this$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+8]
	test	eax, eax
	je	SHORT $L73756

; 36   : 				delete [] m_pBuffer;

	mov	ecx, DWORD PTR _this$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR $T74206[ebp], edx
	mov	eax, DWORD PTR $T74206[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73756:

; 38   : 		}

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
??1?$CTemplateSmartPtrArray@D@@UAE@XZ ENDP		; CTemplateSmartPtrArray<char>::~CTemplateSmartPtrArray<char>
_TEXT	ENDS
;	COMDAT ?Assign@?$CTemplateSmartPtrArray@D@@QAEXPAD_N@Z
_TEXT	SEGMENT
_this$ = -8
$T74209 = -4
_pBuffer$ = 8
_bOwned$ = 12
?Assign@?$CTemplateSmartPtrArray@D@@QAEXPAD_N@Z PROC NEAR ; CTemplateSmartPtrArray<char>::Assign, COMDAT

; 44   : 		{

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	DWORD PTR [ebp-8], -858993460		; ccccccccH
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx

; 45   : 			if (m_pBuffer && m_bOwnObject)

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	je	SHORT $L73760
	mov	ecx, DWORD PTR _this$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+8]
	test	edx, edx
	je	SHORT $L73760

; 47   : 				delete []m_pBuffer;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR $T74209[ebp], ecx
	mov	edx, DWORD PTR $T74209[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73760:

; 49   : 			m_pBuffer = pBuffer;

	mov	eax, DWORD PTR _this$[ebp]
	mov	ecx, DWORD PTR _pBuffer$[ebp]
	mov	DWORD PTR [eax+4], ecx

; 50   : 			m_bOwnObject = bOwned;

	mov	edx, DWORD PTR _this$[ebp]
	mov	al, BYTE PTR _bOwned$[ebp]
	mov	BYTE PTR [edx+8], al

; 51   : 		}

	add	esp, 8
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	8
?Assign@?$CTemplateSmartPtrArray@D@@QAEXPAD_N@Z ENDP	; CTemplateSmartPtrArray<char>::Assign
_TEXT	ENDS
;	COMDAT ?GetBuffer@?$CTemplateSmartPtrArray@D@@QAEPADXZ
_TEXT	SEGMENT
_this$ = -4
?GetBuffer@?$CTemplateSmartPtrArray@D@@QAEPADXZ PROC NEAR ; CTemplateSmartPtrArray<char>::GetBuffer, COMDAT

; 53   : 		{

	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
$L73764:

; 54   : 			ASSERT(m_pBuffer);

	mov	eax, DWORD PTR _this$[ebp]
	cmp	DWORD PTR [eax+4], 0
	jne	SHORT $L73767
	push	54					; 00000036H
	push	OFFSET FLAT:??_C@_0CF@POHN@D?3?2_CODE?23rdParty?2TemplateSmartP@ ; `string'
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73767
	int	3
$L73767:
	xor	ecx, ecx
	test	ecx, ecx
	jne	SHORT $L73764

; 55   : 			return m_pBuffer;

	mov	edx, DWORD PTR _this$[ebp]
	mov	eax, DWORD PTR [edx+4]

; 56   : 		}

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?GetBuffer@?$CTemplateSmartPtrArray@D@@QAEPADXZ ENDP	; CTemplateSmartPtrArray<char>::GetBuffer
_TEXT	ENDS
;	COMDAT ??_G?$CTemplateSmartPtrArray@G@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_G?$CTemplateSmartPtrArray@G@@UAEPAXI@Z PROC NEAR	; CTemplateSmartPtrArray<unsigned short>::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1?$CTemplateSmartPtrArray@G@@UAE@XZ	; CTemplateSmartPtrArray<unsigned short>::~CTemplateSmartPtrArray<unsigned short>
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73772
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73772:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_G?$CTemplateSmartPtrArray@G@@UAEPAXI@Z ENDP		; CTemplateSmartPtrArray<unsigned short>::`scalar deleting destructor'
_TEXT	ENDS
;	COMDAT ??_G?$CTemplateSmartPtrArray@D@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8
_this$ = -4
??_G?$CTemplateSmartPtrArray@D@@UAEPAXI@Z PROC NEAR	; CTemplateSmartPtrArray<char>::`scalar deleting destructor', COMDAT
	push	ebp
	mov	ebp, esp
	push	ecx
	mov	DWORD PTR [ebp-4], -858993460		; ccccccccH
	mov	DWORD PTR _this$[ebp], ecx
	mov	ecx, DWORD PTR _this$[ebp]
	call	??1?$CTemplateSmartPtrArray@D@@UAE@XZ	; CTemplateSmartPtrArray<char>::~CTemplateSmartPtrArray<char>
	mov	eax, DWORD PTR ___flags$[ebp]
	and	eax, 1
	test	eax, eax
	je	SHORT $L73775
	mov	ecx, DWORD PTR _this$[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73775:
	mov	eax, DWORD PTR _this$[ebp]
	add	esp, 4
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	4
??_G?$CTemplateSmartPtrArray@D@@UAEPAXI@Z ENDP		; CTemplateSmartPtrArray<char>::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??_C@_0BM@IBK@D?3?2_CODE?23rdParty?2ggets?4cpp?$AA@ ; `string'
PUBLIC	?ReallocString@@YAPAGPAGIIAA_N@Z		; ReallocString
EXTRN	??2@YAPAXI@Z:NEAR				; operator new
;	COMDAT ??_C@_0BM@IBK@D?3?2_CODE?23rdParty?2ggets?4cpp?$AA@
; File D:\_CODE\3rdParty\ggets.cpp
_DATA	SEGMENT
??_C@_0BM@IBK@D?3?2_CODE?23rdParty?2ggets?4cpp?$AA@ DB 'D:\_CODE\3rdParty'
	DB	'\ggets.cpp', 00H				; `string'
_DATA	ENDS
;	COMDAT ?fggets@@YAHPAPAGPAU_iobuf@@AA_NH@Z
_TEXT	SEGMENT
_cursize$ = -16
_rdsize$ = -28
_nOldSize$ = -4
_buffer$ = -20
_temp$ = -12
_rdpoint$ = -24
_crlocn$ = -8
$T74218 = -32
$T74221 = -36
_ln$ = 8
_f$ = 12
_bOwnBuffer$ = 16
_nDefaultBufferSizeChars$ = 20
?fggets@@YAHPAPAGPAU_iobuf@@AA_NH@Z PROC NEAR		; fggets, COMDAT

; 118  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 40					; 00000028H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-40]
	mov	ecx, 10					; 0000000aH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 119  :    int		cursize, rdsize, nOldSize;
; 120  :    TTCHAR	*buffer, *temp, *rdpoint, *crlocn;
; 121  : 	
; 122  : 	bOwnBuffer = false;

	mov	eax, DWORD PTR _bOwnBuffer$[ebp]
	mov	BYTE PTR [eax], 0
$L73827:

; 123  : 
; 124  : 	ASSERT(*ln != NULL || nDefaultBufferSizeChars==0);

	mov	ecx, DWORD PTR _ln$[ebp]
	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L73830
	cmp	DWORD PTR _nDefaultBufferSizeChars$[ebp], 0
	je	SHORT $L73830
	push	124					; 0000007cH
	push	OFFSET FLAT:??_C@_0BM@IBK@D?3?2_CODE?23rdParty?2ggets?4cpp?$AA@ ; `string'
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73830
	int	3
$L73830:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L73827

; 125  : 
; 126  : 	// DP 24/06/2007: Allow default buffer to be passed in for reading. Avoids need for "new"s/"delete"s in 99% of cases
; 127  : 	if (nDefaultBufferSizeChars == 0 || *ln == NULL)

	cmp	DWORD PTR _nDefaultBufferSizeChars$[ebp], 0
	je	SHORT $L73833
	mov	eax, DWORD PTR _ln$[ebp]
	cmp	DWORD PTR [eax], 0
	jne	SHORT $L73832
$L73833:

; 129  : 		// DP 29/3/2006: Changed to use new
; 130  : 		buffer = new TTCHAR[INITSIZE];

	push	224					; 000000e0H
	call	??2@YAPAXI@Z				; operator new
	add	esp, 4
	mov	DWORD PTR $T74218[ebp], eax
	mov	ecx, DWORD PTR $T74218[ebp]
	mov	DWORD PTR _buffer$[ebp], ecx

; 131  : 		if (NULL == buffer)

	cmp	DWORD PTR _buffer$[ebp], 0
	jne	SHORT $L73836

; 132  : 			return FGGETS_NOMEM;

	mov	eax, 1
	jmp	$L73819
$L73836:

; 133  : 
; 134  : 		// DP 24/06/2007: Signal that we own the buffer now
; 135  : 		bOwnBuffer = true;

	mov	edx, DWORD PTR _bOwnBuffer$[ebp]
	mov	BYTE PTR [edx], 1

; 137  : 	else

	jmp	SHORT $L73837
$L73832:

; 139  : 		buffer = *ln;

	mov	eax, DWORD PTR _ln$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _buffer$[ebp], ecx
$L73837:

; 141  : 
; 142  :    *ln = NULL; /* default */

	mov	edx, DWORD PTR _ln$[ebp]
	mov	DWORD PTR [edx], 0

; 143  : //   if (NULL == (buffer = (char*)malloc(INITSIZE)))
; 144  : 
; 145  : 	// DP 29/3/2006: Added oldsize variable
; 146  : 	// Initialise original size to default buffer size or INITSIZE, whichever applies
; 147  : 	cursize = rdsize = nOldSize = (nDefaultBufferSizeChars==0 ? INITSIZE:nDefaultBufferSizeChars);

	cmp	DWORD PTR _nDefaultBufferSizeChars$[ebp], 0
	jne	SHORT $L74219
	mov	DWORD PTR -40+[ebp], 112		; 00000070H
	jmp	SHORT $L74220
$L74219:
	mov	eax, DWORD PTR _nDefaultBufferSizeChars$[ebp]
	mov	DWORD PTR -40+[ebp], eax
$L74220:
	mov	ecx, DWORD PTR -40+[ebp]
	mov	DWORD PTR _nOldSize$[ebp], ecx
	mov	edx, DWORD PTR _nOldSize$[ebp]
	mov	DWORD PTR _rdsize$[ebp], edx
	mov	eax, DWORD PTR _rdsize$[ebp]
	mov	DWORD PTR _cursize$[ebp], eax

; 148  :    rdpoint = buffer;

	mov	ecx, DWORD PTR _buffer$[ebp]
	mov	DWORD PTR _rdpoint$[ebp], ecx

; 149  :    *buffer = '\0';

	mov	edx, DWORD PTR _buffer$[ebp]
	mov	WORD PTR [edx], 0

; 150  : 
; 151  : //   if (NULL == fgets(rdpoint, rdsize, f)) 
; 152  :    if (NULL == GetFileString(rdpoint, rdsize, f)) 

	mov	eax, DWORD PTR _f$[ebp]
	push	eax
	mov	ecx, DWORD PTR _rdsize$[ebp]
	push	ecx
	mov	edx, DWORD PTR _rdpoint$[ebp]
	push	edx
	call	?GetFileString@@YAPAGPAGHPAU_iobuf@@@Z	; GetFileString
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	SHORT $L73838

; 154  : //      free(buffer);
; 155  : 		// DP 29/3/2006: Changed to use delete
; 156  : 		if (bOwnBuffer)

	mov	eax, DWORD PTR _bOwnBuffer$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax]
	test	ecx, ecx
	je	SHORT $L73839

; 158  : 			delete buffer;

	mov	edx, DWORD PTR _buffer$[ebp]
	mov	DWORD PTR $T74221[ebp], edx
	mov	eax, DWORD PTR $T74221[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73839:

; 160  :       return EOF;

	or	eax, -1
	jmp	$L73819
$L73838:

; 162  : 
; 163  :    /* initial read succeeded, now decide about expansion */
; 164  : //   while (NULL == (crlocn = strchr(rdpoint, '\n'))) 
; 165  :    while (NULL == (crlocn = FindCharInString(rdpoint, '\n'))) 

	push	10					; 0000000aH
	mov	ecx, DWORD PTR _rdpoint$[ebp]
	push	ecx
	call	?FindCharInString@@YAPAGPBGG@Z		; FindCharInString
	add	esp, 8
	mov	DWORD PTR _crlocn$[ebp], eax
	cmp	DWORD PTR _crlocn$[ebp], 0
	jne	$L73843

; 167  :       /* line is not completed, expand */
; 168  : 
; 169  : 		// DP 29/3/2006: Save current buffer size
; 170  : 		nOldSize = cursize;

	mov	edx, DWORD PTR _cursize$[ebp]
	mov	DWORD PTR _nOldSize$[ebp], edx

; 171  : 
; 172  :       /* set up cursize, rdpoint and rdsize, expand buffer */
; 173  :       rdsize = DELTASIZE + 1;   /* allow for a final '\0' */

	mov	DWORD PTR _rdsize$[ebp], 129		; 00000081H

; 174  :       cursize += DELTASIZE;

	mov	eax, DWORD PTR _cursize$[ebp]
	add	eax, 128				; 00000080H
	mov	DWORD PTR _cursize$[ebp], eax

; 175  : //      if (NULL == (temp = (char*)realloc(buffer, (size_t)cursize))) {
; 176  : 
; 177  : 		// DP 29/3/2006: Changed to use custom realloc
; 178  :       if (NULL == (temp = ReallocString<TTCHAR>(buffer, (size_t)cursize, (size_t)nOldSize, bOwnBuffer))) 

	mov	ecx, DWORD PTR _bOwnBuffer$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nOldSize$[ebp]
	push	edx
	mov	eax, DWORD PTR _cursize$[ebp]
	push	eax
	mov	ecx, DWORD PTR _buffer$[ebp]
	push	ecx
	call	?ReallocString@@YAPAGPAGIIAA_N@Z	; ReallocString
	add	esp, 16					; 00000010H
	mov	DWORD PTR _temp$[ebp], eax
	cmp	DWORD PTR _temp$[ebp], 0
	jne	SHORT $L73853

; 180  :          /* ran out of memory */
; 181  :          *ln = buffer; /* partial line, next call may fail */

	mov	edx, DWORD PTR _ln$[ebp]
	mov	eax, DWORD PTR _buffer$[ebp]
	mov	DWORD PTR [edx], eax

; 182  :          return FGGETS_NOMEM;

	mov	eax, 1
	jmp	$L73819
$L73853:

; 184  :       buffer = temp;

	mov	ecx, DWORD PTR _temp$[ebp]
	mov	DWORD PTR _buffer$[ebp], ecx

; 185  :       /* Read into the '\0' up */
; 186  :       rdpoint = buffer + (cursize - DELTASIZE - 1);

	mov	edx, DWORD PTR _cursize$[ebp]
	mov	eax, DWORD PTR _buffer$[ebp]
	lea	ecx, DWORD PTR [eax+edx*2-258]
	mov	DWORD PTR _rdpoint$[ebp], ecx

; 187  : 
; 188  :       /* get the next piece of this line */
; 189  : //      if (NULL == fgets(rdpoint, rdsize, f)) {
; 190  :       if (NULL == GetFileString(rdpoint, rdsize, f)) 

	mov	edx, DWORD PTR _f$[ebp]
	push	edx
	mov	eax, DWORD PTR _rdsize$[ebp]
	push	eax
	mov	ecx, DWORD PTR _rdpoint$[ebp]
	push	ecx
	call	?GetFileString@@YAPAGPAGHPAU_iobuf@@@Z	; GetFileString
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	SHORT $L73854

; 192  :          /* early EOF encountered */
; 193  : //         crlocn = strchr(buffer, '\0');
; 194  :          crlocn = FindCharInString(buffer, '\0');

	push	0
	mov	edx, DWORD PTR _buffer$[ebp]
	push	edx
	call	?FindCharInString@@YAPAGPBGG@Z		; FindCharInString
	add	esp, 8
	mov	DWORD PTR _crlocn$[ebp], eax

; 195  :          break;

	jmp	SHORT $L73843
$L73854:

; 197  :    } /* while line not complete */

	jmp	$L73838
$L73843:

; 198  : 
; 199  :    *crlocn = '\0';  /* mark line end, strip \n */

	mov	eax, DWORD PTR _crlocn$[ebp]
	mov	WORD PTR [eax], 0

; 200  :    rdsize = (int)(crlocn - buffer);

	mov	ecx, DWORD PTR _crlocn$[ebp]
	sub	ecx, DWORD PTR _buffer$[ebp]
	sar	ecx, 1
	mov	DWORD PTR _rdsize$[ebp], ecx

; 201  : 
; 202  : //   if (NULL == (temp = (char*)realloc(buffer, (size_t)rdsize + 1))) {
; 203  : 
; 204  : 	// DP 29/3/2006: Changed to use custom realloc
; 205  :    if (NULL == (temp = ReallocString<TTCHAR>(buffer, (size_t)rdsize + 1, (size_t)cursize, bOwnBuffer))) 

	mov	edx, DWORD PTR _bOwnBuffer$[ebp]
	push	edx
	mov	eax, DWORD PTR _cursize$[ebp]
	push	eax
	mov	ecx, DWORD PTR _rdsize$[ebp]
	add	ecx, 1
	push	ecx
	mov	edx, DWORD PTR _buffer$[ebp]
	push	edx
	call	?ReallocString@@YAPAGPAGIIAA_N@Z	; ReallocString
	add	esp, 16					; 00000010H
	mov	DWORD PTR _temp$[ebp], eax
	cmp	DWORD PTR _temp$[ebp], 0
	jne	SHORT $L73864

; 207  :       *ln = buffer;  /* without reducing it */

	mov	eax, DWORD PTR _ln$[ebp]
	mov	ecx, DWORD PTR _buffer$[ebp]
	mov	DWORD PTR [eax], ecx

; 208  :       return FGGETS_OK;

	xor	eax, eax
	jmp	SHORT $L73819
$L73864:

; 210  :    *ln = temp;

	mov	edx, DWORD PTR _ln$[ebp]
	mov	eax, DWORD PTR _temp$[ebp]
	mov	DWORD PTR [edx], eax

; 211  :    return FGGETS_OK;

	xor	eax, eax
$L73819:

; 212  : } /* fggets */

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?fggets@@YAHPAPAGPAU_iobuf@@AA_NH@Z ENDP		; fggets
_TEXT	ENDS
EXTRN	__imp__fgetws:NEAR
;	COMDAT ?GetFileString@@YAPAGPAGHPAU_iobuf@@@Z
_TEXT	SEGMENT
_string$ = 8
_n$ = 12
_stream$ = 16
?GetFileString@@YAPAGPAGHPAU_iobuf@@@Z PROC NEAR	; GetFileString, COMDAT

; 91   : {

	push	ebp
	mov	ebp, esp
	push	esi

; 92   : 	return fgetws( string, n, stream );

	mov	esi, esp
	mov	eax, DWORD PTR _stream$[ebp]
	push	eax
	mov	ecx, DWORD PTR _n$[ebp]
	push	ecx
	mov	edx, DWORD PTR _string$[ebp]
	push	edx
	call	DWORD PTR __imp__fgetws
	add	esp, 12					; 0000000cH
	cmp	esi, esp
	call	__chkesp

; 93   : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?GetFileString@@YAPAGPAGHPAU_iobuf@@@Z ENDP		; GetFileString
_TEXT	ENDS
EXTRN	__imp__wcschr:NEAR
;	COMDAT ?FindCharInString@@YAPAGPBGG@Z
_TEXT	SEGMENT
_string$ = 8
_c$ = 12
?FindCharInString@@YAPAGPBGG@Z PROC NEAR		; FindCharInString, COMDAT

; 105  : {	

	push	ebp
	mov	ebp, esp
	push	esi

; 106  : 	return (wchar_t *)wcschr( string, c );

	mov	esi, esp
	mov	ax, WORD PTR _c$[ebp]
	push	eax
	mov	ecx, DWORD PTR _string$[ebp]
	push	ecx
	call	DWORD PTR __imp__wcschr
	add	esp, 8
	cmp	esi, esp
	call	__chkesp

; 107  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?FindCharInString@@YAPAGPBGG@Z ENDP			; FindCharInString
_TEXT	ENDS
PUBLIC	?ReallocString@@YAPADPADIIAA_N@Z		; ReallocString
;	COMDAT ?fggets@@YAHPAPADPAU_iobuf@@AA_NH@Z
_TEXT	SEGMENT
_cursize$ = -16
_rdsize$ = -28
_nOldSize$ = -4
_buffer$ = -20
_temp$ = -12
_rdpoint$ = -24
_crlocn$ = -8
$T74228 = -32
$T74231 = -36
_ln$ = 8
_f$ = 12
_bOwnBuffer$ = 16
_nDefaultBufferSizeChars$ = 20
?fggets@@YAHPAPADPAU_iobuf@@AA_NH@Z PROC NEAR		; fggets, COMDAT

; 118  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 40					; 00000028H
	push	ebx
	push	esi
	push	edi
	lea	edi, DWORD PTR [ebp-40]
	mov	ecx, 10					; 0000000aH
	mov	eax, -858993460				; ccccccccH
	rep stosd

; 119  :    int		cursize, rdsize, nOldSize;
; 120  :    TTCHAR	*buffer, *temp, *rdpoint, *crlocn;
; 121  : 	
; 122  : 	bOwnBuffer = false;

	mov	eax, DWORD PTR _bOwnBuffer$[ebp]
	mov	BYTE PTR [eax], 0
$L73873:

; 123  : 
; 124  : 	ASSERT(*ln != NULL || nDefaultBufferSizeChars==0);

	mov	ecx, DWORD PTR _ln$[ebp]
	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L73876
	cmp	DWORD PTR _nDefaultBufferSizeChars$[ebp], 0
	je	SHORT $L73876
	push	124					; 0000007cH
	push	OFFSET FLAT:??_C@_0BM@IBK@D?3?2_CODE?23rdParty?2ggets?4cpp?$AA@ ; `string'
	call	?AfxAssertFailedLine@@YGHPBDH@Z		; AfxAssertFailedLine
	test	eax, eax
	je	SHORT $L73876
	int	3
$L73876:
	xor	edx, edx
	test	edx, edx
	jne	SHORT $L73873

; 125  : 
; 126  : 	// DP 24/06/2007: Allow default buffer to be passed in for reading. Avoids need for "new"s/"delete"s in 99% of cases
; 127  : 	if (nDefaultBufferSizeChars == 0 || *ln == NULL)

	cmp	DWORD PTR _nDefaultBufferSizeChars$[ebp], 0
	je	SHORT $L73878
	mov	eax, DWORD PTR _ln$[ebp]
	cmp	DWORD PTR [eax], 0
	jne	SHORT $L73877
$L73878:

; 129  : 		// DP 29/3/2006: Changed to use new
; 130  : 		buffer = new TTCHAR[INITSIZE];

	push	112					; 00000070H
	call	??2@YAPAXI@Z				; operator new
	add	esp, 4
	mov	DWORD PTR $T74228[ebp], eax
	mov	ecx, DWORD PTR $T74228[ebp]
	mov	DWORD PTR _buffer$[ebp], ecx

; 131  : 		if (NULL == buffer)

	cmp	DWORD PTR _buffer$[ebp], 0
	jne	SHORT $L73881

; 132  : 			return FGGETS_NOMEM;

	mov	eax, 1
	jmp	$L73865
$L73881:

; 133  : 
; 134  : 		// DP 24/06/2007: Signal that we own the buffer now
; 135  : 		bOwnBuffer = true;

	mov	edx, DWORD PTR _bOwnBuffer$[ebp]
	mov	BYTE PTR [edx], 1

; 137  : 	else

	jmp	SHORT $L73882
$L73877:

; 139  : 		buffer = *ln;

	mov	eax, DWORD PTR _ln$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _buffer$[ebp], ecx
$L73882:

; 141  : 
; 142  :    *ln = NULL; /* default */

	mov	edx, DWORD PTR _ln$[ebp]
	mov	DWORD PTR [edx], 0

; 143  : //   if (NULL == (buffer = (char*)malloc(INITSIZE)))
; 144  : 
; 145  : 	// DP 29/3/2006: Added oldsize variable
; 146  : 	// Initialise original size to default buffer size or INITSIZE, whichever applies
; 147  : 	cursize = rdsize = nOldSize = (nDefaultBufferSizeChars==0 ? INITSIZE:nDefaultBufferSizeChars);

	cmp	DWORD PTR _nDefaultBufferSizeChars$[ebp], 0
	jne	SHORT $L74229
	mov	DWORD PTR -40+[ebp], 112		; 00000070H
	jmp	SHORT $L74230
$L74229:
	mov	eax, DWORD PTR _nDefaultBufferSizeChars$[ebp]
	mov	DWORD PTR -40+[ebp], eax
$L74230:
	mov	ecx, DWORD PTR -40+[ebp]
	mov	DWORD PTR _nOldSize$[ebp], ecx
	mov	edx, DWORD PTR _nOldSize$[ebp]
	mov	DWORD PTR _rdsize$[ebp], edx
	mov	eax, DWORD PTR _rdsize$[ebp]
	mov	DWORD PTR _cursize$[ebp], eax

; 148  :    rdpoint = buffer;

	mov	ecx, DWORD PTR _buffer$[ebp]
	mov	DWORD PTR _rdpoint$[ebp], ecx

; 149  :    *buffer = '\0';

	mov	edx, DWORD PTR _buffer$[ebp]
	mov	BYTE PTR [edx], 0

; 150  : 
; 151  : //   if (NULL == fgets(rdpoint, rdsize, f)) 
; 152  :    if (NULL == GetFileString(rdpoint, rdsize, f)) 

	mov	eax, DWORD PTR _f$[ebp]
	push	eax
	mov	ecx, DWORD PTR _rdsize$[ebp]
	push	ecx
	mov	edx, DWORD PTR _rdpoint$[ebp]
	push	edx
	call	?GetFileString@@YAPADPADHPAU_iobuf@@@Z	; GetFileString
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	SHORT $L73883

; 154  : //      free(buffer);
; 155  : 		// DP 29/3/2006: Changed to use delete
; 156  : 		if (bOwnBuffer)

	mov	eax, DWORD PTR _bOwnBuffer$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax]
	test	ecx, ecx
	je	SHORT $L73884

; 158  : 			delete buffer;

	mov	edx, DWORD PTR _buffer$[ebp]
	mov	DWORD PTR $T74231[ebp], edx
	mov	eax, DWORD PTR $T74231[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73884:

; 160  :       return EOF;

	or	eax, -1
	jmp	$L73865
$L73883:

; 162  : 
; 163  :    /* initial read succeeded, now decide about expansion */
; 164  : //   while (NULL == (crlocn = strchr(rdpoint, '\n'))) 
; 165  :    while (NULL == (crlocn = FindCharInString(rdpoint, '\n'))) 

	push	10					; 0000000aH
	mov	ecx, DWORD PTR _rdpoint$[ebp]
	push	ecx
	call	?FindCharInString@@YAPADPBDH@Z		; FindCharInString
	add	esp, 8
	mov	DWORD PTR _crlocn$[ebp], eax
	cmp	DWORD PTR _crlocn$[ebp], 0
	jne	$L73888

; 167  :       /* line is not completed, expand */
; 168  : 
; 169  : 		// DP 29/3/2006: Save current buffer size
; 170  : 		nOldSize = cursize;

	mov	edx, DWORD PTR _cursize$[ebp]
	mov	DWORD PTR _nOldSize$[ebp], edx

; 171  : 
; 172  :       /* set up cursize, rdpoint and rdsize, expand buffer */
; 173  :       rdsize = DELTASIZE + 1;   /* allow for a final '\0' */

	mov	DWORD PTR _rdsize$[ebp], 129		; 00000081H

; 174  :       cursize += DELTASIZE;

	mov	eax, DWORD PTR _cursize$[ebp]
	add	eax, 128				; 00000080H
	mov	DWORD PTR _cursize$[ebp], eax

; 175  : //      if (NULL == (temp = (char*)realloc(buffer, (size_t)cursize))) {
; 176  : 
; 177  : 		// DP 29/3/2006: Changed to use custom realloc
; 178  :       if (NULL == (temp = ReallocString<TTCHAR>(buffer, (size_t)cursize, (size_t)nOldSize, bOwnBuffer))) 

	mov	ecx, DWORD PTR _bOwnBuffer$[ebp]
	push	ecx
	mov	edx, DWORD PTR _nOldSize$[ebp]
	push	edx
	mov	eax, DWORD PTR _cursize$[ebp]
	push	eax
	mov	ecx, DWORD PTR _buffer$[ebp]
	push	ecx
	call	?ReallocString@@YAPADPADIIAA_N@Z	; ReallocString
	add	esp, 16					; 00000010H
	mov	DWORD PTR _temp$[ebp], eax
	cmp	DWORD PTR _temp$[ebp], 0
	jne	SHORT $L73898

; 180  :          /* ran out of memory */
; 181  :          *ln = buffer; /* partial line, next call may fail */

	mov	edx, DWORD PTR _ln$[ebp]
	mov	eax, DWORD PTR _buffer$[ebp]
	mov	DWORD PTR [edx], eax

; 182  :          return FGGETS_NOMEM;

	mov	eax, 1
	jmp	$L73865
$L73898:

; 184  :       buffer = temp;

	mov	ecx, DWORD PTR _temp$[ebp]
	mov	DWORD PTR _buffer$[ebp], ecx

; 185  :       /* Read into the '\0' up */
; 186  :       rdpoint = buffer + (cursize - DELTASIZE - 1);

	mov	edx, DWORD PTR _cursize$[ebp]
	mov	eax, DWORD PTR _buffer$[ebp]
	lea	ecx, DWORD PTR [eax+edx-129]
	mov	DWORD PTR _rdpoint$[ebp], ecx

; 187  : 
; 188  :       /* get the next piece of this line */
; 189  : //      if (NULL == fgets(rdpoint, rdsize, f)) {
; 190  :       if (NULL == GetFileString(rdpoint, rdsize, f)) 

	mov	edx, DWORD PTR _f$[ebp]
	push	edx
	mov	eax, DWORD PTR _rdsize$[ebp]
	push	eax
	mov	ecx, DWORD PTR _rdpoint$[ebp]
	push	ecx
	call	?GetFileString@@YAPADPADHPAU_iobuf@@@Z	; GetFileString
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	SHORT $L73899

; 192  :          /* early EOF encountered */
; 193  : //         crlocn = strchr(buffer, '\0');
; 194  :          crlocn = FindCharInString(buffer, '\0');

	push	0
	mov	edx, DWORD PTR _buffer$[ebp]
	push	edx
	call	?FindCharInString@@YAPADPBDH@Z		; FindCharInString
	add	esp, 8
	mov	DWORD PTR _crlocn$[ebp], eax

; 195  :          break;

	jmp	SHORT $L73888
$L73899:

; 197  :    } /* while line not complete */

	jmp	$L73883
$L73888:

; 198  : 
; 199  :    *crlocn = '\0';  /* mark line end, strip \n */

	mov	eax, DWORD PTR _crlocn$[ebp]
	mov	BYTE PTR [eax], 0

; 200  :    rdsize = (int)(crlocn - buffer);

	mov	ecx, DWORD PTR _crlocn$[ebp]
	sub	ecx, DWORD PTR _buffer$[ebp]
	mov	DWORD PTR _rdsize$[ebp], ecx

; 201  : 
; 202  : //   if (NULL == (temp = (char*)realloc(buffer, (size_t)rdsize + 1))) {
; 203  : 
; 204  : 	// DP 29/3/2006: Changed to use custom realloc
; 205  :    if (NULL == (temp = ReallocString<TTCHAR>(buffer, (size_t)rdsize + 1, (size_t)cursize, bOwnBuffer))) 

	mov	edx, DWORD PTR _bOwnBuffer$[ebp]
	push	edx
	mov	eax, DWORD PTR _cursize$[ebp]
	push	eax
	mov	ecx, DWORD PTR _rdsize$[ebp]
	add	ecx, 1
	push	ecx
	mov	edx, DWORD PTR _buffer$[ebp]
	push	edx
	call	?ReallocString@@YAPADPADIIAA_N@Z	; ReallocString
	add	esp, 16					; 00000010H
	mov	DWORD PTR _temp$[ebp], eax
	cmp	DWORD PTR _temp$[ebp], 0
	jne	SHORT $L73909

; 207  :       *ln = buffer;  /* without reducing it */

	mov	eax, DWORD PTR _ln$[ebp]
	mov	ecx, DWORD PTR _buffer$[ebp]
	mov	DWORD PTR [eax], ecx

; 208  :       return FGGETS_OK;

	xor	eax, eax
	jmp	SHORT $L73865
$L73909:

; 210  :    *ln = temp;

	mov	edx, DWORD PTR _ln$[ebp]
	mov	eax, DWORD PTR _temp$[ebp]
	mov	DWORD PTR [edx], eax

; 211  :    return FGGETS_OK;

	xor	eax, eax
$L73865:

; 212  : } /* fggets */

	pop	edi
	pop	esi
	pop	ebx
	add	esp, 40					; 00000028H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?fggets@@YAHPAPADPAU_iobuf@@AA_NH@Z ENDP		; fggets
_TEXT	ENDS
EXTRN	__imp__fgets:NEAR
;	COMDAT ?GetFileString@@YAPADPADHPAU_iobuf@@@Z
_TEXT	SEGMENT
_string$ = 8
_n$ = 12
_stream$ = 16
?GetFileString@@YAPADPADHPAU_iobuf@@@Z PROC NEAR	; GetFileString, COMDAT

; 86   : {

	push	ebp
	mov	ebp, esp
	push	esi

; 87   : 	return fgets( string, n, stream );

	mov	esi, esp
	mov	eax, DWORD PTR _stream$[ebp]
	push	eax
	mov	ecx, DWORD PTR _n$[ebp]
	push	ecx
	mov	edx, DWORD PTR _string$[ebp]
	push	edx
	call	DWORD PTR __imp__fgets
	add	esp, 12					; 0000000cH
	cmp	esi, esp
	call	__chkesp

; 88   : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?GetFileString@@YAPADPADHPAU_iobuf@@@Z ENDP		; GetFileString
_TEXT	ENDS
EXTRN	__imp___mbschr:NEAR
;	COMDAT ?FindCharInString@@YAPADPBDH@Z
_TEXT	SEGMENT
_string$ = 8
_c$ = 12
?FindCharInString@@YAPADPBDH@Z PROC NEAR		; FindCharInString, COMDAT

; 97   : {	

	push	ebp
	mov	ebp, esp
	push	esi

; 98   : 	// This *should* work OK even with UTF-8, since we're only searching for
; 99   : 	// '\n' in practice. Careful if you try to use this for anything other
; 100  : 	// than ASCII characters. 
; 101  : 	return (char*)_mbschr( (const unsigned char *)string, c );

	mov	esi, esp
	mov	eax, DWORD PTR _c$[ebp]
	push	eax
	mov	ecx, DWORD PTR _string$[ebp]
	push	ecx
	call	DWORD PTR __imp___mbschr
	add	esp, 8
	cmp	esi, esp
	call	__chkesp

; 102  : }

	pop	esi
	cmp	ebp, esp
	call	__chkesp
	pop	ebp
	ret	0
?FindCharInString@@YAPADPBDH@Z ENDP			; FindCharInString
_TEXT	ENDS
EXTRN	_memcpy:NEAR
;	COMDAT ?ReallocString@@YAPAGPAGIIAA_N@Z
_TEXT	SEGMENT
_ptr$ = 8
_newsize$ = 12
_oldsize$ = 16
_bOwnBuffer$ = 20
_pTemp$73912 = -4
$T74238 = -8
$T74241 = -12
$T74242 = -16
?ReallocString@@YAPAGPAGIIAA_N@Z PROC NEAR		; ReallocString, COMDAT

; 43   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 20					; 00000014H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 44   : 	// Only reallocate if needed
; 45   : 	if (newsize > oldsize)

	mov	eax, DWORD PTR _newsize$[ebp]
	cmp	eax, DWORD PTR _oldsize$[ebp]
	jbe	$L73911

; 47   : 		TTCHAR* pTemp = NULL;

	mov	DWORD PTR _pTemp$73912[ebp], 0

; 48   : 
; 49   : 		// Declare the new string and zero it out
; 50   : 		pTemp = new TTCHAR[newsize] ;

	mov	ecx, DWORD PTR _newsize$[ebp]
	shl	ecx, 1
	push	ecx
	call	??2@YAPAXI@Z				; operator new
	add	esp, 4
	mov	DWORD PTR $T74238[ebp], eax
	mov	edx, DWORD PTR $T74238[ebp]
	mov	DWORD PTR _pTemp$73912[ebp], edx

; 51   : 		memset(pTemp, '\0', newsize*sizeof(TTCHAR));

	mov	eax, DWORD PTR _newsize$[ebp]
	shl	eax, 1
	push	eax
	push	0
	mov	ecx, DWORD PTR _pTemp$73912[ebp]
	push	ecx
	call	_memset
	add	esp, 12					; 0000000cH

; 52   : 
; 53   : 		// Copy the old string if there is one
; 54   : 		if (ptr)

	cmp	DWORD PTR _ptr$[ebp], 0
	je	SHORT $L73924

; 56   : 			// Ensure we only copy up to the size of the new buffer
; 57   : 			memcpy(pTemp, ptr, min(oldsize*sizeof(TTCHAR), newsize*sizeof(TTCHAR)));

	mov	edx, DWORD PTR _oldsize$[ebp]
	shl	edx, 1
	mov	eax, DWORD PTR _newsize$[ebp]
	shl	eax, 1
	cmp	edx, eax
	jae	SHORT $L74239
	mov	ecx, DWORD PTR _oldsize$[ebp]
	shl	ecx, 1
	mov	DWORD PTR -20+[ebp], ecx
	jmp	SHORT $L74240
$L74239:
	mov	edx, DWORD PTR _newsize$[ebp]
	shl	edx, 1
	mov	DWORD PTR -20+[ebp], edx
$L74240:
	mov	eax, DWORD PTR -20+[ebp]
	push	eax
	mov	ecx, DWORD PTR _ptr$[ebp]
	push	ecx
	mov	edx, DWORD PTR _pTemp$73912[ebp]
	push	edx
	call	_memcpy
	add	esp, 12					; 0000000cH

; 58   : 
; 59   : 			// Don't delete a default buffer we didn't create
; 60   : 			if (bOwnBuffer)

	mov	eax, DWORD PTR _bOwnBuffer$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax]
	test	ecx, ecx
	je	SHORT $L73924

; 62   : 				if(oldsize > 1)

	cmp	DWORD PTR _oldsize$[ebp], 1
	jbe	SHORT $L73922

; 64   : 					delete [] ptr;

	mov	edx, DWORD PTR _ptr$[ebp]
	mov	DWORD PTR $T74241[ebp], edx
	mov	eax, DWORD PTR $T74241[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 66   : 				else

	jmp	SHORT $L73924
$L73922:

; 68   : 					delete ptr;

	mov	ecx, DWORD PTR _ptr$[ebp]
	mov	DWORD PTR $T74242[ebp], ecx
	mov	edx, DWORD PTR $T74242[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73924:

; 72   : 
; 73   : 		// We own it now!
; 74   : 		bOwnBuffer = true;

	mov	eax, DWORD PTR _bOwnBuffer$[ebp]
	mov	BYTE PTR [eax], 1

; 75   : 
; 76   : 		return pTemp; 

	mov	eax, DWORD PTR _pTemp$73912[ebp]
	jmp	SHORT $L73910
$L73911:

; 78   : 
; 79   : 	return ptr;

	mov	eax, DWORD PTR _ptr$[ebp]
$L73910:

; 80   : } 

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?ReallocString@@YAPAGPAGIIAA_N@Z ENDP			; ReallocString
_TEXT	ENDS
;	COMDAT ?ReallocString@@YAPADPADIIAA_N@Z
_TEXT	SEGMENT
_ptr$ = 8
_newsize$ = 12
_oldsize$ = 16
_bOwnBuffer$ = 20
_pTemp$73928 = -4
$T74245 = -8
$T74248 = -12
$T74249 = -16
?ReallocString@@YAPADPADIIAA_N@Z PROC NEAR		; ReallocString, COMDAT

; 43   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 20					; 00000014H
	mov	eax, -858993460				; ccccccccH
	mov	DWORD PTR [ebp-20], eax
	mov	DWORD PTR [ebp-16], eax
	mov	DWORD PTR [ebp-12], eax
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], eax

; 44   : 	// Only reallocate if needed
; 45   : 	if (newsize > oldsize)

	mov	eax, DWORD PTR _newsize$[ebp]
	cmp	eax, DWORD PTR _oldsize$[ebp]
	jbe	$L73927

; 47   : 		TTCHAR* pTemp = NULL;

	mov	DWORD PTR _pTemp$73928[ebp], 0

; 48   : 
; 49   : 		// Declare the new string and zero it out
; 50   : 		pTemp = new TTCHAR[newsize] ;

	mov	ecx, DWORD PTR _newsize$[ebp]
	push	ecx
	call	??2@YAPAXI@Z				; operator new
	add	esp, 4
	mov	DWORD PTR $T74245[ebp], eax
	mov	edx, DWORD PTR $T74245[ebp]
	mov	DWORD PTR _pTemp$73928[ebp], edx

; 51   : 		memset(pTemp, '\0', newsize*sizeof(TTCHAR));

	mov	eax, DWORD PTR _newsize$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _pTemp$73928[ebp]
	push	ecx
	call	_memset
	add	esp, 12					; 0000000cH

; 52   : 
; 53   : 		// Copy the old string if there is one
; 54   : 		if (ptr)

	cmp	DWORD PTR _ptr$[ebp], 0
	je	SHORT $L73940

; 56   : 			// Ensure we only copy up to the size of the new buffer
; 57   : 			memcpy(pTemp, ptr, min(oldsize*sizeof(TTCHAR), newsize*sizeof(TTCHAR)));

	mov	edx, DWORD PTR _oldsize$[ebp]
	cmp	edx, DWORD PTR _newsize$[ebp]
	jae	SHORT $L74246
	mov	eax, DWORD PTR _oldsize$[ebp]
	mov	DWORD PTR -20+[ebp], eax
	jmp	SHORT $L74247
$L74246:
	mov	ecx, DWORD PTR _newsize$[ebp]
	mov	DWORD PTR -20+[ebp], ecx
$L74247:
	mov	edx, DWORD PTR -20+[ebp]
	push	edx
	mov	eax, DWORD PTR _ptr$[ebp]
	push	eax
	mov	ecx, DWORD PTR _pTemp$73928[ebp]
	push	ecx
	call	_memcpy
	add	esp, 12					; 0000000cH

; 58   : 
; 59   : 			// Don't delete a default buffer we didn't create
; 60   : 			if (bOwnBuffer)

	mov	edx, DWORD PTR _bOwnBuffer$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx]
	test	eax, eax
	je	SHORT $L73940

; 62   : 				if(oldsize > 1)

	cmp	DWORD PTR _oldsize$[ebp], 1
	jbe	SHORT $L73938

; 64   : 					delete [] ptr;

	mov	ecx, DWORD PTR _ptr$[ebp]
	mov	DWORD PTR $T74248[ebp], ecx
	mov	edx, DWORD PTR $T74248[ebp]
	push	edx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 66   : 				else

	jmp	SHORT $L73940
$L73938:

; 68   : 					delete ptr;

	mov	eax, DWORD PTR _ptr$[ebp]
	mov	DWORD PTR $T74249[ebp], eax
	mov	ecx, DWORD PTR $T74249[ebp]
	push	ecx
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L73940:

; 72   : 
; 73   : 		// We own it now!
; 74   : 		bOwnBuffer = true;

	mov	edx, DWORD PTR _bOwnBuffer$[ebp]
	mov	BYTE PTR [edx], 1

; 75   : 
; 76   : 		return pTemp; 

	mov	eax, DWORD PTR _pTemp$73928[ebp]
	jmp	SHORT $L73926
$L73927:

; 78   : 
; 79   : 	return ptr;

	mov	eax, DWORD PTR _ptr$[ebp]
$L73926:

; 80   : } 

	add	esp, 20					; 00000014H
	cmp	ebp, esp
	call	__chkesp
	mov	esp, ebp
	pop	ebp
	ret	0
?ReallocString@@YAPADPADIIAA_N@Z ENDP			; ReallocString
_TEXT	ENDS
END

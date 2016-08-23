/*
 * mkrtf-ctrl.c -- generate rtf-ctrl file containing rtf control words
 * and their major and minor numbers.
 *
 * The rtf-ctrl file has the following uses:
 * - by making the table of control words external to translators, the
 * data segment size on Macintosh applications shrinks considerably.
 * This avoids problems with THINK C.  The scheme will still fail if
 * the table ends up with 32K of string space, because then this program
 * won't itself be compilable.  That's a ways off, I hope.
 * - program sizes become smaller.
 *
 * 08 Apr 94	Paul DuBois	dubois@primate.wisc.edu
 *
 * 08 Apr 94
 * - Created.
 */

# include	<stdio.h>

# define	rtfInternal
# include	"rtf.h"
# undef		rtfInternal


typedef struct RTFKey	RTFKey;

struct RTFKey
{
	int	major;	/* major number */
	int	minor;	/* minor number */
	char	*str;	/* symbol name */
	int	junk;	/* symbol name hash value */
};

/*
 * A minor number of -1 means the token has no minor number
 * (all valid minor numbers are >= 0).
 */

static RTFKey	rtfKey[] =
{
	/*
	 * Special characters
	 */

	rtfSpecialChar,	rtfIIntVersion,		"vern",		0,
	rtfSpecialChar,	rtfIEditTime,		"edmins",	0,
	rtfSpecialChar,	rtfIYear,		"yr",		0,
	rtfSpecialChar,	rtfIMonth,		"mo",		0,
	rtfSpecialChar,	rtfIDay,		"dy",		0,
	rtfSpecialChar,	rtfIHour,		"hr",		0,
	rtfSpecialChar,	rtfIMinute,		"min",		0,
	rtfSpecialChar,	rtfISecond,		"sec",		0,
	rtfSpecialChar,	rtfINPages,		"nofpages",	0,
	rtfSpecialChar,	rtfINWords,		"nofwords",	0,
	rtfSpecialChar,	rtfINChars,		"nofchars",	0,
	rtfSpecialChar,	rtfIIntID,		"id",		0,

	rtfSpecialChar,	rtfCurHeadDate,		"chdate",	0,
	rtfSpecialChar,	rtfCurHeadDateLong,	"chdpl",	0,
	rtfSpecialChar,	rtfCurHeadDateAbbrev,	"chdpa",	0,
	rtfSpecialChar,	rtfCurHeadTime,		"chtime",	0,
	rtfSpecialChar,	rtfCurHeadPage,		"chpgn",	0,
	rtfSpecialChar,	rtfSectNum,		"sectnum",	0,
	rtfSpecialChar,	rtfCurFNote,		"chftn",	0,
	rtfSpecialChar,	rtfCurAnnotRef,		"chatn",	0,
	rtfSpecialChar,	rtfFNoteSep,		"chftnsep",	0,
	rtfSpecialChar,	rtfFNoteCont,		"chftnsepc",	0,
	rtfSpecialChar,	rtfCell,		"cell",		0,
	rtfSpecialChar,	rtfRow,			"row",		0,
	rtfSpecialChar,	rtfPar,			"par",		0,
	/* newline and carriage return are synonyms for */
	/* \par when they are preceded by a \ character */
	rtfSpecialChar,	rtfPar,			"\n",		0,
	rtfSpecialChar,	rtfPar,			"\r",		0,
	rtfSpecialChar,	rtfSect,		"sect",		0,
	rtfSpecialChar,	rtfPage,		"page",		0,
	rtfSpecialChar,	rtfColumn,		"column",	0,
	rtfSpecialChar,	rtfLine,		"line",		0,
	rtfSpecialChar,	rtfSoftPage,		"softpage",	0,
	rtfSpecialChar,	rtfSoftColumn,		"softcol",	0,
	rtfSpecialChar,	rtfSoftLine,		"softline",	0,
	rtfSpecialChar,	rtfSoftLineHt,		"softlheight",	0,
	rtfSpecialChar,	rtfTab,			"tab",		0,
	rtfSpecialChar,	rtfEmDash,		"emdash",	0,
	rtfSpecialChar,	rtfEnDash,		"endash",	0,
	rtfSpecialChar,	rtfEmSpace,		"emspace",	0,
	rtfSpecialChar,	rtfEnSpace,		"enspace",	0,
	rtfSpecialChar,	rtfBullet,		"bullet",	0,
	rtfSpecialChar,	rtfLQuote,		"lquote",	0,
	rtfSpecialChar,	rtfRQuote,		"rquote",	0,
	rtfSpecialChar,	rtfLDblQuote,		"ldblquote",	0,
	rtfSpecialChar,	rtfRDblQuote,		"rdblquote",	0,
	rtfSpecialChar,	rtfFormula,		"|",		0,
	rtfSpecialChar,	rtfNoBrkSpace,		"~",		0,
	rtfSpecialChar,	rtfNoReqHyphen,		"-",		0,
	rtfSpecialChar,	rtfNoBrkHyphen,		"_",		0,
	rtfSpecialChar,	rtfOptDest,		"*",		0,
	rtfSpecialChar,	rtfLTRMark,		"ltrmark",	0,
	rtfSpecialChar,	rtfRTLMark,		"rtlmark",	0,
	rtfSpecialChar,	rtfNoWidthJoiner,	"zwj",		0,
	rtfSpecialChar,	rtfNoWidthNonJoiner,	"zwnj",		0,
	/* is this valid? */
	rtfSpecialChar,	rtfCurHeadPict,		"chpict",	0,
	rtfSpecialChar,	rtfIVersion,		"version",	0,

	/*
	 * Character formatting attributes
	 */

	rtfCharAttr,	rtfPlain,		"plain",	0,
	rtfCharAttr,	rtfBold,		"b",		0,
	rtfCharAttr,	rtfAllCaps,		"caps",		0,
	rtfCharAttr,	rtfCharGridNum,		"cgrid",	0, /*jrm21*/
	rtfCharAttr,	rtfDeleted,		"deleted",	0,
	rtfCharAttr,	rtfSubScript,		"dn",		0,
	rtfCharAttr,	rtfSubScrShrink,	"sub",		0,
	rtfCharAttr,	rtfNoSuperSub,		"nosupersub",	0,
	rtfCharAttr,	rtfExpand,		"expnd",	0,
	rtfCharAttr,	rtfExpandTwips,		"expndtw",	0,
	rtfCharAttr,	rtfKerning,		"kerning",	0,
	rtfCharAttr,	rtfLanguageFarEast,	"langfe",	0, /*jrm21*/
	rtfCharAttr,	rtfLanguageFarEastNoProof,"langfenp",	0, /*jrm21*/
	rtfCharAttr,	rtfLanguage,		"lang",		0,
	rtfCharAttr,	rtfLanguageNoProof,	"langnp",	0, /*jrm21*/
	rtfCharAttr,	rtfFontNum,		"f",		0,
	rtfCharAttr,	rtfFontSize,		"fs",		0,
	rtfCharAttr,	rtfItalic,		"i",		0,
	rtfCharAttr,	rtfOutline,		"outl",		0,
	rtfCharAttr,	rtfRevised,		"revised",	0,
	rtfCharAttr,	rtfRevAuthor,		"revauth",	0,
	rtfCharAttr,	rtfRevDTTM,		"revdttm",	0,
	rtfCharAttr,	rtfSmallCaps,		"scaps",	0,
	rtfCharAttr,	rtfShadow,		"shad",		0,
	rtfCharAttr,	rtfStrikeThru,		"strike",	0,
	rtfCharAttr,	rtfUnderline,		"ul",		0,
	rtfCharAttr,	rtfDotUnderline,	"uld",		0,
	rtfCharAttr,	rtfDbUnderline,		"uldb",		0,
	rtfCharAttr,	rtfNoUnderline,		"ulnone",	0,
	rtfCharAttr,	rtfWordUnderline,	"ulw",		0,
	rtfCharAttr,	rtfSuperScript,		"up",		0,
	rtfCharAttr,	rtfSuperScrShrink,	"super",	0,
	rtfCharAttr,	rtfInvisible,		"v",		0,
	rtfCharAttr,	rtfForeColor,		"cf",		0,
	rtfCharAttr,	rtfBackColor,		"cb",		0,
	rtfCharAttr,	rtfRTLChar,		"rtlch",	0,
	rtfCharAttr,	rtfLTRChar,		"ltrch",	0,
	rtfCharAttr,	rtfNoProof,		"noproof",	0,
	rtfCharAttr,	rtfCharStyleNum,	"cs",		0,
	rtfCharAttr,	rtfCharCharSet,		"cchs",		0,
	/* this has disappeared from spec 1.2 */
	rtfCharAttr,	rtfGray,		"gray",		0,

	/*
	 * Paragraph formatting attributes
	 */

	rtfParAttr,	rtfParDef,		"pard",		0,
	rtfParAttr,	rtfStyleNum,		"s",		0,
	rtfParAttr,	rtfHyphenate,		"hyphpar",	0,
	rtfParAttr,	rtfInTable,		"intbl",	0,
	rtfParAttr,	rtfParaNesting,		"itap",		0, /*jrm21*/
	rtfParAttr,	rtfKeep,		"keep",		0,
	rtfParAttr,	rtfKeepNext,		"keepn",	0,
	rtfParAttr,	rtfOutlineLevel,	"level",	0,
	rtfParAttr,	rtfNoLineNum,		"noline",	0,
	rtfParAttr,	rtfNoWidowControl,	"nowidctlpar",	0,
	rtfParAttr,	rtfWidowControl,	"widctlpar",	0, /*jrm21*/
	rtfParAttr,	rtfOutlineLevel,	"outlinelevel",	0, /*jrm21*/
	rtfParAttr,	rtfPBBefore,		"pagebb",	0,
	rtfParAttr,	rtfSideBySide,		"sbys",		0,
	rtfParAttr,	rtfQuadLeft,		"ql",		0,
	rtfParAttr,	rtfQuadRight,		"qr",		0,
	rtfParAttr,	rtfQuadJust,		"qj",		0,
	rtfParAttr,	rtfQuadCenter,		"qc",		0,
	rtfParAttr,	rtfFirstIndent,		"fi",		0,
	rtfParAttr,	rtfLeftIndent,		"li",		0,
	rtfParAttr,	rtfLeftIndent,		"lin",		0, /*jrm21*/
	rtfParAttr,	rtfRightIndent,		"ri",		0,
	rtfParAttr,	rtfRightIndent,		"rin",		0, /*jrm21*/
	rtfParAttr,	rtfRightIndent,		"curi",		0, /*jrm21*/
	rtfParAttr,	rtfRightIndentAdjust,	"adjustright",	0, /*jrm21*/
	rtfParAttr,	rtfSpaceBefore,		"sb",		0,
	rtfParAttr,	rtfSpaceAfter,		"sa",		0,
	rtfParAttr,	rtfSpaceBetween,	"sl",		0,
	rtfParAttr,	rtfSpaceMultiply,	"slmult",	0,

	rtfParAttr,	rtfSubDocument,		"subdocument",	0,

	rtfParAttr,	rtfRTLPar,		"rtlpar",	0,
	rtfParAttr,	rtfLTRPar,		"ltrpar",	0,

	rtfParAttr,	rtfTabPos,		"tx",		0,
	/*
	 * FrameMaker writes \tql (to mean left-justified tab, apparently)
	 * although it's not in the spec.  It's also redundant, since lj
	 * tabs are the default.
	 */
	rtfParAttr,	rtfTabLeft,		"tql",		0,
	rtfParAttr,	rtfTabRight,		"tqr",		0,
	rtfParAttr,	rtfTabCenter,		"tqc",		0,
	rtfParAttr,	rtfTabDecimal,		"tqdec",	0,
	rtfParAttr,	rtfTabBar,		"tb",		0,
	rtfParAttr,	rtfLeaderDot,		"tldot",	0,
	rtfParAttr,	rtfLeaderHyphen,	"tlhyph",	0,
	rtfParAttr,	rtfLeaderUnder,		"tlul",		0,
	rtfParAttr,	rtfLeaderThick,		"tlth",		0,
	rtfParAttr,	rtfLeaderEqual,		"tleq",		0,

	rtfParAttr,	rtfParLevel,		"pnlvl",	0,
	rtfParAttr,	rtfParBullet,		"pnlvlblt",	0,
	rtfParAttr,	rtfParSimple,		"pnlvlbody",	0,
	rtfParAttr,	rtfParNumCont,		"pnlvlcont",	0,
	rtfParAttr,	rtfParNumOnce,		"pnnumonce",	0,
	rtfParAttr,	rtfParNumAcross,	"pnacross",	0,
	rtfParAttr,	rtfParHangIndent,	"pnhang",	0,
	rtfParAttr,	rtfParNumRestart,	"pnrestart",	0,
	rtfParAttr,	rtfParNumCardinal,	"pncard",	0,
	rtfParAttr,	rtfParNumDecimal,	"pndec",	0,
	rtfParAttr,	rtfParNumULetter,	"pnucltr",	0,
	rtfParAttr,	rtfParNumURoman,	"pnucrm",	0,
	rtfParAttr,	rtfParNumLLetter,	"pnlcltr",	0,
	rtfParAttr,	rtfParNumLRoman,	"pnlcrm",	0,
	rtfParAttr,	rtfParNumOrdinal,	"pnord",	0,
	rtfParAttr,	rtfParNumOrdinalText,	"pnordt",	0,
	rtfParAttr,	rtfParNumBold,		"pnb",		0,
	rtfParAttr,	rtfParNumItalic,	"pni",		0,
	rtfParAttr,	rtfParNumAllCaps,	"pncaps",	0,
	rtfParAttr,	rtfParNumSmallCaps,	"pnscaps",	0,
	rtfParAttr,	rtfParNumUnder,		"pnul",		0,
	rtfParAttr,	rtfParNumDotUnder,	"pnuld",	0,
	rtfParAttr,	rtfParNumDbUnder,	"pnuldb",	0,
	rtfParAttr,	rtfParNumNoUnder,	"pnulnone",	0,
	rtfParAttr,	rtfParNumWordUnder,	"pnulw",	0,
	rtfParAttr,	rtfParNumStrikethru,	"pnstrike",	0,
	rtfParAttr,	rtfParNumForeColor,	"pncf",		0,
	rtfParAttr,	rtfParNumFont,		"pnf",		0,
	rtfParAttr,	rtfParNumFontSize,	"pnfs",		0,
	rtfParAttr,	rtfParNumIndent,	"pnindent",	0,
	rtfParAttr,	rtfParNumSpacing,	"pnsp",		0,
	rtfParAttr,	rtfParNumInclPrev,	"pnprev",	0,
	rtfParAttr,	rtfParNumCenter,	"pnqc",		0,
	rtfParAttr,	rtfParNumLeft,		"pnql",		0,
	rtfParAttr,	rtfParNumRight,		"pnqr",		0,
	rtfParAttr,	rtfParNumStartAt,	"pnstart",	0,

	rtfParAttr,	rtfBorderTop,		"brdrt",	0,
	rtfParAttr,	rtfBorderBottom,	"brdrb",	0,
	rtfParAttr,	rtfBorderLeft,		"brdrl",	0,
	rtfParAttr,	rtfBorderRight,		"brdrr",	0,
	rtfParAttr,	rtfBorderBetween,	"brdrbtw",	0,
	rtfParAttr,	rtfBorderBar,		"brdrbar",	0,
	rtfParAttr,	rtfBorderBox,		"box",		0,
	rtfParAttr,	rtfBorderSingle,	"brdrs",	0,
	rtfParAttr,	rtfBorderThick,		"brdrth",	0,
	rtfParAttr,	rtfBorderShadow,	"brdrsh",	0,
	rtfParAttr,	rtfBorderDouble,	"brdrdb",	0,
	rtfParAttr,	rtfBorderDot,		"brdrdot",	0,
	rtfParAttr,	rtfBorderDot,		"brdrdash",	0,
	rtfParAttr,	rtfBorderHair,		"brdrhair",	0,
	rtfParAttr,	rtfBorderWidth,		"brdrw",	0,
	rtfParAttr,	rtfBorderColor,		"brdrcf",	0,
	rtfParAttr,	rtfBorderSpace,		"brsp",		0,

	rtfParAttr,	rtfShading,		"shading",	0,
	rtfParAttr,	rtfBgPatH,		"bghoriz",	0,
	rtfParAttr,	rtfBgPatV,		"bgvert",	0,
	rtfParAttr,	rtfFwdDiagBgPat,	"bgfdiag",	0,
	rtfParAttr,	rtfBwdDiagBgPat,	"bgbdiag",	0,
	rtfParAttr,	rtfHatchBgPat,		"bgcross",	0,
	rtfParAttr,	rtfDiagHatchBgPat,	"bgdcross",	0,
	rtfParAttr,	rtfDarkBgPatH,		"bgdkhoriz",	0,
	rtfParAttr,	rtfDarkBgPatV,		"bgdkvert",	0,
	rtfParAttr,	rtfFwdDarkBgPat,	"bgdkfdiag",	0,
	rtfParAttr,	rtfBwdDarkBgPat,	"bgdkbdiag",	0,
	rtfParAttr,	rtfDarkHatchBgPat,	"bgdkcross",	0,
	rtfParAttr,	rtfDarkDiagHatchBgPat,	"bgdkdcross",	0,
	rtfParAttr,	rtfBgPatLineColor,	"cfpat",	0,
	rtfParAttr,	rtfBgPatColor,		"cbpat",	0,
	/* Word 97/2000 */
	rtfParAttr,	rtfParList,		"ls",	0, /*jrm21*/
	rtfParAttr,	rtfParListLevel,	"ilvl",	0, /*jrm21*/

	/*
	 * Section formatting attributes
	 */

	rtfSectAttr,	rtfSectDef,		"sectd",	0,
	rtfSectAttr,	rtfENoteHere,		"endnhere",	0,
	rtfSectAttr,	rtfPrtBinFirst,		"binfsxn",	0,
	rtfSectAttr,	rtfPrtBin,		"binsxn",	0,
	rtfSectAttr,	rtfSectStyleNum,	"ds",		0,

	rtfSectAttr,	rtfNoBreak,		"sbknone",	0,
	rtfSectAttr,	rtfColBreak,		"sbkcol",	0,
	rtfSectAttr,	rtfPageBreak,		"sbkpage",	0,
	rtfSectAttr,	rtfEvenBreak,		"sbkeven",	0,
	rtfSectAttr,	rtfOddBreak,		"sbkodd",	0,

	rtfSectAttr,	rtfColumns,		"cols",		0,
	rtfSectAttr,	rtfColumnSpace,		"colsx",	0,
	rtfSectAttr,	rtfColumnNumber,	"colno",	0,
	rtfSectAttr,	rtfColumnSpRight,	"colsr",	0,
	rtfSectAttr,	rtfColumnWidth,		"colw",		0,
	rtfSectAttr,	rtfColumnLine,		"linebetcol",	0,

	rtfSectAttr,	rtfLineModulus,		"linemod",	0,
	rtfSectAttr,	rtfLineDist,		"linex",	0,
	rtfSectAttr,	rtfLineStarts,		"linestarts",	0,
	rtfSectAttr,	rtfLineRestart,		"linerestart",	0,
	rtfSectAttr,	rtfLineRestartPg,	"lineppage",	0,
	rtfSectAttr,	rtfLineCont,		"linecont",	0,

	rtfSectAttr,	rtfSectPageWid,		"pgwsxn",	0,
	rtfSectAttr,	rtfSectPageHt,		"pghsxn",	0,
	rtfSectAttr,	rtfSectMarginLeft,	"marglsxn",	0,
	rtfSectAttr,	rtfSectMarginRight,	"margrsxn",	0,
	rtfSectAttr,	rtfSectMarginTop,	"margtsxn",	0,
	rtfSectAttr,	rtfSectMarginBottom,	"margbsxn",	0,
	rtfSectAttr,	rtfSectMarginGutter,	"guttersxn",	0,
	rtfSectAttr,	rtfSectLandscape,	"lndscpsxn",	0,
	rtfSectAttr,	rtfTitleSpecial,	"titlepg",	0,
	rtfSectAttr,	rtfHeaderY,		"headery",	0,
	rtfSectAttr,	rtfFooterY,		"footery",	0,

	rtfSectAttr,	rtfPageStarts,		"pgnstarts",	0,
	rtfSectAttr,	rtfPageCont,		"pgncont",	0,
	rtfSectAttr,	rtfPageRestart,		"pgnrestart",	0,
	rtfSectAttr,	rtfPageNumRight,	"pgnx",		0,
	rtfSectAttr,	rtfPageNumTop,		"pgny",		0,
	rtfSectAttr,	rtfPageDecimal,		"pgndec",	0,
	rtfSectAttr,	rtfPageURoman,		"pgnucrm",	0,
	rtfSectAttr,	rtfPageLRoman,		"pgnlcrm",	0,
	rtfSectAttr,	rtfPageULetter,		"pgnucltr",	0,
	rtfSectAttr,	rtfPageLLetter,		"pgnlcltr",	0,
	rtfSectAttr,	rtfPageNumHyphSep,	"pgnhnsh",	0,
	rtfSectAttr,	rtfPageNumSpaceSep,	"pgnhnsp",	0,
	rtfSectAttr,	rtfPageNumColonSep,	"pgnhnsc",	0,
	rtfSectAttr,	rtfPageNumEmdashSep,	"pgnhnsm",	0,
	rtfSectAttr,	rtfPageNumEndashSep,	"pgnhnsn",	0,

	rtfSectAttr,	rtfTopVAlign,		"vertalt",	0,
	/* misspelled as "vertal" in specification 1.0 */
	rtfSectAttr,	rtfBottomVAlign,	"vertalb",	0,
	rtfSectAttr,	rtfCenterVAlign,	"vertalc",	0,
	rtfSectAttr,	rtfJustVAlign,		"vertalj",	0,

	rtfSectAttr,	rtfRTLSect,		"rtlsect",	0,
	rtfSectAttr,	rtfLTRSect,		"ltrsect",	0,
	rtfSectAttr,	rtfHozSectRendering,	"horzsect",	0,
	rtfSectAttr,	rtfVertSectRendering,	"vertsect",	0,

	rtfSectAttr,	rtfSectExpand,		"sectexpand",	0,
	rtfSectAttr,	rtfSectLineGrid,	"sectlinegrid",	0,
	rtfSectAttr,	rtfSectDefaultState,	"sectdefaultcl",0,
	rtfSectAttr,	rtfSectCharsPerLine,	"sectspecifycl",	0,
	rtfSectAttr,	rtfSectCPLLPerPage,	"sectspecifyl",	0,
	rtfSectAttr,	rtfSectTextSnapToGrid,	"sectspecifygen",	0,


	rtfSectAttr,	rtfPageNumLevel,	"pgnhn",	0,
	rtfSectAttr,	rtfSectUnlocked,	"sectunlocked",	0,

	/* I've seen these in an old spec, but not in real files... */
	/*rtfSectAttr,	rtfNoBreak,		"nobreak",	0,*/
	/*rtfSectAttr,	rtfColBreak,		"colbreak",	0,*/
	/*rtfSectAttr,	rtfPageBreak,		"pagebreak",	0,*/
	/*rtfSectAttr,	rtfEvenBreak,		"evenbreak",	0,*/
	/*rtfSectAttr,	rtfOddBreak,		"oddbreak",	0,*/

	/*
	 * Document formatting attributes
	 */

	rtfDocAttr,	rtfDefTab,		"deftab",	0,
	rtfDocAttr,	rtfHyphHotZone,		"hyphhotz",	0,
	rtfDocAttr,	rtfHyphConsecLines,	"hyphconsec",	0,
	rtfDocAttr,	rtfHyphCaps,		"hyphcaps",	0,
	rtfDocAttr,	rtfHyphAuto,		"hyphauto",	0,
	rtfDocAttr,	rtfLineStart,		"linestart",	0,
	rtfDocAttr,	rtfFracWidth,		"fracwidth",	0,
	/* \makeback was given in old version of spec, it's now */
	/* listed as \makebackup */
	rtfDocAttr,	rtfMakeBackup,		"makeback",	0,
	rtfDocAttr,	rtfMakeBackup,		"makebackup",	0,
	rtfDocAttr,	rtfRTFDefault,		"defformat",	0,
	rtfDocAttr,	rtfPSOverlay,		"psover",	0,
	rtfDocAttr,	rtfDocTemplate,		"doctemp",	0,
	rtfDocAttr,	rtfDefLanguage,		"deflang",	0,
	rtfDocAttr,	rtfDefLanguageFarEast,	"deflangfe",	0, /*jrm21*/
	rtfDocAttr,	rtfWindowCaption,	"windowcaption",0, /*jrm21*/
	rtfDocAttr,	rtfAutoFormatDocType,	"doctype",	0, /*jrm21*/
	rtfDocAttr,	rtfOriginallyText,	"fromtext",	0, /*jrm21*/
	rtfDocAttr,	rtfOriginallyHTML,	"fromhtml",	0, /*jrm21*/
	rtfDocAttr,	rtfHozDocRendering,	"horzdoc",	0, /*jrm21*/
	rtfDocAttr,	rtfVertDocRendering,	"vertdoc",	0, /*jrm21*/
	rtfDocAttr,	rtfCompressingJust,	"jcompress",	0, /*jrm21*/
	rtfDocAttr,	rtfExpandingJust,	"jexpand",	0, /*jrm21*/
	rtfDocAttr,	rtfLineBasedOnGrid,	"lnongrid",	0, /*jrm21*/
	rtfDocAttr,	rtfViewKindNum,		"viewkind",	0, /*jrm21*/
	rtfDocAttr,	rtfViewScaleNum,	"viewscale",	0, /*jrm21*/
	rtfDocAttr,	rtfViewZoomKind,	"viewzk",	0, /*jrm21*/

	rtfDocAttr,	rtfFENoteType,		"fet",		0,
	rtfDocAttr,	rtfFNoteEndSect,	"endnotes",	0,
	rtfDocAttr,	rtfFNoteEndDoc,		"enddoc",	0,
	rtfDocAttr,	rtfFNoteText,		"ftntj",	0,
	rtfDocAttr,	rtfFNoteBottom,		"ftnbj",	0,
	rtfDocAttr,	rtfENoteEndSect,	"aendnotes",	0,
	rtfDocAttr,	rtfENoteEndDoc,		"aenddoc",	0,
	rtfDocAttr,	rtfENoteText,		"aftntj",	0,
	rtfDocAttr,	rtfENoteBottom,		"aftnbj",	0,
	rtfDocAttr,	rtfFNoteStart,		"ftnstart",	0,
	rtfDocAttr,	rtfENoteStart,		"aftnstart",	0,
	rtfDocAttr,	rtfFNoteRestartPage,	"ftnrstpg",	0,
	rtfDocAttr,	rtfFNoteRestart,	"ftnrestart",	0,
	rtfDocAttr,	rtfFNoteRestartCont,	"ftnrstcont",	0,
	rtfDocAttr,	rtfENoteRestart,	"aftnrestart",	0,
	rtfDocAttr,	rtfENoteRestartCont,	"aftnrstcont",	0,
	rtfDocAttr,	rtfFNoteNumArabic,	"ftnnar",	0,
	rtfDocAttr,	rtfFNoteNumLLetter,	"ftnnalc",	0,
	rtfDocAttr,	rtfFNoteNumULetter,	"ftnnauc",	0,
	rtfDocAttr,	rtfFNoteNumLRoman,	"ftnnrlc",	0,
	rtfDocAttr,	rtfFNoteNumURoman,	"ftnnruc",	0,
	rtfDocAttr,	rtfFNoteNumChicago,	"ftnnchi",	0,
	rtfDocAttr,	rtfENoteNumArabic,	"aftnnar",	0,
	rtfDocAttr,	rtfENoteNumLLetter,	"aftnnalc",	0,
	rtfDocAttr,	rtfENoteNumULetter,	"aftnnauc",	0,
	rtfDocAttr,	rtfENoteNumLRoman,	"aftnnrlc",	0,
	rtfDocAttr,	rtfENoteNumURoman,	"aftnnruc",	0,
	rtfDocAttr,	rtfENoteNumChicago,	"aftnnchi",	0,

	rtfDocAttr,	rtfPaperWidth,		"paperw",	0,
	rtfDocAttr,	rtfPaperHeight,		"paperh",	0,
	rtfDocAttr,	rtfPaperSize,		"psz",		0,
	rtfDocAttr,	rtfLeftMargin,		"margl",	0,
	rtfDocAttr,	rtfRightMargin,		"margr",	0,
	rtfDocAttr,	rtfTopMargin,		"margt",	0,
	rtfDocAttr,	rtfBottomMargin,	"margb",	0,
	rtfDocAttr,	rtfFacingPage,		"facingp",	0,
	rtfDocAttr,	rtfGutterWid,		"gutter",	0,
	rtfDocAttr,	rtfMirrorMargin,	"margmirror",	0,
	rtfDocAttr,	rtfLandscape,		"landscape",	0,
	rtfDocAttr,	rtfPageStart,		"pgnstart",	0,
	rtfDocAttr,	rtfWidowCtrl,		"widowctrl",	0,

	rtfDocAttr,	rtfLinkStyles,		"linkstyles",	0,

	rtfDocAttr,	rtfNoAutoTabIndent,	"notabind",	0,
	rtfDocAttr,	rtfWrapSpaces,		"wraptrsp",	0,
	rtfDocAttr,	rtfPrintColorsBlack,	"prcolbl",	0,
	rtfDocAttr,	rtfNoExtraSpaceRL,	"noextrasprl",	0,
	rtfDocAttr,	rtfNoColumnBalance,	"nocolbal",	0,
	rtfDocAttr,	rtfCvtMailMergeQuote,	"cvmme",	0,
	rtfDocAttr,	rtfSuppressTopSpace,	"sprstsp",	0,
	rtfDocAttr,	rtfSuppressPreParSpace,	"sprsspbf",	0,
	rtfDocAttr,	rtfCombineTblBorders,	"otblrul",	0,
	rtfDocAttr,	rtfTranspMetafiles,	"transmf",	0,
	rtfDocAttr,	rtfSwapBorders,		"swpbdr",	0,
	rtfDocAttr,	rtfShowHardBreaks,	"brkfrm",	0,

	rtfDocAttr,	rtfFormProtected,	"formprot",	0,
	rtfDocAttr,	rtfAllProtected,	"allprot",	0,
	rtfDocAttr,	rtfFormShading,		"formshade",	0,
	rtfDocAttr,	rtfFormDisplay,		"formdisp",	0,
	rtfDocAttr,	rtfPrintData,		"printdata",	0,

	rtfDocAttr,	rtfRevProtected,	"revprot",	0,
	rtfDocAttr,	rtfRevisions,		"revisions",	0,
	rtfDocAttr,	rtfRevDisplay,		"revprop",	0,
	rtfDocAttr,	rtfRevBar,		"revbar",	0,

	rtfDocAttr,	rtfAnnotProtected,	"annotprot",	0,

	rtfDocAttr,	rtfRTLDoc,		"rtldoc",	0,
	rtfDocAttr,	rtfLTRDoc,		"ltrdoc",	0,
	rtfDocAttr,	rtfClickAndType,		"cts",	0,
	rtfDocAttr,	rtfDrawGridHozSpace,	"dghspace",	0,
	rtfDocAttr,	rtfDrawGridVertSpace,	"dgvspace",	0,
	rtfDocAttr,	rtfDrawGridHozOrigin,	"dghorigin",	0,
	rtfDocAttr,	rtfDrawGridVertOrigin,	"dgvorigin",	0,
	rtfDocAttr,	rtfDrawGridShowHorLine,	"dghshow",	0,
	rtfDocAttr,	rtfDrawGridShowVertLine,"dgvshow",	0,
	rtfDocAttr,	rtfDrawGridSnapTo,	"dgsnap",	0,
	rtfDocAttr,	rtfDrawGridFollowMargins,	"dgmargin",	0,

	rtfDocAttr,	rtfPageBorderHead,	"pgbrdrhead",	0, /*jrm21*/
	rtfDocAttr,	rtfPageBorderFoot,	"pgbrdrfoot",	0, /*jrm21*/
	rtfDocAttr,	rtfAutoSpaceAlpha,	"aspalpha",	0, /*jrm21*/
	rtfDocAttr,	rtfAutoSpaceNum,	"aspnum",	0, /*jrm21*/
	rtfDocAttr,	rtfFontAlignAuto,	"faauto",	0, /*jrm21*/

	rtfDocAttr,	rtfNoLeadingXSpace,	"truncex",	0, /*jrm21*/
	rtfDocAttr,	rtfBodyBeforeHeader,	"bdbfhdr",	0, /*jrm21*/
	rtfDocAttr,	rtfNoBalanceSingleByte,	"dntblnsbdb",	0, /*jrm21*/
	rtfDocAttr,	rtfExpandCharSpaces,	"expshrtn",	0, /*jrm21*/
	rtfDocAttr,	rtfNoCentreLineHeights,	"lytexcttp",	0, /*jrm21*/
	rtfDocAttr,	rtfUsePrinterMetrics,	"lytprtmet",	0, /*jrm21*/
	rtfDocAttr,	rtfSmallCapsMacFive,	"msmcap",	0, /*jrm21*/
	rtfDocAttr,	rtfNoLeadMacFive,	"nolead",	0, /*jrm21*/
	rtfDocAttr,	rtfNoSpaceForUnderline,	"nospaceforul",	0, /*jrm21*/
	rtfDocAttr,	rtfNoUnderlineTrailSp,	"noultrlspc",	0, /*jrm21*/
	rtfDocAttr,	rtfNoBkSlashToYen,	"noxlattoyen",	0, /*jrm21*/
	rtfDocAttr,	rtfOldAutoSpacing,	"oldas",	0, /*jrm21*/
	rtfDocAttr,	rtfNoWord97LineBrkRules, "lnbrkrule",	0, /*jrm21*/
	rtfDocAttr,	rtfWord97BorderRules,	"bdrrlswsix",	0, /*jrm21*/
	rtfDocAttr,	rtfSuppressTopSomething,"sprstsm",	0, /*jrm21*/
	rtfDocAttr,	rtfNoAdjustLineHeightTbl, "nolnhtadjtbl", 0, /*jrm21*/


	/*
	 * Style attributes
	 */

	rtfStyleAttr,	rtfAdditive,		"additive",	0,
	rtfStyleAttr,	rtfBasedOn,		"sbasedon",	0,
	rtfStyleAttr,	rtfNext,		"snext",	0,
	rtfStyleAttr,	rtfStyleAutoUpdate,	"sautoupd",	0, /*jrm21*/

	/*
	 * Picture attributes
	 */

	rtfPictAttr,	rtfEMFPict,		"emfblip",	0,
	rtfPictAttr,	rtfPNGPict,		"pngblip",	0,
	rtfPictAttr,	rtfJPEGPict,		"jpegblip",	0,
	/* we probably want to ignore an shppict ? */
	rtfPictAttr,	rtfNotWord97Pict,	"nonshppict",	0, /*jrm21*/
	rtfPictAttr,	rtfMacQD,		"macpict",	0,
	rtfPictAttr,	rtfPMMetafile,		"pmmetafile",	0,
	rtfPictAttr,	rtfWinMetafile,		"wmetafile",	0,
	rtfPictAttr,	rtfDevIndBitmap,	"dibitmap",	0,
	rtfPictAttr,	rtfWinBitmap,		"wbitmap",	0,
	rtfPictAttr,	rtfPixelBits,		"wbmbitspixel",	0,
	rtfPictAttr,	rtfBitmapPlanes,	"wbmplanes",	0,
	rtfPictAttr,	rtfBitmapWid,		"wbmwidthbytes", 0,

	rtfPictAttr,	rtfPicWid,		"picw",		0,
	rtfPictAttr,	rtfPicHt,		"pich",		0,
	rtfPictAttr,	rtfPicGoalWid,		"picwgoal",	0,
	rtfPictAttr,	rtfPicGoalHt,		"pichgoal",	0,
	/* these two aren't in the spec, but some writers emit them */
	rtfPictAttr,	rtfPicGoalWid,		"picwGoal",	0,
	rtfPictAttr,	rtfPicGoalHt,		"pichGoal",	0,
	rtfPictAttr,	rtfPicScaleX,		"picscalex",	0,
	rtfPictAttr,	rtfPicScaleY,		"picscaley",	0,
	rtfPictAttr,	rtfPicScaled,		"picscaled",	0,
	rtfPictAttr,	rtfPicCropTop,		"piccropt",	0,
	rtfPictAttr,	rtfPicCropBottom,	"piccropb",	0,
	rtfPictAttr,	rtfPicCropLeft,		"piccropl",	0,
	rtfPictAttr,	rtfPicCropRight,	"piccropr",	0,

	rtfPictAttr,	rtfPicMFHasBitmap,	"picbmp",	0,
	rtfPictAttr,	rtfPicMFBitsPerPixel,	"picbpp",	0,

	rtfPictAttr,	rtfPicBinary,		"bin",		0,
	rtfPictAttr,	rtfPicUnitsPerInch,	"blipupi",	0,/*jrm21*/
/* if the following isa uncommented, then the pictures are screwed up */
	/*rtfPictAttr,	rtfPicIdent,		"blipuid",	0,*//*jrm21*/
	rtfPictAttr,	rtfPicTag,		"bliptag",	0,/*jrm21*/

	/*
	 * NeXT graphic attributes
	 */

	rtfNeXTGrAttr,	rtfNeXTGWidth,		"width",	0,
	rtfNeXTGrAttr,	rtfNeXTGHeight,		"height",	0,

	/*
	 * Destinations
	 */

	rtfDestination,	rtfFontTbl,		"fonttbl",	0,
	rtfDestination,	rtfFontAltName,		"falt",		0,
	rtfDestination,	rtfEmbeddedFont,	"fonteb",	0,
	rtfDestination,	rtfFontFile,		"fontfile",	0,
	rtfDestination,	rtfFileTbl,		"filetbl",	0,
	rtfDestination,	rtfFileInfo,		"file",		0,
	rtfDestination,	rtfColorTbl,		"colortbl",	0,
	rtfDestination,	rtfStyleSheet,		"stylesheet",	0,
	rtfDestination,	rtfKeyCode,		"keycode",	0,
	rtfDestination,	rtfRevisionTbl,		"revtbl",	0,
	rtfDestination,	rtfInfo,		"info",		0,
	rtfDestination,	rtfITitle,		"title",	0,
	rtfDestination,	rtfISubject,		"subject",	0,
	rtfDestination,	rtfIAuthor,		"author",	0,
	rtfDestination,	rtfIOperator,		"operator",	0,
	rtfDestination,	rtfIKeywords,		"keywords",	0,
	rtfDestination,	rtfIComment,		"comment",	0,
	rtfDestination,	rtfIDoccomm,		"doccomm",	0,
	rtfDestination,	rtfIVerscomm,		"verscomm",	0,
	rtfDestination,	rtfNextFile,		"nextfile",	0,
	rtfDestination,	rtfTemplate,		"template",	0,
	rtfDestination,	rtfFNSep,		"ftnsep",	0,
	rtfDestination,	rtfFNContSep,		"ftnsepc",	0,
	rtfDestination,	rtfFNContNotice,	"ftncn",	0,
	rtfDestination,	rtfENSep,		"aftnsep",	0,
	rtfDestination,	rtfENContSep,		"aftnsepc",	0,
	rtfDestination,	rtfENContNotice,	"aftncn",	0,
	rtfDestination,	rtfParNumLevelStyle,	"pnseclvl",	0,
	rtfDestination,	rtfHeader,		"header",	0,
	rtfDestination,	rtfFooter,		"footer",	0,
	rtfDestination,	rtfHeaderLeft,		"headerl",	0,
	rtfDestination,	rtfHeaderRight,		"headerr",	0,
	rtfDestination,	rtfHeaderFirst,		"headerf",	0,
	rtfDestination,	rtfFooterLeft,		"footerl",	0,
	rtfDestination,	rtfFooterRight,		"footerr",	0,
	rtfDestination,	rtfFooterFirst,		"footerf",	0,
	rtfDestination,	rtfParNumText,		"pntext",	0,
	rtfDestination,	rtfParNumbering,	"pn",		0,
	rtfDestination,	rtfParNumTextAfter,	"pntxta",	0,
	rtfDestination,	rtfParNumTextBefore,	"pntxtb",	0,
	rtfDestination,	rtfBookmarkStart,	"bkmkstart",	0,
	rtfDestination,	rtfBookmarkEnd,		"bkmkend",	0,
	rtfDestination,	rtfPict,		"pict",		0,
	rtfDestination,	rtfObject,		"object",	0,
	rtfDestination,	rtfObjClass,		"objclass",	0,
	rtfDestination,	rtfObjName,		"objname",	0,
	rtfDestination,	rtfObjTime,		"objtime",	0,
	rtfDestination,	rtfObjData,		"objdata",	0,
	rtfDestination,	rtfObjAlias,		"objalias",	0,
	rtfDestination,	rtfObjSection,		"objsect",	0,
	/* objitem and objtopic aren't documented in the spec! */
	rtfDestination,	rtfObjItem,		"objitem",	0,
	rtfDestination,	rtfObjTopic,		"objtopic",	0,
	rtfDestination,	rtfObjResult,		"result",	0,
	rtfDestination,	rtfDrawObject,		"do",		0,
	rtfDestination,	rtfFootnote,		"footnote",	0,
	rtfDestination,	rtfAnnotRefStart,	"atrfstart",	0,
	rtfDestination,	rtfAnnotRefEnd,		"atrfend",	0,
	rtfDestination,	rtfAnnotID,		"atnid",	0,
	rtfDestination,	rtfAnnotAuthor,		"atnauthor",	0,
	rtfDestination,	rtfAnnotation,		"annotation",	0,
	rtfDestination,	rtfAnnotRef,		"atnref",	0,
	rtfDestination,	rtfAnnotTime,		"atntime",	0,
	rtfDestination,	rtfAnnotIcon,		"atnicn",	0,
	rtfDestination,	rtfField,		"field",	0,
	rtfDestination,	rtfFieldInst,		"fldinst",	0,
	rtfDestination,	rtfFieldResult,		"fldrslt",	0,
	rtfDestination,	rtfDataField,		"datafield",	0,
	rtfDestination,	rtfIndex,		"xe",		0,
	rtfDestination,	rtfIndexText,		"txe",		0,
	rtfDestination,	rtfIndexRange,		"rxe",		0,
	rtfDestination,	rtfTOC,			"tc",		0,
	rtfDestination,	rtfNeXTGraphic,		"NeXTGraphic",	0,
	rtfDestination,	rtfNeXTHelpM,		"NeXTHelpMarker",	0,
	rtfDestination,	rtfNeXTHelpL,		"NeXTHelpLink",	0,
	rtfDestination,	rtfIBackupTime,		"buptim",	0,
	rtfDestination,	rtfICreateTime,		"creatim",	0,
	rtfDestination,	rtfIPrintTime,		"printim",	0,
	rtfDestination,	rtfIRevisionTime,	"revtim",	0,
	rtfDestination,	rtfDrawTextBoxText,	"dptxbxtext",	0,
	rtfDestination,	rtfFontEmbedded,	"fontemb",	0,
	rtfDestination,	rtfTOCN,			"tcn",		0,
	rtfDestination,	rtfWord97Pict,		"shppict",	0, /* jrm21 */
	rtfDestination,	rtfParListText,		"listtext",0, /*jrm21*/
	rtfDestination,	rtfPrivate,		"private",	0, /*jrm21*/
	rtfDestination,	rtfShapeGroup,		"shpgrp",	0, /*jrm21*/

	/*
	 * Font families
	 */

	rtfFontFamily,	rtfFFNil,		"fnil",		0,
	rtfFontFamily,	rtfFFRoman,		"froman",	0,
	rtfFontFamily,	rtfFFSwiss,		"fswiss",	0,
	rtfFontFamily,	rtfFFModern,		"fmodern",	0,
	rtfFontFamily,	rtfFFScript,		"fscript",	0,
	rtfFontFamily,	rtfFFDecor,		"fdecor",	0,
	rtfFontFamily,	rtfFFTech,		"ftech",	0,
	rtfFontFamily,	rtfFFBidirectional,	"fbidi",	0,

	/*
	 * Font attributes
	 */

	rtfFontAttr,	rtfFontCharSet,		"fcharset",	0,
	rtfFontAttr,	rtfFontPitch,		"fprq",		0,
	rtfFontAttr,	rtfFontCodePage,	"cpg",		0,
	rtfFontAttr,	rtfFTypeNil,		"ftnil",	0,
	rtfFontAttr,	rtfFTypeTrueType,	"fttruetype",	0,

	/*
	 * File table attributes
	 */

	rtfFileAttr,	rtfFileNum,		"fid",		0,
	rtfFileAttr,	rtfFileRelPath,		"frelative",	0,
	rtfFileAttr,	rtfFileOSNum,		"fosnum",	0,

	/*
	 * File sources
	 */

	rtfFileSource,	rtfSrcMacintosh,	"fvalidmac",	0,
	rtfFileSource,	rtfSrcDOS,		"fvaliddos",	0,
	rtfFileSource,	rtfSrcNTFS,		"fvalidntfs",	0,
	rtfFileSource,	rtfSrcHPFS,		"fvalidhpfs",	0,
	rtfFileSource,	rtfSrcNetwork,		"fnetwork",	0,

	/*
	 * Color names
	 */

	rtfColorName,	rtfRed,			"red",		0,
	rtfColorName,	rtfGreen,		"green",	0,
	rtfColorName,	rtfBlue,		"blue",		0,

	/*
	 * Charset names
	 */

	rtfCharSet,	rtfMacCharSet,		"mac",		0,
	rtfCharSet,	rtfAnsiCharSet,		"ansi",		0,
	rtfCharSet,	rtfPcCharSet,		"pc",		0,
	rtfCharSet,	rtfPcaCharSet,		"pca",		0,
	/* this isn't really a top-level charset, it is a unicode->charset
	   mapping that follows the definition of the charset... */
	rtfCharSet,	rtfAnsiCodePageCharSet,	"ansicpg",	0, /*jrm21*/
	/* and what about \uc1, to specify 1 byte per character? */


	/*
	 * Table attributes
	 */

	rtfTblAttr,	rtfRowDef,		"trowd",	0,
	rtfTblAttr,	rtfRowGapH,		"trgaph",	0,
	rtfTblAttr,	rtfCellPos,		"cellx",	0,
	rtfTblAttr,	rtfMergeRngFirst,	"clmgf",	0,
	rtfTblAttr,	rtfMergePrevious,	"clmrg",	0,

	rtfTblAttr,	rtfRowLeft,		"trql",		0,
	rtfTblAttr,	rtfRowRight,		"trqr",		0,
	rtfTblAttr,	rtfRowCenter,		"trqc",		0,
	rtfTblAttr,	rtfRowLeftEdge,		"trleft",	0,
	rtfTblAttr,	rtfRowHt,		"trrh",		0,
	rtfTblAttr,	rtfRowHeader,		"trhdr",	0,
	rtfTblAttr,	rtfRowKeep,		"trkeep",	0,

	rtfTblAttr,	rtfRTLRow,		"rtlrow",	0,
	rtfTblAttr,	rtfLTRRow,		"ltrrow",	0,

	rtfTblAttr,	rtfRowBordTop,		"trbrdrt",	0,
	rtfTblAttr,	rtfRowBordLeft,		"trbrdrl",	0,
	rtfTblAttr,	rtfRowBordBottom,	"trbrdrb",	0,
	rtfTblAttr,	rtfRowBordRight,	"trbrdrr",	0,
	rtfTblAttr,	rtfRowBordHoriz,	"trbrdrh",	0,
	rtfTblAttr,	rtfRowBordVert,		"trbrdrv",	0,

	rtfTblAttr,	rtfCellBordBottom,	"clbrdrb",	0,
	rtfTblAttr,	rtfCellBordTop,		"clbrdrt",	0,
	rtfTblAttr,	rtfCellBordLeft,	"clbrdrl",	0,
	rtfTblAttr,	rtfCellBordRight,	"clbrdrr",	0,

	rtfTblAttr,	rtfCellVertAlignTop,	"clvertalt",	0,/*jrm21*/
	rtfTblAttr,	rtfCellVertAlignCentre,	"clvertalc",	0,/*jrm21*/
	rtfTblAttr,	rtfCellVertAlignBottom,	"clvertalb",	0,/*jrm21*/
	rtfTblAttr,	rtfCellVertTextAlignL,	"cltxlrtb",	0,/*jrm21*/
	rtfTblAttr,	rtfCellVertTextAlignR,	"cltxlbrl",	0,/*jrm21*/

	rtfTblAttr,	rtfCellShading,		"clshdng",	0,
	rtfTblAttr,	rtfCellBgPatH,		"clbghoriz",	0,
	rtfTblAttr,	rtfCellBgPatV,		"clbgvert",	0,
	rtfTblAttr,	rtfCellFwdDiagBgPat,	"clbgfdiag",	0,
	rtfTblAttr,	rtfCellBwdDiagBgPat,	"clbgbdiag",	0,
	rtfTblAttr,	rtfCellHatchBgPat,	"clbgcross",	0,
	rtfTblAttr,	rtfCellDiagHatchBgPat,	"clbgdcross",	0,
	/*
	 * The spec lists "clbgdkhor", but the corresponding non-cell
	 * control is "bgdkhoriz".  At any rate Macintosh Word seems
	 * to accept both "clbgdkhor" and "clbgdkhoriz".
	 */
	rtfTblAttr,	rtfCellDarkBgPatH,	"clbgdkhoriz",	0,
	rtfTblAttr,	rtfCellDarkBgPatH,	"clbgdkhor",	0,
	rtfTblAttr,	rtfCellDarkBgPatV,	"clbgdkvert",	0,
	rtfTblAttr,	rtfCellFwdDarkBgPat,	"clbgdkfdiag",	0,
	rtfTblAttr,	rtfCellBwdDarkBgPat,	"clbgdkbdiag",	0,
	rtfTblAttr,	rtfCellDarkHatchBgPat,	"clbgdkcross",	0,
	rtfTblAttr,	rtfCellDarkDiagHatchBgPat, "clbgdkdcross",	0,
	rtfTblAttr,	rtfCellBgPatLineColor, "clcfpat",	0,
	rtfTblAttr,	rtfCellBgPatColor,	"clcbpat",	0,

	/*
	 * Field attributes
	 */

	rtfFieldAttr,	rtfFieldDirty,		"flddirty",	0,
	rtfFieldAttr,	rtfFieldEdited,		"fldedit",	0,
	rtfFieldAttr,	rtfFieldLocked,		"fldlock",	0,
	rtfFieldAttr,	rtfFieldPrivate,	"fldpriv",	0,
	rtfFieldAttr,	rtfFieldAlt,		"fldalt",	0,

	/*
	 * Positioning attributes
	 */

	rtfPosAttr,	rtfAbsWid,		"absw",		0,
	rtfPosAttr,	rtfAbsHt,		"absh",		0,

	rtfPosAttr,	rtfRPosMargH,		"phmrg",	0,
	rtfPosAttr,	rtfRPosPageH,		"phpg",		0,
	rtfPosAttr,	rtfRPosColH,		"phcol",	0,
	rtfPosAttr,	rtfPosX,		"posx",		0,
	rtfPosAttr,	rtfPosNegX,		"posnegx",	0,
	rtfPosAttr,	rtfPosXCenter,		"posxc",	0,
	rtfPosAttr,	rtfPosXInside,		"posxi",	0,
	rtfPosAttr,	rtfPosXOutSide,		"posxo",	0,
	rtfPosAttr,	rtfPosXRight,		"posxr",	0,
	rtfPosAttr,	rtfPosXLeft,		"posxl",	0,

	rtfPosAttr,	rtfRPosMargV,		"pvmrg",	0,
	rtfPosAttr,	rtfRPosPageV,		"pvpg",		0,
	rtfPosAttr,	rtfRPosParaV,		"pvpara",	0,
	rtfPosAttr,	rtfPosY,		"posy",		0,
	rtfPosAttr,	rtfPosNegY,		"posnegy",	0,
	rtfPosAttr,	rtfPosYInline,		"posyil",	0,
	rtfPosAttr,	rtfPosYTop,		"posyt",	0,
	rtfPosAttr,	rtfPosYCenter,		"posyc",	0,
	rtfPosAttr,	rtfPosYBottom,		"posyb",	0,

	rtfPosAttr,	rtfNoWrap,		"nowrap",	0,
	rtfPosAttr,	rtfDistFromTextAll,	"dxfrtext",	0,
	rtfPosAttr,	rtfDistFromTextX,	"dfrmtxtx",	0,
	rtfPosAttr,	rtfDistFromTextY,	"dfrmtxty",	0,
	/* \dyfrtext no longer exists in spec 1.2, apparently */
	/* replaced by \dfrmtextx and \dfrmtexty. */
	rtfPosAttr,	rtfTextDistY,		"dyfrtext",	0,

	rtfPosAttr,	rtfDropCapLines,	"dropcapli",	0,
	rtfPosAttr,	rtfDropCapType,		"dropcapt",	0,

	/*
	 * Object controls
	 */

	rtfObjAttr,	rtfObjEmb,		"objemb",	0,
	rtfObjAttr,	rtfObjLink,		"objlink",	0,
	rtfObjAttr,	rtfObjAutoLink,		"objautlink",	0,
	rtfObjAttr,	rtfObjSubscriber,	"objsub",	0,
	rtfObjAttr,	rtfObjPublisher,	"objpub",	0,
	rtfObjAttr,	rtfObjICEmb,		"objicemb",	0,

	rtfObjAttr,	rtfObjLinkSelf,		"linkself",	0,
	rtfObjAttr,	rtfObjLock,		"objupdate",	0,
	rtfObjAttr,	rtfObjUpdate,		"objlock",	0,

	rtfObjAttr,	rtfObjHt,		"objh",		0,
	rtfObjAttr,	rtfObjWid,		"objw",		0,
	rtfObjAttr,	rtfObjSetSize,		"objsetsize",	0,
	rtfObjAttr,	rtfObjAlign,		"objalign",	0,
	rtfObjAttr,	rtfObjTransposeY,	"objtransy",	0,
	rtfObjAttr,	rtfObjCropTop,		"objcropt",	0,
	rtfObjAttr,	rtfObjCropBottom,	"objcropb",	0,
	rtfObjAttr,	rtfObjCropLeft,		"objcropl",	0,
	rtfObjAttr,	rtfObjCropRight,	"objcropr",	0,
	rtfObjAttr,	rtfObjScaleX,		"objscalex",	0,
	rtfObjAttr,	rtfObjScaleY,		"objscaley",	0,

	rtfObjAttr,	rtfObjResRTF,		"rsltrtf",	0,
	rtfObjAttr,	rtfObjResPict,		"rsltpict",	0,
	rtfObjAttr,	rtfObjResBitmap,	"rsltbmp",	0,
	rtfObjAttr,	rtfObjResText,		"rslttxt",	0,
	rtfObjAttr,	rtfObjResMerge,		"rsltmerge",	0,

	rtfObjAttr,	rtfObjBookmarkPubObj,	"bkmkpub",	0,
	rtfObjAttr,	rtfObjPubAutoUpdate,	"pubauto",	0,

	/*
	 * Associated character formatting attributes
	 */

	rtfACharAttr,	rtfACBold,		"ab",		0,
	rtfACharAttr,	rtfACAllCaps,		"acaps",	0,
	rtfACharAttr,	rtfACForeColor,		"acf",		0,
	rtfACharAttr,	rtfACSubScript,		"adn",		0,
	rtfACharAttr,	rtfACExpand,		"aexpnd",	0,
	rtfACharAttr,	rtfACFontNum,		"af",		0,
	rtfACharAttr,	rtfACFontSize,		"afs",		0,
	rtfACharAttr,	rtfACItalic,		"ai",		0,
	rtfACharAttr,	rtfACLanguage,		"alang",	0,
	rtfACharAttr,	rtfACOutline,		"aoutl",	0,
	rtfACharAttr,	rtfACSmallCaps,		"ascaps",	0,
	rtfACharAttr,	rtfACShadow,		"ashad",	0,
	rtfACharAttr,	rtfACStrikeThru,	"astrike",	0,
	rtfACharAttr,	rtfACUnderline,		"aul",		0,
	rtfACharAttr,	rtfACDotUnderline,	"auld",		0,
	rtfACharAttr,	rtfACDbUnderline,	"auldb",	0,
	rtfACharAttr,	rtfACNoUnderline,	"aulnone",	0,
	rtfACharAttr,	rtfACWordUnderline,	"aulw",		0,
	rtfACharAttr,	rtfACSuperScript,	"aup",		0,

	/*
	 * Footnote attributes
	 */

	rtfFNoteAttr,	rtfFNAlt,		"ftnalt",	0,

	/*
	 * Key code attributes
	 */

	rtfKeyCodeAttr,	rtfAltKey,		"alt",		0,
	rtfKeyCodeAttr,	rtfShiftKey,		"shift",	0,
	rtfKeyCodeAttr,	rtfControlKey,		"ctrl",		0,
	rtfKeyCodeAttr,	rtfFunctionKey,		"fn",		0,

	/*
	 * Bookmark attributes
	 */

	rtfBookmarkAttr, rtfBookmarkFirstCol,	"bkmkcolf",	0,
	rtfBookmarkAttr, rtfBookmarkLastCol,	"bkmkcoll",	0,

	/*
	 * Index entry attributes
	 */

	rtfIndexAttr,	rtfIndexNumber,		"xef",		0,
	rtfIndexAttr,	rtfIndexBold,		"bxe",		0,
	rtfIndexAttr,	rtfIndexItalic,		"ixe",		0,

	/*
	 * Table of contents attributes
	 */

	rtfTOCAttr,	rtfTOCType,		"tcf",		0,
	rtfTOCAttr,	rtfTOCLevel,		"tcl",		0,

	/*
	 * Drawing object attributes
	 */

	rtfDrawAttr,	rtfDrawLock,		"dolock",	0,
	rtfDrawAttr,	rtfDrawPageRelX,	"dobxpage",	0,
	rtfDrawAttr,	rtfDrawColumnRelX,	"dobxcolumn",	0,
	rtfDrawAttr,	rtfDrawMarginRelX,	"dobxmargin",	0,
	rtfDrawAttr,	rtfDrawPageRelY,	"dobypage",	0,
	rtfDrawAttr,	rtfDrawMarginRelY,	"dobymargin",	0,
	rtfDrawAttr,	rtfDrawHeight,		"dodhgt",	0,

	rtfDrawAttr,	rtfDrawBeginGroup,	"dpgroup",	0,
	rtfDrawAttr,	rtfDrawGroupCount,	"dpcount",	0,
	rtfDrawAttr,	rtfDrawEndGroup,	"dpendgroup",	0,
	rtfDrawAttr,	rtfDrawArc,		"dparc",	0,
	rtfDrawAttr,	rtfDrawCallout,		"dpcallout",	0,
	rtfDrawAttr,	rtfDrawEllipse,		"dpellipse",	0,
	rtfDrawAttr,	rtfDrawLine,		"dpline",	0,
	rtfDrawAttr,	rtfDrawPolygon,		"dppolygon",	0,
	rtfDrawAttr,	rtfDrawPolyLine,	"dppolyline",	0,
	rtfDrawAttr,	rtfDrawRect,		"dprect",	0,
	rtfDrawAttr,	rtfDrawTextBox,		"dptxbx",	0,

	rtfDrawAttr,	rtfDrawOffsetX,		"dpx",		0,
	rtfDrawAttr,	rtfDrawSizeX,		"dpxsize",	0,
	rtfDrawAttr,	rtfDrawOffsetY,		"dpy",		0,
	rtfDrawAttr,	rtfDrawSizeY,		"dpysize",	0,

	rtfDrawAttr,	rtfCOAngle,		"dpcoa",	0,
	rtfDrawAttr,	rtfCOAccentBar,		"dpcoaccent",	0,
	rtfDrawAttr,	rtfCOBestFit,		"dpcobestfit",	0,
	rtfDrawAttr,	rtfCOBorder,		"dpcoborder",	0,
	rtfDrawAttr,	rtfCOAttachAbsDist,	"dpcodabs",	0,
	rtfDrawAttr,	rtfCOAttachBottom,	"dpcodbottom",	0,
	rtfDrawAttr,	rtfCOAttachCenter,	"dpcodcenter",	0,
	rtfDrawAttr,	rtfCOAttachTop,		"dpcodtop",	0,
	rtfDrawAttr,	rtfCOLength,		"dpcolength",	0,
	rtfDrawAttr,	rtfCONegXQuadrant,	"dpcominusx",	0,
	rtfDrawAttr,	rtfCONegYQuadrant,	"dpcominusy",	0,
	rtfDrawAttr,	rtfCOOffset,		"dpcooffset",	0,
	rtfDrawAttr,	rtfCOAttachSmart,	"dpcosmarta",	0,
	rtfDrawAttr,	rtfCODoubleLine,	"dpcotdouble",	0,
	rtfDrawAttr,	rtfCORightAngle,	"dpcotright",	0,
	rtfDrawAttr,	rtfCOSingleLine,	"dpcotsingle",	0,
	rtfDrawAttr,	rtfCOTripleLine,	"dpcottriple",	0,

	rtfDrawAttr,	rtfDrawTextBoxMargin,	"dptxbxmar",	0,
	rtfDrawAttr,	rtfDrawRoundRect,	"dproundr",	0,

	rtfDrawAttr,	rtfDrawPointX,		"dpptx",	0,
	rtfDrawAttr,	rtfDrawPointY,		"dppty",	0,
	rtfDrawAttr,	rtfDrawPolyCount,	"dppolycount",	0,

	rtfDrawAttr,	rtfDrawArcFlipX,	"dparcflipx",	0,
	rtfDrawAttr,	rtfDrawArcFlipY,	"dparcflipy",	0,

	rtfDrawAttr,	rtfDrawLineBlue,	"dplinecob",	0,
	rtfDrawAttr,	rtfDrawLineGreen,	"dplinecog",	0,
	rtfDrawAttr,	rtfDrawLineRed,		"dplinecor",	0,
	rtfDrawAttr,	rtfDrawLinePalette,	"dplinepal",	0,
	rtfDrawAttr,	rtfDrawLineDashDot,	"dplinedado",	0,
	rtfDrawAttr,	rtfDrawLineDashDotDot,	"dplinedadodo",	0,
	rtfDrawAttr,	rtfDrawLineDash,	"dplinedash",	0,
	rtfDrawAttr,	rtfDrawLineDot,		"dplinedot",	0,
	rtfDrawAttr,	rtfDrawLineGray,	"dplinegray",	0,
	rtfDrawAttr,	rtfDrawLineHollow,	"dplinehollow",	0,
	rtfDrawAttr,	rtfDrawLineSolid,	"dplinesolid",	0,
	rtfDrawAttr,	rtfDrawLineWidth,	"dplinew",	0,

	rtfDrawAttr,	rtfDrawHollowEndArrow,	"dpaendhol",	0,
	rtfDrawAttr,	rtfDrawEndArrowLength,	"dpaendl",	0,
	rtfDrawAttr,	rtfDrawSolidEndArrow,	"dpaendsol",	0,
	rtfDrawAttr,	rtfDrawEndArrowWidth,	"dpaendw",	0,
	rtfDrawAttr,	rtfDrawHollowStartArrow,"dpastarthol",	0,
	rtfDrawAttr,	rtfDrawStartArrowLength,"dpastartl",	0,
	rtfDrawAttr,	rtfDrawSolidStartArrow,	"dpastartsol",	0,
	rtfDrawAttr,	rtfDrawStartArrowWidth,	"dpastartw",	0,

	rtfDrawAttr,	rtfDrawBgFillBlue,	"dpfillbgcb",	0,
	rtfDrawAttr,	rtfDrawBgFillGreen,	"dpfillbgcg",	0,
	rtfDrawAttr,	rtfDrawBgFillRed,	"dpfillbgcr",	0,
	rtfDrawAttr,	rtfDrawBgFillPalette,	"dpfillbgpal",	0,
	rtfDrawAttr,	rtfDrawBgFillGray,	"dpfillbggray",	0,
	rtfDrawAttr,	rtfDrawFgFillBlue,	"dpfillfgcb",	0,
	rtfDrawAttr,	rtfDrawFgFillGreen,	"dpfillfgcg",	0,
	rtfDrawAttr,	rtfDrawFgFillRed,	"dpfillfgcr",	0,
	rtfDrawAttr,	rtfDrawFgFillPalette,	"dpfillfgpal",	0,
	rtfDrawAttr,	rtfDrawFgFillGray,	"dpfillfggray",	0,
	rtfDrawAttr,	rtfDrawFillPatIndex,	"dpfillpat",	0,

	rtfDrawAttr,	rtfDrawShadow,		"dpshadow",	0,
	rtfDrawAttr,	rtfDrawShadowXOffset,	"dpshadx",	0,
	rtfDrawAttr,	rtfDrawShadowYOffset,	"dpshady",	0,
	rtfDrawAttr,	rtfDrawParaRelY,	"dobypara",	0,


	rtfVersion,	-1,			"rtf",		0,
	rtfDefFont,	-1,			"deff",		0,
};


int main (int argc, char *argv[])
{
RTFKey	*rp;
int	i, nKeys;
char	*s, c;

	if (freopen ("rtf-ctrl", "w", stdout) == NULL)
	{
		fprintf (stderr, "cannot open rtf-ctrl file.\n");
		exit (1);
	}

	nKeys = sizeof (rtfKey) / sizeof (RTFKey);
	fprintf (stderr, "number of keys: %d\n", nKeys);
	printf ("# number of control words\n");
	printf ("%d\n", nKeys);

	printf ("# control word table\n");

	for (i = 0; i < nKeys; i++)
	{
		rp = &rtfKey[i];
		printf ("%d %d \"", rp->major, rp->minor);
		s = rp->str;
		while ((c = *s++) != '\0')
		{
			switch (c)
			{
			default:
				putchar (c);
				break;
			case '\n':
				printf ("\\n");
				break;
			case '\r':
				printf ("\\r");
				break;
			case '"':
				printf ("\\%c", c);
				break;
			}
		}
		printf ("\"\n");
	}
	exit (0);
}

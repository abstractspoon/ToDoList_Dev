// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2004 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// http://www.fossware.com
// mailto:foss@fossware.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#if (!defined __PROF_UIS_H)
#define __PROF_UIS_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#ifndef __AFXWIN_H__
	#include <AfxWin.h>         // MFC core and standard components
#endif

#ifndef __AFXEXT_H__
	#include <afxext.h>         // MFC extensions
#endif

//#if (!defined __AFXPRIV_H__)
//	#include <AfxPriv.h>
//#endif
//#if _MFC_VER < 0x700
//	#include <../src/AfxImpl.h>
//#else
//	#include <../src/mfc/AfxImpl.h>
//#endif


#ifndef _AFX_NO_OLE_SUPPORT
	#ifndef __AFXOLE_H__
		#include <AfxOle.h>     // MFC OLE classes
	#endif
	#ifndef __AFXODLGS_H__
		#include <AfxOdlgs.h>   // MFC OLE dialog classes
	#endif
	#ifndef __AFXDISP_H__
		#include <AfxDisp.h>    // MFC Automation classes
	#endif
#endif // _AFX_NO_OLE_SUPPORT

#ifndef __AFXTEMPL_H__
	#include <AfxTempl.h>
#endif

#ifndef __AFXMT_H__
	#include <AfxMT.h>
#endif

#ifndef __AFXCMN_H__
	#include <AfxCmn.h>
#endif

#ifndef __AFXDTCTL_H__
	#include <AfxDtCtl.h>
#endif

#ifndef __AFXCONV_H__
	#include <AfxConv.h>
#endif

#define __EXT_MFC_NO_TAB_CTRL
#define __EXT_MFC_NO_TABMDI_CTRL
#define __EXT_MFC_NO_TABFLAT_CTRL
#define __EXT_MFC_NO_TAB_CONTROLBARS
#define __EXT_MFC_NO_TOOLBOX_CTRL
#define __EXT_MFC_NO_SHORTCUTLIST_CTRL
#define __EXT_MFC_NO_PAGECONTAINER_CTRL
//#define __EXT_MFC_NO_SCROLLWND
#define __EXT_MFC_NO_SCROLLITEMWND
#define __EXT_MFC_NO_GRIDBASEWND
#define __EXT_MFC_NO_GRIDWND
#define __EXT_MFC_NO_IMAGEEDITWND
#define __EXT_MFC_NO_COLORPALETTE
#define __EXT_MFC_NO_ICONEDITDLG
#define __EXT_MFC_NO_BUILTIN_TEXTFIELD
#define __EXT_MFC_NO_CUSTOMIZE
#define __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL
#define __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL
#define __EXT_MFC_NO_EXTCONTENTEXPANDWND_H

#if (!defined _AFXDLL || defined __STATPROFUIS_WITH_DLLMFC__)
	#define __EXT_MFC_NO_CUSTOMIZE
#endif // (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)

#if (defined __EXT_MFC_NO_TAB_CTRL)
	#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
		#define   __EXT_MFC_NO_TABMDI_CTRL
	#endif
	#if (!defined __EXT_MFC_NO_TABFLAT_CTRL)
		#define   __EXT_MFC_NO_TABFLAT_CTRL
	#endif
	#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		#define   __EXT_MFC_NO_TAB_CONTROLBARS
	#endif
	#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL)
		#define   __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL
	#endif
	#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL)
		#define   __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL
	#endif
#endif // (defined __EXT_MFC_NO_TAB_CTRL)

#if (defined __EXT_MFC_NO_SCROLLWND)
	#if (!defined __EXT_MFC_NO_IMAGEEDITWND)
		#define   __EXT_MFC_NO_IMAGEEDITWND
	#endif
	#if (!defined __EXT_MFC_NO_COLORPALETTE)
		#define   __EXT_MFC_NO_COLORPALETTE
	#endif
	#if (!defined __EXT_MFC_NO_SCROLLITEMWND)
		#define   __EXT_MFC_NO_SCROLLITEMWND
	#endif
#endif // (defined __EXT_MFC_NO_SCROLLWND)

#if (defined __EXT_MFC_NO_SCROLLITEMWND)
	#if (!defined __EXT_MFC_NO_GRIDBASEWND)
		#define  __EXT_MFC_NO_GRIDBASEWND
	#endif
#endif

#if (defined __EXT_MFC_NO_GRIDBASEWND)
	#if (!defined __EXT_MFC_NO_GRIDWND)
		#define  __EXT_MFC_NO_GRIDWND
	#endif
#endif

#if (defined __EXT_MFC_NO_IMAGEEDITWND) || (defined __EXT_MFC_NO_COLORPALETTE)
	#if (!defined __EXT_MFC_NO_ICONEDITDLG)
		#define   __EXT_MFC_NO_ICONEDITDLG
	#endif
#endif // (defined __EXT_MFC_NO_IMAGEEDITWND) || (defined __EXT_MFC_NO_COLORPALETTE)

#if (defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL)
	#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL)
		#define  __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL
	#endif
#endif

#if (defined __EXT_MFC_NO_TABFLAT_CTRL)
	#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL)
		#define  __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL
	#endif
#endif

#if (!defined __PROF_UIS_IMPL__)

	#if (!defined __EXTBTNONFLAT_H)
		#include <ExtBtnOnFlat.h>
	#endif

	#if (!defined __EXT_BUTTON_H)
		#include <ExtButton.h>
	#endif

	#if (!defined __EXT_COMBO_BOX_H)
		#include <ExtComboBox.h>
	#endif

	#if (!defined __EXT_EDIT_H)
		#include <ExtEdit.h>
	#endif

	#if (!defined __EXT_CMD_ICON_H)
		#include <ExtCmdIcon.h>
	#endif

	#if (!defined __ExtCmdManager_H)
		#include <ExtCmdManager.h>
	#endif

	#if (!defined __EXT_PAINT_MANAGER_H)
		#include <ExtPaintManager.h>
	#endif

	#if (!defined __EXT_CONTROLBAR_H)
		#include <ExtControlBar.h>
	#endif

	#if (!defined __EXT_TOOLCONTROLBAR_H)
		#include <ExtToolControlBar.h>
	#endif

	#if (!defined __EXT_MENUCONTROLBAR_H)
		#include <ExtMenuControlBar.h>
	#endif

	#if (!defined __EXT_STATUSCONTROLBAR_H)
		#include <ExtStatusControlBar.h>
	#endif

	#if (!defined __EXT_POPUP_MENU_WND_H)
		#include <ExtPopupMenuWnd.h>
	#endif
	#if (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)
		#if (!defined __EXT_POPUP_CTRL_MENU_H)
			#include <ExtPopupCtrlMenu.h>
		#endif
	#endif // (!defined __EXT_MFC_NO_BUILTIN_TEXTFIELD)

	#if (!defined __EXT_RESIZABLE_DIALOG_H)
		#include <ExtResizableDialog.h>
	#endif

	#if (!defined __EXTCOLORCTRL_H)
		#include <ExtColorCtrl.h>
	#endif

	#if (!defined __EXTCOLORDLG_H)
		#include <ExtColorDlg.h>
	#endif

	#if (!defined __EXT_TEMPL_H)
		#include <ExtTempl.h>
	#endif

	#if (!defined __EXT_MFC_NO_TAB_CTRL)
		#if (!defined __EXT_TABWND_H)
			#include <ExtTabWnd.h>
		#endif
	#endif // (!defined __EXT_MFC_NO_TAB_CTRL)

	#if (!defined __EXT_MFC_NO_TABFLAT_CTRL)
		#if (! defined __EXT_TAB_FLAT_WND_H)
			#include <ExtTabFlatWnd.h>
		#endif
	#endif // (!defined __EXT_MFC_NO_TABFLAT_CTRL)

	#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL)
		#if (! defined __EXT_TAB_PAGE_CONTAINER_WND_H)
			#include <ExtTabPageContainerWnd.h>
		#endif
	#endif // (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL)

	#if (!defined __EXT_MFC_NO_TOOLBOX_CTRL)
		#if (!defined __EXTTOOLBOXWND_H)
			#include <ExtToolBoxWnd.h>
		#endif
	#endif // (!defined __EXT_MFC_NO_TOOLBOX_CTRL)

	#if (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)
		#if (!defined __EXTSHORTCUTLISTWND_H)
			#include <ExtShortcutListWnd.h>
		#endif
	#endif // (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)
	
	#if (!defined __EXT_MFC_NO_PAGECONTAINER_CTRL)
		#if (!defined __EXT_PAGECONTAINERWND_H)
			#include <ExtPageContainerWnd.h>
		#endif
	#endif //  (!defined __EXT_MFC_NO_PAGECONTAINER_CTRL)

	#if (!defined __EXT_MFC_NO_SCROLLWND)
		#if (!defined __EXT_SCROLLWND_H)
			#include <ExtScrollWnd.h>
		#endif 
	#endif // #if (!defined __EXT_MFC_NO_SCROLLWND)

	#if (!defined __EXT_MFC_NO_GRIDBASEWND)
		#if (!defined __EXT_GRIDWND_H)
			#include <ExtGridWnd.h>
		#endif 
	#endif // #if (!defined __EXT_MFC_NO_GRIDBASEWND)

	#if (!defined __EXT_MFC_NO_IMAGEEDITWND)
		#if (!defined __EXT_IMAGEEDITWND_H)
			#include <ExtImageEditWnd.h>
		#endif
	#endif // #if (!defined __EXT_MFC_NO_IMAGEEDITWND)

	#if (!defined __EXT_MFC_NO_COLORPALETTE)
		#if (!defined __EXT_COLORPALETTE_WND_H)
			#include <ExtColorPaletteWnd.h>
		#endif
	#endif // #if (!defined __EXT_MFC_NO_COLORPALETTE)

	#if (!defined __EXT_MFC_NO_ICONEDITDLG)
		#if (!defined __EXT_ICONEDITDLG_H)
			#include <ExtIconEditDlg.h>
		#endif
	#endif // (!defined __EXT_MFC_NO_ICONEDITDLG)

	#if (!defined __EXT_MFC_NO_CUSTOMIZE)
		#if (!defined __EXTCUSTOMIZE_H)
			#include <ExtCustomize.h>
		#endif
	#endif // #if (!defined __EXT_MFC_NO_CUSTOMIZE)

#endif // !__PROF_UIS_IMPL__

#endif // __PROF_UIS_H



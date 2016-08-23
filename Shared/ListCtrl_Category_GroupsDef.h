#pragma once

// This header-file is an UGLY hack to enable Vista SDK features on VS2003

#define _WIN32_WINNT 0x0600

#define LVN_LINKCLICK       (LVN_FIRST-84)

#define LVGS_NORMAL         0x00000000
#define LVGS_COLLAPSED      0x00000001
#define LVGS_HIDDEN         0x00000002
#define LVGS_COLLAPSIBLE    0x00000008

#define LVGA_HEADER_LEFT    0x00000001
#define LVGA_HEADER_CENTER  0x00000002
#define LVGA_HEADER_RIGHT   0x00000004  // Don't forget to validate exclusivity
#define LVGA_FOOTER_LEFT    0x00000008
#define LVGA_FOOTER_CENTER  0x00000010
#define LVGA_FOOTER_RIGHT   0x00000020  // Don't forget to validate exclusivity

typedef struct tagLVHITTESTINFO2
{
    POINT pt;
    UINT flags;
    int iItem;
#if (_WIN32_IE >= 0x0300)
    int iSubItem;    // this is was NOT in win95.  valid only for LVM_SUBITEMHITTEST
#endif
#if _WIN32_WINNT >= 0x0600
    int iGroup; // readonly. index of group. only valid for owner data.
                // supports single item in multiple groups.
#endif
} LVHITTESTINFO2, *LPLVHITTESTINFO2;
#define LVHITTESTINFO tagLVHITTESTINFO2

#define LVHT_EX_GROUP_HEADER       0x10000000
#define LVHT_EX_GROUP_FOOTER       0x20000000
#define LVHT_EX_GROUP_COLLAPSE     0x40000000
#define LVHT_EX_GROUP_BACKGROUND   0x80000000
#define LVHT_EX_GROUP_STATEICON    0x01000000
#define LVHT_EX_GROUP_SUBSETLINK   0x02000000
#define LVHT_EX_GROUP              (LVHT_EX_GROUP_BACKGROUND | LVHT_EX_GROUP_COLLAPSE | LVHT_EX_GROUP_FOOTER | LVHT_EX_GROUP_HEADER | LVHT_EX_GROUP_STATEICON | LVHT_EX_GROUP_SUBSETLINK)

#define ListView_HitTestEx(hwndLV, pinfo) \
    (int)SNDMSG((hwndLV), LVM_HITTEST, (WPARAM)-1, (LPARAM)(LV_HITTESTINFO *)(pinfo))
#define LVM_GETGROUPCOUNT         (LVM_FIRST + 152)
#define ListView_GetGroupCount(hwnd) \
    SNDMSG((hwnd), LVM_GETGROUPCOUNT, (WPARAM)0, (LPARAM)0)
#define LVM_GETGROUPINFOBYINDEX         (LVM_FIRST + 153)
#define ListView_GetGroupInfoByIndex(hwnd, iIndex, pgrp) \
    SNDMSG((hwnd), LVM_GETGROUPINFOBYINDEX, (WPARAM)(iIndex), (LPARAM)(pgrp))
#define LVM_GETGROUPRECT               (LVM_FIRST + 98)
#define ListView_GetGroupRect(hwnd, iGroupId, type, prc) \
    SNDMSG((hwnd), LVM_GETGROUPRECT, (WPARAM)(iGroupId), \
        ((prc) ? (((RECT*)(prc))->top = (type)), (LPARAM)(RECT*)(prc) : (LPARAM)(RECT*)NULL))

#if _WIN32_WINNT >= 0x0600
typedef struct tagNMLVLINK
{
    NMHDR       hdr;
    LITEM       link;
    int         iItem;
    int         iSubItem;
} NMLVLINK,  *PNMLVLINK;
#endif

typedef struct tagLVGROUP2
{
    UINT    cbSize;
    UINT    mask;
    LPWSTR  pszHeader;
    int     cchHeader;

    LPWSTR  pszFooter;
    int     cchFooter;

    int     iGroupId;

    UINT    stateMask;
    UINT    state;
    UINT    uAlign;
#if _WIN32_WINNT >= 0x0600
    LPWSTR  pszSubtitle;
    UINT    cchSubtitle;
    LPWSTR  pszTask;
    UINT    cchTask;
    LPWSTR  pszDescriptionTop;
    UINT    cchDescriptionTop;
    LPWSTR  pszDescriptionBottom;
    UINT    cchDescriptionBottom;
    int     iTitleImage;
    int     iExtendedImage;
    int     iFirstItem;         // Read only
    UINT    cItems;             // Read only
    LPWSTR  pszSubsetTitle;     // NULL if group is not subset
    UINT    cchSubsetTitle;

#define LVGROUP_V5_SIZE CCSIZEOF_STRUCT(LVGROUP, uAlign)

#endif
} LVGROUP2, *PLVGROUP2;

#define LVGROUP tagLVGROUP2

#define LVGF_NONE				0x00000000
#define LVGF_HEADER				0x00000001
#define LVGF_FOOTER				0x00000002
#define LVGF_STATE				0x00000004
#define LVGF_ALIGN				0x00000008
#define LVGF_GROUPID			0x00000010
#define LVGF_SUBTITLE           0x00000100  // pszSubtitle is valid
#define LVGF_TASK               0x00000200  // pszTask is valid
#define LVGF_DESCRIPTIONTOP     0x00000400  // pszDescriptionTop is valid
#define LVGF_DESCRIPTIONBOTTOM  0x00000800  // pszDescriptionBottom is valid
#define LVGF_TITLEIMAGE         0x00001000  // iTitleImage is valid
#define LVGF_EXTENDEDIMAGE      0x00002000  // iExtendedImage is valid
#define LVGF_ITEMS              0x00004000  // iFirstItem and cItems are valid
#define LVGF_SUBSET             0x00008000  // pszSubsetTitle is valid
#define LVGF_SUBSETITEMS        0x00010000  // readonly, cItems holds count of items in visible subset, iFirstItem is valid
#ifndef INCLUDED_SHELLICONS_H
#define INCLUDED_SHELLICONS_H

#pragma once

namespace ShellIcons
{
	enum SHELLICON 
	{
		SI_UNKNOWN = 0,					///< Unknown File Type
		SI_DEF_DOCUMENT = 1,			///< Default document
		SI_DEF_APPLICATION = 2,			///< Default application
		SI_FOLDER_CLOSED = 3,			///< Closed folder
		SI_FOLDER_OPEN = 4,				///< Open folder
		SI_FLOPPY_514 = 5,				///< 5 1/4 floppy
		SI_FLOPPY_35 = 6,				///< 3 1/2 floppy
		SI_REMOVABLE = 7,				///< Removable drive
		SI_HDD = 8,						///< Hard disk drive
		SI_NETWORKDRIVE = 9,			///< Network drive
		SI_NETWORKDRIVE_DISCONNECTED = 10,	///< network drive offline
		SI_CDROM = 11,					///< CD drive
		SI_RAMDISK = 12,				///< RAM disk
		SI_NETWORK = 13,				///< Entire network
		// 14 = ?
		SI_MYCOMPUTER = 15,				///< My Computer
		SI_PRINTMANAGER = 16,			///< Printer Manager
		SI_NETWORK_NEIGHBORHOOD = 17,	///< Network Neighborhood
		SI_NETWORK_WORKGROUP = 18,		///< Network Workgroup
		SI_STARTMENU_PROGRAMS = 19,		///< Start Menu Programs
		SI_STARTMENU_DOCUMENTS = 20,	///< Start Menu Documents
		SI_STARTMENU_SETTINGS = 21,		///< Start Menu Settings
		SI_STARTMENU_FIND = 22,			///< Start Menu Find
		SI_STARTMENU_HELP = 23,			///< Start Menu Help
		SI_STARTMENU_RUN = 24,			///< Start Menu Run
		SI_STARTMENU_SUSPEND = 25,		///< Start Menu Suspend
		SI_STARTMENU_DOCKING = 26,		///< Start Menu Docking
		SI_STARTMENU_SHUTDOWN = 27,		///< Start Menu Shutdown
		SI_SHARE = 28,					///< Sharing overlay (hand)
		SI_SHORTCUT = 29,				///< Shortcut overlay (small arrow)
		SI_PRINTER_DEFAULT = 30,		///< Default printer overlay (small tick)
		SI_RECYCLEBIN_EMPTY = 31,		///< Recycle bin empty
		SI_RECYCLEBIN_FULL = 32,		///< Recycle bin full
		SI_DUN = 33,					///< Dial-up Network Folder
		SI_DESKTOP = 34,				///< Desktop
		SI_CONTROLPANEL = 35,			///< Control Panel
		SI_PROGRAMGROUPS = 36,			///< Program Group
		SI_PRINTER = 37,				///< Printer
		SI_FONT = 38,					///< Font Folder
		SI_TASKBAR = 39,				///< Taskbar
		SI_AUDIO_CD = 40,				///< Audio CD
		//41 = my computer
		//42 = ?
		SI_FAVORITES = 43,				///< IE favorites		
		SI_LOGOFF = 44,					///< Start Menu Logoff
		SI_LOCK = 47,					///< Lock
		SI_HIBERNATE = 48,				///< Hibernate
	};
	
	HICON ExtractIcon(SHELLICON nIndex, bool bLarge = false);
	BOOL DrawIcon(CDC* pDC, SHELLICON nIndex, const CPoint& ptTopLeft, bool bLarge = false);
}

/************************************************************************//**
 *
 *	Get a shell icon
 */

#endif // INCLUDED_SHELLICONS_H
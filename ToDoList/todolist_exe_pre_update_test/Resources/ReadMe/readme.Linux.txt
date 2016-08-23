version 1.1

Running TDL on Linux
::::::::::::::::::::

TDL can be run on Linux systems. This document outlines the necessary steps and gives some hints.

Installation
============
To run TDL on a Linux system there need to be WINE (http://wiki.winehq.org/) and a few specific libraries (DLLs) installed. TDL has been successfully tested on LinuxLite, Lubuntu, Fedora and Porteus with WINE stable and staging.

* using a GUI
-------------
Start your graphical package/software manager (like Synaptic or Yum Extender) and install:
	cabextract
	wine
	winetricks

If winetricks is not part of the repository, it cannot be installed via GUI or CLI. Instead, see here for download and execution/installation:
http://wiki.winehq.org/winetricks

Start wintetricks from your menu (Wine -> Winetricks) or the downloaded version via Terminal (see: http://wiki.winehq.org/winetricks) and install the following DLLs (-> Select the default wineprefix -> Install a Windows DLL or component):
	comctl32
	mfc42
	vcrun6
	
* using the terminal/ CLI
-------------------------
Run the following commands...
... on deb based systems:
	sudo apt-get install wine winetricks
	winetricks comctl32
	winetricks mfc42
	winetricks vcrun6

... on rpm based systems:
	sudo yum install wine winetricks
	winetricks comctl32
	winetricks mfc42
	winetricks vcrun6

If winetricks is not part of the repository, it cannot be installed via GUI or CLI. Instead, see here for download and execution/installation:
http://wiki.winehq.org/winetricks


Setup/ Start of TDL
===================

Assuming you have unzipped the todolist zip file into your home directory in a folder called todolist.

* using a launcher/ icon
------------------------

To create a menu entry under the category Office, create the following file in the given location with your text editor of choice:
~/.local/share/applications/ToDoList.desktop

Or create a file (launcher) ToDoList.desktop directly on your Desktop with your text editor of choice.

Content of ToDoList.desktop:

[Desktop Entry]
Name=ToDoList
GenericName=ToDoList
Comment=task manager
Exec=wine todolist/ToDoList.exe
Icon=application-x-wine-extension-tdl
Terminal=false
Type=Application
Categories=Office;Application;
Encoding=UTF-8
Name[en_US]=ToDoList
Comment[en_US]=task management tool


* using the terminal/ CLI
-------------------------
$ wine ~/todolist/ToDoList.exe


Linux specific settings/ limitations/ issues
============================================

TDL menu: -> Tools -> Preferences -> User Interface -> General:

- Use the following theme (XP and later only), i.e. not available in Linux

- Enable window fading when showing dialog boxes: not available in newer versions for Linux, needs to be deactivated in older versions!


Feedback/ Further Information
=============================
Comments and Discussions: 
http://www.codeproject.com/Articles/5371/WebControls/#_comments

GitHub/Wiki:
https://github.com/abstractspoon/ToDoList_Wiki/wiki
https://github.com/abstractspoon/ToDoList_Wiki/wiki/Linux

Youtube Channel:
https://www.youtube.com/channel/UCxvJumT59uFITT_QDGGRAhQ

Website:
http://www.abstractspoon.com/tdl_resources.html

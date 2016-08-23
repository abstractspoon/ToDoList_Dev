Wear.exe (joe porkka)

Yet another "which" command. I spelled it differently to make it distictive.

Reports *all* matches for an exe in the path, not just the first. (Has an option to print just the first.)

Supports wildcards. For example "Wear *list*", will match things like:
	tasklist.dll
	tlist.exe
	listdlls.exe
or, "wear *.bat *.cmd" will find all batch and cmd files in your path.

Can search different path environment variables, not just "PATH". (wear -var include stdio.h).

(Updated now to not require the VC7 msvcrt.dll)

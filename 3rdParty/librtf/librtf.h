/***************************************************************************
 *   Copyright (C) 2006 by Tomasz Hławiczka                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef ___LIBFTF_H___
#define ___LIBFTF_H___

#ifdef __cplusplus

// #include "librtf_cpp.h"

#endif // __cplusplus

#ifndef LIBRTF_VERSION
	#define LIBRTF_VERSION 1
#endif // LIBRTF_VERSION

#include "textproc.h"

enum rtf_warnings
{
	LIBRTF_WARN_UNKNOWN = 0,
	LIBRTF_WARN_UNKNOWN_COMMAND,
	LIBRTF_WARN_TOO_LONG_FONT_NAME,
	LIBRTF_WARN_COMMAND_WITH_PARAM,		// when command should be without param
	LIBRTF_WARN_COMMAND_WITHOUT_PARAM,	// when command should bw with param
	LIBRTF_WARN_COMMAND_NOT_FOUND,
};

enum rtf_errors
{
	LIBRTF_ERR_UNKNOWN = 0,
	LIBRTF_ERR_OUT_OF_MEMORY,
	LIBRTF_ERR_MORE_UNGET_NEEDED,
	LIBRTF_ERR_BAD_CHARACTER,
	LIBRTF_ERR_NOT_ALLOCATED_BUF,
	LIBRTF_ERR_TOO_LONG_WORD,
};

// Function for reading rtf source.
//__attribute__ ((visibility("default")))
extern void         rtf_process( source * src, processor * proc, observer * log );

// Functions for creating rtf output.
//__attribute__ ((visibility("default")))
extern processor *  create_rtf_processor( output * out  );

//__attribute__ ((visibility("default")))
extern void         destroy_rtf_processor( processor * proc );



#endif /// ___LIBFTF_H___

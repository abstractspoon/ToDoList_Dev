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

#ifndef ___LIBRTF_RTF_PROCESSOR_H___
#define ___LIBRTF_RTF_PROCESSOR_H___

#include <stdio.h> // for size_t

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#define SKIP_ONE_WORD	2

#define CHECK_PARAM_NOT_NULL(x) // FIXME: implementation needed

#ifdef _MSC_VER
#define ENV_MSVC
#else
#define ENV_GNU
#endif // _MSC_VER

#ifdef ENV_MSVC
#define snprintf _snprintf
#endif // ENV_MSVC

#include "librtf.h"
#include "textproc.h"

enum {
	CHARSET_ANSI=1,
	CHARSET_MAC,
	CHARSET_CP437,
	CHARSET_CP850,
};

typedef struct {
	unsigned short length, allocated;
	char * content;
} string;

typedef struct  {
	int             type;

	int             has_param;
	int             param;

	string          content;
} rtf_part;

enum
{
	PART_TYPE_TEXT,
	PART_TYPE_BLOCK_START,
	PART_TYPE_BLOCK_END,
	PART_TYPE_COMMAND,
	PART_TYPE_HEX_CHAR,
};

typedef struct internal_source {

	source   * iface;
	observer * log;

	rtf_part current_part;

	int use_current_part;

	int last_returned_ch;	// =0;
	int ungot_char;         // = -1;
	int ungot_char2;        // = -1;
	int ungot_char3;        //= -1;
	
	char * buf;
	unsigned long index, buf_size, read_bytes;

} internal_source;

typedef struct
{
	processor        * iface;
	observer         * log;
	internal_source  * src;

	codepage_info * charset_codepage;
	int   coming_pars_that_are_tabular; // = 0;
	int   have_printed_row_begin; // =FALSE;
	int   have_printed_cell_begin; // =FALSE;
	int   have_printed_row_end; // =FALSE;
	int   have_printed_cell_end; // =FALSE;
	int   within_table; // = FALSE;
	int   within_picture; // = FALSE;
	int   simulate_smallcaps;
	int   simulate_allcaps;
	short numchar_table;
	
	image_descr  current_image;
	image_mgr    * current_image_mgr;

} internal_processor;

rtf_part * get_next_part ( internal_source * src );
rtf_part * get_next_command( internal_source * src );
void skip_block_to_end( internal_source * src );
void skip_block_to_outside( internal_source * src );
void push_back_current_part ( internal_source * src );

string * append( string * str, const char * toadd );
void     string_destroy( string * str );
void     string_init( string * str );
void     string_reset( string * str );

#endif // ___LIBRTF_RTF_PROCESSOR_H___

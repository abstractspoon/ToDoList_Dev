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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "parser.h"

static void my_unget_char (internal_source * src, int ch)
{
	if (src->ungot_char>=0 && src->ungot_char2>=0 && src->ungot_char3>=0) 
	{
		src->log->error(src->log, LIBRTF_ERR_MORE_UNGET_NEEDED, "More than 3 ungot chars");
	}

	src->ungot_char3 = src->ungot_char2;
	src->ungot_char2 = src->ungot_char;
	src->ungot_char = ch;
}

static int get_next_char (internal_source * src)
{
	int ch;

	if (src->ungot_char>=0) {
		ch = src->ungot_char; 

		src->ungot_char = src->ungot_char2; 
		src->ungot_char2 = src->ungot_char3;
		src->ungot_char3 = -1;

		src->last_returned_ch = ch;
		if(ch > 255)
		{
			fprintf(stderr, "returning bad ch = '%c' (0%o)\n",
				ch, ch);
		}
		return ch;
	}
	do {

		if (src->index >= src->buf_size ) {
			if (!src->buf) {
				src->buf_size = 256;
				src->buf = (char*) malloc (src->buf_size);
				if (!src->buf)
				{
				//		rdr->observer->error(rdr->observer, ERR_OUT_OF_MEMORY, "Cannot allocate buffer for reading");
					return EOF;
				}
			}
			
			if( (src->read_bytes = src->iface->read(src->iface, src->buf, src->buf_size )) == 0 )
				return EOF;

			src->index = 0;
		}
		if ( src->read_bytes == src->index )
			return EOF;
				   
		ch = src->buf [src->index ++];

		if (ch=='\n') {
			if (src->last_returned_ch=='\\') {
				my_unget_char (src,' ');
				my_unget_char (src,'r');
				my_unget_char (src,'a');
				ch = 'p';
				break;
			}
		}
	} 
	while (ch=='\r' /* || ch=='\n' */ );

	if (ch=='\t') ch = ' ';

	src->last_returned_ch = ch;
	if(ch > 255)
	{
		char buf [ 64 ];
		sprintf(buf, "returning bad ch '%c' (0%o)", ch, ch );
		src->log->error(src->log, LIBRTF_ERR_BAD_CHARACTER, buf );
	}
	return ch;
}

static void 
append_to_part ( rtf_part * part, const char * toadd )
{
	append( & part->content, toadd );
}
		
static void  
append_char( rtf_part * part, char onechar )
{
	char buf [ 2 ] = { onechar, '\0' };
	append_to_part( part, buf );
}

void
push_back_current_part ( internal_source * src )
{
	src->use_current_part = TRUE;
}

void skip_block_to_end( internal_source * src )
{
	skip_block_to_outside( src );
	push_back_current_part( src );
}

void skip_block_to_outside( internal_source * src )
{
	int blocks = 0;
	rtf_part * part;
	part = get_next_part( src );
	while ( part )
	{
		if( part->type == PART_TYPE_BLOCK_START )
			blocks ++;
		else if ( part->type == PART_TYPE_BLOCK_END )
		{
			if ( blocks == 0 )
				return;
			blocks --;
		}
		part = get_next_part( src );
	}
}

rtf_part * get_next_command( internal_source * src )
{
	rtf_part * part = NULL;
	part = get_next_part ( src );
	while ( part )
	{
		if( part->type == PART_TYPE_COMMAND )
			return part;
		else if( part->type == PART_TYPE_BLOCK_START)
			skip_block_to_outside( src );	// anyway it should not happen.
		else if( part->type == PART_TYPE_BLOCK_END )
			break;
		part = get_next_part ( src );
	}
	return NULL;
}

rtf_part * get_next_part ( internal_source * src )
{
	int ch, ch2;
	rtf_part * part;
	int is_control_word = 0;

	int have_whitespace=0;
	int need_unget=0;
	int negation = 1;

	if( src->use_current_part == TRUE )
	{
		src->use_current_part = FALSE;
		return & src->current_part;
	}

	part = & src->current_part;
	
	part->param = 0;
	part->has_param = 0;
	part->type = PART_TYPE_TEXT;
	
	string_reset( & part->content );

	do {
		ch = get_next_char( src );
	} 
	while (ch == '\n');

	if (ch==' ')
	{
		// Compress multiple space chars down to one.

		while (ch == ' ') {
			ch = get_next_char(src);
			have_whitespace=TRUE;
		}
		if (have_whitespace) {
			my_unget_char (src, ch);
			append_char ( part, ' ');
			return part;
		}
	}

	switch(ch)
	{
	case EOF: 
		return 0;

	case '\\':
		ch2 = get_next_char( src );

		// Look for two-character command words.
		switch (ch2) 
		{
		case '\n':
			append_to_part( part, "\\par" );
			return part;
		case '~':
		case '{':
		case '}':
		case '\\':
		case '_':
		case '-':
			append_char( part, '\\' );
			append_char( part, ch2 );
			return part;

		case '\'':
			// Preserve \'## expressions (hex char exprs) for later. 
			// append( & part->word, "\\\'");
			append_char( part, get_next_char(src) ) ;
			append_char( part, get_next_char(src) );
			part->type = PART_TYPE_HEX_CHAR;
			return part;
		}

		part->type = PART_TYPE_COMMAND;
		is_control_word = 1;
		// append_char( & part->word, ch );
		ch=ch2;
		break;

	case '\t':
		// In RTF, a tab char is the same as \tab.
		append_to_part( part, "\\tab");
		return part;

	case '{':
		part->type = PART_TYPE_BLOCK_START;
		return part;	
	case '}':
		part->type = PART_TYPE_BLOCK_END;
		return part;
	case ';':
		append_char( part, ch );
		return part;
	}

	while (ch!=EOF)
	{
		// Several chars always ends a word, and we need to save them.
 
		if (ch=='\t' || ch=='{' || ch=='}' || ch=='\\') {
			need_unget=TRUE;
			break;
		}

		// A newline always ends a command word; we don't save it. 
  		//  A newline is ignored if this is not a command word.
 
		if (ch=='\n') { 
			if (is_control_word) 
				break;
			ch = get_next_char(src); 
			continue; 
		}

		// A semicolon always ends a command word; we do save it. 
  		// A semicolon never ends a regular word.
 
		if (ch==';') {
			if (is_control_word) {
				need_unget=TRUE;
				break;
			}
		}

		// In this parser, a space character terminates
  		// any word, and if it does not follow a command,
  		// then it is a word in itself.

		if (ch==' ') {
			if (!is_control_word)
				need_unget=TRUE;
			break;
		}

		// Identify a control word's numeric parameter.
		if (is_control_word) {
			if (!part->has_param && (isdigit(ch) || ch=='-'))
				part->has_param = TRUE;
			else
			if (part->has_param && !isdigit(ch)) {
				if (ch!=' ')
					need_unget=TRUE;
				break;
			}
		}
		
		if( part->has_param )
		{
			if( ch == '-' )
				negation = -1;
			else
				part->param = (part->param * 10) + (ch - '0');
		}
		else
			append_char( part, ch );
		
		if( part->type == PART_TYPE_TEXT && part->content.length >= 256)
		{
			// part->unfinised = TRUE;
			break;
		}
		
		ch = get_next_char (src);
	}

	if (need_unget)
		my_unget_char(src, ch);

	part->param *= negation;

	return part;
}

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


//#ifdef HAVE_CONFIG_H
//#include <config.h>
//#endif

//#ifdef HAVE_STDIO_H
#include <stdio.h>
//#endif

//#ifdef HAVE_STRING_H
#include <string.h>
//#endif

//#ifdef HAVE_STDLIB_H
#include <stdlib.h>
//#endif

#include "parser.h"

const char * std_fonts [] = {
	"Times,TimesRoman,TimesNewRoman",
	"Times,Palatino",
	"Helvetica,Arial",
	"Courier,Verdana",
	"Cursive,ZapfChancery",
	"ZapfChancery",
	"Symbol",
};

string *
append( string * str, const char * toadd )
{
	char * buf;
	int length, to_allocate = 0;

	if( ! toadd || ! *toadd )
		return str;

	length = strlen( toadd );
	to_allocate = str->length + length + 1;

	if( to_allocate > str->allocated )
	{
		to_allocate += 32;

		buf = malloc( to_allocate );
		if( str->length )
			strncpy( buf, str->content, str->length );
		
		if( str->allocated )
			free( str->content );

		str->allocated = to_allocate;
		str->content = buf;
	}

	strncpy( str->content + str->length, toadd, length );

	str->length += length;
	str->content[ str->length ] = '\0';

	return str;
}

void
string_init( string * str)
{
	str->content   = "";
	str->length    = 0;
	str->allocated = 0;
}

void
string_destroy( string * str )
{
	if( str->allocated )
		free( str->content );
	string_init( str );
}

void
string_reset( string * str )
{
	if( str->allocated > 0 )
		str->content[0] = '\0';
	str->length = 0;	
}

/***************************************************************************
 *   Copyright (C) 2006 by Tomasz Hławiczka                                *
 *   tomek@tru.pl                                                          *
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

#include "librtf.h"

#include <stdlib.h>	// malloc

#define OUT(P) ((output*) P->object)

static void 
rtf_begin(processor * p)
{
	OUT(p)->append( OUT(p), "{\\rtf1" );
}

static void 
rtf_end(processor * p)
{
	OUT(p)->append( OUT(p), "}" );
}

processor *
create_rtf_processor( output * out  )
{
	processor * proc = NULL;
	
	proc = (processor *) malloc( sizeof( processor ) );
	
	proc->object = (void*) out;

	proc->begin    = & rtf_begin;
	proc->end      = & rtf_end;
	
	return proc;
}

void
destroy_rtf_processor( processor * proc )
{
	// free( proc->object);
	free( proc );
}

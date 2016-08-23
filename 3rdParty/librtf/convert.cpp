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

//#ifdef HAVE_STDIO_H
#include <stdio.h>
//#endif

//#ifdef HAVE_STDLIB_H
#include <stdlib.h>
//#endif

//#ifdef HAVE_CTYPE_H
#include <ctype.h>
//#endif

//#ifdef HAVE_STRING_H
#include <string.h>
//#endif

#include "librtf.h"

#include "parser.h"

/* Paragraph alignment (kludge)
 */
enum {
	ALIGN_LEFT=0,
	ALIGN_RIGHT,
	ALIGN_CENTER,
	ALIGN_JUSTIFY
};


void print_with_special_exprs (internal_processor *, const char *s);
static void process_block (internal_processor * f, rtf_part *);
void starting_body(internal_processor * filter);

/*========================================================================
 * Name:	starting_body
 * Purpose:	Switches output stream for writing document contents.
 * Args:	None.
 * Returns:	None.
 *=======================================================================*/


static int
compare( rtf_part * part, const char * command )
{
	return !strcmp( part->content.content, command );
}

static int 
get_param ( rtf_part * part )
{
	if ( part->has_param == FALSE )
	{
		// ?
	}
	return part->param;
}

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/


/*========================================================================
 * Name:	process_font_table
 * Purpose:	Processes the font table of an RTF file.
 * Args:	Tree of words.
 * Returns:	None.
 *=======================================================================*/

void
process_font_table (internal_processor * proc)
{
	 string fontname = {0,0,0};
	 int charset;
	 int fontblock_level = 0;
	 int fontid = 0;
	 rtf_part * part = NULL;

//	CHECK_PARAM_NOT_NULL(part);
//	CHECK_PARAM_NOT_NULL(filter);

	part = get_next_part( proc->src );
	while (part)
	{
		if( part->type == PART_TYPE_BLOCK_START ) 
		{
			fontblock_level++;
			charset = 0;
			fontid = 0;
			string_destroy( &fontname );
		}
		else if ( part->type == PART_TYPE_BLOCK_END )
		{
			if( fontblock_level == 1 )
			{
				char * tmp;
				if ( fontname.length && (tmp = strchr(fontname.content, ';')))
					*tmp = '\0';

				proc->iface->register_font(proc->iface, fontid, fontname.content );
				
				string_destroy( & fontname );
			}
			else if ( fontblock_level == 0 )
			{
				push_back_current_part( proc->src );
				break;
			}
			fontblock_level --;
		}
		else if ( part->type == PART_TYPE_COMMAND )
		{
			if( fontblock_level == 1 )
			{
				if( compare( part, "f" ))
					fontid = get_param( part );
				else if ( compare( part, "charset" ))
					charset = get_param ( part );
			}
		}
		else if ( part->type == PART_TYPE_TEXT )
		{
			if ( fontblock_level == 1)
				append( &fontname , part->content.content );
		}
		part = get_next_part( proc->src );
	}
}

/*========================================================================
 * Name:	process_index_entry
 * Purpose:	Processes an index entry of an RTF file.
 * Args:	Tree of words.
 * Returns:	None.
 *=======================================================================*/

void
process_index_entry (internal_processor * proc)
{
//	CHECK_PARAM_NOT_NULL(w);
	skip_block_to_end ( proc->src );
}


/*========================================================================
 * Name:	process_toc_entry
 * Purpose:	Processes an index entry of an RTF file.
 * Args:	Tree of words, flag to say whether to include a page#.
 * Returns:	None.
 *=======================================================================*/

void
process_toc_entry (internal_processor * proc, int include_page_num)
{

//	CHECK_PARAM_NOT_NULL(w);

	skip_block_to_end ( proc->src );
}


/*========================================================================
 * Name:	process_info_group
 * Purpose:	Processes the \info group of an RTF file.
 * Args:	Tree of words.
 * Returns:	None.
 *=======================================================================*/

string *
read_to_end( string * result, internal_processor * proc )
{
	rtf_part * part = NULL;
	int blocks = 0;

	string_destroy( result );

	part = get_next_part( proc->src );

	while ( part )
	{
		if( part->type == PART_TYPE_TEXT )
			append( result, part->content.content );
		else if ( part->type == PART_TYPE_BLOCK_START )
			blocks ++;
		else if ( part->type == PART_TYPE_BLOCK_END )
		{
			if( blocks == 0 )
			{
				push_back_current_part(proc->src);
				break;
			}
			blocks --;
		}
		part = get_next_part( proc->src );
	}

	if( part == NULL )
	{
		// unexpected end of source.
	}

	return result;
}

void
process_info_group (internal_processor * proc)
{
	int block_level = 0;
	string tmp_str = {0,0,0};

	rtf_part * part = get_next_part( proc->src );
	while ( part )
	{
		if( part->type == PART_TYPE_BLOCK_START )
			block_level ++;
		else if ( part->type == PART_TYPE_BLOCK_END )
		{
			if( block_level == 0 )
			{
				push_back_current_part(proc->src);
				break;
			}
			block_level --;
		}
		else if( part->type == PART_TYPE_COMMAND && block_level == 1 )
		{
			if( compare( part, "title" ) )
				proc->iface->title( proc->iface, read_to_end( &tmp_str, proc )->content );
			else if ( compare( part, "keywords" ) )
				proc->iface->keywords( proc->iface, read_to_end( &tmp_str, proc)->content );
			else if ( compare( part, "author" ) )
				proc->iface->author( proc->iface, read_to_end( &tmp_str, proc)->content );
			else if ( compare( part, "comments" ) )
				{}
			else if ( compare ( part, "nofpages" ) )
				proc->iface->summary( proc->iface, SUMMARY_NUBMER_OF_PAGES, get_param( part ));
			else if ( compare ( part, "nofwords" ) )
				proc->iface->summary( proc->iface, SUMMARY_NUMBER_OF_WORDS, get_param( part ));
			else if ( compare ( part, "nofchars" ) )
				proc->iface->summary( proc->iface, SUMMARY_NUMBER_OF_CHARS, get_param( part ));
			else if ( compare ( part, "creatim" ) )	// creation date
				{}
			else if ( compare ( part, "printim" ) )	// last print date
				{}
			else if ( compare ( part, "buptim" ) ) // last backup date
				{}
			else if ( compare ( part, "revtime" ) )	// revision date
				{}
			else if ( compare ( part, "hlinkbase" ) )
				proc->iface->hyperlink_base( proc->iface, read_to_end( &tmp_str, proc )->content );
		}

		part = get_next_part ( proc->src );
	}
}

static int
cmd_generator ( internal_processor * proc, rtf_part * part )
{
	string tmp;
	string_init ( & tmp );
	
	read_to_end( & tmp, proc );
	
	// proc->iface->info(proc->iface, LIBRTF_INFO_GENERATOR, tmp );
	
	string_destroy ( & tmp );
	return FALSE;
}

/*========================================================================
 * Name:	process_color_table
 * Purpose:	Processes the color table of an RTF file.
 * Args:	Tree of words.
 * Returns:	None.
 *=======================================================================*/

void
process_color_table (internal_processor * proc)
{
	int r,g,b;
	rtf_part * part;

//	CHECK_PARAM_NOT_NULL(w);

	/* Sometimes, RTF color tables begin with a semicolon,
	 * i.e. an empty color entry. This seems to indicate that color 0 
	 * will not be used, so here I set it to black.
	 */
	r=g=b=0;

	part = get_next_part( proc->src );

	while(part)
	{
		if ( part->type == PART_TYPE_BLOCK_START )
			skip_block_to_outside(proc->src);
		else if( part->type == PART_TYPE_BLOCK_END )
		{
			push_back_current_part(proc->src);
			break;
		}
		else if ( part->type == PART_TYPE_COMMAND )
		{
			if ( compare ( part, "red" ) )
				r = get_param( part ) ;
			else if ( compare ( part, "green" ) )
				g = get_param( part );
			else if ( compare ( part, "blue" ) )
				b = get_param( part );
		}
		else if( part->type == PART_TYPE_TEXT )
		{
			if( compare ( part, ";" ) )
			{
				while(r>255) r>>=8;
				while(g>255) g>>=8;
				while(b>255) b>>=8;
				proc->iface->register_color(proc->iface, r,g,b );
				r=g=b=0;
			}
		}

		part = get_next_part ( proc->src );
	}
}

/*========================================================================
 * Name:	cmd_cf
 * Purpose:	Executes the \cf command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_cf (internal_processor * f, rtf_part *w)
{
	
	f->iface->attr_push(f->iface, ATTR_FOREGROUND, get_param( w ) );
	return FALSE;
}

/*========================================================================
 * Name:	cmd_cs
 * Purpose:	Executes the \cs command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_cs (internal_processor * f, rtf_part *w)
{
	if( w->has_param )
		f->iface->attr_push_style( f->iface, Section, get_param ( w ) );
	else
	{
		// \cs without param should be only in \stylesheet block
	}
	return FALSE;
}


/*========================================================================
 * Name:	cmd_cb
 * Purpose:	Executes the \cb command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_cb (internal_processor * f, rtf_part *w)
{
	f->iface->attr_push(f->iface, ATTR_BACKGROUND, get_param( w ));
	return FALSE;
}


/*========================================================================
 * Name:	cmd_fs
 * Purpose:	Executes the \fs command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/
static int 
cmd_fs (internal_processor * f, rtf_part *w )
{

	/* Note, fs20 means 10pt */
	f->iface->attr_push(f->iface, ATTR_FONTSIZE, get_param(w) / 2 );

	return FALSE;
}


/*========================================================================
 * Name:	cmd_field
 * Purpose:	Interprets fields looking for hyperlinks.
 * Comment:	Because hyperlinks are put in \field groups,
 *		we must interpret all \field groups, which is
 *		slow and laborious.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_field (internal_processor * proc, rtf_part *w)
{

// 	CHECK_PARAM_NOT_NULL(w);

	/*	Specification :
		{\field <fieldmod>? <fieldinst> <fieldrslt> }
		{\* \fldinst <para>+ <fldalt>? }
		{\fldrslt <para>+ }
		<fieldmod> : \flddirty \fldedit \fldlock \fldpriv
	*/

	rtf_part * part = get_next_part( proc->src );

	while( part )
	{
		if ( part->type == PART_TYPE_BLOCK_END )
		{
			push_back_current_part( proc->src );
			break;
		}
		else if ( part->type == PART_TYPE_COMMAND )
		{
			if( compare( part, "flddirty" ) )
			{
				// TODO: A formatting change has been made to the field result since the field was last updated.
			}
			else if( compare( part,  "fldedit") )
			{
				// TODO: Text has been added to, or removed from, the field result since the field was last updated.
			}
			else if( compare( part, "fldlock" ) )
			{
				// TODO: Field is locked and cannot be updated.
			}
			else if( compare( part, "fldpriv" ))
			{
				// TODO: Result is not in a form suitable for display (for example, binary data used by fields whose result is a picture).
			}
			else
			{
				// Unknown commmand.
			}
		}
		else if ( part->type == PART_TYPE_BLOCK_START )
		{
			part = get_next_command( proc->src );

			if( part )
			{
				if ( compare( part, "fldrslt" ) )
				{
					process_block( proc, part );
				}
				else if ( compare( part, "*" ) )
				{
					part = get_next_command ( proc->src );
					if( part )
					{
						if( compare( part, "fldinst") )
						{
							part = get_next_part( proc->src );
							while ( part )
							{
								if( part->type == PART_TYPE_BLOCK_START )
									skip_block_to_outside(proc->src);
								else if ( part->type == PART_TYPE_BLOCK_END )
									break;
								else if ( part->type == PART_TYPE_TEXT )
								{
									if( compare( part, "SYMBOL" ))
									{
										// TODO:
									}
									else if( compare ( part, "SHAPE" ))
									{
										// TODO:
									}
									else if( compare ( part, "HYPERLINK" ))
									{
										// FIXME:
										proc->iface->hyperlink(proc->iface, "FIXME: URL" );
									}
								}
								part = get_next_part( proc->src );
							}
						}
						else
							skip_block_to_outside(proc->src);
					}
				}
				else
					skip_block_to_outside(proc->src);
			}
		}
		part = get_next_part ( proc->src );
	}
	return TRUE;
}



/*========================================================================
 * Name:	cmd_f
 * Purpose:	Executes the \f command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/
static int 
cmd_f (internal_processor * f, rtf_part *w)
{

	f->iface->attr_push(f->iface, ATTR_FONTFACE, get_param( w ) );
	return FALSE;
}


/*========================================================================
 * Name:	cmd_highlight
 * Purpose:	Executes the \cf command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_highlight (internal_processor * f, rtf_part *w) 
{
	f->iface->attr_push(f->iface, ATTR_BACKGROUND, get_param ( w ) );
	return FALSE;
}



/*========================================================================
 * Name:	cmd_tab
 * Purpose:	Executes the \tab command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_tab (internal_processor * f, rtf_part *w) 
{
	/* Tab presents a genuine problem
	 * since some output formats don't have 
	 * an equivalent. As a kludge fix, I shall 
	 * assume the font is fixed width and that
	 * the tabstops are 8 characters apart.
	 */
	f->iface->print_char(f->iface, CHAR_TAB );
	/* FIXME
	int need= 8-(f->total_chars_this_line%8);
	f->total_chars_this_line += need;
	while(need>0) {
		filter->print_forced_space(filter);
		need--;
	}
	printf("\n");
	*/
	return FALSE;
}


/*========================================================================
 * Name:	cmd_plain
 * Purpose:	Executes the \plain command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_plain (internal_processor * f, rtf_part *w) {
	f->iface->attr_pop_all(f->iface);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_fnil
 * Purpose:	Executes the \fnil command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/
static int 
cmd_fnil (internal_processor * f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_FONTFACE, FONTNIL_ID );
	return FALSE;
}



/*========================================================================
 * Name:	cmd_froman
 * Purpose:	Executes the \froman command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/
static int 
cmd_froman (internal_processor * f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_FONTFACE,FONTROMAN_ID );
	return FALSE;
}


/*========================================================================
 * Name:	cmd_fswiss
 * Purpose:	Executes the \fswiss command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_fswiss (internal_processor * f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_FONTFACE,FONTSWISS_ID );
	return FALSE;
}


/*========================================================================
 * Name:	cmd_fmodern
 * Purpose:	Executes the \fmodern command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_fmodern (internal_processor * f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_FONTFACE,FONTMODERN_ID );
	return FALSE;
}


/*========================================================================
 * Name:	cmd_fscript
 * Purpose:	Executes the \fscript command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_fscript (internal_processor * f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_FONTFACE,FONTSCRIPT_ID );
	return FALSE;
}

/*========================================================================
 * Name:	cmd_fdecor
 * Purpose:	Executes the \fdecor command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_fdecor (internal_processor * f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_FONTFACE,FONTDECOR_ID );
	return FALSE;
}

/*========================================================================
 * Name:	cmd_ftech
 * Purpose:	Executes the \ftech command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_ftech (internal_processor * f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_FONTFACE,FONTTECH_ID );
	return FALSE;
}

/*========================================================================
 * Name:	cmd_expand
 * Purpose:	Executes the \expand command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_expand (internal_processor * f, rtf_part *w) {
	if (w->has_param) {
		int param = get_param ( w );
		if (!param)
			f->iface->attr_pop(f->iface, ATTR_EXPAND);
		else 
			f->iface->attr_push(f->iface, ATTR_EXPAND, param );
	}
	return FALSE;
}


/*========================================================================
 * Name:	cmd_emboss
 * Purpose:	Executes the \embo command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_emboss (internal_processor * f, rtf_part *w) {
	if (w->has_param && !get_param(w))
		f->iface->attr_pop(f->iface, ATTR_EMBOSS);
	else
		f->iface->attr_push(f->iface, ATTR_EMBOSS, get_param(w));
	return FALSE;
}


/*========================================================================
 * Name:	cmd_engrave
 * Purpose:	Executes the \impr command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_engrave (internal_processor * f, rtf_part *w) {
	if (w->has_param && !get_param(w)) 
		f->iface->attr_pop(f->iface, ATTR_ENGRAVE);
	else
		f->iface->attr_push(f->iface, ATTR_ENGRAVE, get_param(w));
	return FALSE;
}

/*========================================================================
 * Name:	cmd_caps
 * Purpose:	Executes the \caps command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_caps (internal_processor * f, rtf_part *w) {
	if (w->has_param && !get_param(w))
		f->iface->attr_pop(f->iface, ATTR_CAPS);
	else 
		f->iface->attr_push(f->iface, ATTR_CAPS, 0);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_scaps
 * Purpose:	Executes the \scaps command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/
static int 
cmd_scaps (internal_processor * f, rtf_part *w) {
	if (w->has_param && !get_param(w))
		f->iface->attr_pop(f->iface, ATTR_SMALLCAPS);
	else 
		f->iface->attr_push(f->iface, ATTR_SMALLCAPS, 0);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_bullet
 * Purpose:	Executes the \bullet command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/
static int 
cmd_bullet (internal_processor *f, rtf_part *w) {

	f->iface->print_char(f->iface, CHAR_BULLET );
	
//	if (op->chars.bullet) {
//		printf(op->chars.bullet);
//		++total_chars_this_line; /* \tab */
//	}
	return FALSE;
}

/*========================================================================
 * Name:	cmd_ldblquote
 * Purpose:	Executes the \ldblquote command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/
static int 
cmd_ldblquote (internal_processor * f, rtf_part *w) {

	f->iface->print_char(f->iface, CHAR_DBL_QUOTE);
	
//	if (op->chars.left_dbl_quote) {
//		printf(op->chars.left_dbl_quote);
//		++total_chars_this_line; /* \tab */
//	}
	return FALSE;
}


/*========================================================================
 * Name:	cmd_rdblquote
 * Purpose:	Executes the \rdblquote command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_rdblquote (internal_processor * f, rtf_part *w) {
	
	f->iface->print_char(f->iface, CHAR_RIGHT_DBL_QUOTE);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_lquote
 * Purpose:	Executes the \lquote command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/
static int 
cmd_lquote (internal_processor * f, rtf_part *w) {

	f->iface->print_char(f->iface, CHAR_LEFT_DBL_QUOTE );
	return FALSE;
}


/*========================================================================
 * Name:	cmd_nonbreaking_space
 * Purpose:	Executes the nonbreaking space command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_nonbreaking_space (internal_processor * f, rtf_part *w) {

	f->iface->print_char(f->iface, CHAR_NONBREAKING_SPACE);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_nonbreaking_hyphen
 * Purpose:	Executes the nonbreaking hyphen command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_nonbreaking_hyphen (internal_processor * f, rtf_part *w) {

	f->iface->print_char(f->iface, CHAR_NONBREAKING_HYPHEN);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_optional_hyphen
 * Purpose:	Executes the optional hyphen command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_optional_hyphen (internal_processor * f, rtf_part *w) {

	f->iface->print_char(f->iface, CHAR_OPTIONAL_HYPHEN);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_emdash
 * Purpose:	Executes the \emdash command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/
static int 
cmd_emdash (internal_processor * f, rtf_part *w) {

	f->iface->print_char(f->iface, CHAR_EMDASH);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_endash
 * Purpose:	Executes the \endash command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_endash (internal_processor * f, rtf_part *w) {

	f->iface->print_char(f->iface, CHAR_ENDASH);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_rquote
 * Purpose:	Executes the \rquote command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_rquote (internal_processor * f, rtf_part *w) {

	f->iface->print_char(f->iface, CHAR_RIGHT_QUOTE);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_par
 * Purpose:	Executes the \par command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/
static int 
cmd_par (internal_processor * f, rtf_part *w) {

	f->iface->print_char(f->iface, CHAR_LINE_BREAK );
	return FALSE;
}

static int
cmd_pntxta (internal_processor * proc, rtf_part *w) {
	
	skip_block_to_end( proc->src );
	return FALSE;
}

static int
cmd_pntxtb (internal_processor * proc, rtf_part *w) {

	skip_block_to_end( proc->src );
	return FALSE;
}

/*========================================================================
 * Name:	cmd_line
 * Purpose:	Executes the \line command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_line (internal_processor * f, rtf_part *w) {

	f->iface->print_char(f->iface, CHAR_LINE_BREAK);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_page
 * Purpose:	Executes the \page command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_page (internal_processor *f, rtf_part *w) {

	f->iface->print_char(f->iface, CHAR_PAGE_BREAK);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_intbl
 * Purpose:	Executes the \intbl command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_intbl (internal_processor *f, rtf_part *w) {
//	++f->coming_pars_that_are_tabular;
	return FALSE;
}


/*========================================================================
 * Name:	cmd_ulnone
 * Purpose:	Executes the \ulnone command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_ulnone (internal_processor *f, rtf_part *w) {

	static int underlines [] = { ATTR_UNDERLINE,
		ATTR_DOT_UL,ATTR_DASH_UL,ATTR_DOT_DASH_UL,ATTR_2DOT_DASH_UL,
		ATTR_WORD_UL,ATTR_WAVE_UL,ATTR_THICK_UL,ATTR_DOUBLE_UL };

	f->iface->attr_remove(f->iface, underlines, sizeof( underlines ) / sizeof( underlines[0] ) );

	return FALSE;
}

/*========================================================================
 * Name:	cmd_ul
 * Purpose:	Executes the \ul command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_ul (internal_processor *f, rtf_part *w) {
	if (w->has_param && get_param(w) == 0) {
		cmd_ulnone(f, w);
	} else {
		f->iface->attr_push(f->iface, ATTR_UNDERLINE, 0);
	}
	return FALSE;
}

/*========================================================================
 * Name:	cmd_uld
 * Purpose:	Executes the \uld command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_uld (internal_processor *f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_DOUBLE_UL, 0);
	return FALSE;
}

/*========================================================================
 * Name:	cmd_uldb
 * Purpose:	Executes the \uldb command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_uldb (internal_processor *f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_DOT_UL, 0);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_uldash
 * Purpose:	Executes the \uldash command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_uldash (internal_processor *f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_DASH_UL, 0);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_uldashd
 * Purpose:	Executes the \cmd_uldashd command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_uldashd (internal_processor *f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_DOT_DASH_UL,0);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_uldashdd
 * Purpose:	Executes the \uldashdd command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_uldashdd (internal_processor *f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_2DOT_DASH_UL,0);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_ulw
 * Purpose:	Executes the \ulw command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_ulw (internal_processor *f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_WORD_UL,0);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_ulth
 * Purpose:	Executes the \ulth command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_ulth (internal_processor *f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_THICK_UL,0);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_ulthd
 * Purpose:	Executes the \ulthd command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_ulthd (internal_processor *f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_THICK_UL, 0);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_ulthdash
 * Purpose:	Executes the \ulthdash command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_ulthdash (internal_processor *f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_THICK_UL, 0);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_ulwave
 * Purpose:	Executes the \ulwave command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_ulwave (internal_processor *f, rtf_part *w) {
	f->iface->attr_push(f->iface, ATTR_WAVE_UL,0);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_strike
 * Purpose:	Executes the \strike command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_strike (internal_processor *f, rtf_part *w) {
	if (w->has_param && get_param(w)==0)
		f->iface->attr_pop(f->iface,ATTR_STRIKE);
	else
		f->iface->attr_push(f->iface,ATTR_STRIKE,0);
	return FALSE;
}

/*========================================================================
 * Name:	cmd_strikedl
 * Purpose:	Executes the \strikedl command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_strikedl (internal_processor *f, rtf_part *w) {
	if (w->has_param && get_param(w)==0)
		f->iface->attr_pop(f->iface, ATTR_DBL_STRIKE);
	else
		f->iface->attr_push(f->iface, ATTR_DBL_STRIKE,0);
	return FALSE;
}


/*========================================================================
 * Name:	cmd_striked
 * Purpose:	Executes the \striked command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_striked (internal_processor * f, rtf_part *w) {
	if (w->has_param && get_param(w)==0)
		f->iface->attr_pop(f->iface, ATTR_DBL_STRIKE);
	else
		f->iface->attr_push(f->iface, ATTR_DBL_STRIKE,0);
	return FALSE;
}

static int cmd_pgdsctbl ( internal_processor * proc, rtf_part * w)
{
	skip_block_to_end( proc->src );
	return FALSE;
}

/*========================================================================
 * Name:	cmd_rtf
 * Purpose:	Executes the \rtf command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_rtf (internal_processor *f, rtf_part *w) {
	return FALSE;
}


/*========================================================================
 * Name:	cmd_up
 * Purpose:	Executes the \up command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_up (internal_processor *f, rtf_part *w) {
	if (w->has_param && get_param(w)==0)
		f->iface->attr_pop(f->iface, ATTR_SUPER);
	else
		f->iface->attr_push(f->iface, ATTR_SUPER,0);
	return FALSE;
}

#if 1 /* daved - 0.19.4 */
/*========================================================================
 * Name:	cmd_u
 * Purpose:	Processes a Unicode character
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, always false
 *=======================================================================*/

static int cmd_u (internal_processor *f, rtf_part *w) {

	if( f->iface->print_unicode(f->iface, get_param(w) ) )
		return(SKIP_ONE_WORD);
	return FALSE;
}
#endif

/*========================================================================
 * Name:	cmd_dn
 * Purpose:	Executes the \dn command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_dn (internal_processor *f, rtf_part *w) {
	if (w->has_param && get_param(w)==0)
		f->iface->attr_pop(f->iface, ATTR_SUB);
	else
		f->iface->attr_push(f->iface, ATTR_SUB,0);
	return FALSE;
}

/*========================================================================
 * Name:	cmd_nosupersub
 * Purpose:	Executes the \nosupersub command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_nosupersub (internal_processor *f, rtf_part *w) {
	f->iface->attr_pop(f->iface, ATTR_SUPER);
	f->iface->attr_pop(f->iface, ATTR_SUB);
	return FALSE;
}

/*========================================================================
 * Name:	cmd_super
 * Purpose:	Executes the \super command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_super (internal_processor *f, rtf_part *w) {
	if (w->has_param && get_param(w)==0)
		f->iface->attr_pop(f->iface, ATTR_SUPER);
	else
		f->iface->attr_push(f->iface, ATTR_SUPER,0);
	return FALSE;
}

/*========================================================================
 * Name:	cmd_sub
 * Purpose:	Executes the \sub command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_sub (internal_processor *f, rtf_part *w) {
	if (w->has_param && get_param(w)==0)
		f->iface->attr_pop(f->iface, ATTR_SUB);
	else
		f->iface->attr_push(f->iface, ATTR_SUB,0);
	return FALSE;
}

/*========================================================================
 * Name:	cmd_shad
 * Purpose:	Executes the \shad command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_shad (internal_processor *f, rtf_part *w) {
	if (w->has_param && get_param(w)==0)
		f->iface->attr_pop(f->iface, ATTR_SHADOW);
	else
		f->iface->attr_push(f->iface, ATTR_SHADOW,0);
	return FALSE;
}

/*========================================================================
 * Name:	cmd_b
 * Purpose:	Executes the \b command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int 
cmd_b (internal_processor *f, rtf_part *w) {
	if (w->has_param && get_param(w)==0) {
		f->iface->attr_pop(f->iface, ATTR_BOLD);
	}
	else
		f->iface->attr_push(f->iface, ATTR_BOLD,0);
	return FALSE;
}

/*========================================================================
 * Name:	cmd_i
 * Purpose:	Executes the \i command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_i (internal_processor *f, rtf_part *w) {
	if (w->has_param && get_param(w)==0)
		f->iface->attr_pop(f->iface, ATTR_ITALIC);
	else
		f->iface->attr_push(f->iface, ATTR_ITALIC,0);
	return FALSE;
}

/*========================================================================
 * Name:	cmd_s
 * Purpose:	Executes the \s command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/
static int cmd_s (internal_processor *f, rtf_part *w) {
	if(w->has_param)
		f->iface->attr_push_style( f->iface, Paragraph, get_param(w) );
	else
	{
		// \s without param should be only in \stylesheet block
	}

	return FALSE;
}

/*========================================================================
 * Name:	cmd_sect
 * Purpose:	Executes the \sect command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_sect (internal_processor *f, rtf_part *w) {
	/* XX kludge */
	f->iface->print_char(f->iface, CHAR_PARAGRAPH_BEGIN );
//	if (op->paragraph_begin) {
//		printf(op->paragraph_begin);
//	}
	return FALSE;
}

/*========================================================================
 * Name:	cmd_shp
 * Purpose:	Executes the \shp command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_shp (internal_processor *f, rtf_part *w) {

	// FIXME:
//	if (op->comment_begin) {
//		printf(op->comment_begin);
//		printf("Drawn Shape (ignored--not implemented yet)");
//		printf(op->comment_end);	/* daved 0.20.2 */
//	}

	return FALSE;
}

/*========================================================================
 * Name:	cmd_outl
 * Purpose:	Executes the \outl command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_outl (internal_processor *f, rtf_part *w) {
	if (w->has_param && get_param(w)==0)
		f->iface->attr_pop(f->iface, ATTR_OUTLINE);
	else
		f->iface->attr_push(f->iface, ATTR_OUTLINE,0);
	return FALSE;
}

/*========================================================================
 * Name:	cmd_ansi
 * Purpose:	Executes the \ansi command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_ansi (internal_processor *f, rtf_part *w) {
	f->iface->charset(f->iface, CHARSET_ANSI );
	return FALSE;
}

/*========================================================================
 * Name:	cmd_ansicpg
 * Purpose:	Executes the \ansicpg command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_ansicpg (internal_processor *f, rtf_part *w) {
	
	f->iface->codepage( f->iface, get_param(w) );
	/*
	int i;
	for (i = 0; i < sizeof(codepages) / sizeof(CodepageInfo); i ++) {
		f->charset_codepage = &codepages[i];
		if (f->charset_codepage->cp == param) {
			// FIXME:
			//if (op->comment_begin) {
			//	printf(op->comment_begin);
			//	printf("document uses ANSI codepage %d character set", param);
			//	printf(op->comment_end);
			//}
			break;
		}
	}
	*/
//	FIXME:
//	if ((charset_codepage == NULL || charset_codepage->cp == 0) && op->comment_begin) {
//		printf(op->comment_begin);
//		printf("document uses default ANSI codepage character set");
//		printf(op->comment_end);
//	}
	return FALSE;
}

/*========================================================================
 * Name:	cmd_pc
 * Purpose:	Executes the \pc command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_pc (internal_processor *f, rtf_part *w) {
	f->iface->charset(f->iface, CHARSET_CP437 );
	return FALSE;
}

/*========================================================================
 * Name:	cmd_pca
 * Purpose:	Executes the \pca command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_pca (internal_processor *f, rtf_part *w) {
	f->iface->charset(f->iface, CHARSET_CP850);
	return FALSE;
}

/*========================================================================
 * Name:	cmd_mac
 * Purpose:	Executes the \mac command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_mac (internal_processor *f, rtf_part *w) {
	f->iface->charset(f->iface, CHARSET_MAC);
	return FALSE;
}

/*========================================================================
 * Name:	cmd_colortbl
 * Purpose:	Executes the \colortbl command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_colortbl (internal_processor *f, rtf_part *w) {
	
	process_color_table(f);

	return TRUE;
}

/*========================================================================
 * Name:	cmd_fonttbl
 * Purpose:	Executes the \fonttbl command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_fonttbl (internal_processor * f, rtf_part *w) {
	
	process_font_table(f);

	return TRUE;
}

/*========================================================================
 * Name:	cmd_header
 * Purpose:	Executes the \header command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_header (internal_processor *f, rtf_part *w) {
	return TRUE;
}

/*========================================================================
 * Name:	cmd_headerl
 * Purpose:	Executes the \headerl command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_headerl (internal_processor *f, rtf_part *w) {
	return TRUE;
}

/*========================================================================
 * Name:	cmd_headerr
 * Purpose:	Executes the \headerr command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_headerr (internal_processor *f, rtf_part *w) {
	return TRUE;
}

/*========================================================================
 * Name:	cmd_headerf
 * Purpose:	Executes the \headerf command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_headerf (internal_processor *f, rtf_part *w) {
	return TRUE;
}

/*========================================================================
 * Name:	cmd_footer
 * Purpose:	Executes the \footer command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_footer (internal_processor *f, rtf_part *w) {
	return TRUE;
}

/*========================================================================
 * Name:	cmd_footerl
 * Purpose:	Executes the \footerl command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_footerl (internal_processor *f, rtf_part *w) {
	return TRUE;
}

/*========================================================================
 * Name:	cmd_footerr
 * Purpose:	Executes the \footerr command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_footerr (internal_processor *f, rtf_part *w) {
	return TRUE;
}

/*========================================================================
 * Name:	cmd_footerf
 * Purpose:	Executes the \footerf command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_footerf (internal_processor *f, rtf_part *w) {
	return TRUE;
}

/*========================================================================
 * Name:	cmd_ignore
 * Purpose:	Dummy function to get rid of subgroups 
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_ignore (internal_processor *f, rtf_part *w) {
	return TRUE;
}

/*========================================================================
 * Name:	cmd_info
 * Purpose:	Executes the \info command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_info (internal_processor *f, rtf_part *w) {
	process_info_group (f);
	return TRUE;
}

/*========================================================================
 * Name:	cmd_pict
 * Purpose:	Executes the \pict command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_pict (internal_processor *f, rtf_part *w) {
	f->within_picture=TRUE;
	f->current_image.width = f->current_image.height = 0;
	f->current_image.type = PICT_WB;
	return FALSE;		
}

/*========================================================================
 * Name:	cmd_bin
 * Purpose:	Executes the \bin command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_bin (internal_processor *f, rtf_part *w) {
	return FALSE;		
}


/*========================================================================
 * Name:	cmd_macpict
 * Purpose:	Executes the \macpict command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_macpict (internal_processor *f, rtf_part *w) {
	f->current_image.type = PICT_MAC;
	return FALSE;		
}

/*========================================================================
 * Name:	cmd_jpegblip
 * Purpose:	Executes the \jpegblip command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_jpegblip (internal_processor *f, rtf_part *w) {
	f->current_image.type = PICT_JPEG;
	return FALSE;		
}

/*========================================================================
 * Name:	cmd_pngblip
 * Purpose:	Executes the \pngblip command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_pngblip (internal_processor *f, rtf_part *w) {
	f->current_image.type = PICT_PNG;
	return FALSE;		
}

/*========================================================================
 * Name:	cmd_pnmetafile
 * Purpose:	Executes the \pnmetafile command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_pnmetafile (internal_processor *f, rtf_part *w) {
	f->current_image.type = PICT_PM;
	return FALSE;		
}

/*========================================================================
 * Name:	cmd_wmetafile
 * Purpose:	Executes the \wmetafile command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_wmetafile (internal_processor *f, rtf_part *w) {
	f->current_image.type = PICT_WM;
	if (f->within_picture) {
		f->current_image.wmetafile_type=get_param(w);
		switch(get_param(w))
		{
		case 1: f->current_image.wmetafile_type_str="MM_TEXT"; break;
		case 2: f->current_image.wmetafile_type_str="MM_LOMETRIC"; break;
		case 3: f->current_image.wmetafile_type_str="MM_HIMETRIC"; break;
		case 4: f->current_image.wmetafile_type_str="MM_LOENGLISH"; break;
		case 5: f->current_image.wmetafile_type_str="MM_HIENGLISH"; break;
		case 6: f->current_image.wmetafile_type_str="MM_TWIPS"; break;
		case 7: f->current_image.wmetafile_type_str="MM_ISOTROPIC"; break;
		case 8: f->current_image.wmetafile_type_str="MM_ANISOTROPIC"; break;
		default: f->current_image.wmetafile_type_str="default:MM_TEXT"; break;
		}
	}
	return FALSE;
}

/*========================================================================
 * Name:	cmd_wbmbitspixel
 * Purpose:	Executes the \wbmbitspixel command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_wbmbitspixel (internal_processor *f, rtf_part *w) {
	if (f->within_picture) 
		f->current_image.bits_per_pixel = get_param(w);
	return FALSE;		
}

/*========================================================================
 * Name:	cmd_picw
 * Purpose:	Executes the \picw command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_picw (internal_processor *f, rtf_part *w) {
	if (f->within_picture) 
		f->current_image.width = get_param(w);
	return FALSE;		
}

/*========================================================================
 * Name:	cmd_pich
 * Purpose:	Executes the \pich command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_pich (internal_processor *f, rtf_part *w) {
	if (f->within_picture) 
		f->current_image.height = get_param(w);
	return FALSE;		
}


/*========================================================================
 * Name:	cmd_xe
 * Purpose:	Executes the \xe (index entry) command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_xe (internal_processor * f, rtf_part *w) {
	process_index_entry (f);
	return TRUE;		
}

/*========================================================================
 * Name:	cmd_tc
 * Purpose:	Executes the \tc (TOC entry) command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int cmd_tc (internal_processor *f, rtf_part *w) {
	process_toc_entry (f, TRUE);
	return TRUE;		
}

/*========================================================================
 * Name:	cmd_tcn
 * Purpose:	Executes the \tcn (TOC entry, no page #) command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int
cmd_tcn (internal_processor *f, rtf_part *w) {
	process_toc_entry (f, FALSE);
	return TRUE;
}

/*========================================================================
 * Name:	cmd_stylesheet
 * Purpose:	Executes the \stylesheet command.
 * Args:	rtf_part, paragraph align info, and numeric param if any.
 * Returns:	Flag, true only if rest of rtf_parts on line should be ignored.
 *=======================================================================*/

static int
cmd_stylesheet (internal_processor * proc, rtf_part * part) {

	/* Specification :
		<stylesheet> 	'{' \stylesheet <style>+ '}'
		<style>			'{' <styledef>?<keycode>? <formatting> <additive>? <based>? <next>? <autoupd>? <link>? <locked>? <hidden>? <semihidden>? <personal>? <compose>? <reply>? <styleid>? <stylename>? ';' '}'
		<styledef>		\s  |\*\cs  | \ds | \ts\tsrowd
		<keycode>		'{' \keycode <keys> '}'
		<keys>			( \shift? & \ctrl? & \alt?) <key>
		<key>			\fn | #PCDATA
		<additive>		\additive
		<based>			\sbasedon
		<next>			\snext
		<autoupd>		\sautoupd
		<hidden>		\shidden
		<link>			\slinkN
		<locked>		\slocked
		<personal>		\spersonal
		<compose>		\scompose
		<reply>			\sreply
		<formatting>	(<brdrdef> | <parfmt> | <apoctl> | <tabdef> | <shading> | <chrfmt>)+
		<styleid>		\styrsidN
		<semihidden>	\ssemihidden
		<stylename>	#PCDATA
	*/

	int block_level = 0;
	
	part = get_next_part ( proc->src );

	TextStyle current_style;
	string stylename;
	
	while( part )
	{
		if( part->type == PART_TYPE_BLOCK_START )
		{
			block_level++;
			if( block_level == 1 )
			{
				current_style.bold = -1;
				current_style.fontface = -1;
				current_style.fontsize = -1;
				current_style.italic = -1;
				current_style.id = 0;
				current_style.rsid = -1;
				string_init( & stylename );
				current_style.type = Paragraph;
			}
		}
		else if( part->type == PART_TYPE_BLOCK_END )
		{
			if( block_level == 0 )
			{
				push_back_current_part( proc->src );
				break;
			}
			else
			{
				current_style.stylename = stylename.content;
				proc->iface->register_style(proc->iface, & current_style );
				string_destroy( & stylename );
			}
			block_level--;
		}
		else if( part->type == PART_TYPE_COMMAND )
		{
			if( block_level == 2 ) // <keycode>
			{
				// if( !strcmp("\\keycode", style ) )
				{
					/* This group is specified within the description of a style
					in the style sheet in the RTF header. The syntax for this group
					is '{\*’\keycode <keys>'}' where <keys> are the characters used
					in the key code. For example, a style, Normal, may be defined
					{\s0 {\*\keycode \shift\ctrl n}Normal;} within the RTF style sheet.
					See the Special Character control words for the characters outside
					the alphanumeric range that may be used.

					\alt	The ALT modifier key. Used to describe shortcut key codes for styles.
					\shift	The SHIFT modifier key. Used to describe shortcut key codes for styles.
					\ctrl	The CTRL modifier key. Used to describe shortcut key codes for styles.
					\fnN	Specifies a function key where N is the function key number. Used to describe shortcut-key codes for styles.

									*/
									// TODO: 
				}
				skip_block_to_end( proc->src );
			}
			if( block_level == 1 ) // <style>
			{
				// <styledef>
				if( compare(part, "s") && part->has_param )
				{
					// Designates paragraph style.
					current_style.id = part->param;
					current_style.type = Paragraph;
				}
				else if( compare(part, "cs") && part->has_param /* was_star */ )
				{
					/*	Designates character style. Like \s, \cs is not a destination control word.
						However, it is important to treat it like one inside the style sheet;
						that is, \cs must be prefixed with \* and must appear as the first item
						inside a group. Doing so ensures that readers that do not understand
						character styles will skip the character style information correctly.
						When used in body text to indicate that a character style was applied,
						do not include the \* prefix.
					*/
					current_style.id = part->param;
					current_style.type = Character;
				}
				else if( compare(part, "ds") && part->has_param )
				{
					// Designates section style.
					// TODO:
					current_style.id = part->param;
					current_style.type = Section;
				}
				else if( compare(part, "ts") && part->has_param )
				{
					// Designates table style, in the same style as \cs for placement and prefixes.
					// TODO:
					current_style.id = part->param;
					current_style.type = Table;
				}
				else if( compare(part, "tsrowd") )
				{
					// Like \trowd but for table style definitions.
					// TODO:
					current_style.type = TableRow;
				}
				// </styledef>
				else if(compare(part, "additive")) 
				{
					// Used in a character style definition ('{\*'\cs'}'). Indicates that character style attributes are to be added to the current paragraph style attributes, rather than setting the paragraph attributes to only those defined in the character style definition.
				}
				else if(compare(part, "sbasedon")) 
				{
					// Defines the number of the style the current style is based on (the default is 222—no style).
				}
				else if(compare(part, "snext")) 
				{
					// Defines the style to be used in the next paragraph after the paragraph marked by this style. If it is omitted, the next style is the current style.
				}
				else if(compare(part, "sautoupd")) 
				{
					// Automatically update styles.
				}
				else if(compare(part, "shidden")) 
				{
					// Style does not appear in the Styles drop-down list in the Style dialog box  (on the Format menu, click Styles).
				}
				else if(compare(part, "slink") && part->has_param )
				{
					// The style is linked to the style whose stylesheet index is denoted by N.  A paragraph style is linked to a character style when they share the same font properties and the character style is updated when the paragraph style changes.  Normally rtf_part will suppress the display of the linked character style in most style lists.
				}
				else if(compare(part, "slocked")) 
				{
					// The style is locked. It cannot be used in the current document if protection is on.
				}
				else if(compare(part, "spersonal")) 
				{
					// Style is a personal e-mail style.
				}
				else if(compare(part, "scompose")) 
				{
					// Style is the e-mail compose style.
				}
				else if(compare(part, "sreply")) 
				{
					// Style is the e-mail reply style.
				}
				else if(compare(part, "styrsid") && part->has_param )
				{
					// Tied to the rsid table, N is the rsid of the author who implemented the style.
					current_style.rsid = part->param;
				}
				else if(compare(part, "ssemihidden")) 
				{
					// Style does not appear in drop-down menus.
				}
				else if(compare(part, "b")) 
				{
					// bold
					current_style.bold = 1;
				}
				else if(compare(part, "i")) 
				{
					// italic
					current_style.italic = 1;
				}
				else if(compare(part, "fs") && part->has_param ) 
				{
					// font size
					current_style.fontsize = part->param;
				}
				else if(compare(part, "f") && part->has_param ) 
				{
					// font
					current_style.fontface = part->param;
				}
				else 
				{
					// TODO: Error: Unknown command
				}
			}
		}
		else if( part->type == PART_TYPE_TEXT )
		{
			if( block_level == 1 )	
				append( & stylename, part->content.content );
		}
		part = get_next_part( proc->src );
	}
	return FALSE;
}



typedef struct {
	const char *name;
	char needed_param;	// 2 = no, 1 = yes, 0 = don't check
	int (*func)(internal_processor *, rtf_part* );
	char *debug_print;
} HashItem;


/* All of the possible commands that RTF might recognize. */
static HashItem hashArray_other [] = {
	{ "*",	0, cmd_ignore, NULL },
	{ "-",	0, cmd_optional_hyphen, "optional hyphen" },
	{ "_",	0, cmd_nonbreaking_hyphen, "nonbreaking hyphen" },
	{ "~",	0, cmd_nonbreaking_space, NULL },
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_a [] = {
	{ "ansi"	,0, &cmd_ansi , NULL },
	{ "ansicpg"	,0, &cmd_ansicpg , NULL },
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_b [] = {
	{ "b"		,0, &cmd_b, NULL },
	{ "bullet"	,0, &cmd_bullet, NULL },
	{ "bin"		,0, &cmd_bin, "picture is binary" },
#if 0
	{ "bgbdiag"	,0, NULL, NULL },
	{ "bgcross"	,0, NULL, NULL },
	{ "bgdcross"	,0, NULL, NULL },
	{ "bgfdiag"	,0, NULL, NULL },
	{ "bghoriz"	,0, NULL, NULL },
	{ "bgkbdiag"	,0, NULL, NULL },
	{ "bgkcross"	,0, NULL, NULL },
	{ "bgkdcross"	,0, NULL, NULL },
	{ "bgkfdiag"	,0, NULL, NULL },
	{ "bgkhoriz"	,0, NULL, NULL },
	{ "bgkvert"	,0, NULL, NULL },
	{ "bgvert"	,0, NULL, NULL },
	{ "brdrcf"	,0, NULL, NULL },
	{ "brdrdb"	,0, NULL, NULL },
	{ "brdrdot"	,0, NULL, NULL },
	{ "brdrhair"	,0, NULL, NULL },
	{ "brdrs"	,0, NULL, NULL },
	{ "brdrsh"	,0, NULL, NULL },
	{ "brdrth"	,0, NULL, NULL },
	{ "brdrw"	,0, NULL, NULL },
#endif
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_c [] = {
	{ "caps"	,0, &cmd_caps, NULL },
	{ "cb"		,1, cmd_cb, NULL },
	{ "cf"		,1, cmd_cf, NULL },
	{ "colortbl"	,0, &cmd_colortbl, "color table" },
	{ "cols"	,0, NULL, "columns (not implemented)" },
	{ "column"	,0, NULL, "column break (not implemented)" },
	{ "cs"		,1, &cmd_cs, "Section style" },
	{ "cbpat"	,0, NULL, "Paragraph Shading" },
	{ "cellx"	,0, NULL, "Table Definitions" },
	{ "cfpat"	,0, NULL, NULL },
	{ "cgrid"	,0, NULL, NULL },
	{ "charrsid"	,0, NULL, "Revision Mark (ignore)" },
	{ "clbgbcross"	,0, NULL, NULL },
	{ "clbgbdiag"	,0, NULL, NULL },
	{ "clbgbkbdiag"	,0, NULL, NULL },
	{ "clbgbkcross"	,0, NULL, NULL },
	{ "clbgbkdcross",0, NULL, NULL },
	{ "clbgbkfdiag"	,0, NULL, NULL },
	{ "clbgbkhor"	,0, NULL, NULL },
	{ "clbgbkvert"	,0, NULL, NULL },
	{ "clbgdcross"	,0, NULL, NULL },
	{ "clbgfdiag"	,0, NULL, NULL },
	{ "clbghoriz"	,0, NULL, NULL },
	{ "clbgvert"	,0, NULL, NULL },
	{ "clbrdrb"	,0, NULL, NULL },
	{ "clbrdrl"	,0, NULL, NULL },
	{ "clbrdrr"	,0, NULL, NULL },
	{ "clbrdrt"	,0, NULL, NULL },
	{ "clcbpat"	,0, NULL, NULL },
	{ "clcfpat"	,0, NULL, NULL },
	{ "clmgf"	,0, NULL, NULL },
	{ "clmrg"	,0, NULL, NULL },
	{ "clshdng"	,0, NULL, NULL },
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_d [] = {
	{ "deff"	,0, NULL, "Default Font" },
	{ "dn"		,0, &cmd_dn, NULL },
#if 0
	{ "dibitmap"	,0, NULL, NULL },
#endif
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_e [] = {
	{ "emdash"	,0, cmd_emdash, NULL },
	{ "endash"	,0, cmd_endash, NULL },
	{ "embo"	,0, &cmd_emboss, NULL },
	{ "expand"	,0, &cmd_expand, NULL },
	{ "expnd"	,0, &cmd_expand, NULL },
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_f [] = {
	{ "f"		,1, cmd_f, NULL },
	{ "fdecor"	,0, cmd_fdecor, NULL },
	{ "fmodern"	,0, cmd_fmodern, NULL },
	{ "fnil"	,0, cmd_fnil, NULL },
	{ "fonttbl"	,0, cmd_fonttbl, "font table" },
	{ "froman"	,0, cmd_froman, NULL },
	{ "fs"		,1, cmd_fs, NULL },
	{ "fscript"	,0, cmd_fscript, NULL },
	{ "fswiss"	,0, cmd_fswiss, NULL },
	{ "ftech"	,0, cmd_ftech, NULL },
	{ "field"	,0, cmd_field, NULL },
	{ "footer"	,0, cmd_footer, NULL },
	{ "footerf"	,0, cmd_footerf, NULL },
	{ "footerl"	,0, cmd_footerl, NULL },
	{ "footerr"	,0, cmd_footerr, NULL },
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_g [] = {
	{ "generator"	,0, cmd_generator, NULL },
};
static HashItem hashArray_h [] = {
	{ "highlight"	,0, &cmd_highlight, NULL },
	{ "header"	,0, cmd_header, NULL },
	{ "headerf"	,0, cmd_headerf, NULL },
	{ "headerl"	,0, cmd_headerl, NULL },
	{ "headerr"	,0, cmd_headerr, NULL },
	{ "hl"		,0, cmd_ignore, "hyperlink within object" },
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_i [] = {
	{ "i"		,0, &cmd_i, NULL },
	{ "info"	,0, &cmd_info, NULL },
	{ "insrsid"	,0, NULL, "Revision Mark (ignore)" },
	{ "intbl"	,0, &cmd_intbl, NULL },
	{ "impr"	,0, &cmd_engrave, NULL },
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_j [] = {
	{ "jpegblip"	,0, &cmd_jpegblip, NULL },
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_l [] = {
	{ "ldblquote"	,0, &cmd_ldblquote, NULL },
	{ "line"	,0, &cmd_line, NULL },
	{ "lquote"	,0, &cmd_lquote, NULL },
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_m [] = {
	{ "mac"		,0, &cmd_mac , NULL },
	{ "macpict"	,0, &cmd_macpict, NULL },
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_n [] = {
	{ "nosupersub"	,0, &cmd_nosupersub, NULL },
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_o [] = {
	{ "outl"	,0, &cmd_outl, NULL },
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_p [] = {
	{ "page"	,0, &cmd_page, NULL },
	{ "par"		,0, &cmd_par, NULL },
	{ "pc"		,0, &cmd_pc , NULL },
	{ "pca"		,0, &cmd_pca , NULL },
	{ "pich"	,1, &cmd_pich, NULL },
	{ "pict"	,2, &cmd_pict, "picture" },
	{ "picw"	,1, &cmd_picw, NULL },
	{ "pgdsctbl"    ,0, &cmd_pgdsctbl,     "hgw"},
	{ "plain"	,0, &cmd_plain, NULL },
	{ "pngblip"	,0, &cmd_pngblip, NULL },
	{ "pnmetafile"	,0, &cmd_pnmetafile, NULL },
	{ "pntxta"      ,0, &cmd_pntxta,     "hgw"},
	{ "pntxtb"      ,0, &cmd_pntxtb,     "hgw"},
#if 0
	{ "piccropb"	,0, NULL, NULL },
	{ "piccropl"	,0, NULL, NULL },
	{ "piccropr"	,0, NULL, NULL },
	{ "piccropt"	,0, NULL, NULL },
	{ "pichgoal"	,0, NULL, NULL },
	{ "pichgoal"	,0, NULL, NULL },
	{ "picscaled"	,0, NULL, NULL },
	{ "picscalex"	,0, NULL, NULL },
	{ "picwgoal"	,0, NULL, NULL },
#endif
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_r [] = {
	{ "rdblquote"	,0, &cmd_rdblquote, NULL },
	{ "rquote"	,0, &cmd_rquote, NULL },
	{ "rtf"		,0, &cmd_rtf, NULL },
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_s [] = {
	{ "s"		,1, cmd_s, "style" },
	{ "sect"	,0, &cmd_sect, "section break"},
	{ "scaps"	,0, &cmd_scaps, NULL },
	{ "super"	,0, &cmd_super, NULL },
	{ "sub"		,0, &cmd_sub, NULL },
	{ "shad"	,0, &cmd_shad, NULL },
	{ "strike"	,0, &cmd_strike, NULL },
	{ "striked"	,0, &cmd_striked, NULL },
	{ "strikedl"	,0, &cmd_strikedl, NULL },
	{ "stylesheet"	,0, &cmd_stylesheet, "style sheet" },
	{ "shp"		,0, cmd_shp, "drawn shape" },
#if 0
	{ "shading", NULL, NULL },
#endif
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_t [] = {
	{ "tab"		,0, &cmd_tab, NULL },
	{ "tc"		,0, cmd_tc, "TOC entry" },
	{ "tcn"		,0, cmd_tcn, "TOC entry" },
#if 0
	{ "tcf"		,0, NULL , NULL },
	{ "tcl"		,0, NULL , NULL },
	{ "trgaph"	,0, NULL , NULL },
	{ "trleft"	,0, NULL , NULL },
	{ "trowd"	,0, NULL , NULL },
	{ "trqc"	,0, NULL , NULL },
	{ "trql"	,0, NULL , NULL },
	{ "trqr"	,0, NULL , NULL },
	{ "trrh"	,0, NULL , NULL },
#endif
	{ NULL, 0, NULL, NULL}
};
static HashItem hashArray_u [] = {
#if 1 /* daved - 0.19.4 */
	{ "u"		,0, &cmd_u, NULL },
#endif
	{ "ul"		,0, &cmd_ul, NULL },
	{ "up"		,0, &cmd_up, NULL },
	{ "uld"		,0, &cmd_uld, NULL },
	{ "uldash"	,0, &cmd_uldash, NULL },
	{ "uldashd"	,0, &cmd_uldashd, NULL },
	{ "uldashdd"	,0, &cmd_uldashdd, NULL },
	{ "uldb"	,0, &cmd_uldb, NULL },
	{ "ulnone"	,0, &cmd_ulnone, NULL },
	{ "ulth"	,0, &cmd_ulth, NULL },
	{ "ulthd"	,0, &cmd_ulthd, NULL },
	{ "ulthdash"	,0, &cmd_ulthdash, NULL },
	{ "ulw"		,0, &cmd_ulw, NULL },
	{ "ulwave"	,0, &cmd_ulwave, NULL },
	{ NULL, 0, NULL, NULL}
};

static HashItem hashArray_v [] = {
	{ "v"		,0, NULL, "Hidden Text" },
	{ NULL, 0, NULL, NULL }
};

static HashItem hashArray_w [] = {
	{ "wbmbitspixel",1, &cmd_wbmbitspixel, NULL },
	{ "wmetafile"	,0, &cmd_wmetafile, NULL },
#if 0
	{ "wbitmap"	,0, NULL, NULL },
	{ "wbmplanes"	,0, NULL, NULL },
	{ "wbmwidthbytes",0, NULL, NULL },
#endif
	{ NULL, 0, NULL, NULL}
};

static HashItem hashArray_x [] = {
	{ "xe"		,0, cmd_xe, "index entry" },
	{ NULL, 0, NULL, NULL}
};

static HashItem *hash [26] = {
	hashArray_a,
	hashArray_b,
	hashArray_c,
	hashArray_d,
	hashArray_e,
	hashArray_f,
	hashArray_g,
	hashArray_h,
	hashArray_i,
	hashArray_j,
	NULL,
	hashArray_l,
	hashArray_m,
	hashArray_n,
	hashArray_o,
	hashArray_p,
	NULL,
	hashArray_r,
	hashArray_s,
	hashArray_t,
	hashArray_u,
	hashArray_v,
	hashArray_w,
	hashArray_x, 
	NULL,
	NULL,
};


/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/



/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/


/*========================================================================
 * Name:	
 * Purpose:	
 * Args:	None.
 * Returns:	None.
 *=======================================================================*/

void 
print_with_special_exprs (internal_processor *f, const char *s)
{
	f->iface->print( f->iface, s );
}



/*========================================================================
 * Name:	
 * Purpose:	
 * Args:	None.
 * Returns:	None.
 *=======================================================================*/

static void 
begin_table(internal_processor * f)
{
	f->within_table=TRUE;
	f->have_printed_row_begin = FALSE;
	f->have_printed_cell_begin = FALSE;
	f->have_printed_row_end = FALSE;
	f->have_printed_cell_end = FALSE;
	f->iface->attrstack_push(f->iface);

	f->iface->table_begin( f->iface );
}



/*========================================================================
 * Name:	
 * Purpose:	
 * Args:	None.
 * Returns:	None.
 *=======================================================================*/

void 
end_table (internal_processor * proc )
{
	if (proc->within_table) {
		if (!proc->have_printed_cell_end) {
			proc->iface->attr_pop_dump(proc->iface);
			proc->iface->table_cell_end( proc->iface );
		}
		if (!proc->have_printed_row_end) {
			proc->iface->table_row_end( proc->iface );
		}
		proc->iface->table_end(proc->iface);

		proc->within_table=FALSE;
		proc->have_printed_row_begin = FALSE;
		proc->have_printed_cell_begin = FALSE;
		proc->have_printed_row_end = FALSE;
		proc->have_printed_cell_end = FALSE;
	}
}



/*========================================================================
 * Name:	
 * Purpose:	
 * Args:	None.
 * Returns:	None.
 *=======================================================================*/

void 
starting_text(internal_processor * proc) {
	if (proc->within_table) {
		if (!proc->have_printed_row_begin) {
			proc->iface->table_row_begin(proc->iface);
			proc->have_printed_row_begin=TRUE;
			proc->have_printed_row_end=FALSE;
			proc->have_printed_cell_begin=FALSE;
		}
		if (!proc->have_printed_cell_begin) {
			proc->iface->table_cell_begin(proc->iface);

			// FIXME: there is no another way to set style in every cell ?
			//filter->attrstack_express_all(filter);
			proc->have_printed_cell_begin=TRUE;
			proc->have_printed_cell_end=FALSE;
		}
	}
}




/*========================================================================
 * Name:	
 * Purpose:	
 * Args:	None.
 * Returns:	None.
 *=======================================================================*/

static void
starting_paragraph_align (internal_processor * proc, int align)
{
	switch (align) 
	{
	case ALIGN_CENTER:
		proc->iface->print_char( proc->iface, CHAR_CENTER_BEGIN );
		break;
	case ALIGN_LEFT:
		break;
	case ALIGN_RIGHT:
		proc->iface->print_char( proc->iface, CHAR_RIGHT_BEGIN );
		break;
	case ALIGN_JUSTIFY:
		proc->iface->print_char( proc->iface, CHAR_JUSTIFY_BEGIN );
		break;
	}
}



/*========================================================================
 * Name:	
 * Purpose:	
 * Args:	None.
 * Returns:	None.
 *=======================================================================*/

static void
ending_paragraph_align (internal_processor * proc, int align)
{
	switch (align) {
	case ALIGN_CENTER:
		proc->iface->print_char( proc->iface, CHAR_CENTER_END );
		break;
	case ALIGN_LEFT:
		break;
	case ALIGN_RIGHT:
		proc->iface->print_char( proc->iface, CHAR_RIGHT_END );
		break;
	case ALIGN_JUSTIFY:
		proc->iface->print_char( proc->iface, CHAR_JUSTIFY_END );
		break;
	}
}


/*========================================================================
 * Name:	
 * Purpose:	Recursive routine to produce the output in the target
 *		format given on a tree of words.
 * Args:	rtf_part* (the tree).
 * Returns:	None.
 *=======================================================================*/

static void
process_block (internal_processor * proc, rtf_part * part )
{
	int is_cell_group=FALSE;
	int paragraph_begined=FALSE;
	int paragraph_align=ALIGN_LEFT;

	CHECK_PARAM_NOT_NULL(w);

	if (!proc->coming_pars_that_are_tabular && proc->within_table) {
		end_table(proc);
	}
	else if (proc->coming_pars_that_are_tabular && !proc->within_table) {
		begin_table(proc);
	}

	proc->iface->attrstack_push(proc->iface);

	while ( part ) 
	{
		if ( part->type == PART_TYPE_TEXT ) 
		{
			const char * s = part->content.content;

			starting_text(proc);

			if (!paragraph_begined) {
				starting_paragraph_align (proc, paragraph_align);
				paragraph_begined=TRUE;
			}

			/*----------------------------------------*/
			if (proc->within_picture) {
				if (!proc->current_image_mgr)
				{
					proc->current_image_mgr = proc->iface->image_begin(proc->iface, & proc->current_image );
				}

				if (s[0]!=' ') {
					const char *s2;
					if (proc->current_image.width && proc->current_image.height ) 
						// && proc->current_image.bits_per_pixel)
					{
						unsigned char buf [ 512 ];
						unsigned long index = 0;
						
						s2=s;
						while (*s2) {
							unsigned int tmp,value;
							tmp=tolower(*s2++);
							if (tmp>'9') tmp-=('a'-10); 
							else tmp-='0';
							value=16*tmp;
							tmp=tolower(*s2++);
							if (tmp>'9') tmp-=('a'-10); 
							else tmp-='0';
							value+=tmp;
							
							buf[ index++ ] = value;
							
							if( index == sizeof( buf ) )
							{
								if (proc->current_image_mgr) 
									proc->current_image_mgr->put(proc->current_image_mgr, buf, index);
								index = 0;
							}
							
						}
						if( index )
						{
							if (proc->current_image_mgr) 
								proc->current_image_mgr->put(proc->current_image_mgr, buf, index);
						}
					}
				}
			}
			/*----------------------------------------*/
			else 
				print_with_special_exprs (proc, s);
		}
		else if ( part->type == PART_TYPE_COMMAND )
				/*----Paragraph alignment----------------------------------------------------*/
		{
			const char * s = part->content.content;
			
			if (!strcmp ("ql", s))
				paragraph_align = ALIGN_LEFT;
			else if (!strcmp ("qr", s))
				paragraph_align = ALIGN_RIGHT;
			else if (!strcmp ("qj", s))
				paragraph_align = ALIGN_JUSTIFY;
			else if (!strcmp ("qc", s))
				paragraph_align = ALIGN_CENTER;
			else if (!strcmp ("pard", s)) 
			{
				/* Clear out all font attributes.
				 */
				proc->iface->attr_pop_all(proc->iface);

				ending_paragraph_align(proc, paragraph_align);
				
				// loading default paragraph style.
				proc->iface->attr_push_style(proc->iface, Character, 0);
				proc->iface->attr_push_style(proc->iface, Paragraph, 0);
				
				paragraph_align = ALIGN_LEFT;
				paragraph_begined = FALSE; // ?
			}
			else if (!strcmp (s, "cell"))
			{
			/*----Table keywords---------------------------------------------------------*/
				is_cell_group=TRUE;
				if (!proc->have_printed_cell_begin) {
					/* Need this with empty cells */
					proc->iface->table_cell_begin( proc->iface );
					// Using of this same style is needed.	
					// f->attrstack_express_all(f);
				}
				proc->iface->attr_pop_dump( proc->iface );
				proc->iface->table_cell_end( proc->iface );
				proc->have_printed_cell_begin = FALSE;
				proc->have_printed_cell_end=TRUE;
			}
			else if (!strcmp (s, "row")) {
				if (proc->within_table) {
					proc->iface->table_row_end( proc->iface );
					proc->have_printed_row_begin = FALSE;
					proc->have_printed_row_end=TRUE;
				} else {
					// if (f->debug_mode) {
					// FIXME: 
					//	printf(op->comment_begin);
					//	printf("end of table row");
					//	printf(op->comment_end);
					// }
				}
			}

			/* else if (*s == '\'')
			{
				// ----Special chars---------------------------------------------------------

				// \'XX is a hex char code expression 
				int ch = h2toi (&s[1]);
				char *s2;

				s2 = f->translate_char( f, ch );

				if (!s2 || !*s2) {
				//	FIXME
				//	printf(op->comment_begin);
				//	printf("char 0x%02x",ch);
				//	printf(op->comment_end);
				} else {
					f->print_char( f, CHAR_WORD_BEGIN);
					f->print(f, s2);
					f->print_char( f, CHAR_WORD_END );
				}
			} */
			else 
			{
				/*----Search the RTF command hash-------------------------------------------*/
				int ch;
				int index=0;
				HashItem *hip;
				const char *p;
				int match = FALSE;  /* Pacify gcc,  st001906 - 0.19.6 */

				/* Look for a parameter */
				p = s;

				/* Generate a hash index */
				ch = tolower(*s);
				if (ch >= 'a' && ch <= 'z') 
					hip = hash[ch - 'a'];
				else
					hip = hashArray_other;

				if (hip)
				{
					while (1)
					{
						char * debug;
						match = FALSE;

						match = !strcmp(s, hip[index].name);

						if (!match)
						{
							index++;
							if (!hip[index].name)
								break;
							continue;
						}
						
						if (hip[index].func)
						{
							int terminate_group;
							
							if ( hip[index].needed_param )
							{
								int func_param = hip[index].needed_param;
								if( part->has_param && func_param == 2)
								{
									char buf [128];
									snprintf(buf, sizeof(buf), "Command \'%s\' should be without param.", s); 
									proc->log->warning(proc->log, LIBRTF_WARN_COMMAND_WITHOUT_PARAM, buf );
									break;
								}
								else if( part->has_param == FALSE && func_param == 1)
								{
									char buf [128];
									snprintf(buf, sizeof(buf), "Command \'%s\' should be with param.", s); 
									proc->log->warning(proc->log, LIBRTF_WARN_COMMAND_WITH_PARAM, buf );
									break;
								}
							}
							
							terminate_group = hip[index].func (proc,part);

							/*
							if (terminate_group == SKIP_ONE_WORD)
									w=w->next;
							else if (terminate_group)
								while(w)
									w=w->next;
						*/
						}

						debug=hip[index].debug_print;

						// if (debug && f->debug_mode) {
						// FIXME
						//	printf(op->comment_begin);
						//	printf("%s", debug);
						//	printf(op->comment_end);
						// }
						break;
					}
				}
				if (!match)
				{
					char buf [128];
					snprintf(buf, sizeof(buf)-1, "Command not found in hash table : %s", s);
					proc->log->warning(proc->log, LIBRTF_WARN_COMMAND_NOT_FOUND, buf );
				}
			}
		}
		else if ( part->type == PART_TYPE_BLOCK_START )
		{
			if (!paragraph_begined) {
				starting_paragraph_align (proc, paragraph_align);
				paragraph_begined=TRUE;
			}

			process_block( proc, get_next_part( proc->src ) );
		}
		else if ( part->type == PART_TYPE_BLOCK_END )
		{
			break;
		}
		part = get_next_part( proc->src );
	}

	if (proc->current_image_mgr) {
		proc->iface->image_end(proc->iface, proc->current_image_mgr );
		proc->current_image_mgr = NULL;
		proc->within_picture = FALSE;
	}

	/* Undo font attributes UNLESS we're doing table cells
	 * since they would appear between </td> and </tr>.
	 */
	if (!is_cell_group)
		proc->iface->attr_pop_all(proc->iface);
	else
		proc->iface->attr_drop_all(proc->iface);

	/* Undo paragraph alignment
	 */
	if (paragraph_begined)
		ending_paragraph_align (proc, paragraph_align);

	proc->iface->attrstack_drop(proc->iface);
}


/*========================================================================
 * Name:	
 * Purpose:	
 * Args:	None.
 * Returns:	None.
 *=======================================================================*/

void 
rtf_process( source * src, processor * proc, observer * log ) 
{
	internal_source isrc;
	internal_processor iproc;

	isrc.iface = src;
	isrc.log = log;

	string_init( & isrc.current_part.content );

	isrc.use_current_part = 0;
	isrc.last_returned_ch = 0;
	isrc.ungot_char = -1;
	isrc.ungot_char2 = -1;
	isrc.ungot_char3 = -1;
	isrc.buf = 0;
	isrc.index = 0;
	isrc.buf_size = 0;
	isrc.read_bytes = 0;

	iproc.iface = proc;
	iproc.log = log;
	iproc.charset_codepage = 0;
	iproc.coming_pars_that_are_tabular = 0;
	iproc.within_table = 0;
	iproc.have_printed_row_begin = 0;
	iproc.have_printed_cell_begin = 0;
	iproc.have_printed_row_end = 0;
	iproc.have_printed_cell_end = 0;
	iproc.within_picture = 0;
	iproc.current_image_mgr = 0;
	iproc.simulate_smallcaps = 0;
	iproc.simulate_allcaps = 0;
	iproc.numchar_table = 0;
	iproc.src = &isrc;
	
	iproc.current_image.width = 0;
	iproc.current_image.height = 0;
	iproc.current_image.bits_per_pixel = 0;
	iproc.current_image.type = 0;
	iproc.current_image.wmetafile_type = 0;
	iproc.current_image.wmetafile_type_str = 0;

	proc->begin(proc);

	iproc.within_table=FALSE;
	iproc.simulate_allcaps=FALSE;

	process_block ( & iproc, get_next_part ( & isrc ) );

	end_table(& iproc);

	proc->end(proc);

	string_destroy( & isrc.current_part.content );
}

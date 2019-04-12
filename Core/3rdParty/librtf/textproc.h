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

#ifndef ___TEXT_PROCESSOR_H___
#define ___TEXT_PROCESSOR_H___

#include <stdio.h> // for size_t

enum TextStyleType 
{
	Paragraph, Character, Section, Table, TableRow,
};

struct TextStyle
{
	int id;
	enum TextStyleType type;
	const char * stylename;		// might be NULL
	int rsid;

	int bold;
	int italic;
	int fontsize;
	int fontface;
};

enum attr_code {
	ATTR_NONE=0,
	ATTR_BOLD, ATTR_ITALIC,

	ATTR_UNDERLINE, ATTR_DOUBLE_UL, ATTR_WORD_UL, 

	ATTR_THICK_UL, ATTR_WAVE_UL, 

	ATTR_DOT_UL, ATTR_DASH_UL, ATTR_DOT_DASH_UL, ATTR_2DOT_DASH_UL,

	ATTR_FONTSIZE, ATTR_STD_FONTSIZE,
	ATTR_FONTFACE,
	ATTR_FOREGROUND, ATTR_BACKGROUND,
	ATTR_CAPS,
	ATTR_SMALLCAPS,

	ATTR_SHADOW,
	ATTR_OUTLINE, 
	ATTR_EMBOSS, 
	ATTR_ENGRAVE, 

	ATTR_SUPER, ATTR_SUB, 
	ATTR_STRIKE, 
	ATTR_DBL_STRIKE, 

	ATTR_EXPAND,
	/* ATTR_CONDENSE */
};

enum special_char {
	CHAR_BULLET,
	CHAR_LEFT_QUOTE,
	CHAR_RIGHT_QUOTE,
	CHAR_LEFT_DBL_QUOTE,
	CHAR_RIGHT_DBL_QUOTE,
	CHAR_NONBREAKING_SPACE,
	CHAR_EMDASH,
	CHAR_ENDASH,
	CHAR_LESSTHAN,
	CHAR_GREATERTHAN,
	CHAR_AMP,
	CHAR_COPYRIGHT,
	CHAR_TRADEMARK,
	CHAR_NONBREAKING_HYPHEN,
	CHAR_OPTIONAL_HYPHEN,
	CHAR_DBL_QUOTE,
	CHAR_LINE_BREAK,
	CHAR_PAGE_BREAK,
	CHAR_PARAGRAPH_BEGIN,
	CHAR_CENTER_BEGIN,	// move to attr_code
	CHAR_RIGHT_BEGIN,	// -- || --
	CHAR_JUSTIFY_BEGIN,	// -- || --
	CHAR_CENTER_END,	// -- || --
	CHAR_RIGHT_END,		// -- || --
	CHAR_JUSTIFY_END,	// -- || --
	CHAR_TAB,
	
	LAST_CHARS
};

extern const char * std_fonts [];

enum std_fonts_id 
{
	FONTNIL_ID = 0,
	FONTROMAN_ID,
	FONTSWISS_ID,
	FONTMODERN_ID,
	FONTSCRIPT_ID,
	FONTDECOR_ID,
	FONTTECH_ID,
};


/** 
 *
 */
struct observer 
{
	void * object;

	void (* debug )   ( struct __observer *, const char * );
	void (* warning ) ( struct __observer *, int id, const char * );
	void (* error )   ( struct __observer *, int id, const char *);

	// void (* progress) ( struct __observer *, int step );
	
};

struct source 
{
	void * object;

	size_t (* read)   (struct __source *,char * buf, size_t size);
	void (* open)     (struct __source *);
	void (* close)    (struct __source *);

};

struct output 
{
	void (*append) ( struct __output *, const char * );
};

struct image_mgr 
{
	void * object;

	void (* put) (struct __image_mgr *, const unsigned char * ch, unsigned long size );
	
	// size_t part_length; // = 512;
	// void (* mput) (struct __image_mgr *, unsigned char * buf, size_t length);

};

enum picture_type {
	PICT_UNKNOWN=0,
	PICT_WM,
	PICT_MAC,
	PICT_PM,
	PICT_DI,
	PICT_WB,
	PICT_JPEG,
	PICT_PNG,
};

enum document_summary {
	SUMMARY_NUBMER_OF_PAGES,
	SUMMARY_NUMBER_OF_WORDS,
	SUMMARY_NUMBER_OF_CHARS,
};

struct image_descr
{
	int width;
	int height;
	int bits_per_pixel;	// =1;
	int type; 		// =PICT_UNKNOWN;
	int wmetafile_type;
	char * wmetafile_type_str;
};

struct codepage_info
{
	int cp;
	unsigned short chars[128];
};

struct processor
{
	void		* object;
	
	// add attribute to last block
	void (* attr_push)(struct __processor *, int attr, int param );

	// remove last attribute from last last block if it is equal to attr
	int (* attr_pop)(struct __processor *, int attr);

	// add new block
	void (* attrstack_push)(struct __processor *);

	// remove last block and rollback all attributes
	void (* attrstack_drop) (struct __processor *);

	// clear last block without rollingback attributes
	void (* attr_drop_all) (struct __processor *);

	// rollback all attributes from last block and removes all attributes from last block
	void (* attr_pop_all) (struct __processor *);

	// rollback all attributes from last block without removing them
	void (* attr_pop_dump) (struct __processor *);

	// remove all possible attribues from last block 
	void (* attr_remove ) (struct __processor *, int * tab, int size );

	// push attributes from registred style
	void (* attr_push_style ) (struct __processor *, enum TextStyleType type, int id );

	void (* begin) (struct __processor *);
	void (* end) (struct __processor *);

	void (* title) (struct __processor *, const char * );
	void (* keywords) (struct __processor *, const char * );
	void (* author) (struct __processor *, const char * );
	void (* summary) (struct __processor *, enum document_summary type, int param ); 

	void (* hyperlink_base) (struct __processor *, const char * url );

	void (* font_smaller_begin ) ( struct __processor * );
	void (* font_smaller_end ) ( struct __processor * );

	char* (* translate_char) (struct __processor *, int );

	void (*print) ( struct __processor * , const char * );
	void (*print_symbol) ( struct __processor *, const char * );
	void (*print_forced_space) ( struct __processor * );
	int  (*print_unicode) (struct __processor *, int );
	void (*hyperlink) ( struct __processor *, const char * );
	void (*print_char) ( struct __processor *, enum special_char );

	void (*table_begin) ( struct __processor *);
	void (*table_end) ( struct __processor *);
	void (*table_cell_begin) ( struct __processor *);
	void (*table_cell_end) ( struct __processor *);
	void (*table_row_begin) (struct __processor *);
	void (*table_row_end) (struct __processor *);

	image_mgr * (*image_begin) ( struct __processor *, const image_descr * );
	void (*image_end) ( struct __processor *, image_mgr * );
	
	void (* charset) (struct __processor *, int charset_id );
	void (* codepage) (struct __processor *, int cp_id );
	
	void (* register_font ) (struct __processor *, int id, const char * font_name );
	void (* register_color ) (struct __processor *, unsigned char r, unsigned char g, unsigned char b );
	void (* register_style ) (struct __processor *, const TextStyle * style );

};

#endif /// ___TEXT_PROCESSOR_H___

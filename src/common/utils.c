/*
 * Copyright (C) 2017-2023 Adrien Gesta-Fline
 *
 * This file is part of libAAF.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <wchar.h>
#include <stdlib.h>

#include "utils.h"




int snprintf_realloc( char **str, int *size, size_t offset, const char *format, ... )
{
  int tmpsize = 0;
  if ( !size ) {
    size = &tmpsize;
  }

  int retval, needed;
  va_list ap;
  va_start(ap, format);
  while ( 0 <= (retval = vsnprintf((*str)+offset, (*size)-offset, format, ap)) // Error?
         && (int64_t)((*size)-offset) <  (needed = retval + 1)) // Space?
  {
    va_end(ap);
    *size *= 2;                                                  // Space?
    if ((int64_t)((*size)-offset) < needed) *size = needed + offset;                          // Space!
    char *p = realloc(*str, *size);                              // Alloc
    if (p) {
      *str = p;
    } else {
      free(*str);
      *str  = NULL;
      *size = 0;
      return -1;
    }
    va_start(ap, format);
  }
  va_end(ap);
  return retval;
}


int vsnprintf_realloc( char **str, int *size, int offset, const char *fmt, va_list *args )
{
  va_list args2, args3;
  va_copy( args2, *args );
  va_copy( args3, *args );

  int needed = vsnprintf( NULL, 0, fmt, args2 ) + 1;
  if ( needed >= (*size) - offset ) {
    char *p = realloc( *str, offset+needed );
    if (p) {
      *str = p;
      *size = offset+needed;
    } else {
      /* TODO: realloc() faillure */
      // free(*str);
      // *str  = NULL;
      // *size = 0;
      // return -1;
    }
  }
  va_end( args2 );

  int written = vsnprintf( (*str)+offset, (*size)-offset, fmt, args3 );
  va_end( args3 );

  return written;
}



char * c99strdup( const char *src )
{
  if ( !src ) {
    return NULL;
  }

  int len = 0;

  while ( src[len] ) {
    len++;
  }

  char *str = malloc( len + 1 );

  if ( !str )
    return NULL;

  char *p = str;

  while ( *src ) {
    *(p++) = *(src++);
  }

  *p = '\0';

  return str;
}


size_t utf16toa( char *astr, uint16_t alen, uint16_t *wstr, uint16_t wlen )
{
     uint32_t i = 0;

     /*
      *  Remove the leading byte in SF_DATA_STREAM if strlen is odd
      *  -> /Header-2/EssenceData-1902/properties -> PID_EssenceData_Data : Data-2702
      *
      *  TODO What is that leading byte doing here ???? -> 0x55 (U)
      */

     if ( wlen % 2 )
     {
         wstr = (uint16_t*)((char*)wstr + 1);
         wlen--;
     }

     uint16_t len = (wlen >> 1);

     for ( i = 0; (i < len && i < alen); i++ )
         astr[i] = (char)wstr[i];


     astr[--i] = 0x00;

     return i;
}



wchar_t * atowchar( const char *astr, uint16_t alen )
{
	uint16_t i = 0;

	wchar_t *wstr = calloc( alen+1, sizeof(wchar_t) );

	for ( i = 0; i < alen; i++ )
		wstr[i] = (wchar_t)astr[i];


//	wstr[--i] = 0x0000;

	return wstr;
}



/* https://stackoverflow.com/questions/2736753/how-to-remove-extension-from-file-name */
char *remove_file_ext (char* myStr, char extSep, char pathSep)
{
    char *retStr, *lastExt, *lastPath;

    // Error checks and allocate string.

    if (myStr == NULL) return NULL;
    if ((retStr = malloc (strlen (myStr) + 1)) == NULL) return NULL;

    // Make a copy and find the relevant characters.

    strcpy (retStr, myStr);
    lastExt = strrchr (retStr, extSep);
    lastPath = (pathSep == 0) ? NULL : strrchr (retStr, pathSep);

    // If it has an extension separator.

    if (lastExt != NULL) {
        // and it's to the right of the path separator.

        if (lastPath != NULL) {
            if (lastPath < lastExt) {
                // then remove it.

                *lastExt = '\0';
            }
        } else {
            // Has extension separator with no path separator.

            *lastExt = '\0';
        }
    }

    // Return the modified string.

    return retStr;
}



wchar_t * w16tow32( wchar_t *w32buf, uint16_t *w16buf, size_t w16len )
{
    size_t i = 0;

    for ( i = 0; i < w16len/2; i++ )
    {
        w32buf[i] = ((uint16_t*)w16buf)[i];
    }

    return w32buf;
}



const char ASCII[] = {

    /*
     *  Basic Latin (ASCII)
     */

    '.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
    '.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
    '.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
    '.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',

    ' ',  '!',  '"',  '#',  '$',  '%',  '&',  '\'',
    '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
    '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',
    '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',

    '@',  'A',  'B',  'C',  'D',  'E',  'F',  'G',
    'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
    'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
    'X',  'Y',  'Z',  '[',  '\\', ']',  '^',  '_',

    '`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',
    'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
    'p',  'q',  'r',  's',  't',  'u',  'v',  'w',
    'x',  'y',  'z',  '{',  '|',  '}',  '~',  '.',

    /*
     *  Latin-1 Supplement
     */

    '.',  ' ',  ',',  '.',  '"',  '.',  '.',  '.',
    '^',  '%',  'S',  '<',  'E',  ' ',  'Z',  ' ',
    ' ',  '\'', '\'', '"',  '"',  '.',  '-',  '-',
    '~',  '.',  'S',  '>',  'e',  ' ',  'z',  'Y',

    ' ',  'i',  '.',  '.',  '.',  '.',  '|',  '.',
    '.',  '.',  '.',  '"',  '.',  '-',  '.',  '.',
    '.',  '.',  '2',  '3',  '\'', 'u',  '.',  '.',
    '.',  '1',  '.',  '"',  '.',  '.',  '.',  '?',

    'A',  'A',  'A',  'A',  'A',  'A',  'E',  'C',
    'E',  'E',  'E',  'E',  'I',  'I',  'I',  'I',
    '.',  'N',  'O',  'O',  'O',  'O',  'O',  'x',
    'O',  'U',  'U',  'U',  'U',  'Y',  '.',  'B',

    'a',  'a',  'a',  'a',  'a',  'a',  'e',  'c',
    'e',  'e',  'e',  'e',  'i',  'i',  'i',  'i',
    '.',  'n',  'o',  'o',  'o',  'o',  'o',  '/',
    'o',  'u',  'u',  'u',  'u',  'y',  '.',  'y'
};

wchar_t * eascii_to_ascii( wchar_t *str )
{
    size_t len = wcslen( str );
    size_t i = 0;

    for ( ; i < len; i++ )
    {
        if ( str[i] <= 0xff )
            str[i] = ASCII[str[i]];
        else
            str[i] = str[i];
    }

    return str;
}




int dump_hex( const unsigned char *stream, size_t stream_sz, char **buf, int *bufsz, int offset )
{
	if ( stream == NULL )
	{
		// fprintf( stderr, "dump_hex() : Stream pointer is NULL.\n\n" );
		return -1;
	}

  int initialOffset = offset;
	uint32_t i = 0;

	char hex[49];
	char ascii[19];

	uint32_t count   = 0;

	offset += snprintf_realloc( buf, bufsz, offset, " ______________________________ Hex Dump ______________________________\n\n" );

	while ( count < stream_sz )
	{
		uint32_t lineLen = (stream_sz - count) / 16;

		if ( lineLen <= 0 )
			lineLen = (stream_sz) % 16;
		else
			lineLen = 16;


		memset( &hex,   0x20, sizeof(hex) );
		memset( &ascii, 0x00, sizeof(ascii) );

        uint32_t linepos = 0;

		for ( i=0; i < lineLen; i++ )
		{
			linepos += snprintf( &hex[linepos], sizeof(hex)-(linepos), "%02x%s", *(unsigned char*)(stream+count+i), (i==7) ? "  " : " " );

            if ( i < 8 )
            {
                if ( isalnum( *(stream+count+i) ) )     ascii[i] = *(unsigned char*)(stream+count+i);
                else                                    ascii[i] = '.';
            }
            else if ( i > 8 )
            {
                if ( isalnum( *(stream+count+i) ) )     ascii[i+1] = *(unsigned char*)(stream+count+i);
                else                                    ascii[i+1] = '.';
            }
            else
            {
                if ( isalnum( *(stream+count+i) ) )
                {
                    ascii[i] = ' ';
                    ascii[i+1] = *(unsigned char*)(stream+count+i);
                }
                else
                {
                    ascii[i] = ' ';
                    ascii[i+1] = '.';
                }
            }

		}

        /* Fill with blank the rest of the line */
		if ( lineLen < 16 )
        {
            for ( i=linepos; i < 48; i++ )
            {
			    hex[linepos++] = 0x20;
            }
        }

        /* terminate  the line */
        hex[48]  = 0x00;

		count += lineLen;

		offset += snprintf_realloc( buf, bufsz, offset, " %s  |  %s\n", hex, ascii );

	}

	offset += snprintf_realloc( buf, bufsz, offset, " ______________________________________________________________________\n\n" );

  return offset - initialOffset; // bytes written
}




char * url_decode( char *dst, char *src )
{
 	char a, b;

 	while (*src)
 	{
 		if ( (*src == '%') &&
 			 ((a = src[1]) && (b = src[2])) &&
 			 (isxdigit(a)  && isxdigit(b)))
 		{
 			if (a >= 'a')
 				a -= 'a'-'A';
 			if (a >= 'A')
 				a -= ('A' - 10);
 			else
 				a -= '0';
 			if (b >= 'a')
 				b -= 'a'-'A';
 			if (b >= 'A')
 				b -= ('A' - 10);
 			else
 				b -= '0';
 			*dst++ = 16 * a + b;
 			src+=3;
 		}
 		else if (*src == '+')
 		{
 			*dst++ = ' ';
 			src++;
 		}
 		else
 		{
 			*dst++ = *src++;
 		}
 	}

 	*dst++ = '\0';

 	return dst;
}


wchar_t * wurl_decode( wchar_t *dst, wchar_t *src )
{
 	wchar_t a, b;

 	while (*src)
 	{
 		if ( (*src == '%') &&
 			 ((a = src[1]) && (b = src[2])) &&
 			 (isxdigit(a)  && isxdigit(b)))
 		{
 			if (a >= 'a')
 				a -= 'a'-'A';
 			if (a >= 'A')
 				a -= ('A' - 10);
 			else
 				a -= '0';
 			if (b >= 'a')
 				b -= 'a'-'A';
 			if (b >= 'A')
 				b -= ('A' - 10);
 			else
 				b -= '0';
 			*dst++ = 16 * a + b;
 			src+=3;
 		}
 		else if (*src == '+')
 		{
 			*dst++ = ' ';
 			src++;
 		}
 		else
 		{
 			*dst++ = *src++;
 		}
 	}

 	*dst++ = '\0';

 	return dst;
}

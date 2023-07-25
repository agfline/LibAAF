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


#define BUILD_PATH_DEFAULT_BUF_SIZE 1024


int wstr_contains_nonlatin( const wchar_t *str )
{
	for ( size_t i = 0; str[i] != 0x0000; i++ ) {
		/* if char is out of the Basic Latin range */
		if ( str[i] > 0xff ) {
      return 1;
		}
	}

  return 0;
}
// wchar_t * utoa( wchar_t *str )
// {
//   /*
//    * Unicode to ASCII
//    */
//
//   for ( size_t i = 0; str[i]; i++ ) {
//     wchar_t c = str[i];
//     // printf("0x%02x\n", c );
//
//     switch ( c ) {
//
//       case L'à':
//       case L'á':
//       case L'â':
//       case L'ã':
//       case L'ä':
//       case L'å':
//       case L'æ':
//         str[i] = 'a';
//         break;
//
//       case L'é':
//       case L'è':
//       case L'ê':
//       case L'ë':
//         str[i] = 'e';
//         break;
//
//       case L'ì':
//       case L'í':
//       case L'î':
//       case L'ï':
//         str[i] = 'i';
//         break;
//
//       case L'ò':
//       case L'ó':
//       case L'ô':
//       case L'õ':
//       case L'ö':
//       case L'ø':
//       case L'œ':
//         str[i] = 'o';
//         break;
//
//       case L'ù':
//       case L'ú':
//       case L'û':
//       case L'ü':
//         str[i] = 'u';
//         break;
//
//       case L'ý':
//       case L'ÿ':
//         str[i] = 'y';
//         break;
//
//       case L'À':
//       case L'Á':
//       case L'Â':
//       case L'Ã':
//       case L'Ä':
//       case L'Å':
//       case L'Æ':
//         str[i] = 'A';
//         break;
//
//       case L'È':
//       case L'É':
//       case L'Ê':
//       case L'Ë':
//         str[i] = 'E';
//         break;
//
//       case L'Ì':
//       case L'Í':
//       case L'Î':
//       case L'Ï':
//         str[i] = 'I';
//         break;
//
//       case L'Ò':
//       case L'Ó':
//       case L'Ô':
//       case L'Õ':
//       case L'Ö':
//       case L'Ø':
//       case L'Œ':
//         str[i] = 'O';
//         break;
//
//       case L'Ù':
//       case L'Ú':
//       case L'Û':
//       case L'Ü':
//         str[i] = 'U';
//         break;
//
//       case L'Ý':
//       case L'Ÿ':
//         str[i] = 'Y';
//         break;
//
//       case L'ç':
//         str[i] = 'c';
//         break;
//
//       case L'Ç':
//         str[i] = 'C';
//         break;
//
//       case L'ð':
//         str[i] = 'd';
//         break;
//
//       case L'Ð':
//         str[i] = 'D';
//         break;
//
//       case L'ß':
//         str[i] = 'S';
//         break;
//
//       case L'¿':
//         str[i] = '?';
//         break;
//
//       case L'¡':
//         str[i] = '!';
//         break;
//     }
//   }
//
//   return str;
// }



char * clean_filename( char *fname )
{
  /*
   * sanitize file/dir name
   * https://stackoverflow.com/a/31976060
   */
  size_t len = strlen(fname);

  for ( size_t i = 0; i < len; i++ ) {

    unsigned char c = fname[i];

    if ( c == '/' ||
         c == '<' ||
         c == '>' ||
         c == ':' ||
         c == '"' ||
         c == '|' ||
         c == '?' ||
         c == '*' ||
         c == '\\' ||
        (c > 0 && c < 0x20)
       )
    {
      fname[i] = '_';
    }
  }

  /* windows filenames can't end with ' ' or '.' */
  for ( int i = len-1; i > 0; i-- ) {
    char c = fname[i];
    if ( c != ' ' && c != '.' ) {
      break;
    }
    fname[i] = '_';
  }

  return fname;
}



const char * fop_get_file( const char *filepath )
{
  if ( filepath == NULL ) {
    return NULL;
  }

  const char *end = filepath + strlen(filepath);

  while ( end > filepath && !IS_DIR_SEP(*end) ) {
    --end;
  }

  return ( IS_DIR_SEP(*end) ) ? end+1 : end;
}



char * build_path( const char *sep, const char *first, ... )
{
  char *str = malloc( BUILD_PATH_DEFAULT_BUF_SIZE );
  size_t len = BUILD_PATH_DEFAULT_BUF_SIZE;
  size_t offset = 0;

  va_list args;

  if ( !sep ) {
    sep = DIR_SEP_STR;
  }


  int element_count = 0;
  va_start( args, first );

  const char *arg = first;

  do {

    int arglen = strlen(arg);
    int argstart = 0;
    int has_leading_sep = 0;

    /* trim leading DIR_SEP */
    for ( int i = 0; arg[i] != 0x00; i++ ) {
      if ( IS_DIR_SEP(arg[i]) ) {
        has_leading_sep = 1;
        argstart++;
      } else {
        break;
      }
    }

    /* trim trailing DIR_SEP */
    for ( int i = arglen-1; i >= argstart; i-- ) {
      if ( IS_DIR_SEP(arg[i]) ) {
        arglen--;
      } else {
        break;
      }
    }

    if ( element_count == 0 && has_leading_sep ) {
    } else {
    }

    size_t reqlen = snprintf( NULL, 0, "%.*s", arglen-argstart, arg+argstart ) + 1;

    if ( offset + reqlen >= len ) {
      reqlen = ((offset + reqlen) > (len + BUILD_PATH_DEFAULT_BUF_SIZE)) ? (offset + reqlen) : (len + BUILD_PATH_DEFAULT_BUF_SIZE);
      char *tmp = realloc( str, (offset + reqlen) );
      if ( tmp ) {
        str = tmp;
        len = (offset + reqlen);
      } else {
        free( str );
        return NULL;
      }
    }

    offset += snprintf( str + offset, len - offset, "%s%.*s",
      ( (element_count == 0 && has_leading_sep) || (element_count > 0) ) ? sep : "",
      arglen-argstart,
      arg+argstart );

    element_count++;

  } while ( (arg = va_arg(args, char*)) != NULL );

  va_end( args );

  // printf( "built path : %s\n", str );

  return str;
}



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
			linepos += snprintf( &hex[linepos], sizeof(hex)-(linepos), "%02x%s", *(const unsigned char*)(stream+count+i), (i==7) ? "  " : " " );

            if ( i < 8 )
            {
                if ( isalnum( *(stream+count+i) ) )     ascii[i] = *(const unsigned char*)(stream+count+i);
                else                                    ascii[i] = '.';
            }
            else if ( i > 8 )
            {
                if ( isalnum( *(stream+count+i) ) )     ascii[i+1] = *(const unsigned char*)(stream+count+i);
                else                                    ascii[i+1] = '.';
            }
            else
            {
                if ( isalnum( *(stream+count+i) ) )
                {
                    ascii[i] = ' ';
                    ascii[i+1] = *(const unsigned char*)(stream+count+i);
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

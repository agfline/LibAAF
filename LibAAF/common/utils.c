
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <wchar.h>



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




void dump_hex( const unsigned char * stream, size_t stream_sz )
{
	if ( stream == NULL )
	{
		fprintf( stderr, "dump_hex() : Stream pointer is NULL.\n" );
		return;
	}


	uint32_t i = 0;

	char hex[49];
	char ascii[19];

	uint32_t count   = 0;

	printf( " ______________________________ Hex Dump ______________________________\n\n" );

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

		printf( " %s  |  %s\n", hex, ascii );

	}

	printf( " ______________________________________________________________________\n\n" );

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

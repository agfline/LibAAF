
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

const char ASCII[] = {
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

char * eascii_to_ascii( char *str )
{
    int len = strlen( str );
    int i = 0;

    for ( ; i < len; i++ )
    {
        str[i] = ASCII[(uint8_t)str[i]];
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


		memset( &hex,   0x20, 48 );
		memset( &ascii, 0x00, 32 );

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

 /*
  *  This file is part of LibAAF.
  *
  *  Copyright (c) 2017 Adrien Gesta-Fline
  *
  *  LibAAF is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU Affero General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  any later version.
  *
  *  LibAAF is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU Affero General Public License for more details.
  *
  *  You should have received a copy of the GNU Affero General Public License
  *  along with LibAAF. If not, see <http://www.gnu.org/licenses/>.
  */

#include <stdio.h>
#include <string.h>

#include <ctype.h>	// isxdigit()

#include "AAFTypes.h"
#include "AAFUtils.h"



size_t utf16toa( char *astr, uint16_t alen, uint16_t *wstr, uint16_t wlen )
{
     uint32_t i = 0;

     // Remove the leading byte in SF_DATA_STREAM if strlen is odd
     // -> /Header-2/EssenceData-1902/properties -> PID_EssenceData_Data : Data-2702
     // TODO What is that leading byte doing here ???? -> 0x55 (U)
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




void printObjectProperties( AAF_Data *aafd, aafObject *Obj )
{
	aafProperty * Prop = NULL;

	for ( Prop = Obj->Properties;  Prop != NULL; Prop = Prop->next )
	{
		printf( ":.: (0x%04x) %s\n", Prop->pid, PIDToText( aafd, Prop->pid ) );

		// WARNING : Wont print strong references (set/vector) corectly.
		dump_hex( Prop->val, Prop->len );
	}
}


/*
char * printMobID( aafMobID_t *mobid )
{
	static char buf[2 * sizeof(aafMobID_t)];

	uint32_t i = 0;
    uint32_t offset = 0;

	for ( i = 0; i < sizeof(aafMobID_t); i++ )
    {
		offset += snprintf( buf+offset, (2 * sizeof(aafMobID_t)), "%02x", ((unsigned char*)mobid)[i] );
    }

	return buf;
}
*/





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

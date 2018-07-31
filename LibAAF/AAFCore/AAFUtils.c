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



void printStream( unsigned char * stream, size_t stream_sz )
{
     uint32_t i = 0;

     char hex[48];
     char str[32];

     uint32_t count   = 0;
     uint32_t lineLen = 0;

     printf( " _____________________________ Hex Dump _____________________________\n\n" );

     while ( count < stream_sz )
     {
         lineLen = (stream_sz - count) / 16;

         if ( lineLen <= 0 )
             lineLen = (stream_sz) % 16;
         else
             lineLen = 16;


         memset( &hex, 0x20, 48 );
         memset( &str, 0x00, 32 );

         // build line
         for ( i=0; i < lineLen; i++ )
         {
             // build the HEX part
             snprintf( &hex[i*3], 48-(i*3), "%02x ", *(unsigned char*)(stream+count+i) );

             // build the ASCII part
             if ( *(unsigned char*)(stream+count+i) > 0x1F &&
                  *(unsigned char*)(stream+count+i) < 0x7F )
                     str[i] = *(unsigned char*)(stream+count+i);
             else
                     str[i] = '.';
         }

         hex[47]  = 0x00;

         // fill the HEX part with blank if needed
         // TODO is it necessary ? ( see memset() above )
         if ( lineLen < 16 )
             hex[i*3] = 0x20;

         count += lineLen;

         // print line
         printf( " %s  |  %s\n", hex, str );

     }

     printf( " ____________________________________________________________________\n\n" );
}





void printObjectProperties( AAF_Data *aafd, aafObject *Obj )
{
	aafProperty * Prop = NULL;

	for ( Prop = Obj->Properties;  Prop != NULL; Prop = Prop->next )
	{
		printf( ":.: (0x%04x) %s\n", Prop->pid, PIDToText( aafd, Prop->pid ) );

		// WARNING : Wont print strong references (set/vector) corectly.
		cfb_printStream( Prop->val, Prop->len );
	}
}




// 
// char * printUID( aafUID_t *auid )
// {
// 	static char buf[74];
//
// 	snprintf( buf, 74, "{0x%08x, 0x%04x, 0x%04x, { 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x }}",
// 		auid->Data1,
// 		auid->Data2,
// 		auid->Data3,
//
// 		auid->Data4[0],
// 		auid->Data4[1],
// 		auid->Data4[2],
// 		auid->Data4[3],
// 		auid->Data4[4],
// 		auid->Data4[5],
// 		auid->Data4[6],
// 		auid->Data4[7]
// 	);
//
// 	return buf;
// }



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

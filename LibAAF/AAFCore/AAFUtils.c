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

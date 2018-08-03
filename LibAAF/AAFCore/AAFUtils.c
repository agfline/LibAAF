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

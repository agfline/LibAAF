#ifndef __AAFUtils_h__
#define __AAFUtils_h__

/*
 *	This file is part of LibAAF.
 *
 *	Copyright (c) 2017 Adrien Gesta-Fline
 *
 *	LibAAF is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Affero General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	any later version.
 *
 *	LibAAF is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Affero General Public License for more details.
 *
 *	You should have received a copy of the GNU Affero General Public License
 *	along with LibAAF. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 *	@brief Some utility functions and macro
 *	@author Adrien Gesta-Fline
 *	@version 0.1
 *	@date 04 october 2017
 */

#include <string.h>

#include "AAFTypes.h"
#include "AAFCore.h"
#include "AAFToText.h"





/**
 *	Compares two aafUID_t, returns 1 if equal or 0 otherwise.
 */

#define auidCmp( auid1, auid2 ) \
	( memcmp( auid1, auid2, sizeof(aafUID_t) ) == 0 )



/**
 *	Compares two aafMobID_t, returns 1 if equal or 0 otherwise.
 */

#define mobIDCmp( mobID1, mobID2 ) \
	( memcmp( mobID1, mobID2, sizeof(aafMobID_t) ) == 0 )



/**
 *	Compares two aafSlotID_t, returns 1 if equal or 0 otherwise.
 */

#define slotIDCmp( slotID1, slotID2 ) \
	( slotID1 == slotID2 )



/**
 *	Converts an aafRational_t to a float number.
 */

#define rationalToFloat( r ) \
	(( r->denominator == 0 ) ? 0 : ((float)r->numerator/r->denominator))


/**
 *	Converts an aafRational_t to a int64 number.
 */

#define rationalToint64( r ) \
	(( r->denominator == 0 ) ? 0 : (int64_t)(r->numerator/r->denominator))



size_t utf16toa( char *astr, uint16_t alen, uint16_t *wstr, uint16_t wlen );

// void   printObjectProperties( AAF_Data *aafd, aafObject *Obj );


#endif // ! __AAFUtils_h__

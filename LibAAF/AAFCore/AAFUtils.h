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

void   printStream( unsigned char * stream, size_t stream_sz );

void   printObjectProperties( AAF_Data *aafd, aafObject *Obj );

char * printUID( aafUID_t *auid );

char * printMobID( aafMobID_t *mobid );

char  *url_decode( char *dst, char *src );

/*
int AAF_MobIDCompare( const aafMobID_t *MobID1, const aafMobID_t *MobID2 )
{
	if ( MobID1->SMPTELabel[0] != MobID2->SMPTELabel[0] ||
		 MobID1->SMPTELabel[1] != MobID2->SMPTELabel[1] ||
		 MobID1->SMPTELabel[2] != MobID2->SMPTELabel[2] ||
		 MobID1->SMPTELabel[3] != MobID2->SMPTELabel[3] ||
		 MobID1->SMPTELabel[4] != MobID2->SMPTELabel[4] ||
		 MobID1->SMPTELabel[5] != MobID2->SMPTELabel[5] ||
		 MobID1->SMPTELabel[6] != MobID2->SMPTELabel[6] ||
		 MobID1->SMPTELabel[7] != MobID2->SMPTELabel[7] ||
		 MobID1->SMPTELabel[8] != MobID2->SMPTELabel[8] ||
		 MobID1->SMPTELabel[9] != MobID2->SMPTELabel[9] ||
		 MobID1->SMPTELabel[10] != MobID2->SMPTELabel[10] ||
		 MobID1->SMPTELabel[11] != MobID2->SMPTELabel[11] )
			return 0;

	if ( MobID1->length != MobID2->length )
			return 0;

	if ( MobID1->instanceHigh != MobID2->instanceHigh ||
		 MobID1->instanceMid  != MobID2->instanceMid  ||
		 MobID1->instanceLow  != MobID2->instanceLow )
			return 0;

	if ( MobID1->material.Data1 != MobID2->material.Data1 ||
		 MobID1->material.Data2 != MobID2->material.Data2 ||
		 MobID1->material.Data3 != MobID2->material.Data3 ||
		 MobID1->material.Data4[0] != MobID2->material.Data4[0] ||
		 MobID1->material.Data4[1] != MobID2->material.Data4[1] ||
		 MobID1->material.Data4[2] != MobID2->material.Data4[2] ||
		 MobID1->material.Data4[3] != MobID2->material.Data4[3] ||
		 MobID1->material.Data4[4] != MobID2->material.Data4[4] ||
		 MobID1->material.Data4[5] != MobID2->material.Data4[5] ||
		 MobID1->material.Data4[6] != MobID2->material.Data4[6] ||
		 MobID1->material.Data4[7] != MobID2->material.Data4[7] )
			return 0;


	return 1;
}
*/

#endif // ! __AAFUtils_h__

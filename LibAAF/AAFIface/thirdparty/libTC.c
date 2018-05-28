#ifndef __libTC_h__
#define __libTC_h__

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>


enum timecode_type_e
{
	TC_DROP_FRAME     = 0,
	TC_NON_DROP_FRAME = 1,

} timecode_type_e;

typedef struct rational_t
{
	int32_t numerator;
	int32_t denominator;

} rational_t;

typedef struct timecode_t
{
	uint64_t   tc;

	uint8_t    df;

	rational_t fps;

	char       sep;
	char       sep_df;
	char       sep_ndf;

} timecode_t;





/**
 *	@}
 */

#endif // ! __libTC_h__

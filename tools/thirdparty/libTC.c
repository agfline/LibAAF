/*
 * Copyright (C) 2017-2024 Adrien Gesta-Fline
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
#include <stdlib.h> // abs()
#include <math.h>	// round()
#include <assert.h>
#include <limits.h> 

#include "libTC.h"



/*
 * SMPTE ST12-1 p9 :
 * Sufficient precision is necessary in calculations to assure that rounding or
 * truncation operations will not create errors in the end result.
 */

static rational_t TC_FPS[] = {
	{0x00000000, 0x00000001},  /* UNKNOWN       0/1        */
	{0x00005dc0, 0x000003e9},  /* TC_23_976     24000/1001 */
	{0x00000018, 0x00000001},  /* TC_24         24/1       */
	{0x00000019, 0x00000001},  /* TC_25         25/1       */
	{0x00007530, 0x000003e9},  /* TC_29_97_NDF  30000/1001 */
	{0x00007530, 0x000003e9},  /* TC_29_97_DF   30000/1001 */
	{0x0000001e, 0x00000001},  /* TC_30_NDF     30/1       */
	{0x0000001e, 0x00000001},  /* TC_30_DF      30/1       */
	{0x0000bb80, 0x000003e9},  /* TC_47_95      48000/1001 */
	{0x00000030, 0x00000001},  /* TC_48         48/1       */
	{0x00000032, 0x00000001},  /* TC_50         50/1       */
	{0x0000ea60, 0x000003e9},  /* TC_59_94_NDF  60000/1001 */
	{0x0000ea60, 0x000003e9},  /* TC_59_94_DF   60000/1001 */
	{0x0000003c, 0x00000001},  /* TC_60_NDF     60/1       */
	{0x0000003c, 0x00000001},  /* TC_60_DF      60/1       */
	{0x00000048, 0x00000001},  /* TC_72         72/1       */
	{0x00000060, 0x00000001},  /* TC_96         96/1       */
	{0x00000064, 0x00000001},  /* TC_100        100/1      */
	{0x00002ed4, 0x00000064},  /* TC_119_88_NDF 11988/100  */
	{0x00002ed4, 0x00000064},  /* TC_119_88_DF  11988/100  */
	{0x00000078, 0x00000001},  /* TC_120_NDF    120/1      */
	{0x00000078, 0x00000001}   /* TC_120_DF     120/1      */
};



const char *TC_FORMAT_STR[] = {
	"unknown",
	"TC_23_976",
	"TC_24",
	"TC_25",
	"TC_29_97_NDF",
	"TC_29_97_DF",
	"TC_30_NDF",
	"TC_30_DF",
	"TC_47_95",
	"TC_48",
	"TC_50",
	"TC_59_94_NDF",
	"TC_59_94_DF",
	"TC_60_NDF",
	"TC_60_DF",
	"TC_72",
	"TC_96",
	"TC_100",
	"TC_119_88_NDF",
	"TC_119_88_DF",
	"TC_120_NDF",
	"TC_120_DF"
};

#define TC_RATIONAL_TO_FLOAT( r ) \
	(( r.denominator == 0 ) ? 0 : ((float)r.numerator/(float)r.denominator))




static int is_format_drop_frame( enum TC_FORMAT tcfmt );
static void unitValueToFrames( struct timecode *tc );
static void hmsfToString( struct timecode *tc );
static void stringToHmsf( struct timecode *tc );
static void hmsfToFrames( struct timecode *tc );
static void framesToHmsf( struct timecode *tc, enum TC_ROLLOVER_OPT rollover );




enum TC_FORMAT tc_get_format_by_fps( float fps, uint8_t isDrop )
{
	uint32_t ifps = (uint32_t)( fps * 100 );

	// printf("%i\n", ifps );

	uint16_t tc_format = 0;

	for ( tc_format = 0; tc_format < TC_FORMAT_LEN; tc_format++ ) {

		if ( ifps == (uint32_t)(TC_RATIONAL_TO_FLOAT( TC_FPS[tc_format] ) * 100) ) {

			// printf("tc_format: %i (%i) == %i\n", ifps, isDrop, (uint32_t)(TC_RATIONAL_TO_FLOAT( TC_FPS[tc_format] ) * 100) );

			if ( ( tc_format == TC_29_97_NDF ||
			       tc_format == TC_30_NDF ||
			       tc_format == TC_59_94_NDF ||
			       tc_format == TC_60_NDF ||
			       tc_format == TC_119_88_NDF ||
			       tc_format == TC_120_NDF ) &&
			       isDrop == 1 )
			{
				return ++tc_format;
			}

			return tc_format;
		}
	}

	return TC_FORMAT_UNK;
}



void tc_set_by_unitValue( struct timecode *tc, int64_t unitValue, rational_t *unitRate, enum TC_FORMAT format, enum TC_ROLLOVER_OPT rollover )
{
	memset( tc, 0x00, sizeof(struct timecode) );

	if ( unitRate == NULL ) {
		return;
	}

	tc->format    = format;
	tc->unitValue = unitValue;
	tc->unitRate  = *unitRate;

	unitValueToFrames( tc );

	framesToHmsf( tc, rollover );

	hmsfToString( tc );
}



void tc_set_by_frames( struct timecode *tc, int32_t frameNumber, enum TC_FORMAT format, enum TC_ROLLOVER_OPT rollover )
{
	memset( tc, 0x00, sizeof(struct timecode) );

	tc->format      = format;
	tc->frameNumber = frameNumber;

	framesToHmsf( tc, rollover );

	hmsfToString( tc );
}



// void rollover( struct timecode *tc ) {
//
// 	float fps = round( TC_RATIONAL_TO_FLOAT( TC_FPS[tc->format] ) );
//
// 	if ( is_format_drop_frame(tc->format) ) {
//
// 		/*
// 		 * SMPTE ST 12-1 p10 :
// 		 * To minimize the NTSC time deviation from real time, the first two
// 		 * frame numbers (00 and 01) shall be omitted from the count at the
// 		 * start of each minute except minutes 00, 10, 20, 30, 40, and 50.
// 		 */
//
// 		if ( rollover == TC_ROLLOVER ) {
// 			/* Rollover if frameNumber > 23:59:59:XX */
// 			uint32_t frameNumber24h = (((((fps*60))*10) - (2*9)) * 6 * 24);
// 			frameNumber = frameNumber % frameNumber24h;
// 		}
//
//
// 		int32_t framesPerMinute    = (fps * 60) - 2;
// 		int32_t framesPer10Minutes = (framesPerMinute * 10) + 2;
//
// 		int32_t chunksOf10Minutes  = frameNumber / framesPer10Minutes;
// 		int32_t chunksOf1Minute    = ((int32_t)(frameNumber % framesPer10Minutes) - 2) / framesPerMinute; /* minus 2 corresponds to the first two frame numbers (00 and 01) */
//
// 		int32_t tenMinuteDrops     = 2 * 9 * chunksOf10Minutes;
// 		int32_t oneMinuteDrops     = ( chunksOf1Minute < 0 ) ? 0 : (2 * chunksOf1Minute);
//
//
// 		uint32_t dropFrames = tenMinuteDrops + oneMinuteDrops;
//
//
// 		if ( tc->format == TC_59_94_DF ||
// 		     tc->format == TC_60_DF )
// 		{
// 			dropFrames *= 2;
// 		}
// 		else if ( tc->format == TC_119_88_DF ||
// 		          tc->format == TC_120_DF )
// 		{
// 			dropFrames *= 4;
// 		}
//
// 		frameNumber += dropFrames;
// 	}
// 	else if ( rollover == TC_ROLLOVER ) {
// 		/* Rollover if frameNumber > 23:59:59:XX */
// 		uint32_t frameNumber24h = (((fps*60)*10) * 6 * 24);
// 		frameNumber = (frameNumber24h) ? frameNumber % frameNumber24h : 0;
// 	}
// }



void tc_set_by_hmsf( struct timecode *tc, int hours, int minutes, int seconds, int frames, enum TC_FORMAT format, enum TC_ROLLOVER_OPT rollover )
{
	memset( tc, 0x00, sizeof(struct timecode) );

	tc->format  = format;
	tc->hours   = hours;
	tc->minutes = minutes;
	tc->seconds = seconds;
	tc->frames  = frames;

	hmsfToFrames( tc );

	if ( rollover ) {
		framesToHmsf( tc, rollover );
	}

	hmsfToString( tc );

	// if ( rollover ) {
	// 	rollover( tc );
	// }
}



void tc_set_by_string( struct timecode *tc, const char *str, enum TC_FORMAT format, enum TC_ROLLOVER_OPT rollover )
{
	memset( tc, 0x00, sizeof(struct timecode) );

	tc->format = format;
	strncpy( tc->string, str, sizeof(((struct timecode){0}).string)-1 );

	stringToHmsf( tc );

	hmsfToFrames( tc );

	if ( rollover ) {
		framesToHmsf( tc, rollover );
		hmsfToString( tc );
	}

	// if ( rollover ) {
	// 	rollover( tc );
	// }
}



void tc_set_format( struct timecode *tc, enum TC_FORMAT format, enum TC_ROLLOVER_OPT rollover )
{
	tc->format  = format;

	framesToHmsf( tc, rollover );
	hmsfToString( tc );
}



// void tc_convert( struct timecode *tc, enum TC_FORMAT format, enum TC_ROLLOVER_OPT rollover )
// {
// 	tc->format  = format;
//
// 	if ( tc->frames > round(TC_RATIONAL_TO_FLOAT(TC_FPS[format])) ) {
// 		tc->frames = round(TC_RATIONAL_TO_FLOAT(TC_FPS[format])) - 1;
// 	}
//
// 	hmsfToFrames( tc );
// 	framesToHmsf( tc, rollover );
// 	hmsfToString( tc );
// }



int tc_add( struct timecode *tc_a, struct timecode *tc_b, enum TC_ROLLOVER_OPT rollover )
{
	if ( tc_a->format != tc_b->format ) {
		return -1;
	}

	tc_a->frameNumber += tc_b->frameNumber;

	framesToHmsf( tc_a, rollover );

	hmsfToString( tc_a );

	return 0;
}



int tc_sub( struct timecode *tc_a, struct timecode *tc_b, enum TC_ROLLOVER_OPT rollover )
{
	if ( tc_a->format != tc_b->format ) {
		return -1;
	}

	tc_a->frameNumber -= tc_b->frameNumber;

	framesToHmsf( tc_a, rollover );

	hmsfToString( tc_a );

	return 0;
}



static int is_format_drop_frame( enum TC_FORMAT tcfmt )
{
	return ( tcfmt == TC_29_97_DF  ||
	         tcfmt == TC_30_DF     ||
	         tcfmt == TC_59_94_DF  ||
	         tcfmt == TC_60_DF     ||
	         tcfmt == TC_119_88_DF ||
	         tcfmt == TC_120_DF );
}



static void unitValueToFrames( struct timecode *tc )
{
	float fps      = TC_RATIONAL_TO_FLOAT( TC_FPS[tc->format] );
	float unitRate = TC_RATIONAL_TO_FLOAT( tc->unitRate       );

	if ( fps == unitRate ) {

		assert( unitRate > INT_MIN && unitRate < INT_MAX );

		tc->frameNumber = (int32_t)tc->unitValue;

	} else {
		tc->frameNumber = (int32_t)( (float)tc->unitValue / ( (float)unitRate / (float)fps ) );
	}

	// printf( "%lu (%i/%i) -> %u (%i/%i)\n",
	// 	tc->unitValue,
	// 	tc->unitRate.numerator,
	// 	tc->unitRate.denominator,
	// 	tc->frameNumber,
	// 	TC_FPS[tc->format].numerator,
	// 	TC_FPS[tc->format].denominator );
}



static void hmsfToString( struct timecode *tc )
{
	snprintf( tc->string,  16, "%s%02u%c%02u%c%02u%c%02u",
	          (tc->frameNumber < 0) ? "-" : "",
	          (tc->hours   <= 9999) ? tc->hours   : 0, TC_SEP_CHAR,
	          (tc->minutes <=   99) ? tc->minutes : 0, TC_SEP_CHAR,
	          (tc->seconds <=   99) ? tc->seconds : 0, (is_format_drop_frame( tc->format )) ? TC_SEP_DROP_CHAR : TC_SEP_CHAR,
	          (tc->frames  <=  999) ? tc->frames  : 0 );
}



static void stringToHmsf( struct timecode *tc )
{
	// sscanf( tc->string, "%hi%*c%hi%*c%hi%*c%hi",
	sscanf( tc->string, "%i%*c%i%*c%i%*c%i",
	       &tc->hours,
	       &tc->minutes,
	       &tc->seconds,
	       &tc->frames );
}



static void hmsfToFrames( struct timecode *tc )
{
	double fps = round(TC_RATIONAL_TO_FLOAT( TC_FPS[tc->format] ));

	uint32_t dropFrames = 0;

	if ( is_format_drop_frame(tc->format) ) {
		int dropFramesInt = (tc->hours   * (2 * 9 * 6))    + \
		                   ((tc->minutes / 10)  * (2 * 9)) + \
		                  (((tc->minutes % 10)) *  2);

		assert( dropFramesInt >= 0 );

		dropFrames = (uint32_t)dropFramesInt;
	}

	if ( tc->format == TC_59_94_DF ||
	     tc->format == TC_60_DF )
	{
		dropFrames *= 2;
	}
	else if ( tc->format == TC_119_88_DF ||
	          tc->format == TC_120_DF )
	{
		dropFrames *= 4;
	}

	double frameNumber = (tc->hours * 3600 * fps) + \
	                     (tc->minutes * 60 * fps) + \
	                     (tc->seconds * fps )     + \
	                     (tc->frames )            - \
	                     dropFrames;

	assert( frameNumber > INT_MIN && frameNumber < INT_MAX );

	tc->frameNumber = (int32_t)frameNumber;
}



static void framesToHmsf( struct timecode *tc, enum TC_ROLLOVER_OPT rollover )
{
	int32_t frameNumber = abs( tc->frameNumber );

	double fps = round( TC_RATIONAL_TO_FLOAT( TC_FPS[tc->format] ) );


	if ( is_format_drop_frame(tc->format) ) {

		/*
		 * SMPTE ST 12-1 p10 :
		 * To minimize the NTSC time deviation from real time, the first two
		 * frame numbers (00 and 01) shall be omitted from the count at the
		 * start of each minute except minutes 00, 10, 20, 30, 40, and 50.
		 */

		if ( rollover == TC_ROLLOVER ) {
			/* Rollover if frameNumber > 23:59:59:XX */
			double frameNumber24hdouble = (((((fps*60))*10) - (2*9)) * 6 * 24/*hours*/);

			assert( frameNumber24hdouble > 0 && frameNumber24hdouble < INT_MAX );

			int32_t frameNumber24h = (int32_t)frameNumber24hdouble;

			frameNumber = frameNumber % frameNumber24h;
		}

		double framesPerMinuteDouble = (fps * 60) - 2;

		assert( framesPerMinuteDouble > 0 && framesPerMinuteDouble < INT_MAX );

		int32_t framesPerMinute = (int32_t)framesPerMinuteDouble;

		int32_t framesPer10Minutes = (framesPerMinute * 10) + 2;

		int32_t chunksOf10Minutes  = frameNumber / framesPer10Minutes;
		int32_t chunksOf1Minute    = ((int32_t)(frameNumber % framesPer10Minutes) - 2) / framesPerMinute; /* minus 2 corresponds to the first two frame numbers (00 and 01) */

		int32_t tenMinuteDrops     = 2 * 9 * chunksOf10Minutes;
		int32_t oneMinuteDrops     = (chunksOf1Minute < 0) ? 0 : (2 * chunksOf1Minute);

		int32_t droppedFrames =  abs( tenMinuteDrops + oneMinuteDrops );

		if ( tc->format == TC_59_94_DF ||
		     tc->format == TC_60_DF )
		{
			droppedFrames *= 2;
		}
		else if ( tc->format == TC_119_88_DF ||
		          tc->format == TC_120_DF )
		{
			droppedFrames *= 4;
		}

		frameNumber += droppedFrames;
	}
	else if ( rollover == TC_ROLLOVER ) {
		/* Rollover if frameNumber > 23:59:59:XX */
		double frameNumber24hdouble = (((fps*60)*10) * 6 * 24);

		assert( frameNumber24hdouble > 0 && frameNumber24hdouble < INT_MAX );

		int32_t frameNumber24h = (int32_t)frameNumber24hdouble;

		frameNumber = (frameNumber24h) ? frameNumber % frameNumber24h : 0;
	}


	tc->frames  =   frameNumber % (int32_t)fps;
	tc->seconds =  (frameNumber / (int32_t)fps) % 60;
	tc->minutes = ((frameNumber / (int32_t)fps) / 60) % 60;
	tc->hours   = ((frameNumber / (int32_t)fps) / 60) / 60;
}

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
#include <stdint.h>


#define TC_SEP          ':'
#define TC_SEP_DROP     ';'


/*
 *	SMPTE ST12-1 p6 :
 *	This Standard specifies a time and control code for use in television and
 *	accompanying audio systems operating at nominal rates of 60, 59.94, 50,
 *	48, 47.95, 30, 29.97, 25, 24, and 23.98 frames per second.
 *
 *	SMPTE ST428-11:2013 p3 :
 *	This first part defines the additional frame rates individually at 25, 30,
 *	50, 60, 96, 100 and 120 frames per second.
 *
 *	NOTE about 30 DF and 60 DF (from "Pro Tools for Video, Film and Multimedia" p45) :
 *	Many manufacturers, including Digidesign, label one time code format as 30 fps
 *	drop-frame. This is not a real format. The term is commonly misused as a name
 *	for 29.97 drop-frame. If you think about it, 30 fps does not need to drop frame
 *	numbers in order to be in sync with the real-time clock.
 *	There would be no normal use for 30 drop-frame time code. In Pro Tools, the time
 *	code options include both 29.97 drop-frame and 30 drop-frame. The 30 drop-frame
 *	is a true 30fps with skipped frame numbers. In relation to the real-time clock,
 *	this format runs faster, and after one hour of real-time, reads: 01:00:03:18.
 *	This format is very misleading and should only be used when correcting errors
 *	in existing time code. Beware, and double check your time code format!
 *
 */

enum TC_FORMAT {

	TC_FORMAT_UNK = 0,

	TC_23_98,
	TC_24,
	TC_25,
	TC_29_97_NDF,
	TC_29_97_DF,
	TC_30,
	TC_47_95,
	TC_48,
	TC_50,
	TC_59_94_NDF,
	TC_59_94_DF,
	TC_60,
	TC_72,
	TC_96,
	TC_100,
	TC_120,

	TC_FORMAT_LEN
};



// typedef uint64_t rational_t;
typedef struct rational_t
{
	int32_t           numerator;
	int32_t           denominator;

} __attribute__((packed)) rational_t;

// static rational_t TC_FPS[] = {
// 	0x0000000000000001,  // UNKNWON       0/1
// 	0x00005dc0000003e9,  // TC_23_976     24000/1001
// 	0x0000001800000001,  // TC_24         24/1
// 	0x0000001900000001,  // TC_25         25/1
// 	0x00007530000003e9,  // TC_29_97_NDF  30000/1001
// 	0x00007530000003e9,  // TC_29_97_DF   30000/1001
// 	0x0000001e00000001,  // TC_30         30/1
// 	0x0000001e00000001,  // TC_30_DF      30/1
// 	0x0000003200000001,  // TC_50         50/1
// 	0x0000ea60000003e9,  // TC_59_94_NDF  60000/1001
// 	0x0000ea60000003e9,  // TC_59_94_DF   60000/1001
// 	0x0000003c00000001,  // TC_60         60/1
// 	0x0000003c00000001,  // TC_60_DF      60/1
// 	0x0000006000000001,  // TC_96         96/1
// 	0x0000006400000001,  // TC_100        100/1
// 	0x0000007800000001   // TC_120        120/1
// };

// static rational_t TC_FPS[] = {
// 	{0x00000000, 0x00000001},  // UNKNWON       0/1
// 	{0x00005dc0, 0x000003e9},  // TC_23_976     24000/1001
// 	{0x00000018, 0x00000001},  // TC_24         24/1
// 	{0x00000019, 0x00000001},  // TC_25         25/1
// 	{0x00007530, 0x000003e9},  // TC_29_97_NDF  30000/1001
// 	{0x00007530, 0x000003e9},  // TC_29_97_DF   30000/1001
// 	{0x0000001e, 0x00000001},  // TC_30         30/1
// 	{0x0000001e, 0x00000001},  // TC_30_DF      30/1
// 	{0x00000032, 0x00000001},  // TC_50         50/1
// 	{0x0000ea60, 0x000003e9},  // TC_59_94_NDF  60000/1001
// 	{0x0000ea60, 0x000003e9},  // TC_59_94_DF   60000/1001
// 	{0x0000003c, 0x00000001},  // TC_60         60/1
// 	{0x0000003c, 0x00000001},  // TC_60_DF      60/1
// 	{0x00000060, 0x00000001},  // TC_96         96/1
// 	{0x00000064, 0x00000001},  // TC_100        100/1
// 	{0x00000078, 0x00000001}   // TC_120        120/1
// };


#define inttorational( n, d ) \
	{n, d}
	// ( ((uint64_t)n << 32) | (d & 0xffffffff) )
/*
#define rationaltofloat( n ) \
	(float)(( (n & 0xffffffff) == 0 ) ? 0 : ( (float)(n >> 32) / (n & 0xffffffff) ) )
*/

#ifndef rationalToFloat
#define rationalToFloat( r ) \
	(( r.denominator == 0 ) ? 0 : ((float)r.numerator/r.denominator))
#endif

struct timecode
{
	uint64_t   unitValue;	// some value of unknwon unit (eg. samples)

	rational_t unitRate;	// value units per second (eg. 48000)



	int32_t    frameNumber;


	uint16_t   hours;

	uint16_t   minutes;

	uint16_t   seconds;

	uint16_t   frames;


	enum TC_FORMAT format;

	uint8_t    noRollover;  // disable rollover if TC is bigger than day limit

	// rational_t framePerSecond;
    //
	// uint8_t    isDropFrame;

	/**
	 *	Holds the timecode as a null terminated string.
	 */

	char       string[32];

};


int tc_add( struct timecode *tc_a, struct timecode *tc_b );

int tc_sub( struct timecode *tc_a, struct timecode *tc_b );


void tc_convert( struct timecode *tc, enum TC_FORMAT format );

void tc_convert_frames( struct timecode *tc, enum TC_FORMAT format );


enum TC_FORMAT tc_fps2format( float fps, uint8_t isDrop );


void tc_set_by_string( struct timecode *tc, const char *str, enum TC_FORMAT format );

void tc_set_by_frames( struct timecode *tc, uint32_t frameNumber, enum TC_FORMAT format );

void tc_set_by_hmsf( struct timecode *tc, uint16_t hours, uint16_t minutes, uint16_t seconds, uint16_t frames, enum TC_FORMAT format );

void tc_set_by_unitValue( struct timecode *tc, uint64_t unitValue, rational_t *unitRate, enum TC_FORMAT format );


// int oldTCbyFrames( struct timecode *tc, uint32_t frameNumber, rational_t framePerSecond, uint8_t isDropFrame );


/*
 *
 *	TODO Test on Avid Media Composer
 *	TODO Test on Final Cut Pro
 *	TODO Test on all -> xx:01:00:01
 *                   -> xx:01:00:02
 *	TODO Test 60fps
 *
 *	=============================================================
 *	29.97 fps Drop
 *	-------------------------------------------------------------
 *	23:59:59:29    - 2589407
 *
 *	    Source    |     Frames     |     Samples     |   Freq   |
 *	--------------+----------------+-----------------+----------|
 *	 Ardour       |        2589407 |      4147194251 |    48000 |
 *	 ProTools     |        2589407 |      4147194251 |    48000 |
 x	 SoundDevices |        2589409 |      4147198399 |    48000 |
 *
 *
 *	=============================================================
 *	29.97 fps NON-Drop
 *	-------------------------------------------------------------
 *	23:59:59:29    - 2591999
 *
 *	    Source    |     Frames     |     Samples     |   Freq   |
 *	--------------+----------------+-----------------+----------|
 *	 Ardour       |        2591999 |      4151345598 |    48000 |
 *	 ProTools     |              - |               - |        - |
 *	 SoundDevices |        2591999 |      4151345599 |    48000 |
 */


/**
 *	@}
 */

#endif // ! __libTC_h__

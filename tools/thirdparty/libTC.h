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

#ifndef __libTC_h__
#define __libTC_h__


#include <stdio.h>
#include <stdint.h>

#if defined(__linux__)
	#include <limits.h>
	#include <linux/limits.h>
#elif defined(__APPLE__)
	#include <sys/syslimits.h>
#elif defined(_WIN32)
	#include <windows.h> // MAX_PATH
	#include <limits.h>
#endif


#define TC_SEP_CHAR          ':'
#define TC_SEP_DROP_CHAR     ';'
#define TC_SEP_STR           ":"
#define TC_SEP_DROP_STR      ";"


/*
 * SMPTE ST12-1 p6 :
 * This Standard specifies a time and control code for use in television and
 * accompanying audio systems operating at nominal rates of 60, 59.94, 50,
 * 48, 47.95, 30, 29.97, 25, 24, and 23.98 frames per second.
 *
 * SMPTE ST428-11:2013 p3 :
 * This first part defines the additional frame rates individually at 25, 30,
 * 50, 60, 96, 100 and 120 frames per second.
 *
 * NOTE about 30 DF and 60 DF (from "Pro Tools for Video, Film and Multimedia" p45) :
 * Many manufacturers, including Digidesign, label one time code format as 30 fps
 * drop-frame. This is not a real format. The term is commonly misused as a name
 * for 29.97 drop-frame. If you think about it, 30 fps does not need to drop frame
 * numbers in order to be in sync with the real-time clock.
 * There would be no normal use for 30 drop-frame time code. In Pro Tools, the time
 * code options include both 29.97 drop-frame and 30 drop-frame. The 30 drop-frame
 * is a true 30fps with skipped frame numbers. In relation to the real-time clock,
 * this format runs faster, and after one hour of real-time, reads: 01:00:03:18.
 * This format is very misleading and should only be used when correcting errors
 * in existing time code. Beware, and double check your time code format!
 *
 */

enum TC_FORMAT {

	TC_FORMAT_UNK = 0,

	TC_23_98,
	TC_24,
	TC_25,
	TC_29_97_NDF,
	TC_29_97_DF,
	TC_30_NDF,
	TC_30_DF,
	TC_47_95,
	TC_48,
	TC_50,
	TC_59_94_NDF,
	TC_59_94_DF,
	TC_60_NDF,
	TC_60_DF,
	TC_72,
	TC_96,
	TC_100,
	TC_119_88_NDF,
	TC_119_88_DF,
	TC_120_NDF,
	TC_120_DF,

	TC_FORMAT_LEN
};

extern const char *TC_FORMAT_STR[];

enum TC_ROLLOVER_OPT {
	TC_ROLLOVER = 0,
	TC_NO_ROLLOVER
};

typedef struct rational_t {
	int32_t           numerator;
	int32_t           denominator;
} rational_t;


struct timecode {

	enum TC_FORMAT format;

	int64_t        unitValue;	// some value of unknown unit (eg. samples)
	rational_t     unitRate;	// value units per second (eg. 48000/1)

	int32_t        frameNumber;

	int32_t        hours;
	int32_t        minutes;
	int32_t        seconds;
	int32_t        frames;

	char           string[32];
};


enum TC_FORMAT tc_get_format_by_fps( float fps, uint8_t isDrop );

void tc_set_by_unitValue( struct timecode *tc, int64_t unitValue, rational_t *unitRate, enum TC_FORMAT format, enum TC_ROLLOVER_OPT rollover );
void tc_set_by_frames( struct timecode *tc, int32_t frameNumber, enum TC_FORMAT format, enum TC_ROLLOVER_OPT rollover );
void tc_set_by_hmsf( struct timecode *tc, int hours, int minutes, int seconds, int frames, enum TC_FORMAT format, enum TC_ROLLOVER_OPT rollover );
void tc_set_by_string( struct timecode *tc, const char *str, enum TC_FORMAT format, enum TC_ROLLOVER_OPT rollover );

void tc_set_format( struct timecode *tc, enum TC_FORMAT format, enum TC_ROLLOVER_OPT rollover );
void tc_convert( struct timecode *tc, enum TC_FORMAT format, enum TC_ROLLOVER_OPT rollover );

int tc_add( struct timecode *tc_a, struct timecode *tc_b, enum TC_ROLLOVER_OPT rollover );
int tc_sub( struct timecode *tc_a, struct timecode *tc_b, enum TC_ROLLOVER_OPT rollover );


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

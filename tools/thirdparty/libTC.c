
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // abs()
#include <math.h>	// round()

#include "libTC.h"



/*
 *	SMPTE ST12-1 p9 :
 *	Sufficient precision is necessary in calculations to assure that rounding or
 *	truncation operations will not create errors in the end result.
 */

static rational_t TC_FPS[] = {
	{0x00000000, 0x00000001},  // UNKNWON       0/1
	{0x00005dc0, 0x000003e9},  // TC_23_976     24000/1001
	{0x00000018, 0x00000001},  // TC_24         24/1
	{0x00000019, 0x00000001},  // TC_25         25/1
	{0x00007530, 0x000003e9},  // TC_29_97_NDF  30000/1001
	{0x00007530, 0x000003e9},  // TC_29_97_DF   30000/1001
	{0x0000001e, 0x00000001},  // TC_30         30/1
	{0x0000bb80, 0x000003e9},  // TC_47_95      48000/1001
	{0x00000030, 0x00000001},  // TC_48         48/1
	{0x00000032, 0x00000001},  // TC_50         50/1
	{0x0000ea60, 0x000003e9},  // TC_59_94_NDF  60000/1001
	{0x0000ea60, 0x000003e9},  // TC_59_94_DF   60000/1001
	{0x0000003c, 0x00000001},  // TC_60         60/1
	{0x00000048, 0x00000001},  // TC_72         72/1
	{0x00000060, 0x00000001},  // TC_96         96/1
	{0x00000064, 0x00000001},  // TC_100        100/1
	{0x00000078, 0x00000001}   // TC_120        120/1
};

/*
static char *TC_FORMAT_STR[] = {
	"unknown",
	"TC_23_976",
	"TC_24",
	"TC_25",
	"TC_29_97_NDF",
	"TC_29_97_DF",
	"TC_30",
	"TC_47_95",
	"TC_48",
	"TC_50",
	"TC_59_94_NDF",
	"TC_59_94_DF",
	"TC_60",
	"TC_72",
	"TC_96",
	"TC_100",
	"TC_120"
};
*/




static void unitValueToFrames( struct timecode *tc )
{

	float fps      = aafRationalToFloat( TC_FPS[tc->format] );
	float unitRate = aafRationalToFloat( tc->unitRate       );

	tc->frameNumber = (uint32_t)( tc->unitValue / ( unitRate / fps ) );

}




static void hmsfToString( struct timecode *tc )
{
	char string[16];

	snprintf( string,  16, "%02u%c%02u%c%02u%c%02u",
	          (tc->hours   <= 9999) ? tc->hours   : 0, TC_SEP,
	          (tc->minutes <=   99) ? tc->minutes : 0, TC_SEP,
	          (tc->seconds <=   99) ? tc->seconds : 0, (tc->format == TC_29_97_DF || tc->format == TC_59_94_DF) ? TC_SEP_DROP : TC_SEP,
	          (tc->frames  <=  999) ? tc->frames  : 0 );

	if ( tc->frameNumber < 0 )
	{
		tc->string[0] = '-';
		strcpy( tc->string+1, string );
	}
	else
	{
		strcpy( tc->string, string );
	}
}




static void StringToHmsf( struct timecode *tc )
{

	sscanf( tc->string, "%hu%*c%hu%*c%hu%*c%hu",
	       &tc->hours,
	       &tc->minutes,
	       &tc->seconds,
	       &tc->frames  );

}




static void hmsfToFrames( struct timecode *tc )
{

	float fps = round(aafRationalToFloat( TC_FPS[tc->format] ));

	uint32_t dropFrames = 0;

	if ( tc->format == TC_29_97_DF ||
		 tc->format == TC_59_94_DF )
	{
		dropFrames =   (tc->hours   * (2 * 9 * 6))    + \
		              ((tc->minutes / 10)  * (2 * 9)) + \
					 (((tc->minutes % 10)) *  2);
					  // - ((tc->minutes % 10)  ?  2 : 0 ); // we don't take out the first two frame numbers (00 and 01) of the current 10min block because we start the counting at 0..
	}


	tc->frameNumber = ( tc->hours * 3600 * fps ) + \
					  ( tc->minutes * 60 * fps ) + \
					  ( tc->seconds * fps )      + \
					  ( tc->frames )             - \
					    dropFrames;

}




static void framesToHmsf( struct timecode *tc )
{

	int32_t frameNumber = abs(tc->frameNumber);

	float fps = round(aafRationalToFloat( TC_FPS[tc->format] ));


	if ( tc->format == TC_29_97_DF ||
		 tc->format == TC_59_94_DF )
	{

		/*
		 *	SMPTE ST 12-1 p10 :
		 *	To minimize the NTSC time deviation from real time, the first two
		 *	frame numbers (00 and 01) shall be omitted from the count at the
		 *	start of each minute except minutes 00, 10, 20, 30, 40, and 50.
		 */


		/*
		 *	Rollover if frameNumber > 23:59:59:29
		 */

		if ( tc->noRollover == 0 )
		{
			uint32_t frameNumber24h = (((((fps*60))*10) - (2*9)) * 6 * 24); // 2589408 frames @ 29.97fps

			frameNumber = frameNumber % frameNumber24h;
		}


		int32_t framesPerMinute    = (fps * 60) - 2;
		int32_t framesPer10Minutes = (framesPerMinute * 10) + 2;

		int32_t chunksOf10Minutes  = frameNumber / framesPer10Minutes;
		int32_t chunksOf1Minute    = ((int32_t)(frameNumber % framesPer10Minutes) - 2) / framesPerMinute; // minus 2 correspond to the first two frame numbers (00 and 01)

		int32_t tenMinuteDrops     = 2 * 9 * chunksOf10Minutes;
		int32_t oneMinuteDrops     = ( chunksOf1Minute < 0 ) ? 0 : (2 * chunksOf1Minute);


		uint32_t dropFrames = tenMinuteDrops + oneMinuteDrops;


		if ( tc->format == TC_59_94_DF )
		{
			dropFrames *= 2;
		}


		// printf(" frameNumber        : %u\n", tc->frameNumber );
		// printf(" frame per second   : %02.2f\n", fps );
		// printf(" framesPerMinute    : %u\n", framesPerMinute );
		// printf(" framesPer10Minutes : %u\n", framesPer10Minutes );
		// printf(" chunksOf1Minute    : %i\n", chunksOf1Minute );
		// printf(" chunksOf10Minutes  : %u\n", chunksOf10Minutes );
		// printf(" oneMinuteDrops     : %u\n", oneMinuteDrops );
		// printf(" tenMinuteDrops     : %u\n", tenMinuteDrops );
		// printf(" dropFrames         : %u\n", dropFrames );


		frameNumber += dropFrames;

	}
	else if ( tc->noRollover == 0 )
	{

		/*
		 *	Rollover if frameNumber > 23:59:59:29
		 */

		uint32_t frameNumber24h = (((fps*60)*10) * 6 * 24);

		frameNumber = frameNumber % frameNumber24h;

	}


	tc->frames  =   frameNumber % (uint32_t)fps;
	tc->seconds =  (frameNumber / (uint32_t)fps) % 60;
	tc->minutes = ((frameNumber / (uint32_t)fps) / 60) % 60;
	tc->hours   = ((frameNumber / (uint32_t)fps) / 60) / 60;

}




int tc_add( struct timecode *tc_a, struct timecode *tc_b )
{
	if ( tc_a->format != tc_b->format )
	{
		return -1;
	}

	tc_a->frameNumber += tc_b->frameNumber;

	framesToHmsf( tc_a );

	hmsfToString( tc_a );

	return 0;
}




int tc_sub( struct timecode *tc_a, struct timecode *tc_b )
{
	if ( tc_a->format != tc_b->format )
	{
		return -1;
	}

	tc_a->frameNumber -= tc_b->frameNumber;

	framesToHmsf( tc_a );

	hmsfToString( tc_a );

	return 0;
}




void tc_convert( struct timecode *tc, enum TC_FORMAT format )
{
	tc->format  = format;

	framesToHmsf( tc );
	hmsfToString( tc );
}




void tc_convert_frames( struct timecode *tc, enum TC_FORMAT format )
{
	tc->format  = format;

	if ( tc->frames > round(aafRationalToFloat(TC_FPS[format])) )
	{
		tc->frames = round(aafRationalToFloat(TC_FPS[format])) - 1;
	}

	hmsfToFrames( tc );
	framesToHmsf( tc );
	hmsfToString( tc );
}




enum TC_FORMAT tc_fps2format( float fps, uint8_t isDrop )
{
	/* TODO is function ok ? */

	uint32_t ifps = (uint32_t)( fps * 100 );

	// printf("%i\n", ifps );

	uint16_t tc_format = 0;

	for ( tc_format = 0; tc_format < TC_FORMAT_LEN; tc_format++ )
	{

		if ( ifps == (uint32_t)(aafRationalToFloat( TC_FPS[tc_format] ) * 100) )
		{
			if ( ( tc_format == TC_29_97_NDF ||
				   tc_format == TC_59_94_NDF ) &&
			      isDrop == 1 )
			{
				return tc_format++;
			}

			return tc_format;
		}
	}

	return TC_FORMAT_UNK;
}




void tc_set_by_string( struct timecode *tc, const char *str, enum TC_FORMAT format )
{

	tc->format = format;

	tc->noRollover = 0;

	strncpy( tc->string, str, 12 );


	StringToHmsf( tc );

	hmsfToFrames( tc );

}




void tc_set_by_frames( struct timecode *tc, uint32_t frameNumber, enum TC_FORMAT format )
{

	/* TODO test or set to zero */

	tc->unitValue = frameNumber;
	memcpy( &tc->unitRate, &TC_FPS[format], sizeof(rational_t) );

	tc->frameNumber = frameNumber;
	tc->format = format;

	tc->noRollover = 0;


	framesToHmsf( tc );

	hmsfToString( tc );

}




void tc_set_by_hmsf( struct timecode *tc, uint16_t hours, uint16_t minutes, uint16_t seconds, uint16_t frames, enum TC_FORMAT format )
{

	tc->hours   = hours;
	tc->minutes = minutes;
	tc->seconds = seconds;
	tc->frames  = frames;

	tc->format  = format;

	tc->noRollover = 0;


	hmsfToFrames( tc );

	hmsfToString( tc );

}




void tc_set_by_unitValue( struct timecode *tc, uint64_t unitValue, rational_t *unitRate, enum TC_FORMAT format )
{

	tc->unitValue = unitValue;
	tc->unitRate  = *unitRate;

	tc->format = format;

	tc->noRollover = 0;


	unitValueToFrames( tc );

	framesToHmsf( tc );

	hmsfToString( tc );

}

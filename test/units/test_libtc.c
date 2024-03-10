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

#include "../../tools/thirdparty/libTC.h"
#include "common.h"


static int test_tc( int line, int32_t frameNumber, const char *tcstr, uint16_t h, uint16_t m, uint16_t s, uint16_t f, enum TC_ROLLOVER_OPT rollover, enum TC_FORMAT tcFormat ) {

	int rc = 0;
	struct timecode tc;


	tc_set_by_frames( &tc, frameNumber, tcFormat, rollover );

	if ( tc.hours   != h ||
	     tc.minutes != m ||
	     tc.seconds != s ||
	     tc.frames  != f )
	{
		TEST_LOG( "tc_set_by_frames() @ %s error : %i %i %i %i != %i %i %i %i\n", TC_FORMAT_STR[tcFormat], tc.hours, tc.minutes, tc.seconds, tc.frames, h, m, s, f );
		rc++;
	}
	if ( strcmp(tc.string, tcstr) != 0 ) {
		TEST_LOG( "tc_set_by_frames() @ %s error : %s != %s\n", TC_FORMAT_STR[tcFormat], tc.string, tcstr );
		rc++;
	}


	tc_set_by_hmsf( &tc, h, m, s, f, tcFormat, rollover );

	if ( tc.frameNumber != frameNumber ) {
		TEST_LOG( "tc_set_by_hmsf() @ %s error : %i != %i\n", TC_FORMAT_STR[tcFormat], tc.frameNumber, frameNumber );
		rc++;
	}
	if ( strcmp(tc.string, tcstr) != 0 ) {
		TEST_LOG( "tc_set_by_hmsf() @ %s error : %s != %s\n", TC_FORMAT_STR[tcFormat], tc.string, tcstr );
		rc++;
	}


	tc_set_by_string( &tc, tcstr, tcFormat, rollover );

	if ( tc.frameNumber != frameNumber ) {
		TEST_LOG( "tc_set_by_string() @ %s error : %i != %i\n", TC_FORMAT_STR[tcFormat], tc.frameNumber, frameNumber );
		rc++;
	}
	if ( tc.hours   != h ||
	     tc.minutes != m ||
	     tc.seconds != s ||
	     tc.frames  != f )
	{
		TEST_LOG( "tc_set_by_string() @ %s error : %i %i %i %i != %i %i %i %i\n", TC_FORMAT_STR[tcFormat], tc.hours, tc.minutes, tc.seconds, tc.frames, h, m, s, f );
		rc++;
	}


	TEST_LOG( TEST_PASSED_STR "%s\n", line, TC_FORMAT_STR[tcFormat] );

	return rc;
}



int main( int argc, char *argv[] ) {

	(void)argc;
	(void)argv;

#ifdef _WIN32
	INIT_WINDOWS_CONSOLE()
#endif

	SET_LOCALE()

	int errors = 0;

	TEST_LOG("\n");

	errors += test_tc( __LINE__, 86640,  "01:00:10:00", 01,00,10,00, TC_NO_ROLLOVER, TC_23_98      );
	errors += test_tc( __LINE__, 86640,  "01:00:10:00", 01,00,10,00, TC_NO_ROLLOVER, TC_24         );
	errors += test_tc( __LINE__, 90250,  "01:00:10:00", 01,00,10,00, TC_NO_ROLLOVER, TC_25         );
	errors += test_tc( __LINE__, 108300, "01:00:10:00", 01,00,10,00, TC_NO_ROLLOVER, TC_29_97_NDF  );
	errors += test_tc( __LINE__, 108192, "01:00:10;00", 01,00,10,00, TC_NO_ROLLOVER, TC_29_97_DF   );
	errors += test_tc( __LINE__, 108300, "01:00:10:00", 01,00,10,00, TC_NO_ROLLOVER, TC_30_NDF     );
	errors += test_tc( __LINE__, 108192, "01:00:10;00", 01,00,10,00, TC_NO_ROLLOVER, TC_30_DF      );
	// errors += test_tc( __LINE__, xxxxxx, "01:00:10:00", 01,00,10,00, TC_NO_ROLLOVER, TC_47_95      );
	// errors += test_tc( __LINE__, xxxxxx, "01:00:10:00", 01,00,10,00, TC_NO_ROLLOVER, TC_48         );
	errors += test_tc( __LINE__, 180500, "01:00:10:00", 01,00,10,00, TC_NO_ROLLOVER, TC_50         );
	errors += test_tc( __LINE__, 216600, "01:00:10:00", 01,00,10,00, TC_NO_ROLLOVER, TC_59_94_NDF  );
	errors += test_tc( __LINE__, 216384, "01:00:10;00", 01,00,10,00, TC_NO_ROLLOVER, TC_59_94_DF   );
	errors += test_tc( __LINE__, 216600, "01:00:10:00", 01,00,10,00, TC_NO_ROLLOVER, TC_60_NDF     );
	errors += test_tc( __LINE__, 216384, "01:00:10;00", 01,00,10,00, TC_NO_ROLLOVER, TC_60_DF      );
	// errors += test_tc( __LINE__, xxxxxx, "01:00:10:00", 01,00,10,00, TC_NO_ROLLOVER, TC_72         );
	// errors += test_tc( __LINE__, xxxxxx, "01:00:10:00", 01,00,10,00, TC_NO_ROLLOVER, TC_96         );
	errors += test_tc( __LINE__, 361000, "01:00:10:00", 01,00,10,00, TC_NO_ROLLOVER, TC_100        );
	errors += test_tc( __LINE__, 433200, "01:00:10:00", 01,00,10,00, TC_NO_ROLLOVER, TC_119_88_NDF );
	errors += test_tc( __LINE__, 432768, "01:00:10;00", 01,00,10,00, TC_NO_ROLLOVER, TC_119_88_DF  );
	errors += test_tc( __LINE__, 433200, "01:00:10:00", 01,00,10,00, TC_NO_ROLLOVER, TC_120_NDF    );
	errors += test_tc( __LINE__, 432768, "01:00:10;00", 01,00,10,00, TC_NO_ROLLOVER, TC_120_DF     );

	// errors += test_tc( __LINE__, 107892*25, "01:00:00;00", 01,00,00,00, TC_ROLLOVER, TC_29_97_DF );

	// errors += test_tc( __LINE__, 107892*25, "25:00:00;00", 25,00,00,00, TC_NO_ROLLOVER, TC_29_97_DF );
	// errors += test_tc( __LINE__, 300, "00:00:10;00", 00,00,10,00, TC_ROLLOVER, TC_29_97_DF );

	TEST_LOG("\n");

	return errors;
}

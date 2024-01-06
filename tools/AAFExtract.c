/*
 * Copyright (C) 2017-2023 Adrien Gesta-Fline
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
#include <locale.h>

#ifdef _MSC_VER
	#include "win/getopt.h"
#else
	#include <getopt.h>
#endif

#include <libaaf.h>
#include <libaaf/utils.h> // ANSI colors, laaf_util_c99strdup()

#include "common.h"


static void showHelp( void ) {
	printf(
		"Usage: AAFExtract [options] --output-path <path> [AAFFILE]\n"
		"\n"
		"  --output-path <path>  Where audio files will be extracted.\n"
		"  --no-nonlatin         If internal audio filename contains non-latin characters,\n"
		"                        extracted file will be named out of its UUID.\n"
		"\n"
		"  --trace               Prints AAF class/object tree.\n"
		"\n"
		"  --no-color            Disable ANSI colors in output.\n"
		"  --verb         <num>  0=quiet 1=error 2=warning 3=debug.\n"

		"\n\n"
	);
}

int main( int argc, char *argv[] )
{
	setlocale( LC_ALL, "" );

	int rc = 0;
	int no_nonlatin = 0;
	char *output_path = NULL;
	int ansicolor = 1;
	AAF_Iface *aafi = NULL;
	int trace = 0;
	enum verbosityLevel_e verb = VERB_WARNING;


	const char* optstring = "hontc:v:";

	const struct option longopts[] = {

		{ "output-path",     required_argument, 0, 'o' },
		{ "no-nonlatin",     no_argument,       0, 'n' },

		{ "trace",           no_argument,       0, 't' },

		{ "no-color",        no_argument,       0, 'c' },
		{ "verb",            required_argument, 0, 'v' },

		{ "help",            no_argument,       0, 'h' },
	};

	int c = 0;
	int optindex = 0;

	while ( EOF != (c = getopt_long( argc, argv, optstring, longopts, &optindex )) ) {

		switch (c) {

			case 'o': output_path = laaf_util_c99strdup(optarg);  break;
			case 'n': no_nonlatin = 1;                            break;

			case 't': trace = 1;                                  break;

			case 'c': ansicolor = 0;                              break;
			case 'v': verb = atoi(optarg);                        break;

			case 'h': showHelp();                                 goto end;

			default:
				printf( "Try 'AAFExtract --help' for more informations.\n" );
				return 1;
		}
	}

	if ( verb )
		printf( "\nAAFExtract\nlibAAF %s\n\n", LIBAAF_VERSION );


	if ( optind == argc ) {

		if ( verb )
			fprintf( stderr,
				"Command line error: missing AAF file\n"
				"Try 'AAFExtract --help' for more informations.\n" );

		goto err;
	}

	if ( output_path == NULL ) {

		if ( verb )
			fprintf( stderr,
				"Command line error: missing --output-path\n"
				"Try 'AAFExtract --help' for more informations.\n" );

		goto err;
	}


	aafi = aafi_alloc( NULL );

	if ( !aafi ) {
		fprintf( stderr, "Failed to init AAF_Iface context.\n" );
		goto err;
	}

	aafi_set_debug( aafi, verb, ansicolor, stdout, NULL, NULL );

	aafi_set_option_int( aafi, "trace",                     trace       );
	aafi_set_option_int( aafi, "forbid_nonlatin_filenames", no_nonlatin );

	enable_windows_VT100_output();


	if ( aafi_load_file( aafi, argv[argc-1] ) ) {
		goto err;
	}


	aafiAudioEssence *audioEssence = NULL;

	foreachEssence( audioEssence, aafi->Audio->Essences ) {

		if ( audioEssence->is_embedded ) {
			if ( aafi_extract_audio_essence( aafi, audioEssence, output_path, NULL ) == 0 ) {
				if ( verb )
					printf( " Audio file extracted to %ls\n", audioEssence->usable_file_path );
			} else {
				/* error was already printed by library */
			}
		}
	}

	if ( verb )
		printf( "\nCompleted.\n\n" );

	goto end;

err:
	rc = -1;

end:

	if ( aafi )
		aafi_release( &aafi );

	if ( output_path )
		free( output_path );

	return rc;
}

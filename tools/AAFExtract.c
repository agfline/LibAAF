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

#include "../src/common/utils.h" // ANSI colors, c99strdup()


static void showHelp( void ) {
	printf(
		"\n"
		" CFB Analysis :\n"
		"\n"
		"   --cfb-header                   Displays the CFB Header.\n"
		"   --cfb-fat                      Displays the CFB FAT.\n"
		"   --cfb-minifat                  Displays the CFB MiniFAT.\n"
		"   --cfb-difat                    Displays the CFB DiFAT.\n"
		"   --cfb-nodes                    Displays the CFB node Tree.\n"
		"\n"
		"   --get-node             <path>  Retrieves and displays the node located at the given <path>.\n"
		"\n"
		"\n"
		" AAF Analysis :\n"
		"\n"
		"   --aaf-summary                  Displays AAF informations from both header and identification objects.\n"
		"   --aaf-essences                 Lists AAF essences.\n"
		"   --aaf-clips                    Lists AAF clips.\n"
		"   --aaf-classes                  Lists AAF Classes. The ones from MetaDictionary are shown in yellow.\n"
		"   --aaf-meta                     Lists Classes and Properties from the MetaDictionary.\n"
		"   --aaf-properties               Displays all Properties.\n"
		"\n"
		"   --trace                        Dumps trace of AAF parsing.\n"
		"   --trace-meta                   Prints MetaProperties in the trace.\n"
		"   --trace-class    <AAFClassID>  Prints properties of a specific AAFClass in the trace.\n"
		"\n"
		"\n"
		" Options :\n"
		"\n"
		"   --media-location       <path>  Where to find external essence files.\n"
		"\n"
		"   --pos-format <tc|hms|samples>  Position and length display format.\n"
		"   --show-automation              Shows track and clip automation values.\n"
		"\n"
		"   --verbose               <num>  0=quiet 1=error 2=warning 3=debug.\n"
		"\n\n"
	);
}

int main( int argc, char *argv[] )
{
	setlocale( LC_ALL, "" );

	int rc = 0;
	int no_nonlatin = 0;
	char *output_path = NULL;
	char *aaf_file = NULL;

	printf( "\nAAFExtract\nlibAAF %s\n\n", LIBAAF_VERSION );


	const char* optstring = "hnp:f:";

	const struct option longopts[] = {

		{ "help",            no_argument,       0, 'h' },

		{ "output-path",     required_argument, 0, 'p' },
		{ "no-nonlatin",     required_argument, 0, 'n' },
		{ "file",            required_argument, 0, 'f' },
	};

	int c = 0;

	while ( EOF != (c = getopt_long( argc, argv, optstring, longopts, (int*)0 )) ) {

		switch (c) {

			case 'h':
				showHelp();
				break;

			case 'p':
				output_path = c99strdup(optarg);
				break;

			case 'n':
				no_nonlatin = 1;
				break;

			case 'f':
				aaf_file = c99strdup(optarg);
				break;

			default:
				printf( "Error: unrecognized option. See --help for usage information.\n" );
				return 1;
		}
	}


	AAF_Iface *aafi = aafi_alloc( NULL );

	aafi_set_debug( aafi, VERB_DEBUG, stdout, NULL, NULL );

	aafi->ctx.options.verb = VERB_DEBUG;
	aafi->ctx.options.trace = 1;
	aafi->ctx.options.forbid_nonlatin_filenames = no_nonlatin;

	if ( aafi_load_file( aafi, aaf_file ) ) {
		goto err;
	}


	aafiAudioEssence *audioEssence = NULL;

	foreachEssence( audioEssence, aafi->Audio->Essences ) {

		if ( audioEssence->is_embedded ) {
			if ( aafi_extract_audio_essence( aafi, audioEssence, output_path, NULL ) == 0 ) {
				printf( "Media file extracted @ %ls\n", audioEssence->usable_file_path );
			}
		}
	}


	goto end;

err:
	rc = -1;

end:

	aafi_release( &aafi );

	if ( output_path )
		free( output_path );

	if ( aaf_file )
		free( aaf_file );

	return rc;
}

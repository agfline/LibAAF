#include <stdio.h>

#ifdef _MSC_VER
#include <libaaf/win/getopt.h>
#else
#include <getopt.h>
#endif

#include <libaaf.h>


void usage() {

}

int main( int argc, char *argv[] )
{
	int rc = 0;

	char *output_path = NULL;
	char *aaf_file = NULL;

	const char* optstring = "hp:f:";

	const struct option longopts[] = {

    { "help",            no_argument,       0, 'h' },

    { "output-path",     required_argument, 0, 'p' },
    { "file",            required_argument, 0, 'f' },
	};

	int c = 0;

	while ( EOF != (c = getopt_long( argc, argv, optstring, longopts, (int*)0 )) ) {

		switch (c) {

      case 'h':
        usage();
        break;

      case 'p':
        output_path = strdup(optarg);
        break;

      case 'f':
				aaf_file = strdup(optarg);
        break;

      default:
				printf( "Error: unrecognized option. See --help for usage information.\n" );
				return 1;
		}
	}


	AAF_Iface *aafi = aafi_alloc( NULL );

	aafi->ctx.options.verb = VERB_DEBUG;
	aafi->ctx.options.trace = 1;

	if ( aafi_load_file( aafi, aaf_file ) ) {
		goto err;
	}


	aafiAudioEssence *audioEssence = NULL;

	foreachEssence( audioEssence, aafi->Audio->Essences ) {

		if ( audioEssence->is_embedded ) {
			if ( aafi_extract_audio_essence( aafi, audioEssence, output_path, NULL, 0 ) == 0 ) {
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

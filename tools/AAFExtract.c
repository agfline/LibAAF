#include <stdio.h>

#include "../LibAAF/libAAF.h"




int main( int argc, char *argv[] )
{
	if ( argc < 2 )
	{
		return 1;
	}


	AAF_Iface *aafi = aafi_alloc( NULL );

	if ( aafi_load_file( aafi, argv[argc-1] ) )
	{
		return 1;
	}



	/* This set path to absolute path to AAFExtract */

	// char path[1024];
	// memset(path, 0x00, 1024);
    //
	// readlink( "/proc/self/exe", path, 1024 );
    //
	// path[strlen(path)-10] = 0x00;



	char *path = "/tmp";

	int format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;



	aafiAudioEssence *audioEssence = NULL;

	foreachAudioEssence( audioEssence, aafi->Audio->Essences )
	{
		if ( audioEssence->is_embedded )
		{
			aafi_extract_audio_essence( aafi, audioEssence, path, NULL, format );
			printf( ":: %ls\n", audioEssence->exported_file_path );
		}
	}



	aafi_release( &aafi );

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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


	// retrieveEssences( aafi );

	// retrieveClips( aafi );



	char progPath[1024];
	memset(progPath, 0x00, 1024);

	readlink( "/proc/self/exe", progPath, 1024 );

	progPath[strlen(progPath)-10] = 0x00;



	aafiAudioEssence *audioEssence = NULL;
	char *filename = NULL;
	char  target[255];
	uint32_t i = 0;

	foreachAudioEssence( audioEssence, aafi->Audio->Essences )
	{
		aafi_get_essence_filename( audioEssence, &filename, aafi->compositionName, &i );

		snprintf( target, 255, "%s%s", progPath, filename );

		extractAudioEssence( aafi, audioEssence, target );
	}

	if ( filename != NULL )
	{
		free( filename );
	}


	aafi_release( &aafi );

	return 0;
}

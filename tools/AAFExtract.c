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

	if ( aaf_load_file( aafi->aafd, argv[argc-1] ) )
	{
		return 1;
	}


	retrieveEssences( aafi );

	retrieveClips( aafi );


	aafiAudioEssence *ae = NULL;

	uint32_t i = 0;

	foreachAudioEssence( ae, aafi->Audio->Essences )
	{
		char file[255];

		snprintf( file, 255, "./%s_%d", aafi->compositionName, i++ );

		extractAudioEssence( aafi, ae, file );
	}

	aaf_release( &(aafi->aafd) );

	return 0;
}

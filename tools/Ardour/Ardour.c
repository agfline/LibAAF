
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "Ardour.h"

int writeArdourSessionFile( char *aux/*AAF_Data *aafd, AAFI_AudioEssence *aafiae, const char *file*/ )
{

	char progPath[1024];
	memset( progPath, 0x00, 1024 );

	readlink( "/proc/self/exe", progPath, 1024 );

	progPath[strlen(progPath)-10] = 0x00;

	char file[1024];
	memset( file, 0x00, 1024 );

	snprintf( file, 1024, "%sArdour/session/Ardour.ardour", progPath );


	FILE *fp = fopen( file, "w+" );


	char *buf = malloc( 400000 );
	memset( buf, 0x00, 400000 );

	snprintf( buf, 400000, ARDOUR_SESSION_FILE_CONTENT, aux );

	size_t len = strlen( buf );

	fwrite( buf, sizeof(unsigned char), len, fp );

	free( buf );

	fclose( fp );

	return 0;
}




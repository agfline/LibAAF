#ifndef __AAFIAudioFiles_h__
#define __AAFIAudioFiles_h__

#include <sndfile.h>

#include "../libAAF.h"


#define BUFFER_LEN 4096


typedef struct
{
    sf_count_t      offset;
    sf_count_t      length;
	unsigned char  *data;
} VIO_DATA ;


int parse_audio_summary( AAF_Iface *aafi, aafiAudioEssence *audioEssence );

int aafi_extract_audio_essence( AAF_Iface *aafi, aafiAudioEssence *audioEssence, const char *outfilepath, const char *forcedFileName, int format );

#endif

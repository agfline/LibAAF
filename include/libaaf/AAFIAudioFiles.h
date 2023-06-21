#ifndef __AAFIAudioFiles_h__
#define __AAFIAudioFiles_h__

#include <wchar.h>

#define BUFFER_LEN 4096

typedef struct AAF_Iface AAF_Iface;
typedef struct aafiAudioEssence aafiAudioEssence;


#ifdef USE_LIBSNDFILE
  #include <sndfile.h>

  typedef struct {
    sf_count_t      offset;
    sf_count_t      length;
    unsigned char  *data;
  } VIO_DATA;
#endif

char * locate_external_essence_file( AAF_Iface *aafi, const wchar_t *original_file_path, const char *search_location );

int parse_audio_summary( AAF_Iface *aafi, aafiAudioEssence *audioEssence );

int aafi_extract_audio_essence( AAF_Iface *aafi, aafiAudioEssence *audioEssence, const char *outfilepath, const wchar_t *forcedFileName, int format );

#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h> // readlink()
#include <wchar.h>
#include <unistd.h> // access()
#include <sndfile.h>
#include <linux/limits.h> // PATH_MAX
#include <dirent.h>

#include <arpa/inet.h>
#include <ctype.h>
#include <mntent.h>

#include "../libAAF.h"
#include "../common/utils.h"
#include "../common/debug.h"

// #define SIGNED_SIZEOF(x) sizeof(x)



static sf_count_t vfget_filelen( void *user_data );
static sf_count_t vfseek( sf_count_t offset, int whence, void *user_data );
static sf_count_t vfread( void *ptr, sf_count_t count, void *user_data );
static sf_count_t vfwrite( const void *ptr, sf_count_t count, void *user_data );
static sf_count_t vftell( void *user_data );

static const char * sf_format_to_file_ext( uint32_t format );
static int32_t sf_format_to_samplesize( uint32_t format );
static int32_t samplesize_to_PCM_sf_format( uint32_t samplesize );

// static char * eascii_to_ascii( char *str );



static const char * sf_format_to_file_ext( uint32_t format )
{
    switch ( format & SF_FORMAT_TYPEMASK )
    {
        case SF_FORMAT_WAV:         return "wav";
        case SF_FORMAT_RF64:        return "wav";
        case SF_FORMAT_AIFF:        return "aiff";
        case SF_FORMAT_AU:          return "au";
        case SF_FORMAT_RAW:         return "raw";
        case SF_FORMAT_W64:         return "w64";

        default:                    break;
    }

    /* TODO move to caller */
    // _error( "Could not retrieve sample size from libsndfile format.\n" );
    return NULL;
}



static int32_t sf_format_to_samplesize( uint32_t format )
{
    switch ( format & SF_FORMAT_SUBMASK )
    {
        case SF_FORMAT_PCM_U8:
        case SF_FORMAT_PCM_S8:      return  8;
        case SF_FORMAT_PCM_16:      return 16;
        case SF_FORMAT_PCM_24:      return 24;
        case SF_FORMAT_PCM_32:      return 32;
        case SF_FORMAT_FLOAT:       return 32;
        case SF_FORMAT_DOUBLE:      return 64;

        // case SF_FORMAT_DPCM_8:      return  8;
        // case SF_FORMAT_DPCM_16:     return 16;
        //
        // case SF_FORMAT_DWVW_12:     return 12;
        // case SF_FORMAT_DWVW_16:     return 16;
        // case SF_FORMAT_DWVW_24:     return 24;
        //
        // case SF_FORMAT_ALAC_16:     return 16;
        // case SF_FORMAT_ALAC_20:     return 20;
        // case SF_FORMAT_ALAC_24:     return 24;
        // case SF_FORMAT_ALAC_32:     return 32;

        default:                    break;
    }

    /* TODO move to caller */
    // _error( "Could not retrieve sample size from libsndfile format.\n" );
    return -1;
}




static int32_t samplesize_to_PCM_sf_format( uint32_t samplesize )
{
    int32_t format = 0 | SF_FORMAT_RAW;

    switch ( samplesize )
    {
        case  8:    format |= SF_FORMAT_PCM_S8;     break;
        case 16:    format |= SF_FORMAT_PCM_16;     break;
        case 24:    format |= SF_FORMAT_PCM_24;     break;
        case 32:    format |= SF_FORMAT_PCM_32;     break;

        default:

            /* TODO move to caller */
            // _error( "Could not build libsndfile format from sample size %u.\n", samplesize );
            return -1;
    }

    return format;
}

//
// const char ASCII[] = {
//     '.', '.', '.', '.', '.', '.', '.', '.',
//     '.', '.', '.', '.', '.', '.', '.', '.',
//     '.', '.', '.', '.', '.', '.', '.', '.',
//     '.', '.', '.', '.', '.', '.', '.', '.',
//
//     ' ', '!', '"', '#', '$', '%', '&', '\'',
//     '(', ')', '*', '+', ',', '-', '.', '/',
//     '0', '1', '2', '3', '4', '5', '6', '7',
//     '8', '9', ':', ';', '<', '=', '>', '?',
//
//     '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
//     'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
//     'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
//     'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
//
//     '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
//     'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
//     'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
//     'x', 'y', 'z', '{', '|', '}', '~', '.',
//
//     '.', ' ', ',', '.', '"', '.', '.', '.',
//     '^', '%', 'S', '<', 'E', ' ', 'Z', ' ',
//     ' ', '\'', '\'', '"', '"', '.', '-', '-',
//     '~', '.', 'S', '>', 'e', ' ', 'z', 'Y',
//
//     ' ', 'i', '.', '.', '.', '.', '|', '.',
//     '.', '.', '.', '"', '.', '-', '.', '.',
//     '.', '.', '2', '3', '\'', 'u', '.', '.',
//     '.', '1', '.', '"', '.', '.', '.', '?',
//
//     'A', 'A', 'A', 'A', 'A', 'A', 'E', 'C',
//     'E', 'E', 'E', 'E', 'I', 'I', 'I', 'I',
//     '.', 'N', 'O', 'O', 'O', 'O', 'O', 'x',
//     'O', 'U', 'U', 'U', 'U', 'Y', '.', 'B',
//
//     'a', 'a', 'a', 'a', 'a', 'a', 'e', 'c',
//     'e', 'e', 'e', 'e', 'i', 'i', 'i', 'i',
//     '.', 'n', 'o', 'o', 'o', 'o', 'o', '/',
//     'o', 'u', 'u', 'u', 'u', 'y', '.', 'y'
// };
//
// char * eascii_to_ascii( char *str )
// {
//     int len = strlen( str );
//
//     int i = 0;
//
//     for ( ; i < len; i++ )
//     {
//         str[i] = ASCII[(uint8_t)str[i]];
//     }
//
//     return str;
// }





char * locate_external_essence_file( AAF_Iface *aafi, const wchar_t *original_file /*aafiAudioEssence *audioEssence*/ )
{
  /* Absolute Uniform Resource Locator (URL) complying with RFC 1738 or relative Uniform Resource Identifier (URI)
   * complying with RFC 2396 for file containing the essence. If it is a relative URI, the base URI is determined from the
   * URI of the AAF file itself.
   *
   * Informative note: A valid URL or URI uses a constrained character set and uses the / character as the path separator
   */

  /* TODO handle realpath return value and free(absFilePath) in case of error */
  char *filePath = malloc( PATH_MAX*2 );
  snprintf( filePath, PATH_MAX, "%ls", original_file /*audioEssence->original_file*/ );


  /* Try AAF essence's file path */

  if ( access( filePath, F_OK ) != -1 )
  {
      return filePath;
  }


  /*
    * AAFInfo --aaf-clips ../libaaf_testfiles/fonk_2.AAF
    file://localhost/Users/horlaprod/Music/Logic/fonk_2/Audio Files_1/fonk_2_3#04.wav

    * AAFInfo --aaf-clips ../libaaf_testfiles/ADP/ADP3_51-ST-MONO-NOBREAKOUT.aaf
    file:///C:/Users/Loviniou/Downloads/ChID-BLITS-EBU-Narration441-16b.wav

    * AAFInfo --aaf-clips ../libaaf_testfiles/ADP/ADP2_SEQ-FULL.aaf
    file://?/E:/Adrien/ADPAAF/Sequence A Rendu.mxf

    * AAFInfo --aaf-clips ../libaaf_testfiles/TEST-AVID_COMP2977052\ \ -\ \ OFF\ PODIUM\ ETAPE\ 2.aaf
    file:////C:/Users/mix_limo/Desktop/TEST2977052  -  OFF PODIUM ETAPE 2.aaf

    * AAFInfo --aaf-clips ../ardio/watchfolder/3572607_RUGBY_F_1_1.aaf
    file://10.87.230.71/mixage/DR2/Avid MediaFiles/MXF/1/3572607_RUGBY_F2_S65CFA3D0V.mxf

    * AAFInfo --aaf-clips ../libaaf_testfiles/ProTools/pt2MCC.aaf
    file:///_system/Users/horlaprod/pt2MCCzmhsFRHQgdgsTMQX.mxf
  */

  int pos = 7;
  int posmax = strlen(filePath);
  char tmp[PATH_MAX+1];

  if ( filePath[0] == 'f' &&
       filePath[1] == 'i' &&
       filePath[2] == 'l' &&
       filePath[3] == 'e' &&
       filePath[4] == ':' &&
       filePath[5] == '/' &&
       filePath[6] == '/' )
  {

    while ( pos < posmax && (filePath[pos] == '/' || filePath[pos] == '?') ) {
      /*
       * file://
       * file:///
       * file://?/
       * file:////
       */
      pos++;
    }

    // printf("%s\n",filePath+pos );

    // int ipa, ipb, ipc, ipd = 0;
    // if ( sscanf( filePath+pos-2, "//%i.%i.%i.%i/", &ipa, &ipb, &ipc, &ipd ) == 4 )

    sscanf( filePath+pos, "%[^/]", tmp );
    struct sockaddr_in sa;

    /* Remote filesystem */
    if ( inet_pton( AF_INET, tmp, &(sa.sin_addr) ) == 1 )
    {
      pos-=2; // get first "//" back

      struct mntent *m;

      // printf( "_PATH_MOUNTED macro: %s\n", _PATH_MOUNTED );

      FILE *f = setmntent( _PATH_MOUNTED, "r" );

      while ( (m = getmntent(f)) )
      {
        // printf( "Device: %s      Mount Point: %s\n", m->mnt_fsname, m->mnt_dir );

        if ( strncmp( m->mnt_fsname, filePath+pos, strlen(m->mnt_fsname) ) == 0 )
        {
          snprintf( tmp, PATH_MAX, "%s/%s", m->mnt_dir, filePath+pos+strlen(m->mnt_fsname) );
          snprintf( filePath, PATH_MAX*2, "%s", tmp );

          // printf("\n\nRemote file : %s\n\n", filePath );
          endmntent(f);
          return filePath;
        }
      }

      // printf("\n\nCould not retrieve mounted point for remote file : %s\n\n", filePath+pos );

      free( filePath );
      return NULL;

      endmntent(f);
    }
    /* local filesystem */
    else if ( pos + 9 + 1 < posmax &&
              filePath[pos+0] == 'l' &&
              filePath[pos+1] == 'o' &&
              filePath[pos+2] == 'c' &&
              filePath[pos+3] == 'a' &&
              filePath[pos+4] == 'l' &&
              filePath[pos+5] == 'h' &&
              filePath[pos+6] == 'o' &&
              filePath[pos+7] == 's' &&
              filePath[pos+8] == 't' &&
              filePath[pos+9] == '/' )
    {
      pos+=10;
    }


    /* Windows drive */
    if ( pos + 2 + 1 < posmax &&
         isalpha(filePath[pos+0]) &&
         filePath[pos+1] == ':' &&
         filePath[pos+2] == '/' )
    {
      snprintf( tmp, PATH_MAX, "%s", filePath+pos );
      snprintf( filePath, PATH_MAX*2, "%s", tmp );

      // printf("\n\nLocal Windows file : %s\n\n", filePath );

      return filePath;
    }
    /* MacOS */
    else if ( pos + 7 + 1 < posmax &&
              filePath[pos+0] == '_' &&
              filePath[pos+1] == 's' &&
              filePath[pos+2] == 'y' &&
              filePath[pos+3] == 's' &&
              filePath[pos+4] == 't' &&
              filePath[pos+5] == 'e' &&
              filePath[pos+6] == 'm' &&
              filePath[pos+7] == '/' )
    {
      pos+=7;

      snprintf( tmp, PATH_MAX, "%s", filePath+pos );
      snprintf( filePath, PATH_MAX*2, "%s", tmp );

      // printf("\n\nLocal MacOS file : %s\n\n", filePath );
    }
    /* Unix: MacOS / Linux */
    else
    {
      pos--; // -1 to get first '/'

      snprintf( tmp, PATH_MAX, "%s", filePath+pos );
      snprintf( filePath, PATH_MAX*2, "%s", tmp );

      // printf("\n\nLocal Unix file : %s\n\n", filePath );

      return filePath;
    }

    /* TODO: Try network domain name */
  }



  // char *absFilePath = malloc( PATH_MAX*2 );

  /* Prepare research */
  char *file    = strrchr( filePath, '/' ) + 1;
  char *aafFile = strrchr( aafi->aafd->cfbd->file, '/' );

  char path[PATH_MAX];

  snprintf( path, (aafFile - aafi->aafd->cfbd->file)+2, "%s", aafi->aafd->cfbd->file );

  // printf( "Path : %s\n", path );
  // printf( "File : %s\n", file );


  /* Search AAF's directory */

  struct dirent *dir;
  DIR *d = opendir( path );

  if ( d )
  {
      while ( (dir = readdir(d)) != NULL )
      {
          if ( dir->d_type != DT_REG )
          {
              continue;
          }

          if ( strcmp( dir->d_name, file ) == 0 )
          {
            /*
==5402== Source and destination overlap in mempcpy(0x13e86d53, 0x13e86d65, 31)
==5402==    at 0x483D580: mempcpy (vg_replace_strmem.c:1536)
==5402==    by 0x91A5887: _IO_default_xsputn (genops.c:386)
==5402==    by 0x91A5887: _IO_default_xsputn (genops.c:370)
==5402==    by 0x91781FA: vfprintf (vfprintf.c:1638)
==5402==    by 0x91A079F: vsnprintf (vsnprintf.c:114)
==5402==    by 0x91806BE: snprintf (snprintf.c:33)
==5402==    by 0x487A11C: locate_external_essence_file (AAFIAudioFiles.c:213)
==5402==    by 0x128AF0: main (aaf.cc:779)
==5402==
            */
              snprintf( filePath, PATH_MAX*2, "%s%s", path, file );
              // printf( "::FOUND %s\n", filePath );

              // return realpath(filePath, absFilePath);
              return filePath;
          }
      }

      closedir(d);
  }


  /* Search one level inside all directories next to AAF file */

  char subPath[PATH_MAX*2];
  struct dirent *sdir;
  DIR *sd = NULL;

  d = opendir( path );

  if ( d )
  {
      while ( (dir = readdir(d)) != NULL )
      {
          if (  dir->d_type    != DT_DIR ||
              ( dir->d_name[0] == '.' && dir->d_name[1] == '\0') ||
              ( dir->d_name[0] == '.' && dir->d_name[1] == '.' && dir->d_name[2] == '\0' ) )
          {
              continue;
          }

          snprintf( subPath, sizeof(subPath), "%s%s/", path, dir->d_name );

          sd = opendir( subPath );

          if ( sd )
          {
              while ( (sdir = readdir(sd)) != NULL )
              {
                  if ( strcmp( sdir->d_name, file ) == 0 )
                  {
                      snprintf( filePath, PATH_MAX*2, "%s%s", subPath, file );
                      // printf( "::FOUND %s\n", filePath );

                      // return realpath(filePath, absFilePath);
                      return filePath;
                  }
              }

              closedir(sd);
          }
      }

      closedir(d);
  }

  free( filePath );
  return NULL;
}




SNDFILE * open_external_file( const char *filePath, SF_INFO *sfinfo )
{
    if ( filePath == NULL )
    {
        /* TODO move to caller */
        // _error( "Could not locate external essence file.\n" );
        return NULL;
    }

    memset( sfinfo, 0x00, sizeof(SF_INFO) );

    SNDFILE *file = sf_open( filePath, SFM_READ, sfinfo );

    return file;
}




SNDFILE * open_internal_file( AAF_Iface *aafi, aafiAudioEssence *audioEssence, SF_INFO *sfinfo )
{
    uint32_t          id = 0;
    size_t     sectorLen = 0;
    aafByte_t *sectorBuf = NULL;

    SF_VIRTUAL_IO sfvirtual;
    sfvirtual.get_filelen = vfget_filelen;
    sfvirtual.seek = vfseek;
    sfvirtual.read = vfread;
    sfvirtual.write = vfwrite;
    sfvirtual.tell = vftell;

    VIO_DATA user_data;
    user_data.length = 0;
    user_data.offset = 0;

    memset( &user_data, 0x00, sizeof(VIO_DATA) );

    cfb_foreachSectorInStream( aafi->aafd->cfbd, audioEssence->node, &sectorBuf, &sectorLen, &id )
    {
        user_data.data   = sectorBuf;
        user_data.length = sectorLen;

        break;
    }

    memset( sfinfo, 0x00, sizeof(SF_INFO) );

    SNDFILE *file = sf_open_virtual( &sfvirtual, SFM_READ, sfinfo, &user_data );

    free( sectorBuf );

    return file;
}




SNDFILE * open_summary( aafiAudioEssence *audioEssence, SF_INFO *sfinfo )
{
    SF_VIRTUAL_IO sfvirtual;
    sfvirtual.get_filelen = vfget_filelen;
    sfvirtual.seek = vfseek;
    sfvirtual.read = vfread;
    sfvirtual.write = vfwrite;
    sfvirtual.tell = vftell;

    VIO_DATA user_data;
    user_data.length = 0;
    user_data.offset = 0;
    user_data.data   = audioEssence->summary->val;
    user_data.length = audioEssence->summary->len;

    memset( sfinfo, 0x00, sizeof(SF_INFO) );

    SNDFILE *file = sf_open_virtual( &sfvirtual, SFM_READ, sfinfo, &user_data );

    return file;
}




int parse_audio_summary( AAF_Iface *aafi, aafiAudioEssence *audioEssence )
{
    SNDFILE *file = NULL;

    SF_INFO sfinfo;

    char *externalFilePath = NULL;



    if ( audioEssence->summary != NULL )
    {

        file = open_summary( audioEssence, &sfinfo );

        if ( file == NULL )
        {
            /*
             *  The summary should be a copy of the header without audio samples.
             *  It has been seen (on fairlight's AIFC-AAF) that the summary does
             *  not contain the full header and stop right before the SSND chunk.
             *  In that case, libsndfile does not handle it.
             *
             *  In such cases we fallback on the first stream sector to retrieve
             *  full WAVE/AIFC header, or on external audio file if essence is
             *  not embedded.
             */

            if ( audioEssence->is_embedded )
            {
    		    _warning( aafi->ctx.options.verb, "libsndfile could not read descriptor summary : %s. Falling back on internal audio file stream.\n", sf_strerror( NULL ) );

                file = open_internal_file( aafi, audioEssence, &sfinfo );

                if ( file == NULL )
                {
                    _error( aafi->ctx.options.verb, "Could not open embedded essence file.\n" );
                    return -1;
                }
            }
            else
            {
                _warning( aafi->ctx.options.verb, "libsndfile could not read descriptor summary : %s. Falling back on external audio file stream.\n", sf_strerror( NULL ) );

                // dump_hex( audioEssence->summary->val, audioEssence->summary->len );

                externalFilePath = locate_external_essence_file( aafi, audioEssence->original_file );

                // printf("externalFilePath : %s\n", externalFilePath );

                file = open_external_file( externalFilePath, &sfinfo );

                if ( file == NULL )
                {
                    _error( aafi->ctx.options.verb, "Could not open external essence file.\n" );
                    free(externalFilePath);
                    return -1;
                }

                free(externalFilePath);
            }
        }
    }


    /* Extract usefull data */

    audioEssence->format     = sfinfo.format;
    audioEssence->channels   = sfinfo.channels;
    audioEssence->samplerate = sfinfo.samplerate;
    audioEssence->samplesize = sf_format_to_samplesize( audioEssence->format );

    if ( aafi->Audio->samplerate >= 0 )
  	{
  			aafi->Audio->samplerate = ( aafi->Audio->samplerate == 0 || aafi->Audio->samplerate == audioEssence->samplerate ) ? audioEssence->samplerate : (unsigned)-1;
  	}

    // printf("%i  :  %i\n", aafi->Audio->samplesize, audioEssence->samplesize );

    if ( aafi->Audio->samplesize >= 0 )
    {
      aafi->Audio->samplesize = ( aafi->Audio->samplesize == 0 || aafi->Audio->samplesize == audioEssence->samplesize ) ? audioEssence->samplesize : -1;
    }




    /*
     *  The length was retrieved by retrieve_EssenceData() and correspond
     *  to the full stream length, including file header. Here we need to
     *  get only audio data stream length to do proper duration calculation,
     *
     *  TODO this should be optionnal, since we might need it only when analysing files.
     */

    SF_CHUNK_INFO	    chunk_info;
    SF_CHUNK_ITERATOR  *iterator;
    int err = 0;

    memset( &chunk_info, 0x00, sizeof(SF_CHUNK_INFO) );


    char *chunk = NULL;

    if ( audioEssence->type == AAFI_TYPE_WAVE )
    {
        chunk = "data";
    }
    else if ( audioEssence->type == AAFI_TYPE_AIFC )
    {
        chunk = "SSND";
    }

    snprintf( chunk_info.id, sizeof(chunk_info.id), chunk );



    iterator = sf_get_chunk_iterator( file, &chunk_info );

    if ( iterator == NULL )
    {
        if ( audioEssence->is_embedded )
        {
            _warning( aafi->ctx.options.verb, "Could not retrieve \"%s\" chunk. Falling back on stream length calculation.\n", chunk );

            /*
             *  NOTE nothing guarentee there are no chunk after audio data, in which
             *  case the length would be wrong. In practice, this was not encountered.
             *  on embedded files.
             */

            audioEssence->length = (audioEssence->length - (1<<cfb_getStreamSectorShift( aafi->aafd->cfbd, audioEssence->node ))) +
                                   (sfinfo.frames * (audioEssence->samplesize/8));
            sf_close( file );

            return 0;
        }
        else if ( externalFilePath == NULL ) /* we have not open file yet */
        {
            _warning( aafi->ctx.options.verb, "Could not retrieve \"%s\" chunk from summary. Falling back on external essence parsing.\n", chunk );

            /* Close previous stream */
            sf_close( file );

            externalFilePath = locate_external_essence_file( aafi, audioEssence->original_file );

            file = open_external_file( externalFilePath, &sfinfo );

            if ( file == NULL )
            {
                _error( aafi->ctx.options.verb, "Could not open external essence file : %s\n", sf_strerror( NULL ) );
                free(externalFilePath);
                return -1;
            }

            free(externalFilePath);
        }
        else
        {
            _error( aafi->ctx.options.verb, "Could not retrieve \"%s\" chunk.\n", chunk );
            sf_close( file );
            return -1;
        }
    }



    err = sf_get_chunk_size( iterator, &chunk_info );

    if ( err != SF_ERR_NO_ERROR )
    {
        if ( audioEssence->is_embedded )
        {
            _warning( aafi->ctx.options.verb, "Could not retrieve \"%s\" chunk size : %s. Falling back on stream length calculation.", chunk, sf_error_number(err) );

            /*
             *  NOTE nothing guarentee there are no chunk after audio data, in which
             *  case the length would be wrong. In practice, this was not encountered.
             *  on embedded files.
             */

            audioEssence->length = (audioEssence->length - (1<<cfb_getStreamSectorShift( aafi->aafd->cfbd, audioEssence->node ))) +
                                   (sfinfo.frames * (audioEssence->samplesize/8));
            sf_close( file );
            return 0;
        }
        else
        {
            _error( aafi->ctx.options.verb, "Could not retrieve \"%s\" chunk size : %s\n", chunk, sf_error_number (err) );
            sf_close( file );
            return -1;
        }
    }



    audioEssence->length = chunk_info.datalen;


    sf_close( file );

    return 0;
}




int aafi_extract_audio_essence( AAF_Iface *aafi, aafiAudioEssence *audioEssence, const char *outfilepath, const wchar_t *forcedFileName, int format )
{
    SF_VIRTUAL_IO  sfvirtual;
    VIO_DATA       user_data;
    SF_INFO        sfinfo;
    SNDFILE       *infile = NULL;

    if ( audioEssence->is_embedded == 0 )
    {
        _error( aafi->ctx.options.verb, "Essence is not embedded : nothing to extract.\n" );
        return -1;
    }

    sfvirtual.get_filelen = vfget_filelen ;
  	sfvirtual.seek = vfseek ;
  	sfvirtual.read = vfread ;
  	sfvirtual.write = vfwrite ;
    sfvirtual.tell = vftell ;

    user_data.length = 0;
    user_data.offset = 0;

    memset( &sfinfo, 0x00, sizeof(SF_INFO) );



    /*
     *  Set parameters manually if raw PCM stream. Otherwise, they will
     *  be set by libsndfile on stream opening.
     */

    if ( audioEssence->summary == NULL )
    {
      sfinfo.format     = samplesize_to_PCM_sf_format( audioEssence->samplesize );
	    sfinfo.channels   = audioEssence->channels;
      sfinfo.samplerate = audioEssence->samplerate;
    }



    /* Retrieve stream from CFB */

    cfb_getStream( aafi->aafd->cfbd, audioEssence->node, (unsigned char**)(&user_data.data), (uint64_t*)(&user_data.length) );

    if ( user_data.data == NULL )
    {
        _error( aafi->ctx.options.verb, "Could not get essence stream from CFB.\n" );
        return -1;
    }



    /* Open stream with libsndfile */

    infile = sf_open_virtual( &sfvirtual, SFM_READ, &sfinfo, &user_data );

    if ( infile == 0 )
    {
      _error( aafi->ctx.options.verb, "libsndfile could not open stream : %s.\n", sf_strerror( NULL ) );

      free( user_data.data );

      return -1;
    }



    /* Build file path */

    char filePath[1024];

    snprintf( filePath, 1024, "%s/%ls.%s",
        outfilepath,
        ( forcedFileName != NULL ) ? forcedFileName : eascii_to_ascii(audioEssence->unique_file_name),
        sf_format_to_file_ext( format ) );



    /* Set format */

	sfinfo.format = format;

	if ( sf_format_check( &sfinfo ) == 0 )
	{
		_error( aafi->ctx.options.verb, "Invalid encoding.\n" );
    sf_close( infile );
		return -1;
	}



    /* Prepare bext chunk if WAVE file */

    SF_BROADCAST_INFO bext;

    memset( &bext, 0, sizeof (bext) );

    if ( format & SF_FORMAT_WAV )
    {
        /* Non-AAF standard, but makes sense */

    	snprintf( bext.description, sizeof(bext .description), "%ls %ls %ls",
            aafi->aafd->Identification.CompanyName,
            aafi->aafd->Identification.ProductName,
            aafi->aafd->Identification.ProductVersionString );
    	snprintf( bext.originator, sizeof(bext.originator), "%ls", aafi->aafd->Identification.CompanyName );
    	snprintf( bext.originator_reference, sizeof(bext.originator_reference), "%ls", aafi->aafd->Identification.ProductName );


        /* AAF Standard */

        memcpy( bext.origination_date, audioEssence->originationDate, sizeof(bext.origination_date) );
        memcpy( bext.origination_time, audioEssence->originationTime, sizeof(bext.origination_time) );
        memcpy( bext.umid, audioEssence->umid, sizeof(bext.umid) );
        bext.time_reference_high = (uint32_t)( audioEssence->timeReference >> 32 );
        bext.time_reference_low  = (uint32_t)( audioEssence->timeReference & 0xffffffff );
    	bext.coding_history_size = 0;
    }



    /* Prepare output file */

    SNDFILE *outfile = sf_open( filePath, SFM_WRITE, &sfinfo );

	if ( outfile == 0 )
	{
    _error( aafi->ctx.options.verb, "Could not open file %s : %s.\n", filePath, sf_strerror( NULL ) );
    sf_close( infile );
		return -1;
	}



    /* Add bext chunk */

    if ( ( format & SF_FORMAT_WAV ) && sf_command( outfile, SFC_SET_BROADCAST_INFO, &bext, sizeof(bext) ) == SF_FALSE )
    {
      _error ( aafi->ctx.options.verb, "libsndfile could not add bext chunk.\n" );
    }



    /* Write output file */

    int   readcount = 0;
    float buffer[BUFFER_LEN];

	  while ( (readcount = sf_read_float( infile, buffer, BUFFER_LEN )) > 0 )
    {
		  sf_write_float (outfile, buffer, readcount);
    }



    audioEssence->exported_file_path = malloc( (strlen(filePath) + 1) * sizeof(wchar_t) );

    swprintf( audioEssence->exported_file_path, (strlen(filePath) * sizeof(wchar_t)), L"%s", filePath );



    free( user_data.data );

    sf_close( outfile );
    sf_close( infile );

    return 0;
}




/******************************************************************************
                          libsndfile callbacks
*******************************************************************************/




static sf_count_t vfget_filelen( void *user_data )
{
    VIO_DATA *vf = (VIO_DATA*)user_data;

	return vf->length;
}




static sf_count_t vfseek( sf_count_t offset, int whence, void *user_data )
{
    VIO_DATA *vf = (VIO_DATA*)user_data;

	switch ( whence )
	{
        case SEEK_SET:    vf->offset = offset;                  break;
		case SEEK_CUR:    vf->offset = vf->offset + offset;     break;
		case SEEK_END:    vf->offset = vf->length + offset;     break;

		default:          break;
	}

	return vf->offset;
}




static sf_count_t vfread( void *ptr, sf_count_t count, void *user_data )
{
    VIO_DATA *vf = (VIO_DATA*)user_data;

	/*
	 *	This will brack badly for files over 2Gig in length, but
	 *	is sufficient for testing.
	 */

	if ( vf->offset + count > vf->length )
    {
		count = vf->length - vf->offset;
    }

	memcpy (ptr, vf->data + vf->offset, count);

	vf->offset += count;

	return count;
}




static sf_count_t vfwrite( const void *ptr, sf_count_t count, void *user_data )
{
    VIO_DATA *vf = (VIO_DATA*)user_data;

	/*
	 *	This will break badly for files over 2Gig in length, but
	 *	is sufficient for testing.
	 */

	if ( vf->offset >= vf->length /*SIGNED_SIZEOF (vf->data)*/ )
		return 0;

	if ( vf->offset + count > vf->length/*SIGNED_SIZEOF (vf->data)*/ )
		count = /*sizeof (vf->data)*/ vf->length - vf->offset;

	memcpy( vf->data + vf->offset, ptr, (size_t)count );

	vf->offset += count;

	if ( vf->offset > vf->length )
		 vf->length = vf->offset;

	return count;
}




static sf_count_t vftell( void *user_data )
{
    VIO_DATA *vf = (VIO_DATA*)user_data;

	return vf->offset;
}

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
#include <string.h>
// #include <unistd.h> // readlink()
#include <wchar.h>
// #include <dirent.h>
#include <inttypes.h>
#include <ctype.h>
// #include <libgen.h> // basename()
#include <stdarg.h>

#if defined(__linux__)
  #include <linux/limits.h>
  #include <arpa/inet.h>
  #include <mntent.h>
  #include <unistd.h> // access()
#elif defined(__APPLE__)
  #include <sys/syslimits.h>
  #include <unistd.h> // access()
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  #include <limits.h>
  #define R_OK    4       /* Test for read permission.  */
  #define W_OK    2       /* Test for write permission.  */
  // #define X_OK    1       /* execute permission - unsupported in windows*/
  #define F_OK    0       /* Test for existence.  */
  #ifndef _MSC_VER
    #include <unistd.h> // access()
  #endif
#endif


#include <libaaf/AAFIface.h>
// #include <libaaf/AAFToText.h> // MobIDToText()
#include <libaaf/AAFIAudioFiles.h>
#include <libaaf/debug.h>

#include "RIFFParser.h"
#include "URIParser.h"

#include "../common/utils.h"

// #define SIGNED_SIZEOF(x) sizeof(x)

/*
 * swprintf() specific string format identifiers
 * https://learn.microsoft.com/en-us/cpp/c-runtime-library/format-specification-syntax-printf-and-wprintf-functions?view=msvc-170#type
 */
#ifdef _WIN32
  #define WPRIs  L"S" // char*
	#define WPRIws L"s" // wchar_t*
#else
  #define WPRIs  L"s"  // char*
	#define WPRIws L"ls" // wchar_t*
#endif


#define debug( ... ) \
	_dbg( aafi->dbg, aafi, DEBUG_SRC_ID_AAF_IFACE, VERB_DEBUG, __VA_ARGS__ )

#define warning( ... ) \
	_dbg( aafi->dbg, aafi, DEBUG_SRC_ID_AAF_IFACE, VERB_WARNING, __VA_ARGS__ )

#define error( ... ) \
	_dbg( aafi->dbg, aafi, DEBUG_SRC_ID_AAF_IFACE, VERB_ERROR, __VA_ARGS__ )


#ifdef USE_LIBSNDFILE
  // #include <sndfile.h>
  //
  // static sf_count_t vfget_filelen( void *user_data );
  // static sf_count_t vfseek( sf_count_t offset, int whence, void *user_data );
  // static sf_count_t vfread( void *ptr, sf_count_t count, void *user_data );
  // static sf_count_t vfwrite( const void *ptr, sf_count_t count, void *user_data );
  // static sf_count_t vftell( void *user_data );
  //
  // static const char * sf_format_to_file_ext( uint32_t format );
  // static int32_t sf_format_to_samplesize( uint32_t format );
  // static int32_t samplesize_to_PCM_sf_format( uint32_t samplesize );
#endif


#ifdef _WIN32
#define DIR_SEP '\\'
#define DIR_SEP_STR "\\"
#else
#define DIR_SEP '/'
#define DIR_SEP_STR "/"
#endif

#define IS_DIR_SEP(c) \
  ( (c) == DIR_SEP || (c) == '/' )

/*
#define _fop_get_parent_path( start, end )\
{\
  end--;\
  while ( end > start && *end == DIR_SEP ) {\
    if ( end-1 == start && *start == DIR_SEP )\
      break;\
    --end;\
  }\
  while ( end > start && *end != DIR_SEP ) {\
    if ( end-1 == start && *start == DIR_SEP )\
      break;\
    --end;\
  }\
}
*/

// static char * fop_get_parent_path( const char *path, char **buf, size_t *bufsz, char include_trailing_sep );
static const char * fop_get_file( const char *filepath );
static size_t embeddedAudioDataReaderCallback( unsigned char *buf, size_t offset, size_t reqLen, void *user1, void *user2, void *user3 );
static size_t externalAudioDataReaderCallback( unsigned char *buf, size_t offset, size_t reqLen, void *user1, void *user2, void *user3 );




// static char * fop_get_parent_path( const char *path, char **buf, size_t *bufsz, char include_trailing_sep )
// {
//   if ( path == NULL )
//     return NULL;
//
//   char  *pbuf = NULL;
//   size_t pathsz = strlen( path ) + 1;
//
//   if ( buf == NULL ) {
//     pbuf = c99strdup( path );
//   }
//   else if ( *buf == NULL ) {
//     *buf = c99strdup( path );
//     if ( bufsz )
//       *bufsz = pathsz;
//     pbuf = *buf;
//   }
//   else if ( *buf && bufsz ) {
//     if ( *bufsz < pathsz ) {
//       *buf = realloc( *buf, pathsz );
//       *bufsz = pathsz;
//       pbuf = *buf;
//     }
//   }
//
//   const char *start = path;
//   const char *end = path + (pathsz-1);
//
//   // debug( "Get parent path from : %.*s", (int)(end - start), path );
//
//   _fop_get_parent_path( start, end );
//
//   snprintf( pbuf, pathsz, "%.*s%s", (int)(end - start), start, (include_trailing_sep==DIR_SEP) ? DIR_SEP_STR : "" );
//
//   // debug( "Parent path          : %s", pbuf );
//
//   return pbuf;
// }



static const char * fop_get_file( const char *filepath )
{
  if ( filepath == NULL ) {
    return NULL;
  }

  const char *end = filepath + strlen(filepath);

  while ( end > filepath && !IS_DIR_SEP(*end) ) {
    --end;
  }

  return ( IS_DIR_SEP(*end) ) ? end+1 : end;
}


char * locate_external_essence_file( AAF_Iface *aafi, const wchar_t *original_file_path, const char *search_location )
{
  /*
   * Absolute Uniform Resource Locator (URL) complying with RFC 1738 or relative
   * Uniform Resource Identifier (URI) complying with RFC 2396 for file containing
   * the essence. If it is a relative URI, the base URI is determined from the URI
   * of the AAF file itself.
   *
   * Informative note: A valid URL or URI uses a constrained character set and
   * uses the / character as the path separator.
   */

  if ( original_file_path == NULL ) {
    return NULL;
  }


  char *filepath = malloc( wcslen(original_file_path)+1 );
  snprintf( filepath, wcslen(original_file_path)+1, "%ls", original_file_path /*audioEssence->original_file*/ );

  // debug( "filepath : %s", filepath );


  if ( search_location ) {

    char *fpath = build_path( DIR_SEP_STR, search_location, fop_get_file(filepath), NULL );

    // debug( "search_location : %s", fpath );

    if ( access( fpath, F_OK ) != -1 ) {
      // debug( "FOUND: %s", fpath );
      free( filepath );
      return fpath;
    }

    free( fpath );
  }


  /* Try AAF essence's URI */

  if ( access( filepath, F_OK ) != -1 ) {
    // debug( "FOUND: %s", filepath );
    return filepath;
  }


  /* Try URI path */

  struct uri *uri = uriParse( filepath, URI_OPT_DECODE_ALL, aafi->dbg );

  // debug( "uri->path : %s", uri->path );

  if ( access( uri->path, F_OK ) != -1 ) {
    char *path = c99strdup( uri->path );
    // debug( "FOUND: %s", path );
    uriFree( uri );
    free( filepath );
    return path;
  }


  /* Try to reconstitute a relative path */

  if ( uri->flags & URI_T_LOCALHOST ) {

    /*
     * URI is supposed to handle relative file path, however we haven't encountered
     * any of this in real AAF files.
     * Instead, we see a lot of absolute files path, so we try to reconstitute a
     * relative path from current AAF file location and the first media parent dir
     */

    /*
     * extract relative path to essence file : "<firstparent>/<essence.file>"
     */

    char *relativeEssencePath = NULL;
    char *p = uri->path + strlen(uri->path);

    int sepcount = 0;

    while ( p > uri->path ) {
      if ( *p == '/' ) {
        sepcount++;
        if ( sepcount == 2 ) {
          relativeEssencePath = (p+1);
          break;
        }
      }
      p--;
    }


    /*
     * extract path to AAF file
     */

    char *aafPath = c99strdup( aafi->aafd->cfbd->file );

    p = aafPath + strlen(aafPath);

    while ( p > aafPath ) {
      if ( IS_DIR_SEP(*p) ) {
        *p = 0x00;
        break;
      }
      p--;
    }


    char *fpath = build_path( DIR_SEP_STR, aafPath, relativeEssencePath, NULL );

    free( aafPath );

    // debug( "reconstituted relative path : %s", fpath );

    if ( access( fpath, F_OK ) != -1 ) {
      uriFree( uri );
      free( filepath );
      // debug( "FOUND: %s", filepath );
      return fpath;
    }

    free( fpath );
  }
  else { /* remote */
  // if ( uri->flags & URI_T_REMOTE ) {
    if ( uri->flags & URI_T_HOST_IPV4 ) {

    }
    else if ( uri->flags & URI_T_HOST_IPV6 ) {

    }
    else if ( uri->flags & URI_T_HOST_REGNAME ) {

    }
  }

  uriFree( uri );

  free( filepath );

  return NULL;


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
}






int aafi_extract_audio_essence( AAF_Iface *aafi, aafiAudioEssence *audioEssence, const char *outfilepath, const wchar_t *forcedFileName, int format )
{
  (void)format;
  // SF_VIRTUAL_IO  sfvirtual;
  // VIO_DATA       user_data;
  // SF_INFO        sfinfo;
  // SNDFILE       *infile = NULL;

  if ( audioEssence->is_embedded == 0 ) {
    error( "Audio essence is not embedded : nothing to extract." );
    return -1;
  }

  // sfvirtual.get_filelen = vfget_filelen ;
	// sfvirtual.seek = vfseek ;
	// sfvirtual.read = vfread ;
	// sfvirtual.write = vfwrite ;
  // sfvirtual.tell = vftell ;

  // user_data.length = 0;
  // user_data.offset = 0;

  // memset( &sfinfo, 0x00, sizeof(SF_INFO) );



  // /*
  //  *  Set parameters manually if raw PCM stream. Otherwise, they will
  //  *  be set by libsndfile on stream opening.
  //  */
  // if ( audioEssence->summary == NULL )
  // {
  //   sfinfo.format     = samplesize_to_PCM_sf_format( audioEssence->samplesize );
  //   sfinfo.channels   = audioEssence->channels;
  //   sfinfo.samplerate = audioEssence->samplerate;
  // }





  /* Retrieve stream from CFB */

  unsigned char *data = NULL;
  uint64_t datasz = 0;

  cfb_getStream( aafi->aafd->cfbd, audioEssence->node, &data, &datasz );

  if ( data == NULL ) {
    error( "Could not retrieve audio essence stream from CFB." );
    return -1;
  }



  // /* Open stream with libsndfile */
  //
  // infile = sf_open_virtual( &sfvirtual, SFM_READ, &sfinfo, &user_data );
  //
  // if ( infile == 0 )
  // {
  //   error( "libsndfile could not open stream : %s.", sf_strerror( NULL ) );
  //
  //   free( user_data.data );
  //
  //   return -1;
  // }



  /* Build file path */

  char filePath[1024];

  snprintf( filePath, 1024, "%s%s%ls.%s",
      outfilepath,
      ( !IS_DIR_SEP(*(outfilepath+strlen(outfilepath)-1)) ) ? DIR_SEP_STR : "",
      ( forcedFileName != NULL ) ? forcedFileName : eascii_to_ascii(audioEssence->unique_file_name),
      (audioEssence->type == AAFI_ESSENCE_TYPE_AIFC) ? "aif" : "wav" );



  FILE *fp = fopen( filePath, "wb" );

  if ( fp == NULL ) {
    error( "Could not open audio essence file for writing (%s) : %s", filePath, strerror(errno) );
    free(data);
    return -1;
  }


  if ( audioEssence->type == AAFI_ESSENCE_TYPE_PCM ) {

    struct wavFmtChunk wavFmt;
    wavFmt.channels = audioEssence->channels;
    wavFmt.samples_per_sec = audioEssence->samplerate;
    wavFmt.bits_per_sample = audioEssence->samplesize;

    struct wavBextChunk wavBext;
    memset( &wavBext, 0x00, sizeof(wavBext) );
    memcpy( wavBext.umid, audioEssence->sourceMobID, sizeof(aafMobID_t) );
    if ( audioEssence->mobSlotEditRate ) {
      wavBext.time_reference = eu2sample( audioEssence->samplerate, audioEssence->mobSlotEditRate, audioEssence->timeReference );
    }

    if ( datasz >= (uint32_t)-1 ) {
      // TODO RF64 support ?
      error( "Audio essence is bigger than maximum wav size (2^32 bytes) : %"PRIu64" bytes", datasz );
      free(data);
      return -1;
    }

    // struct riffContext riffctx;
    // riffctx.verb = aafi->verb;
    // riffctx.debug_callback = aafi->debug_callback;
    // riffctx._dbg_msg = NULL;
    // riffctx._dbg_msg_size = 0;
    // riffctx.user = NULL;

    if ( riff_writeWavFileHeader( fp, &wavFmt, &wavBext, (uint32_t)datasz, aafi->dbg ) < 0 ) {
      error( "Could not write wav audio essence header : %s", filePath );
    }
  }


  uint64_t writtenBytes = fwrite( data, sizeof(unsigned char), datasz, fp );

  if ( writtenBytes < datasz ) {
    error( "Could not write audio essence file (%"PRIu64" bytes written out of %"PRIu64" bytes) : %s", writtenBytes, datasz, filePath );
    fclose( fp );
    free(data);
    return -1;
  }

  fclose( fp );


  // /* Prepare bext chunk if WAVE file */
  //
  // SF_BROADCAST_INFO bext;
  //
  // memset( &bext, 0, sizeof (bext) );
  //
  // if ( format & SF_FORMAT_WAV )
  // {
  //     /* Non-AAF standard, but makes sense */
  //
  // 	snprintf( bext.description, sizeof(bext .description), "%ls %ls %ls",
  //         aafi->aafd->Identification.CompanyName,
  //         aafi->aafd->Identification.ProductName,
  //         aafi->aafd->Identification.ProductVersionString );
  // 	snprintf( bext.originator, sizeof(bext.originator), "%ls", aafi->aafd->Identification.CompanyName );
  // 	snprintf( bext.originator_reference, sizeof(bext.originator_reference), "%ls", aafi->aafd->Identification.ProductName );
  //
  //
  //     /* AAF Standard */
  //
  //     memcpy( bext.origination_date, audioEssence->originationDate, sizeof(bext.origination_date) );
  //     memcpy( bext.origination_time, audioEssence->originationTime, sizeof(bext.origination_time) );
  //     memcpy( bext.umid, audioEssence->umid, sizeof(bext.umid) );
  //     bext.time_reference_high = (uint32_t)( audioEssence->timeReference >> 32 );
  //     bext.time_reference_low  = (uint32_t)( audioEssence->timeReference & 0xffffffff );
  // 	bext.coding_history_size = 0;
  // }



  // /* Prepare output file */
  //
  // SNDFILE *outfile = sf_open( filePath, SFM_WRITE, &sfinfo );
  //
	// if ( outfile == 0 )
	// {
  //   error( "Could not open file %s : %s.", filePath, sf_strerror( NULL ) );
  //   sf_close( infile );
	// 	return -1;
	// }
  //
  //
  //
  // /* Add bext chunk */
  //
  // if ( ( format & SF_FORMAT_WAV ) && sf_command( outfile, SFC_SET_BROADCAST_INFO, &bext, sizeof(bext) ) == SF_FALSE )
  // {
  //   error( "libsndfile could not add bext chunk." );
  // }
  //
  //
  //
  // /* Write output file */
  //
  // int   readcount = 0;
  // float buffer[BUFFER_LEN];
  //
  // while ( (readcount = sf_read_float( infile, buffer, BUFFER_LEN )) > 0 )
  // {
	//   sf_write_float (outfile, buffer, readcount);
  // }



  audioEssence->usable_file_path = malloc( (strlen(filePath) + 1) * sizeof(wchar_t) );

  swprintf( audioEssence->usable_file_path, strlen(filePath)+1, L"%" WPRIs, filePath );

  free( data );

  // sf_close( outfile );
  // sf_close( infile );

  return 0;
}



static size_t embeddedAudioDataReaderCallback( unsigned char *buf, size_t offset, size_t reqLen, void *user1, void *user2, void *user3 ) {

  unsigned char *data = user1;
  size_t datasz = *(size_t*)user2;
  AAF_Iface *aafi = (AAF_Iface*)user3;

  // FILE *fp = (FILE*)user1;
  //
  // fseek( fp, offset, SEEK_SET );
  //
  // return fread( buf, sizeof(unsigned char), reqLen, fp );
  if ( offset >= datasz ) {
    error( "Error: Requested data starts beyond data length" );
    return -1;
  }

  if ( offset+reqLen >= datasz ) {
    reqLen = datasz - (offset+reqLen);
  }

  memcpy( buf, data+offset, reqLen );

  return reqLen;
}

static size_t externalAudioDataReaderCallback( unsigned char *buf, size_t offset, size_t reqLen, void *user1, void *user2, void *user3 ) {

  (void)user2;
  AAF_Iface *aafi = (AAF_Iface*)user3;
  (void)aafi;

  FILE *fp = (FILE*)user1;

  fseek( fp, offset, SEEK_SET );

  return fread( buf, sizeof(unsigned char), reqLen, fp );
}

int parse_audio_summary( AAF_Iface *aafi, aafiAudioEssence *audioEssence )
{

  // dump_hex( audioEssence->summary->val, audioEssence->summary->len );

  int rc = 0;

  char *externalFilePath = NULL;



  struct RIFFAudioFile RIFFAudioFile;


  if ( audioEssence->is_embedded ) {

    if ( audioEssence->summary == NULL ) {
      warning( "TODO: Audio essence has no summary. Should try essence data stream ?" );
      return -1;
    }

    /*
        Adobe Premiere Pro, embedded mp3/mp4 files converted to PCM/AIFF on export, AAFClassID_AIFCDescriptor, 'COMM' is valid.
        ______________________________ Hex Dump ______________________________

        46 4f 52 4d 00 00 00 32  41 49 46 43 43 4f 4d 4d  |  FORM...2 AIFCCOMM
        00 00 00 26 00 01 00 00  00 00 00 10 40 0e bb 80  |  ........ ........
        00 00 00 00 00 00 4e 4f  4e 45 0e 4e 6f 74 20 43  |  ......NO NE.Not.C
        6f 6d 70 72 65 73 73 65  64 00                    |  ompresse d.
        ______________________________________________________________________
     */

    // dump_hex( audioEssence->summary->val, audioEssence->summary->len );

    // struct riffContext riffctx;
    // riffctx.verb = aafi->verb;
    // riffctx.debug_callback = aafi->debug_callback;
    // riffctx._dbg_msg = NULL;
    // riffctx._dbg_msg_size = 0;
    // riffctx.user = NULL;

    rc = riff_parseAudioFile( &RIFFAudioFile, &embeddedAudioDataReaderCallback, audioEssence->summary->val, &audioEssence->summary->len, aafi, aafi->dbg );

    if ( rc < 0 ) {
      warning( "TODO: Could not parse embedded essence summary. Trying essence data stream." );
      return -1;
    }
  }
  else {

    /* TODO: can external essence have audioEssence->summary too ? */

    externalFilePath = locate_external_essence_file( aafi, audioEssence->original_file_path, aafi->ctx.options.media_location );

    if ( externalFilePath == NULL ) {
      error( "Could not locate external audio essence file : %ls", audioEssence->original_file_path );
      return -1;
    }

    audioEssence->usable_file_path = malloc( (strlen(externalFilePath) + 1) * sizeof(wchar_t) );

    swprintf( audioEssence->usable_file_path, strlen(externalFilePath)+1, L"%" WPRIs, externalFilePath );



    FILE *fp = fopen( externalFilePath, "rb" );

    if ( fp == NULL ) {
      error( "Could not open external audio essence file for reading : %s", externalFilePath );
      return -1;
    }

    // struct riffContext riffctx;
    // riffctx.verb = aafi->verb;
    // riffctx.debug_callback = aafi->debug_callback;
    // riffctx._dbg_msg = NULL;
    // riffctx._dbg_msg_size = 0;
    // riffctx.user = NULL;

    rc = riff_parseAudioFile( &RIFFAudioFile, &externalAudioDataReaderCallback, fp, NULL, aafi, aafi->dbg );

    if ( rc < 0 ) {
      error( "TODO: Could not parse external essence file : %s", externalFilePath );
      fclose(fp);
      free(externalFilePath);
      return -1;
    }

    fclose(fp);
    free(externalFilePath);
  }


  // audioEssence->format     = sfinfo.format;
  audioEssence->channels   = RIFFAudioFile.channels;
  audioEssence->samplerate = RIFFAudioFile.sampleRate;
  audioEssence->samplesize = RIFFAudioFile.sampleSize;
  audioEssence->length     = RIFFAudioFile.duration;



  /* TODO : what the fuck was that for ? */

  // if ( aafi->Audio->samplerate >= 0 ) {
		// aafi->Audio->samplerate = ( aafi->Audio->samplerate == 0 || aafi->Audio->samplerate == audioEssence->samplerate ) ? audioEssence->samplerate : (unsigned)-1;
	// }
  //
  // if ( aafi->Audio->samplesize >= 0 ) {
    // aafi->Audio->samplesize = ( aafi->Audio->samplesize == 0 || aafi->Audio->samplesize == audioEssence->samplesize ) ? audioEssence->samplesize : -1;
  // }


  return 0;
}

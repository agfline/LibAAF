#include <stdio.h>
#include <string.h>
#include <unistd.h> // readlink()
#include <wchar.h>
#include <unistd.h> // access()
#include <dirent.h>
#include <inttypes.h>
#include <ctype.h>
#include <libgen.h> // dirname()
#include <stdarg.h>

#if defined(__linux__)
  #include <linux/limits.h>
  #include <arpa/inet.h>
  #include <mntent.h>
#elif defined(__APPLE__)
  #include <sys/syslimits.h>
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  #include <limits.h>
  // #include <Ws2tcpip.h>
#endif


#include <libaaf/AAFIface.h>
// #include <libaaf/AAFToText.h> // MobIDToText()
#include <libaaf/debug.h>

#include "RIFFParser.h"
#include "URIParser.h"
#include "../common/utils.h"

// #define SIGNED_SIZEOF(x) sizeof(x)



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

#define _fop_get_parent_path( start, end )\
{\
  end--;\
  /* removes any remaining trailing '/' */\
  while ( end > start && *end == DIR_SEP ) {\
    if ( end-1 == start && *start == DIR_SEP )\
      break;\
    --end;\
  }\
  /* removes any file/dir name, up to '/' */\
  while ( end > start && *end != DIR_SEP ) {\
    if ( end-1 == start && *start == DIR_SEP )\
      break;\
    --end;\
  }\
}

char * fop_get_parent_path( const char *path, char **buf, size_t *bufsz, char include_trailing_sep )
{
  if ( path == NULL )
    return NULL;

  char  *pbuf = NULL;
  size_t pathsz = strlen( path ) + 1;

  if ( buf == NULL ) {
    pbuf = strdup( path );
  }
  else if ( *buf == NULL ) {
    *buf = strdup( path );
    if ( bufsz )
      *bufsz = pathsz;
    pbuf = *buf;
  }
  else if ( *buf && bufsz ) {
    if ( *bufsz < pathsz ) {
      *buf = realloc( *buf, pathsz );
      *bufsz = pathsz;
      pbuf = *buf;
    }
  }

  const char *start = path;
  const char *end = path + (pathsz-1);

  // debug( "Get parent path from : %.*s", (int)(end - start), path );

  _fop_get_parent_path( start, end );

  snprintf( pbuf, pathsz, "%.*s%s", (int)(end - start), start, (include_trailing_sep==DIR_SEP) ? DIR_SEP_STR : "" );

  // debug( "Parent path          : %s", pbuf );

  return pbuf;
}



/*
 *  https://stackoverflow.com/questions/15198033/number-of-passed-arguments-in-a-va-list
 */
#define NARGS_SEQ(_1,_2,_3,_4,_5,_6,_7,_8,_9,N,...) N
#define NARGS(...) NARGS_SEQ(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define fop_concat_paths2(pconcat, pconcatsize, ...) fop_concat_paths2(NARGS(__VA_ARGS__) - 1, pconcat, pconcatsize, __VA_ARGS__)

char * (fop_concat_paths2)( int argc, char **pconcat, int *pconcatsize, const char *fmt, ... )
{
  va_list   args1, args2;
  va_start( args1, fmt );
  va_copy(  args2, args1 );

  int len = vsnprintf( NULL, 0, fmt, args1 ) + argc + 1; va_end( args1 );

  char   *buf = NULL;
  char **pbuf = &buf;

  if ( pconcat ) {
    pbuf = pconcat;
  }

  if ( pconcatsize == NULL || (pconcatsize && *pconcatsize < len) ) {
    *pbuf = realloc( *pbuf, len );

    if ( *pbuf == NULL ) {
      // debug( "realloc() failed : %s", strerror(errno) );
      fprintf( stderr, "realloc() failed : %s", strerror(errno) );
      return NULL;
    }

    if ( pconcatsize ) {
      *pconcatsize = len;
    }
  }


  char *arg    = NULL;
  int   arglen = 0;
  int   bufpos = 0;

  int modcount = 0;
  int totalmods = 0;

  for ( const char *p = fmt; *p; p++ )
    if ( *p == '%' )
      totalmods++;

  for ( const char *p = fmt; *p; p++ ) {
    if ( *p != '%' ) {
      bufpos += snprintf( (*pbuf)+bufpos, (len-bufpos), "%c", *p );
      continue;
    }

    modcount++;

switchagain:

    switch ( *(++p) ) {
      case 'i':  bufpos += snprintf( (*pbuf)+bufpos, (len-bufpos), "%i", va_arg( args2, int    ) );  break;
      case 'd':  bufpos += snprintf( (*pbuf)+bufpos, (len-bufpos), "%d", va_arg( args2, int    ) );  break;
      case 'c':  bufpos += snprintf( (*pbuf)+bufpos, (len-bufpos), "%c", va_arg( args2, int    ) );  break;
      case 'f':  bufpos += snprintf( (*pbuf)+bufpos, (len-bufpos), "%f", va_arg( args2, double ) );  break;

      case '.':
      case '*':
        goto switchagain;

      case 's':

        if ( *(p-1) == '%' ) {
          arg    = va_arg( args2, char* );
          arglen = strlen( arg );
        }
        else if ( p-2 > fmt && *(p-1) == '*' && *(p-2) == '.' ) {
          arglen = va_arg( args2, int );
          arg    = va_arg( args2, char* );
        }
        else {
          // debug("expected %%s or %%.*s modifier");
          fprintf( stderr, "expected %%s or %%.*s modifier" );
        }


        /*
         *  Removes all trailing '/', unless we're on the last
         *  path element, where we keep one single '/' if any
         */

        while ( arglen > 1 && *(arg+(arglen-1)) == DIR_SEP ) {
          // debug( "%i %c %c", arglen, *(arg+(arglen-1)), *(arg+(arglen-2)) );
          if ( modcount == totalmods && *(arg+(arglen-2)) != DIR_SEP )
            break;
          arglen--;
        }

        /*
         *  Removes all leading '/', unless we're on the first
         *  path element, where we keep one single '/' if any
         */

        while ( *arg == DIR_SEP ) {
          if ( modcount == 1 && *(arg+1) != DIR_SEP )
            break;
          arg++; arglen--;
        }

        bufpos += snprintf( (*pbuf)+bufpos, (len-bufpos), "%.*s%s", arglen, arg, (modcount==totalmods) ? "" : DIR_SEP_STR );
        break;

      default:
        // debug("modifier not recognized '%c'", *p );
        fprintf( stderr, "modifier not recognized '%c'", *p );
        break;
    }
  }


  // for ( int i = 0; i < argc; i++ ) {
  //   char *arg = va_arg( args2, char* );
  //   char arglen = strlen(arg);
  //
  //   debug("RAWARG: %s", arg);
  //
  //   while ( arglen > 1 && *(arg+(arglen-1)) == DIR_SEP ) {
  //     debug("arglen--");
  //     arglen--;
  //   }
  //
  //   while ( *arg == DIR_SEP ) {
  //     if ( i == 0 && *(arg+1) != DIR_SEP ) {
  //       break; // keeps path leading '/' if any
  //     }
  //     debug("%c", *arg);
  //     arg++;
  //     arglen--;
  //   }
  //
  //   debug(">>>>> %.*s", arglen, arg);
  //
  //
  //   pos += snprintf( buf+pos, (len-pos), "%.*s%s", arglen, arg, (i+1==argc) ? "" : DIR_SEP_STR );
  //   // debug("arg %s", arg);
  // }

  // vsprintf( node->str, fmt, args2 ); va_end( args2 );
  va_end( args2 );

  // debug("%s", buf);
  return *pbuf;
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



  char *filepath = malloc( wcslen(original_file_path)+1 );
  snprintf( filepath, wcslen(original_file_path)+1, "%ls", original_file_path /*audioEssence->original_file*/ );

  // printf( "\nfilepath : %s\n", filepath );


  if ( search_location ) {

    char *fpath = fop_concat_paths2( NULL, NULL, "%s%s", search_location, basename(filepath) );

    // printf( "search_location : %s\n", fpath );

    if ( access( fpath, F_OK ) != -1 ) {
      free( filepath );
      return fpath;
    }

    free( fpath );
  }


  /* Try AAF essence's URI */

  if ( access( filepath, F_OK ) != -1 ) {
    return filepath;
  }


  /* Try URI path */

  struct uri *uri = uriParse( filepath, URI_OPT_DECODE_ALL );

  // printf( "uri->path : %s\n", uri->path );

  if ( access( uri->path, F_OK ) != -1 ) {
    char *path = strdup( uri->path );
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

    char *aafPath = strdup( aafi->aafd->cfbd->file );

    p = aafPath + strlen(aafPath);

    while ( p > aafPath ) {
      if ( *p == DIR_SEP ) {
        *p = 0x00;
        break;
      }
      p--;
    }



    char *fpath = fop_concat_paths2( NULL, NULL, "%s%s", aafPath, relativeEssencePath );

    free( aafPath );

    // printf("reconstituted relative path : %s\n", fpath );

    if ( access( fpath, F_OK ) != -1 ) {
      uriFree( uri );
      free( filepath );
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



// char * locate_external_essence_file( AAF_Iface *aafi, const wchar_t *original_file /*aafiAudioEssence *audioEssence*/ )
// {
//   /* Absolute Uniform Resource Locator (URL) complying with RFC 1738 or relative Uniform Resource Identifier (URI)
//    * complying with RFC 2396 for file containing the essence. If it is a relative URI, the base URI is determined from the
//    * URI of the AAF file itself.
//    *
//    * Informative note: A valid URL or URI uses a constrained character set and uses the / character as the path separator
//    */
//
//   /* TODO handle realpath return value and free(absFilePath) in case of error */
//   char *filePath = malloc( PATH_MAX*2 );
//   snprintf( filePath, PATH_MAX, "%ls", original_file /*audioEssence->original_file*/ );
//
//
//   /* Try AAF essence's file path */
//
//   if ( access( filePath, F_OK ) != -1 )
//   {
//       return filePath;
//   }
//
//
//   /*
//     * AAFInfo --aaf-clips ../libaaf_testfiles/fonk_2.AAF
//     file://localhost/Users/horlaprod/Music/Logic/fonk_2/Audio Files_1/fonk_2_3#04.wav
//
//     * AAFInfo --aaf-clips ../libaaf_testfiles/ADP/ADP3_51-ST-MONO-NOBREAKOUT.aaf
//     file:///C:/Users/Loviniou/Downloads/ChID-BLITS-EBU-Narration441-16b.wav
//
//     * AAFInfo --aaf-clips ../libaaf_testfiles/ADP/ADP2_SEQ-FULL.aaf
//     file://?/E:/Adrien/ADPAAF/Sequence A Rendu.mxf
//
//     * AAFInfo --aaf-clips ../libaaf_testfiles/TEST-AVID_COMP2977052\ \ -\ \ OFF\ PODIUM\ ETAPE\ 2.aaf
//     file:////C:/Users/mix_limo/Desktop/TEST2977052  -  OFF PODIUM ETAPE 2.aaf
//
//     * AAFInfo --aaf-clips ../ardio/watchfolder/3572607_RUGBY_F_1_1.aaf
//     file://10.87.230.71/mixage/DR2/Avid MediaFiles/MXF/1/3572607_RUGBY_F2_S65CFA3D0V.mxf
//
//     * AAFInfo --aaf-clips ../libaaf_testfiles/ProTools/pt2MCC.aaf
//     file:///_system/Users/horlaprod/pt2MCCzmhsFRHQgdgsTMQX.mxf
//   */
//
//   int pos = 7;
//   int posmax = strlen(filePath);
//   char tmp[PATH_MAX+1];
//
//   if ( filePath[0] == 'f' &&
//        filePath[1] == 'i' &&
//        filePath[2] == 'l' &&
//        filePath[3] == 'e' &&
//        filePath[4] == ':' &&
//        filePath[5] == '/' &&
//        filePath[6] == '/' )
//   {
//
//     while ( pos < posmax && (filePath[pos] == '/' || filePath[pos] == '?') ) {
//       /*
//        * file://
//        * file:///
//        * file://?/
//        * file:////
//        */
//       pos++;
//     }
//
//     // printf("%s\n",filePath+pos );
//
//     // int ipa, ipb, ipc, ipd = 0;
//     // if ( sscanf( filePath+pos-2, "//%i.%i.%i.%i/", &ipa, &ipb, &ipc, &ipd ) == 4 )
//
//     sscanf( filePath+pos, "%[^/]", tmp );
//
//
// #if defined(__linux__)
//
//     /* Remote filesystem */
//     struct sockaddr_in sa;
//
//     if ( inet_pton( AF_INET, tmp, &(sa.sin_addr) ) == 1 )
//     {
//       pos-=2; // get first "//" back
//
//       struct mntent *m;
//
//       // printf( "_PATH_MOUNTED macro: %s\n", _PATH_MOUNTED );
//
//       FILE *f = setmntent( _PATH_MOUNTED, "r" );
//
//       while ( (m = getmntent(f)) )
//       {
//         // printf( "Device: %s      Mount Point: %s\n", m->mnt_fsname, m->mnt_dir );
//
//         if ( strncmp( m->mnt_fsname, filePath+pos, strlen(m->mnt_fsname) ) == 0 )
//         {
//           snprintf( tmp, PATH_MAX, "%s/%s", m->mnt_dir, filePath+pos+strlen(m->mnt_fsname) );
//           snprintf( filePath, PATH_MAX*2, "%s", tmp );
//
//           // printf("\n\nRemote file : %s\n\n", filePath );
//           endmntent(f);
//           return filePath;
//         }
//       }
//
//       // printf("\n\nCould not retrieve mounted point for remote file : %s\n\n", filePath+pos );
//
//       free( filePath );
//       return NULL;
//
//       endmntent(f);
//     }
//     /* local filesystem */
//     else
//
// #endif
//
//     if ( pos + 9 + 1 < posmax &&
//          filePath[pos+0] == 'l' &&
//          filePath[pos+1] == 'o' &&
//          filePath[pos+2] == 'c' &&
//          filePath[pos+3] == 'a' &&
//          filePath[pos+4] == 'l' &&
//          filePath[pos+5] == 'h' &&
//          filePath[pos+6] == 'o' &&
//          filePath[pos+7] == 's' &&
//          filePath[pos+8] == 't' &&
//          filePath[pos+9] == '/' )
//     {
//       pos+=10;
//     }
//
//
//     /* Windows drive */
//     if ( pos + 2 + 1 < posmax &&
//          isalpha(filePath[pos+0]) &&
//          filePath[pos+1] == ':' &&
//          filePath[pos+2] == '/' )
//     {
//       snprintf( tmp, PATH_MAX, "%s", filePath+pos );
//       snprintf( filePath, PATH_MAX*2, "%s", tmp );
//
//       // printf("\n\nLocal Windows file : %s\n\n", filePath );
//
//       return filePath;
//     }
//     /* MacOS */
//     else if ( pos + 7 + 1 < posmax &&
//               filePath[pos+0] == '_' &&
//               filePath[pos+1] == 's' &&
//               filePath[pos+2] == 'y' &&
//               filePath[pos+3] == 's' &&
//               filePath[pos+4] == 't' &&
//               filePath[pos+5] == 'e' &&
//               filePath[pos+6] == 'm' &&
//               filePath[pos+7] == '/' )
//     {
//       pos+=7;
//
//       snprintf( tmp, PATH_MAX, "%s", filePath+pos );
//       snprintf( filePath, PATH_MAX*2, "%s", tmp );
//
//       // printf("\n\nLocal MacOS file : %s\n\n", filePath );
//     }
//     /* Unix: MacOS / Linux */
//     else
//     {
//       pos--; // -1 to get first '/'
//
//       snprintf( tmp, PATH_MAX, "%s", filePath+pos );
//       snprintf( filePath, PATH_MAX*2, "%s", tmp );
//
//       // printf("\n\nLocal Unix file : %s\n\n", filePath );
//
//       return filePath;
//     }
//
//     /* TODO: Try network domain name */
//   }
//
//
//
//
//
// #if defined(__linux__)
//
//   // char *absFilePath = malloc( PATH_MAX*2 );
//
//   /* Prepare research */
//   char *file    = strrchr( filePath, '/' ) + 1;
//   char *aafFile = strrchr( aafi->aafd->cfbd->file, '/' );
//
//
//   if ( file == 1 ) {
//     free( filePath );
//     return NULL;
//   }
//
//
//   char path[PATH_MAX];
//
//   snprintf( path, (aafFile - aafi->aafd->cfbd->file)+2, "%s", aafi->aafd->cfbd->file );
//
//   // printf( "Path : %s\n", path );
//   // printf( "File : %s\n", file );
//
//
//   /* Search AAF's directory */
//
//   struct dirent *dir;
//   DIR *d = opendir( path );
//
//   if ( d )
//   {
//       while ( (dir = readdir(d)) != NULL )
//       {
//           if ( dir->d_type != DT_REG )
//           {
//               continue;
//           }
//
//           if ( strcmp( dir->d_name, file ) == 0 )
//           {
//             /*
// ==5402== Source and destination overlap in mempcpy(0x13e86d53, 0x13e86d65, 31)
// ==5402==    at 0x483D580: mempcpy (vg_replace_strmem.c:1536)
// ==5402==    by 0x91A5887: _IO_default_xsputn (genops.c:386)
// ==5402==    by 0x91A5887: _IO_default_xsputn (genops.c:370)
// ==5402==    by 0x91781FA: vfprintf (vfprintf.c:1638)
// ==5402==    by 0x91A079F: vsnprintf (vsnprintf.c:114)
// ==5402==    by 0x91806BE: snprintf (snprintf.c:33)
// ==5402==    by 0x487A11C: locate_external_essence_file (AAFIAudioFiles.c:213)
// ==5402==    by 0x128AF0: main (aaf.cc:779)
// ==5402==
//             */
//               snprintf( filePath, PATH_MAX*2, "%s%s", path, file );
//               // printf( "::FOUND %s\n", filePath );
//
//               // return realpath(filePath, absFilePath);
//               return filePath;
//           }
//       }
//
//       closedir(d);
//   }
//
//
//   /* Search one level inside all directories next to AAF file */
//
//   char subPath[PATH_MAX*2];
//   struct dirent *sdir;
//   DIR *sd = NULL;
//
//   d = opendir( path );
//
//   if ( d )
//   {
//       while ( (dir = readdir(d)) != NULL )
//       {
//           if (  dir->d_type    != DT_DIR ||
//               ( dir->d_name[0] == '.' && dir->d_name[1] == '\0') ||
//               ( dir->d_name[0] == '.' && dir->d_name[1] == '.' && dir->d_name[2] == '\0' ) )
//           {
//               continue;
//           }
//
//           snprintf( subPath, sizeof(subPath), "%s%s/", path, dir->d_name );
//
//           sd = opendir( subPath );
//
//           if ( sd )
//           {
//               while ( (sdir = readdir(sd)) != NULL )
//               {
//                   if ( strcmp( sdir->d_name, file ) == 0 )
//                   {
//                       snprintf( filePath, PATH_MAX*2, "%s%s", subPath, file );
//                       // printf( "::FOUND %s\n", filePath );
//
//                       // return realpath(filePath, absFilePath);
//                       return filePath;
//                   }
//               }
//
//               closedir(sd);
//           }
//       }
//
//       closedir(d);
//   }
//
//
// #endif
//
//   free( filePath );
//   return NULL;
// }











#ifdef USE_LIBSNDFILE

// static const char * sf_format_to_file_ext( uint32_t format )
// {
//     switch ( format & SF_FORMAT_TYPEMASK )
//     {
//         case SF_FORMAT_WAV:         return "wav";
//         case SF_FORMAT_RF64:        return "wav";
//         case SF_FORMAT_AIFF:        return "aiff";
//         case SF_FORMAT_AU:          return "au";
//         case SF_FORMAT_RAW:         return "raw";
//         case SF_FORMAT_W64:         return "w64";
//
//         default:                    break;
//     }
//
//     /* TODO move to caller */
//     // _error( "Could not retrieve sample size from libsndfile format.\n" );
//     return NULL;
// }
//
//
// static int32_t sf_format_to_samplesize( uint32_t format )
// {
//     switch ( format & SF_FORMAT_SUBMASK )
//     {
//         case SF_FORMAT_PCM_U8:
//         case SF_FORMAT_PCM_S8:      return  8;
//         case SF_FORMAT_PCM_16:      return 16;
//         case SF_FORMAT_PCM_24:      return 24;
//         case SF_FORMAT_PCM_32:      return 32;
//         case SF_FORMAT_FLOAT:       return 32;
//         case SF_FORMAT_DOUBLE:      return 64;
//
//         // case SF_FORMAT_DPCM_8:      return  8;
//         // case SF_FORMAT_DPCM_16:     return 16;
//         //
//         // case SF_FORMAT_DWVW_12:     return 12;
//         // case SF_FORMAT_DWVW_16:     return 16;
//         // case SF_FORMAT_DWVW_24:     return 24;
//         //
//         // case SF_FORMAT_ALAC_16:     return 16;
//         // case SF_FORMAT_ALAC_20:     return 20;
//         // case SF_FORMAT_ALAC_24:     return 24;
//         // case SF_FORMAT_ALAC_32:     return 32;
//
//         default:                    break;
//     }
//
//     /* TODO move to caller */
//     // _error( "Could not retrieve sample size from libsndfile format.\n" );
//     return -1;
// }
//
//
// static int32_t samplesize_to_PCM_sf_format( uint32_t samplesize )
// {
//     int32_t format = 0 | SF_FORMAT_RAW;
//
//     switch ( samplesize )
//     {
//         case  8:    format |= SF_FORMAT_PCM_S8;     break;
//         case 16:    format |= SF_FORMAT_PCM_16;     break;
//         case 24:    format |= SF_FORMAT_PCM_24;     break;
//         case 32:    format |= SF_FORMAT_PCM_32;     break;
//
//         default:
//
//             /* TODO move to caller */
//             // _error( "Could not build libsndfile format from sample size %u.\n", samplesize );
//             return -1;
//     }
//
//     return format;
// }
//
//
// SNDFILE * open_external_file( const char *filePath, SF_INFO *sfinfo )
// {
//     if ( filePath == NULL )
//     {
//         /* TODO move to caller */
//         // _error( "Could not locate external essence file.\n" );
//         return NULL;
//     }
//
//     memset( sfinfo, 0x00, sizeof(SF_INFO) );
//
//     SNDFILE *file = sf_open( filePath, SFM_READ, sfinfo );
//
//     return file;
// }
//
//
// SNDFILE * open_internal_file( AAF_Iface *aafi, aafiAudioEssence *audioEssence, SF_INFO *sfinfo )
// {
//     uint32_t          id = 0;
//     size_t     sectorLen = 0;
//     aafByte_t *sectorBuf = NULL;
//
//     SF_VIRTUAL_IO sfvirtual;
//     sfvirtual.get_filelen = vfget_filelen;
//     sfvirtual.seek = vfseek;
//     sfvirtual.read = vfread;
//     sfvirtual.write = vfwrite;
//     sfvirtual.tell = vftell;
//
//     VIO_DATA user_data;
//     user_data.length = 0;
//     user_data.offset = 0;
//
//     memset( &user_data, 0x00, sizeof(VIO_DATA) );
//
//     cfb_foreachSectorInStream( aafi->aafd->cfbd, audioEssence->node, &sectorBuf, &sectorLen, &id )
//     {
//         user_data.data   = sectorBuf;
//         user_data.length = sectorLen;
//
//         break;
//     }
//
//     memset( sfinfo, 0x00, sizeof(SF_INFO) );
//
//     SNDFILE *file = sf_open_virtual( &sfvirtual, SFM_READ, sfinfo, &user_data );
//
//     free( sectorBuf );
//
//     return file;
// }
//
//
// SNDFILE * open_summary( aafiAudioEssence *audioEssence, SF_INFO *sfinfo )
// {
//     SF_VIRTUAL_IO sfvirtual;
//     sfvirtual.get_filelen = vfget_filelen;
//     sfvirtual.seek = vfseek;
//     sfvirtual.read = vfread;
//     sfvirtual.write = vfwrite;
//     sfvirtual.tell = vftell;
//
//     VIO_DATA user_data;
//     user_data.length = 0;
//     user_data.offset = 0;
//     user_data.data   = audioEssence->summary->val;
//     user_data.length = audioEssence->summary->len;
//
//     memset( sfinfo, 0x00, sizeof(SF_INFO) );
//
//     SNDFILE *file = sf_open_virtual( &sfvirtual, SFM_READ, sfinfo, &user_data );
//
//     return file;
// }
//
//
// int parse_audio_summary( AAF_Iface *aafi, aafiAudioEssence *audioEssence )
// {
//     SNDFILE *file = NULL;
//
//     SF_INFO sfinfo;
//
//     char *externalFilePath = NULL;
//
//
//
//     if ( audioEssence->summary != NULL )
//     {
//
//         file = open_summary( audioEssence, &sfinfo );
//
//         if ( file == NULL )
//         {
//             /*
//              *  The summary should be a copy of the header without audio samples.
//              *  It has been seen (on fairlight's AIFC-AAF) that the summary does
//              *  not contain the full header and stop right before the SSND chunk.
//              *  In that case, libsndfile does not handle it.
//              *
//              *  In such cases we fallback on the first stream sector to retrieve
//              *  full WAVE/AIFC header, or on external audio file if essence is
//              *  not embedded.
//              */
//
//             if ( audioEssence->is_embedded )
//             {
//     		    _warning( aafi->ctx.options.verb, "libsndfile could not read descriptor summary : %s. Falling back on internal audio file stream.\n", sf_strerror( NULL ) );
//
//                 file = open_internal_file( aafi, audioEssence, &sfinfo );
//
//                 if ( file == NULL )
//                 {
//                     _error( aafi->ctx.options.verb, "Could not open embedded essence file.\n" );
//                     return -1;
//                 }
//             }
//             else
//             {
//                 _warning( aafi->ctx.options.verb, "libsndfile could not read descriptor summary : %s. Falling back on external audio file stream.\n", sf_strerror( NULL ) );
//
//                 // dump_hex( audioEssence->summary->val, audioEssence->summary->len );
//
//                 externalFilePath = locate_external_essence_file( aafi, audioEssence->original_file );
//
//                 // printf("externalFilePath : %s\n", externalFilePath );
//
//                 file = open_external_file( externalFilePath, &sfinfo );
//
//                 if ( file == NULL )
//                 {
//                     _error( aafi->ctx.options.verb, "Could not open external essence file.\n" );
//                     free(externalFilePath);
//                     return -1;
//                 }
//
//                 free(externalFilePath);
//             }
//         }
//     }
//
//
//     /* Extract usefull data */
//
//     audioEssence->format     = sfinfo.format;
//     audioEssence->channels   = sfinfo.channels;
//     audioEssence->samplerate = sfinfo.samplerate;
//     audioEssence->samplesize = sf_format_to_samplesize( audioEssence->format );
//
//     // if ( aafi->Audio->samplerate >= 0 )
//   	// {
//   			aafi->Audio->samplerate = ( aafi->Audio->samplerate == 0 || aafi->Audio->samplerate == audioEssence->samplerate ) ? audioEssence->samplerate : (unsigned)-1;
//   	// }
//
//     // printf("%i  :  %i\n", aafi->Audio->samplesize, audioEssence->samplesize );
//
//     if ( aafi->Audio->samplesize >= 0 )
//     {
//       aafi->Audio->samplesize = ( aafi->Audio->samplesize == 0 || aafi->Audio->samplesize == audioEssence->samplesize ) ? audioEssence->samplesize : -1;
//     }
//
//
//
//
//     /*
//      *  The length was retrieved by retrieve_EssenceData() and correspond
//      *  to the full stream length, including file header. Here we need to
//      *  get only audio data stream length to do proper duration calculation,
//      *
//      *  TODO this should be optionnal, since we might need it only when analysing files.
//      */
//
//     SF_CHUNK_INFO	    chunk_info;
//     SF_CHUNK_ITERATOR  *iterator;
//     int err = 0;
//
//     memset( &chunk_info, 0x00, sizeof(SF_CHUNK_INFO) );
//
//
//     char *chunk = NULL;
//
//     if ( audioEssence->type == AAFI_TYPE_WAVE )
//     {
//         chunk = "data";
//     }
//     else if ( audioEssence->type == AAFI_TYPE_AIFC )
//     {
//         chunk = "SSND";
//     }
//
//     snprintf( chunk_info.id, sizeof(chunk_info.id), chunk );
//
//
//
//     iterator = sf_get_chunk_iterator( file, &chunk_info );
//
//     if ( iterator == NULL )
//     {
//         if ( audioEssence->is_embedded )
//         {
//             _warning( aafi->ctx.options.verb, "Could not retrieve \"%s\" chunk. Falling back on stream length calculation.\n", chunk );
//
//             /*
//              *  NOTE nothing guarentee there are no chunk after audio data, in which
//              *  case the length would be wrong. In practice, this was not encountered.
//              *  on embedded files.
//              */
//
//             audioEssence->length = (audioEssence->length - (1<<cfb_getStreamSectorShift( aafi->aafd->cfbd, audioEssence->node ))) +
//                                    (sfinfo.frames * (audioEssence->samplesize/8));
//             sf_close( file );
//
//             return 0;
//         }
//         else if ( externalFilePath == NULL ) /* we have not open file yet */
//         {
//             _warning( aafi->ctx.options.verb, "Could not retrieve \"%s\" chunk from summary. Falling back on external essence parsing.\n", chunk );
//
//             /* Close previous stream */
//             sf_close( file );
//
//             externalFilePath = locate_external_essence_file( aafi, audioEssence->original_file );
//
//             file = open_external_file( externalFilePath, &sfinfo );
//
//             if ( file == NULL )
//             {
//                 _error( aafi->ctx.options.verb, "Could not open external essence file : %s\n", sf_strerror( NULL ) );
//                 free(externalFilePath);
//                 return -1;
//             }
//
//             free(externalFilePath);
//         }
//         else
//         {
//             _error( aafi->ctx.options.verb, "Could not retrieve \"%s\" chunk.\n", chunk );
//             sf_close( file );
//             return -1;
//         }
//     }
//
//
//
//     err = sf_get_chunk_size( iterator, &chunk_info );
//
//     if ( err != SF_ERR_NO_ERROR )
//     {
//         if ( audioEssence->is_embedded )
//         {
//             _warning( aafi->ctx.options.verb, "Could not retrieve \"%s\" chunk size : %s. Falling back on stream length calculation.", chunk, sf_error_number(err) );
//
//             /*
//              *  NOTE nothing guarentee there are no chunk after audio data, in which
//              *  case the length would be wrong. In practice, this was not encountered.
//              *  on embedded files.
//              */
//
//             audioEssence->length = (audioEssence->length - (1<<cfb_getStreamSectorShift( aafi->aafd->cfbd, audioEssence->node ))) +
//                                    (sfinfo.frames * (audioEssence->samplesize/8));
//             sf_close( file );
//             return 0;
//         }
//         else
//         {
//             _error( aafi->ctx.options.verb, "Could not retrieve \"%s\" chunk size : %s\n", chunk, sf_error_number (err) );
//             sf_close( file );
//             return -1;
//         }
//     }
//
//
//
//     audioEssence->length = chunk_info.datalen;
//
//
//     sf_close( file );
//
//     return 0;
// }
//
//
// int aafi_extract_audio_essence( AAF_Iface *aafi, aafiAudioEssence *audioEssence, const char *outfilepath, const wchar_t *forcedFileName, int format )
// {
//     SF_VIRTUAL_IO  sfvirtual;
//     VIO_DATA       user_data;
//     SF_INFO        sfinfo;
//     SNDFILE       *infile = NULL;
//
//     if ( audioEssence->is_embedded == 0 )
//     {
//         _error( aafi->ctx.options.verb, "Essence is not embedded : nothing to extract.\n" );
//         return -1;
//     }
//
//     sfvirtual.get_filelen = vfget_filelen ;
//   	sfvirtual.seek = vfseek ;
//   	sfvirtual.read = vfread ;
//   	sfvirtual.write = vfwrite ;
//     sfvirtual.tell = vftell ;
//
//     user_data.length = 0;
//     user_data.offset = 0;
//
//     memset( &sfinfo, 0x00, sizeof(SF_INFO) );
//
//
//
//     /*
//      *  Set parameters manually if raw PCM stream. Otherwise, they will
//      *  be set by libsndfile on stream opening.
//      */
//
//     if ( audioEssence->summary == NULL )
//     {
//       sfinfo.format     = samplesize_to_PCM_sf_format( audioEssence->samplesize );
// 	    sfinfo.channels   = audioEssence->channels;
//       sfinfo.samplerate = audioEssence->samplerate;
//     }
//
//
//
//     /* Retrieve stream from CFB */
//
//     cfb_getStream( aafi->aafd->cfbd, audioEssence->node, (unsigned char**)(&user_data.data), (uint64_t*)(&user_data.length) );
//
//     if ( user_data.data == NULL )
//     {
//         _error( aafi->ctx.options.verb, "Could not get essence stream from CFB.\n" );
//         return -1;
//     }
//
//
//
//     /* Open stream with libsndfile */
//
//     infile = sf_open_virtual( &sfvirtual, SFM_READ, &sfinfo, &user_data );
//
//     if ( infile == 0 )
//     {
//       _error( aafi->ctx.options.verb, "libsndfile could not open stream : %s.\n", sf_strerror( NULL ) );
//
//       free( user_data.data );
//
//       return -1;
//     }
//
//
//
//     /* Build file path */
//
//     char filePath[1024];
//
//     snprintf( filePath, 1024, "%s/%ls.%s",
//         outfilepath,
//         ( forcedFileName != NULL ) ? forcedFileName : eascii_to_ascii(audioEssence->unique_file_name),
//         sf_format_to_file_ext( format ) );
//
//
//
//     /* Set format */
//
// 	sfinfo.format = format;
//
// 	if ( sf_format_check( &sfinfo ) == 0 )
// 	{
// 		_error( aafi->ctx.options.verb, "Invalid encoding.\n" );
//     sf_close( infile );
// 		return -1;
// 	}
//
//
//
//     /* Prepare bext chunk if WAVE file */
//
//     SF_BROADCAST_INFO bext;
//
//     memset( &bext, 0, sizeof (bext) );
//
//     if ( format & SF_FORMAT_WAV )
//     {
//         /* Non-AAF standard, but makes sense */
//
//     	snprintf( bext.description, sizeof(bext .description), "%ls %ls %ls",
//             aafi->aafd->Identification.CompanyName,
//             aafi->aafd->Identification.ProductName,
//             aafi->aafd->Identification.ProductVersionString );
//     	snprintf( bext.originator, sizeof(bext.originator), "%ls", aafi->aafd->Identification.CompanyName );
//     	snprintf( bext.originator_reference, sizeof(bext.originator_reference), "%ls", aafi->aafd->Identification.ProductName );
//
//
//         /* AAF Standard */
//
//         memcpy( bext.origination_date, audioEssence->originationDate, sizeof(bext.origination_date) );
//         memcpy( bext.origination_time, audioEssence->originationTime, sizeof(bext.origination_time) );
//         memcpy( bext.umid, audioEssence->umid, sizeof(bext.umid) );
//         bext.time_reference_high = (uint32_t)( audioEssence->timeReference >> 32 );
//         bext.time_reference_low  = (uint32_t)( audioEssence->timeReference & 0xffffffff );
//     	bext.coding_history_size = 0;
//     }
//
//
//
//     /* Prepare output file */
//
//     SNDFILE *outfile = sf_open( filePath, SFM_WRITE, &sfinfo );
//
// 	if ( outfile == 0 )
// 	{
//     _error( aafi->ctx.options.verb, "Could not open file %s : %s.\n", filePath, sf_strerror( NULL ) );
//     sf_close( infile );
// 		return -1;
// 	}
//
//
//
//     /* Add bext chunk */
//
//     if ( ( format & SF_FORMAT_WAV ) && sf_command( outfile, SFC_SET_BROADCAST_INFO, &bext, sizeof(bext) ) == SF_FALSE )
//     {
//       _error ( aafi->ctx.options.verb, "libsndfile could not add bext chunk.\n" );
//     }
//
//
//
//     /* Write output file */
//
//     int   readcount = 0;
//     float buffer[BUFFER_LEN];
//
// 	  while ( (readcount = sf_read_float( infile, buffer, BUFFER_LEN )) > 0 )
//     {
// 		  sf_write_float (outfile, buffer, readcount);
//     }
//
//
//
//     audioEssence->usable_file_path = malloc( (strlen(filePath) + 1) * sizeof(wchar_t) );
//
//     swprintf( audioEssence->usable_file_path, (strlen(filePath) * sizeof(wchar_t)), L"%s", filePath );
//
//
//
//     free( user_data.data );
//
//     sf_close( outfile );
//     sf_close( infile );
//
//     return 0;
// }
//
//
// static sf_count_t vfget_filelen( void *user_data )
// {
//     VIO_DATA *vf = (VIO_DATA*)user_data;
//
// 	return vf->length;
// }
//
//
// static sf_count_t vfseek( sf_count_t offset, int whence, void *user_data )
// {
//     VIO_DATA *vf = (VIO_DATA*)user_data;
//
// 	switch ( whence )
// 	{
//         case SEEK_SET:    vf->offset = offset;                  break;
// 		case SEEK_CUR:    vf->offset = vf->offset + offset;     break;
// 		case SEEK_END:    vf->offset = vf->length + offset;     break;
//
// 		default:          break;
// 	}
//
// 	return vf->offset;
// }
//
//
// static sf_count_t vfread( void *ptr, sf_count_t count, void *user_data )
// {
//     VIO_DATA *vf = (VIO_DATA*)user_data;
//
// 	/*
// 	 *	This will brack badly for files over 2Gig in length, but
// 	 *	is sufficient for testing.
// 	 */
//
// 	if ( vf->offset + count > vf->length )
//     {
// 		count = vf->length - vf->offset;
//     }
//
// 	memcpy (ptr, vf->data + vf->offset, count);
//
// 	vf->offset += count;
//
// 	return count;
// }
//
//
// static sf_count_t vfwrite( const void *ptr, sf_count_t count, void *user_data )
// {
//     VIO_DATA *vf = (VIO_DATA*)user_data;
//
// 	/*
// 	 *	This will break badly for files over 2Gig in length, but
// 	 *	is sufficient for testing.
// 	 */
//
// 	if ( vf->offset >= vf->length /*SIGNED_SIZEOF (vf->data)*/ )
// 		return 0;
//
// 	if ( vf->offset + count > vf->length/*SIGNED_SIZEOF (vf->data)*/ )
// 		count = /*sizeof (vf->data)*/ vf->length - vf->offset;
//
// 	memcpy( vf->data + vf->offset, ptr, (size_t)count );
//
// 	vf->offset += count;
//
// 	if ( vf->offset > vf->length )
// 		 vf->length = vf->offset;
//
// 	return count;
// }
//
//
// static sf_count_t vftell( void *user_data )
// {
//     VIO_DATA *vf = (VIO_DATA*)user_data;
//
// 	return vf->offset;
// }









#else // NO_LIBSNDFILE

/******************************************************************************
                          N O   L I B S N D F I L E
*******************************************************************************/


int aafi_extract_audio_essence( AAF_Iface *aafi, aafiAudioEssence *audioEssence, const char *outfilepath, const wchar_t *forcedFileName, int format )
{
  (void)format;
  // SF_VIRTUAL_IO  sfvirtual;
  // VIO_DATA       user_data;
  // SF_INFO        sfinfo;
  // SNDFILE       *infile = NULL;

  if ( audioEssence->is_embedded == 0 ) {
    _error( aafi->ctx.options.verb, "Audio essence is not embedded : nothing to extract.\n" );
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
    _error( aafi->ctx.options.verb, "Could not retrieve audio essence stream from CFB.\n" );
    return -1;
  }



  // /* Open stream with libsndfile */
  //
  // infile = sf_open_virtual( &sfvirtual, SFM_READ, &sfinfo, &user_data );
  //
  // if ( infile == 0 )
  // {
  //   _error( aafi->ctx.options.verb, "libsndfile could not open stream : %s.\n", sf_strerror( NULL ) );
  //
  //   free( user_data.data );
  //
  //   return -1;
  // }



  /* Build file path */

  char filePath[1024];

  snprintf( filePath, 1024, "%s%s%ls.%s",
      outfilepath,
      (*(outfilepath+strlen(outfilepath)-1) != DIR_SEP) ? DIR_SEP_STR : "",
      ( forcedFileName != NULL ) ? forcedFileName : eascii_to_ascii(audioEssence->unique_file_name),
      "wav" );



  FILE *fp = fopen( filePath, "wb" );

  if ( fp == NULL ) {
    _error( aafi->ctx.options.verb, "Could not open audio essence file for writing (%s) : %s\n", filePath, strerror(errno) );
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
      _error( aafi->ctx.options.verb, "Audio essence is bigger than maximum wav size (2^32 bytes) : %"PRIu64" bytes\n", datasz );
      free(data);
      return -1;
    }

    if ( riff_writeWavFileHeader( fp, &wavFmt, &wavBext, (uint32_t)datasz ) < 0 ) {
      _error( aafi->ctx.options.verb, "Could not write wav audio essence header : %s\n", filePath );
    }
  }


  // printf("Writting to file : %c %c %c %c\n", data[0], data[1], data[2], data[3] );

  uint64_t writtenBytes = fwrite( data, sizeof(unsigned char), datasz, fp );

  if ( writtenBytes < datasz ) {
    _error( aafi->ctx.options.verb, "Could not write audio essence file (%"PRIu64" bytes written out of %"PRIu64" bytes) : %s\n", writtenBytes, datasz, filePath );
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
  //   _error( aafi->ctx.options.verb, "Could not open file %s : %s.\n", filePath, sf_strerror( NULL ) );
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
  //   _error ( aafi->ctx.options.verb, "libsndfile could not add bext chunk.\n" );
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

  swprintf( audioEssence->usable_file_path, (strlen(filePath) * sizeof(wchar_t)), L"%s", filePath );



  free( data );

  // sf_close( outfile );
  // sf_close( infile );

  return 0;
}



size_t embeddedAudioDataReaderCallback( unsigned char *buf, size_t offset, size_t reqLen, void *user1, void *user2 ) {

  unsigned char *data = user1;
  size_t datasz = *(size_t*)user2;

  // FILE *fp = (FILE*)user1;
  //
  // fseek( fp, offset, SEEK_SET );
  //
  // return fread( buf, sizeof(unsigned char), reqLen, fp );
  if ( offset >= datasz ) {
    printf( "Error: Requested data starts beyond data length.\n" );
    return -1;
  }

  if ( offset+reqLen >= datasz ) {
    reqLen = datasz - (offset+reqLen);
  }

  memcpy( buf, data+offset, reqLen );

  return reqLen;
}

size_t externalAudioDataReaderCallback( unsigned char *buf, size_t offset, size_t reqLen, void *user1, void *user2 ) {

  (void)user2;

  FILE *fp = (FILE*)user1;

  fseek( fp, offset, SEEK_SET );

  return fread( buf, sizeof(unsigned char), reqLen, fp );
}

int parse_audio_summary( AAF_Iface *aafi, aafiAudioEssence *audioEssence )
{

  // dump_hex( audioEssence->summary->val, audioEssence->summary->len );

  int rc = 0;

  char *externalFilePath = NULL;


  if ( audioEssence->summary == NULL ) {
    _warning( aafi->ctx.options.verb, "TODO: Audio essence has no summary. Trying essence data stream.\n" );
    return -1;
  }


  struct RIFFAudioFile RIFFAudioFile;


  if ( audioEssence->is_embedded ) {

    /*
        Adobe Premiere Pro, embedded mp3/mp4 files converted to PCM/AIFF on export, AAFClassID_AIFCDescriptor, 'COMM' is valid.
        ______________________________ Hex Dump ______________________________

        46 4f 52 4d 00 00 00 32  41 49 46 43 43 4f 4d 4d  |  FORM...2 AIFCCOMM
        00 00 00 26 00 01 00 00  00 00 00 10 40 0e bb 80  |  ........ ........
        00 00 00 00 00 00 4e 4f  4e 45 0e 4e 6f 74 20 43  |  ......NO NE.Not.C
        6f 6d 70 72 65 73 73 65  64 00                    |  ompresse d.
        ______________________________________________________________________
     */

    rc = riff_parseAudioFile( &RIFFAudioFile, &embeddedAudioDataReaderCallback, audioEssence->summary->val, &audioEssence->summary->len );

    if ( rc < 0 ) {
      _warning( aafi->ctx.options.verb, "TODO: Could not parse embedded essence summary. Trying essence data stream.\n" );
      return -1;
    }
  }
  else {

    externalFilePath = locate_external_essence_file( aafi, audioEssence->original_file_path, aafi->ctx.options.media_location );

    if ( externalFilePath == NULL ) {
      _error( aafi->ctx.options.verb, "Could not locate external audio essence file : %ls\n", audioEssence->original_file_path );
      return -1;
    }

    audioEssence->usable_file_path = malloc( (strlen(externalFilePath) + 1) * sizeof(wchar_t) );

    swprintf( audioEssence->usable_file_path, (strlen(externalFilePath) * sizeof(wchar_t)), L"%s", externalFilePath );



    FILE *fp = fopen( externalFilePath, "rb" );

    if ( fp == NULL ) {
      _error( aafi->ctx.options.verb, "Could not open external audio essence file for reading : %s\n", externalFilePath );
      return -1;
    }

    rc = riff_parseAudioFile( &RIFFAudioFile, &externalAudioDataReaderCallback, fp, NULL );

    if ( rc < 0 ) {
      _error( aafi->ctx.options.verb, "TODO: Could not parse external essence file : %s\n", externalFilePath );
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
  // // printf("%i  :  %i\n", aafi->Audio->samplesize, audioEssence->samplesize );
  //
  // if ( aafi->Audio->samplesize >= 0 ) {
    // aafi->Audio->samplesize = ( aafi->Audio->samplesize == 0 || aafi->Audio->samplesize == audioEssence->samplesize ) ? audioEssence->samplesize : -1;
  // }


  return 0;
}

#endif

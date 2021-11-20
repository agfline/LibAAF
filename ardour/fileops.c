#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <time.h>
#include <pthread.h>

#include <regex.h>

/*
https://github.com/luvit/pcre2/blob/master/src/pcre2demo.c
*/
/* gcc ./test_fop.c ./fileops.c -o test_fop -g -lpcre2-8 */
// #define PCRE2_CODE_UNIT_WIDTH 8
// #include <pcre2.h>

#include "fileops.h"



static uint32_t xcrc32( uint32_t init, const unsigned char *buf, int len );


static struct fop_watch_file * fop_watch_remove_file( struct fop_watch *wf, struct fop_watch_file **file );


struct fop_watch * fop_new_watch( struct fop_watch **list, const char *path, enum fop_watch_type type, uint32_t flags, uint8_t completion_timeout )
{
    // if ( fop_file_exists( path, NULL ) != DT_DIR )
    // {
    //     printf( "Cannot watch on something else than a directory.\n" );
    //     return NULL;
    // }

    struct fop_watch *wf = (struct fop_watch*)malloc( sizeof(struct fop_watch)/*, sizeof(char)*/ );

    strncpy( wf->path, path, PATH_MAX );

    wf->path_len = strlen(path);

    if ( path[wf->path_len-1] == DIR_SEP )
        wf->path[--wf->path_len] = 0x00;


    wf->type = type;
    wf->flags = flags;

    wf->completion_timeout = completion_timeout;

    wf->regexp_filters    = NULL;
    wf->extension_filters = NULL;

    wf->filelist = NULL;

    wf->interrupt = 0;
    wf->interrupted = 0;
    wf->thread = 0;
    wf->rc = 0;

    wf->prev = NULL;
    wf->next = NULL;

    wf->_file = NULL;

    if ( list )
    {
      if ( *list == NULL )
      {
        *list = wf;
      }
      else
      {
        struct fop_watch *tmp = *list;

        while( tmp->next != NULL )
          tmp = tmp->next;

        tmp->next = wf;
        wf->prev = tmp;
      }
    }

    return wf;
}

void fop_release_watch( struct fop_watch **wf )
{
    /* free files */

    struct fop_watch_file *wfile = NULL;

    while ( (wfile = (*wf)->filelist) )
    {
      fop_watch_remove_file( *wf, &wfile );
    }


    struct fop_file *file = (*wf)->_file;

    if ( file )
    {
      /* TODO not fully working

      ==2799== 32,816 bytes in 1 blocks are definitely lost in loss record 4 of 4
      ==2799==    at 0x483577F: malloc (vg_replace_malloc.c:299)
      ==2799==    by 0x510DA75: __alloc_dir (opendir.c:118)
      ==2799==    by 0x510DB7C: opendir_tail (opendir.c:69)
      ==2799==    by 0x111A91: _fop_foreach_file.part.3 (fileops.c:890)
      ==2799==    by 0x111CC5: _fop_foreach_file (fileops.c:839)
      ==2799==    by 0x111CC5: fop_manual_scan_poll (fileops.c:300)
      ==2799==    by 0x112523: fop_watch_poll (fileops.c:445)
      ==2799==    by 0x10C2FC: main (main.c:2681)

       */
      for( int level = 0; level < FOP_MAX_SUBFOLDERS_SCAN; level++ )
      {
        if ( file->_dir[level] )
          closedir( file->_dir[level] );
      }
    }

    /* free filters */

    struct fop_watch_filter *filter = NULL;
    struct fop_watch_filter *next   = NULL;

    for ( filter = (*wf)->regexp_filters; filter != NULL; )
    {
        next = filter->next;
        free( filter->str );
        free( filter );
        filter = next;
    }

    for ( filter = (*wf)->extension_filters; filter != NULL; )
    {
        next = filter->next;
        free( filter->str );
        free( filter );
        filter = next;
    }


    struct fop_watch *tmp = (*wf)->next;

    free( *wf );

    *wf = tmp;
}

static struct fop_watch_file * fop_watch_new_file( struct fop_watch *wf, const char *path, const char *name )
{
    struct fop_watch_file *file = (struct fop_watch_file*)malloc( sizeof(struct fop_watch_file)/*, sizeof(char)*/ );

    if ( wf->type == WF_TYPE_MANUAL_SCAN ||
       ( wf->type == WF_TYPE_INOTIFY && wf->flags & WF_NOTIFY_GROW ) )
    {
        file->state = WF_FSTATE_GROWING;
    } else {
        file->state = WF_FSTATE_COMPLETED;
    }

    file->notified = 0;

    snprintf( file->filepath, PATH_MAX, "%s%c%s", path, DIR_SEP, name );
    file->p_path    = file->filepath;
    file->p_subpath = file->filepath + wf->path_len;
    file->p_name    = fop_get_filename( file->filepath );
    file->p_lastsep = file->p_name - 1;
    file->p_ext     = fop_get_fileext( file->p_name );

    // printf("file->p_path : %s\n", file->p_path );
    // printf("file->p_subpath : %s\n", file->p_subpath );
    // printf("file->p_name : %s\n", file->p_name );
    // printf("file->p_lastsep : %s\n", file->p_lastsep );
    // printf("file->p_ext : %s\n", file->p_ext );


    if ( wf->type == WF_TYPE_MANUAL_SCAN )
    {
      file->size = fop_get_filesize( file->filepath );
      file->last_modified = 0;
      file->stopped_growing_since = 0;
      // file->last_check = 0;
      file->last_seen = 0;
    }


    /* add to list */

    if ( wf->filelist )
      wf->filelist->prev = file;

    file->prev = NULL;
    file->next = wf->filelist;
    wf->filelist = file;


    return file;
}

static struct fop_watch_file * fop_watch_remove_file( struct fop_watch *wf, struct fop_watch_file **file )
{
    struct fop_watch_file *prev = (*file)->prev;
    struct fop_watch_file *next = (*file)->next;

    if ( prev )
      prev->next = next;

    if ( next )
      next->prev = prev;

    if ( wf->filelist == (*file) )
      wf->filelist = next;

    free( *file );

    return next;
}

static struct fop_watch_file * fop_watch_get_file( struct fop_watch *wf, const char *name )
{
    struct fop_watch_file *file = wf->filelist;

    while( file != NULL )
    {
      // printf("file->p_name : %s\n", file->p_name );

      if ( strcmp( file->p_name, name ) == 0 )
        return file;

      file = file->next;
    }

    return NULL;
}

int fop_watch_new_filter( struct fop_watch *wf, const char *str, enum fop_watch_filter_type type )
{
    struct fop_watch_filter *filter = (struct fop_watch_filter *)malloc( sizeof(struct fop_watch_filter) );

    filter->str = (char*)malloc( strlen(str) + 1 );
    // strncpy( filter->str, str, strlen(str) + 1 );
    strcpy( filter->str, str );


    switch ( type )
    {
      case WF_FILTER_EXTENSION:   filter->next = wf->extension_filters;   wf->extension_filters = filter;   break;
      case WF_FILTER_REGEXP:      filter->next = wf->regexp_filters;      wf->regexp_filters = filter;      break;
      default: break;
    }

    return 0;
}

static int regexp_match( const char *str, const char *pattern )
{

    regex_t regex;
    int rc;
    char msgbuf[100];
    int match = 0;
    int flags = REG_EXTENDED | REG_ICASE | REG_NOSUB;

    /* Compile regular expression */
    if ( (rc = regcomp( &regex, pattern, flags )) )
    {
        /* TODO in that case, ignore filter ! */
        fprintf( stderr, "Could not compile regex\n" );
        return -1;
    }

    /* Execute regular expression */
    if ( (rc = regexec( &regex, str, 0, NULL, 0 )) == 0 )
    {
        match = 1;
    }
    else if ( rc == REG_NOMATCH )
    {
        match = 0;
    }
    else
    {
        regerror( rc, &regex, msgbuf, sizeof(msgbuf) );
        fprintf( stderr, "Regex match failed: %s\n", msgbuf );
        match = -1;
    }

    regfree( &regex );

    // printf("\n%s || %s     :     %s\n", str, pattern, (match) ? "- Matched -" : "  Not Matched" );


    return match;
}

static void fop_apply_filters( struct fop_watch *wf, struct fop_watch_file *file )
{
    struct fop_watch_filter *filter = NULL;
    int include = 0;

    if ( wf->extension_filters == NULL &&
         wf->regexp_filters    == NULL )
    {
        /* no filter set, include file */
        return;
    }


    for ( filter = wf->extension_filters; filter != NULL; filter = filter->next )
        if ( strcasecmp( file->p_ext, filter->str ) == 0 )
            include++;

    for ( filter = wf->regexp_filters; filter != NULL; filter = filter->next )
        if ( regexp_match( file->p_name, filter->str ) == 1 )
            include++;


    if ( (include  > 0 && (wf->flags & WF_FILTER_EXCLUDE)) ||
         (include == 0 && (wf->flags & WF_FILTER_INCLUDE)) )
    {
        file->state = WF_FSTATE_IGNORED;
    }

}

static int fop_manual_scan_poll( struct fop_watch *wf )
{

    struct fop_file *file = NULL;
    struct fop_watch_file *wfile = NULL;

    wf->_file = file;

    int flags = (wf->flags & WF_INCLUDE_SUBDIRECTORIES) ? SCAN_ALL_SUBFOLDERS : SCAN_ONLY_ROOT;

    time_t now = time(NULL);
    ssize_t size = 0;
    int rc = 0;

    fop_foreach_file( &file, wf->path, flags, rc )
    {
        if ( file->type != DT_REG )
            continue;

        if ( ( wf->flags & WF_IGNORE_HIDDEN_FILES ) &&
             (file->name[0] == '.' || strcmp( file->name, "Thumbs.db" ) == 0) )
        {
            continue;
        }

        wfile = fop_watch_get_file( wf, file->name );

        if ( !wfile )
        {
          wfile = fop_watch_new_file( wf, file->path, file->name );
          wfile->last_seen = now;

          fop_apply_filters( wf, wfile );

          continue;
        }

        wfile->last_seen = now;

        if ( wfile->state == WF_FSTATE_IGNORED )
        {
          continue;
        }



        /* TODO what's that for ? */
        struct stat sb;

        if ( lstat( wfile->filepath, &sb ) != 0 )
        {
            perror("Error");
            return -1;
        }

        /* check last modified to detect overwriten file */
        // if ( wfile->state != WF_FSTATE_GROWING )
        // {
        //     if ( sb.st_mtime > wfile->last_modified )
        //     {
        //         wfile->state = WF_FSTATE_GROWING;
        //         wfile->stopped_growing_since = 0;
        //         wfile->size  = 0;
        //     }
        // }

        if ( wfile->state == WF_FSTATE_COMPLETED )
        {
            continue;
        }

        if ( wfile->state == WF_FSTATE_GROWING )
        {
            size = fop_get_filesize( wfile->filepath );

            // printf("%u  |  %u\n", wfile->size, size );

            if ( size > wfile->size )
            {
                /* keep growing */
                wfile->size = size;
                wfile->stopped_growing_since = 0;
                continue;
            }

            if ( size == wfile->size )
            {
                if ( wfile->stopped_growing_since == 0 )
                {
                    // printf("STOP_GROWINGGGGGGGGGGGGGGGG\n" );
                    wfile->stopped_growing_since = now;
                    continue;
                }

                if ( now > (wfile->stopped_growing_since + wf->completion_timeout) )
                {
                    // printf("time_t    now : %u       sgs : %u\n", now, wfile->stopped_growing_since );
                    size = fop_get_filesize( wfile->filepath );

                    if ( size > wfile->size )
                    {
                        /* keep growing */
                        printf("!!!!!!!!!!!!!!!!!! keep growing\n" );
                        wfile->size = size;
                        wfile->stopped_growing_since = 0;
                        continue;
                    }

                    // printf("COMPLE\n", );
                    if ( wfile->notified == 1 )
                    {
                      wfile->last_modified = sb.st_mtime;

                      /* only change state when user has been notified of previous state */
                      wfile->state = WF_FSTATE_COMPLETED;
                      wfile->notified = 0;
                    }
                }
            }
        }
    }

    if ( rc < 0 )
      return -1;


    /* loops through all files to check for deletion */

    for ( wfile = wf->filelist; wfile != NULL; wfile = wfile->next )
    {
      if ( wfile->last_seen != now )
      {
        if ( wfile->notified == 1 )
        {
          wfile->state = WF_FSTATE_REMOVED;
          wfile->notified = 0;
        }
      }
    }

    return 0;
}

int fop_watch_poll( struct fop_watch *wf, struct fop_watch_file **file )
{
    /* at that point, *file contains the last file that was seen by user loop */
    if ( *file )
    {
      (*file)->notified = 1;

      if ( (*file)->state == WF_FSTATE_REMOVED )
      {
        *file = fop_watch_remove_file( wf, file ); // set to file->next;
      }
    }


    if ( wf->type == WF_TYPE_MANUAL_SCAN )
    {
      if ( fop_manual_scan_poll( wf ) < 0 )
        return -1;
    }


    if ( *file == NULL )
      *file = wf->filelist;

    while ( *file != NULL )
    {
      if ( (*file)->notified == 0 )
      {
          if ( wf->flags & WF_NOTIFY_GROW        && (*file)->state == WF_FSTATE_GROWING )
              return 1;
          if ( wf->flags & WF_NOTIFY_COMPLETION  && (*file)->state == WF_FSTATE_COMPLETED )
              return 1;
          if ( wf->flags & WF_NOTIFY_DELETION    && (*file)->state == WF_FSTATE_REMOVED )
              return 1;
      }

      /* if user do not subscribe to WF_NOTIFY_DELETION, remove file */
      if ( !(wf->flags & WF_NOTIFY_DELETION) && (*file)->state == WF_FSTATE_REMOVED )
      {
          *file = fop_watch_remove_file( wf, file ); // set to file->next;
          continue;
      }

      *file = (*file)->next;
    }


    return 1;
}


// int thread_callback( struct fop_watch *wf, struct fop_watch_file *file, int pollrc )

static void * fop_watch_thread( void *arg )
{
  struct fop_watch *wf = (struct fop_watch *)arg;

  wf->interrupted = 0;

  int rc = 0;
  uint32_t us = 0;

  struct fop_watch_file *file = NULL;

  while ( 1 )
  {
    pthread_mutex_lock( &wf->lock );

    if ( wf->interrupt )
      break;

    rc = fop_watch_poll( wf, &file );

    us = wf->poll_interval;

    wf->rc = wf->thread_callback( wf, file, rc );

    pthread_mutex_unlock( &wf->lock );


    usleep( us );
  }

  wf->interrupted = 1;

  pthread_mutex_unlock( &wf->lock );

  return NULL;
}



int fop_create_watch_thread( struct fop_watch *wf, uint32_t poll_interval, int (*thread_callback)( struct fop_watch *wf, struct fop_watch_file *file, int pollrc ), void *userdata )
{
  wf->poll_interval = poll_interval * 1000;
  wf->thread_callback = thread_callback;
  wf->userdata = userdata;
  wf->interrupt = 0;
  wf->interrupted = 1;
  wf->rc = 0;

  pthread_mutex_init( &wf->lock, NULL );

  int err = pthread_create( &wf->thread, NULL, &fop_watch_thread, (void*)wf );

  if ( err )
  {
    return err;
  }

  pthread_mutex_lock( &wf->lock );

  pthread_detach( wf->thread );

  pthread_mutex_unlock( &wf->lock );

  return 0;
}



int fop_end_watch_thread( struct fop_watch *wf )
{
  if ( !wf->thread )
    return 0;

  pthread_mutex_lock( &wf->lock );
	wf->interrupt = 1;
  pthread_mutex_unlock( &wf->lock );

	while ( 1 )
	{
    pthread_mutex_lock( &wf->lock );

    if ( wf->interrupted )
      break;

    pthread_mutex_unlock( &wf->lock );

		usleep(200000);
		/* TODO timeout and force ? */
	}

  pthread_mutex_unlock( &wf->lock );

  pthread_mutex_destroy( &wf->lock );


	return 0;
}









// const char * fop_absolute_path( const char *path )
// {
//     static char buf[PATH_MAX+1];
//
//
// }


int fop_file_exists( const char *pathname, struct stat *s )
{
    struct stat sb;

    if ( stat( pathname, &sb ) == -1 )
    {
        switch ( errno )
        {
            case ENOENT:  return  0; /* file does not exists */
            default:      return -1;
        }
    }

    if ( s )
    {
        memcpy( s, &sb, sizeof(struct stat) );
    }

    switch ( sb.st_mode & S_IFMT )
    {
      case S_IFREG:  return DT_REG;  // 8
      case S_IFDIR:  return DT_DIR;  // 4
      case S_IFLNK:  return DT_LNK;  // 10
      case S_IFBLK:  return DT_BLK;  // 6
      case S_IFCHR:  return DT_CHR;  // 2
      case S_IFIFO:  return DT_FIFO; // 1
      case S_IFSOCK: return DT_SOCK; // 12
      default:       break;
    }

    return DT_UNKNOWN; // 0
}


ssize_t fop_get_filesize( const char *file )
{
  FILE *fp = fopen( file, "r" );

  if ( !fp )
    return -1;

  fseek( fp, 0L, SEEK_END );

  ssize_t size = ftell( fp );

  fclose( fp );

  if ( size < 0 )
  {
    return -1;
  }

  return size;
}


const char * fop_get_filename( const char *filepath )
{
    // static char buf[NAME_MAX];
    //
    // strncpy( buf, filepath, sizeof(buf) );
    //
    // return basename( buf );

    const char *end = filepath + strlen(filepath);

    while ( end > filepath && *end != DIR_SEP ) {
        --end;
    }

    // if ((end > filepath && *end == '.') &&
    //     (*(end - 1) != '\\' && *(end - 1) != '/')) {
    //     *end = '\0';
    // }


    return end+1;
}


const char * fop_get_dirname( const char *filepath )
{
    static char buf[NAME_MAX+1];
    char *start = buf;

    strncpy( buf, filepath, NAME_MAX );

    // return dirname( buf );


    char *end = start + strlen(buf);

    while ( end > start && *end != DIR_SEP /**end != '\\' && *end != '/'*/ ) {
        --end;
    }

    if ( end > start && *end == DIR_SEP )
    {
        *end = '\0';
    }

    return start;
}


const char * fop_get_fileext( const char *filepath )
{
    // const char *dot = NULL;
    //
    // if ( (dot = strrchr( filename, '.' )) == NULL || /* filename */
    //       dot == filename ) /* .filename */
    //   return "";
    //
    // return (dot + 1);
    const char *end = filepath + strlen(filepath);

    while ( end > filepath && *end != '.' && *end != DIR_SEP/**end != '\\' && *end != '/'*/ ) {
        --end;
    }

    if ( *end != '.' && end > filepath )
      return end; // TODO check if that points to '\0'

    return ++end;

    // if ((end > fname && *end == '.') &&
    //     (*(end - 1) != '\\' && *(end - 1) != '/')) {
    //     *end = '\0';
    // }
}


const char * fop_trim_fileext( const char *filename )
{
    static char buf[NAME_MAX+1];
    char *dot = NULL;

    strncpy( buf, filename, NAME_MAX );

    if ( (dot = strrchr( buf, '.' )) == NULL || /* filename */
          dot == buf ) /* .filename */
      return buf;

    *dot = 0x00;
    return buf;
}


int fop_new_dir( const char *dir )
{
  // if ( fop_file_exists( dir, NULL ) != 0 )
  //   return -1;

  /* don't test prior so we can check errno */
  return mkdir( dir, 0754 ); // drwxr-xr-x
}


uint32_t fop_compute_file_crc32( const char *file )
{
    /*
      https://github.com/gcc-mirror/gcc/blob/master/libiberty/crc32.c
      https://stackoverflow.com/questions/51240770/how-to-calculate-crc32-over-blocks-that-are-splitted-and-buffered-of-a-large-dat
    */

    char buf[4096];
    ssize_t nread = 0;
    uint32_t crc = 0xffffffff;

    int fd = open( file, O_RDONLY );

    if ( fd < 0 )
    {
      return 0;
    }

    while ( (nread = read( fd, buf, sizeof(buf) )) > 0 )
    {
      crc = xcrc32( crc, (unsigned char*)buf, nread );
    }

    close( fd );

    return crc;
}


int fop_cp( const char *from, const char *to, uint32_t flags )
{
    /*
      https://stackoverflow.com/questions/2180079/how-can-i-copy-a-file-on-unix-using-c
    */

    int fd_to, fd_from;
    char buf[4096];
    ssize_t nread;
    int saved_errno;


    fd_from = open( from, O_RDONLY );

    if ( fd_from < 0 )
    {
      return -1;
    }


    char to_file[PATH_MAX+1];

    /* if output is a directory, build output file path */
    if ( fop_file_exists( to, NULL ) == DT_DIR )
    {
      snprintf( to_file, PATH_MAX, "%s%c%s", to, DIR_SEP, fop_get_filename( from ) );
    }
    else
    {
      snprintf( to_file, PATH_MAX, "%s", to );
    }

    /* (O_CREAT | O_EXCL) fails with EEXIST if file already exists */
    fd_to = open( to_file, O_WRONLY | O_CREAT | ((flags & REPLACE_IF_FILE_EXISTS) ? 0 : O_EXCL), 0666 );

    if ( fd_to < 0 )
    {
      close( fd_from );
      return -1;
    }

    uint32_t crc1 = 0xffffffff;

    while ( (nread = read( fd_from, buf, sizeof(buf) )) > 0 )
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        if ( flags & CRC32_CHECK )
          crc1 = xcrc32( crc1, (unsigned char*)buf, nread );

        do {
            nwritten = write( fd_to, out_ptr, (size_t)nread );

            if ( nwritten >= 0 )
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if ( errno != EINTR )
            {
                goto out_error;
            }
        } while ( nread > 0 );
    }

    if ( nread == 0 )
    {
        if ( close( fd_to ) < 0 )
        {
            fd_to = -1;
            goto out_error;
        }

        close( fd_from );

        if ( flags & CRC32_CHECK )
        {
            uint32_t crc2 = fop_compute_file_crc32( to_file );

            if ( crc1 != crc2 )
            {
              goto out_error;
            }

            // printf( "CRC32 check ok : 0x%08x 0x%08x\n", crc1, crc2 );
        }

        /* Success! */
        return 0;
    }

out_error:

    saved_errno = errno;

    if ( fd_from >= 0 )
        close( fd_from );

    if ( fd_to >= 0 )
        close( fd_to );

    errno = saved_errno;

    if ( flags & REMOVE_OUTPUT_ON_FAILURE )
    {
      remove( to_file );
    }

    return -1;
}


int fop_mv( const char *from, const char *to, uint32_t flags )
{
  /* first, try rename() : standard, quickest, safest */
  int rc = rename( from, to );

  if ( rc < 0 )
  {
    if ( errno != EXDEV )
    {
      return -1;
    }
  }
  else
  {
    return 0;
  }


  /* if rename() fails with EXDEV, the two files are on different fs.
   * fallback on copy then.
   */
  if ( fop_cp( from, to, flags ) )
  {
    /* if copy fails not cause file exists, ensures there is no output file at all */
    /* NOTE this is handled by fop_cp() */
    // if ( errno != EEXIST && flags & REMOVE_OUTPUT_ON_FAILURE )
    //   remove( to );

    return -1;
  }

  remove( from );

  return 0;
}


int _fop_foreach_file( struct fop_file **file, const char *basepath, int scandepth )
{
  /*
    https://codeforwin.org/2018/03/c-program-to-list-all-files-in-a-directory-recursively.html
  */
  int ret = 0;
start:

    if ( *file && (*file)->_dir[0] == NULL )
    { /* we're done */
end:
      free( *file );
      *file = NULL;
      return ret;
    }
    else if ( !*file )
    { /* first call, we start the scan */
      *file = (struct fop_file*)calloc( sizeof(struct fop_file), sizeof(unsigned char) );
      (*file)->type = -1;
    }

    if ( (*file)->_dir[(*file)->_level] == NULL ||
         ((*file)->type == DT_DIR &&
            ( scandepth > SCAN_ONLY_ROOT && (*file)->_level < scandepth && (*file)->_level < FOP_MAX_SUBFOLDERS_SCAN )) )
    {
      /*
       *  If this is the first call OR
       *  If this is a directory AND we're going down subfolders (not SCAN_ONLY_ROOT/0) AND we havent yet reached a limit
       */

      char path[PATH_MAX+1];

      // if ( (*file)->type == DT_DIR /*&& ((*file)->_level >= scandepth || (*file)->_level >= FOP_MAX_SUBFOLDERS_SCAN)*/ )
      // { /* do not enter a folder if we have reached depth limit */
      //   goto next;
      // }
      if ( (*file)->type == DT_DIR )
      { /* enter a new dir */
        if ( (unsigned int)snprintf( path, sizeof(path), "%s%c%s", (*file)->_path[(*file)->_level], DIR_SEP, (*file)->name ) >= sizeof(path) )
        {
          /* filepath bigger than PATH_MAX */
          return -1; // TODO better handling of exception : do not enter that dir
        }

        (*file)->_level++;
      }
      else
      { /* first call, set first directory to scan */
        snprintf( path, sizeof(path), "%s", basepath );

        /* removes trailing '/' if any */
        if ( path[strlen(path)-1] == DIR_SEP )
          path[strlen(path)-1] = 0x00;
      }

      /* open directory */
      (*file)->_dir[(*file)->_level] = opendir( path );

      if ( !(*file)->_dir[(*file)->_level] )
      {
        ret = -1;
        goto end;
      }

      /* set current path */
      strncpy( (*file)->_path[(*file)->_level], path, PATH_MAX+1 );
      (*file)->path = (*file)->_path[(*file)->_level];
    }


    struct dirent *dp;
    errno = 0;

    while ( (dp = readdir( (*file)->_dir[(*file)->_level] )) != NULL )
    {
        char *name = dp->d_name;

        if ( ( name[0] == '.' && name[1] == 0x00 ) ||
             ( name[0] == '.' && name[1] == '.' && name[2] == 0x00 ) )
        {
          continue;
        }

        /*
         *  got a file
         */

        strncpy( (*file)->name, name, NAME_MAX+1 );
        (*file)->type = dp->d_type;

        return 1;
    }

    if ( errno != 0 )
    {
      /* readdir() error */
      ret = -1;
      goto end;
    }

    /*
     *  get back to previous directory level
     */

// next:

    /* in case a directory is empty, so we do not run into that directory again in an infinite loop */
    (*file)->type = DT_REG;

    /* close current directory */
    closedir( (*file)->_dir[(*file)->_level] );
    (*file)->_dir[(*file)->_level] = NULL;

    (*file)->_level--;

    /* set current path to previous level's path */
    (*file)->path = (*file)->_path[(*file)->_level];


    goto start;
}


static const unsigned int crc32_table[] =
{
  /*
    https://github.com/gcc-mirror/gcc/blob/master/libiberty/crc32.c
  */
  0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9,
  0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
  0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
  0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
  0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9,
  0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
  0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011,
  0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
  0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
  0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
  0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81,
  0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
  0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49,
  0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
  0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
  0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
  0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae,
  0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
  0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16,
  0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
  0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
  0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
  0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066,
  0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
  0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e,
  0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
  0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
  0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
  0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e,
  0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
  0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686,
  0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
  0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
  0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
  0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f,
  0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
  0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47,
  0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
  0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
  0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
  0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7,
  0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
  0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f,
  0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
  0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
  0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
  0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f,
  0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
  0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640,
  0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
  0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
  0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
  0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30,
  0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
  0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088,
  0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
  0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
  0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
  0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18,
  0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
  0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0,
  0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
  0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
  0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};


static uint32_t xcrc32( uint32_t init, const unsigned char *buf, int len )
{
  /*
    https://github.com/gcc-mirror/gcc/blob/master/libiberty/crc32.c
  */
  uint32_t crc = init;
  while (len--)
    {
      crc = (crc << 8) ^ crc32_table[((crc >> 24) ^ *buf) & 255];
      buf++;
    }
  return crc;
}

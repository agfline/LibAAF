#ifndef __fileops_h__
#define __fileops_h__

#include <limits.h>
#include <dirent.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

#define FOP_MAX_SUBFOLDERS_SCAN 32
#define DIR_SEP '/'


enum fop_flags {
  CRC32_CHECK              = (1<<1),
  REPLACE_IF_FILE_EXISTS   = (1<<2), /* TODO */
  REMOVE_OUTPUT_ON_FAILURE = (1<<3), /* TODO */
};

enum fop_scandepth {
  SCAN_ONLY_ROOT = 0,
  SCAN_ALL_SUBFOLDERS = FOP_MAX_SUBFOLDERS_SCAN,
  /* all other positive values specifies the explicit number of sublevels to scan */
};

struct fop_file {

  /* NOTE: 131640 Bytes */

  char           name[NAME_MAX+1];
  char          *path; // points to the corresponding _path[]
  int            type;

  DIR        *_dir[FOP_MAX_SUBFOLDERS_SCAN+1];
  char       _path[FOP_MAX_SUBFOLDERS_SCAN+1][PATH_MAX+1];
  uint8_t    _level;
};






enum fop_watch_fstate {
  WF_FSTATE_IGNORED = 1,
  WF_FSTATE_GROWING,
  WF_FSTATE_COMPLETED,
  WF_FSTATE_REMOVED,
};

struct fop_watch_file {

  char    filepath[PATH_MAX+1];

  char    *p_path;    // ptr to filepath :  ^ /watch/folder/path/subpath '\0' file.ext
  char    *p_subpath; // ptr to filepath : /watch/folder/path/ ^ subpath '\0' file.ext
  const char    *p_lastsep; // ptr to filepath : /watch/folder/path/subpath ^ '\0' file.ext
  const char    *p_name;    // ptr to filepath : /watch/folder/path/subpath '\0' ^ file.ext
  const char    *p_ext;     // ptr to filepath : /watch/folder/path/subpath '\0' file. ^ ext


  /* Growing watch */
  ssize_t          size;            // current file size
  // struct timespec last_modified;   // last time the file was modified.
  time_t          last_modified;   // last time the file was modified.
  time_t          stopped_growing_since; // when the file first ended its growth. If it's not growing anymore up to wf->completion_timeout, then file is completed.
  // time_t          last_check;      // last time we checked the file.

  /* check for deletion */
  time_t          last_seen;

  enum fop_watch_fstate  state;    // IGNORED, GROWING, COMPLETE, REMOVED
  uint8_t                notified; // once state has changed, and has been seen in poll loop.

  struct fop_watch_file *prev;
  struct fop_watch_file *next;
};

enum fop_watch_type {
  WF_TYPE_INOTIFY     = 1,
  WF_TYPE_MANUAL_SCAN,

  WF_TYPE_MOUNTED_FS,
  WF_TYPE_SFTP,
  WF_TYPE_FTP,
};

enum fop_watch_flags {
  WF_NOTIFY_GROW            = (1<<1),
  WF_NOTIFY_COMPLETION      = (1<<2),
  WF_NOTIFY_DELETION        = (1<<3),

  WF_FILTER_INCLUDE         = (1<<6),
  WF_FILTER_EXCLUDE         = (1<<7),

  WF_IGNORE_HIDDEN_FILES    = (1<<10), // all dot-files AND Thumbs.db
  WF_INCLUDE_SUBDIRECTORIES = (1<<11),
};

enum fop_watch_filter_type {
  // WF_FILTER_REG_INCLUSION = 1,
  // WF_FILTER_REG_EXCLUSION,
  // WF_FILTER_EXT_INCLUSION,
  // WF_FILTER_EXT_EXCLUSION,
  WF_FILTER_EXTENSION = 1,
  WF_FILTER_REGEXP
};

struct fop_watch_filter {
  char                    *str;
  struct fop_watch_filter *next;
};

struct fop_watch {

  char path[PATH_MAX+1];
  int  path_len;

  enum fop_watch_type   type;
  uint32_t              flags; // enum fop_watch_flags

  /* WF_TYPE_MANUAL_SCAN */
  uint8_t  completion_timeout; // seconds

  // struct fop_watch_filter *reg_inclusion;
  // struct fop_watch_filter *reg_exclusion;
  // struct fop_watch_filter *ext_inclusion;
  // struct fop_watch_filter *ext_exclusion;

  struct fop_watch_filter *extension_filters;
  struct fop_watch_filter *regexp_filters;


  // time_t last_scan; // updated each time we call fop_manual_scan_poll()

  struct fop_watch_file   *filelist;

  struct fop_file *_file; // TODO keep track of current file within fop_foreach_file() so we can clear it on quit


  pthread_t         thread; // thread of the current watchfolder when using thread
  int             (*thread_callback)( struct fop_watch *wf, struct fop_watch_file *file, int pollrc );
  pthread_mutex_t   lock; // protect struct fop_watch when using thread
  void             *userdata; // passing to callback when using thread
  uint32_t          poll_interval; // folder poll interval in us when using thread
  int               interrupt; // order to interrupt thread
  int               interrupted; // notification of thread interruption
  int               rc; // callback return code. Does someone care ?

  struct fop_watch *prev; // for user implemented watchfolder list
  struct fop_watch *next; // for user implemented watchfolder list
};











struct fop_watch * fop_new_watch( struct fop_watch **list, const char *path, enum fop_watch_type type, uint32_t flags, uint8_t completion_timeout );
void fop_release_watch( struct fop_watch **wf );
int fop_watch_new_filter( struct fop_watch *wf, const char *str, enum fop_watch_filter_type type );

int fop_watch_poll( struct fop_watch *wf, struct fop_watch_file **file );



int fop_create_watch_thread( struct fop_watch *wf, uint32_t interval, int (*thread_callback)( struct fop_watch *wf, struct fop_watch_file *file, int pollrc ), void *userdata );
int fop_end_watch_thread( struct fop_watch *wf );



/**
 *	Test if a file exists and return its type.
 *
 *	@param  pathname   The file to test.
 *	@param  s          Optional : pointer to a stat structure where function will memcpy its stat, otherwise NULL if you don't care.
 *
 *	@return            > 0 if file exists, return value is therfore a valid DT_TYPE value (see dirent.h)
 *                     -1 on error and errno set by stat()
 */

int fop_file_exists( const char *pathname, struct stat *s );


/**
 *	Retrieve a file's size.
 *
 *	@param  file   Filepath of the targeted file
 *
 *	@return        the file size on success
 *                 -1 on error and errno set
 */

ssize_t fop_get_filesize( const char *file );


const char * fop_get_filename( const char *filepath );

const char * fop_get_dirname( const char *filepath );

const char * fop_get_fileext( const char *filename );

const char * fop_trim_fileext( const char *filename );


int fop_new_dir( const char *dir );


/**
 *	Compute the CRC32 checksum of a file.
 *
 *	@param  file   Filepath of the targeted file
 *
 *	@return        crc32 checksum on success
 *                 -1 (0xffffffff) on error and errno set
 */

uint32_t fop_compute_file_crc32( const char *file );



/*
  TODO implemente rc value, so we can check for error, or implement as file->rc

  #define fop_foreach_file( file, basepath, scandepth, rc )\
    while ( (rc = _fop_foreach_file( file, basepath, scandepth )) == 1 )
*/
#define fop_foreach_file( file, basepath, scandepth, rc )\
  while ( (rc = _fop_foreach_file( file, basepath, scandepth )) == 1 )

/**
 *	Loop through all files and directory contained in basepath.
 *
 *  \note
 *  This function shall only be used through the fop_foreach_file() macro.
 *
 *	@param  file       Pointer to pointer to unallocated NULL struct fop_file. Allocation and free are handled by _fop_foreach_file().
 *	@param  basepath   Path to the folder to scan.
 *	@param  scandepth  The number of subfolder levels to scan, bitween 0 (SCAN_ONLY_ROOT) and FOP_MAX_SUBFOLDERS_SCAN (SCAN_ALL_SUBFOLDERS).
 *
 *	@return             1 while there is a file
 *                      0 when finished
 *                     -1 on error
 */

int _fop_foreach_file( struct fop_file **file, const char *basepath, int scandepth );


/**
 *	Copy a source file to a destination file.
 *
 *	@param  to     Filepath of the destination file
 *	@param  from   Filepath of the source file
 *	@param  flags  Can take CRC32_CHECK | REPLACE_IF_FILE_EXISTS | REMOVE_OUTPUT_ON_FAILURE
 *
 *	@return         0 on success
 *                 -1 on error and errno set
 */

int fop_cp( const char *from, const char *to, uint32_t flags );


/**
 *	Move a source file to a destination file.
 *
 *	@param  to     Filepath of the destination file
 *	@param  from   Filepath of the source file
 *	@param  flags  Can take CRC32_CHECK | REPLACE_IF_OUTPUT_EXISTS | REMOVE_OUTPUT_ON_FAILURE
 *
 *	@return         0 on success
 *                 -1 on error and errno set
 */

int fop_mv( const char *from, const char *to, uint32_t flags );


#endif // !__fileops_h__

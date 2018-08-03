#ifndef __utils_h__
#define __utils_h__


#ifndef _WIN32
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[92m"
#define ANSI_COLOR_YELLOW  "\x1b[93m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#else
#define ANSI_COLOR_RED     ""
#define ANSI_COLOR_GREEN   ""
#define ANSI_COLOR_YELLOW  ""
#define ANSI_COLOR_BLUE    ""
#define ANSI_COLOR_MAGENTA ""
#define ANSI_COLOR_CYAN    ""
#define ANSI_COLOR_RESET   ""
#endif



wchar_t * eascii_to_ascii( wchar_t *str );

wchar_t * w16tow32( wchar_t *w32buf, uint16_t *w16buf, size_t w16len );

void dump_hex( const unsigned char * stream, size_t stream_sz );

char * url_decode( char *dst, char *src );

wchar_t * wurl_decode( wchar_t *dst, wchar_t *src );

#endif // ! __utils_h__

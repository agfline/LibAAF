#ifndef __utils_h__
#define __utils_h__

#define ANSI_COLOR_RED      "\033[38;5;124m" //"\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[92m"
#define ANSI_COLOR_YELLOW   "\x1b[33m" //"\x1b[93m"
#define ANSI_COLOR_ORANGE   "\033[38;5;130m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"
#define ANSI_COLOR_CYAN     "\033[38;5;81m" //"\x1b[36m"
#define ANSI_COLOR_DARKGREY "\x1b[38;5;242m"
#define ANSI_COLOR_BOLD     "\x1b[1m"
#define ANSI_COLOR_RESET    "\x1b[0m"



size_t utf16toa( char *astr, uint16_t alen, uint16_t *wstr, uint16_t wlen );
wchar_t * atowchar( const char *astr, uint16_t alen );


wchar_t * eascii_to_ascii( wchar_t *str );

char *remove_file_ext (char* myStr, char extSep, char pathSep);

wchar_t * w16tow32( wchar_t *w32buf, uint16_t *w16buf, size_t w16len );

void dump_hex( const unsigned char * stream, size_t stream_sz );

char * url_decode( char *dst, char *src );

wchar_t * wurl_decode( wchar_t *dst, wchar_t *src );

#endif // ! __utils_h__

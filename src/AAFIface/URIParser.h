/*
 * Copyright (C) 2023 Adrien Gesta-Fline
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

#ifndef URI_PARSER_H
#define URI_PARSER_H


#define MAX_URI_LENGTH 64000

enum uri_option {
  URI_OPT_NONE            =     0,
  URI_OPT_IGNORE_USERPASS = (1<<0),
  URI_OPT_IGNORE_QUERY    = (1<<1),
  URI_OPT_IGNORE_FRAGMENT = (1<<2),

  URI_OPT_DECODE_HOSTNAME = (1<<3),
  URI_OPT_DECODE_USERINFO = (1<<4),
  URI_OPT_DECODE_USERPASS = (1<<5),
  URI_OPT_DECODE_PATH     = (1<<6),
  URI_OPT_DECODE_QUERY    = (1<<7),
  URI_OPT_DECODE_FRAGMENT = (1<<8)
};

#define URI_OPT_DECODE_ALL (\
  URI_OPT_DECODE_HOSTNAME |\
  URI_OPT_DECODE_USERINFO |\
  URI_OPT_DECODE_USERPASS |\
  URI_OPT_DECODE_PATH |\
  URI_OPT_DECODE_QUERY |\
  URI_OPT_DECODE_FRAGMENT)


enum uri_type {

  // URI_T_LOCAL              = (1<<0),
  // URI_T_REMOTE             = (1<<1),

  URI_T_GUESSED_OS_LINUX   = (1<<2),
  URI_T_GUESSED_OS_APPLE   = (1<<3),
  URI_T_GUESSED_OS_WINDOWS = (1<<4),

  URI_T_HOST_EMPTY         = (1<<5),
  URI_T_HOST_IPV4          = (1<<6),
  URI_T_HOST_IPV6          = (1<<7),
  URI_T_HOST_REGNAME       = (1<<8),

  URI_T_LOCALHOST          = (1<<9),
};

#define URI_T_GUESSED_OS_MASK (URI_T_GUESSED_OS_LINUX | URI_T_GUESSED_OS_APPLE | URI_T_GUESSED_OS_WINDOWS)
#define URI_T_HOST_MASK (URI_T_HOST_EMPTY | URI_T_HOST_IPV4 | URI_T_HOST_IPV6 | URI_T_HOST_REGNAME)



// // empty authority (thus empty host) is only valid for 'file:' scheme and a few others like 'tel:'
// // 'file:' scheme empty authority means 'localhost' means local machine
// // https://datatracker.ietf.org/doc/html/rfc3986
// enum uri_slash_type {
//
//   URI_SLASH_T_UNKNOWN = 0,
//
//   /*
//    * RFC 8089
//    * minimal local absolute path with no authority
//    * windows 'file' scheme only (with letter drives, eg. file:c:/path/)
//    * https://datatracker.ietf.org/doc/html/rfc8089#appendix-E.2
//    */
//   URI_SLASH_T_NO_SLASH, /* "" */
//
//   /*
//    * RFC 8089
//    * minimal local absolute path with no authority
//    * 'file' scheme only
//    * https://datatracker.ietf.org/doc/html/rfc8089#appendix-E.2
//    */
//   URI_SLASH_T_1_SLASH, /* "/" */
//
//   /*
//    * RFC 1738
//    * remote UNC file with explicit authority. valid for all schemes
//    * « The scheme specific data start with a double slash "//"
//    * to indicate that it complies with the common Internet scheme
//    * syntax. »
//    * https://datatracker.ietf.org/doc/html/rfc1738#section-3.1
//    * https://datatracker.ietf.org/doc/html/rfc8089#appendix-E.3.1
//    */
// 	URI_SLASH_T_2_SLASH, /* "//" */
//
//   /*
//    * local absolute path with empty authority
//    * 'file' scheme only
//    * https://datatracker.ietf.org/doc/html/rfc8089#appendix-B
//    */
// 	URI_SLASH_T_3_SLASH, /* "///" */
//
//   /*
//    * RFC 8089
//    * remote UNC path with empty authority
//    * https://datatracker.ietf.org/doc/html/rfc8089#appendix-E.3.2
//    * https://datatracker.ietf.org/doc/html/rfc8089#appendix-F
//    */
// 	URI_SLASH_T_4_SLASH, /* "////" */
//
//   /*
//    * RFC 8089
//    * 'file' scheme only
//    * remote UNC path with empty authority with additional slash
//    * https://datatracker.ietf.org/doc/html/rfc8089#appendix-E.3.2
//    * https://datatracker.ietf.org/doc/html/rfc1738#section-3.11
//    * https://datatracker.ietf.org/doc/html/rfc8089#appendix-F
//    */
// 	URI_SLASH_T_5_SLASH, /* "/////" */
//
//   /*
//    * Windows specific « DOS device path »
//    * 'file' scheme only
//    * https://learn.microsoft.com/en-us/dotnet/standard/io/file-path-formats#dos-device-paths
//    */
// 	URI_SLASH_T_DOS_DEV_DOT, /* "//./" */
//
//   /*
//    * Windows specific « DOS device path »
//    * 'file' scheme only
//    * https://learn.microsoft.com/en-us/dotnet/standard/io/file-path-formats#dos-device-paths
//    */
// 	URI_SLASH_T_DOS_DEV_QUESTION_MARK, /* "//?/" */
// };



enum uri_scheme_type {
  URI_SCHEME_T_UNKNOWN = 0,
  URI_SCHEME_T_AFP,
	URI_SCHEME_T_CIFS,
	URI_SCHEME_T_DATA,
	URI_SCHEME_T_DNS,
	URI_SCHEME_T_FILE,
	URI_SCHEME_T_FTP,
	URI_SCHEME_T_HTTP,
	URI_SCHEME_T_HTTPS,
	URI_SCHEME_T_IMAP,
	URI_SCHEME_T_IRC,
	URI_SCHEME_T_MAILTO,
	URI_SCHEME_T_NFS,
	URI_SCHEME_T_POP,
	URI_SCHEME_T_RTSP,
	URI_SCHEME_T_SFTP,
	URI_SCHEME_T_SIP,
	URI_SCHEME_T_SMB,
	URI_SCHEME_T_SSH,
  URI_SCHEME_T_TEL,
	URI_SCHEME_T_TELNET,
};



struct uri {

  char *scheme;
  char *authority;
  char *userinfo;
  char *user;
  char *pass;
  char *host;
  int   port;
  char *path;
  char *query;
  char *fragment;

  // enum uri_slash_type slash_t;
  enum uri_scheme_type scheme_t;

  enum uri_option opts;
  enum uri_type flags;
};



struct uri * uriParse( const char *, enum uri_option );
char * uriDecodeString( char *str );
void uriFree( struct uri * );

int isIPv4( const char *s, int size, char **err );
int isIPv6( const char *s, int size, char **err );

#endif // ! URI_PARSER_H

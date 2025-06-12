/*
 * Copyright (C) 2017-2024 Adrien Gesta-Fline
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
#include <stdlib.h>

#include "../../src/AAFIface/URIParser.h"
#include "common.h"


static int  _uri_cmp( const struct uri *a, const struct uri *b );
static void _uri_dump_diff( struct uri *a, struct uri *b, int totalDifferencies );
static int  _uri_test( const char *uristr, enum uri_option optflags, struct uri expectedRes, int line );


static int _uri_cmp( const struct uri *a, const struct uri *b ) {

	int differenciesCount = 0;

	if ( a == NULL || b == NULL ) {
		return -1;
	}

	if ( (strcmp((a->scheme)   ? a->scheme   : "", (b->scheme)   ? b->scheme   : "") != 0 ) ) { differenciesCount++; }
	if ( (strcmp((a->userinfo) ? a->userinfo : "", (b->userinfo) ? b->userinfo : "") != 0 ) ) { differenciesCount++; }
	if ( (strcmp((a->user)     ? a->user     : "", (b->user)     ? b->user     : "") != 0 ) ) { differenciesCount++; }
	if ( (strcmp((a->pass)     ? a->pass     : "", (b->pass)     ? b->pass     : "") != 0 ) ) { differenciesCount++; }
	if ( (strcmp((a->host)     ? a->host     : "", (b->host)     ? b->host     : "") != 0 ) ) { differenciesCount++; }
	if ( (strcmp((a->path)     ? a->path     : "", (b->path)     ? b->path     : "") != 0 ) ) { differenciesCount++; }
	if ( (strcmp((a->query)    ? a->query    : "", (b->query)    ? b->query    : "") != 0 ) ) { differenciesCount++; }
	if ( (strcmp((a->fragment) ? a->fragment : "", (b->fragment) ? b->fragment : "") != 0 ) ) { differenciesCount++; }
	if ( a->port     != b->port     ) { differenciesCount++; }
	if ( a->scheme_t != b->scheme_t ) { differenciesCount++; }
	if ( a->flags    != b->flags    ) { differenciesCount++; }

	return differenciesCount;
}



static void _uri_dump_diff( struct uri *a, struct uri *b, int totalDifferencies ) {

	int differenciesCount = 0;

	if ( a == NULL || b == NULL ) {
		return;
	}

	if ( (strcmp((a->scheme) ? a->scheme : "", (b->scheme) ? b->scheme : "") != 0 ) ) {
	  TEST_LOG( "      \x1b[38;5;242m"TREE_LINE"\x1b[0m   \x1b[38;5;124m%s .scheme : \"%s\" (expected: \"%s\")\n", (++differenciesCount < totalDifferencies) ? TREE_ENTRY : TREE_LAST_ENTRY, a->scheme, b->scheme );
	}
	if ( (strcmp((a->userinfo) ? a->userinfo : "", (b->userinfo) ? b->userinfo : "") != 0 ) ) {
		TEST_LOG( "      \x1b[38;5;242m"TREE_LINE"\x1b[0m   \x1b[38;5;124m%s .userinfo : \"%s\" (expected: \"%s\")\n", (++differenciesCount < totalDifferencies) ? TREE_ENTRY : TREE_LAST_ENTRY, a->userinfo, b->userinfo );
	}
	if ( (strcmp((a->user) ? a->user : "", (b->user) ? b->user : "") != 0 ) ) {
		TEST_LOG( "      \x1b[38;5;242m"TREE_LINE"\x1b[0m   \x1b[38;5;124m%s .user : \"%s\" (expected: \"%s\")\n", (++differenciesCount < totalDifferencies) ? TREE_ENTRY : TREE_LAST_ENTRY, a->user, b->user );
	}
	if ( (strcmp((a->pass) ? a->pass : "", (b->pass) ? b->pass : "") != 0 ) ) {
		TEST_LOG( "      \x1b[38;5;242m"TREE_LINE"\x1b[0m   \x1b[38;5;124m%s .pass : \"%s\" (expected: \"%s\")\n", (++differenciesCount < totalDifferencies) ? TREE_ENTRY : TREE_LAST_ENTRY, a->pass, b->pass );
	}
	if ( (strcmp((a->host) ? a->host : "", (b->host) ? b->host : "") != 0 ) ) {
		TEST_LOG( "      \x1b[38;5;242m"TREE_LINE"\x1b[0m   \x1b[38;5;124m%s .host : \"%s\" (expected: \"%s\")\n", (++differenciesCount < totalDifferencies) ? TREE_ENTRY : TREE_LAST_ENTRY, a->host, b->host );
	}
	if ( (strcmp((a->path) ? a->path : "", (b->path) ? b->path : "") != 0 ) ) {
		TEST_LOG( "      \x1b[38;5;242m"TREE_LINE"\x1b[0m   \x1b[38;5;124m%s .path : \"%s\" (expected: \"%s\")\n", (++differenciesCount < totalDifferencies) ? TREE_ENTRY : TREE_LAST_ENTRY, a->path, b->path );
	}
	if ( (strcmp((a->query) ? a->query : "", (b->query) ? b->query : "") != 0 ) ) {
		TEST_LOG( "      \x1b[38;5;242m"TREE_LINE"\x1b[0m   \x1b[38;5;124m%s .query : \"%s\" (expected: \"%s\")\n", (++differenciesCount < totalDifferencies) ? TREE_ENTRY : TREE_LAST_ENTRY, a->query, b->query );
	}
	if ( (strcmp((a->fragment) ? a->fragment : "", (b->fragment) ? b->fragment : "") != 0 ) ) {
		TEST_LOG( "      \x1b[38;5;242m"TREE_LINE"\x1b[0m   \x1b[38;5;124m%s .fragment : \"%s\" (expected: \"%s\")\n", (++differenciesCount < totalDifferencies) ? TREE_ENTRY : TREE_LAST_ENTRY, a->fragment, b->fragment );
	}

	if ( a->port != b->port ) {
		TEST_LOG( "      \x1b[38;5;242m"TREE_LINE"\x1b[0m   \x1b[38;5;124m%s .port : %i (expected: %i)\n", (++differenciesCount < totalDifferencies) ? TREE_ENTRY : TREE_LAST_ENTRY, a->port, b->port );
	}
	if ( a->scheme_t != b->scheme_t ) {
		TEST_LOG( "      \x1b[38;5;242m"TREE_LINE"\x1b[0m   \x1b[38;5;124m%s .scheme_t : %i (expected: %i)\n", (++differenciesCount < totalDifferencies) ? TREE_ENTRY : TREE_LAST_ENTRY, a->scheme_t, b->scheme_t );
	}
	if ( a->flags != b->flags ) {
		TEST_LOG( "      \x1b[38;5;242m"TREE_LINE"\x1b[0m   \x1b[38;5;124m%s .flags : %i (expected: %i)\n", (++differenciesCount < totalDifferencies) ? TREE_ENTRY : TREE_LAST_ENTRY, a->flags, b->flags );
	}
}



static int _uri_test( const char *uristr, enum uri_option optflags, struct uri expectedRes, int line ) {

	struct uri *uri = laaf_uri_parse( uristr, optflags, NULL );

	int differenciesCount = 0;

	if ( (differenciesCount = _uri_cmp( uri, &expectedRes )) == 0 ) {
		TEST_LOG( TEST_PASSED_STR "laaf_uri_parse(): %s\n", line, uristr );
	}
	else {

		TEST_LOG( TEST_ERROR_STR "laaf_uri_parse(): %s\n", line, uristr );

		TEST_LOG( "\x1b[38;5;124m"); // red
		_uri_dump_diff( uri, &expectedRes, differenciesCount );
		TEST_LOG( "\x1b[0m");

		TEST_LOG( "      \x1b[38;5;242m"TREE_LINE"\x1b[0m\n");
	}

	laaf_uri_free(uri);

	return differenciesCount;
}



int main( int argc, char *argv[] ) {

	(void)argc;
	(void)argv;

#ifdef _WIN32
	INIT_WINDOWS_CONSOLE()
#endif

	SET_LOCALE()

	TEST_LOG("\n");

	int errors = 0;

#ifdef __GNUC__
	#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
#endif

	errors += _uri_test( "", URI_OPT_NONE, (struct uri){ .scheme = NULL, .scheme_t = URI_SCHEME_T_UNKNOWN, .host = NULL, .port = 0, .path = NULL, .query = NULL, .fragment = NULL }, __LINE__ );
	errors += _uri_test( "https://www.server.com/PT_UTF-8_%D8%A7%D9%84%D9%81%D9%88%D8%B6%D9%89.aaf", URI_OPT_DECODE_ALL, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 0, .path = "/PT_UTF-8_الفوضى.aaf", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 0, .path = NULL, .query = NULL, .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://user:pass@www.server.com", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .userinfo = "user:pass", .user = "user", .pass = "pass", .host = "www.server.com", .port = 0, .path = NULL, .query = NULL, .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "HTTPS://www.server.com", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 0, .path = NULL, .query = NULL, .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "hTtPs://www.server.com", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 0, .path = NULL, .query = NULL, .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com:8080", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 8080, .path = NULL, .query = NULL, .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com:8080?foo=bar", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 8080, .path = NULL, .query = "foo=bar", .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com:8080#anchor", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 8080, .path = NULL, .query = NULL, .fragment = "anchor", .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com/", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 0, .path = "/", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com/?foo=bar", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 0, .path = "/", .query = "foo=bar", .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com/////?foo=bar", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 0, .path = "/", .query = "foo=bar", .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com///////", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 0, .path = "/", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com?foo=bar", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 0, .path = NULL, .query = "foo=bar", .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com#anchor", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 0, .path = NULL, .query = NULL, .fragment = "anchor", .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com/path/to/file.html?foo=bar&foo2=bar2#anchor", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 0, .path = "/path/to/file.html", .query = "foo=bar&foo2=bar2", .fragment = "anchor", .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com:80/", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 80, .path = "/", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com:/", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 0, .path = "/", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com:", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 0, .path = "", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );

	errors += _uri_test( "https://[8:3:1:2:1234:5678::]:8080/ipv6", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "8:3:1:2:1234:5678::", .port = 8080, .path = "/ipv6", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_IPV6 }, __LINE__ );
	errors += _uri_test( "https://[2001:db8:0:85a3::ac1f:8001]:8080/ipv6", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "2001:db8:0:85a3::ac1f:8001", .port = 8080, .path = "/ipv6", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_IPV6 }, __LINE__ );
	errors += _uri_test( "https://user:pass@[2001:db8:3333:4444:5555:6666:1.2.3.4]:8080/ipv6", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .userinfo = "user:pass", .user = "user", .pass = "pass", .host = "2001:db8:3333:4444:5555:6666:1.2.3.4", .port = 8080, .path = "/ipv6", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_IPV6 }, __LINE__ );
	errors += _uri_test( "https://192.168.0.1:8080/ipv4", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "192.168.0.1", .port = 8080, .path = "/ipv4", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_IPV4 }, __LINE__ );
	errors += _uri_test( "https://127.0.0.1:8080/ipv4loopback", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "127.0.0.1", .port = 8080, .path = "/ipv4loopback", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_IPV4 | URI_T_LOCALHOST }, __LINE__ );
	errors += _uri_test( "https://localhost:8080/loopback", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "localhost", .port = 8080, .path = "/loopback", .query = NULL, .fragment = NULL, .flags = URI_T_LOCALHOST }, __LINE__ );
	errors += _uri_test( "https://[0:0:0:0:0:0:0:1]:8080/ipv6loopback", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "0:0:0:0:0:0:0:1", .port = 8080, .path = "/ipv6loopback", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_IPV6 | URI_T_LOCALHOST }, __LINE__ );
	errors += _uri_test( "https://[::0:0:0:1]:8080/ipv6loopback", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "::0:0:0:1", .port = 8080, .path = "/ipv6loopback", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_IPV6 | URI_T_LOCALHOST }, __LINE__ );
	errors += _uri_test( "https://[::0:0000:0:001]:8080/ipv6loopback", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "::0:0000:0:001", .port = 8080, .path = "/ipv6loopback", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_IPV6 | URI_T_LOCALHOST }, __LINE__ );
	errors += _uri_test( "https://[::1]:8080/ipv6loopback", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "::1", .port = 8080, .path = "/ipv6loopback", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_IPV6 | URI_T_LOCALHOST }, __LINE__ );

	errors += _uri_test( "https://user:pass@192.168.0.1:8080/ipv4", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .userinfo = "user:pass", .user = "user", .pass = "pass", .host = "192.168.0.1", .port = 8080, .path = "/ipv4", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_IPV4 }, __LINE__ );

	errors += _uri_test( "file://///C:/windows/path", URI_OPT_NONE, (struct uri){ .scheme = "file", .scheme_t = URI_SCHEME_T_FILE, .host = NULL, .port = 0, .path = "C:/windows/path", .query = NULL, .fragment = NULL, .flags = URI_T_LOCALHOST }, __LINE__ );
	errors += _uri_test( "file:C:/windows/path", URI_OPT_NONE, (struct uri){ .scheme = "file", .scheme_t = URI_SCHEME_T_FILE, .host = NULL, .port = 0, .path = "C:/windows/path", .query = NULL, .fragment = NULL, .flags = URI_T_LOCALHOST }, __LINE__ );
	errors += _uri_test( "file:/C:/windows/path", URI_OPT_NONE, (struct uri){ .scheme = "file", .scheme_t = URI_SCHEME_T_FILE, .host = NULL, .port = 0, .path = "C:/windows/path", .query = NULL, .fragment = NULL, .flags = URI_T_LOCALHOST }, __LINE__ );
	errors += _uri_test( "file:///C:/windows/path", URI_OPT_NONE, (struct uri){ .scheme = "file", .scheme_t = URI_SCHEME_T_FILE, .host = NULL, .port = 0, .path = "C:/windows/path", .query = NULL, .fragment = NULL, .flags = URI_T_LOCALHOST }, __LINE__ );
	errors += _uri_test( "file://?/C:/windows/path", URI_OPT_NONE, (struct uri){ .scheme = "file", .scheme_t = URI_SCHEME_T_FILE, .host = NULL, .port = 0, .path = "C:/windows/path", .query = NULL, .fragment = NULL, .flags = URI_T_LOCALHOST }, __LINE__ );
	errors += _uri_test( "file://./C:/windows/path", URI_OPT_NONE, (struct uri){ .scheme = "file", .scheme_t = URI_SCHEME_T_FILE, .host = NULL, .port = 0, .path = "C:/windows/path", .query = NULL, .fragment = NULL, .flags = URI_T_LOCALHOST }, __LINE__ );

	// Examples from AAF files external essences
	errors += _uri_test( "file:///C:/Users/username/Downloads/441-16b.wav", URI_OPT_NONE, (struct uri){ .scheme = "file", .scheme_t = URI_SCHEME_T_FILE, .host = NULL, .port = 0, .path = "C:/Users/username/Downloads/441-16b.wav", .query = NULL, .fragment = NULL, .flags = URI_T_LOCALHOST }, __LINE__ );
	errors += _uri_test( "file://?/E:/ADPAAF/Sequence A Rendu.mxf", URI_OPT_NONE, (struct uri){ .scheme = "file", .scheme_t = URI_SCHEME_T_FILE, .host = NULL, .port = 0, .path = "E:/ADPAAF/Sequence A Rendu.mxf", .query = NULL, .fragment = NULL, .flags = URI_T_LOCALHOST }, __LINE__ );
	errors += _uri_test( "file:////C:/Users/username/Desktop/TEST2977052.aaf", URI_OPT_NONE, (struct uri){ .scheme = "file", .scheme_t = URI_SCHEME_T_FILE, .host = NULL, .port = 0, .path = "C:/Users/username/Desktop/TEST2977052.aaf", .query = NULL, .fragment = NULL, .flags = URI_T_LOCALHOST }, __LINE__ );
	errors += _uri_test( "file://localhost/Users/username/Music/fonk_2_3#04.wav", URI_OPT_NONE, (struct uri){ .scheme = "file", .scheme_t = URI_SCHEME_T_FILE, .host = "localhost", .port = 0, .path = "/Users/username/Music/fonk_2_3#04.wav", .query = NULL, .fragment = NULL, .flags = URI_T_LOCALHOST }, __LINE__ );
	errors += _uri_test( "file://10.87.230.71/mixage/DR2/Avid MediaFiles/MXF/1/3572607.mxf", URI_OPT_NONE, (struct uri){ .scheme = "file", .scheme_t = URI_SCHEME_T_FILE, .host = "10.87.230.71", .port = 0, .path = "/mixage/DR2/Avid MediaFiles/MXF/1/3572607.mxf", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_IPV4 }, __LINE__ );
	errors += _uri_test( "file:///_system/Users/username/pt2MCCzmhsFRHQgdgsTMQX.mxf", URI_OPT_NONE, (struct uri){ .scheme = "file", .scheme_t = URI_SCHEME_T_FILE, .host = NULL, .port = 0, .path = "/_system/Users/username/pt2MCCzmhsFRHQgdgsTMQX.mxf", .query = NULL, .fragment = NULL, .flags = URI_T_LOCALHOST }, __LINE__ );

	// URL Percent Decoding
	errors += _uri_test( "https://www.server.com/NON_DECODING/%C2%B0%2B%29%3D%C5%93%21%3A%3B%2C%3F.%2F%C2%A7%C3%B9%2A%24%C2%B5%C2%A3%7D%5D%E2%80%9C%23%7B%5B%7C%5E%40%5D%3C%3E", URI_OPT_NONE, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 0, .path = "/NON_DECODING/%C2%B0%2B%29%3D%C5%93%21%3A%3B%2C%3F.%2F%C2%A7%C3%B9%2A%24%C2%B5%C2%A3%7D%5D%E2%80%9C%23%7B%5B%7C%5E%40%5D%3C%3E", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com/DECODING/%C2%B0%2B%29%3D%C5%93%21%3A%3B%2C%3F.%2F%C2%A7%C3%B9%2A%24%C2%B5%C2%A3%7D%5D%E2%80%9C%23%7B%5B%7C%5E%40%5D%3C%3E", URI_OPT_DECODE_ALL, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .host = "www.server.com", .port = 0, .path = "/DECODING/°+)=œ!:;,?./§ù*$µ£}]“#{[|^@]<>", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );
	errors += _uri_test( "https://www.server.com/DECODING_UTF8/%E3%82%B5%E3%83%B3%E3%83%97%E3%83%AB%E7%B2%BE%E5%BA%A6%E7%B7%A8%E9%9B%86", URI_OPT_DECODE_ALL, (struct uri){ .scheme = "https", .scheme_t = URI_SCHEME_T_HTTPS, .userinfo = NULL, .user = NULL, .pass = NULL, .host = "www.server.com", .port = 0, .path = "/DECODING_UTF8/サンプル精度編集", .query = NULL, .fragment = NULL, .flags = URI_T_HOST_REGNAME }, __LINE__ );

	// Examples from https://en.wikipedia.org/wiki/Uniform_Resource_Identifier
	errors += _uri_test( "tel:+1-816-555-1212", URI_OPT_NONE, (struct uri){ .scheme = "tel", .scheme_t = URI_SCHEME_T_TEL, .userinfo = NULL, .user = NULL, .pass = NULL, .host = NULL, .port = 0, .path = "+1-816-555-1212", .query = NULL, .fragment = NULL, .flags = 0 }, __LINE__ );
	errors += _uri_test( "mailto:John.Doe@example.com", URI_OPT_NONE, (struct uri){ .scheme = "mailto", .scheme_t = URI_SCHEME_T_MAILTO, .userinfo = NULL, .user = NULL, .pass = NULL, .host = NULL, .port = 0, .path = "John.Doe@example.com", .query = NULL, .fragment = NULL, .flags = 0 }, __LINE__ );
	errors += _uri_test( "urn:oasis:names:specification:docbook:dtd:xml:4.1.2", URI_OPT_NONE, (struct uri){ .scheme = "urn", .scheme_t = URI_SCHEME_T_UNKNOWN, .userinfo = NULL, .user = NULL, .pass = NULL, .host = NULL, .port = 0, .path = "oasis:names:specification:docbook:dtd:xml:4.1.2", .query = NULL, .fragment = NULL, .flags = 0 }, __LINE__ );
	errors += _uri_test( "ldap://[2001:db8::7]/c=GB?objectClass?one", URI_OPT_NONE, (struct uri){ .scheme = "ldap", .scheme_t = URI_SCHEME_T_UNKNOWN, .userinfo = NULL, .user = NULL, .pass = NULL, .host = "2001:db8::7", .port = 0, .path = "/c=GB", .query = "objectClass?one", .fragment = NULL, .flags = URI_T_HOST_IPV6 }, __LINE__ );
	errors += _uri_test( "news:comp.infosystems.www.servers.unix", URI_OPT_NONE, (struct uri){ .scheme = "news", .scheme_t = URI_SCHEME_T_UNKNOWN, .userinfo = NULL, .user = NULL, .pass = NULL, .host = NULL, .port = 0, .path = "comp.infosystems.www.servers.unix", .query = NULL, .fragment = NULL, .flags = 0 }, __LINE__ );

	// errors += _uri_test( "xxxxxxxx", URI_OPT_NONE, (struct uri){ .scheme = NULL, .scheme_t = URI_SCHEME_T_UNKNOWN, .userinfo = NULL, .user = NULL, .pass = NULL, .host = NULL, .port = 0, .path = NULL, .query = NULL, .fragment = NULL, .flags = 0 }, __LINE__ );
	// errors += _uri_test( "xxxxxxxx", URI_OPT_NONE, (struct uri){ .scheme = NULL, .scheme_t = URI_SCHEME_T_UNKNOWN, .userinfo = NULL, .user = NULL, .pass = NULL, .host = NULL, .port = 0, .path = NULL, .query = NULL, .fragment = NULL, .flags = 0 }, __LINE__ );

	TEST_LOG("\n");

	return errors;
}

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
#include <libaaf/utils.h>

#include "common.h"


#define test_build_path( line, expected, dirsep, ... ) \
{\
	char *res = laaf_util_build_path( dirsep, __VA_ARGS__ );\
	if ( !expected ) {\
		if ( !res ) {\
			TEST_LOG( TEST_PASSED_STR "laaf_util_build_path(): \"%s\"\n", line, res );\
		} else {\
			TEST_LOG( TEST_ERROR_STR "laaf_util_build_path(): \"%s\"\n", line, res );\
			errors++;\
		}\
	} else if ( strcmp( res, expected ) == 0 ) {\
		TEST_LOG( TEST_PASSED_STR "laaf_util_build_path(): \"%s\"\n", line, res );\
	} else {\
		TEST_LOG( TEST_ERROR_STR "laaf_util_build_path(): \"%s\"\n", line, res );\
		errors++;\
	}\
}

static int test_relative_path( int line, const char *p1, const char *p2, const char *expected );
static int test_clean_filename( int line, const char *fname, const char *expected );



static int test_relative_path( int line, const char *p1, const char *p2, const char *expected ) {

	int pad1len = 32;
	int pad2len = 32;

	pad1len -= (p1) ? (int)laaf_util_utf8strCharLen(p1) : (int)strlen("(NULL)");
	pad2len -= (p2) ? (int)laaf_util_utf8strCharLen(p2) : (int)strlen("(NULL)");

	char *res = laaf_util_relative_path( p1, p2 );

	if ( expected == NULL && res != NULL ) {
		TEST_LOG( TEST_ERROR_STR "laaf_util_relative_path(): \"%s\"%*s   \"%s\"%*s   res:   %s\n", line, p1, pad1len, " ", p2, pad2len, " ", res );
		return 1;
	}

	if ( expected && !res ) {
		TEST_LOG( TEST_ERROR_STR "laaf_util_relative_path(): \"%s\"%*s   \"%s\"%*s   res:   (NULL)\n", line, p1, pad1len, " ", p2, pad2len, " " );
		return 1;
	}

	if ( res && strcmp( res, expected ) != 0 ) {
		TEST_LOG( TEST_ERROR_STR "laaf_util_relative_path(): \"%s\"%*s   \"%s\"%*s   res:   %s\n", line, p1, pad1len, " ", p2, pad2len, " ", res );
		return 1;
	}

	TEST_LOG( TEST_PASSED_STR "laaf_util_relative_path(): \"%s\"%*s   \"%s\"%*s   res:   %s\n", line, p1, pad1len, " ", p2, pad2len, " ", res );

	free( res );
	return 0;
}



static int test_clean_filename( int line, const char *fname, const char *expected ) {

	char buf[1024];
	int pad1len = 32;

	pad1len -= (fname) ? (int)laaf_util_utf8strCharLen(fname) : (int)strlen("(NULL)");

	if ( fname ) {
		snprintf( buf, sizeof(buf), "%s", fname );
	}

	char *res = laaf_util_clean_filename( (fname) ? buf : NULL );

	if ( expected == NULL && res != NULL ) {
		TEST_LOG( TEST_ERROR_STR "laaf_util_clean_filename(): \"%s\"%*s   res:   \"%s\"\n", line, fname, pad1len, " ", res );
		return 1;
	}

	if ( expected && !res ) {
		TEST_LOG( TEST_ERROR_STR "laaf_util_clean_filename(): \"%s\"%*s   res:   \"(NULL)\"\n", line, fname, pad1len, " " );
		return 1;
	}

	if ( res && strcmp( res, expected ) != 0 ) {
		TEST_LOG( TEST_ERROR_STR "laaf_util_clean_filename(): \"%s\"%*s   res:   \"%s\"\n", line, fname, pad1len, " ", res );
		return 1;
	}

	TEST_LOG( TEST_PASSED_STR "laaf_util_clean_filename(): \"%s\"%*s   res:   \"%s\"\n", line, fname, pad1len, " ", res );

	return 0;
}



static int test_is_fileext( int line, const char *filepath, const char *ext, int expected ) {

	int pad1len = 32;
	pad1len -= (filepath) ? (int)laaf_util_utf8strCharLen(filepath) : (int)strlen("(NULL)");


	int res = laaf_util_is_fileext( filepath, ext );

	if ( expected != res ) {
		TEST_LOG( TEST_ERROR_STR "laaf_util_is_fileext(): \"%s\"%*s     \"%s\"\n", line, filepath, pad1len, " ", ext );
		return 1;
	}

	TEST_LOG( TEST_PASSED_STR "laaf_util_is_fileext(): \"%s\"%*s     \"%s\"\n", line, filepath, pad1len, " ", ext );
	return 0;
}



int main( int argc, char *argv[] ) {

	(void)argc;
	(void)argv;

#ifdef _WIN32
	INIT_WINDOWS_CONSOLE()
#endif

	SET_LOCALE()


	int errors = 0;

	TEST_LOG("\n");

	errors += test_clean_filename( __LINE__, NULL, NULL );
	errors += test_clean_filename( __LINE__, "", NULL );
	errors += test_clean_filename( __LINE__, " ", NULL );
	errors += test_clean_filename( __LINE__, ".", NULL );
	errors += test_clean_filename( __LINE__, "abc ", "abc" );
	errors += test_clean_filename( __LINE__, "abc.", "abc" );
	errors += test_clean_filename( __LINE__, "a bc", "a bc" );
	errors += test_clean_filename( __LINE__, " abc", " abc" );
	errors += test_clean_filename( __LINE__, "/<>:\"|?*\\ ", "_________" );
	errors += test_clean_filename( __LINE__, "サンプル", "サンプル" );

	TEST_LOG("\n");

	test_build_path( __LINE__, "a/b/c", "/", "a", "b", "c", NULL );
	test_build_path( __LINE__, "/a/b/c", "/", "/a", "b", "c", NULL );
	test_build_path( __LINE__, "/a/b/c", "/", "/", "a", "b", "c", NULL );
	test_build_path( __LINE__, "/a/b/c", "/", "///", "///a///", "///b///", "///c///", NULL );
	test_build_path( __LINE__, "/a/b/c/", "/", "/", "a", "b", "c", "/", NULL );
	test_build_path( __LINE__, "/サ/ン/ル", "/", "/", "サ", "ン", "ル", NULL );

	TEST_LOG("\n");

	test_is_fileext( __LINE__, "", "", 0 );
	test_is_fileext( __LINE__, "a", "", 0 );
	test_is_fileext( __LINE__, NULL, NULL, 0 );
	test_is_fileext( __LINE__, "a.ext", "ext", 1 );
	test_is_fileext( __LINE__, "a.ext", "EXT", 1 );
	test_is_fileext( __LINE__, "/a/b/c/d.ext", "ext", 1 );
	test_is_fileext( __LINE__, ".ext", "ext", 1 );
	test_is_fileext( __LINE__, "ル.ext", "ext", 1 );
	test_is_fileext( __LINE__, "a.ン", "ン", 1 );

	TEST_LOG("\n");

	errors += test_relative_path( __LINE__, NULL, NULL, NULL );
	errors += test_relative_path( __LINE__, "", "", NULL );
	errors += test_relative_path( __LINE__, "", "/a", NULL );
	errors += test_relative_path( __LINE__, "/a", "", NULL );
	errors += test_relative_path( __LINE__, "/a/b/c/file.ext", "/a", "./b/c/file.ext" );
	errors += test_relative_path( __LINE__, "/a/b/c/file.ext", "/a/b/c/d/e/", "../../file.ext" );
	errors += test_relative_path( __LINE__, "///a///b///c///file.ext", "//a///b/c////d/e/////", "../../file.ext" );
	errors += test_relative_path( __LINE__, "/file.ext", "/a/", "../file.ext" );
	errors += test_relative_path( __LINE__, "file.ext", "/a/", NULL );
	errors += test_relative_path( __LINE__, "/a", "/a", "./" );
	errors += test_relative_path( __LINE__, "/a/b", "/c/d", "../../a/b" );
	errors += test_relative_path( __LINE__, "C:/a/b", "C:\\c/d", "../../a/b" );
	errors += test_relative_path( __LINE__, "C:/a/b", "c:\\c/d", "../../a/b" );
	errors += test_relative_path( __LINE__, "C:/a/b", "D:/c/d", NULL );
	errors += test_relative_path( __LINE__, "C:/a/b", "/c/d", NULL );
	errors += test_relative_path( __LINE__, "/a/b", "c:/c/d", NULL );
	errors += test_relative_path( __LINE__, "c:/a/サンプル", "c:/c/", "../a/サンプル" );

	TEST_LOG("\n");

	return errors;
}

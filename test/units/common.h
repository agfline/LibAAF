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

#ifdef _WIN32
	#include <windows.h>
	#include <fcntl.h> // _O_U8TEXT
#endif

#include <locale.h>
#include <libaaf/utils.h>

#define TEST_ERROR_STR   "\x1b[38;5;242m%05i "TREE_LINE" \x1b[0m[\x1b[38;5;124mer\x1b[0m] "
#define TEST_PASSED_STR  "\x1b[38;5;242m%05i "TREE_LINE" \x1b[0m[\x1b[92mok\x1b[0m] "



#ifdef _WIN32
	#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
		#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
	#endif

	#define INIT_WINDOWS_CONSOLE() \
		HANDLE hOut = GetStdHandle( STD_OUTPUT_HANDLE );\
		DWORD dwMode = 0;\
		GetConsoleMode( hOut, &dwMode );\
		SetConsoleMode( hOut, (dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) );\
		SetConsoleOutputCP(65001);\
		_setmode( _fileno(stdout), _O_U8TEXT );
#endif


#ifdef __MINGW32__
	#define WPRIws L"ls" // wchar_t*
	#define WPRIs  L"s" // char*

	#define SET_LOCALE() setlocale( LC_ALL, "" );

	#define TEST_LOG( ... )\
	{\
		size_t len = (size_t)snprintf( NULL, 0, __VA_ARGS__ );\
		char *str = malloc( len +1 );\
		laaf_util_snprintf_realloc( &str, &len, 0, __VA_ARGS__ );\
		wchar_t *wstr = laaf_util_windows_utf8toutf16( str );\
		fwprintf( stdout, L"%"WPRIws, wstr );\
		free( wstr );\
	}
#else
	#define WPRIws L"ls" // wchar_t*
	#define WPRIs  L"S" // char*

	#ifdef _MSC_VER
		#define SET_LOCALE() setlocale( LC_ALL, ".utf8" );
	#else
		#define SET_LOCALE() setlocale( LC_ALL, "" );
	#endif

	#define TEST_LOG( ... )\
		fprintf( stdout, __VA_ARGS__ );
#endif

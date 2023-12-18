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


#include "common.h"

#ifdef _WIN32
	#include <windows.h>
	#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
		#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
	#endif
#endif



void aafi_enable_windows_VT100_output( void ) {
#ifdef _WIN32
	/* enables ANSI colors and unicode chars */
	HANDLE hOut = GetStdHandle( STD_OUTPUT_HANDLE );
	DWORD dwMode = 0;
	GetConsoleMode( hOut, &dwMode );
	SetConsoleMode( hOut, (dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) );
#endif
}

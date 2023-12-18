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

/**
 * @file LibAAF/AAFIface/AAFIface.c
 * @brief AAF processing
 * @author Adrien Gesta-Fline
 * @version 0.1
 * @date 04 october 2017
 *
 * @ingroup AAFIface
 * @addtogroup AAFIface
 *
 * The AAFIface provides the actual processing of the AAF Objects in order to show
 * essences and clips in a simplified manner. Indeed, AAF has many different ways to
 * store data and metadata. Thus, the AAFIface is an abstraction layer that provides
 * a constant and unique representation method of essences and clips.
 *
 *
 *
 * @{
 */


#include "common.h"

#ifdef _WIN32
	#include <windows.h>
	#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
		#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
	#endif
#endif



void aafi_enable_windows_VT100_output( void )
{
#ifdef _WIN32
	/* enables ANSI colors and unicode chars */
	HANDLE hOut = GetStdHandle( STD_OUTPUT_HANDLE );
	DWORD dwMode = 0;
	GetConsoleMode( hOut, &dwMode );
	SetConsoleMode( hOut, (dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) );
#endif
}

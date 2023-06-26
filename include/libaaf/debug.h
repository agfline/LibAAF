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

#ifndef __debug_h__
#define __debug_h__

#include <stdio.h>



typedef enum verbosityLevel_e {
	VERB_QUIET = 0,
	VERB_ERROR,
	VERB_WARNING,
	VERB_DEBUG,
	MAX_VERB
} verbosityLevel_e;



/**
 *	Displays a warning with its associated source filename,
 *	function name, line number and a custom message.
 *
 *	The `if ( 1 )` allows to expand the macro inside an
 *	if statement with or without brackets.
 *
 *	This macro doesn't change the execution flow.
 */

#define _warning( v, ... )                              \
	if ( v >= VERB_WARNING )                           \
	{                                                \
		fprintf( stderr, "WARNING %s:%d in %s() : ", \
			__FILE__,                                \
			__LINE__,                                \
			__func__ );                              \
		fprintf( stderr, __VA_ARGS__ );              \
	}




/**
 *	Displays an error with its associated source filename,
 *	function name, line number and a custom message.
 *
 *	The `if ( 1 )` allows to expand the macro inside an
 *	if statement with or without brackets.
 *
 *	This macro doesn't change the execution flow, however
 *	the user should interrupt the function right after
 *	calling _error().
 */

#define _error( v, ... )                                \
	if ( v >= VERB_ERROR )                                         \
	{                                                \
		fprintf( stderr, "__ERROR %s:%d in %s() : ", \
			__FILE__,                                \
			__LINE__,                                \
			__func__ );                              \
		fprintf( stderr, __VA_ARGS__ );              \
	}




/**
 *	Displays an error with its associated source filename,
 *	function name, line number and a custom message.
 *
 *	The `if ( 1 )` allows to expand the macro inside an
 *	if statement with or without brackets.
 *
 *	This macro interrupts the execution and exits the program.
 */
/*
#define _fatal( ... )                                \
	if ( 1 )                                         \
	{                                                \
	 	fprintf( stderr, "__FATAL %s:%d in %s() : ", \
			__FILE__,                                \
			__LINE__,                                \
			__func__ );                              \
		fprintf( stderr, __VA_ARGS__ );              \
		exit( 1 );                                   \
	}
*/

#endif // ! __debug_h__

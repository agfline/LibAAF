#ifndef __debug_h__
#define __debug_h__

#include <stdio.h>

/**
 *	Displays a warning with its associated source filename,
 *	function name, line number and a custom message.
 *
 *	The `if ( 1 )` allows to expand the macro inside an
 *	if statement with or without brackets.
 *
 *	This macro doesn't change the execution flow.
 */

#define _warning( ... )                              \
	if ( 1 )                                         \
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

#define _error( ... )                                \
	if ( 1 )                                         \
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


#endif // ! __debug_h__

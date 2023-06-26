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

#ifndef __AAFIParser_h__
#define __AAFIParser_h__

/**
 *	@file LibAAF/AAFIface/AAFIParser.h
 *	@brief AAF processing
 *	@author Adrien Gesta-Fline
 *	@version 0.1
 *	@date 27 june 2018
 *
 *	@ingroup AAFIface
 *	@addtogroup AAFIface
 *	@{
 */

// #include <libaaf/AAFTypes.h>
// #include <libaaf/AAFIface.h>
#include <libaaf/AAFCore.h>
#include <libaaf/AAFIface.h>

typedef struct AAF_Iface AAF_Iface;



#define TD_OK      0
#define TD_ERROR   1
#define TD_WARNING 2
#define TD_INFO    3
#define TD_NOT_SUPPORTED 4

#define __td_set( __td, __ptd, offset ) \
	__td.fn  = __LINE__;    \
	__td.pfn = __ptd->fn;    \
	__td.lv  = __ptd->lv+offset; \
	__td.ll  = __ptd->ll;   \
	__td.ll[__td.lv] = (offset > 0) ? 0 : __td.ll[__td.lv];   \
	__td.eob = 0; \
	__td.hc  = 0; \
	__td.sub = 0; \


#define DUMP_OBJ( aafi, Obj, __td ) \
	_DUMP_OBJ( aafi, Obj, __td, TD_OK, __LINE__, "" );

#define DUMP_OBJ_ERROR( aafi, Obj, __td, ... ) \
	(__td)->eob = 1; \
	_DUMP_OBJ( aafi, Obj, __td, TD_ERROR, __LINE__, __VA_ARGS__ );

#define DUMP_OBJ_WARNING( aafi, Obj, __td, ... ) \
	_DUMP_OBJ( aafi, Obj, __td, TD_WARNING, __LINE__, __VA_ARGS__ );

#define DUMP_OBJ_INFO( aafi, Obj, __td, ... ) \
	_DUMP_OBJ( aafi, Obj, __td, TD_OK, __LINE__, __VA_ARGS__ );

#define DUMP_OBJ_NO_SUPPORT( aafi, Obj, __td ) \
	(__td)->eob = 1; \
	_DUMP_OBJ_NO_SUPPORT( aafi, Obj, __td, __LINE__ ); \
	// aaf_dump_ObjectProperties( aafi->aafd, Obj );


int aafi_retrieveData( AAF_Iface *aafi );

void _DUMP_OBJ( AAF_Iface *aafi, aafObject *Obj, struct trace_dump *__td, int state, int line, char *fmt, ... );
void _DUMP_OBJ_NO_SUPPORT( AAF_Iface *aafi, aafObject *Obj, struct trace_dump *__td, int line );
void trace_obj( AAF_Iface *aafi, aafObject *Obj, char *color );

int parse_Segment( AAF_Iface *aafi, aafObject *Segment, td *__ptd );


#endif // __AAFIParser_h__

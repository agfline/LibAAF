#ifndef __CFBTypes_h__
#define __CFBTypes_h__

/*
 *	This file is part of LibCFB.
 *
 *	Copyright (c) 2017 Adrien Gesta-Fline
 *
 *	LibCFB is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Affero General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	any later version.
 *
 *	LibCFB is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Affero General Public License for more details.
 *
 *	You should have received a copy of the GNU Affero General Public License
 *	along with LibCFB. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *	@file LibCFB/CFBTypes.h
 *	@brief Compound File Binary Library
 *	@author Adrien Gesta-Fline
 *	@version 0.1
 *	@date 04 october 2017
 *
 *	@ingroup LibCFB
 *	@addtogroup LibCFB
 *	@{
 */





#include <stdint.h>



/**
 *	@name Types
 *	@{
 */



/**
 *	Class Identifier structure.
 *
 *	Used by cfbHeader._clsid and cfbNode._clsId.
 *
 *	16-byte long, binary compatible with GUID and AAF's AUID.
 */

struct cfbCLSID_t
{
	uint32_t Data1;
	uint16_t Data2;
	uint16_t Data3;
	uint8_t  Data4[8];

} __attribute__((packed));

typedef struct cfbCLSID_t cfbCLSID_t;



/**
 *	64-bit value representing number of 100 nanoseconds since January 1, 1601.
 *
 *	Used in cfbNode.
 */

struct cfbFiletime_t
{          
  uint32_t dwLowDateTime;
  uint32_t dwHighDateTime;

} __attribute__((packed));

typedef struct cfbFiletime_t cfbFiletime_t;



/**
 *	A sector ID, that is an index into the FAT or the MiniFAT.
 */

typedef uint32_t cfbSectorID_t;




/**
 *	A stream identifier, that is an index into the array of nodes (directory entries).
 */

typedef uint32_t cfbSID_t;




/**
 *	@}
 */

/*
 *	@}
 */

#endif // ! __CFBTypes_h__

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

#ifndef __AAFClass_h__
#define __AAFClass_h__


/**
 *	@brief AAF core functions.
 *	@author Adrien Gesta-Fline
 *	@version 0.1
 *	@date 04 october 2017
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libaaf/AAFTypes.h>
#include <libaaf/AAFCore.h>




#define foreachClass( Class, Classes ) \
	for( Class = Classes; Class != NULL; Class = Class->next )



#define foreachClassInheritance( Class, Classes ) \
	for( Class = Classes; Class != NULL; Class = Class->Parent )



#define foreachPropertyDefinition( PDef, PDefs ) \
	for( PDef = PDefs; PDef != NULL; PDef = PDef->next )



/**
 *	When it expands, allocates a new aafPropertyDef,
 *	sets its fields according to arguments, and adds it
 *	to the aafClass.Properties list.
 *
 *	@param Class  A pointer to the aafClass to attach the new aafPropertyDef to.
 *	@param Prop   A pointer to an aafPropertyDef struct.
 *	@param Pid    The Pid of the new aafPropertyDef struct.
 *	@param IsReq  Tells if the property is either REQUIRED or OPTIONAL.
 */
/*	NOTE Moved to AAFClass.c and AAFCore.c due to specific return types.
#define attachNewProperty( Class, Prop, Pid, IsReq ) \
	Prop = malloc( sizeof(aafPropertyDef) );         \
	if ( Prop == NULL )                              \
	{                                                \
		error( "%s.", strerror( errno ) );        \
		return -1;                                   \
	}                                                \
	Prop->pid         = Pid;                         \
	Prop->isReq       = IsReq;                       \
	Prop->meta        = 0;                           \
	Prop->next        = Class->Properties;           \
	Class->Properties = Prop;                        \
*/



int classExists( AAF_Data *aafd, aafUID_t *ClassID );

aafClass * defineNewClass( AAF_Data *aafd, const aafUID_t *id, uint8_t isConcrete, aafClass *parent );


aafClass * getClassByID( AAF_Data *aafd, const aafUID_t *id );

aafPropertyDef * getPropertyDefinitionByID( aafClass *Classes, aafPID_t PID );

void printClasses( aafClass *Class, int depth ); // TODO move to AAFDump ?


int setDefaultClasses( AAF_Data *aafd );


#endif // ! __AAFClass_h__

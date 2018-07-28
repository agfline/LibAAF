#ifndef __AAFClass_h__
#define __AAFClass_h__

/*
 *	This file is part of LibAAF.
 *
 *	Copyright (c) 2017 Adrien Gesta-Fline
 *
 *	LibAAF is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Affero General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	any later version.
 *
 *	LibAAF is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Affero General Public License for more details.
 *
 *	You should have received a copy of the GNU Affero General Public License
 *	along with LibAAF. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 *	@brief AAF core functions.
 *	@author Adrien Gesta-Fline
 *	@version 0.1
 *	@date 04 october 2017
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AAFTypes.h"
#include "AAFCore.h"




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
		_error( "%s.\n", strerror( errno ) );        \
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

void printClasses( aafClass *Class, int depth );


int setDefaultClasses( AAF_Data *aafd );


#endif // ! __AAFClass_h__

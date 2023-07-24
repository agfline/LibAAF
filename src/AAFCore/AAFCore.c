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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <libaaf/AAFTypes.h>
#include <libaaf/AAFCore.h>
#include <libaaf/AAFToText.h>
#include <libaaf/AAFDump.h>

#include <libaaf/AAFDefs/AAFClassDefUIDs.h>
#include <libaaf/AAFDefs/AAFPropertyIDs.h>
#include <libaaf/AAFDefs/AAFFileKinds.h>
#include <libaaf/AAFDefs/AAFTypeDefUIDs.h>

#include <libaaf/debug.h>

#include "AAFClass.h"
#include "../common/utils.h"



#define debug( ... ) \
	_dbg( aafd->dbg, aafd, DEBUG_SRC_ID_AAF_CORE, VERB_DEBUG, __VA_ARGS__ )

#define warning( ... ) \
	_dbg( aafd->dbg, aafd, DEBUG_SRC_ID_AAF_CORE, VERB_WARNING, __VA_ARGS__ )

#define error( ... ) \
	_dbg( aafd->dbg, aafd, DEBUG_SRC_ID_AAF_CORE, VERB_ERROR, __VA_ARGS__ )


/**
 *	Loops through each aafPropertyIndexEntry_t of a "properties" node stream.
 *
 *	@param Header Pointer to the stream's aafPropertyIndexHeader_t struct.
 *	@param Entry  Pointer that will receive each aafPropertyIndexEntry_t struct.
 *	@param Value  Pointer to each property's data value, of aafPropertyIndexEntry_t._length
 *	              bytes length.
 *	@param i      uint32_t iterator.
 */

#define foreachPropertyEntry( Header, Entry, Value, i )                                            \
	for ( Entry = (aafPropertyIndexEntry_t*)(((char*)Header) + sizeof(aafPropertyIndexHeader_t)),    \
	      value = ((unsigned char*)Entry) + (Header->_entryCount * sizeof(aafPropertyIndexEntry_t)), \
	      i = 0;                                                                                     \
	      i < Header->_entryCount;                                                                   \
	      value += Entry->_length,                                                                   \
	      Entry++,                                                                                   \
	      i++ )




/**
 *	Loops through each aafStrongRefSetEntry_t of a StrongRefSet Index node stream.
 *
 *	@param Header Pointer to the stream's aafStrongRefSetHeader_t struct.
 *	@param Entry  Pointer that will receive each aafStrongRefSetEntry_t struct.
 *	@param i      uint32_t iterator.
 */

#define foreachStrongRefSetEntry( Header, Entry, i )                                                                   \
	for( Entry = (aafStrongRefSetEntry_t*)((char*)Header + sizeof(aafStrongRefSetHeader_t)),                             \
	     i = 0;                                                                                                          \
	     i < Header->_entryCount;                                                                                        \
	     Entry = (aafStrongRefSetEntry_t*)((char*)Entry + Header->_identificationSize + sizeof(aafStrongRefSetEntry_t)), \
	     i++ )




/**
 *	Loops through each aafStrongRefVectorEntry_t of a StrongRefVector Index node stream.
 *
 *	@param Header Pointer to the stream's aafStrongRefVectorHeader_t struct.
 *	@param Entry  Pointer that will receive each aafStrongRefVectorEntry_t struct.
 *	@param i      uint32_t iterator.
 */

#define foreachStrongRefVectorEntry( Header, Entry, i )                                            \
	for( Entry = (aafStrongRefVectorEntry_t*)(((char*)Header) + sizeof(aafStrongRefVectorHeader_t)), \
	     i = 0;                                                                                      \
	     i < Header->_entryCount;                                                                    \
	     Entry++,                                                                                    \
	     i++ )






#define attachNewProperty( Class, PDef, Pid, IsReq )              \
	PDef = calloc( sizeof(aafPropertyDef), sizeof(unsigned char) ); \
	if ( PDef == NULL ) {                                           \
		error( "%s.", strerror( errno ) );                            \
		return NULL;                                                  \
	}                                                               \
	PDef->pid         = Pid;                                        \
	PDef->isReq       = IsReq;                                      \
	PDef->meta        = 0;                                          \
	PDef->name        = NULL;                                       \
	PDef->next        = Class->Properties;                          \
	Class->Properties = PDef;                                       \







/*
 *	Retrieves useful file informations out of Header Object.
 *
 *	@param  aafd  Pointer to the AAF_Data structure.
 *
 *	@return        0 on success\n
 *	              -1 on error.
 */

static int parse_Header( AAF_Data *aafd );



/*
 *	Retrieves useful file informations out of Identification Object.
 *
 *	@param  aafd  Pointer to the AAF_Data structure.
 *
 *	@return        0 on success\n
 *	              -1 on error.
 */

static int parse_Identification( AAF_Data *aafd );



/*
 *	Tests the CFB_Data.hdr._clsid field for a valid AAF file.
 *
 *	@note The spec says that the AAFFileKind signature should be retrieved from the CLSID
 *	of the Root IStorage. In practice, this CLSID holds the #AAFClassID_Root value, and
 *	the AAFFileKind is (sometimes) found in the CLSID of the CFB_Header, which according
 *	to the CFB spec should be zero. All of this has been observed in AAF files built with
 *	the official AAFSDK.
 *
 *	As a conclusion, the way to test for a valid AAF file is not a valid test itself.. or
 *	not sufficiently documented. Thus, this function shall not be trusted until further
 *	knowledge improvement.
 *
 *	@param  aafd  Pointer to the AAF_Data structure.
 *
 *	@return       1 if the file *looks like* a valid AAF\n
 *	              0 otherwise.
 */

// static int isValidAAF( AAF_Data *aafd );



/**
 *	Sets the AAF_Data structure's pointers to the main AAF Tree objects. These pointers
 *	can then be used for quick conveniant objects access.
 *
 *	@param  aafd   Pointer to the AAF_Data structure.
 */

static void setObjectShortcuts( AAF_Data *aafd );



/**
 *	Parses the entire Compound File Binary Tree and retrieves Objets and Properties.
 *
 *	This function first parses the Root Object, then follows the Root::MetaDictionary to
 *	retrieve potential custom Classes and Properties with retrieveMetaDictionaryClass(),
 *	and then parses the rest of the Tree starting at Root::Header.
 *
 *	This function should be called after the AAF Classes has been defined by
 *	#setDefaultClasses(). This function is called by aaf_load_file().
 *
 *	@param  aafd   Pointer to the AAF_Data structure.
 */

static int retrieveObjectTree( AAF_Data *aafd );



/**
 *	Parses the entire MetaDictionary, retrieving potential custom classes and properties.
 *	This function is to be called within a loop that iterates through the
 *	MetaDictionary::ClassDefinitions Objects, and by itself when looking for parent
 *	Classes.
 *
 *	@param  aafd           Pointer to the AAF_Data structure.
 *	@param  TargetClassDef Pointer to the current ClassDefinition Object.
 *
 *	@return                A pointer to the retrieved Class.
 */

static aafClass * retrieveMetaDictionaryClass( AAF_Data *aafd, aafObject *TargetClassDef );



/**
 *	Allocates a new aafObject structure, and adds it to the AAF_Data.Objects list.
 *
 *	@param  aafd   Pointer to the AAF_Data structure.
 *	@param  node   Pointer to the corresponding cfbNode structure.
 *	@param  Class  Pointer to the corresponding class definition aafClass structure.
 *	@param  parent Pointer to the new Object's parent object, that is the one that has an
 *	               ownership reference (Strong Ref Set/Vector) to the new Object.
 *
 *	@return        A pointer to the newly created aafObject.
 */

static aafObject * newObject( AAF_Data *aafd, cfbNode *node, aafClass *Class, aafObject *parent );



/**
 *	Allocates a new aafProperty structure.
 *
 *	@param  Def  Pointer to the corresponding property definition aafPropertyDef structure.
 *
 *	@return      A pointer to the newly created aafProperty.
 */

static aafProperty * newProperty( AAF_Data *aafd, aafPropertyDef *Def );



/**
 *	Test whether or not, a property ID (property definition) was already retrieved for a given Class.
 *
 *	@param  Class  Pointer to the aafClass.
 *	@param  Pid    Property ID.
 *
 *	@return        A pointer to the retrieved property definition\n
 *                 NULL otherwise.
 */

static aafPropertyDef * propertyIdExistsInClass( aafClass *Class, aafPID_t Pid );



/**
 *	Sets the aafStrongRefSetHeader_t Obj->Header and aafStrongRefSetEntry_t Obj->Entry,
 *	when parsing an Object from a StrongReferenceSet. This function is called by the
 *	#retrieveStrongReferenceSet() function.
 *
 *	@param  Obj    Pointer to an aafObject structure.
 *	@param  Header Pointer to an aafStrongRefSetHeader_t structure.
 *	@param  Entry  Pointer to an aafStrongRefSetEntry_t structure.
 */

static int setObjectStrongRefSet( aafObject *Obj, aafStrongRefSetHeader_t *Header, aafStrongRefSetEntry_t *Entry );



/**
 *	Sets the aafStrongRefVectorHeader_t Obj->Header and aafStrongRefVectorEntry_t
 *	Obj->Entry, when parsing an Object from a StrongReferenceSet. This function is called
 *	by the retrieveStrongReferenceVector() function.
 *
 *	@param  Obj    Pointer to an aafObject structure.
 *	@param  Header Pointer to an aafStrongRefVectorHeader_t structure.
 *	@param  Entry  Pointer to an aafStrongRefVectorEntry_t structure.
 */

static int setObjectStrongRefVector( aafObject *Obj, aafStrongRefVectorHeader_t *Header, aafStrongRefVectorEntry_t *Entry );



/**
 *	Retrieves and parses a single StrongReference Object. This function is called by
 *	retrieveProperty() when it encounters an SF_STRONG_OBJECT_REFERENCE property.
 *
 *	@param aafd   Pointer to the AAF_Data structure.
 *	@param Prop   Pointer to the property holding the SF_STRONG_OBJECT_REFERENCE.
 *	@param parent Pointer to the parent Object which holds the Prop property.
 */

static int retrieveStrongReference( AAF_Data *aafd, aafProperty *Prop, aafObject *parent );



/**
 *	Retrieves and parses StrongReferenceSet Objects. This function is called by
 *	retrieveProperty() when it encounters an SF_STRONG_OBJECT_REFERENCE_SET property.
 *
 *	@param aafd   Pointer to the AAF_Data structure.
 *	@param Prop   Pointer to the property holding the SF_STRONG_OBJECT_REFERENCE_SET.
 *	@param parent Pointer to the parent Object which holds the Prop property.
 */

static int retrieveStrongReferenceSet( AAF_Data *aafd, aafProperty *Prop, aafObject *parent );



/**
 *	Retrieve and parse StrongReferenceVector Objects. This function is called by
 *	retrieveProperty() when it encounters an SF_STRONG_OBJECT_REFERENCE_VECTOR property.
 *
 *	@param aafd   Pointer to the AAF_Data structure.
 *	@param Prop   Pointer to the property holding the SF_STRONG_OBJECT_REFERENCE_VECTOR.
 *	@param parent Pointer to the parent Object which holds the Prop property.
 */

static int retrieveStrongReferenceVector( AAF_Data *aafd, aafProperty *Prop, aafObject *parent );



/**
 *	Adds a new aafProperty to an Object->properties list. If the property Stored Form is
 *	either SF_STRONG_OBJECT_REFERENCE, SF_STRONG_OBJECT_REFERENCE_SET or
 *	SF_STRONG_OBJECT_REFERENCE_VECTOR, then the function follows the "link" to the
 *	Object(s) by calling respectively retrieveStrongReference(),
 *	retrieveStrongReferenceSet() or retrieveStrongReferenceVector(). This function is
 *	called by retrieveObjectProperties().
 *
 *	@param aafd Pointer to the AAF_Data structure.
 *	@param Obj  Pointer to the aafObject structure holding this property.
 *	@param Def  Pointer to the aafPropertyDef structure defining this property.
 *	@param p    Pointer to the aafPropertyIndexEntry_t structure representing the property
 *	            in the file.
 *	@param v    Pointer to a p->_length long byte array holding the actual property value.
 *	@param bo   uint8_t specifying the property's Byte Order. TO BE IMPLEMENTED
 *
 *	@TODO Take ByteOrder into account
 */

static int retrieveProperty( AAF_Data *aafd, aafObject *Obj, aafPropertyDef *Def, aafPropertyIndexEntry_t *p, aafByte_t *v, uint8_t bo );



/**
 *	Retrieves the properties for a given aafObject.
 *
 *	@param aafd Pointer to the AAF_Data structure.
 *	@param Obj  Pointer to the aafObject holding the properties.
 */

static int retrieveObjectProperties( AAF_Data  *aafd, aafObject *Obj );



/**
 *	Retrieves a StrongRef Set/Vector Index Node in the Compound File Tree. This function
 *	is called by both retrieveStrongReferenceSet() and retrieveStrongReferenceVector().
 *
 *	@param  aafd    Pointer to the AAF_Data structure.
 *	@param  parent  Pointer to the parent aafObject.
 *	@param  refName Pointer to a null terminated string holding the reference name.
 *
 *	@return         Pointer to the retrieved Node cfbNode structure.
 */

static cfbNode * getStrongRefIndexNode( AAF_Data *aafd, aafObject *parent, const wchar_t *refName );



/**
 *	Retrieves a StrongRef Set or Vector Entry Node in the Compound File Tree. This
 *	function is called by both retrieveStrongReferenceSet() and
 *	retrieveStrongReferenceVector().
 *
 *	@param aafd     Pointer to the AAF_Data structure.
 *	@param parent   Pointer to the parent Index Node.
 *	@param baseName Pointer to a null terminated string holding the reference base name.
 *	@param index    uint32_t number representing the index number of the reference.
 *
 *	@return         Pointer to the retrieved Node cfbNode structure.
 */

static cfbNode * getStrongRefEntryNode( AAF_Data *aafd, aafObject *parent, const wchar_t *baseName, uint16_t index );



/**
 *	Retrieves and returns a list of aafPropertyIndexHeader_t.
 *	For a given cfbNode, retrieves its /properties Stream Node and returns the stream as
 *	a pointer to an aafPropertyIndexHeader_t structure, wich the stream should begin with.
 *
 *	@param  aafd Pointer to the AAF_Data structure.
 *	@param  node Pointer to a cfbNode structure.
 *
 *	@return      Pointer to an aafPropertyIndexHeader_t structure, followed by _entryCount
 *	             aafPropertyIndexEntry_t structures.
 */

static aafPropertyIndexHeader_t * getNodeProperties( AAF_Data *aafd, cfbNode *node );



/**
 *	Retrieves and returns a list of StrongReferenceSet.
 *
 *	For a given Index cfbNode, retrieves its Stream and returns it as a pointer to an
 *	aafStrongRefSetHeader_t structure, wich the stream should begin with.
 *
 *	@param aafd   Pointer to the AAF_Data structure.
 *	@param node   Pointer to an Index cfbNode structure.
 *	@param parent Pointer to the aafObject parent, only used on error printing.
 *
 *	@return       Pointer to an aafStrongRefSetHeader_t structure, followed by _entryCount
 *	              aafStrongRefSetEntry_t structures.
 */

static aafStrongRefSetHeader_t * getStrongRefSetList( AAF_Data *aafd, cfbNode *node, aafObject *parent );



/**
 *	Retrieves and returns a list of StrongReferenceVectors.
 *
 *	For a given Index cfbNode, retrieves its Stream and returns it as a pointer to an
 *	aafStrongRefVectorHeader_t structure, wich the stream should begin with.
 *
 *	@param  aafd   Pointer to the AAF_Data structure.
 *	@param  node   Pointer to an Index cfbNode structure.
 *	@param  parent Pointer to the aafObject parent, only used on error printing.
 *
 *	@return        Pointer to an aafStrongRefVectorHeader_t structure, followed by
 *	               _entryCount aafStrongRefVectorEntry_t structures.
 */

static aafStrongRefVectorHeader_t * getStrongRefVectorList( AAF_Data *aafd, cfbNode *node, aafObject *parent );








AAF_Data *aaf_alloc( struct dbg *dbg )
{
	AAF_Data *aafd = calloc( sizeof(AAF_Data), sizeof(unsigned char) );

	if ( aafd == NULL )
		error( "%s.", strerror( errno ) );

	aafd->cfbd = NULL;
	// aafd->verb = VERB_QUIET;

	aafd->Identification.CompanyName = NULL;
	aafd->Identification.ProductName = NULL;
	aafd->Identification.ProductVersionString = NULL;
	aafd->Identification.Platform = NULL;

	aafd->Classes = NULL;
	aafd->Objects = NULL;
	// aafd->debug_callback = &debug_callback;
	aafd->dbg = dbg;


	aafd->cfbd = cfb_alloc( dbg );

	if ( aafd->cfbd == NULL ) {
		return NULL;
	}

	// aafd->cfbd->verb = aafd->verb;

	return aafd;
}



int aaf_load_file( AAF_Data *aafd, const char *file )
{
	if ( file == NULL )
		return 1;

	aafd->Objects = NULL;
	aafd->Classes = NULL;


	if ( cfb_load_file( &aafd->cfbd, file ) < 0 ) {
		return 1;
	}

	/*
	 *	NOTE: at least Avid Media Composer doesn't respect
	 *	the standard clsid AAFFileKind_Aaf4KBinary identifier.
	 *	Therefore isValidAAF() is useless until futher findings..
	 */

	// if ( isValidAAF( aafd ) == 0 ) {
	// 	return 1;
	// }

	if ( setDefaultClasses( aafd ) < 0 ) {
		return -1;
	}

	if ( retrieveObjectTree( aafd ) < 0 ) {
		return -1;
	}

	if ( parse_Header( aafd ) < 0 ) {
		return -1;
	}

	if ( parse_Identification( aafd ) < 0 ) {
		return -1;
	}

	return 0;
}



void aaf_release( AAF_Data **aafd )
{
	if ( aafd == NULL || *aafd == NULL )
		return;


	if ( (*aafd)->cfbd != NULL )
		cfb_release( &((*aafd)->cfbd) );


	aafClass *Class    = NULL;
	aafClass *tmpClass = NULL;

	for ( Class = (*aafd)->Classes; Class != NULL; Class = tmpClass )
	{
		tmpClass = Class->next;

		aafPropertyDef *PDef    = NULL;
		aafPropertyDef *tmpPDef = NULL;

		if ( Class->name != NULL ) {
			free( Class->name );
		}

		for ( PDef = Class->Properties; PDef != NULL; PDef = tmpPDef )
		{
			tmpPDef = PDef->next;

			// if ( PDef->meta ) {
				if ( PDef->name != NULL )
					free( PDef->name );
			// }

			free( PDef );
		}

		free( Class );
	}


	aafObject *Object    = NULL;
	aafObject *tmpObject = NULL;

	for ( Object = (*aafd)->Objects; Object != NULL; Object = tmpObject )
	{
		tmpObject = Object->nextObj;

		if ( Object->Header != NULL )
			free( Object->Header );

		if ( Object->Entry  != NULL )
			free( Object->Entry );


		aafProperty *Prop    = NULL;
		aafProperty *tmpProp = NULL;

		for ( Prop = Object->Properties; Prop != NULL; Prop = tmpProp )
		{
			tmpProp = Prop->next;

			switch ( Prop->sf )
			{
				case SF_STRONG_OBJECT_REFERENCE:
				case SF_STRONG_OBJECT_REFERENCE_SET:
				case SF_STRONG_OBJECT_REFERENCE_VECTOR:
					break;

				default:
					free( Prop->val );
			}

			free( Prop );
		}

		free( Object );
	}


	if ( (*aafd)->Identification.CompanyName != NULL ) {
		free( (*aafd)->Identification.CompanyName );
	}

	if ( (*aafd)->Identification.ProductName != NULL ) {
		free( (*aafd)->Identification.ProductName );
	}

	if ( (*aafd)->Identification.ProductVersionString != NULL ) {
		free( (*aafd)->Identification.ProductVersionString );
	}

	if ( (*aafd)->Identification.Platform != NULL ) {
		free( (*aafd)->Identification.Platform );
	}

	/* free once in AAFIface */
	// if ( (*aafd)->dbg ) {
	// 	free_debug( (*aafd)->dbg );
	// }

	free( *aafd );

	*aafd = NULL;
}



wchar_t * aaf_get_ObjectPath( aafObject *Obj )
{
	static wchar_t path[CFB_PATH_NAME_SZ];

	uint32_t offset = CFB_PATH_NAME_SZ;
	path[--offset] = 0x0000; // NULL terminating byte

	while ( Obj != NULL )
	{
		for ( int i = wcslen(Obj->Name)-1; i >= 0 && offset > 0; i-- ) {
			path[--offset] = Obj->Name[i];
		}

		if ( offset == 0 )
			break;

		path[--offset] = '/';

		Obj = Obj->Parent;
	}

	return path + offset;
}



int aaf__foreach_ObjectInSet( aafObject **Obj, aafObject *head, const aafUID_t *filter )
{
	if ( *Obj == NULL )
		*Obj = head;
	else
		*Obj = (*Obj)->next;

	if ( filter != NULL )
		for (; *Obj != NULL; *Obj = (*Obj)->next )
			if ( aafUIDCmp( (*Obj)->Class->ID, filter ) )
				break;

	return ( *Obj == NULL ) ? 0 : 1;
}



aafObject * aaf_get_ObjectByWeakRef( aafObject *list, aafWeakRef_t *ref )
{
	/* TODO check _identificationSize mismatch */

	if ( list == NULL || list->Entry == NULL || ref == NULL )
		return NULL;

	// AAF_Data *aafd = list->aafd;

	// Target is a Reference Vector
	if ( list->Header->_identificationSize == 0 ) {
		// debug( "Has local key" );
		for (; list != NULL; list = list->next )
		{
			if ( list->Entry->_localKey == ref->_referencedPropertyIndex ) {
				// debug( "list->Entry->_localKey            : 0x%x", list->Entry->_localKey );
				// debug( "list->Header->_identificationSize : %u", list->Header->_identificationSize );
				// debug( "FOUND : 0x%x", list->Entry->_localKey );
				return list;
			}
		}
	}
	else // Target is a Reference Set
	{
		for (; list != NULL; list = list->next )
		{
			/* TODO is the following check important ? */
			// if ( list->Header->_identificationSize != ref->_identificationSize )
			// 	continue;

			if ( memcmp( list->Entry->_identification, ref->_identification, ref->_identificationSize ) == 0 )
				return list;
		}
	}

	// debug( "localKey : 0x%x", list->Entry->_localKey );
	return NULL;
}



aafObject * aaf_get_MobByID( aafObject *Mobs, aafMobID_t *MobID )
{
	aafObject *Mob = NULL;

	if ( MobID == NULL )
		return NULL;

	aaf_foreach_ObjectInSet( &Mob, Mobs, NULL ) {

		aafMobID_t *Current = aaf_get_propertyValue( Mob, PID_Mob_MobID, &AAFTypeID_MobIDType );

		if ( Current == NULL || aafMobIDCmp( Current, MobID ) )
			break;
	}

	return Mob;
}



aafObject * aaf_get_MobSlotBySlotID( aafObject *MobSlots, aafSlotID_t SlotID )
{
	aafObject *MobSlot = NULL;

	aaf_foreach_ObjectInSet( &MobSlot, MobSlots, NULL ) {

		aafSlotID_t *CurrentSlotID = aaf_get_propertyValue( MobSlot, PID_MobSlot_SlotID, &AAFTypeID_UInt32 );

		if ( CurrentSlotID == NULL || *CurrentSlotID == SlotID )
			break;
	}

	return MobSlot;
}



/*
 *	TODO Works when the property was retrieved from MetaDictionary. What if the property is standard ?
 */

aafPID_t aaf_get_PropertyIDByName( AAF_Data *aafd, const wchar_t *name )
{
	aafClass *Class = NULL;

	foreachClass( Class, aafd->Classes ) {

		aafPropertyDef *PDef = NULL;

		foreachPropertyDefinition( PDef, Class->Properties ) {
			if ( PDef->name != NULL && wcscmp( PDef->name, name ) == 0 ) {
				return PDef->pid;
			}
		}
	}

	return 0;
}



aafProperty * aaf_get_property( aafObject *Obj, aafPID_t pid )
{
	if ( Obj == NULL )
		return NULL;

	AAF_Data *aafd = Obj->aafd;

	aafProperty *Prop = NULL;

	for ( Prop = Obj->Properties; Prop != NULL; Prop = Prop->next )
		if ( Prop->pid == pid )
			break;


	if ( Prop == NULL ) {

		aafPropertyDef *PDef = getPropertyDefinitionByID( Obj->Class, pid );

		if ( PDef == NULL ) {
			warning( "Unknown property 0x%04x (%ls) of Class %ls", pid, PIDToText( aafd, pid ), ClassIDToText(aafd, Obj->Class->ID) );
			return NULL;
		}

		if ( PDef->isReq ) {
			error( "Could not retrieve %ls required property 0x%04x (%ls)", ClassIDToText(aafd, Obj->Class->ID), pid, PIDToText(aafd, pid) );
		} else {
			debug( "Could not retrieve %ls optional property 0x%04x (%ls)", ClassIDToText(aafd, Obj->Class->ID), pid, PIDToText(aafd, pid) );
		}
	}

	return Prop;
}



void * aaf_get_propertyValue( aafObject *Obj, aafPID_t pid, const aafUID_t *typeID )
{
	if ( Obj == NULL ) {
		return NULL;
	}

	AAF_Data *aafd = Obj->aafd;
	aafProperty *Prop = aaf_get_property( Obj, pid );

	if ( Prop == NULL ) {
		return NULL;
	}

	void *value = Prop->val;
	uint16_t len = Prop->len;

	if ( Prop->sf == SF_DATA_STREAM || aafUIDCmp( typeID, &AAFTypeID_Indirect ) ) {
		/*
		 * DATA_STREAM stored form and IndirectValues start with a byte identifying byte order : 0x4c, 0x42, 0x55
		 * We must skip that byte.
		 */
		value = (void*)(((char*)value) + 1);
		len--;
	}

	if ( aafUIDCmp( typeID, &AAFTypeID_String ) ) {

		if ( ((uint16_t*)value)[(len/2)-1] != 0x0000 ) {
			error( "Object %ls string property 0x%04x (%ls) does not end with NULL", ClassIDToText(aafd, Obj->Class->ID), pid, PIDToText(aafd, pid) );
			return NULL;
		}

		return cfb_w16towchar( NULL, value, len );
	}

	if ( aafUIDCmp( typeID, &AAFTypeID_Indirect ) ) {

		/*
		 * In case of Indirect with string value we check NULL termination here,
		 * because when calling next aaf_get_indirectValue() we wont have access
		 * to Prop->len anymore.
		 */

		aafIndirect_t *Indirect = value;

		if ( aafUIDCmp( &Indirect->TypeDef, &AAFTypeID_String ) && ((uint16_t*)value)[(len/2)-1] != 0x0000 ) {
			error( "Object %ls Indirect::string property 0x%04x (%ls) does not end with NULL", ClassIDToText(aafd, Obj->Class->ID), pid, PIDToText(aafd, pid) );
			return NULL;
		}
	}

	if ( ( aafUIDCmp( typeID, &AAFTypeID_Boolean )         && len != sizeof(aafBoolean_t)        ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_Int8 )            && len != sizeof(int8_t)              ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_UInt8 )           && len != sizeof(uint8_t)             ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_Int16 )           && len != sizeof(int16_t)             ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_UInt16 )          && len != sizeof(uint16_t)            ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_Int32 )           && len != sizeof(int32_t)             ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_UInt32 )          && len != sizeof(uint32_t)            ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_Int64 )           && len != sizeof(int64_t)             ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_UInt64 )          && len != sizeof(uint64_t)            ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_PositionType )    && len != sizeof(aafPosition_t)       ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_LengthType )      && len != sizeof(aafLength_t)         ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_Rational )        && len != sizeof(aafRational_t)       ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_TimeStamp )       && len != sizeof(aafTimeStamp_t)      ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_VersionType )     && len != sizeof(aafVersionType_t)    ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_ProductVersion )  && len != sizeof(aafProductVersion_t) ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_UsageType )       && len != sizeof(aafUID_t)            ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_AUID )            && len != sizeof(aafUID_t)            ) ||
	     ( aafUIDCmp( typeID, &AAFTypeID_MobIDType )       && len != sizeof(aafMobID_t)          ) )
	{
		error( "Object %ls property 0x%04x (%ls) size (%u) does not match type %ls", ClassIDToText(aafd, Obj->Class->ID), pid, PIDToText(aafd, pid), len, TypeIDToText(typeID) );
		return NULL;
	}

	return value;
}



void * aaf_get_indirectValue( AAF_Data *aafd, aafIndirect_t *Indirect, const aafUID_t *typeDef )
{
	if ( Indirect == NULL ) {
		error( "Indirect is NULL" );
		return NULL;
	}

	if ( typeDef && aafUIDCmp( &Indirect->TypeDef, typeDef ) == 0 ) {
		error( "Requested Indirect value of type %ls but has type %ls", TypeIDToText(typeDef), TypeIDToText(&Indirect->TypeDef) );
		return NULL;
	}

	if ( aafUIDCmp( typeDef, &AAFTypeID_String ) ) {
		/*
		 * Indirect->Value is guaranted by aaf_get_property() to be NULL terminated
		 * so we can safely use CFB_W16TOWCHAR_STRLEN
		 */
		return cfb_w16towchar( NULL, (uint16_t*)Indirect->Value, CFB_W16TOWCHAR_STRLEN/*(Prop->len - 1 - sizeof(aafIndirect_t))*/ );
	}

	return &Indirect->Value;
}











static int parse_Header( AAF_Data *aafd )
{
	aafObject *Header = aafd->Header.obj;

	if ( Header == NULL ) {
		error( "Missing Header Object." );
		return -1;
	}


	int16_t *ByteOrder = aaf_get_propertyValue( Header, PID_Header_ByteOrder, &AAFTypeID_Int16 );

	if ( ByteOrder == NULL ) {
		warning( "Missing Header::ByteOrder." );
	}

	aafd->Header.ByteOrder = *ByteOrder;


	aafTimeStamp_t *LastModified = aaf_get_propertyValue( Header, PID_Header_LastModified, &AAFTypeID_TimeStamp );

	if ( LastModified == NULL ) {
		warning( "Missing Header::LastModified." );
	}

	aafd->Header.LastModified = LastModified;


	aafVersionType_t *Version = aaf_get_propertyValue( Header, PID_Header_Version, &AAFTypeID_VersionType );

	if ( Version == NULL ) {
		warning( "Missing Header::Version." );
	}

	aafd->Header.Version = Version;


	uint32_t *ObjectModelVersion = aaf_get_propertyValue( Header, PID_Header_ObjectModelVersion, &AAFTypeID_UInt32 );

	if ( ObjectModelVersion == NULL ) {
		warning( "Missing Header::ObjectModelVersion." );
	}

	aafd->Header.ObjectModelVersion = *ObjectModelVersion;


	const aafUID_t *OperationalPattern = aaf_get_propertyValue( Header, PID_Header_OperationalPattern, &AAFTypeID_AUID );

	if ( OperationalPattern == NULL ) {
		warning( "Missing Header::OperationalPattern." );
		OperationalPattern = (const aafUID_t*)&AUID_NULL;
	}

	aafd->Header.OperationalPattern = OperationalPattern;


	return 0;
}



static int parse_Identification( AAF_Data *aafd )
{
	aafObject *Identif = aafd->Identification.obj;

	if ( Identif == NULL ) {
		error( "Missing Identification Object." );
		return -1;
	}


	wchar_t *Company = aaf_get_propertyValue( Identif, PID_Identification_CompanyName, &AAFTypeID_String );

	if ( Company == NULL ) {
		warning( "Missing Identification::CompanyName." );
	}

	aafd->Identification.CompanyName = Company;


	wchar_t *ProductName = aaf_get_propertyValue( Identif, PID_Identification_ProductName, &AAFTypeID_String );

	if ( ProductName == NULL ) {
		warning( "Missing Identification::ProductName." );
	}

	aafd->Identification.ProductName = ProductName;


	aafProductVersion_t *ProductVersion = aaf_get_propertyValue( Identif, PID_Identification_ProductVersion, &AAFTypeID_ProductVersion );

	if ( ProductVersion == NULL ) {
		warning( "Missing Identification::ProductVersion." );
	}

	aafd->Identification.ProductVersion = ProductVersion;


	wchar_t *ProductVersionString = aaf_get_propertyValue( Identif, PID_Identification_ProductVersionString, &AAFTypeID_String );

	if ( ProductVersionString == NULL ) {
		warning( "Missing Identification::ProductVersionString." );
	}

	aafd->Identification.ProductVersionString = ProductVersionString;


	aafUID_t *ProductID = aaf_get_propertyValue( Identif, PID_Identification_ProductID, &AAFTypeID_AUID );

	if ( ProductID == NULL ) {
		warning( "Missing Identification::ProductID." );
	}

	aafd->Identification.ProductID = ProductID;


	aafTimeStamp_t *Date = aaf_get_propertyValue( Identif, PID_Identification_Date, &AAFTypeID_TimeStamp );

	if ( Date == NULL ) {
		warning( "Missing Identification::Date." );
	}

	aafd->Identification.Date = Date;


	aafProductVersion_t *ToolkitVersion = aaf_get_propertyValue( Identif, PID_Identification_ToolkitVersion, &AAFTypeID_ProductVersion );

	if ( ToolkitVersion == NULL ) {
		warning( "Missing Identification::ToolkitVersion." );
	}

	aafd->Identification.ToolkitVersion = ToolkitVersion;


	wchar_t *Platform = aaf_get_propertyValue( Identif, PID_Identification_Platform, &AAFTypeID_String );

	if ( Platform == NULL ) {
		warning( "Missing Identification::Platform." );
	}

	aafd->Identification.Platform = Platform;


	aafUID_t *GenerationAUID = aaf_get_propertyValue( Identif, PID_Identification_GenerationAUID, &AAFTypeID_AUID );

	if ( GenerationAUID == NULL ) {
		warning( "Missing Identification::GenerationAUID." );
	}

	aafd->Identification.GenerationAUID = GenerationAUID;


	return 0;
}



// static int isValidAAF( AAF_Data *aafd )
// {
// 	aafUID_t *hdrClsID = (aafUID_t*)&aafd->cfbd->hdr->_clsid;
//
// 	if ( aafUIDCmp( hdrClsID, &AAFFileKind_Aaf512Binary ) ||
// 		 aafUIDCmp( hdrClsID, &AAFFileKind_Aaf4KBinary  ) )
// 			return 1;
//
// //  warning( "Unsuported AAF encoding (%ls).", FileKindToText( hdrClsID ) );
//
// 	return 0;
// }



static void setObjectShortcuts( AAF_Data *aafd )
{
//  aafd->Root = aafd->Root;

	aafd->Header.obj              = aaf_get_propertyValue( aafd->Root,       PID_Root_Header, &AAFUID_NULL );
//  aafd->MetaDictionary          = aaf_get_propertyValue( aafd->Root,       PID_Root_MetaDictionary                 );

	aafd->ClassDefinition         = aaf_get_propertyValue( aafd->MetaDictionary, PID_MetaDictionary_ClassDefinitions, &AAFTypeID_ClassDefinitionStrongReferenceSet );
	aafd->TypeDefinition          = aaf_get_propertyValue( aafd->MetaDictionary, PID_MetaDictionary_TypeDefinitions,  &AAFTypeID_TypeDefinitionStrongReferenceSet  );

	aafd->Identification.obj      = aaf_get_propertyValue( aafd->Header.obj, PID_Header_IdentificationList, &AAFTypeID_IdentificationStrongReferenceVector         );
	aafd->Content                 = aaf_get_propertyValue( aafd->Header.obj, PID_Header_Content,            &AAFTypeID_ContentStorageStrongReference               );
	aafd->Dictionary              = aaf_get_propertyValue( aafd->Header.obj, PID_Header_Dictionary,         &AAFTypeID_DictionaryStrongReference                   );

	aafd->Mobs                    = aaf_get_propertyValue( aafd->Content,    PID_ContentStorage_Mobs,        &AAFTypeID_MobStrongReferenceSet                      );
	aafd->EssenceData             = aaf_get_propertyValue( aafd->Content,    PID_ContentStorage_EssenceData, &AAFTypeID_EssenceDataStrongReferenceSet              );

	aafd->OperationDefinition     = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_OperationDefinitions,     &AAFTypeID_OperationDefinitionStrongReferenceSet     );
	aafd->ParameterDefinition     = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_ParameterDefinitions,     &AAFTypeID_ParameterDefinitionStrongReferenceSet     );
	aafd->DataDefinition          = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_DataDefinitions,          &AAFTypeID_DataDefinitionStrongReferenceSet          );
	aafd->PluginDefinition        = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_PluginDefinitions,        &AAFTypeID_PluginDefinitionStrongReferenceSet        );
	aafd->CodecDefinition         = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_CodecDefinitions,         &AAFTypeID_CodecDefinitionStrongReferenceSet         );
	aafd->ContainerDefinition     = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_ContainerDefinitions,     &AAFTypeID_ContainerDefinitionStrongReferenceSet     );
	aafd->InterpolationDefinition = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_InterpolationDefinitions, &AAFTypeID_InterpolationDefinitionStrongReferenceSet );
	aafd->KLVDataDefinition       = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_KLVDataDefinitions,       &AAFTypeID_KLVDataDefinitionStrongReferenceSet       );
	aafd->TaggedValueDefinition   = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_TaggedValueDefinitions,   &AAFTypeID_TaggedValueDefinitionStrongReferenceSet   );

}



static int retrieveObjectTree( AAF_Data *aafd )
{
	int rc = 0;

	cfbNode *Node  = aafd->cfbd->nodes[0];

	aafClass *Class = getClassByID( aafd, (aafUID_t*)&Node->_clsId );


	if ( Class == NULL && aafUIDCmp( Class->ID, (aafUID_t*)&Node->_clsId ) != 0 ) {
		error( "Looks like the fist Object is not the Root Class : %ls.", ClassIDToText( aafd, Class->ID ) );
		return -1;
	}


	aafd->Root = newObject( aafd, Node, Class, NULL );

	if ( aafd->Root == NULL ) {
		return -1;
	}



	/******************* retrieveObjectProperties() *********************/

	aafPropertyIndexHeader_t *Header = getNodeProperties( aafd, aafd->Root->Node );
	aafPropertyIndexEntry_t  *Prop   = NULL;

	aafPropertyIndexEntry_t  *AAFHeaderProp = NULL;
	aafPropertyIndexEntry_t  *AAFMetaDcProp = NULL;

	aafByte_t      *AAFHeaderVal  = NULL;
	aafByte_t      *AAFMetaDcVal  = NULL;

	aafByte_t      *value         = NULL;

	aafPropertyDef *PDef          = NULL;

	if ( Header == NULL ) {
		error( "Could not retrieve properties for %ls.", aaf_get_ObjectPath( aafd->Root ) );
		return -1;
	}


	uint32_t i = 0;


	foreachPropertyEntry( Header, Prop, value, i ) {

		if ( Prop->_pid == PID_Root_Header ) {
			AAFHeaderProp = Prop;
			AAFHeaderVal  = value;
		}

		if ( Prop->_pid == PID_Root_MetaDictionary ) {
			AAFMetaDcProp = Prop;
			AAFMetaDcVal  = value;
		}
	}

	PDef = getPropertyDefinitionByID( aafd->Root->Class, PID_Root_MetaDictionary );

	/* Start recursive parsing of /Root/Header/{*} */

	rc = retrieveProperty( aafd, aafd->Root, PDef, AAFMetaDcProp, AAFMetaDcVal, Header->_byteOrder );

	if ( rc < 0 ) {
		error( "Could not retrieve property %ls.", PIDToText( aafd, PDef->pid ) );
		return -1;
	}

	// aaf_dump_rawProperties( aafd, Header );

	/*
	 *	Retrieve MetaDictionary.
	 */

	aafObject *MetaDic   = aaf_get_propertyValue( aafd->Root, PID_Root_MetaDictionary, &AAFUID_NULL );

	if ( MetaDic == NULL ) {
		error( "Missing PID_Root_MetaDictionary." );
		return -1;
	}


	aafObject *ClassDefs = aaf_get_propertyValue( MetaDic, PID_MetaDictionary_ClassDefinitions, &AAFTypeID_ClassDefinitionStrongReferenceSet );

	if ( ClassDefs == NULL ) {
		error( "Missing PID_MetaDictionary_ClassDefinitions." );
		return -1;
	}


	aafObject *ClassDef  = NULL;

	aaf_foreach_ObjectInSet( &ClassDef, ClassDefs, NULL )
		retrieveMetaDictionaryClass( aafd, ClassDef );



	PDef = getPropertyDefinitionByID( aafd->Root->Class, PID_Root_Header );

	/* Starts recursive parsing of /Root/Header/{*} */

	rc = retrieveProperty( aafd, aafd->Root, PDef, AAFHeaderProp, AAFHeaderVal, Header->_byteOrder );

	if ( rc < 0 ) {
		error( "Could not retrieve property %ls.", PIDToText( aafd, PDef->pid ) );
		return -1;
	}

	free( Header );



	setObjectShortcuts( aafd );


	return 0;
}



static aafClass * retrieveMetaDictionaryClass( AAF_Data *aafd, aafObject *TargetClassDef )
{
	aafObject *MetaDic = aaf_get_propertyValue( aafd->Root, PID_Root_MetaDictionary, &AAFUID_NULL );



	aafObject *ClassDefs = aaf_get_propertyValue( MetaDic, PID_MetaDictionary_ClassDefinitions, &AAFTypeID_ClassDefinitionStrongReferenceSet );
	aafObject *ClassDef  = NULL;

	aaf_foreach_ObjectInSet( &ClassDef, ClassDefs, NULL )
		if ( ClassDef == TargetClassDef )
			break;

	if ( ClassDef == NULL ) {
		error( "Could not retrieve ClassDefinition." );
		return NULL;
	}


	aafUID_t    *ClassID = aaf_get_propertyValue( ClassDef, PID_MetaDefinition_Identification, &AAFTypeID_AUID );

	aafWeakRef_t *parent = aaf_get_propertyValue( ClassDef, PID_ClassDefinition_ParentClass, &AAFTypeID_ClassDefinitionWeakReference );
	aafObject    *Parent = aaf_get_ObjectByWeakRef( ClassDefs, parent );


	aafClass *ParentClass = NULL;

	if ( Parent != ClassDef ) {
		ParentClass = retrieveMetaDictionaryClass( aafd, Parent );
	}
	else if ( aafUIDCmp( ClassID, &AAFClassID_InterchangeObject ) == 0 &&
	          aafUIDCmp( ClassID, &AAFClassID_MetaDefinition    ) == 0 &&
	          aafUIDCmp( ClassID, &AAFClassID_MetaDictionary    ) == 0 )
	{
		/*
		 *	TODO: what is this ? when does it happen ?
		 */
		error( "Parent's Class equals Child's : %ls.", ClassIDToText( aafd, ClassID ) );
		return NULL;
	}


	aafClass *Class = getClassByID( aafd, ClassID );

	if ( Class == NULL ) {

		aafBoolean_t *isCon = aaf_get_propertyValue( ClassDef, PID_ClassDefinition_IsConcrete, &AAFTypeID_Boolean );

		if ( isCon == NULL ) {
			error( "Missing ClassDefinition::IsConcrete." );
			return NULL;
		}

		Class = defineNewClass( aafd, ClassID, *isCon, ParentClass );

		Class->name = aaf_get_propertyValue( ClassDef, PID_MetaDefinition_Name, &AAFTypeID_String );
		Class->meta = 1;
	}
	else {	// if class is standard, we only set its name
		if ( Class->name == NULL ) {
			Class->name = aaf_get_propertyValue( ClassDef, PID_MetaDefinition_Name, &AAFTypeID_String );
		}
	}


	aafObject *Props = aaf_get_propertyValue( ClassDef, PID_ClassDefinition_Properties, &AAFTypeID_PropertyDefinitionStrongReferenceSet );
	aafObject *Prop  = NULL;

	aaf_foreach_ObjectInSet( &Prop, Props, NULL ) {

		aafPID_t *Pid = aaf_get_propertyValue( Prop, PID_PropertyDefinition_LocalIdentification, &AAFTypeID_UInt16 );

		if ( Pid == NULL ) {
			error( "Missing PropertyDefinition::LocalIdentification." );
			return NULL;
		}


		aafBoolean_t *isOpt = aaf_get_propertyValue( Prop, PID_PropertyDefinition_IsOptional, &AAFTypeID_Boolean );

		if ( isOpt == NULL ) {
			error( "Missing PropertyDefinition::IsOptional." );
			return NULL;
		}

		/*
		 *	We skip all the properties that were already defined in setDefaultClasses().
		 */

		aafPropertyDef *PDef = NULL;

		if ( !(PDef = propertyIdExistsInClass( Class, *Pid )) ) {
			attachNewProperty( Class, PDef, *Pid, ( *isOpt ) ? 0 : 1 );
			PDef->meta = 1;
		}
		else {
			// debug( "Property %d exists.", *Pid );
			continue;
		}

		PDef->name = aaf_get_propertyValue( Prop, PID_MetaDefinition_Name, &AAFTypeID_String );


		aafObject *TypeDefs = aaf_get_propertyValue( MetaDic, PID_MetaDictionary_TypeDefinitions, &AAFTypeID_TypeDefinitionStrongReferenceSet );

		if ( TypeDefs == NULL ) {
			error( "Missing TypeDefinitions from MetaDictionary" );
			return NULL;
		}


		aafWeakRef_t *WeakRefToType = aaf_get_propertyValue( Prop, PID_PropertyDefinition_Type, &AAFTypeID_PropertyDefinitionWeakReference );

		if ( WeakRefToType == NULL ) {
			error( "Missing PID_PropertyDefinition_Type" );
			return NULL;
		}


		aafObject *TypeDef = aaf_get_ObjectByWeakRef( TypeDefs, WeakRefToType );

		if ( TypeDef == NULL ) {
			error( "Could not retrieve TypeDefinition from dictionary." );
			return NULL;
		}


		aafUID_t *typeUID = aaf_get_propertyValue( TypeDef, PID_MetaDefinition_Identification, &AAFTypeID_AUID );

		if ( typeUID == NULL ) {
			error( "Missing PID_MetaDefinition_Identification" );
			return NULL;
		}

		/*
		 *  Looks like nobody cares about AAF standard TypeDefinition. All observed files
		 *	had incorrect values for Type Name and Identification, even Avid's files. So...
		 */

		memcpy( &PDef->type, typeUID, sizeof(aafUID_t) );


		// wchar_t *typeName = aaf_get_propertyValue( TypeDef, PID_MetaDefinition_Name, &AAFTypeID_String );
		//
		// debug( "TypeName :  %ls (%ls) |  name : %ls.",
		// // AUIDToText(typeUID),
		// 	typeName,
		// 	TypeIDToText( typeUID ),
		// 	PDef->name );
		//
		// free( typeName );
	}

	return Class;
}



static aafObject * newObject( AAF_Data *aafd, cfbNode *Node, aafClass *Class, aafObject *Parent )
{
	aafObject *Obj = calloc( sizeof(aafObject), sizeof(unsigned char) );

	if ( Obj == NULL ) {
		error( "%s.", strerror( errno ) );
		return NULL;
	}

	cfb_w16towchar( Obj->Name, Node->_ab, Node->_cb );

	Obj->aafd       = aafd;
	Obj->Class      = Class;
	Obj->Node       = Node;
	Obj->Properties = NULL;
	Obj->Parent     = Parent;
	Obj->Header     = NULL;
	Obj->Entry      = NULL;

	Obj->next       = NULL;
	Obj->prev       = NULL;
	Obj->nextObj    = aafd->Objects;
	aafd->Objects   = Obj;

	return Obj;
}



static aafProperty * newProperty( AAF_Data *aafd, aafPropertyDef *Def )
{
	aafProperty *Prop = calloc( sizeof(aafProperty), sizeof(unsigned char) );

	if ( Prop == NULL ) {
		error( "%s.", strerror( errno ) );
		return NULL;
	}

	Prop->pid = Def->pid;
	Prop->def = Def;

	return Prop;
}



static aafPropertyDef * propertyIdExistsInClass( aafClass *Class, aafPID_t Pid )
{
	aafPropertyDef *PDef = NULL;

	foreachPropertyDefinition( PDef, Class->Properties )
		if ( PDef->pid == Pid )
			return PDef;

	return NULL;
}



static int setObjectStrongRefSet( aafObject *Obj, aafStrongRefSetHeader_t *Header, aafStrongRefSetEntry_t *Entry )
{
	AAF_Data *aafd = Obj->aafd;

	Obj->Header = malloc( sizeof(aafStrongRefSetHeader_t) );

	if ( Obj->Header == NULL ) {
		error( "%s.", strerror( errno ) );
		return -1;
	}

	memcpy( Obj->Header, Header, sizeof(aafStrongRefSetHeader_t)  );


	/* True entrySize, taking _identification into account. */
	uint32_t entrySize = sizeof(aafStrongRefSetEntry_t) + Header->_identificationSize;

	Obj->Entry = malloc( entrySize );

	if ( Obj->Entry == NULL ) {
		error( "%s.", strerror( errno ) );
		return -1;
	}

	memcpy( Obj->Entry,  Entry,  entrySize   );

	return 0;
}



static int setObjectStrongRefVector( aafObject *Obj, aafStrongRefVectorHeader_t *Header, aafStrongRefVectorEntry_t *Entry )
{
	/*
	 * aafStrongRefVectorHeader_t  and  aafStrongRefSetHeader_t begins with the same
	 * data bytes,  so  we  can  safely  memcpy to the first one from the second one,
	 * the remaining bytes simply remaining null.
	 * The same applies  to aafStrongRefVectorEntry_t and aafStrongRefVectorHeader_t.
	 */

	AAF_Data *aafd = Obj->aafd;

	Obj->Header = calloc( sizeof(aafStrongRefSetHeader_t), sizeof(unsigned char) );

	if ( Obj->Header == NULL ) {
		error( "%s.", strerror( errno ) );
		return -1;
	}

	memcpy( Obj->Header, Header, sizeof(aafStrongRefVectorHeader_t)  );


	Obj->Entry = calloc( sizeof(aafStrongRefSetEntry_t), sizeof(unsigned char) );

	if ( Obj->Entry == NULL ) {
		error( "%s.", strerror( errno ) );
		return -1;
	}

	memcpy( Obj->Entry, Entry,  sizeof(aafStrongRefVectorEntry_t)   );


	return 0;
}



static int retrieveStrongReference( AAF_Data *aafd, aafProperty *Prop, aafObject *Parent )
{
	/*
	 * Initial property value is a wchar string holding the name of a child node.
	 * This child node being the object referenced, we store that object dirctly
	 * as the property value, instead of the initial child node name.
	 */

	wchar_t name[CFB_NODE_NAME_SZ];

	cfb_w16towchar( name, Prop->val, Prop->len );

	free( Prop->val );
	Prop->val = NULL;

	cfbNode *Node  = cfb_getChildNode( aafd->cfbd, name, Parent->Node );

	if ( Node == NULL ) {
		error( "Could not find child node." );
		return -1;
	}


	aafClass *Class = getClassByID( aafd, (aafUID_t*)&Node->_clsId );

	if ( Class == NULL ) {
		error( "Could not retrieve Class %ls @ \"%ls\".", ClassIDToText( aafd, (aafUID_t*)&Node->_clsId ), aaf_get_ObjectPath( Parent ) );
		return -1;
	}


	Prop->val = newObject( aafd, Node, Class, Parent );

	if ( Prop->val == NULL ) {
		return -1;
	}


	int rc = retrieveObjectProperties( aafd, Prop->val );

	if ( rc < 0 ) {
		return -1;
	}

	return 0;
}



static int retrieveStrongReferenceSet( AAF_Data *aafd, aafProperty *Prop, aafObject *Parent )
{
	wchar_t refName[CFB_NODE_NAME_SZ];

	cfb_w16towchar( refName, Prop->val, Prop->len );

	free( Prop->val );
	Prop->val = NULL;


	cfbNode * Node = getStrongRefIndexNode( aafd, Parent, refName );

	if ( Node == NULL ) {
		error( "Could not retrieve StrongReferenceSet's Index node." );
		return -1;
	}

	aafStrongRefSetHeader_t *Header = getStrongRefSetList( aafd, Node, Parent );
	aafStrongRefSetEntry_t  *Entry  = NULL;

	if ( Header == NULL ) {
		error( "Could not retrieve StrongReferenceSet's CFB Stream." );
		return -1;
	}

	uint32_t i = 0;
	int rc = 0;

	foreachStrongRefSetEntry( Header, Entry, i ) {

		Node = getStrongRefEntryNode( aafd, Parent, refName, Entry->_localKey );

		if ( Node == NULL ) {
			continue;
		}

		aafClass *Class = getClassByID( aafd, (aafUID_t*)&Node->_clsId );

		if ( Class == NULL ) {
			error( "Could not retrieve Class %ls.", ClassIDToText( aafd, (aafUID_t*)&Node->_clsId ) );
			continue;
		}

		aafObject * Obj = newObject( aafd, Node, Class, Parent );

		if ( Obj == NULL ) {
			return -1;
		}

		rc = setObjectStrongRefSet( Obj, Header, Entry );

		if ( rc < 0 ) {
			return -1;
		}

		rc = retrieveObjectProperties( aafd, Obj );

		if ( rc < 0 ) {
			return -1;
		}

		Obj->next = Prop->val;
		Prop->val = Obj;
	}

	if ( Header != NULL )
		free( Header );

	return 0;
}



static int retrieveStrongReferenceVector( AAF_Data *aafd, aafProperty *Prop, aafObject *Parent )
{
	wchar_t refName[CFB_NODE_NAME_SZ];

	cfb_w16towchar( refName, Prop->val, Prop->len );

	free( Prop->val );
	Prop->val = NULL;


	cfbNode * Node = getStrongRefIndexNode( aafd, Parent, refName );

	if ( Node == NULL ) {
		return -1;
	}

	aafStrongRefVectorHeader_t *Header = getStrongRefVectorList( aafd, Node, Parent );
	aafStrongRefVectorEntry_t  *Entry  = NULL;

	if ( Header == NULL ) {
		return -1;
	}


	uint32_t i = 0;
	int rc = 0;

	foreachStrongRefVectorEntry( Header, Entry, i ) {

		Node = getStrongRefEntryNode( aafd, Parent, refName, Entry->_localKey );

		if ( Node == NULL ) {
			continue;
		}

		aafClass * Class = getClassByID( aafd, (aafUID_t*)&Node->_clsId );

		if ( Class == NULL ) {
			warning( "Could not retrieve Class ID %ls.", ClassIDToText( aafd, (aafUID_t*)&Node->_clsId ) );
			continue;
		}

		aafObject * Obj = newObject( aafd, Node, Class, Parent );

		if ( Obj == NULL ) {
			return -1;
		}

		rc = setObjectStrongRefVector( Obj, Header, Entry );

		if ( rc < 0 ) {
			return -1;
		}

		rc = retrieveObjectProperties( aafd, Obj );

		if ( rc < 0 ) {
			return -1;
		}


		/*
		 *	Vector are ordered.
		 */

		if ( Prop->val != NULL ) {
			aafObject *tmp = Prop->val;

			for (; tmp != NULL; tmp = tmp->next)
				if ( tmp->next == NULL )
					break;

			Obj->prev = tmp;
			tmp->next = Obj;
		}
		else
		{
			Obj->prev = NULL;
			Prop->val = Obj;
		}

	}

	if ( Header != NULL )
		free( Header );

	return 0;
}



static int retrieveProperty( AAF_Data *aafd, aafObject *Obj, aafPropertyDef *Def, aafPropertyIndexEntry_t *p, aafByte_t *v, uint8_t bo )
{
	(void)bo; // TODO: ByteOrder support ?

	aafProperty *Prop = newProperty( aafd, Def );

	if ( Prop == NULL ) {
		return -1;
	}

	Prop->sf = p->_storedForm;

	/*
		TODO Prop->len / Prop->val ---> retrieveStrongReference() retrieveStrongReferenceSet() retrieveStrongReferenceVector()
		only used to retrieve node name ? There could be a better approach.
	 */

	Prop->len = p->_length;

	Prop->val = malloc( p->_length );

	if ( Prop->val == NULL ) {
		error( "%s.", strerror( errno ) );
		return -1;
	}

	memcpy( Prop->val, v, p->_length );


	Prop->next = Obj->Properties;
	Obj->Properties = Prop;

	switch ( p->_storedForm )
	{
		case SF_STRONG_OBJECT_REFERENCE:
			return retrieveStrongReference( aafd, Prop, Obj );

		case SF_STRONG_OBJECT_REFERENCE_SET:
			return retrieveStrongReferenceSet( aafd, Prop, Obj );

		case SF_STRONG_OBJECT_REFERENCE_VECTOR:
			return retrieveStrongReferenceVector( aafd, Prop, Obj );

		default: break;
	}

	return 0;
}



static int retrieveObjectProperties( AAF_Data *aafd, aafObject *Obj )
{
	int rc = 0;

	aafPropertyIndexHeader_t *Header = getNodeProperties( aafd, Obj->Node );
	aafPropertyIndexEntry_t  *Prop   = NULL;

	aafByte_t      *value  = NULL;
	aafPropertyDef *PDef   = NULL;

	if ( Header == NULL ) {
		error( "Could not retrieve object %ls properties : %ls", ClassIDToText(aafd, Obj->Class->ID), aaf_get_ObjectPath( Obj ) );
		return -1;
	}


	uint32_t i = 0;


	foreachPropertyEntry( Header, Prop, value, i ) {

		PDef = getPropertyDefinitionByID( Obj->Class, Prop->_pid );

		if ( PDef == NULL ) {
			warning( "Unknown property 0x%04x (%ls) of object %ls", Prop->_pid, PIDToText( aafd, Prop->_pid ), ClassIDToText(aafd, Obj->Class->ID) );
			continue;
		}


		rc = retrieveProperty( aafd, Obj, PDef, Prop, value, Header->_byteOrder );

		if ( rc < 0 ) {
			error( "Could not retrieve property %ls of object %ls", PIDToText( aafd, PDef->pid ), ClassIDToText(aafd, Obj->Class->ID) );
			return -1;
		}

	}

	free( Header );

	return 0;
}



static cfbNode * getStrongRefIndexNode( AAF_Data *aafd, aafObject *Parent, const wchar_t *refName )
{
	wchar_t name[CFB_NODE_NAME_SZ];

	swprintf( name, CFB_NODE_NAME_SZ, L"%" WPRIws L" index", refName );

	cfbNode * Node = cfb_getChildNode( aafd->cfbd, name, Parent->Node );

	if ( Node == NULL ) {
		error( "Could not retrieve Reference Set/Vector Index Node @ \"%ls/%ls index\"", aaf_get_ObjectPath( Parent ), refName );
		return NULL;
	}

	return Node;
}



static cfbNode * getStrongRefEntryNode( AAF_Data *aafd, aafObject *Parent, const wchar_t *refName, uint16_t index )
{
	wchar_t name[CFB_NODE_NAME_SZ];

	swprintf( name, CFB_NODE_NAME_SZ, L"%" WPRIws L"{%x}", refName, index );

	cfbNode *Node = cfb_getChildNode( aafd->cfbd, name, Parent->Node );

	if ( Node == NULL ) {
		error( "Could not retrieve Reference Set/vector Entry Node @ \"%ls/%ls index\"", aaf_get_ObjectPath( Parent ), refName );
		return NULL;
	}

	return Node;
}



static aafPropertyIndexHeader_t * getNodeProperties( AAF_Data *aafd, cfbNode *Node )
{
	if ( Node == NULL ) {
		error( "Node is NULL" );
		return NULL;
	}

	uint64_t   stream_sz = 0;
	aafByte_t *stream    = NULL;


	cfbNode  *propNode = cfb_getChildNode( aafd->cfbd, L"properties", Node );

	if ( propNode == NULL ) {
		error( "Could not retrieve Property Node" );
		return NULL;
	}


	cfb_getStream( aafd->cfbd, propNode, &stream, &stream_sz );

	if ( stream == NULL ) {
		error( "Could not retrieve Property Stream" );
		return NULL;
	}


	/*
	 *  Ensures PropHeader + all PropEntries + all PropValues matches the Stream size.
	 *  TODO : is the following test important ?
	 */

/*
	uint32_t prop_sz = sizeof(aafPropertyIndexHeader_t);

	uint32_t i = 0;

	for ( i = 0; i < ((aafPropertyIndexHeader_t*)stream)->_entryCount; i++ )
		prop_sz += (((aafPropertyIndexEntry_t*)(stream+((sizeof(aafPropertyIndexEntry_t)*i)+sizeof(aafPropertyIndexHeader_t))))->_length) + sizeof(aafPropertyIndexEntry_t);

	if ( prop_sz != stream_sz )
		warning( "Stream length (%lu Bytes) does not match property length (%u Bytes).",
			stream_sz,
			prop_sz );
*/

	return (aafPropertyIndexHeader_t*)stream;
}



static aafStrongRefSetHeader_t * getStrongRefSetList( AAF_Data *aafd, cfbNode *Node, aafObject *Parent )
{
	if ( Node == NULL )
		return NULL;

	aafByte_t *stream = NULL;
	uint64_t   stream_sz = 0;

	cfb_getStream( aafd->cfbd, Node, &stream, &stream_sz );

	if ( stream == NULL ) {
		wchar_t refName[CFB_NODE_NAME_SZ];

		cfb_w16towchar( refName, Node->_ab, Node->_cb );

		error( "Could not retrieve StrongReferenceSet Index Stream @ \"%ls/%ls index\"", aaf_get_ObjectPath( Parent ), refName );

		return NULL;
	}

	return (aafStrongRefSetHeader_t*)stream;
}



static aafStrongRefVectorHeader_t * getStrongRefVectorList( AAF_Data *aafd, cfbNode *Node, aafObject *Parent )
{
	if ( Node == NULL )
		return NULL;

	aafByte_t *stream = NULL;
	uint64_t   stream_sz = 0;

	cfb_getStream( aafd->cfbd, Node, &stream, &stream_sz );

	if ( stream == NULL ) {
		wchar_t refName[CFB_NODE_NAME_SZ];

		cfb_w16towchar( refName, Node->_ab, Node->_cb );

		error( "Could not retrieve StrongReferenceVector Index Stream \"%ls/%ls index\"", aaf_get_ObjectPath( Parent ), refName );
		return NULL;
	}

	return (aafStrongRefVectorHeader_t*)stream;
}

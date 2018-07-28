/*
 *  This file is part of LibAAF.
 *
 *  Copyright (c) 2017 Adrien Gesta-Fline
 *
 *  LibAAF is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  LibAAF is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with LibAAF. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "AAFTypes.h"
#include "AAFCore.h"
#include "AAFClass.h"
//#include "AAFNavigation.h"
#include "AAFUtils.h"
#include "AAFToText.h"

#include "AAFDefs/AAFClassDefUIDs.h"
#include "AAFDefs/AAFPropertyIDs.h"
#include "AAFDefs/AAFFileKinds.h"
#include "AAFDefs/AAFTypeDefUIDs.h"

#include "../common/debug.h"






/**
 *	Loops through each aafPropertyIndexEntry_t of a "properties" node stream.
 *
 *	@param Header Pointer to the stream's aafPropertyIndexHeader_t struct.
 *	@param Entry  Pointer that will receive each aafPropertyIndexEntry_t struct.
 *	@param Value  Pointer to each property's data value, of aafPropertyIndexEntry_t._length
 *	              bytes length.
 *	@param i      uint32_t iterator.
 */

#define foreachPropertyEntry( Header, Entry, Value, i )                                              \
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

#define foreachStrongRefSetEntry( Header, Entry, i )                                                                     \
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

#define foreachStrongRefVectorEntry( Header, Entry, i )                                              \
	for( Entry = (aafStrongRefVectorEntry_t*)(((char*)Header) + sizeof(aafStrongRefVectorHeader_t)), \
	     i = 0;                                                                                      \
	     i < Header->_entryCount;                                                                    \
	     Entry++,                                                                                    \
	     i++ )








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

static aafObject * newObject( AAF_Data  *aafd,
                              cfbNode   *node,
                              aafClass  *Class,
                              aafObject *parent );


/**
 *	Allocates a new aafProperty structure.
 *
 *	@param  Def  Pointer to the corresponding property definition aafPropertyDef structure.
 *
 *	@return      A pointer to the newly created aafProperty.
 */

static aafProperty * newProperty( aafPropertyDef *Def );


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

static void retrieveObjectTree( AAF_Data *aafd );


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

static aafClass * retrieveMetaDictionaryClass( AAF_Data  *aafd,
                                               aafObject *TargetClassDef );


/**
 *	Sets the aafStrongRefSetHeader_t Obj->Header and aafStrongRefSetEntry_t Obj->Entry,
 *	when parsing an Object from a StrongReferenceSet. This function is called by the
 *	#retrieveStrongReferenceSet() function.
 *
 *	@param  Obj    Pointer to an aafObject structure.
 *	@param  Header Pointer to an aafStrongRefSetHeader_t structure.
 *	@param  Entry  Pointer to an aafStrongRefSetEntry_t structure.
 */

static void setObjectStrongRefSet( aafObject               *Obj,
                                   aafStrongRefSetHeader_t *Header,
                                   aafStrongRefSetEntry_t  *Entry );


/**
 *	Sets the aafStrongRefVectorHeader_t Obj->Header and aafStrongRefVectorEntry_t
 *	Obj->Entry, when parsing an Object from a StrongReferenceSet. This function is called
 *	by the retrieveStrongReferenceVector() function.
 *
 *	@param  Obj    Pointer to an aafObject structure.
 *	@param  Header Pointer to an aafStrongRefVectorHeader_t structure.
 *	@param  Entry  Pointer to an aafStrongRefVectorEntry_t structure.
 */

static void setObjectStrongRefVector( aafObject                  *Obj,
                                      aafStrongRefVectorHeader_t *Header,
                                      aafStrongRefVectorEntry_t  *Entry );


/**
 *	Retrieves and parses a single StrongReference Object. This function is called by
 *	retrieveProperty() when it encounters an SF_STRONG_OBJECT_REFERENCE property.
 *
 *	@param aafd   Pointer to the AAF_Data structure.
 *	@param Prop   Pointer to the property holding the SF_STRONG_OBJECT_REFERENCE.
 *	@param parent Pointer to the parent Object which holds the Prop property.
 */

static void retrieveStrongReference( AAF_Data    *aafd,
                                     aafProperty *Prop,
                                     aafObject   *parent );


/**
 *	Retrieves and parses StrongReferenceSet Objects. This function is called by
 *	retrieveProperty() when it encounters an SF_STRONG_OBJECT_REFERENCE_SET property.
 *
 *	@param aafd   Pointer to the AAF_Data structure.
 *	@param Prop   Pointer to the property holding the SF_STRONG_OBJECT_REFERENCE_SET.
 *	@param parent Pointer to the parent Object which holds the Prop property.
 */

static void retrieveStrongReferenceSet( AAF_Data    *aafd,
                                        aafProperty *Prop,
                                        aafObject   *parent );


/**
 *	Retrieve and parse StrongReferenceVector Objects. This function is called by
 *	retrieveProperty() when it encounters an SF_STRONG_OBJECT_REFERENCE_VECTOR property.
 *
 *	@param aafd   Pointer to the AAF_Data structure.
 *	@param Prop   Pointer to the property holding the SF_STRONG_OBJECT_REFERENCE_VECTOR.
 *	@param parent Pointer to the parent Object which holds the Prop property.
 */

static void retrieveStrongReferenceVector( AAF_Data    *aafd,
                                           aafProperty *Prop,
                                           aafObject   *parent );


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
 *	@param p    Pointer to the aafPropertyIndexEntry_t structure representing the property  *	           in the file.
 *	@param v    Pointer to a p->_length long byte array holding the actual property value.
 *	@param bo   uint8_t specifying the property's Byte Order. TO BE IMPLEMENTED
 *
 *	@TODO Take ByteOrder into account
 */

static void retrieveProperty( AAF_Data                *aafd,
                              aafObject               *Obj,
                              aafPropertyDef          *Def,
                              aafPropertyIndexEntry_t *p,
                              aafByte_t               *v,
                              uint8_t                  bo );


/**
 *	Retrieves the properties for a given aafObject.
 *
 *	@param aafd Pointer to the AAF_Data structure.
 *	@param Obj  Pointer to the aafObject holding the properties.
 */

static void retrieveObjectProperties( AAF_Data  *aafd,
                                      aafObject *Obj );


/**
 *	Retrieves a StrongRef Set/Vector Index Node in the Compound File Tree. This function
 *	is called by both retrieveStrongReferenceSet() and retrieveStrongReferenceVector().
 *
 *	@param  cfbd    Pointer to the CFB_Data structure.
 *	@param  parent  Pointer to the parent aafObject.
 *	@param  refName Pointer to a null terminated string holding the reference name.
 *
 *	@return         Pointer to the retrieved Node cfbNode structure.
 */

static cfbNode * getStrongRefIndexNode( CFB_Data   *cfbd,
                                        aafObject  *parent,
                                        const char *refName );


/**
 *	Retrieves a StrongRef Set or Vector Entry Node in the Compound File Tree. This
 *	function is called by both retrieveStrongReferenceSet() and
 *	retrieveStrongReferenceVector().
 *
 *	@param cfbd     Pointer to the CFB_Data structure.
 *	@param parent   Pointer to the parent Index Node.
 *	@param baseName Pointer to a null terminated string holding the reference base name.
 *	@param index    uint32_t number representing the index number of the reference.
 *
 *	@return         Pointer to the retrieved Node cfbNode structure.
 */

static cfbNode * getStrongRefEntryNode( CFB_Data   *cfbd,
                                        aafObject  *parent,
                                        const char *baseName,
                                        uint16_t    index );


/**
 *	Retrieves and returns a list of aafPropertyIndexHeader_t.
 *	For a given cfbNode, retrieves its /properties Stream Node and returns the stream as
 *	a pointer to an aafPropertyIndexHeader_t structure, wich the stream should begin with.
 *
 *	@param  cfbd Pointer to the CFB_Data structure.
 *	@param  node Pointer to a cfbNode structure.
 *
 *	@return      Pointer to an aafPropertyIndexHeader_t structure, followed by _entryCount
 *	             aafPropertyIndexEntry_t structures.
 */

static aafPropertyIndexHeader_t * getNodeProperties( CFB_Data *cfbd,
                                                     cfbNode  *node );


/**
 *	Retrieves and returns a list of StrongReferenceSet.
 *
 *	For a given Index cfbNode, retrieves its Stream and returns it as a pointer to an
 *	aafStrongRefSetHeader_t structure, wich the stream should begin with.
 *
 *	@param cfbd   Pointer to the CFB_Data structure.
 *	@param node   Pointer to an Index cfbNode structure.
 *	@param parent Pointer to the aafObject parent, only used on error printing.
 *
 *	@return       Pointer to an aafStrongRefSetHeader_t structure, followed by _entryCount
 *	              aafStrongRefSetEntry_t structures.
 */

static aafStrongRefSetHeader_t * getStrongRefSetList( CFB_Data  *cfbd,
                                                      cfbNode   *node,
                                                      aafObject *parent );


/**
 *	Retrieves and returns a list of StrongReferenceVectors.
 *
 *	For a given Index cfbNode, retrieves its Stream and returns it as a pointer to an
 *	aafStrongRefVectorHeader_t structure, wich the stream should begin with.
 *
 *	@param  cfbd   Pointer to the CFB_Data structure.
 *	@param  node   Pointer to an Index cfbNode structure.
 *	@param  parent Pointer to the aafObject parent, only used on error printing.
 *
 *	@return        Pointer to an aafStrongRefVectorHeader_t structure, followed by
 *	               _entryCount aafStrongRefVectorEntry_t structures.
 */

static aafStrongRefVectorHeader_t * getStrongRefVectorList( CFB_Data  *cfbd,
                                                            cfbNode   *node,
                                                            aafObject *parent );







AAF_Data *aaf_alloc()
{
	AAF_Data *aafd = calloc( sizeof(AAF_Data), sizeof(unsigned char) );

	if ( aafd == NULL )
		_fatal( "%s.\n", strerror( errno ) );

	return aafd;
}





int aaf_load_file( AAF_Data *aafd, const char *file )
{
	if ( file == NULL )
		return 1;

	aafd->Objects = NULL;
	aafd->Classes = NULL;

	aafd->cfbd = cfb_alloc();

	if ( cfb_load_file( aafd->cfbd, file ) < 0 )
		return 1;

	/*
	 *	NOTE at least Avid Media Composer doesn't respect
	 *	the standard clsid AAFFileKind_Aaf4KBinary identifier.
	 *	Therefore isValidAAF() is useless until futher findings..
	 */

	// if ( isValidAAF( aafd ) == 0 )
	// {
	// 	cfb_release( &aafd->cfbd );
	// 	return 1;
	// }

	setDefaultClasses( aafd );

	retrieveObjectTree( aafd );

	return 0;
}




void aaf_release( AAF_Data **aafd )
{
	if ( *aafd == NULL )
		return;


	cfb_release( &(*aafd)->cfbd );


	aafClass *Class    = NULL;
	aafClass *tmpClass = NULL;

	for ( Class = (*aafd)->Classes; Class != NULL; Class = tmpClass )
	{
		tmpClass = Class->next;

		aafPropertyDef *PDef    = NULL;
		aafPropertyDef *tmpPDef = NULL;

		if ( Class->name != NULL )
		{
			free( Class->name );
		}

		for ( PDef = Class->Properties; PDef != NULL; PDef = tmpPDef )
		{
			tmpPDef = PDef->next;

			if ( PDef->meta )
			{
				if ( PDef->name != NULL )
					free( PDef->name );
			}

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

	free( *aafd );

	*aafd = NULL;
}






char * aaf_get_ObjectPath( aafObject *Obj )
{
	// char *path = calloc( CFB_PATH_NAME_SZ, sizeof(char) );
    //
	// if ( path == NULL )
	// {
	// 	_error( "%s.\n", strerror( errno ) );
	// 	return NULL;
	// }

	static char path[CFB_PATH_NAME_SZ];

	path[0] = '\0';
	// memset( path, 0x00, CFB_PATH_NAME_SZ );


	uint32_t offset  = 0;
	uint32_t pathLen = 0;

	while ( Obj != NULL )
	{
		offset  = strlen( Obj->Name );
		pathLen = strlen(    path   );

		if ( offset + pathLen + 1 > CFB_PATH_NAME_SZ )
			_fatal( "Retrieved path length is bigger than CFB_PATH_NAME_SZ.\n" );

		// if ( pathLen > 0 )
		memcpy( path+(++offset), path, pathLen+1 ); // +1 for \0

		memcpy( path, Obj->Name, offset );

		if ( pathLen > 0 )
			path[offset-1] = '/';


		Obj = Obj->Parent;
	}

	path[pathLen+offset] = '\0';

	return path;
}



int aaf__foreach_ObjectInSet( aafObject **Obj, aafObject *head, const aafUID_t *filter )
{

	if ( *Obj == NULL )
		 *Obj = head;
	else
		 *Obj = (*Obj)->next;

	if ( filter != NULL )
		for (; *Obj != NULL; *Obj = (*Obj)->next )
			if ( auidCmp( (*Obj)->Class->ID, filter ) )
				break;

	return ( *Obj == NULL ) ? 0 : 1;
}




aafObject * aaf_get_ObjectByWeakRef( aafObject *list, aafWeakRef_t *ref )
{
//	aafObject *Obj = NULL;

//	printf( "ref->_referencedPropertyIndex : 0x%x\n", ref->_referencedPropertyIndex );
//	printf( "ref->_identificationSize      : %u\n", ref->_identificationSize );

//	printf( "\n\n\nL o o k i n g    f o r  : \n" );
//	CFB_printStream( ref->_identification, ref->_identificationSize );

//	printf( "\n\n\nT R Y I N G  :\n" );

/* TODO check _identificationSize mismatch */

	if ( list == NULL || list->Entry == NULL )
		return NULL;

	// Target is a Reference Vector
	if ( list->Header->_identificationSize == 0 )
	{
//		printf( "Has local key\n" );
		for (; list != NULL; list = list->next )
			if ( list->Entry->_localKey == ref->_referencedPropertyIndex )
			{
//				printf( "Target->Entry->_localKey            : 0x%x\n", Target->Entry->_localKey );
//				printf( "Target->Header->_identificationSize : %u\n", Target->Header->_identificationSize );
//				printf( "FOUND : 0x%x\n", Target->Entry->_localKey );
				return list;
			}

	}
	else // Target is a Reference Set
	{
//		printf( "Has identif\n" );
		for (; list != NULL; list = list->next ) {
			if ( memcmp( list->Entry->_identification, ref->_identification, ref->_identificationSize ) == 0 )
				return list;
		}
	}

//		printf( "localKey : 0x%x\n", Target->Entry->_localKey );
	return NULL;
}





aafObject * aaf_get_MobByID( aafObject *Mobs, aafMobID_t *MobID )
{
	aafObject *Mob = NULL;

	if ( MobID == NULL )
		return NULL;

	aaf_foreach_ObjectInSet( &Mob, Mobs, NULL )
	{
		aafMobID_t *Current = aaf_get_propertyValue( Mob, PID_Mob_MobID );

		if ( Current == NULL || mobIDCmp( Current, MobID ) )
			break;
	}

	return Mob;
}




aafObject * aaf_get_MobSlotBySlotID( aafObject *MobSlots, aafSlotID_t SlotID )
{
	aafObject *MobSlot = NULL;

	aaf_foreach_ObjectInSet( &MobSlot, MobSlots, NULL )
	{
		aafSlotID_t *CurrentSlotID = aaf_get_propertyValue( MobSlot, PID_MobSlot_SlotID );

		if ( CurrentSlotID == NULL || *CurrentSlotID == SlotID )
			break;
	}

	return MobSlot;
}




/*
 *	TODO Works when the property was retrieved from MetaDictionary. What if the property is standard ?
 */

aafPID_t aaf_get_PropertyIDByName( AAF_Data *aafd, const char *name )
{
	aafClass *Class = NULL;

	foreachClass( Class, aafd->Classes )
	{
		aafPropertyDef *PDef = NULL;

		foreachPropertyDefinition( PDef, Class->Properties )
		{
			if ( PDef->name != NULL && strcmp( PDef->name, name ) == 0 )
			{
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


	aafProperty *Prop = NULL;

	for ( Prop = Obj->Properties; Prop != NULL; Prop = Prop->next )
		if ( Prop->pid == pid )
			break;


	if ( Prop == NULL )
	{
		aafPropertyDef *PDef = NULL;

		foreachPropertyDefinition( PDef, Obj->Class->Properties )
		{
			/* TODO pass aafd to PIDToText() */
			if ( PDef->pid == pid && PDef->isReq == 1 )
				_warning( "Could not find the required property %s (%u)\n", PIDToText( NULL, pid ), pid );
		}
	}

	return Prop;
}



void * aaf_get_propertyValue( aafObject *Obj, aafPID_t pid )
{
	aafProperty *Prop = aaf_get_property( Obj, pid );

	if ( Prop == NULL )
	{
		return NULL;
	}

	return Prop->val;
}



char * aaf_get_propertyValueText( aafObject *Obj, aafPID_t pid )
{
	aafProperty *Prop = aaf_get_property( Obj, pid );

	if ( Prop == NULL )
	{
		return NULL;
	}

	char *string  = malloc( ( Prop->len >> 1 ) + 1 );

	utf16toa( string, (Prop->len >> 1) + 1, Prop->val, Prop->len );


	return string;
}



void * aaf_get_propertyIndirectValue( aafObject *Obj, aafPID_t pid )
{
	aafIndirect_t *Indirect = aaf_get_propertyValue( Obj, pid );

	if ( Indirect == NULL )
	{
		return NULL;
	}

	// TODO ? ensures the Indirect->Value is what it pretend to be by size check.

	return Indirect->Value;
}



char * aaf_get_propertyIndirectValueText( aafObject *Obj, aafPID_t pid )
{

	aafProperty *Prop = aaf_get_property( Obj, pid );

	if ( Prop == NULL )
	{
		return NULL;
	}

	aafIndirect_t *Indirect = Prop->val;

	if ( auidCmp( &Indirect->TypeDef, &AAFTypeID_String ) == 0 )
	{
		_warning( "Indirect value is not of type String.\n" );
		return NULL;
	}

	char *string  = malloc( (( Prop->len - sizeof(aafIndirect_t)) >> 1 ) + 1 );

	utf16toa( string, ((Prop->len - sizeof(aafIndirect_t)) >> 1) + 1, (uint16_t*)Indirect->Value, (Prop->len - sizeof(aafIndirect_t)) );
	// utf16toa(char *astr, uint16_t alen, uint16_t *wstr, uint16_t wlen)

	return string;
}
























// static int isValidAAF( AAF_Data *aafd )
// {
// 	aafUID_t *hdrClsID = (aafUID_t*)&aafd->cfbd->hdr->_clsid;
//
// 	if ( auidCmp( hdrClsID, &AAFFileKind_Aaf512Binary ) ||
// 		 auidCmp( hdrClsID, &AAFFileKind_Aaf4KBinary  ) )
// 			return 1;
//
// //  _warning( "Unsuported AAF encoding (%s).\n", FileKindToText( hdrClsID ) );
//
// 	return 0;
// }








static aafObject * newObject( AAF_Data *aafd, cfbNode *Node, aafClass *Class, aafObject *Parent )
{
	// aafObject *Obj = malloc( sizeof(aafObject) ); // NOTE valgrind errors on AAFInfo --aaf-properties
	aafObject *Obj = calloc( sizeof(aafObject), sizeof(unsigned char) );

	if ( Obj == NULL )
		_fatal( "%s.\n", strerror( errno ) );


	utf16toa( Obj->Name, CFB_NODE_NAME_SZ, Node->_ab, Node->_cb );

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




static aafProperty * newProperty( aafPropertyDef *Def )
{
	aafProperty *Prop = calloc( sizeof(aafProperty), sizeof(unsigned char) );

	if ( Prop == NULL )
		_fatal( "%s.\n", strerror( errno ) );


	Prop->pid = Def->pid;
	Prop->def = Def;

	return Prop;
}





static void setObjectShortcuts( AAF_Data *aafd )
{
//  aafd->Root = aafd->Root;

	aafd->Header                  = aaf_get_propertyValue( aafd->Root,       PID_Root_Header                         );
//  aafd->MetaDictionary          = aaf_get_propertyValue( aafd->Root,       PID_Root_MetaDictionary                 );

	aafd->ClassDefinition         = aaf_get_propertyValue( aafd->MetaDictionary, PID_MetaDictionary_ClassDefinitions );
	aafd->TypeDefinition          = aaf_get_propertyValue( aafd->MetaDictionary, PID_MetaDictionary_TypeDefinitions  );

	aafd->Identification          = aaf_get_propertyValue( aafd->Header,     PID_Header_IdentificationList           );
	aafd->Content                 = aaf_get_propertyValue( aafd->Header,     PID_Header_Content                      );
	aafd->Dictionary              = aaf_get_propertyValue( aafd->Header,     PID_Header_Dictionary                   );

	aafd->Mobs                    = aaf_get_propertyValue( aafd->Content,    PID_ContentStorage_Mobs                 );
	aafd->EssenceData             = aaf_get_propertyValue( aafd->Content,    PID_ContentStorage_EssenceData          );

	aafd->OperationDefinition     = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_OperationDefinitions     );
	aafd->ParameterDefinition     = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_ParameterDefinitions     );
	aafd->DataDefinition          = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_DataDefinitions          );
	aafd->PluginDefinition        = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_PluginDefinitions        );
	aafd->CodecDefinition         = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_CodecDefinitions         );
	aafd->ContainerDefinition     = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_ContainerDefinitions     );
	aafd->InterpolationDefinition = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_InterpolationDefinitions );
	aafd->KLVDataDefinition       = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_KLVDataDefinitions       );
	aafd->TaggedValueDefinition   = aaf_get_propertyValue( aafd->Dictionary, PID_Dictionary_TaggedValueDefinitions   );

}








static void retrieveObjectTree( AAF_Data *aafd )
{
	cfbNode *Node  = aafd->cfbd->nodes[0];

	aafClass *Class = getClassByID( aafd, (aafUID_t*)&Node->_clsId );


	if ( Class == NULL && auidCmp( Class->ID, (aafUID_t*)&Node->_clsId ) != 0 )
		_fatal( "Looks like the fist Object is not the Root Class : %s\n",
				ClassIDToText( aafd, Class->ID ) );


	aafd->Root = newObject( aafd, Node, Class, NULL );





	/******************* retrieveObjectProperties() *********************/

	aafPropertyIndexHeader_t *Header = getNodeProperties( aafd->cfbd, aafd->Root->Node );
	aafPropertyIndexEntry_t  *Prop   = NULL;

	aafPropertyIndexEntry_t  *AAFHeaderProp = NULL;
	aafPropertyIndexEntry_t  *AAFMetaDcProp = NULL;

	aafByte_t      *AAFHeaderVal  = NULL;
	aafByte_t      *AAFMetaDcVal  = NULL;

	aafByte_t      *value         = NULL;

	aafPropertyDef *PDef   = NULL;

	if ( Header == NULL )
		_fatal( "Could not retrieve properties : %s\n.", aaf_get_ObjectPath( aafd->Root ) );


	uint32_t i = 0;


	foreachPropertyEntry( Header, Prop, value, i )
	{

		if ( Prop->_pid == PID_Root_Header )
		{
			AAFHeaderProp = Prop;
			AAFHeaderVal  = value;
		}

		if ( Prop->_pid == PID_Root_MetaDictionary )
		{
			AAFMetaDcProp = Prop;
			AAFMetaDcVal  = value;
		}

	}

	PDef = getPropertyDefinitionByID( aafd->Root->Class, PID_Root_MetaDictionary );

	retrieveProperty( aafd, aafd->Root, PDef, AAFMetaDcProp, AAFMetaDcVal, Header->_byteOrder );


	/*
	 *	Retrieve MetaDictionary.
	 */

	aafObject *MetaDic   = aaf_get_propertyValue( aafd->Root, PID_Root_MetaDictionary );
	aafObject *ClassDefs = aaf_get_propertyValue( MetaDic, PID_MetaDictionary_ClassDefinitions );
	aafObject *ClassDef  = NULL;

	aaf_foreach_ObjectInSet( &ClassDef, ClassDefs, NULL )
		retrieveMetaDictionaryClass( aafd, ClassDef );



	PDef = getPropertyDefinitionByID( aafd->Root->Class, PID_Root_Header );

	retrieveProperty( aafd, aafd->Root, PDef, AAFHeaderProp, AAFHeaderVal, Header->_byteOrder );

	free( Header );



	setObjectShortcuts( aafd );

}




static int propertyIdExistsInClass( aafClass *Class, aafPID_t Pid )
{
	aafPropertyDef *PDef = NULL;

	foreachPropertyDefinition( PDef, Class->Properties )
		if ( PDef->pid == Pid )
			return 1;

	return 0;
}




static aafClass * retrieveMetaDictionaryClass( AAF_Data *aafd, aafObject *TargetClassDef )
{
	aafObject *MetaDic   = aaf_get_propertyValue( aafd->Root, PID_Root_MetaDictionary );



	aafObject *ClassDefs = aaf_get_propertyValue( MetaDic, PID_MetaDictionary_ClassDefinitions );
	aafObject *ClassDef  = NULL;

	aaf_foreach_ObjectInSet( &ClassDef, ClassDefs, NULL )
		if ( ClassDef == TargetClassDef )
			break;

	if ( ClassDef == NULL )
		_fatal( "Could not retrieve ClassDefinition.\n" );


	aafUID_t    *ClassID = aaf_get_propertyValue( ClassDef, PID_MetaDefinition_Identification );

	aafWeakRef_t *parent = aaf_get_propertyValue( ClassDef, PID_ClassDefinition_ParentClass   );
	aafObject    *Parent = aaf_get_ObjectByWeakRef( ClassDefs, parent );




	aafClass *ParentClass = NULL;

	if ( Parent != ClassDef )
	{
		ParentClass = retrieveMetaDictionaryClass( aafd, Parent );
	}
	else if ( auidCmp( ClassID, &AAFClassID_InterchangeObject ) == 0 &&
			  auidCmp( ClassID, &AAFClassID_MetaDefinition    ) == 0 &&
			  auidCmp( ClassID, &AAFClassID_MetaDictionary    ) == 0 )
	{
		_fatal( "Parent's Class equals Child's : %s.\n", ClassIDToText( aafd, ClassID ) );
	}



	aafClass *Class = getClassByID( aafd, ClassID );

	if ( Class == NULL )
	{

		aafBoolean_t *isCon = aaf_get_propertyValue( ClassDef, PID_ClassDefinition_IsConcrete );

		if ( isCon == NULL )
			_fatal( "Missing ClassDefinition::IsConcrete\n" );

		Class = defineNewClass( aafd, ClassID, *isCon, ParentClass );

		Class->name = aaf_get_propertyValueText( ClassDef, PID_MetaDefinition_Name );
		Class->meta = 1;
	}
	else	// if class is standard, we only set its name
	{
		if ( Class->name == NULL )
		     Class->name = aaf_get_propertyValueText( ClassDef, PID_MetaDefinition_Name );
	}


	aafObject *Props = aaf_get_propertyValue( ClassDef, PID_ClassDefinition_Properties );
	aafObject *Prop  = NULL;

	aaf_foreach_ObjectInSet( &Prop, Props, NULL )
	{

		aafPID_t *Pid = aaf_get_propertyValue( Prop, PID_PropertyDefinition_LocalIdentification );

		if ( Pid == NULL )
			_fatal( "Missing PropertyDefinition::LocalIdentification.\n" );

		/*
		 *	We skip all the properties that were already defined in setDefaultClasses().
		 *	TODO propertyIdExists()
		 */

		if ( propertyIdExistsInClass( Class, *Pid ) )
		{
			// printf("Property %d exists.\n", *Pid );
			continue;
		}


		aafBoolean_t *isOpt = aaf_get_propertyValue( Prop, PID_PropertyDefinition_IsOptional );

		if ( isOpt == NULL )
			_fatal( "Missing PropertyDefinition::IsOptional.\n" );


		aafPropertyDef *PDef = NULL;

		attachNewProperty( Class, PDef, *Pid, ( *isOpt ) ? 0 : 1 );

		PDef->meta = 1;
		PDef->name = aaf_get_propertyValueText( Prop, PID_MetaDefinition_Name );


/*

		aafObject *TypeDefs  = aaf_get_propertyValue( MetaDic, PID_MetaDictionary_TypeDefinitions  );
		aafWeakRef_t *WeakRefToType = aaf_get_propertyValue( Property, PID_PropertyDefinition_Type );

		aafObject *TypeDef = aaf_get_ObjectByWeakRef( TypeDefs, WeakRefToType );

		char     *typeName = NULL;
		aafUID_t *typeUID  = NULL;

		if ( TypeDef != NULL )
		{
			typeUID  = aaf_get_propertyValue( TypeDef, PID_MetaDefinition_Identification );
			typeName = aaf_get_propertyValueText( TypeDef, PID_MetaDefinition_Name );
			printf( "TypeName : %s (%s) |  name : %s\n",
				typeName,
				TypeIDToText( typeUID ),    // shows unknown value
				name );
		}
*/

	}

	return Class;
}












static void setObjectStrongRefSet( aafObject *Obj, aafStrongRefSetHeader_t *Header, aafStrongRefSetEntry_t *Entry )
{
	Obj->Header = malloc( sizeof(aafStrongRefSetHeader_t) );

	if ( Obj->Header == NULL )
		_fatal( "%s.\n", strerror( errno ) );

	memcpy( Obj->Header, Header, sizeof(aafStrongRefSetHeader_t)  );


	/* True entrySize, taking _identification into account. */
	uint32_t entrySize = sizeof(aafStrongRefSetEntry_t) + Header->_identificationSize;

	Obj->Entry = malloc( entrySize );

	if ( Obj->Entry == NULL )
		_fatal( "%s.\n", strerror( errno ) );

	memcpy( Obj->Entry,  Entry,  entrySize   );
}




static void setObjectStrongRefVector( aafObject *Obj, aafStrongRefVectorHeader_t *Header, aafStrongRefVectorEntry_t *Entry )
{
	/*
	 * aafStrongRefVectorHeader_t  and  aafStrongRefSetHeader_t begins with the same
	 * data bytes,  so  we  can  safely  memcpy to the first one from the second one,
	 * the remaining bytes simply remaining null.
	 * The same applies  to aafStrongRefVectorEntry_t and aafStrongRefVectorHeader_t.
	 */

	Obj->Header = calloc( sizeof(aafStrongRefSetHeader_t), sizeof(unsigned char) );

	if ( Obj->Header == NULL )
		_fatal( "%s.\n", strerror( errno ) );

	memcpy( Obj->Header, Header, sizeof(aafStrongRefVectorHeader_t)  );



	Obj->Entry = calloc( sizeof(aafStrongRefSetEntry_t), sizeof(unsigned char) );

	if ( Obj->Entry == NULL )
		_fatal( "%s.\n", strerror( errno ) );

	memcpy( Obj->Entry, Entry,  sizeof(aafStrongRefVectorEntry_t)   );
}




static void retrieveStrongReference( AAF_Data *aafd, aafProperty *Prop, aafObject *Parent )
{
	char name[CFB_NODE_NAME_SZ];

	utf16toa( name, CFB_NODE_NAME_SZ, Prop->val, Prop->len );

	free( Prop->val );


	cfbNode *Node  = cfb_getChildNode( aafd->cfbd, name, Parent->Node );
	aafClass *Class = getClassByID( aafd, (aafUID_t*)&Node->_clsId );

	if ( Class == NULL )
		_fatal( "Could not retrieve Class %s @ \"%s\"\n",
				ClassIDToText( aafd, (aafUID_t*)&Node->_clsId ),
				aaf_get_ObjectPath( Parent ) );


	Prop->val = newObject( aafd, Node, Class, Parent );

	retrieveObjectProperties( aafd, Prop->val );
}




static void retrieveStrongReferenceSet( AAF_Data *aafd, aafProperty *Prop, aafObject *Parent )
{
	char refName[CFB_NODE_NAME_SZ];

	utf16toa( refName, CFB_NODE_NAME_SZ, Prop->val, Prop->len );

	free( Prop->val );
	Prop->val = NULL;


	cfbNode * Node = getStrongRefIndexNode( aafd->cfbd, Parent, refName );

	aafStrongRefSetHeader_t *Header = getStrongRefSetList( aafd->cfbd, Node, Parent );
	aafStrongRefSetEntry_t  *Entry  = NULL;


	uint32_t i = 0;

	foreachStrongRefSetEntry( Header, Entry, i )
	{

		Node = getStrongRefEntryNode( aafd->cfbd, Parent, refName, Entry->_localKey );

		aafClass *Class = getClassByID( aafd, (aafUID_t*)&Node->_clsId );

		if ( Class == NULL )
		{
			_error( "Could not retrieve Class %s\n",
					ClassIDToText( aafd, (aafUID_t*)&Node->_clsId ) );
			continue;
		}

		aafObject * Obj = newObject( aafd, Node, Class, Parent );


		setObjectStrongRefSet( Obj, Header, Entry );

		retrieveObjectProperties( aafd, Obj );

		Obj->next = Prop->val;
		Prop->val = Obj;
	}

	if ( Header != NULL )
		free( Header );
}




static void retrieveStrongReferenceVector( AAF_Data *aafd, aafProperty *Prop, aafObject *Parent )
{
	char refName[CFB_NODE_NAME_SZ];

	utf16toa( refName, CFB_NODE_NAME_SZ, Prop->val, Prop->len );

	free( Prop->val );
	Prop->val = NULL;


	cfbNode * Node = getStrongRefIndexNode( aafd->cfbd, Parent, refName );

	aafStrongRefVectorHeader_t *Header = getStrongRefVectorList( aafd->cfbd, Node, Parent );
	aafStrongRefVectorEntry_t  *Entry  = NULL;


	uint32_t i = 0;

	foreachStrongRefVectorEntry( Header, Entry, i )
	{

		Node = getStrongRefEntryNode( aafd->cfbd, Parent, refName, Entry->_localKey );

		aafClass * Class = getClassByID( aafd, (aafUID_t*)&Node->_clsId );

		if ( Class == NULL )
		{
			_warning( "Could not retrieve Class ID %s\n",
					ClassIDToText( aafd, (aafUID_t*)&Node->_clsId ) );
			continue;
		}

		aafObject * Obj = newObject( aafd, Node, Class, Parent );


		setObjectStrongRefVector( Obj, Header, Entry );

		retrieveObjectProperties( aafd, Obj );


		/*
		 *	Vector are ordered.
		 */

		if ( Prop->val != NULL )
		{
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
}




static void retrieveProperty( AAF_Data *aafd, aafObject *Obj, aafPropertyDef *Def, aafPropertyIndexEntry_t *p, aafByte_t *v, uint8_t bo )
{
	bo++;

	aafProperty *Prop = newProperty( Def );

	Prop->sf  = p->_storedForm;

	Prop->len = p->_length;

	Prop->val = malloc( p->_length );

	if ( Prop->val == NULL )
		_fatal( "%s.\n", strerror( errno ) );

	memcpy( Prop->val, v, p->_length );


	Prop->next = Obj->Properties;
	Obj->Properties = Prop;


	switch ( p->_storedForm )
	{
		case SF_STRONG_OBJECT_REFERENCE:
			retrieveStrongReference( aafd, Prop, Obj );        break;

		case SF_STRONG_OBJECT_REFERENCE_SET:
			retrieveStrongReferenceSet( aafd, Prop, Obj );     break;

		case SF_STRONG_OBJECT_REFERENCE_VECTOR:
			retrieveStrongReferenceVector( aafd, Prop, Obj );  break;

		default: break;
	}

}




static void retrieveObjectProperties( AAF_Data *aafd, aafObject *Obj )
{

	aafPropertyIndexHeader_t *Header = getNodeProperties( aafd->cfbd, Obj->Node );
	aafPropertyIndexEntry_t  *Prop   = NULL;

	aafByte_t      *value  = NULL;
	aafPropertyDef *PDef   = NULL;

	if ( Header == NULL )
		_fatal( "Could not retrieve properties : %s\n.", aaf_get_ObjectPath( Obj ) );


	uint32_t i = 0;


	foreachPropertyEntry( Header, Prop, value, i )
	{

		PDef = getPropertyDefinitionByID( Obj->Class, Prop->_pid );

		if ( PDef != NULL )
			retrieveProperty( aafd, Obj, PDef, Prop, value, Header->_byteOrder );
		else
			_warning( "Unknown property 0x%04x (%s)\n", Prop->_pid, PIDToText( aafd, Prop->_pid ) );

	}

	free( Header );
}




static cfbNode * getStrongRefIndexNode( CFB_Data *cfbd, aafObject *Parent, const char *refName )
{
	char name[CFB_NODE_NAME_SZ];

	snprintf( name, CFB_NODE_NAME_SZ, "%s index", refName );

	cfbNode * Node = cfb_getChildNode( cfbd, name, Parent->Node );

	if ( Node == NULL )
		_fatal( "Could not retrieve Reference Set/Vector Index Node @ \"%s/%s index\"\n",
				aaf_get_ObjectPath( Parent ),
				refName );


	return Node;
}



static cfbNode * getStrongRefEntryNode( CFB_Data *cfbd, aafObject *Parent, const char *refName, uint16_t index )
{
	char name[CFB_NODE_NAME_SZ];

	snprintf( name, CFB_NODE_NAME_SZ, "%s{%x}", refName, index );

	cfbNode *Node = cfb_getChildNode( cfbd, name, Parent->Node );

	if ( Node == NULL )
		_fatal( "Could not retrieve Reference Set/vector Entry Node @ \"%s/%s index\"\n",
				aaf_get_ObjectPath( Parent ),
				refName );


	return Node;
}



static aafPropertyIndexHeader_t * getNodeProperties( CFB_Data *cfbd, cfbNode *Node )
{
	if ( Node == NULL )
		return NULL;

	uint64_t   stream_sz = 0;
	aafByte_t *stream    = NULL;


	cfbNode  *propNode  = cfb_getChildNode( cfbd, "properties", Node );

	if ( propNode == NULL )
		_fatal( "Could not retrieve Property Node.\n" );


	cfb_getStream( cfbd, propNode, &stream, &stream_sz );

	if ( stream == NULL )
		_fatal( "Could not retrieve Property Stream.\n" );


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
		_warning( "Stream length (%lu Bytes) does not match property length (%u Bytes)\n",
			stream_sz,
			prop_sz );
*/

	return (aafPropertyIndexHeader_t*)stream;
}




static aafStrongRefSetHeader_t * getStrongRefSetList( CFB_Data *cfbd, cfbNode *Node, aafObject *Parent )
{
	if ( Node == NULL )
		return NULL;

	aafByte_t *stream = NULL;
	size_t  stream_sz = 0;

	cfb_getStream( cfbd, Node, &stream, &stream_sz );

	if ( stream == NULL )
	{
		char refName[CFB_NODE_NAME_SZ];

		utf16toa( refName, CFB_NODE_NAME_SZ, Node->_ab, Node->_cb );

		_fatal( "Could not retrieve StrongReferenceSet Index Stream @ \"%s/%s index\"\n",
				aaf_get_ObjectPath( Parent ),
				refName );
	}

	return (aafStrongRefSetHeader_t*)stream;
}




static aafStrongRefVectorHeader_t * getStrongRefVectorList( CFB_Data *cfbd, cfbNode *Node, aafObject *Parent )
{
	if ( Node == NULL )
		return NULL;

	aafByte_t *stream = NULL;
	size_t  stream_sz = 0;

	cfb_getStream( cfbd, Node, &stream, &stream_sz );

	if ( stream == NULL )
	{
		char refName[CFB_NODE_NAME_SZ];

		utf16toa( refName, CFB_NODE_NAME_SZ, Node->_ab, Node->_cb );

		_fatal( "Could not retrieve StrongReferenceVector Index Stream \"%s/%s index\"\n",
				aaf_get_ObjectPath( Parent ),
				refName );
	}

	return (aafStrongRefVectorHeader_t*)stream;
}

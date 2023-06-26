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
#include <string.h>

#include <libaaf/AAFTypes.h>
#include <libaaf/AAFToText.h>

#include "../common/utils.h"

#include "AAFClass.h"





void aaf_dump_Header( AAF_Data *aafd )
{

    printf( " ByteOrder            : %ls (0x%04x)\n", ByteOrderToText( aafd->Header.ByteOrder ), aafd->Header.ByteOrder );
    printf( " LastModified         : %ls\n", TimestampToText( aafd->Header.LastModified ) );
    printf( " AAF ObjSpec Version  : %ls\n", VersionToText( aafd->Header.Version ) );
    printf( " ObjectModel Version  : %u\n", aafd->Header.ObjectModelVersion );
    printf( " Operational Pattern  : %ls\n", OPDefToText( aafd->Header.OperationalPattern ) );

    printf( "\n\n" );
}




void aaf_dump_Identification( AAF_Data *aafd )
{

    printf( " CompanyName          : %ls\n", ( aafd->Identification.CompanyName ) ? aafd->Identification.CompanyName : L"n/a" );
    printf( " ProductName          : %ls\n", ( aafd->Identification.ProductName ) ? aafd->Identification.ProductName : L"n/a" );
    printf( " ProductVersion       : %ls\n", ProductVersionToText( aafd->Identification.ProductVersion ) );
    printf( " ProductVersionString : %ls\n", ( aafd->Identification.ProductVersionString ) ? aafd->Identification.ProductVersionString : L"n/a" );
    printf( " ProductID            : %ls\n", AUIDToText( aafd->Identification.ProductID ) );
    printf( " Date                 : %ls\n", TimestampToText( aafd->Identification.Date ) );
    printf( " ToolkitVersion       : %ls\n", ProductVersionToText( aafd->Identification.ToolkitVersion ) );
    printf( " Platform             : %ls\n", ( aafd->Identification.Platform ) ? aafd->Identification.Platform : L"n/a" );
    printf( " GenerationAUID       : %ls\n", AUIDToText( aafd->Identification.GenerationAUID ) );

    printf( "\n\n" );
}



void aaf_dump_ObjectProperty( AAF_Data *aafd, aafProperty *Prop )
{
	printf( " :.: (0x%04x) %ls (%ls)\n", Prop->pid, PIDToText( aafd, Prop->pid ), StoredFormToText( Prop->sf ) /*AUIDToText( &Prop->def->type ),*/ /*TypeIDToText( &(Prop->def->type) )*/ );
	// WARNING : Wont print strong references (set/vector) corectly.
	dump_hex( Prop->val, Prop->len );
}



void aaf_dump_ObjectProperties( AAF_Data *aafd, aafObject *Obj )
{
  /*
   *  List the properties once they have been parsed and interpreted by AAFCore.
   */

	aafProperty * Prop = NULL;

	for ( Prop = Obj->Properties;  Prop != NULL; Prop = Prop->next )
	{
    aaf_dump_ObjectProperty( aafd, Prop );
		// printf( " :.: (0x%04x) %ls (%ls)\n", Prop->pid, PIDToText( aafd, Prop->pid ), StoredFormToText( Prop->sf ) /*AUIDToText( &Prop->def->type ),*/ /*TypeIDToText( &(Prop->def->type) )*/ );
    //
		// // WARNING : Wont print strong references (set/vector) corectly.
		// dump_hex( Prop->val, Prop->len );
	}
}



void aaf_dump_rawProperties( AAF_Data *aafd, aafPropertyIndexHeader_t *PropHeader )
{
  aafPropertyIndexHeader_t *Header = PropHeader;
  aafPropertyIndexEntry_t  *Prop   = NULL;
  aafByte_t                *value  = NULL;

  uint32_t i = 0;

  if ( Header == NULL ) {
    printf(
      " ## Property_Header____________________________________________________\n\n"
      " aafPropertyIndexHeader_t is NULL\n"
      " ======================================================================\n\n"
    );
    return;
  }


	printf(
		" ## Property_Header____________________________________________________\n\n"
		" _byteOrder     : 0x%02x\n"
		" _formatVersion : 0x%02x\n"
		" _entryCount    : %u\n\n"
        " ======================================================================\n\n",
		Header->_byteOrder,
		Header->_formatVersion,
		Header->_entryCount
	);

	printf( "\n\n" );

	/*
	 *	Since the following for-loop macro is not intended to be user
     *  accessible, it has been defined as a local macro in AAFCore.c.
	 */

	// foreachPropertyEntry( Header, Prop, value, i )
	for ( Prop = (aafPropertyIndexEntry_t*)(((char*)Header) + sizeof(aafPropertyIndexHeader_t)),    \
	      value = ((unsigned char*)Prop) + (Header->_entryCount * sizeof(aafPropertyIndexEntry_t)), \
	      i = 0;                                                                                    \
	      i < Header->_entryCount;                                                                  \
	      value += Prop->_length,                                                                   \
	      Prop++,                                                                                   \
	      i++ )
	{

		printf(
			" #%u Property_Entry_____________________________________________________\n"
			" _pid        : 0x%04x (%ls)\n"
			" _storedForm : %ls\n"
			" _length     : %u bytes\n",
			i,
			Prop->_pid, PIDToText( aafd, Prop->_pid ),
			StoredFormToText( Prop->_storedForm ),
			Prop->_length
		);

		dump_hex( value, Prop->_length );

		printf( "\n\n" );
	}
}




void aaf_dump_nodeStreamProperties( AAF_Data *aafd, cfbNode *node )
{
    /*
     *  List the raw properties directly from a CFB Node's stream.
     */

	aafPropertyIndexHeader_t *Header = NULL;
	// aafPropertyIndexEntry_t  *Prop   = NULL;
	// aafByte_t                *value  = NULL;

	cfb_getStream( aafd->cfbd, node, (unsigned char**)&Header, NULL );

    aaf_dump_rawProperties( aafd, Header );

/*
	uint32_t i = 0;


	printf(
		" ## Property_Header____________________________________________________\n\n"
		" _byteOrder     : 0x%02x\n"
		" _formatVersion : 0x%02x\n"
		" _entryCount    : %u\n\n"
        " ======================================================================\n\n",
		Header->_byteOrder,
		Header->_formatVersion,
		Header->_entryCount
	);

	printf( "\n\n" );


	// foreachPropertyEntry( Header, Prop, value, i )
	for ( Prop = (aafPropertyIndexEntry_t*)(((char*)Header) + sizeof(aafPropertyIndexHeader_t)),    \
	      value = ((unsigned char*)Prop) + (Header->_entryCount * sizeof(aafPropertyIndexEntry_t)), \
	      i = 0;                                                                                    \
	      i < Header->_entryCount;                                                                  \
	      value += Prop->_length,                                                                   \
	      Prop++,                                                                                   \
	      i++ )
	{

		printf(
			" #%u Property_Entry_____________________________________________________\n"
			" _pid        : 0x%04x (%ls)\n"
			" _storedForm : %ls\n"
			" _length     : %u bytes\n",
			i,
			Prop->_pid, PIDToText( aafd, Prop->_pid ),
			StoredFormToText( Prop->_storedForm ),
			Prop->_length
		);

		dump_hex( value, Prop->_length );

		printf( "\n\n" );
	}
*/

	free( Header );
}




void aaf_dump_MetaDictionary( AAF_Data *aafd )
{
    /*
     *  NOTE Only dumps the "custom" classes/properties, since those are the only
     *  ones we register when parsing. That is, all standard classes/properties
     *  wont be printed out.
     */

    aafClass *Class = NULL;


    foreachClass( Class, aafd->Classes )
    {

        int print = 0;

        aafPropertyDef *PDef = NULL;

        foreachPropertyDefinition( PDef, Class->Properties )
        {
            if ( Class->meta )
            {
                printf( ANSI_COLOR_YELLOW "%ls::%ls (0x%04x)\n" ANSI_COLOR_RESET,
                    Class->name,
                    PDef->name,
                    PDef->pid );

                print++;
            }
            else if ( PDef->meta )
            {
                printf( "%ls::" ANSI_COLOR_YELLOW "%ls (0x%04x)\n" ANSI_COLOR_RESET,
                    ClassIDToText( aafd, Class->ID ),
                    PDef->name,
                    PDef->pid );

                print++;
            }
        }

        if ( print )
            printf( "\n" );

        print = 1;

    }

    printf( "\n\n" );
}




void aaf_dump_Classes( AAF_Data *aafd )
{
    aafClass *ConcreteClass = NULL;
    aafClass *Class         = NULL;

    foreachClass( ConcreteClass, aafd->Classes )
    {

        foreachClassInheritance( Class, ConcreteClass )
        {

            printf( "%s%ls%s",
                (Class->meta) ? ANSI_COLOR_YELLOW : "",
                ClassIDToText( aafd, Class->ID ),
                (Class->meta) ? ANSI_COLOR_RESET  : "" );


            if ( Class->Parent != NULL )
                printf( " > " );

        }

        printf( "\n" );
    }

    printf( "\n\n" );
}

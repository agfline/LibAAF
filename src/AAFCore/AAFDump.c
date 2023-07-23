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
#include <libaaf/AAFDump.h>

#include "AAFClass.h"
#include "../common/utils.h"



void aaf_dump_Header( AAF_Data *aafd )
{
  int offset = 0;
  struct dbg *dbg = aafd->dbg;

  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " ByteOrder            : %ls (0x%04x)\n", ByteOrderToText( aafd->Header.ByteOrder ), aafd->Header.ByteOrder );
  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " LastModified         : %ls\n", TimestampToText( aafd->Header.LastModified ) );
  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " AAF ObjSpec Version  : %ls\n", VersionToText( aafd->Header.Version ) );
  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " ObjectModel Version  : %u\n", aafd->Header.ObjectModelVersion );
  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " Operational Pattern  : %ls\n", OPDefToText( aafd->Header.OperationalPattern ) );

  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, "\n\n" );

  dbg->debug_callback( (void*)aafd, DEBUG_SRC_ID_DUMP, 0, "", "", 0, dbg->_dbg_msg, dbg->user );
}



void aaf_dump_Identification( AAF_Data *aafd )
{
  int offset = 0;
  struct dbg *dbg = aafd->dbg;

  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " CompanyName          : %ls\n", ( aafd->Identification.CompanyName ) ? aafd->Identification.CompanyName : L"n/a" );
  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " ProductName          : %ls\n", ( aafd->Identification.ProductName ) ? aafd->Identification.ProductName : L"n/a" );
  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " ProductVersion       : %ls\n", ProductVersionToText( aafd->Identification.ProductVersion ) );
  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " ProductVersionString : %ls\n", ( aafd->Identification.ProductVersionString ) ? aafd->Identification.ProductVersionString : L"n/a" );
  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " ProductID            : %ls\n", AUIDToText( aafd->Identification.ProductID ) );
  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " Date                 : %ls\n", TimestampToText( aafd->Identification.Date ) );
  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " ToolkitVersion       : %ls\n", ProductVersionToText( aafd->Identification.ToolkitVersion ) );
  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " Platform             : %ls\n", ( aafd->Identification.Platform ) ? aafd->Identification.Platform : L"n/a" );
  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " GenerationAUID       : %ls\n", AUIDToText( aafd->Identification.GenerationAUID ) );

  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, "\n\n" );

  dbg->debug_callback( (void*)aafd, DEBUG_SRC_ID_DUMP, 0, "", "", 0, dbg->_dbg_msg, dbg->user );
}



void aaf_dump_ObjectProperty( AAF_Data *aafd, aafProperty *Prop )
{
  int offset = 0;
  struct dbg *dbg = aafd->dbg;

	offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " :.: (0x%04x) %ls (%ls)\n", Prop->pid, PIDToText( aafd, Prop->pid ), StoredFormToText( Prop->sf ) /*AUIDToText( &Prop->def->type ),*/ /*TypeIDToText( &(Prop->def->type) )*/ );

	// WARNING : Wont print strong references (set/vector) corectly.
	offset += dump_hex( Prop->val, Prop->len, &aafd->dbg->_dbg_msg, &aafd->dbg->_dbg_msg_size, offset );

  dbg->debug_callback( (void*)aafd, DEBUG_SRC_ID_DUMP, 0, "", "", 0, dbg->_dbg_msg, dbg->user );
}



void aaf_dump_ObjectProperties( AAF_Data *aafd, aafObject *Obj )
{
  /*
   *  List the properties once they have been parsed and interpreted by AAFCore.
   */

  // int offset = 0;
  // struct dbg *dbg = aafd->dbg;

	aafProperty * Prop = NULL;

	for ( Prop = Obj->Properties;  Prop != NULL; Prop = Prop->next )
	{
    aaf_dump_ObjectProperty( aafd, Prop );
		// offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " :.: (0x%04x) %ls (%ls)\n", Prop->pid, PIDToText( aafd, Prop->pid ), StoredFormToText( Prop->sf ) /*AUIDToText( &Prop->def->type ),*/ /*TypeIDToText( &(Prop->def->type) )*/ );
    //
		// // WARNING : Wont print strong references (set/vector) corectly.
		// dump_hex( Prop->val, Prop->len );
	}
}



void aaf_dump_rawProperties( AAF_Data *aafd, aafPropertyIndexHeader_t *PropHeader )
{
  int offset = 0;
  struct dbg *dbg = aafd->dbg;

  aafPropertyIndexHeader_t *Header = PropHeader;
  aafPropertyIndexEntry_t  *Prop   = NULL;
  aafByte_t                *value  = NULL;

  uint32_t i = 0;

  if ( Header == NULL ) {
    offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset,
      " ## Property_Header____________________________________________________\n\n"
      " aafPropertyIndexHeader_t is NULL\n"
      " ======================================================================\n\n"
    );
    return;
  }


	offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset,
		" ## Property_Header____________________________________________________\n\n"
		" _byteOrder     : 0x%02x\n"
		" _formatVersion : 0x%02x\n"
		" _entryCount    : %u\n\n"
        " ======================================================================\n\n",
		Header->_byteOrder,
		Header->_formatVersion,
		Header->_entryCount
	);

	offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, "\n\n" );

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

		offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset,
			" #%u Property_Entry_____________________________________________________\n"
			" _pid        : 0x%04x (%ls)\n"
			" _storedForm : %ls\n"
			" _length     : %u bytes\n",
			i,
			Prop->_pid, PIDToText( aafd, Prop->_pid ),
			StoredFormToText( Prop->_storedForm ),
			Prop->_length
		);

		offset += dump_hex( value, Prop->_length, &aafd->dbg->_dbg_msg, &aafd->dbg->_dbg_msg_size, offset );

		offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, "\n\n" );
	}

  dbg->debug_callback( (void*)aafd, DEBUG_SRC_ID_DUMP, 0, "", "", 0, dbg->_dbg_msg, dbg->user );
}



void aaf_dump_nodeStreamProperties( AAF_Data *aafd, cfbNode *node )
{
  /*
   *  List the raw properties directly from a CFB Node's stream.
   */

  // int offset = 0;
  // struct dbg *dbg = aafd->dbg;

	aafPropertyIndexHeader_t *Header = NULL;
	// aafPropertyIndexEntry_t  *Prop   = NULL;
	// aafByte_t                *value  = NULL;

	cfb_getStream( aafd->cfbd, node, (unsigned char**)&Header, NULL );

  aaf_dump_rawProperties( aafd, Header );

/*
	uint32_t i = 0;


	offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset,
		" ## Property_Header____________________________________________________\n\n"
		" _byteOrder     : 0x%02x\n"
		" _formatVersion : 0x%02x\n"
		" _entryCount    : %u\n\n"
        " ======================================================================\n\n",
		Header->_byteOrder,
		Header->_formatVersion,
		Header->_entryCount
	);

	offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, "\n\n" );


	// foreachPropertyEntry( Header, Prop, value, i )
	for ( Prop = (aafPropertyIndexEntry_t*)(((char*)Header) + sizeof(aafPropertyIndexHeader_t)),    \
	      value = ((unsigned char*)Prop) + (Header->_entryCount * sizeof(aafPropertyIndexEntry_t)), \
	      i = 0;                                                                                    \
	      i < Header->_entryCount;                                                                  \
	      value += Prop->_length,                                                                   \
	      Prop++,                                                                                   \
	      i++ )
	{

		offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset,
			" #%u Property_Entry_____________________________________________________\n"
			" _pid        : 0x%04x (%ls)\n"
			" _storedForm : %ls\n"
			" _length     : %u bytes\n",
			i,
			Prop->_pid, PIDToText( aafd, Prop->_pid ),
			StoredFormToText( Prop->_storedForm ),
			Prop->_length
		);

		dump_hex( value, Prop->_length, &aafd->dbg->_dbg_msg, &aafd->dbg->_dbg_msg_size );

		offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, "\n\n" );
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

  int offset = 0;
  struct dbg *dbg = aafd->dbg;

  aafClass *Class = NULL;


  foreachClass( Class, aafd->Classes )
  {

    int print = 0;

    aafPropertyDef *PDef = NULL;

    foreachPropertyDefinition( PDef, Class->Properties )
    {
      if ( Class->meta )
      {
        offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, ANSI_COLOR_YELLOW "%ls::%ls (0x%04x)\n" ANSI_COLOR_RESET,
          Class->name,
          PDef->name,
          PDef->pid );

        print++;
      }
      else if ( PDef->meta )
      {
        offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, "%ls::" ANSI_COLOR_YELLOW "%ls (0x%04x)\n" ANSI_COLOR_RESET,
          ClassIDToText( aafd, Class->ID ),
          PDef->name,
          PDef->pid );

        print++;
      }
    }

    if ( print )
      offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, "\n" );

    print = 1;
  }

  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, "\n\n" );

  dbg->debug_callback( (void*)aafd, DEBUG_SRC_ID_DUMP, 0, "", "", 0, dbg->_dbg_msg, dbg->user );
}



void aaf_dump_Classes( AAF_Data *aafd )
{
  int offset = 0;
  struct dbg *dbg = aafd->dbg;

  aafClass *ConcreteClass = NULL;
  aafClass *Class         = NULL;

  foreachClass( ConcreteClass, aafd->Classes )
  {
    foreachClassInheritance( Class, ConcreteClass )
    {
      offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, "%s%ls%s",
        (Class->meta) ? ANSI_COLOR_YELLOW : "",
        ClassIDToText( aafd, Class->ID ),
        (Class->meta) ? ANSI_COLOR_RESET  : "" );


      if ( Class->Parent != NULL )
        offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, " > " );

    }

    offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, "\n" );
  }

  offset += snprintf_realloc( &dbg->_dbg_msg, &dbg->_dbg_msg_size, offset, "\n\n" );

  dbg->debug_callback( (void*)aafd, DEBUG_SRC_ID_DUMP, 0, "", "", 0, dbg->_dbg_msg, dbg->user );
}

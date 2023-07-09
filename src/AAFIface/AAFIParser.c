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

/**
 *	@file LibAAF/AAFIface/AAFIParser.c
 *	@brief AAF processing
 *	@author Adrien Gesta-Fline
 *	@version 0.1
 *	@date 27 june 2018
 *
 *	@ingroup AAFIface
 *	@addtogroup AAFIface
 *
 *
 *
 *
 *	@{
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <wchar.h>
#include <locale.h>

#include <math.h>

#include <libaaf/AAFIface.h>
#include <libaaf/AAFIParser.h>
#include <libaaf/AAFIAudioFiles.h>
#include <libaaf/AAFToText.h>
#include <libaaf/AAFDump.h>
#include <libaaf/debug.h>

#include <libaaf/ProTools.h>
#include <libaaf/Resolve.h>


#include  <libaaf/AAFDefs/AAFClassDefUIDs.h>
#include  <libaaf/AAFDefs/AAFPropertyIDs.h>
// #include  <libaaf/AAFDefs/AAFCompressionDefs.h>
#include  <libaaf/AAFDefs/AAFDataDefs.h>
#include  <libaaf/AAFDefs/AAFOperationDefs.h>
#include  <libaaf/AAFDefs/AAFParameterDefs.h>
#include  <libaaf/AAFDefs/AAFInterpolatorDefs.h>
#include  <libaaf/AAFDefs/AAFTypeDefUIDs.h>
#include  <libaaf/AAFDefs/AAFExtEnum.h>
// #include  <libaaf/AAFDefs/AAFFileKinds.h>
#include  <libaaf/AAFDefs/AAFOPDefs.h>
// #include  <libaaf/AAFDefs/AAFContainerDefs.h>

#include "../common/utils.h"



#ifndef uint
#define uint unsigned int // windows
#endif

#ifdef _WIN32
  #define WPRIs  L"S" // char*
	#define WPRIws L"s" // wchar_t*
#else
  #define WPRIs  L"s"  // char*
	#define WPRIws L"ls" // wchar_t*
#endif








// #define OK      0
// #define ERROR   1
// #define WARNING 2
// #define INFO    3
// #define NOT_SUPPORTED 4


#define INIT_DUMP( aafi ) \
	int current_dump_level = aafi->ctx.current_dump_level;

// #define DUMP_OBJ( aafi, Obj, state )
// 	_dump_obj( aafi, Obj, state, current_dump_level );




#define RESET_CONTEXT( ctx )                        \
	/*ctx.MobSlot = NULL;*/                             \
	ctx.current_track = NULL;                       \
	ctx.current_track_is_multichannel = 0;          \
	ctx.current_multichannel_track_channel = 0;     \
	ctx.current_multichannel_track_clip_length = 0; \
	/*ctx.current_pos = 0;*/                            \
	ctx.current_transition = NULL;                  \
	ctx.current_clip_gain = NULL;                        \
	ctx.current_clip_automation = NULL;                        \
	ctx.current_essence = NULL;                \
	ctx.current_clip = NULL;                        \
	ctx.current_clip_is_muted = 0;                  \



// typedef struct trace_dump {
// 	int   fn; // line number of current __td
// 	int  pfn; // line number of previous __td
// 	int   lv; // current level
// 	int  *ll; // level loop : each entry correspond to a level and tell if there is more to print
// 	int  eob; // end of branch
// 	int   hc; // have children
// 	int  sub;
// } td;

/*
#define __td_set( __td, __ptd, offset ) \
	__td.fn  = __LINE__;    \
	__td.pfn = __ptd->fn;    \
	__td.lv  = __ptd->lv+offset; \
	__td.ll  = __ptd->ll;   \
	__td.ll[__td.lv] = (offset > 0) ? 0 : __td.ll[__td.lv];   \
	__td.eob = 0; \
	__td.hc  = 0; \
	__td.sub = 0; \
*/





// static void trace_obj( AAF_Iface *aafi, aafObject *Obj, char *color );


static wchar_t   * build_unique_audiofilename( AAF_Iface *aafi, aafiAudioEssence *audioEssence );
static wchar_t   * build_unique_videofilename( AAF_Iface *aafi, aafiVideoEssence *videoEssence );

/* TODO move to AAFCore.c */
static aafObject * get_Object_Ancestor( AAF_Iface *aafi, aafObject *Obj, const aafUID_t *ClassID );

static aafUID_t  * get_Component_DataDefinition( AAF_Iface *aafi, aafObject *Component );
// static aafUID_t  * get_FileDescriptor_ContainerFormat( AAF_Iface *aafi, aafObject *FileDescriptor );
static aafUID_t  * get_OperationGroup_OperationIdentification( AAF_Iface *aafi, aafObject *OperationGroup );
static aafUID_t  * get_Parameter_InterpolationIdentification( AAF_Iface *aafi, aafObject *Parameter );

static aafObject * get_EssenceData_By_MobID( AAF_Iface *aafi, aafMobID_t *MobID );

// static aafiAudioEssence * getAudioEssenceBySourceMobID( AAF_Iface *aafi, aafMobID_t *sourceMobID );
// static aafiVideoEssence * getVideoEssenceBySourceMobID( AAF_Iface *aafi, aafMobID_t *sourceMobID );

static int   parse_DigitalImageDescriptor( AAF_Iface *aafi, aafObject *DIDescriptor, td *__ptd );
static int   parse_CDCIDescriptor( AAF_Iface *aafi, aafObject *CDCIDescriptor, td *__ptd );

static int   parse_EssenceDescriptor( AAF_Iface *aafi, aafObject *EssenceDesc, td *__ptd );
static int   parse_PCMDescriptor( AAF_Iface *aafi, aafObject *PCMDescriptor, td *__ptd );
static int   parse_WAVEDescriptor( AAF_Iface *aafi, aafObject *WAVEDescriptor, td *__ptd );
static int   parse_AIFCDescriptor( AAF_Iface *aafi, aafObject *AIFCDescriptor, td *__ptd );

static int   parse_Locator( AAF_Iface *aafi, aafObject *Locator, td *__ptd );
static int   parse_NetworkLocator( AAF_Iface *aafi, aafObject *NetworkLocator, td *__ptd );

static int   parse_EssenceData( AAF_Iface *aafi, aafObject *EssenceData, td *__ptd );


static int   parse_Component( AAF_Iface *aafi, aafObject *Component, td *__ptd );
static int   parse_Transition( AAF_Iface *aafi, aafObject *Transition, td *__ptd );
static int   parse_NestedScope( AAF_Iface *aafi, aafObject *NestedScope, td *__ptd );
// static int   parse_Segment( AAF_Iface *aafi, aafObject *Segment, td *__ptd );
static int   parse_Filler( AAF_Iface *aafi, aafObject *Filler, td *__ptd );
static int   parse_Sequence( AAF_Iface *aafi, aafObject *Sequence, td *__ptd );
static int   parse_Timecode( AAF_Iface *aafi, aafObject *Timecode, td *__ptd );
static int   parse_OperationGroup( AAF_Iface *aafi, aafObject *OpGroup, td *__ptd );
static int   parse_SourceClip( AAF_Iface *aafi, aafObject *SourceClip, td *__ptd );
static int   parse_Selector( AAF_Iface *aafi, aafObject *Selector, td *__ptd );


static int   parse_Parameter( AAF_Iface *aafi, aafObject *Parameter, td *__ptd );
static int   parse_ConstantValue( AAF_Iface *aafi, aafObject *ConstantValue, td *__ptd );
static int   parse_VaryingValue( AAF_Iface *aafi, aafObject *VaryingValue, td *__ptd );
static int   retrieve_ControlPoints( AAF_Iface *aafi, aafObject *Points, aafRational_t *times[], aafRational_t *values[] );


static int   parse_Mob( AAF_Iface *aafi, aafObject *Mob );
static int   parse_CompositionMob( AAF_Iface *aafi, aafObject *CompoMob, td *__ptd );
// static int   parse_MasterMob( AAF_Iface *aafi, aafObject *MasterMob, td *__ptd );
static int   parse_SourceMob( AAF_Iface *aafi, aafObject *SourceMob, td *__ptd );

static int   parse_MobSlot( AAF_Iface *aafi, aafObject *MobSlot, td *__ptd );

static void xplore_StrongObjectReferenceVector( AAF_Iface *aafi, aafObject *ObjCollection, td *__ptd );



static void xplore_StrongObjectReferenceVector( AAF_Iface *aafi, aafObject *ObjCollection, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 0);

	// aaf_dump_ObjectProperties( aafi->aafd, ComponentAttributeList );


	aafObject *Obj = NULL;

	aaf_foreach_ObjectInSet( &Obj, ObjCollection, NULL )
	{
		// aaf_dump_ObjectProperties( aafi->aafd, ObjCollection );

		/* TODO implement retrieve_TaggedValue() */

		if ( aaf_get_property( Obj, PID_TaggedValue_Name ) &&
		     aaf_get_property( Obj, PID_TaggedValue_Value ) )
		{
			wchar_t  *name = aaf_get_propertyValueWstr( Obj, PID_TaggedValue_Name );
			void    *value = aaf_get_propertyIndirectValue( Obj, PID_TaggedValue_Value );
			aafUID_t *type = aaf_get_propertyIndirectValueType( Obj, PID_TaggedValue_Value );

			if ( aafUIDCmp( type, &AAFTypeID_Int32 ) ) {
				printf( "Tagged     |     Name: %ls%*s      Value (%ls)  : %i\n", name, 56-(int)wcslen(name), " ", TypeIDToText(type), *(int32_t*)value );
			}
			else
			if ( aafUIDCmp( type, &AAFTypeID_String ) ) {
				value = aaf_get_propertyIndirectValueWstr( Obj, PID_TaggedValue_Value );
				printf( "Tagged     |     Name: %ls%*s      Value (%ls) : %ls\n", name, 56-(int)wcslen(name), " ", TypeIDToText(type), (wchar_t*)value );
				free( value );
			}
			else {
				printf( "Tagged     |     Name: %ls%*s      Value (%s%ls%s) : %sUNKNOWN_TYPE%s\n", name, 56-(int)wcslen(name), " ", ANSI_COLOR_RED, TypeIDToText(type), ANSI_COLOR_RESET, ANSI_COLOR_RED, ANSI_COLOR_RESET );
			}

			free( name );
		}
		else {
			aaf_dump_ObjectProperties( aafi->aafd, Obj );
		}
	}
}



void _DUMP_OBJ( AAF_Iface *aafi, aafObject *Obj, struct trace_dump *__td, int state, int line, const char *fmt, ... )
{
	if ( aafi->ctx.options.trace == 0 )
		return;

	/* Print caller line number */

	if ( Obj )
	{
		switch ( state ) {
			case TD_ERROR:		      printf( "%s", ANSI_COLOR_RED      );  break;
			case TD_WARNING:	      printf( "%s", ANSI_COLOR_YELLOW   );  break;
			case TD_NOT_SUPPORTED: printf( "%s", ANSI_COLOR_ORANGE    );  break;
			default:            printf( "%s", ANSI_COLOR_DARKGREY );  break;
		}
		printf( "%05i", line );
	}
	else
	{
		printf("     ");
	}

	printf( "%s%ls%s", ANSI_COLOR_DARKGREY, L"\u2502", ANSI_COLOR_RESET ); // │

	/* Print padding and vertical lines */

	if ( __td->lv > 0 )
	{
		for ( int i = 0; i < __td->lv; i++ )
		{
			/* current level iteration has more than one entry remaining in loop */
			if ( __td->ll[i] > 1 )
			{
				/* next level iteration is current trace */
				if ( i+1 == __td->lv )
				{
					if ( Obj ) {
						printf( "%ls", L"\u251c\u2500\u2500\u25fb " ); // ├──◻
					}
					else {
						printf( "%ls", L"\u2502    " ); // │
					}
				}
				else {
					printf( "%ls", L"\u2502    " ); // │
				}
			}
			else if ( i+1 == __td->lv && Obj ) {
				printf( "%ls", L"\u2514\u2500\u2500\u25fb " ); // └──◻
			}
			else {
				printf("     ");
			}
		}
	}


	if ( Obj )
	{

		switch ( state ) {
			case TD_ERROR:		      printf( "%s", ANSI_COLOR_RED      );  break;
			case TD_WARNING:	      printf( "%s", ANSI_COLOR_YELLOW   );  break;
			case TD_NOT_SUPPORTED: printf( "%s", ANSI_COLOR_ORANGE    );  break;
			case TD_INFO:
			case TD_OK:
				if ( __td->sub )
					printf( "%s", ANSI_COLOR_DARKGREY );
				else
					printf( "%s", ANSI_COLOR_CYAN );

				break;
		}


		printf( "%ls ", ClassIDToText(aafi->aafd, Obj->Class->ID) );

		printf( "%s", ANSI_COLOR_RESET );


		if ( aafUIDCmp( Obj->Class->ID, &AAFClassID_TimelineMobSlot ) && aafUIDCmp( Obj->Parent->Class->ID, &AAFClassID_CompositionMob ) )
		{
			aafObject *Segment        = aaf_get_propertyValue( Obj, PID_MobSlot_Segment );
			aafUID_t  *DataDefinition = get_Component_DataDefinition( aafi, Segment );
			wchar_t   *name           = aaf_get_propertyValueWstr( Obj, PID_MobSlot_SlotName );
      int32_t   *slotID         = aaf_get_propertyValue( Obj, PID_MobSlot_SlotID );
      int32_t   *trackNo        = aaf_get_propertyValue( Obj, PID_MobSlot_PhysicalTrackNumber );

			printf("[slot:%s%i%s track:%s%i%s] (DataDef : %s%ls%s) %s%ls ",
				ANSI_COLOR_BOLD,
				(slotID) ? *slotID : -1,
				ANSI_COLOR_RESET,
        ANSI_COLOR_BOLD,
        (trackNo) ? *trackNo : -1,
        ANSI_COLOR_RESET,
				ANSI_COLOR_DARKGREY,
				DataDefToText( aafi->aafd, DataDefinition ),
				ANSI_COLOR_RESET,
				(name[0] != 0x00) ? ": " : "", (name) ? name : L""
			 );

			 free( name );
		}
		else if ( aafUIDCmp( Obj->Class->ID, &AAFClassID_CompositionMob ) ||
		          aafUIDCmp( Obj->Class->ID, &AAFClassID_MasterMob ) ||
		          aafUIDCmp( Obj->Class->ID, &AAFClassID_SourceMob ) )
		{
			aafUID_t *usageCode = aaf_get_propertyValue( Obj, PID_Mob_UsageCode );
			wchar_t  *name      = aaf_get_propertyValueWstr( Obj, PID_Mob_Name );

			printf( "(UsageCode: %s%ls%s) %s%ls",
				ANSI_COLOR_DARKGREY,
				UsageCodeToText( usageCode ),
				ANSI_COLOR_RESET,
			  (name && name[0] != 0x00) ? ": " : "", (name) ? name : L""
			);

			free( name );
		}
		else if ( aafUIDCmp( Obj->Class->ID, &AAFClassID_OperationGroup ) )
		{
			aafUID_t *OperationIdentification = get_OperationGroup_OperationIdentification( aafi, Obj );

			printf( "(OpIdent: %s%ls%s) ",
				ANSI_COLOR_DARKGREY,
				OperationDefToText( aafi->aafd, OperationIdentification ),
				ANSI_COLOR_RESET
			);
		}
		// else if ( aafUIDCmp( Obj->Class->ID, &AAFClassID_TapeDescriptor ) ||
		// 					aafUIDCmp( Obj->Class->ID, &AAFClassID_FilmDescriptor ) ||
		// 	        aafUIDCmp( Obj->Class->ID, &AAFClassID_CDCIDescriptor ) ||
		//           aafUIDCmp( Obj->Class->ID, &AAFClassID_RGBADescriptor ) ||
		// 					aafUIDCmp( Obj->Class->ID, &AAFClassID_TIFFDescriptor ) ||
		// 					aafUIDCmp( Obj->Class->ID, &AAFClassID_SoundDescriptor ) ||
		//           aafUIDCmp( Obj->Class->ID, &AAFClassID_PCMDescriptor ) ||
		// 					aafUIDCmp( Obj->Class->ID, &AAFClassID_AES3PCMDescriptor ) ||
		// 					aafUIDCmp( Obj->Class->ID, &AAFClassID_WAVEDescriptor ) ||
		// 					aafUIDCmp( Obj->Class->ID, &AAFClassID_AIFCDescriptor ) )
		// {
		// 	aafUID_t *ContainerFormat = get_FileDescriptor_ContainerFormat( aafi, Obj );
		// 	printf( "(ContainerIdent : \x1b[38;5;242m%ls\x1b[0m)", ContainerToText(ContainerFormat) );
		// }


		if ( state == TD_ERROR )
		{
			printf( ": %s", ANSI_COLOR_RED );
		}
		else if ( state == TD_INFO )
		{
			printf( ": %s", ANSI_COLOR_CYAN );
		}

		va_list args;

		va_start(args, fmt);
		vprintf(fmt, args);
		va_end(args);

		if ( state == TD_ERROR || state == TD_INFO )
		{
			printf(".");
		}

		if ( state == TD_NOT_SUPPORTED || ( aafi->ctx.options.trace_class && wcscmp( ClassIDToText(aafi->aafd, Obj->Class->ID), aafi->ctx.options.trace_class ) == 0) ) {

			printf( "\n%s", ( state == TD_NOT_SUPPORTED ) ? ANSI_COLOR_ORANGE : "" );

			// printf("CFB Object Dump : %ls\n", aaf_get_ObjectPath( Obj ) );
			// printf("=================\n" );
			// cfb_dump_node( aafi->aafd->cfbd, Obj->Node, 1 );

			printf( "Properties Dump (%ls)\n", aaf_get_ObjectPath( Obj ) );
			printf( "===============\n\n" );
			// aaf_dump_nodeStreamProperties( aafi->aafd, Obj->Node );
			aaf_dump_ObjectProperties( aafi->aafd, Obj );
		}
		else {

			aafProperty * Prop = NULL;
			int hasUnknownProps = 0;
			for ( Prop = Obj->Properties;  Prop != NULL; Prop = Prop->next )
			{
				if ( Prop->def->meta ) {

					// printf("\n");

					if ( aafi->ctx.options.trace_meta ) {
						// aaf_dump_ObjectProperties( aafi->aafd, Obj );

						// if ( Prop->pid == 0xffca ) {
						if ( Prop->sf == SF_STRONG_OBJECT_REFERENCE_VECTOR ) {
							printf("\n" );
							printf( " >>> (0x%04x) %ls (%ls)\n", Prop->pid, PIDToText( aafi->aafd, Prop->pid ), StoredFormToText( Prop->sf ) /*AUIDToText( &Prop->def->type ),*/ /*TypeIDToText( &(Prop->def->type) )*/ );
							void *propValue = aaf_get_propertyValue( Obj, Prop->pid );
							xplore_StrongObjectReferenceVector( aafi, propValue, __td );

							// DUMP_OBJ_NO_SUPPORT( aafi, propValue, __td );
						}
						else {
							printf("\n" );
							aaf_dump_ObjectProperty( aafi->aafd, Prop );
						}
					}
					else {
						printf( "%s%s %ls[0x%04x]", ANSI_COLOR_RESET, (!hasUnknownProps) ? "  (MetaProps:" : "", PIDToText( aafi->aafd, Prop->pid ), Prop->pid );
						// dump_hex( Prop->val, Prop->len );
						hasUnknownProps++;
					}
				}
			}
			if ( aafi->ctx.options.trace_meta == 0 && hasUnknownProps ) {
				printf(")");
			}
		}

		printf( "%s", ANSI_COLOR_RESET );
	}


	printf("\n");


	/* if end of branch, print one line padding */
	if ( Obj && ( __td->eob || state == TD_ERROR ) )
		_DUMP_OBJ( aafi, NULL, __td, 0, -1, "" );
}



void _DUMP_OBJ_NO_SUPPORT( AAF_Iface *aafi, aafObject *Obj, struct trace_dump *__td, int line )
{

	// aafUID_t *DataDefinition = NULL;

	if ( aafUIDCmp( Obj->Class->ID, &AAFClassID_TimelineMobSlot ) && aafUIDCmp( Obj->Parent->Class->ID, &AAFClassID_CompositionMob ) )
	{
		/* this part is handled by _DUMP_OBJ() already. */
		_DUMP_OBJ( aafi, Obj, __td, TD_NOT_SUPPORTED, line, "" );
		return;
	// 	aafObject *Segment = aaf_get_propertyValue( Obj, PID_MobSlot_Segment );
	//
	// 	if ( Segment != NULL ) /* req */
	// 	{
	// 		DataDefinition = get_Component_DataDefinition( aafi, Segment );
	// 	}
	// }
	// else
	// {
	// 	DataDefinition = get_Component_DataDefinition( aafi, Obj );
	}

	// DataDefinition = get_Component_DataDefinition( aafi, Obj );


	_DUMP_OBJ( aafi, Obj, __td, TD_NOT_SUPPORTED, line, "" );

	// _DUMP_OBJ( aafi, Obj, __td, WARNING, line, "%s%ls%s",
	// 	// ClassIDToText( aafi->aafd, Obj->Class->ID ),
	//  	((DataDefinition) ? "(Segment DataDefinition: \x1b[38;5;242m" : ""),
	// 	((DataDefinition) ? DataDefToText( aafi->aafd, DataDefinition ) : L""),
	//  	((DataDefinition) ? ") \x1b[0m" : "") );

}


/*
#define DUMP_OBJ( aafi, Obj, __td ) \
	_DUMP_OBJ( aafi, Obj, __td, OK, __LINE__, "" );

#define DUMP_OBJ_ERROR( aafi, Obj, __td, ... ) \
	(__td)->eob = 1; \
	_DUMP_OBJ( aafi, Obj, __td, ERROR, __LINE__, __VA_ARGS__ );

#define DUMP_OBJ_WARNING( aafi, Obj, __td, ... ) \
	_DUMP_OBJ( aafi, Obj, __td, WARNING, __LINE__, __VA_ARGS__ );

#define DUMP_OBJ_INFO( aafi, Obj, __td, ... ) \
	_DUMP_OBJ( aafi, Obj, __td, OK, __LINE__, __VA_ARGS__ );

#define DUMP_OBJ_NO_SUPPORT( aafi, Obj, __td ) \
	(__td)->eob = 1; \
	_DUMP_OBJ_NO_SUPPORT( aafi, Obj, __td, __LINE__ ); \
	// aaf_dump_ObjectProperties( aafi->aafd, Obj );
*/






void trace_obj( AAF_Iface *aafi, aafObject *Obj, const char *color )
{
	return;
	char buf[4096];
	char tmp[1024];
	int  offset = sizeof(buf) - 1;
	aafObject *lastChild = Obj;

	// buf[offset] = 0x00;
	memset( buf, 0x00, sizeof(buf) );
	// memset( tmp, 0x00, sizeof(tmp) );

	for (; aafUIDCmp( Obj->Class->ID, &AAFClassID_ContentStorage ) == 0 /*Obj != NULL*/; Obj = Obj->Parent )
	{

		if ( aafUIDCmp( Obj->Class->ID, &AAFClassID_TimelineMobSlot ) && aafUIDCmp( Obj->Parent->Class->ID, &AAFClassID_CompositionMob ) )
		{
			wchar_t *name = aaf_get_propertyValueWstr( Obj, PID_MobSlot_SlotName );
			snprintf( tmp, sizeof(tmp), "%ls (%ls)", ClassIDToText( aafi->aafd, Obj->Class->ID ), name );
			free( name );
		}
		else if ( aafUIDCmp( Obj->Class->ID, &AAFClassID_MasterMob ) || aafUIDCmp( Obj->Class->ID, &AAFClassID_SourceMob ) )
		{
			wchar_t *name  = aaf_get_propertyValueWstr( Obj, PID_Mob_Name );
			snprintf( tmp, sizeof(tmp), "%ls (%ls)", ClassIDToText( aafi->aafd, Obj->Class->ID ), name );
			free( name );
		}
		else if ( aafUIDCmp( Obj->Class->ID, &AAFClassID_CompositionMob ) )
		{
			wchar_t *name = aaf_get_propertyValueWstr( Obj, PID_Mob_Name );
			const wchar_t *usage = UsageCodeToText( aaf_get_propertyValue( Obj, PID_Mob_UsageCode ) );
			snprintf( tmp, sizeof(tmp), "%ls (%ls : %ls)", ClassIDToText( aafi->aafd, Obj->Class->ID ), usage, name );
			free( name );
		}
		else if ( aafUIDCmp( Obj->Class->ID, &AAFClassID_OperationGroup ) )
		{
			aafUID_t *OperationIdentification = get_OperationGroup_OperationIdentification( aafi, Obj );

			const wchar_t *name = OperationDefToText( aafi->aafd, OperationIdentification ) /*AUIDToText( OpIdent )*/;
			snprintf( tmp, sizeof(tmp), "%ls (%ls)", ClassIDToText( aafi->aafd, Obj->Class->ID ), name );
		}
		else
		{
			snprintf( tmp, sizeof(tmp), "%ls", ClassIDToText( aafi->aafd, Obj->Class->ID ) );
		}

		if ( Obj != lastChild )
		{
			buf[--offset] = ' ';
			buf[--offset] = '>';
			buf[--offset] = ' ';
		}

		offset -= strlen(tmp);

		memcpy( buf+offset, tmp, strlen(tmp) );
	}

	printf("%s%s%s\n", color, buf+offset, ANSI_COLOR_RESET );
}




static wchar_t * build_unique_audiofilename( AAF_Iface *aafi, aafiAudioEssence *audioEssence )
{
	/* TODO 1024 should be a macro ! */

	wchar_t *unique = calloc( sizeof(wchar_t), 1024 );

	size_t file_name_len = wcslen( audioEssence->file_name );

	// printf("%i\n", file_name_len );

	memcpy( unique, audioEssence->file_name, (file_name_len + 1) * sizeof(wchar_t) );

	// printf("%ls\n", unique );

	aafiAudioEssence *ae = NULL;

	if ( 1 )
	{
		size_t i = 0;

		for ( ; i < file_name_len; i++ )
		{
			/* if char is out of the Basic Latin range */
			if ( unique[i] > 0xff )
			{
				// printf("MobID : %ls\n", MobIDToText( audioEssence->sourceMobID ) );
				aafUID_t *uuid = &(audioEssence->sourceMobID->material);
				swprintf( unique, 1024, L"%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
					uuid->Data1,
					uuid->Data2,
					uuid->Data3,
					uuid->Data4[0],
				 	uuid->Data4[1],
					uuid->Data4[2],
					uuid->Data4[3],
					uuid->Data4[4],
					uuid->Data4[5],
					uuid->Data4[6],
					uuid->Data4[7] );

				audioEssence->unique_file_name = unique;

				return unique;
			}
		}
	}


	int id = 0;

	foreachEssence( ae, aafi->Audio->Essences )
	{
		if ( ae->unique_file_name != NULL && wcscmp( ae->unique_file_name, unique ) == 0 )
		{
			swprintf( unique+file_name_len, (1024-file_name_len), L"_%i", ++id );
			// printf("%ls\n", unique );
			ae = aafi->Audio->Essences; // check again
		}
	}

	audioEssence->unique_file_name = unique;

	// printf("%ls\n", audioEssence->wunique_file_name );

	return unique;
}




static wchar_t * build_unique_videofilename( AAF_Iface *aafi, aafiVideoEssence *videoEssence )
{
	/* TODO 1024 should be a macro ! */

	wchar_t *unique = calloc( sizeof(wchar_t), 1024 );

	size_t file_name_len = wcslen( videoEssence->file_name );

	// printf("%i\n", file_name_len );

	memcpy( unique, videoEssence->file_name, (file_name_len + 1) * sizeof(wchar_t) );

	// printf("%ls\n", unique );

	aafiVideoEssence *ve = NULL;

	if ( 1 )
	{
		size_t i = 0;

		for ( ; i < file_name_len; i++ )
		{
			/* if char is out of the Basic Latin range */
			if ( unique[i] > 0xff )
			{
				// printf("MobID : %ls\n", MobIDToText( videoEssence->sourceMobID ) );
				aafUID_t *uuid = &(videoEssence->sourceMobID->material);
				swprintf( unique, 1024, L"%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
					uuid->Data1,
					uuid->Data2,
					uuid->Data3,
					uuid->Data4[0],
				 	uuid->Data4[1],
					uuid->Data4[2],
					uuid->Data4[3],
					uuid->Data4[4],
					uuid->Data4[5],
					uuid->Data4[6],
					uuid->Data4[7] );

				videoEssence->unique_file_name = unique;

				return unique;
			}
		}
	}


	int id = 0;

	foreachEssence( ve, aafi->Video->Essences )
	{
		if ( ve->unique_file_name != NULL && wcscmp( ve->unique_file_name, unique ) == 0 )
		{
			swprintf( unique+file_name_len, (1024-file_name_len), L"_%i", ++id );
			// printf("%ls\n", unique );
			ve = aafi->Video->Essences; // check again
		}
	}

	videoEssence->unique_file_name = unique;

	// printf("%ls\n", videoEssence->wunique_file_name );

	return unique;
}





static aafObject * get_Object_Ancestor( AAF_Iface *aafi, aafObject *Obj, const aafUID_t *ClassID )
{
	(void)aafi;

	/*
	 * NOTE : AAFClassID_ContentStorage is the container of Mob and EssenceData,
	 * not of Identification, Dictionary and MetaDictionary. If needed, the func
	 * should work for them too thanks to Obj != NULL.
	 */

	for (; Obj != NULL && aafUIDCmp( Obj->Class->ID, &AAFClassID_ContentStorage ) == 0; Obj = Obj->Parent )
		if ( aafUIDCmp( Obj->Class->ID, ClassID ) )
			return Obj;
		/* Also work with abstract class */
		else if ( aafUIDCmp( ClassID, &AAFClassID_Mob ) && ( aafUIDCmp( Obj->Class->ID, &AAFClassID_CompositionMob ) || aafUIDCmp( Obj->Class->ID, &AAFClassID_MasterMob ) || aafUIDCmp( Obj->Class->ID, &AAFClassID_SourceMob ) ) )
			return Obj;
		else if ( aafUIDCmp( ClassID, &AAFClassID_MobSlot ) && ( aafUIDCmp( Obj->Class->ID, &AAFClassID_TimelineMobSlot ) || aafUIDCmp( Obj->Class->ID, &AAFClassID_StaticMobSlot ) || aafUIDCmp( Obj->Class->ID, &AAFClassID_EventMobSlot ) ) )
			return Obj;

	return NULL;
}







/******************************************************************************
                             D i c t i o n a r y
 ******************************************************************************

 ******************************************************************************
 *****************************************************************************/

static aafUID_t * get_Component_DataDefinition( AAF_Iface *aafi, aafObject *Component )
{
	aafWeakRef_t *weakRef = aaf_get_propertyValue( Component, PID_Component_DataDefinition );

	if ( weakRef == NULL )
	{
		_warning( aafi->ctx.options.verb, "Missing Component::DataDefinition.\n" );
		return NULL;
	}


	aafObject *DataDefinition = aaf_get_ObjectByWeakRef( aafi->aafd->DataDefinition, weakRef );

	if ( DataDefinition == NULL )
	{
		_warning( aafi->ctx.options.verb, "Could not retrieve WeakRef from Dictionary::DataDefinition.\n" );
		return NULL;
	}


	aafUID_t  *DataIdentification = aaf_get_propertyValue( DataDefinition, PID_DefinitionObject_Identification );

	if ( DataIdentification == NULL )
	{
		_warning( aafi->ctx.options.verb, "Missing DataDefinition's DefinitionObject::Identification.\n" );
		return NULL;
	}


	return DataIdentification;
}




// static aafUID_t * get_FileDescriptor_ContainerFormat( AAF_Iface *aafi, aafObject *FileDescriptor )
// {
// 	aafWeakRef_t *ContainerDefWeakRef = aaf_get_propertyValue( FileDescriptor, PID_FileDescriptor_ContainerFormat );
//
// 	if ( ContainerDefWeakRef == NULL )
// 	{
// 		_warning( aafi->ctx.options.verb, "Missing FileDescriptor::ContainerFormat.\n" );
// 		return NULL;
// 	}
//
// 	aafObject *ContainerDefinition = aaf_get_ObjectByWeakRef( aafi->aafd->ContainerDefinition, ContainerDefWeakRef );
//
// 	if ( ContainerDefinition == NULL )
// 	{
// 		_warning( aafi->ctx.options.verb, "Could not retrieve WeakRef from Dictionary::ContainerDefinitions.\n" );
// 		return NULL;
// 	}
//
//
// 	aafUID_t  *ContainerIdentification = aaf_get_propertyValue( ContainerDefinition, PID_DefinitionObject_Identification );
//
// 	if ( ContainerIdentification == NULL )
// 	{
// 		_warning( aafi->ctx.options.verb, "Missing ContainerDefinition's DefinitionObject::Identification.\n" );
// 		return NULL;
// 	}
//
//
// 	return ContainerIdentification;
// }




static aafUID_t * get_OperationGroup_OperationIdentification( AAF_Iface *aafi, aafObject *OperationGroup )
{
	aafWeakRef_t *OperationDefWeakRef = aaf_get_propertyValue( OperationGroup, PID_OperationGroup_Operation );

	if ( OperationDefWeakRef == NULL )
	{
		_error( aafi->ctx.options.verb, "Missing OperationGroup::Operation.\n" );
		return NULL;
	}


	aafObject *OperationDefinition = aaf_get_ObjectByWeakRef( aafi->aafd->OperationDefinition, OperationDefWeakRef );

	if ( OperationDefinition == NULL )
	{
		_error( aafi->ctx.options.verb, "Could not retrieve OperationDefinition from dictionary.\n" );
		return NULL;
	}


	aafUID_t *OperationIdentification = aaf_get_propertyValue( OperationDefinition, PID_DefinitionObject_Identification );

	if ( OperationIdentification == NULL )
	{
		_error( aafi->ctx.options.verb, "Missing DefinitionObject::Identification.\n" );
		return NULL;
	}


	return OperationIdentification;
}



/* TODO not parameter ? VaryingValue ? */
static aafUID_t * get_Parameter_InterpolationIdentification( AAF_Iface *aafi, aafObject *Parameter )
{
	aafWeakRef_t *InterpolationDefWeakRef = aaf_get_propertyValue( Parameter, PID_VaryingValue_Interpolation );

	if ( InterpolationDefWeakRef == NULL )
	{
		_error( aafi->ctx.options.verb, "Missing Parameter::Interpolation.\n" );
		return NULL;
	}


	aafObject *InterpolationDefinition = aaf_get_ObjectByWeakRef( aafi->aafd->InterpolationDefinition, InterpolationDefWeakRef );

	if ( InterpolationDefinition == NULL )
	{
		_error( aafi->ctx.options.verb, "Could not find InterpolationDefinition.\n" );
		return NULL;
	}


	aafUID_t  *InterpolationIdentification  = aaf_get_propertyValue( InterpolationDefinition, PID_DefinitionObject_Identification );

	if ( InterpolationIdentification == NULL )
	{
		_error( aafi->ctx.options.verb, "Missing Parameter DefinitionObject::Identification.\n" );
		return NULL;
	}


	return InterpolationIdentification;
}








static aafObject * get_EssenceData_By_MobID( AAF_Iface *aafi, aafMobID_t *MobID )
{
	aafMobID_t *DataMobID = NULL;
	aafObject  *EssenceData = NULL;

	for ( EssenceData = aafi->aafd->EssenceData; EssenceData != NULL; EssenceData = EssenceData->next )
	{
		DataMobID = aaf_get_propertyValue( EssenceData, PID_EssenceData_MobID );

		if ( aafMobIDCmp( DataMobID, MobID ) )
			break;
	}

	return EssenceData;
}



// /* TODO is this SourceMobID or SourceID (masterMobID) ??? */
// static aafiAudioEssence * getAudioEssenceBySourceMobID( AAF_Iface *aafi, aafMobID_t *sourceMobID )
// {
// 	aafiAudioEssence * audioEssence = NULL;
//
//
// 	for ( audioEssence = aafi->Audio->Essences; audioEssence != NULL; audioEssence = audioEssence->next )
// 	{
// 		if ( aafMobIDCmp( audioEssence->masterMobID, sourceMobID ) )
// 			break;
// 	}
//
//
// 	return audioEssence;
// }

// /* TODO is this SourceMobID or SourceID (masterMobID) ??? */
// static aafiVideoEssence * getVideoEssenceBySourceMobID( AAF_Iface *aafi, aafMobID_t *sourceMobID )
// {
// 	aafiVideoEssence * videoEssence = NULL;
//
// 	// printf("%p\n", aafi->Video->tc );
// 	printf("%p\n", aafi->Video->Essences );
// 	printf("%ls\n", MobIDToText( sourceMobID ) );
//
//
// 	for ( videoEssence = aafi->Video->Essences; videoEssence != NULL; videoEssence = videoEssence->next )
// 	{
// 		printf("coucou\n" );
// 		if ( aafMobIDCmp( videoEssence->masterMobID, sourceMobID ) )
// 			break;
// 	}
//
//
// 	return videoEssence;
// }














/******************************************************************************
                      E s s e n c e D e s c r i p t o r
 ******************************************************************************

    EssenceDescriptor (abs)
            |
						|--> FileDescriptor (abs)
						|          |
						|          |--> WAVEDescriptor
						|          |--> AIFCDescriptor
						|          |--> SoundDescriptor
						|          |          |
						|          |          `--> PCMDescriptor
						|          |
						|          `--> DigitalImageDescriptor (abs)
						|                     |
						|                     `--> CDCIDescriptor
						|
						|
						|--> PhysicalDescriptor
						`--> TapeDescriptor

******************************************************************************
******************************************************************************/

static int parse_EssenceDescriptor( AAF_Iface *aafi, aafObject *EssenceDesc, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 0);


	if ( aafUIDCmp( EssenceDesc->Class->ID, &AAFClassID_PCMDescriptor ) )
	{
		parse_PCMDescriptor( aafi, EssenceDesc, &__td );
	}
	else if ( aafUIDCmp( EssenceDesc->Class->ID, &AAFClassID_WAVEDescriptor ) )
	{
		parse_WAVEDescriptor( aafi, EssenceDesc, &__td );
	}
	else if ( aafUIDCmp( EssenceDesc->Class->ID, &AAFClassID_AIFCDescriptor ) )
	{
		parse_AIFCDescriptor( aafi, EssenceDesc, &__td );
	}
	else if ( aafUIDCmp( EssenceDesc->Class->ID, &AAFClassID_SoundDescriptor ) )
	{
		/* Compressed Audio (MP3, AAC ?). Not encountered yet */

		__td.lv++;
		DUMP_OBJ_NO_SUPPORT( aafi, EssenceDesc, &__td );
		__td.lv--;
	}
	else if ( aafUIDCmp( EssenceDesc->Class->ID, &AAFClassID_AES3PCMDescriptor ) )
	{
		/* Not described in specs, not encountered yet. */

		__td.lv++;
 		DUMP_OBJ_NO_SUPPORT( aafi, EssenceDesc, &__td );
		__td.lv--;
	}
	else if ( aafUIDCmp( EssenceDesc->Class->ID, &AAFClassID_MultipleDescriptor ) )
	{
		/*
		 *	A MultipleDescriptor contains a vector of FileDescriptor objects and is
		 *	used when the file source consists of multiple tracks of essence (e.g MXF).
		 *	Each essence track is described by a MobSlots object in the SourceMob and a
		 *	FileDescriptor object. The FileDescriptor is linked to the MobSlot by
		 *	setting the FileDescriptor::LinkedSlotID property equal to the
		 *	MobSlot::SlotID property.
		 *
		 *	-> test.aaf
		 */

		__td.lv++;
 		DUMP_OBJ_NO_SUPPORT( aafi, EssenceDesc, &__td );
		__td.lv--;

	}
	else if ( aafUIDCmp( EssenceDesc->Class->ID, &AAFClassID_CDCIDescriptor ) )
	{
		parse_CDCIDescriptor( aafi, EssenceDesc, &__td );
	}
	else
	{
		__td.lv++;
		DUMP_OBJ_NO_SUPPORT( aafi, EssenceDesc, &__td );
		__td.lv--;
	}


	/*
	 *	Locators are a property of EssenceDescriptor. The property holds a vector of
	 *	Locators object, that should provide information to help find a file that
	 *	contains the essence (WAV, MXF, etc.) or to help find the physical media.
	 *
	 *	A Locator can either be a NetworkLocator or a TextLocator.
	 *
	 *  A NetworkLocator holds a URLString property :
	 *
	 *	p.41 : Absolute Uniform Resource Locator (URL) complying with RFC 1738 or relative
	 *	Uniform Resource Identifier (URI) complying with RFC 2396 for file containing
	 *	the essence. If it is a relative URI, the base URI is determined from the URI
	 *	of the AAF file itself.
	 *	Informative note: A valid URL or URI uses a constrained character set and uses
	 *	the / character as the path separator.
	 */

	aafObject *Locator  = NULL;
	aafObject *Locators = aaf_get_propertyValue( EssenceDesc, PID_EssenceDescriptor_Locator ); /* opt */

	__td.lv++;
	int i = 0;

	aaf_foreach_ObjectInSet( &Locator, Locators, NULL )
	{
		/* TODO retrieve all locators, then when searching file, try all parsed locators. */
		__td.ll[__td.lv] = (Locators->Header->_entryCount > 1) ? (Locators->Header->_entryCount - i++) : 0;
		parse_Locator( aafi, Locator, &__td );
	}


	return 0;
}




static int parse_DigitalImageDescriptor( AAF_Iface *aafi, aafObject *DIDescriptor, td *__ptd )
{
	struct trace_dump __td;
	__td_set(__td, __ptd, 0);


	/* TODO parse and save content to videoEssence */


	aafiVideoEssence *videoEssence = aafi->ctx.current_video_essence;//(aafiVideoEssence*)aafi->ctx.current_essence;

	if ( videoEssence == NULL )
	{
		DUMP_OBJ_ERROR( aafi, DIDescriptor, &__td, "aafi->ctx.current_video_essence not set" );
		return -1;
	}


	aafRational_t *framerate = (aafRational_t*)aaf_get_propertyValue( DIDescriptor, PID_FileDescriptor_SampleRate );

	if ( framerate == NULL )
	{
		DUMP_OBJ_ERROR( aafi, DIDescriptor, &__td, "Missing PID_FileDescriptor_SampleRate" );
		return -1;
	}

	videoEssence->framerate = framerate;



	uint32_t *storedHeight = (uint32_t*)aaf_get_propertyValue( DIDescriptor, PID_DigitalImageDescriptor_StoredHeight );

	if ( storedHeight == NULL )
	{
		DUMP_OBJ_ERROR( aafi, DIDescriptor, &__td, "Missing PID_DigitalImageDescriptor_StoredHeight" );
		return -1;
	}

	// printf("storedHeight : %u\n", *storedHeight );



	uint32_t *storedWidth = (uint32_t*)aaf_get_propertyValue( DIDescriptor, PID_DigitalImageDescriptor_StoredWidth );

	if ( storedWidth == NULL )
	{
		DUMP_OBJ_ERROR( aafi, DIDescriptor, &__td, "Missing PID_DigitalImageDescriptor_StoredWidth" );
		return -1;
	}

	// printf("storedWidth : %u\n", *storedWidth );



	uint32_t *displayHeight = (uint32_t*)aaf_get_propertyValue( DIDescriptor, PID_DigitalImageDescriptor_DisplayHeight );

	if ( displayHeight == NULL )
	{
		DUMP_OBJ_ERROR( aafi, DIDescriptor, &__td, "Missing PID_DigitalImageDescriptor_DisplayHeight" );
		return -1;
	}

	// printf("displayHeight : %u\n", *displayHeight );



	uint32_t *displayWidth = (uint32_t*)aaf_get_propertyValue( DIDescriptor, PID_DigitalImageDescriptor_DisplayWidth );

	if ( displayWidth == NULL )
	{
		DUMP_OBJ_ERROR( aafi, DIDescriptor, &__td, "Missing PID_DigitalImageDescriptor_DisplayWidth" );
		return -1;
	}

	// printf("displayWidth : %u\n", *displayWidth );



	aafRational_t *imageAspectRatio = (aafRational_t*)aaf_get_propertyValue( DIDescriptor, PID_DigitalImageDescriptor_ImageAspectRatio );

	if ( imageAspectRatio == NULL )
	{
		DUMP_OBJ_ERROR( aafi, DIDescriptor, &__td, "Missing PID_DigitalImageDescriptor_ImageAspectRatio" );
		return -1;
	}

	// printf("imageAspectRatio : %i/%i\n", imageAspectRatio->numerator, imageAspectRatio->denominator );



	return 0;
}




static int parse_CDCIDescriptor( AAF_Iface *aafi, aafObject *CDCIDescriptor, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 1);

	if ( !aaf_get_propertyValue( CDCIDescriptor, PID_EssenceDescriptor_Locator ) )
		__td.eob = 1;


	/* TODO parse CDCI class */



	int rc = parse_DigitalImageDescriptor( aafi, CDCIDescriptor, __ptd );

	if ( !rc )
		DUMP_OBJ( aafi, CDCIDescriptor, &__td );


	return rc;
}




static int parse_PCMDescriptor( AAF_Iface *aafi, aafObject *PCMDescriptor, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 1);

	if ( !aaf_get_propertyValue( PCMDescriptor, PID_EssenceDescriptor_Locator ) )
		__td.eob = 1;



	aafiAudioEssence *audioEssence = (aafiAudioEssence*)aafi->ctx.current_essence;

	if ( audioEssence == NULL )
	{
		DUMP_OBJ_ERROR( aafi, PCMDescriptor, &__td, "aafi->ctx.current_essence not set" );
		return -1;
	}


	audioEssence->type = AAFI_ESSENCE_TYPE_PCM;



	/* Duration of the essence in sample units (not edit units !) */
	aafPosition_t *length = (aafPosition_t*)aaf_get_propertyValue( PCMDescriptor, PID_FileDescriptor_Length );

	if ( length == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, PCMDescriptor, &__td, "Missing PID_FileDescriptor_Length" );
		return -1;
	}

	audioEssence->length = *length;



	uint32_t *channels = (uint32_t*)aaf_get_propertyValue( PCMDescriptor, PID_SoundDescriptor_Channels );

	if ( channels == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, PCMDescriptor, &__td, "Missing PID_SoundDescriptor_Channels" );
		return -1;
	}

	audioEssence->channels = *channels;



	uint32_t *samplerate = (uint32_t*)aaf_get_propertyValue( PCMDescriptor, PID_FileDescriptor_SampleRate );

	if ( samplerate == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, PCMDescriptor, &__td, "Missing PID_FileDescriptor_SampleRate" );
		return -1;
	}

	audioEssence->samplerate = *samplerate;

	// if ( aafi->Audio->samplerate >= 0 )
	// {
		/* Set global AAF SampleRate, if it equals preceding. Otherwise set to -1 */
		// aafi->Audio->samplerate = ( aafi->Audio->samplerate == 0 || aafi->Audio->samplerate == *samplerate ) ? *samplerate : (unsigned)-1;
	// }



	uint32_t *samplesize = (uint32_t*)aaf_get_propertyValue( PCMDescriptor, PID_SoundDescriptor_QuantizationBits ); // uint32_t in AAF std

	if ( samplesize == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, PCMDescriptor, &__td, "Missing PID_SoundDescriptor_QuantizationBits" );
		return -1;
	}

	if ( *samplesize >= (1<<15) )
	{
		DUMP_OBJ_ERROR( aafi, PCMDescriptor, &__td, "PID_SoundDescriptor_QuantizationBits value error : %u", *samplesize );
		return -1;
	}

	audioEssence->samplesize = (int16_t)*samplesize;

	if ( aafi->Audio->samplesize >= 0 )
	{
		/* Set global AAF SampleSize, if it equals preceding. Otherwise set to -1 */
		aafi->Audio->samplesize = ( aafi->Audio->samplesize == 0 || (uint16_t)aafi->Audio->samplesize == audioEssence->samplesize ) ? audioEssence->samplesize : -1;
	}


	/* TODO parse the rest of the class */

	DUMP_OBJ( aafi, PCMDescriptor, &__td );


	return 0;
}




static int parse_WAVEDescriptor( AAF_Iface *aafi, aafObject *WAVEDescriptor, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 1);

	if ( !aaf_get_propertyValue( WAVEDescriptor, PID_EssenceDescriptor_Locator ) )
		__td.eob = 1;



	aafiAudioEssence *audioEssence = (aafiAudioEssence*)aafi->ctx.current_essence;

	if ( audioEssence == NULL )
	{
		DUMP_OBJ_ERROR( aafi, WAVEDescriptor, &__td, "aafi->ctx.current_essence not set" );
		return -1;
	}


	audioEssence->type = AAFI_ESSENCE_TYPE_WAVE;



	aafProperty *summary = aaf_get_property( WAVEDescriptor, PID_WAVEDescriptor_Summary );

	if ( summary == NULL )
	{
		DUMP_OBJ_ERROR( aafi, WAVEDescriptor, &__td, "Missing PID_WAVEDescriptor_Summary" );
		return -1;
	}

	audioEssence->summary = summary;

	/*
	 *	NOTE : Summary is parsed later in "post-processing" aafi_retrieveData(),
	 *	to be sure clips and essences are linked, so we are able to fallback on
	 *	essence stream in case summary does not contain the full header part.
	 *
	 *  TODO parse it here
	 */

	DUMP_OBJ( aafi, WAVEDescriptor, &__td );


	return 0;
}




static int parse_AIFCDescriptor( AAF_Iface *aafi, aafObject *AIFCDescriptor, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 1);

	if ( !aaf_get_propertyValue( AIFCDescriptor, PID_EssenceDescriptor_Locator ) )
		__td.eob = 1;



	aafiAudioEssence *audioEssence = (aafiAudioEssence*)aafi->ctx.current_essence;

	if ( audioEssence == NULL )
	{
		DUMP_OBJ_ERROR( aafi, AIFCDescriptor, &__td, "aafi->ctx.current_essence not set" );
		return -1;
	}


	audioEssence->type = AAFI_ESSENCE_TYPE_AIFC;



	aafProperty *summary = aaf_get_property( AIFCDescriptor, PID_AIFCDescriptor_Summary );

	if ( summary == NULL )
	{
		DUMP_OBJ_ERROR( aafi, AIFCDescriptor, &__td, "Missing PID_AIFCDescriptor_Summary" );
		return -1;
	}

	audioEssence->summary = summary;


	/*
	 *	NOTE : Summary is parsed later in "post-processing" aafi_retrieveData(),
	 *	to be sure clips and essences are linked, so we are able to fallback on
	 *	essence stream in case summary does not contain the full header part.
	 */

	DUMP_OBJ( aafi, AIFCDescriptor, &__td );


	return 0;
}








/*
		         Locator (abs)
		            |
		    ,---------------.
		    |               |
  NetworkLocator   TextLocator

*/

static int parse_Locator( AAF_Iface *aafi, aafObject *Locator, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 0);


	if ( aafUIDCmp( Locator->Class->ID, &AAFClassID_NetworkLocator ) )
	{
		parse_NetworkLocator( aafi, Locator, &__td );
	}
	else if ( aafUIDCmp( Locator->Class->ID, &AAFClassID_TextLocator ) )
	{
		/*
		 *	A TextLocator object provides information to the user to help locate the file
		 *	containing the essence or to locate the physical media. The TextLocator is not
		 *	intended for applications to use without user intervention.
		 *
		 *	TODO what to do with those ???
		 *       never encountered anyway..
		 */

		__td.eob = 1;
		__td.lv++;
		DUMP_OBJ_NO_SUPPORT( aafi, Locator, &__td );

		// wchar_t *name = aaf_get_propertyValueWstr( Locator, PID_TextLocator_Name );
		// _warning( aafi->ctx.options.verb, "Got an AAFClassID_TextLocator : \"%ls\"\n", name );
		// free( name );
	}
	else
	{
		__td.eob = 1;
		__td.lv++;
		DUMP_OBJ_NO_SUPPORT( aafi, Locator, &__td );
	}


	return 0;
}




static int parse_NetworkLocator( AAF_Iface *aafi, aafObject *NetworkLocator, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 1);
	__td.eob = 1;


	/*
	 *	This holds an URI pointing to the essence file, when it is not embedded.
	 *	However, sometimes it holds an URI to the AAF file itself when essence is
	 *	embedded so it is not a valid way to test if essence is embedded or not.
	 */

	wchar_t *original_file_path = aaf_get_propertyValueWstr( NetworkLocator, PID_NetworkLocator_URLString );

	if ( original_file_path == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, NetworkLocator, &__td, "Missing PID_NetworkLocator_URLString" );
		return -1;
	}

	wurl_decode( original_file_path, original_file_path ); // TODO : What about URIParser lib ?!


	/* TODO find a better way to check if we're parsing audio */

	if ( aafi->ctx.current_essence ) {
		aafi->ctx.current_essence->original_file_path = original_file_path;
	}
	else if ( aafi->ctx.current_video_essence ) {
		aafi->ctx.current_video_essence->original_file_path = original_file_path;
	}
	else
	{
		DUMP_OBJ_ERROR( aafi, NetworkLocator, &__td, "aafi->ctx.current_essence AND aafi->ctx.current_video_essence not set" );
		return -1;
	}

	DUMP_OBJ_INFO( aafi, NetworkLocator, &__td, ": %ls", original_file_path );


	return 0;
}








static int parse_EssenceData( AAF_Iface *aafi, aafObject *EssenceData, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 1);
	__td.eob = 1;



	aafiAudioEssence *audioEssence = (aafiAudioEssence*)aafi->ctx.current_essence;

	if ( audioEssence == NULL )
	{
		DUMP_OBJ_ERROR( aafi, EssenceData, &__td, "aafi->ctx.current_essence not set" );
		return -1;
	}

	/*
	 *	The EssenceData::Data property has the stored form SF_DATA_STREAM, so
	 *	it holds the name of the Data stream, which should be located at
	 *	/Path/To/EssenceData/DataStream
	 */

	wchar_t *StreamName = aaf_get_propertyValueWstr( EssenceData, PID_EssenceData_Data );

	if ( StreamName == NULL )
	{
		DUMP_OBJ_ERROR( aafi, EssenceData, &__td, "Missing PID_EssenceData_Data" );
		return -1;
	}



	wchar_t DataPath[CFB_PATH_NAME_SZ];
	memset( DataPath, 0x00, sizeof(DataPath) );

	wchar_t *path = aaf_get_ObjectPath( EssenceData );

	swprintf( DataPath, CFB_PATH_NAME_SZ, L"%" WPRIws L"/%" WPRIws, path, StreamName );

	free( StreamName );



	cfbNode *DataNode = cfb_getNodeByPath( aafi->aafd->cfbd, DataPath, 0 );

	if ( DataNode == NULL )
	{
		DUMP_OBJ_ERROR( aafi, EssenceData, &__td, "Could not retrieve Data stream node %ls", DataPath );
		return -1;
	}

	audioEssence->node = DataNode;

	audioEssence->is_embedded = 1; /* TODO to be set elsewhere ? */



	/* disable raw data byte length, because we want it to be the exact audio length in samples */

	// uint64_t dataLen = cfb_getNodeStreamLen( aafi->aafd->cfbd, DataNode );
	//
	// if ( dataLen == 0 )
	// {
	// 	DUMP_OBJ_WARNING( aafi, EssenceData, &__td, "Got 0 Bytes Data stream length" );
	// 	return -1;
	// }
	// else
	// {
	// 	DUMP_OBJ( aafi, EssenceData, &__td );
	// }
	//
	// /* NOTE Might be tweaked by parse_audio_summary() */
	// audioEssence->length = dataLen;


	return 0;
}









/******************************************************************************
                             C o m p o n e n t
 ******************************************************************************

                       Component (abs)
                            |
                      ,-----------.
                      |           |
                 Transition    Segment (abs)
                                  |
                                  |--> Sequence
                                  |--> Filler
                                  |--> TimeCode
                                  |--> OperationGroup
                                  `--> SourceReference (abs)
                                              |
                                              `--> SourceClip

******************************************************************************
******************************************************************************/

static int parse_Component( AAF_Iface *aafi, aafObject *Component, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 0);



	if ( aafUIDCmp( Component->Class->ID, &AAFClassID_Transition ) )
	{
		/*
		 *	A Transition between a Filler and a SourceClip sets a Fade In.
		 *	A Transition between a SourceClip and a Filler sets a Fade Out.
		 *	A Transition between two SourceClips sets a Cross-Fade.
		 *
		 *	Since the Transition applies to the elements that suround it in
		 *	the Sequence, the OperationGroup::InputSegments is then left unused.
		 */

		parse_Transition( aafi, Component, &__td );
	}
	else
	{
		parse_Segment( aafi, Component, &__td );
	}


	return 0;
}




static int parse_Transition( AAF_Iface *aafi, aafObject *Transition, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 1);



	aafUID_t *DataDefinition = get_Component_DataDefinition( aafi, Transition );

	if ( DataDefinition == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, Transition, &__td, "Could not retrieve DataDefinition" );
		return -1;
	}

	if ( !aafUIDCmp( DataDefinition, &AAFDataDef_Sound ) &&
			 !aafUIDCmp( DataDefinition, &AAFDataDef_LegacySound ) )
	{
		DUMP_OBJ_ERROR( aafi, Transition, &__td, "Current implementation only supports Transition inside Audio Tracks" );
		return -1;
	}



	int64_t *length = aaf_get_propertyValue( Transition, PID_Component_Length );

	if ( length == NULL )
	{
		DUMP_OBJ_ERROR( aafi, Transition, &__td, "Missing PID_Component_Length" );
		return -1;
	}


	int flags = 0;

	if ( Transition->prev != NULL && aafUIDCmp( Transition->prev->Class->ID, &AAFClassID_Filler ) )
	{
		flags |= AAFI_TRANS_FADE_IN;
	}
	else if ( Transition->next != NULL && aafUIDCmp( Transition->next->Class->ID, &AAFClassID_Filler ) )
	{
		flags |= AAFI_TRANS_FADE_OUT;
	}
	else if ( Transition->next != NULL && aafUIDCmp( Transition->next->Class->ID, &AAFClassID_Filler ) == 0 &&
						Transition->prev != NULL && aafUIDCmp( Transition->prev->Class->ID, &AAFClassID_Filler ) == 0 )
	{
		flags |= AAFI_TRANS_XFADE;
	}
	else
	{
		DUMP_OBJ_ERROR( aafi, Transition, &__td, "Could not guess if type is FadeIn, FadeOut or xFade" );
		return -1;
	}




	aafiTimelineItem *Item  = aafi_newTimelineItem( aafi, aafi->ctx.current_track, AAFI_TRANS );

	aafiTransition   *Trans = (aafiTransition*)&Item->data;



  Trans->len = *length;
	Trans->flags = flags;


	int missing_cutpt = 0;

	aafPosition_t *cut_point = aaf_get_propertyValue( Transition, PID_Transition_CutPoint );

	if ( cut_point == NULL ) /* req */
	{
		// DUMP_OBJ_WARNING( aafi, Transition, &__td, "Missing PID_Transition_CutPoint : Setting to Trans->len/2" );
		missing_cutpt = 1;
    Trans->cut_pt = Trans->len/2; // set default cutpoint to the middle of transition
	}
	else
	{
		Trans->cut_pt = *cut_point;
	}


	aafObject * OpGroup = aaf_get_propertyValue( Transition, PID_Transition_OperationGroup );

	if ( OpGroup != NULL ) /* req */
	{
		if ( missing_cutpt ) {
			DUMP_OBJ_WARNING( aafi, Transition, &__td, "Missing PID_Transition_CutPoint : Setting to Trans->len/2" );
		} else {
			DUMP_OBJ( aafi, Transition, &__td );
		}

		/*
		 * Don't handle parse_OperationGroup() return code, since it should
		 * always fallback to default in case of failure.
		 */

		aafi->ctx.current_transition = Trans;
		parse_OperationGroup( aafi, OpGroup, &__td );
		aafi->ctx.current_transition = NULL;
	}
	else
	{
		/* Setting fade to default */

		__td.eob = 1;

		if ( missing_cutpt ) {
			DUMP_OBJ_WARNING( aafi, Transition, &__td, "Missing PID_Transition_CutPoint AND PID_Transition_OperationGroup : Setting to Trans->len/2; Linear" );
		} else {
			DUMP_OBJ_WARNING( aafi, Transition, &__td, "Missing PID_Transition_OperationGroup : Setting to Linear interpolation" );
		}

		Trans->flags |= (AAFI_INTERPOL_LINEAR | AAFI_TRANS_SINGLE_CURVE);

		Trans->time_a  = calloc( 2, sizeof(aafRational_t) );
		Trans->value_a = calloc( 2, sizeof(aafRational_t) );

		Trans->time_a[0].numerator   = 0;
		Trans->time_a[0].denominator = 0;
		Trans->time_a[1].numerator   = 1;
		Trans->time_a[1].denominator = 1;

		if ( Trans->flags & AAFI_TRANS_FADE_IN ||
				 Trans->flags & AAFI_TRANS_XFADE )
		{
			Trans->value_a[0].numerator   = 0;
			Trans->value_a[0].denominator = 0;
			Trans->value_a[1].numerator   = 1;
			Trans->value_a[1].denominator = 1;
		}
		else if ( Trans->flags & AAFI_TRANS_FADE_OUT )
		{
			Trans->value_a[0].numerator   = 1;
			Trans->value_a[0].denominator = 1;
			Trans->value_a[1].numerator   = 0;
			Trans->value_a[1].denominator = 0;
		}
	}


	aafi->ctx.current_track->current_pos -= *length;


	return 0;
}



static int parse_NestedScope( AAF_Iface *aafi, aafObject *NestedScope, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 1);



	aafObject *Slot  = NULL;
	aafObject *Slots = aaf_get_propertyValue( NestedScope, PID_NestedScope_Slots );

	if ( Slots == NULL )
	{
		DUMP_OBJ_ERROR( aafi, NestedScope, &__td, "Missing PID_NestedScope_Slots" );
		return -1;
	}


	DUMP_OBJ( aafi, NestedScope, &__td );


	int i = 0;

	aaf_foreach_ObjectInSet( &Slot, Slots, NULL )
	{
		__td.ll[__td.lv] = (Slots->Header->_entryCount > 1) ? (Slots->Header->_entryCount - i++) : 0;//(MobSlot->next) ? 1 : 0;
		parse_Segment( aafi, Slot, &__td );
	}

	/* TODO should we take parse_Segment() return code into account ? */

	return 0;
}



int parse_Segment( AAF_Iface *aafi, aafObject *Segment, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 0);



	if ( aafUIDCmp( Segment->Class->ID, &AAFClassID_Sequence ) )
	{
		return parse_Sequence( aafi, Segment, &__td );
	}
	else if ( aafUIDCmp( Segment->Class->ID, &AAFClassID_SourceClip ) )
	{
		return parse_SourceClip( aafi, Segment, &__td );
	}
	else if ( aafUIDCmp( Segment->Class->ID, &AAFClassID_OperationGroup ) )
	{
		return parse_OperationGroup( aafi, Segment, &__td );
	}
	else if ( aafUIDCmp( Segment->Class->ID, &AAFClassID_Filler ) )
	{
		return parse_Filler( aafi, Segment, &__td );
	}
	else if ( aafUIDCmp( Segment->Class->ID, &AAFClassID_Selector ) )
	{
		return parse_Selector( aafi, Segment, &__td );
	}
	else if ( aafUIDCmp( Segment->Class->ID, &AAFClassID_NestedScope ) )
	{
		return parse_NestedScope( aafi, Segment, &__td );
	}
	else if ( aafUIDCmp( Segment->Class->ID, &AAFClassID_Timecode ) )
	{
		/*
		 *	TODO can contain sequence ? other Timecode SMPTE ..
		 */

		return parse_Timecode( aafi, Segment, &__td );
	}
	else if ( aafUIDCmp( Segment->Class->ID, &AAFClassID_DescriptiveMarker ) )
	{
		if ( resolve_AAF( aafi ) ) {
			resolve_parse_aafObject_DescriptiveMarker( aafi, Segment, &__td );
		}
		else {
			__td.lv++;
			DUMP_OBJ_NO_SUPPORT( aafi, Segment, &__td );
			return -1;
		}
	}
	else if ( aafUIDCmp( Segment->Class->ID, &AAFClassID_EssenceGroup ) )
	{
		/*
		 *	Should provide support for multiple essences representing the same
		 *	source material with different resolution, compression, codec, etc.
		 *
		 *	TODO To be tested with Avid and rendered effects.
		 */

		__td.lv++;
		DUMP_OBJ_NO_SUPPORT( aafi, Segment, &__td );
		return -1;
	}
	else
	{
		__td.lv++;
		DUMP_OBJ_NO_SUPPORT( aafi, Segment, &__td );
	}


	return 0;
}




static int parse_Filler( AAF_Iface *aafi, aafObject *Filler, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 1);

	__td.eob = 1;



	aafUID_t *DataDefinition = get_Component_DataDefinition( aafi, Filler );

	if ( DataDefinition == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, Filler, &__td, "Could not retrieve DataDefinition" );
		return -1;
	}




	if ( aafUIDCmp( Filler->Parent->Class->ID, &AAFClassID_TimelineMobSlot ) )
	{
		/*
		 *	Just an empty track, do nothing.
		 */
	}
	else
	if ( aafUIDCmp( Filler->Parent->Class->ID, &AAFClassID_Sequence ) ||
	     aafUIDCmp( Filler->Parent->Class->ID, &AAFClassID_Selector ) )
	{
		/*
		 *	This represents an empty space on the timeline, between two clips
		 *	which is Component::Length long.
		 */

		int64_t *length = (int64_t*)aaf_get_propertyValue( Filler, PID_Component_Length );

		if ( length == NULL ) /* probably req for Filler */
		{
			DUMP_OBJ_ERROR( aafi, Filler, &__td, "Missing PID_Component_Length" );
			return -1;
		}


		if ( aafUIDCmp( DataDefinition, &AAFDataDef_Sound ) ||
				 aafUIDCmp( DataDefinition, &AAFDataDef_LegacySound ) )
		{
			aafi->ctx.current_track->current_pos += *length;
		}
		else if ( aafUIDCmp( DataDefinition, &AAFDataDef_Picture ) ||
		          aafUIDCmp( DataDefinition, &AAFDataDef_LegacyPicture ) )
		{
			aafi->Video->Tracks->current_pos += *length;
		}

	}
	else
	{
		DUMP_OBJ_NO_SUPPORT( aafi, Filler, &__td );
		return -1;
	}


	DUMP_OBJ( aafi, Filler, &__td );


	return 0;
}




static int parse_Sequence( AAF_Iface *aafi, aafObject *Sequence, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 1);



	aafObject * Component  = NULL;
	aafObject * Components = aaf_get_propertyValue( Sequence, PID_Sequence_Components );

	if ( Components == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, Sequence, &__td, "Missing PID_Sequence_Components" );
		return -1;
	}


	DUMP_OBJ( aafi, Sequence, &__td );


	int i = 0;

	aaf_foreach_ObjectInSet( &Component, Components, NULL )
	{
		__td.ll[__td.lv] = (Components->Header->_entryCount > 1) ? (Components->Header->_entryCount - i++) : 0;//(MobSlot->next) ? 1 : 0;
		parse_Component( aafi, Component, &__td );
	}


	return 0;
}




static int parse_Timecode( AAF_Iface *aafi, aafObject *Timecode, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 1);

	__td.eob = 1;



	aafPosition_t *tc_start = (int64_t *)aaf_get_propertyValue( Timecode, PID_Timecode_Start );

	if ( tc_start == NULL )
	{
		DUMP_OBJ_ERROR( aafi, Timecode, &__td, "Missing PID_Timecode_Start" );
		return -1;
	}


	uint16_t *tc_fps   = (uint16_t*)aaf_get_propertyValue( Timecode, PID_Timecode_FPS   );

	if ( tc_fps == NULL )
	{
		DUMP_OBJ_ERROR( aafi, Timecode, &__td, "Missing PID_Timecode_FPS" );
		return -1;
	}


	uint8_t  *tc_drop  = (uint8_t *)aaf_get_propertyValue( Timecode, PID_Timecode_Drop  );

	if ( tc_drop == NULL )
	{
		DUMP_OBJ_ERROR( aafi, Timecode, &__td, "Missing PID_Timecode_Drop" );
		return -1;
	}


	/* TODO this should be retrieved directly from TimelineMobSlot */

	aafObject *ParentMobSlot = get_Object_Ancestor( aafi, Timecode, &AAFClassID_MobSlot );

	if ( ParentMobSlot == NULL )
	{
		DUMP_OBJ_ERROR( aafi, Timecode, &__td, "Could not retrieve parent MobSlot" );
		return -1;
	}

	aafRational_t *tc_edit_rate = (aafRational_t*)aaf_get_propertyValue( /*aafi->ctx.*/ParentMobSlot, PID_TimelineMobSlot_EditRate );

	if ( tc_edit_rate == NULL )
	{
		DUMP_OBJ_ERROR( aafi, Timecode, &__td, "Missing parent MobSlot PID_TimelineMobSlot_EditRate" );
		return -1;
	}


	/* TODO allocate in specific function */

	aafiTimecode *tc = calloc( sizeof(aafiTimecode), sizeof(unsigned char) );

	if ( tc == NULL )
	{
		DUMP_OBJ_ERROR( aafi, Timecode, &__td, "calloc() : %s", strerror( errno ) );
		return -1;
	}

	tc->start = *tc_start;
	tc->fps   = *tc_fps;
	tc->drop  = *tc_drop;
	tc->edit_rate = tc_edit_rate;

	aafi->Audio->tc = tc;
	aafi->Video->tc = tc; // TODO do it properly !


	DUMP_OBJ( aafi, Timecode, &__td );


	return 0;
}




static int parse_OperationGroup( AAF_Iface *aafi, aafObject *OpGroup, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 1);


	if ( !aaf_get_property( OpGroup, PID_OperationGroup_InputSegments ) &&
	     !aaf_get_property( OpGroup, PID_OperationGroup_Parameters ) )
	{
		__td.eob = 1;
	}


	aafObject *ParentMob = get_Object_Ancestor( aafi, OpGroup, &AAFClassID_Mob );

	if ( ParentMob == NULL )
	{
		DUMP_OBJ_ERROR( aafi, OpGroup, &__td, "Could not retrieve parent Mob" );
		return -1;
	}

	if ( ! aafUIDCmp( ParentMob->Class->ID, &AAFClassID_CompositionMob ) )
	{
		DUMP_OBJ_ERROR( aafi, OpGroup, &__td, "OperationGroup parser is currently implemented for AAFClassID_CompositionMob children only" );
		return -1;
	}


	aafUID_t *OperationIdentification = get_OperationGroup_OperationIdentification( aafi, OpGroup );



	/* *** PRINT OPERATIONDEFINITIONS *** */

	// aafObject * Parameters = aaf_get_propertyValue( OpGroup, PID_OperationGroup_Parameters );
	//
	// if ( Parameters )
	// {
	// 	aafObject * Param = NULL;
	//
	// 	aaf_foreach_ObjectInSet( &Param, Parameters, NULL )
	// 	{
	// 	 aafUID_t *ParamDef = aaf_get_propertyValue( Param, PID_Parameter_Definition );
	// 	 printf("     OpDef %ls     (%ls) | %ls\n", OperationDefToText(aafi->aafd, OperationIdentification), AUIDToText( ParamDef ), ParameterToText( aafi->aafd, ParamDef ) );
	// 	}
	// }





	// if ( aafUIDCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_MasterMob ) )
	// {
	//
	// 	/*
	// 	 *	TODO: This was seen in the spec, but never encountered in real world.
	// 	 */
	//
	// 	trace_obj( aafi, Segment, ANSI_COLOR_RED );
	// 	_error( aafi->ctx.options.verb, "MobSlot::Segment > OperationGroup Not implemented yet.\n" );
	// 	return -1;
	//
	// }



	int rc = 0;

	if ( aafUIDCmp( OpGroup->Parent->Class->ID, &AAFClassID_Transition ) )
	{

		aafiTransition *Trans = aafi->ctx.current_transition;


		if ( aafUIDCmp( OperationIdentification, &AAFOperationDef_MonoAudioDissolve ) )
		{
			/*
			 *	Mono Audio Dissolve (Fade, Cross Fade)
			 *
			 *	The same parameter (curve/level) is applied to the outgoing fade on first
			 *	clip (if any) and to the incoming fade on second clip (if any).
			 */

			// __td.eob = 1;



			Trans->flags |= AAFI_TRANS_SINGLE_CURVE;


			int set_default = 0;


			aafObject * Param = NULL;
			aafObject * Parameters = aaf_get_propertyValue( OpGroup, PID_OperationGroup_Parameters ); /* opt */

			if ( Parameters )
			{
				/* Retrieve AAFParameterDef_Level parameter */

				aaf_foreach_ObjectInSet( &Param, Parameters, NULL )
				{
					aafUID_t *ParamDef = aaf_get_propertyValue( Param, PID_Parameter_Definition );

					if ( aafUIDCmp( ParamDef, &AAFParameterDef_Level ) )
						break;
				}
			}
			else
			{
				// DUMP_OBJ_WARNING( aafi, OpGroup, &__td, "Missing PID_OperationGroup_Parameters: Falling back to Linear" );
				set_default = 1;
			}

			if ( Param )
			{
				DUMP_OBJ( aafi, OpGroup, &__td );

				if ( aaf_get_propertyValue( OpGroup, PID_OperationGroup_InputSegments ) ) {
					__td.ll[__td.lv] = 2;
				}

				if ( parse_Parameter( aafi, Param, &__td ) < 0 )
				{
					set_default = 1;
				}

				__td.ll[__td.lv] = 0;
			}
			else
			{
				/*
				 * Do not notify exception since this case is standard compliant :
				 *
				 * ParameterDef_Level (optional; default is a VaryingValue object with
				 * two control points: Value 0 at time 0, and value 1 at time 1)
				 */

				__td.eob = 1;
				DUMP_OBJ( aafi, OpGroup, &__td );

				// DUMP_OBJ_WARNING( aafi, OpGroup, &__td, "Missing Parameter AAFParameterDef_Level: Setting to Linear" );

				set_default = 1;
			}



			if ( set_default )
			{
				/*
				 *	ParameterDef_Level (optional; default is a VaryingValue object
				 *	with two control points: Value 0 at time 0, and value 1 at time 1)
				 *
				 *	This is also a fallback in case of parse_Parameter() failure.
				 */


				Trans->flags |= AAFI_INTERPOL_LINEAR;

				Trans->time_a  = calloc( 2, sizeof(aafRational_t) );
				Trans->value_a = calloc( 2, sizeof(aafRational_t) );

				Trans->time_a[0].numerator   = 0;
				Trans->time_a[0].denominator = 0;
				Trans->time_a[1].numerator   = 1;
				Trans->time_a[1].denominator = 1;

				if ( Trans->flags & AAFI_TRANS_FADE_IN ||
				     Trans->flags & AAFI_TRANS_XFADE )
				{
					Trans->value_a[0].numerator   = 0;
					Trans->value_a[0].denominator = 0;
					Trans->value_a[1].numerator   = 1;
					Trans->value_a[1].denominator = 1;
				}
				else if ( Trans->flags & AAFI_TRANS_FADE_OUT )
				{
					Trans->value_a[0].numerator   = 1;
					Trans->value_a[0].denominator = 1;
					Trans->value_a[1].numerator   = 0;
					Trans->value_a[1].denominator = 0;
				}
			}

		}
		else if ( aafUIDCmp( OperationIdentification, &AAFOperationDef_TwoParameterMonoAudioDissolve ) )
		{
			DUMP_OBJ_NO_SUPPORT( aafi, OpGroup, &__td );
			/* Two distinct parameters are used for the outgoing and incoming fades. */
		}
		else if ( aafUIDCmp( OperationIdentification, &AAFOperationDef_StereoAudioDissolve ) )
		{
			DUMP_OBJ_NO_SUPPORT( aafi, OpGroup, &__td );
			/* 	TODO Unknown usage and implementation */
		}
		else
		{
			DUMP_OBJ_NO_SUPPORT( aafi, OpGroup, &__td );
		}

	}
	else if ( aafUIDCmp( OperationIdentification, &AAFOperationDef_AudioChannelCombiner ) )
	{

		/**************************************************************************************/

		aafi->ctx.current_track_is_multichannel = 1;
		aafi->ctx.current_multichannel_track_channel = 0;


		DUMP_OBJ( aafi, OpGroup, &__td );


		aafObject *InputSegment  = NULL;
		aafObject *InputSegments = aaf_get_propertyValue( OpGroup, PID_OperationGroup_InputSegments );


		// int i = InputSegments->Header->_entryCount;
		__td.ll[__td.lv] = InputSegments->Header->_entryCount;

		aaf_foreach_ObjectInSet( &InputSegment, InputSegments, NULL )
		{
			// __td.ll[__td.lv] = __td.ll[__td.lv] - i++;//(MobSlot->next) ? 1 : 0;
			parse_Segment( aafi, InputSegment, &__td );
			aafi->ctx.current_multichannel_track_channel++;

			__td.ll[__td.lv]--;
		}


		/*
		 *	Sets the track format.
		 */
		aafiAudioTrack *current_track = (aafiAudioTrack*)aafi->ctx.current_track;

		if ( aafi->ctx.current_multichannel_track_channel == 2 )
		{
			current_track->format = AAFI_TRACK_FORMAT_STEREO;
		}
		else if ( aafi->ctx.current_multichannel_track_channel == 6 )
		{
			current_track->format = AAFI_TRACK_FORMAT_5_1;
		}
		else if ( aafi->ctx.current_multichannel_track_channel == 8 )
		{
			current_track->format = AAFI_TRACK_FORMAT_7_1;
		}
		else
		{
			DUMP_OBJ_ERROR( aafi, OpGroup, &__td, "Unknown track format (%u)", aafi->ctx.current_multichannel_track_channel );
			return -1;
		}


		/*
		 *	Update the current position.
		 */

		aafi->ctx.current_track->current_pos += aafi->ctx.current_multichannel_track_clip_length;


		/*
		 *	Reset multichannel track context.
		 */

		aafi->ctx.current_multichannel_track_channel = 0;
		aafi->ctx.current_track_is_multichannel = 0;


		/**************************************************************************************/

		// return;
	}
	else if ( aafUIDCmp( OperationIdentification, &AAFOperationDef_MonoAudioGain ) )
	{

		aafObject * Param = NULL;
		aafObject * Parameters = aaf_get_propertyValue( OpGroup, PID_OperationGroup_Parameters );

		if ( Parameters == NULL )
		{
			DUMP_OBJ_ERROR( aafi, OpGroup, &__td, "Missing PID_OperationGroup_Parameters" );
			rc = -1; goto end; // we still have to parse segments
		}


		/* Retrieve AAFParameterDef_Amplitude parameter */

		aaf_foreach_ObjectInSet( &Param, Parameters, NULL )
		{
			aafUID_t *ParamDef = aaf_get_propertyValue( Param, PID_Parameter_Definition );

			if ( aafUIDCmp( ParamDef, &AAFParameterDef_Amplitude ) )
				break;
		}


		if ( Param == NULL )
		{
			DUMP_OBJ_ERROR( aafi, OpGroup, &__td, "Missing Parameter ParameterDef_Amplitude" );
			rc = -1; goto end; // we still have to parse segments
		}

		DUMP_OBJ( aafi, OpGroup, &__td );

		__td.ll[__td.lv] = 2;

		rc = parse_Parameter( aafi, Param, &__td );

		// if ( rc == 0 ) {
		// 	DUMP_OBJ( aafi, OpGroup, &__td );
		// }
		// else {
		// 	DUMP_OBJ_ERROR( aafi, OpGroup, &__td, "Failed parsing parameter" );
		// }

	}
	else if ( aafUIDCmp( OperationIdentification, &AAFOperationDef_StereoAudioGain ) )
	{
		DUMP_OBJ_NO_SUPPORT( aafi, OpGroup, &__td );
		/* TODO Unknown usage and implementation */
	}
	else if ( aafUIDCmp( OperationIdentification, &AAFOperationDef_MonoAudioPan ) )
	{
		/* TODO Should Only be Track-based (first Segment of TimelineMobSlot.) */

		/*
		 *	We have to loop because of custom Parameters.
		 *	Seen in AVID Media Composer AAFs (test.aaf). TODO ParamDef PanVol_IsTrimGainEffect ?
		 */

		 aafObject * Param = NULL;
		 aafObject * Parameters = aaf_get_propertyValue( OpGroup, PID_OperationGroup_Parameters );

		 if ( Parameters == NULL )
		 {
			DUMP_OBJ_ERROR( aafi, OpGroup, &__td, "Missing PID_OperationGroup_Parameters" );
			rc = -1; goto end; // we still have to parse segments
		 }


		 /* Retrieve AAFParameterDef_Pan parameter */

		 aaf_foreach_ObjectInSet( &Param, Parameters, NULL )
		 {
		 	aafUID_t *ParamDef = aaf_get_propertyValue( Param, PID_Parameter_Definition );

		 	if ( aafUIDCmp( ParamDef, &AAFParameterDef_Pan ) )
		 		break;
		 }


		 if ( Param == NULL )
		 {
			DUMP_OBJ_ERROR( aafi, OpGroup, &__td, "Missing Parameter ParameterDef_Amplitude" );
			rc = -1; goto end; // we still have to parse segments
		 }


		 DUMP_OBJ( aafi, OpGroup, &__td );

		 __td.ll[__td.lv] = 2;

		 rc = parse_Parameter( aafi, Param, &__td );

		 // if ( rc == 0 ) {
			//  DUMP_OBJ( aafi, OpGroup, &__td );
		 // }
		 // else {
			//  DUMP_OBJ_ERROR( aafi, OpGroup, &__td, "Failed parsing parameter" );
		 // }
	}
	else if ( aafUIDCmp( OperationIdentification, &AAFOperationDef_MonoAudioMixdown ) )
	{
		DUMP_OBJ_NO_SUPPORT( aafi, OpGroup, &__td );
		/* TODO Unknown usage and implementation */
	}
	else
	{
		DUMP_OBJ_NO_SUPPORT( aafi, OpGroup, &__td );
	}



end:

	/*
	 *	Parses Segments in the OperationGroup::InputSegments, only if
	 *	OperationGroup is not a Transition as a Transition has no InputSegments,
	 *	and not an AudioChannelCombiner as they were already parsed.
	 */

	if ( aafUIDCmp( OpGroup->Parent->Class->ID, &AAFClassID_Transition ) == 0 &&
	     aafUIDCmp( OperationIdentification, &AAFOperationDef_AudioChannelCombiner ) == 0 )
	{
		aafObject *InputSegment  = NULL;
		aafObject *InputSegments = aaf_get_propertyValue( OpGroup, PID_OperationGroup_InputSegments );

		int i = 0;
		__td.ll[__td.lv] = (InputSegments) ? InputSegments->Header->_entryCount : 0;

		aaf_foreach_ObjectInSet( &InputSegment, InputSegments, NULL )
		{
			__td.ll[__td.lv] = __td.ll[__td.lv] - i++;
			parse_Segment( aafi, InputSegment, &__td );
		}
	}


	/* End of current OperationGroup context. */

	aafObject *Obj = OpGroup;
	for (; Obj != NULL && aafUIDCmp( Obj->Class->ID, &AAFClassID_ContentStorage ) == 0; Obj = Obj->Parent )
		if ( !aafUIDCmp( Obj->Class->ID, &AAFClassID_OperationGroup ) )
			break;

	if ( aafUIDCmp( OperationIdentification, &AAFOperationDef_MonoAudioGain ) )
	{
		if ( !aafUIDCmp( Obj->Class->ID, &AAFClassID_TimelineMobSlot ) )
		{
			if ( aafi->ctx.clips_using_gain == 0 )
			{
				aafi_freeAudioGain( aafi->ctx.current_clip_gain );
			}

			if ( aafi->ctx.clips_using_automation == 0 )
			{
				aafi_freeAudioGain( aafi->ctx.current_clip_automation );
			}

			/* Clip-based Gain */
			aafi->ctx.current_clip_is_muted = 0;
			aafi->ctx.current_clip_gain = NULL;
			aafi->ctx.current_clip_automation = NULL;
			aafi->ctx.clips_using_gain = 0;
			aafi->ctx.clips_using_automation = 0;
		}

		// free( aafi->ctx.current_track->gain );
		// aafi->ctx.current_track->gain = NULL;
	}

	return rc;
}




static int parse_SourceClip( AAF_Iface *aafi, aafObject *SourceClip, td *__ptd )
{

	struct trace_dump __td;
	__td_set(__td, __ptd, 1);

	__td.hc = 1; // link to MasterMob, SourceMob



	aafUID_t *DataDefinition = get_Component_DataDefinition( aafi, SourceClip );

	if ( DataDefinition == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Could not retrieve DataDefinition" );
		return -1;
	}


	aafObject *ParentMob = get_Object_Ancestor( aafi, SourceClip, &AAFClassID_Mob );

	if ( ParentMob == NULL )
	{
		DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Could not retrieve parent Mob" );
		return -1;
	}

	aafMobID_t *parentMobID = (aafMobID_t*)aaf_get_propertyValue( ParentMob, PID_Mob_MobID );

	if ( parentMobID == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Missing parent Mob PID_Mob_MobID" );
		return -1;
	}



	aafMobID_t *sourceID = (aafMobID_t*)aaf_get_propertyValue( SourceClip, PID_SourceReference_SourceID );

	if ( sourceID == NULL ) /* opt */
	{
		/* NOTE: PID_SourceReference_SourceID is optionnal, there might be none. */
		// DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Missing PID_SourceReference_SourceID" );
		// return -1;
	}

	uint32_t *SourceMobSlotID = (uint32_t*)aaf_get_propertyValue( SourceClip, PID_SourceReference_SourceMobSlotID );

	if ( SourceMobSlotID == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Missing PID_SourceReference_SourceMobSlotID" );
		return -1;
	}

	/*
	 *	TODO: handle SourceReference::MonoSourceSlotIDs and associated conditional rules.
	 *	(Multi-channels)
	 */


	aafObject *refMob = NULL;
	aafObject *refMobSlot = NULL;

	if ( sourceID == NULL )
	{
		/*
		 *	p.49 : To create a SourceReference that refers to a MobSlot within
		 *	the same Mob as the SourceReference, omit the SourceID property.
		 *
		 *	[SourceID] Identifies the Mob being referenced. If the property has a
		 *	value 0, it means that the Mob owning the SourceReference describes
		 *	the original source.
		 *
		 *	TODO: in that case, is MobSlots NULL ?
		 */

		// sourceID = parentMobID;
		// refMob = ParentMob;
	}
	else
	{
		refMob = aaf_get_MobByID( aafi->aafd->Mobs, sourceID );

		if ( refMob == NULL )
		{
			DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Could not retrieve target Mob by ID : %ls", MobIDToText(sourceID) );
			return -1;
		}

		aafObject *refMobSlots = aaf_get_propertyValue( refMob, PID_Mob_Slots );

		if ( refMobSlots == NULL )
		{
			DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Missing target Mob PID_Mob_Slots" );
			return -1;
		}

		refMobSlot = aaf_get_MobSlotBySlotID( refMobSlots, *SourceMobSlotID );

		if ( refMobSlot == NULL )
		{
			/* TODO check if there is a workaround :
			 *  AAFInfo --aaf-clips '/home/agfline/Developpement/libaaf_testfiles/ADP/ADP_STTRACK_CLIPGAIN_TRACKGAIN_XFADE_NOOPTONEXPORT.aaf'
			 */
			DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Could not retrieve target MobSlot ID : %u", *SourceMobSlotID );
			return -1;
		}
	}





	/*** Clip ***/

	if ( aafUIDCmp( ParentMob->Class->ID, &AAFClassID_CompositionMob ) )
	{

		// DUMP_OBJ( aafi, SourceClip, &__td );


		int64_t *length = (int64_t*)aaf_get_propertyValue( SourceClip, PID_Component_Length );

		if ( length == NULL )
		{
			DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Missing PID_Component_Length" );
			return -1;
		}


		int64_t *startTime = (int64_t*)aaf_get_propertyValue( SourceClip, PID_SourceClip_StartTime );

		if ( startTime == NULL )
		{
			DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Missing PID_SourceClip_StartTime" );
			return -1;
		}




		struct aafiContext ctxBackup;


		aafUID_t *CurrentUsageCode = (aafUID_t*)aaf_get_propertyValue( ParentMob, PID_Mob_UsageCode );

		if ( CurrentUsageCode == NULL )
		{
			/* NOTE: PID_Mob_UsageCode is optionnal, there might be none. */
			// DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Missing PID_Mob_UsageCode" );
			// return -1;
		}

		// if ( aafUIDCmp( aafi->aafd->Header.OperationalPattern, &AAFOPDef_EditProtocol ) )
		{

			// if ( (CurrentUsageCode && aafUIDCmp( CurrentUsageCode, &AAFUsage_SubClip )) || CurrentUsageCode == NULL )
			// {
			// 	trace_obj( aafi, SourceClip, ANSI_COLOR_YELLOW );
			// }
			// else
			// {
			// 	trace_obj( aafi, SourceClip, ANSI_COLOR_MAGENTA );
			// }



			/*
			 *	If SourceClip points to a CompositionMob instead of a MasterMob, we
			 *	are at the begining (or inside) a derivation chain.
			 */

			if ( aafUIDCmp( refMob->Class->ID, &AAFClassID_CompositionMob ) )
			{

				// printf("\n\n\nREF TO SUBCLIP\n\n\n" );
				//
				// printf( "SourceClip::SourceID        : %ls\n", MobIDToText( sourceID ) );
				// printf( "CurrentMob::MobID           : %ls\n", MobIDToText( parentMobID ) );
				// printf( "SourceClip::SourceMobSlotID : %i\n", *SourceMobSlotID );
				// printf( "UsageCode                   : %ls\n", UsageCodeToText( UsageCode ) );

				if ( refMobSlot == NULL )
				{
					/* TODO isn't it already checked above ? */
					DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Missing target MobSlot" );
					return -1;
				}

				DUMP_OBJ( aafi, SourceClip, &__td );



				/* Only to print trace */
				__td.lv++;
				DUMP_OBJ( aafi, refMob, &__td );
				// __td.lv++;


				memcpy( &ctxBackup, &(aafi->ctx), sizeof(struct aafiContext) );

				RESET_CONTEXT( aafi->ctx );

				aafi->ctx.current_track = ctxBackup.current_track;
				aafi->ctx.is_inside_derivation_chain = 1;

				parse_MobSlot( aafi, refMobSlot, &__td );

				void *new_clip = aafi->ctx.current_clip;

				memcpy( &(aafi->ctx), &ctxBackup, sizeof(struct aafiContext) );


				if ( aafUIDCmp( DataDefinition, &AAFDataDef_Sound ) ||
						 aafUIDCmp( DataDefinition, &AAFDataDef_LegacySound ) )
				{

					aafi->ctx.current_clip = (aafiAudioClip*)new_clip;

					if ( new_clip && ( CurrentUsageCode && aafUIDCmp( CurrentUsageCode, &AAFUsage_TopLevel ) ) )
					{
						/*
						 *	All derivation chain calls ended.
						 *
						 *	We came back at level zero of parse_SourceClip() nested calls, so
						 *	the clip and its source was added, we only have to set its length,
						 *	offset and gain with correct values.
						 *
						 *  TODO: aafi->current_clip pointer to new_clip instead ?
						 */

						((aafiAudioClip*)new_clip)->len = *length;
						((aafiAudioClip*)new_clip)->essence_offset = *startTime;
						((aafiAudioClip*)new_clip)->gain = aafi->ctx.current_clip_gain;
						((aafiAudioClip*)new_clip)->automation = aafi->ctx.current_clip_automation;
						((aafiAudioClip*)new_clip)->mute = aafi->ctx.current_clip_is_muted;
						aafi->ctx.clips_using_gain++;
						aafi->ctx.clips_using_automation++;

						aafi->ctx.current_track->current_pos += ((aafiAudioClip*)new_clip)->len;
					}
				}
				else if ( aafUIDCmp( DataDefinition, &AAFDataDef_Picture ) ||
				          aafUIDCmp( DataDefinition, &AAFDataDef_LegacyPicture ) )
				{
					if ( new_clip && ( CurrentUsageCode && aafUIDCmp( CurrentUsageCode, &AAFUsage_TopLevel ) ) )
					{
						/*
						 *	All derivation chain calls ended.
						 *
						 *	We came back at level zero of parse_SourceClip() nested calls, so
						 *	the clip and its source was added, we only have to set its length,
						 *	offset and gain with correct values.
						 */

						((aafiVideoClip*)new_clip)->len = *length;
						((aafiVideoClip*)new_clip)->essence_offset = *startTime;

						aafi->Video->Tracks->current_pos += ((aafiVideoClip*)new_clip)->len;
					}
				}

				return 0;

			}
			else if ( aafUIDCmp( refMob->Class->ID, &AAFClassID_MasterMob ) )
			{
				/*
				 * We are inside the derivation chain and we reached the SourceClip
				 * pointing to MasterMob (the audio essence).
				 *
				 * Thus, we can add the clip and parse the audio essence normaly.
				 */
			}
		}



		if ( aafUIDCmp( DataDefinition, &AAFDataDef_Sound ) ||
				 aafUIDCmp( DataDefinition, &AAFDataDef_LegacySound ) )
		{

			// if ( *length == 1 )
			// {
			// 	/*
			// 	 *	If length equals 1 EditUnit, the clip is probably a padding for "Media Composer Compatibility".
			// 	 *	Therefore, we don't need it.
			// 	 *
			// 	 *	TODO BUT this could also be some rendered fade.. we should find a way to distinguish between the two.
			// 	 */
			//
			// 	// aaf_dump_ObjectProperties( aafi->aafd, SourceClip );
			//
			// 	_warning( aafi->ctx.options.verb, "Got a 1 EU length clip, probably some NLE compatibility padding : Skipping.\n" );
			//
			//
			//
		 	// 	if ( aafi->ctx.current_track_is_multichannel == 0 )
		 	// 	{
		 	// 		aafi->ctx.current_pos += *length;
		 	// 	}
		 	// 	else
		 	// 	{
		 	// 		aafi->ctx.current_multichannel_track_clip_length = *length;
		 	// 	}
			//
			// 	return -1;
			// }





			/* Add the new clip */

			aafiTimelineItem *item = aafi_newTimelineItem( aafi, aafi->ctx.current_track, AAFI_AUDIO_CLIP );

			aafiAudioClip    *audioClip = (aafiAudioClip*)&item->data;

			aafi->ctx.clips_using_gain++;
			aafi->ctx.clips_using_automation++;
			audioClip->gain = aafi->ctx.current_clip_gain;
			audioClip->automation = aafi->ctx.current_clip_automation;
			audioClip->mute = aafi->ctx.current_clip_is_muted;
			audioClip->pos  = aafi->ctx.current_track->current_pos;
			audioClip->len  = *length;

			audioClip->essence_offset = *startTime;


			aafi->ctx.current_clip = audioClip;



			/*
			 *	p.49 : To create a SourceReference that refers to a MobSlot within
			 *	the same Mob as the SourceReference, omit the SourceID property.
			 *
			 *	NOTE: This should not happen here because The "CompositionMob > SourceClip::SourceID"
			 *	should always point to the corresponding "MasterMob", that is a different Mob.
			 */

			audioClip->masterMobID = sourceID;

			// if ( audioClip->masterMobID == NULL )
			// {
			// 	audioClip->masterMobID = aaf_get_propertyValue( ParentMob, PID_Mob_MobID );
			// 	_warning( aafi->ctx.options.verb, "Missing SourceReference::SourceID, retrieving from parent Mob.\n" );
			// }




			if ( aafi->ctx.current_track_is_multichannel == 0 )
			{
				if ( /*!aafUIDCmp( aafi->aafd->Header.OperationalPattern, &AAFOPDef_EditProtocol ) ||*/
				     aafUIDCmp( CurrentUsageCode, &AAFUsage_TopLevel ) )
				{
					/*
					 *	NOTE for AAFOPDef_EditProtocol only :
					 *
					 *	If SourceClip belongs to a TopLevel Mob, we can update position.
					 *	Otherwise, it means we are inside a derivation chain ( ie: TopLevel:CompositionMob -> SourceClip -> SubLevel:CompositionMob -> SourceClip )
					 *	and the clip length is not the good one. In that case, position is updated above.
					 */

					aafi->ctx.current_track->current_pos += audioClip->len;
				}
			}
			else
			{
				aafi->ctx.current_multichannel_track_clip_length = audioClip->len;
			}




			if ( aafUIDCmp( refMob->Class->ID, &AAFClassID_MasterMob ) )
			{

				if ( refMobSlot == NULL )
				{
					/* TODO isn't it already checked above ? */
					DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Missing target MobSlot" );
					return -1;
				}

				DUMP_OBJ( aafi, SourceClip, &__td );


				/* Only to print trace */
				__td.lv++;
				DUMP_OBJ( aafi, refMob, &__td );


				memcpy( &ctxBackup, &(aafi->ctx), sizeof(struct aafiContext) );

				RESET_CONTEXT( aafi->ctx );

				aafi->ctx.current_track = ctxBackup.current_track;
				aafi->ctx.current_clip = audioClip;

				parse_MobSlot( aafi, refMobSlot, &__td );

				memcpy( &(aafi->ctx), &ctxBackup, sizeof(struct aafiContext) );

			}

		}
		else if ( aafUIDCmp( DataDefinition, &AAFDataDef_Picture ) ||
		          aafUIDCmp( DataDefinition, &AAFDataDef_LegacyPicture ) )
		{

			if ( aafi->Video->Tracks->Items )
			{
				DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Current implementation supports only one video clip" );
				return -1;
			}

			/* Add the new clip */

			aafiTimelineItem *item = aafi_newTimelineItem( aafi, aafi->Video->Tracks, AAFI_VIDEO_CLIP );

			aafiVideoClip    *videoClip = (aafiVideoClip*)&item->data;

			videoClip->pos  = aafi->Video->Tracks->current_pos;
			videoClip->len  = *length;

			videoClip->essence_offset = *startTime;


			/*
			 *	p.49 : To create a SourceReference that refers to a MobSlot within
			 *	the same Mob as the SourceReference, omit the SourceID property.
			 *
			 *	NOTE: This should not happen here because The "CompositionMob > SourceClip::SourceID"
			 *	should always point to the corresponding "MasterMob", that is a different Mob.
			 */

			videoClip->masterMobID = sourceID;

			// if ( videoClip->masterMobID == NULL )
			// {
			// 	videoClip->masterMobID = aaf_get_propertyValue( ParentMob, PID_Mob_MobID );
			// 	_warning( aafi->ctx.options.verb, "Missing SourceReference::SourceID, retrieving from parent Mob.\n" );
			// }





			if ( !aafUIDCmp( aafi->aafd->Header.OperationalPattern, &AAFOPDef_EditProtocol ) ||
			     ( CurrentUsageCode && aafUIDCmp( CurrentUsageCode, &AAFUsage_TopLevel ) ) )
			{
				/*
				 *	NOTE for AAFOPDef_EditProtocol only :
				 *
				 *	If SourceClip belongs to a TopLevel Mob, we can update position.
				 *	Otherwise, it means we are inside a derivation chain ( ie: TopLevelCompositionMob -> SourceClip -> SubLevel:CompositionMob -> SourceClip )
				 *	and the clip length is not the good one. In that case, position is updated above.
				 */

				aafi->Video->Tracks->current_pos += videoClip->len;
			}


			aafi->ctx.current_video_clip = videoClip;


			if ( aafUIDCmp( refMob->Class->ID, &AAFClassID_MasterMob ) )
			{

				if ( refMobSlot == NULL )
				{
					/* TODO isn't it already checked above ? */
					DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Missing target MobSlot" );
					return -1;
				}


				DUMP_OBJ( aafi, SourceClip, &__td );


				/* Only to print trace */
				__td.lv++;
				DUMP_OBJ( aafi, refMob, &__td );


				// memcpy( &ctxBackup, &(aafi->ctx), sizeof(struct aafiContext) );
				//
				// RESET_CONTEXT( aafi->ctx );

				parse_MobSlot( aafi, refMobSlot, &__td );

				// memcpy( &(aafi->ctx), &ctxBackup, sizeof(struct aafiContext) );

			}

		}
	}

	/*** Essence ***/

	else if ( aafUIDCmp( ParentMob->Class->ID, &AAFClassID_MasterMob ) )
	{

		// /* Only to print trace */
		// __td.lv--;
		// parse_MasterMob( aafi, ParentMob, &__td );
		// __td.lv += 2;


		aafMobID_t *masterMobID = aaf_get_propertyValue( ParentMob, PID_Mob_MobID );

		if ( masterMobID == NULL )
		{
			DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Could not retrieve parent Mob PID_Mob_MobID" );
			return -1;
		}

		aafObject *ParentMobSlot = get_Object_Ancestor( aafi, SourceClip, &AAFClassID_MobSlot );

		if ( ParentMobSlot == NULL )
		{
			DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Could not retrieve parent MobSlot" );
			return -1;
		}

		uint32_t *masterMobSlotID = aaf_get_propertyValue( ParentMobSlot, PID_MobSlot_SlotID );



		if ( aafUIDCmp( DataDefinition, &AAFDataDef_Sound ) ||
				 aafUIDCmp( DataDefinition, &AAFDataDef_LegacySound ) )
		{

			/* Check if this Essence has already been retrieved */

			// int slotID = MobSlot->Entry->_localKey;


			// aafObject *Obj = aaf_get_MobByID( aafi->aafd, SourceID );
			// printf("\n\n\n\n");
			// printf("SourceMobID : %ls\n", MobIDToText(SourceID) );
			// printf("MasterMobID : %ls\n", MobIDToText(mobID) );
			// printf("\n\n\n\n");

			if ( !aafi->ctx.current_clip )
			{
				DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "aafi->ctx.current_clip not set" );
				return -1;
			}



			aafiAudioEssence *audioEssence = NULL;

			foreachEssence( audioEssence, aafi->Audio->Essences )
			{
				if ( aafMobIDCmp( audioEssence->sourceMobID, sourceID ) && audioEssence->sourceMobSlotID == (unsigned)*SourceMobSlotID )
				{
					/* Essence already retrieved */
					aafi->ctx.current_clip->Essence = audioEssence;
					__td.eob = 1;
					DUMP_OBJ_INFO( aafi, SourceClip, &__td, "Essence already parsed: Linking with %ls", audioEssence->file_name );
					return 0;
				}
			}


			/* new Essence, carry on. */


			audioEssence = aafi_newAudioEssence( aafi );

			aafi->ctx.current_essence = audioEssence;

			audioEssence->masterMobSlotID = *masterMobSlotID;
			audioEssence->masterMobID = masterMobID;


			audioEssence->file_name = aaf_get_propertyValueWstr( ParentMob, PID_Mob_Name );


			/*
			 *	p.49 : To create a SourceReference that refers to a MobSlot within
			 *	the same Mob as the SourceReference, omit the SourceID property.
			 */

			audioEssence->sourceMobSlotID = *SourceMobSlotID;
			audioEssence->sourceMobID = sourceID;

			// if ( audioEssence->sourceMobID == NULL )
			// {
			// 	audioEssence->sourceMobID = aaf_get_propertyValue( ParentMob, PID_Mob_MobID );
			// 	_warning( aafi->ctx.options.verb, "Could not retrieve SourceReference::SourceID, retrieving from parent Mob.\n" );
			// }


			DUMP_OBJ( aafi, SourceClip, &__td );




			aafObject *SourceMob = aaf_get_MobByID( aafi->aafd->Mobs, audioEssence->sourceMobID );

			if ( SourceMob == NULL )
			{
				DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Could not retrieve SourceMob by ID : %ls", MobIDToText(audioEssence->sourceMobID) );
				return -1;
			}

			audioEssence->SourceMob = SourceMob;



			aafObject *EssenceData = get_EssenceData_By_MobID( aafi, audioEssence->sourceMobID );

			if ( EssenceData )
				__td.ll[__td.lv] = 2;


			parse_SourceMob( aafi, SourceMob, &__td );

			__td.ll[__td.lv] = 0;



			if ( EssenceData == NULL )
			{
				/*
				 *	It means essence is not embedded.
				 */

				// return -1;
			}
			else
			{
				parse_EssenceData( aafi, EssenceData, &__td );
			}



			audioEssence->unique_file_name = build_unique_audiofilename( aafi, audioEssence );


			aafi->ctx.current_clip->Essence = audioEssence;


			// trace_obj( aafi, SourceClip, ANSI_COLOR_MAGENTA );


		}
		else if ( aafUIDCmp( DataDefinition, &AAFDataDef_Picture ) ||
		          aafUIDCmp( DataDefinition, &AAFDataDef_LegacyPicture ) )
		{
			/* Check if this Essence has already been retrieved */

			// int slotID = MobSlot->Entry->_localKey;


			// aafObject *Obj = aaf_get_MobByID( aafi->aafd, sourceID );
			// printf("\n\n\n\n");
			// printf("SourceMobID : %ls\n", MobIDToText(sourceID) );
			// printf("MasterMobID : %ls\n", MobIDToText(mobID) );
			// printf("\n\n\n\n");


			if ( !aafi->ctx.current_video_clip )
			{
				DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "aafi->ctx.current_video_clip not set" );
				return -1;
			}


			aafiVideoEssence *videoEssence = NULL;

			foreachEssence( videoEssence, aafi->Video->Essences )
			{
				if ( aafMobIDCmp( videoEssence->sourceMobID, sourceID ) && videoEssence->sourceMobSlotID == (unsigned)*SourceMobSlotID )
				{
					/* Essence already retrieved */
					aafi->ctx.current_video_clip->Essence = videoEssence;
					return 0;
				}
			}


			/* new Essence, carry on. */


			videoEssence = aafi_newVideoEssence( aafi );

			aafi->ctx.current_video_clip->Essence = videoEssence;


			videoEssence->masterMobSlotID = *masterMobSlotID;
			videoEssence->masterMobID = masterMobID;


			videoEssence->file_name = aaf_get_propertyValueWstr( ParentMob, PID_Mob_Name );


			/*
			 *	p.49 : To create a SourceReference that refers to a MobSlot within
			 *	the same Mob as the SourceReference, omit the SourceID property.
			 */

			videoEssence->sourceMobSlotID = *SourceMobSlotID;
			videoEssence->sourceMobID = sourceID;

			// if ( audioEssence->sourceMobID == NULL )
			// {
			// 	audioEssence->sourceMobID = aaf_get_propertyValue( ParentMob, PID_Mob_MobID );
			// 	_warning( aafi->ctx.options.verb, "Could not retrieve SourceReference::SourceID, retrieving from parent Mob.\n" );
			// }



			aafi->ctx.current_video_essence = videoEssence;


			DUMP_OBJ( aafi, SourceClip, &__td );



			aafObject *SourceMob = aaf_get_MobByID( aafi->aafd->Mobs, videoEssence->sourceMobID );

			if ( SourceMob == NULL )
			{
				DUMP_OBJ_ERROR( aafi, SourceClip, &__td, "Could not retrieve SourceMob by ID : %ls", MobIDToText(videoEssence->sourceMobID) );
				return -1;
			}

			videoEssence->SourceMob = SourceMob;

			parse_SourceMob( aafi, SourceMob, &__td );




			aafObject *EssenceData = get_EssenceData_By_MobID( aafi, videoEssence->sourceMobID );

			if ( EssenceData == NULL )
			{
				/*
				 *	It means essence is not embedded.
				 */

				// return -1;
			}
			else
			{
				parse_EssenceData( aafi, EssenceData, &__td );
			}



			videoEssence->unique_file_name = build_unique_videofilename( aafi, videoEssence );




			// trace_obj( aafi, SourceClip, ANSI_COLOR_MAGENTA );






			// printf( "\n\n\n\nMaster MOB SOURCE CLIP\n\n\n\n" );
			//
			// aafiVideoEssence *videoEssence = aafi_newVideoEssence( aafi );
			//
			// aafi->ctx.current_essence = (aafiVideoEssence*)videoEssence;
			//
			//
			// videoEssence->file_name = aaf_get_propertyValueWstr( /*aafi->ctx.*/ParentMob, PID_Mob_Name );
			//
			//
			// videoEssence->masterMobID = aaf_get_propertyValue( /*aafi->ctx.*/ParentMob, PID_Mob_MobID );
			//
			// if ( videoEssence->masterMobID == NULL )
			// {
			// 	trace_obj( aafi, SourceClip, ANSI_COLOR_RED );
			// 	_error( aafi->ctx.options.verb, "Could not retrieve Mob::MobID.\n" );
			// 	return -1;
			// }
			//
			// /*
			//  *	p.49 : To create a SourceReference that refers to a MobSlot within
			//  *	the same Mob as the SourceReference, omit the SourceID property.
			//  */
			//
			// videoEssence->sourceMobID = SourceID; //aaf_get_propertyValue( SourceClip, PID_SourceReference_SourceID );
			//
			// if ( videoEssence->sourceMobID == NULL )
			// {
			// 	// aafObject *Mob = NULL;
			// 	//
			// 	// for ( Mob = SourceClip; Mob != NULL; Mob = Mob->Parent )
			// 	// {
			// 	// 	if ( aafUIDCmp( Mob->Class->ID, &AAFClassID_MasterMob ) )
			// 	// 		break;
			// 	// }
			//
			// 	videoEssence->sourceMobID = aaf_get_propertyValue( ParentMob, PID_Mob_MobID );
			//
			// 	_warning( aafi->ctx.options.verb, "Could not retrieve SourceReference::SourceID, retrieving from parent Mob.\n" );
			// }
			//
			//
			//
			// aafObject *SourceMob = aaf_get_MobByID( aafi->aafd->Mobs, videoEssence->sourceMobID );
			//
			// if ( SourceMob == NULL )
			// {
			// 	trace_obj( aafi, SourceClip, ANSI_COLOR_RED );
			// 	_error( aafi->ctx.options.verb, "Could not retrieve SourceMob.\n" );
			// 	return -1;
			// }
			//
			// videoEssence->SourceMob = SourceMob;
			//
			//
			//
			// // parse_SourceMob( aafi, SourceMob );
			//
			//
			// /* TODO the following must be moved to parse_SourceMob() !!! */
			//
			// aafObject *EssenceDesc = aaf_get_propertyValue( SourceMob, PID_SourceMob_EssenceDescription );
			//
			// if ( EssenceDesc == NULL )
			// {
			// 	trace_obj( aafi, SourceClip, ANSI_COLOR_RED );
			// 	_error( aafi->ctx.options.verb, "Could not retrieve EssenceDesc.\n" );
			// 	return -1;
			// }
			//
			//
			// // TODO
			// parse_EssenceDescriptor( aafi, EssenceDesc );
			//
			//
			//
			// videoEssence->unique_file_name = build_unique_videofilename( aafi, videoEssence );
			//
			//
			//
			// /* NOTE since multiple clips can point to the same MasterMob, we have to loop. */
			//
			// aafiVideoTrack   * videoTrack = NULL;
			// aafiTimelineItem * videoItem  = NULL;
			//
			// foreach_videoTrack( videoTrack, aafi )
			// {
			// 	foreach_Item( videoItem, videoTrack )
			// 	{
			// 		if ( videoItem->type != AAFI_VIDEO_CLIP )
			// 		{
			// 			continue;
			// 		}
			//
			// 		aafiVideoClip *videoClip = (aafiVideoClip*)&videoItem->data;
			//
			// 		if ( aafMobIDCmp( videoClip->masterMobID, videoEssence->masterMobID ) )
			// 		{
			// 			printf("FOUND VIDEO ESSENCE CLIP !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
			// 			videoClip->Essence = videoEssence;
			// 		}
			// 	}
			// }
			//
			// trace_obj( aafi, SourceClip, ANSI_COLOR_MAGENTA );
			//
		}
	}
	else
	{
		DUMP_OBJ_NO_SUPPORT( aafi, SourceClip, &__td );
		return -1;
	}

	return 0;
}



static int parse_Selector( AAF_Iface *aafi, aafObject *Selector, td *__ptd )
{
	/*
	 *	The Selector class is a sub-class of the Segment class.
	 *
	 *	The Selector class provides the value of a single Segment (PID_Selector_Selected)
	 *	while preserving references to unused alternatives (PID_Selector_Alternates)
	 */


 	struct trace_dump __td;
 	__td_set(__td, __ptd, 1);


	if ( resolve_AAF( aafi ) ) {
		return resolve_parse_aafObject_Selector( aafi, Selector, &__td );
	}


	aafObject *Selected = aaf_get_propertyValue( Selector, PID_Selector_Selected );

	if ( Selected == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, Selector, &__td, "Missing PID_Selector_Selected" );
		return -1;
	}

	// aafObject *Alternate = NULL;
	aafObject *Alternates = aaf_get_propertyValue( Selector, PID_Selector_Alternates );

	if ( Alternates == NULL ) /* opt */
	{
		// DUMP_OBJ_WARNING( aafi, Selector, &__td, "Missing PID_Selector_Alternates" );
	}


	DUMP_OBJ( aafi, Selector, &__td );

	/* without specific software implementation we stick to Selected and forget about Alternates */
	return parse_Segment( aafi, Selected, &__td );
}








/*
		        Parameter (abs)
		            |
		    ,--------------.
		    |              |
  ConstantValue   VaryingValue


	A Parameter object shall be owned by an OperationGroup object.
*/

static int parse_Parameter( AAF_Iface *aafi, aafObject *Parameter, td *__ptd )
{
	struct trace_dump __td;
	__td_set(__td, __ptd, 0);


	if ( aafUIDCmp( Parameter->Class->ID, &AAFClassID_ConstantValue ) )
	{

		return parse_ConstantValue( aafi, Parameter, &__td );

	}
	else if ( aafUIDCmp( Parameter->Class->ID, &AAFClassID_VaryingValue ) )
	{

		return parse_VaryingValue( aafi, Parameter, &__td );

	}

	return -1;
}




static int parse_ConstantValue( AAF_Iface *aafi, aafObject *ConstantValue, td *__ptd )
{
	struct trace_dump __td;
	__td_set(__td, __ptd, 1);

	// __td.sub = 1;

	if ( !aaf_get_propertyValue( ConstantValue->Parent, PID_OperationGroup_InputSegments ) ) {
		__td.eob = 1;
	}


	aafUID_t *ParamDef = aaf_get_propertyValue( ConstantValue, PID_Parameter_Definition );

	if ( ParamDef == NULL )	/* req */
	{
		DUMP_OBJ_ERROR( aafi, ConstantValue, &__td, "Missing PID_Parameter_Definition" );
		return -1;
	}

	aafUID_t *OperationIdentification = get_OperationGroup_OperationIdentification( aafi, ConstantValue->Parent );

	if ( OperationIdentification == NULL )
	{
		DUMP_OBJ_ERROR( aafi, ConstantValue, &__td, "Could not retrieve OperationIdentification" );
		return -1;
	}



	if ( aafUIDCmp( OperationIdentification, &AAFOperationDef_MonoAudioGain ) &&
       aafUIDCmp( ParamDef, &AAFParameterDef_Amplitude ) )
	{

		aafRational_t *multiplier = aaf_get_propertyIndirectValue( ConstantValue, PID_ConstantValue_Value );

		if ( multiplier == NULL )
		{
			DUMP_OBJ_ERROR( aafi, ConstantValue, &__td, "Missing PID_ConstantValue_Value" );
			return -1;
		}



		aafiAudioGain *Gain = calloc( sizeof(aafiAudioGain), sizeof(unsigned char) );


		Gain->pts_cnt = 1;
		Gain->value   = calloc( 1, sizeof(aafRational_t*) );
		Gain->flags  |= AAFI_AUDIO_GAIN_CONSTANT;

		memcpy( &Gain->value[0], multiplier, sizeof(aafRational_t) );



		/*
		 *	Loop through ancestors to find out who is the parent of OperationGroup.
		 *	If it belongs to TimelineMobSlot, that means the Parameter is attached
		 *	to a Track. If it belongs to a Component, the Parameter is attached to
		 *	a clip.
		 *
		 *	NOTE: We can't just check the Parent since we can have nested OperationGroups
		 *	providing different effects like Pan, Gain, CustomFx.. Therefore looping
		 *	is required.
		 */

		aafObject *Obj = ConstantValue->Parent; // Start with the last OperationGroup
		for (; Obj != NULL && aafUIDCmp( Obj->Class->ID, &AAFClassID_ContentStorage ) == 0; Obj = Obj->Parent )
			if ( !aafUIDCmp( Obj->Class->ID, &AAFClassID_OperationGroup ) )
				break;


		if ( aafUIDCmp( Obj->Class->ID, &AAFClassID_TimelineMobSlot ) )
		{
			/* Track-based Gain */
			aafi->ctx.current_track->gain = Gain;
		}
		else
		{
			/* Clip-based Gain */
			if ( aafi->ctx.current_clip_gain ) {

				DUMP_OBJ_ERROR( aafi, ConstantValue, &__td, "Clip gain was already set : +%05.1lf dB", 20 * log10( aafRationalToFloat( aafi->ctx.current_clip_gain->value[0] ) ) );

				// for ( int i = 0; i < aafi->ctx.current_clip_gain->pts_cnt; i++ ) {
				// 	printf( "   VaryingValue:  _time: %f   _value: %f\n",
				// 		aafRationalToFloat( aafi->ctx.current_clip_gain->time[i]  ),
				// 		aafRationalToFloat( aafi->ctx.current_clip_gain->value[i] ) );
				// }

				aafi_freeAudioGain( Gain );
				return -1;
			}
			else {
				aafi->ctx.current_clip_gain = Gain;
				aafi->ctx.clips_using_gain = 0;
			}
		}


		DUMP_OBJ( aafi, ConstantValue, &__td );

	}
	else if ( aafUIDCmp( OperationIdentification, &AAFOperationDef_MonoAudioPan ) &&
	          aafUIDCmp( ParamDef, &AAFParameterDef_Pan ) )
	{

		/*
		 *	Pan automation shall be track-based. If an application has a different
		 *	native representation (e.g., clip-based pan), it shall convert to and
		 *	from its native representation when exporting and importing the composition.
		 */

		aafRational_t *multiplier = aaf_get_propertyIndirectValue( ConstantValue, PID_ConstantValue_Value );

 		if ( multiplier == NULL )
 		{
 			DUMP_OBJ_ERROR( aafi, ConstantValue, &__td, "Missing PID_ConstantValue_Value" );
 			return -1;
 		}


		aafiAudioPan *Pan = calloc( sizeof(aafiAudioPan), sizeof(unsigned char) );


		Pan->pts_cnt = 1;
		Pan->value   = calloc( 1, sizeof(aafRational_t*) );
		Pan->flags  |= AAFI_AUDIO_GAIN_CONSTANT;

		memcpy( &Pan->value[0], multiplier, sizeof(aafRational_t) );


		/* Pan is Track-based only. */
		aafi->ctx.current_track->pan = Pan;

		DUMP_OBJ( aafi, ConstantValue, &__td );

	}
	else
	{
		DUMP_OBJ_NO_SUPPORT( aafi, ConstantValue, &__td );
	}

	return 0;
}




static int parse_VaryingValue( AAF_Iface *aafi, aafObject *VaryingValue, td *__ptd )
{
	struct trace_dump __td;
 	__td_set(__td, __ptd, 1);

	// __td.sub = 1;

	if ( !aaf_get_propertyValue( VaryingValue->Parent, PID_OperationGroup_InputSegments ) ) {
		__td.eob = 1;
	}


	aafUID_t *ParamDef = aaf_get_propertyValue( VaryingValue, PID_Parameter_Definition );

	if ( ParamDef == NULL )	/* req */
	{
		DUMP_OBJ_ERROR( aafi, VaryingValue, &__td, "Missing PID_Parameter_Definition" );
		return -1;
	}

	aafUID_t *OperationIdentification = get_OperationGroup_OperationIdentification( aafi, VaryingValue->Parent );

	if ( OperationIdentification == NULL )
	{
		DUMP_OBJ_ERROR( aafi, VaryingValue, &__td, "Could not retrieve OperationIdentification" );
		return -1;
	}


	aafiInterpolation_e interpolation = 0;
	aafUID_t *InterpolationIdentification = get_Parameter_InterpolationIdentification( aafi, VaryingValue );

	if ( InterpolationIdentification == NULL )
	{
		DUMP_OBJ_WARNING( aafi, VaryingValue, &__td, "Could not retrieve InterpolationIdentification: Setting to Linear" );
		interpolation = AAFI_INTERPOL_LINEAR;
	}
	else if ( aafUIDCmp( InterpolationIdentification, &AAFInterpolationDef_None ) )
	{
		interpolation = AAFI_INTERPOL_NONE;
	}
	else if ( aafUIDCmp( InterpolationIdentification, &AAFInterpolationDef_Linear ) )
	{
		interpolation = AAFI_INTERPOL_LINEAR;
	}
	else if ( aafUIDCmp( InterpolationIdentification, &AAFInterpolationDef_Power ) )
	{
		interpolation = AAFI_INTERPOL_POWER;
	}
	else if ( aafUIDCmp( InterpolationIdentification, &AAFInterpolationDef_Constant ) )
	{
		interpolation = AAFI_INTERPOL_CONSTANT;
	}
	else if ( aafUIDCmp( InterpolationIdentification, &AAFInterpolationDef_BSpline ) )
	{
		interpolation = AAFI_INTERPOL_BSPLINE;
	}
	else if ( aafUIDCmp( InterpolationIdentification, &AAFInterpolationDef_Log ) )
	{
		interpolation = AAFI_INTERPOL_LOG;
	}
	else
	{
		DUMP_OBJ_WARNING( aafi, VaryingValue, &__td, "Unknown value for InterpolationIdentification: Falling back to Linear" );
		interpolation = AAFI_INTERPOL_LINEAR;
	}



	aafObject *Points = aaf_get_propertyValue( VaryingValue, PID_VaryingValue_PointList );

	if ( Points == NULL )
	{
		/*
		 *	Some files like the ProTools and LogicPro break standard by having no
		 *	PointList entry for AAFOperationDef_MonoAudioGain.
		 */

		DUMP_OBJ_ERROR( aafi, VaryingValue, &__td, "Missing PID_VaryingValue_PointList" );
		return -1;
	}



	// if ( aafUIDCmp( VaryingValue->Parent->Parent->Class->ID, &AAFClassID_Transition ) )
	if ( aafUIDCmp( OperationIdentification, &AAFOperationDef_MonoAudioDissolve ) &&
       aafUIDCmp( ParamDef, &AAFParameterDef_Level ) )
	{

		aafiTransition *Trans = aafi->ctx.current_transition;

		Trans->flags    |= interpolation;
		Trans->pts_cnt_a = retrieve_ControlPoints( aafi, Points, &(Trans->time_a), &(Trans->value_a) );

		if ( Trans->pts_cnt_a < 0 )
		{
			/* In that case, parse_OperationGroup() will set transition to default. */
			DUMP_OBJ_ERROR( aafi, VaryingValue, &__td, "Could not retrieve ControlPoints" );
			return -1;
		}

		// for ( int i = 0; i < Trans->pts_cnt_a; i++ ) {
		// 	printf("time_%i : %i/%i   value_%i : %i/%i\n", i, Trans->time_a[i].numerator, Trans->time_a[i].denominator, i, Trans->value_a[i].numerator, Trans->value_a[i].denominator  );
		// }

		DUMP_OBJ( aafi, VaryingValue, &__td );

	}
	else if ( aafUIDCmp( OperationIdentification, &AAFOperationDef_MonoAudioGain ) &&
            aafUIDCmp( ParamDef, &AAFParameterDef_Amplitude ) )
	{

		aafiAudioGain *Gain = calloc( sizeof(aafiAudioGain), sizeof(unsigned char) );

		Gain->flags  |= interpolation;
		Gain->pts_cnt = retrieve_ControlPoints( aafi, Points, &Gain->time, &Gain->value );

		if ( Gain->pts_cnt < 0 )
		{
			DUMP_OBJ_ERROR( aafi, VaryingValue, &__td, "Could not retrieve ControlPoints" );
			free( Gain );
			return -1;
		}

		// for ( int i = 0; i < Gain->pts_cnt; i++ ) {
		// 	printf("time_%i : %i/%i   value_%i : %i/%i\n", i, Gain->time[i].numerator, Gain->time[i].denominator, i, Gain->value[i].numerator, Gain->value[i].denominator );
		// }


    /* If gain has 2 ControlPoints with both the same value, it means
     * we have a flat gain curve. So we can assume constant gain here. */

    if ( Gain->pts_cnt == 2 &&
        ( Gain->value[0].numerator   == Gain->value[1].numerator   ) &&
        ( Gain->value[0].denominator == Gain->value[1].denominator ) )
    {
			if ( aafRationalToFloat(Gain->value[0]) == 1.0f ) {
				/*
				 * gain is null, skip it. Skipping it allows not to set a useless gain then miss the real clip gain later (Resolve 18.5.AAF)
				 */
				aafi_freeAudioGain( Gain );
				return -1;
			}
			Gain->flags |= AAFI_AUDIO_GAIN_CONSTANT;
    }
    else
    {
      Gain->flags |= AAFI_AUDIO_GAIN_VARIABLE;
    }



		/*
		 *	Loop through ancestors to find out who is the parent of OperationGroup.
		 *	If it belongs to TimelineMobSlot, that means the Parameter is attached
		 *	to a Track. If it belongs to a Component, the Parameter is attached to
		 *	a clip.
		 *
		 *	NOTE: We can't just check the Parent since we can have nested OperationGroups
		 *	providing different effects like Pan, Gain, CustomFx.. Therefore looping
		 *	is required.
		 */

		aafObject *Obj = VaryingValue->Parent; // Start with the last OperationGroup
		for (; Obj != NULL && aafUIDCmp( Obj->Class->ID, &AAFClassID_ContentStorage ) == 0; Obj = Obj->Parent )
			if ( !aafUIDCmp( Obj->Class->ID, &AAFClassID_OperationGroup ) )
				break;


		if ( aafUIDCmp( Obj->Class->ID, &AAFClassID_TimelineMobSlot ) ) {
			/* Track-based Gain */

			if ( aafi->ctx.current_track->gain ) {
				DUMP_OBJ_ERROR( aafi, VaryingValue, &__td, "Track Gain was already set" );
				aafi_freeAudioGain( Gain );
				return -1;
			}
			else {
				aafi->ctx.current_track->gain = Gain;
				DUMP_OBJ( aafi, VaryingValue, &__td );
			}
		}
		else {
			/* Clip-based Gain */

			if ( Gain->flags & AAFI_AUDIO_GAIN_CONSTANT ) {
				if ( aafi->ctx.current_clip_gain ) {
					DUMP_OBJ_ERROR( aafi, VaryingValue, &__td, "Clip gain was already set" );
					aafi_freeAudioGain( Gain );
					return -1;
				}
				else {
					aafi->ctx.current_clip_gain = Gain;
					aafi->ctx.clips_using_gain = 0;
				}
			}
			else {
				if ( aafi->ctx.current_clip_automation ) {
					DUMP_OBJ_ERROR( aafi, VaryingValue, &__td, "Clip automation was already set" );
					aafi_freeAudioGain( Gain );
					return -1;
				}
				else {
					aafi->ctx.current_clip_automation = Gain;
					aafi->ctx.clips_using_automation = 0;
				}
			}
		}
	}
	else if ( aafUIDCmp( OperationIdentification, &AAFOperationDef_MonoAudioPan ) &&
	          aafUIDCmp( ParamDef, &AAFParameterDef_Pan ) )
	{

		/*
		 *	Pan automation shall be track-based. If an application has a different
		 *	native representation (e.g., clip-based pan), it shall convert to and
		 *	from its native representation when exporting and importing the composition.
		 */

		aafiAudioPan *Pan = calloc( sizeof(aafiAudioPan), sizeof(unsigned char) );

		Pan->flags |= AAFI_AUDIO_GAIN_VARIABLE;
		Pan->flags |= interpolation;

		Pan->pts_cnt = retrieve_ControlPoints( aafi, Points, &Pan->time, &Pan->value );

		if ( Pan->pts_cnt < 0 ) {
			DUMP_OBJ_ERROR( aafi, VaryingValue, &__td, "Could not retrieve ControlPoints" );
			free( Pan );
			return -1;
		}

		// for ( int i = 0; i < Gain->pts_cnt; i++ ) {
		// 	printf("time_%i : %i/%i   value_%i : %i/%i\n", i, Gain->time[i].numerator, Gain->time[i].denominator, i, Gain->value[i].numerator, Gain->value[i].denominator  );
		// }

		/* If Pan has 2 ControlPoints with both the same value, it means
		 * we have a constant Pan curve. So we can assume constant Pan here. */

		if ( Pan->pts_cnt == 2 &&
				( Pan->value[0].numerator   == Pan->value[1].numerator   ) &&
				( Pan->value[0].denominator == Pan->value[1].denominator ) )
		{
			// if ( aafRationalToFloat(Gain->value[0]) == 1.0f ) {
			// 	/*
			// 	 * Pan is null, skip it. Skipping it allows not to set a useless gain then miss the real clip gain later (Resolve 18.5.AAF)
			// 	 */
			// 	aafi_freeAudioGain( Gain );
			// 	return -1;
			// }
			Pan->flags |= AAFI_AUDIO_GAIN_CONSTANT;
		}
		else
		{
			Pan->flags |= AAFI_AUDIO_GAIN_VARIABLE;
		}

		if ( aafi->ctx.current_track->pan ) {
			DUMP_OBJ_ERROR( aafi, VaryingValue, &__td, "Track Pan was already set" );
			aafi_freeAudioGain( Pan );
			return -1;
		}
		else {
			aafi->ctx.current_track->pan = Pan;
			DUMP_OBJ( aafi, VaryingValue, &__td );
		}
	}
	else
	{
		DUMP_OBJ( aafi, VaryingValue, &__td );
	}

	return 0;
}



static int retrieve_ControlPoints( AAF_Iface *aafi, aafObject *Points, aafRational_t *times[], aafRational_t *values[] )
{

	*times  = calloc( Points->Header->_entryCount, sizeof(aafRational_t) );
	*values = calloc( Points->Header->_entryCount, sizeof(aafRational_t) );


	aafObject *Point  = NULL;

	uint i = 0;

	aaf_foreach_ObjectInSet( &Point, Points, &AAFClassID_ControlPoint )
	{

		aafRational_t *time  = aaf_get_propertyValue( Point, PID_ControlPoint_Time );

		if ( time == NULL )
		{
			trace_obj( aafi, Points, ANSI_COLOR_RED );
			_error( aafi->ctx.options.verb, "Missing ControlPoint::Time.\n" );

			free( times );  *times  = NULL;
			free( values ); *values = NULL;

			return -1;
		}


		aafRational_t *value = aaf_get_propertyIndirectValue( Point, PID_ControlPoint_Value );

		if ( value == NULL )
		{
			trace_obj( aafi, Points, ANSI_COLOR_RED );
			_error( aafi->ctx.options.verb, "Missing ControlPoint::Value.\n" );

			free( times );  *times  = NULL;
			free( values ); *values = NULL;

			return -1;
		}


		memcpy( (*times+i),  time, sizeof(aafRational_t) );
		memcpy( (*values+i), value, sizeof(aafRational_t) );


		i++;
	}

	if ( Points->Header->_entryCount != i )
	{
		trace_obj( aafi, Points, ANSI_COLOR_YELLOW );
		_warning( aafi->ctx.options.verb, "Points _entryCount (%i) does not match iteration (%i).\n", Points->Header->_entryCount, i );
		return i;
	}


	// trace_obj( aafi, Points, ANSI_COLOR_MAGENTA );

	return Points->Header->_entryCount;
}








/******************************************************************************
                                   M o b
 ******************************************************************************

                              Mob (abs)
                               |
                               |--> CompositionMob
                               |--> MasterMob
                               `--> SourceMob

******************************************************************************
******************************************************************************/

static int parse_Mob( AAF_Iface *aafi, aafObject *Mob )
{
	// int dl = DUMP_INIT(aafi);
	// __td(NULL);
	static td __td;
	__td.fn = __LINE__;
	__td.pfn = 0;
	__td.lv = 0;
	__td.ll = calloc( 1024, sizeof(int) ); /* TODO free */
	__td.ll[0] = 0;
	// aafi->ctx.trace_leveloop = __td.ll; // keep track of __td.ll for free



	aafObject *MobSlots = aaf_get_propertyValue( Mob, PID_Mob_Slots );

	if ( MobSlots == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, Mob, &__td, "Missing PID_Mob_Slots" );
		free(__td.ll);
		return -1;
	}


	if ( aafUIDCmp( Mob->Class->ID, &AAFClassID_CompositionMob ) )
	{
    aafUID_t *UsageCode = (aafUID_t*)aaf_get_propertyValue( Mob, PID_Mob_UsageCode );

    if ( aafUIDCmp( UsageCode, &AAFUsage_AdjustedClip ) ) {
      DUMP_OBJ_ERROR( aafi, Mob, &__td, "Skipping AAFUsage_AdjustedClip" );
      return -1;
    }

		parse_CompositionMob( aafi, Mob, &__td );
	}
	else if ( aafUIDCmp( Mob->Class->ID, &AAFClassID_MasterMob ) )
	{
		DUMP_OBJ( aafi, Mob, &__td );
	}
	else if ( aafUIDCmp( Mob->Class->ID, &AAFClassID_SourceMob ) )
	{
		DUMP_OBJ( aafi, Mob, &__td );
	}



	/*
	 *	Loops through MobSlots
	 */

	aafObject *MobSlot = NULL;
	// aafObject *MobSlots = aaf_get_propertyValue( Mob, PID_Mob_Slots );
	//
	// if ( MobSlots != NULL ) /* req */
	// {
	// 	DUMP_OBJ_ERROR( aafi, Mob, &__td, "Missing PID_Mob_Slots" );
	// 	return -1;
	// }


	int i = 0;

	aaf_foreach_ObjectInSet( &MobSlot, MobSlots, NULL )
	{
		__td.ll[__td.lv] = MobSlots->Header->_entryCount - i++;
		parse_MobSlot( aafi, MobSlot, &__td );
	}


	free(__td.ll);
	return 0;
}



static int parse_CompositionMob( AAF_Iface *aafi, aafObject *CompoMob, td *__ptd )
{

	struct trace_dump __td;
	__td_set( __td, __ptd, 0);


	DUMP_OBJ( aafi, CompoMob, &__td );



	aafi->compositionName = aaf_get_propertyValueWstr( CompoMob, PID_Mob_Name ); /* opt */




	aafObject *UserComment  = NULL;
	aafObject *UserComments = aaf_get_propertyValue( CompoMob, PID_Mob_UserComments ); /* opt */

	aaf_foreach_ObjectInSet( &UserComment, UserComments, NULL )
	{
		/* TODO implement retrieve_TaggedValue() */

		wchar_t *name = aaf_get_propertyValueWstr( UserComment, PID_TaggedValue_Name );

		if ( name == NULL ) /* req */
		{
			DUMP_OBJ_ERROR( aafi, UserComment, &__td, "Missing PID_TaggedValue_Name" );
			continue;
		}


		wchar_t *text = aaf_get_propertyIndirectValueWstr( UserComment, PID_TaggedValue_Value );

		if ( text == NULL ) /* req */
		{
			DUMP_OBJ_ERROR( aafi, UserComment, &__td, "Missing PID_TaggedValue_Value" );
			continue;
		}


		aafiUserComment *Comment = aafi_newUserComment( aafi, &aafi->Comments );

		Comment->name = name;
		Comment->text = text;

	}


	return 0;
}



// static int parse_MasterMob( AAF_Iface *aafi, aafObject *MasterMob, td *__ptd )
// {
// 	struct trace_dump __td;
// 	__td_set(__td, __ptd, 1);
//
// 	__td.hc = 1;
//
// 	DUMP_OBJ( aafi, MasterMob, &__td );
//
// 	return 0;
// }


static int parse_SourceMob( AAF_Iface *aafi, aafObject *SourceMob, td *__ptd )
{
	struct trace_dump __td;
	__td_set(__td, __ptd, 1);

	__td.hc = 1;

	// int dl = DUMP_INIT(aafi);

	// DUMP_OBJ( aafi, SourceMob, dl );


	// aafUID_t *DataDefinition = get_Component_DataDefinition( aafi, SourceMob );
	//
	// if ( DataDefinition == NULL ) /* req */
	// {
	// 	_error( aafi->ctx.options.verb, "Could not retrieve MobSlot::Segment DataDefinition.\n" );
	// 	return -1;
	// }
	//
	//
	//
	// if ( aafUIDCmp( DataDefinition, &AAFDataDef_Sound ) ||
	// 		 aafUIDCmp( DataDefinition, &AAFDataDef_LegacySound ) )

	/* TODO find a better way to check if we're parsing audio */

	if ( aafi->ctx.current_essence )
	{

		if ( aafi->ctx.current_essence == NULL )
		{
			DUMP_OBJ_ERROR( aafi, SourceMob, &__td, "ctx->current_essence no set" );
			return -1;
		}

		aafiAudioEssence *audioEssence = (aafiAudioEssence*)aafi->ctx.current_essence;



		aafMobID_t *MobID = aaf_get_propertyValue( SourceMob, PID_Mob_MobID );

		if ( MobID == NULL )
		{
			DUMP_OBJ_ERROR( aafi, SourceMob, &__td, "Missing PID_Mob_MobID" );
			return -1;
		}

		memcpy( audioEssence->umid, MobID, sizeof(aafMobID_t) );



		aafTimeStamp_t *CreationTime = aaf_get_propertyValue( SourceMob, PID_Mob_CreationTime );

		if ( CreationTime == NULL )
		{
			DUMP_OBJ_ERROR( aafi, SourceMob, &__td, "Missing PID_Mob_CreationTime" );
			return -1;
		}

		snprintf( audioEssence->originationDate, sizeof(audioEssence->originationDate), "%04u:%02u:%02u",
			(CreationTime->date.year  <= 9999) ? CreationTime->date.year : 0,
			(CreationTime->date.month <= 99) ? CreationTime->date.month : 0,
			(CreationTime->date.day <= 99) ? CreationTime->date.day : 0 );

		snprintf( audioEssence->originationTime, sizeof(audioEssence->originationTime), "%02u:%02u:%02u",
			(CreationTime->time.hour <= 99) ? CreationTime->time.hour : 0,
			(CreationTime->time.minute <= 99) ? CreationTime->time.minute : 0,
			(CreationTime->time.second <= 99) ? CreationTime->time.second : 0 );

	}



	aafObject *EssenceDesc = aaf_get_propertyValue( SourceMob, PID_SourceMob_EssenceDescription );

	if ( EssenceDesc == NULL )
	{
		DUMP_OBJ_ERROR( aafi, SourceMob, &__td, "Could not retrieve EssenceDescription" );
		return -1;
	}

	DUMP_OBJ( aafi, SourceMob, &__td );


	parse_EssenceDescriptor( aafi, EssenceDesc, &__td );

	return 0;
}





static aafiAudioTrack * get_audio_track_by_tracknumber( AAF_Iface *aafi, int tracknumber )
{
	aafiAudioTrack *audioTrack = NULL;
	int count = 0;

	foreach_audioTrack( audioTrack, aafi )
	{
		if ( ++count == tracknumber )
			return audioTrack;
	}

	return NULL;
}


/******************************************************************************
                               M o b S l o t
 ******************************************************************************

                            MobSlot (abs)
                               |
                               |--> TimelineMobSlot
                               |--> EventMobSlot
                               `--> StaticMobSlot

******************************************************************************
******************************************************************************/

static int parse_MobSlot( AAF_Iface *aafi, aafObject *MobSlot, td *__ptd )
{

	// printf("MS : %p\n", __ptd->ll );

	struct trace_dump __td;
	__td_set(__td, __ptd, 1);

	__td.hc = 1;



	aafObject *Segment = aaf_get_propertyValue( MobSlot, PID_MobSlot_Segment );

	if ( Segment == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, MobSlot, &__td, "Missing PID_MobSlot_Segment" );
		return -1;
	}


	aafUID_t *DataDefinition = get_Component_DataDefinition( aafi, Segment );

	if ( DataDefinition == NULL ) /* req */
	{
		DUMP_OBJ_ERROR( aafi, MobSlot, &__td, "Could not retrieve DataDefinition" );
		return -1;
	}




  aafPosition_t session_end = 0;

	if ( aafUIDCmp( MobSlot->Class->ID, &AAFClassID_TimelineMobSlot ) )
	{

		/*
		 *	Each TimelineMobSlot represents a track, either audio or video.
		 *
		 *	The Timeline MobSlot::Segment should hold a Sequence of Components.
		 *	This Sequence represents the timeline track. Therefore, each SourceClip
		 *	contained in the Sequence::Components represents a clip on the timeline.
		 *
		 *	CompositionMob can have TimelineMobSlots, StaticMobSlots, EventMobSlots
		 */

		aafRational_t *edit_rate = (aafRational_t*)aaf_get_propertyValue( MobSlot, PID_TimelineMobSlot_EditRate );

		if ( edit_rate == NULL ) /* req */
		{
			DUMP_OBJ_ERROR( aafi, MobSlot, &__td, "Missing PID_TimelineMobSlot_EditRate" );
			return -1;
		}


		if ( aafUIDCmp( /*aafi->ctx.*/MobSlot->Parent->Class->ID, &AAFClassID_CompositionMob ) )
		{

			/*
			 *	There should be only one Composition, since a CompositionMob represents the overall
			 *	composition (i.e project). Observations on files confirm that.
			 *
			 *	However, the AAF Edit Protocol says that there could be multiple CompositionMobs
			 *	(Mob::UsageCode TopLevel), containing other CompositionMobs (Mob::UsageCode LowerLevel).
			 *	This was not encountered yet, even on Avid exports with AAF_EditProtocol enabled.
			 *
			 *	TODO: update note
			 *	TODO: implement multiple TopLevel compositions support
			 */

			if ( aafUIDCmp( DataDefinition, &AAFDataDef_Sound ) ||
			     aafUIDCmp( DataDefinition, &AAFDataDef_LegacySound ) )
			{

				/*
				 *	p.11 : In a CompositionMob or MasterMob, PhysicalTrackNumber is the
				 *	output channel number that the MobSlot should be routed to when played.
				 */

				if ( !aafi->ctx.is_inside_derivation_chain )
				{
					uint32_t tracknumber = 0;
					uint32_t *track_num  = (uint32_t*)aaf_get_propertyValue( MobSlot, PID_MobSlot_PhysicalTrackNumber );

					if ( track_num == NULL ) { /* opt */
						tracknumber = aafi->Audio->track_count + 1;
					} else {
						tracknumber = *track_num;
					}


					aafiAudioTrack *track = get_audio_track_by_tracknumber( aafi, tracknumber );

					if ( !track )
					{
						track = aafi_newAudioTrack( aafi );
					}

					track->number = tracknumber;

					aafi->Audio->track_count += 1;

					aafi->ctx.current_track = track;



					track->name = aaf_get_propertyValueWstr( MobSlot, PID_MobSlot_SlotName );

					track->edit_rate = edit_rate;
				}

				// aaf_dump_ObjectProperties( aafi->aafd, MobSlot );

				/***********************************************************************************************************************/

				/*
				 *	The following seems to be ProTools proprietary.
				 *	If a track is multi-channel, it specifies its format : 2 (stereo), 6 (5.1) or 8 (7.1).
				 *
				 *	In the current implementation we don't need this. We guess the format at the OperationGroup level with the
				 *	AAFOperationDef_AudioChannelCombiner OperationDefinition, which also looks to be ProTools specific.
				 */

				// aafPID_t PIDTimelineMobAttributeList = aaf_get_PropertyIDByName( aafi->aafd, "TimelineMobAttributeList" );
				// // aafPID_t PIDTimelineMobAttributeList = aaf_get_PropertyIDByName( aafi->aafd, "MobAttributeList" );
				//
				// if ( PIDTimelineMobAttributeList != 0 )
				// {
				// 	aafObject *TaggedValues = aaf_get_propertyValue( aafi->ctx.MobSlot, PIDTimelineMobAttributeList );
				// 	aafObject *TaggedValue  = NULL;
				//
				// 	aaf_foreach_ObjectInSet( &TaggedValue, TaggedValues, NULL )
				// 	{
				// 		char *name = aaf_get_propertyValueText( TaggedValue, PID_TaggedValue_Name );
				//
				// 		printf("TaggedValue %s\n", name );
				//
				// 		if ( strncmp( "_TRACK_FORMAT", name, 13 ) == 0 )
				// 		{
				// 			uint32_t *format = (uint32_t*)aaf_get_propertyIndirectValue( TaggedValue, PID_TaggedValue_Value );
				//
				// 			if ( format != NULL )
				// 				aafi->ctx.current_track->format = *format;
				//
				// 			printf("Format : %u\n", aafi->ctx.current_track->format );
				// 		}
				//
				// 		free( name );
				// 	}
				// }

				/***********************************************************************************************************************/

				DUMP_OBJ( aafi, MobSlot, &__td );


				/* Reset timeline position */
				// aafi->ctx.current_track->current_pos = 0;

				parse_Segment( aafi, Segment, &__td );


        /* update session_end if needed */
				// session_end = ( aafi->ctx.current_pos > session_end ) ? aafi->ctx.current_pos : session_end;
				// fprintf( stderr, "AAFIParser 4286: Current pos : %lu\n", aafi->ctx.current_track->current_pos );
				session_end = ( aafi->ctx.current_track->current_pos > session_end ) ? aafi->ctx.current_track->current_pos : session_end;

        // printf( "SESSIon_end : %li\n", session_end );

			}
			else if ( aafUIDCmp( DataDefinition, &AAFDataDef_Timecode ) ||
					      aafUIDCmp( DataDefinition, &AAFDataDef_LegacyTimecode ) )
			{
				DUMP_OBJ( aafi, MobSlot, &__td );

				parse_Segment( aafi, Segment, &__td );
			}
			else if ( aafUIDCmp( DataDefinition, &AAFDataDef_Picture ) ||
			          aafUIDCmp( DataDefinition, &AAFDataDef_LegacyPicture ) )
			{

				// printf("%ls\n", ClassIDToText(aafi->aafd, Segment->Class->ID) );
				// printf("\n\nCOUCOU VIDEO\n\n" );


				/**** ADP NESTED SCOPE ****/

				// aafObject *NSSegment = NULL;
				// aafObject *NSSegments = aaf_get_propertyValue( Segment, PID_NestedScope_Slots );
				//
				// aaf_foreach_ObjectInSet( &NSSegment, NSSegments, NULL )
				// {
				// 	aaf_dump_ObjectProperties( aafi->aafd, NSSegment );
				// 	printf("\n\n\n\n" );
				// }



				if ( aafi->Video->Tracks )
				{
					DUMP_OBJ_ERROR( aafi, MobSlot, &__td, "Current implementation supports only one video track" );
					return -1;
				}


				/*
				 *	p.11 : In a CompositionMob or MasterMob, PhysicalTrackNumber is the output channel number that the
				 *	MobSlot should be routed to when played.
				 */

				uint32_t tracknumber = 0;
				uint32_t *track_num = (uint32_t*)aaf_get_propertyValue( MobSlot, PID_MobSlot_PhysicalTrackNumber );

				if ( track_num == NULL ) { /* opt */
					tracknumber = 1;       /* Current implementation supports only one video track. */
				} else {
					tracknumber = *track_num;
				}


				aafiVideoTrack *track = aafi_newVideoTrack( aafi );


				track->number = tracknumber;

				track->name = aaf_get_propertyValueWstr( MobSlot, PID_MobSlot_SlotName );

				track->edit_rate = edit_rate;


				DUMP_OBJ( aafi, MobSlot, &__td );


				parse_Segment( aafi, Segment, &__td );


        /* update session_end if needed */
				// session_end = ( aafi->ctx.current_track->current_pos > session_end ) ? aafi->ctx.current_track->current_pos : session_end;
			}
			else
			{
				DUMP_OBJ_NO_SUPPORT( aafi, MobSlot, &__td );
			}

		}
		else if ( aafUIDCmp( MobSlot->Parent->Class->ID, &AAFClassID_MasterMob ) )
		{
			/* Retrieve Essences */

			// if ( aafUIDCmp( DataDefinition, &AAFDataDef_Sound ) ||
			// 	   aafUIDCmp( DataDefinition, &AAFDataDef_LegacySound ) )
			// {
				DUMP_OBJ( aafi, MobSlot, &__td );


				parse_Segment( aafi, Segment, &__td );

			// }
			// else if ( aafUIDCmp( DataDefinition, &AAFDataDef_Picture ) ||
			// 				  aafUIDCmp( DataDefinition, &AAFDataDef_LegacyPicture ) )
			// {
			// 	parse_Segment( aafi, Segment );
			//
			// 	retrieve_EssenceData( aafi );
			// }
			// else
			// {
			// 	trace_obj( aafi, MobSlot, ANSI_COLOR_YELLOW );
			// 	printf( "%ls\n", DataDefToText( aafi->aafd, DataDefinition ) );
			// }
		}
		else if ( aafUIDCmp( MobSlot->Parent->Class->ID, &AAFClassID_SourceMob ) )
		{

			if ( aafi->ctx.current_essence != NULL )
			{
				aafiAudioEssence *audioEssence = (aafiAudioEssence*)aafi->ctx.current_essence;

				aafPosition_t *Origin = aaf_get_propertyValue( MobSlot, PID_TimelineMobSlot_Origin );

				if ( Origin == NULL ) /* req */
				{
					DUMP_OBJ_ERROR( aafi, MobSlot, &__td, "Missing PID_TimelineMobSlot_Origin" );
					return -1;
				}

				audioEssence->timeReference = *Origin;
				audioEssence->mobSlotEditRate = edit_rate;

				DUMP_OBJ( aafi, MobSlot, &__td );
			}
			else
			{
				DUMP_OBJ_ERROR( aafi, MobSlot, &__td, "aafi->ctx.current_essence no set" );
			}
		}
		else
		{
			/* Not in CompositionMob, MasterMob, or SourceMob. Can not happen. */
			DUMP_OBJ_NO_SUPPORT( aafi, MobSlot, &__td );
		}

	}
	else if ( aafUIDCmp( MobSlot->Class->ID, &AAFClassID_EventMobSlot ) ) {

		aafRational_t *edit_rate = (aafRational_t*)aaf_get_propertyValue( MobSlot, PID_EventMobSlot_EditRate );

		if ( edit_rate == NULL ) /* req */
		{
			DUMP_OBJ_ERROR( aafi, MobSlot, &__td, "Missing PID_EventMobSlot_EditRate" );
			return -1;
		}

		aafi->ctx.current_markers_edit_rate = edit_rate;

		DUMP_OBJ( aafi, MobSlot, &__td );

		return parse_Segment( aafi, Segment, &__td );
	}
	else
	{
		/* Not AAFClassID_TimelineMobSlot */
		DUMP_OBJ_NO_SUPPORT( aafi, MobSlot, &__td );
	}

	/* TODO implement global (audio and video) session start and end */
	// if ( aafi->ctx.current_tree_type == AAFI_TREE_TYPE_AUDIO )
	  if ( session_end > 0 && aafi->Audio->tc && aafi->Audio->tc->end < session_end )
	      aafi->Audio->tc->end = session_end;
	// if ( aafi->ctx.current_tree_type == AAFI_TREE_TYPE_VIDEO )
	//   if ( session_end > 0 && aafi->Video->tc )
	//       aafi->Video->tc->end = session_end;
	// else
  //     _error("MISSING aafiTimecode !\n"); // TODO handle session's end more properly

	return 0;
}




int aafi_retrieveData( AAF_Iface *aafi )
{
	aafObject *Mob = NULL;


	aaf_foreach_ObjectInSet( &Mob, aafi->aafd->Mobs, &AAFClassID_CompositionMob )
	{

		aafUID_t *UsageCode = (aafUID_t*)aaf_get_propertyValue( Mob, PID_Mob_UsageCode );

		if ( aafUIDCmp( aafi->aafd->Header.OperationalPattern, &AAFOPDef_EditProtocol ) && !aafUIDCmp( UsageCode, &AAFUsage_TopLevel ) )
		{
			/*
			 *	If we run against AAFOPDef_EditProtocol, we process only TopLevels CompositionMobs.
			 *	If there is more than one, we have multiple Compositions in a single AAF.
			 */


			// trace_obj( aafi, Mob, ANSI_COLOR_RED );



			// // aaf_dump_ObjectProperties( aafi->aafd, aafi->ctx.Mob );
			//
			// aafObject *MobSlots = aaf_get_propertyValue( aafi->ctx.Mob, PID_Mob_Slots );
			// aafObject *MobSlot = NULL;
			// uint32_t SlotID = 0;
			//
			// aaf_foreach_ObjectInSet( &MobSlot, MobSlots, NULL )
			// {
			// 	aaf_dump_ObjectProperties( aafi->aafd, MobSlot );
			// 	printf("\n\n\n\n\n" );
			// }

			continue;
		}

		RESET_CONTEXT( aafi->ctx );

		aafi->ctx.current_dump_level = 0;

		parse_Mob( aafi, Mob );

	}



	// aafiAudioTrack   *audioTrack = NULL;
	// aafiTimelineItem *audioItem  = NULL;
	// // aafiAudioClip    *audioClip  = NULL;
	//
	// // uint32_t i = 0;
	//
	// foreach_audioTrack( audioTrack, aafi )
	// {
	// 	foreach_Item( audioItem, audioTrack )
	// 	{
	//
	// 		if ( audioItem->type == AAFI_TRANS )
	// 		{
	// 			continue;
	// 		}
	//
	// 		aafiAudioClip *audioClip = (aafiAudioClip*)&audioItem->data;
	//
	// 		if ( audioClip->masterMobID && !audioClip->Essence )
	// 		{
	// 			printf("\n\n\n\n\n\n    E m p t y   C l i p\n\n\n\n" );
	//
	// 			aafObject *Mob = NULL;
	//
	// 			aaf_foreach_ObjectInSet( &Mob, aafi->aafd->Mobs, NULL )
	// 			{
	// 				/* loops through Mobs */
	// 				aafUID_t *UsageCode = (aafUID_t*)aaf_get_propertyValue( Mob, PID_Mob_UsageCode );
	//
	// 				aafMobID_t *MobID = (aafMobID_t*)aaf_get_propertyValue( Mob, PID_Mob_MobID );
	//
	// 				if ( !aafMobIDCmp( MobID, audioClip->masterMobID ) )
	// 				{
	// 					continue;
	// 				}
	//
	// 				// aaf_dump_ObjectProperties( aafi->aafd, Mob );
	// 				printf("Clip SourceID      : %ls\n\n", MobIDToText(MobID) );
	//
	// 				printf("PointedMob ClassID : %ls\n", ClassIDToText(aafi->aafd, Mob->Class->ID) );
	// 				printf("PointedMob UsageCd : %ls\n", UsageCodeToText(UsageCode) );
	// 				printf("PointedMob Name    : %ls\n\n", aaf_get_propertyValueWstr(Mob, PID_Mob_Name) );
	//
	//
	//
	// 				aafObject *MobSlots = aaf_get_propertyValue( Mob, PID_Mob_Slots );
	// 				aafObject *MobSlot = NULL;
	// 				int SlotID = 1;
	// 				aaf_foreach_ObjectInSet( &MobSlot, MobSlots, NULL )
	// 				{
	// 					printf("  SlotID %u\n", SlotID );
	//
	// 					aafObject *Segment = aaf_get_propertyValue( MobSlot, PID_MobSlot_Segment );
	//
	// 					if ( Segment == NULL )
	// 					{
	// 						_error( aafi->ctx.options.verb, "Missing MobSlot::Segment.\n" );
	// 						return -1;
	// 					}
	//
	//
	// 					aafUID_t *DataDefinition = get_Component_DataDefinition( aafi, Segment );
	//
	// 					if ( DataDefinition == NULL )
	// 					{
	// 						_error( aafi->ctx.options.verb, "Could not retrieve MobSlot::Segment DataDefinition.\n" );
	// 						return -1;
	// 					}
	//
	// 					// CURRENTPOINTER
	//
	// 					printf("    Segment : %ls\n", ClassIDToText( aafi->aafd, Segment->Class->ID ) );
	// 					printf("    DataDefinition : %ls\n", DataDefToText(aafi->aafd, DataDefinition) );
	//
	// 					printf("\n");
	//
	//
	//
	//
	//
	// 					if ( aafUIDCmp( Segment->Class->ID, &AAFClassID_SourceClip ) )
	// 					{
	// 						aafMobID_t *SourceID = aaf_get_propertyValue( Segment, PID_SourceReference_SourceID );
	//
	// 						printf("     SourceID : %ls\n", MobIDToText(sourceID) );
	// 					}
	//
	// 					SlotID++;
	// 				}
	// 				printf("\n");
	//
	//
	// 			}
	// 		}
	// 	}
	// }





	/*
	 */
	// aaf_foreach_ObjectInSet( &(aafi->ctx.Mob), aafi->aafd->Mobs, &AAFClassID_SourceMob )
	// {
	//
	// 	aafObject *MobSlots = aaf_get_propertyValue( aafi->ctx.Mob, PID_Mob_Slots );
	//
	// 	aaf_foreach_ObjectInSet( &(aafi->ctx.MobSlot), MobSlots, NULL )
	// 	{
	//
	// 		/*
	// 		 *	Check if the SourceMob was parsed.
	// 		 *	If it was not, we can print the trace.
	// 		 *
	// 		 *	NOTE We do it after the main loop, so we make sure all MasterMobs was parsed.
	// 		 */
	//
	// 		aafObject *Segment = aaf_get_propertyValue( aafi->ctx.MobSlot, PID_MobSlot_Segment );
	//
 	// 		aafUID_t  *DataDefinition = get_Component_DataDefinition( aafi, Segment );
	//
	// 		aafMobID_t *MobID = aaf_get_propertyValue( aafi->ctx.Mob, PID_Mob_MobID );
	//
	// 		aafiAudioEssence *audioEssence = NULL;
	//
	// 		foreachEssence( audioEssence, aafi->Audio->Essences )
	// 		{
	// 			if ( aafMobIDCmp( MobID, audioEssence->sourceMobID ) )
	// 				break;
	// 		}
	//
	// 		if ( audioEssence == NULL )
	// 		{
	// 			trace_obj( aafi, aafi->ctx.MobSlot, ANSI_COLOR_YELLOW );
	// 			printf( "%ls\n", DataDefToText( aafi->aafd, DataDefinition ) );
	// 		}
	//
	// 	}
	// }



	/* *** Post processing *** */

	/* TODO move to parse_***() */
	/* Parse summary descriptor (WAVE/AIFC) if any */

	aafiAudioEssence *audioEssence = NULL;

	foreachEssence( audioEssence, aafi->Audio->Essences ) {

		/* TODO: rename (not only summary, can be external file too) */
		parse_audio_summary( aafi, audioEssence );

		/* TODO : check samplerate / samplesize proportions accross essences, and choose the most used values as composition values */
		if ( aafi->Audio->samplerate == 0 || aafi->Audio->samplerate == audioEssence->samplerate ) {
			aafi->Audio->samplerate = audioEssence->samplerate;
		}
		else {
			// _warning( aafi->ctx.options.verb, "audioEssence '%ls' has different samplerate : %u\n", audioEssence->file_name, audioEssence->samplerate );
		}

		if ( aafi->Audio->samplesize == 0 || aafi->Audio->samplesize == audioEssence->samplesize ) {
			aafi->Audio->samplesize = audioEssence->samplesize;
		}
		else {
			// _warning( aafi->ctx.options.verb, "audioEssence '%ls' has different samplesize : %i\n", audioEssence->file_name, audioEssence->samplesize );
		}
	}


	aafiAudioTrack *audioTrack = NULL;

	foreach_audioTrack( audioTrack, aafi ) {
		if ( audioTrack->current_pos > aafi->Audio->length ) {
			aafi->Audio->length = audioTrack->current_pos;
			aafi->Audio->length_editRate.numerator   = audioTrack->edit_rate->numerator;
			aafi->Audio->length_editRate.denominator = audioTrack->edit_rate->denominator;
		}
	}

	aafiVideoTrack *videoTrack = NULL;

	foreach_videoTrack( videoTrack, aafi ) {
		if ( videoTrack->current_pos > aafi->Video->length ) {
			aafi->Video->length = videoTrack->current_pos;
			aafi->Video->length_editRate.numerator   = videoTrack->edit_rate->numerator;
			aafi->Video->length_editRate.denominator = videoTrack->edit_rate->denominator;
		}
	}

	if ( aafi->Audio->length > aafi->Video->length ) {
		aafi->compositionLength = aafi->Audio->length;
		aafi->compositionLength_editRate.numerator   = aafi->Audio->length_editRate.numerator;
		aafi->compositionLength_editRate.denominator = aafi->Audio->length_editRate.denominator;
	}
	else {
		aafi->compositionLength = aafi->Video->length;
		aafi->compositionLength_editRate.numerator   = aafi->Video->length_editRate.numerator;
		aafi->compositionLength_editRate.denominator = aafi->Video->length_editRate.denominator;
	}



	if ( aafi->Audio->tc && !aafi->Video->tc ) {
		aafi->compositionStart = aafi->Audio->tc->start;
		aafi->compositionStart_editRate.numerator   = aafi->Audio->tc->edit_rate->numerator;
		aafi->compositionStart_editRate.denominator = aafi->Audio->tc->edit_rate->denominator;
	}
	else
	if ( !aafi->Audio->tc && aafi->Video->tc ) {
		aafi->compositionStart = aafi->Video->tc->start;
		aafi->compositionStart_editRate.numerator   = aafi->Video->tc->edit_rate->numerator;
		aafi->compositionStart_editRate.denominator = aafi->Video->tc->edit_rate->denominator;
	}
	else
	if ( aafi->Audio->tc && aafi->Video->tc ) {
		if ( aafi->Audio->tc->start > aafi->Video->tc->start ) {
			aafi->compositionStart = aafi->Audio->tc->start;
			aafi->compositionStart_editRate.numerator   = aafi->Audio->tc->edit_rate->numerator;
			aafi->compositionStart_editRate.denominator = aafi->Audio->tc->edit_rate->denominator;
		}
		else {
			aafi->compositionStart = aafi->Video->tc->start;
			aafi->compositionStart_editRate.numerator   = aafi->Video->tc->edit_rate->numerator;
			aafi->compositionStart_editRate.denominator = aafi->Video->tc->edit_rate->denominator;
		}
	}


	if ( protools_AAF( aafi ) ) {
		protools_post_processing( aafi );
	}

	return 0;
}

/**
 *	@}
 */

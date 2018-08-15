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

#include <wchar.h>
#include <locale.h>

#include "../libAAF.h"

#include "./ProTools.h"

#include "../common/debug.h"
#include "../common/utils.h"

// #include "thirdparty/libriff.h"
// #include "thirdparty/libwav.h"









#define RESET_CONTEXT( ctx )                        \
	ctx.MobSlot = NULL;                             \
	ctx.current_track = NULL;                       \
	ctx.current_track_is_multichannel = 0;          \
	ctx.current_multichannel_track_channel = 0;     \
	ctx.current_multichannel_track_clip_length = 0; \
	ctx.current_pos = 0;                            \
	ctx.current_transition = NULL;                  \
	ctx.current_gain = NULL;                        \
	ctx.current_audioEssence = NULL;                \



static void trace_obj( AAF_Iface *aafi, aafObject *Obj, char *color );


static wchar_t   * build_unique_filename( AAF_Iface *aafi, aafiAudioEssence *audioEssence );


static aafUID_t  * get_Component_DataDefinition( AAF_Iface *aafi, aafObject *Component );
static aafUID_t  * get_FileDescriptor_ContainerFormat( AAF_Iface *aafi, aafObject *FileDescriptor );
static aafUID_t  * get_OperationGroup_OperationIdentification( AAF_Iface *aafi, aafObject *OperationGroup );
static aafUID_t  * get_Parameter_InterpolationIdentification( AAF_Iface *aafi, aafObject *Parameter );

static aafObject * get_EssenceData_By_MobID( AAF_Iface *aafi, aafMobID_t *MobID );

static aafiAudioEssence * getEssenceBySourceMobID( AAF_Iface *aafi, aafMobID_t *sourceMobID );


static int   parse_EssenceDescriptor( AAF_Iface *aafi, aafObject *EssenceDesc );
static int   parse_PCMDescriptor( AAF_Iface *aafi, aafObject *PCMDescriptor );
static int   parse_WAVEDescriptor( AAF_Iface *aafi, aafObject *WAVEDescriptor );
static int   parse_AIFCDescriptor( AAF_Iface *aafi, aafObject *AIFCDescriptor );

static int   parse_Locator( AAF_Iface *aafi, aafObject *Locator );
static int   parse_NetworkLocator( AAF_Iface *aafi, aafObject *NetworkLocator );

static int   retrieve_EssenceData( AAF_Iface *aafi );


static int   parse_Component( AAF_Iface *aafi, aafObject *Component );
static int   parse_Transition( AAF_Iface *aafi, aafObject *Transition );
static int   parse_Segment( AAF_Iface *aafi, aafObject *Segment );
static int   parse_Filler( AAF_Iface *aafi, aafObject *Filler );
static int   parse_Sequence( AAF_Iface *aafi, aafObject *Sequence );
static int   parse_Timecode( AAF_Iface *aafi, aafObject *Timecode );
static int   parse_OperationGroup( AAF_Iface *aafi, aafObject *OpGroup );
static void * parse_SourceClip( AAF_Iface *aafi, aafObject *SourceClip );


static int   parse_Parameter( AAF_Iface *aafi, aafObject *Parameter );
static int   parse_ConstantValue( AAF_Iface *aafi, aafObject *ConstantValue );
static int   parse_VaryingValue( AAF_Iface *aafi, aafObject *VaryingValue );
static int   retrieve_ControlPoints( AAF_Iface *aafi, aafObject *Points, aafRational_t *times[], aafRational_t *values[] );


static int   parse_Mob( AAF_Iface *aafi, aafObject *Mob );
static int   parse_SourceMob( AAF_Iface *aafi, aafObject *SourceMob );

static int   parse_MobSlot( AAF_Iface *aafi, aafObject *MobSlot );








static void trace_obj( AAF_Iface *aafi, aafObject *Obj, char *color )
{
	char buf[1024];
	char tmp[1024];

	memset( buf, 0x00, 1024 );
	memset( tmp, 0x00, 1024 );

	for (; auidCmp( Obj->Class->ID, &AAFClassID_ContentStorage ) == 0 /*Obj != NULL*/; Obj = Obj->Parent )
	{
		// snprintf( tmp, 1024, "%s", ClassIDToText( aafi->aafd, Obj->Class->ID ) );
		strncpy( tmp, buf, 1024 );

		if ( auidCmp( Obj->Class->ID, &AAFClassID_TimelineMobSlot ) && auidCmp( Obj->Parent->Class->ID, &AAFClassID_CompositionMob ) )
		{
			wchar_t *name = aaf_get_propertyValueWstr( Obj, PID_MobSlot_SlotName );
			snprintf( buf, 1024, "%ls (%ls) > %s", ClassIDToText( aafi->aafd, Obj->Class->ID ), name, tmp );
			free( name );
		}
		else if ( auidCmp( Obj->Class->ID, &AAFClassID_MasterMob ) || auidCmp( Obj->Class->ID, &AAFClassID_SourceMob ) )
		{
			wchar_t *name  = aaf_get_propertyValueWstr( Obj, PID_Mob_Name );
			snprintf( buf, 1024, "%ls (%ls) > %s", ClassIDToText( aafi->aafd, Obj->Class->ID ), name, tmp );
			free( name );
		}
		else if ( auidCmp( Obj->Class->ID, &AAFClassID_CompositionMob ) )
		{
			wchar_t *name = aaf_get_propertyValueWstr( Obj, PID_Mob_Name );
			const wchar_t *usage = UsageCodeToText( aaf_get_propertyValue( Obj, PID_Mob_UsageCode ) );
			snprintf( buf, 1024, "%ls (%ls : %ls) > %s", ClassIDToText( aafi->aafd, Obj->Class->ID ), usage, name, tmp );
			free( name );
		}
		else if ( auidCmp( Obj->Class->ID, &AAFClassID_OperationGroup ) )
		{
			aafUID_t *OperationIdentification = get_OperationGroup_OperationIdentification( aafi, Obj );

			const wchar_t *name = OperationDefToText( aafi->aafd, OperationIdentification ) /*AUIDToText( OpIdent )*/;
			snprintf( buf, 1024, "%ls (%ls) > %s", ClassIDToText( aafi->aafd, Obj->Class->ID ), name, tmp );
		}
		else
		{
			snprintf( buf, 1024, "%ls > %s", ClassIDToText( aafi->aafd, Obj->Class->ID ), tmp );
		}
	}

	buf[strlen(buf)-2] = 0x00;

	printf("%s%s%s\n", color, buf, ANSI_COLOR_RESET );
}




static wchar_t * build_unique_filename( AAF_Iface *aafi, aafiAudioEssence *audioEssence )
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

	foreachAudioEssence( ae, aafi->Audio->Essences )
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








/******************************************************************************
                             D i c t i o n a r y
 ******************************************************************************

 ******************************************************************************
 *****************************************************************************/

static aafUID_t * get_Component_DataDefinition( AAF_Iface *aafi, aafObject *Component )
{
	aafWeakRef_t *weakRef = aaf_get_propertyValue( Component, PID_Component_DataDefinition );

	if ( weakRef == NULL )
		_warning( "Missing Component::DataDefinition.\n" );


	aafObject *DataDefinition = aaf_get_ObjectByWeakRef( aafi->aafd->DataDefinition, weakRef );

	if ( DataDefinition == NULL )
		_warning( "Could not retrieve WeakRef from Dictionary::DataDefinition.\n" );


	aafUID_t  *DataIdentification = aaf_get_propertyValue( DataDefinition, PID_DefinitionObject_Identification );

	if ( DataIdentification == NULL )
		_warning( "Missing DataDefinition's DefinitionObject::Identification.\n" );


	return DataIdentification;
}




static aafUID_t * get_FileDescriptor_ContainerFormat( AAF_Iface *aafi, aafObject *FileDescriptor )
{
	aafWeakRef_t *ContainerDefWeakRef = aaf_get_propertyValue( FileDescriptor, PID_FileDescriptor_ContainerFormat );

	if ( ContainerDefWeakRef == NULL )
		_warning( "Missing FileDescriptor::ContainerFormat.\n" );


	aafObject *ContainerDefinition = aaf_get_ObjectByWeakRef( aafi->aafd->ContainerDefinition, ContainerDefWeakRef );

	if ( ContainerDefinition == NULL )
		_warning( "Could not retrieve WeakRef from Dictionary::ContainerDefinitions.\n" );


	aafUID_t  *ContainerIdentification = aaf_get_propertyValue( ContainerDefinition, PID_DefinitionObject_Identification );

	if ( ContainerIdentification == NULL )
		_warning( "Missing ContainerDefinition's DefinitionObject::Identification.\n" );


	return ContainerIdentification;
}




static aafUID_t * get_OperationGroup_OperationIdentification( AAF_Iface *aafi, aafObject *OperationGroup )
{
	aafWeakRef_t *OperationDefWeakRef = aaf_get_propertyValue( OperationGroup, PID_OperationGroup_Operation );

	if ( OperationDefWeakRef == NULL )
	{
		_error( "Missing OperationGroup::Operation.\n" );
		return NULL;
	}


	aafObject *OperationDefinition = aaf_get_ObjectByWeakRef( aafi->aafd->OperationDefinition, OperationDefWeakRef );

	if ( OperationDefinition == NULL )
	{
		_error( "Could not retrieve OperationDefinition from dictionary.\n" );
		return NULL;
	}


	aafUID_t *OperationIdentification = aaf_get_propertyValue( OperationDefinition, PID_DefinitionObject_Identification );

	if ( OperationIdentification == NULL )
	{
		_error( "Missing DefinitionObject::Identification.\n" );
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
		_error( "Missing Parameter::Interpolation.\n" );
		return NULL;
	}


	aafObject *InterpolationDefinition = aaf_get_ObjectByWeakRef( aafi->aafd->InterpolationDefinition, InterpolationDefWeakRef );

	if ( InterpolationDefinition == NULL )
	{
		_error( "Could not find InterpolationDefinition.\n" );
		return NULL;
	}


	aafUID_t  *InterpolationIdentification  = aaf_get_propertyValue( InterpolationDefinition, PID_DefinitionObject_Identification );

	if ( InterpolationIdentification == NULL )
	{
		_error( "Missing Parameter DefinitionObject::Identification.\n" );
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

		if ( mobIDCmp( DataMobID, MobID ) )
			break;
	}

	return EssenceData;
}



/* TODO is this SourceMobID or SourceID (masterMobID) ??? */
static aafiAudioEssence * getEssenceBySourceMobID( AAF_Iface *aafi, aafMobID_t *sourceMobID )
{
	aafiAudioEssence * audioEssence = NULL;


	for ( audioEssence = aafi->Audio->Essences; audioEssence != NULL; audioEssence = audioEssence->next )
	{
		if ( mobIDCmp( audioEssence->masterMobID, sourceMobID ) )
			break;
	}


	return audioEssence;
}














/******************************************************************************
                      E s s e n c e D e s c r i p t o r
 ******************************************************************************

                EssenceDescriptor (abs)
				        |
						|--> FileDescriptor (abs)
						|          |
						|          |--> WAVEDescriptor
						|          |--> AIFCDescriptor
						|          `--> SoundDescriptor
						|                     |
						|                     `--> PCMDescriptor
						|
						|--> PhysicalDescriptor
						`--> TapeDescriptor

******************************************************************************
******************************************************************************/

static int parse_EssenceDescriptor( AAF_Iface *aafi, aafObject *EssenceDesc )
{

	if ( auidCmp( EssenceDesc->Class->ID, &AAFClassID_PCMDescriptor ) )
	{

		parse_PCMDescriptor( aafi, EssenceDesc );

	}
	else if ( auidCmp( EssenceDesc->Class->ID, &AAFClassID_WAVEDescriptor ) )
	{

		parse_WAVEDescriptor( aafi, EssenceDesc );

	}
	else if ( auidCmp( EssenceDesc->Class->ID, &AAFClassID_AIFCDescriptor ) )
	{

		parse_AIFCDescriptor( aafi, EssenceDesc );

	}
	else if ( auidCmp( EssenceDesc->Class->ID, &AAFClassID_SoundDescriptor ) )
	{

		trace_obj( aafi, EssenceDesc, ANSI_COLOR_RED );

		_warning( "No support for AAFClassID_SoundDescriptor." );

		// aafUID_t *ContainerFormat = get_FileDescriptor_ContainerFormat( aafi, EssenceDesc );
		// printf("ContainerFormat : %s\n", ContainerToText(ContainerFormat) );

		// aaf_dump_ObjectProperties( aafi->aafd, EssenceDesc );

	}
	else if ( auidCmp( EssenceDesc->Class->ID, &AAFClassID_AES3PCMDescriptor ) )
	{

		/*
		 *	Not described in specs.
		 */

		trace_obj( aafi, EssenceDesc, ANSI_COLOR_RED );

		_warning( "No support for AAFClassID_AES3PCMDescriptor." );

		// aafUID_t *ContainerFormat = get_FileDescriptor_ContainerFormat( aafi, EssenceDesc );
		// printf("ContainerFormat : %s\n", ContainerToText(ContainerFormat) );

		aaf_dump_ObjectProperties( aafi->aafd, EssenceDesc );

	}
	else if ( auidCmp( EssenceDesc->Class->ID, &AAFClassID_MultipleDescriptor ) )
	{

		/*
		 *	A MultipleDescriptor contains a vector of FileDescriptor objects and is
		 *	used when the file source consists of multiple tracks of essence (e.g MXF).
		 *	Each essence track is described by a MobSlots object in the SourceMob and a
		 *	FileDescriptor object. The FileDescriptor is linked to the MobSlot by
		 *	setting the FileDescriptor::LinkedSlotID property equal to the
		 *	MobSlot::SlotID property.
		 */

		trace_obj( aafi, EssenceDesc, ANSI_COLOR_RED );

		_warning( "MultipleDescriptor not supported yet.\n\n" );

		// aafUID_t *ContainerFormat = get_FileDescriptor_ContainerFormat( aafi, EssenceDesc );
		// printf("ContainerFormat : %s\n", ContainerToText(ContainerFormat) );

		aaf_dump_ObjectProperties( aafi->aafd, EssenceDesc );

	}
	else
	{

		trace_obj( aafi, EssenceDesc, ANSI_COLOR_RED );

		_warning( "Unsupported (yet ?) ClassID : %ls\n", ClassIDToText( aafi->aafd, EssenceDesc->Class->ID ) );

		aafUID_t *ContainerFormat = get_FileDescriptor_ContainerFormat( aafi, EssenceDesc );
		printf("ContainerFormat : %ls\n", ContainerToText(ContainerFormat) );

		aaf_dump_ObjectProperties( aafi->aafd, EssenceDesc );

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

	aafObject *Locators = aaf_get_propertyValue( EssenceDesc, PID_EssenceDescriptor_Locator );
	aafObject *Locator  = NULL;

	aaf_foreach_ObjectInSet( &Locator, Locators, NULL )
	{
		parse_Locator( aafi, Locator );
	}


	return 0;
}




static int parse_PCMDescriptor( AAF_Iface *aafi, aafObject *PCMDescriptor )
{
	trace_obj( aafi, PCMDescriptor, ANSI_COLOR_MAGENTA );

	// aafUID_t *ContainerFormat = get_FileDescriptor_ContainerFormat( aafi, PCMDescriptor );
	// printf("ContainerFormat : %s\n", ContainerToText(ContainerFormat) );

	aafiAudioEssence *audioEssence = aafi->ctx.current_audioEssence;

	audioEssence->type = AAFI_TYPE_PCM;


	uint32_t *channels = (uint32_t*)aaf_get_propertyValue( PCMDescriptor, PID_SoundDescriptor_Channels );

	if ( channels == NULL )
	{
		_error( "Missing PCMDescriptor SoundDescriptor::Channels.\n" );
		return -1;
	}

	audioEssence->channels = *channels;



	uint32_t *samplerate = (uint32_t*)aaf_get_propertyValue( PCMDescriptor, PID_FileDescriptor_SampleRate );

	if ( samplerate == NULL )
	{
		_error( "Missing PCMDescriptor FileDescriptor::SampleRate.\n" );
		return -1;
	}

	audioEssence->samplerate = *samplerate;



	uint32_t *samplesize = (uint32_t*)aaf_get_propertyValue( PCMDescriptor, PID_SoundDescriptor_QuantizationBits );

	if ( samplesize == NULL )
	{
		_error( "Missing PCMDescriptor SoundDescriptor::QuantizationBits.\n" );
		return -1;
	}

	audioEssence->samplesize = *samplesize;



	/* TODO parse the rest of the class */

	return 0;
}




static int parse_WAVEDescriptor( AAF_Iface *aafi, aafObject *WAVEDescriptor )
{
	trace_obj( aafi, WAVEDescriptor, ANSI_COLOR_MAGENTA );

	// aafUID_t *ContainerFormat = get_FileDescriptor_ContainerFormat( aafi, WAVEDescriptor );
	// printf("ContainerFormat : %s\n", ContainerToText(ContainerFormat) );

	aafiAudioEssence *audioEssence = aafi->ctx.current_audioEssence;

	audioEssence->type = AAFI_TYPE_WAVE;


	aafProperty *summary = aaf_get_property( WAVEDescriptor, PID_WAVEDescriptor_Summary );

	if ( summary == NULL )
	{
		_error( "Missing WAVEDescriptor::Summary.\n" );
		return -1;
	}

	audioEssence->summary = summary;

	/*
	 *	NOTE : Summary is parsed later in "post-processing" aafi_retrieveData(),
	 *	to be sure clips and essences are linked, so we are able to fallback on
	 *	essence stream in case summary does not contain the full header part.
	 */

	return 0;
}




static int parse_AIFCDescriptor( AAF_Iface *aafi, aafObject *AIFCDescriptor )
{
	trace_obj( aafi, AIFCDescriptor, ANSI_COLOR_MAGENTA );

	// aafUID_t *ContainerFormat = get_FileDescriptor_ContainerFormat( aafi, AIFCDescriptor );
	// printf("ContainerFormat : %s\n", ContainerToText(ContainerFormat) );

	aafiAudioEssence *audioEssence = aafi->ctx.current_audioEssence;

	audioEssence->type = AAFI_TYPE_AIFC;


	aafProperty *summary = aaf_get_property( AIFCDescriptor, PID_AIFCDescriptor_Summary );

	if ( summary == NULL )
	{
		_error( "Missing AIFCDescriptor::Summary.\n" );
		return -1;
	}

	audioEssence->summary = summary;

	/*
	 *	NOTE : Summary is parsed later in "post-processing" aafi_retrieveData(),
	 *	to be sure clips and essences are linked, so we are able to fallback on
	 *	essence stream in case summary does not contain the full header part.
	 */

	return 0;
}








/*
		         Locator (abs)
		            |
		    ,---------------.
		    |               |
      NetworkLocator   TextLocator

*/

static int parse_Locator( AAF_Iface *aafi, aafObject *Locator )
{

	if ( auidCmp( Locator->Class->ID, &AAFClassID_NetworkLocator ) )
	{

		parse_NetworkLocator( aafi, Locator );

	}
	else if ( auidCmp( Locator->Class->ID, &AAFClassID_TextLocator ) )
	{

		/*
		 *	A TextLocator object provides information to the user to help locate the file
		 *	containing the essence or to locate the physical media. The TextLocator is not
		 *	intended for applications to use without user intervention.
		 *
		 *	TODO what to do with those ???
		 *       never encountered anyway..
		 */

		trace_obj( aafi, Locator, ANSI_COLOR_RED );

		wchar_t *name = aaf_get_propertyValueWstr( Locator, PID_TextLocator_Name );

		_warning( "Got an AAFClassID_TextLocator : \"%ls\"\n", name );

		free( name );

	}
	else
	{

		trace_obj( aafi, Locator, ANSI_COLOR_RED );

	}


	return 0;
}




static int parse_NetworkLocator( AAF_Iface *aafi, aafObject *NetworkLocator )
{
	/*
	 *	This holds an URI pointing to the essence file, when it is not embedded.
	 *	However, sometimes it holds an URI to the AAF file itself when essence is
	 *	embedded so it is not a valid way to test if essence is embedded or not.
	 */

	trace_obj( aafi, NetworkLocator, ANSI_COLOR_MAGENTA );


	aafiAudioEssence *audioEssence = aafi->ctx.current_audioEssence;


	// audioEssence->original_file = aaf_get_propertyValueText( NetworkLocator, PID_NetworkLocator_URLString );
	audioEssence->original_file = aaf_get_propertyValueWstr( NetworkLocator, PID_NetworkLocator_URLString );

	if ( audioEssence->original_file == NULL )
		_warning( "Missing NetworkLocator NetworkLocator::URLString.\n" );

	wurl_decode( audioEssence->original_file, audioEssence->original_file );

	return 0;
}








static int retrieve_EssenceData( AAF_Iface *aafi )
{

	aafiAudioEssence *audioEssence = aafi->ctx.current_audioEssence;


	aafObject *EssenceData = get_EssenceData_By_MobID( aafi, audioEssence->sourceMobID );

	if ( EssenceData == NULL )
	{
		/*
		 *	It means essence is not embedded.
		 */

		return -1;
	}

	trace_obj( aafi, EssenceData, ANSI_COLOR_MAGENTA );


	/*
	 *	The EssenceData::Data property has the stored form SF_DATA_STREAM, so
	 *	it holds the name of the Data stream, which should be located at
	 *	/Path/To/EssenceData/DataStream
	 */

	// aafProperty *PropStreamName = aaf_get_property( EssenceData, PID_EssenceData_Data );
    //
	// if ( PropStreamName == NULL )
	// {
	// 	_error( "Missing EssenceData::Data.\n" );
	// 	return -1;
	// }
    //
	// wchar_t StreamName[CFB_NODE_NAME_SZ];
    //
	// w16tow32( StreamName, (uint16_t*)(((unsigned char*)PropStreamName->val)+1), PropStreamName->len-1 );

	wchar_t *StreamName = aaf_get_propertyValueWstr( EssenceData, PID_EssenceData_Data );

	if ( StreamName == NULL )
	{
		_error( "Missing EssenceData::Data.\n" );
		return -1;
	}



	wchar_t DataPath[CFB_PATH_NAME_SZ];

	wchar_t *path = aaf_get_ObjectPath( EssenceData );

	swprintf( DataPath, CFB_PATH_NAME_SZ, L"%ls/%ls", path, StreamName );

	free( StreamName );



	cfbNode *DataNode = cfb_getNodeByPath( aafi->aafd->cfbd, DataPath, 0 );

	if ( DataNode == NULL )
	{
		_error( "Could not retrieve Data stream node %ls.\n", DataPath );
		return -1;
	}

	audioEssence->node = DataNode;

	audioEssence->is_embedded = 1;



	uint64_t dataLen = cfb_getNodeStreamLen( aafi->aafd->cfbd, DataNode );

	if ( dataLen == 0 )
	{
		_warning( "Got 0 Bytes Data stream length.\n" );
		return -1;
	}

	/* NOTE Might be tweaked by parse_audio_summary() */
	audioEssence->length = dataLen;


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

static int parse_Component( AAF_Iface *aafi, aafObject *Component )
{
	if ( auidCmp( Component->Class->ID, &AAFClassID_Transition ) )
	{

		/*
		 *	A Transition between a Filler and a SourceClip sets a Fade In.
		 *	A Transition between a SourceClip and a Filler sets a Fade Out.
		 *	A Transition between two SourceClips sets a Cross-Fade.
		 *
		 *	Since the Transition applies to the elements that suround it in
		 *	the Sequence, the OperationGroup::InputSegments is then left unused.
		 */

		parse_Transition( aafi, Component );

	}
	else
	{

		parse_Segment( aafi, Component );

	}

	return 0;
}




static int parse_Transition( AAF_Iface *aafi, aafObject *Transition )
{

	int64_t *length = aaf_get_propertyValue( Transition, PID_Component_Length );

	if ( length == NULL )
	{
		_error( "Missing Filler Component::Length.\n" );
		return -1;
	}

	aafi->ctx.current_pos += *length;


	aafiTimelineItem *Item  = aafi_newTimelineItem( aafi->ctx.current_track, AAFI_TRANS );


	aafiTransition   *Trans = (aafiTransition*)&Item->data;


	aafi->ctx.current_transition = Trans;


	/* Set transition type */

	if ( Transition->prev != NULL && auidCmp( Transition->prev->Class->ID, &AAFClassID_Filler ) )
	{
		Trans->flags |= AAFI_TRANS_FADE_IN;
	}
	else if ( Transition->next != NULL && auidCmp( Transition->next->Class->ID, &AAFClassID_Filler ) )
	{
		Trans->flags |= AAFI_TRANS_FADE_OUT;
	}
	else if ( Transition->next != NULL && auidCmp( Transition->next->Class->ID, &AAFClassID_Filler ) == 0 &&
	          Transition->prev != NULL && auidCmp( Transition->prev->Class->ID, &AAFClassID_Filler ) == 0 )
	{
		Trans->flags |= AAFI_TRANS_XFADE;
	}
	else
	{

		/* TODO */

	}

	aafPosition_t *cut_point = aaf_get_propertyValue( Transition, PID_Transition_CutPoint );

	if ( cut_point == NULL )
	{
		_error( "Missing Transition::CutPoint.\n" );
		return -1;
	}

	Trans->cut_pt = *cut_point;


	aafObject * OpGroup = aaf_get_propertyValue( Transition, PID_Transition_OperationGroup );

	if ( OpGroup == NULL )
	{
		_error( "Missing Transition::OperationGroup.\n" );
		return -1;
	}


	parse_OperationGroup( aafi, OpGroup );


	return 0;
}




static int parse_Segment( AAF_Iface *aafi, aafObject *Segment )
{

	/* TODO This function is a bit messy.. */


	if ( auidCmp( Segment->Class->ID, &AAFClassID_Sequence ) )
	{

		parse_Sequence( aafi, Segment );

	}
	else if ( auidCmp( Segment->Class->ID, &AAFClassID_SourceClip ) )
	{

		parse_SourceClip( aafi, Segment );

	}
	else if ( auidCmp( Segment->Class->ID, &AAFClassID_Timecode ) )
	{

		/*
		 *	TODO can contain sequence ? other Timecode SMPTE ..
		 */

		parse_Timecode( aafi, Segment );

	}
	else if ( auidCmp( Segment->Class->ID, &AAFClassID_OperationGroup ) )
	{

		if ( auidCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_CompositionMob ) )
		{

			if ( auidCmp( Segment->Parent->Class->ID, &AAFClassID_TimelineMobSlot ) )
			{

				// AAFClassID_TimelineMobSlot > AAFClassID_OperationGroup
				trace_obj( aafi, Segment, ANSI_COLOR_RED );

			}
			else if ( auidCmp( Segment->Parent->Class->ID, &AAFClassID_Sequence ) ||
		              auidCmp( Segment->Parent->Class->ID, &AAFClassID_OperationGroup ) )
			{

				parse_OperationGroup( aafi, Segment );

			}
			else
			{

				trace_obj( aafi, Segment, ANSI_COLOR_RED );

			}

		}
		else if ( auidCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_MasterMob ) )
		{

			/*
			 *	TODO: This was seen in the spec, but never encountered in real world.
			 */

			trace_obj( aafi, Segment, ANSI_COLOR_RED );
			_error( "MobSlot::Segment > OperationGroup Not implemented yet.\n" );
			return -1;

		}
		else
		{

			trace_obj( aafi, Segment, ANSI_COLOR_RED );

		}
	}
	else if ( auidCmp( Segment->Class->ID, &AAFClassID_Filler ) )
	{

		parse_Filler( aafi, Segment );

	}
	else if ( auidCmp( Segment->Class->ID, &AAFClassID_EssenceGroup ) )
	{

		/*
		 *	Should provide support for multiple essences representing the same
		 *	source material with different resolution, compression, codec, etc.
		 *
		 *	TODO To be tested with Avid and rendered effects.
		 */

		trace_obj( aafi, Segment, ANSI_COLOR_RED );
		_error( "MobSlot::Segment > EssenceGroup Not implemented yet.\n" );
		return -1;

	}
	else
	{

		trace_obj( aafi, Segment, ANSI_COLOR_RED );

	}

	return 0;
}




static int parse_Filler( AAF_Iface *aafi, aafObject *Filler )
{
	trace_obj( aafi, Filler, ANSI_COLOR_MAGENTA );


	if ( auidCmp( Filler->Parent->Class->ID, &AAFClassID_TimelineMobSlot ) )
	{
		/*
		 *	Just an empty track.
		 */
	}
	else if ( auidCmp( Filler->Parent->Class->ID, &AAFClassID_Sequence ) )
	{
		/*
		 *	This represents an empty space on the timeline, between two clips
		 *	which is Component::Length long.
		 */

		int64_t *length = (int64_t*)aaf_get_propertyValue( Filler, PID_Component_Length );

		if ( length == NULL )
		{
			_error( "Missing Filler Component::Length.\n" );
			return -1;
		}

		aafi->ctx.current_pos += *length;
	}
	else
	{

		trace_obj( aafi, Filler, ANSI_COLOR_RED );

	}

	return 0;
}




static int parse_Sequence( AAF_Iface *aafi, aafObject *Sequence )
{
	/* Get Sequence's Components */
	aafObject * Components = aaf_get_propertyValue( Sequence, PID_Sequence_Components );
	aafObject * Component  = NULL;

	/* Loop through Sequence's Components */
	aaf_foreach_ObjectInSet( &Component, Components, NULL )
	{
		parse_Component( aafi, Component );
	}

	return 0;
}




static int parse_Timecode( AAF_Iface *aafi, aafObject *Timecode )
{
	trace_obj( aafi, Timecode, ANSI_COLOR_MAGENTA );



	/* TODO allocate in specific function */

	aafiTimecode *tc = calloc( sizeof(aafiTimecode), sizeof(unsigned char) );

	if ( tc == NULL )
	{
		_error( "%s.\n", strerror( errno ) );
		return -1;
	}



	aafPosition_t *tc_start = (int64_t *)aaf_get_propertyValue( Timecode, PID_Timecode_Start );

	if ( tc_start == NULL )
	{
		_error( "Missing Timecode::Start.\n" );
		return -1;
	}


	uint16_t *tc_fps   = (uint16_t*)aaf_get_propertyValue( Timecode, PID_Timecode_FPS   );

	if ( tc_fps == NULL )
	{
		_error( "Missing Timecode::FPS.\n" );
		return -1;
	}


	uint8_t  *tc_drop  = (uint8_t *)aaf_get_propertyValue( Timecode, PID_Timecode_Drop  );

	if ( tc_drop == NULL )
	{
		_error( "Missing Timecode::Drop.\n" );
		return -1;
	}


	aafRational_t *tc_edit_rate = (aafRational_t*)aaf_get_propertyValue( aafi->ctx.MobSlot, PID_TimelineMobSlot_EditRate );

	if ( tc_edit_rate == NULL )
	{
		_error( "Missing Timecode TimelineMobSlot::EditRate.\n" );
		return -1;
	}


	tc->start = *tc_start;
	tc->fps   = *tc_fps;
	tc->drop  = *tc_drop;
	tc->edit_rate = tc_edit_rate;

	aafi->Audio->tc = tc;

	return 0;
}




static int parse_OperationGroup( AAF_Iface *aafi, aafObject *OpGroup )
{

	aafUID_t *OperationIdentification = get_OperationGroup_OperationIdentification( aafi, OpGroup );


	if ( auidCmp( OpGroup->Parent->Class->ID, &AAFClassID_Transition ) )
	{

		aafiTransition *Trans = aafi->ctx.current_transition;


		if ( auidCmp( OperationIdentification, &AAFOperationDef_MonoAudioDissolve ) )
		{
			/*
			 *	Mono Audio Dissolve (Fade, Cross Fade)
			 *
			 *	The same parameter (curve/level) is applied to the outgoing fade on first
			 *	clip (if any) and to the incoming fade on second clip (if any).
			 */

			Trans->flags |= AAFI_TRANS_SINGLE_CURVE;


			aafObject * Param  = aaf_get_propertyValue( OpGroup, PID_OperationGroup_Parameters );
			// aafObject * Param  = NULL;

			if ( Param == NULL )
			{
				trace_obj( aafi, OpGroup, ANSI_COLOR_MAGENTA );

				/* TODO is there realy some transition here ??? (fairlight AAF) */

				_warning( "Missing OperationGroup::Parameters on transition : falling back on default AAFInterpolationDef_Linear.\n" );

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
			else
			{

				/*
				 *	Since this is a Single Parameter, we should have only one Parameter Object
				 *	within the vector. So there's no need to loop through the vector.
				 *	But still, we can have custom objects..
				 */

				if ( Param->Header->_entryCount > 1 )
				{
					_error( "Multiple Parameters in MonoAudioDissolve Transition OperationGroup.\n" );
					return -1;
				}

				// printf( " Count of Params : %u\n", Params->Header->_entryCount );
	            //
				// aaf_foreach_ObjectInSet( &Param, Params, NULL )
				// {
				// 	aafUID_t *ParamDef = aaf_get_propertyValue( Param, PID_Parameter_Definition );
	            //
				// 	if ( auidCmp( ParamDef, &AAFParameterDef_Level ) )
				// 		break;
				// }
	            //
				// if ( Param == NULL )
				// 	_error( "Could not retrieve Parameter Object.\n" );


				parse_Parameter( aafi, Param );

			}
		}
		else if ( auidCmp( OperationIdentification, &AAFOperationDef_TwoParameterMonoAudioDissolve ) )
		{
			/* Two distinct parameters are used for the outgoing and incoming fades. */

			trace_obj( aafi, OpGroup , ANSI_COLOR_RED);
			_warning( "AAFOperationDef_TwoParameterMonoAudioDissolve not supported yet.\n" );

			// Trans->flags |= AAFI_TRANS_TWO_CURVE;
		}
		else if ( auidCmp( OperationIdentification, &AAFOperationDef_StereoAudioDissolve ) )
		{
			/* 	TODO Unknown usage and implementation */

			trace_obj( aafi, OpGroup, ANSI_COLOR_RED );
			_warning( "AAFOperationDef_StereoAudioDissolve not supported yet.\n" );
		}
		else
		{
			trace_obj( aafi, OpGroup, ANSI_COLOR_RED );
		}

	}
	else if ( auidCmp( OperationIdentification, &AAFOperationDef_AudioChannelCombiner ) )
	{
		trace_obj( aafi, OpGroup, ANSI_COLOR_MAGENTA );

		/**************************************************************************************/

		aafi->ctx.current_track_is_multichannel = 1;
		aafi->ctx.current_multichannel_track_channel = 0;


		aafObject *InputSegments = aaf_get_propertyValue( OpGroup, PID_OperationGroup_InputSegments );
		aafObject *InputSegment  = NULL;

		aaf_foreach_ObjectInSet( &InputSegment, InputSegments, NULL )
		{
			parse_Segment( aafi, InputSegment );
			aafi->ctx.current_multichannel_track_channel++;
		}


		/*
		 *	Sets the track format.
		 */

		if ( aafi->ctx.current_multichannel_track_channel == 2 )
		{
			aafi->ctx.current_track->format = AAFI_TRACK_FORMAT_STEREO;
		}
		else if ( aafi->ctx.current_multichannel_track_channel == 6 )
		{
			aafi->ctx.current_track->format = AAFI_TRACK_FORMAT_5_1;
		}
		else if ( aafi->ctx.current_multichannel_track_channel == 8 )
		{
			aafi->ctx.current_track->format = AAFI_TRACK_FORMAT_7_1;
		}
		else
		{
			/* TODO What to do then ? */
			_warning( "Unknown track format (%u).\n", aafi->ctx.current_multichannel_track_channel );
		}


		/*
		 *	Update the current position.
		 */

		aafi->ctx.current_pos += aafi->ctx.current_multichannel_track_clip_length;


		/*
		 *	Reset multichannel track context.
		 */

		aafi->ctx.current_multichannel_track_channel = 0;
		aafi->ctx.current_track_is_multichannel = 0;

		/**************************************************************************************/

		// return;
	}
	else if ( auidCmp( OperationIdentification, &AAFOperationDef_MonoAudioGain ) )
	{

		aafObject *Parameters = aaf_get_propertyValue( OpGroup, PID_OperationGroup_Parameters );
		aafObject *Parameter  = NULL;

		if ( Parameters == NULL )
		{
			_error( "Could not retrieve Amplitude Parameter.\n" );
			return -1;
		}

		/*
		 *	We have to loop because of custom Parameters.
		 *	Seen in AVID Media Composer AAFs (test.aaf). TODO ParamDef PanVol_IsTrimGainEffect ?
		 */

		// printf("_entryCount : %u\n", Parameters->Header->_entryCount );

		aaf_foreach_ObjectInSet( &Parameter, Parameters, NULL )
		{

			parse_Parameter( aafi, Parameter );

		}

	}
	else if ( auidCmp( OperationIdentification, &AAFOperationDef_StereoAudioGain ) )
	{

		/* TODO Unknown usage and implementation */

		trace_obj( aafi, OpGroup, ANSI_COLOR_RED );
		_warning( "AAFOperationDef_StereoAudioGain not supported yet.\n" );

	}
	else if ( auidCmp( OperationIdentification, &AAFOperationDef_MonoAudioPan ) )
	{
		/* TODO Should Only be Track-based (first Segment of TimelineMobSlot.) */

		trace_obj( aafi, OpGroup, ANSI_COLOR_RED );
		_warning( "AAFOperationDef_MonoAudioPan not supported yet.\n" );

	}
	else if ( auidCmp( OperationIdentification, &AAFOperationDef_MonoAudioMixdown ) )
	{

		/* TODO Unknown usage and implementation */

		trace_obj( aafi, OpGroup, ANSI_COLOR_RED );
		_warning( "AAFOperationDef_MonoAudioMixdown not supported yet.\n" );

	}
	else
	{
		trace_obj( aafi, OpGroup, ANSI_COLOR_RED );
	}



	/*
	 *	Parses Segments in the OperationGroup::InputSegments, only if
	 *	OperationGroup is not a Transition as a Transition has no InputSegments,
	 *	and not an AudioChannelCombiner as they were already parsed.
	 */

	if ( auidCmp( OpGroup->Parent->Class->ID, &AAFClassID_Transition ) == 0 &&
		 auidCmp( OperationIdentification, &AAFOperationDef_AudioChannelCombiner ) == 0 )
	{

		aafObject *InputSegments = aaf_get_propertyValue( OpGroup, PID_OperationGroup_InputSegments );
		aafObject *InputSegment  = NULL;

		aaf_foreach_ObjectInSet( &InputSegment, InputSegments, NULL )
		{

			parse_Segment( aafi, InputSegment );

		}

		aafi->ctx.current_gain = NULL;
	}

	return 0;
}



/* TODO better error returning */

static void * parse_SourceClip( AAF_Iface *aafi, aafObject *SourceClip )
{
	trace_obj( aafi, SourceClip, ANSI_COLOR_MAGENTA );


	/*** Clip ***/

	if ( auidCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_CompositionMob ) )
	{

		int64_t *length = (int64_t*)aaf_get_propertyValue( SourceClip, PID_Component_Length );

		if ( length == NULL )
		{
			_error( "Missing SourceClip Component::Length.\n" );
			return NULL;
		}

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
		// 	_warning( "Got a 1 EU length clip, probably some NLE compatibility padding : Skipping.\n" );
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
		// 	return NULL;
		// }



		aafiTimelineItem *item  = aafi_newTimelineItem( aafi->ctx.current_track, AAFI_CLIP );

		aafiAudioClip    *audioClip = (aafiAudioClip*)&item->data;


		audioClip->gain = aafi->ctx.current_gain;

		audioClip->pos  = aafi->ctx.current_pos;

		audioClip->len  = *length;


		int64_t *startTime = (int64_t*)aaf_get_propertyValue( SourceClip, PID_SourceClip_StartTime );

		if ( startTime == NULL )
		{
			_error( "Missing SourceClip::StartTime.\n" );
			return NULL;
		}

		audioClip->essence_offset = *startTime;


		/*
		 *	p.49 : To create a SourceReference that refers to a MobSlot within
		 *	the same Mob as the SourceReference, omit the SourceID property.
		 *
		 *	NOTE: This should not happen here because The "CompositionMob > SourceClip::SourceID"
		 *	should always point to the corresponding "MasterMob", that is a different Mob.
		 */

		audioClip->masterMobID = (aafMobID_t*)aaf_get_propertyValue( SourceClip, PID_SourceReference_SourceID );

		if ( audioClip->masterMobID == NULL )
		{
			aafObject *Mob = NULL;

			for ( Mob = SourceClip; Mob != NULL; Mob = Mob->Parent )
			{
				if ( auidCmp( Mob->Class->ID, &AAFClassID_CompositionMob ) )
					break;
			}

			audioClip->masterMobID = aaf_get_propertyValue( Mob, PID_Mob_MobID );

			_warning( "Missing SourceReference::SourceID, retrieving from parent Mob.\n" );
		}




		// aafObject *mob = aaf_get_MobByID( aafi->aafd->Mobs, audioClip->masterMobID );
		// // aafSlotID_t *SlotID = aaf_get_propertyValue( SourceClip, PID_SourceReference_SourceMobSlotID );
		// // aafObject *MobSlots = aaf_get_propertyValue( mob, PID_Mob_Slots );
        // //
		// printf("\n:::: Clip ::::\n");
		// // aaf_dump_ObjectProperties( aafi->aafd, MobSlot );
		// aaf_dump_ObjectProperties(SourceClip);
		// printf("SourceMobSlotID : %u\n", *(uint32_t*)aaf_get_propertyValue( SourceClip, PID_SourceReference_SourceMobSlotID ) );
		// aafObject *Slots =  aaf_get_propertyValue( mob, PID_Mob_Slots );
        //
		// aafSlotID_t *SlotID = aaf_get_propertyValue( Slots, PID_MobSlot_SlotID );
        //
		// aafObject *MobSlot2 = aaf_get_MobSlotBySlotID( mob, *SlotID );
		// aafSlotID_t *SlotID2 =  aaf_get_propertyValue( MobSlot2, PID_MobSlot_SlotID );
		// if ( SlotID2 )
		// {
		// 	printf("      MobSlotID2 : %u\n", (*SlotID2) );
		// }
		// printf("      MobSlotID : %u\n", (*SlotID) );
        //
		// printf("\n\n");




		/* link the clip with the essence, if the essence was already parsed. */

		audioClip->Essence = getEssenceBySourceMobID( aafi, audioClip->masterMobID );


		if ( aafi->ctx.current_track_is_multichannel == 0 )
		{
			aafi->ctx.current_pos += audioClip->len;
		}
		else
		{
			aafi->ctx.current_multichannel_track_clip_length = audioClip->len;
		}

		return audioClip;

	}

	/*** Essence ***/

	else if ( auidCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_MasterMob ) )
	{
		aafiAudioEssence *audioEssence = aafi_newAudioEssence( aafi );

		aafi->ctx.current_audioEssence = audioEssence;


		audioEssence->file_name = aaf_get_propertyValueWstr( aafi->ctx.Mob, PID_Mob_Name );


		audioEssence->masterMobID = aaf_get_propertyValue( aafi->ctx.Mob, PID_Mob_MobID );

		if ( audioEssence->masterMobID == NULL )
		{
			_error( "Could not retrieve Mob::MobID.\n" );
			return NULL;
		}

		/*
		 *	p.49 : To create a SourceReference that refers to a MobSlot within
		 *	the same Mob as the SourceReference, omit the SourceID property.
		 */

		audioEssence->sourceMobID = aaf_get_propertyValue( SourceClip, PID_SourceReference_SourceID );

		if ( audioEssence->sourceMobID == NULL )
		{
			aafObject *Mob = NULL;

			for ( Mob = SourceClip; Mob != NULL; Mob = Mob->Parent )
			{
				if ( auidCmp( Mob->Class->ID, &AAFClassID_MasterMob ) )
					break;
			}

			audioEssence->sourceMobID = aaf_get_propertyValue( Mob, PID_Mob_MobID );

			_warning( "Could not retrieve SourceReference::SourceID, retrieving from parent Mob.\n" );
		}



		aafObject *SourceMob = aaf_get_MobByID( aafi->aafd->Mobs, audioEssence->sourceMobID );

		if ( SourceMob == NULL )
		{
			_error( "Could not retrieve SourceMob.\n" );
			return NULL;
		}

		audioEssence->SourceMob = SourceMob;



		parse_SourceMob( aafi, SourceMob );



		aafObject *EssenceDesc = aaf_get_propertyValue( SourceMob, PID_SourceMob_EssenceDescription );

		if ( EssenceDesc == NULL )
		{
			_error( "Could not retrieve EssenceDesc.\n" );
			return NULL;
		}


		parse_EssenceDescriptor( aafi, EssenceDesc );



		audioEssence->unique_file_name = build_unique_filename( aafi, audioEssence );



		/* NOTE since multiple clips can point to the same MasterMob, we have to loop. */

		aafiAudioTrack   * audioTrack = NULL;
		aafiTimelineItem * audioItem  = NULL;

		foreach_audioTrack( audioTrack, aafi )
		{
			foreach_audioItem( audioItem, audioTrack )
			{
				if ( audioItem->type != AAFI_CLIP )
				{
					continue;
				}

				aafiAudioClip *audioClip = (aafiAudioClip*)&audioItem->data;

				if ( mobIDCmp( audioClip->masterMobID, audioEssence->masterMobID ) )
				{
					audioClip->Essence = audioEssence;
				}
			}
		}

		return NULL;

	}
	else
	{

		trace_obj( aafi, SourceClip, ANSI_COLOR_RED );

	}

	return NULL;
}








/*
		        Parameter (abs)
		            |
		    ,--------------.
		    |              |
      ConstantValue   VaryingValue


	A Parameter object shall be owned by an OperationGroup object.
*/

static int parse_Parameter( AAF_Iface *aafi, aafObject *Parameter )
{

	if ( auidCmp( Parameter->Class->ID, &AAFClassID_ConstantValue ) )
	{

		parse_ConstantValue( aafi, Parameter );

	}
	else if ( auidCmp( Parameter->Class->ID, &AAFClassID_VaryingValue ) )
	{

		parse_VaryingValue( aafi, Parameter );

	}

	return 0;
}




static int parse_ConstantValue( AAF_Iface *aafi, aafObject *ConstantValue )
{
	aafUID_t *ParamDef = aaf_get_propertyValue( ConstantValue, PID_Parameter_Definition );

	aafUID_t *OperationIdentification = get_OperationGroup_OperationIdentification( aafi, ConstantValue->Parent );


	if ( auidCmp( OperationIdentification, &AAFOperationDef_MonoAudioGain ) &&
         auidCmp( ParamDef, &AAFParameterDef_Amplitude ) )
	{

		trace_obj( aafi, ConstantValue, ANSI_COLOR_MAGENTA );


		if ( aafi->ctx.current_gain == NULL )
		     aafi->ctx.current_gain = calloc( sizeof(aafiAudioGain), sizeof(unsigned char) );


		aafiAudioGain *Gain = aafi->ctx.current_gain;


		aafRational_t *multiplier = aaf_get_propertyIndirectValue( ConstantValue, PID_ConstantValue_Value );

		if ( multiplier == NULL )
		{
			_error( "Could not retrieve Constant Value.\n" );
			return -1;
		}


		Gain->value    = calloc( 1, sizeof(aafRational_t*) );
		Gain->pts_cnt  = 1;

		memcpy( &Gain->value[0], multiplier, sizeof(aafRational_t) );

		Gain->flags   |= AAFI_AUDIO_GAIN_CONSTANT;

		// aaf_dump_ObjectProperties( aafi->aafd, ConstantValue );

	}
	else
	{
		/* TODO on pt-ja.aaf -> might be pan or else ??? */

		trace_obj( aafi, ConstantValue, ANSI_COLOR_RED );
		printf("ParamDef %ls (%ls)\n\n", ParameterToText( aafi->aafd, ParamDef ), AUIDToText( ParamDef ) );
		aaf_dump_ObjectProperties( aafi->aafd, ConstantValue );
	}

	return 0;
}




static int parse_VaryingValue( AAF_Iface *aafi, aafObject *VaryingValue )
{
	aafUID_t *ParamDef = aaf_get_propertyValue( VaryingValue, PID_Parameter_Definition );

	aafUID_t *OperationIdentification = get_OperationGroup_OperationIdentification( aafi, VaryingValue->Parent );


	aafUID_t *InterpolationIdentification = get_Parameter_InterpolationIdentification( aafi, VaryingValue );

	aafiInterpolation_e interpolation = 0;

	if ( auidCmp( InterpolationIdentification, &AAFInterpolationDef_None ) )
	{
		interpolation = AAFI_INTERPOL_NONE;
	}
	else if ( auidCmp( InterpolationIdentification, &AAFInterpolationDef_Linear ) )
	{
		interpolation = AAFI_INTERPOL_LINEAR;
	}
	else if ( auidCmp( InterpolationIdentification, &AAFInterpolationDef_Power ) )
	{
		interpolation = AAFI_INTERPOL_POWER;
	}
	else if ( auidCmp( InterpolationIdentification, &AAFInterpolationDef_Constant ) )
	{
		interpolation = AAFI_INTERPOL_CONSTANT;
	}
	else if ( auidCmp( InterpolationIdentification, &AAFInterpolationDef_BSpline ) )
	{
		interpolation = AAFI_INTERPOL_BSPLINE;
	}
	else if ( auidCmp( InterpolationIdentification, &AAFInterpolationDef_Log ) )
	{
		interpolation = AAFI_INTERPOL_LOG;
	}
	else
	{
		/* TODO should fallback to some predefined default */
		_warning( "Unknwon Interpolation.\n" );
	}



	if ( auidCmp( VaryingValue->Parent->Parent->Class->ID, &AAFClassID_Transition ) )
	{

		aafiTransition *Trans = aafi->ctx.current_transition;

		Trans->flags |= interpolation;


		aafObject *Points = aaf_get_propertyValue( VaryingValue, PID_VaryingValue_PointList );

		if ( Points == NULL )
		{
			_error( "Missing VaryingValue::PointList.\n" );
			return -1;
		}

		Trans->pts_cnt_a = retrieve_ControlPoints( aafi, Points, &(Trans->time_a), &(Trans->value_a) );

		// int i = 0;
        //
		// for ( i = 0; i < Trans->pts_cnt_a; i++ )
		// {
		// 	printf("time_%i : %i/%i   value_%i : %i/%i\n", i, Trans->time_a[i].numerator, Trans->time_a[i].denominator, i, Trans->value_a[i].numerator, Trans->value_a[i].denominator  );
		// }
	}
	else if ( auidCmp( OperationIdentification, &AAFOperationDef_MonoAudioGain ) &&
              auidCmp( ParamDef, &AAFParameterDef_Amplitude ) )
	{

		aafObject *Points = aaf_get_propertyValue( VaryingValue, PID_VaryingValue_PointList );

		if ( Points == NULL )
		{

			/*
			 *	Some files like the ProTools and Logic ones break standard by having no PointList entry.
			 */

			trace_obj( aafi, VaryingValue, ANSI_COLOR_YELLOW );

			_warning( "Missing VaryingValue::PointList.\n" );

			return -1;
		}

		if ( aafi->ctx.current_gain == NULL )
			 aafi->ctx.current_gain = calloc( sizeof(aafiAudioGain), sizeof(unsigned char) );

		aafiAudioGain *Gain = aafi->ctx.current_gain;

		Gain->flags |= AAFI_AUDIO_GAIN_VARIABLE;
		Gain->flags |= interpolation;

		Gain->pts_cnt = retrieve_ControlPoints( aafi, Points, &Gain->time, &Gain->value );

		// int i = 0;
        //
		// for ( i = 0; i < Gain->pts_cnt; i++ )
		// {
		// 	printf("time_%i : %i/%i   value_%i : %i/%i\n", i, Gain->time[i].numerator, Gain->time[i].denominator, i, Gain->value[i].numerator, Gain->value[i].denominator  );
		// }
	}

	return 0;
}




static int retrieve_ControlPoints( AAF_Iface *aafi, aafObject *Points, aafRational_t *times[], aafRational_t *values[] )
{
	trace_obj( aafi, Points, ANSI_COLOR_MAGENTA );

	// calm down GCC
	if ( aafi->ctx.Mob ){}

	*times  = calloc( Points->Header->_entryCount, sizeof(aafRational_t) );
	*values = calloc( Points->Header->_entryCount, sizeof(aafRational_t) );


	aafObject *Point  = NULL;

	uint i = 0;

	aaf_foreach_ObjectInSet( &Point, Points, &AAFClassID_ControlPoint )
	{

		aafRational_t *time  = aaf_get_propertyValue( Point, PID_ControlPoint_Time );

		if ( time == NULL )
		{
			_error( "Missing ControlPoint::Time.\n" );
			return -1;
		}


		aafRational_t *value = aaf_get_propertyIndirectValue( Point, PID_ControlPoint_Value );

		if ( value == NULL )
		{
			_error( "Missing ControlPoint::Value.\n" );
			return -1;
		}


		memcpy( (*times+i),  time, sizeof(aafRational_t) );
		memcpy( (*values+i), value, sizeof(aafRational_t) );


		i++;
	}

	if ( Points->Header->_entryCount != i )
	{
		_warning( "Points _entryCount does not match iteration.\n" );

		return i;
	}


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

	if ( auidCmp( Mob->Class->ID, &AAFClassID_CompositionMob ) )
	{
		/*
		 *	Retrieve Composition's Name
		 */

		aafi->compositionName = aaf_get_propertyValueWstr( Mob, PID_Mob_Name );



		/*
		 *	Retrieve Composition's UserComments
		 */

		trace_obj( aafi, Mob, ANSI_COLOR_MAGENTA );

		aafObject *UserComments = aaf_get_propertyValue( Mob, PID_Mob_UserComments );
		aafObject *UserComment  = NULL;


		aaf_foreach_ObjectInSet( &UserComment, UserComments, NULL )
		{

			wchar_t *name   = aaf_get_propertyValueWstr( UserComment, PID_TaggedValue_Name );

			if ( name == NULL )
				_warning( "Missing UserComment TaggedValue::Name.\n" );


			wchar_t *text = aaf_get_propertyIndirectValueWstr( UserComment, PID_TaggedValue_Value );

			if ( text == NULL )
				_warning( "Missing UserComment TaggedValue::Value.\n" );


			aafiUserComment *Comment = aafi_newUserComment( &aafi->Comments );

			Comment->name = name;
			Comment->text = text;

		}

	}
	else if ( auidCmp( Mob->Class->ID, &AAFClassID_MasterMob ) )
	{

		trace_obj( aafi, Mob, ANSI_COLOR_MAGENTA );

	}
	else if ( auidCmp( Mob->Class->ID, &AAFClassID_SourceMob ) )
	{

		trace_obj( aafi, Mob, ANSI_COLOR_MAGENTA );

	}


	/*
	 *	Loops through MobSlots
	 */

	aafObject *MobSlots = aaf_get_propertyValue( aafi->ctx.Mob, PID_Mob_Slots );

	aaf_foreach_ObjectInSet( &(aafi->ctx.MobSlot), MobSlots, NULL )
	{

		parse_MobSlot( aafi, aafi->ctx.MobSlot );

	}


	return 0;
}




static int parse_SourceMob( AAF_Iface *aafi, aafObject *SourceMob )
{

	if ( aafi->ctx.current_audioEssence != NULL )
	{
		aafiAudioEssence *audioEssence = aafi->ctx.current_audioEssence;



		aafMobID_t *MobID = aaf_get_propertyValue( SourceMob, PID_Mob_MobID );

		if ( MobID == NULL )
		{
			_error( "Could not retrieve SourceMob Mob::MobID.\n" );
			return -1;
		}

		memcpy( audioEssence->umid, MobID, sizeof(aafMobID_t) );



		aafTimeStamp_t *CreationTime = aaf_get_propertyValue( SourceMob, PID_Mob_CreationTime );

		if ( CreationTime == NULL )
		{
			_error( "Could not retrieve SourceMob Mob::CreationTime.\n" );
			return -1;
		}

		snprintf( audioEssence->originationDate, sizeof(audioEssence->originationDate), "%04i:%02u:%02u",
			CreationTime->date.year,
			CreationTime->date.month,
			CreationTime->date.day );

		snprintf( audioEssence->originationTime, sizeof(audioEssence->originationTime), "%02u:%02u:%02u",
			CreationTime->time.hour,
			CreationTime->time.minute,
			CreationTime->time.second );

	}


	return 0;
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

static int parse_MobSlot( AAF_Iface *aafi, aafObject *MobSlot )
{

	aafObject *Segment = aaf_get_propertyValue( MobSlot, PID_MobSlot_Segment );

	if ( Segment == NULL )
	{
		_error( "Missing MobSlot::Segment.\n" );
		return -1;
	}


	aafUID_t *DataDefinition = get_Component_DataDefinition( aafi, Segment );

	if ( DataDefinition == NULL )
	{
		_error( "Could not retrieve MobSlot::Segment DataDefinition.\n" );
		return -1;
	}




	if ( auidCmp( MobSlot->Class->ID, &AAFClassID_TimelineMobSlot ) )
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

		if ( auidCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_CompositionMob ) )
		{
			/*
			 *	There should be only one Composition, since a CompositionMob represents the overall
			 *	composition (i.e project). Observations on files confirm that.
			 *
			 *	However, the AAF Edit Protocol says that there could be multiple CompositionMobs
			 *	(Mob::UsageCode TopLevel), containing other CompositionMobs (Mob::UsageCode LowerLevel).
			 *	This was not encountered yet, even on Avid exports with AAF_EditProtocol enabled.
			 */

			if ( auidCmp( DataDefinition, &AAFDataDef_Sound ) ||
				 auidCmp( DataDefinition, &AAFDataDef_LegacySound ) )
			{

				aafiAudioTrack *track = aafi_newAudioTrack( aafi );


				/*
				 *	p.11 : In a CompositionMob or MasterMob, PhysicalTrackNumber is the output channel number that the
				 *	MobSlot should be routed to when played.
				 */

				uint32_t *track_num = (uint32_t*)aaf_get_propertyValue( MobSlot, PID_MobSlot_PhysicalTrackNumber );

				if ( track_num == NULL )
				{
					_warning( "Missing MobSlot::PhysicalTrackNumber.\n" );
				}
				else
				{
					track->number = *track_num;
				}


				track->name = aaf_get_propertyValueWstr( MobSlot, PID_MobSlot_SlotName );


				track->edit_rate = aaf_get_propertyValue( MobSlot, PID_TimelineMobSlot_EditRate );

				if ( track->edit_rate == NULL )
				{
					_error( "Missing MobSlot::PID_TimelineMobSlot_EditRate.\n" );
					return -1;
				}



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


				/* Reset timeline position */
				aafi->ctx.current_pos = 0;

				parse_Segment( aafi, Segment );

			}
			else if ( auidCmp( DataDefinition, &AAFDataDef_Timecode ) ||
					  auidCmp( DataDefinition, &AAFDataDef_LegacyTimecode ) )
			{
				parse_Segment( aafi, Segment );
			}
			else
			{
				trace_obj( aafi, MobSlot, ANSI_COLOR_YELLOW );
				printf( "%ls\n", DataDefToText( aafi->aafd, DataDefinition ) );
			}

		}
		else if ( auidCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_MasterMob ) )
		{
			/* Retrieve Essences */

			if ( auidCmp( DataDefinition, &AAFDataDef_Sound ) ||
				 auidCmp( DataDefinition, &AAFDataDef_LegacySound ) )
			{
				parse_Segment( aafi, Segment );

				retrieve_EssenceData( aafi );
			}
			else
			{
				trace_obj( aafi, MobSlot, ANSI_COLOR_YELLOW );
				printf( "%ls\n", DataDefToText( aafi->aafd, DataDefinition ) );
			}
		}
		else if ( auidCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_SourceMob ) )
		{

			if ( aafi->ctx.current_audioEssence != NULL )
			{
				aafiAudioEssence *audioEssence = aafi->ctx.current_audioEssence;

				aafPosition_t *Origin = aaf_get_propertyValue( MobSlot, PID_TimelineMobSlot_Origin );

				if ( DataDefinition == NULL )
				{
					_error( "Could not retrieve MobSlot TimelineMobSlot::Origin.\n" );
					return -1;
				}

				audioEssence->timeReference = *Origin;
			}

		}
		else
		{
			trace_obj( aafi, MobSlot, ANSI_COLOR_YELLOW );
			printf( "%ls\n", DataDefToText( aafi->aafd, DataDefinition ) );
		}

	}
	else
	{
		trace_obj( aafi, MobSlot, ANSI_COLOR_YELLOW );
		printf( "%ls\n", DataDefToText( aafi->aafd, DataDefinition ) );
	}

	return 0;
}




int aafi_retrieveData( AAF_Iface *aafi )
{

	aaf_foreach_ObjectInSet( &(aafi->ctx.Mob), aafi->aafd->Mobs, NULL )
	{

		RESET_CONTEXT( aafi->ctx );

		parse_Mob( aafi, aafi->ctx.Mob );

	}


	/*
	 */
	aaf_foreach_ObjectInSet( &(aafi->ctx.Mob), aafi->aafd->Mobs, &AAFClassID_SourceMob )
	{

		aafObject *MobSlots = aaf_get_propertyValue( aafi->ctx.Mob, PID_Mob_Slots );

		aaf_foreach_ObjectInSet( &(aafi->ctx.MobSlot), MobSlots, NULL )
		{

			/*
			 *	Check if the SourceMob was parsed.
			 *	If it was not, we can print the trace.
			 *
			 *	NOTE We do it after the main loop, so we make sure all MasterMobs was parsed.
			 */

			aafObject *Segment = aaf_get_propertyValue( aafi->ctx.MobSlot, PID_MobSlot_Segment );

 			aafUID_t  *DataDefinition = get_Component_DataDefinition( aafi, Segment );

			aafMobID_t *MobID = aaf_get_propertyValue( aafi->ctx.Mob, PID_Mob_MobID );

			aafiAudioEssence *audioEssence = NULL;

			foreachAudioEssence( audioEssence, aafi->Audio->Essences )
			{
				if ( mobIDCmp( MobID, audioEssence->sourceMobID ) )
					break;
			}

			if ( audioEssence == NULL )
			{
				trace_obj( aafi, aafi->ctx.MobSlot, ANSI_COLOR_YELLOW );
				printf( "%ls\n", DataDefToText( aafi->aafd, DataDefinition ) );
			}

		}
	}



	/* *** Post processing *** */


	/* Parse summary descriptor (WAVE/AIFC) if any */

	aafiAudioEssence *audioEssence = NULL;

	foreachAudioEssence( audioEssence, aafi->Audio->Essences )
	{
		if ( audioEssence->summary != NULL )
		{
			parse_audio_summary( aafi, audioEssence );
		}
	}


	/* Remove sample accurate edit clips and rebuild true fades out of rendered fade clips */

	if ( wcscmp( aafi->aafd->Identification.ProductName, L"ProTools" ) == 0 )
	{
		protools_post_processing( aafi, (PROTOOLS_PP_REMOVE_SAMPLE_ACCURATE_EDIT | PROTOOLS_PP_REPLACE_RENDERED_CLIP_FADES) );
	}

	return 0;
}

/**
 *	@}
 */

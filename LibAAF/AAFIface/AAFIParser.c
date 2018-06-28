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

#include "../libAAF.h"
#include "../common/debug.h"

#include "thirdparty/libriff.h"
#include "thirdparty/libwav.h"







#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[92m"
#define ANSI_COLOR_YELLOW  "\x1b[93m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"



static void trace_obj( AAF_Iface *aafi, aafObject *Obj, char *color );


static aafUID_t  * get_Component_DataDefinition( AAF_Iface *aafi, aafObject *Component );
static aafUID_t  * get_FileDescriptor_ContainerFormat( AAF_Iface *aafi, aafObject *FileDescriptor );
static aafUID_t  * get_OperationGroup_OperationIdentification( AAF_Iface *aafi, aafObject *OperationGroup );
static aafUID_t  * get_Parameter_InterpolationIdentification( AAF_Iface *aafi, aafObject *Parameter );

static aafObject * get_EssenceData_By_MobID( AAF_Iface *aafi, aafMobID_t *MobID );

static aafiAudioEssence * getEssenceBySourceMobID( AAF_Iface *aafi, aafMobID_t *sourceMobID );


static void   parse_EssenceDescriptor( AAF_Iface *aafi, aafObject *EssenceDesc );
static void   parse_PCMDescriptor( AAF_Iface *aafi, aafObject *PCMDescriptor );
static void   parse_WAVEDescriptor( AAF_Iface *aafi, aafObject *WAVEDescriptor );
static void   parse_AIFCDescriptor( AAF_Iface *aafi, aafObject *AIFCDescriptor );

static void   parse_Locator( AAF_Iface *aafi, aafObject *Locator );
static void   parse_NetworkLocator( AAF_Iface *aafi, aafObject *NetworkLocator );

static void   retrieve_EssenceData( AAF_Iface *aafi );


static void   parse_Component( AAF_Iface *aafi, aafObject *Component );
static void   parse_Transition( AAF_Iface *aafi, aafObject *Transition );
static void   parse_Segment( AAF_Iface *aafi, aafObject *Segment );
static void   parse_Filler( AAF_Iface *aafi, aafObject *Filler );
static void   parse_Sequence( AAF_Iface *aafi, aafObject *Sequence );
static void   parse_Timecode( AAF_Iface *aafi, aafObject *Timecode );
static void   parse_OperationGroup( AAF_Iface *aafi, aafObject *OpGroup );
static void * parse_SourceClip( AAF_Iface *aafi, aafObject *SourceClip );


static void   parse_Parameter( AAF_Iface *aafi, aafObject *Parameter );
static void   parse_ConstantValue( AAF_Iface *aafi, aafObject *ConstantValue );
static void   parse_VaryingValue( AAF_Iface *aafi, aafObject *VaryingValue );
static int    retrieve_ControlPoints( AAF_Iface *aafi, aafObject *Points, aafRational_t *times[], aafRational_t *values[] );









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
			char *name = aaf_get_propertyValueText( Obj, PID_MobSlot_SlotName );
			snprintf( buf, 1024, "%s (%s) > %s", ClassIDToText( aafi->aafd, Obj->Class->ID ), name, tmp );
			free( name );
		}
		else if ( auidCmp( Obj->Class->ID, &AAFClassID_MasterMob ) || auidCmp( Obj->Class->ID, &AAFClassID_SourceMob ) )
		{
			char *name  = aaf_get_propertyValueText( Obj, PID_Mob_Name );
			snprintf( buf, 1024, "%s (%s) > %s", ClassIDToText( aafi->aafd, Obj->Class->ID ), name, tmp );
			free( name );
		}
		else if ( auidCmp( Obj->Class->ID, &AAFClassID_CompositionMob ) )
		{
			char *name = aaf_get_propertyValueText( Obj, PID_Mob_Name );
			const char *usage = UsageCodeToText( aaf_get_propertyValue( Obj, PID_Mob_UsageCode ) );
			snprintf( buf, 1024, "%s (%s : %s) > %s", ClassIDToText( aafi->aafd, Obj->Class->ID ), usage, name, tmp );
			free( name );
		}
		else if ( auidCmp( Obj->Class->ID, &AAFClassID_OperationGroup ) )
		{
			aafUID_t *OperationIdentification = get_OperationGroup_OperationIdentification( aafi, Obj );

			const char *name = OperationDefToText( aafi->aafd, OperationIdentification ) /*printUID( OpIdent )*/;
			snprintf( buf, 1024, "%s (%s) > %s", ClassIDToText( aafi->aafd, Obj->Class->ID ), name, tmp );
		}
		else
		{
			snprintf( buf, 1024, "%s > %s", ClassIDToText( aafi->aafd, Obj->Class->ID ), tmp );
		}
	}

	buf[strlen(buf)-2] = 0x00;

	printf("%s%s%s\n", color, buf, ANSI_COLOR_RESET );
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
		_fatal( "Missing OperationGroup::Operation.\n" );


	aafObject *OperationDefinition = aaf_get_ObjectByWeakRef( aafi->aafd->OperationDefinition, OperationDefWeakRef );

	if ( OperationDefinition == NULL )
		_fatal( "Could not retrieve OperationDefinition from dictionary.\n" );


	aafUID_t *OperationIdentification = aaf_get_propertyValue( OperationDefinition, PID_DefinitionObject_Identification );

	if ( OperationIdentification == NULL )
		_fatal( "Missing DefinitionObject::Identification.\n" );


	return OperationIdentification;
}



/* TODO not parameter ? VaryingValue ? */
static aafUID_t * get_Parameter_InterpolationIdentification( AAF_Iface *aafi, aafObject *Parameter )
{
	aafWeakRef_t *InterpolationDefWeakRef = aaf_get_propertyValue( Parameter, PID_VaryingValue_Interpolation );

	if ( InterpolationDefWeakRef == NULL )
		_fatal( "Missing Parameter::Interpolation.\n" );


	aafObject *InterpolationDefinition = aaf_get_ObjectByWeakRef( aafi->aafd->InterpolationDefinition, InterpolationDefWeakRef );

	if ( InterpolationDefinition == NULL )
		_fatal( "Could not find InterpolationDefinition.\n" );


	aafUID_t  *InterpolationIdentification  = aaf_get_propertyValue( InterpolationDefinition, PID_DefinitionObject_Identification );

	if ( InterpolationIdentification == NULL )
		_fatal( "Missing Parameter DefinitionObject::Identification.\n" );


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
















inline uint16_t Reverse16( uint16_t value )
{
    return (((value & 0x00FF) << 8) |
            ((value & 0xFF00) >> 8));
}

typedef struct AIFFExtFloat{
  uint8_t exponent[2];
  uint8_t mantissa[8];
}AIFFExtFloat;

static uint64_t getAIFCSampleRate( unsigned char *buf )
{
  AIFFExtFloat ext;
  uint64_t m = 0;
  int32_t e, i;

  memcpy( &ext, buf, 10 );

  // fread(&ext, 1, 10, fp);

  for(i=0; i<8; i++)
    m = (m<<8) + ext.mantissa[i];

  e = (((int32_t)ext.exponent[0]&0x7F)<<8) | ext.exponent[1];

  if(e == 0x7FFF && m)
    return 0.0;

  e -= 16383 + 63;

  if(ext.exponent[0] & 0x80)
    m = -m;

  if(e > 0)
    m <<= e;
  else if(e < 0)
    m >>= -e;

  return m;
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

static void parse_EssenceDescriptor( AAF_Iface *aafi, aafObject *EssenceDesc )
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

		// printObjectProperties( aafi->aafd, EssenceDesc );

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

		printObjectProperties( aafi->aafd, EssenceDesc );

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

		printObjectProperties( aafi->aafd, EssenceDesc );

	}
	else
	{

		trace_obj( aafi, EssenceDesc, ANSI_COLOR_RED );

		_warning( "Unsupported (yet ?) ClassID : %s\n", ClassIDToText( aafi->aafd, EssenceDesc->Class->ID ) );

		aafUID_t *ContainerFormat = get_FileDescriptor_ContainerFormat( aafi, EssenceDesc );
		printf("ContainerFormat : %s\n", ContainerToText(ContainerFormat) );

		printObjectProperties( aafi->aafd, EssenceDesc );

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

}




static void parse_PCMDescriptor( AAF_Iface *aafi, aafObject *PCMDescriptor )
{
	trace_obj( aafi, PCMDescriptor, ANSI_COLOR_MAGENTA );


	aafiAudioEssence *audioEssence = aafi->ctx.current_audioEssence;

	audioEssence->type            = AAFI_TYPE_PCM;


	audioEssence->wFormatTag      =  0x0001;	// PCM


	uint32_t *nChannels = (uint32_t*)aaf_get_propertyValue( PCMDescriptor, PID_SoundDescriptor_Channels );

	if ( nChannels == NULL )
		_fatal( "Missing PCMDescriptor SoundDescriptor::Channels.\n" );

	audioEssence->nChannels       = *nChannels;


	uint32_t *nSamplesPerSec = (uint32_t*)aaf_get_propertyValue( PCMDescriptor, PID_FileDescriptor_SampleRate );

	if ( nSamplesPerSec == NULL )
		_fatal( "Missing PCMDescriptor FileDescriptor::SampleRate.\n" );

	audioEssence->nSamplesPerSec  = *nSamplesPerSec;


	uint32_t *nAvgBytesPerSec = (uint32_t*)aaf_get_propertyValue( PCMDescriptor, PID_PCMDescriptor_AverageBPS );

	if ( nAvgBytesPerSec == NULL )
		_fatal( "Missing PCMDescriptor PCMDescriptor::AverageBPS.\n" );

	audioEssence->nAvgBytesPerSec = *nAvgBytesPerSec;


	uint32_t *nBlockAlign = (uint32_t*)aaf_get_propertyValue( PCMDescriptor, PID_PCMDescriptor_BlockAlign );

	if ( nBlockAlign == NULL )
		_fatal( "Missing PCMDescriptor PCMDescriptor::BlockAlign.\n" );

	audioEssence->nBlockAlign     = *nBlockAlign;


	uint32_t *wBitsPerSample = (uint32_t*)aaf_get_propertyValue( PCMDescriptor, PID_SoundDescriptor_QuantizationBits );

	if ( wBitsPerSample == NULL )
		_fatal( "Missing PCMDescriptor SoundDescriptor::QuantizationBits.\n" );

	audioEssence->wBitsPerSample  = *wBitsPerSample;

	/* TODO parse the rest of the class */

	// aafUID_t *ContainerFormat = get_FileDescriptor_ContainerFormat( aafi, PCMDescriptor );

	// printf("ContainerFormat : %s\n", ContainerToText(ContainerFormat) );
}




static void parse_WAVEDescriptor( AAF_Iface *aafi, aafObject *WAVEDescriptor )
{
	trace_obj( aafi, WAVEDescriptor, ANSI_COLOR_MAGENTA );


	aafiAudioEssence *audioEssence = aafi->ctx.current_audioEssence;

	audioEssence->type = AAFI_TYPE_WAVE;


	aafProperty *summary = aaf_get_property( WAVEDescriptor, PID_WAVEDescriptor_Summary );

	if ( summary == NULL )
		_fatal( "Missing WAVEDescriptor::Summary.\n" );

	struct chunk *cklist = get_riff_chunk_list( summary->val, summary->len );

	struct chunk *ckfmt  = get_riff_chunk_by_id( cklist, "fmt " );

	struct fmt   *fmtck  = (struct fmt*)(ckfmt->bytes - RIFF_CK_HEADER_SZ);

	audioEssence->wFormatTag = fmtck->format_tag;
	audioEssence->nChannels  = fmtck->channels;
	audioEssence->nSamplesPerSec = fmtck->samples_per_sec;
	audioEssence->nAvgBytesPerSec = fmtck->avg_bytes_per_sec;
	audioEssence->nBlockAlign = fmtck->block_align;
	audioEssence->wBitsPerSample = fmtck->bits_per_sample;

	free_riff_chunk_list( &cklist );

	/*
	 *	The summary should end with the data chunk header, so the last 4 bytes are for
	 *	data chunk size, that is audio length in bytes.
	 *	NOTE this should match the EssenceData stream size
	 */

	audioEssence->length = *(uint32_t*)(summary->val + (summary->len - 4));


	// aafUID_t *ContainerFormat = get_FileDescriptor_ContainerFormat( aafi, WAVEDescriptor );

	// printf("ContainerFormat : %s\n", ContainerToText(ContainerFormat) );
}




static void parse_AIFCDescriptor( AAF_Iface *aafi, aafObject *AIFCDescriptor )
{
	trace_obj( aafi, AIFCDescriptor, ANSI_COLOR_MAGENTA );


	aafiAudioEssence *audioEssence = aafi->ctx.current_audioEssence;

	audioEssence->type = AAFI_TYPE_AIFC;


	/* TODO This is proof of concept : do it with proper lib */

	aafProperty *summary = aaf_get_property( AIFCDescriptor, PID_AIFCDescriptor_Summary );


	audioEssence->wFormatTag      = 0x0001; // PCM
	audioEssence->nChannels       = Reverse16( *(uint16_t*)(summary->val+20) );
	audioEssence->nSamplesPerSec  = getAIFCSampleRate( (unsigned char*)(summary->val+28) );
	audioEssence->wBitsPerSample  = Reverse16( *(uint16_t*)(summary->val+26) );
	audioEssence->nBlockAlign     = audioEssence->nChannels * audioEssence->wBitsPerSample / 8;
	audioEssence->nAvgBytesPerSec = audioEssence->nSamplesPerSec * audioEssence->nBlockAlign;


	// aafUID_t *ContainerFormat = get_FileDescriptor_ContainerFormat( aafi, AIFCDescriptor );

	// printf("ContainerFormat : %s\n", ContainerToText(ContainerFormat) );
}








/*
		         Locator (abs)
		            |
		    ,---------------.
		    |               |
      NetworkLocator   TextLocator

*/

static void parse_Locator( AAF_Iface *aafi, aafObject *Locator )
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

		_warning( "Got an AAFClassID_TextLocator : \"%s\"\n", aaf_get_propertyValueText( Locator, PID_TextLocator_Name ) );

	}
	else
	{

		trace_obj( aafi, Locator, ANSI_COLOR_RED );

	}

}




static void parse_NetworkLocator( AAF_Iface *aafi, aafObject *NetworkLocator )
{
	/*
	 *	This holds an URI pointing to the essence file, when it is not embedded.
	 *	However, sometimes it holds an URI to the AAF file itself when essence is
	 *	embedded so it is not a valid way to test if essence is embedded or not.
	 */

	trace_obj( aafi, NetworkLocator, ANSI_COLOR_MAGENTA );


	aafiAudioEssence *audioEssence = aafi->ctx.current_audioEssence;


	audioEssence->original_file = aaf_get_propertyValueText( NetworkLocator, PID_NetworkLocator_URLString );

	if ( audioEssence->original_file == NULL )
		_warning( "Missing NetworkLocator NetworkLocator::URLString.\n" );

	url_decode( audioEssence->original_file, audioEssence->original_file );
}








static void retrieve_EssenceData( AAF_Iface *aafi )
{

	aafiAudioEssence *audioEssence = aafi->ctx.current_audioEssence;


	aafObject *EssenceData = get_EssenceData_By_MobID( aafi, audioEssence->sourceMobID );

	if ( EssenceData == NULL )
	{
		/*
		 *	It means essence is not embedded.
		 */

		return;
	}

	trace_obj( aafi, EssenceData, ANSI_COLOR_MAGENTA );


	/*
	 *	The EssenceData::Data property has the stored form SF_DATA_STREAM, so
	 *	it holds the name of the Data stream, which should be located at
	 *	/Path/To/EssenceData/DataStream
	 */

	char *StreamName = aaf_get_propertyValueText( EssenceData, PID_EssenceData_Data );

	if ( StreamName == NULL )
		_fatal( "Missing EssenceData EssenceData::Data.\n" );



	char DataPath[CFB_PATH_NAME_SZ];

	char *path = aaf_get_ObjectPath( EssenceData );

	snprintf( DataPath, CFB_PATH_NAME_SZ, "/%s/%s", path, StreamName );

	free( StreamName );
	free( path );



	cfbNode *DataNode = cfb_getNodeByPath( aafi->aafd->cfbd, DataPath, 0 );

	if ( DataNode == NULL )
		_fatal( "Could not retrieve Data stream node.\n" );

	audioEssence->node = DataNode;



	uint64_t dataLen = cfb_getNodeStreamLen( aafi->aafd->cfbd, DataNode );

	if ( dataLen == 0 )
		_warning( "Got 0 Bytes Data stream length.\n" );

	audioEssence->length = dataLen;
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

static void parse_Component( AAF_Iface *aafi, aafObject *Component )
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
}




static void parse_Transition( AAF_Iface *aafi, aafObject *Transition )
{

	int64_t *length = aaf_get_propertyValue( Transition, PID_Component_Length );

	if ( length == NULL )
		_fatal( "Missing Filler Component::Length.\n" );

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
		_fatal( "Missing Transition::CutPoint.\n" );

	Trans->cut_pt = *cut_point;


	aafObject * OpGroup = aaf_get_propertyValue( Transition, PID_Transition_OperationGroup );

	if ( OpGroup == NULL )
		_fatal( "Missing Transition::OperationGroup.\n" );


	parse_OperationGroup( aafi, OpGroup );
}




static void parse_Segment( AAF_Iface *aafi, aafObject *Segment )
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
			_fatal( "MobSlot::Segment > OperationGroup Not implemented yet.\n" );

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
		 *	TODO Provides support for multiple essences ???
		 */

		trace_obj( aafi, Segment, ANSI_COLOR_RED );
		_fatal( "MobSlot::Segment > EssenceGroup Not implemented yet.\n" );

	}
	else
	{

		trace_obj( aafi, Segment, ANSI_COLOR_RED );

	}

}




static void parse_Filler( AAF_Iface *aafi, aafObject *Filler )
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
			_fatal( "Missing Filler Component::Length.\n" );

		aafi->ctx.current_pos += *length;
	}
	else
	{

		trace_obj( aafi, Filler, ANSI_COLOR_RED );

	}
}




static void parse_Sequence( AAF_Iface *aafi, aafObject *Sequence )
{
	/* Get Sequence's Components */
	aafObject * Components = aaf_get_propertyValue( Sequence, PID_Sequence_Components );
	aafObject * Component  = NULL;

	/* Loop through Sequence's Components */
	aaf_foreach_ObjectInSet( &Component, Components, NULL )
	{
		parse_Component( aafi, Component );
	}
}




static void parse_Timecode( AAF_Iface *aafi, aafObject *Timecode )
{
	trace_obj( aafi, Timecode, ANSI_COLOR_MAGENTA );

	aafiTimecode *tc = calloc( sizeof(aafiTimecode), sizeof(unsigned char) );

	if ( tc == NULL )
		_fatal( "%s.\n", strerror( errno ) );



	int64_t  *tc_start = (int64_t *)aaf_get_propertyValue( Timecode, PID_Timecode_Start );

	if ( tc_start == NULL )
		_fatal( "Missing Timecode::Start.\n" );


	uint16_t *tc_fps   = (uint16_t*)aaf_get_propertyValue( Timecode, PID_Timecode_FPS   );

	if ( tc_fps == NULL )
		_fatal( "Missing Timecode::FPS.\n" );


	uint8_t  *tc_drop  = (uint8_t *)aaf_get_propertyValue( Timecode, PID_Timecode_Drop  );

	if ( tc_drop == NULL )
		_fatal( "Missing Timecode::Drop.\n" );



	tc->start = *tc_start;
	tc->fps   = *tc_fps;
	tc->drop  = *tc_drop;


	aafi->Audio->tc = tc;
}




static void parse_OperationGroup( AAF_Iface *aafi, aafObject *OpGroup )
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
			 *	clip and to the incoming fade on second clip.
			 */

			Trans->flags |= AAFI_TRANS_SINGLE_CURVE;


			aafObject * Param  = aaf_get_propertyValue( OpGroup, PID_OperationGroup_Parameters );
			// aafObject * Param  = NULL;

			if ( Param == NULL )
			{
				trace_obj( aafi, OpGroup, ANSI_COLOR_MAGENTA );

				/* TODO is there realy some transition here ??? (fairlight AAF) */

				_warning( "No PID_OperationGroup_Parameters on transition : falling back on default AAFInterpolationDef_Linear.\n" );

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

				// printf( "\n\nxxx OperationGroup xxx\n" );
				// printObjectProperties( aafi->aafd, OpGroup );
				// printf( "\n\nxxx Transition xxx\n" );
				// printObjectProperties( aafi->aafd, Transition );

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
					_fatal( "Multiple Parameters in MonoAudioDissolve Transition OperationGroup.\n" );
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
				// 	_fatal( "Could not retrieve Parameter Object.\n" );


				parse_Parameter( aafi, Param );

			}
		}
		else if ( auidCmp( OperationIdentification, &AAFOperationDef_TwoParameterMonoAudioDissolve ) )
		{
			/*
			 *	Two Parameters Mono Audio Dissolve
			 *
			 *	Two distinct parameters are used for outgoing and incoming fades.
			 */

			trace_obj( aafi, OpGroup , ANSI_COLOR_RED);
			_warning( "AAFOperationDef_TwoParameterMonoAudioDissolve not supported yet.\n" );

			// Trans->flags |= AAFI_TRANS_TWO_CURVE;
		}
		else if ( auidCmp( OperationIdentification, &AAFOperationDef_StereoAudioDissolve ) )
		{
			/*
			 *	Stereo Audio Dissolve
			 *
			 *	TODO Unknown usage and implementation
			 */

			trace_obj( aafi, OpGroup, ANSI_COLOR_RED );
			_warning( "AAFOperationDef_StereoAudioDissolve not supported yet.\n" );
		}
		else
		{
			trace_obj( aafi, OpGroup, ANSI_COLOR_RED );
		}
	}
	else if ( auidCmp( OperationIdentification, &AAFOperationDef_MonoAudioPan ) )
	{
		/*
		 *	Mono Audio Pan (Track Pan)
		 *
		 *	TODO : Only Track-based
		 */

		trace_obj( aafi, OpGroup, ANSI_COLOR_RED );
		_warning( "AAFOperationDef_MonoAudioPan not supported yet.\n" );

		return;
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

	/* TODO else if () */
	else
	{

		aafi->ctx.current_gain = calloc( sizeof(aafiAudioGain), sizeof(unsigned char) );


		if ( auidCmp( OperationIdentification, &AAFOperationDef_MonoAudioGain ) )
		{

			aafObject *Parameters = aaf_get_propertyValue( OpGroup, PID_OperationGroup_Parameters );
			aafObject *Parameter  = NULL;

			if ( Parameters == NULL )
				_fatal( "Could not retrieve Amplitude Parameter.\n" );

			/*
			 *	Retrieves the Amplitude Parameter
			 *
			 *	We have to loop because of custom Parameters.
			 *	Seen in AVID Media Composer AAFs.
			 */

			// printf("_entryCount : %u\n", Parameters->Header->_entryCount );

			aaf_foreach_ObjectInSet( &Parameter, Parameters, NULL )
			{
				aafUID_t *paramDef = aaf_get_propertyValue( Parameter, PID_Parameter_Definition );

				// printf("ParamDef %s (%s)\n\n", ParameterToText(paramDef), printUID(paramDef) );

				if ( auidCmp( paramDef, &AAFParameterDef_Amplitude ) )
					break;
			}

			if ( Parameter == NULL )
				_fatal( "Could not retrieve Amplitude Parameter.\n" );

			parse_Parameter( aafi, Parameter );

		}
		else if ( auidCmp( OperationIdentification, &AAFOperationDef_MonoAudioMixdown ) )
		{

			/*
			 *	Mono Audio Mixdown
			 *
			 *	TODO Unknown usage and implementation
			 */

			trace_obj( aafi, OpGroup, ANSI_COLOR_RED );
			_warning( "AAFOperationDef_MonoAudioMixdown not supported yet.\n" );

		}
		else if ( auidCmp( OperationIdentification, &AAFOperationDef_StereoAudioGain ) )
		{

			/*
			 *	Stereo Audio Gain
			 *
			 *	TODO Unknown usage and implementation
			 */

			trace_obj( aafi, OpGroup, ANSI_COLOR_RED );
			_warning( "AAFOperationDef_StereoAudioGain not supported yet.\n" );

		}
		else
		{
			trace_obj( aafi, OpGroup, ANSI_COLOR_RED );
			return;
		}


		/*
		 *	Parses each SourceClip in the OperationGroup::InputSegments and apply gain
		 */


		aafObject *InputSegments = aaf_get_propertyValue( OpGroup, PID_OperationGroup_InputSegments );
		aafObject *InputSegment  = NULL;

		// TODO : Shall have one input if MonoAudioGain.

		aaf_foreach_ObjectInSet( &InputSegment, InputSegments, NULL )
		{

			// if ( auidCmp( InputSegment->Class->ID, &AAFClassID_SourceClip) == 0 )
			// {
			// 	trace_obj( aafi, InputSegment, ANSI_COLOR_CYAN );
			// 	continue;
			// }

			// aafiAudioClip *audioClip = (aafiAudioClip*)parse_SourceClip( aafi, InputSegment );
			parse_Segment( aafi, InputSegment );

			// audioClip->gain = aafi->ctx.current_gain;
		}

		aafi->ctx.current_gain = NULL;
	}

}




static void * parse_SourceClip( AAF_Iface *aafi, aafObject *SourceClip )
{
	trace_obj( aafi, SourceClip, ANSI_COLOR_MAGENTA );

	// printObjectProperties( aafi->aafd, SourceClip );

	/*** Clip ***/

	if ( auidCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_CompositionMob ) )
	{

		int64_t *length = (int64_t*)aaf_get_propertyValue( SourceClip, PID_Component_Length );

		if ( length == NULL )
			_fatal( "Missing SourceClip Component::Length.\n" );

		// if ( *length == 1 )
		// {
		// 	/*
		// 	 *	If length equals 1 EditUnit, the clip is probably a padding for "Media Composer Compatibility".
		// 	 *	Therefore, we don't need it.
		// 	 *
		// 	 *	TODO BUT this could also be some rendered fade.. we should find a way to distinguish between the two.
		// 	 */
        //
		// 	// printObjectProperties( aafi->aafd, SourceClip );
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


		audioClip->pos = aafi->ctx.current_pos;


		audioClip->len = *length;


		audioClip->gain = aafi->ctx.current_gain;


		int64_t *startTime = (int64_t*)aaf_get_propertyValue( SourceClip, PID_SourceClip_StartTime );

		if ( startTime == NULL )
			_fatal( "Missing SourceClip::StartTime.\n" );

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
		// // printObjectProperties( aafi->aafd, MobSlot );
		// printObjectProperties(SourceClip);
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


		// aafMobID_t *sourceMobID = (aafMobID_t*)aaf_get_propertyValue( SourceClip, PID_SourceReference_SourceID );
		// aafObject *mob       = aaf_get_MobByID( aafi->aafd->Mobs, sourceMobID );
		// aafSlotID_t *SlotID  = aaf_get_propertyValue( SourceClip, PID_SourceReference_SourceMobSlotID );
		// aafObject *MobSlots  = aaf_get_propertyValue( mob, PID_Mob_Slots );
		// aafObject *MobSlot   = aaf_get_MobSlotBySlotID( MobSlots, *SlotID );
		// printf("\n:::: Essence SourceMob ::::\n");
		// // printObjectProperties( aafi->aafd, MobSlot );
		// printObjectProperties(mob);
		// printf("\n\n");


		audioEssence->masterMobID = aaf_get_propertyValue( aafi->ctx.Mob, PID_Mob_MobID );

		if ( audioEssence->masterMobID == NULL )
			_fatal( "Could not retrieve Mob::MobID.\n" );


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
			_fatal( "Could not retrieve SourceMob.\n" );



		// printf("SourceMob ??? %s\n", ClassIDToText( aafi->aafd, SourceMob->Class->ID ) );
        //
		// aafObject *SourceMobSlots = aaf_get_propertyValue( SourceMob, PID_Mob_Slots );
		// aafObject *SourceMobSlot  = NULL;
        //
		// aaf_foreach_ObjectInSet( &SourceMobSlot, SourceMobSlots, NULL )
		// {
		// 	printf(">>>> SourceMobSlot %s\n", ClassIDToText( aafi->aafd, SourceMobSlot->Class->ID ) );
		// 	printObjectProperties( aafi->aafd, SourceMobSlot );
        //
		// 	// aafObject *Segment = aaf_get_propertyValue( aafi->ctx.MobSlot, PID_MobSlot_Segment );
		// 	// printf("Segment %s\n", ClassIDToText( aafi->aafd, Segment->Class->ID ) );
		// 	// printObjectProperties( aafi->aafd, Segment );
        //
		// 	printf("\n\n");
		// }



		aafObject *EssenceDesc = aaf_get_propertyValue( SourceMob, PID_SourceMob_EssenceDescription );

		if ( EssenceDesc == NULL )
			_fatal( "Could not retrieve EssenceDesc.\n" );





		// aafSlotID_t *SlotID  = aaf_get_propertyValue( SourceClip, PID_SourceReference_SourceMobSlotID );
		// aafObject *MobSlots  = aaf_get_propertyValue( SourceMob, PID_Mob_Slots );
		// aafObject *MobSlot   = aaf_get_MobSlotBySlotID( MobSlots, *SlotID );
		// printf("\n:::: Essence SourceMob ::::\n");
		// // printObjectProperties( aafi->aafd, MobSlot );
		// printObjectProperties(SourceMob);
		// printf("\n\n");
		// printf("EssenceDescriptor : %s\n", ClassIDToText( aafi->aafd, EssenceDesc->Class->ID ) );
		// printObjectProperties(EssenceDesc);
		// printf("\n\n\n\n");




		// printObjectProperties( aafi->aafd, SourceClip );

		parse_EssenceDescriptor( aafi, EssenceDesc );


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

static void parse_Parameter( AAF_Iface *aafi, aafObject *Parameter )
{

	// aafUID_t *paramDef = aaf_get_propertyValue( Parameter, PID_Parameter_Definition );
	// printf("ParamDef : %s\n", ParameterToText(paramDef) );

	if ( auidCmp( Parameter->Class->ID, &AAFClassID_ConstantValue ) )
	{

		parse_ConstantValue( aafi, Parameter );

	}
	else if ( auidCmp( Parameter->Class->ID, &AAFClassID_VaryingValue ) )
	{

		parse_VaryingValue( aafi, Parameter );

	}
}




static void parse_ConstantValue( AAF_Iface *aafi, aafObject *ConstantValue )
{
	aafUID_t *OperationIdentification = get_OperationGroup_OperationIdentification( aafi, ConstantValue->Parent );


	if ( auidCmp( OperationIdentification, &AAFOperationDef_MonoAudioGain ) )
	{

		trace_obj( aafi, ConstantValue, ANSI_COLOR_MAGENTA );

		aafiAudioGain *Gain = aafi->ctx.current_gain;


		Gain->flags   |= AAFI_AUDIO_GAIN_CONSTANT;


		aafRational_t *multiplier = aaf_get_propertyIndirectValue( ConstantValue, PID_ConstantValue_Value );

		if ( multiplier == NULL )
			_fatal( "Could not retrieve Constant Value.\n" );


		Gain->value    = calloc( 1, sizeof(aafRational_t*) );
		Gain->pts_cnt  = 1;

		memcpy( &Gain->value[0], multiplier, sizeof(aafRational_t) );
		// Gain->value[0] = multiplier;

		// printf( "RAW   %i/%i\n", multiplier->numerator, multiplier->denominator );
		// printf( "GAIN  %+05.1lf dB\n", 20 * log10( rationalToFloat( Gain->value[0] ) ) );

	}
}




static void parse_VaryingValue( AAF_Iface *aafi, aafObject *VaryingValue )
{
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
		_warning( "Unknwon Interpolation\n" );
	}



	if ( auidCmp( VaryingValue->Parent->Parent->Class->ID, &AAFClassID_Transition ) )
	{

		aafiTransition *Trans = aafi->ctx.current_transition;

		Trans->flags |= interpolation;


		aafObject *Points = aaf_get_propertyValue( VaryingValue, PID_VaryingValue_PointList );

		if ( Points == NULL )
			_fatal( "Missing VaryingValue::PointList.\n" );

		Trans->pts_cnt_a = retrieve_ControlPoints( aafi, Points, &(Trans->time_a), &(Trans->value_a) );

		// int i = 0;
        //
		// for ( i = 0; i < Trans->pts_cnt_a; i++ )
		// {
		// 	printf("time_%i : %i/%i   value_%i : %i/%i\n", i, Trans->time_a[i].numerator, Trans->time_a[i].denominator, i, Trans->value_a[i].numerator, Trans->value_a[i].denominator  );
		// }
	}
	else if ( auidCmp( OperationIdentification, &AAFOperationDef_MonoAudioGain ) )
	{

		aafObject *Points = aaf_get_propertyValue( VaryingValue, PID_VaryingValue_PointList );

		if ( Points == NULL )
		{

			/*
			 *	Some files like ProTools and Logic break standard by having no PointList entry.
			 */

			trace_obj( aafi, VaryingValue, ANSI_COLOR_YELLOW );

			_warning( "Missing VaryingValue::PointList.\n" );

			free( aafi->ctx.current_gain );
			aafi->ctx.current_gain = NULL;

			return;
		}

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
			_fatal( "Missing ControlPoint::Time.\n" );


		aafRational_t *value = aaf_get_propertyIndirectValue( Point, PID_ControlPoint_Value );

		if ( value == NULL )
			_fatal( "Missing ControlPoint::Value.\n" );


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








int aafi_retrieveData( AAF_Iface *aafi )
{

	/*
	 *	Loop through CompositionMobs
	 *
	 *	There should be only one, since a CompositionMob represents the overall
	 *	composition (i.e project). Observations on files confirm that.
	 *
	 *	However, the AAF Edit Protocol says that there could be multiple CompositionMobs
	 *	(Mob::UsageCode TopLevel), containing other CompositionMobs (Mob::UsageCode LowerLevel).
	 *	This was never encountered for now.
	 *
	 *	TODO implement fallback
	 */

	aaf_foreach_ObjectInSet( &(aafi->ctx.Mob), aafi->aafd->Mobs, NULL )
	{

		if ( auidCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_CompositionMob ) )
		{
			/******************************************************************/

			trace_obj( aafi, aafi->ctx.Mob, ANSI_COLOR_MAGENTA );

			aafObject *UserComments = aaf_get_propertyValue( aafi->ctx.Mob, PID_Mob_UserComments );
			aafObject *UserComment  = NULL;


			aaf_foreach_ObjectInSet( &UserComment, UserComments, NULL )
			{

				char *name   = aaf_get_propertyValueText( UserComment, PID_TaggedValue_Name );

				if ( name == NULL )
					_warning( "Missing UserComment TaggedValue::Name.\n" );


				char *text = aaf_get_propertyIndirectValueText( UserComment, PID_TaggedValue_Value );

				if ( text == NULL )
					_warning( "Missing UserComment TaggedValue::Value.\n" );


				aafiUserComment *Comment = aafi_newUserComment( &aafi->Comments );

				Comment->name = name;
				Comment->text = text;

			}

			/******************************************************************/

			aafi->compositionName = aaf_get_propertyValueText( aafi->ctx.Mob, PID_Mob_Name );
		}


		/* Get MobSlots */
		aafObject *MobSlots = aaf_get_propertyValue( aafi->ctx.Mob, PID_Mob_Slots );


		/* Loop through MobSlots */
		aaf_foreach_ObjectInSet( &(aafi->ctx.MobSlot), MobSlots, NULL )
		{

			aafObject *Segment = aaf_get_propertyValue( aafi->ctx.MobSlot, PID_MobSlot_Segment );

			if ( Segment == NULL )
				_fatal( "Missing MobSlot::Segment.\n" );

			aafUID_t *DataDefinition = get_Component_DataDefinition( aafi, Segment );

			if ( DataDefinition == NULL )
				_fatal( "Could not retrieve MobSlot::Segment DataDefinition.\n" );


			/*
			 *	Each TimelineMobSlot represents a track, either audio or video.
			 *
			 *	The Timeline MobSlot::Segment should hold a Sequence of Components.
			 *	This Sequence represents the timeline track. Therefore, each SourceClip
			 *	contained in the Sequence::Components represents a clip on the timeline.
			 *
			 *	A CompositionMob's SourceClip can be :
			 *
			 *		- directly inside the Sequence::Components.
			 *
			 *		- inside a Sequence::Components > OperationGroup::InputSegments, in
			 *		  which case the OperationGroup's effect (Gain, Pan) applies to the
			 *		  OperationGroup::InputSegments SourceClip(s).
			 *
			 *	CompositionMob can have TimelineMobSlots, StaticMobSlots, EventMobSlots
			 */

			if ( auidCmp( aafi->ctx.MobSlot->Class->ID, &AAFClassID_TimelineMobSlot ) == 0 )
			{
				trace_obj( aafi, aafi->ctx.MobSlot, ANSI_COLOR_YELLOW );
				printf( "%s\n", DataDefToText( DataDefinition ) );

				continue;
			}




			if ( auidCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_CompositionMob ) )
			{
				/* Retrieve Clips and TimeCode */

				if ( auidCmp( DataDefinition, &AAFDataDef_Sound ) ||
					 auidCmp( DataDefinition, &AAFDataDef_LegacySound ) )
				{

					/* TODO rename as aafi_addAudioTrack() */
					aafi_newAudioTrack( aafi, aafi->ctx.MobSlot, -1 );


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


					/* (re)set timeline position */
					aafi->ctx.current_pos = 0;

					parse_Segment( aafi, Segment );

				}
				else if ( auidCmp( DataDefinition, &AAFDataDef_Timecode ) ||
						  auidCmp( DataDefinition, &AAFDataDef_LegacyTimecode ) )
				{
					parse_Segment( aafi, Segment );
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
					trace_obj( aafi, aafi->ctx.MobSlot, ANSI_COLOR_YELLOW );
					printf( "%s\n", DataDefToText( DataDefinition ) );
				}
			}
			else if ( auidCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_SourceMob ) )
			{
				/* See below.. */
			}
			else
			{
				trace_obj( aafi, aafi->ctx.MobSlot, ANSI_COLOR_YELLOW );
				printf( "%s\n", DataDefToText( DataDefinition ) );
			}

		}

	}





	aaf_foreach_ObjectInSet( &(aafi->ctx.Mob), aafi->aafd->Mobs, &AAFClassID_SourceMob )
	{

		aafObject *MobSlots = aaf_get_propertyValue( aafi->ctx.Mob, PID_Mob_Slots );

		aaf_foreach_ObjectInSet( &(aafi->ctx.MobSlot), MobSlots, NULL )
		{

			/*
			 *	Check if the SourceMob was parsed.
			 *	If it was not, we can print the trace.
			 *
			 *	NOTE We do it after the main loop, so we're sure all MasterMobs was parsed.
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
				printf( "%s\n", DataDefToText( DataDefinition ) );
			}

		}
	}

	return 0;
}

/**
 *	@}
 */

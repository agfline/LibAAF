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
 *	@file LibAAF/AAFIface/AAFIface.c
 *	@brief AAF processing
 *	@author Adrien Gesta-Fline
 *	@version 0.1
 *	@date 04 october 2017
 *
 *	@ingroup AAFIface
 *	@addtogroup AAFIface
 *
 *	The AAFIface provides the actual processing of the AAF Objects in order to show
 *	essences and clips in a simplified manner. Indeed, AAF has many different ways to
 *	store data and metadata. Thus, the AAFIface is an abstraction layer that provides
 *	a constant and unique representation method of essences and clips.
 *
 *
 *
 *	@{
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <ctype.h>	// isxdigit()

//#include <math.h>	// log10() for gain calc

#include "../libAAF.h"

#include "AAFIface.h"

#include "thirdparty/libriff.h"
#include "thirdparty/libwav.h"

#include "../common/debug.h"





#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[92m"
#define ANSI_COLOR_YELLOW  "\x1b[93m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"






struct trace_log
{
	char trace[32][256];
	int  trace_loop[32];
	int  trace_levl;
	int  trace_i;
};


#define INIT_TRACE()                                                       \
	struct trace_log *TRACE_LOG = calloc( sizeof(struct trace_log), 1 );   \


#define PUSH_TRACE( ... )                                                  \
	snprintf( TRACE_LOG->trace[TRACE_LOG->trace_levl], 256, __VA_ARGS__ ); \
	TRACE_LOG->trace_loop[TRACE_LOG->trace_levl] = 0;                      \
	TRACE_LOG->trace_levl++;



#define POP_TRACE() \
	TRACE_LOG->trace_levl--;



#define PRINT_TRACE( color, ... )                                                                    \
	PUSH_TRACE( __VA_ARGS__ );                                                                       \
	printf( color );                                                                                 \
	for ( TRACE_LOG->trace_i = 0; TRACE_LOG->trace_i < TRACE_LOG->trace_levl; TRACE_LOG->trace_i++ ) \
		if ( TRACE_LOG->trace_loop[TRACE_LOG->trace_i] == 0 )                                        \
		{                                                                                            \
			if ( TRACE_LOG->trace_i + 1 < TRACE_LOG->trace_levl )                                    \
				printf( "%s > ", TRACE_LOG->trace[TRACE_LOG->trace_i] );                             \
			else                                                                                     \
				printf( "%s", TRACE_LOG->trace[TRACE_LOG->trace_i] );                                \
			TRACE_LOG->trace_loop[TRACE_LOG->trace_i] = 1;                                           \
		}                                                                                            \
		else                                                                                         \
			if ( TRACE_LOG->trace_loop[TRACE_LOG->trace_i + 1] == 1 )                                \
				printf( "%*c   ", (uint32_t)strlen( TRACE_LOG->trace[TRACE_LOG->trace_i] ), ' ' );   \
			else                                                                                     \
				printf( "%*c > ", (uint32_t)strlen( TRACE_LOG->trace[TRACE_LOG->trace_i] ), ' ' );   \
	printf( "\n" ANSI_COLOR_RESET );                                                                 \
	POP_TRACE();


#define END_OF_TRACE() \
	free( TRACE_LOG );


















AAF_Iface * aafi_alloc( AAF_Data *aafd )
{
	AAF_Iface *aafi = calloc( sizeof(AAF_Iface), sizeof(unsigned char) );

	if ( aafi == NULL )
		_fatal( "%s.\n", strerror( errno ) );


	aafi->Audio = calloc( 1, sizeof(aafiAudio) );

	if ( aafi->Audio == NULL )
		_fatal( "%s.\n", strerror( errno ) );

	if ( aafd != NULL )
	{
		aafi->aafd = aafd;
	}
	else
	{
		aafi->aafd = aaf_alloc();
	}

	return aafi;
}






aafiTimelineItem * aafi_newTimelineItem( aafiAudioTrack *track, int itemType )
{

	aafiTimelineItem *item = NULL;

	if ( itemType == AAFI_CLIP )
	{
		item = calloc( sizeof(aafiTimelineItem) + sizeof(aafiAudioClip),  1 );

		if ( item == NULL )
			_fatal( "%s.\n", strerror( errno ) );


		item->type |= AAFI_CLIP;

		aafiAudioClip *aClip = (aafiAudioClip*)&item->data;

		aClip->track = track;
	}
	else if ( itemType == AAFI_TRANS )
	{
		item = calloc( sizeof(aafiTimelineItem) + sizeof(aafiTransition), 1 );

		if ( item == NULL )
			_fatal( "%s.\n", strerror( errno ) );

		item->type |= AAFI_TRANS;
	}



	if ( track != NULL )
	{
		/*
		 *	Add to track's item list
		 */

		if ( track->Items != NULL )
		{
			aafiTimelineItem *tmp = track->Items;

			for (; tmp != NULL; tmp = tmp->next )
				if ( tmp->next == NULL )
					break;

			tmp->next  = item;
			item->prev = tmp;
		}
		else
		{
			track->Items = item;
			item->prev = NULL;
		}
	}


	return item;
}


/*

aafiAudioClip * aafi_newAudioClip( AAF_Iface *aafi, aafiAudioTrack *track )
{

	aafiAudioClip *aClip = calloc( sizeof(aafiAudioClip), sizeof(unsigned char) );

	if ( aClip == NULL )
		_fatal( "%s.\n", strerror( errno ) );

	aClip->track = track;

	aClip->next  = NULL;



	if ( aafi != NULL )
	{

		if ( aafi->Audio->Clips != NULL )
		{
			aafiAudioClip *tmp = aafi->Audio->Clips;

			for (; tmp != NULL; tmp = tmp->next )
				if ( tmp->next == NULL )
					break;

			tmp->next = aClip;
		}
		else
			aafi->Audio->Clips = aClip;
	}



	if ( track != NULL )
	{

		if ( track->Clips != NULL )
		{
			aafiAudioClip *tmp = track->Clips;

			for (; tmp != NULL; tmp = tmp->trackNext )
				if ( tmp->trackNext == NULL )
					break;

			tmp->trackNext = aClip;
		}
		else
			track->Clips = aClip;
	}



	return aClip;
}

*/



aafiAudioTrack * aafi_newAudioTrack( AAF_Iface *aafi, aafObject *MobSlot, uint32_t number )
{
	aafiAudioTrack *track = calloc( sizeof(aafiAudioTrack), sizeof(unsigned char) );

	if ( track == NULL )
		_fatal( "%s.\n", strerror( errno ) );

	if ( MobSlot != NULL )
	{
		uint32_t *track_num = (uint32_t*)aaf_get_propertyValue( MobSlot, PID_MobSlot_PhysicalTrackNumber );

		if ( track_num != NULL )
			track->number = *track_num;

//		if ( track_num == NULL )
//			_fatal( "Missing MobSlot::PhysicalTrackNumber\n" );


		track->name = aaf_get_propertyValueText( MobSlot, PID_MobSlot_SlotName );


		track->edit_rate = aaf_get_propertyValue( MobSlot, PID_TimelineMobSlot_EditRate );

		if ( track->edit_rate == NULL )
			_fatal( "Missing TimelineMobSlot::EditRate\n" );
	}
	else
		track->number = number;



	/*
	 *	Add to track list
	 */

	if ( aafi->Audio->Tracks != NULL )
	{
		aafiAudioTrack *tmp = aafi->Audio->Tracks;

		for (; tmp != NULL; tmp = tmp->next )
			if ( tmp->next == NULL )
				break;

		tmp->next = track;
	}
	else
		aafi->Audio->Tracks = track;


	return track;
}





aafiAudioEssence * aafi_newAudioEssence( AAF_Iface *aafi )
{
	aafiAudioEssence * aEssence = calloc( sizeof(aafiAudioEssence), sizeof(char) );

	if ( aEssence == NULL )
		_fatal( "%s.\n", strerror( errno ) );


	printf( "%p \n", aEssence );
	printf( "%p \n", aafi->Audio );
	printf( "%p \n", aafi->Audio->Essences );

	aEssence->next = aafi->Audio->Essences;
	aafi->Audio->Essences = aEssence;

	return aEssence;
}





aafiEssenceDataNode * aafi_newEssenceDataNode( aafiAudioEssence *aEssence )
{
	aafiEssenceDataNode *dataNode = calloc( sizeof(aafiEssenceDataNode), sizeof(char) );

	if ( dataNode == NULL )
		_fatal( "%s.\n", strerror( errno ) );



	/*
	 *	Add to audio essence's nodes list
	 */

	aafiEssenceDataNode *tmp = aEssence->nodes;

	if ( tmp != NULL )
	{
		for (; tmp != NULL; tmp = tmp->next )
			if ( tmp->next == NULL )
				break;

		tmp->next = dataNode;
	}
	else
		aEssence->nodes = dataNode;


	return dataNode;
}



















aafUID_t * getMobSlotDataDef( aafObject *DataDefinition, aafObject *MobSlot )
{
	aafUID_t *DataDefVal = NULL;

	if ( MobSlot == NULL )
		return NULL;

	aafObject *Segment = aaf_get_propertyValue( MobSlot, PID_MobSlot_Segment );

	if ( Segment )
	{
//		printf( "   : %s\n", ClassIDToText( Segment->Class->ID ) );

		aafWeakRef_t *weakRef = aaf_get_propertyValue( Segment, PID_Component_DataDefinition );

		aafObject *DataDef = aaf_get_ObjectByWeakRef( DataDefinition, weakRef );

		if ( DataDef )
		{
//			printf( "   :: %s\n", ClassIDToText( DataDef->Class->ID ) );

//			aafProperty *DataDefProp = getProperty( DataDef, PID_DefinitionObject_Name );
//			printf( "   :: > %s\n", CFB_utf16Toascii( DataDefProp->val, DataDefProp->len ) );

			DataDefVal = aaf_get_propertyValue( DataDef, PID_DefinitionObject_Identification );
//			printf( "   :: > %s\n", DataDefToText( DataDefVal ) );
//			getPropertyValue( Segment, PID_Component_DataDefinition ); // WeakRef
		}
	}

	return DataDefVal;
}




cfbNode * getEssenceDataStreamNode( AAF_Data *aafd, aafObject *EssenceData )
{

	aafProperty *DataProp = aaf_get_property( EssenceData, PID_EssenceData_Data );


	char DataNodeName[CFB_NODE_NAME_SZ];
	utf16toa( DataNodeName, CFB_NODE_NAME_SZ, DataProp->val, DataProp->len );


	char DataPath[CFB_PATH_NAME_SZ];
	char *path = aaf_get_ObjectPath( EssenceData );

	snprintf( DataPath, CFB_PATH_NAME_SZ, "/%s/%s", path, DataNodeName );

	free( path );

	cfbNode *DataNode = cfb_getNodeByPath( aafd->cfbd, DataPath, 0 );


	return DataNode;
}






aafiTransition * get_fadein( aafiTimelineItem *audioItem )
{

	if ( audioItem->prev != NULL &&
		 audioItem->prev->type & AAFI_TRANS )
	{
		aafiTransition *Trans = (aafiTransition*)(audioItem->prev->data);

		if ( Trans->flags & AAFI_TRANS_FADE_IN )
			return (aafiTransition*)(audioItem->prev->data);
	}

	return NULL;
}




aafiTransition * get_fadeout( aafiTimelineItem *audioItem )
{

	if ( audioItem->next != NULL &&
		 audioItem->next->type & AAFI_TRANS )
	{
		aafiTransition *Trans = (aafiTransition*)(audioItem->next->data);

		if ( Trans->flags & AAFI_TRANS_FADE_OUT )
			return (aafiTransition*)(audioItem->next->data);
	}

	return NULL;
}














char * url_decode( char *dst, char *src )
{
	char a, b;

	while (*src)
	{
		if ( (*src == '%') &&
			 ((a = src[1]) && (b = src[2])) &&
			 (isxdigit(a)  && isxdigit(b)))
		{
			if (a >= 'a')
				a -= 'a'-'A';
			if (a >= 'A')
				a -= ('A' - 10);
			else
				a -= '0';
			if (b >= 'a')
				b -= 'a'-'A';
			if (b >= 'A')
				b -= ('A' - 10);
			else
				b -= '0';
			*dst++ = 16*a+b;
			src+=3;
		}
		else if (*src == '+')
		{
			*dst++ = ' ';
			src++;
		}
		else
		{
			*dst++ = *src++;
		}
	}

	*dst++ = '\0';

	return dst;
}


char * printUID( aafUID_t *auid )
{
	char *buf = malloc( 64 );

	snprintf( buf, 64, "{0x%08x, 0x%04x, 0x%04x, { 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x }}",
		auid->Data1,
		auid->Data2,
		auid->Data3,

		auid->Data4[0],
		auid->Data4[1],
		auid->Data4[2],
		auid->Data4[3],
		auid->Data4[4],
		auid->Data4[5],
		auid->Data4[6],
		auid->Data4[7]
	);

	return buf;
}

char * printMobID( unsigned char *mobid )
{
	char *buf = malloc( 64 );

	uint32_t i = 0;

	for (i = 0; i < sizeof(aafMobID_t); i++ )
		snprintf( buf+i, 64, "%02x", mobid[i] );

	return buf;
}



void printObjectProperties( aafObject *Obj )
{
	aafProperty * Prop = NULL;

	for ( Prop = Obj->Properties;  Prop != NULL; Prop = Prop->next )
	{
		printf( ":.: %s\n", PIDToText( Prop->pid ) );

		// WARNING : Wont print strong references (set/vector) corectly.
		cfb_printStream( Prop->val, Prop->len );
	}
}




int extractAudioEssence( AAF_Iface *aafi, aafiAudioEssence *aafiae, const char *file )
{

	/*
	 *	If the aafiae->file is not NULL, it means that the essence isn't embedded in the
	 *	AAF. The essence file can therefore be retrieved from the URI hold by aafiae->file.
	 */

	if ( aafiae->file != NULL )
		return 1;

	char filePath[1024];

	snprintf( filePath, 1024, "%s.wav", file );

	// printf("%s\n", filePath );

	FILE *fp = fopen( filePath, "w+" );

	if ( fp == NULL )
		_fatal( "%s.\n", strerror(errno) );

	if ( aafiae->type == AAFI_TYPE_PCM )
	{
		/* Write WAVE Header */
		unsigned char header[44];

		header[0]  = 'R';
		header[1]  = 'I';
		header[2]  = 'F';
		header[3]  = 'F';

		*(uint32_t*)(header+4)  = aafiae->length + 36;

		header[8]  = 'W';
		header[9]  = 'A';
		header[10] = 'V';
		header[11] = 'E';

		header[12] = 'f';
		header[13] = 'm';
		header[14] = 't';
		header[15] = ' ';

		*(uint32_t*)(header+16) = 16;
		*(uint16_t*)(header+20) = aafiae->wFormatTag;
		*(uint16_t*)(header+22) = aafiae->nChannels;
		*(uint32_t*)(header+24) = aafiae->nSamplesPerSec;
		*(uint32_t*)(header+28) = aafiae->nAvgBytesPerSec;
		*(uint16_t*)(header+32) = aafiae->nBlockAlign;
		*(uint16_t*)(header+34) = aafiae->wBitsPerSample;

		header[36] = 'd';
		header[37] = 'a';
		header[38] = 't';
		header[39] = 'a';

		*(uint32_t*)(header+40) = (uint32_t)aafiae->length;

		fwrite( header, sizeof(unsigned char), 44, fp );

		fseek( fp, 44, SEEK_SET );
	}


	uint32_t id     = 0;
	size_t   len    = 0;
	uint64_t offset = 0;

	aafByte_t *buf = NULL;

	aafiEssenceDataNode *node = NULL;

	for ( node = aafiae->nodes; node != NULL; node = node->next )
	{
		cfb_foreachSectorInStream( aafi->aafd->cfbd, node->node, &buf, &len, &id )
		{
			offset += fwrite( buf, sizeof(aafByte_t), len, fp );
			fseek( fp, offset, SEEK_SET );
		}

		if ( buf )
			free( buf );
	}

	// save filename
	aafiae->file = malloc( strlen( file ) + 2 );

	if ( aafiae->file == NULL )
		_fatal( "%s.\n", strerror( errno ) );

	snprintf( aafiae->file, strlen( file ) + 1, "%s", file );


	// printf( "WRITING %s\n", filePath );

	fclose( fp );

	return 0;
}




aafObject * getEssenceDataByMobID( aafObject *EssenceData, aafMobID_t *MobID )
{
	aafMobID_t *DataMobID = NULL;

	for (; EssenceData != NULL; EssenceData = EssenceData->next )
	{
		DataMobID = aaf_get_propertyValue( EssenceData, PID_EssenceData_MobID );

		if ( mobIDCmp( DataMobID, MobID ) )
			break;
	}

	return EssenceData;
}




int retrieveEssenceData( AAF_Iface *aafi, aafiAudioEssence *aafiae, aafMobID_t *SourceMobID )
{
	aafiEssenceDataNode *dataNode = aafi_newEssenceDataNode( aafiae );


	/* Get EssenceData Object */
	aafObject *EssenceData = getEssenceDataByMobID( aafi->aafd->EssenceData, SourceMobID );

	if ( EssenceData == NULL )
	{
		_warning( "Could not retrieve EssenceData\n" );

		return 1;
	}
	else
	{
		/* Get EssenceData Stream */
		dataNode->node = getEssenceDataStreamNode( aafi->aafd, EssenceData );

		if ( dataNode->node == NULL )
			_fatal( "Could not retrieve Data Stream Node.\n" );


		/* Retrieve data stream length */
		uint64_t dataLen = getNodeStreamLen( aafi->aafd->cfbd, dataNode->node );

		if ( dataLen == 0 )
			_warning( "Got 0 Bytes data stream length.\n" );


		aafiae->length += dataLen;
	}


	return 0;
}




/* TODO rename to "SourceMob" ? */

int retrieveEssenceDesc( AAF_Iface *aafi, aafMobID_t *SourceMobID, aafMobID_t *MasterMobID )
{
	aafiAudioEssence *aafiae = aafi_newAudioEssence( aafi );

	/*
	 *	Retrieves the SourceMob from the SourceMobID.
	 */

	aafObject *SourceMob = aaf_get_MobByID( aafi->aafd->Mobs, SourceMobID );

	if ( SourceMob == NULL )
		_fatal( "Could not retrieve SourceMob.\n" );



	aafObject *EssenceDesc = aaf_get_propertyValue( SourceMob, PID_SourceMob_EssenceDescription );

	if ( EssenceDesc == NULL )
		_fatal( "Could not retrieve EssenceDesc.\n" );



	memcpy( &aafiae->sourceMobID, SourceMobID, sizeof(aafMobID_t) );
	memcpy( &aafiae->masterMobID, MasterMobID, sizeof(aafMobID_t) );


	if ( auidCmp( EssenceDesc->Class->ID, &AAFClassID_PCMDescriptor ) )
	{
		aafiae->type            = AAFI_TYPE_PCM;

		aafiae->wFormatTag      =  0x0001;	// PCM
		aafiae->nChannels       = *(uint32_t*)aaf_get_propertyValue( EssenceDesc, PID_SoundDescriptor_Channels         );
		aafiae->nSamplesPerSec  = *(uint32_t*)aaf_get_propertyValue( EssenceDesc, PID_FileDescriptor_SampleRate        );
		aafiae->nAvgBytesPerSec = *(uint32_t*)aaf_get_propertyValue( EssenceDesc, PID_PCMDescriptor_AverageBPS         );
		aafiae->nBlockAlign     = *(uint32_t*)aaf_get_propertyValue( EssenceDesc, PID_PCMDescriptor_BlockAlign         );
		aafiae->wBitsPerSample  = *(uint32_t*)aaf_get_propertyValue( EssenceDesc, PID_SoundDescriptor_QuantizationBits );

	}
	else if ( auidCmp( EssenceDesc->Class->ID, &AAFClassID_WAVEDescriptor ) )
	{
		aafiae->type            = AAFI_TYPE_WAVE;

		/** TODO Parse WAVE Summary */

		aafProperty *sum = aaf_get_property( EssenceDesc, PID_WAVEDescriptor_Summary );

		if ( sum == NULL )
			_fatal( "Missing WAVEDescriptor::Summary.\n" );

		struct chunk *cklist = get_riff_chunk_list( sum->val, sum->len );

		struct chunk *ckfmt  = get_riff_chunk_by_id( cklist, "fmt " );

		struct fmt   *fmtck  = (struct fmt*)(ckfmt->bytes - RIFF_CK_HEADER_SZ);

		aafiae->wFormatTag = fmtck->format_tag;
		aafiae->nChannels  = fmtck->channels;
		aafiae->nSamplesPerSec = fmtck->samples_per_sec;
		aafiae->nAvgBytesPerSec = fmtck->avg_bytes_per_sec;
		aafiae->nBlockAlign = fmtck->block_align;
		aafiae->wBitsPerSample = fmtck->bits_per_sample;

		/*
		 * The summary should end with the data chunk header, so the last 4 bytes are for
		 * data chunk size, that is audio length in bytes.
		 */

		aafiae->length = *(uint32_t*)(sum->val + (sum->len - 4));


	}
	else if ( auidCmp( EssenceDesc->Class->ID, &AAFClassID_AIFCDescriptor ) )
	{
		aafiae->type            = AAFI_TYPE_AIFC;

		/** TODO Parse AIFC Summary */

		_fatal( "AIFCDescriptor not supported yet.\n" );

	}
	else if ( auidCmp( EssenceDesc->Class->ID, &AAFClassID_MultipleDescriptor ) )
	{
		/*
		 *	Yet another thing missing from the specs... no, realy. thank you guys.
		 *
		 *	A MultipleDescriptor contains a vector of FileDescriptor objects and is
		 *	used when the file source consists of multiple tracks of essence (e.g MXF).
		 *	Each essence track is described by a MobSlots object in the SourceMob and a
		 *	FileDescriptor object. The FileDescriptor is linked to the MobSlot by
		 *	setting the FileDescriptor::LinkedSlotID property equal to the
		 *	MobSlot::SlotID property.
		 */

		_warning( "MultipleDescriptor not supported yet.\n" );

//		return 1;
	}
	else
	{
		_warning( "Unsupported (yet ?) ClassID : %s\n", ClassIDToText( EssenceDesc->Class->ID ) );

		printObjectProperties( EssenceDesc );

		return 1;
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
p41: 	Absolute Uniform Resource Locator (URL) complying with RFC 1738 or relative
	 *	Uniform Resource Identifier (URI) complying with RFC 2396 for file containing
	 *	the essence. If it is a relative URI, the base URI is determined from the URI
	 *	of the AAF file itself.
	 *	Informative note: A valid URL or URI uses a constrained character set and uses
	 *	the / character as the path separator.
	 */

	aafObject *Locator = aaf_get_propertyValue( EssenceDesc, PID_EssenceDescriptor_Locator );

	if ( Locator != NULL )
	{
		/*
		 *	If there are multiple Locators in Vector. Even though a vector should hold
		 *	multiple objects by definition, it seems in practice that there is only one
		 *	Locator for each EssenceDescriptor. So if we encounter a different case, it
		 *	should be notified for further support.
		 */

		if ( Locator->next != NULL )
		{
			_warning( "Locator has multiple entries : \n" );

			aafObject *Loc = NULL;

			aaf_foreach_ObjectInSet( &Loc, Locator, NULL )
			{

				if ( auidCmp( Loc->Class->ID, &AAFClassID_TextLocator ) )
					printf( "   > %s\n", aaf_get_propertyValueText( Loc, PID_TextLocator_Name ) );

				else if ( auidCmp( Loc->Class->ID, &AAFClassID_NetworkLocator ) )
					printf( "   > %s\n", aaf_get_propertyValueText( Loc, PID_NetworkLocator_URLString ) );

			}

//			return 1;
		}

		/*
		 *	A TextLocator was not encountered in analysed AAF files. So if we do, it
		 *	should be notified for further support.
		 */

		if ( auidCmp( Locator->Class->ID, &AAFClassID_TextLocator ) )
			_warning( "Got a text Locator : \"%s\"\n", aaf_get_propertyValueText( Locator, PID_TextLocator_Name ) );


		/*
		 *	The NetworkLocator holds an URI pointing to the essence file. Later when
		 *	it comes to find the EssenceData, we should first look if the aafiae->file
		 *	is NULL. If it isn't, it means the essence exists in an external file.
		 */

		if ( auidCmp( Locator->Class->ID, &AAFClassID_NetworkLocator ) )
		{
			aafiae->file = aaf_get_propertyValueText( Locator, PID_NetworkLocator_URLString );

			url_decode( aafiae->file, aafiae->file );
		}

	}
	else
	{
		retrieveEssenceData( aafi, aafiae, SourceMobID );
	}


	return 0;
}



int retrieveEssences( AAF_Iface *aafi )
{

	printf( "\n\n"
			"Retrieve Essence Trace _____________________________________________\n" );

	aafObject *Mob = NULL;

	INIT_TRACE();

	/* Loop through MasterMobs */
	aaf_foreach_ObjectInSet( &Mob, aafi->aafd->Mobs, &AAFClassID_MasterMob )
	{

		PUSH_TRACE( "MasterMob::Slots" );

		aafMobID_t *MasterMobID = aaf_get_propertyValue( Mob, PID_Mob_MobID );

		/* Get MasterMob::Slots */
		aafObject  *MobSlots    = aaf_get_propertyValue( Mob, PID_Mob_Slots );
		aafObject  *MobSlot     = NULL;


		/* Loop through MobSlots */
		aaf_foreach_ObjectInSet( &MobSlot, MobSlots, NULL )
		{

			/* Get MobSlot::Segment > Component::DataDefinition */
			aafUID_t *DataDef = getMobSlotDataDef( aafi->aafd->DataDefinition, MobSlot );

			if ( DataDef == NULL )
				_fatal( "Could not retrieve TimelineMobSlot's DataDefinition.\n" );



			if ( auidCmp( MobSlot->Class->ID, &AAFClassID_TimelineMobSlot ) == 0 )
			{

				PRINT_TRACE( ANSI_COLOR_YELLOW, "%s__%s",
					ClassIDToText( MobSlot->Class->ID ),
					DataDefToText( DataDef )
				);

				continue;
			}


			/* We only want audio */
			if ( auidCmp( DataDef, &AAFDataDef_LegacySound ) == 0 &&
				 auidCmp( DataDef, &AAFDataDef_Sound ) == 0 )
			{

				PRINT_TRACE( ANSI_COLOR_YELLOW, "TimelineMobSlot__%s",
					DataDefToText( DataDef )
				);

				continue;
			}


			/* Get MobSlot::Segment */
			aafObject  *Segment     = aaf_get_propertyValue( MobSlot, PID_MobSlot_Segment );

			aafMobID_t *SourceMobID = NULL;


			PUSH_TRACE( "TimelineMobSlot::Segment" );

			/*
			 * If MobSlot::Segment points to a Sequence, then loop
			 * through Sequence::Components Objects to retrieve SourceClips
			 * If MobSlot::Segment points to a Sequence, that is to
			 * a SourceClips list, it means that each SourceClip represents
			 * a part of one essence.
			 */

			if ( auidCmp( Segment->Class->ID, &AAFClassID_Sequence ) )
			{
				aafObject *Components = aaf_get_propertyValue( Segment, PID_Sequence_Components );
				aafObject *Component  = NULL;

				PUSH_TRACE( "Sequence::Components" );

				aaf_foreach_ObjectInSet( &Component, Components, NULL )
				{

					if ( auidCmp( Component->Class->ID, &AAFClassID_SourceClip ) == 0 )
					{
						PRINT_TRACE( ANSI_COLOR_YELLOW, "%s",
							ClassIDToText( Component->Class->ID )
						);

						continue;
					}

					PRINT_TRACE( ANSI_COLOR_GREEN, "SourceClip" );

					/*
					 *	SourceMobID identify the SourceMob and the payload in EssenceData
					 *	FIXME SourceMobID is optional, also we should take SourceMobSlotID
					 *	into serious consideration.
					 */

					SourceMobID = aaf_get_propertyValue( Component, PID_SourceReference_SourceID );

					if ( SourceMobID == NULL )
						_fatal( "Could not retrieve SourceClip::SourceID.\n" );

					retrieveEssenceDesc( aafi, SourceMobID, MasterMobID );

				}

				POP_TRACE();

			}
			else if ( auidCmp( Segment->Class->ID, &AAFClassID_SourceClip ) )
			{

				PRINT_TRACE( ANSI_COLOR_GREEN, "SourceClip" );

				/*
				 *	SourceMobID identify the SourceMob and the payload in EssenceData
				 *	FIXME SourceMobID is optional, also we should take SourceMobSlotID
				 *	into serious consideration for multichannel essences.
				 */

				SourceMobID = aaf_get_propertyValue( Segment, PID_SourceReference_SourceID );

				if ( SourceMobID == NULL )
					_fatal( "Could not retrieve SourceClip::SourceID.\n" );

				retrieveEssenceDesc( aafi, SourceMobID, MasterMobID );

			}
			else if ( auidCmp( Segment->Class->ID, &AAFClassID_EssenceGroup ) )
			{

				/*
				 *	Provides support for multiple e
				 */

				PRINT_TRACE( ANSI_COLOR_RED, "EssenceGroup" );

				_fatal( "MobSlot::Segment > EssenceGroup Not implemented yet.\n" );

			}
			else if ( auidCmp( Segment->Class->ID, &AAFClassID_OperationGroup ) )
			{

				/*
				 *	TODO: This was seen in the spec, but never encountered in real world.
				 */

				PRINT_TRACE( ANSI_COLOR_RED, "OperationGroup" );

				_fatal( "MobSlot::Segment > OperationGroup Not implemented yet.\n" );

			}
			else
			{

				PRINT_TRACE( ANSI_COLOR_RED, "%s", ClassIDToText( Segment->Class->ID ) );

			}


			POP_TRACE();
		}


		POP_TRACE();
	}


	return 0;
}


/* In a Composition Mob, it represents the timecode associated
   with the virtual media represented by the  Composition Mob. */

aafiTimecode * get_CompoMob_Timecode( AAF_Data *aafd, aafObject *CompoMob )
{

	aafiTimecode *tc = NULL;


	aafObject *MobSlots = aaf_get_propertyValue( CompoMob, PID_Mob_Slots );
	aafObject *MobSlot  = NULL;

	aaf_foreach_ObjectInSet( &MobSlot, MobSlots, &AAFClassID_TimelineMobSlot )
	{

		/* Get MobSlot::Segment > Component::DataDefinition */
		aafUID_t *DataDef = getMobSlotDataDef( aafd->DataDefinition, MobSlot );

		if ( DataDef == NULL )
			_fatal( "Could not retrieve MobSlot's DataDefinition.\n" );


		if ( auidCmp( DataDef, &AAFDataDef_Timecode )       == 0 &&
			 auidCmp( DataDef, &AAFDataDef_LegacyTimecode ) == 0 )
		{
			continue;
		}



		aafObject * Segment = aaf_get_propertyValue( MobSlot, PID_MobSlot_Segment );

		if ( Segment == NULL || auidCmp( Segment->Class->ID, &AAFClassID_Timecode ) == 0 )
			continue;



		tc = calloc( sizeof(aafiTimecode), sizeof(unsigned char) );

		if ( tc == NULL )
			_fatal( "%s.\n", strerror( errno ) );



		int64_t  *tc_start = (int64_t *)aaf_get_propertyValue( Segment, PID_Timecode_Start );

		if ( tc_start == NULL )
			_fatal( "Missing Timecode::Start.\n" );


		uint16_t *tc_fps   = (uint16_t*)aaf_get_propertyValue( Segment, PID_Timecode_FPS   );

		if ( tc_fps == NULL )
			_fatal( "Missing Timecode::FPS.\n" );


		uint8_t  *tc_drop  = (uint8_t *)aaf_get_propertyValue( Segment, PID_Timecode_Drop  );

		if ( tc_drop == NULL )
			_fatal( "Missing Timecode::Drop.\n" );


		tc->start = *tc_start;
		tc->fps   = *tc_fps;
		tc->drop  = *tc_drop;

		break;
	}

	return tc;
}







aafiTimecode * retrieve_CompoMob_Timecode( AAF_Iface *aafi, aafObject *Timecode )
{

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


	return aafi->Audio->tc = tc;
}







aafiAudioEssence * getEssenceByMasterMobID( AAF_Iface *aafi, aafMobID_t *SourceID )
{
	aafiAudioEssence * aEssence = NULL;


	for ( aEssence = aafi->Audio->Essences; aEssence != NULL; aEssence = aEssence->next )
	{
		if ( mobIDCmp( &aEssence->masterMobID, SourceID ) )
			break;
	}

	if ( aEssence != NULL )
		 aEssence->subClipCnt++;

	return aEssence;
}




aafiAudioClip * retrieve_CompoMob_SourceClip( AAF_Iface *aafi, aafPosition_t *pos, aafiAudioTrack *track, aafObject *SourceClip )
{

//	aafiAudioClip *aClip = aafi_newAudioClip( aafi, track );

	aafiTimelineItem *item = aafi_newTimelineItem( track, AAFI_CLIP );

	aafiAudioClip *aClip = (aafiAudioClip*)&item->data;

//	aClip->edit_rate  = pos->edit_rate;
	aClip->pos        = *pos;

//	aClip->editRate    = rationalToFloat( track->edit_rate );

//	aClip->timelinePos = pos->cursor;

//	aClip->track = track;



	int64_t *length = (int64_t*)aaf_get_propertyValue( SourceClip, PID_Component_Length );

	if ( length == NULL )
		_fatal( "Missing SourceClip Component::Length !\n" );

//	aClip->length = *length;

	aClip->len = *length;



	int64_t *startTime = (int64_t*)aaf_get_propertyValue( SourceClip, PID_SourceClip_StartTime );

	if ( startTime == NULL )
		_fatal( "Missing SourceClip::StartTime" );

//	aClip->essenceStartOffset = *startTime;
	aClip->essence_offset = *startTime;


	aafMobID_t *SourceID = aaf_get_propertyValue( SourceClip, PID_SourceReference_SourceID );

	if ( SourceID == NULL )
		_fatal( "Missing SourceReference::SourceID\n" );



	aClip->Essence = getEssenceByMasterMobID( aafi, SourceID );

	if ( aClip->Essence == NULL )
		_fatal( "Missing Essence\n" );



	aClip->subClipNum = aClip->Essence->subClipCnt;



	*pos += aClip->len;


	return aClip;
}




aafiAudioGain * retrieveAudioGain( AAF_Iface *aafi, aafObject *OpGroup )
{

	aafiAudioGain *gain   = NULL;

	aafObject *Parameters = aaf_get_propertyValue( OpGroup, PID_OperationGroup_Parameters );
	aafObject *Parameter  = NULL;


	/*
	 *	Retrieves the Amplitude Parameter
	 */

	aaf_foreach_ObjectInSet( &Parameter, Parameters, NULL )
	{
		aafUID_t *paramDef = aaf_get_propertyValue( Parameter, PID_Parameter_Definition );

		if ( auidCmp( paramDef, &AAFParameterDef_Amplitude ) )
			break;
	}

	if ( Parameter == NULL )
		_fatal( "Could not retrieve Amplitude Parameter.\n" );



	if ( auidCmp( Parameter->Class->ID, &AAFClassID_ConstantValue ) )
	{
		/*
		 *	The Gain is a constant value.
		 */

	 	gain           = calloc( sizeof(aafiAudioGain), sizeof(unsigned char) );

		gain->flags   |= AAFI_AUDIO_GAIN_CONSTANT;


		aafRational_t *mul = aaf_get_propertyIndirectValue( Parameter, PID_ConstantValue_Value );

		if ( mul == NULL )
			_fatal( "Could not retrieve Constant Value.\n" );


		gain->value    = calloc( 1, sizeof(aafRational_t*) );

		gain->pts_cnt  = 1;
		gain->value[0] = mul;

	}
	else if ( auidCmp( Parameter->Class->ID, &AAFClassID_VaryingValue ) )
	{
		/*
		 *	The Gain is time-variable.
		 *
		 *	TODO has not been tested yet.
		 */

	 	gain         = calloc( sizeof(aafiAudioGain), sizeof(unsigned char) );

		gain->flags |= AAFI_AUDIO_GAIN_VARIABLE;


		aafWeakRef_t *Inter = aaf_get_propertyValue( Parameter, PID_VaryingValue_Interpolation );

		aafObject *IObj = aaf_get_ObjectByWeakRef( aafi->aafd->InterpolationDefinition, Inter );

		aafUID_t *IIdent = aaf_get_propertyValue( IObj, PID_DefinitionObject_Identification );


		if ( auidCmp( IIdent, &AAFInterpolationDef_None ) )
		{
			_warning( "Interpolation is set to None. Falling back to Linear.\n" );

			gain->flags |= AAFI_INTERPOL_NONE;
		}
		else if ( auidCmp( IIdent, &AAFInterpolationDef_Linear ) )
			gain->flags |= AAFI_INTERPOL_LINEAR;
		else if ( auidCmp( IIdent, &AAFInterpolationDef_Power ) )
			gain->flags |= AAFI_INTERPOL_POWER;
		else if ( auidCmp( IIdent, &AAFInterpolationDef_Constant ) )
			gain->flags |= AAFI_INTERPOL_CONSTANT;
		else if ( auidCmp( IIdent, &AAFInterpolationDef_Log ) )
			gain->flags |= AAFI_INTERPOL_LOG;
		else if ( auidCmp( IIdent, &AAFInterpolationDef_BSpline ) )
			gain->flags |= AAFI_INTERPOL_BSPLINE;



		aafObject *Points = aaf_get_propertyValue( Parameter, PID_VaryingValue_PointList );
		aafObject *Point  = NULL;

		if ( Points == NULL )
			_fatal( "Missing VaryingValue::PointList.\n" );

//		gain->pts_cnt = Points->Header->_entryCount;

		gain->time  = calloc( Points->Header->_entryCount, sizeof(aafRational_t*) );
		gain->value = calloc( Points->Header->_entryCount, sizeof(aafRational_t*) );

		int i = 0;

		aaf_foreach_ObjectInSet( &Point, Points, &AAFClassID_ControlPoint )
		{
			aafRational_t *time  = aaf_get_propertyValue( Point, PID_ControlPoint_Time );

			if ( time == NULL )
				_fatal( "Missing ControlPoint::Time\n" );


			aafRational_t *value = aaf_get_propertyIndirectValue( Point, PID_ControlPoint_Value );

			if ( value == NULL )
				_fatal( "Missing ControlPoint::Value\n" );


			gain->time[i]  = time;
			gain->value[i] = value;

			i++;
		}


		/* set pts_cnt here in case the vector has "custom objects" */
		gain->pts_cnt = i;
	}

	return gain;
}




uint64_t retrieve_CompoMob_OperationGroup( AAF_Iface *aafi, aafPosition_t *pos, aafiAudioTrack *track, aafObject *OpGroup, struct trace_log *TRACE_LOG )
{

	/*
	 *	Should it be recursive ? Or can a SourceClip only have a Gain Effect ?
	 */

	aafiAudioGain *gain = NULL;


	aafWeakRef_t *OpDef = aaf_get_propertyValue( OpGroup, PID_OperationGroup_Operation );

	aafObject *OpDefObj = aaf_get_ObjectByWeakRef( aafi->aafd->OperationDefinition, OpDef );

	aafUID_t  *OpIdent  = aaf_get_propertyValue( OpDefObj, PID_DefinitionObject_Identification );



	if ( auidCmp( OpIdent, &AAFOperationDef_MonoAudioGain ) )
	{
		/*
		 *	Mono Audio Gain (Clip Gain)
		 */

		PUSH_TRACE( "OperationGroup__MonoAudioGain__::InputSegments" );

		gain = retrieveAudioGain( aafi, OpGroup );

	}
//	else if ( auidCmp( OpIdent, &AAFOperationDef_MonoAudioPan ) )
//	{
		/*
		 *	Mono Audio Pan (Track Pan)
		 *
		 *	TODO : Only Track-based
		 */

//		PUSH_TRACE( "OperationGroup__MonoAudioPan__::InputSegments" );
//	}
	else if ( auidCmp( OpIdent, &AAFOperationDef_MonoAudioMixdown ) )
	{
		/*
		 *	Mono Audio Mixdown
		 *
		 *	TODO Unknown usage and implementation
		 */

		PUSH_TRACE( "OperationGroup__MonoAudioMixdown__::InputSegments" );
	}
	else if ( auidCmp( OpIdent, &AAFOperationDef_StereoAudioGain ) )
	{
		/*
		 *	Stereo Audio Gain
		 *
		 *	TODO Unknown usage and implementation
		 */

		PUSH_TRACE( "OperationGroup__StereoAudioGain__::InputSegments" );
	}
	else
	{
		PRINT_TRACE( ANSI_COLOR_RED, "OperationGroup__%s__\n",
			OperationDefToText( OpIdent )
		);

		return 1;
	}



	aafObject *InputSegments = aaf_get_propertyValue( OpGroup, PID_OperationGroup_InputSegments );
	aafObject *InputSegment  = NULL;

	// TODO : Shall have one input if MonoAudioGain.

	aaf_foreach_ObjectInSet( &InputSegment, InputSegments, NULL )
	{

		if ( auidCmp( InputSegment->Class->ID, &AAFClassID_SourceClip) == 0 )
		{
			PRINT_TRACE( ANSI_COLOR_GREEN, "%s",
				ClassIDToText( InputSegment->Class->ID )
			);

			continue;
		}


		PRINT_TRACE( ANSI_COLOR_GREEN, "SourceClip" );

		aafiAudioClip *aClip = retrieve_CompoMob_SourceClip( aafi, pos, track, InputSegment );

		aClip->gain = gain;

	}

	POP_TRACE();

	return 0;
}





void retrieve_CompoMob_Transition( AAF_Iface *aafi, aafiAudioTrack *track, aafObject *Transition, struct trace_log *TRACE_LOG )
{

	aafObject * OpGroup = aaf_get_propertyValue( Transition, PID_Transition_OperationGroup );

	if ( OpGroup == NULL )
		_fatal( "Missing Transition::OperationGroup.\n" );


	aafWeakRef_t *OpDef = aaf_get_propertyValue( OpGroup, PID_OperationGroup_Operation );

	if ( OpDef == NULL )
		_fatal( "Missing OperationGroup::Operation.\n" );


	aafObject *OpDefObj = aaf_get_ObjectByWeakRef( aafi->aafd->OperationDefinition, OpDef );

	if ( OpDefObj == NULL )
		_fatal( "Could not retrieve OperationDefinition from dictionary.\n" );


	aafUID_t *OpIdent = aaf_get_propertyValue( OpDefObj, PID_DefinitionObject_Identification );

	if ( OpIdent == NULL )
		_fatal( "Missing DefinitionObject::Identification.\n" );



	aafiTimelineItem *Item = aafi_newTimelineItem( track, AAFI_TRANS );

	aafiTransition  *Trans = (aafiTransition*)&Item->data;


	if ( Transition->prev != NULL &&
		 auidCmp( Transition->prev->Class->ID, &AAFClassID_Filler ) )
	{
		Trans->flags |= AAFI_TRANS_FADE_IN;
	}
	else if ( Transition->next != NULL &&
			  auidCmp( Transition->next->Class->ID, &AAFClassID_Filler ) )
	{
		Trans->flags |= AAFI_TRANS_FADE_OUT;
	}
	else if ( Transition->next != NULL && Transition->prev != NULL &&
			  auidCmp( Transition->next->Class->ID, &AAFClassID_Filler ) == 0 &&
			  auidCmp( Transition->prev->Class->ID, &AAFClassID_Filler ) == 0 )
	{
		Trans->flags |= AAFI_TRANS_XFADE;
	}


	aafPosition_t *cut_point = aaf_get_propertyValue( Transition, PID_Transition_CutPoint );

	if ( cut_point == NULL )
		_fatal( "Missing Transition::CutPoint.\n" );

	Trans->cut_pt = *cut_point;

//	printf( "Cut Point : %li\n", *cut_point );




	aafObject * Params = aaf_get_propertyValue( OpGroup, PID_OperationGroup_Parameters );
	aafObject * Param  = NULL;

	// is optional
	if ( Params == NULL )
	{
		/*
		 *	TODO What does that mean ? on fade in and fade out Fairlight
		 *	TODO ./TestFiles/2985731\ \ -\ \ OFF\ CIGOGNE\ A\ LIMOGES_WAVE+AVID.aaf
		 *	Look at OperationDefinition::ParametersDefined ???
		 */
		PRINT_TRACE( ANSI_COLOR_GREEN, "Transition::OperationGroup" );
//		printf( "OpIdentification : %s\n", OperationDefToText( OpIdent ) );

		printf( "\n\nxxx OperationGroup xxx\n" );
		printObjectProperties( OpGroup );
		printf( "\n\nxxx Transition xxx\n" );
		printObjectProperties( Transition );
		return;
	}
	else
	{
		PUSH_TRACE( "Transition::OperationGroup" );
	}
//		_fatal( "Missing OperationGroup::Parameters.\n" );


	if ( auidCmp( OpIdent, &AAFOperationDef_MonoAudioDissolve ) )
	{
		/*
		 *	Mono Audio Dissolve (Fade, Cross Fade)
		 *
		 *	The same parameter (curve/level) is applied to the outgoing fade on first
		 *	clip and to the incoming fade on second clip.
		 */

		// TODO Wrong trace..
		PUSH_TRACE( "OperationGroup__MonoAudioDissolve__::PointList" );

		Trans->flags |= AAFI_TRANS_SINGLE_CURVE;


		/*
		 *	Since this is a Single Parameter, we should have only one Parameter Object
		 *	within the vector. So there's no need to loop through the vector.
		 *	But still, we can have custom objects..
		 */

//		printf( " Count of Params : %u\n", Params->Header->_entryCount );

		aaf_foreach_ObjectInSet( &Param, Params, NULL )
		{
			aafUID_t *ParamDef = aaf_get_propertyValue( Param, PID_Parameter_Definition );

			if ( auidCmp( ParamDef, &AAFParameterDef_Level ) )
				break;
		}

		if ( Param == NULL )
			_fatal( "Could not retrieve Parameter Object.\n" );


		aafWeakRef_t *InterDef = aaf_get_propertyValue( Param, PID_VaryingValue_Interpolation );
		aafObject *InterDefObj = aaf_get_ObjectByWeakRef( aafi->aafd->InterpolationDefinition, InterDef );
		aafUID_t  *InterIdent  = aaf_get_propertyValue( InterDefObj, PID_DefinitionObject_Identification );

		if ( InterIdent == NULL )
			_fatal( "Missing Identification.\n" );

//		printObjectProperties( InterDefObj );

		if ( auidCmp( InterIdent, &AAFInterpolationDef_None ) )
			Trans->flags |= AAFI_INTERPOL_NONE;
		else if ( auidCmp( InterIdent, &AAFInterpolationDef_Linear ) )
			Trans->flags |= AAFI_INTERPOL_LINEAR;
		else if ( auidCmp( InterIdent, &AAFInterpolationDef_Power ) )
			Trans->flags |= AAFI_INTERPOL_POWER;
		else if ( auidCmp( InterIdent, &AAFInterpolationDef_Constant ) )
			Trans->flags |= AAFI_INTERPOL_CONSTANT;
		else if ( auidCmp( InterIdent, &AAFInterpolationDef_BSpline ) )
			Trans->flags |= AAFI_INTERPOL_BSPLINE;
		else if ( auidCmp( InterIdent, &AAFInterpolationDef_Log ) )
			Trans->flags |= AAFI_INTERPOL_LOG;
		else
			printf( "Unknwon Interpolation\n" );


		aafObject *Points = aaf_get_propertyValue( Param, PID_VaryingValue_PointList );
		aafObject *Point  = NULL;

		if ( Points == NULL )
			_fatal( "Missing VaryingValue::PointList.\n" );


		Trans->time_a  = calloc( Points->Header->_entryCount, sizeof(aafRational_t*) );
		Trans->value_a = calloc( Points->Header->_entryCount, sizeof(aafRational_t*) );


		int i = 0;

		aaf_foreach_ObjectInSet( &Point, Points, &AAFClassID_ControlPoint )
		{
			PRINT_TRACE( ANSI_COLOR_GREEN, "ControlPoint" );

			aafRational_t *time  = aaf_get_propertyValue( Point, PID_ControlPoint_Time );

			if ( time == NULL )
				_fatal( "Missing ControlPoint::Time\n" );


			aafRational_t *value = aaf_get_propertyIndirectValue( Point, PID_ControlPoint_Value );

			if ( value == NULL )
				_fatal( "Missing ControlPoint::Value\n" );


			Trans->time_a[i]  = time;
			Trans->value_a[i] = value;

			i++;
		}


		/* set pts_cnt here in case the vector has "custom objects" */
		Trans->pts_cnt_a = i;

/*
		aaf_foreach_ObjectInSet( &Point, Points, NULL )
		{

			PRINT_TRACE( ANSI_COLOR_GREEN, "ControlPoint" );

			aafRational_t *CPTime = aaf_get_propertyValue( Point, PID_ControlPoint_Time );

			aafIndirect_t *CPVal = aaf_get_propertyIndirectValue( Point, PID_ControlPoint_Value );

			aafRational_t *value = (aafRational_t*)&CPVal->Value;

		}
*/

		POP_TRACE();
	}
	else if ( auidCmp( OpIdent, &AAFOperationDef_TwoParameterMonoAudioDissolve ) )
	{
		/*
		 *	Two Parameters Mono Audio Dissolve
		 *
		 *	Two distinct parameters are used for outgoing and incoming fades.
		 */

		PRINT_TRACE( ANSI_COLOR_RED, "OperationGroup__TwoParameterMonoAudioDissolve__::InputSegments" );

		Trans->flags |= AAFI_TRANS_TWO_CURVE;
	}
	else if ( auidCmp( OpIdent, &AAFOperationDef_StereoAudioDissolve ) )
	{
		/*
		 *	Stereo Audio Dissolve
		 *
		 *	TODO Unknown usage and implementation
		 */

		PRINT_TRACE( ANSI_COLOR_RED, "OperationGroup__StereoAudioDissolve__::InputSegments" );
	}
	else
	{
		PRINT_TRACE( ANSI_COLOR_RED, "OperationGroup__%s__\n",
			OperationDefToText( OpIdent )
		);

//		return;
	}

	POP_TRACE();
}






int retrieveClips( AAF_Iface *aafi )
{
	printf( "\n\n"
			"Retrieve Clip Trace ___________________________________________________\n" );

	INIT_TRACE();


	aafObject * Mob = NULL;


	/*
	 *	Loop through CompositionMobs
	 *
	 *	There should be only one, since a CompositionMob
	 *	represent the overall composition (i.e project).
	 */
	aaf_foreach_ObjectInSet( &Mob, aafi->aafd->Mobs, &AAFClassID_CompositionMob )
	{

		aafi->compositionName = aaf_get_propertyValueText( Mob, PID_Mob_Name );

		PUSH_TRACE( "CompositionMob::Slots" );

		/* Get MobSlots */
		aafObject *MobSlots = aaf_get_propertyValue( Mob, PID_Mob_Slots );
		aafObject *MobSlot  = NULL;


//		aafi->Audio->tc = get_CompoMob_Timecode( aafi->aafd, Mob );


		/* Loop through MobSlots */
		aaf_foreach_ObjectInSet( &MobSlot, MobSlots, NULL )
		{

			/* Get MobSlot::Segment > Component::DataDefinition */
			aafUID_t *DataDef = getMobSlotDataDef( aafi->aafd->DataDefinition, MobSlot );

			if ( DataDef == NULL )
				_fatal( "Could not retrieve MobSlot's DataDefinition.\n" );


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
			 */

			if ( auidCmp( MobSlot->Class->ID, &AAFClassID_TimelineMobSlot ) == 0 )
			{
				PRINT_TRACE( ANSI_COLOR_YELLOW, "%s__%s",
					ClassIDToText( MobSlot->Class->ID ),
					DataDefToText( DataDef )
				);

				continue;
			}


			aafiAudioTrack *track = NULL;
			aafPosition_t   pos   = 0;

			if ( auidCmp( DataDef, &AAFDataDef_Sound ) ||
				 auidCmp( DataDef, &AAFDataDef_LegacySound ) )
			{

				track = aafi_newAudioTrack( aafi, MobSlot, -1 );

				track->Audio = aafi->Audio;
			}
			else if ( auidCmp( DataDef, &AAFDataDef_Timecode ) == 0 &&
					  auidCmp( DataDef, &AAFDataDef_LegacyTimecode ) == 0 )
			{
				PRINT_TRACE( ANSI_COLOR_YELLOW, "TimelineMobSlot__%s",
					DataDefToText( DataDef )
				);

				continue;
			}





			/* Get MobSlot::Segment */
			aafObject *Segment = aaf_get_propertyValue( MobSlot, PID_MobSlot_Segment );


			PUSH_TRACE( "TimelineMobSlot__%s__::Segment", DataDefToText( DataDef ) + 11 );

			if ( auidCmp( Segment->Class->ID, &AAFClassID_Sequence ) )
			{

//				pos.prev_trans_len = 0;
//				pos = 0; //tc->start;

				/* Get Sequence's Components */
				aafObject * Components = aaf_get_propertyValue( Segment, PID_Sequence_Components );
				aafObject * Component  = NULL;

				PUSH_TRACE( "Sequence::Components" );

				/* Loop through Sequence's Components */
				aaf_foreach_ObjectInSet( &Component, Components, NULL )
				{

					if ( auidCmp( Component->Class->ID, &AAFClassID_SourceClip ) )
					{
						/*
						 *	SourceClip
						 *
						 *	A simple SourceClip.
						 */

						PRINT_TRACE( ANSI_COLOR_GREEN, "SourceClip" );

						retrieve_CompoMob_SourceClip( aafi, &pos, track, Component );

					}
					else if ( auidCmp( Component->Class->ID, &AAFClassID_Filler ) )
					{
						/*
						 *	Filler
						 *
						 *	This represents an empty space on the timeline, which is
						 *	Component::Length long.
						 */

						PRINT_TRACE( ANSI_COLOR_GREEN, "Filler" );

//						if ( Component->prev == NULL || Component->next == NULL )
//							continue;

						int64_t *length = (int64_t*)aaf_get_propertyValue( Component, PID_Component_Length );

						if ( length == NULL )
							_fatal( "Missing Filler Component::Length\n" );

						pos += *length;

//						printf( "FILLER : %li\n", *length );

					}
					else if ( auidCmp( Component->Class->ID, &AAFClassID_OperationGroup ) )
					{
						/*
						 *	OperationGroup
						 *
						 *	An OperationGroup allows to apply an effect to one or more
						 *	SourceClips.
						 *
						 *	SourceClip(s) are in OperationGroup::InputSegments.
						 */

						retrieve_CompoMob_OperationGroup( aafi, &pos, track, Component, TRACE_LOG );

					}
					else if ( auidCmp( Component->Class->ID, &AAFClassID_Transition ) )
					{
						/*
						 *	Transition
						 *
						 *	A Transition between a Filler and a SourceClip sets a Fade In.
						 *
						 *	A Transition between a SourceClip and a Filler sets a Fade Out.
						 *
						 *	A Transition between two SourceClips sets a Cross-Fade.
						 *
						 *	Since the Transition applies to the elements that suround it in
						 *	the Sequence, The OperationGroup::InputSegments is then left unused.
						 */

						int64_t *length = aaf_get_propertyValue( Component, PID_Component_Length );

						pos -= *length;

//						printf( "PREV_TRANS_LEN : %li\n", pos.prev_trans_len );


						retrieve_CompoMob_Transition( aafi, track, Component, TRACE_LOG );


					} // --- if ( auidCmp( Component->Class->ID, &AAFClassID_Transition ) )
					else
					{

						PRINT_TRACE( ANSI_COLOR_YELLOW, "%s",
							ClassIDToText( Component->Class->ID )
						);

					}


				} // --- aaf_foreach_ObjectInSet( &Component, Components, NULL )


				POP_TRACE();

			} // --- if ( auidCmp( Segment->Class->ID, &AAFClassID_Sequence ) )
			else if ( auidCmp( Segment->Class->ID, &AAFClassID_Timecode ) )
			{
				/*
				 *	TODO can contain sequence ? other Timecode SMPTE ..
				 */

				PRINT_TRACE( ANSI_COLOR_GREEN, "Timecode" );

				retrieve_CompoMob_Timecode( aafi, Segment );

			}
			else if ( auidCmp( Segment->Class->ID, &AAFClassID_OperationGroup ) )
			{

				/*
				 *	For track-based Gain and PAN. The OperationGroup::InputSegments shall
				 *	contain the Sequence with SourceClips, or other OperationGroup.
				 */

			}
			else if ( auidCmp( Segment->Class->ID, &AAFClassID_Filler ) )
			{

				/*
				 *	Some AAF files hold a TimelineMobSlot::Segment > Filler, and thus
				 *	with no SourceClip. It seems that those represent existing tracks
				 *	with no clips in the source software.
				 */

				PRINT_TRACE( ANSI_COLOR_GREEN, "Filler" );

			}
			else
			{

				PRINT_TRACE( ANSI_COLOR_RED, "%s", ClassIDToText( Segment->Class->ID ) );

			}


			POP_TRACE(); // MobSlots::Segment

		} // aaf_foreach_ObjectInSet( &MobSlot, MobSlots, NULL )


		POP_TRACE(); // CompositionMob::Slots

	} // end of CompoMobs Loop


	return 0;
}

/**
 *	@}
 */

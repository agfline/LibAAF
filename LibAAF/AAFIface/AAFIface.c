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

// #include <ctype.h>	// isxdigit()

//#include <math.h>	// log10() for gain calc

#include "../libAAF.h"

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







/*
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
*/




static void aafi_freeAudioTracks( aafiAudioTrack **tracks );
static void aafi_freeTimelineItems( aafiTimelineItem **items );
static void aafi_freeTransition( aafiTransition *trans );

static void aafi_freeAudioEssences( aafiAudioEssence **essences );
static void aafi_freeEssenceDataNode( aafiEssenceDataNode **nodes );

static void   parse_OperationGroup( AAF_Iface *aafi, aafObject *OpGroup );
static void   parse_Transition( AAF_Iface *aafi, aafObject *Transition );
static void   parse_Component( AAF_Iface *aafi, aafObject *Component );
static void   parse_Segment( AAF_Iface *aafi, aafObject *Segment );
static void * parse_SourceClip( AAF_Iface *aafi, aafObject *SourceClip );
static void   parse_Timecode( AAF_Iface *aafi, aafObject *Timecode );
static void   parse_Sequence( AAF_Iface *aafi, aafObject *Sequence );
static void   parse_Filler( AAF_Iface *aafi, aafObject *Filler );

static void   parse_EssenceDescriptor( AAF_Iface *aafi, aafObject *EssenceDesc );

static void   retrieve_EssenceData( AAF_Iface *aafi );

static int    retrieve_ControlPoints( AAF_Iface *aafi, aafObject *Points, aafRational_t *times[], aafRational_t *values[] );
static void   parse_Parameter( AAF_Iface *aafi, aafObject *Parameter );






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
			char *name = aaf_get_propertyValueText( Obj, PID_Mob_Name );
			snprintf( buf, 1024, "%s (%s) > %s", ClassIDToText( aafi->aafd, Obj->Class->ID ), name, tmp );
			free( name );
		}
		else if ( auidCmp( Obj->Class->ID, &AAFClassID_CompositionMob ) )
		{
			char *name = aaf_get_propertyValueText( Obj, PID_Mob_Name );
			snprintf( buf, 1024, "%s (%s) > %s", ClassIDToText( aafi->aafd, Obj->Class->ID ), name, tmp );
			free( name );
		}
		else if ( auidCmp( Obj->Class->ID, &AAFClassID_OperationGroup ) )
		{
			aafWeakRef_t *OpDef = aaf_get_propertyValue( Obj, PID_OperationGroup_Operation );

			if ( OpDef == NULL )
				_fatal( "Missing OperationGroup::Operation.\n" );


			aafObject *OpDefObj = aaf_get_ObjectByWeakRef( aafi->aafd->OperationDefinition, OpDef );

			if ( OpDefObj == NULL )
				_fatal( "Could not retrieve OperationDefinition from dictionary.\n" );


			aafUID_t *OpIdent = aaf_get_propertyValue( OpDefObj, PID_DefinitionObject_Identification );

			if ( OpIdent == NULL )
				_fatal( "Missing DefinitionObject::Identification.\n" );


			const char *name = OperationDefToText( aafi->aafd, OpIdent ) /*printUID( OpIdent )*/;
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

	aafi->compositionName = NULL;

	return aafi;
}



void aafi_release( AAF_Iface **aafi )
{
	if ( *aafi == NULL )
		return;


	aaf_release( &(*aafi)->aafd );

	if ( (*aafi)->compositionName != NULL )
	{
		free( (*aafi)->compositionName );
	}

	if ( (*aafi)->Audio != NULL )
	{
		if ( (*aafi)->Audio->Tracks != NULL )
		{
			aafi_freeAudioTracks( &(*aafi)->Audio->Tracks );
		}

		if ( (*aafi)->Audio->Essences != NULL )
		{
			aafi_freeAudioEssences( &(*aafi)->Audio->Essences );
		}

		if ( (*aafi)->Audio->tc != NULL )
		{
			free( (*aafi)->Audio->tc );
		}

		free( (*aafi)->Audio );
	}

	free( *aafi );
}



int aafi_load_file( AAF_Iface *aafi, const char * file )
{
	if ( aaf_load_file( aafi->aafd, file ) )
	{
		return 1;
	}

	aafi_retrieveData( aafi );

	return 0;
}


void swap( unsigned char *tab, int i, int j )
{
	unsigned char c = tab[i];
	tab[i] = tab[j];
	tab[j] = c;
}

int extractAudioEssence( AAF_Iface *aafi, aafiAudioEssence *audioEssence, const char *file )
{

	/*
	 *	If the audioEssence->file is not NULL, it means that the essence isn't embedded in the
	 *	AAF. The essence file can therefore be retrieved from the URI hold by audioEssence->file.
	 */

	if ( audioEssence->node == NULL )
		return 1;

	char filePath[1024];

	snprintf( filePath, 1024, "%s.wav", file );

	// printf("%s\n", filePath );

	FILE *fp = fopen( filePath, "w+" );

	if ( fp == NULL )
		_fatal( "%s.\n", strerror(errno) );

	if ( audioEssence->type == AAFI_TYPE_PCM || audioEssence->type == AAFI_TYPE_WAVE || audioEssence->type == AAFI_TYPE_AIFC )
	{
		/* Write WAVE Header */
		unsigned char header[44];

		header[0]  = 'R';
		header[1]  = 'I';
		header[2]  = 'F';
		header[3]  = 'F';

		*(uint32_t*)(header+4)  = audioEssence->length + 36;

		header[8]  = 'W';
		header[9]  = 'A';
		header[10] = 'V';
		header[11] = 'E';

		header[12] = 'f';
		header[13] = 'm';
		header[14] = 't';
		header[15] = ' ';

		*(uint32_t*)(header+16) = 16;
		*(uint16_t*)(header+20) = audioEssence->wFormatTag;
		*(uint16_t*)(header+22) = audioEssence->nChannels;
		*(uint32_t*)(header+24) = audioEssence->nSamplesPerSec;
		*(uint32_t*)(header+28) = audioEssence->nAvgBytesPerSec;
		*(uint16_t*)(header+32) = audioEssence->nBlockAlign;
		*(uint16_t*)(header+34) = audioEssence->wBitsPerSample;

		header[36] = 'd';
		header[37] = 'a';
		header[38] = 't';
		header[39] = 'a';

		*(uint32_t*)(header+40) = (uint32_t)audioEssence->length;

		fwrite( header, sizeof(unsigned char), 44, fp );

		fseek( fp, 44, SEEK_SET );
	}

	printf("length : %lu\n", audioEssence->length );


	uint32_t id     = 0;
	size_t   len    = 0;
	// uint64_t offset = 44;

	aafByte_t *nodeBuf  = NULL;
	// aafByte_t *audioBuf = malloc( 512 * 4096 ); // 2 097 152 bytes
	// size_t     audioBuf_len = 0;
	// size_t i = 0;
    //
	aafiEssenceDataNode *node = audioEssence->node;

	// for ( node = audioEssence->nodes; node != NULL; node = node->next )
	// {
		/* TODO multiple aafiEssenceDataNode ???? */

		cfb_foreachSectorInStream( aafi->aafd->cfbd, node->node, &nodeBuf, &len, &id )
		{

			if ( audioEssence->type == AAFI_TYPE_AIFC )
			{
				size_t i = 0;

				for( i = 0; i < len; i += 2 )
				{
					swap( nodeBuf, i, i+1 );
				}
			}

			fwrite( nodeBuf, sizeof(aafByte_t), len, fp );

			// i++;
		}

		if ( nodeBuf )
			free( nodeBuf );

	// }

	// save filename
	audioEssence->file = malloc( strlen( file ) + 2 );

	if ( audioEssence->file == NULL )
		_fatal( "%s.\n", strerror( errno ) );

	snprintf( audioEssence->file, strlen( file ) + 1, "%s", file );


	// printf( "WRITING %s\n", filePath );

	fclose( fp );

	return 0;
}








/*
	NOTE: we can't fallback on Locator::URLString since that field can sometimes points the aaf file itself.
	TODO: preserve/add or leave/remove file extension
*/

char * aafi_get_essence_filename( aafiAudioEssence *audioEssence, char **filename, char *fb_str, uint32_t *fb_num/*, uint8_t include_ext*/ )
{
	if ( *filename == NULL )
	{
		/* TODO use FILE_MAX for size */
		*filename = malloc(1024);
	}


	if ( audioEssence->file_name != NULL )
	{
		snprintf( *filename, 1024, "%s", audioEssence->file_name );
	}
	else
	{
		snprintf( *filename, 1024, "%s_%02u", fb_str, (*fb_num)++ );
	}

	return *filename;
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













aafiTimelineItem * aafi_newTimelineItem( aafiAudioTrack *track, int itemType )
{

	aafiTimelineItem *item = NULL;

	if ( itemType == AAFI_CLIP )
	{
		item = calloc( sizeof(aafiTimelineItem) + sizeof(aafiAudioClip),  1 );

		if ( item == NULL )
			_fatal( "%s.\n", strerror( errno ) );


		item->type |= AAFI_CLIP;

		aafiAudioClip *audioClip = (aafiAudioClip*)&item->data;

		audioClip->track = track;
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




static void aafi_freeTimelineItems( aafiTimelineItem **items )
{
	aafiTimelineItem *item = NULL;
	aafiTimelineItem *nextItem = NULL;

	for ( item = (*items); item != NULL; item = nextItem )
	{
		nextItem = item->next;

		if ( item->type == AAFI_TRANS )
		{
			aafi_freeTransition( (aafiTransition*)&(item->data) );
		}
		else if ( item->type == AAFI_CLIP )
		{
			aafiAudioClip *audioClip = (aafiAudioClip*)item->data;

			if ( audioClip->gain != NULL )
			{
				if ( audioClip->gain->time != NULL )
				{
					// int i;
                    //
					// for ( i = 0; i < audioClip->gain->pts_cnt; i++ )
					// {
					// 	free( audioClip->gain->time[i] );
					// }

					free( audioClip->gain->time );
				}

				if ( audioClip->gain->value != NULL )
				{
					// int i;
                    //
					// for ( i = 0; i < audioClip->gain->pts_cnt; i++ )
					// {
					// 	free( audioClip->gain->value[i] );
					// }

					free( audioClip->gain->value );
				}

				free( audioClip->gain );
			}
		}

		free( item );
	}

	*items = NULL;
}





static void aafi_freeTransition( aafiTransition *Transition )
{
	if ( Transition->value_a != NULL )
	{
		free( Transition->value_a );
	}

	if ( Transition->value_b != NULL )
	{
		free( Transition->value_b );
	}

	if ( Transition->time_a != NULL )
	{
		free( Transition->time_a );
	}

	if ( Transition->time_b != NULL )
	{
		free( Transition->time_b );
	}
}









aafiAudioTrack * aafi_newAudioTrack( AAF_Iface *aafi, aafObject *MobSlot, uint32_t number )
{
	aafiAudioTrack *track = calloc( sizeof(aafiAudioTrack), sizeof(unsigned char) );

	if ( track == NULL )
		_fatal( "%s.\n", strerror( errno ) );


	track->next = NULL;

	track->format = AAFI_TRACK_FORMAT_MONO;


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
	{
		aafi->Audio->Tracks = track;
	}


	track->Audio = aafi->Audio;

	aafi->ctx.current_track = track;


	return track;
}





static void aafi_freeAudioTracks( aafiAudioTrack **tracks )
{
	if ( *(tracks) == NULL )
	{
		return;
	}

	aafiAudioTrack *track = NULL;
	aafiAudioTrack *nextTrack = NULL;

	for ( track = (*tracks); track != NULL; track = nextTrack )
	{
		nextTrack = track->next;

		if ( track->name != NULL )
		{
			free( track->name );
		}

		if ( track->Items != NULL )
		{
			aafi_freeTimelineItems( &(track->Items) );
		}

		free( track );
	}

	*tracks = NULL;
}






aafiAudioEssence * aafi_newAudioEssence( AAF_Iface *aafi )
{
	aafiAudioEssence * audioEssence = calloc( sizeof(aafiAudioEssence), sizeof(char) );

	if ( audioEssence == NULL )
		_fatal( "%s.\n", strerror( errno ) );


	// printf( "%p \n", audioEssence );
	// printf( "%p \n", aafi->Audio );
	// printf( "%p \n", aafi->Audio->Essences );

	audioEssence->next = aafi->Audio->Essences;

	audioEssence->file        = NULL;
	audioEssence->file_name   = aaf_get_propertyValueText( aafi->ctx.Mob, PID_Mob_Name );

	aafi->Audio->Essences = audioEssence;

	return audioEssence;
}

static void aafi_freeAudioEssences( aafiAudioEssence **audioEssence )
{
	if ( *(audioEssence) == NULL )
	{
		return;
	}

	// aafiAudioEssence *audioEssence = NULL;
	aafiAudioEssence *nextAudioEssence = NULL;

	for (; (*audioEssence) != NULL; *audioEssence = nextAudioEssence )
	{
		nextAudioEssence = (*audioEssence)->next;

		if ( (*audioEssence)->file != NULL )
		{
			free( (*audioEssence)->file );
		}

		if ( (*audioEssence)->file_name != NULL )
		{
			free( (*audioEssence)->file_name );
		}

		if ( (*audioEssence)->node != NULL )
		{
			aafi_freeEssenceDataNode( &((*audioEssence)->node) );
		}

		free( *audioEssence );
	}

	*audioEssence = NULL;
}





aafiEssenceDataNode * aafi_newEssenceDataNode( aafiAudioEssence *audioEssence )
{
	aafiEssenceDataNode *dataNode = calloc( sizeof(aafiEssenceDataNode), sizeof(char) );

	if ( dataNode == NULL )
		_fatal( "%s.\n", strerror( errno ) );

	/*
	 *	Add to audio essence's nodes list
	 */

	// aafiEssenceDataNode *tmp = audioEssence->nodes;
    //
	// if ( tmp != NULL )
	// {
	// 	for (; tmp != NULL; tmp = tmp->next )
	// 		if ( tmp->next == NULL )
	// 			break;
    //
	// 	tmp->next = dataNode;
	// }
	// else

	audioEssence->node = dataNode;


	return dataNode;
}




static void aafi_freeEssenceDataNode( aafiEssenceDataNode **node )
{
	if ( node == NULL || *node == NULL )
	{
		return;
	}

	free( *node );

	*node = NULL;
	// aafiEssenceDataNode *nextNode = NULL;

	// for ( node = (*nodes); node != NULL; node = nextNode )
	// {
	//		nextNode = node->next;

	// }
}



















aafUID_t * getMobSlotDataDef( aafObject *DataDefinition, aafObject *MobSlot )
{
	aafUID_t *DataDefVal = NULL;

	if ( MobSlot == NULL )
		return NULL;

	aafObject *Segment = aaf_get_propertyValue( MobSlot, PID_MobSlot_Segment );

	if ( Segment )
	{
//		printf( "   : %s\n", ClassIDToText( aafi->aafd, Segment->Class->ID ) );

		aafWeakRef_t *weakRef = aaf_get_propertyValue( Segment, PID_Component_DataDefinition );

		aafObject *DataDef = aaf_get_ObjectByWeakRef( DataDefinition, weakRef );

		if ( DataDef )
		{
//			printf( "   :: %s\n", ClassIDToText( aafi->aafd, DataDef->Class->ID ) );

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




static void retrieve_EssenceData( AAF_Iface *aafi )
{
	aafiAudioEssence *audioEssence = aafi->ctx.current_audioEssence;

	/* Get EssenceData Object */
	aafObject *EssenceData = getEssenceDataByMobID( aafi->aafd->EssenceData, aafi->ctx.current_audioEssence->sourceMobID );

	if ( EssenceData == NULL )
	{
		/*
		 *	It means essence is not embedded.
		 */

		// _warning( "Could not retrieve EssenceData\n" );

		return;
	}

	trace_obj( aafi, EssenceData, ANSI_COLOR_MAGENTA );


	aafiEssenceDataNode *dataNode = aafi_newEssenceDataNode( audioEssence );


	/* Get EssenceData Stream */
	dataNode->node = getEssenceDataStreamNode( aafi->aafd, EssenceData );

	if ( dataNode->node == NULL )
		_fatal( "Could not retrieve Data Stream Node.\n" );


	/* Retrieve data stream length */
	uint64_t dataLen = getNodeStreamLen( aafi->aafd->cfbd, dataNode->node );

	if ( dataLen == 0 )
		_warning( "Got 0 Bytes data stream length.\n" );


	// audioEssence->length += dataLen;
	audioEssence->length = dataLen;
}



/*
		         Locator
		            |
		    ,---------------.
		    |               |
      NetworkLocator   TextLocator

*/

static void parse_Locator( AAF_Iface *aafi, aafObject *Locator )
{

	trace_obj( aafi, Locator, ANSI_COLOR_MAGENTA );

	aafiAudioEssence *audioEssence = aafi->ctx.current_audioEssence;


	if ( auidCmp( Locator->Class->ID, &AAFClassID_NetworkLocator ) )
	{
		/*
		 *	This holds an URI pointing to the essence file, when it is not embedded.
		 *	However, sometimes it holds an URI to the AAF file itself when essence is
		 *	embedded so it is not a valid way to test if essence is embedded or not.
		 */

		audioEssence->file = aaf_get_propertyValueText( Locator, PID_NetworkLocator_URLString );

		if ( audioEssence->file == NULL )
			_warning( "Missing Locator::PID_NetworkLocator_URLString.\n" );

		url_decode( audioEssence->file, audioEssence->file );

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

		_warning( "Got an AAFClassID_TextLocator : \"%s\"\n", aaf_get_propertyValueText( Locator, PID_TextLocator_Name ) );

	}

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


/*
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
*/

static void parse_EssenceDescriptor( AAF_Iface *aafi, aafObject *EssenceDesc )
{

	aafiAudioEssence *audioEssence = aafi->ctx.current_audioEssence;


	if ( auidCmp( EssenceDesc->Class->ID, &AAFClassID_PCMDescriptor ) )
	{
		trace_obj( aafi, EssenceDesc, ANSI_COLOR_MAGENTA );

		audioEssence->type            = AAFI_TYPE_PCM;

		audioEssence->wFormatTag      =  0x0001;	// PCM
		audioEssence->nChannels       = *(uint32_t*)aaf_get_propertyValue( EssenceDesc, PID_SoundDescriptor_Channels         );
		audioEssence->nSamplesPerSec  = *(uint32_t*)aaf_get_propertyValue( EssenceDesc, PID_FileDescriptor_SampleRate        );
		audioEssence->nAvgBytesPerSec = *(uint32_t*)aaf_get_propertyValue( EssenceDesc, PID_PCMDescriptor_AverageBPS         );
		audioEssence->nBlockAlign     = *(uint32_t*)aaf_get_propertyValue( EssenceDesc, PID_PCMDescriptor_BlockAlign         );
		audioEssence->wBitsPerSample  = *(uint32_t*)aaf_get_propertyValue( EssenceDesc, PID_SoundDescriptor_QuantizationBits );

		// aafUID_t *ContainerDef = aaf_get_propertyValue( EssenceDesc, PID_FileDescriptor_ContainerFormat );

		// printf("ContainerDef : %s\n", ContainerToText( ContainerDef ) );
	}
	else if ( auidCmp( EssenceDesc->Class->ID, &AAFClassID_WAVEDescriptor ) )
	{

		trace_obj( aafi, EssenceDesc, ANSI_COLOR_MAGENTA );

		audioEssence->type = AAFI_TYPE_WAVE;


		aafProperty *summary = aaf_get_property( EssenceDesc, PID_WAVEDescriptor_Summary );

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

		// aafUID_t *ContainerDef = aaf_get_propertyValue( EssenceDesc, PID_FileDescriptor_ContainerFormat );

		// printf("ContainerDef : %s\n", ContainerToText( ContainerDef ) );
	}
	else if ( auidCmp( EssenceDesc->Class->ID, &AAFClassID_AIFCDescriptor ) )
	{

		trace_obj( aafi, EssenceDesc, ANSI_COLOR_MAGENTA );

		audioEssence->type = AAFI_TYPE_AIFC;


		/* TODO This is proof of concept : do it with proper lib */

		aafProperty *summary = aaf_get_property( EssenceDesc, PID_AIFCDescriptor_Summary );


		audioEssence->wFormatTag      = 0x0001; // PCM
		audioEssence->nChannels       = Reverse16( *(uint16_t*)(summary->val+20) );
		audioEssence->nSamplesPerSec  = getAIFCSampleRate( (unsigned char*)(summary->val+28) );
		audioEssence->wBitsPerSample  = Reverse16( *(uint16_t*)(summary->val+26) );
		audioEssence->nBlockAlign     = audioEssence->nChannels * audioEssence->wBitsPerSample / 8;
		audioEssence->nAvgBytesPerSec = audioEssence->nSamplesPerSec * audioEssence->nBlockAlign;

		// aafUID_t *ContainerDef = aaf_get_propertyValue( EssenceDesc, PID_FileDescriptor_ContainerFormat );

		// printf("ContainerDef : %s\n", ContainerToText( ContainerDef ) );
	}
	else if ( auidCmp( EssenceDesc->Class->ID, &AAFClassID_SoundDescriptor ) )
	{

		trace_obj( aafi, EssenceDesc, ANSI_COLOR_RED );

		_warning( "No support for AAFClassID_SoundDescriptor." );

		// printObjectProperties( aafi->aafd, EssenceDesc );

		// aafUID_t *ContainerDef = aaf_get_propertyValue( EssenceDesc, PID_FileDescriptor_ContainerFormat );

		// printf("ContainerDef : %s\n", ContainerToText( ContainerDef ) );
	}
	else if ( auidCmp( EssenceDesc->Class->ID, &AAFClassID_AES3PCMDescriptor ) )
	{

		/*
		 *	Not described in specs.
		 */

		trace_obj( aafi, EssenceDesc, ANSI_COLOR_RED );

		_warning( "No support for AAFClassID_AES3PCMDescriptor." );

		printObjectProperties( aafi->aafd, EssenceDesc );

		// aafUID_t *ContainerDef = aaf_get_propertyValue( EssenceDesc, PID_FileDescriptor_ContainerFormat );

		// printf("ContainerDef : %s\n", ContainerToText( ContainerDef ) );

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

		trace_obj( aafi, EssenceDesc, ANSI_COLOR_RED );

		_warning( "MultipleDescriptor not supported yet.\n\n" );

		// printObjectProperties( aafi->aafd, EssenceDesc );

	}
	else
	{
		trace_obj( aafi, EssenceDesc, ANSI_COLOR_RED );

		_warning( "Unsupported (yet ?) ClassID : %s\n", ClassIDToText( aafi->aafd, EssenceDesc->Class->ID ) );

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
p41: 	Absolute Uniform Resource Locator (URL) complying with RFC 1738 or relative
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





















aafiAudioEssence * getEssenceBySourceMobID( AAF_Iface *aafi, aafMobID_t *sourceMobID )
{
	aafiAudioEssence * audioEssence = NULL;


	for ( audioEssence = aafi->Audio->Essences; audioEssence != NULL; audioEssence = audioEssence->next )
	{
		if ( mobIDCmp( audioEssence->masterMobID, sourceMobID ) )
			break;
	}


	return audioEssence;
}


aafiAudioClip * getClipBySourceMobID( AAF_Iface *aafi, aafMobID_t *sourceMobID )
{
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

			if ( mobIDCmp( audioClip->sourceMobID, sourceMobID ) )
			{
				return audioClip;
			}
		}
	}

	return NULL;
}

























/*
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
*/

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





static void parse_Segment( AAF_Iface *aafi, aafObject *Segment )
{

	// printf("PARENT CONTEXT : %s\n", ClassIDToText( aafi->aafd, Segment->Parent->Class->ID ) );

	if ( auidCmp( Segment->Class->ID, &AAFClassID_Sequence ) )
	{

		parse_Sequence( aafi, Segment );

	}
	else if ( auidCmp( Segment->Class->ID, &AAFClassID_SourceClip ) )
	{

		/*
		 *	A simple SourceClip.
		 *
		 *	Plays for both essence and clip.
		 */

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

				/*
				 *	For track-based Gain and PAN. The OperationGroup::InputSegments shall
				 *	contain the Sequence with SourceClips, or other OperationGroup.
				 *
				 *	TODO to be implemented, not encountered yet.
				 */

				 trace_obj( aafi, Segment, ANSI_COLOR_RED );

			}
			else if ( auidCmp( Segment->Parent->Class->ID, &AAFClassID_Sequence ) )
			{

				/*
				 *	OperationGroup
				 *
				 *	An OperationGroup allows to apply an effect to one or more
				 *	SourceClips.
				 *
				 *	SourceClip(s) are in OperationGroup::InputSegments.
				 */

				parse_OperationGroup( aafi, Segment );

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
			_fatal( "Missing Filler Component::Length\n" );

		// *pos += *length;
		aafi->ctx.current_pos += *length;
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





static void * parse_SourceClip( AAF_Iface *aafi, aafObject *SourceClip )
{
	trace_obj( aafi, SourceClip, ANSI_COLOR_MAGENTA );

	// printObjectProperties( aafi->aafd, SourceClip );

	/*** Clip ***/

	if ( auidCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_CompositionMob ) )
	{

		aafiTimelineItem *item  = aafi_newTimelineItem( aafi->ctx.current_track, AAFI_CLIP );

		aafiAudioClip    *audioClip = (aafiAudioClip*)&item->data;


		audioClip->pos = aafi->ctx.current_pos; /**pos;*/



		int64_t *length = (int64_t*)aaf_get_propertyValue( SourceClip, PID_Component_Length );

		if ( length == NULL )
			_fatal( "Missing SourceClip Component::Length !\n" );

		audioClip->len = *length;



		int64_t *startTime = (int64_t*)aaf_get_propertyValue( SourceClip, PID_SourceClip_StartTime );

		if ( startTime == NULL )
			_fatal( "Missing SourceClip::StartTime" );

		audioClip->essence_offset = *startTime;


		/*
p.49	 *	To create a SourceReference that refers to a MobSlot within
		 *	the same Mob as the SourceReference, omit the SourceID property.
		 */

		audioClip->sourceMobID = (aafMobID_t*)aaf_get_propertyValue( SourceClip, PID_SourceReference_SourceID );

		if ( audioClip->sourceMobID == NULL )
		{
			aafObject *Mob = NULL;

			for ( Mob = SourceClip; Mob != NULL; Mob = Mob->Parent )
			{
				if ( auidCmp( Mob->Class->ID, &AAFClassID_CompositionMob ) )
					break;
			}

			audioClip->sourceMobID = aaf_get_propertyValue( Mob, PID_Mob_MobID );

			_warning( "Missing SourceReference::SourceID, retrieving from parent Mob.\n" );
		}




		// aafObject *mob = aaf_get_MobByID( aafi->aafd->Mobs, audioClip->sourceMobID );
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

		audioClip->Essence = getEssenceBySourceMobID( aafi, audioClip->sourceMobID );


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
p.49	 *	To create a SourceReference that refers to a MobSlot within
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
		// 	aafObject *Segment = aaf_get_propertyValue( aafi->ctx.MobSlot, PID_MobSlot_Segment );
        //
		// 	printf("Segment %s\n", ClassIDToText( aafi->aafd, Segment->Class->ID ) );
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






		parse_EssenceDescriptor( aafi, EssenceDesc );



		aafiAudioClip * audioClip = getClipBySourceMobID( aafi, audioEssence->masterMobID );

		if ( audioClip != NULL )
		{
			/* that means the clip was parsed before the essence, so we must do the linking here */
			audioClip->Essence = audioEssence;
		}

	}

	return NULL;
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


/*
		        Parameter
		            |
		    ,--------------.
		    |              |
      ConstantValue   VaryingValue


	A Parameter object shall be owned by an OperationGroup object.
*/

static void parse_Parameter( AAF_Iface *aafi, aafObject *Parameter )
{

	/* Retrieves Operation's Identification */

	aafWeakRef_t *OpDef = aaf_get_propertyValue( Parameter->Parent, PID_OperationGroup_Operation );

	if ( OpDef == NULL )
		_fatal( "Missing OperationGroup::Operation.\n" );


	aafObject *OpDefObj = aaf_get_ObjectByWeakRef( aafi->aafd->OperationDefinition, OpDef );

	if ( OpDefObj == NULL )
		_fatal( "Could not retrieve OperationDefinition from dictionary.\n" );


	aafUID_t *OpIdent = aaf_get_propertyValue( OpDefObj, PID_DefinitionObject_Identification );

	if ( OpIdent == NULL )
		_fatal( "Missing DefinitionObject::Identification.\n" );





	// aafUID_t *paramDef = aaf_get_propertyValue( Parameter, PID_Parameter_Definition );
    //
	// printf("ParamDef : %s\n", ParameterToText(paramDef) );

	if ( auidCmp( Parameter->Class->ID, &AAFClassID_ConstantValue ) )
	{

		if ( auidCmp( OpIdent, &AAFOperationDef_MonoAudioGain ) )
		{

			trace_obj( aafi, Parameter, ANSI_COLOR_MAGENTA );

			aafiAudioGain *Gain = aafi->ctx.current_gain;


			Gain->flags   |= AAFI_AUDIO_GAIN_CONSTANT;


			aafRational_t *multiplier = aaf_get_propertyIndirectValue( Parameter, PID_ConstantValue_Value );

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
	else if ( auidCmp( Parameter->Class->ID, &AAFClassID_VaryingValue ) )
	{

		aafWeakRef_t *InterDef = aaf_get_propertyValue( Parameter, PID_VaryingValue_Interpolation );

		if ( InterDef == NULL )
			_fatal( "Missing Parameter::Interpolation.\n" );


		aafObject *InterDefObj = aaf_get_ObjectByWeakRef( aafi->aafd->InterpolationDefinition, InterDef );

		if ( InterDefObj == NULL )
			_fatal( "Could not find InterpolationDefinition.\n" );


		aafUID_t  *InterIdent  = aaf_get_propertyValue( InterDefObj, PID_DefinitionObject_Identification );

		if ( InterIdent == NULL )
			_fatal( "Missing Parameter DefinitionObject::Identification.\n" );



		aafiInterpolation_e interpolation = 0;

		if ( auidCmp( InterIdent, &AAFInterpolationDef_None ) )
			interpolation = AAFI_INTERPOL_NONE;
		else if ( auidCmp( InterIdent, &AAFInterpolationDef_Linear ) )
			interpolation = AAFI_INTERPOL_LINEAR;
		else if ( auidCmp( InterIdent, &AAFInterpolationDef_Power ) )
			interpolation = AAFI_INTERPOL_POWER;
		else if ( auidCmp( InterIdent, &AAFInterpolationDef_Constant ) )
			interpolation = AAFI_INTERPOL_CONSTANT;
		else if ( auidCmp( InterIdent, &AAFInterpolationDef_BSpline ) )
			interpolation = AAFI_INTERPOL_BSPLINE;
		else if ( auidCmp( InterIdent, &AAFInterpolationDef_Log ) )
			interpolation = AAFI_INTERPOL_LOG;
		else
			printf( "Unknwon Interpolation\n" );



		if ( auidCmp( Parameter->Parent->Parent->Class->ID, &AAFClassID_Transition ) )
		{

			aafiTransition *Trans = aafi->ctx.current_transition;

			Trans->flags |= interpolation;


			aafObject *Points = aaf_get_propertyValue( Parameter, PID_VaryingValue_PointList );

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
		else if ( auidCmp( OpIdent, &AAFOperationDef_MonoAudioGain ) )
		{
			aafiAudioGain *Gain = aafi->ctx.current_gain;

			Gain->flags |= AAFI_AUDIO_GAIN_VARIABLE;
			Gain->flags |= interpolation;


			aafObject *Points = aaf_get_propertyValue( Parameter, PID_VaryingValue_PointList );

			if ( Points == NULL )
				_fatal( "Missing VaryingValue::PointList.\n" );

			Gain->pts_cnt = retrieve_ControlPoints( aafi, Points, &Gain->time, &Gain->value );

			// int i = 0;
            //
			// for ( i = 0; i < Gain->pts_cnt; i++ )
			// {
			// 	printf("time_%i : %i/%i   value_%i : %i/%i\n", i, Gain->time[i].numerator, Gain->time[i].denominator, i, Gain->value[i].numerator, Gain->value[i].denominator  );
			// }
		}
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
			_fatal( "Missing ControlPoint::Time\n" );


		aafRational_t *value = aaf_get_propertyIndirectValue( Point, PID_ControlPoint_Value );

		if ( value == NULL )
			_fatal( "Missing ControlPoint::Value\n" );


		memcpy( (*times+i),  time, sizeof(aafRational_t) );
		memcpy( (*values+i), value, sizeof(aafRational_t) );


		i++;
	}

	if ( Points->Header->_entryCount != i )
	{
		_warning( "Points _entryCount does not match iteration.\n" );

		return i;
	}

	/*
	 *	Sets pts_cnt here in case the vector has "custom objects"
	 *	TODO is this even possible ?
	 */

	// Trans->pts_cnt_a = i;

	return Points->Header->_entryCount;
}




static void parse_OperationGroup( AAF_Iface *aafi, aafObject *OpGroup )
{

	/* Retrieves Operation's Identification */

	aafWeakRef_t *OpDef = aaf_get_propertyValue( OpGroup, PID_OperationGroup_Operation );

	if ( OpDef == NULL )
		_fatal( "Missing OperationGroup::Operation.\n" );


	aafObject *OpDefObj = aaf_get_ObjectByWeakRef( aafi->aafd->OperationDefinition, OpDef );

	if ( OpDefObj == NULL )
		_fatal( "Could not retrieve OperationDefinition from dictionary.\n" );


	aafUID_t *OpIdent = aaf_get_propertyValue( OpDefObj, PID_DefinitionObject_Identification );

	if ( OpIdent == NULL )
		_fatal( "Missing DefinitionObject::Identification.\n" );




	if ( auidCmp( OpGroup->Parent->Class->ID, &AAFClassID_Transition ) )
	{

		aafiTransition *Trans = aafi->ctx.current_transition;


		if ( auidCmp( OpIdent, &AAFOperationDef_MonoAudioDissolve ) )
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
		else if ( auidCmp( OpIdent, &AAFOperationDef_TwoParameterMonoAudioDissolve ) )
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
		else if ( auidCmp( OpIdent, &AAFOperationDef_StereoAudioDissolve ) )
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
	else if ( auidCmp( OpIdent, &AAFOperationDef_MonoAudioPan ) )
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
	else if ( auidCmp( OpIdent, &AAFOperationDef_AudioChannelCombiner ) )
	{
		trace_obj( aafi, OpGroup, ANSI_COLOR_MAGENTA );

		/**************************************************************************************/

		// printObjectProperties( aafi->aafd, OpGroup );

		aafi->ctx.current_track_is_multichannel = 1;
		aafi->ctx.current_multichannel_track_channel = 0;


		aafObject *InputSegments = aaf_get_propertyValue( OpGroup, PID_OperationGroup_InputSegments );
		aafObject *InputSegment  = NULL;

		aaf_foreach_ObjectInSet( &InputSegment, InputSegments, NULL )
		{
			// printObjectProperties( aafi->aafd, InputSegment );

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

		return;
	}

	/* TODO else if () */
	else
	{
		aafiAudioGain *Gain = calloc( sizeof(aafiAudioGain), sizeof(unsigned char) );

		aafi->ctx.current_gain = Gain;


		if ( auidCmp( OpIdent, &AAFOperationDef_MonoAudioGain ) )
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
		else if ( auidCmp( OpIdent, &AAFOperationDef_MonoAudioMixdown ) )
		{

			/*
			 *	Mono Audio Mixdown
			 *
			 *	TODO Unknown usage and implementation
			 */

			trace_obj( aafi, OpGroup, ANSI_COLOR_RED );
			_warning( "AAFOperationDef_MonoAudioMixdown not supported yet.\n" );

		}
		else if ( auidCmp( OpIdent, &AAFOperationDef_StereoAudioGain ) )
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

			if ( auidCmp( InputSegment->Class->ID, &AAFClassID_SourceClip) == 0 )
			{
				continue;
			}

			aafiAudioClip *audioClip = (aafiAudioClip*)parse_SourceClip( aafi, InputSegment );

			audioClip->gain = Gain;
		}
	}

}





static void parse_Transition( AAF_Iface *aafi, aafObject *Transition )
{

	// printObjectProperties( aafi->aafd, Transition );
	// printf("\n\n" );



	int64_t *length = aaf_get_propertyValue( Transition, PID_Component_Length );

	if ( length == NULL )
		_fatal( "Missing Filler Component::Length\n" );

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



	aafPosition_t *cut_point = aaf_get_propertyValue( Transition, PID_Transition_CutPoint );

	if ( cut_point == NULL )
		_fatal( "Missing Transition::CutPoint.\n" );

	Trans->cut_pt = *cut_point;



	aafObject * OpGroup = aaf_get_propertyValue( Transition, PID_Transition_OperationGroup );

	if ( OpGroup == NULL )
		_fatal( "Missing Transition::OperationGroup.\n" );

	parse_OperationGroup( aafi, OpGroup );

}

















int aafi_retrieveData( AAF_Iface *aafi )
{

	/*
	 *	Loop through CompositionMobs
	 *
	 *	There should be only one, since a CompositionMob
	 *	represents the overall composition (i.e project).
	 *	Observations on files confirm that.
	 *
	 *	However, the AAF Edit Protocol says that there could be
	 *	multiple CompositionMobs (Mob::UsageCode TopLevel), containing
	 *	other CompositionMobs (Mob::UsageCode LowerLevel). This was
	 *	never encountered for now.
	 *
	 *	TODO implement fallback
	 */

	aaf_foreach_ObjectInSet( &(aafi->ctx.Mob), aafi->aafd->Mobs, NULL )
	{
		// 	printf("\n");
		// 	printf("Object  : %s\n", aaf_get_ObjectPath( aafi->ctx.Mob ) );
		// 	printf("ClassID : %s\n", ClassIDToText( aafi->aafd, aafi->ctx.Mob->Class->ID ) );
		// 	printf("MobName : %s\n", aaf_get_propertyValueText( aafi->ctx.Mob, PID_Mob_Name ) );
		// 	printf("\n");

		if ( auidCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_CompositionMob ) )
		{
			/******************************************************************/

			// printObjectProperties(aafi->ctx.Mob);
            //
			// aafObject *UserComments = aaf_get_propertyValue( aafi->ctx.Mob, PID_Mob_UserComments );
			// aafObject *UserComment  = NULL;
            //
			// aaf_foreach_ObjectInSet( &UserComment, UserComments, NULL )
			// {
			// 	printf("\n\n");
			// 	printObjectProperties(UserComment);
			// 	unsigned char *string = NULL;
			// 	aafIndirect_t *indirectCom = aaf_get_propertyValue(UserComment, PID_TaggedValue_Value);
			// 	unsigned char *com = indirectCom->Value;
			// 	printf("Indirect Type : %s\n", TypeIDToText( &indirectCom->TypeDef) );
			// 	printf("Indirect auid type : %s\n", printUID(&indirectCom->TypeDef) );
			// 	// utf16toa( string, 20, com, 10 );
			// 	// printf("%s\n", string );
			// 	printf("\n\n");
			// }
            //
			// aafObject *MobAttributeList = aaf_get_propertyValue( aafi->ctx.Mob, 0xfff9 );
			// aafObject *MobAttribute     = NULL;
            //
			// aaf_foreach_ObjectInSet( &MobAttribute, MobAttributeList, NULL )
			// {
			// 	printf("\nMobAttribute\n");
			// 	printObjectProperties(MobAttribute);
			// 	printf("\n\n");
			// }

			/******************************************************************/


			aafi->compositionName = aaf_get_propertyValueText( aafi->ctx.Mob, PID_Mob_Name );
		}


		/* Get MobSlots */
		aafObject *MobSlots = aaf_get_propertyValue( aafi->ctx.Mob, PID_Mob_Slots );


		/* Loop through MobSlots */
		aaf_foreach_ObjectInSet( &(aafi->ctx.MobSlot), MobSlots, NULL )
		{

			/* Get MobSlot::Segment > Component::DataDefinition */
			aafi->ctx.DataDef = getMobSlotDataDef( aafi->aafd->DataDefinition, aafi->ctx.MobSlot );

			if ( aafi->ctx.DataDef == NULL )
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
			 *	TODO Can a CompositionMob contains something different than a AAFClassID_TimelineMobSlot ???
			 */

			if ( auidCmp( aafi->ctx.MobSlot->Class->ID, &AAFClassID_TimelineMobSlot ) == 0 )
			{
				continue;
			}



			if ( auidCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_CompositionMob ) )
			{

				/* Retrieve Clips and TimeCode */

				if ( auidCmp( aafi->ctx.DataDef, &AAFDataDef_Sound ) ||
					 auidCmp( aafi->ctx.DataDef, &AAFDataDef_LegacySound ) )
				{

					/* TODO rename as aafi_addAudioTrack() */
					aafi_newAudioTrack( aafi, aafi->ctx.MobSlot, -1 );

					/* reset timeline position */
					aafi->ctx.current_pos = 0;

					aafObject *Segment = aaf_get_propertyValue( aafi->ctx.MobSlot, PID_MobSlot_Segment );

					parse_Segment( aafi, Segment );

				}
				else if ( auidCmp( aafi->ctx.DataDef, &AAFDataDef_Timecode ) ||
						  auidCmp( aafi->ctx.DataDef, &AAFDataDef_LegacyTimecode ) )
				{
					aafObject *Segment = aaf_get_propertyValue( aafi->ctx.MobSlot, PID_MobSlot_Segment );

					parse_Segment( aafi, Segment );
				}

			}
			else if ( auidCmp( aafi->ctx.Mob->Class->ID, &AAFClassID_MasterMob ) )
			{
				/* Retrieve Essences */

				if ( auidCmp( aafi->ctx.DataDef, &AAFDataDef_Sound ) ||
					 auidCmp( aafi->ctx.DataDef, &AAFDataDef_LegacySound ) )
				{
					aafObject *Segment = aaf_get_propertyValue( aafi->ctx.MobSlot, PID_MobSlot_Segment );

					parse_Segment( aafi, Segment );

					retrieve_EssenceData( aafi );
				}

			}

		}

	}

	return 0;
}

/**
 *	@}
 */

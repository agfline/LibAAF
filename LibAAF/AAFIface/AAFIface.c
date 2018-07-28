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

#include "../libAAF.h"
#include "../common/debug.h"

#include "thirdparty/libriff.h"
#include "thirdparty/libwav.h"










AAF_Iface * aafi_alloc( AAF_Data *aafd )
{
	AAF_Iface *aafi = calloc( sizeof(AAF_Iface), sizeof(unsigned char) );

	if ( aafi == NULL )
	{
		_error( "%s.\n", strerror( errno ) );
		return NULL;
	}


	aafi->Audio = calloc( 1, sizeof(aafiAudio) );

	if ( aafi->Audio == NULL )
	{
		_error( "%s.\n", strerror( errno ) );
		return NULL;
	}


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

	if ( (*aafi)->Comments )
	{
		aafi_freeUserComments( &((*aafi)->Comments) );
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








static void swap( unsigned char *tab, int i, int j )
{
	unsigned char c = tab[i];
	tab[i] = tab[j];
	tab[j] = c;
}

int extractAudioEssence( AAF_Iface *aafi, aafiAudioEssence *audioEssence, const char *file )
{

	/*
	 *	If the audioEssence->node is NULL, it means that the essence isn't embedded in the
	 *	AAF. The essence file can therefore be retrieved from the URI hold by audioEssence->original_file.
	 */

	if ( audioEssence->node == NULL )
		return 1;

	char filePath[1024];

	snprintf( filePath, 1024, "%s.wav", file );

	// printf("%s\n", filePath );

	FILE *fp = fopen( filePath, "w+" );

	if ( fp == NULL )
	{
		_error( "%s.\n", strerror( errno ) );
		return -1;
	}

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

	cfb_foreachSectorInStream( aafi->aafd->cfbd, audioEssence->node, &nodeBuf, &len, &id )
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

	// save filename
	audioEssence->source_file = malloc( strlen( file ) + 2 );

	if ( audioEssence->source_file == NULL )
	{
		_error( "%s.\n", strerror( errno ) );
		return -1;
	}

	snprintf( audioEssence->source_file, strlen( file ) + 1, "%s", file );


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
		{
			_error( "%s.\n", strerror( errno ) );
			return NULL;
		}


		item->type |= AAFI_CLIP;

		aafiAudioClip *audioClip = (aafiAudioClip*)&item->data;

		audioClip->track = track;
	}
	else if ( itemType == AAFI_TRANS )
	{
		item = calloc( sizeof(aafiTimelineItem) + sizeof(aafiTransition), 1 );

		if ( item == NULL )
		{
			_error( "%s.\n", strerror( errno ) );
			return NULL;
		}

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




void aafi_freeTimelineItems( aafiTimelineItem **items )
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
					free( audioClip->gain->time );
				}

				if ( audioClip->gain->value != NULL )
				{
					free( audioClip->gain->value );
				}

				free( audioClip->gain );
			}
		}

		free( item );
	}

	*items = NULL;
}








aafiUserComment * aafi_newUserComment( aafiUserComment **CommentList )
{

	aafiUserComment *UserComment = calloc( sizeof(aafiUserComment),  1 );

	if ( UserComment == NULL )
	{
		_error( "%s.\n", strerror( errno ) );
		return NULL;
	}


	if ( CommentList != NULL )
	{
		UserComment->next = *CommentList;
		*CommentList = UserComment;
	}
	else
	{
		*CommentList = UserComment;
	}


	return UserComment;
}




void aafi_freeUserComments( aafiUserComment **CommentList )
{
	aafiUserComment *UserComment = *CommentList;
	aafiUserComment *tmp = NULL;

	while( UserComment != NULL )
	{
		tmp = UserComment;
		UserComment = UserComment->next;

		if ( tmp->name != NULL )
		{
			free( tmp->name );
		}

		if ( tmp->text != NULL )
		{
			free( tmp->text );
		}

		free( tmp );
	}

	*CommentList = NULL;
}








void aafi_freeTransition( aafiTransition *Transition )
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
	{
		_error( "%s.\n", strerror( errno ) );
		return NULL;
	}


	track->next = NULL;

	track->format = AAFI_TRACK_FORMAT_MONO;


	if ( MobSlot != NULL )
	{
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


		track->name = aaf_get_propertyValueText( MobSlot, PID_MobSlot_SlotName );


		track->edit_rate = aaf_get_propertyValue( MobSlot, PID_TimelineMobSlot_EditRate );

		if ( track->edit_rate == NULL )
		{
			_error( "%s.\n", strerror( errno ) );
			return NULL;
		}
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




void aafi_freeAudioTracks( aafiAudioTrack **tracks )
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
	{
		_error( "%s.\n", strerror( errno ) );
		return NULL;
	}


	// printf( "%p \n", audioEssence );
	// printf( "%p \n", aafi->Audio );
	// printf( "%p \n", aafi->Audio->Essences );

	audioEssence->next = aafi->Audio->Essences;

	audioEssence->original_file = NULL;
	audioEssence->source_file   = NULL;
	audioEssence->file_name     = aaf_get_propertyValueText( aafi->ctx.Mob, PID_Mob_Name );

	aafi->Audio->Essences = audioEssence;

	return audioEssence;
}




void aafi_freeAudioEssences( aafiAudioEssence **audioEssence )
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

		if ( (*audioEssence)->original_file != NULL )
		{
			free( (*audioEssence)->original_file );
		}

		if ( (*audioEssence)->source_file != NULL )
		{
			free( (*audioEssence)->source_file );
		}

		if ( (*audioEssence)->file_name != NULL )
		{
			free( (*audioEssence)->file_name );
		}

		free( *audioEssence );
	}

	*audioEssence = NULL;
}



/**
 *	@}
 */

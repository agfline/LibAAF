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

#include <libaaf/debug.h>
#include <libaaf/AAFIface.h>
#include <libaaf/AAFIParser.h>









AAF_Iface * aafi_alloc( AAF_Data *aafd )
{
	AAF_Iface *aafi = calloc( sizeof(AAF_Iface), sizeof(unsigned char) );

	if ( aafi == NULL )
	{
		_error( aafi->ctx.options.verb, "%s.\n", strerror( errno ) );
		return NULL;
	}


	aafi->Audio = malloc( sizeof(aafiAudio) );

	if ( aafi->Audio == NULL )
	{
		_error( aafi->ctx.options.verb, "%s.\n", strerror( errno ) );
		return NULL;
	}

	aafi->Audio->Essences = NULL;
	aafi->Audio->tc = NULL;
	aafi->Audio->samplerate = 0;
	aafi->Audio->samplesize = 0;
	aafi->Audio->Tracks = NULL;
	aafi->Audio->track_count = 0;
	aafi->Audio->length = 0;


	aafi->Video = malloc( sizeof(aafiVideo) );

	if ( aafi->Video == NULL )
	{
		_error( aafi->ctx.options.verb, "%s.\n", strerror( errno ) );
		return NULL;
	}

	aafi->Video->Essences = NULL;
	aafi->Video->tc = NULL;
	aafi->Video->Tracks = NULL;
	aafi->Video->length = 0;


	if ( aafd != NULL )
	{
		aafi->aafd = aafd;
	}
	else
	{
		aafi->aafd = aaf_alloc();
	}

	aafi->Markers = NULL;

	aafi->compositionName = NULL;

	aafi->ctx.is_inside_derivation_chain = 0;

	aafi->ctx.options.verb = VERB_QUIET;
	aafi->ctx.options.trace = 0;
	// aafi->ctx.trace_leveloop = NULL;

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



	if ( (*aafi)->Video != NULL )
	{
		if ( (*aafi)->Video->Tracks != NULL )
		{
			aafi_freeVideoTracks( &(*aafi)->Video->Tracks );
		}

		if ( (*aafi)->Video->Essences != NULL )
		{
			aafi_freeVideoEssences( &(*aafi)->Video->Essences );
		}

		if ( (*aafi)->Video->tc != NULL )
		{
			// free( (*aafi)->Video->tc );
		}

		free( (*aafi)->Video );
	}

	if ( (*aafi)->Markers ) {
		aafi_freeMarkers( &(*aafi)->Markers );
	}

	// if ( (*aafi)->ctx.trace_leveloop )
	// 	free( (*aafi)->ctx.trace_leveloop );


	free( *aafi );

	*aafi = NULL;
}




int aafi_load_file( AAF_Iface *aafi, const char * file )
{
	aafi->aafd->verb = aafi->ctx.options.verb;

	if ( aaf_load_file( aafi->aafd, file ) )
	{
		// aaf_release( &aafi->aafd );
		// printf("aafi_load_file() : aaf_load_file() sets aafi->aafd : %p\n", aafi->aafd );
		return 1;
	}

	aafi_retrieveData( aafi );

	return 0;
}




aafiTransition * get_fadein( aafiTimelineItem *audioItem )
{

	if ( audioItem->prev != NULL &&
			 audioItem->prev->type == AAFI_TRANS )
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
			 audioItem->next->type == AAFI_TRANS )
	{
		aafiTransition *Trans = (aafiTransition*)(audioItem->next->data);

		if ( Trans->flags & AAFI_TRANS_FADE_OUT )
			return (aafiTransition*)(audioItem->next->data);
	}

	return NULL;
}




aafiTransition * get_xfade( aafiTimelineItem *audioItem )
{

// 	if ( audioItem->next != NULL &&
// 		 audioItem->next->type & AAFI_TRANS )
// 	{
// 		aafiTransition *Trans = (aafiTransition*)(audioItem->next->data);
//
// 		if ( Trans->flags & AAFI_TRANS_XFADE )
// 			return (aafiTransition*)(audioItem->next->data);
// 	}


  if ( audioItem->prev != NULL &&
			 audioItem->prev->type == AAFI_TRANS )
	{
		aafiTransition *Trans = (aafiTransition*)(audioItem->prev->data);

		if ( Trans->flags & AAFI_TRANS_XFADE )
			return (aafiTransition*)(audioItem->prev->data);
	}

	return NULL;
}






aafiMarker * aafi_newMarker( AAF_Iface *aafi, aafRational_t *editRate, aafPosition_t start, aafPosition_t length, wchar_t *name, wchar_t *comment, uint16_t *(RVBColor[3]) )
{
	aafiMarker *marker = malloc( sizeof(aafiMarker) );

	marker->edit_rate = editRate;
	marker->start = start;
	marker->length = length;

	marker->name = name;
	marker->comment = comment;

	marker->prev = NULL;
	marker->next = NULL;

	if ( RVBColor ) {
		marker->RVBColor[0] = (*RVBColor)[0];
		marker->RVBColor[1] = (*RVBColor)[1];
		marker->RVBColor[2] = (*RVBColor)[2];
	}

	if ( aafi->Markers != NULL ) {

		aafiMarker *tmp = aafi->Markers;

		for (; tmp != NULL; tmp = tmp->next )
			if ( tmp->next == NULL )
				break;

		tmp->next = marker;
		marker->prev = marker;
	}
	else
	{
		aafi->Markers = marker;
		marker->prev = NULL;
	}

	return marker;
}




void aafi_freeMarkers( aafiMarker **Markers )
{
	aafiMarker *marker = NULL;
	aafiMarker *nextMarker = NULL;

	for ( marker = (*Markers); marker != NULL; marker = nextMarker )
	{
		nextMarker = marker->next;

		if ( marker->name )
			free( marker->name );

		if ( marker->comment )
			free( marker->comment );

		free( marker );
	}

	*Markers = NULL;
}






aafiTimelineItem * aafi_newTimelineItem( AAF_Iface *aafi, void *track, int itemType )
{

	aafiTimelineItem *item = NULL;

	if ( itemType == AAFI_AUDIO_CLIP )
	{
		item = calloc( sizeof(aafiTimelineItem) + sizeof(aafiAudioClip),  1 );

		if ( item == NULL )
		{
			_error( aafi->ctx.options.verb, "%s.\n", strerror( errno ) );
			return NULL;
		}


		item->type = AAFI_AUDIO_CLIP;

		aafiAudioClip *audioClip = (aafiAudioClip*)&item->data;

		audioClip->track = (aafiAudioTrack*)track;
		audioClip->Item = item;
	}
	else if ( itemType == AAFI_VIDEO_CLIP )
	{
		item = calloc( sizeof(aafiTimelineItem) + sizeof(aafiVideoClip),  1 );

		if ( item == NULL )
		{
			_error( aafi->ctx.options.verb, "%s.\n", strerror( errno ) );
			return NULL;
		}


		item->type = AAFI_VIDEO_CLIP;

		aafiVideoClip *videoClip = (aafiVideoClip*)&item->data;

		videoClip->track = (aafiVideoTrack*)track;
	}
	else if ( itemType == AAFI_TRANS )
	{
		item = calloc( sizeof(aafiTimelineItem) + sizeof(aafiTransition), 1 );

		if ( item == NULL )
		{
			_error( aafi->ctx.options.verb, "%s.\n", strerror( errno ) );
			return NULL;
		}

		item->type = AAFI_TRANS;
	}


	if ( itemType == AAFI_AUDIO_CLIP || itemType == AAFI_TRANS )
	{
		if ( track != NULL )
		{
			/*
			 *	Add to track's item list
			 */

			if ( ((aafiAudioTrack*)track)->Items != NULL )
			{
				aafiTimelineItem *tmp = ((aafiAudioTrack*)track)->Items;

				for (; tmp != NULL; tmp = tmp->next )
					if ( tmp->next == NULL )
						break;

				tmp->next  = item;
				item->prev = tmp;
			}
			else
			{
				((aafiAudioTrack*)track)->Items = item;
				item->prev = NULL;
			}
		}
	}
	else if ( itemType == AAFI_VIDEO_CLIP )
	{
		if ( track != NULL )
		{
			/*
			 *	Add to track's item list
			 */

			if ( ((aafiVideoTrack*)track)->Items != NULL )
			{
				aafiTimelineItem *tmp = ((aafiVideoTrack*)track)->Items;

				for (; tmp != NULL; tmp = tmp->next )
					if ( tmp->next == NULL )
						break;

				tmp->next  = item;
				item->prev = tmp;
			}
			else
			{
				((aafiVideoTrack*)track)->Items = item;
				item->prev = NULL;
			}
		}
	}


	return item;
}



int aafi_removeTimelineItem( AAF_Iface *aafi, aafiTimelineItem *item ) {

	if ( item->prev != NULL ) {
		item->prev->next = item->next;
	}

	if ( item->next != NULL ) {
		item->next->prev = item->prev;
	}


	aafiAudioTrack *audioTrack = NULL;

	foreach_audioTrack( audioTrack, aafi ) {
		if ( audioTrack->Items == item ) {
			audioTrack->Items = item->next;
		}
	}


	aafi_freeTimelineItem( &item );

	return 0;
}



void aafi_freeAudioGain( aafiAudioGain *gain )
{
	if ( gain == NULL )
	{
		return;
	}


	if ( gain->time != NULL )
	{
		free( gain->time );
	}

	if ( gain->value != NULL )
	{
		free( gain->value );
	}

	free( gain );
}



void aafi_freeAudioPan( aafiAudioPan *pan )
{
	aafi_freeAudioGain( (aafiAudioGain*)pan );
}



void aafi_freeAudioClip( aafiAudioClip *audioClip )
{
	if ( audioClip->gain != NULL )
	{
		aafi_freeAudioGain( audioClip->gain );
	}

	if ( audioClip->automation != NULL )
	{
		aafi_freeAudioGain( audioClip->automation );
	}

	// if ( audioClip->gain != NULL )
	// {
	// 	if ( audioClip->gain->time != NULL )
	// 	{
	// 		free( audioClip->gain->time );
	// 	}
	//
	// 	if ( audioClip->gain->value != NULL )
	// 	{
	// 		free( audioClip->gain->value );
	// 	}
	// }
	//
	//
	// free( audioClip->gain );
}





void aafi_freeTimelineItem( aafiTimelineItem **item )
{

	if ( (*item)->type == AAFI_TRANS )
	{
		aafi_freeTransition( (aafiTransition*)&((*item)->data) );
	}
	else if ( (*item)->type == AAFI_AUDIO_CLIP )
	{
		aafi_freeAudioClip( (aafiAudioClip*)(*item)->data );
	}
	else if ( (*item)->type == AAFI_VIDEO_CLIP )
	{
		// aafi_freeAudioClip( (aafiVideoClip*)(*item)->data );
	}

	free( *item );

	*item = NULL;
}




void aafi_freeTimelineItems( aafiTimelineItem **items )
{
	aafiTimelineItem *item = NULL;
	aafiTimelineItem *nextItem = NULL;

	for ( item = (*items); item != NULL; item = nextItem )
	{
		nextItem = item->next;

		aafi_freeTimelineItem( &item );
	}

	*items = NULL;
}








aafiUserComment * aafi_newUserComment( AAF_Iface *aafi, aafiUserComment **CommentList )
{

	aafiUserComment *UserComment = calloc( sizeof(aafiUserComment),  1 );

	if ( UserComment == NULL )
	{
		_error( aafi->ctx.options.verb, "%s.\n", strerror( errno ) );
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








aafiAudioTrack * aafi_newAudioTrack( AAF_Iface *aafi )
{
	aafiAudioTrack *track = calloc( sizeof(aafiAudioTrack), sizeof(unsigned char) );

	if ( track == NULL )
	{
		_error( aafi->ctx.options.verb, "%s.\n", strerror( errno ) );
		return NULL;
	}


	track->next = NULL;

	track->pan = NULL;

	track->gain = NULL;

	track->format = AAFI_TRACK_FORMAT_MONO;



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

	track->current_pos = 0;

	// aafi->ctx.current_track = track;
	// aafi->ctx.current_track_number++;

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

		if ( track->gain != NULL )
		{
			aafi_freeAudioGain( track->gain );

			// if ( track->gain->time != NULL )
			// {
			// 	free( track->gain->time );
			// }
			//
			// if ( track->gain->value != NULL )
			// {
			// 	free( track->gain->value );
			// }
			//
			// free( track->gain );
		}

		if ( track->pan != NULL )
		{
			aafi_freeAudioPan( track->pan );
			// if ( track->pan->time != NULL )
			// {
			// 	free( track->pan->time );
			// }
			//
			// if ( track->pan->value != NULL )
			// {
			// 	free( track->pan->value );
			// }
			//
			// free( track->pan );

			// free( track->pan->time );
			// free( track->pan->value );
			// free( track->pan );
		}

		if ( track->Items != NULL )
		{
			aafi_freeTimelineItems( &(track->Items) );
		}

		free( track );
	}

	*tracks = NULL;
}



aafiVideoTrack * aafi_newVideoTrack( AAF_Iface *aafi )
{
	aafiVideoTrack *track = calloc( sizeof(aafiVideoTrack), sizeof(unsigned char) );

	if ( track == NULL )
	{
		_error( aafi->ctx.options.verb, "%s.\n", strerror( errno ) );
		return NULL;
	}


	track->next = NULL;

	// track->pan = NULL;

	// track->gain = NULL;

	// track->format = AAFI_TRACK_FORMAT_MONO;



	/*
	 *	Add to track list
	 */

	if ( aafi->Video->Tracks != NULL )
	{
		aafiVideoTrack *tmp = aafi->Video->Tracks;

		for (; tmp != NULL; tmp = tmp->next )
			if ( tmp->next == NULL )
				break;

		tmp->next = track;
	}
	else
	{
		aafi->Video->Tracks = track;
	}


	track->Video = aafi->Video;

	track->current_pos = 0;

	// aafi->ctx.current_track = track;


	return track;
}

void aafi_freeVideoTracks( aafiVideoTrack **tracks )
{
	if ( *(tracks) == NULL )
	{
		return;
	}

	aafiVideoTrack *track = NULL;
	aafiVideoTrack *nextTrack = NULL;

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
		_error( aafi->ctx.options.verb, "%s.\n", strerror( errno ) );
		return NULL;
	}


	audioEssence->next = aafi->Audio->Essences;

	audioEssence->original_file_path = NULL;
	audioEssence->usable_file_path = NULL;
	audioEssence->file_name = NULL;
	audioEssence->unique_file_name = NULL;
	audioEssence->clip_count = 0;

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

		if ( (*audioEssence)->original_file_path != NULL )
		{
			free( (*audioEssence)->original_file_path );
		}

		if ( (*audioEssence)->usable_file_path != NULL )
		{
			free( (*audioEssence)->usable_file_path );
		}

		if ( (*audioEssence)->file_name != NULL )
		{
			free( (*audioEssence)->file_name );
		}

		if ( (*audioEssence)->unique_file_name != NULL )
		{
			free( (*audioEssence)->unique_file_name );
		}

		free( *audioEssence );
	}

	*audioEssence = NULL;
}




aafiVideoEssence * aafi_newVideoEssence( AAF_Iface *aafi )
{
	aafiVideoEssence * videoEssence = calloc( sizeof(aafiVideoEssence), sizeof(char) );

	if ( videoEssence == NULL )
	{
		_error( aafi->ctx.options.verb, "%s.\n", strerror( errno ) );
		return NULL;
	}


	videoEssence->next = aafi->Video->Essences;

	videoEssence->original_file_path = NULL;
	videoEssence->usable_file_path = NULL;
	videoEssence->file_name = NULL;
	videoEssence->unique_file_name = NULL;

	aafi->Video->Essences = videoEssence;

	return videoEssence;
}


void aafi_freeVideoEssences( aafiVideoEssence **videoEssence )
{
	if ( *(videoEssence) == NULL )
	{
		return;
	}

	// aafiAudioEssence *audioEssence = NULL;
	aafiVideoEssence *nextVideoEssence = NULL;

	for (; (*videoEssence) != NULL; *videoEssence = nextVideoEssence )
	{
		nextVideoEssence = (*videoEssence)->next;

		if ( (*videoEssence)->original_file_path != NULL )
		{
			free( (*videoEssence)->original_file_path );
		}

		if ( (*videoEssence)->usable_file_path != NULL )
		{
			free( (*videoEssence)->usable_file_path );
		}

		if ( (*videoEssence)->file_name != NULL )
		{
			free( (*videoEssence)->file_name );
		}

		if ( (*videoEssence)->unique_file_name != NULL )
		{
			free( (*videoEssence)->unique_file_name );
		}

		free( *videoEssence );
	}

	*videoEssence = NULL;
}



/**
 *	@}
 */

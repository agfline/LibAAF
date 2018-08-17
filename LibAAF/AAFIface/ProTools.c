
#include <stdio.h>

#include "../libAAF.h"

#include "./ProTools.h"


static int is_rendered_fade( const wchar_t *clipName )
{
	// uint32_t clipNameLen = wcslen( clipName ) + 1;
	// uint32_t clipNameByteLen = clipNameLen * sizeof(wchar_t);

	// switch ( clipNameLen )
	// {
	// 	case PROTOOLS_CLIP_NAME_FADE_EN_LEN:
	// 	// case PROTOOLS_CLIP_NAME_FADE_DE_LEN:
	// 	// case PROTOOLS_CLIP_NAME_FADE_JA_LEN:

			if ( memcmp( clipName, PROTOOLS_CLIP_NAME_FADE_EN, PROTOOLS_CLIP_NAME_FADE_EN_LEN ) == 0 )
			{
				return 1;
			}

		// 	break;
        //
		// case PROTOOLS_CLIP_NAME_FADE_ES_LEN:

			if ( memcmp( clipName, PROTOOLS_CLIP_NAME_FADE_ES, PROTOOLS_CLIP_NAME_FADE_ES_LEN ) == 0 )
			{
				return 1;
			}

		// 	break;
        //
		// case PROTOOLS_CLIP_NAME_FADE_FR_LEN:

			if ( memcmp( clipName, PROTOOLS_CLIP_NAME_FADE_FR, PROTOOLS_CLIP_NAME_FADE_FR_LEN ) == 0 )
			{
				return 1;
			}

		// 	break;
        //
		// case PROTOOLS_CLIP_NAME_FADE_ZH_CN_LEN:
		// // case PROTOOLS_CLIP_NAME_FADE_ZH_TW_LEN:
		// // case PROTOOLS_CLIP_NAME_FADE_KO_LEN:

			if ( memcmp( clipName, PROTOOLS_CLIP_NAME_FADE_ZH_CN, PROTOOLS_CLIP_NAME_FADE_ZH_CN_LEN ) == 0 )
			{
				return 1;
			}
			else if ( memcmp( clipName, PROTOOLS_CLIP_NAME_FADE_ZH_TW, PROTOOLS_CLIP_NAME_FADE_ZH_TW_LEN ) == 0 )
			{
				return 1;
			}
			else if ( memcmp( clipName, PROTOOLS_CLIP_NAME_FADE_KO, PROTOOLS_CLIP_NAME_FADE_KO_LEN ) == 0 )
			{
				return 1;
			}

	// 		break;
    //
    //
	// 	default:
	// 		break;
	// }

	return 0;
}




static int is_sample_accurate_edit( const wchar_t *clipName )
{
	uint32_t clipNameLen = wcslen( clipName ) + 1;
	uint32_t clipNameByteLen = clipNameLen * sizeof(wchar_t);

	switch ( clipNameLen )
	{
		case PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_EN_LEN:

			if ( memcmp( clipName, PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_EN, clipNameByteLen ) == 0 )
			{
				return 1;
			}

			break;

		case PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_DE_LEN:

			if ( memcmp( clipName, PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_DE, clipNameByteLen ) == 0 )
			{
				return 1;
			}

			break;

		case PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_ES_LEN:

			if ( memcmp( clipName, PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_ES, clipNameByteLen ) == 0 )
			{
				return 1;
			}

			break;

		case PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_FR_LEN:

			if ( memcmp( clipName, PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_FR, clipNameByteLen ) == 0 )
			{
				return 1;
			}

			break;

		case PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_JA_LEN:

			if ( memcmp( clipName, PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_JA, clipNameByteLen ) == 0 )
			{
				return 1;
			}

			break;

		case PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_ZH_CN_LEN:
		// case PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_ZH_TW_LEN:

			if ( memcmp( clipName, PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_ZH_CN, clipNameByteLen ) == 0 )
			{
				return 1;
			}
			else if ( memcmp( clipName, PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_ZH_TW, clipNameByteLen ) == 0 )
			{
				return 1;
			}

			break;

		case PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_KO_LEN:

			if ( memcmp( clipName, PROTOOLS_CLIP_NAME_SAMPLE_ACCURATE_EDIT_KO, clipNameByteLen ) == 0 )
			{
				return 1;
			}

			break;


		default:
			break;
	}

	return 0;
}




int aafi_remove_trackitem( AAF_Iface *aafi, aafiTimelineItem *Item )
{

	if ( Item->prev != NULL )
	{
		Item->prev->next = Item->next;
	}

	if ( Item->next != NULL )
	{
		Item->next->prev = Item->prev;
	}



	aafiAudioTrack *audioTrack = NULL;

	foreach_audioTrack( audioTrack, aafi )
	{
		if ( audioTrack->Items == Item )
		{
			audioTrack->Items = Item->next;
		}
	}



	aafi_freeTimelineItem( &Item );

	return 0;
}




static int replace_clipfade_with_fade( AAF_Iface *aafi, aafiTimelineItem *Item )
{
	if ( Item->type != AAFI_CLIP )
	{
		return -1;
	}


	aafiAudioClip *audioClip = (aafiAudioClip*)Item->data;

	aafPosition_t currentpos = audioClip->pos;
	aafPosition_t currentlen = audioClip->len;



	aafiTimelineItem *transItem = malloc( sizeof(aafiTimelineItem) + sizeof(aafiTransition) );
	aafiTransition   *trans = (aafiTransition*)transItem->data;

	memset( transItem, 0x00, sizeof(aafiTimelineItem) + sizeof(aafiTransition) );

	transItem->type = AAFI_TRANS;
	transItem->next = NULL;
	transItem->prev = NULL;

	trans->len = audioClip->len;
	trans->flags = AAFI_INTERPOL_CONSTANT;



	// printf("\n\n");
	// printf("%ls\n", audioClip->Essence->unique_file_name );

	aafiAudioClip *prevClip = NULL;
	aafiAudioClip *nextClip = NULL;

	if ( Item->prev != NULL )
	{
		if ( Item->prev->type == AAFI_CLIP )
		{
			prevClip = (aafiAudioClip*)Item->prev->data;

			// printf("PREVIOUS POS %lu\n", prevClip->pos + prevClip->len );
			// printf("CURENT   POS %lu\n\n", currentpos );

			if ( prevClip->pos + prevClip->len < currentpos - 1 )
			{
				prevClip = NULL;
			}
		}
	}

	if ( Item->next != NULL )
	{
		if ( Item->next->type == AAFI_CLIP )
		{
			nextClip = (aafiAudioClip*)Item->next->data;

			if ( is_sample_accurate_edit( nextClip->Essence->file_name ) )
			{
				if ( Item->next->next != NULL )
				{
					nextClip = (aafiAudioClip*)Item->next->next->data;

					// printf("NEXT   POS %lu\n", nextClip->pos );
					// printf("CURENT POS %lu\n\n", currentpos + currentlen );

					if ( nextClip->pos != currentpos + currentlen + 1 )
					{
						nextClip = NULL;
					}
				}
				else
				{
					nextClip = NULL;
				}
			}
			else
			{
				// nextClip = (aafiAudioClip*)Item->next->data;

				// printf("NEXT   POS %lu\n", nextClip->pos );
				// printf("CURENT POS %lu\n\n", currentpos + currentlen );

				if ( nextClip->pos != currentpos + currentlen )
				{
					nextClip = NULL;
				}
			}
		}
	}



	trans->time_a  = calloc( 2, sizeof(aafRational_t) );
	trans->value_a = calloc( 2, sizeof(aafRational_t) );

	trans->time_a[0].numerator   = 0;
	trans->time_a[0].denominator = 0;
	trans->time_a[1].numerator   = 1;
	trans->time_a[1].denominator = 1;

	if ( prevClip && nextClip )
	{
		// printf(":: XFADE\n");
		trans->flags |= AAFI_TRANS_XFADE;

		trans->value_a[0].numerator   = 0;
		trans->value_a[0].denominator = 0;
		trans->value_a[1].numerator   = 1;
		trans->value_a[1].denominator = 1;
	}
	else if ( prevClip )
	{
		// printf(":: FADE OUT\n");
		trans->flags |= AAFI_TRANS_FADE_OUT;

		trans->value_a[0].numerator   = 1;
		trans->value_a[0].denominator = 1;
		trans->value_a[1].numerator   = 0;
		trans->value_a[1].denominator = 0;
	}
	else if ( nextClip )
	{
		// printf(":: FADE IN\n");
		trans->flags |= AAFI_TRANS_FADE_IN;

		trans->value_a[0].numerator   = 0;
		trans->value_a[0].denominator = 0;
		trans->value_a[1].numerator   = 1;
		trans->value_a[1].denominator = 1;
	}



	if ( Item->prev )
	{
		Item->prev->next = transItem;
		transItem->prev = Item->prev;
	}
	else
	{
		aafiAudioTrack *audioTrack = NULL;

		foreach_audioTrack( audioTrack, aafi )
		{
			if ( audioTrack->Items == Item )
			{
				audioTrack->Items = transItem;
			}
		}

		transItem->prev = NULL;
	}

	if ( Item->next )
	{
		Item->next->prev = transItem;
	}


	transItem->next = Item->next;


	aafi_freeTimelineItem( &Item );


	// printf("\n\n");

	return 0;
}




int protools_post_processing( AAF_Iface *aafi, uint32_t flags )
{
	aafiAudioTrack   *audioTrack = NULL;

	foreach_audioTrack( audioTrack, aafi )
	{

		aafiTimelineItem *audioItem = audioTrack->Items;

		while ( audioItem != NULL )
		{
			if ( audioItem->type != AAFI_CLIP )
			{
				audioItem = audioItem->next;
				continue;
			}


			aafiAudioClip *audioClip = (aafiAudioClip*)audioItem->data;

			wchar_t *clipName = audioClip->Essence->file_name;

			if ( (flags & PROTOOLS_PP_REPLACE_RENDERED_CLIP_FADES) && is_rendered_fade( clipName ) )
			{
				replace_clipfade_with_fade( aafi, audioItem );

				audioItem = audioTrack->Items;
				continue;
			}
			else if ( (flags & PROTOOLS_PP_REMOVE_SAMPLE_ACCURATE_EDIT) && is_sample_accurate_edit( clipName ) )
			{
				aafi_remove_trackitem( aafi, audioItem );

				audioItem = audioTrack->Items;
				continue;
			}


			audioItem = audioItem->next;
		}
	}

	return 0;
}

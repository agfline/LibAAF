#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../LibAAF/libAAF.h"

#include "./Ardour/Ardour.h"





int main( int argc, char *argv[] )
{
	if ( argc < 2 )
		return 1;


	AAF_Iface *aafi = aafi_alloc( NULL );


	if ( aafi_load_file( aafi, argv[argc-1] ) )
	{
		return 1;
	}


	// retrieveEssences( aafi );

	// retrieveClips( aafi );



	char progPath[1024];
	memset(progPath, 0x00, 1024);

	readlink( "/proc/self/exe", progPath, 1024 );

	progPath[strlen(progPath)-10] = 0x00;


	aafiAudioEssence *audioEssence = NULL;
	char *filename = NULL;
	char  target[255];
	uint32_t i = 0;

	foreachAudioEssence( audioEssence, aafi->Audio->Essences )
	{
		// char file[255];

		// snprintf( file, 255, "%sArdour/session/interchange/AAF/audiofiles/%s_%d", progPath, aafi->compositionName, i++ );

		aafi_get_essence_filename( audioEssence, &filename, aafi->compositionName, &i );

		snprintf( target, 255, "%sArdour/session/interchange/AAF/audiofiles/%s", progPath, filename );

		extractAudioEssence( aafi, audioEssence, target );
	}

	free( filename );




	size_t buf_sz = 400000;

	char *buf = malloc( buf_sz );
	memset( buf, 0x00, buf_sz );

	size_t offset = 0;





	offset += snprintf( buf+offset, buf_sz-offset, "  <Sources>\n" );


	aafiAudioTrack   *audioTrack = NULL;
	aafiTimelineItem *audioItem  = NULL;
	aafiAudioClip    *audioClip  = NULL;
	aafiAudioEssence *audioMedia = NULL;

	foreachAudioEssence( audioMedia, aafi->Audio->Essences )
	{
		offset += snprintf( buf+offset, buf_sz-offset, "    <Source name=\"%s.wav\" type=\"audio\" flags=\"\" id=\"%u\" captured-for=\"PAD 1\" channel=\"0\" origin=\"\" gain=\"1\"/>\n",
						audioMedia->file_name,
						(uint16_t)((uint64_t)((&audioMedia->sourceMobID)) & 0xffff) );

	}

	offset += snprintf( buf+offset, buf_sz-offset, "  </Sources>\n" );




	offset += snprintf( buf+offset, buf_sz-offset, "  <Regions>\n" );

	foreach_audioTrack( audioTrack, aafi )
	{

		foreach_audioItem( audioItem, audioTrack )
		{

			if ( audioItem->type != AAFI_CLIP )
				continue;

			audioClip = (aafiAudioClip*)&audioItem->data;

			offset += snprintf( buf+offset, buf_sz-offset, "<Region name=\"%s\" muted=\"0\" opaque=\"1\" locked=\"0\" video-locked=\"0\" automatic=\"1\" whole-file=\"1\" import=\"0\" external=\"0\" sync-marked=\"0\" left-of-split=\"0\" right-of-split=\"0\" hidden=\"0\" position-locked=\"0\" valid-transients=\"0\" start=\"%li\" length=\"%li\" position=\"%li\" beat=\"0\" sync-position=\"0\" ancestral-start=\"0\" ancestral-length=\"0\" stretch=\"1\" shift=\"1\" positional-lock-style=\"AudioTime\" layering-index=\"0\" envelope-active=\"0\" default-fade-in=\"0\" default-fade-out=\"0\" fade-in-active=\"1\" fade-out-active=\"1\" scale-amplitude=\"1\" id=\"%u\" type=\"audio\" first-edit=\"nothing\" source-0=\"%u\" master-source-0=\"%u\" channels=\"%u\"/>\n",
	 					audioClip->Essence->file_name,
/*						ac->essenceStartOffset * (48000/25),*/
						eu2sample( audioClip, audioClip->essence_offset ),
/*						ac->length * (48000/25),*/
						eu2sample( audioClip, audioClip->len ),
/*						ac->timelinePos * (48000/25),*/
						eu2sample( audioClip, (audioClip->pos + audioClip->track->Audio->tc->start) ),
						(uint16_t)((uint64_t)(&audioClip->Essence->sourceMobID) & 0xffff ) + 1,
						(uint16_t)((uint64_t)(&audioClip->Essence->sourceMobID) & 0xffff ),
						(uint16_t)((uint64_t)(&audioClip->Essence->sourceMobID) & 0xffff ),
						audioClip->track->number );
		}
	}

	offset += snprintf( buf+offset, buf_sz-offset, "  </Regions>\n" );







	offset += snprintf( buf+offset, buf_sz-offset, "  <Playlists>\n" );
	int id = 0;

	foreach_audioTrack( audioTrack, aafi )
	{

		offset += snprintf( buf+offset, buf_sz-offset, "    <Playlist id=\"18772\" name=\"Audio %i.1\" type=\"audio\" orig-track-id=\"18730\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\">\n", audioTrack->number /*i*/ );

			foreach_audioItem( audioItem, audioTrack )
			{

				if ( audioItem->type != AAFI_CLIP )
					continue;

				audioClip = (aafiAudioClip*)&audioItem->data;

				char name[255];

				// snprintf( name, 255, "%s.%u",
				// 			audioClip->Essence->file,
				// 			audioClip->subClipNum );

				snprintf( name, 255, "%s", audioClip->Essence->file_name );

			offset += snprintf( buf+offset, buf_sz-offset, "      <Region name=\"%s\" muted=\"0\" opaque=\"1\" locked=\"0\" video-locked=\"0\" automatic=\"1\" whole-file=\"0\" import=\"0\" external=\"0\" sync-marked=\"0\" left-of-split=\"0\" right-of-split=\"0\" hidden=\"0\" position-locked=\"0\" valid-transients=\"0\" start=\"%li\" length=\"%li\" position=\"%li\" beat=\"0\" sync-position=\"0\" ancestral-start=\"0\" ancestral-length=\"0\" stretch=\"1\" shift=\"1\" positional-lock-style=\"AudioTime\" layering-index=\"0\" envelope-active=\"0\" default-fade-in=\"0\" default-fade-out=\"0\" fade-in-active=\"1\" fade-out-active=\"1\" scale-amplitude=\"1\" id=\"%u\" type=\"audio\" first-edit=\"nothing\" source-0=\"%u\" master-source-0=\"%u\" channels=\"1\"></Region>\n",
		 					name,
/*							ac->essenceStartOffset * (48000/25),*/
							eu2sample( audioClip, audioClip->essence_offset ),
/*							ac->length * (48000/25),*/
							eu2sample( audioClip, audioClip->len ),
/*							ac->timelinePos * (48000/25),*/
							eu2sample( audioClip, (audioClip->pos + audioClip->track->Audio->tc->start) ),
							id++/*(uint16_t)(((uint64_t)(&audioClip->Essence->sourceMobID)-audioClip->subClipNum) & 0xffff)*/,
							(uint16_t)((uint64_t)(&audioClip->Essence->sourceMobID) & 0xffff),
							(uint16_t)((uint64_t)(&audioClip->Essence->sourceMobID) & 0xffff ) );
			}


		offset += snprintf( buf+offset, buf_sz-offset, "    </Playlist>\n" );
	}


	offset += snprintf( buf+offset, buf_sz-offset, "  </Playlists>\n" );

	writeArdourSessionFile( buf );

	free( buf );


	aafi_release( &aafi );

	return 0;
}

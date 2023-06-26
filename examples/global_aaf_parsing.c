/*
 * Copyright (C) 2023 Adrien Gesta-Fline
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

#include "../include/libaaf.h"

/*
 * Compile:
 *  gcc global_aaf_parsing.c -laaf -o global_aaf_parsing
 *
 * Usage:
 *  ./global_aaf_parsing myAAFFile.aaf
 */


int main( int argc, char *argv[] ) {

  if ( argc < 1 ) {
    return -1;
  }

  const char *aafFile = argv[1];


  /*
   * alloc AAFIface context
   */

  AAF_Iface *aafi = aafi_alloc( NULL );


  /*
   * load and parse AAF file
   */

  if ( aafi_load_file( aafi, aafFile ) ) {
    fprintf( stderr, "Could not load file %s\n", aafFile );
    aafi_release( &aafi );
    return -1;
  }




  aafiAudioTrack *audioTrack = NULL;

  foreach_audioTrack( audioTrack, aafi )
  {

    printf( "Track Num: %u  -  Format: %s  -  Name: \"%ls\"\n",
         audioTrack->number,
        (audioTrack->format == AAFI_TRACK_FORMAT_MONO)   ? "MONO"   :
        (audioTrack->format == AAFI_TRACK_FORMAT_STEREO) ? "STEREO" :
        (audioTrack->format == AAFI_TRACK_FORMAT_5_1)    ? "5.1"    :
        (audioTrack->format == AAFI_TRACK_FORMAT_7_1)    ? "7.1"    : "Unknown",
        (audioTrack->name != NULL) ? audioTrack->name : L""
    );


    uint32_t clipCount = 0;
    aafiTimelineItem *audioItem = NULL;

    foreach_Item( audioItem, audioTrack )
    {

      if ( audioItem->type != AAFI_AUDIO_CLIP ) {
        continue;
      }

      aafiAudioClip *audioClip = (aafiAudioClip*)&audioItem->data;
      aafiAudioEssence *audioEssence = audioClip->Essence;


      /*
       * All temporal values in AAF files are expressed in "edit units".
       * "Edit units" are defined by an "edit rate", representing the number of "edit units" per second.
       * "Edit units" are most likely equal to video FPS, or audio sample rate.
       * "Edit units" is defined per track, although it will most likely be the same accross all tracks.
       *
       * To convert from edit unit to any samples per second based value, we can use eu2sample( samplePerSecond, editRate, editUnitValue )
       */

      uint64_t clip_start_pos = eu2sample( 48000, audioClip->track->edit_rate, (audioClip->pos + audioClip->track->Audio->tc->start) );
      uint64_t clip_length    = eu2sample( 48000, audioClip->track->edit_rate, (audioClip->pos + audioClip->len + audioClip->track->Audio->tc->start) );
      uint64_t clip_end_pos   = eu2sample( 48000, audioClip->track->edit_rate,  audioClip->len );

      printf( " Clip:%u  Track:%u  "
          " Start:%lu  Len:%lu  End:%lu\n",
        clipCount,
        audioClip->track->number,
        clip_start_pos,
        clip_length,
        clip_end_pos,
        (audioClip->Essence) ? audioClip->Essence->unique_file_name : L"",
        (audioClip->Essence) ? audioClip->Essence->file_name : L""
      );


      if ( audioEssence ) {
        printf( "    Essence:  Type: %s  Duration: %u h %02u min %02u s %03u ms   %u Ch - %u Hz - %u bit  file : %ls  file_name : %ls   (%ls)\n",
          ( audioEssence->type == AAFI_ESSENCE_TYPE_PCM  ) ? "PCM"  :
          ( audioEssence->type == AAFI_ESSENCE_TYPE_WAVE ) ? "WAVE" :
          ( audioEssence->type == AAFI_ESSENCE_TYPE_AIFC ) ? "AIFC" :
          ( audioEssence->type == AAFI_ESSENCE_TYPE_BWAV ) ? "BWAV" : "",
          aeDuration_h( audioEssence ),
          aeDuration_m( audioEssence ),
          aeDuration_s( audioEssence ),
          aeDuration_ms( audioEssence ),
          audioEssence->channels,
          audioEssence->samplerate,
          audioEssence->samplesize,
          ( audioEssence->is_embedded ) ? L"EMBEDDED" : audioEssence->original_file,
          audioEssence->unique_file_name,
          audioEssence->file_name
        );
      }

      clipCount++;
    }

    printf( "\n" );
  }


  aafi_release( &aafi );

  return 0;
}

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <locale.h>

#ifdef _MSC_VER
	#include "win/getopt.h"
#else
	#include <getopt.h>
#endif

#include <libaaf.h>

#include "./thirdparty/libTC.h"
#include "../src/common/utils.h" // ANSI colors, laaf_util_c99strdup()
#include "common.h" // aafi_enable_windows_VT100_output()



#define POS_FORMAT_BUFFER_LEN 32

enum pos_format {
	POS_FORMAT_TC = 0,
	POS_FORMAT_SAMPLES,
	POS_FORMAT_HMS
};

#define INTERPOL_TO_STRING( x ) \
	(x) ? \
		(x->flags & AAFI_INTERPOL_NONE)     ? "CURV_NON" : \
		(x->flags & AAFI_INTERPOL_LINEAR)   ? "CURV_LIN" : \
		(x->flags & AAFI_INTERPOL_LOG)      ? "CURV_LOG" : \
		(x->flags & AAFI_INTERPOL_CONSTANT) ? "CURV_CST" : \
		(x->flags & AAFI_INTERPOL_POWER)    ? "CURV_PWR" : \
		(x->flags & AAFI_INTERPOL_BSPLINE)  ? "CURV_BSP" : \
		"" : "none    "

#define ESSENCE_TYPE_TO_STRING( type ) \
	( type == AAFI_ESSENCE_TYPE_PCM  ) ? "PCM " : \
	( type == AAFI_ESSENCE_TYPE_WAVE ) ? "WAVE" : \
	( type == AAFI_ESSENCE_TYPE_AIFC ) ? "AIFC" : \
	( type == AAFI_ESSENCE_TYPE_BWAV ) ? "BWAV" : ""




static const char * gainToStr( aafiAudioGain *gain ) {

	static char str[32];

	if ( gain == NULL ) {
		return "none    ";
	}
	else
	if ( gain->flags & AAFI_AUDIO_GAIN_CONSTANT ) {
		snprintf( str, 32, "%+05.1lf dB", 20 * log10( aafRationalToFloat( gain->value[0] ) ) );
		return str;
	}
	else
	if ( gain->flags & AAFI_AUDIO_GAIN_VARIABLE ) {
		return "(A)     ";
	}

	return " wtf ? ";
}



static const char * panToStr( aafiAudioPan *pan )
{
	static char str[32];

	if ( pan == NULL ) {
		return "none   ";
	}
	else if ( pan->flags & AAFI_AUDIO_GAIN_CONSTANT ) {

		float panval = aafRationalToFloat( (*pan->value) );

		snprintf( str, 32, "%0.1f %s ",
			  panval,
			( panval == 0.0 ) ? "(L)" :
			( panval == 0.5 ) ? "(C)" :
			( panval == 1.0 ) ? "(R)" : "   " );
		return str;
	}
	else if ( pan->flags & AAFI_AUDIO_GAIN_VARIABLE ) {
		return "(A)    ";
	}

	return " wtf ? ";
}



/*
 *	NOTE : since aafiAudioPan is an alias of aafiAudioGain, the
 *	function can be called for both.
 */

static void dumpVaryingValues( aafiAudioGain *Gain ) {
	for ( int i = 0; i < Gain->pts_cnt; i++ ) {
		printf( "   VaryingValue:  _time: %f   _value: %f\n",
			aafRationalToFloat( Gain->time[i] ),
			aafRationalToFloat( Gain->value[i] ) );
	}
}



static const char * formatPosValue( aafPosition_t pos, aafRational_t *editRate, enum pos_format posFormat, enum TC_FORMAT tcFormat, uint64_t samplerate, char *buf )
{
	struct timecode tc;

	if ( posFormat == POS_FORMAT_TC ) {
		tc_set_by_unitValue( &tc, pos, (rational_t*)editRate, tcFormat );
		snprintf( buf, POS_FORMAT_BUFFER_LEN, "%s", tc.string );
		return buf;
	}
	else if ( posFormat == POS_FORMAT_SAMPLES ) {
		snprintf( buf, POS_FORMAT_BUFFER_LEN, "%"PRIi64, eu2sample( samplerate, editRate, pos ) );
		return buf;
	}
	else if ( posFormat == POS_FORMAT_HMS ) {

		uint64_t unitPerHour = aafRationalToFloat(*editRate) * 3600;
		uint64_t unitPerMin  = aafRationalToFloat(*editRate) * 60;
		uint64_t unitPerSec  = aafRationalToFloat(*editRate);
		uint64_t unitPerMS   = aafRationalToFloat(*editRate) / 1000;

		uint16_t h  = (unitPerHour) ?  (pos / unitPerHour) : 0;
		uint16_t m  = (unitPerMin)  ? ((pos % unitPerHour) / unitPerMin) : 0;
		uint16_t s  = (unitPerSec)  ? ((pos % unitPerHour  % unitPerMin) / unitPerSec) : 0;
		uint16_t ms = (unitPerMS)   ? ((pos % unitPerHour  % unitPerMin  % unitPerSec) / unitPerMS) : 0;

		snprintf( buf, POS_FORMAT_BUFFER_LEN, "%02u:%02u:%02u.%03u", h, m, s, ms );
		return buf;
	}

	return "wtf ?";
}



static void showHelp( void )
{
	printf(
		"\n"
		" CFB Analysis :\n"
		"\n"
		"   --cfb-header                   Displays the CFB Header.\n"
		"   --cfb-fat                      Displays the CFB FAT.\n"
		"   --cfb-minifat                  Displays the CFB MiniFAT.\n"
		"   --cfb-difat                    Displays the CFB DiFAT.\n"
		"   --cfb-nodes                    Displays the CFB node Tree.\n"
		"\n"
		"   --get-node             <path>  Retrieves and displays the node located at the given <path>.\n"
		"\n"
		"\n"
		" AAF Analysis :\n"
		"\n"
		"   --aaf-summary                  Displays AAF informations from both header and identification objects.\n"
		"   --aaf-essences                 Lists AAF essences.\n"
		"   --aaf-clips                    Lists AAF clips.\n"
		"   --aaf-classes                  Lists AAF Classes. The ones from MetaDictionary are shown in yellow.\n"
		"   --aaf-meta                     Lists Classes and Properties from the MetaDictionary.\n"
		"   --aaf-properties               Displays all Properties.\n"
		"\n"
		"   --trace                        Dumps trace of AAF parsing.\n"
		"   --trace-meta                   Prints MetaProperties in the trace.\n"
		"   --trace-class    <AAFClassID>  Prints properties of a specific AAFClass in the trace.\n"
		"\n"
		"\n"
		" Options :\n"
		"\n"
		"   --media-location       <path>  Where to find external essence files.\n"
		"\n"
		"   --pos-format <tc|hms|samples>  Position and length display format.\n"
		"   --show-automation              Shows track and clip automation values.\n"
		"\n"
		"   --verbose               <num>  0=quiet 1=error 2=warning 3=debug.\n"
		"\n\n"
	);
}



int main( int argc, char *argv[] )
{
	setlocale( LC_ALL, "" );

	int rc = 0;
	AAF_Data *aafd = NULL;
	AAF_Iface *aafi = NULL;

	int cfb_header         = 0;
	int cfb_fat            = 0;
	int cfb_minifat        = 0;
	int cfb_difat          = 0;
	int cfb_nodes          = 0;

	char *get_node_str     = NULL;

	int aaf_summary        = 0;
	int aaf_essences       = 0;
	int aaf_clips          = 0;
	int aaf_classes        = 0;
	int aaf_meta           = 0;
	int aaf_properties     = 0;

	char *media_location   = NULL;

	enum pos_format posFormat = POS_FORMAT_TC;
	int show_automation    = 0;

	enum verbosityLevel_e verb = VERB_DEBUG;
	int trace = 0;
	int trace_meta = 0;

	char *trace_class = NULL;

	int cmd = 0;


	printf( "\nAAFInfo\nlibAAF %s\n\n", LIBAAF_VERSION );


	static struct option long_options[] = {

		{ "help",			       no_argument,		     0,  	'h' },

		{ "cfb-header",      no_argument,        0,  0x81 },
		{ "cfb-fat",         no_argument,        0,  0x82 },
		{ "cfb-minifat",     no_argument,        0,  0x83 },
		{ "cfb-difat",       no_argument,        0,  0x84 },
		{ "cfb-nodes",       no_argument,        0,  0x85 },

		{ "get-node",        required_argument,	 0,	 0x86 },

		{ "aaf-summary",     no_argument,        0,  0x87 },
		{ "aaf-essences",    no_argument,        0,  0x88 },
		{ "aaf-clips",       no_argument,        0,  0x89 },
		{ "aaf-classes",     no_argument,        0,  0x8a },
		{ "aaf-meta",        no_argument,        0,  0x8b },
		{ "aaf-properties",  no_argument,        0,  0x8c },

		{ "trace",           no_argument,	       0,	 0x91 },
		{ "trace-meta",      no_argument,	       0,	 0x92 },
		{ "trace-class",     required_argument,  0,  0x93 },

		{ "media-location",  required_argument,	 0,	 0x8d },
		{ "pos-format",      required_argument,	 0,	 0x8e },

		{ "show-automation", no_argument,	       0,	 0x8f },

		{ "verbose",         required_argument,	 0,	 0x90 },

		{ 0,                 0,                  0,  0x00 }
	};


	int c = 0;

	while ( 1 )
	{
		int option_index = 0;

		c = getopt_long ( argc, argv, "h", long_options, &option_index );

		if ( c == -1 )
			break;

		switch ( c )
		{
			case 0x81:  cfb_header     = 1;         cmd++;       break;
			case 0x82:  cfb_fat        = 1;         cmd++;       break;
			case 0x83:  cfb_minifat    = 1;         cmd++;       break;
			case 0x84:  cfb_difat      = 1;         cmd++;       break;
			case 0x85:  cfb_nodes      = 1;         cmd++;       break;

			case 0x86:	get_node_str   = optarg;    cmd++;       break;

			case 0x87:  aaf_summary    = 1;         cmd++;       break;
			case 0x88:  aaf_essences   = 1;         cmd++;       break;
			case 0x89:  aaf_clips      = 1;         cmd++;       break;
			case 0x8a:  aaf_classes    = 1;         cmd++;       break;
			case 0x8b:  aaf_meta       = 1;         cmd++;       break;
			case 0x8c:  aaf_properties = 1;         cmd++;       break;

			case 0x8d:  media_location = optarg;                 break;

			case 0x8e:

				if ( strcmp( optarg, "tc" ) == 0 )
					posFormat = POS_FORMAT_TC;
				else if ( strcmp( optarg, "samples" ) == 0 )
					posFormat = POS_FORMAT_SAMPLES;
				else if ( strcmp( optarg, "hms" ) == 0 )
					posFormat = POS_FORMAT_HMS;
				else {
					fprintf( stderr,
						"AAFInfo: wrong --pos-format value (must be \"tc\" or \"samples\")\n"
						"Try 'AAFInfo --help' for more informations.\n"
					);
					goto err;
				}

				break;

			case 0x8f:  show_automation = 1;                     break;

			case 0x90:  verb  = atoi(optarg);                    break;
			case 0x91:  trace = 1;                  cmd++;       break;
			case 0x92:  trace_meta = 1;                          break;
			case 0x93:  trace_class = optarg;                    break;

			case 'h':	showHelp();        												 goto end;

			default:                                             break;
		}
	}


	if ( optind == argc ) {
		fprintf( stderr,
			"AAFInfo: missing file operand\n"
			"Try 'AAFInfo --help' for more informations.\n"
		);

		goto err;
	}


	if ( cmd == 0 ) {
		fprintf( stderr,
			"Usage: AAFInfo [CMD] [FILE]\n"
			"Try 'AAFInfo --help' for more informations.\n"
		);

		goto err;
	}


	if ( verb < VERB_QUIET || verb >= MAX_VERB ) {
		fprintf( stderr,
			"AAFInfo: Wrong --verbosity level\n"
			"Try 'AAFInfo --help' for more informations.\n"
		);

		goto err;
	}




	// aafd = aaf_alloc( NULL );

	aafi = aafi_alloc( NULL );

	if ( !aafi ) {
		fprintf( stderr, "Failed to init AAF_Iface context.\n" );
		goto err;
	}

	aafd = aafi->aafd;

	aafi_set_debug( aafi, verb, stdout, NULL, NULL );

	aafi_enable_windows_VT100_output();

	aafi->ctx.options.verb = verb;
	aafi->ctx.options.trace = trace;
	aafi->ctx.options.trace_meta = trace_meta;
	aafi->ctx.options.protools = PROTOOLS_ALL;
	aafi->ctx.options.resolve = RESOLVE_ALL;

	if ( media_location ) {
		aafi_set_media_location( aafi, media_location );
	}

	if ( trace_class ) {
		aafi_set_trace_class( aafi, trace_class );
	}


	if ( aafi_load_file( aafi, argv[argc-1] ) ) {
		fprintf( stderr, "Failed to open %s\n", argv[argc-1] );
		goto err;
	}


	printf( "\n" );





	if ( get_node_str != NULL ) {

		wchar_t wget_node_str[1024];

		swprintf( wget_node_str, 1024, L"%" WPRIws, get_node_str );

		cfbNode *node = cfb_getNodeByPath( aafd->cfbd, wget_node_str, 0 );

		if ( node == NULL ) {
			printf( "Could not find node at \"%s\"\n", get_node_str );
		}
		else {
			cfb_dump_node( aafd->cfbd, node, 0 );

			wchar_t name[CFB_NODE_NAME_SZ];

			cfb_w16towchar( name, node->_ab, node->_cb );

			if ( wcsncmp( name, L"properties", 10 ) == 0 ) {
				aaf_dump_nodeStreamProperties( aafd, node );
			}
			else if ( node->_mse == STGTY_STREAM ) {
				cfb_dump_nodeStream( aafd->cfbd, node );
			}
		}

		printf( "\n\n" );
	}


	if ( cfb_header ) {
		cfb_dump_header( aafd->cfbd );
	}


	if ( cfb_fat ) {
		cfb_dump_FAT( aafd->cfbd );
	}


	if ( cfb_minifat ) {
		cfb_dump_MiniFAT( aafd->cfbd );
	}


	if ( cfb_difat ) {
		cfb_dump_DiFAT( aafd->cfbd );
	}


	if ( cfb_nodes ) {
		uint32_t i = 0;
		cfb_dump_nodePaths( aafd->cfbd, 0, NULL, &i, NULL );
	}





	enum TC_FORMAT tcFormat;

	if ( aafi->Timecode->fps == 30 && aafi->Timecode->drop ) {
		tcFormat = TC_29_97_DF;
	}
	else {
		tcFormat = tc_fps2format( (float)(aafi->Timecode->fps ), aafi->Timecode->drop );
	}


	if ( aaf_summary ) {

		aaf_dump_Header( aafd );
		aaf_dump_Identification( aafd );

		printf( " Composition Name     : %ls\n", aafi->compositionName );

		printf( "\n" );

		printf( " TC EditRrate         : %i/%i\n", aafi->Timecode->edit_rate->numerator, aafi->Timecode->edit_rate->denominator );
		printf( " TC FPS               : %u %s\n", aafi->Timecode->fps, (aafi->Timecode->drop) ? "DF" : "NDF" );
		printf( " TC Start (EU)        : %"PRIi64"\n", aafi->Timecode->start );
		printf( " TC End (EU)          : %"PRIi64"\n", aafi->Timecode->end );

		printf( " TC Start (samples)   : %"PRIi64"\n", eu2sample( aafi->Audio->samplerate, aafi->Timecode->edit_rate, aafi->Timecode->start ) );
		printf( " TC End (samples)     : %"PRIi64"\n", eu2sample( aafi->Audio->samplerate, aafi->Timecode->edit_rate, aafi->Timecode->end ) );

		struct timecode tc_start;
		struct timecode tc_end;

		tc_set_by_unitValue( &tc_start, aafi->Timecode->start, (rational_t*)aafi->Timecode->edit_rate, tcFormat );
		tc_set_by_unitValue( &tc_end, aafi->Timecode->end, (rational_t*)aafi->Timecode->edit_rate, tcFormat );

		printf( " TC Start             : %s (%u fps %s)\n",
			tc_start.string,
			aafi->Timecode->fps,
			(aafi->Timecode->drop) ? "DF" : "NDF"
		);

		printf( " TC End               : %s (%u fps %s)\n",
			tc_end.string,
			aafi->Timecode->fps,
			(aafi->Timecode->drop) ? "DF" : "NDF"
		);


		printf( "\n" );

		printf( " Sample Rate          : %"PRIi64"\n", aafi->Audio->samplerate );
		printf( " Sample Size          : %i\n", aafi->Audio->samplesize );


		if ( aafi->Comments != NULL )	{

			printf( "\n" );

			printf( " UserComments :\n" );

			aafiUserComment *Comment = aafi->Comments;
			int i = 0;

			while ( Comment != NULL ) {
				printf( "   [%i]  Name: \"%ls\"   Text: \"%ls\"\n", i++, Comment->name, Comment->text );
				Comment = Comment->next;
			}
		}

		printf("\n\n");
	}


	if ( aaf_classes ) {
		aaf_dump_Classes( aafd );
		printf("\n\n");
	}


	if ( aaf_meta ) {
		aaf_dump_MetaDictionary( aafd );
		printf("\n\n");
	}


	if ( aaf_properties ) {

		aafObject *Object = aafd->Objects;

		for ( Object = aafd->Objects; Object != NULL; Object = Object->nextObj )
		{
			printf( "\n\n\n" ANSI_COLOR_MAGENTA " Object" ANSI_COLOR_RESET " @ %ls\n", aaf_get_ObjectPath( Object ) );
			aaf_dump_ObjectProperties( aafd, Object );
		}

		printf("\n\n");
	}







	if ( aaf_essences ) {

		printf( "Media Essences :\n"
	          "================\n\n" );
		aafiAudioEssence *audioEssence = NULL;

		uint32_t i = 0;

		foreachEssence( audioEssence, aafi->Audio->Essences )
		{
			char posFormatBuf[POS_FORMAT_BUFFER_LEN];
			aafRational_t essenceSampleRate = { audioEssence->samplerate, 1 };

			printf( " %s%u:  Type: %s  Length: %s   %02u Ch - %u Hz - %u bits   file : %ls  file_name : %ls%s%ls%s\n",
				( i < 10 ) ? " " : "", i,
				ESSENCE_TYPE_TO_STRING( audioEssence->type ),
				formatPosValue( audioEssence->length, &essenceSampleRate, posFormat, tcFormat, audioEssence->samplerate, posFormatBuf ),
				audioEssence->channels,
				audioEssence->samplerate,
				audioEssence->samplesize,
				( audioEssence->is_embedded ) ? L"EMBEDDED" : ( audioEssence->usable_file_path ) ? audioEssence->usable_file_path : audioEssence->original_file_path,
				audioEssence->file_name,
				( audioEssence->file_name && wcslen(audioEssence->file_name) == wcslen(audioEssence->unique_file_name) ) ? "" : "   (",
				( audioEssence->file_name && wcslen(audioEssence->file_name) == wcslen(audioEssence->unique_file_name) ) ? L"" : audioEssence->unique_file_name,
				( audioEssence->file_name && wcslen(audioEssence->file_name) == wcslen(audioEssence->unique_file_name) ) ? "" : ")"
			);

			// printf( "MOBID    %s\n", aaft_MobIDToText( audioEssence->sourceMobID ) );

			i++;
		}

		printf( "\n\n" );
	}




	if ( aaf_clips ) {

		printf( "Tracks & Clips :\n"
						"================\n\n" );


		aafiVideoTrack   *videoTrack = aafi->Video->Tracks;
		aafiTimelineItem *videoItem  = NULL;
		aafiVideoClip    *videoClip  = NULL;

		if ( videoTrack != NULL ) {

			printf( "VideoTrack %s(%u) - edit_rate %i/%i (%02.2f)  -  \"%ls\"\n",
					(videoTrack->number < 10) ? " " : "",
					videoTrack->number,
					videoTrack->edit_rate->numerator, videoTrack->edit_rate->denominator,
					aafRationalToFloat((*videoTrack->edit_rate)),
					(videoTrack->name != NULL) ? videoTrack->name : L""
			 );

			 if ( videoTrack->Items ) {

				 videoItem = videoTrack->Items;
				 videoClip = (aafiVideoClip*)videoItem->data;

				 // aafi_locate_external_essence_file( aafi, videoClip->Essence->original_file_path );

				 struct timecode tc_in;
				 struct timecode tc_out;
				 struct timecode tc_len;

				 memset(&tc_in,  0x00, sizeof(struct timecode));
				 memset(&tc_out, 0x00, sizeof(struct timecode));
				 memset(&tc_len, 0x00, sizeof(struct timecode));

				 aafPosition_t sessionStart = convertEditUnit( aafi->Timecode->start, *aafi->Timecode->edit_rate, *videoClip->track->edit_rate );

				 tc_set_by_unitValue( &tc_in,  (videoClip->pos + sessionStart),                  (rational_t*)videoClip->track->edit_rate, tcFormat );
				 tc_set_by_unitValue( &tc_len,  videoClip->len,                                  (rational_t*)videoClip->track->edit_rate, tcFormat );
				 tc_set_by_unitValue( &tc_out, (videoClip->pos + videoClip->len + sessionStart), (rational_t*)videoClip->track->edit_rate, tcFormat );

				 printf( " VideoClip "
						 " Start:%s  Len:%s  End:%s  "
						 " SourceFile: %ls   (%ls)\n",
					 // i, ( i < 10 ) ? " " : "",
					 // videoClip->track->number, ( videoClip->track->number < 10 ) ? " " : "",
					 tc_in.string,
					 tc_len.string,
					 tc_out.string,
					 // (audioClip->pos + audioClip->len + audioClip->track->Audio->tc->start),
					 (videoClip->Essence) ? videoClip->Essence->original_file_path : L"",
					 (videoClip->Essence) ? videoClip->Essence->file_name : L""
				 );

				 printf( "\n\n\n" );
			 }
		 }




		aafiAudioTrack   *audioTrack = NULL;
		aafiTimelineItem *audioItem  = NULL;
		aafiAudioClip    *audioClip  = NULL;

		uint32_t clipCount = 0;

		foreach_audioTrack( audioTrack, aafi ) {

			/*
			 *  Composition Timecode does not always share the same edit rate as tracks and clips.
			 *	Therefore, we need to do the conversion prior to any maths.
			 *  For exemple, if TC is 30000/1001 and audio clips are 48000/1, then TC->start has to be converted from FPS to samples.
			 */

			aafPosition_t sessionStart = convertEditUnit( aafi->compositionStart, aafi->compositionStart_editRate, *audioTrack->edit_rate );


			printf( "Track %s(%u) - %s - Gain: %s - Pan: %s - edit_rate: %i/%i (%02.2f)  -  \"%ls\"\n",
					(audioTrack->number < 10) ? " " : "",
					audioTrack->number,
					(audioTrack->format == AAFI_TRACK_FORMAT_MONO)   ? "MONO  " :
					(audioTrack->format == AAFI_TRACK_FORMAT_STEREO) ? "STEREO" :
					(audioTrack->format == AAFI_TRACK_FORMAT_5_1)    ? "5.1   " :
					(audioTrack->format == AAFI_TRACK_FORMAT_7_1)    ? "7.1   " : "Unknwn",
					gainToStr( audioTrack->gain ),
					panToStr( audioTrack->pan ),
					audioTrack->edit_rate->numerator, audioTrack->edit_rate->denominator,
					aafRationalToFloat((*audioTrack->edit_rate)),
					(audioTrack->name != NULL) ? audioTrack->name : L""
			 );

			 if ( show_automation && audioTrack->gain != NULL && audioTrack->gain->flags & AAFI_AUDIO_GAIN_VARIABLE ) {
				 printf( "TRACK GAIN AUTOMATION : \n" );
				 dumpVaryingValues( audioTrack->gain );
			 }

			 if ( show_automation && audioTrack->pan != NULL && audioTrack->pan->flags & AAFI_AUDIO_GAIN_VARIABLE ) {
				 printf( "TRACK PAN AUTOMATION : \n" );
				 dumpVaryingValues( audioTrack->pan );
			 }

			foreach_Item( audioItem, audioTrack ) {

				if ( audioItem->type == AAFI_TRANS ) {

					aafiTransition *Trans = (aafiTransition*)audioItem->data;

					if ( ! ( Trans->flags & AAFI_TRANS_XFADE ) )
						continue;

					printf( " xfade:   %s\n", INTERPOL_TO_STRING( Trans ) );
					continue;
				}
				else if ( audioItem->type == AAFI_AUDIO_CLIP ) {

					audioClip = (aafiAudioClip*)audioItem->data;

					// /*
					//  *  Composition Timecode does not always share the same edit rate as tracks and clips.
					//  *	Therefore, we need to do the conversion prior to any maths.
					//  *  For exemple, if TC is 30000/1001 and audio clips are 48000/1, then TC->start has to be converted from FPS to samples.
					//  */
					//
					// aafPosition_t sessionStart = convertEditUnit( audioClip->track->Audio->tc->start, aafi->Audio->tc->edit_rate, audioClip->track->edit_rate );


					aafiTransition *fadein  = aafi_get_fadein( audioItem );
					aafiTransition *fadeout = aafi_get_fadeout( audioItem );

					char posFormatBuf1[POS_FORMAT_BUFFER_LEN];
					char posFormatBuf2[POS_FORMAT_BUFFER_LEN];
					char posFormatBuf3[POS_FORMAT_BUFFER_LEN];
					char posFormatBuf4[POS_FORMAT_BUFFER_LEN];

					printf( " Clip:%u%s  Gain: %s %s  GainAuto: %s "
							" Start:%s  Len:%s  End:%s  "
							" Fadein: %s  Fadeout: %s  SrcOffset: %s  SourceFile: %ls   (%ls)\n",
						clipCount, ( clipCount < 10 ) ? " " : "",
						gainToStr( audioClip->gain ),
						(audioClip->mute) ? "(mute)" : "      ",
						(audioClip->automation) ? "(A) " : "none",
						formatPosValue( (audioClip->pos + sessionStart),                  audioClip->track->edit_rate, posFormat, tcFormat, aafi->Audio->samplerate, posFormatBuf1 ),
						formatPosValue( (audioClip->len),                                 audioClip->track->edit_rate, posFormat, tcFormat, aafi->Audio->samplerate, posFormatBuf2 ),
						formatPosValue( (audioClip->pos + sessionStart + audioClip->len), audioClip->track->edit_rate, posFormat, tcFormat, aafi->Audio->samplerate, posFormatBuf3 ),
						INTERPOL_TO_STRING( fadein ),
						INTERPOL_TO_STRING( fadeout ),
						// eu2sample( aafi->Audio->samplerate, audioClip->track->edit_rate, audioClip->essence_offset ),
						formatPosValue( audioClip->essence_offset, audioClip->track->edit_rate, posFormat, tcFormat, aafi->Audio->samplerate, posFormatBuf4 ),
						(audioClip->Essence) ? audioClip->Essence->unique_file_name : L"",
						(audioClip->Essence) ? audioClip->Essence->file_name : L""
					);

					if ( show_automation && audioClip->automation ) {
						printf( "CLIP GAIN AUTOMATION : \n" );
						dumpVaryingValues( audioClip->automation );
					}

					// if ( fadein )
					// 	free( fadein );
					//
					// if ( fadeout )
					// 	free( fadeout );

					clipCount++;
				}
			}

			printf( "\n\n" );
		}



		if ( aafi->Markers ) {
			printf( "Markers :\n"
							"=========\n\n" );
		}

		uint32_t markerCount = 0;
		aafiMarker *marker = NULL;

		foreachMarker( marker, aafi ) {

			/*
			 *  Composition Timecode does not always share the same edit rate as markers.
			 *	Therefore, we need to do the conversion prior to any maths.
			 *  For exemple, if TC is 30000/1001 and markers are 48000/1, then TC->start has to be converted from FPS to samples.
			 */

			aafPosition_t sessionStart = convertEditUnit( aafi->Timecode->start, *aafi->Timecode->edit_rate, *marker->edit_rate );

			char posFormatBuf1[POS_FORMAT_BUFFER_LEN];
			char posFormatBuf2[POS_FORMAT_BUFFER_LEN];

			printf(" Marker[%i]:  Start: %s  Length: %s  Color: #%02x%02x%02x  Label: \"%ls\"  Comment: \"%ls\"\n",
				markerCount++,
				formatPosValue( (marker->start + sessionStart), marker->edit_rate, posFormat, tcFormat, aafi->Audio->samplerate, posFormatBuf1 ),
				formatPosValue(  marker->length,                marker->edit_rate, posFormat, tcFormat, aafi->Audio->samplerate, posFormatBuf2 ),
				marker->RVBColor[0],
				marker->RVBColor[1],
				marker->RVBColor[2],
				marker->name,
				marker->comment );
		}
	}



	printf( "\n\n" );

	goto end;

err:
	rc = -1;

end:

	if ( aafi ) {
		aafi_release( &aafi );
	}
	else {
		if ( media_location ) {
			free( media_location );
		}
		if ( aafd ) {
			aaf_release( &aafd );
		}
	}


	return rc;
}

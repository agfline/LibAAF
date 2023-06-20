#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <getopt.h>
#include <locale.h>

#include <libaaf.h>


#include "./thirdparty/libTC.h"
#include "../src/common/utils.h" // ANSI colors



static char * gainToStr( aafiAudioGain *gain )
{
	static char str[32];

	memset( str, 0x00, 32 );

	if ( gain == NULL )
	{
		snprintf( str, 32, "      n/a   " );
	}
	else
	if ( gain->flags & AAFI_AUDIO_GAIN_CONSTANT ||
		 ( gain->flags & AAFI_AUDIO_GAIN_VARIABLE && gain->pts_cnt == 2 && gain->value[0].numerator == gain->value[1].numerator ) )
	{
		/*
		 *	NOTE some implementations use VaryingValue to store a single gain value.
		 *	This is indicated by the "(A)".
		 *	TODO implementation have changed !
		 */

		// aafRational_t *value = &(gain->value[0]);

		snprintf( str, 32, "%s%+05.1lf dB",
				( gain->flags & AAFI_AUDIO_GAIN_VARIABLE ) ? "(A) " : "    ",
				  20 * log10( aafRationalToFloat( gain->value[0] ) ) );
	}
	else if ( gain->flags & AAFI_AUDIO_GAIN_VARIABLE )
	{
		snprintf( str, 32, " automation " );
	}

	return str;
}




static char * panToStr( aafiAudioPan *pan )
{
	static char str[32];

	memset( str, 0x00, 32 );

	if ( pan == NULL )
	{
		snprintf( str, 32, "           n/a  " );
	}
	else if ( pan->flags & AAFI_AUDIO_GAIN_CONSTANT ||
			( pan->flags & AAFI_AUDIO_GAIN_VARIABLE && pan->pts_cnt == 2 && pan->value[0].numerator == pan->value[1].numerator ) )
	{
		/*
		 *	NOTE some implementations use VaryingValue to store a single gain value.
		 *	This is indicated by the "(A)".
		 */

		float panval = aafRationalToFloat((*pan->value));

		snprintf( str, 32, "%s%0.1f %s",
				( pan->flags & AAFI_AUDIO_GAIN_VARIABLE ) ? "(A) " : "    ",
				  panval,
				( panval == 0.0 ) ? "(Left)  " :
				( panval == 0.5 ) ? "(Center)" :
				( panval == 1.0 ) ? "(Right) " : "" );
	}
	else if ( pan->flags & AAFI_AUDIO_GAIN_VARIABLE )
	{
		snprintf( str, 32, " automation " );
	}


	return str;
}


/*
 *	NOTE : since aafiAudioPan is an alias of aafiAudioGain, the
 *	function can be called with both structures.
 */

static void aafi_dump_VaryingValues( aafiAudioGain *Gain )
{

	int i = 0;

	for ( i = 0; i < Gain->pts_cnt; i++ ) {

	   // printf( "   PT:  _t: %i/%i   _v: %i/%i\n",
	   // 	audioTrack->gain->time[i].numerator,
	   // 	audioTrack->gain->time[i].denominator,
	   // 	audioTrack->gain->value[i].numerator,
	   // 	audioTrack->gain->value[i].denominator
	   // );

	   // aafRational_t *time  = &(Gain->time[i] );
	   // aafRational_t *value = &(Gain->value[i]);

	   printf( "   VaryingValue:  _time: %f   _value: %f\n",
		   aafRationalToFloat( Gain->time[i]  ),
		   aafRationalToFloat( Gain->value[i] )
	   );

	   // printf("   PT:  _t: %i/%i   Gain: %.01f dB\n",
	   // 	audioTrack->gain->time[i].numerator,
	   // 	audioTrack->gain->time[i].denominator,
	   // 	20 * log10( (( audioTrack->gain->value[0].denominator == 0 ) ? 0 : ((float)audioTrack->gain->value[0].numerator/audioTrack->gain->value[0].denominator)) ) );
	}
}




void showHelp()
{
	printf(
		"\n"
		" AAFInfo 0.1\n"
		"\n"
		" Compound File Binary Analysis :\n"
		" ===============================\n"
		"   --cfb-header           Displays the CFB Header.\n"
		"   --cfb-fat              Displays the CFB FAT.\n"
		"   --cfb-minifat          Displays the CFB MiniFAT.\n"
		"   --cfb-difat            Displays the CFB DiFAT.\n"
		"   --cfb-nodes            Displays the CFB node Tree.\n"
		"\n"
		"   --get-node    <path>   Retrieves and displays the node\n"
		"                          located at the given <path>.\n"
		"\n"
		" Advanced Authoring File Analysis :\n"
		" ==================================\n"
		"   --aaf-summary          Displays AAF informations from both\n"
		"                          header and identification objects.\n"
		"   --aaf-essences         Lists AAF essences.\n"
		"   --aaf-clips            Lists AAF clips.\n"
		"   --aaf-classes          Lists AAF Classes. The ones from\n"
		"                          MetaDictionary are shown in yellow.\n"
		"   --aaf-meta             Lists Classes and Properties from\n"
		"                          the MetaDictionary.\n"
		"   --aaf-properties       Displays all Properties.\n"
		"\n\n"
	);

	exit( 0 );
}



int main( int argc, char *argv[] )
{
	setlocale( LC_ALL, "" );

	int cfb_header     = 0;
	int cfb_fat        = 0;
	int cfb_minifat    = 0;
	int cfb_difat      = 0;
	int cfb_nodes      = 0;

	int aaf_summary    = 0;
	int aaf_essences   = 0;
	int aaf_clips      = 0;
	int aaf_classes    = 0;
	int aaf_meta       = 0;
	int aaf_properties = 0;

	char *get_node_str = NULL;


	int cmd = 0;


	static struct option long_options[] = {

		{ "help",			no_argument,		0,	'h'  },

		{ "cfb-header",     no_argument,        0,  0x81 },
		{ "cfb-fat",        no_argument,        0,  0x82 },
		{ "cfb-minifat",    no_argument,        0,  0x83 },
		{ "cfb-difat",      no_argument,        0,  0x84 },
		{ "cfb-nodes",      no_argument,        0,  0x85 },

		{ "aaf-summary",    no_argument,        0,  0x86 },
		{ "aaf-essences",   no_argument,        0,  0x88 },
		{ "aaf-clips",      no_argument,        0,  0x89 },
		{ "aaf-classes",    no_argument,        0,  0x8a },
		{ "aaf-meta",       no_argument,        0,  0x8b },
		{ "aaf-properties", no_argument,        0,  0x8c },

		{ "get-node",   	required_argument,	0,	0x99 },

		{ 0,                0,                  0,  0x00 }
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

			case 0x86:  aaf_summary    = 1;         cmd++;       break;
			case 0x88:  aaf_essences   = 1;         cmd++;       break;
			case 0x89:  aaf_clips      = 1;         cmd++;       break;
			case 0x8a:  aaf_classes    = 1;         cmd++;       break;
			case 0x8b:  aaf_meta       = 1;         cmd++;       break;
			case 0x8c:  aaf_properties = 1;         cmd++;       break;

			case 0x99:	get_node_str   = optarg;    cmd++;       break;


			case 'h':	showHelp();                 cmd++;       break;

			default:                                cmd++;       break;
		}
	}



	if ( optind == argc ) {
		fprintf( stderr,
			"AAFInfo: missing file operand\n"
			"Try 'AAFInfo --help' for more informations.\n"
		);

		return 1;
	}



	if ( cmd == 0 ) {
		fprintf( stderr,
			"Usage: AAFInfo [CMD] [FILE]\n"
			"Try 'AAFInfo --help' for more informations.\n"
		);

		return 1;
	}



	printf( "\n\n" );


	AAF_Data *aafd = aaf_alloc();

	AAF_Iface *aafi = aafi_alloc( aafd );
	aafi->ctx.options.verb = VERB_DEBUG;
	aafi->ctx.options.trace = 1;

	if ( aafi_load_file( aafi, argv[argc-1] ) ) {
		aafi_release( &aafi );
		return 1;
	}


	printf( "\n\n" );





	if ( get_node_str != NULL ) {

		wchar_t wget_node_str[1024];

		swprintf( wget_node_str, 1024, L"%s", get_node_str );

		cfbNode *node = cfb_getNodeByPath( aafd->cfbd, wget_node_str, 0 );

		if ( node == NULL ) {
			printf( "Could not find node at \"%s\"\n", get_node_str );
		}
		else {
			cfb_dump_node( aafd->cfbd, node, 0 );

			char name[CFB_NODE_NAME_SZ];

			utf16toa( name, CFB_NODE_NAME_SZ, node->_ab, node->_cb );

			if ( strncmp( name, "properties", 10 ) == 0 ) {
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






	if ( aaf_summary ) {
		aaf_dump_Header( aafd );
		aaf_dump_Identification( aafd );
	}


	if ( aaf_classes ) {
		aaf_dump_Classes( aafd );
	}


	if ( aaf_meta ) {
		aaf_dump_MetaDictionary( aafd );
	}


	if ( aaf_properties ) {

		aafObject *Object = aafd->Objects;

		for ( Object = aafd->Objects; Object != NULL; Object = Object->nextObj )
		{
			printf( "\n\n\n" ANSI_COLOR_MAGENTA " Object" ANSI_COLOR_RESET " @ %ls\n", aaf_get_ObjectPath( Object ) );
			aaf_dump_ObjectProperties( aafd, Object );
		}
	}







	if ( aaf_essences ) {

		aafiAudioEssence *audioEssence = NULL;

		uint32_t i = 0;

		foreachEssence( audioEssence, aafi->Audio->Essences )
		{

			printf( " %s%u:  Type: %s  Duration: %u h  %02u mn  %02u s  %03u ms   %u Ch - %u Hz - %u bit  file : %ls  file_name : %ls   (%ls)\n",
				( i < 10 ) ? " " : "", i,
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

			// printf( "MOBID    %s\n", MobIDToText( audioEssence->sourceMobID ) );

			i++;
		}

		printf( "\n\n" );
	}




	if ( aaf_clips ) {

    printf( "EditRrate  : %i/%i\n", aafi->Audio->tc->edit_rate->numerator, aafi->Audio->tc->edit_rate->denominator );
    printf( "Start (EU) : %"PRIi64"\n", aafi->Audio->tc->start );
    printf( "End (EU)   : %"PRIi64"\n", aafi->Audio->tc->end );

    printf( "session start : %"PRIi64"\n", eu2sample( 48000, aafi->Audio->tc->edit_rate, aafi->Audio->tc->start ) );
    printf( "session end   : %"PRIi64"\n\n", eu2sample( 48000, aafi->Audio->tc->edit_rate, aafi->Audio->tc->end ) );

		printf( "Composition Name     : %ls\n", aafi->compositionName );

		enum TC_FORMAT format = tc_fps2format( (float)(aafi->Audio->tc->fps ), aafi->Audio->tc->drop );

		struct timecode tc_comp;
		tc_set_by_unitValue( &tc_comp, aafi->Audio->tc->start, (rational_t*)aafi->Audio->tc->edit_rate, format );

		printf("Composition TC Start : %s (%u fps %s)\n",
			tc_comp.string,
			aafi->Audio->tc->fps,
			(aafi->Audio->tc->drop) ? "DF" : "NDF"
		);

		printf( "======================\n\n" );

		if ( aafi->Comments != NULL )	{

			printf( "UserComments :\n" );
			printf( "==============\n" );

			aafiUserComment *Comment = aafi->Comments;

			while ( Comment != NULL ) {
				printf( "   * %ls : %ls\n", Comment->name, Comment->text );
				Comment = Comment->next;
			}

			printf("\n\n");
		}


		// locate_external_essence_file( aafi, L"file://10.87.230.71/mixage/DR2/Avid MediaFiles/MXF/1/3572607_RUGBY_F2_S65CFA3D0V.mxf" );
		// locate_external_essence_file( aafi, L"file:////C:/Users/mix_limo/Desktop/TEST2977052  -  OFF PODIUM ETAPE 2.aaf" );
		// locate_external_essence_file( aafi, L"file://?/E:/Adrien/ADPAAF/Sequence A Rendu.mxf" );
		// locate_external_essence_file( aafi, L"file://localhost/D:/horlaprod/Music/Logic/fonk_2/Audio Files_1/fonk_2_3#04.wav" );
		// locate_external_essence_file( aafi, L"file://localhost/Users/horlaprod/Music/Logic/fonk_2/Audio Files_1/fonk_2_3#04.wav" );
		// locate_external_essence_file( aafi, L"file:///_system/Users/horlaprod/pt2MCCzmhsFRHQgdgsTMQX.mxf" );



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
				 videoClip = (aafiVideoClip*)&(videoItem->data);

				 // locate_external_essence_file( aafi, videoClip->Essence->original_file );

				 struct timecode tc_in;
				 struct timecode tc_out;
				 struct timecode tc_len;

				 memset(&tc_in,  0x00, sizeof(struct timecode));
				 memset(&tc_out, 0x00, sizeof(struct timecode));
				 memset(&tc_len, 0x00, sizeof(struct timecode));

				 tc_set_by_unitValue( &tc_in,  (videoClip->pos + videoClip->track->Video->tc->start),                  (rational_t*)videoClip->track->edit_rate, format );
				 tc_set_by_unitValue( &tc_out, (videoClip->pos + videoClip->len + videoClip->track->Video->tc->start), (rational_t*)videoClip->track->edit_rate, format );
				 tc_set_by_unitValue( &tc_len,  videoClip->len,                                                        (rational_t*)videoClip->track->edit_rate, format );

				 printf( " VideoClip "
						 " Start:%s  Len:%s  End:%s  "
						 " SourceFile: %ls   (%ls)\n",
					 // i, ( i < 10 ) ? " " : "",
					 // videoClip->track->number, ( videoClip->track->number < 10 ) ? " " : "",
					 tc_in.string,
					 tc_len.string,
					 tc_out.string,
					 // (audioClip->pos + audioClip->len + audioClip->track->Audio->tc->start),
					 (videoClip->Essence) ? videoClip->Essence->original_file : L"",
					 (videoClip->Essence) ? videoClip->Essence->file_name : L""
				 );

				 printf( "\n\n\n" );
			 }
		 }




		aafiAudioTrack   *audioTrack = NULL;
		aafiTimelineItem *audioItem  = NULL;
		aafiAudioClip    *audioClip  = NULL;

		uint32_t i = 0;

		foreach_audioTrack( audioTrack, aafi ) {

			printf( "Track %s(%u) - %s - Gain  %s - Pan %s - edit_rate %i/%i (%02.2f)  -  \"%ls\"\n",
					(audioTrack->number < 10) ? " " : "",
					audioTrack->number,
					(audioTrack->format == AAFI_TRACK_FORMAT_MONO)   ? "MONO"   :
					(audioTrack->format == AAFI_TRACK_FORMAT_STEREO) ? "STEREO" :
					(audioTrack->format == AAFI_TRACK_FORMAT_5_1)    ? "5.1"    :
					(audioTrack->format == AAFI_TRACK_FORMAT_7_1)    ? "7.1"    : "Unknown",
					gainToStr( audioTrack->gain ),
					panToStr( audioTrack->pan ),
					audioTrack->edit_rate->numerator, audioTrack->edit_rate->denominator,
					aafRationalToFloat((*audioTrack->edit_rate)),
					(audioTrack->name != NULL) ? audioTrack->name : L""
			 );

			 if ( audioTrack->gain != NULL && audioTrack->gain->flags & AAFI_AUDIO_GAIN_VARIABLE ) {
				 printf( "TRACK GAIN AUTOMATION : \n" );
				 aafi_dump_VaryingValues( audioTrack->gain );
			 }

			 if ( audioTrack->pan != NULL && audioTrack->pan->flags & AAFI_AUDIO_GAIN_VARIABLE ) {
				 printf( "TRACK PAN AUTOMATION : \n" );
				 aafi_dump_VaryingValues( audioTrack->pan );
			 }

			foreach_Item( audioItem, audioTrack ) {

				if ( audioItem->type == AAFI_TRANS ) {

					aafiTransition *Trans = (aafiTransition*)&audioItem->data;

					if ( ! ( Trans->flags & AAFI_TRANS_XFADE ) )
						continue;


					printf( " xfade:   %s\n",
						(Trans->flags & AAFI_INTERPOL_NONE)     ? "CURV_NON" :
						(Trans->flags & AAFI_INTERPOL_LINEAR)   ? "CURV_LIN" :
						(Trans->flags & AAFI_INTERPOL_LOG)      ? "CURV_LOG" :
						(Trans->flags & AAFI_INTERPOL_CONSTANT) ? "CURV_CST" :
						(Trans->flags & AAFI_INTERPOL_POWER)    ? "CURV_PWR" :
						(Trans->flags & AAFI_INTERPOL_BSPLINE)  ? "CURV_BSP" :
						""
					);
	/*
					printf( "\n" );

	//					aafiTransition *Trans = (aafiTransition*)&audioItem->data;

					printf( " Fade     : %s\n",
						(Trans->flags & AAFI_TRANS_FADE_IN)  ? "AAFI_TRANS_FADE_IN"  :
						(Trans->flags & AAFI_TRANS_FADE_OUT) ? "AAFI_TRANS_FADE_OUT" :
						(Trans->flags & AAFI_TRANS_XFADE)    ? "AAFI_TRANS_XFADE"    :
						""
					);

					printf( " Interpol : %s\n",
						(Trans->flags & AAFI_INTERPOL_NONE)     ? "AAFI_INTERPOL_NONE"     :
						(Trans->flags & AAFI_INTERPOL_LINEAR)   ? "AAFI_INTERPOL_LINEAR"   :
						(Trans->flags & AAFI_INTERPOL_LOG)      ? "AAFI_INTERPOL_LOG"      :
						(Trans->flags & AAFI_INTERPOL_CONSTANT) ? "AAFI_INTERPOL_CONSTANT" :
						(Trans->flags & AAFI_INTERPOL_POWER)    ? "AAFI_INTERPOL_POWER"    :
						(Trans->flags & AAFI_INTERPOL_BSPLINE)  ? "AAFI_INTERPOL_BSPLINE"  :
						""
					);


					int  i = 0;
					for( i = 0; i < Trans->pts_cnt; i++ )
					{
	//						printf( "   PT:  _t: %i/%i   _v: %i/%i\n",
	//							Trans->time[i]->numerator,
	//							Trans->time[i]->denominator,
	//							Trans->value[i]->numerator,
	//							Trans->value[i]->denominator
	//						);

						printf( "   PT:  _t: %.0f   _v: %.0f\n",
							aafRationalToFloat( Trans->time[i] ),
							aafRationalToFloat( Trans->value[i] )
						);
					}

					printf( "\n" );
	*/

					continue;
				}

				audioClip = (aafiAudioClip*)&audioItem->data;

				aafiTransition *fadein  = get_fadein( audioItem );
				aafiTransition *fadeout = get_fadeout( audioItem );

				struct timecode tc_in;
				struct timecode tc_out;
				struct timecode tc_len;

				// enum TC_FORMAT format = tc_fps2format( (float)(aafi->Audio->tc->fps ), aafi->Audio->tc->drop );

				// printf( "EditRate : %f\n", aafRationalToFloat(audioClip->track->edit_rate) );
				// printf( "EditRate : %f\n", aafRationalToFloat(audioClip->track->edit_rate) );

				// printf( "Format : %s\n", TC_FORMAT_STR[format] );

				tc_set_by_unitValue( &tc_in,  (audioClip->pos + audioClip->track->Audio->tc->start),                  (rational_t*)audioClip->track->edit_rate, format );
				tc_set_by_unitValue( &tc_out, (audioClip->pos + audioClip->len + audioClip->track->Audio->tc->start), (rational_t*)audioClip->track->edit_rate, format );
				tc_set_by_unitValue( &tc_len,  audioClip->len,                                                        (rational_t*)audioClip->track->edit_rate, format );

				printf( " Clip:%u%s  Track:%u  Gain: %s %s"
						" Start:%s  Len:%s  End:%s  "
						" Fadein: %s  Fadeout: %s  SourceFile: %ls   (%ls)\n",
					i, ( i < 10 ) ? " " : "",
					audioClip->track->number,
					gainToStr( audioClip->gain ),
					(audioClip->mute) ? "(mute)" : "      ",
					tc_in.string,
					tc_len.string,
					tc_out.string,
					( fadein != NULL ) ?
						(fadein->flags & AAFI_INTERPOL_NONE)     ? "CURV_NON" :
						(fadein->flags & AAFI_INTERPOL_LINEAR)   ? "CURV_LIN" :
						(fadein->flags & AAFI_INTERPOL_LOG)      ? "CURV_LOG" :
						(fadein->flags & AAFI_INTERPOL_CONSTANT) ? "CURV_CST" :
						(fadein->flags & AAFI_INTERPOL_POWER)    ? "CURV_PWR" :
						(fadein->flags & AAFI_INTERPOL_BSPLINE)  ? "CURV_BSP" :
						"" :
					"none    ",
					( fadeout != NULL ) ?
						(fadeout->flags & AAFI_INTERPOL_NONE)     ? "CURV_NON" :
						(fadeout->flags & AAFI_INTERPOL_LINEAR)   ? "CURV_LIN" :
						(fadeout->flags & AAFI_INTERPOL_LOG)      ? "CURV_LOG" :
						(fadeout->flags & AAFI_INTERPOL_CONSTANT) ? "CURV_CST" :
						(fadeout->flags & AAFI_INTERPOL_POWER)    ? "CURV_PWR" :
						(fadeout->flags & AAFI_INTERPOL_BSPLINE)  ? "CURV_BSP" :
						"" :
					"none    ",
					(audioClip->Essence) ? audioClip->Essence->unique_file_name : L"",
					(audioClip->Essence) ? audioClip->Essence->file_name : L""
				);


				i++;
			}

			printf( "\n\n" );
		}

		printf( "\n\n" );
	}






	if ( aafi != NULL ) {
		aafi_release( &aafi );
	}
	else if ( aafd != NULL ) {
		aaf_release( &aafd );
	}

	return 0;
}

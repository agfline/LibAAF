#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include <getopt.h>

#include "../LibAAF/libAAF.h"
#include "./thirdparty/libTC.h"


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[92m"
#define ANSI_COLOR_YELLOW  "\x1b[93m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"




char * gainToStr( char *str, aafiAudioClip *aClip )
{
	memset( str, 0x00, 16 );

	if ( aClip->gain == NULL )
		snprintf( str, 16, " n/a    " );

	else if ( aClip->gain->flags & AAFI_AUDIO_GAIN_CONSTANT )
		snprintf( str, 16, "%+05.1lf dB",
				 20 * log10( (( aClip->gain->value[0].denominator == 0 ) ? 0 : ((float)aClip->gain->value[0].numerator/aClip->gain->value[0].denominator)) ) );
			  // 20 * log10( rationalToFloat( &(aClip->gain->value[0]) ) ) );

	else if ( aClip->gain->flags & AAFI_AUDIO_GAIN_VARIABLE )
		snprintf( str, 16, " automation " );

	return str;
}





void printPropertyStream( AAF_Data *aafd, cfbNode *node )
{

	aafPropertyIndexHeader_t *Header = NULL;
	aafPropertyIndexEntry_t  *Prop   = NULL;
	aafByte_t                *value  = NULL;

	cfb_getStream( aafd->cfbd, node, (unsigned char**)&Header, NULL );

	uint32_t i = 0;

	printf(
		" ## Property_Header__________________________________________________\n"
		" _byteOrder     : 0x%02x\n"
		" _formatVersion : 0x%02x\n"
		" _entryCount    : %u\n",
		Header->_byteOrder,
		Header->_formatVersion,
		Header->_entryCount
	);

	printf( "\n\n\n\n" );


	/*
	 *	Since the following for() loop is not intended to be used by the user, it has been defined
	 *	as a local macro in AAFCore.c.
	 */

//	foreachPropertyEntry( Header, Prop, value, i )
	for ( Prop = (aafPropertyIndexEntry_t*)(((char*)Header) + sizeof(aafPropertyIndexHeader_t)),    \
	      value = ((unsigned char*)Prop) + (Header->_entryCount * sizeof(aafPropertyIndexEntry_t)), \
	      i = 0;                                                                                    \
	      i < Header->_entryCount;                                                                  \
	      value += Prop->_length,                                                                   \
	      Prop++,                                                                                   \
	      i++ )
	{

		printf(
			" #%u Property_Entry___________________________________________________\n"
			" _pid        : 0x%04x (%s)\n"
			" _storedForm : %s\n"
			" _length     : %u bytes\n",
			i,
			Prop->_pid, PIDToText( aafd, Prop->_pid ),
			StoredFormToText( Prop->_storedForm ),
			Prop->_length
		);

		cfb_printStream( value, Prop->_length );

		printf( "\n\n" );
	}

	free( Header );

}


void printRawStream( AAF_Data *aafd, cfbNode *node )
{

	unsigned char *stream = NULL;
	uint64_t       stream_sz = 0;

	cfb_getStream( aafd->cfbd, node, &stream, &stream_sz );

	cfb_printStream( stream, stream_sz );

	free( stream );

}




void printIdentification( AAF_Data *aafd )
{
	aafObject *Identif = aafd->Identification;

	if ( Identif == NULL )
	{
		printf( "Missing IdentificationList.\n" );
	}
	else
	{
		char *Company = aaf_get_propertyValueText( Identif, PID_Identification_CompanyName );

		if ( Company != NULL )
		{
			printf( " CompanyName          : %s\n", Company );

			free( Company );
		}
		else
			printf( " CompanyName          : n/a\n" );




		char *ProdName = aaf_get_propertyValueText( Identif, PID_Identification_ProductName );

		if ( ProdName != NULL )
		{
			printf( " ProductName          : %s\n", ProdName );

			free( ProdName );
		}
		else
			printf( " ProductName          : n/a\n" );




		aafProductVersion_t *ProdVer = aaf_get_propertyValue( Identif, PID_Identification_ProductVersion );
		if ( ProdVer != NULL )
		{
			printf( " ProductVersion       : %u.%u.%u.%u (%u)\n",
				ProdVer->major,
				ProdVer->minor,
				ProdVer->tertiary,
				ProdVer->patchLevel,
				ProdVer->type
			);
		}
		else
			printf( " ProductVersion       : n/a\n" );




		char *ProdVerStr = aaf_get_propertyValueText( Identif, PID_Identification_ProductVersionString );

		if ( ProdVerStr != NULL )
		{
			printf( " ProductVersionString : %s\n", ProdVerStr );

			free( ProdVerStr );
		}
		else
			printf( " ProductVersionString : n/a\n" );




		aafUID_t *ProdID = aaf_get_propertyValue( Identif, PID_Identification_ProductID );

		if ( ProdID != NULL )
		{
			printf( " ProductID            : {%08x %04x %04x {%02x %02x %02x %02x %02x %02x %02x %02x}}\n",
				ProdID->Data1,
				ProdID->Data2,
				ProdID->Data3,
				ProdID->Data4[0],
				ProdID->Data4[1],
				ProdID->Data4[2],
				ProdID->Data4[3],
				ProdID->Data4[4],
				ProdID->Data4[5],
				ProdID->Data4[6],
				ProdID->Data4[7]
			);
		}
		else
			printf( " ProductID            : n/a\n" );




		aafTimeStamp_t *Date = aaf_get_propertyValue( Identif, PID_Identification_Date );

		if ( Date != NULL )
		{
			printf( " Date                 : %04u-%02u-%02u %02u:%02u:%02u.%02u\n",
				Date->date.year,
				Date->date.month,
				Date->date.day,
				Date->time.hour,
				Date->time.minute,
				Date->time.second,
				Date->time.fraction
			);
		}
		else
			printf( " Date                 : n/a\n" );




		aafProductVersion_t *ToolkitVer = aaf_get_propertyValue( Identif, PID_Identification_ToolkitVersion );

		if ( ToolkitVer != NULL )
		{
			printf( " ToolkitVersion       : %u.%u.%u.%u (%u)\n",
				ToolkitVer->major,
				ToolkitVer->minor,
				ToolkitVer->tertiary,
				ToolkitVer->patchLevel,
				ToolkitVer->type
			);
		}
		else
			printf( " ToolkitVersion       : n/a\n" );




		char *Platform = aaf_get_propertyValueText( Identif, PID_Identification_Platform );

		if ( Platform != NULL )
		{
			printf( " Platform             : %s\n", Platform );

			free( Platform );
		}
		else
			printf( " Platform             : n/a\n" );




		aafUID_t *Generation = aaf_get_propertyValue( Identif, PID_Identification_GenerationAUID );

		if ( Generation != NULL )
		{
			printf( " GenerationAUID       : {%08x %04x %04x {%02x %02x %02x %02x %02x %02x %02x %02x}}\n",
				Generation->Data1,
				Generation->Data2,
				Generation->Data3,
				Generation->Data4[0],
				Generation->Data4[1],
				Generation->Data4[2],
				Generation->Data4[3],
				Generation->Data4[4],
				Generation->Data4[5],
				Generation->Data4[6],
				Generation->Data4[7]
			);
		}
		else
			printf( " GenerationAUID       : n/a\n" );


	}

	printf( "\n\n" );
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

//	int cfb_summary    = 0;
	int cfb_header     = 0;
	int cfb_fat        = 0;
	int cfb_minifat    = 0;
	int cfb_difat      = 0;
	int cfb_nodes      = 0;

	int aaf_summary    = 0;
//	int aaf_origin     = 0;
	int aaf_essences   = 0;
	int aaf_clips      = 0;
	int aaf_classes    = 0;
	int aaf_meta       = 0;
	int aaf_properties = 0;

	char *get_node_str = NULL;

//	int cfb_get_node = 0;

	int cmd = 0;


	static struct option long_options[] = {

		{ "help",			no_argument,		0,	'h'  },

//		{ "cfb-summary",    no_argument,        0,  0x80 },
		{ "cfb-header",     no_argument,        0,  0x81 },
		{ "cfb-fat",        no_argument,        0,  0x82 },
		{ "cfb-minifat",    no_argument,        0,  0x83 },
		{ "cfb-difat",      no_argument,        0,  0x84 },
		{ "cfb-nodes",      no_argument,        0,  0x85 },

		{ "aaf-summary",    no_argument,        0,  0x86 },
//		{ "aaf-origin",     no_argument,        0,  0x87 },
		{ "aaf-essences",   no_argument,        0,  0x88 },
		{ "aaf-clips",      no_argument,        0,  0x89 },
		{ "aaf-classes",    no_argument,        0,  0x8a },
		{ "aaf-meta",       no_argument,        0,  0x8b },
		{ "aaf-properties", no_argument,        0,  0x8c },

		{ "get-node",   	required_argument,	0,	0x99 },
//		{ "get-node-id",	required_argument,	0,	0x87 },

		{ 0,                0,                  0,  0    }
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
//			case 0x80:  cfb_summary    = 1;         cmd++;       break;
			case 0x81:  cfb_header     = 1;         cmd++;       break;
			case 0x82:  cfb_fat        = 1;         cmd++;       break;
			case 0x83:  cfb_minifat    = 1;         cmd++;       break;
			case 0x84:  cfb_difat      = 1;         cmd++;       break;
			case 0x85:  cfb_nodes      = 1;         cmd++;       break;

			case 0x86:  aaf_summary    = 1;         cmd++;       break;
//			case 0x87:  aaf_origin     = 1;         cmd++;       break;
			case 0x88:  aaf_essences   = 1;         cmd++;       break;
			case 0x89:  aaf_clips      = 1;         cmd++;       break;
			case 0x8a:  aaf_classes    = 1;         cmd++;       break;
			case 0x8b:  aaf_meta       = 1;         cmd++;       break;
			case 0x8c:  aaf_properties = 1;         cmd++;       break;

			case 0x99:	get_node_str   = optarg;    cmd++;       break;

//			case 0x81:	get_node_id    = optarg;    cmd++;       break;

			case 'h':	showHelp();                 cmd++;       break;

			default:                                cmd++;       break;
		}
	}



	if ( optind == argc )
	{
		fprintf( stderr,
			"AAFInfo: missing file operand\n"
			"Try 'AAFInfo --help' for more informations.\n"
		);

		return 1;
	}



	if ( cmd == 0 )
	{
		fprintf( stderr,
			"Usage: AAFInfo [CMD] [FILE]\n"
			"Try 'AAFInfo --help' for more informations.\n"
		);

		return 1;
	}



	printf( "\n\n" );


	AAF_Data *aafd = aaf_alloc();

	if ( aaf_load_file( aafd, argv[argc-1] ) )
		return 1;


	AAF_Iface *aafi = NULL;

	if ( aaf_essences || aaf_clips )
	{
		aafi = aafi_alloc( aafd );

		aafi_retrieveData( aafi );
	}

	printf( "\n\n" );





	if ( get_node_str != NULL )
	{
		cfbNode *node = cfb_getNodeByPath( aafd->cfbd, get_node_str, 0 );

		if ( node == NULL )
			printf( "Could not find node at \"%s\"\n", get_node_str );
		else
		{
			cfb_printNode( node, NULL );

			char name[CFB_NODE_NAME_SZ];

			utf16toa( name, CFB_NODE_NAME_SZ, node->_ab, node->_cb );

			if ( strncmp( name, "properties", 10 ) == 0 )
				printPropertyStream( aafd, node );
			else if ( node->_mse == STGTY_STREAM )
				printRawStream( aafd, node );
		}

		printf( "\n\n" );
	}




	if ( cfb_header )
	{
		printf( "_CFB_Header____________________________________________________________________________________\n\n" );

		cfb_printHeader( aafd->cfbd );

		printf( "\n\n" );
	}



	if ( cfb_fat )
	{
		printf( "_CFB_FAT_______________________________________________________________________________________\n\n" );

		uint32_t i = 0;

		for ( i = 0; i < aafd->cfbd->fat_sz; i++ )
			printf( " SECT[%u] : 0x%08x %s\n",
				i,
				aafd->cfbd->fat[i],
				( aafd->cfbd->fat[i] == CFB_MAX_REG_SECT ) ? "(CFB_MAX_REG_SECT)" :
				( aafd->cfbd->fat[i] == CFB_DIFAT_SECT   ) ? "(CFB_DIFAT_SECT)"   :
				( aafd->cfbd->fat[i] == CFB_FAT_SECT     ) ? "(CFB_FAT_SECT)"     :
				( aafd->cfbd->fat[i] == CFB_END_OF_CHAIN ) ? "(CFB_END_OF_CHAIN)" :
				( aafd->cfbd->fat[i] == CFB_FREE_SECT    ) ? "(CFB_FREE_SECT)"    :
				""
			);

		printf( "\n" );

		printf( " End of FAT.\n\n" );

		printf( " Total FAT entries   : %u\n", aafd->cfbd->fat_sz );
		printf( " Count of FAT sector : %u\n", aafd->cfbd->hdr->_csectFat );

		printf( "\n\n" );
	}



	if ( cfb_minifat )
	{
		printf( "_CFB_MiniFAT___________________________________________________________________________________\n\n" );

		uint32_t i = 0;

		for ( i = 0; i < aafd->cfbd->miniFat_sz; i++ )
			printf( " SECT[%u] : 0x%08x %s\n",
				i,
				aafd->cfbd->miniFat[i],
				( aafd->cfbd->miniFat[i] == CFB_MAX_REG_SECT ) ? "(CFB_MAX_REG_SECT)" :
				( aafd->cfbd->miniFat[i] == CFB_DIFAT_SECT   ) ? "(CFB_DIFAT_SECT)"   :
				( aafd->cfbd->miniFat[i] == CFB_FAT_SECT     ) ? "(CFB_FAT_SECT)"     :
				( aafd->cfbd->miniFat[i] == CFB_END_OF_CHAIN ) ? "(CFB_END_OF_CHAIN)" :
				( aafd->cfbd->miniFat[i] == CFB_FREE_SECT    ) ? "(CFB_FREE_SECT)"    :
				""
			);

		printf( "\n" );

		printf( " End of MiniFAT.\n\n" );

		printf( " Total MiniFAT entries   : %u\n", aafd->cfbd->miniFat_sz );
		printf( " First MiniFAT sector ID : %u\n", aafd->cfbd->hdr->_sectMiniFatStart );
		printf( " Count of MiniFAT sector : %u\n", aafd->cfbd->hdr->_csectMiniFat );

		printf( "\n\n" );
	}



	if ( cfb_difat )
	{
		printf( "_CFB_DiFAT_____________________________________________________________________________________\n\n" );

		uint32_t i = 0;

		for ( i = 0; i < aafd->cfbd->DiFAT_sz; i++ )
			printf( " SECT[%u] : 0x%08x %s\n",
				i,
				aafd->cfbd->DiFAT[i],
				( aafd->cfbd->DiFAT[i] == CFB_MAX_REG_SECT ) ? "(CFB_MAX_REG_SECT)" :
				( aafd->cfbd->DiFAT[i] == CFB_DIFAT_SECT   ) ? "(CFB_DIFAT_SECT)"   :
				( aafd->cfbd->DiFAT[i] == CFB_FAT_SECT     ) ? "(CFB_FAT_SECT)"     :
				( aafd->cfbd->DiFAT[i] == CFB_END_OF_CHAIN ) ? "(CFB_END_OF_CHAIN)" :
				( aafd->cfbd->DiFAT[i] == CFB_FREE_SECT    ) ? "(CFB_FREE_SECT)"    :
				""
			);

		printf( "\n" );

		printf( " End of DiFAT.\n\n" );

		printf( " Total DiFAT entries   : %u\n", aafd->cfbd->DiFAT_sz );
		printf( " First DiFAT sector ID : %u\n", aafd->cfbd->hdr->_sectDifStart );
		printf( " Count of DiFAT sector : Header + %u\n", aafd->cfbd->hdr->_csectDif );

		printf( "\n\n" );
	}





	if ( aaf_summary )
	{

		uint16_t *byteOrder = (uint16_t*)aaf_get_propertyValue( aafd->Header, PID_Header_ByteOrder );

		if ( byteOrder != NULL )
			printf( " ByteOrder            : %s (0x%04x)\n",
				(*byteOrder == AAF_HEADER_BYTEORDER_LE) ? "Little-Endian" :
				(*byteOrder == AAF_HEADER_BYTEORDER_BE) ? "Big-Endian" :
				"unknwon",
				*byteOrder );
		else
			printf( " ByteOrder            : n/a\n" );






		aafTimeStamp_t *Date = aaf_get_propertyValue( aafd->Header, PID_Header_LastModified );

		if ( Date != NULL )
		{
			printf( " LastModified         : %04u-%02u-%02u %02u:%02u:%02u.%02u\n",
				Date->date.year,
				Date->date.month,
				Date->date.day,
				Date->time.hour,
				Date->time.minute,
				Date->time.second,
				Date->time.fraction
			);
		}
		else
			printf( " LastModified         : n/a\n" );





		aafVersionType_t *Vers = (aafVersionType_t*)aaf_get_propertyValue( aafd->Header, PID_Header_Version );

		if ( Vers != NULL )
			printf( " AAF ObjSpec Version  : %i.%i\n",
				Vers->major,
				Vers->minor );
		else
			printf( " AAF ObjSpec Version  : n/a\n" );



/*
		aafObject *Content = aaf_get_propertyValue( aafd->Header, PID_Header_Content );

		if ( Content != NULL )
			printf( " ContentStorage Obj   : %s\n",
				Content->Name );
		else
			printf( " ContentStorage Obj   : n/a\n" );
*/




		uint32_t *ObjModelVer = (uint32_t*)aaf_get_propertyValue( aafd->Header, PID_Header_ObjectModelVersion );

		if ( ObjModelVer != NULL )
			printf( " ObjectModel Version  : %u\n",
				*ObjModelVer );
		else
			printf( " ObjectModel Version  : n/a\n" );




		aafUID_t *Op = (aafUID_t*)aaf_get_propertyValue( aafd->Header, PID_Header_OperationalPattern );

		if ( Op != NULL )
			printf( " Operational Pattern  : %s\n",
				OPDefToText( Op ) );
		else
			printf( " Operational Pattern  : n/a\n" );



		printf( "\n\n" );

		printIdentification( aafd );
	}




	if ( cfb_nodes )
	{
		uint32_t i = 0;

		cfb_printEachNodePath( aafd->cfbd, 0, NULL, &i, NULL );

		printf( "\n\n" );

	}






	if ( aaf_essences )
	{

		aafiAudioEssence *audioEssence = NULL;

		uint32_t i = 0;

		foreachAudioEssence( audioEssence, aafi->Audio->Essences )
		{


			if ( audioEssence->type == AAFI_TYPE_PCM || audioEssence->type == AAFI_TYPE_WAVE || audioEssence->type == AAFI_TYPE_AIFC )
			{
				printf( " %s%u:  Type: %s  Duration: %u h  %02u mn  %02u s  %03u ms   %u Ch - %u Hz - %u bit  file : %s  file_name : %s\n",
					( i < 10 ) ? " " : "", i,
					( audioEssence->type == AAFI_TYPE_PCM  ) ? "PCM"  :
					( audioEssence->type == AAFI_TYPE_WAVE ) ? "WAVE" :
					( audioEssence->type == AAFI_TYPE_AIFC ) ? "AIFC" :
					( audioEssence->type == AAFI_TYPE_BWAV ) ? "BWAV" : "",
					(uint16_t)(audioEssence->length / audioEssence->samplerate / (audioEssence->samplesize / 8)) / 3600,
					(uint16_t)(audioEssence->length / audioEssence->samplerate / (audioEssence->samplesize / 8)) % 3600 / 60,
					(uint16_t)(audioEssence->length / audioEssence->samplerate / (audioEssence->samplesize / 8)) % 3600 % 60,
					(uint16_t)(audioEssence->length / (audioEssence->samplerate / 1000) / (audioEssence->samplesize / 8)) % 3600000 % 60000 % 1000,
					audioEssence->channels,
					audioEssence->samplerate,
					audioEssence->samplesize,
					( audioEssence->is_embedded ) ? "EMBEDDED" : audioEssence->original_file,
					audioEssence->unique_file_name
				);
			}
			else
			{
				// printf( "len : %lu\n", audioEssence->length );
				// printf( "nSamplesPerSec : %u\n", audioEssence->nSamplesPerSec );
				// printf( "wBitsPerSample : %u\n", audioEssence->wBitsPerSample );
				// printf( " Audio_%s:%u%s   Duration %02u:%02u:%02u\n",
				// 	( audioEssence->type == AAFI_TYPE_PCM  ) ? "PCM"  :
				// 	( audioEssence->type == AAFI_TYPE_WAVE ) ? "WAVE" :
				// 	( audioEssence->type == AAFI_TYPE_AIFC ) ? "AIFC" :
				// 	( audioEssence->type == AAFI_TYPE_BWAV ) ? "BWAV" : "",
				// 	i, ( i < 10 ) ? " " : "",
				// 	(uint16_t)(audioEssence->length / 48000) / 3600,
				// 	(uint16_t)(audioEssence->length / 48000) % 3600 / 60,
				// 	(uint16_t)(audioEssence->length / 48000) % 3600 % 60
				// );
			}

			i++;
		}

		printf( "\n\n" );
	}




	if ( aaf_clips )
	{
		printf( "Composition Name     : %s\n", aafi->compositionName );
		printf( "======================\n" );

		enum TC_FORMAT format = tc_fps2format( (float)(aafi->Audio->tc->fps ), aafi->Audio->tc->drop );

		struct timecode tc_comp;
		tc_set_by_unitValue( &tc_comp, aafi->Audio->tc->start, (rational_t*)aafi->Audio->tc->edit_rate, format );


		printf("Composition TC Start : %s (%u fps %s)\n",
			tc_comp.string,
			// eu2tc_h( aafi->Audio->tc->edit_rate, 90004*1920 ),
			// eu2tc_m( aafi->Audio->tc->edit_rate, 90004*1920 ),
			// eu2tc_s( aafi->Audio->tc->edit_rate, 90004*1920 ),
			// eu2tc_f( aafi->Audio->tc->edit_rate, aafi->Audio->tc, 90004*1920),
			aafi->Audio->tc->fps,
			(aafi->Audio->tc->drop) ? "DF" : "NDF"
		);
		printf( "======================\n\n" );

		aafiUserComment *Comment = aafi->Comments;

		if ( Comment != NULL )
		{
			printf( "UserComments :\n==============\n" );

			while ( Comment != NULL )
			{
				printf( "   * %s : %s\n", Comment->name, Comment->text );

				Comment = Comment->next;
			}

			printf("\n\n");
		}

		aafiAudioTrack   *audioTrack = NULL;
		aafiTimelineItem *audioItem  = NULL;
		aafiAudioClip    *audioClip  = NULL;

		uint32_t i = 0;

		foreach_audioTrack( audioTrack, aafi )
		{

			printf( "Track (%u) - %s - edit_rate %i/%i (%02.2f)  -  \"%s\"\n",
			        audioTrack->number,
					(audioTrack->format == AAFI_TRACK_FORMAT_MONO)   ? "MONO"   :
					(audioTrack->format == AAFI_TRACK_FORMAT_STEREO) ? "STEREO" :
					(audioTrack->format == AAFI_TRACK_FORMAT_5_1)    ? "5.1"    :
					(audioTrack->format == AAFI_TRACK_FORMAT_7_1)    ? "7.1"    : "Unknown",
			        audioTrack->edit_rate->numerator, audioTrack->edit_rate->denominator,
					rationalToFloat(audioTrack->edit_rate),
					(audioTrack->name != NULL) ? audioTrack->name : ""
			 );

			foreach_audioItem( audioItem, audioTrack )
			{

				if ( audioItem->type == AAFI_TRANS )
				{

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
							rationalToFloat( Trans->time[i] ),
							rationalToFloat( Trans->value[i] )
						);
					}

					printf( "\n" );
*/

					continue;
				}

				audioClip = (aafiAudioClip*)&audioItem->data;

				aafiTransition *fadein  = get_fadein( audioItem );
				aafiTransition *fadeout = get_fadeout( audioItem );

				char str[16];

				struct timecode tc_in;
				struct timecode tc_out;
				struct timecode tc_len;

				// enum TC_FORMAT format = tc_fps2format( (float)(aafi->Audio->tc->fps ), aafi->Audio->tc->drop );

				// printf( "EditRate : %f\n", rationalToFloat(audioClip->track->edit_rate) );
				// printf( "EditRate : %f\n", rationalToFloat(audioClip->track->edit_rate) );

				// printf( "Format : %s\n", TC_FORMAT_STR[format] );

				tc_set_by_unitValue( &tc_in,  (audioClip->pos + audioClip->track->Audio->tc->start),                  (rational_t*)audioClip->track->edit_rate, format );
				tc_set_by_unitValue( &tc_out, (audioClip->pos + audioClip->len + audioClip->track->Audio->tc->start), (rational_t*)audioClip->track->edit_rate, format );
				tc_set_by_unitValue( &tc_len,  audioClip->len,                                                        (rational_t*)audioClip->track->edit_rate, format );

				printf( " Clip:%u%s  Track:%u  Gain:%s "
						" Start:%s  Len:%s  End:%s  "
						" Fadein: %s  Fadeout: %s  SourceFile: %s\n",
					i, ( i < 10 ) ? " " : "",
					audioClip->track->number,
					gainToStr( str, audioClip ),
					tc_in.string,
					tc_len.string,
					tc_out.string,
					// tc_in.hours,  tc_in.minutes,  tc_in.seconds,  tc_in.frames,
					// tc_len.hours, tc_len.minutes, tc_len.seconds, tc_len.frames,
					// tc_out.hours, tc_out.minutes, tc_out.seconds, tc_out.frames,
					// eu2tc_h( audioClip->track->edit_rate, (audioClip->pos + audioClip->track->Audio->tc->start) ),
					// eu2tc_m( audioClip->track->edit_rate, (audioClip->pos + audioClip->track->Audio->tc->start) ),
					// eu2tc_s( audioClip->track->edit_rate, (audioClip->pos + audioClip->track->Audio->tc->start) ),
					// eu2tc_f( audioClip->track->edit_rate, audioClip->track->Audio->tc, (audioClip->pos + audioClip->track->Audio->tc->start) ),
					// eu2tc_h( audioClip->track->edit_rate, (audioClip->len) ),
					// eu2tc_m( audioClip->track->edit_rate, (audioClip->len) ),
					// eu2tc_s( audioClip->track->edit_rate, (audioClip->len) ),
					// eu2tc_f( audioClip->track->edit_rate, audioClip->track->Audio->tc, (audioClip->len) ),
					// eu2tc_h( audioClip->track->edit_rate, (audioClip->pos + audioClip->len + audioClip->track->Audio->tc->start) ),
					// eu2tc_m( audioClip->track->edit_rate, (audioClip->pos + audioClip->len + audioClip->track->Audio->tc->start) ),
					// eu2tc_s( audioClip->track->edit_rate, (audioClip->pos + audioClip->len + audioClip->track->Audio->tc->start) ),
					// eu2tc_f( audioClip->track->edit_rate, audioClip->track->Audio->tc, (audioClip->pos + audioClip->len + audioClip->track->Audio->tc->start) ),
					// (audioClip->pos + audioClip->len + audioClip->track->Audio->tc->start),
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
					(audioClip->Essence) ? audioClip->Essence->file_name : ""
				);


				i++;
			}

			printf( "\n\n" );
		}

		printf( "\n\n" );
	}




	if ( aaf_classes )
	{

		aafClass *ConcreteClass = NULL;
		aafClass *Class         = NULL;

		foreachClass( ConcreteClass, aafd->Classes )
		{

			foreachClassInheritance( Class, ConcreteClass )
			{
				// if ( Class->meta )
				// 	printf( ANSI_COLOR_YELLOW
				// 			"%s"
				// 			ANSI_COLOR_RESET,
				// 			Class->name );
				// else
					printf( "%s", ClassIDToText( aafd, Class->ID ) );

				if ( Class->Parent != NULL )
					printf( " > " );

			}

			printf( "\n" );

		}

		printf( "\n\n" );
	}




	if ( aaf_meta )
	{

		aafClass *Class = NULL;


		foreachClass( Class, aafd->Classes )
		{

			int print = 0;

			aafPropertyDef *PDef = NULL;

			foreachPropertyDefinition( PDef, Class->Properties )
			{
				if ( Class->meta )
				{
					printf( ANSI_COLOR_YELLOW "%s::%s (0x%04x)\n" ANSI_COLOR_RESET,
						Class->name,
						PDef->name,
						PDef->pid );

					print++;
				}
				else if ( PDef->meta )
				{
					printf( "%s::" ANSI_COLOR_YELLOW "%s (0x%04x)\n" ANSI_COLOR_RESET,
						ClassIDToText( aafd, Class->ID ),
						PDef->name,
						PDef->pid );

					print++;
				}
			}

			if ( print )
				printf( "\n" );

			print = 1;

		}

		printf( "\n\n" );
	}


	if ( aaf_properties )
	{
		// aafProperty *Prop = NULL;
		aafObject *Object = aafd->Objects;

		for ( Object = aafd->Objects; Object != NULL; Object = Object->nextObj )
		{
			// char *objPath = aaf_get_ObjectPath( Object );

			printf( "\n\n\n Object @ %s\n", aaf_get_ObjectPath( Object ) );

			// free( objPath );


			aafProperty *Prop = NULL;

			for ( Prop = Object->Properties; Prop != NULL; Prop = Prop->next )
			{
				printf( ":.: (0x%04x) %s\n", Prop->pid, PIDToText( aafd, Prop->pid ) );
				//
				// WARNING : Wont print strong references (set/vector) corectly.
				cfb_printStream( Prop->val, Prop->len );
			}
		}
	}






	if ( aafi != NULL )
	{
		aafi_release( &aafi );
	}
	else if ( aafd != NULL )
	{
		aaf_release( &aafd );
	}

	return 0;
}

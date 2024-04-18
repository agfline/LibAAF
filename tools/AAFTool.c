/*
 * Copyright (C) 2017-2024 Adrien Gesta-Fline
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
#include <math.h>
#include <locale.h>

#ifdef _MSC_VER
	#include "win/getopt.h"
#else
	#include <getopt.h>
#endif

#ifdef __MINGW32__
#include <fcntl.h>
#include <io.h>
#endif

#include <libaaf.h>
#include <libaaf/utils.h> // ANSI colors, laaf_util_c99strdup()

#ifdef HAVE_SNDFILE
	#include <sndfile.h>
#endif

#include "./thirdparty/libTC.h"


#ifndef BIN_NAME
#define BIN_NAME "aaftool"
#endif

#define BUILD_YEAR (__DATE__ + 7)


#define POS_FORMAT_BUFFER_LEN 64
#define FLOAT_2_STR_BUFFER_LEN 12

enum pos_format {
	POS_FORMAT_TC = 0,
	POS_FORMAT_SAMPLES,
	POS_FORMAT_HMS,
	POS_FORMAT_RAW
};

#define INTERPOL_TO_STRING( x ) \
	(x) ? \
		(x->flags & AAFI_INTERPOL_NONE)     ? "CURV_NON" : \
		(x->flags & AAFI_INTERPOL_LINEAR)   ? "CURV_LIN" : \
		(x->flags & AAFI_INTERPOL_LOG)      ? "CURV_LOG" : \
		(x->flags & AAFI_INTERPOL_CONSTANT) ? "CURV_CST" : \
		(x->flags & AAFI_INTERPOL_POWER)    ? "CURV_PWR" : \
		(x->flags & AAFI_INTERPOL_BSPLINE)  ? "CURV_BSP" : \
		"" : ""

#define ESSENCE_TYPE_TO_STRING( type ) \
	( type == AAFI_ESSENCE_TYPE_PCM  ) ? "PCM " : \
	( type == AAFI_ESSENCE_TYPE_WAVE ) ? "WAVE" : \
	( type == AAFI_ESSENCE_TYPE_AIFC ) ? "AIFC" : \
	( type == AAFI_ESSENCE_TYPE_BWAV ) ? "BWAV" : \
	( type == AAFI_ESSENCE_TYPE_UNK  ) ? "UNK " : \
	""



static const char * gainToStr( aafiAudioGain *gain );
static const char * panToStr( aafiAudioPan *pan );
static void dumpVaryingValues( AAF_Iface *aafi, aafiAudioGain *Gain, char showdb, const char *padding );
static const char * formatPosValue( aafPosition_t pos, aafRational_t *editRate, enum pos_format posFormat, enum TC_FORMAT tcFormat, aafRational_t *samplerateRational, char *buf );
static void showHelp( void );



#define log( log, ... ) \
	LOG_BUFFER_WRITE( log, __VA_ARGS__ ); \
	(log)->log_callback( log, (void*)aafi, LOG_SRC_ID_DUMP, 0, "", "", 0, log->_msg, log->user );



static const char * float2str( char *buf, const char *format, double num ) {

	snprintf( buf, FLOAT_2_STR_BUFFER_LEN, format, num );

	size_t buflen = strlen(buf);

	for ( size_t i = 0; i < buflen; i++ ) {
		if ( buf[i] == ',' )
			buf[i] = '.';
	}

	return buf;
}



static const char * gainToStr( aafiAudioGain *gain ) {

	static char str[FLOAT_2_STR_BUFFER_LEN];

	if ( gain == NULL ) {
		return "none    ";
	}
	else
	if ( gain->flags & AAFI_AUDIO_GAIN_CONSTANT ) {
		return float2str( str, "%+05.1lf dB", 20 * log10(aafRationalToDouble(gain->value[0])) );
	}
	else
	if ( gain->flags & AAFI_AUDIO_GAIN_VARIABLE ) {
		return "(A)     ";
	}

	return " wtf ? ";
}



static const char * panToStr( aafiAudioPan *pan ) {

	static char str[FLOAT_2_STR_BUFFER_LEN];

	if ( pan == NULL ) {
		return "none   ";
	}
	else if ( pan->flags & AAFI_AUDIO_GAIN_CONSTANT ) {

		char f2sbuf[FLOAT_2_STR_BUFFER_LEN];

		double panval = aafRationalToDouble( (*pan->value) );

		snprintf( str, 32, "%s %s ",
			float2str( f2sbuf, "%0.2f", panval ),
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
 * NOTE : since aafiAudioPan is an alias of aafiAudioGain, the
 * function can be called for both.
 */

static void dumpVaryingValues( AAF_Iface *aafi, aafiAudioGain *Gain, char showdb, const char *padding ) {

	char f2sbuf1[FLOAT_2_STR_BUFFER_LEN];
	char f2sbuf2[FLOAT_2_STR_BUFFER_LEN];

	for ( unsigned int i = 0; i < Gain->pts_cnt; i++ ) {

		log( aafi->log, "%s_time: %s%s%s   _value: %s%s%s",
			padding,
			ANSI_COLOR_DARKGREY(aafi->log),
			float2str( f2sbuf1, "%0.6f", aafRationalToDouble( Gain->time[i] ) ),
			ANSI_COLOR_RESET(aafi->log),
			ANSI_COLOR_DARKGREY(aafi->log),
			float2str( f2sbuf2, "%0.6f", aafRationalToDouble( Gain->value[i] ) ),
			ANSI_COLOR_RESET(aafi->log) );

		if ( showdb ) {
			log( aafi->log, " %s(%s dB)%s",
				ANSI_COLOR_DARKGREY(aafi->log),
				float2str( f2sbuf1, "%+05.1lf", 20 * log10( aafRationalToDouble( Gain->value[i] ) ) ),
				ANSI_COLOR_RESET(aafi->log) );
		}

		log( aafi->log, "\n" );
	}
}



static const char * formatPosValue( aafPosition_t pos, aafRational_t *editRate, enum pos_format posFormat, enum TC_FORMAT tcFormat, aafRational_t *samplerateRational, char *buf ) {

	struct timecode tc;

	if ( posFormat == POS_FORMAT_TC ) {
		tc_set_by_unitValue( &tc, pos, (rational_t*)editRate, tcFormat, TC_NO_ROLLOVER );
		snprintf( buf, POS_FORMAT_BUFFER_LEN, "%s", tc.string );
		return buf;
	}
	else if ( posFormat == POS_FORMAT_SAMPLES ) {
		snprintf( buf, POS_FORMAT_BUFFER_LEN, "%"PRIi64, aafi_convertUnit( pos, editRate, samplerateRational ) );
		return buf;
	}
	else if ( posFormat == POS_FORMAT_HMS ) {

		uint32_t unitPerHour = (uint32_t)aafRationalToDouble(*editRate) * 3600;
		uint32_t unitPerMin  = (uint32_t)aafRationalToDouble(*editRate) * 60;
		uint32_t unitPerSec  = (uint32_t)aafRationalToDouble(*editRate);
		uint32_t unitPerMS   = (uint32_t)aafRationalToDouble(*editRate) / 1000;

		int64_t h  = (unitPerHour) ?  (pos / unitPerHour) : 0;
		int64_t m  = (unitPerMin)  ? ((pos % unitPerHour) / unitPerMin) : 0;
		int64_t s  = (unitPerSec)  ? ((pos % unitPerHour  % unitPerMin) / unitPerSec) : 0;
		int64_t ms = (unitPerMS)   ? ((pos % unitPerHour  % unitPerMin  % unitPerSec) / unitPerMS) : 0;

		snprintf( buf, POS_FORMAT_BUFFER_LEN, "%02"PRIi64":%02"PRIi64":%02"PRIi64".%03"PRIi64, h, m, s, ms );
		return buf;
	}
	else if ( posFormat == POS_FORMAT_RAW ) {
		snprintf( buf, POS_FORMAT_BUFFER_LEN, "%"PRIi64, pos );
		return buf;
	}

	return "wtf ?";
}



static size_t getLongestMetadataName( aafiMetaData *metadataList ) {

	size_t longestNameLen = 0;

	aafiMetaData *metadata = NULL;

	AAFI_foreachMetadata( metadataList, metadata ) {
		if ( metadata->name && strlen(metadata->name) > longestNameLen )
			longestNameLen = strlen(metadata->name);
	}

	return longestNameLen;
}



static void showHelp( void ) {

	fprintf( stderr,
		"usage: %s [analysis|extraction] [options] [AAFFILE]\n"
		"\n", BIN_NAME );

	fprintf( stderr,
		" CFB Analysis:\n"
		"\n"
		"   --cfb-header                       Display CFB Header.\n"
		"   --cfb-fat                          Display CFB FAT.\n"
		"   --cfb-minifat                      Display CFB MiniFAT.\n"
		"   --cfb-difat                        Display CFB DiFAT.\n"
		"   --cfb-nodes                        Display CFB node list.\n"
		"\n"
		"   --cfb-node                 <path>  Display node located at the given <path>.\n"
		"\n"
		"\n" );

	fprintf( stderr,
		" AAF Analysis:\n"
		"\n"
		"   --aaf-summary                      Display informations from both header and identification objects.\n"
		"   --aaf-essences                     Display retrieved AAF audio and video essences.\n"
		"   --aaf-clips                        Display retrieved AAF audio and video clips.\n"
		"   --aaf-classes                      Display classes contained in AAF file.\n"
		"   --aaf-meta                         Display classes and properties from the MetaDictionary.\n"
		"   --aaf-properties                   Display properties of all objects in file.\n"
		"\n"
		"   --trace                            Display AAF class/object structure while parsing.\n"
		"   --dump-meta                        Display MetaProperties details for each parsed class containing meta properties.\n"
		"   --dump_tagged_value                Display standard TaggedValue properties of Mobs and Components objects.\n"
		"   --dump-class         <AAFClassID>  Display aaf properties of a specific AAFClass when it is parsed.\n"
		"   --dump-class-raw     <AAFClassID>  Display raw properties of a specific AAFClass when it is parsed.\n"
		"\n"
		"\n"
		" Embedded Media Extraction:\n"
		"\n"
#ifdef HAVE_SNDFILE
		// "   --extract-essences                 Extract all embedded audio essences in their original format (wav, bwav,\n"
		// "                                      aiff or raw PCM), unless --extract-format is set.\n"
		"   --extract-essences                 Extract all embedded audio essences byte-for-byte in their original format\n"
		"                                      unless --extract-format is set.\n"
		"   --extract-clips                    Extract all embedded audio clips (portion of essence). If a multichannel clip\n"
		"                                      is composed of multiple mono files, they are all interleaved in a single\n"
		"                                      multichannel output file.\n"
		"   --extract-mono-clips               Extract all embedded audio clips (portion of essence). If a multichannel clip\n"
		"                                      is composed of multiple mono files, they are all exported as separate files.\n"
#else
		"   --extract-essences                 Extract all embedded audio essences byte-for-byte in their original format\n"
		"                                      unless --extract-format is set to originpcm, in which case no file header is\n"
		"                                      written.\n"
#endif
		"\n"
		"   --extract-path             <path>  Location where embedded files are extracted.\n"
		"   --extract-mobid                    Name extracted files with their MobID. This also prevents any non-latin\n"
		"                                      character in filename.\n"
		"\n"
#ifdef HAVE_SNDFILE
		"   --extract-format            <fmt>  Force extract format. Possible <fmt> values are: wav, bwav, aiff, pcm, origin\n"
		"                                      (byte-for-byte copy of original file, same as no --extract-format) and originpcm\n"
		"                                      (byte-for-byte copy of raw original pcm stream, without file header)\n"
		"\n"
		"   --extract-samplesize       <bits>  Force extract sample size. Possible <bits> values are: s8, s16, s24, s32, f32\n"
		"   --extract-samplerate       <rate>  Force extract sample rate. Only sets output file sample rate without conversion.\n"
		"\n"
#else
		"   --extract-format            <fmt>  Force extract format. Possible <fmt> vlaues are: origin (byte-for-byte copy of\n"
		"                                      original file, same as no --extract-format) and originpcm (byte-for-byte copy\n"
		"                                      of raw original pcm stream, without file header)\n"
#endif
		"\n"
		"\n"
		" Software Specific Options:\n"
		"\n"
		"   --pt-true-fades                    Replace rendered ProTools fade clips with real fades when possible, when\n"
		"                                      surrounding clips has enough handle size.\n"
		"   --pt-remove-sae                    Remove all ProTools \"sample accurate edit\" small clips.\n"
		"\n"
		"\n"
		" Options:\n"
		"\n"
		"   --media-location           <path>  Location of external audio and video essence files.\n"
		"\n"
		"   --samplerate               <rate>  Sample rate used for converting displayed position and duration values. If not\n"
		"                                      set, AAF dominant sample rate is used.\n"
		"   --pos-format <tc|hms|samples|raw>  Position and duration display format.\n"
		"   --show-automation                  Show track and clip automation values when --aaf-clips is set.\n"
		"   --show-metadata                    Show source and clip metadata when --aaf-essences or --aaf-clips is set.\n"
		"\n"
		"   --relative-path                    Prints AAF's relative essence path when --aaf-essences is set.\n"
		"\n"
		"   --no-color                         Disable colored output.\n"
		"   --log-file                 <file>  Save output to file instead of stdout.\n"
		"\n"
		"   --verb                      <num>  0=quiet 1=error 2=warning 3=debug.\n"
		"\n\n" );
}



int main( int argc, char *argv[] ) {

#ifdef _WIN32
	#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
		#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
	#endif
	/* enables "VT100" ANSI colors and UTF-8 console code page */
	HANDLE hOut = GetStdHandle( STD_OUTPUT_HANDLE );
	DWORD dwMode = 0;
	GetConsoleMode( hOut, &dwMode );
	SetConsoleMode( hOut, (dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) );
	SetConsoleOutputCP(65001);
#endif

#ifdef __MINGW32__
	setlocale( LC_ALL, "" );
#elif defined(_MSC_VER)
	setlocale( LC_ALL, ".utf8" );
#else
	setlocale( LC_ALL, "" );
#endif


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

	int extract_essences   = 0;
	int extract_clips      = 0;
	const char *extract_path = NULL;
	int extract_mobid_filename = 0;
	int extract_format     = AAFI_EXTRACT_ORIGINAL;
	int extract_samplesize = 0;
	int extract_samplerate = 0;
	int extract_mono_clips = 0;

	int protools_options   = 0;

	const char *media_location = NULL;

	aafRational_t displaySamplerate = { 0, 1 };
	enum pos_format posFormat = POS_FORMAT_TC;
	int show_automation    = 0;
	int show_metadata      = 0;
	int relative_path      = 0;

	enum verbosityLevel_e verb = VERB_WARNING;
	int trace = 0;
	int dump_meta = 0;
	int dump_tagged_value = 0;
	int ansicolor = 1;

	const char *logfile = NULL;
	FILE *logfp = stdout;

	const char *dump_class_aaf_properties = NULL;
	const char *dump_class_raw_properties = NULL;

	char *aafPath = NULL;

	int cmd = 0;
	int cmderror = 0;

	fprintf( logfp, "\n%s Copyright (c) 2017-%s Adrien Gesta-Fline\nlibaaf %s\n", BIN_NAME, BUILD_YEAR, LIBAAF_VERSION );
#ifdef HAVE_SNDFILE
	fprintf( logfp, "Using: %s\n", sf_version_string() );
#endif
	fprintf( logfp, "\n" );


	static struct option long_options[] = {

		{ "help",                 no_argument,        0,   'h' },

		{ "cfb-header",           no_argument,        0,  0x01 },
		{ "cfb-fat",              no_argument,        0,  0x02 },
		{ "cfb-minifat",          no_argument,        0,  0x03 },
		{ "cfb-difat",            no_argument,        0,  0x04 },
		{ "cfb-nodes",            no_argument,        0,  0x05 },

		{ "cfb-node",             required_argument,  0,  0x06 },

		{ "aaf-summary",          no_argument,        0,  0x10 },
		{ "aaf-essences",         no_argument,        0,  0x11 },
		{ "aaf-clips",            no_argument,        0,  0x12 },
		{ "aaf-classes",          no_argument,        0,  0x13 },
		{ "aaf-meta",             no_argument,        0,  0x14 },
		{ "aaf-properties",       no_argument,        0,  0x15 },

		{ "trace",                no_argument,        0,  0x20 },
		{ "dump-meta",            no_argument,        0,  0x21 },
		{ "dump-tagged-value",    no_argument,        0,  0x22 },
		{ "dump-class",           required_argument,  0,  0x23 },
		{ "dump-class-raw",       required_argument,  0,  0x24 },

		{ "extract-essences",     no_argument,        0,  0x30 },
#ifdef HAVE_SNDFILE
		{ "extract-clips",        no_argument,        0,  0x31 },
		{ "extract-mono-clips",   no_argument,        0,  0x37 },
#endif
		{ "extract-path",         required_argument,  0,  0x32 },
		{ "extract-mobid",        no_argument,        0,  0x33 },
		{ "extract-format",       required_argument,  0,  0x34 },
#ifdef HAVE_SNDFILE
		{ "extract-samplesize",   required_argument,  0,  0x35 },
		{ "extract-samplerate",   required_argument,  0,  0x36 },
#endif

		{ "pt-true-fades",        no_argument,        0,  0x40 },
		{ "pt-remove-sae",        no_argument,        0,  0x41 },

		{ "media-location",       required_argument,  0,  0x50 },
		{ "samplerate",           required_argument,  0,  0x51 },
		{ "pos-format",           required_argument,  0,  0x52 },

		{ "show-automation",      no_argument,        0,  0x53 },
		{ "show-metadata",        no_argument,        0,  0x54 },
		{ "relative-path",        no_argument,        0,  0x55 },

		{ "no-color",             no_argument,        0,  0x56 },

		{ "log-file",             required_argument,  0,  0x57 },
		{ "verb",                 required_argument,  0,  0x58 },

		{ 0,                      0,                  0,  0x00 }
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
			case 0x01:  cfb_header     = 1;         cmd++;          break;
			case 0x02:  cfb_fat        = 1;         cmd++;          break;
			case 0x03:  cfb_minifat    = 1;         cmd++;          break;
			case 0x04:  cfb_difat      = 1;         cmd++;          break;
			case 0x05:  cfb_nodes      = 1;         cmd++;          break;

			case 0x06:
				free( get_node_str );
				get_node_str = laaf_util_c99strdup( optarg );
			                                        cmd++;          break;

			case 0x10:  aaf_summary    = 1;         cmd++;          break;
			case 0x11:  aaf_essences   = 1;         cmd++;          break;
			case 0x12:  aaf_clips      = 1;         cmd++;          break;
			case 0x13:  aaf_classes    = 1;         cmd++;          break;
			case 0x14:  aaf_meta       = 1;         cmd++;          break;
			case 0x15:  aaf_properties = 1;         cmd++;          break;

			case 0x20:  trace = 1;                  cmd++;          break;
			case 0x21:  dump_meta = 1;                              break;
			case 0x22:  dump_tagged_value = 1;                      break;
			case 0x23:  dump_class_aaf_properties = optarg;         break;
			case 0x24:  dump_class_raw_properties = optarg;         break;

			case 0x30:  extract_essences = 1;       cmd++;          break;
			case 0x32:  extract_path = optarg;                      break;
			case 0x33:  extract_mobid_filename = 1; cmd++;          break;

#ifdef HAVE_SNDFILE
			case 0x31:  extract_clips = 1;          cmd++;          break;

			case 0x34:
				if      ( strcmp( optarg, "wav"       ) == 0 ) extract_format = AAFI_EXTRACT_WAV;
				else if ( strcmp( optarg, "bwav"      ) == 0 ) extract_format = AAFI_EXTRACT_BWAV;
				else if ( strcmp( optarg, "aiff"      ) == 0 ) extract_format = AAFI_EXTRACT_AIFF;
				else if ( strcmp( optarg, "pcm"       ) == 0 ) extract_format = AAFI_EXTRACT_PCM;
				else if ( strcmp( optarg, "origin"    ) == 0 ) extract_format = AAFI_EXTRACT_ORIGINAL;
				else if ( strcmp( optarg, "originpcm" ) == 0 ) extract_format = AAFI_EXTRACT_ORIGINAL_PCM;
				else {
					fprintf( stderr,
						"Command line error: wrong --extract-format <value>\n"
						"Try '%s --help' for more informations.\n", BIN_NAME );
					goto err;
				}
				break;

			case 0x35:
				if      ( strcmp( optarg, "s8"  ) == 0 ) extract_samplesize = SF_FORMAT_PCM_S8;
				else if ( strcmp( optarg, "s16" ) == 0 ) extract_samplesize = SF_FORMAT_PCM_16;
				else if ( strcmp( optarg, "s24" ) == 0 ) extract_samplesize = SF_FORMAT_PCM_24;
				else if ( strcmp( optarg, "s32" ) == 0 ) extract_samplesize = SF_FORMAT_PCM_32;
				else if ( strcmp( optarg, "f32" ) == 0 ) extract_samplesize = SF_FORMAT_FLOAT;
				else {
					fprintf( stderr,
						"Command line error: wrong --extract-samplesize <value>\n"
						"Try '%s --help' for more informations.\n", BIN_NAME );
					goto err;
				}
				break;

			case 0x36:
				extract_samplerate = atoi(optarg);

				if ( extract_samplerate < 0 ) {
					fprintf( stderr,
						"Command line error: wrong --extract-samplerate <value>\n"
						"Try '%s --help' for more informations.\n", BIN_NAME );
					goto err;
				}
				break;

			case 0x37:
				extract_mono_clips = 1;
				break;
#else
			case 0x34:
				if      ( strcmp( optarg, "origin"    ) == 0 ) extract_format = AAFI_EXTRACT_ORIGINAL;
				else if ( strcmp( optarg, "originpcm" ) == 0 ) extract_format = AAFI_EXTRACT_ORIGINAL_PCM;
				else {
					fprintf( stderr,
						"Command line error: wrong --extract-format <value>\n"
						"Try '%s --help' for more informations.\n", BIN_NAME );
					goto err;
				}
				break;
#endif
			case 0x40:  protools_options |= AAFI_PROTOOLS_OPT_REPLACE_CLIP_FADES;          break;
			case 0x41:  protools_options |= AAFI_PROTOOLS_OPT_REMOVE_SAMPLE_ACCURATE_EDIT; break;

			case 0x50:  media_location = optarg;                    break;
			case 0x51:  displaySamplerate.numerator = atoi(optarg); break;
			case 0x52:
				if      ( strcmp( optarg, "tc"      ) == 0 ) posFormat = POS_FORMAT_TC;
				else if ( strcmp( optarg, "samples" ) == 0 ) posFormat = POS_FORMAT_SAMPLES;
				else if ( strcmp( optarg, "hms"     ) == 0 ) posFormat = POS_FORMAT_HMS;
				else if ( strcmp( optarg, "raw"     ) == 0 ) posFormat = POS_FORMAT_RAW;
				else {
					fprintf( stderr,
						"Command line error: wrong --pos-format <value>\n"
						"Try '%s --help' for more informations.\n", BIN_NAME );
					goto err;
				}
				break;

			case 0x53:  show_automation = 1;                        break;
			case 0x54:  show_metadata = 1;                          break;
			case 0x55:  relative_path = 1;                          break;

			case 0x56:  ansicolor = 0;                              break;

			case 0x57:	logfile = optarg;                           break;
			case 0x58:  verb = atoi(optarg);                        break;

			case 'h':	showHelp();                                goto end;

			default:
				cmderror++;
				fprintf( stderr, "Try '%s --help' for more informations.\n", BIN_NAME );
				break;
		}
	}

	if ( cmderror ) {
		goto err;
	}

	if ( optind == argc ) {
		fprintf( stderr,
			"Command line error: missing AAF file\n"
			"Try '%s --help' for more informations.\n", BIN_NAME );
		goto err;
	}


	if ( cmd == 0 ) {
		fprintf( stderr,
			"Command line error: at least one analysis or extract option is required.\n"
			"Try '%s --help' for more informations.\n", BIN_NAME );
		goto err;
	}


	if ( !extract_path ) {

		if ( extract_clips || extract_mono_clips ) {
			fprintf( stderr,
				"Command line error: %s requires --extract-path to be set.\n"
				"Try '%s --help' for more informations.\n", (extract_clips) ? "--extract-clips" : "--extract-mono-clips", BIN_NAME );
			goto err;
		}

		if ( extract_essences ) {
			fprintf( stderr,
				"Command line error: --extract-essences requires --extract-path to be set.\n"
				"Try '%s --help' for more informations.\n", BIN_NAME );
			goto err;
		}
	}


	if ( extract_clips + extract_mono_clips + extract_essences > 1 ) {
		fprintf( stderr,
			"Command line error: only one of the following can be used at a time : --extract-clips --extract-mono-clips --extract-essences\n"
			"Try '%s --help' for more informations.\n", BIN_NAME );
		goto err;
	}



	if ( extract_format == AAFI_EXTRACT_ORIGINAL ||
	     extract_format == AAFI_EXTRACT_ORIGINAL_PCM )
	{
		if ( extract_clips || extract_mono_clips ) {
			fprintf( stderr,
				"Command line error: can't use %s with --extract-format set to \"origin\" or \"originpcm\".\n"
				"Try '%s --help' for more informations.\n", (extract_clips) ? "--extract-clips" : "--extract-mono-clips", BIN_NAME );
			goto err;
		}

		if ( extract_samplerate ) {
			fprintf( stderr,
				"Command line error: can't use --extract-samplerate with --extract-format set to \"origin\" or \"originpcm\".\n"
				"Try '%s --help' for more informations.\n", BIN_NAME );
			goto err;
		}

		if ( extract_samplesize ) {
			fprintf( stderr,
				"Command line error: can't use --extract-samplesize with --extract-format set to \"origin\" or \"originpcm\".\n"
				"Try '%s --help' for more informations.\n", BIN_NAME );
			goto err;
		}
	}


	if ( verb < VERB_QUIET || verb >= MAX_VERB ) {
		fprintf( stderr,
			"Command line error: wrong --verb <value>\n"
			"Try '%s --help' for more informations.\n", BIN_NAME );
		goto err;
	}


	aafi = aafi_alloc( NULL );

	if ( !aafi ) {
		fprintf( stderr, "Failed to init AAF_Iface context.\n" );
		goto err;
	}

	aafd = aafi->aafd;

	if ( logfile ) {
		logfp = fopen( logfile, "w" );

		if ( logfp == NULL ) {
			fprintf( stderr, "Failed to open logfile for writting : %s\n", logfile );
			goto err;
		}
	}

#ifdef __MINGW32__
	/* mingw fails to print UTF-8 without _setmode() */
	_setmode( _fileno(logfp), _O_U8TEXT );
#endif

	aafi_set_debug( aafi, verb, ansicolor, logfp, NULL, NULL );

	aafi_set_option( aafi, AAFI_OPT_TRACE,                  trace                     );
	aafi_set_option( aafi, AAFI_OPT_DUMP_META,              dump_meta                 );
	aafi_set_option( aafi, AAFI_OPT_DUMP_TAGGED_VALUE,      dump_tagged_value         );
	aafi_set_option( aafi, AAFI_OPT_DUMP_CLASS_PROPS,       dump_class_aaf_properties );
	aafi_set_option( aafi, AAFI_OPT_DUMP_CLASS_RAW_PROPS,   dump_class_raw_properties );

	aafi_set_option( aafi, AAFI_OPT_MEDIA_LOCATION,         media_location            );
	aafi_set_option( aafi, AAFI_OPT_MOBID_ESSENCE_FILENAME, extract_mobid_filename    );

	aafi_set_option( aafi, AAFI_OPT_PROTOOLS,               protools_options          );


	if ( aafi_load_file( aafi, argv[argc-1] ) ) {
		fprintf( stderr, "Failed to open %s\n", argv[argc-1] );
		goto err;
	}

	if ( displaySamplerate.numerator == 0 ) {
		displaySamplerate.numerator = (int)aafi->Audio->samplerate;
	}


	/* use aafi->aafd->cfbd->file instead of argv[argc-1] because it is set absolute */
	aafPath = laaf_util_c99strdup( aafi->aafd->cfbd->file );

	if ( !aafPath ) {
		fprintf( stderr, "Could not duplicate AAF filepath : %s", aafi->aafd->cfbd->file );
		goto err;
	}

	char *p = aafPath + strlen(aafPath);

	while ( p > aafPath ) {
		if ( IS_DIR_SEP(*p) ) {
			*p = 0x00;
			break;
		}
		p--;
	}


	log( aafi->log, "\n" );


	if ( get_node_str != NULL ) {

		cfbNode *node = cfb_getNodeByPath( aafd->cfbd, get_node_str, 0 );

		if ( node == NULL ) {
			fprintf( stderr, "Could not find node at \"%s\"\n", get_node_str );
		}
		else {
			cfb_dump_node( aafd->cfbd, node, 0, " " );

			char *name = cfb_w16toUTF8( node->_ab, node->_cb );

			if ( strncmp( name, "properties", 10 ) == 0 ) {
				aaf_dump_nodeStreamProperties( aafd, node, " " );
			}
			else if ( node->_mse == STGTY_STREAM ) {
				cfb_dump_nodeStream( aafd->cfbd, node, " " );
			}

			free( name );
		}

		log( aafi->log, "\n\n" );
	}


	if ( cfb_header ) {
		cfb_dump_header( aafd->cfbd, " " );
	}


	if ( cfb_fat ) {
		cfb_dump_FAT( aafd->cfbd, " " );
	}


	if ( cfb_minifat ) {
		cfb_dump_MiniFAT( aafd->cfbd, " " );
	}


	if ( cfb_difat ) {
		cfb_dump_DiFAT( aafd->cfbd, " " );
	}


	if ( cfb_nodes ) {
		uint32_t i = 0;
		cfb_dump_nodePaths( aafd->cfbd, 0, NULL, &i, NULL, " ", 1 );
	}


	enum TC_FORMAT tcFormat = tc_get_format_by_fps( aafRationalToFloat(*aafi->Timecode->edit_rate), aafi->Timecode->drop );

	if ( tcFormat == TC_FORMAT_UNK ) {
		/*
		 * Needed for non-SMPTE frame rates from Adobe Premiere Pro (eg. 300260000/10000000, 30.025999)
		 */
		tcFormat = tc_get_format_by_fps( (float)aafi->Timecode->fps, aafi->Timecode->drop );
	}

	if ( tcFormat == TC_FORMAT_UNK ) {
		fprintf( stderr, "Could not set timecode format: %i (%i/%i, %f)\n", aafi->Timecode->fps, aafi->Timecode->edit_rate->numerator, aafi->Timecode->edit_rate->denominator, aafRationalToDouble(*aafi->Timecode->edit_rate) );
		goto err;
	}


	if ( aaf_summary ) {

		aaf_dump_Header( aafd, " " );

		log( aafd->log, "\n" );

		aaf_dump_Identification( aafd, " " );

		log( aafd->log, "\n" );

		aafPosition_t compoStart = aafi_convertUnit( aafi->compositionStart, aafi->compositionStart_editRate, aafi->compositionLength_editRate );

		struct timecode tc_start;
		struct timecode tc_end;

		tc_set_by_unitValue( &tc_end, aafi_convertUnit( compoStart + aafi->compositionLength, aafi->compositionLength_editRate, aafi->Timecode->edit_rate ), (rational_t*)aafi->Timecode->edit_rate, tcFormat, TC_NO_ROLLOVER );
		tc_set_by_unitValue( &tc_start, aafi->Timecode->start, (rational_t*)aafi->Timecode->edit_rate, tcFormat, TC_NO_ROLLOVER );

		log( aafd->log, " Composition Name            : %s%s%s\n", ANSI_COLOR_DARKGREY(aafi->log), aafi->compositionName, ANSI_COLOR_RESET(aafi->log) );

		log( aafd->log, "\n" );

		log( aafd->log, " TC EditRrate                : %s%i/%i%s\n", ANSI_COLOR_DARKGREY(aafi->log), aafi->Timecode->edit_rate->numerator, aafi->Timecode->edit_rate->denominator, ANSI_COLOR_RESET(aafi->log) );
		log( aafd->log, " TC FPS                      : %s%u %s%s\n", ANSI_COLOR_DARKGREY(aafi->log), aafi->Timecode->fps, (aafi->Timecode->drop) ? "DF" : "NDF", ANSI_COLOR_RESET(aafi->log) );

		log( aafd->log, "\n" );

		log( aafd->log, " Composition Start (EU)      : %s%"PRIi64"%s\n", ANSI_COLOR_DARKGREY(aafi->log), aafi->compositionStart, ANSI_COLOR_RESET(aafi->log) );
		log( aafd->log, " Composition Start (samples) : %s%"PRIi64"%s\n", ANSI_COLOR_DARKGREY(aafi->log), aafi_convertUnit( aafi->compositionStart, aafi->compositionStart_editRate, &displaySamplerate ), ANSI_COLOR_RESET(aafi->log) );
		log( aafd->log, " Composition Start           : %s%s%s\n", ANSI_COLOR_DARKGREY(aafi->log), tc_start.string, ANSI_COLOR_RESET(aafi->log) );

		log( aafd->log, "\n" );

		log( aafd->log, " Composition End (EU)        : %s%"PRIi64" (EditRate: %i/%i)%s\n", ANSI_COLOR_DARKGREY(aafi->log), compoStart + aafi->compositionLength, (aafi->compositionLength_editRate) ? aafi->compositionLength_editRate->numerator : 0, (aafi->compositionLength_editRate) ? aafi->compositionLength_editRate->denominator : 0, ANSI_COLOR_RESET(aafi->log) );
		log( aafd->log, " Composition End (samples)   : %s%"PRIi64"%s\n", ANSI_COLOR_DARKGREY(aafi->log), aafi_convertUnit( compoStart + aafi->compositionLength, aafi->compositionLength_editRate, &displaySamplerate ), ANSI_COLOR_RESET(aafi->log) );
		log( aafd->log, " Composition End             : %s%s%s\n", ANSI_COLOR_DARKGREY(aafi->log), tc_end.string, ANSI_COLOR_RESET(aafi->log) );

		log( aafd->log, "\n" );

		log( aafd->log, " Dominant Sample Rate        : %s%u%s\n", ANSI_COLOR_DARKGREY(aafi->log), aafi->Audio->samplerate, ANSI_COLOR_RESET(aafi->log) );
		log( aafd->log, " Dominant Sample Size        : %s%u bits%s\n", ANSI_COLOR_DARKGREY(aafi->log), aafi->Audio->samplesize, ANSI_COLOR_RESET(aafi->log) );

		if ( aafi->metadata != NULL )	{

			log( aafd->log, "\n" );

			log( aafd->log, " Metadata:\n" );

			size_t longestName = getLongestMetadataName( aafi->metadata ) + 1;

			aafiMetaData *metadata = NULL;

			AAFI_foreachMetadata( aafi->metadata, metadata ) {
				log( aafd->log, " - Name: %s\"%s\"%s%*s Text: %s\"%s\"%s\n",
					ANSI_COLOR_DARKGREY(aafi->log),
					(metadata->name) ? metadata->name : "<unknown>",
					ANSI_COLOR_RESET(aafi->log),

					(metadata->name) ? (int)(longestName-strlen(metadata->name)) : (int)(longestName-strlen("<unknown>")), " ",

					ANSI_COLOR_DARKGREY(aafi->log),
					metadata->text,
					ANSI_COLOR_RESET(aafi->log) );
			}
		}

		log( aafd->log, "\n\n" );
	}


	if ( aaf_classes ) {
		aaf_dump_Classes( aafd, " " );
		log( aafd->log, "\n\n" );
	}


	if ( aaf_meta ) {
		aaf_dump_MetaDictionary( aafd, " " );
		log( aafd->log, "\n\n" );
	}


	if ( aaf_properties ) {

		aafObject *Object = NULL;

		for ( Object = aafd->Objects; Object != NULL; Object = Object->nextObj ) {
			log( aafd->log, "\n\n\n Object @ %s%s%s\n", ANSI_COLOR_DARKGREY(aafd->log), aaf_get_ObjectPath( Object ), ANSI_COLOR_RESET(aafd->log) );
			aaf_dump_ObjectProperties( aafd, Object, " " );
		}

		log( aafd->log, "\n\n" );
	}


	if ( aaf_essences ) {

		log( aafi->log, "Media Essences :\n"
		                "================\n\n" );

		uint32_t essenceIndex = 1;
		aafiVideoEssenceFile *videoEssenceFile = NULL;

		AAFI_foreachVideoEssenceFile( aafi, videoEssenceFile )
		{
			char posFormatBuf[POS_FORMAT_BUFFER_LEN];

			log( aafi->log, " Video[%u] :: Length: %s%s%s  Name: %s\"%s\"%s  UniqueName: %s\"%s\"%s\n",
				essenceIndex,

				ANSI_COLOR_DARKGREY(aafi->log),
				formatPosValue( videoEssenceFile->length, &displaySamplerate, posFormat, tcFormat, &displaySamplerate, posFormatBuf ),
				ANSI_COLOR_RESET(aafi->log),

				ANSI_COLOR_DARKGREY(aafi->log),
				videoEssenceFile->name,
				ANSI_COLOR_RESET(aafi->log),

				ANSI_COLOR_DARKGREY(aafi->log),
				videoEssenceFile->unique_name,
				ANSI_COLOR_RESET(aafi->log) );

			char *relativeFilePath = NULL;

			if ( !videoEssenceFile->is_embedded && videoEssenceFile->usable_file_path && relative_path ) {
				relativeFilePath = laaf_util_relative_path( videoEssenceFile->usable_file_path, aafPath );
			}

			log( aafi->log, " %s File: %s\"%s\"%s\n",
				( show_metadata && videoEssenceFile->metadata ) ? TREE_ENTRY : TREE_LAST_ENTRY,
				ANSI_COLOR_DARKGREY(aafi->log),
				( videoEssenceFile->is_embedded ) ? "EMBEDDED" : ( videoEssenceFile->usable_file_path ) ? (( relative_path ) ? relativeFilePath : videoEssenceFile->usable_file_path ) : videoEssenceFile->original_file_path,
				ANSI_COLOR_RESET(aafi->log) );

			if ( show_metadata && videoEssenceFile->metadata != NULL )	{

				size_t longestName = getLongestMetadataName( videoEssenceFile->metadata ) + 1;

				log( aafi->log, " %s Metadata:\n", TREE_LAST_ENTRY );

				aafiMetaData *metadata = NULL;

				AAFI_foreachMetadata( videoEssenceFile->metadata, metadata ) {
					log( aafi->log, "     - Name: %s\"%s\"%s%*s Text: %s\"%s\"%s\n",
						ANSI_COLOR_DARKGREY(aafi->log),
						(metadata->name) ? metadata->name : "<unknown>",
						ANSI_COLOR_RESET(aafi->log),

						(metadata->name) ? (int)(longestName-strlen(metadata->name)) : (int)(longestName-strlen("<unknown>")), " ",

						ANSI_COLOR_DARKGREY(aafi->log),
						metadata->text,
						ANSI_COLOR_RESET(aafi->log) );
				}
			}

			log( aafi->log, "\n" );
			essenceIndex++;
		}

		if ( aafi->Video->essenceFiles ) {
			log( aafi->log, "\n" );
		}


		essenceIndex = 1;
		aafiAudioEssenceFile *audioEssenceFile = NULL;

		AAFI_foreachAudioEssenceFile( aafi, audioEssenceFile ) {

			char posFormatBuf[POS_FORMAT_BUFFER_LEN];

			log( aafi->log, " Audio[%u] :: Format: %s%s - %02u ch - %s%u Hz - %s%u bits%s  Length: %s%s%s  Name: %s\"%s\"%s  UniqueName: %s\"%s\"%s\n",
				essenceIndex,

				ANSI_COLOR_DARKGREY(aafi->log),
				ESSENCE_TYPE_TO_STRING( audioEssenceFile->type ),
				audioEssenceFile->channels,
				( audioEssenceFile->samplerate >= 10000 ) ? "" : ( audioEssenceFile->samplerate >= 1000 ) ? " " : ( audioEssenceFile->samplerate >= 100 ) ? "  " : ( audioEssenceFile->samplerate >= 10 ) ? "   " : "    ",
				audioEssenceFile->samplerate,
				( audioEssenceFile->samplerate >= 10 ) ? "" : " ",
				audioEssenceFile->samplesize,
				ANSI_COLOR_RESET(aafi->log),

				ANSI_COLOR_DARKGREY(aafi->log),
				formatPosValue( audioEssenceFile->framelength, audioEssenceFile->samplerateRational, posFormat, tcFormat, audioEssenceFile->samplerateRational, posFormatBuf ),
				ANSI_COLOR_RESET(aafi->log),

				ANSI_COLOR_DARKGREY(aafi->log),
				audioEssenceFile->name,
				ANSI_COLOR_RESET(aafi->log),

				ANSI_COLOR_DARKGREY(aafi->log),
				audioEssenceFile->unique_name,
				ANSI_COLOR_RESET(aafi->log) );


			char *relativeFilePath = NULL;

			if ( !audioEssenceFile->is_embedded && audioEssenceFile->usable_file_path && relative_path ) {
				relativeFilePath = laaf_util_relative_path( audioEssenceFile->usable_file_path, aafPath );
			}

			log( aafi->log, " %s File: %s\"%s\"%s\n",
				( show_metadata && audioEssenceFile->metadata ) ? TREE_ENTRY : TREE_LAST_ENTRY,
				ANSI_COLOR_DARKGREY(aafi->log),
				( audioEssenceFile->is_embedded ) ? "EMBEDDED" : (( audioEssenceFile->usable_file_path ) ? (( relative_path ) ? relativeFilePath : audioEssenceFile->usable_file_path ) : audioEssenceFile->original_file_path),
				ANSI_COLOR_RESET(aafi->log) );

			free( relativeFilePath );


			if ( show_metadata && audioEssenceFile->metadata != NULL )	{

				size_t longestName = getLongestMetadataName( audioEssenceFile->metadata ) + 1;

				log( aafi->log, " %s Metadata:\n", TREE_LAST_ENTRY );

				aafiMetaData *metadata = NULL;

				AAFI_foreachMetadata( audioEssenceFile->metadata, metadata ) {
					log( aafi->log, "     - Name: %s\"%s\"%s%*s Text: %s\"%s\"%s\n",
						ANSI_COLOR_DARKGREY(aafi->log),
						(metadata->name) ? metadata->name : "<unknown>",
						ANSI_COLOR_RESET(aafi->log),

						(metadata->name) ? (int)(longestName-strlen(metadata->name)) : (int)(longestName-strlen("<unknown>")), " ",

						ANSI_COLOR_DARKGREY(aafi->log),
						metadata->text,
						ANSI_COLOR_RESET(aafi->log) );
				}
			}

			log( aafi->log, "\n" );
			essenceIndex++;
		}

		log( aafi->log, "\n" );
	}


	if ( aaf_clips ) {

		char f2sbuf[FLOAT_2_STR_BUFFER_LEN];

		aafPosition_t sessionStart = 0;

		log( aafi->log, "Tracks & Clips :\n"
		                "================\n\n" );

		aafiVideoTrack   *videoTrack = aafi->Video->Tracks;
		aafiTimelineItem *videoItem  = NULL;
		aafiVideoClip    *videoClip  = NULL;

		if ( videoTrack != NULL ) {

			/*
			 * Composition Timecode does not always share the same edit rate as tracks and clips.
			 * Therefore, we need to do the conversion prior to any maths.
			 * For example, if TC is 30000/1001 and audio clips are 48000/1, then TC->start has to be converted from FPS to samples.
			 */

			sessionStart = aafi_convertUnit( aafi->compositionStart, aafi->compositionStart_editRate, videoTrack->edit_rate );


			log( aafi->log, " VideoTrack[%u] ::  EditRate: %s%i/%i (%s)%s",
				videoTrack->number,
				ANSI_COLOR_DARKGREY(aafi->log),
				videoTrack->edit_rate->numerator, videoTrack->edit_rate->denominator,
				float2str( f2sbuf, "%02.2f", aafRationalToDouble(*videoTrack->edit_rate) ),
				ANSI_COLOR_RESET(aafi->log) );

			if ( videoTrack->name && videoTrack->name[0] != 0x00 ) {
				log( aafi->log, "  Name: %s\"%s\"%s",
					ANSI_COLOR_DARKGREY(aafi->log),
					videoTrack->name,
					ANSI_COLOR_RESET(aafi->log) );
			}

			log( aafi->log, "\n" );

			if ( videoTrack->timelineItems ) {

				videoItem = videoTrack->timelineItems;
				videoClip = (aafiVideoClip*)videoItem->data;

				char posFormatBuf1[POS_FORMAT_BUFFER_LEN];
				char posFormatBuf2[POS_FORMAT_BUFFER_LEN];
				char posFormatBuf3[POS_FORMAT_BUFFER_LEN];
				char posFormatBuf4[POS_FORMAT_BUFFER_LEN];

				log( aafi->log, " %s Clip (1): Start: %s%s%s  Len: %s%s%s  End: %s%s%s  SourceOffset: %s%s%s\n",
					TREE_LAST_ENTRY,

					ANSI_COLOR_DARKGREY(aafi->log),
					formatPosValue( (videoClip->pos + sessionStart),                  videoClip->track->edit_rate, posFormat, tcFormat, &displaySamplerate, posFormatBuf1 ),
					ANSI_COLOR_RESET(aafi->log),

					ANSI_COLOR_DARKGREY(aafi->log),
					formatPosValue( (videoClip->len),                                 videoClip->track->edit_rate, posFormat, tcFormat, &displaySamplerate, posFormatBuf2 ),
					ANSI_COLOR_RESET(aafi->log),

					ANSI_COLOR_DARKGREY(aafi->log),
					formatPosValue( (videoClip->pos + sessionStart + videoClip->len), videoClip->track->edit_rate, posFormat, tcFormat, &displaySamplerate, posFormatBuf3 ),
					ANSI_COLOR_RESET(aafi->log),

					ANSI_COLOR_DARKGREY(aafi->log),
					formatPosValue(  videoClip->essence_offset,                       videoClip->track->edit_rate, posFormat, tcFormat, &displaySamplerate, posFormatBuf4 ),
					ANSI_COLOR_RESET(aafi->log) );

				log( aafi->log, "\n" );
			}

			log( aafi->log, "\n" );
		}

		log( aafi->log, "\n" );



		aafiAudioTrack   *audioTrack = NULL;
		aafiTimelineItem *audioItem  = NULL;

		AAFI_foreachAudioTrack( aafi, audioTrack ) {

			/*
			 * Composition Timecode does not always share the same edit rate as tracks and clips.
			 * Therefore, we need to do the conversion prior to any maths.
			 * For example, if TC is 30000/1001 and audio clips are 48000/1, then TC->start has to be converted from FPS to samples.
			 */

			sessionStart = aafi_convertUnit( aafi->compositionStart, aafi->compositionStart_editRate, audioTrack->edit_rate );

			int trackLineItemCounter = 1;
			int totalTrackLineItems = 0;
			totalTrackLineItems += ( show_automation && audioTrack->pan  != NULL && audioTrack->pan->flags  & AAFI_AUDIO_GAIN_VARIABLE ) ? 1 : 0;
			totalTrackLineItems += ( show_automation && audioTrack->gain != NULL && audioTrack->gain->flags & AAFI_AUDIO_GAIN_VARIABLE ) ? 1 : 0;
			totalTrackLineItems += audioTrack->clipCount;

			log( aafi->log, " AudioTrack[%u] ::  EditRate: %s%i/%i (%s)%s  Format: %s%s%s",
				audioTrack->number,

				ANSI_COLOR_DARKGREY(aafi->log),
				audioTrack->edit_rate->numerator, audioTrack->edit_rate->denominator,
				float2str( f2sbuf, "%02.2f", aafRationalToDouble(*audioTrack->edit_rate) ),
				ANSI_COLOR_RESET(aafi->log),

				ANSI_COLOR_DARKGREY(aafi->log),
				(audioTrack->format == AAFI_TRACK_FORMAT_MONO)   ? "MONO" :
				(audioTrack->format == AAFI_TRACK_FORMAT_STEREO) ? "STEREO" :
				(audioTrack->format == AAFI_TRACK_FORMAT_5_1)    ? "5.1" :
				(audioTrack->format == AAFI_TRACK_FORMAT_7_1)    ? "7.1" : "Unknwn",
				ANSI_COLOR_RESET(aafi->log) );

			if ( audioTrack->solo ) {
				log( aafi->log, "  Solo: %s%s%s",
					ANSI_COLOR_DARKGREY(aafi->log),
					"SOLO",
					ANSI_COLOR_RESET(aafi->log) );
			}

			if ( audioTrack->mute ) {
				log( aafi->log, "  Mute: %s%s%s",
					ANSI_COLOR_DARKGREY(aafi->log),
					"MUTE",
					ANSI_COLOR_RESET(aafi->log) );
			}

			if ( audioTrack->gain ) {
				log( aafi->log, "  Gain: %s%s%s",
					ANSI_COLOR_DARKGREY(aafi->log),
					gainToStr( audioTrack->gain ),
					ANSI_COLOR_RESET(aafi->log) );
			}

			if ( audioTrack->pan ) {
				log( aafi->log, "  Pan: %s%s%s",
					ANSI_COLOR_DARKGREY(aafi->log),
					panToStr( audioTrack->pan ),
					ANSI_COLOR_RESET(aafi->log) );
			}

			if ( audioTrack->name && audioTrack->name[0] != 0x00 ) {
				log( aafi->log, "  Name: %s\"%s\"%s",
					ANSI_COLOR_DARKGREY(aafi->log),
					audioTrack->name,
					ANSI_COLOR_RESET(aafi->log) );
			}

			log( aafi->log, "\n" );

			AAFI_foreachTrackItem( audioTrack, audioItem ) {

				aafiTransition *crossFade = aafi_timelineItemToCrossFade( audioItem );
				aafiAudioClip  *audioClip = aafi_timelineItemToAudioClip( audioItem );

				if ( crossFade ) {

					char posFormatBuf1[POS_FORMAT_BUFFER_LEN];
					char posFormatBuf2[POS_FORMAT_BUFFER_LEN];

					if ( !(crossFade->flags & AAFI_TRANS_XFADE) )
						continue;

					log( aafi->log, " %s X-FADE: %s%s%s  Length: %s%s%s  CutPoint: %s%s%s\n",
						( trackLineItemCounter <= totalTrackLineItems ) ? TREE_ENTRY : TREE_LAST_ENTRY,
						ANSI_COLOR_DARKGREY(aafi->log),
						INTERPOL_TO_STRING( crossFade ),
						ANSI_COLOR_RESET(aafi->log),

						ANSI_COLOR_DARKGREY(aafi->log),
						formatPosValue( crossFade->len,    audioTrack->edit_rate, posFormat, tcFormat, &displaySamplerate, posFormatBuf1 ),
						ANSI_COLOR_RESET(aafi->log),

						ANSI_COLOR_DARKGREY(aafi->log),
						formatPosValue( crossFade->cut_pt, audioTrack->edit_rate, posFormat, tcFormat, &displaySamplerate, posFormatBuf2 ),
						ANSI_COLOR_RESET(aafi->log) );

					log( aafi->log, " %s\n", ( trackLineItemCounter <= totalTrackLineItems ) ? TREE_LINE : " " );
					continue;
				}
				else if ( audioClip ) {

					aafiTransition *fadein  = aafi_getFadeIn( audioClip );
					aafiTransition *fadeout = aafi_getFadeOut( audioClip );

					char posFormatBuf1[POS_FORMAT_BUFFER_LEN];
					char posFormatBuf2[POS_FORMAT_BUFFER_LEN];
					char posFormatBuf3[POS_FORMAT_BUFFER_LEN];
					char posFormatBuf4[POS_FORMAT_BUFFER_LEN];
					char posFormatBuf5[POS_FORMAT_BUFFER_LEN];
					char posFormatBuf6[POS_FORMAT_BUFFER_LEN];

					log( aafi->log, " %s Clip (%u):  Start: %s%s%s  Len: %s%s%s  End: %s%s%s  SourceOffset: %s%s%s  Channels: %s%i%s",
						( trackLineItemCounter < totalTrackLineItems ) ? TREE_ENTRY : TREE_LAST_ENTRY,
						aafi_get_clipIndex(audioClip),
						ANSI_COLOR_DARKGREY(aafi->log),
						formatPosValue( (audioClip->pos + sessionStart),                  audioClip->track->edit_rate, posFormat, tcFormat, &displaySamplerate, posFormatBuf1 ),
						ANSI_COLOR_RESET(aafi->log),

						ANSI_COLOR_DARKGREY(aafi->log),
						formatPosValue( (audioClip->len),                                 audioClip->track->edit_rate, posFormat, tcFormat, &displaySamplerate, posFormatBuf2 ),
						ANSI_COLOR_RESET(aafi->log),

						ANSI_COLOR_DARKGREY(aafi->log),
						formatPosValue( (audioClip->pos + sessionStart + audioClip->len), audioClip->track->edit_rate, posFormat, tcFormat, &displaySamplerate, posFormatBuf3 ),
						ANSI_COLOR_RESET(aafi->log),

						ANSI_COLOR_DARKGREY(aafi->log),
						formatPosValue( audioClip->essence_offset,                        audioClip->track->edit_rate, posFormat, tcFormat, &displaySamplerate, posFormatBuf4 ),
						ANSI_COLOR_RESET(aafi->log),

						ANSI_COLOR_DARKGREY(aafi->log),
						audioClip->channels,
						ANSI_COLOR_RESET(aafi->log) );

					if ( audioClip->mute ) {
						log( aafi->log, "  Mute: %s%s%s",
							ANSI_COLOR_DARKGREY(aafi->log),
							"MUTE",
							ANSI_COLOR_RESET(aafi->log) );
					}

					if ( audioClip->gain ) {
						log( aafi->log, "  Gain: %s%s%s",
							ANSI_COLOR_DARKGREY(aafi->log),
							gainToStr( audioClip->gain ),
							ANSI_COLOR_RESET(aafi->log) );
					}

					if ( audioClip->automation ) {
						log( aafi->log, "  VolumeAutomation: %s%s%s",
							ANSI_COLOR_DARKGREY(aafi->log),
							"YES",
							ANSI_COLOR_RESET(aafi->log) );
					}

					if ( fadein ) {
						log( aafi->log, "  FadeIn: %s%s (%s)%s",
						ANSI_COLOR_DARKGREY(aafi->log),
							INTERPOL_TO_STRING( fadein ),
							formatPosValue( fadein->len, audioClip->track->edit_rate, posFormat, tcFormat, &displaySamplerate, posFormatBuf5 ),
							ANSI_COLOR_RESET(aafi->log) );
					}

					if ( fadeout ) {
						log( aafi->log, "  FadeOut: %s%s (%s)%s",
							ANSI_COLOR_DARKGREY(aafi->log),
							INTERPOL_TO_STRING( fadeout ),
							formatPosValue( fadeout->len, audioClip->track->edit_rate, posFormat, tcFormat, &displaySamplerate, posFormatBuf6 ),
							ANSI_COLOR_RESET(aafi->log) );
					}

					if ( audioClip->subClipName ) {
						log( aafi->log, "  ClipName: %s\"%s\"%s",
							ANSI_COLOR_DARKGREY(aafi->log),
							audioClip->subClipName,
							ANSI_COLOR_RESET(aafi->log) );
					}

					log( aafi->log, "\n" );


					int clipLineItemCounter = 1;
					int totalClipLineItems  = 0;
					totalClipLineItems += ( show_metadata && audioClip->metadata ) ? 1 : 0;
					totalClipLineItems += ( show_automation && audioClip->automation ) ? 1 : 0;

					aafiAudioEssencePointer *audioEssencePtr = NULL;

					AAFI_foreachClipEssencePointer( audioClip, audioEssencePtr ) {
						totalClipLineItems++;
					}


					AAFI_foreachClipEssencePointer( audioClip, audioEssencePtr ) {
						if ( audioEssencePtr->essenceChannel ) {
							log( aafi->log, " %s   %s SourceFile [ch %i]: %s\"%s\"%s\n",
								( trackLineItemCounter < totalTrackLineItems ) ? TREE_LINE : " ",
								( clipLineItemCounter  < totalClipLineItems  ) ? TREE_ENTRY : TREE_LAST_ENTRY,
								audioEssencePtr->essenceChannel,
								ANSI_COLOR_DARKGREY(aafi->log),
								audioEssencePtr->essenceFile->unique_name,
								ANSI_COLOR_RESET(aafi->log) );
						}
						else {
							log( aafi->log, " %s   %s SourceFile [ch ALL]: %s\"%s\"%s\n",
								( trackLineItemCounter < totalTrackLineItems ) ? TREE_LINE : " ",
								( clipLineItemCounter  < totalClipLineItems  ) ? TREE_ENTRY : TREE_LAST_ENTRY,
								ANSI_COLOR_DARKGREY(aafi->log),
								audioEssencePtr->essenceFile->unique_name,
								ANSI_COLOR_RESET(aafi->log) );
						}
						clipLineItemCounter++;
					}


					if ( show_metadata && audioClip->metadata ) {

						log( aafi->log, " %s   %s Metadata %s\n",
							( trackLineItemCounter < totalTrackLineItems ) ? TREE_LINE : " ",
							( clipLineItemCounter  < totalClipLineItems  ) ? TREE_ENTRY : TREE_LAST_ENTRY,
							"\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80" );

						char padding[16];
						snprintf( padding, sizeof(padding), " %s   %s   ",
							( trackLineItemCounter < totalTrackLineItems ) ? TREE_LINE : " ",
							( clipLineItemCounter  < totalClipLineItems  ) ? TREE_LINE : " " );


						size_t longestName = getLongestMetadataName( audioClip->metadata ) + 1;

						aafiMetaData *metadata = NULL;

						AAFI_foreachMetadata( audioClip->metadata, metadata ) {
							log( aafi->log, "%s- Name: %s\"%s\"%s %*s Text: %s\"%s\"%s\n",
								padding,
								ANSI_COLOR_DARKGREY(aafi->log),
								(metadata->name) ? metadata->name : "<unknown>",
								ANSI_COLOR_RESET(aafi->log),

								(metadata->name) ? (int)(longestName-strlen(metadata->name)) : (int)(longestName-strlen("<unknown>")), " ",

								ANSI_COLOR_DARKGREY(aafi->log),
								metadata->text,
								ANSI_COLOR_RESET(aafi->log) );
						}

						clipLineItemCounter++;
					}

					if ( show_automation && audioClip->automation ) {

						log( aafi->log, " %s   %s Clip Volume Automation %s\n",
							( trackLineItemCounter < totalTrackLineItems ) ? TREE_LINE : " ",
							( clipLineItemCounter  < totalClipLineItems  ) ? TREE_ENTRY : TREE_LAST_ENTRY,
							"\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80" );

						char padding[16];
						snprintf( padding, sizeof(padding), " %s   %s   ",
							( trackLineItemCounter < totalTrackLineItems ) ? TREE_LINE : " ",
							( clipLineItemCounter  < totalClipLineItems  ) ? TREE_LINE : " " );

						dumpVaryingValues( aafi, audioClip->automation, 1, padding );
						clipLineItemCounter++;
					}

					log( aafi->log, " %s\n",
						( trackLineItemCounter < totalTrackLineItems ) ? TREE_LINE : " " );

					trackLineItemCounter++;
				}
			}


			if ( show_automation && audioTrack->pan != NULL && audioTrack->pan->flags & AAFI_AUDIO_GAIN_VARIABLE ) {

				log( aafi->log, " %s Track Pan Automation %s\n",
					TREE_LAST_ENTRY,
					"\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80" );

				const char *padding = "     ";
				if ( trackLineItemCounter < totalTrackLineItems ) {
					padding = TREE_PADDED_LINE;
				}

				dumpVaryingValues( aafi, audioTrack->pan, 0, padding );
				trackLineItemCounter++;
			}


			if ( show_automation && audioTrack->gain != NULL && audioTrack->gain->flags & AAFI_AUDIO_GAIN_VARIABLE ) {

				log( aafi->log, " %s Track Volume Automation %s\n",
					TREE_LAST_ENTRY,
					"\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80" );

				const char *padding = "     ";
				if ( trackLineItemCounter < totalTrackLineItems ) {
					padding = TREE_PADDED_LINE;
				}

				dumpVaryingValues( aafi, audioTrack->gain, 0, padding );
				trackLineItemCounter++;
			}

			log( aafi->log, "\n" );
		}



		if ( aafi->Markers ) {
			log( aafi->log, "Markers :\n"
			                "=========\n\n" );
		}

		uint32_t markerCount = 0;
		aafiMarker *marker = NULL;

		AAFI_foreachMarker( aafi, marker ) {

			/*
			 * Composition Timecode does not always share the same edit rate as markers.
			 * Therefore, we need to do the conversion prior to any maths.
			 * For example, if TC is 30000/1001 and markers are 48000/1, then TC->start has to be converted from FPS to samples.
			 */

			sessionStart = aafi_convertUnit( aafi->compositionStart, aafi->compositionStart_editRate, marker->edit_rate );

			char posFormatBuf1[POS_FORMAT_BUFFER_LEN];
			char posFormatBuf2[POS_FORMAT_BUFFER_LEN];

			log( aafi->log, " Marker[%i]:  EditRate: %s%i/%i (%s)%s  Start: %s%s%s  Length: %s%s%s  Color: %s#%02x%02x%02x%s  Label: %s\"%s\"%s  Comment: %s\"%s\"%s\n",
				markerCount++,

				ANSI_COLOR_DARKGREY(aafi->log),
				marker->edit_rate->numerator,
				marker->edit_rate->denominator,
				float2str( f2sbuf, "%02.2f", aafRationalToDouble(*marker->edit_rate) ),
				ANSI_COLOR_RESET(aafi->log),

				ANSI_COLOR_DARKGREY(aafi->log),
				formatPosValue( (marker->start + sessionStart), marker->edit_rate, posFormat, tcFormat, &displaySamplerate, posFormatBuf1 ),
				ANSI_COLOR_RESET(aafi->log),

				ANSI_COLOR_DARKGREY(aafi->log),
				formatPosValue(  marker->length, marker->edit_rate, posFormat, tcFormat, &displaySamplerate, posFormatBuf2 ),
				ANSI_COLOR_RESET(aafi->log),

				ANSI_COLOR_DARKGREY(aafi->log),
				marker->RGBColor[0],
				marker->RGBColor[1],
				marker->RGBColor[2],
				ANSI_COLOR_RESET(aafi->log),

				ANSI_COLOR_DARKGREY(aafi->log),
				marker->name,
				ANSI_COLOR_RESET(aafi->log),

				ANSI_COLOR_DARKGREY(aafi->log),
				marker->comment,
				ANSI_COLOR_RESET(aafi->log) );
		}
	}


	if ( extract_essences ) {

		int hasEmbeddedEssences = 0;
		aafiAudioEssenceFile *audioEssenceFile = NULL;

		AAFI_foreachAudioEssenceFile( aafi, audioEssenceFile ) {

			if ( audioEssenceFile->is_embedded ) {

#ifdef HAVE_SNDFILE
				if ( extract_format != AAFI_EXTRACT_ORIGINAL &&
				     extract_format != AAFI_EXTRACT_ORIGINAL_PCM )
				{
					if ( aafi_extract_audioEssenceFile( aafi, audioEssenceFile, extract_path, NULL, 0, 0, extract_format, extract_samplerate, extract_samplesize, NULL ) == 0 ) {
						log( aafi->log, "[%ssuccess%s] Audio essence file extracted to %s\"%s\"%s\n", ANSI_COLOR_GREEN(aafi->log), ANSI_COLOR_RESET(aafi->log), ANSI_COLOR_DARKGREY(aafi->log), audioEssenceFile->usable_file_path, ANSI_COLOR_RESET(aafi->log) );
					} else {
						log( aafi->log, "[%s error %s] Audio essence file extraction failed : %s\"%s\"%s\n", ANSI_COLOR_RED(aafi->log), ANSI_COLOR_RESET(aafi->log), ANSI_COLOR_DARKGREY(aafi->log), audioEssenceFile->unique_name, ANSI_COLOR_RESET(aafi->log) );
					}
				}
				else {
#endif
					if ( aafi_extract_original_audioEssenceFile( aafi, audioEssenceFile, extract_path, NULL, (extract_format == AAFI_EXTRACT_ORIGINAL_PCM) ) == 0 ) {
						log( aafi->log, "[%ssuccess%s] Audio essence file extracted to %s\"%s\"%s\n", ANSI_COLOR_GREEN(aafi->log), ANSI_COLOR_RESET(aafi->log), ANSI_COLOR_DARKGREY(aafi->log), audioEssenceFile->usable_file_path, ANSI_COLOR_RESET(aafi->log) );
					} else {
						log( aafi->log, "[%s error %s] Audio essence file extraction failed : %s\"%s\"%s\n", ANSI_COLOR_RED(aafi->log), ANSI_COLOR_RESET(aafi->log), ANSI_COLOR_DARKGREY(aafi->log), audioEssenceFile->unique_name, ANSI_COLOR_RESET(aafi->log) );
					}
#ifdef HAVE_SNDFILE
				}
#endif
				hasEmbeddedEssences = 1;
			}
		}

		if ( !hasEmbeddedEssences ) {
			log( aafi->log, "[%s error %s] File has no embedded essence to extract.\n", ANSI_COLOR_RED(aafi->log), ANSI_COLOR_RESET(aafi->log) );
		}
	}

#ifdef HAVE_SNDFILE
	if ( extract_clips || extract_mono_clips ) {

		aafiAudioTrack   *audioTrack = NULL;
		aafiTimelineItem *audioItem  = NULL;
		aafiAudioClip    *audioClip  = NULL;

		AAFI_foreachAudioTrack( aafi, audioTrack ) {

			AAFI_foreachTrackItem( audioTrack, audioItem ) {

				if ( audioItem->type != AAFI_AUDIO_CLIP ) {
					continue;
				}

				audioClip = audioItem->data;

				char *usable_file_path = NULL;

				if ( !extract_mono_clips ) {

					if ( aafi_extract_audioClip( aafi, audioClip, extract_path, extract_format, extract_samplerate, extract_samplesize, &usable_file_path ) == 0 ) {
						log( aafi->log, "[%ssuccess%s] Audio clip file extracted to %s\"%s\"%s\n", ANSI_COLOR_GREEN(aafi->log), ANSI_COLOR_RESET(aafi->log), ANSI_COLOR_DARKGREY(aafi->log), usable_file_path, ANSI_COLOR_RESET(aafi->log) );
					}
					else {
						log( aafi->log, "[%s error %s] Audio clip file extraction failed : %s\"%s\"%s\n", ANSI_COLOR_RED(aafi->log), ANSI_COLOR_RESET(aafi->log), ANSI_COLOR_DARKGREY(aafi->log), audioClip->essencePointerList->essenceFile->name, ANSI_COLOR_RESET(aafi->log) );
					}

					free(usable_file_path);
				}
				else {

					aafiAudioEssencePointer *audioEssencePtr = NULL;

					AAFI_foreachClipEssencePointer( audioClip, audioEssencePtr ) {

						aafiAudioEssenceFile *audioEssenceFile = audioEssencePtr->essenceFile;

						if ( !audioEssenceFile->is_embedded ) {
							log( aafi->log, "Audio essence \"%s\" is not embedded : nothing to extract", audioEssenceFile->unique_name );
							continue;
						}

						uint64_t frameOffset = aafi_convertUnitUint64( audioClip->essence_offset, audioClip->track->edit_rate, audioEssenceFile->samplerateRational );
						uint64_t frameLength = aafi_convertUnitUint64( audioClip->len,            audioClip->track->edit_rate, audioEssenceFile->samplerateRational );

						char *name = NULL;

						laaf_util_snprintf_realloc( &name, NULL, 0, "%i_%i_%s", audioClip->track->number, aafi_get_clipIndex(audioClip), audioEssenceFile->unique_name );

						if ( aafi_extract_audioEssenceFile( aafi, audioEssenceFile, extract_path, name, frameOffset, frameLength, extract_format, extract_samplerate, extract_samplesize, &usable_file_path ) == 0 ) {
							log( aafi->log, "[%ssuccess%s] Audio clip file extracted to %s\"%s\"%s\n", ANSI_COLOR_GREEN(aafi->log), ANSI_COLOR_RESET(aafi->log), ANSI_COLOR_DARKGREY(aafi->log), usable_file_path, ANSI_COLOR_RESET(aafi->log) );
						}
						else {
							log( aafi->log, "[%s error %s] Audio clip file extraction failed : %s\"%s\"%s\n", ANSI_COLOR_RED(aafi->log), ANSI_COLOR_RESET(aafi->log), ANSI_COLOR_DARKGREY(aafi->log), name, ANSI_COLOR_RESET(aafi->log) );
						}

						free( usable_file_path );
						free( name );
					}
				}
			}
		}
	}
#endif

	goto end;

err:
	rc = -1;

end:

	free( aafPath );

	free( get_node_str );

	aafi_release( &aafi );

	if ( logfp != stdout ) {
		fclose( logfp );
	}

	return rc;
}

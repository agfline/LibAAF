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
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>
#include <assert.h>
// #include <sys/param.h> // MAX()

#include <libaaf/AAFIface.h>
#include <libaaf/AAFIEssenceFile.h>
#include <libaaf/version.h>
#include <libaaf/log.h>

#include <libaaf/utils.h>
#include <libaaf/MediaComposer.h>

#include "RIFFParser.h"
#include "URIParser.h"


#define debug( ... ) \
	AAF_LOG( aafi->log, aafi, LOG_SRC_ID_AAF_IFACE, VERB_DEBUG, __VA_ARGS__ )

#define success( ... ) \
	AAF_LOG( aafi->log, aafi, LOG_SRC_ID_AAF_IFACE, VERB_SUCCESS, __VA_ARGS__ )

#define warning( ... ) \
	AAF_LOG( aafi->log, aafi, LOG_SRC_ID_AAF_IFACE, VERB_WARNING, __VA_ARGS__ )

#define error( ... ) \
	AAF_LOG( aafi->log, aafi, LOG_SRC_ID_AAF_IFACE, VERB_ERROR, __VA_ARGS__ )



#ifdef HAVE_SNDFILE
static const char * sfaaf_endian_name( int format );
static const char * sfaaf_container_name( int format );
static const char * sfaaf_codec_name( int format );
static int sfaaf_set_bext( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile, SNDFILE *sfile );
static SNDFILE * sfaaf_open_outputFile( AAF_Iface *aafi, SF_INFO *sfinfo, const char *filepath, int format, uint32_t channels, int samplerate, int samplesize );
static int sfaaf_extract_audio( AAF_Iface *aafi, uint32_t infilecount, SNDFILE *infiles[], SNDFILE *outfile, sf_count_t frameOffset, sf_count_t frameLength, int processFloatingPoint );
#endif

static int set_audioEssenceWithRIFF( AAF_Iface *aafi, const char *filename, aafiAudioEssenceFile *audioEssenceFile, struct RIFFAudioFile *RIFFAudioFile, int isExternalFile );
static size_t embeddedAudioDataReaderCallback( unsigned char *buf, size_t offset, size_t reqLen, void *user1, void *user2, void *user3 );
static size_t externalAudioDataReaderCallback( unsigned char *buf, size_t offset, size_t reqLen, void *user1, void *user2, void *user3 );



size_t aafi_embeddedAudioEssenceFile_size( aafiAudioEssenceFile *audioEssenceFile )
{
	return cfb_stream_size( audioEssenceFile->sd );
}



size_t aafi_embeddedAudioEssenceFile_tell( aafiAudioEssenceFile *audioEssenceFile )
{
	return cfb_stream_tell( audioEssenceFile->sd );
}



ssize_t aafi_embeddedAudioEssenceFile_seek( aafiAudioEssenceFile *audioEssenceFile, int whence, int64_t pos )
{
	return cfb_stream_seek( audioEssenceFile->sd, whence, pos );
}



ssize_t aafi_embeddedAudioEssenceFile_read( aafiAudioEssenceFile *audioEssenceFile, void* buf, size_t nbyte )
{
	return cfb_stream_read( audioEssenceFile->sd, buf, nbyte );
}



int aafi_build_unique_audio_essence_name( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile )
{
	if ( audioEssenceFile->unique_name ) {
		debug( "Unique name was already set" );
		return -1;
	}

	if ( aafi->ctx.options.mobid_essence_filename ) {

			aafUID_t *uuid = &(audioEssenceFile->sourceMobID->material);

			int rc = laaf_util_snprintf_realloc( &audioEssenceFile->unique_name, 0, 0, "%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
				uuid->Data1,
				uuid->Data2,
				uuid->Data3,
				uuid->Data4[0],
				uuid->Data4[1],
				uuid->Data4[2],
				uuid->Data4[3],
				uuid->Data4[4],
				uuid->Data4[5],
				uuid->Data4[6],
				uuid->Data4[7] );

			if ( rc < 0 ) {
				error( "Failed to set unique filename with SourceMobID UID" );
				free( audioEssenceFile->unique_name );
				return -1;
			}

			return 0;
	}
	else {
		audioEssenceFile->unique_name = laaf_util_c99strdup( (audioEssenceFile->name) ? audioEssenceFile->name : "unknown" );

		if ( !audioEssenceFile->unique_name ) {
			error( "Could not duplicate essence name : %s", (audioEssenceFile->name) ? audioEssenceFile->name : "unknown" );
			return -1;
		}
	}

	size_t unique_size = strlen( audioEssenceFile->unique_name ) + 1;

	int index = 0;
	aafiAudioEssenceFile *ae = NULL;

	AAFI_foreachAudioEssenceFile( aafi, ae ) {

		if ( ae != audioEssenceFile && ae->unique_name != NULL && strcmp( ae->unique_name, audioEssenceFile->unique_name ) == 0 ) {

			if ( laaf_util_snprintf_realloc( &audioEssenceFile->unique_name, &unique_size, 0, "%s_%i", (audioEssenceFile->name) ? audioEssenceFile->name : "unknown", ++index ) < 0 ) {
				error( "Failed to increment unique filename" );
				return -1;
			}

			/* recheck entire essence list */
			ae = aafi->Audio->essenceFiles;
		}
	}

	return 0;
}



char * aafi_locate_external_essence_file( AAF_Iface *aafi, const char *original_uri_filepath, const char *search_location )
{
	/*
	 * Absolute Uniform Resource Locator (URL) complying with RFC 1738 or relative
	 * Uniform Resource Identifier (URI) complying with RFC 2396 for file containing
	 * the essence. If it is a relative URI, the base URI is determined from the URI
	 * of the AAF file itself.
	 *
	 * Informative note: A valid URL or URI uses a constrained character set and
	 * uses the / character as the path separator.
	 */

	char *local_filepath = NULL;
	char *aaf_path = NULL;
	const char *foundpath = NULL;
	char *retpath = NULL;

	struct uri *uri = NULL;

	if ( original_uri_filepath == NULL ) {
		error( "Cant locate a NULL filepath" );
		goto err;
	}

	debug( "Original URI : %s", original_uri_filepath );


	uri = laaf_uri_parse( original_uri_filepath, URI_OPT_DECODE_ALL, aafi->log );

	if ( uri == NULL ) {
		error( "Could not parse URI" );
		goto err;
	}

	if ( uri->path == NULL ) {
		error( "Could not retrieve <path> out of URI" );
		goto err;
	}

	debug( "Decoded URI's path : %s", uri->path );


	/* extract relative path to essence file : "<firstparent>/<essence.file>" */

	const char *relativeEssencePath = NULL;
	const char *essenceFileName = NULL;

	int sepcount = 0;
	char *p = uri->path + strlen(uri->path);

	while ( p > uri->path ) {
		if ( *p == '/' ) { /* parsing URI, so will always be '/' as separator character */
			sepcount++;
			if ( sepcount == 1 ) {
				essenceFileName = (p+1);
			}
			else
			if ( sepcount == 2 ) {
				relativeEssencePath = (p+1);
				break;
			}
		}
		p--;
	}

	if ( !relativeEssencePath ) {
		error( "Could not retrieve relative file path out of URI : %s", uri->path );
		goto err;
	}

	if ( !essenceFileName ) {
		error( "Could not retrieve file name out of URI : %s", uri->path );
		goto err;
	}

	debug( "Essence filename : %s", essenceFileName );


	if ( search_location ) {

		/*
		 * "<search_location>/<essence.file>"
		 */

		local_filepath = laaf_util_build_path( "/", search_location, essenceFileName, NULL );

		if ( !local_filepath ) {
			error( "Could not build search filepath" );
			goto err;
		}

		debug( "Search filepath : %s", local_filepath );


		if ( laaf_util_file_exists( local_filepath ) == 1 ) {
			foundpath = local_filepath;
			goto found;
		}

		free( local_filepath ); local_filepath = NULL;


		/*
		 * "<search_location>/<firstparentInOriginalEssencePath>/<essence.file>"
		 */

		local_filepath = laaf_util_build_path( "/", search_location, relativeEssencePath, NULL );

		if ( !local_filepath ) {
			error( "Could not build search filepath" );
			goto err;
		}

		debug( "Search filepath : %s", local_filepath );

		if ( laaf_util_file_exists( local_filepath ) == 1 ) {
			foundpath = local_filepath;
			goto found;
		}

		free( local_filepath ); local_filepath = NULL;
	}


	/* Try raw essence's URI, just in case... */

	if ( laaf_util_file_exists( original_uri_filepath ) == 1 ) {
		foundpath = original_uri_filepath;
		goto found;
	}


	/* Try <path> part of URI */

	if ( laaf_util_file_exists( uri->path ) == 1 ) {
		foundpath = uri->path;
		goto found;
	}


	/*
	 * Try to locate essence file from the AAF file location.
	 *
	 * e.g.
	 *      AAF filepath : /home/user/AAFFile.aaf
	 *    + Essence URI  : file://localhost/C:/Users/user/Desktop/AudioFiles/essence.wav
	 *    = /home/user/AudioFiles/essence.file
	 */


	/* extract path to AAF file */

	aaf_path = laaf_util_c99strdup( aafi->aafd->cfbd->file );

	if ( !aaf_path ) {
		error( "Could not duplicate AAF filepath : %s", aafi->aafd->cfbd->file );
		goto err;
	}

	p = aaf_path + strlen(aaf_path);

	while ( p > aaf_path ) {
		if ( IS_DIR_SEP(*p) ) {
			*p = 0x00;
			break;
		}
		p--;
	}


	/*
	 * "<localPathToAAFfile>/<essence.file>"
	 */

	local_filepath = laaf_util_build_path( DIR_SEP_STR, aaf_path, essenceFileName, NULL );

	if ( !local_filepath ) {
		error( "Could not build filepath" );
		goto err;
	}

	debug( "AAF relative filepath : %s", local_filepath );

	if ( laaf_util_file_exists( local_filepath ) == 1 ) {
		foundpath = local_filepath;
		goto found;
	}

	free( local_filepath ); local_filepath = NULL;


	/*
	 * "<localPathToAAFfile>/<firstparentInOriginalEssencePath>/<essence.file>"
	 */

	local_filepath = laaf_util_build_path( DIR_SEP_STR, aaf_path, relativeEssencePath, NULL );

	if ( !local_filepath ) {
		error( "Could not build filepath" );
		goto err;
	}

	debug( "AAF relative sub filepath : %s", local_filepath );

	if ( laaf_util_file_exists( local_filepath ) == 1 ) {
		foundpath = local_filepath;
		goto found;
	}

	free( local_filepath ); local_filepath = NULL;


	debug( "File not found" );

found:
	if ( foundpath ) {
		/*
		 * When runing through wine, computing absolute path adds a Z:/ drive letter.
		 * This causes issue when trying to make relative essence path from the AAF
		 * file path, since it also went through laaf_util_absolute_path().
		 * So even if foundpath is already absolute, we need that drive letter at it
		 * start.
		 */
		retpath = laaf_util_absolute_path(foundpath);

		if ( !retpath ) {
			error( "Could not make absolute path to located file : %s", foundpath );
			goto err;
		}

		debug( "File found at : %s", foundpath );
	}

	goto end;

err:
	retpath = NULL;

end:
	laaf_uri_free( uri );

	free( local_filepath );
	free( aaf_path );

	return retpath;
}



#ifdef HAVE_SNDFILE

int aafi_extract_audioEssenceFile( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile, const char *outpath, const char *rename, uint64_t frameOffset, uint64_t frameLength, enum aafiExtractFormat format, int samplerate, int samplesize, char **usable_file_path )
{
	int rc = 0;

	char *filepath = NULL;
	void *buf = NULL;

	SF_INFO sfinfo;

	SNDFILE *infiles[1] = { NULL };
	SNDFILE *outfile = NULL;

	if ( !audioEssenceFile->is_embedded ) {
		error( "Audio essence \"%s\" is not embedded : nothing to extract", audioEssenceFile->unique_name );
		goto err;
	}

	if ( !outpath ) {
		error( "Missing output path" );
		goto err;
	}

	if ( audioEssenceFile->usable_file_path ) {
		debug( "usable_file_path was already set" );
		free( audioEssenceFile->usable_file_path );
		audioEssenceFile->usable_file_path = NULL;
	}

	debug( "request audio essence file extraction of \"%s\"", audioEssenceFile->name );
	debug( "  destination:  \"%s\"", outpath );
	debug( "  rename:       %s%s%s", (rename) ? "\"" : "", (rename) ? rename : "NULL", (rename) ? "\"" : "" );
	debug( "  frame offset: %"PRIu64, frameOffset );
	debug( "  frame length: %"PRIu64, frameLength );
	debug( "  format:       %s", aafi_extractFormatToText(format) );
	debug( "  sample rate:  %i", samplerate );
	debug( "  sample size:  %i", samplesize );


	filepath = aafi_build_audioEssenceFilePath( aafi, audioEssenceFile, outpath, rename, format, 0 );

	if ( !filepath ) {
		error( "Could not build file path" );
		goto err;
	}


	infiles[0] = aafi_sf_open_virtual_audioEssenceFile( aafi, audioEssenceFile, &sfinfo );

	if ( !infiles[0] ) {
		error( "Could not open audio essence file virtual input stream" );
		goto err;
	}

	int informat = sfinfo.format & SF_FORMAT_SUBMASK;


	outfile = sfaaf_open_outputFile( aafi, &sfinfo, filepath, format, 1, samplerate, samplesize );

	if ( !outfile ) {
		error( "Could not open output file : %s", filepath );
		goto err;
	}

	int outformat = sfinfo.format & SF_FORMAT_SUBMASK;


	if ( format == AAFI_EXTRACT_BWAV ) {
		sfaaf_set_bext( aafi, audioEssenceFile, outfile );
	}


	int processFloatingPoint = 0;

	if ( informat == SF_FORMAT_FLOAT ||
	     informat == SF_FORMAT_DOUBLE ||
	     outformat == SF_FORMAT_FLOAT ||
	     outformat == SF_FORMAT_DOUBLE )
	{
		processFloatingPoint = 1;
	}

	assert( frameOffset < SF_COUNT_MAX );
	assert( frameLength < SF_COUNT_MAX );

	rc = sfaaf_extract_audio( aafi, 1, infiles, outfile, (sf_count_t)frameOffset, (sf_count_t)frameLength, processFloatingPoint );

	if ( rc < 0 ) {
		error( "Could not extract audio to file \"%s\"", filepath );
		goto err;
	}

	audioEssenceFile->usable_file_path = laaf_util_c99strdup( filepath );

	if ( !audioEssenceFile->usable_file_path ) {
		error( "Could not duplicate usable filepath : %s", filepath );
		goto err;
	}


	if ( usable_file_path ) {
		*usable_file_path = laaf_util_c99strdup( filepath );

		if ( !(*usable_file_path) ) {
			error( "Could not duplicate usable filepath : %s", filepath );
			goto err;
		}
	}

	rc = 0;
	goto end;

err:
	rc = -1;

end:

	if ( infiles[0] )
		sf_close( infiles[0] );

	if ( outfile )
		sf_close( outfile );

	free( filepath );
	free( buf );

	return rc;
}



sf_count_t aafi_sf_embeddedAudioEssenceFile_size( void *audioEssenceFile )
{
	size_t val = cfb_stream_size( ((aafiAudioEssenceFile*)(audioEssenceFile))->sd );

	assert( val < SF_COUNT_MAX );

	return (sf_count_t)val;
}



sf_count_t aafi_sf_embeddedAudioEssenceFile_tell( void *audioEssenceFile )
{
	size_t val = cfb_stream_tell( ((aafiAudioEssenceFile*)(audioEssenceFile))->sd );

	assert( val < SF_COUNT_MAX );

	return (sf_count_t)val;
}



sf_count_t aafi_sf_embeddedAudioEssenceFile_seek( sf_count_t pos, int whence, void *audioEssenceFile )
{
	ssize_t val = cfb_stream_seek( ((aafiAudioEssenceFile*)(audioEssenceFile))->sd, whence, pos );

	assert( val < SF_COUNT_MAX );

	return (sf_count_t)val;
}



sf_count_t aafi_sf_embeddedAudioEssenceFile_read( void* buf, sf_count_t nbyte, void *audioEssenceFile )
{
	if ( nbyte < 0 ) {
		return -1;
	}

	ssize_t val = cfb_stream_read( ((aafiAudioEssenceFile*)(audioEssenceFile))->sd, buf, (size_t)nbyte );

	assert( val < SF_COUNT_MAX );

	return (sf_count_t)val;
}

#endif



#ifdef HAVE_SNDFILE

static const char * sfaaf_endian_name( int format )
{
	switch ( format & SF_FORMAT_ENDMASK )
	{
		case SF_ENDIAN_FILE:   return "file endianness";
		case SF_ENDIAN_LITTLE: return "little endian";
		case SF_ENDIAN_BIG:    return "big endian";
		case SF_ENDIAN_CPU:    return "cpu endianness";
		default: break;
	}

	return "unknown endianness";
}

static const char * sfaaf_container_name( int format )
{
	switch ( format & SF_FORMAT_TYPEMASK )
	{
		case SF_FORMAT_WAV:   return "WAV";
		case SF_FORMAT_AIFF:  return "AIFF";
		case SF_FORMAT_AU:    return "AU";
		case SF_FORMAT_RAW:   return "RAW";
		case SF_FORMAT_PAF:   return "PAF";
		case SF_FORMAT_SVX:   return "SVX";
		case SF_FORMAT_NIST:  return "NIST";
		case SF_FORMAT_VOC:   return "VOC";
		case SF_FORMAT_IRCAM: return "IRCAM";
		case SF_FORMAT_W64:   return "W64";
		case SF_FORMAT_MAT4:  return "MAT4";
		case SF_FORMAT_MAT5:  return "MAT5";
		case SF_FORMAT_PVF:   return "PVF";
		case SF_FORMAT_XI:    return "XI";
		case SF_FORMAT_HTK:   return "HTK";
		case SF_FORMAT_SDS:   return "SDS";
		case SF_FORMAT_AVR:   return "AVR";
		case SF_FORMAT_WAVEX: return "WAVEX";
		case SF_FORMAT_SD2:   return "SD2";
		case SF_FORMAT_FLAC:  return "FLAC";
		case SF_FORMAT_CAF:   return "CAF";
		case SF_FORMAT_WVE:   return "WVE";
		case SF_FORMAT_OGG:   return "OGG";
		case SF_FORMAT_MPC2K: return "MPC2K";
		case SF_FORMAT_RF64:  return "RF64";
		case SF_FORMAT_MPEG:  return "MPEG";
		default: break;
	}

	return "unknown format";
}

static const char * sfaaf_codec_name( int format )
{
	switch ( format & SF_FORMAT_SUBMASK )
	{
		case SF_FORMAT_PCM_S8:         return "signed 8 bit PCM";
		case SF_FORMAT_PCM_16:         return "16 bit PCM";
		case SF_FORMAT_PCM_24:         return "24 bit PCM";
		case SF_FORMAT_PCM_32:         return "32 bit PCM";
		case SF_FORMAT_PCM_U8:         return "unsigned 8 bit PCM";
		case SF_FORMAT_FLOAT:          return "32 bit float";
		case SF_FORMAT_DOUBLE:         return "64 bit double";
		case SF_FORMAT_ULAW:           return "u-law";
		case SF_FORMAT_ALAW:           return "a-law";
		case SF_FORMAT_IMA_ADPCM:      return "IMA ADPCM";
		case SF_FORMAT_MS_ADPCM:       return "MS ADPCM";
		case SF_FORMAT_GSM610:         return "gsm610";
		case SF_FORMAT_VOX_ADPCM:      return "Vox ADPCM";
		case SF_FORMAT_G721_32:        return "g721 32kbps";
		case SF_FORMAT_G723_24:        return "g723 24kbps";
		case SF_FORMAT_G723_40:        return "g723 40kbps";
		case SF_FORMAT_DWVW_12:        return "12 bit DWVW";
		case SF_FORMAT_DWVW_16:        return "16 bit DWVW";
		case SF_FORMAT_DWVW_24:        return "14 bit DWVW";
		case SF_FORMAT_DWVW_N:         return "DWVW";
		case SF_FORMAT_DPCM_8:         return "8 bit DPCM";
		case SF_FORMAT_DPCM_16:        return "16 bit DPCM";
		case SF_FORMAT_VORBIS:         return "Vorbis";
		case SF_FORMAT_ALAC_16:        return "16 bit ALAC";
		case SF_FORMAT_ALAC_20:        return "20 bit ALAC";
		case SF_FORMAT_ALAC_24:        return "24 bit ALAC";
		case SF_FORMAT_ALAC_32:        return "32 bit ALAC";
		case SF_FORMAT_OPUS:           return "Opus";
		case SF_FORMAT_MPEG_LAYER_I:   return "MPEG layer 1";
		case SF_FORMAT_MPEG_LAYER_II:  return "MPEG layer 2";
		case SF_FORMAT_MPEG_LAYER_III: return "MPEG layer 3";
		default : break;
	}

	return "unknown codec";
}



static int sfaaf_set_bext( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile, SNDFILE *sfile )
{
	SF_BROADCAST_INFO bext;

	memset( &bext, 0, sizeof(SF_BROADCAST_INFO) );

	bext.coding_history_size = 0;

	uint64_t time_reference = aafi_convertUnitUint64( audioEssenceFile->sourceMobSlotOrigin, audioEssenceFile->sourceMobSlotEditRate, audioEssenceFile->samplerateRational );
	bext.time_reference_high = (uint32_t)(time_reference >> 32);
	bext.time_reference_low  = (uint32_t)(time_reference & 0xffffffff);

	snprintf( bext.description, sizeof(((SF_BROADCAST_INFO*)0)->description), "%s\n%s.aaf", audioEssenceFile->name, aafi->compositionName );
	snprintf( bext.originator, sizeof(((SF_BROADCAST_INFO*)0)->originator), "%s %s", aafi->aafd->Identification.ProductName, (mediaComposer_AAF(aafi)) ? "" : aafi->aafd->Identification.ProductVersionString );
	snprintf( bext.originator_reference, sizeof(((SF_BROADCAST_INFO*)0)->originator_reference), "libAAF %s", LIBAAF_VERSION );

	memcpy( bext.origination_date, audioEssenceFile->originationDate, sizeof(((SF_BROADCAST_INFO*)0)->origination_date) );
	memcpy( bext.origination_time, audioEssenceFile->originationTime, sizeof(((SF_BROADCAST_INFO*)0)->origination_time) );
	memcpy( bext.umid, audioEssenceFile->sourceMobID, sizeof(aafMobID_t) );

	if ( sf_command( sfile, SFC_SET_BROADCAST_INFO, &bext, sizeof(bext) ) == SF_FALSE ) {
		error( "libsndfile could not add bext chunk : %s", sf_strerror(sfile) );
		return -1;
	}

	return 0;
}



SNDFILE * aafi_sf_open_virtual_audioEssenceFile( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile, SF_INFO *sfinfo )
{
	SNDFILE *sndfile = NULL;
	SF_VIRTUAL_IO sfvirtual;

	sfvirtual.get_filelen = aafi_sf_embeddedAudioEssenceFile_size;
	sfvirtual.seek = aafi_sf_embeddedAudioEssenceFile_seek;
	sfvirtual.read = aafi_sf_embeddedAudioEssenceFile_read;
	sfvirtual.tell = aafi_sf_embeddedAudioEssenceFile_tell;
	sfvirtual.write = NULL;

	memset( sfinfo, 0x00, sizeof(SF_INFO) );

	if ( audioEssenceFile->type == AAFI_ESSENCE_TYPE_PCM ) {

		sfinfo->format = SF_FORMAT_RAW;

		switch ( audioEssenceFile->samplesize ) {
			case  8:  sfinfo->format |= SF_FORMAT_PCM_S8;  break;
			case 16:  sfinfo->format |= SF_FORMAT_PCM_16;  break;
			case 24:  sfinfo->format |= SF_FORMAT_PCM_24;  break;
			case 32:  sfinfo->format |= SF_FORMAT_FLOAT;   break;
			default:
				error( "Could not retrieve SF_FORMAT out of sample size %i", audioEssenceFile->samplesize );
				return NULL;
		}

		if ( audioEssenceFile->samplerate >= INT_MAX ) {
			error( "audioEssenceFile sample rate is bigger than INT_MAX" );
			return NULL;
		}

		sfinfo->samplerate = (int)audioEssenceFile->samplerate;

		sfinfo->channels = audioEssenceFile->channels;
	}

	if ( aafi_embeddedAudioEssenceFile_seek( audioEssenceFile, SEEK_SET, 0 ) < 0 ) {
		error( "Could not seek to embedded essence stream position 0" );
		return NULL;
	}


	/* Open stream with libsndfile */

	sndfile = sf_open_virtual( &sfvirtual, SFM_READ, sfinfo, audioEssenceFile );

	if ( !sndfile ) {
		error( "libsndfile could not open stream : %s", sf_strerror( NULL ) );
		return NULL;
	}


	/* Set format */

	if ( !sf_format_check( sfinfo ) ) {

		error( "Could not opent virtual input file" );

		error( "Input SF_INFO.samplerate: %i", sfinfo->samplerate );
		error( "Input SF_INFO.channels:   %i", sfinfo->channels   );
		error( "Input SF_INFO.format:     0x%x (%s, %s, %s)", sfinfo->format, sfaaf_container_name( sfinfo->format ), sfaaf_codec_name( sfinfo->format ), sfaaf_endian_name( sfinfo->format ) );
		error( "Input SF_INFO.sections:   %i", sfinfo->sections   );
		error( "Input SF_INFO.seekable:   %i", sfinfo->seekable   );

		return NULL;
	}

	debug( "Input SF_INFO.samplerate: %i", sfinfo->samplerate );
	debug( "Input SF_INFO.channels:   %i", sfinfo->channels   );
	debug( "Input SF_INFO.format:     0x%x (%s, %s, %s)", sfinfo->format, sfaaf_container_name( sfinfo->format ), sfaaf_codec_name( sfinfo->format ), sfaaf_endian_name( sfinfo->format ) );
	debug( "Input SF_INFO.sections:   %i", sfinfo->sections   );
	debug( "Input SF_INFO.seekable:   %i", sfinfo->seekable   );

	return sndfile;
}



static SNDFILE * sfaaf_open_outputFile( AAF_Iface *aafi, SF_INFO *sfinfo, const char *filepath, int format, uint32_t channels, int samplerate, int samplesize )
{
	SNDFILE *sndfile = NULL;

	if ( format == AAFI_EXTRACT_PCM &&
	     sfinfo->format & SF_FORMAT_AIFF )
	{
		/* When extracting raw PCM out of AIFC, we want to maintain endianness. */
		sfinfo->format &= ~SF_FORMAT_ENDMASK;
		sfinfo->format |= SF_ENDIAN_BIG;
	}

	if ( format > 0 ) {
		sfinfo->format &= ~SF_FORMAT_TYPEMASK;
		switch ( format ) {
			case AAFI_EXTRACT_WAV:
			case AAFI_EXTRACT_BWAV: sfinfo->format |= SF_FORMAT_WAV;  break;
			case AAFI_EXTRACT_AIFF: sfinfo->format |= SF_FORMAT_AIFF; break;
			case AAFI_EXTRACT_PCM:  sfinfo->format |= SF_FORMAT_RAW;  break;
			default:
				error( "Could not convert AAFI_EXTRACT format to SF_FORMAT : %i", format );
				return NULL;
		}
	}

	if ( samplesize > 0 ) {
		sfinfo->format &= ~SF_FORMAT_SUBMASK;
		sfinfo->format |= samplesize;
	}

	if ( samplerate > 0 ) {
		sfinfo->samplerate = samplerate;
	}

	assert( channels < INT_MAX );

	sfinfo->channels = (int)channels;


	sndfile = sf_open( filepath, SFM_WRITE, sfinfo );

	if ( !sndfile ) {

		error( "Could not open output file %s : %s", filepath, sf_strerror( NULL ) );

		error( "Output SF_INFO.samplerate: %i", sfinfo->samplerate );
		error( "Output SF_INFO.channels:   %i", sfinfo->channels   );
		error( "Output SF_INFO.format:     0x%x (%s, %s, %s)", sfinfo->format, sfaaf_container_name( sfinfo->format ), sfaaf_codec_name( sfinfo->format ), sfaaf_endian_name( sfinfo->format ) );
		error( "Output SF_INFO.sections:   %i", sfinfo->sections   );
		error( "Output SF_INFO.seekable:   %i", sfinfo->seekable   );

		return NULL;
	}

	debug( "Output SF_INFO.samplerate: %i", sfinfo->samplerate );
	debug( "Output SF_INFO.channels:   %i", sfinfo->channels   );
	debug( "Output SF_INFO.format:     0x%x (%s, %s, %s)", sfinfo->format, sfaaf_container_name( sfinfo->format ), sfaaf_codec_name( sfinfo->format ), sfaaf_endian_name( sfinfo->format ) );
	debug( "Output SF_INFO.sections:   %i", sfinfo->sections   );
	debug( "Output SF_INFO.seekable:   %i", sfinfo->seekable   );

	return sndfile;
}



static int sfaaf_extract_audio( AAF_Iface *aafi, uint32_t infilecount, SNDFILE *infiles[], SNDFILE *outfile, sf_count_t frameOffset, sf_count_t frameLength, int processFloatingPoint )
{
	int rc = 0;

	void *inbuf = NULL;
	void *outbuf = NULL;

	sf_count_t frameread[MAX_AUDIO_FILES_MERGE] = { 0 };
	sf_count_t totalframeread[MAX_AUDIO_FILES_MERGE] = { 0 };

	sf_count_t framewritten = 0;
	sf_count_t totalframewritten = 0;

	if ( infilecount >= MAX_AUDIO_FILES_MERGE ) {
		error( "Number of input files (%u) exceed maximum limit (%i)", infilecount, MAX_AUDIO_FILES_MERGE );
		goto err;
	}

	debug( "Request audio extract: infilecount: %u  process float: %i", infilecount, processFloatingPoint );


	if ( processFloatingPoint ) {
		inbuf  = calloc( AAF_EXTRACT_FILE_BUFFER_SZ, sizeof(double) );
		outbuf = calloc( AAF_EXTRACT_FILE_BUFFER_SZ * infilecount, sizeof(double) );
	}
	else {
		inbuf  = calloc( AAF_EXTRACT_FILE_BUFFER_SZ, sizeof(int) );
		outbuf = calloc( AAF_EXTRACT_FILE_BUFFER_SZ * infilecount, sizeof(int) );
	}

	if ( !inbuf || !outbuf ) {
		error( "Could not allocate in or out buffer" );
		goto err;
	}


	while ( 1 ) {

		sf_count_t writeOffset = 0;
		sf_count_t writeLen = 0;

		// sf_count_t maxframeread = 0;
		int skipRead = 0;

		for ( int ch = 0; ch < (int)infilecount; ch++ ) {

			// debug("reading file %i", ch);

			memset( (int*)inbuf, 0, AAF_EXTRACT_FILE_BUFFER_SZ );

			if ( processFloatingPoint ) {
				frameread[ch] = sf_readf_double( infiles[ch], inbuf, AAF_EXTRACT_FILE_BUFFER_SZ );
			}
			else {
				frameread[ch] = sf_readf_int( infiles[ch], inbuf, AAF_EXTRACT_FILE_BUFFER_SZ );
			}

			if ( frameread[ch] <= 0 ) {
				if ( sf_error(infiles[ch]) != SF_ERR_NO_ERROR ) {
					error( "libsndfile %s() error: %s", (processFloatingPoint) ? "sf_readf_double" : "sf_readf_int", sf_strerror(infiles[ch]) );
					goto err;
				}
				/* EOF */
				goto end;
			}

			totalframeread[ch] += frameread[ch];

			writeOffset = 0;
			writeLen = frameread[ch];

			if ( frameOffset > 0 ) {

				if ( totalframeread[ch] < frameOffset ) {
					skipRead = 1;
					continue;
				}

				if ( (totalframeread[ch] - frameOffset) < frameread[ch] ) {
					writeOffset = frameread[ch] - (totalframeread[ch] - frameOffset);
					writeLen = frameread[ch] - writeOffset;
					// debug( "Writting first %"PRIi64" file sample frames at offset %"PRIi64" : bufferoffset: %"PRIi64, writeLen, frameOffset, writeOffset );
				}
			}

			if ( frameLength > 0 ) {

				if ( totalframewritten + frameread[ch] >= frameLength ) {
					writeLen = frameLength - totalframewritten;
					// debug( "Writting last %"PRIi64" file sample frames", writeLen );
				}
			}

			// debug("writeLen : %"PRIi64, writeLen);

			if ( processFloatingPoint ) {
				for ( sf_count_t i = 0; i < writeLen; i++ ) {
					((double*)outbuf)[(i*infilecount)+ch] = ((double*)inbuf)[writeOffset+i];
				}
			}
			else {
				for ( sf_count_t i = 0; i < writeLen; i++ ) {
					((int*)outbuf)[(i*infilecount)+ch] = ((int*)inbuf)[writeOffset+i];
				}
			}

			// maxframeread = MAX( maxframeread, frameread[ch] );
		}

		if ( skipRead ) {
			continue;
		}


		if ( processFloatingPoint ) {
			framewritten = sf_writef_double( outfile, (double*)outbuf, writeLen );
		}
		else {
			framewritten = sf_writef_int( outfile, (int*)outbuf, writeLen );
		}

		if ( framewritten != writeLen ) {
			error( "Wrote %"PRIi64" out of %"PRIi64" sample frames", framewritten, writeLen );
			if ( sf_error(outfile) != SF_ERR_NO_ERROR ) {
				error( "libsndfile %s() error: %s", (processFloatingPoint) ? "sf_writef_double" : "sf_writef_int", sf_strerror(outfile) );
			}
			goto err;
		}

		totalframewritten += framewritten;

		// debug( "Wrote %"PRIi64" (total: %"PRIi64") %"PRIi64, framewritten, totalframewritten, frameLength );

		if ( frameLength && totalframewritten == frameLength ) {
			// debug( "Reached requested frame count: %"PRIi64, frameLength );
			break;
		}
	}

	goto end;

err:
	rc = -1;

end:

	// debug( "Extracted %"PRIi64" sample frames", totalframewritten );

	free(inbuf);
	free(outbuf);

	return rc;
}



int aafi_extract_audioClip( AAF_Iface *aafi, aafiAudioClip *audioClip, const char *outpath, enum aafiExtractFormat format, int samplerate, int samplesize, char **usable_file_path )
{
	int rc = 0;

	uint64_t frameOffset = 0;
	uint64_t frameLength = 0;

	SNDFILE *infiles[MAX_AUDIO_FILES_MERGE];
	SNDFILE *outfile = NULL;
	SF_INFO sfinfo;
	char *filepath = NULL;

	int hasEmbeddedEssences = 0;
	uint32_t outchannels = 0;

	int processFloatingPoint = 0;

	if ( samplesize == SF_FORMAT_FLOAT ) {
		processFloatingPoint = 1;
	}


	aafiAudioEssencePointer *audioEssencePtr = NULL;

	AAFI_foreachClipEssencePointer( audioClip, audioEssencePtr ) {

		aafiAudioEssenceFile *audioEssenceFile = audioEssencePtr->essenceFile;

		if ( !audioEssenceFile ) {
			error( "Fatal: essence pointer holds no essence file" );
			continue;
		}

		if ( !audioEssenceFile->is_embedded ) {
			debug( "Audio essence \"%s\" is not embedded : nothing to extract", audioEssenceFile->unique_name );
			continue;
		}

		hasEmbeddedEssences = 1;

		frameOffset = aafi_convertUnitUint64( audioClip->essence_offset, audioClip->track->edit_rate, audioEssenceFile->samplerateRational );
		frameLength = aafi_convertUnitUint64( audioClip->len,            audioClip->track->edit_rate, audioEssenceFile->samplerateRational );


		if ( outchannels >= MAX_AUDIO_FILES_MERGE ) {
			error( "Too many files to merge, max is %i", MAX_AUDIO_FILES_MERGE );
			goto err;
		}

		infiles[outchannels] = aafi_sf_open_virtual_audioEssenceFile( aafi, audioEssenceFile, &sfinfo );

		if ( !infiles[outchannels] ) {
			error( "Could not open audio essence file virtual input stream" );
			goto err;
		}

		if ( (sfinfo.format & SF_FORMAT_SUBMASK) == SF_FORMAT_FLOAT ||
		     (sfinfo.format & SF_FORMAT_SUBMASK) == SF_FORMAT_DOUBLE )
		{
			processFloatingPoint = 1;
		}

		outchannels++;
	}

	if ( !hasEmbeddedEssences ) {
		warning( "Clip has no embedded essence to extract" );
		goto err;
	}


	char *name = NULL;

	laaf_util_snprintf_realloc( &name, NULL, 0, "%i_%i_%s", audioClip->track->number, aafi_get_clipIndex(audioClip), audioClip->essencePointerList->essenceFile->unique_name );

	filepath = aafi_build_audioEssenceFilePath( aafi, audioClip->essencePointerList->essenceFile, outpath, name, format, 1 );

	free( name );

	if ( !filepath ) {
		error( "Could not build file path" );
		goto err;
	}

	debug( "filepath: %s", filepath );

	outfile = sfaaf_open_outputFile( aafi, &sfinfo, filepath, format, outchannels, samplerate, samplesize );

	if ( !outfile ) {
		error( "Could not open output file : %s", filepath );
		goto err;
	}

	if ( format == AAFI_EXTRACT_BWAV ) {
		sfaaf_set_bext( aafi, audioClip->essencePointerList->essenceFile, outfile );
	}

	assert( frameOffset < SF_COUNT_MAX );
	assert( frameLength < SF_COUNT_MAX );

	debug( "frameOffset: %"PRIi64, frameOffset );
	debug( "frameLength: %"PRIi64, frameLength );

	rc = sfaaf_extract_audio( aafi, outchannels, infiles, outfile, (sf_count_t)frameOffset, (sf_count_t)frameLength, processFloatingPoint );

	if ( rc < 0 ) {
		error( "Audio clip file extraction failed : %s\"%s\"%s", ANSI_COLOR_DARKGREY(aafi->log), filepath, ANSI_COLOR_RESET(aafi->log) );
		goto err;
	}

	debug( "Audio clip file extracted to %s\"%s\"%s", ANSI_COLOR_DARKGREY(aafi->log), filepath, ANSI_COLOR_RESET(aafi->log) );


	if ( usable_file_path ) {
		*usable_file_path = laaf_util_c99strdup( filepath );

		if ( !(*usable_file_path) ) {
			error( "Could not duplicate usable filepath : %s", filepath );
			goto err;
		}
	}

	goto end;

err:
	rc = -1;

end:

	for ( size_t i = 0; i < outchannels; i++ ) {
		if ( infiles[i] )
			sf_close( infiles[i] );
	}

	if ( outfile )
		sf_close( outfile );

	free( filepath );

	return rc;
}

#endif // HAVE_SNDFILE



static int set_audioEssenceWithRIFF( AAF_Iface *aafi, const char *filename, aafiAudioEssenceFile *audioEssenceFile, struct RIFFAudioFile *RIFFAudioFile, int isExternalFile )
{
	if ( RIFFAudioFile->sampleCount >= INT64_MAX ) {
		error( "%s : summary sample count is bigger than INT64_MAX (%"PRIu64")", audioEssenceFile->usable_file_path, RIFFAudioFile->sampleCount );
		return -1;
	}

	if ( RIFFAudioFile->sampleRate >= INT_MAX ) {
		error( "%s : summary sample rate is bigger than INT_MAX (%u)", audioEssenceFile->usable_file_path, RIFFAudioFile->sampleRate );
		return -1;
	}

	if ( audioEssenceFile->channels > 0 && audioEssenceFile->channels != RIFFAudioFile->channels ) {
		warning( "%s : summary channel count (%i) mismatch %s (%i)", filename, audioEssenceFile->channels, ((isExternalFile) ? "located file" : "previously retrieved data"), RIFFAudioFile->channels );
	}

	if ( audioEssenceFile->samplerate > 0 && audioEssenceFile->samplerate != RIFFAudioFile->sampleRate ) {
		warning( "%s : summary samplerate (%i) mismatch %s (%i)", filename, audioEssenceFile->samplerate, ((isExternalFile) ? "located file" : "previously retrieved data"), RIFFAudioFile->sampleRate );
	}

	if ( audioEssenceFile->samplesize > 0 && audioEssenceFile->samplesize != RIFFAudioFile->sampleSize ) {
		warning( "%s : summary samplesize (%i) mismatch %s (%i)", filename , audioEssenceFile->samplesize, ((isExternalFile) ? "located file" : "previously retrieved data"), RIFFAudioFile->sampleSize );
	}

	if ( audioEssenceFile->length > 0 && (uint64_t)audioEssenceFile->length != RIFFAudioFile->sampleCount ) {
		warning( "%s : summary samplecount (%"PRIi64") mismatch %s (%"PRIi64")", filename , audioEssenceFile->length, ((isExternalFile) ? "located file" : "previously retrieved data"), RIFFAudioFile->sampleCount );
	}

	audioEssenceFile->channels   = RIFFAudioFile->channels;
	audioEssenceFile->samplerate = RIFFAudioFile->sampleRate;
	audioEssenceFile->samplesize = RIFFAudioFile->sampleSize;

	audioEssenceFile->length = (aafPosition_t)RIFFAudioFile->sampleCount;
	audioEssenceFile->pcm_audio_start_offset = (uint64_t)RIFFAudioFile->pcm_audio_start_offset;
	audioEssenceFile->samplerateRational->numerator = (int32_t)audioEssenceFile->samplerate;

	audioEssenceFile->samplerateRational->denominator = 1;


	return 0;
}



int aafi_extract_original_audioEssenceFile( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile, const char *outpath, const char *rename, int skipHeader )
{
	int rc = 0;

	char *filepath = NULL;
	FILE* fp = NULL;
	void *buf = NULL;

	int64_t fileOffset = 0;

	if ( audioEssenceFile->is_embedded == 0 ) {
		error( "Audio essence is not embedded : nothing to extract" );
		goto err;
	}

	if ( !outpath ) {
		error( "Missing output path" );
		goto err;
	}

	if ( audioEssenceFile->usable_file_path ) {
		debug( "usable_file_path was already set" );
		free( audioEssenceFile->usable_file_path );
		audioEssenceFile->usable_file_path = NULL;
	}

	debug( "Request audio essence (byte-for-byte) file extraction of \"%s\"", audioEssenceFile->name );
	debug( "  destination: \"%s\"", outpath );
	debug( "  rename:      %s%s%s", (rename) ? "\"" : " ", (rename) ? rename : "NULL", (rename) ? "\"" : "" );
	debug( "  skip header:  %i", skipHeader );

	if ( skipHeader ) {

		assert( audioEssenceFile->pcm_audio_start_offset < INT64_MAX );

		fileOffset = (int64_t)audioEssenceFile->pcm_audio_start_offset;
		debug( "Skipping header with file offset: %"PRIi64" bytes", fileOffset );
	}

	filepath = aafi_build_audioEssenceFilePath( aafi, audioEssenceFile, outpath, rename, (skipHeader) ? AAFI_EXTRACT_ORIGINAL_PCM : AAFI_EXTRACT_ORIGINAL, 0 );

	if ( !filepath ) {
		error( "Could not build file path" );
		goto err;
	}

	debug( "Computed file path: \"%s\"", filepath );


	fp = fopen( filepath, "wb" );

	if ( !fp ) {
		error( "Could not open '%s' for writing : %s", filepath, strerror(errno) );
		goto err;
	}

	buf = malloc( AAF_EXTRACT_FILE_BUFFER_SZ );

	if ( !buf ) {
		goto err;
	}

	if ( cfb_stream_seek( audioEssenceFile->sd, SEEK_SET, fileOffset ) < 0 ) {
		error( "Could not seek to embedded essence stream position %"PRIi64, fileOffset );
		goto err;
	}

	size_t bufsz = cfb_stream_size( audioEssenceFile->sd ) - (size_t)fileOffset;
	size_t totalWrittenBytes = 0;

	while ( totalWrittenBytes < bufsz ) {

		ssize_t readBytes = cfb_stream_read( audioEssenceFile->sd, buf, AAF_EXTRACT_FILE_BUFFER_SZ );

		if ( readBytes < 0 ) {
			error( "Error reading essence data stream" );
			goto err;
		}

		size_t writtenBytes = fwrite( buf, sizeof(unsigned char), (size_t)readBytes, fp );

		if ( writtenBytes != (size_t)readBytes ) {
			error( "Error writting essence data stream" );
			goto err;
		}

		totalWrittenBytes += writtenBytes;
	}

	debug( "Extracted %zu bytes", totalWrittenBytes );


	audioEssenceFile->usable_file_path = laaf_util_c99strdup( filepath );

	if ( !audioEssenceFile->usable_file_path ) {
		error( "Could not duplicate usable filepath : %s", filepath );
		goto err;
	}

	rc = 0;
	goto end;

err:
	rc = -1;

end:
	free( filepath );
	free( buf );

	if ( fp )
		fclose( fp );

	return rc;
}



char * aafi_build_audioEssenceFilePath( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile, const char *outpath, const char *rename, int format, int clean_channel_identif )
{
	char *filename = NULL;
	char *filepath = NULL;

	char *name = NULL;

	if ( rename ) {
		name = laaf_util_c99strdup( rename );
	} else {
		name = laaf_util_c99strdup( audioEssenceFile->unique_name );
	}

	if ( !name ) {
		error( "Could not duplicate name" );
		goto end;
	}

	const char *fileext = NULL;

	if ( format == AAFI_EXTRACT_WAV ||
	     format == AAFI_EXTRACT_BWAV ||
	   ((format == AAFI_EXTRACT_DEFAULT ||
	     format == AAFI_EXTRACT_ORIGINAL) && audioEssenceFile->type == AAFI_ESSENCE_TYPE_WAVE) )
	{
		if ( !laaf_util_is_fileext( name, "wav"  ) &&
		     !laaf_util_is_fileext( name, "wave" ) )
		{
			fileext = "wav";
		}
	}
	else if ( format == AAFI_EXTRACT_AIFF ||
	        ((format == AAFI_EXTRACT_DEFAULT ||
	          format == AAFI_EXTRACT_ORIGINAL) && audioEssenceFile->type == AAFI_ESSENCE_TYPE_AIFC) )
	{
		if ( !laaf_util_is_fileext( name, "aif"  ) &&
		     !laaf_util_is_fileext( name, "aiff" ) &&
		     !laaf_util_is_fileext( name, "aifc" ) )
		{
			fileext = "aif";
		}
	}
	else if ( format == AAFI_EXTRACT_PCM ||
	          format == AAFI_EXTRACT_ORIGINAL_PCM ||
	        ((format == AAFI_EXTRACT_DEFAULT ||
	          format == AAFI_EXTRACT_ORIGINAL) && audioEssenceFile->type == AAFI_ESSENCE_TYPE_PCM) )
	{
		if ( !laaf_util_is_fileext( name, "pcm"  ) &&
				 !laaf_util_is_fileext( name, "raw" ) )
		{
			fileext = "pcm";
		}
	}

	if ( fileext ) {
		if ( laaf_util_snprintf_realloc( &filename, NULL, 0, "%s.%s", laaf_util_clean_filename(name, clean_channel_identif), fileext ) < 0 ) {
			error( "Could not concat filename + fileext" );
			goto end;
		}
	}
	else {
		filename = laaf_util_c99strdup( name );

		if ( !filename ) {
			error( "Could not duplicate filename : %s", name );
			goto end;
		}
	}


	filepath = laaf_util_build_path( DIR_SEP_STR, outpath, filename, NULL );

	if ( !filepath ) {
		error( "Could not build filepath." );
		goto end;
	}

end:

	free( name );

	free( filename );

	return filepath;
}



int aafi_parse_audioEssenceFile( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile )
{
	int rc = 0;
	uint64_t dataStreamSize = 0;
	unsigned char *dataStream = NULL;
	FILE *fp = NULL;
	struct RIFFAudioFile RIFFAudioFile;


	/* try audioEssenceFile->summary first, for both embedded and external */

	if ( audioEssenceFile->summary ) {

		rc = laaf_riff_parseAudioFile( &RIFFAudioFile, RIFF_PARSE_AAF_SUMMARY, &embeddedAudioDataReaderCallback, audioEssenceFile->summary->val, &audioEssenceFile->summary->len, aafi, aafi->log );

		if ( rc < 0 ) {

			if ( !audioEssenceFile->is_embedded && !audioEssenceFile->usable_file_path ) {
				warning( "Could not parse essence summary of \"%s\".", audioEssenceFile->name );
				goto err;
			}

			warning( "Could not parse essence summary of \"%s\". %s",
				audioEssenceFile->name,
				(audioEssenceFile->is_embedded)      ? "Trying essence data stream."   :
				(audioEssenceFile->usable_file_path) ? "Trying external essence file." : " WTF ???" );
		}
		else {

			if ( set_audioEssenceWithRIFF( aafi, "AAF Summary", audioEssenceFile, &RIFFAudioFile, 0 ) < 0 ) {
				goto err;
			}

			if ( !RIFFAudioFile.channels   ||
			     !RIFFAudioFile.sampleRate ||
			     !RIFFAudioFile.sampleSize ||
			     !RIFFAudioFile.sampleCount )
			{
				/*
				 * Adobe Premiere Pro AIFC/WAVE Summaries of external files are missing
				 * SSND chunk/DATA chunk size (RIFFAudioFile.sampleCount)
				 */

				if ( !audioEssenceFile->is_embedded && !audioEssenceFile->usable_file_path ) {
					warning( "Summary of \"%s\" is missing some data.", audioEssenceFile->name );
					goto err;
				}

				warning( "Summary of \"%s\" is missing some data. %s",
					 audioEssenceFile->name,
					(audioEssenceFile->is_embedded)      ? "Trying essence data stream."   :
					(audioEssenceFile->usable_file_path) ? "Trying external essence file." : " WTF ???" );
			}
			else {
				goto end;
			}
		}
	}
	else if ( audioEssenceFile->is_embedded ) {
		warning( "Embedded audio essence \"%s\" has no summary. Trying essence data stream.", audioEssenceFile->name );
	}
	else if ( audioEssenceFile->usable_file_path ) {
		warning( "External audio essence \"%s\" has no summary. Trying external file.", audioEssenceFile->name );
	}


	if ( audioEssenceFile->is_embedded ) {

		cfb_getStream( aafi->aafd->cfbd, audioEssenceFile->node, &dataStream, &dataStreamSize );

		if ( dataStream == NULL ) {
			error( "Could not retrieve audio essence stream from CFB" );
			goto err;
		}

		rc = laaf_riff_parseAudioFile( &RIFFAudioFile, RIFF_PARSE_AAF_SUMMARY, &embeddedAudioDataReaderCallback, dataStream, &dataStreamSize, aafi, aafi->log );

		if ( rc < 0 ) {
			warning( "Could not parse embedded essence stream of \"%s\".", audioEssenceFile->name );
			goto err;
		}

		if ( set_audioEssenceWithRIFF( aafi, "AAF Embedded stream", audioEssenceFile, &RIFFAudioFile, 0 ) < 0 ) {
			goto err;
		}

		goto end;
	}



	if ( laaf_util_is_fileext( audioEssenceFile->usable_file_path, "wav"  ) ||
	     laaf_util_is_fileext( audioEssenceFile->usable_file_path, "wave" ) ||
	     laaf_util_is_fileext( audioEssenceFile->usable_file_path, "aif"  ) ||
	     laaf_util_is_fileext( audioEssenceFile->usable_file_path, "aiff" ) ||
	     laaf_util_is_fileext( audioEssenceFile->usable_file_path, "aifc" ) )
	{
		fp = fopen( audioEssenceFile->usable_file_path, "rb" );

		if ( fp == NULL ) {
			error( "Could not open external audio essence file for reading : %s", audioEssenceFile->usable_file_path );
			goto err;
		}


		rc = laaf_riff_parseAudioFile( &RIFFAudioFile, 0, &externalAudioDataReaderCallback, fp, audioEssenceFile->usable_file_path, aafi, aafi->log );

		if ( rc < 0 ) {
			error( "Failed parsing external audio essence file : %s", audioEssenceFile->usable_file_path );
			goto err;
		}

		if ( set_audioEssenceWithRIFF( aafi, audioEssenceFile->usable_file_path, audioEssenceFile, &RIFFAudioFile, 1 ) < 0 ) {
			goto err;
		}
	}
	else {
		/*
		 * should be considered as a non-pcm audio format
		 *
│ 04317│├──◻ AAFClassID_TimelineMobSlot [slot:6 track:4] (DataDef : AAFDataDef_Sound) : Audio 4 - Layered Audio Editing
│ 01943││    └──◻ AAFClassID_Sequence
│ 02894││         └──◻ AAFClassID_SourceClip
│ 02899││              └──◻ AAFClassID_MasterMob (UsageCode: n/a) : speech-sample
│ 04405││                   └──◻ AAFClassID_TimelineMobSlot [slot:1 track:1] (DataDef : AAFDataDef_Sound)
│ 03104││                        └──◻ AAFClassID_SourceClip
│ 04140││                             └──◻ AAFClassID_SourceMob (UsageCode: n/a) : speech-sample
│ 01287││                                  └──◻ AAFClassID_PCMDescriptor
│ 01477││                                       └──◻ AAFClassID_NetworkLocator : file:///C:/Users/user/Desktop/libAAF/test/res/speech-sample.mp3
		 *
		 */

		audioEssenceFile->type = AAFI_ESSENCE_TYPE_UNK;
	}


	rc = 0;
	goto end;

err:
	rc = -1;

end:
	free( dataStream );

	if ( fp )
		fclose( fp );

	return rc;
}



static size_t embeddedAudioDataReaderCallback( unsigned char *buf, size_t offset, size_t reqlen, void *user1, void *user2, void *user3 )
{
	unsigned char *data = user1;
	size_t datasz = *(size_t*)user2;
	AAF_Iface *aafi = (AAF_Iface*)user3;

	if ( offset > datasz ) {
		error( "Requested data starts beyond data length" );
		return RIFF_READER_ERROR;
	}

	if ( offset+reqlen > datasz ) {
		reqlen = datasz - (offset+reqlen);
	}

	memcpy( buf, data+offset, reqlen );

	return reqlen;
}



static size_t externalAudioDataReaderCallback( unsigned char *buf, size_t offset, size_t reqlen, void *user1, void *user2, void *user3 )
{
	FILE *fp = (FILE*)user1;
	const char *filename = (const char*)user2;
	AAF_Iface *aafi = (AAF_Iface*)user3;

#ifdef _WIN32
	assert( offset < _I64_MAX );

	if ( _fseeki64( fp, (__int64)offset, SEEK_SET ) < 0 ) {
		error( "Could not seek to %"PRIu64" in file '%s' : %s", offset, filename, strerror(errno) );
		return RIFF_READER_ERROR;
	}
#else
	assert( offset < LONG_MAX );

	if ( fseek( fp, (long)offset, SEEK_SET ) < 0 ) {
		error( "Could not seek to %"PRIu64" in file '%s' : %s", offset, filename, strerror(errno) );
		return RIFF_READER_ERROR;
	}
#endif

	size_t byteRead = fread( buf, sizeof(unsigned char), reqlen, fp );

	if ( feof(fp) ) {
		if ( byteRead < reqlen ) {
			error( "Incomplete fread() of '%s' due to EOF : %"PRIu64" bytes read out of %"PRIu64" requested", filename, byteRead, reqlen );
			return RIFF_READER_ERROR;
		}
		debug( "fread() : EOF reached in file '%s'", filename );
	}
	else if ( ferror(fp) ) {
		if ( byteRead < reqlen ) {
			error( "Incomplete fread() of '%s' due to error : %"PRIu64" bytes read out of %"PRIu64" requested", filename, byteRead, reqlen );
		}
		else {
			error( "fread() error of '%s' : %"PRIu64" bytes read out of %"PRIu64" requested", filename, byteRead, reqlen );
		}
		return RIFF_READER_ERROR;
	}

	return byteRead;
}



const char *aafi_extractFormatToText( enum aafiExtractFormat fmt )
{
	switch ( fmt )
	{
		case AAFI_EXTRACT_DEFAULT:      return "AAFI_EXTRACT_DEFAULT";
		case AAFI_EXTRACT_ORIGINAL:     return "AAFI_EXTRACT_ORIGINAL";
		case AAFI_EXTRACT_ORIGINAL_PCM: return "AAFI_EXTRACT_ORIGINAL_PCM";
		case AAFI_EXTRACT_WAV:          return "AAFI_EXTRACT_WAV";
		case AAFI_EXTRACT_BWAV:         return "AAFI_EXTRACT_BWAV";
		case AAFI_EXTRACT_AIFF:         return "AAFI_EXTRACT_AIFF";
		case AAFI_EXTRACT_PCM:          return "AAFI_EXTRACT_PCM";
		default: break;
	}

	return "unknown value";
}

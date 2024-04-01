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

#ifndef __AAFIEssenceFile_h__
#define __AAFIEssenceFile_h__

/**
 * @file LibAAF/AAFIface/AAFIEssenceFile.h
 * @brief AAF processing
 * @author Adrien Gesta-Fline
 * @version 0.1
 * @date 27 june 2024
 *
 * @ingroup AAFIface
 * @addtogroup AAFIface
 * @{
 */

#include <libaaf/AAFIface.h>

#ifdef HAVE_SNDFILE
#include <sndfile.h>
#endif

/**
 * Buffer size for embedded audio essence/clip file extraction.
 */

#define AAF_EXTRACT_FILE_BUFFER_SZ 4096


/**
 * Define extract format when calling aafi_extract_audioEssenceFile() or aafi_extract_audioClip()
 */

enum aafiExtractFormat {
	AAFI_EXTRACT_DEFAULT = 0, /**< keeps original format, but not a byte-for-byte copy */
	AAFI_EXTRACT_ORIGINAL, /**< byte-for-byte copy of the original file */
	AAFI_EXTRACT_ORIGINAL_PCM, /**< byte-for-byte copy of the original PCM audio stream */
	AAFI_EXTRACT_WAV, /**< output wav file */
	AAFI_EXTRACT_BWAV, /**< output wav file with bext chunk */
	AAFI_EXTRACT_AIFF, /**< output aiff file */
	AAFI_EXTRACT_PCM /**< output raw PCM file */
};



char * aafi_locate_external_audioEssenceFile( AAF_Iface *aafi, const char *original_uri_filepath, const char *search_location );

#ifdef HAVE_SNDFILE
/**
 * Maximum number of input files when combining multiple mono files to a
 * single multichannel clip. Consequently, this also defines the maximum
 * channel count in output file.
 */
#define MAX_AUDIO_FILES_MERGE 16
#endif

size_t  aafi_embeddedAudioEssenceFile_size( aafiAudioEssenceFile *audioEssenceFile );
size_t  aafi_embeddedAudioEssenceFile_tell( aafiAudioEssenceFile *audioEssenceFile );
ssize_t aafi_embeddedAudioEssenceFile_seek( aafiAudioEssenceFile *audioEssenceFile, int whence, int64_t pos );
ssize_t aafi_embeddedAudioEssenceFile_read( aafiAudioEssenceFile *audioEssenceFile, void* buf, size_t nbyte );

#ifdef HAVE_SNDFILE

sf_count_t aafi_sf_embeddedAudioEssenceFile_size( void *audioEssenceFile );
sf_count_t aafi_sf_embeddedAudioEssenceFile_tell( void *audioEssenceFile );
sf_count_t aafi_sf_embeddedAudioEssenceFile_seek( sf_count_t pos, int whence, void *audioEssenceFile );
sf_count_t aafi_sf_embeddedAudioEssenceFile_read( void* buf, sf_count_t nbyte, void *audioEssenceFile );

/**
 * Fill SF_INFO structure with correct values and open the embedded audio essence
 * file, even if essence is raw PCM.
 */
SNDFILE * aafi_sf_open_virtual_audioEssenceFile( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile, SF_INFO *sfinfo );



/**
 * @note Requires libaaf to be compiled with `-DHAVE_SNDFILE`
 *
 * Extract audio essence file using libsndfile, with the ability to choose the
 * output format, sample rate and sample size.
 *
 * Function allows to extract a portion of essence file, using frameOffset and
 * frameLength.
 *
 * Function will set output file extension if needed.
 *
 * Function will set aafiAudioEssenceFile.usable_file_path on success.
 *
 * Function does not check if output file already exists and any existing file will
 * be overwritten.
 *
 * @param aafi              Pointer to the current AAF_Iface struct.
 * @param audioEssenceFile  Pointer to the aafiAudioEssenceFile to extract.
 * @param outpath           Location where the file should be extracted.
 * @param rename            If not NULL, overrides original file name.
 * @param frameOffset       If > 0, sets the offset of audio sample frame to extract.
 * @param frameLength       If > 0, sets the length of audio sample frame to extract.
 * @param format            If not AAFI_EXTRACT_DEFAULT, sets output format.
 * @param samplerate        If > 0, sets sample rate of output file (without performing any conversion).
 * @param samplesize        If > 0, sets sample size of output file.
 * @param usable_file_path  If not NULL, then pointer will be allocated and store the essence file path.
 *
 * @return                  0 on success, -1 on error
 */

int aafi_extract_audioEssenceFile( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile, const char *outpath, const char *rename, uint64_t frameOffset, uint64_t frameLength, enum aafiExtractFormat format, int samplerate, int samplesize, char **usable_file_path );

/**
 * @note Requires libaaf to be compiled with `-DHAVE_SNDFILE`
 *
 * Extract audio essence clip using libsndfile, with the ability to choose the
 * output format, sample rate, sample size.
 *
 * If clip is multichannel, a single multichannel file is created, even if all
 * channels are stored as separate file in AAF.
 *
 * Function does not check if output file already exists and any existing file will
 * be overwritten.
 *
 * @param aafi              Pointer to the current AAF_Iface struct.
 * @param audioClip         Pointer to the aafiAudioClip to extract.
 * @param outpath           Location where the file should be extracted.
 * @param format            If not AAFI_EXTRACT_DEFAULT, sets output format.
 * @param samplerate        If > 0, sets sample rate of output file (without performing any conversion).
 * @param samplesize        If > 0, sets sample size of output file.
 * @param usable_file_path  If not NULL, then pointer will be allocated and store the clip file path.
 *
 * @return                  0 on success, -1 on error
 */

int aafi_extract_audioClip( AAF_Iface *aafi, aafiAudioClip *audioClip, const char *outpath, enum aafiExtractFormat format, int samplerate, int samplesize, char **usable_file_path );

#endif


/**
 * Extract audio essence file as it is stored in AAF file (WAV, BWAV, AIFF or raw
 * PCM). The extraction is a byte-for-byte copy of the original file. As an option,
 * one can choose to extract only raw PCM data by skipping any file header.
 *
 * Function will set output file extension if needed.
 *
 * Function will set aafiAudioEssenceFile.usable_file_path on success.
 *
 * Function does not check if output file already exists and any existing file will
 * be overwritten.
 *
 * @param aafi              Pointer to the current AAF_Iface struct.
 * @param audioEssenceFile  Pointer to the aafiAudioEssenceFile to extract.
 * @param outpath           Location where the file should be extracted.
 * @param rename            Optional name to override orginal file name.
 * @param skipHeader        Optional boolean to extract only raw PCM.
 *
 * @return                  0 on success, -1 on error
 */

int aafi_extract_original_audioEssenceFile( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile, const char *outpath, const char *rename, int skipHeader );


int aafi_parse_audioEssenceFile( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile );

int aafi_build_audioEssenceFileUniqueName( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile );

char * aafi_build_audioEssenceFilePath( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile, const char *outpath, const char *rename, int format, int clean_channel_identif );

const char * aafi_extractFormatToText( enum aafiExtractFormat fmt );

/**
 * @}
 */
#endif // !__AAFIEssenceFile_h__

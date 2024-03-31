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



char * aafi_locate_external_essence_file( AAF_Iface *aafi, const char *original_uri_filepath, const char *search_location );

size_t  aafi_embeddedAudioEssenceFile_size( aafiAudioEssenceFile *audioEssenceFile );
size_t  aafi_embeddedAudioEssenceFile_tell( aafiAudioEssenceFile *audioEssenceFile );
ssize_t aafi_embeddedAudioEssenceFile_seek( aafiAudioEssenceFile *audioEssenceFile, int whence, int64_t pos );
ssize_t aafi_embeddedAudioEssenceFile_read( aafiAudioEssenceFile *audioEssenceFile, void* buf, size_t nbyte );
#ifdef HAVE_SNDFILE
/**
 * Extract audio essence file.
 *
 * @param aafi XXXXXX
 */
int aafi_extractAudioEssenceFile( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile, enum aafiExtractFormat extractFormat, const char *outfilepath, uint64_t sampleOffset, uint64_t sampleLength, const char *forcedFileName, char **usable_file_path );

int aafi_extractAudioClip( AAF_Iface *aafi, aafiAudioClip *audioClip, enum aafiExtractFormat extractFormat, const char *outfilepath );

int aafi_parse_audio_essence( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile );

int aafi_build_unique_audio_essence_name( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile );

/**
 * @}
 */
#endif // !__AAFIEssenceFile_h__

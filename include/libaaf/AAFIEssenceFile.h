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

/**
 * @file libaaf/AAFIEssenceFile.h
 */

#ifndef __AAFIEssenceFile_h__
#define __AAFIEssenceFile_h__

#include <libaaf/AAFIface.h>

#ifdef HAVE_SNDFILE

#include <sndfile.h>

/**
 * Maximum number of input files when combining multiple mono files to a
 * single multichannel clip. Consequently, this also defines the maximum
 * channel count in output file.
 */
#define MAX_AUDIO_FILES_MERGE 16

#endif // HAVE_SNDFILE


/**
 * Buffer size for embedded audio essence/clip file extraction.
 */

#define AAF_EXTRACT_FILE_BUFFER_SZ 4096


/**
 * @addtogroup libaaf_api_audio_essence
 *
 * Retrieve all essence files described in AAF file
 * ================================================
 *
 * @code{.c}
struct aafiAudioEssenceFile *audioEssenceFile = NULL;

AAFI_foreachAudioEssenceFile( aafi, audioEssenceFile ) {
	// ...
}
 * @endcode
 *
 * Retrieve essence files composing an audio clip
 * ==============================================
 *
 * It is common for a multichannel audio clip to be composed of multiple
 * audio essence files, for example when each audio channel is on a separate
 * mono file. That's why an aafiAudioClip, instead of holding aafiAudioEssenceFile
 * directly, holds a list of aafiAudioEssencePointer, each one pointing to an
 * aafiAudioEssenceFile.
 *
 * @code{.c}
struct aafiAudioEssenceFile *audioEssenceFile = NULL;
struct aafiAudioEssencePointer *audioEssencePtr = NULL;

AAFI_foreachClipEssencePointer( audioClip, audioEssencePtr ) {
	audioEssenceFile = audioEssencePtr->essenceFile;
 // ...
}
 * @endcode
 *
 * Once you have access to an aafiAudioEssenceFile, you can access its audio
 * informations (aafiAudioEssenceFile.type, aafiAudioEssenceFile.samplerate,
 * aafiAudioEssenceFile.samplesize, aafiAudioEssenceFile.channels, aafiAudioEssenceFile.framelength,
 * etc.). You can also check if the file is embedded or not with aafiAudioEssenceFile.is_embedded.
 *
 * Access external audio essence file
 * ==================================
 *
 * When an audio file is stored outside the AAF file, libaaf tries to find it.
 *
 * If libaaf is able to locate the file, it sets aafiAudioEssenceFile.usable_file_path
 * with the absolute file path.
 *
 * @note When using **libsndfile**, it is possible to open a `SNDFILE` for reading
 * both embedded and external essence file using aafi_sf_open_audioEssenceFile(),
 * see below.
 *
 * Access embedded audio essence file
 * ==================================
 * Embedded audio essence file extraction
 * --------------------------------------
 *
 * When an audio file is embedded, it can easily be extracted using one of the
 * following functions.
 *
 * aafi_extract_original_audioEssenceFile() is used to extract essence file, as
 * an exact byte-for-byte copy of the original file.
 *
 * aafiAudioEssenceFile.usable_file_path is set on success with the absolute file
 * path to the extracted file.
 *
 * @code{.c}
const char *outpath = "/output/path/";
const char *rename = NULL;
int extractRawPCM = 0;

if ( aafi_extract_original_audioEssenceFile( aafi, audioEssenceFile, outpath, rename, extractRawPCM ) < 0 ) {
	printf( "Failed to extract file %s", audioEssenceFile->name );
}
else {
	printf( "File '%s' successfully extracted.", audioEssenceFile->usable_file_path );
}
 * @endcode
 *
 * aafi_extract_audioEssenceFile() is used to extract essence file, with some
 * formatting and clipping options.
 *
 * aafiAudioEssenceFile.usable_file_path is set on success with the absolute file
 * path to the extracted file, only if full essence file is extracted (no
 * **frameOffset** nor **frameLength** value set).
 *
 * In all cases, you still can pass a **usableFilePath** pointer to get back the
 * absolute file path.
 *
 * @note aafi_extract_audioEssenceFile() requires libaaf to be compiled with
 * `-DHAVE_SNDFILE`
 *
 * @code{.c}
const char *outpath = "/output/path/";
const char *rename = NULL;
uint64_t frameOffset = -1; // 0 or -1 sets no frameOffset
uint64_t frameLength = -1; // 0 or -1 sets no frameLength
enum aafiExtractFormat extractFormat = AAFI_EXTRACT_DEFAULT;
int extractSamplerate = -1; // 0 or -1 sets source samplerate
int extractSamplesize = -1; // 0 or -1 sets source samplesize
char *usableFilePath = NULL;

if ( aafi_extract_audioEssenceFile( aafi, audioEssenceFile, outpath, rename, frameOffset, frameLength, extractFormat, extractSamplerate, extractSamplesize, &usableFilePath ) < 0 ) {
	printf( "Failed to extract file %s", audioEssenceFile->name );
}
else {
	printf( "File '%s' successfully extracted.", usableFilePath );
	free( usableFilePath );
}
 * @endcode
 *
 * Embedded audio essence file read
 * --------------------------------
 *
 * It is possible to read an embedded audio file directly from the AAF, without
 * extracting it.
 *
 * @code{.c}
size_t filesize = aafi_size_embeddedAudioEssenceFile( audioEssenceFile );
ssize_t bytesread = 0;
char but[4096];

ssize_t currentpos = aafi_seek_embeddedAudioEssenceFile( audioEssenceFile, SEEK_SET, 0 );

while ( (bytesread = aafi_read_embeddedAudioEssenceFile( audioEssenceFile, buf, 4096 )) > 0 ) {
	// currentpos = aafi_tell_embeddedAudioEssenceFile( audioEssenceFile );
	// ...
}
 * @endcode
 *
 *
 * Read embedded & external with libsndfile
 * ========================================
 *
 * Libaaf provides a function to abstract the opening of a `SNDFILE`, for reading
 * both embedded and external essence files.
 *
 * All it takes is substitute **sf_open()** / **sf_open_virtual()** with
 * aafi_sf_open_audioEssenceFile(). Libaaf will set the `SF_VIRTUAL_IO` structure
 * when reading embedded, prepare `SF_INFO` structure when essence file is raw PCM
 * and handle proper file opening.
 *
 * @code{.c}
SF_INFO sfinfo;

SNDFILE *file = aafi_sf_open_audioEssenceFile( aafi, audioEssenceFile, &sfinfo );
// ...
aafi_sf_close( sndfile );
 * @endcode
 *
 * As an alternative for reading embedded files, you can use libsndfile-ready
 * callbacks if you prefer handling the opening yourself. Keep in mind that
 * `SF_INFO` will have to be set properly when essence file type is raw PCM.
 *
 * @code{.c}
SF_INFO sfinfo;
SF_VIRTUAL_IO sfvirtual;

sfvirtual.get_filelen = aafi_sf_size_embeddedAudioEssenceFile;
sfvirtual.seek = aafi_sf_seek_embeddedAudioEssenceFile;
sfvirtual.read = aafi_sf_read_embeddedAudioEssenceFile;
sfvirtual.tell = aafi_sf_tell_embeddedAudioEssenceFile;
sfvirtual.write = NULL;

SNDFILE *sndfile = sf_open_virtual( &sfvirtual, SFM_READ, &sfinfo, audioEssenceFile );
 * @endcode
 *
 * @{
 */

/**
 * @brief Possible values for aafiAudioEssenceFile.type
 */

enum aafiEssenceType {
	AAFI_ESSENCE_TYPE_PCM  = 0x01,
	AAFI_ESSENCE_TYPE_WAVE = 0x02,
	AAFI_ESSENCE_TYPE_AIFC = 0x03,
	AAFI_ESSENCE_TYPE_BWAV = 0x04,
	AAFI_ESSENCE_TYPE_UNK  = 0xff /* non-pcm */
};

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



/**
 * Describe a single audio essence file.
 */

typedef struct aafiAudioEssenceFile
{
	/**
	 * Holds the name of essence file, as specified in `MasterMob::Name`. This is
	 * usually the name exposed in the original software. This name *might* not be
	 * unique accross all essences. For a unique name, use aafiAudioEssenceFile.unique_name
	 * instead.
	 */

	char *name;

	/**
	 * This is the same as aafiAudioEssenceFile.name, but this one is guaranted to
	 * be unique. If an AAF has different essence files sharing the same name,
	 * then libaaf will add a suffix to **unique_name**. Thus, **unique_name** can
	 * be used instead of **name** when exporting embedded essences.
	 */

	char *unique_name;

	/**
	 * Holds a URL/URI (RFC 1738 or RFC 2396) targetting essence file, coming from
	 * `NetworkLocator::URLString`.
	 *
	 * Stored string is *not* percent-decoded.
	 */

	char *original_file_path;

	/**
	 * Holds a local, absolute filepath to the essence file.
	 *
	 * If essence file is not embedded, then libaaf tries to locate it and sets
	 * **usable_file_path** on success. **usable_file_path** remains NULL otherwise.
	 *
	 * If essence file is embedded, **usable_file_path** is NULL until essence file
	 * gets extracted, either by aafi_extract_original_audioEssenceFile() or
	 * aafi_extract_audioEssenceFile()
	 */

	char *usable_file_path;

	/**
	 * Is set based on the AAF class describing the essence: PCMDescriptor, WAVEDescriptor, AIFCDescriptor.
	 */

	enum aafiEssenceType type;

	/**
	 * Flag to tell if essence data is embedded or external. It is set only if
	 * FileSourceMob contains EssenceData.
	 */

	uint8_t is_embedded;

	/**
	 * Total audio frame count.
	 *
	 * Might be retrieved from `FileDescriptor::Length`, or from the parsing of
	 * `WAVEDescriptor::Summary`, `AIFCDescriptor::Summary` embedded or external
	 * essence file.
	 *
	 * When retrieved from essence summary or file, **framelength** is obtained with
	 * the following formula `data chunk size / channels / samplesize / 8`
	 */

	aafPosition_t framelength;

	/**
	 * Audio essence file channel count.
	 */

	uint16_t channels;

	/**
	 * Audio essence file sample rate
	 */

	uint32_t samplerate;

	/**
	 * Audio essence file sample rate as a rational number
	 */

	aafRational_t *samplerateRational;

	/**
	 * Audio essence file sample size
	 */

	uint16_t samplesize;

	/**
	 * Metadata retrieved from main `MasterMob::UserComments`
	 */

	aafiMetaData *metadata;

	/**
	 * Allow user to attach custom data to essence file.
	 */

	void *user;


	/* *** private *** */

	aafProperty *summary; /* Buffer storing audio file header, from `WAVEDescriptor::Summary` or AIFCDescriptor::Summary`. NULL if essence is raw PCM. */
	uint64_t pcmAudioStreamOffset; /* Offset in the file that points to the begining of PCM audio stream. */

	cfbNode *dataStreamNode; /* Data stream CFB node, containing essence data if it's embedded. */

	aafMobID_t *sourceMobID; /* SourceMob `Mob::ID` -> BEXT.umid */
	aafSlotID_t sourceMobSlotID; /* SourceMob's `MobSlot::SlotID` */
	aafPosition_t sourceMobSlotOrigin; /* SourceMob's `TimelineMobSlot::Origin` -> BEXT.time_reference */
	aafRational_t *sourceMobSlotEditRate; /* SourceMob's `TimelineMobSlot::EditRate` used to interpret sourceMobSlotOrigin */
	aafTimeStamp_t *sourceMobCreationTime; /* `SourceMob::CreationTime` -> BEXT.origination_date and BEXT.origination_time */

	aafMobID_t *masterMobID; /* MasterMob `Mob::ID` */
	aafSlotID_t masterMobSlotID; /* MasterMob `MobSlot::SlotID` */

	CFBStreamDescriptor *sd;

	struct AAF_Iface *aafi;

	struct aafiAudioEssenceFile *next;

} aafiAudioEssenceFile;



/**
 * aafiAudioEssencePointer is used by aafiAudioClip, to points to one or more
 * aafiAudioEssenceFile and specify which channel of essence file the clip uses.
 */

typedef struct aafiAudioEssencePointer
{
	aafiAudioEssenceFile *essenceFile;
	uint32_t essenceChannel; /* channel selector inside multichannel essence. If zero, then all essence channels must be used. */

	/**
	 * Allow user to attach custom data to essence pointer.
	 */

	void *user;

	struct AAF_Iface *aafi;

	struct aafiAudioEssencePointer *next; /* List: audioClip.essencePointerList */

	/* those are used to track all structures for freeing */
	struct aafiAudioEssencePointer *allocPrev; /* List: aafi.essencePointerList */
	struct aafiAudioEssencePointer *allocNext; /* List: aafi.essencePointerList */

} aafiAudioEssencePointer;



typedef struct aafiVideoEssenceFile
{
	char          *original_file_path; // NetworkLocator::URLString should point to original essence file if external (and in some cases, points to the AAF itself if internal..)
	char          *usable_file_path; // Holds the file path, once the essence has been exported, copied or linked.
	char          *name; // MasterMob::Name -> file name
	char          *unique_name;	// unique name generated from aafiVideoEssenceFile->name. Sometimes, multiple files share the same names so this unique name should be used on export.

	aafPosition_t  length; // Length of Essence Data

	cfbNode       *node; // The node holding the audio stream if embedded

	aafRational_t *framerate;

	aafMobID_t    *sourceMobID;	// Holds the SourceMob Mob::ID references this EssenceData
	uint32_t       sourceMobSlotID; // SlotID of the MobSlot inside MasterMob (CompoMob's Sequence SourceClip::SourceMobSlotID)
	aafMobID_t    *masterMobID;	// Holds the MasterMob Mob::ID (used by CompoMob's Sequence SourceClip::SourceID)
	uint32_t       masterMobSlotID; // SlotID of the MobSlot inside MasterMob (CompoMob's Sequence SourceClip::SourceMobSlotID)

	aafObject     *SourceMob;

	uint8_t        is_embedded;

	/*
	 * Metadata retrieved from main MasterMob::UserComments
	 */

	aafiMetaData  *metadata;

	struct aafiVideoEssenceFile *next;

} aafiVideoEssenceFile;


/**
 * Loops through each essence pointer in an aafiAudioClip.
 *
 * @param clip            Pointer to an aafiAudioClip struct list.
 * @param essencePointer  NULL pointer to an aafiAudioEssencePointer struct.
 */

#define AAFI_foreachClipEssencePointer( clip, essencePointer ) \
  for ( essencePointer = clip->essencePointerList;             \
        essencePointer != NULL;                                \
        essencePointer = essencePointer->next )                \

/**
 * Loops through each essence pointer in a list.
 *
 * @param essencePointerList Pointer to an aafiAudioEssencePointer struct list.
 * @param essencePointer     NULL pointer to an aafiAudioEssencePointer struct.
 */

#define AAFI_foreachEssencePointer( essencePointerList, essencePointer ) \
  for ( essencePointer = essencePointerList;                             \
        essencePointer != NULL;                                          \
        essencePointer = essencePointer->next )                          \

/**
 * Loops through each audio essence file in AAF file.
 *
 * @param aafi              Pointer to the current AAF_Iface struct.
 * @param audioEssenceFile  NULL pointer to an aafiAudioEssenceFile struct.
 */

#define AAFI_foreachAudioEssenceFile( aafi, audioEssenceFile ) \
  for ( audioEssenceFile = aafi->Audio->essenceFiles;          \
        audioEssenceFile != NULL;                              \
        audioEssenceFile = audioEssenceFile->next )            \

/**
 * Loops through each video essence file in AAF file.
 *
 * @param aafi          Pointer to the current AAF_Iface struct.
 * @param videoEssenceFile  NULL pointer to an aafiVideoEssenceFile struct.
 */

#define AAFI_foreachVideoEssenceFile( aafi, videoEssenceFile ) \
  for ( videoEssenceFile = aafi->Video->essenceFiles;          \
        videoEssenceFile != NULL;                              \
        videoEssenceFile = videoEssenceFile->next )            \

/**
 * Loops through each essence file in a video or audio list.
 *
 * @param essenceFileList  Pointer to an aafiAudioEssenceFile or aafiVideoEssenceFile struct list.
 * @param essenceFile      NULL pointer to an aafiAudioEssenceFile or aafiVideoEssenceFile struct.
 */

#define AAFI_foreachEssenceFile( essenceFileList, essenceFile ) \
  for ( essenceFile = essenceFileList;                          \
        essenceFile != NULL;                                    \
        essenceFile = essenceFile->next )                       \



/**
 * Get the size of an embedded audio essence file stream.
 *
 * @param audioEssenceFile  Pointer to the aafiAudioEssenceFile.
 * @return                  The size of the embedded audio essence file stream, in bytes.
 */

size_t  aafi_size_embeddedAudioEssenceFile( aafiAudioEssenceFile *audioEssenceFile );

/**
 * Get the current read position into an embedded audio essence file stream.
 *
 * @param audioEssenceFile  Pointer to the aafiAudioEssenceFile.
 * @return                  The current position in the stream, in bytes.
 */

size_t  aafi_tell_embeddedAudioEssenceFile( aafiAudioEssenceFile *audioEssenceFile );

/**
 * Set the read position into an embedded audio essence file stream.
 *
 * @param pos               Byte offset to add to the position specified by whence.
 * @param whence            Can be SEEK_SET, SEEK_CUR or SEEK_END.
 * @param audioEssenceFile  Pointer to the aafiAudioEssenceFile.
 *
 * @return                  The current position in the stream, in bytes, or -1 if an error occured.
 */

ssize_t aafi_seek_embeddedAudioEssenceFile( aafiAudioEssenceFile *audioEssenceFile, int whence, int64_t pos );

/**
 * Read nbyte of an embedded audio essence file stream, into buf.
 *
 * @param buf               Pointer to a buffer with at least room for nbyte.
 * @param nbyte             number of bytes to read from audio essence file stream.
 * @param audioEssenceFile  Pointer to the aafiAudioEssenceFile.
 *
 * @return                  The total bytes read, or -1 if an error occured.
 */

ssize_t aafi_read_embeddedAudioEssenceFile( aafiAudioEssenceFile *audioEssenceFile, void* buf, size_t nbyte );

#ifdef HAVE_SNDFILE

/**
 * @note Requires libaaf to be compiled with `-DHAVE_SNDFILE`
 *
 * Open an **embedded** or **external** audio essence file using `sf_open_virtual()`,
 * `sf_open()` or `sf_wchar_open()`.
 *
 * Function handles the fill of `SF_INFO` structure when essence is a raw PCM file.
 *
 * Opened `SNDFILE` must be closed using aafi_sf_close()
 *
 * @param aafi              Pointer to the current AAF_Iface struct.
 * @param audioEssenceFile  Pointer to the aafiAudioEssenceFile to open.
 * @param sfinfo            Pointer to the SF_INFO structure.
 *
 * @return                  The opened SNDFILE on success, NULL on error
 */

SNDFILE * aafi_sf_open_audioEssenceFile( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile, SF_INFO *sfinfo );

/**
 * @note Requires libaaf to be compiled with `-DHAVE_SNDFILE`
 *
 * Close a **SNDFILE** that was opened with aafi_sf_open_audioEssenceFile()
 *
 * @param sndfile  Pointer to a previously opened SNDFILE.
 * @return         0 on success, a libsndfile error value otherwise
 */

int aafi_sf_close( SNDFILE *sndfile );

/**
 * @note Requires libaaf to be compiled with `-DHAVE_SNDFILE`
 *
 * libsndfile-ready function to get the size of an embedded audio essence file
 * stream.
 *
 * This function is to be assigned to libsndfile's `SF_VIRTUAL_IO.get_filelen`
 *
 * @param audioEssenceFile  Pointer to the aafiAudioEssenceFile.
 * @return                  The size of the embedded audio essence file stream, in bytes.
 */

sf_count_t aafi_sf_size_embeddedAudioEssenceFile( void *audioEssenceFile );

/**
 * @note Requires libaaf to be compiled with `-DHAVE_SNDFILE`
 *
 * libsndfile-ready function to get the current read position into an embedded
 * audio essence file stream.
 *
 * This function is to be assigned to libsndfile's `SF_VIRTUAL_IO.tell`
 *
 * @param audioEssenceFile  Pointer to the aafiAudioEssenceFile.
 * @return                  The current position in the stream, in bytes.
 */

sf_count_t aafi_sf_tell_embeddedAudioEssenceFile( void *audioEssenceFile );

/**
 * @note Requires libaaf to be compiled with `-DHAVE_SNDFILE`
 *
 * libsndfile-ready function to set the read position into an embedded audio
 * essence file stream.
 *
 * This function is to be assigned to libsndfile's `SF_VIRTUAL_IO.seek`
 *
 * @param pos               Byte offset to add to the position specified by whence.
 * @param whence            Can be SEEK_SET, SEEK_CUR or SEEK_END.
 * @param audioEssenceFile  Pointer to the aafiAudioEssenceFile.
 *
 * @return                  The current position in the stream, in bytes, or -1 if an error occured.
 */

sf_count_t aafi_sf_seek_embeddedAudioEssenceFile( sf_count_t pos, int whence, void *audioEssenceFile );

/**
 * @note Requires libaaf to be compiled with `-DHAVE_SNDFILE`
 *
 * libsndfile-ready function to read nbyte of an embedded audio essence file
 * stream, into buf.
 *
 * This function is to be assigned to libsndfile's `SF_VIRTUAL_IO.read`
 *
 * @param buf               Pointer to a buffer with at least room for nbyte.
 * @param nbyte             number of bytes to read from audio essence file stream.
 * @param audioEssenceFile  Pointer to the aafiAudioEssenceFile.
 *
 * @return                  The total bytes read, or -1 if an error occured.
 */

sf_count_t aafi_sf_read_embeddedAudioEssenceFile( void* buf, sf_count_t nbyte, void *audioEssenceFile );

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
 * If function is called with frameOffset <= 0 and frameLength <= 0, (i.e. if function
 * extracts an entire essence file) then aafiAudioEssenceFile.usable_file_path will
 * be set on success.
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

#endif // HAVE_SNDFILE

/**
 * Extract audio essence file as it is embedded in AAF file (WAV, BWAV, AIFF or raw
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

/**
 * @}
 */

/**
 * @ingroup AAFIface
 * @{
 */

aafiAudioEssencePointer * aafi_newAudioEssenceFilePointer( AAF_Iface *aafi, aafiAudioEssencePointer **list, aafiAudioEssenceFile *audioEssenceFile, uint32_t *essenceChannelNum );

aafiAudioEssenceFile * aafi_newAudioEssenceFile( AAF_Iface *aafi );

aafiVideoEssenceFile * aafi_newVideoEssenceFile( AAF_Iface *aafi );


aafiAudioEssencePointer * aafi_freeAudioEssencePointer( aafiAudioEssencePointer *audioEssencePointer );

aafiAudioEssenceFile * aafi_freeAudioEssenceFile( aafiAudioEssenceFile *audioEssenceFile );

aafiVideoEssenceFile * aafi_freeVideoEssenceFile( aafiVideoEssenceFile *videoEssenceFile );


char * aafi_locate_external_audioEssenceFile( AAF_Iface *aafi, const char *original_uri_filepath, const char *search_location );

int aafi_build_audioEssenceFileUniqueName( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile );

char * aafi_build_audioEssenceFilePath( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile, const char *outpath, const char *rename, int format, int clean_channel_identif );

int aafi_parse_audioEssenceFile( AAF_Iface *aafi, aafiAudioEssenceFile *audioEssenceFile );

int aafi_getAudioEssencePointerChannelCount( struct aafiAudioEssencePointer *essencePointerList );

aafiAudioEssencePointer * aafi_audioEssencePointer_exists_before( AAF_Iface *aafi, aafiAudioEssencePointer *audioEssencePointerList );

const char * aafi_extractFormatToText( enum aafiExtractFormat fmt );

/**
 * @}
 */

#endif // !__AAFIEssenceFile_h__

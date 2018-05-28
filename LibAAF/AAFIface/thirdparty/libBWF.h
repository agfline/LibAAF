#ifndef __libBWF_h__
#define __libBWF_h__

#include <stdint.h>
#include <uuid/uuid.h>






/**
 *	A data block unit of a RIFF/RF64 file.
 */

typedef struct chunk
{
	char     id[4];
	uint32_t sz;

	unsigned char data[];

}__attribute__((__packed__)) chunk;







/**
 *	@addtogroup WAVE
 *	@{
 *
 *	## Format Chunk
 *
 *	The Format chunk is mandatory and should be placed before the data chunk in the RIFF 
 *	file. It should carry all the metadata used to describe and later process audio data. 
 *	There are 3 types of format chunk :
 *
 *	- **PCM audio** format chunk,
 *	- **Multiple channels** (> 2) format chunk,
 *	- **Non-PCM** audio format chunk.
 *
 *	No matter what format chunk is used, they all begin with the same five fields :
 *	**wFormatTag**, **wChannels**, **dwSamplesPerSec**, **dwAvgBytesPerSec**, 
 *	and **wBlockAlign**. The first field (*wFormatTag*) is used to retrieve the audio data 
 *	type. Therefore, before choosing a structure when parsing, one should cast the 
 *	chunk.data with uint16_t and test its value, so we know what structure to use, and 
 *	only then we can cast the chunk.data with the suitable struct.
 *
 *	**Example :**
 *	@code
	chunk *ck = get_ck_by_id( "fmt " );

	if ( *(uint16_t*)ck->data == WAVE_FORMAT_PCM )
		waveFmtPCM *fmt = ck->data;
	else if ( *(uint16_t*)ck->data == WAVE_FORMAT_EXTENSIBLE )
		waveFmtExt *fmt = ck->data;
	else if ( *(uint16_t*)ck->data == WAVE_FORMAT_MPEG )
		waveFmtMPEG *fmt = ck->data;
 *	@endcode
 *
 *	When working with one or two channels **PCM audio**, one should use the waveFmtPCM 
 *	structure.
 *
 *	@}
 */

/**
 *	Expands to set every fmt chunk data structure with the first common data fields.
 */

#define WAVEFORMAT_HEAD                                                                    \
	/**
	 	A number indicating the WAVE format category of the file. The content of the
	 	\<format-specific-fields\> portion of the ‘fmt’ chunk, and the interpretation of the
	 	waveform data, depend on this value.
	 */                                                                                    \
                                                                                           \
	uint16_t      wFormatTag;                                                              \
                                                                                           \
	/**
	 	The number of channels represented in the waveform data, such as 1 for mono or 2 for
	 	stereo.
	 */                                                                                    \
                                                                                           \
	uint16_t      wChannels;                                                               \
                                                                                           \
	/**
	 	The sampling rate (in samples per second) at which each channel should be played.
	 */                                                                                    \
                                                                                           \
	uint32_t      dwSamplesPerSec;                                                         \
                                                                                           \
	/**
	 	The average number of bytes per second at which the waveform data should be 
	 	transferred. Playback software can estimate the buffer size using this value.
	 */                                                                                    \
                                                                                           \
	uint32_t      dwAvgBytesPerSec;                                                        \
                                                                                           \
	/**
	 	The block alignment (in bytes) of the waveform data. Playback software needs to 
	 	process a multiple of wBlockAlign bytes of data at a time, so the value of 
	 	wBlockAlign can be used for buffer alignment.
	 */                                                                                    \
                                                                                           \
	uint16_t      wBlockAlign;                                                             \
                                                                                           \





/**
 *	This structure defines the content of the fmt chunk, that is the fmt chunk.data field.
 *
 *	If the wFormatTag is different than WAVE_FORMAT_PCM, the structure shall have some 
 *	extension bytes at the end (waveFmt.ext). Those bytes can be casted with the waveFmtExt
 *	structure.
 *
 *	If the wFormatTag is WAVE_FORMAT_EXTENSIBLE, then the waveFmt.ext should be casted with
 *	the wfx_EXT structure. The WAVE_FORMAT_EXTENSIBLE is used when the file has more than
 *	two audio channels, so the wfx_EXT.SubFormat shall be used to set the audio format
 *	instead of waveFmt.wFormatTag.
 *
 *	For all other wFormatTag values, that is for all non-pcm audio format, the waveFmt.ext
 *	shall be casted with either waveFmtExt or any specific structure. Note that in each
 *	case, the waveFmtExt always match.
 */

typedef struct waveFmt
{
	WAVEFORMAT_HEAD;

}__attribute__((__packed__)) waveFmt;




typedef struct waveFmtPCM
{
	WAVEFORMAT_HEAD;

	/**
	 *	The number of bits of data used to represent each sample of each channel. If there 
	 *	are multiple channels, the sample size is the same for each channel.
	 */

	uint16_t  wBitsPerSample;

}__attribute__((__packed__)) waveFmtPCM;




// WINDOWS ?
#ifdef GUID
	typedef GUID uuid_t;
#endif

typedef struct waveFmtExtensible
{
	WAVEFORMAT_HEAD;

	/**
	 *	The number of bits of data used to represent each sample of each channel. If there 
	 *	are multiple channels, the sample size is the same for each channel.
	 */

	uint16_t     wBitsPerSample;

	uint16_t     cbSize;


	union
	{
		uint16_t wValidBitsPerSample;

		uint16_t wSamplesPerBlock;

		uint16_t wReserved;

	} Samples;

	uint32_t     dwChannelMask;

	uuid_t       SubFormat;

}__attribute__((__packed__)) waveFmtExt;





typedef struct waveFmtMPEG
{
	WAVEFORMAT_HEAD;

	/**
	 *	The number of bits of data used to represent each sample of each channel. If there 
	 *	are multiple channels, the sample size is the same for each channel.
	 */

	uint16_t  wBitsPerSample;

	uint16_t  cbSize;


	uint16_t  fwHeadLayer;

	uint32_t  dwHeadBitrate;

	uint16_t  fwHeadMode;

	uint16_t  fwHeadModeExt;

	uint16_t  wHeadEmphasis;

	uint16_t  fwHeadFlags;

	uint32_t  dwPTSLow;

	uint16_t  dwPTSHigh;

}__attribute__((__packed__)) waveFmtMPEG;







typedef struct bwfBEXT
{
	/**
	 *	ASCII description of the sound sequence.
	 *
	 *	ASCII string (maximum 256 characters) containing a free description of the sequence.
	 *	To help applications which display only a short description, it is recommended that 
	 *	a resume of the description is contained in the first 64 characters and the last 
	 *	192 characters are used for details.
	 *
	 *	If the length of the string is less than 256 characters the last one shall be 
	 *	followed by a null character (00).
	 */

	char          Description[256];

	/**
	 *	ASCII name of the originator.
	 *
	 *	ASCII string (maximum 32 characters) containing the name of the originator/producer 
	 *	of the audio file. If the length of the string is less than 32 characters the field 
	 *	shall be ended by a null character.
	 */

	char          Originator[32];

	/**
	 *	ASCII reference of the originator.
	 *
	 *	ASCII string (maximum 32 characters) containing an unambiguous reference allocated 
	 *	by the originating organisation. If the length of the string is less than 32 
	 *	characters the field shall be terminated by a null character.
	 *
	 *	@note The EBU has defined a format for the OriginatorReference field. 
	 *	      See EBU Recommendation R-99.
	 */

	char          OriginatorReference[32];

	/**
	 *	ASCII creation date : **yyyy:mm:dd**
	 *
	 *	10 ASCII characters containing the date of creation of the audio sequence. The 
	 *	format shall be « ‘,year’,-,’month,’-‘,day,’» with 4 characters for the year and 
	 *	2 characters per other item.
	 *
	 *	- Year is defined from 0000 to 9999 
	 *	- Month is defined from 1 to 12 
	 *	- Day is defined from 1 to 28, 29, 30 or 31
	 *
	 *	The separator between the items can be anything but it is recommended that one of 
	 *	the following characters be used :
	 *
	 *	‘-’  hyphen  ‘_’  underscore  ‘:’  colon  ‘ ’  space  ‘.’  stop
	 */

	char          OriginationDate[10];

	/**
	 *	ASCII creation time : **hh:mm:ss**
	 *
	 *	8 ASCII characters containing the time of creation of the audio sequence. The format
	 *	shall be « ‘hour’-‘minute’-‘second’» with 2 characters per item.
	 *
	 *	- Hour is defined from 0 to 23. 
	 *	- Minute and second are defined from 0 to 59.
	 *
	 *	The separator between the items can be anything but it is recommended that one of 
	 *	the following characters be used :
	 *
	 *	‘-’  hyphen  ‘_’  underscore  ‘:’  colon  ‘ ’  space  ‘.’  stop
	 */

	char          OriginationTime[8];

	/**
	 *	First sample count since midnight, low word.
	 *
	 *	These fields shall contain the time-code of the sequence. It is a 64-bit value which
	 *	contains the first sample count since midnight. The number of samples per second 
	 *	depends on the sample frequency which is defined in the field \<nSamplesPerSec\>
	 *	from the \<format chunk\>.
	 */

	uint32_t      TimeReferenceLow;

	/**
	 *	First sample count since midnight, high word.
	 */

	uint32_t      TimeReferenceHigh;

	/**
	 *	Version of the BWF.
	 *
	 *	An unsigned binary number giving the version of the BWF. This number is particularly
	 *	relevant for the carriage of the UMID and loudness information. For Version 1 it 
	 *	shall be set to 0001h and for Version 2 it shall be set to 0002h. 
	 */

	uint16_t      Version;

	/* ****************************** since bext v1 (2001) ****************************** */

	/**
	 *	SMPTE UMID.
	 *
	 *	64 bytes containing a UMID (Unique Material Identifier) to standard SMPTE 330M [1]. 
	 *	If only a 32 byte "basic UMID" is used, the last 32 bytes should be set to zero. 
	 *	(The length of the UMID is given internally.)
	 */

	unsigned char UMID[64];

	/* ****************************** since bext v2 (2011) ****************************** */

	/**
	 *	Integrated Loudness Value of the file in LUFS (multiplied by 100).
	 *
	 *	values : 0xd8f1 - 0xffff (-99.99 -0.1) and 0x000 0x270f (0.00 99.99).
	 *
	 *	Any value outside valid ranges shall be ignored. If unused, the value shall be set
	 *	to 0x7fff.
	 */

	int16_t       LoudnessValue;

	/**
	 *	Loudness Range of the file in LU (multiplied by 100).
	 *
	 *	values : 0x000 0x270f (0.00 99.99).
	 *
	 *	Any value outside valid range shall be ignored. If unused, the value shall be set
	 *	to 0x7fff.
	 */

	int16_t       LoudnessRange;

	/**
	 *	Maximum True Peak Level of the file expressed as dBTP (multiplied by 100).
	 *
	 *	Range : 0xd8f1 - 0xffff (-99.99 -0.1) and 0x000 0x270f (0.00 99.99).
	 *
	 *	Any value outside valid ranges shall be ignored. If unused, the value shall be set
	 *	to 0x7fff.
	 */

	int16_t       MaxTruePeakLevel;

	/**
	 *	Highest value of the Momentary Loudness Level of the file in LUFS (multiplied 
	 *	by 100).
	 *
	 *	Range : 0xd8f1 - 0xffff (-99.99 -0.1) and 0x000 0x270f (0.00 99.99).
	 *
	 *	Any value outside valid ranges shall be ignored. If unused, the value shall be set
	 *	to 0x7fff.
	 */

	int16_t       MaxMomentaryLoudness;

	/**
	 *	Highest value of the Short-Term Loudness Level of the file in LUFS (multiplied 
	 *	by 100).
	 *
	 *	Range : 0xd8f1 - 0xffff (-99.99 -0.1) and 0x000 0x270f (0.00 99.99).
	 *
	 *	Any value outside valid ranges shall be ignored. If unused, the value shall be set
	 *	to 0x7fff.
	 */

	int16_t       MaxShortTermLoudness;

	/**
	 *	180 bytes, reserved for future use, set to zero.
	 */

	char          Reserved[180];

	/**
	 *	Unrestricted ASCII characters containing a collection of strings terminated by 
	 *	CR/LF. Each string shall contain a description of a coding process applied to the 
	 *	audio data. Each new coding application shall add a new string with the appropriate 
	 *	information.
	 *
	 *	This information shall contain the type of sound (PCM or MPEG) with its specific 
	 *	parameters : 
	 *
	 *	- PCM : mode (mono, stereo), size of the sample (8, 16 bits) and sample 
	 *	  frequency,
	 *
	 *	- MPEG : sample frequency, bit rate, layer (I or II) and the mode (mono, stereo,
	 *	  joint stereo or dual channel), 
	 *
	 *	It is recommended that the manufacturers of the coders provide an ASCII string for 
	 *	use in the coding history.
	 *
	 *	@note The EBU has defined a format for coding history which will simplify the
	 *		  interpretation of the information provided in this field. See EBU 
	 *		  Recommendation R-98.
	 */

	char          CodingHistory[];

} __attribute__((__packed__)) bwfBEXT;





//typedef struct quality_chunk {
//	uint32_t FileSecurityReport; /* FileSecurityCode of quality report */
//	uint32_t FileSecurityWave; /* FileSecurityCode of BWF wave data */
//	char BasicData[ ]; /* ASCII: << Basic data >> */
//	char StartModulation[]; /* ASCII: << Start modulation data >> */
//	char QualityEvent[ ]; /* ASCII: << Quality event data >> */
//	char QualityParameter[ ]; /* ASCII: << Quality parameter data >> */
//	char EndModulation[]; /* ASCII: << End modulation data >> */
//	char QualityParameter[ ]; /* ASCII: << Quality parameter data >> */
//	char OperatorComment[ ]; /* ASCII: << Comments of operator >> */
//	char CueSheet[]; /* ASCII: << Cue sheet data >> */
//} bwfQuality;





typedef struct ChunkSize64
{
	/**
	 *	chunk ID (i.e. “big1” – this chunk is a big one)
	 */

	char chunkId[4];

	/**
	 *	low 4 byte chunk size
	 */

	uint32_t chunkSizeLow;

	/**
	 *	high 4 byte chunk size
	 */

	uint32_t chunkSizeHigh;

}__attribute__((__packed__)) cs64_t;


typedef struct DataSize64Chunk
{
	/**
	 *	low 4 byte size of RF64 block
	 */

	uint32_t riffSizeLow;

	/**
	 *	high 4 byte size of RF64 block
	 */

	uint32_t riffSizeHigh;

	/**
	 *	low 4 byte size of data chunk
	 */

	uint32_t dataSizeLow;

	/**
	 *	high 4 byte size of data chunk
	 */

	uint32_t dataSizeHigh;

	/**
	 *	low 4 byte sample count of fact chunk
	 */

	uint32_t sampleCountLow;

	/**
	 *	high 4 byte sample count of fact chunk
	 */

	uint32_t sampleCountHigh;

	/**
	 *	number of valid entries in array “table”
	 */

	uint32_t tableLength;

	cs64_t   table[];

}__attribute__((__packed__)) ds64;


#endif // !__libBWF_h__

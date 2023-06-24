#ifndef __RIFFParser__
#define __RIFFParser__


struct RIFFAudioFile {
  /* common to wave/aiff */
  uint32_t sampleRate;
  uint16_t sampleSize;
  uint16_t channels;
  uint64_t duration; /* total samples for 1 channel (no matter channel count). (duration / sampleRate) = duration in seconds */
};


struct riffHeaderChunk {
  char ckid[4];
  uint32_t cksz;

  char format[4];
  unsigned char data[];
};

struct riffChunk {
  char ckid[4];
  uint32_t cksz;

  unsigned char data[];
};

struct wavFmtChunk {
	char ckid[4]; //'fmt '
	uint32_t cksz;

	uint16_t format_tag;
	uint16_t channels;
	uint32_t samples_per_sec;
	uint32_t avg_bytes_per_sec;
	uint16_t block_align;
	uint16_t bits_per_sample;
};

struct wavBextChunk {
	char     ckid[4]; //'bext'
	uint32_t cksz;

  char 	   description[256];

	char 	   originator[32];
	char 	   originator_reference[32];
	char 	   origination_date[10];
	char 	   origination_time[8];

	uint64_t time_reference;

	uint16_t version;

	/* 	since bext v1 (2001) 	*/
	unsigned char umid[64];

	/*	since bext v2 (2011)
   *
	 * If any loudness parameter is not
	 * being used,  its  value shall be
	 * set to 0x7fff. Any value outside
	 * valid  ranges  shall be ignored.
	 */
	int16_t  loudness_value;	   // 0xd8f1 - 0xffff (-99.99 -0.1) and 0x000 0x270f (0.00 99.99)
	int16_t  loudness_range;	   // 0x0000 - 0x270f (0.00  99.99)
	int16_t  max_true_peak_level;	   // 0xd8f1 - 0xffff (-99.99 -0.1) and 0x000 0x270f (0.00 99.99)
	int16_t  max_momentary_loudness;   // 0xd8f1 - 0xffff (-99.99 -0.1) and 0x000 0x270f (0.00 99.99)
	int16_t  max_short_term_loudness;  // 0xd8f1 - 0xffff (-99.99 -0.1) and 0x000 0x270f (0.00 99.99)

	char reserved[180];

	/*
		Because it is variable size, we
		do not  include  coding history
		in the bext structure. However,
		we know  it  starts at  the end
		of bext structure when parsing.
	*/

};

struct aiffCOMMChunk {
  char ckid[4]; //'COMM'
  uint32_t cksz;

  uint16_t numChannels;
  uint32_t numSampleFrames;
  uint16_t sampleSize;
  unsigned char sampleRate[10]; // 80 bit IEEE Standard 754 floating point number
};



int riff_parseAudioFile( struct RIFFAudioFile *RIFFAudioFile, size_t (*readerCallback)(unsigned char *, size_t, size_t, void*, void*), void *user1, void *user2 );

int riff_writeWavFileHeader( FILE *fp, struct wavFmtChunk *wavFmt, struct wavBextChunk *wavBext, uint32_t audioDataSize );

#endif // ! __RIFFParser__

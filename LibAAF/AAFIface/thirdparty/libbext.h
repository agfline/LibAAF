/*
 *   This file is part of Wavfix.
 *
 *   Copyright (c) 2016, 2017 Adrien Gesta-Fline
 *
 *   Wavfix is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   any later version.
 *
 *   Wavfix is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with Wavfix. If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _LIBBEXT_H_
#define _LIBBEXT_H_

#include <stdint.h>

#ifndef RIFF_CK_HEADER_SZ
#define RIFF_CK_HEADER_SZ	8
#endif


struct bext {
	char	 ckid[4];
	uint32_t cksz;

	char 	 description[256];

	char 	 originator[32];
	char 	 originator_reference[32];
	char 	 origination_date[10];
	char 	 origination_time[8];

	uint64_t time_reference;

	uint16_t version;

	/* 	since bext v1 (2001) 	*/
	unsigned char 	 umid[64];

	/*	since bext v2 (2011)

		If any loudness parameter is not 
		being used,  its  value shall be 
		set to 0x7fff. Any value outside
		valid  ranges  shall be ignored.
	*/
	int16_t  loudness_value;	   // 0xd8f1 - 0xffff (-99.99 -0.1) and 0x000 0x270f (0.00 99.99)
	int16_t  loudness_range;	   // 0x0000 - 0x270f (0.00  99.99)
	int16_t  max_true_peak_level;	   // 0xd8f1 - 0xffff (-99.99 -0.1) and 0x000 0x270f (0.00 99.99)
	int16_t  max_momentary_loudness;   // 0xd8f1 - 0xffff (-99.99 -0.1) and 0x000 0x270f (0.00 99.99)
	int16_t  max_short_term_loudness;  // 0xd8f1 - 0xffff (-99.99 -0.1) and 0x000 0x270f (0.00 99.99)

	char 	 reserved[180];

	/*
		Because it is variable size, we
		do not  include  coding history
		in the bext structure. However,
		we know  it  starts at  the end
		of bext structure when parsing.
	*/

} __attribute__((__packed__));


/*
	EBU r098-1999 :
	algorithm = { ANALOGUE, PCM, MPEG1L1, 
	MPEG1L2,  MPEG1L3,  MPEG2L1, MPEG2L2, 
	MPEG2L3 }
*/
#define BEXT_CDH_MAX_ALG_SZ		  16

#define BEXT_CDH_MODE_MONO		0x01
#define BEXT_CDH_MODE_DUAL_MONO		0x02
#define BEXT_CDH_MODE_STEREO		0x03
#define BEXT_CDH_MODE_JOINT_STEREO	0x04

struct coding_history {
	char 	 algorithm[BEXT_CDH_MAX_ALG_SZ + 1];
	uint16_t frequency;
	uint16_t bitrate;
	uint8_t	 wordlength;
	uint8_t	 mode;
	char	 *text;

	struct coding_history *next;
};


#define _get_bext_coding_history_addr( bext_addr ) \
	( ((char *)bext_addr)   + sizeof(struct bext) )

#define _get_bext_coding_history_size( cksz ) \
	( (cksz + RIFF_CK_HEADER_SZ) - sizeof(struct bext) )


/**
 * Allocates and creates a new \<bext\> chunk structure. 
 * It must be later freed with free_bext().
 * 
 * @returns A pointer to the bext struct.
 */

struct bext * new_bext( void );

/**
 * Frees a bext structure previously allocated using
 * new_bext().
 *
 * @param b pointer to a bext struct.
 */

struct bext * free_bext( struct bext **b );

/**
 * Allocates and creates a new entry in the coding_history 
 * field. This must be later freed with
 */

struct coding_history * new_bext_coding_history_line( void );
struct coding_history * free_bext_coding_history( struct coding_history **codh_ls );
struct coding_history * add_bext_coding_history_line( struct coding_history **, struct coding_history * );
struct coding_history * get_bext_coding_history_list( struct bext *b );

uint8_t bext_codh_mode_to_chan( uint8_t mode );


#endif

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


#ifndef _LIBWAV_H_
#define _LIBWAV_H_


#include <stdint.h>


#define WAVE_ID		0x45564157

// TODO : Check implementation on microsoft's msdn
// 	  and finally add FULL SUPPORT of non-PCM!
#define WAVE_FORMAT_PCM		    0x0001	// BWF compatible
#define WAVE_FORMAT_IEEE_FLOAT	    0x0003
#define WAVE_FORMAT_ALAW	    0x0006
#define WAVE_FORMAT_MULAW	    0x0007
#define WAVE_FORMAT_MPEG	    0x0050	// BWF compatible
#define WAVE_FORMAT_EXTENSIBLE	    0xfffe

struct fmt {

	char	 ckid[4];
	uint32_t cksz;

	// common fields
	uint16_t format_tag;
	uint16_t channels;
	uint32_t samples_per_sec;
	uint32_t avg_bytes_per_sec;
	uint16_t block_align;

	// PCM Format only
	uint16_t bits_per_sample;

//	char _pad[8];
	// other ?
//	uint16_t cbsize;
//	uint16_t valid_bits_per_sample;
//	uint32_t channel_mask;
//	unsigned char sub_format[16];
} __attribute__((__packed__));


struct fmt * wav_new_fmt( uint16_t format_tag );
struct fmt * wav_free_fmt( struct fmt **fmt );

#endif

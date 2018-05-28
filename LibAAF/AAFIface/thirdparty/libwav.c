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


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "libriff.h"
#include "libwav.h"
#include "libbext.h"


struct fmt * wav_new_fmt( uint16_t format_tag ) {

	/*
		TODO: Ajust fmt size depending on format_tag ?
	*/

	struct fmt *fmt = malloc(sizeof(struct fmt));

	if ( fmt == NULL ) {
		fprintf(stderr, "%s:%d in %s() : %s\n", 
			__FILE__, 
			__LINE__, 
			__func__, 
			strerror(errno));

		return NULL;
	}

	fmt->ckid[0] = 'f';
	fmt->ckid[1] = 'm';
	fmt->ckid[2] = 't';
	fmt->ckid[3] = ' ';

	switch ( format_tag ) {
		case WAVE_FORMAT_PCM:	fmt->cksz = 16;		break;
	}

	fmt->format_tag = format_tag;
	fmt->channels = 0;
	fmt->samples_per_sec = 0;
	fmt->avg_bytes_per_sec = 0;
	fmt->block_align = 0;

	fmt->bits_per_sample = 0;

	return fmt;
}

struct fmt * wav_free_fmt( struct fmt **fmt ) {

	if ( *fmt == NULL )
		return *fmt;

	free( *fmt );

	*fmt = NULL;

	return *fmt;
}



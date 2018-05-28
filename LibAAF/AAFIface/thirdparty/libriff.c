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



struct chunk * new_riff_chunk( void ) {

	struct chunk *ck = malloc(sizeof(struct chunk));

	if ( ck == NULL ) {
		fprintf(stderr, "%s:%d in %s() : %s\n", 
			__FILE__, 
			__LINE__, 
			__func__, 
			strerror(errno));

		return NULL;
	}

	ck->id[0] = 0;
	ck->id[1] = 0;
	ck->id[2] = 0;
	ck->id[3] = 0;

	ck->sz = 0;

	ck->bytes = NULL;
	ck->free_bytes = NULL;

	ck->prev = NULL;
	ck->next = NULL;

	return ck;
}


struct chunk * free_riff_chunk_list( struct chunk **ckls ) {

	struct chunk *ck = *ckls;

	while ( ck != NULL ) {

		*ckls = ck->next;

		if ( ck->free_bytes != NULL && ck->bytes != NULL ) {
			void *data = (ck->bytes - RIFF_CK_HEADER_SZ);
			ck->free_bytes( &data );
		}

		free( ck );

		ck = *ckls;
	}

	return ck;
}


struct chunk * get_riff_chunk_by_id( struct chunk *ckls, const char *ckid ) {

	while ( ckls != NULL && !is_riff_chunk_id( ckls, ckid ) )
		ckls = ckls->next;

	return ckls;
}


struct chunk * get_riff_chunk_list( unsigned char *buf, size_t buf_sz ) {

	struct chunk *ckls = NULL;
	struct chunk *ck   = NULL;

	size_t i = RIFF_HEADER_SZ;

	int junk_count = 0;

	while ( i < buf_sz ) {

		// if is not a valid chunk
		if ( is_riff_chunk( (buf + i), (buf_sz - i) ) == 0 ) {
			junk_count++;
			i++;

			continue;
		}

		// if we have a valid chunk
		if ( junk_count > 0 ) {
			add_riff_null_chunk( &ckls, buf + i, junk_count );
			junk_count = 0;
		}

		ck = new_riff_chunk();

		set_riff_chunk_from_buf( ck, (const char *)buf + i );
		add_riff_chunk( &ckls,  ck );

		/*
			special consideration for <data> chunk: if it's size is garbage 
			bytes, set it to zero. This will make the following bytes to be
			parsed as null bytes, then check_data_chunk(); will rebuild it.
		*/

		if ( ck->id[0] == 'd' &&
		     ck->id[1] == 'a' &&
		     ck->id[2] == 't' &&
		     ck->id[3] == 'a' &&
		     ck->sz + i + RIFF_CK_HEADER_SZ > buf_sz ) {

			ck->sz = 0;
		}


		/* 
			The following issue was found in many Pro Tools session files.

			<DIGI> chunk size says 95 bytes, but is actualy 96 bytes. That
			makes everyting following  that chunk to  be one-byte  shifted
			and miss parsed.

			We DO NOT handle it as a problem since Pro Tools seems to work 
			that way, plus the <DIGI> chunk comes after <fmt > and <data>, 
			so those files should play correctly on any player anyway. 

			However, this can be "fixed" with the following two lines :

			if ( is_riff_chunk_id( ck, "DIGI" ) )
				ck->sz++;
		*/

		i += ck->sz + RIFF_CK_HEADER_SZ;

		// check oddity and padding byte
		if ( ck->sz % 2 && buf[i] == 0x00 )
			i++;

	}

	if ( junk_count > 0 ) {
		add_riff_null_chunk( &ckls, buf + i, junk_count );
		junk_count = 0;
	}

	return ckls;
}


struct chunk * get_last_riff_chunk( struct chunk *ckls ) {

	if ( ckls == NULL )
		return ckls;

	while ( ckls->next != NULL )
		ckls = ckls->next;

	return ckls;
}



int set_riff_chunk_from_buf( struct chunk *ck, const char *buf ) {

	ck->id[0] = buf[0];
	ck->id[1] = buf[1];
	ck->id[2] = buf[2];
	ck->id[3] = buf[3];

	ck->sz = *(uint32_t *)(buf + 4);

	ck->bytes = (unsigned char *)buf + RIFF_CK_HEADER_SZ;

	return 0;
}


int is_riff_chunk_id( struct chunk *ck, const char *ckid ) {

	if ( ck == NULL )
		return 0;

	if ( ck->id[0] == ckid[0] &&
	     ck->id[1] == ckid[1] &&
	     ck->id[2] == ckid[2] &&
	     ck->id[3] == ckid[3] ) {

		return 1;
	}

	return 0;
}


int is_riff_chunk( unsigned char *b, unsigned int b_sz ) {


	/*
		We check that each one of the four presumed chunk-id
		bytes  is  a  valid  ASCII  alphanumeric characters,
		according to the Microsoft/IBM RIFF spec :

		"A  FOURCC  is represented as a  sequence  of one to
		four  ASCII  alphanumeric characters,  padded on the 
		right with blank characters  ( ASCII character value 
		32 ) as required, with no embedded blanks."
	*/

	if ( !( ( /*b[0] == 0x20 ||*/			 // if space 
		  ( b[0] >= 0x30 && b[0] <= 0x39 ) ||	 // if 0 - 9
		  ( b[0] >= 0x41 && b[0] <= 0x5A ) ||	 // if A - Z
		  ( b[0] >= 0x61 && b[0] <= 0x7A ) ) &&  // if a - z
		( b[1] == 0x20 || 
		  ( b[1] >= 0x30 && b[1] <= 0x39 ) || 
		  ( b[1] >= 0x41 && b[1] <= 0x5A ) || 
		  ( b[1] >= 0x61 && b[1] <= 0x7A ) ) && 
		( b[2] == 0x20 || 
		  ( b[2] >= 0x30 && b[2] <= 0x39 ) || 
		  ( b[2] >= 0x41 && b[2] <= 0x5A ) || 
		  ( b[2] >= 0x61 && b[2] <= 0x7A ) ) && 
		( b[3] == 0x20 || 
		  ( b[3] >= 0x30 && b[3] <= 0x39 ) || 
		  ( b[3] >= 0x41 && b[3] <= 0x5A ) || 
		  ( b[3] >= 0x61 && b[3] <= 0x7A ) ) ) ) {

		return 0;
	}


	/*
		If the presumed chunk-id is <data>,  we do not check
		the presumed chunk size.

		Indeed, if the file was  corrupted during recording,
		it  is most likely that  the  <data> size  is either 
		zero or garbage bytes making that test not reliable.
	*/

	if ( b[0] == 'd' &&
	     b[1] == 'a' &&
	     b[2] == 't' &&
	     b[3] == 'a' ) {

		return 1;
	}


	/*
		Finally, check if the four following bytes represent
		a valid 32-bit chunk size.
	*/

	if ( *(uint32_t *)(b + 4) <= b_sz )
		return 1;


	return 0;
}


int add_riff_chunk( struct chunk **ckls, struct chunk *ck ) {

	struct chunk *tmp_ck = NULL;

	/*
		add chunks to the bottom of the list,
		so  we  keep  the  right chunk order.
	*/

	if ( ( tmp_ck = get_last_riff_chunk( *ckls ) ) == NULL )
		*ckls = ck;
	else {
		ck->prev = tmp_ck;
		tmp_ck->next = ck;
	}

	return 0;
}


int add_riff_null_chunk( struct chunk **ckls, unsigned char *buf, size_t sz ) {

	struct chunk *null_ck = new_riff_chunk();

	null_ck->id[0] = 'N';
	null_ck->id[1] = 'U';
	null_ck->id[2] = 'L';
	null_ck->id[3] = 'L';

	null_ck->sz = sz;

	null_ck->bytes = buf - RIFF_CK_HEADER_SZ - sz;

	add_riff_chunk( ckls, null_ck );

	return 0;
}


int insert_riff_chunk_before_id( struct chunk **ckls, struct chunk *ck, const char *ckid ) {

	if ( !( *ckls != NULL && ck != NULL ) )
		return 1;


	struct chunk *tmp_ck = *ckls;

	while ( tmp_ck != NULL && !is_riff_chunk_id( tmp_ck, ckid ) )
		tmp_ck = tmp_ck->next;


	ck->next = tmp_ck;

	if ( tmp_ck->prev != NULL )
		tmp_ck->prev->next = ck;
	else
		*ckls = ck;

	tmp_ck->prev = ck;


	return 0;
}


int insert_riff_chunk_after_id( struct chunk **ckls, struct chunk *ck, const char *ckid ) {

	if ( !( *ckls != NULL && ck != NULL ) )
		return 1;


	struct chunk *tmp_ck = *ckls;

	while ( tmp_ck != NULL && !is_riff_chunk_id( tmp_ck, ckid ) )
		tmp_ck = tmp_ck->next;

	ck->next = tmp_ck->next;
	ck->prev = tmp_ck;
	tmp_ck->next = ck;

	return 0;
}


size_t get_riff_chunk_offset( struct chunk *ckls, struct chunk *ck, uint8_t from ) {

	// TODO: That part is a mess..

	size_t offset = 0;

	if ( from == OFFSET_FROM_BOF )
		offset = RIFF_HEADER_SZ;

	int do_count = from;

	while ( ckls != NULL ) {

		if ( ckls == ck && from == OFFSET_FROM_BOF )
			do_count = 0;

		if ( do_count )
			offset += ckls->sz + RIFF_CK_HEADER_SZ;

		if ( ckls == ck && from == OFFSET_FROM_EOF )
			do_count = 1;

		ckls = ckls->next;
	}

	return offset;
}


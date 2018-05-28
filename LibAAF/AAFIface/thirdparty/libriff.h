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


#ifndef _LIBRIFF_H_
#define _LIBRIFF_H_


#include <stddef.h>	// size_t
#include <stdint.h>



#define RIFF_ID		0x46464952

#define RIFF_HEADER_SZ		12
#define RIFF_CK_HEADER_SZ	 8

#define OFFSET_FROM_BOF		 1
#define OFFSET_FROM_EOF		 0

struct chunk {

	char id[4];
	uint32_t sz;

	unsigned char *bytes;

	void *(*free_bytes)( void *b );

	struct chunk *prev;
	struct chunk *next;
};


struct chunk * new_riff_chunk( void );

struct chunk * free_riff_chunk_list( struct chunk ** );

struct chunk * get_riff_chunk_list( unsigned char *, size_t );

struct chunk * get_riff_chunk_by_id( struct chunk *, const char * );

struct chunk * get_last_riff_chunk( struct chunk * );

int set_riff_chunk_from_buf( struct chunk *, const char * );

int is_riff_chunk_id( struct chunk *, const char * );

int is_riff_chunk( unsigned char *, unsigned int );

int add_riff_chunk( struct chunk **ckls, struct chunk *ck );

int add_riff_null_chunk( struct chunk **ckls, unsigned char *buf, size_t sz );

int insert_riff_chunk_before_id( struct chunk **, struct chunk *, const char * );

int insert_riff_chunk_after_id( struct chunk **, struct chunk *, const char * );

size_t get_riff_chunk_offset( struct chunk *, struct chunk *, uint8_t );

#endif


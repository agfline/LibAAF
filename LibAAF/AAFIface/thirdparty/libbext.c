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

#include "libbext.h"




static int get_cdh_alg( struct coding_history *cdh, char **c );
static int get_cdh_freq( struct coding_history *cdh, char **c );
static int get_cdh_brate( struct coding_history *cdh, char **c );
static int get_cdh_wlength( struct coding_history *cdh, char **c );
static int get_cdh_mode( struct coding_history *cdh, char **c );
static int get_cdh_text( struct coding_history *cdh, char **c );



struct bext * new_bext( void ) {

	struct bext *b = malloc( sizeof(struct bext) );

	if ( b == NULL ) {
		fprintf(stderr, "%s:%d in %s() : %s\n", 
			__FILE__, 
			__LINE__, 
			__func__, 
			strerror(errno));

		return NULL;
	}

	b->ckid[0] = 'b';
	b->ckid[1] = 'e';
	b->ckid[2] = 'x';
	b->ckid[3] = 't';

	// the following is surely not valid, set to zero ?
	b->cksz = sizeof(struct bext);

	memset(b->description,			0x00, 256);
	memset(b->originator,			0x00,  32);
	memset(b->originator_reference, 0x00,  32);
	memset(b->origination_date,		0x00,  10);
	memset(b->origination_time,		0x00,   8);

	b->time_reference = 0;
	b->version = 2;

	memset(b->umid, 0x00, 64);

	b->loudness_value = 0x7fff;
	b->loudness_range = 0x7fff;
	b->max_true_peak_level = 0x7fff;
	b->max_momentary_loudness  = 0x7fff;
	b->max_short_term_loudness = 0x7fff;

	memset(b->reserved, 0x00, 180);

//	memset(b->coding_history, 0x00, 256);
//	b->coding_history = NULL;

	return b;
}

struct bext * free_bext( struct bext **b ) {

	if ( *b == NULL )
		return NULL;

	free(*b);

	*b = NULL;

	return *b;
}

/**********************************************************************************************************
 *						CODING HISTORY						  *
 **********************************************************************************************************/

struct coding_history * new_bext_coding_history_line( void ) {

	struct coding_history * codh = malloc( sizeof(struct coding_history) );

	if ( codh == NULL ) {
		fprintf(stderr, "%s:%d in %s() : %s\n", 
			__FILE__, 
			__LINE__, 
			__func__, 
			strerror(errno));

		return NULL;
	}

	memset(codh->algorithm, 0, BEXT_CDH_MAX_ALG_SZ);

	codh->frequency = 0;
	codh->bitrate = 0;
	codh->wordlength = 0;
	codh->mode = 0;
	codh->text = NULL;

	codh->next = NULL;

	return codh;

}

struct coding_history * free_bext_coding_history( struct coding_history **codh_ls ) {

	struct coding_history *codh = *codh_ls;

	while ( codh != NULL ) {

		*codh_ls = codh->next;

		if ( codh->text != NULL )
			free( codh->text );

		free( codh );

		codh = *codh_ls;
	}

	return codh;
}

struct coding_history * add_bext_coding_history_line( struct coding_history **codh_ls, struct coding_history *codh ) {

	if ( *codh_ls == NULL ) {
		*codh_ls = codh;
	} else {
		codh->next = *codh_ls;
		*codh_ls = codh;
	}

	return *codh_ls;
}


struct coding_history * get_bext_coding_history_list( struct bext *b ) {

	if ( b == NULL )
		return NULL;

	struct coding_history *codh_ls = NULL;
	struct coding_history *codh    = NULL;

	char *c = _get_bext_coding_history_addr( b );

	unsigned int i = 0;

	while ( i < _get_bext_coding_history_size(b->cksz) && 
		*c != 0x00 ) {

		codh = new_bext_coding_history_line();

		while ( *c >= 0x20 ) {

			if ( *c == 'A' && *(c+1) == '=' )
				get_cdh_alg(codh, &c);

			if ( *c == 'F' && *(c+1) == '=' )
				get_cdh_freq(codh, &c);

			if ( *c == 'B' && *(c+1) == '=' )
				get_cdh_brate(codh, &c);

			if ( *c == 'W' && *(c+1) == '=' )
				get_cdh_wlength(codh, &c);

			if ( *c == 'M' && *(c+1) == '=' )
				get_cdh_mode(codh, &c);

			if ( *c == 'T' && *(c+1) == '=' )
				get_cdh_text(codh, &c);

			c++;
			i++;
		}

		add_bext_coding_history_line( &codh_ls, codh );

		if ( *c == 0x0d )
			c++;
		if ( *c == 0x0a )
			c++;
		if ( *c == 0x00 )
			break;

		// this guy should never play
//		printf("x");
		c++;
	}

	return codh_ls;
}


inline uint8_t bext_codh_mode_to_chan( uint8_t mode ) {

	switch ( mode ) {
		case BEXT_CDH_MODE_STEREO:       return 2;
		case BEXT_CDH_MODE_DUAL_MONO:    return 2;
		case BEXT_CDH_MODE_JOINT_STEREO: return 2;
		case BEXT_CDH_MODE_MONO:         return 1;
		default:                         return 0;
	}

	return 0;
}


static int get_cdh_alg( struct coding_history *cdh, char **c ) {

	int i = 0;

	*c += 2; // 'A' '='

	while ( *(*c+i) != ',' && *(*c+i) >= 0x20 )
		i++;

	if ( i > BEXT_CDH_MAX_ALG_SZ )
		i = BEXT_CDH_MAX_ALG_SZ;

	strncpy(cdh->algorithm, *c, i);

	*c += i;

	return 0;
}

static int get_cdh_freq( struct coding_history *cdh, char **c ) {

	int i = 0;

	char tmp[1024];
	memset(tmp, 0, 1024);

	*c += 2; // 'F' '='

	while ( *(*c+i) != ',' && *(*c+i) >= 0x20 ) {
		tmp[i] = *(*c + i);
		i++;
	}

	cdh->frequency = atoi(tmp);

	*c += i;

	return 0;
}

static int get_cdh_brate( struct coding_history *cdh, char **c ) {

	int i = 0;

	char tmp[1024];
	memset(tmp, 0, 1024);

	*c += 2; // 'B' '='

	while ( *(*c+i) != ',' && *(*c+i) >= 0x20 ) {
		tmp[i] = *(*c + i);
		i++;
	}

	cdh->bitrate = atoi(tmp);

	*c += i;

	return 0;
}

static int get_cdh_wlength( struct coding_history *cdh, char **c ) {

	int i = 0;

	char tmp[1024];
	memset(tmp, 0, 1024);

	*c += 2; // 'W' '='

	while ( *(*c+i) != ',' && *(*c+i) >= 0x20 ) {
		tmp[i] = *(*c + i);
		i++;
	}

	cdh->wordlength = atoi(tmp);

	*c += i;

	return 0;
}

static int get_cdh_mode( struct coding_history *cdh, char **c ) {

	*c += 2; // 'M' '='

	if ( *(*c)   == 's' &&
/*	     *(*c+1) == 't' &&
	     *(*c+2) == 'e' &&
	     *(*c+3) == 'r' &&
	     *(*c+4) == 'e' && */
	     *(*c+5) == 'o' ) {

		cdh->mode = BEXT_CDH_MODE_STEREO;
		*c += 7;

		return 1;
	}

	if ( *(*c)   == 'd' &&
/*	     *(*c+1) == 'u' &&
	     *(*c+2) == 'a' &&
	     *(*c+3) == 'l' &&
	     *(*c+4) == '-' &&
	     *(*c+5) == 'm' &&
	     *(*c+6) == 'o' &&
	     *(*c+7) == 'n' && */
	     *(*c+8) == 'o' ) {

		cdh->mode = BEXT_CDH_MODE_DUAL_MONO;
		*c += 10;

		return 1;
	}

	if ( *(*c)   == 'm' &&
/*	     *(*c+1) == 'o' &&
	     *(*c+2) == 'n' && */
	     *(*c+3) == 'o' ) {

		cdh->mode = BEXT_CDH_MODE_MONO;
		*c += 5;

		return 1;
	}

	return 0;
}

static int get_cdh_text( struct coding_history *cdh, char **c ) {

	int i = 0;

	*c += 2; // 'T' '='

	while ( *(*c+i) != ',' && *(*c+i) >= 0x20 )
		i++;

	char *text = malloc( i + 2 ); // +1 for ending char, +1 for size

	if ( text == NULL ) {
		fprintf(stderr, "%s:%d in %s() : %s\n", 
			__FILE__, 
			__LINE__, 
			__func__, 
			strerror(errno));

		return 1;
	}

	strncpy(text, *c, i);

	cdh->text = text;

	*c += i;

	return 0;
}



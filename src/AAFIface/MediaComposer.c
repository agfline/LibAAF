/*
 * Copyright (C) 2024 Adrien Gesta-Fline
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

#include <stdio.h>
#include <stdint.h>

#include <libaaf/AAFIParser.h>
#include <libaaf/AAFToText.h>
#include <libaaf/AAFDefs/AAFPropertyIDs.h>
#include <libaaf/AAFDefs/AAFTypeDefUIDs.h>

#include <libaaf.h>
#include <libaaf/log.h>


#define debug( ... ) \
	AAF_LOG( aafi->log, aafi, LOG_SRC_ID_AAF_IFACE, VERB_DEBUG, __VA_ARGS__ )

#define warning( ... ) \
	AAF_LOG( aafi->log, aafi, LOG_SRC_ID_AAF_IFACE, VERB_WARNING, __VA_ARGS__ )

#define error( ... ) \
	AAF_LOG( aafi->log, aafi, LOG_SRC_ID_AAF_IFACE, VERB_ERROR, __VA_ARGS__ )



int mediaComposer_AAF( struct AAF_Iface *aafi )
{
	int probe = 0;

	if ( aafi->aafd->Identification.CompanyName && strncmp( aafi->aafd->Identification.CompanyName, "Avid Technology, Inc.", strlen( "Avid Technology, Inc." ) ) == 0 ) {
		probe++;
	}

	if ( aafi->aafd->Identification.ProductName && strncmp( aafi->aafd->Identification.ProductName, "Avid Media Composer", strlen( "Avid Media Composer" ) ) == 0 ) {
		probe++;
	}

	if ( probe == 2 ) {
		return 1;
	}

	return 0;
}

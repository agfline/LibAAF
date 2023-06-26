/*
 * Copyright (C) 2017-2023 Adrien Gesta-Fline
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
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <inttypes.h>

#include <libaaf/LibCFB.h>
#include <libaaf/CFBDump.h>

#include "../common/utils.h"





void cfb_dump_node( CFB_Data *cfbd, cfbNode *node, int print_stream )
{

	if ( node == NULL )
		return;

	if ( node->_mse == STGTY_INVALID )
		return;

	char * asciiName = cfb_utf16toa( node->_ab, node->_cb );

	printf( "\n" );

	printf( " _ab          : %s\n", asciiName );

	printf( " _cb          : %u\n", node->_cb );

	printf( " _mse         : %s\n",
		node->_mse == 0 ? "STGTY_INVALID"   :
		node->_mse == 1 ? "STGTY_STORAGE"   :
		node->_mse == 2 ? "STGTY_STREAM"    :
		node->_mse == 3 ? "STGTY_LOCKBYTES" :
		node->_mse == 4 ? "STGTY_PROPERTY"  :
		node->_mse == 5 ? "STGTY_ROOT" : "" );

	printf( " _bflags      : %s\n", node->_bflags == 1 ? "BLACK" : "RED" );

	printf( " _sidLeftSib  : 0x%08x\n", node->_sidLeftSib );

	printf( " _sidRightSib : 0x%08x\n", node->_sidRightSib );

	if ( node->_mse == STGTY_STORAGE || node->_mse == STGTY_ROOT )
	{
		printf( " _sidChild    : 0x%08x\n", node->_sidChild );

		printf( " _clsid       : %ls\n", CLSIDToText( &(node->_clsId) ) );

		printf( " _dwUserFlags : 0x%08x (%d)\n", node->_dwUserFlags, node->_dwUserFlags );
	}

	if ( node->_mse == STGTY_INVALID )
	{
		printf( " _time  (cre) : 0x%08x%08x\n",
			node->_time[0].dwHighDateTime,
			node->_time[0].dwLowDateTime );

		printf( " _      (mod) : 0x%08x%08x\n",
			node->_time[1].dwHighDateTime,
			node->_time[1].dwLowDateTime );
	}

	if ( node->_mse == STGTY_STREAM || node->_mse == STGTY_ROOT )
	{
		printf( " _sectStart   : 0x%08x (%d)\n", node->_sectStart, node->_sectStart );

		printf( " _ulSizeLow   : 0x%08x (%d)\n", node->_ulSizeLow, node->_ulSizeLow );

		printf( " _ulSizeHigh  : 0x%08x (%d)\n", node->_ulSizeHigh, node->_ulSizeHigh );
	}

	printf( "\n\n" );

	if ( asciiName )
    {
		free( asciiName );
    }


    if ( print_stream == 1 )
    {
        cfb_dump_nodeStream( cfbd, node );
    }

}




void cfb_dump_nodePath( CFB_Data *cfbd, const wchar_t *path, int print_stream )
{
    cfbNode *node = cfb_getNodeByPath( cfbd, path, 0 );

    if ( node == NULL )
    {
        fprintf( stderr, "cfb_dump_nodePath() : Could not find node at \"%ls\"\n", path );
        return;
    }

    cfb_dump_node( cfbd, node, print_stream );


    printf( "\n\n" );
}




void cfb_dump_nodeStream( CFB_Data *cfbd, cfbNode *node )
{
    unsigned char *stream = NULL;
    uint64_t       stream_sz = 0;

    cfb_getStream( cfbd, node, &stream, &stream_sz );

    dump_hex( stream, stream_sz );

    free( stream );
}




void cfb_dump_nodePathStream( CFB_Data *cfbd, const wchar_t *path )
{
    cfbNode *node = cfb_getNodeByPath( cfbd, path, 0 );

    if ( node == NULL )
    {
        fprintf( stderr, "cfb_dump_nodePathStream() : Could not find node at \"%ls\"\n", path );
        return;
    }

    unsigned char *stream = NULL;
    uint64_t       stream_sz = 0;

    cfb_getStream( cfbd, node, &stream, &stream_sz );

    dump_hex( stream, stream_sz );

    free( stream );
}




// static int compareStrings( const void *a, const void *b )
// {
// 	return strcmp( (const char *)a, (const char *)b );
// }

// void cfb_dump_nodePaths( CFB_Data *cfbd, uint32_t prevPath, char strArray[][CFB_PATH_NAME_SZ], uint32_t *str_i, cfbNode *node )
void cfb_dump_nodePaths( CFB_Data *cfbd, uint32_t prevPath, char *strArray[], uint32_t *str_i, cfbNode *node )
{

	if ( node == NULL )
	{
		/*
		 *	the begining of the first function call.
		 */

		node     = cfbd->nodes[0];
		strArray = calloc( cfbd->nodes_cnt, sizeof(char*) );
	}



	uint32_t thisPath = (*str_i);

	char *name = cfb_utf16toa( node->_ab, node->_cb );

	int pathlen = snprintf( NULL, 0, "%s/%s", strArray[prevPath], name );

	if ( pathlen < 0 ) {
		// TODO error
		return;
	}

	pathlen++;

	strArray[thisPath] = malloc( pathlen );

	snprintf( strArray[thisPath], pathlen, "%s/%s", strArray[prevPath], name );

	free( name );

	(*str_i)++;



	if ( (int32_t)node->_sidChild > 0 )
		cfb_dump_nodePaths( cfbd, thisPath, strArray, str_i, cfbd->nodes[node->_sidChild] );

	if ( (int32_t)node->_sidLeftSib > 0 )
		cfb_dump_nodePaths( cfbd, prevPath, strArray, str_i, cfbd->nodes[node->_sidLeftSib] );

	if ( (int32_t)node->_sidRightSib > 0 )
		cfb_dump_nodePaths( cfbd, prevPath, strArray, str_i, cfbd->nodes[node->_sidRightSib] );



	/*
	 *	the end of the first function call,
	 *	recursion is over.
	 */

	if ( node == cfbd->nodes[0] )
	{
		/* commented out because output is proper this way... why did we call qsort() in the first place ?! */
		// qsort( strArray, *str_i, sizeof(char*), compareStrings );

		for ( uint32_t i = 0; i < cfbd->nodes_cnt && strArray[i] != NULL; i++ ) {
			printf( "%05i : %s\n", i, strArray[i] );
			free( strArray[i] );
		}

		free( strArray );

    printf( "\n\n" );
	}

}




void cfb_dump_header( CFB_Data *cfbd )
{
	cfbHeader *cfbh = cfbd->hdr;

	printf( "_abSig              : 0x%08"PRIx64"\n", cfbh->_abSig );

	printf( "_clsId              : %ls\n", CLSIDToText( &(cfbh->_clsid) ) );

	printf( " version            : %u.%u ( 0x%04x 0x%04x )\n",
		cfbh->_uMinorVersion, cfbh->_uDllVersion,
		cfbh->_uMinorVersion, cfbh->_uDllVersion );

	printf( "_uByteOrder         : %s ( 0x%04x )\n",
		cfbh->_uByteOrder == 0xFFFE ? "little-endian" :
		cfbh->_uByteOrder == 0xFEFF ? "big-endian" : "?",
		cfbh->_uByteOrder );

	printf( "_uSectorShift       : %u (%u bytes sectors)\n",
		cfbh->_uSectorShift,
		1<<cfbh->_uSectorShift );

	printf( "_uMiniSectorShift   : %u (%u bytes mini-sectors)\n",
		cfbh->_uMiniSectorShift,
		1<<cfbh->_uMiniSectorShift );

	printf( "_usReserved0        : 0x%02x\n", cfbh->_usReserved );
	printf( "_ulReserved1        : 0x%04x\n", cfbh->_ulReserved1 );
	printf( "_csectDir           : %u\n", cfbh->_csectDir );
	printf( "_csectFat           : %u\n", cfbh->_csectFat );
	printf( "_sectDirStart       : %u\n", cfbh->_sectDirStart );
	printf( "_signature          : %u\n", cfbh->_signature );
	printf( "_ulMiniSectorCutoff : %u\n", cfbh->_ulMiniSectorCutoff );
	printf( "_sectMiniFatStart   : %u\n", cfbh->_sectMiniFatStart );
	printf( "_csectMiniFat       : %u\n", cfbh->_csectMiniFat );
	printf( "_sectDifStart       : %u\n", cfbh->_sectDifStart );
	printf( "_csectDif           : %u\n", cfbh->_csectDif );

	printf( "\n" );
}




void cfb_dump_FAT( CFB_Data *cfbd )
{
    printf( "_CFB_FAT_______________________________________________________________________________________\n\n" );

    uint32_t i = 0;

    for ( i = 0; i < cfbd->fat_sz; i++ )
        printf( " SECT[%u] : 0x%08x %s\n",
            i,
            cfbd->fat[i],
            ( cfbd->fat[i] == CFB_MAX_REG_SECT ) ? "(CFB_MAX_REG_SECT)" :
            ( cfbd->fat[i] == CFB_DIFAT_SECT   ) ? "(CFB_DIFAT_SECT)"   :
            ( cfbd->fat[i] == CFB_FAT_SECT     ) ? "(CFB_FAT_SECT)"     :
            ( cfbd->fat[i] == CFB_END_OF_CHAIN ) ? "(CFB_END_OF_CHAIN)" :
            ( cfbd->fat[i] == CFB_FREE_SECT    ) ? "(CFB_FREE_SECT)"    :
            ""
        );

    printf( "\n" );

    printf( " End of FAT.\n\n" );

    printf( " Total FAT entries   : %u\n", cfbd->fat_sz );
    printf( " Count of FAT sector : %u\n", cfbd->hdr->_csectFat );

    printf( "\n\n" );
}




void cfb_dump_MiniFAT( CFB_Data *cfbd )
{
    printf( "_CFB_MiniFAT___________________________________________________________________________________\n\n" );

    uint32_t i = 0;

    for ( i = 0; i < cfbd->miniFat_sz; i++ )
        printf( " SECT[%u] : 0x%08x %s\n",
            i,
            cfbd->miniFat[i],
            ( cfbd->miniFat[i] == CFB_MAX_REG_SECT ) ? "(CFB_MAX_REG_SECT)" :
            ( cfbd->miniFat[i] == CFB_DIFAT_SECT   ) ? "(CFB_DIFAT_SECT)"   :
            ( cfbd->miniFat[i] == CFB_FAT_SECT     ) ? "(CFB_FAT_SECT)"     :
            ( cfbd->miniFat[i] == CFB_END_OF_CHAIN ) ? "(CFB_END_OF_CHAIN)" :
            ( cfbd->miniFat[i] == CFB_FREE_SECT    ) ? "(CFB_FREE_SECT)"    :
            ""
        );

    printf( "\n" );

    printf( " End of MiniFAT.\n\n" );

    printf( " Total MiniFAT entries   : %u\n", cfbd->miniFat_sz );
    printf( " First MiniFAT sector ID : %u\n", cfbd->hdr->_sectMiniFatStart );
    printf( " Count of MiniFAT sector : %u\n", cfbd->hdr->_csectMiniFat );

    printf( "\n\n" );
}




void cfb_dump_DiFAT( CFB_Data *cfbd )
{
    printf( "_CFB_DiFAT_____________________________________________________________________________________\n\n" );

    uint32_t i = 0;

    for ( i = 0; i < cfbd->DiFAT_sz; i++ )
        printf( " SECT[%u] : 0x%08x %s\n",
            i,
            cfbd->DiFAT[i],
            ( cfbd->DiFAT[i] == CFB_MAX_REG_SECT ) ? "(CFB_MAX_REG_SECT)" :
            ( cfbd->DiFAT[i] == CFB_DIFAT_SECT   ) ? "(CFB_DIFAT_SECT)"   :
            ( cfbd->DiFAT[i] == CFB_FAT_SECT     ) ? "(CFB_FAT_SECT)"     :
            ( cfbd->DiFAT[i] == CFB_END_OF_CHAIN ) ? "(CFB_END_OF_CHAIN)" :
            ( cfbd->DiFAT[i] == CFB_FREE_SECT    ) ? "(CFB_FREE_SECT)"    :
            ""
        );

    printf( "\n" );

    printf( " End of DiFAT.\n\n" );

    printf( " Total DiFAT entries   : %u\n", cfbd->DiFAT_sz );
    printf( " First DiFAT sector ID : %u\n", cfbd->hdr->_sectDifStart );
    printf( " Count of DiFAT sector : Header + %u\n", cfbd->hdr->_csectDif );

    printf( "\n\n" );
}

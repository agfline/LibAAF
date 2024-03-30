/*
 * Copyright (C) 2017-2024 Adrien Gesta-Fline
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



/**
 * @file LibCFB/LibCFB.c
 * @brief Compound File Binary Library
 * @author Adrien Gesta-Fline
 * @version 0.1
 * @date 04 october 2017
 *
 * @ingroup LibCFB
 * @addtogroup LibCFB
 * @{
 * @brief LibCFB is a library that allows to parse any Compound File Binary File Format
 * (a.k.a *Structured Storage File Format*).
 *
 * The specifications of the CFB can be found at https://www.amwa.tv/projects/MS-03.shtml
 *
 * @note When using LibAAF, you should not have to talk to LibCFB directly.\n
 * All the following steps are handled by LibAAF.
 *
 * Interaction with LibCFB is done through the CFB_Data structure, which is allocated
 * with cfb_alloc().
 *
 * The first thing you will need to do in order to parse any CFB file is to allocate
 * CFB_Data with cfb_alloc(). Then, you can "load" a file by calling cfb_load_file().
 * It will open the file for reading, check if it is a regular CFB file, then retrieve
 * all the CFB components (Header, DiFAT, FAT, MiniFAT) needed to later parse the file's
 * directories (nodes) and streams.
 *
 * **Example:**
 * @code
 * CFB_Data *cfbd = cfb_alloc();
 * cfb_load_file( cfbd, "/path/to/file" );
 * @endcode
 *
 * Once the file is loaded, you can access the nodes with the functions
 * cfb_getNodeByPath() and cfb_getChildNode(), and access a node's stream with the
 * functions cfb_getStream() or CFB_foreachSectorInStream(). The former is prefered
 * for small-size streams, since it allocates the entire stream to memory, while the
 * later loops through each sector that composes a stream, better for the big ones.
 *
 * **Example:**
 * @code
 * // Get the root "directory" (node, SID 0) "directly"
 * cfbNode *root = cfbd->nodes[0];
 *
 *
 * // Get a "file" (stream node) called "properties" inside the "Header" directory
 * cfbNode *properties = cfb_getNodeByPath( cfbd, "/Header/properties", 0 );
 *
 *
 * // Get the "properties" node stream
 * unsigned char *stream    = NULL;
 * uint64_t       stream_sz = 0;
 *
 * cfb_getStream( cfbd, properties, &stream, &stream_sz );
 *
 *
 * // Loop through each sector that composes the "properties" stream
 * cfbSectorID_t sectorID = 0;
 *
 * CFB_foreachSectorInStream( cfbd, properties, &stream, &stream_sz, &sectorID )
 * {
 * 	// do stuff..
 * }
 * 	@endcode
 *
 * Finaly, once you are done working with the file, you can close the file and free
 * the CFB_Data and its content by simply calling cfb_release().
 *
 * **Example:**
 * @code
 * cfb_release( &cfbd );
 * @endcode
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>	// ceil()
#include <wchar.h>
#include <limits.h>
#include <assert.h>

#include <libaaf/CFBDump.h>
#include <libaaf/LibCFB.h>
#include <libaaf/log.h>

#include <libaaf/utils.h>


#define debug( ... ) \
	AAF_LOG( cfbd->log, cfbd, LOG_SRC_ID_LIB_CFB, VERB_DEBUG, __VA_ARGS__ )

#define warning( ... ) \
	AAF_LOG( cfbd->log, cfbd, LOG_SRC_ID_LIB_CFB, VERB_WARNING, __VA_ARGS__ )

#define error( ... ) \
	AAF_LOG( cfbd->log, cfbd, LOG_SRC_ID_LIB_CFB, VERB_ERROR, __VA_ARGS__ )




static int cfb_getFileSize( CFB_Data *cfbd );

static int cfb_openFile( CFB_Data *cfbd );

static ssize_t cfb_readFile( CFB_Data *cfbd, unsigned char *buf, size_t offset, size_t len );

static void cfb_closeFile( CFB_Data *cfbd );

static cfbSectorID_t cfb_getStreamSectorIDBySectorPosition( CFB_Data *cfbd, cfbSectorID_t start, cfbSectorID_t pos );

static cfbSectorID_t cfb_getStreamMiniSectorIDBySectorPosition( CFB_Data *cfbd, cfbSectorID_t start, cfbSectorID_t pos );

static ssize_t cfb_getSectorOffset( CFB_Data *cfbd, cfbSectorID_t id );

static ssize_t cfb_getMiniSectorOffset( CFB_Data *cfbd, cfbSectorID_t id );

static int cfb_is_valid( CFB_Data *cfbd );

static int cfb_retrieveFileHeader( CFB_Data *cfbd );

static int cfb_retrieveDiFAT( CFB_Data *cfbd );

static int cfb_retrieveFAT( CFB_Data * cfbd );

static int cfb_retrieveMiniFAT( CFB_Data * cfbd );

static int cfb_retrieveNodes( CFB_Data *cfbd );

static cfbSID_t getNodeCount( CFB_Data *cfbd );

static cfbSID_t cfb_getIDByNode( CFB_Data *cfbd, cfbNode *node );






const char * cfb_CLSIDToText( const cfbCLSID_t *clsid )
{
	static char str[96];

	if ( clsid == NULL ) {
		str[0] = 'n';
		str[1] = '/';
		str[2] = 'a';
		str[3] = '\0';
	}
	else {
		int rc = snprintf( str, sizeof(str), "{ 0x%08x 0x%04x 0x%04x { 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x } }",
			clsid->Data1,
			clsid->Data2,
			clsid->Data3,
			clsid->Data4[0],
			clsid->Data4[1],
			clsid->Data4[2],
			clsid->Data4[3],
			clsid->Data4[4],
			clsid->Data4[5],
			clsid->Data4[6],
			clsid->Data4[7] );

		if ( rc < 0 || (size_t)rc >= sizeof(str) ) {
			// TODO error
			return NULL;
		}
	}

	return str;
}



/**
 * Allocates a new CFB_Data structure.
 *
 * @return Pointer to the newly allocated CFB_Data structure.
 */

CFB_Data * cfb_alloc( struct aafLog *log )
{
	CFB_Data * cfbd = calloc( 1, sizeof(CFB_Data) );

	if ( !cfbd ) {
		return NULL;
	}

	cfbd->log = log;

	return cfbd;
}



/**
 * fclose() the file pointer then frees all the components of
 * the CFB_Data structure, including the structure itself.
 *
 * @param cfbd Pointer to Pointer to the CFB_Data structure.
 */

void cfb_release( CFB_Data **cfbd )
{
	if ( cfbd == NULL || *cfbd == NULL )
		return;

	cfb_closeFile( *cfbd );

	free( (*cfbd)->file );
	(*cfbd)->file = NULL;

	free( (*cfbd)->DiFAT );
	(*cfbd)->DiFAT = NULL;

	free( (*cfbd)->fat );
	(*cfbd)->fat = NULL;

	free( (*cfbd)->miniFat );
	(*cfbd)->miniFat = NULL;

	free( (*cfbd)->nodes );
	(*cfbd)->nodes = NULL;

	free( (*cfbd)->hdr );
	(*cfbd)->hdr = NULL;

	free( *cfbd );
	*cfbd = NULL;
}



/**
 * Loads a Compound File Binary File, retrieves its Header, FAT,
 * MiniFAT and Nodes. then sets the CFB_Data structure so it is
 * ready for parsing the file. The user should call cfb_release()
 * once he's done using the file.
 *
 * @param  cfbd     Pointer to the CFB_Data structure.
 * @param  file     Pointer to a NULL terminated string holding the file path.
 *
 * @return          0 on success\n
 *                  1 on error
 */

int cfb_load_file( CFB_Data **cfbd_p, const char *file )
{
	CFB_Data *cfbd = *cfbd_p;

	cfbd->file = laaf_util_absolute_path( file );

	if ( !cfbd->file ) {
		error( "Could not resolve absolute file path" );
		return -1;
	}

	if ( cfb_openFile( cfbd ) < 0 ) {
		cfb_release( cfbd_p );
		return -1;
	}

	if ( cfb_getFileSize( cfbd ) < 0 ) {
		cfb_release( cfbd_p );
		return -1;
	}

	if ( cfb_is_valid( cfbd ) == 0 ) {
		cfb_release( cfbd_p );
		return -1;
	}

	if ( cfb_retrieveFileHeader( cfbd ) < 0 ) {
		error( "Could not retrieve CFB header." );
		cfb_release( cfbd_p );
		return -1;
	}

	if ( cfb_retrieveDiFAT( cfbd ) < 0 ) {
		error( "Could not retrieve CFB DiFAT." );
		cfb_release( cfbd_p );
		return -1;
	}

	if ( cfb_retrieveFAT( cfbd ) < 0 ) {
		error( "Could not retrieve CFB FAT." );
		cfb_release( cfbd_p );
		return -1;
	}

	if ( cfb_retrieveMiniFAT( cfbd ) < 0 ) {
		error( "Could not retrieve CFB MiniFAT." );
		cfb_release( cfbd_p );
		return -1;
	}

	if ( cfb_retrieveNodes( cfbd ) < 0 ) {
		error( "Could not retrieve CFB Nodes." );
		cfb_release( cfbd_p );
		return -1;
	}

	return 0;
}



int cfb_new_file( CFB_Data *cfbd, const char *file, int sectSize )
{
	(void)file;

	if ( sectSize != 512 &&
	     sectSize != 4096 )
	{
		error( "Only standard sector sizes (512 and 4096 bytes) are supported." );
		return -1;
	}

	cfbHeader *hdr = malloc( sizeof(cfbHeader) );

	if ( !hdr ) {
		error( "Out of memory" );
		return -1;
	}

	cfbd->hdr = hdr;

	hdr->_abSig = 0xe11ab1a1e011cfd0;

	/*
	 * _uMinorVersion is set to 33 reference implementation.
	 * _uMinorVersion is set to 0x3e in all AAF files
	 */
	hdr->_uMinorVersion = 0x3e;

	hdr->_uDllVersion   = ( sectSize == 512 ) ? 3 : 4;
	hdr->_uByteOrder    = 0xfffe;
	hdr->_uSectorShift  = ( sectSize == 512 ) ? 9 : 12;
	hdr->_uMiniSectorShift = 6;
	hdr->_usReserved  = 0;
	hdr->_ulReserved1 = 0;

	hdr->_csectDir = 0;
	hdr->_csectFat = 0;
	hdr->_sectDirStart = 0;
	hdr->_signature = 0;

	hdr->_ulMiniSectorCutoff = 4096;

	hdr->_sectMiniFatStart = 0;
	hdr->_csectMiniFat = 0;
	hdr->_sectDifStart = 0;
	hdr->_csectDif = 0;

	int i = 0;

	for ( i = 0; i < 109; i++ )
		hdr->_sectFat[i] = CFB_FREE_SECT;

	return 0;
}



/**
 * Ensures the file is a valid Compound File Binary File.
 *
 * @param  cfbd Pointer to the CFB_Data structure.
 *
 * @return      1 if the file is valid,\n
 *              0 otherwise.
 */

static int cfb_is_valid( CFB_Data *cfbd )
{
	uint64_t signature = 0;

	if ( cfbd->file_sz < sizeof(struct StructuredStorageHeader) ) {
		error( "Not a valid Compound File : File size is lower than header size." );
		return 0;
	}

	if ( cfb_readFile( cfbd, (unsigned char*)&signature, 0, sizeof(uint64_t) ) != sizeof(uint64_t) ) {
		return 0;
	}

	if ( signature != 0xe11ab1a1e011cfd0 ) {
		error( "Not a valid Compound File : Wrong signature." );
		return 0;
	}

	return 1;
}



/**
 * Retrieves the Compound File Binary File size.
 *
 * @param  cfbd Pointer to the CFB_Data structure.
 * @return      0.
 */

static int cfb_getFileSize( CFB_Data * cfbd )
{

#ifdef _WIN32
	if ( _fseeki64( cfbd->fp, 0L, SEEK_END ) < 0 ) {
		error( "fseek() failed : %s.", strerror(errno) );
		return -1;
	}

	__int64 filesz = _ftelli64( cfbd->fp );

#else
	if ( fseek( cfbd->fp, 0L, SEEK_END ) < 0 ) {
		error( "fseek() failed : %s.", strerror(errno) );
		return -1;
	}

	long filesz = ftell( cfbd->fp );

#endif

	if ( filesz < 0 ) {
		error( "ftell() failed : %s.", strerror(errno) );
		return -1;
	}

	if ( filesz == 0 ) {
		error( "File is empty (0 byte)." );
		return -1;
	}

	cfbd->file_sz = (size_t)filesz;

	return 0;
}



/**
 * Opens a given Compound File Binary File for reading.
 *
 * @param  cfbd Pointer to the CFB_Data structure.
 * @return      O.
 */

static int cfb_openFile( CFB_Data *cfbd )
{
	if ( !cfbd->file ) {
		return -1;
	}

#ifdef _WIN32

	wchar_t *wfile = laaf_util_windows_utf8toutf16( cfbd->file );

	if ( !wfile ) {
		error( "Unable to convert filepath to wide string : %s", cfbd->file );
		return -1;
	}

	cfbd->fp = _wfopen( wfile, L"rb" );

	free( wfile );

#else
	cfbd->fp = fopen( cfbd->file, "rb" );
#endif

	if ( !cfbd->fp ) {
		error( "%s.", strerror(errno) );
		return -1;
	}

	return 0;
}



/**
 * Reads a bytes block from the file. This function is
 * called by cfb_getSector() and cfb_getMiniSector()
 * that will do the sector index to file offset conversion.
 *
 * @param cfbd   Pointer to the CFB_Data structure.
 * @param buf    Pointer to the buffer that will hold the len bytes read.
 * @param offset Position in the file the read should start.
 * @param len    Number of bytes to read from the offset position.
 */

static ssize_t cfb_readFile( CFB_Data *cfbd, unsigned char *buf, size_t offset, size_t reqlen )
{
	FILE *fp = cfbd->fp;

	// debug( "Requesting file read @ offset %"PRIu64" of length %"PRIu64, offset, reqlen );

	if ( offset >= LONG_MAX ) {
		error( "Requested data offset is bigger than LONG_MAX" );
		return -1;
	}

	if ( reqlen + offset > cfbd->file_sz ) {
		error( "Requested data goes %"PRIu64" bytes beyond the EOF : offset %"PRIu64" | length %"PRIu64"", (reqlen + offset) - cfbd->file_sz, offset, reqlen );
		return -1;
	}

	int rc = fseek( fp, (long)offset, SEEK_SET );

	if ( rc < 0 ) {
		error( "%s.", strerror(errno) );
		return -1;
	}


	size_t byteRead = fread( buf, sizeof(unsigned char), reqlen, fp );

	if ( feof(fp) ) {
		if ( byteRead < reqlen ) {
			error( "Incomplete fread() of CFB due to EOF : %"PRIu64" bytes read out of %"PRIu64" requested", byteRead, reqlen );
		}
		debug( "fread() : EOF reached in CFB file" );
	}
	else if ( ferror(fp) ) {
		if ( byteRead < reqlen ) {
			error( "Incomplete fread() of CFB due to error : %"PRIu64" bytes read out of %"PRIu64" requested", byteRead, reqlen );
		}
		else {
			error( "fread() error of CFB : %"PRIu64" bytes read out of %"PRIu64" requested", byteRead, reqlen );
		}
	}

	return (ssize_t)byteRead;
}



/**
 * Closes the file pointer hold by the CFB_Data.fp.
 *
 * @param cfbd Pointer to the CFB_Data structure.
 */

static void cfb_closeFile( CFB_Data *cfbd )
{
	if ( cfbd == NULL || cfbd->fp == NULL )
		return;

	if ( fclose( cfbd->fp ) != 0 ) {
		error( "%s.", strerror(errno) );
	}

	cfbd->fp = NULL;
}



/*
 * Retrieves a sector ID by its position in a stream.
 *
 * For example :
 *    Sectors:    |   | 5 |   | 1 | E | 4 | ...
 *    SectorIDs:    0   1   2   3   4   5   ...
 *
 * A stream is composed of sectorIDs 3, 1, 5 and ends with sector 4.
 * A request for the ID of sector at position 3 (third sector in the stream)
 * returns 5.
 */
static cfbSectorID_t cfb_getStreamSectorIDBySectorPosition( CFB_Data *cfbd, cfbSectorID_t start, cfbSectorID_t pos )
{
	if ( start >= cfbd->fat_sz ) {
		error( "FAT sector %u is out of range (Maximum FAT index is %u)", start, cfbd->fat_sz );
		return CFB_SECT_ERROR;
	}

	cfbSectorID_t id = start;

	/* loop condition is just here for safety, just in case... */
	for ( cfbSectorID_t i = 0; i < cfbd->fat_sz; i++ ) {

		if ( i == pos ) {
			return id;
		}

		if ( id >= CFB_MAX_REG_SECT ) {
			error( "Invalid FAT index %u (0x%08x) at pos %u", id, id, i );
			return CFB_SECT_ERROR;
		}

		if ( id >= cfbd->fat_sz ) {
			error( "FAT index %u (0x%08x) at pos %i is out of range (Maximum FAT index is %u)", id, id, i, cfbd->fat_sz );
			return CFB_SECT_ERROR;
		}

		id = cfbd->fat[id];
	}

	/* should never be reached */
	return CFB_SECT_ERROR;
}



/*
 * Retrieves a sector ID by its position in a mini-stream.
 *
 * For example :
 *    Sectors:    |   | 5 |   | 1 | E | 4 | ...
 *    SectorIDs:    0   1   2   3   4   5   ...
 *
 * A stream is composed of sectorIDs 3, 1, 5 and ends with sector 4.
 * A request for the ID of sector at position 3 (third sector in the stream)
 * returns 5.
 */
static cfbSectorID_t cfb_getStreamMiniSectorIDBySectorPosition( CFB_Data *cfbd, cfbSectorID_t start, cfbSectorID_t pos )
{
	if ( start >= cfbd->miniFat_sz ) {
		error( "MiniFAT sector %u is out of range (Maximum MiniFAT index is %u)", start, cfbd->miniFat_sz );
		return CFB_SECT_ERROR;
	}

	cfbSectorID_t id = start;

	/* loop condition is just here for safety, just in case... */
	for ( cfbSectorID_t i = 0; i < cfbd->miniFat_sz; i++ ) {

		if ( i == pos ) {
			return id;
		}

		if ( id >= CFB_MAX_REG_SECT ) {
			error( "Invalid miniFAT index %u (0x%08x) at pos %u", id, id, i );
			return CFB_SECT_ERROR;
		}

		if ( id >= cfbd->miniFat_sz ) {
			error( "MiniFAT index %u (0x%08x) at pos %i is out of range (Maximum MiniFAT index is %u)", id, id, i, cfbd->miniFat_sz );
			return CFB_SECT_ERROR;
		}

		id = cfbd->miniFat[id];
	}

	/* should never be reached */
	return CFB_SECT_ERROR;
}



static ssize_t cfb_getSectorOffset( CFB_Data *cfbd, cfbSectorID_t id )
{
	/*
	 * foreachSectorInChain() calls cfb_getSector() before
	 * testing the ID, so we have to ensure the ID is valid
	 * before we try to actualy get the sector.
	 */

	/* Note: fat_sz is zero when retrieving DiFAT */
	if ( (cfbd->fat_sz && id >= cfbd->fat_sz) || id >= CFB_MAX_REG_SECT ) {
		error( "FAT index %u (0x%08x) is out of range : max FAT index is %u", id, id, cfbd->fat_sz );
		return -1;
	}

	return (id + 1) << cfbd->hdr->_uSectorShift;
}



/**
 * Retrieves a sector content from the FAT.
 *
 * @param id   Index of the sector to retrieve in the FAT.
 * @param cfbd Pointer to the CFB_Data structure.
 * @return     Pointer to the sector's data bytes.
 */

unsigned char * cfb_getSector( CFB_Data *cfbd, cfbSectorID_t id )
{
	/*
	 * foreachSectorInChain() calls cfb_getSector() before
	 * testing the ID, so we have to ensure the ID is valid
	 * before we try to actualy get the sector.
	 */

	if ( id == CFB_END_OF_CHAIN ) {
		return NULL;
	}

	ssize_t fileOffset = cfb_getSectorOffset( cfbd, id );

	if ( fileOffset < 0 ) {
		error( "Could not get file offset of mini sector ID %u (0x%08x)", id, id );
		return NULL;
	}

	uint64_t sectorSize = (1 << cfbd->hdr->_uSectorShift);


	unsigned char *buf = calloc( 1, sectorSize );

	if ( !buf ) {
		error( "Out of memory" );
		return NULL;
	}


	if ( cfb_readFile( cfbd, buf, (size_t)fileOffset, sectorSize ) < 0 ) {
		free( buf );
		return NULL;
	}

	// laaf_util_dump_hex( buf, (1<<cfbd->hdr->_uSectorShift), &cfbd->log->_msg, &cfbd->log->_msg_size, cfbd->log->_msg_pos, "" );

	return buf;
}



ssize_t cfb_getMiniSectorOffset( CFB_Data *cfbd, cfbSectorID_t id )
{
	/*
	 * foreachMiniSectorInChain() calls cfb_getMiniSector() before
	 * testing the ID, so we have to ensure the ID is valid before
	 * we try to actualy get the sector.
	 */

	if ( id >= cfbd->miniFat_sz || id >= CFB_MAX_REG_SECT ) {
		error( "MiniFAT index %u (0x%08x) is out of range : max MiniFAT index is %u", id, id, cfbd->miniFat_sz );
		return -1;
	}

	uint32_t SectorSize     = 1 << cfbd->hdr->_uSectorShift;
	uint32_t MiniSectorSize = 1 << cfbd->hdr->_uMiniSectorShift;

	cfbSectorID_t miniFatSectorsPerFATSector = SectorSize / MiniSectorSize;

	/*
	 * « The mini stream is chained within the FAT in exactly the same fashion
	 * as any normal stream. The mini stream's starting sector is referenced in
	 * the first directory entry (SID 0). »
	 */

	cfbSectorID_t fatID = cfbd->nodes[0]._sectStart;

	for ( cfbSectorID_t i = 0; i < id/miniFatSectorsPerFATSector; i++ ) {

		if ( fatID >= cfbd->fat_sz || fatID >= CFB_MAX_REG_SECT ) {
			error( "FAT index (%i/%i) is out of range: %u (0x%08x)", i, (id/miniFatSectorsPerFATSector), fatID, fatID );
			return -1;
		}

		fatID = cfbd->fat[fatID];
	}

	size_t offset = 0;

	offset  = ((fatID + 1) << cfbd->hdr->_uSectorShift);
	offset += ((id % miniFatSectorsPerFATSector ) << cfbd->hdr->_uMiniSectorShift);

	return (ssize_t)offset;
}



/**
 * Retrieves a mini-sector content from the MiniFAT.
 *
 * @param id   Index of the mini-sector to retrieve in the MiniFAT.
 * @param cfbd Pointer to the CFB_Data structure.
 * @return     Pointer to the mini-sector's data bytes.
 */

unsigned char * cfb_getMiniSector( CFB_Data *cfbd, cfbSectorID_t id )
{
	/*
	 * foreachMiniSectorInChain() calls cfb_getMiniSector() before
	 * testing the ID, so we have to ensure the ID is valid before
	 * we try to actualy get the sector.
	 */

	if ( id == CFB_END_OF_CHAIN ) {
		return NULL;
	}

	ssize_t fileOffset = cfb_getMiniSectorOffset( cfbd, id );

	if ( fileOffset < 0 ) {
		error( "Could not get file offset of mini sector ID %u (0x%08x)", id, id );
		return NULL;
	}

	uint32_t MiniSectorSize = 1 << cfbd->hdr->_uMiniSectorShift;


	unsigned char * buf = calloc( 1, MiniSectorSize );

	if ( !buf ) {
		error( "Out of memory" );
		return NULL;
	}


	if ( cfb_readFile( cfbd, buf, (size_t)fileOffset, MiniSectorSize ) < 0 ) {
		goto err;
	}


	goto end;

err:
	free( buf );
	buf = NULL;

end:
	return buf;
}



/**
 * Retrieves a stream from a stream Node.
 *
 * @param cfbd      Pointer to the CFB_Data structure.
 * @param node      Pointer to the node to retrieve the stream from.
 * @param stream    Pointer to the pointer where the stream data will be saved.
 * @param stream_sz Pointer to an uint64_t where the stream size will be saved.
 * @return          The retrieved stream size.
 */

uint64_t cfb_getStream( CFB_Data *cfbd, cfbNode *node, unsigned char **stream, uint64_t *stream_sz )
{
	if ( node == NULL ) {
		return 0;
	}


	uint64_t stream_len = CFB_getNodeStreamLen( cfbd, node );

	if ( stream_len == 0 ) {
		return 0;
	}

	*stream = calloc( 1, stream_len );

	if ( !(*stream) ) {
		error( "Out of memory" );
		return 0;
	}


	unsigned char *buf    = NULL;
	cfbSectorID_t  id     = node->_sectStart;
	uint64_t       offset = 0;
	uint64_t       cpy_sz = 0;

	if ( stream_len < cfbd->hdr->_ulMiniSectorCutoff ) { /* mini-stream */

		CFB_foreachMiniSectorInChain( cfbd, buf, id ) {

			if ( !buf ) {
				free( *stream );
				*stream = NULL;
				return 0;
			}

			cpy_sz = ( (stream_len - offset) < (uint64_t)(1<<cfbd->hdr->_uMiniSectorShift) ) ?
			           (stream_len - offset) : (uint64_t)(1<<cfbd->hdr->_uMiniSectorShift);

			memcpy( *stream+offset, buf, cpy_sz );

			free( buf );

			offset += (1<<cfbd->hdr->_uMiniSectorShift);
		}
	}
	else {

		CFB_foreachSectorInChain( cfbd, buf, id ) {

			cpy_sz = ( (stream_len - offset) < (uint64_t)(1<<cfbd->hdr->_uSectorShift) ) ?
			           (stream_len - offset) : (uint64_t)(1<<cfbd->hdr->_uSectorShift);

			memcpy( *stream+offset, buf, cpy_sz );

			free( buf );

			offset += (1<<cfbd->hdr->_uSectorShift);
		}
	}

	if ( stream_sz != NULL )
		*stream_sz = stream_len;

	return stream_len;
}



CFBStreamDescriptor * cfb_open_stream( CFB_Data *cfbd, cfbNode *node )
{
	CFBStreamDescriptor *sd = calloc( 1, sizeof(CFBStreamDescriptor) );

	if ( !sd ) {
		return NULL;
	}

	sd->cfbd = cfbd;
	sd->node = node;
	sd->stream_sz = CFB_getNodeStreamLen( cfbd, node );

	if ( sd->stream_sz < cfbd->hdr->_ulMiniSectorCutoff ) {
		sd->is_ministream = 1;
	}

	if ( sd->stream_sz == 0 ) {
		free( sd );
		return NULL;
	}

	cfb_stream_seek( sd, SEEK_SET, 0 );

	return sd;
}



void cfb_close_stream( CFBStreamDescriptor *sd )
{
	if ( !sd ) {
		return;
	}

	free( sd );
}



size_t cfb_stream_size( CFBStreamDescriptor *sd )
{
	return sd->stream_sz;
}



size_t cfb_stream_tell( CFBStreamDescriptor *sd )
{
	return sd->offset;
}



ssize_t cfb_stream_seek( CFBStreamDescriptor *sd, int whence, int64_t pos )
{
	if ( !sd || !sd->cfbd ) {
		return -1;
	}

	CFB_Data *cfbd = sd->cfbd;

	switch ( whence ) {
		case SEEK_SET:  break;
		case SEEK_CUR:

			if ( sd->offset >= INT64_MAX ) {
				error( "Stream offset bigger than INT64_MAX : %zu", sd->offset );
				return -1;
			}

			pos = (int64_t)sd->offset + pos;

			break;

		case SEEK_END:

			if ( sd->stream_sz >= INT64_MAX ) {
				error( "Stream size bigger than INT64_MAX : %zu", sd->stream_sz );
				return -1;
			}

			pos = (int64_t)sd->stream_sz + pos;

			break;

		default:
			error( "Unknown whence : %i", whence );
			return -1;
	}

	if ( pos < 0 ) {
		error( "Calculated stream position is negative : %"PRIi64, pos );
		return -1;
	}

	if ( (size_t)pos > sd->stream_sz ) {
		error( "Calculated stream position (%zd) is beyond end of stream (%li)", sd->stream_sz, pos );
		return -1;
	}


	uint32_t sectorSize = (sd->is_ministream) ? (1 << cfbd->hdr->_uMiniSectorShift) : (1 << cfbd->hdr->_uSectorShift);

	size_t sectorPos = ((size_t)pos/sectorSize);

	assert( sectorPos < CFB_MAX_REG_SECT );

	cfbSectorID_t id = 0;

	if ( sd->is_ministream ) {
		id = cfb_getStreamMiniSectorIDBySectorPosition( cfbd, sd->node->_sectStart, (cfbSectorID_t)sectorPos );
	}
	else {
		id = cfb_getStreamSectorIDBySectorPosition( cfbd, sd->node->_sectStart, (cfbSectorID_t)sectorPos );
	}

	if ( id == CFB_SECT_ERROR ) {
		error( "Could not retrieve sector ID at pos %zd out of %s", sectorPos, (sd->is_ministream) ? "miniFAT" : "FAT" );
		return -1;
	}

	sd->sectorPos = (cfbSectorID_t)sectorPos;
	sd->sectorOffset = (size_t)pos % sectorSize;
	sd->offset = (size_t)pos;
	sd->id = id;

	return (ssize_t)sd->offset;
}



ssize_t cfb_stream_read( CFBStreamDescriptor *sd, void* buf, size_t nbytes )
{
	if ( !sd || !sd->cfbd || !buf ) {
		return -1;
	}

	CFB_Data *cfbd = sd->cfbd;
	cfbNode *node = sd->node;

	if ( !node ) {
		error( "StreamDescriptor node is not set" );
		return -1;
	}

	if ( sd->offset >= sd->stream_sz ) {
		error( "Requested offset %lu goes beyond stream size %lu", sd->offset, sd->stream_sz );
		return -1;
	}


	uint32_t sectorSize = (sd->is_ministream) ? (1 << cfbd->hdr->_uMiniSectorShift) : (1 << cfbd->hdr->_uSectorShift);

	// if ( nbytes < sectorSize ) {
	// 	error( "nbytes must be > 4096" );
	// 	return -1;
	// }


	// debug( "Requesting stream [size: %lu / sectorsize: %u] : offset: %lu  nbytes: %lu  sectoroffset: %lu", sd->stream_sz, sectorSize, sd->offset, nbytes, sd->sectorOffset );


	size_t totalBytesRead = 0;
	ssize_t fileOffset = 0;

	while ( totalBytesRead < nbytes ) {

		fileOffset = ( sd->is_ministream ) ? cfb_getMiniSectorOffset( cfbd, sd->id ) : cfb_getSectorOffset( cfbd, sd->id );

		if ( fileOffset < 0 ) {
			error("fileOffset error");
			return (ssize_t)totalBytesRead;
		}

		size_t reqBytes = sectorSize;

		if ( sd->sectorOffset /*&& totalBytesRead == 0*/ ) {
			fileOffset += (ssize_t)sd->sectorOffset;
			reqBytes -= sd->sectorOffset;
			// debug( "Reading a portion of the first requested sector : %lu", reqBytes );
		}

		if ( sd->offset+reqBytes >= sd->stream_sz ) {
			reqBytes = sd->stream_sz - sd->offset;
			// debug( "Reading the last portion of the last sector in stream : %lu", reqBytes );
		}

		if ( totalBytesRead+reqBytes > nbytes ) {
			reqBytes = nbytes - totalBytesRead;
			// debug( "Reading a portion of the last requested sector : %lu", reqBytes );
		}

		ssize_t bytesRead = cfb_readFile( cfbd, (unsigned char*)buf+totalBytesRead, (size_t)fileOffset, reqBytes );

		if ( bytesRead < 0 ) {
			error( "cfb_readFile()" );
			return (ssize_t)totalBytesRead;
		}

		// debug( "Read %lu bytes out of sector %i.  totalread: %li   streampos: %li", bytesRead, sd->id, totalBytesRead+(size_t)bytesRead, sd->offset+totalBytesRead+(size_t)bytesRead );

		totalBytesRead += (size_t)bytesRead;
		sd->offset += (size_t)bytesRead;

		if ( sd->offset == sd->stream_sz ) {
			// debug( "End of stream : %li", sd->offset );
			return (ssize_t)totalBytesRead;
		}

		size_t sectorPos = sd->offset / sectorSize;

		assert( sectorPos <= CFB_MAX_REG_SECT );

		sd->sectorOffset = sd->offset % sectorSize;

		/* get next sector id */
		if ( sd->sectorPos < sectorPos ) {

			// debug( "Moving forward to next sector index" );

			if ( sd->is_ministream ) {

				sd->id = cfbd->miniFat[sd->id];

				if ( sd->id >= cfbd->miniFat_sz || sd->id >= CFB_MAX_REG_SECT ) {
					error( "Invalid miniFAT index %u (0x%08x) at pos %zd", sd->id, sd->id, sd->offset );
					return (ssize_t)totalBytesRead;
				}
			}
			else {

				sd->id = cfbd->fat[sd->id];

				if ( sd->id >= cfbd->fat_sz || sd->id >= CFB_MAX_REG_SECT ) {
					error( "Invalid FAT index %u (0x%08x) at pos %zd", sd->id, sd->id, sd->offset );
					return (ssize_t)totalBytesRead;
				}
			}
		}

		sd->sectorPos = (cfbSectorID_t)sectorPos;
	}

	return (ssize_t)totalBytesRead;
}



/**
 * Loops through all the sectors that compose a stream
 * and retrieve their content.
 *
 * This function should be called through the macro CFB_foreachSectorInStream().
 *
 * @param  cfbd      Pointer to the CFB_Data structure.
 * @param  node      Pointer to the Node that hold the stream.
 * @param  buf       Pointer to pointer to the buffer that will receive the sector's
 *                   content.
 * @param  bytesRead Pointer to a size_t that will receive the number of bytes retreived.
 *                   This should be equal to the sectorSize, except for the last sector
 *                   in which the stream might end before the end of the sector.
 * @param  sectID    Pointer to the Node index.
 * @return           1 if we have retrieved a sector with some data, \n
 *                   0 if we have reached the #CFB_END_OF_CHAIN.
 */

int cfb__foreachSectorInStream( CFB_Data *cfbd, cfbNode *node, unsigned char **buf, size_t *bytesRead, cfbSectorID_t *sectID )
{
	if ( node == NULL )
		return 0;

	if ( *sectID >= CFB_MAX_REG_SECT )
		return 0;

	/* free the previously allocated buf, if any */
	free( *buf );
	*buf = NULL;


	/* if *nodeID == 0, then it is the first function call */
	*sectID = ( *sectID == 0 ) ? node->_sectStart : *sectID;


	size_t stream_sz = CFB_getNodeStreamLen( cfbd, node );

	if ( stream_sz < cfbd->hdr->_ulMiniSectorCutoff ) {
		/* Mini-Stream */
		*buf       = cfb_getMiniSector( cfbd, *sectID );
		*bytesRead = (1<<cfbd->hdr->_uMiniSectorShift);
		*sectID    = cfbd->miniFat[*sectID];
	}
	else {
		/* Stream */
		*buf       = cfb_getSector( cfbd, *sectID );
		*bytesRead = (1<<cfbd->hdr->_uSectorShift);
		*sectID    = cfbd->fat[*sectID];
	}


	return 1;
}



/**
 * Retrieves the Header of the Compound File Binary.
 * The Header begins at offset 0 and is 512 bytes long,
 * regardless of the file's sector size.
 *
 * @param  cfbd Pointer to the CFB_Data structure.
 * @return      0 on success\n
 *              1 on failure
 */

static int cfb_retrieveFileHeader( CFB_Data *cfbd )
{
	cfbd->hdr = calloc( 1, sizeof(cfbHeader) );

	if ( !cfbd->hdr ) {
		error( "Out of memory" );
		return -1;
	}

	if ( cfb_readFile( cfbd, (unsigned char*)cfbd->hdr, 0, sizeof(cfbHeader) ) < 0 ) {
		goto err;
	}

	if ( cfbd->hdr->_uSectorShift != 9 &&
	     cfbd->hdr->_uSectorShift != 12 )
	{
		goto err;
	}

	return 0;

err:
	free( cfbd->hdr );
	cfbd->hdr = NULL;

	return -1;
}



static int cfb_retrieveDiFAT( CFB_Data *cfbd )
{
	cfbSectorID_t *DiFAT = NULL;
	unsigned char *buf   = NULL;

	/*
	 * Check DiFAT properties in header.
	 * (Exemple AMWA aaf files.)
	 */

	cfbSectorID_t csectDif = 0;

	if ( cfbd->hdr->_csectFat > 109 ) {

		double csectDifdouble = ceil( (float)((cfbd->hdr->_csectFat - 109) * 4) / (1<<cfbd->hdr->_uSectorShift) );

		if ( csectDifdouble >= UINT_MAX || csectDifdouble < 0 ) {
			warning( "Calculated csectDif is negative or bigger than UINT_MAX" );
			// return -1;
		}

		csectDif = (cfbSectorID_t)csectDifdouble;
	}

	if ( csectDif != cfbd->hdr->_csectDif ) {
		warning( "_csectDif value (%u) does not match calculation : %u", cfbd->hdr->_csectDif, csectDif );
	}

	if ( csectDif == 0 && cfbd->hdr->_sectDifStart != CFB_END_OF_CHAIN ) {
		debug( "_sectDifStart is %u (0x%08x) : should be CFB_END_OF_CHAIN (0x%08x)", cfbd->hdr->_sectDifStart, cfbd->hdr->_sectDifStart, CFB_END_OF_CHAIN );
		cfbd->hdr->_sectDifStart = CFB_END_OF_CHAIN;
	}


	/*
	 * DiFAT size is the number of FAT sector entries in the DiFAT chain.
	 * _uSectorShift is guaranted to be 9 or 12, so DiFAT_sz will never override UINT_MAX
	 */

	size_t DiFAT_sz = cfbd->hdr->_csectDif
		* (((1<<cfbd->hdr->_uSectorShift) / sizeof(cfbSectorID_t)) - 1)
		+ 109;

	if ( DiFAT_sz >= UINT_MAX ) {
		error( "DiFAT size is bigger than UINT_MAX : %lu", DiFAT_sz );
		return -1;
	}

	DiFAT = calloc( DiFAT_sz, sizeof(cfbSectorID_t) );

	if ( !DiFAT ) {
		error( "Out of memory" );
		return -1;
	}


	/*
	 * Retrieves the 109 first DiFAT entries, from the last bytes of
	 * the cfbHeader structure.
	 */

	memcpy( DiFAT, cfbd->hdr->_sectFat, 109 * sizeof(cfbSectorID_t) );


	cfbSectorID_t id     = 0;
	uint64_t      offset = 109 * sizeof(cfbSectorID_t);

	uint64_t cnt = 0;

	/* _uSectorShift is guaranted to be 9 or 12, so sectorSize will never be negative */
	uint32_t sectorSize = (1U<<cfbd->hdr->_uSectorShift)-4U;


	CFB_foreachSectorInDiFATChain( cfbd, buf, id ) {

		if ( buf == NULL ) {
			error( "Error retrieving sector %u (0x%08x) out of the DiFAT chain.", id, id );
			goto err;
		}

		memcpy( (unsigned char*)DiFAT+offset, buf, sectorSize );

		offset += sectorSize;
		cnt++;


		/*
		 * sector list should end with CFB_END_OF_CHAIN, but sometimes it does not,
		 * like in Fairlight's AAF. So we must count.
		 */

		if ( cnt >= cfbd->hdr->_csectDif )
			break;
	}

	free( buf );
	buf = NULL;

	/*
	 * Standard says DIFAT should end with a CFB_END_OF_CHAIN index,
	 * however it has been observed that some files end with CFB_FREE_SECT.
	 * So we consider it ok.
	 */
	if ( id != CFB_END_OF_CHAIN ) {
		debug( "Incorrect end of DiFAT Chain %u (0x%08x) : should be CFB_END_OF_CHAIN (0x%08x)", id, id, CFB_END_OF_CHAIN );
	}

	cfbd->DiFAT    = DiFAT;
	cfbd->DiFAT_sz = (uint32_t)DiFAT_sz;

	return 0;

err:
	free( DiFAT );
	free( buf );

	return -1;
}



/**
 * Retrieves the FAT (File Allocation Table). Requires
 * the DiFAT to be retrieved first.
 *
 * @param  cfbd Pointer to the CFB_Data structure.
 * @return      0.
 */

static int cfb_retrieveFAT( CFB_Data * cfbd )
{

	cfbSectorID_t *FAT    = NULL;
	uint32_t       FAT_sz = (((cfbd->hdr->_csectFat) * (1<<cfbd->hdr->_uSectorShift)))
	                      / sizeof(cfbSectorID_t);


	FAT = calloc( FAT_sz, sizeof(cfbSectorID_t) );

	if ( !FAT ) {
		error( "Out of memory" );
		return -1;
	}

	cfbd->fat    = FAT;
	cfbd->fat_sz = FAT_sz;



	unsigned char *buf    = NULL;
	cfbSectorID_t  id     = 0;
	uint64_t       offset = 0;

	CFB_foreachFATSectorIDInDiFAT( cfbd, id ) {

		if ( cfbd->DiFAT[id] == CFB_FREE_SECT ) {
			continue;
		}

		/* observed in fairlight's AAFs.. */
		if ( cfbd->DiFAT[id] == 0x00000000 && id > 0 ) {
			warning( "Got a NULL FAT index in the DiFAT @ %u, should be CFB_FREE_SECT.", id );
			continue;
		}

		buf = cfb_getSector( cfbd, cfbd->DiFAT[id] );

		if ( buf == NULL ) {
			error( "Error retrieving FAT sector %u (0x%08x).", id, id );
			return -1;
		}

		memcpy( ((unsigned char*)FAT)+offset, buf, (1<<cfbd->hdr->_uSectorShift) );

		free ( buf );

		offset += (1<<cfbd->hdr->_uSectorShift);
	}


	return 0;
}



/**
 * Retrieves the MiniFAT (Mini File Allocation Table).
 *
 * @param  cfbd Pointer to the CFB_Data structure.
 * @return      0.
 */

static int cfb_retrieveMiniFAT( CFB_Data * cfbd )
{

	uint32_t miniFat_sz = cfbd->hdr->_csectMiniFat * (1<<cfbd->hdr->_uSectorShift)
	                    / sizeof(cfbSectorID_t);

	cfbSectorID_t *miniFat = calloc( miniFat_sz, sizeof(cfbSectorID_t) );

	if ( !miniFat ) {
		error( "Out of memory" );
		return -1;
	}


	unsigned char *buf    = NULL;
	cfbSectorID_t  id     = cfbd->hdr->_sectMiniFatStart;
	uint64_t       offset = 0;

	CFB_foreachSectorInChain( cfbd, buf, id ) {
		if ( buf == NULL ) {
			error( "Error retrieving MiniFAT sector %u (0x%08x).", id, id );
			return -1;
		}

		memcpy( (unsigned char*)miniFat+offset, buf, (1<<cfbd->hdr->_uSectorShift) );

		free( buf );

		offset += (1<<cfbd->hdr->_uSectorShift);
	}


	cfbd->miniFat    = miniFat;
	cfbd->miniFat_sz = miniFat_sz;


	return 0;
}



/**
 * Retrieves the nodes (directories and files) of the
 * Compound File Tree, as an array of cfbNodes.
 *
 * Each node correspond to a cfbNode struct of 128 bytes
 * length. The nodes are stored in a dedicated FAT chain,
 * starting at the FAT sector[cfbHeader._sectDirStart].
 *
 * Once retrieved, the nodes are accessible through the
 * CFB_Data.nodes pointer. Each Node is then accessible by
 * its ID (a.k.a SID) :
 *
 * ```
	cfbNode *node = CFB_Data.nodes[ID];
 * ```
 *
 * @param cfbd Pointer to the CFB_Data structure.
 * @return     0.
 */

static int cfb_retrieveNodes( CFB_Data *cfbd )
{
	cfbd->nodes_cnt = getNodeCount( cfbd );

	cfbNode *node  = calloc( cfbd->nodes_cnt, sizeof(cfbNode) );

	if ( !node ) {
		error( "Out of memory" );
		return -1;
	}

	unsigned char *buf = NULL;
	cfbSectorID_t  id  = cfbd->hdr->_sectDirStart;
	cfbSID_t       i   = 0;


	if ( cfbd->hdr->_uSectorShift == 9 ) { /* 512 bytes sectors */

		CFB_foreachSectorInChain( cfbd, buf, id ) {

			if ( buf == NULL ) {
				error( "Error retrieving Directory sector %u (0x%08x).", id, id );
				return -1;
			}

			memcpy( &node[i++], buf,     CFB_NODE_SIZE );
			memcpy( &node[i++], buf+128, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+256, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+384, CFB_NODE_SIZE );

			free( buf );
		}
	}
	else if ( cfbd->hdr->_uSectorShift == 12 ) { /* 4096 bytes sectors */

		CFB_foreachSectorInChain( cfbd, buf, id ) {

			if ( buf == NULL ) {
				error( "Error retrieving Directory sector %u (0x%08x).", id, id );
				return -1;
			}

			memcpy( &node[i++], buf,      CFB_NODE_SIZE );
			memcpy( &node[i++], buf+128,  CFB_NODE_SIZE );
			memcpy( &node[i++], buf+256,  CFB_NODE_SIZE );
			memcpy( &node[i++], buf+384,  CFB_NODE_SIZE );
			memcpy( &node[i++], buf+512,  CFB_NODE_SIZE );
			memcpy( &node[i++], buf+640,  CFB_NODE_SIZE );
			memcpy( &node[i++], buf+768,  CFB_NODE_SIZE );
			memcpy( &node[i++], buf+896,  CFB_NODE_SIZE );
			memcpy( &node[i++], buf+1024, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+1152, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+1280, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+1408, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+1536, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+1664, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+1792, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+1920, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+2048, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+2176, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+2304, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+2432, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+2560, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+2688, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+2816, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+2944, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+3072, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+3200, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+3328, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+3456, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+3584, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+3712, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+3840, CFB_NODE_SIZE );
			memcpy( &node[i++], buf+3968, CFB_NODE_SIZE );

			free( buf );
		}
	}
	else {

		/* handle non-standard sector size, that is different than 512B or 4kB */
		/* TODO has not been tested yet, should not even exist anyway */

		warning( "Parsing non-standard sector size !!! (%u bytes)", (1<<cfbd->hdr->_uSectorShift) );

		/* _uSectorShift is guaranted to be 9 or 12, so nodesPerSect will never override UINT_MAX */
		uint32_t nodesPerSect = (1U<<cfbd->hdr->_uMiniSectorShift) / sizeof(cfbNode);


		CFB_foreachSectorInChain( cfbd, buf, id ) {
			if ( buf == NULL ) {
				error( "Error retrieving Directory sector %u (0x%08x).", id, id );
				return -1;
			}

			for ( i = 0; i < nodesPerSect; i++ ) {
				memcpy( &node[i], buf+(i * CFB_NODE_SIZE), CFB_NODE_SIZE );
			}

			free( buf );
		}
	}


	cfbd->nodes = node;

	return 0;
}



/**
 * Converts UTF-16 to UTF-8.
 *
 * @param w16buf   Pointer to a NULL terminated uint16_t UTF-16 array.
 * @param w16blen  Size of the w16buf array in bytes
 *
 * @return          New allocated buffer with UTF-8 string\n
 *                  NULL on failure.
 */

char * cfb_w16toUTF8( const uint16_t *w16buf, size_t w16blen )
{
	(void)w16blen;

	if ( !w16buf ) {
		return NULL;
	}

	return laaf_util_utf16Toutf8( w16buf );
}



/**
 * Retrieves a Node in the Compound File Tree by path.
 *
 * @param cfbd      Pointer to the CFB_Data structure.
 * @param path      Pointer to a NULL terminated char array, holding the Node path.
 * @param id        Next node index. This is used internaly by the function and should
 *                  be set to 0 when called by the user.
 *
 * @return          Pointer to the retrieved Node,\n
 *                  NULL on failure.
 */

cfbNode * cfb_getNodeByPath( CFB_Data *cfbd, const char *path, cfbSID_t id )
{

	/*
	 * begining of the first function call.
	 */

	if ( id == 0 ) {

		if ( path[0] == '/' && path[1] == 0x0000 ) {
			return &cfbd->nodes[0];
		}

		/*
		 * work either with or without "/Root Entry"
		 */

		if ( strncmp( path, "/Root Entry", 11 ) != 0 ) {
			id = cfbd->nodes[0]._sidChild;
		}
	}


	/*
	 * retrieves the first node's name from path
	 */

	uint32_t nameLen = 0;

	for( nameLen = 0; nameLen < strlen(path); nameLen++ ) {

		if ( nameLen == UINT_MAX ) {
			error( "Name length is bigger than UINT_MAX" );
			return NULL;
		}

		if ( nameLen > 0 && path[nameLen] == '/' ) {
			break;
		}
	}

	/*
	 * removes any leading '/'
	 */

	if ( path[0] == '/' ) {
		path++;
		nameLen--;
	}


	size_t nameUTF16Len = (nameLen+1)<<1;

	if ( nameUTF16Len >= INT_MAX ) {
		error( "Name length is bigger than INT_MAX" );
		return NULL;
	}

	char *ab = NULL;

	while ( 1 ) {

		if ( id >= cfbd->nodes_cnt ) {
			error( "Out of range Node index %u, max %u.", id, cfbd->nodes_cnt );
			return NULL;
		}

		ab = cfb_w16toUTF8( cfbd->nodes[id]._ab, cfbd->nodes[id]._cb );

		int rc = 0;

		if ( strlen(ab) == nameLen )
			rc = strncmp( path, ab, nameLen );
		else {
			rc = (int)nameUTF16Len - cfbd->nodes[id]._cb;
		}

		free( ab );

		/*
		 * Some node in the path was found.
		 */

		if ( rc == 0 ) {
			/*
			 * get full path length minus any terminating '/'
			 */

			size_t pathLen = strlen(path);

			if ( path[pathLen-1] == '/' )
				pathLen--;

			/*
			 * If pathLen equals node name length, then
			 * we got the target node. Else, move forward
			 * to next node in the path.
			 */

			if ( pathLen == nameLen )
				return &cfbd->nodes[id];
			else
				return cfb_getNodeByPath( cfbd, path+nameLen, cfbd->nodes[id]._sidChild );
		}
		else if ( rc > 0 )	id = cfbd->nodes[id]._sidRightSib;
		else if ( rc < 0 )	id = cfbd->nodes[id]._sidLeftSib;


		if ( (int32_t)id < 0 )
			return NULL;
	}
}



/**
 * Retrieves a child node of a parent startNode.
 *
 * @param cfbd      Pointer to the CFB_Data structure.
 * @param name      Pointer to a NULL terminated string, holding the name of the
 *                  searched node.
 * @param startNode Pointer to the parent Node of the searched node.
 *
 * @return          Pointer to the retrieved node,\n
 *                  NULL if not found.
 */

cfbNode * cfb_getChildNode( CFB_Data *cfbd, const char *name, cfbNode *startNode )
{
	int rc = 0;

	cfbSID_t id = cfb_getIDByNode( cfbd, &cfbd->nodes[startNode->_sidChild] );

	if ( id == UINT_MAX ) {
		error( "Could not retrieve id by node" );
		return NULL;
	}

	size_t nameUTF16Len = ((strlen(name)+1) << 1);

	if ( nameUTF16Len >= INT_MAX ) {
		error( "Name length is bigger than INT_MAX" );
		return NULL;
	}

	while ( 1 ) {

		if ( id >= cfbd->nodes_cnt ) {
			error( "Out of range Node index %u, max %u.", id, cfbd->nodes_cnt );
			return NULL;
		}

		char *nodename = cfb_w16toUTF8( cfbd->nodes[id]._ab, cfbd->nodes[id]._cb );

		if ( cfbd->nodes[id]._cb == nameUTF16Len )
			rc = strcmp( name, nodename );
		else {
			rc = (int)nameUTF16Len - cfbd->nodes[id]._cb;
		}

		free(nodename);
		/*
		 * Node found
		 */

		if ( rc == 0 )
			break;

		/*
		 * Not found, go right
		 */

		else if ( rc > 0 )
			id = cfbd->nodes[id]._sidRightSib;

		/*
		 * Not found, go left
		 */

		else if ( rc < 0 )
			id = cfbd->nodes[id]._sidLeftSib;



		/*
		 * Not found
		 */

		if ( id >= CFB_MAX_REG_SECT )
			break;
	}

	if ( rc == 0 ) {
		return &cfbd->nodes[id];
	}

	return NULL;
}



static cfbSID_t cfb_getIDByNode( CFB_Data *cfbd, cfbNode *node )
{
	cfbSID_t id = 0;

	for ( ; id < cfbd->nodes_cnt; id++ ) {
		if ( node == &cfbd->nodes[id] ) {
			return id;
		}
	}

	return UINT_MAX;
}



/**
 * Loops through each FAT sector in the Directory chain
 * to retrieve the total number of Directories (Nodes)
 *
 * @param cfbd Pointer to the CFB_Data structure.
 * @return     The retrieved Nodes count.
 */

static cfbSID_t getNodeCount( CFB_Data *cfbd )
{
	uint32_t      cnt = ( 1<<cfbd->hdr->_uSectorShift );
	cfbSectorID_t id  = cfbd->hdr->_sectDirStart;

	while ( id < CFB_MAX_REG_SECT ) {

		if ( id >= cfbd->fat_sz ) {
			error( "index (%u) > FAT size (%u).", id, cfbd->fat_sz );
			break;
		}

		id = cfbd->fat[id];

		cnt += ( 1<<cfbd->hdr->_uSectorShift );
	}

	return cnt / sizeof(cfbNode);
}


/**
 * @}
 */

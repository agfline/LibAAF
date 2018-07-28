
/*
 *	This file is part of LibCFB.
 *
 *	Copyright (c) 2017 Adrien Gesta-Fline
 *
 *	LibCFB is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Affero General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	any later version.
 *
 *	LibCFB is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Affero General Public License for more details.
 *
 *	You should have received a copy of the GNU Affero General Public License
 *	along with LibCFB. If not, see <http://www.gnu.org/licenses/>.
 */



/**
 *	@file LibCFB/LibCFB.c
 *	@brief Compound File Binary Library
 *	@author Adrien Gesta-Fline
 *	@version 0.1
 *	@date 04 october 2017
 *
 *	@ingroup LibCFB
 *	@addtogroup LibCFB
 *	@{
 *	@brief LibCFB is a library that allows to parse any Compound File Binary File Format
 *	(a.k.a *Structured Storage File Format*).
 *
 *	The specifications of the CFB can be found at https://www.amwa.tv/projects/MS-03.shtml
 *
 *	@note When using LibAAF, you should not have to talk to LibCFB directly.\n
 *	All the following steps are handled by LibAAF.
 *
 *	Interaction with LibCFB is done through the CFB_Data structure, which is allocated
 *	with cfb_alloc().
 *
 *	The first thing you will need to do in order to parse any CFB file is to allocate
 *	CFB_Data with cfb_alloc(). Then, you can "load" a file by calling cfb_load_file().
 *	It will open the file for reading, check if it is a regular CFB file, then retrieve
 *	all the CFB components (Header, DiFAT, FAT, MiniFAT) needed to later parse the file's
 *	directories (nodes) and streams.
 *
 *	**Example:**
 *	@code
 *	CFB_Data *cfbd = cfb_alloc();
 *	cfb_load_file( cfbd, "/path/to/file" );
 *	@endcode
 *
 *	Once the file is loaded, you can access the nodes with the functions
 *	cfb_getNodeByPath() and cfb_getChildNode(), and access a node's stream with the
 *	functions cfb_getStream() or cfb_foreachSectorInStream(). The former is prefered
 *	for small-size streams, since it allocates the entire stream to memory, while the
 *	later loops through each sector that composes a stream, better for the big ones.
 *
 *	**Example:**
 *	@code
 *	// Get the root "directory" (node, SID 0) "directly"
 *	cfbNode *root = cfbd->nodes[0];
 *
 *
 *	// Get a "file" (stream node) called "properties" inside the "Header" directory
 *	cfbNode *properties = cfb_getNodeByPath( cfbd, "/Header/properties", 0 );
 *
 *
 *	// Get the "properties" node stream
 *	unsigned char *stream    = NULL;
 *	uint64_t       stream_sz = 0;
 *
 *	cfb_getStream( cfbd, properties, &stream, &stream_sz );
 *
 *
 *	// Loop through each sector that composes the "properties" stream
 *	cfbSectorID_t sectorID = 0;
 *
 *	cfb_foreachSectorInStream( cfbd, properties, &stream, &stream_sz, &sectorID )
 *	{
 *		// do stuff..
 *	}
 * 	@endcode
 *
 *	Finaly, once you are done working with the file, you can close the file and free
 *	the CFB_Data and its content by simply calling cfb_release().
 *
 *	**Example:**
 *	@code
 *	cfb_release( &cfbd );
 *	@endcode
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>	// ceil()

#include "LibCFB.h"
#include "CFBTypes.h"
#include "../common/debug.h"







static int cfb_getFileSize( CFB_Data *cfbd );

static int cfb_openFile( CFB_Data *cfbd );

static uint64_t cfb_readFile( CFB_Data *cfbd, unsigned char *buf, uint64_t offset, uint64_t len );

static void cfb_closeFile( CFB_Data *cfbd );

static int cfb_is_valid( CFB_Data *cfbd );



static int cfb_retrieveFileHeader( CFB_Data *cfbd );

static int cfb_retrieveDiFAT( CFB_Data *cfbd );

static int cfb_retrieveFAT( CFB_Data * cfbd );

static int cfb_retrieveMiniFAT( CFB_Data * cfbd );

static int cfb_retrieveNodes( CFB_Data *cfbd );



static cfbSID_t getNodeCount( CFB_Data *cfbd );



static cfbSID_t cfb_getIDByNode( CFB_Data *cfbd, cfbNode *node );





/**
 *	Allocates a new CFB_Data structure.
 *
 *	@return Pointer to the newly allocated CFB_Data structure.
 */

CFB_Data * cfb_alloc()
{
	CFB_Data * cfbd = calloc( sizeof(CFB_Data), sizeof(unsigned char) );

	if ( cfbd == NULL )
		_error( "%s.\n", strerror(errno) );

	return cfbd;
}





/**
 *	fclose() the file pointer then frees all the components of
 *	the CFB_Data structure, including the structure itself.
 *
 *	@param cfbd Pointer to Pointer to the CFB_Data structure.
 */

void cfb_release( CFB_Data **cfbd )
{
	cfb_closeFile( *cfbd );


	free( (*cfbd)->DiFAT );

	free( (*cfbd)->fat );

	free( (*cfbd)->miniFat );

	cfbSID_t i = 0;

	/* TODO foreachNodeInFile() ? */
	while ( i < (*cfbd)->nodes_cnt )
	{
		free( (*cfbd)->nodes[i]);
		i += ( 1 << (*cfbd)->hdr->_uSectorShift ) / sizeof(cfbNode);
	}

	free( (*cfbd)->nodes );

	free( (*cfbd)->hdr );

	free( *cfbd );

	*cfbd = NULL;
}





/**
 *	Loads a Compound File Binary File, retrieves its Header, FAT,
 *	MiniFAT and Nodes. then sets the CFB_Data structure so it is
 *	ready for parsing the file. The user should call cfb_release()
 *	once he's done using the file.
 *
 *	@param  cfbd     Pointer to the CFB_Data structure.
 *	@param  file     Pointer to a NULL terminated string holding the file path.
 *
 *	@return          0 on success\n
 *	                 1 on error
 */

int cfb_load_file( CFB_Data *cfbd, const char * file )
{
	strncpy( cfbd->file, file, strlen(file) );


	if ( cfb_openFile( cfbd ) < 0 )
	{
		return -1;
	}

	if ( cfb_getFileSize( cfbd ) < 0 )
	{
		return -1;
	}

	if ( cfb_is_valid( cfbd ) == 0 )
	{
		return -1;
	}

	if ( cfb_retrieveFileHeader( cfbd ) < 0 )
	{
		_error( "Could not retrieve CFB header.\n" );
		return -1;
	}

	if ( cfb_retrieveDiFAT( cfbd ) < 0 )
	{
		_error( "Could not retrieve CFB DiFAT.\n" );
		return -1;
	}

	if ( cfb_retrieveFAT( cfbd ) < 0 )
	{
		_error( "Could not retrieve CFB FAT.\n" );
		return -1;
	}

	if ( cfb_retrieveMiniFAT( cfbd ) < 0 )
	{
		_error( "Could not retrieve CFB MiniFAT.\n" );
		return -1;
	}

	if ( cfb_retrieveNodes( cfbd ) < 0 )
	{
		_error( "Could not retrieve CFB Nodes.\n" );
		return -1;
	}

	return 0;
}



int cfb_new_file( CFB_Data *cfbd, const char *file, int sectSize )
{
	// calm GCC
	file = (file == NULL) ? NULL : NULL;

	if ( sectSize != 512 &&
		 sectSize != 4096 )
	{
		_error( "Only standard sector sizes (512 and 4096 bytes) are supported.\n" );
		return -1;
	}

	cfbHeader *hdr = malloc( sizeof(cfbHeader) );

	if ( cfbd->hdr == NULL )
	{
		_error( "%s.\n", strerror( errno ) );
		return -1;
	}

	hdr->_abSig = 0xe11ab1a1e011cfd0;

								// 33 is written by reference implementation.
	hdr->_uMinorVersion = 0x3e;	// 0x3e was seen in all AAF files

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
 *	Ensures the file is a valid Compound File Binary File.
 *
 *	@param  cfbd Pointer to the CFB_Data structure.
 *
 *	@return      1 if the file is valid,\n
 *	             0 otherwise.
 */

static int cfb_is_valid( CFB_Data *cfbd )
{
	uint64_t signature = 0;

	if ( cfbd->file_sz < sizeof(struct StructuredStorageHeader) )
	{
		_error( "Not a valid Compound File : File size is lower than header.\n" );
		return 0;
	}

	if ( cfb_readFile( cfbd, (unsigned char*)&signature, 0, sizeof(uint64_t) ) != sizeof(uint64_t) )
	{
		return 0;
	}

	if ( signature != 0xe11ab1a1e011cfd0 )
	{
		_error( "Not a valid Compound File : Wrong signature.\n" );
		return 0;
	}

	return 1;
}





/**
 *	Retrieves the Compound File Binary File size.
 *
 *	@param  cfbd Pointer to the CFB_Data structure.
 *	@return      0.
 */

static int cfb_getFileSize( CFB_Data * cfbd )
{
	fseek( cfbd->fp, 0L, SEEK_END );

	cfbd->file_sz = ftell( cfbd->fp );

	if ( cfbd->file_sz == 0 )
	{
		_error( "File is empty (filesize is 0 byte).\n" );
		return -1;
	}

	return 0;
}




/**
 *	Opens a given Compound File Binary File for reading.
 *
 *	@param  cfbd Pointer to the CFB_Data structure.
 *	@return      O.
 */

static int cfb_openFile( CFB_Data *cfbd )
{
	cfbd->fp = fopen( cfbd->file, "rb" );

	if ( cfbd->fp == NULL )
		_error( "%s.\n", strerror(errno) );

	return 0;
}




/**
 *	Reads a bytes block from the file. This function is
 *	called by cfb_getSector() and cfb_getMiniSector()
 *	that will do the sector index to file offset conversion.
 *
 *	@param cfbd   Pointer to the CFB_Data structure.
 *	@param buf    Pointer to the buffer that will hold the len bytes read.
 *	@param offset Position in the file the read should start.
 *	@param len    Number of bytes to read from the offset position.
 */

static uint64_t cfb_readFile( CFB_Data *cfbd, unsigned char *buf, uint64_t offset, uint64_t len )
{
	FILE *fp = cfbd->fp;


	if ( len + offset > cfbd->file_sz )
	{
		_error( "Requested data goes %lu bytes beyond the EOF : offset %lu | length %lu\n",
		        (len + offset) - cfbd->file_sz,
				offset,
				len );
		return 0;
	}


	int rc = fseek( fp, offset, SEEK_SET );

	if ( rc < 0 )
	{
		_error( "%s.\n", strerror(errno) );
		return 0;
	}



	uint64_t byteRead = fread( buf, sizeof(unsigned char), len, fp );

	if ( byteRead < len )
	{
		_warning( "Could only retrieve %lu bytes out of %lu requested.\n",
		          byteRead, len );
	}


	return byteRead;
}




/**
 *	Closes the file pointer hold by the CFB_Data.fp.
 *
 *	@param cfbd Pointer to the CFB_Data structure.
 */

static void cfb_closeFile( CFB_Data *cfbd )
{
	if ( cfbd->fp == NULL )
		return;

	if ( fclose( cfbd->fp ) != 0 )
	{
		_error( "%s.\n", strerror(errno) );
	}
}


























/**
 *	Retrieves a sector content from the FAT.
 *
 *	@param id   Index of the sector to retrieve in the FAT.
 *	@param cfbd Pointer to the CFB_Data structure.
 *	@return     Pointer to the sector's data bytes.
 */

unsigned char * cfb_getSector( CFB_Data *cfbd, cfbSectorID_t id )
{
	/*
	 * foreachSectorInChain() calls cfb_getSector() before
	 * testing the ID, so we have to ensure the ID is valid
	 * before we try to actualy get the sector.
	 */

	if ( id >= CFB_MAX_REG_SID )
		return NULL;


	if ( cfbd->fat_sz > 0 && id >= cfbd->fat_sz )
	{
		_error( "Asking for an out of range FAT sector @ index %u (max FAT index is %u)\n", id, cfbd->fat_sz );
		return NULL;
	}



	uint64_t sectorSize = (1 << cfbd->hdr->_uSectorShift);
	uint64_t fileOffset = (id + 1) << cfbd->hdr->_uSectorShift;



	unsigned char *buf = calloc( sectorSize, sizeof(unsigned char) );

	if ( buf == NULL )
	{
		_error( "%s.\n", strerror(errno) );
		return NULL;
	}



	if ( cfb_readFile( cfbd, buf, fileOffset, sectorSize ) == 0 )
	{
		free( buf );
		return NULL;
	}


	return buf;
}




/**
 *	Retrieves a mini-sector content from the MiniFAT.
 *
 *	@param id   Index of the mini-sector to retrieve in the MiniFAT.
 *	@param cfbd Pointer to the CFB_Data structure.
 *	@return     Pointer to the mini-sector's data bytes.
 */

unsigned char * cfb_getMiniSector( CFB_Data *cfbd, cfbSectorID_t id )
{
	/*
	 * foreachMiniSectorInChain() calls cfb_getMiniSector() before
	 * testing the ID, so we have to ensure the ID is valid before
	 * we try to actualy get the sector.
	 */

	if ( id >= CFB_MAX_REG_SID )
		return NULL;


	if ( cfbd->fat_sz > 0 && id >= cfbd->miniFat_sz )
	{
		_error( "Asking for an out of range MiniFAT sector @ index %u (Maximum MiniFAT index is %u)\n", id, cfbd->miniFat_sz );
		return NULL;
	}

	int MiniSectorSize = 1 << cfbd->hdr->_uMiniSectorShift;
	int SectorSize     = 1 << cfbd->hdr->_uSectorShift;



	unsigned char * buf = calloc( MiniSectorSize, sizeof(unsigned char) );

	if ( buf == NULL )
	{
		_error( "%s.\n", strerror(errno) );
		return NULL;
	}



	/* *** Retrieve the MiniSector file offset *** */
	cfbSectorID_t fatId  = cfbd->nodes[0]->_sectStart;
	uint64_t      offset = 0;
	uint32_t      i      = 0;

	/* Fat Divisor: allow to guess the number of mini-stream sectors per standard sector */
	unsigned int fatDiv = SectorSize / MiniSectorSize;

	/* move forward in the FAT's mini-stream chain to retrieve the sector we want. */
	for ( i = 0; i < id/fatDiv; i++ ) {   fatId = cfbd->fat[fatId];   }

	offset  = ((fatId + 1) << cfbd->hdr->_uSectorShift);
	offset += ((id % fatDiv ) << cfbd->hdr->_uMiniSectorShift);



	if ( cfb_readFile( cfbd, buf, offset, MiniSectorSize ) == 0 )
	{
		free( buf );
		return NULL;
	}


	return buf;
}




/**
 *	Retrieves a stream from a stream Node.
 *
 *	@param cfbd      Pointer to the CFB_Data structure.
 *	@param node      Pointer to the node to retrieve the stream from.
 *	@param stream    Pointer to the pointer where the stream data will be saved.
 *	@param stream_sz Pointer to an uint64_t where the stream size will be saved.
 *	@return          The retrieved stream size.
 */

uint64_t cfb_getStream( CFB_Data *cfbd, cfbNode *node, unsigned char **stream, uint64_t *stream_sz )
{

//	if ( node == NULL || node->_mse == STGTY_ROOT )
//		return;

//	Should not happen.. or could it ?
//	if ( node->_ulSizeLow < 1 )
//		return;


	uint64_t stream_len = cfb_getNodeStreamLen( cfbd, node );  //node->_ulSizeLow;


	*stream    = calloc( stream_len, sizeof(unsigned char) );

	if ( stream == NULL )
	{
		_error( "%s.\n", strerror( errno ) );
		return 0;
	}



	unsigned char *buf    = NULL;
	cfbSectorID_t  id     = node->_sectStart;
	uint64_t       offset = 0;
	uint64_t       cpy_sz = 0;

	if ( stream_len < cfbd->hdr->_ulMiniSectorCutoff ) // mini-stream
		cfb_foreachMiniSectorInChain( cfbd, buf, id )
		{

			cpy_sz = ( (stream_len - offset) < (uint64_t)(1<<cfbd->hdr->_uMiniSectorShift) ) ?
			           (stream_len - offset) : (uint64_t)(1<<cfbd->hdr->_uMiniSectorShift);

			memcpy( *stream+offset, buf, cpy_sz );
/*
			if ( ((stream_len) - offset) > (uint32_t)(1 << cfbd->hdr->_uMiniSectorShift) )
				memcpy( *(stream)+offset, buf, (1<<cfbd->hdr->_uMiniSectorShift) );
			else
				memcpy( *(stream)+offset, buf, stream_len - offset );
*/

			free( buf );

			offset += (1<<cfbd->hdr->_uMiniSectorShift);
		}
	else //
		cfb_foreachSectorInChain( cfbd, buf, id )
		{

			cpy_sz = ( (stream_len - offset) < (uint64_t)(1<<cfbd->hdr->_uSectorShift) ) ?
			           (stream_len - offset) : (uint64_t)(1<<cfbd->hdr->_uSectorShift);

			memcpy( *stream+offset, buf, cpy_sz );

			free( buf );

			offset += (1<<cfbd->hdr->_uSectorShift);
		}

	if ( stream_sz != NULL )
		*stream_sz = stream_len;

	return stream_len;
}




/**
 *	Loops through all the sectors that compose a stream
 *	and retrieve their content.
 *
 *	This function should be called through the macro cfb_foreachSectorInStream().
 *
 *	@param  cfbd      Pointer to the CFB_Data structure.
 *	@param  node      Pointer to the Node that hold the stream.
 *	@param  buf       Pointer to pointer to the buffer that will receive the sector's
 *	                  content.
 *	@param  bytesRead Pointer to a size_t that will receive the number of bytes retreived.
 *	                  This should be equal to the sectorSize, except for the last sector
 *	                  in which the stream might end before the end of the sector.
 *	@param  sectID    Pointer to the Node index.
 *	@return           1 if we have retrieved a sector with some data, \n
 *	                  0 if we have reached the #CFB_END_OF_CHAIN.
 */

int cfb__foreachSectorInStream( CFB_Data *cfbd, cfbNode *node, unsigned char **buf, size_t *bytesRead, cfbSectorID_t *sectID )
{

	if ( node == NULL )
		return 0;

	if ( *sectID >= CFB_MAX_REG_SID )
		return 0;

	/* is this possible ? */
//	if ( node->_ulSizeLow < 1 )
//		return 0;

	/* free the previously allocated buf, if any */
	if ( *buf != NULL )
	{
		free( *buf );
		*buf = NULL;
	}


	/* if *nodeID == 0, then it is the first function call */
	*sectID = ( *sectID == 0 ) ? node->_sectStart : *sectID;


	size_t stream_sz = node->_ulSizeLow;

	if ( stream_sz < cfbd->hdr->_ulMiniSectorCutoff )
	{		/* Mini-Stream */
			*buf       = cfb_getMiniSector( cfbd, *sectID );
			*bytesRead = (1<<cfbd->hdr->_uMiniSectorShift);
			*sectID    = cfbd->miniFat[*sectID];
	}
	else
	{		/* Stream */
			*buf       = cfb_getSector( cfbd, *sectID );
			*bytesRead = (1<<cfbd->hdr->_uSectorShift);
			*sectID    = cfbd->fat[*sectID];
	}


	// trim data length to match the EXACT stream size

	// if ( *sectID >= CFB_MAX_REG_SECT )
	//      *bytesRead = ( stream_sz % *bytesRead );
	//      *bytesRead = ( stream_sz % *bytesRead );


	return 1;
}



/**
 *	Retrieves the Header of the Compound File Binary.
 *	The Header begins at offset 0 and is 512 bytes long,
 *	regardless of the file's sector size.
 *
 *	@param  cfbd Pointer to the CFB_Data structure.
 *	@return      0 on success\n
 *	             1 on failure
 */

static int cfb_retrieveFileHeader( CFB_Data *cfbd )
{

	cfbd->hdr = calloc( sizeof(cfbHeader), sizeof(unsigned char) );

	if ( cfbd->hdr == NULL )
	{
		_error( "%s.\n", strerror(errno) );
		return -1;
	}


	if ( cfb_readFile( cfbd, (unsigned char*)cfbd->hdr, 0, sizeof(cfbHeader) ) == 0 )
	{
		free( cfbd->hdr );
		return -1;
	}


	return 0;
}




static int cfb_retrieveDiFAT( CFB_Data *cfbd )
{
	cfbSectorID_t * DiFAT = NULL;


	/*
	 *	Check DiFAT properties in header.
	 *	(Exemple AMWA aaf files.)
	 */

	cfbSectorID_t csectDif = 0;

	if ( cfbd->hdr->_csectFat > 109 )
	{
		csectDif = ceil( (float)((cfbd->hdr->_csectFat - 109) * 4) / (1<<cfbd->hdr->_uSectorShift) );
	}

	if ( csectDif != cfbd->hdr->_csectDif )
	{
		_warning( "cfbd->hdr->_csectDif value seems wrong (%u). Correcting from cfbd->hdr->_csectFat.\n", cfbd->hdr->_csectDif );

		cfbd->hdr->_csectDif = csectDif;
	}

	if ( csectDif == 0 && cfbd->hdr->_sectDifStart != CFB_END_OF_CHAIN )
	{
		_warning( "cfbd->hdr->_sectDifStart is 0x%08x (%u) but should be CFB_END_OF_CHAIN. Correcting.\n", cfbd->hdr->_sectDifStart, cfbd->hdr->_sectDifStart );

		cfbd->hdr->_sectDifStart = CFB_END_OF_CHAIN;
	}


	/*
	 *	DiFAT size is the number of FAT sector entries in the DiFAT chain.
	 */

	uint32_t DiFAT_sz = ( cfbd->hdr->_csectDif )
					    * (((1<<cfbd->hdr->_uSectorShift) / sizeof(cfbSectorID_t)) - 1)
						+ 109;



	DiFAT = calloc( DiFAT_sz, sizeof(cfbSectorID_t) );

	if ( DiFAT == NULL )
	{
		_error( "%s.\n", strerror( errno ) );
		return -1;
	}



	/*
	 *	Retrieve the 109 first DiFAT entries, from the last bytes of
	 *	the cfbHeader structure.
	 */

	memcpy( DiFAT, cfbd->hdr->_sectFat, 109 * sizeof(cfbSectorID_t) );


	unsigned char  *buf    = NULL;
	cfbSectorID_t   id     = 0; //cfbd->hdr->_sectDifStart;
	uint64_t        offset = 109 * sizeof(cfbSectorID_t);

	uint64_t cnt = 0;

	cfb_foreachSectorInDiFATChain( cfbd, buf, id )
	{
		memcpy( (unsigned char*)DiFAT+offset, buf, (1<<cfbd->hdr->_uSectorShift)-4 );

		offset += (1 << cfbd->hdr->_uSectorShift) - 4;
		cnt++;

		/*
		 *	If we count more DiFAT sector when parsing than
		 *	there should be, it means the sector list does
		 *	not end by a proper CFB_END_OF_CHAIN.
		 */

		if ( cnt >= cfbd->hdr->_csectDif )
			break;
	}

	free( buf );

	if ( id != CFB_END_OF_CHAIN )
		_warning( "Incorrect end of DiFAT Chain 0x%08x (%d)\n", id, id );


	cfbd->DiFAT    = DiFAT;
	cfbd->DiFAT_sz = DiFAT_sz;

	return 0;
}





/**
 *	Retrieves the FAT (File Allocation Table). Requires
 *	the DiFAT to be retrieved first.
 *
 *	@param  cfbd Pointer to the CFB_Data structure.
 *	@return      0.
 */

static int cfb_retrieveFAT( CFB_Data * cfbd )
{

	cfbSectorID_t *FAT    = NULL;
	uint64_t       FAT_sz = (((cfbd->hdr->_csectFat) * (1<<cfbd->hdr->_uSectorShift)))
	                      / sizeof(cfbSectorID_t);


	FAT = calloc( FAT_sz, sizeof(cfbSectorID_t) );

	if ( FAT == NULL )
	{
		_error( "%s.\n", strerror( errno ) );
		return -1;
	}




	unsigned char *buf    = NULL;
	cfbSectorID_t  id     = 0;
	uint64_t       offset = 0;

	cfb_foreachFATSectorIDInDiFAT( cfbd, id )
	{

		if ( cfbd->DiFAT[id] == CFB_FREE_SECT )
			continue;

		if ( cfbd->DiFAT[id] == 0x00000000 && id > 0 ) // observed in fairlight's AAFs..
		{
			_warning( "Got a NULL FAT index in the DiFAT @ %u, should be CFB_FREE_SECT.\n", id );
			continue;
		}

		buf = cfb_getSector( cfbd, cfbd->DiFAT[id] );

		memcpy( ((unsigned char*)FAT)+offset, buf, (1<<cfbd->hdr->_uSectorShift) );

		free ( buf );

		offset += (1<<cfbd->hdr->_uSectorShift);
	}


	cfbd->fat    = FAT;
	cfbd->fat_sz = FAT_sz;

	return 0;
}




/**
 *	Retrieves the MiniFAT (Mini File Allocation Table).
 *
 *	@param  cfbd Pointer to the CFB_Data structure.
 *	@return      0.
 */

static int cfb_retrieveMiniFAT( CFB_Data * cfbd )
{

	uint64_t miniFat_sz = cfbd->hdr->_csectMiniFat * (1<<cfbd->hdr->_uSectorShift)
	                    / sizeof(cfbSectorID_t);

	cfbSectorID_t * miniFat = calloc( miniFat_sz, sizeof(cfbSectorID_t) );

	if ( miniFat == NULL )
	{
		_error( "%s.\n", strerror(errno) );
		return -1;
	}



	unsigned char *buf    = NULL;
	cfbSectorID_t  id     = cfbd->hdr->_sectMiniFatStart;
	uint64_t       offset = 0;


	cfb_foreachSectorInChain( cfbd, buf, id )
	{
		memcpy( (unsigned char*)miniFat+offset, buf, (1<<cfbd->hdr->_uSectorShift) );

		free( buf );

		offset += (1<<cfbd->hdr->_uSectorShift);
	}


	cfbd->miniFat    = miniFat;
	cfbd->miniFat_sz = miniFat_sz;


	return 0;
}



/**
 *	Retrieves the nodes (directories and files) of the
 *	Compound File Tree, as an array of cfbNodes.
 *
 *	Each node correspond to a cfbNode struct of 128 bytes
 *	length. The nodes are stored in a dedicated FAT chain,
 *	starting at the FAT sector[cfbHeader._sectDirStart].
 *
 *	Once retrieved, the nodes are accessible through the
 *	CFB_Data.nodes pointer. Each Node is then accessible by
 *	its ID (a.k.a SID) :
 *
 *	```
	cfbNode *node = CFB_Data.nodes[ID];
 *	```
 *
 *	@param cfbd Pointer to the CFB_Data structure.
 *	@return     0.
 */

static int cfb_retrieveNodes( CFB_Data *cfbd )
{

	cfbd->nodes_cnt = getNodeCount( cfbd );


	cfbNode **node  = calloc( cfbd->nodes_cnt, sizeof(cfbNode*) );

	if ( node == NULL )
	{
		_error( "%s.\n", strerror( errno ) );
		return -1;
	}

	unsigned char *buf = NULL;
	cfbSectorID_t  id  = cfbd->hdr->_sectDirStart;
	cfbSID_t       i   = 0;


	if ( cfbd->hdr->_uSectorShift == 9 ) // 512 bytes sectors
		cfb_foreachSectorInChain( cfbd, buf, id )
		{
			node[i++] = (cfbNode*)buf;
			node[i++] = (cfbNode*)(buf+128);
			node[i++] = (cfbNode*)(buf+256);
			node[i++] = (cfbNode*)(buf+384);
		}
	else if ( cfbd->hdr->_uSectorShift == 12 ) // 4096 bytes sectors
		cfb_foreachSectorInChain( cfbd, buf, id )
		{
			node[i++] = (cfbNode*)buf;
			node[i++] = (cfbNode*)(buf+128);
			node[i++] = (cfbNode*)(buf+256);
			node[i++] = (cfbNode*)(buf+384);
			node[i++] = (cfbNode*)(buf+512);
			node[i++] = (cfbNode*)(buf+640);
			node[i++] = (cfbNode*)(buf+768);
			node[i++] = (cfbNode*)(buf+896);
			node[i++] = (cfbNode*)(buf+1024);
			node[i++] = (cfbNode*)(buf+1152);
			node[i++] = (cfbNode*)(buf+1280);
			node[i++] = (cfbNode*)(buf+1408);
			node[i++] = (cfbNode*)(buf+1536);
			node[i++] = (cfbNode*)(buf+1664);
			node[i++] = (cfbNode*)(buf+1792);
			node[i++] = (cfbNode*)(buf+1920);
			node[i++] = (cfbNode*)(buf+2048);
			node[i++] = (cfbNode*)(buf+2176);
			node[i++] = (cfbNode*)(buf+2304);
			node[i++] = (cfbNode*)(buf+2432);
			node[i++] = (cfbNode*)(buf+2560);
			node[i++] = (cfbNode*)(buf+2688);
			node[i++] = (cfbNode*)(buf+2816);
			node[i++] = (cfbNode*)(buf+2944);
			node[i++] = (cfbNode*)(buf+3072);
			node[i++] = (cfbNode*)(buf+3200);
			node[i++] = (cfbNode*)(buf+3328);
			node[i++] = (cfbNode*)(buf+3456);
			node[i++] = (cfbNode*)(buf+3584);
			node[i++] = (cfbNode*)(buf+3712);
			node[i++] = (cfbNode*)(buf+3840);
			node[i++] = (cfbNode*)(buf+3968);
		}
	else    /* handle non-standard sector size, that is different than 512B or 4kB */
	{       /* TODO has not been tested yet */
		uint32_t nodesPerSect = (1 << cfbd->hdr->_uMiniSectorShift) / sizeof(cfbNode);

		cfb_foreachSectorInChain( cfbd, buf, id )
			for ( i = 0; i < nodesPerSect; i++ )
				node[i] = (cfbNode*)(buf + (i * 128));
	}


	cfbd->nodes = node;

	return 0;
}



/**
 *	Retrieves a Node in the Compound File Tree by path.
 *
 *	@param cfbd      Pointer to the CFB_Data structure.
 *	@param path      Pointer to a NULL terminated char array, holding the Node path.
 *	@param id        Next node index. This is used internaly by the function and should
 *	                 be set to 0 when called by the user.
 *
 *	@return          Pointer to the retrieved Node,\n
 *	                 NULL on failure.
 */

cfbNode * cfb_getNodeByPath( CFB_Data *cfbd, const char *path, cfbSID_t id )
{

	/*
	 *	begining of the first function call.
	 */

	if ( id == 0 )
	{
		if ( path[0] == '/' && path[1] == 0x00 )
			return cfbd->nodes[0];

		/*
		 *	work either with or without "/Root Entry"
		 */

		if ( strncmp( path, "/Root Entry", 11 ) != 0 )
			id = cfbd->nodes[0]->_sidChild;
	}



	uint32_t l = 0;

	/*
	 *	retrieves the first node's name from path
	 */

	for( l = 0; l < strlen(path); l++ )
		if ( l > 0 && path[l] == '/' )
			break;

	/*
	 *	removes any leading '/'
	 */

	if ( path[0] == '/' )
	{
		path++;
		l--;
	}


	while ( 1 )
	{

		if ( id >= cfbd->nodes_cnt )
		{
			_error( "Out of range Node index %d, max %u.\n", id, cfbd->nodes_cnt );
			return NULL;
		}


		char   *ab = cfb_utf16toa( cfbd->nodes[id]->_ab, cfbd->nodes[id]->_cb );

		int32_t rc = 0;

		if ( strlen(ab) == l )
			rc = strncmp( path, ab, l );
		else
			rc = l - strlen(ab);

		free( ab );



		/*
		 *	Some node in the path was found.
		 */

		if ( rc == 0 )
		{
			/*
			 *	get full path length minus any terminating '/'
			 */

			uint32_t pathLen = strlen(path);

			if ( path[pathLen-1] == '/' )
				pathLen--;

			/*
			 *	If pathLen equals node name length, then
			 *	we got the target node. Else, move forward
			 *	to next node in the path.
			 */

			if ( pathLen == l )
				return cfbd->nodes[id];
			else
				return cfb_getNodeByPath( cfbd, path+l, cfbd->nodes[id]->_sidChild );
		}
		else if ( rc > 0 )	id = cfbd->nodes[id]->_sidRightSib;
		else if ( rc < 0 )	id = cfbd->nodes[id]->_sidLeftSib;


		if ( (int32_t)id < 0 )
			return NULL;
	}
}







/**
 *	Retrieves a child node of a parent startNode.
 *
 *	@param cfbd      Pointer to the CFB_Data structure.
 *	@param name      Pointer to a NULL terminated string, holding the name of the
 *	                 searched node.
 *	@param startNode Pointer to the parent Node of the searched node.
 *
 *	@return          Pointer to the retrieved node,\n
 *	                 NULL if not found.
 */

cfbNode * cfb_getChildNode( CFB_Data *cfbd, const char *name, cfbNode *startNode )
{

	int32_t   rc = 0;

	/** @TODO : cfb_getIDByNode should be quiker (macro ?) */
	cfbSID_t  id = cfb_getIDByNode( cfbd, cfbd->nodes[startNode->_sidChild] );

	uint32_t  nameLen   = strlen( name );

	uint16_t *utf16name = cfb_atoutf16( name, nameLen );
	uint16_t  utf16len  = ((nameLen + 1) << 1);  /* includes NULL Unicode */


	while ( 1 )
	{
		if ( id >= cfbd->nodes_cnt )
		{
			_error( "Out of range Node index %u, max %u.\n", id, cfbd->nodes_cnt );
			return NULL;
		}


		if ( cfbd->nodes[id]->_cb == utf16len )
			rc = memcmp( utf16name, cfbd->nodes[id]->_ab, cfbd->nodes[id]->_cb );
		else
			rc = utf16len - cfbd->nodes[id]->_cb;


		/*
		 *	Node found
		 */

		if ( rc == 0 )
			break;

		/*
		 *	Not found, go right
		 */

		else if ( rc > 0 )
			id = cfbd->nodes[id]->_sidRightSib;

		/*
		 *	Not found, go left
		 */

		else if ( rc < 0 )
			id = cfbd->nodes[id]->_sidLeftSib;



		/*
		 *	Not found
		 */

		if ( id >= CFB_MAX_REG_SID )
			break;

	}

	free( utf16name );

	if ( rc == 0 )
		return cfbd->nodes[id];

	return NULL;
}





static cfbSID_t cfb_getIDByNode( CFB_Data *cfbd, cfbNode *node )
{
	cfbSID_t id = 0;

	for ( ; id < cfbd->nodes_cnt; id++ )
		if ( node == cfbd->nodes[id] )
			return id;

	return -1;
}






/**
 *	Loops through each FAT sector in the Directory chain
 *	to retrieve the total number of Directories (Nodes)
 *
 *	@param cfbd Pointer to the CFB_Data structure.
 *	@return     The retrieved Nodes count.
 */

static cfbSID_t getNodeCount( CFB_Data *cfbd )
{
	uint64_t      cnt = ( 1<<cfbd->hdr->_uSectorShift );
	cfbSectorID_t id  = cfbd->hdr->_sectDirStart;

	while ( id < CFB_MAX_REG_SID )
	{
		id = cfbd->fat[id];

		cnt += ( 1<<cfbd->hdr->_uSectorShift );
	}

	return cnt / sizeof(cfbNode);
}









/**
 *	Converts a Compound File String (UTF-16 wide char array)
 *	to an ASCII NULL terminated char array.
 *
 *	@param  wstr Pointer to a 2 * wlen bytes long uint16_t array.
 *	@param  wlen Length of the wstr uint16_t array, including the NULL terminated byte.
 *
 *	@return      Pointer to the NULL terminated ASCII string.
 */

char * cfb_utf16toa( uint16_t *wstr, uint16_t wlen )
{
	uint16_t i = 0;


	uint16_t len  = (wlen >> 1);

	char    *astr = malloc( len + 1 );


	for ( i = 0; i < len; i++ )
		astr[i] = (char)wstr[i];


	return astr;
}




/**
 *	Converts a Compound File String (UTF-16 wide char array)
 *	to an ASCII NULL terminated char array.
 *
 *	@param  astr Pointer to a NULL terminated const char array.
 *	@param  alen Length of the astr char array, including or excluding the NULL terminated
 *	             byte.
 *
 *	@return      Pointer to the NULL terminated ASCII string.
 */

uint16_t * cfb_atoutf16( const char *astr, uint16_t alen )
{
	uint16_t i = 0;

	uint16_t *wstr = calloc( alen+1, sizeof(uint16_t) );

	for ( i = 0; i < alen; i++ )
		wstr[i] = (uint16_t)astr[i];


//	wstr[--i] = 0x0000;

	return wstr;
}







static int compareStrings( const void *a, const void *b )
{
    return strcmp( (const char *)a, (const char *)b );
}

void cfb_printEachNodePath( CFB_Data *cfbd, uint32_t prevPath, char strArray[][CFB_PATH_NAME_SZ], uint32_t *str_i, cfbNode *node )
{

	/*
	 *	the begining of the first function call.
	 */

	if ( node == NULL )
	{
		node     = cfbd->nodes[0];
		strArray = calloc( cfbd->nodes_cnt * CFB_PATH_NAME_SZ, sizeof(char) );
	}



	uint32_t thisPath = (*str_i);

	char *name = cfb_utf16toa( node->_ab, node->_cb );

	snprintf( strArray[thisPath], CFB_PATH_NAME_SZ, "%s/%s", strArray[prevPath], name );

	(*str_i)++;

	free( name );




	if ( (int32_t)node->_sidChild > 0 )
		cfb_printEachNodePath( cfbd, thisPath, strArray, str_i, cfbd->nodes[node->_sidChild]    );

	if ( (int32_t)node->_sidLeftSib > 0 )
		cfb_printEachNodePath( cfbd, prevPath, strArray, str_i, cfbd->nodes[node->_sidLeftSib]  );

	if ( (int32_t)node->_sidRightSib > 0 )
		cfb_printEachNodePath( cfbd, prevPath, strArray, str_i, cfbd->nodes[node->_sidRightSib] );



	/*
	 *	the end of the first function call,
	 *	recursion is over.
	 */

	if ( node == cfbd->nodes[0] )
	{
		uint32_t i = 0;

		qsort( strArray, *str_i, CFB_PATH_NAME_SZ, compareStrings );

		for ( i = 0; i < cfbd->nodes_cnt && strArray[i][0] != 0x00; i++ )
			printf( "%s\n", strArray[i] );

		free( strArray );
	}

}




void cfb_printHeader( CFB_Data *cfbd )
{
	cfbHeader *cfbh = cfbd->hdr;

//	printf( "\n" );

	printf( "_abSig              : 0x%08lx\n", cfbh->_abSig );
/*
	printf( "0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
		cfbh->_abSig[0],
		cfbh->_abSig[1],
		cfbh->_abSig[2],
		cfbh->_abSig[3],
		cfbh->_abSig[4],
		cfbh->_abSig[5],
		cfbh->_abSig[6],
		cfbh->_abSig[7]
	);
*/
	printf( "_clsId              : "
			"{0x%08x, 0x%04x, 0x%04x, "
			"{ 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x }}\n",
			cfbh->_clsid.Data1,
			cfbh->_clsid.Data2,
			cfbh->_clsid.Data3,

			cfbh->_clsid.Data4[0],
			cfbh->_clsid.Data4[1],
			cfbh->_clsid.Data4[2],
			cfbh->_clsid.Data4[3],
			cfbh->_clsid.Data4[4],
			cfbh->_clsid.Data4[5],
			cfbh->_clsid.Data4[6],
			cfbh->_clsid.Data4[7]
		);

/*
	printf( " 0x%02x 0x%02x 0x%02x 0x%02x (%c%c%c%c)\n"
			"             0x%02x 0x%02x\n"
			"             0x%02x 0x%02x\n"
			"             0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
		*((unsigned char*)(&cfbh->_clsid.Data1)+0),
		*((unsigned char*)(&cfbh->_clsid.Data1)+1),
		*((unsigned char*)(&cfbh->_clsid.Data1)+2),
		*((unsigned char*)(&cfbh->_clsid.Data1)+3),
		*((unsigned char*)(&cfbh->_clsid.Data1)+0),
		*((unsigned char*)(&cfbh->_clsid.Data1)+1),
		*((unsigned char*)(&cfbh->_clsid.Data1)+2),
		*((unsigned char*)(&cfbh->_clsid.Data1)+3),
		*((unsigned char*)(&cfbh->_clsid.Data2)+0),
		*((unsigned char*)(&cfbh->_clsid.Data2)+1),
		*((unsigned char*)(&cfbh->_clsid.Data3)+0),
		*((unsigned char*)(&cfbh->_clsid.Data3)+1),
		cfbh->_clsid.Data4[0],
		cfbh->_clsid.Data4[1],
		cfbh->_clsid.Data4[2],
		cfbh->_clsid.Data4[3],
		cfbh->_clsid.Data4[4],
		cfbh->_clsid.Data4[5],
		cfbh->_clsid.Data4[6],
		cfbh->_clsid.Data4[7]
	);
*/
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


void cfb_printFat( cfbSectorID_t *fat, const char *str, int size, int start, int end )
{
	int i = 0;

	while ( i < size )
	{

		if ( end > 0 && end < i )
			break;

		if ( i >= start )
			printf( "%s[%u] : 0x%08x\n", str, i, fat[i] );

		i++;
	}
}


void cfb_printNode( cfbNode *node, const char * (*clsidToText)( cfbCLSID_t * ) )
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

		printf( " _clsid       :" );
		printf( " {0x%08x, 0x%04x, 0x%04x, { 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x }}\n",
			node->_clsId.Data1,
			node->_clsId.Data2,
			node->_clsId.Data3,

			node->_clsId.Data4[0],
			node->_clsId.Data4[1],
			node->_clsId.Data4[2],
			node->_clsId.Data4[3],
			node->_clsId.Data4[4],
			node->_clsId.Data4[5],
			node->_clsId.Data4[6],
			node->_clsId.Data4[7]
		);

		if ( clsidToText != NULL )
			printf( " _            : (%s)\n", clsidToText( &(node->_clsId) ) );

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

		printf( " _ulSizeHigh  : 0x%04x (%d)\n", node->_ulSizeHigh, node->_ulSizeHigh );
	}

	printf( "\n\n" );

	if ( asciiName )
		free( asciiName );

}


void cfb_printStream( unsigned char * stream, size_t stream_sz )
{
	if ( stream == NULL )
	{
		_warning( "Stream pointer is NULL.\n" );
		return;
	}


	uint32_t i = 0;

	char hex[48];
	char str[32];

	uint32_t count   = 0;
	uint32_t lineLen = 0;

	printf( " _____________________________ Hex Dump _____________________________\n\n" );

	while ( count < stream_sz )
	{
		lineLen = (stream_sz - count) / 16;

		if ( lineLen <= 0 )
			lineLen = (stream_sz) % 16;
		else
			lineLen = 16;


		memset( &hex, 0x20, 48 );
		memset( &str, 0x00, 32 );

		for ( i=0; i < lineLen; i++ )
		{
			snprintf( &hex[i*3], 48-(i*3), "%02x ", *(unsigned char*)(stream+count+i) );

			if ( *(unsigned char*)(stream+count+i) > 0x1F &&
				 *(unsigned char*)(stream+count+i) < 0x7F )
					str[i] = *(unsigned char*)(stream+count+i);
			else
					str[i] = '.';
		}

		hex[47]  = 0x00;

		if ( lineLen < 16 )
			hex[i*3] = 0x20;

		count += lineLen;

		printf( " %s  |  %s\n", hex, str );

//		if ( count > 1000 )
//			break;
	}

	printf( " ____________________________________________________________________\n\n" );

}

/**
 *	@}
 */

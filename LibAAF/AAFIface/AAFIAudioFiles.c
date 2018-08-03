#include <stdio.h>
#include <string.h>
#include <unistd.h> // readlink()
#include <wchar.h>

#include <sndfile.h>

#include "../libAAF.h"
#include "../common/utils.h"
#include "../common/debug.h"

// #define SIGNED_SIZEOF(x) sizeof(x)



static sf_count_t vfget_filelen( void *user_data );
static sf_count_t vfseek( sf_count_t offset, int whence, void *user_data );
static sf_count_t vfread( void *ptr, sf_count_t count, void *user_data );
static sf_count_t vfwrite( const void *ptr, sf_count_t count, void *user_data );
static sf_count_t vftell( void *user_data );

static const char * sf_format_to_file_ext( uint32_t format );
static int32_t sf_format_to_samplesize( uint32_t format );
static int32_t samplesize_to_PCM_sf_format( uint32_t samplesize );

// static char * eascii_to_ascii( char *str );



static const char * sf_format_to_file_ext( uint32_t format )
{
    switch ( format & SF_FORMAT_TYPEMASK )
    {
        case SF_FORMAT_WAV:         return "wav";
        case SF_FORMAT_RF64:        return "wav";
        case SF_FORMAT_AIFF:        return "aiff";
        case SF_FORMAT_AU:          return "au";
        case SF_FORMAT_RAW:         return "raw";
        case SF_FORMAT_W64:         return "w64";

        default:                    break;
    }

    _error( "Could not retrieve sample size from libsndfile format.\n" );
    return NULL;
}



static int32_t sf_format_to_samplesize( uint32_t format )
{
    switch ( format & SF_FORMAT_SUBMASK )
    {
        case SF_FORMAT_PCM_U8:
        case SF_FORMAT_PCM_S8:      return  8;
        case SF_FORMAT_PCM_16:      return 16;
        case SF_FORMAT_PCM_24:      return 24;
        case SF_FORMAT_PCM_32:      return 32;
        case SF_FORMAT_FLOAT:       return 32;
        case SF_FORMAT_DOUBLE:      return 64;

        // case SF_FORMAT_DPCM_8:      return  8;
        // case SF_FORMAT_DPCM_16:     return 16;
        //
        // case SF_FORMAT_DWVW_12:     return 12;
        // case SF_FORMAT_DWVW_16:     return 16;
        // case SF_FORMAT_DWVW_24:     return 24;
        //
        // case SF_FORMAT_ALAC_16:     return 16;
        // case SF_FORMAT_ALAC_20:     return 20;
        // case SF_FORMAT_ALAC_24:     return 24;
        // case SF_FORMAT_ALAC_32:     return 32;

        default:                    break;
    }

    _error( "Could not retrieve sample size from libsndfile format.\n" );
    return -1;
}




static int32_t samplesize_to_PCM_sf_format( uint32_t samplesize )
{
    int32_t format = 0 | SF_FORMAT_RAW;

    switch ( samplesize )
    {
        case  8:    format |= SF_FORMAT_PCM_S8;     break;
        case 16:    format |= SF_FORMAT_PCM_16;     break;
        case 24:    format |= SF_FORMAT_PCM_24;     break;
        case 32:    format |= SF_FORMAT_PCM_32;     break;

        default:

            _error( "Could not build libsndfile format from sample size %u.\n", samplesize );
            return -1;
    }

    return format;
}

//
// const char ASCII[] = {
//     '.', '.', '.', '.', '.', '.', '.', '.',
//     '.', '.', '.', '.', '.', '.', '.', '.',
//     '.', '.', '.', '.', '.', '.', '.', '.',
//     '.', '.', '.', '.', '.', '.', '.', '.',
//
//     ' ', '!', '"', '#', '$', '%', '&', '\'',
//     '(', ')', '*', '+', ',', '-', '.', '/',
//     '0', '1', '2', '3', '4', '5', '6', '7',
//     '8', '9', ':', ';', '<', '=', '>', '?',
//
//     '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
//     'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
//     'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
//     'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
//
//     '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
//     'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
//     'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
//     'x', 'y', 'z', '{', '|', '}', '~', '.',
//
//     '.', ' ', ',', '.', '"', '.', '.', '.',
//     '^', '%', 'S', '<', 'E', ' ', 'Z', ' ',
//     ' ', '\'', '\'', '"', '"', '.', '-', '-',
//     '~', '.', 'S', '>', 'e', ' ', 'z', 'Y',
//
//     ' ', 'i', '.', '.', '.', '.', '|', '.',
//     '.', '.', '.', '"', '.', '-', '.', '.',
//     '.', '.', '2', '3', '\'', 'u', '.', '.',
//     '.', '1', '.', '"', '.', '.', '.', '?',
//
//     'A', 'A', 'A', 'A', 'A', 'A', 'E', 'C',
//     'E', 'E', 'E', 'E', 'I', 'I', 'I', 'I',
//     '.', 'N', 'O', 'O', 'O', 'O', 'O', 'x',
//     'O', 'U', 'U', 'U', 'U', 'Y', '.', 'B',
//
//     'a', 'a', 'a', 'a', 'a', 'a', 'e', 'c',
//     'e', 'e', 'e', 'e', 'i', 'i', 'i', 'i',
//     '.', 'n', 'o', 'o', 'o', 'o', 'o', '/',
//     'o', 'u', 'u', 'u', 'u', 'y', '.', 'y'
// };
//
// char * eascii_to_ascii( char *str )
// {
//     int len = strlen( str );
//
//     int i = 0;
//
//     for ( ; i < len; i++ )
//     {
//         str[i] = ASCII[(uint8_t)str[i]];
//     }
//
//     return str;
// }




int parse_audio_summary( AAF_Iface *aafi, aafiAudioEssence *audioEssence )
{
    SF_VIRTUAL_IO  sfvirtual;
    VIO_DATA       user_data;
    SF_INFO        sfinfo;
    SNDFILE       *file = NULL;


    sfvirtual.get_filelen = vfget_filelen;
	sfvirtual.seek = vfseek;
	sfvirtual.read = vfread;
	sfvirtual.write = vfwrite;
    sfvirtual.tell = vftell;

    user_data.length = 0;
    user_data.offset = 0;

    memset( &sfinfo, 0x00, sizeof(SF_INFO) );



    /* We get the AIFC/WAVE file header from summary */

    user_data.data   = audioEssence->summary->val;
    user_data.length = audioEssence->summary->len;



    /* Open stream with libsndfile */

    file = sf_open_virtual( &sfvirtual, SFM_READ, &sfinfo, &user_data );

    if ( file == NULL )
    {
        if ( audioEssence->is_embedded )
        {
            /*
             *  The summary should be a copy of the header without audio samples.
             *  It has been seen (on fairlight's AIFC-AAF) that the summary does
             *  not contains the full header and stop right before the SSND chunk.
             *  In that case, libsndfile does not handle it.
             *  In that case we fallback on the first stream sector to retrieve
             *  full WAVE/AIFC header.
             */

		    _warning( "libsndfile could not read descriptor summary : %s.\n", sf_strerror( NULL ) );
            _warning( "Falling back on audio file stream node.." );

            uint32_t   id   = 0;
            size_t     sectorLen  = 0;
            aafByte_t *sectorBuf  = NULL;

            memset( &user_data, 0x00, sizeof(VIO_DATA) );

            cfb_foreachSectorInStream( aafi->aafd->cfbd, audioEssence->node, &sectorBuf, &sectorLen, &id )
            {
                user_data.data   = sectorBuf;
                user_data.length = sectorLen;

                break;
            }

            file = sf_open_virtual( &sfvirtual, SFM_READ, &sfinfo, &user_data );

            free( sectorBuf );

            if ( file == NULL )
            {
                printf( "fail\n" );
                return -1;
            }

            printf( "success\n" );
        }
        else
        {
            _error( "libsndfile could not read descriptor summary : %s.\n", sf_strerror( NULL ) );
		    return -1 ;
        }
    }



    /* Extract usefull data */

    audioEssence->format     = sfinfo.format;
    audioEssence->channels   = sfinfo.channels;
    audioEssence->samplerate = sfinfo.samplerate;
    audioEssence->samplesize = sf_format_to_samplesize( audioEssence->format );



    /*
     *  The length was retrieved by retrieve_EssenceData() and correspond
     *  to the full stream length, including file header. Since we need to
     *  get only audio data stream length to do proper duration calculation,
     */

    SF_CHUNK_INFO	    chunk_info;
    SF_CHUNK_ITERATOR  *iterator;
    int err = 0;

    memset( &chunk_info, 0x00, sizeof(SF_CHUNK_INFO) );


    char *chunk = NULL;

    if ( audioEssence->type == AAFI_TYPE_WAVE )
    {
        chunk = "data";
    }
    else if ( audioEssence->type == AAFI_TYPE_AIFC )
    {
        chunk = "SSND";
    }

    snprintf( chunk_info.id, sizeof(chunk_info.id), "SSND" );



    iterator = sf_get_chunk_iterator( file, &chunk_info );

    if ( iterator == NULL )
    {
        if ( audioEssence->is_embedded )
        {
            _warning( "Could not retrieve \"%s\" chunk. Falling back on stream length calculation.\n", chunk );

            /*
             *  NOTE nothing guarentee there are no chunk after audio data, in which
             *  case the length would be wrong. In practice, this was not encountered.
             *  on embedded files.
             */

            audioEssence->length = (audioEssence->length - (1<<cfb_getStreamSectorShift( aafi->aafd->cfbd, audioEssence->node ))) +
                                   (sfinfo.frames * (audioEssence->samplesize/8));
            sf_close( file );
            return 0;
        }
        else
        {
            _error( "Could not retrieve \"%s\" chunk.\n", chunk );
            sf_close( file );
            return -1;
        }
    }



    err = sf_get_chunk_size( iterator, &chunk_info );

    if ( err != SF_ERR_NO_ERROR )
    {
        if ( audioEssence->is_embedded )
        {
            _warning( "Could not retrieve \"%s\" chunk (%s). Falling back on stream length calculation.", chunk, sf_error_number(err) );

            /*
             *  NOTE nothing guarentee there are no chunk after audio data, in which
             *  case the length would be wrong. In practice, this was not encountered.
             *  on embedded files.
             */

            audioEssence->length = (audioEssence->length - (1<<cfb_getStreamSectorShift( aafi->aafd->cfbd, audioEssence->node ))) +
                                   (sfinfo.frames * (audioEssence->samplesize/8));
            sf_close( file );
            return 0;
        }
        else
        {
            _error( "Could not retrieve \"%s\" chunk : %s\n", chunk, sf_error_number (err) );
            sf_close( file );
            return -1;
        }
    }



    audioEssence->length = chunk_info.datalen;


    sf_close( file );

    return 0;
}




int aafi_extract_audio_essence( AAF_Iface *aafi, aafiAudioEssence *audioEssence, const char *outfilepath, const wchar_t *forcedFileName, int format )
{
    SF_VIRTUAL_IO  sfvirtual;
    VIO_DATA       user_data;
    SF_INFO        sfinfo;
    SNDFILE       *infile = NULL;

    if ( audioEssence->is_embedded == 0 )
    {
        _error( "Essence is not embedded : nothing to extract.\n" );
        return -1;
    }

    sfvirtual.get_filelen = vfget_filelen ;
	sfvirtual.seek = vfseek ;
	sfvirtual.read = vfread ;
	sfvirtual.write = vfwrite ;
    sfvirtual.tell = vftell ;

    user_data.length = 0;
    user_data.offset = 0;

    memset( &sfinfo, 0x00, sizeof(SF_INFO) );



    /*
     *  Set parameters manually if raw PCM stream. Otherwise, they will
     *  be set by libsndfile on stream opening.
     */

    if ( audioEssence->summary == NULL )
    {
        sfinfo.format     = samplesize_to_PCM_sf_format( audioEssence->samplesize );
	    sfinfo.channels   = audioEssence->channels;
        sfinfo.samplerate = audioEssence->samplerate;
    }



    /* Retrieve stream from CFB */

    cfb_getStream( aafi->aafd->cfbd, audioEssence->node, (unsigned char**)(&user_data.data), (uint64_t*)(&user_data.length) );

    if ( user_data.data == NULL )
    {
        _error( "Could not get essence stream from CFB.\n" );
        return -1;
    }



    /* Open stream with libsndfile */

    infile = sf_open_virtual( &sfvirtual, SFM_READ, &sfinfo, &user_data );

    if ( infile == 0 )
    {
        _error( "libsndfile could not open stream : %s.\n", sf_strerror( NULL ) );

        free( user_data.data );

		return -1;
    }



    /* Build file path */

    char filePath[1024];

    snprintf( filePath, 1024, "%s/%ls.%s",
        outfilepath,
        ( forcedFileName != NULL ) ? forcedFileName : eascii_to_ascii(audioEssence->unique_file_name),
        sf_format_to_file_ext( format ) );



    /* Set format */

	sfinfo.format = format;

	if ( sf_format_check( &sfinfo ) == 0 )
	{
		_error( "Invalid encoding.\n" );
        sf_close( infile );
		return -1;
	}



    /* Prepare bext chunk if WAVE file */

    SF_BROADCAST_INFO bext;

    memset( &bext, 0, sizeof (bext) );

    if ( format & SF_FORMAT_WAV )
    {
        /* Non-AAF standard, but makes sense */

    	snprintf( bext.description, sizeof(bext .description), "%s %s %s",
            aafi->aafd->Identification.CompanyName,
            aafi->aafd->Identification.ProductName,
            aafi->aafd->Identification.ProductVersionString );
    	snprintf( bext.originator, sizeof(bext.originator), "%s", aafi->aafd->Identification.CompanyName );
    	snprintf( bext.originator_reference, sizeof(bext.originator_reference), "%s", aafi->aafd->Identification.ProductName );


        /* AAF Standard */

        memcpy( bext.origination_date, audioEssence->originationDate, sizeof(bext.origination_date) );
        memcpy( bext.origination_time, audioEssence->originationTime, sizeof(bext.origination_time) );
        memcpy( bext.umid, audioEssence->umid, sizeof(bext.umid) );
        bext.time_reference_high = (uint32_t)( audioEssence->timeReference >> 32 );
        bext.time_reference_low  = (uint32_t)( audioEssence->timeReference & 0xffffffff );
    	bext.coding_history_size = 0;
    }



    /* Prepare output file */

    SNDFILE *outfile = sf_open( filePath, SFM_WRITE, &sfinfo );

	if ( outfile == 0 )
	{
        _error( "Could not open file %s : %s.\n", filePath, sf_strerror( NULL ) );
        sf_close( infile );
		return -1;
	}



    /* Add bext chunk */

    if ( ( format & SF_FORMAT_WAV ) && sf_command( outfile, SFC_SET_BROADCAST_INFO, &bext, sizeof(bext) ) == SF_FALSE )
    {
        _error ( "libsndfile could not add bext chunk.\n" );
    }



    /* Write output file */

    int   readcount = 0;
    float buffer[BUFFER_LEN];

	while ( (readcount = sf_read_float( infile, buffer, BUFFER_LEN )) > 0 )
    {
		sf_write_float (outfile, buffer, readcount);
    }



    audioEssence->exported_file = malloc( (strlen(filePath) + 1) * sizeof(wchar_t) );

    swprintf( audioEssence->exported_file, (strlen(filePath) + 1) * sizeof(wchar_t), L"%s", filePath );



    free( user_data.data );

    sf_close( outfile );
    sf_close( infile );

    return 0;
}




/******************************************************************************
                          libsndfile callbacks
*******************************************************************************/




static sf_count_t vfget_filelen( void *user_data )
{
    VIO_DATA *vf = (VIO_DATA*)user_data;

	return vf->length;
}




static sf_count_t vfseek( sf_count_t offset, int whence, void *user_data )
{
    VIO_DATA *vf = (VIO_DATA*)user_data;

	switch ( whence )
	{
        case SEEK_SET:    vf->offset = offset;                  break;
		case SEEK_CUR:    vf->offset = vf->offset + offset;     break;
		case SEEK_END:    vf->offset = vf->length + offset;     break;

		default:          break;
	}

	return vf->offset;
}




static sf_count_t vfread( void *ptr, sf_count_t count, void *user_data )
{
    VIO_DATA *vf = (VIO_DATA*)user_data;

	/*
	 *	This will brack badly for files over 2Gig in length, but
	 *	is sufficient for testing.
	 */

	if ( vf->offset + count > vf->length )
    {
		count = vf->length - vf->offset;
    }

	memcpy (ptr, vf->data + vf->offset, count);

	vf->offset += count;

	return count;
}




static sf_count_t vfwrite( const void *ptr, sf_count_t count, void *user_data )
{
    VIO_DATA *vf = (VIO_DATA*)user_data;

	/*
	 *	This will break badly for files over 2Gig in length, but
	 *	is sufficient for testing.
	 */

	if ( vf->offset >= vf->length /*SIGNED_SIZEOF (vf->data)*/ )
		return 0;

	if ( vf->offset + count > vf->length/*SIGNED_SIZEOF (vf->data)*/ )
		count = /*sizeof (vf->data)*/ vf->length - vf->offset;

	memcpy( vf->data + vf->offset, ptr, (size_t)count );

	vf->offset += count;

	if ( vf->offset > vf->length )
		 vf->length = vf->offset;

	return count;
}




static sf_count_t vftell( void *user_data )
{
    VIO_DATA *vf = (VIO_DATA*)user_data;

	return vf->offset;
}

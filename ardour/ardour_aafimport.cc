/*
 * Copyright (C) 2019 Robin Gareus <robin@gareus.org>
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
 *
 *
 *
 *
 *
 */

#include <cstdlib>
#include <getopt.h>
#include <iostream>


#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>


#include <glibmm.h>

// #include <libAAF/libAAF.h>
#include "../LibAAF/libAAF.h"

#include "ardour/audioengine.h"
#include "ardour/filename_extensions.h"
#include "ardour/template_utils.h"
#include "ardour/session_directory.h"

#include "common.h"


#include <unistd.h>
/*
#include "pbd/pthread_utils.h"
#include "pbd/basename.h"
#include "pbd/shortpath.h"
#include "pbd/stateful_diff_command.h"*/

// #include "ardour/session.h"

#include "ardour/import_status.h"
#include "ardour/region_factory.h"
#include "ardour/playlist.h"
#include "ardour/source_factory.h"
#include "ardour/audioregion.h"
#include "ardour/audiofilesource.h"
// #include "ardour/smf_source.h"
#include "ardour/audio_track.h"
// #include "ardour/operations.h"
// #include "ardour/utils.h"
// #include "pbd/memento_command.h"


#include "fileops.h"



using namespace std;
using namespace ARDOUR;
using namespace SessionUtils;

using namespace Timecode;

using namespace PBD;




#ifndef UTILNAME
#define UTILNAME "ardour_aafimport"
#endif



#define PRINT_I( fmt, ... ) \
    { fprintf( stderr, "[\033[1;38;5;81mi\x1B[0m] \x1b[38;5;239m%s : %s() on line %i :\x1B[0m ", __FILE__, __func__, __LINE__ ); fprintf( stderr, fmt, ##__VA_ARGS__ ); }
#define PRINT_W( fmt, ... ) \
    { fprintf( stderr, "[\x1B[33mw\x1B[0m] \x1b[38;5;239m%s : %s() on line %i :\x1B[0m ", __FILE__, __func__, __LINE__ ); fprintf( stderr, fmt, ##__VA_ARGS__ ); }
#define PRINT_E( fmt, ... ) \
    { fprintf( stderr, "[\x1B[31me\x1B[0m] \x1b[38;5;239m%s : %s() on line %i :\x1B[0m ", __FILE__, __func__, __LINE__ ); fprintf( stderr, fmt, ##__VA_ARGS__ ); }













static void usage ()
{

	// help2man compatible format (standard GNU help-text)
	printf (UTILNAME " - create a new session from the commandline.\n\n");
	printf ("Usage: " UTILNAME " [ OPTIONS ] <session-dir> [session-name]\n\n");
	printf ("Options:\n\
  -h, --help                        Display this help and exit.\n\
  -L, --list-templates              List available templates and  exit.\n\
  \n\
  -m, --master-channels      <chn>  Master-bus channel num (default 2).\n\
  -r, --sample-rate         <rate>  Sample rate in Hz (default is AAF).\n\
  -b, --bit-depth       <16|24|32>  Audio bit depth   (default is AAF).\n\
  \n\
  -t, --template        <template>  Use given template for new session.\n\
  -p, --session-path        <path>  Location of the new session folder.\n\
  -s, --session-name        <name>  Session name.  A new folder will be\n\
                                    created into session path with that\n\
                                    name : <path>/<name>/\n\
                                    (Default is the   AAF  file  name).\n\
  \n\
  -a, --aaf              <aaffile>  AAF file to load.\n\
\n");

	printf ("\n\
This tool creates a new Ardour session, optionally based on a\n\
session-template.\n\
\n\
If the session-name is unspecified, the sesion-dir-name is used.\n\
If specified, the tool expects a session-name without .ardour\n\
file-name extension.\n\
\n\
If no template is specified, an empty session with a stereo master\n\
bus is created. The -m option allows to specify the master-bus channel\n\
count. If zero is used as channel count, no master-bus is created.\n\
\n\
Note: this tool can only use static session templates.\n\
Interactive Lua init-scripts or dynamic templates are not supported.\n\
\n");

	printf ("\n\
Examples:\n\
" UTILNAME " -s 44100 -m 4 /tmp/NewSession\n\
\n");

	printf ("Report bugs to <http://tracker.ardour.org/>\n"
	        "Website: <http://ardour.org/>\n");
	::exit (EXIT_SUCCESS);
}



static void list_templates()
{
	vector<TemplateInfo> templates;
	find_session_templates( templates, false );

	for ( vector<TemplateInfo>::iterator x = templates.begin (); x != templates.end (); ++x )
	{
		printf( "%s\n", (*x).name.c_str() );
	}
}



static std::string template_path_from_name( std::string const& name )
{
	vector<TemplateInfo> templates;
	find_session_templates( templates, false );

	for ( vector<TemplateInfo>::iterator x = templates.begin (); x != templates.end (); ++x )
	{
		if ( (*x).name == name )
			return (*x).path;
	}

	return "";
}



static Session* create_new_session( string const &dir, string const &state, float samplerate, ARDOUR::SampleFormat bitdepth, int master_bus_chn, string const &template_path )
{
	AudioEngine* engine = AudioEngine::create();

	if ( !engine->set_backend( "None (Dummy)", "Unit-Test", "" ) )
	{
		PRINT_E( "Cannot create Audio/MIDI engine.\n" );
		return NULL;
	}

	// engine->set_input_channels( 32 );
	// engine->set_output_channels( 32 );

	if ( engine->set_sample_rate( samplerate ) )
	{
		PRINT_E( "Cannot set session's samplerate to %lf.\n", samplerate );
		return NULL;
	}

	// if ( engine->start() != 0 )
	// {
	// 	PRINT_E( "Cannot start Audio/MIDI engine.\n" );
	// 	return NULL;
	// }

	string s = Glib::build_filename( dir, state + statefile_suffix );

	if ( Glib::file_test( dir, Glib::FILE_TEST_EXISTS ) )
	{
		PRINT_E( "Session folder already exists '%s'\n", dir.c_str() );
		return NULL;
	}

	if ( Glib::file_test( s, Glib::FILE_TEST_EXISTS ) )
	{
		PRINT_E( "Session file exists '%s'\n", s.c_str() );
		return NULL;
	}

	BusProfile  bus_profile;
	BusProfile *bus_profile_ptr = NULL;

	if ( master_bus_chn > 0 )
	{
		bus_profile_ptr = &bus_profile;
		bus_profile.master_out_channels = master_bus_chn;
	}

	if ( !template_path.empty() )
	{
		bus_profile_ptr = NULL;
	}

	Session* session = new Session( *engine, dir, state, bus_profile_ptr, template_path );

	engine->set_session( session );

	session->config.set_native_file_data_format(bitdepth);


	return session;
}






#include <pbd/gstdio_compat.h> // g_unlink()

#include <ardour/sndfileimportable.h> // SndFileImportableSource Class
#include <ardour/smf_source.h> // Evoral::SMF stuff
#include <ardour/resampled_source.h> // ResampledImportableSource Class
#include <ardour/analyser.h> //  Analyser::queue_source_for_analysis()

#include <ardour/sndfilesource.h>


/*
 * libs/ardour/import.cc
 * - Not Modified -
 */

static boost::shared_ptr<ImportableSource>
open_importable_source (const string& path, samplecnt_t samplerate, ARDOUR::SrcQuality quality)
{
	/* try libsndfile first, because it can get BWF info from .wav, which ExtAudioFile cannot.
	   We don't necessarily need that information in an ImportableSource, but it keeps the
	   logic the same as in SourceFactory::create()
	*/

	try {
		boost::shared_ptr<SndFileImportableSource> source(new SndFileImportableSource(path));

		if (source->samplerate() == samplerate) {
			return source;
		}

		/* rewrap as a resampled source */

		return boost::shared_ptr<ImportableSource>(new ResampledImportableSource(source, samplerate, quality));
	}

	catch (...) {

#ifdef HAVE_COREAUDIO

		/* libsndfile failed, see if we can use CoreAudio to handle the IO */

		CAImportableSource* src = new CAImportableSource(path);
		boost::shared_ptr<CAImportableSource> source (src);

		if (source->samplerate() == samplerate) {
			return source;
		}

		/* rewrap as a resampled source */

		return boost::shared_ptr<ImportableSource>(new ResampledImportableSource(source, samplerate, quality));

#else
		throw; // rethrow
#endif

	}
}




/*
 * libs/ardour/import.cc
 * - Not Modified -
 */

static void
remove_file_source (boost::shared_ptr<Source> source)
{
	boost::shared_ptr<FileSource> fs = boost::dynamic_pointer_cast<FileSource> (source);

	fs->DropReferences ();

	if (fs) {
		::g_unlink (fs->path().c_str());
	}
}




/*
 * libs/ardour/import.cc
 * - Modified -
 */

static bool
create_mono_sources_for_writing (const vector<string>& new_paths,
                                 Session& sess, uint32_t samplerate,
                                 vector<boost::shared_ptr<Source> >& newfiles,
                                 samplepos_t natural_position)
{

	for ( vector<string>::const_iterator i = new_paths.begin(); i != new_paths.end(); ++i )
  {

		boost::shared_ptr<Source> source;

		try {

      /* NOTE: Modification here.
       * createWritable() creates a new audio file.
       * createExternal() refers to an existing audio file.
       */

			// const DataType type = SMFSource::safe_midi_file_extension (*i) ? DataType::MIDI : DataType::AUDIO;
			// source = SourceFactory::createWritable (type, sess,
			//                                         i->c_str(),
			//                                         false, // destructive
			//                                         samplerate);

      source = SourceFactory::createExternal( DataType::AUDIO, sess, i->c_str(), 0/*chn*/, Source::Flag() );
		}
		catch ( const failed_constructor& err ) {
			error << string_compose( "Unable to create file %1 during import", *i ) << endmsg;
			return false;
		}


		newfiles.push_back( boost::dynamic_pointer_cast<Source>(source) );

		/* for audio files, reset the timeline position so that any BWF-ish
		   information in the original files we are importing from is maintained.
		*/

		boost::shared_ptr<AudioFileSource> afs;
		if ( (afs = boost::dynamic_pointer_cast<AudioFileSource>(source)) != 0 ) {
			afs->set_natural_position( natural_position );
		}
	}

	return true;
}




/*
 * libs/ardour/import.cc
 * - Modified -
 */

void
Session::import_files (ImportStatus& status)
{
	typedef vector<boost::shared_ptr<Source> > Sources;
	Sources all_new_sources;
	boost::shared_ptr<AudioFileSource> afs;
	boost::shared_ptr<SMFSource> smfs;
	uint32_t channels = 0;
	vector<string> smf_names;

	status.sources.clear ();

	for (vector<string>::const_iterator p = status.paths.begin();
	     p != status.paths.end() && !status.cancel;
	     ++p)
	{
		// boost::shared_ptr<ImportableSource> source;
    //
		// const DataType type = SMFSource::safe_midi_file_extension (*p) ? DataType::MIDI : DataType::AUDIO;
		// boost::scoped_ptr<Evoral::SMF> smf_reader;
    //
		// if (type == DataType::AUDIO) {
		// 	try {
		// 		source = open_importable_source (*p, sample_rate(), status.quality);
		// 		channels = source->channels();
		// 	} catch (const failed_constructor& err) {
		// 		error << string_compose( "Import: cannot open input sound file \"%1\"", (*p)) << endmsg;
		// 		status.done = status.cancel = true;
		// 		return;
		// 	}
    //
		// }
    // else {
		// 	try {
		// 		smf_reader.reset (new Evoral::SMF());
    //
		// 		if (smf_reader->open(*p)) {
		// 			throw Evoral::SMF::FileError (*p);
		// 		}
    //
		// 		if (smf_reader->is_type0 () && status.split_midi_channels) {
		// 			channels = smf_reader->channels().size();
		// 		} else {
		// 			channels = smf_reader->num_tracks();
		// 			switch (status.midi_track_name_source) {
		// 			case SMFTrackNumber:
		// 				break;
		// 			case SMFTrackName:
		// 				smf_reader->track_names (smf_names);
		// 				break;
		// 			case SMFInstrumentName:
		// 				smf_reader->instrument_names (smf_names);
		// 				break;
		// 			}
		// 		}
		// 	} catch (...) {
		// 		error << "Import: error opening MIDI file" << endmsg;
		// 		status.done = status.cancel = true;
		// 		return;
		// 	}
		// }
    //
		// if (channels == 0) {
		// 	error << "Import: file contains no channels." << endmsg;
		// 	continue;
		// }

    /* NOTE: Modification here.
     * Keep existing path, as we are importing already existing files.
     */
		vector<string> new_paths; // = get_paths_for_new_sources (status.replace_existing_source, *p, channels, smf_names);
    new_paths.push_back(*p);


		Sources newfiles;
		samplepos_t natural_position = 0; //source ? source->natural_position() : 0;


		if (status.replace_existing_source) {
			fatal << "THIS IS NOT IMPLEMENTED YET, IT SHOULD NEVER GET CALLED!!! DYING!" << endmsg;

      /* NOTE: Modification here.
       * Avoid re-implement of unused static function. (libs/ardour/import.cc)
       */

		  // status.cancel = !map_existing_mono_sources (new_paths, *this, sample_rate(), newfiles, this);
		} else {
			status.cancel = !create_mono_sources_for_writing (new_paths, *this, sample_rate(), newfiles, natural_position);
		}


		// copy on cancel/failure so that any files that were created will be removed below
		std::copy (newfiles.begin(), newfiles.end(), std::back_inserter(all_new_sources));

		if (status.cancel) {
			break;
		}

		for (Sources::iterator i = newfiles.begin(); i != newfiles.end(); ++i) {
			if ((afs = boost::dynamic_pointer_cast<AudioFileSource>(*i)) != 0) {
				afs->prepare_for_peakfile_writes ();
			}
		}


    /* NOTE: Modification here.
     * Do not write output file, as we are importing already existing files.
     */

		// if (source) { // audio
		// 	status.doing_what = compose_status_message (*p, source->samplerate(),
		// 	                                            sample_rate(), status.current, status.total);
		// 	write_audio_data_to_new_files (source.get(), status, newfiles);
		// }
    // else if (smf_reader) { // midi
		// 	status.doing_what = string_compose( "Loading MIDI file %1", *p);
		// 	write_midi_data_to_new_files (smf_reader.get(), status, newfiles, status.split_midi_channels);
		// }

		++status.current;
		status.progress = 0;

    // for (Sources::iterator i = newfiles.begin(); i != newfiles.end(); ++i) {
    //   // SndFileSource *sndfile = boost::dynamic_pointer_cast<SndFileSource>(*i);
    //   delete &(*i);// &(*i);
    //   // SndFileSource
    // }

	}



	if (!status.cancel) {
		struct tm* now;
		time_t xnow;
		time (&xnow);
		now = localtime (&xnow);
		status.freeze = true;

		/* flush the final length(s) to the header(s) */

		for (Sources::iterator x = all_new_sources.begin(); x != all_new_sources.end(); ) {

			if ((afs = boost::dynamic_pointer_cast<AudioFileSource>(*x)) != 0) {
				// afs->update_header((*x)->natural_position(), *now, xnow);
				afs->done_with_peakfile_writes ();

				/* now that there is data there, requeue the file for analysis */

				if (Config->get_auto_analyse_audio()) {
					Analyser::queue_source_for_analysis (boost::static_pointer_cast<Source>(*x), false);
				}
			}

			/* imported, copied files cannot be written or removed
			 */

			boost::shared_ptr<FileSource> fs = boost::dynamic_pointer_cast<FileSource>(*x);
			if (fs) {
				/* Only audio files should be marked as
				   immutable - we may need to rewrite MIDI
				   files at any time.
				*/
				if (boost::dynamic_pointer_cast<AudioFileSource> (fs)) {
					fs->mark_immutable ();
				} else {
					fs->mark_immutable_except_write ();
				}
				fs->mark_nonremovable ();
			}

			/* don't create tracks for empty MIDI sources (channels) */

			if ((smfs = boost::dynamic_pointer_cast<SMFSource>(*x)) != 0 && smfs->is_empty()) {
				x = all_new_sources.erase(x);
			} else {
				++x;
			}
		}

		/* save state so that we don't lose these new Sources */


    /* NOTE: Modification here.
     * Do not re-write session file on each imported file, makes the all process quickest.
     */
		// save_state (_name);

		std::copy (all_new_sources.begin(), all_new_sources.end(), std::back_inserter(status.sources));

	} else {
		try {
			std::for_each (all_new_sources.begin(), all_new_sources.end(), remove_file_source);
		} catch (...) {
			error << "Failed to remove some files after failed/cancelled import operation" << endmsg;
		}
	}


	status.done = true;
}




/*
bool Session::*/
bool import_sndfile_as_region( Session *s, struct aafiAudioEssence *audioEssence, SrcQuality quality, samplepos_t &pos, SourceList &sources, ImportStatus &status, vector<boost::shared_ptr<Region> > *regions /* boost::shared_ptr<Region> r*/ )
{
  /* https://github.com/Ardour/ardour/blob/365f6d633731229e7bc5d37a5fe2c9107b527e28/libs/ardour/import_pt.cc#L82 */

  wstring ws(audioEssence->exported_file_path);
  string exported_file_path(ws.begin(), ws.end());


	/* Import the source */
	status.paths.clear();
	status.paths.push_back( exported_file_path /*path*/);
	status.current = 1;
	status.total = 1;
	status.freeze = false;
	status.quality = quality;
	status.replace_existing_source = false;
	status.split_midi_channels = false;
	status.done = false;
	status.cancel = false;

	s->import_files( status );


	sources.clear();

	/* FIXME: There is no way to tell if cancel button was pressed
	 * or if the file failed to import, just that one of these occurred.
	 * We want status.cancel to reflect the user's choice only
	 */
	if ( status.cancel && status.current > 1 )
	{
		/* Succeeded to import file, assume user hit cancel */
		return false;
	}
	else if ( status.cancel && status.current == 1 )
	{
		/* Failed to import file, assume user did not hit cancel */
		status.cancel = false;
		return false;
	}


	sources.push_back( status.sources.front() );

  /* save ardour sourceID for later */
  snprintf( audioEssence->ardour_src_id, 32, "%s", sources.back()->id().to_s().c_str() );


	/* build peakfiles */
  for ( SourceList::iterator x = sources.begin(); x != sources.end(); ++x )
	{
		SourceFactory::setup_peakfile( *x, true );
	}



  // return true;



  /* NOTE The following is required for the source to appear in source list */

	/* Put the source on a region */
	// vector<boost::shared_ptr<Region> > regions;
  boost::shared_ptr<Region> region;
	string region_name;


	// /* take all the sources we have and package them up as a region */
	//
	// region_name = region_name_from_path (status.paths.front(), (sources.size() > 1), false);
	//
	// /* we checked in import_sndfiles() that there were not too many */
	//
	// while (RegionFactory::region_by_name (region_name)) {
	// 	region_name = bump_name_once (region_name, '.');
	// }

  ws = audioEssence->unique_file_name;
  string unique_file_name( ws.begin(), ws.end() );

	PropertyList proplist;

	proplist.add (ARDOUR::Properties::start,  0 /*eu2sample_fromclip( clip, clip->essence_offset )*/);
	proplist.add (ARDOUR::Properties::length, /*audioEssence->*/  /*eu2sample_fromclip( clip, clip->len )*/ sources[0]->length (pos));
  // plist.add (ARDOUR::Properties::position, eu2sample_fromclip( audioClip, (audioClip->pos  + audioClip->track->Audio->tc->start) ) );
	proplist.add (ARDOUR::Properties::name, unique_file_name/*region_name*/);
	proplist.add (ARDOUR::Properties::layer, 0);
	proplist.add (ARDOUR::Properties::whole_file, true);
	proplist.add (ARDOUR::Properties::external, false);
  // proplist.add (ARDOUR::Properties::sync_position, 192000);


	region = RegionFactory::create( sources, proplist );
  (*regions).push_back( region );



  /* NOTE Don't know what that's for */

	// bool use_timestamp;
	// use_timestamp = (pos == -1);
	// if (use_timestamp && boost::dynamic_pointer_cast<AudioRegion>(r)) {
	// 	boost::dynamic_pointer_cast<AudioRegion>(r)->special_set_position(sources[0]->natural_position());
	// }
	//
	//
	// /* if we're creating a new track, name it after the cleaned-up
	//  * and "merged" region name.
	//  */
	//
	// regions.push_back (r);
	// int n = 0;
	//
	// for (vector<boost::shared_ptr<Region> >::iterator r = regions.begin(); r != regions.end(); ++r, ++n) {
	// 	boost::shared_ptr<AudioRegion> ar = boost::dynamic_pointer_cast<AudioRegion> (*r);
	//
	// 	if (use_timestamp) {
	// 		if (ar) {
	//
	// 			/* get timestamp for this region */
	//
	// 			const boost::shared_ptr<Source> s (ar->sources().front());
	// 			const boost::shared_ptr<AudioSource> as = boost::dynamic_pointer_cast<AudioSource> (s);
	//
	// 			assert (as);
	//
	// 			if (as->natural_position() != 0) {
	// 				pos = as->natural_position();
	// 			} else {
	// 				pos = 0;
	// 			}
	// 		} else {
	// 			/* should really get first position in MIDI file, but for now, use 0 */
	// 			pos = 0;
	// 		}
	// 	}
	// }


	return true;
}





/* Overloaded function to add start/end markers locking.
 * libs/ardour/session.cc#L1759
 */
void Session::set_session_extents( samplepos_t start, samplepos_t end )
{
	Location* existing;
	if ( (existing = _locations->session_range_location()) == 0 )
	{
		// if there is no existing session, we need to make a new session location  (should never happen)
		existing = new Location( *this, 0, 0, ("session"), Location::IsSessionRange, 0 );
	}

	if ( end <= start )
	{
		error << ("Session: you can't use that location for session start/end)") << endmsg;
		return;
	}

	existing->set( start, end );

  /* NOTE: Modification here.
   * add lock of start and end markers
   */
  existing->lock();

	set_dirty();
}


static void set_session_range_from_aaf( Session *s, AAF_Iface *aafi )
{
  samplepos_t start = eu2sample( s->sample_rate(), aafi->Audio->tc->edit_rate, aafi->Audio->tc->start );
  samplepos_t end   = eu2sample( s->sample_rate(), aafi->Audio->tc->edit_rate, aafi->Audio->tc->end ) + start;

  s->set_session_extents( start, end );
}








static const char * get_surface_a2j_midi_port( const char *mode )
{

	    FILE *fp;
	    static char midiPort[1024];

			char cmd[1024];

			snprintf( cmd, sizeof(cmd), "jack_lsp | grep \"a2j:iCON QCON Pro\" | grep %s", mode );

	    if ( (fp = popen( cmd, "r" )) == NULL )
	    {
	        /* TODO notify client ? */
	        PRINT_E( "Failed to run '%s'\n", cmd );
	        pclose(fp);
	        return NULL;
	    }

			/* "a2j:iCON QCON Pro V1 04 [28] (capture): iCON QCON Pro V1 04 MIDI 1" */
			/* "a2j:iCON QCON Pro V1 04 [28] (playback): iCON QCON Pro V1 04 MIDI 1" */

	    fgets( midiPort, sizeof(midiPort)-1, fp );

	    midiPort[strlen(midiPort)-1] = 0x00; // removes \n

	    pclose(fp);


	    return midiPort;
}



static int configure_midi_control_surface( const char *session_file )
{

    xmlDocPtr pDoc = xmlReadFile( session_file, NULL, XML_PARSE_NOBLANKS | XML_PARSE_NOERROR | XML_PARSE_NOWARNING | XML_PARSE_NONET );
    if ( pDoc == NULL ) {
        PRINT_E( "XML: Could not read '%s'\n", session_file );
        return -1;
    }

    xmlNodePtr root_element = xmlDocGetRootElement( pDoc );
    if ( root_element == NULL ) {
        PRINT_E( "XML: Document is empty : '%s'\n", session_file );
        xmlFreeDoc( pDoc );
        return -1;
    }


    /*
      <Session>
        <ControlProtocols>
            <Protocol name="Mackie" ...>
                <Configurations>
                    <Configuration name="iCON QCon Pro">
                        <Surfaces>
                            <Surface name="iCON QCon Pro">
                                <Port>
                                    <Input>
                                        <Port name="mackie control in" direction="input">
                                            <Connection other="a2j:iCON QCON Pro V1 04 [28] (capture): iCON QCON Pro V1 04 MIDI 1"/>
                                        </Port>
                                    </Input>
                                    <Output>
                                        <Port name="mackie control out" direction="output">
                                            <Connection other="a2j:iCON QCON Pro V1 04 [28] (capture): iCON QCON Pro V1 04 MIDI 1"/>
                                        </Port>
                                    </Output>
    */
    const xmlChar *xpath = (const xmlChar*)"/Session/ControlProtocols/Protocol[@name='Mackie']/Configurations/Configuration[@name='iCON QCon Pro']/Surfaces/Surface/Port";

    xmlXPathContextPtr context = xmlXPathNewContext( pDoc );
    if ( context == NULL ) {
        PRINT_E( "XML: Could not create XPath context.\n" );
        return -1;
    }


    xmlXPathObjectPtr result = xmlXPathEvalExpression( xpath, context );

    xmlXPathFreeContext( context );

    if ( result == NULL )
    {
        PRINT_E( "XML: Could not evaluate XPath '%s'\n", xpath );
        return -1;
    }
    if ( xmlXPathNodeSetIsEmpty( result->nodesetval ) )
    {
        xmlXPathFreeObject( result );
        PRINT_E( "XML: Node not found using XPath '%s'\n", xpath );
        return -1;
    };


    xmlNodeSetPtr nodeset = result->nodesetval;

    if ( nodeset->nodeNr > 1 )
    {
        PRINT_E( "XML: Multiple nodes found using XPath, there should be only one.\n" );
        return -1;
    }



    xmlNodePtr parentPortNode = nodeset->nodeTab[0]; // <Port>

    for ( xmlNodePtr currentNode = parentPortNode->children; currentNode != NULL; currentNode = currentNode->next )
    {

				const xmlChar *connection = NULL;

        if ( strcmp( (const char*)currentNode->name, "Input" )  == 0 )
				{
						// connection = (const xmlChar*)"a2j:iCON QCON Pro V1 04 [28] (capture): iCON QCON Pro V1 04 MIDI 1";
						connection = (const xmlChar*)get_surface_a2j_midi_port( "capture" );
				}
        else if ( strcmp( (const char*)currentNode->name, "Output" ) == 0 )
        {
            // connection = (const xmlChar*)"a2j:iCON QCON Pro V1 04 [28] (playback): iCON QCON Pro V1 04 MIDI 1";
						connection = (const xmlChar*)get_surface_a2j_midi_port( "playback" );
				}
				else
				{
            continue;
        }

        xmlNodePtr portNode = xmlFirstElementChild( currentNode );

        if ( !portNode || strcmp( (const char*)portNode->name, "Port" ) != 0 )
        {
            PRINT_E( "Missing Port node.\n" ); // Surface/Port/Input/Port
            return -1;
        }

        xmlNodePtr pNode = xmlNewNode( 0, (const xmlChar*)"Connection" );
        xmlSetProp( pNode, (const xmlChar*)"other", connection );
        xmlAddChild( portNode, pNode );

        PRINT_I( "Configuring Midi Surface <%s> : %s.\n", (const char*)currentNode->name, connection );
    }


    xmlXPathFreeObject( result );

    /* TODO write to tmp file first */
    xmlSaveFormatFileEnc( session_file, pDoc, "utf-8", 1 );


    xmlFreeDoc( pDoc );
    xmlCleanupParser();

    return 0;
}




static boost::shared_ptr<Region> build_region_from_aaf_audioclip( aafiAudioClip *audioClip, boost::shared_ptr<ARDOUR::Source> source )
{
  wstring ws = audioClip->Essence->unique_file_name;
  string unique_file_name(ws.begin(), ws.end());

  PropertyList proplist;

  proplist.add( ARDOUR::Properties::start, eu2sample_fromclip( audioClip, audioClip->essence_offset ) );
  proplist.add( ARDOUR::Properties::length, eu2sample_fromclip( audioClip, audioClip->len ) );
  proplist.add( ARDOUR::Properties::name, unique_file_name );
  proplist.add( ARDOUR::Properties::layer, 0 );
  proplist.add( ARDOUR::Properties::whole_file, false );
  proplist.add( ARDOUR::Properties::external, true );
  /* NOTE position is set when calling add_region() */
  // proplist.add (ARDOUR::Properties::position, 172800000/*eu2sample_fromclip( audioClip, (audioClip->pos  + audioClip->track->Audio->tc->start) )*/ );


  /* update source natural position
   * NOTE: only because it matches native ardour session, dont know what it's used for.
   */
  source->set_natural_position( eu2sample_fromclip( audioClip, (audioClip->pos  + audioClip->track->Audio->tc->start) ) );


  return RegionFactory::create( source, proplist );
}


static void set_region_gain_from_aaf_audioclip( aafiAudioClip *audioClip, boost::shared_ptr<Region> region )
{
  if ( audioClip->gain && audioClip->gain->flags & AAFI_AUDIO_GAIN_CONSTANT )
  {
    // PRINT_W("Setting gain\n" );
    // boost::dynamic_pointer_cast<AudioRegion>(region)->set_scale_amplitude( rationalToFloat(audioClip->gain->value[0].numerator / audioClip->gain->value[0].denominator) );
    boost::dynamic_pointer_cast<AudioRegion>(region)->set_scale_amplitude( rationalToFloat(audioClip->gain->value[0]) );
  }
}


static void put_region_on_track_from_aaf_audioclip( aafiAudioClip *audioClip, boost::shared_ptr<Region> region, Session *s )
{
  Glib::Threads::Mutex::Lock lx( AudioEngine::instance()->process_lock (), Glib::Threads::NOT_LOCK );

  lx.acquire();

  /* Add region to track
   * ===================
   * https://github.com/Ardour/ardour/blob/365f6d633731229e7bc5d37a5fe2c9107b527e28/libs/ardour/import_pt.cc#L327
   */

  boost::shared_ptr<AudioTrack> track;

  /* Use existing track if possible */
  track = s->get_nth_audio_track( audioClip->track->number );

  /* Or create a new track if needed */
  if ( !track ) {

      PRINT_W( "Track number %i does not exist. Adding new track.\n", audioClip->track->number );

      lx.release();
      list<boost::shared_ptr<AudioTrack> > at( s->new_audio_track( 1, 2, 0, 1, "", PresentationInfo::max_order, Normal ) );
      if ( at.empty() ) {
        PRINT_E( "Could not create new audio track.\n" );
        ::exit( EXIT_FAILURE );
      }
      lx.acquire();

      track = at.back();
  }

  /* Put region on track */
  boost::shared_ptr<Playlist> playlist = track->playlist();
  playlist->add_region( region, eu2sample_fromclip( audioClip, (audioClip->pos + audioClip->track->Audio->tc->start) ) );

  lx.release();
}

static void set_region_fade_from_aaf_audioclip( aafiAudioClip *audioClip, boost::shared_ptr<Region> region )
{
  /* Set fades if any
   * ================
   *
   * https://github.com/Ardour/ardour/blob/b84c99639f0dd28e210ed9c064429c17014093a7/libs/ardour/ardour/types.h#L705
   *
   * enum FadeShape {
   *   FadeLinear,
   *   FadeFast,
   *   FadeSlow,
   *   FadeConstantPower,
   *   FadeSymmetric,
   * };
   *
   * https://github.com/Ardour/ardour/blob/365f6d633731229e7bc5d37a5fe2c9107b527e28/libs/ardour/ardour/audioregion.h#L143
   * https://github.com/Ardour/ardour/blob/365f6d633731229e7bc5d37a5fe2c9107b527e28/libs/temporal/temporal/types.h#L39
   */

  aafiTransition *fadein  = get_fadein( audioClip->Item );
  aafiTransition *fadeout = get_fadeout( audioClip->Item );

  if ( fadein == NULL )
  {
      fadein = get_xfade( audioClip->Item );
  }

  FadeShape fade_shape;
  samplecnt_t fade_len;

  if ( fadein != NULL )
  {
    fade_shape = (fadein->flags & AAFI_INTERPOL_NONE)     ? FadeLinear :
                 (fadein->flags & AAFI_INTERPOL_LINEAR)   ? FadeLinear :
                 (fadein->flags & AAFI_INTERPOL_LOG)      ? FadeSymmetric :
                 (fadein->flags & AAFI_INTERPOL_CONSTANT) ? FadeConstantPower :
                 (fadein->flags & AAFI_INTERPOL_POWER)    ? FadeConstantPower :
                 (fadein->flags & AAFI_INTERPOL_BSPLINE)  ? FadeLinear :
                 FadeLinear;
    fade_len = eu2sample_fromclip( audioClip, fadein->len );

    boost::dynamic_pointer_cast<AudioRegion>(region)->set_fade_in( fade_shape, fade_len );
  }

  if ( fadeout != NULL )
  {
    fade_shape = (fadeout->flags & AAFI_INTERPOL_NONE)     ? FadeLinear :
                 (fadeout->flags & AAFI_INTERPOL_LINEAR)   ? FadeLinear :
                 (fadeout->flags & AAFI_INTERPOL_LOG)      ? FadeSymmetric :
                 (fadeout->flags & AAFI_INTERPOL_CONSTANT) ? FadeConstantPower :
                 (fadeout->flags & AAFI_INTERPOL_POWER)    ? FadeConstantPower :
                 (fadeout->flags & AAFI_INTERPOL_BSPLINE)  ? FadeLinear :
                 FadeLinear;
    fade_len = eu2sample_fromclip( audioClip, fadeout->len );

    boost::dynamic_pointer_cast<AudioRegion>(region)->set_fade_out( fade_shape, fade_len );
  }
}



static void set_session_video_from_aaf( Session *s, AAF_Iface *aafi )
{
  if ( aafi->Video->Tracks && aafi->Video->Tracks->Items )
	{
		aafiVideoClip *videoClip = (aafiVideoClip*)&aafi->Video->Tracks->Items->data;

		// printf( "\n\n\nGot video Track and Item : %ls\n\n\n", videoClip->Essence->original_file/*->Essence->original_file*/ );
    char origf[PATH_MAX+1];
    snprintf(origf, PATH_MAX, "%ls", videoClip->Essence->original_file );
    printf("Looking for : %s\n", strrchr(origf, '/') + 1 );

		char *file = locate_external_essence_file( aafi, videoClip->Essence->original_file );

		if ( file != NULL )
		{
      PRINT_I( "Importing video : %s\n", fop_get_filename(file) );

			/* get absolute video file path */
			char absFile[PATH_MAX];
			realpath(file, absFile);

			// /* get original mxf video filename */
			// char *file_name = remove_file_ext( basename(file), '.', '/' );
			//
			// /* creates project video folder */
			// mkdir( s->session_directory().video_path().c_str(), 0755 );
			//
			// /* extract mpeg video from original mxf */
			// char cmdstr[PATH_MAX*6];
			// snprintf( cmdstr, sizeof(cmdstr), "ffmpeg -y -i \"%s\" -c copy -f mpeg2video \"%s/%s.mpg\"", absFile, s->session_directory().video_path().c_str(), file_name );
			// //snprintf( cmdstr, sizeof(cmdstr), "ffmpeg -y -i \"%s\" -c copy -map_metadata 0 \"%s/%s.mkv\"", absFile, s->session_directory().video_path().c_str(), file_name );
			//
			// system(cmdstr);


			/* Add video to Ardour
			 * ===================
			 * https://github.com/Ardour/ardour/blob/6987196ea18cbf171e22ed62760962576ccb54da/gtk2_ardour/ardour_ui_video.cc#L317
			 *
			 *	<Videotimeline Filename="/home/agfline/Developpement/ardio/watchfolder/3572607_RUGBY_F2_S65CFA3D0V.mxf" AutoFPS="1" LocalFile="1" OriginalVideoFile="/home/agfline/Developpement/ardio/watchfolder/3572607_RUGBY_F2_S65CFA3D0V.mxf"/>
            <RulerVisibility timecode="1" bbt="1" samples="0" minsec="0" tempo="1" meter="1" marker="1" rangemarker="1" transportmarker="1" cdmarker="1" videotl="1"/>
			 */

			XMLNode* videoTLnode = new XMLNode( "Videotimeline" );
			videoTLnode->set_property( "Filename", string(absFile)/*string(file_name) + ".mpg"*/ );
			videoTLnode->set_property( "AutoFPS", true );
			videoTLnode->set_property( "LocalFile", true );
			videoTLnode->set_property( "OriginalVideoFile", string(absFile) );
			videoTLnode->set_property( "id", 51 );
			videoTLnode->set_property( "Height", 3 );
			videoTLnode->set_property( "VideoOffsetLock", true );
			videoTLnode->set_property( "VideoOffset", eu2sample( s->sample_rate(), videoClip->track->Video->tc->edit_rate, (videoClip->pos + videoClip->track->Video->tc->start)) );

      // printf("\n\n\n%li  |  %li\n\n\n", videoClip->pos, videoClip->track->Video->tc->start );

			XMLNode* videoMONnode = new XMLNode( "Videomonitor" );
			videoMONnode->set_property( "active", true );



			XMLNode* xjnode = new XMLNode( "XJSettings" );

      XMLNode* xjsetting;
      xjsetting = xjnode->add_child( "XJSetting" );
      xjsetting->set_property( "k", "set offset" );
      xjsetting->set_property( "v", "-90000" ); //videoClip->pos * videoClip->track->Video->tc->edit_rate );

      xjsetting = xjnode->add_child( "XJSetting" );
      xjsetting->set_property( "k", "osd smpte" );
      xjsetting->set_property( "v", "95" );

      /* video_monitor.cc
      <XJSettings>
        <XJSetting k="window fullscreen" v="on"/>
        <XJSetting k="set offset" v="-90000"/>
        <XJSetting k="osd smpte" v="95"/>
      </XJSettings>
      */

			s->add_extra_xml(*xjnode);
			s->add_extra_xml(*videoTLnode);
			s->add_extra_xml(*videoMONnode);

			// s->set_dirty();
		}
    else
    {
      PRINT_E( "Could not locate video file : %ls\n", videoClip->Essence->original_file );
    }
	}
  else
  {
    PRINT_E( "Could not retrieve video from AAF.\n" );
  }
}



static void set_session_fps_from_aaf( Session *s, AAF_Iface *aafi )
{
  uint16_t aaftc2 = aafi->Audio->tc->fps;
  aafRational_t *aaftc1 = ( aafi->Video ) ? ( aafi->Video->Essences ) ? aafi->Video->Essences->framerate : NULL : NULL;
  TimecodeFormat ardourtc;

  /*
   *  The following is based on adobe premiere pro's AAF.
   *  Fractional timecodes are never explicitly set into tc->fps, so we deduce
   *  them based on edit_rate value.
   *
   *  Available timecodes in ardour (libs/ardour/enums.cc) :
   *
   *    	REGISTER_ENUM (timecode_23976);
   *     	REGISTER_ENUM (timecode_24);
   *    	REGISTER_ENUM (timecode_24976);
   *    	REGISTER_ENUM (timecode_25);
   *    	REGISTER_ENUM (timecode_2997);
   *    	REGISTER_ENUM (timecode_2997drop);
   *    	REGISTER_ENUM (timecode_30);
   *    	REGISTER_ENUM (timecode_30drop);
   *    	REGISTER_ENUM (timecode_5994);
   *   	  REGISTER_ENUM (timecode_60);
   */
  if ( aaftc1 )
  {
    if ( aaftc1->numerator == 24000 && aaftc1->denominator == 1001 )
    {
      ardourtc = timecode_23976;
    }
    else if ( aaftc1->numerator == 24 && aaftc1->denominator == 1 )
    {
      ardourtc = timecode_24;
    }
    else if ( aaftc1->numerator == 25 && aaftc1->denominator == 1 )
    {
      ardourtc = timecode_25;
    }
    else if ( aaftc1->numerator == 29000 && aaftc1->denominator == 1001 )
    {
      ardourtc = timecode_2997;
    }
    else if ( aaftc1->numerator == 30 && aaftc1->denominator == 1 )
    {
      ardourtc = timecode_30;
    }
    else if ( aaftc1->numerator == 59000 && aaftc1->denominator == 1001 )
    {
      ardourtc = timecode_5994;
    }
    else if ( aaftc1->numerator == 60 && aaftc1->denominator == 1 )
    {
      ardourtc = timecode_60;
    }
    else
    {
      PRINT_E( "Unknown AAF timecode fps : %i/%i.\n", aaftc1->numerator, aaftc1->denominator );
      return;
    }
  }
  else
  {
    switch ( aaftc2 )
    {
      case 24:
        if ( aafi->Audio->tc->edit_rate->numerator   == 24000 &&
             aafi->Audio->tc->edit_rate->denominator == 1001 )
        {
          ardourtc = timecode_23976;
        }
        else
        {
          ardourtc = timecode_24;
        }
        break;

      case 25:
        ardourtc = timecode_25;
        break;

      case 30:
        if ( aafi->Audio->tc->edit_rate->numerator   == 30000 &&
             aafi->Audio->tc->edit_rate->denominator == 1001 )
        {
          if ( aafi->Audio->tc->drop )
          {
            ardourtc = timecode_2997drop;
          }
          else
          {
            ardourtc = timecode_2997;
          }
        }
        else
        {
          if ( aafi->Audio->tc->drop )
          {
            ardourtc = timecode_30drop;
          }
          else
          {
            ardourtc = timecode_30;
          }
        }
        break;

      case 60:
        if ( aafi->Audio->tc->edit_rate->numerator   == 60000 &&
             aafi->Audio->tc->edit_rate->denominator == 1001 )
        {
          ardourtc = timecode_5994;
        }
        else
        {
          ardourtc = timecode_60;
        }
        break;


      default:
        PRINT_E( "Unknown AAF timecode fps : %i.\n", aaftc2 );
        return;
    }
  }

  s->config.set_timecode_format( ardourtc );
}


int main( int argc, char* argv[] )
{
  ARDOUR::SampleFormat bitdepth = ARDOUR::FormatInt24;
  int    bitdepthint    = 0;
	int    samplerate     = 0;
	int    master_bus_chn = 2;
	string template_path;
  string output_folder;
	string session_name;
  string aaf_file;

	const char* optstring = "hlm:r:b:t:p:s:a:";


	const struct option longopts[] = {
    { "help",            no_argument,       0, 'h' },

		{ "list-templates",  no_argument,       0, 'l' },

		{ "master-channels", required_argument, 0, 'm' },

		{ "sample-rate",     required_argument, 0, 'r' },
    { "bit-depth",       required_argument, 0, 'b' },

		{ "template",        required_argument, 0, 't' },
    { "session-path",    required_argument, 0, 'p' },
		{ "session-name",    required_argument, 0, 's' },
    { "aaf",             required_argument, 0, 'a' },
	};

	int c = 0;
	while ( EOF != (c = getopt_long( argc, argv, optstring, longopts, (int*)0 )) )
	{
		switch (c)
		{
      case 'h':
        usage();
        break;


      case 'l':
        list_templates();
        exit( EXIT_SUCCESS );
        break;


      case 'm':
        master_bus_chn = atoi(optarg);
        /* TODO check min / max */
        break;


      case 'r':
        samplerate = atoi(optarg);

        if (samplerate < 44100 || samplerate > 192000)
        {
          PRINT_E("Invalid sample rate (%s). Sample rate must be between 44100 and 192000.\n", optarg);
          ::exit( EXIT_FAILURE );
        }
        break;


      case 'b':
        bitdepthint = atoi(optarg);

        if ( bitdepthint != 16 && bitdepthint != 24 && bitdepthint != 32 )
        {
            PRINT_E("Invalid bit depth (%s). Bit depth must be either 16,24 or 32.\n", optarg);
            ::exit( EXIT_FAILURE );
        }
        break;


			case 't':
				template_path = template_path_from_name( optarg );
				if ( template_path.empty() )
				{
					cerr << "Invalid (non-existent) template:" << optarg << "\n";
					::exit( EXIT_FAILURE );
				}
				break;


      case 'p':
        output_folder = string(optarg);
        break;


			case 's':
				session_name = string(optarg);
				break;


      case 'a':
        aaf_file = string(optarg);
        break;


			default:
				cerr << "Error: unrecognized option. See --help for usage information.\n";
				::exit( EXIT_FAILURE );
				break;
		}
	}

  int missing_param = 0;

	if ( template_path.empty() )
	{
		PRINT_E( "Missing template. Use --template parameter.\n" );
    missing_param = 1;
	}

  if ( output_folder.empty() )
  {
    PRINT_E( "Missing session path. Use --session-path parameter.\n" );
    missing_param = 1;
  }

  if ( aaf_file.empty() )
  {
    PRINT_E( "Missing AAF file. Use --aaf parameter.\n" );
    missing_param = 1;
  }


  if ( missing_param )
  {
    ::exit( EXIT_FAILURE );
  }




	if ( fop_file_exists( string(output_folder + "/" + session_name).c_str(), NULL ) == DT_DIR )
	{
		PRINT_E( "Session folder already exists '%s'\n", string(output_folder + "/" + session_name).c_str() );
		::exit( EXIT_FAILURE );
	}




	AAF_Iface *aafi = aafi_alloc( NULL );

  if ( aafi_load_file( aafi, aaf_file.c_str() ) )
	{
		PRINT_E( "Could not load the AAF file.\n" );
		::exit( EXIT_FAILURE );
	}

  // printf("AAF samplerate : %i\n", aafi->Audio->samplesize );

  if ( !samplerate )
  {
    if ( aafi->Audio->samplerate > 0 )
      samplerate = aafi->Audio->samplerate;
    else
      samplerate = 48000;
  }

  if ( !bitdepthint )
  {
    if ( aafi->Audio->samplesize > 0 )
      bitdepthint = aafi->Audio->samplesize;
    else
      bitdepthint = 24;
  }

  switch ( bitdepthint )
  {
    case 16:  bitdepth = ARDOUR::FormatInt16; break;
    case 24:  bitdepth = ARDOUR::FormatInt24; break;
    case 32:  bitdepth = ARDOUR::FormatFloat; break;
    default:
      PRINT_E( "Invalid bit depth (%i). Bit depth must be either 16,24 or 32.\n", bitdepthint );
      ::exit( EXIT_FAILURE );
  }

	if ( session_name.empty() )
	{
		session_name = string(fop_trim_fileext(basename(aafi->aafd->cfbd->file)));
	}





	SessionUtils::init();
	Session* s = 0;

  // sleep(2);

	try {
		s = create_new_session( output_folder + "/" + session_name /*session_file*/, session_name, samplerate, bitdepth, master_bus_chn, template_path );
	} catch ( ARDOUR::SessionException& e ) {
		// cerr << "Error: " << e.what () << "\n";
    PRINT_E( "Could not create ardour session : %s\n", e.what() );
		SessionUtils::unload_session(s);
		SessionUtils::cleanup();
		aafi_release( &aafi );
		::exit( EXIT_FAILURE );
	} catch (...) {
		// cerr << "Error: unknown exception.\n";
    PRINT_E( "Could not create ardour session.\n" );
		SessionUtils::unload_session(s);
		SessionUtils::cleanup();
		aafi_release( &aafi );
		::exit( EXIT_FAILURE );
	}

  // usleep(1000000);


	/* To prevent random segmentation faults
	 * =====================================
	 * https://github.com/Ardour/ardour/blob/365f6d633731229e7bc5d37a5fe2c9107b527e28/libs/ardour/import_pt.cc#L212
...
SIGSEGV___D
SIGSEGV___E
SIGSEGV___B
SIGSEGV___B1
SIGSEGV___B2
SIGSEGV___B3 ---- 1
SIGSEGV___B4
SIGSEGV___B5
SIGSEGV___C
SIGSEGV___D
SIGSEGV___E
SIGSEGV___B
SIGSEGV___B1
SIGSEGV___B2
ITW over-read: 516095
SIGSEGV___B3 ---- 1
==2688== Thread 14:
==2688== Invalid read of size 8
==2688==    at 0x8E962D7: __dynamic_cast (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25)
==2688==    by 0x5855D78: boost::shared_ptr<ARDOUR::Track> boost::dynamic_pointer_cast<ARDOUR::Track, ARDOUR::Route>(boost::shared_ptr<ARDOUR::Route> const&) (shared_ptr.hpp:904)
==2688==    by 0x5EB7322: ARDOUR::Session::non_realtime_overwrite(int, bool&) (session_transport.cc:1307)
==2688==    by 0x5EB6F67: ARDOUR::Session::butler_transport_work() (session_transport.cc:1286)
==2688==    by 0x585272F: ARDOUR::Butler::thread_work() (butler.cc:209)
==2688==    by 0x5851FEE: ARDOUR::Butler::_thread_work(void*) (butler.cc:157)
==2688==    by 0x69B17FB: fake_thread_start(void*) (pthread_utils.cc:101)
==2688==    by 0x762CFA2: start_thread (pthread_create.c:486)
==2688==    by 0x92214CE: clone (clone.S:95)
==2688==  Address 0x8 is not stack'd, malloc'd or (recently) free'd
==2688==
==2688==
==2688== Process terminating with default action of signal 11 (SIGSEGV)
==2688==  Access not within mapped region at address 0x8
==2688==    at 0x8E962D7: __dynamic_cast (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25)
==2688==    by 0x5855D78: boost::shared_ptr<ARDOUR::Track> boost::dynamic_pointer_cast<ARDOUR::Track, ARDOUR::Route>(boost::shared_ptr<ARDOUR::Route> const&) (shared_ptr.hpp:904)
==2688==    by 0x5EB7322: ARDOUR::Session::non_realtime_overwrite(int, bool&) (session_transport.cc:1307)
==2688==    by 0x5EB6F67: ARDOUR::Session::butler_transport_work() (session_transport.cc:1286)
==2688==    by 0x585272F: ARDOUR::Butler::thread_work() (butler.cc:209)
==2688==    by 0x5851FEE: ARDOUR::Butler::_thread_work(void*) (butler.cc:157)
==2688==    by 0x69B17FB: fake_thread_start(void*) (pthread_utils.cc:101)
==2688==    by 0x762CFA2: start_thread (pthread_create.c:486)
==2688==    by 0x92214CE: clone (clone.S:95)
==2688==  If you believe this happened as a result of a stack
==2688==  overflow in your program's main thread (unlikely but
==2688==  possible), you can try to increase the size of the
==2688==  main thread stack using the --main-stacksize= flag.
==2688==  The main thread stack size used in this run was 8388608.
==2688==
==2688== HEAP SUMMARY:
==2688==     in use at exit: 109,164,256 bytes in 96,996 blocks
==2688==   total heap usage: 1,564,687 allocs, 1,467,691 frees, 1,494,801,548 bytes allocated
==2688==
==2688== LEAK SUMMARY:
==2688==    definitely lost: 71,807 bytes in 576 blocks
==2688==    indirectly lost: 62,751 bytes in 491 blocks
==2688==      possibly lost: 9,952 bytes in 38 blocks
==2688==    still reachable: 109,019,746 bytes in 95,891 blocks
==2688==                       of which reachable via heuristic:
==2688==                         length64           : 3,584 bytes in 74 blocks
==2688==                         newarray           : 846,736 bytes in 47 blocks
==2688==                         multipleinheritance: 61,920 bytes in 3 blocks
==2688==         suppressed: 0 bytes in 0 blocks
==2688== Rerun with --leak-check=full to see details of leaked memory
	*/
	Glib::Threads::Mutex::Lock lx( AudioEngine::instance()->process_lock (), Glib::Threads::NOT_LOCK );

  lx.acquire();





  /*
   *
   *  Extract audio files and import as sources
   *  libs/ardour/import_pt.cc#L188
   *
   */

  SourceList sources;
	SourceList just_one_src;
  ARDOUR::ImportStatus import_status;
  vector<boost::shared_ptr<Region> > source_regions;
  samplepos_t pos = -1;

  aafiAudioEssence *audioEssence = NULL;

	foreachEssence( audioEssence, aafi->Audio->Essences )
	{
    // if ( audioEssence->clip_count == 0 )
    //   continue;

		if ( audioEssence->is_embedded )
		{
      if ( aafi_extract_audio_essence( aafi, audioEssence, s->session_directory().sound_path().c_str(), NULL, (SF_FORMAT_WAV | SF_FORMAT_PCM_24) ) < 0 )
      {
          PRINT_E( "Could not extract audio file '%ls' from AAF.\n", audioEssence->unique_file_name );
          continue; // TODO or fail ?
      }
		}
    else
    {
      char *file = locate_external_essence_file( aafi, audioEssence->original_file );

      if ( !file )
      {
        PRINT_E( "Could not locate external audio file : '%ls'\n", audioEssence->original_file );
        continue;
      }

      char output_file[PATH_MAX+1];
      snprintf( output_file, PATH_MAX, "%s%c%s", s->session_directory().sound_path().c_str(), DIR_SEP, fop_get_filename(file) );

      audioEssence->exported_file_path = (wchar_t*)malloc( (PATH_MAX + 1) * sizeof(wchar_t) );
      swprintf( audioEssence->exported_file_path, (PATH_MAX * sizeof(wchar_t)), L"%s", output_file );

      if ( fop_cp( file, output_file, 0 ) )
      {
        PRINT_E( "Could not copy external audio file to session : '%ls'\n", audioEssence->original_file );
        continue;
      }
    }

    if ( !import_sndfile_as_region( s, audioEssence, SrcBest, pos, just_one_src, import_status, &source_regions ) )
    {
      PRINT_E( "Could not import '%ls' to session.\n", audioEssence->unique_file_name );
      continue; // TODO or fail ?
    }

    sources.push_back( just_one_src.back() );
    PRINT_I( "Source file '%ls' successfully imported to session.\n", audioEssence->unique_file_name );
	}

  just_one_src.clear();




  /*
   *
   *  Import all audio clips
   *
   */

  aafiAudioTrack   *audioTrack = NULL;
  aafiTimelineItem *audioItem  = NULL;
  aafiAudioClip    *audioClip  = NULL;

  foreach_audioTrack( audioTrack, aafi )
  {
    printf("%i\n", audioTrack->number );

    foreach_Item( audioItem, audioTrack )
    {

      if ( audioItem->type != AAFI_AUDIO_CLIP )
      {
          continue;
      }

      audioClip = (aafiAudioClip*)&audioItem->data;

      if ( audioClip->Essence == NULL )
      {
        continue;
      }


      for ( SourceList::iterator source = sources.begin(); source != sources.end(); ++source )
      {

        if ( strcmp( audioClip->Essence->ardour_src_id, (*source)->id().to_s().c_str() ) != 0 )
        {
          continue;
        }


        PRINT_I( "Importing new clip [%+05.1lf dB] on track %i @%s\n",
          (( audioClip->gain && audioClip->gain->flags & AAFI_AUDIO_GAIN_CONSTANT ) ? 20 * log10( rationalToFloat( audioClip->gain->value[0] ) ) : 0),
          audioClip->track->number,
          timecode_format_sampletime( eu2sample_fromclip( audioClip, (audioClip->pos  + audioClip->track->Audio->tc->start) ), samplerate, audioClip->track->Audio->tc->fps, false ).c_str()
        );


        /* Create Region
         * =============
         */

        boost::shared_ptr<Region> region = build_region_from_aaf_audioclip( audioClip, *source );

        if ( !region )
        {
          PRINT_E( "Could not create new region.\n" );
          ::exit( EXIT_FAILURE );
        }


        /* Enable "Move to Original Position"
         * =================================
         */

        for ( vector<boost::shared_ptr<Region>>::iterator region = source_regions.begin(); region != source_regions.end(); ++region )
        {
          if ( (*region)->source(0) == *source )
          {
            (*region)->set_position( eu2sample_fromclip( audioClip, (audioClip->pos  + audioClip->track->Audio->tc->start) ) - eu2sample_fromclip( audioClip, audioClip->essence_offset ) );
            // (*y)->set_length( (samplecnt_t)(eu2sample_fromclip( audioClip, audioClip->len ) + eu2sample_fromclip( audioClip, audioClip->essence_offset )), 0 );
            // (*y)->set_start( (samplecnt_t)(eu2sample_fromclip( audioClip, audioClip->essence_offset )) );
          }
        }


        lx.release();
        put_region_on_track_from_aaf_audioclip( audioClip, region, s );
        lx.acquire();

        set_region_gain_from_aaf_audioclip( audioClip, region );


        set_region_fade_from_aaf_audioclip( audioClip, region );

      }
    }
  }

  /*  Avoids the following output :
   *  programming error: SessionHandleRef exists across session deletion! Dynamic type: ARDOUR::SndFileSource @ 0x559054065c18
   *  programming error: SessionHandleRef exists across session deletion! Dynamic type: ARDOUR::AudioRegion @ 0x55b3b0554440
   */

  sources.clear();
  source_regions.clear();




  /*
   *  Set Session Range
   */

  set_session_range_from_aaf( s, aafi );



  /*
   *  Import Video from AAF
   */

	set_session_video_from_aaf( s, aafi );



  /*
   *  Import Video from AAF
   */

  set_session_fps_from_aaf( s, aafi );


  lx.release();




  import_status.progress = 1.0;
	import_status.done = true;
	s->save_state("");
	import_status.sources.clear();
  import_status.all_done = true;



  /* NOTE: we need to build this before releasing session ! */
  string session_file_path = s->session_directory().root_path() + string("/") + session_name + string(".ardour");


	SessionUtils::unload_session(s);
	SessionUtils::cleanup();

	aafi_release( &aafi );





  /*
   *  Map iCon QCon Midi input / output
   *  Looks like the loss of surface control between template and session is a
   *  bug in current Ardour's version
   */

  configure_midi_control_surface( session_file_path.c_str() );



	return 0;
}

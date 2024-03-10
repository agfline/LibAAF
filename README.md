# libaaf
> [!WARNING]
>
> While major version number remains 0, API MUST NOT be considered stable.
>
> Releases are made everytime libaaf code is updated into Ardour source tree.


libaaf is a C coded library for **AAF file reading**. The AAF beeing a quite complex file format, libaaf provides an easy-to-implement, audio-oriented, FLOSS solution.

libaaf runs under Linux, Windows and OSX.

## aaftool

**aaftool** is a command line tool to inspect AAF structure, essences, tracks and clips, extract embedded media files, etc.

```
aaftool Copyright (c) 2017-2024 Adrien Gesta-Fline
libaaf v0.6-45-g9171e40-dirty

usage: aaftool [analysis|extraction] [options] [AAFFILE]

 CFB Analysis:

   --cfb-header                       Display CFB Header.
   --cfb-fat                          Display CFB FAT.
   --cfb-minifat                      Display CFB MiniFAT.
   --cfb-difat                        Display CFB DiFAT.
   --cfb-nodes                        Display CFB node list.

   --cfb-node                 <path>  Display node located at the given <path>.


 AAF Analysis:

   --aaf-summary                      Display informations from both header and identification objects.
   --aaf-essences                     Display retrieved AAF audio and video essences.
   --aaf-clips                        Display retrieved AAF audio and video clips.
   --aaf-classes                      Display classes contained in AAF file.
   --aaf-meta                         Display classes and properties from the MetaDictionary.
   --aaf-properties                   Display properties of all objects in file.

   --trace                            Display AAF class/object structure while parsing.
   --dump-meta                        Display MetaProperties details for each parsed class containing meta properties.
   --dump_tagged_value                Display standard TaggedValue properties of Mobs and Components objects.
   --dump-class         <AAFClassID>  Display aaf properties of a specific AAFClass when it is parsed.
   --dump-class-raw     <AAFClassID>  Display raw properties of a specific AAFClass when it is parsed.


 Embedded Media Extraction:

   --extract-essences                 Extract all embedded audio essences as they are stored (wav or aiff),
                                      unless --extract-format is set. Raw PCM is extracted as wav file.
   --extract-clips                    Extract all embedded audio clips (trimed essences) as wav files.
   --extract-path             <path>  Location where embedded files are extracted.
   --extract-format       <bwav|wav>  Force extract format to wav or broadcast wav.
   --extract-mobid                    Name extracted files with their MobID. This also prevents any non-latin
                                      character in filename.


 Software Specific Options:

   --pt-true-fades                    Replace rendered ProTools fade clips with real fades when possible,
                                      when surrounding clips has enough handle size.
   --pt-remove-sae                    Remove all ProTools "sample accurate edit" small clips.


 Options:

   --media-location           <path>  Location of external audio and video essence files.

   --samplerate               <rate>  Sample rate used for converting displayed position
                                      and duration values. If not set, AAF dominant sample
                                      rate is used.
   --pos-format <tc|hms|samples|raw>  Position and duration display format.
   --show-automation                  Show track and clip automation values when --aaf-clips is set.
   --show-metadata                    Show source and clip metadata when --aaf-essences or --aaf-clips is set.

   --hide-path                        Don't print full path to located essences when --aaf-essences is set.

   --no-color                         Disable colored output.
   --log-file                 <file>  Save output to file instead of stdout.

   --verb                      <num>  0=quiet 1=error 2=warning 3=debug.
```

## libaaf Features

* Supports embedded raw PCM/WAV/AIFF essences and provides function for file extraction.
* Supports external essence files, with auto-location function (relative to AAF file location).
* Supports multi-channel clip out of a single multi-channel essence file.
* Supports multi-channel clip out of multiple single-channel essence files.
* Supports multi-channel tracks.
* Supports fade in/out and x-fades.
* Supports clip-based and track-based fixed and varying gain.
* Supports track-based fixed/varying pan.
* Supports timecode
* Supports composition metadata (comments, tapeid, etc.)
* libaaf being audio-oriented right now, it only supports a single video clip. Better video support could be implemented later.

AAF is known for its poor interoperability, each software having its own way to describe things, some adding non-standard features. libaaf implements all of those *proprietary features* that have been observed so far in dozens of files coming from the following software :

* Avid Media Composer 8.4.5 - 8.8.3 - 18.12.5 - 23.12
* Adobe Premiere Pro 12.0 - 23.5.0
* ProTools 10.3.10 - 2023.12
* Davinci Resolve 17.4.6 - 18.5
* Logic Pro 9.1.7 - 10.7.4
* Fairlight Evo/Dream 2.5.1 - 4.1.75

### Avid Media Composer

* Supports *SubClip* CompositionMobs.
* Supports *AdjustedClip* CompositionMobs.
* Supports proprietary *Audio Warp* OperationGroup (leading to a wrong clip source offset when importing in most commercial implementations).
* Supports proprietary markers: position, length, name, description, color.
* Supports proprietary track solo / mute.
* Supports proprietary fade curve types.
* Supports proprietary source metadata.
* Supports proprietary *SubClip* metadata.

### ProTools

* Supports recovering of real fades, out of rendered fade clips.
* Supports removing of *sample accurate edit* clips, recovering length of useful clips.

### Davinci Resolve

* Supports proprietary markers.


## Library usage

```C
#include <libaaf.h>

/* allocate a new AAF_Iface object */
AAF_Iface *aafi = aafi_alloc( NULL );

if ( !aafi ) {
	return -1; /* error */
}

/* set verbosity level, optional file pointer or callback function */
aafi_set_debug( aafi, VERB_DEBUG, 1, stdout, NULL, NULL );

/* set libaaf options */
aafi_set_option_int( aafi, "trace", 1 );
aafi_set_option_int( aafi, "dump_meta", 1 );
aafi_set_option_int( aafi, "dump_tagged_value", 1 );
aafi_set_option_int( aafi, "protools", (AAFI_PROTOOLS_OPT_REPLACE_CLIP_FADES | AAFI_PROTOOLS_OPT_REMOVE_SAMPLE_ACCURATE_EDIT)  );
aafi_set_option_str( aafi, "media_location", "./media/" );
aafi_set_option_str( aafi, "dump_class_aaf_properties", "AAFClassID_TimelineMobSlot" );

/* load AAF file */
if ( aafi_load_file( aafi, "./file.aaf" ) ) {
	return -1; /* error */
}
```

### Retrieve Media Essences

```C
aafiAudioEssenceFile *audioEssenceFile = NULL;

AAFI_foreachAudioEssenceFile( aafi, audioEssenceFile ) {
	/* process audio essence */
}
```

### Retrieve Tracks and Clips

```C
aafiAudioTrack   *audioTrack = NULL;
aafiTimelineItem *audioItem  = NULL;

AAFI_foreachAudioTrack( aafi, audioTrack ) {

	/* process track */

	AAFI_foreachTrackItem( audioTrack, audioItem ) {

		aafiAudioClip  *audioClip = aafi_timelineItemToAudioClip( audioItem );
		aafiTransition *crossFade = aafi_timelineItemToCrossFade( audioItem );

		if ( audioClip ) {

			/* process clip */

			aafiTransition *fadein  = aafi_getFadeIn( audioClip );
			aafiTransition *fadeout = aafi_getFadeOut( audioClip );

			if ( fadein ) {
				/* process fade in */
			}

			if ( fadeout ) {
				/* process fade out */
			}
		}
		else if ( crossFade ) {
			/* process cross fade */
		}
	}
}
```

## Build

libaaf doesn't rely on any specific dependency.

```
cd build
cmake ..
make
```

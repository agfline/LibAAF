> test.sh is not operational yet

There should be one AAF file for each test.

All essences must be as short as possible to keep the overall size low. Whenever possible, AAF files should rely on external essence files provided in [test/res/](https://github.com/agfline/LibAAF/tree/master/test/res). Otherwise, internal essence files should be taken from [test/res/](https://github.com/agfline/LibAAF/tree/master/test/res).

## Internal audio essence

All essence retrieval should get proper audio attributes : samplerate, samplesize, channel count.

- [x] Retrieve PCM audio
- [ ] Retrieve WAV audio <!-- Adobe Premiere Pro -->
- [ ] Retrieve AIFF audio <!-- ArdourXchange, Adobe Premiere Pro "creative_post.aaf" "100_BARS.aaf" "ADP_STTRACK_CLIPGAIN_TRACKGAIN_XFADE_AIFF_AUDIOFXRENDER.aaf"  -->

## External audio essence

All essence retrieval should get proper audio attributes : samplerate, samplesize, channel count.

- [x] Retrieve WAV audio with summary
- [x] Retrieve AIFF audio with summary
- [x] Retrieve MXF audio with PCMDescriptor
- [x] Retrieve MP3 audio with summary
- [x] Locate essence besides AAF file
- [x] Locate essence in a sub directory besides AAF file
- [x] Locate essence with user path

## Clip attributes

- [x] Mono clip positioning
- [x] Stereo clip positioning
- [ ] Mono clip offset in source file
- [ ] Stereo clip offset in source file
- [ ] Multiple clips sharing the same essence file <!-- Adobe Premiere pro -->

## Fades

- [ ] Retrieve fade-in on a mono clip
	- [ ] AAFI_INTERPOL_LINEAR
	- [ ] AAFI_INTERPOL_LOG
	- [ ] AAFI_INTERPOL_CONSTANT
	- [ ] AAFI_INTERPOL_POWER
	- [ ] AAFI_INTERPOL_BSPLINE
- [ ] Retrieve fade-out on a mono clip
	- [ ] AAFI_INTERPOL_LINEAR
	- [ ] AAFI_INTERPOL_LOG
	- [ ] AAFI_INTERPOL_CONSTANT
	- [ ] AAFI_INTERPOL_POWER
	- [ ] AAFI_INTERPOL_BSPLINE
- [ ] Retrieve cross-fade on a mono clip
	- [ ] AAFI_INTERPOL_LINEAR
	- [ ] AAFI_INTERPOL_LOG
	- [ ] AAFI_INTERPOL_CONSTANT
	- [ ] AAFI_INTERPOL_POWER
	- [ ] AAFI_INTERPOL_BSPLINE
- [ ] Retrieve fade-in on a stereo clip
- [ ] Retrieve fade-out on a stereo clip
- [ ] Retrieve cross-fade on a stereo clip

## Automation

- [ ] Mono clip volume automation
- [ ] Mono clip pan automation
- [ ] Stereo clip volume automation
- [ ] Stereo clip pan automation
- [ ] Mono track volume automation
- [ ] Mono track pan automation
- [ ] Stereo track volume automation
- [ ] Stereo track pan automation
- [ ] Mono clip + track volume automation
- [ ] Mono clip + track pan automation

## Vendor specific implementations

- [ ] PROTOOLS_REMOVE_SAMPLE_ACCURATE_EDIT
- [ ] PROTOOLS_REPLACE_CLIP_FADES
- [ ] RESOLVE_INCLUDE_DISABLED_CLIPS

## Video

- [ ] Single external video file
	- [ ] 24 FPS
	- [ ] 25 FPS
	- [ ] 29.97 FPS
	- [ ] 30 FPS
	- [ ] 50 FPS
	- [ ] 60 FPS

## Misc

- [x] Empty AAF file
- [ ] Comment
- [ ] Marker

## AAF Structure

- [ ] Multiple TopLevel CompositionMob
- [ ] SubLevel CompositionMob (AdjustedClip, SubClip, LowerLevel)

## Libs

- [ ] URIParser

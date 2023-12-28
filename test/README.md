> test.sh is not operational yet

There should be one AAF file for each test.

All essences must be as short as possible to keep the overall size low. Whenever possible, AAF files should rely on external essence files provided in [test/res/](https://github.com/agfline/LibAAF/tree/master/test/res). Otherwise, internal essence files should be taken from [test/res/](https://github.com/agfline/LibAAF/tree/master/test/res).

## Internal audio essence

All essence retrieval should get proper audio attributes : samplerate, samplesize, channel count.

- [ ] Retrieve PCM audio
	- [ ] 1 channel
	- [ ] 2 channel stereo
<!-- [ ] - [ ] Retrieve AES3 audio -->
- [ ] Retrieve WAV audio
- [ ] Retrieve AIFF audio
- [ ] Retrieve MXF audio

## External audio essence

All essence retrieval should get proper audio attributes : samplerate, samplesize, channel count.

- [ ] Retrieve WAV audio
	- [ ] Located besides AAF file
	- [ ] Located besides AAF file in a sub directory
	- [ ] Using user provided location
- [ ] Retrieve AIFF audio
	- [ ] Located besides AAF file
	- [ ] Located besides AAF file in a sub directory
	- [ ] Using user provided location
- [ ] Retrieve MXF audio
	- [ ] Located besides AAF file
	- [ ] Located besides AAF file in a sub directory
	- [ ] Using user provided location

## Track attributes

- [ ] Track name

## Clip attributes

- [ ] Mono clip position
- [ ] Stereo clip position
- [ ] Mono clip offset in essence file
- [ ] Stereo clip offset in essence file
- [ ] Multiple clips sharing the same essence file

## Fades

- [ ] Retrieve fade-in on a mono clip
	- [ ] AAFI_INTERPOL_NONE
	- [ ] AAFI_INTERPOL_LINEAR
	- [ ] AAFI_INTERPOL_LOG
	- [ ] AAFI_INTERPOL_CONSTANT
	- [ ] AAFI_INTERPOL_POWER
	- [ ] AAFI_INTERPOL_BSPLINE
- [ ] Retrieve fade-out on a mono clip
	- [ ] AAFI_INTERPOL_NONE
	- [ ] AAFI_INTERPOL_LINEAR
	- [ ] AAFI_INTERPOL_LOG
	- [ ] AAFI_INTERPOL_CONSTANT
	- [ ] AAFI_INTERPOL_POWER
	- [ ] AAFI_INTERPOL_BSPLINE
- [ ] Retrieve cross-fade on a mono clip
	- [ ] AAFI_INTERPOL_NONE
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

## Libs

- [ ] URIParser

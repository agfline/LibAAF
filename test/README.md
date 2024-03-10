
## Embedded audio essence

- [x] Retrieve PCM audio
- [x] Retrieve WAV audio with summary
- [x] Retrieve AIFF audio with summary

## External audio essence

- [x] Retrieve WAV audio with summary
- [x] Retrieve AIFF audio with summary
- [x] Retrieve MXF audio with PCMDescriptor
- [x] Retrieve MP3 audio with summary
- [x] Locate essence besides AAF file
- [x] Locate essence in a sub directory besides AAF file
- [x] Locate essence with user path <!-- PT_WAV_External.aaf -->

## Clip attributes

- [x] Mono clip positioning
- [x] Multichannel clip positioning
- [x] Multichannel stereo from multiple mono source files
- [x] Multichannel 5.1 from multiple mono source files
- [x] Multichannel 7.1 from multiple mono source files
- [x] Multichannel stereo from a single multichannel source file
- [x] Multichannel 5.1 from a single multichannel source file
- [x] Multichannel 7.1 from a single multichannel source file
- [x] Mono clip offset in source file <!-- PT_WAV_External.aaf and PT_AIFF_External.aaf -->
- [x] Multichannel clip offset in source file <!-- all the above AAF has offset in source -->
- [x] Multiple clips sharing the same essence file <!-- PR_Fades.aaf -->
<!-- - [ ] Skipping any 1:1 gain allows not to miss any other actual gain (eg. Resolve 18.5.AAF) -->

## Fades

- [x] Retrieve log fade-in / fade-out / x-fade on mono clip(s)
- [x] Retrieve log fade-in / fade-out / x-fade on stereo clip(s)
- [x] Retrieve linear fade-in / fade-out / x-fade on mono clip(s)
- [x] Retrieve linear fade-in / fade-out / x-fade on stereo clip(s)
- [x] Retrieve equal power fade-in / fade-out / x-fade on mono clip(s)
- [x] Retrieve equal power fade-in / fade-out / x-fade on stereo clip(s)

## Automation

- [x] Mono clip gain
- [x] Stereo clip gain
- [x] Mono clip volume automation
- [x] Stereo clip volume automation
- [ ] Mono track volume automation
- [ ] Stereo track volume automation
- [ ] Mono clip + track volume automation
- [x] Mono clip gain + clip volume automation
- [x] Stereo clip gain + clip volume automation
- [ ] Mono clip pan automation
- [ ] Stereo clip pan automation
- [ ] Mono track pan automation
- [ ] Stereo track pan automation

## Vendor specific implementations

- [ ] PROTOOLS_REMOVE_SAMPLE_ACCURATE_EDIT
- [ ] PROTOOLS_REPLACE_CLIP_FADES
- [ ] RESOLVE_INCLUDE_DISABLED_CLIPS

## Video

## Misc

- [x] Empty AAF file
- [x] Markers
- [ ] Composition Comments


## AAF Structure

- [ ] Multiple TopLevel CompositionMob
- [ ] SubLevel CompositionMob (AdjustedClip, SubClip, LowerLevel)

## Libs

- [ ] URIParser

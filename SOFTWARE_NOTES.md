## Adobe Premiere Pro

Tested versions :
* CC 2018 - 12.0

#### Transitions

ADP exports fades and cross fades.

#### Volume/Gain

ADP exports both clip-based gain and track-based gain as either fixed or varying value.
ADP does not export clip-based volume (curve).

#### PAN

ADP exports track-based PAN as either fixed or varying value.

#### Multi-channel Tracks

ADP splits multi-channel tracks into paned mono tracks on export, if **Breakout to Mono** is enabled. In this case, stereo tracks are suffixed with "*_G*" and "*_D*" (**TODO** language dependent ??) and 5.1 tracks with "*_G*", "*_D*", "*_C*", "*_LFE*", "*_SG*" and "*_SD*".

Otherwise, looks like tracks are split anyway ??? (**TODO** import *ADP3_51-ST-MONO-NOBREAKOUT.aaf* and *ADP3_51-ST-MONO.aaf* back to ADP, and see if it recognizes its own "multi-channels")

#### Rendered Audio Effects

ADP exports clips with effects rendered if **Render audio clip effects** is enabled. If **Include clip copies without effects** is enabled, ADP exports additional tracks with clean audio clips. Those track names are suffixed with "*_No FX*", while fx tracks are suffixed with "*FX*".

#### Misc

ADP supports nested sequences, however it doesn't export them as multiple CompositionMobs as it should, according to the AAF EditProtocol... so that's fine ! Instead, nested sequences are rendered as single clips.

ADP does not set the Composition TC start (aafi->Audio->tc->start)

## ProTools

Tested versions :
* 10.13.10

#### Transitions

ProTools does not export fades nor cross fades, instead it renders them as audio clips labeled "*Fade 1", "*Fade 2*", etc. or simply "*Fade*", translated to the current PT language, if **Enforce Media Composer Compatibility** was ticked (**TODO** what does that mean ?).

#### Volume / Gain

ProTools exports clip-based gain but not track-based gain if **Enforce Media Composer Compatibility** is enabled.

ProTools exports track-based gain as clip-based gain if **Enforce Media Composer Compatibility** is disabled. Therefore, original clip-based gain is not retained.

#### PAN

Looks like ProTools does not export single-value PAN data (**TODO** do more tests, with automation).

#### Multi-channel Tracks

ProTools splits multi-channel audio tracks into multiple mono tracks by default.

When **Export Stereo, 5.1 and 7.1 tracks as multi-channel** is enabled, multi-channel tracks are preserved in a non-standard way.

#### Essences

ProTools exports embedded audio as raw PCM only and external audio as WAVE, AIFF or MXF files.

## LogicPro

Tested versions :
* 9.1.7

#### Transitions

Logic does not export fades nor cross fades, instead it renders them as audio clips suffixed with "*.logic-f24m*" (**TODO** make sure of that, do more test !).

#### Volume / Gain

Logic exports track's volume automation as clip-based gain.
Logic does not export clip-based gain.

#### PAN

Logic does not export PAN.

#### Multi-channel Tracks

Logic splits multi-channel audio tracks into multiple mono tracks. In that case, clip name is suffixed with "*.L*" or "*.R*", and track name is suffixed with " *-L*" or " *-R*" (**TODO** what about 5.1, 7.1 ?)

#### Essences

Logic exports only external audio as WAVE or AIFF files.

#### Misc

Logic does not export track numbers.

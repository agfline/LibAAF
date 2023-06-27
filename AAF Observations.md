
## Resolve 18.5

  - Fixed track Volume and Pan are not exported.

  - Track Volume automation is exported clip-based.

  - Track Pan automation is exported track-based. However, it only works if the pan automation was "recorded". It doesn't work if automation points where manually added to the curve.

  - *Automation Follows Edit* have no effect on how automation is exported.

  - *Adaptative* Format tracks (custom 1 to 24 channels) are exported as multi mono tracks (one per channel). All tracks share the exact same name, as defined in Resolve.

  - 5.1 and 7.1 tracks are exported as multi-channel. In Resolve, tracks can be set as 5.1 (*SMPTE ordering*) / 5.1 Film (*ITU775 ordering*) / 7.1 (*SMPTE ordering*) / 7.1 (*ITU775 ordering*). In the exported AAF, no distinction is made between SMPTE and ITU775. On import back in Resolve, all surround tracks are imported in SMPTE ordering.

  - Disabled clips are "hidden" inside the exported AAF. They are exposed as a *Selector* Object with a *Filler* Object as Selected (default), and the clip as Alternate. So by default, Clip is "removed". Selector has a meta property `AAFClassID_Selector::ComponentAttributeList` that defines a boolean TaggedValue `_DISABLE_CLIP_FLAG`.

  ```
03411││              ├──◻ AAFClassID_Selector
01926││ Selected --> │    └──◻ AAFClassID_Filler
02359││ Alternate -> │    └──◻ AAFClassID_OperationGroup (OpIdent: AAFOperationDef_MonoAudioGain)
03822││              │         ├──◻ AAFClassID_VaryingValue
02877││              │         └──◻ AAFClassID_SourceClip
02882││              │              └──◻ AAFClassID_MasterMob (UsageCode: n/a) : speech-sample.mp3 - disabled
04460││              │                   └──◻ AAFClassID_TimelineMobSlot
03089││              │                        └──◻ AAFClassID_SourceClip
04167││              │                             └──◻ AAFClassID_SourceMob (UsageCode: n/a) : speech-sample.mp3 - disabled
01249││              │                                  └──◻ AAFClassID_PCMDescriptor
01455││              │                                       └──◻ AAFClassID_NetworkLocator : file:///C:/Users/user/Desktop/libAAF/test/res/speech-sample.mp3

  ```

  - Bounced clips are exported the exact same way as disabled clips (impossible to make a distinction between the two). The only exported clip as *Alternate* is the original one, the bounced one isn't exported at all.

  - When *Layered Audio Editing* is enabled and used, only the lower clip in the stack get exported.

  - All fades (fade objects AND "fairlight fades"), fade in/out and cross fades, get exported as linear. No matter the original curve chosen in Resolve. All fade length seem to be shorten (a 6 frames long fade get shorten to 5 frames on import back in Resolve).

  - Clip linking disapear on export.

  - Clip metadata (Camera, Reel, Scene, Shot, Take, 'Good Take', Color and Comments) are not exported.

  - Timeline Markers are exported, with *Name*, *Comment*, *Color* and *Length*.

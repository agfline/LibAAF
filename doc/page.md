
@page aaf_notes AAF Notes

@subpage aaf_multichannel_audio

@page aaf_multichannel_audio AAF Multichannel Audio

There are multiple ways an AAF can describe multichannel audio clip / essence.

Here is an attempt to summary what have been observed in real AAF files so far,
compared to what the standard allows...

## Multichannel Track

AAF doesn't provide any information to tell if a track is mono or multichannel.
LibAAF will guess the track format based on its clips format.

## Multichannel Clip

The only way observed so far to describe multichannel clips, is missing from the
AAF specifications.

All software observed so far (Avid MediaComposer, ProTools, Adobe Premiere Pro,
Davinci Resolve) that can export multichannel audio, use an **OperationGroup**
with an **AudioChannelCombiner** definition. This OperationGroup holds a list of
**SourceClip**s, all describing a single audio channel.

Then, each SourceClip points to a MasterMob, which points to SourceMob. So each
channel of a multichannel clip is always described independently, there is no
such thing as a multichannel SourceClip.

## PT_PCM_Internal_Multichannel_EMCC-ExpTracksAsMultiChannel.aaf

AAF file with internal (embedded) audio essences. Each audio channel of a clip is
stored as a separate mono file.

```
│ 00229│├── AAFClassID_CompositionMob (UsageCode: AAFUsage_TopLevel) : PT_PCM_Internal_Multichannel_EMCC-ExpTracksAsMultiChannel MobID: 060a2b340101010501010f10 - 13 - 00 - 00 - 00 - { 0x5e06f400 0xeb6e 0xa5ea { 0x00 0x1a 0xd2 0x18 0xc0 0x4d 0xa9 0xa9 } }  (MetaProps: MobAttributeList[0xfffd])
│ 00430││   ├── AAFClassID_TimelineMobSlot [slot:3 track:1 editrate:30/1] (DataDef: AAFDataDef_LegacySound) : Audio 1   (MetaProps: TimelineMobAttributeList[0xfffc])
│ 01621││   │   └── AAFClassID_Sequence (Length: 15)
│ 01941││   │       ├── AAFClassID_OperationGroup (OpIdent: AAFOperationDef_AudioChannelCombiner; Length: 1)   (MetaProps: ComponentAttributeList[0xffff])
│ 01186││   │       │   ├── AAFClassID_SourceClip (Length: 1; StartTime: 0)
│ 01189││   │       │   │   └── AAFClassID_MasterMob (UsageCode: n/a) : Sample accurate edit MobID: 060a2b340101010501010f10 - 13 - 00 - 00 - 00 - { 0x5d06f400 0xebce 0xa5ea { 0x00 0x42 0xf4 0x18 0xc0 0x4d 0xa9 0xa9 } }
│ 00513││   │       │   │       └── AAFClassID_TimelineMobSlot [slot:3 track:1 editrate:30/1] (DataDef: AAFDataDef_LegacySound) : PT_PCM_Internal_Multichannel_EMCC-ExpTracksAsMultiChannel
│ 01350││   │       │   │           └── AAFClassID_SourceClip (Length: 1; StartTime: 0)
│ 00268││   │       │   │               ├── AAFClassID_SourceMob (UsageCode: n/a) : PT_PCM_Internal_Multichannel_EMCC-ExpTracksAsMultiChannel MobID: 060a2b340101010501010f10 - 13 - 00 - 00 - 00 - { 0x3406f400 0xebab 0xa5ea { 0x00 0xda 0xe4 0x18 0xc0 0x4d 0xa9 0xa9 } }
│ 02797││   │       │   │               │   ├── AAFClassID_PCMDescriptor (ContainerIdent : AAFContainerDef_AAFKLV)
│      ││   │       │   │               │   │   
│ 00532││   │       │   │               │   └── AAFClassID_TimelineMobSlot [slot:3 track:-1 editrate:30/1] (DataDef: AAFDataDef_LegacySound)  
│ 01463││   │       │   │               │       └── AAFClassID_SourceClip (Length: 1; StartTime: 0)
│ 03105││   │       │   │               └── AAFClassID_EssenceData (Data: Data-2702)
│      ││   │       │   │                   
│ 01186││   │       │   └── AAFClassID_SourceClip (Length: 1; StartTime: 0)
│ 01189││   │       │       └── AAFClassID_MasterMob (UsageCode: n/a) : Sample accurate edit MobID: 060a2b340101010501010f10 - 13 - 00 - 00 - 00 - { 0x5e06f400 0xeb25 0xa5ea { 0x00 0x50 0xd1 0x18 0xc0 0x4d 0xa9 0xa9 } }
│ 00513││   │       │           └── AAFClassID_TimelineMobSlot [slot:3 track:1 editrate:30/1] (DataDef: AAFDataDef_LegacySound) : PT_PCM_Internal_Multichannel_EMCC-ExpTracksAsMultiChannel
│ 01350││   │       │               └── AAFClassID_SourceClip (Length: 1; StartTime: 0)
│ 00268││   │       │                   ├── AAFClassID_SourceMob (UsageCode: n/a) : PT_PCM_Internal_Multichannel_EMCC-ExpTracksAsMultiChannel MobID: 060a2b340101010501010f10 - 13 - 00 - 00 - 00 - { 0x4806f400 0xebc1 0xa5ea { 0x00 0x75 0x06 0x18 0xc0 0x4d 0xa9 0xa9 } }
│ 02797││   │       │                   │   ├── AAFClassID_PCMDescriptor (ContainerIdent : AAFContainerDef_AAFKLV)
│      ││   │       │                   │   │   
│ 00532││   │       │                   │   └── AAFClassID_TimelineMobSlot [slot:3 track:-1 editrate:30/1] (DataDef: AAFDataDef_LegacySound)  
│ 01463││   │       │                   │       └── AAFClassID_SourceClip (Length: 1; StartTime: 0)
│ 03105││   │       │                   └── AAFClassID_EssenceData (Data: Data-2702)
```

**Summary** :

```
AAFClassID_OperationGroup
|-- AAFClassID_SourceClip
|    |-- AAFClassID_MasterMob (A) : audio file (MONO)
|         |-- AAFClassID_SourceMob (C) : essence_data of left channel
|
|-- AAFClassID_SourceClip
|    |-- AAFClassID_MasterMob (B) : audio file (MONO)
|         |-- AAFClassID_SourceMob (D) : essence_data of right channel
```

```
audioEssencePtr
-> audioEssenceFile LEFT
-> audioEssenceFile RIGHT
```

## PT_Multichannel_stereo_multi_source.aaf

AAF file with external audio essences. Each audio channel of a clip is stored as
a separate mono file.

```
│ 00229│├── AAFClassID_CompositionMob (UsageCode: AAFUsage_TopLevel) : PT_Multichannel_stereo_multi_source MobID: 060a2b340101010501010f10 - 13 - 00 - 00 - 00 - { 0x7c32e800 0xeb5b 0xa59e { 0x00 0x35 0x19 0x18 0xc0 0x4d 0xa9 0xa9 } }  (MetaProps: MobAttributeList[0xfffd])
│ 00430││   ├── AAFClassID_TimelineMobSlot [slot:3 track:1 editrate:30/1] (DataDef: AAFDataDef_LegacySound) : Audio 1   (MetaProps: TimelineMobAttributeList[0xfffc])
│ 01621││   │   └── AAFClassID_Sequence (Length: 9054)
│ 00852││   │       ├── AAFClassID_Filler (Length: 9000)
│      ││   │       │   
│ 01941││   │       ├── AAFClassID_OperationGroup (OpIdent: AAFOperationDef_AudioChannelCombiner; Length: 53)   (MetaProps: ComponentAttributeList[0xffff])
│ 01186││   │       │   ├── AAFClassID_SourceClip (Length: 53; StartTime: 2)
│ 01189││   │       │   │   └── AAFClassID_MasterMob (UsageCode: n/a) : stereo-identif-01.L MobID: 060a2b340101010501010f10 - 13 - 00 - 00 - 00 - { 0x5a32e800 0xebcf 0xa59e { 0x00 0x46 0xd6 0x18 0xc0 0x4d 0xa9 0xa9 } }
│ 00513││   │       │   │       └── AAFClassID_TimelineMobSlot [slot:3 track:1 editrate:30/1] (DataDef: AAFDataDef_LegacySound) : PT_MultichammsyJFRRzgmmQRBB
│ 01350││   │       │   │           └── AAFClassID_SourceClip (Length: 60; StartTime: 0)
│ 00268││   │       │   │               └── AAFClassID_SourceMob (UsageCode: n/a) : PT_MultichammsyJFRRzgmmQRBB MobID: 060a2b340101010101010f00 - 13 - 00 - 00 - 00 - { 0x87664f00 0x6600 0x24ea { 0x88 0x06 0x0e 0x2b 0x34 0x7f 0x7f 0x2a } }
│ 02841││   │       │   │                   ├── AAFClassID_WAVEDescriptor (ContainerIdent : AAFContainerDef_AAF)
│ 03045││   │       │   │                   │   └── AAFClassID_NetworkLocator (URLString: file:///c%3a/Users/user/Desktop/LibAAFTestFiles/tmp/PT_Multichannel_stereo_multi_source/PT_MultichammsyJFRRzgmmQRBB.wav)
│      ││   │       │   │                   │       
│ 00532││   │       │   │                   └── AAFClassID_TimelineMobSlot [slot:3 track:-1 editrate:30/1] (DataDef: AAFDataDef_LegacySound)  
│ 01463││   │       │   │                       └── AAFClassID_SourceClip (Length: 60; StartTime: 0)
│ 01186││   │       │   └── AAFClassID_SourceClip (Length: 53; StartTime: 2)
│ 01189││   │       │       └── AAFClassID_MasterMob (UsageCode: n/a) : stereo-identif-01.R MobID: 060a2b340101010501010f10 - 13 - 00 - 00 - 00 - { 0x6d32e800 0xeb53 0xa59e { 0x00 0x70 0xaf 0x18 0xc0 0x4d 0xa9 0xa9 } }
│ 00513││   │       │           └── AAFClassID_TimelineMobSlot [slot:3 track:1 editrate:30/1] (DataDef: AAFDataDef_LegacySound) : PT_MultichapyccMFRRgvmmQRBB
│ 01350││   │       │               └── AAFClassID_SourceClip (Length: 60; StartTime: 0)
│ 00268││   │       │                   └── AAFClassID_SourceMob (UsageCode: n/a) : PT_MultichapyccMFRRgvmmQRBB MobID: 060a2b340101010101010f00 - 13 - 00 - 00 - 00 - { 0x8766b400 0xe700 0x2511 { 0x88 0x06 0x0e 0x2b 0x34 0x7f 0x7f 0x2a } }
│ 02841││   │       │                       ├── AAFClassID_WAVEDescriptor (ContainerIdent : AAFContainerDef_AAF)
│ 03045││   │       │                       │   └── AAFClassID_NetworkLocator (URLString: file:///c%3a/Users/user/Desktop/LibAAFTestFiles/tmp/PT_Multichannel_stereo_multi_source/PT_MultichapyccMFRRgvmmQRBB.wav)
│      ││   │       │                       │       
│ 00532││   │       │                       └── AAFClassID_TimelineMobSlot [slot:3 track:-1 editrate:30/1] (DataDef: AAFDataDef_LegacySound)  
│ 01463││   │       │                           └── AAFClassID_SourceClip (Length: 60; StartTime: 0)
│      ││   │       │   
```

**Summary** :

```
AAFClassID_OperationGroup
|-- AAFClassID_SourceClip
|    |-- AAFClassID_MasterMob (A) : audio file (MONO LEFT)
|         |-- AAFClassID_SourceMob (C) : /path/to/mono_file_left_channel.wav
|
|-- AAFClassID_SourceClip
|    |-- AAFClassID_MasterMob (B) : audio file (MONO RIGHT)
|         |-- AAFClassID_SourceMob (D) : /path/to/mono_file_right_channel.wav
```

```
audioEssencePtr
-> audioEssenceFile LEFT
-> audioEssenceFile RIGHT
```

## DR_Multichannel_stereo_single_source.aaf

AAF file with external audio essences. Stereo clip is stored as a standard interleaved
WAV audio file. Each channel of the clip is described by a separate SourceMob, describing
the same multichannel source file. Each MasterMob's SourceClip is targetting a channel
of that source file,

```
│ 00229│└── AAFClassID_CompositionMob (UsageCode: AAFUsage_TopLevel) : DR_Multichannel_stereo_single_source MobID: 060a2b340101010101010f00 - 13 - 00 - 00 - 00 - { 0x9dd07400 0xa765 0x5a81 { 0x20 0x06 0x0e 0x2b 0x34 0x7f 0x7f 0x2a } }
│ 00494│    ├── AAFClassID_TimelineMobSlot [slot:1 track:1 editrate:24/1] (DataDef: AAFDataDef_Timecode)  
│ 01544│    │   └── AAFClassID_Timecode (Length: 7236)
│      │    │       
│ 00479│    ├── AAFClassID_TimelineMobSlot [slot:2 track:1 editrate:24/1] (DataDef: AAFDataDef_Picture)  
│ 01621│    │   └── AAFClassID_Sequence (Length: 7236)
[warning] AAFCore.c:1022 in aaf_get_property() : Could not retrieve 0x0000 (Unknown PID_MetaDictionary) of Class AAFClassID_Sequence
│ 00852│    │       └── AAFClassID_Filler (Length: 7236)
│      │    │           
│ 00430│    └── AAFClassID_TimelineMobSlot [slot:3 track:1 editrate:48000/1] (DataDef: AAFDataDef_Sound) : Audio 1   (MetaProps: TimelineMobAttributeList[0xffff])
│ 01621│        └── AAFClassID_Sequence (Length: 14472000)
[warning] AAFCore.c:1022 in aaf_get_property() : Could not retrieve 0x0000 (Unknown PID_MetaDictionary) of Class AAFClassID_Sequence
│ 00852│            ├── AAFClassID_Filler (Length: 14384522)
│      │            │   
│ 01941│            ├── AAFClassID_OperationGroup (OpIdent: AAFOperationDef_AudioChannelCombiner; Length: 87334)
[warning] AAFCore.c:1022 in aaf_get_property() : Could not retrieve 0x0000 (Unknown PID_MetaDictionary) of Class AAFClassID_SourceClip
│ 01186│            │   ├── AAFClassID_SourceClip (Length: 86000; StartTime: 2000)
│ 01189│            │   │   └── AAFClassID_MasterMob (UsageCode: n/a) : stereo-identif MobID: 060a2b340101010101010f00 - 13 - 00 - 00 - 00 - { 0x9dd07400 0xa865 0x5a81 { 0x20 0x06 0x0e 0x2b 0x34 0x7f 0x7f 0x2a } }
│ 00513│            │   │       └── AAFClassID_TimelineMobSlot [slot:1 track:1 editrate:48000/1] (DataDef: AAFDataDef_Sound)  
│ 01350│            │   │           └── AAFClassID_SourceClip (Length: 93860; StartTime: 0)
│ 00268│            │   │               └── AAFClassID_SourceMob (UsageCode: n/a) : stereo-identif MobID: 060a2b340101010101010f00 - 13 - 00 - 00 - 00 - { 0x9dd07400 0xa965 0x5a81 { 0x20 0x06 0x0e 0x2b 0x34 0x7f 0x7f 0x2a } }
│ 02841│            │   │                   ├── AAFClassID_WAVEDescriptor (ContainerIdent : n/a)
│ 03045│            │   │                   │   └── AAFClassID_NetworkLocator (URLString: file://localhost/C:/Users/user/Desktop/res/stereo-identif.wav)
│      │            │   │                   │       
│ 00532│            │   │                   └── AAFClassID_TimelineMobSlot [slot:1 track:1 editrate:48000/1] (DataDef: AAFDataDef_Sound)  
│ 01463│            │   │                       └── AAFClassID_SourceClip (Length: 93860; StartTime: 0)
│ 01186│            │   └── AAFClassID_SourceClip (Length: 86000; StartTime: 2000)
│ 01189│            │       └── AAFClassID_MasterMob (UsageCode: n/a) : stereo-identif MobID: 060a2b340101010101010f00 - 13 - 00 - 00 - 00 - { 0x9dd07400 0xa865 0x5a81 { 0x20 0x06 0x0e 0x2b 0x34 0x7f 0x7f 0x2a } }
│ 00513│            │           └── AAFClassID_TimelineMobSlot [slot:2 track:2 editrate:48000/1] (DataDef: AAFDataDef_Sound)  
│ 01350│            │               └── AAFClassID_SourceClip (Length: 93860; StartTime: 0)
│ 00268│            │                   └── AAFClassID_SourceMob (UsageCode: n/a) : stereo-identif MobID: 060a2b340101010101010f00 - 13 - 00 - 00 - 00 - { 0x9dd07400 0xab65 0x5a81 { 0x20 0x06 0x0e 0x2b 0x34 0x7f 0x7f 0x2a } }
│ 02841│            │                       ├── AAFClassID_WAVEDescriptor (ContainerIdent : n/a)
│ 03045│            │                       │   └── AAFClassID_NetworkLocator (URLString: file://localhost/C:/Users/user/Desktop/res/stereo-identif.wav)
│      │            │                       │       
│ 00532│            │                       └── AAFClassID_TimelineMobSlot [slot:1 track:2 editrate:48000/1] (DataDef: AAFDataDef_Sound)  
│ 01463│            │                           └── AAFClassID_SourceClip (Length: 93860; StartTime: 0)
│      │            │   
```

**Summary** :

```
AAFClassID_OperationGroup
|-- AAFClassID_SourceClip
|    |-- AAFClassID_MasterMob (A) : audio file (STEREO) points to SourceMob's TimelineMobSlot 1 (channel left)
|         |-- AAFClassID_SourceMob (B) : /path/to/stereo_file.wav
|
|-- AAFClassID_SourceClip
|    |-- AAFClassID_MasterMob (A) : audio file (STEREO) points to SourceMob's TimelineMobSlot 2 (channel right)
|         |-- AAFClassID_SourceMob (C) : /path/to/stereo_file.wav
```

In this case, there should be only one single SourceMob to describe stereo_file.wav.
Instead, there are two different SourceMobs describing twice the exact same thing.
So we can't tell with certainty if those two are describing the same file or not.

```
audioEssencePtr
-> audioEssenceFile STEREO
```

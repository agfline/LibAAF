

static const char ARDOUR_SESSION_FILE_CONTENT[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?> \n\
<Session version=\"3002\" name=\"tmp\" sample-rate=\"48000\" end-is-free=\"1\" id-counter=\"19045\" name-counter=\"1\" event-counter=\"382\" vca-counter=\"1\"> \n\
  <ProgramVersion created-with=\"Ardour 5.10-525-ga1143a0ed\" modified-with=\"Ardour 5.10-525-ga1143a0ed\"/> \n\
  <MIDIPorts> \n\
    <Port name=\"MIDI Clock in\" direction=\"input\"/> \n\
    <Port name=\"MIDI Clock out\" direction=\"output\"/> \n\
    <Port name=\"MIDI control in\" direction=\"input\"/> \n\
    <Port name=\"MIDI control out\" direction=\"output\"/> \n\
    <Port name=\"MMC in\" direction=\"input\"/> \n\
    <Port name=\"MMC out\" direction=\"output\"/> \n\
    <Port name=\"MTC in\" direction=\"input\"/> \n\
    <Port name=\"MTC out\" direction=\"output\"/> \n\
    <Port name=\"Scene in\" direction=\"input\"/> \n\
    <Port name=\"Scene out\" direction=\"output\"/> \n\
  </MIDIPorts> \n\
  <Config> \n\
    <Option name=\"destructive-xfade-msecs\" value=\"2\"/> \n\
    <Option name=\"use-region-fades\" value=\"1\"/> \n\
    <Option name=\"use-transport-fades\" value=\"1\"/> \n\
    <Option name=\"use-monitor-fades\" value=\"1\"/> \n\
    <Option name=\"native-file-data-format\" value=\"FormatInt16\"/> \n\
    <Option name=\"native-file-header-format\" value=\"WAVE\"/> \n\
    <Option name=\"auto-play\" value=\"0\"/> \n\
    <Option name=\"auto-return\" value=\"0\"/> \n\
    <Option name=\"auto-input\" value=\"1\"/> \n\
    <Option name=\"punch-in\" value=\"0\"/> \n\
    <Option name=\"punch-out\" value=\"0\"/> \n\
    <Option name=\"count-in\" value=\"0\"/> \n\
    <Option name=\"session-monitoring\" value=\"\"/> \n\
    <Option name=\"layered-record-mode\" value=\"0\"/> \n\
    <Option name=\"subframes-per-frame\" value=\"100\"/> \n\
    <Option name=\"timecode-format\" value=\"timecode_25\"/> \n\
    <Option name=\"minitimeline-span\" value=\"120\"/> \n\
    <Option name=\"raid-path\" value=\"\"/> \n\
    <Option name=\"audio-search-path\" value=\"\"/> \n\
    <Option name=\"midi-search-path\" value=\"\"/> \n\
    <Option name=\"track-name-number\" value=\"0\"/> \n\
    <Option name=\"track-name-take\" value=\"0\"/> \n\
    <Option name=\"take-name\" value=\"Take1\"/> \n\
    <Option name=\"jack-time-master\" value=\"1\"/> \n\
    <Option name=\"use-video-sync\" value=\"0\"/> \n\
    <Option name=\"video-pullup\" value=\"0\"/> \n\
    <Option name=\"external-sync\" value=\"0\"/> \n\
    <Option name=\"insert-merge-policy\" value=\"InsertMergeRelax\"/> \n\
    <Option name=\"timecode-offset\" value=\"0\"/> \n\
    <Option name=\"timecode-offset-negative\" value=\"1\"/> \n\
    <Option name=\"slave-timecode-offset\" value=\" 00:00:00:00\"/> \n\
    <Option name=\"timecode-generator-offset\" value=\" 00:00:00:00\"/> \n\
    <Option name=\"glue-new-markers-to-bars-and-beats\" value=\"0\"/> \n\
    <Option name=\"midi-copy-is-fork\" value=\"0\"/> \n\
    <Option name=\"glue-new-regions-to-bars-and-beats\" value=\"0\"/> \n\
    <Option name=\"realtime-export\" value=\"0\"/> \n\
    <Option name=\"use-video-file-fps\" value=\"0\"/> \n\
    <Option name=\"videotimeline-pullup\" value=\"1\"/> \n\
    <Option name=\"wave-amplitude-zoom\" value=\"0\"/> \n\
    <Option name=\"wave-zoom-factor\" value=\"2\"/> \n\
    <Option name=\"show-summary\" value=\"1\"/> \n\
    <Option name=\"show-group-tabs\" value=\"1\"/> \n\
    <Option name=\"show-region-fades\" value=\"1\"/> \n\
    <Option name=\"show-busses-on-meterbridge\" value=\"0\"/> \n\
    <Option name=\"show-master-on-meterbridge\" value=\"1\"/> \n\
    <Option name=\"show-midi-on-meterbridge\" value=\"1\"/> \n\
    <Option name=\"show-rec-on-meterbridge\" value=\"1\"/> \n\
    <Option name=\"show-mute-on-meterbridge\" value=\"0\"/> \n\
    <Option name=\"show-solo-on-meterbridge\" value=\"0\"/> \n\
    <Option name=\"show-monitor-on-meterbridge\" value=\"0\"/> \n\
    <Option name=\"show-name-on-meterbridge\" value=\"1\"/> \n\
    <Option name=\"meterbridge-label-height\" value=\"0\"/> \n\
  </Config> \n\
  <Metadata/> \n\
%s \
  <Selection> \n\
    <StripableAutomationControl stripable=\"18730\" control=\"0\" order=\"6\"/> \n\
  </Selection> \n\
  <Locations> \n\
    <Location id=\"20182\" name=\"session\" start=\"172800000\" end=\"179857919\" flags=\"IsSessionRange\" locked=\"1\" position-lock-style=\"AudioTime\"/> \n\
  </Locations> \n\
  <Bundles/> \n\
  <VCAManager/> \n\
  <Routes> \n\
    <Route id=\"18627\" name=\"Master\" default-type=\"audio\" strict-io=\"0\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak0dB\"> \n\
      <PresentationInfo order=\"5\" flags=\"MasterOut,OrderSet\" color=\"4222009087\"/> \n\
      <Controllable name=\"solo\" id=\"18635\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/> \n\
      <Controllable name=\"solo-iso\" id=\"18641\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/> \n\
      <Controllable name=\"solo-safe\" id=\"18643\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/> \n\
      <IO name=\"Master\" id=\"18655\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\"> \n\
        <Port type=\"audio\" name=\"Master/audio_in 1\"> \n\
          <Connection other=\"Audio 1/audio_out 1\"/> \n\
          <Connection other=\"Audio 2/audio_out 1\"/> \n\
          <Connection other=\"Audio 3/audio_out 1\"/> \n\
          <Connection other=\"Audio 4/audio_out 1\"/> \n\
          <Connection other=\"Audio 5/audio_out 1\"/> \n\
        </Port> \n\
        <Port type=\"audio\" name=\"Master/audio_in 2\"> \n\
          <Connection other=\"Audio 1/audio_out 2\"/> \n\
          <Connection other=\"Audio 2/audio_out 2\"/> \n\
          <Connection other=\"Audio 3/audio_out 2\"/> \n\
          <Connection other=\"Audio 4/audio_out 2\"/> \n\
          <Connection other=\"Audio 5/audio_out 2\"/> \n\
        </Port> \n\
      </IO> \n\
      <IO name=\"Master\" id=\"18656\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\"> \n\
        <Port type=\"audio\" name=\"Master/audio_out 1\"> \n\
          <Connection other=\"system:playback_1\"/> \n\
        </Port> \n\
        <Port type=\"audio\" name=\"Master/audio_out 2\"> \n\
          <Connection other=\"system:playback_2\"/> \n\
        </Port> \n\
      </IO> \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/> \n\
      <Controllable name=\"mute\" id=\"18637\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Controllable name=\"phase\" id=\"18639\" flags=\"Toggle\" value=\"0\" phase-invert=\"00\"/> \n\
      <Automation> \n\
        <AutomationList automation-id=\"solo\" id=\"18634\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"solo-iso\" id=\"18640\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"solo-safe\" id=\"18642\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"mute\" id=\"18636\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"phase\" id=\"18638\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
      </Automation> \n\
      <Pannable> \n\
        <Controllable name=\"pan-azimuth\" id=\"18646\" flags=\"\" value=\"0.5\"/> \n\
        <Controllable name=\"pan-width\" id=\"18650\" flags=\"\" value=\"1\"/> \n\
        <Controllable name=\"pan-elevation\" id=\"18648\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-frontback\" id=\"18652\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-lfe\" id=\"18654\" flags=\"\" value=\"0\"/> \n\
        <Automation> \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"18645\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"18647\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-width\" id=\"18649\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"18651\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"18653\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
      </Pannable> \n\
      <Processor id=\"18658\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\"> \n\
        <Automation> \n\
          <AutomationList automation-id=\"trim\" id=\"18632\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
        <Controllable name=\"trimcontrol\" id=\"18633\" flags=\"GainLike\" value=\"1\"/> \n\
      </Processor> \n\
      <Processor id=\"18657\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\"> \n\
        <Automation> \n\
          <AutomationList automation-id=\"gain\" id=\"18630\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
        <Controllable name=\"gaincontrol\" id=\"18631\" flags=\"GainLike\" value=\"1\"/> \n\
      </Processor> \n\
      <Processor id=\"18659\" name=\"meter-Master\" active=\"1\" user-latency=\"0\" type=\"meter\"/> \n\
      <Processor id=\"18660\" name=\"Master\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Master\" type=\"main-outs\" role=\"Main\"> \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/> \n\
        <Pannable> \n\
          <Controllable name=\"pan-azimuth\" id=\"18646\" flags=\"\" value=\"0.5\"/> \n\
          <Controllable name=\"pan-width\" id=\"18650\" flags=\"\" value=\"1\"/> \n\
          <Controllable name=\"pan-elevation\" id=\"18648\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-frontback\" id=\"18652\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-lfe\" id=\"18654\" flags=\"\" value=\"0\"/> \n\
          <Automation> \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"18645\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"18647\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-width\" id=\"18649\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"18651\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"18653\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          </Automation> \n\
        </Pannable> \n\
      </Processor> \n\
      <Slavable/> \n\
    </Route> \n\
    <Route id=\"18730\" name=\"Audio 1\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak0dB\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\"> \n\
      <PresentationInfo order=\"0\" flags=\"AudioTrack,OrderSet\" color=\"2732956927\"/> \n\
      <Controllable name=\"solo\" id=\"18738\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/> \n\
      <Controllable name=\"solo-iso\" id=\"18744\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/> \n\
      <Controllable name=\"solo-safe\" id=\"18746\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/> \n\
      <IO name=\"Audio 1\" id=\"18758\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\"> \n\
        <Port type=\"audio\" name=\"Audio 1/audio_in 1\"/> \n\
      </IO> \n\
      <IO name=\"Audio 1\" id=\"18759\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\"> \n\
        <Port type=\"audio\" name=\"Audio 1/audio_out 1\"> \n\
          <Connection other=\"Master/audio_in 1\"/> \n\
        </Port> \n\
        <Port type=\"audio\" name=\"Audio 1/audio_out 2\"> \n\
          <Connection other=\"Master/audio_in 2\"/> \n\
        </Port> \n\
      </IO> \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/> \n\
      <Controllable name=\"mute\" id=\"18740\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Controllable name=\"phase\" id=\"18742\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/> \n\
      <Automation> \n\
        <AutomationList automation-id=\"solo\" id=\"18737\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"solo-iso\" id=\"18743\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"solo-safe\" id=\"18745\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"mute\" id=\"18739\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"rec-enable\" id=\"18765\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"rec-safe\" id=\"18767\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"phase\" id=\"18741\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"monitor\" id=\"18769\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
      </Automation> \n\
      <Pannable> \n\
        <Controllable name=\"pan-azimuth\" id=\"18749\" flags=\"\" value=\"0.5\"/> \n\
        <Controllable name=\"pan-width\" id=\"18753\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-elevation\" id=\"18751\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-frontback\" id=\"18755\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-lfe\" id=\"18757\" flags=\"\" value=\"0\"/> \n\
        <Automation> \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"18748\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"18750\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-width\" id=\"18752\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"18754\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"18756\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
      </Pannable> \n\
      <Processor id=\"18761\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\"> \n\
        <Automation> \n\
          <AutomationList automation-id=\"trim\" id=\"18735\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
        <Controllable name=\"trimcontrol\" id=\"18736\" flags=\"GainLike\" value=\"1\"/> \n\
      </Processor> \n\
      <Processor id=\"18760\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\"> \n\
        <Automation> \n\
          <AutomationList automation-id=\"gain\" id=\"18733\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
        <Controllable name=\"gaincontrol\" id=\"18734\" flags=\"GainLike\" value=\"1\"/> \n\
      </Processor> \n\
      <Processor id=\"18762\" name=\"meter-Audio 1\" active=\"1\" user-latency=\"0\" type=\"meter\"/> \n\
      <Processor id=\"18763\" name=\"Audio 1\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 1\" type=\"main-outs\" role=\"Main\"> \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/> \n\
        <Pannable> \n\
          <Controllable name=\"pan-azimuth\" id=\"18749\" flags=\"\" value=\"0.5\"/> \n\
          <Controllable name=\"pan-width\" id=\"18753\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-elevation\" id=\"18751\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-frontback\" id=\"18755\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-lfe\" id=\"18757\" flags=\"\" value=\"0\"/> \n\
          <Automation> \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"18748\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"18750\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-width\" id=\"18752\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"18754\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"18756\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          </Automation> \n\
        </Pannable> \n\
      </Processor> \n\
      <Slavable/> \n\
      <Controllable name=\"monitor\" id=\"18770\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/> \n\
      <Controllable name=\"rec-safe\" id=\"18768\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Controllable name=\"rec-enable\" id=\"18766\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 1.1\" name=\"Audio 1\" id=\"18771\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/> \n\
    </Route> \n\
    <Route id=\"18778\" name=\"Audio 2\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak0dB\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\"> \n\
      <PresentationInfo order=\"1\" flags=\"AudioTrack,OrderSet\" color=\"1921902079\"/> \n\
      <Controllable name=\"solo\" id=\"18786\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/> \n\
      <Controllable name=\"solo-iso\" id=\"18792\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/> \n\
      <Controllable name=\"solo-safe\" id=\"18794\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/> \n\
      <IO name=\"Audio 2\" id=\"18806\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\"> \n\
        <Port type=\"audio\" name=\"Audio 2/audio_in 1\"/> \n\
      </IO> \n\
      <IO name=\"Audio 2\" id=\"18807\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\"> \n\
        <Port type=\"audio\" name=\"Audio 2/audio_out 1\"> \n\
          <Connection other=\"Master/audio_in 1\"/> \n\
        </Port> \n\
        <Port type=\"audio\" name=\"Audio 2/audio_out 2\"> \n\
          <Connection other=\"Master/audio_in 2\"/> \n\
        </Port> \n\
      </IO> \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/> \n\
      <Controllable name=\"mute\" id=\"18788\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Controllable name=\"phase\" id=\"18790\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/> \n\
      <Automation> \n\
        <AutomationList automation-id=\"solo\" id=\"18785\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"solo-iso\" id=\"18791\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"solo-safe\" id=\"18793\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"mute\" id=\"18787\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"rec-enable\" id=\"18813\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"rec-safe\" id=\"18815\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"phase\" id=\"18789\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"monitor\" id=\"18817\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
      </Automation> \n\
      <Pannable> \n\
        <Controllable name=\"pan-azimuth\" id=\"18797\" flags=\"\" value=\"0.5\"/> \n\
        <Controllable name=\"pan-width\" id=\"18801\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-elevation\" id=\"18799\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-frontback\" id=\"18803\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-lfe\" id=\"18805\" flags=\"\" value=\"0\"/> \n\
        <Automation> \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"18796\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"18798\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-width\" id=\"18800\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"18802\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"18804\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
      </Pannable> \n\
      <Processor id=\"18809\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\"> \n\
        <Automation> \n\
          <AutomationList automation-id=\"trim\" id=\"18783\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
        <Controllable name=\"trimcontrol\" id=\"18784\" flags=\"GainLike\" value=\"1\"/> \n\
      </Processor> \n\
      <Processor id=\"18808\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\"> \n\
        <Automation> \n\
          <AutomationList automation-id=\"gain\" id=\"18781\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
        <Controllable name=\"gaincontrol\" id=\"18782\" flags=\"GainLike\" value=\"1\"/> \n\
      </Processor> \n\
      <Processor id=\"18810\" name=\"meter-Audio 2\" active=\"1\" user-latency=\"0\" type=\"meter\"/> \n\
      <Processor id=\"18811\" name=\"Audio 2\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 2\" type=\"main-outs\" role=\"Main\"> \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/> \n\
        <Pannable> \n\
          <Controllable name=\"pan-azimuth\" id=\"18797\" flags=\"\" value=\"0.5\"/> \n\
          <Controllable name=\"pan-width\" id=\"18801\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-elevation\" id=\"18799\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-frontback\" id=\"18803\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-lfe\" id=\"18805\" flags=\"\" value=\"0\"/> \n\
          <Automation> \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"18796\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"18798\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-width\" id=\"18800\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"18802\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"18804\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          </Automation> \n\
        </Pannable> \n\
      </Processor> \n\
      <Slavable/> \n\
      <Controllable name=\"monitor\" id=\"18818\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/> \n\
      <Controllable name=\"rec-safe\" id=\"18816\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Controllable name=\"rec-enable\" id=\"18814\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 2.1\" name=\"Audio 2\" id=\"18819\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/> \n\
    </Route> \n\
    <Route id=\"18826\" name=\"Audio 3\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak0dB\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\"> \n\
      <PresentationInfo order=\"2\" flags=\"AudioTrack,OrderSet\" color=\"1927538175\"/> \n\
      <Controllable name=\"solo\" id=\"18834\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/> \n\
      <Controllable name=\"solo-iso\" id=\"18840\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/> \n\
      <Controllable name=\"solo-safe\" id=\"18842\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/> \n\
      <IO name=\"Audio 3\" id=\"18854\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\"> \n\
        <Port type=\"audio\" name=\"Audio 3/audio_in 1\"/> \n\
      </IO> \n\
      <IO name=\"Audio 3\" id=\"18855\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\"> \n\
        <Port type=\"audio\" name=\"Audio 3/audio_out 1\"> \n\
          <Connection other=\"Master/audio_in 1\"/> \n\
        </Port> \n\
        <Port type=\"audio\" name=\"Audio 3/audio_out 2\"> \n\
          <Connection other=\"Master/audio_in 2\"/> \n\
        </Port> \n\
      </IO> \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/> \n\
      <Controllable name=\"mute\" id=\"18836\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Controllable name=\"phase\" id=\"18838\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/> \n\
      <Automation> \n\
        <AutomationList automation-id=\"solo\" id=\"18833\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"solo-iso\" id=\"18839\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"solo-safe\" id=\"18841\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"mute\" id=\"18835\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"rec-enable\" id=\"18861\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"rec-safe\" id=\"18863\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"phase\" id=\"18837\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"monitor\" id=\"18865\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
      </Automation> \n\
      <Pannable> \n\
        <Controllable name=\"pan-azimuth\" id=\"18845\" flags=\"\" value=\"0.5\"/> \n\
        <Controllable name=\"pan-width\" id=\"18849\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-elevation\" id=\"18847\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-frontback\" id=\"18851\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-lfe\" id=\"18853\" flags=\"\" value=\"0\"/> \n\
        <Automation> \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"18844\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"18846\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-width\" id=\"18848\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"18850\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"18852\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
      </Pannable> \n\
      <Processor id=\"18857\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\"> \n\
        <Automation> \n\
          <AutomationList automation-id=\"trim\" id=\"18831\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
        <Controllable name=\"trimcontrol\" id=\"18832\" flags=\"GainLike\" value=\"1\"/> \n\
      </Processor> \n\
      <Processor id=\"18856\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\"> \n\
        <Automation> \n\
          <AutomationList automation-id=\"gain\" id=\"18829\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
        <Controllable name=\"gaincontrol\" id=\"18830\" flags=\"GainLike\" value=\"1\"/> \n\
      </Processor> \n\
      <Processor id=\"18858\" name=\"meter-Audio 3\" active=\"1\" user-latency=\"0\" type=\"meter\"/> \n\
      <Processor id=\"18859\" name=\"Audio 3\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 3\" type=\"main-outs\" role=\"Main\"> \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/> \n\
        <Pannable> \n\
          <Controllable name=\"pan-azimuth\" id=\"18845\" flags=\"\" value=\"0.5\"/> \n\
          <Controllable name=\"pan-width\" id=\"18849\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-elevation\" id=\"18847\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-frontback\" id=\"18851\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-lfe\" id=\"18853\" flags=\"\" value=\"0\"/> \n\
          <Automation> \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"18844\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"18846\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-width\" id=\"18848\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"18850\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"18852\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          </Automation> \n\
        </Pannable> \n\
      </Processor> \n\
      <Slavable/> \n\
      <Controllable name=\"monitor\" id=\"18866\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/> \n\
      <Controllable name=\"rec-safe\" id=\"18864\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Controllable name=\"rec-enable\" id=\"18862\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 3.1\" name=\"Audio 3\" id=\"18867\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/> \n\
    </Route> \n\
    <Route id=\"18874\" name=\"Audio 4\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak0dB\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\"> \n\
      <PresentationInfo order=\"3\" flags=\"AudioTrack,OrderSet\" color=\"3856109311\"/> \n\
      <Controllable name=\"solo\" id=\"18882\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/> \n\
      <Controllable name=\"solo-iso\" id=\"18888\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/> \n\
      <Controllable name=\"solo-safe\" id=\"18890\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/> \n\
      <IO name=\"Audio 4\" id=\"18902\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\"> \n\
        <Port type=\"audio\" name=\"Audio 4/audio_in 1\"/> \n\
      </IO> \n\
      <IO name=\"Audio 4\" id=\"18903\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\"> \n\
        <Port type=\"audio\" name=\"Audio 4/audio_out 1\"> \n\
          <Connection other=\"Master/audio_in 1\"/> \n\
        </Port> \n\
        <Port type=\"audio\" name=\"Audio 4/audio_out 2\"> \n\
          <Connection other=\"Master/audio_in 2\"/> \n\
        </Port> \n\
      </IO> \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/> \n\
      <Controllable name=\"mute\" id=\"18884\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Controllable name=\"phase\" id=\"18886\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/> \n\
      <Automation> \n\
        <AutomationList automation-id=\"solo\" id=\"18881\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"solo-iso\" id=\"18887\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"solo-safe\" id=\"18889\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"mute\" id=\"18883\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"rec-enable\" id=\"18909\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"rec-safe\" id=\"18911\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"phase\" id=\"18885\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"monitor\" id=\"18913\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
      </Automation> \n\
      <Pannable> \n\
        <Controllable name=\"pan-azimuth\" id=\"18893\" flags=\"\" value=\"0.5\"/> \n\
        <Controllable name=\"pan-width\" id=\"18897\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-elevation\" id=\"18895\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-frontback\" id=\"18899\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-lfe\" id=\"18901\" flags=\"\" value=\"0\"/> \n\
        <Automation> \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"18892\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"18894\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-width\" id=\"18896\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"18898\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"18900\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
      </Pannable> \n\
      <Processor id=\"18905\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\"> \n\
        <Automation> \n\
          <AutomationList automation-id=\"trim\" id=\"18879\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
        <Controllable name=\"trimcontrol\" id=\"18880\" flags=\"GainLike\" value=\"1\"/> \n\
      </Processor> \n\
      <Processor id=\"18904\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\"> \n\
        <Automation> \n\
          <AutomationList automation-id=\"gain\" id=\"18877\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
        <Controllable name=\"gaincontrol\" id=\"18878\" flags=\"GainLike\" value=\"1\"/> \n\
      </Processor> \n\
      <Processor id=\"18906\" name=\"meter-Audio 4\" active=\"1\" user-latency=\"0\" type=\"meter\"/> \n\
      <Processor id=\"18907\" name=\"Audio 4\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 4\" type=\"main-outs\" role=\"Main\"> \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/> \n\
        <Pannable> \n\
          <Controllable name=\"pan-azimuth\" id=\"18893\" flags=\"\" value=\"0.5\"/> \n\
          <Controllable name=\"pan-width\" id=\"18897\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-elevation\" id=\"18895\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-frontback\" id=\"18899\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-lfe\" id=\"18901\" flags=\"\" value=\"0\"/> \n\
          <Automation> \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"18892\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"18894\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-width\" id=\"18896\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"18898\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"18900\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          </Automation> \n\
        </Pannable> \n\
      </Processor> \n\
      <Slavable/> \n\
      <Controllable name=\"monitor\" id=\"18914\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/> \n\
      <Controllable name=\"rec-safe\" id=\"18912\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Controllable name=\"rec-enable\" id=\"18910\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 4.1\" name=\"Audio 4\" id=\"18915\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/> \n\
    </Route> \n\
    <Route id=\"18922\" name=\"Audio 5\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak0dB\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\"> \n\
      <PresentationInfo order=\"4\" flags=\"AudioTrack,OrderSet\" color=\"2632307967\"/> \n\
      <Controllable name=\"solo\" id=\"18930\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/> \n\
      <Controllable name=\"solo-iso\" id=\"18936\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/> \n\
      <Controllable name=\"solo-safe\" id=\"18938\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/> \n\
      <IO name=\"Audio 5\" id=\"18950\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\"> \n\
        <Port type=\"audio\" name=\"Audio 5/audio_in 1\"/> \n\
      </IO> \n\
      <IO name=\"Audio 5\" id=\"18951\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\"> \n\
        <Port type=\"audio\" name=\"Audio 5/audio_out 1\"> \n\
          <Connection other=\"Master/audio_in 1\"/> \n\
        </Port> \n\
        <Port type=\"audio\" name=\"Audio 5/audio_out 2\"> \n\
          <Connection other=\"Master/audio_in 2\"/> \n\
        </Port> \n\
      </IO> \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/> \n\
      <Controllable name=\"mute\" id=\"18932\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Controllable name=\"phase\" id=\"18934\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/> \n\
      <Automation> \n\
        <AutomationList automation-id=\"solo\" id=\"18929\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"solo-iso\" id=\"18935\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"solo-safe\" id=\"18937\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"mute\" id=\"18931\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"rec-enable\" id=\"18957\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"rec-safe\" id=\"18959\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"phase\" id=\"18933\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
        <AutomationList automation-id=\"monitor\" id=\"18961\" interpolation-style=\"Discrete\" state=\"Off\"/> \n\
      </Automation> \n\
      <Pannable> \n\
        <Controllable name=\"pan-azimuth\" id=\"18941\" flags=\"\" value=\"0.5\"/> \n\
        <Controllable name=\"pan-width\" id=\"18945\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-elevation\" id=\"18943\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-frontback\" id=\"18947\" flags=\"\" value=\"0\"/> \n\
        <Controllable name=\"pan-lfe\" id=\"18949\" flags=\"\" value=\"0\"/> \n\
        <Automation> \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"18940\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"18942\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-width\" id=\"18944\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"18946\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"18948\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
      </Pannable> \n\
      <Processor id=\"18953\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\"> \n\
        <Automation> \n\
          <AutomationList automation-id=\"trim\" id=\"18927\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
        <Controllable name=\"trimcontrol\" id=\"18928\" flags=\"GainLike\" value=\"1\"/> \n\
      </Processor> \n\
      <Processor id=\"18952\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\"> \n\
        <Automation> \n\
          <AutomationList automation-id=\"gain\" id=\"18925\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
        </Automation> \n\
        <Controllable name=\"gaincontrol\" id=\"18926\" flags=\"GainLike\" value=\"1\"/> \n\
      </Processor> \n\
      <Processor id=\"18954\" name=\"meter-Audio 5\" active=\"1\" user-latency=\"0\" type=\"meter\"/> \n\
      <Processor id=\"18955\" name=\"Audio 5\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 5\" type=\"main-outs\" role=\"Main\"> \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/> \n\
        <Pannable> \n\
          <Controllable name=\"pan-azimuth\" id=\"18941\" flags=\"\" value=\"0.5\"/> \n\
          <Controllable name=\"pan-width\" id=\"18945\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-elevation\" id=\"18943\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-frontback\" id=\"18947\" flags=\"\" value=\"0\"/> \n\
          <Controllable name=\"pan-lfe\" id=\"18949\" flags=\"\" value=\"0\"/> \n\
          <Automation> \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"18940\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"18942\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-width\" id=\"18944\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"18946\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"18948\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
          </Automation> \n\
        </Pannable> \n\
      </Processor> \n\
      <Slavable/> \n\
      <Controllable name=\"monitor\" id=\"18962\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/> \n\
      <Controllable name=\"rec-safe\" id=\"18960\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Controllable name=\"rec-enable\" id=\"18958\" flags=\"Toggle,RealTime\" value=\"0\"/> \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 5.1\" name=\"Audio 5\" id=\"18963\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/> \n\
    </Route> \n\
  </Routes> \n\
  <UnusedPlaylists/> \n\
  <RouteGroups/> \n\
  <Click> \n\
    <IO name=\"Click\" id=\"18624\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\"> \n\
      <Port type=\"audio\" name=\"Click/audio_out 1\"> \n\
        <Connection other=\"system:playback_1\"/> \n\
      </Port> \n\
      <Port type=\"audio\" name=\"Click/audio_out 2\"> \n\
        <Connection other=\"system:playback_2\"/> \n\
      </Port> \n\
    </IO> \n\
    <Processor id=\"18625\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\"> \n\
      <Automation> \n\
        <AutomationList automation-id=\"gain\" id=\"18622\" interpolation-style=\"Linear\" state=\"Off\"/> \n\
      </Automation> \n\
      <Controllable name=\"gaincontrol\" id=\"18623\" flags=\"GainLike\" value=\"1\"/> \n\
    </Processor> \n\
  </Click> \n\
  <LTC-In> \n\
    <IO name=\"LTC In\" id=\"18620\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\"> \n\
      <Port type=\"audio\" name=\"LTC-in\"> \n\
        <Connection other=\"system:capture_1\"/> \n\
      </Port> \n\
    </IO> \n\
  </LTC-In> \n\
  <LTC-Out> \n\
    <IO name=\"LTC Out\" id=\"18621\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\"> \n\
      <Port type=\"audio\" name=\"LTC-out\"/> \n\
    </IO> \n\
  </LTC-Out> \n\
  <Speakers> \n\
    <Speaker azimuth=\"240\" elevation=\"0\" distance=\"1\"/> \n\
    <Speaker azimuth=\"120\" elevation=\"0\" distance=\"1\"/> \n\
  </Speakers> \n\
  <TempoMap> \n\
    <Tempo pulse=\"0\" frame=\"0\" movable=\"0\" lock-style=\"AudioTime\" beats-per-minute=\"120\" note-type=\"4\" clamped=\"0\" end-beats-per-minute=\"120\" active=\"1\" locked-to-meter=\"1\"/> \n\
    <Meter pulse=\"0\" frame=\"0\" movable=\"0\" lock-style=\"AudioTime\" bbt=\"1|1|0\" beat=\"0\" note-type=\"4\" divisions-per-bar=\"4\"/> \n\
  </TempoMap> \n\
  <ControlProtocols> \n\
    <Protocol name=\"PreSonus FaderPort8\" active=\"0\"/> \n\
    <Protocol name=\"Mackie\" active=\"0\"/> \n\
    <Protocol name=\"Generic MIDI\" active=\"0\"/> \n\
    <Protocol name=\"Steinberg CC121\" active=\"0\"/> \n\
    <Protocol name=\"Ableton Push 2\" active=\"0\"/> \n\
    <Protocol name=\"PreSonus FaderPort\" active=\"0\"/> \n\
    <Protocol name=\"Open Sound Control (OSC)\" active=\"0\"/> \n\
  </ControlProtocols> \n\
  <Extra> \n\
    <UI> \n\
      <Window name=\"key-editor\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"session-options-editor\" visible=\"0\" x-off=\"0\" y-off=\"214\" x-size=\"752\" y-size=\"342\"/> \n\
      <Window name=\"speaker-config\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"about\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"add-routes\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"290\" y-size=\"324\"/> \n\
      <Window name=\"add-video\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"inspector\" visible=\"0\" x-off=\"0\" y-off=\"0\" x-size=\"620\" y-size=\"370\"/> \n\
      <Window name=\"audio-midi-setup\" visible=\"0\" x-off=\"358\" y-off=\"185\" x-size=\"650\" y-size=\"398\"/> \n\
      <Window name=\"video-export\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"script-manager\" visible=\"0\" x-off=\"363\" y-off=\"323\" x-size=\"361\" y-size=\"324\"/> \n\
      <Window name=\"bundle-manager\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"locations\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"big-clock\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"audio-connection-manager\" visible=\"0\" x-off=\"338\" y-off=\"161\" x-size=\"514\" y-size=\"502\"/> \n\
      <Window name=\"midi-connection-manager\" visible=\"0\" x-off=\"440\" y-off=\"277\" x-size=\"310\" y-size=\"271\"/> \n\
      <Window name=\"idle-o-meter\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"RPM-18627\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"P-18627-18658\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18627-18657\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18627-18659\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18627-18660\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"PM-18658-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18657-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18659-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18660-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"RPM-18730\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"P-18730-18761\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18730-18760\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18730-18762\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18730-18763\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"PM-18761-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18760-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18762-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18763-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"RPM-18778\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"P-18778-18809\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18778-18808\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18778-18810\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18778-18811\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"PM-18809-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18808-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18810-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18811-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"RPM-18826\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"P-18826-18857\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18826-18856\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18826-18858\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18826-18859\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"PM-18857-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18856-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18858-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18859-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"RPM-18874\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"P-18874-18905\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18874-18904\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18874-18906\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18874-18907\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"PM-18905-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18904-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18906-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18907-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"RPM-18922\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"P-18922-18953\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18922-18952\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18922-18954\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"P-18922-18955\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"1\"/> \n\
      <Window name=\"PM-18953-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18952-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18954-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <Window name=\"PM-18955-\" visible=\"0\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/> \n\
      <GUIObjectState> \n\
        <Object id=\"strip 51\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 81\"/> \n\
        </Object> \n\
        <Object id=\"rtav 1114\" height=\"66\" visible=\"1\"/> \n\
        <Object id=\"automation 1118\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 1120\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 1114 12/0/0\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 1133\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"rtav 1162\" height=\"66\" visible=\"1\"/> \n\
        <Object id=\"automation 1166\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 1168\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 1162 12/0/0\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 1181\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"rtav 1210\" height=\"66\" visible=\"1\"/> \n\
        <Object id=\"automation 1214\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 1216\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 1210 12/0/0\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 1229\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"rtav 1258\" height=\"66\" visible=\"1\"/> \n\
        <Object id=\"automation 1262\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 1264\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 1258 12/0/0\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 1277\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"strip 154\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 184\"/> \n\
          <Object id=\"processor 4156\"/> \n\
          <Object id=\"processor 5958\"/> \n\
          <Object id=\"processor 5966\"> \n\
            <Object id=\"control 5980\" visible=\"1\"/> \n\
          </Object> \n\
          <Object id=\"processor 6241\"> \n\
            <Object id=\"control 6255\" visible=\"1\"/> \n\
          </Object> \n\
        </Object> \n\
        <Object id=\"strip 202\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 232\"/> \n\
          <Object id=\"processor 5987\"> \n\
            <Object id=\"control 6001\" visible=\"1\"/> \n\
          </Object> \n\
          <Object id=\"processor 6263\"> \n\
            <Object id=\"control 6277\" visible=\"1\"/> \n\
          </Object> \n\
          <Object id=\"processor 6285\"/> \n\
        </Object> \n\
        <Object id=\"strip 250\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 280\"/> \n\
        </Object> \n\
        <Object id=\"strip 298\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 328\"/> \n\
        </Object> \n\
        <Object id=\"strip 346\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 376\"/> \n\
        </Object> \n\
        <Object id=\"strip 394\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 424\"/> \n\
        </Object> \n\
        <Object id=\"strip 442\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 472\"/> \n\
        </Object> \n\
        <Object id=\"strip 490\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 520\"/> \n\
        </Object> \n\
        <Object id=\"strip 538\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 568\"/> \n\
        </Object> \n\
        <Object id=\"strip 586\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 616\"/> \n\
        </Object> \n\
        <Object id=\"strip 634\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 664\"/> \n\
        </Object> \n\
        <Object id=\"strip 682\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 712\"/> \n\
        </Object> \n\
        <Object id=\"strip 730\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 760\"/> \n\
        </Object> \n\
        <Object id=\"strip 778\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 808\"/> \n\
        </Object> \n\
        <Object id=\"strip 826\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 856\"/> \n\
        </Object> \n\
        <Object id=\"strip 874\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 904\"/> \n\
        </Object> \n\
        <Object id=\"strip 922\" visible=\"1\"> \n\
          <Object id=\"processor 952\"/> \n\
        </Object> \n\
        <Object id=\"strip 970\" visible=\"1\"> \n\
          <Object id=\"processor 1000\"/> \n\
        </Object> \n\
        <Object id=\"strip 1018\" visible=\"1\"> \n\
          <Object id=\"processor 1048\"/> \n\
        </Object> \n\
        <Object id=\"strip 1066\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 1096\"/> \n\
        </Object> \n\
        <Object id=\"strip 1114\" visible=\"1\"> \n\
          <Object id=\"processor 1144\"/> \n\
        </Object> \n\
        <Object id=\"strip 1162\" visible=\"1\"> \n\
          <Object id=\"processor 1192\"/> \n\
        </Object> \n\
        <Object id=\"strip 1210\" visible=\"1\"> \n\
          <Object id=\"processor 1240\"/> \n\
        </Object> \n\
        <Object id=\"strip 1258\" visible=\"1\"> \n\
          <Object id=\"processor 1288\"/> \n\
        </Object> \n\
        <Object id=\"strip 1874\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 1904\"/> \n\
        </Object> \n\
        <Object id=\"strip 1922\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 1952\"/> \n\
        </Object> \n\
        <Object id=\"strip 1970\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 2000\"/> \n\
        </Object> \n\
        <Object id=\"strip 2018\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 2048\"/> \n\
        </Object> \n\
        <Object id=\"strip 2134\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 2164\"/> \n\
        </Object> \n\
        <Object id=\"strip 2176\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 2206\"/> \n\
        </Object> \n\
        <Object id=\"strip 2218\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 2248\"/> \n\
        </Object> \n\
        <Object id=\"strip 2260\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 2290\"/> \n\
        </Object> \n\
        <Object id=\"automation 177\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"strip 6010\" strip-width=\"Wide\" visible=\"1\"> \n\
          <Object id=\"processor 6040\"/> \n\
        </Object> \n\
        <Object id=\"strip 6052\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 6082\"/> \n\
        </Object> \n\
        <Object id=\"strip 6094\" visible=\"1\"> \n\
          <Object id=\"processor 6124\"/> \n\
        </Object> \n\
        <Object id=\"strip 6136\" strip-width=\"Wide\" visible=\"1\"> \n\
          <Object id=\"processor 6166\"/> \n\
        </Object> \n\
        <Object id=\"rtav 18627\" height=\"66\" visible=\"1\"/> \n\
        <Object id=\"automation 18631\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18633\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18627 12/0/0\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18646\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18650\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"strip 18627\" visible=\"1\" strip-width=\"Wide\"> \n\
          <Object id=\"processor 18657\"/> \n\
        </Object> \n\
        <Object id=\"strip 18730\" visible=\"1\"> \n\
          <Object id=\"processor 18760\"/> \n\
        </Object> \n\
        <Object id=\"strip 18778\" visible=\"1\"> \n\
          <Object id=\"processor 18808\"/> \n\
        </Object> \n\
        <Object id=\"strip 18826\" visible=\"1\"> \n\
          <Object id=\"processor 18856\"/> \n\
        </Object> \n\
        <Object id=\"strip 18874\" visible=\"1\"> \n\
          <Object id=\"processor 18904\"/> \n\
        </Object> \n\
        <Object id=\"strip 18922\" visible=\"1\"> \n\
          <Object id=\"processor 18952\"/> \n\
        </Object> \n\
        <Object id=\"rtav 18730\" height=\"66\" visible=\"1\"/> \n\
        <Object id=\"automation 18734\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18736\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18730 12/0/0\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18749\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"rtav 18778\" height=\"66\" visible=\"1\"/> \n\
        <Object id=\"automation 18782\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18784\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18778 12/0/0\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18797\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"rtav 18826\" height=\"66\" visible=\"1\"/> \n\
        <Object id=\"automation 18830\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18832\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18826 12/0/0\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18845\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"rtav 18874\" height=\"66\" visible=\"1\"/> \n\
        <Object id=\"automation 18878\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18880\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18874 12/0/0\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18893\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"rtav 18922\" height=\"66\" visible=\"1\"/> \n\
        <Object id=\"automation 18926\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18928\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18922 12/0/0\" height=\"66\" visible=\"0\"/> \n\
        <Object id=\"automation 18941\" height=\"66\" visible=\"0\"/> \n\
      </GUIObjectState> \n\
    </UI> \n\
    <Videomonitor active=\"0\"/> \n\
  </Extra> \n\
  <Script lua=\"Lua 5.3\">c2NyaXB0cyA9IHt9IA==</Script> \n\
</Session>";

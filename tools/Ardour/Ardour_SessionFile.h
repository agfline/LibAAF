#ifndef __Ardour_SessionFile_h__
#define __Ardour_SessionFile_h__

static const char ARDOUR_SESSION_FILE_CONTENT[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>  \n\
<Session version=\"3002\" name=\"testF3\" sample-rate=\"48000\" end-is-free=\"0\" id-counter=\"16454\" name-counter=\"1\" event-counter=\"378\" vca-counter=\"1\">  \n\
  <ProgramVersion created-with=\"Ardour 1:5.5.0~dfsg-1+b1\" modified-with=\"Ardour 5.10-525-ga1143a0ed\"/>  \n\
  <MIDIPorts>  \n\
    <Port name=\"MIDI Clock in\" direction=\"input\"/>  \n\
    <Port name=\"MIDI Clock out\" direction=\"output\"/>  \n\
    <Port name=\"MIDI control in\" direction=\"input\"/>  \n\
    <Port name=\"MIDI control out\" direction=\"output\"/>  \n\
    <Port name=\"MMC in\" direction=\"input\"/>  \n\
    <Port name=\"MMC out\" direction=\"output\"/>  \n\
    <Port name=\"MTC in\" direction=\"input\"/>  \n\
    <Port name=\"MTC out\" direction=\"output\"/>  \n\
    <Port name=\"Scene in\" direction=\"input\"/>  \n\
    <Port name=\"Scene out\" direction=\"output\"/>  \n\
  </MIDIPorts>  \n\
  <Config>  \n\
    <Option name=\"destructive-xfade-msecs\" value=\"2\"/>  \n\
    <Option name=\"use-region-fades\" value=\"1\"/>  \n\
    <Option name=\"use-transport-fades\" value=\"1\"/>  \n\
    <Option name=\"use-monitor-fades\" value=\"1\"/>  \n\
    <Option name=\"native-file-data-format\" value=\"FormatFloat\"/>  \n\
    <Option name=\"native-file-header-format\" value=\"WAVE\"/>  \n\
    <Option name=\"auto-play\" value=\"0\"/>  \n\
    <Option name=\"auto-return\" value=\"0\"/>  \n\
    <Option name=\"auto-input\" value=\"1\"/>  \n\
    <Option name=\"punch-in\" value=\"0\"/>  \n\
    <Option name=\"punch-out\" value=\"0\"/>  \n\
    <Option name=\"count-in\" value=\"0\"/>  \n\
    <Option name=\"session-monitoring\" value=\"\"/>  \n\
    <Option name=\"layered-record-mode\" value=\"0\"/>  \n\
    <Option name=\"subframes-per-frame\" value=\"100\"/>  \n\
    <Option name=\"timecode-format\" value=\"timecode_30\"/>  \n\
    <Option name=\"minitimeline-span\" value=\"120\"/>  \n\
    <Option name=\"raid-path\" value=\"\"/>  \n\
    <Option name=\"audio-search-path\" value=\"\"/>  \n\
    <Option name=\"midi-search-path\" value=\"\"/>  \n\
    <Option name=\"track-name-number\" value=\"0\"/>  \n\
    <Option name=\"track-name-take\" value=\"0\"/>  \n\
    <Option name=\"take-name\" value=\"Take1\"/>  \n\
    <Option name=\"jack-time-master\" value=\"1\"/>  \n\
    <Option name=\"use-video-sync\" value=\"0\"/>  \n\
    <Option name=\"video-pullup\" value=\"0\"/>  \n\
    <Option name=\"external-sync\" value=\"0\"/>  \n\
    <Option name=\"insert-merge-policy\" value=\"InsertMergeRelax\"/>  \n\
    <Option name=\"timecode-offset\" value=\"0\"/>  \n\
    <Option name=\"timecode-offset-negative\" value=\"1\"/>  \n\
    <Option name=\"slave-timecode-offset\" value=\" 00:00:00:00\"/>  \n\
    <Option name=\"timecode-generator-offset\" value=\" 00:00:00:00\"/>  \n\
    <Option name=\"glue-new-markers-to-bars-and-beats\" value=\"0\"/>  \n\
    <Option name=\"midi-copy-is-fork\" value=\"0\"/>  \n\
    <Option name=\"glue-new-regions-to-bars-and-beats\" value=\"0\"/>  \n\
    <Option name=\"realtime-export\" value=\"0\"/>  \n\
    <Option name=\"use-video-file-fps\" value=\"0\"/>  \n\
    <Option name=\"videotimeline-pullup\" value=\"1\"/>  \n\
    <Option name=\"wave-amplitude-zoom\" value=\"0\"/>  \n\
    <Option name=\"wave-zoom-factor\" value=\"2\"/>  \n\
    <Option name=\"show-summary\" value=\"1\"/>  \n\
    <Option name=\"show-group-tabs\" value=\"1\"/>  \n\
    <Option name=\"show-region-fades\" value=\"1\"/>  \n\
    <Option name=\"show-busses-on-meterbridge\" value=\"0\"/>  \n\
    <Option name=\"show-master-on-meterbridge\" value=\"1\"/>  \n\
    <Option name=\"show-midi-on-meterbridge\" value=\"1\"/>  \n\
    <Option name=\"show-rec-on-meterbridge\" value=\"1\"/>  \n\
    <Option name=\"show-mute-on-meterbridge\" value=\"0\"/>  \n\
    <Option name=\"show-solo-on-meterbridge\" value=\"0\"/>  \n\
    <Option name=\"show-monitor-on-meterbridge\" value=\"0\"/>  \n\
    <Option name=\"show-name-on-meterbridge\" value=\"1\"/>  \n\
    <Option name=\"meterbridge-label-height\" value=\"0\"/>  \n\
  </Config>  \n\
  <Metadata/>  \n\
%s  \
  <Selection>  \n\
    <StripableAutomationControl stripable=\"154\" control=\"0\" order=\"3\"/>  \n\
  </Selection>  \n\
  <Locations>  \n\
    <Location id=\"6351\" name=\"session\" start=\"0\" end=\"98447552\" flags=\"IsSessionRange\" locked=\"0\" position-lock-style=\"AudioTime\"/>  \n\
  </Locations>  \n\
  <Bundles/>  \n\
  <VCAManager/>  \n\
  <Routes>  \n\
    <Route id=\"51\" name=\"Master\" default-type=\"audio\" strict-io=\"0\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak0dB\">  \n\
      <PresentationInfo order=\"28\" flags=\"MasterOut,OrderSet\" color=\"4222009087\"/>  \n\
      <Controllable name=\"solo\" id=\"59\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"65\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"67\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Master\" id=\"79\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Master/audio_in 1\">  \n\
          <Connection other=\"AMB 1/audio_out 1\"/>  \n\
          <Connection other=\"AMB 2/audio_out 1\"/>  \n\
          <Connection other=\"AMB 3/audio_out 1\"/>  \n\
          <Connection other=\"Audio 10/audio_out 1\"/>  \n\
          <Connection other=\"Audio 11/audio_out 1\"/>  \n\
          <Connection other=\"Audio 12/audio_out 1\"/>  \n\
          <Connection other=\"Audio 13/audio_out 1\"/>  \n\
          <Connection other=\"Audio 14/audio_out 1\"/>  \n\
          <Connection other=\"Audio 15/audio_out 1\"/>  \n\
          <Connection other=\"Audio 16/audio_out 1\"/>  \n\
          <Connection other=\"Audio 17/audio_out 1\"/>  \n\
          <Connection other=\"Audio 18/audio_out 1\"/>  \n\
          <Connection other=\"Audio 19/audio_out 1\"/>  \n\
          <Connection other=\"Audio 20/audio_out 1\"/>  \n\
          <Connection other=\"Audio 6/audio_out 1\"/>  \n\
          <Connection other=\"Audio 7/audio_out 1\"/>  \n\
          <Connection other=\"Audio 8/audio_out 1\"/>  \n\
          <Connection other=\"Audio 9/audio_out 1\"/>  \n\
          <Connection other=\"Bus 3/audio_out 1\"/>  \n\
          <Connection other=\"Bus 4/audio_out 1\"/>  \n\
          <Connection other=\"COM/audio_out 1\"/>  \n\
          <Connection other=\"ITW 1/audio_out 1\"/>  \n\
          <Connection other=\"ITW/audio_out 1\"/>  \n\
          <Connection other=\"PAD 1/audio_out 1\"/>  \n\
          <Connection other=\"PAD 2/audio_out 1\"/>  \n\
          <Connection other=\"PAD 3/audio_out 1\"/>  \n\
          <Connection other=\"PAD 4/audio_out 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Master/audio_in 2\">  \n\
          <Connection other=\"AMB 1/audio_out 2\"/>  \n\
          <Connection other=\"AMB 2/audio_out 2\"/>  \n\
          <Connection other=\"AMB 3/audio_out 2\"/>  \n\
          <Connection other=\"Audio 10/audio_out 2\"/>  \n\
          <Connection other=\"Audio 11/audio_out 2\"/>  \n\
          <Connection other=\"Audio 12/audio_out 2\"/>  \n\
          <Connection other=\"Audio 13/audio_out 2\"/>  \n\
          <Connection other=\"Audio 14/audio_out 2\"/>  \n\
          <Connection other=\"Audio 15/audio_out 2\"/>  \n\
          <Connection other=\"Audio 16/audio_out 2\"/>  \n\
          <Connection other=\"Audio 17/audio_out 2\"/>  \n\
          <Connection other=\"Audio 18/audio_out 2\"/>  \n\
          <Connection other=\"Audio 19/audio_out 2\"/>  \n\
          <Connection other=\"Audio 20/audio_out 2\"/>  \n\
          <Connection other=\"Audio 6/audio_out 2\"/>  \n\
          <Connection other=\"Audio 7/audio_out 2\"/>  \n\
          <Connection other=\"Audio 8/audio_out 2\"/>  \n\
          <Connection other=\"Audio 9/audio_out 2\"/>  \n\
          <Connection other=\"Bus 3/audio_out 2\"/>  \n\
          <Connection other=\"Bus 4/audio_out 2\"/>  \n\
          <Connection other=\"COM/audio_out 2\"/>  \n\
          <Connection other=\"ITW 1/audio_out 2\"/>  \n\
          <Connection other=\"ITW/audio_out 2\"/>  \n\
          <Connection other=\"PAD 1/audio_out 2\"/>  \n\
          <Connection other=\"PAD 2/audio_out 2\"/>  \n\
          <Connection other=\"PAD 3/audio_out 2\"/>  \n\
          <Connection other=\"PAD 4/audio_out 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <IO name=\"Master\" id=\"80\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Master/audio_out 1\">  \n\
          <Connection other=\"system:playback_1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Master/audio_out 2\">  \n\
          <Connection other=\"system:playback_2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"61\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"63\" flags=\"Toggle\" value=\"0\" phase-invert=\"00\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15818\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15824\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15826\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15820\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15822\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"70\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"74\" flags=\"\" value=\"1\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"72\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"76\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"78\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15829\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15831\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15833\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15835\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15837\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"82\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15816\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"57\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"81\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15814\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"55\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"83\" name=\"meter-Master\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"84\" name=\"Master\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Master\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"70\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"74\" flags=\"\" value=\"1\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"72\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"76\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"78\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15829\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15831\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15833\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15835\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15837\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
    </Route>  \n\
    <Route id=\"154\" name=\"ITW\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterInput\" meter-type=\"MeterPeak0dB\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"0\" flags=\"AudioTrack,OrderSet\" color=\"2716113919\"/>  \n\
      <Controllable name=\"solo\" id=\"162\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"168\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"170\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"ITW\" id=\"182\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"ITW/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"ITW\" id=\"183\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"ITW/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"ITW/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"164\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"166\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"14522\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"14528\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"14530\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"14524\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"14550\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"14552\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"14526\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"14554\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"173\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-width\" id=\"177\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"175\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"179\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"181\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"14533\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"14535\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"14537\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"14539\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"14541\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"185\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"14520\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"160\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"186\" name=\"meter-Audio 1\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"5958\" name=\"C* Sin - Sine wave generator\" active=\"1\" user-latency=\"0\" type=\"ladspa\" unique-id=\"1781\" count=\"1\" custom=\"0\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"parameter-0\" id=\"14558\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"parameter-1\" id=\"14560\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <ConfiguredInput>  \n\
          <Channels type=\"audio\" count=\"1\"/>  \n\
        </ConfiguredInput>  \n\
        <CustomSinks/>  \n\
        <ConfiguredOutput>  \n\
          <Channels type=\"audio\" count=\"1\"/>  \n\
        </ConfiguredOutput>  \n\
        <PresetOutput/>  \n\
        <InputMap-0/>  \n\
        <OutputMap-0>  \n\
          <Channelmap type=\"audio\" from=\"0\" to=\"0\"/>  \n\
        </OutputMap-0>  \n\
        <ThruMap/>  \n\
        <ladspa last-preset-uri=\"\" last-preset-label=\"\" parameter-changed-since-last-preset=\"1\">  \n\
          <Port number=\"0\" value=\"1000\"/>  \n\
          <Port number=\"1\" value=\"0.12600000202655792\"/>  \n\
        </ladspa>  \n\
        <Controllable name=\"f (Hz)\" id=\"5960\" flags=\"\" value=\"1000\" parameter=\"0\"/>  \n\
        <Controllable name=\"volume\" id=\"5962\" flags=\"\" value=\"0.12600000202655792\" parameter=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"184\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"14518\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"158\" flags=\"GainLike\" value=\"0.99999994039535522\"/>  \n\
      </Processor>  \n\
      <Processor id=\"6241\" name=\"ITW 1\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"1\" type=\"intsend\" role=\"Aux\" bitslot=\"3\" selfdestruct=\"0\" target=\"6010\" allow-feedback=\"0\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"14575\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\">  \n\
          <Panner uri=\"http://ardour.org/plugin/panner_1in2out\" type=\"Mono to Stereo Panner\"/>  \n\
        </PannerShell>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"173\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-width\" id=\"177\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"175\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"179\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"181\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"14533\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"14535\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"14537\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"14539\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"14541\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
        <Processor id=\"6256\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"gain\" id=\"14575\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
          <Controllable name=\"gaincontrol\" id=\"6255\" flags=\"GainLike\" value=\"0.99999994039535522\"/>  \n\
        </Processor>  \n\
      </Processor>  \n\
      <Processor id=\"187\" name=\"ITW\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"ITW\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"173\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-width\" id=\"177\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"175\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"179\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"181\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"14533\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"14535\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"14537\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"14539\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"14541\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"14555\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"14553\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"14551\" flags=\"Toggle,RealTime\" value=\"1\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"ITW\" name=\"ITW\" id=\"195\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"202\" name=\"AMB\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak0dB\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"1\" flags=\"AudioTrack,OrderSet\" color=\"1921902079\"/>  \n\
      <Controllable name=\"solo\" id=\"210\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"216\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"218\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"AMB\" id=\"230\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"AMB/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"AMB\" id=\"231\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"AMB/audio_out 1\">  \n\
          <Connection other=\"PAD 1/audio_in 1\"/>  \n\
          <Connection other=\"PAD 4/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"AMB/audio_out 2\">  \n\
          <Connection other=\"PAD 2/audio_in 1\"/>  \n\
          <Connection other=\"PAD 4/audio_in 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"212\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"214\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"14593\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"14599\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"14601\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"14595\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"14621\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"14623\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"14597\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"14625\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"221\" flags=\"\" value=\"1\"/>  \n\
        <Controllable name=\"pan-width\" id=\"225\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"223\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"227\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"229\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"14604\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"14606\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"14608\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"14610\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"14612\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"233\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"14591\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"208\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"6285\" name=\"C* Sin - Sine wave generator\" active=\"1\" user-latency=\"0\" type=\"ladspa\" unique-id=\"1781\" count=\"1\" custom=\"0\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"parameter-0\" id=\"14629\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"parameter-1\" id=\"14631\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <ConfiguredInput>  \n\
          <Channels type=\"audio\" count=\"1\"/>  \n\
        </ConfiguredInput>  \n\
        <CustomSinks/>  \n\
        <ConfiguredOutput>  \n\
          <Channels type=\"audio\" count=\"1\"/>  \n\
        </ConfiguredOutput>  \n\
        <PresetOutput/>  \n\
        <InputMap-0/>  \n\
        <OutputMap-0>  \n\
          <Channelmap type=\"audio\" from=\"0\" to=\"0\"/>  \n\
        </OutputMap-0>  \n\
        <ThruMap/>  \n\
        <ladspa last-preset-uri=\"\" last-preset-label=\"\" parameter-changed-since-last-preset=\"1\">  \n\
          <Port number=\"0\" value=\"440\"/>  \n\
          <Port number=\"1\" value=\"0.5\"/>  \n\
        </ladspa>  \n\
        <Controllable name=\"f (Hz)\" id=\"5960\" flags=\"\" value=\"440\" parameter=\"0\"/>  \n\
        <Controllable name=\"volume\" id=\"5962\" flags=\"\" value=\"0.5\" parameter=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"232\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"14589\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"206\" flags=\"GainLike\" value=\"0.99999994039535522\"/>  \n\
      </Processor>  \n\
      <Processor id=\"6263\" name=\"AMB 3\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"1\" type=\"intsend\" role=\"Aux\" bitslot=\"4\" selfdestruct=\"0\" target=\"6052\" allow-feedback=\"0\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"14646\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\">  \n\
          <Panner uri=\"http://ardour.org/plugin/panner_1in2out\" type=\"Mono to Stereo Panner\"/>  \n\
        </PannerShell>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"221\" flags=\"\" value=\"1\"/>  \n\
          <Controllable name=\"pan-width\" id=\"225\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"223\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"227\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"229\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"14604\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"14606\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"14608\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"14610\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"14612\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
        <Processor id=\"6278\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"gain\" id=\"14646\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
          <Controllable name=\"gaincontrol\" id=\"6277\" flags=\"GainLike\" value=\"0.99999994039535522\"/>  \n\
        </Processor>  \n\
      </Processor>  \n\
      <Processor id=\"234\" name=\"meter-Audio 2\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"235\" name=\"AMB\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"AMB\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"221\" flags=\"\" value=\"1\"/>  \n\
          <Controllable name=\"pan-width\" id=\"225\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"223\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"227\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"229\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"14604\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"14606\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"14608\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"14610\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"14612\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"14626\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"14624\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"14622\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"AMB\" name=\"AMB\" id=\"243\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"250\" name=\"AMB 1\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"2\" flags=\"AudioTrack,OrderSet\" color=\"1927538175\"/>  \n\
      <Controllable name=\"solo\" id=\"258\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"264\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"266\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"AMB 1\" id=\"278\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"AMB 1/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"AMB 1\" id=\"279\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"AMB 1/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
          <Connection other=\"PAD 1/audio_in 1\"/>  \n\
          <Connection other=\"PAD 4/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"AMB 1/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
          <Connection other=\"PAD 2/audio_in 1\"/>  \n\
          <Connection other=\"PAD 4/audio_in 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"260\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"262\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"14664\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"14670\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"14672\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"14666\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"14692\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"14694\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"14668\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"14696\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"269\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"273\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"271\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"275\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"277\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"14675\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"14677\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"14679\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"14681\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"14683\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"281\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"14662\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"256\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"280\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"14660\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"254\" flags=\"GainLike\" value=\"0.99999994039535522\"/>  \n\
      </Processor>  \n\
      <Processor id=\"282\" name=\"meter-Audio 3\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"283\" name=\"AMB 1\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"AMB 1\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"269\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"273\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"271\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"275\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"277\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"14675\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"14677\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"14679\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"14681\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"14683\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"14697\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"14695\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"14693\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"AMB\" name=\"AMB\" id=\"291\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"298\" name=\"AMB 2\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"3\" flags=\"AudioTrack,OrderSet\" color=\"3856109311\"/>  \n\
      <Controllable name=\"solo\" id=\"306\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"312\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"314\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"AMB 2\" id=\"326\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"AMB 2/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"AMB 2\" id=\"327\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"AMB 2/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
          <Connection other=\"PAD 1/audio_in 1\"/>  \n\
          <Connection other=\"PAD 4/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"AMB 2/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
          <Connection other=\"PAD 2/audio_in 1\"/>  \n\
          <Connection other=\"PAD 4/audio_in 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"308\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"310\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"14709\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"14715\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"14717\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"14711\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"14737\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"14739\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"14713\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"14741\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"317\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"321\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"319\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"323\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"325\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"14720\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"14722\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"14724\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"14726\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"14728\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"329\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"14707\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"304\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"328\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"14705\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"302\" flags=\"GainLike\" value=\"0.99999994039535522\"/>  \n\
      </Processor>  \n\
      <Processor id=\"330\" name=\"meter-Audio 4\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"331\" name=\"AMB 2\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"AMB 2\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"317\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"321\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"319\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"323\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"325\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"14720\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"14722\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"14724\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"14726\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"14728\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"14742\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"14740\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"14738\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"AMB\" name=\"AMB\" id=\"339\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"346\" name=\"COM\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"4\" flags=\"AudioTrack,OrderSet\" color=\"2632307967\"/>  \n\
      <Controllable name=\"solo\" id=\"354\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"360\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"362\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"COM\" id=\"374\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"COM/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"COM\" id=\"375\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"COM/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
          <Connection other=\"PAD 1/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"COM/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
          <Connection other=\"PAD 2/audio_in 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"356\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"358\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"14754\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"14760\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"14762\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"14756\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"14782\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"14784\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"14758\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"14786\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"365\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"369\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"367\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"371\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"373\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"14765\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"14767\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"14769\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"14771\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"14773\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"377\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"14752\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"352\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"376\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"14750\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"350\" flags=\"GainLike\" value=\"0.99999994039535522\"/>  \n\
      </Processor>  \n\
      <Processor id=\"378\" name=\"meter-Audio 5\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"379\" name=\"COM\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"COM\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"365\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"369\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"367\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"371\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"373\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"14765\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"14767\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"14769\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"14771\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"14773\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"14787\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"14785\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"14783\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"COM\" name=\"COM\" id=\"387\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"394\" name=\"Audio 6\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"5\" flags=\"AudioTrack,OrderSet\" color=\"3856104703\"/>  \n\
      <Controllable name=\"solo\" id=\"402\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"408\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"410\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Audio 6\" id=\"422\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 6/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"Audio 6\" id=\"423\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 6/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
          <Connection other=\"PAD 1/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Audio 6/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
          <Connection other=\"PAD 2/audio_in 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"404\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"406\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"14799\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"14805\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"14807\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"14801\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"14827\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"14829\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"14803\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"14831\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"413\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"417\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"415\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"419\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"421\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"14810\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"14812\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"14814\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"14816\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"14818\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"425\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"14797\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"400\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"424\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"14795\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"398\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"426\" name=\"meter-Audio 6\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"427\" name=\"Audio 6\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 6\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"413\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"417\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"415\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"419\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"421\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"14810\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"14812\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"14814\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"14816\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"14818\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"14832\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"14830\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"14828\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 6.1\" name=\"Audio 6\" id=\"435\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"442\" name=\"Audio 7\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"6\" flags=\"AudioTrack,OrderSet\" color=\"3850531583\"/>  \n\
      <Controllable name=\"solo\" id=\"450\" flags=\"Toggle,RealTime\" value=\"1\" self-solo=\"1\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"456\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"458\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Audio 7\" id=\"470\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 7/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"Audio 7\" id=\"471\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 7/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
          <Connection other=\"PAD 1/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Audio 7/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
          <Connection other=\"PAD 2/audio_in 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"452\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"454\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"14844\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"14850\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"14852\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"14846\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"14872\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"14874\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"14848\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"14876\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"461\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"465\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"463\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"467\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"469\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"14855\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"14857\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"14859\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"14861\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"14863\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"473\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"14842\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"448\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"472\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"14840\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"446\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"474\" name=\"meter-Audio 7\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"475\" name=\"Audio 7\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 7\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"461\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"465\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"463\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"467\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"469\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"14855\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"14857\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"14859\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"14861\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"14863\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"14877\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"14875\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"14873\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 7.1\" name=\"Audio 7\" id=\"483\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"490\" name=\"Audio 8\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"7\" flags=\"AudioTrack,OrderSet\" color=\"3806697215\"/>  \n\
      <Controllable name=\"solo\" id=\"498\" flags=\"Toggle,RealTime\" value=\"1\" self-solo=\"1\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"504\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"506\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Audio 8\" id=\"518\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 8/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"Audio 8\" id=\"519\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 8/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
          <Connection other=\"PAD 1/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Audio 8/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
          <Connection other=\"PAD 2/audio_in 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"500\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"502\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"14889\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"14895\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"14897\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"14891\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"14917\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"14919\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"14893\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"14921\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"509\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"513\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"511\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"515\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"517\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"14900\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"14902\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"14904\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"14906\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"14908\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"521\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"14887\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"496\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"520\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"14885\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"494\" flags=\"GainLike\" value=\"0.99999994039535522\"/>  \n\
      </Processor>  \n\
      <Processor id=\"522\" name=\"meter-Audio 8\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"523\" name=\"Audio 8\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 8\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"509\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"513\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"511\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"515\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"517\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"14900\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"14902\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"14904\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"14906\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"14908\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"14922\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"14920\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"14918\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 8.1\" name=\"Audio 8\" id=\"531\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"538\" name=\"Audio 9\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"8\" flags=\"AudioTrack,OrderSet\" color=\"3769889535\"/>  \n\
      <Controllable name=\"solo\" id=\"546\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"552\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"554\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Audio 9\" id=\"566\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 9/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"Audio 9\" id=\"567\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 9/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
          <Connection other=\"PAD 1/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Audio 9/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
          <Connection other=\"PAD 2/audio_in 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"548\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"550\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"14934\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"14940\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"14942\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"14936\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"14962\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"14964\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"14938\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"14966\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"557\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"561\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"559\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"563\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"565\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"14945\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"14947\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"14949\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"14951\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"14953\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"569\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"14932\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"544\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"568\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"14930\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"542\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"570\" name=\"meter-Audio 9\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"571\" name=\"Audio 9\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 9\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"557\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"561\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"559\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"563\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"565\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"14945\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"14947\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"14949\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"14951\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"14953\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"14967\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"14965\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"14963\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 9.1\" name=\"Audio 9\" id=\"579\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"586\" name=\"Audio 10\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"9\" flags=\"AudioTrack,OrderSet\" color=\"3850286079\"/>  \n\
      <Controllable name=\"solo\" id=\"594\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"600\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"602\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Audio 10\" id=\"614\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 10/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"Audio 10\" id=\"615\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 10/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
          <Connection other=\"PAD 1/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Audio 10/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
          <Connection other=\"PAD 2/audio_in 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"596\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"598\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"14979\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"14985\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"14987\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"14981\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"15007\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"15009\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"14983\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"15011\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"605\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"609\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"607\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"611\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"613\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"14990\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"14992\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"14994\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"14996\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"14998\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"617\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"14977\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"592\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"616\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"14975\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"590\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"618\" name=\"meter-Audio 10\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"619\" name=\"Audio 10\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 10\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"605\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"609\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"607\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"611\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"613\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"14990\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"14992\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"14994\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"14996\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"14998\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"15012\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"15010\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"15008\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 10.1\" name=\"Audio 10\" id=\"627\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"634\" name=\"Audio 11\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"10\" flags=\"AudioTrack,OrderSet\" color=\"1927648767\"/>  \n\
      <Controllable name=\"solo\" id=\"642\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"648\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"650\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Audio 11\" id=\"662\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 11/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"Audio 11\" id=\"663\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 11/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
          <Connection other=\"PAD 1/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Audio 11/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
          <Connection other=\"PAD 2/audio_in 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"644\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"646\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15024\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15030\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15032\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15026\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"15052\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"15054\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15028\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"15056\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"653\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"657\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"655\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"659\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"661\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15035\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15037\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15039\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15041\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15043\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"665\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15022\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"640\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"664\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15020\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"638\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"666\" name=\"meter-Audio 11\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"667\" name=\"Audio 11\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 11\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"653\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"657\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"655\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"659\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"661\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15035\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15037\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15039\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15041\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15043\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"15057\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"15055\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"15053\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 11.1\" name=\"Audio 11\" id=\"675\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"682\" name=\"Audio 12\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"11\" flags=\"AudioTrack,OrderSet\" color=\"3538259711\"/>  \n\
      <Controllable name=\"solo\" id=\"690\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"696\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"698\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Audio 12\" id=\"710\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 12/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"Audio 12\" id=\"711\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 12/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
          <Connection other=\"PAD 1/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Audio 12/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
          <Connection other=\"PAD 2/audio_in 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"692\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"694\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15069\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15075\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15077\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15071\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"15097\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"15099\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15073\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"15101\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"701\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"705\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"703\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"707\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"709\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15080\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15082\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15084\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15086\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15088\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"713\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15067\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"688\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"712\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15065\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"686\" flags=\"GainLike\" value=\"1.1230720281600952\"/>  \n\
      </Processor>  \n\
      <Processor id=\"714\" name=\"meter-Audio 12\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"715\" name=\"Audio 12\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 12\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"701\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"705\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"703\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"707\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"709\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15080\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15082\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15084\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15086\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15088\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"15102\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"15100\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"15098\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 12.1\" name=\"Audio 12\" id=\"723\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"730\" name=\"Audio 13\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"12\" flags=\"AudioTrack,OrderSet\" color=\"3933585407\"/>  \n\
      <Controllable name=\"solo\" id=\"738\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"744\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"746\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Audio 13\" id=\"758\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 13/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"Audio 13\" id=\"759\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 13/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
          <Connection other=\"PAD 1/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Audio 13/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
          <Connection other=\"PAD 2/audio_in 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"740\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"742\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15114\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15120\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15122\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15116\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"15142\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"15144\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15118\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"15146\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"749\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"753\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"751\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"755\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"757\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15125\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15127\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15129\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15131\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15133\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"761\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15112\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"736\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"760\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15110\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"734\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"762\" name=\"meter-Audio 13\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"763\" name=\"Audio 13\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 13\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"749\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"753\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"751\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"755\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"757\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15125\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15127\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15129\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15131\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15133\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"15147\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"15145\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"15143\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 13.1\" name=\"Audio 13\" id=\"771\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"778\" name=\"Audio 14\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"13\" flags=\"AudioTrack,OrderSet\" color=\"3654477311\"/>  \n\
      <Controllable name=\"solo\" id=\"786\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"792\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"794\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Audio 14\" id=\"806\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 14/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"Audio 14\" id=\"807\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 14/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
          <Connection other=\"PAD 1/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Audio 14/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
          <Connection other=\"PAD 2/audio_in 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"788\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"790\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15159\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15165\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15167\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15161\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"15187\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"15189\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15163\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"15191\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"797\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"801\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"799\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"803\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"805\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15170\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15172\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15174\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15176\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15178\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"809\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15157\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"784\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"808\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15155\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"782\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"810\" name=\"meter-Audio 14\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"811\" name=\"Audio 14\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 14\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"797\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"801\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"799\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"803\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"805\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15170\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15172\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15174\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15176\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15178\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"15192\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"15190\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"15188\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 14.1\" name=\"Audio 14\" id=\"819\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"826\" name=\"Audio 15\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"14\" flags=\"AudioTrack,OrderSet\" color=\"3849494271\"/>  \n\
      <Controllable name=\"solo\" id=\"834\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"840\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"842\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Audio 15\" id=\"854\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 15/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"Audio 15\" id=\"855\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 15/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
          <Connection other=\"PAD 1/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Audio 15/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
          <Connection other=\"PAD 2/audio_in 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"836\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"838\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15204\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15210\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15212\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15206\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"15232\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"15234\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15208\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"15236\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"845\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"849\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"847\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"851\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"853\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15215\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15217\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15219\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15221\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15223\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"857\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15202\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"832\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"856\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15200\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"830\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"858\" name=\"meter-Audio 15\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"859\" name=\"Audio 15\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 15\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"845\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"849\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"847\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"851\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"853\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15215\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15217\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15219\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15221\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15223\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"15237\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"15235\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"15233\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 15.1\" name=\"Audio 15\" id=\"867\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"874\" name=\"Audio 16\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"15\" flags=\"AudioTrack,OrderSet\" color=\"3756385279\"/>  \n\
      <Controllable name=\"solo\" id=\"882\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"888\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"890\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Audio 16\" id=\"902\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 16/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"Audio 16\" id=\"903\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 16/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
          <Connection other=\"PAD 1/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Audio 16/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
          <Connection other=\"PAD 2/audio_in 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"884\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"886\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15249\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15255\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15257\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15251\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"15277\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"15279\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15253\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"15281\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"893\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"897\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"895\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"899\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"901\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15260\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15262\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15264\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15266\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15268\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"905\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15247\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"880\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"904\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15245\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"878\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"906\" name=\"meter-Audio 16\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"907\" name=\"Audio 16\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 16\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"893\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"897\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"895\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"899\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"901\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15260\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15262\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15264\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15266\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15268\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"15282\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"15280\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"15278\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 16.1\" name=\"Audio 16\" id=\"915\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"922\" name=\"Audio 17\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"16\" flags=\"AudioTrack,OrderSet\" color=\"4074142207\"/>  \n\
      <Controllable name=\"solo\" id=\"930\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"936\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"938\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Audio 17\" id=\"950\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 17/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"Audio 17\" id=\"951\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 17/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Audio 17/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"932\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"934\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15294\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15300\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15302\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15296\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"15322\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"15324\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15298\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"15326\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"941\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"945\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"943\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"947\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"949\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15305\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15307\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15309\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15311\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15313\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"953\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15292\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"928\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"952\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15290\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"926\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"954\" name=\"meter-Audio 17\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"955\" name=\"Audio 17\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 17\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"941\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"945\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"943\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"947\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"949\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15305\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15307\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15309\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15311\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15313\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"15327\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"15325\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"15323\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 17.1\" name=\"Audio 17\" id=\"963\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"970\" name=\"Audio 18\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"17\" flags=\"AudioTrack,OrderSet\" color=\"2061857535\"/>  \n\
      <Controllable name=\"solo\" id=\"978\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"984\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"986\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Audio 18\" id=\"998\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 18/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"Audio 18\" id=\"999\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 18/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Audio 18/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"980\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"982\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15339\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15345\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15347\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15341\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"15367\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"15369\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15343\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"15371\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"989\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"993\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"991\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"995\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"997\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15350\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15352\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15354\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15356\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15358\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"1001\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15337\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"976\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"1000\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15335\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"974\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"1002\" name=\"meter-Audio 18\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"1003\" name=\"Audio 18\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 18\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"989\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"993\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"991\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"995\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"997\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15350\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15352\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15354\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15356\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15358\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"15372\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"15370\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"15368\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 18.1\" name=\"Audio 18\" id=\"1011\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"1018\" name=\"Audio 19\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"18\" flags=\"AudioTrack,OrderSet\" color=\"3856513023\"/>  \n\
      <Controllable name=\"solo\" id=\"1026\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"1032\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"1034\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Audio 19\" id=\"1046\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 19/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"Audio 19\" id=\"1047\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 19/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Audio 19/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"1028\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"1030\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15384\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15390\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15392\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15386\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"15412\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"15414\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15388\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"15416\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"1037\" flags=\"\" value=\"1\"/>  \n\
        <Controllable name=\"pan-width\" id=\"1041\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"1039\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"1043\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"1045\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15395\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15397\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15399\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15401\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15403\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"1049\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15382\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"1024\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"1048\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15380\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"1022\" flags=\"GainLike\" value=\"1.2561534643173218\"/>  \n\
      </Processor>  \n\
      <Processor id=\"1050\" name=\"meter-Audio 19\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"1051\" name=\"Audio 19\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 19\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"1037\" flags=\"\" value=\"1\"/>  \n\
          <Controllable name=\"pan-width\" id=\"1041\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"1039\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"1043\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"1045\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15395\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15397\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15399\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15401\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15403\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"15417\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"15415\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"15413\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 19.1\" name=\"Audio 19\" id=\"1059\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"1066\" name=\"Audio 20\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"19\" flags=\"AudioTrack,OrderSet\" color=\"3202724351\"/>  \n\
      <Controllable name=\"solo\" id=\"1074\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"1080\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"1082\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Audio 20\" id=\"1094\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 20/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"Audio 20\" id=\"1095\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Audio 20/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Audio 20/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"1076\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"1078\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15429\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15435\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15437\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15431\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"15457\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"15459\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15433\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"15461\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"1085\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"1089\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"1087\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"1091\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"1093\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15440\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15442\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15444\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15446\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15448\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"1097\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15427\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"1072\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"1096\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15425\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"1070\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"1098\" name=\"meter-Audio 20\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"1099\" name=\"Audio 20\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Audio 20\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"1085\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"1089\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"1087\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"1091\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"1093\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15440\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15442\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15444\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15446\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15448\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"15462\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"15460\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"15458\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"Audio 20.1\" name=\"Audio 20\" id=\"1107\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"1874\" name=\"PAD 1\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterInput\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"20\" flags=\"AudioTrack,OrderSet\" color=\"3850859263\"/>  \n\
      <Controllable name=\"solo\" id=\"1882\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"2\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"1888\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"1890\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"PAD 1\" id=\"1902\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"PAD 1/audio_in 1\">  \n\
          <Connection other=\"AMB 1/audio_out 1\"/>  \n\
          <Connection other=\"AMB 2/audio_out 1\"/>  \n\
          <Connection other=\"AMB/audio_out 1\"/>  \n\
          <Connection other=\"Audio 10/audio_out 1\"/>  \n\
          <Connection other=\"Audio 11/audio_out 1\"/>  \n\
          <Connection other=\"Audio 12/audio_out 1\"/>  \n\
          <Connection other=\"Audio 13/audio_out 1\"/>  \n\
          <Connection other=\"Audio 14/audio_out 1\"/>  \n\
          <Connection other=\"Audio 15/audio_out 1\"/>  \n\
          <Connection other=\"Audio 16/audio_out 1\"/>  \n\
          <Connection other=\"Audio 6/audio_out 1\"/>  \n\
          <Connection other=\"Audio 7/audio_out 1\"/>  \n\
          <Connection other=\"Audio 8/audio_out 1\"/>  \n\
          <Connection other=\"Audio 9/audio_out 1\"/>  \n\
          <Connection other=\"COM/audio_out 1\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <IO name=\"PAD 1\" id=\"1903\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"PAD 1/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"PAD 1/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"1884\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"1886\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15474\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15480\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15482\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15476\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"15502\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"15504\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15478\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"15506\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"1893\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-width\" id=\"1897\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"1895\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"1899\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"1901\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15485\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15487\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15489\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15491\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15493\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"1905\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15472\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"1880\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"1906\" name=\"meter-Audio 1\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"1904\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"8597\" interpolation-style=\"Linear\" state=\"Touch\">  \n\
            <events>0 0.999999940395355  \n\
173454512 0.999999940395355  \n\
173456560 0.97156465053558305  \n\
173458608 0.87639498710632302  \n\
173460656 0.85042184591293302  \n\
173461680 0.81245398521423295  \n\
173463728 0.77565801143646196  \n\
173465776 0.70549911260604903  \n\
173467824 0.68310779333114602  \n\
173468848 0.65043288469314597  \n\
173470896 0.61883461475372303  \n\
173472944 0.56850874423980702  \n\
173474992 0.53029936552047696  \n\
173476016 0.49386262893676802  \n\
173478064 0.45915240049362199  \n\
173479088 0.42612272500991799  \n\
173481136 0.41022387146949801  \n\
173483184 0.37222808599472001  \n\
173485232 0.35060352087020902  \n\
173490352 0.24928979575634  \n\
173493424 0.21191510558128401  \n\
173495472 0.187622174620628  \n\
173497520 0.14865350723266599  \n\
173503664 0.0818610489368439  \n\
173506736 0.066581167280673995  \n\
173507760 0.059684809297323199  \n\
173513904 0.053262449800968198  \n\
173655216 0.053262449800968198  \n\
</events>  \n\
          </AutomationList>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"1878\" flags=\"GainLike\" value=\"0.999999940395355\"/>  \n\
      </Processor>  \n\
      <Processor id=\"1907\" name=\"PAD 1\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"PAD 1\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"1893\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-width\" id=\"1897\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"1895\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"1899\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"1901\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15485\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15487\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15489\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15491\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15493\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"15507\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"15505\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"15503\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"PAD 1\" name=\"PAD 1\" id=\"1915\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"1922\" name=\"PAD 2\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterInput\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"21\" flags=\"AudioTrack,OrderSet\" color=\"1927645695\"/>  \n\
      <Controllable name=\"solo\" id=\"1930\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"2\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"1936\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"1938\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"PAD 2\" id=\"1950\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"PAD 2/audio_in 1\">  \n\
          <Connection other=\"AMB 1/audio_out 2\"/>  \n\
          <Connection other=\"AMB 2/audio_out 2\"/>  \n\
          <Connection other=\"AMB/audio_out 2\"/>  \n\
          <Connection other=\"Audio 10/audio_out 2\"/>  \n\
          <Connection other=\"Audio 11/audio_out 2\"/>  \n\
          <Connection other=\"Audio 12/audio_out 2\"/>  \n\
          <Connection other=\"Audio 13/audio_out 2\"/>  \n\
          <Connection other=\"Audio 14/audio_out 2\"/>  \n\
          <Connection other=\"Audio 15/audio_out 2\"/>  \n\
          <Connection other=\"Audio 16/audio_out 2\"/>  \n\
          <Connection other=\"Audio 6/audio_out 2\"/>  \n\
          <Connection other=\"Audio 7/audio_out 2\"/>  \n\
          <Connection other=\"Audio 8/audio_out 2\"/>  \n\
          <Connection other=\"Audio 9/audio_out 2\"/>  \n\
          <Connection other=\"COM/audio_out 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <IO name=\"PAD 2\" id=\"1951\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"PAD 2/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"PAD 2/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"1932\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"1934\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15519\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15525\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15527\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15521\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"15547\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"15549\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15523\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"15551\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"1941\" flags=\"\" value=\"1\"/>  \n\
        <Controllable name=\"pan-width\" id=\"1945\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"1943\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"1947\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"1949\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15530\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15532\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15534\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15536\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15538\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"1953\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15517\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"1928\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"1954\" name=\"meter-Audio 2\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"1952\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15515\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"1926\" flags=\"GainLike\" value=\"0.99999994039535522\"/>  \n\
      </Processor>  \n\
      <Processor id=\"1955\" name=\"PAD 2\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"PAD 2\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"1941\" flags=\"\" value=\"1\"/>  \n\
          <Controllable name=\"pan-width\" id=\"1945\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"1943\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"1947\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"1949\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15530\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15532\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15534\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15536\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15538\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"15552\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"15550\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"15548\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"PAD 2\" name=\"PAD 2\" id=\"1963\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"1970\" name=\"PAD 3\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterInput\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"22\" flags=\"AudioTrack,OrderSet\" color=\"3463636479\"/>  \n\
      <Controllable name=\"solo\" id=\"1978\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"1984\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"1986\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"PAD 3\" id=\"1998\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"PAD 3/audio_in 1\"/>  \n\
      </IO>  \n\
      <IO name=\"PAD 3\" id=\"1999\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"PAD 3/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"PAD 3/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"1\"/>  \n\
      <Controllable name=\"mute\" id=\"1980\" flags=\"Toggle,RealTime\" value=\"1\"/>  \n\
      <Controllable name=\"phase\" id=\"1982\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15564\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15570\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15572\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15566\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"15592\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"15594\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15568\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"15596\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"1989\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"1993\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"1991\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"1995\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"1997\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15575\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15577\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15579\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15581\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15583\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"2001\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15562\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"1976\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"2002\" name=\"meter-Audio 3\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"2000\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15560\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"1974\" flags=\"GainLike\" value=\"0.99999994039535522\"/>  \n\
      </Processor>  \n\
      <Processor id=\"2003\" name=\"PAD 3\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"PAD 3\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"1989\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"1993\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"1991\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"1995\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"1997\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15575\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15577\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15579\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15581\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15583\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"15597\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"15595\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"15593\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"PAD 3\" name=\"PAD 3\" id=\"2011\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"2018\" name=\"PAD 4\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterInput\" meter-type=\"MeterPeak\" saved-meter-point=\"MeterPostFader\" mode=\"Normal\">  \n\
      <PresentationInfo order=\"23\" flags=\"AudioTrack,OrderSet\" color=\"2380624639\"/>  \n\
      <Controllable name=\"solo\" id=\"2026\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"2032\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"2034\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"PAD 4\" id=\"2046\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"PAD 4/audio_in 1\">  \n\
          <Connection other=\"AMB 1/audio_out 1\"/>  \n\
          <Connection other=\"AMB 1/audio_out 2\"/>  \n\
          <Connection other=\"AMB 2/audio_out 1\"/>  \n\
          <Connection other=\"AMB 2/audio_out 2\"/>  \n\
          <Connection other=\"AMB/audio_out 1\"/>  \n\
          <Connection other=\"AMB/audio_out 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <IO name=\"PAD 4\" id=\"2047\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"PAD 4/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"PAD 4/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"1\"/>  \n\
      <Controllable name=\"mute\" id=\"2028\" flags=\"Toggle,RealTime\" value=\"1\"/>  \n\
      <Controllable name=\"phase\" id=\"2030\" flags=\"Toggle\" value=\"0\" phase-invert=\"0\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15609\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15615\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15617\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15611\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-enable\" id=\"15637\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"rec-safe\" id=\"15639\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15613\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"monitor\" id=\"15641\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"2037\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"2041\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"2039\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"2043\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"2045\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15620\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15622\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15624\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15626\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15628\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"2049\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15607\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"2024\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"2050\" name=\"meter-Audio 4\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"2048\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15605\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"2022\" flags=\"GainLike\" value=\"0.99999994039535522\"/>  \n\
      </Processor>  \n\
      <Processor id=\"2051\" name=\"PAD 4\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"PAD 4\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"2037\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"2041\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"2039\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"2043\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"2045\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15620\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15622\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15624\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15626\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15628\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
      <Controllable name=\"monitor\" id=\"15642\" flags=\"RealTime\" value=\"0\" monitoring=\"\"/>  \n\
      <Controllable name=\"rec-safe\" id=\"15640\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"rec-enable\" id=\"15638\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Diskstream flags=\"Recordable\" playlist=\"PAD 4\" name=\"PAD 4\" id=\"2059\" speed=\"1\" capture-alignment=\"Automatic\" record-safe=\"0\" channels=\"1\"/>  \n\
    </Route>  \n\
    <Route id=\"6010\" name=\"ITW 1\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak0dB\">  \n\
      <PresentationInfo order=\"24\" flags=\"AudioBus,OrderSet\" color=\"4222009087\"/>  \n\
      <Controllable name=\"solo\" id=\"6018\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"6024\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"6026\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"ITW 1\" id=\"6038\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"ITW 1/audio_in 1\"/>  \n\
        <Port type=\"audio\" name=\"ITW 1/audio_in 2\"/>  \n\
      </IO>  \n\
      <IO name=\"ITW 1\" id=\"6039\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"ITW 1/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"ITW 1/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"1\"/>  \n\
      <Controllable name=\"mute\" id=\"6020\" flags=\"Toggle,RealTime\" value=\"1\"/>  \n\
      <Controllable name=\"phase\" id=\"6022\" flags=\"Toggle\" value=\"0\" phase-invert=\"00\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15654\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15660\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15662\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15656\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15658\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"6029\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"6033\" flags=\"\" value=\"1\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"6031\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"6035\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"6037\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15665\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15667\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15669\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15671\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15673\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"6046\" name=\"return 2\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"1\" type=\"intreturn\" bitslot=\"1\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15683\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Processor>  \n\
      <Processor id=\"6041\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15652\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"6016\" flags=\"GainLike\" value=\"1.4125370979309082\"/>  \n\
      </Processor>  \n\
      <Processor id=\"6040\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15650\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"6014\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"6042\" name=\"meter-Bus 1\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"6043\" name=\"ITW 1\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"ITW 1\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"6029\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"6033\" flags=\"\" value=\"1\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"6031\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"6035\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"6037\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15665\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15667\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15669\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15671\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15673\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
    </Route>  \n\
    <Route id=\"6052\" name=\"AMB 3\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak0dB\">  \n\
      <PresentationInfo order=\"25\" flags=\"AudioBus,OrderSet\" color=\"2732956927\"/>  \n\
      <Controllable name=\"solo\" id=\"6060\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"6066\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"6068\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"AMB 3\" id=\"6080\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"AMB 3/audio_in 1\"/>  \n\
        <Port type=\"audio\" name=\"AMB 3/audio_in 2\"/>  \n\
      </IO>  \n\
      <IO name=\"AMB 3\" id=\"6081\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"AMB 3/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"AMB 3/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"1\"/>  \n\
      <Controllable name=\"mute\" id=\"6062\" flags=\"Toggle,RealTime\" value=\"1\"/>  \n\
      <Controllable name=\"phase\" id=\"6064\" flags=\"Toggle\" value=\"0\" phase-invert=\"00\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15695\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15701\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15703\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15697\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15699\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"6071\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"6075\" flags=\"\" value=\"1\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"6073\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"6077\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"6079\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15706\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15708\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15710\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15712\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15714\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"6088\" name=\"return 3\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"1\" type=\"intreturn\" bitslot=\"2\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15724\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Processor>  \n\
      <Processor id=\"6083\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15693\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"6058\" flags=\"GainLike\" value=\"1.4125370979309082\"/>  \n\
      </Processor>  \n\
      <Processor id=\"6082\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15691\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"6056\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"6084\" name=\"meter-Bus 2\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"6085\" name=\"AMB 3\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"AMB 3\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"6071\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"6075\" flags=\"\" value=\"1\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"6073\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"6077\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"6079\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15706\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15708\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15710\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15712\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15714\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
    </Route>  \n\
    <Route id=\"6094\" name=\"Bus 3\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak0dB\">  \n\
      <PresentationInfo order=\"26\" flags=\"AudioBus,OrderSet\" color=\"1921902079\"/>  \n\
      <Controllable name=\"solo\" id=\"6102\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"6108\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"6110\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Bus 3\" id=\"6122\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Bus 3/audio_in 1\"/>  \n\
        <Port type=\"audio\" name=\"Bus 3/audio_in 2\"/>  \n\
      </IO>  \n\
      <IO name=\"Bus 3\" id=\"6123\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Bus 3/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Bus 3/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"6104\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"6106\" flags=\"Toggle\" value=\"0\" phase-invert=\"00\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15736\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15742\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15744\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15738\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15740\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"6113\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"6117\" flags=\"\" value=\"1\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"6115\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"6119\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"6121\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15747\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15749\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15751\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15753\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15755\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"6130\" name=\"return 4\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"1\" type=\"intreturn\" bitslot=\"3\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15765\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Processor>  \n\
      <Processor id=\"6125\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15734\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"6100\" flags=\"GainLike\" value=\"1.4125370979309082\"/>  \n\
      </Processor>  \n\
      <Processor id=\"6124\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15732\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"6098\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"6126\" name=\"meter-Bus 3\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"6127\" name=\"Bus 3\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Bus 3\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"6113\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"6117\" flags=\"\" value=\"1\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"6115\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"6119\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"6121\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15747\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15749\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15751\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15753\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15755\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
    </Route>  \n\
    <Route id=\"6136\" name=\"Bus 4\" default-type=\"audio\" strict-io=\"1\" active=\"1\" denormal-protection=\"0\" meter-point=\"MeterPostFader\" meter-type=\"MeterPeak\">  \n\
      <PresentationInfo order=\"27\" flags=\"AudioBus,OrderSet\" color=\"1927538175\"/>  \n\
      <Controllable name=\"solo\" id=\"6144\" flags=\"Toggle,RealTime\" value=\"0\" self-solo=\"0\" soloed-by-upstream=\"0\" soloed-by-downstream=\"0\"/>  \n\
      <Controllable name=\"solo-iso\" id=\"6150\" flags=\"Toggle,RealTime\" value=\"0\" solo-isolated=\"0\"/>  \n\
      <Controllable name=\"solo-safe\" id=\"6152\" flags=\"Toggle\" value=\"0\" solo-safe=\"0\"/>  \n\
      <IO name=\"Bus 4\" id=\"6164\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Bus 4/audio_in 1\"/>  \n\
        <Port type=\"audio\" name=\"Bus 4/audio_in 2\"/>  \n\
      </IO>  \n\
      <IO name=\"Bus 4\" id=\"6165\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
        <Port type=\"audio\" name=\"Bus 4/audio_out 1\">  \n\
          <Connection other=\"Master/audio_in 1\"/>  \n\
        </Port>  \n\
        <Port type=\"audio\" name=\"Bus 4/audio_out 2\">  \n\
          <Connection other=\"Master/audio_in 2\"/>  \n\
        </Port>  \n\
      </IO>  \n\
      <MuteMaster mute-point=\"PostFader,Listen,Main\" muted=\"0\"/>  \n\
      <Controllable name=\"mute\" id=\"6146\" flags=\"Toggle,RealTime\" value=\"0\"/>  \n\
      <Controllable name=\"phase\" id=\"6148\" flags=\"Toggle\" value=\"0\" phase-invert=\"00\"/>  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"solo\" id=\"15777\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-iso\" id=\"15783\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"solo-safe\" id=\"15785\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"mute\" id=\"15779\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
        <AutomationList automation-id=\"phase\" id=\"15781\" interpolation-style=\"Discrete\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Pannable>  \n\
        <Controllable name=\"pan-azimuth\" id=\"6155\" flags=\"\" value=\"0.5\"/>  \n\
        <Controllable name=\"pan-width\" id=\"6159\" flags=\"\" value=\"1\"/>  \n\
        <Controllable name=\"pan-elevation\" id=\"6157\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-frontback\" id=\"6161\" flags=\"\" value=\"0\"/>  \n\
        <Controllable name=\"pan-lfe\" id=\"6163\" flags=\"\" value=\"0\"/>  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"pan-azimuth\" id=\"15788\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-elevation\" id=\"15790\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-width\" id=\"15792\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-frontback\" id=\"15794\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          <AutomationList automation-id=\"pan-lfe\" id=\"15796\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Pannable>  \n\
      <Processor id=\"6172\" name=\"return 5\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"1\" type=\"intreturn\" bitslot=\"4\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15806\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
      </Processor>  \n\
      <Processor id=\"6167\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"trim\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"trim\" id=\"15775\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"trimcontrol\" id=\"6142\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"6166\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
        <Automation>  \n\
          <AutomationList automation-id=\"gain\" id=\"15773\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
        </Automation>  \n\
        <Controllable name=\"gaincontrol\" id=\"6140\" flags=\"GainLike\" value=\"1\"/>  \n\
      </Processor>  \n\
      <Processor id=\"6168\" name=\"meter-Bus 4\" active=\"1\" user-latency=\"0\" type=\"meter\"/>  \n\
      <Processor id=\"6169\" name=\"Bus 4\" active=\"1\" user-latency=\"0\" own-input=\"1\" own-output=\"0\" output=\"Bus 4\" type=\"main-outs\" role=\"Main\">  \n\
        <PannerShell bypassed=\"0\" user-panner=\"\" linked-to-route=\"1\"/>  \n\
        <Pannable>  \n\
          <Controllable name=\"pan-azimuth\" id=\"6155\" flags=\"\" value=\"0.5\"/>  \n\
          <Controllable name=\"pan-width\" id=\"6159\" flags=\"\" value=\"1\"/>  \n\
          <Controllable name=\"pan-elevation\" id=\"6157\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-frontback\" id=\"6161\" flags=\"\" value=\"0\"/>  \n\
          <Controllable name=\"pan-lfe\" id=\"6163\" flags=\"\" value=\"0\"/>  \n\
          <Automation>  \n\
            <AutomationList automation-id=\"pan-azimuth\" id=\"15788\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-elevation\" id=\"15790\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-width\" id=\"15792\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-frontback\" id=\"15794\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
            <AutomationList automation-id=\"pan-lfe\" id=\"15796\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
          </Automation>  \n\
        </Pannable>  \n\
      </Processor>  \n\
      <Slavable/>  \n\
    </Route>  \n\
  </Routes>  \n\
  <Playlists>  \n\
    <Playlist id=\"196\" name=\"ITW\" type=\"audio\" orig-track-id=\"154\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\">  \n\
      <Region name=\"ITW-1.1\" muted=\"0\" opaque=\"1\" locked=\"0\" video-locked=\"0\" automatic=\"0\" whole-file=\"0\" import=\"0\" external=\"0\" sync-marked=\"0\" left-of-split=\"0\" right-of-split=\"0\" hidden=\"0\" position-locked=\"0\" valid-transients=\"0\" start=\"0\" length=\"431104\" position=\"0\" beat=\"0\" sync-position=\"0\" ancestral-start=\"0\" ancestral-length=\"0\" stretch=\"1\" shift=\"1\" positional-lock-style=\"AudioTime\" layering-index=\"0\" envelope-active=\"0\" default-fade-in=\"0\" default-fade-out=\"0\" fade-in-active=\"1\" fade-out-active=\"1\" scale-amplitude=\"1\" id=\"16438\" type=\"audio\" first-edit=\"nothing\" source-0=\"14583\" master-source-0=\"14583\" channels=\"1\">  \n\
        <Envelope default=\"yes\"/>  \n\
        <FadeIn>  \n\
          <AutomationList automation-id=\"fadein\" id=\"16439\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1.0000000116860974e-07  \n\
64 1  \n\
</events>  \n\
          </AutomationList>  \n\
        </FadeIn>  \n\
        <InverseFadeIn>  \n\
          <AutomationList automation-id=\"fadein\" id=\"16440\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1  \n\
64 1.0000000116860974e-07  \n\
</events>  \n\
          </AutomationList>  \n\
        </InverseFadeIn>  \n\
        <FadeOut>  \n\
          <AutomationList automation-id=\"fadeout\" id=\"16441\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1  \n\
64 1.0000000116860974e-07  \n\
</events>  \n\
          </AutomationList>  \n\
        </FadeOut>  \n\
        <InverseFadeOut>  \n\
          <AutomationList automation-id=\"fadeout\" id=\"16442\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1.0000000116860974e-07  \n\
64 1  \n\
</events>  \n\
          </AutomationList>  \n\
        </InverseFadeOut>  \n\
      </Region>  \n\
    </Playlist>  \n\
    <Playlist id=\"244\" name=\"AMB\" type=\"audio\" orig-track-id=\"298\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"292\" name=\"AMB\" type=\"audio\" orig-track-id=\"298\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"340\" name=\"AMB\" type=\"audio\" orig-track-id=\"298\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"388\" name=\"COM\" type=\"audio\" orig-track-id=\"346\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"436\" name=\"Audio 6.1\" type=\"audio\" orig-track-id=\"394\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"484\" name=\"Audio 7.1\" type=\"audio\" orig-track-id=\"442\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"532\" name=\"Audio 8.1\" type=\"audio\" orig-track-id=\"490\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"580\" name=\"Audio 9.1\" type=\"audio\" orig-track-id=\"538\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"628\" name=\"Audio 10.1\" type=\"audio\" orig-track-id=\"586\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"676\" name=\"Audio 11.1\" type=\"audio\" orig-track-id=\"634\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"724\" name=\"Audio 12.1\" type=\"audio\" orig-track-id=\"682\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"772\" name=\"Audio 13.1\" type=\"audio\" orig-track-id=\"730\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"820\" name=\"Audio 14.1\" type=\"audio\" orig-track-id=\"778\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"868\" name=\"Audio 15.1\" type=\"audio\" orig-track-id=\"826\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"916\" name=\"Audio 16.1\" type=\"audio\" orig-track-id=\"874\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"964\" name=\"Audio 17.1\" type=\"audio\" orig-track-id=\"922\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"1012\" name=\"Audio 18.1\" type=\"audio\" orig-track-id=\"970\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"1060\" name=\"Audio 19.1\" type=\"audio\" orig-track-id=\"1018\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"1108\" name=\"Audio 20.1\" type=\"audio\" orig-track-id=\"1066\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\"/>  \n\
    <Playlist id=\"1916\" name=\"PAD 1\" type=\"audio\" orig-track-id=\"1874\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\">  \n\
      <Region name=\"PAD 1-3.1\" muted=\"0\" opaque=\"1\" locked=\"0\" video-locked=\"0\" automatic=\"0\" whole-file=\"0\" import=\"0\" external=\"0\" sync-marked=\"0\" left-of-split=\"0\" right-of-split=\"0\" hidden=\"0\" position-locked=\"0\" valid-transients=\"0\" start=\"0\" length=\"570368\" position=\"173658288\" beat=\"7235.7619999999997\" sync-position=\"0\" ancestral-start=\"0\" ancestral-length=\"0\" stretch=\"1\" shift=\"1\" positional-lock-style=\"AudioTime\" layering-index=\"0\" envelope-active=\"0\" default-fade-in=\"0\" default-fade-out=\"0\" fade-in-active=\"1\" fade-out-active=\"1\" scale-amplitude=\"1\" id=\"14116\" type=\"audio\" first-edit=\"nothing\" source-0=\"13125\" master-source-0=\"13125\" channels=\"1\">  \n\
        <Envelope default=\"yes\"/>  \n\
        <FadeIn>  \n\
          <AutomationList automation-id=\"fadein\" id=\"14117\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1.0000000116860999e-07  \n\
64 1  \n\
</events>  \n\
          </AutomationList>  \n\
        </FadeIn>  \n\
        <InverseFadeIn>  \n\
          <AutomationList automation-id=\"fadein\" id=\"14118\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1  \n\
64 1.0000000116860999e-07  \n\
</events>  \n\
          </AutomationList>  \n\
        </InverseFadeIn>  \n\
        <FadeOut>  \n\
          <AutomationList automation-id=\"fadeout\" id=\"14119\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1  \n\
64 1.0000000116860999e-07  \n\
</events>  \n\
          </AutomationList>  \n\
        </FadeOut>  \n\
        <InverseFadeOut>  \n\
          <AutomationList automation-id=\"fadeout\" id=\"14120\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1.0000000116860999e-07  \n\
64 1  \n\
</events>  \n\
          </AutomationList>  \n\
        </InverseFadeOut>  \n\
      </Region>  \n\
    </Playlist>  \n\
    <Playlist id=\"1964\" name=\"PAD 2\" type=\"audio\" orig-track-id=\"1922\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\">  \n\
      <Region name=\"PAD 2-3.1\" muted=\"0\" opaque=\"1\" locked=\"0\" video-locked=\"0\" automatic=\"0\" whole-file=\"0\" import=\"0\" external=\"0\" sync-marked=\"0\" left-of-split=\"0\" right-of-split=\"0\" hidden=\"0\" position-locked=\"0\" valid-transients=\"0\" start=\"0\" length=\"570368\" position=\"173658288\" beat=\"7235.7619999999997\" sync-position=\"0\" ancestral-start=\"0\" ancestral-length=\"0\" stretch=\"1\" shift=\"1\" positional-lock-style=\"AudioTime\" layering-index=\"0\" envelope-active=\"0\" default-fade-in=\"0\" default-fade-out=\"0\" fade-in-active=\"1\" fade-out-active=\"1\" scale-amplitude=\"1\" id=\"14141\" type=\"audio\" first-edit=\"nothing\" source-0=\"13170\" master-source-0=\"13170\" channels=\"1\">  \n\
        <Envelope default=\"yes\"/>  \n\
        <FadeIn>  \n\
          <AutomationList automation-id=\"fadein\" id=\"14142\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1.0000000116860999e-07  \n\
64 1  \n\
</events>  \n\
          </AutomationList>  \n\
        </FadeIn>  \n\
        <InverseFadeIn>  \n\
          <AutomationList automation-id=\"fadein\" id=\"14143\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1  \n\
64 1.0000000116860999e-07  \n\
</events>  \n\
          </AutomationList>  \n\
        </InverseFadeIn>  \n\
        <FadeOut>  \n\
          <AutomationList automation-id=\"fadeout\" id=\"14144\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1  \n\
64 1.0000000116860999e-07  \n\
</events>  \n\
          </AutomationList>  \n\
        </FadeOut>  \n\
        <InverseFadeOut>  \n\
          <AutomationList automation-id=\"fadeout\" id=\"14145\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1.0000000116860999e-07  \n\
64 1  \n\
</events>  \n\
          </AutomationList>  \n\
        </InverseFadeOut>  \n\
      </Region>  \n\
    </Playlist>  \n\
    <Playlist id=\"2012\" name=\"PAD 3\" type=\"audio\" orig-track-id=\"1970\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\">  \n\
      <Region name=\"PAD 3-3.1\" muted=\"0\" opaque=\"1\" locked=\"0\" video-locked=\"0\" automatic=\"0\" whole-file=\"0\" import=\"0\" external=\"0\" sync-marked=\"0\" left-of-split=\"0\" right-of-split=\"0\" hidden=\"0\" position-locked=\"0\" valid-transients=\"0\" start=\"0\" length=\"570368\" position=\"173658288\" beat=\"7235.7619999999997\" sync-position=\"0\" ancestral-start=\"0\" ancestral-length=\"0\" stretch=\"1\" shift=\"1\" positional-lock-style=\"AudioTime\" layering-index=\"0\" envelope-active=\"0\" default-fade-in=\"0\" default-fade-out=\"0\" fade-in-active=\"1\" fade-out-active=\"1\" scale-amplitude=\"1\" id=\"14068\" type=\"audio\" first-edit=\"nothing\" source-0=\"13215\" master-source-0=\"13215\" channels=\"1\">  \n\
        <Envelope default=\"yes\"/>  \n\
        <FadeIn>  \n\
          <AutomationList automation-id=\"fadein\" id=\"14069\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1.0000000116860999e-07  \n\
64 1  \n\
</events>  \n\
          </AutomationList>  \n\
        </FadeIn>  \n\
        <InverseFadeIn>  \n\
          <AutomationList automation-id=\"fadein\" id=\"14070\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1  \n\
64 1.0000000116860999e-07  \n\
</events>  \n\
          </AutomationList>  \n\
        </InverseFadeIn>  \n\
        <FadeOut>  \n\
          <AutomationList automation-id=\"fadeout\" id=\"14071\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1  \n\
64 1.0000000116860999e-07  \n\
</events>  \n\
          </AutomationList>  \n\
        </FadeOut>  \n\
        <InverseFadeOut>  \n\
          <AutomationList automation-id=\"fadeout\" id=\"14072\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1.0000000116860999e-07  \n\
64 1  \n\
</events>  \n\
          </AutomationList>  \n\
        </InverseFadeOut>  \n\
      </Region>  \n\
    </Playlist>  \n\
    <Playlist id=\"2060\" name=\"PAD 4\" type=\"audio\" orig-track-id=\"2018\" shared-with-ids=\"\" frozen=\"0\" combine-ops=\"0\">  \n\
      <Region name=\"PAD 4-3.1\" muted=\"0\" opaque=\"1\" locked=\"0\" video-locked=\"0\" automatic=\"0\" whole-file=\"0\" import=\"0\" external=\"0\" sync-marked=\"0\" left-of-split=\"0\" right-of-split=\"0\" hidden=\"0\" position-locked=\"0\" valid-transients=\"0\" start=\"0\" length=\"570368\" position=\"173658288\" beat=\"7235.7619999999997\" sync-position=\"0\" ancestral-start=\"0\" ancestral-length=\"0\" stretch=\"1\" shift=\"1\" positional-lock-style=\"AudioTime\" layering-index=\"0\" envelope-active=\"0\" default-fade-in=\"0\" default-fade-out=\"0\" fade-in-active=\"1\" fade-out-active=\"1\" scale-amplitude=\"1\" id=\"14092\" type=\"audio\" first-edit=\"nothing\" source-0=\"13260\" master-source-0=\"13260\" channels=\"1\">  \n\
        <Envelope default=\"yes\"/>  \n\
        <FadeIn>  \n\
          <AutomationList automation-id=\"fadein\" id=\"14093\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1.0000000116860999e-07  \n\
64 1  \n\
</events>  \n\
          </AutomationList>  \n\
        </FadeIn>  \n\
        <InverseFadeIn>  \n\
          <AutomationList automation-id=\"fadein\" id=\"14094\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1  \n\
64 1.0000000116860999e-07  \n\
</events>  \n\
          </AutomationList>  \n\
        </InverseFadeIn>  \n\
        <FadeOut>  \n\
          <AutomationList automation-id=\"fadeout\" id=\"14095\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1  \n\
64 1.0000000116860999e-07  \n\
</events>  \n\
          </AutomationList>  \n\
        </FadeOut>  \n\
        <InverseFadeOut>  \n\
          <AutomationList automation-id=\"fadeout\" id=\"14096\" interpolation-style=\"Curved\" state=\"Off\">  \n\
            <events>0 1.0000000116860999e-07  \n\
64 1  \n\
</events>  \n\
          </AutomationList>  \n\
        </InverseFadeOut>  \n\
      </Region>  \n\
    </Playlist>  \n\
  </Playlists>  \n\
  <UnusedPlaylists/>  \n\
  <RouteGroups/>  \n\
  <Click>  \n\
    <IO name=\"Click\" id=\"48\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
      <Port type=\"audio\" name=\"Click/audio_out 1\">  \n\
        <Connection other=\"system:playback_1\"/>  \n\
      </Port>  \n\
      <Port type=\"audio\" name=\"Click/audio_out 2\">  \n\
        <Connection other=\"system:playback_2\"/>  \n\
      </Port>  \n\
    </IO>  \n\
    <Processor id=\"49\" name=\"Amp\" active=\"1\" user-latency=\"0\" type=\"amp\">  \n\
      <Automation>  \n\
        <AutomationList automation-id=\"gain\" id=\"49\" interpolation-style=\"Linear\" state=\"Off\"/>  \n\
      </Automation>  \n\
      <Controllable name=\"gaincontrol\" id=\"47\" flags=\"GainLike\" value=\"1\"/>  \n\
    </Processor>  \n\
  </Click>  \n\
  <LTC-In>  \n\
    <IO name=\"LTC In\" id=\"47\" direction=\"Input\" default-type=\"audio\" user-latency=\"0\">  \n\
      <Port type=\"audio\" name=\"LTC-in\">  \n\
        <Connection other=\"system:capture_1\"/>  \n\
      </Port>  \n\
    </IO>  \n\
  </LTC-In>  \n\
  <LTC-Out>  \n\
    <IO name=\"LTC Out\" id=\"48\" direction=\"Output\" default-type=\"audio\" user-latency=\"0\">  \n\
      <Port type=\"audio\" name=\"LTC-out\"/>  \n\
    </IO>  \n\
  </LTC-Out>  \n\
  <Speakers>  \n\
    <Speaker azimuth=\"240\" elevation=\"0\" distance=\"1\"/>  \n\
    <Speaker azimuth=\"120\" elevation=\"0\" distance=\"1\"/>  \n\
  </Speakers>  \n\
  <TempoMap>  \n\
    <Tempo pulse=\"0\" frame=\"0\" movable=\"0\" lock-style=\"AudioTime\" beats-per-minute=\"120\" note-type=\"4\" clamped=\"0\" end-beats-per-minute=\"120\" active=\"1\" locked-to-meter=\"1\"/>  \n\
    <Meter pulse=\"0\" frame=\"0\" movable=\"0\" lock-style=\"AudioTime\" bbt=\"1|1|0\" beat=\"0\" note-type=\"4\" divisions-per-bar=\"4\"/>  \n\
  </TempoMap>  \n\
  <ControlProtocols>  \n\
    <Protocol name=\"PreSonus FaderPort8\" active=\"0\"/>  \n\
    <Protocol name=\"Mackie\" active=\"0\"/>  \n\
    <Protocol name=\"Generic MIDI\" active=\"0\"/>  \n\
    <Protocol name=\"Steinberg CC121\" active=\"0\"/>  \n\
    <Protocol name=\"Ableton Push 2\" active=\"0\"/>  \n\
    <Protocol name=\"PreSonus FaderPort\" active=\"0\"/>  \n\
    <Protocol name=\"Open Sound Control (OSC)\" active=\"0\"/>  \n\
  </ControlProtocols>  \n\
  <Extra>  \n\
    <UI>  \n\
      <Window name=\"key-editor\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"session-options-editor\" visible=\"no\" x-off=\"0\" y-off=\"214\" x-size=\"752\" y-size=\"342\"/>  \n\
      <Window name=\"speaker-config\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"about\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"add-routes\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"290\" y-size=\"324\"/>  \n\
      <Window name=\"add-video\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"inspector\" visible=\"no\" x-off=\"0\" y-off=\"0\" x-size=\"620\" y-size=\"370\"/>  \n\
      <Window name=\"audio-midi-setup\" visible=\"no\" x-off=\"398\" y-off=\"185\" x-size=\"569\" y-size=\"398\"/>  \n\
      <Window name=\"video-export\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"script-manager\" visible=\"no\" x-off=\"363\" y-off=\"323\" x-size=\"196\" y-size=\"260\"/>  \n\
      <Window name=\"bundle-manager\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"locations\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"big-clock\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"audio-connection-manager\" visible=\"no\" x-off=\"338\" y-off=\"161\" x-size=\"514\" y-size=\"502\"/>  \n\
      <Window name=\"midi-connection-manager\" visible=\"no\" x-off=\"440\" y-off=\"277\" x-size=\"310\" y-size=\"271\"/>  \n\
      <Window name=\"RPM-154\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-202\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-250\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-298\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-346\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-394\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-442\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-490\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-538\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-586\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-634\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-682\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-730\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-778\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-826\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-874\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-922\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-970\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-1018\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-1066\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-1874\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-1922\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-1970\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-2018\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-6010\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-6052\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-6094\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-6136\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"RPM-51\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-154-185\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-154-5958\" visible=\"no\" x-off=\"717\" y-off=\"419\" x-size=\"628\" y-size=\"179\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-154-184\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-154-6241\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-154-186\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-154-187\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-185-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-5958-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-184-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6241-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-186-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-187-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-202-233\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-202-6285\" visible=\"no\" x-off=\"738\" y-off=\"0\" x-size=\"628\" y-size=\"179\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-202-232\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-202-6263\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-202-234\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-202-235\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-233-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6285-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-232-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6263-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-234-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-235-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-250-281\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-250-280\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-250-282\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-250-283\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-281-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-280-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-282-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-283-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-298-329\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-298-328\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-298-330\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-298-331\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-329-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-328-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-330-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-331-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-346-377\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-346-376\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-346-378\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-346-379\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-377-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-376-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-378-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-379-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-394-425\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-394-424\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-394-426\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-394-427\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-425-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-424-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-426-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-427-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-442-473\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-442-472\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-442-474\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-442-475\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-473-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-472-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-474-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-475-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-490-521\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-490-520\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-490-522\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-490-523\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-521-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-520-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-522-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-523-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-538-569\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-538-568\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-538-570\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-538-571\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-569-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-568-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-570-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-571-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-586-617\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-586-616\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-586-618\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-586-619\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-617-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-616-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-618-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-619-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-634-665\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-634-664\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-634-666\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-634-667\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-665-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-664-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-666-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-667-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-682-713\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-682-712\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-682-714\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-682-715\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-713-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-712-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-714-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-715-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-730-761\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-730-760\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-730-762\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-730-763\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-761-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-760-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-762-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-763-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-778-809\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-778-808\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-778-810\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-778-811\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-809-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-808-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-810-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-811-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-826-857\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-826-856\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-826-858\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-826-859\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-857-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-856-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-858-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-859-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-874-905\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-874-904\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-874-906\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-874-907\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-905-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-904-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-906-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-907-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-922-953\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-922-952\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-922-954\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-922-955\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-953-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-952-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-954-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-955-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-970-1001\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-970-1000\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-970-1002\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-970-1003\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-1001-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-1000-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-1002-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-1003-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-1018-1049\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-1018-1048\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-1018-1050\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-1018-1051\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-1049-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-1048-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-1050-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-1051-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-1066-1097\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-1066-1096\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-1066-1098\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-1066-1099\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-1097-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-1096-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-1098-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-1099-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-1874-1905\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-1874-1906\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-1874-1904\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-1874-1907\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-1905-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-1906-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-1904-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-1907-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-1922-1953\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-1922-1954\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-1922-1952\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-1922-1955\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-1953-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-1954-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-1952-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-1955-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-1970-2001\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-1970-2002\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-1970-2000\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-1970-2003\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-2001-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-2002-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-2000-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-2003-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-2018-2049\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-2018-2050\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-2018-2048\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-2018-2051\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-2049-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-2050-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-2048-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-2051-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-6010-6046\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6010-6041\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6010-6040\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6010-6042\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6010-6043\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-6046-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6041-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6040-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6042-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6043-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-6052-6088\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6052-6083\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6052-6082\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6052-6084\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6052-6085\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-6088-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6083-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6082-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6084-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6085-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-6094-6130\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6094-6125\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6094-6124\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6094-6126\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6094-6127\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-6130-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6125-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6124-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6126-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6127-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-6136-6172\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6136-6167\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6136-6166\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6136-6168\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-6136-6169\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-6172-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6167-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6166-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6168-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-6169-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"P-51-82\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-51-81\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-51-83\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"P-51-84\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\" custom-ui=\"no\"/>  \n\
      <Window name=\"PM-82-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-81-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-83-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <Window name=\"PM-84-\" visible=\"no\" x-off=\"-1\" y-off=\"-1\" x-size=\"-1\" y-size=\"-1\"/>  \n\
      <GUIObjectState>  \n\
        <Object id=\"rtav 51\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 55\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 57\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 51 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 70\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 74\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"strip 51\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 81\"/>  \n\
        </Object>  \n\
        <Object id=\"rtav 154\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 158\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 160\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 154 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 173\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 202\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 206\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 208\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 202 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 221\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 250\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 254\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 256\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 250 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 269\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 298\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 302\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 304\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 298 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 317\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 346\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 350\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 352\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 346 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 365\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 394\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 398\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 400\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 394 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 413\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 442\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 446\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 448\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 442 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 461\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 490\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 494\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 496\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 490 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 509\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 538\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 542\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 544\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 538 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 557\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 586\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 590\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 592\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 586 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 605\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 634\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 638\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 640\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 634 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 653\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 682\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 686\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 688\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 682 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 701\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 730\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 734\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 736\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 730 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 749\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 778\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 782\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 784\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 778 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 797\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 826\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 830\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 832\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 826 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 845\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 874\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 878\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 880\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 874 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 893\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 922\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 926\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 928\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 922 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 941\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 970\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 974\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 976\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 970 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 989\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 1018\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 1022\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1024\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1018 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1037\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 1066\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 1070\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1072\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1066 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1085\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 1114\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 1118\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1120\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1114 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1133\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 1162\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 1166\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1168\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1162 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1181\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 1210\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 1214\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1216\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1210 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1229\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 1258\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 1262\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1264\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1258 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1277\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"strip 154\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 184\"/>  \n\
          <Object id=\"processor 4156\"/>  \n\
          <Object id=\"processor 5958\"/>  \n\
          <Object id=\"processor 5966\">  \n\
            <Object id=\"control 5980\" visible=\"1\"/>  \n\
          </Object>  \n\
          <Object id=\"processor 6241\">  \n\
            <Object id=\"control 6255\" visible=\"1\"/>  \n\
          </Object>  \n\
        </Object>  \n\
        <Object id=\"strip 202\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 232\"/>  \n\
          <Object id=\"processor 5987\">  \n\
            <Object id=\"control 6001\" visible=\"1\"/>  \n\
          </Object>  \n\
          <Object id=\"processor 6263\">  \n\
            <Object id=\"control 6277\" visible=\"1\"/>  \n\
          </Object>  \n\
          <Object id=\"processor 6285\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 250\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 280\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 298\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 328\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 346\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 376\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 394\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 424\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 442\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 472\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 490\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 520\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 538\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 568\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 586\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 616\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 634\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 664\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 682\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 712\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 730\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 760\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 778\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 808\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 826\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 856\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 874\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 904\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 922\" visible=\"1\">  \n\
          <Object id=\"processor 952\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 970\" visible=\"1\">  \n\
          <Object id=\"processor 1000\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 1018\" visible=\"1\">  \n\
          <Object id=\"processor 1048\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 1066\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 1096\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 1114\" visible=\"1\">  \n\
          <Object id=\"processor 1144\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 1162\" visible=\"1\">  \n\
          <Object id=\"processor 1192\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 1210\" visible=\"1\">  \n\
          <Object id=\"processor 1240\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 1258\" visible=\"1\">  \n\
          <Object id=\"processor 1288\"/>  \n\
        </Object>  \n\
        <Object id=\"rtav 1874\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 1878\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 1880\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1874 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1893\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 1922\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 1926\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1928\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1922 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1941\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 1970\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 1974\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1976\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1970 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 1989\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 2018\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 2022\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 2024\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 2018 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 2037\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"strip 1874\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 1904\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 1922\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 1952\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 1970\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 2000\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 2018\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 2048\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 2134\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 2164\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 2176\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 2206\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 2218\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 2248\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 2260\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 2290\"/>  \n\
        </Object>  \n\
        <Object id=\"automation 177\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 6010\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 6014\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6016\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6010 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6029\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6033\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 6052\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 6056\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6058\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6052 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6071\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6075\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 6094\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 6098\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6100\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6094 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6113\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6117\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"rtav 6136\" height=\"66\" visible=\"1\"/>  \n\
        <Object id=\"automation 6140\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6142\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6136 12/0/0\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6155\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"automation 6159\" height=\"66\" visible=\"0\"/>  \n\
        <Object id=\"strip 6010\" strip-width=\"Wide\" visible=\"1\">  \n\
          <Object id=\"processor 6040\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 6052\" visible=\"1\" strip-width=\"Wide\">  \n\
          <Object id=\"processor 6082\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 6094\" visible=\"1\">  \n\
          <Object id=\"processor 6124\"/>  \n\
        </Object>  \n\
        <Object id=\"strip 6136\" strip-width=\"Wide\" visible=\"1\">  \n\
          <Object id=\"processor 6166\"/>  \n\
        </Object>  \n\
      </GUIObjectState>  \n\
    </UI>  \n\
    <Videomonitor active=\"0\"/>  \n\
  </Extra>  \n\
  <Script lua=\"Lua 5.3\">c2NyaXB0cyA9IHt9IA==</Script>  \n\
</Session>";

#endif // __Ardour_SessionFile_h__

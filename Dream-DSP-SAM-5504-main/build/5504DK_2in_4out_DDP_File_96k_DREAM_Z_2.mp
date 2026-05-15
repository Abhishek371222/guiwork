Note: Addresses and sizes for P16 are in words

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
            text     400   31B3     12723 words (CON,REL,ROM)

   Addr Size Module
  ----- ---- ----------------
   00400 0200 libcrtp16.a:crtp16.s@(_TARGET_IC==5???)
   00600 0098 main.o:main.s
   00698 0098 dsp1.o:dsp1.s
   00730 00C1 dsp2.o:dsp2.s
   007F1 0098 dsp3.o:dsp3.s
   00889 00C2 dsp4.o:dsp4.s
   0094B 02E6 midictrl.o:midictrl.s
   00C31 07F4 BiquadCtrl.o:BiquadCtrl.s
   01425 0413 mainbus.o:mainbus.s
   01838 0022 dspDesignerWrappers.o:dspDesignerWrappers.s
   0185A 0015 libsys.a:clearRAM.s@(_TARGET_IC==5???)
   0186F 001C libsys.a:clearRAMEx.s@(_TARGET_IC==5???)
   0188B 0022 libsys.a:DSPresetAll.s@(_TARGET_IC==5???)
   018AD 002A libsys.a:DSPalloc.s@(_TARGET_IC==5???)
   018D7 000C libsys.a:DSProutEx.s@(_TARGET_IC==5???)
   018E3 001A libsys.a:DSProutInEx.s@(_TARGET_IC==5???)
   018FD 0055 libsys.a:DSPGain.s@(_TARGET_IC==5???)
   01952 001D libsys.a:convdB.s@(_TARGET_IC==5???)
   0196F 0038 libsys.a:rom_LoadStartP24.s@_TARGET_IC==5???
   019A7 000C libsys.a:rom_StopP24.s@_TARGET_IC==5???
   019B3 001B libsys.a:rom_DIVIDE.s@_TARGET_IC==5???
   019CE 0016 libsys.a:sys_CalcTimerReload.s@(_TARGET_IC==5???)
   019E4 0008 libsys.a:sys_GetCrystalFreq.s@(_TARGET_IC==5???)
   019EC 000A libsys.a:sys_GetSystemFreq.s@(_TARGET_IC==5???)
   019F6 002A libsys.a:sys_SwitchPLL.s@_TARGET_IC==5???
   01A20 0024 libsys.a:sys_GetSamplingRate.s@(_TARGET_IC==5???)
   01A44 0004 libsys.a:sys_GetFSPinState.s
   01A48 0036 libUSB.a:usb_Dde5xxxIoctl.s@(_TARGET_IC==5???)
   01A7E 0008 libUSB.a:usb_Core.s@(_TARGET_IC==5???)
   01A86 0004 libUSB.a:usb_CoreHS.s@(_TARGET_IC==5???)
   01A8A 0076 libUSB.a:usb_P16Poll.s@(_TARGET_IC==5???)
   01B00 000C libUSB.a:usb_Poll.s@(_TARGET_IC==5???)
   01B0C 028D libUSB.a:usb_Enum.s@(_TARGET_IC==5???)
   01D99 000B libUSB.a:usb_Init.s@(_TARGET_IC==5???)
   01DA4 0013 libUSB.a:usb_Event.s@(_TARGET_IC==5???)
   01DB7 0012 libUSB.a:usb_MidiClass.s@(_TARGET_IC==5???)
   01DC9 0069 libUSB.a:usb_MidiSendWrp.s@(_TARGET_IC==5???)
   01E32 00B0 libUSB.a:usb_DrvMidiClass.s@(_TARGET_IC==5???)
   01EE2 0295 libUSB.a:usb_DrvSam5xxx.s@(_TARGET_IC==5???)
   02177 09D3 libFX5000.a:MixPA.s@(_TARGET_IC==5???)
   02B4A 0099 libFX5000.a:StandardFunctions.s@(_TARGET_IC==5???)
   02BE3 0306 libFX5000.a:MixPA_XW.s@(_TARGET_IC==5???)
   02EE9 005C libcp16.a:5x_fexp.s@(_TARGET_IC==5???)
   02F45 0014 libcp16.a:5x_ftrunc.s@(_TARGET_IC==5???)
   02F59 0034 libcp16.a:5x_fsqrt.s@(_TARGET_IC==5???)
   02F8D 0261 libcp16.a:5x_float.s@(_TARGET_IC==5???)
   031EE 0077 libcp16.a:5x_ftrig.s@(_TARGET_IC==5???)
   03265 0023 libcp16.a:5x_poly.s@(_TARGET_IC==5???)
   03288 0032 libcp16.a:FilterIIR_Base.s@(_TARGET_IC==5???)
   032BA 000F libcp16.a:Filter_1stDenom.s@(_TARGET_IC==5???)
   032C9 0062 libcp16.a:Filter_CommFact.s@(_TARGET_IC==5???)
   0332B 000B libcp16.a:Filter_FltFloatCoeff.s@(_TARGET_IC==5???)
   03336 003A libcp16.a:Filter_CheckParameters.s@(_TARGET_IC==5???)
   03370 003D libcp16.a:Filter_CheckStability.s@(_TARGET_IC==5???)
   033AD 0024 libcp16.a:Filter_AP1.s@(_TARGET_IC==5???)
   033D1 0017 libcp16.a:Filter_AP2.s@(_TARGET_IC==5???)
   033E8 0014 libcp16.a:Filter_FLAT.s@(_TARGET_IC==5???)
   033FC 002A libcp16.a:Filter_HP1.s@(_TARGET_IC==5???)
   03426 001D libcp16.a:Filter_HP2.s@(_TARGET_IC==5???)
   03443 004D libcp16.a:Filter_HS1.s@(_TARGET_IC==5???)
   03490 0025 libcp16.a:Filter_LP1.s@(_TARGET_IC==5???)
   034B5 0036 libcp16.a:Filter_LP2.s@(_TARGET_IC==5???)
   034EB 0058 libcp16.a:Filter_LS1.s@(_TARGET_IC==5???)
   03543 003D libcp16.a:Filter_PEQ.s@(_TARGET_IC==5???)
   03580 000D libcp16.a:math_lin2db.s@(_TARGET_IC==5???)
   0358D 0026 libcp16.a:math_LN2.s

   Addr  Global Symbol
  -----  ----------------
   00400  CRTP16$Startup
   00400  __text_start
   00401  __crtP16_ConfigDataV460
   00401  __crtP16_ConfigDataV459
   00401  __crtP16_ConfigDataV455
   00401  __crtP16_ConfigDataV454
   00401  __crtP16_ConfigDataV462
   00565  CRTP16$FATAL$
   005FD  _Sys_Register1msPollingFuncFar
   005FF  __lowlevel_prepare_default
   005FF  __lowlevel_init_default
   005FF  __lowlevel_init
   005FF  __lowlevel_prepare
   00600  _InitUSB
   00637  _main
   00693  _Send
   00698  _dsp1InitAndRoute
   00711  _dsp1NrpnHandler
   00730  _dsp2InitAndRoute
   007D2  _dsp2NrpnHandler
   007F1  _dsp3InitAndRoute
   0086A  _dsp3NrpnHandler
   00889  _dsp4InitAndRoute
   0092C  _dsp4NrpnHandler
   0094B  _dspDesigner_InitNrpnFunction
   00964  _dspDesigner_HandleNRPN
   009D2  _dspDesigner_HandleMidiEvent
   00B78  _dichotomicSearch
   00C0B  _sendSysExMessage
   00C31  _biquadSetLoadingPreset
   00C36  _SetButterworthType
   00CE3  _GetBesselFrequency
   00D26  _SetBesselType
   00DCF  _SetLinkwitzRileyType
   00E70  _Set1stOrder
   00EC4  _SetFlat
   00EE7  _SetXOverFilterType
   00F5F  _FilterCoefficientsUpdate
   01028  _SetFilterGain
   01081  _SetFilterFreq
   0111E  _SetFilterQ
   0118E  _getNumberOfXoverBands
   011D1  _clearLockedBands
   011E9  _flatBands
   0120D  _SetFilterType
   012D3  _InitDefaultPeqBands
   01356  _UpdateXOverIIRCoeff
   0150F  _shared_NrpnDispatch_DelayBiquad
   0161D  _shared_NrpnDispatch_GainBiquadComp
   01783  _OutBusOf
   0178B  _getBus
   017A8  _dspDesigner_InitAndRoute
   01838  updateBiquadCallback
   01838  _updateBiquadCallback$R1R0R4R5R6R7$
   01841  _dspNrpnHandlerCallback
   01858  _swapDword$R1R0$
   01858  swapDword
   0185A  __Sys_ClearRAM$R4R3R0$
   0185A  _Sys_ClearRAM
   0186F  __Sys_ClearRAMex$R4R3R1R0$
   0186F  _Sys_ClearRAMex
   0188B  _DSPresetAll
   0188B  __DSPresetAll$$
   018AD  __DSPalloc$R1$R2
   018AD  _DSPalloc
   018D7  _DSProutEx
   018D7  __DSProutEx$R1R2$R3
   018E3  _DSProutInEx
   018E3  __DSProutInEx$R4R2$R1R7
   018FD  __Math_dBToLinear$R7R6$
   018FD  _Math_dBToLinear
   0190C  __Sys_FloatToFix$R7R6R1$
   0190C  _Sys_FloatToFix
   01915  __Sys_dBLinearToLinear$R7$
   01915  _Sys_dBLinearToLinear
   01932  __Sys_MididBGain$R7R1$
   01932  _Sys_MididBGain
   0193A  _Sys_MididBGainToDSP
   01941  __Sys_MidiSquaredGain$R7R1$
   01941  _Sys_MidiSquaredGain
   0194D  _Sys_MidiSquaredGainToDSP
   01952  __Sys_ConvdB$R6$R3
   01952  _Sys_ConvdB
   0196F  _LoadP24
   0199C  __DSPstart$R4$
   0199C  _DSPstart
   0199C  _StartDSP
   0199C  _StartP24
   0199C  __StartDSP$R4$
   019A7  _StopP24
   019B3  _DIVIDE16
   019B5  _DIVIDE32
   019CE  _Sys_CalcTimerReload
   019CE  __Sys_CalcTimerReload$R3R2$
   019D6  __Sys_CalcTimerReloadForFrequency$R3R2R5R4$
   019D6  _Sys_CalcTimerReloadForFrequency
   019E4  __Sys_GetCrystalFreq$$
   019E4  _Sys_GetCrystalFreq
   019EC  __Sys_GetSystemFreq$$
   019EC  _Sys_GetSystemFreq
   019F6  _Sys_SwitchPll5xxx
   019F6  __Sys_SwitchPll5xxx$R6R7$
   01A20  _Sys_GetSamplingRate
   01A20  __Sys_GetSamplingRate$$
   01A2D  __Sys_InvalidateSamplingRate$$
   01A2D  _Sys_InvalidateSamplingRate
   01A44  __Sys_GetFSPinState$$
   01A44  _Sys_GetFSPinState
   01A52  _USBDDE_SAM5xxx_IOCTL
   01A52  __USBDDE_SAM5xxx_IOCTL$R5R7R6$
   01A7E  __USB_GetStatus$$
   01A7E  _USB_GetStatus
   01A85  _UsbcTraceStop$$
   01A85  UsbcTraceStop
   01A86  UsbCore$HsChanged
   01A8A  __USB_RawPoll$$
   01A8A  _USB_RawPoll
   01AE0  UsbPoll16$SOF
   01AF7  __USB_RegisterP16Polling$$
   01AF7  _USB_RegisterP16Polling
   01B00  _USB_Poll
   01B00  __USB_Poll$$
   01B06  UsbPoll$SetPollFunc
   01B0F  Usbc$SendControlPacket
   01B1D  UsbEnum$AckSetupPacket
   01CEB  UsbEnum$SendShortCtrlPacket
   01CEE  UsbEnum$SendCtrlPacket
   01CFA  UsbEnum$ProcessSetup
   01D11  UsbEnum$StpIgnoreRequest
   01D68  UsbEnum$BusReset
   01D88  UsbEnum$Suspend
   01D8C  UsbEnum$Resume
   01D8D  __USB_SetPowerState$R7$
   01D8D  _USB_SetPowerState
   01D95  __USB_GetDataPtr$$
   01D95  _USB_GetDataPtr
   01DA2  UsbInit$CallbackDummy
   01DA3  _UsbEventDummy
   01DA4  __USB_SetEventHook$R7R6$
   01DA4  _USB_SetEventHook
   01DA8  UsbEvent$HandleEventC
   01DB7  __USBMC_SendMidiEvent$R7R6$
   01DB7  _USBMC_SendMidiEvent
   01DBD  _USBMC_PollMidiEvent
   01DBD  __USBMC_PollMidiEvent$R1$
   01DC3  _USBMC_InitSetBuffer
   01DC3  __USBMC_InitSetBuffer$R0R1R6R7$
   01DC9  _USBMC_Send
   01DC9  __USBMC_Send$R7$
   021A2  __MixPA_ResetDSP$R4$
   021A2  _MixPA_ResetDSP
   021BD  __MixPA_IsDSPCycleOverflow$R4$
   021BD  _MixPA_IsDSPCycleOverflow
   021C2  __MixPA_SetProcAdd2$R4R1$
   021C2  _MixPA_SetProcAdd2
   021C6  _MixPA_Biquad_OnOff
   021C6  __MixPA_Biquad_OnOff$R4R1R0$
   021D0  _MixPA_Biquad_InGainValue
   021D0  __MixPA_Biquad_InGainValue$R4R1R7$
   021DA  __MixPA_Biquad_InLinearGainValue$R4R1R7$
   021DA  _MixPA_Biquad_InLinearGainValue
   021E1  _MixPA_Biquad_InGainPhase
   021E1  __MixPA_Biquad_InGainPhase$R4R1R0$
   021FC  __MixPA_Biquad_UpdateCoeff$R4R3R1R7$
   021FC  _MixPA_Biquad_UpdateCoeff
   02203  __MixPA_Biquad_Update48bitCoeff$R4R3R1R7$
   02203  _MixPA_Biquad_Update48bitCoeff
   02265  __MixPA_Biquad_Flat$R4R3R1R7$
   02265  _MixPA_Biquad_Flat
   0228B  __MixPA_SetProcBiquad$R4R1$
   0228B  _MixPA_SetProcBiquad
   0228F  __MixPA_SetProcBiquadEx$R4R1$
   0228F  _MixPA_SetProcBiquadEx
   02293  __MixPA_BusSendN_GainValue$R4R1R0R7$
   02293  _MixPA_BusSendN_GainValue
   022A0  __MixPA_BusSendN_LinearGainValue$R4R1R3R7$
   022A0  _MixPA_BusSendN_LinearGainValue
   022AB  __MixPA_BusSendN_GainPhase$R4R1R3R0$
   022AB  _MixPA_BusSendN_GainPhase
   022B6  __MixPA_SetProcBusSend$R4R1$
   022B6  _MixPA_SetProcBusSend
   022BA  _MixPA_Compressor_OnOff
   022BA  __MixPA_Compressor_OnOff$R4R1R0$
   022CA  _MixPA_Compressor_Threshold
   022CA  __MixPA_Compressor_Threshold$R4R1R0$
   022E9  _MixPA_Compressor_Ratio
   022E9  __MixPA_Compressor_Ratio$R4R1R7$
   022F3  _MixPA_Compressor_ConnectLevel
   022F3  __MixPA_Compressor_ConnectLevel$R4R2R1$
   02302  _MixPA_Compressor_Boost
   02302  __MixPA_Compressor_Boost$R4R1R7$
   02313  _MixPA_Compressor_BoostPhase
   02313  __MixPA_Compressor_BoostPhase$R4R1R0$
   02319  _MixPA_Compressor_GetGainReduction
   02319  __MixPA_Compressor_GetGainReduction$R4R1$
   02324  __MixPA_SetProcCompressor$R4R1$
   02324  _MixPA_SetProcCompressor
   02328  __MixPA_Compander_OnOff$R4R1R0$
   02328  _MixPA_Compander_OnOff
   02340  __MixPA_Compander_Threshold$R4R1R0$
   02340  _MixPA_Compander_Threshold
   02358  __MixPA_Compander_Threshold2$R4R1R0$
   02358  _MixPA_Compander_Threshold2
   02375  __MixPA_Compander_Softness$R4R1R0$
   02375  _MixPA_Compander_Softness
   02396  __MixPA_Compander_Ratio$R4R1R7$
   02396  _MixPA_Compander_Ratio
   023A7  __MixPA_Compander_Ratio2$R4R1R7$
   023A7  _MixPA_Compander_Ratio2
   023BE  __MixPA_Compander_ConnectLevel$R4R2R1$
   023BE  _MixPA_Compander_ConnectLevel
   023CD  __MixPA_Compander_Boost$R4R1R7$
   023CD  _MixPA_Compander_Boost
   023DE  __MixPA_Compander_BoostPhase$R4R1R0$
   023DE  _MixPA_Compander_BoostPhase
   023E4  __MixPA_Compander_GetGainReduction$R4R1$
   023E4  _MixPA_Compander_GetGainReduction
   023EE  __MixPA_SetProcCompander$R4R1$
   023EE  _MixPA_SetProcCompander
   0247A  _MixPA_SoftKnee
   026E3  _MixPA_Delay_OnOff
   026E3  __MixPA_Delay_OnOff$R4R1R0$
   026ED  _MixPA_Delay_OutGainValue
   026ED  __MixPA_Delay_OutGainValue$R4R1R7$
   026F7  __MixPA_Delay_OutLinearGainValue$R4R1R7$
   026F7  _MixPA_Delay_OutLinearGainValue
   026FE  _MixPA_Delay_OutGainPhase
   026FE  __MixPA_Delay_OutGainPhase$R4R1R0$
   02738  _MixPA_Delay_SetStartAddr
   02738  __MixPA_Delay_SetStartAddr$R4R1R2R0$
   02757  _MixPA_Delay_SetTime
   02757  __MixPA_Delay_SetTime$R4R1R7R6$
   027B8  __MixPA_Delay_GetTime$R4R1$
   027B8  _MixPA_Delay_GetTime
   027C0  __MixPA_SetProcDelay$R4R1$
   027C0  _MixPA_SetProcDelay
   027C4  __MixPA_SetProcDelay_INTRAM$R4R1$
   027C4  _MixPA_SetProcDelay_INTRAM
   027D1  __MixPA_Exciter_OnOff$R4R1R0$
   027D1  _MixPA_Exciter_OnOff
   027DB  __MixPA_Exciter_InGainValue$R4R1R7$
   027DB  _MixPA_Exciter_InGainValue
   027E5  __MixPA_Exciter_Intensity$R4R1R7$
   027E5  _MixPA_Exciter_Intensity
   027F0  __MixPA_Exciter_LoContourAmount$R4R1R7$
   027F0  _MixPA_Exciter_LoContourAmount
   027FB  __MixPA_Exciter_ThresholdLow$R4R1R7$
   027FB  _MixPA_Exciter_ThresholdLow
   02803  __MixPA_Exciter_ThresholdHigh$R4R1R7$
   02803  _MixPA_Exciter_ThresholdHigh
   0280B  __MixPA_SetProcExciter$R4R1$
   0280B  _MixPA_SetProcExciter
   02810  __MixPA_FreqShifter_OnOff$R4R1R7$
   02810  _MixPA_FreqShifter_OnOff
   0281A  __MixPA_FreqShifter_Amount$R4R1R7$
   0281A  _MixPA_FreqShifter_Amount
   02837  __MixPA_FreqShifter_SetMaxScale$R4R1R7$
   02837  _MixPA_FreqShifter_SetMaxScale
   02849  __MixPA_FreqShifter_PostHP$R4R1R7$
   02849  _MixPA_FreqShifter_PostHP
   02853  __MixPA_FreqShifter_InGainValue$R4R1R7$
   02853  _MixPA_FreqShifter_InGainValue
   0285D  __MixPA_FreqShifter_InLinearGainValue$R4R1R7$
   0285D  _MixPA_FreqShifter_InLinearGainValue
   02864  __MixPA_FreqShifter_InGainPhase$R4R1R0$
   02864  _MixPA_FreqShifter_InGainPhase
   0286A  __MixPA_SetProcFreqShifter$R4R1$
   0286A  _MixPA_SetProcFreqShifter
   0286F  _MixPA_Gain_Value
   0286F  __MixPA_Gain_Value$R4R1R7$
   0288E  __MixPA_Gain_LinearGainValue$R4R1R7$
   0288E  _MixPA_Gain_LinearGainValue
   02895  _MixPA_Gain_Phase
   02895  __MixPA_Gain_Phase$R4R1R0$
   0289E  __MixPA_SetProcGain$R4R1$
   0289E  _MixPA_SetProcGain
   028A2  __MixPA_GainN_Value$R4R1R0R7$
   028A2  _MixPA_GainN_Value
   028AF  __MixPA_GainN_LinearGainValue$R4R1R3R7$
   028AF  _MixPA_GainN_LinearGainValue
   028BA  __MixPA_GainN_Phase$R4R1R3R0$
   028BA  _MixPA_GainN_Phase
   028C5  __MixPA_SetProcGainN$R4R1$
   028C5  _MixPA_SetProcGainN
   028C9  _MixPA_LevelDetect_Allocate
   028C9  __MixPA_LevelDetect_Allocate$R4$
   028FD  __MixPA_SetProcLEVELDETECT2$R4R1$
   028FD  _MixPA_SetProcLEVELDETECT2
   02901  _MixPA_LevelDetect_Attack
   02901  __MixPA_LevelDetect_Attack$R4R1R7$
   02939  _MixPA_LevelDetect_Release
   02939  __MixPA_LevelDetect_Release$R4R1R7$
   02971  __MixPA_LevelDetect_GetLevel$R4R1$
   02971  _MixPA_LevelDetect_GetLevel
   02979  __MixPA_MixN_GainValue$R4R1R0R7$
   02979  _MixPA_MixN_GainValue
   02986  __MixPA_MixN_GainPhase$R4R1R3R0$
   02986  _MixPA_MixN_GainPhase
   02991  __MixPA_MixN_LinearGainValue$R4R1R3R7$
   02991  _MixPA_MixN_LinearGainValue
   0299C  __MixPA_SetProcMixN$R4R1$
   0299C  _MixPA_SetProcMixN
   029A0  __MixPA_NoiseGate_OnOff$R4R1R0$
   029A0  _MixPA_NoiseGate_OnOff
   029AA  __MixPA_NoiseGate_InGainValue$R4R1R7$
   029AA  _MixPA_NoiseGate_InGainValue
   029B4  __MixPA_NoiseGate_InGainPhase$R4R1R0$
   029B4  _MixPA_NoiseGate_InGainPhase
   029BA  __MixPA_NoiseGate_Threshold$R4R1R7$
   029BA  _MixPA_NoiseGate_Threshold
   029C6  __MixPA_NoiseGate_Attack$R4R1R7$
   029C6  _MixPA_NoiseGate_Attack
   029D6  __MixPA_NoiseGate_Release$R4R1R7$
   029D6  _MixPA_NoiseGate_Release
   029EA  __MixPA_SetProcNoiseGate$R4R1$
   029EA  _MixPA_SetProcNoiseGate
   029EF  __MixPA_PeakLevel_Allocate$R4$
   029EF  _MixPA_PeakLevel_Allocate
   02A05  __MixPA_PeakLevel_GetPeak$R4R1$
   02A05  _MixPA_PeakLevel_GetPeak
   02A12  __MixPA_SetProcPeakLevel$R4R1$
   02A12  _MixPA_SetProcPeakLevel
   02A16  __MixPA_Spatializer_SetDelayStartAddr$R4R1R2R0$
   02A16  _MixPA_Spatializer_SetDelayStartAddr
   02A34  __MixPA_Spatializer_OnOff$R4R1R0$
   02A34  _MixPA_Spatializer_OnOff
   02A3E  __MixPA_Spatializer_Level$R4R1R7$
   02A3E  _MixPA_Spatializer_Level
   02A48  __MixPA_Spatializer_SetDelayTime$R4R1R6$
   02A48  _MixPA_Spatializer_SetDelayTime
   02A9D  __MixPA_Spatializer_InputInvertedMode$R4R1R7$
   02A9D  _MixPA_Spatializer_InputInvertedMode
   02AA6  __MixPA_Spatializer_4chOutputMode$R4R1R7$
   02AA6  _MixPA_Spatializer_4chOutputMode
   02AAF  __MixPA_SetProcSpatializer$R4R1$
   02AAF  _MixPA_SetProcSpatializer
   02AB4  __MixPA_SetProcSub2$R4R1$
   02AB4  _MixPA_SetProcSub2
   02AB8  __MixPA_PitchShifter_OnOff$R4R1R0$
   02AB8  _MixPA_PitchShifter_OnOff
   02AC2  __MixPA_PitchShifter_SetPitch$R4R1R7$
   02AC2  _MixPA_PitchShifter_SetPitch
   02B0F  __MixPA_SetProcPitchShifter$R4R1$
   02B0F  _MixPA_SetProcPitchShifter
   02B13  _MixPA_SetProcIN
   02B13  __MixPA_SetProcIN$R4R1R7$
   02B28  _MixPA_SetProcOUT
   02B28  __MixPA_SetProcOUT$R4R1R7$
   02B3D  __MixPA_SetProcIOStep$R4R1R7$
   02B3D  _MixPA_SetProcIOStep
   02B4A  _GetSystemSamplingRateFloat
   02B51  _DSPRAM_Clear
   02B58  __GetNextParamAddr$R4R3$
   02B58  _GetNextParamAddr
   02B5C  _CheckParameterEnd
   02B72  UpdateModulo4
   02B7C  _GetNewProcessID
   02B91  __SetALLBYPASS
   02B97  _ResetProcessParameter
   02BA0  _GetProcDataPtr
   02BA5  _IsDSPCycleOverflow
   02BB0  _CommonGain_midiTodBGain
   02BB8  _CommonGain_writeToDSP
   02BC6  _CommonGain_UpdatePhase
   02BE3  __MixPA_Init
   02BE6  __MixPA_ResetDSP
   02BEB  __MixPA_IsDSPCycleOverflow
   02BF0  __MixPA_Add2_Allocate
   02BF5  __MixPA_SetProcAdd2
   02BFB  __MixPA_Biquad_Allocate
   02C01  __MixPA_Biquad_OnOff
   02C08  __MixPA_Biquad_InGainValue
   02C0F  __MixPA_Biquad_InLinearGainValue
   02C16  __MixPA_Biquad_InGainPhase
   02C1D  __MixPA_Biquad_UpdateCoeff
   02C25  __MixPA_Biquad_Update48bitCoeff
   02C2D  __MixPA_Biquad_Flat
   02C35  __MixPA_SetProcBiquad
   02C3B  __MixPA_BiquadEx_Allocate
   02C41  __MixPA_SetProcBiquadEx
   02C47  __MixPA_BusSendN_Allocate
   02C4D  __MixPA_BusSendN_GainValue
   02C55  __MixPA_BusSendN_LinearGainValue
   02C5D  __MixPA_BusSendN_GainPhase
   02C65  __MixPA_SetProcBusSend
   02C6B  __MixPA_Compressor_Allocate
   02C70  __MixPA_Compressor_OnOff
   02C77  __MixPA_Compressor_Threshold
   02C7E  __MixPA_Compressor_Ratio
   02C85  __MixPA_Compressor_ConnectLevel
   02C8C  __MixPA_Compressor_Boost
   02C93  __MixPA_Compressor_BoostPhase
   02C9A  __MixPA_Compressor_GetGainReduction
   02CA0  __MixPA_SetProcCompressor
   02CA6  __MixPA_Compander_Allocate
   02CAB  __MixPA_Compander_OnOff
   02CB2  __MixPA_Compander_Threshold
   02CB9  __MixPA_Compander_Threshold2
   02CC0  __MixPA_Compander_Softness
   02CC7  __MixPA_Compander_Ratio
   02CCE  __MixPA_Compander_Ratio2
   02CD5  __MixPA_Compander_ConnectLevel
   02CDC  __MixPA_Compander_Boost
   02CE3  __MixPA_Compander_BoostPhase
   02CEA  __MixPA_Compander_GetGainReduction
   02CF0  __MixPA_SetProcCompander
   02CF6  __MixPA_Delay_Allocate
   02CFB  __MixPA_Delay_OnOff
   02D02  __MixPA_Delay_OutGainValue
   02D09  __MixPA_Delay_OutLinearGainValue
   02D10  __MixPA_Delay_OutGainPhase
   02D17  __MixPA_Delay_SetStartAddr
   02D1F  __MixPA_Delay_SetTime
   02D27  __MixPA_Delay_GetTime
   02D2D  __MixPA_SetProcDelay
   02D33  __MixPA_SetProcDelay_INTRAM
   02D39  __MixPA_Exciter_Allocate
   02D3E  __MixPA_Exciter_OnOff
   02D45  __MixPA_Exciter_InGainValue
   02D4C  __MixPA_Exciter_Intensity
   02D53  __MixPA_Exciter_LoContourAmount
   02D5A  __MixPA_Exciter_ThresholdLow
   02D61  __MixPA_Exciter_ThresholdHigh
   02D68  __MixPA_SetProcExciter
   02D6E  __MixPA_FreqShifter_Allocate
   02D73  __MixPA_FreqShifter_OnOff
   02D7A  __MixPA_FreqShifter_Amount
   02D81  __MixPA_FreqShifter_SetMaxScale
   02D88  __MixPA_FreqShifter_PostHP
   02D8F  __MixPA_FreqShifter_InGainValue
   02D96  __MixPA_FreqShifter_InLinearGainValue
   02D9D  __MixPA_FreqShifter_InGainPhase
   02DA4  __MixPA_SetProcFreqShifter
   02DAA  __MixPA_Gain_Allocate
   02DAF  __MixPA_Gain_Value
   02DB6  __MixPA_Gain_LinearGainValue
   02DBD  __MixPA_Gain_Phase
   02DC4  __MixPA_SetProcGain
   02DCA  __MixPA_GainN_Allocate
   02DD0  __MixPA_GainN_Value
   02DD8  __MixPA_GainN_LinearGainValue
   02DE0  __MixPA_GainN_Phase
   02DE8  __MixPA_SetProcGainN
   02DEE  __MixPA_LevelDetect_Allocate
   02DF3  __MixPA_SetProcLEVELDETECT2
   02DF9  __MixPA_LevelDetect_Attack
   02E00  __MixPA_LevelDetect_Release
   02E07  __MixPA_LevelDetect_GetLevel
   02E0D  __MixPA_MixN_Allocate
   02E13  __MixPA_MixN_GainValue
   02E1B  __MixPA_MixN_GainPhase
   02E23  __MixPA_MixN_LinearGainValue
   02E2B  __MixPA_SetProcMixN
   02E31  __MixPA_NoiseGate_Allocate
   02E36  __MixPA_NoiseGate_OnOff
   02E3D  __MixPA_NoiseGate_InGainValue
   02E44  __MixPA_NoiseGate_InGainPhase
   02E4B  __MixPA_NoiseGate_Threshold
   02E52  __MixPA_NoiseGate_Attack
   02E59  __MixPA_NoiseGate_Release
   02E60  __MixPA_SetProcNoiseGate
   02E66  __MixPA_PeakLevel_Allocate
   02E6B  __MixPA_PeakLevel_GetPeak
   02E71  __MixPA_SetProcPeakLevel
   02E77  __MixPA_Spatializer_Allocate
   02E7C  __MixPA_Spatializer_SetDelayStartAddr
   02E84  __MixPA_Spatializer_OnOff
   02E8B  __MixPA_Spatializer_Level
   02E92  __MixPA_Spatializer_SetDelayTime
   02E99  __MixPA_Spatializer_InputInvertedMode
   02EA0  __MixPA_Spatializer_4chOutputMode
   02EA7  __MixPA_SetProcSpatializer
   02EAD  __MixPA_Sub2_Allocate
   02EB2  __MixPA_SetProcSub2
   02EB8  __MixPA_PitchShifter_Allocate
   02EC0  __MixPA_PitchShifter_OnOff
   02EC7  __MixPA_PitchShifter_SetPitch
   02ECE  __MixPA_SetProcPitchShifter
   02ED4  __MixPA_SetProcIN
   02EDB  __MixPA_SetProcOUT
   02EE2  __MixPA_SetProcIOStep
   02EE9  _fexp
   02EE9  __fexp$R7R6$
   02F45  __ftrunc$R7R6$
   02F45  _ftrunc
   02F54  __ffract$R7R6$
   02F54  _ffract
   02F59  __fsqrt$R5R4$R2R3R4R5R6R7
   02F59  _fsqrt
   02F8D  _float
   02F8D  __float$R7R6$R2R3R4R5R6R7
   02FAA  __floatex$R7R6R1$R2R3R4R5R6R7
   02FAA  _floatex
   02FDB  _ftol
   02FDB  __ftol$R7R6$R2R3R4R5R6R7
   02FF5  _fmul
   02FF5  __fmul$R7R6R5R4$R2R3R4R5R6R7
   0305C  _fdiv
   0305C  __fdiv$R7R6R5R4$R2R3R4R5R6R7
   030AE  _fadd
   030AE  __fadd$R7R6R5R4$R2R3R4R5R6R7
   030EE  _fsub
   030EE  __fsub$R7R6R5R4$R2R3R4R6R7
   030F7  _fneg
   030F7  __fneg$R1R0$R2R3R4R5R6R7
   030FC  _fsign
   030FC  __fsign$R7R6$R1R2R3R4R5R6R7
   030FF  __fcmp$R7R6R5R4$R1R2R3R4R5R6R7
   030FF  _fcmp
   03115  __flimit$R7R6R5R4$R2R3R4R5R6R7
   03115  _flimit
   0311E  _fgt
   0311E  __fgt$R7R6R5R4$R1R2R3R4R5R6R7
   0312A  __fabs$R1R0$R2R3R4R5R6R7
   0312A  _fabs
   0312D  __fgetexp$R7R6$R1R2R3R4R5R6R7
   0312D  _fgetexp
   03132  _fpow2
   03132  __fpow2$R1$R2R3R4R5R6R7
   031C7  _fgtLegacy
   031C7  __fgtLegacy$R7R6R5R4$R1R2R3R4R5R6R7
   031EE  __fsin$R7R6$R2R3R4R5R6R7
   031EE  _fsin
   03235  __fcos$R7R6$R2R3R4R5R6R7
   03235  _fcos
   0325B  __ftan$R7R6$R2R3R4R5R6R7
   0325B  _ftan
   03265  _fpolyEx
   03265  __fpolyEx$R7R6R5R4R3$R2R6R7
   03290  _Biquad_FloatToFixCoeff
   03290  __Biquad_FloatToFixCoeff$R2R3R7$
   032AA  _Biquad$Prewarp
   032BA  _Biquad$1stOrder_Denominator
   032C9  _Biquad$CommonFx
   032D6  _Biquad$ScaleddB
   032E6  _Biquad$CommonF0Q
   032EF  _Biquad$CommonAlpha
   032FE  _Biquad$CommonA1A2
   03326  _Biquad$CommonFactor
   0332B  _Biquad_FltFloatCoeff
   0332B  __Biquad_FltFloatCoeff$R1R0R2R3$
   03336  __Biquad_CheckParameters$R2$
   03336  _Biquad_CheckParameters
   03370  __Biquad_CheckStability$R3$
   03370  _Biquad_CheckStability
   033AD  __Biquad_FilterAP1
   033D1  __Biquad_FilterAP2
   033E8  __Biquad_FilterFLAT
   033FC  __Biquad_FilterHP1
   03426  __Biquad_FilterHP2
   03443  __Biquad_FilterHS1
   03490  __Biquad_FilterLP1
   034B5  __Biquad_FilterLP2
   034EB  __Biquad_FilterLS1
   03543  __Biquad_FilterPEQ
   03580  __Math_lin2dB$R6$R3
   03580  _Math_lin2dB
   0358D  __Math_LN2$R6$R3
   0358D  _Math_LN2
   035B3  __text_end

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
           const    35B3    326       806 words (CON,REL,ROM)

   Addr Size Module
  ----- ---- ----------------
   035B3 0006 libcrtp16.a:crtp16.s@(_TARGET_IC==5???)
   035B9 008C main.o:main.s
   03645 0026 dsp1.o:dsp1.s
   0366B 0034 dsp2.o:dsp2.s
   0369F 0026 dsp3.o:dsp3.s
   036C5 0034 dsp4.o:dsp4.s
   036F9 009E BiquadCtrl.o:BiquadCtrl.s
   03797 0044 mainbus.o:mainbus.s
   037DB 0008 libsys.a:sys_GetCrystalFreq.s@(_TARGET_IC==5???)
   037E3 0004 libsys.a:sys_GetSamplingRate.s@(_TARGET_IC==5???)
   037E7 0016 libUSB.a:usb_Enum.s@(_TARGET_IC==5???)
   037FD 0010 libUSB.a:usb_DrvMidiClass.s@(_TARGET_IC==5???)
   0380D 002A libUSB.a:usb_DrvSam5xxx.s@(_TARGET_IC==5???)
   03837 0030 libFX5000.a:MixPA.s@(_TARGET_IC==5???)
   03867 000A libcp16.a:5x_fexp.s@(_TARGET_IC==5???)
   03871 004E libcp16.a:5x_float.s@(_TARGET_IC==5???)
   038BF 001A libcp16.a:5x_ftrig.s@(_TARGET_IC==5???)

   Addr  Global Symbol
  -----  ----------------
   035B3  __const_start
   035B3  __crtP16_AppVersion
   035B6  __crtP16_SamVSVersion
   035B9  _UsbDescriptorInfo
   03645  _nrpn1List
   0365F  _nrpn1BiquadTable
   0366B  _nrpn2List
   03687  _nrpn2BiquadTable
   0369F  _nrpn3List
   036B9  _nrpn3BiquadTable
   036C5  _nrpn4List
   036E1  _nrpn4BiquadTable
   036F9  _TAB_BUTTERWORTH
   03721  _TAB_BESSEL
   03771  _TAB_LINKRILEY
   03797  _dspRouting_In
   037B7  _dspRouting_Out
   037FD  __USBDDE_MidiClass
   0380D  __USBDDE_SAM5xxx
   038D9  __const_end

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
           cnstu    38D9     18        24 words (CON,REL,ROM)

   Addr Size Module
  ----- ---- ----------------
   038D9 0018 libUSB.a:usb_Enum.s@(_TARGET_IC==5???)

   Addr  Global Symbol
  -----  ----------------
   038D9  __cnstu_start
   038F1  __cnstu_end

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
          systab    38F1      0         0 words (CON,REL,ROM)

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
           idata    38F1     4A        74 words (CON,REL,ROM)

   Addr Size Module
  ----- ---- ----------------
   038F1 0002 libcrtp16.a:crtp16.s@(_TARGET_IC==5???)
   038F3 0006 dsp1.o:dsp1.s
   038F9 000C dsp2.o:dsp2.s
   03905 0006 dsp3.o:dsp3.s
   0390B 000C dsp4.o:dsp4.s
   03917 0020 midictrl.o:midictrl.s
   03937 0002 BiquadCtrl.o:BiquadCtrl.s
   03939 0002 libsys.a:sys_GetSamplingRate.s@(_TARGET_IC==5???)

   Addr  Global Symbol
  -----  ----------------
   038F1  __crtP16_Version
   038F1  __idata_start
   0393B  __idata_end

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
            data       0     4A        74 words (CON,REL,RAM)

   Addr Size Module
  ----- ---- ----------------
   00000 0002 libcrtp16.a:crtp16.s@(_TARGET_IC==5???)
   00002 0006 dsp1.o:dsp1.s
   00008 000C dsp2.o:dsp2.s
   00014 0006 dsp3.o:dsp3.s
   0001A 000C dsp4.o:dsp4.s
   00026 0020 midictrl.o:midictrl.s
   00046 0002 BiquadCtrl.o:BiquadCtrl.s
   00048 0002 libsys.a:sys_GetSamplingRate.s@(_TARGET_IC==5???)

   Addr  Global Symbol
  -----  ----------------
   00000  __data_start
   00002  _biquad1ParamAddr2
   00008  _biquad2ParamAddr2
   0000E  _biquad2ParamAddr4
   00014  _biquad3ParamAddr2
   0001A  _biquad4ParamAddr2
   00020  _biquad4ParamAddr4
   0004A  __data_end

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
            iram      23     17        23 words (CON,REL,RAM)

   Addr Size Module
  ----- ---- ----------------
   00023 0006 libcrtp16.a:crtp16.s@(_TARGET_IC==5???)
   00029 0004 libUSB.a:usb_Dde5xxxIoctl.s@(_TARGET_IC==5???)
   0002D 0006 libUSB.a:usb_Core.s@(_TARGET_IC==5???)
   00033 0000 libUSB.a:usb_CoreHS.s@(_TARGET_IC==5???)
   00033 0000 libUSB.a:usb_P16Poll.s@(_TARGET_IC==5???)
   00033 0000 libUSB.a:usb_Poll.s@(_TARGET_IC==5???)
   00033 0000 libUSB.a:usb_Enum.s@(_TARGET_IC==5???)
   00033 0000 libUSB.a:usb_Init.s@(_TARGET_IC==5???)
   00033 0000 libUSB.a:usb_Event.s@(_TARGET_IC==5???)
   00033 0000 libUSB.a:usb_MidiClass.s@(_TARGET_IC==5???)
   00033 0004 libUSB.a:usb_MidiSendWrp.s@(_TARGET_IC==5???)
   00037 0000 libUSB.a:usb_DrvMidiClass.s@(_TARGET_IC==5???)
   00037 0003 libUSB.a:usb_DrvSam5xxx.s@(_TARGET_IC==5???)

   Addr  Global Symbol
  -----  ----------------
   00023  _Sys_1msPollingFunc_farptr
   00023  __iram_start
   00025  CRTP16$FATAL$Handler
   00027  CRTP16$FATAL$Message
   00029  Cusb5xxx_RpcCallbackDefault
   0002B  Cusb5xxx_RpcCallback
   0002D  UsbIspAddr
   0002F  UsbFlags
   00030  UsbInterruptPort
   00031  UsbInterruptMask
   00032  Usb$Activity
   0003A  __iram_end

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
           iramh     100      0         0 words (CON,REL,RAM)

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
            fixd       0      0         0 words (CON,REL,ROM)

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
            fixi       0      0         0 words (CON,REL,ROM)

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
             cst       0      0         0 words (CON,REL,ROM)

   Addr  Global Constants
  -----  ----------------
   00000  __cst_end[<default>]
   00000  __cst_start[<default>]

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
             bss      4A    47E      1150 words (CON,REL,RAM)

   Addr Size Module
  ----- ---- ----------------
   0004A 0044 libcrtp16.a:crtp16.s@(_TARGET_IC==5???)
   0008E 019C main.o:main.s
   0022A 002A dsp1.o:dsp1.s
   00254 004E dsp2.o:dsp2.s
   002A2 002A dsp3.o:dsp3.s
   002CC 004E dsp4.o:dsp4.s
   0031A 002A midictrl.o:midictrl.s
   00344 0004 mainbus.o:mainbus.s
   00348 0172 libUSB.a:usb_Core.s@(_TARGET_IC==5???)
   004BA 0000 libUSB.a:usb_CoreHS.s@(_TARGET_IC==5???)
   004BA 0000 libUSB.a:usb_P16Poll.s@(_TARGET_IC==5???)
   004BA 0002 libUSB.a:usb_Poll.s@(_TARGET_IC==5???)
   004BC 0000 libUSB.a:usb_Enum.s@(_TARGET_IC==5???)
   004BC 0000 libUSB.a:usb_Init.s@(_TARGET_IC==5???)
   004BC 0000 libUSB.a:usb_Event.s@(_TARGET_IC==5???)
   004BC 0000 libUSB.a:usb_MidiClass.s@(_TARGET_IC==5???)
   004BC 000C libUSB.a:usb_DrvMidiClass.s@(_TARGET_IC==5???)
   004C8 0000 libUSB.a:usb_DrvSam5xxx.s@(_TARGET_IC==5???)

   Addr  Global Symbol
  -----  ----------------
   0004A  __crtP16_FSState
   0004A  __bss_start
   0004B  __crtP16_IntVec
   0008B  __crtP16_CatchVec
   0008E  _UsbMidiOutBuf
   001EA  _UsbMidiInBuf
   0022A  _biquad1RawFrequency2
   00230  _biquad1XoverType2
   00233  _biquad1Type2
   00236  _biquad1Parameters2
   0024E  _dsp1pcs
   00254  _biquad2RawFrequency4
   0025A  _biquad2RawFrequency2
   00260  _biquad2XoverType4
   00263  _biquad2XoverType2
   00266  _biquad2Type4
   00269  _biquad2Type2
   0026C  _biquad2Parameters4
   00284  _biquad2Parameters2
   0029C  _dsp2pcs
   002A2  _biquad3RawFrequency2
   002A8  _biquad3XoverType2
   002AB  _biquad3Type2
   002AE  _biquad3Parameters2
   002C6  _dsp3pcs
   002CC  _biquad4RawFrequency4
   002D2  _biquad4RawFrequency2
   002D8  _biquad4XoverType4
   002DB  _biquad4XoverType2
   002DE  _biquad4Type4
   002E1  _biquad4Type2
   002E4  _biquad4Parameters4
   002FC  _biquad4Parameters2
   00314  _dsp4pcs
   0031A  _sysExData
   0033B  _nrpnFunctionPtrTable
   00344  _dsp
   00348  IspBusy
   00349  UsbData
   004C8  __bss_end

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
           stack     4C8    400      1024 words (CON,REL,ROM)

   Addr Size Module
  ----- ---- ----------------

   Addr  Global Symbol
  -----  ----------------
   004C8  __stack_start
   008C8  __stack_end

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
            fram     8C8      0         0 words (CON,REL,ROM)

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
            txth    393B    AB8      2744 words (CON,REL,ROM)

   Addr Size Module
  ----- ---- ----------------
   0393B 00E3 libUSB.a:usb_Init.s@(_TARGET_IC==5???)
   03A1E 000C libUSB.a:usb_DrvMidiClass.s@(_TARGET_IC==5???)
   03A2A 0024 libUSB.a:usb_DrvSam5xxx.s@(_TARGET_IC==5???)
   03A4E 09A5 libFX5000.a:MixPA.s@(_TARGET_IC==5???)

   Addr  Global Symbol
  -----  ----------------
   0393B  _USB_Initialize
   0393B  __USB_Initialize$$
   0393B  __txth_start
   03990  _USB_InitRegisterEntry
   03990  __USB_InitRegisterEntry$R3R2R7R6$
   039BA  _USB_InitStartupEx
   039BA  __USB_InitStartupEx$R3R2R5R4R7R6$
   039DF  Uci$Mark1
   039E9  Uci$Mark2
   039F7  Uci$Mark3
   03A04  Uci$Mark4
   03A09  UsbInit$SetFeatureCallback
   03A12  __USB_InitRegisterDfuDetachHandler$R3R2R0$
   03A12  _USB_InitRegisterDfuDetachHandler
   03A4E  _MixPA_Init
   03A4E  __MixPA_Init$$
   03F57  __MixPA_Add2_Allocate$R4$
   03F57  _MixPA_Add2_Allocate
   03FCF  _MixPA_Biquad_Allocate
   03FCF  __MixPA_Biquad_Allocate$R4R1$
   03FDE  __MixPA_BiquadEx_Allocate$R4R1$
   03FDE  _MixPA_BiquadEx_Allocate
   03FED  __MixPA_BusSendN_Allocate$R4R1$
   03FED  _MixPA_BusSendN_Allocate
   0401F  _MixPA_Compressor_Allocate
   0401F  __MixPA_Compressor_Allocate$R4$
   04054  __MixPA_Compander_Allocate$R4$
   04054  _MixPA_Compander_Allocate
   040B1  _MixPA_Delay_Allocate
   040B1  __MixPA_Delay_Allocate$R4$
   040FA  __MixPA_Exciter_Allocate$R4$
   040FA  _MixPA_Exciter_Allocate
   041A8  __MixPA_FreqShifter_Allocate$R4$
   041A8  _MixPA_FreqShifter_Allocate
   04234  _MixPA_Gain_Allocate
   04234  __MixPA_Gain_Allocate$R4$
   04267  __MixPA_GainN_Allocate$R4R1$
   04267  _MixPA_GainN_Allocate
   04298  __MixPA_MixN_Allocate$R4R1$
   04298  _MixPA_MixN_Allocate
   042C6  __MixPA_NoiseGate_Allocate$R4$
   042C6  _MixPA_NoiseGate_Allocate
   04300  __MixPA_Spatializer_Allocate$R4$
   04300  _MixPA_Spatializer_Allocate
   04353  __MixPA_Sub2_Allocate$R4$
   04353  _MixPA_Sub2_Allocate
   04376  __MixPA_PitchShifter_Allocate$R4R7R6R0$
   04376  _MixPA_PitchShifter_Allocate
   043F3  __txth_end

Area               Addr   Size   Decimal Units (Attributes)
----------------   ----   ----   ------- ----- ------------
           cnsth    43F3      0         0 words (CON,REL,ROM)

Files linked by order:

   libcrtp16.a:crtp16.s@(_TARGET_IC==5???)
   main.o:main.s
   dsp1.o:dsp1.s
   dsp2.o:dsp2.s
   dsp3.o:dsp3.s
   dsp4.o:dsp4.s
   midictrl.o:midictrl.s
   BiquadCtrl.o:BiquadCtrl.s
   mainbus.o:mainbus.s
   dspDesignerWrappers.o:dspDesignerWrappers.s
   libsys.a:clearRAM.s@(_TARGET_IC==5???)
   libsys.a:clearRAMEx.s@(_TARGET_IC==5???)
   libsys.a:DSPresetAll.s@(_TARGET_IC==5???)
   libsys.a:DSPalloc.s@(_TARGET_IC==5???)
   libsys.a:DSProutEx.s@(_TARGET_IC==5???)
   libsys.a:DSProutInEx.s@(_TARGET_IC==5???)
   libsys.a:DSPGain.s@(_TARGET_IC==5???)
   libsys.a:convdB.s@(_TARGET_IC==5???)
   libsys.a:rom_LoadStartP24.s@_TARGET_IC==5???
   libsys.a:rom_StopP24.s@_TARGET_IC==5???
   libsys.a:rom_DIVIDE.s@_TARGET_IC==5???
   libsys.a:sys_CalcTimerReload.s@(_TARGET_IC==5???)
   libsys.a:sys_GetCrystalFreq.s@(_TARGET_IC==5???)
   libsys.a:sys_GetSystemFreq.s@(_TARGET_IC==5???)
   libsys.a:sys_SwitchPLL.s@_TARGET_IC==5???
   libsys.a:sys_GetSamplingRate.s@(_TARGET_IC==5???)
   libsys.a:sys_GetFSPinState.s
   libUSB.a:usb_Dde5xxxIoctl.s@(_TARGET_IC==5???)
   libUSB.a:usb_Core.s@(_TARGET_IC==5???)
   libUSB.a:usb_CoreHS.s@(_TARGET_IC==5???)
   libUSB.a:usb_P16Poll.s@(_TARGET_IC==5???)
   libUSB.a:usb_Poll.s@(_TARGET_IC==5???)
   libUSB.a:usb_Enum.s@(_TARGET_IC==5???)
   libUSB.a:usb_Init.s@(_TARGET_IC==5???)
   libUSB.a:usb_Event.s@(_TARGET_IC==5???)
   libUSB.a:usb_MidiClass.s@(_TARGET_IC==5???)
   libUSB.a:usb_MidiSendWrp.s@(_TARGET_IC==5???)
   libUSB.a:usb_DrvMidiClass.s@(_TARGET_IC==5???)
   libUSB.a:usb_DrvSam5xxx.s@(_TARGET_IC==5???)
   libFX5000.a:MixPA.s@(_TARGET_IC==5???)
   libFX5000.a:StandardFunctions.s@(_TARGET_IC==5???)
   libFX5000.a:MixPA_XW.s@(_TARGET_IC==5???)
   libcp16.a:5x_fexp.s@(_TARGET_IC==5???)
   libcp16.a:5x_ftrunc.s@(_TARGET_IC==5???)
   libcp16.a:5x_fsqrt.s@(_TARGET_IC==5???)
   libcp16.a:5x_float.s@(_TARGET_IC==5???)
   libcp16.a:5x_ftrig.s@(_TARGET_IC==5???)
   libcp16.a:5x_poly.s@(_TARGET_IC==5???)
   libcp16.a:FilterIIR_Base.s@(_TARGET_IC==5???)
   libcp16.a:Filter_1stDenom.s@(_TARGET_IC==5???)
   libcp16.a:Filter_CommFact.s@(_TARGET_IC==5???)
   libcp16.a:Filter_FltFloatCoeff.s@(_TARGET_IC==5???)
   libcp16.a:Filter_CheckParameters.s@(_TARGET_IC==5???)
   libcp16.a:Filter_CheckStability.s@(_TARGET_IC==5???)
   libcp16.a:Filter_AP1.s@(_TARGET_IC==5???)
   libcp16.a:Filter_AP2.s@(_TARGET_IC==5???)
   libcp16.a:Filter_FLAT.s@(_TARGET_IC==5???)
   libcp16.a:Filter_HP1.s@(_TARGET_IC==5???)
   libcp16.a:Filter_HP2.s@(_TARGET_IC==5???)
   libcp16.a:Filter_HS1.s@(_TARGET_IC==5???)
   libcp16.a:Filter_LP1.s@(_TARGET_IC==5???)
   libcp16.a:Filter_LP2.s@(_TARGET_IC==5???)
   libcp16.a:Filter_LS1.s@(_TARGET_IC==5???)
   libcp16.a:Filter_PEQ.s@(_TARGET_IC==5???)
   libcp16.a:math_lin2db.s@(_TARGET_IC==5???)
   libcp16.a:math_LN2.s


Files linked by Module:

C:\Users\abhis\AppData\Local\Programs\DREAM\SamVS-C\lib\libcrtp16.a [2024-10-24 18:04 V5.135]:
	crtp16.s@(_TARGET_IC==5???)

main.o [2026-05-15 14:08]:
	main.s

dsp1.o [2026-05-15 14:08]:
	dsp1.s

dsp2.o [2026-05-15 14:08]:
	dsp2.s

dsp3.o [2026-05-15 14:08]:
	dsp3.s

dsp4.o [2026-05-15 16:56]:
	dsp4.s

midictrl.o [2026-05-15 14:08]:
	midictrl.s

BiquadCtrl.o [2026-05-15 14:08]:
	BiquadCtrl.s

mainbus.o [2026-05-15 14:08]:
	mainbus.s

dspDesignerWrappers.o [2026-05-15 14:08]:
	dspDesignerWrappers.s

C:\Users\abhis\AppData\Local\Programs\DREAM\SamVS-C\lib\libsys.a [2025-06-10 17:07 V4.650]:
	clearRAM.s@(_TARGET_IC==5???)
	clearRAMEx.s@(_TARGET_IC==5???)
	DSPresetAll.s@(_TARGET_IC==5???)
	DSPalloc.s@(_TARGET_IC==5???)
	DSProutEx.s@(_TARGET_IC==5???)
	DSProutInEx.s@(_TARGET_IC==5???)
	DSPGain.s@(_TARGET_IC==5???)
	convdB.s@(_TARGET_IC==5???)
	rom_LoadStartP24.s@_TARGET_IC==5???
	rom_StopP24.s@_TARGET_IC==5???
	rom_DIVIDE.s@_TARGET_IC==5???
	sys_CalcTimerReload.s@(_TARGET_IC==5???)
	sys_GetCrystalFreq.s@(_TARGET_IC==5???)
	sys_GetSystemFreq.s@(_TARGET_IC==5???)
	sys_SwitchPLL.s@_TARGET_IC==5???
	sys_GetSamplingRate.s@(_TARGET_IC==5???)
	sys_GetFSPinState.s

C:\Users\abhis\AppData\Local\Programs\DREAM\SamVS-C\lib\libUSB.a [2024-11-07 17:13 V4.643]:
	usb_Dde5xxxIoctl.s@(_TARGET_IC==5???)
	usb_Core.s@(_TARGET_IC==5???)
	usb_CoreHS.s@(_TARGET_IC==5???)
	usb_P16Poll.s@(_TARGET_IC==5???)
	usb_Poll.s@(_TARGET_IC==5???)
	usb_Enum.s@(_TARGET_IC==5???)
	usb_Init.s@(_TARGET_IC==5???)
	usb_Event.s@(_TARGET_IC==5???)
	usb_MidiClass.s@(_TARGET_IC==5???)
	usb_MidiSendWrp.s@(_TARGET_IC==5???)
	usb_DrvMidiClass.s@(_TARGET_IC==5???)
	usb_DrvSam5xxx.s@(_TARGET_IC==5???)

C:\Users\abhis\AppData\Local\Programs\DREAM\SamVS-C\lib\libFX5000.a [2025-07-29 18:00 V4.630]:
	MixPA.s@(_TARGET_IC==5???)
	StandardFunctions.s@(_TARGET_IC==5???)
	MixPA_XW.s@(_TARGET_IC==5???)

C:\Users\abhis\AppData\Local\Programs\DREAM\SamVS-C\lib\libcp16.a [2025-03-17 13:17 V4.632]:
	5x_fexp.s@(_TARGET_IC==5???)
	5x_ftrunc.s@(_TARGET_IC==5???)
	5x_fsqrt.s@(_TARGET_IC==5???)
	5x_float.s@(_TARGET_IC==5???)
	5x_ftrig.s@(_TARGET_IC==5???)
	5x_poly.s@(_TARGET_IC==5???)
	FilterIIR_Base.s@(_TARGET_IC==5???)
	Filter_1stDenom.s@(_TARGET_IC==5???)
	Filter_CommFact.s@(_TARGET_IC==5???)
	Filter_FltFloatCoeff.s@(_TARGET_IC==5???)
	Filter_CheckParameters.s@(_TARGET_IC==5???)
	Filter_CheckStability.s@(_TARGET_IC==5???)
	Filter_AP1.s@(_TARGET_IC==5???)
	Filter_AP2.s@(_TARGET_IC==5???)
	Filter_FLAT.s@(_TARGET_IC==5???)
	Filter_HP1.s@(_TARGET_IC==5???)
	Filter_HP2.s@(_TARGET_IC==5???)
	Filter_HS1.s@(_TARGET_IC==5???)
	Filter_LP1.s@(_TARGET_IC==5???)
	Filter_LP2.s@(_TARGET_IC==5???)
	Filter_LS1.s@(_TARGET_IC==5???)
	Filter_PEQ.s@(_TARGET_IC==5???)
	math_lin2db.s@(_TARGET_IC==5???)
	math_LN2.s


User Global Definitions
__dp0h = 0x8E8
XDATAPAGE = 0x8E8
XDATASTART = 0x8E80000
XDATABASE = 0x8E80000
BASEPAGE = 0x0
__intram_size = 0x9000
_nil = 0x0
__xsph = 0x8E8
__xspl = 0x8C8
FASTDATAPAGE = 0x8E8
__fastdata_start = 0x8C8
__heap_start = 0x8C8
__data24_start = 0x6000
__data24_page = 0x8E0
__data24_base = 0x8E06000
___trace_fifo = 0xFFFF

User Base Address Definitions
text = 0x400
const = 0x35B3
cnstu = 0x38D9
systab = 0x38F1
idata = 0x38F1
data = 0x0
iram = 0x23
iramh = 0x100
fixd = 0x0
fixi = 0x0
cst = 0x0
bss = 0x4A
stack = 0x4C8
fram = 0x8C8
txth = 0x393B
cnsth = 0x43F3

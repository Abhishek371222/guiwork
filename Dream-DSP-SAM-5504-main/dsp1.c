#include <system.h>
#include <libFX5000.h>
#include "dspDesigner.h"
#include "custom.h"
#ifndef    _SKIP_DDD_NRPN_CTRL
#include "midictrl.h"
#endif
#include "BiquadCtrl.h"
#include "shared_nrpn.h"

/*
 * DSP #1: Gain -> Biquad (3-band EQ) -> LevelDetect -> Compressor
 * Input:  DSP_IN[0]  (DAAD0L, left input)
 * Output: DSP_OUT[0] (routed to DABD0L via mainbus.c)
 */

#define BIQUAD_ITEMCOUNT            1
#define NUMBER_OF_BIQUAD_EXTRAFUNCTION 4
#define BIQUAD2BANDCOUNT            3    /* 3 bands of parametric EQ */

#ifndef    _SKIP_DDD_NRPN_CTRL

_FILTER_PARAM biquad1Parameters2[BIQUAD2BANDCOUNT];
WORD          biquad1Type2[BIQUAD2BANDCOUNT];
WORD          biquad1XoverType2[BIQUAD2BANDCOUNT];
DWORD         biquad1RawFrequency2[BIQUAD2BANDCOUNT];

BiquadParameters biquad1ParamAddr2 = {
    biquad1Parameters2,
    biquad1Type2,
    biquad1XoverType2,
    biquad1RawFrequency2,
    BIQUAD2BANDCOUNT
};

#endif /* _SKIP_DDD_NRPN_CTRL */

WORD dsp1pcs[6];

WORD dsp1InitAndRoute(void)
{
    WORD dspId;

    dspId = _MixPA_Init();
    if (dspId == -1) return 0;

#ifdef _customPreInitFunction1
    customPreInitFunction1(dspId);
#endif

    /* Process #1: Gain */
    dsp1pcs[1] = _MixPA_Gain_Allocate(dspId);
    _MixPA_SetProcIN( dspId, GAIN_SAMPLE_IN  | dsp1pcs[1], PCS_DSP_IN | 0 );
    _MixPA_SetProcOUT(dspId, GAIN_SAMPLE_OUT | dsp1pcs[1], PCS_NODE   | 0 );

    /* Process #2: Biquad (3-band) */
    dsp1pcs[2] = _MixPA_Biquad_Allocate(dspId, BIQUAD2BANDCOUNT);
    _MixPA_SetProcIN( dspId, BIQUAD_SAMPLE_IN  | dsp1pcs[2], PCS_NODE   | 0 );
    _MixPA_SetProcOUT(dspId, BIQUAD_SAMPLE_OUT | dsp1pcs[2], PCS_NODE   | 1 );

#ifndef    _SKIP_DDD_NRPN_CTRL
    InitDefaultPeqBands(&biquad1ParamAddr2, dspId, dsp1pcs[2]);
#endif

    /* Process #4: LevelDetect (sidechain for compressor) */
    dsp1pcs[4] = _MixPA_LevelDetect_Allocate(dspId);
    _MixPA_SetProcIN(dspId, LEVELDETECT_SAMPLE_IN | dsp1pcs[4], PCS_NODE | 1);

    /* Process #5: Compressor */
    dsp1pcs[5] = _MixPA_Compressor_Allocate(dspId);
    _MixPA_SetProcIN( dspId, COMPRESSOR_SAMPLE_IN  | dsp1pcs[5], PCS_NODE   | 1 );
    _MixPA_SetProcOUT(dspId, COMPRESSOR_SAMPLE_OUT | dsp1pcs[5], PCS_DSP_OUT| 0 );
    _MixPA_Compressor_ConnectLevel(dspId, dsp1pcs[5], dsp1pcs[4]);

#ifdef _customPostInitFunction1
    customPostInitFunction1(dspId);
#endif

    return dspId;
}

#ifndef    _SKIP_DDD_NRPN_CTRL

/*
 * NRPN command table for DSP #1.
 * Format: { NRPN_base, functionId }
 * Entries must be sorted ascending by NRPN_base for dichotomicSearch.
 */
const WORD nrpn1List[NUMBEROFCOMMAND1][2] =
{
    { 0x0100, 0x002F }, /* _MixPA_Gain_Value          */
    { 0x0101, 0x0030 }, /* _MixPA_Gain_Phase          */
    { 0x0200, 0x0000 }, /* _MixPA_Biquad_OnOff        */
    { 0x0201, 0x0001 }, /* _MixPA_Biquad_InGainPhase  */
    { 0x0202, 0x0002 }, /* _MixPA_Biquad_InGainValue  */
    { 0x0400, 0x0033 }, /* _MixPA_LevelDetect_Attack  */
    { 0x0401, 0x0034 }, /* _MixPA_LevelDetect_Release */
    { 0x0500, 0x0019 }, /* _MixPA_Compressor_GetGainReduction */
    { 0x0501, 0x001A }, /* _MixPA_Compressor_OnOff    */
    { 0x0502, 0x001B }, /* _MixPA_Compressor_Threshold*/
    { 0x0503, 0x001C }, /* _MixPA_Compressor_Ratio    */
    { 0x0504, 0x001D }, /* _MixPA_Compressor_Boost    */
    { 0x0505, 0x001E }  /* _MixPA_Compressor_BoostPhase */
};

#define NB_BIQUAD_COMMAND1 4

const BiquadParamsTable nrpn1BiquadTable[NB_BIQUAD_COMMAND1] =
{
    { 0x0203, 0x4003, &biquad1ParamAddr2 }, /* SetFilterType  band 0 */
    { 0x0222, 0x4004, &biquad1ParamAddr2 }, /* SetFilterQ     band 0 */
    { 0x0241, 0x4005, &biquad1ParamAddr2 }, /* SetFilterFreq  band 0 */
    { 0x0260, 0x4006, &biquad1ParamAddr2 }  /* SetFilterGain  band 0 */
};

WORD dsp1NrpnHandler(WORD nrpn, WORD dspId, WORD processId, DWORD value, WORD format)
{
#ifdef _customPreNrpnFunction1
    if (customPreNrpnFunction1(dspId, nrpn, &value, format))
        return 1;
#endif

    if (shared_NrpnDispatch_GainBiquadComp(
            nrpn, dspId, processId, value, format,
            dsp1pcs,
            nrpn1List,      NUMBEROFCOMMAND1,
            nrpn1BiquadTable, NB_BIQUAD_COMMAND1))
        return 1;

#ifdef _customPostNrpnFunction1
    {
        WORD val8bit = (WORD)(value >> 8);
        DWORD dvalue = (format == FORMAT_14BIT_PRECISION) ? ((DWORD)value << (DWORD)13) : value;
        if (customPostNrpnFunction1(dspId, nrpn, val8bit, value, dvalue))
            return 1;
    }
#endif

    return 0;
}

#endif /* _SKIP_DDD_NRPN_CTRL */
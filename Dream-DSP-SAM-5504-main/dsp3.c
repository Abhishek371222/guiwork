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
 * DSP #3: Gain -> Biquad (3-band EQ) -> LevelDetect -> Compressor
 * Input:  DSP_IN[0]  (DAAD0R, right input)
 * Output: DSP_OUT[0] (routed to DABD1L via mainbus.c)
 */

#define BIQUAD_ITEMCOUNT            1
#define NUMBER_OF_BIQUAD_EXTRAFUNCTION 4
#define BIQUAD2BANDCOUNT            3    /* 3 bands of parametric EQ */

#ifndef    _SKIP_DDD_NRPN_CTRL

_FILTER_PARAM biquad3Parameters2[BIQUAD2BANDCOUNT];
WORD          biquad3Type2[BIQUAD2BANDCOUNT];
WORD          biquad3XoverType2[BIQUAD2BANDCOUNT];
DWORD         biquad3RawFrequency2[BIQUAD2BANDCOUNT];

BiquadParameters biquad3ParamAddr2 = {
    biquad3Parameters2, biquad3Type2, biquad3XoverType2, biquad3RawFrequency2, BIQUAD2BANDCOUNT
};

#endif /* _SKIP_DDD_NRPN_CTRL */

WORD dsp3pcs[6];

WORD dsp3InitAndRoute(void)
{
    WORD dspId;

    dspId = _MixPA_Init();
    if (dspId == -1) return 0;

#ifdef _customPreInitFunction3
    customPreInitFunction3(dspId);
#endif

    /* Process #1: Gain */
    dsp3pcs[1] = _MixPA_Gain_Allocate(dspId);
    _MixPA_SetProcIN( dspId, GAIN_SAMPLE_IN  | dsp3pcs[1], PCS_DSP_IN | 0 );
    _MixPA_SetProcOUT(dspId, GAIN_SAMPLE_OUT | dsp3pcs[1], PCS_NODE   | 0 );

    /* Process #2: Biquad (3-band) */
    dsp3pcs[2] = _MixPA_Biquad_Allocate(dspId, BIQUAD2BANDCOUNT);
    _MixPA_SetProcIN( dspId, BIQUAD_SAMPLE_IN  | dsp3pcs[2], PCS_NODE   | 0 );
    _MixPA_SetProcOUT(dspId, BIQUAD_SAMPLE_OUT | dsp3pcs[2], PCS_NODE   | 1 );

#ifndef    _SKIP_DDD_NRPN_CTRL
    InitDefaultPeqBands(&biquad3ParamAddr2, dspId, dsp3pcs[2]);
#endif

    /* Process #4: LevelDetect (sidechain for compressor) */
    dsp3pcs[4] = _MixPA_LevelDetect_Allocate(dspId);
    _MixPA_SetProcIN(dspId, LEVELDETECT_SAMPLE_IN | dsp3pcs[4], PCS_NODE | 1);

    /* Process #5: Compressor */
    dsp3pcs[5] = _MixPA_Compressor_Allocate(dspId);
    _MixPA_SetProcIN( dspId, COMPRESSOR_SAMPLE_IN  | dsp3pcs[5], PCS_NODE    | 1 );
    _MixPA_SetProcOUT(dspId, COMPRESSOR_SAMPLE_OUT | dsp3pcs[5], PCS_DSP_OUT | 0 );
    _MixPA_Compressor_ConnectLevel(dspId, dsp3pcs[5], dsp3pcs[4]);

#ifdef _customPostInitFunction3
    customPostInitFunction3(dspId);
#endif

    return dspId;
}

#ifndef    _SKIP_DDD_NRPN_CTRL

/*
 * NRPN command table for DSP #3.
 * Entries sorted ascending by NRPN_base.
 */
const WORD nrpn3List[NUMBEROFCOMMAND3][2] =
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

#define NB_BIQUAD_COMMAND3 4

const BiquadParamsTable nrpn3BiquadTable[NB_BIQUAD_COMMAND3] =
{
    { 0x0203, 0x4003, &biquad3ParamAddr2 },
    { 0x0222, 0x4004, &biquad3ParamAddr2 },
    { 0x0241, 0x4005, &biquad3ParamAddr2 },
    { 0x0260, 0x4006, &biquad3ParamAddr2 }
};

WORD dsp3NrpnHandler(WORD nrpn, WORD dspId, WORD processId, DWORD value, WORD format)
{
#ifdef _customPreNrpnFunction3
    if (customPreNrpnFunction3(dspId, nrpn, &value, format))
        return 1;
#endif

    if (shared_NrpnDispatch_GainBiquadComp(
            nrpn, dspId, processId, value, format,
            dsp3pcs,
            nrpn3List,      NUMBEROFCOMMAND3,
            nrpn3BiquadTable, NB_BIQUAD_COMMAND3))
        return 1;

#ifdef _customPostNrpnFunction3
    {
        WORD val8bit = (WORD)(value >> 8);
        DWORD dvalue = (format == FORMAT_14BIT_PRECISION) ? ((DWORD)value << (DWORD)13) : value;
        if (customPostNrpnFunction3(dspId, nrpn, val8bit, value, dvalue))
            return 1;
    }
#endif

    return 0;
}

#endif /* _SKIP_DDD_NRPN_CTRL */
#include <system.h>
#include <libFX5000.h>
#include "dspDesigner.h"
#include "custom.h"
#ifndef    _SKIP_DDD_NRPN_CTRL
#include "midictrl.h"
#endif
#include "BiquadCtrl.h"
#include "memorymap.h"
#include "shared_nrpn.h"

/*
 * DSP #4: Delay -> Biquad (ch A) and Delay -> Biquad (ch B)
 * Input:  DSP_IN[0] (ch A), DSP_IN[1] (ch B)
 * Output: DSP_OUT[2] (DABD1L), DSP_OUT[3] (DABD1R) via mainbus.c
 *
 * Note: input assignments are the mirror of DSP #2 to implement the
 * crossover topology in mainbus.c.
 */

#define BIQUAD_ITEMCOUNT            2
#define NUMBER_OF_BIQUAD_EXTRAFUNCTION 4
#define BIQUAD2BANDCOUNT            3    /* 3 bands on channel A biquad */
#define BIQUAD4BANDCOUNT            3    /* 3 bands on channel B biquad */

#ifndef    _SKIP_DDD_NRPN_CTRL

_FILTER_PARAM biquad4Parameters2[BIQUAD2BANDCOUNT], biquad4Parameters4[BIQUAD4BANDCOUNT];
WORD          biquad4Type2[BIQUAD2BANDCOUNT],        biquad4Type4[BIQUAD4BANDCOUNT];
WORD          biquad4XoverType2[BIQUAD2BANDCOUNT],   biquad4XoverType4[BIQUAD4BANDCOUNT];
DWORD         biquad4RawFrequency2[BIQUAD2BANDCOUNT],biquad4RawFrequency4[BIQUAD4BANDCOUNT];

BiquadParameters biquad4ParamAddr2 = {
    biquad4Parameters2, biquad4Type2, biquad4XoverType2, biquad4RawFrequency2, BIQUAD2BANDCOUNT
};
BiquadParameters biquad4ParamAddr4 = {
    biquad4Parameters4, biquad4Type4, biquad4XoverType4, biquad4RawFrequency4, BIQUAD4BANDCOUNT
};

#endif /* _SKIP_DDD_NRPN_CTRL */

WORD dsp4pcs[5];

WORD dsp4InitAndRoute(void)
{
    WORD dspId;

    dspId = _MixPA_Init();
    if (dspId == -1) return 0;

#ifdef _customPreInitFunction4
    customPreInitFunction4(dspId);
#endif

    /* Process #1: Delay (ch A) */
    dsp4pcs[1] = _MixPA_Delay_Allocate(dspId);
    _MixPA_SetProcIN( dspId, DELAY_SAMPLE_IN  | dsp4pcs[1], PCS_DSP_IN | 0 );
    _MixPA_SetProcOUT(dspId, DELAY_SAMPLE_OUT | dsp4pcs[1], PCS_NODE   | 0 );
    _MixPA_Delay_SetStartAddr(dspId, dsp4pcs[1], MEMADDR_DSP04_PCS00PAR02);

    /* Process #3: Delay (ch B) */
    dsp4pcs[3] = _MixPA_Delay_Allocate(dspId);
    _MixPA_SetProcIN( dspId, DELAY_SAMPLE_IN  | dsp4pcs[3], PCS_DSP_IN | 1 );
    _MixPA_SetProcOUT(dspId, DELAY_SAMPLE_OUT | dsp4pcs[3], PCS_NODE   | 1 );
    _MixPA_Delay_SetStartAddr(dspId, dsp4pcs[3], MEMADDR_DSP04_PCS01PAR02);

    /* Process #2: Biquad (ch A, 3-band) */
    dsp4pcs[2] = _MixPA_Biquad_Allocate(dspId, BIQUAD2BANDCOUNT);
    _MixPA_SetProcIN( dspId, BIQUAD_SAMPLE_IN  | dsp4pcs[2], PCS_NODE    | 0 );
    _MixPA_SetProcOUT(dspId, BIQUAD_SAMPLE_OUT | dsp4pcs[2], PCS_DSP_OUT | 2 );

    /* Process #4: Biquad (ch B, 3-band) */
    dsp4pcs[4] = _MixPA_Biquad_Allocate(dspId, BIQUAD4BANDCOUNT);
    _MixPA_SetProcIN( dspId, BIQUAD_SAMPLE_IN  | dsp4pcs[4], PCS_NODE    | 1 );
    _MixPA_SetProcOUT(dspId, BIQUAD_SAMPLE_OUT | dsp4pcs[4], PCS_DSP_OUT | 3 );

#ifndef    _SKIP_DDD_NRPN_CTRL
    InitDefaultPeqBands(&biquad4ParamAddr2, dspId, dsp4pcs[2]);
    InitDefaultPeqBands(&biquad4ParamAddr4, dspId, dsp4pcs[4]);
#endif

#ifdef _customPostInitFunction4
    customPostInitFunction4(dspId);
#endif

    return dspId;
}

#ifndef    _SKIP_DDD_NRPN_CTRL

/*
 * NRPN command table for DSP #4.
 * Entries sorted ascending by NRPN_base.
 */
const WORD nrpn4List[NUMBEROFCOMMAND4][2] =
{
    { 0x0100, 0x001F }, /* _MixPA_Delay_OnOff        (proc 1) */
    { 0x0101, 0x0020 }, /* _MixPA_Delay_SetTime      (proc 1) */
    { 0x0102, 0x0021 }, /* _MixPA_Delay_OutGainValue (proc 1) */
    { 0x0103, 0x0022 }, /* _MixPA_Delay_OutGainPhase (proc 1) */
    { 0x0200, 0x0000 }, /* _MixPA_Biquad_OnOff       (proc 2) */
    { 0x0201, 0x0001 }, /* _MixPA_Biquad_InGainPhase (proc 2) */
    { 0x0202, 0x0002 }, /* _MixPA_Biquad_InGainValue (proc 2) */
    { 0x0300, 0x001F }, /* _MixPA_Delay_OnOff        (proc 3) */
    { 0x0301, 0x0020 }, /* _MixPA_Delay_SetTime      (proc 3) */
    { 0x0302, 0x0021 }, /* _MixPA_Delay_OutGainValue (proc 3) */
    { 0x0303, 0x0022 }, /* _MixPA_Delay_OutGainPhase (proc 3) */
    { 0x0400, 0x0000 }, /* _MixPA_Biquad_OnOff       (proc 4) */
    { 0x0401, 0x0001 }, /* _MixPA_Biquad_InGainPhase (proc 4) */
    { 0x0402, 0x0002 }  /* _MixPA_Biquad_InGainValue (proc 4) */
};

#define NB_BIQUAD_COMMAND4 8

const BiquadParamsTable nrpn4BiquadTable[NB_BIQUAD_COMMAND4] =
{
    { 0x0203, 0x4003, &biquad4ParamAddr4 },
    { 0x0222, 0x4004, &biquad4ParamAddr4 },
    { 0x0241, 0x4005, &biquad4ParamAddr4 },
    { 0x0260, 0x4006, &biquad4ParamAddr4 },
    { 0x0403, 0x4003, &biquad4ParamAddr2 },
    { 0x0422, 0x4004, &biquad4ParamAddr2 },
    { 0x0441, 0x4005, &biquad4ParamAddr2 },
    { 0x0460, 0x4006, &biquad4ParamAddr2 }
};

WORD dsp4NrpnHandler(WORD nrpn, WORD dspId, WORD processId, DWORD value, WORD format)
{
#ifdef _customPreNrpnFunction4
    if (customPreNrpnFunction4(dspId, nrpn, &value, format))
        return 1;
#endif

    if (shared_NrpnDispatch_DelayBiquad(
            nrpn, dspId, processId, value, format,
            dsp4pcs,
            nrpn4List,      NUMBEROFCOMMAND4,
            nrpn4BiquadTable, NB_BIQUAD_COMMAND4))
        return 1;

#ifdef _customPostNrpnFunction4
    {
        WORD val8bit = (WORD)(value >> 8);
        DWORD dvalue = (format == FORMAT_14BIT_PRECISION) ? ((DWORD)value << (DWORD)13) : value;
        if (customPostNrpnFunction4(dspId, nrpn, val8bit, value, dvalue))
            return 1;
    }
#endif

    return 0;
}

#endif /* _SKIP_DDD_NRPN_CTRL */

/* This file contains all dsp routing and initialization */
#include <system.h>
#include "dspDesigner.h"

/*
 * shared_nrpn.c is included here directly because the DSP Designer IDE
 * does not automatically add user-created .c files to the build.
 * Including it here ensures the two shared dispatch functions
 * (shared_NrpnDispatch_GainBiquadComp, shared_NrpnDispatch_DelayBiquad)
 * are compiled and available to the linker.
 */
#ifndef _SKIP_DDD_NRPN_CTRL
#include "shared_nrpn.c"
#endif

WORD OutBusOf( WORD DSPid, WORD out ){ return( 0x80+((DSPid-0x30)<<3)+out ); }

WORD dsp[NBDSPINITIALIZED];

/*
 * Signal flow (2-in / 4-out, 96kHz):
 *
 *   DAAD0L --> DSP1 (Gain -> EQ -> Compressor) -+-> DSP2 IN[0]: ch-A (Delay->EQ) --> DABD0L
 *                                                +-> DSP2 IN[1]: ch-B (Delay->EQ) --> DABD0R
 *
 *   DAAD0R --> DSP3 (Gain -> EQ -> Compressor) -+-> DSP4 IN[0]: ch-A (Delay->EQ) --> DABD1L
 *                                                +-> DSP4 IN[1]: ch-B (Delay->EQ) --> DABD1R
 *
 * Internal bus encoding in getBus():
 *   bit15 set  -> resolve via OutBusOf()
 *   bits[7:4]  -> index into dsp[] array (0-based)
 *   bits[2:0]  -> output port of that DSP
 *
 *   0x8000 = dsp[0] OUT[0] = DSP1 processed left signal
 *   0x8020 = dsp[2] OUT[0] = DSP3 processed right signal
 */

/*
 * Input routing  [dspIndex][inputPort]
 *
 * DSP1 IN[0] = DAAD0L  (left ADC)
 * DSP2 IN[0] = DSP1 OUT[0] (ch-A of left post-processor)
 * DSP2 IN[1] = DSP1 OUT[0] (ch-B of left post-processor, same source)
 * DSP3 IN[0] = DAAD0R  (right ADC)
 * DSP4 IN[0] = DSP3 OUT[0] (ch-A of right post-processor)
 * DSP4 IN[1] = DSP3 OUT[0] (ch-B of right post-processor, same source)
 */
const WORD dspRouting_In[4][8] =
{
    /* DSP1 */ { DAAD0L, 0,      0, 0, 0, 0, 0, 0 },
    /* DSP2 */ { 0x8000, 0x8000, 0, 0, 0, 0, 0, 0 },
    /* DSP3 */ { DAAD0R, 0,      0, 0, 0, 0, 0, 0 },
    /* DSP4 */ { 0x8020, 0x8020, 0, 0, 0, 0, 0, 0 }
};

/*
 * Output routing  [dspIndex][outputPort]
 *
 * DSP1 OUT[0] -> internal bus only (no direct DAC, feeds DSP2)
 * DSP2 OUT[0] -> DABD0L  (left output A)
 * DSP2 OUT[1] -> DABD0R  (left output B)
 * DSP3 OUT[0] -> internal bus only (no direct DAC, feeds DSP4)
 * DSP4 OUT[2] -> DABD1L  (right output A)  -- dsp4.c routes biquad to PCS_DSP_OUT|2
 * DSP4 OUT[3] -> DABD1R  (right output B)  -- dsp4.c routes biquad to PCS_DSP_OUT|3
 */
const WORD dspRouting_Out[4][8] =
{
    /* DSP1 */ { -1,     -1,     -1,     -1, -1, -1, -1, -1 },
    /* DSP2 */ { DABD0L, DABD0R, -1,     -1, -1, -1, -1, -1 },
    /* DSP3 */ { -1,     -1,     -1,     -1, -1, -1, -1, -1 },
    /* DSP4 */ { -1,     -1, DABD1L, DABD1R, -1, -1, -1, -1 }
};

WORD getBus( WORD bus )
{
    if ( bus == (WORD)-1 ) return (WORD)-1;
    if ( bus & (1<<15) )
        return OutBusOf( dsp[(bus>>4)&0xF], bus&0x7 );
    else
        return bus;
}

void dspDesigner_InitAndRoute (void)
{
    WORD rout[8], i, j;
    WORD dspIndex[4] = { 0, 1, 2, 3 };

    dsp[0] = dsp1InitAndRoute();
    dsp[1] = dsp2InitAndRoute();
    dsp[2] = dsp3InitAndRoute();
    dsp[3] = dsp4InitAndRoute();

    /* Apply routing and start each DSP */
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 8; j++)
            rout[j] = getBus( dspRouting_In[i][j] );
        _DSProutInEx( dsp[dspIndex[i]], rout );

        for (j = 0; j < 8; j++)
            rout[j] = getBus( dspRouting_Out[i][j] );
        _DSProutEx( dsp[dspIndex[i]], rout );

        _StartDSP( dsp[dspIndex[i]] );
    }
}
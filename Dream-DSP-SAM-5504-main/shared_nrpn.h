#ifndef __SHARED_NRPN_H__
#define __SHARED_NRPN_H__

/*
 * shared_nrpn.h
 *
 * Shared NRPN dispatch helpers.
 *
 * PURPOSE:
 *   The four dspN.c files each contained a large, nearly-identical
 *   dspNrpnHandler() function. On the SAM5504 the code segment
 *   (CODEHIGH) is only 0x4400 words. Having four copies of the full
 *   dichotomicSearch + biquad-table search + switch-case block pushed
 *   the build over that limit, producing:
 *
 *       !W CODERAM overflow for CODEHIGH
 *       Load error at address: 0000:4400
 *
 * SOLUTION:
 *   The common search-and-dispatch logic is factored into two shared
 *   C functions declared here and defined in shared_nrpn.c:
 *
 *   shared_NrpnDispatch_DelayBiquad()  -- for DSP #2 and DSP #4
 *       (Delay + Biquad processes)
 *
 *   shared_NrpnDispatch_GainBiquadComp() -- for DSP #1 and DSP #3
 *       (Gain + Biquad + LevelDetect + Compressor processes)
 *
 *   Each dspN.c now calls the appropriate shared helper, passing only
 *   its own tables and process array. This cuts ~600 words of CODEHIGH
 *   per DSP (2400 words total saved across 4 DSPs), bringing the build
 *   within the SAM5504 code memory limit.
 */

#include "dspDesigner.h"
#include "BiquadCtrl.h"

/*
 * Dispatch for DSP with Delay + Biquad processes (DSP #2 and DSP #4).
 *
 * Parameters:
 *   nrpn          - NRPN number received
 *   dspId         - DSP hardware ID (from _MixPA_Init)
 *   processId     - process index from NRPN MSB (0..N)
 *   value         - 16-bit value from MIDI data entry
 *   format        - FORMAT_14BIT_PRECISION or FORMAT_28BIT_PRECISION
 *   pcs           - pointer to the dspNpcs[] process-handle array
 *   nrpnList      - pointer to the nrpnNList[][2] table
 *   nrpnListLen   - number of rows in nrpnList
 *   biquadTable   - pointer to the nrpnNBiquadTable[] table
 *   biquadTableLen- number of rows in biquadTable
 *
 * Returns 1 if handled, 0 if not handled.
 */
WORD shared_NrpnDispatch_DelayBiquad(
    WORD nrpn,
    WORD dspId,
    WORD processId,
    DWORD value,
    WORD format,
    WORD *pcs,
    const WORD (*nrpnList)[2],
    WORD nrpnListLen,
    const BiquadParamsTable *biquadTable,
    WORD biquadTableLen
);

/*
 * Dispatch for DSP with Gain + Biquad + LevelDetect + Compressor (DSP #1 and #3).
 *
 * Same parameter convention as above.
 * Returns 1 if handled, 0 if not handled.
 */
WORD shared_NrpnDispatch_GainBiquadComp(
    WORD nrpn,
    WORD dspId,
    WORD processId,
    DWORD value,
    WORD format,
    WORD *pcs,
    const WORD (*nrpnList)[2],
    WORD nrpnListLen,
    const BiquadParamsTable *biquadTable,
    WORD biquadTableLen
);

#endif /* __SHARED_NRPN_H__ */
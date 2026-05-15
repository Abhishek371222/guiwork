/*
 * shared_nrpn.c
 *
 * Single implementation of the NRPN search-and-dispatch logic used by
 * all four DSP handler files (dsp1.c .. dsp4.c).
 *
 * See shared_nrpn.h for full explanation of why this file exists.
 *
 * COPYRIGHT (c) DREAM S.A.S. 2025 / user project
 */

#include <system.h>
#include <libFX5000.h>
#include "dspDesigner.h"
#include "BiquadCtrl.h"
#include "midictrl.h"
#include "shared_nrpn.h"

/* -----------------------------------------------------------------------
 * Internal helper: search nrpnList and biquadTable, fill out resolved
 * functionId, index, processId, and (for biquad hits) theBiquad /
 * updateCoeffFunc.
 *
 * Returns 1 if found (and *outFunctionId / *outIndex / *outProcessId set),
 * 0 if not found.
 * ----------------------------------------------------------------------- */
static WORD resolveNrpn(
    WORD nrpn,
    WORD processId,
    WORD *pcs,
    const WORD (*nrpnList)[2],
    WORD nrpnListLen,
    const BiquadParamsTable *biquadTable,
    WORD biquadTableLen,
    /* outputs */
    WORD *outFunctionId,
    WORD *outIndex,
    WORD *outProcessId,
    BiquadParameters **outBiquad,
    UpdateCoeffCallback *outCb
)
{
    WORD i;

    /* ---- search plain NRPN list ---- */
    i = dichotomicSearch( _cptr32(nrpnList), 2, 0, nrpnListLen, nrpn );

    if (i & (1<<15))
    {
        i &= 0x7FFF;
        if (i == nrpnListLen)
            i--;
        else if (nrpn < nrpnList[i][0])
            i--;
        if (!(nrpnList[i][1] & (1<<14)))
            i = (WORD)-1;
    }

    if (i != (WORD)-1)
    {
        *outIndex      = nrpn - nrpnList[i][0];
        *outFunctionId = nrpnList[i][1];
        *outProcessId  = pcs[processId];
        return 1;
    }

    /* ---- search biquad table ---- */
    i = dichotomicSearch( _cptr32(biquadTable), 3, 0, biquadTableLen, nrpn );

    if (i != (WORD)-1)
    {
        if (i & (1<<15))
        {
            i &= 0x7FFF;
            if (i >= biquadTableLen)
                i--;
            else if (nrpn < biquadTable[i].nrpn)
                i--;
            if (!(biquadTable[i].functionId & (1<<14)))
                i = (WORD)-1;
        }

        if (i != (WORD)-1)
        {
            *outIndex      = nrpn - biquadTable[i].nrpn;
            *outFunctionId = biquadTable[i].functionId;
            *outProcessId  = pcs[processId];
            *outBiquad     = (BiquadParameters *)biquadTable[i].parametersTable;
            outCb->BIQUAD_UpdateCoeffFuncPtr = _cptr32( &_MixPA_Biquad_UpdateCoeff );
            outCb->BIQUAD_FlatFuncPtr        = _cptr32( &_MixPA_Biquad_Flat );
            return 1;
        }
    }

    return 0;
}

/* -----------------------------------------------------------------------
 * Dispatch for DSP with Delay + Biquad processes (DSP #2 and DSP #4)
 * ----------------------------------------------------------------------- */
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
)
{
    WORD functionId = 0, index = 0, resolvedProcess = 0, val8bit;
    DWORD dvalue;
    BiquadParameters *theBiquad = 0;
    UpdateCoeffCallback updateCoeffFunc;

    val8bit = (WORD)(value >> 8);
    dvalue  = (format == FORMAT_14BIT_PRECISION) ? ((DWORD)value << (DWORD)13) : value;

    if (!resolveNrpn(nrpn, processId, pcs,
                     nrpnList, nrpnListLen,
                     biquadTable, biquadTableLen,
                     &functionId, &index, &resolvedProcess,
                     &theBiquad, &updateCoeffFunc))
        return 0;

    switch (functionId)
    {
        /* Delay */
        case 0x001F: _MixPA_Delay_OnOff(      dspId, resolvedProcess, val8bit ); return 1;
        case 0x0020: _MixPA_Delay_SetTime(     dspId, resolvedProcess, dvalue  ); return 1;
        case 0x0021: _MixPA_Delay_OutGainValue(dspId, resolvedProcess, value   ); return 1;
        case 0x0022: _MixPA_Delay_OutGainPhase(dspId, resolvedProcess, val8bit ); return 1;
        /* Biquad */
        case 0x0000: _MixPA_Biquad_OnOff(      dspId, resolvedProcess, val8bit ); return 1;
        case 0x0001: _MixPA_Biquad_InGainPhase(dspId, resolvedProcess, val8bit ); return 1;
        case 0x0002: _MixPA_Biquad_InGainValue(dspId, resolvedProcess, value   ); return 1;
        case 0x4003: SetFilterType( &updateCoeffFunc, theBiquad, dspId, resolvedProcess, index, val8bit ); return 1;
        case 0x4004: SetFilterQ(    &updateCoeffFunc, theBiquad, dspId, resolvedProcess, index, value   ); return 1;
        case 0x4005: SetFilterFreq( &updateCoeffFunc, theBiquad, dspId, resolvedProcess, index, dvalue  ); return 1;
        case 0x4006: SetFilterGain( &updateCoeffFunc, theBiquad, dspId, resolvedProcess, index, value   ); return 1;
        default: break;
    }
    return 0;
}

/* -----------------------------------------------------------------------
 * Dispatch for DSP with Gain + Biquad + LevelDetect + Compressor (DSP #1, #3)
 * ----------------------------------------------------------------------- */
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
)
{
    WORD functionId = 0, index = 0, resolvedProcess = 0, val8bit;
    DWORD dvalue;
    BiquadParameters *theBiquad = 0;
    UpdateCoeffCallback updateCoeffFunc;

    val8bit = (WORD)(value >> 8);
    dvalue  = (format == FORMAT_14BIT_PRECISION) ? ((DWORD)value << (DWORD)13) : value;

    if (!resolveNrpn(nrpn, processId, pcs,
                     nrpnList, nrpnListLen,
                     biquadTable, biquadTableLen,
                     &functionId, &index, &resolvedProcess,
                     &theBiquad, &updateCoeffFunc))
        return 0;

    switch (functionId)
    {
        /* Gain */
        case 0x002F: _MixPA_Gain_Value( dspId, resolvedProcess, value   ); return 1;
        case 0x0030: _MixPA_Gain_Phase( dspId, resolvedProcess, val8bit ); return 1;
        /* Biquad */
        case 0x0000: _MixPA_Biquad_OnOff(      dspId, resolvedProcess, val8bit ); return 1;
        case 0x0001: _MixPA_Biquad_InGainPhase(dspId, resolvedProcess, val8bit ); return 1;
        case 0x0002: _MixPA_Biquad_InGainValue(dspId, resolvedProcess, value   ); return 1;
        case 0x4003: SetFilterType( &updateCoeffFunc, theBiquad, dspId, resolvedProcess, index, val8bit ); return 1;
        case 0x4004: SetFilterQ(    &updateCoeffFunc, theBiquad, dspId, resolvedProcess, index, value   ); return 1;
        case 0x4005: SetFilterFreq( &updateCoeffFunc, theBiquad, dspId, resolvedProcess, index, dvalue  ); return 1;
        case 0x4006: SetFilterGain( &updateCoeffFunc, theBiquad, dspId, resolvedProcess, index, value   ); return 1;
        /* LevelDetect */
        case 0x0033: _MixPA_LevelDetect_Attack( dspId, resolvedProcess, value   ); return 1;
        case 0x0034: _MixPA_LevelDetect_Release(dspId, resolvedProcess, value   ); return 1;
        /* Compressor */
        case 0x0019: sendSysExMessage( value, _MixPA_Compressor_GetGainReduction(dspId, resolvedProcess) ); return 1;
        case 0x001A: _MixPA_Compressor_OnOff(      dspId, resolvedProcess, val8bit ); return 1;
        case 0x001B: _MixPA_Compressor_Threshold(  dspId, resolvedProcess, value   ); return 1;
        case 0x001C: _MixPA_Compressor_Ratio(      dspId, resolvedProcess, value   ); return 1;
        case 0x001D: _MixPA_Compressor_Boost(      dspId, resolvedProcess, value   ); return 1;
        case 0x001E: _MixPA_Compressor_BoostPhase( dspId, resolvedProcess, val8bit ); return 1;
        default: break;
    }
    return 0;
}
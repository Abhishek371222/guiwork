// This file can be modified by user and will not be overwritten by Dream DSP Designer.
#ifndef __CUSTOM_H__
#define __CUSTOM_H__

//DSP#1 custom functions, uncomment the '#define' line where you want to add custom code, and implement the function below the '#define' in a separate C file
//#define _customPreInitFunction1
WORD customPreInitFunction1( WORD dspId );
//#define _customPostInitFunction1
WORD customPostInitFunction1( WORD dspId );
//#define _customPreNrpnFunction1
WORD customPreNrpnFunction1( WORD dspId, WORD nrpn, DWORD *pval, WORD format );
//#define _customPostNrpnFunction1
WORD customPostNrpnFunction1( WORD dspId, WORD nrpn, WORD val8bit, WORD value, DWORD dvalue );

//DSP#2 custom functions, uncomment the '#define' line where you want to add custom code, and implement the function below the '#define' in a separate C file
//#define _customPreInitFunction2
WORD customPreInitFunction2( WORD dspId );
//#define _customPostInitFunction2
WORD customPostInitFunction2( WORD dspId );
//#define _customPreNrpnFunction2
WORD customPreNrpnFunction2( WORD dspId, WORD nrpn, DWORD *pval, WORD format );
//#define _customPostNrpnFunction2
WORD customPostNrpnFunction2( WORD dspId, WORD nrpn, WORD val8bit, WORD value, DWORD dvalue );

//DSP#3 custom functions, uncomment the '#define' line where you want to add custom code, and implement the function below the '#define' in a separate C file
//#define _customPreInitFunction3
WORD customPreInitFunction3( WORD dspId );
//#define _customPostInitFunction3
WORD customPostInitFunction3( WORD dspId );
//#define _customPreNrpnFunction3
WORD customPreNrpnFunction3( WORD dspId, WORD nrpn, DWORD *pval, WORD format );
//#define _customPostNrpnFunction3
WORD customPostNrpnFunction3( WORD dspId, WORD nrpn, WORD val8bit, WORD value, DWORD dvalue );

//DSP#4 custom functions, uncomment the '#define' line where you want to add custom code, and implement the function below the '#define' in a separate C file
//#define _customPreInitFunction4
WORD customPreInitFunction4( WORD dspId );
//#define _customPostInitFunction4
WORD customPostInitFunction4( WORD dspId );
//#define _customPreNrpnFunction4
WORD customPreNrpnFunction4( WORD dspId, WORD nrpn, DWORD *pval, WORD format );
//#define _customPostNrpnFunction4
WORD customPostNrpnFunction4( WORD dspId, WORD nrpn, WORD val8bit, WORD value, DWORD dvalue );

// BiquadCtrl.c custom functions, uncomment the '#define' line where you want to add custom code, and implement the function below the '#define' in a separate C file
//#define _customFilterFreq
void customFilterFreq( _FILTER_PARAM *iirFP, BiquadParameters *biquadParam, WORD dspId, WORD procId, WORD band, DWORD v );

//#define _customFilterGain
void customFilterGain( _FILTER_PARAM *iirFP, BiquadParameters *biquadParam, WORD dspId, WORD procId, WORD band, DWORD v );

//#define _customFilterQ
void customFilterQ( _FILTER_PARAM *iirFP, BiquadParameters *biquadParam, WORD dspId, WORD procId, WORD band, DWORD v );



#endif //__CUSTOM_H__
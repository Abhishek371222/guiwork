#include <system.h>
#include <libfx5000.h>
#include "dspDesigner.h"
#include "BiquadCtrl.h"
#include "custom.h"

// Butterworth Q table
const FLOAT TAB_BUTTERWORTH[5][4] = {	 {0x3f3504f3 ,0x00000000 ,0x00000000 ,0x00000000 },
								 {0x3f800000 ,0x00000001 ,0x00000000 ,0x00000000 },
								 {0x3f0a8c15 ,0x3fa73b64 ,0x00000000 ,0x00000000 },
								 {0x3F0487FD ,0x3F350481 ,0x3FF74BC7 ,0x00000000 },
								 {0x3f028241 ,0x3f19eecc ,0x3f665fd9 ,0x402404ea}};

// Bessel FSF/Q table
const FLOAT TAB_BESSEL[5][8] = { 	{0x3fa30553 ,0x3f13c9ef ,0x00000000 ,0x00000000 ,0x00000000  ,0x00000000  ,0x00000000  ,0x00000000  },
							{0x3fb9e83e ,0x3f30e560 ,0x3fa9db23 ,0x00000001 ,0x00000000  ,0x00000000  ,0x00000000  ,0x00000000  },
							{0x3fb5a858 ,0x3f059b3d ,0x3fcbac71 ,0x3f4e353f ,0x00000000  ,0x00000000  ,0x00000000  ,0x00000000  },
							{0x3FCD9168 ,0x3F02A305 ,0x3FD87C85 ,0x3F1C779A ,0x3FF41BDA  ,0x3F82FEC5  ,0x00000000  ,0x00000000  },
 							{0x3fe45048 ,0x3f018937 ,0x400c7fcc ,0x3f9ce704 ,0x3ffac3ca  ,0x3f35fd8b  ,0x3feb367a  ,0x3f0f41f2 }};

// Linkwitz-Riley Q table
const FLOAT TAB_LINKRILEY[4][4] = {{0x3f000000 ,0x00000000 ,0x00000000 ,0x00000000  },
							 {0x3f3504f3 ,0x3f3504f3 ,0x00000000 ,0x00000000  },
							 {0x3f000000 ,0x3f800000 ,0x3f800000 ,0x00000000  },
							 {0x3f0a8c15 ,0x3fa73b64 ,0x3f0a8c15 ,0x3fa73b64}};

// If "isLoadingPreset" flag is set a Biquad filter band will only be updated once after all filter parameters are loaded.
// In case of single band filters (PEQ etc.) it is done after having received the filter Gain parameter (is last in preset storing order).
// In case of Xover filters it is done after having received the filter Frequency parameter (Q & Gain are ignored).
static WORD isLoadingPreset = 0;

// Functions used for biquad preset loading
void biquadSetLoadingPreset( WORD value )
{
	isLoadingPreset = value;
}

#define XOVER_READ_BANDNB	255	//special value for "maxBand" used for special case of reading number of XOver filter bands used

// Butterworth filter type
WORD SetButterworthType( XOverButterworthType ButterSlope, WORD maxBand, WORD ButterFilter, FLOAT f0, _FILTER_PARAM *FilterParam, WORD *FilterType, WORD *xoverType )
{
	WORD bandCount,i;
	// switch case ButterSlope
	switch (ButterSlope)
	{
		case XOVER_BUTTER_1ST:
			bandCount = 1;// number of 2nd order band used.
			break;
			
		case XOVER_BUTTER_2ND:
			bandCount = 1;// number of 2nd order band used.
			break;
			
		case XOVER_BUTTER_3RD:	
			bandCount = 2;// number of 2nd order band used.
			break;
			
		case XOVER_BUTTER_4TH:	
			bandCount = 2;// number of 2nd order band used.
			break;
			
		case XOVER_BUTTER_6TH:	
			bandCount = 3;// number of 2nd order band used.
			break;

		case XOVER_BUTTER_8TH:	
			bandCount = 4;// number of 2nd order band used.
			break;

		default : return -1;	// invalid type
	}
	
	if ( maxBand == XOVER_READ_BANDNB )		//special function call to only get number of bands used
		return bandCount;

	if (bandCount>maxBand)
		return -1;		// not fit

	for ( i = 0; i < bandCount; i++ )
	{
		xoverType[i] = (i<<8) | ButterSlope;
		// Load *FilterParam with Butterworth Q table and f0.
		FilterParam[i].f0 = f0;
		FilterParam[i].Q = TAB_BUTTERWORTH[(ButterSlope&7)-2][i]; 
		// Load Type Filter ( 0 = LP ; 1 = HP )
		// Special case for 3rd order Butter Filter
		if( FilterParam[i].Q==(FLOAT)1) // 1st order filter?
		{
			if ( ButterFilter == XOVER_LP) FilterType[i] = (FilterType[i]&(BIQUAD_EX_FLAG)) | UPDATE_LP1_FILTER;
			else FilterType[i] = (FilterType[i]&(BIQUAD_EX_FLAG)) | UPDATE_HP1_FILTER;
		}
		else
		{
			if ( ButterFilter == XOVER_LP) FilterType[i] = (FilterType[i]&(BIQUAD_EX_FLAG)) | UPDATE_LP2_FILTER;
			else FilterType[i] = (FilterType[i]&(BIQUAD_EX_FLAG)) | UPDATE_HP2_FILTER;
		}
	}

	// return number of 2nd order band which will be used 
	return bandCount; 
}

FLOAT GetBesselFrequency( XOverBesselType BesselSlope, WORD FilterType, WORD band, FLOAT f0 )
{
	FLOAT fret;
	if ( ( (FilterType&(BIQUAD_MSB_USED-1)) == UPDATE_HP2_FILTER ) || ( (FilterType&(BIQUAD_MSB_USED-1)) == UPDATE_HP1_FILTER) ) 
		fret = _fdiv( f0, (TAB_BESSEL[(BesselSlope&7)-1][(2*band)]) );  // f0 = f0/FSF for highpass 
	else 
		fret = _fmul( f0, (TAB_BESSEL[(BesselSlope&7)-1][(2*band)]) );  // f0 = f0*FSF for lowpass
	return fret;
}

// Bessel filter type
WORD SetBesselType( XOverBesselType BesselSlope, WORD maxBand, WORD BesselFilter, FLOAT f0, _FILTER_PARAM *FilterParam, WORD *FilterType, WORD *xoverType )
{
	WORD bandCount,i;
	// switch case BesselSlope
	switch (BesselSlope)
	{
		case XOVER_BESSEL_2ND:
			bandCount = 1;// number of 2nd order band used.
			break;

		case XOVER_BESSEL_3RD:
			bandCount = 2;// number of 2nd order band used.
			break;
			
		case XOVER_BESSEL_4TH:	
			bandCount = 2;// number of 2nd order band used.
			break;
			
		case XOVER_BESSEL_6TH:	
			bandCount = 3;// number of 2nd order band used.
			break;

		case XOVER_BESSEL_8TH:	
			bandCount = 4;// number of 2nd order band used.
			break;

		default : return -1;	// invalid type
	}
	
	if ( maxBand == XOVER_READ_BANDNB )		//special function call to only get number of bands used
		return bandCount;

	if (bandCount>maxBand)
		return -1;		// not fit

	for ( i = 0; i < bandCount; i++ ) 
	{
		xoverType[i] = (i<<8) | BesselSlope;
		FilterParam[i].f0 = f0;
		// Load *FilterParam with Bessel Q table
		FilterParam[i].Q = TAB_BESSEL[(BesselSlope&7)-1][(2*i)+1]; 
		if( FilterParam[i].Q==(FLOAT)1) // 1st order filter?
		{
			if ( BesselFilter == XOVER_LP) 
				FilterType[i] = (FilterType[i]&(BIQUAD_EX_FLAG)) | UPDATE_LP1_FILTER;
			else 
				FilterType[i] = (FilterType[i]&(BIQUAD_EX_FLAG)) | UPDATE_HP1_FILTER;
		}
		else
		{
			if ( BesselFilter == XOVER_LP) 
				FilterType[i] = (FilterType[i]&(BIQUAD_EX_FLAG)) | UPDATE_LP2_FILTER;
			else 
				FilterType[i] = (FilterType[i]&(BIQUAD_EX_FLAG)) | UPDATE_HP2_FILTER;
		}		
	}

	// return number of 2nd order band which will be used 
	return bandCount; 		
}

// Linkwitz-Riley type
WORD SetLinkwitzRileyType( XOverLinkwitzRileyType LinkwitzRileySlope, WORD maxBand, WORD LinkwitzRileyFilter, FLOAT f0, _FILTER_PARAM *FilterParam, WORD *FilterType, WORD *xoverType )
{
	WORD bandCount,i;
	
	// switch case LinkwitzRileySlope
	switch (LinkwitzRileySlope)
	{
		case XOVER_LINKRILEY_2ND:
			bandCount = 1;// number of 2nd order band used.
			break;
			
		case XOVER_LINKRILEY_4TH:	
			bandCount = 2;// number of 2nd order band used.
			break;
			
		case XOVER_LINKRILEY_6TH:	
			bandCount = 3;// number of 2nd order band used.
			break;

		case XOVER_LINKRILEY_8TH:	
			bandCount = 4;// number of 2nd order band used.
			break;

		default : return -1;	// invalid type
	}

	if ( maxBand == XOVER_READ_BANDNB )		//special function call to only get number of bands used
		return bandCount;

	if (bandCount>maxBand)
		return -1;		// not fit

	for ( i = 0; i < bandCount; i++ ) 
	{
		xoverType[i] = (i<<8) | LinkwitzRileySlope;
		// Load *FilterParam with LinkwitzRiley Q table and f0.
		FilterParam[i].f0 = f0;
		FilterParam[i].Q = TAB_LINKRILEY[(LinkwitzRileySlope&7)-1][i]; 
		if( FilterParam[i].Q==(FLOAT)1) // 1st order filter?
		{
			if ( LinkwitzRileyFilter == XOVER_LP) FilterType[i] = (FilterType[i]&(BIQUAD_EX_FLAG)) | UPDATE_LP1_FILTER;
			else FilterType[i] = (FilterType[i]&(BIQUAD_EX_FLAG)) | UPDATE_HP1_FILTER;
		}
		else
		{
			if ( LinkwitzRileyFilter == XOVER_LP) FilterType[i] = (FilterType[i]&(BIQUAD_EX_FLAG)) | UPDATE_LP2_FILTER;
			else FilterType[i] = (FilterType[i]&(BIQUAD_EX_FLAG)) | UPDATE_HP2_FILTER;
		}
	}

	// return number of 2nd order band which will be used 
	return bandCount; 
}

WORD Set1stOrder( WORD F1stOrderType, WORD maxBand, FLOAT f0, _FILTER_PARAM *FilterParam, WORD *FilterType, WORD *xoverType )
{
	WORD i;

	if ( maxBand == XOVER_READ_BANDNB )		//special function call to only get number of bands used
		return 1;

	if (maxBand<1)
		return -1;		// not fit

	for ( i = 0; i < 1; i++ )
	{
		xoverType[i] = (i<<8) | F1stOrderType;
		FilterParam[i].f0 = f0;
		FilterParam[i].Q = 0; 
		if ( F1stOrderType == XOVER_LP) 
			FilterType[i] = (FilterType[i]&(BIQUAD_EX_FLAG)) | UPDATE_LP1_FILTER;
		else
			FilterType[i] = (FilterType[i]&(BIQUAD_EX_FLAG)) | UPDATE_HP1_FILTER;
	}
	FilterParam[0].Q = 1;
	return 1; // 1 band
}

WORD SetFlat( WORD maxBand, _FILTER_PARAM *FilterParam, WORD *xoverTypeTable )
{
	WORD i;

	if ( maxBand == XOVER_READ_BANDNB )		//special function call to only get number of bands used
		return 1;

	for ( i = 0; i < 1; i++ )
	{
		// Load *FilterParam with Q=0.
		xoverTypeTable[i] = 0;
		FilterParam[i].Q = 0; 
	}
	return 1; // 1 band
}

/*-------------------------------------------------------------------------------------------------------
Function: SetXOverFilterType

Input: 
	xOverLPHP: 	xover filter type low pass or high pass(XOVER_LP | XOVER_HP)
	maxBand: 	number of band allowed for the xover filter
	xoverType:	xover filter type, subtype value from following enums: XOverButterworthType, XOverBesselworthType, XOverLinkwitzRileyType.
	
Output:
	iirFP & FP are updated with correct parameters according to input parameters.
	return -1 if failed, else number of bands used.
*/
WORD SetXOverFilterType( WORD xOverLPHP, WORD maxBand, WORD xoverType, _FILTER_PARAM *iirFP, WORD *FP, WORD *xoverTypeTable )
{
	WORD ret=(WORD)-1;
	if ( !xoverType )	ret=SetFlat( maxBand, iirFP, xoverTypeTable ); // set Flat
	else if (xoverType == 1)	ret=Set1stOrder( xOverLPHP, maxBand, iirFP->f0, iirFP, FP, xoverTypeTable );
	else if ( xoverType & (1<<3) ) ret=SetBesselType( xoverType, maxBand, xOverLPHP, iirFP->f0, iirFP, FP, xoverTypeTable );
	else if ( xoverType & (1<<4) ) ret=SetLinkwitzRileyType( xoverType, maxBand, xOverLPHP, iirFP->f0, iirFP, FP, xoverTypeTable );
	else ret=SetButterworthType( xoverType, maxBand, xOverLPHP, iirFP->f0, iirFP, FP, xoverTypeTable );
	return ret;
}
WORD FilterCoefficientsUpdate( WORD *FilterType, _FILTER_PARAM *iirFP, _BIQUAD_FIXEDCOEFF *iirCoeff )
{
	WORD stable, realFilterType;
	_BIQUAD_COEFF iirFloatCoeff;	// where to store coefficient
	PTR32 bqFilt;

	realFilterType = (*FilterType) & ( BIQUAD_MSB_USED - 1 );
	bqFilt = _BIQUAD_FLTPTR(_Biquad_FilterFLAT);

	if ( !iirFP->Q && realFilterType ) // power-up init condition: allow to set FLAT
	{
		if ( !(realFilterType & (1<<6)) ) // not crossover filter
		{
			if ( (realFilterType != 2)&&(realFilterType != 3)&&(realFilterType != 6)&&(realFilterType != 7) )
			{
				iirFP->Q = FLOAT_ONE; // if was not set, set 1.0 by default
				bqFilt = _BIQUAD_FLTPTR(_Biquad_FilterPEQ);
				*FilterType = ((*FilterType)&BIQUAD_EX_FLAG) | 1; // PEQ by default
			}
		}
	}

	realFilterType = *FilterType;
	switch ((realFilterType)&((1<<6)-1))
	{
		case 0: bqFilt = _BIQUAD_FLTPTR(_Biquad_FilterFLAT); break;
		case 1: bqFilt = _BIQUAD_FLTPTR(_Biquad_FilterPEQ); break;
		case 2: bqFilt = _BIQUAD_FLTPTR(_Biquad_FilterHP1); break;
		case 3: bqFilt = _BIQUAD_FLTPTR(_Biquad_FilterLP1); break;
		case 4: bqFilt = _BIQUAD_FLTPTR(_Biquad_FilterHP2); break;
		case 5: bqFilt = _BIQUAD_FLTPTR(_Biquad_FilterLP2); break;
		case 6: bqFilt = _BIQUAD_FLTPTR(_Biquad_FilterHS1); break;
		case 7: bqFilt = _BIQUAD_FLTPTR(_Biquad_FilterLS1); break;
		case 10: bqFilt = _BIQUAD_FLTPTR(_Biquad_FilterAP1); break;
		case 11: bqFilt = _BIQUAD_FLTPTR(_Biquad_FilterAP2); break;
		default :
			return( 0 );
	}

	iirFP->fs = (FLOAT)_float(_Sys_GetSamplingRate()); // default sampling rate at 96000Hz (floating point value)

	stable = _Biquad_FltFloatCoeff( bqFilt, iirFP, &iirFloatCoeff );
	if ( (*FilterType)&BIQUAD_EX_FLAG )
		_Biquad_FloatToFixCoeff( &iirFloatCoeff, iirCoeff, _BIQUAD_FRACTBITS_1_8_15 );
	else
		_Biquad_FloatToFixCoeff( &iirFloatCoeff, iirCoeff, _BIQUAD_FRACTBITS_1_2_21 );

	return(stable);
}

/* -----------------------------------------------------
* set EQ band gain
* band: EQ band
* v:  in range [ 01000h .. 04000h .. 05E00h ] -> [ -24dB .. 0dB .. +15dB ]
*/
void SetFilterGain( UpdateCoeffCallback *updateCoeffCallback, BiquadParameters *biquadParam, WORD DSPid, WORD PROCid, WORD band, WORD v )
{
	_FILTER_PARAM *iirFP;
	_BIQUAD_FIXEDCOEFF iirCoeff;

	if ( biquadParam->type[band] & (BIQUAD_XOVER_TYPE_BAND | (1<<6)) )	// filter band used by high order XOver filter?
		return;							// Do not allow custom Gain settings for XOver filters

	iirFP = (_FILTER_PARAM *)&biquadParam->filterParameters[band];

#ifdef _customFilterGain
	customFilterGain( iirFP, biquadParam, DSPid, PROCid, band, v );
#else
	if ( v > 0x5E00)
		v = 0x5E00;
	if ( v < 0x1000)
		v = 0x1000;
	iirFP->dBGain = _fmul( _float((int)(v-0x4000)), 0x3B000000 ); // 1/512
#endif

	if( FilterCoefficientsUpdate( &biquadParam->type[band], iirFP, &iirCoeff ) ) //stable?
		updateBiquadCallback( updateCoeffCallback->BIQUAD_UpdateCoeffFuncPtr, DSPid, PROCid, (WORD)&iirCoeff, band );
}

/* -----------------------------------------------------
* set EQ band frequency
* band: EQ band
* v: frequency value in range 20..21600 (in Hz)
*/
void SetFilterFreq( UpdateCoeffCallback *updateCoeffCallback, BiquadParameters *biquadParam, WORD DSPid, WORD PROCid, WORD band, DWORD v )
{
	_FILTER_PARAM *iirFP;
	_BIQUAD_FIXEDCOEFF iirCoeff;
	WORD maxBand;
	DWORD *rawFrequency;

	if ( biquadParam->type[band] & BIQUAD_XOVER_TYPE_BAND )	// filter band used by high order XOver filter?
		return;

	iirFP = (_FILTER_PARAM *)&biquadParam->filterParameters[band];

#ifdef _customFilterFreq
	customFilterFreq( iirFP, biquadParam, DSPid, PROCid, band, v );
#else
	rawFrequency = &biquadParam->frequency[band];
	biquadParam->frequency[band] = v;

	v = (WORD)((*rawFrequency)>>12);
	if( v<20 ) v=20;            //range limited to 20..21600Hz
	else if( v>21600 ) v=21600;

	iirFP->f0 = _fdiv( _float(biquadParam->frequency[band]), 0x45800000/* 2^12 */ );
#endif

	if ( biquadParam->type[band] & (1<<6) ) // XOver filter?
	{
		maxBand = getNumberOfXoverBands( biquadParam, band );
		UpdateXOverIIRCoeff( updateCoeffCallback, DSPid, PROCid, band, maxBand, iirFP, biquadParam );
	}
	else
	{
		if( !isLoadingPreset && FilterCoefficientsUpdate( &biquadParam->type[band], iirFP, &iirCoeff ) ) //stable?
			updateBiquadCallback( updateCoeffCallback->BIQUAD_UpdateCoeffFuncPtr, DSPid, PROCid, (WORD)&iirCoeff, band );
	}
}

/* -----------------------------------------------------
* set EQ band Q
* band: EQ band
* v: Q value in range 0,0x0100..0x7F00 = 0.01, ...8
*/
void SetFilterQ( UpdateCoeffCallback *updateCoeffCallback, BiquadParameters *biquadParam, WORD DSPid, WORD PROCid, WORD band, WORD v )
{
	_FILTER_PARAM *iirFP;
	_BIQUAD_FIXEDCOEFF iirCoeff;

	if ( biquadParam->type[band] & (BIQUAD_XOVER_TYPE_BAND | (1<<6)) )	// filter band used by high order XOver filter?
		return;							// Do not allow custom Gain settings for XOver filters

	iirFP = (_FILTER_PARAM *)&biquadParam->filterParameters[band];

#ifdef _customFilterQ
	customFilterQ( iirFP, biquadParam, DSPid, PROCid, band, v );
#else
	iirFP->Q = _fdiv( _float(v>>1), 0x467FFC00/*16383.0*/ );
	iirFP->Q = _fmul( _fmul( iirFP->Q, iirFP->Q ), 0x42EF6666/*119.7*/ );
	iirFP->Q = _fadd( iirFP->Q, 0x3E99999A/*0.3*/ );
#endif
	if( !isLoadingPreset && FilterCoefficientsUpdate( &biquadParam->type[band], iirFP, &iirCoeff ) ) //stable?
		updateBiquadCallback( updateCoeffCallback->BIQUAD_UpdateCoeffFuncPtr, DSPid, PROCid, (WORD)&iirCoeff, band );
}

WORD getNumberOfXoverBands( BiquadParameters *biquadParam, WORD band )
{
	WORD numberOfBands;
	int maxBandAllowed = 1;

	maxBandAllowed = ( ( biquadParam->numberOfBand - band ) > 4 ) ? 4 : ( biquadParam->numberOfBand - band );

	if ( !(biquadParam->type[band] & (1<<6)) ) // not XOver filter
		return 0;

	numberOfBands = SetXOverFilterType( 0, XOVER_READ_BANDNB, biquadParam->xoverType[band], NULL, NULL, NULL );
	if( numberOfBands > maxBandAllowed )
		numberOfBands = maxBandAllowed;

	return numberOfBands;
}

void clearLockedBands( WORD *bandsType, WORD numberOfBandsToUnlock )
{
	WORD i;
	for ( i=0; i<numberOfBandsToUnlock; i++ )
		bandsType[i] &= BIQUAD_EX_FLAG; // unlock by setting to FLAT
}

void flatBands( UpdateCoeffCallback *updateCoeffCallback, WORD DSPid, WORD PROCid, WORD bandStart, WORD numberOfBandsToUnlock )
{
	WORD i; 
	for( i = 0; i < numberOfBandsToUnlock; i++ )
		updateBiquadCallback(updateCoeffCallback->BIQUAD_FlatFuncPtr, DSPid, PROCid, 1, bandStart + i ); // flat band.
}

/* -----------------------------------------------------
* set EQ band Type
* band: EQ band
* v: Type
*/
void SetFilterType( UpdateCoeffCallback *updateCoeffCallback, BiquadParameters *biquadParam, WORD DSPid, WORD PROCid, WORD band, WORD v )
{
	_FILTER_PARAM *iirFP;
	_BIQUAD_FIXEDCOEFF iirCoeff;
	WORD type;
	int maxBand;

	if ( biquadParam->type[band] & BIQUAD_XOVER_TYPE_BAND )	// filter band used by high order XOver filter?
		return;

	iirFP = (_FILTER_PARAM *)&biquadParam->filterParameters[band];	

	if ( ( maxBand = getNumberOfXoverBands( biquadParam, band ) ) > 1 )
	{
		clearLockedBands( &biquadParam->type[band+1], maxBand-1 );	// "unlock" following bands used for Biquad of higher order
		flatBands( updateCoeffCallback, DSPid, PROCid, band+1, maxBand-1 );
	}

	if ( !(v & (1<<6)) )	// not XOver filter?
	{
		biquadParam->type[band] = (biquadParam->type[band] & BIQUAD_EX_FLAG) | v;

		if( !isLoadingPreset && FilterCoefficientsUpdate( &biquadParam->type[band], iirFP, &iirCoeff ) ) //stable?
			updateBiquadCallback( updateCoeffCallback->BIQUAD_UpdateCoeffFuncPtr, DSPid, PROCid, (WORD)&iirCoeff, band );
	}
	else	// XOver filter
	{
		v &= (1<<6)-1;
		if ( v > XOVER_LINKRILEY_8TH ) // is LP or HP, if true then HP
		{
			type = XOVER_HP;
			v -= 40;
		}
		else
			type = XOVER_LP;
		maxBand = ( (biquadParam->numberOfBand-band) > 4 ) ? 4 : ( biquadParam->numberOfBand - band );
		if ( ( maxBand = SetXOverFilterType( type, maxBand, v, iirFP, &biquadParam->type[band], &biquadParam->xoverType[band] ) ) != -1 )
		{
			if ( !isLoadingPreset )
				UpdateXOverIIRCoeff( updateCoeffCallback, DSPid, PROCid, band, maxBand, iirFP, biquadParam );
		}
	}
}

void InitDefaultPeqBands( BiquadParameters *biquadParam, WORD DSPid, WORD PROCid )
{
	static const DWORD defaultFreq[3] = { (DWORD)(120UL << 12), (DWORD)(1000UL << 12), (DWORD)(6000UL << 12) };
	UpdateCoeffCallback updateCoeffCallback;
	WORD band, maxBand;

	if ( !biquadParam )
		return;

	updateCoeffCallback.BIQUAD_UpdateCoeffFuncPtr = _cptr32(&_MixPA_Biquad_UpdateCoeff);
	updateCoeffCallback.BIQUAD_FlatFuncPtr = _cptr32(&_MixPA_Biquad_Flat);

	_MixPA_Biquad_OnOff( DSPid, PROCid, 1 );

	maxBand = (biquadParam->numberOfBand > 3) ? 3 : (WORD)biquadParam->numberOfBand;
	for ( band = 0; band < maxBand; band++ )
	{
		SetFilterFreq( &updateCoeffCallback, biquadParam, DSPid, PROCid, band, defaultFreq[band] );
		SetFilterQ( &updateCoeffCallback, biquadParam, DSPid, PROCid, band, 0x2000 );
		SetFilterGain( &updateCoeffCallback, biquadParam, DSPid, PROCid, band, 0x4000 );
		SetFilterType( &updateCoeffCallback, biquadParam, DSPid, PROCid, band, 1 );
	}
}

void UpdateXOverIIRCoeff( UpdateCoeffCallback *updateCoeffCallback, WORD DSPid, WORD PROCid,  WORD bandStart, WORD NumberOfBand, _FILTER_PARAM *IIRFilterParam, BiquadParameters *biquadParam )
{
	_FILTER_PARAM iirTmp;
	_BIQUAD_FIXEDCOEFF iirCoeff;
	WORD *FP, xoverType;
	
	WORD i, stable;
	FLOAT Q, f0;
	
	FP = &biquadParam->type[bandStart];
	f0 = IIRFilterParam->f0;
	xoverType = biquadParam->xoverType[bandStart]&((1<<8)-1);

	for (i=0; i<NumberOfBand; i++ )
	{
		IIRFilterParam[i].f0 = f0;
		if ( xoverType & (1<<3) ) // special frequency for Bessel filter
			iirTmp.f0 = (FLOAT)GetBesselFrequency( xoverType, FP[i]&(BIQUAD_EX_FLAG-1), i, f0 );
		else
			iirTmp.f0 = f0;
		iirTmp.Q = Q = IIRFilterParam[i].Q;	// Save Q 'cause it could be modified by XOverFilterUpdate function.
		iirTmp.fs =  IIRFilterParam[i].fs;
		stable = FilterCoefficientsUpdate( &FP[i], &iirTmp, &iirCoeff );
		if (stable)
		{
			if ( (!Q) )
				updateBiquadCallback( updateCoeffCallback->BIQUAD_FlatFuncPtr, DSPid, PROCid, 1, bandStart+i ); // flat 1 band.
			else
				updateBiquadCallback( updateCoeffCallback->BIQUAD_UpdateCoeffFuncPtr, DSPid, PROCid, (WORD)&iirCoeff, bandStart+i );
		}
		if ( ( NumberOfBand > 1 ) && ( i < ( NumberOfBand - 1 ) ) ) // tag only implied XOver bands
			biquadParam->type[bandStart+i+1] |= BIQUAD_XOVER_TYPE_BAND;	// Reserve this band for this XOver filter
	}
}


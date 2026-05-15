#ifndef __BIQUADCTRL_H__
#define __BIQUADCTRL_H__

// Biquad filter type defines & Biquad control function prototypes

#define XOVER_FLAT	0

#define XOVER_LP	0
#define XOVER_HP 	1

typedef enum 
{
	XOVER_BUTTER_1ST = 1,
	XOVER_BUTTER_2ND,
	XOVER_BUTTER_3RD,
	XOVER_BUTTER_4TH,
	XOVER_BUTTER_6TH,
	XOVER_BUTTER_8TH
} XOverButterworthType;

typedef enum
{
	XOVER_BESSEL_2ND = 1 | (1<<3),
	XOVER_BESSEL_3RD = 2 | (1<<3),
	XOVER_BESSEL_4TH = 3 | (1<<3),
	XOVER_BESSEL_6TH = 4 | (1<<3),
	XOVER_BESSEL_8TH = 5 | (1<<3)
} XOverBesselType;

typedef enum
{
	XOVER_LINKRILEY_2ND	= 1 | (1<<4),
	XOVER_LINKRILEY_4TH	= 2 | (1<<4),
	XOVER_LINKRILEY_6TH	= 3 | (1<<4),
	XOVER_LINKRILEY_8TH	= 4 | (1<<4)
} XOverLinkwitzRileyType;

#define UPDATE_HP1_FILTER ( (1<<6) | 2 )
#define UPDATE_LP1_FILTER ( (1<<6) | 3 )
#define UPDATE_HP2_FILTER ( (1<<6) | 4 )
#define UPDATE_LP2_FILTER ( (1<<6) | 5 )

WORD SetFlat( WORD maxBand, _FILTER_PARAM *FilterParam, WORD *xoverTypeTable );
WORD Set1stOrder( WORD F1stOrderType, WORD maxBand, FLOAT f0, _FILTER_PARAM *FilterParam, WORD *FilterType, WORD *xoverTypeTable );
WORD SetButterworthType( XOverButterworthType ButterSlope, WORD maxBand, WORD ButterFilter, FLOAT f0, _FILTER_PARAM *FilterParam, WORD *FilterType, WORD *xoverTypeTable );
WORD SetBesselType( XOverBesselType BesselSlope, WORD maxBand, WORD BesselFilter, FLOAT f0, _FILTER_PARAM *FilterParam, WORD *FilterType, WORD *xoverTypeTable );
WORD SetLinkwitzRileyType( XOverLinkwitzRileyType LinkwitzRileySlope, WORD maxBand, WORD LinkwitzRileyFilter, FLOAT f0, _FILTER_PARAM *FilterParam, WORD *FilterType, WORD *xoverTypeTable );
FLOAT GetBesselFrequency( XOverBesselType BesselSlope, WORD FilterType, WORD band, FLOAT f0 );
WORD SetXOverFilterType( WORD xOverLPHP, WORD maxBand, WORD xoverType, _FILTER_PARAM *iirFP, WORD *FilterType, WORD *xoverTypeTable );

#define	BIQUAD_EX_FLAG	(1<<15)
#define BIQUAD_XOVER_TYPE_BAND	(1<<14)
#define BIQUAD_MSB_USED	BIQUAD_XOVER_TYPE_BAND

void SetFilterGain( UpdateCoeffCallback *updateCoeffCallback, BiquadParameters *biquadParam, WORD DSPid, WORD PROCid, WORD band, WORD v );
void SetFilterFreq( UpdateCoeffCallback *updateCoeffCallback, BiquadParameters *biquadParam, WORD DSPid, WORD PROCid, WORD band, DWORD v );
void SetFilterQ( UpdateCoeffCallback *updateCoeffCallback, BiquadParameters *biquadParam, WORD DSPid, WORD PROCid, WORD band, WORD v );
void SetFilterType( UpdateCoeffCallback *updateCoeffCallback, BiquadParameters *biquadParam, WORD DSPid, WORD PROCid, WORD band, WORD v );
void InitDefaultPeqBands( BiquadParameters *biquadParam, WORD DSPid, WORD PROCid );
void UpdateXOverIIRCoeff( UpdateCoeffCallback *updateCoeffCallback, WORD DSPid, WORD PROCid, WORD bandStart, WORD NumberOfBand, _FILTER_PARAM *IIRFilterParam, BiquadParameters *biquadParam  );
void biquadSetLoadingPreset( WORD value );
void biquadSetNumberOfBandUsed( WORD value );
void updateBiquadCallback( PTR32 function_pointer, WORD DSPid, WORD PROCid, WORD count, WORD band );
WORD getNumberOfXoverBands( BiquadParameters *biquadParam, WORD band );

#endif //__BIQUADCTRL_H__
#ifndef __DSPDESIGNER_H__
#define __DSPDESIGNER_H__

#define NBDSPINITIALIZED 4

typedef struct biquadTag
{	struct _tagFILTER_PARAM *filterParameters;
	WORD *type;
	WORD *xoverType;
	DWORD *frequency; // 20.12 fixedpoint format
	int numberOfBand;
	int channel; // command channel attributed to the biquad
} BiquadParameters;

typedef struct biquadParamsTabTag
{	WORD nrpn;
	WORD functionId;
	BiquadParameters *parametersTable;
} BiquadParamsTable;

typedef struct biquadCommandParamsTabTag
{	WORD command;
	BiquadParameters *parametersTable;
} BiquadCommandParamsTable;

typedef struct UpdateCoeffCallback_tag
{
	PTR32 BIQUAD_FlatFuncPtr;
	PTR32 BIQUAD_UpdateCoeffFuncPtr;
} UpdateCoeffCallback;

void dspDesigner_InitAndRoute (void);
void dspDesigner_InitNrpnFunction (void);
WORD dichotomicSearch( PTR32 table, WORD rowSize, WORD itemIndex, WORD tableLen, WORD valueToSearch );
WORD dspNrpnHandlerCallback ( PTR32 FunctionPointer32, WORD nrpn, WORD dspId, WORD processId, DWORD value, WORD format );
extern WORD dsp[NBDSPINITIALIZED];

WORD dsp1InitAndRoute(void);
extern WORD dsp1pcs[6];
#define	NUMBEROFPROCESS1	0x0004
#define	NUMBEROFCOMMAND1	0x000D
extern const WORD nrpn1List[NUMBEROFCOMMAND1][2];
extern WORD dsp1NrpnHandler( WORD nrpn, WORD dspId, WORD processId, DWORD value, WORD format );

WORD dsp2InitAndRoute(void);
extern WORD dsp2pcs[5];
#define	NUMBEROFPROCESS2	0x0004
#define	NUMBEROFCOMMAND2	0x000E
extern const WORD nrpn2List[NUMBEROFCOMMAND2][2];
extern WORD dsp2NrpnHandler( WORD nrpn, WORD dspId, WORD processId, DWORD value, WORD format );

WORD dsp3InitAndRoute(void);
extern WORD dsp3pcs[6];
#define	NUMBEROFPROCESS3	0x0004
#define	NUMBEROFCOMMAND3	0x000D
extern const WORD nrpn3List[NUMBEROFCOMMAND3][2];
extern WORD dsp3NrpnHandler( WORD nrpn, WORD dspId, WORD processId, DWORD value, WORD format );

WORD dsp4InitAndRoute(void);
extern WORD dsp4pcs[5];
#define	NUMBEROFPROCESS4	0x0004
#define	NUMBEROFCOMMAND4	0x000E
extern const WORD nrpn4List[NUMBEROFCOMMAND4][2];
extern WORD dsp4NrpnHandler( WORD nrpn, WORD dspId, WORD processId, DWORD value, WORD format );

#endif //__DSPDESIGNER_H__
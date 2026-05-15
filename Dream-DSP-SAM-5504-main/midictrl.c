#include <system.h>
#include "dspDesigner.h"
#include "midictrl.h"

extern void Send( UCHAR data );	// function to send MIDI data is defined by application firmware (e.g. USB or UART)

PTR32 nrpnFunctionPtrTable[NBDSPINITIALIZED];

void dspDesigner_InitNrpnFunction( void )
{
	nrpnFunctionPtrTable[0] = _cptr32( dsp1NrpnHandler );
	nrpnFunctionPtrTable[1] = _cptr32( dsp2NrpnHandler );
	nrpnFunctionPtrTable[2] = _cptr32( dsp3NrpnHandler );
	nrpnFunctionPtrTable[3] = _cptr32( dsp4NrpnHandler );

}
typedef WORD (*NrpnFunctionPtr)(WORD, WORD, WORD, DWORD, WORD);

SysEx sysExData;

/* -----------------------------------------------------
 * handle MIDI NRPN parameter control
 * return 0, if NRPN not handled, else NRPN is handled
 */ 
WORD dspDesigner_HandleNRPN( WORD ch, WORD nrpn, DWORD v, WORD format )
{ 
	PTR32 nrpnFunction;
	WORD processId;
	WORD handled;

	// return if nrpn is not mapped
	if (nrpn == 0xFFFF) return 0;

	if (ch > 15)
		return 0;

	processId = nrpn>>8;

	/*
	 * Primary path: zero-based channel mapping (MIDI status byte semantics).
	 */
	if (ch < NBDSPINITIALIZED)
	{
		nrpnFunction = nrpnFunctionPtrTable[ch];
		handled = dspNrpnHandlerCallback( nrpnFunction, nrpn, dsp[ch], processId, v, format );
		if (handled)
			return 1;
	}

	/*
	 * Fallback for some SysEx senders that encode channel as 1..N instead of 0..N-1.
	 * Only applied to SysEx/28-bit path to avoid changing normal CC/NRPN handling.
	 */
	if ( (format == FORMAT_28BIT_PRECISION) && (ch > 0) && (ch <= NBDSPINITIALIZED) )
	{
		WORD ch0 = ch - 1;
		nrpnFunction = nrpnFunctionPtrTable[ch0];
		return dspNrpnHandlerCallback( nrpnFunction, nrpn, dsp[ch0], processId, v, format );
	}

	return 0;

}

WORD dspDesigner_HandleMidiEvent( DWORD Ev )
{
	WORD cin,ch,b2,b3;
	int sysExIndex;
	DWORD nrpnData;
	WORD SX_data[3];
	static WORD nrpn[16] = {	//NRPN number, initialized with 0xFFFF = undefined
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
		0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF };
	static WORD nrpn_val[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };	//NRPN value
	WORD nrpnTmp;

	//d = | code | byte1 | byte2 | byte3 |
	cin=(Ev>>24)&0x0F;	//code index number
	ch=(Ev>>16)&0x0F;	//MIDI channel (0..15)
	b2=(Ev>>8)&0x7F;	//MIDI byte 2
	b3=Ev&0x7F;			//MIDI byte 3

	if( cin==0xB )	//control change?
	{
		if( b2==98 )	//NRPN LSB?
		{
			nrpn[ch]=(nrpn[ch]&0xFF00)|b3;
			nrpn_val[ch]=0;
		}
		else if( b2==99 ) 	//NRPN MSB?
		{
			nrpn[ch]=(nrpn[ch]&0x00FF)|b3<<8;
			nrpn_val[ch]=0;
		}
		else if( b2==100 || b2==101 )	//RPN LSB or MSB?
		{
			nrpn[ch]=0xFFFF;	//clear NRPN
			nrpn_val[ch]=0;
		}
		else if( b2==38 )	//data entry LSB?
		{
			nrpn_val[ch]=(nrpn_val[ch]&0x7F00)|b3<<1;	//LSB
		}
		else if( b2==6 )	//data entry MSB?
		{
			nrpn_val[ch]=(nrpn_val[ch]&0x00FF)|b3<<8;	//MSB
			nrpnTmp = nrpn[ch];
			if ( ( ((nrpnTmp&0xFF00)>>8) == 0xFF ) || ( (nrpnTmp&0xFF) == 0xFF ) ) // is undefined?
				return 0;
			return dspDesigner_HandleNRPN( ch, nrpnTmp, (DWORD)nrpn_val[ch], FORMAT_14BIT_PRECISION );
		}
	}

	if ( cin == 0x4 ) // SysEx Continue
	{
		SX_data[0] = (Ev>>16)&0xFF;
		SX_data[1] = (Ev>>8)&0xFF;
		SX_data[2] = (Ev&0xFF);

		if (SX_data[0] == 0xF0)
			sysExData.pnt = 0;
		if (sysExData.pnt >= SYSEX_SIZE-3) return 0;
		sysExData.Buffer[sysExData.pnt++] = SX_data[0];
		sysExData.Buffer[sysExData.pnt++] = SX_data[1];
		sysExData.Buffer[sysExData.pnt++] = SX_data[2];
	}
	if ( cin >= 0x5 && cin <= 0x7 ) // SysEx End.
	{
		SX_data[0] = (Ev>>16)&0xFF;
		SX_data[1] = (Ev>>8)&0xFF;
		SX_data[2] = (Ev&0xFF);
		if (sysExData.pnt >= SYSEX_SIZE-1) return 0;
		sysExData.Buffer[sysExData.pnt++] = SX_data[0];
		if (cin >= 6)
		{
			if (sysExData.pnt >= SYSEX_SIZE-1) return 0;
			sysExData.Buffer[sysExData.pnt++] = SX_data[1];
		}
		if (cin == 7)
		{
			if (sysExData.pnt >= SYSEX_SIZE-1) return 0;
			sysExData.Buffer[sysExData.pnt++] = SX_data[2];
		}
		switch(sysExData.Buffer[SYSEX_CMD])
		{
			case 0x3D:
				b2 = sysExData.Buffer[SYSEX_NRPN_BASE+SYSEX_NRPN_COUNT];
				if( b2 > 4 ) return 0;
				for( sysExIndex = b2, nrpnData = 0;; sysExIndex-- )
				{
					nrpnData |= (DWORD)sysExData.Buffer[SYSEX_NRPN_BASE+SYSEX_NRPN_DATA+b2-sysExIndex];
					if (sysExIndex <= 1)
						break;
					nrpnData <<= (DWORD)7;
				}
				ch = sysExData.Buffer[SYSEX_NRPN_BASE+SYSEX_NRPN_CHANNEL];
				nrpnTmp = (sysExData.Buffer[SYSEX_NRPN_BASE+SYSEX_NRPN_HI]<<8) | sysExData.Buffer[SYSEX_NRPN_BASE+SYSEX_NRPN_LO];
				return dspDesigner_HandleNRPN( ch, nrpnTmp, nrpnData, FORMAT_28BIT_PRECISION );
				break;
			default: break; 
		}
	}
	return 0;
}

WORD dichotomicSearch( PTR32 table, WORD rowSize, WORD itemIndex, WORD tableLen, WORD valueToSearch )
{

	WORD min, max, mid, oldMid, pointedValue, lastElement;

	if (!tableLen) return -1;

	lastElement	= _rdxtmemEx(table+(tableLen-1)*rowSize+itemIndex);
	// Special case for 1st and Last element
	if (valueToSearch == lastElement )
		return tableLen-1;
	if ( (valueToSearch>>8) > (lastElement>>8) )
		return -1; // probably in custom function implementation
	if ( valueToSearch == _rdxtmemEx(table+itemIndex) )
		return 0;

	for ( oldMid = min = 0, max = tableLen-1, mid = (max+min)>>1; ; mid = (max+min)>>1 )
	{
		pointedValue = _rdxtmemEx(table+mid*rowSize+itemIndex);
		if ( valueToSearch == pointedValue )
		{
			return (mid);
		}
		else if ( valueToSearch < pointedValue )
		{
			max = mid;
		}
		else
		{
			min = mid;
		}

		if ( oldMid == mid )
		{
			if ( valueToSearch > pointedValue )
			{
				mid++;
				if (mid>=tableLen)
					return (1<<15) | mid;
			}
			return (1<<15) | mid;
		}
		oldMid = mid;
	}
}

void sendSysExMessage (WORD id, unsigned char msg)
{
	WORD id_val = ((id>>8) & 0x007f);

	Send( 0xF0 );
	Send( 0 );
	Send( 0x20 );
	Send( 0 );
	Send( id_val );
	Send( msg );
	Send( 0xF7 );

}
// This file can be modified by user and will not be overwritten by Dream DSP Designer.
#ifndef __MAINAPPCONFIG_H__
#define __MAINAPPCONFIG_H__

//[!USER CODE]
/*
 * SAM5504 memory map:
 *   EXTRAM  : 0x40000000  (external SDRAM if fitted)
 *   INT24RAM: 0x08E00000  (internal 24-bit data RAM, used for delay lines)
 *   INT16RAM: 0x08E10000  (internal 16-bit data RAM)
 *
 * These addresses are fixed hardware constants for the SAM5504.
 * Do NOT change them.
 */
#define    MEMADDR_EXTRAM_START    0x40000000
#define    MEMADDR_INT24RAM_START  0x08E00000
#define    MEMADDR_INT16RAM_START  0x08E10000
//[USER CODE!]

#endif //__MAINAPPCONFIG_H__
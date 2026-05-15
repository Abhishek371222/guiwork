#ifndef __MEMORYMAP_H__
#define __MEMORYMAP_H__

#include "MainAppConfig.h"

/*
 * Delay line memory map for SAM5504 internal 24-bit RAM.
 *
 * At 96kHz, maximum delay time supported by the DSP Designer delay block
 * determines required buffer size. Each delay process gets 0x200 words
 * (512 samples = ~5.3ms @ 96kHz), which is safe for the default max delay.
 *
 * Layout (non-overlapping, 0x200 spacing):
 *
 *   DSP #2, Process #1 (Delay): 0x08E00000 .. 0x08E001FF
 *   DSP #2, Process #3 (Delay): 0x08E00200 .. 0x08E003FF
 *   DSP #4, Process #1 (Delay): 0x08E00400 .. 0x08E005FF
 *   DSP #4, Process #3 (Delay): 0x08E00600 .. 0x08E007FF
 *
 * Total used: 0x800 words = 2048 words of internal 24-bit RAM.
 * INT24RAM size on SAM5504 = 0x10000 words, so usage is well within limits.
 *
 * DSP #1 and DSP #3 have no delay processes, so they need no RAM entries here.
 */

/* ---------------- DSP #2 ---------------- */
#define MEMADDR_DSP02_PCS00PAR02            (MEMADDR_INT24RAM_START + 0x0000)
#define MEMADDR_DSP02_PCS00PAR02_NEXTBASE   (MEMADDR_INT24RAM_START + 0x0200)

#define MEMADDR_DSP02_PCS01PAR02            (MEMADDR_INT24RAM_START + 0x0200)
#define MEMADDR_DSP02_PCS01PAR02_NEXTBASE   (MEMADDR_INT24RAM_START + 0x0400)

/* ---------------- DSP #4 ---------------- */
#define MEMADDR_DSP04_PCS00PAR02            (MEMADDR_INT24RAM_START + 0x0400)
#define MEMADDR_DSP04_PCS00PAR02_NEXTBASE   (MEMADDR_INT24RAM_START + 0x0600)

#define MEMADDR_DSP04_PCS01PAR02            (MEMADDR_INT24RAM_START + 0x0600)
#define MEMADDR_DSP04_PCS01PAR02_NEXTBASE   (MEMADDR_INT24RAM_START + 0x0800)

#endif //__MEMORYMAP_H__
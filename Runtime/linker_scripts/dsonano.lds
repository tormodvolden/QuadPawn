/* Linker script for QuadPawn on DSO Nano
 * Based on a linker script for Olimex STM32-H103 eval board.
 *
 *  Copyright 2007 CodeSourcery.
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 *
 */

/* Define memory regions. */
MEMORY
{
  /* the dual boot open-source benf+fft app ends at 0x0801c128 */
  /* the closed-source benf app ends below 0x08014c00 */
  rom (rx) : ORIGIN = 0x08014C00, LENGTH = 0x1F000 - 0x14C00 /* (DSO apps store settings at 0x801F000) */
  ram (rwx) : ORIGIN = 0x20000600, LENGTH = 20K - 0x600 - 1330 /* Leave 1.3K for stack */
}

_estack = 0x20000000 + 20K;

SECTIONS
  {
    .  = 0x0;          /* From 0x00000000 */

    .nanolib 0x08007800 (NOLOAD) : {
      build/nanolib.o
    }

    .text : {
      _vectors = .;
      *(.isr_vectors)      /* Vector table */
      _scode = .;
      *(.text*)        /* Program code */
      _ecode = .;
      *(.rodata*)      /* Read only data */
      _etext = .;
      _sidata = _etext;
    } >rom

    .  = 0x20000000;   /* From 0x20000000 */      

    .data : {
      _sdata = . ;
      *(.data)        /* Data memory */
      _edata = .;
    } >ram AT > rom

  .bss : {
    _sbss = .;
    *(.bss)         /* Zero-filled run time allocate data memory */
    _ebss = .;
    } >ram AT > rom
    
    .dummy (NOLOAD) : {
         /* We don't need no stinking exceptions */
        *(.ARM.exidx*)
    }
  }  
/*========== end of file ==========*/

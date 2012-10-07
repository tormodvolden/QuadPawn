
/* Prototypes for functions in nanoglue.c - for Nano-only use */

#ifndef NANOGLUE_H
#define NANOGLUE_H

/* This is defined in BIOS.h for the Quad, in nanoglue.c for the Nano */
uint32_t LCD_RD_Type();

/* These are not defined (yet) for the Quad */
void putcolumn(u16 x, u16 y, u32 *pixels, int count, int wait);
void getcolumn(u16 x, u16 y, u32 *pixels, int count);
void init_nano(void);

#endif /* NANOGLUE_H */

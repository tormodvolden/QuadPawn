
#ifndef NANOGLUE_H
#define NANOGLUE_H

/* This is defined in BIOS.h for the Quad, in nanoglue.c for the Nano */
uint32_t LCD_RD_Type();

/* This is defined in BIOS.h for the Quad, in the LIB for the Nano */
void LCD_WR_REG(u16 Reg, u16 Data);

/* These are not defined (yet) for the Quad */
void putcolumn(u16 x, u16 y, u16 count, u8 wait);
void getcolumn(u16 x, u16 y, u32 *pixels, u32 count);

#endif /* NANOGLUE_H */


/* Prototypes for functions in DSO Nano LIB used in Nano-only code */

#ifndef NANOLIB_H
#define NANOLIB_H

/* Not used for the Quad, in the LIB for the Nano */
void Set_System(void);
void GPIO_Config(void);
void LCD_Initial(void);
u8 MSD_Init(void);
void RCC_APB2PeriphClockCmd(u32 RCC_APB2Periph, FunctionalState NewState);

#endif /* NANOLIB_H */


#ifndef HARDWARE_H
#define HARDWARE_H

/* LCD screen size */
#ifdef DSONANO
# define LCD_X 320
#else
# define LCD_X 400 /* DSO Quad */
#endif
#define LCD_Y 240 /* for both Quad and Nano */

#ifdef DSONANO
# define VM_SIZE 16384
#else
# define VM_SIZE 32768
#endif

#endif /* HARDWARE_H */

#!/bin/sh
# md5sum 16312 bytes @ 0x8007800: d24e63cb4d66e3ddec55ab1ab0e39eed dso-lib.bin 
#<addr in LIB>	<name in Pawn>	<name in LIB>
awk 'BEGIN{printf ".thumb\n.syntax unified\n.code 16\n\n"} { printf ".globl %s\n.thumb_func\n. = 0x%08x\n%s:\n\n", $2, strtonum($1)-0x08007800, $2 }' <<EOF
0x08007acc	Set_System	Set_System
0x08007c04	GPIO_Config	GPIO_Config
0x08007c90	SPI_Config	SPI_Config
0x08007f30	__Get_TAB_8x14	Get_Font_8x14
0x08007f48	LCD_WR_REG	LCD_WR_REG
0x08007f74	__LCD_Initial	LCD_Initial
0x08008140	__Point_SCR	Point_SCR
0x0800817c	__LCD_SetPixl	Set_Pixel
0x08008194	__Clear_Screen	Clear_Screen
0x080081b0	__Display_Str	Display_Str
0x08008b08	MSD_Init	MSD_Init
0x08008dec	__USB_Istr	USB_Istr
0x08009be4	RCC_APB2PeriphClockCmd	RCC_APB2PeriphClockCmd
0x0800a4e0	__USB_Init	USB_Init
0x0800a674	__CTR_HP	CTR_HP


#include "stm32f10x.h"
#include "nanoglue.h"
#include "BIOS.h"
#include "nanolib-addr.h"

/* New functions to replace Quad-specific code in callers */

void putcolumn(u16 x, u16 y, u16 count, u8 wait)
{
}

void getcolumn(u16 x, u16 y, u32 *pixels, u32 count)
{
}

/* Before using functions from LIB, we must initialize the LIB data
   if we are coming straight from the bootloader (and not via LIB) */
static void init_nanolib_data(void) {

    unsigned long *src, *dst;
    /* copy the data segment into ram */
    src = nanolib_sidata;
    dst = nanolib_sdata;
    if (src != dst)
        while(dst < nanolib_edata)
            *(dst++) = *(src++);

    /* zero the bss segment */
    dst = nanolib_sbss;
    while(dst < nanolib_ebss)
        *(dst++) = 0;
}

/* If we are starting straight from a bootloader, the hardware
   must be initialized. On the Quad this is done by the BIOS */
void init_nano(void)
{
	init_nanolib_data();
	Set_System(); /* configures peripheral clocks */
	GPIO_Config();
	__LCD_Initial();
	__Clear_Screen(0);
	__Display_Str(8, 87, 0x07E0, 0, (u8*) "   Booting Pawn ");
	MSD_Init(); /* calls SPI_Config() */
}

/* These Quad BIOS functions have equivalents in the DSO Nano LIB:
 void __CTR_HP(void);
 void __USB_Istr(void);
 void __USB_Init(void);
 void __LCD_Initial(void);
 void __Clear_Screen(u16 Color);
 void __Point_SCR(u16 x0, u16 y0);
 void __Display_Str(u16 x0, u16 y0, u16 Color, u8 Mode, u8 *s);
 void __LCD_SetPixl(u16 Color);  = Set_Pixel
 u16  __Get_TAB_8x14(u8 Code, u16 Row); = Get_Font_8x14
*/

/* Provide functions missing from the DSO Nano LIB */

 uint32_t LCD_RD_Type()
 {
	return 0; // FIXME: probe or return something more useful
 }

 u16  __LCD_GetPixl(void)
 {
	return 0; // TODO
 }

 void __LCD_Fill(u16 *pBuffer,u16 NumPixel)    // Fill number of pixel by DMA 
 {
 } // TODO

 void __LCD_DMA_Ready(void)                    // Wait LCD data DMA ready
 {}
 
 u32  __Read_FIFO(void)                        // Read data from FIFO & Ptr+1
 {
	return 0;
 }
 
 void __Set(u8 Object, u32 Value)
 {
 }

 u32  __Get(u8 Object)
 {
	return 0;
 }
 
 void __Set_Param(u8 RegAddr, u8 Parameter)
 {
 }
 
 u8   __ProgDiskPage(u8* pBuffer, u32 ProgAddr)
 {
	return 0;
 }
 
/* These Quad BIOS functions are not used in current QuadPawn:
 void __LCD_Set_Block(u16 x1, u16 x2, u16 y1, u16 y2);
 
 void __LCD_Copy(uc16 *pBuffer, u16  NumPixel); // Send a row data to LCD
 void __Row_Copy(uc16 *S_Buffer,u16 *T_Buffer); // Copy one row base data to buffer
 void __Row_DMA_Ready(void);                    // Wait row base data DMA ready
 
 u8   __FLASH_Erase(u32 Address);
 u8   __FLASH_Prog(u32 Address, u16 Data);
 void __FLASH_Unlock(void);
 void __FLASH_Lock(void);
 
 u8   __Chk_SYS(u32 Licence);
 u8*  __Chk_DFU(void);
 u8*  __Chk_HDW(void);

 u8 __OpenFileWr(u8* Buffer, u8* FileName, u16* Cluster, u32* pDirAddr);
 u8 __OpenFileRd(u8* Buffer, u8* FileName, u16* Cluster, u32* pDirAddr);
 u8 __ReadFileSec(u8* Buffer, u16* Cluster);
 u8 __ProgFileSec(u8* Buffer, u16* Cluster);
 u8 __CloseFile(u8* Buffer, u32 Lenght, u16* Cluster, u32* pDirAddr);

 u32  __Input_Lic(u16 x0, u8 y0);               //Return: 32Bits Licence 
 u32  __GetDev_SN(void);                        // Get 32bits Device Serial Number

 u8   __Ident(u32 Dev_ID, u32 Proj_ID, u32 Lic_No);
 
 void __ExtFlash_PageRD(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
 void __ExtFlash_PageWR(u8* pBuffer, u32 WriteAddr);
 u8   __ReadDiskData(u8* pBuffer, u32 ReadAddr, u16 Lenght);
*/

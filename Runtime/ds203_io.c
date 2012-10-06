#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "hardware.h"
#include "ds203_io.h"
#include "BIOS.h"
#include <stdio.h>
#include "mathutils.h"
#include "drawing.h"
#include "ff.h"

/* ---- LCD drawing ---- */

static char buffer[55];

// Format the message to statically allocated buffer and then call BIOS to
// print it on screen.
int lcd_printf(u16 x0, u16 y0, u16 color, u8 mode, const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int rv = vsnprintf(buffer, sizeof(buffer), fmt, va);
    va_end(va);
    
    __Display_Str(x0, y0, color, mode, (u8*)buffer);
    return rv;
}

bool f_exists(const char *name)
{
    FIL file;
    if (f_open(&file, name, FA_READ | FA_OPEN_EXISTING) != FR_OK)
        return false;
    
    f_close(&file);
    return true;
}

// Find a filename that is not in use. Format is a printf format string.
char *select_filename(const char *format)
{
    static char filename[13];
    for (int i = 0; i <= 999; i++)
    {
        snprintf(filename, sizeof(filename), format, i);
        if (!f_exists(filename)) break;
    }
    return filename;
}


/* ----------------- Bitmap writing ------------ */

// 16-color palette for bitmaps. Could be adjusted per-application to
// get better looking images.
const uint32_t bmp_default_palette[16] = {
    RGB565RGB(0,0,0), RGB565RGB(255,255,255), RGB565RGB(128,128,128),
    RGB565RGB(255,0,0), RGB565RGB(0,255,0), RGB565RGB(0,0,255),
    RGB565RGB(255,255,0), RGB565RGB(0,255,255), RGB565RGB(255,0,255),
    RGB565RGB(128,0,0), RGB565RGB(0,128,0), RGB565RGB(0,0,128),
    
    // Following 4 places are used for application-specific colors
    RGB565RGB(63, 63, 63), RGB565RGB(0, 0, 63)
};

// Find the closest color in palette, using Manhattan distance.
// Comparing in HSV space would be more accurate but slower.
static int quantize(uint16_t color, const uint32_t *palette)
{
    int min_delta = 999999;
    int closest = 0;
    for (int i = 0; i < 16; i++)
    {
        int delta = (
            abs(RGB565_R(color) - RGB565_R(palette[i])) +
            abs(RGB565_G(color) - RGB565_G(palette[i])) +
            abs(RGB565_B(color) - RGB565_B(palette[i]))
        );
        if (delta < min_delta)
        {
            min_delta = delta;
            closest = i;
        }
    }
    
    return closest;
}

#define LCD_X_HI (LCD_X >> 8)
#define LCD_X_LO (LCD_X & 0xff)
#define LCD_Y_HI (LCD_Y >> 8)
#define LCD_Y_LO (LCD_Y & 0xff)
#define BMP_FSIZE_HI    ((LCD_X * LCD_Y / 2 + 118) >> 8)
#define BMP_FSIZE_LO    ((LCD_X * LCD_Y / 2 + 118) & 0xff)
#define BMAP_SIZE_HI    ((LCD_X * LCD_Y / 2) >> 8)
#define BMAP_SIZE_LO    ((LCD_X * LCD_Y / 2) & 0xff)
// We know the LCD size in advance, so the file header can be written out
// by hand.
// The main header is 14 bytes, bitmap info header is 40 bytes and the palette
// is 64 bytes. Therefore bitmap data starts at offset 118.
const uint8_t BMP_HEADER[54] = {
    0x42, 0x4D, // BMP magic
    BMP_FSIZE_LO, BMP_FSIZE_HI, 0x00, 0x00, // File size, X * Y / 2 + 118
    0x00, 0x00, 0x00, 0x00, // Creator
    0x76, 0x00, 0x00, 0x00, // Offset to bitmap data = 118 bytes 
    0x28, 0x00, 0x00, 0x00, // Header size = 40 bytes
    LCD_X_LO, LCD_X_HI, 0x00, 0x00, // Bitmap width
    LCD_Y_LO, LCD_Y_HI, 0x00, 0x00, // Bitmap height
    0x01, 0x00,             // 1 color plane
    0x04, 0x00,             // 4 bits per pixel
    0x00, 0x00, 0x00, 0x00, // Uncompressed
    BMAP_SIZE_LO, BMAP_SIZE_HI, 0x00, 0x00, // Bitmap data size
    0x70, 0x17, 0x00, 0x00, // Horizontal pixel per meter = 6000
    0x70, 0x17, 0x00, 0x00, // Vertical pixel per meter
    0x10, 0x00, 0x00, 0x00, // Palette colors = 16
    0x00, 0x00, 0x00, 0x00  // Important colors
};

bool write_bitmap(const char *filename, const uint32_t *palette)
{
    FIL file;
    
    if (f_open(&file, filename, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
        return false;
    
    // Write header
    unsigned bytes;
    f_write(&file, BMP_HEADER, sizeof(BMP_HEADER), &bytes);
    if (bytes != sizeof(BMP_HEADER))
        return false;
    
    // Write palette
    for (int i = 0; i < 16; i++)
    {
        char entry[4] = {
            RGB565_B(palette[i]),
            RGB565_G(palette[i]),
            RGB565_R(palette[i]),
            0
        };
        f_write(&file, entry, sizeof(entry), &bytes);
    }
    
    // Write bitmap data
    for (int y = 0; y < LCD_Y; y++)
    {
        for (int x = 0; x < LCD_X; x += 2)
        {
            __Point_SCR(x, y);
            int colorH = quantize(__LCD_GetPixl(), palette);
            __Point_SCR(x + 1, y);
            int colorL = quantize(__LCD_GetPixl(), palette);
            
            char byte = (colorH << 4) | colorL;
            f_write(&file, &byte, 1, &bytes);
        }
    }
    
    return f_close(&file) == FR_OK;
}




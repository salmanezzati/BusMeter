/************************************************************************************************
* DESCRIPTION
* ===========
* Reading BMP from SD card
* using : Petit FatFs - FAT file system module  R0.01a (C)ChaN, 2009
*		  
* Display on TFT QVGA circuitidea.com BL-TFT240320PLUS
* support only bmp 24 bit
* support BMP padding data. Option for complex BMP (image width devide by 4 giving 0).
* for small size and fast display.
*
* BMP padding
* -----------
* in BMP total horizontal bytes must devide by 4. remain is padding 0-3 byte.
* For fast way to display picture. do this.
* 1. create picture width can device by 4 give 0
* 2. using FILL IN AREA mode of QVGA.
*
* HISTORY
* -------
* 01/10/2009 Using stream mode for faster and smooter.
* 	         **NOTE** 
*            Becare full to set stream parameter before use function. bmpShow has no link to FATFS (FATFS not public variable).
*
*			 fs.flag |= FA_STREAM;	// Set stream mode
*			 bmpShow((unsigned int)SCREEN_HOR_SIZE/2 - w/2, (unsigned int)SCREEN_VER_SIZE/2 - h/2);
*			 fs.flag &= ~FA_STREAM;	// Clear stream mode
* 
************************************************************************************************/
#include "sd_bmp.h"


typedef signed char             int8;
typedef unsigned char           uint8;
typedef signed int              int16;
typedef unsigned int            uint16;
typedef signed long int         int32;
typedef unsigned long int       uint32;
typedef struct BMIH { //declares BitMap Info Header structure
	unsigned long biSize;            //Length of bitmap information header (40 bytes for Windows V3 bitmaps)
	unsigned long biWidth;           //Width (pixels)
	unsigned long biHeight;          //Height (pixels)
	unsigned short biPlanes;         //Color planes, always 1
	unsigned short biBitCount;       //Color bits per pixel (1, 4, 8, 16, 24 and 32)
	unsigned long biCompression;     //Compression method, we only read uncompressed (type 0)
	unsigned long biSizeImage;       //Image data length
	unsigned long biXPelsPerMeter;   //Horizontal resolution (pixels per meter)
	unsigned long biYPelsPerMeter;   //Vertical resolution (pixel per meter)
	unsigned long biClrUsed;         //Number of colors, ignored.
	unsigned long biClrImportant;    //Number of important colors, ignored.                                  
}BMP_INFO;

typedef struct BMFH { //declares BitMap File Header structure
	unsigned short bfType;           //Always 0x42 0x4D (hex for BM <-- indicating a bitmap)
	unsigned long  bfSize;           //File size (bytes)
    unsigned short bfReserved1;      //Reserved, ignored
    unsigned short bfReserved2;      //Reserved, ignored
    unsigned long  bfOffBits;        //Location in file of the first bitmap data               
}BMP_HEADER;

unsigned long bmpWidth;           //Width (pixels)
unsigned long bmpHeight;          //Height (pixels)
extern FIL file;
FRESULT bmpFile(const char* fname)
{
    BMP_HEADER BMPHeader;
    BMP_INFO BMPInfo;
    //FRESULT res;
    UINT br;
         
    if (f_open(&file,fname,FA_READ) != FR_OK);                        
        //return 1; // File Open count
    if (f_read(&file,&BMPHeader, sizeof(BMPHeader), &br) != FR_OK);   
        //return 2; // read bmp header
    if (BMPHeader.bfType != 0x4d42 );                                
        //return 3; // type is 'BM'
    if (f_read(&file,&BMPInfo, sizeof(BMPInfo), &br) != FR_OK);       
        //return 4; // read bmp info
    //if (BMPInfo.biSize     != 40)                               
        //return 5; //unknown header format/length
    //if (BMPInfo.biPlanes      !=   1)                               
        //return 6; //this should be 1
    //if ((BMPInfo.biWidth >> 4 )      >  240)                               
        //return 7; //image over size
    //if (BMPInfo.biHeight      >  320)                               
        //return 8; //image over size
    //if (BMPInfo.biCompression !=   0)                               
        //return 9; //0=no compression

    bmpWidth  = BMPInfo.biWidth >>16;
    bmpHeight = BMPInfo.biSize >>16;  //bmpHeight = BMPInfo.biHeight>>4;
         
    //if (f_lseek(&file,BMPHeader.bfOffBits)!= FR_OK)            return 10; // move to start of bmp data
    return FR_OK;
}

unsigned long createRGB(int r, int g, int b)
{   
    return ((r & 0xff) << 11) | ((g & 0xff) << 5) | (b & 0xff);
}

unsigned long Create_RGB(int R, int G, int B)
{   
    return (((R >> 3) & 0x1f) << 11) | (((G >> 2) & 0x3f) << 5) | ((B >> 3) & 0x1f);
}

void RGB(uint8 G, uint8 B, uint8 R)
{    
    int16 code=0;
    B = B >> 3; 
    G = G >> 2;
    R = R >> 3;       
    code = (R & 0x1f) << 11;
    code |= (G & 0x3f) << 5;
    code |= (B & 0x1f) << 0;
    //ili9325_wrreg(TS_INS_RW_GRAM,code);
    LCD_WriteRAM(code);     
}


unsigned char Bmp_Show(unsigned int sx, unsigned int sy,const char* fname, char mode)    // support BMP 24 bit True color only
{                                               // 320      240
    //unsigned short color;
    unsigned int x,y;
    unsigned int i,j;
    unsigned int Code[10];
    //unsigned int ex,ey;
    uint8 res,buffer[720];
    UINT br;
    
    unsigned int c;
    bmpFile(fname); 
    
	LCD_WriteReg(TS_INS_DISP_CTRL1,0x0123);

	delay_ms(10);
      
    if (mode==0)
    {
        LCD_WriteReg(TS_INS_GRAM_ADX,318);
        LCD_WriteReg(TS_INS_GRAM_ADY,0);
        LCD_WriteReg(TS_INS_START_ADX,0);  
        LCD_WriteReg(TS_INS_END_ADX,319);
        LCD_WriteReg(TS_INS_START_ADY,0); 
        LCD_WriteReg(TS_INS_END_ADY,239); 
        LCD_Reg(TS_INS_RW_GRAM);
        x=240*3 ;
        y=320;  
        for (j=0; j<y; j++)                      
        {
            //res =  f_read(&file,buffer,x,&br);
            //if(res) return res;  
            f_read(&file,buffer,x,&br);
            for (i=0; i<x; i+=3)
            {    
                  RGB(buffer[i+2],buffer[i+1],buffer[i]); 
            }
        }
        res=f_close(&file);   
    }
    else if (mode==1)
    {
        LCD_WriteReg(TS_INS_GRAM_ADX,bmpHeight+sx-2);
        LCD_WriteReg(TS_INS_GRAM_ADY,sy);
        LCD_WriteReg(TS_INS_START_ADX,sx);
        LCD_WriteReg(TS_INS_END_ADX,sx+bmpHeight-1);
        LCD_WriteReg(TS_INS_START_ADY,sy);
        LCD_WriteReg(TS_INS_END_ADY,sy+bmpWidth-1);
        LCD_Reg(TS_INS_RW_GRAM);
            
        x = (bmpWidth*3);
        y = bmpHeight; 
        for (j=0; j<y; j++)                      
        { 
            f_read(&file,buffer,x,&br);
            for (i=0; i<x; i+=3)
            {    
                RGB(buffer[i+2],buffer[i+1],buffer[i]); 
            }
        } 
        res=f_close(&file);            
    } 
    else 
    {
        LCD_WriteReg(TS_INS_GRAM_ADX,sx);
        //lcd_write_index_register(TS_INS_GRAM_ADX);
        //lcd_write_wdr(sx);                    
        LCD_WriteReg(TS_INS_START_ADX,sx);
        //lcd_write_index_register(TS_INS_START_ADX);
        //lcd_write_wdr(sx);                
        LCD_WriteReg(TS_INS_END_ADX,sx+bmpWidth-1);
        //lcd_write_index_register(TS_INS_END_ADX);
        //lcd_write_wdr(sx+bmpWidth-1);    
        LCD_WriteReg(TS_INS_GRAM_ADY,239-sy);
        //lcd_write_index_register(TS_INS_GRAM_ADY);
        //lcd_write_wdr(239-sy);
        LCD_WriteReg(TS_INS_START_ADY,239-(sy+bmpHeight-1));
        //lcd_write_index_register(TS_INS_START_ADY);
        //lcd_write_wdr(239-(sy+bmpHeight-1));      
        LCD_WriteReg(TS_INS_END_ADY,239-sy);
        //lcd_write_index_register(TS_INS_END_ADY);
        //lcd_write_wdr(239-sy);                    
        LCD_Reg(TS_INS_RW_GRAM);
        //lcd_write_index_register(TS_INS_RW_GRAM);
        
        c = bmpWidth * 3; 
        if (c % 4 != 0)
        {
            c += 4 - (c % 4);
        }
        c = c - (bmpWidth * 3);   
        x=((bmpWidth)*3);
        y=(bmpHeight);
        for (j=0; j<y; j++)                      
        {
            res =  f_read(&file,buffer,x+c,&br);
            if(res) return res; 
            for (i=0; i<x-c; i+=3)
            {
                RGB(buffer[i+2],buffer[i+1],buffer[i]); 
            }
        } 
        
        res=f_close(&file);            
    }
    
    LCD_WriteReg(TS_INS_GRAM_ADX,0);
    LCD_WriteReg(TS_INS_GRAM_ADY,0);
    LCD_WriteReg(TS_INS_START_ADX,0);
    LCD_WriteReg(TS_INS_START_ADY,0);
    LCD_WriteReg(TS_INS_END_ADX,319);
    LCD_WriteReg(TS_INS_END_ADY,239);
    
    LCD_WriteReg(TS_INS_DISP_CTRL1,0x0133);
//    delay_ms(10);  
}  
       

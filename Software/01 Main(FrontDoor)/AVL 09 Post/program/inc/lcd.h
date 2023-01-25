
#ifndef __LCD_H
#define __LCD_H	

#include "main.h"

#define   LANDSCAPE

typedef struct  
{										    
	u16 width;			
	u16 height;			
	u16 id;				
	u8  dir;			
	u8	wramcmd;		
	u8  setxcmd;		
	u8  setycmd;		
}_lcd_dev; 	  


extern _lcd_dev lcddev;	

extern u16  POINT_COLOR;
extern u16  BACK_COLOR; 

//////////////////////////////////////////////////////////////////////////////////	 
#define TS_INS_START_OSC             0x00
#define TS_INS_DRIV_OUT_CTRL         0x01
#define TS_INS_DRIV_WAV_CTRL         0x02
#define TS_INS_ENTRY_MOD             0x03
#define TS_INS_RESIZE_CTRL           0x04
#define TS_INS_DISP_CTRL1            0x07
#define TS_INS_DISP_CTRL2            0x08
#define TS_INS_DISP_CTRL3            0x09
#define TS_INS_DISP_CTRL4            0x0A
#define TS_INS_RGB_DISP_IF_CTRL1     0x0C
#define TS_INS_FRM_MARKER_POS        0x0D
#define TS_INS_RGB_DISP_IF_CTRL2     0x0F
#define TS_INS_POW_CTRL1             0x10
#define TS_INS_POW_CTRL2             0x11
#define TS_INS_POW_CTRL3             0x12
#define TS_INS_POW_CTRL4             0x13
#define TS_INS_GRAM_HOR_AD           0x20
#define TS_INS_GRAM_VER_AD           0x21
#define TS_INS_RW_GRAM               0x22
#define TS_INS_POW_CTRL7             0x29
#define TS_INS_FRM_RATE_COL_CTRL     0x2B
#define TS_INS_GAMMA_CTRL1           0x30
#define TS_INS_GAMMA_CTRL2           0x31
#define TS_INS_GAMMA_CTRL3           0x32
#define TS_INS_GAMMA_CTRL4           0x35 
#define TS_INS_GAMMA_CTRL5           0x36
#define TS_INS_GAMMA_CTRL6           0x37
#define TS_INS_GAMMA_CTRL7           0x38
#define TS_INS_GAMMA_CTRL8           0x39
#define TS_INS_GAMMA_CTRL9           0x3C
#define TS_INS_GAMMA_CTRL10          0x3D
#define TS_INS_HOR_START_AD          0x50
#define TS_INS_HOR_END_AD            0x51
#define TS_INS_VER_START_AD          0x52
#define TS_INS_VER_END_AD            0x53
#define TS_INS_GATE_SCAN_CTRL1       0x60
#define TS_INS_GATE_SCAN_CTRL2       0x61
#define TS_INS_GATE_SCAN_CTRL3       0x6A
#define TS_INS_PART_IMG1_DISP_POS    0x80
#define TS_INS_PART_IMG1_START_AD    0x81
#define TS_INS_PART_IMG1_END_AD      0x82
#define TS_INS_PART_IMG2_DISP_POS    0x83
#define TS_INS_PART_IMG2_START_AD    0x84
#define TS_INS_PART_IMG2_END_AD      0x85
#define TS_INS_PANEL_IF_CTRL1        0x90
#define TS_INS_PANEL_IF_CTRL2        0x92
#define TS_INS_PANEL_IF_CTRL3        0x93
#define TS_INS_PANEL_IF_CTRL4        0x95
#define TS_INS_PANEL_IF_CTRL5        0x97
#define TS_INS_PANEL_IF_CTRL6        0x98           


#ifdef LANDSCAPE
#define WIDTH       	320
#define HEIGHT      	240
#define TS_VAL_ENTRY_MOD			0x0028
#define TS_INS_GRAM_ADX				TS_INS_GRAM_VER_AD
#define TS_INS_GRAM_ADY				TS_INS_GRAM_HOR_AD
#define TS_INS_START_ADX   			TS_INS_VER_START_AD
#define TS_INS_END_ADX   			TS_INS_VER_END_AD
#define TS_INS_START_ADY   			TS_INS_HOR_START_AD
#define TS_INS_END_ADY   			TS_INS_HOR_END_AD
#else
#define WIDTH       	240 
#define HEIGHT      	320
#define TS_VAL_ENTRY_MOD             0x0030
#define TS_INS_GRAM_ADX              TS_INS_GRAM_HOR_AD
#define TS_INS_GRAM_ADY              TS_INS_GRAM_VER_AD
#define TS_INS_START_ADX             TS_INS_HOR_START_AD
#define TS_INS_END_ADX               TS_INS_HOR_END_AD
#define TS_INS_START_ADY             TS_INS_VER_START_AD
#define TS_INS_END_ADY               TS_INS_VER_END_AD
#endif


typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;
#define LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////
#define L2R_U2D  0 
#define L2R_D2U  1 
#define R2L_U2D  2 
#define R2L_D2U  3 

#define U2D_L2R  4 
#define U2D_R2L  5 
#define D2U_L2R  6 
#define D2U_R2L  7 

#define DFT_SCAN_DIR  L2R_U2D  

#define Good_White_1       0xEFBD
#define Good_White     0xF7DE
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 
#define BRRED 			 0XFC07 
#define GRAY  			 0X8430 
	
	
#define Brand_Blue       0X017F
#define DARKBLUE      	 0X01CF	
#define LIGHTBLUE      	 0X7D7C	
#define GRAYBLUE       	 0X5458 

 
#define LIGHTGREEN     	 0X841F 

#define LGRAY 			 0XC618 

#define LGRAYBLUE        0XA651 
#define LBBLUE           0X2B12 

void LCD_Init(void);
void LCD_DisplayOn(void);													
void LCD_DisplayOff(void);													
void LCD_Clear(u16 Color);	
void myLCD_Clear(u16 color); 												
void LCD_SetCursor(u16 Xpos, u16 Ypos);										
void LCD_DrawPoint(u16 x,u16 y);
void myLCD_DrawPoint(u16 x,u16 y,u16 Point_color);
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);								
u16  LCD_ReadPoint(u16 x,u16 y); 											
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 Point_Color);										
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);	
void myLCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);						
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);	
void myLCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u8 fill, u16 color);	   				
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);		   				
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);
void LCD_MyShowChar(u16 x,u16 y,u8 num,u16 size,u8 mode,u16 Point_color,u16 Back_color);
void LCD_MyShowChar3(u16 x,u16 y,u8 num,u16 size,u8 mode,u16 Point_color,u16 Back_color);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		
void LCD_ShowStringWithSize(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u16 str_size);
void LCD_MyShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u16 Point_color,u16 Back_color);
void LCD_MyShowString2(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u16 Point_color);
void LCD_MyShowString3(u16 x,u16 y,u8 size,u8 *p,u16 Point_color ,u16 Back_color);
void Button(u16 xs, u16 ys, u16 xe, u16 ye, u16 button_color,u16 width, u8 font_size,u8 *p,u16 font_color);
void Button2(u16 xs, u16 ys, u16 xe, u16 ye, u16 button_color,u16 font_width1, u8 font_size1,u8 *p1, u16 font_color1, u16 font_width2, u8 font_size2,u8 *p2,u16 font_color2);

void LCD_Show_Image(int x,int y,const u16 *pointer);
void LCD_MyShow_Image(int x,int y,u8 *pointer,u16 Point_color,u16 Back_color);

void myLCD_set_font_color(int);
void myLCD_set_font_size(int);
void myLCD_fa_num(void);
void myLCD_en_num(void);
void myLCD_font_effect(int);
void myLCD_italic_on(void);
void myLCD_italic_off(void);
void myLCD_put_char(char);
void myLCD_put_bold_char(char , int);
void myLCD_goto_xy(int,int);
void myLCD_text_highlight_on(void);
void myLCD_text_highlight_off(void);
void myLCD_text_highlight_color(unsigned int);
void myLCD_putsf_mix(const char * , int);
void unicode2ascii(char * , char *);
int Get_String_Length(const char *string);
void myLCD_draw_round_rectangle(int x0,int y0,int x1,int,int,char,int);
void myLCD_putchar(char character,int forground_color,int background_color,int transparent_mode);
void myLCD_putsf(const char *string,int forground_color,int background_color,int transparent_mode);
void myLCD_puts(char *string,int forground_color,int background_color,int transparent_mode);


void LCD_Reg(u8 LCD_Reg);
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u8 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);		  
void LCD_Scan_Dir(u8 dir);				
void LCD_Display_Dir(u8 dir);			
 					   																			 

#define R0             0x00
#define R1             0x01
#define R2             0x02
#define R3             0x03
#define R4             0x04
#define R5             0x05
#define R6             0x06
#define R7             0x07
#define R8             0x08
#define R9             0x09
#define R10            0x0A
#define R12            0x0C
#define R13            0x0D
#define R14            0x0E
#define R15            0x0F
#define R16            0x10
#define R17            0x11
#define R18            0x12
#define R19            0x13
#define R20            0x14
#define R21            0x15
#define R22            0x16
#define R23            0x17
#define R24            0x18
#define R25            0x19
#define R26            0x1A
#define R27            0x1B
#define R28            0x1C
#define R29            0x1D
#define R30            0x1E
#define R31            0x1F
#define R32            0x20
#define R33            0x21
#define R34            0x22
#define R36            0x24
#define R37            0x25
#define R40            0x28
#define R41            0x29
#define R43            0x2B
#define R45            0x2D
#define R48            0x30
#define R49            0x31
#define R50            0x32
#define R51            0x33
#define R52            0x34
#define R53            0x35
#define R54            0x36
#define R55            0x37
#define R56            0x38
#define R57            0x39
#define R59            0x3B
#define R60            0x3C
#define R61            0x3D
#define R62            0x3E
#define R63            0x3F
#define R64            0x40
#define R65            0x41
#define R66            0x42
#define R67            0x43
#define R68            0x44
#define R69            0x45
#define R70            0x46
#define R71            0x47
#define R72            0x48
#define R73            0x49
#define R74            0x4A
#define R75            0x4B
#define R76            0x4C
#define R77            0x4D
#define R78            0x4E
#define R79            0x4F
#define R80            0x50
#define R81            0x51
#define R82            0x52
#define R83            0x53
#define R96            0x60
#define R97            0x61
#define R106           0x6A
#define R118           0x76
#define R128           0x80
#define R129           0x81
#define R130           0x82
#define R131           0x83
#define R132           0x84
#define R133           0x85
#define R134           0x86
#define R135           0x87
#define R136           0x88
#define R137           0x89
#define R139           0x8B
#define R140           0x8C
#define R141           0x8D
#define R143           0x8F
#define R144           0x90
#define R145           0x91
#define R146           0x92
#define R147           0x93
#define R148           0x94
#define R149           0x95
#define R150           0x96
#define R151           0x97
#define R152           0x98
#define R153           0x99
#define R154           0x9A
#define R157           0x9D
#define R192           0xC0
#define R193           0xC1
#define R229           0xE5							  		 
#endif  


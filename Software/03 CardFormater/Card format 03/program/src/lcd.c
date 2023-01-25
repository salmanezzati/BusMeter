
#include "lcd.h"
#include "font.h" 
#include "PE & EN font1.h"
#include "FAmap.h"	   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//2.4/2.8��/3.5�� TFTҺ������	  
//֧������IC�ͺŰ���:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/SPFD5408/SSD1289/1505/B505/C505��	    
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/7
//�汾��V2.2
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved	
//********************************************************************************
//V1.2�޸�˵��
//֧����SPFD5408������,�����Һ��IDֱ�Ӵ�ӡ��HEX��ʽ.����鿴LCD����IC.
//V1.3
//�����˿���IO��֧��
//�޸��˱�����Ƶļ��ԣ�������V1.8���Ժ�Ŀ�����汾��
//����1.8�汾֮ǰ(������1.8)��Һ��ģ��,���޸�LCD_Init������LCD_LED=1;ΪLCD_LED=1;
//V1.4
//�޸���LCD_ShowChar������ʹ�û��㹦�ܻ��ַ���
//�����˺�������ʾ��֧��
//V1.5 20110730
//1,�޸���B505Һ������ɫ�����bug.
//2,�޸��˿���IO�������������÷�ʽ.
//V1.6 20111116
//1,�����LGDP4535Һ��������֧��
//V1.7 20120713
//1,����LCD_RD_DATA����
//2,���Ӷ�ILI9341��֧��
//3,����ILI9325�Ķ�����������
//4,����LCD_Scan_Dir����(����ʹ��)	  
//6,�����޸��˲���ԭ���ĺ���,����Ӧ9341�Ĳ���
//V1.8 20120905
//1,����LCD��Ҫ�������ýṹ��lcddev
//2,����LCD_Display_Dir����,֧�����ߺ������л�
//V1.9 20120911
//1,����RM68042������ID:6804��������6804��֧�ֺ�����ʾ����ԭ�򣺸ı�ɨ�跽ʽ��
//����6804��������ʧЧ���Թ��ܶ෽�������У���ʱ�޽⡣
//V2.0 20120924
//�ڲ�Ӳ����λ�������,ILI9341��ID��ȡ�ᱻ�����9300,�޸�LCD_Init,���޷�ʶ��
//�����������IDΪ9300/�Ƿ�ID��,ǿ��ָ������ICΪILI9341��ִ��9341�ĳ�ʼ����
//V2.1 20120930
//����ILI9325����ɫ��bug��
//V2.2 20121007
//����LCD_Scan_Dir��bug��
//////////////////////////////////////////////////////////////////////////////////	 
				 
//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;
	
int font_color, x_font, y_font;
char highlight,fa_num;
int effect = 0, italic = 0, font_size = 1;
unsigned int highlight_color;
int x_text=0,y_text=0;
		   
//д�Ĵ�������
//regval:�Ĵ���ֵ
void LCD_WR_REG(u16 regval)
{ 
	LCD->LCD_REG=regval;//д��Ҫд�ļĴ������	 
}
//дLCD����
//data:Ҫд���ֵ
void LCD_WR_DATA(u16 data)
{										    	   
	LCD->LCD_RAM=data;		 
}
//��LCD����
//����ֵ:������ֵ
u16 LCD_RD_DATA(void)
{										    	   
	return LCD->LCD_RAM;		 
}
//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void LCD_Reg(u8 LCD_Reg)
{	
	LCD->LCD_REG = LCD_Reg;		//д��Ҫд�ļĴ������	 	    		 
}
//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//д��Ҫд�ļĴ������	 
	LCD->LCD_RAM = LCD_RegValue;//д������	    		 
}	   
//���Ĵ���
//LCD_Reg:�Ĵ�����ַ
//����ֵ:����������
u16 LCD_ReadReg(u8 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//д��Ҫ���ļĴ������
	delay_us(5);		  
	return LCD_RD_DATA();		//���ض�����ֵ
}   
//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}	 
//LCDдGRAM
//RGB_Code:��ɫֵ
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;//дʮ��λGRAM
}
//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
//��mdk -O1ʱ���Ż�ʱ��Ҫ����
//��ʱi
void opt_delay(u8 i)
{
	while(i--);
}
//��ȡ��ĳ�����ɫֵ	 
//x,y:����
//����ֵ:�˵����ɫ
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//�����˷�Χ,ֱ�ӷ���		   
	LCD_SetCursor(x,y);	    
	if(lcddev.id==0X9341||lcddev.id==0X6804)LCD_WR_REG(0X2E);//9341/6804 ���Ͷ�GRAMָ��
	else LCD_WR_REG(R34);      		 				//����IC���Ͷ�GRAMָ��
 	if(lcddev.id==0X9320)opt_delay(2);				//FOR 9320,��ʱ2us	    
	if(LCD->LCD_RAM)r=0;							//dummy Read	   
	opt_delay(2);	  
 	r=LCD->LCD_RAM;  		  						//ʵ��������ɫ
 	if(lcddev.id==0X9341)//9341Ҫ��2�ζ���
 	{
		opt_delay(2);	  
		b=LCD->LCD_RAM; 
		g=r&0XFF;//����9341,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
		g<<=8;
	}else if(lcddev.id==0X6804)r=LCD->LCD_RAM;//6804�ڶ��ζ�ȡ�Ĳ�����ʵֵ
	if(lcddev.id==0X9325||lcddev.id==0X4535||lcddev.id==0X4531||lcddev.id==0X8989||lcddev.id==0XB505)return r;//�⼸��ICֱ�ӷ�����ɫֵ
	else if(lcddev.id==0X9341)return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));	//ILI9341��Ҫ��ʽת��һ��
	else return LCD_BGR2RGB(r);												//����IC
}			 
//LCD������ʾ
void LCD_DisplayOn(void)
{					   
	if(lcddev.id==0X9341||lcddev.id==0X6804)LCD_WR_REG(0X29);	//������ʾ
	else LCD_WriteReg(R7,0x0173); 			//������ʾ
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	   
	if(lcddev.id==0X9341||lcddev.id==0X6804)LCD_WR_REG(0X28);	//�ر���ʾ
	else LCD_WriteReg(R7,0x0);//�ر���ʾ 
}   
//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
 	if(lcddev.id==0X9341||lcddev.id==0X6804)
	{		    
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8); 
		LCD_WR_DATA(Xpos&0XFF);	 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8); 
		LCD_WR_DATA(Ypos&0XFF);
	}else
	{
		if(lcddev.dir==1)Xpos=lcddev.width-1-Xpos;//������ʵ���ǵ�תx,y����
		LCD_WriteReg(lcddev.setxcmd, Xpos);
		LCD_WriteReg(lcddev.setycmd, Ypos);
	}	 
} 		 
//����LCD���Զ�ɨ�跽��
//ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341/6804����������),
//����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
//dir:0~7,����8������(���嶨���lcd.h)
//9320/9325/9328/4531/4535/1505/b505/8989/5408/9341��IC�Ѿ�ʵ�ʲ���	   	   
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u8 dirreg=0;
	u16 temp;  
	if(lcddev.dir==1&&lcddev.id!=0X6804)//����ʱ����6804���ı�ɨ�跽��
	{			   
		switch(dir)//����ת��
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	}
	if(lcddev.id==0x9341||lcddev.id==0X6804)//9341/6804,������
	{
		switch(dir)
		{
			case L2R_U2D://������,���ϵ���
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		dirreg=0X36;
 		regval|=0X08;//BGR   
		if(lcddev.id==0X6804)regval|=0x02;//6804��BIT6��9341�ķ���	   
		LCD_WriteReg(dirreg,regval);
 		if(regval&0X20)
		{
			if(lcddev.width<lcddev.height)//����X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}else  
		{
			if(lcddev.width>lcddev.height)//����X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}  
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
  	}else 
	{
		switch(dir)
		{
			case L2R_U2D://������,���ϵ���
				regval|=(1<<5)|(1<<4)|(0<<3); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(0<<5)|(1<<4)|(0<<3); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(0<<5)|(0<<4)|(0<<3); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(1<<5)|(1<<4)|(1<<3); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(1<<5)|(0<<4)|(1<<3); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(0<<5)|(1<<4)|(1<<3); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(0<<5)|(0<<4)|(1<<3); 
				break;	 
		}
		if(lcddev.id==0x8989)//8989 IC
		{
			dirreg=0X11;
			regval|=0X6040;	//65K   
	 	}else//��������IC		  
		{
			dirreg=0X03;
			regval|=1<<12;  
		}
		LCD_WriteReg(dirreg,regval);
	}
}   
//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD->LCD_RAM=POINT_COLOR; 
}

void myLCD_DrawPoint(u16 x,u16 y,u16 Point_color)
{
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD->LCD_RAM=Point_color; 
}
//���ٻ���
//x,y:����
//color:��ɫ
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
	if(lcddev.id==0X9341||lcddev.id==0X6804)
	{		    
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8); 
		LCD_WR_DATA(x&0XFF);	 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8); 
		LCD_WR_DATA(y&0XFF);
	}else
	{
 		if(lcddev.dir==1)x=lcddev.width-1-x;//������ʵ���ǵ�תx,y����
		LCD_WriteReg(lcddev.setxcmd,x);
		LCD_WriteReg(lcddev.setycmd,y);
	}			 
	LCD->LCD_REG=lcddev.wramcmd; 
	LCD->LCD_RAM=color; 
}	 


//����LCD��ʾ����6804��֧�ֺ�����ʾ��
//dir:0,������1,����
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)//����
	{
		lcddev.dir=0;//����
		lcddev.width=240;
		lcddev.height=320;
		if(lcddev.id==0X9341||lcddev.id==0X6804)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
			if(lcddev.id==0X6804)
			{
				lcddev.width=320;
				lcddev.height=480;
			}
		}else if(lcddev.id==0X8989)
		{
			lcddev.wramcmd=R34;
	 		lcddev.setxcmd=0X4E;
			lcddev.setycmd=0X4F;  
		}else
		{
			lcddev.wramcmd=R34;
	 		lcddev.setxcmd=R32;
			lcddev.setycmd=R33;  
		}
	}else if(lcddev.id!=0X6804)//6804��֧�ֺ�����ʾ	
	{	  
		lcddev.dir=1;//����
		lcddev.width=320;
		lcddev.height=240;
		if(lcddev.id==0X9341)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
		}else if(lcddev.id==0X8989)
		{
			lcddev.wramcmd=R34;
	 		lcddev.setxcmd=0X4F;
			lcddev.setycmd=0X4E;  
		}else
		{
			lcddev.wramcmd=R34;
	 		lcddev.setxcmd=R33;
			lcddev.setycmd=R32;  
		}
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
}	 
//��ʼ��lcd
//�ó�ʼ���������Գ�ʼ������ILI93XXҺ��,�������������ǻ���ILI9320��!!!
//�������ͺŵ�����оƬ��û�в���! 
void LCD_Init(void)
{ 										  
		GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;

    //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//ʹ��FSMCʱ��
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);//ʹ��PORTB,D,E,G�Լ�AFIO���ù���ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 //PB0 ������� ����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 		 //??????   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	//PORTD�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 		 //??????   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	  
	//PORTE�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 		 //??????   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	  
   	//	//PORTG12����������� A0	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;	 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 		 //??????   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure); 
 
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);	
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_FSMC);


	readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
    readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // ���ݱ���ʱ��Ϊ16��HCLK,��ΪҺ������IC�Ķ����ݵ�ʱ���ٶȲ���̫�죬�����1289���IC��
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
    

	writeTiming.FSMC_AddressSetupTime = 0x03;	 //��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK  
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨A		
    writeTiming.FSMC_DataSetupTime = 0x09;		 ////���ݱ���ʱ��Ϊ4��HCLK	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 

 
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  ��������ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]��
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // ���������ݵ�ַ
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit   
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  �洢��дʹ��
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // ��дʹ�ò�ͬ��ʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //��дʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //дʱ��

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����

   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // ʹ��BANK1 
			 
 	delay_ms(50); // delay 50 ms 
 	LCD_WriteReg(0x0000,0x0001);
	delay_ms(50); // delay 50 ms 
  	lcddev.id = LCD_ReadReg(0x0000);   
  	if(lcddev.id<0XFF||lcddev.id==0XFFFF||lcddev.id==0X9300)//����ID����ȷ,����lcddev.id==0X9300�жϣ���Ϊ9341��δ����λ������»ᱻ����9300
	{	
 		//����9341 ID�Ķ�ȡ		
		LCD_WR_REG(0XD3);				   
		LCD_RD_DATA(); 				//dummy read 	
 		LCD_RD_DATA();   	    	//����0X00
  		lcddev.id=LCD_RD_DATA();   	//��ȡ93								   
 		lcddev.id<<=8;
		lcddev.id|=LCD_RD_DATA();  	//��ȡ41 	   			   
 		if(lcddev.id!=0X9341)		//��9341,�����ǲ���6804
		{	
 			LCD_WR_REG(0XBF);				   
			LCD_RD_DATA(); 			//dummy read 	 
	 		LCD_RD_DATA();   	    //����0X01			   
	 		LCD_RD_DATA(); 			//����0XD0 			  	
	  		lcddev.id=LCD_RD_DATA();//�������0X68 
			lcddev.id<<=8;
	  		lcddev.id|=LCD_RD_DATA();//�������0X04	   	  
 		} 
		if(lcddev.id!=0X9341&&lcddev.id!=0X6804)lcddev.id=0x9341;//����������ʶ��9341 	     
	}
 	//printf(" LCD ID:%x\r\n",lcddev.id); //��ӡLCD ID  
	if(lcddev.id==0X9341)	//9341��ʼ��
	{	 
		LCD_WR_REG(0xCF);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0xC1); 
		LCD_WR_DATA(0X30); 
		LCD_WR_REG(0xED);  
		LCD_WR_DATA(0x64); 
		LCD_WR_DATA(0x03); 
		LCD_WR_DATA(0X12); 
		LCD_WR_DATA(0X81); 
		LCD_WR_REG(0xE8);  
		LCD_WR_DATA(0x85); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x7A); 
		LCD_WR_REG(0xCB);  
		LCD_WR_DATA(0x39); 
		LCD_WR_DATA(0x2C); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x34); 
		LCD_WR_DATA(0x02); 
		LCD_WR_REG(0xF7);  
		LCD_WR_DATA(0x20); 
		LCD_WR_REG(0xEA);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_REG(0xC0);    //Power control 
		LCD_WR_DATA(0x1B);   //VRH[5:0] 
		LCD_WR_REG(0xC1);    //Power control 
		LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
		LCD_WR_REG(0xC5);    //VCM control 
		LCD_WR_DATA(0x30); 	 //3F
		LCD_WR_DATA(0x30); 	 //3C
		LCD_WR_REG(0xC7);    //VCM control2 
		LCD_WR_DATA(0XB7); 
		LCD_WR_REG(0x36);    // Memory Access Control 
		LCD_WR_DATA(0x48); 
		LCD_WR_REG(0x3A);   
		LCD_WR_DATA(0x55); 
		LCD_WR_REG(0xB1);   
		LCD_WR_DATA(0x00);   
		LCD_WR_DATA(0x1A); 
		LCD_WR_REG(0xB6);    // Display Function Control 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0xA2); 
		LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
		LCD_WR_DATA(0x00); 
		LCD_WR_REG(0x26);    //Gamma curve selected 
		LCD_WR_DATA(0x01); 
		LCD_WR_REG(0xE0);    //Set Gamma 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x2A); 
		LCD_WR_DATA(0x28); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x0E); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x54); 
		LCD_WR_DATA(0XA9); 
		LCD_WR_DATA(0x43); 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 		 
		LCD_WR_REG(0XE1);    //Set Gamma 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x15); 
		LCD_WR_DATA(0x17); 
		LCD_WR_DATA(0x07); 
		LCD_WR_DATA(0x11); 
		LCD_WR_DATA(0x06); 
		LCD_WR_DATA(0x2B); 
		LCD_WR_DATA(0x56); 
		LCD_WR_DATA(0x3C); 
		LCD_WR_DATA(0x05); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_REG(0x2B); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x3f);
		LCD_WR_REG(0x2A); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xef);	 
		LCD_WR_REG(0x11); //Exit Sleep
		delay_ms(120);
		LCD_WR_REG(0x29); //display on	
	}else if(lcddev.id==0x6804) //6804��ʼ��
	{
		LCD_WR_REG(0X11);
		delay_ms(20);
		LCD_WR_REG(0XD0);//VCI1  VCL  VGH  VGL DDVDH VREG1OUT power amplitude setting
		LCD_WR_DATA(0X07); 
		LCD_WR_DATA(0X42); 
		LCD_WR_DATA(0X1D); 
		LCD_WR_REG(0XD1);//VCOMH VCOM_AC amplitude setting
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X1a);
		LCD_WR_DATA(0X09); 
		LCD_WR_REG(0XD2);//Operational Amplifier Circuit Constant Current Adjust , charge pump frequency setting
		LCD_WR_DATA(0X01);
		LCD_WR_DATA(0X22);
		LCD_WR_REG(0XC0);//REV SM GS 
		LCD_WR_DATA(0X10);
		LCD_WR_DATA(0X3B);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X02);
		LCD_WR_DATA(0X11);
		
		LCD_WR_REG(0XC5);// Frame rate setting = 72HZ  when setting 0x03
		LCD_WR_DATA(0X03);
		
		LCD_WR_REG(0XC8);//Gamma setting
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X25);
		LCD_WR_DATA(0X21);
		LCD_WR_DATA(0X05);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X0a);
		LCD_WR_DATA(0X65);
		LCD_WR_DATA(0X25);
		LCD_WR_DATA(0X77);
		LCD_WR_DATA(0X50);
		LCD_WR_DATA(0X0f);
		LCD_WR_DATA(0X00);	  
						  
   		LCD_WR_REG(0XF8);
		LCD_WR_DATA(0X01);	  

 		LCD_WR_REG(0XFE);
 		LCD_WR_DATA(0X00);
 		LCD_WR_DATA(0X02);
		
		LCD_WR_REG(0X20);//Exit invert mode

		LCD_WR_REG(0X36);
		LCD_WR_DATA(0X08);//ԭ����a
		
		LCD_WR_REG(0X3A);
		LCD_WR_DATA(0X55);//16λģʽ	  
		LCD_WR_REG(0X2B);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X01);
		LCD_WR_DATA(0X3F);
		
		LCD_WR_REG(0X2A);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X00);
		LCD_WR_DATA(0X01);
		LCD_WR_DATA(0XDF);
		delay_ms(120);
		LCD_WR_REG(0X29); 	 
 	}else if(lcddev.id==0x9325)//9325
	{
		LCD_WriteReg(0x00E5,0x78F0); 
		LCD_WriteReg(0x0001,0x0100); 
		LCD_WriteReg(0x0002,0x0700); 
		LCD_WriteReg(0x0003,0x1030); 
		LCD_WriteReg(0x0004,0x0000); 
		LCD_WriteReg(0x0008,0x0202);  
		LCD_WriteReg(0x0009,0x0000);
		LCD_WriteReg(0x000A,0x0000); 
		LCD_WriteReg(0x000C,0x0000); 
		LCD_WriteReg(0x000D,0x0000);
		LCD_WriteReg(0x000F,0x0000);
		//power on sequence VGHVGL
		LCD_WriteReg(0x0010,0x0000);   
		LCD_WriteReg(0x0011,0x0007);  
		LCD_WriteReg(0x0012,0x0000);  
		LCD_WriteReg(0x0013,0x0000); 
		LCD_WriteReg(0x0007,0x0000); 
		//vgh 
		LCD_WriteReg(0x0010,0x1690);   
		LCD_WriteReg(0x0011,0x0227);
		//delayms(100);
		//vregiout 
		LCD_WriteReg(0x0012,0x009D); //0x001b
		//delayms(100); 
		//vom amplitude
		LCD_WriteReg(0x0013,0x1900);
		//delayms(100); 
		//vom H
		LCD_WriteReg(0x0029,0x0025); 
		LCD_WriteReg(0x002B,0x000D); 
		//gamma
		LCD_WriteReg(0x0030,0x0007);
		LCD_WriteReg(0x0031,0x0303);
		LCD_WriteReg(0x0032,0x0003);// 0006
		LCD_WriteReg(0x0035,0x0206);
		LCD_WriteReg(0x0036,0x0008);
		LCD_WriteReg(0x0037,0x0406); 
		LCD_WriteReg(0x0038,0x0304);//0200
		LCD_WriteReg(0x0039,0x0007); 
		LCD_WriteReg(0x003C,0x0602);// 0504
		LCD_WriteReg(0x003D,0x0008); 
		//ram
		LCD_WriteReg(0x0050,0x0000); 
		LCD_WriteReg(0x0051,0x00EF);
		LCD_WriteReg(0x0052,0x0000); 
		LCD_WriteReg(0x0053,0x013F);  
		LCD_WriteReg(0x0060,0xA700); 
		LCD_WriteReg(0x0061,0x0001); 
		LCD_WriteReg(0x006A,0x0000); 
		//
		LCD_WriteReg(0x0080,0x0000); 
		LCD_WriteReg(0x0081,0x0000); 
		LCD_WriteReg(0x0082,0x0000); 
		LCD_WriteReg(0x0083,0x0000); 
		LCD_WriteReg(0x0084,0x0000); 
		LCD_WriteReg(0x0085,0x0000); 
		//
		LCD_WriteReg(0x0090,0x0010); 
		LCD_WriteReg(0x0092,0x0600); 
		
		LCD_WriteReg(0x0007,0x0133);
		LCD_WriteReg(0x00,0x0022);//
	}else if(lcddev.id==0x9328)//ILI9328   OK  
	{
  		LCD_WriteReg(0x00EC,0x108F);// internal timeing      
 		LCD_WriteReg(0x00EF,0x1234);// ADD        
		//LCD_WriteReg(0x00e7,0x0010);      
        //LCD_WriteReg(0x0000,0x0001);//�����ڲ�ʱ��
        LCD_WriteReg(0x0001,0x0100);     
        LCD_WriteReg(0x0002,0x0700);//��Դ����                    
		//LCD_WriteReg(0x0003,(1<<3)|(1<<4) ); 	//65K  RGB
		//DRIVE TABLE(�Ĵ��� 03H)
		//BIT3=AM BIT4:5=ID0:1
		//AM ID0 ID1   FUNCATION
		// 0  0   0	   R->L D->U
		// 1  0   0	   D->U	R->L
		// 0  1   0	   L->R D->U
		// 1  1   0    D->U	L->R
		// 0  0   1	   R->L U->D
		// 1  0   1    U->D	R->L
		// 0  1   1    L->R U->D �����������.
		// 1  1   1	   U->D	L->R
        LCD_WriteReg(0x0003,(1<<12)|(3<<4)|(0<<3) );//65K    
        LCD_WriteReg(0x0004,0x0000);                                   
        LCD_WriteReg(0x0008,0x0202);	           
        LCD_WriteReg(0x0009,0x0000);         
        LCD_WriteReg(0x000a,0x0000);//display setting         
        LCD_WriteReg(0x000c,0x0001);//display setting          
        LCD_WriteReg(0x000d,0x0000);//0f3c          
        LCD_WriteReg(0x000f,0x0000);
		//��Դ����
        LCD_WriteReg(0x0010,0x0000);   
        LCD_WriteReg(0x0011,0x0007);
        LCD_WriteReg(0x0012,0x0000);                                                                 
        LCD_WriteReg(0x0013,0x0000);                 
     	LCD_WriteReg(0x0007,0x0001);                 
       	delay_ms(50); 
        LCD_WriteReg(0x0010,0x1490);   
        LCD_WriteReg(0x0011,0x0227);
        delay_ms(50); 
        LCD_WriteReg(0x0012,0x008A);                  
        delay_ms(50); 
        LCD_WriteReg(0x0013,0x1a00);   
        LCD_WriteReg(0x0029,0x0006);
        LCD_WriteReg(0x002b,0x000d);
        delay_ms(50); 
        LCD_WriteReg(0x0020,0x0000);                                                            
        LCD_WriteReg(0x0021,0x0000);           
		delay_ms(50); 
		//٤��У��
        LCD_WriteReg(0x0030,0x0000); 
        LCD_WriteReg(0x0031,0x0604);   
        LCD_WriteReg(0x0032,0x0305);
        LCD_WriteReg(0x0035,0x0000);
        LCD_WriteReg(0x0036,0x0C09); 
        LCD_WriteReg(0x0037,0x0204);
        LCD_WriteReg(0x0038,0x0301);        
        LCD_WriteReg(0x0039,0x0707);     
        LCD_WriteReg(0x003c,0x0000);
        LCD_WriteReg(0x003d,0x0a0a);
        delay_ms(50); 
        LCD_WriteReg(0x0050,0x0000); //ˮƽGRAM��ʼλ�� 
        LCD_WriteReg(0x0051,0x00ef); //ˮƽGRAM��ֹλ��                    
        LCD_WriteReg(0x0052,0x0000); //��ֱGRAM��ʼλ��                    
        LCD_WriteReg(0x0053,0x013f); //��ֱGRAM��ֹλ��  
 
         LCD_WriteReg(0x0060,0xa700);        
        LCD_WriteReg(0x0061,0x0001); 
        LCD_WriteReg(0x006a,0x0000);
        LCD_WriteReg(0x0080,0x0000);
        LCD_WriteReg(0x0081,0x0000);
        LCD_WriteReg(0x0082,0x0000);
        LCD_WriteReg(0x0083,0x0000);
        LCD_WriteReg(0x0084,0x0000);
        LCD_WriteReg(0x0085,0x0000);
      
        LCD_WriteReg(0x0090,0x0010);     
        LCD_WriteReg(0x0092,0x0600);  
        //������ʾ����    
        LCD_WriteReg(0x0007,0x0133); 
	}else if(lcddev.id==0x9320)//����OK.
	{
		LCD_WriteReg(0x00,0x0000);
		LCD_WriteReg(0x01,0x0100);	//Driver Output Contral.
		LCD_WriteReg(0x02,0x0700);	//LCD Driver Waveform Contral.
		LCD_WriteReg(0x03,0x1030);//Entry Mode Set.
		//LCD_WriteReg(0x03,0x1018);	//Entry Mode Set.
	
		LCD_WriteReg(0x04,0x0000);	//Scalling Contral.
		LCD_WriteReg(0x08,0x0202);	//Display Contral 2.(0x0207)
		LCD_WriteReg(0x09,0x0000);	//Display Contral 3.(0x0000)
		LCD_WriteReg(0x0a,0x0000);	//Frame Cycle Contal.(0x0000)
		LCD_WriteReg(0x0c,(1<<0));	//Extern Display Interface Contral 1.(0x0000)
		LCD_WriteReg(0x0d,0x0000);	//Frame Maker Position.
		LCD_WriteReg(0x0f,0x0000);	//Extern Display Interface Contral 2.	    
		delay_ms(50); 
		LCD_WriteReg(0x07,0x0101);	//Display Contral.
		delay_ms(50); 								  
		LCD_WriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	//Power Control 1.(0x16b0)
		LCD_WriteReg(0x11,0x0007);								//Power Control 2.(0x0001)
		LCD_WriteReg(0x12,(1<<8)|(1<<4)|(0<<0));				//Power Control 3.(0x0138)
		LCD_WriteReg(0x13,0x0b00);								//Power Control 4.
		LCD_WriteReg(0x29,0x0000);								//Power Control 7.
	
		LCD_WriteReg(0x2b,(1<<14)|(1<<4));	    
		LCD_WriteReg(0x50,0);	//Set X Star
		//ˮƽGRAM��ֹλ��Set X End.
		LCD_WriteReg(0x51,239);	//Set Y Star
		LCD_WriteReg(0x52,0);	//Set Y End.t.
		LCD_WriteReg(0x53,319);	//
	
		LCD_WriteReg(0x60,0x2700);	//Driver Output Control.
		LCD_WriteReg(0x61,0x0001);	//Driver Output Control.
		LCD_WriteReg(0x6a,0x0000);	//Vertical Srcoll Control.
	
		LCD_WriteReg(0x80,0x0000);	//Display Position? Partial Display 1.
		LCD_WriteReg(0x81,0x0000);	//RAM Address Start? Partial Display 1.
		LCD_WriteReg(0x82,0x0000);	//RAM Address End-Partial Display 1.
		LCD_WriteReg(0x83,0x0000);	//Displsy Position? Partial Display 2.
		LCD_WriteReg(0x84,0x0000);	//RAM Address Start? Partial Display 2.
		LCD_WriteReg(0x85,0x0000);	//RAM Address End? Partial Display 2.
	
		LCD_WriteReg(0x90,(0<<7)|(16<<0));	//Frame Cycle Contral.(0x0013)
		LCD_WriteReg(0x92,0x0000);	//Panel Interface Contral 2.(0x0000)
		LCD_WriteReg(0x93,0x0001);	//Panel Interface Contral 3.
		LCD_WriteReg(0x95,0x0110);	//Frame Cycle Contral.(0x0110)
		LCD_WriteReg(0x97,(0<<8));	//
		LCD_WriteReg(0x98,0x0000);	//Frame Cycle Contral.	   
		LCD_WriteReg(0x07,0x0173);	//(0x0173)
	}else if(lcddev.id==0X9331)//OK |/|/|			 
	{
		LCD_WriteReg(0x00E7, 0x1014);
		LCD_WriteReg(0x0001, 0x0100); // set SS and SM bit
		LCD_WriteReg(0x0002, 0x0200); // set 1 line inversion
        LCD_WriteReg(0x0003,(1<<12)|(3<<4)|(1<<3));//65K    
		//LCD_WriteReg(0x0003, 0x1030); // set GRAM write direction and BGR=1.
		LCD_WriteReg(0x0008, 0x0202); // set the back porch and front porch
		LCD_WriteReg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
		LCD_WriteReg(0x000A, 0x0000); // FMARK function
		LCD_WriteReg(0x000C, 0x0000); // RGB interface setting
		LCD_WriteReg(0x000D, 0x0000); // Frame marker Position
		LCD_WriteReg(0x000F, 0x0000); // RGB interface polarity
		//*************Power On sequence ****************//
		LCD_WriteReg(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_WriteReg(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
		LCD_WriteReg(0x0012, 0x0000); // VREG1OUT voltage
		LCD_WriteReg(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
		delay_ms(200); // Dis-charge capacitor power voltage
		LCD_WriteReg(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_WriteReg(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
		delay_ms(50); // Delay 50ms
		LCD_WriteReg(0x0012, 0x000C); // Internal reference voltage= Vci;
		delay_ms(50); // Delay 50ms
		LCD_WriteReg(0x0013, 0x0800); // Set VDV[4:0] for VCOM amplitude
		LCD_WriteReg(0x0029, 0x0011); // Set VCM[5:0] for VCOMH
		LCD_WriteReg(0x002B, 0x000B); // Set Frame Rate
		delay_ms(50); // Delay 50ms
		LCD_WriteReg(0x0020, 0x0000); // GRAM horizontal Address
		LCD_WriteReg(0x0021, 0x013f); // GRAM Vertical Address
		// ----------- Adjust the Gamma Curve ----------//
		LCD_WriteReg(0x0030, 0x0000);
		LCD_WriteReg(0x0031, 0x0106);
		LCD_WriteReg(0x0032, 0x0000);
		LCD_WriteReg(0x0035, 0x0204);
		LCD_WriteReg(0x0036, 0x160A);
		LCD_WriteReg(0x0037, 0x0707);
		LCD_WriteReg(0x0038, 0x0106);
		LCD_WriteReg(0x0039, 0x0707);
		LCD_WriteReg(0x003C, 0x0402);
		LCD_WriteReg(0x003D, 0x0C0F);
		//------------------ Set GRAM area ---------------//
		LCD_WriteReg(0x0050, 0x0000); // Horizontal GRAM Start Address
		LCD_WriteReg(0x0051, 0x00EF); // Horizontal GRAM End Address
		LCD_WriteReg(0x0052, 0x0000); // Vertical GRAM Start Address
		LCD_WriteReg(0x0053, 0x013F); // Vertical GRAM Start Address
		LCD_WriteReg(0x0060, 0x2700); // Gate Scan Line
		LCD_WriteReg(0x0061, 0x0001); // NDL,VLE, REV 
		LCD_WriteReg(0x006A, 0x0000); // set scrolling line
		//-------------- Partial Display Control ---------//
		LCD_WriteReg(0x0080, 0x0000);
		LCD_WriteReg(0x0081, 0x0000);
		LCD_WriteReg(0x0082, 0x0000);
		LCD_WriteReg(0x0083, 0x0000);
		LCD_WriteReg(0x0084, 0x0000);
		LCD_WriteReg(0x0085, 0x0000);
		//-------------- Panel Control -------------------//
		LCD_WriteReg(0x0090, 0x0010);
		LCD_WriteReg(0x0092, 0x0600);
		LCD_WriteReg(0x0007, 0x0133); // 262K color and display ON
	}else if(lcddev.id==0x5408)
	{
		LCD_WriteReg(0x01,0x0100);								  
		LCD_WriteReg(0x02,0x0700);//LCD Driving Waveform Contral 
		LCD_WriteReg(0x03,0x1030);//Entry Mode���� 	   
		//ָ������������϶��µ��Զ���ģʽ
		//Normal Mode(Window Mode disable)
		//RGB��ʽ
		//16λ����2�δ����8��������
		LCD_WriteReg(0x04,0x0000); //Scalling Control register     
		LCD_WriteReg(0x08,0x0207); //Display Control 2 
		LCD_WriteReg(0x09,0x0000); //Display Control 3	 
		LCD_WriteReg(0x0A,0x0000); //Frame Cycle Control	 
		LCD_WriteReg(0x0C,0x0000); //External Display Interface Control 1 
		LCD_WriteReg(0x0D,0x0000); //Frame Maker Position		 
		LCD_WriteReg(0x0F,0x0000); //External Display Interface Control 2 
 		delay_ms(20);
		//TFT Һ����ɫͼ����ʾ����14
		LCD_WriteReg(0x10,0x16B0); //0x14B0 //Power Control 1
		LCD_WriteReg(0x11,0x0001); //0x0007 //Power Control 2
		LCD_WriteReg(0x17,0x0001); //0x0000 //Power Control 3
		LCD_WriteReg(0x12,0x0138); //0x013B //Power Control 4
		LCD_WriteReg(0x13,0x0800); //0x0800 //Power Control 5
		LCD_WriteReg(0x29,0x0009); //NVM read data 2
		LCD_WriteReg(0x2a,0x0009); //NVM read data 3
		LCD_WriteReg(0xa4,0x0000);	 
		LCD_WriteReg(0x50,0x0000); //���ò������ڵ�X�Ὺʼ��
		LCD_WriteReg(0x51,0x00EF); //���ò������ڵ�X�������
		LCD_WriteReg(0x52,0x0000); //���ò������ڵ�Y�Ὺʼ��
		LCD_WriteReg(0x53,0x013F); //���ò������ڵ�Y�������
		LCD_WriteReg(0x60,0x2700); //Driver Output Control
		//������Ļ�ĵ����Լ�ɨ�����ʼ��
		LCD_WriteReg(0x61,0x0001); //Driver Output Control
		LCD_WriteReg(0x6A,0x0000); //Vertical Scroll Control
		LCD_WriteReg(0x80,0x0000); //Display Position �C Partial Display 1
		LCD_WriteReg(0x81,0x0000); //RAM Address Start �C Partial Display 1
		LCD_WriteReg(0x82,0x0000); //RAM address End - Partial Display 1
		LCD_WriteReg(0x83,0x0000); //Display Position �C Partial Display 2
		LCD_WriteReg(0x84,0x0000); //RAM Address Start �C Partial Display 2
		LCD_WriteReg(0x85,0x0000); //RAM address End �C Partail Display2
		LCD_WriteReg(0x90,0x0013); //Frame Cycle Control
		LCD_WriteReg(0x92,0x0000);  //Panel Interface Control 2
		LCD_WriteReg(0x93,0x0003); //Panel Interface control 3
		LCD_WriteReg(0x95,0x0110);  //Frame Cycle Control
		LCD_WriteReg(0x07,0x0173);		 
		delay_ms(50);
	}	
	else if(lcddev.id==0x1505)//OK
	{
		// second release on 3/5  ,luminance is acceptable,water wave appear during camera preview
        LCD_WriteReg(0x0007,0x0000);
        delay_ms(50); 
        LCD_WriteReg(0x0012,0x011C);//0x011A   why need to set several times?
        LCD_WriteReg(0x00A4,0x0001);//NVM	 
        LCD_WriteReg(0x0008,0x000F);
        LCD_WriteReg(0x000A,0x0008);
        LCD_WriteReg(0x000D,0x0008);	    
  		//٤��У��
        LCD_WriteReg(0x0030,0x0707);
        LCD_WriteReg(0x0031,0x0007); //0x0707
        LCD_WriteReg(0x0032,0x0603); 
        LCD_WriteReg(0x0033,0x0700); 
        LCD_WriteReg(0x0034,0x0202); 
        LCD_WriteReg(0x0035,0x0002); //?0x0606
        LCD_WriteReg(0x0036,0x1F0F);
        LCD_WriteReg(0x0037,0x0707); //0x0f0f  0x0105
        LCD_WriteReg(0x0038,0x0000); 
        LCD_WriteReg(0x0039,0x0000); 
        LCD_WriteReg(0x003A,0x0707); 
        LCD_WriteReg(0x003B,0x0000); //0x0303
        LCD_WriteReg(0x003C,0x0007); //?0x0707
        LCD_WriteReg(0x003D,0x0000); //0x1313//0x1f08
        delay_ms(50); 
        LCD_WriteReg(0x0007,0x0001);
        LCD_WriteReg(0x0017,0x0001);//������Դ
        delay_ms(50); 
  		//��Դ����
        LCD_WriteReg(0x0010,0x17A0); 
        LCD_WriteReg(0x0011,0x0217);//reference voltage VC[2:0]   Vciout = 1.00*Vcivl
        LCD_WriteReg(0x0012,0x011E);//0x011c  //Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?
        LCD_WriteReg(0x0013,0x0F00);//VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl
        LCD_WriteReg(0x002A,0x0000);  
        LCD_WriteReg(0x0029,0x000A);//0x0001F  Vcomh = VCM1[4:0]*Vreg1out    gate source voltage??
        LCD_WriteReg(0x0012,0x013E);// 0x013C  power supply on
        //Coordinates Control//
        LCD_WriteReg(0x0050,0x0000);//0x0e00
        LCD_WriteReg(0x0051,0x00EF); 
        LCD_WriteReg(0x0052,0x0000); 
        LCD_WriteReg(0x0053,0x013F); 
    	//Pannel Image Control//
        LCD_WriteReg(0x0060,0x2700); 
        LCD_WriteReg(0x0061,0x0001); 
        LCD_WriteReg(0x006A,0x0000); 
        LCD_WriteReg(0x0080,0x0000); 
    	//Partial Image Control//
        LCD_WriteReg(0x0081,0x0000); 
        LCD_WriteReg(0x0082,0x0000); 
        LCD_WriteReg(0x0083,0x0000); 
        LCD_WriteReg(0x0084,0x0000); 
        LCD_WriteReg(0x0085,0x0000); 
  		//Panel Interface Control//
        LCD_WriteReg(0x0090,0x0013);//0x0010 frenqucy
        LCD_WriteReg(0x0092,0x0300); 
        LCD_WriteReg(0x0093,0x0005); 
        LCD_WriteReg(0x0095,0x0000); 
        LCD_WriteReg(0x0097,0x0000); 
        LCD_WriteReg(0x0098,0x0000); 
  
        LCD_WriteReg(0x0001,0x0100); 
        LCD_WriteReg(0x0002,0x0700); 
        LCD_WriteReg(0x0003,0x1038);//ɨ�跽�� ��->��  ��->�� 
        LCD_WriteReg(0x0004,0x0000); 
        LCD_WriteReg(0x000C,0x0000); 
        LCD_WriteReg(0x000F,0x0000); 
        LCD_WriteReg(0x0020,0x0000); 
        LCD_WriteReg(0x0021,0x0000); 
        LCD_WriteReg(0x0007,0x0021); 
        delay_ms(20);
        LCD_WriteReg(0x0007,0x0061); 
        delay_ms(20);
        LCD_WriteReg(0x0007,0x0173); 
        delay_ms(20);
	}else if(lcddev.id==0xB505)
	{
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		
		LCD_WriteReg(0x00a4,0x0001);
		delay_ms(20);		  
		LCD_WriteReg(0x0060,0x2700);
		LCD_WriteReg(0x0008,0x0202);
		
		LCD_WriteReg(0x0030,0x0214);
		LCD_WriteReg(0x0031,0x3715);
		LCD_WriteReg(0x0032,0x0604);
		LCD_WriteReg(0x0033,0x0e16);
		LCD_WriteReg(0x0034,0x2211);
		LCD_WriteReg(0x0035,0x1500);
		LCD_WriteReg(0x0036,0x8507);
		LCD_WriteReg(0x0037,0x1407);
		LCD_WriteReg(0x0038,0x1403);
		LCD_WriteReg(0x0039,0x0020);
		
		LCD_WriteReg(0x0090,0x001a);
		LCD_WriteReg(0x0010,0x0000);
		LCD_WriteReg(0x0011,0x0007);
		LCD_WriteReg(0x0012,0x0000);
		LCD_WriteReg(0x0013,0x0000);
		delay_ms(20);
		
		LCD_WriteReg(0x0010,0x0730);
		LCD_WriteReg(0x0011,0x0137);
		delay_ms(20);
		
		LCD_WriteReg(0x0012,0x01b8);
		delay_ms(20);
		
		LCD_WriteReg(0x0013,0x0f00);
		LCD_WriteReg(0x002a,0x0080);
		LCD_WriteReg(0x0029,0x0048);
		delay_ms(20);
		
		LCD_WriteReg(0x0001,0x0100);
		LCD_WriteReg(0x0002,0x0700);
        LCD_WriteReg(0x0003,0x1038);//ɨ�跽�� ��->��  ��->�� 
		LCD_WriteReg(0x0008,0x0202);
		LCD_WriteReg(0x000a,0x0000);
		LCD_WriteReg(0x000c,0x0000);
		LCD_WriteReg(0x000d,0x0000);
		LCD_WriteReg(0x000e,0x0030);
		LCD_WriteReg(0x0050,0x0000);
		LCD_WriteReg(0x0051,0x00ef);
		LCD_WriteReg(0x0052,0x0000);
		LCD_WriteReg(0x0053,0x013f);
		LCD_WriteReg(0x0060,0x2700);
		LCD_WriteReg(0x0061,0x0001);
		LCD_WriteReg(0x006a,0x0000);
		//LCD_WriteReg(0x0080,0x0000);
		//LCD_WriteReg(0x0081,0x0000);
		LCD_WriteReg(0x0090,0X0011);
		LCD_WriteReg(0x0092,0x0600);
		LCD_WriteReg(0x0093,0x0402);
		LCD_WriteReg(0x0094,0x0002);
		delay_ms(20);
		
		LCD_WriteReg(0x0007,0x0001);
		delay_ms(20);
		LCD_WriteReg(0x0007,0x0061);
		LCD_WriteReg(0x0007,0x0173);
		
		LCD_WriteReg(0x0020,0x0000);
		LCD_WriteReg(0x0021,0x0000);	  
		LCD_WriteReg(0x00,0x22);  
	}else if(lcddev.id==0xC505)
	{
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		delay_ms(20);		  
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
		LCD_WriteReg(0x0000,0x0000);
 		LCD_WriteReg(0x00a4,0x0001);
		delay_ms(20);		  
		LCD_WriteReg(0x0060,0x2700);
		LCD_WriteReg(0x0008,0x0806);
		
		LCD_WriteReg(0x0030,0x0703);//gamma setting
		LCD_WriteReg(0x0031,0x0001);
		LCD_WriteReg(0x0032,0x0004);
		LCD_WriteReg(0x0033,0x0102);
		LCD_WriteReg(0x0034,0x0300);
		LCD_WriteReg(0x0035,0x0103);
		LCD_WriteReg(0x0036,0x001F);
		LCD_WriteReg(0x0037,0x0703);
		LCD_WriteReg(0x0038,0x0001);
		LCD_WriteReg(0x0039,0x0004);
		
		
		
		LCD_WriteReg(0x0090, 0x0015);	//80Hz
		LCD_WriteReg(0x0010, 0X0410);	//BT,AP
		LCD_WriteReg(0x0011,0x0247);	//DC1,DC0,VC
		LCD_WriteReg(0x0012, 0x01BC);
		LCD_WriteReg(0x0013, 0x0e00);
		delay_ms(120);
		LCD_WriteReg(0x0001, 0x0100);
		LCD_WriteReg(0x0002, 0x0200);
		LCD_WriteReg(0x0003, 0x1030);
		
		LCD_WriteReg(0x000A, 0x0008);
		LCD_WriteReg(0x000C, 0x0000);
		
		LCD_WriteReg(0x000E, 0x0020);
		LCD_WriteReg(0x000F, 0x0000);
		LCD_WriteReg(0x0020, 0x0000);	//H Start
		LCD_WriteReg(0x0021, 0x0000);	//V Start
		LCD_WriteReg(0x002A,0x003D);	//vcom2
		delay_ms(20);
		LCD_WriteReg(0x0029, 0x002d);
		LCD_WriteReg(0x0050, 0x0000);
		LCD_WriteReg(0x0051, 0xD0EF);
		LCD_WriteReg(0x0052, 0x0000);
		LCD_WriteReg(0x0053, 0x013F);
		LCD_WriteReg(0x0061, 0x0000);
		LCD_WriteReg(0x006A, 0x0000);
		LCD_WriteReg(0x0092,0x0300); 
 
 		LCD_WriteReg(0x0093, 0x0005);
		LCD_WriteReg(0x0007, 0x0100);
	}else if(lcddev.id==0x8989)//OK |/|/|
	{	   
		LCD_WriteReg(0x0000,0x0001);//�򿪾���
    	LCD_WriteReg(0x0003,0xA8A4);//0xA8A4
    	LCD_WriteReg(0x000C,0x0000);    
    	LCD_WriteReg(0x000D,0x080C);   
    	LCD_WriteReg(0x000E,0x2B00);    
    	LCD_WriteReg(0x001E,0x00B0);    
    	LCD_WriteReg(0x0001,0x2B3F);//�����������320*240  0x6B3F
    	LCD_WriteReg(0x0002,0x0600);
    	LCD_WriteReg(0x0010,0x0000);  
    	LCD_WriteReg(0x0011,0x6078); //�������ݸ�ʽ  16λɫ 		���� 0x6058
    	LCD_WriteReg(0x0005,0x0000);  
    	LCD_WriteReg(0x0006,0x0000);  
    	LCD_WriteReg(0x0016,0xEF1C);  
    	LCD_WriteReg(0x0017,0x0003);  
    	LCD_WriteReg(0x0007,0x0233); //0x0233       
    	LCD_WriteReg(0x000B,0x0000);  
    	LCD_WriteReg(0x000F,0x0000); //ɨ�迪ʼ��ַ
    	LCD_WriteReg(0x0041,0x0000);  
    	LCD_WriteReg(0x0042,0x0000);  
    	LCD_WriteReg(0x0048,0x0000);  
    	LCD_WriteReg(0x0049,0x013F);  
    	LCD_WriteReg(0x004A,0x0000);  
    	LCD_WriteReg(0x004B,0x0000);  
    	LCD_WriteReg(0x0044,0xEF00);  
    	LCD_WriteReg(0x0045,0x0000);  
    	LCD_WriteReg(0x0046,0x013F);  
    	LCD_WriteReg(0x0030,0x0707);  
    	LCD_WriteReg(0x0031,0x0204);  
    	LCD_WriteReg(0x0032,0x0204);  
    	LCD_WriteReg(0x0033,0x0502);  
    	LCD_WriteReg(0x0034,0x0507);  
    	LCD_WriteReg(0x0035,0x0204);  
    	LCD_WriteReg(0x0036,0x0204);  
    	LCD_WriteReg(0x0037,0x0502);  
    	LCD_WriteReg(0x003A,0x0302);  
    	LCD_WriteReg(0x003B,0x0302);  
    	LCD_WriteReg(0x0023,0x0000);  
    	LCD_WriteReg(0x0024,0x0000);  
    	LCD_WriteReg(0x0025,0x8000);  
    	LCD_WriteReg(0x004f,0);        //����ַ0
    	LCD_WriteReg(0x004e,0);        //����ַ0
	}else if(lcddev.id==0x4531)//OK |/|/|
	{
		LCD_WriteReg(0X00,0X0001);   
		delay_ms(10);   
		LCD_WriteReg(0X10,0X1628);   
		LCD_WriteReg(0X12,0X000e);//0x0006    
		LCD_WriteReg(0X13,0X0A39);   
		delay_ms(10);   
		LCD_WriteReg(0X11,0X0040);   
		LCD_WriteReg(0X15,0X0050);   
		delay_ms(10);   
		LCD_WriteReg(0X12,0X001e);//16    
		delay_ms(10);   
		LCD_WriteReg(0X10,0X1620);   
		LCD_WriteReg(0X13,0X2A39);   
		delay_ms(10);   
		LCD_WriteReg(0X01,0X0100);   
		LCD_WriteReg(0X02,0X0300);   
		LCD_WriteReg(0X03,0X1038);//�ı䷽���   
		LCD_WriteReg(0X08,0X0202);   
		LCD_WriteReg(0X0A,0X0008);   
		LCD_WriteReg(0X30,0X0000);   
		LCD_WriteReg(0X31,0X0402);   
		LCD_WriteReg(0X32,0X0106);   
		LCD_WriteReg(0X33,0X0503);   
		LCD_WriteReg(0X34,0X0104);   
		LCD_WriteReg(0X35,0X0301);   
		LCD_WriteReg(0X36,0X0707);   
		LCD_WriteReg(0X37,0X0305);   
		LCD_WriteReg(0X38,0X0208);   
		LCD_WriteReg(0X39,0X0F0B);   
		LCD_WriteReg(0X41,0X0002);   
		LCD_WriteReg(0X60,0X2700);   
		LCD_WriteReg(0X61,0X0001);   
		LCD_WriteReg(0X90,0X0210);   
		LCD_WriteReg(0X92,0X010A);   
		LCD_WriteReg(0X93,0X0004);   
		LCD_WriteReg(0XA0,0X0100);   
		LCD_WriteReg(0X07,0X0001);   
		LCD_WriteReg(0X07,0X0021);   
		LCD_WriteReg(0X07,0X0023);   
		LCD_WriteReg(0X07,0X0033);   
		LCD_WriteReg(0X07,0X0133);   
		LCD_WriteReg(0XA0,0X0000); 
	}else if(lcddev.id==0x4535)
	{			      
		LCD_WriteReg(0X15,0X0030);   
		LCD_WriteReg(0X9A,0X0010);   
 		LCD_WriteReg(0X11,0X0020);   
 		LCD_WriteReg(0X10,0X3428);   
		LCD_WriteReg(0X12,0X0002);//16    
 		LCD_WriteReg(0X13,0X1038);   
		delay_ms(40);   
		LCD_WriteReg(0X12,0X0012);//16    
		delay_ms(40);   
  		LCD_WriteReg(0X10,0X3420);   
 		LCD_WriteReg(0X13,0X3038);   
		delay_ms(70);   
		LCD_WriteReg(0X30,0X0000);   
		LCD_WriteReg(0X31,0X0402);   
		LCD_WriteReg(0X32,0X0307);   
		LCD_WriteReg(0X33,0X0304);   
		LCD_WriteReg(0X34,0X0004);   
		LCD_WriteReg(0X35,0X0401);   
		LCD_WriteReg(0X36,0X0707);   
		LCD_WriteReg(0X37,0X0305);   
		LCD_WriteReg(0X38,0X0610);   
		LCD_WriteReg(0X39,0X0610); 
		  
		LCD_WriteReg(0X01,0X0100);   
		LCD_WriteReg(0X02,0X0300);   
		LCD_WriteReg(0X03,0X1030);//�ı䷽���   
		LCD_WriteReg(0X08,0X0808);   
		LCD_WriteReg(0X0A,0X0008);   
 		LCD_WriteReg(0X60,0X2700);   
		LCD_WriteReg(0X61,0X0001);   
		LCD_WriteReg(0X90,0X013E);   
		LCD_WriteReg(0X92,0X0100);   
		LCD_WriteReg(0X93,0X0100);   
 		LCD_WriteReg(0XA0,0X3000);   
 		LCD_WriteReg(0XA3,0X0010);   
		LCD_WriteReg(0X07,0X0001);   
		LCD_WriteReg(0X07,0X0021);   
		LCD_WriteReg(0X07,0X0023);   
		LCD_WriteReg(0X07,0X0033);   
		LCD_WriteReg(0X07,0X0133);   
	}
#ifdef LANDSCAPE
	LCD_Display_Dir(1);		 	//Ĭ��Ϊ����
#else 
	LCD_Display_Dir(0);
#endif
	GPIOB->BSRRH = GPIO_Pin_10;					//��������
	LCD_Clear(WHITE);
	//LCD_Clear(BLACK);
}  
//��������
//color:Ҫ���������ɫ
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 	//�õ��ܵ���
    
    LCD_WriteReg(TS_INS_DISP_CTRL1,0x0123);
    delay_ms(10);
    
	LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
	LCD_WriteRAM_Prepare();     //��ʼд��GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM=color;	   
	}
    
    LCD_WriteReg(TS_INS_DISP_CTRL1,0x0133);
    delay_ms(10);
}  
//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;

    //LCD_WriteReg(TS_INS_DISP_CTRL1,0x0123);
    //delay_ms(10);   
    
	for(i=sy;i<=ey;i++)
	{
	 	LCD_SetCursor(sx,i);      				//���ù��λ�� 
		LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
		for(j=0;j<xlen;j++) 
            LCD_WR_DATA(color);	//���ù��λ�� 	    
	}
    
    //LCD_WriteReg(TS_INS_DISP_CTRL1,0x0133);
    //delay_ms(10);
}  
//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 		//�õ����Ŀ��
	height=ey-sy+1;		//�߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		for(j=0;j<width;j++)LCD->LCD_RAM=color[i*height+j];//д������ 
	}	  
}  
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void myLCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		myLCD_DrawPoint(uRow, uCol, color);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    

void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
void myLCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u8 fill, u16 color)
{
	if(x1 < 0) 
		x1 = 0;
	if(y1 < 0) 
		y1 = 0;
	
	if(x2 > WIDTH-1) 
		x2 = WIDTH - 1;
	if(y2 > HEIGHT - 1) 
		y2 = HEIGHT - 1;

	 
	if(fill)
	{
		int counter_x,counter_y; 
		 
		for(counter_y = y1;counter_y <= y2;counter_y++)
		  for(counter_x = x1;counter_x <= x2;counter_x++)
			myLCD_DrawPoint(counter_x,counter_y,color);
	} 
	else
	{
		myLCD_DrawLine(x1,y1,x2,y1, color);
		myLCD_DrawLine(x1,y1,x1,y2, color);
		myLCD_DrawLine(x1,y2,x2,y2, color);
		myLCD_DrawLine(x2,y1,x2,y2, color);
	}
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 Point_Color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		myLCD_DrawPoint(x0+a,y0-b,Point_Color);             //5
 		myLCD_DrawPoint(x0+b,y0-a,Point_Color);             //0           
		myLCD_DrawPoint(x0+b,y0+a,Point_Color);             //4               
		myLCD_DrawPoint(x0+a,y0+b,Point_Color);             //6 
		myLCD_DrawPoint(x0-a,y0+b,Point_Color);             //1       
 		myLCD_DrawPoint(x0-b,y0+a,Point_Color);             
		myLCD_DrawPoint(x0-a,y0-b,Point_Color);             //2             
  		myLCD_DrawPoint(x0-b,y0-a,Point_Color);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_MyShowChar3(u16 x,u16 y,u8 num,u16 size,u8 mode,u16 Point_color,u16 Back_color)
{
    unsigned int font_pixel, font_Height, font_Width, i, j;
    int k;
    unsigned char Temp;
    
    if(size==48) 
    {    
        font_pixel=asc2_6432[num*2][0];
        font_Height=asc2_6432[num*2][1];
        font_Width=asc2_6432[num*2][2];
    }
    for(i=0;i<font_Height;i++)
    {
        for(j=0;j<font_Width;j++)
        {
            Temp=asc2_6432[(num*2)+1][(i*font_Width)+j];
            for(k=7;k>=0;k--)
            {
                if(!mode)
                {
                    if(((Temp & (0x01<<k))>>k) == 1)
                        myLCD_DrawPoint(x+(j*8)+(7-k),y+i,Point_color);
                    else
                        myLCD_DrawPoint(x+(j*8)+(7-k),y+i,Back_color);
                }
                else
                {
                    if(((Temp & (0x01<<k))>>k) == 1)
                        myLCD_DrawPoint(x+(j*8)+(7-k),y+i,Point_color);
                }
            }
        }
    }
    
}
void LCD_MyShowChar(u16 x,u16 y,u8 num,u16 size,u8 mode,u16 Point_color,u16 Back_color)
{  							  
    u16 temp,t1,t;
	u16 y0=y;
	u16 colortemp=POINT_COLOR;      			     
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	if(!mode) //�ǵ��ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else if(size==16) temp=asc2_1608[num][t];	            
	        for(t1=0;t1<(size/2);t1++)
			{			    
		        if(temp&0x80)POINT_COLOR=Point_color; //POINT_COLOR=colortemp;
				else POINT_COLOR=Back_color; //POINT_COLOR=BACK_COLOR;
				LCD_DrawPoint(x,y);	
				temp<<=1;
				y++;
				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}  	 
	    }    
	}else//���ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else temp=asc2_1608[num][t];		 //����1608���� 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)myLCD_DrawPoint(x,y,Point_color);   //LCD_DrawPoint(x,y); 
				temp<<=1;
				y++;
				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}  	 
	    }     
	}
	POINT_COLOR=colortemp;	    	   	 	  
}  

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u16 colortemp=POINT_COLOR;      			     
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	if(!mode) //�ǵ��ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else temp=asc2_1608[num][t];		 //����1608���� 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80) POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_DrawPoint(x,y);	
				temp<<=1;
				y++;
				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}  	 
	    }    
	}else//���ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else temp=asc2_1608[num][t];		 //����1608���� 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)LCD_DrawPoint(x,y); 
				temp<<=1;
				y++;
				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}  	 
	    }     
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   
//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}		


//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}
void LCD_ShowStringWithSize(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u8 str_size)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while(str_size != 0)//�ж��ǲ��ǷǷ��ַ�!
    {       
        str_size--;
		if(x>=WIDTH)
		{
			x=x0;
			y+=size;
		}
		
        if(y>=HEIGHT)
			break;//�˳�
		
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}
void LCD_MyShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u16 Point_color,u16 Back_color)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_MyShowChar(x,y,*p,size,0,Point_color,Back_color);
        x+=size/2;
        p++;
    }  
}

void LCD_MyShowString2(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u16 Point_color)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_MyShowChar(x,y,*p,size,1,Point_color,BACK_COLOR);
        x+=size/2;
        p++;
    }  
}

void LCD_MyShowString3 (u16 x, u16 y, u8 size, u8 *p, u16 Point_color, u16 Back_color)
{
    char Ch,Space;
    int i=0;
    
    while(p[i] != 0)
    {
        switch(p[i])
        {
            case '0': 
                Ch=0;
                Space = size/2;
                break;
            case '1': 
                Ch=1;
                Space = size/2;
                break;
            case '2': 
                Ch=2;
                Space = size/2;
                break;
            case '3': 
                Ch=3;
                Space = size/2;
                break;
            case '4': 
                Ch=4;
                Space = size/2;
                break;
            case '5': 
                Ch=5;
                Space = size/2;
                break;
            case '6': 
                Ch=6;
                Space = size/2;
                break;
            case '7': 
                Ch=7;
                Space = size/2;
                break;
            case '8': 
                Ch=8;
                Space = size/2;
                break;
            case '9': 
                Ch=9;
                Space = size/2;
                break;
            case ':': 
                Ch=10;
                Space = size/4;
                break;
        }
            
        LCD_MyShowChar3(x,y,Ch,size,0,Point_color,Back_color); 
        x+=Space;
        i++;
    }
}
//-----------------------------------------------------------------------
void Button(u16 xs, u16 ys, u16 xe, u16 ye, u16 button_color,u16 font_width, u8 font_size,u8 *p,u16 font_color)
{
    
    LCD_Fill(xs,ys,xe,ye,button_color);
    if(font_size == 12 || font_size == 16)
        LCD_MyShowString(((xs+xe)/2)-((font_size/2)*(font_width/2)), ((ys+ye)/2)-(font_size/2), (font_width*font_size), font_size, font_size, p, font_color, button_color);
    else if(font_size == 48)    
        LCD_MyShowString3(((xs+xe)/2)-((font_size/2)*(font_width/2)), ((ys+ye)/2)-(font_size/2), font_size, p, font_color, button_color);
    
    
}
//-----------------------------------------------------------------------
void Button2(u16 xs, u16 ys, u16 xe, u16 ye, u16 button_color,u16 font_width1, u8 font_size1,u8 *p1, u16 font_color1, u16 font_width2, u8 font_size2,u8 *p2,u16 font_color2)
{
    
    LCD_Fill(xs,ys,xe,ye,button_color);
    if(font_size1 == 12 || font_size1 == 16)
        LCD_MyShowString(((xs+xe)/2)-((font_size1/2)*(font_width1/2)), ((ys+ye)/2)-(font_size1*1.1), (font_width1*font_size1), font_size1, font_size1, p1, font_color1, button_color);
    if(font_size2 == 12 || font_size2 == 16)
		LCD_MyShowString(((xs+xe)/2)-((font_size2/2)*(font_width2/2)), ((ys+ye)/2)+(font_size2*0.1), (font_width2*font_size2), font_size2, font_size2, p2, font_color2, button_color);
    
}
//-----------------------------------------------------------------------
	// Show A Picture On The LCD Screen
//-----------------------------------------------------------------------
void LCD_Show_Image(int x,int y,const u16 *pointer)
{
	unsigned long int counter,compare_value=(unsigned long int)pointer[0]*pointer[1];
  
	LCD_WriteReg(TS_INS_GRAM_ADX, x);
	LCD_WriteReg(TS_INS_START_ADX, x);
	LCD_WriteReg(TS_INS_END_ADX, x+pointer[0]-1);  
	
	#ifdef PORTRAIT
		LCD_WriteReg(TS_INS_GRAM_ADY, y);
		LCD_WriteReg(TS_INS_START_ADY, y);
		LCD_WriteReg(TS_INS_END_ADY, y+pointer[1]-1);
	#else
		LCD_WriteReg(TS_INS_GRAM_ADY, (HEIGHT-1)-y);
		LCD_WriteReg(TS_INS_START_ADY, (HEIGHT-1)-(y+pointer[1]));
		LCD_WriteReg(TS_INS_END_ADY, (HEIGHT-1)-y);
	#endif 
	LCD_WR_REG(TS_INS_RW_GRAM);  
	//pointer+=5;
	for(counter=0;counter <compare_value+1;counter++)
		LCD_WR_DATA(*++pointer);
	
	LCD_WriteReg(TS_INS_GRAM_ADX, 0);
	LCD_WriteReg(TS_INS_START_ADX, 0);
	LCD_WriteReg(TS_INS_END_ADX, (WIDTH-1));
	LCD_WriteReg(TS_INS_GRAM_ADY, 0);
	LCD_WriteReg(TS_INS_START_ADY, 0);
	LCD_WriteReg(TS_INS_END_ADY, (HEIGHT-1));
}


//-----------------------------------------------------------------------
	// Show A Picture On The LCD Screen
//-----------------------------------------------------------------------
void LCD_MyShow_Image(int x,int y,u8 *pointer,u16 Point_color,u16 Back_color)
{
	unsigned long int counter,compare_value=(unsigned long int)pointer[0]*pointer[1];
  
	LCD_WriteReg(TS_INS_GRAM_ADX, x);
	LCD_WriteReg(TS_INS_START_ADX, x);
	LCD_WriteReg(TS_INS_END_ADX, x+pointer[0]-1);  
	
	#ifdef PORTRAIT
		LCD_WriteReg(TS_INS_GRAM_ADY, y);
		LCD_WriteReg(TS_INS_START_ADY, y);
		LCD_WriteReg(TS_INS_END_ADY, y+pointer[1]-1);
	#else
		LCD_WriteReg(TS_INS_GRAM_ADY, (HEIGHT-1)-y);
		LCD_WriteReg(TS_INS_START_ADY, (HEIGHT-1)-(y+pointer[1]));
		LCD_WriteReg(TS_INS_END_ADY, (HEIGHT-1)-y);
	#endif 
	LCD_WR_REG(TS_INS_RW_GRAM);  
	//pointer+=5;
	for(counter=0;counter <compare_value;counter++)
		if(pointer[counter+2] == 1)
            LCD_WR_DATA(Point_color);
        else if(pointer[counter+2] == 0)
            LCD_WR_DATA(Back_color);
	
	LCD_WriteReg(TS_INS_GRAM_ADX, 0);
	LCD_WriteReg(TS_INS_START_ADX, 0);
	LCD_WriteReg(TS_INS_END_ADX, (WIDTH-1));
	LCD_WriteReg(TS_INS_GRAM_ADY, 0);
	LCD_WriteReg(TS_INS_START_ADY, 0);
	LCD_WriteReg(TS_INS_END_ADY, (HEIGHT-1));
}

//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Set Farsi font color
//----------------------------------------------------------------------------------------------------
void myLCD_set_font_color(int color)
{
	font_color = color;
}
//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Set Farsi font Size
//----------------------------------------------------------------------------------------------------
void myLCD_set_font_size(int size)
{
	font_size = size;
}
//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Set number format to farsi
//----------------------------------------------------------------------------------------------------
void myLCD_fa_num(void)
{
	fa_num = 1;
}
//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Set number format to english
//----------------------------------------------------------------------------------------------------
void myLCD_en_num(void)
{
	fa_num = 0;
}

//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Set efect of the farsi font
// ef : effect number			ef:1,2,...,5
//----------------------------------------------------------------------------------------------------
void myLCD_font_effect(int ef)
{
	if(ef >= 0 && ef <= 5)
		effect = ef;
}

//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Enable Italic effect for farsi font 
//----------------------------------------------------------------------------------------------------
void myLCD_italic_on(void)
{
	italic = 1;
}

//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Disable Italic effect for farsi font 
//----------------------------------------------------------------------------------------------------
void myLCD_italic_off(void)
{
	italic = 0;
}

//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Puts a Farsi Character On The LCD Screen      simlpe
//----------------------------------------------------------------------------------------------------
void myLCD_put_char(char character)
{
	//x_text
	int counter_x,counter_y;
	char letter_length,flag_p;

	letter_length = font12x16[character][0]>>12;
	 for(counter_y=0;counter_y<16;counter_y++)
	    for(counter_x=0;counter_x<letter_length;counter_x++)
			{
				flag_p = font12x16[character][counter_y]>>(counter_x);
				flag_p = flag_p&0x01;
				if(flag_p)
				myLCD_DrawPoint(WIDTH - 3 -(x_font+(counter_x)),y_font+counter_y,font_color);
				else if(highlight)
				myLCD_DrawPoint(WIDTH - 3 -(x_font+(counter_x)),y_font+counter_y,highlight_color);
			}

	x_font += letter_length;
	if(x_font > WIDTH - 1)
	{
		x_font = 0;
		y_font += 16;
		if(y_font > HEIGHT-0)
			y_font = 0;
	}
	
}
	
//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Get Length of a Farsi - Enlish String
//----------------------------------------------------------------------------------------------------
int Get_String_Length(const char *string)
{
	int length=0;
	unsigned char letter,nt_letter,pr_letter;
	unsigned char CN=0,CP=0,pr_CN=0,nt_CP=0,place=0,flag1=0;
	
	while(*string)
	{
		if (flag1 == 0) 
				pr_letter = 0x20;
		else 
			if(*(string - 1) > 0x80)
				pr_letter = *(string - 1);
			else
				pr_letter = 0x20;

				letter = *string++;
				
		if(*string > 0x80)
			if (*string == 0) 
				nt_letter = 0x20;
			else 
				nt_letter = *string;
		else
			nt_letter = 0x20;
				
		flag1 = 1;

		if(letter > 0x98) letter = letter;
		else if(letter == 0x98) letter = 0xBC;
		else if(letter == 0x90) letter = 0xC0;
		else if(letter == 0x8D) letter = 0xBE;
		else if(letter == 0x81) letter = 0xBD;
		else if(letter == 0x8E) letter = 0xBF;

		if(pr_letter > 0x98) pr_letter = pr_letter;
		else if(pr_letter == 0x98) pr_letter = 0xBC;
		else if(pr_letter == 0x90) pr_letter = 0xC0;
		else if(pr_letter == 0x8D) pr_letter = 0xBE;
		else if(pr_letter == 0x81) pr_letter = 0xBD;
		else if(pr_letter == 0x8E) pr_letter = 0xBF;

		if(nt_letter > 0x98) nt_letter = nt_letter;
		else if(nt_letter == 0x98) nt_letter=0xBC;
		else if(nt_letter == 0x90) nt_letter=0xC0;
		else if(nt_letter == 0x8D) nt_letter=0xBE;
		else if(nt_letter == 0x81) nt_letter=0xBD;
		else if(nt_letter == 0x8E) nt_letter=0xBF;
	  


		if(pr_letter > 0x80)
			pr_CN = ((FAmap[(pr_letter - 0xBC)][5]) == 1);
		else
			pr_CN = 0; 
				
		if(nt_letter > 0x80)
			nt_CP = ((FAmap[(nt_letter - 0xBC)][4]) == 1);
		else
			nt_CP = 0;
				
		if(letter > 0x80)
			CP = ((FAmap[(letter - 0xBC)][4]) == 1);
		else 
			CP = 0; 
				
		if(letter > 0x80)
			CN = ((FAmap[(letter-0xBC)][5]) == 1);
		else
			CN = 0;

		CP = pr_CN && CP;
		CN = CN && nt_CP;
		place = (CP << 1) | CN;	
		

		length += font12x16 [FAmap[(letter - 0xBC)][place]][1]>>12;
		
		
	}
		

	return length;
}


//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Puts a Farsi Character On The LCD Screen with scaling
// size : Scale factor       size:1,2,3,...
//----------------------------------------------------------------------------------------------------
void myLCD_put_bold_char(char character , int size)
{
	int counter_x,counter_y,i,j;
	char letter_length,flag_p;
	

	letter_length = font12x16[character][0]>>12;
	 for(counter_y=0;counter_y<16;counter_y++)
	    for(counter_x=0;counter_x<letter_length;counter_x++)
			{
				flag_p = font12x16[character][counter_y]>>(counter_x);
				flag_p = flag_p&0x01;
				
				
				if(flag_p)
				{
					
					if(effect == 0)
						for(i=0;i<size;i++)
							for(j=0;j<size;j++)
								myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + i -(x_font+(size*counter_x)),y_font+size*counter_y + j,font_color);
					// Very good effect (1)
					else if(effect == 1)
					{
						// good for size = 2
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y - 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y + 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) - 1 -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
					}
					else if(effect == 2)
					{
						// good for size = 2
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y - 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y + 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) - 1 -(x_font+(size*counter_x)),y_font+size*counter_y + 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y + 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) - 1 -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y + 2,font_color);
					}
					else if(effect == 3)
					{
						// good for size = 3
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y - 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y - 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) - 1 -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 2 -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y + 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y + 1,font_color);
					}
					else if(effect == 4)
					{
						// good for size = 3
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y - 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y - 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y + 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) - 1 -(x_font+(size*counter_x)),y_font+size*counter_y + 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y + 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 2 -(x_font+(size*counter_x)),y_font+size*counter_y + 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) - 1 -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 2 -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y + 2,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y + 2,font_color);
					}
					else if(effect == 5)
					{
						// good for size = 1,3
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y + 1,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y + 1,font_color);
					}
					
					//myLCD_draw_circle(WIDTH - 3 -(x_font+(size*counter_x)),y_font+size*counter_y,size,1,font_color);
					
				}
				else if(highlight)
				{
					
						
					if(effect == 0)
						for(i=0;i<size;i++)
							for(j=0;j<size;j++)
								myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + i -(x_font+(size*counter_x)),y_font+size*counter_y + j,highlight_color);
					// Very good effect (1)
					else if(effect == 1)
					{
						// good for size = 2
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y,font_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y - 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y + 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) - 1 -(x_font+(size*counter_x)),y_font+size*counter_y,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y,highlight_color);
					}
					else if(effect == 2)
					{
						// good for size = 2
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y - 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y + 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) - 1 -(x_font+(size*counter_x)),y_font+size*counter_y + 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y + 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) - 1 -(x_font+(size*counter_x)),y_font+size*counter_y,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y + 2,highlight_color);
					}
					else if(effect == 3)
					{
						// good for size = 3
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y - 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y - 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) - 1 -(x_font+(size*counter_x)),y_font+size*counter_y,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 2 -(x_font+(size*counter_x)),y_font+size*counter_y,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y + 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y + 1,highlight_color);
					}
					else if(effect == 4)
					{
						// good for size = 3
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y - 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y - 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y + 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) - 1 -(x_font+(size*counter_x)),y_font+size*counter_y + 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y + 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 2 -(x_font+(size*counter_x)),y_font+size*counter_y + 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) - 1 -(x_font+(size*counter_x)),y_font+size*counter_y,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 2 -(x_font+(size*counter_x)),y_font+size*counter_y,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y + 2,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y + 2,highlight_color);
					}
					else if(effect == 5)
					{
						// good for size = 1,3
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y + 1,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y,highlight_color);
						myLCD_DrawPoint(WIDTH - 3 + italic*(-counter_y+7) + 1 -(x_font+(size*counter_x)),y_font+size*counter_y + 1,highlight_color);
					}
						
						
						
					//myLCD_draw_circle(WIDTH - 3 + italic*(-counter_y+7) -(x_font+(size*counter_x)),y_font+size*counter_y,size,1,highlight_color);
					
					
				}
			}

	x_font += size*letter_length;
	if(x_font > WIDTH - 1)
	{
		x_font = 0;
		y_font += size*16;
		if(y_font > HEIGHT-0)
			y_font = 0;
	}
	
}

//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Go to a specific pont for farsi font (x:0..WIDTH , y:0..HEIGHT)
//----------------------------------------------------------------------------------------------------
void myLCD_goto_xy(int x,int y)
{
	if((x >= WIDTH) || (x < 0))
		x_font = 0;
	else
		x_font = x;
	
	if((y >= HEIGHT) || (y < 0))
		y_font = 0;
	else
		y_font = y;
}
//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Enable Farsi font Highlight
//----------------------------------------------------------------------------------------------------
void myLCD_text_highlight_on(void)
{
	highlight = 1;
}
//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Disable Farsi font Highlight
//----------------------------------------------------------------------------------------------------
void myLCD_text_highlight_off(void)
{
	highlight = 0;
}
//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Set Farsi font Highlight color
//----------------------------------------------------------------------------------------------------
void myLCD_text_highlight_color(unsigned int color)
{
	highlight_color = color;
}
//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Show a farsi-englisg string on the LCD with specific size
// size : Scale factor       size:1,2,3,...
//----------------------------------------------------------------------------------------------------
void myLCD_putsf_mix(const char *string , int size)
{
	unsigned char letter,nt_letter,pr_letter;
	unsigned char CN=0,CP=0,pr_CN=0,nt_CP=0,place=0,flag1=0;
	unsigned char letter_counter=0,letter_count=0,length=0,enter=0;
	unsigned char text_buffer[32];
	char en_end=0;


	flag1 = 0;

	while(*string)
	{
		if(*string > 0x80)
		{
			while((*string > 0x80) && (*string != 0))
			{
				if (flag1 == 0) 
					pr_letter = 0x20;
				else 
					if(*(string - 1) > 0x80)
						pr_letter = *(string - 1);
					else
						pr_letter = 0x20;

				letter = *string++;
				
				if(*string > 0x80)
					if (*string == 0) 
						nt_letter = 0x20;
					else 
						nt_letter = *string;
				else
					nt_letter = 0x20;
				
				flag1 = 1;

				if(letter > 0x98) letter = letter;
				else if(letter == 0x98) letter = 0xBC;
				else if(letter == 0x90) letter = 0xC0;
				else if(letter == 0x8D) letter = 0xBE;
				else if(letter == 0x81) letter = 0xBD;
				else if(letter == 0x8E) letter = 0xBF;

				if(pr_letter > 0x98) pr_letter = pr_letter;
				else if(pr_letter == 0x98) pr_letter = 0xBC;
				else if(pr_letter == 0x90) pr_letter = 0xC0;
				else if(pr_letter == 0x8D) pr_letter = 0xBE;
				else if(pr_letter == 0x81) pr_letter = 0xBD;
				else if(pr_letter == 0x8E) pr_letter = 0xBF;

				if(nt_letter > 0x98) nt_letter = nt_letter;
				else if(nt_letter == 0x98) nt_letter=0xBC;
				else if(nt_letter == 0x90) nt_letter=0xC0;
				else if(nt_letter == 0x8D) nt_letter=0xBE;
				else if(nt_letter == 0x81) nt_letter=0xBD;
				else if(nt_letter == 0x8E) nt_letter=0xBF;
	  


				if(pr_letter > 0x80)
					pr_CN = ((FAmap[(pr_letter - 0xBC)][5]) == 1);
				else
					pr_CN = 0; 
				
				if(nt_letter > 0x80)
					nt_CP = ((FAmap[(nt_letter - 0xBC)][4]) == 1);
				else
					nt_CP = 0;
				
				if(letter > 0x80)
					CP = ((FAmap[(letter - 0xBC)][4]) == 1);
				else 
					CP = 0; 
				
				if(letter > 0x80)
					CN = ((FAmap[(letter-0xBC)][5]) == 1);
				else
					CN = 0;

				CP = pr_CN && CP;
				CN = CN && nt_CP;
				place = (CP << 1) | CN;	
		
				text_buffer[letter_counter++] = FAmap[(letter - 0xBC)][place];

				length += size*(font12x16 [FAmap[(letter - 0xBC)][place]][1]>>12);
			}
	

	
			if(length < WIDTH - x_font - 3)
				for(letter_count=0;letter_count<letter_counter;letter_count++)
					myLCD_put_bold_char(text_buffer[letter_count],size);
			else
			{
			
				x_font = 0;
				y_font += size*16;
				if(y_font > HEIGHT - size*16)
					y_font = 0;
				
				for(letter_count=0;letter_count<letter_counter;letter_count++)
					myLCD_put_bold_char(text_buffer[letter_count],size);
			}
			letter_counter = 0;
			length = 0;
		}	 		
		else if(*string == 0x20)   //Space detect
			myLCD_put_bold_char(*string++,size);
		else   //English letter & Number & Enter detect
		{	
			if((*string == 0x0D) || (*string == 0x0A))   //Enter detect
			{
				x_font = 0;
				y_font += size*16;
				if(y_font > HEIGHT - size*16)
					y_font = 0;
				
				string += 2;
				goto p1;
			}	
			
			
			
			while((*string < 0x81) && (*string != 0))
			{
				
				
				
				if(fa_num)
				{
					if((*string > 0x2F)&&(*string < 0x3A))	
					{
						letter = (*string) - 0x20;
						text_buffer[letter_counter++] = letter;
						string++;
						goto P2;
					} 
				}		
				text_buffer[letter_counter++] = *string++;
				P2:
				
				
				
				if(((*string == 0x20) && ((letter_counter * size*8) < (WIDTH - x_font - 3))) || (*string > 0x80))
					flag1 = letter_counter;
	
				if((letter_counter * size*8) > (WIDTH - x_font - 3))
				{
					string -= (letter_counter - flag1);
					letter_counter = flag1;
					enter = 1;

					break;
				}
				
				if((*string == 0x20) && (*(string+1) > 0x80))   //farsi after Space detect
					break;
			
			}
			
			if(((letter_counter * size*8) < (WIDTH - x_font - 1))  || (*string > 0x80))
			{
				for(letter_count=(letter_counter);letter_count>0;letter_count--)
					myLCD_put_bold_char(text_buffer[letter_count - 1],size);
			
				if(enter)
				{
					enter = 0;
					x_font = 0;
					y_font += size*16;
					if(y_font > HEIGHT - size*16)
						y_font = 0;
				}
				
				
				
			}
			p1:
			letter_counter = 0;
			flag1 = 0; 

		}
		
	}
	flag1 = 0;

}


//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Make an ascii string from an unicode string 
//----------------------------------------------------------------------------------------------------
void unicode2ascii(char *uni_str , char *ascii_str)
{
	int counter = 0;
	int Uch = 0;
	char chl,chh;
	
	
	while(*uni_str) 
	{
		chl = *uni_str++; 
		chh = *uni_str++;
		
		Uch = 0;
		Uch = ((Uch | chh) << 8) | chl;
		
		if(Uch > 1574 && Uch < 1591)
			*(ascii_str + counter) = (char)(Uch - 1376);
		else if(Uch > 1590 && Uch < 1595)
			*(ascii_str + counter) = (char)(Uch - 1375);
		else if(Uch > 1600 && Uch < 1603)
			*(ascii_str + counter) = (char)(Uch - 1380);
		else if(Uch == 1705)
			*(ascii_str + counter) = (char)(Uch - 1482);
		else if(Uch == 1604)
			*(ascii_str + counter) = (char)(Uch - 1379);
		else if(Uch > 1604 && Uch < 1609)
			*(ascii_str + counter) = (char)(Uch - 1378);
		else if(Uch == 1740)
			*(ascii_str + counter) = (char)(Uch - 1503);
		else if(Uch == 1574)
			*(ascii_str + counter) = (char)(Uch - 1381);
		else if(Uch == 1662)
			*(ascii_str + counter) = (char)(Uch - 1533);
		else if(Uch == 1670)
			*(ascii_str + counter) = (char)(Uch - 1529);
		else if(Uch == 1688)
			*(ascii_str + counter) = (char)(Uch - 1546);
		else if(Uch == 1711)
			*(ascii_str + counter) = (char)(Uch - 1567);
		else if(Uch == 1570)
			*(ascii_str + counter) = (char)(Uch - 1376);
		else if(Uch > 1631 && Uch < 1642)
			*(ascii_str + counter) = (char)(Uch - 1584);
		else if(Uch == 65536)
			*(ascii_str + counter) = NULL;
		else
			*(ascii_str + counter) = (char) Uch;
		
		
		counter++;
		
	}
	*(ascii_str + counter) = NULL;
	*(ascii_str + counter - 1) = NULL;
	
}

//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Description	: Draws a beveled figure on the screen. 
// Input       	: x0, y0 - coordinate position of the upper left center
//		      		: x1, y1 - coordinate position of the lower right center
//             	: rad    - defines the redius of the circle,
//             	: fill   - fill yes or no
//----------------------------------------------------------------------------------------------------
void myLCD_draw_round_rectangle(int x0,int y0,int x1,int y1,int rad,char fill,int color)
{
	signed int a,b,P;

	a = 0;       				// increment by 1
	b = rad;  				// decrement by 1 using P
	P = 1 - rad;


	if (fill)
	{		
		myLCD_DrawRectangle(x0,y0+rad,x1,y1-rad,1,color);
		
		do
		{
			myLCD_DrawRectangle(x0-a+rad,y0-b+rad,a+x1-rad,y0-b+rad,1,color);	// 8 --> 1
			myLCD_DrawRectangle(x0-b+rad,y0-a+rad,b+x1-rad,y0-a+rad,1,color);	// 7 --> 2
			myLCD_DrawRectangle(x0-b+rad,a+y1-rad,b+x1-rad,a+y1-rad,1,color);	// 6 --> 3
			myLCD_DrawRectangle(x0-a+rad,b+y1-rad,a+x1-rad,b+y1-rad,1,color);	// 5 --> 4

			if(P < 0)
				P+= 3 + 2*a++;
			else	
				P+= 5 + 2*(a++ - b--);
			
		} while(a <= b);
	} //fill
	else
	{
		myLCD_DrawRectangle(x0+rad,y0,x1-rad,y0,1,color);	// top
		myLCD_DrawRectangle(x0+rad,y1,x1-rad,y1,1,color);	// bottom
		myLCD_DrawRectangle(x0,y0+rad,x0,y1-rad,1,color);	// left
		myLCD_DrawRectangle(x1,y0+rad,x1,y1-rad,1,color);	// right

		do
		{
			myLCD_DrawPoint(a+x1-rad,y0-b+rad,color);	// `````` Segment 1
			myLCD_DrawPoint(b+x1-rad,y0-a+rad,color);	// `````` Segment 2
			
			myLCD_DrawPoint(b+x1-rad,a+y1-rad,color);	// `````` Segment 3
			myLCD_DrawPoint(a+x1-rad,b+y1-rad,color);	// `````` Segment 4
			
			myLCD_DrawPoint(x0-a+rad,b+y1-rad,color);	// `````` Segment 5
			myLCD_DrawPoint(x0-b+rad,a+y1-rad,color);	// `````` Segment 6
			
			myLCD_DrawPoint(x0-b+rad,y0-a+rad,color);	// `````` Segment 7
			myLCD_DrawPoint(x0-a+rad,y0-b+rad,color);	// `````` Segment 8
			
			if(P < 0)
				P += 3 + 2*a++;
			else				
				P += 5 + 2*(a++ - b--);
		} while(a <= b);
	} //no fill
}	//RoundRectangle

//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Puts A Character On The LCD Screen
//----------------------------------------------------------------------------------------------------
void myLCD_putchar(char character,int forground_color,int background_color,int transparent_mode)
{  
    int width_counter,length_counter;
    char character_segment_data;
	
    for(length_counter=0;length_counter<16;length_counter++)
    {
      character_segment_data = (char)(font12x16[character][length_counter] & 0xFF);
      for(width_counter=0;width_counter<8;width_counter++)
      {
        if(character_segment_data & (0x80 >> width_counter)) 
          myLCD_DrawPoint((x_text*8) + width_counter,(y_text*16) + length_counter,forground_color);
		    else
			  {
			    if(!transparent_mode)
						myLCD_DrawPoint((x_text*8) + width_counter,(y_text*16) + length_counter,background_color);
					else
						myLCD_DrawPoint((x_text*8) + width_counter,(y_text*16) + length_counter,LCD_ReadPoint((x_text*8) + width_counter,(y_text*16) + length_counter));
			  };
			};
    };
    
    if(++x_text >= WIDTH/8)
    {            
      x_text = 0;
      if(++y_text >= HEIGHT/16) 
				y_text = 0;
    };
}  
//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Show A String That Stored In The Flash Memory Of The Microcontroller
//----------------------------------------------------------------------------------------------------    
void myLCD_putsf(const char *string,int forground_color,int background_color,int transparent_mode)
{
   while(*string)
     myLCD_putchar(*string++,forground_color,background_color,transparent_mode);
}

//----------------------------------------------------------------------------------------------------
//************************************* ECA 2.8 inch LCD Module **************************************
//----------------------------------------------------------------------------------------------------
// Show A String That Stored In The SRAM Of The Microcontroller
//----------------------------------------------------------------------------------------------------     
void myLCD_puts(char *string,int forground_color,int background_color,int transparent_mode)
{
   while(*string)
     myLCD_putchar(*string++,forground_color,background_color,transparent_mode);
}
	
















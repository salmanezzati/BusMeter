
#include "dialler.h"

EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
    
extern unsigned char Key[13];
extern unsigned char Pass[5];
extern unsigned char Dtmf, Sec, Sec_2, Dialling;
extern int Ring_count, D_sec, Usart_sec;
extern unsigned  char Code[4];
extern unsigned char Show_phone, Phone_flag, Draw_arm_button; 
extern unsigned char Arm;
extern unsigned char Lcd_buffer[250], Buffer_tx[250], Buffer_rx[250];

//************************** SIN TABLE *************************************
// Samples table : one period sampled on 128 samples and
// quantized on 7 bit
//**************************************************************************
unsigned char auc_SinParam [128] = {
64,67,
70,73,
76,79,
82,85,
88,91,
94,96,
99,102,
104,106,
109,111,
113,115,
117,118,
120,121,
123,124,
125,126,
126,127,
127,127,
127,127,
127,127,
126,126,
125,124,
123,121,
120,118,
117,115,
113,111,
109,106,
104,102,
99,96,
94,91,
88,85,
82,79,
76,73,
70,67,
64,60,
57,54,
51,48,
45,42,
39,36,
33,31,
28,25,
23,21,
18,16,
14,12,
10,9,
7,6,
4,3,
2,1,
1,0,
0,0,
0,0,
0,0,
1,1,
2,3,
4,6,
7,9,
10,12,
14,16,
18,21,
23,25,
28,31,
33,36,
39,42,
45,48,
51,54,
57,60};

//***************************  x_SW  ***************************************
//Table of x_SW (excess 8): x_SW = ROUND(8*N_samples*f*510/Fck)
//**************************************************************************

//high frequency (coloun)
//1209hz  ---> x_SW = 79
//1336hz  ---> x_SW = 87
//1477hz  ---> x_SW = 96
//1633hz  ---> x_SW = 107

const unsigned char auc_frequencyH [4] = {
107,96,
87,79};

//low frequency (row)
//697hz  ---> x_SW = 46
//770hz  ---> x_SW = 50
//852hz  ---> x_SW = 56
//941hz  ---> x_SW = 61

const unsigned char auc_frequencyL [4] = {
61,56,
50,46};


//**************************  global variables  *****************************************
unsigned char x_SWa = 0x00;               // step width of high frequency
unsigned char x_SWb = 0x00;               // step width of low frequency
unsigned int  i_CurSinValA = 0;           // position freq. A in LUT (extended format)
unsigned int  i_CurSinValB = 0;           // position freq. B in LUT (extended format)
unsigned int  i_TmpSinValA;               // position freq. A in LUT (actual position)
unsigned int  i_TmpSinValB;               // position freq. B in LUT (actual position)
//***************************************************************************************

void EXTI0_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_EVENTOUT); 
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
    
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;				
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
	NVIC_Init(&NVIC_InitStructure); 
}

void EXTI1_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_EVENTOUT); 
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI0_IRQHandler(void)
{
	char i;
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        Sec_2=0;
        //Led_ring_on
        //TIM_Cmd(TIM5, ENABLE); //Start_Timer5
        //delay_ms(1);
        //Led_ring_off
		Dialling=1;
        if(Sec >= 2)
        {
            Sec=0;
            Ring_count++;
            //Led_ring_on
            //delay_ms(1);
            //Led_ring_off
            
//             if(Phone_flag)
//                 Phone_flag = 0;
//             else 
//                 Phone_flag = 1;
//             Show_phone=1;
            
            sprintf(Lcd_buffer,"%d              ",Ring_count);
            LCD_MyShowString(10, 146, 96, 16, 16, Lcd_buffer, WHITE, MAGENTA);
        }
        if(Ring_count>=4)
        {
            Line_connect
            //Led_ring_on
            for(i=0;i<3;i++)
            {
                delay_ms(300);
                TIM_Cmd(TIM9, ENABLE); //Beeb_on
                delay_ms(300);
                TIM_Cmd(TIM9, DISABLE); //Beeb_off
            }
            Ring_count=0;
            Sec=0;
        }
        
        EXTI_ClearITPendingBit(EXTI_Line0); 
    }    
}

void EXTI1_IRQHandler(void)
{
 	//static int i=0;
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        //Led_ring_on
        //delay_ms(1);
        //Led_ring_off
        
        Dtmf = GPIO_ReadInputData(GPIOB) & 0x000F;
        
//         sprintf(Lcd_buffer,"%d",Dtmf);
//         LCD_MyShowString(i++, 146, 16, 16, 16, Lcd_buffer, WHITE, MAGENTA);
        
        EXTI_ClearITPendingBit(EXTI_Line1); 
    }
}
//*************************************************
void TIM5_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 
 
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	//TIM_Cmd(TIM5, ENABLE);  
}

void TIM5_IRQHandler(void)   
{
// 	static char x;
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  
// 		GPIO_ToggleBits(GPIOC, GPIO_Pin_0);
		
//         if(x==1)
//         {
//             TIM_Cmd(TIM3, ENABLE);  
//             x=0;
//         }
//         else
//         {
//             TIM_Cmd(TIM3, DISABLE); 
//             x=1;
//         }
        
        //if(Usart_sec<2000)
        //    Usart_sec++;
        
		if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_4))
            Sec_2=0;
        if(Sec_2 > 4)
        {
            //Ring_count=0;
            //Sec_2=0;
            Error();
        }
		Sec++;
        Sec_2++;
		if(Sec>=20)
		{
            Error();
        }
        
        if(!GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_4))
        {    
            if(Phone_flag)
                Phone_flag = 0;
            else 
                Phone_flag = 1;
        }
        else
            Phone_flag = 1;
        
        Show_phone=1;
            
	}
}

//**************************************************
void Stop_DTMF(void)
{
    x_SWb=0;
    x_SWb=0;
    TIM_Cmd(TIM3, DISABLE);
}
//***************************************************
void Start_DTMF(void)
{
    TIM_Cmd(TIM3, ENABLE);
}

//***************************************************
void DTMF_Init (void)
{
    TIM3_Pwm_Init();                       // Interrupts enabled
}
//***************************************************
//---PWM for dialling---
void TIM3_Pwm_Init(void)
{
    uint16_t TimerPeriod = 0;
    uint16_t Channel2Pulse = 0;
    GPIO_InitTypeDef  GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
    
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
    
    Channel2Pulse = (uint16_t) (((uint32_t) 0 * (2676- 1)) / 254);
    
	TIM_TimeBaseStructure.TIM_Period = (u32) 2676; 
	TIM_TimeBaseStructure.TIM_Prescaler = (u32)(SystemCoreClock / (4*16800000)) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  
    
    NVIC_EnableIRQ(TIM3_IRQn);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); 
    
    //TIM_Cmd(TIM3, ENABLE);  
}
void TIM3_IRQHandler(void)   
{
    static char PW;
    uint16_t Channel2Pulse = 0;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  
	{
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
        
        // move Pointer about step width aheaed
        i_CurSinValA += x_SWa;
        i_CurSinValB += x_SWb;
        // normalize Temp-Pointer
        i_TmpSinValA  =  (char)(((i_CurSinValA+4) >> 3)&(0x007F));
        i_TmpSinValB  =  (char)(((i_CurSinValB+4) >> 3)&(0x007F));
        // calculate PWM value: high frequency value + 3/4 low frequency value
        PW = (auc_SinParam[i_TmpSinValA] + (auc_SinParam[i_TmpSinValB]-(auc_SinParam[i_TmpSinValB]>>2)));
        
        Channel2Pulse = (uint16_t) (((uint32_t) PW * (2676- 1)) / 254);
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
        TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
        TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
        TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
        TIM_OC2Init(TIM3, &TIM_OCInitStructure);
        
    }
}
//******************************************************
//---PWM for beeb---
void TIM9_Pwm_Init(void)
{
    uint16_t TimerPeriod = 0;
    uint16_t Channel1Pulse = 0;
    GPIO_InitTypeDef  GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE); 
    
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    //Buzzer PWM
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9);
    
    Channel1Pulse = (uint16_t) (((uint32_t) 500 * (3000- 1)) / 1000);
    
	TIM_TimeBaseStructure.TIM_Period = (u32) 3000; 
	TIM_TimeBaseStructure.TIM_Prescaler = (u32)(SystemCoreClock / 16800000) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure); 
 
// 	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );

// 	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
// 	NVIC_Init(&NVIC_InitStructure);  
//     
//  NVIC_EnableIRQ(TIM3_IRQn);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM9, &TIM_OCInitStructure);
    
    TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM9, ENABLE); 
    
    //TIM_Cmd(TIM9, ENABLE);  
}

void Wait_Ring(void)
{
	unsigned char i=0,j=10;
    
    TIM_Cmd(TIM5, DISABLE);
    TIM_Cmd(TIM9, DISABLE);
	Sec=0;
	Ring_count=0;
    Line_disconnect
    //Led_ring_off
	//Led_error_off
    
    //LCD_Clear(GREEN);
    LCD_MyShowString(10, 130, 96, 16, 16, "Wait Ring        ", WHITE, MAGENTA);
	while(1)
	{
		while(!GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_4))
		{
			while(Dtmf>0 && Dtmf<11)
			{
				sprintf(Lcd_buffer,"%d",Key[Dtmf]);
				LCD_MyShowString(j, 146, 16, 16, 16, Lcd_buffer, WHITE, MAGENTA);
                j+=8;
                i++;
				Sec=0;
				Dtmf=0;
			}
		}
		i=0;
		Get_Pass();
	}
}

void Get_Pass(void)
{
    unsigned char i=0,j=10;
    LCD_MyShowString(10, 146, 200, 16, 16, "Enter Password:    ", WHITE, MAGENTA);
    Sec=0;
    while(i<4)
    {
        while(Dtmf>0 && Dtmf<11)
        {
            Code[i]=Key[Dtmf];
            sprintf(Lcd_buffer,"%d",Key[Dtmf]);
            LCD_MyShowString(j, 162, 16, 16, 16, Lcd_buffer, WHITE, MAGENTA);
            j+=8;
            i++;
            Sec=0;
            Dtmf=0;
        }
        
    }
    for(i=0;i<4;i++)
    {
        if(Code[i] != Pass[i]-'0')
        {
			Error();
			break;
		}
    }
	
    if(i==4)
		Well();
}

void Well(void)
{
    //LCD_MyShowString(10, 162, 200, 16, 16, "Password Is Correct.", WHITE, MAGENTA);
    Sec=0;
	//delay_ms(3000);
    //LCD_MyShowString(10, 162, 200, 16, 16, "                    ", WHITE, MAGENTA);
    //Error();
    
    while(Sec<20)
    {
        while(Dtmf>0 && Dtmf<13)
        {
            switch(Key[Dtmf])
            {
                case 0:
                    
                    break;
                
                case 1:
                    Arm=1;
                    Draw_arm_button=1;
                    break;
                
                case 2:
                    Arm=0;
                    Draw_arm_button=1;
                    break;
                
                case 3:
                    
                    break;
                
                case 4:
                    
                    break;
                
                case 5:
                    
                    break;
                
                case 6:
                    
                    break;
                
                case 7:
                    
                    break;
                
                case 8:
                    
                    break;
                
                case 9:
                    
                    break;
                
                case 11:
                    Error();
                    break;
                
                case 12:
                    
                    break;
                
            }
            Sec=0;
            Dtmf=0;
        }
    }
    Error();
}

void Error (void)
{
	unsigned char i;
    
    for(i=0;i<3;i++)
	{
		TIM_Cmd(TIM9, ENABLE); //Beeb_on
		//Led_error_on
		//delay_ms(300);
		TIM_Cmd(TIM9, DISABLE); //Beeb_off
		//Led_error_off
		//delay_ms(300);
	}
    Dialling=0;
    Line_disconnect
    TIM_Cmd(TIM5, DISABLE);
    TIM_Cmd(TIM9, DISABLE);
    Phone_flag = 0;
    Show_phone = 1;
    
	Sec=0;
    Sec_2=0;
	Ring_count=0;
    //Led_ring_off
	//Led_error_off
    
    //LCD_Clear(GREEN);
    LCD_MyShowString(10, 146, 200, 16, 16, "Error             ", WHITE, MAGENTA);
    delay_ms(2000);
    LCD_MyShowString(10, 146, 200, 16, 16, "                  ", WHITE, MAGENTA);
    //LCD_Clear(GREEN);
    LCD_MyShowString(10, 130, 200, 16, 16, "Wait Ring        ", WHITE, MAGENTA);
}


#include "hs1527.h"

unsigned char RF_buffer[3][24], Counter, Gap=3, Time_count, X;
extern unsigned char Lcd_buffer[250], RF_buffer_ok_data[25], Show_hs1527;

//***********************************************
void EXTIX2_Init(void)
{
//	GPIO_InitTypeDef  GPIO_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_EVENTOUT); 
	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	 
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;				
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
	NVIC_Init(&NVIC_InitStructure); 
}

void EXTI2_IRQHandler(void)
{
    char Data, In, i;
    static char j=0,k=0;
    if(EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
        In=GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);
        if(In==0 && X==1)
        {   
            
            if(Time_count/Gap >= 3) 
            {
                Data = 1; 
            }
            else if(Time_count/Gap >= 1) 
            {
                Data = 0;
            }
            
            if(Data)
                RF_buffer[0][Counter] = 1+'0';
            else 
                RF_buffer[0][Counter] = '0';
                
            Counter++;
            if(Counter >= 24)
            {
                Counter=0;
                X=0;  
                //Show_flag=1;
                
                for(i=0;i<24;i++)
                {
                    if(RF_buffer[0][i] == '1')
                        j++;
                    else
                        k++;
                    
                    if(RF_buffer[0][i] != RF_buffer[1][i])
                        break;
                }
                if(i==24 && j!=24 && k!=24)
                {
                    for(i=0;i<24;i++)
                    {
                        RF_buffer_ok_data[i] = RF_buffer[0][i];
                    }
                    Show_hs1527=1;
                }
                else
                {
                    for(i=0;i<24;i++)
                    {
                        RF_buffer[1][i] = RF_buffer[0][i];
                    }
                } 
                i=0;
                j=0;
                k=0;
                
                
//                 for(i=0;i<24;i++)
//                 {
//                     if(RF_buffer[0][i] != RF_buffer[1][i])
//                         break;
//                 }
//                 if(i==24)
//                 {
//                     for(i=0;i<24;i++)
//                     {
//                         if(RF_buffer[0][i] != RF_buffer[2][i])
//                             break;
//                     }
//                     if(i==24)
//                     {
//                         for(i=0;i<24;i++)
//                         {
//                             RF_buffer_ok_data[i] = RF_buffer[0][i];
//                         }
//                         Show_hs1527=1; 
//                     }
//                     else
//                     {
//                         for(i=0;i<24;i++)
//                         {
//                             RF_buffer[2][i] = RF_buffer[1][i];
//                             RF_buffer[1][i] = RF_buffer[0][i];
//                         }
//                     }
//                 }
//                 else
//                 {
//                     for(i=0;i<24;i++)
//                     {
//                         RF_buffer[2][i] = RF_buffer[1][i];
//                         RF_buffer[1][i] = RF_buffer[0][i];
//                     }
//                 }  
            }
        }
        else 
        {
            if(Time_count/Gap >= 31)
                X=1;
        }
        
        Time_count=0;
        EXTI_ClearITPendingBit(EXTI_Line2); 
    }
}

//**********************************************
void TIM2_Int_Init(u32 arr,u32 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM2, ENABLE);  
}

void TIM2_IRQHandler(void)   
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  
        
        Time_count++;
	}
}


#include "dac.h"

void Dac1_Init(void)
{
  
	//GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE );	
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  	GPIO_Init(GPIOA, &GPIO_InitStructure);
// 	GPIO_SetBits(GPIOA,GPIO_Pin_4);
					
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	
    DAC_Init(DAC_Channel_1,&DAC_InitType);	

	DAC_Cmd(DAC_Channel_1, ENABLE); 
  
    DAC_SetChannel1Data(DAC_Align_12b_R, 0); 
}

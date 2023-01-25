
#ifndef __DIALLER_H
#define __DIALLER_H

#include "main.h"


void DTMF_Init (void);
void Send_DTMF (char * number);
void EXTI0_Init(void);
void EXTI1_Init(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void TIM5_Int_Init(u16 arr,u16 psc);
void TIM3_Pwm_Init(void);
void TIM9_Pwm_Init(void);
void Wait_Ring(void);
void Get_Pass(void);
void Well(void);
void Error (void);


#endif /* __DIALLER_H */

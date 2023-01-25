
#ifndef __HS1527_H
#define __HS1527_H

#include "main.h"

void EXTIX2_Init(void);
void EXTI2_IRQHandler(void);
void TIM2_Int_Init(u32 arr,u32 psc);
void TIM2_IRQHandler(void);

#endif /* __HS1527_H */

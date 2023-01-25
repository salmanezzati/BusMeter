
#ifndef __TIM_H
#define __TIM_H

#include "main.h"

extern char Ok_icon[1227];
extern char Error_icon[1262];
extern unsigned char Show_time, Show_setting, Set_num;
extern unsigned char Current_month, Current_day;

void TIM1_Int_Init(u32 arr,u32 psc);
void TIM10_Int_Init(u32 arr,u32 psc);
void TIM1_UP_TIM10_IRQHandler(void);

void TIM11_Int_Init(u32 arr,u32 psc);
void TIM1_TRG_COM_TIM11_IRQHandler(void);

void TIM7_Int_Init(u32 arr,u32 psc);
void TIM8_Int_Init(u32 arr,u32 psc);
void TIM12_Int_Init(u32 arr,u32 psc);
void TIM13_Int_Init(u32 arr,u32 psc);
void TIM14_Int_Init(u32 arr,u32 psc);
void TIM8_UP_TIM13_IRQHandler(void);
void TIM8_BRK_TIM12_IRQHandler(void);
void TIM8_TRG_COM_TIM14_IRQHandler(void);
void TIM7_IRQHandler(void);
void Compose_My_Sector_Key(unsigned char* card_id, unsigned char block_addr);
unsigned char Purchase_price_Get(void);
unsigned char Card_Pass_Get(void);
unsigned char Get_Key(void);
void Get_Top_Key(void);

#endif /* __TIM_H */

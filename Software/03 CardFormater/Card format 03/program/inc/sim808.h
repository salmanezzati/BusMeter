
#ifndef __SIM808_H
#define __SIM808_H

#include "main.h"

#define DEL_READ     1
#define DEL_UNREAD   2
#define DEL_SENT     3
#define DEL_UNSENT   4
#define DEL_INBOX    5
#define DEL_ALL      6

// GPS Coordinates
extern unsigned char SMS_received_flag;
extern unsigned char SMS_num[1];
extern unsigned char Latitude[14];  //  Latitude, North (vi do)
extern unsigned char Longitude[15]; // Longitude, East  (kinh do)
extern unsigned char Elevation[13];  // Elevation  (do cao)
extern unsigned char Speed[7];
extern unsigned char UTC_Time[7];
extern unsigned char UTC_Date[10];
extern unsigned int dUTC_Time;

extern unsigned char Phone_Num_1[12];
extern unsigned char Phone_Num_2[12];

extern unsigned char HTTP_sucsessful_upload;

void Usart1_Init(void);
void USART1_IRQHandler(void);

void Usart_Send(unsigned char Ch);
void Usart_Send_Str(unsigned char *Data, unsigned char Data_len);

void TIM4_Int_Init(u16 Arr,u16 Psc);
//void TIM2_Int_Init(u16 Arr,u16 Psc);
void Blink_1(void);
void Blink_2(void);
void Sim808_PowerOnOff(void);
void Sim808_Init(void);
unsigned char Get_Signal_Quality(void);
unsigned char check_string(const char *str,unsigned char Clear);
unsigned char check_string2(const char *str,unsigned char Clear);
unsigned char SMS_Send(unsigned char *Phone_num, unsigned char *Content);
void SMS_Read(unsigned char *Index);
void SMS_Receive(void);
unsigned char Check_Change_Setting(void);
void SMS_Delete(unsigned char Type);
void Clear_UART0Buffer(void); 
void Fill_UART0Buffer(void);
void HTTP_Config(void);
void HTTP_Upload(unsigned char *data_str);
void GetInfo(void);
void GetSpeed(void);

#endif /* __SIM808_H */


#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>
#include "stm32f4xx.h"
#include "stdlib.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "lcd.h" 
#include "rtc.h"
#include "SDIO.h"
#include "delay.h"          
#include "stm32f4xx_gpio.h" 
#include "stm32f4xx_exti.h" 
#include "stm32f4xx_tim.h"  
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_pwr.h"
#include "misc.h"           
#include "stm32f4xx_rcc.h"
#include "gpio.h"
#include "exti.h"
#include "tim.h"
#include "sim808.h"        
#include "stm32f4_sdio_sd.h"
#include "ff.h"
#include "sd_bmp.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_mfrc522.h"
//#include "Date_Convert.h"

typedef struct
{
  int RTC_WeekDay; 
  int RTC_Month;  
  int RTC_Date;  
  int RTC_Year;    
}RTC_DateType;

#define bus_mode       0
#define taxi_mode      1
#define shop_mode      2

#define device_id_addr                   4000
#define location_data_counter_addr       4001
#define financial_data_counter_addr      4002
#define location_write_block_addr        4003
#define location_write_block_end_addr    4004
#define location_send_block_addr         4005
#define financial_write_block_addr       4006
#define financial_write_block_end_addr   4007
#define financial_send_block_addr        4008
#define trip_price_addr                  4009
#define location_data_time_addr          4010
#define data_time_addr                   4011
#define phone_num_1_addr                 4012
#define phone_num_2_addr                 4013
#define password_addr                    4014
#define current_month_addr               4015
#define current_day_addr                 4016
#define daily_operation_amount_addr      4017
#define front_door_card_addr             4018
#define back_door_card_addr              4019
#define url_1_addr              		 4020
#define url_2_addr                       4021
#define line_code_addr                   4022
#define line_name_addr                   4023

#define LCD_Backlight_ON    GPIO_SetBits(GPIOG, GPIO_Pin_4);
#define LCD_Backlight_OFF   GPIO_ResetBits(GPIOG, GPIO_Pin_4);

#define Buzzer_ON    GPIO_SetBits(GPIOF, GPIO_Pin_7);
#define Buzzer_OFF   GPIO_ResetBits(GPIOF, GPIO_Pin_7);

#define Sim808_on         GPIO_SetBits(GPIOG, GPIO_Pin_3);
#define Sim808_off        GPIO_ResetBits(GPIOG, GPIO_Pin_3);

extern const unsigned char Front_door[];
extern const unsigned char Back_door[];
extern const unsigned char Daily_operation[];
extern const unsigned char No_new_massage[];
extern const unsigned char Line_name[];
extern const unsigned char Line_fare[];
extern const unsigned char Sun[];
extern const unsigned char Mon[];
extern const unsigned char Tue[];
extern const unsigned char Wed[];
extern const unsigned char Thu[];
extern const unsigned char Fri[];
extern const unsigned char Sat[];


extern char path1[];
extern char path2[];
extern char path3[];
extern char path4[];

extern unsigned char Key_str[10];
extern unsigned char Pass[5];
extern int D_sec, Usart_sec;
extern unsigned  char Code[4];
extern unsigned char Buffer_1[512], Buffer_2[512], L_Tx_1[250], L_Tx_2[250], L_Tx_3[250], L_Tx_4[250], F_Tx_1[250], F_Tx_2[250], F_Tx_3[250], F_Tx_4[250], Buffer_tx[500], Buffer_rx[500], Buffer_SMS[250], RF_buffer_ok_data[25];
extern unsigned char Show_phone, Phone_flag, Draw_arm_button, Show_sim808, Show_rc522;
extern unsigned char Show_daily_operation, Show_center_massage, Show_change_device_mode;
extern unsigned char Backgrond, Light_off;
extern unsigned char Sending_id[8][7];

//Recognized card ID
extern unsigned char Card_ID[5], Block_Addr, Card_detection, Access_Bits[4];
extern unsigned char Sector_Key_1[6];
extern unsigned char Sector_Key_2[6];
extern unsigned char Block_Data_1[16]; 
extern unsigned char Block_Data_2[16]; 
extern unsigned char Block_Data_3[16];
extern unsigned char Block_Data_4[16];
extern unsigned long int Value_1, Value_2, Trip_price, Daily_operation_amount;
extern unsigned int Front_door_card, Back_door_card;
extern unsigned char Defined_card;

extern unsigned int Temp;
extern unsigned char Location_get_flag, Location_data_time, Data_send_flag, Data_time, Shop_data_send_flag;
extern unsigned int Location_write_block, Location_send_block, Financial_write_block, Financial_send_block;
extern unsigned int Location_write_block_end, Financial_write_block_end;
extern unsigned int Location_data_counter, Financial_data_counter;
extern unsigned char Location_Data_send_counter, Financial_Data_send_counter;
extern unsigned char Device_ID[7];
extern unsigned char Device_mode;
extern unsigned char Purchase_price[9];
extern unsigned char Card_password[5];
extern unsigned char Peyment_status;
extern unsigned char Show_signal_quality_flag;
extern unsigned char GSM_Signal_quality, GPS_Signal_quality;

extern unsigned char Key_char[2];

extern RTC_TimeTypeDef  RTC_TimeStruct;
extern RTC_DateTypeDef  M_RTC_DateStruct, Sh_RTC_DateStruct;
extern RTC_DateType     M_RTC_Date, Sh_RTC_Date;
extern unsigned char Set_time_flag;
extern unsigned char Date[11], Time[7];
extern unsigned long New_secound, Last_change_secound_1, Last_change_secound_2;

void Loading(void);
void microSD_Test(void);
void Clear_Buffer(unsigned char *buffer,unsigned int buffer_size);
void Micro_SD_Write_Starting_Data(void);
void Micro_SD_Read_Starting_Data(void);
void Location_Icon(char Active_mode);
void Massege_Icon(char Active_mode);
void Danger_Icon(char Active_mode);
void Data_Icon_Up(char Active_mode);
void Data_Icon_Down(char Active_mode);

#endif /* __MAIN_H */

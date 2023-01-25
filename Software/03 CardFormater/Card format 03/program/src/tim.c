
#include "tim.h"


unsigned char i;
unsigned char Show_time=0, Show_setting=0, Set_num=0 , Show_signal_counter=0;
unsigned char Current_month=0, Current_day=0;

const unsigned char Successful_peyment[] =
{
    0x28, 0x06, 0x31, 0x06, 0x2F, 0x06, 0x27, 0x06, 0x2E, 0x06, 0x2A, 0x06, 0x20, 0x00, 
    0x45, 0x06, 0x48, 0x06, 0x41, 0x06, 0x42, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Low_money[] =
{
    0xA9, 0x06, 0x45, 0x06, 0x28, 0x06, 0x48, 0x06, 0x2F, 0x06, 0x20, 0x00, 
    0x27, 0x06, 0x39, 0x06, 0x2A, 0x06, 0x28, 0x06, 0x27, 0x06, 0x31, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Undefined_card[] =
{
    0xA9, 0x06, 0x27, 0x06, 0x31, 0x06, 0x2A, 0x06, 0x20, 0x00, 
    0x46, 0x06, 0x27, 0x06, 0x45, 0x06, 0x39, 0x06, 0x2A, 0x06, 0x28, 0x06, 0x31, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Successful_format[] =
{
    0x41, 0x06, 0x31, 0x06, 0x45, 0x06, 0x2A, 0x06, 0x20, 0x00, 
    0x45, 0x06, 0x48, 0x06, 0x41, 0x06, 0x42, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Formated_befor[] =
{
    0x42, 0x06, 0x28, 0x06, 0x44, 0x06, 0x27, 0x06, 0x20, 0x00,
	0x41, 0x06, 0x31, 0x06, 0x45, 0x06, 0x2A, 0x06, 0x20, 0x00, 
	0x34, 0x06, 0x2F, 0x06, 0x47, 0x06, 0x20, 0x00,
    0x27, 0x06, 0x33, 0x06, 0x2A, 0x06, 0x20, 0x00, 0x00
} ;

void TIM1_Int_Init(u32 arr,u32 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 
 
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM1, ENABLE);  
}

void TIM10_Int_Init(u32 arr,u32 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE); 
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure); 
 
    TIM_ClearITPendingBit(TIM10, TIM_IT_Update);  
	TIM_ITConfig(TIM10,TIM_IT_Update,ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM10, ENABLE);  
}


void TIM1_UP_TIM10_IRQHandler(void)   
{
//---LCD Show Tim-------------    
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  
        TIM_Cmd(TIM1, DISABLE);  
        
        
        
        TIM_Cmd(TIM1, ENABLE);  
	}

//---Get & Set RTC Tim-------------    
    else if (TIM_GetITStatus(TIM10, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM10, TIM_IT_Update);  
        TIM_Cmd(TIM10, DISABLE); 
        
        if(Set_time_flag == 1)
		{
			Set_time_flag = 0;
			
			RTC_TimeStruct.RTC_Hours = (dUTC_Time/10000);
			RTC_TimeStruct.RTC_Minutes = ((dUTC_Time%10000)/100);
			RTC_TimeStruct.RTC_Seconds = (dUTC_Time%100)+2;
			RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
		}
//         M_RTC_DateStruct.RTC_Year=19;
//         M_RTC_DateStruct.RTC_Month=10;
//         M_RTC_DateStruct.RTC_Date=8;
//         M_RTC_DateStruct.RTC_WeekDay=3;
//         RTC_SetDate(RTC_Format_BIN, &M_RTC_DateStruct);
        
		
        RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
        RTC_GetDate(RTC_Format_BIN, &M_RTC_DateStruct);
        M2Sh(&Sh_RTC_Date.RTC_Year, &Sh_RTC_Date.RTC_Month, &Sh_RTC_Date.RTC_Date, (int) 2000+M_RTC_DateStruct.RTC_Year, (int) M_RTC_DateStruct.RTC_Month, (int) M_RTC_DateStruct.RTC_Date);
        Show_time = 1;
//---SD write & send data synchronization-------
		if(Sh_RTC_Date.RTC_Month != Current_month)
		{
			Current_month = Sh_RTC_Date.RTC_Month;
			Clear_Buffer(Buffer_1, 512);
			sprintf(Buffer_1, "%i", Current_month);
			SD_WriteSingleBlock(Buffer_1, current_month_addr);
			
			Format_write_block_end = Format_write_block-1;
			Clear_Buffer(Buffer_1, 512);
			sprintf(Buffer_1, "%i", Format_write_block_end);
			SD_WriteSingleBlock(Buffer_1, format_write_block_end_addr);
		
			Format_write_block = 35000;
			Clear_Buffer(Buffer_1, 512);
			sprintf(Buffer_1, "%i", Format_write_block);
			SD_WriteSingleBlock(Buffer_1, format_write_block_addr);
			
			Format_data_counter = 0;
			Clear_Buffer(Buffer_1, 512);
			sprintf(Buffer_1, "%i", Format_data_counter);
			SD_WriteSingleBlock(Buffer_1, format_data_counter_addr);
		}
//---sim800--------------        
        if(Usart_sec<2000)
            Usart_sec++;
		
		New_secound = (RTC_TimeStruct.RTC_Hours * 3600) + (RTC_TimeStruct.RTC_Minutes * 60) + (RTC_TimeStruct.RTC_Seconds);
		if(Last_change_secound_1 == 0)
		{
			Last_change_secound_1 = New_secound;
			Last_change_secound_2 = New_secound;
		}
		
		if((Last_change_secound_1 + Location_data_time) <= New_secound)
		{	
			Last_change_secound_1 = New_secound;
			//Location_get_flag = 1;
		}
		if((Last_change_secound_2 + Data_time + 2) <= New_secound)
		{	
			Last_change_secound_2 = Last_change_secound_1;
			//Data_send_flag = 1;
		}
		
		Show_signal_counter++;
		if(Show_signal_counter >= 5)
		{
			Show_signal_counter = 0;
			//Show_signal_quality_flag = 1;
		}
		
        TIM_Cmd(TIM10, ENABLE);  
	}
}

//***SIM808 get location data & date & time*****************************
void TIM7_Int_Init(u32 arr,u32 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); 
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); 
 
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =4;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM7, ENABLE);  
}

void TIM7_IRQHandler(void)   
{

	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	TIM_Cmd(TIM7, DISABLE); 
	
	
	TIM_Cmd(TIM7, ENABLE);
}

//***MFRC522 card reader*****************************
void TIM8_Int_Init(u32 arr,u32 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); 
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); 
 
    TIM_ClearITPendingBit(TIM8, TIM_IT_Update);  
	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM8, ENABLE);  
}

void TIM12_Int_Init(u32 arr,u32 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE); 
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure); 
 
    TIM_ClearITPendingBit(TIM12, TIM_IT_Update);  
	TIM_ITConfig(TIM12,TIM_IT_Update,ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_BRK_TIM12_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM12, ENABLE);  
}

void TIM13_Int_Init(u32 arr,u32 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE); 
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure); 
 
    TIM_ClearITPendingBit(TIM13, TIM_IT_Update);  
	TIM_ITConfig(TIM13,TIM_IT_Update,ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM13, ENABLE);  
}

void TIM14_Int_Init(u32 arr,u32 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); 
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure); 
 
    TIM_ClearITPendingBit(TIM14, TIM_IT_Update);  
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_TRG_COM_TIM14_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM14, ENABLE);  
}

void TIM8_UP_TIM13_IRQHandler(void)   
{
	static int j=0;   
    if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
		TIM_Cmd(TIM8, DISABLE); 
		//If any card detected
		
		if(Show_rc522 == 0)
		{
			if(Card_detection == 0)
			{
				MFRC522_AntennaOn();
				if(MFRC522_Check(Card_ID) == MI_OK) 
				{
					MFRC522_AntennaOff();
					//delay_ms(10);
					//Card_detection = 1; 
//					LCD_ShowString(0, 0, 150, 16, 16, "Card detected         ");
//					sprintf(Buffer_1, "0x%02x  0x%02x  0x%02x  0x%02x  0x%02x", Card_ID[0], Card_ID[1], Card_ID[2], Card_ID[3], Card_ID[4]);
//					LCD_ShowString(10, 48, 300, 16, 16, Buffer_1);
//					delay_ms(2000);
					
					Block_Addr = 3;
					//Block_Addr = ((((Card_ID[3] & 0xFC) >> 2) / 4) * 4) + 3; //Round Block Address
					Compose_My_Sector_Key(Card_ID, Block_Addr); //Coding Sector Key
	
					Block_Data_3[0] = Sector_Key_2[0];
					Block_Data_3[1] = Sector_Key_2[1];
					Block_Data_3[2] = Sector_Key_2[2];
					Block_Data_3[3] = Sector_Key_2[3];
					Block_Data_3[4] = Sector_Key_2[4];
					Block_Data_3[5] = Sector_Key_2[5];
					
//					Block_Data_4[2] = 0xC3;
//					Block_Data_4[3] = 0x50;
//					Block_Data_4[6] = ~(Block_Data_4[2]);
//					Block_Data_4[7] = ~(Block_Data_4[3]);
					
					//MFRC522_SelectTag(Card_ID);
						
					Sector_Key_1[0] = 0xFF;
					Sector_Key_1[1] = 0xFF;
					Sector_Key_1[2] = 0xFF;
					Sector_Key_1[3] = 0xFF;
					Sector_Key_1[4] = 0xFF;
					Sector_Key_1[5] = 0xFF;
					//Checking Sector key
					Defined_card = 0;
					j=0;
					MFRC522_AntennaOn();
					while(Sector_Key_1[0] != 0 || Sector_Key_1[1] != 0 || Sector_Key_1[2] != 0 || Sector_Key_1[3] != 0 || Sector_Key_1[4] != 0 || Sector_Key_1[5] != 0)
					{
					
						//sprintf(Buffer_1, "0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x", Sector_Key_1[5], Sector_Key_1[4], Sector_Key_1[3], Sector_Key_1[2], Sector_Key_1[1], Sector_Key_1[0]);
						//LCD_ShowString(10, 48, 300, 16, 16, Buffer_1);
						//MFRC522_AntennaOn();
						if(MFRC522_Check(Card_ID) == MI_OK) 
						{
							//MFRC522_AntennaOn();
							//MFRC522_Check(Card_ID);
							MFRC522_SelectTag(Card_ID);
							if(MFRC522_Auth(PICC_AUTHENT1A, Block_Addr, Sector_Key_1, Card_ID) == MI_OK)
							{
								Defined_card = 1;
								sprintf(Buffer_1, "0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x", Sector_Key_1[5], Sector_Key_1[4], Sector_Key_1[3], Sector_Key_1[2], Sector_Key_1[1], Sector_Key_1[0]);
								LCD_ShowString(10, 64, 300, 16, 16, Buffer_1);
								while(1);
								break;
							}
	//						else
	//						{
	//							MFRC522_Check(Card_ID);
	//							MFRC522_SelectTag(Card_ID);
	//							if(MFRC522_Auth(PICC_AUTHENT1A, Block_Addr, Sector_Key_2, Card_ID) == MI_OK)
	//							{
	//								Defined_card = 2;
	//								break;
	//							}
	//						}
							//MFRC522_AntennaOff();
							
							Sector_Key_1[0] -= 13;
							Sector_Key_1[1] -= 3;
							Sector_Key_1[2] -= 7;
							Sector_Key_1[3] -= 5;
							Sector_Key_1[4] -= 1;
							Sector_Key_1[5] -= 13;
							sprintf(Buffer_1, "0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x", Sector_Key_1[5], Sector_Key_1[4], Sector_Key_1[3], Sector_Key_1[2], Sector_Key_1[1], Sector_Key_1[0]);
							LCD_ShowString(10, 48, 300, 16, 16, Buffer_1);
							
//							if(Sector_Key_1[0] == 0)
//							{
//								Sector_Key_1[1] -= 1;
//								Sector_Key_1[0] = 0xFF;
//								sprintf(Buffer_1, "0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x", Sector_Key_1[5], Sector_Key_1[4], Sector_Key_1[3], Sector_Key_1[2], Sector_Key_1[1], Sector_Key_1[0]);
//								LCD_ShowString(10, 48, 300, 16, 16, Buffer_1);
//								if(Sector_Key_1[1] == 0)
//								{
//									Sector_Key_1[2] -= 1;
//									Sector_Key_1[1] = 0xFF;
//									if(Sector_Key_1[2] == 0)
//									{
//										Sector_Key_1[3] -= 1;
//										Sector_Key_1[2] = 0xFF;
//										if(Sector_Key_1[3] == 0)
//										{
//											Sector_Key_1[4] -= 1;
//											Sector_Key_1[3] = 0xFF;
//											if(Sector_Key_1[4] == 0)
//											{
//												Sector_Key_1[5] -= 1;
//												Sector_Key_1[4] = 0xFF;
//												if(Sector_Key_1[5] == 0)
//												{
//													LCD_ShowString(0, 160, 150, 16, 16, "Can not open password ");
//													while(1);
//												}
//											}
//										}
//									}
//								}
//							}
						}
						else
						{
							LCD_ShowString(0, 0, 150, 16, 16, "Card can not detected         ");
							delay_ms(1000);
							LCD_ShowString(0, 0, 150, 16, 16, "                              ");
						}
						//MFRC522_AntennaOff();
						//j++;
					}
					if(Defined_card == 1)
					{
						Defined_card = 0;
						//Writting sector key
						MFRC522_Write(Block_Addr, Block_Data_3); //Writting sector key
						//MFRC522_Write(Block_Addr-1, Block_Data_4); //Writting chrge value
						
						//Checking Sector key
						j=0;
						while(j < 20)
						{
							MFRC522_AntennaOn();
							MFRC522_Check(Card_ID);
							MFRC522_SelectTag(Card_ID);
							if(MFRC522_Auth(PICC_AUTHENT1A, Block_Addr, Sector_Key_2, Card_ID) == MI_OK)
							{
								Defined_card = 1;
								break;
							}
							MFRC522_AntennaOff();
							j++;
						}
						if(Defined_card == 1)
						{
							Defined_card = 0;
							MFRC522_AntennaOn();
							MFRC522_Halt();
							MFRC522_AntennaOff();
							
							Backgrond=1;
							LCD_Clear(Good_White);
							//LCD_MyShowString(80, 48, 300, 16, 16, "Successful_format", WHITE, MAGENTA);
							myLCD_goto_xy(95, 90);
							unicode2ascii((char *)Successful_format, Buffer_1);
							myLCD_set_font_color(Brand_Blue);
							myLCD_font_effect(5);
							myLCD_putsf_mix((unsigned char*)Buffer_1, 2);
							
							Buzzer_ON
							delay_ms(500);
							Buzzer_OFF
							delay_ms(1000);
							
							Format_data_counter++;
				
							sprintf(Date,"%04i/%02i/%02i", Sh_RTC_Date.RTC_Year/*-1300*/, Sh_RTC_Date.RTC_Month, Sh_RTC_Date.RTC_Date);
							sprintf(Time,"%02i%02i%02i", RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, RTC_TimeStruct.RTC_Seconds);

							Clear_Buffer(Buffer_1, 512);
							sprintf(Buffer_1, "id=%05d&A=%s&B=%s&C=%s&D=%02x%02x%02x%02x%02x", Format_data_counter, Device_ID, Date, Time, Card_ID[0], Card_ID[1], Card_ID[2], Card_ID[3], Card_ID[4]);
														
							do
							{
								SD_WriteSingleBlock(Buffer_1, Format_write_block);
								Clear_Buffer(Buffer_2, 512);
								SD_ReadSingleBlock(Buffer_2, Format_write_block);
							}while(strstr(Buffer_1, Buffer_2) == '\0');
							
							Format_write_block++;
							if(Format_write_block > 60000)
								Format_write_block = 35000;
							
							Clear_Buffer(Buffer_1, 512);
							sprintf(Buffer_1, "%i", Format_data_counter);
							SD_WriteSingleBlock(Buffer_1, format_data_counter_addr);
							
							Clear_Buffer(Buffer_1, 512);
							sprintf(Buffer_1, "%i", Format_write_block);
							SD_WriteSingleBlock(Buffer_1, format_write_block_addr);
						}
					}
					else if(Defined_card == 2)
					{
						Defined_card = 0;
						MFRC522_AntennaOn();
						MFRC522_Halt();
						MFRC522_AntennaOff();
						
						Backgrond=1;
						LCD_Clear(Good_White);
						//LCD_MyShowString(80, 48, 300, 16, 16, "Formated_befor", WHITE, MAGENTA);
						myLCD_goto_xy(60, 90);
						unicode2ascii((char *)Formated_befor, Buffer_1);
						myLCD_set_font_color(Brand_Blue);
						myLCD_font_effect(5);
						myLCD_putsf_mix((unsigned char*)Buffer_1, 2);
						
						Buzzer_ON
						delay_ms(100);
						Buzzer_OFF
						delay_ms(50);
						Buzzer_ON
						delay_ms(100);
						Buzzer_OFF
						delay_ms(1000);
					}
					else //Undefined card
					{
						Defined_card = 0;
						MFRC522_AntennaOn();
						MFRC522_Halt();
						MFRC522_AntennaOff();
						
						Backgrond=1;
						LCD_Clear(Good_White);
						//LCD_MyShowString(80, 48, 300, 16, 16, "Undefined_card", WHITE, MAGENTA);
						myLCD_goto_xy(95, 90);
						unicode2ascii((char *)Undefined_card, Buffer_1);
						myLCD_set_font_color(Brand_Blue);
						myLCD_font_effect(5);
						myLCD_putsf_mix((unsigned char*)Buffer_1, 2);
						
						Buzzer_ON
						delay_ms(100);
						Buzzer_OFF
						delay_ms(50);
						Buzzer_ON
						delay_ms(100);
						Buzzer_OFF
						delay_ms(1000);
					}
				}
				MFRC522_AntennaOff();
			}			
			else if(Card_detection == 1)
			{
				Card_detection = 0;
				MFRC522_1_Init();
			}
			if(Card_detection == 1)
			{
				MFRC522_1_Init();
				for(j=0; j<40 ; j++)
				{
					//delay_ms(50);
					MFRC522_AntennaOn();
					while(MFRC522_Check_2(Card_ID) == MI_OK)
					{
						MFRC522_AntennaOff();
						j = 0;
						delay_ms(100);
						Buzzer_OFF
					}
					MFRC522_AntennaOff();
				}
			}
			MFRC522_1_Init();
		
        }
		Buzzer_OFF

		TIM_Cmd(TIM8, ENABLE); 
	}
    else if (TIM_GetITStatus(TIM13, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM13, TIM_IT_Update);  
        
        //GPIO_ToggleBits(GPIOB, GPIO_Pin_9);
	}
}

void TIM8_BRK_TIM12_IRQHandler(void)   
{
    if (TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM12, TIM_IT_Update);  
        TIM_Cmd(TIM12, DISABLE);
        
		//Purchase_price_Get();
		
		//Card_Pass_Get();
		
		Get_Top_Key();
			
		TIM_Cmd(TIM12, ENABLE); 
	}
}

void TIM8_TRG_COM_TIM14_IRQHandler(void)   
{
    if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM14, TIM_IT_Update);  
        
        //GPIO_ToggleBits(GPIOB, GPIO_Pin_9);
	}
}
//****************************************************
void TIM11_Int_Init(u32 arr,u32 psc)
{
    uint16_t TimerPeriod = 0;
    uint16_t Channel1Pulse = 0;
    GPIO_InitTypeDef  GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
// 	TIM_TimeBaseStructure.TIM_Period = arr; 
// 	TIM_TimeBaseStructure.TIM_Prescaler =psc;
// 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
// 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
// 	TIM_TimeBaseInit(TIM11, &TIM_TimeBaseStructure); 
//  
//     TIM_ClearITPendingBit(TIM11, TIM_IT_Update);  
// 	TIM_ITConfig(TIM11,TIM_IT_Update,ENABLE );

// 	NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_TIM11_IRQn;  
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
// 	NVIC_Init(&NVIC_InitStructure);  

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_TIM11);
    
    Channel1Pulse = (uint16_t) (((uint32_t) 50 * (8-1)) / 100);
    //200khz 1/4 pw
	TIM_TimeBaseStructure.TIM_Period = (u32) (8-1); 
	TIM_TimeBaseStructure.TIM_Prescaler = (u32) (105-1); 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM11, &TIM_TimeBaseStructure); 

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM11, &TIM_OCInitStructure);
    
    TIM_OC1PreloadConfig(TIM11, TIM_OCPreload_Enable);
    
    TIM_ARRPreloadConfig(TIM11, ENABLE);
    
	TIM_Cmd(TIM11, ENABLE);  
}

void TIM1_TRG_COM_TIM11_IRQHandler(void)   
{
    if (TIM_GetITStatus(TIM11, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM11, TIM_IT_Update);  
        
        //GPIO_ToggleBits(GPIOB, GPIO_Pin_8);
	}
}
//---Compose_My_Sector_Key------------------------
void Compose_My_Sector_Key(unsigned char* card_id, unsigned char block_addr)
{
	for(i=0 ; i<5 ; i++)
	{
		Sector_Key_2[i] = (card_id[i] * i) + block_addr;
	}
	Sector_Key_2[5] = (card_id[1] + card_id[3]) + block_addr;
}

//---Purchase_price_Get----------------------------
unsigned char Purchase_price_Get(void)
{
	unsigned char i, j, Key=0;
	Clear_Buffer(Key_str, 10);
	for(j=1;j<9;j++)
	{
		i = j-1;
		Key = 0;
		while(Key == 0)
			Key = Get_Key();
		
		if(i<8 && Key != 4 && Key != 8 && Key != 12 && Key != 16)
		{	
			switch(Key)
			{
				case 1: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 80, 319, 16, 16, Key_char, 1);
					break;
				
				case 2: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 80, 319, 16, 16, Key_char, 1);
					break;
				
				case 3: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 80, 319, 16, 16, Key_char, 1);
					break;
				
				case 4: 
					break;
				
				case 5: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 80, 319, 16, 16, Key_char, 1);
					break;
				
				case 6: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 80, 319, 16, 16, Key_char, 1);
					break;
				
				case 7: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 80, 319, 16, 16, Key_char, 1);
					break;
				
				case 8: 
					break;
				
				case 9: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 80, 319, 16, 16, Key_char, 1);
					break;
				
				case 10: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 80, 319, 16, 16, Key_char, 1);
					break;
				
				case 11: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 80, 319, 16, 16, Key_char, 1);
					break;
				
				case 12: 
					break;
				
				case 13: 
					j = 0;
					Clear_Buffer(Key_str, 10);
					LCD_MyShowString(137, 115, 300, 16, 16, "          ", Brand_Blue, Good_White);
					break;
				
				case 14: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 80, 319, 16, 16, Key_char, 1);
					break;
				
				case 15: 
					Key_str[i] = 0;
					strcpy(Purchase_price, Key_str);
					//LCD_ShowStringWithSize(0, 144, 319, 16, 16, Purchase_price,10);
					j = 9; //return 1;
					break;
				
				case 16: 
					break;
				
				default:
					break;
			}
			
			//LCD_ShowStringWithSize(0, 80, 319, 16, 16, Key_str,10);
			LCD_MyShowString(137, 115, 300, 16, 16, Key_str, Brand_Blue, Good_White);
			
			Buzzer_ON
			delay_ms(100);
			Buzzer_OFF
			
			while(Get_Key() != 0)
			{
				while(Get_Key() != 0)
					delay_ms(150);
				delay_ms(150);
			}
		}
	}
}

//---Card_Pass_Get---------------------------------
unsigned char Card_Pass_Get(void)
{
	unsigned char i, j, Key=0;
	Clear_Buffer(Key_str, 10);
	for(j=1;j<6;j++)
	{
		i = j-1;
		Key = 0;
		while(Key == 0)
			Key = Get_Key();
		
		if((i<4 && Key != 4 && Key != 8 && Key != 12 && Key != 13 && Key != 15 && Key != 16) || (i>=4 && (Key == 13 || Key == 15)))
		{	
			switch(Key)
			{
				case 1: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 112, 319, 16, 16, Key_char, 1);
					break;
				
				case 2: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 112, 319, 16, 16, Key_char, 1);
					break;
				
				case 3: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 112, 319, 16, 16, Key_char, 1);
					break;
				
				case 4: 
					break;
				
				case 5: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 112, 319, 16, 16, Key_char, 1);
					break;
				
				case 6: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 112, 319, 16, 16, Key_char, 1);
					break;
				
				case 7: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 112, 319, 16, 16, Key_char, 1);
					break;
				
				case 8: 
					break;
				
				case 9: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 112, 319, 16, 16, Key_char, 1);
					break;
				
				case 10: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 112, 319, 16, 16, Key_char, 1);
					break;
				
				case 11: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 112, 319, 16, 16, Key_char, 1);
					break;
				
				case 12: 
					break;
				
				case 13: 
					j = 0;
					Clear_Buffer(Key_str, 10);
					LCD_MyShowString(145, 162, 300, 16, 16, "    ", Brand_Blue, Good_White);
					break;
				
				case 14: 
					Key_str[i] = Key_char[0];
					//LCD_ShowStringWithSize((i*8), 112, 319, 16, 16, Key_char, 1);
					break;
				
				case 15: 
					Key_str[i] = 0;
					strcpy(Card_password, Key_str);
					//LCD_ShowStringWithSize(0, 144, 319, 16, 16, Card_password,10);
					j = 6;//return 1;
					break;
				
				case 16: 
					break;
				
				default:
					break;
			}
			
			//LCD_ShowStringWithSize(0, 112, 319, 16, 16, Key_str,6);
			if(i < 4)
				LCD_MyShowString(145+(i*8), 162, 300, 16, 16, "*", Brand_Blue, Good_White);
			
			Buzzer_ON
			delay_ms(100);
			Buzzer_OFF
			
			while(Get_Key() != 0)
			{
				while(Get_Key() != 0)
					delay_ms(150);
				delay_ms(150);
			}
		}
	}
}

//---Get_Key---------------------------------------
unsigned char Get_Key(void)
{
	Key_char[0]=0;
	Key_char[1]=0;
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	GPIO_ResetBits(GPIOC, GPIO_Pin_5);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	delay_us(10);
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4))
	{
		Key_char[0]='1';
		//while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4));
		return 1;
	}
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))
	{
		Key_char[0]='4';
		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7));
		return 5;
	}
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6))
	{
		Key_char[0]='7';
		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6));
		return 9;
	}
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))
	{
		Key_char[0]='D';
		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5));
		return 13;
	}
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	delay_us(10);
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4))
	{
		Key_char[0]='2';
		//while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4));
		return 2;
	}
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))
	{
		Key_char[0]='5';
		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7));
		return 6;
	}
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6))
	{
		Key_char[0]='8';
		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6));
		return 10;
	}
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))
	{
		Key_char[0]='0';
		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5));
		return 14;
	}
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	delay_us(10);
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4))
	{
		Key_char[0]='3';
		//while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4));
		return 3;
	}
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))
	{
		Key_char[0]='6';
		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7));
		return 7;
	}
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6))
	{
		Key_char[0]='9';
		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6));
		return 11;
	}
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))
	{
		Key_char[0]='O';
		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5));
		return 15;
	}
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_5);
	delay_us(10);
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4))
	{
		Key_char[0]='R';
		//while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4));
		return 4;
	}
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))
	{
		Key_char[0]='B';
		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7));
		return 8;
	}
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6))
	{
		Key_char[0]='F';
		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6));
		return 12;
	}
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))
	{
		Key_char[0]='S';
		//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5));
		return 16;
	}
	GPIO_ResetBits(GPIOC, GPIO_Pin_5);
	
	return 0;
}

//---Get_Top_Key---------------------------------
void Get_Top_Key(void)
{
	unsigned char i;
	if(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11))
	{
		i = 0;
		while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11))
		{
			i++;
			delay_ms(30);
			if(i >=10)
			{
				Show_daily_operation = 1;
				break;
			}
		}
	}
	
//	else if(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13))
//	{
//		i = 0;
//		while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13))
//		{
//			i++;
//			delay_ms(30);
//			if(i >=10)
//			{
//				Show_center_massage = 1;
//				break;
//			}
//		}
//	}
//	
//	else if(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14))
//	{
//		i = 0;
//		while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14))
//		{
//			i++;
//			delay_ms(100);
//			if(i >=10)
//			{
//				Show_change_device_mode = 1;
//				break;
//			}
//		}
//	}
	
}


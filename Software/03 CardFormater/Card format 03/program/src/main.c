/*****************************************************************************






*****************************************************************************/
#include "main.h"
#include "Date_Convert.h"

/* Private variables ---------------------------------------------------------*/
SD_Error Status = SD_OK;
FATFS filesystem;		/* volume lable */
FRESULT ret;			  /* Result code */
FIL file;				    /* File object */
DIR dir;				    /* Directory object */
FILINFO fno;			  /* File information object */
UINT bw, br;

char Pic_Path_1[]="0:/Brand.bmp";

const unsigned char Sun[] = 
{
    0xCC, 0x06, 0xA9, 0x06, 0x34, 0x06, 0x46, 0x06, 0x28, 0x06, 0x47, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Mon[] = 
{
    0x2F, 0x06, 0x48, 0x06, 0x34, 0x06, 0x46, 0x06, 0x28, 0x06, 0x47, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Tue[] = 
{
    0x33, 0x06, 0x47, 0x06, 0x20, 0x00, 0x34, 0x06, 0x46, 0x06, 0x28, 0x06, 0x47, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Wed[] = 
{
    0x2C, 0x06, 0x47, 0x06, 0x27, 0x06, 0x31, 0x06, 0x34, 0x06, 0x46, 0x06, 0x28, 0x06, 0x47, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Thu[] = 
{
    0x28, 0x06, 0x46, 0x06, 0x2C, 0x06, 0x20, 0x00, 0x34, 0x06, 0x46, 0x06, 0x28, 0x06, 0x47, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Fri[] = 
{
    0x2C, 0x06, 0x45, 0x06, 0x39, 0x06, 0x47, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Sat[] = 
{
    0x34, 0x06, 0x46, 0x06, 0x28, 0x06, 0x47, 0x06, 0x20, 0x00, 0x00
} ;

const unsigned char Daily_operation[] = 
{
    0xA9, 0x06, 0x27, 0x06, 0x31, 0x06, 0xA9, 0x06, 0x31, 0x06, 0x2F, 0x06, 0x20, 0x00, 
	0x31, 0x06, 0x48, 0x06, 0x32, 0x06, 0x27, 0x06, 0x46, 0x06, 0x47, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Front_door[] = 
{
    0x2F, 0x06, 0x31, 0x06, 0x28, 0x06, 0x20, 0x00, 0x2C, 0x06, 0x44, 0x06, 0x48, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Back_door[] = 
{
    0x2F, 0x06, 0x31, 0x06, 0x28, 0x06, 0x20, 0x00, 0x39, 0x06, 0x42, 0x06, 0x28, 0x06, 0x20, 0x00, 0x00
} ;

const unsigned char No_new_massage[] = 
{
    0x28, 0x06, 0xCC, 0x06, 0x27, 0x06, 0x45, 0x06, 0x20, 0x00, 
	0x2C, 0x06, 0x2F, 0x06, 0xCC, 0x06, 0x2F, 0x06, 0xCC, 0x06, 0x20, 0x00, 
	0x45, 0x06, 0x48, 0x06, 0x2C, 0x06, 0x48, 0x06, 0x2F, 0x06, 0x20, 0x00, 
	0x46, 0x06, 0x45, 0x06, 0xCC, 0x06, 0x20, 0x00, 0x28, 0x06, 0x27, 0x06, 0x34, 0x06, 0x2F, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Buy[] = 
{
    0x2E, 0x06, 0x31, 0x06, 0xCC, 0x06, 0x2F, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Buy_amount[] = 
{
    0x45, 0x06, 0x28, 0x06, 0x44, 0x06, 0x3A, 0x06, 0x20, 0x00, 
	0x2E, 0x06, 0x31, 0x06, 0xCC, 0x06, 0x2F, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Card_pass[] = 
{
    0x31, 0x06, 0x45, 0x06, 0x32, 0x06, 0x20, 0x00, 
	0xA9, 0x06, 0x27, 0x06, 0x31, 0x06, 0x2A, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Data_sending[] = 
{
    0x27, 0x06, 0x31, 0x06, 0x33, 0x06, 0x27, 0x06, 0x44, 0x06, 0x20, 0x00, 
	0x27, 0x06, 0x37, 0x06, 0x44, 0x06, 0x27, 0x06, 0x39, 0x06, 0x27, 0x06, 0x2A, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Successful_transaction[] = 
{
    0x2A, 0x06, 0x31, 0x06, 0x27, 0x06, 0xA9, 0x06, 0x46, 0x06, 0x34, 0x06, 0x20, 0x00, 
	0x45, 0x06, 0x48, 0x06, 0x41, 0x06, 0x42, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Password_error[] = 
{
    0x31, 0x06, 0x45, 0x06, 0x32, 0x06, 0x20, 0x00, 
	0x27, 0x06, 0x34, 0x06, 0x2A, 0x06, 0x28, 0x06, 0x27, 0x06, 0x47, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Low_cash[] = 
{
    0xA9, 0x06, 0x45, 0x06, 0x28, 0x06, 0x48, 0x06, 0x2F, 0x06, 0x20, 0x00, 
    0x27, 0x06, 0x39, 0x06, 0x2A, 0x06, 0x28, 0x06, 0x27, 0x06, 0x31, 0x06, 0x20, 0x00, 0x00
} ;

unsigned char Key_str[10];
unsigned char Key[13]={0,1,2,3,4,5,6,7,8,9,0,11,12};
unsigned char Pass[5];
int D_sec, Usart_sec;
unsigned  char Code[4];
unsigned char Buffer_1[512], Buffer_2[512], Buffer_tx[250], Buffer_rx[250], Buffer_SMS[250];
unsigned char Show_phone=1, Phone_flag=0, Draw_arm_button=0, Show_sim808=0, Show_rc522=0;
unsigned char Show_daily_operation=0, Show_center_massage=0, Show_change_device_mode=0;
unsigned char Backgrond=0, Light_off=0;
unsigned char Sending_id[6];
	
//Recognized card ID
unsigned char Card_ID[5], Block_Addr, Card_detection=0;
unsigned char Sector_Key_1[6]={0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char Sector_Key_2[6];
unsigned char Sector_Key_3[6]={0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5};
unsigned char Sector_Key_4[6]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char Sector_Key_5[6]={0x09, 0x08, 0x07, 0x06, 0x05, 0x04};

unsigned char Block_Data_1[16]; 
unsigned char Block_Data_2[16]; 
unsigned char Block_Data_3[16];
unsigned char Block_Data_4[16];
unsigned long int Value_1=0, Value_2=0, Trip_price=210, Daily_operation_amount=0;
unsigned int Front_door_card=0, Back_door_card=0;
unsigned char Defined_card=0;

unsigned int Temp;
unsigned char Location_get_flag=0, Location_data_time=30, Data_send_flag=0, Data_time=60, Shop_data_send_flag=0;
unsigned int Location_write_block=5000, Location_send_block=5000, Format_write_block=35000, Format_send_block=35000;
unsigned int Location_write_block_end=30000, Format_write_block_end=60000;
unsigned int Location_data_counter=0, Format_data_counter=0;
unsigned char Location_Data_send_counter, Format_Data_send_counter;
unsigned char Device_ID[7] = {'0','0','0','0','0','0','\0'};
unsigned char Device_mode=0;
unsigned char Purchase_price[9];
unsigned char Card_password[5];
unsigned char Peyment_status=0;
unsigned char Show_signal_quality_flag=1;
unsigned char Signal_quality;

unsigned char Key_char[2];

RTC_TimeTypeDef  RTC_TimeStruct;
RTC_DateTypeDef  M_RTC_DateStruct, Sh_RTC_DateStruct;
RTC_DateType     M_RTC_Date, Sh_RTC_Date;
unsigned char Set_time_flag=0;
unsigned char Date[11], Time[7];
unsigned long New_secound=0, Last_change_secound_1=0, Last_change_secound_2=0;

static void fault_err (FRESULT rc)
{
    const char *str =
                    "OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
                    "INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
                    "INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
                    "LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0";
    FRESULT i;

    for (i = (FRESULT)0; i != rc && *str; i++) 
    {
        while (*str++) ;
    }
    sprintf(Buffer_1,"rc=%u FR_%s\n\r", (UINT)rc, str);
    LCD_MyShowString(10, 156, 200, 16, 16, Buffer_1, WHITE, MAGENTA);
    while(1);
}


int main(void)
{
	unsigned int i=0;
	
	delay_ms(100);
	
	//Uart_Init(115200);
	GPIO_Initi();
    LCD_Init();
	//LCD_Clear(WHITE);
	//LCD_Clear(MAGENTA);
//	LCD_Backlight_ON
//	LCD_Clear(WHITE);
//	delay_ms(2000);	
//	LCD_Clear(Good_White);
//	delay_ms(2000);	
	
    if (f_mount(0, &filesystem) != FR_OK) 
    {
        LCD_ShowString(0, 15, 300, 16, 16,"could not open filesystem"); 
    }
	//---Brand show-------------
	Bmp_Show(0,0,Pic_Path_1,0);
	LCD_Backlight_ON
//	delay_ms(3000);	
	//LCD_Backlight_OFF
	//LCD_Clear(WHITE); 
	
	SD_LowLevel_Initial();
	SD_Initial();
	
	Micro_SD_Write_Starting_Data();
	Micro_SD_Read_Starting_Data();
	
//***Sim808_Test******************************************
	//TIM10_Int_Init(3999,41999);
	//Usart1_Init ();
	//TIM4_Int_Init(19,41999);
	//LCD_Backlight_ON
	//Sim808_PowerOnOff();
	//Sim808_Init();
//***main_program******************************************    
    
//     LCD_Clear(WHITE);
//     
//     Loading();
//     sprintf(Buffer_1,"Memory initialing...            ");
//     LCD_MyShowString(88, 160, 200, 16, 16, Buffer_1, Brand_Blue, WHITE); 
//     
//     Loading();
//     sprintf(Buffer_1,"  GSM & GPS initialing...       ");
//     LCD_MyShowString(88, 160, 200, 16, 16, Buffer_1, Brand_Blue, WHITE); 
//     
//     Loading();
//     sprintf(Buffer_1," Card reader initialing...      ");
//     LCD_MyShowString(88, 160, 200, 16, 16, Buffer_1, Brand_Blue, WHITE); 
//     
//     Loading(); 
//     sprintf(Buffer_1,"Peripheral Testing...           ");
//     LCD_MyShowString(88, 160, 200, 16, 16, Buffer_1, Brand_Blue, WHITE); 
//     
//     Loading();
          

	MFRC522_1_Init();
	TIM8_Int_Init(1999,41999); //0.4 secound
	
    RTC_Start();
    Backgrond=1;
	//TIM7_Int_Init(19999,41999); //10 secound
	
	TIM10_Int_Init(3999,41999); //1 secound
	TIM12_Int_Init(399,41999); //0.1 secound
	TIM_GenerateEvent(TIM10,1);
    //TIM1_Int_Init(799,41999);
	
    while(1)
    {
		if(Show_daily_operation)
		{
			Backgrond = 1;
			LCD_Clear(Good_White);
				
			Clear_Buffer(Buffer_1, 512);
			sprintf(Buffer_1,"%03i", Front_door_card);
			Button2(220, 180, 310, 230, LGRAY, 9, 16," ", Brand_Blue, 2, 16,Buffer_1, Brand_Blue);
			myLCD_goto_xy(32, 183);
			unicode2ascii((char *)Front_door, Buffer_1);
			myLCD_set_font_color(Brand_Blue);
			myLCD_font_effect(5);
			myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
			
			Clear_Buffer(Buffer_1, 512);
			sprintf(Buffer_1,"%03i", Back_door_card);
			Button2(10, 180, 100, 230, LGRAY, 10, 16," ", Brand_Blue, 2, 16,Buffer_1, Brand_Blue);
			myLCD_goto_xy(242, 183);
			unicode2ascii((char *)Back_door, Buffer_1);
			myLCD_set_font_color(Brand_Blue);
			myLCD_font_effect(5);
			myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
			
			Clear_Buffer(Buffer_1, 512);
			sprintf(Buffer_1,"%06i", Daily_operation_amount);			
			Button2(90, 80, 230, 130, LGRAY, 15, 16," ", Brand_Blue, 6, 16,Buffer_1, Brand_Blue);
			myLCD_goto_xy(120, 83);
			unicode2ascii((char *)Daily_operation, Buffer_1);
			myLCD_set_font_color(Brand_Blue);
			myLCD_font_effect(5);
			myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
			
			Buzzer_ON
			delay_ms(300);
			Buzzer_OFF
			
			delay_ms(3000);
			Show_daily_operation = 0;
		}
//		else if(Show_center_massage)
//		{
//			Backgrond = 1;
//			LCD_Clear(Good_White);
//			
//			myLCD_goto_xy(20, 20);
//			unicode2ascii((char *)No_new_massage, Buffer_1);
//			myLCD_set_font_color(Brand_Blue);
//			myLCD_font_effect(5);
//			myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
//			
//			Buzzer_ON
//			delay_ms(300);
//			Buzzer_OFF
//			
//			delay_ms(3000);
//			Show_center_massage = 0;
//		}
//		else if(Show_change_device_mode)
//		{
//			Backgrond = 1;
//			LCD_Clear(Good_White);
//			
//			if(Device_mode == 0)
//			{
//				Device_mode = 2;
//				LCD_MyShowString(128, 112, 200, 16, 16, "Shop mode",Brand_Blue, Good_White);
//			}
//			else if(Device_mode == 2)
//			{
//				Device_mode = 0;
//				LCD_MyShowString(128, 112, 200, 16, 16, "Bus mode",Brand_Blue, Good_White);
//			}
//			delay_ms(1000);
//			Show_change_device_mode = 0;
//		}
		else
		{

			if(Backgrond)
			{
				Backgrond=0;
				LCD_Clear(Good_White);
				
				Clear_Buffer(Buffer_1, 512);
				sprintf(Buffer_1,"%03i", Front_door_card);
				Button2(220, 180, 310, 230, LGRAY, 9, 16," ", Brand_Blue, 2, 16,Buffer_1, Brand_Blue);
				myLCD_goto_xy(32, 183);
				unicode2ascii((char *)Front_door, Buffer_1);
				myLCD_set_font_color(Brand_Blue);
				myLCD_font_effect(5);
				myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
				
				Clear_Buffer(Buffer_1, 512);
				sprintf(Buffer_1,"%03i", Back_door_card);
				Button2(10, 180, 100, 230, LGRAY, 10, 16," ", Brand_Blue, 2, 16,Buffer_1, Brand_Blue);
				myLCD_goto_xy(242, 183);
				unicode2ascii((char *)Back_door, Buffer_1);
				myLCD_set_font_color(Brand_Blue);
				myLCD_font_effect(5);
				myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
				
				Show_signal_quality_flag = 1;
                //GPIO_ResetBits(GPIOB, GPIO_Pin_10);
                //GPIO_SetBits(GPIOE, GPIO_Pin_5);
			}
//---Time----------------------------
			if(Show_time)
			{
				Show_time = 0;
				sprintf(Buffer_1,"%02i:%02i", RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes/*, RTC_TimeStruct.RTC_Seconds*/);
				LCD_MyShowString3 (107, 50, 48, Buffer_1,Brand_Blue, Good_White);
				
				sprintf(Buffer_1,"%02i/%02i/%02i", Sh_RTC_Date.RTC_Year/*-1300*/, Sh_RTC_Date.RTC_Month, Sh_RTC_Date.RTC_Date);
				LCD_MyShowString(90, 85, 200, 16, 16, Buffer_1,Brand_Blue, Good_White);
				
				switch(M_RTC_DateStruct.RTC_WeekDay)
				{
					case 1: 
						//sprintf(Buffer_1,"Sun");
						myLCD_goto_xy(90, 85);
						unicode2ascii((char *)Sun, Buffer_1);
						break;
					case 2: 
						//sprintf(Buffer_1,"Mon");
						myLCD_goto_xy(90, 85);
						unicode2ascii((char *)Mon, Buffer_1);
						break;
					case 3: 
						//sprintf(Buffer_1,"Tue");
						myLCD_goto_xy(90, 85);
						unicode2ascii((char *)Tue, Buffer_1);
						break;
					case 4: 
						//sprintf(Buffer_1,"Wed");
						myLCD_goto_xy(90, 85);
						unicode2ascii((char *)Wed, Buffer_1);
						break;
					case 5: 
						//sprintf(Buffer_1,"Thu");
						myLCD_goto_xy(90, 85);
						unicode2ascii((char *)Thu, Buffer_1);
						break;
					case 6: 
						//sprintf(Buffer_1,"Fri");
						myLCD_goto_xy(90, 85);
						unicode2ascii((char *)Fri, Buffer_1);
						break;
					case 7: 
						//sprintf(Buffer_1,"Sat");
						myLCD_goto_xy(90, 85);
						unicode2ascii((char *)Sat, Buffer_1);
						break;
				}
				myLCD_set_font_color(Brand_Blue);
				myLCD_font_effect(5);
				myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
				//LCD_MyShowString(193, 85, 40, 16, 16, Buffer_1,Brand_Blue, Good_White);
				
				LCD_Backlight_ON
			
			}
//---Setting whit SMS----------------------------			
			if(SMS_received_flag)
			{
				SMS_received_flag=0;
				
				TIM_Cmd(TIM8, DISABLE);
				TIM_Cmd(TIM10, DISABLE);
				TIM_Cmd(TIM12, DISABLE);
				SMS_Read(SMS_num);
				
				SMS_Delete(DEL_ALL);
				
				TIM_Cmd(TIM8, ENABLE);
				TIM_Cmd(TIM10, ENABLE);
				TIM_Cmd(TIM12, ENABLE);
				
				
			}
        }
//---Get & save location data----------------------------		
//		if(Location_get_flag)
//		{
//			Location_get_flag = 0;
//			
//			TIM_Cmd(TIM8, DISABLE);
//			TIM_Cmd(TIM10, DISABLE);
//			TIM_Cmd(TIM12, DISABLE);
//			GetInfo();
//			TIM_Cmd(TIM8, ENABLE);
//			TIM_Cmd(TIM10, ENABLE); 
//			TIM_Cmd(TIM12, ENABLE);
//			
//			Location_data_counter++;
//			sprintf(Date,"%04i/%02i/%02i", Sh_RTC_Date.RTC_Year/*-1300*/, Sh_RTC_Date.RTC_Month, Sh_RTC_Date.RTC_Date);
//			sprintf(Time,"%02i%02i%02i", RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, RTC_TimeStruct.RTC_Seconds);
//			
//			Clear_Buffer(Buffer_1, 512);
//			sprintf(Buffer_1, "id=%05d&A=%s&B=%s&C=%s&D=%s&E=%s", Location_data_counter, Device_ID, Date, Time, Latitude, Longitude);
//			
//			do
//			{
//				SD_WriteSingleBlock(Buffer_1, Location_write_block);
//				Clear_Buffer(Buffer_2, 512);
//				SD_ReadSingleBlock(Buffer_2, Location_write_block);
//			}while(strstr(Buffer_1, Buffer_2) == '\0');
//			
//			Location_write_block++;
//			if(Location_write_block > 30000)
//				Location_write_block = 5000;
//			
//			Clear_Buffer(Buffer_1, 512);
//			sprintf(Buffer_1, "%i", Location_data_counter);
//			SD_WriteSingleBlock(Buffer_1, location_data_counter_addr);
//			
//			Clear_Buffer(Buffer_1, 512);
//			sprintf(Buffer_1, "%i", Location_write_block);
//			SD_WriteSingleBlock(Buffer_1, location_write_block_addr);
//			
//			//LCD_ShowStringWithSize(0, 112, 319, 16, 16, Buffer_2, 120);
//			
//			if(RTC_TimeStruct.RTC_Minutes==00 && RTC_TimeStruct.RTC_Seconds<=40)
//			{
//				Set_time_flag = 1;
//			}
//		}
//---Send financial & location data----------------------------		
		if(Data_send_flag)
		{
			Data_send_flag = 0;
			//---Location Data send---------
//			Location_Data_send_counter = 0;
//			while(Location_send_block != Location_write_block && Location_Data_send_counter <2)
//			{
//				Location_Data_send_counter++;
//				Clear_Buffer(Buffer_2, 512);
//				SD_ReadSingleBlock(Buffer_2, Location_send_block);
//				
//				for(i=3;i<8;i++)
//					Sending_id[i-3] = Buffer_2[i];
//				Sending_id[5] = 0;
//				
//				for(i=20;i<30;i++)
//					Date[i-20] = Buffer_2[i];
//				Date[10] = 0;
//				
//				Clear_Buffer(Buffer_1, 512);
//				strcpy (Buffer_1,"http://sebazar.ir/sebApi/dataDastgah/insertData.php?");
//				strcat (Buffer_1,Buffer_2);
//				
//				TIM_Cmd(TIM8, DISABLE);
//				TIM_Cmd(TIM10, DISABLE);
//				TIM_Cmd(TIM12, DISABLE);
//				HTTP_Upload(Buffer_1);
//				TIM_Cmd(TIM8, ENABLE);
//				TIM_Cmd(TIM10, ENABLE);
//				TIM_Cmd(TIM12, ENABLE);
//				
//				if(HTTP_sucsessful_upload)
//				{
//					HTTP_sucsessful_upload = 0;
//					Location_send_block++;
//					if(Location_send_block > 30000 && Location_send_block > Location_write_block_end)
//					{
//						Location_send_block = 5000;
//						Location_write_block_end = 30000;
//						Clear_Buffer(Buffer_1, 512);
//						sprintf(Buffer_1, "%i", Location_write_block_end);
//						SD_WriteSingleBlock(Buffer_1, location_write_block_end_addr);
//					}
//					Clear_Buffer(Buffer_1, 512);
//					sprintf(Buffer_1, "%i", Location_send_block);
//					SD_WriteSingleBlock(Buffer_1, location_send_block_addr);
//					
//					Blink_2();
//				}
//			}
			//---Financial Data send---------
			Format_Data_send_counter = 0;
			while(Format_send_block != Format_write_block && Format_Data_send_counter <2)
			{
				Format_Data_send_counter++;
				Clear_Buffer(Buffer_2, 512);
				SD_ReadSingleBlock(Buffer_2, Format_send_block);
				
				for(i=3;i<8;i++)
					Sending_id[i-3] = Buffer_2[i];
				Sending_id[5] = 0;
				
				for(i=20;i<30;i++)
					Date[i-20] = Buffer_2[i];
				Date[10] = 0;
				
				Clear_Buffer(Buffer_1, 512);
				strcpy (Buffer_1,"http://sebazar.ir/sebApi/dataDastgah/v2/insertData.php?");
				strcat (Buffer_1,Buffer_2);
				
				TIM_Cmd(TIM8, DISABLE);
				TIM_Cmd(TIM10, DISABLE);
				TIM_Cmd(TIM12, DISABLE);
				HTTP_Upload(Buffer_1);
				TIM_Cmd(TIM8, ENABLE);
				TIM_Cmd(TIM10, ENABLE);
				TIM_Cmd(TIM12, ENABLE);
				
				if(HTTP_sucsessful_upload)
				{
					HTTP_sucsessful_upload = 0;
					Format_send_block++;
					if(Format_send_block > 60000 && Format_send_block > Format_write_block_end)
					{
						Format_send_block = 35000;
						Format_write_block_end = 60000;
						Clear_Buffer(Buffer_1, 512);
						sprintf(Buffer_1, "%i", Format_write_block_end);
						SD_WriteSingleBlock(Buffer_1, format_write_block_end_addr);
					}
					Clear_Buffer(Buffer_1, 512);
					sprintf(Buffer_1, "%i", Format_send_block);
					SD_WriteSingleBlock(Buffer_1, format_send_block_addr);
					
					Blink_2();
				}
			}
			//sprintf(Buffer_1, "http://sebazar.ir/sebApi/dataDastgah/insertData.php?id=%02d&data1=%c&data2=2019&data3=%s&data4=%s%02d&data5=%02d", Data2, 'M', "salman", "DriverName", 25, Data1);
		
			//strcpy (Buffer_tx,"AT+CMGR=");
			//strcat (Buffer_tx,Temp);
		}
//---Send shop data----------------------------		
//		if(Shop_data_send_flag)
//		{
//			Shop_data_send_flag = 0;
//		
//			Backgrond = 1;
//			LCD_Clear(Good_White);
//			//LCD_MyShowString(0, 48, 300, 16, 16, "Buy", WHITE, MAGENTA);
//			myLCD_goto_xy(135, 40);
//			unicode2ascii((char *)Buy, Buffer_1);
//			myLCD_set_font_color(Brand_Blue);
//			myLCD_font_effect(0);
//			myLCD_putsf_mix((unsigned char*)Buffer_1, 2);
//			
//			//LCD_MyShowString(0, 64, 300, 16, 16, "Buy_amount:", WHITE, MAGENTA);
//			myLCD_goto_xy(133, 90);
//			unicode2ascii((char *)Buy_amount, Buffer_1);
//			myLCD_set_font_color(Brand_Blue);
//			myLCD_font_effect(5);
//			myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
//			
//			Purchase_price_Get();
//			
//			//LCD_MyShowString(0, 96, 300, 16, 16, "Card_pass:", WHITE, MAGENTA);
//			myLCD_goto_xy(132, 140);
//			unicode2ascii((char *)Card_pass, Buffer_1);
//			myLCD_set_font_color(Brand_Blue);
//			myLCD_font_effect(5);
//			myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
//			
//			Card_Pass_Get();
//			
//			sprintf(Date, "%04i/%02i/%02i", Sh_RTC_Date.RTC_Year/*-1300*/, Sh_RTC_Date.RTC_Month, Sh_RTC_Date.RTC_Date);
//			sprintf(Time, "%02i%02i%02i", RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, RTC_TimeStruct.RTC_Seconds);
//			
//			Clear_Buffer(Buffer_1, 512);
//			sprintf(Buffer_1, "http://sebazar.ir/sebApi/dataDastgah/insertTarakonesh.php?card_id=%02x%02x%02x%02x%02x&device_id=%s&date=%s&time=%s&password=%s&purchase_price=%s", Card_ID[0], Card_ID[1], Card_ID[2], Card_ID[3], Card_ID[4], Device_ID, Date, Time, Card_password, Purchase_price);
//			//LCD_ShowStringWithSize(0, 112, 319, 16, 16, Buffer_1, 160);
//			//delay_ms(3000);
//			
//			myLCD_goto_xy(120, 190);
//			unicode2ascii((char *)Data_sending, Buffer_2);
//			myLCD_set_font_color(Brand_Blue);
//			myLCD_font_effect(5);
//			myLCD_putsf_mix((unsigned char*)Buffer_2, 1);
//			
//			TIM_Cmd(TIM8, DISABLE);
//			TIM_Cmd(TIM10, DISABLE);
//			TIM_Cmd(TIM12, DISABLE);
//			HTTP_Upload(Buffer_1);
//			TIM_Cmd(TIM8, ENABLE);
//			TIM_Cmd(TIM10, ENABLE);
//			TIM_Cmd(TIM12, ENABLE);
//			
//			if(HTTP_sucsessful_upload)
//			{
//				LCD_Clear(Good_White);
//				if(Peyment_status == 1)
//				{
//					//LCD_ShowStringWithSize(0, 176, 319, 16, 16, "Successful_transaction", 40);
//					myLCD_goto_xy(95, 90);
//					unicode2ascii((char *)Successful_transaction, Buffer_1);
//					myLCD_set_font_color(Brand_Blue);
//					myLCD_font_effect(5);
//					myLCD_putsf_mix((unsigned char*)Buffer_1, 2);
//					
//					Buzzer_ON
//					delay_ms(300);
//					Buzzer_OFF
//				}
//				if(Peyment_status == 2)
//				{
//					//LCD_ShowStringWithSize(0, 176, 319, 16, 16, "Password_error   ", 40);
//					myLCD_goto_xy(100, 90);
//					unicode2ascii((char *)Password_error, Buffer_1);
//					myLCD_set_font_color(Brand_Blue);
//					myLCD_font_effect(5);
//					myLCD_putsf_mix((unsigned char*)Buffer_1, 2);
//					
//					Buzzer_ON
//					delay_ms(100);
//					Buzzer_OFF
//					delay_ms(50);
//					Buzzer_ON
//					delay_ms(100);
//					Buzzer_OFF
//				}
//				if(Peyment_status == 3)
//				{
//					//LCD_ShowStringWithSize(0, 176, 319, 16, 16, "Low_cash          ", 40);
//					myLCD_goto_xy(95, 90);
//					unicode2ascii((char *)Low_cash, Buffer_1);
//					myLCD_set_font_color(Brand_Blue);
//					myLCD_font_effect(5);
//					myLCD_putsf_mix((unsigned char*)Buffer_1, 2);
//					
//					Buzzer_ON
//					delay_ms(100);
//					Buzzer_OFF
//					delay_ms(50);
//					Buzzer_ON
//					delay_ms(100);
//					Buzzer_OFF
//				}
//				delay_ms(2000);
//				Peyment_status = 0;
//				HTTP_sucsessful_upload = 0;
//			}					
//		}
//---Show signal quality----------------------------		
//		if(Show_signal_quality_flag)
//		{
//			Show_signal_quality_flag = 0;
//			
//			TIM_Cmd(TIM8, DISABLE);
//			TIM_Cmd(TIM10, DISABLE);
//			TIM_Cmd(TIM12, DISABLE);
//			Signal_quality = Get_Signal_Quality();
//			TIM_Cmd(TIM8, ENABLE);
//			TIM_Cmd(TIM10, ENABLE);
//			TIM_Cmd(TIM12, ENABLE);
//			
//			if(Signal_quality >= 2)
//				LCD_Fill(10,20,14,25,Brand_Blue);
//			else
//				LCD_Fill(10,20,14,25,LGRAY);
//			
//			if(Signal_quality >= 12)
//				LCD_Fill(16,16,20,25,Brand_Blue);
//			else
//				LCD_Fill(16,16,20,25,LGRAY);
//			
//			if(Signal_quality >= 18)
//				LCD_Fill(22,12,26,25,Brand_Blue);
//			else
//				LCD_Fill(22,12,26,25,LGRAY);
//			
//			if(Signal_quality >= 24)
//				LCD_Fill(28,8,32,25,Brand_Blue);
//			else
//				LCD_Fill(28,8,32,25,LGRAY);
//			
//			if(Signal_quality >= 30)
//				LCD_Fill(34,4,38,25,Brand_Blue);
//			else
//				LCD_Fill(34,4,38,25,LGRAY);
//		}	
    }
}

//---Clear_Buffer----------------------------
void Clear_Buffer(unsigned char *buffer,unsigned int buffer_size)
{
	unsigned char *Buffer;
	unsigned int k;
	Buffer = buffer;
	for(k=0 ; k<buffer_size ; k++)
	{
		*Buffer = 0;
		Buffer++;
	}
}
//---Read starting data----------------------
void Micro_SD_Write_Starting_Data(void)
{
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 0);
	SD_WriteSingleBlock(Buffer_1, location_data_counter_addr);
	
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 5000);
	SD_WriteSingleBlock(Buffer_1, location_write_block_addr);
	
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 0);
	SD_WriteSingleBlock(Buffer_1, format_data_counter_addr);
	
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 35000);
	SD_WriteSingleBlock(Buffer_1, format_write_block_addr);
	
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 30000);
	SD_WriteSingleBlock(Buffer_1, location_write_block_end_addr);

	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 5000);
	SD_WriteSingleBlock(Buffer_1, location_send_block_addr);
	
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 60000);
	SD_WriteSingleBlock(Buffer_1, format_write_block_end_addr);
	
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 35000);
	SD_WriteSingleBlock(Buffer_1, format_send_block_addr);
}
//---Read starting data----------------------
void Micro_SD_Read_Starting_Data(void)
{
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, device_id_addr);
	//LCD_ShowStringWithSize(0, 0, 319, 16, 16, Buffer_2, 6);
	strcpy (Device_ID, Buffer_2);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, location_data_counter_addr);
	//LCD_ShowStringWithSize(0, 16, 319, 16, 16, Buffer_2, 5);
	Location_data_counter = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, location_write_block_addr);
	//LCD_ShowStringWithSize(0, 32, 319, 16, 16, Buffer_2, 5);
	Location_write_block = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, location_write_block_end_addr);
	//LCD_ShowStringWithSize(0, 48, 319, 16, 16, Buffer_2, 5);
	Location_write_block_end = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, location_send_block_addr);
	//LCD_ShowStringWithSize(0, 64, 319, 16, 16, Buffer_2, 5);
	Location_send_block = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, format_data_counter_addr);
	//LCD_ShowStringWithSize(0, 80, 319, 16, 16, Buffer_2, 5);
	Format_data_counter = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, format_write_block_addr);
	//LCD_ShowStringWithSize(0, 96, 319, 16, 16, Buffer_2, 5);
	Format_write_block = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, format_write_block_end_addr);
	//LCD_ShowStringWithSize(0, 112, 319, 16, 16, Buffer_2, 5);
	Format_write_block_end = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, format_send_block_addr);
	//LCD_ShowStringWithSize(0, 128, 319, 16, 16, Buffer_2, 5);
	Format_send_block = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, trip_price_addr);
	//LCD_ShowStringWithSize(100, 0, 319, 16, 16, Buffer_2, 4);
	Trip_price = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, location_data_time_addr);
	Buffer_2[0] = Buffer_2[1];
	Buffer_2[1] = Buffer_2[2];
	Buffer_2[2] = Buffer_2[3];
	Buffer_2[3] = 0;
	Temp = strtold(Buffer_2,NULL);
//	Clear_Buffer(Buffer_2, 512);
//	sprintf(Buffer_2, "%i", Temp);
//	LCD_ShowStringWithSize(100, 16, 319, 16, 16, Buffer_2, 4);
	if(Temp >= 30)
		Location_data_time = Temp;
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, data_time_addr);
	Buffer_2[0] = Buffer_2[1];
	Buffer_2[1] = Buffer_2[2];
	Buffer_2[2] = Buffer_2[3];
	Buffer_2[3] = 0;
	Temp = strtold(Buffer_2,NULL);
//	Clear_Buffer(Buffer_2, 512);
//	sprintf(Buffer_2, "%i", Temp);
//	LCD_ShowStringWithSize(100, 32, 319, 16, 16, Buffer_2, 4);
	if(Temp >= 60)
		Data_time = Temp;
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, phone_num_2_addr);
	//LCD_ShowStringWithSize(100, 48, 319, 16, 16, Buffer_2, 11);
	strcpy (Phone_Num_2, Buffer_2);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, current_month_addr);
	//LCD_ShowStringWithSize(100, 64, 319, 16, 16, Buffer_2, 2);
	Current_month = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, current_day_addr);
	//LCD_ShowStringWithSize(100, 80, 319, 16, 16, Buffer_2, 2);
	Current_day = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, daily_operation_amount_addr);
	//LCD_ShowStringWithSize(100, 96, 319, 16, 16, Buffer_2, 7);
	Daily_operation_amount = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, front_door_card_addr);
	//LCD_ShowStringWithSize(100, 112, 319, 16, 16, Buffer_2, 4);
	Front_door_card = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, back_door_card_addr);
	//LCD_ShowStringWithSize(100, 128, 319, 16, 16, Buffer_2, 4);
	Back_door_card = strtold(Buffer_2,NULL);
}
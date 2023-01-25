
#include "sim808.h"

#define CtrlZ       26
#define CLEAR       1
#define NOTCLEAR    0
#define NOSIGNAL    0
#define NOTFIXED    1
#define FIXED       2
#define READY       1
#define NOTREADY    0
 
#define   NUM_RIGHT      1
#define   NUM_WRONG      0
#define   SYNTAX_RIGHT   1
#define   SYNTAX_WRONG   0
#define   PASS_RIGHT     1
#define   PASS_WRONG     0

const unsigned char Settings_done[] =
{
    0x2A, 0x06, 0x46, 0x06, 0x38, 0x06, 0xCC, 0x06, 0x45, 0x06, 0x27, 0x06, 0x2A, 0x06, 0x20, 0x00, 
    0x27, 0x06, 0x46, 0x06, 0x2C, 0x06, 0x27, 0x06, 0x45, 0x06, 0x20, 0x00,
	0x34, 0x06, 0x2F, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char Line_name_change[] =
{
    0x46, 0x06, 0x27, 0x06, 0x45, 0x06, 0x20, 0x00, 
	0x2E, 0x06, 0x37, 0x06, 0x20, 0x00, 
    0x2A, 0x06, 0x3A, 0x06, 0xCC, 0x06, 0xCC, 0x06, 0x31, 0x06, 0x20, 0x00,
	0xCC, 0x06, 0x27, 0x06, 0x41, 0x06, 0x2A, 0x06, 0x20, 0x00, 0x00
} ;
const unsigned char URL_settings_done[] =
{
    0x2A, 0x06, 0x46, 0x06, 0x38, 0x06, 0xCC, 0x06, 0x45, 0x06, 0x27, 0x06, 0x2A, 0x06, 0x20, 0x00,
	0x55, 0x00, 0x52, 0x00, 0x4C, 0x00, 0x20, 0x00,
    0x27, 0x06, 0x46, 0x06, 0x2C, 0x06, 0x27, 0x06, 0x45, 0x06, 0x20, 0x00,
	0x34, 0x06, 0x2F, 0x06, 0x20, 0x00, 0x00
} ;
char apn[]="mtnirancell";
char user_name[];
char password[];
unsigned char data_str[250];
unsigned char Data1, Data2, Data3, Data4, Data5, Data6, Data7, Data8;
char count=0;
char result=1,result2=0,result3=0,result4=0,result5=0,result6=0;
unsigned char SMS_received_flag=0;
unsigned char SMS_num[1];
unsigned char SMS_CountryCode[4];
unsigned char SMS_PhoneNumber[12];     /* phone number gui*/
unsigned char SMS_Sender[15];     /* The sender (the name three) */
unsigned char SMS_Date[9];    /* Ngay gui*/
unsigned char SMS_Time[12];    /* gio gui */
unsigned char SMS_Content[200];     /* noi dung tin nhan */
unsigned char SMS_Password[7];
unsigned char Trip_Price[5];
unsigned char Data_Time[4];
unsigned char Location_Data_Time[4];

// GPS Coordinates
unsigned char Latitude[14];  //  Latitude, North (vi do)
unsigned char Longitude[15]; // Longitude, East  (kinh do)
unsigned char Altitude[12];  // Elevation  (do cao)
unsigned char Lat_degree=0;
unsigned char Lat_minute=0;
float Lat_second=0;
unsigned char Lat_dir;
unsigned char Long_degree=0;
unsigned char Long_minute=0;
float Long_second=0;
unsigned char Long_dir;
unsigned char Speed[9];
double dbSpeed;
unsigned char Course[11];

unsigned char UTC_Time[7];
unsigned int dUTC_Time;
unsigned char UTC_Date[9];
unsigned int dUTC_Date;
unsigned char Position_Fix;
unsigned char Satellites_Used[3];

unsigned char Phone_Num_1[12]="09148882278";
unsigned char Phone_Num_2[12];
unsigned char Temp_Phone_Num[12];
unsigned char Password[7]="975310";
unsigned char Command[2];
unsigned char New_Password[7];
unsigned char Temp_Device_ID[7];
unsigned char Reset_Status[2];
unsigned char Line_Code[5];
unsigned char Line_Name[50];
unsigned char phone_num[11]="09396897845";
unsigned char URL_1[100], URL_2[100], URL[50];
int rx_index=0;


unsigned char HTTP_sucsessful_upload = 0;

//---Usart1_Init (void)---------------------------------------
void Usart1_Init (void)
{
    GPIO_InitTypeDef     GPIO_InitStructure;
    USART_InitTypeDef    USART_InitStructure;
    NVIC_InitTypeDef     NVIC_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    
    USART_InitStructure.USART_BaudRate = 57600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(USART1,ENABLE);
    //Usart1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
}

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
void USART1_IRQHandler(void)
{
	unsigned char i;
	static unsigned char j=0;
	/*while*/if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // Received characters modify string
	{	
		//GPIO_SetBits(GPIOD, GPIO_Pin_7);
		
		if (Usart_sec >= 3)
            rx_index = 0;
        
        Buffer_rx[rx_index++] = USART_ReceiveData(USART1);
	}
    Usart_sec=0;
	
	if(check_string2("+CMTI: \"SM\",",NOTCLEAR))
	{
		j++;
		if(j >= 2)
		{
			j = 0;
			SMS_num[0] = Buffer_rx[14];
			SMS_received_flag = 1;
			Clear_UART0Buffer();
		}
	}
	USART_ClearFlag(USART1, USART_IT_RXNE);
	
	//GPIO_ResetBits(GPIOD, GPIO_Pin_7);
}

//---Usart_Send(unsigned char Ch)--------------------------
void Usart_Send(unsigned char Ch)
{
    USART_SendData(USART1,Ch);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
}

//---Usart_Send_Str(unsigned char *Data, unsigned char Data_len)----
void Usart_Send_Str(unsigned char *Data, unsigned int Data_len)
{
    int i;
    for(i=0; i<Data_len; i++)
    {
        USART_SendData(USART1,Data[i]);
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
        
    }
}

//******************************************************************
//
//******************************************************************
void TIM4_Int_Init(u16 Arr,u16 Psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	
	
	TIM_TimeBaseStructure.TIM_Period = Arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =Psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
 
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM4, ENABLE);  
}

void TIM4_IRQHandler(void)   
{
    unsigned char *Buffer;
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  
 		//GPIO_ToggleBits(GPIOC, GPIO_Pin_0);
        
        if(Show_sim808)
        {    
            Show_sim808=0;
//            LCD_Clear(MAGENTA); 
//            LCD_ShowString(0, 16, 220, 16, 16, SMS_CountryCode);
//            LCD_ShowString(0, 32, 220, 16, 16, SMS_PhoneNumber);
//            LCD_ShowString(0, 48, 220, 16, 16, SMS_Sender);
//            LCD_ShowString(0, 64, 220, 16, 16, SMS_Date);
//            LCD_ShowString(0, 80, 220, 16, 16, SMS_Time);
//            LCD_ShowString(0, 96, 220, 16, 16, SMS_Content);
//            LCD_ShowString(0, 112, 220, 16, 16, SMS_Password);
//            LCD_ShowString(0, 128, 220, 16, 16, SMS_Command);
//			LCD_ShowStringWithSize(0, 144, 300, 16, 16, Buffer_rx, 200);
        }
        
        if(check_string2("+CMTI: \"SM\",",NOTCLEAR))
        {    
//            if(Buffer_rx[15] >= 0x30 && Buffer_rx[15] <= 0x39)
//                SMS_num=((Buffer_rx[14]-0x30)*10)+(Buffer_rx[15]-0x30);
//            else
//                SMS_num=Buffer_rx[14]-0x30;
            
            //sprintf(Buffer_1, "%u ", SMS_num);
            //LCD_ShowString(0, 172, 200, 16, 16, Buffer_1);
        }
        
        if(check_string2("RING",NOTCLEAR))
        {    
            LCD_ShowString(10, 156, 200, 16, 16, "RING");
        }
	}
}

/*******************************************************************/
void Blink_1(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_7);
	delay_ms(100);
	GPIO_ResetBits(GPIOD, GPIO_Pin_7);
}

/*******************************************************************/
void Blink_2(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_7);
	delay_ms(50);
	GPIO_ResetBits(GPIOD, GPIO_Pin_7);
	delay_ms(50);
	GPIO_SetBits(GPIOD, GPIO_Pin_7);
	delay_ms(50);
	GPIO_ResetBits(GPIOD, GPIO_Pin_7);
}
/*******************************************************************
** Function name:Sim800_PowerOnOff
**
** Descriptions:
**
** parameters:None
** Returned value:None
**
*****************************************************************************/
void Sim808_PowerOnOff(void)
{
	//LCD_Clear(WHITE);
	//LCD_ShowString(0, 0, 200, 16, 16, "SIM808 is starting...          ");
	GPIO_SetBits(GPIOG, GPIO_Pin_3);
	delay_ms(1000);
	GPIO_ResetBits(GPIOG, GPIO_Pin_3);
	//LCD_ShowString(0, 16, 200, 16, 16, "SIM808 is ON              ");
	delay_ms(3000);
}
/*****************************************************************************
** Function name:Sim800_Init
**
** Descriptions:
**
** parameters:None
** Returned value:None
**
*****************************************************************************/
void Sim808_Init(void)
{
	unsigned char i=0,j=0;
    
    //Usart1_Init ();
    //TIM4_Int_Init(199,41999);
    //Clear_UART0Buffer();
	
	do
	{
		Usart_Send_Str("AT\r", 3);
        delay_ms(200);
		//LCD_ShowStringWithSize(0, 32, 300, 16, 16, Buffer_rx, 50);
	}while(!check_string("OK",CLEAR));
    
	do
	{
		Usart_Send_Str("ATZ\r", 4);
		delay_ms(200);
		//LCD_ShowStringWithSize(0, 32, 300, 16, 16, Buffer_rx, 50);
	}while(!check_string("OK",CLEAR));
    
	do
	{
		Usart_Send_Str("ATE0\r", 5); //echo mode: off
		delay_ms(200);
		//LCD_ShowStringWithSize(0, 32, 300, 16, 16, Buffer_rx, 40);
	}while(!check_string("OK",CLEAR));
	
	do
	{ 
		Usart_Send_Str("AT+IPR=57600\r", 13); //Set TE-TA fixed local rate :57600bps
		delay_ms(200);
		//LCD_ShowStringWithSize(0, 32, 300, 16, 16, Buffer_rx, 40);
	}while(!check_string("OK",CLEAR));
	
//	do
//	{
//		Usart_Send_Str("AT+CGPSIPR=57600\r", 17); //Set TE-TA fixed local rate (for GPS) :57600bps
//		delay_ms(200);
//		LCD_ShowStringWithSize(0, 32, 300, 16, 16, Buffer_rx, 40);
//	}while(!check_string("OK",CLEAR));
	
	j=0;
	for(i=0;i<5;i++)
	{ 
		Usart_Send_Str("AT+CPIN?\r", 9); //check sim card
		delay_ms(500); 
		//LCD_ShowStringWithSize(0, 32, 300, 16, 16, Buffer_rx, 50);
		if(!check_string("READY",CLEAR)) 
			j++;
		else 
			break;
	}
	if (j==5)
	{
		LCD_ShowString(0, 30, 120, 16, 16, "Insert SIM card");
		while(1); 
	}
	
//	do
//	{ 
//		Usart_Send_Str("AT+ICF?\r", 8); //8 data bit, 1 stop bit,no parity
//		delay_ms(1000);
//		LCD_ShowStringWithSize(0, 32, 300, 16, 16, Buffer_rx, 40);
//		delay_ms(500);
//	}while(!check_string("OK",CLEAR));
	
//	do
//	{ 
//		Usart_Send_Str("AT+ICF=3,3\r", 11); //8 data bit, 1 stop bit,no parity
//		delay_ms(200);
//		LCD_ShowStringWithSize(0, 32, 300, 16, 16, Buffer_rx, 40);
//	}while(!check_string("OK",CLEAR));
	
// 	do
// 	{ 
// 		Usart_Send_Str("AT+CRES", 7); //restore sms settings
// 		delay_ms(200);
//         
//         Lcd_Show();
// 	}while(!check_string("OK",CLEAR));
 	
	do
	{
		Usart_Send_Str("AT+CMGF=1\r", 10); //set SMS system into text mode
		delay_ms(200);
		//LCD_ShowStringWithSize(0, 32, 300, 16, 16, Buffer_rx, 40);
	}while(!check_string("OK",CLEAR));
 	
	do
	{ 
		Usart_Send_Str("AT+CSCS=\"GSM\"\r", 16); //GSM 7 bits default alphabet
		delay_ms(200);
		//LCD_ShowStringWithSize(0, 32, 300, 16, 16, Buffer_rx, 40);
	}while(!check_string("OK",CLEAR));
 	
	do
	{
		Usart_Send_Str("AT+CNMI=2,0,0,0,0\r", 18); //New message indication enable.
		delay_ms(200);
		//LCD_ShowStringWithSize(0, 32, 300, 16, 16, Buffer_rx, 40);
	}while(!check_string("OK",CLEAR));
	 
	do
	{
		Usart_Send_Str("AT+CNMI=1,1,0,0,0\r", 18); //return the position save massages 
		delay_ms(200);
		//LCD_ShowStringWithSize(0, 32, 300, 16, 16, Buffer_rx, 40);
	}while(!check_string("OK",CLEAR));
	
//	do
//	{
//		Usart_Send_Str("AT+CNMI=1,2,0,0,0\r", 18); //return the position save massages 
//		delay_ms(200);
//		//LCD_ShowStringWithSize(0, 32, 300, 16, 16, Buffer_rx, 40);
//	}while(!check_string("OK",CLEAR));
	
	do
	{
		Usart_Send_Str("AT+CSAS\r", 8); //save configuration sms
		delay_ms(200);
		//LCD_ShowStringWithSize(0, 32, 300, 16, 16, Buffer_rx, 40);
	}while(!check_string("OK",CLEAR));
	
	SMS_Delete(DEL_ALL);

// 	do{
// 		 UARTSend(_LPC_UART, "AT+CREG=5\r", 9);
// 	   delay(2000000);
// 		 UART_Send(_LPC_UART2, rb.rx, 150, BLOCKING);
//   }while(!check_string("OK",CLEAR));
	
	do
	{
		Usart_Send_Str("AT+CREG?\r", 9);
		delay_ms(2000);
		//LCD_ShowStringWithSize(0, 48, 300, 16, 16, Buffer_rx, 40);
	}while(!check_string("+CREG: 0,1",CLEAR));
	
	//signal quality report
//	do
//	{
//		Usart_Send_Str("AT+CSQ\r", 7);
//		delay_ms(200);
//		LCD_ShowStringWithSize(0, 64, 300, 16, 16, Buffer_rx, 40);
//		delay_ms(5000);
//	}while(!check_string("OK",CLEAR));
	
	do
	{ 
		Usart_Send_Str("AT+CGPSPWR=1\r", 13); //Turn on GPS power supply
		delay_ms(200);
	}while(!check_string("OK",CLEAR)); 
	
	do
	{
		Usart_Send_Str("AT+CGPSRST=1\r", 13); //reset GPS in Hot mode
		delay_ms(200);
	}while(!check_string("OK",CLEAR)); 
	
//	do
//	{ 
//		Usart_Send_Str("AT+CGPSSTATUS?\r", 15); 
//		delay_ms(1000);
//		//LCD_ShowStringWithSize(0, 128, 300, 16, 16, Buffer_rx, 40);
//	}while(!check_string("+CGPSSTATUS: Location 3D Fix",CLEAR));
	
//	do
//	{ 
//		Usart_Send_Str("AT+CGPSOUT=8\r", 15); 
//		delay_ms(200);
//		//LCD_ShowStringWithSize(0, 16, 300, 16, 16, Buffer_rx, 40);
//	}while(!check_string("OK",CLEAR));
	
//	do
//	{
//		Usart_Send_Str("AT+CGPSINF=2\r", 13);
//		delay_ms(200);
//		LCD_ShowStringWithSize(0, 112, 319, 16, 16, Buffer_rx, 120);
//		delay_ms(1000);
//		Clear_UART0Buffer();
//	}while(1);
	
	Clear_UART0Buffer();
	
    //SMS_Send("09141704476","Hi. We can. This SMS sent from Alarm System.");
    //SMS_Send("09148882278","Hi. We can. This SMS sent from Bus Meter.");
    

	//LCD_ShowString(0, 32, 120, 16, 16, "SIM808 Configed");
    

	HTTP_Config();
//	Blink_1();
//	while(1)
//	{
//		Data1 = 1;
//		Data2 = 2;
//		Data3 = 4;
//		if(Data1 > 30)
//			Data1 = 1;
//		
//		if(Data2 > 30)
//			Data2 = 2;
//		
//		//snprintf(data_str, sizeof(data_str), "http://icai.ir/data.php?data=insert?%s?salman?2019/7/6?%s?%s", data1, data2, data3);
//		sprintf(data_str, "http://sebazar.ir/sebApi/dataDastgah/insertData.php?id=%05d&B=%s&C=%s&id=%05d&A=%d&id=%05d&B=%s", Data1, "salman", "DriverName", Data2, Data3, Data3, "salman");
//		HTTP_Upload(data_str);
//		//LCD_ShowStringWithSize(0, 80, 319, 16, 16, Buffer_SMS, 120);
//		Blink_2();
//	}
	
//	while(1)
//	{
//		for(i=3;i<30;i++)
//		{
//			SMS_Read(i);
//			Show_sim808=1;
//			delay_ms(1000);
//		}
//	} 
}

//---Get_GSM_Signal_Quality-------------------------------
void Get_GSM_Signal_Quality(void)
{
	unsigned char Buffer[3], j=0;
	
	//TIM_Cmd(TIM8, DISABLE);
	//TIM_Cmd(TIM10, DISABLE);
	//TIM_Cmd(TIM12, DISABLE);
	do
	{
		Clear_Buffer(Buffer_rx, 500);
		rx_index=0;
		Usart_Send_Str("AT+CSQ\r", 7);
		delay_ms(200);
		Buffer[0] = Buffer_rx[8];
		Buffer[1] = Buffer_rx[9];
		Buffer[2] = 0;
		
		//LCD_ShowStringWithSize(0, 100, 300, 16, 16, Buffer_rx, 40);
		
		if(check_string("+CSQ: ",CLEAR))
		{
			GSM_Signal_quality = strtold(Buffer,NULL);
			
			if(GSM_Signal_quality >= 2)
				LCD_Fill(10,20,14,25,Brand_Blue);
			else
				LCD_Fill(10,20,14,25,LGRAY);
			
			if(GSM_Signal_quality >= 12)
				LCD_Fill(16,16,20,25,Brand_Blue);
			else
				LCD_Fill(16,16,20,25,LGRAY);
			
			if(GSM_Signal_quality >= 18)
				LCD_Fill(22,12,26,25,Brand_Blue);
			else
				LCD_Fill(22,12,26,25,LGRAY);
			
			if(GSM_Signal_quality >= 24)
				LCD_Fill(28,8,32,25,Brand_Blue);
			else
				LCD_Fill(28,8,32,25,LGRAY);
			
			if(GSM_Signal_quality >= 30)
				LCD_Fill(34,4,38,25,Brand_Blue);
			else
				LCD_Fill(34,4,38,25,LGRAY);
			
			break;
		}
		j++;
	}while(j<3);

	//TIM_Cmd(TIM8, ENABLE);
	//TIM_Cmd(TIM10, ENABLE);
	//TIM_Cmd(TIM12, ENABLE);
}
//---Get_GPS_Signal_Quality-------------------------------
unsigned char Get_GPS_Signal_Quality(void)
{
	unsigned char j=0;
	do
	{ 
		Usart_Send_Str("AT+CGPSSTATUS?\r", 15); 
		delay_ms(200);
		//LCD_ShowStringWithSize(0, 128, 300, 16, 16, Buffer_rx, 40);
		if(check_string("+CGPSSTATUS: Location 3D Fix",CLEAR))
		{
			return 1;
		}	
		j++;
	}while(j<3);
	
	return 0;
}
/*****************************************************************************
** Function name:SMS_Send
**
** Descriptions:gui tin nhan
**Ex:   SMS_Send(0974925676,"HAHA.GOT IT..");
** parameters:None
** Returned value:
 
*****************************************************************************/
unsigned char SMS_Send(unsigned char *Phone_num, unsigned char *Content)
{
	//sprintf (Buffer_tx,"AT+CMGS=\"+98%d%d\"\r",Phone_num_part1,Phone_num_part2);
    strcpy (Buffer_tx,"AT+CMGS=\"");
	strcat (Buffer_tx,Phone_num);
	strcat (Buffer_tx,"\"\r");
    
	Clear_UART0Buffer();
	
	do
	{
		Usart_Send_Str(Buffer_tx, sizeof(Buffer_tx));
		delay_ms(100);
	}while(!check_string(">",CLEAR));
	
	Usart_Send_Str(Content, 44);
	delay_ms(200);	
	
	Usart_Send(CtrlZ);
	delay_ms(3000);
	if (check_string("OK",CLEAR))
	{
		sprintf(Buffer_1, "Message Sent");
		LCD_ShowString(0, 30, 120, 16, 16, Buffer_1);
		return 1; 
	}
	else 
		return 0;
	 
}
/*****************************************************************************
** Function name:SMS_Read
**
** parameters:doc tin nhan o vi tri index (index<100)
** Returned value:none
*****************************************************************************/
void SMS_Read(unsigned char *Index)
{
//	unsigned char Temp[2];
//	 
//	sprintf(Temp,"%d",Index);
//	 
//	strcpy (Buffer_tx,"AT+CMGR=");
//	strcat (Buffer_tx,Index);
//	strcat (Buffer_tx,"\r");
//	 
//	Clear_UART0Buffer();
//	Usart_Send_Str(Buffer_tx, 10);
    
	Clear_Buffer(Buffer_rx, 500);
	rx_index = 0;
	Usart_Send_Str("AT+CMGR=1\r", 10);
	
	delay_ms(1000);
	
//	LCD_ShowStringWithSize(0, 100, 319, 16, 16, Buffer_rx, 200);
//	delay_ms(4000);
	
	SMS_Receive();
}
 
/*****************************************************************************
** Function name:SMS_Receive
**
** Descriptions:xu li tin nhan SMS
**Frame: \r\n+CMT: "+84974925676","Duc","12/10/05,20:34:26+28"\r\n<NoiDungTinNhan>\r\n
hoac: \r\n+CMGR: "REC UNREAD","+84974925676","Duc","12/10/05,20:34:26+28"\r\n<NoiDungTinNhan>\r\n
** parameters:None
** Returned value:
*****************************************************************************/
void SMS_Receive(void)
{
	unsigned char i=0, j=0, S1, S2; 
	unsigned char *Buffer;
	unsigned char Temp_Buffer[4];
	
	Buffer = Buffer_rx;
	 
	//tim dau " thu 3
	for(i=0; i<3; i++)
	{
		while(*Buffer++ != '\"'); 
	}
	  
//---SMS_CountryCode-
	Clear_Buffer(SMS_CountryCode, 3);
    for(i=0;i<3;i++)
	{
		SMS_CountryCode[i] = *Buffer++;
	}
//---SMS_PhoneNumber-
	Clear_Buffer(SMS_PhoneNumber, 12);
    SMS_PhoneNumber[0]='0';
	i=1;
	while (*Buffer != '\"')
	{
		SMS_PhoneNumber[i++] = *Buffer++; 
	}
	while (*++Buffer != '\"');
//---SMS_Sender---
	i=0;
	while (*++Buffer != '\"')
	{
		SMS_Sender[i++] = *Buffer;  
	}
	while (*++Buffer != '\"');
//---SMS_Date-----
	i=0;
	while (*++Buffer != '\,')   
	{
		SMS_Date[i++] = *Buffer; 
	} 
//---SMS_Time-----
	i=0;
	while (*++Buffer != '\"')
	{
		SMS_Time[i++] = *Buffer;
	}
	  
	while (*++Buffer != '\n');  //bo qua ki tu ko can thiet   
	 
	S1 = strcmp(Phone_Num_1, SMS_PhoneNumber);
	S2 = strcmp(Phone_Num_2, SMS_PhoneNumber);
    if(S1 == 0 || S2 == 0)
	{
//---SMS_Content--
		Clear_Buffer(SMS_Content, 200);
		i=0;
		while (*++Buffer != '\r')
		{
			SMS_Content[i++] = *Buffer; 
		}
		
		Buffer = SMS_Content;
		
		if(*Buffer =='0' && *(Buffer+1) =='0' && *(Buffer+4) =='0' && *(Buffer+5) =='6')
		{
			Clear_Buffer(Line_Name, 50);
			i=0;
			while(!(*Buffer =='0' && *(Buffer+1) =='0' && *(Buffer+2) =='2' && *(Buffer+3) =='2' && *(Buffer+4) =='0' && *(Buffer+5) =='0' && *(Buffer+6) =='2' && *(Buffer+7) =='C'))
			{
				for(j=0;j<4;j++)
				{
					if(*(Buffer+j) == '0')
						Temp_Buffer[j] = 0;
					else if(*(Buffer+j) == '1')
						Temp_Buffer[j] = 1;
					else if(*(Buffer+j) == '2')
						Temp_Buffer[j] = 2;
					else if(*(Buffer+j) == '3')
						Temp_Buffer[j] = 3;
					else if(*(Buffer+j) == '4')
						Temp_Buffer[j] = 4;
					else if(*(Buffer+j) == '5')
						Temp_Buffer[j] = 5;
					else if(*(Buffer+j) == '6')
						Temp_Buffer[j] = 6;
					else if(*(Buffer+j) == '7')
						Temp_Buffer[j] = 7;
					else if(*(Buffer+j) == '8')
						Temp_Buffer[j] = 8;
					else if(*(Buffer+j) == '9')
						Temp_Buffer[j] = 9;
					else if(*(Buffer+j) == 'A')
						Temp_Buffer[j] = 10;
					else if(*(Buffer+j) == 'B')
						Temp_Buffer[j] = 11;
					else if(*(Buffer+j) == 'C')
						Temp_Buffer[j] = 12;
					else if(*(Buffer+j) == 'D')
						Temp_Buffer[j] = 13;
					else if(*(Buffer+j) == 'E')
						Temp_Buffer[j] = 14;
					else if(*(Buffer+j) == 'F')
						Temp_Buffer[j] = 15;
				}
				
				Line_Name[i++] = (Temp_Buffer[2] * 16) + Temp_Buffer[3];
				Line_Name[i++] = (Temp_Buffer[0] * 16) + Temp_Buffer[1];
				Buffer += 4;
			}
			
			SD_WriteSingleBlock(Line_Name, line_name_addr);
			
			Backgrond = 1;
			LCD_Clear(Good_White);
			
			myLCD_set_font_color(Brand_Blue);
			myLCD_font_effect(5);
			myLCD_goto_xy(70, 100);
			unicode2ascii((char *)Line_name_change, Buffer_1);
			myLCD_putsf_mix((unsigned char*)Buffer_1, 2);
			Buzzer_ON
			delay_ms(100);
			Buzzer_OFF
			delay_ms(50);
			Buzzer_ON
			delay_ms(300);
			Buzzer_OFF
			delay_ms(2000);
		}
		else
		{
//---SMS_Password-	
			Clear_Buffer(SMS_Password, 7);
			i=0;
			while (!(*Buffer == '\"' && *(Buffer+1) == '\,'))//for(i=0;i<6;i++)
				SMS_Password[i++] = *Buffer++;
				
			Buffer+=3;
//---Command-	
			Clear_Buffer(Command, 2);
			i=0;
			while (!(*Buffer == '\"' && *(Buffer+1) == '\,'))//for(i=0;i<6;i++)
				Command[i++] = *Buffer++;
				
			Buffer+=3;
			
			if(strtold(Command,NULL) == 0)
			{
//---New_Password-	
				Clear_Buffer(New_Password, 7);
				i=0;
				while (!(*Buffer == '\"' && *(Buffer+1) == '\,'))//for(i=0;i<6;i++)
					New_Password[i++] = *Buffer++;
					
				Buffer+=3;
//---Reset_Status------ 
				Clear_Buffer(Reset_Status, 2);
				i=0;
				while (!(*Buffer == '\"' && *(Buffer+1) == '\,'))//for(i=0;i<6;i++)
					Reset_Status[i++] = *Buffer++;
				
				Buffer+=3;
//---Device_ID------ 
				Clear_Buffer(Temp_Device_ID, 7);
				i=0;
				while (!(*Buffer == '\"' && *(Buffer+1) == '\,'))//for(i=0;i<6;i++)
					Temp_Device_ID[i++] = *Buffer++;
				
				Buffer+=3;
//---Line_Name------ 
				Clear_Buffer(Line_Code, 5);
				i=0;
				while (!(*Buffer == '\"' && *(Buffer+1) == '\,'))//for(i=0;i<6;i++)
					Line_Code[i++] = *Buffer++;
				
				Buffer+=3;
//---Trip_Price------ 
				Clear_Buffer(Trip_Price, 5);
				i=0;
				while (!(*Buffer == '\"' && *(Buffer+1) == '\,'))//for(i=0;i<4;i++)
					Trip_Price[i++] = *Buffer++;

				Buffer+=3;
//---Data_Time------ 
				Clear_Buffer(Data_Time, 4);
				i=0;
				while (!(*Buffer == '\"' && *(Buffer+1) == '\,'))//for(i=0;i<3;i++)
					Data_Time[i++] = *Buffer++;

				Buffer+=3;
//---Location_Data_Time------ 
				Clear_Buffer(Location_Data_Time, 4);
				i=0;
				while (!(*Buffer == '\"' && *(Buffer+1) == '\,'))//for(i=0;i<3;i++)
					Location_Data_Time[i++] = *Buffer++;
				
				Buffer+=3;
//---Phone_Num------ 
				Clear_Buffer(Temp_Phone_Num, 12);
				i=0;
				while (!(*Buffer == '\"' && *(Buffer+1) == '\,'))//for(i=0;i<11;i++)
					Temp_Phone_Num[i++] = *Buffer++;
				
//				sprintf(Buffer_1, "Phone_Num=%s,Password=%s,Command=%s,New_Password=%s,Reset_ststus=%s,Device_ID=%s,Line_Code=%s,Trip_Price=%s,All_Data_Time=%s,Location_Data_Time=%s,Phone_Num_2=%s", SMS_PhoneNumber, SMS_Password,Command, New_Password, Reset_Status, Device_ID, Line_Code, Trip_Price, Data_Time, Location_Data_Time, Temp_Phone_Num);
//				LCD_ShowStringWithSize(0, 100, 319, 16, 16, Buffer_1, 200);
				
				S1 = strcmp(Password, SMS_Password);
				if(S1 == 0)
				{
					if(strtold(Reset_Status,NULL) == 1)
						Micro_SD_Write_Starting_Data();
					
					strcpy(Device_ID, Temp_Device_ID);
					SD_WriteSingleBlock(Device_ID, device_id_addr);
					
					SD_WriteSingleBlock(Line_Code, line_code_addr);
					
					Trip_price = strtold(Trip_Price,NULL);
					SD_WriteSingleBlock(Trip_Price, trip_price_addr);
					
					Data_time = strtold(Data_Time,NULL);
					SD_WriteSingleBlock(Data_Time, data_time_addr);
					
					Location_data_time = strtold(Location_Data_Time,NULL);
					SD_WriteSingleBlock(Location_Data_Time, location_data_time_addr);
					
					if(strtold(Temp_Phone_Num,NULL) != 0)
					{
						strcpy(Phone_Num_2, Temp_Phone_Num);
						SD_WriteSingleBlock(Phone_Num_2, phone_num_2_addr);
					}
					
					Backgrond = 1;
					LCD_Clear(Good_White);
					
					myLCD_set_font_color(Brand_Blue);
					myLCD_font_effect(5);
					myLCD_goto_xy(70, 100);
					unicode2ascii((char *)Settings_done, Buffer_1);
					myLCD_putsf_mix((unsigned char*)Buffer_1, 2);
//					LCD_ShowStringWithSize(100, 32, 319, 16, 16, "Settings_done", 13);
					Buzzer_ON
					delay_ms(100);
					Buzzer_OFF
					delay_ms(50);
					Buzzer_ON
					delay_ms(300);
					Buzzer_OFF
					delay_ms(2000);
		//			sprintf(Buffer_1, "Phone_Num=%s,Password=%s,New_Password=%s,Reset_ststus=%s,Device_ID=%s,Line_name=%s,Trip_Price=%s,All_Data_Time=%s,Location_Data_Time=%s,Phone_Num_2=%s,URL_1=%s,URL_2=%s", SMS_PhoneNumber, SMS_Password, New_Password, Reset_Status, Device_ID, Line_Name, Trip_Price, Data_Time, Location_Data_Time, Phone_Num_2, URL_1, URL_2);
		//			LCD_ShowStringWithSize(0, 100, 319, 16, 16, Buffer_1, 270);
				}
			}
			else if(strtold(Command,NULL) == 1)
			{
//---URL_1------ 
				Clear_Buffer(URL_1, 100);
				i=0;
				while (!(*Buffer == '\"' && *(Buffer+1) == '\,'))//for(i=0;i<11;i++)
					URL_1[i++] = *Buffer++;
				
				Buffer+=3;
//---URL_2------ 
				Clear_Buffer(URL_2, 100);
				i=0;
				while (!(*Buffer == '\"' && *(Buffer+1) == '\,'))//for(i=0;i<11;i++)
					URL_2[i++] = *Buffer++;
				
				SD_WriteSingleBlock(URL_1, url_1_addr);
				SD_WriteSingleBlock(URL_2, url_2_addr);
				
				Backgrond = 1;
				LCD_Clear(Good_White);
				
				myLCD_set_font_color(Brand_Blue);
				myLCD_font_effect(5);
				myLCD_goto_xy(45, 100);
				unicode2ascii((char *)URL_settings_done, Buffer_1);
				myLCD_putsf_mix((unsigned char*)Buffer_1, 2);
				Buzzer_ON
				delay_ms(100);
				Buzzer_OFF
				delay_ms(50);
				Buzzer_ON
				delay_ms(300);
				Buzzer_OFF
				delay_ms(2000);
//				sprintf(Buffer_1, "Phone_Num=%s,Password=%s,Command=%s,URL_1=%s,URL_2=%s", SMS_PhoneNumber, SMS_Password,Command, URL_1, URL_2);
//				LCD_ShowStringWithSize(0, 100, 319, 16, 16, Buffer_1, 200);
			}
		}
		
	}
}
//***********************************************************************
void SMS_Delete(unsigned char Type)
{
 
	do
	{
		switch (Type)
		{ 
			case DEL_READ:
				Usart_Send_Str("AT+CMGDA=\"DEL READ\"\r", 22);
				break;  //delete all read messages
			
			case DEL_UNREAD: 
				Usart_Send_Str("AT+CMGDA=\"DEL UNREAD\"\r", 24);
				break;  //delete all unread messages
			
			case DEL_SENT: 
				Usart_Send_Str("AT+CMGDA=\"DEL SENT\"\r", 22);
				break;  //delete all sent messages
			
			case DEL_UNSENT:
				Usart_Send_Str("AT+CMGDA=\"DEL UNSENT\"\r", 24); 
				break;  //delete all unsent messages
			
			case DEL_INBOX: 
				Usart_Send_Str("AT+CMGDA=\"DEL INBOX\"\r", 23);
				break; //delete all received messages
			
			case DEL_ALL: 
				Usart_Send_Str("AT+CMGDA=\"DEL ALL\"\r", 21);
				break;  //delete all SMS
			
			default: 
				break;
		}
		delay_ms(1000);
	}while(!check_string("OK",CLEAR)); //CMGS: 0\r\n\r\nOK
}
 
//***********************************************************************
void Clear_UART0Buffer(void)
{
	unsigned char *Buffer;
	int k;
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	Buffer = Buffer_rx;
	for(k=0;k<500;k++)
	{
		*Buffer = 0;
		Buffer++;
	}
	rx_index = 0;
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void Fill_UART0Buffer(void)
{
 
	unsigned char *Buffer;
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	Buffer = Buffer_rx;
	while(*Buffer)
	{
	 
		*Buffer = 0x01;
		Buffer++;
	  
	}
	*Buffer = 0x01;
	rx_index = 0;
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
 
}

//***********************************************************************
unsigned char check_string(const char *str,unsigned char Clear)
{
    char *S;
    
    S = strstr(Buffer_rx,str);
    
    if(S != '\0')
	{ 
		if (Clear) 
			Clear_UART0Buffer();
		return 1;  
    }			
    else
	{
		Fill_UART0Buffer();
		S = strstr(Buffer_rx,str);			
		if (Clear) 
			Clear_UART0Buffer();
		if(S != '\0')
		    return 1;
		else
		    return 0;  
    }
}

unsigned char check_string2(const char *str,unsigned char Clear)
{
    char *S; 
    S = strstr(Buffer_rx,str);
    
    if(S != '\0')
	{ 
		if (Clear) 
			Clear_UART0Buffer();
		return 1;  
    }			
    else
	{
		return 0;  
    }
}
//********************************************************
void HTTP_Config(void)
{
//	do
//	{
//		Usart_Send_Str("AT+CREG?\r", 9);
//		delay_ms(200);
//		LCD_ShowStringWithSize(0, 48, 300, 16, 16, Buffer_rx, 40);
//	}while(!check_string("+CREG: 0,1",CLEAR));
	
	do
	{
		Usart_Send_Str("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r", 34);
		delay_ms(200);
	}while(!check_string("OK",CLEAR));
	
	snprintf(Buffer_tx, sizeof(Buffer_tx), "AT+SAPBR=3,1,\"APN\",\"%s\"\r", apn);
	do
	{
		Usart_Send_Str(Buffer_tx, sizeof(Buffer_tx));
		delay_ms(200);
	}while(!check_string("OK",CLEAR));
	
//	snprintf(Buffer_tx, sizeof(Buffer_tx), "AT+SAPBR=3,1,\"USER\",\"%s\"\r", user_name);
//	do
//	{
//		Usart_Send_Str(Buffer_tx, sizeof(Buffer_tx));
//		delay_ms(200);
//	}while(!check_string("OK",CLEAR));
//	
//	snprintf(Buffer_tx, sizeof(Buffer_tx), "AT+SAPBR=3,1,\"PWD\",\"%s\"\r", password);
//	do
//	{
//		Usart_Send_Str(Buffer_tx, sizeof(Buffer_tx));
//		delay_ms(200);
//	}while(!check_string("OK",CLEAR));
	
	do
	{
		Usart_Send_Str("AT+SAPBR=1,1\r", 13);
		delay_ms(200);
	}while(!check_string("OK",CLEAR));
	
	do
	{
		Usart_Send_Str("AT+SAPBR=2,1\r", 13);
		delay_ms(200);
		//LCD_ShowStringWithSize(0, 64, 300, 16, 16, Buffer_rx, 40);
	}while(!check_string("OK",CLEAR));
	
//	do
//	{
//		Usart_Send_Str("AT+SAPBR=0,1\r", 13);
//		delay_ms(200);
//	}while(!check_string("OK",CLEAR));
}
//***********************************************************************
//
//
//***********************************************************************
void HTTP_Upload(unsigned char *data_str)
{
	unsigned char i, j, S;
	
	Data_Icon_Up(1);
	
	Usart_Send_Str("AT+HTTPINIT\r", 12);
	delay_ms(200);
	if(check_string("OK",CLEAR))
	{
		Usart_Send_Str("AT+HTTPPARA=\"CID\",1\r", 22);
		delay_ms(200);
		if(check_string("OK",CLEAR))
		{
			Clear_Buffer(Buffer_tx, 500);
			snprintf(Buffer_tx, sizeof(Buffer_tx), "AT+HTTPPARA=\"URL\",\"%s\"\r", data_str);
			//LCD_ShowStringWithSize(0, 132, 319, 16, 16, Buffer_tx, 199);
			Usart_Send_Str(Buffer_tx, sizeof(Buffer_tx));
			delay_ms(200);	
			if(check_string("OK",CLEAR))
			{
				Usart_Send_Str("AT+HTTPACTION=0\r", 16);                                                  
				delay_ms(2000);
				//LCD_ShowStringWithSize(0, 32, 319, 16, 16, Buffer_rx, 40);
				if(check_string("+HTTPACTION: 0,200,",CLEAR))
				{
					Data_Icon_Down(1);
					Usart_Send_Str("AT+HTTPREAD=1,200\r", 18);
					delay_ms(300);
					//LCD_ShowStringWithSize(0, 100, 319, 16, 16, Buffer_rx, 80);
					
					if(Device_mode == 0)
					{
						Clear_Buffer(Buffer_2, 512);
						sprintf(Buffer_2, "  +HTTPREAD: 28  %05s:true ", Sending_id[0]);
						//LCD_ShowStringWithSize(0, 144, 319, 16, 16, Buffer_2, 80);
						
						S = 0;
						for(i=17;i<27;i++)
							if(Buffer_2[i] != Buffer_rx[i])
								S = 1;
						
						if(S == 0)
							HTTP_sucsessful_upload = 1;
					}
					else if(Device_mode == 2)
					{
						Clear_Buffer(Buffer_2, 512);
						sprintf(Buffer_2, "  +HTTPREAD: 48  %02x%02x%02x%02x%02x:%06s:%s:  OK", Card_ID[0], Card_ID[1], Card_ID[2], Card_ID[3], Card_ID[4], Device_ID, Date);
						//LCD_ShowStringWithSize(0, 144, 319, 16, 16, Buffer_2, 80);
						//delay_ms(3000);
						
						S = 0;
						for(i=17;i<46;i++)
							if(Buffer_2[i] != Buffer_rx[i])
								S = 1;
							
						if(S == 0)
						{
							if(Buffer_rx[46] == 's')
							{
								HTTP_sucsessful_upload = 1;
								Peyment_status = 1;
							}
							else if(Buffer_rx[46] == 'e')
							{
								HTTP_sucsessful_upload = 1;
								Peyment_status = 2;
							}
							else if(Buffer_rx[46] == 'l')
							{
								HTTP_sucsessful_upload = 1;
								Peyment_status = 3;
							}
						}
					}
				}
			}
		}
	}
	
//	do
//	{
//		Usart_Send_Str("AT+HTTPTERM\r", 12);
//		delay_ms(500);
//		LCD_ShowStringWithSize(0, 116, 319, 16, 16, Buffer_rx, 40);
//	}while(!check_string("OK",CLEAR));
	
	Usart_Send_Str("AT+HTTPTERM\r", 12);
	delay_ms(200);
	Clear_UART0Buffer();
	
	Data_Icon_Up(0);
	Data_Icon_Down(0);
}
//***********************************************************************
//
//
//***********************************************************************
void HTTP_Post_Upload(unsigned char *data_str_1, unsigned char *data_str_2, unsigned char *data_str_3, unsigned char *data_str_4)
{
	unsigned char i, j, S;
	
	Data_Icon_Up(1);
	
	Usart_Send_Str("AT+HTTPINIT\r", 12);
	delay_ms(200);
	if(check_string("OK",CLEAR))
	{
			Usart_Send_Str("AT+HTTPPARA=\"CID\",1\r", 22);
			delay_ms(200);
			if(check_string("OK",CLEAR))
			{
				Clear_Buffer(Buffer_tx, 500);
				snprintf(Buffer_tx, sizeof(Buffer_tx), "AT+HTTPPARA=\"URL\",\"%s\"\r", "http://sebazar.ir/api/getData.php");
				//LCD_ShowStringWithSize(0, 132, 319, 16, 16, Buffer_tx, 199);
				Usart_Send_Str(Buffer_tx, sizeof(Buffer_tx));
				delay_ms(200);	
				if(check_string("OK",CLEAR))
				{
					//Usart_Send_Str("AT+HTTPPARA=\"CONTENT\",\"text/plain\"\r", 35);
					//Usart_Send_Str("AT+HTTPPARA=\"CONTENT\",\"text/html\"\r", 34);
					//Usart_Send_Str("AT+HTTPPARA=\"CONTENT\",\"application/form-data\"\r", 46);
					Usart_Send_Str("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"\r", 58);					
					delay_ms(200);	
					if(check_string("OK",CLEAR))
					{
						Usart_Send_Str("AT+HTTPDATA=436,1000\r", 21);	
//						sprintf(Buffer_tx, "AT+HTTPDATA=%d,1000\r", sizeof(data_str_1));
//						Usart_Send_Str(Buffer_tx, sizeof(Buffer_tx));
						
						delay_ms(200);	
						if(check_string("DOWNLOAD",CLEAR))
						{
							Clear_Buffer(Buffer_tx, 500);
							snprintf(Buffer_tx, sizeof(Buffer_tx), "%s\r", "id_1=000001&device_id=000001&date=981103&time_1=133015&latitude_1=38.1043&longitude_1=46.3027&speed_1=1.000001&course_1=230.011236&id_2=000002&time_2=133025&latitude_2=38.1044&longitude_2=46.3028&speed_2=1.000002&course_2=250.011236&id_3=000003&time_3=133235&latitude_3=38.1045&longitude_3=46.3029&speed_3=1.000005&course_3=260.011236&id_4=000004&time_4=133235&latitude_4=38.1045&longitude_4=46.3029&speed_4=1.000007&course_4=270.011236");
							Usart_Send_Str(Buffer_tx, sizeof(Buffer_tx));
							
							//Usart_Send_Str(data_str_1, sizeof(data_str_1));
							//Usart_Send_Str("\r", 1);
							
							delay_ms(200);	
							if(check_string("OK",CLEAR))
							{
								Usart_Send_Str("AT+HTTPACTION=1\r", 16);                                                  
								delay_ms(2000);
								LCD_ShowStringWithSize(0, 32, 319, 16, 16, Buffer_rx, 40);
								if(check_string("+HTTPACTION: 1,200,",CLEAR))
								{
									Data_Icon_Down(1);
									Usart_Send_Str("AT+HTTPREAD=1,200\r", 18);
									delay_ms(300);
									LCD_ShowStringWithSize(0, 100, 319, 16, 16, Buffer_rx, 80);
									
									if(Device_mode == 0)
									{
										Clear_Buffer(Buffer_2, 512);
										sprintf(Buffer_2, "  +HTTPREAD: 28  %s:true %s:true %s:true", Sending_id[0], Sending_id[1], Sending_id[2]);
										//LCD_ShowStringWithSize(0, 144, 319, 16, 16, Buffer_2, 80);
										
										S = 0;
										for(i=17;i<37;i++)
											if(Buffer_2[i] != Buffer_rx[i])
												S = 1;
										
										if(S == 0)
											HTTP_sucsessful_upload = 1;
									}
									else if(Device_mode == 2)
									{
										Clear_Buffer(Buffer_2, 512);
										sprintf(Buffer_2, "  +HTTPREAD: 48  %02x%02x%02x%02x%02x:%06s:%s:  OK", Card_ID[0], Card_ID[1], Card_ID[2], Card_ID[3], Card_ID[4], Device_ID, Date);
										//LCD_ShowStringWithSize(0, 144, 319, 16, 16, Buffer_2, 80);
										//delay_ms(3000);
										
										S = 0;
										for(i=17;i<46;i++)
											if(Buffer_2[i] != Buffer_rx[i])
												S = 1;
											
										if(S == 0)
										{
											if(Buffer_rx[46] == 's')
											{
												HTTP_sucsessful_upload = 1;
												Peyment_status = 1;
											}
											else if(Buffer_rx[46] == 'e')
											{
												HTTP_sucsessful_upload = 1;
												Peyment_status = 2;
											}
											else if(Buffer_rx[46] == 'l')
											{
												HTTP_sucsessful_upload = 1;
												Peyment_status = 3;
											}
										}
									}
								}
							}
						}
					}
					
				}
			}
//		}
	}
	
//	do
//	{
//		Usart_Send_Str("AT+HTTPTERM\r", 12);
//		delay_ms(500);
//		LCD_ShowStringWithSize(0, 116, 319, 16, 16, Buffer_rx, 40);
//	}while(!check_string("OK",CLEAR));
	
	Usart_Send_Str("AT+HTTPTERM\r", 12);
	delay_ms(200);
	Clear_UART0Buffer();
	
	Data_Icon_Up(0);
	Data_Icon_Down(0);
}
/*****************************************************************************
** Function name:LayToaDo
**
** Descriptions: lay toa do va do cao
**Frame: AT+CGPSINF=2 2,155906,1045.604076,N,10639.698347,E,1,5,1.792122,62.074841,M,2.310848,M,,0000
 
** parameters:None
** Returned value:
 
*****************************************************************************/
void GetInfo(void)
{
	unsigned char i=0;
	unsigned char *buffer;
	long double ldbLatitude,ldbLongitude,ldbTemp;
	unsigned long int Temp1, Temp2; 
	
//	do
//	{
//		Clear_UART0Buffer();
//		Usart_Send_Str("AT+CGPSINF=2\r", 13);
//		delay_ms(200);
//	}while(!check_string("+CGPSINF: 2,",NOTCLEAR));
//	LCD_ShowStringWithSize(0, 64, 319, 16, 16, Buffer_rx, 120);

	do
	{
		Clear_UART0Buffer();
		Usart_Send_Str("AT+CGPSINF=0\r", 13);
		delay_ms(200);
	}while(!check_string("+CGPSINF: 0,",NOTCLEAR));
	//LCD_ShowStringWithSize(0, 160, 319, 16, 16, Buffer_rx, 120);
	
	buffer = Buffer_rx;
	  
	while (*buffer++ != '\,');  
	  
	while (*buffer != '\,')
	{
		Latitude[i++] = *buffer++; 
	}
	Latitude[11] = 0;
	
	i=0;
	while (*++buffer != '\,')   
	{
		Longitude[i++] = *buffer;   
	}
	Longitude[11] = 0;
	
	i=0;
	while (*++buffer != '\,')   
	{
		Altitude[i++] = *buffer;  
	}
	Altitude[11] = 0;

	i=0; 
	while (i < 8)   
	{
		UTC_Date[i++] = *++buffer; 
	}
	UTC_Date[8] = 0;
	
	i=0;
	while (*++buffer != '\,')   
	{
		UTC_Time[i++] = *buffer;  
	}
	UTC_Time[6] = 0;
	
	while (*++buffer != '\,');
	
	i=0;
	while (*++buffer != '\,')   
	{
		Satellites_Used[i++] = *buffer;   
	}
	Satellites_Used[i] = 0;
	
	i=0;
	while (*++buffer != '\,')   
	{
		Speed[i++] = *buffer;   
	}
	Speed[8] = 0;
	
	i=0;
	while(i<10)  
	{
		Course[i++] = *++buffer;   
	}
	Course[10] = 0;

	dUTC_Time=strtold(UTC_Time,NULL);
	if(((dUTC_Time%10000)/100) < 30)
		dUTC_Time += (3*10000)+(30*100);
	else
		dUTC_Time += (4*10000)-(30*100);
	sprintf(UTC_Time,"%d",dUTC_Time);
	UTC_Time[6] = 0;	
	
	dUTC_Date=strtold(UTC_Date,NULL);
	
	ldbLatitude=strtold(Latitude,NULL);
	Temp1 = ldbLatitude / 100;
	Temp2 = ldbLatitude * 10000;
	ldbTemp = (((Temp2 % 1000000) * 10) / 6.0) / 1000000;
	ldbLatitude = Temp1 + ldbTemp;
	snprintf(Latitude,8,"%f",ldbLatitude);
	
	ldbLongitude=strtold(Longitude,NULL);
	Temp1 = ldbLongitude / 100;
	Temp2 = ldbLongitude * 10000;
	ldbTemp = (((Temp2 % 1000000) * 10) / 6.0) / 1000000;
	ldbLongitude = Temp1 + ldbTemp;
	snprintf(Longitude,8,"%f",ldbLongitude);
	
//	sprintf(Buffer_1, "%s %s %s %s %s %s %s %s", Latitude, Longitude, Altitude, UTC_Date, UTC_Time, Satellites_Used, Speed, Course);
//	LCD_ShowStringWithSize(0, 112, 319, 16, 16, Buffer_1, 80);
}
 
/*****************************************************************************
** Function name: LayTocDo
**
** Descriptions: lay toc do
**Frame:
 
** parameters:None
** Returned value:
 
*****************************************************************************/
void GetSpeed(void)
{
 
	unsigned char *buffer;
	unsigned char i=0,j,timeout;
	 
	Clear_UART0Buffer();
	//UARTSend( 1,"AT+CGPSINF=64\r"); //Course over ground & Ground speed
	Usart_Send_Str("AT+CGPSINF=64\r", 14);
	delay_ms(200);
	buffer = Buffer_rx;
	 
	for (j=0;j<7;j++)
	{
		timeout=30;
		while ((*buffer++ != '\,')&&(timeout--)); //bo qua 7 dau "," dau tien
	}
	 
	while (*buffer != '\,')
	{
		Speed[i++] = *buffer++;   // km/h
	}
	dbSpeed= strtod(Speed,NULL);
  
//	UART_Send(_LPC_UART2, Speed, sizeof(Speed), BLOCKING);
}
//***********************************************************************
// END OF SIM800.c
//***********************************************************************
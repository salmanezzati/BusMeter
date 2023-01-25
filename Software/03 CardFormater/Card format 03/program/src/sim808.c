
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
unsigned char SMS_Content[100];     /* noi dung tin nhan */
unsigned char SMS_Password[7];
unsigned char Trip_Price[5];
unsigned char Data_Time[4];
unsigned char Location_Data_Time[4];

// GPS Coordinates
unsigned char Latitude[14];  //  Latitude, North (vi do)
unsigned char Longitude[15]; // Longitude, East  (kinh do)
unsigned char Elevation[13];  // Elevation  (do cao)
unsigned char Lat_degree=0;
unsigned char Lat_minute=0;
float Lat_second=0;
unsigned char Lat_dir;
unsigned char Long_degree=0;
unsigned char Long_minute=0;
float Long_second=0;
unsigned char Long_dir;
unsigned char Speed[7];
long double ldbLatitude,ldbLongitude;
double dbSpeed;
unsigned int dUTC_Time;
unsigned char UTC_Time[7];
unsigned char UTC_Date[10];
unsigned char Position_Fix;
unsigned char Satellites_Used[];

unsigned char Phone_Num_1[12]="09148882278";
unsigned char Phone_Num_2[12];
unsigned char Temp_Phone_Num[12];
unsigned char Password[7]="975310";
unsigned char Temp_Device_ID[7];

unsigned char phone_num[11]="09396897845";
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
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
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
	while(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // Received characters modify string
	{
        if (Usart_sec >= 10)
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
}

//---Usart_Send(unsigned char Ch)--------------------------
void Usart_Send(unsigned char Ch)
{
    USART_SendData(USART1,Ch);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
}

//---Usart_Send_Str(unsigned char *Data, unsigned char Data_len)----
void Usart_Send_Str(unsigned char *Data, unsigned char Data_len)
{
    char i;
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
		LCD_ShowStringWithSize(0, 32, 300, 16, 16, Buffer_rx, 40);
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
	
	do
	{ 
		Usart_Send_Str("AT+CGPSSTATUS?\r", 15); 
		delay_ms(1000);
		//LCD_ShowStringWithSize(0, 128, 300, 16, 16, Buffer_rx, 40);
	}while(!check_string("+CGPSSTATUS: Location 3D Fix",CLEAR));
	
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
    

	LCD_ShowString(0, 32, 120, 16, 16, "SIM808 Configed");
    

	HTTP_Config();
//	Blink_1();
//	while(1)
//	{
//		Data1 ++;
//		Data2 +=2;
//		Data3 +=4;
//		if(Data1 > 30)
//			Data1 = 1;
//		
//		if(Data2 > 30)
//			Data2 = 2;
//		
//		//snprintf(data_str, sizeof(data_str), "http://icai.ir/data.php?data=insert?%s?salman?2019/7/6?%s?%s", data1, data2, data3);
//		sprintf(data_str, "http://sebazar.ir/sebApi/dataDastgah/insertData.php?id=%02d&data1=%c&data2=2019&data3=%s&data4=%s%02d&data5=%02d", Data2, 'M', "salman", "DriverName", 25, Data1);
//		//HTTP_Upload();
//		LCD_ShowStringWithSize(0, 80, 319, 16, 16, Buffer_SMS, 120);
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

//---Get_Signal_Quality-------------------------------
unsigned char Get_Signal_Quality(void)
{
	unsigned char Buffer[3];
	do
	{
		Usart_Send_Str("AT+CSQ\r", 7);
		delay_ms(200);
		Buffer[0] = Buffer_rx[8];
		Buffer[1] = Buffer_rx[9];
		Buffer[2] = 0;
	}while(!check_string("OK",CLEAR));
	
	return strtold(Buffer,NULL);
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
    
	Clear_UART0Buffer();
	Usart_Send_Str("AT+CMGR=1\r", 10);
	
	delay_ms(1000);
	
	SMS_Receive();
	if(Check_Change_Setting())
	{
		sprintf(Buffer_1, "Phone_Num=%s,,Password=%s,,Device_ID=%s,,Trip_Price=%s,,All_Data_Time=%s,,Location_Data_Time=%s", SMS_PhoneNumber, SMS_Password, Device_ID, Trip_Price, Data_Time, Location_Data_Time);
		LCD_ShowStringWithSize(0, 112, 319, 16, 16, Buffer_1, 120);
	}
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
	unsigned char i=0, j=0;
	unsigned char *Buffer;
	 
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
//---SMS_Content--
	Clear_Buffer(SMS_Content, 100);
	i=0;
	while (*++Buffer != '\r')
	{
		SMS_Content[i++] = *Buffer; 
	}
//---SMS_Password-	
	Clear_Buffer(SMS_Password, 7);
	for(i=0;i<6;i++)
		SMS_Password[i] = SMS_Content[i];
	
//---Device_ID------ 
	Clear_Buffer(Temp_Device_ID, 7);
    for(i=0;i<6;i++)
		Temp_Device_ID[i] = SMS_Content[i+7];
	
//---Trip_Price------ 
	Clear_Buffer(Trip_Price, 5);
    for(i=0;i<4;i++)
		Trip_Price[i] = SMS_Content[i+14];

//---Data_Time------ 
	Clear_Buffer(Data_Time, 4);
    for(i=0;i<3;i++)
		Data_Time[i] = SMS_Content[i+19];

//---Location_Data_Time------ 
	Clear_Buffer(Location_Data_Time, 4);
    for(i=0;i<3;i++)
		Location_Data_Time[i] = SMS_Content[i+23];
	
//---Location_Data_Time------ 
	Clear_Buffer(Temp_Phone_Num, 12);
    for(i=0;i<11;i++)
		Temp_Phone_Num[i] = SMS_Content[i+27];
}
//---Check_Change_Setting---------- 
unsigned char Check_Change_Setting(void)
{
	unsigned char S1, S2; 
 
    S1 = strcmp(Phone_Num_1, SMS_PhoneNumber);
	S2 = strcmp(Phone_Num_2, SMS_PhoneNumber);
    if(S1 == 0 || S2 == 0) 
	{	    
		S1 = strcmp(Password, SMS_Password);
		if(S1 == 0)
		{
			strcpy(Device_ID, Temp_Device_ID);
			SD_WriteSingleBlock(Device_ID, device_id_addr);
			
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
			
			LCD_ShowStringWithSize(0, 160, 319, 16, 16, "Parameters Set.", 40);
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
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
	uint8_t k; 
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	Buffer = Buffer_rx;
	for(k=0;k<250;k++)
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
		LCD_ShowStringWithSize(0, 64, 300, 16, 16, Buffer_rx, 40);
	}while(!check_string("OK",CLEAR));
}
//***********************************************************************
//
//
//***********************************************************************
void HTTP_Upload(unsigned char *data_str)
{
	unsigned char i, j, S, *C1, *C2, *C3;
	
	Usart_Send_Str("AT+HTTPINIT\r", 12);
	delay_ms(200);
	if(check_string("OK",CLEAR))
	{
		Usart_Send_Str("AT+HTTPPARA=\"CID\",1\r", 22);
		delay_ms(200);
		if(check_string("OK",CLEAR))
		{
			snprintf(Buffer_tx, sizeof(Buffer_tx), "AT+HTTPPARA=\"URL\",\"%s\"\r", data_str);
			Usart_Send_Str(Buffer_tx, sizeof(Buffer_tx));
			delay_ms(200);	
			if(check_string("OK",CLEAR))
			{
				j = 0;
				do
				{
					do
					{
						j++;
						if(j > 2)
							break;
						Usart_Send_Str("AT+HTTPACTION=0\r", 16);
						//delay_ms(300);
						//LCD_ShowStringWithSize(0, 80, 319, 16, 16, "                                                          ", 60);
						delay_ms(2000);
						//LCD_ShowStringWithSize(0, 80, 319, 16, 16, Buffer_rx, 60);
					}while(!check_string("+HTTPACTION: 0,200,",CLEAR));
					
					if(j > 2)
						break;
					
					Usart_Send_Str("AT+HTTPREAD=1,200\r", 18);
					delay_ms(300);
					//LCD_ShowStringWithSize(0, 112, 319, 16, 16, Buffer_rx, 80);
					
					if(Device_mode == 0)
					{
						Clear_Buffer(Buffer_2, 512);
						sprintf(Buffer_2, "  +HTTPREAD: 28  %05s:%06s:%s:true  OK", Sending_id, Device_ID, Date);
						//LCD_ShowStringWithSize(0, 144, 319, 16, 16, Buffer_2, 80);
						
						S = 0;
						for(i=17;i<44;i++)
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
					
					
				}while(S != 0);
			}
		}
	}
	do
	{
		Usart_Send_Str("AT+HTTPTERM\r", 12);
		delay_ms(200);
	}while(!check_string("OK",CLEAR));
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
	 
	//Clear_UART0Buffer();
	do
	{
		Clear_UART0Buffer();
		Usart_Send_Str("AT+CGPSINF=2\r", 13);
		delay_ms(200);
	}while(!check_string("+CGPSINF: 2,",NOTCLEAR));
	//LCD_ShowStringWithSize(0, 112, 319, 16, 16, Buffer_rx, 120);
	//while(1);
	
	buffer = Buffer_rx;
	  
	while (*buffer++ != '\,');  //tim dau " , " dau tien
	  
	while (*buffer != '\,')
	{
		UTC_Time[i++] = *buffer++; 
	}
	UTC_Time[6] = 0;
	
	i=0;
	while (*++buffer != '\,')
	{
		Latitude[i++] = *buffer;   //vi do
	}
	Latitude[i] = *++buffer;  //North or West
	Latitude[13] = 0;  
	
	i=0;
	buffer++;
	while (*++buffer != '\,')   
	{
		Longitude[i++] = *buffer;   //kinh do
	}
	Longitude[i] = *++buffer;   //North or West
	Longitude[14] = 0;
	
	i=0;
	 
	buffer+=2;
	Position_Fix = *buffer;
	buffer+=2;
	*Satellites_Used = *buffer;
	
	buffer++;
	while (*++buffer != '\,');
	 
	while (*++buffer != '\,')
	{
		Elevation[i++]=*buffer;
	}
	Elevation[i] = *++buffer;
	Elevation[12] = 0;
	
	dUTC_Time=strtold(UTC_Time,NULL);
	if(((dUTC_Time%10000)/100) < 30)
		dUTC_Time += (3*10000)+(30*100);
	else
		dUTC_Time += (4*10000)-(30*100);
	sprintf(UTC_Time,"%d",dUTC_Time);
	UTC_Time[6] = 0;	
	
//	ldbLatitude=strtold(Latitude,NULL);
//	ldbLatitude=ldbLatitude + 4.1690;
//	snprintf(Latitude,12,"%f",ldbLatitude);
//	
//	ldbLongitude=strtold(Longitude,NULL);
//	ldbLongitude=ldbLongitude + 12.1066;
//	snprintf(Longitude,12,"%f",ldbLongitude);
	
//	UART_Send(_LPC_UART2, UTC_Time, sizeof(UTC_Time), BLOCKING);
//	UART_Send(_LPC_UART2, Latitude, sizeof(Latitude), BLOCKING);
//	UART_Send(_LPC_UART2, Longitude, sizeof(Longitude), BLOCKING);
//	UART_Send(_LPC_UART2, Satellites_Used, sizeof(Satellites_Used), BLOCKING);
//	UART_Send(_LPC_UART2, Elevation, sizeof(Elevation), BLOCKING);
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

#include <stdint.h>
#include "stm32f4xx.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "lcd.h"
#include "delay.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

#define CtrlZ       26
#define CLEAR       1
#define NOTCLEAR    0
#define NOSIGNAL    0
#define NOTFIXED    1
#define FIXED       2
#define READY       1
#define NOTREADY    0
 
 
#define DEL_READ     1
#define DEL_UNREAD   2
#define DEL_SENT     3
#define DEL_UNSENT   4
#define DEL_INBOX    5
#define DEL_ALL      6
 
#define   NUM_RIGHT      1
#define   NUM_WRONG      0
#define   SYNTAX_RIGHT   1
#define   SYNTAX_WRONG   0
#define   PASS_RIGHT     1
#define   PASS_WRONG     0

unsigned char SMS_PhoneNumber[14];     /* phone number gui*/
unsigned char SMS_Sender[15];     /* The sender (the name three) */
unsigned char SMS_Date[9];    /* Ngay gui*/
unsigned char SMS_Time[12];    /* gio gui */
unsigned char SMS_Content[100];     /* noi dung tin nhan */
unsigned char SMS_Syntax[6];
unsigned char SMS_Password[5];
 
unsigned char defaultPhoneNum[13]="+989148882278";
unsigned char defaultSyntax[7]="samin";
unsigned char defaultPassword[5]="1234";


unsigned char phone_num[11]="09396897845";
int rx_index=0;
unsigned char Show_flag;
unsigned char SMS_num;

extern unsigned char Lcd_buffer[250], Buffer_tx[250], Buffer_rx[250];
extern int Usart_sec;
/************************** PRIVATE FUNCTIONS *************************/
/* Interrupt service routines */
void USART1_IRQHandler(void);
void Usart1_Init(void);
void Usart_Send(unsigned char Ch);
void Usart_Send_Str(unsigned char *Data, unsigned char Data_len);

void TIM4_Int_Init(u16 Arr,u16 Psc);
void TIM2_Int_Init(u16 Arr,u16 Psc);

void Sim900_PowerOnOff(void);
void Sim900_Init(void);
unsigned char check_string(const char *str,unsigned char Clear);
unsigned char check_string2(const char *str,unsigned char Clear);
unsigned char SMS_Send(unsigned char *Phone_num, unsigned char *Content);
void SMS_Read(unsigned char Index);
void SMS_Receive(void);
void SMS_Delete(unsigned char Type);
void Clear_UART0Buffer(void); 
void Fill_UART0Buffer(void);
void Lcd_Show(void);
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
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
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
	while(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // Received characters modify string
	{
        if (Usart_sec >= 10)
            rx_index = 0;
        
        Buffer_rx[rx_index++] = USART_ReceiveData(USART1);
	}
    Usart_sec=0;
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
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
        
        if(check_string("+CMTI: \"SM\",",NOTCLEAR))
        {    
            //Lcd_Show();
            delay_ms(200);
            if(Buffer_rx[15] >= 0x30 && Buffer_rx[15] <= 0x39)
                SMS_Read(((Buffer_rx[14]-0x30)*10)+(Buffer_rx[15]-0x30));
            else
                SMS_Read(Buffer_rx[14]-0x30);
            //Lcd_Show();
            LCD_ShowString(0, 30, 220, 15, 15, SMS_PhoneNumber);
            LCD_ShowString(0, 45, 220, 15, 15, SMS_Sender);
            LCD_ShowString(0, 60, 220, 15, 15, SMS_Date);
            LCD_ShowString(0, 75, 220, 15, 15, SMS_Time);
            LCD_ShowString(0, 90, 220, 15, 15, SMS_Content);
            LCD_ShowString(0, 105, 220, 15, 15, SMS_Syntax);
            LCD_ShowString(0, 120, 220, 15, 15, SMS_Password);
        }
	}
}

//**************************************************************
//
//**************************************************************
void TIM2_Int_Init(u16 Arr,u16 Psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	
	
	TIM_TimeBaseStructure.TIM_Period = Arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =Psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM2, ENABLE);  
}

void TIM2_IRQHandler(void)   
{
    
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  
 		//GPIO_ToggleBits(GPIOC, GPIO_Pin_0);
        
        if(Show_flag)
        {    
            Show_flag=0;
            LCD_Clear(GREEN);
            LCD_ShowString(0, 30, 220, 15, 15, SMS_PhoneNumber);
            LCD_ShowString(0, 45, 220, 15, 15, SMS_Sender);
            LCD_ShowString(0, 60, 220, 15, 15, SMS_Date);
            LCD_ShowString(0, 75, 220, 15, 15, SMS_Time);
            LCD_ShowString(0, 90, 220, 15, 15, SMS_Content);
            LCD_ShowString(0, 105, 220, 15, 15, SMS_Syntax);
            LCD_ShowString(0, 120, 220, 15, 15, SMS_Password);
            //Lcd_Show();
        }
        else if(check_string2("+CMTI: \"SM\",",NOTCLEAR))
        {    
            if(Buffer_rx[15] >= 0x30 && Buffer_rx[15] <= 0x39)
                SMS_num=((Buffer_rx[14]-0x30)*10)+(Buffer_rx[15]-0x30);
            else
                SMS_num=Buffer_rx[14]-0x30;
        }
	}
}
/*******************************************************************
** Function name:Sim900_PowerOnOff
**
** Descriptions:
**
** parameters:None
** Returned value:None
**
*****************************************************************************/
void Sim900_PowerOnOff(void)
{
	sprintf(Lcd_buffer, "SIM900 is Starting");
	LCD_ShowString(0, 0, 150, 15, 15, Lcd_buffer);
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	delay_ms(500);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	sprintf(Lcd_buffer, "SIM900 is On");
	LCD_ShowString(0, 15, 100, 15, 15, Lcd_buffer);
	delay_ms(1000);
}
/*****************************************************************************
** Function name:Sim900_Init
**
** Descriptions:
**
** parameters:None
** Returned value:None
**
*****************************************************************************/
void Sim900_Init(void)
{
	unsigned char i,j=0;
    
    //Usart1_Init ();
    //TIM4_Int_Init(199,41999);
    
	do
	{
        sprintf(Buffer_tx, "AT\r");
		Usart_Send_Str(Buffer_tx, 3);
        delay_ms(200);
	}while(!check_string("OK",CLEAR));
    
	do
	{
		sprintf(Buffer_tx, "ATZ\r");
		Usart_Send_Str(Buffer_tx, 4);
		delay_ms(200);
	}while(!check_string("OK",CLEAR));
    
	do
	{
		sprintf(Buffer_tx, "ATE0\r");  //echo mode: off
		Usart_Send_Str(Buffer_tx, 5);
		delay_ms(200);
	}while(!check_string("OK",CLEAR));
	
	do
	{
		sprintf(Buffer_tx, "AT+IPR=57600\r");  //Set TE-TA fixed local rate :57600bps
		Usart_Send_Str(Buffer_tx, 13);
		delay_ms(200);
	}while(!check_string("OK",CLEAR));
	
	//do
	//{
	//	sprintf(Buffer_tx, "AT+CGPSIPR=57600\r");  //Set TE-TA fixed local rate (for GPS) :57600bps
	//	Usart_Send_Str(Buffer_tx, 17);
	//	delay_ms(200);
	//}while(!check_string("OK",CLEAR));
	
	for(i=0;i<5;i++)
	{
		sprintf(Buffer_tx, "AT+CPIN?\r");  //check sim card
		Usart_Send_Str(Buffer_tx, 9);
		delay_ms(500); 
		if(!check_string("READY",CLEAR)) 
			j++;
		else 
			break;
	}
	if (j==5)
	{
		sprintf(Lcd_buffer, "Insert SIM card");
		LCD_ShowString(0, 30, 120, 16, 16, Lcd_buffer);
		while(1); 
	}
	
	do
	{
		sprintf(Buffer_tx, "AT+ICF=3,3\r");  //8 data bit, 1 stop bit,no parity
		Usart_Send_Str(Buffer_tx, 11);
		delay_ms(200);
	}while(!check_string("OK",CLEAR));
	
// 	do
// 	{
// 		sprintf(Buffer_tx, "AT+CRES");  //restore sms settings
// 		Usart_Send_Str(Buffer_tx, 7);
// 		delay_ms(200);
//         
//         Lcd_Show();
// 	}while(!check_string("OK",CLEAR));
 	
	do
	{
		sprintf(Buffer_tx, "AT+CMGF=1\r");  //set SMS system into text mode
		Usart_Send_Str(Buffer_tx, 10);
		delay_ms(200);
	}while(!check_string("OK",CLEAR));
 	
	do
	{
		sprintf(Buffer_tx, "AT+CSCS=\"GSM\"\r");  //GSM 7 bits default alphabet
		Usart_Send_Str(Buffer_tx, 16);
		delay_ms(200);
	}while(!check_string("OK",CLEAR));
 	
	do
	{
		sprintf(Buffer_tx, "AT+CNMI=2,0,0,0,0\r");  //New message indication enable.
		Usart_Send_Str(Buffer_tx, 18);
		delay_ms(200);
	}while(!check_string("OK",CLEAR));
	 
	do
	{
		sprintf(Buffer_tx, "AT+CNMI=1,1,0,0,0\r"); //return the position save massages 
		Usart_Send_Str(Buffer_tx, 18);
		delay_ms(200);
	}while(!check_string("OK",CLEAR));
	
	do
	{
		sprintf(Buffer_tx, "AT+CSAS\r"); //save configuration sms
		Usart_Send_Str(Buffer_tx, 8);
		delay_ms(200);
	}while(!check_string("OK",CLEAR));
	
	//SMS_Delete(DEL_ALL);

// 	do{
// 		 UARTSend(_LPC_UART, "AT+CREG=5\r", 9);
// 	   delay(2000000);
// 		 UART_Send(_LPC_UART2, rb.rx, 150, BLOCKING);
//   }while(!check_string("OK",CLEAR));
	
	do
	{
		sprintf(Buffer_tx, "AT+CREG?\r"); 
		Usart_Send_Str(Buffer_tx, 9);
		delay_ms(2000);
	}while(!check_string("+CREG: 0,1",CLEAR));
	
// 	do{
// 		 UARTSend(_LPC_UART, "AT+CREG?\r", 9);
// 	   delay(2000000);
// 		 UART_Send(_LPC_UART2, rb.rx, 150, BLOCKING);
//   }while(!check_string("+CREG: 0,5",CLEAR));
	
	//do
	//{
	//	sprintf(Buffer_tx, "AT+CGPSPWR=1\r"); //Turn on GPS power supply
	//	Usart_Send_Str(Buffer_tx, 13);
	//	delay_ms(200);
	//}while(!check_string("OK",CLEAR)); 
	
	//do
	//{
	//	sprintf(Buffer_tx, "AT+CGPSRST=0\r"); //reset GPS in cold mode
	//	Usart_Send_Str(Buffer_tx, 13);
	//	delay_ms(200);
	//}while(!check_string("OK",CLEAR)); 
	
	
	//sprintf(Lcd_buffer, "GPS Tracker");
	//LCD_ShowString(0, 30, 120, 16, 16, Lcd_buffer);
	//delay_ms(1000); //delay 35s for cold starting
	
	//do
	//{
	//	sprintf(Buffer_tx, "AT+CGPSSTATUS?\r"); //luu cau hinh sms
	//	Usart_Send_Str(Buffer_tx, 15);
	//	delay_ms(200);
	//}while(!check_string("OK",CLEAR));
	
	Clear_UART0Buffer();
	
    //SMS_Send("09141704476","Hi. We can. This SMS sent from Alarm System.");
    
    while(1)
    {
        SMS_Read(5);
        Show_flag=1;
        delay_ms(1000);
     
        if(SMS_num) 
        {            
            for(i=0;i<2;i++)
            {
                SMS_Read(SMS_num);
                Show_flag=1;
                delay_ms(2000);
            }
            SMS_num=0;
        }
    } 
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
		sprintf(Lcd_buffer, "Message Sent");
		LCD_ShowString(0, 30, 120, 16, 16, Lcd_buffer);
		return 1; 
	}
	else 
		return 0;
	 
}
	//doc tin nhan o vi tri index (index<100)
	// module sim908 se tra ve noi dung tin nhan
	//                    nam o vi tri index trong bo nho
void SMS_Read(unsigned char Index)
{
	char Temp[2];
	 
	sprintf(Temp,"%d",Index);
	 
	strcpy (Buffer_tx,"AT+CMGR=");
	strcat (Buffer_tx,Temp);
	strcat (Buffer_tx,",1\r");
	 
	Clear_UART0Buffer();
	Usart_Send_Str(Buffer_tx, 14);
    
	delay_ms(1000);
	
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
  
	unsigned char i=0;
	unsigned char *Buffer;
	 
	Buffer = Buffer_rx;
	 
	//tim dau " thu 3
	for(i=0; i<3; i++)
	{
		while(*Buffer++ != '\"'); 
	}
	  
	 //sdt nguoi gui
	for(i=0;i<13;i++)
	{
		SMS_PhoneNumber[i]='\0';
	}
	i=0;
	while (*Buffer != '\"')
	{
		SMS_PhoneNumber[i++] = *Buffer++; 
	}
	
	while (*++Buffer != '\"');
	  
	 //ten nguoi gui
	 i=0;
	while (*++Buffer != '\"')
	{
		SMS_Sender[i++] = *Buffer;  
	}
	while (*++Buffer != '\"');
	  
	 //ngay gui
	i=0;
	while (*++Buffer != '\,')   
	{
		SMS_Date[i++] = *Buffer; 
	}
	  
	 //gio gui
	i=0;
	while (*++Buffer != '\"')
	{
		SMS_Time[i++] = *Buffer;
	}
	  
	while (*++Buffer != '\n');  //bo qua ki tu ko can thiet
	   
	 //noi dung tin nhan
	for(i=0;i<100;i++) 
        SMS_Content[i]='\0';
	 
	i=0;
	while (*++Buffer != '\r')
	{
		SMS_Content[i++] = *Buffer; 
	}
	//loc ra cu phap
	for(i=0;i<5;i++)
	{
		SMS_Syntax[i]=SMS_Content[i];
	}
	  //loc ra mat khau
	for(i=0;i<5;i++)  
		SMS_Password[i]='\0';
	 
	for(i=0;i<strlen(SMS_Content)-5;i++)
	{
		SMS_Password[i]=SMS_Content[i+5];
	}
 
}
 
unsigned char CheckPhoneNum()
{
	unsigned char S; 
 
    S=strcmp(defaultPhoneNum,SMS_PhoneNumber);
    if(S==0) 
		return NUM_RIGHT;     
    else 
		return NUM_WRONG;
 
}
unsigned char CheckSyntax()
{
	unsigned char S; 
 
    S=strcmp(defaultSyntax,SMS_Syntax);  
    if(S==0) 
		return SYNTAX_RIGHT;     
    else 
		return SYNTAX_WRONG;
 
}
unsigned char CheckPassword()
{
	unsigned char S; 
 
    //len=strlen(SMS_Content);
    S=strcmp(defaultPassword,SMS_Password);
    if(S==0) 
		return PASS_RIGHT;     
    else 
		return PASS_WRONG;
 
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
    
    if(S != '\0'){ 
			 if (Clear) Clear_UART0Buffer();
		   return 1;  
    }			
    else
	{
		Fill_UART0Buffer();
		S = strstr(Buffer_rx,str);			
		if (Clear) Clear_UART0Buffer();
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
		if (Clear) Clear_UART0Buffer();
			return 1;  
    }			
    else
	{
		return 0;  
    }
}

void Lcd_Show(void)
{
    unsigned char i;
    for(i=0;i<100;i++)
    {
        
        if(i<40)
            LCD_ShowChar(8*i, 140, Buffer_rx[i], 16, 1);
        else if(i>=40 && i<80)
            LCD_ShowChar(8*(i-40), 155, Buffer_rx[i], 16, 1);
        else if(i>=80 && i<120)
            LCD_ShowChar(8*(i-80), 170, Buffer_rx[i], 16, 1);
        else if(i>=120 && i<160)
            LCD_ShowChar(8*(i-120), 185, Buffer_rx[i], 16, 1);
        else if(i>=160 && i<200)
            LCD_ShowChar(8*(i-160), 200, Buffer_rx[i], 16, 1);
        else if(i>=200 && i<240)
            LCD_ShowChar(8*(i-200), 215, Buffer_rx[i], 16, 1);
    }
}
//***********************************************************************
// END OF SIM900.h
//***********************************************************************
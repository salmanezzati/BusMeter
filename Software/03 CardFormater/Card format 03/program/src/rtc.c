	
#include "rtc.h"

#define RTC_CLOCK_SOURCE_LSE

RTC_InitTypeDef RTC_InitStructure;
__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;
	
void RTC_Config(void)
{
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);
    
#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
/* The RTC Clock may varies due to LSI frequency dispersion. */   
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;

#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;

#else
  #error Please select the RTC Clock source inside the main.c file
#endif /* RTC_CLOCK_SOURCE_LSI */
  
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
}

RTC_TimeTypeDef RTC_TimeStructure;

void RTC_TimeRegulate(void)
{
	uint32_t tmp_hh = 00, tmp_mm = 00, tmp_ss = 01;

//	printf("\n\r==============Time Settings=====================================\n\r");
	
	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours = tmp_hh;
	RTC_TimeStructure.RTC_Minutes = tmp_mm;
	RTC_TimeStructure.RTC_Seconds = tmp_ss;

	/* Configure the RTC time register */
	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
//		printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
	} 
	else
	{
//		printf("\n\r>> !! RTC Set Time success. !! <<\n\r");
		/* Indicator for the RTC configuration */
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	}
}

void RTC_Start(void)
{
	/* Output a message on Hyperterminal using printf function */
	//printf("\n\r  *********************** RTC Hardware Calendar Example ***********************\n\r");

	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
	{  
		/* RTC configuration  */
		RTC_Config();

		/* Configure the RTC data register and RTC prescaler */
		RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
		RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;

		/* Check on RTC init */
		if (RTC_Init(&RTC_InitStructure) == ERROR)
		{
//		  printf("\n\r        /!\\***** RTC Prescaler Config failed ********/!\\ \n\r");
		}

		/* Configure the time register */
		RTC_TimeRegulate(); 
	}
	else
	{
		/* Check if the Power On Reset flag is set */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
//		  printf("\r\n Power On Reset occurred....\n\r");
		}
		/* Check if the Pin Reset flag is set */
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
//		  printf("\r\n External Reset occurred....\n\r");
		}

//		printf("\n\r No need to configure RTC....\n\r");

		/* Enable the PWR clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* Allow access to RTC */
		PWR_BackupAccessCmd(ENABLE);

		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();

		/* Clear the RTC Alarm Flag */
		RTC_ClearFlag(RTC_FLAG_ALRAF);

		/* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
		//EXTI_ClearITPendingBit(EXTI_Line17);

		/* Display the RTC Time and Alarm */
		//RTC_TimeShow();
		//RTC_AlarmShow();
	}	
}

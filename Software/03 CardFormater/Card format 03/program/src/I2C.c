
//***I2C_EEPROM****************************************** 
    while(1)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_4);
        GPIO_ResetBits(GPIOB, GPIO_Pin_5);
        
        if(Write_24Cxx(0x0000,0x90,M2432))
        {
            //error
            //while(1);
        }

        if((Read_24Cxx(0x0000,M2432))==0xff)
        {      // ERROR
            while(1)
            {
                GPIO_ToggleBits(GPIOB, GPIO_Pin_4);
                delay_ms(500);
            }
        }
        else if((Read_24Cxx(0x0000,M2432))==0x90)
        {
            //Success
            GPIO_ToggleBits(GPIOB, GPIO_Pin_5);
        }
        delay_ms(2000);
    }

//---I2C1_Init (void)----for EEPROM--------------------------
void I2C1_Init (void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    I2C_InitTypeDef   I2C_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* Configure I2C1 */
    I2C_DeInit(I2C1);
    
    /* Set the I2C structure parameters */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0xEE;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 30000;

    I2C_Init(I2C1, &I2C_InitStructure);

    I2C_Cmd(I2C1, ENABLE);
    
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);
}

//------Write_24Cxx(uint16_t Addr, uint8_t Data, uint8_t Mem_Type)------
uint8_t Write_24Cxx(uint16_t Addr, uint8_t Data, uint8_t Mem_Type)
{

	uint32_t timeout = I2C_TIMEOUT_MAX;
	uint8_t upper_addr,lower_addr;

	lower_addr = (uint8_t)((0x00FF) & Addr);

// 	if(Mem_Type==M24512)
// 	{
//         Addr = Addr&gt;&gt;8;
//         upper_addr = (uint8_t)((0x00FF) & Addr);
//     }
    /* Generate the Start Condition */
    I2C_GenerateSTART(I2C1, ENABLE);

    /* Test on I2C1 EV5, Start trnsmitted successfully and clear it */
    timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        /* If the timeout delay is exeeded, exit with error code */
        //if ((timeout--) == 0) return 0xFF;
    }

    /* Send Memory device slave Address for write */
    I2C_Send7bitAddress(I2C1, MEM_DEVICE_WRITE_ADDR, I2C_Direction_Transmitter);

    /* Test on I2C1 EV6 and clear it */
    timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
       /* If the timeout delay is exeeded, exit with error code */
        //if ((timeout--) == 0) return 0xFF;
    }

// 	if(Mem_Type==M24512)
// 	{
// 		/* Send I2C1 location address LSB */
// 		I2C_SendData(I2C1, upper_addr);

// 		/* Test on I2C1 EV8 and clear it */
// 		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
// 		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
// 		{
// 			/* If the timeout delay is exeeded, exit with error code */
// 			if ((timeout--) == 0) return 0xFF;
// 		}
// 	}
	/* Send I2C1 location address LSB */
    I2C_SendData(I2C1, lower_addr);

    /* Test on I2C1 EV8 and clear it */
    timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        /* If the timeout delay is exeeded, exit with error code */
        //if ((timeout--) == 0) return 0xFF;
    }

    /* Send Data */
    I2C_SendData(I2C1, Data);

    /* Test on I2C1 EV8 and clear it */
    timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
         /* If the timeout delay is exeeded, exit with error code */
        //if ((timeout--) == 0) return 0xFF;
    }  

    /* Send I2C1 STOP Condition */
    I2C_GenerateSTOP(I2C1, ENABLE);

    /* If operation is OK, return 0 */
    return 0;
}

//------Read_24Cxx(uint16_t Addr, uint8_t Mem_Type)------------
uint8_t Read_24Cxx(uint16_t Addr, uint8_t Mem_Type)
{
    uint32_t timeout = I2C_TIMEOUT_MAX;
    uint8_t Data = 0;

    uint8_t upper_addr,lower_addr;

    lower_addr = (uint8_t)((0x00FF)& Addr);

//     if(Mem_Type==M24512)
//     {
//         Addr = Addr&gt;&gt;8;
//         upper_addr = (uint8_t)((0x00FF)&amp;Addr);
//     }
    /* Generate the Start Condition */
    I2C_GenerateSTART(I2C1, ENABLE);

    /* Test on I2C1 EV5 and clear it */
    timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        /* If the timeout delay is exceeded, exit with error code */
        //if ((timeout--) == 0) return 0xFF;
    } 

    I2C_Send7bitAddress(I2C1, MEM_DEVICE_WRITE_ADDR, I2C_Direction_Transmitter);

    /* Test on I2C1 EV6 and clear it */
    timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        /* If the timeout delay is exeeded, exit with error code */
        //if ((timeout--) == 0) return 0xFF;
    } 

// 	if(Mem_Type==M24512)
// 	{
// 		/* Send I2C1 location address LSB */
// 		I2C_SendData(I2C1,upper_addr);

// 		/* Test on I2C1 EV8 and clear it */
// 		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
// 		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
// 		{
// 			/* If the timeout delay is exeeded, exit with error code */
// 			if ((timeout--) == 0) return 0xFF;
// 		}
// 	}		

    /* Send I2C1 location address LSB */
    I2C_SendData(I2C1, lower_addr);

    /* Test on I2C1 EV8 and clear it */
    timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        /* If the timeout delay is exeeded, exit with error code */
        //if ((timeout--) == 0) return 0xFF;
    }  

    /* Clear AF flag if arised */
    //I2C1-&gt;SR1 |= (uint16_t)0x0400;

    /* Generate the Start Condition */
    I2C_GenerateSTART(I2C1, ENABLE);

    /* Test on I2C1 EV6 and clear it */
    timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        /* If the timeout delay is exeeded, exit with error code */
        //if ((timeout--) == 0) return 0xFF;
    } 

    I2C_Send7bitAddress(I2C1, MEM_DEVICE_READ_ADDR, I2C_Direction_Receiver);

    /* Test on I2C1 EV6 and clear it */
    timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        /* If the timeout delay is exeeded, exit with error code */
        //if ((timeout--) == 0) return 0xFF;
    }  

    /* Prepare an NACK for the next data received */
    I2C_AcknowledgeConfig(I2C1, DISABLE);  

    /* Test on I2C1 EV7 and clear it */
    timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
        /* If the timeout delay is exeeded, exit with error code */
        //if ((timeout--) == 0) return 0xFF;
    }

    I2C_GenerateSTOP(I2C1, ENABLE);

    /* Receive the Data */
    Data = I2C_ReceiveData(I2C1);

    /* return the read data */
    return Data;
}
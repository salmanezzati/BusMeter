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

char Data_icon_up[310]={   //14*22
    14,22,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,1,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,1,1,1,1,1,1,1,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
char Data_icon_down[310]={   //14*22
    14,22,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,
	0,0,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,0,1,1,1,1,1,1,1,1,1,0,0,
	0,0,0,0,1,1,1,1,1,1,1,0,0,0,
	0,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,0,1,1,1,0,0,0,0,0,
	0,0,0,0,0,0,0,1,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
char Danger_icon[772]={   //26*22
    26,22,
	0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,
	0,0,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,
	0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0
};

char Massege_icon[772]={   //33*22
    35,22,
    0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
	0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,
	0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,
	0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,
	0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,
	0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,
	0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,
	0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,
	0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,
	0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
	0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0
};
char Location_icon[420]={   //19*22
    19,22,
	0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
	0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,
	0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,
	0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,
	0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,
	0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,
	0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,
	0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,
	0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0
};
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
const unsigned char Line_name[] = 
{
	0x46, 0x06, 0x27, 0x06, 0x45, 0x06, 0x20, 0x00,
    0x2E, 0x06, 0x37, 0x06, 0x20, 0x00, 0x00
};
const unsigned char Line_fare[] = 
{
	0x45, 0x06, 0x28, 0x06, 0x44, 0x06, 0x3A, 0x06, 0x20, 0x00, 
    0x2E, 0x06, 0x37, 0x06, 0x20, 0x00, 0x00  //0xA9, 0x06, 0x31, 0x06, 0x27, 0x06, 0xCC, 0x06, 0x47, 0x06, 0x20, 0x00, 0x00
};


unsigned char Key_str[10];
unsigned char Key[13]={0,1,2,3,4,5,6,7,8,9,0,11,12};
unsigned char Pass[5];
int D_sec, Usart_sec;
unsigned  char Code[4];
unsigned char Buffer_1[512], Buffer_2[512], L_Tx_1[250], L_Tx_2[250], L_Tx_3[250], L_Tx_4[250], F_Tx_1[250], F_Tx_2[250], F_Tx_3[250], F_Tx_4[250], Buffer_tx[500], Buffer_rx[500], Buffer_SMS[250], RF_buffer_ok_data[25];
unsigned char Show_phone=1, Phone_flag=0, Draw_arm_button=0, Show_sim808=0, Show_rc522=0;
unsigned char Show_daily_operation=0, Show_center_massage=0, Show_change_device_mode=0;
unsigned char Backgrond=0, Light_off=0;
unsigned char Sending_id[8][7];

unsigned char Buff_ID[7];
unsigned char Buff_Device_ID[7];
unsigned char Buff_Date[7];
unsigned char Buff_Time[7];
unsigned char Buff_Latitude[8];
unsigned char Buff_Longitude[8];
unsigned char Buff_Speed[9];
unsigned char Buff_Course[11];

//Recognized card ID
unsigned char Card_ID[5], Block_Addr, Card_detection=0, Access_Bits[4]={0xFF, 0x07, 0x80, 0x69};
unsigned char Sector_Key_1[6]={0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char Sector_Key_2[6];
unsigned char Block_Data_1[16]; 
unsigned char Block_Data_2[16]; 
unsigned char Block_Data_3[16]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char Block_Data_4[16];
unsigned long int Value_1=0, Value_2=0, Trip_price=50, Daily_operation_amount=0;
unsigned int Front_door_card=0, Back_door_card=0;
unsigned char Defined_card=0;

unsigned int Temp;
unsigned char Location_get_flag=0, Location_data_time=5, Data_send_flag=0, Data_time=30, Shop_data_send_flag=0;
unsigned int Location_write_block=5000, Location_send_block=5000, Financial_write_block=35000, Financial_send_block=35000;
unsigned int Location_write_block_end=30000, Financial_write_block_end=60000;
unsigned int Location_data_counter=0, Financial_data_counter=0;
unsigned char Location_Data_send_counter, Financial_Data_send_counter;
unsigned char Device_ID[7] = {'0','0','0','0','0','0','\0'};
unsigned char Device_mode=0;
unsigned char Purchase_price[9];
unsigned char Card_password[5];
unsigned char Peyment_status=0;
unsigned char Show_signal_quality_flag=1;
unsigned char GSM_Signal_quality=0, GPS_Signal_quality=0;

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
	unsigned int i=0, j=0;
	
	delay_ms(200);
	//Uart_Init(115200);
	GPIO_Initi();
    LCD_Init();
	
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
	
	strcpy (URL_1,"http://sebazar.ir/sebApi/dataDastgah/insertData.php?data=");
	//strcpy (URL_1,"http://mohammadsalehpour.ir/mas/Financial.php?data=");
	strcpy (URL_2,"http://sebazar.ir/Das/insertT2.php?data=");
	//strcpy (URL_2,"http://mohammadsalehpour.ir/mas/Financial.php?data=");
	
	Micro_SD_Write_Starting_Data();
	Micro_SD_Read_Starting_Data();
//***Sim808_Test******************************************
	//TIM10_Int_Init(3999,41999);
	Usart1_Init ();
	//TIM4_Int_Init(19,41999);
	//LCD_Backlight_ON
	Sim808_PowerOnOff();
	Sim808_Init();
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
	TIM8_Int_Init(1599,41999); //0.4 secound
	
    RTC_Start();
    Backgrond=1;
	//TIM7_Int_Init(19999,41999); //10 secound
	
	TIM10_Int_Init(3999,41999); //1 secound
	TIM12_Int_Init(799,41999); //0.2 secound
	TIM_GenerateEvent(TIM10,1);
    TIM1_Int_Init(1999,41999); //0.5 secound
	
    while(1)
    {
//		if(Show_daily_operation)
//		{
//			Backgrond = 1;
//			LCD_Clear(Good_White);
//				
//			Clear_Buffer(Buffer_1, 512);
//			sprintf(Buffer_1,"%03i", Front_door_card);
//			Button2(220, 180, 310, 230, LGRAY, 9, 16," ", Brand_Blue, 2, 16,Buffer_1, Brand_Blue);
//			myLCD_goto_xy(32, 183);
//			unicode2ascii((char *)Front_door, Buffer_1);
//			myLCD_set_font_color(Brand_Blue);
//			myLCD_font_effect(5);
//			myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
//			
//			Clear_Buffer(Buffer_1, 512);
//			sprintf(Buffer_1,"%03i", Back_door_card);
//			Button2(10, 180, 100, 230, LGRAY, 10, 16," ", Brand_Blue, 2, 16,Buffer_1, Brand_Blue);
//			myLCD_goto_xy(242, 183);
//			unicode2ascii((char *)Back_door, Buffer_1);
//			myLCD_set_font_color(Brand_Blue);
//			myLCD_font_effect(5);
//			myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
//			
//			Clear_Buffer(Buffer_1, 512);
//			sprintf(Buffer_1,"%06i", Daily_operation_amount);			
//			Button2(90, 80, 230, 130, LGRAY, 15, 16," ", Brand_Blue, 6, 16,Buffer_1, Brand_Blue);
//			myLCD_goto_xy(120, 83);
//			unicode2ascii((char *)Daily_operation, Buffer_1);
//			myLCD_set_font_color(Brand_Blue);
//			myLCD_font_effect(5);
//			myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
//			
//			Buzzer_ON
//			delay_ms(300);
//			Buzzer_OFF
//			
//			delay_ms(3000);
//			Show_daily_operation = 0;
//		}
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
//		else
//		{

//			if(Backgrond)
//			{
//				Backgrond=0;
//				
//				LCD_Clear(Good_White);
//				Massege_Icon(0);
//				Data_Icon_Up(0);
//				Data_Icon_Down(0);
//				
//				if(Financial_send_block == Financial_write_block)
//					Danger_Icon(0);
//				else
//					Danger_Icon(1);
//				
//				if(GPS_Signal_quality)
//					Location_Icon(1);
//				else
//					Location_Icon(0);
//				
//				Clear_Buffer(Buffer_1, 512);
//				sprintf(Buffer_1,"%03i", Front_door_card);
//				Button2(220, 180, 310, 230, LGRAY, 9, 16," ", Brand_Blue, 2, 16,Buffer_1, Brand_Blue);
//				myLCD_goto_xy(32, 183);
//				unicode2ascii((char *)Front_door, Buffer_1);
//				myLCD_set_font_color(Brand_Blue);
//				myLCD_font_effect(5);
//				myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
//				
//				Clear_Buffer(Buffer_1, 512);
//				sprintf(Buffer_1,"%03i", Back_door_card);
//				Button2(10, 180, 100, 230, LGRAY, 10, 16," ", Brand_Blue, 2, 16,Buffer_1, Brand_Blue);
//				myLCD_goto_xy(242, 183);
//				unicode2ascii((char *)Back_door, Buffer_1);
//				myLCD_set_font_color(Brand_Blue);
//				myLCD_font_effect(5);
//				myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
//				
////				myLCD_set_font_color(Brand_Blue);
////				myLCD_font_effect(5);
//				
//				myLCD_goto_xy(90, 125);
//				unicode2ascii((char *)Line_name, Buffer_1);
//				myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
//				
//				myLCD_goto_xy(200, 125);
//				unicode2ascii((char *)Line_Name, Buffer_1);
//				myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
//				
//				myLCD_goto_xy(90, 150);
//				unicode2ascii((char *)Line_fare, Buffer_1);
//				myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
//				
//				sprintf(Buffer_1,"%d", Trip_price);
//				LCD_MyShowString(90, 150, 200, 16, 16, Buffer_1,Brand_Blue, Good_White);
//				
//				
//				Show_time = 1;
//				Show_signal_quality_flag = 1;
//                //GPIO_ResetBits(GPIOB, GPIO_Pin_10);
//                //GPIO_SetBits(GPIOE, GPIO_Pin_5);
//			}
////---Time----------------------------
//			if(Show_time)
//			{
//				Show_time = 0;
//				sprintf(Buffer_1,"%02i:%02i", RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes/*, RTC_TimeStruct.RTC_Seconds*/);
//				LCD_MyShowString3 (107, 50, 48, Buffer_1,Brand_Blue, Good_White);
//				
//				sprintf(Buffer_1,"%02i/%02i/%02i", Sh_RTC_Date.RTC_Year/*-1300*/, Sh_RTC_Date.RTC_Month, Sh_RTC_Date.RTC_Date);
//				LCD_MyShowString(90, 85, 200, 16, 16, Buffer_1,Brand_Blue, Good_White);
//				
//				switch(M_RTC_DateStruct.RTC_WeekDay)
//				{
//					case 1: 
//						//sprintf(Buffer_1,"Sun");
//						myLCD_goto_xy(90, 85);
//						unicode2ascii((char *)Sun, Buffer_1);
//						break;
//					case 2: 
//						//sprintf(Buffer_1,"Mon");
//						myLCD_goto_xy(90, 85);
//						unicode2ascii((char *)Mon, Buffer_1);
//						break;
//					case 3: 
//						//sprintf(Buffer_1,"Tue");
//						myLCD_goto_xy(90, 85);
//						unicode2ascii((char *)Tue, Buffer_1);
//						break;
//					case 4: 
//						//sprintf(Buffer_1,"Wed");
//						myLCD_goto_xy(90, 85);
//						unicode2ascii((char *)Wed, Buffer_1);
//						break;
//					case 5: 
//						//sprintf(Buffer_1,"Thu");
//						myLCD_goto_xy(90, 85);
//						unicode2ascii((char *)Thu, Buffer_1);
//						break;
//					case 6: 
//						//sprintf(Buffer_1,"Fri");
//						myLCD_goto_xy(90, 85);
//						unicode2ascii((char *)Fri, Buffer_1);
//						break;
//					case 7: 
//						//sprintf(Buffer_1,"Sat");
//						myLCD_goto_xy(90, 85);
//						unicode2ascii((char *)Sat, Buffer_1);
//						break;
//				}
//				myLCD_set_font_color(Brand_Blue);
//				myLCD_font_effect(5);
//				myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
//				//LCD_MyShowString(193, 85, 40, 16, 16, Buffer_1,Brand_Blue, Good_White);
//			}
//---Setting whit SMS----------------------------			
			if(SMS_received_flag)
			{
				SMS_received_flag=0;
				//LCD_ShowStringWithSize(100, 16, 319, 16, 16, "SMS received", 12);
				//TIM_Cmd(TIM8, DISABLE);
				//TIM_Cmd(TIM10, DISABLE);
				//TIM_Cmd(TIM12, DISABLE);
				SMS_Read(SMS_num);
				
				SMS_Delete(DEL_ALL);
				
				//TIM_Cmd(TIM8, ENABLE);
				//TIM_Cmd(TIM10, ENABLE);
				//TIM_Cmd(TIM12, ENABLE);
				
				
			}
//        }
//---Get & save location data----------------------------		
		if(Location_get_flag)
		{
			Location_get_flag = 0;
			
			//TIM_Cmd(TIM8, DISABLE);
			//TIM_Cmd(TIM10, DISABLE);
			//TIM_Cmd(TIM12, DISABLE);
			GPS_Signal_quality = Get_GPS_Signal_Quality();
			//TIM_Cmd(TIM8, ENABLE);
			//TIM_Cmd(TIM10, ENABLE);
			//TIM_Cmd(TIM12, ENABLE);
			
			if(GPS_Signal_quality)
			{
				//GPS_Signal_quality = 0;
				Location_Icon(1);
				
				//TIM_Cmd(TIM8, DISABLE);
				//TIM_Cmd(TIM10, DISABLE);
				//TIM_Cmd(TIM12, DISABLE);
				GetInfo();
				//TIM_Cmd(TIM8, ENABLE);
				//TIM_Cmd(TIM10, ENABLE); 
				//TIM_Cmd(TIM12, ENABLE);
				
				if(strtold(Satellites_Used,NULL) >= 5)
				{
					Location_data_counter++;
					sprintf(Date,"%02i%02i%02i", (Sh_RTC_Date.RTC_Year % 100)/*-1300*/, Sh_RTC_Date.RTC_Month, Sh_RTC_Date.RTC_Date);
					sprintf(Time,"%02i%02i%02i", RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, RTC_TimeStruct.RTC_Seconds);
					
					Clear_Buffer(Buffer_1, 512);
					//sprintf(Buffer_1, "%05d%s%s%s%s%s", Location_data_counter, Device_ID, Date, Time, Latitude, Longitude, Speed, Course);
					sprintf(Buffer_1, "%06d%s%s%s%s%s%s%s", Location_data_counter, Device_ID, Date, Time, Latitude, Longitude, Speed, Course);
					
					do
					{
						SD_WriteSingleBlock(Buffer_1, Location_write_block);
						Clear_Buffer(Buffer_2, 512);
						SD_ReadSingleBlock(Buffer_2, Location_write_block);
						//LCD_ShowStringWithSize(0, 164, 319, 16, 16, Buffer_2, 80);
					}while(strstr(Buffer_1, Buffer_2) == '\0');
					
					Location_write_block++;
					if(Location_write_block > 30000)
						Location_write_block = 5000;
					
					Clear_Buffer(Buffer_1, 512);
					sprintf(Buffer_1, "%i", Location_data_counter);
					SD_WriteSingleBlock(Buffer_1, location_data_counter_addr);
					
					Clear_Buffer(Buffer_1, 512);
					sprintf(Buffer_1, "%i", Location_write_block);
					SD_WriteSingleBlock(Buffer_1, location_write_block_addr);
				}
				//LCD_ShowStringWithSize(0, 112, 319, 16, 16, Buffer_2, 120);
				
				if(RTC_TimeStruct.RTC_Minutes==00 && RTC_TimeStruct.RTC_Seconds<=40)
				{
					Set_time_flag = 1;
				}
			}
			else
			{
				Location_Icon(0);
			}
		}
//---Send financial & location data----------------------------		
		if(Data_send_flag)
		{
			Data_send_flag = 0;
			//---Location Data send---------
			Location_Data_send_counter = 0;
			while(Location_send_block != Location_write_block && Location_Data_send_counter <1)
			{
				Location_Data_send_counter++;
				
				Clear_Buffer(Buffer_1, 512);
				//strcpy (Buffer_1,"http://sebazar.ir/sebApi/dataDastgah/insertData.php?data=");
				strcpy (Buffer_1, URL_1);
				
				j=0;
				Clear_Buffer(L_Tx_1, 250);
				Clear_Buffer(L_Tx_2, 250);
				Clear_Buffer(L_Tx_3, 250);
				Clear_Buffer(L_Tx_4, 250);
				
				Clear_Buffer(F_Tx_1, 250);
				Clear_Buffer(F_Tx_2, 250);
				Clear_Buffer(F_Tx_3, 250);
				Clear_Buffer(F_Tx_4, 250);
					
				while((Location_send_block+j) != Location_write_block)
				{
					Clear_Buffer(Buffer_2, 512);
					SD_ReadSingleBlock(Buffer_2, (Location_send_block+j));
					
					for(i=0;i<6;i++)
						Sending_id[j][i] = Buffer_2[i];
					Sending_id[j][6] = 0;
					
					for(i=0;i<6;i++)
						Buff_ID[i] = Buffer_2[i];
					Buff_ID[i] = 0;
					
					for(i=0;i<6;i++)
						Buff_Device_ID[i] = Buffer_2[i+6];
					Buff_Device_ID[i] = 0;
					
					for(i=0;i<6;i++)
						Buff_Date[i] = Buffer_2[i+12];
					Buff_Date[i] = 0;
					
					for(i=0;i<6;i++)
						Buff_Time[i] = Buffer_2[i+18];
					Buff_Time[i] = 0;
					
					for(i=0;i<7;i++)
						Buff_Latitude[i] = Buffer_2[i+24];
					Buff_Latitude[i] = 0;
					
					for(i=0;i<7;i++)
						Buff_Longitude[i] = Buffer_2[i+31];
					Buff_Longitude[i] = 0;
					
					for(i=0;i<8;i++)
						Buff_Speed[i] = Buffer_2[i+38];
					Buff_Speed[i] = 0;
					
					for(i=0;i<10;i++)
						Buff_Course[i] = Buffer_2[i+46];
					Buff_Course[i] = 0;
					
					if(j == 0)
					{
						sprintf(L_Tx_1, "id_1=%s&device_id=%s&date=%s&time_1=%s&latitude_1=%s&longitude_1=%s&speed_1=%s&course_1=%s", Buff_ID, Buff_Device_ID, Buff_Date, Buff_Time, Buff_Latitude, Buff_Longitude, Buff_Speed, Buff_Course);
					}
					else if(j > 0 && j <= 1)
					{
						Clear_Buffer(Buffer_2, 512);
						sprintf(Buffer_2, "&id_%d=%s&time_%d=%s&latitude_%d=%s&longitude_%d=%s&speed_%d=%s&course_%d=%s", (j+1), Buff_ID, (j+1), Buff_Time, (j+1), Buff_Latitude, (j+1), Buff_Longitude, (j+1), Buff_Speed, (j+1), Buff_Course);
						strcat (L_Tx_1,Buffer_2);
					}
					else if(j > 1 && j <= 3)
					{
						Clear_Buffer(Buffer_2, 512);
						sprintf(Buffer_2, "&id_%d=%s&time_%d=%s&latitude_%d=%s&longitude_%d=%s&speed_%d=%s&course_%d=%s", (j+1), Buff_ID, (j+1), Buff_Time, (j+1), Buff_Latitude, (j+1), Buff_Longitude, (j+1), Buff_Speed, (j+1), Buff_Course);
						strcat (L_Tx_2,Buffer_2);
					}
					else if(j > 3 && j <= 5)
					{
						Clear_Buffer(Buffer_2, 512);
						sprintf(Buffer_2, "&id_%d=%s&time_%d=%s&latitude_%d=%s&longitude_%d=%s&speed_%d=%s&course_%d=%s", (j+1), Buff_ID, (j+1), Buff_Time, (j+1), Buff_Latitude, (j+1), Buff_Longitude, (j+1), Buff_Speed, (j+1), Buff_Course);
						strcat (L_Tx_3,Buffer_2);
					}
					else if(j > 5 && j <= 7)
					{
						Clear_Buffer(Buffer_2, 512);
						sprintf(Buffer_2, "&id_%d=%s&time_%d=%s&latitude_%d=%s&longitude_%d=%s&speed_%d=%s&course_%d=%s", (j+1), Buff_ID, (j+1), Buff_Time, (j+1), Buff_Latitude, (j+1), Buff_Longitude, (j+1), Buff_Speed, (j+1), Buff_Course);
						strcat (L_Tx_4,Buffer_2);
					}
					
					j++;
					if(j>=8 || (Location_send_block+j) > Location_write_block_end)
						break;
				}
				LCD_ShowStringWithSize(0, 132, 319, 16, 16, L_Tx_1, 160);
				//delay_ms(5000);
				
				//TIM_Cmd(TIM8, DISABLE);
				//TIM_Cmd(TIM10, DISABLE);
				//TIM_Cmd(TIM12, DISABLE);
				//HTTP_Upload(Buffer_1);
				HTTP_Post_Upload(L_Tx_1, L_Tx_2, L_Tx_3, L_Tx_4);
				//TIM_Cmd(TIM8, ENABLE);
				//TIM_Cmd(TIM10, ENABLE);
				//TIM_Cmd(TIM12, ENABLE);
				
				if(HTTP_sucsessful_upload)
				{
					HTTP_sucsessful_upload = 0;
					Location_send_block += j;
					if(Location_send_block > Location_write_block_end)
					{
						Location_send_block = 5000;
						Location_write_block_end = 30000;
						Clear_Buffer(Buffer_1, 512);
						sprintf(Buffer_1, "%i", Location_write_block_end);
						SD_WriteSingleBlock(Buffer_1, location_write_block_end_addr);
					}
					Clear_Buffer(Buffer_1, 512);
					sprintf(Buffer_1, "%i", Location_send_block);
					SD_WriteSingleBlock(Buffer_1, location_send_block_addr);
					
					Blink_2();
				}
			}
			//---Financial Data send---------
			Financial_Data_send_counter = 0;
			while(Financial_send_block != Financial_write_block && Financial_Data_send_counter <1)
			{
				Financial_Data_send_counter++;
				
				Clear_Buffer(Buffer_1, 512);
				//strcpy (Buffer_1,"http://sebazar.ir/sebApi/dataDastgah/v2/insertData.php?data=");
				strcpy (Buffer_1, URL_2);
				Buffer_1[40] = '0';
				
				j=0;
				while((Financial_send_block+j) != Financial_write_block)
				{
					Clear_Buffer(Buffer_2, 512);
					SD_ReadSingleBlock(Buffer_2, (Financial_send_block+j));
					
					for(i=0;i<5;i++)
						Sending_id[j][i] = Buffer_2[i];
					Sending_id[j][5] = 0;
					
					if(j>0)
					{
						for(i=5;i<30;i++)
							Buffer_2[i] = Buffer_2[i+12];
						
						Buffer_2[i] = 0;
					}
//					for(i=20;i<26;i++)
//						Date[i-20] = Buffer_2[i];
//					Date[6] = 0;
					
					
					strcat (Buffer_1,Buffer_2);
					
					j++;
					Buffer_1[40] = ('0' + j);
					if(j>=5 || (Financial_send_block+j) > Financial_write_block_end)
						break;
				}
				//LCD_ShowStringWithSize(0, 132, 319, 16, 16, Buffer_1, 160);
				
				//TIM_Cmd(TIM8, DISABLE);
				//TIM_Cmd(TIM10, DISABLE);
				//TIM_Cmd(TIM12, DISABLE);
				HTTP_Upload(Buffer_1);
				//TIM_Cmd(TIM8, ENABLE);
				//TIM_Cmd(TIM10, ENABLE);
				//TIM_Cmd(TIM12, ENABLE);
				
				if(HTTP_sucsessful_upload)
				{
					HTTP_sucsessful_upload = 0;
					Financial_send_block += j;
					if(Financial_send_block > Financial_write_block_end)
					{
						Financial_send_block = 35000;
						Financial_write_block_end = 60000;
						Clear_Buffer(Buffer_1, 512);
						sprintf(Buffer_1, "%i", Financial_write_block_end);
						SD_WriteSingleBlock(Buffer_1, financial_write_block_end_addr);
					}
					Clear_Buffer(Buffer_1, 512);
					sprintf(Buffer_1, "%i", Financial_send_block);
					SD_WriteSingleBlock(Buffer_1, financial_send_block_addr);
					
					Blink_2();
				}
			}
			
			if(Financial_send_block == Financial_write_block)
				Danger_Icon(0);
			else
				Danger_Icon(1);
			
			//sprintf(Buffer_1, "http://sebazar.ir/sebApi/dataDastgah/insertData.php?id=%02d&data1=%c&data2=2019&data3=%s&data4=%s%02d&data5=%02d", Data2, 'M', "salman", "DriverName", 25, Data1);
		
			//strcpy (Buffer_tx,"AT+CMGR=");
			//strcat (Buffer_tx,Temp);
		}
//---Send shop data----------------------------		
		if(Shop_data_send_flag)
		{
			Shop_data_send_flag = 0;
		
			Backgrond = 1;
			LCD_Clear(Good_White);
			//LCD_MyShowString(0, 48, 300, 16, 16, "Buy", WHITE, MAGENTA);
			myLCD_goto_xy(135, 40);
			unicode2ascii((char *)Buy, Buffer_1);
			myLCD_set_font_color(Brand_Blue);
			myLCD_font_effect(0);
			myLCD_putsf_mix((unsigned char*)Buffer_1, 2);
			
			//LCD_MyShowString(0, 64, 300, 16, 16, "Buy_amount:", WHITE, MAGENTA);
			myLCD_goto_xy(133, 90);
			unicode2ascii((char *)Buy_amount, Buffer_1);
			myLCD_set_font_color(Brand_Blue);
			myLCD_font_effect(5);
			myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
			
			Purchase_price_Get();
			
			//LCD_MyShowString(0, 96, 300, 16, 16, "Card_pass:", WHITE, MAGENTA);
			myLCD_goto_xy(132, 140);
			unicode2ascii((char *)Card_pass, Buffer_1);
			myLCD_set_font_color(Brand_Blue);
			myLCD_font_effect(5);
			myLCD_putsf_mix((unsigned char*)Buffer_1, 1);
			
			Card_Pass_Get();
			
			sprintf(Date, "%04i/%02i/%02i", Sh_RTC_Date.RTC_Year/*-1300*/, Sh_RTC_Date.RTC_Month, Sh_RTC_Date.RTC_Date);
			sprintf(Time, "%02i%02i%02i", RTC_TimeStruct.RTC_Hours, RTC_TimeStruct.RTC_Minutes, RTC_TimeStruct.RTC_Seconds);
			
			Clear_Buffer(Buffer_1, 512);
			sprintf(Buffer_1, "http://sebazar.ir/sebApi/dataDastgah/insertTarakonesh.php?card_id=%02x%02x%02x%02x%02x&device_id=%s&date=%s&time=%s&password=%s&purchase_price=%s", Card_ID[0], Card_ID[1], Card_ID[2], Card_ID[3], Card_ID[4], Device_ID, Date, Time, Card_password, Purchase_price);
			//LCD_ShowStringWithSize(0, 112, 319, 16, 16, Buffer_1, 160);
			//delay_ms(3000);
			
			myLCD_goto_xy(120, 190);
			unicode2ascii((char *)Data_sending, Buffer_2);
			myLCD_set_font_color(Brand_Blue);
			myLCD_font_effect(5);
			myLCD_putsf_mix((unsigned char*)Buffer_2, 1);
			
			//TIM_Cmd(TIM8, DISABLE);
			//TIM_Cmd(TIM10, DISABLE);
			//TIM_Cmd(TIM12, DISABLE);
			HTTP_Upload(Buffer_1);
			//TIM_Cmd(TIM8, ENABLE);
			//TIM_Cmd(TIM10, ENABLE);
			//TIM_Cmd(TIM12, ENABLE);
			
			if(HTTP_sucsessful_upload)
			{
				LCD_Clear(Good_White);
				if(Peyment_status == 1)
				{
					//LCD_ShowStringWithSize(0, 176, 319, 16, 16, "Successful_transaction", 40);
					myLCD_goto_xy(95, 130);
					unicode2ascii((char *)Successful_transaction, Buffer_1);
					myLCD_set_font_color(GREEN);
					myLCD_font_effect(5);
					myLCD_putsf_mix((unsigned char*)Buffer_1, 2);
					
					LCD_MyShow_Image(142,130,Ok_icon,GREEN,Good_White);
					
					Buzzer_ON
					delay_ms(300);
					Buzzer_OFF
				}
				if(Peyment_status == 2)
				{
					//LCD_ShowStringWithSize(0, 176, 319, 16, 16, "Password_error   ", 40);
					myLCD_goto_xy(100, 130);
					unicode2ascii((char *)Password_error, Buffer_1);
					myLCD_set_font_color(RED);
					myLCD_font_effect(5);
					myLCD_putsf_mix((unsigned char*)Buffer_1, 2);
					
					LCD_MyShow_Image(142,130,Error_icon,RED,Good_White);
					
					Buzzer_ON
					delay_ms(100);
					Buzzer_OFF
					delay_ms(50);
					Buzzer_ON
					delay_ms(100);
					Buzzer_OFF
				}
				if(Peyment_status == 3)
				{
					//LCD_ShowStringWithSize(0, 176, 319, 16, 16, "Low_cash          ", 40);
					myLCD_goto_xy(95, 130);
					unicode2ascii((char *)Low_cash, Buffer_1);
					myLCD_set_font_color(RED);
					myLCD_font_effect(5);
					myLCD_putsf_mix((unsigned char*)Buffer_1, 2);
					
					LCD_MyShow_Image(142,130,Error_icon,RED,Good_White);
					
					Buzzer_ON
					delay_ms(100);
					Buzzer_OFF
					delay_ms(50);
					Buzzer_ON
					delay_ms(100);
					Buzzer_OFF
				}
				delay_ms(2000);
				Peyment_status = 0;
				HTTP_sucsessful_upload = 0;
			}					
		}
//---Show signal quality----------------------------		
		if(Show_signal_quality_flag)
		{
			Show_signal_quality_flag = 0;
			Get_GSM_Signal_Quality();
		}	
    }
}
    
void Loading(void)
{
    unsigned char i;
    
    for(i=0;i<4;i++)
    {
        LCD_WriteReg(TS_INS_DISP_CTRL1,0x0123);
        delay_ms(10);
        LCD_Fill(125,85,155,115,Brand_Blue);
        LCD_Fill(165,85,195,115,GRAY);
        LCD_Fill(165,125,195,155,GRAY);
        LCD_Fill(125,125,155,155,GRAY);
        LCD_WriteReg(TS_INS_DISP_CTRL1,0x0133);
        delay_ms(100);
        
        LCD_WriteReg(TS_INS_DISP_CTRL1,0x0123);
        delay_ms(10);
        LCD_Fill(125,85,155,115,GRAY);
        LCD_Fill(165,85,195,115,Brand_Blue);
        LCD_Fill(165,125,195,155,GRAY);
        LCD_Fill(125,125,155,155,GRAY);
        LCD_WriteReg(TS_INS_DISP_CTRL1,0x0133);
        delay_ms(100);
        
        LCD_WriteReg(TS_INS_DISP_CTRL1,0x0123);
        delay_ms(10);
        LCD_Fill(125,85,155,115,GRAY);
        LCD_Fill(165,85,195,115,GRAY);
        LCD_Fill(165,125,195,155,Brand_Blue);
        LCD_Fill(125,125,155,155,GRAY);
        LCD_WriteReg(TS_INS_DISP_CTRL1,0x0133);
        delay_ms(100);
        
        LCD_WriteReg(TS_INS_DISP_CTRL1,0x0123);
        delay_ms(10);
        LCD_Fill(125,85,155,115,GRAY);
        LCD_Fill(165,85,195,115,GRAY);
        LCD_Fill(165,125,195,155,GRAY);
        LCD_Fill(125,125,155,155,Brand_Blue);
        LCD_WriteReg(TS_INS_DISP_CTRL1,0x0133);
        delay_ms(100);
    }
}

//void microSD_Test(void)
//{
//	 /* mount the filesystem */
//    if (f_mount(0, &filesystem) != FR_OK) 
//    {
//        LCD_ShowString(0,15, 300, 16, 16,"could not open filesystem");  
//    }
//    delay_ms(10);	

//    LCD_ShowString(0,15, 300, 16, 16,"Open a test file (message.txt)"); 
//    
//    ret = f_open(&file, "MESSAGE.TXT", FA_READ);
//    if (ret)
//    {
//        LCD_ShowString(0,30, 300, 16, 16,"not exist the test file (message.txt)"); 
//		//delay_ms(2000);
//    }
//    else 
//    {
//        LCD_ShowString(0,30, 300, 16, 16,"Type the file content");
//        for (;;) 
//        {
//            ret = f_read(&file, Buffer_1, sizeof(Buffer_1), &br);	/* Read a chunk of file */
//            if (ret || !br)
//            {
//                break;			/* Error or end of file */
//            }
//            Buffer_1[br] = 0;
//            LCD_ShowString(0,45, 300, 16, 16,Buffer_1);
//        }
//		//delay_ms(2000);
//		
//        if (ret) 
//        {
//            LCD_ShowString(0,60, 300, 16, 16,"Read the file error");
//            fault_err(ret);
//        }

//        LCD_ShowString(0,75, 300, 16, 16,"Close the file");
//        ret = f_close(&file);
//        if (ret) 
//        {
//            LCD_ShowString(0,90, 300, 16, 16,"Close the file error");
//        }
//    }

//    /*  hello.txt write test*/
//    delay_ms(50);	
//    LCD_ShowString(0,105, 300, 16, 16,"Create a new file (hello.txt)");
//    ret = f_open(&file, "HELLO.TXT", FA_WRITE | FA_CREATE_ALWAYS);
//    if (ret) 
//    {
//        LCD_ShowString(0,120, 300, 16, 16,"Create a new file error");
//        fault_err(ret);
//    } 
//    else 
//    {
//        LCD_ShowString(0,120, 300, 16, 16,"Write a text data. (hello.txt)");
//		//delay_ms(2000);
//        ret = f_write(&file, "Hello world! We can ofcurse.", 28, &bw);
//        if (ret) 
//        {
//            LCD_ShowString(0,135, 300, 16, 16,"Write a text data to file error");
//        } 
//        else 
//        {
//            sprintf(Buffer_1,"%u bytes written\n\r", bw);
//            LCD_ShowString(0,90, 300, 16, 16,Buffer_1);  
//        }
//        delay_ms(50);
//        LCD_ShowString(0,165, 300, 16, 16,"Close the file");
//        ret = f_close(&file);
//        if (ret) 
//        {
//            LCD_ShowString(0,180, 300, 16, 16,"Close the hello.txt file error");			
//        }
//    }

//    /*  hello.txt read test*/
//    delay_ms(50);	
//    LCD_ShowString(0,180, 300, 16, 16,"read the file (hello.txt)");
//    ret = f_open(&file, "HELLO.TXT", FA_READ);
//    if (ret) 
//    {
//        LCD_ShowString(0,195, 300, 16, 16,"open hello.txt file error");
//    } 
//    else 
//    {
//        LCD_ShowString(0,195, 300, 16, 16,"Type the file content(hello.txt)");
//        for (;;) 
//        {
//            ret = f_read(&file, Buffer_1, sizeof(Buffer_1), &br);	/* Read a chunk of file */
//            if (ret || !br) 
//            {
//                break;			/* Error or end of file */
//            }
//            Buffer_1[br] = 0;
//            LCD_ShowString(0,210, 300, 16, 16,Buffer_1);
//        }
//        if (ret) 
//        {
//            LCD_ShowString(0,225, 300, 16, 16,"Read file (hello.txt) error");
//            fault_err(ret);
//        }

//        LCD_ShowString(0,0, 300, 16, 16,"Close the file (hello.txt)");
//        ret = f_close(&file);
//        if (ret) 
//        {
//            LCD_ShowString(0,0, 300, 16, 16,"Close the file (hello.txt) error");
//        }
//    }

//    /*  directory display test*/
//    delay_ms(50);	
//    LCD_ShowString(0,15, 300, 16, 16,"Open root directory                   ");
//    ret = f_opendir(&dir, "");
//    if (ret) 
//    {
//        LCD_ShowString(0,30, 300, 16, 16,"Open root directory error");	
//    } 
//    else 
//    {
//        LCD_ShowString(0,30, 300, 16, 16,"Directory listing...                  ");
//        for (;;) 
//        {
//            ret = f_readdir(&dir, &fno);		/* Read a directory item */
//            if (ret || !fno.fname[0]) 
//            {
//                break;	/* Error or end of dir */
//            }
//            if (fno.fattrib & AM_DIR) 
//            {
//                sprintf(Buffer_1,"  <dir>  %s\n\r", fno.fname);
//                LCD_ShowString(0,45, 300, 16, 16,Buffer_1);
//            } 
//            else 
//            {
//                sprintf(Buffer_1,"%8lu  %s\n\r", fno.fsize, fno.fname);
//                LCD_ShowString(0,60, 300, 16, 16,Buffer_1);
//            }
//            delay_ms(100);
//        }
//        if (ret) 
//        {
//            LCD_ShowString(0,75, 300, 16, 16,"Read a directory error");
//            fault_err(ret);
//        }
//    }
//    delay_ms(50);	
//    LCD_ShowString(0,90, 300, 16, 16,"Test completed");
//	
////	Bmp_Show(0,0,path1,0);
////	delay_ms(2000);	
////	Bmp_Show(0,0,path2,0);
////	delay_ms(2000);
////	Bmp_Show(0,0,path3,0);
////	delay_ms(2000);	
////	Bmp_Show(0,0,path4,0);
////	delay_ms(2000);
//	
//	if (f_mount(0, &filesystem) != FR_OK) 
//    {
//        LCD_ShowString(0, 15, 300, 16, 16,"could not open filesystem"); 
//    }
//}
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
	SD_WriteSingleBlock(Device_ID, device_id_addr);
	
	SD_WriteSingleBlock(URL_1, url_1_addr);
	SD_WriteSingleBlock(URL_2, url_2_addr);
	
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 0);
	SD_WriteSingleBlock(Buffer_1, location_data_counter_addr);
	Location_data_counter = 0;
	
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 5000);
	SD_WriteSingleBlock(Buffer_1, location_write_block_addr);
	Location_write_block = 5000;
	
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 0);
	SD_WriteSingleBlock(Buffer_1, financial_data_counter_addr);
	Financial_data_counter = 0;
	
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 35000);
	SD_WriteSingleBlock(Buffer_1, financial_write_block_addr);
	Financial_write_block = 35000;
	
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 30000);
	SD_WriteSingleBlock(Buffer_1, location_write_block_end_addr);
	Location_write_block_end = 30000;

	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 5000);
	SD_WriteSingleBlock(Buffer_1, location_send_block_addr);
	Location_send_block = 5000;
	
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 60000);
	SD_WriteSingleBlock(Buffer_1, financial_write_block_end_addr);
	Financial_write_block_end = 60000;
	
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", 35000);
	SD_WriteSingleBlock(Buffer_1, financial_send_block_addr);
	Financial_send_block = 35000;
	
	Daily_operation_amount = 0;
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", Daily_operation_amount);
	SD_WriteSingleBlock(Buffer_1, daily_operation_amount_addr);

	Front_door_card = 0;
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", Front_door_card);
	SD_WriteSingleBlock(Buffer_1, front_door_card_addr);

	Back_door_card = 0;
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%i", Back_door_card);
	SD_WriteSingleBlock(Buffer_1, back_door_card_addr);
	
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%03i", Location_data_time);
	SD_WriteSingleBlock(Buffer_1, location_data_time_addr);
	
	Clear_Buffer(Buffer_1, 512);
	sprintf(Buffer_1, "%04i", Data_time);
	SD_WriteSingleBlock(Buffer_1, data_time_addr);
	
//	Clear_Buffer(Buffer_1, 512);
//	sprintf(Buffer_1, "%i", Trip_price);
//	SD_WriteSingleBlock(Buffer_1, trip_price_addr);
	
}
//---Read starting data----------------------
void Micro_SD_Read_Starting_Data(void)
{
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, device_id_addr);
	//LCD_ShowStringWithSize(0, 0, 319, 16, 16, Buffer_2, 6);
	strcpy (Device_ID, Buffer_2);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, line_code_addr);
	//LCD_ShowStringWithSize(0, 0, 319, 16, 16, Buffer_2, 6);
	strcpy (Line_Code, Buffer_2);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, line_name_addr);
	//LCD_ShowStringWithSize(0, 0, 319, 16, 16, Buffer_2, 6);
	strcpy (Line_Name, Buffer_2);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, url_1_addr);
	//LCD_ShowStringWithSize(0, 0, 319, 16, 16, Buffer_2, 6);
	strcpy (URL_1, Buffer_2);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, url_2_addr);
	//LCD_ShowStringWithSize(0, 0, 319, 16, 16, Buffer_2, 6);
	strcpy (URL_2, Buffer_2);
	
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
	SD_ReadSingleBlock(Buffer_2, financial_data_counter_addr);
	//LCD_ShowStringWithSize(0, 80, 319, 16, 16, Buffer_2, 5);
	Financial_data_counter = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, financial_write_block_addr);
	//LCD_ShowStringWithSize(0, 96, 319, 16, 16, Buffer_2, 5);
	Financial_write_block = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, financial_write_block_end_addr);
	//LCD_ShowStringWithSize(0, 112, 319, 16, 16, Buffer_2, 5);
	Financial_write_block_end = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, financial_send_block_addr);
	//LCD_ShowStringWithSize(0, 128, 319, 16, 16, Buffer_2, 5);
	Financial_send_block = strtold(Buffer_2,NULL);
	
	Clear_Buffer(Buffer_2, 512);
	SD_ReadSingleBlock(Buffer_2, trip_price_addr);
	//LCD_ShowStringWithSize(100, 0, 319, 16, 16, Buffer_2, 4);
	if(strtold(Buffer_2,NULL) > 0)
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
	Buffer_2[3] = Buffer_2[4];
	Buffer_2[4] = 0;
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

void Location_Icon(char Active_mode)
{
    if(Active_mode)
    {
        LCD_MyShow_Image(248,213,Location_icon,Brand_Blue,Good_White);
    }
    else
    {
        LCD_MyShow_Image(248,213,Location_icon,LGRAY,Good_White);
    }
}

void Massege_Icon(char Active_mode)
{
    if(Active_mode)
    {
        LCD_MyShow_Image(10,213,Massege_icon,Brand_Blue,Good_White);
    }
    else
    {
        LCD_MyShow_Image(10,213,Massege_icon,LGRAY,Good_White);
    }
}

void Danger_Icon(char Active_mode)
{
    if(Active_mode)
    {
        LCD_MyShow_Image(55,213,Danger_icon,Brand_Blue,Good_White);
    }
    else
    {
        LCD_MyShow_Image(55,213,Danger_icon,LGRAY,Good_White);
    }
}
void Data_Icon_Up(char Active_mode)
{
    if(Active_mode)
    {
        LCD_MyShow_Image(90,213,Data_icon_up,Brand_Blue,Good_White);
    }
    else
    {
        LCD_MyShow_Image(90,213,Data_icon_up,LGRAY,Good_White);
    }
}
void Data_Icon_Down(char Active_mode)
{
    if(Active_mode)
    {
        LCD_MyShow_Image(104,213,Data_icon_down,Brand_Blue,Good_White);
    }
    else
    {
        LCD_MyShow_Image(104,213,Data_icon_down,LGRAY,Good_White);
    }
}
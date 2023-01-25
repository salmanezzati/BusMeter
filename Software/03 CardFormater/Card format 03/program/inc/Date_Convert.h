
//www.radshid.com
 // کدهای زیر توسط مجموعه مهندسی رادشید گرداوری, ایجاد , تست و بصورت رایگان انتشار یافته است.
 
 

typedef struct DATE_E
 {
	unsigned long dtDay;
	unsigned long dtMonth;
	unsigned long dtYear;
} date;

typedef struct _time{
   char sec;     /* seconds after the minute - [0,59]    */
   char min;     /* minutes after the hour -   [0,59]    */
   char hour;    /* hours since midnight -     [0,23]    */
   char mday;    /* day of the month -         [1,31]    */
   char mon;     /* months since January -     [0,11]    */
   int  year;    /* years since                          */
}ATime;

void  M2Sh(int *j_y, int *j_m, int *j_d, int  g_y, int  g_m, int  g_d);// Mildai To Shamsi
void  Sh2M(int *g_y,int *g_m,int *g_d,int  j_y,int  j_m,int  j_d);// Shamsi To Mi 
int	DayOfWeek( int nYear, int nMonth, int nDay );// Rooze Hafte
void  M2Gh(int *year,int *month,int *Day,int d,int m,int y); //Miladi to ghamari;
void Binary2Time(unsigned long binary,ATime *datetime);// TimeStamp Unix To Mildai
unsigned long DateToBinary(int tm_year,int tm_mon,int tm_mday,int tm_hour,int tm_min,int tm_sec);	//Mildai To TimeStamp (Unix)


/* Return 0 ~ 365
 * 0~364 For Normal Year
 * 0~365 For Leap Year
 * */
int Day_Of_Year_Jalali(int  g_y, int  g_m, int  g_d); /*Return 0 To 365*/
unsigned int Day_Of_Year_Miladi(int tm_year,int tm_mon,int tm_mday);

// 0 means normal
// 1 means 1 days forward
// 2 means 1 days Back
date Georgian2Hijri(signed long d,signed long m,signed long y,char Type);

const int           DaysToMonth[13] =     {  0,31,59,90,120,151,181,212,243,273,304,334,365};
const unsigned int  g_days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const unsigned int  j_days_in_month[12] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};


int Day_Of_Year_Jalali(int  g_y, int  g_m, int  g_d) // 
{
          unsigned  long gy, gm, gd;
          //unsigned long jy, jm, jd;
          unsigned  long g_day_no, j_day_no;
          unsigned long j_np;
          unsigned long i;

          if(g_y<2000 || g_y>2100) g_y=2012;
          if(g_m==0   || g_m>12)   g_m=1;
          if(g_d==0   || g_d>31)   g_d=1;

             gy = g_y-1600;
             gm = g_m-1;
             gd = g_d-1;

             g_day_no = 365*gy+(gy+3)/4-(gy+99)/100+(gy+399)/400;
             for (i=0;i<gm;++i)
                g_day_no += g_days_in_month[i];
             if (gm>1 && ((gy%4==0 && gy%100!=0) || (gy%400==0)))
                /* leap and after Feb */
                ++g_day_no;
             g_day_no += gd;

             j_day_no = g_day_no-79;

             j_np = j_day_no / 12053;
             j_day_no %= 12053;

             //jy = 979+33*j_np+4*(j_day_no/1461);
             j_day_no %= 1461;

             if (j_day_no >= 366)
              {
                       // jy += (j_day_no-1)/365;
                        j_day_no = (j_day_no-1)%365;
             }

             return j_day_no;

}

    void M2Sh(int *j_y, int *j_m, int *j_d, int  g_y, int  g_m, int  g_d) // Miladi To shamsi
    {
            unsigned  long gy, gm, gd;
            unsigned long jy, jm, jd;
            unsigned  long g_day_no, j_day_no;
            unsigned long j_np;
            unsigned long i;
            
            if(g_y<2000 || g_y>2100) g_y=2012;
            if(g_m==0   || g_m>12)   g_m=1;
            if(g_d==0   || g_d>31)   g_d=1;

               gy = g_y-1600;
               gm = g_m-1;
               gd = g_d-1;

               g_day_no = 365*gy+(gy+3)/4-(gy+99)/100+(gy+399)/400;
               for (i=0;i<gm;++i)
                  g_day_no += g_days_in_month[i];
               if (gm>1 && ((gy%4==0 && gy%100!=0) || (gy%400==0)))
                  /* leap and after Feb */
                  ++g_day_no;
               g_day_no += gd;

               j_day_no = g_day_no-79;

               j_np = j_day_no / 12053;
               j_day_no %= 12053;

               jy = 979+33*j_np+4*(j_day_no/1461);
               j_day_no %= 1461;

               if (j_day_no >= 366)
                {
                          jy += (j_day_no-1)/365;
                          j_day_no = (j_day_no-1)%365;
               }

               for (i = 0; i < 11 && j_day_no >= j_days_in_month[i]; ++i)
               {
                          j_day_no -= j_days_in_month[i];
               }

               jm = i+1;
               jd = j_day_no+1;
               *j_y = jy;
               *j_m = jm;
               *j_d = jd;
    }


void Sh2M(int *g_y,int *g_m,int *g_d,int  j_y,int  j_m,int  j_d)// shamsi To Mildai
    {
       unsigned long gy, gm, gd;
       unsigned long jy, jm, jd;
       unsigned long g_day_no, j_day_no;
       unsigned long leap;

       unsigned long i;

       jy = j_y-979;
       jm = j_m-1;
       jd = j_d-1;

       j_day_no = 365*jy + (jy/33)*8 + (jy%33+3)/4;
       for (i=0; i < jm; ++i)
          j_day_no += j_days_in_month[i];

       j_day_no += jd;

       g_day_no = j_day_no+79;

       gy = 1600 + 400*(g_day_no/146097); /* 146097 = 365*400 + 400/4 - 400/100 + 400/400 */
       g_day_no = g_day_no % 146097;

       leap = 1;
       if (g_day_no >= 36525) /* 36525 = 365*100 + 100/4 */
       {
          g_day_no--;
          gy += 100*(g_day_no/36524); /* 36524 = 365*100 + 100/4 - 100/100 */
          g_day_no = g_day_no % 36524;

          if (g_day_no >= 365)
             g_day_no++;
          else
             leap = 0;
       }

       gy += 4*(g_day_no/1461); /* 1461 = 365*4 + 4/4 */
       g_day_no %= 1461;

       if (g_day_no >= 366) {
          leap = 0;

          g_day_no--;
          gy += g_day_no/365;
          g_day_no = g_day_no % 365;
       }

       for (i = 0; g_day_no >= g_days_in_month[i] + (i == 1 && leap); i++)
          g_day_no -= (unsigned long)g_days_in_month[i] + (i == 1 && leap);
       gm = i+1;
       gd = g_day_no+1;
	   
       *g_y = gy;
       *g_m = gm;
       *g_d = gd;
    }    


unsigned int Day_Of_Year_Miladi(int tm_year,int tm_mon,int tm_mday)  // 
{

    unsigned  long gy, gm, gd;

            unsigned  long g_day_no;

            unsigned long i;

            if(tm_year<2000 || tm_year>2100) tm_year=2012;
            if(tm_mon==0   || tm_mon>12)   tm_mon=1;
            if(tm_mday==0   || tm_mday>31)   tm_mday=1;

               gy = tm_year-1600;
               gm = tm_mon-1;
               gd = tm_mday-1;

               g_day_no = 365*gy+(gy+3)/4-(gy+99)/100+(gy+399)/400;
               for (i=0;i<gm;++i)
                  g_day_no += g_days_in_month[i];
               if (gm>1 && ((gy%4==0 && gy%100!=0) || (gy%400==0)))
                  /* leap and after Feb */
                  ++g_day_no;
               g_day_no += gd;

               gy = 1600 + 400*(g_day_no/146097); /* 146097 = 365*400 + 400/4 - 400/100 + 400/400 */
                      g_day_no = g_day_no % 146097;

                      if (g_day_no >= 36525) /* 36525 = 365*100 + 100/4 */
                      {
                         g_day_no--;
                         gy += 100*(g_day_no/36524); /* 36524 = 365*100 + 100/4 - 100/100 */
                         g_day_no = g_day_no % 36524;

                         if (g_day_no >= 365)
                            g_day_no++;

                      }

                      gy += 4*(g_day_no/1461); /* 1461 = 365*4 + 4/4 */
                      g_day_no %= 1461;

                      if (g_day_no >= 366) {


                         g_day_no--;
                         gy += g_day_no/365;
                         g_day_no = g_day_no % 365;
                      }

   return (int)g_day_no;
}

unsigned long DateToBinary(int tm_year,int tm_mon,int tm_mday,int tm_hour,int tm_min,int tm_sec) 
{

   unsigned long iday;
   unsigned long val;
    tm_year = tm_year-1900;
    
   iday = 365 * (tm_year - 70) + DaysToMonth[tm_mon-1] + (tm_mday - 1);
   iday = iday + (tm_year - 69) / 4;
   if ((tm_mon > 2) && ((tm_year % 4) == 0)) {
      iday++;
   }
   val = tm_sec + 60 * tm_min + 3600 * (tm_hour + 24 * iday);
   return val;
}
    
void Binary2Time(unsigned long binary,ATime *datetime)
    {

   unsigned long hour;
   unsigned long day;
   unsigned long minute;
   unsigned long second;
   unsigned long month;
   unsigned long year;
     
   unsigned long whole_minutes;
   unsigned long whole_hours;
   unsigned long whole_days;
   unsigned long whole_days_since_1968;
   unsigned long leap_year_periods;
   unsigned long days_since_current_lyear;
   unsigned long whole_years;
   unsigned long days_since_first_of_year;
   unsigned long days_to_month;
   /*unsigned long day_of_week;*/
       
   whole_minutes = binary / 60;
   second = binary - (60 * whole_minutes);                 /* leftover seconds*/

   whole_hours  = whole_minutes / 60;
   minute = whole_minutes - (60 * whole_hours);            /* leftover minutes*/

   whole_days   = whole_hours / 24;
   hour         = whole_hours - (24 * whole_days);        /* leftover hours*/
        
   whole_days_since_1968 = whole_days + 365 + 366;
   leap_year_periods = whole_days_since_1968 / ((4 * 365) + 1);

   days_since_current_lyear = whole_days_since_1968 % ((4 * 365) + 1);
        
   /* if days are after a current leap year then add a leap year period*/
   if ((days_since_current_lyear >= (31 + 29))) {
      leap_year_periods++;
   }
   whole_years = (whole_days_since_1968 - leap_year_periods) / 365;
   days_since_first_of_year = whole_days_since_1968 - (whole_years * 365) - leap_year_periods;

   if ((days_since_current_lyear <= 365) && (days_since_current_lyear >= 60)) {
      days_since_first_of_year++;
   }
   year = whole_years + 68;        

   /* setup for a search for what month it is based on how many days have past*/
   /*   within the current year*/
   month = 13;
   days_to_month = 366;
   while (days_since_first_of_year < days_to_month) {
       month--;
       days_to_month = DaysToMonth[month-1];
       if ((month > 2) && ((year % 4) == 0)) {
           days_to_month++;
        }
   }
   day = days_since_first_of_year - days_to_month + 1;

   /*day_of_week = (whole_days  + 4) % 7;*/

   /*datetime->tm_yday = days_since_first_of_year;      // days since January 1 - [0,365]       */
   datetime->sec  = second;        /* seconds after the minute - [0,59]    */
   datetime->min  = minute;        /* minutes after the hour - [0,59]      */
   datetime->hour = hour;          /* hours since midnight - [0,23]        */
   datetime->mday = day;           /* day of the month - [1,31]            */
  /*datetime->tm_wday = day_of_week;   // days since Sunday - [0,6]            */
   datetime->mon  = month;         /* months since January - [0,11]        */
   datetime->year = year+1900;          /* years since 1900                     */
}

void M2Gh(int *year,int *month,int *Day,int y,int m,int d)//Miladi to ghamari
{
	unsigned long jd,l,j,n;   
        
	
	if ((y>1582)||((y==1582)&&(m>10))||((y==1582)&&(m==10)&&(d>14)))
		jd=(1461*(y+4800+(m-14)/12))/4+(367*(m-2-12*((m-14)/12)))/12-(3*((y+4900+(m-14)/12)/100))/4+d-32075;

	else jd = 367*y-(7*(y+5001+(m-9)/7))/4+(275*m)/9+d+1729777;
        
						l=jd-1948440+10632;
                                                
					n=(l-1)/10631;
					l=l-10631*n+354;
					j=((10985-l)/5316)*((50*l)/17719)+(l/5670)*((43*l)/15238);
					l=l-((30-j)/15)*((17719*j)/50)-(j/16)*((15238*j)/43)+29; 
                                        
                                        
//					date1.dtMonth= m=(24*l)/709;
//					date1.dtDay=d=l-(709*m)/24;
//					date1.dtYear=y=30*n+j-30;   
                                        
                                        *month=m=(24*l)/709;
					*year=y=30*n+j-30;
                                        *Day=d=l-(709*m)/24;
					
}



const int	pnDaysBeforeMonth[] =
{
		 0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};
        
//   flash char	aszWeekDayNames[][16] = {
//		"Sunday", "Monday", "Tuesday", "Wednesday", "Thursay", "Friday", "Saturday"
//	};


int		CountOfFeb29( int nYear )
{
	int		nCount = 0;
	if( nYear > 0 ) {
		nCount = 1;	/* Year 0 is a leap year */
		nYear--;/* Year nYear is not in the period */
	}
	nCount += nYear / 4 - nYear / 100 + nYear / 400;

	return nCount;
}

char	IsLeapYear( int nYear )
{
	if( nYear % 4 != 0 )	return 0;
	if( nYear % 100 != 0 )	return 1;
	return ( nYear % 400 == 0 );
}


int	DayOfWeek( int nYear, int nMonth, int nDay )
{
	int			nDayOfWeek;
	

	nDayOfWeek = 6 + nYear % 7 + CountOfFeb29(nYear) % 7 + 14;/* + 14 : makes nDayOfWeek >= 0 */
	

	nDayOfWeek += pnDaysBeforeMonth[ nMonth ];
	if( nMonth > 2 && IsLeapYear(nYear) )	nDayOfWeek++;
	

	nDayOfWeek += nDay - 1;
	nDayOfWeek %= 7;
        
        if(nDayOfWeek==0)   return 0;
        if(nDayOfWeek==1)   return 1;
        if(nDayOfWeek==2)   return 2;
        if(nDayOfWeek==3)   return 3;
        if(nDayOfWeek==4)   return 4;
        if(nDayOfWeek==5)   return 5;
        if(nDayOfWeek==6)   return 6;
        
        
	
	return 1;
}


date GEtoHI(signed long d,signed long m,signed long y)
{
	unsigned long jd,l,j,n;
	date date1;

	if ((y>1582)||((y==1582)&&(m>10))||((y==1582)&&(m==10)&&(d>14)))
		jd=(1461*(y+4800+(m-14)/12))/4+(367*(m-2-12*((m-14)/12)))/12-(3*((y+4900+(m-14)/12)/100))/4+d-32075;
	else jd = 367*y-(7*(y+5001+(m-9)/7))/4+(275*m)/9+d+1729777;

						l=jd-1948440+10632;

					n=(l-1)/10631;
					l=l-10631*n+354;
					j=((10985-l)/5316)*((50*l)/17719)+(l/5670)*((43*l)/15238);
					l=l-((30-j)/15)*((17719*j)/50)-(j/16)*((15238*j)/43)+29;


					date1.dtMonth= m=(24*l)/709;
					date1.dtDay=d=l-(709*m)/24;
					date1.dtYear=y=30*n+j-30;

return date1;

}

date Georgian2Hijri(signed long d,signed long m,signed long y,char Type)
{
	  // 0 means normal
	  // 1 means 1 days forward
	  // 2 means 1 days Back

	date dateN,dateF,dateP,DataAnswer;

	dateN=GEtoHI(d,m,y);
	dateF=GEtoHI(d+1,m,y);
	dateP=GEtoHI(d-1,m,y);


	DataAnswer=dateN;

	if(Type==1) //Forward
	{
		DataAnswer=dateF;
		if(dateN.dtDay==29 && dateF.dtDay==1)
		{
			DataAnswer=dateN;
			DataAnswer.dtDay=30;
		}
	}

	if(Type==2) //Previuse
	{
		DataAnswer=dateP;
		if(dateN.dtDay==1 && dateP.dtDay==29)
		{
				DataAnswer.dtDay=30;
		}
	}

	return DataAnswer;
}

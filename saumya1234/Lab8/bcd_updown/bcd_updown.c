#include <LPC17xx.h>
 #include <stdio.h>

 #define	FIRST_SEG	0xFFF87FFF
 #define	SECOND_SEG	0xFFF8FFFF
 #define	THIRD_SEG	0xFFF97FFF
 #define	FOURTH_SEG	0xFFF9FFFF
 #define	DISABLE_ALL 0xFA7FFFFF

 unsigned int dig1=0x00,dig2=0x00,dig3=0x00,dig4=0x00;
 unsigned int twenty_count = 0x00,dig_count=0x00,temp1=0x00;
 unsigned char array_dec[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
 unsigned char tmr0_flg = 0x00,one_sec_flg = 0x00;
 unsigned long int temp2 = 0x00000000,i=0;
 unsigned int temp3=0x00;

 void delay(void);
 void Display(void);

 int main(void)
 {    
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 &= 0x3F0000FF;	 //P0.4 to P0.11 GPIO data lines
	LPC_PINCON->PINSEL1 &= 0xFFFFFFC0;	 //P1.23 to P1.26 GPIO enable lines

	LPC_GPIO0->FIODIR |=
	 0x00078FF0;	//P0.4 to P0.11 output
	//LPC_GPIO1->FIODIR |= 0x07800000;	//P1.23 to P1.26 output
	 
while(1)
	{
		delay();
			//LPC_GPIO0->FIOMASK=0X00078000;
if(!(LPC_GPIO0->FIOPIN & (1<<21)))
{
	delay();

		dig_count +=1;  
		if(dig_count == 0x05)
            dig_count = 0x00;

		if(one_sec_flg == 0xFF)
		{
 			one_sec_flg = 0x00;
			dig1 -=1;
              
			if(dig1 == -1)
			{ 
				dig1 = 9;
				dig2 -=1;

				if(dig2 == -1)
				{
					dig2 = 9;
					dig3-=1;

					if(dig3 == -1)
					{
						dig3 = 9;
						dig4 -= 1;
                             
						if(dig4 == -1)
						{
							dig4 = 9;
						} //end of dig4
					
					} //end of dig3
				
				} //end of dig2

			}  //end of dig1

		}   //end of one_sec if

 		Display();	
}
else{
	delay();
		dig_count +=1;  
		if(dig_count == 0x05)
            dig_count = 0x00;

		if(one_sec_flg == 0xFF)
		{
 			one_sec_flg = 0x00;
			dig1 +=1;
              
			if(dig1 == 0x0A)
			{ 
				dig1 = 0;
				dig2 +=1;

				if(dig2 == 0x0A)
				{
					dig2 = 0;
					dig3+=1;

					if(dig3 == 0x0A)
					{
						dig3 = 0;
						dig4 += 1;
                             
						if(dig4 == 0x0A)
						{
							dig4 = 0;
						} //end of dig4
					
					} //end of dig3
				
				} //end of dig2

			}  //end of dig1

		}   //end of one_sec if

 		Display();	
	}
	}
	}
	void delay(void)
 {
	 long int i=0;
	 for (i=0;i<500
	 ;i++);
		if(twenty_count ==300)     //multiplied by 500x2msec for 1 Sec
	{ 
		one_sec_flg = 0xFF;
		twenty_count = 0x00;			 
	}
	else twenty_count += 1;
	
 }
void Display(void)      //To Display on 7-segments
 {

        
	if(dig_count == 0x01)		// For Segment U8
	{
		LPC_GPIO0->FIOMASK=0X00000FF0;
		temp1 = dig1; 
		LPC_GPIO0->FIOPIN = FIRST_SEG;    

	}

	else if(dig_count == 0x02)	// For Segment U9
	{
		LPC_GPIO0->FIOMASK=0X00000FF0;
		temp1 = dig2;             
		LPC_GPIO0->FIOPIN = SECOND_SEG;    
   
	}

	else if(dig_count == 0x03)	// For Segment U10
	{
		LPC_GPIO0->FIOMASK=0X00000FF0;
		temp1 = dig3;           
		LPC_GPIO0->FIOPIN = THIRD_SEG;    
	}
	else if(dig_count == 0x04)	// For Segment U11
	{ 
		LPC_GPIO0->FIOMASK=0X00000FF0;
		temp1 = dig4;   
		LPC_GPIO0->FIOPIN = FOURTH_SEG;    

	}
	temp1 &= 0x0F;
	temp2 = array_dec[temp1];	// Decoding to 7-segment
	temp2 = temp2 << 4;
	LPC_GPIO0->FIOMASK=0X00078000;
	LPC_GPIO0->FIOPIN = temp2;		// Taking Data Lines for 7-Seg
	for(i=0;i<500;i++);
	LPC_GPIO0->FIOCLR = 0x00000FF0;
//	LPC_GPIO1->FIOPIN = DISABLE_ALL; //disable all the segments
	
 }		

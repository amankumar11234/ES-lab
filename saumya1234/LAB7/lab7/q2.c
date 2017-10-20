 //BCD up/down counter
 #include <LPC17xx.h>
 #include <stdio.h>

 #define	FIRST_SEG	0xF87FFFFF
 #define	SECOND_SEG	0xF8FFFFFF
 #define	THIRD_SEG	0xF97FFFFF
 #define	FOURTH_SEG	0xF9FFFFFF
 #define	DISABLE_ALL 0xFA7FFFFF

 unsigned int dig1=0x00,dig2=0x00,dig3=0x00,dig4=0x00;
 unsigned int twenty_count = 0x00,dig_count=0x00,temp1=0x00;
 unsigned char array_dec[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
 unsigned char tmr0_flg = 0x00,one_sec_flg = 0x00;
 unsigned long int temp2 = 0x00000000,i=0;
 unsigned int temp3=0x00;

 void TIMER0_Init(void);
 void TIMER0_IRQHandler(void);
 void Display(void);

 int main(void)
 {    
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 &= 0xFF0000FF;	 //P0.4 to P0.11 GPIO data lines
	LPC_PINCON->PINSEL3 &= 0xFFC03FFF;	 //P1.23 to P1.26 GPIO enable lines
  LPC_TIM0->CTCR=0;
	LPC_TIM0->PR=02;
  LPC_TIM0->MR0=499999;
	LPC_TIM0->TCR=0X2;
	LPC_TIM0->MCR=0X2;
  LPC_TIM0->EMR=0X20;
  LPC_TIM0->TCR=0X1;
	LPC_GPIO0->FIODIR |= 0x00000FF0;	//P0.4 to P0.11 output
	LPC_GPIO1->FIODIR |= 0x07800000;	//P1.23 to P1.26 output
	 
while(1)
	{
		while(!LPC_TIM0->EMR&0X01);
		 LPC_TIM0->EMR=0X20;
		 
		dig_count +=1;  
		if(dig_count == 0x05)
            dig_count = 0x00;
		
		
		
			one_sec_flg = 0x00;
			
			if (!(LPC_GPIO0->FIOPIN & (1<<21)))
			{
				
				dig1 -=1;
              
				if(dig1 == 0xFFFFFFFF)
				{ 
					dig1 = 9;
					dig2 -=1;

					if(dig2 == 0xFFFFFFFF)
					{
						dig2 = 9;
						dig3-=1;

						if(dig3 == 0xFFFFFFFF)
						{
							dig3 = 9;
							dig4 -= 1;
															 
							if(dig4 == 0xFFFFFFFF)
							{
								dig4 = 9;
							} //end of dig4
						
						} //end of dig3
					
					} //end of dig2

				}  //end of dig1
				
			}
			
			else
			{
				dig1 +=1;
              
				if(dig1 == 0xA)
				{ 
					dig1 = 0;
					dig2 +=1;

					if(dig2 == 0xA)
					{
						dig2 = 0;
						dig3+=1;

						if(dig3 == 0xA)
						{
							dig3 = 0;
							dig4 += 1;
															 
							if(dig4 == 0xA)
							{
								dig4 = 0;
							} //end of dig4
						
						} //end of dig3
					
					} //end of dig2

				}  //end of dig1

				
				
			}

 			
			
			
		   //end of one_sec if

 		Display();	
	}
}
void Display(void)      //To Display on 7-segments
 {

        
	if(dig_count == 0x01)		// For Segment U8
	{
		temp1 = dig1; 
		LPC_GPIO1->FIOPIN = FIRST_SEG;    

	}

	else if(dig_count == 0x02)	// For Segment U9
	{
		temp1 = dig2;             
		LPC_GPIO1->FIOPIN = SECOND_SEG;    
   
	}

	else if(dig_count == 0x03)	// For Segment U10
	{
		temp1 = dig3;           
		LPC_GPIO1->FIOPIN = THIRD_SEG;    
	}
	else if(dig_count == 0x04)	// For Segment U11
	{ 
		temp1 = dig4;   
		LPC_GPIO1->FIOPIN = FOURTH_SEG;    

	}
	temp1 &= 0x0F;
	temp2 = array_dec[temp1];	// Decoding to 7-segment
	temp2 = temp2 << 4;
	LPC_GPIO0->FIOPIN = temp2;		// Taking Data Lines for 7-Seg
	for(i=0;i<500;i++);
	LPC_GPIO0->FIOCLR = 0x00000FF0;
//	LPC_GPIO1->FIOPIN = DISABLE_ALL; //disable all the segments
	one_sec_flg++;
 }		

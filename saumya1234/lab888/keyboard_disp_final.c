#include <LPC17xx.h>
#include <stdio.h>

 #define	FIRST_SEG	0xFFF87FFF
 #define	SECOND_SEG	0xFFF8FFFF
 #define	THIRD_SEG	0xFFF97FFF
 #define	FOURTH_SEG	0xFFF9FFFF
 #define	DISABLE_ALL 0xFA7FFFFF

 int dig1=0xF,dig2=0x0F,dig3=0x0F,dig4=0x0F;
 unsigned int dig_count=0x00,temp1=0x00;
 unsigned char array_dec[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x73,0x00,0x00,0x00,0x00,0x00};
 unsigned char one_sec_flg = 0x00;
 unsigned long int temp2 = 0x00000000,i=0;
 unsigned int temp3=0x00;
 unsigned int maskdecoder=0x00078000;
 unsigned int masknum=0x00000FF0;
 
 unsigned int flag=0x00;
 unsigned int entry=0x00;
 unsigned int row=0x00, col=0x00;
 
 
unsigned int opd1=0x00;
unsigned int opr=0x00;
unsigned int opd2=0x00;
 
 void scan(void)
{
	temp3=LPC_GPIO1->FIOPIN;
	temp3&=0x07800000;
	if (temp3!=0x00)
	{
		flag=1;
		if (temp3==0x00800000)
			col=0;
		if (temp3==0x01000000)
			col=1;
		if (temp3==0x02000000)
			col=2;
		if (temp3==0x04000000)
			col=3;

		
		for (i=0; i<100000; i++);
	}
}

 
void calc(void)
{
	 if (entry==0)
	 {
		 opd1=row*4+col;
		 dig1=opd1;
		 dig2=0xF;
		 dig3=0xF;
		 dig4=0xF;
		 
		 entry=1;
	 }
	 else if (entry==1)
	 {
		 dig1=0xA;
		 dig2=opd1;
		 dig3=0xF;
		 dig4=0xF;
		 
		 entry=2;
	 }
	 else if (entry==2)
	 {
		 opd2=row*4+col;
		 dig1=opd2;
		 dig2=0xA;
		 dig3=opd1;
		 dig4=0xF;
		 
		 entry=3;
	 }
	 else if (entry==3)
	 {
		 int s=opd1+opd2;
		 dig1=s%10;
		 if (s>9)
			 dig2=1;
		 else
			 dig2=0xF;
		 dig3=0xF;
		 dig4=0xF;
		 
		 entry=0;
	 }
}


void display(void)      //To Display on 7-segments
 {

  LPC_GPIO0->FIOMASK= masknum;
	if(dig_count == 0x01)		// For Segment U8
	{
		temp1 = dig1; 
		LPC_GPIO0->FIOPIN = FIRST_SEG;    }

	else if(dig_count == 0x02)	// For Segment U9
	{
		temp1 = dig2;             
		LPC_GPIO0->FIOPIN = SECOND_SEG;    	}

	else if(dig_count == 0x03)	// For Segment U10
	{
		temp1 = dig3;           
		LPC_GPIO0->FIOPIN = THIRD_SEG;    
	}
	else if(dig_count == 0x04)	// For Segment U11
	{ 
		temp1 = dig4;   
		LPC_GPIO0->FIOPIN = FOURTH_SEG;    

	}
	temp1 &= 0x0F;
	temp2 = array_dec[temp1];	// Decoding to 7-segment
	temp2 = temp2 << 4;
	LPC_GPIO0->FIOMASK= maskdecoder;
	LPC_GPIO0->FIOPIN = temp2;		// Taking Data Lines for 7-Seg
	for(i=0;i<500;i++);
	LPC_GPIO0->FIOCLR = 0x00000FF0;
/*	LPC_GPIO1->FIOPIN = DISABLE_ALL; //disable all the segments */
	one_sec_flg++;
 }		

 
 
 int main(void)
 {    
	SystemInit();
	SystemCoreClockUpdate();
	 
	LPC_PINCON->PINSEL0 = 0x0;
	LPC_PINCON->PINSEL1 = 0x0;
	LPC_PINCON->PINSEL3 = 0x0;
	LPC_PINCON->PINSEL4 = 0x0;
	
	LPC_GPIO0->FIODIR = 0x00078FF0;
	LPC_GPIO1->FIODIR = 0x00000000;
	LPC_GPIO2->FIODIR = 0xFFFFFFFF;
	 
	 

	
	 
while(1)
	{
		for (row=0; row<4; row++)
		{
			if (row==0)
				temp1=0x00000400;
			else if (row==1)
				temp1=0x00000800;
			else if (row==2)
				temp1=0x00001000;
			else if (row==3)
				temp1=0x00002000;
			LPC_GPIO2->FIOPIN=temp1;
			flag=0;
			scan();
			if (flag==1)
			{
				calc();
				break;
			}
		}
		
		dig_count++;
		if (dig_count>4)
			dig_count=1;

 		display();
		
		
		
	}
}


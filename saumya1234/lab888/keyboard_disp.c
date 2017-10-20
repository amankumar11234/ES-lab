#include <LPC17xx.h>
 #include <stdio.h>

 #define	FIRST_SEG	0xF87FFFFF
 #define	SECOND_SEG	0xF8FFFFFF
 #define	THIRD_SEG	0xF97FFFFF
 #define	FOURTH_SEG	0xF9FFFFFF
 #define	DISABLE_ALL 0xFA7FFFFF
unsigned char col,row,var,flag,key,*ptr;
unsigned long int i,var1,temp,temp3;
 unsigned int dig1=0x00,dig2=0x00,dig3=0x00,dig4=0x00;
 unsigned int twenty_count = 0x00,dig_count=0x00,temp1=0x00;
 unsigned char array_dec[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
 unsigned char tmr0_flg = 0x00,one_sec_flg = 0x00;
 unsigned long int temp2 = 0x00000000,i=0;
	unsigned int x=0,y=0,dig=0;
 char oper=NULL;
void scan(void);
 void delay(void);
 void Display(void);
 int main(void)
 {    
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 = 0xFF0000FF;	 //P0.4 to P0.11 GPIO data lines
	LPC_PINCON->PINSEL3 = 0xFFC03FFF;	 //P1.23 to P1.26 GPIO enable lines
	LPC_PINCON->PINSEL5 = 0xF00FFFFF;
	LPC_GPIO2->FIODIR |=0x000000003;
	LPC_GPIO0->FIODIR |= 0x1800FFFF;	//P0.4 to P0.11 output
	LPC_GPIO1->FIODIR |= 0x07800000;	//P1.23 to P1.26 output
	 
while(1)
	{
		while(1)
		{
		
		for(row=0;row<4;row=row+1)
		{
			if(row==0)
				var1=0x08000000;
			else if(row==1)
				var1=0x10000000;
			else if(row==2)
				var1=0x00000001;
			else if(row==3)
				var1=0x00000002;
			
		

		temp=var1;
		LPC_GPIO0->FIOCLR=0x00003C00;
		LPC_GPIO2->FIOCLR=0X00000003;
		if(row==2||row==3)
			LPC_GPIO2->FIOSET=var1;
		if(row==0||row==1)		
			LPC_GPIO0->FIOSET=var1;
		flag=0;
		scan();
		if(flag==1)
		{	
			Display();
			
			break;
			
		}
	 }
	 
	 if(flag==1)
	 {flag=0;
		 break;}
		
	}
 		
	}
	}
		void scan(void)
	{
		unsigned long temp3;
		temp3= LPC_GPIO0->FIOPIN;
		temp3&=0x07800000;
		if(temp3!=0x00)
		{
			flag=1;
			if(temp3==0x00800000)
				col=0;
			if(temp3==0x01000000)
				col=1;
			if(temp3==0x02000000)
				col=2;
			if(temp3==0x04000000)
				col=3;
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
	
void Display()      //To Display on 7-segments
 {dig_count=0x00;
	delay();
	 dig3=row;
	 dig1=col;
  while(dig_count<=0x04)
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
	dig_count=dig_count+1;}
	if(dig_count==0x05)
		dig_count=0x00;
//	LPC_GPIO1->FIOPIN = DISABLE_ALL; //disable all the segments
	
 }		

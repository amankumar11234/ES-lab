#include <LPC17xx.h>
#include <stdio.h>


 unsigned int hr=0,min=0;
 int i,j, arr1[4];
 unsigned char arr[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x37,0x7B,0x39,0x5E,0x79,0x71};
  unsigned long int a[5] = {0xF87FFFFF,0xF8FFFFFF,0xF97FFFFF,0xF9FFFFFF,0xFA7FFFFF};
 void TIMER0_Init(void);
 void TIMER0_IRQHandler(void);
 void display(void);
 
 int main(void)
 {    

	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 &= 0xFF0000FF;	 //P0.4 to P0.11 GPIO data lines
	LPC_PINCON->PINSEL3 &= 0xFFC03FFF;	 //P1.23 to P1.26 GPIO enable lines

	LPC_GPIO0->FIODIR |= 0x00000FF0;	//P0.4 to P0.11 output
	LPC_GPIO1->FIODIR |= 0x07800000;	//P1.23 to P1.26 output
	LPC_TIM0->CTCR = 0;
	LPC_TIM0->PR=2;
	LPC_TIM0->TCR=2;
	LPC_TIM0->MR0=999999;
	LPC_TIM0->MCR=2;
	LPC_TIM0->EMR=0X20;
	LPC_TIM0->TCR=1;
 
while(1)
	{
		while(!(LPC_TIM0->EMR&1))
			display();
		LPC_TIM0->EMR = 0X20;
		min++;
		if (min==60)
		{
			min = 0;
			hr++;
			if(hr==60)
			{
				hr=0;
			}
		}		
	}
	
}
void display(void)
	{
		arr1[3]=arr[(hr/10)%10];
		arr1[2]=arr[hr%10];
		arr1[2]|=0x80;
		arr1[1]=arr[(min/10)%10];
		arr1[0]=arr[min%10];
		for(i=3;i>=0;i--)
		{
		LPC_GPIO1->FIOPIN = a[i];
		LPC_GPIO0->FIOPIN=(arr1[i])<<4;
		for(j=0;j<500;j++);		
		LPC_GPIO0->FIOCLR = 0x00000FF0;
		LPC_GPIO1->FIOPIN=a[4];
		}
		
	
		}
		

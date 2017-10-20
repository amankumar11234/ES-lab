#include<LPC17xx.h>
#define RS_CTRL 0x8000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

void lcd_init(void);
void wr_cn(void);
void clr_disp(void);
void delay_lcd(unsigned int);
void lcd_com(void);
void wr_dn(void);
void lcd_data(void);
void clear_ports(void);
void lcd_puts(unsigned char *);
void delay(void);
extern unsigned long int temp1, temp2;

unsigned long int temp1=0,temp2=0;

int main(void)
{
	unsigned int i;
	unsigned char clk[]={"00:00:00"};
	//unsigned char Msg3[]={"MIT Diamond"};
	//unsigned char Msg4[]={"Jubilee Year"};

	SystemInit();
	SystemCoreClockUpdate();
	lcd_init();
	//temp1=0x80;
	//	lcd_com();
		//delay_lcd(800);
		//lcd_puts(&Msg3[0]);

	//	temp1=0xC0;
		//lcd_com();
		//delay_lcd(800);
		//lcd_puts(&Msg4[0]);
    while(1)
		{ 
				temp1=0x80;
		    lcd_com();
		    delay_lcd(800);
	      lcd_puts(&clk[0]);
			clk[7]++;
			if(clk[7]==58)
			{clk[7]=48;
        clk[6]++;
         if(clk[6]==54)
				 {clk[6]=48;
          clk[4]++;
          if(clk[4]==58)
					{clk[4]=48;
						clk[3]++;
						if(clk[3]==54)
						{clk[3]=48;
							clk[1]++;
							if(clk[1]==58)
							{clk[1]=48;
               clk[0]++;}
							 if(clk[1]==52&&clk[0]==50)
							 {clk[0]=0;
                clk[1]=0;
                 clk[3]=0;
                 clk[4]=0;
                 clk[6]=0;
                 clk[7]=0;}}}}}
								 delay();
       
}}

//lcd init

void lcd_init()
{
	LPC_PINCON->PINSEL3 &= 0xFC003FFF;
	
	LPC_GPIO0->FIODIR |=DT_CTRL;
	LPC_GPIO0->FIODIR |=RS_CTRL;
	LPC_GPIO0->FIODIR |=EN_CTRL;
	
	clear_ports();
	delay_lcd(3200);
	
	temp2=(0x30<<19);
	wr_cn();
	delay_lcd(30000);
	
	temp2=(0x30<<19);
	wr_cn();
	delay_lcd(30000);
	
	temp2=(0x30<<19);
	wr_cn();
	delay_lcd(30000);
	
	temp2=(0x20<<19);
	wr_cn();
	delay_lcd(30000);
	
	temp1=0x28;
	lcd_com();
	delay_lcd(30000);
	
	temp1=0x0c;
	lcd_com();
	delay_lcd(800);
	
	temp1=0x06;
	lcd_com();
	delay_lcd(800);
	
	temp1=0x01;
	lcd_com();
	delay_lcd(10000);
	
	temp1=0x80;
	lcd_com();
	delay_lcd(800);
	return;
}

void lcd_com(void)
{
	temp2=temp1 & 0xF0;
	temp2=temp2 << 19;
	wr_cn();
	temp2=temp1 & 0x0F;
	temp2=temp2 << 23;
	wr_cn();
	delay_lcd(1000);
	return;
}

void wr_cn(void)
{
	clear_ports();
	LPC_GPIO0->FIOPIN = temp2;
	LPC_GPIO0->FIOCLR = RS_CTRL;
	LPC_GPIO0->FIOSET = EN_CTRL;
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL;
	return;
}

void lcd_data(void)
{
	temp2=temp1 & 0xF0;
	temp2= temp2 << 19;
	wr_dn();
	temp2 = temp1 & 0x0F;
	temp2 = temp2 << 23;
	wr_dn();
	delay_lcd(1000);
	return;
}

void wr_dn(void)
{
	clear_ports();
	
	LPC_GPIO0->FIOPIN = temp2;
	
	LPC_GPIO0->FIOSET = RS_CTRL;
	LPC_GPIO0->FIOSET = EN_CTRL;
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL;
	return;
}

void delay_lcd(unsigned int r1)
{
	unsigned int r;
	for(r=0;r<r1;r++);
	return;
}

void clr_disp(void)
{
	temp1 = 0x01;
	lcd_com();
	delay_lcd(10000);
	return;
}

void clear_ports(void)
{
	LPC_GPIO0->FIOCLR = DT_CTRL;
	LPC_GPIO0->FIOCLR = RS_CTRL;
	LPC_GPIO0->FIOCLR = EN_CTRL;
	return;
}

void lcd_puts(unsigned char *buf1)
{
	unsigned int i=0;
	
	while(buf1[i]!='\0')
	{
		temp1 = buf1[i];
		lcd_data();
		i++;
		if(i==16)
		{
			temp1=0xc0;
			lcd_com();
		}
	}
	return;
}
void delay(void)
{ LPC_TIM0->TCR=0x02;
	LPC_TIM0->EMR=0x20;
	LPC_TIM0->CTCR=0;
	LPC_TIM0->MCR=4;
	
	LPC_TIM0->MR0=2999;
	LPC_TIM0->PR=999;
	LPC_TIM0->TCR=1;
	while(!(LPC_TIM0->EMR&1));
}
#include <LPC17xx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define FIRST_SEG 0xF87FFFFF
#define SECOND_SEG 0xF8FFFFFF
#define THIRD_SEG 0xF97FFFFF
#define FOURTH_SEG 0xF9FFFFFF
#define DISABLE_ALL 0xFA7FFFFF
#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

void delay(void);
void scan(void);
void displayled(char);
void lcd_init(void);
void wr_cn(void);
void clr_disp(void);
void delay_lcd(unsigned int);
void lcd_com(void);
void wr_dn(void);
void lcd_data(void);
void clear_ports(void);
void lcd_puts( char *);

	unsigned int n;
	unsigned char cc;
extern unsigned long int temp2, temp1;
unsigned long int temp1=0,temp2=0;


unsigned char col,row,flag;
unsigned long int i,temp,temp3;

unsigned long seg_list[4] = {FIRST_SEG, SECOND_SEG, THIRD_SEG, FOURTH_SEG};
unsigned char seg_disp[16] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7c,0x58,0x5e,0x79,0x71};
unsigned char SEVEN_CODE[4][4] = {{0x01,0x02,0x03,0x2B},{0x04,0x05,0x06,0x2D},{0x07,0x08,0x09,0x2A},{0x11,0x00,0x3D,0x2F}};
char sign[4] = {'+', '-'};
unsigned char one_sec_flg = 0x00;
int val[2], neg_fl=0, c=0;
 char a;
char op;
char Msg4[20]="";
int main(void)
{
	int count=0, dig1,dig2;
	int res;	
	SystemInit();
	SystemCoreClockUpdate();
	 
	//LPC_PINCON->PINSEL0 &= 0x7F0000FF;	 //P0.4 to P0.11 GPIO data lines
	//LPC_PINCON->PINSEL1 &= 0xFFFFFFF8;
	//LPC_GPIO0->FIODIR |= 0x00078FF0;		//P0.4 to P0.11 output
	LPC_PINCON->PINSEL3 &= 0xFFC03FFF; 	//P1.23 to P1.26 MADE GPIO
	LPC_PINCON->PINSEL4 &= 0xF00FFFFF; 	//P2.10 t P2.13 made GPIO
	LPC_GPIO2->FIODIRL |= 0x3C00; 		//made output P2.10 to P2.13 (rows)
	LPC_GPIO1->FIODIRH &= 0xF87F; 		//made input P1.23 to P1.26 (cols)

	while(1)
		for(row=0;row<4;row++){
			if(row == 0)
			temp = 0x00000400;
			else if(row == 1)
			temp = 0x00000800;
			else if(row == 2)
			temp = 0x00001000;
			else if(row == 3)
			temp = 0x00002000;
		
			LPC_GPIO2->FIOPIN = temp;
			flag = 0;
			scan();
			if(flag == 1)	
			{	int res;
				count++;
			
				for(i=0; i<1000; i++);
				a=SEVEN_CODE[row][col];
				displayled(a);
				if(count==1)
				{   Msg4[0]='\0';
						dig1=a;
				}
				else if(count==2)
				{
						op=a;
				}
				else if(count==3)
				{
						dig2=a;
					
				}
				
				if(count==4)
				{
					count=0;
					switch(op)
					{
							case '+': res=dig1+dig2; break;
							case '-': res=dig1-dig2; break;
							case '*': res=dig1*dig2; break;
							case '/': res=dig1/dig2; break;
							default: res=999999;
					}
					displayled(res);
					
				}
				break;
				/*c++;
				switch(op){
					case 1:	val[0] = row * 3 + col;
					case 2:	op = sign[col];
					case 3:	val[1] = row * 3 + col;
				}
				if(c==3){
					switch(op){
						case '+' :  res = val[0]+val[1];
									break;
						case '-' : 	res = val[0]-val[1];
									break;
					}
					if (res<0){
						neg_fl = 1;
						res *= -1;
					}
					for(i=0; i<2; i++){
						val[i] = res%10;
						res /=10;
					}
				}*/
			}

		}		
}

void displayled(char cc){
	unsigned int x;
	unsigned int i;
	char Msg1[] = {"Enter N1:"};
  char Msg2[] = {"Enter N2:"};
	char Msg3[] = {"Enter Operator:"};
	
	lcd_init();
	temp1= 0x80;
	lcd_com();
 	delay_lcd(800);
 // lcd_puts(&Msg3[0]);
			if(cc==45){
				op=cc;
				sprintf(Msg4,"%c",cc);}
			else if(cc==43){
				op=cc;
				sprintf(Msg4,"%c",cc);
			}
			else if(cc==42){
				op=cc;
				sprintf(Msg4,"%c",cc);
			}
			else if(cc==47)
			{
				op=cc;
				sprintf(Msg4,"%c",cc);
			}
			else if(cc==61){
				sprintf(Msg4,"%c",cc);}
			else
			  sprintf(Msg4,"%d",cc);
	   lcd_puts(&Msg4[0]);
	
}

void scan(void)
{
 	unsigned long temp3;
	temp3 = LPC_GPIO1->FIOPIN;	
	temp3 &= 0x07800000;
	if(temp3 != 0x00000000)
	{
		flag = 1;
		if (temp3 ==0x00800000)
			col=0;
		else if (temp3==0x01000000)
			col=1;
		else if (temp3==0x02000000)
			col=2;
		else if (temp3==0x04000000)
			col=3;
	}
}

void lcd_init()
{
	LPC_PINCON->PINSEL3 &= 0xFC003FFF;
	
	LPC_GPIO0->FIODIR |= DT_CTRL;
	LPC_GPIO0->FIODIR |= RS_CTRL;
	LPC_GPIO0->FIODIR |= EN_CTRL;
	
	clear_ports();
	delay_lcd(3200);
	
	temp2 = (0x30<<19);
	wr_cn();
	delay_lcd(30000);
	
	
	temp2 = (0x30<<19);
	wr_cn();
	delay_lcd(30000);
	
	temp2 = (0x30<<19);
	wr_cn();
	delay_lcd(30000);
	
	temp2 = (0x20<<19);
	wr_cn();
	delay_lcd(30000);
	
	temp1 = 0x28;
	lcd_com();
	delay_lcd(30000);
	
	temp1 = 0x0C;
	lcd_com();
	delay_lcd(800);	

	temp1 = 0x06;
	lcd_com();
	delay_lcd(800);	

	temp1 = 0x01;
	lcd_com();
	delay_lcd(10000);	
	
	temp1 = 0x80;
	lcd_com();
	delay_lcd(800);		
return;
}

void lcd_com(void)
{
	temp2= temp1 & 0xF0;
	temp2= temp2 & 19;
	wr_cn();
	temp2= temp1 & 0x0F;
	temp2 = temp2 << 23;
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
	LPC_GPIO0-> FIOCLR = EN_CTRL;
	return;
	
}
void lcd_data(void)
{
	temp2 = temp1 & 0xF0;
	temp2 = temp2 <<19;
	wr_dn();
	temp2= temp1 & 0x0F;
	temp2 = temp2 <<23;
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
	LPC_GPIO0->FIOCLR= DT_CTRL;
	LPC_GPIO0->FIOCLR= RS_CTRL;
	LPC_GPIO0->FIOCLR= EN_CTRL;
	
	return;
}

void lcd_puts(char *buf1)
{
	unsigned int i=0;
	
		while(buf1[i]!='\0')
		{
			temp1 = buf1[i];
			lcd_data();
			i++;
			if(i==16)
			{
					temp1= 0xC0;
				lcd_com();
			}
		}
		return;
	}
	
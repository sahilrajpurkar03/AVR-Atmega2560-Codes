/*
 * button_module.c
 *
 * Created: 10-12-2018 14:48:13
 * Author : Sahil
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>

#define buttonport PINE
#define indicator_1 PORTA

//variables for button module;
uint8_t pres_butt[8]={0,0,0,0,0,0,0,0};
uint8_t prev_butt[8]={0,0,0,0,0,0,0,0};
uint8_t button[8]={0,0,0,0,0,0,0,0};

void port_init();
void readport();

int main(void)
{
	port_init();
    while (1) 
    {
		readport();	
    }
}

void port_init()
{
	DDRE=0x00;						//button module
	DDRA=0xFF;						//indicator_1
}

void readport()
{
	for (int i=0;i<8;i++)
	{
		pres_butt[i]=(buttonport & (1<<i));
		if (pres_butt[i])
		{
			_delay_ms(5);
			pres_butt[i]=(buttonport & (1<<i));
			if ((pres_butt[i]) && (!prev_butt[i]))
			{
				button[i]=1;
			}
		}
		prev_butt[i]=pres_butt[i];
	}
	
	if (button[0])
	{
		indicator_1=0x01;
		button[0]=0;
	}
	if (button[1])
	{
		indicator_1=0x02;
		button[1]=0;
	}
	if (button[2])
	{
		indicator_1=0x04;
		button[2]=0;
	}
	if (button[3])
	{
		indicator_1=0x08;
		button[3]=0;
	}
	if (button[4])
	{
		indicator_1=0x10;
		button[4]=0;
	}
	if (button[5])
	{
		indicator_1=0x20;
		button[5]=0;
	}
	if (button[6])
	{
		indicator_1=0x40;
		button[6]=0;
	}
	if (button[7])
	{
		indicator_1=0x80;
		button[7]=0;
	}
}

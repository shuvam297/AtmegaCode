/*
 * pwm_2.c
 *
 * Created: 8/25/2017 8:42:40 AM
 * Author : Shubham
 */ 

#define F_CPU 16000000
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
volatile unsigned int s=0;

unsigned char l[10];
unsigned char data;
volatile int flag = 0;
volatile unsigned int i=0;
unsigned int f;
unsigned int j=10;
volatile unsigned char led = 0;
void my_delay_ms(int ms)
{
	while (0 < ms)
	{
		_delay_ms(1);
		--ms;
	}
}

//@ -> angle terminator
//# -> distance terminator
ISR(USART0_RX_vect){
	
	
	data = UDR0;
	
	if(data == '@' || data == '#'|| data == '$'){	
		flag = 1;
		if(data == '@')
			f = 1;
			else if(data == '#')
				f = 2;
				else 
					f = 3;
		}
	else 
		{
		
			l[i]= data;
			i++;
		}
	
	if(flag == 1){
		i--;
		s = l[i] - 48;
		i--;
		while(i){
			s += (j*(l[i]-48));
			j*=10;i--;
		}
		
	}
} 

int main(void)
{
    /* Replace with your application code */
    
	cli();
	
	UCSR0B = 0x00;
	UCSR0A = 0x00;
	UCSR0C = 0x06;
	UBRR0L = 0x67;
	UBRR0H = 0x00;
	UCSR0B = 0x98;
		
	sei();
	
	
	
	DDRB |= 0x90; //7-4
	DDRH |= 0x40; //6
	DDRG |= 0x20;//5
	DDRD |= 0x07;//0-1-2
	TCCR0B = 0x00;
	TCCR0A &= ~(0x52);//1010--01
	TCCR0A |= 0xA1;
	
	TCCR0B &= ~(0x0E);//----0001
	TCCR0B |= 0x01;
	
	
	TCCR2B = 0x00;
	TCCR2A &= ~(0x52);//1010--01
	TCCR2A |= 0xA1;
	
	TCCR2B &= ~(0x0E);//----0001
	TCCR2B |= 0x01;
	//0.055-> clockwise
volatile float ccl = 10.83;//12.7;//15
volatile float acl = 11.41;//16.45
volatile float st = 26;//23.28//26
	while (1) 
    {
		if(flag){	
			
			if(f == 1){
				
				if(l[0] == '+'){
						
						OCR0A = 0x00;
						OCR2A = 0x65;
						OCR0B = 0x65;
						OCR2B = 0x00;
						my_delay_ms(s*acl);
						
						UDR0 = '.';
						
						
				}
					else {
						
						OCR0A = 0x65;
						OCR2A = 0x00;
						OCR0B = 0x00;
						OCR2B = 0x65;
						my_delay_ms(s*ccl);
						
						UDR0 = '.';
					}
			}
			if(f == 2){
					
					OCR0A = 0xFA;
					OCR2A = 0x00;
					OCR0B = 0xFF;
					OCR2B = 0x00;
					
					my_delay_ms(s*st);	
					UDR0 = '.';
			}
			if(f==3){
				
				led = 0x0F&l[i+1];
				PORTD = led;
				if(l[i+1]!= '0')
					my_delay_ms(1000);
				PORTD = 0x00;
				UDR0 = '.';	
			
			}       
					
					i=0;
					s=0;
					j=10;
					f=0;
					flag = 0;
		
		}
	
	OCR0A = 0x00;
	OCR0B = 0x00;
	OCR2A = 0x00;
	OCR2B = 0x00;	
	
	
	}
}


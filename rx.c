/*
 * rx.c
 *
 * Created: 07-11-2017 19:13:46
 * Author : Aditi Ramadwar
 */ 
/*Set fuse bits to L:0xFF and H:0xD9*/
#ifdef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#define BAUD 4000
#define BAUDRATE (F_CPU/16*BAUD)-1

void cmd_4bit(unsigned char cmd)
{
	unsigned char a,b;
	a = cmd & 0xf0;
	PORTB = a | 0x04;
	_delay_ms(100);
	PORTB &=~ 0x04;
	
	b = cmd<<4;
	PORTB = b| 0x04;
	_delay_ms(100);
	PORTB &=~ 0x04;
}

void data_4bit(unsigned char Y)
{
	unsigned char a,b;
	a = Y & 0xf0;
	PORTB = 0x05 | a;
	_delay_ms(100);
	PORTB &=~0x04;
	
	b = Y<<4;
	PORTB = 0x05 | b;
	_delay_ms(100);
	PORTB &=~0x04;
}

void lcd_str(char *str)
{
	while(*str)
	data_4bit(*str++);
}

void lcd_init()
{
	cmd_4bit(0x02);
	cmd_4bit(0x28);
	cmd_4bit(0x0e);
	cmd_4bit(0x06);
	cmd_4bit(0x01);
	cmd_4bit(0x80);
}

// function to initialize UART
void uart_init (void)
{
	
	UBRRL = BAUDRATE;                           // set baud rate
	UBRRH = (BAUDRATE>>8);                      // shift the register right by 8 bits
	UCSRC = (1<<USBS)|(3<<UCSZ0)|(1<<UPM1)|(0<<UPM0);               // 8bit data format,Asynchronous,Even parity,1 stop bit
    UCSRB = (1<<TXEN)|(1<<RXEN);                // enable receiver and transmitter
}

// function to send data
void uart_transmit (unsigned char data)
{
	while (!(UCSRA & (1<<UDRE))) 
	{ 
	          // wait while register is free
	}
	UDR = data;                                   // load data in the register
}

// function to receive data
unsigned char uart_recieve ()
{
	while(!(UCSRA & (1<<RXC)))  
	{     
		            // wait while data is being received
	}
	return(UDR);                                   // return 8-bit data
}

void usart_msg(char *c)
{
	while (*c != 0)
	uart_transmit(*c++);
}




int main(void)
{
	    DDRB = 0xff;
		
	    lcd_init();
		lcd_str("Hello!!!");
		_delay_ms(1000);
		
		cmd_4bit(0x01);
		cmd_4bit(0x80);
		lcd_str("Welcome to");
		
		cmd_4bit(0xC0);
		lcd_str("V-Restaurant");
		_delay_ms(1000);
		
	    uart_init();
		unsigned char data1=0;
		
		

     while (1) 
    {
		
		 data1 = uart_recieve();
		 
		 if (data1=='1')
		 {
			 cmd_4bit(0x01);
			 lcd_str("1.Tea  Rs.10");
			 usart_msg("1.Tea  Rs.10");
			 _delay_ms(1000);
		 }
		 
		 else if(data1=='2')
		 {
			 cmd_4bit(0x01);
			 lcd_str("2.Coffee  Rs.15");
			 usart_msg("2.Coffee  Rs.15");
			 _delay_ms(1000);
		 }
		 
	            
			else if (data1=='3')
			{
				 cmd_4bit(0x01);
				 lcd_str("3.Vadapav  Rs.20");
				 _delay_ms(1000);
			}
		    
			else if (data1=='4')
			{
			 cmd_4bit(0x01);
			lcd_str("4.Idli  Rs.25");
			_delay_ms(1000);
			}
			
			else if (data1=='5')
			{ 
		    cmd_4bit(0x01);
			lcd_str("5.Dosa  Rs.25");
			_delay_ms(1000);
			}
			
			else if (data1=='6')
			{
			 cmd_4bit(0x01);
			lcd_str("6,Lunch  Rs60");
			_delay_ms(1000);
			}
			
			else if (data1=='*')
			{
				cmd_4bit(0x01);
				lcd_str("Order cancelled");
				_delay_ms(1000);
				
			}
			
			else if (data1=='#')
			{
				cmd_4bit(0x01);
				lcd_str("Order Confirmed");
				_delay_ms(1000);
			}
			 else
			 ;
			 _delay_ms(500);		 
		 
	}
}


/*
 * tx.c
 *
 * Created: 07-11-2017 12:50:49
*/ 
//microcontroller atmega

/*Set fuse bits to L:0xFF and H:0xD9*/

#ifdef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#define BAUD 4000
#define BAUDRATE (F_CPU/16*BAUD)-1

#define		KEYPAD_PORT		PORTC

#define		KEYPAD_PIN		PINC
unsigned char read_keypad(void);

void uart_init(void);
void uart_transmit(unsigned char data);
unsigned char uart_recieve(void);
void usart_msg(char *c);


void lcd_init()
{
	cmd_4bit(0x02);
	cmd_4bit(0x28);
	cmd_4bit(0x0e);
	cmd_4bit(0x06);
	cmd_4bit(0x01);
	cmd_4bit(0x80);
}

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
unsigned char read_keypad(void)
{
	unsigned char keypad_input=0xff,keypad_output=0xff;
	KEYPAD_PORT=0xfe;
	_delay_us(2);
	keypad_input=KEYPAD_PIN & 0xf0;

	if(keypad_input==0xe0)
	keypad_output=0x01;
	else if(keypad_input==0xd0)
	keypad_output=0x02;
	else if(keypad_input==0xb0)
	keypad_output=0x03;
	else if(keypad_input==0x70)
	keypad_output=0x0c;
	else
	;

	KEYPAD_PORT=0xfd;
	_delay_us(2);
	keypad_input=KEYPAD_PIN & 0xf0;

	if(keypad_input==0xe0)
	keypad_output=0x04;
	else if(keypad_input==0xd0)
	keypad_output=0x05;
	else if(keypad_input==0xb0)
	keypad_output=0x06;
	else if(keypad_input==0x70)
	keypad_output=0x0d;
	else
	;

	KEYPAD_PORT=0xfb;
	_delay_us(2);
	keypad_input=KEYPAD_PIN & 0xf0;

	if(keypad_input==0xe0)
	keypad_output=0x07;
	else if(keypad_input==0xd0)
	keypad_output=0x08;
	else if(keypad_input==0xb0)
	keypad_output=0x09;
	else if(keypad_input==0x70)
	keypad_output=0x0e;
	else
	;
	
	KEYPAD_PORT=0xf7;
	_delay_us(2);
	keypad_input=KEYPAD_PIN & 0xf0;

	if(keypad_input==0xe0)
	keypad_output=0x0a;
	else if(keypad_input==0xd0)
	keypad_output=0x00;
	else if(keypad_input==0xb0)
	keypad_output=0x0b;
	else if(keypad_input==0x70)
	keypad_output=0x0f;
	else
	;

	return keypad_output;
}

void uart_init (void)
{
	UBRRH = 0x00;                      					// shift the register right by 8 bits
	UBRRL = BAUDRATE;                           		// set baud rate
	UCSRB|= (1<<TXEN)|(1<<RXEN);                		// enable receiver and transmitter
	UCSRC|= (1<<USBS)|(3<<UCSZ0)|(1<<UPM1)|(0<<UPM0);   // 8bit data format,Asynchronous,Even parity
}

// function to send data
void uart_transmit (unsigned char data)
{
	              
	
	while (!( UCSRA & (1<<UDRE)))          				// wait while register is free
	{
		
	}
    UDR = data;                               			// load data in the register
}

// function to receive data
unsigned char uart_recieve ()
{
	while(!(UCSRA & (1<<RXC)))                  		// wait while data is being received
	{
		
	}
	return(UDR);                                   		// return 8-bit data
}

void usart_msg(char *c)
{
	while (*c != 0)
   uart_transmit(*c++);
}


int main(void)
{
    DDRC=0x0f;
    
    PORTC=0xff;
	
    DDRB = 0xff;
	
	 lcd_init();
	 uart_init();
	 
	unsigned char data,data1=0;
	unsigned char keypad_value;
	
	 lcd_str("Hello!!!");
	 _delay_ms(20000);
	  
    cmd_4bit(0x01);
    cmd_4bit(0x80);
    lcd_str("Welcome to");
    _delay_ms(25000);

    cmd_4bit(0xC0);
    lcd_str("V-Restaurant");
    _delay_ms(25000);

    cmd_4bit(0x01);
    cmd_4bit(0x80);
    lcd_str("Press Key to");
    _delay_ms(25000);


    cmd_4bit(0xC0);
    lcd_str("Select Order");
    _delay_ms(25000);

    cmd_4bit(0x01);
    cmd_4bit(0x80);
    lcd_str("# to confirm");
    _delay_ms(25000);
	
	 cmd_4bit(0xC0);
	 lcd_str("* to cancel");
	 _delay_ms(25000);


    cmd_4bit(0x01);
    cmd_4bit(0x80);
    lcd_str("1.Tea  Rs.10");
    _delay_ms(25000);

    cmd_4bit(0xC0);
    lcd_str("2.Coffee  Rs.15");
    _delay_ms(25000);

    cmd_4bit(0x01);
    cmd_4bit(0x80);
    lcd_str("3.Vadapav  Rs.20");
    _delay_ms(25000);

    cmd_4bit(0xC0);
    lcd_str("4.Idli  Rs.25");
    _delay_ms(25000);

    cmd_4bit(0x01);
    cmd_4bit(0x80);
    lcd_str("5.Dosa  Rs.25");
    _delay_ms(25000);

    cmd_4bit(0xC0);
    lcd_str("6.Lunch  Rs.60");
    _delay_ms(25000);
	
	cmd_4bit(0x01);
	cmd_4bit(0x80);
	lcd_str("Press a Key");
	_delay_ms(25000);

	



    while (1) 
    {
		keypad_value=read_keypad();
	    
		if(keypad_value!=0xff)
		{
			 
			
			switch(keypad_value)
			{
				case 0:lcd_str('0');
				break;
				

				case 1:cmd_4bit(0x01);
				cmd_4bit(0x80);
				lcd_str("1.Tea  Rs.10");
				_delay_ms(25000);
				data ='1';
				uart_transmit(data);
				break;
				
				

				case 2:cmd_4bit(0x01);
				cmd_4bit(0x80);
				lcd_str("2.Coffee  Rs.15");
				_delay_ms(25000);
				data ='2';
				uart_transmit(data);
				break;
				
				
				case 3:cmd_4bit(0x01);
				cmd_4bit(0x80);
				lcd_str("3.Vadapav  Rs.20");
				_delay_ms(25000);
				data ='3';
				uart_transmit(data);
				break;
				

				case 4:cmd_4bit(0x01);
				cmd_4bit(0x80);
				lcd_str("4.Idli  Rs.25");
				_delay_ms(25000);
				data ='4';
				uart_transmit(data);
				
				break;
				

				case 5:cmd_4bit(0x01);
				cmd_4bit(0x80);
				lcd_str("5.Dosa  Rs.25");
				_delay_ms(25000);
				data ='5';
				uart_transmit(data);
				
				break;


				case 6:cmd_4bit(0x01);
				cmd_4bit(0x80);
				lcd_str("6.Lunch  Rs.60");
				_delay_ms(25000);
				data ='6';
				uart_transmit(data);
				
				break;


				case 10:cmd_4bit(0x01);
				cmd_4bit(0x80);
				lcd_str("Order Cancelled");
				_delay_ms(25000);
				data ='*';
				uart_transmit(data);
				
				break;

				case 11:cmd_4bit(0x01);
				cmd_4bit(0x80);
				lcd_str("Order Confirmed"); 
				_delay_ms(25000);
				data ='#';
				uart_transmit(data);
					                                                                                                                                                                                     		                          _delay_ms(25000);
				break;

				
			}
		}
		else
		;
		/*Null statement*/

		_delay_ms(500);
    }
}



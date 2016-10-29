/********************************************************************************
 File Name : project.c 
 Written by: Manjunath.K,Amit
 Date      : 23-10-2016
 Purpose/ Description : Autonomous serving bot (Firebird bot code)

 *********************************************************************************/

/**********************************************************************************/
// Include files
/**********************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <math.h> //included to support power function
#include "lcd.c"
#include "uart.h"
#include "global.h"

/**********************************************************************************/
// As per the experiment line follwer sensor value for black is found out to be >0x40
// So threshold is set to this value 
/**********************************************************************************/
#define BLACK_LINE 0x40

/**********************************************************************************/
//Function to initialize ports
//Input : void
//Output : void
//Description : 
/**********************************************************************************/
void port_init();


/**********************************************************************************/
//Timer 5 initialized in PWM mode for velocity control
//Input : void
//Output : void
//Description : 
// Timer 5 initialized in PWM mode for velocity control
// Prescale:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz
/**********************************************************************************/
void timer5_init();


/**********************************************************************************/
// Function for robot velocity control
//Input : unsigned char, unsigned char 
//		  left motor speed, right motor speed
//Output: void
//Result can be seen in the wheel speed
/**********************************************************************************/
void velocity(unsigned char, unsigned char);


/**********************************************************************************/
// Function for robot velocity control
//Input : void
//Output: void
//Result can be seen in the wheel speed
/**********************************************************************************/
void motors_delay();

/**********************************************************************************/
// Function to move robot from finish line to start position
//Input : void
//Output: void
/**********************************************************************************/
void reposition();
/**********************************************************************************/
// Function to handle the obstacle found in the onward path towards the desired table
//Input : void
//Output: void
/**********************************************************************************/
void handle_obst_onward();

/**********************************************************************************/
// Function to handle the obstacle found in the backward path towards the desired table
//Input : void
//Output: void
/**********************************************************************************/
void handle_obst_backward();

/**********************************************************************************/
// Function to move robot by 1 grid
//Input : void
//Output: void
//Robot reaches next intersection when called
/**********************************************************************************/
void grid_forward();

/**********************************************************************************/
// Function to sample sharp and IR sensor values
//Input : void
//Output: void
/**********************************************************************************/
void sample_sensor();

/**********************************************************************************/
// Function for move adjust robot path when robot goes out of black line
//Input : void
//Output: void
//Robot reaches for black line.
/**********************************************************************************/
void adjust_path();

/**********************************************************************************/
// Function for move robot byparticulat distance along with the black line
//Input : distance to move
//Output: void
//Robot moves forward by given diatnce along the black line
/**********************************************************************************/
void forward_mm_totable(int);

/**********************************************************************************/
// Function for sample ADC values of a particular channel
//Input : channel number
//Output: sample value
/**********************************************************************************/
unsigned char ADC_Conversion(unsigned char); // Sam

unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder
unsigned long int ShaftCountRight = 0; //to keep track of right position encoder
unsigned int Degrees; //to accept angle in degrees for turning



unsigned char ADC_Value;      				// sampled ADC value
unsigned char flag = 0;						 
unsigned char Left_white_line = 0;			// to store Left WL sensor value
unsigned char Center_white_line = 0;		// to store center WL sensor value
unsigned char Right_white_line = 0;			// to store right WL sensor value

unsigned char data; 						//to store received data from UDR1
unsigned int check=0;							
unsigned char datac;						// copy of data fom UDR1
unsigned int start=0,stop_s=1,id=0,coordinates=0,x_recei=0,destination_act=0;	// start indication stop indication, varibles to receive Zigbee values
unsigned int x_coor,y_coor;					// present coordinates received from zigbee
unsigned int x_dest,y_dest;					// destination coordinates received from zigbee
unsigned int config_done=0;					// indicates the reception og zigbee values
unsigned int count =100;				
unsigned int last_choice=0;					
unsigned int dest_backward=0;				// going backward to counter
unsigned char Front_Sharp_Sensor=0;			// to store sharp sensor value 
unsigned char Front_IR_Sensor=0;			// to store IR senor value
unsigned char obstflag = 0;					// obstacle indication flag
unsigned int take_left=0;					// left turn indication
unsigned char Left_IR_sensor1=0;			// IR sensor values
unsigned char Left_IR_sensor2=0;
unsigned char Right_IR_sensor4 =0;
unsigned int back_s_a=0;
unsigned int angle_count=0;
unsigned int back_reroute=0;



int flag_reached=0;							// indicates table reached
int flag_startreached=0;					// indicates counter reached
int testmsg=0;								// test message variable
int locmsg=0;				
int tablex=0;				
int tabley=0;
int currentcountx=0;						// present 'x' coordinate
int currentcounty=0;						// present 'y' coordinate
int des_countx=2;							// default table coordinate
int des_county=2;							// default table coordinates
int back_s=0;								
int x_reached=0;							// indicates the matching of 'x' coordinates
int y_reached=0;
int freebot=0;								// indicates whether bot is in service or free
int flagturn=0;								// turn indicator
int flagrightturn=0;//variable to check turn taken
int flag2=0;//to remove redundant increments.

// threshold values for the sensors.
unsigned int thresh_Front_Sharp_Sensor =0x50;
unsigned int thresh_Front_IR_Sensor = 0x64;
unsigned int thresh_weightsensor = 0x8C;


/**********************************************************************************/
//Constants for BOT1
//Robot ID: 'q' or 'Q'
//testreply : when bot is free free and if it receives a polling message from counter,
//			  bot1 replies with BOT1 FY message.
//startreply : message is sent from bot1 when it starts to deliver the order
//coordreply : Bot1 replies with this message when it is going to particular table
//Result can be seen in the wheel speed
//delayvalueonward : delay value during the journey towards the table
//					 based on this value, when an obstacle is encounter, bot waits for this amount
//					 of time.
//delayvalueback   : delay value during the journey from the table
//					 based on this value, when an obstacle is encounter, bot waits for this amount
//					 of time.
/**********************************************************************************/
//values for BOT1
char botid ='q';
char botid2 = 'Q';
char testreply[]="BOT1 FY";
char startreply[]="BOT1 OK";
char coordreply[]="BOT1 GOING TO TAB";
unsigned int delayvalueonward=12000;      // Bot waits for 12000ms before next obstacle check when moving towards table.
unsigned int delayvalueback=6000;		  // Bot waits for 6000ms before next obstacle check when moving from table.


/**********************************************************************************/
//Constants for BOT2
//Robot ID: 'w' or 'W'
//testreply : when bot is free free and if it receives a polling message from counter,
//			  bot2 replies with BOT2 FY message.
//startreply : message is sent from bot2 when it starts to deliver the order
//coordreply : Bot2 replies with this message when it is going to particular table
//Result can be seen in the wheel speed
//delayvalueonward : delay value during the journey towards the table
//					 based on this value, when an obstacle is encounter, bot waits for this amount
//					 of time.
//delayvalueback   : delay value during the journey from the table
//					 based on this value, when an obstacle is encounter, bot waits for this amount
//					 of time.
/**********************************************************************************/
/*
//values for BOT2
char botid ='w';
char botid2 = 'W';
char testreply[]="BOT2 FY";
char startreply[]="BOT2 OK";
char coordreply[]="BOT2 GOING TO TAB";
unsigned int delayvalueonward=8000;
unsigned int delayvalueback=4000;
 */

/**********************************************************************************/
//Constants for BOT3
//Robot ID: 'e' or 'e'
//testreply : when bot is free free and if it receives a polling message from counter,
//			  bot3 replies with BOT3 FY message.
//startreply : message is sent from bot3 when it starts to deliver the order
//coordreply : Bot3 replies with this message when it is going to particular table
//Result can be seen in the wheel speed
//delayvalueonward : delay value during the journey towards the table
//					 based on this value, when an obstacle is encounter, bot waits for this amount
//					 of time.
//delayvalueback   : delay value during the journey from the table
//					 based on this value, when an obstacle is encounter, bot waits for this amount
//					 of time.
/**********************************************************************************/
/*
//values for BOT3
 char botid ='e';
char botid2 = 'E';
char testreply[]="BOT3 FY";
char startreply[]="BOT3 OK";
char coordreply[]="BOT3 GOING TO TAB";
unsigned int delayvalueonward=6000;
unsigned int delayvalueback=3000;
#define BOT_DELAY_ONWARDX 10
#define BOT_DELAY_ONWARDY 7
#define BOT_DELAY_BACKWARDX 1
#define BOT_DELAY_BACKWARDY 4
 */

/**********************************************************************************/
// Function for UART start- Initialization of UART
//Input : void
//Output: void
/**********************************************************************************/
void uart_start(void)
{
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0); //transmit side of hardware
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); //receive side of hardware

	UBRR0L = BAUD_PRESCALE; //set the baud to 9600, have to split it into the two registers
	UBRR0H = (BAUD_PRESCALE >> 8); //high end of baud register

	UCSR0B |= (1 << RXCIE0); //recieve data interrupt, makes sure we don't loose data
}


/**********************************************************************************/
// Function for sending a character through UART
//Input : uint8_t - 8 bit data to be sent
//Output: void
//Description : pass the data to be sent through UART to this function
//				sends a byte of data at a time
//				To send multiple data, call this multiple times
/**********************************************************************************/
void uart_sendint(uint8_t data)
{
	/*
	Use this to send a 8bit long piece of data
	 */
	while ((UCSR0A & (1 << UDRE0)) == 0);//make sure the data register is cleared
	UDR0 = data; //send the data
	//while ((UCSR0A & (1 << UDRE0)) == 0);//make sure the data register is cleared
	//UDR0 = '\n';//send a new line just to be sure
}


/**********************************************************************************/
// Function for sending 16bits of data through UART
//Input : uint16_t - 16 bit data to be sent
//Output: void
//Description : pass the data to be sent through UART to this function
//				sends 2 byte of data at a time
//				To send multiple data, call this multiple times
/**********************************************************************************/
void uart_sendint16(uint16_t data)
{
	/*
	Use this to send a 16bit long piece of data
	 */
	while ((UCSR0A & (1 << UDRE0)) == 0);//make sure the data register is cleared
	UDR0 = data;//send the lower bits
	while ((UCSR0A & (1 << UDRE0)) == 0);//make sure the data register is cleared
	UDR0 = (data >> 8); //send the higher bits
	while ((UCSR0A & (1 << UDRE0)) == 0);//make sure the data register is cleared
	UDR0 = '\n';//send a new line just to be sure
}


/**********************************************************************************/
// Function for sending string of data through UART
//Input : char pointer 
//Output: void
//Description : pass the pointer, which points to data to be sent through UART to this function
//				To send multiple data, call this multiple times
/**********************************************************************************/
void uart_sendstr(char *data)
{
	/*
	Use this to send a string, it will split it up into individual parts
	send those parts, and then send the new line code
	 */
	while (*data) {
		while ((UCSR0A & (1 << UDRE0)) == 0);//make sure the data register is cleared
		UDR0 = *data; //goes through and splits the string into individual bits, sends them
		data += 1;//go to new bit in string
	}
	while ((UCSR0A & (1 << UDRE0)) == 0);//make sure the data register is cleared
	UDR0 = '\n';//send a new line just to be sure
}

void buzzer_on (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore | 0x08;
	PORTC = port_restore;
}

void buzzer_off (void)
{
	unsigned char port_restore = 0;
	port_restore = PINC;
	port_restore = port_restore & 0xF7;
	PORTC = port_restore;
}


/**********************************************************************************/
//ISR for UART receive interrupt
//ISR is called when a character is received over UART
//As the Zigbee is interfaced to UART, when ever bot receives signals through
//Zigbee, this ISR is called
//
//Result can be seen in the wheel speed
//'f' or 'F' indicates the format to check whether bot is free to take up next task
//after 'f'  we need to send bot id to get reply from respective bot
//'f'---> 'bot_id' ----> replies free if it is free
//'s' or 'S' indicates the start of the message
//Format to send initial coordinates of the bot: 's'---->'bot_id'------->'i'------->'x_value'------->'y_value'
//Format to send destination coordinates 's'---->'bot_id'------>'d'------->'x_destination'------->'y_destination'
//'z' or 'Z' is used to end the conversation
//As we are coordinating with multiple bots, it is necessary to close the connection as soon as the values are set.
/**********************************************************************************/
SIGNAL(SIG_USART0_RECV)     // ISR for receive complete interrupt
{
	data = UDR0;
	check=1;
	datac=data;         //making copy of data from UDR0 in 'data' variable
	if((data == 'F' || data== 'f') )
	{
		testmsg=1;
	}

	if( (data == botid || data== botid2 ) && testmsg==1 && start == 0)
	{

		if( freebot==0)
		{
			uart_sendstr(testreply);

		}
		testmsg=0;
	}

	if(data == 'S' || data== 's')           // To send its initial coordinates and destination coordinates
	{										// Format to set these values as specified in the above comments
		start=1;
	}
	if(start)
	{
		if((data== botid2 || data== botid) && stop_s==1 && freebot==0)
		{
			stop_s=0;
			id=1;
			uart_sendstr(startreply);
		}
		if((data=='z' || data=='Z') && stop_s==0)
		{
			stop_s=1;
			id=0;
			config_done=1;
		}
		if(stop_s==0 && id==1)        
		{
			if(coordinates==2)
			{
				if(destination_act==2)                 
				{
					if(data==0x30)					// Decoding destination y-coordinate
					{
						y_dest=0;
					}
					else if(data==0x31)
					{
						y_dest=1;
					}
					else if(data==0x32)
					{
						y_dest=2;
					}
					else if(data==0x33)
					{
						y_dest=3;
					}
					else if(data==0x34)
					{
						y_dest=4;
					}
					else if(data==0x35)
					{
						y_dest=5;
					}
					else if(data==0x36)
					{
						y_dest=6;
					}
					destination_act=3;
				}
				if(destination_act==1) 				//ASCII value of 8
				{
					if(data==0x30)					// Decoding destination x-coordinate
					{
						x_dest=0;
					}
					else if(data==0x31)
					{
						x_dest=1;
					}
					else if(data==0x32)
					{
						x_dest=2;
					}
					else if(data==0x33)
					{
						x_dest=3;
					}
					else if(data==0x34)
					{
						x_dest=4;
					}
					else if(data==0x35)
					{
						x_dest=5;
					}
					else if(data==0x36)
					{
						x_dest=6;
					}
					destination_act=2;
				}
				if(data == 'd' || data == 'D') 			//ASCII value of 8
				{
					destination_act=1;  
				}

			}
			if(x_recei==1 && coordinates==1)			// Decoding initial y-coordinate
			{
				if(data==0x30)
				{
					y_coor=0;
				}
				else if(data==0x31)
				{
					y_coor=1;
				}
				else if(data==0x32)
				{
					y_coor=2;
				}
				else if(data==0x33)
				{
					y_coor=3;
				}
				else if(data==0x34)
				{
					y_coor=4;
				}
				else if(data==0x35)
				{
					y_coor=5;
				}
				else if(data==0x36)
				{
					y_coor=6;
				}
				x_recei=0;
				coordinates=2;
			}
			if(coordinates==1 && x_recei==0)				// Decoding initial x-coordinate
			{
				if(data==0x30)
				{
					x_coor=0;
				}
				else if(data==0x31)
				{
					x_coor=1;
				}
				else if(data==0x32)
				{
					x_coor=2;
				}
				else if(data==0x33)
				{
					x_coor=3;
				}
				else if(data==0x34)
				{
					x_coor=4;
				}
				else if(data==0x35)
				{
					x_coor=5;
				}
				else if(data==0x36)
				{
					x_coor=6;
				}
				x_recei=1;
			}
			if(data=='i' || data=='I')
			{
				coordinates=1;
			}
			if(data=='c' || data=='C')
			{
				uart_sendint(x_coor);
				uart_sendint(y_coor);
				uart_sendint(x_dest);
				uart_sendint(x_dest);
			}
		}
	}
}


/**********************************************************************************/
// Function for configuring LCD ports
//Input : void
//Output: void
/**********************************************************************************/
//Function to configure LCD port
void lcd_port_config (void)
{
	DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
	PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
}


/**********************************************************************************/
// Function for configuring ADC pins
//Input : void
//Output: void
/**********************************************************************************/
//ADC pin configuration
void adc_pin_config (void)
{
	DDRF = 0x00;
	PORTF = 0x00;
	DDRK = 0x00;
	PORTK = 0x00;
}


/**********************************************************************************/
//Function to configure ports to enable robot's motion
//Input : void
//Output: void
/**********************************************************************************/
void motion_pin_config (void)
{
	DDRA = DDRA | 0x0F;
	PORTA = PORTA & 0xF0;
	DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
	PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}


/**********************************************************************************/
// Timer 5 initialized in PWM mode for velocity control
// Prescale:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz
/**********************************************************************************/
void timer5_init()
{
	TCCR5B = 0x00;  //Stop
	TCNT5H = 0xFF;  //Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;  //Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;  //Output compare register high value for Left Motor
	OCR5AL = 0xFF;  //Output compare register low value for Left Motor
	OCR5BH = 0x00;  //Output compare register high value for Right Motor
	OCR5BL = 0xFF;  //Output compare register low value for Right Motor
	OCR5CH = 0x00;  //Output compare register high value for Motor C1
	OCR5CL = 0xFF;  //Output compare register low value for Motor C1
	TCCR5A = 0xA9;  /*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
			For Overriding normal port functionality to OCRnA outputs.
			  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/

	TCCR5B = 0x0B;  //WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}


/**********************************************************************************/
// Functions to initialize ADC
//Input  : Void
//Output : void
/**********************************************************************************/
void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;    //MUX5 = 0
	ADMUX = 0x20;   //Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;    //ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}


/**********************************************************************************/
//Function For ADC Conversion
//Input  : channel number of the ADC
//Output : returns the sampled value
/**********************************************************************************/
unsigned char ADC_Conversion(unsigned char Ch)
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;
	ADMUX= 0x20| Ch;
	ADCSRA = ADCSRA | 0x40;   //Set start conversion bit
	while((ADCSRA&0x10)==0);  //Wait for conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}


/**********************************************************************************/
//Function To Print Sesor Values At Desired Row And Coloumn Location on LCD
/**********************************************************************************/
void print_sensor(char row, char coloumn,unsigned char channel)
{

	ADC_Value = ADC_Conversion(channel);
	lcd_print(row, coloumn, ADC_Value, 3);
}


/**********************************************************************************/
//Function for velocity control
/**********************************************************************************/
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}


/**********************************************************************************/
//Function used for setting motor's direction
/**********************************************************************************/
void motion_set (unsigned char Direction)
{
	unsigned char PortARestore = 0;

	Direction &= 0x0F;     // removing upper nibbel for the protection
	PortARestore = PORTA;    // reading the PORTA original status
	PortARestore &= 0xF0;    // making lower direction nibbel to 0
	PortARestore |= Direction; // adding lower nibbel for forward command and restoring the PORTA status
	PORTA = PortARestore;    // executing the command
}


/**********************************************************************************/
//Function to configure INT4 (PORTE 4) pin as input for the left position encoder
/**********************************************************************************/
void left_encoder_pin_config (void)
{
	DDRE  = DDRE & 0xEF;  //Set the direction of the PORTE 4 pin as input
	PORTE = PORTE | 0x10; //Enable internal pull-up for PORTE 4 pin
}


/**********************************************************************************/
//Function to configure INT5 (PORTE 5) pin as input for the right position encoder
/**********************************************************************************/
void right_encoder_pin_config (void)
{
	DDRE  = DDRE & 0xDF;  //Set the direction of the PORTE 4 pin as input
	PORTE = PORTE | 0x20; //Enable internal pull-up for PORTE 4 pin
}


/**********************************************************************************/
//Function to configure INT5 (PORTE 5) pin as input for the left position encoder
/**********************************************************************************/
void left_position_encoder_interrupt_init (void) //Interrupt 4 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
	EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
	sei();   // Enables the global interrupt
}

void right_position_encoder_interrupt_init (void) //Interrupt 5 enable
{
	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
	EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder
	sei();   // Enables the global interrupt
}

//ISR for right position encoder
ISR(INT5_vect)
{
	ShaftCountRight++;  //increment right shaft position count
}


//ISR for left position encoder
ISR(INT4_vect)
{
	ShaftCountLeft++;  //increment left shaft position count
}



void forward (void) //both wheels forward
{
	motion_set(0x06);
}

void back (void) //both wheels backward
{
	motion_set(0x09);
}

void left (void) //Left wheel backward, Right wheel forward
{
	motion_set(0x05);
}

void right (void) //Left wheel forward, Right wheel backward
{
	motion_set(0x0A);
}

void soft_left (void) //Left wheel stationary, Right wheel forward
{
	motion_set(0x04);
}

void soft_right (void) //Left wheel forward, Right wheel is stationary
{
	motion_set(0x02);
}

void soft_left_2 (void) //Left wheel backward, right wheel stationary
{
	motion_set(0x01);
}

void soft_right_2 (void) //Left wheel stationary, Right wheel backward
{
	motion_set(0x08);
}

void stop (void)
{
	motion_set(0x00);
}


/**********************************************************************************/
//Function used for turning robot by specified degrees
/**********************************************************************************/
void angle_rotate(unsigned int Degrees)
{
	float ReqdShaftCount = 0;
	unsigned long int ReqdShaftCountInt = 0;

	ReqdShaftCount = (float) Degrees/ 4.090; // division by resolution to get shaft count
	ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
	ShaftCountRight = 0;
	ShaftCountLeft = 0;

	while (1)
	{
		if((ShaftCountRight >= ReqdShaftCountInt) | (ShaftCountLeft >= ReqdShaftCountInt))
			break;
	}
	stop(); //Stop robot
}


/**********************************************************************************/
//Function used for moving robot forward by specified distance
/**********************************************************************************/
void linear_distance_mm(unsigned int DistanceInMM)
{
	float ReqdShaftCount = 0;
	unsigned long int ReqdShaftCountInt = 0;

	ReqdShaftCount = DistanceInMM / 5.338; // division by resolution to get shaft count
	ReqdShaftCountInt = (unsigned long int) ReqdShaftCount;

	ShaftCountRight = 0;
	while(1)
	{
		if(ShaftCountRight > ReqdShaftCountInt)
		{
			break;
		}
	}
	stop(); //Stop robot
}


/**********************************************************************************/
//Function to move bot forward
//Input  : distance to be moved in mm
//Output : void
//Description : receives the input which indicates the distance to be moved 
//				bot moves DistanceInMM distance
/**********************************************************************************/
void forward_mm(unsigned int DistanceInMM)
{
	forward();
	linear_distance_mm(DistanceInMM);
}


/**********************************************************************************/
//Function to move bot backward
//Input  : distance to be moved in mm
//Output : void
//Description : receives the input which indicates the distance to be moved 
//				bot moves DistanceInMM distance
/**********************************************************************************/
void back_mm(unsigned int DistanceInMM)
{
	back();
	linear_distance_mm(DistanceInMM);
}


/**********************************************************************************/
//Function to move bot left by Degrees input
//Input  : left turn degrees
//Output : void
//Description : receives the input which indicates the turn to be made 
//				bot turns left by Degrees amount
/**********************************************************************************/
void left_degrees(unsigned int Degrees)
{
	// 88 pulses for 360 degrees rotation 4.090 degrees per count
	left(); //Turn left
	angle_rotate(Degrees);
}


/**********************************************************************************/
//Function to move bot right by Degrees input
//Input  : right turn degrees
//Output : void
//Description : receives the input which indicates the turn to be made 
//				bot turns right by Degrees amount
/**********************************************************************************/
void right_degrees(unsigned int Degrees)
{
	// 88 pulses for 360 degrees rotation 4.090 degrees per count
	right(); //Turn right
	angle_rotate(Degrees);
}


/**********************************************************************************/
//Function to initialize the ports
//Input  : void
//Output : void
//Description : configures LCD ports,ADC pins,bot motor pins
/**********************************************************************************/
void port_init()
{
	lcd_port_config();
	adc_pin_config();
	motion_pin_config();  //robot motion pins config
	left_encoder_pin_config(); //left encoder pin config
	right_encoder_pin_config(); //right encoder pin config
}


/**********************************************************************************/
//Function to initialize all the devices
//Input  : void
//Output : void
//Description : configures LCD ports,ADC pins,bot motor pins
/**********************************************************************************/
void init_devices()
{
	cli(); //Clears the global interrupt
	port_init();  //Initializes all the ports
	left_position_encoder_interrupt_init();
	right_position_encoder_interrupt_init();
	adc_init();
	timer5_init();
	uart_start();
	sei();   // Enables the global interrupt
}


/**********************************************************************************/
//Function to check the obstacle in the forward path
//Input  : void
//Output : void
//Description : samples the sharp sensor and IR sensor values
//				checks if the sensor values are under acceptable values
//				If obstacle is detected, waits for the specified time and sample again
//				Returns when the obstacle moves
/**********************************************************************************/
void checkobst()
{
	sample_sensor();
	while(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor)
	{
		stop();
		_delay_ms(delayvalueonward);
		sample_sensor();
	}
}


/**********************************************************************************/
//Function to check the obstacle in the reverse path
//Input  : void
//Output : void
//Description : samples the sharp sensor and IR sensor values
//				checks if the sensor values are under acceptable values
//				If obstacle is detected, waits for the specified time and sample again
//				Returns when the obstacle moves
/**********************************************************************************/
void checkobst_backpath()
{
	sample_sensor();
	while(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor)
	{
		stop();
		//back_mm(80);
		_delay_ms(delayvalueback);
		sample_sensor();
	}
}


/**********************************************************************************/
//Function to sample LEFT, Right , center WL sensor values
//Input  : void
//Output : void
//Description : samples the WL sensors
/**********************************************************************************/
void adc_sample()
{
	Left_white_line = ADC_Conversion(3);    //Getting data of Left WL Sensor
	Center_white_line = ADC_Conversion(2);  //Getting data of Center WL Sensor
	Right_white_line = ADC_Conversion(1);   //Getting data of Right WL Sensor
}


/**********************************************************************************/
//Function to route the bot to the destination table
//Input  : void
//Output : int
//Description : This function is called repeatedly till the destination table is reached
//			    The following tasks are handles in this function
//				First, it covers the 'x' distance and matches the destination 'x' coordinate with present 'x' coordinate
//				Then, it matches the destination 'y' coordinate with the present 'y' coordinate 
//				Updating the present coordinates of the robot based on the WL sensor readings
//				Taking the right decision to whether turn right or left when 'x' coordinates matches
//				Reaching the particular table.
/**********************************************************************************/
int route()
{
	adc_sample();					//WL sensor reading
	sample_sensor();				//Sharp sensor and IR sensor readings
	flag=0;							
	obstflag=0;						//Obstacle indication
	x_coor=currentcountx;			//Present 'x' coordinates
	y_coor=currentcounty;			//Present 'y' coordinates
	print_sensor(1,1,3);    		//Prints value of White Line Sensor1
	print_sensor(1,5,2);    		//Prints Value of White Line Sensor2
	print_sensor(1,9,1);    		//Prints Value of White Line Sensor3
	lcd_print(2,1,currentcountx,2);
	lcd_print(2,5,currentcounty,2);

	while(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor)
	{
		obstflag=1;					//obstacle found
		stop();					
		buzzer_on();	
		_delay_ms(delayvalueonward);
		buzzer_off();
		sample_sensor();
		//handle_obst_onward();
		checkobst();				//Function to handle the obstacle
	}

	if(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor)
	{
		obstflag=1;
	}
	else
	{
		obstflag=0;
	}

	while(Center_white_line>=BLACK_LINE && (Left_white_line<BLACK_LINE)  && (Right_white_line<BLACK_LINE) && (obstflag==0))  //4 cases
	{
		checkobst();				//Function to handle the obstacle
		flag=1;
		last_choice=1;				//To indicate the last option selected in the last route function 
		forward_mm(5);
		back_s=0;					
		adc_sample();
	}

	while(Center_white_line>=BLACK_LINE && (Left_white_line>=BLACK_LINE)  && (Right_white_line<BLACK_LINE) && (obstflag==0))
	{
		checkobst();
		last_choice=2;
		left_degrees(2);			//Turn left
		forward_mm(5);				//Move forward 5 mm
		back_s=0;
		adc_sample();
	}

	while(Center_white_line>=BLACK_LINE && (Left_white_line<BLACK_LINE)  && (Right_white_line>=BLACK_LINE) && (obstflag==0))
	{
		checkobst();
		last_choice=3;
		right_degrees(2);			//Turn right by 2 degrees
		forward_mm(5);				//Move forward 5 mm
		back_s=0;
		adc_sample();
	}

	while(Center_white_line<BLACK_LINE && (Left_white_line<BLACK_LINE)  && (Right_white_line>=BLACK_LINE) && (obstflag==0))
	{
		checkobst();
		last_choice=4;
		right_degrees(5);
		back_s=0;
		forward_mm(2);
		adc_sample();

	}

	while(Center_white_line<BLACK_LINE && (Left_white_line>=BLACK_LINE)  && (Right_white_line<=BLACK_LINE) && (obstflag==0))
	{
		checkobst();
		last_choice=5;
		left_degrees(5);
		back_s=0;
		forward_mm(2);
		adc_sample();

	}

	if((Center_white_line>=BLACK_LINE) && (Left_white_line>=BLACK_LINE)  && (Right_white_line>=BLACK_LINE) && (flag==0) && (last_choice!=6) && (last_choice!=9) && (obstflag==0) )
	{
		last_choice=6;
		flag=1;
		back_s=0;
		stop();
		if(flagturn==0)									// Still matching present 'x' coordinate with destination 'x' coordinate
		{
			currentcountx = currentcountx+1;			// Increment present 'x' coordinate
		}
		if(flagturn==1)									// 'x' coordinate has been matched
		{
			if(take_left==1)							// Taken left turn after matching 'x' coordinate
			{
				currentcounty=currentcounty+1;			
			}
			else 										// taken right after matchjing 'x' coordinate
			{
				currentcounty=currentcounty-1;			// Decrement 
			}
		}

		forward_mm(5);
		if((currentcountx == des_countx) && x_reached==0 && (obstflag==0)) 		// 'x' coodinates matched
		{
			//_delay_ms(500);
			flagturn=1;															// Take turn
			forward_mm(70); 													//Moves robot forward 100mm					
			if(take_left==1)													// take turn based in destination table
			{
				left_degrees(90);											
			}
			else
			{
				right_degrees(90);
			}
			_delay_ms(500);
			forward();
			x_reached=1;														// 'x' matched
			forward_mm(25);
		}
		if(take_left==1)														// Take decision based on destination coordinates
		{
			if((currentcounty == des_county))
			{
				_delay_ms(500);
			}
		}
		else
		{
			if((currentcounty == (des_county+1)))
			{
				_delay_ms(500);
			}
		}
	}
	// Both 'x' and 'y' coordinates matched
	// Moving towards table
	if(((currentcountx == des_countx) && (currentcounty == des_county) && (take_left==1) && (obstflag==0)) || ((currentcountx == des_countx) && (currentcounty == (des_county+1)) && (take_left==0) && (obstflag==0)))
	{
		last_choice=7;
		if(take_left==1)
		{
			forward_mm_totable(270);								
		}
		else
		{
			forward_mm_totable(200);
		}
		if(take_left==1)
		{
			left_degrees(90);
		}
		else right_degrees(90);


		while(count>0)
		{
			_delay_ms(60);
			count--;
		}
		config_done=0;
		flag_reached=1;
		flagturn=0;
		x_reached=0;
		back_s=0;
		count=100;
	}
	if(Center_white_line<BLACK_LINE && Left_white_line<BLACK_LINE && Right_white_line<BLACK_LINE && (obstflag==0))
	{
		adjust_path();					// Adjust path if bot moves of the track
	}
	while((Center_white_line>=BLACK_LINE) && (Left_white_line>=BLACK_LINE)  && (Right_white_line>=BLACK_LINE)  && (last_choice==6 || last_choice==9) && (obstflag==0) )
	{
		//forward_mm(5);
		last_choice=9;
		checkobst();
		forward_mm(2);
		adc_sample();
	}
	//route(des_countx,des_county,0,0);
	return (1);
}


/**********************************************************************************/
//Function to back route the bot to the counter
//Input  : void
//Output : int
//Description : This function is called repeatedly till the counter is reached
//			    The following tasks are handles in this function
//				First, it covers the 'x' distance and matches the counter 'x' coordinate with present 'x' coordinate
//				Then, it matches the destination 'y' coordinate with the present 'y' coordinate 
//				Updating the present coordinates of the robot based on the WL sensor readings
//				Taking the right decision to whether turn right or left when 'x' coordinates matches
//				Reaching the counter.
/**********************************************************************************/
int backroute()
{
	adc_sample();
	sample_sensor();
	flag=0;
	obstflag=0;
	x_coor=currentcountx;
	y_coor=currentcounty;
	print_sensor(1,1,3);    //Prints value of White Line Sensor1
	print_sensor(1,5,2);    //Prints Value of White Line Sensor2
	print_sensor(1,9,1);    //Prints Value of White Line Sensor3
	lcd_print(2,1,currentcountx,2);
	lcd_print(2,5,currentcounty,2);
	while(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor)
	{
		obstflag=1;								//Obstacle found
		stop();
		buzzer_on();
		_delay_ms(delayvalueback);				//Backpath delay to sense next value
		buzzer_off();
		sample_sensor();
		handle_obst_backward();					//handling the obstacle with rerouting if the obstacle does not move with the delay time
	}
	while(Center_white_line>=BLACK_LINE && (Left_white_line<BLACK_LINE)  && (Right_white_line<BLACK_LINE) && (obstflag==0))  //4 cases
	{
		sample_sensor();
		while(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor)
		{
			obstflag=1;
			stop();
			//buzzer_on();
			_delay_ms(delayvalueback);
			sample_sensor();
			handle_obst_backward();
		}
		flag=1;
		last_choice=1;
		forward_mm(5);
		back_s=0;
		adc_sample();
	}
	while(Center_white_line>=BLACK_LINE && (Left_white_line>=BLACK_LINE)  && (Right_white_line<BLACK_LINE) && (obstflag==0))
	{
		sample_sensor();
		while(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor)

		{
			obstflag=1;
			stop();
			//buzzer_on();
			_delay_ms(delayvalueback);
			sample_sensor();
			handle_obst_backward();
		}
		last_choice=2;
		left_degrees(2);
		forward_mm(5);
		back_s=0;
		adc_sample();
	}
	while(Center_white_line>=BLACK_LINE && (Left_white_line<BLACK_LINE)  && (Right_white_line>=BLACK_LINE) && (obstflag==0))
	{

		sample_sensor();
		while(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor)

		{
			obstflag=1;
			stop();
			//buzzer_on();
			_delay_ms(delayvalueback);
			sample_sensor();
			handle_obst_backward();
		}
		last_choice=3;
		right_degrees(2);
		forward_mm(5);
		back_s=0;
		adc_sample();
	}
	while(Center_white_line<BLACK_LINE && (Left_white_line<BLACK_LINE)  && (Right_white_line>=BLACK_LINE) && (obstflag==0))
	{
		sample_sensor();
		while(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor)

		{
			obstflag=1;
			stop();
			//buzzer_on();
			_delay_ms(delayvalueback);
			sample_sensor();
			handle_obst_backward();
		}
		last_choice=4;
		right_degrees(5);
		forward_mm(2);
		back_s=0;
		adc_sample();
	}
	while(Center_white_line<BLACK_LINE && (Left_white_line>=BLACK_LINE)  && (Right_white_line<=BLACK_LINE) && (obstflag==0))
	{
		sample_sensor();
		while(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor)

		{
			obstflag=1;
			stop();
			//buzzer_on();
			_delay_ms(delayvalueback);
			sample_sensor();
			handle_obst_backward();
		}
		last_choice=5;
		left_degrees(5);
		forward_mm(2);
		back_s=0;
		adc_sample();
	}

	if((Center_white_line>=BLACK_LINE) && (Left_white_line>=BLACK_LINE)  && (Right_white_line>=BLACK_LINE) && (flag==0) && (last_choice!=6) && (last_choice!=9) && (obstflag==0))
	{
		last_choice=6;
		flag=1;
		back_s=0;
		stop();
		if(flagrightturn==0)
		{
			if(dest_backward==1)
			{
				currentcounty = currentcounty-1;
			}
			else 
			{
				currentcounty = currentcounty+1;
			}
		}
		if(flagrightturn==1)
		{
			currentcountx=currentcountx-1;
		}
		forward_mm(5);
		if((currentcounty == des_county) && y_reached==0)
		{
			_delay_ms(500);
			flagrightturn=1;
			forward_mm(50); //Moves robot forward 100mm
			if(dest_backward==1)
			{
				right_degrees(90);
			}
			else
			{
				left_degrees(90);
			}

			_delay_ms(500);
			forward();
			y_reached=1;
			forward_mm(20);
		}
		if((currentcountx == des_countx))
		{
			_delay_ms(500);
		}
	}

	if((currentcountx == des_countx) && (currentcounty == des_county) )
	{
		last_choice=0;
		forward_mm(50);
		config_done=0;
		flag_startreached=1;
		flagturn=0;
		flagrightturn=0;
		y_reached=0;
		back_s=0;
		count=100;
		dest_backward=0;
	}
	if(Center_white_line<BLACK_LINE && Left_white_line<BLACK_LINE && Right_white_line<BLACK_LINE && (obstflag==0))
	{
		last_choice=8;
		adjust_path();
	}
	while((Center_white_line>=BLACK_LINE) && (Left_white_line>=BLACK_LINE)  && (Right_white_line>=BLACK_LINE)  && (last_choice==6 || last_choice==9) && (obstflag==0)  )
	{
		sample_sensor();
		while(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor)

		{
			obstflag=1;
			stop();
			//buzzer_on();
			_delay_ms(delayvalueback);
			sample_sensor();
			handle_obst_backward();
		}
		forward_mm(5);
		last_choice=9;
		adc_sample();

	}

	if(flag_startreached==1)
	{
		reposition(); 
	}

	return (1);
}


/**********************************************************************************/
//Function to take robot from finish point to start point
//Input  : void
//Output : void
//Description : this function is called when robot reaches the counter.
//			    this function takes the robot to the start position where it will be in a position to receive the next order
//				First, bot takes a left turn and then moves forwrd till the time it reaches intersection
//				Then, takes left again and aligns itself to get ready for next order
/**********************************************************************************/
void reposition()
{
	forward_mm(10);
	left_degrees(90);					// take left by 90 degrees
	obstflag=0;							// clearing obstacle flag
	adc_sample();						// sample ADC values
	sample_sensor();					// sample Obstacle sensor values
	checkobst_backpath();				// check for obstacles

	while(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 < thresh_Front_IR_Sensor)
	{
		sample_sensor();
		obstflag=1;						// set the flag if obstacle is detected
		stop();
	}

	while(1)							// loop till a intersection is found(start point)
	{
		adc_sample();
		sample_sensor();
		obstflag=0;						
		if(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor)
		{
			stop();
			obstflag=1;
		}

		if((Center_white_line>BLACK_LINE && Left_white_line>BLACK_LINE && Right_white_line>BLACK_LINE) && obstflag ==0)
		{
			break;						// break when the start point is reached
		}

		if(Center_white_line>=BLACK_LINE && Left_white_line>=BLACK_LINE && Right_white_line<BLACK_LINE && obstflag ==0)
		{
			left_degrees(1);			// adjust bot to the path
		}

		if(Center_white_line>=BLACK_LINE && Left_white_line<BLACK_LINE && Right_white_line>=BLACK_LINE && obstflag ==0)
		{
			right_degrees(1);			// adjust bot to the path
		}

		if(Center_white_line>=BLACK_LINE && Left_white_line<BLACK_LINE && Right_white_line<BLACK_LINE && obstflag ==0)
		{
			forward_mm(5);				// move bot along the line
		}

		if(Center_white_line<BLACK_LINE && Left_white_line<BLACK_LINE && Right_white_line>=BLACK_LINE && obstflag ==0)
		{
			right_degrees(2);			// adjust bot
		}

		if(Center_white_line<BLACK_LINE && Left_white_line>=BLACK_LINE && Right_white_line<BLACK_LINE && obstflag ==0)
		{
			left_degrees(2);
		}

		if(Center_white_line<BLACK_LINE && Left_white_line<BLACK_LINE && Right_white_line<BLACK_LINE && obstflag ==0)
		{
			//stop();
			adjust_path();
		}
	}
	forward_mm(80);
	_delay_ms(100);
	left_degrees(90);
	_delay_ms(100);
	back_mm(15);
	stop();
}


/**********************************************************************************/
//Main function which coordinates with other functions
//Input  : void
//Output : int returns 1
//Description : Initializes the ports
//				Initializes the pins
//				Initialises the devices
//				Waits till the current position and destination points are recived through Zigbee
//				After receiving, starts the route functionality by calling Route function.
//				When the destination is reached, reverses the coordinates of the destination to counter.
//				calls backroute function to reach the counter.
//				Again comes back and initializes for the next order
/**********************************************************************************/
//Main Function

int main(void)
{
	obstflag=0;
	while(1)
	{
		init_devices();								// Initialization
		lcd_set_4bit();		
		lcd_init();
		flag_startreached=0;						// clearing the flags
		take_left=0;				
		back_reroute=0;
		while(config_done!=1);						// received current and destination points

		des_countx = x_dest; 						//desired table no from Zigbee
		des_county = y_dest;				
		currentcountx=x_coor;						//current position of the bot
		currentcounty=y_coor;
		if(currentcounty>y_dest)					// take decision to turn left or right
		{
			take_left=0;
		}	
		else take_left=1;
		uart_sendstr(coordreply);
		freebot=1;
		while(1)
		{
			freebot=1;								// set bot in use variable
			buzzer_off();
			route();								// Route function
			if (flag_reached ==1)					// reached table
			{
				break;
			}

		}
		if (flag_reached ==1)
		{	
			int count=0;
			while(1)
			{
				Left_IR_sensor1 =ADC_Conversion(4);		// Checking the weight sensor
				//print_sensor(2,6,4);
				while(Left_IR_sensor1 < thresh_weightsensor)
				{
					stop();
					//print_sensor(2,6,4);
					buzzer_on();
					_delay_ms(500);
					Left_IR_sensor1 =ADC_Conversion(4);
					count=count+1;
					if(count>10)
					{
						break;
					}
				}
				break;
			}
			buzzer_off();

			//buzzer_on();
			_delay_ms(1000);
			//buzzer_off();
			_delay_ms(1000);
			//buzzer_on();
			//_delay_ms(1000);
			//buzzer_off();
			lcd_print(2,8,y_dest,1);
			lcd_print(2,10,currentcounty,1);
			if(y_dest>=2)										// changing coordinates to counter coordinates
			{
				left_degrees(90);
				des_countx = 0; 								//desired table no from Zigbee
				des_county = 2;
				currentcountx=x_dest;
				currentcounty=y_dest+1;
				dest_backward=1;
			}
			else
			{
				right_degrees(90);
				_delay_ms(100);
				back_mm(50);
				des_countx = 0; 								//desired table no from Zigbee
				des_county = 2;
				currentcountx=x_dest;
				currentcounty=y_dest;
				dest_backward=0;
			}
			flag_reached = 0;
			while(1)
			{
				buzzer_off();
				backroute();
				if(flag_startreached==1)						// reached counter back
				{
					_delay_ms(1000);
					start=0;
					freebot=0;

					break;
				}
			}

		}
	}
}


/**********************************************************************************/
//function to handle obstacle in the backward path
//Input  : void
//Output : void
//Description : Responsible for rerouting the bot based on the obstacle position
//				
/**********************************************************************************/
void handle_obst_backward()
{
	if((y_reached == 0) && currentcounty>2)			// bot is in the backward direction and bot has not matched counter 'y' coordinate
	{
		//for(i=0;i<BOT_DELAY_BACKWARDY;i++)
		//{
		lcd_print(2,3,9,1);
		_delay_ms(2000);
		sample_sensor();
		if(!(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor))
		{
			return;
		}
		sample_sensor();
		//}
		right_degrees(180);							// rerouting to other path by taking 180 degree turn
		back_mm(30);
		sample_sensor();
		//if(!(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor))
		//{
		grid_forward();								// moving forward by a grid_forward
		//_delay_ms(5000);
		left_degrees(90);
		lcd_print(2,1,currentcountx,2);
		//_delay_ms(5000);
		while(currentcountx!=1)						// rerouting till current 'x' is equal to 1
		{
			grid_forward();
			//_delay_ms(5000);
			currentcountx=currentcountx-1;
			lcd_print(2,1,currentcountx,2);
		}
		back_reroute=1;
		if((Center_white_line>=BLACK_LINE && Left_white_line>=BLACK_LINE && Right_white_line>=BLACK_LINE))
		{
			left_degrees(90);
		}
		else buzzer_on();

		//}

	}	
	else if((y_reached == 0) && currentcounty<1)	// rerouting when present 'y coordinate is lewss than 1' 
	{
		//for(i=0;i<BOT_DELAY_BACKWARDY;i++)
		//{
		lcd_print(2,3,9,1);
		_delay_ms(2000);
		sample_sensor();
		if(!(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor))
		{
			return;
		}
		sample_sensor();


		right_degrees(180);
		back_mm(30);
		sample_sensor();
		//if(!(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor))
		//{
		grid_forward();
		//_delay_ms(5000);
		right_degrees(90);
		lcd_print(2,1,currentcountx,2);
		//_delay_ms(5000);
		while(currentcountx!=1)
		{
			grid_forward();
			adc_sample();
			while(Center_white_line>=BLACK_LINE && Left_white_line>=BLACK_LINE && Right_white_line>=BLACK_LINE)
			{
				forward_mm(5);
			}
			//_delay_ms(5000);
			currentcountx=currentcountx-1;
			lcd_print(2,1,currentcountx,2);
		}
		back_reroute=1;
		if((Center_white_line>=BLACK_LINE && Left_white_line>=BLACK_LINE && Right_white_line>=BLACK_LINE))
		{
			right_degrees(90);
		}
		else buzzer_on();

		//}

	}
	else if((y_reached == 0) && currentcounty<2 && currentcounty==1 && back_reroute == 1)
	{
		lcd_print(2,3,9,1);
		_delay_ms(2000);
		sample_sensor();
		if(!(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor))
		{
			return;
		}
		back_mm(60);
		stop();
		_delay_ms(delayvalueback);
		sample_sensor();
		while(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor)
		{

			stop();
			_delay_ms(delayvalueback);
			sample_sensor();
		}
	}
	else if(currentcountx>2 && currentcounty>=2)
	{
		_delay_ms(2000);
		sample_sensor();
		if(!(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor))
		{
			return;
		}
		back_mm(60);
		stop();
		_delay_ms(delayvalueback);
		sample_sensor();
		while(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor)
		{

			stop();
			_delay_ms(delayvalueback);
			sample_sensor();
		}
	}
	else 
	{	

		checkobst_backpath();
	}
}


/**********************************************************************************/
//function to move bot grid by grid when rerouting is taking place
//Input  : void
//Output : void
//Description : Responsible for moving bot by 1 grid
//				
/**********************************************************************************/
void grid_forward()
{
	adc_sample(); 
	lcd_print(2,3,8,1);
	while(!(Center_white_line>=BLACK_LINE && Left_white_line>=BLACK_LINE && Right_white_line>=BLACK_LINE) )
	{
		//forward_mm(5);

		checkobst_backpath();

		if(Center_white_line>=BLACK_LINE && Left_white_line>=BLACK_LINE && Right_white_line<BLACK_LINE)
		{
			left_degrees(1);

		}
		if(Center_white_line>=BLACK_LINE && Left_white_line<BLACK_LINE && Right_white_line>=BLACK_LINE)
		{
			right_degrees(1);

		}
		if(Center_white_line>=BLACK_LINE && Left_white_line<BLACK_LINE && Right_white_line<BLACK_LINE)
		{
			forward_mm(5);

		}
		if(Center_white_line<BLACK_LINE && Left_white_line<BLACK_LINE && Right_white_line>=BLACK_LINE)
		{
			right_degrees(2);

		}
		if(Center_white_line<BLACK_LINE && Left_white_line>=BLACK_LINE && Right_white_line<BLACK_LINE)
		{
			left_degrees(2);

		}
		if(Center_white_line<BLACK_LINE && Left_white_line<BLACK_LINE && Right_white_line<BLACK_LINE)
		{
			//stop();
			adjust_path();

		}
		adc_sample();
		sample_sensor();
		if(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 )
		{
			obstflag=1;
			stop();	
		}
	}
	forward_mm(90);
	return;
}


/**********************************************************************************/
//function to adjust the robot path when it moves out of the track
//Input  : void
//Output : void
//Description : Responsible for bringing robot back to track when it moves out of the track
//				
/**********************************************************************************/
void adjust_path()
{
	adc_sample();
	lcd_print(2,3,7,1);
	int backcount=0;
	while(Center_white_line<BLACK_LINE && Left_white_line<BLACK_LINE && Right_white_line<BLACK_LINE)
	{	
		lcd_print(2,3,5,1);
		left_degrees(2);
		adc_sample();
		if(Center_white_line>=BLACK_LINE || Left_white_line>=BLACK_LINE || Right_white_line>=BLACK_LINE)
		{
			break;
		}
		right_degrees(4);
		adc_sample();

		if(Center_white_line>=BLACK_LINE || Left_white_line>=BLACK_LINE || Right_white_line>=BLACK_LINE)
		{
			break;
		}

		right_degrees(2);
		adc_sample();
		if(Center_white_line>=BLACK_LINE || Left_white_line>=BLACK_LINE || Right_white_line>=BLACK_LINE)
		{
			break;
		}
		left_degrees(4);
		adc_sample();

		if(Center_white_line>=BLACK_LINE || Left_white_line>=BLACK_LINE || Right_white_line>=BLACK_LINE)
		{
			break;
		}


		back_mm(5);
		if(backcount > 20)
		{
			break;
		}
		backcount++;
	}
}


/**********************************************************************************/
//function to sample sensor values
//Input  : void
//Output : void
//Description : Responsible for sampling sensor values
//				
/**********************************************************************************/
void sample_sensor()
{
	Left_IR_sensor2 = ADC_Conversion(5);
	Front_IR_Sensor = ADC_Conversion(6);
	Right_IR_sensor4 = ADC_Conversion(7);
	Front_Sharp_Sensor = ADC_Conversion(11);
}

/**********************************************************************************/
//function to move bot to table when it reaches the coordinate
//Input  : distance to move with following the black line
//Output : void
//Description : distance to move with following the black line
//				
/**********************************************************************************/
void forward_mm_totable(int distance_togo)
{
	int distance_travelled=0;
	adc_sample();

	while(1)
	{
		adc_sample();
		sample_sensor();
		obstflag=0;
		if(Front_Sharp_Sensor>thresh_Front_Sharp_Sensor || Front_IR_Sensor<thresh_Front_IR_Sensor || Left_IR_sensor2 <thresh_Front_IR_Sensor || Right_IR_sensor4 <thresh_Front_IR_Sensor)
		{
			stop();
			obstflag=1;
		}

		if(distance_travelled >= distance_togo)
		{
			break;
		}

		if(Center_white_line>=BLACK_LINE && Left_white_line>=BLACK_LINE && Right_white_line<BLACK_LINE && obstflag ==0)
		{
			left_degrees(1);

		}
		if(Center_white_line>=BLACK_LINE && Left_white_line<BLACK_LINE && Right_white_line>=BLACK_LINE && obstflag ==0)
		{
			right_degrees(1);

		}
		if(Center_white_line>=BLACK_LINE && Left_white_line<BLACK_LINE && Right_white_line<BLACK_LINE && obstflag ==0)
		{
			forward_mm(5);
			distance_travelled=distance_travelled+5;

		}
		if(Center_white_line>=BLACK_LINE && Left_white_line>=BLACK_LINE && Right_white_line>=BLACK_LINE && obstflag ==0)
		{
			forward_mm(5);
			distance_travelled=distance_travelled+5;

		}
		if(Center_white_line<BLACK_LINE && Left_white_line<BLACK_LINE && Right_white_line>=BLACK_LINE && obstflag ==0)
		{
			right_degrees(2);

		}
		if(Center_white_line<BLACK_LINE && Left_white_line>=BLACK_LINE && Right_white_line<BLACK_LINE && obstflag ==0)
		{
			left_degrees(2);

		}
		if(Center_white_line<BLACK_LINE && Left_white_line<BLACK_LINE && Right_white_line<BLACK_LINE && obstflag ==0)
		{
			//stop();
			adjust_path();

		}
	}


}







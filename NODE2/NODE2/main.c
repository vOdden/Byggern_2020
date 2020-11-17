#include "sam.h"
#include "main.h"

//MOVE TO IR.h
#define IR_TRIGGER_VOLTAGE 0.200 //Voltage treshold for goal/not goal
#define IR_TRIGGER_LEVEL (int)(IR_TRIGGER_VOLTAGE*4095/3.3)
#define IR_FILTER_COUNT 50
int IR_triggered();

extern volatile uint8_t ready;
extern volatile uint8_t start;
extern volatile uint8_t Goal;
extern volatile uint8_t Heartbeat;
extern volatile pos_t position;


void setup(void); //function to setup the different drivers.

int main(void)
{
    	/* Initialize the SAM system */
   	SystemInit();
	configure_uart();

	PMC->PMC_PCER0;	//
	PIOA->PIO_PER = PIO_PER_P19 | PIO_PER_P20; //
	PIOA->PIO_OER = PIO_OER_P19 | PIO_PER_P20; // 
	PIOA->PIO_SODR = PIO_SODR_P19 | PIO_SODR_P20; //Turns on LED on Arduino shield
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	//Setup and calibrating

	setup();
	printf("Printf Works \r");


	volatile int a = 0;
	
    while (1) 
    {

		if(start)
		{
			
			
			if(a == 0)			
			{
				PID_start();
				a = 1;
			}

			Servo_set_position(position.x, position.y); // Sends positions data to servo 
						
			if(position.button)// If the user preses the joystick button, shoot the solenoid.
			{
				Solenoid_shoot();
				position.button = 0;
			}
			if(IR_triggered()) // if IR beam is cut off, GOAL!
			{
				start = 0;	//Stop the PID.
				Send_msg(GOAL); //Send GOAL to Node 1
			}	
		}
		else
		{
			PID_stop();
			a = 0;
			
			
			
 			Delay(500);
			Send_msg(READY); //Node 2 is ready to start a new game.	
			 
		}
    }
}

int IR_triggered() //sends a 1 if there is a goal.
{
	int val = ADC_read(ADC_CH_IR); //Read the value from the reciving end of IR.
	
	//printf("%d \r", val);
	
	static int filter_counter;
	
	if(val < IR_TRIGGER_LEVEL)
	{
		if(filter_counter >= IR_FILTER_COUNT) // check to see if the IR beam is broken long enough.
		{
			filter_counter = 0;
			printf("GOAL TRIGGERED! \r"); //There is a GOAL!
			return 1;
		}
		else
		{
			filter_counter++;
		}
	}	
	return 0;
}



void setup(void) // initiate the different drivers
{
	CAN_init(0);
	Delay(1);

	Motor_init();
	Delay(1);

	Solenoid_init();
	Delay(1);

	ADC_init(ADC_CH_IR);	
	Delay(1);

	DAC_init();
	Delay(1);

	Servo_init();

	PID_init();
	PID_encoder_init();

	//PWM_calibrate();

	Send_msg(READY);
}

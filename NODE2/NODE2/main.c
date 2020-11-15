#include "sam.h"
#include "main.h"

//MOVE TO IR.h
#define IR_TRIGGER_VOLTAGE 0.200
#define IR_TRIGGER_LEVEL (int)(IR_TRIGGER_VOLTAGE*4095/3.3)
#define IR_FILTER_COUNT 50
int IR_triggered();

extern volatile uint8_t ready;
extern volatile uint8_t start;
extern volatile uint8_t Goal;
extern volatile uint8_t Heartbeat;
extern volatile pos_t position;

volatile int run = 0;
int running(void)
{
	if(start)
	{
		int temp = run;	
		run = 1;
		return temp;
	}
	else
	{
		return run = 0;
	}
}

void setup(void);

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	configure_uart();

	PMC->PMC_PCER0;	
	PIOA->PIO_PER = PIO_PER_P19 | PIO_PER_P20;
	PIOA->PIO_OER = PIO_OER_P19 | PIO_PER_P20;
	PIOA->PIO_SODR = PIO_SODR_P19 | PIO_SODR_P20;
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	//oppsett og kalirering

	setup();
	printf("Printf funker \r");


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

			Servo_set_position(position.x, position.y);
						
			if(position.button)
			{
				Solenoid_shoot();
				position.button = 0;
			}
			if(IR_triggered())
			{
				start = 0;
				Send_msg(GOAL);
			}	
		}
		else
		{
			PID_stop();
			a = 0;
			
			
			
 			Delay(500);
			Send_msg(READY);	
			 
// 			if(!start)
// 			{

// 			}
// 			else
// 			{
// 				Send_msg(START);
// 			}
		}
    }
}

int IR_triggered()
{
	int val = ADC_read(ADC_CH_IR);
	
	//printf("%d \r", val);
	
	static int filter_counter;
	
	if(val < IR_TRIGGER_LEVEL)
	{
		if(filter_counter >= IR_FILTER_COUNT)
		{
			filter_counter = 0;
			printf("GOAL TRIGGERED! \r");
			return 1;
		}
		else
		{
			filter_counter++;
		}
	}	
	return 0;
}



void setup(void)
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




	/*
	printf("Kalibrerer motor om: \r");
	printf("3\r");
	Delay(1000);
	printf("2\r");
	Delay(1000);
	printf("1\r");
	Delay(1000);
	printf("0\r");
	*/
	PID_init();
	PID_encoder_init();

	//PWM_calibrate();

	Send_msg(READY);
}
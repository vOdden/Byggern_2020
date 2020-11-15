#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#ifdef EX
#include "Exercise/Exercise.h"
#endif

void Send_position(void);

volatile uint8_t running = 0;
extern volatile uint8_t ready;
extern volatile uint8_t start;
extern volatile uint8_t Goal;
extern volatile uint8_t Heartbeat;
extern volatile pos_t position;
extern volatile uint32_t ms_tick;

volatile uint16_t current_score = 0;
uint16_t  historic_score[10] = {0};

int main(void)
{
	GPIO_init();
	SRAM_init();
	UART_init(UBRR);
	Joystick_init();
	slider_init();
	OLED_init();
	Menu_init();
	CAN_init(MODE_NORMAL);
	
	Timer_init();
	Timer3_start();
	
	sei();	

	Send_msg(READY);
	printf("start \n");
	
    while (1) 
    {	

		Send_position();
		_delay_ms(100);
		
		if(!running)
		{
 			Menu_selection();
			_delay_ms(100);
			Goal = 0;
		}
		else
		{
			if(Goal)
			{
				Menu_write_game_over(current_score);
				Timer1_stop();
				start = 0;
				running = 0;
				current_score = 0;	
			}
			else
			{
				Menu_write_score(current_score++);
			}
		}
	}
 		
	return 0;
}


void GPIO_init(void)
{
	DDRB &= ~(1 << PB1);
	DDRB |= 1 << PB0;
	PORTB |= 1 << PB1;
	
	DDRE |= 1 << PE2;
	
	DDRE &= ~(1 << JOYSTICK_PIN);
	PORTE |= 1 << JOYSTICK_PIN;
	
	set_bit(MCUCR, ISC01);
	clear_bit(MCUCR, ISC00);
	set_bit(GICR, INT2);
	
//Sliders missing

	DDRD &= ~(1 << 2);
	
	GICR |= 1<<6;
	MCUCR |= 0x2;	
}

void invert_string(char *s)
{
	while(*s)
	{
		*s = ~*s;
		s++;
	}
};


void Send_position(void)
{
	Joystick_pos_t j_pos = Joystick_get_position();
	Slider_pos_t s_pos = Slider_get_position();
	
	CAN_MESSAGE pos_msg;
	pos_msg.ID = 'P';
	pos_msg.data[0] = Joystick_read_button();
	pos_msg.data[1] = j_pos.dir;
	pos_msg.data[2] = j_pos.x_pos;
	pos_msg.data[3] = j_pos.y_pos;
	pos_msg.data[4] = s_pos.R_pos;	
	pos_msg.data[5] = s_pos.L_pos;
	
	pos_msg.data_length = 6;
	
	CAN_send(&pos_msg);
}
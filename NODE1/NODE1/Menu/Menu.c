#include "../OLED/OLED.h"
#include "Menu.h"
#include "../Analog/Joystick.h"
#include "stdlib.h"
#include "string.h"
#include "../CAN/CAN.h"
#include "stdio.h"
#include "../Timer.h"
#include <string.h>
#include <stdio.h>
#include "../SRAM/SRAM.h"
#include "Menu_strings/Menu_strings.h"

extern volatile uint8_t ready;
extern volatile uint8_t start;
extern volatile uint8_t running; 
extern volatile uint8_t NODE2_READY_FLAG;

static uint16_t highscores[3] = {0, 0, 0};
static int8_t current_player = 1;


#define	Highscore_title_place	credit_name[0]
#define Highscore_name1_place	credit_name[1]
#define Highscore_name2_place	credit_name[2]
#define Highscore_name3_place	credit_name[3]

#define MAIN_MENU_STRING MENU_TITLE_STRINGS[0]
#define START_GAME_STRING MENU_TITLE_STRINGS[1]
#define SET_DIFFICULTY_STRING MENU_TITLE_STRINGS[2]
#define HIGH_SCORE_STRING MENU_TITLE_STRINGS[3]
#define CALIBRATE_STRING MENU_TITLE_STRINGS[4]
#define CREDITS_STRING MENU_TITLE_STRINGS[5]
#define NORMAL_STRING MENU_TITLE_STRINGS[6]
#define HARD_STRING MENU_TITLE_STRINGS[7]
#define PLAYER1_STRING MENU_TITLE_STRINGS[8]
#define PLAYER2_STRING MENU_TITLE_STRINGS[9]
#define PLAYER3_STRING MENU_TITLE_STRINGS[10]



volatile int current_line = 2;
volatile int current_menu_size;
volatile int score = 0;

menu_t *current_menu;

int cutter = 0;
int title_cutter = 0;
int display_line_offset = 0;

menu_t* Menu_create_menu(char* name, void (*func)(int8_t)) 
{
	menu_t* menu = malloc(sizeof(menu_t));
	menu->title = name;
	//strncpy(menu->title, name, SCREEN_WIDTH);
	
	menu->parent = NULL;
	menu->number_of_children = 0;
	menu->func = func;
	return menu;
}

int Menu_allocate_child(menu_t *parent, menu_t *child)
{
	if(parent->number_of_children < 8)
	{
		parent->child[parent->number_of_children++] = child;
		child->parent = parent;
		return 0;
	}
	else
	{
		return -1;
	}
}

menu_t *Menu_init(void) 
{
	menu_t* Main_menu		= Menu_create_menu(MAIN_MENU_STRING, NULL);
	menu_t* Start_game		= Menu_create_menu(START_GAME_STRING, NULL);	
	menu_t* difficulties	= Menu_create_menu(SET_DIFFICULTY_STRING, NULL);	
	menu_t* highscore		= Menu_create_menu(HIGH_SCORE_STRING, Menu_wite_high_score);
	menu_t* calibrate		= Menu_create_menu(CALIBRATE_STRING, NULL);
	menu_t* credits			= Menu_create_menu(CREDITS_STRING, Menu_write_credits);
	
	menu_t* easy			= Menu_create_menu(NORMAL_STRING, NULL);
	menu_t* hard			= Menu_create_menu(HARD_STRING, NULL);	
	menu_t* select_player1	= Menu_create_menu(PLAYER1_STRING, Menu_function_start_game);
	menu_t* select_player2	= Menu_create_menu(PLAYER2_STRING, Menu_function_start_game);
	menu_t* select_player3	= Menu_create_menu(PLAYER3_STRING, Menu_function_start_game);
		
	Menu_allocate_child(Main_menu, Start_game);
	Menu_allocate_child(Start_game, select_player1);
	Menu_allocate_child(Start_game, select_player2);
	Menu_allocate_child(Start_game, select_player3);
		
	Menu_allocate_child(Main_menu, difficulties);
	Menu_allocate_child(Main_menu, highscore);
	Menu_allocate_child(Main_menu, calibrate);
	Menu_allocate_child(Main_menu, credits);

	Menu_allocate_child(difficulties, easy);
	Menu_allocate_child(difficulties, hard);
	
	current_menu = Main_menu;
	current_menu_size = current_menu->number_of_children;
	Menu_print_menu(current_menu, 1);

	return current_menu;
}

void Menu_selection(void) 
{
	Direction_t direction;
	direction = Joystick_get_direction();
			
	static int menu_ptr = 1;
	
	switch(direction)
	{
		case UP:
			if(--menu_ptr < 1)
			{
				menu_ptr = current_menu->number_of_children;
			}
			Menu_print_menu(current_menu, menu_ptr);
		break;
		
		case DOWN:
			if(++menu_ptr > current_menu->number_of_children)
			{
				menu_ptr = 1;
			}
			else if(menu_ptr > 6)
			{
				menu_ptr = 1;
			}
			Menu_print_menu(current_menu, menu_ptr);
		break;
		
		case LEFT:
			if(current_menu->parent != NULL)
			{
				current_menu = current_menu->parent;
			}
		break;
		
		case RIGHT:
// 			if(current_menu->child[menu_ptr-1]->number_of_children != 0)
// 			{
// 				current_menu = current_menu->child[menu_ptr-1];
// 			}
		break;
		
		default:
				Menu_print_menu(current_menu, menu_ptr);	
		break;
	}
	
	if(Joystick_read_button())
	{
		if(current_menu->child[menu_ptr-1]->func != NULL)
		{
			current_menu->child[menu_ptr-1]->func(menu_ptr);
		}
		else if(current_menu->child[menu_ptr-1]->number_of_children != 0)
		{
			current_menu = current_menu->child[menu_ptr-1];
		}
	}
}


void Menu_function_start_game(int8_t choice)
{
	current_player = choice;

	OLED_clear();
		
	if(NODE2_READY_FLAG)
	{	
		Menu_pgm_write_string(MENU_START_STRINGS[0], 2, 10);
			
		Menu_pgm_write_string(MENU_START_STRINGS[1], 3, 10);
		_delay_ms(1000);
		
		Menu_pgm_write_string(MENU_START_STRINGS[2], 3, 10);
		_delay_ms(1000);
		
		Menu_pgm_write_string(MENU_START_STRINGS[3], 3, 10);
		_delay_ms(1000);
		
		Menu_pgm_write_string(MENU_START_STRINGS[4], 3, 10);
		
		running = 1;
		Send_msg(START);
		Timer1_start();
		OLED_clear();
	}
	else
	{
		Menu_pgm_write_string(MENU_START_STRINGS[5], 3, 0);
		
		Menu_pgm_write_string(MENU_START_STRINGS[6], 3, 0);
		_delay_ms(2500);
	}
}


void Menu_print_menu(menu_t *menu, int menu_ptr) 
{
	OLED_goto(0, 0);
	
	uint8_t menu_title[17];	

	strncpy_P(menu_title, menu->title, 16);

//	OLED_write_string(menu->title);
	OLED_clear();
	OLED_write_string(menu_title);

	Menu_pgm_write_string(MENU_GAME_STRINGS[0], 1, 0);
	
	for(int i = 0; i < 6; i++)
	{
		if(i >= menu->number_of_children)
		{
			OLED_clear_line(i+2);
		}
		else
		{
			OLED_goto(i + 2, 0);
			strncpy_P(menu_title, menu->child[i]->title, 16);
			if(i == menu_ptr - 1)
			{
				OLED_INV_write_string(menu_title);
			}
			else
			{
				OLED_write_string(menu_title);
			}
		}
	}
}

void Menu_write_score(uint16_t score)
{
	Menu_pgm_write_string(MENU_GAME_STRINGS[1], 3, 0);
	OLED_goto(4, 0);
	char num_string[6];
	sprintf(num_string, "%d", score);
		
	OLED_write_string(num_string);
}

void Menu_write_game_over(uint16_t score)
{
 	char num_string[6];
 	sprintf(num_string, "%d", score);	
 
 	OLED_clear();
 
 	Menu_pgm_write_string(MENU_GAME_STRINGS[2], 1, 0);
	 
	Menu_pgm_write_string(MENU_GAME_STRINGS[3], 2, 0);
 	
 	OLED_goto(3,0);
 	OLED_write_string(num_string);
	 	
	 _delay_ms(4000);

	Menu_edit_high_score(score, current_player); //Current player
}



void Menu_write_credits(int8_t choice)
{
	OLED_clear();
	for(int i = 0; i < 4; i++)
	{
		Menu_pgm_write_string(MENU_CREDITS_STRING[i], i+1, 0);
	}
	_delay_ms(5000);
}

void Menu_edit_high_score(uint16_t new_score, uint8_t player)
{
	if(new_score > highscores[player-1])
	{
		highscores[player-1] = new_score; 
	}

	Menu_wite_high_score(0);
}


void Menu_wite_high_score(int8_t choice)
{	
	uint8_t s[17];	
	OLED_clear();

	OLED_goto(0,0);
	strncpy_P(s, Highscore_title_place, 16);
	OLED_write_string(s);	
	
	OLED_goto(1,0);
	strncpy_P(s, Highscore_name1_place, 16);
	OLED_write_string(s);	

	OLED_goto(2,0);
	sprintf(s, "%d", highscores[0]);
	OLED_write_string(s);
		
	OLED_goto(3,0);	
	strncpy_P(s, Highscore_name2_place, 16);
	OLED_write_string(s);
	
	OLED_goto(4,0);
	sprintf(s, "%d", highscores[1]);
	OLED_write_string(s);	
	
	OLED_goto(5,0);
	strncpy_P(s, Highscore_name3_place, 16);
	OLED_write_string(s);
	
	OLED_goto(6,0);
	sprintf(s, "%d", highscores[2]);
	OLED_write_string(s);	

 	_delay_ms(4000);
}




		
		
void select_player_func(int8_t choice)
{
	current_player = choice;
}


void Menu_pgm_write_string(const unsigned char *p_s, uint8_t line, uint8_t col)
{
	char s[16];
	OLED_goto(line, col);
	strncpy_P(s, p_s, 16);
	OLED_write_string(s);		
}
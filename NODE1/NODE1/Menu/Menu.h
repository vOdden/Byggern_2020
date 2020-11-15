#ifndef MENU_H_
#define MENU_H_

#include "../utility.h"

#define SCREEN_WIDTH 16

typedef struct menu_struct menu_t;

typedef struct menu_struct
{
	char *title;//[SCREEN_WIDTH+1];
	struct menu_struct* parent;
	struct menu_struct* child[8];
	uint8_t number_of_children;
	void (*func)(int8_t);
} menu_t;

typedef enum
{
	NONE,
	PLAY_GAME,
	GAME_OVER,
	EASY,
	HARD
} menu_option_t;


//menu_option_t oled_menu_selection(void);
//void goto_menu(Direction_t dir);
//void oled_print_status(menu_option_t);


menu_t *Menu_init(void);
void Menu_print_menu(menu_t *menu, int menu_ptr);
void Menu_pgm_write_string(const unsigned char *p_s, uint8_t line, uint8_t col);
void Menu_edit_high_score(uint16_t new_score, uint8_t player);
void Menu_write_game_over(uint16_t score);
void Menu_selection(void);
void Menu_write_score(uint16_t score);


//Sub-menu functions
void Menu_wite_high_score(int8_t choice);
void Menu_wite_high_score(int8_t choice);
void Menu_function_start_game(int8_t choice);
void Menu_write_credits(int8_t);
void set_difficulty_func(int8_t choice);




#endif /* MENU_H_ */
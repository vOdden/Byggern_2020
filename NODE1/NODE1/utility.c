

#include "Utility.h"



void print_direction(Direction_t dir)
{
	switch(dir)
	{
		case NEUTRAL:
			printf("NEUTRAL");
		break;
		
		case UP:
			printf("UP");
		break;
		
		case DOWN:
			printf("DOWN");
		break;
		
		case LEFT:
			printf("LEFT");
		break;
		
		case RIGHT:
			printf("RIGHT");
		break;

		default:
			printf("DIR ERROR");
		break;		
	}
}
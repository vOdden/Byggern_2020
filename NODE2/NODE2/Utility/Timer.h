

#ifndef TIMER_H_
#define TIMER_H_

#define TIMER2 2
#define TIMER1 1
#define TIMER0 0

typedef struct
{
    volatile uint16_t*  source;
    uint16_t            prescaler;
} Timer ;

void TC0_init(uint32_t periode);
void TC1_init(uint32_t periode);
void TC2_init(uint32_t periode);
void Timer_start(int timer);
void Timer_stop(int timer);
//void TC0_Handler(void);
//void TC1_Handler(void);


#endif /* TIMER_H_ */



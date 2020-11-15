#ifndef memory_layout_H_
#define memory_layout_H_


#define ext_ram         ((volatile uint8_t*) 0x1800)
#define ext_ram_size    (0x800)
#define ext_adc         ((volatile uint8_t*) 0x1400)
#define ext_oled_cmd    ((volatile uint8_t*) 0x1000)
#define ext_oled_data   ((volatile uint8_t*) 0x1200)


#endif
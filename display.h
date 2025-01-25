#include "pico/stdlib.h"
#include "hardware/i2c.h"
void lcd_clear();
void lcd_init(i2c_inst_t *i, int address,uint sda_pin, uint scl_pin);
void lcd_print_to_line(int line,char *string);
void lcd_print_wrap(char *string);
void set_cursor(int line, int position);
void send_char(char c);
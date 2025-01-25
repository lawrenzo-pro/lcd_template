#include "hardware/i2c.h"
#include "display_bits.h"
#include "pico/stdlib.h"
#include "stdio.h"
#include "string.h"
int addr;
i2c_inst_t *inst;
void write_byte(uint8_t val){
    i2c_write_blocking(inst, addr,&val,1,false);
}
void toggle_enable(uint8_t val){
    #define DELAY_US 600
    sleep_us(DELAY_US);
    write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    write_byte(val & ~LCD_ENABLE_BIT);
}
void send_byte(uint8_t val,int mode){
    uint8_t high = mode | (val & 0xf0) | LCD_BACKLIGHT;
    uint8_t low = mode |((val<<0x04) & 0xf0) | LCD_BACKLIGHT;

    write_byte(high);
    toggle_enable(high);
    write_byte(low);
    toggle_enable(low);
}
void lcd_clear(){
    send_byte(LCD_CLEARDISPLAY,LCD_COMMAND);
}
void set_cursor(int line, int position){
    int val = (line == 0) ? 0x80 + position : 0xc0 + position;
    send_byte(val, LCD_COMMAND);
}
void send_char(char c){
    send_byte(c,LCD_CHARACTER);
}
void print_str(char *string){
     int len = strlen(string);
    for(int i = 0; i < len;i++){
        send_char(string[i]);
    }
}
void lcd_print_to_line(int line,char *string){
    set_cursor(line,0);
    print_str(string);
}
void lcd_print(int line,int position,char *string){
    set_cursor(line,position);
    print_str(string);
}
void lcd_print_wrap(char *string){
    int len = strlen(string);
    if(len > 32){
        print_str("Too Long!");
        return;
    }
    char line0[17];
    for(int i = 0; i <16; i++){
        line0[i] = string[i];
    }
    lcd_print_to_line(0,line0);
    if(len > 16){
        char line1[17];
        int j = 16;
        for(int i = 0; i <16; i++){
            line1[i] = string[j];
            j++;
        }
        lcd_print_to_line(1,line1);
    }
}
void lcd_init(i2c_inst_t *i, int address,uint sda_pin, uint scl_pin){
    inst = i;
    addr = address;
    i2c_init(i,100 * 1000);
    gpio_set_function(sda_pin,GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
    send_byte(0x03,LCD_COMMAND);
    send_byte(0x03,LCD_COMMAND);
    send_byte(0x03,LCD_COMMAND);
    send_byte(0x02, LCD_COMMAND);

    send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT , LCD_COMMAND);
    send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    send_byte(LCD_DISPLAYCONTROL | (LCD_DISPLAYON | LCD_BLINKON| LCD_CURSORON), LCD_COMMAND);
    

    lcd_clear();
}
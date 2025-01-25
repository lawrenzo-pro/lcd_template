#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "display.h"
#include "hardware/adc.h"
#define LCD_SDA 2
#define LCD_SCL 3

#define BTN 16
#define BTN2 17
#define POT 26

int main()
{
    stdio_init_all();
    adc_init();
    adc_gpio_init(POT);
    adc_select_input(0);
    lcd_init(i2c1,0x3f,LCD_SDA,LCD_SCL);
    gpio_init(BTN);
    gpio_set_dir(BTN, GPIO_IN);
    gpio_pull_down(BTN);
    int count = 0;
    char buff[16];
    while (true) {
        int result = adc_read();
        sprintf(buff,"value = %d", result);
        lcd_print_to_line(1,buff);
        sleep_ms(300);
        lcd_clear();
    }
}

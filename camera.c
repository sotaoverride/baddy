#include <stdio.h>
#include "hardware/irq.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/clocks.h"



int main() {
    stdio_init_all();


    gpio_init(10);
    gpio_init(7);
    gpio_init(6);
    gpio_init(5);
    gpio_init(4);
    gpio_init(11);
    gpio_init(2);
    gpio_init(9);
    clock_gpio_init(21, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS, 10);

    //use pin 24, 25 for I2c0 scl1, sda1
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(24, GPIO_FUNC_I2C);
    gpio_set_function(25, GPIO_FUNC_I2C);
    gpio_pull_up(24);
    gpio_pull_up(25);
    uint8_t txdata = 0xAA;
    uint8_t rxdata;
    i2c_write_blocking(i2c_default, 0x00, &txdata, 1, false);
    i2c_write_blocking(i2c_default, 0x01, &txdata, 1, false);
    i2c_write_blocking(i2c_default, 0x02, &txdata, 1, false);
    i2c_write_blocking(i2c_default, 0x03, &txdata, 1, false);
    i2c_read_blocking(i2c_default, 0x00, &rxdata, 1 , false);
    printf("value ad 0x00 0xAA=%u ?\n", rxdata);
     int i2c_read_blocking(i2c_inst_t *i2c, uint8_t addr, uint8_t *dst, size_t len, bool nostop);

    i2c_read_blocking(i2c_default, 0x01,&rxdata, 1, false);
    i2c_read_blocking(i2c_default, 0x02, &rxdata, 1, false);
    i2c_read_blocking(i2c_default, 0x03, &rxdata, 1, false);
    while(1) {}
}

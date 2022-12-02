#include <stdio.h>
#include "hardware/irq.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/time.h"




int main() {
    stdio_init_all();


    gpio_init(8);
    gpio_init(7);
    gpio_init(6);
    gpio_init(5);
    gpio_init(4);
    gpio_init(3);
    gpio_init(2);
    gpio_init(9);

    //use pin 24, 25 for I2c0 scl1, sda1
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(24, GPIO_FUNC_I2C);
    gpio_set_function(25, GPIO_FUNC_I2C);
    gpio_pull_up(24);
    gpio_pull_up(25);
    while(1) {}
}
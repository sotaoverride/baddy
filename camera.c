#include <stdio.h>
#include "hardware/irq.h"
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

    while(1) {}
}

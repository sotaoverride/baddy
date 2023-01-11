#include <stdio.h>
#include "hardware/irq.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/clocks.h"


// I2C reserves some addresses for special purposes. We exclude these from the scan.
// These are any addresses of the form 000 0xxx or 111 1xxx
bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

int main() {
    stdio_init_all();
    printf("camera prg starting************");
    //clocks_init();
    //clock_gpio_init(21, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS, 10);

    //clock_gpio_init(21, clk_usb, 2);
    clock_gpio_init(21, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS, 10);
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);

    //data lines
    //
    gpio_init(0);
    gpio_set_dir(0, GPIO_IN);
    gpio_init(1);
    gpio_set_dir(1, GPIO_IN);
    gpio_init(2);
    gpio_set_dir(2, GPIO_IN);
    gpio_init(3);
    gpio_set_dir(3, GPIO_IN);
    gpio_init(8);
    gpio_set_dir(8, GPIO_IN);
    gpio_init(9);
    gpio_set_dir(9, GPIO_IN);
    gpio_init(10);
    gpio_set_dir(10, GPIO_IN);
    gpio_init(11);
    gpio_set_dir(11, GPIO_IN);

    //test read
    //
    const uint8_t regaddr[1] = { 0x1c };
    i2c_write_blocking(i2c_default, 0x21, &regaddr, 1, true);
    uint8_t val[1];
    i2c_read_blocking(i2c_default, 0x21, &val, 1, false);

    printf("\ni2c bus scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");

    while(1){
    printf("\ni2c bus scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
	    
    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        // perform a 1-byte dummy read from the probe address. if a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. if the address byte is ignored, the function returns
        // -1.

        // skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if (reserved_addr(addr))
            ret = -1;
        else
            ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("Done. \n");
    printf("Vendor ID: %02x", val[0]);
    }

}


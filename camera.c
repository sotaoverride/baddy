#include <stdio.h>
#include <string.h>
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
#define BLANKING_COUNT 17
#define PIXELS_IN_LINE 176 
#define BYTES_PER_PIXEL_Y 1
#define BYTES_PER_PIXEL 2
#define TOTAL_LINE_DATA (PIXELS_IN_LINE * BYTES_PER_PIXEL_Y)
#define LINES_IN_FRAME 30 // *2 
#define VBLANK_LINES (3+17)
//double check GPIO number for vsync/pclk
uint32_t byte_count =0;
static uint8_t href = 19;
static uint8_t vsync = 12;
static uint32_t pclk = 22;//14;
static uint32_t line=0;
static uint32_t frame=0;
static uint32_t pixel_byte_index =0;
uint8_t line_data[TOTAL_LINE_DATA];
uint8_t frame_data[LINES_IN_FRAME][TOTAL_LINE_DATA];
volatile int foo;
uint32_t pixel_count = 0;
uint8_t pattern[] = "-+*#";
uint32_t read_data_pins(){
	uint32_t all = gpio_get_all();
	uint32_t d0123 = all & 0b1111;   /* I would have written 0xF instead of 0b  but its' teh same
					    thing */
	uint32_t d4567 = all & 0xf00;    /* or 0xb111100000000 */
	return  ( (d4567 >> 4) | d0123);
	//	return gpio_get(0);

}

/*read d0 - d7 on href rising edge after blanking*/
void get_data(){
	//	pixel_byte_index++;
		if ( pixel_byte_index < (TOTAL_LINE_DATA) ) {
			line_data[pixel_byte_index]=read_data_pins();
			pixel_byte_index++;
		}



}

void href_callback() {  
	line++; 
}
void vsync_callback() {
	//printf("pixel count  %d  \n", pixel_count);
	//pixel_count = 0;
	if ((frame % 30) == 0) 
	{ 
	
	} 
	frame++;
       	pixel_byte_index=0;	

} 
void irq_dispatch(uint gpio, uint32_t mask) {
	if (gpio == href) href_callback();
	else if (gpio == vsync) vsync_callback();
}


void init(){
	uint8_t val = 0x08;
	uint8_t val2 = 0x0a;
	uint8_t buf[2];
	buf[0] = 0x0c;
	buf[1] = val;
	i2c_write_blocking(i2c_default, 0x21, buf, 2, false);
	buf[0]=0x12;
	buf[1]=val2;
	i2c_write_blocking(i2c_default, 0x21, buf, 2, false);

//	i2c_write_blocking(i2c_default, 0x3e, &val, 1, true);

	gpio_set_irq_enabled_with_callback(href, GPIO_IRQ_EDGE_RISE, true, &irq_dispatch);
	gpio_set_irq_enabled_with_callback(vsync, GPIO_IRQ_EDGE_FALL, true, &irq_dispatch);

}
uint32_t get_byte(){
 while(!gpio_get(pclk)){} while(gpio_get(pclk)){}
 uint32_t val = read_data_pins();
 
// printf("pixel %d\n", val);
 return val;
}
uint8_t get_byte_y(){
 while(!gpio_get(pclk)){} while(gpio_get(pclk)){}
 while(!gpio_get(pclk)){} while(gpio_get(pclk)){}
 uint8_t val = read_data_pins();
 //pixel_count++;
// printf("pixel %d\n", val);
 //uint32_t all = gpio_get_all();
 //uint32_t d0123 = all & 0b1111;   
 /* I would have written 0xF instead of 0b  but its' teh same
                                            thing */
 //uint32_t d4567 = all & 0xf00;    /* or 0xb111100000000 */
 //pixel_count++;
 //return  ( (d4567 >> 4) | d0123);
 return val;
}


int main() {

	uint8_t regaddr[1] =  {0x1c};
    	const uint8_t* ptr= &regaddr[0];
    	uint8_t val[4] = {0,0,0,0};
	stdio_init_all();
	sleep_ms(3000);
	printf("camera prg starting\n");
	clocks_init();
	gpio_init(21);
	gpio_set_dir(21, GPIO_OUT);
	//CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS
	clock_gpio_init(21, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS, 10);

	//clock_gpio_init(21, clk_usb, 2);
	//clock_gpio_init(21, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS, 9);
	//clock_gpio_init(21, CLOCKS_CLK_GPOUT3_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, 5);
	sleep_ms(3000);
	i2c_init(i2c_default, 10 * 1000);
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
	gpio_init(pclk);
	gpio_set_dir(pclk, GPIO_IN);
	gpio_init(14);
        gpio_set_dir(14, GPIO_OUT);

	init();
	memset(line_data, 99, sizeof(line_data));
	i2c_write_blocking(i2c_default, 0x21, ptr, 1, false);
    	i2c_read_blocking(i2c_default, 0x21, &val[0], 1, false);
	printf("=============>%02x \n", val[0]);
	i2c_write_blocking(i2c_default, 0x21, ptr, 1, false);
        i2c_read_blocking(i2c_default, 0x21, &val[0], 1, false);
        printf("=============>%02x \n", val[0]);

	while(!gpio_get(vsync)){} while(gpio_get(vsync)){}

	for(int j =0; j < LINES_IN_FRAME; j++){


		while(!gpio_get(href)){} 

		for(int i=0; i< TOTAL_LINE_DATA; i++){

				frame_data[j][i]=get_byte_y();
		}
	}
	printf("pixel count  %d  \n", pixel_count);
        pixel_count = 0;

	for(int j =0; j < LINES_IN_FRAME; j++){


		for(int i=0; i <TOTAL_LINE_DATA; i++){
			 printf("%c", pattern[frame_data[j][i] >> 6] );
		}
		printf("\n");
	}


}

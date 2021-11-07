#include "main.h"
#include "uart.h"
#include "rcc.h"
#include "gpio.h"
#include <stdio.h>

int main(void)
{
    rcc_HSE_config();
    rcc_SysTick_config(72000);
    uart_UART1_GPIO_config();
    uart_UART1_config();

    gpio_LED_config();
    gpio_LED_writeRed(true);
    gpio_LED_writeGreen(true);


    printf("Program is starting... \n");
    while(1)
    {
      printf("Hello World\n");
      gpio_LED_toggleGreen();
      rcc_msDelay(1000);
    }



}

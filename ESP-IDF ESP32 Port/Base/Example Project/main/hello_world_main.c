#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"

#include"xpt2046_driver.h"

void app_main(void)
{
    //spi init
    //spi_init();

    //xpt2046 init all
    xpt2046_init();

    //simple test
    uint16_t x=0;
    uint16_t y=0;
    uint8_t state=0;
    while(1)
    {
        //read touch
        xpt2046_read(&x,&y,&state);
        if(state)
        {
            printf("x=%d,y=%d",x,y);
        }
    }
}

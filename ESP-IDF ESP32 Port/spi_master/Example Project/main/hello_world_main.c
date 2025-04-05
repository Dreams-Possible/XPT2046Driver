#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"

#include"spi_driver.h"
#include"xpt2046_driver.h"

void app_main(void)
{
    //spi init
    spi_init();

    //xpt2046 init all
    xpt2046_init();

    //simple test
    uint16_t x=0;
    uint16_t y=0;
    while(1)
    {
        //read touch
        if(xpt2046_read(&x,&y))
        {
            printf("x=%d,y=%d\n",x,y);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

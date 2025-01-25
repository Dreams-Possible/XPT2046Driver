#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"

#include"iic_driver.h"
#include"bmp280_driver.h"

void app_main(void)
{
    //iic init
    iic_init();

    //bmp280 init
    bmp280_init();

    float pressure=0;
    float temperature=0;
    while (1)
    {
        //bmp280 read
        bmp280_read(&pressure,&temperature);
        printf("pressure=%fpa\n",pressure);
        printf("temperature=%f°C\n",temperature);
        vTaskDelay(pdMS_TO_TICKS(400));
    }
}

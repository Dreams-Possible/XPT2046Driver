//enable bmp280 driver
#if 1
#ifndef BMP280_DRIVER_H
#define BMP280_DRIVER_H

//include
#include<stdio.h>
#include<stdint.h>
#include"driver/i2c_master.h"
#include"freertos/FreeRTOS.h"

//bmp280 init
void bmp280_init();
//bmp280 id
uint8_t bmp280_id();
//bmp280 read
void bmp280_read(float*pressure,float*temperature);

#endif//#ifndef BMP280_DRIVER_H
#endif//#if 1

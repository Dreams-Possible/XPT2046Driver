//enable xpt2046 driver
#if 1
#ifndef XPT2046_DRIVER_H
#define XPT2046_DRIVER_H

//include
#include<stdio.h>
#include<stdint.h>
#include"driver/spi_master.h"

//xpt2046 init
void xpt2046_init();
//xpt2046 read
uint8_t xpt2046_read(uint16_t*x,uint16_t*y);

#endif//#ifndef XPT2046_DRIVER_H
#endif//#if 1

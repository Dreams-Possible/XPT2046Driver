//enable xpt2046 driver
#if 1
#ifndef XPT2046_DRIVER_H
#define XPT2046_DRIVER_H

//include
#include<stdio.h>
#include<stdint.h>

//xpt2046 init all
void xpt2046_init();
//read touch
void xpt2046_read(uint16_t*x,uint16_t*y,uint8_t*state);

#endif//#ifndef XPT2046_DRIVER_H
#endif//#if 1

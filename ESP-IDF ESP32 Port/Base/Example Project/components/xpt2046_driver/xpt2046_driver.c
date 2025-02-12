#include"xpt2046_driver.h"
//enable xpt2046 driver
#ifdef XPT2046_DRIVER_H

//config
#define HOR 240//horizontal pixel
#define VER 320//vertical pixel
#define HOR_OFF 250//convert to position:x edge error
#define VER_OFF 150//convert to position:y edge error
#define HOR_FAC 1600//convert to position:x conversion coefficient
#define VER_FAC 1800//convert to position:y conversion coefficient

//example config for specific oem touch
// //config
// #define HOR 240//horizontal pixel
// #define VER 320//vertical pixel
// #define HOR_OFF 250//convert to position:x edge error
// #define VER_OFF 150//convert to position:y edge error
// #define HOR_FAC 1600//convert to position:x conversion coefficient
// #define VER_FAC 1800//convert to position:y conversion coefficient

//define
#define X 0Xd0
#define Y 0X90
#define Z1 0Xb0
#define Z2 0Xc0

//send a bit
static void send(uint8_t send);
//receive two bits
static void receive(uint16_t*receive);
//swap 3 bits
static void swap(uint8_t cmd,uint16_t*data);
//init spi
static void init_spi();
//check if any touch
static uint8_t check();
//read raw data
static void raw(uint16_t*x,uint16_t*y);
//convert to position
static void convert(uint16_t*x,uint16_t*y);
//xpt2046 init
void xpt2046_init();
//xpt2046 read
uint8_t xpt2046_read(uint16_t*x,uint16_t*y);

//send a bit
static void send(uint8_t send)
{
    return;
}

//receive two bits
static void receive(uint16_t*receive)
{
    return;
}

//swap 3 bits
static void swap(uint8_t cmd,uint16_t*data)
{
    send(cmd);
    receive(data);
    return;
}

//init spi
static void init_spi()
{
    return;
}

//xpt2046 init
void xpt2046_init()
{
    init_spi();
    return;
}

//check if any touch
static uint8_t check()
{
    uint16_t data=0;
    swap(Z1,&data);
    uint16_t z1=data>>4;
    data=0;
    swap(Z2,&data);
    uint16_t z2=data>>4;
    if(z1<=8||z2>=2040)
    {
        return 0;
    }else
    {
        return 1;
    }
}

//read raw data
static void raw(uint16_t*x,uint16_t*y)
{
    uint16_t data=0;
    swap(X,&data);
    *x=data>>4;
    data=0;
    swap(Y,&data);
    *y=data>>4;
    return;
}

//convert to position
static void convert(uint16_t*x,uint16_t*y)
{
    float xf=*x;
    float yf=*y;
    xf=(xf-HOR_OFF)/(HOR_FAC)*HOR;
    yf=(yf-VER_OFF)/(VER_FAC)*VER;
    if(xf<0)
    {
        xf=0;
    }
    if(yf<0)
    {
        yf=0;
    }
    if(xf>240)
    {
        xf=240;
    }
    if(yf>320)
    {
        yf=320;
    }
    *x=(int)xf;
    *y=(int)yf;
    return;
}

//xpt2046 read
uint8_t xpt2046_read(uint16_t*x,uint16_t*y)
{
    uint16_t xi=0;
    uint16_t yi=0;
    if(check()==0)
    {
        return 0;
    }else
    {
        raw(&xi,&yi);
        convert(&xi,&yi);
        *x=xi;
        *y=yi;
        return 1;
    }
}

#endif//#ifdef XPT2046_DRIVER_H

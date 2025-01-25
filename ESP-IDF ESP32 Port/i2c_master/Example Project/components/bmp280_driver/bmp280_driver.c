#include"bmp280_driver.h"
//enable bmp280 driver
#ifdef BMP280_DRIVER_H

//config
#define ACCU 1//set 1 as high-precision and 0 for normal precision
#define IIC I2C_NUM_1
#define RATE 100*1000

//define
#define ADDR 0x77//or 0x76 which decide by oem factory
#define ID 0xd0
#define RESET 0xe0
#define CTRL_MEAS 0xf4
#define CONFIG 0xf5
#define PRESS 0xf7
#define TEMP 0xfa
#define COMPENSATION 0x88

//static data
typedef struct static_data_t
{
    i2c_master_dev_handle_t iic;
    uint16_t t1;
    int16_t t2;
    int16_t t3;
    uint16_t p1;
    int16_t p2;
    int16_t p3;
    int16_t p4;
    int16_t p5;
    int16_t p6;
    int16_t p7;
    int16_t p8;
    int16_t p9;
}static_data_t;
static static_data_t static_data={0};

//delay ms
static void delay(uint32_t ms);
//write data
static void write(uint8_t addr,uint8_t data);
//read data
static void read(uint8_t addr,uint8_t*data,uint8_t len);
//init iic
static void init_iic();
//init software
static void init_soft();
//read raw
static void raw(int32_t*pressure,int32_t*temperature);
//process raw
static void process(int32_t*pr,int32_t*tr,float*pp,float*tp);
//bmp280 init
void bmp280_init();
//bmp280 id
uint8_t bmp280_id();
//bmp280 read
void bmp280_read(float*pressure,float*temperature);

//delay ms
static void delay(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
    return;
}

//write data
static void write(uint8_t addr,uint8_t data)
{
    uint8_t cmd[2]={0};
    cmd[0]=addr;
    cmd[1]=data;
    i2c_master_transmit(static_data.iic,cmd,2,-1);    
    return;
}

//read data
static void read(uint8_t addr,uint8_t*data,uint8_t len)
{
    i2c_master_transmit_receive(static_data.iic,&addr,1,data,len,-1);
    return;
}

//init iic
static void init_iic()
{
    //add iic device
    i2c_device_config_t config={0};
    config.dev_addr_length=I2C_ADDR_BIT_LEN_7;
    config.device_address=ADDR;
    config.scl_speed_hz=RATE;
    i2c_master_bus_handle_t handle;
    i2c_master_get_bus_handle(IIC,&handle);
    i2c_master_bus_add_device(handle,&config,&static_data.iic);
    return;
}

//init software
static void init_soft()
{
    //soft reset
    write(RESET,0Xb6);
    delay(400);
    //sampling config
    write(CTRL_MEAS,0x6f);//011: temperature x4, 011: pressure x4, 11: normal mode
    //other config
    write(CONFIG,0x6c);//011: refresh time 250ms, 011: filter x4, 00: disable spi mode
    //read calibration data
    uint8_t data[24]={0};
    read(COMPENSATION,data,24);
    static_data.t1=(uint16_t)((data[1]<<8)|data[0]);
    static_data.t2=(int16_t)((data[3]<<8)|data[2]);
    static_data.t3=(int16_t)((data[5]<<8)|data[4]);
    static_data.p1=(uint16_t)((data[7]<<8)|data[6]);
    static_data.p2=(int16_t)((data[9]<<8)|data[8]);
    static_data.p3=(int16_t)((data[11]<<8)|data[10]);
    static_data.p4=(int16_t)((data[13]<<8)|data[12]);
    static_data.p5=(int16_t)((data[13]<<8)|data[14]);
    static_data.p6=(int16_t)((data[17]<<8)|data[16]);
    static_data.p7=(int16_t)((data[19]<<8)|data[18]);
    static_data.p8=(int16_t)((data[21]<<8)|data[20]);
    static_data.p9=(int16_t)((data[23]<<8)|data[22]);
}

//bmp280 init
void bmp280_init()
{
    init_iic();
    init_soft();
    return;
}

//bmp280 id
uint8_t bmp280_id()
{
    uint8_t id=0;
    read(ID,&id,1);
    return id;
}

//read raw
static void raw(int32_t*pressure,int32_t*temperature)
{
    uint8_t data[3]={0};
    read(PRESS,data,3);
    *pressure=(int32_t)((data[0]<<12)|(data[1]<<4)|(data[2]>>4));
    data[0]=0;
    data[1]=0;
    data[2]=0;
    read(TEMP,data,3);
    *temperature=(int32_t)((data[0]<<12)|(data[1]<<4)|(data[2]>>4));
    return;
}

//process raw
static void process(int32_t*pr,int32_t*tr,float*pp,float*tp)
{
    if(ACCU)
    {
        int32_t t_fine=0;
        double var1=0;
        double var2=0;
        double t=0;
        var1=(((double)*tr)/16384.0-((double)static_data.t1)/1024.0)*((double)static_data.t2);
        var2=((((double)*tr)/131072.0-((double)static_data.t1)/8192.0)*(((double)*tr)/131072.0-((double)static_data.t1)/8192.0))*((double)static_data.t3);
        t_fine=(int32_t)(var1+var2);
        t=(var1+var2)/5120.0;
        *tp=t;
        var1=0;
        var2=0;
        double p=0;
        var1=((double)t_fine/2.0)-64000.0;
        var2=var1*var1*((double)static_data.p6)/32768.0;
        var2=var2+var1*((double)static_data.p5)*2.0;
        var2=(var2/4.0)+(((double)static_data.p4)*65536.0);
        var1=(((double)static_data.p3)*var1*var1/524288.0+((double)static_data.p2)*var1)/524288.0;
        var1=(1.0+var1/32768.0)*((double)static_data.p1);
        if(var1==0.0)
        {
            return;
        }
        p=1048576.0-(double)*pr;
        p=(p-(var2/4096.0))*6250.0/var1;
        var1=((double)static_data.p9)*p*p/2147483648.0;
        var2=p*((double)static_data.p8)/32768.0;
        *pp=p+(var1+var2+((double)static_data.p7))/16.0;
        return;
    }else
    {
        int32_t t_fine=0;
        int64_t var1=0;
        int64_t var2=0;
        int64_t t=0;
        var1=((((*tr>>3)-((int32_t)static_data.t1<<1)))*((int32_t)static_data.t2))>>11;
        var2=(((((*tr>>4)-((int32_t)static_data.t1))*((*tr>>4)-((int32_t)static_data.t1)))>>12)*((int32_t)static_data.t3))>>14;
        t_fine=var1+var2;
        t=(t_fine*5+128)>>8;
        *tp=(float)t/100;
        var1=0;
        var2=0;
        int64_t p;
        var1=((int64_t)t_fine)-128000;
        var2=var1*var1*(int64_t)static_data.p6;
        var2=var2+((var1*(int64_t)static_data.p5)<<17);
        var2=var2+(((int64_t)static_data.p4)<<35);
        var1=((var1*var1*(int64_t)static_data.p3)>>8)+((var1*(int64_t)static_data.p2)<<12);
        var1=(((((int64_t)1)<<47)+var1)*(int64_t)static_data.p1)>>33;
        if(var1==0)
        {
            return ; 
        }
        else
        {
            p=1048576-*pr;
            p=(((p<<31)-var2)*3125)/var1;
            var1=(((int64_t)static_data.p9)*(p>>13)*(p>>13))>>25;
            var2=(((int64_t)static_data.p8)*p)>>19;
            p=((p+var1+var2)>>8)+(((int64_t)static_data.p7)<<4);
            *pp=(float)p/256;
        }
        return;
    }
}

//bmp280 read
void bmp280_read(float*pressure,float*temperature)
{
    int32_t pr=0;
    int32_t tr=0;
    float pp=0;
    float tp=0;
    raw(&pr,&tr);
    process(&pr,&tr,&pp,&tp);
    *pressure=pp;
    *temperature=tp;
    return;
}

#endif//#ifdef BMP280_DRIVER_H

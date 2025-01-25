#include"iic_driver.h"
//enable iic driver
#ifdef IIC_DRIVER_H

//config
#define COM I2C_NUM_1
#define SCL 2
#define SDA 3

//iic init
void iic_init();

//iic init
void iic_init()
{
    i2c_master_bus_config_t config={0};
    config.clk_source=I2C_CLK_SRC_DEFAULT;
    config.i2c_port=COM;
    config.scl_io_num=SCL;
    config.sda_io_num=SDA;
    config.glitch_ignore_cnt=7;
    config.flags.enable_internal_pullup=true;
    i2c_master_bus_handle_t handle=NULL;
    i2c_new_master_bus(&config,&handle);
}

#endif//#ifdef IIC_DRIVER_H

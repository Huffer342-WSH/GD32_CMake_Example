#ifndef _IIC_H_
#define _IIC_H_
#include "gd32f3x0.h"
#include "radar_port_cfg.h"

#define I2C_SLAVE_ADDRESS7 RADAR_IIC_SLAVE_ADDRESS

#define I2CX RADAR_IIC_CHx
#define I2CX_SCL_PORT RADAR_IIC_SCL_PORT
#define I2CX_SDA_PORT RADAR_IIC_SDA_PORT
#define I2CX_SCL_PIN RADAR_IIC_SCL_PIN
#define I2CX_SDS_PIN RADAR_IIC_SDS_PIN

void IIC_Init();
void i2c_bus_reset(void);
#endif
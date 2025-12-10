#include "AS5600.h"

//发送单字节时序
void AS5600_Write_Reg(unsigned char reg, unsigned char value)
{
	HAL_I2C_Mem_Write(&hi2c2, Slave_Addr, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 50);
}


//发送多字节时序
void AS5600_Write_Regs(unsigned char reg, unsigned char *value, unsigned char len)
{
	HAL_I2C_Mem_Write(&hi2c2, Slave_Addr, reg, I2C_MEMADD_SIZE_8BIT, value, len, 50);
}


//IIC读多字节
void AS5600_Read_Reg(unsigned char reg, unsigned char* buf, unsigned short len)
{
	HAL_I2C_Mem_Read(&hi2c2, Slave_Addr, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 50);
}


/* 获取角度值 */
float Get_Current_Angle(void)
{
	float angle_d;
	int16_t in_angle;
	uint8_t temp[2]={0};
	AS5600_Read_Reg( Angle_Hight_Register_Addr, temp, 2);
	in_angle = ((int16_t)temp[0] <<8) | (temp[1]);
	angle_d = (float)in_angle * 360 / 4096;
	return 360-angle_d;		/* 这里用360度减去读取的角度是确保电机转动的方向与实际角度的大小的变化一致 */
	// return angle_d;
}


/* 0-360度归一化函数 */
float normalizeAngle(float angle) {
    // 将角度转换到0-360度的范围内
    while (angle < 0) {
        angle += 360;
    }
    while (angle > 360) {
        angle -= 360;
    }
    return angle;
}


/* 获取转换后的电角度(7极对数) */
float Get_CalibAngle(void)
{
	int16_t Origin_Angle = ((int16_t)normalizeAngle(Get_Current_Angle()-Zero_Angle)*POLE_PAIR_NUM)%360;

	return (float)Origin_Angle * (PI/180.0f);	/* 返回电角度弧度值 */
}	





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



//得到弧度制的角度，范围在0-6.28
float GetAngle_Without_Track(void)
{   
	float angle_d;
	int16_t in_angle;
	uint8_t temp[2]={0};
	AS5600_Read_Reg( Angle_Hight_Register_Addr, temp, 2);
	in_angle = ((int16_t)temp[0] <<8) | (temp[1]);
	angle_d = 6.28-(float)in_angle * (2.0f*PI) / 4096;		//保持给定方向对齐
	return angle_d;
	//angle_d为弧度制，范围在0-6.28
}

/* 获取转过的弧度 */
float GetAngle(void)
{
    float val = GetAngle_Without_Track();
    float d_angle = val - angle_prev;
    //计算旋转的总圈数
    //通过判断角度变化是否大于80%的一圈(0.8f*6.28318530718f)来判断是否发生了溢出，如果发生了，则将full_rotations增加1（如果d_angle小于0）或减少1（如果d_angle大于0）。
    if(fabs(d_angle) > (0.8f*2.0f*PI) ) full_rotations += ( d_angle > 0 ) ? -1 : 1; 
    angle_prev = val;
    return -((float)full_rotations * 6.28318530718f + angle_prev);	/* 当方向取反时 */
    
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

int16_t Get_int_Angle(void)
{
	float angle_d;
	int16_t in_angle;
	uint8_t temp[2]={0};
	AS5600_Read_Reg( Angle_Hight_Register_Addr, temp, 2);
	in_angle = ((int16_t)temp[0] <<8) | (temp[1]);
	angle_d = (int16_t)in_angle * 360 / 4096;
	return 360-angle_d;		/* 这里用360度减去读取的角度是确保电机转动的方向与
							实际角度的大小的变化一致 */
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

// 处理角度环绕的函数
float HandleAngleWrap(float previous, float current) {
    float diff = current - previous;
    
    // 如果角度变化超过180度，认为发生了环绕
    if (diff > PI) {
        diff -= 2.0f*PI;  // 顺时针环绕
    } else if (diff < -PI) {
        diff += 2.0f*PI;  // 逆时针环绕
    }
    
    return diff;
}

/* 获取转换后的电角度 */
float Get_CalibAngle(void)
{
	int16_t Origin_Angle = ((int16_t)normalizeAngle(Get_Current_Angle()-Zero_Angle)*POLE_PAIR_NUM)%360;

	return (float)Origin_Angle * (PI/180.0f);	/* 返回电角度弧度值 */
}	

/* 获取当前电机的转速(r/s) */
float Get_MotorVelocity(Speed_Calculator* CalcStruct)
{	
	CalcStruct->previous_angle_rad = CalcStruct->current_angle_rad;
	CalcStruct->current_angle_rad = GetAngle_Without_Track();		//Get now Angle(rad)
	CalcStruct->angle_diff_rad = HandleAngleWrap(CalcStruct->previous_angle_rad,
									CalcStruct->current_angle_rad);

	CalcStruct->Delta_cnt_diff = TIM2->CNT;
	TIM2->CNT = 0;		//wait for next GetAngle

	CalcStruct->speed_rad_per_s = (CalcStruct->angle_diff_rad * MICROSECONDS_PER_SECOND)
									/CalcStruct->Delta_cnt_diff;

	return CalcStruct->speed_rad_per_s;
}



#ifndef __GM6020_H
#define __GM6020_H


#include "stm32f4xx_hal.h"


/* 电机返回机械转矩在0-8192之间 */
#define GM6020_PPR				8192

#define MAX_VOLT				(30000)
#define MIN_VOLT				(-30000)


/* 位置控制PID参数 */
typedef struct {

	/* 位置式PID参数 */
	float kp;
	float ki;
	float kd;
	
	/* 增量式PID参数 */
	float a1;
	float a2;
	float a3;
	
	/* 变速积分参数 */
	float A;
	float B;

	/* 不完全微分参数 */
	float alpha;

	/* 误差 */
	float cur_err[4];
	float sum_err[4];
	float lst_err[4];
	float prv_err[4];
	float last_uKd[4];
	
	/* 输出限幅 */
	int16_t saturation;
	int8_t is_sat[4];
	
} PID_TypeDef;

typedef struct {
	
	/* CAN */
	CAN_HandleTypeDef* motor_can;
	
	/* 电机的id */
	uint16_t motor_id_group;
	
	/* 电机的机械转角和转矩 */
	int16_t angle[4];
	int16_t velocity[4];
	int16_t torque[4];
	uint8_t temp;

	float vel[4]; //PRS
	
	/* 电机的PID位置控制器 */
	PID_TypeDef pid;
	
	/* 电机的设置位置 */
	int16_t loc_set[4];
	float vel_set[4];
	
	/* 电机的转向 */
	int16_t dir[4];
	
	/* 接受信号缓存 */
	uint8_t active_channel[4];
	uint32_t dataL_buf[4];
	uint32_t dataH_buf[4];
	
	/* 电机的电压输入 */
	int32_t volt[4];
	
} GM6020_TypeDef;


/* 用户接口 */

/* 打开一个电机组 */
GM6020_TypeDef GM6020_Open(CAN_HandleTypeDef* hcan, uint16_t id_group);

/* 放在HAL_CAN_RxFifo0MsgPendingCallback中 */
void GM6020_RxUpdate(GM6020_TypeDef* M, CAN_HandleTypeDef* hcan);

/* 主任务 */
void GM6020_CalcPid(GM6020_TypeDef* M);

/* 设置电机转向 */
void GM6020_SetDir(GM6020_TypeDef* M, int16_t dir1, int16_t dir2, int16_t dir3, int16_t dir4);

/* 设置PID参数和变速积分参数 */
void GM6020_CtrlParams(GM6020_TypeDef* M, float kp, float ki, float kd, int16_t output_saturation);

/* 设置PID控制器的变速积分参数 */
void GM6020_ExCtrlParams(GM6020_TypeDef* M, float A, float B, float alpha);

/* 使用CAN向电机发送命令 */
void GM6020_SendCmd(GM6020_TypeDef* M, int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);

/* 设置电机转速 */
void GM6020_CmdVel(GM6020_TypeDef* M, float vel_rps1, float vel_rps2, float vel_rps3, float vel_rps4);

#endif

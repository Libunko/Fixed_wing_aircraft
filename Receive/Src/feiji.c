#include "feiji.h"

extern uint8_t receive_value[6];
extern uint32_t trans_value[3];

void PWM_Init(void)
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	//舵机归中
	TIM3->CCR3 = 1500;
	TIM3->CCR4 = 1500;
}
void cheak_nrf(void)
{
  while(NRF24L01_Check())
	{
    printf("硬件查寻不到NRF24L01无线模块\r\n");
		OLED_ShowString(0, 32, (uint8_t *)"nrf not found", 16);
		OLED_Refresh_Gram();
		HAL_Delay(1000);
	}
	OLED_Clear();
  printf("NRF24L01无线模块硬件连接正常\r\n");
  NRF24L01_RX_Mode();
  printf("进入数据发送模式");
}
void steering_engine(void)		//控制舵机
{
	if(trans_value[1]/100<=24 && trans_value[1]/100>=17 && trans_value[2]/100<=21 && trans_value[2]/100>=17)
	{
		//舵机归中
		TIM3->CCR3 = 1500;
		TIM3->CCR4 = 1500;
	}
	else
	{
		if(trans_value[1]/100<=25 && trans_value[1]/100>=16)
		{
			TIM3->CCR3 = trans_value[2]/100*25+1000;
			TIM3->CCR4 = 3000-(trans_value[2]/100*25+1000);		//上下转
		}
		else
		{
			TIM3->CCR3 = 3000 - (trans_value[1]/100*25+1000);
			TIM3->CCR4 = 3000 - (trans_value[1]/100*25+1000);		//左右转
		}
	}
}
void data_deal(void)
{
	if(NRF24L01_RxPacket(receive_value)==0)
	{
		trans_value[0] = 4095-((receive_value[1]<<8)+receive_value[0]);		//INT0数据
		trans_value[1] = (receive_value[3]<<8)+receive_value[2];					//INT1数据
		trans_value[2] = (receive_value[5]<<8)+receive_value[4];					//INT2数据
		//处理接收到的数据
		TIM3->CCR1 = trans_value[0]/100 * 25 + 1000;		//电机转速
		steering_engine();		//控制两个舵机
//		printf("NRF24L01无线模块数据接收成功:%d  %d  %d\r\n",trans_value[0],trans_value[1],trans_value[2]);
		OLED_ShowNum(0, 0, trans_value[0], 4, 16);
		OLED_ShowNum(0, 16, trans_value[1], 4, 16);
		OLED_ShowNum(0, 32, trans_value[2], 4, 16);
		OLED_Refresh_Gram();
	}
}

#ifndef __FEIJI_H
#define __FEIJI_H

#include "stm32f1xx_hal.h"
#include "tim.h"
#include "nrf24l01.h"
#include "oled.h"

void PWM_Init(void);
void cheak_nrf(void);
void data_deal(void);

#endif

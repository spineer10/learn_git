#ifndef __relay_H
#define __relay_H
#include "sys.h"
#include "delay.h"
void relay_init(void);
void relay1_init(void);
void relay_on(void);
void relay_off(void);
void relay1_on(void);
void relay1_off(void);
void init_controller(void);
void set_motorpwm(uint16_t);
void TIM1_PWM_Init(uint16_t,uint16_t);
void ADC_init(void);
u16 Get_Adc1(u8);
u16 Get_Adc_Average1(u8,u8);
#endif



#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//ͨ�ö�ʱ�� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
extern u16 count;//����������
extern int flag1;
void TIM3_Int_Init(u16 arr,u16 psc); 
 
 
 
 
 
#define             BASIC_TIM                                 TIM6
#define             BASIC_TIM_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             BASIC_TIM_CLK                             RCC_APB1Periph_TIM6
#define             BASIC_TIM_IRQ                             TIM6_IRQn
#define             BASIC_TIM_IRQHandler                      TIM6_IRQHandler


 
void timing_init();
void timing_deinit();
void BASIC_TIM_Config(void);
void BASIC_TIM_NVIC_Config(void);
 
 
 
 
 
 
 
 
#endif

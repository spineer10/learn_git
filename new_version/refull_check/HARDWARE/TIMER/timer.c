#include "timer.h"
#include "led.h"
#include "relay.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//ͨ�ö�ʱ�� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
 

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
#include "usart.h"
u16 eachcount=0;
void TIM3_Int_Init(u16 arr,u16 psc)
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		eachcount++;
		//pntf("%d\r\n",eachcount);
		if(eachcount>=count){
			eachcount=0;

			relay_off();
			LCD_ShowString(30,270,200,16,16,"Time Over");
			flag1=0;
			
			printf("zhixing");
		}
		LED1=!LED1;
		}
}




/*
**  ��ʼ����ʱ��6
*/

void BASIC_TIM_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	
    TIM_TimeBaseStructure.TIM_Period=10000;    // 10ms����һ���ж�

    TIM_TimeBaseStructure.TIM_Prescaler= 71;  //ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (71+1)=1M
	
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
		
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(BASIC_TIM, ENABLE);
    																
    //��ʱ�رգ��ȴ�ʹ��
    BASIC_TIM_APBxClock_FUN (BASIC_TIM_CLK, DISABLE);     
}
/*
**  �����ж�Դ
*/
void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
		
	// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ;
	
	// ���������ȼ�Ϊ 1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	 
	// ������ռ���ȼ�Ϊ2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
}

/*
** �жϷ������
*/

extern u16 timing = 0;

 void BASIC_TIM_IRQHandler (void)
 {
 	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET )
 	{
		timing++;
 		TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update);
 	}
 }

 // ʹ�ܼ�ʱ
 void timing_init()
 {
	 BASIC_TIM_APBxClock_FUN (BASIC_TIM_CLK, ENABLE);
	 timing = 0;
 }
 
 // ��ֹ��ʱ
void timing_deinit()
{
	BASIC_TIM_APBxClock_FUN (BASIC_TIM_CLK, DISABLE);
}



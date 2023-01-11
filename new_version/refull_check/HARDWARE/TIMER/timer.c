#include "timer.h"
#include "led.h"
#include "relay.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//通用定时器 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
 

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
#include "usart.h"
u16 eachcount=0;
void TIM3_Int_Init(u16 arr,u16 psc)
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
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
**  初始化定时器6
*/

void BASIC_TIM_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	
    TIM_TimeBaseStructure.TIM_Period=10000;    // 10ms产生一次中断

    TIM_TimeBaseStructure.TIM_Prescaler= 71;  //时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M
	
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
		
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(BASIC_TIM, ENABLE);
    																
    //暂时关闭，等待使用
    BASIC_TIM_APBxClock_FUN (BASIC_TIM_CLK, DISABLE);     
}
/*
**  配置中断源
*/
void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
		
	// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ;
	
	// 设置主优先级为 1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	 
	// 设置抢占优先级为2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
}

/*
** 中断服务程序
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

 // 使能计时
 void timing_init()
 {
	 BASIC_TIM_APBxClock_FUN (BASIC_TIM_CLK, ENABLE);
	 timing = 0;
 }
 
 // 禁止计时
void timing_deinit()
{
	BASIC_TIM_APBxClock_FUN (BASIC_TIM_CLK, DISABLE);
}



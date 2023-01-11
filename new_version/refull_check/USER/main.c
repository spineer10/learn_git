#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "relay.h"
#include "myiic.h"
#include "bme280.h"
#include "key.h"
#include "lcd.h"
#include "adc.h"

#include "timer.h"
u16 count=100;
int flag1=0;

extern u16 timing ;



int main(void) 
{
	float bmp280_temp;                                                         //气压计参数
	float bmp280_press;
	float bmp280_humi;
	float high;
	
	float gate = 1050;
	float gata_start;
	u16 adcx;
	u16 adcx3;
	float temp3=0;
	float temp;
	u16 adcx2;
	float temp2;
	//u16 count2=100;

	//int flag1=0;
	int flag2=0;
  u16 motorpwm=0;
	int time_flag = 0;
	u16 time_count = 0;
	u16 speed=0;
//	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                            //设置中断优先级分组2
	uart_init(9600);                                                           //初始化USART
	LED_Init();                                                                //初始化LED
	relay_init();                                                              //初始化GPIO
	relay1_init();                   
	KEY_Init();
	IIC_Init();	
	delay_init();
	bme280Init();
	LCD_Init();
	Adc_Init();
	Adc2_Init();
	POINT_COLOR=RED; 
	LCD_Clear(WHITE);
	LED0=0;
	TIM3_Int_Init(4999,7199);
	TIM1_PWM_Init(8999,159);
  ADC_init();
	BASIC_TIM_Config();
	BASIC_TIM_NVIC_Config();
//	
//	
	LCD_ShowString(30,40,200,24,24,"USTC STM Project");	
	
	bme280GetData(&bmp280_press,&bmp280_temp,&bmp280_humi,&high);
	
	printf("初始值气压 ： %f \r\n",bmp280_press);
//	
	
		delay_ms(200);
	 init_controller();
	
    while(1)
    {
			
			adcx3=Get_Adc_Average1(ADC_Channel_6,10);
			temp3=3.5*(adcx3*3.3)/4096.0;       ///////////////////
			motorpwm=7430 + temp3*100;   //(考虑电阻不同从7300-7400中取)
//			set_motorpwm(motorpwm);
			
			adcx=Get_Adc_Average(ADC_Channel_0,10);    
			temp=(float)adcx*(3.3/4096);
			gate= 1000+(temp-2.1)*216.66;
		
			//printf("%.2f\r\n",gate);
		
			adcx2=Get_Adc2_Average(ADC_Channel_3,10);
			temp2=(float)adcx2*(3.3/4096);
			count=100+(temp2-1.81)*26600.66;
//		printf("%.2f\r\n",temp2);
//		printf("%d\r\n",count);
			bme280GetData(&bmp280_press,&bmp280_temp,&bmp280_humi,&high);
		
			speed = 100.0*(8584-motorpwm)/524.0;
//		printf("bmp280_press:%f\r\n",bmp280_press);
			LCD_ShowString(30,60,200,16,16,"Pressure:");
			LCD_ShowxNum(120,60,bmp280_press,8,16,0);
			LCD_ShowString(30,200,200,16,16,"Gate:");
			LCD_ShowxNum(120,200,gate,8,16,0);
			LCD_ShowString(30,230,200,16,16,"Time:");
			LCD_ShowxNum(120,230,count,8,16,0);
			LCD_ShowString(30,260,200,16,16,"Sppeed:");
			LCD_ShowxNum(120,260,speed,8,16,0);
			switch(KEY_Scan(0))
			{
				case 1 : flag1=1;break;
				case 2 : flag1=0;break;
				case 3 : if(flag2==0)
							flag2=1;
						else if(flag2==1)
							flag2=0;
						break;
			}
		
		if(bmp280_press>=gate&&flag1){
			set_motorpwm(8500);
			LCD_ShowString(30,150,200,16,16,"RELAY1 OFF  ");
			LED1=1;
			
			//测时间的代码:
			if(time_flag == 0)
			{
			timing_init();
			while(bmp280_press>=1026){
				bme280GetData(&bmp280_press,&bmp280_temp,&bmp280_humi,&high);
				LCD_ShowString(30,80,260,16,16,"real_Pressure");
				LCD_ShowxNum(120,80,bmp280_press,8,16,0);
				printf("%d , %f \r\n" , timing , bmp280_press);
				
//				LCD_ShowString(30,260,400,16,16,"Delay");
//				LCD_ShowxNum(140,260,timing,8,16,0);
			};

			LCD_ShowString(30,80,260,16,16,"              ");
			LCD_ShowString(120,80,260,16,16,"              ");

			timing_deinit();
			LCD_ShowString(30,300,400,16,16,"Delay1");
			LCD_ShowxNum(140,300,timing,8,16,0);
			time_count = timing;
			time_flag = 1;
			set_motorpwm(motorpwm);
			}

			else if(time_flag == 1)  //保压
			{
					timing_init();
					while(timing < 4000){
						bme280GetData(&bmp280_press,&bmp280_temp,&bmp280_humi,&high);
						LCD_ShowString(30,100,200,16,16,"pressure maintain");
						LCD_ShowxNum(140,100,bmp280_press,8,16,0);
					}
					LCD_ShowString(30,100,200,16,16,"  ");
					timing_deinit();
      //		relay1_on();    //20s时间到，开始泄气
					relay1_off();
					while(bmp280_press>=1008)
					{
       //			relay1_on(); 
						LCD_ShowString(30,120,200,16,16,"air exhaust");
						bme280GetData(&bmp280_press,&bmp280_temp,&bmp280_humi,&high);
						LCD_ShowxNum(120,120,bmp280_press,8,16,0);	
						
					};
      //		relay1_off();
					relay1_on();
					LCD_ShowString(30,120,200,16,16,"                                ");
					LCD_ShowString(30,130,200,16,16,"waiting....");
      //		relay1_off(); //B泄气结束
					
					//开始间隔时间
						timing_init();
						while(timing <= time_count){
							bme280GetData(&bmp280_press,&bmp280_temp,&bmp280_humi,&high);
							LCD_ShowString(30,100,200,16,16,"real_Pressure");
							LCD_ShowString(30,260,400,16,16,"TIMER");
							LCD_ShowxNum(140,260,timing,8,16,0);
						};
						timing_deinit();
						LCD_ShowString(30,130,200,16,16,"                                       ");						
						set_motorpwm(motorpwm);
						//time_flag = 2;
			}

			else if(time_flag == 2)
			{
			timing_init();
			while(timing <= time_count){
				bme280GetData(&bmp280_press,&bmp280_temp,&bmp280_humi,&high);
				LCD_ShowString(30,100,200,16,16,"real_Pressure");
				LCD_ShowxNum(120,100,bmp280_press,8,16,0);
				LCD_ShowString(30,260,400,16,16,"Delay2");
				LCD_ShowxNum(140,260,timing,8,16,0);
			};
			timing_deinit();
			set_motorpwm(motorpwm);
			LED1=0;
			
			}
			
		}
		//else if(bmp280_press<gate&&flag1)
		if(time_flag == 0)
		{
					if(flag1)
					{
						set_motorpwm(motorpwm);
						LCD_ShowString(30,150,200,16,16,"RELAY1 ON  ");
						LED1=0;
					}
					else if(flag1==0)
					{
						set_motorpwm(8500);
						LCD_ShowString(30,150,200,16,16,"RELAY1 OFF  ");
						LED1=1;
					}
					if(flag2==0)
					{
						set_motorpwm(motorpwm);
						LCD_ShowString(30,180,200,16,16,"RELAY2 OFF  ");
					}
					else if(flag2==1)
					{
						set_motorpwm(8500);
						LCD_ShowString(30,180,200,16,16,"RELAY2 ON  ");
					}
		 }
		
	}
}

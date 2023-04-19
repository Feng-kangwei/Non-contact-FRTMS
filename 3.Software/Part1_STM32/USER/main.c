/*
90614  SCL---PA11
	   SDA---PA12
OLED   SCL---PA5
	   SDA---PA7
按键   拨码1--PB9
	   拨码2--PB8
	   拨码3--PB5
	   拨码4--PB4
	   拨码5--PB3
	   拨码6--PA15
	   
	   按键1--PB7
	   按键2--PB6
	   
串口   PA9----USART1_TX
	   PA10

	   PA2
	   PA3
*/
#include "stm32f10x.h"
#include "sys.h"
#include "oled.h"
#include "key.h"
#include "mlx90614.h"
#include "Timer.h"
#include "led.h"


extern  void  My_USART1_Init();

extern float  valuable;
extern float fTbody ;
extern sMlx90614Typ sMlx ;
extern float BODY;
extern float  Up_limit;
extern float  Low_limit;



int main()
{
	char txt[16];
	delay_init() ;
	
	key_EXIT();
	KEY_Init() ;
	LED_Int(GPIOC,GPIO_Pin_13,RCC_APB2Periph_GPIOC);
	LED_Int(GPIOA,GPIO_Pin_8,RCC_APB2Periph_GPIOA);
	/* TEST */
	vMLX90614_Init(&sMlx,MLX90614_SMBUS_ADDR);
	/* TEST END */

	My_USART1_Init();
	OLED_Init() ;			

	
	while(1) 
	{
		/*初始界面*/
		OLED_Clear();
		TIM_Cmd(TIM3, DISABLE);

		if(KEYB8 == 1&&KEYB9 == 1)
		{
			while(1)
			{
				OLED_ShowCHinese(0,0,5) ;
				OLED_ShowCHinese(16,0,6) ;
				OLED_ShowCHinese(32,0,7) ; 
				OLED_ShowCHinese(48,0,8) ;
				
				OLED_ShowNum(0,2,1,1,16);
				OLED_ShowCHinese(8,2,21) ;
				OLED_ShowCHinese(16,2,9) ;
				OLED_ShowCHinese(32,2,10) ;
				OLED_ShowCHinese(48,2,11) ;
				OLED_ShowCHinese(64,2,12) ; 

				OLED_ShowNum(0,4,2,1,16);
				OLED_ShowCHinese(8,4,21) ;
				OLED_ShowCHinese(16,4,13) ;
				OLED_ShowCHinese(32,4,14) ;
				OLED_ShowCHinese(48,4,15) ;
				OLED_ShowCHinese(64,4,16) ;
				OLED_ShowCHinese(80,4,17) ;
				OLED_ShowCHinese(96,4,18) ;
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);

				if(KEYB9 == 0||KEYB8 == 0)
					break;
			}
		}

		/*拨码1界面*/
		if(KEYB9 == 0 && KEYB8 != 0)
		{
			OLED_Clear();

			OLED_ShowCHinese(24,0,0) ;
			OLED_ShowCHinese(40,0,1) ;
			OLED_ShowCHinese(56,0,2) ; 
			OLED_ShowCHinese(72,0,3) ;
			OLED_ShowCHinese(88,0,4) ;
			
			OLED_ShowCHinese(0,2,22);
			OLED_ShowCHinese(16,2,23);
			OLED_ShowCHinese(32,2,7) ; 
			OLED_ShowCHinese(48,2,8) ;
			OLED_ShowNum(66,2,3,1,16);
			OLED_ShowCHinese(74,2,36) ;
			OLED_ShowNum(90,2,4,1,16);
			OLED_ShowCHinese(100,2,21) ;
			OLED_ShowCHinese(0,4,24); 
			OLED_ShowCHinese(16,4,25);
			OLED_ShowCHinese(32,4,26);
			OLED_ShowCHinese(48,4,27);
			OLED_ShowCHinese(64,4,28);
			OLED_ShowCHinese(80,4,29);

			OLED_ShowCHinese(0,6,30); 
			OLED_ShowCHinese(16,6,31);
			OLED_ShowCHinese(32,6,32);
			OLED_ShowCHinese(48,6,33);
			OLED_ShowCHinese(64,6,34);
			OLED_ShowCHinese(80,6,35);
			OLED_ShowString(96,6,"...",4);
			
			while(KEYB6 == 1&&KEYB7 == 1)   //按下按键继续操作
			{
				if(KEYB9 != 0 || KEYB8 == 0)               //或者拨码1拨下退回主界面
					break;
			}
			
			OLED_Clear();
			TIM3_Int_Init(49,7199);
			
			while(1)
			{
				
				if(KEYB9 == 1||KEYB8 == 0)   
					break;
				
				if(KEYB9 == 0 && BODY > 48 && KEYB5 != 0 && KEYB4 != 0) 
				{
					OLED_Clear();
					BODY = 'H';
					while(1)
					{
						OLED_ShowCHinese(24,0,0) ;
						OLED_ShowCHinese(40,0,1) ;
						OLED_ShowCHinese(56,0,2) ; 
						OLED_ShowCHinese(72,0,3) ;
						OLED_ShowCHinese(88,0,4) ;
						OLED_ShowCHinese(0,4,37) ; 
						OLED_ShowCHinese(16,4,39) ;
						OLED_ShowNum(32,4,4,1,16);
						OLED_ShowNum(40,4,8,1,16);
						OLED_ShowCHinese(48,4,40) ;
						if((BODY >= 28 && BODY <= 48 && KEYB9 == 0)||KEYB9 == 1||KEYB5 == 0||KEYB4 == 0)
						{
							OLED_Clear();
							break;
						}
					}
				}
				if(KEYB9 == 0 && BODY < 28 && KEYB5 != 0 && KEYB4 != 0)
				{
					OLED_Clear();
					BODY = 'L';
					while(1)
					{
						OLED_ShowCHinese(24,0,0) ;
						OLED_ShowCHinese(40,0,1) ;
						OLED_ShowCHinese(56,0,2) ; 
						OLED_ShowCHinese(72,0,3) ;
						OLED_ShowCHinese(88,0,4) ;
						OLED_ShowCHinese(0,4,38) ; 
						OLED_ShowCHinese(16,4,39) ;
						OLED_ShowNum(32,4,2,1,16);
						OLED_ShowNum(40,4,8,1,16);
						OLED_ShowCHinese(48,4,40) ;
						if((BODY >= 28 && BODY <= 48 && KEYB9 == 0)||KEYB9 == 1||KEYB5 == 0||KEYB4 == 0)
						{
							OLED_Clear();
							break;
						}
					}
				}
				
				OLED_ShowCHinese(24,0,0) ;
				OLED_ShowCHinese(40,0,1) ;
				OLED_ShowCHinese(56,0,2) ; 
				OLED_ShowCHinese(72,0,3) ;
				OLED_ShowCHinese(88,0,4) ;
				sprintf(txt,"fTbody:%3.1f",fTbody);
				OLED_ShowString(0,4,(u8 *)txt,1);
				sprintf(txt,"Body:%3.1f",BODY);
				OLED_ShowString(0,5,(u8 *)txt,1);
				sprintf(txt,"U:%2.1f",Up_limit);
				OLED_ShowString(0,6,(u8 *)txt,1);
				sprintf(txt,"L:%2.1f",Low_limit);
				OLED_ShowString(65,6,(u8 *)txt,1);

				if(KEYB5 != 0 && KEYB4 != 0 && (BODY < Low_limit||BODY > Up_limit))
				{
					GPIO_ResetBits(GPIOC,GPIO_Pin_13);
					beep();
				}
				else
				{
					GPIO_SetBits(GPIOC,GPIO_Pin_13);
				}
			}
		}


		/*拨码2界面*/
		if(KEYB8 == 0 && KEYB9 != 0)
		{
			OLED_Clear();
			TIM3_Int_Init(49,7199);
			while(1)
			{
				OLED_ShowCHinese(24,0,41) ;
				OLED_ShowCHinese(40,0,42) ;
				OLED_ShowCHinese(56,0,43) ;
				OLED_ShowCHinese(72,0,44) ;
				OLED_ShowCHinese(88,0,45) ;
				
				sprintf(txt,"valuable:%5.2f",valuable);
				OLED_ShowString(0,2,(u8 *)txt,1);
				sprintf(txt,"fTbody:%3.1f",fTbody);
				OLED_ShowString(0,3,(u8 *)txt,1);
				sprintf(txt,"Body:%3.1f",BODY);
				OLED_ShowString(0,4,(u8 *)txt,1);
				if(KEYB8 == 1||KEYB9 == 0)
					break;
			}
		}

		/*拨码1和2同时拨下*/
		if(KEYB8 == 0&&KEYB9 == 0)
		{
			OLED_Clear();
			TIM_Cmd(TIM3, DISABLE);
			while(1)
			{
				OLED_ShowCHinese(0,0,46) ;
				OLED_ShowCHinese(16,0,47) ;
				OLED_ShowCHinese(32,0,48) ;
				OLED_ShowCHinese(48,0,49) ;
				OLED_ShowCHinese(64,0,50) ;
				OLED_ShowCHinese(80,0,51) ;
				OLED_ShowCHinese(96,0,52) ;
				OLED_ShowCHinese(112,0,53) ;
				if(KEYB8 != 0||KEYB9 != 0)
					break;
			}
		}
	}
}





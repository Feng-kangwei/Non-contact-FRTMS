/***************************************************************************************************
* Copyright (C),    //版权
* File name:mlx90614.c 	
* Author:Charmander     Version:0.1    Date:2019.4.1    //作者、版本、完成日期（初版） 
* Description:  MLX90614红外传感器源文件  
* Other:   
* Contact: 228303760（qqname： Obligation） 228303760@qq.com
* TaoBao: 捷睿电子科技
* Function List:   
* History:    //修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
*      1. Data：
*         Author：Charmander
*          Modification：
***********************************************************************************************************/

#include "mlx90614.h"
#include "smbus.h"
#include "string.h"

/********************* define ***************************/
typedef void  (* vMLX90614_BusInitHandler)() ;

typedef uint8_t (* ucMLX90614_BusWriteHandler)(uint8_t ucSlaveAddr, uint8_t ucRegAddr \
							, uint8_t *pucRegDataBuf, uint16_t uiRegDataBufLength) ;

typedef uint8_t (* ucMLX90614_BusReadHandler)(uint8_t ucSlaveAddr, uint8_t ucRegAddr \
							, uint8_t *pucRegDataBuf, uint16_t uiRegDataBufLength) ;
float a[10],num,reality;
int m[101],n;
int i;
extern float valuable;
/********************** var ****************************/
vMLX90614_BusInitHandler vMLX90614_BusInit = vSMBus_Init ;
ucMLX90614_BusReadHandler ucMLX90614_BusRead = ucSMBUS_StreamRead ;
ucMLX90614_BusWriteHandler ucMLX90614_BusWrite = ucSMBUS_StreamWrite ;

/* 初始化MLX90614,成功1，失败0。 */
uint8_t vMLX90614_Init(sMlx90614Typ *psMlx90614,uint8_t ucAddr)
{
	uint8_t ucResult = 0 ;
	uint8_t ucTempBuf[2] ;
	
	if ( psMlx90614 != NULL)
	{
		psMlx90614->ucAddr = ucAddr ;
		
		vMLX90614_BusInit() ;
		
		ucMLX90614_BusRead(psMlx90614->ucAddr, MLX90614_SLAVE_ADDR_EADDR, ucTempBuf, 2) ;
		
		if ( psMlx90614->ucAddr == ucTempBuf[0]) ucResult = 1 ;
	}
	
	return ucResult ;
}

/* 获取环境温度，并返回读取结果,成功1，失败0。 */
uint8_t ucMLX90614_GetTA(sMlx90614Typ *psMlx90614)
{
	uint8_t ucResult = 0 ;
	uint8_t ucTempBuf[2] ;
	int16_t iTemp ;
	
	if ( psMlx90614 != NULL)
	{	
		if ( ucMLX90614_BusRead(psMlx90614->ucAddr, MLX90614_TA_RADDR, ucTempBuf, 2))
		{
			ucResult = 1 ;
			
			iTemp = ucTempBuf[1] ;
			iTemp = ( iTemp << 8) | ucTempBuf[0] ;
			
			psMlx90614->Ta = ( iTemp / 50.0) - 273.15 ;
		}		
	}
	
	return ucResult ;	
}

/* 获取物体温度1，并返回读取结果 */
uint8_t ucMLX90614_GetTO1(sMlx90614Typ *psMlx90614)
{
	uint8_t ucResult = 0 ;
	uint8_t ucTempBuf[2] ;
	int16_t iTemp ;
	
	if ( psMlx90614 != NULL)
	{	
		if ( ucMLX90614_BusRead(psMlx90614->ucAddr, MLX90614_TOBJ1_RADDR, ucTempBuf, 2))
		{
			ucResult = 1 ;
			
			iTemp = ucTempBuf[1] ;
			iTemp = ( iTemp << 8) | ucTempBuf[0] ;
			
			psMlx90614->To1 = ( iTemp / 50.0) - 273.15 ;
		}		
	}
	
	return ucResult ;	
}

/* 获取物体2温度，并返回读取结果 */
uint8_t ucMLX90614_GetTO2(sMlx90614Typ *psMlx90614)
{
	uint8_t ucResult = 0 ;
	uint8_t ucTempBuf[2] ;
	int16_t iTemp ;
	
	if ( psMlx90614 != NULL)
	{	
		if ( ucMLX90614_BusRead(psMlx90614->ucAddr, MLX90614_TOBJ2_RADDR, ucTempBuf, 2))
		{
			ucResult = 1 ;
			
			iTemp = ucTempBuf[1] ;
			iTemp = ( iTemp << 8) | ucTempBuf[0] ;
			
			psMlx90614->To2 = ( iTemp / 50.0) - 273.15 ;
		}		
	}
	
	return ucResult ;	
}

/* 设置发射率，并返回读取结果 */
uint8_t ucMLX90614_SetEmssivity(sMlx90614Typ *psMlx90614)
{
	uint8_t ucResult = 0 ;
	uint8_t ucTempBuf[2] = {0x00,0x00} ;
	int16_t iTemp ;
	
	if ( psMlx90614 != NULL)
	{	
		/* 不清楚为什么此处必须使用全局地址 */
		if ( ucMLX90614_BusWrite(MLX90614_SMBUS_ALLADDR, MLX90614_EMISSIVITY_COEFFICIENT_EADDR, ucTempBuf, 2))
		{
			ucTempBuf[0] = (uint8_t)( (uint16_t)( 0xFFFF * psMlx90614->Emssivity) & 0x00FF) ;
			ucTempBuf[1] = (uint8_t)( (uint16_t)( 0xFFFF * psMlx90614->Emssivity) >> 8) ;
				
			delay_ms(1000) ;
			
//			/* 不清楚为什么此处必须使用全局地址 */
			if ( ucMLX90614_BusWrite(MLX90614_SMBUS_ALLADDR, MLX90614_EMISSIVITY_COEFFICIENT_EADDR, ucTempBuf, 2))
			{
				ucResult = 1 ;
			}
		}		
	}
	
	return ucResult ;
}



/* 温度补偿设置算法*/   /*与环境温度做比对计算误差*/

float ucMLX90614_TempComp(sMlx90614Typ *psMlx90614)
{
	float Tl ;
	float Th ;
	float Tbody ;
	
	if ( psMlx90614->Ta < 25)   
	{
		Tl = 32.66 + ( 0.186 * ( psMlx90614->Ta - 25)) ; 
		
	}
	else
	{
		Tl = 32.66 + ( 0.086 *( psMlx90614->Ta - 25)) ; 
		Th = 34.84 + ( 0.100 * ( psMlx90614->Ta - 25)) ;
	}
	
	if ( psMlx90614->To1 < Tl)
	{
		Tbody = 36.3 + valuable +( ( 0.551658273 + 0.021525068 * psMlx90614->Ta) * (psMlx90614->To1 - Tl)) ;
	}
	else if ( Th < psMlx90614->To1)
	{
		Tbody = 36.8 + valuable +( ( 0.829320618 + 0.002364434 * psMlx90614->Ta) * (psMlx90614->To1 -Th)) ;
	}
	else
	{
		Tbody = 36.3 + valuable + (0.5 / ( ( Th - Tl) * ( psMlx90614->To1 - Tl))) ;
	}
	
	return Tbody ;
}

float win(float T_in)
{
	a[0] = T_in;
	num = 0;
	for(i=9;i>0;i--)
	{
		a[i]=a[i-1];
	}
	quicksort(1,10);
	for(i=1;i<9;i++)
	{
		num += a[i];
	}
	reality = num/8;
	return reality;
}

void  quicksort(int left,int right)
{
    int  i,j,t,temp;
	  if(left>right)
		return;

	  temp=m[left];
	  i=left;
	  j=right;
	  while(i!=j)
	  {
	       while(m[j]>=temp && i<j)
			   j--;
		   while(m[i]<=temp && i<j)
			   i++;
		   if(i<j)
		   {
			   t=m[i];
			   m[i]=m[j];
			   m[j]=t;
		   }
	  }
	  m[left]=m[i];
	  m[i]=temp;

	  quicksort(left,i-1);
	  quicksort(i+1,right);  
      return;
}


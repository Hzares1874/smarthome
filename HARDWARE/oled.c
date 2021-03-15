#include "oled.h"
#include "font.h"
#include "stdlib.h"
 
uint8_t OLED_GRAM[128][8];



void OLEDIoInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE );	 //使能PG

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_2 | GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 	GPIO_Init( GPIOG, &GPIO_InitStructure );
 	GPIO_SetBits( GPIOG,GPIO_Pin_4 | GPIO_Pin_2 | GPIO_Pin_3| GPIO_Pin_5| GPIO_Pin_6 );
	
}



/*
================================================================================
Function : OLEDWrByte
Description :写一个字节
Input : dat:字节内容 cmd 0:命令 1:数据
Output: None
================================================================================
*/
static void OLEDWrByte( uint8_t dat,uint8_t cmd )
{	
	uint8_t i = 0;
	
	if( cmd ) //写数据
	{
    OLED_DC_H();
  }
	 
	else //命令
	{
    OLED_DC_L();
  }	
			  
	OLED_CS_L();
	
	for( i = 0;i < 8;i++ )
	{			  
		OLED_SCLK_L();
		
		if( dat & 0x80 )
		{
      OLED_DIN_H();
    }
		   
		else 
		{
      OLED_DIN_L();
    }
		   
		OLED_SCLK_H();
		dat <<= 1;   
	}
	
	OLED_CS_H();
	OLED_DC_H();   	  
} 

/*
================================================================================
Function : OLEDOpen
Description :OLED开显示
Input : None
Output: None
================================================================================
*/   
void OLEDOpen(void)
{
	OLEDWrByte( 0X8D,OLED_CMD );  
	OLEDWrByte( 0X14,OLED_CMD );  
	OLEDWrByte( 0XAF,OLED_CMD ); 
}

/*
================================================================================
Function : OLEDClose
Description :OLED关显示
Input : None
Output: None
================================================================================
*/     
void OLEDClose(void)
{
	OLEDWrByte( 0X8D,OLED_CMD ); 
	OLEDWrByte( 0X10,OLED_CMD );  
	OLEDWrByte( 0XAE,OLED_CMD );  
}	


/*
================================================================================
Function : OLED_DrawPoint
Description :OLED画点
Input : x,横坐标 y,纵坐标,dot 0:清点 1:打点
Output: None
================================================================================
*/
void OLED_DrawPoint( uint8_t x,uint8_t y,uint8_t dot )
{
	uint8_t pos = 0;
	uint8_t bx = 0;
	uint8_t temp = 0;
	
	if( x > 127 || y > 63 )
	{
	  return;
  }	

	pos = 7 - y / 8;
	bx = y % 8;
	temp = 1 << ( 7 - bx );
	
	if( dot )
	{
    OLED_GRAM[x][pos] |= temp;
  }	
	
	else
  {
    OLED_GRAM[x][pos] &= ~temp;
  }	    
}

/*
================================================================================
Function : OLEDClear
Description :OLED清屏
Input : None
Output: None
================================================================================
*/
void OLEDClear(void)  
{  
	uint8_t i,n;
  
	for( i = 0;i < 8;i++ )
	{
		for( n = 0;n < 128;n++ )
	  {
      OLED_GRAM[n][i] = 0X00;
    }
  }
 
	OledRefreshGram();//更新显示
}

/*
================================================================================
Function : OledFill
Description :填充矩形
Input : x1,yi:起始坐标 x2,y2:终点坐标 dot 0:清除 1:填充
Output: None
================================================================================
*/
void OledFill( uint8_t x,uint8_t y,uint8_t W,uint8_t H ,uint8_t mode )
{
  uint8_t i,j;
	for( i = x;i < x + W;i++ )
	{
    for(j = y;j < y + H;j++ )
		{
      if( mode )
			{
			  OLED_DrawPoint( i,j,1 );
      }
			else
			{
        OLED_DrawPoint( i,j,0 );
      }
    }
  }
}

/*
================================================================================
Function : OledInit
Description :初始化OLED
Input : None
Output: None
================================================================================
*/				    
void OledInit(void)
{ 	
	OLED_RST_H();
	delay_ms(100);
 	OLED_RST_L();
	delay_ms(100);
	OLED_RST_H(); 

	OLEDWrByte(0xAE,OLED_CMD); //关闭显示
	OLEDWrByte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLEDWrByte(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
	OLEDWrByte(0xA8,OLED_CMD); //设置驱动路数
	OLEDWrByte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	OLEDWrByte(0xD3,OLED_CMD); //设置显示偏移
	OLEDWrByte(0X00,OLED_CMD); //默认为0

	OLEDWrByte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.
													    
	OLEDWrByte(0x8D,OLED_CMD); //电荷泵设置
	OLEDWrByte(0x14,OLED_CMD); //bit2，开启/关闭
	OLEDWrByte(0x20,OLED_CMD); //设置内存地址模式
	OLEDWrByte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLEDWrByte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLEDWrByte(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLEDWrByte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLEDWrByte(0x12,OLED_CMD); //[5:4]配置
		 
	OLEDWrByte(0x81,OLED_CMD); //对比度设置
	OLEDWrByte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLEDWrByte(0xD9,OLED_CMD); //设置预充电周期
	OLEDWrByte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLEDWrByte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLEDWrByte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLEDWrByte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLEDWrByte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	OLEDWrByte(0xAF,OLED_CMD); //开启显示	

	OLEDClear();
}  

/*
================================================================================
Function : OLED_ShowChar
Description :显示一个字符
Input : x:横坐标 y:纵坐标 chr:字符 size:大小 mode 0:反白显示 1:正常显示
Output: None
================================================================================
*/
void OLED_ShowChar( uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode )
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	chr = chr-' ';//得到偏移后的值				   
	
	for( t = 0;t < size;t++ )
	{   
		if( size == 12 ) //1206
		{
			temp = ASCII_1206[chr][t];
		}
		
		else 
		{
			temp = ASCII_1608[chr][t];
		}	
														
		for( t1 = 0;t1 < 8;t1++ )
		{
			if( temp & 0x80 )
			{
        OLED_DrawPoint( x,y,mode );
      }
			else
      {
        OLED_DrawPoint(x,y,!mode);
      }
			
			temp <<= 1;
			y++;
			if( ( y - y0 ) == size )
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}	
}

/*
================================================================================
Function : Pow
Description 返回 m^n次方
Input : m ,n数值
Output: m^n次方
================================================================================
*/
uint32_t Pow( uint8_t m,uint8_t n )
{
	uint32_t res = 1;
	
	while( n-- )
	{
	  res *= m;
  }	
    
	return res;
}	

/*
================================================================================
Function : OLEDDisNum
Description :OLED显示数值(0~4294967295)
Input : x:横坐标 y:纵坐标 num:数值 len:长度 size:字体大小 
Output: None
================================================================================
*/
void OLEDDisNum( uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size )
{         	
	uint8_t t,temp;
	uint8_t enshow = 0;
	
	for( t = 0;t < len;t++ )
	{
		temp =( num / Pow(10,len - t - 1 ) ) % 10;
		
		if( enshow == 0 && t < ( len - 1 ) )
		{
			if( temp == 0 )
			{
				OLED_ShowChar( x + ( size / 2 ) * t,y,'0',size,1 );
				continue;
			}
			
			else 
			{
			  enshow=1;
      }	 
		}
	 	OLED_ShowChar( x + ( size / 2 ) * t,y,temp + '0',size,1 ); 
	}
} 

/*
================================================================================
Function : OLEDDrawLine
Description :OLED画直线
Input : x1，y1起始坐标 x2,y2终点坐标
Output: None
================================================================================
*/
void OLEDDrawLine( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 )
{
	uint8_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1;

	if( delta_x > 0 )
	{
    incx=1; //设置单步方向 
  }
	else if(delta_x==0)
	{
	  incx=0;//垂直线 
  }
	else
  {
    incx=-1;
		delta_x = -delta_x;
  }
	if(delta_y>0)
	{
	  incy = 1;
  }
	else if( delta_y == 0 )
	{
	  incy = 0;//水平线 
  }	
	else
  {
    incy = -1;
    delta_y = -delta_y;
  }
  	
	if( delta_x > delta_y )
	{
	  distance = delta_x; //选取基本增量坐标轴 
  }	
	else
  {
	  distance = delta_y; 
  }
	
	for( t = 0;t <= distance + 1;t++ )//画线输出 
	{  
		OLED_DrawPoint( uRow,uCol,1 );//画点 
		xerr += delta_x ; 
		yerr += delta_y ; 
		if( xerr > distance ) 
		{ 
			xerr -= distance; 
			uRow += incx; 
		} 
		if( yerr > distance ) 
		{ 
			yerr -= distance; 
			uCol += incy; 
		} 
	}  
}

/*
================================================================================
Function : OLEDDrawRectangle
Description :OLED画矩形
Input : x，y起始坐标 W:宽度 H:高度
Output: None
================================================================================
*/
void OLEDDrawRectangle( uint8_t x, uint8_t y, uint8_t W, uint8_t H )
{
	OLEDDrawLine( x,y,x,y + H );
	OLEDDrawLine( x,y,x + W,y );
	OLEDDrawLine( x,y + H,x + W,y + H );
	OLEDDrawLine( x + W,y,x + W,y + H );
}

/*
================================================================================
Function : OLEDDisStr
Description :显示字符串
Input : x:横坐标 y:纵坐标 width:长度范围 height:高度范围 p:字符串指针
Output: None
================================================================================
*/
void OLEDDisStr( uint8_t x,uint8_t y,uint8_t size,uint8_t *p )
{         
// 	uint8_t x0 = x;
// 	width += x;
// 	height += y;
	
	while( ( *p <= '~' ) &&( *p >= ' ' ) )//判断是不是非法字符!
	{       
// 		if( x >= width )
//     {
//       x = x0;
//       y += size;
//     }
// 		if( y >= height )
// 		{
// 		  break;
//     }	

		OLED_ShowChar( x,y,*p,size,1 );
		x += size / 2 + 2;
		p++;
	}  
}

/*
================================================================================
Function : OLEDDisHZ
Description :显示16 * 16汉字
Input : x:横坐标 y:纵坐标 s:字符串指针
Output: None
================================================================================
*/
void OLEDDisHZ( uint8_t x,uint8_t y,char *s )
{
	uint8_t k = 0;
	uint8_t i,j;
	
	while(*s)
	{
		for( k = 0;k < hz16_num;k++ ) 
		{
			if( ( hz16[k].Index[0] == *(s) ) && ( hz16[k].Index[1] == *( s + 1 ) ) )
			{ 
				for( i = 0;i < 16;i++ )
				{
					for( j = 0;j < 8;j++ ) 
					{
						if( hz16[k].Msk[ i * 2 ] & ( 0x80 >> j ) )
						{
							OLED_DrawPoint( x + j,y + i,1 );
						}	
					}
					
					for( j = 0;j < 8;j++ ) 
					{
						if(hz16[k].Msk[i*2+1]&(0x80>>j))
						{
							OLED_DrawPoint( x + j + 8,y + i,1 );
						}
					}
				}
			}
		}
		s += 2;x += 16;
  }
}

/*
================================================================================
Function : OledRefreshGram
Description :OLED更新GRAM
Input : None
Output: None
================================================================================
*/
void OledRefreshGram(void)
{
	uint8_t i,n;
	
	for( i = 0;i < 8;i++ )  
	{  
		OLEDWrByte( 0xb0 + i,OLED_CMD ); //设置页地址（0~7）
		OLEDWrByte( 0x00,OLED_CMD ); //设置显示位置—列低地址
		OLEDWrByte( 0x10,OLED_CMD ); //设置显示位置—列高地址   
		
		for( n = 0;n < 128;n++ )
	  {
      OLEDWrByte(OLED_GRAM[n][i],OLED_DATA); 
    }
	}   
}


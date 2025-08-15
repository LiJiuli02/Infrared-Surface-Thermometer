#include <msp430x24x.h>
#include "ds1302.h"
#include "uart0.h"
#include "ADC_Func.h"
#include "DataTrans.h"
#include <stdio.h>
#include <string.h>

#define wei_h P5OUT|= BIT6
#define wei_l P5OUT&= ~BIT6
#define duan_h  P5OUT |= BIT5
#define duan_l   P5OUT &= ~BIT5
typedef unsigned char uchar;
typedef unsigned int uint;
#define   Num_of_Results 10
//数码管7位段码：0--f . - _
unsigned char scandata[20] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
                      0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x80,0x40,0x08,0x00};
//0x08"-",0x00不亮
uint results_0[Num_of_Results];    //保存ADC转换结果的数组
uint results_1[Num_of_Results];    //保存ADC转换结果的数组
//ADC结果平均
uint average_0;
uint average_1;
//数码管显示缓存
uchar buffer[5];

//si111参数
uint Rt;  //铂电阻阻值
uint Tk; //传感器自身温度
int Tt; //目标温度
uchar TtH,TtL; //目标温度整数部分，小数部分

//记录显示位数的全局变量
uchar cnt_data; //数码管扫描
char s[40];     //串口发送打印格式
//uchar  RecBuf[7]; //串口收
//uchar  Windex;    //串口收计数
//秒，分，时，日，月，星期，年
unsigned char wdata[7]={0x30,0x43,0x16,0x13,0x05,0x01,0x23}; //设置时间变量
unsigned char rdata[7];    //读取时间保存变量


/****************************************************************************
* 名    称：ClkInit
* 功    能：时钟系统初始化  MCLK为8MHz，SMCLK为1MHz
* 入口参数：无
* 出口参数：无
****************************************************************************/
void ClkInit()
{
    char i;
   /*------选择系统主时钟为8MHz-------*/
    BCSCTL1 &= ~XT2OFF;                 //打开XT2高频晶体振荡器
    do
    {
        IFG1 &= ~OFIFG;                 //清除晶振失败标志
        for (i = 0xFF; i > 0; i--);     //等待8MHz晶体起振
    }
    while ((IFG1 & OFIFG));             //晶振失效标志仍然存在？
    BCSCTL2 |= SELM_2 + SELS;           //MCLK和SMCLK选择高频晶振
     
}


///////////////////////主函数///////////////////////////////////////////
void  main( void )
{
  P4DIR = 0XFF;P4OUT = 0XFF;
  P5DIR = 0XFF;P5OUT = 0XFF;
  P6DIR = 0XFF;P6OUT = 0XFF;
  
  WDTCTL = WDT_ADLY_1_9;        // 设置内部看门狗工作在定时器模式，1.9ms中断一次
  IE1 |= WDTIE;                 // 使能看门狗中断
  
   _EINT();                        //使能全局中断
   
  ClkInit();
  InitUART();
  Init_ADC();
  Reset_DS1302();     
    
  /*****更改时间*****/
  Set_DS1302(wdata);    
 
  while(1)
  {    
    Get_DS1302(rdata);

    sprintf(s, "20%02d-%02d-%02d %02d:%02d:%02d ",rdata[0],rdata[1],rdata[2],rdata[3],rdata[4],rdata[5]); 
    PutString(s);
    
    memset(s, 40, 0);
    if (Tt > 0)
      sprintf(s, "%d.%02d \r\n" ,TtH,TtL); 
    else 
      sprintf(s, "-%d.%02d \r\n" ,TtH,TtL);  
    PutString(s);
    
    delay_ms(1000);
    delay_ms(1000);
  }
  
}

/*******************************************
函数名称：ADC12ISR
功    能：ADC中断服务函数，在这里用多次平均的
          计算P6.0口的模拟电压数值
参    数：无       
返回值  ：无
********************************************/
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
    static uchar index_0 = 0;
    static uchar index_1 = 0;

    // 存储第一路 ADC 通道的转换结果
    results_0[index_0++] = ADC12MEM0;
    if(index_0 == Num_of_Results)
    {
      uchar i;
      average_0 = 0;
      for(i = 0; i < Num_of_Results; i++)
          average_0 += results_0[i];
      average_0 = average_0 / Num_of_Results;                            
      index_0 = 0;
      Rt = Rt_Calculate(average_0);
      Tk = Tk_Calculate(Rt); // 计算量过大，影响程序
    }
    // 存储第二路 ADC 通道的转换结果
    results_1[index_1++] = ADC12MEM1;
    if(index_1 == Num_of_Results)
    {
      uchar i;
      average_1 = 0;
      for(i = 0; i < Num_of_Results; i++)
          average_1 += results_1[i];
      average_1 = average_1 / Num_of_Results;
      index_1 = 0;
      Tt = Tt_Calculate(Tk, average_1);
//      Tt = -1234;
      Temp_Dis(Tt,buffer, &TtH, &TtL);
    } 

    
}

/*******************************************
函数名称：watchdog_timer
功    能：看门狗中断服务函数，在这里输出数码管的
          段选和位选信号
参    数：无
返回值  ：无
********************************************/
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{  
    
    /*数码管显示*/

    P4OUT = 0x00;
    duan_h;
    duan_l;
    
    //段选
    if (cnt_data == 2)
      P4OUT = scandata[buffer[cnt_data]] + 0x80 ;
    else
      P4OUT = scandata[buffer[cnt_data]] ; 
 
    duan_h; //段选给高电平 使能段选
    duan_l; //段选给低电平
    
   //位选 
    P4OUT = ~(1 << cnt_data);        //输出位选信号
    wei_h; //位选给高电平 使能位选
    wei_l; //位选给低电平
    
    cnt_data ++;
    cnt_data = cnt_data % 5;                 //cnt >= 8时，cnt=0 cnt~0-7
}

/*******************************************
函数名称：UART0_RXISR
功    能：UART0的接收中断服务函数，在这里唤醒
          CPU，使它退出低功耗模式
参    数：无
返回值  ：无
********************************************/
//#pragma vector = UART0RX_VECTOR
//__interrupt void UART0_RXISR(void)
//{
//  Windex = 0;
//  RecBuf[Windex++] = RXBUF0;
//  Set_DS1302(RecBuf);
//}


#include <msp430.h>
#include <math.h>
typedef unsigned int  uint;
typedef unsigned char uchar;
typedef unsigned long ulong;
//SI-111参数
#define   A   11292410     //1.129241*10^(-3)
#define   B   2341077      //2.341077*10^(-4)
#define   MyC 877.5468     //8.775468*10^(-8) 

//放大倍数
#define   Gain 275.0
//抬升(mV)
#define   Lift 1980.0
/*******************************************
函数名称：Rt_Calculate
功    能：计算Rt
参    数：Vadc_S25 ADC采集单端电压，0-2.5V  
返回值  ：Rt
********************************************/
unsigned int Rt_Calculate(uint Vadc_S25) 
{
  uint Rt;
  Rt = 24900 * ( 4096.0 / Vadc_S25 - 1.0 );   //Vadc_S25是ADC值，不是模拟电压值。该公式经过化简
  
  return Rt;
}

/*******************************************
函数名称：Tk_Calculate
功    能：计算 通过Rt计算Tk
参    数：Rt--16进制数据   
返回值  ：Tk放大100倍
********************************************/
unsigned int Tk_Calculate(uint Rt)
{
  ulong Tk_temp;
  uint Tk;
  double log_Rt;
  log_Rt = log(Rt);
  Tk_temp = A + B * log_Rt + MyC * log_Rt*log_Rt*log_Rt;
  
//  Tk_temp = A + B * log(Rt);
  Tk_temp = 100 * 10000000000 / Tk_temp;  //Tk放大了100倍
  Tk = (int)Tk_temp;
  return Tk;
}

/*******************************************
函数名称：Tt_Calculate
功    能：通过Tk计算Tt
参    数：   
返回值  ：Tt放大100倍
********************************************/
int Tt_Calculate(uint Tk,uint Vadc_D33)
{
  long Tt_temp;
  int Tt;
  Tt_temp = 1000.0* ((3300.0 * Vadc_D33 / 4096.0 ) - Lift) ; //未抬升的放大电压uV
  Tt_temp = Tt_temp / Gain ;  //关键步骤 原始差分电压uV
  Tt_temp = 100.0 * (Tt_temp / 60.0);  //计算温差，放大了100倍
  Tt_temp = Tt_temp + Tk  - 27315;    //计算最终温度
  Tt = Tt_temp;
  return Tt;
}
  
/*******************************************
函数名称：Temp_Dis
功    能：将温度转换成数码管显示字符
参    数：Tk--16位数据   
返回值  ：
********************************************/
void Temp_Dis(int Tt, uchar *ptr, uchar *TtH, uchar *TtL)
{
    if (Tt < 0)      //判断符号
    {
      ptr[0] = 17 ;     //数码管负号
      uint uTt;
      uTt = -Tt;
      //Hex->Dec变换  
      ptr[1] = uTt / 1000 ;
      ptr[2] = uTt / 100 % 10;
      ptr[3] = uTt / 10 % 10;
      ptr[4] = uTt % 10;
      *TtH = uTt / 100;
      *TtL = uTt - *TtH * 100;
    }
    else 
    {
      ptr[0] = 19;     //全灭
      //Hex->Dec变换              
      ptr[1] = Tt / 1000 ;
      ptr[2] = Tt / 100 % 10;
      ptr[3] = Tt / 10 % 10;
      ptr[4] = Tt % 10;
      *TtH = Tt / 100;
      *TtL = Tt - *TtH * 100;
    }
}
#include <msp430.h>
#include <math.h>
typedef unsigned int  uint;
typedef unsigned char uchar;
typedef unsigned long ulong;
//SI-111����
#define   A   11292410     //1.129241*10^(-3)
#define   B   2341077      //2.341077*10^(-4)
#define   MyC 877.5468     //8.775468*10^(-8) 

//�Ŵ���
#define   Gain 275.0
//̧��(mV)
#define   Lift 1980.0
/*******************************************
�������ƣ�Rt_Calculate
��    �ܣ�����Rt
��    ����Vadc_S25 ADC�ɼ����˵�ѹ��0-2.5V  
����ֵ  ��Rt
********************************************/
unsigned int Rt_Calculate(uint Vadc_S25) 
{
  uint Rt;
  Rt = 24900 * ( 4096.0 / Vadc_S25 - 1.0 );   //Vadc_S25��ADCֵ������ģ���ѹֵ���ù�ʽ��������
  
  return Rt;
}

/*******************************************
�������ƣ�Tk_Calculate
��    �ܣ����� ͨ��Rt����Tk
��    ����Rt--16��������   
����ֵ  ��Tk�Ŵ�100��
********************************************/
unsigned int Tk_Calculate(uint Rt)
{
  ulong Tk_temp;
  uint Tk;
  double log_Rt;
  log_Rt = log(Rt);
  Tk_temp = A + B * log_Rt + MyC * log_Rt*log_Rt*log_Rt;
  
//  Tk_temp = A + B * log(Rt);
  Tk_temp = 100 * 10000000000 / Tk_temp;  //Tk�Ŵ���100��
  Tk = (int)Tk_temp;
  return Tk;
}

/*******************************************
�������ƣ�Tt_Calculate
��    �ܣ�ͨ��Tk����Tt
��    ����   
����ֵ  ��Tt�Ŵ�100��
********************************************/
int Tt_Calculate(uint Tk,uint Vadc_D33)
{
  long Tt_temp;
  int Tt;
  Tt_temp = 1000.0* ((3300.0 * Vadc_D33 / 4096.0 ) - Lift) ; //δ̧���ķŴ��ѹuV
  Tt_temp = Tt_temp / Gain ;  //�ؼ����� ԭʼ��ֵ�ѹuV
  Tt_temp = 100.0 * (Tt_temp / 60.0);  //�����²�Ŵ���100��
  Tt_temp = Tt_temp + Tk  - 27315;    //���������¶�
  Tt = Tt_temp;
  return Tt;
}
  
/*******************************************
�������ƣ�Temp_Dis
��    �ܣ����¶�ת�����������ʾ�ַ�
��    ����Tk--16λ����   
����ֵ  ��
********************************************/
void Temp_Dis(int Tt, uchar *ptr, uchar *TtH, uchar *TtL)
{
    if (Tt < 0)      //�жϷ���
    {
      ptr[0] = 17 ;     //����ܸ���
      uint uTt;
      uTt = -Tt;
      //Hex->Dec�任  
      ptr[1] = uTt / 1000 ;
      ptr[2] = uTt / 100 % 10;
      ptr[3] = uTt / 10 % 10;
      ptr[4] = uTt % 10;
      *TtH = uTt / 100;
      *TtL = uTt - *TtH * 100;
    }
    else 
    {
      ptr[0] = 19;     //ȫ��
      //Hex->Dec�任              
      ptr[1] = Tt / 1000 ;
      ptr[2] = Tt / 100 % 10;
      ptr[3] = Tt / 10 % 10;
      ptr[4] = Tt % 10;
      *TtH = Tt / 100;
      *TtL = Tt - *TtH * 100;
    }
}
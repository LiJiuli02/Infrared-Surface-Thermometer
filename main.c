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
//�����7λ���룺0--f . - _
unsigned char scandata[20] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
                      0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x80,0x40,0x08,0x00};
//0x08"-",0x00����
uint results_0[Num_of_Results];    //����ADCת�����������
uint results_1[Num_of_Results];    //����ADCת�����������
//ADC���ƽ��
uint average_0;
uint average_1;
//�������ʾ����
uchar buffer[5];

//si111����
uint Rt;  //��������ֵ
uint Tk; //�����������¶�
int Tt; //Ŀ���¶�
uchar TtH,TtL; //Ŀ���¶��������֣�С������

//��¼��ʾλ����ȫ�ֱ���
uchar cnt_data; //�����ɨ��
char s[40];     //���ڷ��ʹ�ӡ��ʽ
//uchar  RecBuf[7]; //������
//uchar  Windex;    //�����ռ���
//�룬�֣�ʱ���գ��£����ڣ���
unsigned char wdata[7]={0x30,0x43,0x16,0x13,0x05,0x01,0x23}; //����ʱ�����
unsigned char rdata[7];    //��ȡʱ�䱣�����


/****************************************************************************
* ��    �ƣ�ClkInit
* ��    �ܣ�ʱ��ϵͳ��ʼ��  MCLKΪ8MHz��SMCLKΪ1MHz
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void ClkInit()
{
    char i;
   /*------ѡ��ϵͳ��ʱ��Ϊ8MHz-------*/
    BCSCTL1 &= ~XT2OFF;                 //��XT2��Ƶ��������
    do
    {
        IFG1 &= ~OFIFG;                 //�������ʧ�ܱ�־
        for (i = 0xFF; i > 0; i--);     //�ȴ�8MHz��������
    }
    while ((IFG1 & OFIFG));             //����ʧЧ��־��Ȼ���ڣ�
    BCSCTL2 |= SELM_2 + SELS;           //MCLK��SMCLKѡ���Ƶ����
     
}


///////////////////////������///////////////////////////////////////////
void  main( void )
{
  P4DIR = 0XFF;P4OUT = 0XFF;
  P5DIR = 0XFF;P5OUT = 0XFF;
  P6DIR = 0XFF;P6OUT = 0XFF;
  
  WDTCTL = WDT_ADLY_1_9;        // �����ڲ����Ź������ڶ�ʱ��ģʽ��1.9ms�ж�һ��
  IE1 |= WDTIE;                 // ʹ�ܿ��Ź��ж�
  
   _EINT();                        //ʹ��ȫ���ж�
   
  ClkInit();
  InitUART();
  Init_ADC();
  Reset_DS1302();     
    
  /*****����ʱ��*****/
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
�������ƣ�ADC12ISR
��    �ܣ�ADC�жϷ��������������ö��ƽ����
          ����P6.0�ڵ�ģ���ѹ��ֵ
��    ������       
����ֵ  ����
********************************************/
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
    static uchar index_0 = 0;
    static uchar index_1 = 0;

    // �洢��һ· ADC ͨ����ת�����
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
      Tk = Tk_Calculate(Rt); // ����������Ӱ�����
    }
    // �洢�ڶ�· ADC ͨ����ת�����
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
�������ƣ�watchdog_timer
��    �ܣ����Ź��жϷ��������������������ܵ�
          ��ѡ��λѡ�ź�
��    ������
����ֵ  ����
********************************************/
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{  
    
    /*�������ʾ*/

    P4OUT = 0x00;
    duan_h;
    duan_l;
    
    //��ѡ
    if (cnt_data == 2)
      P4OUT = scandata[buffer[cnt_data]] + 0x80 ;
    else
      P4OUT = scandata[buffer[cnt_data]] ; 
 
    duan_h; //��ѡ���ߵ�ƽ ʹ�ܶ�ѡ
    duan_l; //��ѡ���͵�ƽ
    
   //λѡ 
    P4OUT = ~(1 << cnt_data);        //���λѡ�ź�
    wei_h; //λѡ���ߵ�ƽ ʹ��λѡ
    wei_l; //λѡ���͵�ƽ
    
    cnt_data ++;
    cnt_data = cnt_data % 5;                 //cnt >= 8ʱ��cnt=0 cnt~0-7
}

/*******************************************
�������ƣ�UART0_RXISR
��    �ܣ�UART0�Ľ����жϷ������������﻽��
          CPU��ʹ���˳��͹���ģʽ
��    ������
����ֵ  ����
********************************************/
//#pragma vector = UART0RX_VECTOR
//__interrupt void UART0_RXISR(void)
//{
//  Windex = 0;
//  RecBuf[Windex++] = RXBUF0;
//  Set_DS1302(RecBuf);
//}


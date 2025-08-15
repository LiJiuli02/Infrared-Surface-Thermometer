#include <msp430.h>

typedef unsigned int  uint;
typedef unsigned char uchar;

/********************************************
�������ƣ�Init_ADC
��    �ܣ���ʼ��ADC
��    ������
����ֵ  ����
********************************************/
void Init_ADC(void)
{
    P6SEL |= 0x03;                            // ʹ��ADCͨ�� p6.0 p6.1
    ADC12CTL0 = ADC12ON+SHT0_15+MSC+REF2_5V+REFON;   // ��ADC�����ò���ʱ��
    ADC12CTL1 = ADC12DIV_6+SHP+CONSEQ_3;             // ʹ�ò�����ʱ��
    ADC12MCTL0 |= SREF_0+INCH_0;
    ADC12MCTL1 |= SREF_1 + INCH_1 | EOS  ;            // �ڶ�·����ͨ��Ϊ A1������Ϊ���е����һ��ת��
    ADC12IE = 0x0002;                           // ʹ��ADC�ж�
    ADC12CTL0 |= ENC;                         // ʹ��ת��
    ADC12CTL0 |= ADC12SC;                     // ��ʼת��
}


///********************************************
//�������ƣ�Hex2Dec
//��    �ܣ���16����ADCת�����ݱ任��ʮ����
//          ��ʾ��ʽ
//��    ����Hex_Val--16��������  
//          ptr--ָ����ת�������ָ��
//����ֵ  ����
//********************************************/
//void Hex2Dec(uint Hex_val,uchar *ptr)
//{
//    ptr[0] = Hex_val / 1000;        
//    ptr[1] = (Hex_val - ptr[0]*1000)/100;
//    ptr[2] = (Hex_val - ptr[0]*1000 - ptr[1]*100)/10;
//    ptr[3] = (Hex_val - ptr[0]*1000 - ptr[1]*100 - ptr[2]*10); 
//}   
///*******************************************
//�������ƣ�Trans_val_33
//��    �ܣ���16����ADCת�����ݱ任����λ10����
//          ��ʵ��ģ���ѹ����
//��    ����Hex_Val--16��������   
//����ֵ  ����
//********************************************/
//void Trans_val_33(uint Hex_Val,uchar *ptr)
//{
//    float caltmp;
//    uint Curr_Volt;
//    uchar t1;
//    
//    caltmp = (Hex_Val*3.3)/4095.0;           //caltmp = Hex_Val * 3.3/4096  
//    Curr_Volt = (100*caltmp+0.5);             //�Ŵ�100����������
//    ptr[0] = Curr_Volt / 100;                //Hex->Dec�任
//    t1 = Curr_Volt - (ptr[0] * 100);
//    ptr[1] = t1 / 10;
//    ptr[2] = t1 - (ptr[1] * 10);
//}
//
///*******************************************
//�������ƣ�Trans_val_25
//��    �ܣ���16����ADCת�����ݱ任����λ10����
//          ��ʵ��ģ���ѹ����
//��    ����Hex_Val--16��������   
//����ֵ  ����
//********************************************/
//void Trans_val_25(uint Hex_Val,uchar *ptr)
//{
//    float caltmp;
//    uint Curr_Volt;
//    uchar t1;
//    
//    caltmp = (Hex_Val*2.5)/4095.0;           //caltmp = Hex_Val * 2.3/4096  
//    Curr_Volt = (100*caltmp+0.5);             //�Ŵ�100����������
//    ptr[0] = Curr_Volt / 100;                //Hex->Dec�任
//    t1 = Curr_Volt - (ptr[0] * 100);
//    ptr[1] = t1 / 10;
//    ptr[2] = t1 - (ptr[1] * 10);
//}



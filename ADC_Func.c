#include <msp430.h>

typedef unsigned int  uint;
typedef unsigned char uchar;

/********************************************
函数名称：Init_ADC
功    能：初始化ADC
参    数：无
返回值  ：无
********************************************/
void Init_ADC(void)
{
    P6SEL |= 0x03;                            // 使能ADC通道 p6.0 p6.1
    ADC12CTL0 = ADC12ON+SHT0_15+MSC+REF2_5V+REFON;   // 打开ADC，设置采样时间
    ADC12CTL1 = ADC12DIV_6+SHP+CONSEQ_3;             // 使用采样定时器
    ADC12MCTL0 |= SREF_0+INCH_0;
    ADC12MCTL1 |= SREF_1 + INCH_1 | EOS  ;            // 第二路输入通道为 A1，设置为序列的最后一个转换
    ADC12IE = 0x0002;                           // 使能ADC中断
    ADC12CTL0 |= ENC;                         // 使能转换
    ADC12CTL0 |= ADC12SC;                     // 开始转换
}


///********************************************
//函数名称：Hex2Dec
//功    能：将16进制ADC转换数据变换成十进制
//          表示形式
//参    数：Hex_Val--16进制数据  
//          ptr--指向存放转换结果的指针
//返回值  ：无
//********************************************/
//void Hex2Dec(uint Hex_val,uchar *ptr)
//{
//    ptr[0] = Hex_val / 1000;        
//    ptr[1] = (Hex_val - ptr[0]*1000)/100;
//    ptr[2] = (Hex_val - ptr[0]*1000 - ptr[1]*100)/10;
//    ptr[3] = (Hex_val - ptr[0]*1000 - ptr[1]*100 - ptr[2]*10); 
//}   
///*******************************************
//函数名称：Trans_val_33
//功    能：将16进制ADC转换数据变换成三位10进制
//          真实的模拟电压数据
//参    数：Hex_Val--16进制数据   
//返回值  ：无
//********************************************/
//void Trans_val_33(uint Hex_Val,uchar *ptr)
//{
//    float caltmp;
//    uint Curr_Volt;
//    uchar t1;
//    
//    caltmp = (Hex_Val*3.3)/4095.0;           //caltmp = Hex_Val * 3.3/4096  
//    Curr_Volt = (100*caltmp+0.5);             //放大100倍四舍五入
//    ptr[0] = Curr_Volt / 100;                //Hex->Dec变换
//    t1 = Curr_Volt - (ptr[0] * 100);
//    ptr[1] = t1 / 10;
//    ptr[2] = t1 - (ptr[1] * 10);
//}
//
///*******************************************
//函数名称：Trans_val_25
//功    能：将16进制ADC转换数据变换成三位10进制
//          真实的模拟电压数据
//参    数：Hex_Val--16进制数据   
//返回值  ：无
//********************************************/
//void Trans_val_25(uint Hex_Val,uchar *ptr)
//{
//    float caltmp;
//    uint Curr_Volt;
//    uchar t1;
//    
//    caltmp = (Hex_Val*2.5)/4095.0;           //caltmp = Hex_Val * 2.3/4096  
//    Curr_Volt = (100*caltmp+0.5);             //放大100倍四舍五入
//    ptr[0] = Curr_Volt / 100;                //Hex->Dec变换
//    t1 = Curr_Volt - (ptr[0] * 100);
//    ptr[1] = t1 / 10;
//    ptr[2] = t1 - (ptr[1] * 10);
//}



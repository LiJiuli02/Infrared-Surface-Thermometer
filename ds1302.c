#include "ds1302.h"

typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long  ulong;

/*******************************************
函数名称：delay
功    能：延时一段时间
参    数：time--延时长度
返回值  ：无
********************************************/
void delay(uint time)
{
    uint i;
    for(i = 0;i < time;i++)     _NOP();
}
/*******************************************
函数名称：Reset_DS1302
功    能：对DS1302进行复位操作
参    数：无
返回值  ：无
********************************************/
void Reset_DS1302(void)
{
    DS_RST_OUT;  //RST对应的IO设置为输出状态
    DS_SCL_OUT;  //SCLK对应的IO设置为输出状态
    DS_SCL0;     //SCLK=0
    DS_RST0;     //RST=0
    delay(10);
    DS_SCL1;    //SCLK=1
}
/*******************************************
函数名称：Write1Byte
功    能：对DS1302写入1个字节的数据
参    数：wdata--写入的数据
返回值  ：无
********************************************/
void Write1Byte(uchar wdata) 
{ 
    uchar i;

    DS_SDA_OUT;     //SDA对应的IO设置为输出状态
    DS_RST1;        //REST=1;

    for(i = 8; i > 0; i--) 
    {
        if(wdata&0x01)  DS_SDA1;
        else            DS_SDA0;
        DS_SCL0;        //时钟模拟
        delay(10); 
        DS_SCL1;
        delay(10); 
        wdata >>= 1;    //位移，发送不同位
    } 
}
/*******************************************
函数名称：Read1Byte
功    能：从DS1302读出1个字节的数据
参    数：无
返回值  ：读出的一个字节数据
********************************************/
uchar Read1Byte(void) 
{ 
    uchar i;
    uchar rdata = 0X00;

    DS_SDA_IN;  //SDA对应的IO设置为输入状态
    DS_RST1;    //REST=1;

    for(i = 8; i > 0; i--)
    {
        DS_SCL1;
        delay(10);
        DS_SCL0;
        delay(10);
        rdata >>= 1;
        if(DS_SDA_BIT)  rdata |= 0x80;  //如果P2IN & BIT6为1（即P2.6输入为1），则存1
    } 
    
    return(rdata); 
}
/*******************************************
函数名称：W_Data
功    能：向某个寄存器写入一个字节数据
参    数：addr--寄存器地址
          wdata--写入的数据
返回值  ：无
********************************************/
void W_Data(uchar addr, uchar wdata)
{
    DS_RST0; 
    DS_SCL0;   
    _NOP();
    DS_RST1;
    Write1Byte(addr);   //写入地址
    Write1Byte(wdata);  //写入数据
    DS_SCL1;
    DS_RST0;
} 
/*******************************************
函数名称：R_Data
功    能：从某个寄存器读出一个字节数据
参    数：addr--寄存器地址
返回值  ：读出的数据
********************************************/
uchar R_Data(uchar addr)
{
    uchar rdata;
    
    DS_RST0; 
    DS_SCL0; 
    _NOP();
    DS_RST1;
    Write1Byte(addr);    //写入地址
    rdata = Read1Byte();  //读出数据 
    DS_SCL1;
    DS_RST0;
    
    return(rdata);
}
/*******************************************
函数名称：Set_DS1302
功    能：设置DS1302内部的时间
参    数：ptr--指向存放数据地址的指针
返回值  ：无
说明    ：写入数据的格式：
            秒 分 时 日 月 星期 年  【共7个字节】
********************************************/
void Set_DS1302(uchar *ptr) 
{
    uchar i;
    uchar addr = 0x80; 
    
    W_Data(0x8e,0x00);    //允许写入
    
    for(i = 7;i > 0;i--)
    { 
        W_Data(addr,*ptr++); 
        addr += 2;
    }
    W_Data(0x8e,0x80);      //禁止
}
/******************************************************************** 
*
* 名称: Get_DS1302
* 说明: 
* 功能: 读取DS1302当前时间
* 调用: R_Data(uchar addr) 
* 输入: ucCurtime: 保存当前时间地址。当前时间格式为: 秒 分 时 日 月 星期 年 
* 7Byte (BCD码) 1B 1B 1B 1B 1B 1B 1B
* 返回值: 无
***********************************************************************/
/*******************************************
函数名称：Get_DS1302
功    能：读取DS1302内部的时间
参    数：ptr--指向数据存放地址的指针
返回值  ：无
说明    ：读出数据的格式：
            秒 分 时 日 月 星期 年  【共7个字节】
********************************************/
void Get_DS1302(uchar *ptr) 
{
    uchar i;
    uchar addr = 0x81;
    
    uchar BCDbuf[7];
    //读出数据的格式： 秒 分 时 日 月 星期 年  【共7个字节】
    for(i = 0;i < 7;i++)
    {
        BCDbuf[i] = R_Data(addr);
        addr += 2;
    }
    
    for( i=0; i<3; i++ )
    {
      ptr[i+3] = 10*((BCDbuf[2-i]&0xf0)>>4) +(BCDbuf[2-i]&0x0f) ;  //时分秒
    }
    
    ptr[0] = 10*((BCDbuf[6]&0xf0)>>4)+(BCDbuf[6]&0x0f);       //年
    
    for( i=0; i<2; i++ )
    {
      ptr[i+1] = 10*((BCDbuf[4-i]&0xf0)>>4)+(BCDbuf[4-i]&0x0f);     //月日
    }
}

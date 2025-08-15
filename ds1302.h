#ifndef _DS1302_H_
#define _DS1302_H_
#include <msp430x24x.h>

/**************∫Í∂®“Â***************/
#define DS_RST  BIT7        //DS_RST = P2.7 
#define DS_SCL  BIT5        //DS_SCL = P2.5 
#define DS_SDA  BIT6        //DS_SDA = P2.6 
#define DS_RST_IN P2DIR &= ~DS_RST
#define DS_RST_OUT P2DIR |= DS_RST
#define DS_RST0 P2OUT &= ~DS_RST
#define DS_RST1 P2OUT |= DS_RST
#define DS_SCL_IN P2DIR &= ~DS_SCL
#define DS_SCL_OUT P2DIR |= DS_SCL
#define DS_SCL0 P2OUT &= ~DS_SCL
#define DS_SCL1 P2OUT |= DS_SCL
#define DS_SDA_IN P2DIR &= ~DS_SDA
#define DS_SDA_OUT P2DIR |= DS_SDA
#define DS_SDA0 P2OUT &= ~DS_SDA
#define DS_SDA1 P2OUT |= DS_SDA
#define DS_SDA_BIT P2IN & DS_SDA

void delay(unsigned int time);
void Reset_DS1302(void);
void Write1Byte(unsigned char wdata);
unsigned char Read1Byte(void);
void W_Data(unsigned char addr, unsigned char wdata);
unsigned char R_Data(unsigned char addr);
void Set_DS1302(unsigned char *ptr);
void Get_DS1302(unsigned char *ptr);

#endif



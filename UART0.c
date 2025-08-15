#include <msp430x24x.h>

typedef unsigned char uchar;

/*******************************************
�������ƣ�InitUART
��    �ܣ���ʼ��UART�˿�
��    ������
����ֵ  ����
********************************************/
void InitUART(void)
{
    P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 0x41;                            // 8MHz 9600
    UCA0BR1 = 0x03;                              // 8MHz 9600
    UCA0MCTL = UCBRS1;                        // Modulation UCBRSx = 2
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

}
/*******************************************
�������ƣ�Send1Char
��    �ܣ���PC������һ���ַ�
��    ����sendchar--Ҫ���͵��ַ�
����ֵ  ����
********************************************/
void Send1Char(uchar sendchar)
{
      while (!(IFG2 & UCA0TXIFG));    //�ȴ����ͼĴ���Ϊ��         
      UCA0TXBUF = sendchar; 
   
}

/*******************************************
�������ƣ�PutSting
��    �ܣ���PC�������ַ���
��    ����ptr--ָ�����ַ�����ָ��
����ֵ  ����
********************************************/
void PutString(char *ptr)
{
      while(*ptr != '\0')
      {
            Send1Char(*ptr++);                     // ��������
      }
}


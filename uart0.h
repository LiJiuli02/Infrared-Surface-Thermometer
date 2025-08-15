void InitUART(void);
void Send1Char(unsigned char sendchar);
void PutString(char *ptr);

#define CPU          (8000000)
#define delay_us(x)   (__delay_cycles((double)x*CPU/1000000.0))
#define delay_ms(x)   (__delay_cycles((double)x*CPU/1000.0))

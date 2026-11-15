/* LPC2148 ADC0.1 (P0.28) readout through UART0, 9600 bps */
#define PLL0CON  (*(volatile unsigned long *)0xE01FC080)
#define PLL0CFG  (*(volatile unsigned long *)0xE01FC084)
#define PLL0STAT (*(volatile unsigned long *)0xE01FC088)
#define PLL0FEED (*(volatile unsigned long *)0xE01FC08C)
#define VPBDIV   (*(volatile unsigned long *)0xE01FC100)
#define PINSEL0  (*(volatile unsigned long *)0xE002C000)
#define PINSEL1  (*(volatile unsigned long *)0xE002C004)
#define U0THR    (*(volatile unsigned long *)0xE000C000)
#define U0DLL    (*(volatile unsigned long *)0xE000C000)
#define U0DLM    (*(volatile unsigned long *)0xE000C004)
#define U0FDR    (*(volatile unsigned long *)0xE000C028)
#define U0LCR    (*(volatile unsigned long *)0xE000C00C)
#define U0LSR    (*(volatile unsigned long *)0xE000C014)
#define AD0CR    (*(volatile unsigned long *)0xE0034000)
#define AD0GDR   (*(volatile unsigned long *)0xE0034004)

void pll_init(void)
{
    PLL0CON = 0x01;
    PLL0CFG = 0x24;          /* 12 MHz crystal -> 60 MHz CCLK */
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;
    while((PLL0STAT & (1UL<<10)) == 0);
    PLL0CON = 0x03;
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;
    VPBDIV = 0x00;           /* PCLK = CCLK/4 = 15 MHz */
}

void uart_init(void)
{
    PINSEL0 = (PINSEL0 & ~0x0FUL) | 0x05UL;
    U0LCR = 0x83;
    U0DLM = 0;
    U0DLL = 98;              /* 15 MHz PCLK, approx. 9600 baud */
    U0FDR = 0x10;
    U0LCR = 0x03;
}

void tx_char(char c)
{
    while((U0LSR & 0x20UL) == 0);
    U0THR = (unsigned char)c;
}

void tx_string(char *s)
{
    while(*s)
        tx_char(*s++);
}

void tx_num(unsigned int n)
{
    char b[5];
    unsigned char i = 0;
    if(n == 0)
    {
        tx_char('0');
        return;
    }
    while(n > 0)
    {
        b[i++] = (char)('0' + (n % 10));
        n = n / 10;
    }
    while(i > 0)
        tx_char(b[--i]);
}

void delay(void)
{
    volatile unsigned long i;
    for(i=0; i<300000; i++);
}

int main(void)
{
    unsigned long result;
    unsigned int count, mv;
    pll_init();
    uart_init();
    PINSEL1 = (PINSEL1 & ~(3UL<<24)) | (1UL<<24); /* P0.28 = AD0.1 */
    AD0CR = (1UL<<1) | (3UL<<8) | (1UL<<21);    /* CH1, CLKDIV=3, ADC ON */
    tx_string("ADC0.1 Count and Voltage\r\n");
    while(1)
    {
        AD0CR &= ~(7UL<<24);
        AD0CR |= (1UL<<24);                     /* Start conversion */
        do { result = AD0GDR; } while((result & (1UL<<31)) == 0);
        count = (unsigned int)((result >> 6) & 0x3FFUL);
        mv = (unsigned int)((count * 3300UL) / 1023UL);
        tx_string("Count = ");
        tx_num(count);
        tx_string("  Voltage = ");
        tx_num(mv / 1000);
        tx_char('.');
        if((mv % 1000) < 100) tx_char('0');
        if((mv % 1000) < 10) tx_char('0');
        tx_num(mv % 1000);
        tx_string(" V\r\n");
        delay();
    }
}

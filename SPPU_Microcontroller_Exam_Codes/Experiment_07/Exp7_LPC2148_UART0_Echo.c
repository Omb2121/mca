/* LPC2148 UART0 Echo: P0.0=TXD0, P0.1=RXD0, 9600 bps, 8-N-1 */
#define PLL0CON  (*(volatile unsigned long *)0xE01FC080)
#define PLL0CFG  (*(volatile unsigned long *)0xE01FC084)
#define PLL0STAT (*(volatile unsigned long *)0xE01FC088)
#define PLL0FEED (*(volatile unsigned long *)0xE01FC08C)
#define VPBDIV   (*(volatile unsigned long *)0xE01FC100)
#define PINSEL0  (*(volatile unsigned long *)0xE002C000)
#define U0RBR    (*(volatile unsigned long *)0xE000C000)
#define U0THR    (*(volatile unsigned long *)0xE000C000)
#define U0DLL    (*(volatile unsigned long *)0xE000C000)
#define U0DLM    (*(volatile unsigned long *)0xE000C004)
#define U0FDR    (*(volatile unsigned long *)0xE000C028)
#define U0LCR    (*(volatile unsigned long *)0xE000C00C)
#define U0LSR    (*(volatile unsigned long *)0xE000C014)

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
    VPBDIV = 0x01;           /* PCLK = CCLK = 60 MHz */
}

int main(void)
{
    unsigned char ch;
    pll_init();
    PINSEL0 = (PINSEL0 & ~0x0FUL) | 0x05UL;
    U0LCR = 0x83;             /* Enable divisor latch, 8-bit data */
    U0DLM = 0x01;
    U0DLL = 0x87;             /* Divisor 391 for approx. 9600 baud */
    U0FDR = 0x10;
    U0LCR = 0x03;             /* 8 data bits, no parity, 1 stop bit */
    while(1)
    {
        while((U0LSR & 0x01UL) == 0);
        ch = (unsigned char)U0RBR;
        while((U0LSR & 0x20UL) == 0);
        U0THR = ch;
    }
}

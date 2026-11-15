/* LPC2148 GSM call using UART0; replace PHONE with your SIM destination */
#define FOSC     12000000UL
#define PHONE    "0000000000"
#define PINSEL0  (*(volatile unsigned long *)0xE002C000)
#define IO0DIR   (*(volatile unsigned long *)0xE0028008)
#define IO0SET   (*(volatile unsigned long *)0xE0028004)
#define IO0CLR   (*(volatile unsigned long *)0xE002800C)
#define VPBDIV   (*(volatile unsigned long *)0xE01FC100)
#define U0RBR    (*(volatile unsigned long *)0xE000C000)
#define U0THR    (*(volatile unsigned long *)0xE000C000)
#define U0DLL    (*(volatile unsigned long *)0xE000C000)
#define U0DLM    (*(volatile unsigned long *)0xE000C004)
#define U0FCR    (*(volatile unsigned long *)0xE000C008)
#define U0LCR    (*(volatile unsigned long *)0xE000C00C)
#define U0LSR    (*(volatile unsigned long *)0xE000C014)
#define U0FDR    (*(volatile unsigned long *)0xE000C028)
#define U0TER    (*(volatile unsigned long *)0xE000C030)
#define RS       (1UL<<5)
#define RW       (1UL<<6)
#define EN       (1UL<<7)
#define LCDDATA  0x0000FF00UL

void delay(unsigned int n)
{
    volatile unsigned int i, j;
    for(i=0; i<n; i++) for(j=0; j<1000; j++);
}

void lcd_cmd(unsigned char c)
{
    IO0CLR = LCDDATA | RS | RW | EN;
    IO0SET = ((unsigned long)c << 8);
    delay(1);
    IO0SET = EN;
    delay(1);
    IO0CLR = EN;
    delay(2);
}

void lcd_data(unsigned char c)
{
    IO0CLR = LCDDATA | RW | EN;
    IO0SET = ((unsigned long)c << 8) | RS;
    delay(1);
    IO0SET = EN;
    delay(1);
    IO0CLR = EN;
    delay(2);
}

void lcd_string(char *s)
{
    while(*s) lcd_data((unsigned char)*s++);
}

void lcd_init(void)
{
    IO0DIR |= 0x0000FFE0UL;
    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
}

void uart_init(void)
{
    unsigned long d;
    VPBDIV = 0x01;           /* PCLK = 12 MHz */
    PINSEL0 = (PINSEL0 & ~0x0FUL) | 0x05UL;
    d = FOSC / (16UL * 9600UL);
    U0LCR = 0x83;
    U0DLM = (d >> 8) & 0xFF;
    U0DLL = d & 0xFF;
    U0FDR = 0x10;
    U0LCR = 0x03;
    U0FCR = 0x07;
    U0TER = 0x80;
}

void uart_send(char c)
{
    while((U0LSR & 0x20UL) == 0);
    U0THR = (unsigned char)c;
}

void uart_string(char *s)
{
    while(*s) uart_send(*s++);
}

unsigned char gsm_ok(void)
{
    char c, prev = 0;
    unsigned long t = 20000000UL;
    while(t--)
    {
        if(U0LSR & 0x01UL)
        {
            c = (char)U0RBR;
            if(prev == 'O' && c == 'K') return 1;
            prev = c;
        }
    }
    return 0;
}

int main(void)
{
    PINSEL0 = (PINSEL0 & ~0x0FUL) | 0x05UL;
    IO0DIR |= 0x0000FFE0UL;
    lcd_init();
    uart_init();
    lcd_cmd(0x80);
    lcd_string("GSM Testing");
    delay(5000);              /* Allow modem time to be ready */
    uart_string("AT\r");
    if(gsm_ok())
    {
        lcd_cmd(0x01);
        lcd_cmd(0x80);
        lcd_string("GSM OK");
        uart_string("ATD" PHONE ";\r");
        delay(500);
        lcd_cmd(0xC0);
        if(gsm_ok())
            lcd_string("Dial Cmd Sent");
        else
            lcd_string("Dial No Reply");
    }
    else
    {
        lcd_cmd(0xC0);
        lcd_string("GSM Test Fail");
    }
    while(1);
}

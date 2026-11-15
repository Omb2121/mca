/* LPC2148 LCD: RS=P0.4, EN=P0.5, RW=P0.6, D0-D7=P0.8-P0.15 */
#define PINSEL0 (*(volatile unsigned long *)0xE002C000)
#define IO0DIR  (*(volatile unsigned long *)0xE0028008)
#define IO0SET  (*(volatile unsigned long *)0xE0028004)
#define IO0CLR  (*(volatile unsigned long *)0xE002800C)
#define DATA    0x0000FF00UL
#define RS      (1UL<<4)
#define EN      (1UL<<5)
#define RW      (1UL<<6)

void delay(unsigned int n)
{
    volatile unsigned int i, j;
    for(i=0; i<n; i++)
        for(j=0; j<6000; j++);
}

void lcd_cmd(unsigned char c)
{
    IO0CLR = DATA | RS | RW;
    IO0SET = ((unsigned long)c << 8);
    IO0SET = EN;
    delay(1);
    IO0CLR = EN;
    delay(2);
}

void lcd_data(unsigned char d)
{
    IO0CLR = DATA | RW;
    IO0SET = ((unsigned long)d << 8) | RS;
    IO0SET = EN;
    delay(1);
    IO0CLR = EN;
    delay(2);
}

void lcd_string(char *s)
{
    while(*s)
        lcd_data((unsigned char)*s++);
}

int main(void)
{
    PINSEL0 &= ~0xFFFF3F00UL;  /* Select GPIO for P0.4-P0.6, P0.8-P0.15 */
    IO0DIR |= 0x0000FF70UL;
    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_string("NMIET PUNE");
    while(1);
}

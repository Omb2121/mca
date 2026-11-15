/* LPC2148: LEDs connected to P0.8-P0.15 */
#define PINSEL0 (*(volatile unsigned long *)0xE002C000)
#define IO0DIR  (*(volatile unsigned long *)0xE0028008)
#define IO0SET  (*(volatile unsigned long *)0xE0028004)
#define IO0CLR  (*(volatile unsigned long *)0xE002800C)
#define LEDS    0x0000FF00UL

void delay(void)
{
    volatile unsigned long i;
    for(i=0; i<500000; i++);
}

int main(void)
{
    PINSEL0 &= 0x0000FFFFUL;   /* P0.8-P0.15 as GPIO */
    IO0DIR |= LEDS;
    while(1)
    {
        IO0SET = LEDS;
        delay();
        IO0CLR = LEDS;
        delay();
    }
}

#include <reg51.h>

unsigned char code step[4] = {0x09,0x0C,0x06,0x03};

void delay_50ms(void)
{
    TMOD = 0x01;       /* Timer 0, Mode 1 */
    TH0 = 0x4C;
    TL0 = 0x00;
    TF0 = 0;
    TR0 = 1;
    while(TF0 == 0);
    TR0 = 0;
    TF0 = 0;
}

void main(void)
{
    unsigned char i;
    while(1)
    {
        for(i=0; i<4; i++)
        {
            P2 = step[i];
            delay_50ms();
        }
    }
}

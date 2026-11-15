#include <reg51.h>

unsigned char code seg[6] = {0x77,0x7C,0x39,0x5E,0x79,0x71};

void delay(void)
{
    unsigned int i, j;
    for(i=0; i<200; i++)
        for(j=0; j<1275; j++);
}

void main(void)
{
    unsigned char i;
    while(1)
    {
        for(i=0; i<6; i++)
        {
            P2 = seg[i];
            delay();
        }
    }
}

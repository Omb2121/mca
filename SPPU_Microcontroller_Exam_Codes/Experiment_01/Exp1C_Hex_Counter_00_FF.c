#include <reg51.h>

void delay(void)
{
    unsigned int i, j;
    for(i=0; i<300; i++)
        for(j=0; j<1275; j++);
}

void main(void)
{
    unsigned char count = 0x00;
    while(1)
    {
        P2 = count;
        delay();
        count++;
    }
}

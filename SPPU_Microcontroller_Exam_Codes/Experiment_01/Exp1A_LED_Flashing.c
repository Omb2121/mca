#include <reg51.h>

unsigned char code pattern[6] = {0x00,0xFF,0x0F,0xF0,0x55,0xAA};

void delay(void)
{
    unsigned int i, j;
    for(i=0; i<300; i++)
        for(j=0; j<1275; j++);
}

void main(void)
{
    unsigned char i;
    while(1)
    {
        for(i=0; i<6; i++)
        {
            P2 = pattern[i];
            delay();
        }
    }
}

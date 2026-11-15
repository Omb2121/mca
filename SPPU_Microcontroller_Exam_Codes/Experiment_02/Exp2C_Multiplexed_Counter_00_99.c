#include <reg51.h>

/* Assumption: P2 = segments a-g,dp; P3.0 = units select;
   P3.1 = tens select. Common-cathode, active-HIGH selects. */
unsigned char code seg[10] =
{0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void short_delay(void)
{
    unsigned int i;
    for(i=0; i<250; i++);
}

void main(void)
{
    unsigned char n, r;
    while(1)
    {
        for(n=0; n<100; n++)
        {
            for(r=0; r<100; r++)
            {
                P3 = (P3 & 0xFC) | 0x02;   /* Select tens digit */
                P2 = seg[n/10];
                short_delay();
                P3 = P3 & 0xFC;

                P3 = (P3 & 0xFC) | 0x01;   /* Select units digit */
                P2 = seg[n%10];
                short_delay();
                P3 = P3 & 0xFC;
            }
        }
    }
}

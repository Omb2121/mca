/* LPC2148 + 24C02 EEPROM: write/read "ABCDEFGHIJ" and show on 16x2 LCD
   I2C0: P0.2=SCL0, P0.3=SDA0; EEPROM address A0/A1 (A0-A2 grounded)
   LCD: RS=P0.5, RW=P0.6, EN=P0.7, D0-D7=P0.8-P0.15 */
#define PINSEL0   (*(volatile unsigned long *)0xE002C000)
#define IO0DIR    (*(volatile unsigned long *)0xE0028008)
#define IO0SET    (*(volatile unsigned long *)0xE0028004)
#define IO0CLR    (*(volatile unsigned long *)0xE002800C)
#define I2C0CONSET (*(volatile unsigned long *)0xE001C000)
#define I2C0STAT   (*(volatile unsigned long *)0xE001C004)
#define I2C0DAT    (*(volatile unsigned long *)0xE001C008)
#define I2C0SCLH   (*(volatile unsigned long *)0xE001C010)
#define I2C0SCLL   (*(volatile unsigned long *)0xE001C014)
#define I2C0CONCLR (*(volatile unsigned long *)0xE001C018)
#define DATA       0x0000FF00UL
#define RS         (1UL<<5)
#define RW         (1UL<<6)
#define EN         (1UL<<7)
#define EEPROM_W   0xA0
#define EEPROM_R   0xA1

void delay(unsigned int n)
{
    volatile unsigned int i;
    for(i=0; i<n; i++);
}

void lcd_cmd(unsigned char c)
{
    IO0CLR = DATA | RS | RW | EN;
    IO0SET = ((unsigned long)c<<8);
    delay(500);
    IO0SET = EN;
    delay(500);
    IO0CLR = EN;
    delay(500);
}

void lcd_data(unsigned char c)
{
    IO0CLR = DATA | RW | EN;
    IO0SET = ((unsigned long)c<<8) | RS;
    delay(500);
    IO0SET = EN;
    delay(500);
    IO0CLR = EN;
    delay(500);
}

void lcd_string(char *s)
{
    while(*s) lcd_data((unsigned char)*s++);
}

void lcd_init(void)
{
    IO0DIR |= 0x0000FFE0UL;
    delay(300000);
    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
}

unsigned char wait_i2c(void)
{
    unsigned long t = 100000UL;
    while(t--)
        if(I2C0CONSET & 0x08UL) return 1;
    return 0;
}

unsigned char start_i2c(void)
{
    I2C0CONSET = 0x60;       /* I2C enable + START */
    I2C0CONCLR = 0x08;       /* Clear SI */
    if(!wait_i2c()) return 0;
    if(I2C0STAT != 0x08 && I2C0STAT != 0x10) return 0;
    return 1;
}

unsigned char send_i2c(unsigned char d, unsigned char status)
{
    I2C0DAT = d;
    I2C0CONCLR = 0x28;       /* Clear START and SI */
    if(!wait_i2c()) return 0;
    if(I2C0STAT != status) return 0;
    return 1;
}

void stop_i2c(void)
{
    I2C0CONSET = 0x50;       /* STOP + I2C enable */
    I2C0CONCLR = 0x08;
    delay(5000);
}

void i2c_init(void)
{
    PINSEL0 = (PINSEL0 & ~0xF0UL) | 0x50UL;  /* P0.2=SCL0, P0.3=SDA0 */
    I2C0CONCLR = 0x6C;
    I2C0SCLH = 60;           /* 100 kHz at 12 MHz PCLK */
    I2C0SCLL = 60;
    I2C0CONSET = 0x40;
}

unsigned char eeprom_write_byte(unsigned char addr, unsigned char d)
{
    if(!start_i2c()) { stop_i2c(); return 0; }
    if(!send_i2c(EEPROM_W,0x18)) { stop_i2c(); return 0; }
    if(!send_i2c(addr,0x28)) { stop_i2c(); return 0; }
    if(!send_i2c(d,0x28)) { stop_i2c(); return 0; }
    stop_i2c();
    delay(100000);           /* Allow EEPROM internal write cycle */
    return 1;
}

unsigned char eeprom_read_byte(unsigned char addr, unsigned char *d)
{
    if(!start_i2c()) { stop_i2c(); return 0; }
    if(!send_i2c(EEPROM_W,0x18)) { stop_i2c(); return 0; }
    if(!send_i2c(addr,0x28)) { stop_i2c(); return 0; }

    I2C0CONSET = 0x20;        /* Repeated START */
    I2C0CONCLR = 0x08;
    if(!wait_i2c() || I2C0STAT != 0x10) { stop_i2c(); return 0; }
    if(!send_i2c(EEPROM_R,0x40)) { stop_i2c(); return 0; }

    I2C0CONCLR = 0x08;        /* AA=0: receive one byte with NACK */
    if(!wait_i2c() || I2C0STAT != 0x58) { stop_i2c(); return 0; }
    *d = (unsigned char)I2C0DAT;
    stop_i2c();
    return 1;
}

int main(void)
{
    unsigned char i, ch;
    unsigned char text[10] = {'A','B','C','D','E','F','G','H','I','J'};
    unsigned char ok = 1;

    lcd_init();
    i2c_init();
    for(i=0; i<10; i++)
        if(!eeprom_write_byte(i,text[i])) ok = 0;

    lcd_cmd(0x80);
    if(ok) lcd_string("Write Success");
    else   lcd_string("Write Failed");

    lcd_cmd(0xC0);
    if(ok)
    {
        for(i=0; i<10; i++)
        {
            if(eeprom_read_byte(i,&ch)) lcd_data(ch);
            else lcd_string("ERR");
        }
    }
    while(1);
}

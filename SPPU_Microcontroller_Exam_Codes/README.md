# SPPU Microcontroller Practical Exam Codes

Separate, short source files prepared from the numbered experiment write-ups in `MCA Upto Expt 8.pdf` and the supplied `Expt 9 & 10.docx`.

## Experiment index

### Experiment 1 – 8051 LED interfacing
- `Exp1A_LED_Flashing.c`
- `Exp1B_BCD_Counter_00_99.c`
- `Exp1C_Hex_Counter_00_FF.c`
- `Exp1D_Binary_Counter_00_FF.c`

### Experiment 2 – 8051 seven-segment display
- `Exp2A_Seven_Segment_Digits_0_9.c`
- `Exp2B_Seven_Segment_Characters_A_F.c`
- `Exp2C_Multiplexed_Counter_00_99.c`

### Experiment 3 – 8051 DAC waveform generation
- `Exp3A_DAC_Square_Wave.c`
- `Exp3B_DAC_Triangular_Wave.c`
- `Exp3C_DAC_Sine_Wave.c`

### Experiment 4 – 8051 stepper motor using Timer 0
- `Exp4A_Stepper_Clockwise_Timer0.c`
- `Exp4B_Stepper_Anticlockwise_Timer0.c`

### Experiment 5 – LPC2148 LED blinking
- `Exp5_LPC2148_LED_Blinking.c`

### Experiment 6 – LPC2148 16x2 LCD
- `Exp6_LPC2148_LCD_Display.c`

### Experiment 7 – LPC2148 UART0 echo
- `Exp7_LPC2148_UART0_Echo.c`

### Experiment 8 – LPC2148 ADC0.1 with UART0
- `Exp8_LPC2148_ADC0_1_UART.c`

### Experiment 9 – LPC2148 GSM calling using UART0
- `Exp9_LPC2148_GSM_Call.c`

### Experiment 10 – LPC2148 I2C EEPROM read/write
- `Exp10_LPC2148_I2C_EEPROM.c`

## Assumptions and important notes

1. Experiments 1–4 use Keil C51 and `reg51.h` for AT89C51/AT89S52. Experiments 5–10 use direct LPC2148 memory-mapped register access and are intended for the LPC2148/Triton ARM compiler environment.
2. Experiment 1 uses Port 2 for the LED bank as specified in its code section.
3. Experiment 2 uses common-cathode segment patterns. The original manual's body describes a two-digit multiplexed counter but its shown program section separately shows a single-digit counter and character display. The two-digit file is supplied as an exam-use implementation with P2 for segments and P3.1/P3.0 as active-HIGH tens/units digit selects. Confirm these two digit-select connections on the trainer kit before running it.
4. Experiment 3 follows the manual's program section and writes DAC samples to Port 1. The manual's algorithm has a conflicting Port 2 mention; use Port 1 as per its listed code and check the DAC board wiring.
5. Experiment 4 uses the four-step sequence from the manual's stepper table (09H, 0CH, 06H, 03H) and Timer 0 Mode 1 with the documented 11.0592 MHz preload (TH0=4CH, TL0=00H). Use a ULN2003 or suitable motor driver and a common ground. The original source-code snippet uses a different one-coil pattern; the table sequence has been followed here.
6. Experiment 5: LEDs are connected to P0.8–P0.15; the output polarity depends on the trainer-board LED wiring.
7. Experiment 6: LCD mapping is RS=P0.4, EN=P0.5, RW=P0.6, D0–D7=P0.8–P0.15, as described in the experiment body.
8. Experiment 7 assumes a 12 MHz crystal, PLL0=60 MHz and PCLK=60 MHz. UART0 uses P0.0/P0.1 and divisor 391 for approximately 9600 baud.
9. Experiment 8 assumes a 12 MHz crystal, PLL0=60 MHz and PCLK=15 MHz (CCLK/4). ADC0.1 is on P0.28 and UART0 is on P0.0/P0.1. The ADC reference is assumed to be 3.3 V.
10. Experiment 9: replace `PHONE` in the source with the destination number. UART0 pins are P0.0/P0.1; the LCD mapping follows the DOCX. The message `Dial Cmd Sent` means the modem accepted the ATD command; it does not prove the remote party answered.
11. Experiment 10 assumes a 24C02-compatible EEPROM at 7-bit address 0x50 (write byte A0H, read byte A1H), A0–A2 grounded, 12 MHz PCLK, I2C0 at P0.2/P0.3 and 100 kHz. Use pull-ups on SDA/SCL and ensure common ground. The LCD pin mapping follows the DOCX.

## Source/manual inconsistencies

- The index on page 7 of `MCA Upto Expt 8.pdf` lists older/different experiment titles for some later numbers. The actual numbered experiment write-ups later in the PDF are used for Experiments 1–8.
- The PDF's Experiment 2 aim describes a multiplexed two-digit counter, while its program section includes single-digit numeric and character displays. All are provided separately; the two-digit implementation states its assumed digit-select pins.
- The PDF's Experiment 3 algorithm mentions Port 2 at one step, while its waveform code sends samples to Port 1. The code follows the Port 1 program listing.
- The PDF's Experiment 4 stepper table uses 09H, 0CH, 06H, 03H but its sample source uses 01H, 02H, 04H, 08H. This package follows the table sequence.
- The separate DOCX labels Experiment 9 as GSM calling and Experiment 10 as I2C EEPROM. Those DOCX titles are used even though the earlier PDF index shows GPS/RTC for those numbers.

## Build note

These files have been reviewed for source-level consistency and host-side C syntax using small compatibility definitions for 8051 SFRs. A real Keil C51 / LPC214x ARM build and board-level test have not been performed in this environment. Timer delays, LCD contrast, active LED polarity, digit-select polarity, UART peripheral clock, EEPROM timing and physical pin mapping should be confirmed on the actual trainer kit.

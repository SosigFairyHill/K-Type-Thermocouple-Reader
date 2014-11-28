/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
//#include <string.h>

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();
    select_chip(1); // Select the ADT7320

    SSPBUF = 0b00001000; // Command byte indicating a write
    buf_wait();
    SSPBUF = 0b00000000; // Write to the ADT7320 config register
    buf_wait();

    select_chip(2); // Select the AD7793 ADC

    SSPBUF = 0b00001000; // Command byte indicating a write to the
    buf_wait();          // mode register
    SSPBUF = 0b00000000; // First 8 bits of the 16 bit mode register
    buf_wait();
    SSPBUF = 0b11000001; // Second 8 bits of the mode register
    buf_wait();

    select_chip(1);
    
    SSPBUF = 0b01010000; // Command byte to read temperature
    buf_wait();
    int junc_bin = SSPBUF/1000;
    float junc_temp = 25;

    int loop_counter = 0;

    while(1)
    {
        PORTCbits.RC6 = 0b1;
        PORTCbits.RC7 = 0b1;
        PORTBbits.RB5 = 0b1;
        PORTBbits.RB4 = 0b1;
        PORTBbits.RB3 = 0b1;
        PORTBbits.RB2 = 0b1;
        PORTBbits.RB1 = 0b1;
        PORTBbits.RB0 = 0b1;

        PORTA = 0b11110111;

        for (int thermo_sel = 1; thermo_sel < 3; thermo_sel++)
        {
            select_chip(2);

            SSPBUF = 0b00010000; // Command byte indicating a write to the
            buf_wait();          // config register for the thermocouple
            SSPBUF = 0b01000110; // First 8 bits of the 16 bit config register
            buf_wait();          // for the thermocouple
            switch (thermo_sel)
            {
                case 1:
                    SSPBUF = 0b10010000; // Second 8 bits for the first thermo
                    buf_wait();
                    break;
                case 2:
                    SSPBUF = 0b10010001; // Second 8 bits for the second thermo
                    buf_wait();
                    break;

            }

            int not_ready = 1;

            while (not_ready == 1)
            {
                SSPBUF = 0b01000000; // Command byte to read status
                buf_wait();
                not_ready = SSPBUF/10000000;
            }

            SSPBUF = 0b01011000; // Command byte to read from first thermo
            buf_wait();

            float thermo_v = 0;
            for (int i=0; i<3; i++)
            {
                //thermo_v += (binary_decimal(SSPBUF) * (16 - i * 8));
            }

            junc_temp = (binary_decimal(junc_bin))/16.0;

//            if (junc_temp/1000000000000) {junc_temp = (binary_decimal(junc_bin) - 8192)/16.0;}

            float tc = temp_voltage(junc_temp) + thermo_v;

            seven_segment(voltage_temp(tc), thermo_sel);
        }

        loop_counter++;

        if (loop_counter > 100)
        {
            loop_counter = 0;
            select_chip(1);
            SSPBUF = 0b01010000; // Command byte to read temperature
            buf_wait();
            junc_bin = SSPBUF;
        }

        char* str = display_number(216.98);
        char strval[5];
        for (int i=0; i<6; i++) {strval[i] = *(str+i);}
        char digit = strval[0];
        int val = 5;
        switch (digit) {
            case 0x30:
                val = 0;
                break;
            case 0x31:
                val = 1;
                break;
            case 0x32:
                val = 2;
                break;
        }
        int result = val;
        /* TODO <INSERT USER APPLICATION CODE HERE> */
    }

}


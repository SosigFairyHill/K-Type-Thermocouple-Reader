/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <stdio.h>
//#include <string.h>
#include <math.h>

#include "user.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */


void init_SPI(void)
{
    SSPSTATbits.CKE=0b0;
    SSPSTATbits.SMP=0b1;

    SSPCONbits.SSPEN = 0b1;
    SSPCONbits.CKP = 0b1;
    SSPCONbits.SSPM = 0b0000;
}

char* display_number(float number)
{
    static char str[((8 * sizeof(number)-1) / 3 + 2)];
    sprintf(str, "%05.1f", number);
    return str;
    
}

void select_chip(int chip_sel)
{
    if (chip_sel == 1)
    {
        PORTCbits.RC1 = 0;
        PORTCbits.RC2 = 1;
    }
    else if (chip_sel == 1)
    {
        PORTCbits.RC1 = 1;
        PORTCbits.RC2 = 0;
    }
    else
    {
        PORTCbits.RC1 = 1;
        PORTCbits.RC1 = 1;
    }
}

void buf_wait(void)
{
    while (!SSPSTATbits.BF);
}

void seven_segment(float number, int sel) // Write a number to the multiplexed
                                          // 4-digit 7-segment display. We have
                                          // 2 displays so the int sel will
                                          // select which one (1 or 2) we would
                                          // like to output to
{
    static char str[((8 * sizeof(number)-1) / 3 + 2)];
    sprintf(str, "%05.1f", number); // Parse the value into a four digit string
                                    // with one decimal place

    for (int digit = 0; digit<6; digit++) // Loop over digits of the number
    {
        PORTA = 0b11111111;
        switch (str[digit])                 // Set the leds for each number
        {
            case 0x30:
                if (digit == 0) {continue;} // Do not display the first digit
                                            // if it is zero

                else if (digit == 1 && str[0] == 0x30) {continue;} // Do not
                                            // display the first two digits
                                            // if they are both zero
                else                        // set the zero leds
                {
                    SEGMENT_A = 0b1;
                    SEGMENT_B = 0b1;
                    SEGMENT_C = 0b1;
                    SEGMENT_D = 0b1;
                    SEGMENT_E = 0b1;
                    SEGMENT_F = 0b1;
                    SEGMENT_G = 0b0;
                }
                
                break;
            case 0x31:
                SEGMENT_A = 0;
                SEGMENT_B = 1;
                SEGMENT_C = 1;
                SEGMENT_D = 0;
                SEGMENT_E = 0;
                SEGMENT_F = 0;
                SEGMENT_G = 0;
                break;
            case 0x32:
                SEGMENT_A = 1;
                SEGMENT_B = 1;
                SEGMENT_C = 0;
                SEGMENT_D = 1;
                SEGMENT_E = 1;
                SEGMENT_F = 0;
                SEGMENT_G = 1;
                break;
            case 0x33:
                SEGMENT_A = 1;
                SEGMENT_B = 1;
                SEGMENT_C = 1;
                SEGMENT_D = 1;
                SEGMENT_E = 0;
                SEGMENT_F = 0;
                SEGMENT_G = 1;
                break;
            case 0x34:
                SEGMENT_A = 0;
                SEGMENT_B = 1;
                SEGMENT_C = 1;
                SEGMENT_D = 0;
                SEGMENT_E = 0;
                SEGMENT_F = 1;
                SEGMENT_G = 1;
                break;
            case 0x35:
                SEGMENT_A = 1;
                SEGMENT_B = 0;
                SEGMENT_C = 1;
                SEGMENT_D = 1;
                SEGMENT_E = 0;
                SEGMENT_F = 1;
                SEGMENT_G = 1;
                break;
            case 0x36:
                SEGMENT_A = 1;
                SEGMENT_B = 0;
                SEGMENT_C = 1;
                SEGMENT_D = 1;
                SEGMENT_E = 1;
                SEGMENT_F = 1;
                SEGMENT_G = 1;
                break;
            case 0x37:
                SEGMENT_A = 1;
                SEGMENT_B = 1;
                SEGMENT_C = 1;
                SEGMENT_D = 1;
                SEGMENT_E = 1;
                SEGMENT_F = 1;
                SEGMENT_G = 0;
                break;
            case 0x38:
                SEGMENT_A = 1;
                SEGMENT_B = 1;
                SEGMENT_C = 1;
                SEGMENT_D = 1;
                SEGMENT_E = 1;
                SEGMENT_F = 1;
                SEGMENT_G = 1;
                break;
            case 0x39:
                SEGMENT_A = 1;
                SEGMENT_B = 1;
                SEGMENT_C = 1;
                SEGMENT_D = 0;
                SEGMENT_E = 0;
                SEGMENT_F = 1;
                SEGMENT_G = 1;
                break;
            case 0x2e:
//                SEGMENT_A = 1;
//                SEGMENT_B = 1;
//                SEGMENT_C = 1;
//                SEGMENT_D = 1;
//                SEGMENT_E = 1;
//                SEGMENT_F = 1;
//                SEGMENT_G = 0;
//                break;
                continue;
        }

        switch (sel)
        {
            case 1:
                switch(digit) // Drop the line corresponding to each digit low
                {
                    case 0:
                        PORTA = 0b11110111;
                        break;
                    case 1:
                        PORTA = 0b11111011;
                        break;
                    case 2:
                        PORTA = 0b11111101;
                        break;
                    case 3:
                        continue;   // The 4th digit will always be the decimal
                                    // point. For now, we will not use this
                    case 4:
                        PORTA = 0b11111110;
                        break;
                }
                break;

            case 2:
                switch(digit)
                {
                    case 0:
                        PORTA = 0b01111111;
                        break;
                    case 1:
                        PORTA = 0b10111111;
                        break;
                    case 2:
                        PORTA = 0b11011111;
                        break;
                    case 3:
                        continue;
                    case 4:
                        PORTA = 0b11101111;
                        break;
                }
        }
    }
}


void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */
    init_SPI();
    /* Setup analog functionality and port direction */
    TRISA = 0b00000000;

    TRISB = 0b00000011;

    TRISCbits.TRISC1 = 0b0;
    TRISCbits.TRISC2 = 0b0;
    TRISCbits.TRISC6 = 0b0;
    TRISCbits.TRISC7 = 0b0;
    
    TRISEbits.TRISE3 = 0b1;
    /* Initialize peripherals */

    /* Enable interrupts */
}

int binary_decimal(int n) /* Function to convert binary to decimal.*/
{
    int decimal=0, i=0, rem;
    while (n!=0)
    {
        rem = n%10;
        n/=10;
        decimal += rem * pow(2,i);
        ++i;
    }
    return decimal;
}

float temp_voltage(float temp)
{
//    float c_const[10] = {-1.7600413686E1,
//                        3.8921204975E1,
//                        1.8558770032E-2,
//                        -9.9457592874E-5,
//                        3.1870945719E-7,
//                        -5.6072844889E-10,
//                        5.6075059059E-13,
//                        -3.2020720003E-16,
//                        9.7151147152E-20,
//                        -1.2104721275E-23};
//
//    float microvolt = 1.185976E2 * exp(-1.183432E-4 * pow((temp - 126.9686),2));
//
//    for (int i = 0; i < 5; i++)
//    {
//        microvolt += c_const[i] * pow(temp, i);
//    }
//
////    float t0, v0, p1, p2, p3, p4, q1, q2, q3;
////    t0 = 1.0003453;
////    v0 = 25;
////    p1 = 4.0514854E-2;
////    p2 = -3.8789638E-5;
////    p3 = -2.8608478E-6;
////    p4 = -9.5367041E-10;
////    q1 = -1.3948675E-3;
////    q2 = -6.7976627E-5;
////
////    float volt_num = t0 + ((temp - v0) * (p1 + (temp - v0) * (p2 + (temp - v0) * (p3 - p4 * (temp - v0)))));
////    float volt_denom = (1 + (temp - v0) * (q1 - q2 * (temp - v0)));
////
////    float volt_num = 1;
//////    float volt_denom = 1;
////
////    float volt = volt_num / volt_denom;
//
//    return microvolt/1000;
}

float voltage_temp(float millivolt)
{
//    float c_const[8] = {2.5173462E-2,
//                        -1.1662878E-6,
//                        -1.0833638E-9,
//                        -8.9773540E-13,
//                        -3.7342377E-16,
//                        -8.6632643E-20,
//                        -1.0450598E-23,
//                        -5.1920577E-28};
//
//    float microvolt = millivolt * 1000;
//
//    float temp = 0;
//
//    for (int i = 0; i < 9; i++)
//    {
//        temp += c_const[i] * pow(microvolt, i + 1);
//    }
//    float t0, v0, p1, p2, p3, p4, q1, q2, q3;
//
//    if (voltage < -3.554 && voltage > -6.404)
//    {
//        t0 = -1.2147164E2;
//        v0 = -4.1790858;
//        p1 = 36.069513;
//        p2 = 30.722076;
//        p3 = 7.791386;
//        p4 = 0.52593991;
//        q1 = 0.93939547;
//        q2 = 0.27791285;
//        q3 = 0.025163349;
//    }
//    else
//    {
//        t0 = -8.7935962;
//        v0 = -0.34489914;
//        p1 = 25.678719;
//        p2 = -0.49887904;
//        p3 = -0.44705222;
//        p4 = -0.044869203;
//        q1 = 2.3893439E-4;
//        q2 = -0.02039775;
//        q3 = -1.8424107E-3;
//    }
//
//    //float temp_num = ((voltage - v0) * (p1 + (voltage - v0) * (p2 + (voltage - v0) * (p3 + p4 * (voltage - v0)))));
//    //float temp_denom = (1 + (voltage -v0) * (q1 + (voltage - v0) * (q2 + q3 * (voltage -v0))));
//    float temp_num = 1;
//    float temp_denom = 1;
//
//    float temp = t0 + 273.15 + temp_num / temp_denom;

   // return temp;
}
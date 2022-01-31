/*
 * Archivo: lab01
 * Autor: Pedro Luna
 * Creado: 24/01/2022
 * Última Modificación: 27/01/2022
 * 
 * Programa: Contador de 8 bits con botones, comparado con un contador en ADC
 * Hardware: LEDs en port A, botones y potenciometro en port B, 
 * displays en portc y transistores en port d
 */
// PIC16F887 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h> 
#include "adc_des.h"

// variables 
#define _XTAL_FREQ 4000000
#define _tmr0 237;
int banderas;
int var;
int nib1;
int nib2;


//funcion para el setup
void setup(void);

// tabla para los displays
/*char tabla[16] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 
0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111, 0b01110111, 0b01111100, 
0b00111001, 0b01011110, 0b01111001, 0b01110001};*/

// interrupciones

void __interrupt() isr (void)
{
    if (INTCONbits.T0IF == 1) //chequea la bandera del tmr0
    {
        adc_function();
    TMR0 = _tmr0; 
    INTCONbits.T0IF = 0;
    }
}

 
// Main
void main(void)
{
    adc_setup();
    setup();
    //loop principal
    while (1)
    {
        if (!PORTBbits.RB0)
        {
            while (!RB0);
            PORTA++;
            var = PORTA;
        }
        if (!PORTBbits.RB1)
        {
            while (!RB1);
            PORTA--;
            var = PORTA;
        }
        valores();
        
        if (ADCON0bits.GO == 0)
        {    
            ADCON0bits.CHS = 8;
            __delay_us(50);
            ADCON0bits.GO = 1;
        }
        
        if (var2 > var)
        {
            PORTEbits.RE2 = 1;
        }
        else 
        {
            PORTEbits.RE2 = 0;
        }
    }
}

//configuraciones en general
void setup(void)
{
    // config de las ins y outs
    ANSEL = 0b0100000000; //canal 8
    ANSELH = 0;
    
    TRISA = 0; //out
    TRISB = 0b0111; //ins
    TRISC = 0; //out
    TRISD = 0; //out
    TRISE = 0; // out
    
    OPTION_REGbits.nRBPU = 0;
    WPUB = 0b0011;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    
    // config de oscilador
    OSCCONbits.IRCF = 0b0110; // a 4MHz
    OSCCONbits.SCS = 1;
    
    //config rb
    INTCONbits.RBIE = 1;
    
    // config de tmr0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.T0SE = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    TMR0 = _tmr0;
    
   
    
    //interrupciones
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 1;        // Activar interrupciones de PuertoB
    INTCONbits.GIE = 1;
    
    return;
}
/*int valores(void)
{
    nib1 = var%0x10;
    nib2 = var/0x10;
}*/
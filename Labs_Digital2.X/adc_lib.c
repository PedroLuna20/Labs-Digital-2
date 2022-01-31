#include "adc_des.h"

void adc_setup(void)
{
    //config del ADC
    ADCON1bits.ADFM = 0; //justificado a la izquierda
    ADCON1bits.VCFG0 = 0; //voltaje como VDD
    ADCON1bits.VCFG1 = 0; //voltaje como VSS
    
    ADCON0bits.ADCS = 0b001; //conversion de reloj Fosc/8
    ADCON0bits.CHS = 0b1000; //canal analogico 8
    ADCON0bits.ADON = 1;
    __delay_us(50); //delay para dejar que cargue un capacitor
    
}

void adc_function(void)
{
    if (PIR1bits.ADIF) //chequea la bandera del ADC
    {
        if (ADCON0bits.CHS == 8){
            PORTD = ADRESH;
            var2 = PORTD;
        }
        PIR1bits.ADIF = 0;
    }
    valores();
    
    PORTE = 0; //clear al puerto E
    if (banderas == 0) //display 0
    {
        PORTEbits.RE1 = 0;
        PORTEbits.RE0 = 1;
        PORTC = (tabla[nib2]);
        banderas = 1;
    }
    else if (banderas == 1) //display 1
    {
        PORTEbits.RE0 = 0;
        PORTEbits.RE1 = 1;
        PORTC = (tabla[nib1]);
        banderas = 0;
    }
}
    
int valores(void)
{
    nib1 = var2%0x10;
    nib2 = var2/0x10;
}
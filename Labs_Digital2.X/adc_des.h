/* 
 * File: adc_des.h   
 * Author: Pedro Luna
 * Comments: librerias
 * Revision history: 25/01/2022
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef adc_H
#define	adc_H

#include <xc.h> // include processor files - each processor file is guarded.  
#define _XTAL_FREQ 4000000

int var2;
int banderas;
int nib1;
int nib2;

char tabla[16] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 
0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111, 0b01110111, 0b01111100, 
0b00111001, 0b01011110, 0b01111001, 0b01110001};

void adc_setup(void);
void adc_function(void);
int valores(void);

#endif	/* XC_HEADER_TEMPLATE_H */


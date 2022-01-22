/*
 * Archivo: lab0
 * Autor: Pedro Luna
 * Creado: 17/01/2022
 * Última Modificación: 17/01/2022
 * 
 * Programa: Contador de 8 bits con botones
 * Hardware: LEDs en port A, port D y port E, botones en port B y display en port C
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

// variables 
#define _tmr0 237; //variable para la interrucion de tmr0
int banderas;
int uni;
int dec;
int cent;
int var;
int var2;
int var3;
int resi;


//funcion para el setup
void setup(void);  //funcion para todas las configuraciones
int valores(void); //funcion para pasar los valores de binario a decimal

// tabla para los displays
char tabla[10] = {0b00111111, 0b00000110, 0b01011011, 0b01011011, 0b01100110, 
0b01101101, 0b01111101, 0b01001111, 0b01111111, 0b01101111};

// interrupciones
void __interrupt() isr (void)
{
    if (INTCONbits.T0IF == 1) //chequea la bandera del tmr0
    {
        
        if (banderas == 0) //display
        {
            PORTBbits.RB3 = 1; //enciende el pin 3 del puerto B para encender el display
            PORTC = (tabla[uni]); //pasa al puerto c los valores de la tabla
        }
    banderas = 0;
    TMR0 = _tmr0; 
    INTCONbits.T0IF = 0;
    }
}

 
// Main
void main(void)
{
    setup();
    //loop principal
    while (1)
    {
        if (!PORTBbits.RB0) // condicion el pin esta apagado
        {
            while (!RB0);
            PORTE--; //reduce el valor del puerto E cada vez que el boton se presiona
            var = PORTE; //la variable tiene el mismo valor del puerto E
            if (var == 7)
            {
                PORTE = 0b100; //si la variable llega al 7, se le da un nuevo valor al puerto E
                // esto es para lograr que el contador sea por decadas y no binario
            }
            else if (var == 3)
            {
                PORTE = 0b010;
            }
        }
        if (!PORTBbits.RB1) //condicion el pin se apaga
        {
            while (!RB1);
            PORTA++; //cada vez que el boton se presiona, se aumenta el valor del PORTA 
            var2 = PORTA; //una variable adquiere ell valor del puerto A
            PORTBbits.RB4 = 0; //clear al pin 4 del port b, pin que 
            //se enciende si gana el jugador del puerto A (jugador 1))
            if (var2 == 3) 
            {
                PORTA = 4; //saltos en los valores para que el contador sea en decadas y no binario
            }
            else if (var2 == 5)
            {
                PORTA = 8;
            }
            else if (var2 == 9)
            {
                PORTA = 16;
            }
            else if (var2 == 17)
            {
                PORTA = 32;
            }
            else if (var2 == 33)
            {
                PORTA = 64;
            }
            else if (var2 == 65)
            {
                PORTA = 128;
            }
            else if (var2 == 129)
            {
                PORTA = 256;
                PORTBbits.RB4 = 1; //al ganar el jugador 1 se enciende este pin
                var = 1; //se le da un nuevo valor a la variable relacionada al display para que este muestre
                //que jugador ganó
            }
        }
        if (!PORTBbits.RB2) // condición el pin esta apagado
        {
            while (!RB2);
            PORTD++; //aumennta el valor del port d si se oprime el boton
            var3 = PORTD; //una variable adquiere el valor del puerto
            PORTBbits.RB5 = 0; //clear al pin con la led que indica si eñl jugador 2 gana
            if (var3 == 3)
            {
                PORTD = 4; // saltos para volver el contador en decadas
            }
            else if (var3 == 5)
            {
                PORTD = 8;
            }
            else if (var3 == 9)
            {
                PORTD = 16;
            }
            else if (var3 == 17)
            {
                PORTD = 32;
            }
            else if (var3 == 33)
            {
                PORTD = 64;
            }
            else if (var3 == 65)
            {
                PORTD = 128;
            }
            else if (var3 == 129)
            {
                PORTD = 256;
                PORTBbits.RB5 = 1; //encendido del pin que indica al ganador
                var = 2; //nuevo valor a la variable del display para que este tambien muestre al ganador
            }
        }
        valores (); //llamado a la función que transforma el valor binario de la variable var en decimales
    }
}

//configuraciones en general
void setup(void)
{
    // config de las ins y outs
    ANSEL = 0; 
    ANSELH = 0; 
    
    TRISA = 0; //out
    TRISB = 0b000111; //ins    
    TRISC = 0; //out
    TRISD = 0; // out
    TRISE = 0; // out
    
    OPTION_REGbits.nRBPU = 0; //pullups del puerto b habilitados
    WPUB = 0b111; //pullups asignados
    
    PORTA = 0; //clear
    PORTB = 0; //clear
    PORTC = 0; //clear
    PORTD = 0; //clear
    PORTE = 0; //clear
    
    // config de oscilador
    OSCCONbits.IRCF = 0b0100; // a 1MHz
    OSCCONbits.SCS = 1; //set al reloj interno
    
    //config rb
    INTCONbits.RBIE = 1; //interrupciones del puerto b habilitadas
    
    // config de tmr0
    OPTION_REGbits.T0CS = 0; //seleccion del reloj del timer 0
    OPTION_REGbits.T0SE = 0; //
    OPTION_REGbits.PSA = 0; //seleccion del prescaler del tmr0
    OPTION_REGbits.PS2 = 1; // seleccion de prescaler de 111 = 256
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    TMR0 = _tmr0; //valor para el tmr de 237
    
    //interrupciones
    INTCONbits.T0IF = 0; //habilitando bandera del tmr0
    INTCONbits.T0IE = 1; //habilitando interrupciones del tmr0
    INTCONbits.RBIF = 0; //bandera del puerto b
    INTCONbits.RBIE = 1; //Activar interrupciones de puerto b
    INTCONbits.GIE = 1; //habilitando interrupciones generales
    
    return;
}

int valores(void)
{
    resi = var%100;
    uni = resi%10;
    dec = resi/10;
    cent = var/100;
}


#include <xc.h>
#include <math.h>
#include "timers.h"
#include "capture.h"
#include "xlcd.h"
#include "pwm.h"
#include "lib.h"

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator (HS))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 1         // Brown-out Reset Voltage bits (Setting 2 4.33V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = OFF     // CCP2 MUX bit (CCP2 input/output is not multiplexed with RB3)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

unsigned long quant = 0;
unsigned int stopProcess = 0;
int valorFinal;

void __interrupt() highPriority(void);
void __interrupt(low_priority)lowPriority(void);
void func_timer0(void);
void func_timer1(void);
void Interr_para_processo(void);
void Interr_1(void);
void func_CCP1(void);
void insereLCD(int dutyC);

void __interrupt() highPriority(void) {
    if (TMR0IF && TMR0IE) {
        func_timer0();
    } else if (TMR1IF && TMR1IE) {
        func_timer1();
    } else if (CCP1IF && CCP1IE) {
        func_CCP1();
    } else if (INT0IF && INT0IE) {
        Interr_para_processo();
        INT0IF = 0;
        INTEDG0 = INTEDG0 ^ 1;
    } else if (INT2IF && INT2IE) {
        Interr_para_processo();
        INT2IF = 0;
        INTEDG2 = INTEDG2 ^ 1;
    }
}

void __interrupt(low_priority)lowPriority(void) {
    if (INT1IF && INT1IE) {
        Interr_1();
        INTEDG1 = INTEDG1 ^ 1;
    }
}

void Interr_para_processo(void) {
    SetDCPWM2(0);
    stopProcess = 1;
    limpa();
    Mydelay(50);
    if (!RB0 && RB2) {
        writeCommandString(0x40, "PChoque Direito");
    } else if (!RB2 && RB0) {
        writeCommandString(0x00, "PChoque Esquerdo");
    } else if (RB0 && RB2){
        writeCommandString(0x00, "PChoque Esquerdo");
        writeCommandString(0x40, "PChoque Direito");
    } else {
        writeCommandString(0x00, "Retomando...");
    }
}

void Interr_1(void) {
    INT1IF = 0;

    /*
     * f     = 6100hz
     * T     = 164u
     * PR2   = 50
     * DCmax = 205
     */
    OpenPWM2(50); //DCmax = 625
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_8);
}

void func_timer1(void) {
    if (!RB0 && !RB2) {
        stopProcess = 0;
    }
    quant++;
    TMR1IF = 0;
}

void func_timer0(void) {
    static int auxValorFinal;
    static long cont;
    unsigned int auxPWM;
    TMR0IF = 0;
    cont++;

    if (cont >= 5) {
        cont = 0;
        int dutyC = 0;
        if (stopProcess == 0) {
            if (valorFinal == 40) {
                //Setado PWM para funcionar com 20%
                SetDCPWM2(41);
                dutyC = 20;
            } else if (valorFinal < 40) {
                //Setado PWM para funcionar com 10%
                SetDCPWM2(20);
                dutyC = 10;
            } else if (valorFinal >= 172) {
                //Setado PWM para funcionar com 100%
                SetDCPWM2(205);
                dutyC = 100;
            } else {
                //Calculo para proporcionalidade do PWM com a distancia
                dutyC = (int) ((valorFinal * 100) / 172.0);
                auxPWM = (int) ((dutyC / 100.0)*205.0);
                SetDCPWM2(auxPWM);
            }
            if (valorFinal < 173 && valorFinal > 3 && valorFinal != auxValorFinal) {
                auxValorFinal = valorFinal;
                insereLCD(dutyC);
            }
        } else {
            auxValorFinal = 0;
        }

        RD0 = RD0^1;
        Mydelay(1);
        RD0 = RD0^1;

        WriteTimer0(3036);
    } else {
        WriteTimer0(3036);
    }
}

void func_CCP1(void) {
    static unsigned int eventoInicio = 0;
    unsigned int valorTimer1;
    unsigned long tempoEvento;
    float tempofloat;
    if (eventoInicio == 0) {
        quant = 0;
        eventoInicio = 1;
        WriteTimer1(0);
        OpenCapture1(CAPTURE_INT_ON & CAP_EVERY_FALL_EDGE); // Interrupção e borda de subida
        WriteTimer0(3036);
    } else {
        valorTimer1 = ReadCapture1();
        tempoEvento = 65536 * quant + valorTimer1;
        tempofloat = (float) tempoEvento * (0.0000002);
        valorFinal = (int) (((tempofloat) / 0.00005833) + 1); //Ajuste para diminuir a perda de precisao

        eventoInicio = 0;
        OpenCapture1(CAPTURE_INT_ON & CAP_EVERY_RISE_EDGE); // Interrupção e borda de descida
        WriteTimer0(3036);
    }
}

void main(void) {
    TRISC = 0b00000100;
    TRISB = 0b00000111;
    TRISD = 0;
    ADCON1 = 0x0F;

    INT0IF = 0;
    INT1IF = 0;
    INT2IF = 0;

    INTEDG0 = 1; // transição na descida
    INTEDG1 = 0; // transição na descida
    INTEDG2 = 1; // transição na descida

    INT0IE = 1; // interrupção externa 0 ativada
    INT1IE = 1; // interrupção externa 2 ativada
    INT2IE = 1; // interrupção externa 2 ativada

    INT1IP = 0; // prioridade de INT2 baixa
    OpenTimer0(TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_PS_1_8);
    OpenTimer1(TIMER_INT_ON & T1_16BIT_RW & T1_SOURCE_INT & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF & T1_PS_1_1);
    OpenTimer2(TIMER_INT_OFF & T2_PS_1_16);
    SetTmrCCPSrc(T1_SOURCE_CCP); // IMPORTANTE: Seta TIMER1 como fonte para o módulo CCP
    // poderia ser: T3_SOURCE_CCP - > Seta TIMER3 como fonte para o módulo CCP
    //	      : T1_CCP1_T3_CCP2 - > Seta TIMER1 para o CCP1 e TIMER3 para o CCP2
    // Configuração do CCP1
    OpenCapture1(CAPTURE_INT_ON & CAP_EVERY_RISE_EDGE); // Interrupção e borda de descida

    //OpenPWM2(124);
    //SetDCPWM2(200);
    TMR1IP = 1; // Prioridade do TIMER1 como alta;
    CCP1IP = 1; // Prioridade do CCP1 como alta;

    IPEN = 1; // habilita prioridade
    GIEH = 1; // habilita interrupções de alta prioridade
    GIEL = 1; // habilita interrupções de baixa prioridade

    WriteTimer1(0);
    WriteTimer0(3036);

    inic_XLCD();

    while (BusyXLCD());
    SetDDRamAddr(0x00);

    while (1);
}

void insereLCD(int dutyC) {
    limpa();
    writeCommandString(0x00, "PWM:");
    readAndWriteNumber(dutyC, '0');
    writeCommandString('0', "%");

    writeCommandString(0x40, "DISTANCIA:");
    readAndWriteNumber(valorFinal, '0');
    writeCommandString('0', "cm");
}

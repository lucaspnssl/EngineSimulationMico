#include <xc.h>
#include "xlcd.h"
#include "lib.h"

void readAndWriteNumber(int numberFirst, unsigned char posicao) {
    char numero[4];
    int number = numberFirst;
    numero[0] = (number / 100) + 48;
    number = number % 100;
    numero[1] = (number / 10) + 48;
    number = number % 10;
    numero[2] = number + 48;
    numero[3] = '\0';

    writeCommandString(posicao, numero);
}

void writeCommand(unsigned char posicao, unsigned char value) {
    if (posicao != '0') {
        SetDDRamAddr(posicao);
        while (BusyXLCD());
        putcXLCD(value);
        while (BusyXLCD());
        Mydelay(50);
    } else {
        while (BusyXLCD());
        putcXLCD(value);
        while (BusyXLCD());
        Mydelay(50);
    }
}

void Mydelay(int tempo) {
    int i;
    for (i = 0; i < tempo; i++) {
        __delay_ms(1);
    }
}

void inic_XLCD(void) {
    OpenXLCD(FOUR_BIT & LINES_5X7); // configura para 4-bit, 5x7, multiplas linhas
    while (BusyXLCD());
    WriteCmdXLCD(0x06); // move o cursor para direita depois de escrever 
    while (BusyXLCD());
    WriteCmdXLCD(0x0F); // display ligado com cursor piscado
    while (BusyXLCD());
}

void DelayFor18TCY(void) {
    __delay_us((8 * 18.0 * 4000000.0) / _XTAL_FREQ);
    // Seria necessario 18, mas na pratica e necessario aumentar o valor
    // Quanto maior a frequencia, maior o valor
}

void DelayPORXLCD(void) {
    __delay_ms(15);
}

void DelayXLCD(void) {
    __delay_ms(5);
}

void writeCommandString(unsigned char posicao, unsigned char value[20]) {
    if (posicao != '0') {
        SetDDRamAddr(posicao);
        while (BusyXLCD());
        putrsXLCD(value);
        while (BusyXLCD());
        Mydelay(5);
    } else {
        while (BusyXLCD());
        putrsXLCD(value);
        while (BusyXLCD());
        Mydelay(5);
    }
}

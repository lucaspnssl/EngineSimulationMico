#define _XTAL_FREQ 20000000

void readAndWriteNumber(int numberFirst, unsigned char posicao);
void writeCommand(unsigned char posicao, unsigned char value);
void writeCommandString(unsigned char posicao, unsigned char value[20]);
void Mydelay(int tempo);
void inic_XLCD(void);
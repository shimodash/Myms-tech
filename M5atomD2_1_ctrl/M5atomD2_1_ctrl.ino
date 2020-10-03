//board setting : M5Stick-C

#include "M5Atom.h"

extern const unsigned char AtomImageData[375 + 2];

uint8_t DisBuff[2 + 5 * 5 * 3];

void setBuff(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
{
    DisBuff[0] = 0x05;
    DisBuff[1] = 0x05;
    for (int i = 0; i < 25; i++)
    {
        DisBuff[2 + i * 3 + 0] = Rdata;
        DisBuff[2 + i * 3 + 1] = Gdata;
        DisBuff[2 + i * 3 + 2] = Bdata;
    }
}

void setBuff_straight(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
{
    DisBuff[0] = 0x05;
    DisBuff[1] = 0x05;
    for (int i = 0; i < 25; i++)
    {
        if (i%5==2 || i==6||i==8
            ||i==10||i==14){
        DisBuff[2 + i * 3 + 0] = Rdata;
        DisBuff[2 + i * 3 + 1] = Gdata;
        DisBuff[2 + i * 3 + 2] = Bdata;
        }else{
        DisBuff[2 + i * 3 + 0] = 0;
        DisBuff[2 + i * 3 + 1] = 0;
        DisBuff[2 + i * 3 + 2] = 0;
        }
    }
  M5.dis.displaybuff(DisBuff);

}

void setBuff_right(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
{
    DisBuff[0] = 0x05;
    DisBuff[1] = 0x05;
    for (int i = 0; i < 25; i++)
    {
        if (i==2||i==8||i==10 || i==11
            ||i==12||i==13||i==14||i==18||i==22){
        DisBuff[2 + i * 3 + 0] = Rdata;
        DisBuff[2 + i * 3 + 1] = Gdata;
        DisBuff[2 + i * 3 + 2] = Bdata;
        }else{
        DisBuff[2 + i * 3 + 0] = 0;
        DisBuff[2 + i * 3 + 1] = 0;
        DisBuff[2 + i * 3 + 2] = 0;
        }
    }
  M5.dis.displaybuff(DisBuff);
}

void setBuff_left(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
{
    DisBuff[0] = 0x05;
    DisBuff[1] = 0x05;
    for (int i = 0; i < 25; i++)
    {
        if (i==2||i==6||i==10 || i==11
            ||i==12||i==13||i==14||i==16||i==22){
        DisBuff[2 + i * 3 + 0] = Rdata;
        DisBuff[2 + i * 3 + 1] = Gdata;
        DisBuff[2 + i * 3 + 2] = Bdata;
        }else{
        DisBuff[2 + i * 3 + 0] = 0;
        DisBuff[2 + i * 3 + 1] = 0;
        DisBuff[2 + i * 3 + 2] = 0;
        }
    }
  M5.dis.displaybuff(DisBuff);
}

int PWMCHR = 14;
int PWMCHL = 15;

void setup()
{

    M5.begin(true, false, true);
    delay(10);
    setBuff(0xff, 0x00, 0x00);
    M5.dis.displaybuff(DisBuff);
    pinMode(33, OUTPUT);
    pinMode(23, OUTPUT);
//    digitalWrite(33, HIGH);
//    digitalWrite(23, HIGH);
    ledcSetup(PWMCHR, 1000, 8); // 1000Hz, 8Bit(256段階)
    ledcSetup(PWMCHL, 1000, 8); // 1000Hz, 8Bit(256段階)
    ledcAttachPin(33, PWMCHR);
    ledcAttachPin(23, PWMCHL);
    ledcWrite(PWMCHR, 256);
    ledcWrite(PWMCHL, 256);

}

uint8_t FSM = 0;

void loop()
{
    if (M5.Btn.wasPressed())
    {

        switch (FSM)
        {
        case 0:
            setBuff_straight(0x40, 0x00, 0x00);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            setBuff_right(0x00, 0x00, 0x40);
            ledcWrite(PWMCHR, 256);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            setBuff_straight(0x40, 0x00, 0x00);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            setBuff_left(0x00, 0x40, 0x00);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 256);
            delay(1000);
            setBuff_straight(0x40, 0x00, 0x00);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            ledcWrite(PWMCHR, 256);
            ledcWrite(PWMCHL, 256);
            delay(1000);

            break;
        case 1:
            setBuff_straight(0x00, 0x40, 0x00);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            setBuff_right(0x00, 0x40, 0x00);
            ledcWrite(PWMCHR, 256);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            setBuff_straight(0x00, 0x40, 0x00);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            setBuff_left(0x00, 0x40, 0x00);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 256);
            delay(1000);
            setBuff_straight(0x00, 0x40, 0x00);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            ledcWrite(PWMCHR, 256);
            ledcWrite(PWMCHL, 256);
            delay(1000);
            break;
        case 2:
            setBuff(0x00, 0x00, 0x40);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            ledcWrite(PWMCHR, 256);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 256);
            delay(1000);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            ledcWrite(PWMCHR, 256);
            ledcWrite(PWMCHL, 256);
            delay(1000);
            break;
        case 3:
            setBuff(0x20, 0x20, 0x20);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            ledcWrite(PWMCHR, 256);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 256);
            delay(1000);
            ledcWrite(PWMCHR, 64);
            ledcWrite(PWMCHL, 64);
            delay(1000);
            ledcWrite(PWMCHR, 256);
            ledcWrite(PWMCHL, 256);
            delay(1000);
            break;
        default:
            break;
        }
        M5.dis.displaybuff(DisBuff);

        FSM++;
        if (FSM >= 4)
        {
            FSM = 0;
        }
    }

    delay(50);
    M5.update();
}

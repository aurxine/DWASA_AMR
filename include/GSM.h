#ifndef GSM_H
#define GSM_H
#include<Arduino.h>
#include<SoftwareSerial.h>
#include<EEPROM.h>
class GSM
{
private:
    uint8_t Rx_Pin;
    uint8_t Tx_Pin;
    SoftwareSerial SIM800L;
public:
    GSM(uint8_t rx_pin, uint8_t tx_pin);
    // ~GSM();
};



#endif
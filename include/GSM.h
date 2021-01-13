#ifndef GSM_H
#define GSM_H
#include<Arduino.h>
#include<SoftwareSerial.h>
#include<EEPROM.h>

typedef struct SString{
  String number;
  String text;
};


class GSM
{
private:
    uint8_t Rx_Pin;
    uint8_t Tx_Pin;
    SoftwareSerial SIM800L;
public:
    GSM(uint8_t rx_pin, uint8_t tx_pin);  
    void SendMessage(String message, String number);
    SString ReceiveMessage();
    // ~GSM();
};



#endif
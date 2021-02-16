#ifndef GSM_H
#define GSM_H
#include<Arduino.h>
#include<SoftwareSerial.h>
#include<EEPROM.h>

typedef struct Double_String{
  String number;
  String text;
}SString;


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
    void begin(int braudrate);
    void flush();
    void delAllMessage();
    // ~GSM();
};



#endif
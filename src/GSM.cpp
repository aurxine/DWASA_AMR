#include "GSM.h"
#include<Arduino.h>

GSM::GSM(uint8_t rx, uint8_t tx): SIM800L(rx, tx)
{
    Rx_Pin = rx;
    Tx_Pin = tx;
}

void GSM::SendMessage(String message, String number)
{
    unsigned int len = number.length() + 1;
    char Number[len];
    number.toCharArray(Number, len);

    SIM800L.println(Number);
    SIM800L.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    char outString[19];
    sprintf_P(outString, PSTR("AT+CMGS=\"%s\"\r"), Number);
    SIM800L.println(outString);
    delay(1000);
    SIM800L.println(message);// The SMS text you want to send
    delay(100);
    SIM800L.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
}

String GSM::ReceiveMessage()
{
    String textMessage = "";

    SIM800L.print("AT+CMGF=1\r");
    delay(1000);
    SIM800L.println("AT+CNMI=2,2,0,0,0\r");  
    delay(2000);

    if(SIM800L.available() > 0)
    {
        textMessage = SIM800L.readString();
        delay(100);
        Serial.println("Received message = %s\r" + textMessage);
    }
    return textMessage;
}
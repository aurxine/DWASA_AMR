#include "GSM.h"
#include<Arduino.h>

GSM::GSM(uint8_t rx, uint8_t tx): SIM800L(rx, tx)
{
    Rx_Pin = rx;
    Tx_Pin = tx;
}

void GSM::begin(int braud)
{
    SIM800L.begin(braud);
    SIM800L.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
    
}

void GSM::getNetwork()
{
    SIM800L.print("AT+CSQ\r");

    String text = "";

    if(SIM800L.available())
    {
        text += SIM800L.readString();
    }
    Serial.println(text);
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

SString GSM::ReceiveMessage()
{
    String textMessage = "";

    //SIM800L.print("AT+CMGF=1\r");
    //delay(1000);
    SIM800L.println("AT+CNMI=1,1,0,0,0\r");  
    delay(2000);

    SIM800L.println("AT+CMGL=\"REC UNREAD\"\r"); // to get the unread message
    //SIM800L.println("AT+CMGR=3\r"); // to get the message stored at location 3
    if(SIM800L.available() > 0)
    {
        textMessage = SIM800L.readString();
        delay(500);
        
    }

    SString msg;
    String temp;
    String data = textMessage;
    int len = data.length(), cnt = 0;
    
    for(int i = 0; i < len;i++)
    {
        //Serial.println(data.substring(i,i+1));
        temp = data.substring(i,i+1);
        if (temp == "+" && i < len-1)
        {
        if(data.substring(i+1,i+2) == "8")
            msg.number = data.substring(i,i+13);
        }

        if(temp == "\"")
        {
        cnt = i;       
        }
            
    }
    msg.text = (data.substring(cnt+3));
    return msg;
}
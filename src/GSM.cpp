#include "GSM.h"
#include<Arduino.h>

GSM::GSM(uint8_t rx, uint8_t tx): SIM800L(rx, tx)
{   
    Rx_Pin = rx;
    Tx_Pin = tx;
}

void GSM::delAllMessage()
{
    SIM800L.print("AT+CMGD=1,4\n\r");
}

void GSM::begin(int braudrate)
{
    SIM800L.begin(braudrate);
    delay(1000);
    SIM800L.println("AT+IPR=9600");
    delay(500);
    SIM800L.println("AT+CNMI=1,2,0,0,0\r");  
    delay(2000);
}

void GSM::flush()
{
  SIM800L.write(27);
  SIM800L.write(27);
  SIM800L.write(27);
  SIM800L.print("\r\n");
}

void GSM::SendMessage(String message, String number)
{

  SIM800L.println("AT+CMGF=1");    
  delay(1000);
  SIM800L.println("AT+CMGS=\"+" + number + "\"\r"); 
  delay(1000);
  SIM800L.println(message);
  delay(500);
  SIM800L.println((char)26);
  delay(1000);

}

SString GSM::ReceiveMessage()
{
    String textMessage = "";

    // SIM800L.print("AT+CMGF=1\r");
    // delay(500);

    if(SIM800L.available() > 0)
    {
        textMessage = SIM800L.readString();
        delay(500);
        Serial.println(textMessage);
    }
    // Serial.println(textMessage);

    SString msg;
    String temp;
    String data = textMessage;
    int len = data.length(), cnt1 = 0, cnt2 = 0;

    bool next_rn = false;

    for(int i = 0; i < len;i++)
    {
      // Serial.println(data.substring(i,i+1));
      temp = data.substring(i,i+1);

      if(temp == "+" && i < len - 1)
      {
        // Serial.println(data.substring(i+1,i+2));
        if(data.substring(i+1,i+2) == "8")
        {
          msg.number = data.substring(i+3,i+14);
          i += 14;
        }
            
      }

      else if(temp == "\"" && i < len -1)
      {
        if(data.substring(i + 1, i + 2) == "\r" && data.substring(i + 2, i + 3) == "\n")
        {
          cnt1 = i + 3;
          next_rn = true;
          i += 3;
        }
        
      }

      else if(temp == "\r" && i < len -1)
      {
        if(data.substring(i + 1, i + 2) == "\n" && next_rn)
        {
          cnt2 = i;
          // Serial.print("Extracted text: ");
          // Serial.println(data.substring(cnt1, cnt2));
          next_rn = false;
        }
      }
      
            
    }
    msg.text = data.substring(cnt1, cnt2);
    
    return msg;
}
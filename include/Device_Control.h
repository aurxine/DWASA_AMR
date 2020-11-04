#ifndef Device_Control_H
#define Device_Control_H
#include<Arduino.h>
#include<EEPROM.h>
#define Max_Number_of_Contacts 5


class Device_Control
{
private:
    String Contacts[Max_Number_of_Contacts];
public:
    Device_Control(/* args */);
    void writeByteInEEPROM(int address, byte data);
    void writeStringInEEPROM(int address, String data);
    byte readByteInEEPROM(int address);
    String readStringInEEPROM(int address, int length);
    // ~Device_Control();
};


#endif
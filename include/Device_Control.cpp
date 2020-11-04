#include "Device_Control.h"


Device_Control::Device_Control(/* args */)
{

}

void Device_Control::writeByteInEEPROM(int address, byte data)
{
    EEPROM.write(address, data);
}

void Device_Control::writeStringInEEPROM(int address, String data)
{
    int data_length = data.length();
    for(int i = address; i < address + data_length; i++)
    {
        EEPROM.write(i, data[i - address]);
    }
}

byte Device_Control::readByteInEEPROM(int address)
{
    return EEPROM.read(address);
}

String Device_Control::readStringInEEPROM(int address, int length)
{
    String data;
    for(int i = address; i < address + length; i++)
    {
        data += (char)EEPROM.read(i);
    }
    return data;
}
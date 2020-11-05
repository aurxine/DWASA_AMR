#ifndef Device_Control_H
#define Device_Control_H
#include<Arduino.h>
#include<EEPROM.h>
#define Max_Number_of_Contacts 5
#define ID_Length 10

class Device_Control
{
public:
    String ID;
    String Contacts[Max_Number_of_Contacts];
    unsigned long Water_Flow;
    unsigned long Initial_Water_Flow;

    uint8_t ID_start_address = 0;
    uint8_t ID_length_address = 1;
    uint8_t Contacts_start_address = 2;
    uint8_t Number_of_saved_contacts_address = Contacts_start_address + Max_Number_of_Contacts*11;
    uint8_t Water_Flow_start_address = Number_of_saved_contacts_address + 1;

    Device_Control(/* args */);
    void get_ID(String ID);// receives and saves an ID
    void show_ID();//Serial prints ID
    void get_Contact(String number);//gets a number and appends it to the array
    void replace_Contact(String number, int position);//replace a contact in specified position
    bool check_Contact(String number);//checks whether a contact exists or not
    void writeByteInEEPROM(int address, byte data);//writes a byte in EEPROM
    void writeStringInEEPROM(int address, String data);//writes a string (multiple bytes) in EEPROM
    byte readByteInEEPROM(int address);//reads a byte in EEPROM
    String readStringInEEPROM(int address, int length);//reads a string in EEPROM
    // ~Device_Control();
};


#endif
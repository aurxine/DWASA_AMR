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
    int Water_per_Pulse;

    uint8_t ID_start_address = 0;
    uint8_t Number_of_saved_contacts_address = ID_start_address + ID_Length + 1;
    uint8_t Contacts_start_address = Number_of_saved_contacts_address + Max_Number_of_Contacts*11;
    uint8_t Water_per_Pulse_start_address = Contacts_start_address + 1;
    uint8_t Water_Flow_start_address = Water_per_Pulse_start_address + 1;

    Device_Control(/* args */);
    void put_ID(String ID);// receives and saves an ID
    void show_ID();//Serial prints ID
    String device_ID();//returns ID

    bool put_Contact(String number);//puts a number and appends it to the array
    void replace_Contact(String number, int position);//replace a contact in specified position
    bool check_Contact(String number);//checks whether a contact exists or not
    String get_Contact(int pos); //returns contact number at pos position
    void show_Contact(int pos);//shows contact at a specified position
    void show_All_Contacts();//shows all saved contacts

    void put_Water_per_Pulse(byte amount);//saves water flow per pulse data
    void show_Water_per_Pulse();//shows water per pulse data
    void put_Initial_Water_Flow(unsigned long water_flow);//puts and saves past water flow
    void show_Initial_Water_Flow();// shows initial water flow
    unsigned long total_Water_Flow();// returns total water flow
    void save_Water_Flow(unsigned long water_flow);// save stotal water flow in EEPROM
    
    void writeByteInEEPROM(int address, byte data);//writes a byte in EEPROM
    void writeStringInEEPROM(int address, String data);//writes a string (multiple bytes) in EEPROM
    byte readByteInEEPROM(int address);//reads a byte in EEPROM
    String readStringInEEPROM(int address, int length);//reads a string in EEPROM
    // ~Device_Control();
};


#endif
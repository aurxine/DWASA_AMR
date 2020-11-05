#include "Device_Control.h"


Device_Control::Device_Control(/* args */)
{

}

void Device_Control::get_ID(String ID)// receives and saves an ID
{
    this->ID = ID;
    this->writeStringInEEPROM(ID_start_address, ID);
}

void Device_Control::show_ID()//Serial prints ID
{
    Serial.print("Device ID: ");
    Serial.println(this->ID);
}

void Device_Control::get_Contact(String number)//gets a number and appends it to the array
{
    int number_of_saved_contacts = this->readByteInEEPROM(Number_of_saved_contacts_address);
    this->writeStringInEEPROM(Contacts_start_address + number_of_saved_contacts*11, number);
}

void Device_Control::replace_Contact(String number, int position)//replace a contact in specified position
{
    this->writeStringInEEPROM(Contacts_start_address + position*11, number);
}

bool Device_Control::check_Contact(String number)//checks whether a contact exists or not
{
    int number_of_saved_contacts = this->readByteInEEPROM(Number_of_saved_contacts_address);
    //Foysal will complete it
    for(int i = Contacts_start_address; i < number_of_saved_contacts*11; i+=11)
    {
        if(number == this->readStringInEEPROM(i,11))
        {
            return 1;
        }
    }
    return 0;
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
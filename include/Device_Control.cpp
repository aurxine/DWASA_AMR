#include "Device_Control.h"


Device_Control::Device_Control(/* args */)
{
    this->writeByteInEEPROM(Number_of_saved_contacts_address, 0);
}

void Device_Control::get_ID(String ID)// receives and saves an ID
{
    this->ID = ID;
    this->writeStringInEEPROM(ID_start_address, ID);
}

void Device_Control::show_ID()//Serial prints ID
{
    this->ID = readStringInEEPROM(ID_start_address, ID_Length);
    Serial.print("Device ID: ");
    Serial.println(this->ID);
}

bool Device_Control::get_Contact(String number)//gets a number and appends it to the array
{
    int number_of_saved_contacts = this->readByteInEEPROM(Number_of_saved_contacts_address);
    if(number_of_saved_contacts == Max_Number_of_Contacts)
        return false;
    this->Contacts[number_of_saved_contacts - 1] = number;
    this->writeStringInEEPROM(Contacts_start_address + number_of_saved_contacts*11, number);
    this->writeByteInEEPROM(Number_of_saved_contacts_address, number_of_saved_contacts + 1);
    return true;
}

void Device_Control::replace_Contact(String number, int position)//replace a contact in specified position
{
    this->writeStringInEEPROM(Contacts_start_address + position*11, number);
}

bool Device_Control::check_Contact(String number)//checks whether a contact exists or not
{
    int number_of_saved_contacts = this->readByteInEEPROM(Number_of_saved_contacts_address);
    //Foysal will complete it
}

void Device_Control::show_Contact(int pos)//shows contact at a specified position
{
    Serial.print("Contact " + String(pos) + ": ");
    Serial.println(Contacts[pos]);
}
void Device_Control::show_All_Contacts()//shows all saved contacts
{
    int number_of_saved_contacts = this->readByteInEEPROM(Number_of_saved_contacts_address);
    for(int i = 0; i < number_of_saved_contacts; i++)
    {
        this->show_Contact(i);
    }
}

void Device_Control::get_Water_per_Pulse(byte amount)//saves water flow per pulse data
{
    Water_per_Pulse = amount;
    this->writeByteInEEPROM(Water_per_Pulse_start_address, amount);
}

void Device_Control::show_Water_per_Pulse()//shows water per pulse data
{
    this->Water_per_Pulse = this->readByteInEEPROM(Water_per_Pulse_start_address);
    Serial.print("Water flow per pulse: ");
    Serial.println(Water_per_Pulse);
}

void get_Initial_Water_Flow(unsigned long water_flow);//gets and saves past water flow
void show_Initial_Water_Flow();// shows initial water flow
unsigned long total_Water_Flow();// returns total water flow
void save_Water_Flow(unsigned long water_flow);// save stotal water flow in EEPROM

void Device_Control::writeByteInEEPROM(int address, byte data)
{
    EEPROM.write(address, data);
    // byte val = EEPROM.read(address);
    // Serial.println(address);
    // Serial.println(val);
}

void Device_Control::writeStringInEEPROM(int address, String data)
{
    int data_length = data.length();
    for(int i = address; i < address + data_length; i++)
    {
        // Serial.println(data[i - address]);
        EEPROM.write(i, data[i - address]);
        // byte val = EEPROM.read(i);
        // Serial.println((char)val);
    }
}

byte Device_Control::readByteInEEPROM(int address)
{
    byte val = EEPROM.read(address);
    Serial.println(address);
    Serial.println(val);
    return val;
}

String Device_Control::readStringInEEPROM(int address, int length)
{
    String data = "";
    for(int i = address; i < address + length; i++)
    {
        //Serial.println(data);
        byte val = EEPROM.read(i);
        data += (char)val;
    }
    return data;
}

void to_int8(uint8_t* arr, uint64_t *number)
{
    uint8_t total = 0, power = 1, ind = 0;
    for(uint8_t i = 1; i <=64; i++)
    {
        //printf("%d: ", i);
        if(i%8)
        {
            //printf("p%d ", power);

            if((1LL << (i-1))&(*number))
                total += power;
            power *= 2;

        }
        else{
            if((1LL << (i-1))&(*number))
                total += power;
            if(i){

                arr[ind++] = total;
            }

            power = 1;
            total = 0;

        }
    }
}

uint64_t to_uint64(uint8_t* arr)
{
    uint64_t number = 0, power = 1;

    for(uint8_t i = 0; i < 8; i++)
    {
        for(uint8_t j = 0; j < 8; j++)
        {
            if((1LL << (j))&arr[i])
                number += power;

            power *= 2;
        }
    }

    return number;
}
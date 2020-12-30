#include "Device_Control.h"

void to_int8(uint8_t* arr, unsigned long *number)
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

uint64_t to_unsigned_long(uint8_t* arr)
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


Device_Control::Device_Control(/* args */)
{
    this->writeByteInEEPROM(Number_of_saved_contacts_address, 0);
}

void Device_Control::put_ID(String ID)// receives and saves an ID
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

String Device_Control::device_ID()
{
    this->ID = readStringInEEPROM(ID_start_address, ID_Length);
    return ID;
}

bool Device_Control::put_Contact(String number)//puts a number and appends it to the array
{
    Serial.print("Input Number: "); Serial.println(number);
    int number_of_saved_contacts = this->readByteInEEPROM(Number_of_saved_contacts_address);
    Serial.print("Number of already saved contacts: "); Serial.println(number_of_saved_contacts);
    if(number_of_saved_contacts == Max_Number_of_Contacts)
        return false;
    this->Contacts[number_of_saved_contacts] = number;
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
    for(int i = Contacts_start_address; i < number_of_saved_contacts*11; i+=11)
    {
        if(number == this->readStringInEEPROM(i,11))
        {
            return 1;
        }
    }
    return 0;
}

void Device_Control::show_Contact(int pos)//shows contact at a specified position
{
    int number_of_saved_contacts = this->readByteInEEPROM(Number_of_saved_contacts_address);
    for(int i = 0; i < number_of_saved_contacts; i++)
    {
        this->Contacts[i] = this->readStringInEEPROM(Contacts_start_address + i*11, 11);
    }
    Serial.print("Contact " + String(pos) + ": ");
    Serial.println(Contacts[pos]);
}



void Device_Control::show_All_Contacts()//shows all saved contacts
{
    int number_of_saved_contacts = this->readByteInEEPROM(Number_of_saved_contacts_address);
    for(int i = 0; i < number_of_saved_contacts; i++)
    {
        this->Contacts[i] = this->readStringInEEPROM(Contacts_start_address + i*11, 11);
        Serial.print("Contact " + String(i + 1) + ": ");
        Serial.println(Contacts[i]);
    }
}

String Device_Control::get_Contact(int pos)
{
    int number_of_saved_contacts = this->readByteInEEPROM(Number_of_saved_contacts_address);
    if (pos > number_of_saved_contacts)
    {
        Serial.println("Position excedes number of saved contacts");
        return "";
    }

    for(int i = 0; i < number_of_saved_contacts; i++)
    {
        this->Contacts[i] = this->readStringInEEPROM(Contacts_start_address + i*11, 11);
    }
    return Contacts[pos];
}

void Device_Control::put_Water_per_Pulse(byte amount)//saves water flow per pulse data
{
    Water_per_Pulse = amount;
    this->writeByteInEEPROM(Water_per_Pulse_start_address, amount);
    byte val = this->readByteInEEPROM(Water_per_Pulse_start_address);
    Serial.println(val);
}

void Device_Control::show_Water_per_Pulse()//shows water per pulse data
{
    this->Water_per_Pulse = this->readByteInEEPROM(Water_per_Pulse_start_address);
    Serial.print("Water flow per pulse: ");
    Serial.println(Water_per_Pulse);
}

void Device_Control::put_Initial_Water_Flow(unsigned long water_flow)//puts and saves past water flow
{
    uint8_t arr[4]; // unsigned long requires 4 bytes of data
    to_int8(arr, &water_flow);
    for(int i = 0; i < 4; i++)
    {
        this->writeByteInEEPROM(Water_Flow_start_address + i, arr[i]);
    }
}

void Device_Control::show_Initial_Water_Flow()// shows initial water flow
{
    uint8_t arr[4];
    for(int i = 0; i < 4; i++)
    {
        arr[i] = this->readByteInEEPROM(Water_Flow_start_address + i);
    }
    unsigned long initial_water_flow = to_unsigned_long(arr);
    Serial.print("Initial Water Flow: ");
    Serial.println(initial_water_flow);
}

unsigned long Device_Control::total_Water_Flow()// returns total water flow
{
    uint8_t arr[4];
    for(int i = 0; i < 4; i++)
    {
        arr[i] = this->readByteInEEPROM(Water_Flow_start_address + i);
    }
    unsigned long total_water_flow = to_unsigned_long(arr);
    return total_water_flow;
}

void Device_Control::save_Water_Flow(unsigned long water_flow)// save stotal water flow in EEPROM
{
    uint8_t arr[4];
    for(int i = 0; i < 4; i++)
    {
        this->writeByteInEEPROM(Water_Flow_start_address + i, arr[i]);
    }
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
        byte val = EEPROM.read(i);
        data += (char)val;
    }
    return data;
}


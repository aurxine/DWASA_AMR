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
    // this->Device_Info.Number_of_Saved_Contacts = 0;
    // this->Device_Info.ID = "";
    if(this->Device_Info.Manufacturing)
    {
        this->Device_Info.ID = "";
        this->Device_Info.Password = "";
    }
    if(this->Device_Info.Configuration)
    {
        // this->Device_Info.Number_of_Saved_Contacts = 0;
        this->Device_Info.is_Control_Number_Set = false;
        this->Device_Info.Water_per_Pulse = 0;
        this->Device_Info.Water_Flow = 0;
    }
    // this->Get_EEPROM();

}

void Device_Control::put_Password(String pass)
{
    this->Device_Info.Password = pass;
    EEPROM.put(0, this->Device_Info);
}

void Device_Control::put_ID(String ID)// receives and saves an ID
{
    this->Device_Info.ID = ID;
    EEPROM.put(0, this->Device_Info);
    /*
    this->ID = ID;
    this->writeStringInEEPROM(ID_start_address, ID);
    */
}

void Device_Control::show_ID()//Serial prints ID
{
    EEPROM.get(0,this->Device_Info);
    Serial.print("Device ID ");
    Serial.println(this->Device_Info.ID);
    /*
    this->ID = readStringInEEPROM(ID_start_address, ID_Length);
    Serial.print("Device ID: ");
    Serial.println(this->ID);
    */
}

String Device_Control::device_ID()
{
    EEPROM.get(0, this->Device_Info);
    return this->Device_Info.ID;
    /*
    this->ID = readStringInEEPROM(ID_start_address, ID_Length);
    return ID;
    */
}

bool Device_Control::put_Contact(String number)//puts a number and appends it to the array
{
    
    
    // Serial.print("Input Number: "); Serial.println(number);
    
    EEPROM.get(0, this->Device_Info);
    if (this->Device_Info.is_Control_Number_Set)
    {

        return false;
    }
    else
    {
        Serial.println("Setting Control Number "+number);
        this->Device_Info.Control_Number = number;
        this->Device_Info.is_Control_Number_Set = 1;
        EEPROM.put(0, this->Device_Info);
        return true;
    }
    /*
    uint8_t number_of_saved_contacts = Device_Info.Number_of_Saved_Contacts;
    Serial.print("Number of already saved contacts: "); 
    Serial.println(number_of_saved_contacts);
    if(number_of_saved_contacts == Max_Number_of_Contacts)
        return false;
    Serial.print("Saving This Number: "); Serial.println(number);
    this->Device_Info.Contacts[number_of_saved_contacts] = number;
    this->Device_Info.Number_of_Saved_Contacts += 1;
    EEPROM.put(0, this->Device_Info);
    
    int number_of_saved_contacts = this->readByteInEEPROM(Number_of_saved_contacts_address);
    Serial.print("Number of already saved contacts: "); Serial.println(number_of_saved_contacts);
    if(number_of_saved_contacts == Max_Number_of_Contacts)
        return false;
    this->Contacts[number_of_saved_contacts] = number;
    this->writeStringInEEPROM(Contacts_start_address + number_of_saved_contacts*11, number);
    this->writeByteInEEPROM(Number_of_saved_contacts_address, number_of_saved_contacts + 1);
    */
    return true;
}

void Device_Control::replace_Contact(String number)//replace a contact in specified position
{   
    EEPROM.get(0, this->Device_Info);
    //this->Device_Info.Contacts[position] = number;
    this->Device_Info.Control_Number = number;
    EEPROM.put(0, this->Device_Info);
    /*
    this->writeStringInEEPROM(Contacts_start_address + position*11, number);
    */
}

bool Device_Control::check_Contact(String number)//checks whether a contact exists or not
{
    this->Device_Info = EEPROM.get(0, this->Device_Info);
    if (this->Device_Info.Control_Number == number)
    {
        return 1;
    }
    /*
    uint8_t number_of_saved_contacts = this->Device_Info.Number_of_Saved_Contacts;
    //uint8_t number_of_saved_contacts = this->readByteInEEPROM(Number_of_saved_contacts_address);
    
    for(uint8_t i = 0; i < number_of_saved_contacts; i+=1)
    {
        if(number == this->Device_Info.Contacts[i])
        {
            return 1;
        }
    }*/
    return 0;
}

void Device_Control::show_Contact(int pos)//shows contact at a specified position
{
    EEPROM.get(0, this->Device_Info);
    if (pos > 1)
    {
        Serial.println("Only Position 1 exists");
        return;
    }
    Serial.print("Contact "+String(pos)+":");
    Serial.println(this->Device_Info.Control_Number);
    // Serial.println(this->Device_Info.Contacts[pos]);
    /*
    int number_of_saved_contacts = this->readByteInEEPROM(Number_of_saved_contacts_address);
    for(int i = 0; i < number_of_saved_contacts; i++)
    {
        this->Contacts[i] = this->readStringInEEPROM(Contacts_start_address + i*11, 11);
    }
    Serial.print("Contact " + String(pos) + ": ");
    Serial.println(Contacts[pos]);
    */
}



void Device_Control::show_All_Contacts()//shows all saved contacts
{
    EEPROM.get(0, this->Device_Info);
    if (this->Device_Info.is_Control_Number_Set)
    {
        Serial.println("Control Number :"+this->Device_Info.Control_Number);
        return;
    }
    Serial.println("Control Number doesn't exist");
    return;

    // uint8_t number_of_saved_contacts = this->Device_Info.Number_of_Saved_Contacts;
    // Serial.println(number_of_saved_contacts);
    // for(uint8_t i = 0; i < number_of_saved_contacts; i++)
    // {
    //     Serial.print("Contact " + String(i + 1) + ": ");
    //     Serial.println(this->Device_Info.Contacts[i]);
    // }
    /*
    int number_of_saved_contacts = this->readByteInEEPROM(Number_of_saved_contacts_address);
    for(int i = 0; i < number_of_saved_contacts; i++)
    {
        this->Contacts[i] = this->readStringInEEPROM(Contacts_start_address + i*11, 11);
        Serial.print("Contact " + String(i + 1) + ": ");
        Serial.println(Contacts[i]);
    }
    */
}

String Device_Control::get_Contact(int pos)
{
    EEPROM.get(0, this->Device_Info);
    return this->Device_Info.Control_Number;
    /*
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
    */
}

void Device_Control::put_Water_per_Pulse(byte amount)//saves water flow per pulse data
{
    this->Device_Info = EEPROM.get(0, this->Device_Info);
    this->Device_Info.Water_per_Pulse = amount;
    EEPROM.put(0, this->Device_Info);
    /*
    Water_per_Pulse = amount;
    this->writeByteInEEPROM(Water_per_Pulse_start_address, amount);
    byte val = this->readByteInEEPROM(Water_per_Pulse_start_address);
    Serial.println(val);
    */
}

void Device_Control::show_Water_per_Pulse()//shows water per pulse data
{
    this->Device_Info = EEPROM.get(0, this->Device_Info);
    Serial.print("Water flow per pulse: ");
    Serial.println(this->Device_Info.Water_per_Pulse);
    /*
    this->Water_per_Pulse = this->readByteInEEPROM(Water_per_Pulse_start_address);
    Serial.print("Water flow per pulse: ");
    Serial.println(Water_per_Pulse);
    */
}

void Device_Control::put_Initial_Water_Flow(unsigned long water_flow)//puts and saves past water flow
{
    this->Device_Info = EEPROM.get(0, this->Device_Info);
    this->Device_Info.Water_Flow = water_flow;
    EEPROM.put(0, this->Device_Info);
    /*
    uint8_t arr[4]; // unsigned long requires 4 bytes of data
    to_int8(arr, &water_flow);
    for(int i = 0; i < 4; i++)
    {
        this->writeByteInEEPROM(Water_Flow_start_address + i, arr[i]);
    }
    */
}

void Device_Control::show_Initial_Water_Flow()// shows initial water flow
{
    this->Device_Info = EEPROM.get(0, this->Device_Info);
    Serial.print("Initial Water Flow: ");
    Serial.println(this->Device_Info.Water_Flow);
    /*
    uint8_t arr[4];
    for(int i = 0; i < 4; i++)
    {
        arr[i] = this->readByteInEEPROM(Water_Flow_start_address + i);
    }
    unsigned long initial_water_flow = to_unsigned_long(arr);
    Serial.print("Initial Water Flow: ");
    Serial.println(initial_water_flow);
    */
}

unsigned long Device_Control::total_Water_Flow()// returns total water flow
{
    this->Device_Info = EEPROM.get(0, this->Device_Info);
    return this->Device_Info.Water_Flow;
    /*
    uint8_t arr[4];
    for(int i = 0; i < 4; i++)
    {
        arr[i] = this->readByteInEEPROM(Water_Flow_start_address + i);
    }
    unsigned long total_water_flow = to_unsigned_long(arr);
    return total_water_flow;
    */
}

void Device_Control::save_Water_Flow(unsigned long water_flow)// save stotal water flow in EEPROM
{
    this->Device_Info = EEPROM.get(0, this->Device_Info);
    this->Device_Info.Water_Flow = water_flow;
    EEPROM.put(0, this->Device_Info);
    /*
    uint8_t arr[4];
    for(int i = 0; i < 4; i++)
    {
        this->writeByteInEEPROM(Water_Flow_start_address + i, arr[i]);
    }
    */
}

String Device_Control::Execute_Command(String msg, String number)
{
    String command;

    if(this->Device_Info.is_Control_Number_Set)
    {
        if(this->check_Contact(number))
            command = msg;
        
        else if(msg.substring(0, 4) == this->Device_Info.Password)
        {
            command = msg.substring(4);
        }
        else
            return "Not Allowed";
    }

    else if(this->Device_Info.is_Control_Number_Set == 0)
    {
        if(msg.substring(0, 4) == this->Device_Info.Password)
        {
            command = msg.substring(4);
        }
        else
            return "Not Allowed";
    }

    else
        return "Not Allowed";

    if(command.substring(0, 3) == "set")
    {
        if(command.substring(4,10) == "number")
        {
            if(this->put_Contact(command.substring(10)))
            {
                return "Control Number set successfully";
            }
            return "Control Number was not set";
        }

        else if(command.substring(4,9) == "water")
        {
            this->put_Initial_Water_Flow(command.substring(9).toInt());
            return "Initial Water Flow set successfully";
        }

        else if(command.substring(4,8) == "flow")
        {
            this->put_Water_per_Pulse(command.substring(8).toInt());
            return "Flow per Pulse set successfully";
        }
        else
            return "Invalid Command";
    }

    else if(command.substring(0, 3) == "add")
    {
        if(command.substring(4,10) == "number")
        {
            if(this->put_Contact(command.substring(10)))
            {
                return "New Control Number added successfully";
            }
            return "New Control Number was not added";
        }
    }

    else if(command.substring(0, 7) == "replace")
    {
        this->replace_Contact(command.substring(10));
        return "New Control Number was replaced at position " + command[8];

        // return "New Control Number was not replaced";
    }

    else if(command.substring(0, 5) == "query")
    {
        String message = "AMR:";
        if(command.substring(6) == "water")
        {
            message += String(this->total_Water_Flow());
            Serial.println("debug" + message);
            return message;
        }

        else if(command.substring(6) == "flow")
        {
            this->Update_EEPROM();
            message += String(this->Device_Info.Water_per_Pulse);
            return message;
        }

        else if(command.substring(6) == "ID")
        {
            message += this->Device_Info.ID;
            return message;
        }

        else if(command.substring(6) == "password")
        {
            message += this->Device_Info.Password;
            return message;
        }

        else if(command.substring(6, 12) == "number")
        {
            message += this->get_Contact((int)command[13] - 48);
            return message;
        }

        else
            return "Invalid Command";
    }

    else if(command.substring(0, 5) == "reset")
    {
        // this->Device_Info.Initial_Water_Flow = 0;
        this->Device_Info.Water_per_Pulse = 0;
        this->Device_Info.Water_Flow = 0;
        this->Device_Info.Configuration = true;
        this->Update_EEPROM();
        this->Get_EEPROM();
        return "Reset successfully";
    }

    
    return "Invalid Command";
}

void Device_Control::Update_EEPROM()
{
    EEPROM.put(0, this->Device_Info);
}

void Device_Control::Get_EEPROM()
{
    EEPROM.get(0, this->Device_Info);
}

void Device_Control::Reset()
{
    
    this->Device_Info.is_Control_Number_Set = 0;
    EEPROM.put(0,this->Device_Info);
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


#include<Arduino.h>
#include<EEPROM.h>
#include<SoftwareSerial.h>
#include"Device_Control.h"
#include"GSM.h"
//#include"Device_Control.cpp"
// this pin will be attached to the reed switch pin
#define reed_switch_pin 2 

//ms before interrupt is detected again
// used to debounce
#define bounce_time 200

// This pin will be pulled up so that it provides 5V output
// When it iss connected to the sensor it will read 0V input
// When the wire is cut it will read 5V (HIGH) input
#define wire_cut_detect_pin 3
bool is_wire_cut = false;


// This will be used for indication of different states
#define indicator_LED 7


//SoftwareSerial SIM800L(8, 9); // new (Rx, Tx) of pro mini
GSM SIM(8,9);
Device_Control Pro_Mini;
SString Message;

// this variable will count the meter pulses
// when a certain amount of water passes, the meter will send a pulse through reed switch
unsigned long int counter = 0;

// Changing line

typedef struct info_type_struct{
    String number1;
}info_type;



// Changing end line
void Pulse_Counter()
{
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();

    if(interrupt_time - last_interrupt_time > bounce_time)
    {
        counter++;
    }

    last_interrupt_time = interrupt_time;
  
}

void Detect_Wire_Cut()
{
    if(digitalRead(wire_cut_detect_pin) == HIGH)
        is_wire_cut = true;
    else
        is_wire_cut = false;
}

void reset()
{
    // counter and total water will be reseted
}


void Blink_LED(int number_of_times, int Delay)
{
    if(number_of_times == 0 && Delay != 0)
    {
        digitalWrite(indicator_LED, LOW);
    }

    else if(Delay == 0)
    {
        digitalWrite(indicator_LED, HIGH);
    }

    else if( number_of_times != 0 && Delay != 0)
    {
        for(int i = 0; i < number_of_times; i++)
        {
            digitalWrite(indicator_LED, HIGH);
            delay(Delay);
            digitalWrite(indicator_LED, LOW);
            delay(Delay);
        }
    }
    
}

bool Get_ID_Pass()
{
    String id_and_pass;
    Serial.println("ready");
    delay(500);
    id_and_pass = Serial.readString();
    delay(500);

    id_and_pass = "12341103202102100001";
    
    int id_and_pass_length = id_and_pass.length();

    if(id_and_pass_length != 20)
    {
        // Serial.println("Unsuccessful");
        return false;
    }

    else
    {
        Serial.println("successful");
        String id, pass;
        pass = id_and_pass.substring(0, 4);
        id = id_and_pass.substring(4);
        Pro_Mini.put_Password(pass);
        Pro_Mini.put_ID(id);
        return true;
    }
}

/*
typedef struct Device_Info_Structures
{
    bool Manufacturing;
    bool Configuration;
    String Password;
    String ID;
    String Contacts[Max_Number_of_Contacts];
    uint8_t Number_of_Saved_Contacts;
    unsigned long Water_Flow;
    // unsigned long Initial_Water_Flow;
    int Water_per_Pulse;

}Device_Info_Types;

*/
void setup() 
{
    Serial.begin(9600);
    //SIM800L.begin(9600);

    //SIM800L.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
    SIM.begin(9600);
    delay(1000);
    /*
    Device_Info_Types devinfo;

    devinfo.Contacts[0] = "01624593436";
    devinfo.ID = "1234567890";
    devinfo.Password = "1234";
    devinfo.Water_per_Pulse = 100;
    devinfo.Water_Flow = 12345;

    // EEPROM.put(0, devinfo);

    Device_Info_Types devinfo2;

    EEPROM.get(0, devinfo2);

    Serial.println(devinfo2.Contacts[0]);
    Serial.println(devinfo2.ID);
    Serial.println(devinfo2.Password);
    Serial.println(devinfo2.Water_per_Pulse);
    Serial.println(devinfo2.Water_Flow);

*/
    
//     //Serial.write ("Will read Message");

//     pinMode(reed_switch_pin, INPUT_PULLUP);
//     pinMode(wire_cut_detect_pin, INPUT);
//     pinMode(reed_switch_pin, INPUT_PULLUP);
//     pinMode(indicator_LED, OUTPUT);

//     attachInterrupt(digitalPinToInterrupt(reed_switch_pin), Pulse_Counter, FALLING);
//     attachInterrupt(digitalPinToInterrupt(wire_cut_detect_pin), Detect_Wire_Cut, CHANGE);

//     delay(1000);
    
//     Blink_LED(1, 0);

//     // Pro_Mini.Device_Info.Manufacturing = true;
//     // Pro_Mini.Device_Info.Configuration = true;
//     // Pro_Mini.Update_EEPROM();
//     // Pro_Mini.Get_EEPROM();
//     Serial.println(Pro_Mini.Device_Info.Manufacturing);
//     Serial.println(Pro_Mini.Device_Info.Configuration);
//     /*
//     while (Pro_Mini.Device_Info.Manufacturing)
//     {
//         if(Get_ID_Pass())
//         {
//             Pro_Mini.Device_Info.Manufacturing = false;
//             Pro_Mini.Update_EEPROM();
//             break;
//         }
//     }
//     */
//     Blink_LED(5, 150);
//     /*
//     while (Pro_Mini.Device_Info.Configuration)
//     {
//         Blink_LED(1, 100);
//         Serial.println("Waiting for configuration");
//         Message = SIM.ReceiveMessage();
//         Serial.println(Message.text);
//         if(Message.text.length() > 1)
//         {
//             String response = Pro_Mini.Execute_Command(Message.text, Message.number);
//             Serial.println(response);
//             delay(1000);
//             SIM.SendMessage(response, Message.number);
            
//         }
//         delay(1000);
//         // stays here till control numbers are set
//     }
//     */
    
    // Pro_Mini.put_ID("11032020150001");
    // String id = Pro_Mini.ID;
    // Serial.println(id);
    Pro_Mini.show_ID();

    // Pro_Mini.put_Water_per_Pulse(100);
    Pro_Mini.show_Water_per_Pulse();
    /*
    Pro_Mini.put_Contact("01624593436");
    Pro_Mini.put_Contact("01689294634");
    Pro_Mini.put_Contact("01312593436");
    Pro_Mini.put_Contact("01712593436");
    */
    Pro_Mini.Get_EEPROM();
    Pro_Mini.show_All_Contacts();

    Serial.println("Checked");
//     // Serial.println(Pro_Mini.Device_Info.Contacts[0]);
//     /*/ Changing line
//     info_type info;
//     info.number1 = "01521327794";
//     EEPROM.put(0, info);
//     info_type read_info = EEPROM.get(0, info);
//     Serial.println(read_info.number1);
//     info.number1 = "01521327794 new";
//     EEPROM.put(0, info);
//     read_info = EEPROM.get(0, info);
//     Serial.println(read_info.number1);

//     // Changing line end */
//     // if(Pro_Mini.check_Contact("01324593436"))
//     // {
//     //     Serial.println("Contact Available");
//     // }
//     // else
//     // {
//     //     Serial.println("Contact not available");
//     // }
    
//     // SString msg;
//     // msg = SIM.ReceiveMessage();
//     // Serial.println(msg.text);
//     // Serial.println(msg.number);
//     // Serial.println("End");
//     // Pro_Mini.put_Initial_Water_Flow(2297);
//     // Pro_Mini.show_Initial_Water_Flow();

//     // Pro_Mini.put_Water_per_Pulse(10);
//     // Pro_Mini.show_Water_per_Pulse();
//     Serial.print("Size of structure: ");
//     Serial.println(sizeof(Pro_Mini.Device_Info));


}

void loop() 
{
    // bool was_wire_cut = detect_wire_cut();
    // delay(500);
    // Serial.println(counter);
}
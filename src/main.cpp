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

// this pin will be attached to a wire of which one end will be high
// this is used for detecting if the sensor was cut
// when the sensor wire is cut, this wire will also be cut
// thus generating a signal
#define wire_cut_detect_pin A0

// this pin will be used for hardware reset
#define reset_pin 3

// This will be used for indication of different states
#define indicator_LED 7


//SoftwareSerial SIM800L(8, 9); // new (Rx, Tx) of pro mini
GSM SIM(8,9);
Device_Control Pro_Mini;

// this variable will count the meter pulses
// when a certain amount of water passes, the meter will send a pulse through reed switch
unsigned long int counter = 0;

// Changing line

typedef struct info_type_struct{
    String number1;
}info_type;



// Changing end line
void pulse_counter()
{
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();

    if(interrupt_time - last_interrupt_time > bounce_time)
    {
        counter++;
    }

    last_interrupt_time = interrupt_time;
  
}

bool detect_wire_cut()
{
    int check_wire_voltage = analogRead(wire_cut_detect_pin);
    //Serial.println(check_wire_voltage);
    if(check_wire_voltage < 1000)
    {
        //Serial.println("Wire is cut");
        return true;
    }
    return false;
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

bool Get_ID()
{
    String id;
    Serial.println("ready");
    delay(500);
    id = Serial.readString();
    delay(500);
    
    int id_length = id.length();

    if(id_length != 16)
    {
        // Serial.println("Unsuccessful");
        return false;
    }

    else
    {
        Serial.println("successful");
        Pro_Mini.put_ID(id);
        return true;
    }
}


void setup() 
{
    Serial.begin(9600);
    //SIM800L.begin(9600);

    //SIM800L.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
    SIM.begin(9600);
    delay(1000);

    
    //Serial.write ("Will read Message");

    pinMode(reed_switch_pin, INPUT_PULLUP);
    pinMode(wire_cut_detect_pin, INPUT);
    pinMode(reed_switch_pin, INPUT_PULLUP);
    pinMode(indicator_LED, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(reed_switch_pin), pulse_counter, FALLING);
    attachInterrupt(digitalPinToInterrupt(reset_pin), reset, FALLING);

    delay(1000);
    
    Blink_LED(1, 0);

    // Pro_Mini.Device_Info.Manufacturing = true;
    // Pro_Mini.Device_Info.Configuration = true;
    // Pro_Mini.Update_EEPROM();
    Pro_Mini.Get_EEPROM();
    // Serial.println(Pro_Mini.Device_Info.Manufacturing);
    // Serial.println(Pro_Mini.Device_Info.Configuration);

    while (Pro_Mini.Device_Info.Manufacturing)
    {
        // Serial.println("in the while");
        // Blink_LED(2, 100);
        if(Get_ID())
        {
            break;
        }
        // Blink_LED(1, 0);
    }
    Pro_Mini.Device_Info.Manufacturing = false;
    Pro_Mini.Update_EEPROM();
    Blink_LED(5, 150);

    while (Pro_Mini.Device_Info.Configuration)
    {
        // stays here till control numbers are set
    }
    
    // Pro_Mini.put_ID("2020150001");
    // String id = Pro_Mini.ID;
    // Serial.println(id);
    Pro_Mini.show_ID();

    //Pro_Mini.put_Water_per_Pulse(100);
    Pro_Mini.show_Water_per_Pulse();

    Pro_Mini.put_Contact("01624593436");
    Pro_Mini.put_Contact("01689294634");
    Pro_Mini.put_Contact("01312593436");
    Pro_Mini.show_All_Contacts();

    Serial.println();
    Serial.println(Pro_Mini.Device_Info.Contacts[0]);
    /*/ Changing line
    info_type info;
    info.number1 = "01521327794";
    EEPROM.put(0, info);
    info_type read_info = EEPROM.get(0, info);
    Serial.println(read_info.number1);
    info.number1 = "01521327794 new";
    EEPROM.put(0, info);
    read_info = EEPROM.get(0, info);
    Serial.println(read_info.number1);

    // Changing line end */
    if(Pro_Mini.check_Contact("01324593436"))
    {
        Serial.println("Contact Available");
    }
    else
    {
        Serial.println("Contact not available");
    }
    
    SString msg;
    msg = SIM.ReceiveMessage();
    Serial.println(msg.text);
    Serial.println(msg.number);
    Serial.println("End");
    // Pro_Mini.put_Initial_Water_Flow(2297);
    // Pro_Mini.show_Initial_Water_Flow();

    // Pro_Mini.put_Water_per_Pulse(10);
    // Pro_Mini.show_Water_per_Pulse();


}

void loop() 
{
    // bool was_wire_cut = detect_wire_cut();
    // delay(500);
    // Serial.println(counter);
}
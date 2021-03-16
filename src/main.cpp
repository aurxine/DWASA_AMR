#include<Arduino.h>
#include<EEPROM.h>
#include<SoftwareSerial.h>
#include"Device_Control.h"
#include"GSM.h"
//#include"Device_Control.cpp"
// this pin will be attached to the reed switch pin
#define pro_mini_restart_pin 12 // used to reset or restart the pro mini
#define gsm_restart_pin 6 // used to restart GSM module
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

unsigned long startMillis, currentMillis;

// Changing end line
void Pulse_Counter()
{
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();

    if(interrupt_time - last_interrupt_time > bounce_time)
    {
        counter++;
        Pro_Mini.Device_Info.Water_Flow += Pro_Mini.Device_Info.Water_per_Pulse; 
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

void Reset_Pro_mini()
{
    digitalWrite(pro_mini_restart_pin, LOW);
}

void Reset_GSM()
{
    digitalWrite(gsm_restart_pin, LOW);
    delay(100);
    digitalWrite(gsm_restart_pin, HIGH);
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

    // id_and_pass = "12341103202102100001";
    
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


void setup() 
{
    digitalWrite(pro_mini_restart_pin, HIGH);
    digitalWrite(gsm_restart_pin, HIGH);
    delay(100);

    pinMode(pro_mini_restart_pin, OUTPUT);
    pinMode(gsm_restart_pin, OUTPUT);

    Serial.begin(9600);

    SIM.begin(9600);
    delay(1000);
    
    pinMode(wire_cut_detect_pin, INPUT_PULLUP);
    pinMode(reed_switch_pin, INPUT_PULLUP);
    pinMode(indicator_LED, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(reed_switch_pin), Pulse_Counter, FALLING);
    attachInterrupt(digitalPinToInterrupt(wire_cut_detect_pin), Detect_Wire_Cut, CHANGE);

    
    //Serial.write ("Will read Message");

    pinMode(reed_switch_pin, INPUT_PULLUP);
    pinMode(wire_cut_detect_pin, INPUT);
    pinMode(reed_switch_pin, INPUT_PULLUP);
    pinMode(indicator_LED, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(reed_switch_pin), Pulse_Counter, FALLING);
    attachInterrupt(digitalPinToInterrupt(wire_cut_detect_pin), Detect_Wire_Cut, CHANGE);

    delay(1000);
    
    Blink_LED(1, 0);


    while (Pro_Mini.Device_Info.Configuration)
    {
        Pro_Mini.Get_EEPROM();
        // Serial.println(Pro_Mini.Device_Info.Number_of_Saved_Contacts);
        Serial.println(Pro_Mini.Device_Info.Water_Flow);
        Serial.println(Pro_Mini.Device_Info.Water_per_Pulse);
        // if(Pro_Mini.Device_Info.Number_of_Saved_Contacts > 0)
        // {
        //     if(Pro_Mini.Device_Info.Water_Flow > 0 && Pro_Mini.Device_Info.Water_per_Pulse > 0)
        //     {
        //         Pro_Mini.Device_Info.Configuration = false;
        //         Pro_Mini.Update_EEPROM();
        //         break;
        //     }
        // }

        Blink_LED(1, 100);
        Serial.println("Waiting for configuration");
        Message = SIM.ReceiveMessage();
        Serial.println(Message.text);
        if(Message.text.length() > 1)
        {
            String response = Pro_Mini.Execute_Command(Message.text, Message.number);
            Serial.println(response);
            delay(1000);
            SIM.SendMessage(response, Message.number);
        }
        delay(1000);
        // stays here till control numbers are set
    }
    
    Blink_LED(5, 100);
    
    // Pro_Mini.put_ID("2020150001");

    // String id = Pro_Mini.ID;
    // Serial.println(id);
    Pro_Mini.Get_EEPROM();
    Pro_Mini.show_ID();

    Pro_Mini.put_Water_per_Pulse(100);
    Pro_Mini.show_Water_per_Pulse();

    Pro_Mini.show_All_Contacts();
    Pro_Mini.replace_Contact("01689294634");
    startMillis = millis();
    

}

void loop() 
{
    currentMillis = millis();
    Serial.println(currentMillis);
    delay(250);
    if (currentMillis - startMillis >= 2500)  //test whether the period has elapsed
    {
        Serial.println("Resetting");
        Reset_GSM();
        delay(1000);

        Serial.println("Resetting finished");
        Reset_Pro_mini();  //if so, restart the promini
        startMillis = currentMillis;  //IMPORTANT to save the start time of the current 
    }
    
    /*
    if(is_wire_cut)
    {
        // SIM.SendMessage("Sensor wire is cut!", Pro_Mini.Device_Info.Contacts[0]);
        is_wire_cut = false;
    }

    Message = SIM.ReceiveMessage();

     = Pro_Mini.Execute_Command(Message.text, Message.number);
        Serial.println(response);
        delay(1000);
        SIM.SendMessage(response, Message.number);
    }if(Message.text.length() > 1)
    {
        String response

    Serial.print("Count: ");
    Serial.println(counter);
    Pro_Mini.Update_EEPROM();
    delay(500);
    */
}
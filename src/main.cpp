#include <Arduino.h>
#include<EEPROM.h>
#include<SoftwareSerial.h>
#include"Device_Control.h"
#include"Device_Control.cpp"
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


SoftwareSerial SIM800L(8, 9); // new (Rx, Tx) of pro mini

Device_Control Pro_Mini;

// this variable will count the meter pulses
// when a certain amount of water passes, the meter will send a pulse through reed switch
unsigned long int counter = 0;


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


void setup() 
{
    Serial.begin(9600);
    SIM800L.begin(9600);

    SIM800L.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
    delay(1000);
    //Serial.write ("Will read Message");

    pinMode(reed_switch_pin, INPUT_PULLUP);
    pinMode(wire_cut_detect_pin, INPUT);
    pinMode(reed_switch_pin, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(reed_switch_pin), pulse_counter, FALLING);
    attachInterrupt(digitalPinToInterrupt(reset_pin), reset, FALLING);

    delay(1000);

    //Pro_Mini.put_ID("2020150001");
    // String id = Pro_Mini.ID;
    // Serial.println(id);
    Pro_Mini.show_ID();

    //Pro_Mini.put_Water_per_Pulse(100);
    Pro_Mini.show_Water_per_Pulse();

    Pro_Mini.put_Contact("01624593436");
    Pro_Mini.put_Contact("01689294634");
    Pro_Mini.put_Contact("01312593436");
    Pro_Mini.show_All_Contacts();
    Pro_Mini.replace_Contact("01789294634", 1);
    Pro_Mini.show_All_Contacts();

    Pro_Mini.put_Initial_Water_Flow(2297);
    Pro_Mini.show_Initial_Water_Flow();

    Pro_Mini.put_Water_per_Pulse(10);
    Pro_Mini.show_Water_per_Pulse();


}

void loop() 
{
    // bool was_wire_cut = detect_wire_cut();
    // delay(500);
    // Serial.println(counter);
}
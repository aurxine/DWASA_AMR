#include <Arduino.h>
#include<EEPROM.h>
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

#define EEPROM_size 256

// this variable will count the meter pulses
// when a certain amount of water passes, the meter will send a pulse through reed switch
unsigned long int counter = 0;





// byte readByteInEEPRPOM(int address)
// {
//     EEPROM.begin();
//     byte data;
//     data = EEPROM.read(address);
//     //Serial.println("Data: "+String(data));
//     return data;
// }

// String readStringInEEPRPOM(int init_addr, int size)
// {
//     EEPROM.begin();
//     String str;
//     byte data;
//     for(int i=init_addr; i<(size + init_addr); i++)
//     {
//         data = EEPROM.read(i);
//         //Serial.println((char)data);
//         str+=(char)data;
//     }
//     //Serial.println("String: " + str);
//     return str;
// }

// int writeByteInEEPROM(int init_addr, byte data)
// {
//     EEPROM.begin();
//     EEPROM.write(init_addr, data);
//         if(EEPROM.commit())
//         {
//             Serial.println("EEPROM successfully committed");
//         }
//         else 
//         {
//             Serial.println("ERROR! EEPROM commit failed");
//             return -1;
//         }
//     return ++init_addr; //new address
// }

// int writeStringInEEPROM(int init_addr, int size, String data)
// {
//     EEPROM.begin(EEPROM_size);
//     int i, j=0;
//     //Serial.println("size"+String(size));
//     for(i = init_addr; i<(size + init_addr); i++,j++)
//     {
//         EEPROM.write(i, data[j]);
//         //Serial.println(data[j]);
//         if(EEPROM.commit())
//         {
//             Serial.println("EEPROM successfully committed");
//         }
//         else 
//         {
//             Serial.println("ERROR! EEPROM commit failed");
//             return -1;
//         }
//     }
//     return i; //new address
// }

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

}

void setup() 
{
  Serial.begin(9600);

  pinMode(reed_switch_pin, INPUT_PULLUP);
  pinMode(wire_cut_detect_pin, INPUT);
  pinMode(reed_switch_pin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(reed_switch_pin), pulse_counter, FALLING);
  attachInterrupt(digitalPinToInterrupt(reset_pin), reset, FALLING);
  
}

void loop() 
{
    bool was_wire_cut = detect_wire_cut();
    delay(500);
    Serial.println(counter);
}
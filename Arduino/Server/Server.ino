#include <Console.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#define BAUDRATE 115200

RH_RF95 rf95;

int led = A2;
float frequency = 433.0;

void setup() {
    pinMode(led, OUTPUT);
    /*
    All the commented parts must be inverted tu be used to visualize 
    data on the arduino IDE (bridge and console) or with the C app (Serial)     
    */
    // Bridge.begin(BAUDRATE); 
    Serial.begin(9600);
    Serial.println("Start Sketch");
    // Console.begin();
    // Console.println("Start Sketch");

    if (!rf95.init())
        Serial.println("init failed");
        // Console.println("init failed");
    rf95.setFrequency(frequency);
    rf95.setTxPower(13);
}

void loop(){
    StaticJsonBuffer<200> jsonBuffer; 
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    const char ID[RH_RF95_MAX_MESSAGE_LEN];
    const char sendbuf[RH_RF95_MAX_MESSAGE_LEN];

    String Rcvbuff;

    if (rf95.available()){ 
        if (rf95.recv(buf, &len)){ //message recepetion and saving in the buffer
            digitalWrite(led, HIGH);
            Serial.println((char*)buf);
            // Console.println((char*)buf);

            digitalWrite(led, LOW);

            /*
            Waiting for a dowlink command to be received
            If received, instant sending to the end devices
            Command format : {"ID":"arduino2","actuator":"LED","state":1}
            */

            Rcvbuff = Serial.readString();
            // Rcvbuff = Console.readString();
            if(Rcvbuff!=NULL){
                Rcvbuff.toCharArray(sendbuf, sizeof(sendbuf));
                Serial.print("sending:");
                Serial.println(sendbuf);
                // Console.print("sending:");
                // Console.println(sendbuf);

                rf95.send((const uint8_t *)sendbuf, sizeof(sendbuf));
                rf95.waitPacketSent();
            }
        }
        else{
            Serial.println("recv failed");
            // Console.println("recv failed");
        }
    }
}



